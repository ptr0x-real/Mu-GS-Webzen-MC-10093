// QuestUtil.cpp: implementation of the CQuestUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestUtil.h"

#ifdef MODIFY_QUEST_SYSTEM_20070525

#include "Common\winutil.h"

#include "wsJoinServerCli.h"
#include "DBSockMng.h"

CQuestUtil	g_QuestUtil;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CQuestUtil::CQuestUtil()
{

}

CQuestUtil::~CQuestUtil()
{

}

//////////////////////////////////////////////////////////////////////////
// [0x13] 인증서버로 퀘스트 - 몬스터 킬 카운트 정보를 요청한다.
//////////////////////////////////////////////////////////////////////////
void CQuestUtil::GDReqQuestMonsterKillInfo( LPOBJECTSTRUCT lpObj )
{
	// 이미 정보를 받았다면 패스.
	if( lpObj->m_bQuestMonKillDBLoad == TRUE )
		return;

	SDHP_REQ_QUEST_MONSTER_KILL	pMsg;
	pMsg.h.set( ( LPBYTE )&pMsg, 0x13, sizeof( pMsg ) );	
	pMsg.iUserIndex = lpObj->m_Index;
	memcpy( pMsg.szCharName, lpObj->Name, MAX_IDSTRING );
	pMsg.szCharName[MAX_IDSTRING] = 0;
	
	cDBSMng.Send( ( char* )&pMsg, pMsg.h.size );	
}

//////////////////////////////////////////////////////////////////////////
// [0x14] 
//////////////////////////////////////////////////////////////////////////
void CQuestUtil::DGGetQuestMonKillInfo( LPBYTE aRecv )
{
	LPSDHP_QUEST_MONSTER_KILL_INFO lpRecvMsg = ( LPSDHP_QUEST_MONSTER_KILL_INFO )aRecv;

	if( gObjIsConnectedGP( lpRecvMsg->iUserIndex ) == FALSE )
		return;		// 게임 실행중이 아닐때에는 패스.

	int iIndex = lpRecvMsg->iUserIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	
	if( lpObj->m_bQuestMonKillDBLoad == FALSE )
	{
		lpObj->m_iQuestMonKillDBIndex				= lpRecvMsg->btQuestDBIndex;
		lpObj->m_QuestMonKillInfo[0].iQuestMonIndex	= lpRecvMsg->iQuestMonIndex1;
		lpObj->m_QuestMonKillInfo[0].iKillCount		= lpRecvMsg->iKillCount1;		
		lpObj->m_QuestMonKillInfo[1].iQuestMonIndex	= lpRecvMsg->iQuestMonIndex2;
		lpObj->m_QuestMonKillInfo[1].iKillCount		= lpRecvMsg->iKillCount2;		
		lpObj->m_QuestMonKillInfo[2].iQuestMonIndex	= lpRecvMsg->iQuestMonIndex3;
		lpObj->m_QuestMonKillInfo[2].iKillCount		= lpRecvMsg->iKillCount3;		
		lpObj->m_QuestMonKillInfo[3].iQuestMonIndex	= lpRecvMsg->iQuestMonIndex4;
		lpObj->m_QuestMonKillInfo[3].iKillCount		= lpRecvMsg->iKillCount4;		
		lpObj->m_QuestMonKillInfo[4].iQuestMonIndex	= lpRecvMsg->iQuestMonIndex5;
		lpObj->m_QuestMonKillInfo[4].iKillCount		= lpRecvMsg->iKillCount5;

		lpObj->m_bQuestMonKillDBLoad = TRUE;
		
		LogAddTD( "[Quest] Set MonsterKillInfo [%s][%s](%d) (%d : %d/%d, %d/%d, %d/%d, %d/%d, %d/%d)", 
				lpObj->AccountID,
				lpObj->Name,
				lpObj->m_Index,
				lpObj->m_iQuestMonKillDBIndex,
				lpObj->m_QuestMonKillInfo[0].iQuestMonIndex,
				lpObj->m_QuestMonKillInfo[0].iKillCount,
				lpObj->m_QuestMonKillInfo[1].iQuestMonIndex,
				lpObj->m_QuestMonKillInfo[1].iKillCount,
				lpObj->m_QuestMonKillInfo[2].iQuestMonIndex,
				lpObj->m_QuestMonKillInfo[2].iKillCount,
				lpObj->m_QuestMonKillInfo[3].iQuestMonIndex,
				lpObj->m_QuestMonKillInfo[3].iKillCount,
				lpObj->m_QuestMonKillInfo[4].iQuestMonIndex,
				lpObj->m_QuestMonKillInfo[4].iKillCount
				);
	}
}

//////////////////////////////////////////////////////////////////////////
// [0x14] 
//////////////////////////////////////////////////////////////////////////
void CQuestUtil::GDReqQuestMonKillInfoSave( LPOBJECTSTRUCT lpObj )
{
	// 아직 정보도 안 받은 상태라면 패스.
	if( lpObj->m_bQuestMonKillDBLoad == FALSE )
		return;

	SDHP_QUEST_MONSTER_KILL_INFO pMsg;
	PHeadSetB( ( LPBYTE )&pMsg, 0x14, sizeof( pMsg ) );
	memcpy( pMsg.szCharName, lpObj->Name, MAX_IDSTRING );
	pMsg.iUserIndex			= lpObj->m_Index;
	pMsg.btQuestDBIndex		= lpObj->m_iQuestMonKillDBIndex;
	pMsg.iQuestMonIndex1	= lpObj->m_QuestMonKillInfo[0].iQuestMonIndex;
	pMsg.iKillCount1		= lpObj->m_QuestMonKillInfo[0].iKillCount;
	pMsg.iQuestMonIndex2	= lpObj->m_QuestMonKillInfo[1].iQuestMonIndex;
	pMsg.iKillCount2		= lpObj->m_QuestMonKillInfo[1].iKillCount;
	pMsg.iQuestMonIndex3	= lpObj->m_QuestMonKillInfo[2].iQuestMonIndex;
	pMsg.iKillCount3		= lpObj->m_QuestMonKillInfo[2].iKillCount;
	pMsg.iQuestMonIndex4	= lpObj->m_QuestMonKillInfo[3].iQuestMonIndex;
	pMsg.iKillCount4		= lpObj->m_QuestMonKillInfo[3].iKillCount;
	pMsg.iQuestMonIndex5	= lpObj->m_QuestMonKillInfo[4].iQuestMonIndex;
	pMsg.iKillCount5		= lpObj->m_QuestMonKillInfo[4].iKillCount;

	LogAddTD( "[Quest] Save MonsterKillInfo [%s][%s] (%d : %d/%d, %d/%d, %d/%d, %d/%d, %d/%d)", 
			lpObj->AccountID,
			lpObj->Name,			
			pMsg.btQuestDBIndex,
			pMsg.iQuestMonIndex1,
			pMsg.iKillCount1,
			pMsg.iQuestMonIndex2,
			pMsg.iKillCount2,
			pMsg.iQuestMonIndex3,
			pMsg.iKillCount3,
			pMsg.iQuestMonIndex4,
			pMsg.iKillCount4,
			pMsg.iQuestMonIndex5,
			pMsg.iKillCount5
			);

	cDBSMng.Send( ( char* )&pMsg, sizeof( SDHP_QUEST_MONSTER_KILL_INFO ) );
	
}
#endif