// IllusionTempleProcess.cpp: implementation of the CIllusionTempleProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IllusionTempleProcess.h"

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#include "DSProtocol.h"
#include "..\\Common\\Winutil.h"
#include "IllusionTempleEvent.h"
#include "protocol.h"
#include "GameMain.h"
#include "MonsterSetBase.h"
#include "MagicDamage.h"
#include "ProbabilityItemBag.h"
#include "..\\Include\\cb_protocol.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "..\\Common\\Classdef.h"
#include "..\\include\\define.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "Event.h"

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

extern PartyClass		gParty;
extern CMonsterSetBase	gMSetBase;
extern classdef			DCInfo;

extern wsJoinServerCli	wsRServerCli;			//  랭킹서버

extern CProbabilityItemBag* IllusionTempleGhostItemBagA;	// 1~2까지의 몬스터 아이템백
extern CProbabilityItemBag* IllusionTempleGhostItemBagB;	// 3~4까지의 몬스터 아이템백
extern CProbabilityItemBag* IllusionTempleGhostItemBagC;	// 5~6까지의 몬스터 아이템백

int CIllusionTempleEvent::m_nIllusionTempleOpenTime;
int CIllusionTempleEvent::m_nIllusionTempleReadyTime;
int CIllusionTempleEvent::m_nIllusionTemplePlayTime;
int CIllusionTempleEvent::m_nIllusionTempleEndTime;
list <ILLUSION_TEMPLE_STARTTIME>	CIllusionTempleEvent::m_lstIllusionTempleStartTime;	

#define ILLUSION_TEMPLE_OPEN_TIME		CIllusionTempleEvent::m_nIllusionTempleOpenTime
#define ILLUSION_TEMPLE_READY_TIME		CIllusionTempleEvent::m_nIllusionTempleReadyTime
#define ILLUSION_TEMPLE_PLAY_TIME		CIllusionTempleEvent::m_nIllusionTemplePlayTime
#define ILLUSION_TEMPLE_END_TIME		CIllusionTempleEvent::m_nIllusionTempleEndTime

#define LST_ILLUSION_TIME_START_TIME	CIllusionTempleEvent::m_lstIllusionTempleStartTime

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIllusionTempleProcess::CIllusionTempleProcess()
: m_nTempleNumber(0xFF), m_btIllusionTempleState(ILLUSION_TEMPLE_STATE_NONE), m_nNextTimeMin(0), m_nStatusCount(0), m_nMonsterCount(0)
{
	InitializeCriticalSection (&m_critUserData);

	m_nRemainTimeMSec	= -1;
	ResetPosNum();
}

CIllusionTempleProcess::~CIllusionTempleProcess()
{
	DeleteCriticalSection(&m_critUserData);
}

void CIllusionTempleProcess::IllusionTempleProcessInit(int nTempleNum)
{	
	if( m_nTempleNumber == 0xFF)	
	{
		// 환영 사원 번호가 정해져 있지 않을 경우
		m_nTempleNumber = nTempleNum;
	}

	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		ClearUserData(i);
	}

	m_nTickCount				= GetTickCount();	
	m_nRelicsUserIndex			= -1;
	m_btInventoryPos			= -1;
	m_btRelicsTeam				= 0xFF;

	m_nAlliedUserCount			= 0;
	m_nIllusionUserCount		= 0;
	m_btAlliedPoint				= 0;
	m_btIllusionPoint			= 0;

	m_StatusRegenTime			= -1;

	m_btWinTeam					= 0xFF;

	m_bEntrance					= FALSE;

	m_nRemainTimeMSec			= -1;

	m_bGameEndPosMove			= FALSE;

	m_bSendMsgBeforeReady		= FALSE;	
	m_bSendMsgBeforePlay		= FALSE;	

	m_nEnventEndRemainTime		= 0;

	ResetSkillPoint();
	CheckSync();

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
	m_bAllCheckSync				= FALSE;
#endif

	for( int i = 0; i < MAX_CLASSTYPE; i++)
	{
		m_btAlliedClassCnt[i]	= 0;
		m_btIllusionClassCnt[i]	= 0;
	}
	
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013
	m_bSendResult				= FALSE;
#endif
}

// 사용자 정보를 초기화
void CIllusionTempleProcess::ClearUserData(int nIndex)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}
	
	// 스킬 정보리셋
	m_dwViewSkillState[nIndex]					= 0x00;
	m_UserData[nIndex].m_dwSkillProdectionTime	= 0;
	m_UserData[nIndex].m_dwSkillRestraintTime	= 0;
	
	// 사용자 정보
	m_UserData[nIndex].m_nIndex					= -1;
	m_UserData[nIndex].m_btTeam					= 0xFF;
	m_UserData[nIndex].m_btUserKillCount		= 0;
	m_UserData[nIndex].m_btMonsterKillCount		= 0;
	m_UserData[nIndex].m_btRelicsMoveCount		= 0;
	m_UserData[nIndex].m_btUseSkillCount		= 0;
	
	m_UserData[nIndex].m_nRewardExp				= 0;
	m_UserData[nIndex].m_nRankingPoint			= 0;
	m_UserData[nIndex].m_bReward				= FALSE;
}

// 오픈 시간 설정
void CIllusionTempleProcess::CheckSync()
{
	list<ILLUSION_TEMPLE_STARTTIME>::iterator iter;
	ILLUSION_TEMPLE_STARTTIME	prevStartTime;
	ILLUSION_TEMPLE_STARTTIME	nextStartTime;

	DWORD	dwPrevTime	= 0;;		// 이전 시작 시간(분)
	DWORD	dwNextTime	= 0;;		// 다음 시작 시간(분)
	DWORD	dwCurTime	= 0;;		// 현재 시간(분)
	
	struct tm *today;
	time_t ltime;

	if(LST_ILLUSION_TIME_START_TIME.size() == 0)
	{
		MsgBox("Error : Illusion Temple StartTime size is 0");
		return;
	}
	
	time( &ltime );
	today = localtime( &ltime );
	dwCurTime	= today->tm_hour * 60 + today->tm_min;

	// 다음 이벤트 열리 시간을 검색
	prevStartTime = *LST_ILLUSION_TIME_START_TIME.begin();
	for(iter = LST_ILLUSION_TIME_START_TIME.begin(); iter != LST_ILLUSION_TIME_START_TIME.end(); ++iter)
	{
		nextStartTime = *iter;

		// 시작시간을 분으로 계산
		dwPrevTime	= prevStartTime.nHour * 60 + prevStartTime.nMin;
		dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;

		if(dwPrevTime == dwNextTime)
		{
			if(dwCurTime < dwNextTime)
			{
				nextStartTime = *iter;
				break;
			}
			else
			{
				continue;
			}
		}
		
		if(dwCurTime >= dwPrevTime && dwCurTime < dwNextTime)	
		{
			break;			
		}

		prevStartTime = *iter;
	}

	int Cnt = 2;
	while(Cnt--)
	{
		if(iter == LST_ILLUSION_TIME_START_TIME.end())
		{
			// 다음 날 시간
			iter = LST_ILLUSION_TIME_START_TIME.begin();
			nextStartTime = *iter;
		}
		
		// 다음 시작시간을 다시 계산
		dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;

		// 다음 시작까지 남은 시간 계산
		if(today->tm_hour <= nextStartTime.nHour)
		{
			// 다음 이벤트가 오늘
			m_nRemainTimeMin = dwNextTime - dwCurTime;
		}
		else
		{
			// 다음 이벤트가 다음날
			m_nRemainTimeMin = 24 * 60 - dwCurTime + dwNextTime;
		}

		// 현재 시작까지 남은 시간(m_nRemainTime)이 오픈시간(m_iOpenTime)보다 작으면 다음 시간으로 넘긴다
		if( m_nRemainTimeMin <= ILLUSION_TEMPLE_OPEN_TIME)
		{
			iter++;
			if(iter != LST_ILLUSION_TIME_START_TIME.end())
			{
				nextStartTime = *iter;
			}
		}
		else
		{
			//m_nRemainTimeMin -= ILLUSION_TEMPLE_OPEN_TIME;
			break;
		}
	}

	// 다음 분(min) 계산
	int tmpTime = GetTickCount();
	//m_nNextTimeMin = GetTickCount() + (today->tm_sec * 1000);
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
	m_nNextTimeMin = tmpTime + (60 * 1000 - today->tm_sec * 1000);
#else
	m_nNextTimeMin = tmpTime + (today->tm_sec * 1000);
#endif

	LogAddTD("[Illusion Temple] (%d) Sync Open Time. [%d]min remain", m_nTempleNumber+1, m_nRemainTimeMin);
}

void CIllusionTempleProcess::ProcIllusionTemple(int nCurTime)
{
	BYTE btState = m_btIllusionTempleState;
	switch(btState)
	{
	case ILLUSION_TEMPLE_STATE_NONE:
		ProcIllusionTempleState_None(nCurTime);
		break;
	case ILLUSION_TEMPLE_STATE_READY:
		ProcIllusionTempleState_Ready(nCurTime);
		break;
	case ILLUSION_TEMPLE_STATE_PLAYING:
		ProcIllusionTempleState_Playing(nCurTime);
		break;
	case ILLUSION_TEMPLE_STATE_END:
		ProcIllusionTempleState_End(nCurTime);
		break;
	}
}

// 이벤트 시간 아님
void CIllusionTempleProcess::ProcIllusionTempleState_None(int nCurTime)
{
	INT iTICK_MSEC = nCurTime - m_nTickCount;

	if(m_nNextTimeMin < nCurTime)
	{
		// 1분마다 들어옴
		m_nRemainTimeMin--;
		m_nNextTimeMin = nCurTime + 60*1000;

		if( m_nRemainTimeMin <= ILLUSION_TEMPLE_OPEN_TIME && m_nRemainTimeMin > 0
			&& m_nTempleNumber == 0) 
		{
			// "환영사원 입장까지 %d 분 남았습니다."
			PMSG_NOTICE	pNotice;
			TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(3394), m_nRemainTimeMin);	// "환영사원 입장까지 %d 분 남았습니다."
			SendAllUserAnyMsg((LPBYTE) &pNotice, pNotice.h.size);
		}
	}

	if (iTICK_MSEC >= 1000) // 1초마다 들어옴
	{
		if(m_nRemainTimeMSec > 0)
		{
			m_nRemainTimeMSec	-= iTICK_MSEC;
		}

		m_nTickCount			= nCurTime;

#if TESTSERVER == 1		// 테스트 서버 일경우는 무조건 입장
		m_bEntrance = TRUE;
#else
		// 남은 시간이 입장시간 1분전이면 초(sec)로 계산
		if(m_nRemainTimeMin <= ILLUSION_TEMPLE_OPEN_TIME+1 && m_nRemainTimeMSec == -1)
		{
			m_nRemainTimeMSec = (ILLUSION_TEMPLE_OPEN_TIME+1)*60*1000;
		}

		// 입장가능 설정
#ifdef MODIFY_ILLUSION_TEMPLE_20070629
		if(m_nRemainTimeMin <= ILLUSION_TEMPLE_OPEN_TIME && m_nRemainTimeMSec > 5*1000 && m_bEntrance == FALSE)
#else
		if(m_nRemainTimeMin <= ILLUSION_TEMPLE_OPEN_TIME && m_bEntrance == FALSE)
#endif
		{
			m_bEntrance = TRUE;
		}

		// 입장 종료 30초전 공지
		if(m_nRemainTimeMSec <= 30*1000
			&& m_nRemainTimeMSec > 0
			&& m_nTempleNumber == 0 && m_bSendMsgBeforeReady == FALSE)
		{
			m_bSendMsgBeforeReady = TRUE;

			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = TEMPLE_MSG_BEFORE_READY;
			
			for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
			{
				if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
				{
					// 카오스캐슬, 블캐에 접속하지 않은 사람들만 골라서 보낸다.
					if ( !CHECK_BLOODCASTLE(gObj[n].MapNumber)
						&& !CHECK_CHAOSCASTLE(gObj[n].MapNumber) )		
						DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
		
		// Ready 상태로 바뀌기 5초전에 입장 불가로 변경
		if(m_nRemainTimeMSec <= 5*1000 && m_bEntrance == TRUE)
		{
			m_bEntrance = FALSE;

			if(m_nTempleNumber == 0)
			{
				// "환영사원 입장이 종료되었습니다."
				PMSG_NOTICE	pNotice;
				TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(3395));	// "환영사원 입장이 종료되었습니다."
				SendAllUserAnyMsg((LPBYTE) &pNotice, pNotice.h.size);
			}
		}

		// 5초부터 카운트 시작
//		if(m_nRemainTimeMSec <= 5*1000)
//		{
//			// 남은 시간 전송
//		}
#endif
	}

	if( m_nRemainTimeMin <= 0 && m_nRemainTimeMSec <= 0 )
	{
		m_bEntrance = FALSE;
		CheckUsersOnConnect();

		if(m_nUserCount < MIN_ILLUSION_TEMPLE_USER_COUNT)	// 최소 4명이 있어야 시작
		{
			// 인원 부족으로 이벤트 실패시 처리
			// 피의 두루마리의 내구도 변경
			for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
			{
				if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2 
					&& gObj[m_UserData[i].m_nIndex].MapNumber == m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1)
				{
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
					int nScroolPos = m_UserData[i].m_btItemPos;
#else
					int nScroolPos = SearchScollOfBloodItem(m_UserData[i].m_nIndex, 0);
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617					
					if(nScroolPos  > -1)
					{
#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
						if( gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Type == MAKE_ITEMNUM( 13, 51 ) )
						{
							gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability = 1;
							GCItemDurSend(gObj[m_UserData[i].m_nIndex].m_Index, nScroolPos, gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability, 0);
						}
						else if( gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Type == MAKE_ITEMNUM( 13, 61 ) )
						{
							gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability++;
							GCItemDurSend(gObj[m_UserData[i].m_nIndex].m_Index, nScroolPos, gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability, 0);
						}
#else
						gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability = 1;
						GCItemDurSend(gObj[m_UserData[i].m_nIndex].m_Index, nScroolPos, gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Durability, 0);
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
						
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[Illusion Temple] (%d) Payback to User (%s)(%s) 피의 두루마리 (Serial:%u)",
#else
						LogAddTD("[Illusion Temple] (%d) Payback to User (%s)(%s) 피의 두루마리 (Serial:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
							m_nTempleNumber+1, gObj[m_UserData[i].m_nIndex].AccountID, gObj[m_UserData[i].m_nIndex].Name,
							gObj[m_UserData[i].m_nIndex].pInventory[nScroolPos].m_Number);							
					}
					else
					{
						LogAddTD("[Illusion Temple] (%d) Payback to User Failed (%s)(%s) Scroll Not Found",
							m_nTempleNumber+1, gObj[m_UserData[i].m_nIndex].AccountID, gObj[m_UserData[i].m_nIndex].Name);
					}
					
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
					// 데비아스로 이동
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 사원에서 엘베란드로
					gObjMoveGate(m_UserData[i].m_nIndex, 267);
#else
					gObjMoveGate(m_UserData[i].m_nIndex, 22);
#endif
#endif
				}
			}

			LogAddTD("[Illusion Temple] (%d) Failed to Start Illusion Temple (UserCount: %d)", m_nTempleNumber +1, m_nUserCount);

			// 이벤트 실패
			SetIllusionTempleState(ILLUSION_TEMPLE_STATE_END);
			return;
		}

		// 출발 위치로 이동
		int nTeam				= 0;
		int nAlliedPartyNum		= -1;
		int nIllusionPartyNum	= -1;

		char szAlliedTeamLog[256];
		char szIllusionTeamLog[256];

		sprintf(szAlliedTeamLog, "[Illusion Temple] (%d) [AlliedTeam] ", m_nTempleNumber +1);
		sprintf(szIllusionTeamLog, "[Illusion Temple] (%d) [IllusiondTeam] ", m_nTempleNumber +1);

		for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
		{
			if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].m_iIllusionTempleIndex == i)
			{
				//
				if( gObj[m_UserData[i].m_nIndex].Class >= MAX_CLASSTYPE || gObj[m_UserData[i].m_nIndex].Class < 0 )
				{
					continue;
				}
				
				// 피의 두루마리를 지움
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
				int nScrollPos = m_UserData[i].m_btItemPos;
#else
				int nScrollPos = SearchScollOfBloodItem(m_UserData[i].m_nIndex, 0);
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
				if(nScrollPos  > -1)
				{
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
					// 내구도가 0이 아닐땐 continue;
					if( gObj[m_UserData[i].m_nIndex].pInventory[nScrollPos].m_Durability <= 0 ) {
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
					if( gObj[m_UserData[i].m_nIndex].pInventory[nScrollPos].m_Type == MAKE_ITEMNUM( 13, 51 ) )
					{
						gObjInventoryDeleteItem(m_UserData[i].m_nIndex, nScrollPos);
						GCInventoryItemDeleteSend(m_UserData[i].m_nIndex, nScrollPos, 0);
	#ifdef MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
						m_UserData[i].m_btItemPos = -1;
	#endif // MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
					}
					else if( gObj[m_UserData[i].m_nIndex].pInventory[nScrollPos].m_Type == MAKE_ITEMNUM( 13, 61 ) )
					{
						if( gObj[m_UserData[i].m_nIndex].pInventory[nScrollPos].m_Durability <= 0 )
						{
							gObjInventoryDeleteItem(m_UserData[i].m_nIndex, nScrollPos);
							GCInventoryItemDeleteSend(m_UserData[i].m_nIndex, nScrollPos, 0);
	#ifdef MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
							m_UserData[i].m_btItemPos = -1;
	#endif // MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
						}
					}
#else
					gObjInventoryDeleteItem(m_UserData[i].m_nIndex, nScrollPos);				
					GCInventoryItemDeleteSend(m_UserData[i].m_nIndex, nScrollPos, 0);
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
					}
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
				}

				// 클래스별 팀 나눔
				if(m_btAlliedClassCnt[gObj[m_UserData[i].m_nIndex].Class] == m_btIllusionClassCnt[gObj[m_UserData[i].m_nIndex].Class])
				{
					if(m_nAlliedUserCount == m_nIllusionUserCount)
					{
						m_UserData[i].m_btTeam = rand()%2;
					}
					else if(m_nAlliedUserCount > m_nIllusionUserCount)
					{
						m_UserData[i].m_btTeam = TEAM_ILLUSION;
					}
					else	// m_nAlliedUserCount < m_nIllusionUserCount
					{
						m_UserData[i].m_btTeam = TEAM_ALLIED;
					}
				}
				else if(m_btAlliedClassCnt[gObj[m_UserData[i].m_nIndex].Class] < m_btIllusionClassCnt[gObj[m_UserData[i].m_nIndex].Class])
				{
					if(m_nAlliedUserCount > m_nIllusionUserCount)
					{
						m_UserData[i].m_btTeam = TEAM_ILLUSION;
					}
					else
					{
						m_UserData[i].m_btTeam = TEAM_ALLIED;
					}
				}
				else
				{
					if(m_nAlliedUserCount < m_nIllusionUserCount)
					{
						m_UserData[i].m_btTeam = TEAM_ALLIED;
					}
					else
					{
						m_UserData[i].m_btTeam = TEAM_ILLUSION;
					}					
				}
				
				// 팀을 나눔
				//if(nTeam%2?TRUE:FALSE)
				if(m_UserData[i].m_btTeam == TEAM_ALLIED)
				{
					m_UserData[i].m_btTeam = TEAM_ALLIED;
					m_btAlliedClassCnt[gObj[m_UserData[i].m_nIndex].Class]++;

					if(nAlliedPartyNum == -1 && gObj[m_UserData[i].m_nIndex].PartyNumber < 0)
					{
						nAlliedPartyNum = gParty.Create(m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].DBNumber, gObj[m_UserData[i].m_nIndex].Level);	
						if(nAlliedPartyNum == -1)
						{
							// error
							LogAddTD("[Illusion Temple] (%d) Party Create Error, (Team: %d)", m_nTempleNumber +1, m_UserData[i].m_btTeam);
						}
						gObj[m_UserData[i].m_nIndex].PartyNumber = nAlliedPartyNum;
					}
					else
					{
						int nRet = gParty.Add(nAlliedPartyNum, m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].DBNumber, gObj[m_UserData[i].m_nIndex].Level);						
						if(nRet >= 0)
						{
							gObj[m_UserData[i].m_nIndex].PartyNumber = nAlliedPartyNum;
						}
						else
						{
							// error
							LogAddTD("[Illusion Temple] (%d) Party Add Error, (Team: %d)", m_nTempleNumber +1, m_UserData[i].m_btTeam);
						}
					}
					gObjMoveGate(m_UserData[i].m_nIndex, GATE_ILLUSION_TEMPLE_ALLIED1 + m_nTempleNumber);

					// 캐릭터 변신
					gObj[m_UserData[i].m_nIndex].m_Change = ALLIED_CHARACTER;	
					gObjViewportListProtocolCreate(&gObj[m_UserData[i].m_nIndex]);
					
					m_nAlliedUserCount++;
					nTeam++;

					int tmpSize = strlen(szAlliedTeamLog);
					sprintf(szAlliedTeamLog+tmpSize , "(%s),(%s) ", gObj[m_UserData[i].m_nIndex].AccountID, gObj[m_UserData[i].m_nIndex].Name);
				}
				else
				{
					m_UserData[i].m_btTeam = TEAM_ILLUSION;
					m_btIllusionClassCnt[gObj[m_UserData[i].m_nIndex].Class]++;

					if(nIllusionPartyNum == -1 && gObj[m_UserData[i].m_nIndex].PartyNumber < 0)
					{
						nIllusionPartyNum = gParty.Create(m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].DBNumber, gObj[m_UserData[i].m_nIndex].Level);	
						if(nIllusionPartyNum == -1)
						{
							// error
							LogAddTD("[Illusion Temple] (%d) Party Create Error, (Team: %d)", m_nTempleNumber +1, m_UserData[i].m_btTeam);
						}
						gObj[m_UserData[i].m_nIndex].PartyNumber = nIllusionPartyNum;
					}
					else
					{
						int nRet = gParty.Add(nIllusionPartyNum, m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].DBNumber, gObj[m_UserData[i].m_nIndex].Level);						
						if(nRet >= 0)
						{
							gObj[m_UserData[i].m_nIndex].PartyNumber = nIllusionPartyNum;
						}
						else
						{
							// error
							LogAddTD("[Illusion Temple] (%d) Party Add Error, (Team: %d)", m_nTempleNumber +1, m_UserData[i].m_btTeam);
						}
					}
					gObjMoveGate(m_UserData[i].m_nIndex, GATE_ILLUSION_TEMPLE_ILLUSION1 + m_nTempleNumber);

					// 캐릭터 변신
					gObj[m_UserData[i].m_nIndex].m_Change = ILLUSION_CHARACTER;	
					gObjViewportListProtocolCreate(&gObj[m_UserData[i].m_nIndex]);

					m_nIllusionUserCount++;
					nTeam++;

					int tmpSize = strlen(szIllusionTeamLog);
					sprintf(szIllusionTeamLog+tmpSize , "(%s),(%s) ", gObj[m_UserData[i].m_nIndex].AccountID, gObj[m_UserData[i].m_nIndex].Name);
				}
			}
		}
		
		// 파티 정보를 보냄
		CGPartyListAll(nAlliedPartyNum);
		CGPartyListAll(nIllusionPartyNum);

		// ILLUSION_TEMPLE_STATE_READY 상태로 변경
		SetIllusionTempleState(ILLUSION_TEMPLE_STATE_READY);

		LogAddTD("[Illusion Temple] (%d) Allied Team 법사(%d),기사(%d), 요정(%d), 마검사(%d), 다크로드(%d)",
			m_nTempleNumber +1,							// 사원번호
			m_btAlliedClassCnt[CLASS_WIZARD],
			m_btAlliedClassCnt[CLASS_KNIGHT],
			m_btAlliedClassCnt[CLASS_ELF],
			m_btAlliedClassCnt[CLASS_MAGUMSA],	
			m_btAlliedClassCnt[CLASS_DARKLORD]);

		LogAddTD(szAlliedTeamLog);

		LogAddTD("[Illusion Temple] (%d) Illusion Team 법사(%d),기사(%d), 요정(%d), 마검사(%d), 다크로드(%d)",
			m_nTempleNumber +1,							// 사원번호
			m_btIllusionClassCnt[CLASS_WIZARD],
			m_btIllusionClassCnt[CLASS_KNIGHT],
			m_btIllusionClassCnt[CLASS_ELF],
			m_btIllusionClassCnt[CLASS_MAGUMSA],	
			m_btIllusionClassCnt[CLASS_DARKLORD]);

		LogAddTD(szIllusionTeamLog);
	}
}

// 이벤트 준비
void CIllusionTempleProcess::ProcIllusionTempleState_Ready(int nCurTime)
{
	INT iTICK_MSEC = nCurTime - m_nTickCount;
	
	if (iTICK_MSEC >= 1000) // 1초마다 들어옴
	{
		m_nRemainTimeMSec	-= iTICK_MSEC;
		m_nTickCount		= nCurTime;

		if(m_nRemainTimeMSec <= 30*1000 && m_nRemainTimeMSec > 0
			&& m_bSendMsgBeforePlay == FALSE)
		{
			m_bSendMsgBeforePlay = TRUE;

			// 시작전 30초를 보냄
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = TEMPLE_MSG_BEFORE_PLAY;
		
			for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
			{
				if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2
					&& gObj[m_UserData[i].m_nIndex].MapNumber == m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1)
				{
					DataSend(m_UserData[i].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	if(m_nUserCount == 0)
	{
		SetIllusionTempleState(ILLUSION_TEMPLE_STATE_END);
		return;
	}

	if( m_nRemainTimeMSec <= 0 )
	{
		// ILLUSION_TEMPLE_STATE_PLAYING 상태로 변경
		SetIllusionTempleState(ILLUSION_TEMPLE_STATE_PLAYING);

		SendIllusionTemplePlayState(TEAM_ALLIED);
		SendIllusionTemplePlayState(TEAM_ILLUSION);
	}
}

// 이벤트 진행
void CIllusionTempleProcess::ProcIllusionTempleState_Playing(int nCurTime)
{
	INT iTICK_MSEC = nCurTime - m_nTickCount;
	
	if (iTICK_MSEC >= 1000) // 1초마다 들어옴
	{
		m_nRemainTimeMSec	-= iTICK_MSEC;
		m_nTickCount		= nCurTime;

		if( m_nRemainTimeMSec > 0 )
		{
			SendIllusionTemplePlayState(TEAM_ALLIED);
			SendIllusionTemplePlayState(TEAM_ILLUSION);
		}

		// 석상 리젠 시간 설정
		if( m_StatusRegenTime != -1)
		{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
			if(m_StatusRegenTime < nCurTime) 
#else
			if(m_StatusRegenTime > nCurTime) 
#endif
			{
				RegenStatus();	
			}
		}
	}

	if(m_nUserCount == 0)
	{
		SetIllusionTempleState(ILLUSION_TEMPLE_STATE_END);
		return;
	}

	if( m_nRemainTimeMSec <= 0				// 시간 종료
		|| m_btAlliedPoint >= ILLUSION_TEMPLE_MAX_WIN_POINT || m_btIllusionPoint >= ILLUSION_TEMPLE_MAX_WIN_POINT	// 승리점수 획득
		|| m_nAlliedUserCount == 0 || m_nIllusionUserCount == 0)							// 한쪽 팀이 아무도 없을때
	{
		if(m_btAlliedPoint >= ILLUSION_TEMPLE_MAX_WIN_POINT || m_btIllusionPoint >= ILLUSION_TEMPLE_MAX_WIN_POINT)
		{
			// 게임 종료 시 남은 시간 저장
			if(m_nRemainTimeMSec <= 0)
			{
				m_nEnventEndRemainTime = 0;
			}
			else
			{
				m_nEnventEndRemainTime = m_nRemainTimeMSec / 1000;
			}		
		}
		
		// 현재 상태 전송
		SendIllusionTemplePlayState(TEAM_ALLIED);
		SendIllusionTemplePlayState(TEAM_ILLUSION);
		
		// ILLUSION_TEMPLE_STATE_END 상태로 변경
		SetIllusionTempleState(ILLUSION_TEMPLE_STATE_END);
	}
}

// 이벤트 종료
void CIllusionTempleProcess::ProcIllusionTempleState_End(int nCurTime)
{
	INT iTICK_MSEC = nCurTime - m_nTickCount;
	
	if (iTICK_MSEC >= 1000) // 1초마다 들어옴
	{
		m_nRemainTimeMSec	-= iTICK_MSEC;
		m_nTickCount	= nCurTime;

		// 3초 후에 각 진영의 시작지점으로 이동
		if( m_nRemainTimeMSec <= ILLUSION_TEMPLE_END_TIME*60*1000 - 3*1000 
			&& m_nRemainTimeMSec > 0 && m_bGameEndPosMove == FALSE)
		{
			m_bGameEndPosMove = TRUE;

			for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
			{
				if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2
					&& gObj[m_UserData[i].m_nIndex].MapNumber == m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1)
				{
					if( m_UserData[i].m_btTeam == TEAM_ALLIED )
					{
						gObjMoveGate(m_UserData[i].m_nIndex, GATE_ILLUSION_TEMPLE_ALLIED1 + m_nTempleNumber);
					}
					else if ( m_UserData[i].m_btTeam == TEAM_ILLUSION )
					{
						gObjMoveGate(m_UserData[i].m_nIndex, GATE_ILLUSION_TEMPLE_ILLUSION1 + m_nTempleNumber);
					}
					
#ifdef ADD_LOG_ILLUSION_TEMPLE_1_20070727
						gObjSetPosition(m_UserData[i].m_nIndex, gObj[m_UserData[i].m_nIndex].X, gObj[m_UserData[i].m_nIndex].Y);
#endif
				}
			}

			// 성물 제거
			if(m_nRelicsUserIndex >= 0)
			{
				if(m_btInventoryPos >= 0 && gObj[m_nRelicsUserIndex].pInventory[m_btInventoryPos].m_Type == MAKE_ITEMNUM(14, 64) )
				{
					gObjInventoryDeleteItem(m_nRelicsUserIndex, m_btInventoryPos);				
					GCInventoryItemDeleteSend(m_nRelicsUserIndex, m_btInventoryPos, 0);
					
					m_nRelicsUserIndex	= -1;
					m_btInventoryPos	= -1;
					m_btRelicsTeam		= 0xFF;
				}
			}

			// 경기 결과 점수 계산
			CalCursetTempleResult();
		}
	}

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013		// 경기 결과 전송
	// 6초 후에 각 사원 경기 결과를 전송
	if( m_nRemainTimeMSec <= ILLUSION_TEMPLE_END_TIME*60*1000 - 6*1000 
		&& m_nRemainTimeMSec > 0 && m_bGameEndPosMove == TRUE && m_bSendResult == FALSE )
	{
			SendIllusionTempleResult();
			m_bSendResult				= TRUE;
	}
#endif

	if( m_nRemainTimeMSec <= 0 )
	{
		SetIllusionTempleState(ILLUSION_TEMPLE_STATE_NONE);
	}
}

// 이벤트 상태 설정
void CIllusionTempleProcess::SetIllusionTempleState(BYTE btState)
{
	m_btIllusionTempleState = btState;

	switch(btState)
	{
	case ILLUSION_TEMPLE_STATE_NONE:
		SetIllusionTempleStateNone();
		break;
	case ILLUSION_TEMPLE_STATE_READY:
		SetIllusionTempleStateReady();
		break;
	case ILLUSION_TEMPLE_STATE_PLAYING:
		SetIllusionTempleStatePlaying();
		break;
	case ILLUSION_TEMPLE_STATE_END:
		SetIllusionTempleStateEnd();
		break;
	}
}

// 이벤트 상태 얻음
BYTE CIllusionTempleProcess::GetIllusionTempleState()
{
	return m_btIllusionTempleState;
}

void CIllusionTempleProcess::SetIllusionTempleStateNone()
{
#if TESTSERVER == 1
	char pMsg[256];
	sprintf(pMsg, "ILLUSION_TEMPLE(%d) CurrentState (ILLUSION_TEMPLE_STATE_NONE)", m_nTempleNumber);
	SendNoticeMessage(pMsg);
#endif

	SendIllusionTempleState(ILLUSION_TEMPLE_STATE_NONE);

	// 데비아스로 이동
	for (int i = 0 ; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++) 
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2 
			&& gObj[m_UserData[i].m_nIndex].MapNumber == m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1)
		{
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 사원에서 엘베란드로
			gObjMoveGate(m_UserData[i].m_nIndex, 267);
#else
			gObjMoveGate(m_UserData[i].m_nIndex, 22);
#endif
		}
	}
	
	// 이벤트 초기화
	IllusionTempleProcessInit(m_nTempleNumber);
}

void CIllusionTempleProcess::SetIllusionTempleStateReady()
{
	m_nRemainTimeMSec	= ILLUSION_TEMPLE_READY_TIME * 60*1000;	// n분 후 Playing 상태로

#if TESTSERVER == 1
	char pMsg[256];
	sprintf(pMsg, "ILLUSION_TEMPLE(%d) CurrentState (ILLUSION_TEMPLE_STATE_READY)", m_nTempleNumber);
	SendNoticeMessage(pMsg);
#endif

	SendIllusionTempleState(ILLUSION_TEMPLE_STATE_READY);

	// 초기 스킬포인트를 보내줌
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2
			&& gObj[m_UserData[i].m_nIndex].MapNumber == m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1)
		{
			GCIllusionTempleSkillPoint(m_UserData[i].m_nIndex, DEFAULT_SKILL_POINT);
		}
	}
}

void CIllusionTempleProcess::SetIllusionTempleStatePlaying()
{
	m_nRemainTimeMSec	= ILLUSION_TEMPLE_PLAY_TIME * 60*1000;		// 분후 End 상태로

#if TESTSERVER == 1
	char pMsg[256];
	sprintf(pMsg, "ILLUSION_TEMPLE(%d) CurrentState (ILLUSION_TEMPLE_STATE_PLAYING)", m_nTempleNumber);
	SendNoticeMessage(pMsg);
#endif

	// 석상, 성물보관함을 생성
	AddNpcStatus();
	AddNpcRelicsBox();
	AddMonsterGhost();
	
	// 석상을 리젠시킴
	SetStatusRegenTime();
	
	SendIllusionTempleState(ILLUSION_TEMPLE_STATE_PLAYING);
}

void CIllusionTempleProcess::SetIllusionTempleStateEnd()
{
	m_nRemainTimeMSec	= ILLUSION_TEMPLE_END_TIME * 60*1000;		// 초후 None 상태로

#if TESTSERVER == 1
	char pMsg[256];
	sprintf(pMsg, "ILLUSION_TEMPLE(%d) CurrentState (ILLUSION_TEMPLE_STATE_END)", m_nTempleNumber);
	SendNoticeMessage(pMsg);
#endif
	
	SendIllusionTempleState(ILLUSION_TEMPLE_STATE_END);

	// 성물 제거
/*	if(m_nRelicsUserIndex >= 0)
	{
		if(m_btInventoryPos >= 0 && gObj[m_nRelicsUserIndex].pInventory[m_btInventoryPos].m_Type == MAKE_ITEMNUM(14, 64) )
		{
			gObjInventoryDeleteItem(m_nRelicsUserIndex, m_btInventoryPos);				
			GCInventoryItemDeleteSend(m_nRelicsUserIndex, m_btInventoryPos, 0);

			m_nRelicsUserIndex	= -1;
			m_btInventoryPos	= -1;
			m_btRelicsTeam		= 0xFF;
		}
	}
*/
	// 스킬 상태 제거
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2 
			&& gObj[m_UserData[i].m_nIndex].MapNumber == m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1)
		{
			if(m_UserData[i].m_dwSkillProdectionTime > 0)
			{
				m_UserData[i].m_dwSkillProdectionTime = 0;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				gObjRemoveBuffEffect( &gObj[m_UserData[i].m_nIndex], BUFFTYPE_TEMPLE_PROTECTION );
#else
				DEL_VIEWSKILLSTATE( gObj[m_UserData[i].m_nIndex].m_ViewSkillState, STATE_ILLUSION_TEMPLE_PRODECTION );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				SendIllusionTempleSkillEnd(&gObj[m_UserData[i].m_nIndex], AT_SKILL_PRODECTION);
			}

			if(m_UserData[i].m_dwSkillRestraintTime > 0)
			{
				m_UserData[i].m_dwSkillRestraintTime = 0;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				gObjRemoveBuffEffect( &gObj[m_UserData[i].m_nIndex], BUFFTYPE_RESTRICTION );
#else
				DEL_VIEWSKILLSTATE( gObj[m_UserData[i].m_nIndex].m_ViewSkillState, STATE_ILLUSION_TEMPLE_RESTRAINT );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				SendIllusionTempleSkillEnd(&gObj[m_UserData[i].m_nIndex], AT_SKILL_RESTRAINT);
			}
		}
	}

	// NPC, Monster 제거
	RemoveAllObj();
}

// 이벤트맵 입장
BOOL CIllusionTempleProcess::EnterUserIllusionTemple( int aIndex, BYTE btTempleIndex 
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
													 , BYTE btItemPos
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
													 )
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if(GetIllusionTempleState() >= ILLUSION_TEMPLE_STATE_READY )
	{
		return FALSE;
	}

	if(gObj[aIndex].m_iIllusionTempleIndex != -1)
	{
		// error
		LogAddTD("Enter Error: IllusionTempleIndex Fail");
		return FALSE;
	}

	// 성물을 가지고 있으면 지운다
	int nItemPos = SearchRelicsItem(aIndex);
	if(nItemPos >= 0 && gObj[aIndex].pInventory[nItemPos].m_Type == MAKE_ITEMNUM(14, 64) )
	{
		gObjInventoryDeleteItem(aIndex, nItemPos);				
		GCInventoryItemDeleteSend(aIndex, nItemPos, 0);
	}

	BOOL bResult = FALSE;

	EnterCriticalSection(&m_critUserData);
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		if(m_UserData[i].m_nIndex == -1)
		{
			m_UserData[i].m_nIndex = aIndex;
			m_nUserCount++;
			gObj[aIndex].m_iIllusionTempleIndex	= i;
			bResult = TRUE;
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
			m_UserData[i].m_btItemPos = btItemPos;
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617

			break;
		}
	}
	LeaveCriticalSection(&m_critUserData);

	return bResult;
}

// 이벤트맵 나감
BOOL CIllusionTempleProcess::LeaveUserIllusionTemple( int aIndex )
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	EnterCriticalSection(&m_critUserData);
	if(gObj[aIndex].m_iIllusionTempleIndex != -1 
		&& aIndex == m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_nIndex)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
		{
			// 예외처리 - 내구도가 0인 티켓 삭제(사원준비전에 이동창을 통해서 이동할 경우, 접속 종료)
			LPILLUSION_TEMPLE_USER lpUserData = &m_UserData[gObj[aIndex].m_iIllusionTempleIndex];
			if( lpUserData->m_btItemPos > -1 ) {
				CItem* pItem = &gObj[aIndex].pInventory[lpUserData->m_btItemPos];
	#ifdef MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826	
				if( pItem && pItem->IsItem() == TRUE && pItem->m_Durability < 1
					&&(pItem->m_Type == MAKE_ITEMNUM(13, 51) || pItem->m_Type == MAKE_ITEMNUM(13, 61)) 
					)
				{
	#else // MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
				if( pItem && pItem->IsItem() == TRUE && pItem->m_Durability < 1) {
	#endif // MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
					gObjInventoryDeleteItem(aIndex, lpUserData->m_btItemPos);
					GCInventoryItemDeleteSend(aIndex, lpUserData->m_btItemPos, 0);
				}
			}
		}
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617

		// 스킬 상태 제거
		if(m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_dwSkillProdectionTime > 0)
		{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjRemoveBuffEffect( &gObj[aIndex], BUFFTYPE_TEMPLE_PROTECTION );
#else
			DEL_VIEWSKILLSTATE( gObj[aIndex].m_ViewSkillState, STATE_ILLUSION_TEMPLE_PRODECTION );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			SendIllusionTempleSkillEnd(&gObj[aIndex], AT_SKILL_PRODECTION);
		}

		if(m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_dwSkillRestraintTime > 0)
		{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjRemoveBuffEffect( &gObj[aIndex], BUFFTYPE_RESTRICTION );
#else
			DEL_VIEWSKILLSTATE( gObj[aIndex].m_ViewSkillState, STATE_ILLUSION_TEMPLE_RESTRAINT );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			SendIllusionTempleSkillEnd(&gObj[aIndex], AT_SKILL_RESTRAINT);
		}

#ifndef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013	// 사용자 정보 초기화 위치수정
		ClearUserData(gObj[aIndex].m_iIllusionTempleIndex);
		gObj[aIndex].m_iIllusionTempleIndex = -1;
		m_nUserCount--;
#endif

		if(m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_btTeam == TEAM_ALLIED)
		{
			m_nAlliedUserCount--;
		}
		else if(m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_btTeam == TEAM_ILLUSION)
		{
			m_nIllusionUserCount--;
		}

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013
		ClearUserData(gObj[aIndex].m_iIllusionTempleIndex);
		gObj[aIndex].m_iIllusionTempleIndex = -1;
		m_nUserCount--;
#endif

		LogAddTD("[Illusion Temple] (%d) LeaveUser: (%s)(%s)", 
				m_nTempleNumber+1, gObj[aIndex].AccountID, gObj[aIndex].Name);
	}
	LeaveCriticalSection(&m_critUserData);

	// 파티를 끊는다.
	if( gObj[aIndex].PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		
		int pindex = gParty.GetIndex(gObj[aIndex].PartyNumber, gObj[aIndex].m_Index, gObj[aIndex].DBNumber);
		if( pindex >= 0 )
		{
			pMsg.Number = pindex;
			CGPartyDelUser( &pMsg, gObj[aIndex].m_Index);
		}
	}

	// 캐릭터 원래상태로 변신
	gObj[aIndex].m_Change = -1;	
	gObjViewportListProtocolCreate(&gObj[aIndex]);

	return TRUE;
}

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
int CIllusionTempleProcess::CheckCanEnterLevel(int aIndex)
{
	int iLevel = 0;
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	if( lpObj->Level < MIN_ENTER_LEVEL1 )
	{
		return 0;
	}
	else if( lpObj->Level < MIN_ENTER_LEVEL2 )
	{
		return 1;
	}
	else if( lpObj->Level < MIN_ENTER_LEVEL3 )
	{
		return 2;
	}
	else if( lpObj->Level < MIN_ENTER_LEVEL4 )
	{
		return 3;
	}
	else if( lpObj->Level < MIN_ENTER_LEVEL5 )
	{
		return 4;
	}
#ifdef ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else if( lpObj->Level <= MIN_ENTER_LEVEL6 && !lpObj->ThirdChangeUp)
	{
		return 5;
	}
	else if( lpObj->Level == MIN_ENTER_LEVEL6 && lpObj->ThirdChangeUp)
	{
		if(g_bUseMaxLevelIllusionTemple)
		{
			return 6;
		}
		else
		{
			return 5;
		}
	}
#else	// ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else if( lpObj->Level < MIN_ENTER_LEVEL6 )
	{
		return 5;
	}
#endif	// ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else
	{
		return -1;
	}

	return -1;
}
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011

// 환영 사원에 입장한 사용자가 접속 중인지 체크
void CIllusionTempleProcess::CheckUsersOnConnect()
{
	EnterCriticalSection(&m_critUserData);
	
	m_nUserCount = 0;
	for (int i = 0 ; i < MAX_ILLUSION_TEMPLE_USER_COUNT ; i++) {
		if (m_UserData[i].m_nIndex != -1) {
			// 특정 사용자가 접속 중이 아니면 공간을 비운다.
			if (!gObjIsConnected(m_UserData[i].m_nIndex))
			{
				gObj[m_UserData[i].m_nIndex].m_iIllusionTempleIndex = -1;
				ClearUserData(i);
			}
			else 
			{
				if (gObj[m_UserData[i].m_nIndex].MapNumber != MAP_INDEX_ILLUSION_TEMPLE1 + m_nTempleNumber) 
				{
					gObj[m_UserData[i].m_nIndex].m_iIllusionTempleIndex = -1;
					ClearUserData(i);					
				}
				else
				{
					m_nUserCount++;
				}
			}
		}
	}

	LeaveCriticalSection(&m_critUserData);
}

// 입장시 시작까지 남은 시간(분)
BYTE CIllusionTempleProcess::GetRemainTimeMin()
{
	if(GetIllusionTempleState() == ILLUSION_TEMPLE_STATE_NONE && GetEntrance() == TRUE)
	{
		return m_nRemainTimeMin;
	}

	return 0;
}

// 입장까지 남은 시간(분)
BYTE CIllusionTempleProcess::GetEnterRemainTimeMin()
{
	BYTE btRemainTime = 0;

	if(GetIllusionTempleState() == ILLUSION_TEMPLE_STATE_NONE && GetEntrance() == TRUE)
	{
		// 입장가능
		btRemainTime = 0;
	}
	else if(GetIllusionTempleState() == ILLUSION_TEMPLE_STATE_NONE && GetEntrance() == FALSE)
	{
		// 입장 까지 남은시간
		btRemainTime = m_nRemainTimeMin - ILLUSION_TEMPLE_OPEN_TIME;
	}
	else
	{
		// 입장까지 남은시간
		list<ILLUSION_TEMPLE_STARTTIME>::iterator iter;
		ILLUSION_TEMPLE_STARTTIME	prevStartTime;
		ILLUSION_TEMPLE_STARTTIME	nextStartTime;
		
		DWORD	dwPrevTime	= 0;;		// 이전 시작 시간(분)
		DWORD	dwNextTime	= 0;;		// 다음 시작 시간(분)
		DWORD	dwCurTime	= 0;;		// 현재 시간(분)
		
		struct tm *today;
		time_t ltime;
		
		if(LST_ILLUSION_TIME_START_TIME.size() == 0)
		{
			MsgBox("Error : Illusion Temple StartTime size is 0");
			return 0;
		}
		
		time( &ltime );
		today = localtime( &ltime );
		dwCurTime	= today->tm_hour * 60 + today->tm_min;
		
		// 다음 이벤트 열리 시간을 검색
		prevStartTime = *LST_ILLUSION_TIME_START_TIME.begin();
		for(iter = LST_ILLUSION_TIME_START_TIME.begin(); iter != LST_ILLUSION_TIME_START_TIME.end(); ++iter)
		{
			nextStartTime = *iter;
			
			// 시작시간을 분으로 계산
			dwPrevTime	= prevStartTime.nHour * 60 + prevStartTime.nMin;
			dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;
			
			if(dwPrevTime == dwNextTime)
			{
				if(dwCurTime < dwNextTime)
				{
					nextStartTime = *iter;
					break;
				}
				else
				{
					continue;
				}
			}
			
			if(dwCurTime >= dwPrevTime && dwCurTime < dwNextTime)	
			{
				break;			
			}
			
			prevStartTime = *iter;
		}
		
		int Cnt = 2;
		while(Cnt--)
		{
			if(iter == LST_ILLUSION_TIME_START_TIME.end())
			{
				// 다음 날 시간
				iter = LST_ILLUSION_TIME_START_TIME.begin();
				nextStartTime = *iter;
			}
			
			// 다음 시작시간을 다시 계산
			dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;
			
			// 다음 시작까지 남은 시간 계산
			if(today->tm_hour <= nextStartTime.nHour)
			{
				// 다음 이벤트가 오늘
				btRemainTime = dwNextTime - dwCurTime;
			}
			else
			{
				// 다음 이벤트가 다음날
				btRemainTime = 24 * 60 - dwCurTime + dwNextTime;
			}
			
			// 현재 시작까지 남은 시간(btRemainTime)이 오픈시간(m_iOpenTime)보다 작으면 다음 시간으로 넘긴다
			if( btRemainTime <= ILLUSION_TEMPLE_OPEN_TIME)
			{
				iter++;
				if(iter != LST_ILLUSION_TIME_START_TIME.end())
				{
					nextStartTime = *iter;
				}
			}
			else
			{
				btRemainTime -= ILLUSION_TEMPLE_OPEN_TIME;
				break;
			}
		}
		
		// 다음 분(min) 계산
		int tmpTime = GetTickCount();
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
		m_nNextTimeMin = tmpTime + (60 * 1000 - today->tm_sec * 1000);
#else
		m_nNextTimeMin = tmpTime + (today->tm_sec * 1000);
#endif
	}

	return btRemainTime;
}

// 석상 행동
void CIllusionTempleProcess::IllusionHolyRelicsStatusAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	// 리젠상태 확인
	if(lpNpc->m_State == OBJST_NONE || lpNpc->m_PosNum	== -1)
		return;

	// 석상과의 거리를 체크한다.
	if (abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3 ) {
		return;
	}

	// 석상을 없앰
	lpNpc->m_PosNum	= -1;

	lpNpc->Life = 0;
	//lpNpc->m_State	= OBJST_NONE;
	gObjLifeCheck(lpNpc, lpObj, 0, 1, 0, 0, 0);
	

	// 성물을 드랍
	float fDur		= 0;
	int iType		= 0;
	int iLevel		= 0;
	int iOption1	= 0;
	int iOption2	= 0;
	int iOption3	= 0;
	int iDropItemNum	= 0;
	int	iDropItemRate	= 0;
	int iExOption	= 0;

	iType			= ItemGetNumberMake(14, 64);
	iOption1		= 0; 
	iOption2		= 0;
	iOption3		= 0;
	iLevel			= 0;

	ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, (BYTE)lpNpc->X, (BYTE)lpNpc->Y, iType, iLevel, 
		(BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );

	LogAddTD("[Illusion Temple] (%d) [%s][%s] Click To Status (X:%d, Y:%d)", 
		m_nTempleNumber +1, lpObj->AccountID, lpObj->Name, lpNpc->X, lpNpc->Y);

	RegenMonster();
}

// 성물 보관함행동(연합군)
void CIllusionTempleProcess::HolyRelicsBoxAlliedAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if(m_btRelicsTeam == 0xFF || m_btRelicsTeam != TEAM_ALLIED || lpObj->m_Index != m_nRelicsUserIndex)
		return;

	if( lpObj->pInventory[m_btInventoryPos].m_Type == MAKE_ITEMNUM(14, 64) )
	{
		m_btAlliedPoint++;
		DeleteRelicsItem(lpObj);

		m_btRelicsTeam		= 0xFF;
		m_nRelicsUserIndex	= -1;
		m_btInventoryPos	= -1;

		IncRelicsMoveCount(lpObj->m_Index);

		// 로그수정
		LogAddTD("[Illusion Temple] (%d) Increase AlliedTeam Relics Point: (%s)(%s)(Score:%d)", 
				m_nTempleNumber+1, lpObj->AccountID, lpObj->Name, m_btAlliedPoint);
	}

	SetStatusRegenTime();
}

// 성물 보관함행동(환영교)
void CIllusionTempleProcess::HolyRelicsBoxIllusionAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if(m_btRelicsTeam == 0xFF || m_btRelicsTeam != TEAM_ILLUSION || lpObj->m_Index != m_nRelicsUserIndex)
		return;

	if( lpObj->pInventory[m_btInventoryPos].m_Type == MAKE_ITEMNUM(14, 64) )
	{
		m_btIllusionPoint++;
		DeleteRelicsItem(lpObj);

		m_btRelicsTeam		= 0xFF;
		m_nRelicsUserIndex	= -1;
		m_btInventoryPos	= -1;

		IncRelicsMoveCount(lpObj->m_Index);

		// 로그 수정
		LogAddTD("[Illusion Temple] (%d) Increase IllusionTeam Relics Point: (%s)(%s)(Score:%d)", 
				m_nTempleNumber+1, lpObj->AccountID, lpObj->Name, m_btIllusionPoint);
	}

	SetStatusRegenTime();
}

// 인벤토리에서 성물을 찾음
int CIllusionTempleProcess::SearchRelicsItem(int iIndex)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 64) )		// 성물이 존재한다면
			{
				return x;
			}
		}
	}

	return -1;
}

// 인벤토리에서 아이템을 지움 (이벤트 중일 경우에만 사용)
void CIllusionTempleProcess::DeleteRelicsItem(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->pInventory[m_btInventoryPos].m_Type == MAKE_ITEMNUM(14, 64) )
	{
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) (%s)(%s) to Delete Relics Item [Serial:%u]"
	#else
		LogAddTD("[Illusion Temple] (%d) (%s)(%s) to Delete Relics Item [Serial:%d]"
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			, m_nTempleNumber+1, lpObj->AccountID, lpObj->Name, lpObj->pInventory[m_btInventoryPos].m_Number);

		gObjInventoryDeleteItem(lpObj->m_Index, m_btInventoryPos);				
		GCInventoryItemDeleteSend(lpObj->m_Index, m_btInventoryPos, 0);
	}
}

// 인벤토리에서 아이템을 드랍
void CIllusionTempleProcess::DropRelicsItem(int iIndex)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(m_nRelicsUserIndex != iIndex)
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	
	if( m_btInventoryPos == -1 )
		return;

	if( lpObj->pInventory[m_btInventoryPos].m_Type != MAKE_ITEMNUM(14, 64) )
	{
		return;
	}
	
	int iItemPos = m_btInventoryPos;

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof( pResult ));
	pResult.Result		= 0x01;
	pResult.Ipos		= iItemPos;		

	int map_num = gObj[iIndex].MapNumber;	
	int type    = gObj[iIndex].pInventory[iItemPos].m_Type;
	int level   = gObj[iIndex].pInventory[iItemPos].m_Level;
	float dur   = gObj[iIndex].pInventory[iItemPos].m_Durability;
	int ret     = gObj[iIndex].pInventory[iItemPos].IsItem();
	BYTE Option1= gObj[iIndex].pInventory[iItemPos].m_Option1;
	BYTE Option2= gObj[iIndex].pInventory[iItemPos].m_Option2;
	BYTE Option3= gObj[iIndex].pInventory[iItemPos].m_Option3;
	BYTE NOption= gObj[iIndex].pInventory[iItemPos].m_NewOption;
	DWORD s_num = gObj[iIndex].pInventory[iItemPos].m_Number;
	
	BYTE ItemExOption	= g_kJewelOfHarmonySystem.GetItemStrengthenOption( &gObj[iIndex].pInventory[iItemPos] );
	BYTE ItemExLevel	= g_kJewelOfHarmonySystem.GetItemOptionLevel( &gObj[iIndex].pInventory[iItemPos] );
	
	
	BYTE NewOption[8];
	ItemIsBufExOption(NewOption, &gObj[iIndex].pInventory[iItemPos]);

	int	 PetLevel = gObj[iIndex].pInventory[iItemPos].m_PetItem_Level;
	int	 PetExp = 	gObj[iIndex].pInventory[iItemPos].m_PetItem_Exp;

	BYTE SOption= gObj[iIndex].pInventory[iItemPos].m_SetOption;
	BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx;

	DWORD item_number = gObj[iIndex].pInventory[iItemPos].m_Number;
#ifdef ADD_LOG_ILLUSION_TEMPLE_1_20070727
	char szItemName[50]="저주받은성물";
#else 
	char szItemName[50]="Relics";
#endif
	
	int aAntiLootIndex = -1;					// 이렇게 떨구면 누구라도 집을 수 있다.

	short X = 0;
	short Y = 0;

	BYTE attr = MapC[map_num].GetAttr(gObj[iIndex].X, gObj[iIndex].Y);
	if( ((attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) ||
		((attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW) )
	{
		// 드랍할 수 없는 곳이면 맵의 중간에 떨군다.
		X = 168;
		Y = 85;
	}
	else
	{
		X = gObj[iIndex].X;
		Y = gObj[iIndex].Y;
	}

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	if( MapC[map_num].ItemDrop(type, level, dur, X, Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx, NULL, 255 ) == TRUE )
#else
	if( MapC[map_num].ItemDrop(type, level, dur, X, Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx) == TRUE )
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	{  
		// 아이템 버리기에 성공하면..
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) (%s)(%s) to Drop Relics Item [Serial:%u]"
#else
		LogAddTD("[Illusion Temple] (%d) (%s)(%s) to Drop Relics Item [Serial:%d]"
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			, m_nTempleNumber+1, lpObj->AccountID, lpObj->Name, lpObj->pInventory[m_btInventoryPos].m_Number);

		gObjInventoryDeleteItem(iIndex, iItemPos);	
		pResult.Result = 0x01;

		LogAddTD(lMsg.Get(479),
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			gObj[iIndex].MapNumber, 
			gObj[iIndex].X, 
			gObj[iIndex].Y, 
			s_num, 
			szItemName, 
			type, 
			level, 
			Option1, 
			Option2, 
			Option3,
			(int)dur,
			NewOption[0],
			NewOption[1],
			NewOption[2],
			NewOption[3],
			NewOption[4],
			NewOption[5],
			NewOption[6],
			SOption,
			ItemEffectEx>>7,
			ItemExOption,
			ItemExLevel
			);

	}
	else
	{
		// 버릴수 없다.
		pResult.Result = 0x00;
	}

	m_nRelicsUserIndex	= -1;
	m_btInventoryPos	= -1;
	m_btRelicsTeam		= 0xFF;
			
	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
}

void CIllusionTempleProcess::SetInventoryPos(int aIndex, BYTE btPos)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	m_nRelicsUserIndex	= aIndex;		// 성물을 가지고 있는 사용자 인덱스
	m_btInventoryPos	= btPos;		// 성물이 들어있는 인벤토리 위치

	// 팀 추가
	if(gObj[aIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_nIndex != -1 
			&& m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_nIndex ==  aIndex)
		{
			m_btRelicsTeam = m_UserData[gObj[aIndex].m_iIllusionTempleIndex].m_btTeam;	// 성물을 가지고 있는 팀
		}

	}
}

// 성물을 집은사람을 사용자들에게 전송
void CIllusionTempleProcess::SendRelicsUser(LPOBJECTSTRUCT lpObj)
{
	PMSG_RELICS_GET_USER pRelicsUser;
	PHeadSubSetB((LPBYTE)&pRelicsUser, 0xBF, 0x08, sizeof(pRelicsUser));
	pRelicsUser.wUserIndex = lpObj->m_Index;
	memcpy(pRelicsUser.Name,    lpObj->Name, MAX_IDSTRING);	

	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			DataSend(m_UserData[i].m_nIndex, (LPBYTE)&pRelicsUser, pRelicsUser.h.size);
		}
	}	
}

// 다음 상태로 넘어가기위한 남은 시간을 설정
void CIllusionTempleProcess::SetRemainTime()
{
	m_nRemainTimeMin	= 0;	

	// 1초후 다음 상태로 바꿈
	m_nRemainTimeMSec	= 1 * 1000;	
}

// NPC 위치테이블 정보 추가
void CIllusionTempleProcess::AddNpcPosNum(int nClass, int nPosNum)
{
	if(nClass == NPC_ILLUSION_HOLY_RELICS_STATUE)			// 석상
	{
		if(m_nStatusCount > MAX_ILLUSION_TEMPLE_STATUS_COUNT)
			return;

		m_StatusData[m_nStatusCount].m_nIndex	= -1;
		m_StatusData[m_nStatusCount].m_wPosNum	= nPosNum;

		m_nStatusCount++;
	}
	else if(nClass == NPC_HOLY_RELICS_BOX_ALLIED)		// 성물 보관함 (연합군)
	{
		m_RelicsBoxAlliedData.m_nIndex		= -1;
		m_RelicsBoxAlliedData.m_wPosNum		= nPosNum;
	}
	else if(nClass == NPC_HOLY_RELICS_BOX_ILLUSION)		// 성물 보관함 (환영교)
	{
		m_RelicsBoxIllusionData.m_nIndex	= -1;
		m_RelicsBoxIllusionData.m_wPosNum	= nPosNum;
	}
}

// Monster 위치 테이블 정보 추가
void CIllusionTempleProcess::AddMonsterPosNum(int nClass, int nPosNum)
{
	if(m_nMonsterCount > MAX_ILLUSION_TEMPLE_MONSTER_COUNT)
		return;

	m_MonsterData[m_nMonsterCount].m_nIndex		= -1;
	m_MonsterData[m_nMonsterCount].m_wPosNum	= nPosNum;

	m_nMonsterCount++;
}

// NPC, 몬스터의 위치정보를 리셋
void CIllusionTempleProcess::ResetPosNum()
{
	// 석상
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_STATUS_COUNT; i++)
	{
		m_StatusData[i].m_nIndex		= -1;
		m_StatusData[i].m_wPosNum		= -1;
	}

	m_nStatusCount = 0;

	// 성물 보관함
	m_RelicsBoxAlliedData.m_nIndex		= -1;
	m_RelicsBoxAlliedData.m_wPosNum		= -1;

	m_RelicsBoxIllusionData.m_nIndex	= -1;
	m_RelicsBoxIllusionData.m_wPosNum	= -1;

	// 몬스터
	for( int i = 0; i < MAX_ILLUSION_TEMPLE_MONSTER_COUNT; i++)
	{
		m_MonsterData[i].m_nIndex		= -1;
		m_MonsterData[i].m_wPosNum		= -1;
	}

	m_nMonsterCount = 0;
}

// 석상 추가
void CIllusionTempleProcess::AddNpcStatus()
{
	// 석상 NPC 생성
	for(int i = 0; i < m_nStatusCount; i++)
	{
		int result = gObjAddMonster(m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1);

		if( result >= 0 )
		{
			gObjSetPosMonster(result, m_StatusData[i].m_wPosNum);
			gObjSetMonster(result, NPC_ILLUSION_HOLY_RELICS_STATUE);
			
			// 추가만 하고 보이진 않음
			gObj[result].m_PosNum	= -1;
			gObj[result].Live		= 0;
			gObj[result].DieRegen	= 2;
			gObj[result].m_State	= OBJST_DYING;
			gObj[result].RegenTime	= GetTickCount();

			m_StatusData[i].m_nIndex = result;

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
			LogAddTD("[Illusion Temple] (%d) Create NPC Status Name: %s, (%d: %d/%d)", 
				m_nTempleNumber+1, gObj[result].Name, gObj[result].MapNumber, gObj[result].X, gObj[result].Y); 
#endif
		}
		else
		{
			//error
			LogAddTD("[Illusion Temple] (%d) Add Status Error", m_nTempleNumber+1);
		}
	}
}

// 성물 보관함을 추가
void CIllusionTempleProcess::AddNpcRelicsBox()
{
	// 보관함 NPC 생성
	int result = gObjAddMonster(m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1);

	if( result >= 0 )
	{
		gObjSetPosMonster(result, m_RelicsBoxAlliedData.m_wPosNum);
		gObjSetMonster(result, NPC_HOLY_RELICS_BOX_ALLIED);
		
		gObj[result].Dir		= 1;
		gObj[result].m_PosNum	= -1;
		gObj[result].Live		= 1;
		gObj[result].DieRegen	= 0;
		gObj[result].m_State	= OBJST_CREATE;

		m_RelicsBoxAlliedData.m_nIndex = result;

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
			LogAddTD("[Illusion Temple] (%d) Create NPC AlliedRelicsBox Name: %s, (%d: %d/%d)", 
				m_nTempleNumber+1, gObj[result].Name, gObj[result].MapNumber, gObj[result].X, gObj[result].Y); 
#endif
	}
	else
	{
		// error
		LogAddTD("[Illusion Temple] (%d) Add AlliedRelicsBox Error", m_nTempleNumber+1);
	}
	
	result = gObjAddMonster(m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1);

	if( result >= 0 )
	{
		gObjSetPosMonster(result, m_RelicsBoxIllusionData.m_wPosNum);
		gObjSetMonster(result, NPC_HOLY_RELICS_BOX_ILLUSION);
		
		gObj[result].Dir		= 1;
		gObj[result].m_PosNum	= -1;
		gObj[result].Live		= 1;
		gObj[result].DieRegen	= 0;
		gObj[result].m_State	= OBJST_CREATE;

		m_RelicsBoxIllusionData.m_nIndex = result;

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
			LogAddTD("[Illusion Temple] (%d) Create NPC IllusionRelicsBox Name: %s, (%d: %d/%d)", 
				m_nTempleNumber+1, gObj[result].Name, gObj[result].MapNumber, gObj[result].X, gObj[result].Y); 
#endif
	}
	else
	{
		// error
		LogAddTD("[Illusion Temple] (%d) Add IllusionRelicsBox Error", m_nTempleNumber+1);
	}
}

// 몬스터 추가
void CIllusionTempleProcess::AddMonsterGhost()
{
	// 몬스터 생성
	for(int i = 0; i < m_nMonsterCount; i++)
	{
		int result = gObjAddMonster(m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1);

		if( result >= 0 )
		{
			gObjSetPosMonster(result, m_MonsterData[i].m_wPosNum);
			gObjSetMonster(result, gMSetBase.m_Mp[m_MonsterData[i].m_wPosNum].m_Type);
			
			// 추가만 하고 보이진 않음
			gObj[result].Dir		= 1;
			gObj[result].m_PosNum	= -1;
			gObj[result].Live		= 0;
			gObj[result].DieRegen	= 0;
			gObj[result].m_State	= OBJST_DYING;
			gObj[result].RegenTime	= GetTickCount();

			m_MonsterData[i].m_nIndex = result;
		}
		else
		{
			//error
			LogAddTD("[Illusion Temple] (%d) Add Monster Error", m_nTempleNumber+1);
		}
	}
}

// 모든 오브젝트를 제거
void CIllusionTempleProcess::RemoveAllObj()
{
	// 석상제거
	for(int i = 0; i < m_nStatusCount; i++)
	{
		if(m_StatusData[i].m_nIndex != -1)
		{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
			LogAddTD("[Illusion Temple] (%d) RemoveStatus Name: %s, (%d: %d/%d)", 
				m_nTempleNumber+1, gObj[m_StatusData[i].m_nIndex].Name, 
				gObj[m_StatusData[i].m_nIndex].MapNumber, 
				gObj[m_StatusData[i].m_nIndex].X, gObj[m_StatusData[i].m_nIndex].Y); 
#endif

			gObjDel(m_StatusData[i].m_nIndex);
			m_StatusData[i].m_nIndex = -1;
		}
	}

	// 보관함 제거
	if(m_RelicsBoxAlliedData.m_nIndex != -1)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
		LogAddTD("[Illusion Temple] (%d) RemoveAlliedRelicsBox Name: %s, (%d: %d/%d)", 
			m_nTempleNumber+1, gObj[m_RelicsBoxAlliedData.m_nIndex].Name, 
			gObj[m_RelicsBoxAlliedData.m_nIndex].MapNumber, 
			gObj[m_RelicsBoxAlliedData.m_nIndex].X, gObj[m_RelicsBoxAlliedData.m_nIndex].Y); 
#endif

		gObjDel(m_RelicsBoxAlliedData.m_nIndex);
		m_RelicsBoxAlliedData.m_nIndex = -1;
	}

	if(m_RelicsBoxIllusionData.m_nIndex != -1)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
		LogAddTD("[Illusion Temple] (%d) RemoveIllusionRelicsBox Name: %s, (%d: %d/%d)", 
			m_nTempleNumber+1, gObj[m_RelicsBoxIllusionData.m_nIndex].Name, 
			gObj[m_RelicsBoxIllusionData.m_nIndex].MapNumber, 
			gObj[m_RelicsBoxIllusionData.m_nIndex].X, gObj[m_RelicsBoxIllusionData.m_nIndex].Y); 
#endif

		gObjDel(m_RelicsBoxIllusionData.m_nIndex);
		m_RelicsBoxIllusionData.m_nIndex = -1;
	}

	// 몬스터 제거
	for( int i = 0; i < m_nMonsterCount; i++)
	{
		if(m_MonsterData[i].m_nIndex != -1)
		{
			gObjDel(m_MonsterData[i].m_nIndex);
			m_MonsterData[i].m_nIndex = -1;
		}
	}

	LogAddTD("[Illusion Temple] (%d) Remove All Object", m_nTempleNumber+1);
}

// 석상 리젠시간 설정
void CIllusionTempleProcess::SetStatusRegenTime()
{
	m_StatusRegenTime = GetTickCount() + 5 * 1000;		// 5초 후 리젠
}

// 석상중 하나를 리젠시킨다.
void CIllusionTempleProcess::RegenStatus()
{
	// 석상이 존재하는지 조사

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013	// 성물을 들고 있는 사용자가 있으면 석상 리젠 안함
	// 성물을 들고있는 사용자가 있으면 리젠안함
	if(m_nRelicsUserIndex >= 0)
	{
		return;
	}
#endif
	
	int nPos = rand() % m_nStatusCount;

	if(m_StatusData[nPos].m_nIndex == -1)
		return;

	LPOBJECTSTRUCT lpObj = &gObj[m_StatusData[nPos].m_nIndex];
	
	lpObj->m_PosNum		= m_StatusData[nPos].m_wPosNum;
	lpObj->DieRegen		= 2;

	m_StatusRegenTime	= -1;

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
	LogAddTD("[Illusion Temple] (%d) RegenStatus Name: %s, (%d: %d/%d), PosNum(%d), connected:%d", 
				m_nTempleNumber+1, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, lpObj->m_PosNum, lpObj->Connected); 
#else
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
			LogAddTD("[Illusion Temple] (%d) RegenStatus Name: %s, (%d: %d/%d)", 
				m_nTempleNumber+1, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y); 
#endif
#endif
}

// 몬스터를 리젠시킨다.( 리젠되어 있는 몬스터 포함 MAX_ILLUSION_TEMPLE_MONSTER_COUNT 만큼)
void CIllusionTempleProcess::RegenMonster()
{
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_MONSTER_COUNT; i++)
	{
		if(m_MonsterData[i].m_nIndex != -1 && gObj[m_MonsterData[i].m_nIndex].m_PosNum == -1)
		{
			gObj[m_MonsterData[i].m_nIndex].m_PosNum		= m_MonsterData[i].m_wPosNum;
			gObj[m_MonsterData[i].m_nIndex].DieRegen		= 2;
		}
	}
}

void CIllusionTempleProcess::SendNoticeMessage(char* lpMsg)
{
	PMSG_NOTICE	pNotice;

	TNotice::MakeNoticeMsg( &pNotice, 0, lpMsg );

	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			DataSend(m_UserData[i].m_nIndex, (LPBYTE)&pNotice, pNotice.h.size);
		}
	}
}

// 현제 게임의 진행 상태/ 시간 정보 / 파티원 위치를 전송
void CIllusionTempleProcess::SendIllusionTemplePlayState(BYTE btTeam)
{
	// 게임 진행 상황을 보냄
	int nOffSet = 0;
	BYTE SendByte[256];

	PMSG_ILLUSION_TEMPLE_PLAY_STATE pMsg;
	
	if(m_nRemainTimeMSec < 0)
	{
		m_nRemainTimeMSec = 0;
	}

	pMsg.wRemainSec		= m_nRemainTimeMSec / 1000;
	if(m_nRelicsUserIndex != -1)
	{
		pMsg.wUserIndex		= m_nRelicsUserIndex;
		
		pMsg.btX			= gObj[m_nRelicsUserIndex].X;
		pMsg.btY			= gObj[m_nRelicsUserIndex].Y;
	}
	else
	{
		pMsg.wUserIndex		= 0xFFFF;
		
		pMsg.btX			= 0xFF;
		pMsg.btY			= 0xFF;
	}
	
	pMsg.btAlliedPoint		= m_btAlliedPoint;
	pMsg.btIllusionPoint	= m_btIllusionPoint;
	pMsg.btMyTeam			= btTeam;

	nOffSet += sizeof(pMsg);

	int tmpCount = 0;	// 인원수 변경에 대한 확인
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		if(m_UserData[i].m_nIndex != -1 && m_UserData[i].m_btTeam == btTeam && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			PMSG_ILLUSION_TEMPLE_PARTY_POS	pPartyPos;
			pPartyPos.wPartyUserIndex	= m_UserData[i].m_nIndex;
			pPartyPos.byMapNumber		= gObj[m_UserData[i].m_nIndex].MapNumber;
			pPartyPos.btX				= gObj[m_UserData[i].m_nIndex].X;
			pPartyPos.btY				= gObj[m_UserData[i].m_nIndex].Y;
		
			tmpCount++;

			memcpy(SendByte + nOffSet, (char*)&pPartyPos, sizeof(pPartyPos));
			nOffSet += sizeof(pPartyPos);	
		}
	}

	if(btTeam == TEAM_ALLIED)
	{
		if(m_nAlliedUserCount != tmpCount)
		{
			// 에러 확인
			m_nAlliedUserCount = tmpCount;	
		}
		pMsg.btPartyCount = m_nAlliedUserCount;
	}
	else
	{
		if(m_nIllusionUserCount != tmpCount)
		{
			// 에러 확인
			m_nIllusionUserCount = tmpCount;	
		}
		pMsg.btPartyCount		= m_nIllusionUserCount;
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x01, nOffSet);

	memcpy(SendByte, (char*)&pMsg, sizeof(pMsg));
	
	for ( int i = 0 ; i < MAX_ILLUSION_TEMPLE_USER_COUNT ; i++) 
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2 && m_UserData[i].m_btTeam == btTeam)
		{
			DataSend(m_UserData[i].m_nIndex, (LPBYTE)&SendByte , pMsg.h.size);
		}
	}
}

// 현재 이벤트맵의 상태
void CIllusionTempleProcess::SendIllusionTempleState(BYTE btTempleState, int nIndex /* = -1*/)
{
	PMSG_ILLUSION_TEMPLE_STATE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));
	pMsg.btTempleNumber			= m_nTempleNumber;
	pMsg.btIllusionTempleState	= btTempleState;

	if( nIndex != -1)
	{
		DataSend(nIndex, (LPBYTE)&pMsg , pMsg.h.size);
	}
	else
	{
		for (int i = 0 ; i < MAX_ILLUSION_TEMPLE_USER_COUNT ; i++) 
		{
			if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
			{
				DataSend(m_UserData[i].m_nIndex, (LPBYTE)&pMsg , pMsg.h.size);
			}
		}
	}
}


BYTE CIllusionTempleProcess::GetUserTeam(int nIndex)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return 0xFF;
	}
	
	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex
			|| gObj[nIndex].Connected > 2)
		{
			return m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btTeam;
		}
	}

	return 0xFF;
}

// 스킬 포인트 증가
BYTE CIllusionTempleProcess::IncSkillPoint(int nIndex, BYTE btIncPoint)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return 0;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex
			|| gObj[nIndex].Connected > 2)
		{
			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint += btIncPoint;
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint > MAX_ILLUSION_TEMPLE_SKILL_POINT)
			{
				m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint = MAX_ILLUSION_TEMPLE_SKILL_POINT;
			}

			return m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint;
		}
	}
	return 0;	
}

// 스킬 포인트 감소
BYTE CIllusionTempleProcess::DecSkillPoint(int nIndex, BYTE btDecPoint)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return 0;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex
			|| gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint >= btDecPoint)
			{
				m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint -= btDecPoint;
				return m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint;
			}
		}
	}
	return 0;
}

// 스킬 포인트를 얻음
BYTE CIllusionTempleProcess::GetSkillPoint(int nIndex)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return 0;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex
			|| gObj[nIndex].Connected > 2)
		{
			return m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint;
		}
	}
	return 0;
}

// 스킬 포인트 설정
BOOL CIllusionTempleProcess::SetSkillPoint(int nIndex, BYTE btSkillPoint)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return FALSE;
	}
	
	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex
			|| gObj[nIndex].Connected > 2)
		{
			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btSkillPoint = btSkillPoint;
			return TRUE;
		}
	}

	return FALSE;
}

// 스킬 포인트 리셋
void CIllusionTempleProcess::ResetSkillPoint()
{
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		m_UserData[i].m_btSkillPoint = DEFAULT_SKILL_POINT;
	}
}

// 사용자 킬카운트 증가
void CIllusionTempleProcess::IncUserKillCount(int nIndex)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex
			|| gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btUserKillCount > MAX_ILLUSION_TEMPLE_KILL_COUNT)
			{
				return;
			}
			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btUserKillCount++;
		}
	}
}

// 몬스터 킬카운트 증가
void CIllusionTempleProcess::IncMonsterKillCount(int nIndex)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex
			|| gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btMonsterKillCount > MAX_ILLUSION_TEMPLE_KILL_COUNT)
			{
				return;
			}
			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btMonsterKillCount++;
		}
	}
}

// 킬카운트 리셋
void CIllusionTempleProcess::ResetKillCount()
{
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		m_UserData[i].m_btUserKillCount		= 0;
		m_UserData[i].m_btMonsterKillCount	= 0;
	}
}

// 성물 운반수 증가
void CIllusionTempleProcess::IncRelicsMoveCount(int nIndex)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex
			|| gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btRelicsMoveCount > MAX_ILLUSION_TEMPLE_RELICE_MOVE_COUNT)
			{
				return;
			}
			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btRelicsMoveCount++;
		}
	}
}

// 성물 운반수 리셋
void CIllusionTempleProcess::ResetRelicsMoveCount()
{
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		m_UserData[i].m_btRelicsMoveCount		= 0;
	}
}

// 스킬 사용 횟수 증가
void CIllusionTempleProcess::IncUseSkillCount(int nIndex)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if(gObj[nIndex].m_iIllusionTempleIndex != -1)
	{
		if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex == nIndex
			|| gObj[nIndex].Connected > 2)
		{
			if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btUseSkillCount > MAX_ILLUSION_TEMPLE_USE_SKILL_COUNT)
			{
				return;
			}
			m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btUseSkillCount++;
		}
	}
}

// 스킬 사용 횟수 리셋
void CIllusionTempleProcess::ResetUseSkillCount()
{
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		m_UserData[i].m_btUseSkillCount		= 0;
	}
}

// 전용 스킬 사용
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
void CIllusionTempleProcess::IllusionTempleUseSkill(int nIndex, WORD btSkillNumber, int nTargetIndex, BYTE btDis)
#else
void CIllusionTempleProcess::IllusionTempleUseSkill(int nIndex, BYTE btSkillNumber, int nTargetIndex, BYTE btDis)
#endif
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	if (!CHECK_LIMIT(nTargetIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	LPOBJECTSTRUCT	lpObj		= &gObj[nIndex];
	LPOBJECTSTRUCT	lpTargetObj = &gObj[nTargetIndex];

	if(GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_PLAYING)
	{
		// 스킬 결과 전송
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);	// 실패	
		return;
	}

	if(lpObj->Connected <= 2 || lpTargetObj->Connected <= 2)
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
	{
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);	// 실패	
		return;
	}
#else
		return;
#endif

	if(lpObj->MapNumber != lpTargetObj->MapNumber)
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
	{
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);	// 실패	
		return;
	}
#else
		return;
#endif

	if(lpObj->m_iIllusionTempleIndex == -1 || lpTargetObj->m_iIllusionTempleIndex == -1)
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
	{
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);	// 실패	
		return;
	}
#else
		return;
#endif

	int		nResultCount	= -1;
	BOOL	ret = TRUE;
	switch(btSkillNumber)
	{
	case AT_SKILL_PRODECTION:	// 보호의 주문
		{
			nResultCount = MagicDamageC.CheckKillCount(AT_SKILL_PRODECTION, GetSkillPoint(lpObj->m_Index));
			
			if(nResultCount < 0)
			{
				ret = FALSE;
				LogAddTD("[Illusion Temple] (%d) Use Skill KillCount Error (Skill:%d)", m_nTempleNumber+1, btSkillNumber);
				break;
			}

			ret = UseSkillProdection(lpObj);
		}
		break;
	case AT_SKILL_RESTRAINT:	// 속박의 주문
		{
			// 스킬 포인트 검사
			nResultCount = MagicDamageC.CheckKillCount(AT_SKILL_RESTRAINT, GetSkillPoint(lpObj->m_Index));
			
			if(nResultCount < 0)
			{
				ret = FALSE;
				LogAddTD("[Illusion Temple] (%d) Use Skill KillCount Error (Skill:%d)", m_nTempleNumber+1, btSkillNumber);
				break;
			}

			ret = UseSkillRestraint(lpObj, lpTargetObj);
		}
		break;
	case AT_SKILL_CALL:			// 소환의 주문
		{
			nResultCount = MagicDamageC.CheckKillCount(AT_SKILL_CALL, GetSkillPoint(lpObj->m_Index));
			
			if(nResultCount < 0)
			{
				ret = FALSE;
				LogAddTD("[Illusion Temple] (%d) Use Skill KillCount Error (Skill:%d)", m_nTempleNumber+1, btSkillNumber);
				break;
			}

			ret = UseSkillTeleport(lpObj);			
		}
		break;
	case AT_SKILL_SHIELD_BURN:		// 쉴드 번
		{
			nResultCount = MagicDamageC.CheckKillCount(AT_SKILL_SHIELD_BURN, GetSkillPoint(lpObj->m_Index));

			if(nResultCount < 0)
			{
				ret = FALSE;
				LogAddTD("[Illusion Temple] (%d) Use Skill KillCount Error (Skill:%d)", m_nTempleNumber+1, btSkillNumber);
				break;
			}

			ret = UseSkillShieldBurn(lpObj, lpTargetObj);
		}
		break;
	default:
		ret = FALSE;
		break;
	}

	if(ret == TRUE)
	{
		BOOL result = SetSkillPoint(nIndex, nResultCount);
		GCIllusionTempleSkillPoint(nIndex, nResultCount);

		// 스킬 사용횟수 증가
		IncUseSkillCount(nIndex);

		LogAddTD("[Illusion Temple] (%d) Use Skill (%d), (%s)(%s), (%d)",
			m_nTempleNumber+1, btSkillNumber, lpObj->AccountID, lpObj->Name, nResultCount);
	}

	if( btSkillNumber != AT_SKILL_CALL && ret == TRUE)
	{
		// 스킬 결과 전송
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 1);	// 성공
	}
	else if(ret == FALSE)
	{
		// 스킬 결과 전송
		SendUseSkillResult(lpObj->m_Index, lpTargetObj->m_Index, btSkillNumber, 0);	// 실패	
	}
}

// 스킬 사용시 처리
void CIllusionTempleProcess::IllusionTempleSkillProc(LPOBJECTSTRUCT lpObj)
{
	if( m_UserData[lpObj->m_iIllusionTempleIndex].m_nIndex != lpObj->m_Index)
		return;

	// 보호의 주문 
	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime > 0)
	{
		m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime--;

		if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime <= 0)
		{
			m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime = 0;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_TEMPLE_PROTECTION );
#else
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ILLUSION_TEMPLE_PRODECTION );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

			SendIllusionTempleSkillEnd(lpObj, AT_SKILL_PRODECTION);
		}
	}

	// 속박의 주문
	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime > 0)
	{
		m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime--;

		if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime <= 0)
		{
			m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime = 0;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_RESTRICTION );
#else
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ILLUSION_TEMPLE_RESTRAINT );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

			SendIllusionTempleSkillEnd(lpObj, AT_SKILL_RESTRAINT);
		}
	}
}

//----------------------------------------------------------------------------
// GC [0xBF][0x07] 환영 사원에서 전용스킬 종료
//----------------------------------------------------------------------------
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// 스킬번호 BYTE -> WORD
void CIllusionTempleProcess::SendIllusionTempleSkillEnd(LPOBJECTSTRUCT lpObj, WORD btState)
#else
void CIllusionTempleProcess::SendIllusionTempleSkillEnd(LPOBJECTSTRUCT lpObj, BYTE btState)
#endif
{
	PMSG_ILLUSION_TEMPLE_SKILL_END	pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x07, sizeof(pMsg));
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// 스킬번호 BYTE -> WORD
	pMsg.MagicNumberH	= HIBYTE( btState );
	pMsg.MagicNumberL	= LOBYTE( btState );
#else
	pMsg.MagicNumber	= btState;
#endif
	pMsg.wObjIndex		= lpObj->m_Index;

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);		// 자신에게도 보냄
	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

// 보호의 주문 사용
BOOL CIllusionTempleProcess::UseSkillProdection(LPOBJECTSTRUCT lpObj)
{
	m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime	= ILLUSION_TEMPLE_SKILL_PRODECTION_TIME;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpObj, BUFFTYPE_TEMPLE_PROTECTION, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
	lpObj->m_ViewSkillState	|= STATE_ILLUSION_TEMPLE_PRODECTION;

	#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
	ADD_VIEWSKILLSTATE(lpObj->m_ViewSkillState, STATE_ILLUSION_TEMPLE_PRODECTION);
	#else
	lpObj->m_ViewSkillState	|= STATE_ILLUSION_TEMPLE_PRODECTION;
	#endif	

#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	return TRUE;
}

// 속박의 주문 사용
BOOL CIllusionTempleProcess::UseSkillRestraint(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	// 거리 계산
	if(!gCheckSkillDistance(lpObj->m_Index, lpTargetObj->m_Index, AT_SKILL_RESTRAINT))
	{
		return FALSE;
	}

	m_UserData[lpTargetObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime	= ILLUSION_TEMPLE_SKILL_RESTRAINT_TIME;

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
	lpTargetObj->lpAttackObj		= lpObj;
#endif

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_RESTRICTION, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
	#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
	ADD_VIEWSKILLSTATE(lpTargetObj->m_ViewSkillState, STATE_ILLUSION_TEMPLE_RESTRAINT);
	#else
	lpTargetObj->m_ViewSkillState	|= STATE_ILLUSION_TEMPLE_RESTRAINT;
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	lpTargetObj->PathCount			= 0;
	
	lpTargetObj->PathStartEnd		= 0;
	gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
	
	return TRUE;
}

// 소환의 주문 사용
BOOL CIllusionTempleProcess::UseSkillTeleport(LPOBJECTSTRUCT lpObj)
{
	// 스턴이 걸려 있으면 순간이동 할 수 없다.
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STUN ) == true )
#else
	if( lpObj->m_iSkillStunTime > 0 )
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	{
		// 스킬 결과 전송
		SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, AT_SKILL_CALL, 0);	// 실패
		return FALSE;
	}

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 슬립상태에서는 순간이동을 할 수 없다
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == true )
	{
		// 스킬 결과 전송
		SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, AT_SKILL_CALL, 0);	// 실패
		return FALSE;
	}
#endif
	
	// 공간이동 사용
	int tmpRelicsUserIndex = m_nRelicsUserIndex;
	if(tmpRelicsUserIndex == -1)
	{
		// 스킬 결과 전송
		SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, AT_SKILL_CALL, 0);	// 실패
		return FALSE;
	}

	// 성물을 든사람은 소환스킬 금지
	if(tmpRelicsUserIndex == lpObj->m_Index)
	{
		// 스킬 결과 전송
		SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, AT_SKILL_CALL, 0);	// 실패
		return FALSE;
	}

	// 스킬 결과 전송
	SendUseSkillResult(lpObj->m_Index, lpObj->m_Index, AT_SKILL_CALL, 1);	// 성공

	int addx = 0;
	int addy = 0;
	int count = 50;
	BOOL	Find = FALSE;
	while(count--)
	{
		addx = gObj[tmpRelicsUserIndex].X+(-1+rand()%3);
		addy = gObj[tmpRelicsUserIndex].Y+(-1+rand()%3);
		if( gObjCheckTeleportArea(lpObj->m_Index, addx, addy) )
		{
			Find =TRUE;
			break;
		}
	}
	
	if( !Find )
	{	// 소환 위치를 못 찾았을때는.. 소환자와 겹치게 한다.
		addx = gObj[tmpRelicsUserIndex].X;
		addy = gObj[tmpRelicsUserIndex].Y;
	}
	
	gObjTeleportMagicUse(lpObj->m_Index, addx, addy);

	return TRUE;
}

// 쉴드번 사용
BOOL CIllusionTempleProcess::UseSkillShieldBurn(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	// 거리 계산
	if(!gCheckSkillDistance(lpObj->m_Index, lpTargetObj->m_Index, AT_SKILL_SHIELD_BURN))
	{
		return FALSE;
	}

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
	lpTargetObj->lpAttackObj		= lpObj;
#endif

	if(lpTargetObj->iShield <= 1)
	{
		GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
		return TRUE;
	}

	int nShieldDamage = lpTargetObj->iShield / 2;
	lpTargetObj->iShield = nShieldDamage;
	GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, nShieldDamage);

	return TRUE;
}

// 스킬 사용 결과 전송
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
void CIllusionTempleProcess::SendUseSkillResult(int nIndex, int nTargetIndex,  WORD wSkillNumber, BYTE btResult)
#else
void CIllusionTempleProcess::SendUseSkillResult(int nIndex, int nTargetIndex,  BYTE btSkillNumber, BYTE btResult)
#endif
{
	PMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x02, sizeof(pResult));
	pResult.btResult		= btResult;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	pResult.MagicNumberH		= HIBYTE( wSkillNumber );
	pResult.MagicNumberL		= LOBYTE( wSkillNumber );
#else
	pResult.MagicNumber		= btSkillNumber;
#endif
	pResult.wSourceObjIndex	= nIndex;
	pResult.wTargetObjIndex	= nTargetIndex;
	
	DataSend(nIndex, (LPBYTE)&pResult, pResult.h.size);		// 자신에게도 보냄

	// 성공했을 경우만 보냄
	if(btResult == 1)
	{
		MsgSendV2(&gObj[nIndex], (LPBYTE)&pResult, pResult.h.size);
	}
}

// 사용자가 죽었을 때의 처리
void CIllusionTempleProcess::IllusionTempleUserDie(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->m_iIllusionTempleIndex == -1 || m_UserData[lpObj->m_iIllusionTempleIndex].m_nIndex != lpObj->m_Index)
		return;

	// 성물을 떨굼
	DropRelicsItem(lpObj->m_Index);

	// 스킬포인트 초기화
	m_UserData[lpObj->m_iIllusionTempleIndex].m_btSkillPoint	= DEFAULT_SKILL_POINT;	
	GCIllusionTempleSkillPoint(lpObj->m_Index, DEFAULT_SKILL_POINT);

	// 스킬 버프 초기화
	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime > 0)
	{
		m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillProdectionTime = 0;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjRemoveBuffEffect( lpObj, BUFFTYPE_TEMPLE_PROTECTION );
#else
		DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ILLUSION_TEMPLE_PRODECTION );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		SendIllusionTempleSkillEnd(lpObj, AT_SKILL_PRODECTION);
	}

	// 속박의 주문
	if(m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime > 0)
	{
		m_UserData[lpObj->m_iIllusionTempleIndex].m_dwSkillRestraintTime = 0;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjRemoveBuffEffect( lpObj, BUFFTYPE_RESTRICTION );
#else
		DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ILLUSION_TEMPLE_RESTRAINT );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		SendIllusionTempleSkillEnd(lpObj, AT_SKILL_RESTRAINT);
	}
}

// 게임 결과 계산
void CIllusionTempleProcess::CalCursetTempleResult()
{
	// 현재 남아있는 인원 다시 검사
	CheckUsersOnConnect();

	int nAlliedExp		= 0;
	int nIllusionExp	= 0;

	// 우승팀 결정
	if(m_btAlliedPoint >= ILLUSION_TEMPLE_MIN_WIN_POINT  && m_btAlliedPoint > m_btIllusionPoint)
	{
		// 연합군 승
		m_btWinTeam		= TEAM_ALLIED;

		nAlliedExp = (g_IllusionTemple_Default_Exp[m_nTempleNumber] + 
			(m_btAlliedPoint - m_btIllusionPoint) 
			* g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber]) * 2;

		if(m_btIllusionPoint == 0)
		{
			nIllusionExp = g_IllusionTemple_Default_Exp[m_nTempleNumber];
		}
		else
		{
			nIllusionExp = g_IllusionTemple_Default_Exp[m_nTempleNumber] 
				+ m_btIllusionPoint * g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber];
		}
	}
	else if( m_btIllusionPoint >= ILLUSION_TEMPLE_MIN_WIN_POINT && m_btAlliedPoint < m_btIllusionPoint )
	{
		// 환영교단 승
		m_btWinTeam		= TEAM_ILLUSION;

		nIllusionExp = (g_IllusionTemple_Default_Exp[m_nTempleNumber] + 
			(m_btIllusionPoint - m_btAlliedPoint) 
			* g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber]) * 2;

		if(m_btIllusionPoint == 0)
		{
			nAlliedExp = g_IllusionTemple_Default_Exp[m_nTempleNumber];
		}
		else
		{
			nAlliedExp = g_IllusionTemple_Default_Exp[m_nTempleNumber] 
				+ m_btAlliedPoint * g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber];
		}
	}
	else
	{
		// 둘다 실패
		if(m_btAlliedPoint == 0)
		{
			nAlliedExp = g_IllusionTemple_Default_Exp[m_nTempleNumber];
		}
		else
		{
			nAlliedExp = g_IllusionTemple_Default_Exp[m_nTempleNumber]
				+ m_btAlliedPoint * g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber];
		}

		if(m_btIllusionPoint == 0)
		{
			nIllusionExp = g_IllusionTemple_Default_Exp[m_nTempleNumber];
		}
		else
		{
			nIllusionExp = g_IllusionTemple_Default_Exp[m_nTempleNumber] 
				+ m_btIllusionPoint * g_IllusionTemple_Relics_Point_Exp[m_nTempleNumber];
		}
	}
	
	// 로그수정
	LogAddTD("[Illusion Temple] (%d) Result (WinTeam: %d)(Score:[%d][%d])",	
		m_nTempleNumber+1, m_btWinTeam, 
		m_btAlliedPoint, m_btIllusionPoint);

	//각 캐릭터별 보상경험치 계산
	int i;
	for (i = 0 ; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++) 
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
		if(m_UserData[i].m_btTeam == 0xFF)
			continue;
#endif

#ifdef UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710	// 최소 사용스킬 수정
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2 
			&& gObj[m_UserData[i].m_nIndex].MapNumber == m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1
			&& m_UserData[i].m_btUseSkillCount >= g_btIllusionTempleMinUseSkillCnt)
#else
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2 
			&& gObj[m_UserData[i].m_nIndex].MapNumber == m_nTempleNumber+MAP_INDEX_ILLUSION_TEMPLE1
			&& m_UserData[i].m_btUseSkillCount >= MIN_USE_SKILL_COUNT)
#endif	// UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710
		{
			switch(m_UserData[i].m_btTeam)
			{
			case TEAM_ALLIED:
				m_UserData[i].m_nRewardExp = nAlliedExp;
				break;
			case TEAM_ILLUSION:
				m_UserData[i].m_nRewardExp = nIllusionExp;
				break;
			}

#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			if( gObj[m_UserData[i].m_nIndex].ThirdChangeUp )
				m_UserData[i].m_nRewardExp = m_UserData[i].m_nRewardExp * g_iIllusionTempleRewardPercentMaster / 100;
			else
				m_UserData[i].m_nRewardExp = m_UserData[i].m_nRewardExp * g_iIllusionTempleRewardPercent / 100;
#else
	#ifdef UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710	// 경험치 비율적용
			m_UserData[i].m_nRewardExp = m_UserData[i].m_nRewardExp * g_iIllusionTempleRewardPercent / 100;
	#endif
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
			CheckItemOptForGetExpEx( &gObj[m_UserData[i].m_nIndex], m_UserData[i].m_nRewardExp, TRUE);
#else
//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC 적용 후 아래코드 삭제할 예정
#ifdef MODIFY_EVENTMAP_REWARD_BUGFIX_20080404	// 보상경험치에 유지의 인장만 적용
			if( gObjCheckUsedBuffEffect( &gObj[m_UserData[i].m_nIndex], BUFFTYPE_PCBANG_POINT_MARK3 ))
			{
				m_UserData[i].m_nRewardExp = 0;
			}
#else	// MODIFY_EVENTMAP_REWARD_BUGFIX_20080404	
			// 인장 적용

#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205	
			g_PCBangPointSystem.CheckItemOptForGetExp( &gObj[m_UserData[i].m_nIndex], m_UserData[i].m_nRewardExp );
#else			
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
			CheckItemOptForGetExp(&gObj[m_UserData[i].m_nIndex], m_UserData[i].m_nRewardExp, TRUE);
	#else
			//20080416 - 아래 코드는 삭제해도 될것 같음
			int iExpRate = 0;
			iExpRate = gObjGetTotalValueOfEffect( &gObj[m_UserData[i].m_nIndex], EFFECTTYPE_EXPERIENCE );
			//이벤트 맵에선 인장을 통한 경험치 효과를 적용안함 2008/04/07 riverstyx
			if (iExpRate > 100) iExpRate = 100;
			if( gObjCheckUsedBuffEffect( &gObj[m_UserData[i].m_nIndex], BUFFTYPE_PCBANG_POINT_MARK3 ) == false
				&& iExpRate == 0
				)
			{
				iExpRate = 100;
			}

			m_UserData[i].m_nRewardExp = ( m_UserData[i].m_nRewardExp * iExpRate ) / 100;
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
#else
	#ifdef MODIFY_SET_MARK_EFFECT_AT_ILLUSIONTEMPLE_20071018
			// 인장 아이템 적용
			m_UserData[i].m_nRewardExp = ( ( m_UserData[i].m_nRewardExp * gObj[m_UserData[i].m_nIndex].m_wExprienceRate ) / 100 );
	#endif // MODIFY_SET_MARK_EFFECT_AT_ILLUSIONTEMPLE_20071018
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
#endif	// MODIFY_EVENTMAP_REWARD_BUGFIX_20080404
//<--
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

			// 파티원 수
			int nPartyCount = 0;
			if(gObj[m_UserData[i].m_nIndex].PartyNumber >= 0)
			{
				nPartyCount = gParty.GetCount(gObj[m_UserData[i].m_nIndex].PartyNumber);
			}
			else 
			{
				nPartyCount = 1;
			}
			
			// 랭킹 포인트 계산
			// 랭킹포인트 = 보상경험치 + (플레이어킬 수 * 플레이어킬 포인트) + (성물운반 수 * 성물운 반포인트) + 플레이어 수
			m_UserData[i].m_nRankingPoint = m_UserData[i].m_nRewardExp
				+ (m_UserData[i].m_btUserKillCount * g_IllusionTemple_Add_Kill_Exp[m_nTempleNumber])
				+ (m_UserData[i].m_btRelicsMoveCount * g_IllusionTemple_Move_Relics_Exp[m_nTempleNumber])
				+ g_IllusionTemple_PartyUser_Point[nPartyCount];

			// 경험치 적용
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
			int iCAL_EXP	= m_UserData[i].m_nRewardExp;
			int iRET_EXP	= 0;
			while(iCAL_EXP > 0)
			{
				iRET_EXP	= iCAL_EXP;
				iCAL_EXP	= LevelUp(m_UserData[i].m_nIndex, iCAL_EXP);
			}
#else
			int iRET_EXP  = LevelUp(m_UserData[i].m_nIndex, m_UserData[i].m_nRewardExp);
#endif

#ifdef BUGFIX_GAIN_EXP_20071210
			// *마스터레벨 유저의 경험치는 이미 보내졌다.
			int iUserIndex = m_UserData[i].m_nIndex;
			if( !g_MasterLevelSystem.IsMasterLevelUser( &gObj[iUserIndex] ) )
			{
				GCSendExp_INT64( iUserIndex, 0xFFFF, iRET_EXP, 0, 0 );
			}			
#else
			PMSG_KILLPLAYER_EXT	pkillMsg;
			
			PHeadSetBE((LPBYTE)&pkillMsg, 0x9C, sizeof( pkillMsg));		// 원래 0x16 이었으나 INT로 확장하기 위해 0x9C를 쓴다.
			
			pkillMsg.NumberH	= HIBYTE( 0xFFFF );
			pkillMsg.NumberL	= LOBYTE( 0xFFFF );

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
			pkillMsg.ExpH		= HIWORD( m_UserData[i].m_nRewardExp ) ;
			pkillMsg.ExpL		= LOWORD( m_UserData[i].m_nRewardExp ) ;
#else
			pkillMsg.ExpH		= HIWORD( iRET_EXP ) ;
			pkillMsg.ExpL		= LOWORD( iRET_EXP ) ;
#endif

			pkillMsg.DamageH    = HIBYTE( 0 );
			pkillMsg.DamageL    = LOBYTE( 0 );
			
			// 사용자에게 보상경험치를 보낸다.
			DataSend(m_UserData[i].m_nIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);
#endif	// BUGFIX_GAIN_EXP_20071210
		}
	}

#ifndef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013	// 결과 전송을 함수로 변경
	// 위의 점수를 이용 순위 결정( 정렬은 클라이언트에서.. ^^;)
	// 결과 전송
	int nOffSet = 0;
	BYTE SendByte[256];
	PMSG_ILLUSION_TEMPLE_RESULT	pMsg;

	pMsg.btAlliedPoint		= m_btAlliedPoint;
	pMsg.btIllusionPoint	= m_btIllusionPoint;	
	pMsg.btUserCount		= m_nUserCount;

	nOffSet += sizeof(pMsg);

	for(i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			PMSG_ILLUSION_TEMPLE_USER_ADD_EXP pAddExpMsg;
			memcpy(pAddExpMsg.GameID,    gObj[m_UserData[i].m_nIndex].Name, MAX_IDSTRING);
			pAddExpMsg.btMapNumber	= m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1;
			pAddExpMsg.btTeam		= m_UserData[i].m_btTeam;
			pAddExpMsg.btClass		= gObj[m_UserData[i].m_nIndex].Class;
			pAddExpMsg.nAddExp		= m_UserData[i].m_nRewardExp;
			
			memcpy(SendByte + nOffSet, (char*)&pAddExpMsg, sizeof(pAddExpMsg));
			nOffSet += sizeof(pAddExpMsg);

			// 로그수정
			LogAddTD("[Illusion Temple] (%d) RewardExp Result  (Account:%s, Name:%s, Team:%d, Class:%d, UserKillCount:%d, RelicsMoveCount:%d, RewardExp:%d, RankingPoint: %d)", 
				m_nTempleNumber +1,							// 사원번호
				gObj[m_UserData[i].m_nIndex].AccountID,		// 계정명
				gObj[m_UserData[i].m_nIndex].Name,			// 캐릭터명
				m_UserData[i].m_btTeam,						// 팀
				gObj[m_UserData[i].m_nIndex].DbClass,		// 클래스 0:흑마법사, 1:소울마스터, 16:흑기사, 17:블레이드나이트, 32:요정, 33:뮤즈엘프, 48:마검사, 64:다크로드
				m_UserData[i].m_btUserKillCount,			// 사용자 킬수
				m_UserData[i].m_btRelicsMoveCount,			// 성물 이동 수
				m_UserData[i].m_nRewardExp,					// 보상 경험치
				m_UserData[i].m_nRankingPoint);				// 랭킹 포인트

		}
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x04, nOffSet);

	memcpy(SendByte, (char*)&pMsg, sizeof(pMsg));
	
	for (i = 0 ; i < MAX_ILLUSION_TEMPLE_USER_COUNT ; i++) 
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
		if(m_UserData[i].m_btTeam == 0xFF)
			continue;
#endif
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			DataSend(m_UserData[i].m_nIndex, (LPBYTE)&SendByte , pMsg.h.size);
		}
	}
#endif	// MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013

	// 랭킹 디비에 사원 점수를 저장
	if(g_bSaveIllusionTempleRankingPoint)
	{
		for (i = 0 ; i < MAX_ILLUSION_TEMPLE_USER_COUNT ; i++) 
		{
			if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
			{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
				if(m_UserData[i].m_btTeam == 0xFF)
					continue;
#endif
				// 랭킹 서버로 각각의 점수를 보낸다.
				PMSG_ANS_ILLUSION_TEMPLE_SCORE	pMsgScore = { 0, };
				
				pMsgScore.h.c			= PMHC_BYTE;
				pMsgScore.h.headcode	= 0x26;
				pMsgScore.h.size		= sizeof( pMsgScore );
				
				pMsgScore.nINDEX		= m_UserData[i].m_nIndex;
				
				memcpy(pMsgScore.szAccountID, gObj[m_UserData[i].m_nIndex].AccountID, MAX_IDSTRING);
				pMsgScore.szAccountID[sizeof( pMsgScore.szAccountID )] = 0;
				memcpy(pMsgScore.szGameID,    gObj[m_UserData[i].m_nIndex].Name, MAX_IDSTRING);
				pMsgScore.szGameID[sizeof(pMsgScore.szGameID)] = 0;

				pMsgScore.wServerCode	= gGameServerCode / 20;
				pMsgScore.nTempleNumber	= m_nTempleNumber+1;
				pMsgScore.nClass		= gObj[m_UserData[i].m_nIndex].DbClass;
				pMsgScore.nLevel		= gObj[m_UserData[i].m_nIndex].Level;
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
				if(m_btWinTeam == 0xFF)
				{
					pMsgScore.btWin			= 0;	// 실패
				}
				else
				{
					pMsgScore.btWin			= (m_UserData[i].m_btTeam == m_btWinTeam ? 1 : 0);
				}
#else
				pMsgScore.btWin			= (m_UserData[i].m_btTeam == m_btWinTeam ? 1 : 0);
#endif
				pMsgScore.nRankPoint	= m_UserData[i].m_nRankingPoint;
#ifdef PCBANG_ADVANTAGE
				pMsgScore.nPCBangGuid	= gObj[m_UserData[i].m_nIndex].m_bPCBangUser;
#endif // PCBANG_ADVANTAGE
				
				// 실제로 랭킹서버에 전송하는 부분
				DataSendEventChip( ( char* )&pMsgScore, sizeof( pMsgScore ) );
			}
		}
	}	
}

// 승리한팀원에게 혼석을 드랍
void CIllusionTempleProcess::DropChaosGem(int nIndex)
{
	if (!CHECK_LIMIT(nIndex, MAX_OBJECT))
		return;

	if( m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_nIndex != gObj[nIndex].m_Index)
		return;

	if( GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_END)
		return;

	if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btTeam == 0xFF)
		return;

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
	if(m_btWinTeam == 0xFF)
		return;
#endif
	
	// 승리팀인지 검사
	if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_btTeam != m_btWinTeam)
		return;

	// 보상을 이미 받았는지 검사
	if(m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_bReward == TRUE)
		return;

	// 보상
	m_UserData[gObj[nIndex].m_iIllusionTempleIndex].m_bReward = TRUE;

	int iMaxHitUser = nIndex;
	int iType = ItemGetNumberMake(12, 15);	// 혼석
	ItemSerialCreateSend(gObj[nIndex].m_Index, gObj[nIndex].MapNumber , (BYTE)gObj[nIndex].X, (BYTE)gObj[nIndex].Y, iType, 0, (BYTE)0, 0, 0, 0, iMaxHitUser);

}

// 인벤토리에서 피의 두루마리를 찾음
int CIllusionTempleProcess::SearchScollOfBloodItem(int iIndex, float ScrollDur)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	int iFreeTicketIndex = -1;
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011

	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 51) 
				&& gObj[iIndex].pInventory[x].m_Level == m_nTempleNumber + 1 
				&& gObj[iIndex].pInventory[x].m_Durability == ScrollDur) 		
			{
				return x;
			}
#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM( 13, 61 )
				&& gObj[iIndex].pInventory[x].m_Durability > 0
			  )
			{
				iFreeTicketIndex = x;
			}
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
		}
	}

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	if( iFreeTicketIndex >= 0 )
	{
		return iFreeTicketIndex;
	}
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011

	return -1;
}

#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
int CIllusionTempleProcess::SearchTicketItem(int iIndex)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}
	
	int iFreeTicketIndex = -1;
	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
	{
		CItem& item = gObj[iIndex].pInventory[x];
		if( item.IsItem() == TRUE )
		{
			//if( item.m_Type == MAKE_ITEMNUM(13, 51) 
			//&& item.m_Level == (m_nTempleNumber + 1) && item.m_Durability > 0 ) 
			if( item.m_Type == MAKE_ITEMNUM(13, 51) && item.m_Level == (m_nTempleNumber + 1) )
			{
				if (item.m_Durability > 0) {
					iFreeTicketIndex = x;
					break;
				} else {
					gObjInventoryDeleteItem(iIndex, x);
					GCInventoryItemDeleteSend(iIndex, x, 0);
				} 
			}
			
#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
			if( item.m_Type == MAKE_ITEMNUM( 13, 61 ) && item.m_Durability > 0 )
			{
				if (item.m_Durability > 0) {
					iFreeTicketIndex = x;
					break;
				} else {
					gObjInventoryDeleteItem(iIndex, x);
					GCInventoryItemDeleteSend(iIndex, x, 0);
				}
			}
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
		}
	}
	
	return iFreeTicketIndex;
}
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617

// 몬스터가 죽었을때 아이템 드랍
void CIllusionTempleProcess::MonsterDieItemDrop(LPOBJECTSTRUCT lpObj)
{
	if( m_UserData[lpObj->m_iIllusionTempleIndex].m_nIndex != lpObj->m_Index)
		return;

	switch(lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1)
	{
	case 0:
	case 1:	
		IllusionTempleGhostItemBagA->DropIllusionTempleGhostItem(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y);
		break;
	case 2:
	case 3:
		IllusionTempleGhostItemBagB->DropIllusionTempleGhostItem(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y);
		break;
	case 4:
	case 5:
		IllusionTempleGhostItemBagC->DropIllusionTempleGhostItem(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y);
		break;
	}
}

// 접속해있는 모든 사용자들에게 메시지를 보낸다.
void CIllusionTempleProcess::SendAllUserAnyMsg(LPBYTE lpMsg, int iSize)						
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}

int	CIllusionTempleProcess::LevelUp (int iIndex, int iAddExp)
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 0;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 환영사원 MasterLevelUp 처리
	// 마스터레벨은 이쪽에서.
	if( g_MasterLevelSystem.MasterLevelUp( &gObj[iIndex], iAddExp, true ) )
		return 0;

	// 펫 아이템 경험치 분배	
	gObjSetExpPetItem( iIndex, iAddExp );
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912

	INT iLEFT_EXP = 0;

	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %u %d ", 
		gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].X,
		gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level, gObj[iIndex].Experience, iAddExp);

	// 최대 레벨을 MAX_LEVEL로 한다.
	if( gObj[iIndex].Level >= g_GlobalConfig.m_iMaxLevel )
	{
		GCServerMsgStringSend(lMsg.Get(1136), gObj[iIndex].m_Index, 1);
		return 0;
	}

	if( gObj[iIndex].Experience + iAddExp < gObj[iIndex].NextExp ) {
		gObj[iIndex].Experience += iAddExp;
		
#ifndef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
		// 사용안함
		iLEFT_EXP = iAddExp;
#endif
	}
	else {
		iLEFT_EXP = gObj[iIndex].Experience + iAddExp - gObj[iIndex].NextExp;

		gObj[iIndex].Experience = gObj[iIndex].NextExp;
		gObj[iIndex].Level += 1;

		if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA)
		{
			gObj[iIndex].LevelUpPoint += g_GlobalConfig.m_iSpecial_Points;
		}
		else
		{
    		gObj[iIndex].LevelUpPoint += g_GlobalConfig.m_iNormal_Points;
		}

		if( gObj[iIndex].PlusStatQuestClear )
		{	// +1스탯업 퀘스트 성공시 추가 스탯 
			gObj[iIndex].LevelUpPoint += g_GlobalConfig.m_iPowerUP_Points;
			LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].LevelUpPoint);
		}
			
		gObj[iIndex].MaxLife += DCInfo.DefClass[gObj[iIndex].Class].LevelLife;

		if(g_GlobalConfig.m_bLimitLife && gObj[iIndex].MaxLife > g_GlobalConfig.m_iLimitLife)
			gObj[iIndex].MaxLife = g_GlobalConfig.m_iLimitLife;

		gObj[iIndex].MaxMana += DCInfo.DefClass[gObj[iIndex].Class].LevelMana;
		gObj[iIndex].Life = gObj[iIndex].MaxLife;
		gObj[iIndex].Mana = gObj[iIndex].MaxMana;	
		gObjNextExpCal(&gObj[iIndex]);

		gObjSetBP(gObj[iIndex].m_Index);
		
		GCLevelUpMsgSend(gObj[iIndex].m_Index);

		gObjCalcMaxLifePower(gObj[iIndex].m_Index);	// 최대 생명력을 다시 계산한다
		
		LogAddTD(lMsg.Get(520), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level);

#ifdef ADD_LOG_400LEVELUP_PARTY_20070628	// 400레벨 달성시 같이 있던 파티원들 로그 추가
		if(gObj[iIndex].Level == 400 && gObj[iIndex].PartyNumber >=0)
		{
			int partynum = gObj[iIndex].PartyNumber;
			char partyLog[256];
			int number;
			sprintf(partyLog, "400 LevelUp (%s)(%s) Party ", gObj[iIndex].AccountID, gObj[iIndex].Name);
			
			for( int n=0; n<MAX_PARTYUSER; n++)
			{
				number = gParty.m_PartyS[partynum].Number[n];
				if( number >= 0 )
				{
					int noffSet = strlen(partyLog);
					sprintf(partyLog + noffSet, ",(%s)(%s) ",gObj[number].AccountID, gObj[number].Name);								
				}
			}
			
			LogAddTD(partyLog);
		}
#endif
	}

	// 서버에 자동 저장한다.
	GJSetCharacterInfo(&gObj[iIndex], gObj[iIndex].m_Index);
	//LogAdd("캐릭터 저장 메시지 보냄");

	return iLEFT_EXP;
}

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
BYTE CIllusionTempleProcess::GetRealRemainTimeMin()
{
	if(GetIllusionTempleState() == ILLUSION_TEMPLE_STATE_NONE)
	{
		return m_nRemainTimeMin;
	}

	return 0;
}
#endif

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
BOOL CIllusionTempleProcess::CheckSkillProdection(int iIllusionTempleIndex)
{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_5_20070807
	if( !CHECK_LIMIT(iIllusionTempleIndex, MAX_ILLUSION_TEMPLE_USER_COUNT) )
#else
	if( CHECK_LIMIT(iIllusionTempleIndex, MAX_ILLUSION_TEMPLE_USER_COUNT) )
#endif
		return FALSE;

	if(m_UserData[iIllusionTempleIndex].m_nIndex != -1)
	{
		if(m_UserData[iIllusionTempleIndex].m_dwSkillProdectionTime > 0)
			return TRUE;
	}

	return FALSE;
}

BOOL CIllusionTempleProcess::CheckSkillRestraint(int iIllusionTempleIndex)
{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_5_20070807
	if( !CHECK_LIMIT(iIllusionTempleIndex, MAX_ILLUSION_TEMPLE_USER_COUNT) )
#else
	if( CHECK_LIMIT(iIllusionTempleIndex, MAX_ILLUSION_TEMPLE_USER_COUNT) )
#endif
		return FALSE;

	if(m_UserData[iIllusionTempleIndex].m_nIndex != -1)
	{
		if(m_UserData[iIllusionTempleIndex].m_dwSkillRestraintTime > 0)
			return TRUE;
	}

	return FALSE;
}
#endif

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013
// 사원 점수를 보냄
void CIllusionTempleProcess::SendIllusionTempleResult()
{
	// 위의 점수를 이용 순위 결정( 정렬은 클라이언트에서.. ^^;)
	// 결과 전송
	int nOffSet = 0;
	BYTE SendByte[256];
	PMSG_ILLUSION_TEMPLE_RESULT	pMsg;
	
	pMsg.btAlliedPoint		= m_btAlliedPoint;
	pMsg.btIllusionPoint	= m_btIllusionPoint;	
	pMsg.btUserCount		= m_nUserCount;
	
	nOffSet += sizeof(pMsg);
	
	int i;
	for(i = 0; i < MAX_ILLUSION_TEMPLE_USER_COUNT; i++)
	{
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			PMSG_ILLUSION_TEMPLE_USER_ADD_EXP pAddExpMsg;
			memcpy(pAddExpMsg.GameID,    gObj[m_UserData[i].m_nIndex].Name, MAX_IDSTRING);
			pAddExpMsg.btMapNumber	= m_nTempleNumber + MAP_INDEX_ILLUSION_TEMPLE1;
			pAddExpMsg.btTeam		= m_UserData[i].m_btTeam;
			pAddExpMsg.btClass		= gObj[m_UserData[i].m_nIndex].Class;
			pAddExpMsg.nAddExp		= m_UserData[i].m_nRewardExp;
			
			memcpy(SendByte + nOffSet, (char*)&pAddExpMsg, sizeof(pAddExpMsg));
			nOffSet += sizeof(pAddExpMsg);
			
			// 로그수정
			LogAddTD("[Illusion Temple] (%d) RewardExp Result  (Account:%s, Name:%s, Team:%d, Class:%d, UserKillCount:%d, RelicsMoveCount:%d, RewardExp:%d, RankingPoint: %d)", 
				m_nTempleNumber +1,							// 사원번호
				gObj[m_UserData[i].m_nIndex].AccountID,		// 계정명
				gObj[m_UserData[i].m_nIndex].Name,			// 캐릭터명
				m_UserData[i].m_btTeam,						// 팀
				gObj[m_UserData[i].m_nIndex].DbClass,		// 클래스 0:흑마법사, 1:소울마스터, 16:흑기사, 17:블레이드나이트, 32:요정, 33:뮤즈엘프, 48:마검사, 64:다크로드
				m_UserData[i].m_btUserKillCount,			// 사용자 킬수
				m_UserData[i].m_btRelicsMoveCount,			// 성물 이동 수
				m_UserData[i].m_nRewardExp,					// 보상 경험치
				m_UserData[i].m_nRankingPoint);				// 랭킹 포인트
			
		}
	}
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x04, nOffSet);
	
	memcpy(SendByte, (char*)&pMsg, sizeof(pMsg));
	
	for (i = 0 ; i < MAX_ILLUSION_TEMPLE_USER_COUNT ; i++) 
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
		if(m_UserData[i].m_btTeam == 0xFF)
			continue;
#endif
		if(m_UserData[i].m_nIndex != -1 && gObj[m_UserData[i].m_nIndex].Connected > 2)
		{
			DataSend(m_UserData[i].m_nIndex, (LPBYTE)&SendByte , pMsg.h.size);
		}
	}
}
#endif	// MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013

#endif // ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328