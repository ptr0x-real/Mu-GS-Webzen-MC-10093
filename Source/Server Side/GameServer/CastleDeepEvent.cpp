// CastleDeepEvent.cpp: implementation of the CCastleDeepEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CastleDeepEvent.h"

#include "Include\ReadScript.h"
#include "Include\Prodef.h"
#include "GameMain.h"
#include "Protocol.h"
#include "Include\cb_protocol.h"
#include "Event.h"
#include "Common\Winutil.h"
#include "user.h"
#include "gObjMonster.h"

#ifdef CASTLE_MAIN_SCHEDULER_20041111
#include "CastleSiege.h"
#endif

#ifdef CASTLEDEEP_EVENT_20050413

CCastleDeepEvent	g_CastleDeepEvent;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCastleDeepEvent::CCastleDeepEvent	()
{
	m_bDoEvent = FALSE;
	m_bHasData = FALSE;

	srand(time(0));

	InitializeCriticalSection(& m_critEventData);
}

CCastleDeepEvent::~CCastleDeepEvent	()
{
	DeleteCriticalSection(& m_critEventData);
}

VOID CCastleDeepEvent::StartEvent	()
{
	if (m_vtAssultType.size() <= 0) {
		// 습격타입 개수가 0
		LogAddTD("[CastleDeep Event] Start Fail - No Assult Data");
		SetState(CASTLEDEEPEVENT_STATE_NONE);
		return;
	}

	INT iCheckCount		= CASTLEDEEP_MAXSPAWNCHECKCOUNT;
	INT iASSULT_RATE	= 0;
	BOOL bDoStart		= FALSE;
	while (iCheckCount-- > 0) {
		int iRndIndex	= rand()%m_vtAssultType.size();
		m_iCUR_ASSULT_TYPE	= m_vtAssultType[iRndIndex];
		iASSULT_RATE		= m_vtAssultTypeRate[iRndIndex];

		if (rand()%100 < iASSULT_RATE) {
			bDoStart	= TRUE;
			break;
		}
	}

	if (bDoStart == FALSE) {
		// 습격할 그룹을 찾지 못했다.
		LogAddTD("[CastleDeep Event] Start Fail - bDoStart == FALSE");
		SetState(CASTLEDEEPEVENT_STATE_NONE);
		return;
	}

	m_iCUR_SPAWN_STEP	= 0;
	
	TP_CASTLEDEEP_SPAWNTIME_ITR it	= m_mapSpawnTime.find(m_iCUR_ASSULT_TYPE);
	if (it == m_mapSpawnTime.end()) {
		// 습격타입의 스케쥴 정보가 없음
		SetState(CASTLEDEEPEVENT_STATE_NONE);
		return;
	}

	// 시간을 체크하여 몬스터를 출현시킬 스케쥴 데이터를 만든다. (출현하면 데이터 하나씩 삭제)
	m_mapRunCheck.clear();
	vector<CASTLEDEEP_SPAWNTIME>::iterator it2	= ((vector<CASTLEDEEP_SPAWNTIME> *)&it->second)->begin();
	for (; it2 != ((vector<CASTLEDEEP_SPAWNTIME> *)&it->second)->end() ; it2++) {
		CASTLEDEEP_SPAWNTIME & stSpawnTime	= *it2;
		m_mapRunCheck.insert(pair<int, int> (stSpawnTime.m_iMonsterGroup, stSpawnTime.m_iMonsterSpawnMinute));
	}
}

VOID CCastleDeepEvent::StopEvent	()
{
	
}

VOID CCastleDeepEvent::EnableEvent	(BOOL bDoEvent)
{
	m_bDoEvent = bDoEvent;

	if (m_bDoEvent) {
		SetState (CASTLEDEEPEVENT_STATE_CLOSED);
	}
	else {
		SetState (CASTLEDEEPEVENT_STATE_NONE);
	}
}

BOOL CCastleDeepEvent::Load			(LPSTR lpszFileName)
{
	// *> . 스크립트를 읽어서 필요한 데이터를 저장한다.
	if (lpszFileName == NULL)
		return FALSE;

	if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
	{
		MsgBox("[CastleDeep Event] Info file Load Fail [%s]", lpszFileName);
		return FALSE;
	}

	EnterCriticalSection (& m_critEventData);
	m_vtAssultType.clear();
	m_vtAssultTypeRate.clear();
	m_mapSpawnTime.clear();
	m_mapMonsterInfo.clear();
	m_vtEventTime.clear();
	m_mapRunCheck.clear();
	LeaveCriticalSection (& m_critEventData);

	m_bHasData		= FALSE;

	SMDToken Token;
	
	int type = -1;
	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{				
			if( type == 0 )
			{	// 이벤트의 기본 스케쥴정보
				CASTLEDEEP_EVENTTIME pRET;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				pRET.m_iHour = (int)TokenNumber;										// 이벤트 시작 시간
				Token = (*GetToken)(); pRET.m_iMinute			= (int)TokenNumber;		// 이벤트 시작 분
				
				m_vtEventTime.push_back(pRET);
			}
			else if( type == 1 )
			{	// 습격타입 종류
				vector<CASTLEDEEP_MONSTERINFO>	vtTempMonsterInfo;
				vector<CASTLEDEEP_SPAWNTIME>	vtTempSpawnTime;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				INT iAssultType = (int)TokenNumber;										// 습격타입
				Token = (*GetToken)();
				INT iAssultRate	= (int)TokenNumber;										// 습격타입의 확률
				
				m_mapMonsterInfo.insert(pair<int, vector<CASTLEDEEP_MONSTERINFO> > (iAssultType, vtTempMonsterInfo));
				m_mapSpawnTime.insert(pair<int, vector<CASTLEDEEP_SPAWNTIME> > (iAssultType, vtTempSpawnTime));

				m_vtAssultType.push_back(iAssultType);
				m_vtAssultTypeRate.push_back(iAssultRate);
			}
			else if( type == 2 )
			{	// 이벤트 진행 시 습격타입, 그룹 별 시간순서
				CASTLEDEEP_SPAWNTIME stSpawnTime;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				INT iAssultType	= (int)TokenNumber;													// 습격타입
				Token = (*GetToken)(); stSpawnTime.m_iMonsterGroup			= (int)TokenNumber;		// 몬스터 그룹번호
				Token = (*GetToken)(); stSpawnTime.m_iMonsterSpawnMinute	= (int)TokenNumber;		// 몬스터 그룹 리젠시간

				EnterCriticalSection (& m_critEventData);
				TP_CASTLEDEEP_SPAWNTIME_ITR	it	= m_mapSpawnTime.find(iAssultType);
				if (it != m_mapSpawnTime.end()) {
					((vector<CASTLEDEEP_SPAWNTIME> *)&it->second)->push_back(stSpawnTime);
				}
				LeaveCriticalSection (& m_critEventData);
			}
			else if( type == 3 )
			{	// 각 습격타입, 그룹별 몬스터 종류와 개수
				CASTLEDEEP_MONSTERINFO stMonsterInfo;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				INT iAssultType	= (int)TokenNumber;													// 습격타입
				Token = (*GetToken)(); stMonsterInfo.m_iMonsterGroup		= (int)TokenNumber;		// 몬스터 그룹번호
				Token = (*GetToken)(); stMonsterInfo.m_iMonsterType			= (int)TokenNumber;		// 몬스터 타입
				Token = (*GetToken)(); stMonsterInfo.m_iMonsterNumber		= (int)TokenNumber;		// 몬스터 개수
				Token = (*GetToken)(); stMonsterInfo.m_iSX					= (int)TokenNumber;		// 몬스터 출현 시작X
				Token = (*GetToken)(); stMonsterInfo.m_iSY					= (int)TokenNumber;		// 몬스터 출현 시작Y
				Token = (*GetToken)(); stMonsterInfo.m_iDX					= (int)TokenNumber;		// 몬스터 출현 종료X
				Token = (*GetToken)(); stMonsterInfo.m_iDY					= (int)TokenNumber;		// 몬스터 출현 종료Y
				
				EnterCriticalSection (& m_critEventData);
				TP_CASTLEDEEP_MONSTERINFO_ITR	it	= m_mapMonsterInfo.find(iAssultType);
				if (it != m_mapMonsterInfo.end()) {
					((vector<CASTLEDEEP_MONSTERINFO> *)&it->second)->push_back(stMonsterInfo);
				}
				LeaveCriticalSection (& m_critEventData);
			}
		}
	}
	fclose(SMDFile);

	LogAdd("%s file load!", lpszFileName);

	m_bHasData = TRUE;

	return TRUE;
}

VOID CCastleDeepEvent::CheckSync	()
{
	if (m_vtEventTime.empty()) {
		LogAddC(2, "[CastleDeep Event] No Schedule Time Data");
		SetState(CASTLEDEEPEVENT_STATE_NONE);
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime (& sysTime);

	INT iMIN_HOUR		= 24;
	INT iMIN_MINUTE		= 60;
	BOOL bTIME_CHANGED	= FALSE;

	vector <CASTLEDEEP_EVENTTIME>::iterator it = m_vtEventTime.begin();

	for (;it != m_vtEventTime.end();it++) {
		CASTLEDEEP_EVENTTIME & pRET = *it;
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
		it = m_vtEventTime.begin();
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;
		
		for (;it != m_vtEventTime.end();it++) {
			CASTLEDEEP_EVENTTIME & pRET = *it;
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
		m_iTIME_MSEC_REMAIN = ( ((60*iMIN_HOUR) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) * 1000;
		break;
	case TRUE + 1 :		// 현재 시간보다 앞으로 진행될 시간이 작다 (내일 할 예정)
		m_iTIME_MSEC_REMAIN = ( ((60*(iMIN_HOUR+24)) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) * 1000;
		break;
	default:			// 에러
		LogAddC(2, "[CastleDeep Event] No Schedule Time Data");
		SetState(CASTLEDEEPEVENT_STATE_NONE);
		return;
	}
	
	m_iTIME_TICK_COUNT = GetTickCount();
	
	LogAddTD("[CastleDeep Event] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)", m_iTIME_MSEC_REMAIN / (60 * 1000), iMIN_HOUR, iMIN_MINUTE);

	m_iTIME_NOTIFY_COUNT = CASTLEDEEP_NOTIFYTIME;
}

VOID CCastleDeepEvent::SetState		(INT iSTATE)
{ 
	if ((m_iEVENT_STATE < CASTLEDEEPEVENT_STATE_NONE) || (m_iEVENT_STATE > CASTLEDEEPEVENT_STATE_PLAYING))
		return;

	m_iEVENT_STATE = iSTATE;

	switch(m_iEVENT_STATE) {
	case CASTLEDEEPEVENT_STATE_NONE :
		SetState_None();
		break;
	case CASTLEDEEPEVENT_STATE_CLOSED :
		SetState_Closed();
		break;
	case CASTLEDEEPEVENT_STATE_PLAYING :
		SetState_Playing();
		break;
	default:
		break;
	}	
}


VOID CCastleDeepEvent::SetState_None		()
{
	StopEvent();
}

VOID CCastleDeepEvent::SetState_Closed		()
{
	StopEvent();

	if (m_bDoEvent)
		CheckSync();		// 여기서 남은 시간을 계산하여 정한다.
	else
		SetState (CASTLEDEEPEVENT_STATE_NONE);
}

VOID CCastleDeepEvent::SetState_Playing		()
{
//	m_iTIME_MSEC_REMAIN = m_iTIME_MIN_PLAY * 60 * 1000;			// CASTLEDEEPEVENT_STATE_PLAYING -> CASTLEDEEPEVENT_STATE_PLAYEND 까지의 남은 시간
	m_iTIME_MSEC_REMAIN	= 0;
	m_bCheckEnd = FALSE;
	m_uCheckEndTick = 0;

	ClearMonster();			// 이전 이벤트 몬스터 제거
	StartEvent();			// 이벤트 몬스터 등장
}

VOID CCastleDeepEvent::Run			()
{

	if (m_bDoEvent) {
		switch (m_iEVENT_STATE) {
		case CASTLEDEEPEVENT_STATE_NONE :
			ProcState_None();
			break;
		case CASTLEDEEPEVENT_STATE_CLOSED :
			ProcState_Closed();
			break;
		case CASTLEDEEPEVENT_STATE_PLAYING :
			ProcState_Playing();
			break;
		default:
			break;
		}
	}	
}

VOID CCastleDeepEvent::ProcState_None		()
{
	
}

VOID CCastleDeepEvent::ProcState_Closed		()
{
	INT iTICK_MSEC = (GetTickCount() - m_iTIME_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		m_iTIME_TICK_COUNT = GetTickCount();

		if (m_bDoEvent) {
			// 주의> . 공지가 잘 않될 수 있다. !!
			if ((m_iTIME_MSEC_REMAIN <= CASTLEDEEP_NOTIFYTIME * 60 * 1000) && (m_iTIME_MSEC_REMAIN > 0) && (m_iTIME_MSEC_REMAIN / (60*1000) != m_iTIME_NOTIFY_COUNT )) {	
				m_iTIME_NOTIFY_COUNT = m_iTIME_MSEC_REMAIN / (60*1000);

				if (m_iTIME_NOTIFY_COUNT+1 == CASTLEDEEP_NOTIFYTIME) {	// N분 전에 한 번 공지해준다.
					CHAR szTemp[256];

#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
					// 3224 "몬스터들이 로랜 협곡을 향해 진군하고 있소! 원군을 요청하오!"
					wsprintf(szTemp, lMsg.Get(3224));
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
					g_CastleSiege.SendMapServerGroupMsg(szTemp);
#else
					AllSendServerMsg (szTemp);
#endif
				}
			}
		}

		LogAddC(LOGC_GREEN, "[CastleDeep Event] ProcState_Closed()	LEFT-MSEC:%d", m_iTIME_MSEC_REMAIN);
	}
	
	if (m_iTIME_MSEC_REMAIN <= 0) {
		// 대기시간이 다 되었다. -> 이벤트 시작
		CHAR szTemp[256];

#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
		// 3225 "몬스터들이 로랜 협곡의 성을 공격하고 있소. 그대들의 힘을 보여주시오!"
		wsprintf(szTemp, lMsg.Get(3225));
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
		g_CastleSiege.SendMapServerGroupMsg(szTemp);
#else
		AllSendServerMsg (szTemp);
#endif

		if (m_bDoEvent)
			SetState (CASTLEDEEPEVENT_STATE_PLAYING);
		else
			SetState (CASTLEDEEPEVENT_STATE_NONE);

		LogAddTD("[CastleDeep Event] - Event Started");
	}	
}

VOID CCastleDeepEvent::ProcState_Playing	()
{
	INT iTICK_MSEC = (GetTickCount() - m_iTIME_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
//		m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		m_iTIME_MSEC_REMAIN += iTICK_MSEC;
		m_iTIME_TICK_COUNT = GetTickCount();

		// 시간이 되면 몬스터를 하나씩 추가해준다.
		TP_CASTLEDEEP_RUNCHECK_ITR it	= m_mapRunCheck.begin();
		for (; it != m_mapRunCheck.end() ; it++) {
			INT iSpawnMin	= it->second;
			if (iSpawnMin == m_iTIME_MSEC_REMAIN / (60*1000)) {
				AddMonster(m_iCUR_ASSULT_TYPE, it->first);
				m_mapRunCheck.erase(it);
				break;
			}
		}

		LogAddC(LOGC_GREEN, "[CastleDeep Event] ProcState_Playing()	LEFT-MSEC:%d", m_iTIME_MSEC_REMAIN);
	}
	
	if (m_mapRunCheck.size() <= 0) {
		// 몬스터 그룹을 모두 소환하면 일단 이벤트는 끝난다. -> N분 후 종료로 변경됨

		if (m_bCheckEnd == FALSE) {
			m_bCheckEnd = TRUE;
			m_uCheckEndTick = GetTickCount();
		}
		else {
			if (m_uCheckEndTick + CASTLEDEEP_ENDTICKCOUNT < GetTickCount()) {
				CHAR szTemp[256];

#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
				// 3226 "그대들의 활약으로 몬스터들을 물리칠 수 있었소. 감사드리오."
				wsprintf(szTemp, lMsg.Get(3226));
#endif

				AllSendServerMsg (szTemp);

				if (m_bDoEvent)
					SetState (CASTLEDEEPEVENT_STATE_CLOSED);
				else
					SetState (CASTLEDEEPEVENT_STATE_NONE);
			}
		}

		LogAddTD("[CastleDeep Event] - Event Ended");
	}
}

VOID CCastleDeepEvent::ClearMonster			()						// 이벤트 몬스터들을 제거
{
	for (INT iIndex = 0 ; iIndex < MAX_MONSTER ; iIndex++) {
		if (gObjIsConnected(iIndex) &&
			gObj[iIndex].m_Attribute == MON_ATTR_CASTLEDEEP
			) 
		{
			gObjDel(iIndex);
		}
	}
}


VOID CCastleDeepEvent::AddMonster			(						// 특정 습격타입, 그룹의 몬스터들을 추가한다.
	INT iAssultType, 
	INT iGroup
	)
{
	TP_CASTLEDEEP_MONSTERINFO_ITR	it	= m_mapMonsterInfo.find(iAssultType);
	if (it == m_mapMonsterInfo.end())
		return;

	vector<CASTLEDEEP_MONSTERINFO>::iterator it2	= ((vector<CASTLEDEEP_MONSTERINFO> *)&it->second)->begin();
	for (; it2 != ((vector<CASTLEDEEP_MONSTERINFO> *)&it->second)->end() ; it2++) {
		CASTLEDEEP_MONSTERINFO & stMonsterInfo	= *it2;

		if (stMonsterInfo.m_iMonsterGroup != iGroup)
			continue;
				
		// 몬스터 추가
		BYTE cSX, cSY;
		for (INT iMonCount = 0 ; iMonCount < stMonsterInfo.m_iMonsterNumber ; iMonCount++) {
			if (gObjGetRandomFreeArea(MAP_INDEX_CASTLESIEGE, cSX, cSY, stMonsterInfo.m_iSX, stMonsterInfo.m_iSY, stMonsterInfo.m_iDX, stMonsterInfo.m_iDY, 30) == TRUE) {
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
				int result = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
				int result = gObjAdd();
#endif
				if( result >= 0 )
				{
					gObj[result].m_PosNum			= -1;
					gObj[result].X					= cSX;
					gObj[result].Y					= cSY;
					gObj[result].MapNumber			= MAP_INDEX_CASTLESIEGE;
					gObj[result].TX					= gObj[result].X;
					gObj[result].TY					= gObj[result].Y;
					gObj[result].m_OldX				= gObj[result].X;
					gObj[result].m_OldY				= gObj[result].Y;
					gObj[result].Dir				= 1;
					gObj[result].StartX				= (BYTE)gObj[result].X;
					gObj[result].StartY				= (BYTE)gObj[result].Y;
					gObjSetMonster(result, stMonsterInfo.m_iMonsterType);

					gObj[result].m_Attribute		= MON_ATTR_CASTLEDEEP;	// 죽으면 gObjDel()
					gObj[result].MaxRegenTime		= 0;
					gObj[result].Dir				= rand()%8;				// 방향은 랜덤
					gObj[result].DieRegen			= 0;					// 이 수치가 초기화 되지 않아서 몬스터가 생성과 동시에 사라짐

					gObj[result].RegenTime			= 1;
					gObj[result].MaxRegenTime		= 1000;
					gObj[result].m_dwLastCheckTick	= GetTickCount();	// 객체 삭제를 위한 시간설정
				}
			}
		}
	}
	
}


#endif
