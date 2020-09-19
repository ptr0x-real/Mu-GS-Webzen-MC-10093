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

extern	void gObjCharZeroSet(int aIndex);				// 칼리마 게이트에 대한 정보를 바로 모두 삭제

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
	// 소환자의 주변 5x5 공간에 마석을 생성한다.
	BOOL bKalimaGateCreateSucceed	= FALSE;
	INT iMonsterIndex				= -1;

	EnterCriticalSection(&m_critKalimaGate);

	__try {
		if (!gObjIsConnected(iIndex)) {		// 사용자가 접속 중인지 확인한다.
			__leave; //return FALSE;
		}

		LogAddTD("[Kalima] [%s][%s] Try to Create Kalima Gate", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name
			);

		// 1 . 소환자가 마석을 소환할 수 있는 레벨인지 확인한다.
		INT iKalimaGateLevel = GetKalimaGateLevel(iIndex);

#ifdef HIDDEN_KALIMA_20050706
		if( iKalimaGateLevel < (btLevel - 1) )		// 사용자 자신의 게이트 레벨이 마석의 레벨과 같거나 높으면 통과
			__leave;
		iKalimaGateLevel = btLevel - 1;
#else
#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
		// 1-1.  EX ) 마석의 레벨과 소환자의 레벨을 확인한다.
		if( iKalimaGateLevel != (btLevel - 1) )
			__leave;
#endif
#endif

#ifdef ADD_MAPCHECK_OPEN_KALIMAGATE_20041129
		// 칼리마 게이트가 열릴수 있는 곳인지 검사한다.

		if( gObj[iIndex].m_stateEngagePVP == PVP_USER_PLAYING )
			__leave;

		// 악마의광장 이면
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( CHECK_DEVILSQUARE(gObj[iIndex].MapNumber) ) 
#else
		if( gObj[iIndex].MapNumber == 9 ) 
#endif
			__leave;

		// 블러드캐슬 이면
		if( CHECK_BLOODCASTLE( gObj[iIndex].MapNumber ) ) 
			__leave;

		// 카오스캐슬 이면
		if( CHECK_CHAOSCASTLE( gObj[iIndex].MapNumber ) ) 
			__leave;
#endif

#ifdef MODIFY_ILLUSION_TEMPLE_USE_KALIMA_GATE_BUGFIX_20080821
		// 환영사원 이면
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

		// 2 . 소환자가 안전지대에 있지 않은지 확인한다.
		INT iMapNumber		= gObj[iIndex].MapNumber;
		BYTE btMapAttr		= MapC[iMapNumber].GetAttr(gObj[iIndex].X, gObj[iIndex].Y);
		if( btMapAttr & MAP_ATTR_SAFTYZONE )
		{	// 안전지대에서는 마석을 소환할 수 없다.
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Called in Saftey Area (Map:%d, X:%d, Y:%d)", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				gObj[iIndex].MapNumber,
				gObj[iIndex].X,
				gObj[iIndex].Y
				);
			__leave;
		}

		// 3 . 소환자가 이미 생성한 마석이 있는지 확인한다.
		if (gObj[iIndex].m_cKalimaGateExist == (char)true) {			
				GCServerMsgStringSend(lMsg.Get(1248), iIndex, 1);		// "이미 소환된 마석이 있습니다."
				LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Already Have Gate (SummonIndex:%d)", 
					gObj[iIndex].AccountID, 
					gObj[iIndex].Name,
					gObj[iIndex].m_iKalimaGateIndex
					);
				__leave;
		}

		// 4 . 마석을 생성한다.
		INT iMonsterType = MONSTER_INDEX_KALIMA_1+iKalimaGateLevel;		// 실제 몬스터
				
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

		// *> 수정부분 (2004.09.13 - b4nfter)
		if( gObj[iIndex].MapNumber == 10	// 천공
			)
		{	
			// 소환할 수 없는 지역 체크
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Uable to Summon in this Map (MapNumber:%d)", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				gObj[iIndex].MapNumber
				);
			__leave;
		}

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
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
			gObj[iMonsterIndex].m_cKalimaGateEnterCount		= 0;								// 마석의 입장 가능횟수

			gObj[iIndex].m_cKalimaGateExist					= true;								// 다만 마석이 존재하는 것으로 체크한다.
			gObj[iIndex].m_iKalimaGateIndex					= iMonsterIndex;					// 마석의 인덱스를 별도록 기록

			gObj[iMonsterIndex].Level						= lpMATTR->m_Level;

			gObj[iMonsterIndex].Type						= OBJTYPE_MONSTER;	// 몬스터
			gObj[iMonsterIndex].MaxRegenTime				= 1000;
			gObj[iMonsterIndex].Dir							= 1;				// 방향은 화면을 기준으로 입구가 항상 아래로 향한다.
			
			gObj[iMonsterIndex].m_RecallMon					= iIndex;

			gObj[iMonsterIndex].RegenTime					= GetTickCount();	// 마석의 리젠시간은 마석이 처음 생긴 시간이고 나중에 + 생명주기와 함께 체크한다.

			gObj[iMonsterIndex].m_Attribute					= MON_ATTR_KALIMAGATE_1 + iKalimaGateLevel;

			gObj[iMonsterIndex].DieRegen					= 0;				// 이 수치가 초기화 되지 않아서 마석이 사라지는 버그발생 (BUGFIX_KALIMAGATE_VANISHING_20041006)

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
			// 마석이 성공적으로 생성됨
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

	// 소유주가 접속 상태가 아니라면 마석을 제거한다.
	if (!gObjIsConnected(gObj[iIndex].m_RecallMon)) {
		LogAddTD("[Kalima] Kalima Gate Vanished - Summoner Vanished (SummonIndex:%d, EnterCount:%d)", 
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// 마석을 없앤다.
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

	// 마석을 소환한 사람이 죽었다면 마석을 제거한다.
	if (gObj[gObj[iIndex].m_RecallMon].DieRegen > 0) {			// 자신을 소환한 사람이 죽었다면
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Summoner Died (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID,
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// 마석을 없앤다.
		DeleteKalimaGate (iIndex, gObj[iIndex].m_RecallMon);
		return;
	}


	// 마석의 입장 횟수가 N회를 넘었다면 제거한다.
	if (gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMAGATE_ENTER_COUNT) {
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Enter Count Over (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID,
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// 마석을 없앤다.
		DeleteKalimaGate (iIndex, gObj[iIndex].m_RecallMon);
		return;
	}


	// 생성 시간으로 부터 N분이 지났다면 없어진다.
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

		// 마석을 없앤다.
		DeleteKalimaGate (lpObj->m_Index, lpObjCallOwner->m_Index);
		return;
	}

	// 몬스터의 뷰포트 상에 아무도 없다면 끝난다.
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
					// 마석을 소환한 사람이거나 같은 파티원이라면 이동
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
						if( iOverLevel >= iMoveMapLevel )		// 자신에게 맞거나 낮은 레벨이라면 입장 가능하다.
#else
						if( iOverLevel == iMoveMapLevel )		// 자신에게 맞는 마석에만 입장할 수 있다.
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
	INT CKalimaGate::CheckOverlapKundunMark	(			// 쿤둔의 표식을 인벤토리에 겹칠 수 있는지 확인한다. 
		INT		iIndex,
		BYTE	btLevel
		)
#else
	INT CKalimaGate::CheckOverlapKundunMark	(			// 쿤둔의 표식을 인벤토리에 겹칠 수 있는지 확인한다. 
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
				&& gObj[iIndex].pInventory[x].m_Level == btLevel )				// 쿤둔의 표식이 존재하고 레벨이 같으면
#else
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 29) )		// 쿤둔의 표식이 존재한다면
#endif
			{
				INT iITEM_DUR = (INT) gObj[iIndex].pInventory[x].m_Durability;	// 아이템 개수 (내구도) 를 얻는다.

				if (CHECK_LIMIT(iITEM_DUR, MAX_KALIMAGATE_STONE_OVERLAP)) {
					// 만약 10개 미만이라면 여기에 더 넣을 수 있다.
					return x;
				}
			}
		}
	}

	return -1;
}


BOOL CKalimaGate::GetRandomLocation	(					// 사용자 반경내의 랜덤한 좌표를 얻어서 리턴한다.
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
		if( btMapAttr == MAP_ATTR_NONE )	// 이동가능한 지역이라면 성공
			return TRUE;
	}

	return FALSE;
}


INT	CKalimaGate::GetKalimaGateLevel	(					// 사용자가 사용 가능한 마석의 레벨을 얻는다.
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


BOOL CKalimaGate::DeleteKalimaGate	(					// 칼리마 게이트를 제거한다.
	INT		iKalimaGateIndex,
	INT		iCallOwnerIndex
	)
{
	// 이 함수가 실제로 마석을 제거하는 함수

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

		gObj[iCallOwnerIndex].m_cKalimaGateExist	= false;		// 마석이 주인에게 제거되는 유일한 조건이다.
		gObj[iCallOwnerIndex].m_iKalimaGateIndex	= -1;			// 마석이 주인에게 제거되는 유일한 조건이다.

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


BOOL CKalimaGate::DeleteKalimaGate (					// 칼리마 게이트를 제거한다.
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


#ifdef HIDDEN_KALIMA_20050706				// 다음 맵으로 보내는 칼리마 게이트의 생성, 동작 함수


BOOL CKalimaGate::CreateKalimaGate2	(		// 다음 맵으로 보내는 칼리마 게이트의 생성
	INT		iIndex,
	INT		iMonMapNumber,
	UCHAR	cTX, 
	UCHAR	cTY 
	)
{
	// 소환자의 주변 5x5 공간에 마석을 생성한다.
	BOOL bKalimaGateCreateSucceed	= FALSE;
	INT iMonsterIndex				= -1;

	EnterCriticalSection(&m_critKalimaGate);

	__try {
		if (!gObjIsConnected(iIndex)) {		// 사용자가 접속 중인지 확인한다.
			return FALSE;
		}

		LogAddTD("[Kalima] [%s][%s] Try to Create Kalima Gate (NextMap)", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name
			);

		// *> . 몬스터의 위치가 칼리마 1 ~ 5가 아니면 만들지 않는다.
		if (iMonMapNumber < MAP_INDEX_KALIMA1 || iMonMapNumber > MAP_INDEX_KALIMA5)	{
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate (NextMap) - MapNumber Error : %d", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				iMonMapNumber
				);
			return FALSE;
		}

		// *> . 소환자가 이미 생성한 마석이 있는지 확인한다.
		if (gObj[iIndex].m_cKalimaGateExist == (char)true) {			
				GCServerMsgStringSend(lMsg.Get(1248), iIndex, 1);		// "이미 소환된 마석이 있습니다."
				LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate (NextMap) - Already Have Gate (SummonIndex:%d)", 
					gObj[iIndex].AccountID, 
					gObj[iIndex].Name,
					gObj[iIndex].m_iKalimaGateIndex
					);
				return FALSE;
		}

		// *> . 마석을 생성한다.
		INT iMonsterType = MONSTER_INDEX_KALIMA_1 + iMonMapNumber - MAP_INDEX_KALIMA1 + 1;						// 실제 몬스터 (특별 마석은 무조건 마석1 ?)
				
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

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
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
			gObj[iMonsterIndex].m_cKalimaGateEnterCount		= 0;								// 마석의 입장 가능횟수

			gObj[iIndex].m_cKalimaGateExist					= true;								// 다만 마석이 존재하는 것으로 체크한다.
			gObj[iIndex].m_iKalimaGateIndex					= iMonsterIndex;					// 마석의 인덱스를 별도록 기록

			gObj[iMonsterIndex].Level						= lpMATTR->m_Level;

			gObj[iMonsterIndex].Type						= OBJTYPE_MONSTER;	// 몬스터
			gObj[iMonsterIndex].MaxRegenTime				= 1000;
			gObj[iMonsterIndex].Dir							= 1;				// 방향은 화면을 기준으로 입구가 항상 아래로 향한다.
			
			gObj[iMonsterIndex].m_RecallMon					= iIndex;

			gObj[iMonsterIndex].RegenTime					= GetTickCount();	// 마석의 리젠시간은 마석이 처음 생긴 시간이고 나중에 + 생명주기와 함께 체크한다.

			gObj[iMonsterIndex].m_Attribute					= MON_ATTR_KALIMAGATE_NXT;	// 특별 마석

			gObj[iMonsterIndex].DieRegen					= 0;				// 이 수치가 초기화 되지 않아서 마석이 사라지는 버그발생 (BUGFIX_KALIMAGATE_VANISHING_20041006)

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
			// 마석이 성공적으로 생성됨
		}
	}

	return TRUE;
}


VOID CKalimaGate::KalimaGateAct2	(		// 다음 맵으로 보내는 칼리마 게이트의 동작
	INT		iIndex
	)
{
	LPOBJECTSTRUCT lpObj = NULL;
	LPOBJECTSTRUCT lpObjCallOwner = NULL;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;

	if (gObj[iIndex].Live == 0)
		return;

	// 소유주가 접속 상태가 아니라면 마석을 제거한다.
	if (!gObjIsConnected(gObj[iIndex].m_RecallMon)) {
		LogAddTD("[Kalima] Kalima Gate Vanished (NextMap) - Summoner Vanished (SummonIndex:%d, EnterCount:%d)", 
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// 마석을 없앤다.
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

	// 마석을 소환한 사람이 죽었다면 마석을 제거한다.
/*
	if (gObj[gObj[iIndex].m_RecallMon].DieRegen > 0) {			// 자신을 소환한 사람이 죽었다면
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Summoner Died (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID,
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// 마석을 없앤다.
		DeleteKalimaGate (iIndex, gObj[iIndex].m_RecallMon);
		return;
	}
*/
	

	// 마석의 입장 횟수가 N회를 넘었다면 제거한다.
	if (gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMAGATE_ENTER_COUNT) {
		lpObjCallOwner->AccountID[MAX_IDSTRING]		= '\0';
		lpObjCallOwner->Name[MAX_IDSTRING]			= '\0';

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Enter Count Over (SummonIndex:%d, EnterCount:%d)", 
			lpObjCallOwner->AccountID,
			lpObjCallOwner->Name,
			iIndex,
			gObj[iIndex].m_cKalimaGateEnterCount
			);

		// 마석을 없앤다.
		DeleteKalimaGate (iIndex, gObj[iIndex].m_RecallMon);
		return;
	}


	// 생성 시간으로 부터 N분이 지났다면 없어진다. (특별 마석)
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

		// 마석을 없앤다.
		DeleteKalimaGate (lpObj->m_Index, lpObjCallOwner->m_Index);
		return;
	}

	// 몬스터의 뷰포트 상에 아무도 없다면 끝난다.
	if( lpObj->VPCount < 1 ) 
	{
		return;
	}

	// 이 칼리마 게이트는 1 ~ 5 칼리마까지만 작동된다.
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
					// 마석을 소환한 사람이거나 같은 파티원이라면 이동
					if ((lpObj->m_RecallMon == tObjNum) || ((gObj[tObjNum].PartyNumber != -1) && (lpObjCallOwner->PartyNumber == gObj[tObjNum].PartyNumber))) {
						INT iMoveMapLevel	= lpObj->MapNumber - MAP_INDEX_KALIMA1 + 1;		// 항상 다음 맵으로 보낸다.
						INT iGateNumber		= g_iKalimaGateGateIndex[iMoveMapLevel];

						// 특별 칼리마 게이트는 1 ~ 5칼리마 까지 생성되며, 2 ~ 6칼리마 까지 같은 파티원이면 제한없이 보낸다.
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
INT	CKalimaGate::GetKalimaGateLevel2(					// 사용자가 사용 가능한 마석의 레벨을 얻는다.
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


