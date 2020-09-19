//----------------------------------------------------------------------------------
// FileName				: RaklionUtil.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ��� ���� ��ƿ��Ƽ Ŭ����
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
// ��Ŭ������ ���°� ����� ������ Ŭ���̾�Ʈ�� �뺸�Ѵ�.
//----------------------------------------------------------------------------------------
void CRaklionUtil::NotifyRaklionChangeState( int iState, int iDetailState )
{
	PMSG_ANS_RAKLION_STATE_CHANGE	pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x12, sizeof( pMsg ) );
	
	pMsg.btState			= ( BYTE )iState;				// ����� ��Ŭ���� ���� ������
	pMsg.btDetailState		= ( BYTE )iDetailState;			// ����� ��Ŭ���� ���� ������	
	
	if( iState == RAKLION_STATE_NOTIFY_1 )
		SendDataRaklionMapUser( (LPBYTE)&pMsg, sizeof( pMsg ) );
	else
		SendDataRaklionBossMapUser( ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// ��Ŭ���� �������� ����/���� ������ Ŭ���̾�Ʈ�� �˸���.
//----------------------------------------------------------------------------------------
void CRaklionUtil::NotifyRaklionSuccessValue( int iSuccessValue )
{
	PMSG_ANS_RAKLION_BATTLE_RESULT	pMsg = {0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD1, 0x13, sizeof( pMsg ) );
	pMsg.btResult	= iSuccessValue;
	
	SendDataRaklionBossMapUser( ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//----------------------------------------------------------------------------------------
// ��Ŭ���� �����ʿ� �����ϸ� ���� �������� ���¸� Ŭ���̾�Ʈ�� �˸���.
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
// ��Ŭ���� ������ ��ü���� ��ų�� �¾����� �ش� Ŭ���̾�Ʈ�� �˸���.
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
// ��Ŭ���� ������ ���� ����/�������� Ŭ���̾�Ʈ�� �˸���.
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
// ��Ŭ���� �����ʿ� ������ [��������] �������� Ư���� �޼����� [���ʻ�ܰ���]�� ������.
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
// ��Ŭ���� �������� ������ �������� Ư���� �޼����� ������.
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
// ��Ŭ���� �������� ������ �������� Ư���� �����͸� ������.
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
// ��Ŭ���� �ʵ�, �����ʿ� ������ ��� �������� Ư���� �޼����� ������.
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
// ��Ŭ���� �ʵ�, �������� ������ ��� �������� Ư���� �����͸� ������.
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
// ��ü �������� Ư���� �޼����� ������.
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
// ��ü �������� Ư���� �����͸� ������.
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
// Ư�� �������� Ư���� �޼����� ������.
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
// Ư�� �������� Ư���� �����͸� ������.
//----------------------------------------------------------------------------------------
void CRaklionUtil::SendDataToUser( int iIndex, LPBYTE lpMsg, int iSize )
{
	if( ( gObj[iIndex].Connected == CSS_GAMEPLAYING ) && ( gObj[iIndex].Type == OBJTYPE_CHARACTER ) )
	{
		DataSend( iIndex, ( LPBYTE )lpMsg, iSize );			
	}
}

//----------------------------------------------------------------------------------------
// Ư�� �������� �ý��� �޽����� ������.
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