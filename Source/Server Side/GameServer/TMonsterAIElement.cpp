#include "StdAfx.h"
#include "TMonsterAIElement.h"

#include <math.h>
#include "Include\ReadScript.h"

#include "User.h"
#include "protocol.h"
#include "gObjMonster.h"
#include "GameMain.h"

#include "TMonsterSkillManager.h"
#include "TMonsterAIGroup.h"

#include "CrywolfUtil.h"
#include "TMonsterAIUtil.h"

#ifdef ADD_KANTURU_20060627
#include "KanturuUtil.h"

#include "KanturuDefine.h"
#include "KanturuMonsterMng.h"

static CKanturuUtil		KANTURU_UTIL;
#endif // ADD_KANTURU_20060627

#include "DirPath.h"
extern CDirPath	gDirPath;

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

//----------------------------------------------------------------------------------------

BOOL				TMonsterAIElement::s_bDataLoad = FALSE;
TMonsterAIElement	TMonsterAIElement::s_MonsterAIElementArray[MAX_MONSTER_AI_ELEMENT];
TMonsterAIMovePath	TMonsterAIElement::s_MonsterAIMovePath[MAX_MAP];

//----------------------------------------------------------------------------------------

static CCrywolfUtil		UTIL;
static TMonsterAIUtil	MONSTER_UTIL;


TMonsterAIElement::TMonsterAIElement()
{
#if ENABLE_MC_CODE == 1
	s_MonsterAIMovePath = NULL;
#endif // ENABLE_MC_CODE == 1
	Reset();
}

TMonsterAIElement::~TMonsterAIElement()
{
#if ENABLE_MC_CODE == 1
	delete[] s_MonsterAIMovePath;
#endif // ENABLE_MC_CODE == 1
}


void TMonsterAIElement::Reset()
{	
	m_iElementClass		= _INVALID;
	m_iElementType		= _INVALID;
	m_iElementNumber	= _INVALID;

	m_iSuccessRate		= _INVALID;
	m_iDelayTime		= _INVALID;
	m_iTargetType		= _INVALID;
	m_iX				= _INVALID;
	m_iY				= _INVALID;
	
	ZeroMemory( m_szElementName, MAX_AIELEMENTNAME );
}

int TMonsterAIElement::LoadData( LPSTR lpszFileName )
{
	s_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster AI Element] - File load error : File Name Error");
		return FALSE;
	}

#if ENABLE_MC_CODE == 1
	if (g_TerrainManager.Size() < 1)
		g_TerrainManager.Load();

	if (s_MonsterAIMovePath == NULL)
		s_MonsterAIMovePath = new TMonsterAIMovePath[g_TerrainManager.Size()];
#endif // ENABLE_MC_CODE == 1

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster AI Element] - Can't Open %s ", lpszFileName);
			return false;
		}
		
		DelAllAIElement();

		SMDToken Token;
		
		int iType = -1;
		
		while( TRUE )
		{
			Token = GetToken();
			if(Token == END) break;
			iType = (int)TokenNumber;

			while( TRUE )
			{				
				if( iType == 0 )
				{	
					// 1. >	������ �ʱ�ȭ
					char	szElementName[MAX_AIELEMENTNAME] = {0,};
					int		iElementNumber	= _INVALID;
					int		iElementClass	= _INVALID;
					int		iElementType	= _INVALID;
					int		iSuccessRate	= _INVALID;
					int		iDelayTime		= _INVALID;
					int		iTargetType		= _INVALID;
					int		iX				= _INVALID;
					int		iY				= _INVALID;
				
					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iElementNumber	= (int)TokenNumber;
					Token = GetToken(); CopyMemory(szElementName, TokenString, MAX_AIELEMENTNAME);
					Token = GetToken(); iElementClass	= (int)TokenNumber;
					Token = GetToken(); iElementType	= (int)TokenNumber;
					Token = GetToken(); iSuccessRate	= (int)TokenNumber;
					Token = GetToken(); iDelayTime		= (int)TokenNumber;
					Token = GetToken(); iTargetType		= (int)TokenNumber;
					Token = GetToken(); iX				= (int)TokenNumber;
					Token = GetToken(); iY				= (int)TokenNumber;
				
					// 3. >	������ ��ȿ�� �˻�
					if( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_AI_ELEMENT )
					{
						MsgBox("[Monster AI Element] - ElementNumber(%d) Error (%s) File. ", iElementNumber, lpszFileName);
						continue;
					}
					if( iElementClass < 0 || iElementClass >= MAX_MONSTER_AI_ELEMENT_STATE )
					{
						MsgBox("[Monster AI Element] - ElementClass(%d) Error (%s) File. ", iElementClass, lpszFileName);
						continue;
					}


					// 4. > ������ ����
					s_MonsterAIElementArray[iElementNumber].m_iElementNumber	= iElementNumber;
					s_MonsterAIElementArray[iElementNumber].m_iElementClass		= iElementClass;
					s_MonsterAIElementArray[iElementNumber].m_iElementType		= iElementType;
					s_MonsterAIElementArray[iElementNumber].m_iSuccessRate		= iSuccessRate;
					s_MonsterAIElementArray[iElementNumber].m_iDelayTime		= iDelayTime;
					s_MonsterAIElementArray[iElementNumber].m_iTargetType		= iTargetType;
					s_MonsterAIElementArray[iElementNumber].m_iX				= iX;
					s_MonsterAIElementArray[iElementNumber].m_iY				= iY;
					
					CopyMemory( s_MonsterAIElementArray[iElementNumber].m_szElementName, szElementName, MAX_AIELEMENTNAME );
					
				}	
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Monster AI Element] - %s file is Loaded", lpszFileName);

		s_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster AI Element] - Loading Exception Error (%s) File. ", lpszFileName);
	}


	// ���� �̵� ��θ� �ε��Ѵ�.
	s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].LoadData(gDirPath.GetNewPath("MovePath_35.dat"));
	
	return 0;
}

int	 TMonsterAIElement::DelAllAIElement()
{
	for( int i=0; i<MAX_MONSTER_AI_ELEMENT; i++ )
	{
		s_MonsterAIElementArray[i].Reset();
	}

#if ENABLE_MC_CODE == 1
	for (int j = 0; j < g_TerrainManager.Size(); j++)
#else // ENABLE_MC_CODE == 1
	for (int j = 0; j < MAX_MAP; j++)
#endif // ENABLE_MC_CODE == 1
	{
		s_MonsterAIMovePath[j].DelAllAIMonsterMovePath();
	}

	return 0;
}

TMonsterAIElement* TMonsterAIElement::FindAIElement( int iElementNumber )
{
	if( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_AI_ELEMENT )
	{
		LogAddTD("[Monster AI Element] FindAIElement() Error - (ElementNumber=%d) ", iElementNumber);
		return NULL;
	}

	if( s_MonsterAIElementArray[iElementNumber].m_iElementNumber == iElementNumber )
	{
		return &s_MonsterAIElementArray[iElementNumber];
	}
	else
		LogAddTD("[Monster AI Element] FindAIElement() Error - (ElementNumber=%d) ", iElementNumber);
	
	return NULL;
}

int TMonsterAIElement::ForceAIElement( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	// 1. > General value Checking
	if( rand() % 100 > m_iSuccessRate)
	{
#if TESTSERVER == 1
		LogAddC(LOGC_RED, "[Force AI-Element] Failed to apply (%s) Element by SuccessRate", GetElementName());
#endif
		return _FAIL;
	}

	// 2. > Action Time Checking
	// MonsterAIUnit �� Delay Ÿ���� �̿��Ѵ�. - �Ʒ� ��� ����.
	//	if( (GetTickCount()-lpObj->CurActionTime) < (lpObj->NextActionTime+lpObj->DelayActionTime) ) 
	//		return _FAIL;
	//	lpObj->CurActionTime = GetTickCount();	

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	#if TESTSERVER == 1
		DebugLog("[Force AI-Element] apply (%s)  (%d,%d) %s (%d) idx(%d)\n", GetElementName(), lpObj->X, lpObj->Y, lpObj->Name,lpObj->Class, lpObj->m_Index);
	#endif
#endif

	// 3. > Applying
	switch( m_iElementType )
	{
	case MAE_TYPE_COMMON_NORMAL		:
		ApplyElementCommon			( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_MOVE_NORMAL		:
		ApplyElementMove			( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_MOVE_TARGET		:
		ApplyElementMoveTarget		( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_GROUP_MOVE		:
		ApplyElementGroupMove		( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_ATTACK_NORMAL		:
		ApplyElementAttack			( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_ATTACK_AREA		:
		ApplyElementAttackArea		( iIndex, iTargetIndex, pAIState );
		break;
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	case MAE_TYPE_ATTACK_PENETRATION	:
		ApplyElementAttackPenetration	( iIndex, iTargetIndex, pAIState );
		break;
#endif		
	case MAE_TYPE_HEAL_SELF			:
		ApplyElementHealSelf		( iIndex, iTargetIndex, pAIState );
		break;	
	case MAE_TYPE_HEAL_GROUP		:
		ApplyElementHealGroup		( iIndex, iTargetIndex, pAIState );
		break;	
	case MAE_TYPE_AVOID_NORMAL		:
		ApplyElementAvoid			( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_SOMMON	:		
		ApplyElementSpecialSommon	( iIndex, iTargetIndex, pAIState );
		break;

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724	
	case MAE_TYPE_SPECIAL_NIGHTMARE_SUMMON	:
		ApplyElementNightmareSummon ( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_WARP	:
		ApplyElementNightmareWarp ( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_SKILLATTACK :
		ApplyElementSkillAttack	( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_CHANGEAI :
		ApplyElementAIChange ( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_IMMUNE	:
		ApplyElementSpecialImmune	( iIndex, iTargetIndex, pAIState );
		break;
#endif // MODIFY_NEW_MONSTER_AI_01_2006724
		

	};

	return _SUCCESS;
	
}

int	TMonsterAIElement::ApplyElementCommon( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	
	//UTIL.SendCrywolfChattingMsg( iIndex, "Element-�Ϲ�");
	
#ifdef ADD_KANTURU_20060627
	KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "�ѽ� ����!" );
#endif

	return 0;
}

int	TMonsterAIElement::ApplyElementMove( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	
	UTIL.SendCrywolfChattingMsg( iIndex, "Element-�̵�");
#ifdef ADD_KANTURU_20060627	
	KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-�̵�");
#endif

	// 1. > �̵� �� �̰�
	if( lpObj->PathStartEnd != 0 )
	{
		return 0; 
	}

	// 2. > �̵� ��ǥ�� ��쿡 ���� ���Ѵ�.
	BOOL bFindXY = FALSE;

	if( pAIState->m_iTransitionType == MAI_STATE_TRANS_OUT_ENEMY )
	{
		// ���� ���� �� �� �߰� 
		bFindXY = MONSTER_UTIL.GetXYToChase( lpObj );
	}
	else
	{
		// ���� ����
		bFindXY = MONSTER_UTIL.GetXYToPatrol( lpObj );
	}
	
	// 3. > �̵� �Ѵ�.
	if( bFindXY )
	{
		if( MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE) )
		{
			// ��ã�� ����		
		}
		else
		{
			// ��ã�� ����
		}
	}

	return 0;
}

int	TMonsterAIElement::ApplyElementMoveTarget( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-Ÿ���̵�");
#ifdef ADD_KANTURU_20060627	
	KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-Ÿ���̵�");
#endif

	// 1. > �̵� �� �̰�
	if( lpObj->PathStartEnd != 0 )
	{
		return 0; 
	}

	// 3. > ���������� �� ��ǥ�� �̵��Ϸ� �ϸ� �ȵǱ⶧���� ������ �д�. m_iRange

	// 4. > �������� �����ߴٸ� �Ϲ� �̵�
	if( lpObj->X == m_iX && lpObj->Y == m_iY )
	{
		ApplyElementMove( iIndex, iTargetIndex, pAIState );
		return 0;
	}

	// 5. > ���� �̵� PATH �� �̿��ؼ� Target ��ġ�� ã�ư���.
	BOOL	bFindXY			= TRUE;			

	int		iTargetX		= m_iX;
	int		iTargetY		= m_iY;

	int		iTargetDistance	= sqrt( (double)(lpObj->X-iTargetX)*(lpObj->X-iTargetX) + (lpObj->Y-iTargetY)*(lpObj->Y-iTargetY));
	
	// �����Ͱ� �ε� �Ǿ� �ִٸ�,
	if( s_MonsterAIMovePath[lpObj->MapNumber].m_bDataLoad && (iTargetDistance > 10) )
	{
		int iMinCost = 1000000;
		int iMidX	 = _INVALID;
		int iMidY	 = _INVALID;
		int iSpotNum = _INVALID;

		for( int i=0; i<MAX_MONSTER_MOVE_PATH_SPOT; i++ )
		{
			TMonsterAIMovePathInfo & PathInfo = s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].m_MovePathInfo[i];

			// 1 ) ���� ����� �н����̵��� ã�´�.

			float fDistX = (float)(lpObj->X-PathInfo.m_iPathX);
			float fDistY = (float)(lpObj->Y-PathInfo.m_iPathY);
			int iPathSpotDist = (int)(sqrt((fDistX*fDistX)+(fDistY*fDistY)));

			// 20Ÿ�� �̳��� ������
			if( iPathSpotDist < 20 )
			{
				fDistX = (float)(iTargetX-PathInfo.m_iPathX);
				fDistY = (float)(iTargetY-PathInfo.m_iPathY);
				
				// ������������ ��� ���

				// ������ �̵� �ߴ� Spot�� ���� �ʴ� ó�� �ʿ�
				int iMidDist = (int)(sqrt((fDistX*fDistX)+(fDistY*fDistY)));

				if( iMinCost > iMidDist )
				{
					// ���� ������ �ִ� Spot�� ����
					if( iMidDist != 0 )
					{
						iMinCost	= iMidDist;
						iMidX		= PathInfo.m_iPathX;
						iMidY		= PathInfo.m_iPathY;
						
						iSpotNum	= i;
					}	
				}
			}
		}
		
		if( iMinCost != 1000000 )
		{
			iTargetX = iMidX;
			iTargetY = iMidY;
		}
	}

	// 6. > �̵� �Ѵ�.
	if( bFindXY )
	{
		if( MONSTER_UTIL.FindPathToMoveMonster(lpObj, iTargetX, iTargetY, 7) )
		{
			// ��ã�� ����
			lpObj->PathStartEnd = 1;
		}
		else
		{
			// ��ã�� ����
			lpObj->PathStartEnd = 0;
		}		
	}
	
	return 0;
}

int	TMonsterAIElement::ApplyElementGroupMove( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-�׷��̵�");

	// 1. > �̵� �� �̰�
	if( lpObj->PathStartEnd != 0 )
	{
		return 0; 
	}

	// 2. > �̵� ��ǥ�� ��쿡 ���� ���Ѵ�.
	BOOL bFindXY		= FALSE;
	BOOL bFoundLeader	= TRUE;
	int  iLeaderIndex	= _INVALID;
	
	iLeaderIndex = TMonsterAIGroup::FindGroupLeader( lpObj->m_iGroupNumber );

	if( iLeaderIndex == _INVALID || !gObj[iLeaderIndex].Live )
	{
		bFoundLeader = FALSE;
	}

	if( bFoundLeader && gObjCalDistance(lpObj, &gObj[iLeaderIndex]) > 6 )
	{
		lpObj->TargetNumber = iLeaderIndex;
		bFindXY = MONSTER_UTIL.GetXYToChase( lpObj );
	}
	else
	{
		if( pAIState->m_iTransitionType == MAI_STATE_TRANS_OUT_ENEMY )
		{
			// ���� ���� �� �� �߰� 
			bFindXY = MONSTER_UTIL.GetXYToChase( lpObj );
		}
		else
		{
			// ���� ����
			bFindXY = MONSTER_UTIL.GetXYToPatrol( lpObj );
		}

	}

	// 3. > �̵� �Ѵ�.
	if( bFindXY )
	{
		if( MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE) )
		{
			// ��ã�� ����		
		}
		else
		{
			MONSTER_UTIL.GetXYToPatrol( lpObj );
			MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE);
		}
	}

	return 0;
}

int	TMonsterAIElement::ApplyElementAttack( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-����");

	// � ��ų ������� ����!

	// pAIState �� �˻��ؼ� Agro ���°� ���� ��� ������ ���� ��ų ����� �� 
	// Agro �� ���� ���� �ش�.

	if( TMonsterSkillManager::CheckMonsterSkill(lpObj->Class) )
	{
		BOOL bEnableAttack = TRUE;
		// 1) Ÿ���� ������.
		if( lpObj->TargetNumber < 0 )
#ifdef MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		{
			lpObj->TargetNumber			= -1;
			lpObj->m_ActState.Emotion	= 0;
			lpObj->m_ActState.Attack	= 0;
			lpObj->m_ActState.Move		= 0;
			lpObj->NextActionTime		= 1000;
			return _FAIL;
		}
#else	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
			bEnableAttack = FALSE;
#endif	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		
		// 1) �׾���
		// 2) �ڷ���Ʈ ���� ��
		if( !gObj[lpObj->TargetNumber].Live || gObj[lpObj->TargetNumber].Teleport)
		{
			bEnableAttack = FALSE;
		}
		
		// 1) ���� ���� ���� �ƴϰ�,
		// 2) ���� ���̰�,
		if( gObj[lpObj->TargetNumber].Connected <= CSS_LOGINOK || gObj[lpObj->TargetNumber].CloseCount != -1 )
		{
			bEnableAttack = FALSE;
		}
		
		if( !bEnableAttack )
		{
			lpObj->TargetNumber			= -1;
			lpObj->m_ActState.Emotion	= 0;
			lpObj->m_ActState.Attack	= 0;
			lpObj->m_ActState.Move		= 0;
			lpObj->NextActionTime		= 1000;
			return _FAIL;
		}
		
		// ���⼭ ��ų�Ŵ����� ó��
		
		// AI���� ��ų ó��!!!!!!!

		// ���� ������ ��´�.
		LPOBJECTSTRUCT lpTargetObj	= &gObj[lpObj->TargetNumber];
		lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);
		
		// 1. �Ϲ� ����
		if( rand()% 4 == 0 )
		{
			// ���� �Ÿ��� �´��� �˻�

			PMSG_ATTACK pAttackMsg;
			pAttackMsg.AttackAction = AT_ATTACK1;
			pAttackMsg.DirDis       = lpObj->Dir;
			pAttackMsg.NumberH		= HIBYTE(lpObj->TargetNumber);
			pAttackMsg.NumberL		= LOBYTE(lpObj->TargetNumber);

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
			//CGAttack(&pAttackMsg, lpObj->m_Index);
#else
			CGAttack(&pAttackMsg, lpObj->m_Index);
#endif

			GCActionSend(lpObj, AT_ATTACK1, lpObj->m_Index);
			gObjAttack(lpObj, &gObj[lpObj->TargetNumber], NULL, 0, 0);
		}
		else
		{
			// 2. ���� ��ų ����
			TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 0 );
		}
		
		lpObj->m_ActState.Attack = 0;
		
		return _FAIL;
	}
	else
	{
#ifdef MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		if(lpObj->TargetNumber < 0)
			return _FAIL;
#endif	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711

		// ���� ������ ��´�.
		LPOBJECTSTRUCT lpTargetObj	= &gObj[lpObj->TargetNumber];
		lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);
		
		
		PMSG_ATTACK pAttackMsg;
		pAttackMsg.AttackAction = AT_ATTACK1;
		pAttackMsg.DirDis       = lpObj->Dir;
		pAttackMsg.NumberH		= HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL		= LOBYTE(lpObj->TargetNumber);
		CGAttack(&pAttackMsg, lpObj->m_Index);
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
		GCActionSend(lpObj, AT_ATTACK1, lpObj->m_Index, lpTargetObj->m_Index );
#else
		GCActionSend(lpObj, AT_ATTACK1, lpObj->m_Index);
#endif
		gObjAttack(lpObj, &gObj[lpObj->TargetNumber], NULL, 0, 0);
	}

	return 0;
}

int	TMonsterAIElement::ApplyElementAttackArea( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-��������");

	// ���� ���� ���� ĳ���͵��� ã�´�.

	int iTargetX = m_iX + (rand()%5) * (rand()%2==0?1:-1);
	int iTargetY = m_iY + (rand()%5) * (rand()%2==0?1:-1);

	for( int i=ALLOC_USEROBJECTSTART; i < MAX_OBJECT; i++  )
	{
		if( !gObjIsConnected(i) )
			continue;

		LPOBJECTSTRUCT lpTargetObj = &gObj[i];

		if( !lpObj->Live )
			continue;
		
		if( lpObj->MapNumber != lpTargetObj->MapNumber )
			continue;
		
		int	iTargetDistance	= sqrt( (double)(lpTargetObj->X-iTargetX)*(lpTargetObj->X-iTargetX) + (lpTargetObj->Y-iTargetY)*(lpTargetObj->Y-iTargetY));

		// �ش� ������ ĳ���Ϳ��� ���� ������ �̷�������� �˸���.
		if( iTargetDistance < 10 )
		{
			PMSG_NOTIFY_REGION_MONSTER_ATTACK pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x0C, sizeof( pMsg ));
			pMsg.btObjClassH	= HIBYTE(lpObj->Class);
			pMsg.btObjClassL	= LOBYTE(lpObj->Class);

			pMsg.btSourceX		= lpObj->X;
			pMsg.btSourceY		= lpObj->Y;

			pMsg.btPointX		= iTargetX;		
			pMsg.btPointY		= iTargetY;
			
			DataSend( lpTargetObj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg) );
		}

		// ���� ������ �޾Ҵ�.
		if( iTargetDistance < 6 )
		{
			gObjAttack(lpObj, lpTargetObj, NULL, 0, 0);
		}
	}
	
	return 0;
}

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
int	TMonsterAIElement::ApplyElementAttackPenetration( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	
	// iTargetIndex���� RunAIUnit(...)���� 0���� ���´�.
	// ���ݴ���� �����Ѵ�.
	iTargetIndex = lpObj->TargetNumber;	
	
	if ( lpObj->TargetNumber == -1 )
		return 0 ;
	
	if( !gObj[iTargetIndex].Live ) 
		return 0;

	TMonsterSkillManager::UseMonsterSkill( iIndex, iTargetIndex, MSUS_TYPE_ATTACK_PENETRATION );

	return 0;
}
#endif	// MODIFY_NEW_MONSTER_AI_01_2006724

int	TMonsterAIElement::ApplyElementAvoid( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-ȸ��" );

#ifdef ADD_KANTURU_20060627	
	KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-ȸ��" );
#endif

	BOOL bFindXY = MONSTER_UTIL.GetXYToEascape( lpObj );
	
	if( bFindXY )
	{
		if( MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE) )
		{
			// ��ã�� ����		
		}
		else
		{
			// ��ã�� ����
		}
	}
	
	return 0;
}

int	TMonsterAIElement::ApplyElementHealSelf( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{	
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-����ġ��" );
	
	lpObj->Life += lpObj->Life * 20 / 100;

	UTIL.SendCrywolfChattingMsg( iIndex, "HP : %d", (INT)lpObj->Life );

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpObj, BUFFTYPE_DEFENSE_POWER_INC, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
	ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_DEFENSE );
	GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	// ������ ġ���Ѵ�

	return 0;
}

int	TMonsterAIElement::ApplyElementHealGroup( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-�׷�ġ��" );

	// ġ���� �׷� ����� �ִ��� �˻�..(6Ÿ�� �̳�
	TMonsterAIGroupMember* pMemb = TMonsterAIGroup::FindGroupMemberToHeal(lpObj->m_Index,lpObj->m_iGroupNumber,lpObj->m_iGroupMemberGuid, 6);
	
	if( pMemb != NULL )
	{
		LPOBJECTSTRUCT lpTargetObj		= &gObj[pMemb->m_iObjIndex];
		
		if( !lpTargetObj->Live )
			return 0;

		TMonsterSkillUnit *lpSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit( lpObj->m_Index, MSUS_TYPE_HEAL_GROUP );

		if( lpSkillUnit != NULL )
		{
			lpSkillUnit->RunSkill( iIndex, lpTargetObj->m_Index );
		}

		UTIL.SendCrywolfChattingMsg( iIndex, "�׷�ġ�� HP : %d", (INT)lpTargetObj->Life );
		UTIL.SendCrywolfChattingMsg( lpTargetObj->m_Index, "HP : %d", (INT)lpTargetObj->Life );
	}

	return 0;
}


int	TMonsterAIElement::ApplyElementSpecialSommon( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-Ư����ȯ" );
	
	// ��ȯ, �� ���� ��ų �׷� ����� �ִ��� �˻�
	TMonsterAIGroupMember* pMemb = TMonsterAIGroup::FindGroupMemberToSommon(lpObj->m_Index,lpObj->m_iGroupNumber,lpObj->m_iGroupMemberGuid );
	
	if( pMemb != NULL )
	{
		LPOBJECTSTRUCT lpTargetObj		= &gObj[pMemb->m_iObjIndex];

		if( lpTargetObj->Live )
			return 0;

		TMonsterSkillUnit *lpSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit( lpObj->m_Index, MSUS_TYPE_SPECIAL_SOMMON );

		if( lpSkillUnit != NULL )
		{
			lpSkillUnit->RunSkill( iIndex, lpTargetObj->m_Index );
		}

	}

	return 0;
}

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
int TMonsterAIElement::ApplyElementSpecialImmune( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	// �������·� �����Ѵ�.
	// m_iX : ���� ���� �����ð� (��)
	// m_iY : ���� ���� �����ð� (��)
	
	if ( m_iX < 1 ) 
		m_iX = 10;  // �⺻�� 10�� ����
	if ( m_iY < 1 ) 
		m_iY = 10;	// �⺻�� 10�� ����

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE, 0, 0, 0, 0, m_iX );
	gObjAddBuffEffect( lpObj, BUFFTYPE_MONSTER_MELEE_IMMUNE, 0, 0, 0, 0, m_iY );
#else
	lpObj->m_ImmuneToHarmCount	=	m_iX;	// �������� ���� �ð� ����
	lpObj->m_ImmuneToMagicCount	=	m_iY;	// �������� ���� �ð� ����
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	return 0;
}

int TMonsterAIElement::ApplyElementNightmareSummon( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	// ����Ʈ �޾ Ư��.
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	// Ÿ�� ��븦 �����ؾ���.
	if ( lpObj->TargetNumber == -1 )
	{
		// Ÿ���� ���� ��� �ڱ⸦ Ÿ������ ��.
		iTargetIndex = lpObj->m_Index;
	}
	iTargetIndex = lpObj->TargetNumber;

	// ��ų Ȯ��
	TMonsterSkillUnit *lpSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit( lpObj->m_Index, MSUS_TYPE_SPECIAL_SOMMON );
	if( lpSkillUnit != NULL )
	{
		GCUseMonsterSkillSend( &gObj[iIndex], &gObj[iTargetIndex], lpSkillUnit->m_iUnitNumber );	
		//MsgOutput(iTargetIndex, "[USEMONSTERSKILL] %d ", lpSkillUnit->m_iUnitNumber);

#ifdef ADD_KANTURU_20060627
		// ��ȯ ó��
		int iRegenMonster = g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_REGENMONSTER );
#endif

	}
	
	return 0;
}

int TMonsterAIElement::ApplyElementNightmareWarp( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	BYTE x, y;
	x = m_iX;
	y = m_iY;

	/*
	���� ���� üũ ���� ����.
	if( !gObjCheckTeleportArea(iIndex, x, y) )
	{
		LogAddC(LOGC_RED, "[%s][%s] Try Teleport Not Move Area [%d,%d]", 
			   lpObj->AccountID, lpObj->Name, x, y);
		return 0;
	}
	*/

	//KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "����Ʈ�޾�-����" );

	// �����̵� ��ų ��� ��Ŷ : ���� ������� �׽�Ʈ (�̹� ó���ǰ� ����. ���� ����� ����)
	PMSG_MAGICATTACK_RESULT	pAttack;
	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof( pAttack));
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
	pAttack.MagicNumberH   = HIBYTE( AT_SKILL_TELEPORT );
	pAttack.MagicNumberL   = LOBYTE( AT_SKILL_TELEPORT );
#else
	pAttack.MagicNumber   = AT_SKILL_TELEPORT;
#endif
	pAttack.SourceNumberH = HIBYTE(iIndex);
	pAttack.SourceNumberL = LOBYTE(iIndex);	
	pAttack.TargetNumberH = HIBYTE(iIndex);
	pAttack.TargetNumberL = LOBYTE(iIndex);	
	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(iIndex, (LPBYTE)&pAttack, pAttack.h.size);
	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);

	// �ڷ���Ʈ ���
	LogAddC(LOGC_RED, "[%s] Warp [%d,%d] -> [%d,%d]", 
		   lpObj->Name, lpObj->X, lpObj->Y, x, y);
	gObjTeleportMagicUse(iIndex, x, y);

	// Ÿ������
	lpObj->TargetNumber = -1;

	return 0;
}

int TMonsterAIElement::ApplyElementSkillAttack( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	//KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-���Ͻ�ų����");

	// � ��ų ������� ����!

	// pAIState �� �˻��ؼ� Agro ���°� ���� ��� ������ ���� ��ų ����� �� 
	// Agro �� ���� ���� �ش�.

	if( TMonsterSkillManager::CheckMonsterSkill(lpObj->Class) )
	{
		BOOL bEnableAttack = TRUE;
		// 1) Ÿ���� ������.
		if( lpObj->TargetNumber < 0 )
#ifdef MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		{
			lpObj->TargetNumber			= -1;
			lpObj->m_ActState.Emotion	= 0;
			lpObj->m_ActState.Attack	= 0;
			lpObj->m_ActState.Move		= 0;
			lpObj->NextActionTime		= 1000;
			return _FAIL;
		}
#else	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
			bEnableAttack = FALSE;
#endif	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		
		// 1) �׾���
		// 2) �ڷ���Ʈ ���� ��
		if( !gObj[lpObj->TargetNumber].Live || gObj[lpObj->TargetNumber].Teleport)
		{
			bEnableAttack = FALSE;
		}
		
		// 1) ���� ���� ���� �ƴϰ�,
		// 2) ���� ���̰�,
		if( gObj[lpObj->TargetNumber].Connected <= CSS_LOGINOK || gObj[lpObj->TargetNumber].CloseCount != -1 )
		{
			bEnableAttack = FALSE;
		}
		
		if( !bEnableAttack )
		{
			lpObj->TargetNumber			= -1;
			lpObj->m_ActState.Emotion	= 0;
			lpObj->m_ActState.Attack	= 0;
			lpObj->m_ActState.Move		= 0;
			lpObj->NextActionTime		= 1000;
			return _FAIL;
		}
		
		// ���⼭ ��ų�Ŵ����� ó��
		
		// AI���� ��ų ó��!!!!!!!

		// ���� ������ ��´�.
		LPOBJECTSTRUCT lpTargetObj	= &gObj[lpObj->TargetNumber];
		lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);

		// Ȯ�� ����
		int iRate1 = m_iTargetType;		// script ��
		int iRate2 = m_iX;				// script ��
		int iRate3 = m_iY;
		int iRandom = rand()%100;


		//LogAddC(LOGC_RED, "[AI](%s) %s r1:%d r2:%d r3:%d rate:%d", GetElementName(), lpObj->Name, iRate1,iRate2,iRate3, iRandom);

		// 0 Ÿ�� ��ų �ߵ�
		if ( iRandom < iRate1 )
		{
			TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 0 );
		} 
		else if ( iRandom < iRate1 + iRate2 )
		{
			// 1Ÿ�� ��ų �ߵ�
			TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 1 );
		} 
		else if ( iRandom < iRate1 + iRate2 + iRate3 )
		{
			// 2Ÿ�� ��ų �ߵ�
			TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 2 );
		}
		
		lpObj->m_ActState.Attack = 0;
		
		return _FAIL;
	}

	return 0;
}

int TMonsterAIElement::ApplyElementAIChange( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	//KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-AI����");
	TMonsterAIGroup::ChangeAIOrder( m_iTargetType, m_iX	);
	LogAddC(LOGC_RED, "[AI Change] Group %d AI Order %d", m_iTargetType, m_iX);
	return 0;
}

#endif // MODIFY_NEW_MONSTER_AI_01_2006724

#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725
