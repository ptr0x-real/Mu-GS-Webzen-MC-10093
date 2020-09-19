// KanturuEntranceNPC.cpp: implementation of the CKanturuEntranceNPC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuEntranceNPC.h"

#include "Kanturu.h"
extern CKanturu	g_Kanturu;

#include "KanturuUtil.h"
static CKanturuUtil	KANTURU_UTIL;

CKanturuEntranceNPC	g_KanturuEntranceNPC;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuEntranceNPC::CKanturuEntranceNPC()
{

}

CKanturuEntranceNPC::~CKanturuEntranceNPC()
{

}

//---------------------------------------------------------------------------------
// GC [0xD1][0x00]클라이언트가 요청 할 경우 현재 칸투르 상태와 입장가능 여부, 상태별 남은 시간에 대한 정보를 보낸다.
//---------------------------------------------------------------------------------
void CKanturuEntranceNPC::NotifyEntranceInfo( int iIndex )
{
	PMSG_ANS_KANTURU_STATE_INFO		pMsg = {0. };	
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x00, sizeof( pMsg ) );

	int iCurrentState		= g_Kanturu.GetKanturuState();
	int iCurrentDetailState = g_Kanturu.GetKanturuDetailState();	

	pMsg.btState			= iCurrentState;
	pMsg.btDetailState		= iCurrentDetailState;
	pMsg.btEnter			= g_Kanturu.CheckCanEnterKanturuBattle();
	pMsg.btUserCount		= g_KanturuBattleUserMng.GetUserCount();
	pMsg.iRemainTime		= g_Kanturu.GetRemainTime();

	KANTURU_UTIL.SendDataToUser( iIndex, ( LPBYTE )&pMsg, sizeof( pMsg ) );	
}

//---------------------------------------------------------------------------------
// GC [0xD1][0x01] 클라이언트가 칸투르 보스전 전투맵에 입장 요청을 한 경우 응답
//---------------------------------------------------------------------------------
void CKanturuEntranceNPC::NotifyResultEnterKanturuBossMap( int iIndex )
{
	if( !gObjIsConnected( iIndex ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	BOOL	bMoveGateSuccess = FALSE;

	int iResult = g_Kanturu.CheckEnterKanturu( iIndex );
	if( iResult == 0 )	// 입장 가능
	{
		int iState = g_Kanturu.GetKanturuState();
		if( iState == KANTURU_STATE_BATTLE_OF_MAYA )
		{
			bMoveGateSuccess = gObjMoveGate( iIndex, KANTURU_MAYA_MAP_GATE );
			if( bMoveGateSuccess == TRUE )
			{
				// Kanturu Battle User Add
				g_KanturuBattleUserMng.AddUserData( iIndex );

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
				LogAddTD( "[ KANTURU ][ Battle User ] Add User [%s][%s] Current Battle User:%d State:[%d-%d] (CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%u, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name,
					g_KanturuBattleUserMng.GetUserCount(), g_Kanturu.GetKanturuState(), g_Kanturu.GetKanturuDetailState(),
					gObj[iIndex].Class,
					gObj[iIndex].Level,
					gObj[iIndex].LevelUpPoint,
					gObj[iIndex].Experience,
					gObj[iIndex].Strength,
					gObj[iIndex].Dexterity,
					gObj[iIndex].Vitality,
					gObj[iIndex].Energy,
					gObj[iIndex].Leadership,
					gObj[iIndex].MapNumber,
					(int)gObj[iIndex].m_PK_Level
					);
#else
	#ifdef EXTEND_LOG_SYSTEM_04_20060823
				LogAddTD( "[ KANTURU ][ Battle User ] State:[%d-%d] Add User [%s][%s] Current Battle User:%d",
					g_Kanturu.GetKanturuState(), g_Kanturu.GetKanturuDetailState(), gObj[iIndex].AccountID, gObj[iIndex].Name, g_KanturuBattleUserMng.GetUserCount() );
	#endif
#endif // MODIFY_LOG_SYSTEM_2_3_20060828

				KANTURU_UTIL.NotifyKanturuCurrentState( iIndex, g_Kanturu.GetKanturuState(), g_Kanturu.GetKanturuDetailState() );
#ifdef ADD_NEW_NPC_KANTURU_20060612
				gObj[iIndex].m_bKanturuEntranceByNPC	= TRUE;		// 칸투르 보스전 맵에 입장했음을 저장한다.
#endif
			}
		}
		else if( iState == KANTURU_STATE_TOWER_OF_REFINEMENT )
		{
			bMoveGateSuccess = gObjMoveGate( iIndex, KANTURU_TOWER_OPEN_MAP_GATE );
			KANTURU_UTIL.NotifyKanturuCurrentState( iIndex, g_Kanturu.GetKanturuState(), g_Kanturu.GetKanturuDetailState() );

#ifdef ADD_NEW_NPC_KANTURU_20060612
			gObj[iIndex].m_bKanturuEntranceByNPC	= TRUE;		// 칸투르 보스전 맵에 입장했음을 저장한다.
#endif
		}	
		else	// 에러 상황
		{
			LogAddTD( "[ KANTURU ][ BossMapMove ] Error - State(%d) Character[%s][%s]",
					iState, lpObj->AccountID, lpObj->Name );
		}
	}
	else	// 입장 불가
	{
		KANTURU_UTIL.NotifyKanturuEntranceReqResult( iIndex, iResult );	
		
		LogAddTD( "[ KANTURU ][ BossMapMove ] Fail - Result(%d) Character[%s][%s]",
			iResult, lpObj->AccountID, lpObj->Name );

#ifdef ADD_NEW_NPC_KANTURU_20060612
		gObj[iIndex].m_bKanturuEntranceByNPC	= FALSE;
#endif
	}	
}

#endif