// BloodCastle.cpp: implementation of the CBloodCastle class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BloodCastle.h"

#ifdef FOR_BLOODCASTLE
CBloodCastle			g_BloodCastle;		// 블러드 캐슬 이벤트 객체


#include <stdio.h>

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
#include "gObjMonster.h"
#include "GameMain.h"
#include "PartyClass.h"
#include "wzUdp.h"

#ifdef MODIFY_MIX_SYSTEM_20070518
//#include "ChaosBox.h"
#include "MixSystem.h"
#endif

#include "DSProtocol.h"

#include "MapClass.h"
#include "GameServer.h"
#include "DirPath.h"
#include "..\\Common\\Classdef.h"

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219
#include "CastleSiegeSync.h"
#endif

#ifdef BLOODCASTLE_EVENT_5TH_20050531
#include "MonsterAttr.h"
extern CMonsterAttr		gMAttr;
#endif

#ifdef MU_CRYWOLF_PENALTY_20051215
#include "CrywolfSync.h"
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

#include "..\\common\\zzzitem.h"

extern ITEM_ATTRIBUTE	ItemAttribute[];

extern PartyClass		gParty;
extern CMonsterSetBase	gMSetBase;
extern MapClass			*MapC;
extern classdef			DCInfo;


extern wsJoinServerCli	wsRServerCli;			//  랭킹서버
extern wsJoinServerCli	wsEvenChipServerCli;	//  이벤트 칩 서버

extern CDirPath			gDirPath;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBloodCastle::CBloodCastle()
{
	m_bBC_EVENT_ENABLE = false;

	m_iBC_TIME_MIN_OPEN = 10;		// 나중에 파일에서 읽어야 함

	m_iBC_NORMAL_ITEM_DROP = 100;
	m_iBC_EXCEL_ITEM_DROP = 1200;
	m_iBC_MONSTER_REGEN = 0;

	for (int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++) {
		m_BridgeData[i].m_iBC_STATE = BLOODCASTLE_STATE_NONE;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 번호 수정
		m_BridgeData[i].m_iMapNumber = GetMapNumByBCBridge( i );
#else
		m_BridgeData[i].m_iMapNumber = MAP_INDEX_BLOODCASTLE1 + i;
#endif
		m_BridgeData[i].m_iBridgeIndex = i;
		m_BridgeData[i].m_iBC_REMAIN_MSEC = -1;
		m_BridgeData[i].m_iBC_TICK_COUNT = -1;
		m_BridgeData[i].m_nSaintStatuePosNum = -1;
		m_BridgeData[i].m_nCastleDoorPosNum = -1;
		m_BridgeData[i].m_iAngelKingPosNum = -1;
		
	#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
		m_BridgeData[i].m_fRewardExpRate = 1.0f;
	#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			
		InitializeCriticalSection (& m_BridgeData[i].m_critUserData);

		ClearBridgeData(i);
	}
}


CBloodCastle::~CBloodCastle()
{
	for (int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++) {
		DeleteCriticalSection (& m_BridgeData[i].m_critUserData);
	}
}


void CBloodCastle::Init (bool bEVENT_ENABLE)
{
	m_bBC_EVENT_ENABLE = bEVENT_ENABLE;

	// 1 . 광장의 보스 몬스터 데이터를 초기화 한다.
	for (int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++) {
		for (int j = 0 ; j < MAX_BLOODCASTLE_BOSS_COUNT ; j++) {
			m_BridgeData[i].m_iAngelKingPosNum = -1;
			m_BridgeData[i].m_nBossMonsterPosNum[j] = -1;
		}
	}

	// 2 . MonsterSetBase.txt를 읽어서 성자의석상, 성문, 보스몹의 인덱스를 읽어서 저장한다.
	for( int n=0; n<gMSetBase.m_Count; n++)
	{
		if( CHECK_BLOODCASTLE(gMSetBase.m_Mp[n].m_MapNumber) )		// 블러드 캐슬에 들어가는 몬스터만
		{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 몬스터 타입 형변경			
			WORD btMonsterType = gMSetBase.m_Mp[n].m_Type;
#else
			BYTE btMonsterType = gMSetBase.m_Mp[n].m_Type;
#endif
			BYTE btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리번호 수정
			BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
			BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

			if (btMonsterType == NPC_ANGELKING) {					// 대천사는 미리 추가되므로 여기서 새로 추가하지 않는다. -> 대신 포지션 번호만 읽어둔다.
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리번호 수정
				m_BridgeData[btBloodCastleIndex].m_iAngelKingPosNum = n;
#else
				m_BridgeData[gMSetBase.m_Mp[n].m_MapNumber - MAP_INDEX_BLOODCASTLE1].m_iAngelKingPosNum = n;
#endif
				continue;
			}

			if (CHECK_LIMIT(btMonsterType-BC_SAINT_STATUE_1, 3)) {	// 성자의 석상의 포지션 번호를 읽는다.
				m_BridgeData[btBloodCastleIndex].m_nSaintStatuePosNum = n;
				continue;
			}

			if (btMonsterType == 131) {								// 성문의 포지션 번호를 읽는다.
				m_BridgeData[btBloodCastleIndex].m_nCastleDoorPosNum = n;
				continue;
			}

			// 보스로 정해진 몹의 포지션 번호를 읽는다.
			if ((btMonsterType == 89) ||							// 마법해골1
				(btMonsterType == 95) ||							// 마법해골2
				(btMonsterType == 112) ||							// 마법해골3
				(btMonsterType == 118) ||							// 마법해골4
				(btMonsterType == 124) ||							// 마법해골5
				(btMonsterType == 130)								// 마법해골6
#ifdef BLOODCASTLE_EXTEND_20040314
				|| (btMonsterType == 143)							// 마법해골7
#endif		
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// 블러드캐슬 보스몬스터 포지션 셋팅
				|| (btMonsterType == 433)							// 마법해골8
#endif				
				)
			{
				for (int i = 0 ; i < MAX_BLOODCASTLE_BOSS_COUNT ; i++) {
					if (m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[i] == -1) {
						m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[i] = n;
						break;
					}
				}				
			}
		}
	}

	for ( int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++)
		SetState(i, BLOODCASTLE_STATE_CLOSED);
}


void CBloodCastle::Load(char* filename)
{
	// *> . 스크립트를 읽어서 필요한 데이터를 저장한다.
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("[Blood Castle] Info file Load Fail [%s]", filename);
		return ;
	}
	SMDToken Token;
	
	int type = -1;
	int BridgeCount = -1;
	int iBridgeNum;

	for(int i = 0;i < MAX_BLOODCASTLE_BRIDGE_COUNT;++i)
		m_LST_BLOODCASTLE_REWARD[i].clear();
	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{				
			if( type == 0 )
			{	// 블러드캐슬 게임시간 설정
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iBC_TIME_MIN_OPEN = (int)TokenNumber;
				Token = (*GetToken)(); m_iBC_TIME_MIN_PLAY = (int)TokenNumber;
				Token = (*GetToken)(); m_iBC_TIME_MIN_REST = (int)TokenNumber;
			}
			else if( type == 1 )
			{	// 블러드캐슬 기본 및 엑셀 아이템 드롭율, 몬스터 리젠시간 설정
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iBC_NORMAL_ITEM_DROP = (int)TokenNumber;			
				Token = (*GetToken)(); m_iBC_EXCEL_ITEM_DROP	= (int)TokenNumber;
				Token = (*GetToken)(); m_iBC_MONSTER_REGEN		= (int)TokenNumber;
			}
			else if( type == 2 )
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iBridgeNum = (int)TokenNumber;
				
				if (CHECK_LIMIT(iBridgeNum, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
					Token = (*GetToken)(); m_BridgeData[iBridgeNum].m_iCastleStatueHealth = (float)TokenNumber;
					Token = (*GetToken)(); m_BridgeData[iBridgeNum].m_iCastleDoorHealth = (float)TokenNumber;
					Token = (*GetToken)(); m_BridgeData[iBridgeNum].m_iDropItemCount = TokenNumber;
				}
				else {
					Token = (*GetToken)();
					Token = (*GetToken)();
				}
			}
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			else if( type == 3 )
			{	// 블러드 캐슬 보상 경험치 퍼센트
				Token = (*GetToken)();
				if( strcmp("end", TokenString) == NULL ) break;

				iBridgeNum = (int)TokenNumber;
				
				if( CHECK_LIMIT(iBridgeNum, MAX_BLOODCASTLE_BRIDGE_COUNT) ) {
					Token = (*GetToken)();
					m_BridgeData[iBridgeNum].m_fRewardExpRate = (FLOAT)TokenNumber;
				}
				else Token = (*GetToken)();
			}
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
			else if( type == 4 )
			{
				BLOODCASTLE_STARTTIME startTime;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				startTime.nHour = (int)TokenNumber;
				Token = (*GetToken)(); startTime.nMin = (int)TokenNumber;			

				m_LST_BLOODCASTLE_START_TIME.push_back(startTime);

			}
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624			
			else if( type == 5 )
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iBridgeNum = (int)TokenNumber;

				while(true)
				{
					BLOODCASTLE_REWARD reward;

					Token = (*GetToken)();
					if(strcmp("end",TokenString)==NULL) break;

					reward.item_type = (unsigned short)TokenNumber;

					Token = (*GetToken)();
					reward.item_type = ItemGetNumberMake(reward.item_type, (int)TokenNumber);

					Token = (*GetToken)();
					reward.item_level = (char)TokenNumber;

					Token = (*GetToken)();
					reward.is_skill = TokenNumber > 0;

					Token = (*GetToken)();
					reward.is_luck = TokenNumber > 0;

					Token = (*GetToken)();
					reward.option = (char)TokenNumber;

					Token = (*GetToken)();
					reward.exc_option = (char)TokenNumber;

					Token = (*GetToken)();
					reward.max_drop_rate = (unsigned short)TokenNumber;

					if (CHECK_LIMIT(iBridgeNum, MAX_BLOODCASTLE_BRIDGE_COUNT))
						m_LST_BLOODCASTLE_REWARD[iBridgeNum].push_back(reward);
				};
			}
			else if( type == 6 )
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iBridgeNum = (int)TokenNumber;

				while(true)
				{
					Token = (*GetToken)();
					if(strcmp("end",TokenString)==NULL) break;

					if(iBridgeNum >= 0 && iBridgeNum < MAX_BLOODCASTLE_BRIDGE_COUNT)
					{
						g_sttBLOODCASTLE_LEVEL[iBridgeNum].iLOWER_BOUND = TokenNumber;

						Token = (*GetToken)();
						g_sttBLOODCASTLE_LEVEL[iBridgeNum].iUPPER_BOUND = TokenNumber;

						Token = (*GetToken)();
						g_sttBLOODCASTLE_LEVEL[iBridgeNum].iLOWER_BOUND_MAGUMSA = TokenNumber;

						Token = (*GetToken)();
						g_sttBLOODCASTLE_LEVEL[iBridgeNum].iUPPER_BOUND_MAGUMSA = TokenNumber;
					}
					else
					{
						Token = (*GetToken)();
						Token = (*GetToken)();
						Token = (*GetToken)();
					}
				};
			}
		}
	}

	fclose(SMDFile);

	LogAdd("%s file load!", filename);

// grooving 사내 테스트 블러드 캐슬 시간 조정
#if TESTSERVER == 1 && defined(UPDATE_BLOODECASTLE_SCADULE_20080624)

	m_iBC_TIME_MIN_OPEN = 1;
	m_iBC_TIME_MIN_PLAY = 5;
	
	BLOODCASTLE_STARTTIME startTime;

	m_LST_BLOODCASTLE_START_TIME.clear();

	for( int i = 0; i < 24; i++)
	{
		startTime.nHour = i;
		
		for( int j = 0; j < 60; j+= 6 )
		{
			startTime.nMin = j;
			
			m_LST_BLOODCASTLE_START_TIME.push_back(startTime);
		}
	}			
#endif//TESTSERVER

}


void CBloodCastle::LoadItemDropRate ()
{
	// 블러드 캐슬 수행여부와 아이템 드롭확률 부분만 리로드 한다.
	g_bBloodCastle				= GetPrivateProfileInt("GameServerInfo", "BloodCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iAngelKingsPaperDropRate	= GetPrivateProfileInt("GameServerInfo", "AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBloodBoneDropRate		= GetPrivateProfileInt("GameServerInfo", "BloodBoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iStoneDropRate			= GetPrivateProfileInt("GameServerInfo", "StoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_bStoneItemDrop			= GetPrivateProfileInt("GameServerInfo", "StoneItemDrop", 0, gDirPath.GetNewPath("commonserver.cfg"));
}

#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
void CBloodCastle::CheckSync(int iBridgeIndex)			// 최초에 BLOODCASTLE_STATE_CLOSED -> BLOODCASTLE_STATE_PLAYING 상태로 변하기 까지의 시간체크
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	list<BLOODCASTLE_STARTTIME>::iterator iter;
	BLOODCASTLE_STARTTIME	prevStartTime;
	BLOODCASTLE_STARTTIME	nextStartTime;

	DWORD	dwPrevTime	= 0;;		// 이전 시작 시간(분)
	DWORD	dwNextTime	= 0;;		// 다음 시작 시간(분)
	DWORD	dwCurTime	= 0;;		// 현재 시간(분)
	
	struct tm *today;
	time_t ltime;

	m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
	
	if(m_LST_BLOODCASTLE_START_TIME.size() == 0)
	{
		MsgBox("Error : Blood Castle StartTime size is 0");
		return;
	}
	
	time( &ltime );
	today = localtime( &ltime );
	dwCurTime	= today->tm_hour * 60 + today->tm_min;

	// 다음 이벤트 열리 시간을 검색
	prevStartTime = *m_LST_BLOODCASTLE_START_TIME.begin();
	for(iter = m_LST_BLOODCASTLE_START_TIME.begin(); iter != m_LST_BLOODCASTLE_START_TIME.end(); ++iter)
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
		if(iter == m_LST_BLOODCASTLE_START_TIME.end())
		{
			// 다음 날 시간
			iter = m_LST_BLOODCASTLE_START_TIME.begin();
			nextStartTime = *iter;
		}
		
		// 다음 시작시간을 다시 계산
		dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;

		// 다음 시작까지 남은 시간 계산
		if(today->tm_hour <= nextStartTime.nHour && dwNextTime > dwCurTime)
		{
			// 다음 이벤트가 오늘
			m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (dwNextTime - dwCurTime) * 60 * 1000;
		}
		else
		{
			// 다음 이벤트가 다음날
			m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (24 * 60 - dwCurTime + dwNextTime) * 60 * 1000;
		}

		// 현재 시작까지 남은 시간(m_nRemainTime)이 오픈시간(m_iOpenTime)보다 작으면 다음 시간으로 넘긴다
		if( m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= m_iBC_TIME_MIN_OPEN * 60 * 1000 )
		{
			iter++;
			if(iter != m_LST_BLOODCASTLE_START_TIME.end())
			{
				nextStartTime = *iter;
			}
		}
		else
		{
			break;
		}
	}

	// 다음 분(min) 계산
	int tmpTime = GetTickCount();
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC - (today->tm_sec * 1000) ;

	LogAddTD("[Blood Castle] (%d) Sync Open Time. [%d] min remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60 * 1000));

}
#else	// UPDATE_BLOODECASTLE_SCADULE_20080624
void CBloodCastle::CheckSync(int iBridgeIndex)			// 최초에 BLOODCASTLE_STATE_CLOSED -> BLOODCASTLE_STATE_PLAYING 상태로 변하기 까지의 시간체크
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;
	
	
#if TESTSERVER == 1
	// 테스트용 루틴
	//m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_iBC_TIME_MIN_OPEN * 60 * 1000 ;

#ifdef FOR_ONAIR
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (m_iBC_TIME_MIN_OPEN + 5) * 60 * 1000 ;
#else
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (m_iBC_TIME_MIN_OPEN + 1) * 60 * 1000 ;
#endif
	m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
#else
	
	struct tm * today;
	time_t ltime;
	
	time( &ltime );
	today = localtime( &ltime );

	if(!CHECK_LIMIT(g_iBloodCastle_StartHour-1,2))
		g_iBloodCastle_StartHour = 1;

	switch(g_iBloodCastle_StartHour) {
	case 2 :	// 2시간마다
		{
#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511	// 짝수 시간대에서 홀수 시간대로 변경
#ifdef UPDATE_BLOODECASTLE_SCADULE_20070702
			if( (today->tm_hour%2) == g_iBloodCastle_odd_Even_Hour)
#else
			if( (today->tm_hour%2) )
#endif
#else
			if( !(today->tm_hour%2) )
#endif
			{
				// 짝수시간
				if ((today->tm_min * 60 + today->tm_sec) >= 30 * 60) {
					// 30분을 넘겼다 -> 2시간 후의 30분을 기약
					m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (((60*2 + 30) * 60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
				}
				else {
					// 30분을 아직 않넘겼다 -> 시간잡음
					m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = ((30*60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
				}
			}
			else
			{
				// 홀수시간
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (((60 + 30) * 60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
			}
			
			if( m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 0 )
			{
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC += (60 * 60) * 1000;
			}
			
			m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
		}
		break;
	case 1 :	// 1시간마다
	default:
		{
			if ((today->tm_min * 60 + today->tm_sec) >= 30 * 60) {
				// 30분을 넘겼다 -> 1시간 후의 30분을 기약
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = ((60*60) + (30*60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
			}
			else {
				// 30분을 아직 않넘겼다 -> 시간잡음
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = ((30*60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
			}

			if( m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 0 )
			{
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC += ((60 * 60)) * 1000;
			}

			m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
		}
		break;
	}
	
	LogAddTD("[Blood Castle] (%d) Sync Open Time. [%d] min remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60 * 1000));
#endif

}
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624


void CBloodCastle::ClearBridgeData (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	m_BridgeData[iBridgeIndex].m_iTOTAL_EXP							= 0;
	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT				= 0;
	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT				= 0;
	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT		= 0;
	m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT			= 0;
	m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT		= 0;
	m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT	= 0;
	m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER				= 0;
	m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN				= -1;
	m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL				= -1;
	m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX			= -1;
	m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS					= -1;
	m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT					= -1;
	m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE					= TRUE;
	m_BridgeData[iBridgeIndex].m_bBC_REWARDED						= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER				= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY				= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END					= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT				= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER						= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY						= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_PLAY_START						= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE			= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE		= FALSE;

	m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE		= FALSE;
	

	m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index			= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party			= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index		= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party		= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index			= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party			= -10;

	memset(m_BridgeData[iBridgeIndex].m_szKill_Door_CharName, '\0', MAX_IDSTRING+1);
	memset(m_BridgeData[iBridgeIndex].m_szKill_Status_CharName, '\0', MAX_IDSTRING+1);
	memset(m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName, '\0', MAX_IDSTRING+1);

	memset(m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, '\0', MAX_IDSTRING+1);
	memset(m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID, '\0', MAX_IDSTRING+1);
	memset(m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID, '\0', MAX_IDSTRING+1);
	
	for (INT i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP = 0;
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex = -1;
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex = -1;
#ifdef BLOODCASTLE_EVENT_5TH_20050531
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_bBloodCastleComplete = false;
#endif
		}

		m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP					= 0;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore				= 0;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex				= -1;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState			= BC_USER_ALIVE;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg			= false;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak	= false;

#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
		memset(m_BridgeData[iBridgeIndex].m_UserData[i].m_szAccountID, 0, sizeof(m_BridgeData[iBridgeIndex].m_UserData[i].m_szAccountID));
		memset(m_BridgeData[iBridgeIndex].m_UserData[i].m_szCharName, 0, sizeof(m_BridgeData[iBridgeIndex].m_UserData[i].m_szCharName));
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iCharClass = -1;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iBloodCastleIndex = -1;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iBloodCastleSubIndex = -1;
#endif	

#ifdef BLOODCASTLE_EVENT_5TH_20050531
		m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = -1;
#endif
		
#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
		m_BridgeData[iBridgeIndex].m_iDoorIndex				= -1;
#endif
	}
}


void CBloodCastle::SetState(int iBridgeIndex, int iBC_STATE)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	if ((iBC_STATE < BLOODCASTLE_STATE_NONE) || (iBC_STATE > BLOODCASTLE_STATE_PLAYEND))
		return;

	m_BridgeData[iBridgeIndex].m_iBC_STATE = iBC_STATE;

	switch(m_BridgeData[iBridgeIndex].m_iBC_STATE) {
	case BLOODCASTLE_STATE_NONE :
		SetState_None(iBridgeIndex);
		break;
	case BLOODCASTLE_STATE_CLOSED :
		SetState_Closed(iBridgeIndex);
		break;
	case BLOODCASTLE_STATE_PLAYING :
		SetState_Playing(iBridgeIndex);
		break;
	case BLOODCASTLE_STATE_PLAYEND :
		SetState_PlayEnd(iBridgeIndex);
		break;
	default:
		break;
	}
}


void CBloodCastle::Run()					// 초 단위로 시간을 깍는다.
{
	if (m_bBC_EVENT_ENABLE) {
	
		for (INT i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++) {
			switch (m_BridgeData[i].m_iBC_STATE) {
			case BLOODCASTLE_STATE_NONE :
				ProcState_None(i);
				break;
			case BLOODCASTLE_STATE_CLOSED :
				ProcState_Closed(i);
				break;
			case BLOODCASTLE_STATE_PLAYING :
				ProcState_Playing(i);
				break;
			case BLOODCASTLE_STATE_PLAYEND :
				ProcState_PlayEnd(i);
				break;
			default:
				break;
			}
		}

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[13] != AUTH_CHECK_13 )
		DestroyGIocp();
#endif
	}
}


void CBloodCastle::ProcState_None(int iBridgeIndex)
{
	
}


void CBloodCastle::ProcState_Closed(int iBridgeIndex)
{
//	INT iTICK_SEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT) / 1000;
	INT iTICK_MSEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
		//LogAddTD("[Blood Castle] (%d) - CLOSED Check Time. [%d] sec remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
#endif
		if (g_bBloodCastle) {
			if ( (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= m_iBC_TIME_MIN_OPEN * 60 * 1000) && (! m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER) ) {
				// 이제부터 블러드캐슬에 입장과 파티가 가능하다.
				m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = true;
				m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = true;
			}
			
			// 주의> . 공지가 잘 않될 수 있다. !!
			if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= m_iBC_TIME_MIN_OPEN * 60 * 1000) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60*1000) != m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT )) {	
				m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60*1000);

				// 입장 가능시간부터 1분마다 공지한다.
				if (!iBridgeIndex) {
					// 다하면 너무 많으니 1번 다리만 대표로 => 접속한 모든 사용자들에게
					PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
					TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1160), m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT + 1);
#else
					pNotice.type = 0;
					wsprintf(pNotice.Notice, lMsg.Get(1160), m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT + 1);		// "블러드캐슬 입장까지 %d 분 남았습니다."
					PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
					SendAllUserAnyMsg ((LPBYTE) &pNotice, pNotice.h.size);
				}
			}

			if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30 * 1000) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0) && (!m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER)) {
				m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = true;

				if (!iBridgeIndex) {
					// 다하면 너무 많으니 1번 다리만 대표로 => 접속한 모든 사용자들에게
					PMSG_SET_DEVILSQUARE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
					pMsg.Type = BC_MSG_BEFORE_ENTER;

					for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
					{
						if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
						{
							if (!CHECK_BLOODCASTLE(gObj[n].MapNumber)
#ifdef CHAOSCASTLE_SYSTEM_20040408
								&& !CHECK_CHAOSCASTLE(gObj[n].MapNumber)	// 카오스캐슬에 접속하지 않은 사람들만 골라서 보낸다.
#endif
								)		// 블러드캐슬에 접속하지 않은 사람들만 골라서 보낸다.
								DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
						}
					}
				}
			}
		}
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0) {
		if (g_bBloodCastle)
			SetState (iBridgeIndex, BLOODCASTLE_STATE_PLAYING);
		else
			SetState (iBridgeIndex, BLOODCASTLE_STATE_CLOSED);
	}

}


void CBloodCastle::ProcState_Playing(int iBridgeIndex)
{
//	INT iTICK_SEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT) / 1000;
	INT iTICK_MSEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
		//LogAddTD("[Blood Castle] (%d) - PLAYING Check Time. [%d] sec remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
#endif
		if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= (m_iBC_TIME_MIN_PLAY * 60 - BC_MAX_START_WAIT_TERM + 30) * 1000) && (!m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY)) {
			m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = BC_MSG_BEFORE_PLAY;
			SendBridgeAnyMsg ((LPBYTE)&pMsg, sizeof( pMsg ), iBridgeIndex);
		}

		if (m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE) {
			if (m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN != -1) {
				if (GetTickCount() > m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN) {
					// 이제 BC_MAX_TICK_DOOR_OPEN 초가 지났다 -> 성문앞 다리속성 바꾸어라
					ReleaseCastleBridge(iBridgeIndex);						
					SendCastleBridgeBlockInfo(iBridgeIndex, false);
					LogAddTD ("[Blood Castle] (%d) Bridge Change Bridge Attribute -> Open", iBridgeIndex+1);

					m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = -1;

#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
					if(m_BridgeData[iBridgeIndex].m_iDoorIndex == -1)
					{
						LogAddTD ("[Blood Castle] (%d) Door Not Subsist", iBridgeIndex+1);
					}
					else
					{
						LogAddTD ("[Blood Castle] (%d) Door Subsist (%d)(Name: %s)(Dieregen:%d)"
							, iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iDoorIndex
							, gObj[m_BridgeData[iBridgeIndex].m_iDoorIndex].Name
							, gObj[m_BridgeData[iBridgeIndex].m_iDoorIndex].DieRegen);
					}
#endif
				}
			}
		}

		if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= (m_iBC_TIME_MIN_PLAY * 60 - BC_MAX_START_WAIT_TERM) * 1000) && (!m_BridgeData[iBridgeIndex].m_bBC_PLAY_START)) {
			// 경기시간 재설정
			m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_iBC_TIME_MIN_PLAY * 60 * 1000;		// BLOODCASTLE_STATE_PLAYING -> BLOODCASTLE_STATE_PLAYEND 까지의 남은 시간
			m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;

			// 이벤트 시작 메시지 필요!!
			PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
			TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1161), iBridgeIndex + 1 );
#else
			pNotice.type = 0;
			wsprintf(pNotice.Notice, lMsg.Get(1161), iBridgeIndex + 1);				// "제 %d 블러드 캐슬 퀘스트가 시작되었습니다."
			PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
			SendBridgeAnyMsg ((LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);


			// 다리의 입구속성을 해제하고 전체 인원에게 입구상태 속성의 해제를 알린다.
			ReleaseCastleEntrance(iBridgeIndex);
			SendCastleEntranceBlockInfo(iBridgeIndex, false);

			m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = true;
			SetMonster(iBridgeIndex);

			
			// 다리의 전체 인원에게 상태를 보내준다.
			SendNoticeState(iBridgeIndex, BC_STATE_START);

			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Start", iBridgeIndex+1);
		}


		if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30 * 1000) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0) && (!m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END)) {
			m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = BC_MSG_BEFORE_END;
			SendBridgeAnyMsg ((LPBYTE)&pMsg, sizeof( pMsg ), iBridgeIndex);
		}


		if (CheckEveryUserDie(iBridgeIndex)) {
			// 만약 전원 사망했다면 실패 메시지를 보낸다.
			PMSG_NOTICE	pNotice;
			
#ifdef MODIFY_NOTICE_20040325
			TNotice::MakeNoticeMsg( &pNotice, 0, lMsg.Get(1162) );
#else
			pNotice.type = 0;
			wsprintf(pNotice.Notice, lMsg.Get(1162));								// "블러드 캐슬로의 잠입은 실패했습니다."
			PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
			SendBridgeAnyMsg ((LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);

			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]", 
				iBridgeIndex+1, 
				m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				m_BridgeData[iBridgeIndex].m_szKill_Door_CharName
				);
			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]",
				iBridgeIndex+1, 
				m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				m_BridgeData[iBridgeIndex].m_szKill_Status_CharName
				);

			// 실패는 했지만 점수는 준다.
			GiveReward_Fail(iBridgeIndex);
			
			SetState (iBridgeIndex, BLOODCASTLE_STATE_CLOSED);

			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Failed -> Every User Out", iBridgeIndex+1);
		}
		else {
			if (m_BridgeData[iBridgeIndex].m_bBC_PLAY_START) {
				// 아니고 경기 진행중이라면 현재 상태를 계속 보내준다.
				if ( !m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE || m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE )
					SendNoticeState(iBridgeIndex, BC_STATE_PLAY);
				else
					SendNoticeState(iBridgeIndex, BC_STATE_PLAY_BOSS);
			}
		}

#ifdef BLOODCASTLE_EVENT_5TH_20050531
		// 우승자가 나타났는지 체크
		if (CheckWinnerExist(iBridgeIndex) == true) {
			if (CheckWinnerValid(iBridgeIndex) == true) {
				// 현재 블러드캐슬 경기가 끝났는지 확인 (우승자의 파티가 모두 대천사 등록 성공)
				if (CheckWinnerPartyComplete(iBridgeIndex) == true) {
					// 우승 케릭이 있는 다리에 보상을 한다.
					GiveReward_Win(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX, iBridgeIndex);

					// 퀘스트를 종료한다.
					SetState(iBridgeIndex, BLOODCASTLE_STATE_PLAYEND);

					LogAddTD ("[Blood Castle] (%d) CheckWinnerPartyComplete(iBridgeIndex) == true", iBridgeIndex+1);
					return;
				}
			}
			else {
				// 우승자의 존재여부가 옮바르지 않다면 바로 실패처리한다.
				GiveReward_Fail(iBridgeIndex);

				// 퀘스트를 종료한다.
				SetState(iBridgeIndex, BLOODCASTLE_STATE_PLAYEND);

				LogAddTD ("[Blood Castle] (%d) CheckWinnerValid(iBridgeIndex) == false", iBridgeIndex+1);
				return;
			}
		}
#endif
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0) {
		if (!m_BridgeData[iBridgeIndex].m_bBC_REWARDED) {
			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]", 
				iBridgeIndex+1, 
				m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				m_BridgeData[iBridgeIndex].m_szKill_Door_CharName
				);
			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]",
				iBridgeIndex+1, 
				m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID,
				m_BridgeData[iBridgeIndex].m_szKill_Status_CharName
				);

			// 실패는 했지만 점수는 준다.
			GiveReward_Fail(iBridgeIndex);

			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Failed -> Time Out", iBridgeIndex+1);
		}

		SetState (iBridgeIndex, BLOODCASTLE_STATE_PLAYEND);
	}
}


void CBloodCastle::ProcState_PlayEnd(int iBridgeIndex)
{
//	INT iTICK_SEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT) / 1000;
	INT iTICK_MSEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
		//LogAddTD("[Blood Castle] (%d) - PLAY END Check Time. [%d] sec remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC/1000);
#endif
		if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30*1000) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0) && (!m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT)) {
			m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = BC_MSG_BEFORE_QUIT;
			SendBridgeAnyMsg ((LPBYTE)&pMsg, sizeof( pMsg ), iBridgeIndex);
		}
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0) {
		SetState (iBridgeIndex, BLOODCASTLE_STATE_CLOSED);
	}
}


void CBloodCastle::SetState_None(int iBridgeIndex)
{
	// 현재 진행중인 이벤트를 중지하고 초기화 함 (갑자기 이벤트를 중지시켜야 할 때 사용한다.)
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC	= -1;
	m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT		= -1;

	SendNoticeState(iBridgeIndex, BC_STATE_END);

	ClearBridgeData(iBridgeIndex);

	ClearMonster(iBridgeIndex, true);

	// 혹시 블러드캐슬에 남아있는 사람이 있으면 모두 데비아스로 이동시킨다.
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
		if( (gObj[n].MapNumber == iMapNum) && (gObj[n].Connected == 3) )
#else
		if( (gObj[n].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[n].Connected == 3) )
#endif
		{					
			gObjMoveGate(n, 22);
		}
	}

	// 문의 속성 상태를 벽으로 막아놓는다.
	BlockCastleDoor(iBridgeIndex);
}


void CBloodCastle::SetState_Closed(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	SendNoticeState(iBridgeIndex, BC_STATE_END);

	ClearBridgeData(iBridgeIndex);

	ClearMonster(iBridgeIndex, true);

	CheckAngelKingExist (iBridgeIndex);				// 대천사가 존재하는지 체크 -> 없으면 만든다.

//	BlockSector(iBridgeIndex + MAP_INDEX_BLOODCASTLE1, 13,	77,	15,	78);		// !!> 단지 테스트임 -> 성문 안밖의 몬스터 이동이 격리됨

	// 혹시 블러드캐슬에 남아있는 사람이 있으면 절대무기가 있는지 찾아서 지우고 모두 데비아스로 이동시킨다.
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );		
		if( (gObj[n].MapNumber == iMapNum) && (gObj[n].Connected > 2) )
#else
		if( (gObj[n].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[n].Connected > 2) )
#endif
		{
			SearchUserDeleteQuestItem(n);			
			gObjMoveGate(n, 22);
		}
	}

	// 문의 속성 상태를 벽으로 막아놓는다.
	BlockCastleDoor(iBridgeIndex);
	// 문앞 다리의 속성 상태를 벽으로 막아놓는다.
	BlockCastleBridge(iBridgeIndex);
	// 다리앞 입구의 속성 상태를 뚫림으로 막아놓는다.
	BlockCastleEntrance(iBridgeIndex);

	CheckSync(iBridgeIndex);						// BLOODCASTLE_STATE_CLOSED -> BLOODCASTLE_STATE_PLAYING 까지의 남은 시간을 계산해줌

	LogAddTD("[Blood Castle] (%d) SetState CLOSED", iBridgeIndex+1);
}


void CBloodCastle::SetState_Playing(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_iBC_TIME_MIN_PLAY * 60 * 1000;			// BLOODCASTLE_STATE_PLAYING -> BLOODCASTLE_STATE_PLAYEND 까지의 남은 시간
	
	// *> . 입장 시키기 전에 해당 캐슬에 사용자 중 접속을 않한 인원을 걸러낸다. -> 같은 인덱스를 쓰는 다른 캐슬의 사용자와 겹치지 않도록 (2004.06.09)
	CheckUsersOnConnect(iBridgeIndex);
	
	PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsgEx( &pNotice, 1, lMsg.Get(1163), iBridgeIndex + 1, BC_MAX_START_WAIT_TERM );
#else
	pNotice.type = 1;
	wsprintf(pNotice.Notice, lMsg.Get(1163), iBridgeIndex + 1, BC_MAX_START_WAIT_TERM);		// "제 %d 블러드 캐슬의 퀘스트 시작은 %d 초 후 입니다."
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

	SendBridgeAnyMsg ((LPBYTE) &pNotice, pNotice.h.size, iBridgeIndex);

	PMSG_SERVERCMD	ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
	ServerCmd.CmdType = 1;
	ServerCmd.X = 45;
	ServerCmd.Y = 0;
	SendBridgeAnyMsg ((LPBYTE) &ServerCmd, ServerCmd.h.size, iBridgeIndex);

	// 블러드캐슬에 남아있는 사람이 있으면 절대무기가 있는지 찾아서 지운다.
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
		if( (gObj[n].MapNumber == GetMapNumByBCBridge( iBridgeIndex ) ) && (gObj[n].Connected > 2) ) {
#else
		if( (gObj[n].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[n].Connected > 2) ) {
#endif
			SearchUserDeleteQuestItem(n);
		}
	}

	LogAddTD("[Blood Castle] (%d) SetState PLAYING", iBridgeIndex+1);
}


void CBloodCastle::SetState_PlayEnd(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	SendNoticeState(iBridgeIndex, BC_STATE_END);

	ClearMonster(iBridgeIndex, false);

	m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_iBC_TIME_MIN_REST * 60 * 1000;		// BLOODCASTLE_STATE_PLAYEND -> BLOODCASTLE_STATE_CLOSED 까지의 남은 시간

	LogAddTD("[Blood Castle] (%d) SetState PLAYEND", iBridgeIndex+1);

	for (int n=0 ; n < MAX_BLOODCASTLE_USER_COUNT ; n++) {
		if (CHECK_LIMIT(m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex, MAX_OBJECT)) {
			SearchUserDeleteQuestItem(m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex);
		}
	}

//	테스트 !! - 자신이 획특한 경험치를 보내줌
//	SendNoticeScore(iBridgeIndex);
}


int CBloodCastle::GetCurrentState(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

	return m_BridgeData[iBridgeIndex].m_iBC_STATE;
}


int CBloodCastle::GetCurrentRemainSec(int iBridgeIndex)
{
	return m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
}


int CBloodCastle::CheckEnterLevel (int iIndex, int iLevel)		// 해당 사용자가 블러드 캐슬에 입장할 수 있는지 확인
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 2;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return 2;

#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
#ifdef DARKLORD_WORK
	if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA) {
#else
	if (gObj[iIndex].Class == CLASS_MAGUMSA) {
#endif
		if (iLevel == 6) {	// 아이템의 레벨은 6이고
			if ( (gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel].iLOWER_BOUND_MAGUMSA) && (gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel].iUPPER_BOUND_MAGUMSA)) {
				// 나는 7캐슬에 갈 수 있는 레벨이다.
				return 0;
			}
		}
	}
	else {
		if (iLevel == 6) {	// 아이템의 레벨은 6이고
			if ( (gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel].iLOWER_BOUND) && (gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel].iUPPER_BOUND)) {
				// 나는 7캐슬에 갈 수 있는 레벨이다.
				return 0;
			}
		}
	}
#endif

#ifdef DARKLORD_WORK
	if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA) {
#else
	if (gObj[iIndex].Class == CLASS_MAGUMSA) {
#endif
		if ( (gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA) && (gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA))
			return 0;		// 갈수있다.
		else if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA)
			return -1;		// 내 레벨이 낮다.
		else if (gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA)
			return 1;		// 내 레벨이 높다.
	}
	else {
		if ( (gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND) && (gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND))
			return 0;		// 갈수있다.
		else if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND)
			return -1;		// 내 레벨이 낮다.
		else if (gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND)
			return 1;		// 내 레벨이 높다.
	}

	return 2;
}

#ifdef MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
bool CBloodCastle::CheckEnterFreeTicket(INT iIndex)		// 자유 입장권을 소지하고 있는지 확인
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 0;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return 0;

	for(int x=0; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			// 블러드 캐슬 자유 입장권
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM( 13, 47 ) )
			{
				return true;
			}
		}
	}

	return false;
}
#endif // MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627

bool CBloodCastle::BloodCastleChaosMix (int iIndex, int iLEVEL)		// 블러드캐슬 입장권 (투망) 조합을 시작한다.
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return false;
	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 변경
	int iMapNum = GetMapNumByBCBridge( iLEVEL - 1 );

	if( !CHECK_BLOODCASTLE( iMapNum ) )
		return false;
#else	
	if(!CHECK_BLOODCASTLE( iLEVEL - 1 + MAP_INDEX_BLOODCASTLE1 ) )
		return false;
#endif
	
	BOOL bMIX_RESULT=FALSE;		// 조합의 결과가 성공인지 실패인지 ?
	
	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;
	
	gObj[iIndex].ChaosLock = TRUE;
	LogAddTD("[Blood Castle] 투명망토 Mix Chaos Mix Start (Account:%s, Name:%s, Level:%d)", 
		gObj[iIndex].AccountID, 
		gObj[iIndex].Name, 
		iLEVEL);

#ifdef EXTEND_LOG_SYSTEM_05_20060823
	char chChaosLogText[20];

	wsprintf( chChaosLogText, "투명망토 Mix,%d", iLEVEL );
	g_MixSystem.LogChaosItem(&gObj[iIndex], chChaosLogText );
#elif defined(MODIFY_MIX_SYSTEM_20070518)
	g_MixSystem.LogChaosItem(&gObj[iIndex], "투명망토 Mix");
#endif // EXTEND_LOG_SYSTEM_05_20060823
	
	INT iMIX_SUCCESS_RATE = g_iBC_ChoasMixSuccessRate[iLEVEL-1];
	if ( (iMIX_SUCCESS_RATE < 0) || (iMIX_SUCCESS_RATE > 100) ) {
		// 성공확률 요류 (자체오류 ->  거의 나올 확률은 없다.)
		DataSend( iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Blood Castle] 투명망토 Mix Chaos Mix Failed - MixRate Out of Bound (Account:%s, Name:%s, Level:%d)",
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name, 
			iLEVEL
			);
		return FALSE;
	}
	
#ifdef MU_CRYWOLF_BENEFIT_20051215
	// MVP 성공 : 카오스 조합 성공률 증가 ( 블러드캐슬 입장권)
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_PEACE 
		&& g_iCrywolfApplyMvpBenefit	
	  )
	{
		iMIX_SUCCESS_RATE += g_CrywolfSync.GetPlusChaosRate();
	}
#endif

#ifdef MODIFY_CHAOSMIX_RATE_FIX_20070304
	if( iMIX_SUCCESS_RATE > 80 )
	{
		iMIX_SUCCESS_RATE = 80;
	}
#endif // MODIFY_CHAOSMIX_RATE_FIX_20070304

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	if( gObj[iIndex].ChaosSuccessRate > 10 )
	{
		pMsg.Result	= 0xF0;
		gObj[iIndex].ChaosLock = FALSE;
		DataSend( iIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
	iMIX_SUCCESS_RATE += gObj[iIndex].ChaosSuccessRate;
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	
	INT iMIX_NEED_MONEY = g_iBC_ChoasMixMoney[iLEVEL-1];
	
#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 블러드캐슬 투명망토 조합
	INT iChaosTaxMoney		= (INT)((INT64)iMIX_NEED_MONEY * (INT64)g_CastleSiegeSync.GetTaxRateChaos(iIndex) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney	= 0;

	iMIX_NEED_MONEY			= iMIX_NEED_MONEY + iChaosTaxMoney;
#endif

	if ( iMIX_NEED_MONEY < 0 ) {
		// 돈 오류 (자체오류 ->  거의 나올 확률은 없다.)
		DataSend( iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Blood Castle] 투명망토 Mix Chaos Mix Failed - MixMoney < 0 (Account:%s, Name:%s, Level:%d)", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name, 
			iLEVEL
			);
		return FALSE;
	}
	
	if( (gObj[iIndex].Money - iMIX_NEED_MONEY) < 0 )
	{
		// 돈이 모자르다. (0x86, Result: 0x0B 로 알림)
		pMsg.Result	= 0x0B;
		DataSend( iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Blood Castle] 투명망토 Mix Chaos Mix Failed - Not Enough Money (Account:%s, Name:%s, Level:%d)", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name, 
			iLEVEL
			);
		return FALSE;
	}
	
	gObj[iIndex].Money -= iMIX_NEED_MONEY;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 블러드캐슬 투명망토 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(iIndex, gObj[iIndex].Money);
	
	if (rand()%100 < iMIX_SUCCESS_RATE) {
		// 확률이 적용되어 조합에 성공했다. => 만들어 진다.
		int iItemType  = MAKE_ITEMNUM(13, 18);
#ifdef FOR_BLOODCASTLE_TEST
		ItemSerialCreateSend(iIndex, 255, 0, 0, iItemType, iLEVEL, 0, 0, 0, 0);
#else
		ItemSerialCreateSend(iIndex, 255, 0, 0, iItemType, iLEVEL, 255, 0, 0, 0);
#endif

		LogAddTD("[투명망토 Mix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   gObj[iIndex].AccountID, gObj[iIndex].Name, 
			   iMIX_SUCCESS_RATE, gObj[iIndex].Money, iMIX_NEED_MONEY);
	}
	else {
		// 조합에 실패했다.
#ifdef MODIFY_MIX_SYSTEM_20070518
		g_MixSystem.ChaosBoxInit(&gObj[iIndex]);
#endif
		GCUserChaosBoxSend(&gObj[iIndex]);
		DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[투명망토 Mix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   gObj[iIndex].AccountID, gObj[iIndex].Name, 
			   iMIX_SUCCESS_RATE, gObj[iIndex].Money, iMIX_NEED_MONEY);
		
#ifdef 	CHAOS_MIX_UPGRADE
		return FALSE;
#endif
	}
	
	gObjInventoryCommit(iIndex);
	
	return TRUE;
}


int CBloodCastle::CheckChoasMixItem	(int iIndex)		// 해당 사용자가 투명망토를 만들 재료 (대천사의서, 블러드본) 를 가지고 있는지 확인 (0 이면 없음 / 1 ~ 7 은 각 레벨의 재료) / 8 은 다른아이템 발견 / 9는 레벨 않맞음 / 10은 혼석없음 / 11은 아이템중 하나 없음 / 12는 이벤트아이템이 너무 많음 / 13은 돈이 모자르다. / 14 은 내 레벨이 낮다.
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 0;

	INT iCHAOS_MIX_LEVEL = 0;					// 몇레벨의 입장권을 생산할 수 있는가 ?

	BOOL bIsChaosGemExist = FALSE;				// 혼돈의 보석이 있는지 ?
	BOOL bIsAngelKingPaperExist = FALSE;		// 대천사의서가 있는지 ?
	BOOL bIsBloodBoneExist = FALSE;				// 블러드본이 있는지 ?
	BOOL bIsOtherItemExist = FALSE;				// 다른 아이템이 존재하는지 ?

	INT iEventItemCount = 0;

	INT iAngelKingPaperLevel = 0;
	INT iBloodBoneLevel = 0;

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	int iCharmOfLuckCount = 0;
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111

	for( int i=0; i<MAX_CHAOSBOXITEMS; i++)
	{
		if( gObj[iIndex].pChaosBox[i].IsItem() == TRUE )
		{
			if( gObj[iIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼돈의 보석
				bIsChaosGemExist = TRUE;
			}
			else if( gObj[iIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM(13, 16) )
			{	// 대천사의 서
				INT iITEM_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;

				iEventItemCount++;
				bIsAngelKingPaperExist = TRUE;

				iAngelKingPaperLevel = iITEM_LEVEL;
			}
			else if( gObj[iIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM(13, 17) )
			{	// 블러드 본
				INT iITEM_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;

				iEventItemCount++;
				bIsBloodBoneExist = TRUE;

				iBloodBoneLevel = iITEM_LEVEL;
			}
#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
			else if( gObj[iIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += gObj[iIndex].pChaosBox[i].m_Durability;
			}
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
			else
			{	// 이외의 아이템이 올려져 있다
				bIsOtherItemExist = TRUE;
			}
		}		
	}

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	gObj[iIndex].ChaosSuccessRate = iCharmOfLuckCount;
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111

	if (bIsOtherItemExist) {
		return 8;		// 다른 아이템 발견 -> 기존의 카오스 시스템으로 넘긴다.
	}

	if ((!bIsAngelKingPaperExist) && (!bIsBloodBoneExist)) {
		return 0;		// 블러드캐슬이 아닌 다른 조합일 수 있으므로 넘어간다.
	}

	else if ((!bIsAngelKingPaperExist) || (!bIsBloodBoneExist)) {
		return 11;		// 대천사의서, 블러드본 중 하나가 없음
	}

	if (iEventItemCount > 2) {
		return 12;		// 이벤트 아이템이 너무 많다.
	}

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	if( iCharmOfLuckCount > 10 )
	{
		return 15;
	}
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111

	if (iAngelKingPaperLevel != iBloodBoneLevel) {
		return 9;		// 아이템 레벨이 맞지 않음
	}
	else {
		if (!CHECK_LIMIT(iAngelKingPaperLevel-1, MAX_BLOODCASTLE_BRIDGE_COUNT)) 
			return 9;

		if (!CHECK_LIMIT(iBloodBoneLevel-1, MAX_BLOODCASTLE_BRIDGE_COUNT))
			return 9;
	}

#ifdef	FOR_BLOODCASTLE3
	// 2003.10.21 패치 이후에 적용되면 않됨
/*	if ( (iAngelKingPaperLevel == 1) && (iBloodBoneLevel == 1) ) {
		// 레벨 1일 때는 혼석이 있는것으로 간주
		bIsChaosGemExist = TRUE;
	}
*/
#endif

	if (!bIsChaosGemExist) {
		return 10;		// 혼석 없음
	}

	// 블러드캐슬 카오스 조합을 할 수 있는 최소레벨 체크
#ifdef DARKLORD_WORK
	if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA) {
#else
	if (gObj[iIndex].Class == CLASS_MAGUMSA) {
#endif
		if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND_MAGUMSA) {
			return 14;
		}
	}
	else {
		if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND) {
			return 14;
		}
	}

	if (bIsChaosGemExist && bIsAngelKingPaperExist && bIsBloodBoneExist) {
		// 모든 조건이 일치하므로 대천사의서 레벨을 보냄 (블러드본의 레벨도 상관없음)
		return iAngelKingPaperLevel;	
	}

	return 0;			// 그외의 예상치 못한 다른 오류
}


int CBloodCastle::CheckEnterItem (int iIndex)					// 해당 사용자가 블러드 캐슬에 입장할 수 있는 아이템 (투명망토) 이 있는지 확인
{
	INT iITEM_LEVEL = 0;

	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 0;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return 0;

	for(int x=0; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 18) )		// 투명방토가 존재한다면
			{
				iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;				// 아이템 레벨을 얻는다.
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 투명망토8 사용할수 있게.
				if (!CHECK_LIMIT(iITEM_LEVEL, MAX_BLOODCASTLE_BRIDGE_COUNT+1))
#else
				if (!CHECK_LIMIT(iITEM_LEVEL, 8))
#endif
					iITEM_LEVEL = 0;

				if (iITEM_LEVEL)	return iITEM_LEVEL;
			}
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
			// 블러드 캐슬 자유 입장권 추가
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM( 13, 47 ) )
			{
				iITEM_LEVEL = 10;
			}
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		}
	}

	return iITEM_LEVEL;
}


int CBloodCastle::CheckQuestItem (int iIndex)					// 해당 사용자가 블러드 캐슬의 퀘스트 아이템 중 하나를 기지고 있는지 확인
{
	INT iITEM_LEVEL = -1;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return -1;

	if (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber))
		return -1;

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
	int iBridgeIndex = GetBridgeIndexByMapNum( gObj[iIndex].MapNumber );	
	if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1)
#else
	if (m_BridgeData[gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1].m_nBC_QUESTITEM_SERIAL == -1)
#endif
		return -1;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return -1;

	for(int x=0; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 19) )		// 대천사 시리즈가 존재한다면
			{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
				if (gObj[iIndex].pInventory[x].m_Number == m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL) {
#else
				if (gObj[iIndex].pInventory[x].m_Number == m_BridgeData[gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1].m_nBC_QUESTITEM_SERIAL) {
#endif
					// 저장된 시리얼 번호까지 같다면
					iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;			// 아이템 레벨을 얻는다.

					if ((iITEM_LEVEL < 0) || (iITEM_LEVEL > 2))
						iITEM_LEVEL = -1;

					break;
				}
			}
		}
	}

	return iITEM_LEVEL;
}


bool CBloodCastle::CheckWalk (int iIndex, int iMoveX, int iMoveY)
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return false;

	if(!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber) )
		return false;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return false;
	
	if (this->GetCurrentState(gObj[iIndex].MapNumber) == BLOODCASTLE_STATE_CLOSED) {	// 경기시작 직전이고 (BLOODCASTLE_STATE_CLOSED -> 바뀌어야 함!!)
//		if (CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)) {								// 블러드 캐슬 안이라면
			
		BYTE attr = MapC[gObj[iIndex].MapNumber].GetAttr(iMoveX, iMoveY);

		if( ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) )			// 안전지대가 아니라면
			return true;					// 단지 조건에 맞다는 뜻이다. (1>.경기직전, 2>.블러드캐슬안, 3>.안전지대아님)
//		}
	}

	return false;
}


bool CBloodCastle::CheckCanEnter (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER;
}


bool CBloodCastle::CheckCanParty (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY;
}


bool CBloodCastle::CheckQuestItemSerial (int iBridgeIndex, CMapItem * lpItem)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1)
		return false;

	if( lpItem->IsItem() == TRUE )
	{
		if( lpItem->m_Type == MAKE_ITEMNUM(13, 19) )		// 대천사 시리즈가 존재한다면
		{
			INT iITEM_LEVEL = lpItem->m_Level;				// 아이템 레벨을 얻는다.

			if (CHECK_LIMIT(iITEM_LEVEL, 3)) {
				// 레벨은 일단 맞다.
				if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == lpItem->m_Number) {
					// 시리얼까지 맞다면 바로 이것이다.
					return true;
				}
			}
		}
	}

	return false;
}


bool CBloodCastle::CheckPlayStart (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_PLAY_START;
}


#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
int CBloodCastle::GetRemainTime(int iBridgeIndex)
{
	INT iREMAIN_MINUTE = 0;

	if (this->GetCurrentState(iBridgeIndex) == BLOODCASTLE_STATE_CLOSED) 
	{
		iREMAIN_MINUTE = (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60 * 1000) - m_iBC_TIME_MIN_OPEN) + 1;
	}
	else
	{
		list<BLOODCASTLE_STARTTIME>::iterator iter;
		BLOODCASTLE_STARTTIME	prevStartTime;
		BLOODCASTLE_STARTTIME	nextStartTime;
		
		DWORD	dwPrevTime	= 0;;		// 이전 시작 시간(분)
		DWORD	dwNextTime	= 0;;		// 다음 시작 시간(분)
		DWORD	dwCurTime	= 0;;		// 현재 시간(분)
		
		struct tm *today;
		time_t ltime;
		

		time( &ltime );
		today = localtime( &ltime );
		dwCurTime	= today->tm_hour * 60 + today->tm_min;
		
		// 다음 이벤트 열리 시간을 검색
		prevStartTime = *m_LST_BLOODCASTLE_START_TIME.begin();
		for(iter = m_LST_BLOODCASTLE_START_TIME.begin(); iter != m_LST_BLOODCASTLE_START_TIME.end(); ++iter)
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
			if(iter == m_LST_BLOODCASTLE_START_TIME.end())
			{
				// 다음 날 시간
				iter = m_LST_BLOODCASTLE_START_TIME.begin();
				nextStartTime = *iter;
			}
			
			// 다음 시작시간을 다시 계산
			dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;
			
			// 다음 시작까지 남은 시간 계산
			if(today->tm_hour <= nextStartTime.nHour && dwNextTime > dwCurTime)
			{
				// 다음 이벤트가 오늘
				iREMAIN_MINUTE = (dwNextTime - dwCurTime) * 60 * 1000;
			}
			else
			{
				// 다음 이벤트가 다음날
				iREMAIN_MINUTE = (24 * 60 - dwCurTime + dwNextTime) * 60 * 1000;
			}
			
			// 현재 시작까지 남은 시간(m_nRemainTime)이 오픈시간(m_iOpenTime)보다 작으면 다음 시간으로 넘긴다
			if( m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= m_iBC_TIME_MIN_OPEN * 60 * 1000 )
			{
				iter++;
				if(iter != m_LST_BLOODCASTLE_START_TIME.end())
				{
					nextStartTime = *iter;
				}
			}
			else
			{
				break;
			}
		}
		
		// 다음 분(min) 계산
		int tmpTime = GetTickCount();
		iREMAIN_MINUTE = m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC - (today->tm_sec * 1000) ;
	}

	return iREMAIN_MINUTE;
}
#else	// UPDATE_BLOODECASTLE_SCADULE_20080624
int CBloodCastle::GetRemainTime(int iBridgeIndex)
{
	INT iREMAIN_MINUTE = 0;

	if (this->GetCurrentState(iBridgeIndex) == BLOODCASTLE_STATE_CLOSED) {
		iREMAIN_MINUTE = (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60 * 1000) - m_iBC_TIME_MIN_OPEN) + 1;
	}
	else {
		struct tm * today;
		time_t ltime;

		time( &ltime );
		today = localtime( &ltime );

		switch(g_iBloodCastle_StartHour) {
		case 2 :	// 2시간마다 매 짝수시 30분
			{
#ifdef MODIFY_BLOODECASTLE_SCADULE_BUGFIX_20070710
				if( (today->tm_hour%2) == g_iBloodCastle_odd_Even_Hour)
#else
				if( !(today->tm_hour%2) )
#endif	
				{
					// 짝수시간
					if ((today->tm_min * 60 + today->tm_sec) >= 30 * 60) {
						// 30분을 넘겼다 -> 2시간 후의 30분을 기약
						iREMAIN_MINUTE = (((60*2 + 30) * 60) - (today->tm_min * 60 + today->tm_sec)) / 60;
					}
					else {
						// 30분을 아직 않넘겼다 -> 시간잡음
						iREMAIN_MINUTE  = ((30*60) - (today->tm_min * 60 + today->tm_sec)) / 60;
					}
				}
				else
				{
					// 홀수 시간대라면
					iREMAIN_MINUTE = ((30*60) - (today->tm_min * 60 + today->tm_sec)) / 60;
				}

				if( iREMAIN_MINUTE < 0 )
				{
					iREMAIN_MINUTE += 60;
				}
			}
			break;
		case 1 :	// 1시간마다 매시 30분
		default:
			{
				if ((today->tm_min * 60 + today->tm_sec) >= 30 * 60) {
					// 매시 30분을 넘겼다 -> 1시간 후의 30분을 기약
					iREMAIN_MINUTE = ((60*60) + (30*60) - (today->tm_min * 60 + today->tm_sec)) / 60;
				}
				else {
					// 매시 30분을 아직 않넘겼다 -> 시간잡음
					iREMAIN_MINUTE = ((30*60) - (today->tm_min * 60 + today->tm_sec)) / 60;
				}

				if( iREMAIN_MINUTE < 0 )
				{
					iREMAIN_MINUTE += 60;
				}
			}
			break;
		}
	}

	return iREMAIN_MINUTE;
}
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624


void CBloodCastle::ClearMonster(int iBridgeIndex, bool bClearCastleDoor)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;
	
	for( int n=0; n<MAX_MONSTER; n++)
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
		if (gObj[n].MapNumber == GetMapNumByBCBridge( iBridgeIndex ) ) {	// 만약 특정 블러드 캐슬에 있다면 상태가 어떠하든지 상관없이 다 제거
#else
		if (gObj[n].MapNumber == (MAP_INDEX_BLOODCASTLE1 + iBridgeIndex)) {	// 만약 특정 블러드 캐슬에 있다면 상태가 어떠하든지 상관없이 다 제거
#endif

			if (!bClearCastleDoor && (gObj[n].Class == 131))				// 만약에 성문을 없애지 말라고 하면 놔둔다.
				continue;
			if (gObj[n].Class == NPC_ANGELKING)										// 그래도 대천사는 뺀다
				continue;

			gObjDel(n);
		}
	}
}


void CBloodCastle::SetMonster(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {			// 인덱스를 벗어나면 무시
		return;
	}

	// 1 . 사용자 수로 우선 잡아야할 몬스터 수를 정한 후
	SetMonsterKillCount(iBridgeIndex);

	// !!> 테스트
//	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = 3;
	

	// 2 . 성문을 생성한다.
	int nCastleDoorPosNum = m_BridgeData[iBridgeIndex].m_nCastleDoorPosNum;

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 몬스터 타입 형변경			
	WORD btMonsterType = gMSetBase.m_Mp[nCastleDoorPosNum].m_Type;
#else
	BYTE btMonsterType = gMSetBase.m_Mp[nCastleDoorPosNum].m_Type;
#endif
	BYTE btMapNumber = gMSetBase.m_Mp[nCastleDoorPosNum].m_MapNumber;

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
	BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
	BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

	if (nCastleDoorPosNum != -1) {
		if (btMonsterType == 131) {											// 확실히 성문이 맞을때만 추가
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			int result = gObjAddMonster(btMapNumber);
#else
			int result = gObjAdd();
#endif
			if( result >= 0 )
			{
				gObjSetPosMonster(result, nCastleDoorPosNum);
				gObjSetMonster(result, btMonsterType);
				
				gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
				gObj[result].m_ItemRate = m_iBC_NORMAL_ITEM_DROP;
				gObj[result].Dir = 1;
				gObj[result].m_PosNum = -1;
				gObj[result].Live	= 1;
				gObj[result].DieRegen = 0;
				gObj[result].m_State	= OBJST_CREATE;
				gObj[result].MaxRegenTime = 0;
				gObj[result].MaxLife	= m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
				gObj[result].Life		= m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
				m_BridgeData[btBloodCastleIndex].m_iDoorIndex	=	result;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
				LogAddTD("[Blood Castle] (%d) Create Door Successfuly : Index(%d)", btBloodCastleIndex + 1, result);
#else
				LogAddTD("[Blood Castle] (%d) Create Door Successfuly : Index(%d)", btMapNumber - MAP_INDEX_BLOODCASTLE1+1, result);
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
#endif
			}
#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
			else
			{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
				LogAddTD("[Blood Castle] (%d) Create Door Fail", btBloodCastleIndex + 1);
#else
				LogAddTD("[Blood Castle] (%d) Create Door Fail", btMapNumber - MAP_INDEX_BLOODCASTLE1+1);
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
			}
#endif
		}
		else {
			// 성문이 아니다. (심각한 에러)
#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
			LogAddTD("[Blood Castle] (%d) Door monster type Error", btBloodCastleIndex + 1);
#else
			LogAddTD("[Blood Castle] (%d) Door monster type Error", btMapNumber - MAP_INDEX_BLOODCASTLE1+1);
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
#endif	
		}
	}


	// 3 . 일반 몬스터들을 추가한다.
	int result;	
	for( int n=0; n<gMSetBase.m_Count; n++)
	{
		if( CHECK_BLOODCASTLE(gMSetBase.m_Mp[n].m_MapNumber) )		// 블러드 캐슬에 들어가는 몬스터만
		{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 몬스터 타입 형변경			
			WORD btMonsterType = gMSetBase.m_Mp[n].m_Type;
#else
			BYTE btMonsterType = gMSetBase.m_Mp[n].m_Type;
#endif
			BYTE btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
			BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
			BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

			if (btBloodCastleIndex != iBridgeIndex)					// 생성되어야 할 맵이 아니면 무시					
				continue;

			if (btMonsterType == NPC_ANGELKING)						// 대천사는 미리 추가되므로 여기서 새로 추가하지 않는다.
				continue;

			if (btMonsterType == 131)								// 성문도 미리 추가되었으므로 여기서 새로 추가하면 않된다.
				continue;

			// 보스로 정해진 몹들도 여기서 추가하지 않는다.
			if ((btMonsterType == 89) ||							// 마법해골1							
				(btMonsterType == 95) ||							// 마법해골2
				(btMonsterType == 112) ||							// 마법해골3
				(btMonsterType == 118) ||							// 마법해골4
				(btMonsterType == 124) ||							// 마법해골5
				(btMonsterType == 130)								// 마법해골6
#ifdef BLOODCASTLE_EXTEND_20040314
				|| (btMonsterType == 143)							// 마법해골7
#endif	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// 블러드캐슬8 보스몬스터 설정 제외
				|| (btMonsterType == 433)							// 마법해골8
#endif				
				)
				continue;

			if (CHECK_LIMIT(btMonsterType-BC_SAINT_STATUE_1, 3))	// 성자의 석상은 성문이 부서진후 보스몹과 함께 추가되므로 여기서 추가하지 않는다.
				continue;

//			if (MAP_ATTR_NONE != MapC[gObj[result].MapNumber].GetAttr(gObj[result].X, gObj[result].Y))
//				continue;

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);
#else
			result = gObjAdd();
#endif
			if( result >= 0 )
			{
				gObj[result].m_PosNum   = n;
				gObj[result].X			= gMSetBase.m_Mp[n].m_X;
				gObj[result].Y			= gMSetBase.m_Mp[n].m_Y;
				gObj[result].MapNumber	= gMSetBase.m_Mp[n].m_MapNumber;
				gObj[result].TX			= gObj[result].X;
				gObj[result].TY			= gObj[result].Y;
				gObj[result].m_OldX		= gObj[result].X;
				gObj[result].m_OldY		= gObj[result].Y;
				gObj[result].Dir		= gMSetBase.m_Mp[n].m_Dir;
				gObj[result].StartX     = (BYTE)gObj[result].X;
				gObj[result].StartY     = (BYTE)gObj[result].Y;
/*
				LogAddTD ("[BTEST] Pos:%d, X:%d, Y:%d, Type:%d, MapAttr:%d",
					n,
					gObj[result].X,
					gObj[result].Y,
					gObj[result].Class,
					MapC[gObj[result].MapNumber].GetAttr(gObj[result].X, gObj[result].Y)
					);
*/
				gObjSetMonster(result, btMonsterType);

				gObj[result].MaxRegenTime = m_iBC_MONSTER_REGEN;
				gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
//				gObj[result].m_ItemRate = m_iBC_NORMAL_ITEM_DROP;
				gObj[result].Dir = rand()%8;					// 원래 Dir에는 다리번호만 있었으므로 여기에서 실제 방향을 준다.
			}
		}
	}
}


void CBloodCastle::SetBossMonster(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {		// 인덱스를 벗어나면 무시
		return;
	}

	// * . 보스몹들을 추가한다.
	int result;
	for (int x = 0 ; x < MAX_BLOODCASTLE_BOSS_COUNT ; x++) {
		if (m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x] != -1) {
			INT iBossPosNum = m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x];
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 몬스터 타입 형변경			
			WORD btMonsterType = gMSetBase.m_Mp[iBossPosNum].m_Type;
#else
			BYTE btMonsterType = gMSetBase.m_Mp[iBossPosNum].m_Type;
#endif
			BYTE btMapNumber = gMSetBase.m_Mp[iBossPosNum].m_MapNumber;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
			BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
			BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

			if (btBloodCastleIndex != iBridgeIndex)				// 생성되어야 할 맵이 아니면 무시					
				continue;

			// 확실히 보스몹이 맞을때만 추가
			if ((btMonsterType == 89) ||							// 마법해골1						
				(btMonsterType == 95) ||							// 마법해골2
				(btMonsterType == 112) ||							// 마법해골3
				(btMonsterType == 118) ||							// 마법해골4
				(btMonsterType == 124) ||							// 마법해골5
				(btMonsterType == 130)								// 마법해골6
#ifdef BLOODCASTLE_EXTEND_20040314
				|| (btMonsterType == 143)							// 마법해골7
#endif	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// 블러드캐슬8 보스몬스터 추가
				|| (btMonsterType == 433)							// 마법해골8
#endif				
				)
			{
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
				result = gObjAddMonster(btMapNumber);
#else
				result = gObjAdd();
#endif
				if( result >= 0 )
				{
					gObjSetPosMonster(result, iBossPosNum);
					gObjSetMonster(result, btMonsterType);

					gObj[result].MaxRegenTime = m_iBC_MONSTER_REGEN;
					gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
					gObj[result].m_ItemRate = m_iBC_NORMAL_ITEM_DROP;
					gObj[result].Dir = rand()%8;				// 원래 Dir에는 다리번호만 있었으므로 여기에서 실제 방향을 준다.
				}
			}
		}
	}
}


void CBloodCastle::SetSaintStatue(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {		// 인덱스를 벗어나면 무시
		return;
	}

	// * . 성자의석상을 우선 추가한다.
	int nSaintStatuePosNum = m_BridgeData[iBridgeIndex].m_nSaintStatuePosNum;

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 몬스터 타입 형변경			
	WORD btMonsterType = gMSetBase.m_Mp[nSaintStatuePosNum].m_Type;
#else
	BYTE btMonsterType = gMSetBase.m_Mp[nSaintStatuePosNum].m_Type;
#endif
	BYTE btMapNumber = gMSetBase.m_Mp[nSaintStatuePosNum].m_MapNumber;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
	BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
	BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

	if (nSaintStatuePosNum != -1) {
		if (CHECK_LIMIT(btMonsterType-BC_SAINT_STATUE_1, 3)) {			// 확실히 성자의 석상의 맞을때만 추가
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			int result = gObjAddMonster(btMapNumber);
#else
			int result = gObjAdd();
#endif
			if( result >= 0 )
			{
				gObjSetPosMonster(result, nSaintStatuePosNum);
				gObjSetMonster(result, btMonsterType);
				
				gObj[result].Class = BC_SAINT_STATUE_1 + rand()%3;
				gObj[result].m_cBloodCastleIndex	= btBloodCastleIndex;
				gObj[result].m_ItemRate		= m_iBC_NORMAL_ITEM_DROP;
				gObj[result].Dir			= 1;
				gObj[result].m_PosNum		= -1;
				gObj[result].Live			= 1;
				gObj[result].DieRegen		= 0;
				gObj[result].m_State		= OBJST_CREATE;
				gObj[result].MaxRegenTime	= 0;
				gObj[result].MaxLife		= m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
				gObj[result].Life			= m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;

#ifdef EXTEND_LOG_SYSTEM_05_20060823
				LogAddTD("[Blood Castle] (%d) Saint Status be created.", iBridgeIndex+1 );
#endif // EXTEND_LOG_SYSTEM_05_20060823
			}
		}
		else {
			// 성자의 석상이 아니다. (심각한 에러)
#ifdef ADD_LOG_CHECK_CAHOSCASTLE_BLOODCASTLE_EVENT_20060314
			LogAddTD( "[Blood Castle][Bug Tracer] (%d) It's not Saint Statue( Map:%d, BloodCastleIndex:%d, Type:%d )"
				, iBridgeIndex + 1
				, btMapNumber
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
				, GetBridgeIndexByMapNum( btMapNumber )
#else
				, gMSetBase.m_Mp[nSaintStatuePosNum].m_MapNumber - MAP_INDEX_BLOODCASTLE1
#endif
				, btMonsterType 
				);
#endif			
		}
	}
}




int CBloodCastle::LeaveUserBridge (int iBridgeIndex, int iBridgeSubIndex, int iUserIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

	if (!CHECK_LIMIT(iBridgeSubIndex, MAX_BLOODCASTLE_USER_COUNT))
		return -1;

	INT iRET_VAL = -1;

	EnterCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);

	if (m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex == iUserIndex) {
		iRET_VAL = iUserIndex;
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex = -1;
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iEXP = 0;
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iScore = 0;
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_ALIVE;
	}

	LeaveCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}


int	CBloodCastle::EnterUserBridge (int iBridgeIndex, int iUserIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
	if (!gObjIsConnected(iUserIndex))
		return -1;
#endif

	INT iRET_VAL = -1;

	EnterCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);
	
	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			// 사용자의 중복 가입을 막는다.
			if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iUserIndex) {
				iRET_VAL = i;
				break;
			}
		}

		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1) {
			iRET_VAL = i;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = iUserIndex;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_ALIVE;
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
			memcpy (m_BridgeData[iBridgeIndex].m_UserData[i].m_szAccountID, gObj[iUserIndex].AccountID, MAX_IDSTRING);
			memcpy (m_BridgeData[iBridgeIndex].m_UserData[i].m_szCharName, gObj[iUserIndex].Name, MAX_IDSTRING);
			m_BridgeData[iBridgeIndex].m_UserData[i].m_szAccountID[MAX_IDSTRING] = 0;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_szCharName[MAX_IDSTRING] = 0;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iCharClass = gObj[iUserIndex].Class;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iBloodCastleIndex = iBridgeIndex;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iBloodCastleSubIndex = i;
#endif
#ifdef BLOODCASTLE_EVENT_5TH_20050531
			gObj[iUserIndex].m_bBloodCastleComplete = false;
#endif
			break;
		}
	}

	LeaveCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);
	
	return iRET_VAL;
}


#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
int	CBloodCastle::LevelUp (int iIndex, int iAddExp, int iEventType)
#else
int	CBloodCastle::LevelUp (int iIndex, int iAddExp)
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 0;

	INT iLEFT_EXP = 0;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 블러드캐슬 MasterLevelUp 처리
	// 마스터레벨은 이쪽으로.
	if( g_MasterLevelSystem.MasterLevelUp( &gObj[iIndex], iAddExp, true ) )
		return 0;

	// 펫 아이템 경험치 분배	
	gObjSetExpPetItem( iIndex, iAddExp );
#endif // MASTER_LEVEL_UP_SYSTEM_20070912	

#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %uz %d MonsterIndex : %d, EventType : %d", 
		gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].X,
		gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level, gObj[iIndex].Experience, iAddExp,
		0, iEventType );
#endif
	
	// 최대 레벨을 MAX_LEVEL로 한다.
	if( gObj[iIndex].Level >= g_GlobalConfig.m_iMaxLevel )
	{
		GCServerMsgStringSend(lMsg.Get(1136), gObj[iIndex].m_Index, 1);
		return 0;
	}
	
	if( gObj[iIndex].Experience + iAddExp < gObj[iIndex].NextExp ) {
		gObj[iIndex].Experience += iAddExp;
	}
	else {
		iLEFT_EXP = gObj[iIndex].Experience + iAddExp - gObj[iIndex].NextExp;

		gObj[iIndex].Experience = gObj[iIndex].NextExp;
		gObj[iIndex].Level += 1;
#ifdef DARKLORD_WORK
		if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA)
#else
		if(gObj[iIndex].Class == CLASS_MAGUMSA )
#endif
    		gObj[iIndex].LevelUpPoint += g_GlobalConfig.m_iSpecial_Points;
		else
    		gObj[iIndex].LevelUpPoint += g_GlobalConfig.m_iNormal_Points;
		
#ifdef SECOND_QUEST	
		if( gObj[iIndex].PlusStatQuestClear )
		{	// +1스탯업 퀘스트 성공시 추가 스탯 
			gObj[iIndex].LevelUpPoint += g_GlobalConfig.m_iPowerUP_Points;
			LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].LevelUpPoint);
		}
#endif		
			
		gObj[iIndex].MaxLife += DCInfo.DefClass[gObj[iIndex].Class].LevelLife;

		if(g_GlobalConfig.m_bLimitLife && gObj[iIndex].MaxLife > g_GlobalConfig.m_iLimitLife)
			gObj[iIndex].MaxLife = g_GlobalConfig.m_iLimitLife;

		gObj[iIndex].MaxMana += DCInfo.DefClass[gObj[iIndex].Class].LevelMana;
		gObj[iIndex].Life = gObj[iIndex].MaxLife;
		gObj[iIndex].Mana = gObj[iIndex].MaxMana;	
		gObjNextExpCal(&gObj[iIndex]);

#ifdef NEW_FORSKYLAND2
		gObjSetBP(gObj[iIndex].m_Index);
#endif
	

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
		GCLevelUpMsgSend(gObj[iIndex].m_Index);
#else
		short AddPoint		= 0;
		short MaxAddPoint	= 0;
		gObjGetStatPointState(gObj[iIndex].m_Index, AddPoint, MaxAddPoint);
		GCLevelUpMsgSend(gObj[iIndex].m_Index, gObj[iIndex].Level, gObj[iIndex].LevelUpPoint, 
						 (WORD)(gObj[iIndex].MaxLife+gObj[iIndex].AddLife), 
						 (WORD)(gObj[iIndex].MaxMana+gObj[iIndex].AddMana), 
						 gObj[iIndex].MaxBP+gObj[iIndex].AddBP,
						 AddPoint, MaxAddPoint);
#endif // ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712

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



VOID CBloodCastle::CheckUsersOnConnect (							// 카오스 캐슬에 입장한 사용자가 접속 중인지 체크
			INT iBridgeIndex
			)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;
	
	EnterCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);
	
	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			// 특정 사용자가 접속 중이 아니면 공간을 비운다.
			if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex))
			{
				m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
			}
			else {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
				if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber != GetMapNumByBCBridge( iBridgeIndex ) ) {
#else
				if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber != MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) {
#endif
					m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
				}
			}
		}
	}

	LeaveCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);
}


bool CBloodCastle::AddExperience (int iIndex, int iEXP)
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return false;

	if (!CHECK_LIMIT(gObj[iIndex].m_cBloodCastleIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	if (!CHECK_LIMIT(gObj[iIndex].m_cBloodCastleSubIndex, MAX_BLOODCASTLE_USER_COUNT))
		return false;

	if (iEXP > 0) {		// 일단 누적 경험치는 양쪽에 다 저장한다.
		m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_UserData[gObj[iIndex].m_cBloodCastleSubIndex].m_iEXP += iEXP;
		gObj[iIndex].m_iBloodCastleEXP += iEXP;
	}

	return true;
}


void CBloodCastle::BlockCastleEntrance(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = g_btCastleEntranceMapXY[iBridgeIndex][0] ; i <= g_btCastleEntranceMapXY[iBridgeIndex][2] ; i++) {
		for (INT j = g_btCastleEntranceMapXY[iBridgeIndex][1] ; j <= g_btCastleEntranceMapXY[iBridgeIndex][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
			int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
#else
			MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
#endif
		}
	}
}


void CBloodCastle::ReleaseCastleEntrance(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = g_btCastleEntranceMapXY[iBridgeIndex][0] ; i <= g_btCastleEntranceMapXY[iBridgeIndex][2] ; i++) {
		for (INT j = g_btCastleEntranceMapXY[iBridgeIndex][1] ; j <= g_btCastleEntranceMapXY[iBridgeIndex][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
			int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
#else
			MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
#endif
		}
	}
}


void CBloodCastle::BlockCastleBridge(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = g_btCastleBridgeMapXY[iBridgeIndex][0] ; i <= g_btCastleBridgeMapXY[iBridgeIndex][2] ; i++) {
		for (INT j = g_btCastleBridgeMapXY[iBridgeIndex][1] ; j <= g_btCastleBridgeMapXY[iBridgeIndex][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
			int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
#else
			MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
#endif
		}
	}
}


void CBloodCastle::ReleaseCastleBridge(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = g_btCastleBridgeMapXY[iBridgeIndex][0] ; i <= g_btCastleBridgeMapXY[iBridgeIndex][2] ; i++) 
	{
		for (INT j = g_btCastleBridgeMapXY[iBridgeIndex][1] ; j <= g_btCastleBridgeMapXY[iBridgeIndex][3] ; j++) 
		{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
			int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_HOLLOW);
#else
			MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_HOLLOW);
#endif

#ifdef EXTEND_LOG_SYSTEM_05_20060823
			LogAddTD("[Blood Castle] (%d) Castle Door Subsist", iBridgeIndex+1);
#endif // EXTEND_LOG_SYSTEM_05_20060823

#ifdef ADD_LOG_FOR_CASTLE_BUG_20060911
			LogAddTD( "[Blood Castle][Bug Tracer] ReleaseCastleBridge-> %d", iBridgeIndex+1 );
#endif
		}
	}
}


void CBloodCastle::BlockCastleDoor(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT x = 0 ; x < 3 ; x++) {
		for (INT i = g_btCastleDoorMapXY[iBridgeIndex][x][0] ; i <= g_btCastleDoorMapXY[iBridgeIndex][x][2] ; i++) {
			for (INT j = g_btCastleDoorMapXY[iBridgeIndex][x][1] ; j <= g_btCastleDoorMapXY[iBridgeIndex][x][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
				int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
				MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
#else
				MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
#endif
			}
		}
	}
}


void CBloodCastle::ReleaseCastleDoor(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT x = 0 ; x < 3 ; x++) {
		for (INT i = g_btCastleDoorMapXY[iBridgeIndex][x][0] ; i <= g_btCastleDoorMapXY[iBridgeIndex][x][2] ; i++) {
			for (INT j = g_btCastleDoorMapXY[iBridgeIndex][x][1] ; j <= g_btCastleDoorMapXY[iBridgeIndex][x][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
				int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
				MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
#else
				MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
#endif
			}
		}
	}
}


void CBloodCastle::BlockSector(
	INT iMAP_NUM,
	INT iSTART_X, 
	INT iSTART_Y, 
	INT iEND_X, 
	INT iEND_Y
	)
{
	if(!CHECK_BLOODCASTLE(iMAP_NUM))
		return;

	for (INT i = iSTART_X ; i <= iEND_X ; i++) {
		for (INT j = iSTART_Y ; j <= iEND_Y ; j++) {
			MapC[iMAP_NUM].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
		}
	}
}


void CBloodCastle::ReleaseSector(						
	INT iMAP_NUM,
	INT iSTART_X, 
	INT iSTART_Y, 
	INT iEND_X, 
	INT iEND_Y
	)
{
	if(!CHECK_BLOODCASTLE(iMAP_NUM))
		return;

	for (INT i = iSTART_X ; i <= iEND_X ; i++) {
		for (INT j = iSTART_Y ; j <= iEND_Y ; j++) {
			MapC[iMAP_NUM].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
		}
	}
}


void CBloodCastle::SendCastleEntranceBlockInfo (int iBridgeIndex, bool bLive)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	char cTEMP_BUF [256];
	LPPMSG_SETMAPATTR_COUNT lpMsg = (LPPMSG_SETMAPATTR_COUNT) cTEMP_BUF;
	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT) + sizeof(PMSG_SETMAPATTR) * 2 * 3);

	LPPMSG_SETMAPATTR lpMsgBody = (LPPMSG_SETMAPATTR) (cTEMP_BUF + sizeof(PMSG_SETMAPATTR_COUNT));

	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = MAP_ATTR_BLOCK;
	(bLive) ? lpMsg->btMapSetType = 0 : lpMsg->btMapSetType = 1;

	lpMsgBody[0].btX = g_btCastleEntranceMapXY[iBridgeIndex][0];
	lpMsgBody[0].btY = g_btCastleEntranceMapXY[iBridgeIndex][1];
	lpMsgBody[1].btX = g_btCastleEntranceMapXY[iBridgeIndex][2];
	lpMsgBody[1].btY = g_btCastleEntranceMapXY[iBridgeIndex][3];

	for (int i = MAX_MONSTER ; i < MAX_OBJECT ; i++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
		if ((gObj[i].MapNumber == iMapNum) && (gObj[i].Connected > 2)) {
#else
		if ((gObj[i].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[i].Connected > 2)) {
#endif
			DataSend (i, (unsigned char*) lpMsg, lpMsg->h.size);
		}
	}
}


void CBloodCastle::SendCastleBridgeBlockInfo (int iBridgeIndex, bool bLive)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	char cTEMP_BUF [256];
	LPPMSG_SETMAPATTR_COUNT lpMsg = (LPPMSG_SETMAPATTR_COUNT) cTEMP_BUF;
	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT) + sizeof(PMSG_SETMAPATTR) * 2 * 3);

	LPPMSG_SETMAPATTR lpMsgBody = (LPPMSG_SETMAPATTR) (cTEMP_BUF + sizeof(PMSG_SETMAPATTR_COUNT));

	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = MAP_ATTR_HOLLOW;
	(bLive) ? lpMsg->btMapSetType = 0 : lpMsg->btMapSetType = 1;

	lpMsgBody[0].btX = g_btCastleBridgeMapXY[iBridgeIndex][0];
	lpMsgBody[0].btY = g_btCastleBridgeMapXY[iBridgeIndex][1];
	lpMsgBody[1].btX = g_btCastleBridgeMapXY[iBridgeIndex][2];
	lpMsgBody[1].btY = g_btCastleBridgeMapXY[iBridgeIndex][3];

	for (int i = MAX_MONSTER ; i < MAX_OBJECT ; i++) 
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
		if ((gObj[i].MapNumber == iMapNum) && (gObj[i].Connected > 2)) 
#else
		if ((gObj[i].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[i].Connected > 2)) 
#endif
		{
			DataSend (i, (unsigned char*) lpMsg, lpMsg->h.size);

#ifdef ADD_LOG_FOR_CASTLE_BUG_20060911
			LogAddTD( "[Blood Castle][Bug Tracer] SendCastleBridgeBlockInfo-> Bridge:%d/Live:%d, (%d,%d,%d,%d)", 
				iBridgeIndex+1, bLive, lpMsgBody[0].btX, lpMsgBody[0].btY, lpMsgBody[1].btX, lpMsgBody[1].btY );
#endif
		}
	}
}


void CBloodCastle::SendCastleDoorBlockInfo (int iBridgeIndex, bool bLive)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	char cTEMP_BUF [256];
	LPPMSG_SETMAPATTR_COUNT lpMsg = (LPPMSG_SETMAPATTR_COUNT) cTEMP_BUF;
	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT) + sizeof(PMSG_SETMAPATTR) * 2 * 3);

	LPPMSG_SETMAPATTR lpMsgBody = (LPPMSG_SETMAPATTR) (cTEMP_BUF + sizeof(PMSG_SETMAPATTR_COUNT));

	lpMsg->btType = 0;
	lpMsg->btCount = 3;
	lpMsg->btMapAttr = MAP_ATTR_BLOCK;
	(bLive) ? lpMsg->btMapSetType = 0 : lpMsg->btMapSetType = 1;

	lpMsgBody[0].btX = g_btCastleDoorMapXY[iBridgeIndex][0][0];
	lpMsgBody[0].btY = g_btCastleDoorMapXY[iBridgeIndex][0][1];
	lpMsgBody[1].btX = g_btCastleDoorMapXY[iBridgeIndex][0][2];
	lpMsgBody[1].btY = g_btCastleDoorMapXY[iBridgeIndex][0][3];
	lpMsgBody[2].btX = g_btCastleDoorMapXY[iBridgeIndex][1][0];
	lpMsgBody[2].btY = g_btCastleDoorMapXY[iBridgeIndex][1][1];
	lpMsgBody[3].btX = g_btCastleDoorMapXY[iBridgeIndex][1][2];
	lpMsgBody[3].btY = g_btCastleDoorMapXY[iBridgeIndex][1][3];
	lpMsgBody[4].btX = g_btCastleDoorMapXY[iBridgeIndex][2][0];
	lpMsgBody[4].btY = g_btCastleDoorMapXY[iBridgeIndex][2][1];
	lpMsgBody[5].btX = g_btCastleDoorMapXY[iBridgeIndex][2][2];
	lpMsgBody[5].btY = g_btCastleDoorMapXY[iBridgeIndex][2][3];

	for (int i = MAX_MONSTER ; i < MAX_OBJECT ; i++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 수정
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
		if ((gObj[i].MapNumber == iMapNum) && (gObj[i].Connected > 2)) {
#else
		if ((gObj[i].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[i].Connected > 2)) {
#endif
			DataSend (i, (unsigned char*) lpMsg, lpMsg->h.size);
		}
	}
}


void CBloodCastle::SendNoticeMessage (int iBridgeIndex, char * lpszMSG)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsg( &pNotice, 0, lpszMSG );
#else
	pNotice.type = 0;
	strcpy(pNotice.Notice, lpszMSG);
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if ((m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1))
				DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		}
	}
}


void CBloodCastle::SendNoticeScore (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	PMSG_NOTICE	pNotice;
		
	pNotice.type = 0;

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if ((m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1)) {

#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1164), iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP);
#else
				wsprintf(pNotice.Notice, lMsg.Get(1164), iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP);	// "제 %d 블러드 캐슬에서의 당신의 총 획득 경험치는 %d 입니다."
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
				DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}
}


void CBloodCastle::SendNoticeState (int iBridgeIndex, int iPlayState)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

//	return;
	PMSG_STATEBLOODCASTLE pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(pMsg));
	pMsg.btPlayState		= iPlayState;
	pMsg.wRemainSec			= m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;

	if (iPlayState == BC_STATE_PLAY_BOSS) {
		pMsg.wMaxKillMonster	= m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster	= m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT;
	}
	else {
		pMsg.wMaxKillMonster	= m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster	= m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT;
	}

	pMsg.wUserHaveWeapon	= m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX;
	pMsg.btWeaponNum		= m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER + 1;	// 이렇게 해야 무기번호가 제대로 맞는다.

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if ((m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1))
				DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}


bool CBloodCastle::CheckUserBridgeMember( int iBridgeIndex,	int iIndex )
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return false;

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iIndex) {
			return true;
		}
	}

	return false;
}


int CBloodCastle::GetAliveUserTotalEXP (INT iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return 0;

	int iRET_EXP = 0;

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if ((m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber)) {
				iRET_EXP += m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP;
			}
		}
	}

	return iRET_EXP;
}


void CBloodCastle::SearchUserDeleteQuestItem (int iIndex)						// 사용자를 검색하여 퀘스트 아이템을 지운다.
{
	if (!gObjIsConnected (iIndex))
		return;

#ifdef PERSONAL_SHOP_20040113	// 블러드 캐슬에서 개인상점 영역까지 퀘스트 아이템을 검사하여 지운다.
	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
	for(int x=0; x<MAX_INVENTORY; x++)
#endif
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 19) )		// 대천사 시리즈가 존재한다면
			{
				INT iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;			// 아이템 레벨을 얻는다.

				if (CHECK_LIMIT(iITEM_LEVEL, 3)) {
					// 이것이 확실하므로 아이템을 지운다.
					gObjInventoryItemSet(iIndex, x, 0xFF);
					gObjInventoryDeleteItem(iIndex, x);
					GCInventoryItemDeleteSend(iIndex, x, 1);

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Delete Angel King's Weapon (%d) [Serial:%u]",
#else
					LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Delete Angel King's Weapon (%d) [Serial:%d]",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						gObj[iIndex].AccountID,
						gObj[iIndex].Name,
						iITEM_LEVEL,
						gObj[iIndex].pInventory[x].m_Number
						);
				}
			}
		}
	}
}


void CBloodCastle::SearchUserDropQuestItem (int iIndex)							// 사용자를 검색하여 퀘스트 아이템을 드롭시킨다.
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return;

#ifdef PERSONAL_SHOP_20040113	// 블러드 캐슬에서 개인상점 영역까지 퀘스트 아이템을 검사하여 드롭한다.
	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
	for(int x=0; x<MAX_INVENTORY; x++)
#endif
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 19) )		// 대천사 시리즈가 존재한다면
			{
				INT iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;			// 아이템 레벨을 얻는다.

				if (CHECK_LIMIT(iITEM_LEVEL, 3)) {
					// 이것이 확실하므로 아이템을 드롭한다.
					PMSG_ITEMTHROW	pDropItem;
					pDropItem.Ipos  = x;
					pDropItem.px    = (BYTE)gObj[iIndex].X;
					pDropItem.py	= (BYTE)gObj[iIndex].Y;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
					int iBridgeIndex = GetBridgeIndexByMapNum( gObj[iIndex].MapNumber );
					DropItemDirectly( iBridgeIndex, gObj[iIndex].m_Index, MAKE_ITEMNUM(13, 19), x );
#ifdef BLOODCASTLE_ITEMDROP_DIRECTLY_20040728
					DropItemDirectly(gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1, gObj[iIndex].m_Index, MAKE_ITEMNUM(13, 19), x);
#else
					CGItemDropRequest((LPPMSG_ITEMTHROW)&pDropItem, gObj[iIndex].m_Index, 1);		// 죽어서 떨군것으로 로그가 남을 것이다.
#endif
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010

					// 그리고 시리얼까지 같다면 현재 다리에 무기를 가진사람은 없는것이다.
					if (CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 수정
						INT iBridgeIndex = GetBridgeIndexByMapNum( gObj[iIndex].MapNumber );
#else
						INT iBridgeIndex = gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

						if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL != -1) {
							if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number) {
								m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = -1; 
							}
						}

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%u-%u]",
#else
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%d-%d]",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
							gObj[iIndex].AccountID,
							gObj[iIndex].Name,
							iITEM_LEVEL,
							m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL,
							gObj[iIndex].pInventory[x].m_Number
							);
					}
					else {
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%u]",
#else
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%d]",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
							gObj[iIndex].AccountID,
							gObj[iIndex].Name,
							iITEM_LEVEL,
							gObj[iIndex].pInventory[x].m_Number
							);
					}
				}
			}
		}
	}
}


void CBloodCastle::SetUserState(int iIndex, int iState)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;

	int iBridgeIndex = gObj[iIndex].m_cBloodCastleIndex;
	int iBridgeSubIndex = gObj[iIndex].m_cBloodCastleSubIndex;

	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	if (!CHECK_LIMIT(iBridgeSubIndex, MAX_BLOODCASTLE_USER_COUNT))
		return;

/*	if( (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)) || (gObj[iIndex].Connected <= 2))
	{
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_DEAD;
		return;
	}
*/

	switch (iState) {
	case BC_USER_ALIVE :			// 사실상 거의 쓰이지 않음.
		{
			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_ALIVE;
		}
		break;
	case BC_USER_DEAD :				// 퀘스트 아이템 드롭은 -> gObjUserDie() 에서
		{
			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_DEAD;
		}
		break;
	case BC_USER_WINNER :			// 승자라면 파티원들을 찾아서 BC_USER_WINNER_PARTY 상태를 준다.
		{
			if( gObj[iIndex].PartyNumber >= 0 )
			{
//				int iPartyCount = gParty.m_PartyS[gObj[iIndex].PartyNumber].Count;

				for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
					if (i == iBridgeSubIndex) {
						// 우승자는 넘어간다.
						continue;
					}
					else {
#ifdef MODIFY_BLOODCASTLE_USERINDEX_BUGFIX_20080709
						if( m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 ) continue;							
#endif	// MODIFY_BLOODCASTLE_USERINDEX_BUGFIX_20080709
						
						if (gObj[iIndex].PartyNumber == gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber) {
							// 우승자와 같은 파티원이라면 BC_USER_WINNER_PARTY 상태를 준다.
							if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber))
								m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_ALIVE;
							else
								m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
						}
					}
				}
			}

			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_WINNER;
			m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = iBridgeSubIndex;
		}
		break;
	case BC_USER_WINNER_PARTY_ALIVE :		// 사실상 거의 쓰이지 않음.
		{
			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_WINNER_PARTY_ALIVE;
		}
		break;
	case BC_USER_WINNER_PARTY_DEAD :		// 사실상 거의 쓰이지 않음.
		{
			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
		}
		break;
	}
}


void CBloodCastle::GiveReward_Win(int iIndex, int iBridgeIndex)
{
#ifdef BLOODCASTLE_EVENT_5TH_20050531		// 사용자의 상태를 최종적으로 체크 갱신해 준다.
	FixUsersPlayStateWin(iBridgeIndex);
#endif

	CHAR szNOTIFY_MSG [256];
	INT iREWARD_EXP = 0;
	INT iREWARD_ZEN = 0;
	INT iREWARD_SCR = 0;

#ifdef BLOODCASTLE_EVENT_3RD_20040401
	INT iLEFT_TIME = m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
#endif
#ifdef BLOODCASTLE_EVENT_5TH_20050531
	INT iALIVE_PARTYCOUNT = GetWinnerPartyCompleteCount(iBridgeIndex);
	INT iADD_PARTYPOINT = GetWinnerPartyCompletePoint(iBridgeIndex);

	LogAddTD("[Blood Castle] (%d) Winner Party Point - Alive Party Member : %d, Alive Party Point : %d", 
		iBridgeIndex + 1,
		iALIVE_PARTYCOUNT,
		iADD_PARTYPOINT
		);
#endif

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;
	
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	if (m_BridgeData[iBridgeIndex].m_bBC_REWARDED)
		return;

	if (gObj[iIndex].Connected > 2) {
		gObj[iIndex].Name[10] = '\0';
		wsprintf(szNOTIFY_MSG, lMsg.Get(1165), gObj[iIndex].Name);		// "%s 님에 의해 블러드캐슬 퀘스트가 성공하였습니다."
	}
	else {
		wsprintf(szNOTIFY_MSG, lMsg.Get(1166));							// "블러드캐슬 퀘스트가 성공하였습니다."
	}

	if (m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1) {			// 미션이 성공했다면
		// 살아있는 사람들의 경험치의 총합을 미리 구한다.
		INT iALIVE_USER_TOTAL_EXP = GetAliveUserTotalEXP(iBridgeIndex);
		LogAddTD("[Blood Castle] (%d) Quest Complete - All Alive User's Total EXP:%d",
			iBridgeIndex+1,
			iALIVE_USER_TOTAL_EXP
			);


		for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
			if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1)
				continue;
			
			// 접속이 않되어 있다면 보상을 못받는다.
			if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < 3) {
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
				SendRewardScore (&m_BridgeData[iBridgeIndex].m_UserData[i], g_iBC_EventScore_Fail[iBridgeIndex], 0);
#endif
				continue;
			}

			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1) || 
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1) ||
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex)
				) 
			{
				// 정보가 맞는 사용자만 체크
				continue;
			}

			// 우선 축하메시지를 보내고
			
#ifdef MODIFY_NOTICE_20040325
			PMSG_NOTICE	pNotice;
			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNOTIFY_MSG );
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_BLUE);
			TNotice::SendNoticeToUser( m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, &pNotice );
#else
			GCServerMsgStringSend(szNOTIFY_MSG, m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, 0);
#endif

			// 사용자의 성문, 석상파괴, 퀘스트 성공 여부에 따라 추가적인 경험치를 준다.
			// 1 . 성문
			INT iEXTRA_EXP = 0;
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) 
				||
				(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
				)
			{
				iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR];
#if TESTSERVER == 1 
				LogAddTD("[추가경험치 - 성공] [%s][%s] 성문파괴 : %d",
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
					g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR]
					);
#endif
			}
#ifdef FOR_BLOODCASTLE3
			else if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) {
				// 비록 직접 성문을 깨진 않았지만 살아만 있다면 경험치의 50%를 받는다.
				if (m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak)
					iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR] / 2;
			}
#endif
			// 2 . 석상
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party)
				||
				(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
				)
			{
				iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_STATUE];
#if TESTSERVER == 1 
				LogAddTD("[추가경험치 - 성공] [%s][%s] 석상파괴 : %d",
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
					g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_STATUE]
					);
#endif
			}
			// 3 . 퀘스트성공
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party)
				||
				(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index)
				)
			{
				iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_WIN_QUEST];
#if TESTSERVER == 1 
				LogAddTD("[추가경험치 - 성공] [%s][%s] 퀘스트성공 : %d",
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
					g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_WIN_QUEST]
					);
#endif
			}


			// 이전까지로 해서 모든 상태가 지정되었으므로 여기서는 루프를 돌면서 보상만 하면 된다.
			switch (m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState) {
			case BC_USER_ALIVE :		
				{
#ifdef FOR_BLOODCASTLE2	
					// 퀘스트 완료자 추가 경험치
	#if TESTSERVER == 1 
					LogAddTD("[적용전] 퀘스트 완료(완료자) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[적용후] 퀘스트 완료(완료자) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// 경험치 증가
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_ALIVE] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// 젠 증가
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_LOSER]);
					// 점수 증가
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_ALIVE];

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "경험치 %d 와 %d 젠, %d 포인트를 추가로 획득하였습니다."
						iREWARD_EXP, 
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/
					// 다리 점수를 랭킹서버에 누적
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531


#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_ALIVE,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			case BC_USER_DEAD :				
				{
#ifdef FOR_BLOODCASTLE2	
					// 퀘스트 완료자 추가 경험치
	#if TESTSERVER == 1 
					LogAddTD("[적용전] 퀘스트 완료(완료자) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[적용후] 퀘스트 완료(완료자) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// 경험치 증가
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_LOSER] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// 젠 증가
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_LOSER]);
					// 점수 증가
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_DEAD];

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "경험치 %d 와 %d 젠, %d 포인트를 추가로 획득하였습니다."
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/
					// 다리 점수를 랭킹서버에 누적
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531


#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_DEAD,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			case BC_USER_WINNER :			
				{
#ifdef FOR_BLOODCASTLE2	
					// 퀘스트 완료자 추가 경험치
	#if TESTSERVER == 1 
					LogAddTD("[적용전] 퀘스트 완료(완료자) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[적용후] 퀘스트 완료(완료자) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// 경험치 증가
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_WINNER] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// 젠 증가
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_WINNER]);
					// 점수 증가
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_WINNER];

#ifdef BLOODCASTLE_EVENT_5TH_20050531
					iREWARD_SCR += iADD_PARTYPOINT;
#endif

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "경험치 %d 와 %d 젠, %d 포인트를 추가로 획득하였습니다."
						iREWARD_EXP, 
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/					
					// 혼석 떨굼 (iLootTime = 5 * 60 * 1000)
					if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber)) {
						// 성공한 사람도 하나만 받는다.
#ifdef FOR_CHINA		// 중국은 하나 더 준다
						DropChaosGem(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex,iBridgeIndex);
#endif						
						DropChaosGem(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex,iBridgeIndex);
					}

					// 다리 점수를 랭킹서버에 누적
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531


#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_WINNER,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			case BC_USER_WINNER_PARTY_ALIVE :
				{
#ifdef FOR_BLOODCASTLE2	
					// 퀘스트 완료자 추가 경험치
	#if TESTSERVER == 1 
					LogAddTD("[적용전] 퀘스트 완료(완료자) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[적용후] 퀘스트 완료(완료자) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// 경험치 증가
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_WINNER] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// 젠 증가
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_WINNER]);
					// 점수 증가
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_WINNER_PARTY_ALIVE];

#ifdef BLOODCASTLE_EVENT_5TH_20050531
					iREWARD_SCR += iADD_PARTYPOINT;
#endif

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "경험치 %d 와 %d 젠, %d 포인트를 추가로 획득하였습니다."
						iREWARD_EXP, 
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/					
					// 혼석 떨굼 (iLootTime = 5 * 60 * 1000)
					if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber))
						DropChaosGem(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex,iBridgeIndex);

					// 다리 점수를 랭킹서버에 누적
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531


#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_WINNER_PARTY_ALIVE,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			case BC_USER_WINNER_PARTY_DEAD :		
				{
#ifdef FOR_BLOODCASTLE2	
					// 퀘스트 완료자 추가 경험치
	#if TESTSERVER == 1 
					LogAddTD("[적용전] 퀘스트 완료(완료자) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[적용후] 퀘스트 완료(완료자) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// 경험치 증가
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_WINNER] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// 젠 증가
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_WINNER]);
					// 점수 증가
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_WINNER_PARTY_DEAD];

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "경험치 %d 와 %d 젠, %d 포인트를 추가로 획득하였습니다."
						iREWARD_EXP, 
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/					
					// 다리 점수를 랭킹서버에 누적
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531

#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_WINNER_PARTY_DEAD,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			}

			// 획득한 경험치와 젠을 알려준다.
			PMSG_DEVILSQUARERESULT pMsg;
			pMsg.MyRank = 1;	// 성공
			pMsg.Count = 255;
			memcpy(pMsg.Score[0].Name, gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, sizeof(pMsg.Score[0].Name));


			pMsg.Score[0].BonusExp		= iREWARD_EXP;


			pMsg.Score[0].BonusZen		= iREWARD_ZEN;
			pMsg.Score[0].TotalScore	= iREWARD_SCR;
			PHeadSetB((LPBYTE)&pMsg, 0x93, sizeof(PBMSG_HEAD) + 2 + sizeof(DevilSquareScoreInfo) * 2);

			DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
//			SendBridgeAnyMsg((LPBYTE)&pMsg, pMsg.h.size, iBridgeIndex);
		}

		// 이미 보상이 이루어졌음을 기록한다.
		m_BridgeData[iBridgeIndex].m_bBC_REWARDED = true;
	}
}


void CBloodCastle::GiveReward_Fail(int iBridgeIndex)
{
#ifdef BLOODCASTLE_EVENT_5TH_20050531		// 사용자의 상태를 최종적으로 체크 갱신해 준다.
	FixUsersPlayStateFail(iBridgeIndex);
#endif

	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	INT iUserWhoGotUltimateWeapon = -1;
	if ( (iUserWhoGotUltimateWeapon = GetWhoGotUltimateWeapon(iBridgeIndex)) != -1 ) {
		if (CHECK_LIMIT(iUserWhoGotUltimateWeapon, MAX_OBJECT)) {
			LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) got Ultimate Weapon (%d) but Failed - Time Out", 
				iBridgeIndex+1,
				gObj[iUserWhoGotUltimateWeapon].AccountID,
				gObj[iUserWhoGotUltimateWeapon].Name,
				m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER				
				);
		}
	}

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1)
			continue;

		// 접속이 않되어 있다면 보상을 못받는다.
		if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < 3) {
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
			SendRewardScore (&m_BridgeData[iBridgeIndex].m_UserData[i], g_iBC_EventScore_Fail[iBridgeIndex], 0);
#endif
			continue;
		}

		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1) || (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1))
			continue;

		// 사용자의 성문, 석상파괴 여부에 따라 추가적인 경험치를 준다.
		// 1 . 성문
		INT iEXTRA_EXP = 0;
		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
			(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party)
			||
			(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
			)
		{
			iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR];
#if TESTSERVER == 1
			LogAddTD("[추가경험치 - 실패] [%s][%s] 성문파괴 : %d",
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
				g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR]
				);
#endif
		}
#ifdef FOR_BLOODCASTLE3
		else if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) {
			// 비록 직접 성문을 깨진 않았지만 살아만 있다면 경험치의 50%를 받는다.
			if (m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak)
				iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR] / 2;
		}
#endif

		// 2 . 석상
		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
			(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party)
			||
			(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
			)
		{
			iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_STATUE];
#if TESTSERVER == 1 
			LogAddTD("[추가경험치 - 실패] [%s][%s] 석상파괴 : %d",
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
				g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_STATUE]
				);
#endif
		}
		
		// 경험치 증가
		CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);

		// 다리 점수를 랭킹서버에 누적
#ifdef BLOODCASTLE_EVENT_5TH_20050531
		SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iBC_EventScore_Fail[iBridgeIndex], 0, 0);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
		SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iBC_EventScore_Fail[iBridgeIndex], 0);
#else
		SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iBC_EventScore_Fail[iBridgeIndex]);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531

#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
		LogAddTD("[Blood Castle] (%d) Quest Fail Rewarded (Account:%s, Name:%s, Score:%d, ExtEXP:%d, LeftTime:%d)", 
#else
		LogAddTD("[Blood Castle] (%d) Quest Fail Rewarded (Account:%s, Name:%s, Score:%d, ExtEXP:%d)",
#endif
			iBridgeIndex+1,
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
			g_iBC_EventScore_Fail[iBridgeIndex],
			iEXTRA_EXP
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// 타임어택 관련 로그추가
			,0
#endif
			);


		// 획득한 경험치와 젠을 알려준다.
		PMSG_DEVILSQUARERESULT pMsg;
		pMsg.MyRank = 0;	// 실패
		pMsg.Count = 255;
		memcpy(pMsg.Score[0].Name, gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, sizeof(pMsg.Score[0].Name));
		pMsg.Score[0].BonusExp		= iEXTRA_EXP;
		pMsg.Score[0].BonusZen		= 0;
		pMsg.Score[0].TotalScore	= g_iBC_EventScore_Fail[iBridgeIndex];
		PHeadSetB((LPBYTE)&pMsg, 0x93, sizeof(PBMSG_HEAD) + 2 + sizeof(DevilSquareScoreInfo) * 2);

		// 다리의 정식 등록자를 기준으로 블러드 캐슬맵에 없어도 살아만 있다면 메시지를 보내준다.
		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1)) {
			// 다만 재접한 사람이 인덱스가 겹치는 경우 제외
			DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			LogAddTD("[Blood Castle] (%d) Send User Quest Fail Message (Account:%s, Name:%s [State:%d], MapNumber:%d)",
				iBridgeIndex+1,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
				m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
				);
		}

//		SendBridgeAnyMsg((LPBYTE)&pMsg, pMsg.h.size, iBridgeIndex);
	}	
}


int CBloodCastle::CalcSendRewardEXP(int iIndex, int iEXP) 
{
	if (iEXP <= 0)
		return 0;

#ifdef BUGFIX_GAIN_EXP_20071210
	INT64 iRET_EXP = 0;
	INT64 iCAL_EXP = iEXP;
#else
	INT iRET_EXP = 0;
	INT iCAL_EXP = iEXP;
#endif

#ifdef MU_CRYWOLF_PENALTY_20051215
	// MVP 실패 : 경험치 획득 감소 
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
		&& g_iCrywolfApplyMvpPenalty	
	  )
	{
		iCAL_EXP = iCAL_EXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
	}
#endif
	
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
		#ifdef MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
	if( gObj[iIndex].m_wExprienceRate == 0 )
	{
		iEXP = 0;
		iCAL_EXP = 0;
	}
		#else
	if( gObj[iIndex].m_wExprienceRate > 0 )
	{
		iEXP = (int)(iEXP * ((float)gObj[iIndex].m_wExprienceRate / 100 ));
	}
		#endif // MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return 0;

	if (gObj[iIndex].Connected < 3)
		return 0;

	iRET_EXP = iCAL_EXP;

	if( gObj[iIndex].Type == OBJTYPE_CHARACTER ) {
		while (iCAL_EXP > 0) {
			if (iCAL_EXP > 0) {
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// 중국변경> . 전체 몬스터 아이템 드롭 확률을 조정함.
				if (g_bChnPlayTimeLimitOn)
				{
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
					if( gObj[iIndex].m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
					{
						if (gObj[iIndex].m_iPlayTimeLimit >= 5*60*60) {
						iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel2Exp / 100;
						}
						else if (gObj[iIndex].m_iPlayTimeLimit >= 3*60*60) {
							iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel1Exp / 100;
						}
					}
	#else
					if (gObj[iIndex].m_iPlayTimeLimit >= 5*60*60) {
						iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel2Exp / 100;
					}
					else if (gObj[iIndex].m_iPlayTimeLimit >= 3*60*60) {
						iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel1Exp / 100;
					}
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
				}
#endif
				
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626 // 베트남 : 블러드캐슬 경험치 획득량 조정
				if (g_bVtmPlayTimeLimitOn) 
				{
					if (gObj[iIndex].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time * 60 ) 
					{
						iCAL_EXP = iCAL_EXP * g_iVtmPlayTimeLimitLevel2Exp / 100;
					}
					else if (gObj[iIndex].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time * 60) 
					{
						iCAL_EXP = iCAL_EXP * g_iVtmPlayTimeLimitLevel1Exp / 100;
					}
				}
#endif

#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
				CheckItemOptForGetExpEx(&gObj[iIndex], iCAL_EXP, TRUE);
				
				// 계산이 끝난 보상 경험치에 경험치 비율을 적용시킴
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_fRewardExpRate;
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				
				iRET_EXP = (int)iCAL_EXP;
#else // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC 적용 후 아래코드 삭제할 예정
#ifdef MODIFY_EVENTMAP_REWARD_BUGFIX_20080404	// 보상경험치에 유지의 인장만 적용
				if( gObjCheckUsedBuffEffect( &gObj[iIndex], BUFFTYPE_PCBANG_POINT_MARK3 ))
				{
					iCAL_EXP = 0;
				}
				
		#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_fRewardExpRate;
		#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				
				iRET_EXP = iCAL_EXP;
#else	// MODIFY_EVENTMAP_REWARD_BUGFIX_20080404				
#ifdef PCBANG_POINT_SYSTEM_20070206	// 블러드캐슬 - 경험치 조절
				g_PCBangPointSystem.CheckItemOptForGetExp( &gObj[iIndex], (int)iCAL_EXP );
#endif
#endif // MODIFY_EVENTMAP_REWARD_BUGFIX_20080404

		// 계산이 끝난 보상 경험치에 경험치 비율을 적용시킴
		#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_fRewardExpRate;
		#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

#ifdef MODIFY_MARKITEM_20080506
	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
				CheckItemOptForGetExp(&gObj[iIndex], iCAL_EXP, TRUE);
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
#endif // MODIFY_MARKITEM_20080506
				
		#ifdef BUGFIX_GAIN_EXP_20071210				
				iRET_EXP = iCAL_EXP;
		#else
				iRET_EXP = (int)iCAL_EXP;
		#endif
//<--
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#ifndef MODIFY_MARKITEM_20080506
	//버그 픽스후 삭제 예정 riverstyx
	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
				CheckItemOptForGetExp(&gObj[iIndex], iCAL_EXP, TRUE);
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
#endif // MODIFY_MARKITEM_20080506

	#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
				iCAL_EXP = LevelUp( iIndex, iCAL_EXP, EVENT_TYPE_BLOODCASTLE );
	#else
				iCAL_EXP = LevelUp( iIndex, iCAL_EXP );
	#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !!NOT
	#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
		#ifdef ADD_PCS_MARK_OF_EXP_20070205
				if( gObj[iIndex].m_wExprienceRate > 0 )
				{
					iCAL_EXP = LevelUp(iIndex, iCAL_EXP, EVENT_TYPE_BLOODCASTLE );
				}
				else
				{
					iEXP = 0;
					iCAL_EXP = 0;
					iRET_EXP = 0;
				}
		#else
				iCAL_EXP = LevelUp(iIndex, iCAL_EXP, EVENT_TYPE_BLOODCASTLE );
		#endif // ADD_PCS_MARK_OF_EXP_20070205
	#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			}
		}

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	#if TESTSERVER == 1 
			LogAddTD("[Blood Castle] [%s][%s] CBloodCastle::CalcSendRewardEXP() - EXP : %d", 
				gObj[iIndex].AccountID,
				gObj[iIndex].Name,
				iRET_EXP
				);
	#endif
		
		// 사용자에게 보상경험치를 보낸다.
		// *마스터레벨 유저의 경험치는 이미 보내졌다.
		if( !g_MasterLevelSystem.IsMasterLevelUser( &gObj[iIndex] ) )
		{
	#ifdef BUGFIX_GAIN_EXP_20071210
			GCSendExp_INT64( iIndex, 0xFFFF, iRET_EXP, 0, 0 );
	#endif
		}
#else
		PMSG_KILLPLAYER_EXT	pkillMsg;

		PHeadSetBE((LPBYTE)&pkillMsg, 0x9C, sizeof( pkillMsg));		// 원래 0x16 이었으나 INT로 확장하기 위해 0x9C를 쓴다.

		pkillMsg.NumberH	= HIBYTE( 0xFFFF );
		pkillMsg.NumberL	= LOBYTE( 0xFFFF );
		pkillMsg.ExpH		= HIWORD( iRET_EXP ) ;
		pkillMsg.ExpL		= LOWORD( iRET_EXP ) ;
		pkillMsg.DamageH    = HIBYTE( 0 );
		pkillMsg.DamageL    = LOBYTE( 0 );

#if TESTSERVER == 1 
		LogAddTD("[Blood Castle] [%s][%s] CBloodCastle::CalcSendRewardEXP() - EXP : %d", 
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			iRET_EXP
			);
#endif

		// 사용자에게 보상경험치를 보낸다.
		DataSend(iIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);
#endif
	}

	
	return iRET_EXP;
}


int CBloodCastle::CalcSendRewardZEN (int iIndex, int iZEN)
{
	if (iZEN <= 0)
		return 0;

	INT iRET_ZEN = 0;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return 0;

	if (gObj[iIndex].Connected < 3)
		return 0;

#ifdef MODIFY_ZEN_MAX_20040414
	if( !gObjCheckMaxZen( iIndex, iZEN ) )
	{
		iRET_ZEN = MAX_ZEN - gObj[iIndex].Money;
		gObj[iIndex].Money += iRET_ZEN;
		return iRET_ZEN;
	}
	else
#endif
	gObj[iIndex].Money += iZEN;

	iRET_ZEN = iZEN;

#if TESTSERVER == 1 
	LogAddTD("[Blood Castle] [%s][%s] CBloodCastle::CalcSendRewardZEN() - ZEN : %d", 
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		iZEN
		);
#endif

	GCMoneySend(iIndex, gObj[iIndex].Money);

	return iRET_ZEN;
}


void CBloodCastle::DropChaosGem (int iIndex, int iBridgeNum)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;

	int iMaxHitUser = iIndex;

	int drop_count = 0, used_count = 0;

	int count = m_LST_BLOODCASTLE_REWARD[iBridgeNum].size();

	tdBCRewardList::iterator it = m_LST_BLOODCASTLE_REWARD[iBridgeNum].begin();

	for(std::advance(it, rand()%count);used_count < count && drop_count < m_BridgeData[iBridgeNum].m_iDropItemCount;advance(it = m_LST_BLOODCASTLE_REWARD[iBridgeNum].begin(),rand()%count),++used_count)
	{
		BLOODCASTLE_REWARD &reward = (*it);

		if((rand()%1000) < reward.max_drop_rate)
		{
			drop_count++;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// BC_MAPNUMBER_BASE_CHAOSGEM 삭제
			int iChaosGemMapNum = -1;
			switch( gObj[iIndex].MapNumber )
			{
			case MAP_INDEX_BLOODCASTLE1: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM1; break;
			case MAP_INDEX_BLOODCASTLE2: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM2; break;
			case MAP_INDEX_BLOODCASTLE3: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM3; break;
			case MAP_INDEX_BLOODCASTLE4: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM4; break;
			case MAP_INDEX_BLOODCASTLE5: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM5; break;
			case MAP_INDEX_BLOODCASTLE6: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM6; break;
			case MAP_INDEX_BLOODCASTLE7: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM7; break;
			case MAP_INDEX_BLOODCASTLE8: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM8; break;
			}

			ItemSerialCreateSend(gObj[iIndex].m_Index, iChaosGemMapNum, (BYTE)gObj[iIndex].X, (BYTE)gObj[iIndex].Y, reward.item_type, reward.item_level, (BYTE)ItemAttribute[reward.item_type].Durability, reward.is_skill, reward.is_luck, reward.option, iMaxHitUser, reward.exc_option);
#else
			ItemSerialCreateSend(gObj[iIndex].m_Index, BC_MAPNUMBER_BASE_CHAOSGEM + gObj[iIndex].MapNumber, (BYTE)gObj[iIndex].X, (BYTE)gObj[iIndex].Y, reward.item_type, reward.item_level, (BYTE)ItemAttribute[reward.item_type].Durability, reward.is_skill, reward.is_luck, reward.option, iMaxHitUser, reward.exc_option);
#endif
		}
	}
/*
	int iType = ItemGetNumberMake(12, 15);
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// BC_MAPNUMBER_BASE_CHAOSGEM 삭제
	int iChaosGemMapNum = -1;
	switch( gObj[iIndex].MapNumber )
	{
	case MAP_INDEX_BLOODCASTLE1: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM1; break;
	case MAP_INDEX_BLOODCASTLE2: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM2; break;
	case MAP_INDEX_BLOODCASTLE3: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM3; break;
	case MAP_INDEX_BLOODCASTLE4: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM4; break;
	case MAP_INDEX_BLOODCASTLE5: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM5; break;
	case MAP_INDEX_BLOODCASTLE6: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM6; break;
	case MAP_INDEX_BLOODCASTLE7: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM7; break;
	case MAP_INDEX_BLOODCASTLE8: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM8; break;
	}
	
	ItemSerialCreateSend(gObj[iIndex].m_Index, iChaosGemMapNum, (BYTE)gObj[iIndex].X, (BYTE)gObj[iIndex].Y, iType, 0, (BYTE)0, 0, 0, 0, iMaxHitUser);
#else
	ItemSerialCreateSend(gObj[iIndex].m_Index, BC_MAPNUMBER_BASE_CHAOSGEM + gObj[iIndex].MapNumber, (BYTE)gObj[iIndex].X, (BYTE)gObj[iIndex].Y, iType, 0, (BYTE)0, 0, 0, 0, iMaxHitUser);
#endif
*/
}

#ifdef BLOODCASTLE_EVENT_5TH_20050531
void CBloodCastle::SendRewardScore (INT iIndex, INT iSCORE, INT iLeftTime,	INT iAlivePartyCount)	// 랭킹서버에 보상점수를 보낸다.
#else	
#ifdef BLOODCASTLE_EVENT_3RD_20040401
void CBloodCastle::SendRewardScore (int iIndex, int iSCORE, INT iLeftTime)	// 랭킹서버에 보상점수를 보낸다.
#else
void CBloodCastle::SendRewardScore (int iIndex, int iSCORE)	// 랭킹서버에 보상점수를 보낸다.
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;

#ifdef BLOODCASTLE_EVENT_3RD_20040401
#ifdef BLOODCASTLE_EVENT_5TH_20050531
	PMSG_ANS_BLOODCASTLESCORE_5TH pMsg;	
#else
	PMSG_ANS_BLOODCASTLESCORE_3RD pMsg;
#endif
	
	pMsg.h.c		= PMHC_BYTE;

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	pMsg.h.headcode	= 0x0D;
#else	
#ifdef BLOODCASTLE_EVENT_4TH_20040531
	pMsg.h.headcode	= 0x0C;
#else
	pMsg.h.headcode	= 0x0A;
#endif	// BLOODCASTLE_EVENT_4TH_20040531
#endif	// BLOODCASTLE_EVENT_5TH_20050531
	
	pMsg.h.size		= sizeof( pMsg );

	pMsg.Score	= iSCORE;
	pMsg.BridgeNum = gObj[iIndex].m_cBloodCastleIndex;
	pMsg.Class = gObj[iIndex].Class;

	pMsg.ServerCode=gGameServerCode;
	pMsg.iLeftTime = iLeftTime;
	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    gObj[iIndex].Name, MAX_IDSTRING);

#ifndef BLOODCASTLE_EVENT_5TH_20050531
#ifdef FOR_BLOODCASLTE_PCEVENT
	pMsg.iPCRoomGUID = gObj[iIndex].PcBangGuid;
#endif	// FOR_BLOODCASLTE_PCEVENT
#endif	// BLOODCASTLE_EVENT_5TH_20050531

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	pMsg.iAlivePartyCount = iAlivePartyCount;
#endif

#else
	PMSG_ANS_BLOODCASTLESCORE pMsg;	
	
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x07;
	pMsg.h.size		= sizeof( pMsg );

	pMsg.Score	= iSCORE;
	pMsg.BridgeNum = gObj[iIndex].m_cBloodCastleIndex;
	pMsg.Class = gObj[iIndex].Class;

	pMsg.ServerCode=gGameServerCode;
	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    gObj[iIndex].Name, MAX_IDSTRING);
#ifdef FOR_BLOODCASLTE_PCEVENT
	pMsg.iPCRoomGUID = gObj[iIndex].PcBangGuid;
#endif
#endif 

	
	// 실제로 랭킹서버에 전송하는 부분
	if( !IsDevilSquareEventConnected && !DevilSquareEventConnect )
	{
		wsRServerCli.Close();
		wsRServerCli.CreateSocket(ghWnd);
		if( GMRankingServerConnect(gDevilSquareEventServerIp, WM_ASS_RANKINGCLIMSG) == FALSE )
		{
			IsDevilSquareEventConnected = 0;
			LogAddTD("Can not connect Ranking Server");
			return;
		}
		IsDevilSquareEventConnected = 1;
	}

	if( !DevilSquareEventConnect && IsDevilSquareEventConnected )
		wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}


#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604

void CBloodCastle::SendRewardScore		(											// 랭킹서버에 보상점수를 보낸다.
	LPBLOODCASTLE_USER lpBC_USER, 
	INT iSCORE,
	INT iLeftTime
	)
{

#ifdef BLOODCASTLE_EVENT_3RD_20040401
	PMSG_ANS_BLOODCASTLESCORE_3RD pMsg;	
	
	pMsg.h.c		= PMHC_BYTE;
#ifdef BLOODCASTLE_EVENT_4TH_20040531
	pMsg.h.headcode	= 0x0C;
#else
	pMsg.h.headcode	= 0x0A;
#endif
	pMsg.h.size		= sizeof( pMsg );

	pMsg.Score		= iSCORE;
	pMsg.BridgeNum	= lpBC_USER->m_iBloodCastleIndex;
	pMsg.Class		= lpBC_USER->m_iCharClass;

	pMsg.ServerCode=gGameServerCode;
	pMsg.iLeftTime = iLeftTime;
	memcpy(pMsg.AccountID, lpBC_USER->m_szAccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    lpBC_USER->m_szCharName, MAX_IDSTRING);
#ifdef FOR_BLOODCASLTE_PCEVENT
	pMsg.iPCRoomGUID = 0;
#endif
#else
	PMSG_ANS_BLOODCASTLESCORE pMsg;	
	
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x07;
	pMsg.h.size		= sizeof( pMsg );

	pMsg.Score		= iSCORE;
	pMsg.BridgeNum	= lpBC_USER->m_iBloodCastleIndex;
	pMsg.Class		= lpBC_USER->m_iCharClass;

	pMsg.ServerCode=gGameServerCode;
	memcpy(pMsg.AccountID, lpBC_USER->m_szAccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    lpBC_USER->m_szCharName, MAX_IDSTRING);
#ifdef FOR_BLOODCASLTE_PCEVENT
	pMsg.iPCRoomGUID = 0;
#endif
#endif 

	
	// 실제로 랭킹서버에 전송하는 부분
	if( !IsDevilSquareEventConnected && !DevilSquareEventConnect )
	{
		wsRServerCli.Close();
		wsRServerCli.CreateSocket(ghWnd);
		if( GMRankingServerConnect(gDevilSquareEventServerIp, WM_ASS_RANKINGCLIMSG) == FALSE )
		{
			IsDevilSquareEventConnected = 0;
			LogAddTD("Can not connect Ranking Server");
			return;
		}
		IsDevilSquareEventConnected = 1;
	}

	if( !DevilSquareEventConnect && IsDevilSquareEventConnected )
		wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
	
}

#endif


void CBloodCastle::SendBridgeAnyMsg(LPBYTE lpMsg, int iSize, int iBridgeIndex )		// 특정 다리의 사용자들에게 메시지를 보낸다.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			if(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected == 3)
			{
				// 다리의 정식 등록자를 기준으로 블러드 캐슬맵에 없어도 살아만 있다면 메시지를 보내준다.
				if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1))
					DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)lpMsg, iSize);
			}
		}
	}
}


void CBloodCastle::SendAllUserAnyMsg(LPBYTE lpMsg, int iSize)						// 접속해있는 모든 사용자들에게 메시지를 보낸다.
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}


void CBloodCastle::SetMonsterKillCount		(int iBridgeIndex)						// 특정 다리의 몬스터 제거 할당수를 지정한다.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	INT iTOT_USER_COUNT		= 0;
	INT iLIVE_USER_COUNT	= 0;
	INT iKILL_USER_COUNT	= 0;

	for (INT i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		
#ifdef FOR_ONAIR
#ifdef MODIFY_BLOODCASTLE_USERINDEX_BUGFIX_20080709
		if( m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 ) continue;
#endif	// MODIFY_BLOODCASTLE_USERINDEX_BUGFIX_20080709
		
		if( CheckAuthority(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex], AUTHORITY_ADMIN) == true)
//			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Authority&AUTHORITY_ADMIN )
		{
			continue;
		}
#endif
		
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			iTOT_USER_COUNT++;

			if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber)) {
				if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2) {
					iLIVE_USER_COUNT++;
				}
				else {
					iKILL_USER_COUNT++;
				
				}
			}
			else {
				iKILL_USER_COUNT++;
			}
		}
	}
	
//	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = (iTOT_USER_COUNT * BC_PERUSER_MONSTER_KILL) - (iKILL_USER_COUNT * BC_PERUSER_MONSTER_KILL * 8 /10);

	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = iLIVE_USER_COUNT * BC_PERUSER_MONSTER_KILL;
	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;

	LogAddTD("[Blood Castle] (%d) Monster Kill Count Set - Monster:%d, Current Monster:%d, USER TOT:%d, LIVE:%d, DEAD:%d",
		iBridgeIndex+1,
		m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT,
		m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT,
		iTOT_USER_COUNT,
		iLIVE_USER_COUNT,
		iKILL_USER_COUNT
		);
}


bool CBloodCastle::CheckMonsterKillCount	(int iBridgeIndex)		// 특정 다리의 몬스터 제거 할당수가 채워졌는지 확인한다.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

#ifdef ADD_LOG_CHECK_CAHOSCASTLE_BLOODCASTLE_EVENT_20060314
		if (m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT >= m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT)
		{
			LogAddTD( "[Blood Castle][Bug Tracer] (%d) All Monster Kill Success(Max Kill Count:%d / Kill Count:%d)"
			, iBridgeIndex + 1
			, m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT
			, m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT 
			);

			return true;
		}
#else
	if (m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT >= m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT)
		return true;
#endif

	return false;
}


bool CBloodCastle::CheckMonsterKillSuccess	(int iBridgeIndex)		// 특정 다리의 몬스터 제거 할당수가 채워졌는지 확인한다.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE;
}


bool CBloodCastle::CheckBossKillCount	(int iBridgeIndex)			// 특정 다리의 보스 몬스터 제거 할당수가 채워졌는지 확인한다.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

#ifdef ADD_LOG_CHECK_CAHOSCASTLE_BLOODCASTLE_EVENT_20060314
	if( m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT >= m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT )
	{
		LogAddTD( "[Blood Castle][Bug Tracer] (%d) All Boss Monster Kill Success(Max Boss Kill Count:%d / Boss Kill Count:%d)"
			, iBridgeIndex + 1
			, m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT
			, m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT 
			);

		return true;
	}
#else
	if (m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT >= m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT)
		return true;
#endif

	return false;
}


bool CBloodCastle::CheckBossKillSuccess	(int iBridgeIndex)		// 특정 다리의 보스 몬스터 제거 할당수가 채워졌는지 확인한다.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE;
}


bool CBloodCastle::CheckEveryUserDie	(int iBridgeIndex)			// 특정 다리의 모든 사용자가 맵에 없거나 죽었는지 확인한다.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	bool bRET_VAL = true;

	for (INT i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			if (!CHECK_LIMIT(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, MAX_OBJECT)) {
				LogAddTD("error-L3 : [Blood Castle] (%d) User Out of Bound UserIndex:%d, SubIndex:%d", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				continue;
			}

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 변경
			if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByBCBridge( iBridgeIndex ) ) {
#else
			if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) {
#endif
				if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2) {
					// 블러드맵 안에도 있고 버젓이 살아있다면 아직 다 죽지 않은 것이다.
					bRET_VAL = false;
				}
				else
				{
					LogAddTD("error-L3 : [Blood Castle] (%d) Connection Closed UserIndex:%d, SubIndex:%d", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				}
			}
			else {
				if (!m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg) {
					// 아직 종료메시지를 보내주지 않았다면 종료되었다는 메시지를 여기서 보내준다.
					m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = true;

					PMSG_STATEBLOODCASTLE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(pMsg));
					pMsg.btPlayState		= BC_STATE_END;
					pMsg.wRemainSec			= 0;
					pMsg.wMaxKillMonster	= 0;
					pMsg.wCurKillMonster	= 0;
					pMsg.wUserHaveWeapon	= 0;
					pMsg.btWeaponNum		= -1;

					DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	return bRET_VAL;
}


bool CBloodCastle::CheckAngelKingExist	(int iBridgeIndex)			// 특정 다리의 대천사가 있는지 없는지 체크 -> 없으면 만들어줌
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	bool bRET_VAL = false;

	if (m_BridgeData[iBridgeIndex].m_iAngelKingPosNum == -1) {
		// 아예 대천사가 몬스터 포지션에서 빠졌다. -> 치명적인 에러
		LogAddTD("[Blood Castle] (%d) Angel King's Monster Position Doesn't Exist", iBridgeIndex+1);
		return false;
	}

	for( int i=0 ; i<ALLOC_USEROBJECTSTART ; i++) {
		if( gObj[i].Connected == 3 && (gObj[i].Type == OBJTYPE_NPC) ) {
			if (gObj[i].Class == NPC_ANGELKING) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 변경				
				if (gObj[i].MapNumber == GetMapNumByBCBridge( iBridgeIndex ) ) {
#else
				if (gObj[i].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) {
#endif
					// 대천사가 있다면 -> 그냥 나간다.
					bRET_VAL = true;
					break;
				}
			}
		}
	}

	if (!bRET_VAL) {
		// 대천사가 없다!! -> 만든다
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 변경
		int result = gObjAddMonster( GetMapNumByBCBridge( iBridgeIndex ) );
#else
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
		int result = gObjAddMonster(MAP_INDEX_BLOODCASTLE1 + iBridgeIndex);
#else
		int result = gObjAdd();
#endif
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
		if( result >= 0 )
		{
			gObjSetPosMonster(result, m_BridgeData[iBridgeIndex].m_iAngelKingPosNum);
			gObjSetMonster(result, NPC_ANGELKING);

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 변경
			gObj[result].m_cBloodCastleIndex = GetBridgeIndexByMapNum( gObj[result].MapNumber );
#else
			gObj[result].m_cBloodCastleIndex = gObj[result].MapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

			bRET_VAL = true;
		}
		else {
			LogAddTD("[Blood Castle] (%d) Angel King - gObjAdd() failed (no space to add monster, result:%d)", iBridgeIndex+1, result);
			return false;
		}
	}

	return bRET_VAL;
}


int CBloodCastle::GetWhoGotUltimateWeapon (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

	int iBridgeUserIndex = -1;


	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		int iIndex = m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
			continue;

		if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
			continue;

		if (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber))
			continue;

#ifdef PERSONAL_SHOP_20040113	// 블러드 캐슬에서 개인상점 영역까지 검색하여 누가 절대 무기를 가졌는지 조사
		for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
		for(int x=0; x<MAX_INVENTORY; x++)
#endif
		{
			if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
			{
				if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 19) )		// 대천사 시리즈가 존재한다면
				{
					INT iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;			// 아이템 레벨을 얻는다.
					if (CHECK_LIMIT(iITEM_LEVEL, 3)) {
						if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL != -1) {
							if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number) {
								// 시리얼까지 맞다. -> 이 케릭이 대천사의 절대무기를 가지고 있는것이 확실함
								iBridgeUserIndex = iIndex;
								break;
							}
						}
					}
				}
			}
		}
	}

	return iBridgeUserIndex;
}


int CBloodCastle::GetCurrentLiveUserCount (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

	int iRetLiveUserCount = 0;


	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		int iIndex = m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
			continue;

		if (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber))
			continue;

		if (gObj[iIndex].Type != OBJTYPE_CHARACTER)
			continue;

#ifdef FOR_ONAIR
		if( gObj[iIndex].Authority&AUTHORITY_ADMIN )
		{
			continue;
		}
#endif
		if (gObj[iIndex].Connected > 2) {
			iRetLiveUserCount++;
		}
	}

	return iRetLiveUserCount;
}


#ifdef BLOODCASTLE_ITEMDROP_DIRECTLY_20040728

#include "ObjUseSkill.h"
extern CObjUseSkill gObjUseSkill;

BOOL CBloodCastle::DropItemDirectly		(										// 아이템을 조건검사 없이 바로 드롭한다. (대천사의 절대무기 드롭에만 사용)
			INT iBridgeIndex,
			INT iIndex, 
			INT iItemType,
			INT iItemPos
			)
{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 변경
	if (!CHECK_BLOODCASTLE( GetMapNumByBCBridge( iBridgeIndex ) )) {
#else
	if (!CHECK_BLOODCASTLE(MAP_INDEX_BLOODCASTLE1 + iBridgeIndex)) {
#endif
		LogAddTD("[Blood Castle] [%s][%s] DropItemDirectly() failed (iBridgeIndex wrong - %d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iBridgeIndex);
		return false;
	}

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
	
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		BYTE ItemExOption	= g_kJewelOfHarmonySystem.GetItemStrengthenOption( &gObj[iIndex].pInventory[iItemPos] );
		BYTE ItemExLevel	= g_kJewelOfHarmonySystem.GetItemOptionLevel( &gObj[iIndex].pInventory[iItemPos] );
	#endif
#endif // EXTEND_LOG_SYSTEM_03_20060816
	
	BYTE NewOption[8];
	ItemIsBufExOption(NewOption, &gObj[iIndex].pInventory[iItemPos]);

#ifdef DARKLORD_WORK
	int	 PetLevel = gObj[iIndex].pInventory[iItemPos].m_PetItem_Level;
	int	 PetExp = 	gObj[iIndex].pInventory[iItemPos].m_PetItem_Exp;
#endif

	BYTE SOption= gObj[iIndex].pInventory[iItemPos].m_SetOption;
	
#ifdef ADD_380ITEM_NEWOPTION_20060711
	#ifdef MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
		BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx;
	#else
		BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx >> 7;
	#endif // MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
#endif // ADD_380ITEM_NEWOPTION_20060711

	DWORD item_number = gObj[iIndex].pInventory[iItemPos].m_Number;
	char szItemName[50]="Item";
	
	int aAntiLootIndex = -1;					// 이렇게 떨구면 누구라도 집을 수 있다.
	
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	BYTE SocketOption[MAX_SOCKET_SLOT] = { SOCKETSLOT_NONE, SOCKETSLOT_NONE, SOCKETSLOT_NONE, SOCKETSLOT_NONE, SOCKETSLOT_NONE };
	BYTE SocketBonusOption = 0;

	g_SocketOptionSystem.GetSocketOption( &gObj[iIndex].pInventory[iItemPos], SocketOption, SocketBonusOption );
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef DARKLORD_WORK

	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		if( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx, SocketOption, SocketBonusOption ) == TRUE )
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			if( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx) == TRUE )
		#else
			if( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp) == TRUE )
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#else	// #ifdef DARKLORD_WORK


		if( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex) == TRUE )

#endif	// #ifdef DARKLORD_WORK
	{   // 아이템 버리기에 성공하면..
		gObjInventoryDeleteItem(iIndex, iItemPos);	
		pResult.Result = 0x01;

#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
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
		#ifdef MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
			ItemEffectEx>>7,
		#else
			ItemEffectEx,
		#endif// MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
			ItemExOption,
			ItemExLevel
			);
		#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#endif // ADD_380ITEM_NEWOPTION_20060711
#else // EXTEND_LOG_SYSTEM_03_20060816
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
			NewOption[0],
			NewOption[1],
			NewOption[2],
			NewOption[3],
			NewOption[4],
			NewOption[5],
			NewOption[6],
			SOption
			);
#endif // EXTEND_LOG_SYSTEM_03_20060816

	}
	else
	{
		// 버릴수 없다.
		pResult.Result = 0x00;
	}
		
	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	if( pResult.Result == 0x01 )
	{
		// 장비를 교체 했다면 
		if( iItemPos < MAX_EQUIPMENT ) 
		{
			if( iItemPos == EQUIPMENT_RING_RIGHT || iItemPos == EQUIPMENT_RING_LEFT)
			{
				gObjUseSkill.SkillChangeUse(iIndex);				
			}
			gObjMakePreviewCharSet(iIndex);
			
			PMSG_USEREQUIPMENTCHANGED	pChange;
			
			PHeadSetB((LPBYTE)&pChange, 0x25, sizeof( pChange ));

			pChange.NumberH    = HIBYTE( iIndex );
			pChange.NumberL	   = LOBYTE( iIndex );
						
			//pChange.ItemInfo[0] = (BYTE)gObj[iIndex].pInventory[iItemPos].m_Type;
			ItemByteConvert(pChange.ItemInfo, gObj[iIndex].pInventory[iItemPos]);
			pChange.ItemInfo[1] = iItemPos<<4;
			pChange.ItemInfo[1]|= LevelSmallConvert(iIndex, iItemPos)&0x0F;

			
			DataSend(iIndex, (LPBYTE)&pChange, pChange.h.size);
			MsgSendV2(&gObj[iIndex], (LPBYTE)&pChange, pChange.h.size);
		}
	}
	return pResult.Result;
}

#endif


bool CBloodCastle::CheckUserHaveUlimateWeapon (int iIndex)						// 사용자를 검색하여 절대무기를 가지고 있는지 확인
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return false;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return false;

	bool bRetVal = false;

#ifdef PERSONAL_SHOP_20040113	// 블러드 캐슬에서 개인상점 영역까지 검색하여 특정 사용자에게 절대무기가 있는지 조사
	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
	for(int x=0; x<MAX_INVENTORY; x++)
#endif
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( 
				(gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(0, 19)) ||	// 절대검
				(gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(5, 10)) ||	// 절대지팡이		
				(gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(4, 18))		// 절대석궁			
				)
			{
				// 위의 세가지 아이템 중 하나라도 가지고 있다면 참
				bRetVal = true;
				break;
			}
		}
	}

	return bRetVal;
}


#ifdef BLOODCASTLE_EVENT_5TH_20050531

bool CBloodCastle::CheckWinnerExist (								// 등록한 사용자가 있는지 체크
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerExist() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)",	iBridgeIndex + 1);
		return false;
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX != -1) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerExist() - m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX != -1",	iBridgeIndex + 1);
		return true;
	}

	return false;
}


bool CBloodCastle::CheckWinnerValid (								// 등록한 사용자가 접속을 끊거나 맵을 이탈하지 않았는지 체크
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)",	iBridgeIndex + 1);
		return false;
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1) {
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1",	iBridgeIndex + 1);
		return false;
	}

	// 접속을 끊었는지 체크
	if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)",	iBridgeIndex + 1);
		return false;
	}

	// 캐슬정보가 없는 사용자인지 체크
	if ((gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1) || 
		(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleSubIndex == -1) ||
		(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex != iBridgeIndex)
		) 
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - (gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1) ...",	iBridgeIndex + 1);
		return false;
	}

	// 캐슬맵에 존재하지 않는지 체크
	if (!CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber)) {
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber)",	iBridgeIndex + 1);
		return false;
	}

	return true;
}


bool CBloodCastle::CheckUserWinnerParty (							// 사용자가 우승자의 파티에 있는지 확인 (본인이 블러드캐슬 내에 존재해야 함)
	INT iBridgeIndex,
	INT iIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckUserWinnerParty() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)",	iBridgeIndex + 1);
		return false;
	}

	if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckUserWinnerParty() - !gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)",	iBridgeIndex + 1);
		return false;
	}
	
	if (!gObjIsConnected(iIndex)) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckUserWinnerParty() - !gObjIsConnected(iIndex)",	iBridgeIndex + 1);
		return false;
	}

	int iPartyIndex1 = gObj[iIndex].PartyNumber;
	int iPartyIndex2 = gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;

	if (CHECK_LIMIT(iPartyIndex1, MAX_PARTY) &&
		iPartyIndex1 == iPartyIndex2
		)
	{
		// 우승자와 사용자는 같은 파티이다.
		return true;
	}

	return false;
}


bool CBloodCastle::CheckPartyExist		(							// 특정 사용자의 파티원이 블러드캐슬 맵내에 존재하고 대천사와 대화를 않한 사람이 있는지 여부
	INT iIndex
	)
{
	if (gObjIsConnected(iIndex) == FALSE) {
		return false;
	}

	int iPartyIndex = gObj[iIndex].PartyNumber;
	if (!CHECK_LIMIT(iPartyIndex, MAX_PARTY)) {						// 파티가 아예 존재하지 않았음
		return false;
	}

	int iUserIndex;
	for (int iPartyUserIndex = 0; iPartyUserIndex < MAX_PARTYUSER ; iPartyUserIndex++) {
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];
		if (gObjIsConnected(iUserIndex)) {
			if (CHECK_BLOODCASTLE(gObj[iUserIndex].MapNumber) && 
				CHECK_LIMIT(gObj[iUserIndex].m_cBloodCastleIndex, MAX_BLOODCASTLE_BRIDGE_COUNT) &&
				gObj[iUserIndex].Live == 1) 
			{
				if (gObj[iUserIndex].m_bBloodCastleComplete == false) {
					// 블러드캐슬 맵안에 존재하는데도 불구하고 대화를 않했다면
					return true;
					break;
				}
			}
		}
	}
	
	return false;
}


bool CBloodCastle::CheckWinnerPartyComplete	(						// 우승자의 파티가 승리했는지 여부
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		return false;
	}

	if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {
		return false;
	}

	int iPartyIndex = gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	if (!CHECK_LIMIT(iPartyIndex, MAX_PARTY)) {						// 파티가 아예 존재하지 않았다면 성공
		return true;
	}

	int iUserIndex;
	for (int iPartyUserIndex = 0; iPartyUserIndex < MAX_PARTYUSER ; iPartyUserIndex++) {
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];
		if (gObjIsConnected(iUserIndex)) {
			if (CHECK_BLOODCASTLE(gObj[iUserIndex].MapNumber) && 
				CHECK_LIMIT(gObj[iUserIndex].m_cBloodCastleIndex, MAX_BLOODCASTLE_BRIDGE_COUNT) &&
				gObj[iUserIndex].Live == 1) 
			{
				if (gObj[iUserIndex].m_bBloodCastleComplete == false) {
					// 블러드캐슬 맵안에 존재하는데도 불구하고 대화를 않했다면
					return false;
					break;
				}
			}
		}
	}
	
	return true;
}


bool CBloodCastle::SetBridgeWinner		(							// 특정 다리의 우승자를 정한다.
	INT iBridgeIndex,
	INT iIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		return false;
	}

	if (gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {		// 이미 우승자가 존재한다면 무효
		return false;
	}
	
	if (!gObjIsConnected(iIndex)) {
		return false;
	}

	if (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)) {
		return false;
	}

	m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = iIndex;
	return true;
}


int CBloodCastle::GetWinnerPartyCompleteCount (					// 특정 다리의 우승자의 파티에 대화에 성공한 사람이 몇명인지 확인한다.
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		return 0;
	}

	if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {
		return 0;
	}

	int iPartyIndex = gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	if (!CHECK_LIMIT(iPartyIndex, MAX_PARTY)) {						// 파티가 아예 존재하지 않았음
		return 0;
	}

	int iPartyComplete = 0;
	int iUserIndex;
	for (int iPartyUserIndex = 0; iPartyUserIndex < MAX_PARTYUSER ; iPartyUserIndex++) {
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];
		if (gObjIsConnected(iUserIndex)) {
			if (CHECK_BLOODCASTLE(gObj[iUserIndex].MapNumber) && 
				CHECK_LIMIT(gObj[iUserIndex].m_cBloodCastleIndex, MAX_BLOODCASTLE_BRIDGE_COUNT) &&
				gObj[iUserIndex].Live == 1) 
			{
				if (gObj[iUserIndex].m_bBloodCastleComplete == true) {
					// 성공한 파티원
					iPartyComplete++;
				}
			}
		}
	}
	
	return iPartyComplete;
}


int CBloodCastle::GetWinnerPartyCompletePoint (					// 특정 다리의 우승자의 파티생존자에 따른 점수를 구한다.
	INT iBridgeIndex
	)
{
	int iPartyComplete = GetWinnerPartyCompleteCount(iBridgeIndex);
	iPartyComplete--;
	if (CHECK_LIMIT(iPartyComplete, MAX_PARTYUSER)) {
		return g_iBC_Party_EventPoint[iPartyComplete];
	}
	return 0;
}


void CBloodCastle::ChangeMonsterState (							// 블러드캐슬에 등장하는 몬스터는 리젠될 때 능력치 수정 여부를 결정하여 수정한다.
	INT iBridgeIndex,
	INT iIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		return;
	}
	
/*
	if (!gObjIsConnected(iIndex))
		return;

	if (gObj[iIndex].Type != OBJTYPE_MONSTER ||
		!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)
		)
		return;
*/

	INT iAddDamageMax	= 0;
	INT iAddDamageMin	= 0;
	INT iAddDefense		= 0;
	if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 5*60*1000) {
		// 남은 시간이 5분 이하이다 (10분경과)
		iAddDamageMin	= g_iBC_MONSTER_CHANGE_STATE[1][0];
		iAddDamageMax	= g_iBC_MONSTER_CHANGE_STATE[1][1];
		iAddDefense		= g_iBC_MONSTER_CHANGE_STATE[1][2];
	}
	else if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 10*60*1000) {
		// 남은 시간이 10분 이하이다 (5분경과)
		iAddDamageMin	= g_iBC_MONSTER_CHANGE_STATE[0][0];
		iAddDamageMax	= g_iBC_MONSTER_CHANGE_STATE[0][1];
		iAddDefense		= g_iBC_MONSTER_CHANGE_STATE[0][2];
	}

/*
	if (!iAddDamageMin &&
		!iAddDamageMax &&
		!iAddDefense
		)
		return;
*/

	LPMONSTER_ATTRIBUTE lpMA;
	lpMA = gMAttr.GetAttr(gObj[iIndex].Class);	
	if( lpMA == NULL ) {
		return;
	}

	gObj[iIndex].m_AttackDamageMin	= lpMA->m_DamageMin + iAddDamageMin;
	gObj[iIndex].m_AttackDamageMax	= lpMA->m_DamageMax + iAddDamageMax;
	gObj[iIndex].m_Defense			= lpMA->m_Defense + iAddDefense;
}


void CBloodCastle::FixUsersPlayStateWin (	// 블러드캐슬에 성공 시 점수적용 직전에 사용자들의 상태를 다시한번 체크 수정해 줌
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	if (m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1) {			// 미션이 성공했다면

		for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
			if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1) {
				// 존재하는 사용자 체크
				continue;
			}

			if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < 3) {
				// 접속되어 있는 사용자만 체크
				continue;
			}

			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1) || 
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1) ||
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex)
				) 
			{
				// 정보가 맞는 사용자만 체크
				continue;
			}
			
			LPOBJECTSTRUCT lpObj = &gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];
			
			switch (m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState) {
			case BC_USER_ALIVE :				// 생존자	
				{
					// 우승자의 생존자인가 사망자인가 확인 필요
					if ((CHECK_LIMIT(lpObj->PartyNumber, MAX_PARTY)) &&
						(lpObj->PartyNumber == gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber)
						) 
					{
						// 우승자의 파티이다.
						if (CHECK_BLOODCASTLE(lpObj->MapNumber) &&
							lpObj->Live == 1 &&
							lpObj->Life > 0.0
							)
						{
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_ALIVE;
						}
						else {
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
						}
					}
					else {
						// 혹시 죽거나 밖으로 이동했는지 체크한다.
						if (!CHECK_BLOODCASTLE(lpObj->MapNumber) ||
							lpObj->Live == 0 ||
							lpObj->Life <= 0.0
							)
						{
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
						}
					}
				}
				break;
			case BC_USER_DEAD :					// 사망자
				{
					// 우승자의 사망자인가 확인 필요
					if ((CHECK_LIMIT(lpObj->PartyNumber, MAX_PARTY)) &&
						(lpObj->PartyNumber == gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber)
						) 
					{
						// 우승자의 파티이다.
						m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
					}
				}
				break;
			case BC_USER_WINNER :				// 우승자
				{
					// 한번 우승자는 변함 없음 (이전에 CheckWinnerValid() 에서 무결성을 미리 체크한다는 전제)
				}
				break;
			case BC_USER_WINNER_PARTY_ALIVE :	// 생존자 (우승자의 파티원)
				{
					// 우승자가 정해질 때 같이 정해지만 파티가 중간에 끊겼는지 체크, 살아있는지 체크
					if ((CHECK_LIMIT(lpObj->PartyNumber, MAX_PARTY)) &&
						(lpObj->PartyNumber == gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber)
						) 
					{
						// 우승자의 파티이다.
						if (!CHECK_BLOODCASTLE(lpObj->MapNumber) ||
							lpObj->Live == 0 ||
							lpObj->Life <= 0.0
							)
						{
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
						}
					}
					else {
						// 더이상 우승자의 파티가 아니다
						if (CHECK_BLOODCASTLE(lpObj->MapNumber) &&
							lpObj->Live == 1 &&
							lpObj->Life > 0.0
							)
						{
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_ALIVE;
						}
						else {
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
						}
					}
				}
				break;
			case BC_USER_WINNER_PARTY_DEAD :	// 사망자 (우승자의 파티원)
				{
					// 우승자가 정해질 때 같이 정해지만 파티가 중간에 끊겼는지 체크
					if (!(CHECK_LIMIT(lpObj->PartyNumber, MAX_PARTY)) ||
						(lpObj->PartyNumber != gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber)
						) 
					{
						// 더이상 우승자의 파티가 아니다
						m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
					}
				}
				break;
			}

			LogAddTD ("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateWin() - State : %d", 
				iBridgeIndex+1,
				lpObj->AccountID,
				lpObj->Name,
				m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState
				);
		}
	}
}


void CBloodCastle::FixUsersPlayStateFail (	// 블러드캐슬에 실패 시 점수적용 직전에 사용자들의 상태를 다시한번 체크 수정해 줌
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1) {
			// 존재하는 사용자 체크
			continue;
		}

		if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < 3) {
			// 접속되어 있는 사용자만 체크
			continue;
		}

		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1) || 
			(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1) ||
			(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex)
			) 
		{
			// 정보가 맞는 사용자만 체크
			continue;
		}
		
		LPOBJECTSTRUCT lpObj = &gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];
		
		switch (m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState) {
		case BC_USER_ALIVE :				// 생존자	
			{
				// 해당사항 없음
			}
			break;
		case BC_USER_DEAD :					// 사망자
			{
				// 해당사항 없음
			}
			break;
		case BC_USER_WINNER :				// 우승자
			{
				// 해당사항 없음
			}
			break;
		case BC_USER_WINNER_PARTY_ALIVE :	// 생존자 (우승자의 파티원)
			{
				// 살았는지 죽었는지 확인하고 실패처리
				if (!CHECK_BLOODCASTLE(lpObj->MapNumber) ||
					lpObj->Live == 0 ||
					lpObj->Life <= 0.0
					)
				{
					m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
				}
				else {
					m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_ALIVE;
				}
			}
			break;
		case BC_USER_WINNER_PARTY_DEAD :	// 사망자 (우승자의 파티원)
			{
				// 실패처리
				m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
			}
			break;
		}

		LogAddTD ("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateFail() - State : %d", 
			iBridgeIndex+1,
			lpObj->AccountID,
			lpObj->Name,
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState
			);
	}
}


#endif

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리번호로 맵번호 얻기
INT CBloodCastle::GetMapNumByBCBridge( INT iBridgeIndex )
{
	INT iMapNum = 0;
	switch( iBridgeIndex )
	{
	case 0: 	iMapNum = MAP_INDEX_BLOODCASTLE1;	break;
	case 1:		iMapNum = MAP_INDEX_BLOODCASTLE2;	break;
	case 2:		iMapNum = MAP_INDEX_BLOODCASTLE3;	break;
	case 3:		iMapNum = MAP_INDEX_BLOODCASTLE4;	break;
	case 4:		iMapNum = MAP_INDEX_BLOODCASTLE5;	break;
	case 5:		iMapNum = MAP_INDEX_BLOODCASTLE6;	break;
	case 6:		iMapNum = MAP_INDEX_BLOODCASTLE7;	break;
	case 7:		iMapNum = MAP_INDEX_BLOODCASTLE8;	break;		
	}

	return iMapNum;
}

// 맵 번호로 블러드캐슬 인덱스 알아내기
INT	CBloodCastle::GetBridgeIndexByMapNum( INT iMapNum )
{
	int iBridgeIndex = -1;

	switch( iMapNum )
	{
	case 11:	iBridgeIndex = 0;	break;
	case 12:	iBridgeIndex = 1;	break;
	case 13:	iBridgeIndex = 2;	break;
	case 14:	iBridgeIndex = 3;	break;
	case 15:	iBridgeIndex = 4;	break;
	case 16:	iBridgeIndex = 5;	break;
	case 17:	iBridgeIndex = 6;	break;
	case 52:	iBridgeIndex = 7;	break;
	}

	return iBridgeIndex;
}

// DS와 통신(아이템 생성을 위해)을 위해 맵 번호를 구한다.
INT	CBloodCastle::GetMapNumByBC_CHAOSGEM( INT iChaosGem )
{
	// ** 블러드캐슬 내에서 혼석과 절대무기를 만들기 위해 DS와 통신에서 사용하는 번호로
	// ** 아이템을 드롭해야 할 맵 번호를 구한다.
	// 왜 이렇게 했는지는 몰라.

	int iMapNum		= iChaosGem;
	switch( iChaosGem )
	{
	case BC_MAPNUMBER_CHAOSGEM1:		iMapNum = MAP_INDEX_BLOODCASTLE1;	break;
	case BC_MAPNUMBER_CHAOSGEM2: 		iMapNum = MAP_INDEX_BLOODCASTLE2;	break;
	case BC_MAPNUMBER_CHAOSGEM3:		iMapNum = MAP_INDEX_BLOODCASTLE3;	break;
	case BC_MAPNUMBER_CHAOSGEM4:		iMapNum = MAP_INDEX_BLOODCASTLE4;	break;
	case BC_MAPNUMBER_CHAOSGEM5:		iMapNum = MAP_INDEX_BLOODCASTLE5;	break;
	case BC_MAPNUMBER_CHAOSGEM6:		iMapNum = MAP_INDEX_BLOODCASTLE6;	break;
	case BC_MAPNUMBER_CHAOSGEM7:		iMapNum = MAP_INDEX_BLOODCASTLE7;	break;
	case BC_MAPNUMBER_CHAOSGEM8:		iMapNum = MAP_INDEX_BLOODCASTLE8;	break;	
	}

	return iMapNum;
}

INT	CBloodCastle::GetMapNumByBC_ULTIMATEWEAPON( INT iUltimateWeapon )
{
	// ** 블러드캐슬 내에서 혼석과 절대무기를 만들기 위해 DS와 통신에서 사용하는 번호로
	// ** 아이템을 드롭해야 할 맵 번호를 구한다.
	// 왜 이렇게 했는지는 몰라.

	int iMapNum		= iUltimateWeapon;
	switch( iUltimateWeapon )
	{
	case BC_MAPNUMBER_ULTIMATEWEAPON1:		iMapNum = MAP_INDEX_BLOODCASTLE1;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON2: 		iMapNum = MAP_INDEX_BLOODCASTLE2;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON3:		iMapNum = MAP_INDEX_BLOODCASTLE3;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON4:		iMapNum = MAP_INDEX_BLOODCASTLE4;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON5:		iMapNum = MAP_INDEX_BLOODCASTLE5;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON6:		iMapNum = MAP_INDEX_BLOODCASTLE6;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON7:		iMapNum = MAP_INDEX_BLOODCASTLE7;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON8:		iMapNum = MAP_INDEX_BLOODCASTLE8;	break;	
	}

	return iMapNum;
}

#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010

#endif