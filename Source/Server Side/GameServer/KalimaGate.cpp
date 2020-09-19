// KalimaGate.cpp: implementation of the CKalimaGate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KalimaGate.h"

#ifdef ADD_NEW_MAP_KALIMA_20040518


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

#ifdef DEVILSQUARE_EXTEND_20050221
#include "DevilSquare.h"
#endif

extern	CMonsterAttr	gMAttr;

extern	void gObjCharZeroSet(int aIndex);				// Į���� ����Ʈ�� ���� ������ �ٷ� ��� ����

CKalimaGate	g_KalimaGate;


#ifdef ADD_MAPCHECK_OPEN_KALIMAGATE_20041129
#include "BloodCastle.h"
#include "ChaosCastle.h"
#endif

#ifdef MODIFY_ILLUSION_TEMPLE_USE_KALIMA_GATE_BUGFIX_20080821
#include "IllusionTempleProcess.h"
#endif // MODIFY_ILLUSION_TEMPLE_USE_KALIMA_GATE_BUGFIX_20080821

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKalimaGate::CKalimaGate()
{
	InitializeCriticalSection(&m_critKalimaGate);
}


CKalimaGate::~CKalimaGate()
{
	DeleteCriticalSection(&m_critKalimaGate);
}

#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
	BOOL CKalimaGate::CreateKalimaGate	(INT iIndex, BYTE btLevel, UCHAR cTX, UCHAR cTY)
#else
	BOOL CKalimaGate::CreateKalimaGate	(INT iIndex, UCHAR cTX, UCHAR cTY)
#endif
{
	// ��ȯ���� �ֺ� 5x5 ������ ������ �����Ѵ�.
	BOOL bKalimaGateCreateSucceed	= FALSE;
	INT iMonsterIndex				= -1;

	EnterCriticalSection(&m_critKalimaGate);

	__try {
		if (!gObjIsConnected(iIndex)) {		// ����ڰ� ���� ������ Ȯ���Ѵ�.
			__leave; //return FALSE;
		}

		LogAddTD("[Kalima] [%s][%s] Try to Create Kalima Gate", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name
			);

		// 1 . ��ȯ�ڰ� ������ ��ȯ�� �� �ִ� �������� Ȯ���Ѵ�.
		INT iKalimaGateLevel = GetKalimaGateLevel(iIndex);

#ifdef HIDDEN_KALIMA_20050706
		if( iKalimaGateLevel < (btLevel - 1) )		// ����� �ڽ��� ����Ʈ ������ ������ ������ ���ų� ������ ���
			__leave;
		iKalimaGateLevel = btLevel - 1;
#else
#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
		// 1-1.  EX ) ������ ������ ��ȯ���� ������ Ȯ���Ѵ�.
		if( iKalimaGateLevel != (btLevel - 1) )
			__leave;
#endif
#endif

#ifdef ADD_MAPCHECK_OPEN_KALIMAGATE_20041129
		// Į���� ����Ʈ�� ������ �ִ� ������ �˻��Ѵ�.

		if( gObj[iIndex].m_stateEngagePVP == PVP_USER_PLAYING )
			__leave;

		// �Ǹ��Ǳ��� �̸�
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
		if( CHECK_DEVILSQUARE(gObj[iIndex].MapNumber) ) 
#else
		if( gObj[iIndex].MapNumber == 9 ) 
#endif
			__leave;

		// ����ĳ�� �̸�
		if( CHECK_BLOODCASTLE( gObj[iIndex].MapNumber ) ) 
			__leave;

		// ī����ĳ�� �̸�
		if( CHECK_CHAOSCASTLE( gObj[iIndex].MapNumber ) ) 
			__leave;
#endif

#ifdef MODIFY_ILLUSION_TEMPLE_USE_KALIMA_GATE_BUGFIX_20080821
		// ȯ����� �̸�
		if( CHECK_ILLUSION_TEMPLE( gObj[iIndex].MapNumber ) )
			__leave;
#endif // MODIFY_ILLUSION_TEMPLE_USE_KALIMA_GATE_BUGFIX_20080821

		if (!CHECK_LIMIT(iKalimaGateLevel, MAX_KALIMAGATE_COUNT)) {

			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Gate Level Check Error : %d", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				iKalimaGateLevel+1
				);
			__leave;
		}

		// 2 . ��ȯ�ڰ� �������뿡 ���� ������ Ȯ���Ѵ�.
		INT iMapNumber		= gObj[iIndex].MapNumber;
		BYTE btMapAttr		= MapC[iMapNumber].GetAttr(gObj[iIndex].X, gObj[iIndex].Y);
		if( btMapAttr & MAP_ATTR_SAFTYZONE )
		{	// �������뿡���� ������ ��ȯ�� �� ����.
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Called in Saftey Area (Map:%d, X:%d, Y:%d)", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				gObj[iIndex].MapNumber,
				gObj[iIndex].X,
				gObj[iIndex].Y
				);
			__leave;
		}

		// 3 . ��ȯ�ڰ� �̹� ������ ������ �ִ��� Ȯ���Ѵ�.
		if (gObj[iIndex].m_cKalimaGateExist == (char)true) {			
				GCServerMsgStringSend(lMsg.Get(1248), iIndex, 1);		// "�̹� ��ȯ�� ������ �ֽ��ϴ�."
				LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Already Have Gate (SummonIndex:%d)", 
					gObj[iIndex].AccountID, 
					gObj[iIndex].Name,
					gObj[iIndex].m_iKalimaGateIndex
					);
				__leave;
		}

		// 4 . ������ �����Ѵ�.
		INT iMonsterType = MONSTER_INDEX_KALIMA_1+iKalimaGateLevel;		// ���� ����
				
		UCHAR cX = cTX;
		UCHAR cY = cTY;

		btMapAttr = MapC[iMapNumber].GetAttr(cX, cY);
		if( btMapAttr != MAP_ATTR_NONE ) {
			if (!GetRandomLocation(iMapNumber, cX, cY)) {
				LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Failed to get Location (MapNumber:%d, X:%d, Y:%d)", 
					gObj[iIndex].AccountID, 
					gObj[iIndex].Name,
					gObj[iIndex].MapNumber,
					cX, cY
					);
				__leave;
			}
		}

		// *> �����κ� (2004.09.13 - b4nfter)
		if( gObj[iIndex].MapNumber == 10	// õ��
			)
		{	
			// ��ȯ�� �� ���� ���� üũ
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Uable to Summon in this Map (MapNumber:%d)", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				gObj[iIndex].MapNumber
				);
			__leave;
		}

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
		iMonsterIndex = gObjAddMonster(iMapNumber);
#else
		iMonsterIndex = gObjAdd();
#endif
		if( iMonsterIndex >= 0 )
		{
			LPMONSTER_ATTRIBUTE		lpMATTR;
			lpMATTR = gMAttr.GetAttr(iMonsterType);

			if (lpMATTR == NULL) {
				gObjDel (iMonsterIndex);
				LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - lpMATTR == NULL (SummonIndex:%d)", 
					gObj[iIndex].AccountID, 
					gObj[iIndex].Name,
					iMonsterIndex
					);
				__leave;
			}

			gObjSetMonster(iMonsterIndex, iMonsterType);

			gObj[iMonsterIndex].Live						= 1;
			gObj[iMonsterIndex].Life						= 1000.0;
			gObj[iMonsterIndex].MaxLife						= 1000.0;

			gObj[iMonsterIndex].m_PosNum					= -1;
			gObj[iMonsterIndex].X							= cX;
			gObj[iMonsterIndex].Y							= cY;
			gObj[iMonsterIndex].MTX							= cX;
			gObj[iMonsterIndex].MTY							= cY;
			gObj[iMonsterIndex].TX							= cX;
			gObj[iMonsterIndex].TY							= cY;
			gObj[iMonsterIndex].m_OldX						= cX;
			gObj[iMonsterIndex].m_OldY						= cY;
			gObj[iMonsterIndex].StartX						= cX;
			gObj[iMonsterIndex].StartY						= cY;
			gObj[iMonsterIndex].MapNumber					= iMapNumber;
			gObj[iMonsterIndex].m_MoveRange					= 0;
			gObj[iMonsterIndex].m_cKalimaGateEnterCount		= 0;								// ������ ���� ����Ƚ��

			gObj[iIndex].m_cKalimaGateExist					= true;								// �ٸ� ������ �����ϴ� ������ üũ�Ѵ�.
			gObj[iIndex].m_iKalimaGateIndex					= iMonsterIndex;					// ������ �ε����� ������ ���

			gObj[iMonsterIndex].Level						= lpMATTR->m_Level;

			gObj[iMonsterIndex].Type						= OBJTYPE_MONSTER;	// ����
			gObj[iMonsterIndex].MaxRegenTime				= 1000;
			gObj[iMonsterIndex].Dir							= 1;				// ������ ȭ���� �������� �Ա��� �׻� �Ʒ��� ���Ѵ�.
			
			gObj[iMonsterIndex].m_RecallMon					= iIndex;

			gObj[iMonsterIndex].RegenTime					= GetTickCount();	// ������ �����ð��� ������ ó�� ���� �ð��̰� ���߿� + �����ֱ�� �Բ� üũ�Ѵ�.

			gObj[iMonsterIndex].m_Attribute					= MON_ATTR_KALIMAGATE_1 + iKalimaGateLevel;

			gObj[iMonsterIndex].DieRegen					= 0;				// �� ��ġ�� �ʱ�ȭ ���� �ʾƼ� ������ ������� ���׹߻� (BUGFIX_KALIMAGATE_VANISHING_20041006)

			LogAddTD("[Kalima] [%s][%s] Create Kalima Gate Successfuly (Live:%d, GateTick:%u, DieRegen:%d, Map:%d, X:%d, Y:%d, SummonIndex:%d, OwnerIndex:%d, GateNo.:%d)", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				gObj[iMonsterIndex].Live,
				gObj[iMonsterIndex].RegenTime,
				gObj[iMonsterIndex].DieRegen,
				gObj[iMonsterIndex].MapNumber,
				gObj[iMonsterIndex].X,
				gObj[iMonsterIndex].Y,
				iMonsterIndex,
				iIndex,
				iKalimaGateLevel+1
				);
			bKalimaGateCreateSucceed	= TRUE;
		}
	}
	__finally {
		LeaveCriticalSection(&m_critKalimaGate);

		if (bKalimaGateCreateSucceed){
			// ������ ���������� ������
		}
	}

	return bKalimaGateCreateSucceed;
}


VOID CKalimaGate::KalimaGateAct		(INT iIndex)
{
	LPOBJECTSTRUCT lpObj = NULL;
	LPOBJECTSTRUCT lpObjCallOwner = NULL;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;

	if (gObj[iIndex].Live == 0)
		return;

	// �����ְ� ���� ���°� �ƴ϶�� ������ �����Ѵ�.
	if (!gObjIsConnected(gObj[iIndex].m_RecallMon)) {
		LogAddTD("[Kalima] Kalima Gate Vanished - Summoner Vanished (SummonIndex:%d, EnterCount:%d)", 
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// ������ ���ش�.
		GCDiePlayerSend (&gObj[iIndex], iIndex, 0, 0);
		gObjDel(iIndex);
		gObjCharZeroSet(iIndex);
//		gObj[iIndex].m_RecallMon = -1;
//		gObjDel(gObj[iIndex].m_Index);
		return;
	}
	else {
		lpObj = &gObj[iIndex];
		lpObjCallOwner = &gObj[lpObj->m_RecallMon];
	}

	// ������ ��ȯ�� ����� �׾��ٸ� ������ �����Ѵ�.
	if (gObj[gObj[iIndex].m_RecallMon].DieRegen > 0) {			// �ڽ��� ��ȯ�� ����� �׾��ٸ�
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Summoner Died (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID,
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// ������ ���ش�.
		DeleteKalimaGate (iIndex, gObj[iIndex].m_RecallMon);
		return;
	}


	// ������ ���� Ƚ���� Nȸ�� �Ѿ��ٸ� �����Ѵ�.
	if (gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMAGATE_ENTER_COUNT) {
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Enter Count Over (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID,
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// ������ ���ش�.
		DeleteKalimaGate (iIndex, gObj[iIndex].m_RecallMon);
		return;
	}


	// ���� �ð����� ���� N���� �����ٸ� ��������.
	DWORD dwTickCount = GetTickCount();
	if (dwTickCount > lpObj->RegenTime + MAX_KALIMAGATE_LIFE_TICKTERM) {
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Time Out (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID, 
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// ������ ���ش�.
		DeleteKalimaGate (lpObj->m_Index, lpObjCallOwner->m_Index);
		return;
	}

	// ������ ����Ʈ �� �ƹ��� ���ٸ� ������.
	if( lpObj->VPCount < 1 ) return;


	INT	tObjNum	= -1;
	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
	{
		tObjNum = lpObj->VpPlayer[i].number;
		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
			{
//				if( (lpObj->Y == gObj[tObjNum].Y) && (lpObj->X == gObj[tObjNum].X) )
				if (abs(lpObj->Y - gObj[tObjNum].Y) < 2 && abs(lpObj->X - gObj[tObjNum].X) < 2 )
				{
					// ������ ��ȯ�� ����̰ų� ���� ��Ƽ���̶�� �̵�
					if ((lpObj->m_RecallMon == tObjNum) || ((gObj[tObjNum].PartyNumber != -1) && (lpObjCallOwner->PartyNumber == gObj[tObjNum].PartyNumber))) {
						INT iMoveMapLevel	= lpObj->m_Attribute - MON_ATTR_KALIMAGATE_1;
						INT iMapNumber		= MAP_INDEX_KALIMA1 + iMoveMapLevel;
#ifdef HIDDEN_KALIMA_20050706
						INT iGateNumber		= g_iKalimaGateGateIndex[iMoveMapLevel];
#else
						INT iGateNumber		= GATE_INDEX_KALIMA_1 + iMoveMapLevel;
#endif
						INT iOverLevel		= GetKalimaGateLevel(tObjNum);

#ifdef HIDDEN_KALIMA_20050706
						if( iOverLevel >= iMoveMapLevel )		// �ڽſ��� �°ų� ���� �����̶�� ���� �����ϴ�.
#else
						if( iOverLevel == iMoveMapLevel )		// �ڽſ��� �´� �������� ������ �� �ִ�.
#endif
						{							
							if( !gObjMoveGate(tObjNum, iGateNumber) ) {
								LogAddTD("[Kalima] [%s][%s] - [%s][%s] move fail (MapNumber:%d)", 
									lpObjCallOwner->AccountID, 
									lpObjCallOwner->Name,
									gObj[tObjNum].AccountID, 
									gObj[tObjNum].Name,
									iMapNumber)
									;
							}
							else {
								LogAddTD("[Kalima] [%s][%s] - [%s][%s] Transported by Kalima Gate (SummonIndex:%d, GateNo.:%d, MapNumber:%d)", 
									lpObjCallOwner->AccountID, 
									lpObjCallOwner->Name,
									gObj[tObjNum].AccountID, 
									gObj[tObjNum].Name,
									iIndex,
									iMoveMapLevel+1,
									iMapNumber
									);
								
								gObj[iIndex].m_cKalimaGateEnterCount++;
								if (gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMAGATE_ENTER_COUNT)
									return;
							}
						}
					}
				}
			}
		}
	}
}


#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
	INT CKalimaGate::CheckOverlapKundunMark	(			// ����� ǥ���� �κ��丮�� ��ĥ �� �ִ��� Ȯ���Ѵ�. 
		INT		iIndex,
		BYTE	btLevel
		)
#else
	INT CKalimaGate::CheckOverlapKundunMark	(			// ����� ǥ���� �κ��丮�� ��ĥ �� �ִ��� Ȯ���Ѵ�. 
		INT		iIndex
		)
#endif

{
	for(int x=0; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 29) 
				&& gObj[iIndex].pInventory[x].m_Level == btLevel )				// ����� ǥ���� �����ϰ� ������ ������
#else
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 29) )		// ����� ǥ���� �����Ѵٸ�
#endif
			{
				INT iITEM_DUR = (INT) gObj[iIndex].pInventory[x].m_Durability;	// ������ ���� (������) �� ��´�.

				if (CHECK_LIMIT(iITEM_DUR, MAX_KALIMAGATE_STONE_OVERLAP)) {
					// ���� 10�� �̸��̶�� ���⿡ �� ���� �� �ִ�.
					return x;
				}
			}
		}
	}

	return -1;
}


BOOL CKalimaGate::GetRandomLocation	(					// ����� �ݰ泻�� ������ ��ǥ�� �� �����Ѵ�.
	INT		iMapNumber,
	UCHAR & cX,
	UCHAR & cY
	)
{
	INT iCount = 100;
	INT iUX = cX;
	INT iUY = cY;

	while (iCount-- > 0) {
		cX = (rand() % (KALIMAGATE_SUMMON_FIELDSIZE + 1)) * ((rand() % 2) ? 1 : -1) + iUX;
		cY = (rand() % (KALIMAGATE_SUMMON_FIELDSIZE + 1)) * ((rand() % 2) ? 1 : -1) + iUY;

		BYTE btMapAttr = MapC[iMapNumber].GetAttr(cX, cY);
		if( btMapAttr == MAP_ATTR_NONE )	// �̵������� �����̶�� ����
			return TRUE;
	}

	return FALSE;
}


INT	CKalimaGate::GetKalimaGateLevel	(					// ����ڰ� ��� ������ ������ ������ ��´�.
	INT		iIndex
	)
{
	if (gObj[iIndex].Class == CLASS_MAGUMSA || gObj[iIndex].Class == CLASS_DARKLORD) {
		for (INT iLevel = 0 ; iLevel < MAX_KALIMAGATE_COUNT ; iLevel++) {
			if ( (gObj[iIndex].Level >= g_sttKALIMAGATE_LEVEL[iLevel].iLOWER_BOUND_MAGUMSA) && (gObj[iIndex].Level <= g_sttKALIMAGATE_LEVEL[iLevel].iUPPER_BOUND_MAGUMSA))
				return iLevel;
		}
	}
	else {
		for (INT iLevel = 0 ; iLevel < MAX_KALIMAGATE_COUNT ; iLevel++) {
			if ( (gObj[iIndex].Level >= g_sttKALIMAGATE_LEVEL[iLevel].iLOWER_BOUND) && (gObj[iIndex].Level <= g_sttKALIMAGATE_LEVEL[iLevel].iUPPER_BOUND))
				return iLevel;
		}
	}
	
	return -1;
}


BOOL CKalimaGate::DeleteKalimaGate	(					// Į���� ����Ʈ�� �����Ѵ�.
	INT		iKalimaGateIndex,
	INT		iCallOwnerIndex
	)
{
	// �� �Լ��� ������ ������ �����ϴ� �Լ�

	EnterCriticalSection(&m_critKalimaGate);

	BOOL ret = FALSE;

	__try {
		if(
			!CHECK_LIMIT(iKalimaGateIndex, MAX_OBJECT) ||
			!CHECK_LIMIT(iCallOwnerIndex, MAX_OBJECT)
			) 
		{
			LogAddTD("[Kalima] DeleteKalimaGate() - out of Index (iKalimaGateIndex:%d, iCallOwnerIndex:%d)",
				iKalimaGateIndex,
				iCallOwnerIndex
				);
			__leave;
		}
			
		GCDiePlayerSend (&gObj[iKalimaGateIndex], iKalimaGateIndex, 0, 0);

//		lpObj->Live							= 0;
//		lpObj->Life							= -1;
//		lpObj->m_RecallMon					= -1;
//		lpObjCallOwner->m_RecallMon			= -1;

		gObj[iCallOwnerIndex].m_cKalimaGateExist	= false;		// ������ ���ο��� ���ŵǴ� ������ �����̴�.
		gObj[iCallOwnerIndex].m_iKalimaGateIndex	= -1;			// ������ ���ο��� ���ŵǴ� ������ �����̴�.

		gObjDel(iKalimaGateIndex);
		gObjCharZeroSet(iKalimaGateIndex);

		ret = TRUE;
//		gObjMonsterCallKill(lpObjCallOwner->m_Index);
	}
	__finally {
		LeaveCriticalSection(&m_critKalimaGate);
	}

	return ret;
}


BOOL CKalimaGate::DeleteKalimaGate (					// Į���� ����Ʈ�� �����Ѵ�.
	INT		iCallOwnerIndex
	)
{
	if(!CHECK_LIMIT(iCallOwnerIndex, MAX_OBJECT)) 
	{
		LogAddTD("[Kalima] DeleteKalimaGate() - out of Index (iCallOwnerIndex:%d)",
			iCallOwnerIndex
			);
		return FALSE;
	}

	INT iKalimaGateIndex = gObj[iCallOwnerIndex].m_iKalimaGateIndex;
	if(!CHECK_LIMIT(iKalimaGateIndex, MAX_OBJECT)) 
	{
		return FALSE;
	}

	if (
		gObj[iKalimaGateIndex].Live == 0 ||
		!CHECK_KALIMAGATE(gObj[iKalimaGateIndex].m_Attribute)
		) 
	{
		return FALSE;
	}

	return DeleteKalimaGate (iKalimaGateIndex, iCallOwnerIndex);
}


#ifdef HIDDEN_KALIMA_20050706				// ���� ������ ������ Į���� ����Ʈ�� ����, ���� �Լ�


BOOL CKalimaGate::CreateKalimaGate2	(		// ���� ������ ������ Į���� ����Ʈ�� ����
	INT		iIndex,
	INT		iMonMapNumber,
	UCHAR	cTX, 
	UCHAR	cTY 
	)
{
	// ��ȯ���� �ֺ� 5x5 ������ ������ �����Ѵ�.
	BOOL bKalimaGateCreateSucceed	= FALSE;
	INT iMonsterIndex				= -1;

	EnterCriticalSection(&m_critKalimaGate);

	__try {
		if (!gObjIsConnected(iIndex)) {		// ����ڰ� ���� ������ Ȯ���Ѵ�.
			return FALSE;
		}

		LogAddTD("[Kalima] [%s][%s] Try to Create Kalima Gate (NextMap)", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name
			);

		// *> . ������ ��ġ�� Į���� 1 ~ 5�� �ƴϸ� ������ �ʴ´�.
		if (iMonMapNumber < MAP_INDEX_KALIMA1 || iMonMapNumber > MAP_INDEX_KALIMA5)	{
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate (NextMap) - MapNumber Error : %d", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				iMonMapNumber
				);
			return FALSE;
		}

		// *> . ��ȯ�ڰ� �̹� ������ ������ �ִ��� Ȯ���Ѵ�.
		if (gObj[iIndex].m_cKalimaGateExist == (char)true) {			
				GCServerMsgStringSend(lMsg.Get(1248), iIndex, 1);		// "�̹� ��ȯ�� ������ �ֽ��ϴ�."
				LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate (NextMap) - Already Have Gate (SummonIndex:%d)", 
					gObj[iIndex].AccountID, 
					gObj[iIndex].Name,
					gObj[iIndex].m_iKalimaGateIndex
					);
				return FALSE;
		}

		// *> . ������ �����Ѵ�.
		INT iMonsterType = MONSTER_INDEX_KALIMA_1 + iMonMapNumber - MAP_INDEX_KALIMA1 + 1;						// ���� ���� (Ư�� ������ ������ ����1 ?)
				
		UCHAR cX = cTX;
		UCHAR cY = cTY;

		BYTE btMapAttr = MapC[iMonMapNumber].GetAttr(cX, cY);
		if( btMapAttr != MAP_ATTR_NONE ) {
			if (!GetRandomLocation(iMonMapNumber, cX, cY)) {
				LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate (NextMap) - Failed to get Location (MapNumber:%d, X:%d, Y:%d)", 
					gObj[iIndex].AccountID, 
					gObj[iIndex].Name,
					iMonMapNumber,
					cX, cY
					);
				return FALSE;
			}
		}

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
		iMonsterIndex = gObjAddMonster(iMonMapNumber);
#else
		iMonsterIndex = gObjAdd();
#endif
		if( iMonsterIndex >= 0 )
		{
			LPMONSTER_ATTRIBUTE		lpMATTR;
			lpMATTR = gMAttr.GetAttr(iMonsterType);

			if (lpMATTR == NULL) {
				gObjDel (iMonsterIndex);
				LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished (NextMap) - lpMATTR == NULL (SummonIndex:%d)", 
					gObj[iIndex].AccountID, 
					gObj[iIndex].Name,
					iMonsterIndex
					);
				return FALSE;
			}

			gObjSetMonster(iMonsterIndex, iMonsterType);

			gObj[iMonsterIndex].Live						= 1;
			gObj[iMonsterIndex].Life						= 1000.0;
			gObj[iMonsterIndex].MaxLife						= 1000.0;

			gObj[iMonsterIndex].m_PosNum					= -1;
			gObj[iMonsterIndex].X							= cX;
			gObj[iMonsterIndex].Y							= cY;
			gObj[iMonsterIndex].MTX							= cX;
			gObj[iMonsterIndex].MTY							= cY;
			gObj[iMonsterIndex].TX							= cX;
			gObj[iMonsterIndex].TY							= cY;
			gObj[iMonsterIndex].m_OldX						= cX;
			gObj[iMonsterIndex].m_OldY						= cY;
			gObj[iMonsterIndex].StartX						= cX;
			gObj[iMonsterIndex].StartY						= cY;
			gObj[iMonsterIndex].MapNumber					= iMonMapNumber;
			gObj[iMonsterIndex].m_MoveRange					= 0;
			gObj[iMonsterIndex].m_cKalimaGateEnterCount		= 0;								// ������ ���� ����Ƚ��

			gObj[iIndex].m_cKalimaGateExist					= true;								// �ٸ� ������ �����ϴ� ������ üũ�Ѵ�.
			gObj[iIndex].m_iKalimaGateIndex					= iMonsterIndex;					// ������ �ε����� ������ ���

			gObj[iMonsterIndex].Level						= lpMATTR->m_Level;

			gObj[iMonsterIndex].Type						= OBJTYPE_MONSTER;	// ����
			gObj[iMonsterIndex].MaxRegenTime				= 1000;
			gObj[iMonsterIndex].Dir							= 1;				// ������ ȭ���� �������� �Ա��� �׻� �Ʒ��� ���Ѵ�.
			
			gObj[iMonsterIndex].m_RecallMon					= iIndex;

			gObj[iMonsterIndex].RegenTime					= GetTickCount();	// ������ �����ð��� ������ ó�� ���� �ð��̰� ���߿� + �����ֱ�� �Բ� üũ�Ѵ�.

			gObj[iMonsterIndex].m_Attribute					= MON_ATTR_KALIMAGATE_NXT;	// Ư�� ����

			gObj[iMonsterIndex].DieRegen					= 0;				// �� ��ġ�� �ʱ�ȭ ���� �ʾƼ� ������ ������� ���׹߻� (BUGFIX_KALIMAGATE_VANISHING_20041006)

			LogAddTD("[Kalima] [%s][%s] Create Kalima Gate Successfuly (NextMap) (Live:%d, GateTick:%u, DieRegen:%d, Map:%d, X:%d, Y:%d, SummonIndex:%d, OwnerIndex:%d)", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				gObj[iMonsterIndex].Live,
				gObj[iMonsterIndex].RegenTime,
				gObj[iMonsterIndex].DieRegen,
				gObj[iMonsterIndex].MapNumber,
				gObj[iMonsterIndex].X,
				gObj[iMonsterIndex].Y,
				iMonsterIndex,
				iIndex
				);
			bKalimaGateCreateSucceed	= TRUE;
		}
	}
	__finally {
		LeaveCriticalSection(&m_critKalimaGate);

		if (bKalimaGateCreateSucceed){
			// ������ ���������� ������
		}
	}

	return TRUE;
}


VOID CKalimaGate::KalimaGateAct2	(		// ���� ������ ������ Į���� ����Ʈ�� ����
	INT		iIndex
	)
{
	LPOBJECTSTRUCT lpObj = NULL;
	LPOBJECTSTRUCT lpObjCallOwner = NULL;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;

	if (gObj[iIndex].Live == 0)
		return;

	// �����ְ� ���� ���°� �ƴ϶�� ������ �����Ѵ�.
	if (!gObjIsConnected(gObj[iIndex].m_RecallMon)) {
		LogAddTD("[Kalima] Kalima Gate Vanished (NextMap) - Summoner Vanished (SummonIndex:%d, EnterCount:%d)", 
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// ������ ���ش�.
		GCDiePlayerSend (&gObj[iIndex], iIndex, 0, 0);
		gObjDel(iIndex);
		gObjCharZeroSet(iIndex);
//		gObj[iIndex].m_RecallMon = -1;
//		gObjDel(gObj[iIndex].m_Index);
		return;
	}
	else {
		lpObj = &gObj[iIndex];
		lpObjCallOwner = &gObj[lpObj->m_RecallMon];
	}

	// ������ ��ȯ�� ����� �׾��ٸ� ������ �����Ѵ�.
/*
	if (gObj[gObj[iIndex].m_RecallMon].DieRegen > 0) {			// �ڽ��� ��ȯ�� ����� �׾��ٸ�
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Summoner Died (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID,
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// ������ ���ش�.
		DeleteKalimaGate (iIndex, gObj[iIndex].m_RecallMon);
		return;
	}
*/
	

	// ������ ���� Ƚ���� Nȸ�� �Ѿ��ٸ� �����Ѵ�.
	if (gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMAGATE_ENTER_COUNT) {
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Enter Count Over (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID,
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// ������ ���ش�.
		DeleteKalimaGate (iIndex, gObj[iIndex].m_RecallMon);
		return;
	}


	// ���� �ð����� ���� N���� �����ٸ� ��������. (Ư�� ����)
	DWORD dwTickCount = GetTickCount();
	if (dwTickCount > lpObj->RegenTime + MAX_KALIMAGATE_LIFE_TICKTERM2) {
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished (NextMap) - Time Out (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID, 
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// ������ ���ش�.
		DeleteKalimaGate (lpObj->m_Index, lpObjCallOwner->m_Index);
		return;
	}

	// ������ ����Ʈ �� �ƹ��� ���ٸ� ������.
	if( lpObj->VPCount < 1 ) 
	{
		return;
	}

	// �� Į���� ����Ʈ�� 1 ~ 5 Į���������� �۵��ȴ�.
	if (lpObj->MapNumber < MAP_INDEX_KALIMA1 || lpObj->MapNumber > MAP_INDEX_KALIMA5)	
		return;

	INT	tObjNum	= -1;
	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
	{
		tObjNum = lpObj->VpPlayer[i].number;
		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
			{
				if (abs(lpObj->Y - gObj[tObjNum].Y) < 2 && abs(lpObj->X - gObj[tObjNum].X) < 2 )
				{
					// ������ ��ȯ�� ����̰ų� ���� ��Ƽ���̶�� �̵�
					if ((lpObj->m_RecallMon == tObjNum) || ((gObj[tObjNum].PartyNumber != -1) && (lpObjCallOwner->PartyNumber == gObj[tObjNum].PartyNumber))) {
						INT iMoveMapLevel	= lpObj->MapNumber - MAP_INDEX_KALIMA1 + 1;		// �׻� ���� ������ ������.
						INT iGateNumber		= g_iKalimaGateGateIndex[iMoveMapLevel];

						// Ư�� Į���� ����Ʈ�� 1 ~ 5Į���� ���� �����Ǹ�, 2 ~ 6Į���� ���� ���� ��Ƽ���̸� ���Ѿ��� ������.
						if( !gObjMoveGate(tObjNum, iGateNumber) ) {
							LogAddTD("[Kalima] [%s][%s] - [%s][%s] move fail (MapNumber:%d)", 
								lpObjCallOwner->AccountID, 
								lpObjCallOwner->Name,
								gObj[tObjNum].AccountID, 
								gObj[tObjNum].Name,
								lpObj->MapNumber);
						}
						else {
							LogAddTD("[Kalima] [%s][%s] - [%s][%s] Transported by Kalima Gate (NextMap) (SummonIndex:%d, GateNo.:%d, MapNumber:%d)", 
								lpObjCallOwner->AccountID, 
								lpObjCallOwner->Name,
								gObj[tObjNum].AccountID, 
								gObj[tObjNum].Name,
								iIndex,
								iMoveMapLevel+1,
								lpObj->MapNumber
								);
							
							gObj[iIndex].m_cKalimaGateEnterCount++;
							if (gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMAGATE_ENTER_COUNT)
								return;
						}
					}
				}
			}
		}
	}
}

#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
INT	CKalimaGate::GetKalimaGateLevel2(					// ����ڰ� ��� ������ ������ ������ ��´�.
	INT		iIndex
	)
{
	if (gObj[iIndex].Class == CLASS_MAGUMSA || gObj[iIndex].Class == CLASS_DARKLORD) {
		for (INT iLevel = 0 ; iLevel < MAX_KALIMAGATE_COUNT ; iLevel++) {
			if ( (gObj[iIndex].Level >= g_sttKALIMAGATE_LEVEL[iLevel].iLOWER_BOUND_MAGUMSA) && (gObj[iIndex].Level <= g_sttKALIMAGATE_LEVEL[iLevel].iUPPER_BOUND_MAGUMSA))
				return iLevel;
		}
	}
	else {
		for (INT iLevel = 0 ; iLevel < MAX_KALIMAGATE_COUNT ; iLevel++) {
			if ( (gObj[iIndex].Level >= g_sttKALIMAGATE_LEVEL[iLevel].iLOWER_BOUND) && (gObj[iIndex].Level <= g_sttKALIMAGATE_LEVEL[iLevel].iUPPER_BOUND))
				return iLevel;
		}
	}
	
	return -1;
}
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

#endif


#endif


