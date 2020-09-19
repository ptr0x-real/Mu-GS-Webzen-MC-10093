// MonsterHerd.cpp: implementation of the MonsterHerd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef MONSTER_HERD_SYSTEM_20031120

#include "MonsterHerd.h"

#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "..\\Common\\winutil.h"
#include "..\\Include\\GMProtocol.h"
#include "..\\Include\\Public.h"
#include "..\\include\\cb_protocol.h"
#include "..\\Include\\public.h"
#include "..\\Include\\Readscript.h"

#include "User.h"
#include "NpcTalk.h"
#include "protocol.h"

#include "wsJoinServerCli.h"

#include "MonsterSetBase.h"
#include "GameMain.h"
#include "PartyClass.h"
#include "wzUdp.h"
#include "DSProtocol.h"

#include "MapClass.h"
#include "GameServer.h"
#include "DirPath.h"
#include "MonsterAttr.h"
#include "..\\Common\\Classdef.h"

extern CMonsterAttr	gMAttr;

enum {
	EMOTION_NORMAL	=0,		// ���� - ����
	EMOTION_ANGRY	=1,		// ���� - ���
	EMOTION_FLEE	=2,		// ���� - ���� 
	EMOTION_HALT	=3,		// ���� - ����޽�
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


MonsterHerd::MonsterHerd()
{
	m_iMapNumber = -1;
	m_iCUR_X = 0;
	m_iCUR_Y = 0;
	m_iRADIUS = 0;

	m_bHerdActive = FALSE;
	m_bHasInfo = FALSE;

	InitializeCriticalSection (& m_critMonsterHerd);
}


MonsterHerd::~MonsterHerd()
{
	DeleteCriticalSection (& m_critMonsterHerd);
}


BOOL MonsterHerd::SetTotalInfo (
	INT iMapNumber, 
	INT iRadius,
	INT iStartX,
	INT iStartY
	)
{
	if (!CHECK_IN_MAP(iMapNumber))				// �ʹ��� �ȿ� �����ϴ��� �˻�
		return FALSE;

	if (!CHECK_LIMIT(iStartX,255) ||			// ���� ��ǥ�� �˻�
		!CHECK_LIMIT(iStartY,255)
		)
		return FALSE;

	BYTE btMapAttr = MapC[iMapNumber].GetAttr(iStartX, iStartY);
	if( ((btMapAttr&MAP_ATTR_SAFTYZONE)==MAP_ATTR_SAFTYZONE) ||		// ��������
		((btMapAttr&MAP_ATTR_BLOCK)==MAP_ATTR_BLOCK) ||				// ��
		((btMapAttr&MAP_ATTR_HOLLOW)==MAP_ATTR_HOLLOW)				// ���ո�
		)
	{
		return FALSE;
	}

	if (!CHECK_LIMIT(iRadius, MAX_RADIUS))		// �̵� �ݰ� �˻�
		return FALSE;

	m_iMapNumber = iMapNumber;
	m_iCUR_X = iStartX;
	m_iCUR_Y = iStartY;
	m_iRADIUS = iRadius;

	m_bHasInfo = TRUE;			// ���ۿ� �ʿ��� ������ ����

	return TRUE;
}


BOOL MonsterHerd::AddMonster(INT iMonsterType, BOOL bRegen, BOOL bAttackFirst)
{
	// ���� ������ �����ϱ� ���� ���͸� ä���־�� �Ѵ�.

	if (!m_bHasInfo)		// ������ ������ ���� �ʴٸ� ����
		return FALSE;

	INT iIndex;	

	BYTE btMapNumber = m_iMapNumber;

	UCHAR cX = 0;
	UCHAR cY = 0;

	if (!GetRandomLocation(cX, cY))
		return FALSE;

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
	iIndex = gObjAddMonster(m_iMapNumber);
#else
	iIndex = gObjAdd();
#endif
	if( iIndex >= 0 )
	{
		gObj[iIndex].m_PosNum   = -1;
		gObj[iIndex].X			= cX;
		gObj[iIndex].Y			= cY;
		gObj[iIndex].MapNumber	= m_iMapNumber;
		gObj[iIndex].TX			= gObj[iIndex].X;
		gObj[iIndex].TY			= gObj[iIndex].Y;
		gObj[iIndex].m_OldX		= gObj[iIndex].X;
		gObj[iIndex].m_OldY		= gObj[iIndex].Y;
		gObj[iIndex].StartX     = (BYTE)gObj[iIndex].X;
		gObj[iIndex].StartY     = (BYTE)gObj[iIndex].Y;

		LPMONSTER_ATTRIBUTE		lpMATTR;
		lpMATTR = gMAttr.GetAttr(iMonsterType);

		if (lpMATTR == NULL) {
			gObjDel (iIndex);
			return FALSE;
		}

		gObj[iIndex].Level     = lpMATTR->m_Level;


		gObjSetMonster(iIndex, iMonsterType);

		gObj[iIndex].MaxRegenTime = 1000;
		gObj[iIndex].Dir = rand()%8;	// ������ �����ϰ� ���Ѵ�.

		gObj[iIndex].m_bIsInMonsterHerd = true;
		gObj[iIndex].m_bIsMonsterAttackFirst = bAttackFirst;
		gObj[iIndex].m_lpMonsterHerd = this;

		EnterCriticalSection(& m_critMonsterHerd);
		MONSTER_HERD_DATA pMHD;
		pMHD.m_iIndex = iIndex;
		pMHD.m_iType = iMonsterType;
		pMHD.m_iX = cX;
		pMHD.m_iX = cY;
		pMHD.m_bRegen = bRegen;
		m_mapMonsterHerd.insert(pair<INT, MONSTER_HERD_DATA> (iIndex, pMHD));
		LeaveCriticalSection(& m_critMonsterHerd);
	}
	else 
		return FALSE;
	
	return TRUE;
}


VOID MonsterHerd::SetRadius (				// ������ �̵��� �������� ����
	INT	iRadius
	)
{
	if(CHECK_LIMIT(iRadius, MAX_RADIUS))
		return;

	m_iRADIUS = iRadius;
}


VOID MonsterHerd::SetPosition (				// ������ �̵��� ��ǥ�� ����
	UCHAR iTX,
	UCHAR iTY
	)
{
	if (!m_bHasInfo)
		return;

	BYTE btMapAttr = MapC[m_iMapNumber].GetAttr(iTX, iTY);
	if( ((btMapAttr&0x01)==0x01) ||				// ��������
		((btMapAttr&0x04)==0x04) ||				// ��
		((btMapAttr&0x08)==0x08)				// ���ո�
		)
	{
		return;
	}

	m_iCUR_X = iTX;
	m_iCUR_Y = iTY;
}


BOOL MonsterHerd::Start()
{
	if (m_bHerdActive)
		return FALSE;

	if (!m_bHasInfo)
		return FALSE;

	m_bHerdActive = TRUE;

	return TRUE;
}


VOID MonsterHerd::Stop()						// ������ �ػ� (����) ��Ű�� ��� �ڿ��� �ʱ�ȭ �Ѵ�.
{
	m_bHerdActive = FALSE;
	
	m_bHasInfo	  = FALSE;						//20081007 ���ۿ� �ʿ��� �������� �����ϴ��� Ȯ�� �ʱ�ȭ (grooving)
	
	if (!m_mapMonsterHerd.empty()) {
		EnterCriticalSection(& m_critMonsterHerd);
		if (m_mapMonsterHerd.empty()) {
			LeaveCriticalSection(& m_critMonsterHerd);
			return;
		}

		map <INT, MONSTER_HERD_DATA>::iterator it = m_mapMonsterHerd.begin();

		for (; it!=m_mapMonsterHerd.end() ; it++) {
			MONSTER_HERD_DATA & pMH_DATA = it->second;
			gObjDel (pMH_DATA.m_iIndex);		// ������ �����ߴ� ������ ������ ������Ų��.
		}

		m_mapMonsterHerd.erase(m_mapMonsterHerd.begin(), m_mapMonsterHerd.end());

		LeaveCriticalSection(& m_critMonsterHerd);
	}
}


LPMONSTER_HERD_DATA MonsterHerd::GetMonsterData(INT iIndex)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return NULL;

	LPOBJECTSTRUCT lpOBJ = &gObj[iIndex];

	if (lpOBJ->Connected < 3 || lpOBJ->Type != OBJTYPE_MONSTER)
		return NULL;

	MonsterHerd * lpMH = lpOBJ->m_lpMonsterHerd;

	if (lpMH == NULL)
		return NULL;

	LPMONSTER_HERD_DATA lpMHD_RETURN = NULL;

	EnterCriticalSection(& lpMH->m_critMonsterHerd);

	map<INT, MONSTER_HERD_DATA>::iterator it = lpMH->m_mapMonsterHerd.find(iIndex);

	if (it != lpMH->m_mapMonsterHerd.end()) {
		lpMHD_RETURN =  &(it->second);
	}

	LeaveCriticalSection(& lpMH->m_critMonsterHerd);
		
	return lpMHD_RETURN;
}


VOID MonsterHerd::BeenAttacked (	// ���� ������ ������ �Ͽ����� ���� ���¸� �������ִ� �޼���
	LPOBJECTSTRUCT lpObj,			// ��
	LPOBJECTSTRUCT lpTargetObj		// ���� ������ �ΰ�?
	)
{
	if (lpObj == NULL)
		return;

	if (lpObj->Connected < 3 || lpObj->Type != OBJTYPE_MONSTER)
		return;

	if (lpTargetObj->Connected < 3)
		return;

	if (m_bHerdActive == FALSE)
		return;

	map <INT, MONSTER_HERD_DATA>::iterator it = m_mapMonsterHerd.begin();

	for (;it != m_mapMonsterHerd.end();it++) {
		MONSTER_HERD_DATA & pMHD = it->second;

		if (!CHECK_LIMIT(pMHD.m_iIndex, MAX_OBJECT))
			continue;

		if( gObj[pMHD.m_iIndex].Live == 0 ||
			gObj[pMHD.m_iIndex].m_State != OBJST_PLAYING ) 
			continue;

		if (gObj[pMHD.m_iIndex].m_lpMonsterHerd != lpObj->m_lpMonsterHerd)
			continue;

		if (pMHD.m_iIndex == lpObj->m_Index)	// ������ �ڽ��� ����
			continue;

		if (gObj[pMHD.m_iIndex].TargetNumber == -1) {
			// ���� �ٸ� �ɸ��� ���� ���� �ƴϾ���.
			if(rand()%100 < 70) {		// 70% Ȯ���� ����� �Ѵ�.
				gObj[pMHD.m_iIndex].TargetNumber = lpTargetObj->m_Index;
				gObj[pMHD.m_iIndex].m_ActState.Emotion = 1;
				gObj[pMHD.m_iIndex].m_ActState.EmotionCount = 50;
				continue;
			}
		}
		else {
			// ���� �ٸ� �ɸ��� ���� ���̾���.
			if (rand()%100 < 30) {		// 30% Ȯ���� �����ڸ� �ٲ۴�.
				gObj[pMHD.m_iIndex].TargetNumber = lpTargetObj->m_Index;
				gObj[pMHD.m_iIndex].m_ActState.Emotion = 1;
				gObj[pMHD.m_iIndex].m_ActState.EmotionCount = 50;
				continue;
			}
		}
	}
}


VOID MonsterHerd::OrderAttack (					// Ư�� �ΰ�? �� �����϶�� ����
	LPOBJECTSTRUCT lpObj,						// ��
	LPOBJECTSTRUCT lpTargetObj,					// ���� ������ �ΰ�?
	INT iAttackPercent							// ���������� ���ÿ� ���� Ȯ��
	)
{
	if (lpObj == NULL)
		return;

	if (lpObj->Connected < 3 || lpObj->Type != OBJTYPE_MONSTER)
		return;

	if (lpTargetObj->Connected < 3)
		return;

	if (m_bHerdActive == FALSE)
		return;

	if (!CHECK_LIMIT(iAttackPercent, 101))
		return;

	map <INT, MONSTER_HERD_DATA>::iterator it = m_mapMonsterHerd.begin();

	for (;it != m_mapMonsterHerd.end();it++) {
		MONSTER_HERD_DATA & pMHD = it->second;

		if (!CHECK_LIMIT(pMHD.m_iIndex, MAX_OBJECT))
			continue;

		if( gObj[pMHD.m_iIndex].Live == 0 ||
			gObj[pMHD.m_iIndex].m_State != OBJST_PLAYING ) 
			continue;

		if (gObj[pMHD.m_iIndex].m_lpMonsterHerd != lpObj->m_lpMonsterHerd)
			continue;

		if (pMHD.m_iIndex == lpObj->m_Index)	// ������ �ڽ��� ����
			continue;

		if(rand()%100 < iAttackPercent) {		// Ư�� Ȯ���� ����� �Ѵ�. -> ����
			gObj[pMHD.m_iIndex].TargetNumber = lpTargetObj->m_Index;
			gObj[pMHD.m_iIndex].m_ActState.Emotion = 1;
			gObj[pMHD.m_iIndex].m_ActState.EmotionCount = 50;
			continue;
		}
	}
}


BOOL MonsterHerd::CheckInRadius (				// �������� �߽����� �����ȿ� �ִ��� üũ
	INT iIndex
	)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpOBJ = &gObj[iIndex];

	if (lpOBJ->Connected < 3 || lpOBJ->Type != OBJTYPE_MONSTER)
		return FALSE;

	INT iX2 = (lpOBJ->X - m_iCUR_X) * (lpOBJ->X - m_iCUR_X);
	INT iY2 = (lpOBJ->Y - m_iCUR_Y) * (lpOBJ->Y - m_iCUR_Y);

	if (m_iRADIUS*m_iRADIUS < iX2 + iY2)
		return FALSE;
	
	return TRUE;
}


BOOL MonsterHerd::GetCurrentLocation(			// ������ ���� �߽���ǥ�� �� �����Ѵ�.
	UCHAR & cX,
	UCHAR & cY
	)
{
	if (!m_bHasInfo)			// ������ ������ ���� �ʴٸ� ����
		return FALSE;

	cX = m_iCUR_X;
	cY = m_iCUR_Y;

	return TRUE;
}


BOOL MonsterHerd::GetRandomLocation(			// ���� �ݰ泻�� ������ ��ǥ�� �� �����Ѵ�.
	UCHAR & cX,
	UCHAR & cY
	)
{
	if (!m_bHasInfo)			// ������ ������ ���� �ʴٸ� ����
		return FALSE;

	INT iCount = 100;
	while (iCount--) {
		cX = (rand() % (m_iRADIUS + 1)) * ((rand() % 2) ? 1 : -1) + m_iCUR_X;
		cY = (rand() % (m_iRADIUS + 1)) * ((rand() % 2) ? 1 : -1) + m_iCUR_Y;

		BYTE btMapAttr = MapC[m_iMapNumber].GetAttr(cX, cY);
		if( btMapAttr == MAP_ATTR_NONE )	// �̵������� �����̶�� ����
			return TRUE;
	}

	return FALSE;
}


BOOL MonsterHerd::CheckLocation(		// �ش� ������ �Ӽ��� �������� Ȯ��
	UCHAR & cX,
	UCHAR & cY
	)
{
	BYTE btMapAttr = MapC[m_iMapNumber].GetAttr(cX, cY);
	if( ((btMapAttr&MAP_ATTR_SAFTYZONE)==MAP_ATTR_SAFTYZONE) ||		// ��������
		((btMapAttr&MAP_ATTR_BLOCK)==MAP_ATTR_BLOCK) ||				// ��
		((btMapAttr&MAP_ATTR_HOLLOW)==MAP_ATTR_HOLLOW)				// ���ո�
		)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL MonsterHerd::MoveHerd(				// ������ �ش� �������� �̵���Ű�� ���� ���� ����
	UCHAR iTX,
	UCHAR iTY
	)
{
	if (!m_bHasInfo)					// ������ ������ ���� �ʴٸ� ����
		return FALSE;

	BYTE btMapAttr = MapC[m_iMapNumber].GetAttr(iTX, iTY);
	if( ((btMapAttr&MAP_ATTR_SAFTYZONE)==MAP_ATTR_SAFTYZONE) ||		// ��������
		((btMapAttr&MAP_ATTR_BLOCK)==MAP_ATTR_BLOCK) ||				// ��
		((btMapAttr&MAP_ATTR_HOLLOW)==MAP_ATTR_HOLLOW)				// ���ո�
		)
	{
		return FALSE;
	}

	m_iCUR_X = iTX;
	m_iCUR_Y = iTY;

	return TRUE;
}


BOOL MonsterHerd::MonsterHerdItemDrop(
	LPOBJECTSTRUCT lpObj
	)
{

	return FALSE;
}


VOID MonsterHerd::MonsterMoveAction(LPOBJECTSTRUCT lpObj)
{
#ifdef NEW_FORSKYLAND2
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == true )
	#else
	if( lpObj->m_SkillHarden != 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		return;
#endif
	if (lpObj == NULL)
		return;

	if (lpObj->Connected < 3 || lpObj->Type != OBJTYPE_MONSTER)
		return;

	int searchc=10;
	BYTE tpx, tpy;
	lpObj->NextActionTime = 1000;
				
	while(searchc--)
	{

		BOOL bMoveAble = GetRandomLocation (tpx, tpy);

		if (bMoveAble)
		{
			lpObj->TargetNumber			= -1;
			lpObj->m_ActState.Attack	= 0;
			lpObj->NextActionTime		= 500;
			lpObj->m_ActState.Emotion	= 0;
			lpObj->MTX					= tpx;
			lpObj->MTY					= tpy;
			lpObj->m_ActState.Move		= 1;
			break;
		}			
	}
}


VOID MonsterHerd::MonsterAttackAction(		// ���� ���� �� Ư�� ���Ͱ� �������� ���� ó��
	LPOBJECTSTRUCT lpObj,					// ��
	LPOBJECTSTRUCT lpTargetObj				// ���� ������ �ΰ�?
	)
{
#ifdef NEW_FORSKYLAND2
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == true )
	#else
	if( lpObj->m_SkillHarden != 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		return;
#endif

	if (lpObj == NULL)
		return;

	if (lpTargetObj == NULL)
		return;

	if (lpObj->Connected < 3 || lpObj->Type != OBJTYPE_MONSTER)
		return;

	if (lpTargetObj->Connected < 3)
		return;
	
}


VOID MonsterHerd::MonsterBaseAct(LPOBJECTSTRUCT lpObj)
{
	LPOBJECTSTRUCT lpTargetObj=NULL;

	if( lpObj->TargetNumber >= 0 )	// ������ ���ٸ� �ϴ� ���� ���·� ���ư���.
		lpTargetObj = (LPOBJECTSTRUCT)&gObj[lpObj->TargetNumber];
	else lpObj->m_ActState.Emotion = 0;


	switch(lpObj->m_ActState.Emotion) {
	case EMOTION_NORMAL	:	// ���� - ����
		{
			if( lpObj->m_ActState.Attack )	// ������ ���� �� �̾��ٸ� -> ���ݻ��� �ʱ�ȭ & ����
			{
				lpObj->m_ActState.Attack = 0;
				lpObj->TargetNumber = -1;
				lpObj->NextActionTime = 500;
			}
			
			int actcode1 = (rand()%2);		// ���� �ʽ��� ����

			// ��� ����.
			if( actcode1 == 0 )
			{
				//if( lpObj->m_Attribute == 100 )	LogAdd("��� ����.");
				lpObj->m_ActState.Rest = 1;
				lpObj->NextActionTime = 500;
			}
#ifdef NEW_FORSKYLAND2
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			else if( lpObj->m_MoveRange > 0 &&
				( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == false )
				   )
	#else
			else if( lpObj->m_MoveRange>0 && lpObj->m_SkillHarden == 0)
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#else
			else if( lpObj->m_MoveRange>0) 
#endif		
			{
				MonsterMoveAction (lpObj);
			}

			
			if (lpObj->m_bIsMonsterAttackFirst) {	// ���� ���� ���
				lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJTYPE_CHARACTER);
				
				// ���� �߰ߵǸ� ���޾Ƽ� ���� ���� ����~~
				if( lpObj->TargetNumber >= 0 )
				{
					lpObj->m_ActState.EmotionCount = 30;
					lpObj->m_ActState.Emotion = 1;
					//if( lpObj->m_Attribute == 100 )	LogAdd("���� ���� ����");
				}
			}
		}
		break;
	case EMOTION_ANGRY	:	// ���� - ���
		{
			if( lpObj->m_ActState.EmotionCount > 0 ) lpObj->m_ActState.EmotionCount--;
			else {
				lpObj->m_ActState.Emotion = 0;
			}
			
			if( lpObj->TargetNumber >= 0 && lpObj->PathStartEnd == 0 )	// ���� ����� ������..
			{
/*
#ifdef FOR_BLOODCASTLE
				if (CHECK_BLOODCASTLE(lpObj->MapNumber)) {				// ����� ����ĳ���� �����
					INT iRAND_CHANGE_TARGET = rand()%10;
					if (iRAND_CHANGE_TARGET == 3) {
						lpObj->LastAttackerID = -1;
					}
					if (iRAND_CHANGE_TARGET == 1) {
						if ((lpObj->LastAttackerID != -1) && (lpObj->LastAttackerID != lpObj->TargetNumber)) {
							// ���� �������� ������ ����� ���� ��ǥ���� ����� �ٸ���.
							if ((gObj[lpObj->LastAttackerID].Connected > 2) && (lpObj->MapNumber == gObj[lpObj->LastAttackerID].MapNumber)) {
								// ���� ���� �ʻ� �������ִٸ� ���ݴ���� �ٲ۴�.
								lpObj->TargetNumber = lpObj->LastAttackerID;
								lpTargetObj = &gObj[lpObj->LastAttackerID];
							}
						}
					}
				}
#endif
*/
				int dis = gObjCalDistance(lpObj, lpTargetObj);
				
				int attackRange;
				if( lpObj->m_AttackType >= 100 )
				{
					attackRange = lpObj->m_AttackRange+2;
				}
				else attackRange = lpObj->m_AttackRange;


				if(  dis <= attackRange )
				{
					// ���ݹ��� �ȿ� ������ ��üũ�� �� �Ŀ� ����
					int tuser = lpObj->TargetNumber;
					int map = gObj[tuser].MapNumber;
					BYTE attr ;
					if( MapC[map].CheckWall(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == TRUE )
					{
						attr = MapC[map].GetAttr(gObj[tuser].X, gObj[tuser].Y);
						if( (attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE)// ���� �Ӽ��� �ƴϸ�
						{
							lpObj->m_ActState.Attack = 1;
							lpObj->m_ActState.EmotionCount = rand()%30+20;
						}
						else {	// ���� �Ӽ��� �ִٸ�.
							lpObj->TargetNumber = -1;
							lpObj->m_ActState.EmotionCount = 30;
							lpObj->m_ActState.Emotion = 1;					
						}
						lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);
						lpObj->NextActionTime = lpObj->m_AttackSpeed;
					}
				}
				else 
				{
					// ���ݹ��� �ۿ� ������ ���� ��ġ ����
					if( gObjMonsterGetTargetPos(lpObj) == TRUE )
					{
						if( MapC[lpObj->MapNumber].CheckWall(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY) == TRUE )
						{
							// ������ ���̿� ���� ����.
							lpObj->m_ActState.Move = 1;
							lpObj->NextActionTime = 400;
							lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);	// ������ �������� ���Ѵ�.
						}
						else
						{
							// ������ ���̿� ������ ���θ��� �ִ�.
							MonsterMoveAction(lpObj);
							lpObj->m_ActState.Emotion = 3;
							lpObj->m_ActState.EmotionCount = 10;

						}
					}
					else 
					{
						// ������ ��θ� ã�� �� ���� (����Ʈ �ۿ� �ִ� ��)
						MonsterMoveAction(lpObj);
						lpObj->m_ActState.Emotion = 3;
						lpObj->m_ActState.EmotionCount = 10;

//						gObjMonsterMoveAction(lpObj);
					}
				}
			}
			else {			
			}
			
		}
		break;
	case EMOTION_FLEE	:	// ���� - ����
		{
			
		}
		break;
	case EMOTION_HALT	:	// ���� - �������
		{
			if( lpObj->m_ActState.EmotionCount > 0 ) lpObj->m_ActState.EmotionCount--;
			else {
				lpObj->m_ActState.Emotion = 0;
			}
			lpObj->m_ActState.Move = 0;
			lpObj->m_ActState.Attack = 0;
			lpObj->NextActionTime = 400;			
			}
		break;
	default :	// �̷� ���´� ���� - ����
		break;
	}
}


VOID MonsterHerd::MonsterDieAction(LPOBJECTSTRUCT lpObj)
{
	if (lpObj == NULL)
		return;

	if (lpObj->Connected < 3 || lpObj->Type != OBJTYPE_MONSTER)
		return;

}


BOOL MonsterHerd::MonsterRegenAction(LPOBJECTSTRUCT lpObj)
{
	if (lpObj == NULL)
		return FALSE;

	if (lpObj->Connected < 3 || lpObj->Type != OBJTYPE_MONSTER)
		return FALSE;

	MonsterHerd * lpMH = lpObj->m_lpMonsterHerd;

	if (lpMH == NULL)
		return FALSE;

	LPMONSTER_HERD_DATA lpMHD = lpMH->GetMonsterData(lpObj->m_Index);
	if(!lpMHD || !lpMHD->m_bRegen)
		return FALSE;

	// ������ ��Ų��.
	lpObj->Life		= lpObj->MaxLife+lpObj->AddLife;
	lpObj->Mana		= lpObj->MaxMana+lpObj->AddMana;
	lpObj->Live		= 1;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjClearBuffEffect( lpObj, CLEAR_TYPE_DIEREGEN );
#else
	lpObj->m_PoisonBeattackCount= 0;
	lpObj->m_ColdBeattackCount	= 0;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	lpObj->m_ViewState			= 0;
	lpObj->Teleport				= 0;

	for( INT i=0; i<MAXSELFDEFENSE; i++)
	{
		lpObj->SelfDefenseTime[i]	= 0;
	}

	gObjTimeCheckSelfDefense(lpObj);
	
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjRemoveBuffEffect( lpObj, BUFFTYPE_POISON );
	gObjRemoveBuffEffect( lpObj, BUFFTYPE_FREEZE );
#else
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_POISON );
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_FREEZE );
	#else
	lpObj->m_ViewSkillState &= 0xFC;
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	gObjClearViewport(lpObj);

	gObjViewportListProtocolDestroy(lpObj);
	gObjViewportClose(lpObj);

	lpObj->m_ActState.Attack		= 0;
	lpObj->m_ActState.EmotionCount	= 0;
	lpObj->m_ActState.Escape		= 0;
	lpObj->m_ActState.Move			= 0;
	lpObj->m_ActState.Rest			= 0;
	lpObj->m_ActState.Emotion		= 0;
	lpObj->TargetNumber				= -1;
	lpObj->NextActionTime			= 5000;

	UCHAR cX, cY;
	INT iCount = 100;
	BOOL bGetPosition = FALSE;
	while (iCount--) {
		if (lpMH->GetRandomLocation(cX, cY)) {
			bGetPosition = TRUE;
			break;
		}
	}

	if (!bGetPosition) {
		lpObj->Live	   = 0;
		lpObj->m_State   = OBJST_DYING;	
		lpObj->RegenTime = GetTickCount();
		lpObj->DieRegen  = 1;			
		return FALSE;
	}

	lpObj->X = cX;
	lpObj->Y = cY;

	lpObj->MTX		= lpObj->X;
	lpObj->MTY		= lpObj->Y;
	lpObj->TX		= lpObj->X;
	lpObj->TY		= lpObj->Y;
	lpObj->StartX	= (BYTE)lpObj->X;
	lpObj->StartY	= (BYTE)lpObj->Y;

	gObjMonsterHitDamageInit(lpObj);
		
	CreateFrustrum(lpObj->X, lpObj->Y, lpObj->m_Index);
	
	lpObj->DieRegen	= 0;
	lpObj->m_State	= OBJST_CREATE;

	return TRUE;
}


#endif


