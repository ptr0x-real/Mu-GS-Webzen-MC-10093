// ChaosCastle.cpp: implementation of the CChaosCastle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaosCastle.h"

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "Include\Prodef.h"
#include "Include\SProdef.h"
#include "Common\winutil.h"
#include "Include\GMProtocol.h"
#include "Include\Public.h"
#include "Include\cb_protocol.h"
#include "Include\public.h"
#include "Include\Readscript.h"

#include "User.h"
#include "NpcTalk.h"
#include "protocol.h"

#include "wsJoinServerCli.h"

#include "MonsterSetBase.h"
#include "gObjMonster.h"
#include "GameMain.h"
#include "PartyClass.h"
#include "wzUdp.h"
#include "DSProtocol.h"
#include "Event.h"

#include "MapClass.h"
#include "GameServer.h"
#include "DirPath.h"
#include "Common\Classdef.h"
#include "Common\zzzitem.h"

#include "LogToFile.H"

//CLogToFile	g_LogToFile ("GS_CC_LOG", ".\\GS_CC_LOG");		// 로그파일 객체


#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif

#ifdef MU_CRYWOLF_PENALTY_20051215
#include "CrywolfSync.h"
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
CChaosCastle			g_ChaosCastle;				// 카오스 캐슬 이벤트 객체

extern PartyClass		gParty;
extern CMonsterSetBase	gMSetBase;
extern MapClass			MapC[];
extern classdef			DCInfo;


extern wsJoinServerCli	wsRServerCli;				//  랭킹서버
extern wsJoinServerCli	wsEvenChipServerCli;		//  이벤트 칩 서버

extern CDirPath			gDirPath;

extern ITEM_ATTRIBUTE	ItemAttribute [MAX_ITEM];


INT	g_iChaosCastle_OffLineGiftRate	= CHAOSCASTLE_OFFLINE_GIFT_RATE;			// 카오스 캐슬 오프라인 경품에 당첨될 확률을 구한다.


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CChaosCastle::CChaosCastle()
{
	m_iMinUserCount = MIN_PLAYABLE_USERCOUNT;
	m_sMinPlayerMessage = "[Chaos Castle] Finalizo por falta de jugadores";

	m_bCC_EVENT_ENABLE = false;

	m_iCC_TIME_MIN_OPEN = 10;						// 나중에 파일에서 읽어야 함
	m_iCC_NORMAL_ITEM_DROP = 100;
	m_iCC_EXCEL_ITEM_DROP = 1200;
	m_iCC_MONSTER_REGEN = 0;

	for (int i = 0 ; i < MAX_CHAOSCASTLE_COUNT ; i++) {
		m_stChaosCastleData[i].m_iCC_STATE			= CHAOSCASTLE_STATE_NONE;
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 얻기
		m_stChaosCastleData[i].m_iMapNumber			= GetMapNumByChaosCastle( i );
#else
		m_stChaosCastleData[i].m_iMapNumber			= MAP_INDEX_CHAOSCASTLE1 + i;
#endif
		m_stChaosCastleData[i].m_iChaosCastleIndex	= i;
		m_stChaosCastleData[i].m_iCC_REMAIN_MSEC	= -1;
		m_stChaosCastleData[i].m_iCC_TICK_COUNT		= -1;
		
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
		m_stChaosCastleData[i].m_btCC_EVENT			= 0;
#endif
		
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
		m_stChaosCastleData[i].m_fRewardExpRate		= 1.0f;
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
		
		ClearChaosCastleData(i);
	}

	srand(time(0));
}


CChaosCastle::~CChaosCastle()
{
	for (int i = 0 ; i < MAX_CHAOSCASTLE_COUNT ; i++) {
	}
}


VOID CChaosCastle::Run()
{
	if (m_bCC_EVENT_ENABLE) {
	
		for (INT i = 0 ; i < MAX_CHAOSCASTLE_COUNT ; i++) {
			switch (m_stChaosCastleData[i].m_iCC_STATE) {
			case CHAOSCASTLE_STATE_NONE :
				ProcState_None(i);
				break;
			case CHAOSCASTLE_STATE_CLOSED :
				ProcState_Closed(i);
				break;
			case CHAOSCASTLE_STATE_PLAYING :
				ProcState_Playing(i);
				break;
			case CHAOSCASTLE_STATE_PLAYEND :
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


VOID CChaosCastle::Init (bool bEVENT_ENABLE)
{
	m_bCC_EVENT_ENABLE = bEVENT_ENABLE;


	for (INT i = 0 ; i < MAX_CHAOSCASTLE_COUNT ; i++)
		SetState(i, CHAOSCASTLE_STATE_CLOSED);


	for (INT iTRAP_STEP = 0 ; iTRAP_STEP < 4 ; iTRAP_STEP++) {
		//-----------------------------------------------------------------------------------
		// 외곽 범위
		// 1> . 1 사분면 
		g_rtPOINT_FRAME[0].left		= (g_iChaosCastle_GroundAxis[2] + g_iChaosCastle_GroundAxis[0]) / 2 + 1;
		g_rtPOINT_FRAME[0].top		= g_iChaosCastle_GroundAxis[1];
		g_rtPOINT_FRAME[0].right	= g_iChaosCastle_GroundAxis[2];
		g_rtPOINT_FRAME[0].bottom	= (g_iChaosCastle_GroundAxis[3] + g_iChaosCastle_GroundAxis[1]) / 2;
		// 2> . 2 사분면 
		g_rtPOINT_FRAME[1].left		= (g_iChaosCastle_GroundAxis[2] + g_iChaosCastle_GroundAxis[0]) / 2 + 1;
		g_rtPOINT_FRAME[1].top		= (g_iChaosCastle_GroundAxis[3] + g_iChaosCastle_GroundAxis[1]) / 2 + 1;
		g_rtPOINT_FRAME[1].right	= g_iChaosCastle_GroundAxis[2];
		g_rtPOINT_FRAME[1].bottom	= g_iChaosCastle_GroundAxis[3];
		// 3> . 3 사분면 
		g_rtPOINT_FRAME[2].left		= g_iChaosCastle_GroundAxis[0];
		g_rtPOINT_FRAME[2].top		= (g_iChaosCastle_GroundAxis[3] + g_iChaosCastle_GroundAxis[1]) / 2 + 1;
		g_rtPOINT_FRAME[2].right	= (g_iChaosCastle_GroundAxis[2] + g_iChaosCastle_GroundAxis[0]) / 2;
		g_rtPOINT_FRAME[2].bottom	= g_iChaosCastle_GroundAxis[3];
		// 4> . 4 사분면 
		g_rtPOINT_FRAME[3].left		= g_iChaosCastle_GroundAxis[0];
		g_rtPOINT_FRAME[3].top		= g_iChaosCastle_GroundAxis[1];
		g_rtPOINT_FRAME[3].right	= (g_iChaosCastle_GroundAxis[2] + g_iChaosCastle_GroundAxis[0]) / 2;
		g_rtPOINT_FRAME[3].bottom	= (g_iChaosCastle_GroundAxis[3] + g_iChaosCastle_GroundAxis[1]) / 2;

		//-----------------------------------------------------------------------------------
		// 트랩 범위
		// 1> . 1 사분면
		g_rtPOINT_TRAP[0].left	= (g_iChaosCastle_DamageAxis[iTRAP_STEP][2] + g_iChaosCastle_DamageAxis[iTRAP_STEP][0]) / 2 + 1;
		g_rtPOINT_TRAP[0].top		= g_iChaosCastle_DamageAxis[iTRAP_STEP][1];
		g_rtPOINT_TRAP[0].right	= g_iChaosCastle_DamageAxis[iTRAP_STEP][2];
		g_rtPOINT_TRAP[0].bottom	= (g_iChaosCastle_DamageAxis[iTRAP_STEP][3] + g_iChaosCastle_DamageAxis[iTRAP_STEP][1]) / 2;
		// 2> . 2 사분면 
		g_rtPOINT_TRAP[1].left	= (g_iChaosCastle_DamageAxis[iTRAP_STEP][2] + g_iChaosCastle_DamageAxis[iTRAP_STEP][0]) / 2 + 1;
		g_rtPOINT_TRAP[1].top		= (g_iChaosCastle_DamageAxis[iTRAP_STEP][3] + g_iChaosCastle_DamageAxis[iTRAP_STEP][1]) / 2 + 1;
		g_rtPOINT_TRAP[1].right	= g_iChaosCastle_DamageAxis[iTRAP_STEP][2];
		g_rtPOINT_TRAP[1].bottom	= g_iChaosCastle_DamageAxis[iTRAP_STEP][3];
		// 3> . 3 사분면 
		g_rtPOINT_TRAP[2].left	= g_iChaosCastle_DamageAxis[iTRAP_STEP][0];
		g_rtPOINT_TRAP[2].top		= (g_iChaosCastle_DamageAxis[iTRAP_STEP][3] + g_iChaosCastle_DamageAxis[iTRAP_STEP][1]) / 2 + 1;
		g_rtPOINT_TRAP[2].right	= (g_iChaosCastle_DamageAxis[iTRAP_STEP][2] + g_iChaosCastle_DamageAxis[iTRAP_STEP][0]) / 2;
		g_rtPOINT_TRAP[2].bottom	= g_iChaosCastle_DamageAxis[iTRAP_STEP][3];
		// 4> . 4 사분면 
		g_rtPOINT_TRAP[3].left	= g_iChaosCastle_DamageAxis[iTRAP_STEP][0];
		g_rtPOINT_TRAP[3].top		= g_iChaosCastle_DamageAxis[iTRAP_STEP][1];
		g_rtPOINT_TRAP[3].right	= (g_iChaosCastle_DamageAxis[iTRAP_STEP][2] + g_iChaosCastle_DamageAxis[iTRAP_STEP][0]) / 2;
		g_rtPOINT_TRAP[3].bottom	= (g_iChaosCastle_DamageAxis[iTRAP_STEP][3] + g_iChaosCastle_DamageAxis[iTRAP_STEP][1]) / 2;
	}
}


VOID CChaosCastle::Load (char* filename)
{
	// *> . 스크립트를 읽어서 필요한 데이터를 저장한다.
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("[Chaos Castle] Info file Load Fail [%s]", filename);
		return ;
	}
	SMDToken Token;
	
	int type = -1;
	int iChaosCastleCount = -1;

	for(int i = 0;i < MAX_CHAOSCASTLE_COUNT;++i)
		m_LST_CHAOSCASTLE_REWARD[i].clear();

	m_vtChaosCastleOpenTime.erase(m_vtChaosCastleOpenTime.begin(), m_vtChaosCastleOpenTime.end());

	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{				
			if( type == 0 )
			{	// 카오스 캐슬 게임시간 설정
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iCC_TIME_MIN_OPEN = (int)TokenNumber;
				Token = (*GetToken)(); m_iCC_TIME_MIN_PLAY		= (int)TokenNumber;
				Token = (*GetToken)(); m_iCC_TIME_MIN_REST		= (int)TokenNumber;
				Token = (*GetToken)(); m_iMinUserCount			= (int)TokenNumber;
				Token = (*GetToken)(); m_sMinPlayerMessage		= TokenString;
			}
			else if( type == 1 )
			{	// 카오스 캐슬 기본 및 엑셀 아이템 드롭율, 몬스터 리젠시간 설정
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iCC_NORMAL_ITEM_DROP = (int)TokenNumber;			
				Token = (*GetToken)(); m_iCC_EXCEL_ITEM_DROP	= (int)TokenNumber;
				Token = (*GetToken)(); m_iCC_MONSTER_REGEN		= (int)TokenNumber;
			}
			else if( type == 2 )
			{	// 카오스 캐슬이 시작할 시간정보를 얻어서 저장
				CHAOSCASTLE_START_TIME pRET;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				pRET.m_iHour									= (int)TokenNumber;		// 카오스 캐슬 시작 시간
				Token = (*GetToken)(); pRET.m_iMinute			= (int)TokenNumber;		// 카오스 캐슬 시작 분
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				Token = (*GetToken)(); pRET.m_btEvent			= (BYTE)TokenNumber;	// 카오스 캐슬 이벤트 종류
#endif				
				m_vtChaosCastleOpenTime.push_back(pRET);
			}
			else if( type == 3 )
			{	// 카오스 캐슬 오프라인 경품에 당첨될 확률
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				g_iChaosCastle_OffLineGiftRate = (int)TokenNumber;
				g_iChaosCastle_OffLineGiftRate %= 10000;
			}
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			else if( type == 4 )
			{	// 카오스 캐슬 보상 경험치 퍼센트
				Token = (*GetToken)();
				if( strcmp("end", TokenString) == NULL ) break;

				INT iCastelNum = (int)TokenNumber;		
				if( CHECK_LIMIT(iCastelNum, MAX_CHAOSCASTLE_COUNT) ) {
					Token = (*GetToken)();
					m_stChaosCastleData[iCastelNum].m_fRewardExpRate = (FLOAT)TokenNumber;
				}
				else Token = (*GetToken)();				
			}
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			else if( type == 5 )
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iChaosCastleCount = (int)TokenNumber;

				while(true)
				{
					CHAOSCASTLE_REWARD reward;

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

					if (CHECK_LIMIT(iChaosCastleCount, MAX_CHAOSCASTLE_COUNT))
						m_LST_CHAOSCASTLE_REWARD[iChaosCastleCount].push_back(reward);
				};
			}
			else if( type == 6 )
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iChaosCastleCount = (int)TokenNumber;

				Token = (*GetToken)();
				if(iChaosCastleCount >= 0 && iChaosCastleCount < MAX_CHAOSCASTLE_COUNT)
				m_iDropCount[iChaosCastleCount] = TokenNumber;
			}
		}
	}
	fclose(SMDFile);

	LogAdd("%s file load!", filename);
}


VOID CChaosCastle::CheckSync			(INT iChaosCastleIndex)			// 특정 소굴의 싱크를 맞춘다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

//---------------->
// 카오스 캐슬 해외 스케줄 버그 수정 20081124 (grooving)
#if TESTSERVER == 1
// 테스트용 루틴
	//	m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC	= (m_iCC_TIME_MIN_OPEN + 1) * 60 * 1000;
	m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC	= 35000;
	m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT		= GetTickCount();
#ifdef  PCBANG_EVENT_FREE_CHAOSCASTLE_20060307	// TEST
	m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT			= 0;
#endif // #ifdef  PCBANG_EVENT_FREE_CHAOSCASTLE_20060307 // TEST
	
	return;
#endif // TESTSERVER == 1
//---------------->

	// 임의의 시간을 지정하는 루틴

	if (m_vtChaosCastleOpenTime.empty()) {
		LogAddC(2, "[Chaos Castle] (%d) No Schedule Time Data - Chaos Castle Terminated (m_vtChaosCastleOpenTime.empty())", 
			iChaosCastleIndex
			);
		SetState(iChaosCastleIndex+1, CHAOSCASTLE_STATE_NONE);
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime (& sysTime);

	INT iMIN_HOUR = 24;
	INT iMIN_MINUTE = 60;
	BOOL bTIME_CHANGED = FALSE;

	vector <CHAOSCASTLE_START_TIME>::iterator it = m_vtChaosCastleOpenTime.begin();

	for (;it != m_vtChaosCastleOpenTime.end();it++) {
		CHAOSCASTLE_START_TIME & pRET = *it;
		if (sysTime.wHour * 60 + sysTime.wMinute < pRET.m_iHour * 60 +pRET.m_iMinute)
		{
			if (iMIN_HOUR * 60 + iMIN_MINUTE > pRET.m_iHour * 60 +pRET.m_iMinute) {
				bTIME_CHANGED = TRUE;
				
				iMIN_HOUR = pRET.m_iHour;
				iMIN_MINUTE = pRET.m_iMinute;

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT = pRET.m_btEvent;	
#endif
			}
		}
	}

	if (bTIME_CHANGED == FALSE) {
		it = m_vtChaosCastleOpenTime.begin();
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;
		
		for (;it != m_vtChaosCastleOpenTime.end();it++) {
			CHAOSCASTLE_START_TIME & pRET = *it;
			if (iMIN_HOUR * 60 + iMIN_MINUTE > pRET.m_iHour * 60 +pRET.m_iMinute)
			{
				bTIME_CHANGED = TRUE+1;
				
				iMIN_HOUR = pRET.m_iHour;
				iMIN_MINUTE = pRET.m_iMinute;

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT = pRET.m_btEvent;
#endif
			}
		}
	}

	switch(bTIME_CHANGED) {
	case TRUE :			// 현재 시간보다 앞으로 진행될 시간이 크다.
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = ( ((60*iMIN_HOUR) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) * 1000;
		break;
	case TRUE + 1 :		// 현재 시간보다 앞으로 진행될 시간이 작다 (내일 할 예정)
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = ( ((60*(iMIN_HOUR+24)) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) * 1000;
		break;
	default:			// 에러
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
		LogAddC( 2, "[Chaos Castle] (%d)(Event:%d) No Schedule Time Data - Chaos Castle Terminated (bTIME_CHANGED=%d)", 
			iChaosCastleIndex + 1,
			m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
			bTIME_CHANGED			
			);
#else
		LogAddC(2, "[Chaos Castle] (%d) No Schedule Time Data - Chaos Castle Terminated (bTIME_CHANGED=%d)", 
			iChaosCastleIndex+1,
			bTIME_CHANGED			
			);
#endif
		SetState(iChaosCastleIndex, CHAOSCASTLE_STATE_NONE);
		return;
	}
	
	m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT = GetTickCount();
	
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	LogAddTD("[Chaos Castle] (%d)(EVENT:%d) Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		iChaosCastleIndex+1,
		m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / (60 * 1000), 
		iMIN_HOUR, 
		iMIN_MINUTE		
		);
#else
	LogAddTD("[Chaos Castle] (%d) Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		iChaosCastleIndex+1,
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / (60 * 1000), 
		iMIN_HOUR, 
		iMIN_MINUTE
		);	
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307 
//#endif	//#if TESTSERVER == 1
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307	// TEST

}


VOID CChaosCastle::ProcState_None	(INT iChaosCastleIndex)
{

}


VOID CChaosCastle::ProcState_Closed	(INT iChaosCastleIndex)
{
	INT iTICK_MSEC = (GetTickCount() - m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC -= iTICK_MSEC;
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
//		LogAddTD("[Chaos Castle] (%d) - CLOSED Check Time. [%d] sec remain", iChaosCastleIndex+1, m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / 1000);
#endif
		if (g_bChaosCastle) {
			// 1> . 입장시간이 되었음
			if ( (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= m_iCC_TIME_MIN_OPEN * 60 * 1000) && (! m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER) ) {
				// 이제부터 카오스 캐슬에 입장과 파티가 가능하다.
				m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER = true;
				m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_PARTY = true;

#if TESTSERVER == 1		// 테섭모드에서만 임시로 각 캐슬 입장공지를 날려준다.
				PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 0, "카오스 %d 캐슬 지금부터 입장가능!", iChaosCastleIndex+1);
#else
				pNotice.type = 0;
				wsprintf(pNotice.Notice, "카오스 %d 캐슬 지금부터 입장가능!", iChaosCastleIndex+1);
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
//				SendAllUserAnyMsg ((LPBYTE) &pNotice, pNotice.h.size);
#endif
			}
			
			
			// 2> . 입장가능 N분전 공지 (1분마다 한번씩)
			if ((m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= m_iCC_TIME_MIN_OPEN * 60 * 1000) && (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC > 0) && (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / (60*1000) != m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT )) {	
				m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT = m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / (60*1000);

				// 입장 가능시간부터 1분마다 공지한다.
				if (!iChaosCastleIndex) {
#ifdef MODIFY_PCBANG_EVENT_FREE_CHAOSCASTLE_20060331		
					// 다하면 너무 많으니 1번 다리만 대표로
					PMSG_NOTICE	pNotice;
					
					// 이벤트의 종류에 따라 공지를 구분한다.	
					BYTE btCurrent_Event = GetCurrentEvent(iChaosCastleIndex);
					if( btCurrent_Event == CC_EVENT_GENERAL )
					{	
						// 일반 카오스 캐슬
#ifdef MODIFY_NOTICE_20040325						
						TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1232), m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT + 1);		// "카오스 캐슬 입장까지 %d 분 남았습니다."
#else
						pNotice.type = 0;
						wsprintf(pNotice.Notice, lMsg.Get(1232), m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT + 1);					// "카오스 캐슬 입장까지 %d 분 남았습니다."
						PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif	// #ifdef MODIFY_NOTICE_20040325			
						
					}
					else if( btCurrent_Event == CC_EVENT_PCBANG_FREE_ENTER )
					{
						// PC방 전용 카오스 캐슬											
#ifdef MODIFY_NOTICE_20040325
						TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1257), "PC방 전용", m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT + 1);		// "%s 카오스캐슬의 입장 시간이 %d 분 남았습니다."
#else	
						pNotice.type = 0;
						wsprintf(pNotice.Notice, lMsg.Get(1257), "PC방 전용", m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT + 1);					// "%s 카오스캐슬의 입장 시간이 %d 분 남았습니다."
						PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif	// #ifdef MODIFY_NOTICE_20040325						
					}			
					
					SendAllUserAnyMsg( ( LPBYTE )&pNotice, pNotice.h.size );
					
#else	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					// 다하면 너무 많으니 1번 다리만 대표로 => 접속한 모든 사용자들에게
					PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
					TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1232), m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT + 1);		// "카오스 캐슬 입장까지 %d 분 남았습니다."
#else
					pNotice.type = 0;
					wsprintf(pNotice.Notice, lMsg.Get(1232), m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT + 1);					// "카오스 캐슬 입장까지 %d 분 남았습니다."
					PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif	// #ifdef MODIFY_NOTICE_20040325
					SendAllUserAnyMsg ((LPBYTE) &pNotice, pNotice.h.size);
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				}
			}


			// 3> . 입장종료 30초전 공지 (한번만)
			if ((m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 30 * 1000) && (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC > 0) && (!m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_ENTER)) {
				m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_ENTER = true;

				if (!iChaosCastleIndex) {
					// 다하면 너무 많으니 1번 캐슬만 대표로 => 접속한 모든 사용자들에게
					PMSG_SET_DEVILSQUARE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
					pMsg.Type = CC_MSG_BEFORE_ENTER;

					for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
					{
						if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
						{
							if (!CHECK_CHAOSCASTLE(gObj[n].MapNumber)			// 카오스 캐슬에 접속하지 않은 사람들만 골라서 보낸다.
#ifdef FOR_BLOODCASTLE
								&& !CHECK_BLOODCASTLE(gObj[n].MapNumber)		// 블러드 캐슬에 접속하지 않은 사람들만 골라서 보낸다.
#endif
								)		
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
							{
								// 이벤트의 종류에 따라 공지 대상자를 구분한다.	
								BYTE btCurrent_Event = GetCurrentEvent( iChaosCastleIndex );
								if( btCurrent_Event == CC_EVENT_GENERAL )
								{
									DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
								}
								else if( btCurrent_Event == CC_EVENT_PCBANG_FREE_ENTER )
								{
									if( gObj[n].m_bPCBangFreeChaosCastleUser )	// PC방 이용자에게만 메세지를 보낸다.
									{
										DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
									}									
								}
							}
#else
								DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
						}
					}
				}
			}
		}
	}

	if (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 0) {
		if (g_bChaosCastle)
			SetState (iChaosCastleIndex, CHAOSCASTLE_STATE_PLAYING);
		else
			SetState (iChaosCastleIndex, CHAOSCASTLE_STATE_CLOSED);
	}
}


VOID CChaosCastle::ProcState_Playing	(INT iChaosCastleIndex)
{
//	INT iTICK_SEC = (GetTickCount() - m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT) / 1000;
	INT iTICK_MSEC = (GetTickCount() - m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC -= iTICK_MSEC;
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT = GetTickCount();

		//g_LogToFile.Output("[CC_LOG] PROC - PLAYING (%d) : ENTER", iChaosCastleIndex+1 );

#if TESTSERVER == 1
//		LogAddTD("[Chaos Castle] (%d) - PLAYING Check Time. [%d] sec remain", iChaosCastleIndex+1, m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / 1000);
#endif
		if ((m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= (m_iCC_TIME_MIN_PLAY * 60 - CC_MAX_START_WAIT_TERM + 30) * 1000) && (!m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_PLAY)) {
			// 경기시작 30초전 메시지를 보내준다.
			m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_PLAY = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = CC_MSG_BEFORE_PLAY;
			SendChaosCastleAnyMsg ((LPBYTE)&pMsg, sizeof( pMsg ), iChaosCastleIndex);

			//g_LogToFile.Output("[CC_LOG] PROC - BEFORE START 30'S (%d)", iChaosCastleIndex+1 );
		}

		if ((m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 30 * 1000) && (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC > 0) && (!m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_END)) {
			// 경기종료 30초전 메시지를 보내준다.
			m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_END = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = CC_MSG_BEFORE_END;
			SendChaosCastleAnyMsg ((LPBYTE)&pMsg, sizeof( pMsg ), iChaosCastleIndex);

			//g_LogToFile.Output("[CC_LOG] PROC - BEFORE END 30'S (%d)", iChaosCastleIndex+1 );
		}

		if ((m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= (m_iCC_TIME_MIN_PLAY * 60 - CC_MAX_START_WAIT_TERM) * 1000) && (!m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START)) {
			// 카오스 캐슬 경기 시작 !!
			
			// 경기시간 재설정
			m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = m_iCC_TIME_MIN_PLAY * 60 * 1000;		// CHAOSCASTLE_STATE_PLAYING -> CHAOSCASTLE_STATE_PLAYEND 까지의 남은 시간
			m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_PARTY = false;

			// 이벤트 시작 메시지 필요!!
			PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
			TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1233), iChaosCastleIndex + 1);		// "제 %d 카오스 캐슬 퀘스트가 시작되었습니다."
#else
			pNotice.type = 0;
			wsprintf(pNotice.Notice, lMsg.Get(1233), iChaosCastleIndex + 1);					// "제 %d 카오스 캐슬 퀘스트가 시작되었습니다."
			PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
			SendChaosCastleAnyMsg ((LPBYTE)&pNotice, pNotice.h.size, iChaosCastleIndex);

			//g_LogToFile.Output("[CC_LOG] PROC - START !! - 1 (%d)",	iChaosCastleIndex+1	);

			// 캐슬 영역을 안전지대 속성 해제 시키고 통보한다.
			UnSafetyCastleZone (iChaosCastleIndex);
			SendCastleZoneSafetyInfo (iChaosCastleIndex, false);

			//g_LogToFile.Output("[CC_LOG] PROC - START !! - 2 (%d)",	iChaosCastleIndex+1	);

			// 경기 시작 속성을 지정하고 몬스터를 배치한다.
			m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START = true;
			m_stChaosCastleData[iChaosCastleIndex].m_iCC_FIRST_USER_COUNT		= ReCalcUserCount(iChaosCastleIndex);
			m_stChaosCastleData[iChaosCastleIndex].m_iCC_FIRST_MONSTER_COUNT	
				= m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT
				= SetMonster(iChaosCastleIndex);

			//g_LogToFile.Output("[CC_LOG] PROC - START !! - 3 (%d)",	iChaosCastleIndex+1	);

			// 캐슬의 몬스터들에게 아이템을 세팅한다.
			SetItemsToMonster (iChaosCastleIndex);

			// 캐슬의 전체 인원에게 상태를 보내준다.
			SendNoticeState(iChaosCastleIndex, CC_STATE_START);

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
			LogAddTD ("[Chaos Castle] (%d)(Event:%d) Chaos Castle Quest Start (UserCount:%d)", 
				iChaosCastleIndex + 1,
				m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
				GetCurPlayUser(iChaosCastleIndex)				
				);
#else
			LogAddTD ("[Chaos Castle] (%d) Chaos Castle Quest Start (UserCount:%d)", 
				iChaosCastleIndex+1,
				GetCurPlayUser(iChaosCastleIndex)
				);
#endif

			//g_LogToFile.Output("[CC_LOG] PROC - START !! - FIN (%d)", iChaosCastleIndex+1 );
		}


		// *> . 경기 진행 전에 체크할 사항들
		if (m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START == false) {
			// 입장이 완료되고도 사람이 없다면 카오스 캐슬 종료
			if (m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER == false) {
				INT iCUR_USER = GetCurPlayUser(iChaosCastleIndex);
				if (iCUR_USER == 0) {
					SetState (iChaosCastleIndex, CHAOSCASTLE_STATE_CLOSED);
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					LogAddTD ("[Chaos Castle] (%d)(Event:%d) Chaos Castle Quest Closed - No User"
						, iChaosCastleIndex + 1
						, m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT
						);
#else
					LogAddTD ("[Chaos Castle] (%d) Chaos Castle Quest Closed - No User", iChaosCastleIndex+1);
#endif					
					
					//g_LogToFile.Output("[CC_LOG] PROC - NO PLAYER (%d)", iChaosCastleIndex+1 );
					return;				
				}
			}
		}


		// *> . 경기 진행 중에 체크할 사항들
		if (m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START == true) {

			//g_LogToFile.Output("[CC_LOG] PROC - CUR PLAYING - 1 (%d)", iChaosCastleIndex+1 );

			// 1 초에 한번씩 사용자 위치의 맵속성을 체크하여 땅뚫림 속성위에 있다면 죽도록 한다.
			ProcessFallUser (iChaosCastleIndex);
			// 여기서 허공에서 방황하는 케릭터들을 처리해 준다.
			CheckUserInDieTile (iChaosCastleIndex);
			// 여기서 허공에서 방황하는 몬스터들을 처리해 준다.
			CheckMonsterInDieTile (iChaosCastleIndex);
		
			// 1 초에 한번씩 해당 캐슬의 트랩 상태를 확인, 변경 해 준다.
			ProcessTrapStatus (iChaosCastleIndex);

			//g_LogToFile.Output("[CC_LOG] PROC - CUR PLAYING - 2 (%d)", iChaosCastleIndex+1 );

			// 여기서 트랩이 변경되는 그 한 순간에 사용자들을 안으로 밀어준다.
			if (m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS != m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_OLD_STATUS) {
				m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_OLD_STATUS = m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS;
				// 특정 맵 구역의 속성을 트랩 단계에 따라 땅뚫림으로 변경한다.
				SetMapAttrHollow (iChaosCastleIndex, m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS);
				// 트랩 밖의 사용자와 몬스터를 안으로 끌어들인다. 그리고 맵 외곾의 아이템들을 지워준다.
				PullObjInnerPlace (iChaosCastleIndex, m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS);
			}

			//g_LogToFile.Output("[CC_LOG] PROC - CUR PLAYING - 3 (%d)", iChaosCastleIndex+1 );

			// 1초에 한번씩 트랩 범위안에 있는 사용자들을 체크하여 데미지를 준다.
//			CalUsersInTrap (iChaosCastleIndex);


			// 1 초에 한번씩 경기가 성공 / 실패 상태로 되었는지 체크한다.
			INT iCUR_PLAY_STATE = CheckPlayEnded(iChaosCastleIndex);
			if (iCUR_PLAY_STATE == CHAOSCASTLE_PLAYCHECK_SUCCESS) {
				// 경기가 성공적으로 끝났다.

				//g_LogToFile.Output("[CC_LOG] PROC - CUR PLAYING - SUCCESS 1 (%d)", iChaosCastleIndex+1 );

				INT iCC_WINNER_INDEX = -1;
#ifdef MODIFY_CHAOSCASTLE_BUGFIX2_20061026
				INT iCC_LEFT_MONSTER_COUNT = GetMonsterListCount( iChaosCastleIndex );
				if( iCC_LEFT_MONSTER_COUNT <= 0 ) {
#else
				if (m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT <= 0) {
#endif
					// 우승자를 찾아서 성공 메시지를 보내준다.
					iCC_WINNER_INDEX = GetCurrentWinUser(iChaosCastleIndex);
					if (iCC_WINNER_INDEX != -1) {
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
							LogAddTD( "[Chaos Castle] (%d)(Event:%d) [%s][%s] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
								iChaosCastleIndex + 1,
								m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
								gObj[iCC_WINNER_INDEX].AccountID,
								gObj[iCC_WINNER_INDEX].Name,
#ifdef UPDATE_ADD_LOG_20080117
								gObj[iCC_WINNER_INDEX].DbClass,
#else	// UPDATE_ADD_LOG_20080117								
								gObj[iCC_WINNER_INDEX].Class,
#endif	// UPDATE_ADD_LOG_20080117
								gObj[iCC_WINNER_INDEX].Level,
								gObj[iCC_WINNER_INDEX].LevelUpPoint,
								gObj[iCC_WINNER_INDEX].Experience,
								gObj[iCC_WINNER_INDEX].Strength,
								gObj[iCC_WINNER_INDEX].Dexterity,
								gObj[iCC_WINNER_INDEX].Vitality,
								gObj[iCC_WINNER_INDEX].Energy,
								gObj[iCC_WINNER_INDEX].Leadership,
								gObj[iCC_WINNER_INDEX].MapNumber,
								gObj[iCC_WINNER_INDEX].m_PK_Level
								);
	#else
							LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
								iChaosCastleIndex+1,
								gObj[iCC_WINNER_INDEX].AccountID, 
								gObj[iCC_WINNER_INDEX].Name,
								gObj[iCC_WINNER_INDEX].Class,
								gObj[iCC_WINNER_INDEX].Level,
								gObj[iCC_WINNER_INDEX].LevelUpPoint,
								gObj[iCC_WINNER_INDEX].Experience,
								gObj[iCC_WINNER_INDEX].Strength,
								gObj[iCC_WINNER_INDEX].Dexterity,
								gObj[iCC_WINNER_INDEX].Vitality,
								gObj[iCC_WINNER_INDEX].Energy,
								gObj[iCC_WINNER_INDEX].Leadership,
								gObj[iCC_WINNER_INDEX].MapNumber,
								gObj[iCC_WINNER_INDEX].m_PK_Level
								);
	#endif

#else
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
							LogAddTD( "[Chaos Castle] (%d)(Event:%d) [%s][%s] is Winner.",
								iChaosCastleIndex + 1,
								m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
								gObj[iCC_WINNER_INDEX].AccountID,
								gObj[iCC_WINNER_INDEX].Name
								);
	#else
							LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner.",
								iChaosCastleIndex+1,
								gObj[iCC_WINNER_INDEX].AccountID, 
								gObj[iCC_WINNER_INDEX].Name
								);
	#endif
#endif // EXTEND_LOG_SYSTEM_03_20060816

						SendWinMessage (iChaosCastleIndex, iCC_WINNER_INDEX);
						GiveWinnerItem (iChaosCastleIndex, iCC_WINNER_INDEX);

					}
					else {
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
						LogAddTD( "[Chaos Castle] (%d)(Event:%d) Has No Winner.",							
							iChaosCastleIndex + 1,
							m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT
							);
#else
						LogAddTD("[Chaos Castle] (%d) Has No Winner.",
							iChaosCastleIndex+1
							);
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					}
				}
				else {
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					LogAddTD( "[Chaos Castle] (%d)(Event:%d) Has No Winner : Monster Left (%d)",
						iChaosCastleIndex + 1,
						m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
						m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT
						);
#else
					LogAddTD("[Chaos Castle] (%d) Has No Winner : Monster Left (%d)",
						iChaosCastleIndex+1,
						m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT
						);
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				}

				//g_LogToFile.Output("[CC_LOG] PROC - CUR PLAYING - SUCCESS 2 (%d)", iChaosCastleIndex+1 );

				// 실패자 들에게 실패 메시지를 보내준다.
				SendAllLoserFailMessage (iChaosCastleIndex, iCC_WINNER_INDEX);
					
				SetState (iChaosCastleIndex, CHAOSCASTLE_STATE_PLAYEND);
				return;
			}
			if (iCUR_PLAY_STATE == CHAOSCASTLE_PLAYCHECK_FAILED) {
				// 경기가 실패했다.

				//g_LogToFile.Output("[CC_LOG] PROC - CUR PLAYING - FAILED 1 (%d)", iChaosCastleIndex+1 );

				INT iCC_WINNER_INDEX = -1;
#ifdef MODIFY_CHAOSCASTLE_BUGFIX2_20061026
				INT iCC_LEFT_MONSTER_COUNT = GetMonsterListCount( iChaosCastleIndex );
				if( iCC_LEFT_MONSTER_COUNT <= 0 ) {
#else
				if (m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT <= 0) {
#endif
					// 우승자를 찾아서 성공 메시지를 보내준다.
					iCC_WINNER_INDEX = GetCurrentWinUser(iChaosCastleIndex);
					if (iCC_WINNER_INDEX != -1) {
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
							LogAddTD( "[Chaos Castle] (%d)(Event:%d) [%s][%s] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
								iChaosCastleIndex + 1,
								m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
								gObj[iCC_WINNER_INDEX].AccountID,
								gObj[iCC_WINNER_INDEX].Name,
#ifdef UPDATE_ADD_LOG_20080117
								gObj[iCC_WINNER_INDEX].DbClass,
#else	// UPDATE_ADD_LOG_20080117
								gObj[iCC_WINNER_INDEX].Class,
#endif	// UPDATE_ADD_LOG_20080117
								gObj[iCC_WINNER_INDEX].Level,
								gObj[iCC_WINNER_INDEX].LevelUpPoint,
								gObj[iCC_WINNER_INDEX].Experience,
								gObj[iCC_WINNER_INDEX].Strength,
								gObj[iCC_WINNER_INDEX].Dexterity,
								gObj[iCC_WINNER_INDEX].Vitality,
								gObj[iCC_WINNER_INDEX].Energy,
								gObj[iCC_WINNER_INDEX].Leadership,
								gObj[iCC_WINNER_INDEX].MapNumber,
								gObj[iCC_WINNER_INDEX].m_PK_Level
								);
	#else
							LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
								iChaosCastleIndex+1,
								gObj[iCC_WINNER_INDEX].AccountID, 
								gObj[iCC_WINNER_INDEX].Name,
								gObj[iCC_WINNER_INDEX].Class,
								gObj[iCC_WINNER_INDEX].Level,
								gObj[iCC_WINNER_INDEX].LevelUpPoint,
								gObj[iCC_WINNER_INDEX].Experience,
								gObj[iCC_WINNER_INDEX].Strength,
								gObj[iCC_WINNER_INDEX].Dexterity,
								gObj[iCC_WINNER_INDEX].Vitality,
								gObj[iCC_WINNER_INDEX].Energy,
								gObj[iCC_WINNER_INDEX].Leadership,
								gObj[iCC_WINNER_INDEX].MapNumber,
								gObj[iCC_WINNER_INDEX].m_PK_Level
								);
	#endif
#else
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
							LogAddTD( "[Chaos Castle] (%d)(Event:%d) [%s][%s] is Winner.",
								iChaosCastleIndex + 1,
								m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
								gObj[iCC_WINNER_INDEX].AccountID,
								gObj[iCC_WINNER_INDEX].Name
								);
	#else
							LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner.",
								iChaosCastleIndex+1,
								gObj[iCC_WINNER_INDEX].AccountID, 
								gObj[iCC_WINNER_INDEX].Name
								);
	#endif
#endif // EXTEND_LOG_SYSTEM_03_20060816

						SendWinMessage (iChaosCastleIndex, iCC_WINNER_INDEX);
						GiveWinnerItem (iChaosCastleIndex, iCC_WINNER_INDEX);

					}
					else {
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
						LogAddTD( "[Chaos Castle] (%d)(Event:%d) Has No Winner.",
							iChaosCastleIndex + 1,
							m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT
							);
#else
						LogAddTD("[Chaos Castle] (%d) Has No Winner.",
							iChaosCastleIndex+1
							);						
#endif
					}
				}
				else {
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					LogAddTD("[Chaos Castle] (%d)(Event:%d) Has No Winner : Monster Left (%d)",
						iChaosCastleIndex + 1,
						m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
						m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT
						);
#else
					LogAddTD("[Chaos Castle] (%d) Has No Winner : Monster Left (%d)",
						iChaosCastleIndex+1,
						m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT
						);
#endif
				}
				
				//g_LogToFile.Output("[CC_LOG] PROC - CUR PLAYING - FAILED 2 (%d)", iChaosCastleIndex+1 );

				// 실패자 들에게 실패 메시지를 보내준다.
				SendAllLoserFailMessage (iChaosCastleIndex, iCC_WINNER_INDEX);

				SetState (iChaosCastleIndex, CHAOSCASTLE_STATE_PLAYEND);
				return;
			}
			if (iCUR_PLAY_STATE == CHAOSCASTLE_PLAYCHECK_ING) {
				// 경기가 진행 중이다.

				//g_LogToFile.Output("[CC_LOG] PROC - CUR PLAYING - NOT END (%d)", iChaosCastleIndex+1 );

				SendNoticeState(iChaosCastleIndex, CC_STATE_PLAY);
			}
		}

		//g_LogToFile.Output("[CC_LOG] PROC - PLAYING (%d) : OUT", iChaosCastleIndex+1 );
	}

	// 카오스 캐슬 시간이 다 되었다.
	if (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 0) {

		//g_LogToFile.Output("[CC_LOG] PROC - TIME OUT 1 (%d)", iChaosCastleIndex+1 );

		// 경기가 실패했다.
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
		LogAddTD("[Chaos Castle] (%d)(Event:%d) is Over : TIME-OUT (Left User:%d, Monster:%d)",
			iChaosCastleIndex + 1,
			m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
			GetCurPlayUser( iChaosCastleIndex ),
			GetMonsterListCount( iChaosCastleIndex )
			);
#else
		LogAddTD("[Chaos Castle] (%d) is Over : TIME-OUT (Left User:%d, Monster:%d)",
			iChaosCastleIndex+1,
			GetCurPlayUser(iChaosCastleIndex),
			GetMonsterListCount(iChaosCastleIndex)
			);
#endif

		INT iCC_WINNER_INDEX = -1;
#ifdef MODIFY_CHAOSCASTLE_BUGFIX2_20061026
				INT iCC_LEFT_MONSTER_COUNT = GetMonsterListCount( iChaosCastleIndex );
				if( iCC_LEFT_MONSTER_COUNT <= 0 ) {
#else
		if (m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT <= 0) {
#endif
			// 우승자를 찾아서 성공 메시지를 보내준다.
			iCC_WINNER_INDEX = GetCurrentWinUser(iChaosCastleIndex);
			if (iCC_WINNER_INDEX != -1) {
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				LogAddTD( "[Chaos Castle] (%d)(Event:%d) [%s][%s] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
					iChaosCastleIndex + 1,
					m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
					gObj[iCC_WINNER_INDEX].AccountID,
					gObj[iCC_WINNER_INDEX].Name,
#ifdef UPDATE_ADD_LOG_20080117
					gObj[iCC_WINNER_INDEX].DbClass,
#else	// UPDATE_ADD_LOG_20080117
					gObj[iCC_WINNER_INDEX].Class,
#endif	// UPDATE_ADD_LOG_20080117
					gObj[iCC_WINNER_INDEX].Level,
					gObj[iCC_WINNER_INDEX].LevelUpPoint,
					gObj[iCC_WINNER_INDEX].Experience,
					gObj[iCC_WINNER_INDEX].Strength,
					gObj[iCC_WINNER_INDEX].Dexterity,
					gObj[iCC_WINNER_INDEX].Vitality,
					gObj[iCC_WINNER_INDEX].Energy,
					gObj[iCC_WINNER_INDEX].Leadership,
					gObj[iCC_WINNER_INDEX].MapNumber,
					gObj[iCC_WINNER_INDEX].m_PK_Level
					);
	#else
				LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
					iChaosCastleIndex+1,
					gObj[iCC_WINNER_INDEX].AccountID, 
					gObj[iCC_WINNER_INDEX].Name,
					gObj[iCC_WINNER_INDEX].Class,
					gObj[iCC_WINNER_INDEX].Level,
					gObj[iCC_WINNER_INDEX].LevelUpPoint,
					gObj[iCC_WINNER_INDEX].Experience,
					gObj[iCC_WINNER_INDEX].Strength,
					gObj[iCC_WINNER_INDEX].Dexterity,
					gObj[iCC_WINNER_INDEX].Vitality,
					gObj[iCC_WINNER_INDEX].Energy,
					gObj[iCC_WINNER_INDEX].Leadership,
					gObj[iCC_WINNER_INDEX].MapNumber,
					gObj[iCC_WINNER_INDEX].m_PK_Level
					);
	#endif	
#else
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				LogAddTD("[Chaos Castle] (%d)(Event:%d) [%s][%s] is Winner.",
					iChaosCastleIndex + 1,
					m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
					gObj[iCC_WINNER_INDEX].AccountID, 
					gObj[iCC_WINNER_INDEX].Name
					);
	#else
				LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner.",
					iChaosCastleIndex+1,
					gObj[iCC_WINNER_INDEX].AccountID, 
					gObj[iCC_WINNER_INDEX].Name
					);
	#endif
#endif // EXTEND_LOG_SYSTEM_03_20060816

				SendWinMessage (iChaosCastleIndex, iCC_WINNER_INDEX);
				GiveWinnerItem (iChaosCastleIndex, iCC_WINNER_INDEX);

			}
			else {
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				LogAddTD("[Chaos Castle] (%d)(Event:%d) Has No Winner.",
					iChaosCastleIndex + 1,
					m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT
					);
#else
				LogAddTD("[Chaos Castle] (%d) Has No Winner.",
					iChaosCastleIndex+1
					);
#endif
			}
		}
		else {
#ifdef MODIFY_CHAOSCASTLE_SYSTEM_CHANGE_SUCCESS_FLOW_20070129
			// 한명의 유저와 하나 이상의 몬스터가 남은 경우 성공으로 처리한다.
			if( GetCurEnteredUser( iChaosCastleIndex ) == 1 )
			{
				// 우승자를 찾아서 성공 메시지를 보내준다.
				INT iCC_WINNER_INDEX = GetCurrentWinUser( iChaosCastleIndex );
				if( iCC_WINNER_INDEX != -1 ) 
				{
		#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					LogAddTD( "[Chaos Castle] (%d)(Event:%d) [%s][%s] is Winner(Monster Left). [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
						iChaosCastleIndex + 1,
						m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
						gObj[iCC_WINNER_INDEX].AccountID,
						gObj[iCC_WINNER_INDEX].Name,
						gObj[iCC_WINNER_INDEX].Class,
						gObj[iCC_WINNER_INDEX].Level,
						gObj[iCC_WINNER_INDEX].LevelUpPoint,
						gObj[iCC_WINNER_INDEX].Experience,
						gObj[iCC_WINNER_INDEX].Strength,
						gObj[iCC_WINNER_INDEX].Dexterity,
						gObj[iCC_WINNER_INDEX].Vitality,
						gObj[iCC_WINNER_INDEX].Energy,
						gObj[iCC_WINNER_INDEX].Leadership,
						gObj[iCC_WINNER_INDEX].MapNumber,
						gObj[iCC_WINNER_INDEX].m_PK_Level
						);
		#else
					LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner(Monster Left). [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
						iChaosCastleIndex+1,
						gObj[iCC_WINNER_INDEX].AccountID, 
						gObj[iCC_WINNER_INDEX].Name,
						gObj[iCC_WINNER_INDEX].Class,
						gObj[iCC_WINNER_INDEX].Level,
						gObj[iCC_WINNER_INDEX].LevelUpPoint,
						gObj[iCC_WINNER_INDEX].Experience,
						gObj[iCC_WINNER_INDEX].Strength,
						gObj[iCC_WINNER_INDEX].Dexterity,
						gObj[iCC_WINNER_INDEX].Vitality,
						gObj[iCC_WINNER_INDEX].Energy,
						gObj[iCC_WINNER_INDEX].Leadership,
						gObj[iCC_WINNER_INDEX].MapNumber,
						gObj[iCC_WINNER_INDEX].m_PK_Level
						);
		#endif	// PCBANG_EVENT_FREE_CHAOSCASTLE_20060307

					SendWinMessage (iChaosCastleIndex, iCC_WINNER_INDEX);
					GiveWinnerItem (iChaosCastleIndex, iCC_WINNER_INDEX);
				}
			}

#else // MODIFY_CHAOSCASTLE_SYSTEM_CHANGE_SUCCESS_FLOW_20070129
					
#ifdef ADD_LOG_CHAOSCASTLE_LEFT_MONSTER_ATTR_20070118
			// 한명의 유저와 하나 이상의 몬스터가 남은경우 남은 몬스터의 좌표를 로그로 남긴다.					
			if( GetCurEnteredUser( iChaosCastleIndex ) == 1 )
			{
				for( INT iMON = 0 ; iMON < MAX_CHAOSCASTLE_MONSTER_COUNT; iMON++ ) 
				{
					if( m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == -1 )
						continue;

					INT iMonsterIndex = m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON];
					if( !CHECK_LIMIT( iMonsterIndex, MAX_MONSTER ) ) 
					{
						m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] = -1;
						continue;
					}

					if( ( gObj[iMonsterIndex].Life > 0 ) && CHECK_CHAOSCASTLE( gObj[iMonsterIndex].MapNumber ) && gObj[iMonsterIndex].Connected > 0 ) 
					{
						// 이 몬스터들은 아직 살아있다.
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
						LogAddTD( "[Chaos Castle][Bug Tracer] (%d)(Event:%d) Left Monster AttrInfo %d/%d",
							iChaosCastleIndex + 1,
							m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
							gObj[iMonsterIndex].X, gObj[iMonsterIndex].Y
							);
#else
						LogAddTD( "[Chaos Castle][Bug Tracer] (%d) Left Monster AttrInfo %d/%d",
							iChaosCastleIndex + 1,
							gObj[iMonsterIndex].X, gObj[iMonsterIndex].Y
							);
#endif // PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					}
				}
			}
			else
#endif	// ADD_LOG_CHAOSCASTLE_LEFT_MONSTER_ATTR_20070118
			{		
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
			LogAddTD("[Chaos Castle] (%d)(Event:%d) Has No Winner : Monster Left (%d)",
				iChaosCastleIndex + 1,
				m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
				m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT
				);
#else
			LogAddTD("[Chaos Castle] (%d) Has No Winner : Monster Left (%d)",
				iChaosCastleIndex+1,
				m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT
				);
#endif
			}
#endif	// MODIFY_CHAOSCASTLE_SYSTEM_CHANGE_SUCCESS_FLOW_20070129	
		}

		//g_LogToFile.Output("[CC_LOG] PROC - TIME OUT 2 (%d)", iChaosCastleIndex+1 );
		
		// 실패자 들에게 실패 메시지를 보내준다.
		SendAllLoserFailMessage (iChaosCastleIndex, iCC_WINNER_INDEX);

		SetState (iChaosCastleIndex, CHAOSCASTLE_STATE_PLAYEND);
	}

}


VOID CChaosCastle::PullObjInnerPlace	(											// 각 트랩 단계에 따라 트랩위의 사용자들을 트랩이 아닌 안으로 밀어낸다. (재수없으면 떨어짐)
			INT iChaosCastleIndex,
			INT iTRAP_STEP
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if (!CHECK_LIMIT(iTRAP_STEP, CC_MAX_DAMAGE_STEP))
		return;

	if (iTRAP_STEP == CC_TRAP_0)			// 트랩 0 단계에서는 패널티가 없다.
		return;

//	INT iTRAP_STEP = m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS;

	
	// 1 . 현재 시점에서 맵의 트랩 안쪽의 각 사분면의 빈자리를 찾아서 저장한다. (나중에 사람을 밀어넣을 때 속도를 빨리하기 위함)
	INT iUSER_AXIS = 0;						// 사용자가 좌표계의 몇 사분면에 있는지 저장
	vector <POINT> vtMAP_UNTRAP[4];			// 맵에 비있고 땅뚫림이 없는 지점을 저장

	// *> . 여기서 빈자리를 찾아서 저장한다.
	for (INT iAXIS = 0 ; iAXIS < 4 ; iAXIS++) {
		for (INT iMAPX = g_rtPOINT_TRAP[iAXIS].left ; iMAPX <= g_rtPOINT_TRAP[iAXIS].right ; iMAPX++) {
			for (INT iMAPY = g_rtPOINT_TRAP[iAXIS].top ; iMAPY <= g_rtPOINT_TRAP[iAXIS].bottom ; iMAPY++) {
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 얻기
				int iMapNum = GetMapNumByChaosCastle( iChaosCastleIndex );
				BYTE btMapAttr = MapC[iMapNum].GetAttr(iMAPX, iMAPY);
#else
				BYTE btMapAttr = MapC[MAP_INDEX_CHAOSCASTLE1+iChaosCastleIndex].GetAttr(iMAPX, iMAPY);
#endif
				if( ((btMapAttr&MAP_ATTR_STAND)!=MAP_ATTR_STAND)				// 다른 누군가 서있음
					&&((btMapAttr&MAP_ATTR_BLOCK)!=MAP_ATTR_BLOCK)				// 벽
					&&((btMapAttr&MAP_ATTR_HOLLOW)!=MAP_ATTR_HOLLOW)			// 땅뚫림
					)
				{
					POINT iPT;
					iPT.x = iMAPX, iPT.y = iMAPY;
					vtMAP_UNTRAP[iAXIS].push_back(iPT);
				}
			}
		}
	}


	// 2 . 현재 해당 캐슬의 사용자들을 검색한다.
	for (INT i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			if ((gObjIsConnected (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE) &&
//				(CHECK_CHAOSCASTLE(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber))
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) )
#else
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex)
#endif
				)
			{
				INT iSX = gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].X;
				INT iSY = gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Y;

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
				INT iMapNum = GetMapNumByChaosCastle( iChaosCastleIndex );
				BYTE btAttr = (MapC[iMapNum].m_attrbuf[(iSY*MAX_TERRAIN_SIZE)+iSX]) & MAP_ATTR_HOLLOW;
#else
				BYTE btAttr = (MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(iSY*MAX_TERRAIN_SIZE)+iSX]) & MAP_ATTR_HOLLOW;
#endif

				if ( btAttr == MAP_ATTR_HOLLOW) {
					// 땅뚫림 속성안에 있는 사용자는 안쪽으로 끌어들인다.

					// 현재 데미지 범위 안에 있다. 안으로 튕겨야 하므로 이 사람이 몇사분면 안에 있는지 확인한다.
					INT z;

					for (z = 0 ; z < 4 ; z++) {
						if ((iSX >= g_rtPOINT_FRAME[z].left) && (iSX <= g_rtPOINT_FRAME[z].right) && (iSY >= g_rtPOINT_FRAME[z].top) && (iSY <= g_rtPOINT_FRAME[z].bottom)) {
							iUSER_AXIS = z;
							break;
						}
					}

					if (z == 4)		// 사용자가 어디에도 속하지 않았을 때는 (버그) 그냥 넘어간다.
						continue;		

					// 여기서 사분면의 빈 곳을 찾아서 튕겨준다.
					if (!vtMAP_UNTRAP[iUSER_AXIS].empty()) {
						POINT ptEMPTY = vtMAP_UNTRAP[iUSER_AXIS].back();
						vtMAP_UNTRAP[iUSER_AXIS].pop_back();

						if (ObjSetPosition (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, ptEMPTY.x, ptEMPTY.y) == TRUE) {
							// 성공했다면 마지막 밀어내기 한 시간을 기록한다.
							gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_iChaosCastleBlowTime = GetTickCount();
						}
					}
				}
			}
		}	
	}


	// 3 . 현재 캐슬에 남아있는 몬스터들을 검색한다.
	for (INT iMON = 0 ; iMON < MAX_CHAOSCASTLE_MONSTER_COUNT ; iMON++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == -1)
			continue;
		INT iMonsterIndex = m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON];
		if (!CHECK_LIMIT(iMonsterIndex, MAX_MONSTER)) {
			m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] = -1;
			continue;
		}

		if ((gObj[iMonsterIndex].Life > 0) && CHECK_CHAOSCASTLE(gObj[iMonsterIndex].MapNumber) && gObj[iMonsterIndex].Connected > 0) {

			INT iSX = gObj[iMonsterIndex].X;
			INT iSY = gObj[iMonsterIndex].Y;

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
			INT iMapNum	= GetMapNumByChaosCastle( iChaosCastleIndex );
			BYTE btAttr = (MapC[iMapNum].m_attrbuf[(iSY*MAX_TERRAIN_SIZE)+iSX]) & MAP_ATTR_HOLLOW;
#else
			BYTE btAttr = (MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(iSY*MAX_TERRAIN_SIZE)+iSX]) & MAP_ATTR_HOLLOW;
#endif

			if ( btAttr == MAP_ATTR_HOLLOW) {
				// 땅뚫림 속성안에 있는 사용자는 안쪽으로 끌어들인다.

				// 현재 데미지 범위 안에 있다. 안으로 튕겨야 하므로 이 사람이 몇사분면 안에 있는지 확인한다.
				int z;
				for (z = 0 ; z < 4 ; z++) {
					if ((iSX >= g_rtPOINT_FRAME[z].left) && (iSX <= g_rtPOINT_FRAME[z].right) && (iSY >= g_rtPOINT_FRAME[z].top) && (iSY <= g_rtPOINT_FRAME[z].bottom)) {
						iUSER_AXIS = z;
						break;
					}
				}

				if (z == 4)			// 사용자가 어디에도 속하지 않았을 때는 (버그) 그냥 넘어간다.
					continue;		

				// 여기서 사분면의 빈 곳을 찾아서 튕겨준다.
				if (!vtMAP_UNTRAP[iUSER_AXIS].empty()) {
					POINT ptEMPTY = vtMAP_UNTRAP[iUSER_AXIS].back();
					vtMAP_UNTRAP[iUSER_AXIS].pop_back();

					if (ObjSetPosition (iMonsterIndex, ptEMPTY.x, ptEMPTY.y) == TRUE) {
						// 성공했다면 마지막 밀어내기 한 시간을 기록한다.
						gObj[iMonsterIndex].m_iChaosCastleBlowTime = GetTickCount();
					}
				}
			}
		}
	}
	
	// 맵 외곾의 아이템들을 찾아서 지워준다.
	for (INT iMAP_ITEM = 0 ; iMAP_ITEM < MAX_MAPITEM ; iMAP_ITEM++) {
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기		
		INT iMapNum = GetMapNumByChaosCastle( iChaosCastleIndex );
		if (MapC[iMapNum].m_cItem[iMAP_ITEM].IsItem()) 
		{
			// 아이템이 존재한다면 이것이 좌표안에 있는 것인지를 확인한다.
			INT iUX = MapC[iMapNum].m_cItem[iMAP_ITEM].px;
			INT iUY = MapC[iMapNum].m_cItem[iMAP_ITEM].py;
			if(!((iUX >= g_iChaosCastle_DamageAxis[iTRAP_STEP][0]) && (iUX <= g_iChaosCastle_DamageAxis[iTRAP_STEP][2]) && (iUY >= g_iChaosCastle_DamageAxis[iTRAP_STEP][1]) && (iUY <= g_iChaosCastle_DamageAxis[iTRAP_STEP][3])))  
			{
				MapC[iMapNum].m_cItem[iMAP_ITEM].m_State		= OBJST_DIECMD;
				MapC[iMapNum].m_cItem[iMAP_ITEM].Give			= 1;
				MapC[iMapNum].m_cItem[iMAP_ITEM].live			= 0;
			}
		}
#else
		if (MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_cItem[iMAP_ITEM].IsItem()) {
			// 아이템이 존재한다면 이것이 좌표안에 있는 것인지를 확인한다.
			INT iUX = MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_cItem[iMAP_ITEM].px;
			INT iUY = MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_cItem[iMAP_ITEM].py;
			if(!((iUX >= g_iChaosCastle_DamageAxis[iTRAP_STEP][0]) && (iUX <= g_iChaosCastle_DamageAxis[iTRAP_STEP][2]) && (iUY >= g_iChaosCastle_DamageAxis[iTRAP_STEP][1]) && (iUY <= g_iChaosCastle_DamageAxis[iTRAP_STEP][3])))  {
				MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_cItem[iMAP_ITEM].m_State		= OBJST_DIECMD;
				MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_cItem[iMAP_ITEM].Give		= 1;
				MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_cItem[iMAP_ITEM].live		= 0;
			}
		}
#endif
	}
}


VOID CChaosCastle::SetMapAttrHollow	(										// 각 트랩 단계에 따라 맵의 속성을 땅 뚫림으로 변경해 준다.
			INT iChaosCastleIndex,									
			INT iTRAP_STEP
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if (!CHECK_LIMIT(iTRAP_STEP, CC_MAX_DAMAGE_STEP))
		return;

	if (iTRAP_STEP == CC_TRAP_0)			// 트랩 0 단계에서는 패널티가 없다.
		return;

	// 특정 맵 속성을 땅뚫림으로 변경해준다.
	for (INT a = 0 ; a < 4 ; a++) {
		for (INT i = g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][0] ; i <= g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][2] ; i++) {
			for (INT j = g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][1] ; j <= g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][3] ; j++) {
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기		
				INT iMapNum = GetMapNumByChaosCastle( iChaosCastleIndex );
				MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
#else
				MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
#endif
			}
		}
	}
}


VOID CChaosCastle::SetMapAttrFill	(											// 각 트랩 단계에 따라 맵의 속성을 땅 뚫림 아님으로 변경해 준다.
			INT iChaosCastleIndex									
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	// 특정 맵 속성을 땅뚫림을 제거해 준다.
	for (INT iTRAP_STEP = 1 ; iTRAP_STEP < CC_MAX_DAMAGE_STEP ; iTRAP_STEP++) {
		for (INT a = 0 ; a < 4 ; a++) {
			for (INT i = g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][0] ; i <= g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][2] ; i++) {
				for (INT j = g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][1] ; j <= g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][3] ; j++) {
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기		
					INT iMapNum = GetMapNumByChaosCastle( iChaosCastleIndex );
					MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_HOLLOW);
#else
					MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_HOLLOW);
#endif
				}
			}
		}
	}
}


VOID CChaosCastle::ProcState_PlayEnd	(INT iChaosCastleIndex)
{
	INT iTICK_MSEC = (GetTickCount() - m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC -= iTICK_MSEC;
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT = GetTickCount();

		// 여기서 허공에서 방황하는 케릭터들을 처리해 준다 - > 종료 대기 중일 때도 사용자가 나갈 수 있다.
		CheckUserInDieTile (iChaosCastleIndex);			

#if TESTSERVER == 1
//		LogAddTD("[Chaos Castle] (%d) - PLAY END Check Time. [%d] sec remain", iChaosCastleIndex+1, m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC/1000);
#endif
		if ((m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 30*1000) && (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC > 0) && (!m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_QUIT)) {
			m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_QUIT = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = CC_MSG_BEFORE_QUIT;
			SendChaosCastleAnyMsg ((LPBYTE)&pMsg, sizeof( pMsg ), iChaosCastleIndex);
		}
	}

	if (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 0) {
		SetState (iChaosCastleIndex, CHAOSCASTLE_STATE_CLOSED);
	}
}


VOID CChaosCastle::SetState (INT iChaosCastleIndex, INT iCC_STATE)		// 특정 소굴의 스케쥴러를 특정 상태로 변경한다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if ((iCC_STATE < CHAOSCASTLE_STATE_NONE) || (iCC_STATE > CHAOSCASTLE_STATE_PLAYEND))
		return;

	m_stChaosCastleData[iChaosCastleIndex].m_iCC_STATE = iCC_STATE;

	switch(m_stChaosCastleData[iChaosCastleIndex].m_iCC_STATE) {
	case CHAOSCASTLE_STATE_NONE :
		SetState_None(iChaosCastleIndex);
		break;
	case CHAOSCASTLE_STATE_CLOSED :
		SetState_Closed(iChaosCastleIndex);
		break;
	case CHAOSCASTLE_STATE_PLAYING :
		SetState_Playing(iChaosCastleIndex);
		break;
	case CHAOSCASTLE_STATE_PLAYEND :
		SetState_PlayEnd(iChaosCastleIndex);
		break;
	default:
		break;
	}
}


VOID CChaosCastle::SetState_None		(INT iChaosCastleIndex)			// 카오스 캐슬 이벤트 정지 시 사용
{
	
}


VOID CChaosCastle::SetState_Closed	(INT iChaosCastleIndex)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	SendNoticeState(iChaosCastleIndex, CC_STATE_END);

	ClearChaosCastleData(iChaosCastleIndex);

	ClearMonster(iChaosCastleIndex);

	// 혹시 카오스 캐슬에 남아있는 사람이 있으면 모두 데비아스로 이동시킨다.
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기		
		if( (gObj[n].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) ) && (gObj[n].Connected > 2) )
#else
		if( (gObj[n].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex) && (gObj[n].Connected > 2) )
#endif
		{
//*			SearchUserDeleteQuestItem(n);
			gObjMoveGate(n, 22);
		}
	}

	// 캐슬 영역을 안전지대 속성 세팅 시키고 통보한다.
	SafetyCastleZone (iChaosCastleIndex);

	// 캐슬 영역의 땅뚫림 속성을 제거한다.
	SetMapAttrFill (iChaosCastleIndex);

	CheckSync(iChaosCastleIndex);							// CHAOSCASTLE_STATE_CLOSED -> CHAOSCASTLE_STATE_PLAYING 까지의 남은 시간을 계산해줌

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	LogAddTD( "[Chaos Castle] (%d)(Event:%d) SetState CLOSED"
		, iChaosCastleIndex + 1
		, m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT 
		);
#else
	LogAddTD("[Chaos Castle] (%d) SetState CLOSED", iChaosCastleIndex+1);
#endif
}


VOID CChaosCastle::SetState_Playing	(INT iChaosCastleIndex)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER = false;
	m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = m_iCC_TIME_MIN_PLAY * 60 * 1000;		// CHAOSCASTLE_STATE_STATE_PLAYING -> CHAOSCASTLE_STATE_STATE_PLAYEND 까지의 남은 시간

/*	
	PMSG_NOTICE	pNotice;
	pNotice.type = 1;
	wsprintf(pNotice.Notice, lMsg.Get(1163), iChaosCastleIndex + 1, CC_MAX_START_WAIT_TERM);		// "제 %d 카오스 캐슬의 퀘스트 시작은 %d 초 후 입니다."
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
	SendChaosCastleAnyMsg ((LPBYTE) &pNotice, pNotice.h.size, iChaosCastleIndex);

	PMSG_SERVERCMD	ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
	ServerCmd.CmdType = 1;
	ServerCmd.X = 45;
	ServerCmd.Y = 0;
	SendChaosCastleAnyMsg ((LPBYTE) &ServerCmd, ServerCmd.h.size, iChaosCastleIndex);
*/

	// 경기 대기 중이므로 안전지대 속성을 통보한다.
	SendCastleZoneSafetyInfo (iChaosCastleIndex, true);

	// 경기를 시작할 수 있는지 체크한다.
#if TESTSERVER != 1		// 본섭버전 일 때만 체크한다.
	if (CheckCanStartPlay (iChaosCastleIndex) == false) {
		// 경기를 시작할수 없다면 여기서 입장료 등을 돌려주고 돌려보낸다.
		PlayFailedRollBack (iChaosCastleIndex);
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
		LogAddTD("[Chaos Castle] (%d)(Event:%d) Failed to Start Chaos Castle : Lack of User",
			iChaosCastleIndex + 1,
			m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT 
			);
#else
		LogAddTD("[Chaos Castle] (%d) Failed to Start Chaos Castle : Lack of User", iChaosCastleIndex+1);
		
#endif		
		SetState_Closed (iChaosCastleIndex);
		return;
	}
#endif
	
#ifdef CHAOSCASTLE_GUILD_DUEL_BUGFIX_20040611		// 사용자들을 체크하여 만약 길드전이나 결투 중인 사람이 있다면 강제 종료 시킨다.
	TerminateGuildWarDuel(iChaosCastleIndex);
#endif

#ifndef MODIFY_DELETE_ZETTO_EVENT_20061121					// !! NOT
	#ifndef FOR_CHINA		// 중국은 젠 로또경기를 안한다
	#ifndef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307	// 한국도 안한다.
		// 젠 로또 경기를 한다.
		RewardZettoMoney (iChaosCastleIndex);
	#endif	// #ifndef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	#endif
#endif // MODIFY_DELETE_ZETTO_EVENT_20061121

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	LogAddTD( "[Chaos Castle] (%d)(Event:%d) SetState PLAYING"
		, iChaosCastleIndex + 1
		, m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT 
		);
#else
	LogAddTD("[Chaos Castle] (%d) SetState PLAYING", iChaosCastleIndex+1);
#endif
}


VOID CChaosCastle::SetState_PlayEnd	(INT iChaosCastleIndex)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	SendNoticeState(iChaosCastleIndex, CC_STATE_END);

	ClearMonster(iChaosCastleIndex);

	m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER = false;
	m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = m_iCC_TIME_MIN_REST * 60 * 1000;		// CHAOSCASTLE_STATE_PLAYEND -> CHAOSCASTLE_STATE_CLOSED 까지의 남은 시간
	
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	LogAddTD("[Chaos Castle] (%d)(Event:%d) SetState PLAYEND"
		, iChaosCastleIndex + 1
		, m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT 
		);
#else
	LogAddTD("[Chaos Castle] (%d) SetState PLAYEND", iChaosCastleIndex+1);
#endif
}


VOID CChaosCastle::ClearChaosCastleData		(INT iChaosCastleIndex)			// 특정 소굴 데이터를 초기화 한다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	m_stChaosCastleData[iChaosCastleIndex].Clear();

	for (INT i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex		= -1;
			gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex	= -1;
			gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_iChaosCastleBlowTime	= 0;
			gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cKillUserCount		= 0;
			gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cKillMonsterCount		= 0;
		}

		m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].Clear();
	}
}


/* 카오스 캐슬 몬스터 관리 함수들 */
VOID CChaosCastle::ClearMonster	(									// 해당 캐슬의 모든 몬스터 제거
		INT iChaosCastleIndex
		)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;
	
	for( int n=0; n<MAX_MONSTER; n++)
	{
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기		
		if (gObj[n].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) ) {	// 만약 특정 카오스 캐슬에 있다면 상태가 어떠하든지 상관없이 다 제거
#else
		if (gObj[n].MapNumber == (MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex)) {	// 만약 특정 카오스 캐슬에 있다면 상태가 어떠하든지 상관없이 다 제거
#endif
			gObjDel(n);
		}
	}
}


INT CChaosCastle::SetMonster (INT iChaosCastleIndex)				// 카오스 캐슬에 나오는 몬스터의 특정 속성을 SET하고 배치한다. (맵번호가 18~24인 몬스터들 일괄배치)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT)) {	// 인덱스를 벗어나면 무시
		return -1;
	}

	// *> . 최대 몬스터 추가가능 수 = MAX_CHAOSCASTLE_MONSTER_COUNT - 사용자 수
	INT iNOW_ADDED_MONSTER_COUNT = 0;
	INT iMAX_ADDABLE_MONSTER_COUNT = MAX_CHAOSCASTLE_MONSTER_COUNT - GetCurPlayUser(iChaosCastleIndex);
	if (iMAX_ADDABLE_MONSTER_COUNT < 0)
		iMAX_ADDABLE_MONSTER_COUNT = 0;

	// *> . 일반 몬스터들을 추가한다.
	int result;	
	for( int n=0; n<gMSetBase.m_Count; n++)
	{
		if (iNOW_ADDED_MONSTER_COUNT >= iMAX_ADDABLE_MONSTER_COUNT)	// 해당 카오스 캐슬에 추가 가능한 최대 몬스터 수를 채웠다면 여기서 나간다.
			break;

		if( CHECK_CHAOSCASTLE(gMSetBase.m_Mp[n].m_MapNumber) )		// 블러드 캐슬에 들어가는 몬스터만
		{
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 몬스터 타입 형변경			
			WORD btMonsterType			= gMSetBase.m_Mp[n].m_Type;
#else
			BYTE btMonsterType			= gMSetBase.m_Mp[n].m_Type;
#endif
			BYTE btMapNumber			= gMSetBase.m_Mp[n].m_MapNumber;
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 번호 구하기
			BYTE btChaosCastleIndex		= GetChaosCastleByMapNum( btMapNumber );
#else
			BYTE btChaosCastleIndex		= btMapNumber - MAP_INDEX_CHAOSCASTLE1;
#endif

			if (btChaosCastleIndex != iChaosCastleIndex)			// 생성되어야 할 맵이 아니면 무시					
				continue;

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			result = gObjAddMonster(btMapNumber);
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

				gObj[result].MaxRegenTime			= m_iCC_MONSTER_REGEN;
				gObj[result].m_cChaosCastleIndex	= btChaosCastleIndex;
//				gObj[result].m_ItemRate				= m_iCC_NORMAL_ITEM_DROP;
				gObj[result].Dir					= rand()%8;		// 원래 Dir에는 다리번호만 있었으므로 여기에서 실제 방향을 준다.

				iNOW_ADDED_MONSTER_COUNT++;							// 실제 몬스터를 추가한 카운트를 올려준다.
				AddMonsterList (iChaosCastleIndex, result);			// 몬스터를 해당 캐슬의 몬스터 리스트에 등록한다.
			}
		}
	}

	return iNOW_ADDED_MONSTER_COUNT;
}


VOID CChaosCastle::AddMonsterList	(								// 특정 카오스 캐슬의 몬스터 개수를 추가한다.
			INT iChaosCastleIndex,
			INT iMonsterIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	for (INT iMON = 0 ; iMON < MAX_CHAOSCASTLE_MONSTER_COUNT ; iMON++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == -1) {
			InterlockedExchange(&m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON], iMonsterIndex);
			break;
		}	
	}
}


VOID CChaosCastle::DelMonsterList	(								// 특정 카오스 캐슬의 몬스터 개수를 제거한다.
			INT iChaosCastleIndex,
			INT iMonsterIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;
	
#ifdef ADD_LOG_FOR_CASTLE_BUG_20060911
	LogAddTD( "[Chaos Castle][Bug Tracer] MONSTER DEL (%d) Count(%d) : ENTER - Index(%d)", iChaosCastleIndex + 1, m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT, iMonsterIndex );
#endif

	//g_LogToFile.Output("[CC_LOG] MONSTER DEL (%d) : ENTER", iChaosCastleIndex+1 );

	for (INT iMON = 0 ; iMON < MAX_CHAOSCASTLE_MONSTER_COUNT ; iMON++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == iMonsterIndex) {
			InterlockedExchange(&m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON], -1);
			break;
		}	
	}

	InterlockedDecrement(&m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT);
	if (m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT < 0)
		m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT = 0;

#ifdef ADD_LOG_FOR_CASTLE_BUG_20060911
	LogAddTD( "[Chaos Castle][Bug Tracer] MONSTER DEL (%d) Count(%d) : OUT - Index(%d)", iChaosCastleIndex + 1, m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT, iMonsterIndex );
#endif
	
	//g_LogToFile.Output("[CC_LOG] MONSTER DEL (%d) : OUT", iChaosCastleIndex+1 );
}


INT CChaosCastle::GetMonsterListCount (								// 몬스터 리스트의 현재 개수를 얻는다.
			INT iChaosCastleIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;
	
#ifdef MODIFY_CHAOSCASTLE_BUGFIX_20060919		// 카오스캐슬 남은 몬스터 수 카운터 재확인
	int iAliveMonsterCount = 0;

#ifdef MODIFY_CHOSCASTLE_MONSTER_COUNT_RECAL_20070119
	for( INT iMON = 0 ; iMON < MAX_CHAOSCASTLE_MONSTER_COUNT; iMON++ ) 
	{
		if( m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == -1 )
			continue;

		INT iMonsterIndex = m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON];
		if( !CHECK_LIMIT( iMonsterIndex, MAX_MONSTER ) ) 
		{
			m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] = -1;
			continue;
		}

		if( ( gObj[iMonsterIndex].Life > 0 ) && CHECK_CHAOSCASTLE( gObj[iMonsterIndex].MapNumber ) && gObj[iMonsterIndex].Connected > 0 ) 
		{
			++iAliveMonsterCount;
		}
	}	

#else
	int iMapNumber = iChaosCastleIndex + MAP_INDEX_CHAOSCASTLE1;
	for( int iCount = 0; iCount < ALLOC_USEROBJECTSTART; iCount++ )
	{
		if( gObj[iCount].Connected == CSS_GAMEPLAYING && gObj[iCount].Type == OBJTYPE_MONSTER && gObj[iCount].Live == 1 ) 
		{
			if( gObj[iCount].MapNumber == iMapNumber )
			{
				++iAliveMonsterCount;
			}
		}
	}
#endif	// MODIFY_CHOSCASTLE_MONSTER_COUNT_RECAL_20070119
	
	return iAliveMonsterCount;

#else
	return m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT;
	
#endif
}


VOID CChaosCastle::SetItemsToMonster (								// 몬스터에게 지정된 아이템을 세팅한다. (m_vtMonsterCount 가 미리 세팅되어 있어야 함)
			INT iChaosCastleIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	INT iMAX_MONSTER = GetMonsterListCount(iChaosCastleIndex);

	if (iMAX_MONSTER <= 0)
		return;
	
	map <INT, MONSTER_ITEM_DROP>::iterator it;
	MONSTER_ITEM_DROP MID;

#ifdef ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113
	if (g_bNewServer == TRUE)
	{
		// 각 캐슬의 몬스터에게 할당된 아이템을 세팅한다.
		for (INT iA = 0 ; iA < MAX_CHAOSCASTLE_ITEM_DROP_KIND ; iA++) {
			for (INT iB = 0 ; iB < g_iChaosCastle_MonsterItemsNewServer[iChaosCastleIndex][iA][1] ; iB++) {
				for (INT iC = 0 ; iC < MAX_CHAOSCASTLE_ITEM_DROP_LOOP ; iC++) {
					INT iMONSTER_INDEX = m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[rand()%iMAX_MONSTER];
					
					it = m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.find(iMONSTER_INDEX);
					
					if (it == m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.end()) {
						MID.m_iIndex = iMONSTER_INDEX;
						MID.m_iItemKind = g_iChaosCastle_MonsterItemsNewServer[iChaosCastleIndex][iA][0];
						m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.insert(pair<INT, MONSTER_ITEM_DROP> (iMONSTER_INDEX, MID));
						break;
					}
				}
			}
		}
	}
	else
	{
		// 각 캐슬의 몬스터에게 할당된 아이템을 세팅한다.
		for (INT iA = 0 ; iA < MAX_CHAOSCASTLE_ITEM_DROP_KIND ; iA++) {
			for (INT iB = 0 ; iB < g_iChaosCastle_MonsterItems[iChaosCastleIndex][iA][1] ; iB++) {
				for (INT iC = 0 ; iC < MAX_CHAOSCASTLE_ITEM_DROP_LOOP ; iC++) {
					INT iMONSTER_INDEX = m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[rand()%iMAX_MONSTER];
					
					it = m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.find(iMONSTER_INDEX);
					
					if (it == m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.end()) {
						MID.m_iIndex = iMONSTER_INDEX;
						MID.m_iItemKind = g_iChaosCastle_MonsterItems[iChaosCastleIndex][iA][0];
						m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.insert(pair<INT, MONSTER_ITEM_DROP> (iMONSTER_INDEX, MID));
						break;
					}
				}
			}
		}
	}
#else // ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113
	// 각 캐슬의 몬스터에게 할당된 아이템을 세팅한다.
	for (INT iA = 0 ; iA < MAX_CHAOSCASTLE_ITEM_DROP_KIND ; iA++) {
		for (INT iB = 0 ; iB < g_iChaosCastle_MonsterItems[iChaosCastleIndex][iA][1] ; iB++) {
			for (INT iC = 0 ; iC < MAX_CHAOSCASTLE_ITEM_DROP_LOOP ; iC++) {
				INT iMONSTER_INDEX = m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[rand()%iMAX_MONSTER];
				
				it = m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.find(iMONSTER_INDEX);
				
				if (it == m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.end()) {
					MID.m_iIndex = iMONSTER_INDEX;
					MID.m_iItemKind = g_iChaosCastle_MonsterItems[iChaosCastleIndex][iA][0];
					m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.insert(pair<INT, MONSTER_ITEM_DROP> (iMONSTER_INDEX, MID));
					break;
				}
			}
		}
	}
#endif // ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113
}


VOID CChaosCastle::SearchNDropMonsterItem (							// 각 캐슬의 몬스터들이 죽었을 때 위에서 세팅한 몬스터 인덱스에 아이템이 맞으면 해당 아이템을 드롭한다.
			INT iChaosCastleIndex,
			INT iMonsterIndex,
			INT iMaxHitUserIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if (!CHECK_LIMIT(iMonsterIndex, MAX_OBJECT))
		return;

	if (!CHECK_LIMIT(iMaxHitUserIndex, MAX_OBJECT))
		return;

	// 1 . 해당 몬스터 인덱스가 몬스터 아이템 리스트에 존재하는지 체크한다.
	map <INT, MONSTER_ITEM_DROP>::iterator it = m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.find(iMonsterIndex);
	if (it == m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.end()) {
		return;
	}

	// 2 . 만약 몬스터가 존재한다면 해당 아이템을 드롭한다.
	MONSTER_ITEM_DROP MID = it->second;
	INT iType = MID.m_iItemKind;
	INT iLevel = 0;
	
	// *> . 만약 몬스터가 떨어뜨리는 아이템이 열매라면 종류는 랜덤하게 정한다.
	if (iType == MAKE_ITEMNUM(13, 15)) {
		iLevel = rand()%4;		
	}

	ItemSerialCreateSend(gObj[iMaxHitUserIndex].m_Index, gObj[iMaxHitUserIndex].MapNumber, (BYTE)gObj[iMaxHitUserIndex].X, (BYTE)gObj[iMaxHitUserIndex].Y, iType, iLevel, (BYTE)0, 0, 0, 0, iMaxHitUserIndex);
	
	if (CHECK_LIMIT(iMaxHitUserIndex, MAX_OBJECT)) {
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
		LogAddTD("[Chaos Castle] (%d)(Event:%d) Monster Dropped ChaosCastle Item To [%s][%s] (Item:%s)", 
			iChaosCastleIndex+1,
			m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
			gObj[iMaxHitUserIndex].AccountID, 
			gObj[iMaxHitUserIndex].Name,
			ItemAttribute[iType].Name
			);
#else
		LogAddTD("[Chaos Castle] (%d) Monster Dropped ChaosCastle Item To [%s][%s] (Item:%s)", 
			iChaosCastleIndex+1,
			gObj[iMaxHitUserIndex].AccountID, 
			gObj[iMaxHitUserIndex].Name,
			ItemAttribute[iType].Name
			);
#endif
	}
}


/* 카오스 캐슬 공지관련 함수들 */
VOID CChaosCastle::SendAllUserAnyMsg(LPBYTE lpMsg, int iSize)		// 접속해있는 모든 사용자들에게 메시지를 보낸다.
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
VOID CChaosCastle::SendPCBangUserAnyMsg(LPBYTE lpMsg, int iSize)		// 접속해있는 모든 PC방 사용자들에게 메시지를 보낸다.
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER) && gObj[i].m_bPCBangFreeChaosCastleUser )
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}
#endif // #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307

VOID CChaosCastle::SendChaosCastleAnyMsg(LPBYTE lpMsg, int iSize, int iChaosCastleIndex )	// 특정 다리의 사용자들에게 메시지를 보낸다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	for (INT i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			if(gObjIsConnected(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex))
			{
				// 다리의 정식 등록자를 기준으로 카오스 캐슬맵에 없어도 살아만 있다면 메시지를 보내준다.
				if ((gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex != -1) && (gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex != -1))
					DataSend(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, (LPBYTE)lpMsg, iSize);
			}
		}
	}
}


VOID CChaosCastle::SendNoticeMessage (			// 특정 카오스 캐슬의 인원들에게 공지를 보낸다. (0x0D)
	INT iChaosCastleIndex,
	char * lpszMSG
	)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	PMSG_NOTICE	pNotice;
		
#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsgEx( &pNotice, 0, lpszMSG);
#else
	pNotice.type = 0;
	strcpy(pNotice.Notice, lpszMSG);
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if ((m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if ((gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex != -1) && (gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex != -1))
				DataSend(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		}
	}
}


VOID CChaosCastle::SendNoticeState (			// 특정 카오스 캐슬의 인원들에게 현재 경기진행 상황(시간, 몬스터수)을 보낸다.
	INT iChaosCastleIndex,
	INT iPlayState
	)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;


	PMSG_STATEBLOODCASTLE pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(pMsg));
	pMsg.btPlayState		= iPlayState;
	pMsg.wRemainSec			= m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / 1000;

	pMsg.wMaxKillMonster	= m_stChaosCastleData[iChaosCastleIndex].m_iCC_FIRST_USER_COUNT + m_stChaosCastleData[iChaosCastleIndex].m_iCC_FIRST_MONSTER_COUNT;
	pMsg.wCurKillMonster	= GetCurPlayUser(iChaosCastleIndex) + GetMonsterListCount(iChaosCastleIndex);

	// 아래 두 항목은 카오스 캐슬에서는 무시된다.
	pMsg.wUserHaveWeapon	= -1;
	pMsg.btWeaponNum		= -1;

	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if ((m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if ((gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex != -1) && (gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex != -1))
				DataSend(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}

}


VOID CChaosCastle::SendWinMessage (									// 특정 카오스 캐슬의 우승자에게 우승 메시지를 보내준다.
			INT iChaosCastleIndex,
			INT iWinnerIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if (!CHECK_LIMIT(iWinnerIndex, MAX_OBJECT))
		return;

	INT iChaosCastleSubIndex = gObj[iWinnerIndex].m_cChaosCastleSubIndex;

	if (!CHECK_LIMIT(iChaosCastleSubIndex, MAX_CHAOSCASTLE_USER_COUNT))
		return;

	if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_bSendQuitMsg == true)			// 이미 전에 메시지를 보냈다면 보내지 않는다.
		return;

	//g_LogToFile.Output("[CC_LOG] REWARD USER - WIN (%d-%d) : ENTER", iChaosCastleIndex+1, iChaosCastleSubIndex );

	RewardUserEXP (iChaosCastleIndex, gObj[iWinnerIndex].m_cChaosCastleSubIndex, TRUE);							// 여기서 승자에게 보상을 한다.
	
	GCServerMsgStringSend(lMsg.Get(1234), iWinnerIndex, 1);				// "근위병들의 영혼이 정화되었습니다. 퀘스트에 성공하였습니다."

	m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_bSendQuitMsg = true;

	//g_LogToFile.Output("[CC_LOG] REWARD USER - WIN (%d-%d) : EXIT", iChaosCastleIndex+1, iChaosCastleSubIndex );
}


VOID CChaosCastle::SendFailMessage (								// 특정 카오스 캐슬의 우승자를 제외한 나머지 인원들에게 실패 메시지를 보내준다.
			INT iChaosCastleIndex,
			INT iLoserIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if (!CHECK_LIMIT(iLoserIndex, MAX_OBJECT))
		return;

	INT iChaosCastleSubIndex = gObj[iLoserIndex].m_cChaosCastleSubIndex;

	if (!CHECK_LIMIT(iChaosCastleSubIndex, MAX_CHAOSCASTLE_USER_COUNT))
		return;

	if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_bSendQuitMsg == true)	// 이미 전에 메시지를 보냈다면 보내지 않는다.
		return;

	//g_LogToFile.Output("[CC_LOG] REWARD USER - FAIL (%d-%d) : ENTER", iChaosCastleIndex+1, iChaosCastleSubIndex );

	RewardUserEXP (iChaosCastleIndex, gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cChaosCastleSubIndex, FALSE);						// 여기서 패자에게 보상을 한다.

//	GCServerMsgStringSend("카오스 캐슬 퀘스트에 실패하였습니다. 다음 기회를 이용해 주세요.", m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex, 1);		// "퀘스트에 실패하였습니다. 다음기회를 이용해 주세요."

	m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_bSendQuitMsg = true;
	
	//g_LogToFile.Output("[CC_LOG] REWARD USER - FAIL (%d-%d) : EXIT", iChaosCastleIndex+1, iChaosCastleSubIndex );
}


VOID CChaosCastle::SendAllLoserFailMessage (						// 특정 카오스 캐슬의 우승자를 제외한 나머지 인원들에게 실패 메시지를 보내준다.
			INT iChaosCastleIndex,
			INT iWinnerIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if (!CHECK_LIMIT(iWinnerIndex, MAX_OBJECT) && (iWinnerIndex != -1))
		return;

	//g_LogToFile.Output("[CC_LOG] REWARD USER - FAIL ALL (%d) : ENTER", iChaosCastleIndex+1 );

	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 &&
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != iWinnerIndex &&
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_bSendQuitMsg == false
			) 
		{
			RewardUserEXP (iChaosCastleIndex, gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex, FALSE);				// 여기서 패자에게 보상을 한다.

			GCServerMsgStringSend(lMsg.Get(1235), m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, 1);		// "퀘스트에 실패하였습니다. 다음기회를 이용해 주세요."

			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_bSendQuitMsg = true;
		}
	}

	//g_LogToFile.Output("[CC_LOG] REWARD USER - FAIL ALL (%d) : EXIT", iChaosCastleIndex+1 );
}


/* 카오스 캐슬 사용자 괸리 함수들 */
INT CChaosCastle::EnterUserChaosCastle (							// iIndex의 사용자를 iChaosCastleIndex 번째 다리에 입방시킨다. (-1 리턴 시 방없음)	
			INT iChaosCastleIndex, 
			INT iUserIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	// PC방 전용 카오스 캐슬에서는 PC방 유저만 입장 가능
	if( m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT == CC_EVENT_PCBANG_FREE_ENTER )
	{
		if( !gObj[iUserIndex].m_bPCBangFreeChaosCastleUser )
			return -1;
	}
#endif
	
	INT iRET_VAL = -1;

	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			// 사용자의 중복 가입을 막는다.
			if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex == iUserIndex) {
				iRET_VAL = i;
				break;
			}
		}

		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex == -1) {
			iRET_VAL = i;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex = iUserIndex;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iEXP = 0;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iScore = 0;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iUserState = CC_USER_ALIVE;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_bSendQuitMsg	= false;
			m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER ++;

			if (m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER > MAX_CHAOSCASTLE_USER_COUNT)
				m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER = MAX_CHAOSCASTLE_USER_COUNT;

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
			// PC방 사용자인 경우 
			if( gObj[iUserIndex].m_bPCBangFreeChaosCastleUser )
			{
				m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_btEvent = CC_EVENT_PCBANG_FREE_ENTER;
			}
			else
			{
				m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_btEvent = CC_EVENT_GENERAL;
			}
#endif
			break;
		}
	}

	return iRET_VAL;
}


INT CChaosCastle::LeaveUserChaosCastle (							// iChaosCastleIndex 번째 다리의 iChaosCastleSubIndex 번째 사용자의 방을 뺀다. (iUserIndex 가 맞을때만)
			INT iChaosCastleIndex, 
			INT iUserIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;

	INT iRET_VAL = -1;

	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex == iUserIndex) {
			iRET_VAL = iUserIndex;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex		= -1;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iEXP			= 0;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iScore		= 0;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iUserState	= CC_USER_ALIVE;
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_bSendQuitMsg	= false;

			m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER --;

			if (m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER < 0)
				m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER = 0;

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_btEvent	= 0;
#endif				
			break;
		}
	}

	return iRET_VAL;
}


VOID CChaosCastle::AddFallUser (									// 폭발에 날아가서 떨어져 죽을 사용자들을 추가한다. (매번 사용자 리스트를 검색하기 힘들므로)
			INT iChaosCastleIndex,
			INT iUserIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	for (INT iFALL = 0 ; iFALL < MAX_CHAOSCASTLE_USER_COUNT ; iFALL++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL] == -1) {
			InterlockedExchange(&m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL], iUserIndex);
			break;
		}
	}
}


VOID CChaosCastle::ProcessFallUser (								// 떨어져 사망할 사용자의 목록에 있는 대로 사망처리를 한다.
			INT iChaosCastleIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	for (INT iFALL = 0 ; iFALL < MAX_CHAOSCASTLE_USER_COUNT ; iFALL++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL] != -1) {
			INT iFallUserIndex = m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL];
			InterlockedExchange(&m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL], -1);

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
			if ((gObj[iFallUserIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) ) && (gObjIsConnected(iFallUserIndex)) && (gObj[iFallUserIndex].Life > 0)) {
#else
			if ((gObj[iFallUserIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex) && (gObjIsConnected(iFallUserIndex)) && (gObj[iFallUserIndex].Life > 0)) {
#endif
				// 땅뚫림 속성안에 있는 사용자는 죽인다.
				gObj[iFallUserIndex].Life			= 0.0;
				gObj[iFallUserIndex].m_State		= OBJST_DYING;
				gObj[iFallUserIndex].KillerType		= KT_MONSTER;
				gObj[iFallUserIndex].RegenTime		= GetTickCount();
				gObj[iFallUserIndex].DieRegen		= 1;
				gObj[iFallUserIndex].PathCount		= 0;
				
#ifdef ADD_SKILL_WITH_COMBO
				GCDiePlayerSend(&gObj[iFallUserIndex], gObj[iFallUserIndex].m_Index, 0, 0);
#else
				GCDiePlayerSend(&gObj[iFallUserIndex], gObj[iFallUserIndex].m_Index);
#endif
			}
		}
	}
}


void CChaosCastle::SetUserState(int iUserIndex, int iState)
{
	if (!CHECK_LIMIT(iUserIndex, MAX_OBJECT))
		return;

	int iChaosCastleIndex		= gObj[iUserIndex].m_cChaosCastleIndex;
	int iChaosCastleSubIndex	= gObj[iUserIndex].m_cChaosCastleSubIndex;

	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if (!CHECK_LIMIT(iChaosCastleSubIndex, MAX_CHAOSCASTLE_USER_COUNT))
		return;

	switch (iState) {
	case CC_USER_ALIVE :			// 사실상 거의 쓰이지 않음.
		{
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iUserState = CC_USER_ALIVE;
		}
		break;
	case CC_USER_DEAD :				// 퀘스트 아이템 드롭은 -> gObjUserDie() 에서
		{
			m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iUserState = CC_USER_DEAD;
		}
		break;
	}
}


INT	CChaosCastle::CalcSendRewardEXP	(								// 경기 종료 후 경험치를 계산하고 해당 사용자에게 보내준다.
			INT iUserIndex, 
			INT iEXP,
			INT iKILLCOUNT_USER,
			INT iKILLCOUNT_MONSTER
			)
{
	if (iEXP <= 0)
		return 0;
	
#ifdef MU_CRYWOLF_PENALTY_20051215
	// MVP 실패 : 경험치 획득 감소 
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
		&& g_iCrywolfApplyMvpPenalty	
	  )
	{
		iEXP = iEXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
	}
#endif

#ifdef BUGFIX_GAIN_EXP_20071210
	INT64 iRET_EXP = 0;
	INT64 iCAL_EXP = iEXP;	
#else
	INT iRET_EXP = 0;
	INT iCAL_EXP = iEXP;
#endif
	INT iMAX_LEVCOUNT = 0;

	if (!gObjIsConnected(iUserIndex))
		return 0;

	iRET_EXP = iCAL_EXP;

	if( gObj[iUserIndex].Type == OBJTYPE_CHARACTER ) {
		while (iCAL_EXP > 0) {
			if (iCAL_EXP > 0) {
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// 중국변경> . 전체 몬스터 아이템 드롭 확률을 조정함.
				if (g_bChnPlayTimeLimitOn)
				{
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
					if( gObj[iUserIndex].m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
					{
						if (gObj[iUserIndex].m_iPlayTimeLimit >= 5*60*60) {
							iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel2Exp / 100;
						}
						else if (gObj[iUserIndex].m_iPlayTimeLimit >= 3*60*60) {
							iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel1Exp / 100;
						}
					}
	#else
					if (gObj[iUserIndex].m_iPlayTimeLimit >= 5*60*60) {
						iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel2Exp / 100;
					}
					else if (gObj[iUserIndex].m_iPlayTimeLimit >= 3*60*60) {
						iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel1Exp / 100;
					}
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
				}
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// 베트남 : 카오스캐슬 경험치 획득량 조정
				if (g_bVtmPlayTimeLimitOn) 
				{
					if (gObj[iUserIndex].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time * 60) 
					{
						iCAL_EXP = iCAL_EXP * g_iVtmPlayTimeLimitLevel2Exp / 100;
					}
					else if (gObj[iUserIndex].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time * 60) 
					{
						iCAL_EXP = iCAL_EXP * g_iVtmPlayTimeLimitLevel1Exp / 100;
					}
				}
#endif
				
#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
				CheckItemOptForGetExpEx(&gObj[iUserIndex], iCAL_EXP, TRUE);
				
				// 계산이 끝난 보상 경험치에 경험치 비율을 적용시킴
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_stChaosCastleData[gObj[iUserIndex].m_cChaosCastleIndex].m_fRewardExpRate;
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				
				iRET_EXP = (int)iCAL_EXP;
#else
//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC 적용 후 아래코드 삭제할 예정
#ifdef MODIFY_EVENTMAP_REWARD_BUGFIX_20080404	// 보상경험치에 유지의 인장만 적용
				if( gObjCheckUsedBuffEffect( &gObj[iUserIndex], BUFFTYPE_PCBANG_POINT_MARK3 ))
				{
					iCAL_EXP = 0;
				}
				
		// 계산이 끝난 보상 경험치에 경험치 비율을 적용시킴
		#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_stChaosCastleData[gObj[iUserIndex].m_cChaosCastleIndex].m_fRewardExpRate;
		#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
								
				iRET_EXP = iCAL_EXP;
#else	// MODIFY_EVENTMAP_REWARD_BUGFIX_20080404				
#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
				g_PCBangPointSystem.CheckItemOptForGetExp( &gObj[iUserIndex], iCAL_EXP );

		// 계산이 끝난 보상 경험치에 경험치 비율을 적용시킴
		#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_stChaosCastleData[gObj[iUserIndex].m_cChaosCastleIndex].m_fRewardExpRate;
		#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				
				iRET_EXP = iCAL_EXP;
#else				
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
				CheckItemOptForGetExp(&gObj[iUserIndex], iCAL_EXP, TRUE);
	#else 
				//20080416 - 아래 코드는 삭제해도 될것 같음
				int iExpRate = 0;
				iExpRate = gObjGetTotalValueOfEffect( &gObj[iUserIndex], EFFECTTYPE_EXPERIENCE );
				if( gObjCheckUsedBuffEffect( &gObj[iUserIndex], BUFFTYPE_PCBANG_POINT_MARK3 ) == false
					&& iExpRate == 0
					)
				{
					iExpRate = 100;
				}

			// 계산이 끝난 보상 경험치에 경험치 비율을 적용시킴
			#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_stChaosCastleData[gObj[iUserIndex].m_cChaosCastleIndex].m_fRewardExpRate;
			#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

				iCAL_EXP = ( iCAL_EXP * iExpRate ) / 100;
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

				iRET_EXP = iCAL_EXP;
#else

		// 계산이 끝난 보상 경험치에 경험치 비율을 적용시킴
		#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_stChaosCastleData[gObj[iUserIndex].m_cChaosCastleIndex].m_fRewardExpRate;
		#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

				iRET_EXP = iCAL_EXP;

#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
#endif	// MODIFY_EVENTMAP_REWARD_BUGFIX_20080404
//<--
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC


#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
				iCAL_EXP = LevelUp(iUserIndex, iCAL_EXP, EVENT_TYPE_CHAOSCASTLE );
#else
				iCAL_EXP = LevelUp(iUserIndex, iCAL_EXP);
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
			}
			iMAX_LEVCOUNT++;

			if (iMAX_LEVCOUNT > 1000)
				break;
		}

	#ifdef BUGFIX_GAIN_EXP_20071210
		// 사용자에게 보상경험치를 보낸다.
		// *마스터레벨 유저의 경험치는 이미 보내졌다.
		if( !g_MasterLevelSystem.IsMasterLevelUser( &gObj[iUserIndex] ) )
		{
			GCSendExp_INT64( iUserIndex, 0xFFFF, iRET_EXP, 0, 0 );
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

		// 사용자에게 보상경험치를 보낸다.
		DataSend(iUserIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);
	#endif	// BUGFIX_GAIN_EXP_20071210
	}

	return iRET_EXP;
}

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 사용자가 어떤 카오스 캐슬에 들어갈 수 있는지 레벨을 반환한다.
INT	CChaosCastle::GetUserLevelToEnter (								// 사용자가 어떤 카오스 캐슬에 들어갈 수 있는지 레벨을 반환한다.
			INT iUserIndex
			)
{
	if( !gObjIsConnected( iUserIndex ) )
		return -1;

	INT iENTER_LEVEL = -1;

	if( g_MasterLevelSystem.IsMasterLevelUser( &gObj[iUserIndex] ) )
	{
		// 마스터레벨 유저는 7카캐
		iENTER_LEVEL = MAX_CHAOSCASTLE_COUNT - 1;
	}
	else
	{
		for (INT i = 0 ; i < MAX_CHAOSCASTLE_COUNT - 1 ; i++) 
		{
			if( gObj[iUserIndex].Class == CLASS_DARKLORD || gObj[iUserIndex].Class == CLASS_MAGUMSA ) 
			{
				if( ( gObj[iUserIndex].Level >= g_sttCHAOSCASTLE_LEVEL[i].iLOWER_BOUND_MAGUMSA ) &&
					( gObj[iUserIndex].Level <= g_sttCHAOSCASTLE_LEVEL[i].iUPPER_BOUND_MAGUMSA )
					)
				{
					iENTER_LEVEL = i;
					break;
				}
			}
			else 
			{
				if( ( gObj[iUserIndex].Level >= g_sttCHAOSCASTLE_LEVEL[i].iLOWER_BOUND ) &&
					( gObj[iUserIndex].Level <= g_sttCHAOSCASTLE_LEVEL[i].iUPPER_BOUND )
					)
				{
					iENTER_LEVEL = i;
					break;
				}
			}
		}
	}

	return iENTER_LEVEL;
}
#else
INT	CChaosCastle::GetUserLevelToEnter (								// 사용자가 어떤 카오스 캐슬에 들어갈 수 있는지 레벨을 반환한다.
			INT iUserIndex
			)
{
	if (!gObjIsConnected(iUserIndex))
		return -1;

	INT iENTER_LEVEL = -1;

	for (INT i = 0 ; i < MAX_CHAOSCASTLE_COUNT ; i++) {

#ifdef DARKLORD_WORK
		if (gObj[iUserIndex].Class == CLASS_DARKLORD || gObj[iUserIndex].Class == CLASS_MAGUMSA) {
#else
		if(gObj[iUserIndex].Class == CLASS_MAGUMSA ) {
#endif

			if ((gObj[iUserIndex].Level >= g_sttCHAOSCASTLE_LEVEL[i].iLOWER_BOUND_MAGUMSA) &&
				(gObj[iUserIndex].Level <= g_sttCHAOSCASTLE_LEVEL[i].iUPPER_BOUND_MAGUMSA)
				)
			{
				iENTER_LEVEL = i;
				break;
			}
		}
		else {
			if ((gObj[iUserIndex].Level >= g_sttCHAOSCASTLE_LEVEL[i].iLOWER_BOUND) &&
				(gObj[iUserIndex].Level <= g_sttCHAOSCASTLE_LEVEL[i].iUPPER_BOUND)
				)
			{
				iENTER_LEVEL = i;
				break;
			}
		}
	}

	return iENTER_LEVEL;
}
#endif	// ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010

BOOL CChaosCastle::CheckUserEnterMoney (							// 사용자의 충분한 입장료를 가지고 있는지 확인한다.
			INT iUserIndex, 
			INT iEnterLevel
			)
{
	if(!CHECK_LIMIT( iUserIndex, MAX_OBJECT ) )
		return FALSE;

	if (!CHECK_LIMIT(iEnterLevel, MAX_CHAOSCASTLE_COUNT))
		return FALSE;

	if (gObj[iUserIndex].Money >= g_iChaosCastle_EnterCost[iEnterLevel] ) {
		return TRUE;
	}

	return FALSE;
}


BOOL CChaosCastle::PayUserEnterMoney (								// 사용자의 입장료를 지불 시킨다 (FALSE면 에러가나 돈이 모자람)
			INT iUserIndex,
			INT iEnterLevel
			)
{
	if(!CHECK_LIMIT( iUserIndex, MAX_OBJECT ) )
		return FALSE;

	if (!CHECK_LIMIT(iEnterLevel, MAX_CHAOSCASTLE_COUNT))
		return FALSE;

	if (gObj[iUserIndex].Money >= g_iChaosCastle_EnterCost[iEnterLevel] ) {
		gObj[iUserIndex].Money -= g_iChaosCastle_EnterCost[iEnterLevel];
		GCMoneySend(iUserIndex, gObj[iUserIndex].Money);
		return TRUE;
	}

	return FALSE;
}


VOID CChaosCastle::GiveUserDamage (									// 사용자에게 일정량의 데미지를 준다. (데미지 수치도 클라이언트로 보내주며, 죽음도 자체 처리한다)
			INT iUserIndex, 
			INT iDamage
			)
{
	if (!gObjIsConnected(iUserIndex))
		return;

	if (gObj[iUserIndex].Life <= 0.0)
		return;

	PMSG_ATTACKRESULT pResult;

	// [0x15] 공격을 가한/당한 유저에게 결과를 알린다.
#ifdef PACKET_CHANGE	
	PHeadSetB((LPBYTE)&pResult, PACKET_ATTACK, sizeof( pResult ));
#else
	PHeadSetB((LPBYTE)&pResult, 0x15, sizeof( pResult ));
#endif
	
	pResult.NumberH = HIBYTE(iUserIndex);
	pResult.NumberL = LOBYTE(iUserIndex);
	pResult.DamageH = HIBYTE(iDamage);
	pResult.DamageL = LOBYTE(iDamage);
	
#ifdef ADD_SHIELD_POINT_01_20060403
	#ifdef MODIFY_SHIELDDAMAGE_BUGFIX_IN_CHAOSCASTLE_20060516
		pResult.btShieldDamageH = 0;
		pResult.btShieldDamageL = 0;
	#endif // MODIFY_SHIELDDAMAGE_BUGFIX_IN_CHAOSCASTLE_20060516
#endif // ADD_SHIELD_POINT_01_20060403

	if( gObj[iUserIndex].Type == OBJTYPE_CHARACTER )
		DataSend(iUserIndex, (LPBYTE)&pResult, pResult.h.size);

	gObj[iUserIndex].Life -= iDamage;

	// 생명력이 0이면 죽인다.
	if (gObj[iUserIndex].Life <= 0.0) {
		gObj[iUserIndex].Life			= 0.0;
		gObj[iUserIndex].m_State		= OBJST_DYING;
		gObj[iUserIndex].KillerType		= KT_MONSTER;
		gObj[iUserIndex].RegenTime		= GetTickCount();
		gObj[iUserIndex].DieRegen		= 1;
		gObj[iUserIndex].PathCount		= 0;
		
#ifdef ADD_SKILL_WITH_COMBO
		GCDiePlayerSend(&gObj[iUserIndex], gObj[iUserIndex].m_Index, 0, 0);
#else
		GCDiePlayerSend(&gObj[iUserIndex], gObj[iUserIndex].m_Index);
#endif
	}
}


#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
INT	CChaosCastle::LevelUp (int iUserIndex, int iAddExp, int iEventType)
#else
INT	CChaosCastle::LevelUp (int iUserIndex, int iAddExp)
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
{
	if(!CHECK_LIMIT( iUserIndex, MAX_OBJECT ) )
		return 0;

	INT iLEFT_EXP = 0;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 카오스캐슬 MasterLevelUp 처리
	// 마스터레벨은 이쪽으로.
	if( g_MasterLevelSystem.MasterLevelUp( &gObj[iUserIndex], iAddExp, true ) )
		return 0;

	// 펫 아이템 경험치 분배	
	gObjSetExpPetItem( iUserIndex, iAddExp );
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912

#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %u %d MonsterIndex : %d, EventType : %d",
		gObj[iUserIndex].MapNumber, gObj[iUserIndex].X, gObj[iUserIndex].Y,
		gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, gObj[iUserIndex].Level, gObj[iUserIndex].Experience, iAddExp,
		0, iEventType);
#endif

	// 최대 레벨을 MAX_LEVEL로 한다.
	if( gObj[iUserIndex].Level >= g_GlobalConfig.m_iMaxLevel )
	{
		GCServerMsgStringSend(lMsg.Get(1136), gObj[iUserIndex].m_Index, 1);
		return 0;
	}


	if( gObj[iUserIndex].Experience + iAddExp < gObj[iUserIndex].NextExp ) {
		gObj[iUserIndex].Experience += iAddExp;
	}
	else {
		iLEFT_EXP = gObj[iUserIndex].Experience + iAddExp - gObj[iUserIndex].NextExp;

		gObj[iUserIndex].Experience = gObj[iUserIndex].NextExp;
		gObj[iUserIndex].Level += 1;

#ifdef DARKLORD_WORK
		if (gObj[iUserIndex].Class == CLASS_DARKLORD || gObj[iUserIndex].Class == CLASS_MAGUMSA) 
#else
		if(gObj[iUserIndex].Class == CLASS_MAGUMSA )
#endif
			gObj[iUserIndex].LevelUpPoint += g_GlobalConfig.m_iSpecial_Points;
		else
			gObj[iUserIndex].LevelUpPoint += g_GlobalConfig.m_iNormal_Points;
		
#ifdef SECOND_QUEST	
		if( gObj[iUserIndex].PlusStatQuestClear )
		{	// +1스탯업 퀘스트 성공시 추가 스탯 
			gObj[iUserIndex].LevelUpPoint += g_GlobalConfig.m_iPowerUP_Points;
			LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, gObj[iUserIndex].LevelUpPoint);
		}
#endif		
			
		gObj[iUserIndex].MaxLife += DCInfo.DefClass[gObj[iUserIndex].Class].LevelLife;

		if(g_GlobalConfig.m_bLimitLife && gObj[iUserIndex].MaxLife > g_GlobalConfig.m_iLimitLife)
			gObj[iUserIndex].MaxLife = g_GlobalConfig.m_iLimitLife;

		gObj[iUserIndex].MaxMana += DCInfo.DefClass[gObj[iUserIndex].Class].LevelMana;
		gObj[iUserIndex].Life = gObj[iUserIndex].MaxLife;
		gObj[iUserIndex].Mana = gObj[iUserIndex].MaxMana;	
		gObjNextExpCal(&gObj[iUserIndex]);

		gObjSetBP(gObj[iUserIndex].m_Index);
	
	
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
		GCLevelUpMsgSend(gObj[iUserIndex].m_Index);
#else
		short AddPoint = 0;
		short MaxAddPoint = 0;
		gObjGetStatPointState(gObj[iUserIndex].m_Index, AddPoint, MaxAddPoint);
		GCLevelUpMsgSend(gObj[iUserIndex].m_Index, gObj[iUserIndex].Level, gObj[iUserIndex].LevelUpPoint, 
						 (WORD)(gObj[iUserIndex].MaxLife+gObj[iUserIndex].AddLife), 
						 (WORD)(gObj[iUserIndex].MaxMana+gObj[iUserIndex].AddMana), 
						 gObj[iUserIndex].MaxBP+gObj[iUserIndex].AddBP,
						 AddPoint, MaxAddPoint);
#endif // ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712

		gObjCalcMaxLifePower(gObj[iUserIndex].m_Index);	// 최대 생명력을 다시 계산한다

		LogAddTD(lMsg.Get(520), gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, gObj[iUserIndex].Level);

#ifdef ADD_LOG_400LEVELUP_PARTY_20070628	// 400레벨 달성시 같이 있던 파티원들 로그 추가
		if(gObj[iUserIndex].Level == 400 && gObj[iUserIndex].PartyNumber >=0)
		{
			int partynum = gObj[iUserIndex].PartyNumber;
			char partyLog[256];
			int number;
			sprintf(partyLog, "400 LevelUp (%s)(%s) Party ", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name);
			
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
//	GJSetCharacterInfo(&gObj[iUserIndex], gObj[iUserIndex].m_Index);

	//LogAdd("캐릭터 저장 메시지 보냄");

	return iLEFT_EXP;
}


INT CChaosCastle::GetCurrentWinUser (								// 현재 경기에서 이긴 사람을 찾아낸다. (-1) 이면 없음
			INT iChaosCastleIndex
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;

	INT iCC_SCORE = -1;
	INT iCC_WINNER_INDEX = -1;

	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			if ((gObjIsConnected (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE) &&
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) ) &&
#else
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex) &&
#endif
				(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iUserState == CC_USER_ALIVE)
				)
			{
				LPOBJECTSTRUCT lpObj = &gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex];
				INT iCC_TEMP_SCORE = lpObj->m_cKillUserCount * CHAOSCASTLE_KILL_USER_PT + lpObj->m_cKillMonsterCount * CHAOSCASTLE_KILL_MONSTER_PT;
				if (iCC_SCORE < iCC_TEMP_SCORE) {
					iCC_SCORE = iCC_TEMP_SCORE;
					iCC_WINNER_INDEX = lpObj->m_Index;
				}
			}
		}	
	}

	if (iCC_WINNER_INDEX != -1) {
		// 이 사람의 스코어가 0점이라면 1등이 될 수 없다.
		if (gObj[iCC_WINNER_INDEX].m_cKillUserCount == 0 && gObj[iCC_WINNER_INDEX].m_cKillMonsterCount == 0) {
			// 하자만 만일 혼자 남은 것이라면 1등이다.
			INT iCUR_USER_COUNT = GetCurPlayUser(iChaosCastleIndex);
			INT iCUR_MONSTER_COUNT = GetMonsterListCount(iChaosCastleIndex);

			if (iCUR_USER_COUNT != 1 || iCUR_MONSTER_COUNT != 0)
				iCC_WINNER_INDEX = -1;
		}
	}

	return iCC_WINNER_INDEX;
}


/* 카오스 캐슬 관련 각종 상태체크 함수들 */
INT	CChaosCastle::CheckEnterLevel		(INT iIndex, INT iLevel)				// 해당 사용자가 카오스 캐슬에 입장할 수 있는지 확인 (0:가능, -1:낮다, +1:높다, 2:다른에러)
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 2;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return 2;

#ifdef DARKLORD_WORK
	if (gObj[iIndex].Class == CLASS_DARKLORD||gObj[iIndex].Class == CLASS_MAGUMSA) {
#else
	if (gObj[iIndex].Class == CLASS_MAGUMSA) {
#endif
		if ( (gObj[iIndex].Level >= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA) && (gObj[iIndex].Level <= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA))
			return 0;		// 갈수있다.
		else if (gObj[iIndex].Level < g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA)
			return -1;		// 내 레벨이 낮다.
		else if (gObj[iIndex].Level > g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA)
			return 1;		// 내 레벨이 높다.
	}
	else {
		if ( (gObj[iIndex].Level >= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND) && (gObj[iIndex].Level <= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND))
			return 0;		// 갈수있다.
		else if (gObj[iIndex].Level < g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND)
			return -1;		// 내 레벨이 낮다.
		else if (gObj[iIndex].Level > g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND)
			return 1;		// 내 레벨이 높다.
	}

	return 2;
}


INT	CChaosCastle::CheckEnterItem		(INT iIndex)							// 해당 사용자가 카오스 캐슬에 입장할 수 있는 아이템 (근위병의 갑옷세트) 이 있는지 확인 (0 이면 불가 / 1 ~ n 까지 근위병의 갑옷세트 레벨)
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
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 29) )		// 근위병의 갑옷세트가 존재한다면
			{
				iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;				// 아이템 레벨을 얻는다.
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 근위병의갑옷세트 레벨 체크 변경
				if (!CHECK_LIMIT(iITEM_LEVEL, MAX_CHAOSCASTLE_COUNT+1))
#else
				if (!CHECK_LIMIT(iITEM_LEVEL, 8))
#endif
					iITEM_LEVEL = 0;

				if (iITEM_LEVEL)	return iITEM_LEVEL;
			}
		}
	}

	return iITEM_LEVEL;
}


INT	CChaosCastle::CheckPlayEnded		(INT iChaosCastleIndex)					// 현재 카오스 캐슬 경기가 종료했는지 체크한다. (0:진행중, 1:성공, 2:실패)
{
	// P.S.> 이 메서드에서는 시간을 제외한 몬스터 수, 사용자 수 등의 조건만 체크한다.
	
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return false;

	INT iCUR_USER_COUNT = GetCurPlayUser(iChaosCastleIndex);
	INT iCUR_MONSTER_COUNT = GetMonsterListCount(iChaosCastleIndex);

	if (iCUR_USER_COUNT <= 0) {
		// 남은 사람이 하나도 없다면 실패이다.
		return CHAOSCASTLE_PLAYCHECK_FAILED;
	}

	if (iCUR_USER_COUNT == 1 && iCUR_MONSTER_COUNT <= 0) {
		// 남은 몬스터가 하나도 없고 사람만 1명 남았다면 성공이다.
		return CHAOSCASTLE_PLAYCHECK_SUCCESS;
	}

	return CHAOSCASTLE_PLAYCHECK_ING;
}

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
bool CChaosCastle::CheckCanEnter		( INT iUserIndex,INT iChaosCastleIndex )				// 현재 카오스 캐슬 입장이 가능한지 확인한다.
{
	if( !CHECK_LIMIT( iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT ) )
		return false;
	
	if( m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT == CC_EVENT_PCBANG_FREE_ENTER )
	{
		if( !gObj[iUserIndex].m_bPCBangFreeChaosCastleUser )
		{
			return false;
		}
	}

	return m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER;
}

#else

bool CChaosCastle::CheckCanEnter		(INT iChaosCastleIndex)					// 현재 카오스 캐슬 입장이 가능한지 확인한다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return false;

	return m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER;
}
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307

bool CChaosCastle::CheckPlayStart		(INT iChaosCastleIndex)					// 현재 카오스 캐슬 경기가 시작되었는지 확인한다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return false;

	return m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START;
}


bool CChaosCastle::CheckCanStartPlay	(INT iChaosCastleIndex)					// 현재 카오스 캐슬 경기를 시작할 수 있는 상태인지 확인한다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return false;

	// 입장 인원 수가 5명 이상인지 체크한다.
	INT iEnteredUser = GetCurPlayUser	(iChaosCastleIndex);

	if (iEnteredUser < m_iMinUserCount) {
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
		LogAddTD("[Chaos Castle] (%d)(Event:%d) GetCurPlayUser() FAILED (UserCount:%d)", 
			iChaosCastleIndex+1,
			m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
			iEnteredUser
			);
#else
		LogAddTD("[Chaos Castle] (%d) GetCurPlayUser() FAILED (UserCount:%d)", 
			iChaosCastleIndex+1,
			iEnteredUser
			);		
#endif

		return false;
	}

	return true;
}


INT	CChaosCastle::GetCurrentState		(INT iChaosCastleIndex)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;

	return m_stChaosCastleData[iChaosCastleIndex].m_iCC_STATE;
}


INT CChaosCastle::GetCurEnteredUser		(INT iChaosCastleIndex)				// 현재 입장해 있는 사용자의 수를 반환
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;
	
	return m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER;
}


INT CChaosCastle::GetCurPlayUser		(INT iChaosCastleIndex)				// 현재 실제로 입장해서 플레이 준비를 하고있는 사용자의 수를 반환
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;

	INT iPlayUser = 0;

	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			if ((gObjIsConnected (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE) &&
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) )
#else
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex)
#endif
				)
			{
				iPlayUser++;
			}
		}	
	}

	return iPlayUser;
}

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
INT	CChaosCastle::GetCurEventUser	(											// 현재 입장한 사용자 중 이벤트별 사용자 수를 반환
				INT iChaosCastleIndex,
				BYTE btEvent 
				)
{
	if( !CHECK_LIMIT( iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT ) )
		return -1;

	INT iPlayEventUser = 0;

	for( int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT; i++ ) 
	{
		if( m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 ) 
		{
			if( ( gObjIsConnected( m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex ) == TRUE ) 
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
				&& ( gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) )
#else
				&& ( gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex )
#endif
				&& ( m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_btEvent == btEvent )			
				)
			{
				iPlayEventUser++;
			}
		}	
	}

	return iPlayEventUser;
}

BYTE CChaosCastle::GetCurrentEvent		(INT iChaosCastleIndex)		// 현재 진행되고 있는 이벤트 타입 반환
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;
	
	return m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT;	
}
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
INT CChaosCastle::GetRemainTime	(					// 열리기 까지 남은 시간을 확인한다. (분단위)
				INT iUserIndex, 
				INT iChaosCastleIndex
				)
{
	INT iREMAIN_MINUTE = 0;
	
	if( this->GetCurrentState( iChaosCastleIndex ) == CHAOSCASTLE_STATE_CLOSED &&
		this->GetCurrentEvent( iChaosCastleIndex ) == CC_EVENT_GENERAL )			// 일반 카오스캐슬 일 때는 바로 적용
	{
		iREMAIN_MINUTE = ( m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / ( 60 * 1000 ) - m_iCC_TIME_MIN_OPEN) + 1;
	}
	else 
	{
		SYSTEMTIME sysTime;
		GetLocalTime (& sysTime);

		INT iMIN_HOUR = 24;
		INT iMIN_MINUTE = 60;
		BOOL bTIME_CHANGED = FALSE;

		vector <CHAOSCASTLE_START_TIME>::iterator it = m_vtChaosCastleOpenTime.begin();

		for (;it != m_vtChaosCastleOpenTime.end(); it++) 
		{
			CHAOSCASTLE_START_TIME & pRET = *it;
			if (sysTime.wHour * 60 + sysTime.wMinute < pRET.m_iHour * 60 +pRET.m_iMinute)
			{
				if (iMIN_HOUR * 60 + iMIN_MINUTE > pRET.m_iHour * 60 +pRET.m_iMinute) 
				{
					if( pRET.m_btEvent == CC_EVENT_PCBANG_FREE_ENTER )		// PC방 무료입장 시간
					{
						if( gObj[iUserIndex].m_bPCBangFreeChaosCastleUser )	// PC방 이용자
						{
							bTIME_CHANGED = TRUE;
					
							iMIN_HOUR = pRET.m_iHour;
							iMIN_MINUTE = pRET.m_iMinute;
						}
					}
					else if( pRET.m_btEvent ==  CC_EVENT_GENERAL )			// 일반 카오스 캐슬
					{
						bTIME_CHANGED = TRUE;
					
						iMIN_HOUR = pRET.m_iHour;
						iMIN_MINUTE = pRET.m_iMinute;						
					}
				}
			}
		}

		if (bTIME_CHANGED == FALSE) 
		{
			it = m_vtChaosCastleOpenTime.begin();
			iMIN_HOUR = 24;
			iMIN_MINUTE = 60;
			
			for( ; it != m_vtChaosCastleOpenTime.end(); it++ ) 
			{
				CHAOSCASTLE_START_TIME & pRET = *it;
				if( iMIN_HOUR * 60 + iMIN_MINUTE > pRET.m_iHour * 60 + pRET.m_iMinute )
				{
					if( pRET.m_btEvent == CC_EVENT_PCBANG_FREE_ENTER )			// PC방 무료입장 시간
					{
						if( gObj[iUserIndex].m_bPCBangFreeChaosCastleUser )		// PC방 이용자
						{					
							bTIME_CHANGED = TRUE + 1;
							
							iMIN_HOUR = pRET.m_iHour;
							iMIN_MINUTE = pRET.m_iMinute;
						}
					}
					else if( pRET.m_btEvent ==  CC_EVENT_GENERAL )				// 일반 카오스 캐슬
					{
						bTIME_CHANGED = TRUE + 1;
						
						iMIN_HOUR = pRET.m_iHour;
						iMIN_MINUTE = pRET.m_iMinute;
					}
				}
			}
		}

		switch( bTIME_CHANGED ) 
		{
		case TRUE :			// 현재 시간보다 앞으로 진행될 시간이 크다.
			iREMAIN_MINUTE = ( ( ( 60 * iMIN_HOUR ) * 60 + iMIN_MINUTE * 60 ) - ( ( ( ( 60 * sysTime.wHour ) * 60 ) + sysTime.wMinute * 60 + sysTime.wSecond ) ) ) / 60 + 1;
			break;
		case TRUE + 1 :		// 현재 시간보다 앞으로 진행될 시간이 작다 (내일 할 예정)
			iREMAIN_MINUTE = ( ( ( 60 * ( iMIN_HOUR + 24 ) ) * 60 + iMIN_MINUTE * 60 ) - ( ( ( ( 60 * sysTime.wHour ) * 60 ) + sysTime.wMinute * 60 + sysTime.wSecond ) ) ) / 60 + 1;
			break;
		default:			// 에러
			iREMAIN_MINUTE = -1;
		}
	}		

	if( iREMAIN_MINUTE < 0 )
		iREMAIN_MINUTE = 0;

	return iREMAIN_MINUTE;
}

#else	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				
INT CChaosCastle::GetRemainTime			(int iChaosCastleIndex)
{
	INT iREMAIN_MINUTE = 0;

	if (this->GetCurrentState(iChaosCastleIndex) == CHAOSCASTLE_STATE_CLOSED) {
		iREMAIN_MINUTE = (m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / (60 * 1000) - m_iCC_TIME_MIN_OPEN) + 1;
	}
	else {
		SYSTEMTIME sysTime;
		GetLocalTime (& sysTime);

		INT iMIN_HOUR = 24;
		INT iMIN_MINUTE = 60;
		BOOL bTIME_CHANGED = FALSE;

		vector <CHAOSCASTLE_START_TIME>::iterator it = m_vtChaosCastleOpenTime.begin();

		for (;it != m_vtChaosCastleOpenTime.end();it++) {
			CHAOSCASTLE_START_TIME & pRET = *it;
			if (sysTime.wHour * 60 + sysTime.wMinute < pRET.m_iHour * 60 +pRET.m_iMinute)
			{
				if (iMIN_HOUR * 60 + iMIN_MINUTE > pRET.m_iHour * 60 +pRET.m_iMinute) {
					bTIME_CHANGED = TRUE;
					
					iMIN_HOUR = pRET.m_iHour;
					iMIN_MINUTE = pRET.m_iMinute;
				}
			}
		}

		if (bTIME_CHANGED == FALSE) {
			it = m_vtChaosCastleOpenTime.begin();
			iMIN_HOUR = 24;
			iMIN_MINUTE = 60;
			
			for (;it != m_vtChaosCastleOpenTime.end();it++) {
				CHAOSCASTLE_START_TIME & pRET = *it;
				if (iMIN_HOUR * 60 + iMIN_MINUTE > pRET.m_iHour * 60 +pRET.m_iMinute)
				{
					bTIME_CHANGED = TRUE+1;
					
					iMIN_HOUR = pRET.m_iHour;
					iMIN_MINUTE = pRET.m_iMinute;
				}
			}
		}

		switch(bTIME_CHANGED) {
		case TRUE :			// 현재 시간보다 앞으로 진행될 시간이 크다.
			iREMAIN_MINUTE = ( ((60*iMIN_HOUR) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) / 60 + 1;
			break;
		case TRUE + 1 :		// 현재 시간보다 앞으로 진행될 시간이 작다 (내일 할 예정)
			iREMAIN_MINUTE = ( ((60*(iMIN_HOUR+24)) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) / 60 + 1;
			break;
		default:			// 에러
			iREMAIN_MINUTE = -1;
		}
	}

	if (iREMAIN_MINUTE < 0)
		iREMAIN_MINUTE = 0;

	return iREMAIN_MINUTE;
}
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307


INT CChaosCastle::GetCurrentRemainSec	(INT iChaosCastleIndex)				// 단지 다음상태까지 가기위한 초를 반환
{
	return m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / 1000;
}


/* 카오스 캐슬 관련 게임진행 함수들 */
BOOL CChaosCastle::ObjSetPosition		(INT iIndex, INT iX, INT iY)		// 특정 오브젝트 (사용자, 몬스터)의 위치를 특정 위치로 강제 지정
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT)) {
		// 사용자가 접속상태가 아니라면 무시
		return TRUE;
	}

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if (!CHECK_CHAOSCASTLE(lpObj->MapNumber))
		return FALSE;

/*
	if (lpObj->X == iX && lpObj->Y == iY) {
		// 현 위치와 이동하려는 위차가 같다면 된 것이다.
		return TRUE;
	}
*/

	if( lpObj->Teleport ) {
		// 텔레포트 중이라면 무효
		return TRUE;
	}

	// 가려는 곳의 땅 속성을 체크하여 미리 막음
	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(iX, iY);
	if( ((btMapAttr&MAP_ATTR_STAND)==MAP_ATTR_STAND)				// 다른 누군가 서있음
		|| ((btMapAttr&MAP_ATTR_BLOCK)==MAP_ATTR_BLOCK)				// 벽
//		|| ((btMapAttr&MAP_ATTR_HOLLOW)==MAP_ATTR_HOLLOW)			// 땅뚫림
		)
	{
		return FALSE;
	}

	PMSG_POSISTION_SET	pMove;
	pMove.h.c		 = PMHC_BYTE;

#ifdef PACKET_CHANGE	
	pMove.h.headcode = PACKET_POSITION;
#else
	pMove.h.headcode = 0x11;
#endif

	pMove.h.size     = sizeof( pMove );
	pMove.X          = (BYTE)iX;
	pMove.Y			 = (BYTE)iY;
	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(iX, iY);
	lpObj->m_OldX = iX;
	lpObj->m_OldY = iY;


	//----------------------------------------------------------------------------
	// 아래는 protocol.cpp : RecvPositionSetProc() 와 거의 같음

	// 시작 위치 세팅
	lpObj->X			= pMove.X;
	lpObj->Y			= pMove.Y;	
	
	PMSG_RECV_POSISTION_SET pMove2;

#ifdef PACKET_CHANGE	
	PHeadSetB((LPBYTE)&pMove2, PACKET_POSITION, sizeof( pMove2));
#else
	PHeadSetB((LPBYTE)&pMove2, 0x11, sizeof( pMove2));
#endif

	pMove2.NumberH		= HIBYTE(iIndex);
	pMove2.NumberL		= LOBYTE(iIndex);
	pMove2.X			= (BYTE)pMove.X;
	pMove2.Y			= (BYTE)pMove.Y;
		
	lpObj->TX			= (BYTE)pMove.X;
	lpObj->TY			= (BYTE)pMove.Y;
/*
	if( gObjPositionCheck(lpObj) == FALSE ) 
	{	
		// 너무 많이 이동한 것이 아닌지 체크
		return FALSE;
	}
*/
	CreateFrustrum(lpObj->X, lpObj->Y, iIndex);

	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(iIndex, (LPBYTE)&pMove2, pMove2.h.size);

	// 나를 보고 있는 사람들에게 모두 보내준다.
	MsgSendV2 (&gObj[iIndex], (LPBYTE)&pMove2, pMove2.h.size);

	// 카오스 캐슬안에 있는 모든 사용자들에게 보내준다.
/*	for (INT i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[lpObj->MapNumber - MAP_INDEX_CHAOSCASTLE1].m_UserData[i].m_iIndex != -1) {
			if ((gObjIsConnected (m_stChaosCastleData[lpObj->MapNumber - MAP_INDEX_CHAOSCASTLE1].m_UserData[i].m_iIndex) == TRUE) &&
				(CHECK_CHAOSCASTLE(gObj[m_stChaosCastleData[lpObj->MapNumber - MAP_INDEX_CHAOSCASTLE1].m_UserData[i].m_iIndex].MapNumber))
				)
			{
				DataSend(m_stChaosCastleData[lpObj->MapNumber - MAP_INDEX_CHAOSCASTLE1].m_UserData[i].m_iIndex, (LPBYTE)&pMove2, pMove2.h.size);
			}
		}	
	}
*/


/*
	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	// 나를 보고 있는 유저들에게 나의 행동을 보여준다.
	for( INT n=0; n<MVL; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_CHARACTER)
		{
			if( lpObj->VpPlayer2[n].state == STVP_CREATE )
			{
				int number = lpObj->VpPlayer2[n].number;
				if( (gObj[number].Connected > 1) && (gObj[number].Live) )	// 상대편이 접속해 있다면..
					DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pMove2, pMove2.h.size);
				else 
				{
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VpPlayer2[n].state  = STVP_NONE;
					lpObj->VPCount2--;
				}
				
			}
		}
	}
*/

	//---
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);
	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

	return TRUE;
}


VOID CChaosCastle::SearchNBlowObjs		(										// 특정 포인트 주변 5x5 크기를 검색하여 주변의 오브젝트 (사용자, 몬스터) 들을 날려버림
			INT iMapNumber,
			INT iX, 
			INT iY
			)
{
	if (!CHECK_CHAOSCASTLE(iMapNumber)) {
		return;
	}

	if (!CHECK_LIMIT(iX, 256) || !CHECK_LIMIT(iY, 256)) {
		return;
	}

	INT iMIN_X = iX-MAX_BLOWOUT_RANGE;
	INT iMAX_X = iX+MAX_BLOWOUT_RANGE;
	INT iMIN_Y = iY-MAX_BLOWOUT_RANGE;
	INT iMAX_Y = iY+MAX_BLOWOUT_RANGE;

	if (iMIN_X < 0)		iMIN_X = 0;
	if (iMIN_Y < 0)		iMIN_Y = 0;
	if (iMIN_X > 255)	iMIN_X = 255;
	if (iMIN_Y > 255)	iMIN_Y = 255;


	// 정점 주변 5x5의 사용자들이 있는지 검사하여 밀쳐낸다.
	for (INT i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 번호 구하기
		INT iChaosCastleIndex = GetChaosCastleByMapNum( iMapNumber );
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex == -1)
			continue;
#else
		if (m_stChaosCastleData[iMapNumber - MAP_INDEX_CHAOSCASTLE1].m_UserData[i].m_iIndex == -1)
			continue;
#endif

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 번호 구하기
		INT n = m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex;
#else
		INT n = m_stChaosCastleData[iMapNumber - MAP_INDEX_CHAOSCASTLE1].m_UserData[i].m_iIndex;
#endif

		if( (gObj[n].MapNumber == iMapNumber) && (gObj[n].Connected > 2) )
		{
			if ( (gObj[n].X >= iMIN_X && gObj[n].X <= iMAX_X) && (gObj[n].Y >= iMIN_Y && gObj[n].Y <= iMAX_Y)) {
				BlowObjsFromPoint (gObj[n].m_Index, iMapNumber, iX, iY);

				// 해당 위치가 죽는 자리인지 체크하여 땅뚫린 자리라면 사망하도록 등록한다.
				BYTE btAttr = (MapC[iMapNumber].m_attrbuf[(iY*MAX_TERRAIN_SIZE)+iX]) & MAP_ATTR_HOLLOW;

				if ( btAttr == MAP_ATTR_HOLLOW) {
					// 땅뚫림 속성안에 있다. -> 이 사람들은 무조건 죽는다.
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 번호 구하기
					AddFallUser (iChaosCastleIndex, n);
#else
					AddFallUser (iMapNumber - MAP_INDEX_CHAOSCASTLE1, n);
#endif

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 번호 구하기
					LogAddTD("[Chaos Castle] (%d)(Event:%d) [%s][%s] User Dead In Chaos Castle : Fall from Castle (X:%d, Y:%d)", 
						iChaosCastleIndex + 1,
						m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
						gObj[n].AccountID, 
						gObj[n].Name,
						iX,
						iY
						);
#else
					LogAddTD("[Chaos Castle] (%d)(Event:%d) [%s][%s] User Dead In Chaos Castle : Fall from Castle (X:%d, Y:%d)", 
						iMapNumber - MAP_INDEX_CHAOSCASTLE1 + 1,
						m_stChaosCastleData[iMapNumber - MAP_INDEX_CHAOSCASTLE1].m_btCC_EVENT,
						gObj[n].AccountID, 
						gObj[n].Name,
						iX,
						iY
						);
#endif	// ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010					
#else
					LogAddTD("[Chaos Castle] (%d) [%s][%s] User Dead In Chaos Castle : Fall from Castle (X:%d, Y:%d)", 
						iMapNumber - MAP_INDEX_CHAOSCASTLE1 + 1,
						gObj[n].AccountID, 
						gObj[n].Name,
						iX,
						iY
						);
#endif
				}
			}
		}
	}
}


BOOL CChaosCastle::BlowObjsFromPoint	(										// 특정 오브젝트 (사용자, 몬스터)들을 특정 위치를 중심으로 외곽으로 날려버림
			INT iIndex, 
			INT iMapNumber,
			INT & iX, 
			INT & iY
			)
{
	if (!gObjIsConnected(iIndex)) {
		// 사용자가 접속상태가 아니라면 에러
		return FALSE;
	}

	if (!CHECK_LIMIT(iX, 256) || !CHECK_LIMIT(iY, 256)) {
		return FALSE;
	}

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( lpObj->DieRegen ) {
		// 죽어가는 중이면 밀쳐내지 않는다.
		return FALSE;
	}

	if( lpObj->Teleport ) {
		// 텔레포트 중이면 밀쳐내지 않는다.
		return FALSE;
	}

	if (lpObj->MapNumber != iMapNumber) {
		// 밀쳐내려는 사용자와의 맵이 다르다면 밀쳐내지 않는다.
		return FALSE;
	}


	// 정점과 오브젝트간의 거리 구함 (범위 (0~5) 밖이면 그냥 나감)
	INT iOBJ_DIST = CalDistance (lpObj->X, lpObj->Y, iX, iY);
	if (!CHECK_LIMIT(iOBJ_DIST, MAX_BLOWOUT_DISTANCE)) {
		return FALSE;
	}
	

	// 위치에 따른 X, Y 부호 (+. -) 설정 -> 밀쳐내는 방향 (일단 방향은 고정)
	INT iSIGN_X = 1;	// X축 +, -
	INT iSIGN_Y = 1;	// Y축 +, -

	INT iUX = lpObj->X;
	INT iUY = lpObj->Y;

	if (iUX > iX) {
		iSIGN_X = 1;
	}
	else if (iUX < iX) {
		iSIGN_X = -1;
	}
	else {
		INT iRND = rand()%2;
		(!iRND) ? iSIGN_X=1 : iSIGN_X=-1;
	}
	
	if (iUY > iY) {
		iSIGN_Y = 1;
	}
	else if (iUY < iY) {
		iSIGN_Y = -1;
	}
	else {
		INT iRND = rand()%2;
		(!iRND) ? iSIGN_Y=1 : iSIGN_Y=-1;
	}


	BOOL bSuccessBlowOut = FALSE;
	for (INT iBLOWOUT_COUNT = 0 ; iBLOWOUT_COUNT < MAX_SEARCH_SETPOINT ; iBLOWOUT_COUNT++) {
		// 밀쳐낼  X, Y의 크기를 계산 (위치는 밀쳐내기 실패 시 재계산)
		INT iBLOW_MIN = g_iChaosCastle_BlowOutDistance[iOBJ_DIST][0];		// 밀쳐낼 최소거리
		INT iBLOW_MAX = g_iChaosCastle_BlowOutDistance[iOBJ_DIST][1];		// 밀쳐낼 최대거리
		
		INT iBLOW_X = rand() % (iBLOW_MAX - iBLOW_MIN + 1) + iBLOW_MIN;
		INT iBLOW_Y = rand() % (iBLOW_MAX - iBLOW_MIN + 1) + iBLOW_MIN;

		INT iRND = rand()%2;
		if (iRND) {
			// X축 중심일 때 Y축 값을 변경 -> 최대거리 (iBLOW_MAX) 에 가급적 맞도록
			if ( iBLOW_X >= iBLOW_MAX) {
				iBLOW_X = iBLOW_MAX;
				iBLOW_Y = iBLOW_MIN + (rand()%2 - 1);
				if (iBLOW_Y < 0)	iBLOW_Y = 0;
			}
		}
		else {
			// Y축 중심일 때 X축 값을 변경 -> 최대거리 (iBLOW_MAX) 에 가급적 맞도록
			if ( iBLOW_Y >= iBLOW_MAX) {
				iBLOW_Y = iBLOW_MAX;
				iBLOW_X = iBLOW_MIN + (rand()%2 - 1);
				if (iBLOW_X < 0)	iBLOW_X = 0;
			}
		}

		INT iTX = lpObj->X + iBLOW_X * iSIGN_X;
		INT iTY = lpObj->Y + iBLOW_Y * iSIGN_Y;

		if (iTX < 0)	iTX = 0;
		if (iTY < 0)	iTY = 0;
		if (iTX > 255)	iTX = 255;
		if (iTY > 255)	iTY = 255;
		
		if (bSuccessBlowOut = ObjSetPosition (iIndex, iTX, iTY)) {
			// 성공했다면 마지막 밀어내기 한 시간을 기록한다.
			lpObj->m_iChaosCastleBlowTime = GetTickCount();
			// 성공했다면 루프를 빠져나간다.
			iX = iTX;
			iY = iTY;
			// 그전에 데미지를 준다.
			GiveUserDamage (lpObj->m_Index, g_iChaosCastle_BlowOutDamage[iOBJ_DIST%MAX_BLOWOUT_DISTANCE]);
			break;
		}
	}		

	return TRUE;
}


BOOL CChaosCastle::BlowObjsToPoint	(											// 특정 오브젝트 (사용자, 몬스터)들을 특정 위치로 날려버림
			INT iIndex, 
			INT iMapNumber,
			INT iX, 
			INT iY
			)
{

	return TRUE;
}


// 거리 계산..
INT	CChaosCastle::CalDistance			(INT iX1, INT iY1, INT iX2, INT iY2)	// 두 좌표간의 거리를 계산
{
	if( (iX1 == iX2) &&	(iY1 == iY2) )
	{
		return 0;
	}

	float fTX = (float)(iX1-iX2);
	float fTY = (float)(iY1-iY2);
			
	return (int)(sqrt((fTX*fTX)+(fTY*fTY)));
}


VOID CChaosCastle::SafetyCastleZone		(INT iChaosCastleIndex)					// 특정 캐슬의 영역을 안전지대 속성으로 채운다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
	INT iMapNum = GetMapNumByChaosCastle( iChaosCastleIndex );
	for (INT i = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][0] ; i <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][2] ; i++) {
		for (INT j = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][1] ; j <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][3] ; j++) {
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_SAFTYZONE;
		}
	}

	for (INT iHOLE_NUM = 0 ; iHOLE_NUM < 2 ; iHOLE_NUM++) {						// 가운데 땅 뚫린 두 지점을 다시 땅뚫림으로 채운다.
		for (INT i = g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][0] ; i <= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][2] ; i++) {
			for (INT j = g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][1] ; j <= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][3] ; j++) {
				MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
			}
		}
	}
#else

	for (INT i = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][0] ; i <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][2] ; i++) {
		for (INT j = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][1] ; j <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][3] ; j++) {
			MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_SAFTYZONE;
		}
	}

	for (INT iHOLE_NUM = 0 ; iHOLE_NUM < 2 ; iHOLE_NUM++) {						// 가운데 땅 뚫린 두 지점을 다시 땅뚫림으로 채운다.
		for (INT i = g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][0] ; i <= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][2] ; i++) {
			for (INT j = g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][1] ; j <= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][3] ; j++) {
				MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
			}
		}
	}
#endif
}


VOID CChaosCastle::UnSafetyCastleZone	(INT iChaosCastleIndex)					// 특정 캐슬의 영역의 안전지대 속성을 푼다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
	INT iMapNum = GetMapNumByChaosCastle( iChaosCastleIndex );
	for (INT i = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][0] ; i <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][2] ; i++) {
		for (INT j = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][1] ; j <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][3] ; j++) {
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_SAFTYZONE);
		}
	}

	for (INT iHOLE_NUM = 0 ; iHOLE_NUM < 2 ; iHOLE_NUM++) {						// 가운데 땅 뚫린 두 지점을 다시 땅뚫림으로 채운다.
		for (INT i = g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][0] ; i <= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][2] ; i++) {
			for (INT j = g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][1] ; j <= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][3] ; j++) {
				MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
			}
		}
	}

#else
	for (INT i = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][0] ; i <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][2] ; i++) {
		for (INT j = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][1] ; j <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][3] ; j++) {
			MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_SAFTYZONE);
		}
	}

	for (INT iHOLE_NUM = 0 ; iHOLE_NUM < 2 ; iHOLE_NUM++) {						// 가운데 땅 뚫린 두 지점을 다시 땅뚫림으로 채운다.
		for (INT i = g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][0] ; i <= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][2] ; i++) {
			for (INT j = g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][1] ; j <= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][3] ; j++) {
				MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
			}
		}
	}
#endif	// ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
}


VOID CChaosCastle::SendCastleZoneSafetyInfo (									// 특정 캐슬의 모든 사용자에게 특정 캐슬 입구의 현재 안전지대 정보를 보낸다.
			INT iChaosCastleIndex,
			bool bDoSet
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	char cTEMP_BUF [256];
	LPPMSG_SETMAPATTR_COUNT lpMsg = (LPPMSG_SETMAPATTR_COUNT) cTEMP_BUF;
	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT) + sizeof(PMSG_SETMAPATTR) * 2 * 3);

	LPPMSG_SETMAPATTR lpMsgBody = (LPPMSG_SETMAPATTR) (cTEMP_BUF + sizeof(PMSG_SETMAPATTR_COUNT));

	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 0x01;
	(bDoSet) ? lpMsg->btMapSetType = 0 : lpMsg->btMapSetType = 1;

	lpMsgBody[0].btX = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][0];
	lpMsgBody[0].btY = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][1];
	lpMsgBody[1].btX = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][2];
	lpMsgBody[1].btY = g_btCastleSafetyZoneMapXY[iChaosCastleIndex][3];

	for (int i = ALLOC_USEROBJECTSTART ; i < MAX_OBJECT ; i++) {
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
		if ((gObj[i].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) ) && (gObj[i].Connected > 2)) {
#else
		if ((gObj[i].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex) && (gObj[i].Connected > 2)) {
#endif
			DataSend (i, (unsigned char*) lpMsg, lpMsg->h.size);
		}
	}
}


VOID CChaosCastle::CheckMonsterInDieTile	(INT iChaosCastleIndex)				// 사용자가 땅뚫림 속성위에 서있는지 체크하여 있으면 죽었다고 보내줌
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;


	// *> . 현재 시점에서 맵의 트랩 안쪽의 각 사분면의 빈자리를 찾아서 저장한다. (나중에 사람을 밀어넣을 때 속도를 빨리하기 위함)
	INT iUSER_AXIS = 0;						// 사용자가 좌표계의 몇 사분면에 있는지 저장
	vector <POINT> vtMAP_UNTRAP[4];			// 맵에 비있고 땅뚫림이 없는 지점을 저장

	// *> . 여기서 빈자리를 찾아서 저장한다.
	for (INT iAXIS = 0 ; iAXIS < 4 ; iAXIS++) {
		for (INT iMAPX = g_rtPOINT_TRAP[iAXIS].left ; iMAPX <= g_rtPOINT_TRAP[iAXIS].right ; iMAPX++) {
			for (INT iMAPY = g_rtPOINT_TRAP[iAXIS].top ; iMAPY <= g_rtPOINT_TRAP[iAXIS].bottom ; iMAPY++) {
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
				INT iMapNum		= GetMapNumByChaosCastle( iChaosCastleIndex );
				BYTE btMapAttr	= MapC[iMapNum].GetAttr(iMAPX, iMAPY);
#else
				BYTE btMapAttr = MapC[MAP_INDEX_CHAOSCASTLE1+iChaosCastleIndex].GetAttr(iMAPX, iMAPY);
#endif
				if( ((btMapAttr&MAP_ATTR_STAND)!=MAP_ATTR_STAND)				// 다른 누군가 서있음
					&&((btMapAttr&MAP_ATTR_BLOCK)!=MAP_ATTR_BLOCK)				// 벽
					&&((btMapAttr&MAP_ATTR_HOLLOW)!=MAP_ATTR_HOLLOW)			// 땅뚫림
					)
				{
					POINT iPT;
					iPT.x = iMAPX, iPT.y = iMAPY;
					vtMAP_UNTRAP[iAXIS].push_back(iPT);
				}
			}
		}
	}


	// 2 . 몬스터 처리 -> 땅뚫린 속성 위에 있으면 안쪽으로 튕겨낸다.

	for (INT iMON = 0 ; iMON < MAX_CHAOSCASTLE_MONSTER_COUNT ; iMON++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == -1)
			continue;
		INT iMonsterIndex = m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON];
		if (!CHECK_LIMIT(iMonsterIndex, MAX_MONSTER)) {
			m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] = -1;
			continue;
		}

		if ((gObj[iMonsterIndex].Life > 0) && CHECK_CHAOSCASTLE(gObj[iMonsterIndex].MapNumber) && gObj[iMonsterIndex].Connected > 0) {
			// 이 몬스터들은 아직 살아있다.
			INT iSX = gObj[iMonsterIndex].X;
			INT iSY = gObj[iMonsterIndex].Y;

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
			INT iMapNum	= GetMapNumByChaosCastle( iChaosCastleIndex );			
			BYTE btAttr = (MapC[iMapNum].m_attrbuf[(iSY*MAX_TERRAIN_SIZE)+iSX]) & MAP_ATTR_HOLLOW;
#else
			BYTE btAttr = (MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(iSY*MAX_TERRAIN_SIZE)+iSX]) & MAP_ATTR_HOLLOW;
#endif

			if ( btAttr == MAP_ATTR_HOLLOW) {
				// 땅뚫림 속성안에 있는 사용자는 안쪽으로 끌어들인다.

				// 현재 데미지 범위 안에 있다. 안으로 튕겨야 하므로 이 사람이 몇사분면 안에 있는지 확인한다.
				INT z;
				for (z = 0 ; z < 4 ; z++) {
					if ((iSX >= g_rtPOINT_FRAME[z].left) && (iSX <= g_rtPOINT_FRAME[z].right) && (iSY >= g_rtPOINT_FRAME[z].top) && (iSY <= g_rtPOINT_FRAME[z].bottom)) {
						iUSER_AXIS = z;
						break;
					}
				}

				if (z == 4)			// 사용자가 어디에도 속하지 않았을 때는 (버그) 그냥 넘어간다.
					continue;		

				// 여기서 사분면의 빈 곳을 찾아서 튕겨준다.
				if (!vtMAP_UNTRAP[iUSER_AXIS].empty()) {
					POINT ptEMPTY = vtMAP_UNTRAP[iUSER_AXIS].back();
					vtMAP_UNTRAP[iUSER_AXIS].pop_back();

					if (ObjSetPosition (iMonsterIndex, ptEMPTY.x, ptEMPTY.y) == TRUE) {
						// 성공했다면 마지막 밀어내기 한 시간을 기록한다.
						gObj[iMonsterIndex].m_iChaosCastleBlowTime = GetTickCount();
					}
				}
			}			
		}
	}
	
}


VOID CChaosCastle::CheckUserInDieTile	(INT iChaosCastleIndex)				// 사용자가 땅뚫림 속성위에 서있는지 체크하여 있으면 죽었다고 보내줌
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;


	// 1 . 사용자 처리 -> 땅뚫린 속성 위에 있으면 죽인다.
	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			if ((gObjIsConnected (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE) &&
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기			
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) )
#else
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex)
#endif
				)
			{
				if(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Life > 0.0) {
					INT iSX = gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].X;
					INT iSY = gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Y;

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
					INT iMapNum	= GetMapNumByChaosCastle( iChaosCastleIndex );	
					BYTE btAttr = (MapC[iMapNum].m_attrbuf[(iSY*MAX_TERRAIN_SIZE)+iSX]) & MAP_ATTR_HOLLOW;
#else
					BYTE btAttr = (MapC[MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex].m_attrbuf[(iSY*MAX_TERRAIN_SIZE)+iSX]) & MAP_ATTR_HOLLOW;
#endif

					if ( btAttr == MAP_ATTR_HOLLOW) {
						// 땅뚫림 속성안에 있는 사용자는 죽인다.
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Life		= 0.0;
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_State		= OBJST_DYING;
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].KillerType	= KT_MONSTER;
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].RegenTime	= GetTickCount();
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].DieRegen	= 1;
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].PathCount	= 0;

#ifdef ADD_SKILL_WITH_COMBO
						GCDiePlayerSend(&gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex], m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, 0, 0);
#else
						GCDiePlayerSend(&gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex], m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex);
#endif
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
						LogAddTD("[Chaos Castle] (%d)(Event:%d) [%s][%s] User Dead In Chaos Castle : Fall from Castle (X:%d, Y:%d)", 
							iChaosCastleIndex+1,
							m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
							gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].AccountID, 
							gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Name,
							iSX,
							iSY
							);
#else
						LogAddTD("[Chaos Castle] (%d) [%s][%s] User Dead In Chaos Castle : Fall from Castle (X:%d, Y:%d)", 
							iChaosCastleIndex+1,
							gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].AccountID, 
							gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Name,
							iSX,
							iSY
							);
#endif

						// 자신이 얻은 경험치 점수등을 보내준다.
						SendFailMessage (iChaosCastleIndex, m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex);
					}
				}
			}
		}	
	}
}


VOID CChaosCastle::PlayFailedRollBack	(INT iChaosCastleIndex)				// 사용자 수 미달 등으로 경기시작이 실패 시 남은 사람들의 정보를 원상태로 돌린다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;
	
	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			if ((gObjIsConnected (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE) &&
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) )
#else
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex)
#endif
				)
			{
#ifdef MODIFY_PCBANG_EVENT_FREE_CHAOSCASTLE_20060331
				INT iFIRST_MONEY	= 0;
				INT iPAYBACK_MONEY	= 0;

				iFIRST_MONEY		= gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money;		// 해당 유저가 소유한 젠
				iPAYBACK_MONEY		= g_iChaosCastle_GuardSetValue[iChaosCastleIndex];									// 근위병의 갑옷 가격
				if( m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_btEvent != CC_EVENT_PCBANG_FREE_ENTER )	// PC방 사용자는 입장료를 환불하지 않는다.
				{
					// 일반 사용자는 입장료를 환불해준다.					
					iPAYBACK_MONEY	+= g_iChaosCastle_EnterCost[iChaosCastleIndex];
				}
				gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money += iPAYBACK_MONEY;
#else
				// 이 안에 들어와 있는 사람들은 입장료와 근위병의 갑옷세트 가격을 환불해준다.
				INT iFIRST_MONEY	= gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money;
				INT iPAYBACK_MONEY	= g_iChaosCastle_EnterCost[iChaosCastleIndex] + g_iChaosCastle_GuardSetValue[iChaosCastleIndex];

				gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money += g_iChaosCastle_EnterCost[iChaosCastleIndex] + g_iChaosCastle_GuardSetValue[iChaosCastleIndex];
#endif			
				
				INT iNOW_MONEY		= gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money;

				GCMoneySend(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money);
				// 환불해준 사람들을 데비아스로 이동시킨다.
				gObjMoveGate(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, 22);


				// 이벤트 실패 메시지를 보내준다.
				PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 1, lMsg.Get(1236));		// "제 %d 카오스 캐슬 퀘스트가 시작되었습니다."
#else
				pNotice.type = 1;
				wsprintf(pNotice.Notice, lMsg.Get(1236));					// "제 %d 카오스 캐슬 퀘스트가 시작되었습니다."
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
				DataSend(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);

#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 1, (char*)m_sMinPlayerMessage.c_str());		// "제 %d 카오스 캐슬 퀘스트가 시작되었습니다."
#else
				pNotice.type = 1;
				wsprintf(pNotice.Notice, (char*)m_sMinPlayerMessage.c_str());					// "제 %d 카오스 캐슬 퀘스트가 시작되었습니다."
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
				DataSend(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
				LogAddTD("[Chaos Castle] (%d)(Event:%d) [%s][%s] Payback to User ChaosCastle Money (FIRST:%d, PAYBACK:%d, FINAL:%d)", 
					iChaosCastleIndex+1,
					m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
					gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].AccountID, 
					gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Name,
					iFIRST_MONEY,
					iPAYBACK_MONEY,
					iNOW_MONEY
					);
#else
				LogAddTD("[Chaos Castle] (%d) [%s][%s] Payback to User ChaosCastle Money (FIRST:%d, PAYBACK:%d, FINAL:%d)", 
					iChaosCastleIndex + 1,					
					gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].AccountID, 
					gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Name,
					iFIRST_MONEY,
					iPAYBACK_MONEY,
					iNOW_MONEY
					);
#endif
			}
		}	
	}

}


VOID CChaosCastle::RewardUserEXP		(				// 현재 입장한 사용자의 수를 다시 계산 (접속해서 캐슬안에 있는 사람 기준)
			INT iChaosCastleIndex,
			INT iChaosCastleSubIndex,
			BOOL bWinner
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if (!CHECK_LIMIT(iChaosCastleSubIndex, MAX_CHAOSCASTLE_USER_COUNT))
		return;


	if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex != -1) {
		if (gObjIsConnected (m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex) == TRUE) {
			// 이 사용자가 재접한 사람인제 체크한다. (인덱스만 같이 쓸 수 있으므로)
			if ((gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cChaosCastleIndex == -1) ||
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cChaosCastleSubIndex == -1)
				)
			{
				// 이 사람은 재접한 사람이므로 무효이다.
				return;
			}

			// 여기서 사용자들에게 포상을 한다.
			INT iKILLCOUNT_USER			= gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cKillUserCount;
			INT iKILLCOUNT_MONSTER		= gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cKillMonsterCount;
			INT iTOT_EXP				= iKILLCOUNT_USER * g_iChaosCastle_ExpTable[iChaosCastleIndex][CC_EXP_USER_KILL] + iKILLCOUNT_MONSTER * g_iChaosCastle_ExpTable[iChaosCastleIndex][CC_EXP_MONSTER_KILL];
//#ifdef ADD_PCS_MARK_OF_EXP_20070205
#if !defined(MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004) && defined(ADD_PCS_MARK_OF_EXP_20070205)
	#ifdef MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
			if( gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_wExprienceRate == 0 )
			{
				iTOT_EXP = 0;
			}
	#else
			if( gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_wExprienceRate > 0 )
			{
				iTOT_EXP = (int)(iTOT_EXP * ((float)gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_wExprienceRate / 100 ));
			}
			else if( gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_wExprienceRate <= 0 )
			{
				iTOT_EXP = 0;
			}
	#endif // MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
#endif // ADD_PCS_MARK_OF_EXP_20070205
			INT iREWARD_EXP				= CalcSendRewardEXP(m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex, iTOT_EXP, iKILLCOUNT_USER, iKILLCOUNT_MONSTER);

			// 획득한 경험치와 젠을 알려준다.
			PMSG_DEVILSQUARERESULT pMsg;
			pMsg.MyRank = bWinner;		// 1:성공 / 0:실패
			pMsg.Count = 254;			// 카오스 캐슬임을 나타냄
			memcpy(pMsg.Score[0].Name, gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].Name, sizeof(pMsg.Score[0].Name));
		#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			pMsg.Score[0].BonusExp		= iREWARD_EXP;			// 경험치 비율이 적용된 포상 경험치
		#else
			pMsg.Score[0].BonusExp		= iTOT_EXP;				// 포상 경험치
		#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			pMsg.Score[0].BonusZen		= iKILLCOUNT_USER;		// 죽인 사용자 수
			pMsg.Score[0].TotalScore	= iKILLCOUNT_MONSTER;	// 죽인 몬스터 수
			PHeadSetB((LPBYTE)&pMsg, 0x93, sizeof(PBMSG_HEAD) + 2 + sizeof(DevilSquareScoreInfo) * 2);

			DataSend(m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
			LogAddTD("[Chaos Castle] (%d)(Event:%d) [%s][%s] Reward User EXP (USER_KILL:%d, MON_KILL:%d, TOT_EXP:%d, TOT_REWARD_EXP:%d)",
				iChaosCastleIndex + 1,
				m_stChaosCastleData[iChaosCastleIndex].m_btCC_EVENT,
				gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].AccountID, 
				gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].Name,
				iKILLCOUNT_USER,
				iKILLCOUNT_MONSTER,
				iTOT_EXP,
				iREWARD_EXP
				);
#else
			LogAddTD("[Chaos Castle] (%d) [%s][%s] Reward User EXP (USER_KILL:%d, MON_KILL:%d, TOT_EXP:%d, TOT_REWARD_EXP:%d)",
				iChaosCastleIndex+1,
				gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].AccountID, 
				gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].Name,
				iKILLCOUNT_USER,
				iKILLCOUNT_MONSTER,
				iTOT_EXP,
				iREWARD_EXP
				);
#endif
		}
	}

}


VOID CChaosCastle::ProcessTrapStatus	(INT iChaosCastleIndex)				// 특정 캐슬의 트랩 상태를 처리한다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	INT iCUR_LIVES = GetCurPlayUser(iChaosCastleIndex) + GetMonsterListCount(iChaosCastleIndex);

	if (iCUR_LIVES > 40) {
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS = CC_TRAP_0;
	}
	else if (iCUR_LIVES > 30) {
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS = CC_TRAP_1;
		SendNoticeState(iChaosCastleIndex, CC_STATE_TRAP_1);
	}
	else if (iCUR_LIVES > 20) {
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS = CC_TRAP_2;
		SendNoticeState(iChaosCastleIndex, CC_STATE_TRAP_2);
	}
	else {
		m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS = CC_TRAP_3;
		SendNoticeState(iChaosCastleIndex, CC_STATE_TRAP_3);
	}
}


VOID CChaosCastle::CalUsersInTrap		(INT iChaosCastleIndex)				// 특정 캐슬의 사용자들이 트랩 범위 안에 있는지 파악하여 있으면 사용자에게 일정량의 데미지를 준다.
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;
	
	if (m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS == CC_TRAP_0)		// 트랩 0 단계에서는 패널티가 없다.
		return;

	if (!CHECK_LIMIT(m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS, CC_MAX_DAMAGE_STEP))
		return;

	INT iTRAP_STEP = m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS;

	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			if ((gObjIsConnected (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE) &&
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) )
#else
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex)
#endif
				)
			{
				// 여기 실제 접속해 있는 사용자들을 대상으로 데미지 범위 안에 있는지를 확인한다.
				INT iUX = gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].X;
				INT iUY = gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Y;

				if (((iUX >= g_iChaosCastle_GroundAxis[0]) && (iUX <= g_iChaosCastle_GroundAxis[2]) && (iUY >= g_iChaosCastle_GroundAxis[1]) && (iUY <= g_iChaosCastle_GroundAxis[3])) &&
					!((iUX >= g_iChaosCastle_DamageAxis[iTRAP_STEP][0]) && (iUX <= g_iChaosCastle_DamageAxis[iTRAP_STEP][2]) && (iUY >= g_iChaosCastle_DamageAxis[iTRAP_STEP][1]) && (iUY <= g_iChaosCastle_DamageAxis[iTRAP_STEP][3]))
					)
				{
					// 이 조건이 만족하면 데미지 범위 안에 있는 것이다.
					GiveUserDamage(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, CC_TRAP_DAMAGE);

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					LPOBJECTSTRUCT lpObj = &gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex];

					if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_FREEZE ) == false )
					{
						lpObj->DelayActionTime = 800;
						lpObj->DelayLevel = 1;
						lpObj->lpAttackObj = NULL;
						gObjAddBuffEffect( lpObj, BUFFTYPE_FREEZE, 0, 0, 0, 0, 3 );
					}
#else
					if (gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_ColdBeattackCount <= 0) {
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_ColdBeattackCount		= 3;
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].lpAttackObj				= NULL;
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].DelayActionTime			= 800;
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].DelayLevel				= 1;
						gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_ViewSkillState		|= 0x02;	//	2번째 비트

						GCStateInfoSend(&gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex], 1, gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_ViewSkillState);
					}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				}
			}
		}
	}
}


BOOL CChaosCastle::CheckWearingMOPH	(INT iUserIndex)						// 해당 사용자가 변신반지를 착용하고 있는지 확인한다.
{
	if (!gObjIsConnected(iUserIndex)) {
		// 사용자가 접속상태가 아니라면 에러
		return FALSE;
	}

	if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].IsItem())
	{							
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) )
		{
			return TRUE;
		}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// 카캐 입장 금지
		// 엘리트해골전사변신반지(이벤트용)
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{			
			return TRUE;
		}
#endif
		
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		// 잭 오 랜턴 변신반지
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			return TRUE;
		}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		// 산타크로스 변신반지
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 ) ) // 임시
		{
			return TRUE;
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
//------------->
//20081013 해외 크리스마스 이벤트 - 변신반지(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			return TRUE;
		}
#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
//<------------
// 팬더변신반지
#ifdef ADD_PREMIUMITEM_PANDA
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
		{
			return TRUE;
		}
#endif // ADD_PREMIUMITEM_PANDA
	}	

	if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].IsItem())
	{							
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) )
		{
			return TRUE;
		}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// 카캐 입장 금지
		// 엘리트해골전사변신반지(이벤트용)
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{			
			return TRUE;
		}
#endif
		
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			return TRUE;
		}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			return TRUE;
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

//------------->
//20081013 해외 크리스마스 이벤트 - 변신반지(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			return TRUE;
		}
#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
//<------------
#ifdef ADD_PREMIUMITEM_PANDA
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
		{
			return TRUE;
		}
#endif // ADD_PREMIUMITEM_PANDA
	}	

	return FALSE;
}


VOID CChaosCastle::GiveWinnerItem		(									// 카오스 캐슬 우승자에게 아이템을 준다.
				INT iChaosCastleIndex,
				INT iWinnerIndex
				)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;

	if (!gObjIsConnected(iWinnerIndex))
		return;

	//g_LogToFile.Output("[CC_LOG] GIVE WINNER ITEM (%d) : ENTER", iChaosCastleIndex+1 );
#ifdef OLD_CHAOSCASTLE_ITEM_SYSTEM
	INT iItemDropRate	= rand()%100;
	INT iItemType		= 0;
	INT iItemLevel		= 0;
	INT iItemDur		= 0;

//	if (iItemDropRate < 35) {						// 2004.08.30 - 혼석 상품에서 제외됨
//		iItemType = MAKE_ITEMNUM(12,15);			// 혼돈의 보석
//		iItemLevel = 0;
//		iItemDur = 0;
//
//		LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Chaos",
//			iChaosCastleIndex+1,
//			gObj[iWinnerIndex].AccountID, 
//			gObj[iWinnerIndex].Name
//			);
//	}
//	else if (iItemDropRate < 65) {

#ifdef ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113
	int nDropRate1, nDropRate2;
	int nCastleGrade = 0;
#ifndef MODIFY_CHAOSCASTLE_REWARD_20090608
	#ifdef SEASON4_PATCH_GROOVING
	// grooving 시즌4 20080624
	// 드롭 확률 조정 창석(50->40%), 생석(45->55%), 세트 아이템(5->5%)
	// 1캐슬: 축석, 영석, 아이템중 랜덤
	// 2~7캐슬: 생석, 창석, 셋템
	nDropRate1 = 40;
	nDropRate2 = 55 + nDropRate1;
	#else
	nDropRate1 = 50;
	nDropRate2 = 45 + nDropRate2;
	#endif
#else 
	nDropRate1 = 50;
	nDropRate2 = 45 + nDropRate2;
#endif //MODIFY_CHAOSCASTLE_REWARD_20090608

	if (g_bNewServer == TRUE)
	{
		// 신섭 드롭확률 조정 창석(40%->40%), 생석(55%->57%), 세트 아이템(5->3%)
		// 1~2캐슬: 축석, 영석, 아이템중 랜덤
		// 3~7캐슬: 생석, 창석, 셋템
		nDropRate1 = 40;
		nDropRate2 = 57 + nDropRate1;
	}


	if (iItemDropRate < nDropRate1) 
	{
		switch (iChaosCastleIndex) {
		case 0 :
			{
				iItemType = MAKE_ITEMNUM(14,14);	// 영혼의 보석

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Soul",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬7 보상
		case 6 :
#endif			
			{
				iItemType = MAKE_ITEMNUM(14,22);	// 창조의 보석

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Creative",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		}
		iItemLevel = 0;
		iItemDur = 0;
	}
	else if (iItemDropRate < nDropRate2) 
	{
		switch (iChaosCastleIndex) {
		case 0 :
			{
				iItemType = MAKE_ITEMNUM(14,13);	// 축복의 보석

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Bless",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬7 보상
		case 6 :
#endif
			{
				iItemType = MAKE_ITEMNUM(14,16);	// 생명의 보석

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Life",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		}
		iItemLevel = 0;
		iItemDur = 0;
	}
	else 
	{
		switch (iChaosCastleIndex) {
		case 0 :
			{
				//신규서버는 1캐슬에서 아이템드랍을 하지 않는다. 
				if (g_bNewServer)
				{
					iItemType = MAKE_ITEMNUM(14,14);	// 영혼의 보석
					
					LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Chaos of Bless",
						iChaosCastleIndex+1,
						gObj[iWinnerIndex].AccountID, 
						gObj[iWinnerIndex].Name
					);

					goto lab_end;
				}
				else
				{
					RingEventItemBoxOpen (&gObj[iWinnerIndex]);		// 일반 행운의 상자 드롭
					
					LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Lucky Box Item",
						iChaosCastleIndex+1,
						gObj[iWinnerIndex].AccountID, 
						gObj[iWinnerIndex].Name
					);
				}
			}
			break;
		case 1 :
			{
				//신규서버는 1캐슬에서 아이템드랍을 하지 않는다. 
				if (g_bNewServer)
				{
					iItemType = MAKE_ITEMNUM(14,13);	// 축복의 보석
					
					LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Bless",
						iChaosCastleIndex+1,
						gObj[iWinnerIndex].AccountID, 
						gObj[iWinnerIndex].Name
					);

					goto lab_end;
				}
				else
					goto lab_6;
			}
		case 2 :
		case 3 :
		case 4 :
		case 5 :
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬7 보상
		case 6 :
#endif
			{
lab_6:
				MakeRandomSetItem(iWinnerIndex);				// 세트 아이템 드롭

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Set Item",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		}

		//g_LogToFile.Output("[CC_LOG] GIVE WINNER ITEM (%d) : OUT (SET ITEM)", iChaosCastleIndex+1 );

		return;
	}
lab_end:
#else // ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113

#ifdef SEASON4_PATCH_GROOVING
	// grooving 시즌4 20080624
	// 드롭 확률 조정 창석(50->40%), 생석(45->55%), 세트 아이템(5->5%)
	if (iItemDropRate < 40)
#else
	if (iItemDropRate < 50) 
#endif
	{
		switch (iChaosCastleIndex) {
		case 0 :
			{
				iItemType = MAKE_ITEMNUM(14,14);	// 영혼의 보석

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Soul",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬7 보상
		case 6 :
#endif			
			{
				iItemType = MAKE_ITEMNUM(14,22);	// 창조의 보석

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Creative",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		}
		iItemLevel = 0;
		iItemDur = 0;
	}
	else if (iItemDropRate < 95) {
		switch (iChaosCastleIndex) {
		case 0 :
			{
				iItemType = MAKE_ITEMNUM(14,13);	// 축복의 보석

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Bless",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬7 보상
		case 6 :
#endif
			{
				iItemType = MAKE_ITEMNUM(14,16);	// 생명의 보석

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Life",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		}
		iItemLevel = 0;
		iItemDur = 0;
	}
	else {
		switch (iChaosCastleIndex) {
		case 0 :
			{
				RingEventItemBoxOpen (&gObj[iWinnerIndex]);		// 일반 행운의 상자 드롭

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Lucky Box Item",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬7 보상
		case 6 :
#endif
			{
				MakeRandomSetItem(iWinnerIndex);				// 세트 아이템 드롭

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Set Item",
					iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, 
					gObj[iWinnerIndex].Name
					);
			}
			break;
		}

		//g_LogToFile.Output("[CC_LOG] GIVE WINNER ITEM (%d) : OUT (SET ITEM)", iChaosCastleIndex+1 );

		return;
	}
#endif // ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113

	ItemSerialCreateSend(
		gObj[iWinnerIndex].m_Index, 
		gObj[iWinnerIndex].MapNumber, 
		(BYTE)gObj[iWinnerIndex].X, 
		(BYTE)gObj[iWinnerIndex].Y, 
		iItemType,								// 아아템 종류
		iItemLevel,								// 레벨
		(BYTE)iItemDur,							// 내구력
		0,										// 옵션 1
		0,										// 옵션 2
		0,										// 옵션 3
		iWinnerIndex							// 루팅 인덱스
		);
	
	//g_LogToFile.Output("[CC_LOG] GIVE WINNER ITEM (%d) : OUT (GEM)", iChaosCastleIndex+1 );
#else
	int drop_count = 0, used_count = 0;

	int count = m_LST_CHAOSCASTLE_REWARD[iChaosCastleIndex].size();

	tdCCRewardList::iterator it = m_LST_CHAOSCASTLE_REWARD[iChaosCastleIndex].begin();

	for(std::advance(it, rand()%count);used_count < count && drop_count < m_iDropCount[iChaosCastleIndex];advance(it = m_LST_CHAOSCASTLE_REWARD[iChaosCastleIndex].begin(),rand()%count),++used_count)
	{
		CHAOSCASTLE_REWARD &reward = (*it);

		if((rand()%1000) < reward.max_drop_rate)
		{
			drop_count++;
			ItemSerialCreateSend(gObj[iWinnerIndex].m_Index, gObj[iWinnerIndex].MapNumber, (BYTE)gObj[iWinnerIndex].X, (BYTE)gObj[iWinnerIndex].Y, reward.item_type, reward.item_level, (BYTE)ItemAttribute[reward.item_type].Durability, reward.is_skill, reward.is_luck, reward.option, iWinnerIndex, reward.exc_option);
		}
	}
#endif
}


INT CChaosCastle::ReCalcUserCount		(INT iChaosCastleIndex)				// 현재 입장한 사용자의 수를 다시 계산 (접속해서 캐슬안에 있는 사람 기준)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;

	INT iRET_VAL = 0;

	INT iPlayUser = GetCurPlayUser (iChaosCastleIndex);
	if (iPlayUser > 0) {
		iRET_VAL = m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER = iPlayUser;
	}
	else {
		iRET_VAL = m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER = 0;
	}

	return iRET_VAL;
}


INT CChaosCastle::RewardZettoMoney		(INT iChaosCastleIndex)				// 특정 캐슬에 제또 보상을 한다 (전체 입장수입의 20%를 제외한 금액)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return -1;

	INT iPlayUser = GetCurPlayUser (iChaosCastleIndex);

	if (iPlayUser <= 0) {
		return -1;
	}

	INT iZettoMoney = iPlayUser * (g_iChaosCastle_EnterCost[iChaosCastleIndex] * 80 / 100);

	if (iZettoMoney <= 0) {
		LogAddTD("[Chaos Castle] (%d) ERROR : REWARD ZEN <= 0 (%d)",
			iChaosCastleIndex+1,
			iZettoMoney
			);
		return -1;
	}

	vector<INT> vtZettoWinner;

	for (int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			if ((gObjIsConnected (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE) &&
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex ) )
#else
				(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex)
#endif
				)
			{
				vtZettoWinner.push_back(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex);
			}
		}
	}

	if ( (iPlayUser = vtZettoWinner.size()) <= 0 ) {
		return -1;
	}

	INT iZettoWinnerIndex = vtZettoWinner[rand()%iPlayUser];
	CHAR szZettoWinnerName[MAX_IDSTRING+1] = {0,};
	memcpy (szZettoWinnerName, gObj[iZettoWinnerIndex].Name, MAX_IDSTRING);

	gObj[iZettoWinnerIndex].Money += iZettoMoney;

#ifdef MODIFY_ZEN_MAX_20040414
	if (gObj[iZettoWinnerIndex].Money > MAX_ZEN) {
		gObj[iZettoWinnerIndex].Money = MAX_ZEN;
	}
#endif

	GCMoneySend(iZettoWinnerIndex, gObj[iZettoWinnerIndex].Money);

	// 여기서 공지를 한다.
	PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1237),	// "카오스 %d 캐슬 : %s 님께서 입장료를 모두 얻는 행운 당첨"
		iChaosCastleIndex+1, 
		szZettoWinnerName,
		iZettoMoney
		);
#else
	pNotice.type = 0;
	wsprintf(pNotice.Notice, lMsg.Get(1237),				// "카오스 %d 캐슬 : %s 님께서 입장료를 모두 얻는 행운 당첨"
		iChaosCastleIndex+1, 
		szZettoWinnerName,
		iZettoMoney
		);	
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

	for ( int i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1) {
			if(gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Connected == 3)
			{
				// 다리의 정식 등록자를 기준으로 카오스 캐슬맵에 없어도 살아만 있다면 메시지를 보내준다.
				if ((gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex != -1) && (gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex != -1))
					DataSend(m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);
				
#ifdef ADD_LOG_CHECK_CAHOSCASTLE_BLOODCASTLE_EVENT_20060314
				LogAddTD( "[Chaos Castle][Bug Tracer] (%d) [%s][%s] Reword ZettoMoney(Map:%d) Send Message",
					iChaosCastleIndex + 1, 
					gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].AccountID,
					gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Name,					
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 기존 버그 수정
					gObj[iZettoWinnerIndex].MapNumber
#else
					gObj[iZettoWinnerIndex].MapNumber - MAP_INDEX_CHAOSCASTLE1
#endif	// ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
					);
#endif
			}
		}
	}

//	SendAllUserAnyMsg ((LPBYTE) &pNotice, pNotice.h.size);

	if (CHECK_LIMIT(iZettoWinnerIndex, MAX_OBJECT)) {
		LogAddTD("[Chaos Castle] (%d) [%s][%s] Win Zen Lotto (RewardZen:%d, UserZen:%d)",
			iChaosCastleIndex+1,
			gObj[iZettoWinnerIndex].AccountID, 
			gObj[iZettoWinnerIndex].Name,
			iZettoMoney,
			gObj[iZettoWinnerIndex].Money
			);
	}

	return iZettoWinnerIndex;
}


#ifdef CHAOSCASTLE_GUILD_DUEL_BUGFIX_20040611									// 사용자들을 체크하여 만약 길드전이나 결투 중인 사람이 있다면 강제 종료 시킨다.

VOID CChaosCastle::TerminateGuildWarDuel (
			INT iChaosCastleIndex								
			)
{
	if (!CHECK_LIMIT(iChaosCastleIndex, MAX_CHAOSCASTLE_COUNT))
		return;
	
	for (INT i = 0 ; i < MAX_CHAOSCASTLE_USER_COUNT ; i++) {
		if (m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 &&
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 구하기
			gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByChaosCastle( iChaosCastleIndex )
#else
			gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1 + iChaosCastleIndex
#endif
			) 
		{
			LPOBJECTSTRUCT lpObj = &gObj[m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex];

			if (gObjIsConnected (lpObj->m_Index)) {
				// 1 . 길드전을 체크하여 길드전 중이라면 강제로 끊는다.
				if (lpObj->lpGuild != NULL) {					
					if (lpObj->lpGuild->WarState == 1 && strcmp(lpObj->lpGuild->Name, "")) {
						GCManagerGuildWarEnd(lpObj->lpGuild->Name);
					}
				}
				
				// 2 . 결투를 체크한다. -> 결투 중이라면 강제 종료
				if (gObjDuelCheck(lpObj)) {
					gObjSendDuelEnd(&gObj[lpObj->m_iDuelUser]);
					gObjSendDuelEnd(&lpObj);
					gObjResetDuel(&lpObj);
				}
				
			}
			else {
				m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex = -1;
			}
		}
	}
}

#endif


/* 카오스 캐슬 오프라인 경품 지급 관련 함수들 */
VOID CChaosCastle::CheckRegisterOfflineGift (									// 카오스 캐슬의 오프라인 경품에 당첨 시킬 수 있는지 체크한다.
			INT iUserIndex
			)
{
	if (!gObjIsConnected(iUserIndex))
		return;
	
#ifdef MU_CHN_OFFLINE_GIFT_EVENT_20040908
	if (!gbChnOfflineGiftEvent)
		return;

	INT iRate1		= rand()%10000+1;
	INT iRate2		= rand()%10000+1;
	INT iGiftRate	= iRate1 * iRate2;				// 1/100000000 (중국)
	if (iGiftRate <= giChnOfflineGiftEventDropRate) {
		// 오프라인 경품에 당첨 되었다.
		EGReqRegCCOfflineGift (iUserIndex);
	}
#else
	INT iGiftRate = rand()%10000;					// 1/10000 (그외)

	if (iGiftRate < g_iChaosCastle_OffLineGiftRate) {
		// 오프라인 경품에 당첨 되었다.
		EGReqRegCCOfflineGift (iUserIndex);
	}
#endif
	
}

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 다리번호, 맵번호 얻기
// 카오스캐슬 번호로 맵 번호 구하기
INT	CChaosCastle::GetMapNumByChaosCastle( INT iChaosCastleIndex )
{
	INT iMapNum = 0;
	switch( iChaosCastleIndex )
	{
	case 0: 	iMapNum = MAP_INDEX_CHAOSCASTLE1;	break;
	case 1:		iMapNum = MAP_INDEX_CHAOSCASTLE2;	break;
	case 2:		iMapNum = MAP_INDEX_CHAOSCASTLE3;	break;
	case 3:		iMapNum = MAP_INDEX_CHAOSCASTLE4;	break;
	case 4:		iMapNum = MAP_INDEX_CHAOSCASTLE5;	break;
	case 5:		iMapNum = MAP_INDEX_CHAOSCASTLE6;	break;
	case 6:		iMapNum = MAP_INDEX_CHAOSCASTLE7;	break;	
	}

	return iMapNum;	
}

// 맵 번호로 카오스캐슬 번호 구하기
INT	CChaosCastle::GetChaosCastleByMapNum( INT iMapNum )
{
	int iChaosCastleIndex = -1;
	switch( iMapNum )
	{
	case 18:	iChaosCastleIndex = 0;	break;
	case 19:	iChaosCastleIndex = 1;	break;
	case 20:	iChaosCastleIndex = 2;	break;
	case 21:	iChaosCastleIndex = 3;	break;
	case 22:	iChaosCastleIndex = 4;	break;
	case 23:	iChaosCastleIndex = 5;	break;	
	case 53:	iChaosCastleIndex = 6;	break;
	}

	return iChaosCastleIndex;	
}

#endif

#endif