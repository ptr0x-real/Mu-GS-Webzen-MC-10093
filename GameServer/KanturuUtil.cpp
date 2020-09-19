// KanturuUtil.cpp: implementation of the CKanturuUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuUtil.h"

#ifdef KANTURU_TIMEATTACK_EVENT_20060822
#include "Kanturu.h"
#else
#include "KanturuDefine.h"
#endif
#include "KanturuBattleUserMng.h"

#include "User.h"
#include "Protocol.h"

CKanturuUtil	g_KanturuUtil;
extern void GCServerMsgSend(BYTE msg, int aIndex);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuUtil::CKanturuUtil()
{

}

CKanturuUtil::~CKanturuUtil()
{

}

//----------------------------------------------------------------------------------------
// ĭ������ ���°� ����� ������ Ŭ���̾�Ʈ�� �뺸�Ѵ�.
//----------------------------------------------------------------------------------------
void CKanturuUtil::NotifyKanturuChangeState( int iState, int iDetailState )
{
	PMSG_ANS_KANTURU_STATE_CHANGE	pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x03, sizeof( pMsg ) );
	
	pMsg.btState			= ( BYTE )iState;				// ����� ĭ���� ���� ������
	pMsg.btDetailState		= ( BYTE )iDetailState;			// ����� ĭ���� ���� ������	
	
	SendDataKanturuBossMapUser( ( LPBYTE )&pMsg, sizeof( pMsg ) );

//#if ( TESTSERVER == 1 )
//	SendMsgKanturuBattleUser( "CURRENT STATE : %d DETAILSTATE: %d", iState, iDetailState );
//#endif
}

//----------------------------------------------------------------------------------------
// ĭ������ ���� ��û�� ���� ������ Ŭ���̾�Ʈ�� �˸���.
//----------------------------------------------------------------------------------------
void CKanturuUtil::NotifyKanturuEntranceReqResult( int iIndex, int iResult )
{
	PMSG_ANS_ENTER_KANTURU_BOSS_MAP		pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x01, sizeof( pMsg ) );
	pMsg.btResult = iResult;	
	
	DataSend( iIndex, ( LPBYTE )&pMsg, sizeof( pMsg ) );	
}

//----------------------------------------------------------------------------------------
// ĭ���� �� ������ ����/���� ������ Ŭ���̾�Ʈ�� �˸���.
//----------------------------------------------------------------------------------------
void CKanturuUtil::NotifyKanturuSuccessValue( int iSuccessValue )
{
	PMSG_ANS_KANTURU_BATTLE_RESULT	pMsg = {0. };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x04, sizeof( pMsg ) );
	pMsg.btResult	= iSuccessValue;

	SendDataKanturuBattleUser( ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// ĭ���� �����ʿ� �����ϸ� ���� �������� ���¸� Ŭ���̾�Ʈ�� �˸���.
//----------------------------------------------------------------------------------------
void CKanturuUtil::NotifyKanturuCurrentState( int iIndex, int iState, int iDetailState )
{
	PMSG_ANS_KANTURU_CURRENT_STATE	pMsg = {0. };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x02, sizeof( pMsg ) );
	pMsg.btCurrentState			= iState;
	pMsg.btCurrentDetailState	= iDetailState;
	
	DataSend( iIndex, ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// ĭ���� ������ �����ð��� �����ʿ� ������ Ŭ���̾�Ʈ�� �˸���.
//----------------------------------------------------------------------------------------
void CKanturuUtil::NotifyKanturuBattleTime( int iBattleTime )
{
	PMSG_ANS_KANTURU_BATTLE_SCENE_TIMELIMIT	pMsg;
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x05, sizeof( pMsg ) );	
	pMsg.btTimeLimit	= iBattleTime;
	
	SendDataKanturuBattleUser( ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// ĭ���� ������ ��ü���� ��ų�� �¾����� �ش� Ŭ���̾�Ʈ�� �˸���.
//----------------------------------------------------------------------------------------
void CKanturuUtil::NotifyKanturuWideAreaAttack( int iIndex, int iTargetIndex, int iSkillType )
{
	PMSG_NOTIFY_KANTURU_WIDE_AREA_ATTACK pMsg;
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x06, sizeof( pMsg ) );
	pMsg.btObjClassH	= HIBYTE( gObj[iIndex].Class );
	pMsg.btObjClassL	= LOBYTE( gObj[iIndex].Class );
	pMsg.btType			= iSkillType;		
	
	SendDataToUser( iTargetIndex, ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// ĭ���� ������ ���� ����/�������� Ŭ���̾�Ʈ�� �˸���.
//----------------------------------------------------------------------------------------
void CKanturuUtil::NotifyKanturuUserMonsterCount( int iMonsterCount, int iUserCount )
{
	PMSG_NOTIFY_KANTURU_USER_MONSTER_COUNT pMsg;
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x07, sizeof( pMsg ) );
	pMsg.btMonsterCount	= iMonsterCount;
	pMsg.btUserCount	= iUserCount;

	SendDataKanturuBattleUser( ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

#ifdef KANTURU_TEST_200607119
//----------------------------------------------------------------------------------------
// ĭ���� �����ʿ� ������ [��������] �������� Ư���� �޼����� [���ʻ�ܰ���]�� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::NotifyLeftTopMsgKanturuBattleUser( LPSTR lpszMsg, ... )
{
	if( lpszMsg == NULL )
		return;

	char		szBuffer[512]="";
	va_list		pArguments;

	va_start( pArguments, lpszMsg );
    vsprintf( szBuffer, lpszMsg, pArguments );
    va_end( pArguments );
	
	PMSG_NOTICE	pNotice;
	TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_LEFTTOP, szBuffer );

	int iUserIndex = 0;
	for( int iCount = 0; iCount < MAX_KANTURU_BATTLE_USER; iCount++ )
	{
#ifdef MEM_KUZIMO
		if(g_KanturuBattleUserMng.m_BattleUser[iCount].IsUseData() == FALSE) continue;
#endif
		iUserIndex = g_KanturuBattleUserMng.m_BattleUser[iCount].GetIndex();
		if( ( gObj[iUserIndex].Connected == CSS_GAMEPLAYING ) && ( gObj[iUserIndex].Type == OBJTYPE_CHARACTER ) )
		{
			TNotice::SendNoticeToUser( iUserIndex, &pNotice );
		}
	}	
}
#endif



//----------------------------------------------------------------------------------------
// ĭ���� �����ʿ� ������ �������� �������� Ư���� �޼����� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendMsgKanturuBattleUser( LPSTR lpszMsg, ... )
{
	if( lpszMsg == NULL )
		return;

	char		szBuffer[512] = "";
	va_list		pArguments;
	
	va_start( pArguments, lpszMsg );
    vsprintf( szBuffer, lpszMsg, pArguments );
    va_end( pArguments );

	PMSG_NOTICE	pNotice;
	TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, szBuffer );

	int iUserIndex = 0;
	for( int iCount = 0; iCount < MAX_KANTURU_BATTLE_USER; iCount++ )
	{
#ifdef MEM_KUZIMO
		if(g_KanturuBattleUserMng.m_BattleUser[iCount].IsUseData() == FALSE) continue;
#endif
		iUserIndex = g_KanturuBattleUserMng.m_BattleUser[iCount].GetIndex();
		if( ( gObj[iUserIndex].Connected == CSS_GAMEPLAYING ) && ( gObj[iUserIndex].Type == OBJTYPE_CHARACTER ) )
		{
			TNotice::SendNoticeToUser( iUserIndex, &pNotice );
		}
	}
}

//----------------------------------------------------------------------------------------
// ĭ���� �����ʿ� ������ �������� �������� Ư���� �����͸� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendDataKanturuBattleUser( LPBYTE lpMsg, int iSize )
{
	int iUserIndex = 0;
	for( int iCount = 0; iCount < MAX_KANTURU_BATTLE_USER; iCount++ )
	{
#ifdef MEM_KUZIMO
		if(g_KanturuBattleUserMng.m_BattleUser[iCount].IsUseData() == FALSE) continue;
#endif
		iUserIndex = g_KanturuBattleUserMng.m_BattleUser[iCount].GetIndex();
		if( ( gObj[iUserIndex].Connected == CSS_GAMEPLAYING ) && ( gObj[iUserIndex].Type == OBJTYPE_CHARACTER ) )
		{
			DataSend( iUserIndex, ( LPBYTE )lpMsg, iSize );
		}
	}	
}

//----------------------------------------------------------------------------------------
// ĭ���� �����ʿ� ������ ��� �������� Ư���� �޼����� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendMsgKauturuBossMapUser( LPSTR lpszMsg, ... )
{
	if( lpszMsg == NULL )
		return;

	char		szBuffer[512] = "";
	va_list		pArguments;
	
	va_start( pArguments, lpszMsg );
    vsprintf( szBuffer, lpszMsg, pArguments );
    va_end( pArguments );

	PMSG_NOTICE	pNotice;

	TNotice::MakeNoticeMsg( &pNotice, 0, szBuffer );
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{		
		if( ( gObj[iCount].Connected == CSS_GAMEPLAYING ) 
			&& ( gObj[iCount].Type == OBJTYPE_CHARACTER )
			&& ( gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS ) 
			)
		{
			TNotice::SendNoticeToUser( iCount, &pNotice );
		}	
	}
}

//----------------------------------------------------------------------------------------
// ĭ���� �����ʿ� ������ ��� �������� Ư���� �����͸� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendDataKanturuBossMapUser( LPBYTE lpMsg, int iSize )
{
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		if( ( gObj[iCount].Connected == CSS_GAMEPLAYING ) 
			&& ( gObj[iCount].Type == OBJTYPE_CHARACTER ) 
			&& ( gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS ) 
			)
		{
			DataSend( iCount, ( LPBYTE )lpMsg, iSize );
		}
	}
}

//----------------------------------------------------------------------------------------
// ĭ���� 1, 2�� ��, �����ʿ� ������ ��� �������� Ư���� �޼����� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendMsgKauturuMapUser( LPSTR lpszMsg, ... )
{
	if( lpszMsg == NULL )
		return;

	char		szBuffer[512] = "";
	va_list		pArguments;
	
	va_start( pArguments, lpszMsg );
    vsprintf( szBuffer, lpszMsg, pArguments );
    va_end( pArguments );

	PMSG_NOTICE	pNotice;

	TNotice::MakeNoticeMsg( &pNotice, 0, szBuffer );
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{		
		if( gObj[iCount].Connected == CSS_GAMEPLAYING && gObj[iCount].Type == OBJTYPE_CHARACTER )
		{
			if( gObj[iCount].MapNumber == MAP_INDEX_KANTURU1
				|| gObj[iCount].MapNumber == MAP_INDEX_KANTURU2
				|| gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS )
			{
				TNotice::SendNoticeToUser( iCount, &pNotice );
			}
		}	
	}
}

//----------------------------------------------------------------------------------------
// ĭ���� 1, 2�� ��, �����ʿ� ������ ��� �������� Ư���� �����͸� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendDataKanturuMapUser( LPBYTE lpMsg, int iSize )
{
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		if( gObj[iCount].Connected == CSS_GAMEPLAYING && gObj[iCount].Type == OBJTYPE_CHARACTER ) 
		{
			if( gObj[iCount].MapNumber == MAP_INDEX_KANTURU1
				|| gObj[iCount].MapNumber == MAP_INDEX_KANTURU2
				|| gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS )
			{
				DataSend( iCount, ( LPBYTE )lpMsg, iSize );
			}
		}
	}
}

//----------------------------------------------------------------------------------------
// ��ü �������� Ư���� �޼����� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendMsgAllUser( LPSTR lpszMsg, ... )
{
	if( lpszMsg == NULL )
		return;

	char		szBuffer[512] = "";
	va_list		pArguments;
	
	va_start( pArguments, lpszMsg );
    vsprintf( szBuffer, lpszMsg, pArguments );
    va_end( pArguments );

	PMSG_NOTICE	pNotice;

	TNotice::MakeNoticeMsg( &pNotice, 0, szBuffer );
	TNotice::SendNoticeToAllUser( &pNotice );
}

//----------------------------------------------------------------------------------------
// ��ü �������� Ư���� �����͸� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendDataAllUser( LPBYTE lpMsg, int iSize )
{
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		if( ( gObj[iCount].Connected == CSS_GAMEPLAYING ) && ( gObj[iCount].Type == OBJTYPE_CHARACTER ) )
		{
			DataSend( iCount, ( LPBYTE )lpMsg, iSize );			
		}
	}	
}

//----------------------------------------------------------------------------------------
// Ư�� �������� Ư���� �޼����� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendMsgToUser( int iIndex, LPSTR lpszMsg, ... )
{
	if( lpszMsg == NULL )
		return;

	char		szBuffer[512] = "";
	va_list		pArguments;
	
	va_start( pArguments, lpszMsg );
    vsprintf( szBuffer, lpszMsg, pArguments );
    va_end( pArguments );

	PMSG_NOTICE	pNotice;

	TNotice::MakeNoticeMsg( &pNotice, 0, szBuffer );
	TNotice::SendNoticeToUser( iIndex, &pNotice );
}

//----------------------------------------------------------------------------------------
// Ư�� �������� Ư���� �����͸� ������.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendDataToUser( int iIndex, LPBYTE lpMsg, int iSize )
{
	if( ( gObj[iIndex].Connected == CSS_GAMEPLAYING ) && ( gObj[iIndex].Type == OBJTYPE_CHARACTER ) )
	{
		DataSend( iIndex, ( LPBYTE )lpMsg, iSize );			
	}
}

//----------------------------------------------------------------------------------------
// ������ ä�ø޼��� - AI �׽�Ʈ��.
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendKanturuChattingMsg( int iIndex, LPSTR lpszMsg, ... )
{
#if !TESTSERVER	// �׽�Ʈ�� ���������� ����Ѵ�.
	return;
#endif

	if( lpszMsg == NULL )
		return;
	
	if( iIndex < 0 || iIndex > MAX_OBJECT )
		return;

	LPOBJECTSTRUCT	lpObj = &gObj[iIndex];

	char		szBuffer[512]="";
	va_list		pArguments;
	
	va_start( pArguments, lpszMsg );
    vsprintf( szBuffer, lpszMsg, pArguments );
    va_end( pArguments );
	
	char szChat[MAX_CHAT] = { 0, };

	CopyMemory( szChat, szBuffer, MAX_CHAT - 1 );

	for( int iCount = 0; iCount < MAX_MONVIEWPORTOBJECT; iCount++ )
	{
		if( lpObj->VpPlayer2[iCount].state )
		{
			int iTargetNumber = lpObj->VpPlayer2[iCount].number;
			if( iTargetNumber >= 0 || iTargetNumber < MAX_OBJECT )
			{
				ChatTargetSend( lpObj, szChat, iTargetNumber );
			}
		}
	}
}

#ifdef KANTURU_TIMEATTACK_EVENT_20060822
#include "..\INCLUDE\cb_protocol.h"
#include "Event.h"
#include "gamemain.h"

#include "Kanturu.h"
//----------------------------------------------------------------------------------------
// ĭ���� ������ Ÿ�Ӿ��� �̺�Ʈ - EventServer�� ������ ����
//----------------------------------------------------------------------------------------
void CKanturuUtil::SendDataKanturuTimeAttackEvent( int iIndex, BYTE btFlag, int iClearTime )
{
	PMSG_REQ_LOG_KANTURU_TIMEATTACK_EVENT pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0x22, 0x00, sizeof( pMsg ) );
	
	pMsg.nINDEX = iIndex;
	memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
	pMsg.szUID[sizeof( pMsg.szUID )] = 0;
	memcpy( pMsg.szNAME, gObj[iIndex].Name, MAX_IDSTRING );
	pMsg.szNAME[sizeof( pMsg.szNAME )] = 0;
	
	pMsg.wServerCode	= gGameServerCode / 20;

	char szKanturuBattleDate[14];
	memset( szKanturuBattleDate, 0, sizeof( szKanturuBattleDate ) );	
	wsprintf( szKanturuBattleDate, "%d%03d%02d", g_Kanturu.GetKanturuBattleDate(), gGameServerCode, g_Kanturu.GetKanturuBattleCounter() );
	memcpy( pMsg.szBattleID, szKanturuBattleDate, sizeof( pMsg.szBattleID ) );
	
	pMsg.btStageNumber		= btFlag;
	pMsg.wClearTime			= iClearTime;
	pMsg.iLevel				= gObj[iIndex].Level;
	pMsg.iExp				= gObj[iIndex].Experience;

	LogAddTD( "[ KANTURU ][ TimeAttackEvent ] [%s][%s] %d/BattleID:%s, StageNum:%d, ClearTime:%d, Level:%d(%d)",
		pMsg.szUID,
		pMsg.szNAME,
		pMsg.wServerCode,
		pMsg.szBattleID,
		pMsg.btStageNumber,
		pMsg.wClearTime,
		pMsg.iLevel,
		pMsg.iExp
		);
	
	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );

}
#endif	// KANTURU_TIMEATTACK_EVENT_20060822

#endif