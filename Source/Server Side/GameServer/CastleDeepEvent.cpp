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
		// ����Ÿ�� ������ 0
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
		// ������ �׷��� ã�� ���ߴ�.
		LogAddTD("[CastleDeep Event] Start Fail - bDoStart == FALSE");
		SetState(CASTLEDEEPEVENT_STATE_NONE);
		return;
	}

	m_iCUR_SPAWN_STEP	= 0;
	
	TP_CASTLEDEEP_SPAWNTIME_ITR it	= m_mapSpawnTime.find(m_iCUR_ASSULT_TYPE);
	if (it == m_mapSpawnTime.end()) {
		// ����Ÿ���� ������ ������ ����
		SetState(CASTLEDEEPEVENT_STATE_NONE);
		return;
	}

	// �ð��� üũ�Ͽ� ���͸� ������ų ������ �����͸� �����. (�����ϸ� ������ �ϳ��� ����)
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
	// *> . ��ũ��Ʈ�� �о �ʿ��� �����͸� �����Ѵ�.
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
			{	// �̺�Ʈ�� �⺻ ����������
				CASTLEDEEP_EVENTTIME pRET;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				pRET.m_iHour = (int)TokenNumber;										// �̺�Ʈ ���� �ð�
				Token = (*GetToken)(); pRET.m_iMinute			= (int)TokenNumber;		// �̺�Ʈ ���� ��
				
				m_vtEventTime.push_back(pRET);
			}
			else if( type == 1 )
			{	// ����Ÿ�� ����
				vector<CASTLEDEEP_MONSTERINFO>	vtTempMonsterInfo;
				vector<CASTLEDEEP_SPAWNTIME>	vtTempSpawnTime;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				INT iAssultType = (int)TokenNumber;										// ����Ÿ��
				Token = (*GetToken)();
				INT iAssultRate	= (int)TokenNumber;										// ����Ÿ���� Ȯ��
				
				m_mapMonsterInfo.insert(pair<int, vector<CASTLEDEEP_MONSTERINFO> > (iAssultType, vtTempMonsterInfo));
				m_mapSpawnTime.insert(pair<int, vector<CASTLEDEEP_SPAWNTIME> > (iAssultType, vtTempSpawnTime));

				m_vtAssultType.push_back(iAssultType);
				m_vtAssultTypeRate.push_back(iAssultRate);
			}
			else if( type == 2 )
			{	// �̺�Ʈ ���� �� ����Ÿ��, �׷� �� �ð�����
				CASTLEDEEP_SPAWNTIME stSpawnTime;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				INT iAssultType	= (int)TokenNumber;													// ����Ÿ��
				Token = (*GetToken)(); stSpawnTime.m_iMonsterGroup			= (int)TokenNumber;		// ���� �׷��ȣ
				Token = (*GetToken)(); stSpawnTime.m_iMonsterSpawnMinute	= (int)TokenNumber;		// ���� �׷� �����ð�

				EnterCriticalSection (& m_critEventData);
				TP_CASTLEDEEP_SPAWNTIME_ITR	it	= m_mapSpawnTime.find(iAssultType);
				if (it != m_mapSpawnTime.end()) {
					((vector<CASTLEDEEP_SPAWNTIME> *)&it->second)->push_back(stSpawnTime);
				}
				LeaveCriticalSection (& m_critEventData);
			}
			else if( type == 3 )
			{	// �� ����Ÿ��, �׷캰 ���� ������ ����
				CASTLEDEEP_MONSTERINFO stMonsterInfo;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				INT iAssultType	= (int)TokenNumber;													// ����Ÿ��
				Token = (*GetToken)(); stMonsterInfo.m_iMonsterGroup		= (int)TokenNumber;		// ���� �׷��ȣ
				Token = (*GetToken)(); stMonsterInfo.m_iMonsterType			= (int)TokenNumber;		// ���� Ÿ��
				Token = (*GetToken)(); stMonsterInfo.m_iMonsterNumber		= (int)TokenNumber;		// ���� ����
				Token = (*GetToken)(); stMonsterInfo.m_iSX					= (int)TokenNumber;		// ���� ���� ����X
				Token = (*GetToken)(); stMonsterInfo.m_iSY					= (int)TokenNumber;		// ���� ���� ����Y
				Token = (*GetToken)(); stMonsterInfo.m_iDX					= (int)TokenNumber;		// ���� ���� ����X
				Token = (*GetToken)(); stMonsterInfo.m_iDY					= (int)TokenNumber;		// ���� ���� ����Y
				
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
	case TRUE :			// ���� �ð����� ������ ����� �ð��� ũ��.
		m_iTIME_MSEC_REMAIN = ( ((60*iMIN_HOUR) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) * 1000;
		break;
	case TRUE + 1 :		// ���� �ð����� ������ ����� �ð��� �۴� (���� �� ����)
		m_iTIME_MSEC_REMAIN = ( ((60*(iMIN_HOUR+24)) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) * 1000;
		break;
	default:			// ����
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
		CheckSync();		// ���⼭ ���� �ð��� ����Ͽ� ���Ѵ�.
	else
		SetState (CASTLEDEEPEVENT_STATE_NONE);
}

VOID CCastleDeepEvent::SetState_Playing		()
{
//	m_iTIME_MSEC_REMAIN = m_iTIME_MIN_PLAY * 60 * 1000;			// CASTLEDEEPEVENT_STATE_PLAYING -> CASTLEDEEPEVENT_STATE_PLAYEND ������ ���� �ð�
	m_iTIME_MSEC_REMAIN	= 0;
	m_bCheckEnd = FALSE;
	m_uCheckEndTick = 0;

	ClearMonster();			// ���� �̺�Ʈ ���� ����
	StartEvent();			// �̺�Ʈ ���� ����
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
			// ����> . ������ �� �ʵ� �� �ִ�. !!
			if ((m_iTIME_MSEC_REMAIN <= CASTLEDEEP_NOTIFYTIME * 60 * 1000) && (m_iTIME_MSEC_REMAIN > 0) && (m_iTIME_MSEC_REMAIN / (60*1000) != m_iTIME_NOTIFY_COUNT )) {	
				m_iTIME_NOTIFY_COUNT = m_iTIME_MSEC_REMAIN / (60*1000);

				if (m_iTIME_NOTIFY_COUNT+1 == CASTLEDEEP_NOTIFYTIME) {	// N�� ���� �� �� �������ش�.
					CHAR szTemp[256];

#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
					// 3224 "���͵��� �η� ������ ���� �����ϰ� �ּ�! ������ ��û�Ͽ�!"
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
		// ���ð��� �� �Ǿ���. -> �̺�Ʈ ����
		CHAR szTemp[256];

#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
		// 3225 "���͵��� �η� ������ ���� �����ϰ� �ּ�. �״���� ���� �����ֽÿ�!"
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

		// �ð��� �Ǹ� ���͸� �ϳ��� �߰����ش�.
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
		// ���� �׷��� ��� ��ȯ�ϸ� �ϴ� �̺�Ʈ�� ������. -> N�� �� ����� �����

		if (m_bCheckEnd == FALSE) {
			m_bCheckEnd = TRUE;
			m_uCheckEndTick = GetTickCount();
		}
		else {
			if (m_uCheckEndTick + CASTLEDEEP_ENDTICKCOUNT < GetTickCount()) {
				CHAR szTemp[256];

#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
				// 3226 "�״���� Ȱ������ ���͵��� ����ĥ �� �־���. ����帮��."
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

VOID CCastleDeepEvent::ClearMonster			()						// �̺�Ʈ ���͵��� ����
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


VOID CCastleDeepEvent::AddMonster			(						// Ư�� ����Ÿ��, �׷��� ���͵��� �߰��Ѵ�.
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
				
		// ���� �߰�
		BYTE cSX, cSY;
		for (INT iMonCount = 0 ; iMonCount < stMonsterInfo.m_iMonsterNumber ; iMonCount++) {
			if (gObjGetRandomFreeArea(MAP_INDEX_CASTLESIEGE, cSX, cSY, stMonsterInfo.m_iSX, stMonsterInfo.m_iSY, stMonsterInfo.m_iDX, stMonsterInfo.m_iDY, 30) == TRUE) {
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
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

					gObj[result].m_Attribute		= MON_ATTR_CASTLEDEEP;	// ������ gObjDel()
					gObj[result].MaxRegenTime		= 0;
					gObj[result].Dir				= rand()%8;				// ������ ����
					gObj[result].DieRegen			= 0;					// �� ��ġ�� �ʱ�ȭ ���� �ʾƼ� ���Ͱ� ������ ���ÿ� �����

					gObj[result].RegenTime			= 1;
					gObj[result].MaxRegenTime		= 1000;
					gObj[result].m_dwLastCheckTick	= GetTickCount();	// ��ü ������ ���� �ð�����
				}
			}
		}
	}
	
}


#endif
