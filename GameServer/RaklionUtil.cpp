//----------------------------------------------------------------------------------
// FileName				: RaklionUtil.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 통신 관련 유틸리티 클래스
//----------------------------------------------------------------------------------
// RaklionUtil.cpp: implementation of the CRaklionUtil class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_RAKLION_20080408

#include "RaklionUtil.h"

#include "RaklionDefine.h"
#include "RaklionBattleUserMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionUtil::CRaklionUtil()
{

}

CRaklionUtil::~CRaklionUtil()
{

}

//----------------------------------------------------------------------------------------
// 라클리온의 상태가 변경될 때마다 클라이언트에 통보한다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::NotifyRaklionChangeState( int iState, int iDetailState )
{
	PMSG_ANS_RAKLION_STATE_CHANGE	pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x12, sizeof( pMsg ) );
	
	pMsg.btState			= ( BYTE )iState;				// 변경된 라클리온 메인 스케줄
	pMsg.btDetailState		= ( BYTE )iDetailState;			// 변경된 라클리온 세부 스케줄	
	
	if( iState == RAKLION_STATE_NOTIFY_1 )
		SendDataRaklionMapUser( (LPBYTE)&pMsg, sizeof( pMsg ) );
	else
		SendDataRaklionBossMapUser( ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// 라클리온 보스전의 성공/실패 정보를 클라이언트에 알린다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::NotifyRaklionSuccessValue( int iSuccessValue )
{
	PMSG_ANS_RAKLION_BATTLE_RESULT	pMsg = {0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x13, sizeof( pMsg ) );
	pMsg.btResult	= iSuccessValue;
	
	SendDataRaklionBossMapUser( ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// 라클리온 보스맵에 입장하면 현재 보스맵의 상태를 클라이언트에 알린다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::NotifyRaklionCurrentState( int iIndex, int iState, int iDetailState )
{
	PMSG_ANS_RAKLION_CURRENT_STATE	pMsg = {0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x11, sizeof( pMsg ) );
	pMsg.btCurrentState			= iState;
	pMsg.btCurrentDetailState	= iDetailState;
	
	DataSend( iIndex, ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// 라클리온 전투중 전체공격 스킬에 맞았음을 해당 클라이언트에 알린다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::NotifyRaklionWideAreaAttack( int iIndex, int iTargetIndex, int iSkillType )
{
	PMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK pMsg;
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x14, sizeof( pMsg ) );
	pMsg.btObjClassH	= HIBYTE( gObj[iIndex].Class );
	pMsg.btObjClassL	= LOBYTE( gObj[iIndex].Class );
	pMsg.btType			= iSkillType;		
	
	SendDataToUser( iTargetIndex, ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// 라클리온 전투중 남은 몬스터/유저수를 클라이언트에 알린다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::NotifyRaklionUserMonsterCount( int iMonsterCount, int iUserCount )
{
	PMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT pMsg;
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x15, sizeof( pMsg ) );
	pMsg.btMonsterCount	= iMonsterCount;
	pMsg.btUserCount	= iUserCount;
	
	SendDataRaklionBossMapUser( ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// 라클리온 전투맵에 입장한 [전투중인] 유저에게 특정한 메세지를 [왼쪽상단공지]로 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::NotifyLeftTopMsgRaklionBattleUser( LPSTR lpszMsg, ... )
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
	
	for( int iCount = 0; iCount < g_RaklionBattleUserMng.m_BattleUser.size(); iCount++ )
	{
		if( ( gObj[g_RaklionBattleUserMng.m_BattleUser[iCount]].Connected == CSS_GAMEPLAYING )
			&& ( gObj[g_RaklionBattleUserMng.m_BattleUser[iCount]].Type == OBJTYPE_CHARACTER ) )
		{
			TNotice::SendNoticeToUser( g_RaklionBattleUserMng.m_BattleUser[iCount], &pNotice );
		}
	}
}

//----------------------------------------------------------------------------------------
// 라클리온 보스존에 입장한 유저에게 특정한 메세지를 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendMsgRaklionBossMapUser( LPSTR lpszMsg, ... )
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
			&& ( gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS ) 
			)
		{
			TNotice::SendNoticeToUser( iCount, &pNotice );
		}	
	}
}

//----------------------------------------------------------------------------------------
// 라클리온 보스존에 입장한 유저에게 특정한 데이터를 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendDataRaklionBossMapUser( LPBYTE lpMsg, int iSize )
{
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		if( ( gObj[iCount].Connected == CSS_GAMEPLAYING ) 
			&& ( gObj[iCount].Type == OBJTYPE_CHARACTER ) 
			&& ( gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS ) 
			)
		{
			DataSend( iCount, ( LPBYTE )lpMsg, iSize );
		}
	}
}

//----------------------------------------------------------------------------------------
// 라클리온 필드, 보스맵에 입장한 모든 유저에게 특정한 메세지를 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendMsgRaklionMapUser( LPSTR lpszMsg, ... )
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
			if( gObj[iCount].MapNumber == MAP_INDEX_RAKLION_FIELD
				|| gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS )
			{
				TNotice::SendNoticeToUser( iCount, &pNotice );
			}
		}	
	}
}

//----------------------------------------------------------------------------------------
// 라클리온 필드, 보스존에 입장한 모든 유저에게 특정한 데이터를 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendDataRaklionMapUser( LPBYTE lpMsg, int iSize )
{
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		if( gObj[iCount].Connected == CSS_GAMEPLAYING && gObj[iCount].Type == OBJTYPE_CHARACTER ) 
		{
			if( gObj[iCount].MapNumber == MAP_INDEX_RAKLION_FIELD
				|| gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS )
			{
				DataSend( iCount, ( LPBYTE )lpMsg, iSize );
			}
		}
	}
}

//----------------------------------------------------------------------------------------
// 전체 유저에게 특정한 메세지를 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendMsgAllUser( LPSTR lpszMsg, ... )
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
// 전체 유저에게 특정한 데이터를 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendDataAllUser( LPBYTE lpMsg, int iSize )
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
// 특정 유저에게 특정한 메세지를 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendMsgToUser( int iIndex, LPSTR lpszMsg, ... )
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
// 특정 유저에게 특정한 데이터를 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendDataToUser( int iIndex, LPBYTE lpMsg, int iSize )
{
	if( ( gObj[iIndex].Connected == CSS_GAMEPLAYING ) && ( gObj[iIndex].Type == OBJTYPE_CHARACTER ) )
	{
		DataSend( iIndex, ( LPBYTE )lpMsg, iSize );			
	}
}

//----------------------------------------------------------------------------------------
// 특정 유저에게 시스템 메시지를 보낸다.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendSystemMsgToUser( INT iIndex, LPSTR lpszMsg, ... )
{
	if( lpszMsg == NULL )
		return;
	
	char		szBuffer[512] = "";
	va_list		pArguments;
	
	va_start( pArguments, lpszMsg );
    vsprintf( szBuffer, lpszMsg, pArguments );
    va_end( pArguments );
	
	TNotice::GCServerMsgStringSend( lpszMsg, iIndex, 1 );
}

#endif