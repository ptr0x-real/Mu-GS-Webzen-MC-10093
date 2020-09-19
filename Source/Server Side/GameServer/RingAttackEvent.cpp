// RingAttackEvent.cpp: implementation of the CRingAttackEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef MONSTER_HERD_SYSTEM_20031120

#include <time.h>

#include "RingAttackEvent.h"
#include "..\\Include\\ReadScript.h"
#include "..\\Include\\Prodef.h"
#include "GameMain.h"
#include "Protocol.h"
#include "..\\Include\\cb_protocol.h"
#include "Event.h"
#include "..\\Common\\Winutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*==========================================================
	클래스 설명 : MonsterHerd 클래스를 상속받은 반지의 제왕 
		몬스터 무리 클래스
==========================================================*/

CRingMonsterHerd::CRingMonsterHerd()
{
	
}

CRingMonsterHerd::~CRingMonsterHerd()
{
	
}

BOOL CRingMonsterHerd::Start()
{
	m_bEventSuccess = FALSE;

	return MonsterHerd::Start();
}

BOOL CRingMonsterHerd::MonsterHerdItemDrop(
	LPOBJECTSTRUCT lpObj
	)
{
	if (lpObj->Class == 135) {		// 백색의 마법사라면 무조건 축석
		int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		int iType = ItemGetNumberMake(14, 13);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE) lpObj->X, (BYTE) lpObj->Y, iType, 0, (BYTE)0, 0, 0, 0, iMaxHitUser);

		// 이벤트가 끝났다는 공지를 한다.
		CHAR szTemp[256];
		wsprintf(szTemp, lMsg.Get(1205), gObj[iMaxHitUser].Name, g_TerrainManager.GetMapName(lpObj->MapNumber));	// "%s님이 %s를 침략한 백색의 마법사 군단을 물리쳤습니다."
		AllSendServerMsg (szTemp);

		LogAddTD("[Ring Event] White Wizard Killed by [%s][%s], MapNumber:%d", gObj[iMaxHitUser].AccountID, gObj[iMaxHitUser].Name, gObj[iMaxHitUser].MapNumber);
		return TRUE;
	}
	else if (
		lpObj->Class == 136 ||
		lpObj->Class == 137			// 오크들이라면 30% 확률로 마법의 반지
		) 
	{
		if (rand()%100 < 30) {
			int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			int iType = ItemGetNumberMake(13, 20);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE) lpObj->X, (BYTE) lpObj->Y, iType, 0, (BYTE)30, 0, 0, 0, iMaxHitUser);
			return TRUE;
		}
		else if (rand()%g_iRingOrcKillGiftRate == 0) {			//  1/n 확률로 당첨이 된다면
			int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

			SendEventGiftWinner (iMaxHitUser, 1);				// 오크를 죽인 경품을 탄다.
			return TRUE;
		}
		else {
			// 나머지는 돈을 10000젠 떨군다.
			MapC[lpObj->MapNumber].MoneyItemDrop((int)10000, (BYTE) lpObj->X ,(BYTE) lpObj->Y);
			return TRUE;
		}
	}

	return FALSE;
}


VOID CRingMonsterHerd::MonsterAttackAction(	// 무리 몬스터 중 특정 몬스터가 공격했을 때의 처리
	LPOBJECTSTRUCT lpObj,					// 나
	LPOBJECTSTRUCT lpTargetObj				// 내가 공격한 인간?
	)
{
#ifdef NEW_FORSKYLAND2
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == true )
	#else
	if( lpObj->m_SkillHarden != 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		return;
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STUN ) == true )
	#else
	if( lpObj->m_iSkillStunTime > 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		return;
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 슬립상태에서는 공격을 할 수 없다
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == true )
	{
		return;
	}
#endif
	
	if (lpObj == NULL)
		return;

	if (lpTargetObj == NULL)
		return;

	if (lpObj->Connected < 3 || lpObj->Type != OBJTYPE_MONSTER)
		return;

	if (lpTargetObj->Connected < 3)
		return;

	if (lpObj->Class == 135) {			
		OrderAttack(lpObj, lpTargetObj, 90);	// 백색의 마법사가 무리 전원에게 공격을 명령
	}
}


VOID CRingMonsterHerd::SendEventGiftWinner(INT iIndex, INT iGiftKind)
{
	if (gObjIsConnected(iIndex) == FALSE)
		return;

	if( gObj[iIndex].UseEventServer )
	return;

	gObj[iIndex].UseEventServer = TRUE;

	PMSG_REQ_REG_RINGGIFT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x10, sizeof(pMsg));

	pMsg.iINDEX = iIndex;
	memcpy (pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING);
	pMsg.szUID[sizeof(pMsg.szUID)-1] = 0;
	pMsg.btGiftKind = iGiftKind;

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));

	LogAddTD("[Ring Event] [%s][%s] Request to Register Gift - Gift Kind (%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iGiftKind);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*==========================================================
	클래스 설명 : MonsterHerd 클래스를 상속받은 반지의 제왕 
		몬스터 무리 클래스
==========================================================*/

CRingAttackEvent::CRingAttackEvent()
{
	m_bDoEvent = FALSE;
	m_bHasData = FALSE;

	m_iRADIUS_MIN = RINGEVENT_MIN_RADIUS;
	m_iRADIUS_MAX = RINGEVENT_MAX_RADIUS;
	m_iMOVE_RAND_SIZE = RINGEVENT_MAX_MOVE;

	InitializeCriticalSection (& m_critMonsterAddData);
}

CRingAttackEvent::~CRingAttackEvent()
{
	DeleteCriticalSection (& m_critMonsterAddData);
}

VOID CRingAttackEvent::StartEvent ()
{
	if (!m_bHasData)
		return;

	INT iRandX, iRandY;
	
#ifdef SEASON4_PATCH_GROOVING
	//--->
	// grooving 20080624
	// 백색 마법사 출현 설정 변경
	// 로랜시아, 노리아, 데비아스 맵 중 두 군데만 랜덤하게 출현 하게 수정
	INT iSkipMap = 0;
	iSkipMap = rand() % 3;
	//<---

#endif

	for (INT i = 0 ; i < RINGEVENT_MAX_MAP ; i++ ) {

#ifdef SEASON4_PATCH_GROOVING
		//--->
		// grooving 20080624
		// 백색 마법사 출현 설정 변경
		if( iSkipMap == i )
			continue;
		//<---
#endif
		INT iCount = 1000;

		while (iCount--) {
			iRandX = rand()%(g_RingMapPosition[i][2]-g_RingMapPosition[i][0])+g_RingMapPosition[i][0];
			iRandY = rand()%(g_RingMapPosition[i][3]-g_RingMapPosition[i][1])+g_RingMapPosition[i][1];

			if (m_RingMonsterHerd[i].SetTotalInfo(g_RingEventMapNum[i], RINGEVENT_MIN_RADIUS, iRandX, iRandY)) {
				break;
			}
		}

		if (!iCount) {
			// 중심잡기 실패
		}
		else {
			if (m_vtMonsterAddData.empty()) {
				// 몬스터 데이터가 없다. - 에러
				LogAddTD("[Ring Event] - Error : No Monster Data Exist");
				continue;
			}
			else {
				LogAddTD("[Ring Event] - Monster Start Position MapNumber:%d, X:%d, Y:%d", g_RingEventMapNum[i], iRandX, iRandY);
			}
		}

		EnterCriticalSection (& m_critMonsterAddData);
		vector <RINGMONSTER_DATA>::iterator it = m_vtMonsterAddData.begin();

		for (;it!=m_vtMonsterAddData.end();it++) {
			RINGMONSTER_DATA & pRMD = *it;

			iCount = 100;
			while (iCount--) {
				if (m_RingMonsterHerd[i].AddMonster(pRMD.m_iType, pRMD.m_bDoRegen, pRMD.m_bDoAttackFirst))
					break;
			}
		}
		LeaveCriticalSection (& m_critMonsterAddData);

		m_RingMonsterHerd[i].Start();
	}
}

VOID CRingAttackEvent::StopEvent ()
{
	for (INT i = 0 ; i < RINGEVENT_MAX_MAP ; i++ )
		m_RingMonsterHerd[i].Stop();

	m_bMonsterToDest = FALSE;
}

VOID CRingAttackEvent::EnableEvent (BOOL bDoEvent)
{
	m_bDoEvent = bDoEvent;

	if (m_bDoEvent) {
		SetState (RINGEVENT_STATE_CLOSED);
	}
	else {
		SetState (RINGEVENT_STATE_NONE);
	}
}

BOOL CRingAttackEvent::Load (LPSTR lpszFileName)
{
	// *> . 스크립트를 읽어서 필요한 데이터를 저장한다.
	if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
	{
		MsgBox("[Ring Event] Info file Load Fail [%s]", lpszFileName);
		return FALSE;
	}

	EnterCriticalSection (& m_critMonsterAddData);
	m_vtMonsterAddData.erase(m_vtMonsterAddData.begin(), m_vtMonsterAddData.end());
	LeaveCriticalSection (& m_critMonsterAddData);

	m_vtEventTime.erase(m_vtEventTime.begin(), m_vtEventTime.end());

	m_bHasData = FALSE;

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
			{	// 반지이벤트의 기본 스케쥴정보
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iTIME_MIN_OPEN = (int)TokenNumber;		// 이벤트 시작전 몇분 전부터 광고하는가?
				Token = (*GetToken)(); m_iTIME_MIN_PLAY = (int)TokenNumber;				// 이벤트 진행시간
				Token = (*GetToken)(); m_iTIME_MONSTER_TO_DEST = (int)TokenNumber;		// 몬스터들이 종료 몇분 전에 목적지로 향할 것인가?
			}
			else if( type == 1 )
			{	// 반지이벤트의 무리의 움직임에 관한 정보
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iMOVE_RAND_SIZE = (int)TokenNumber;									// 한번 움직일 때의 최대 크기
				Token = (*GetToken)(); m_iRADIUS_MIN			= TokenNumber;			// 한번 움직일 때의 최소 반경
				Token = (*GetToken)(); m_iRADIUS_MAX			= TokenNumber;			// 한번 움직일 때의 최대 반경

			}
			else if( type == 2 )
			{	// 반지이벤트의 무리에 추가되는 몬스터들의 속성
				RINGMONSTER_DATA pRMD;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				pRMD.m_iType = (int)TokenNumber;										// 몬스터 타입번호			
				Token = (*GetToken)(); pRMD.m_bDoRegen			= (BOOL)TokenNumber;	// 몬스터의 리젠여부
				Token = (*GetToken)(); pRMD.m_bDoAttackFirst	= (BOOL)TokenNumber;	// 몬스터의 선공여부
				
				EnterCriticalSection (& m_critMonsterAddData);
				m_vtMonsterAddData.push_back(pRMD);
				LeaveCriticalSection (& m_critMonsterAddData);
			}
			else if( type == 3 )
			{	// 반지이벤트가 시작할 시간정보를 얻어서 저장
				RINGMONSTER_EVENT_TIME pRET;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				pRET.m_iHour = (int)TokenNumber;										// 이벤트 시작 시간
				Token = (*GetToken)(); pRET.m_iMinute			= (int)TokenNumber;		// 이벤트 시작 분
				
				m_vtEventTime.push_back(pRET);
			}
		}
	}
	fclose(SMDFile);

	LogAdd("%s file load!", lpszFileName);

	m_bHasData = TRUE;

	return TRUE;
}


VOID CRingAttackEvent::Run()
{

#ifdef EVENT_MANAGEMENT_20040711
	if (m_bMenualStart)
		return;
#endif

	if (m_bDoEvent) {
		switch (m_iEVENT_STATE) {
		case RINGEVENT_STATE_NONE :
			ProcState_None();
			break;
		case RINGEVENT_STATE_CLOSED :
			ProcState_Closed();
			break;
		case RINGEVENT_STATE_PLAYING :
			ProcState_Playing();
			break;
		default:
			break;
		}
	}
}


VOID CRingAttackEvent::CheckSync()
{

#if TESTSERVER == 1
	// 테스트용 루틴
	m_iTIME_MSEC_REMAIN = (m_iTIME_MIN_OPEN + 1) * 60 * 1000;
	m_iTIME_TICK_COUNT = GetTickCount();

	// 완전 임시 !!!!!
	m_iTIME_MSEC_REMAIN = 5000;
#else
	// 임의의 시간을 지정하는 루틴

	if (m_vtEventTime.empty()) {
		LogAddC(2, "[Ring Event] No Schedule Time Data");
		SetState(RINGEVENT_STATE_NONE);
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime (& sysTime);

	INT iMIN_HOUR = 24;
	INT iMIN_MINUTE = 60;
	BOOL bTIME_CHANGED = FALSE;

	vector <RINGMONSTER_EVENT_TIME>::iterator it = m_vtEventTime.begin();

	for (;it != m_vtEventTime.end();it++) {
		RINGMONSTER_EVENT_TIME & pRET = *it;
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
			RINGMONSTER_EVENT_TIME & pRET = *it;
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
		LogAddC(2, "[Ring Event] No Schedule Time Data");
		SetState(RINGEVENT_STATE_NONE);
		return;
	}
	
	m_iTIME_TICK_COUNT = GetTickCount();
	
	LogAddTD("[Ring Event] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)", m_iTIME_MSEC_REMAIN / (60 * 1000), iMIN_HOUR, iMIN_MINUTE);
	
/*	
	// 기존에 4시간 단위로 시간을 지정하는 루틴

	struct tm * today;
	time_t ltime;
	
	time( &ltime );
	today = localtime( &ltime );

	INT iTimeMod = today->tm_hour%4;

	m_iTIME_MSEC_REMAIN = (((60*4) * 60) - ((((60*iTimeMod) * 60) + today->tm_min * 60 + today->tm_sec))) * 1000;

	if( m_iTIME_MSEC_REMAIN < 0 )
	{
		m_iTIME_MSEC_REMAIN += (60 * 60) * 1000;
	}
	
	m_iTIME_TICK_COUNT = GetTickCount();

	LogAddTD("[Ring Event] Sync Start Time. [%d] min remain", m_iTIME_MSEC_REMAIN / (60 * 1000));
*/
#endif

	m_iTIME_NOTIFY_COUNT = m_iTIME_MIN_OPEN;

}


VOID CRingAttackEvent::SetState(INT iSTATE)
{
	if ((m_iEVENT_STATE < RINGEVENT_STATE_NONE) || (m_iEVENT_STATE > RINGEVENT_STATE_PLAYING))
		return;

	m_iEVENT_STATE = iSTATE;

	switch(m_iEVENT_STATE) {
	case RINGEVENT_STATE_NONE :
		SetState_None();
		break;
	case RINGEVENT_STATE_CLOSED :
		SetState_Closed();
		break;
	case RINGEVENT_STATE_PLAYING :
		SetState_Playing();
		break;
	default:
		break;
	}
}


VOID CRingAttackEvent::ProcState_None ()
{
	
}


VOID CRingAttackEvent::ProcState_Closed ()
{
	INT iTICK_MSEC = (GetTickCount() - m_iTIME_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		m_iTIME_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
		//LogAddTD("[Ring Event] - CLOSED Check Time. [%d] sec remain", m_iTIME_MSEC_REMAIN / 1000);
#endif
		if (m_bDoEvent) {
			// 주의> . 공지가 잘 않될 수 있다. !!
			if ((m_iTIME_MSEC_REMAIN <= m_iTIME_MIN_OPEN * 60 * 1000) && (m_iTIME_MSEC_REMAIN > 0) && (m_iTIME_MSEC_REMAIN / (60*1000) != m_iTIME_NOTIFY_COUNT )) {	
				m_iTIME_NOTIFY_COUNT = m_iTIME_MSEC_REMAIN / (60*1000);

#if TESTSERVER == 1
				CHAR szTemp[256];
				wsprintf(szTemp, lMsg.Get(1206), m_iTIME_NOTIFY_COUNT+1);	// "백색의 마법사 군단이 뮤 월드를 노리고 있습니다. (%d분전)"
				AllSendServerMsg (szTemp);
				
				PMSG_ANS_CL_EFFECT pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
				pMsg.wEffectNum = 0;			// 시작 n분 전의 사운드 효과를 출력하라
				DataSendAll ((LPBYTE)&pMsg, pMsg.h.size);
#else
				if (m_iTIME_NOTIFY_COUNT + 1 == 3) {	// 3분 전에 한 번 공지해준다.
					CHAR szTemp[256];
					wsprintf(szTemp, lMsg.Get(1207));	// "백색의 마법사 군단이 뮤 월드를 노리고 있습니다."
					AllSendServerMsg (szTemp);

					PMSG_ANS_CL_EFFECT pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
					pMsg.wEffectNum = 0;		// 시작 n분 전의 사운드 효과를 출력하라
					DataSendAll ((LPBYTE)&pMsg, pMsg.h.size);

					LogAddTD("[Ring Event] - Before 3 minutes - Advertise");
				}
#endif
			}
		}
	}
	
	if (m_iTIME_MSEC_REMAIN <= 0) {
		// 대기시간이 다 되었다. -> 이벤트 시작
		CHAR szTemp[256];
		wsprintf(szTemp, lMsg.Get(1208));		// "백색의 마법사 군단이 파괴의 오크들을 이끌고 뮤 월드를 침략하고 있습니다."
		AllSendServerMsg (szTemp);

		PMSG_ANS_CL_EFFECT pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
		pMsg.wEffectNum = 1;		// 시작 시의 사운드 효과를 출력하라
		DataSendAll ((LPBYTE)&pMsg, pMsg.h.size);

		if (m_bDoEvent)
			SetState (RINGEVENT_STATE_PLAYING);
		else
			SetState (RINGEVENT_STATE_NONE);

		LogAddTD("[Ring Event] - Event Started");
	}
}


VOID CRingAttackEvent::ProcState_Playing ()
{
//#if TESTSERVER == 1		// 30초 마다 군단의 위치를 출력하기 위함
	static INT	iTIME_SEC_NOTIFY = -1;
//#endif

	INT iTICK_MSEC = (GetTickCount() - m_iTIME_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		m_iTIME_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
		//LogAddTD("[Ring Event] - PLAYING Check Time. [%d] sec remain", m_iTIME_MSEC_REMAIN / 1000);
#endif

		if (rand()%10 == 0) {	// 가끔 한번씩 움직여 준다.
			Move ();
		}
	}

//#if TESTSERVER == 1		// 30초 마다 군단의 위치를 출력하기 위함
	if ((m_iTIME_MSEC_REMAIN/1000) % 30 == 0 &&
		(m_iTIME_MSEC_REMAIN/1000) / 30 != iTIME_SEC_NOTIFY
		)
	{
		iTIME_SEC_NOTIFY = (m_iTIME_MSEC_REMAIN/1000) / 30;
		
		for (INT i = 0 ; i < RINGEVENT_MAX_MAP ; i++ ) {
			BOOL bLive = FALSE;
			
			for( int n=0; n<ALLOC_USEROBJECTSTART; n++) {
				if (gObj[n].Class == 135) {				// 백색 마법사 중에
					if (gObj[n].m_bIsInMonsterHerd) {	// 무리에 속해있고
						if (gObj[n].Live &&				// 살아있는 자가 있는지 체크
							gObj[n].MapNumber == g_RingEventMapNum[i]
							) 
						{				
							bLive = TRUE;
						}
					}
				}
			}

			LogAddTD("[Ring Event] - per 30'S Inform MapNumber:%d, X:%d, Y:%d, Boss Live:%d",
				g_RingEventMapNum[i],
				m_RingMonsterHerd[i].GetX(),
				m_RingMonsterHerd[i].GetY(),
				bLive
				);

#if TESTSERVER == 1			// 30초 마다 군단의 위치를 출력하기 위함
	#ifndef FOR_ONAIR
			PMSG_NOTICE	pNotice;

			#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 1, "군단목적지[%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), m_RingMonsterHerd[i].GetX(), m_RingMonsterHerd[i].GetY());
//				TNotice::SendNoticeToAllUser( &pNotice );
			#else
				pNotice.type = 1;
				wsprintf(pNotice.Notice, "군단목적지[%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), m_RingMonsterHerd[i].GetX(), m_RingMonsterHerd[i].GetY());
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));

				for( int m=ALLOC_USEROBJECTSTART; m<MAX_OBJECT; m++)
				{
					if( gObj[m].Connected == 3 && (gObj[m].Type == OBJTYPE_CHARACTER) )
					{
//						DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
					}
				}
			#endif
				
	#endif
#endif
		}
	}
//#endif


	if (m_iTIME_MSEC_REMAIN <= m_iTIME_MONSTER_TO_DEST * 60 * 1000 &&		// n 분 남았다면 복적지로 향한다.
		m_bMonsterToDest == FALSE
		)
	{
		for (INT i = 0 ; i < RINGEVENT_MAX_MAP ; i++ ) {

			UCHAR iRandX, iRandY;

			UCHAR ucRAND = rand()%4;

			iRandX = g_RingMapDestPosition[i][ucRAND*2];
			iRandY = g_RingMapDestPosition[i][ucRAND*2+1];
			
			if (m_RingMonsterHerd[i].MoveHerd(iRandX, iRandY)) {
#if TESTSERVER == 1
	#ifndef FOR_ONAIR				
				PMSG_NOTICE	pNotice;

			#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 1, "군단이동 [%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), iRandX, iRandY);
//				TNotice::SendNoticeToAllUser( &pNotice );
			#else
				pNotice.type = 1;
				wsprintf(pNotice.Notice, "군단이동 [%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), iRandX, iRandY);
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));

				for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
				{
					if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
					{
//						DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
					}
				}
			#endif
	#endif
#endif

				LogAddTD("[Ring Event] - Monster Herd Move MapNumber:%d, X:%d, Y:%d",
					g_RingEventMapNum[i],
					iRandX,
					iRandY
					);
			}
		}

		m_bMonsterToDest = TRUE;
	}
	
	if (m_iTIME_MSEC_REMAIN <= 0) {
		// 이벤트가 종료되었다. -> 공지
		CHAR szTemp[256];
		wsprintf(szTemp, lMsg.Get(1209));		// "백색 마법사 군단이 사라졌습니다."
		AllSendServerMsg (szTemp);
	
		PMSG_ANS_CL_EFFECT pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
		pMsg.wEffectNum = 2;		// 종료 시의 사운드 효과를 출력하라
		DataSendAll ((LPBYTE)&pMsg, pMsg.h.size);

		if (m_bDoEvent)
			SetState (RINGEVENT_STATE_CLOSED);
		else
			SetState (RINGEVENT_STATE_NONE);

		LogAddTD("[Ring Event] - Event Ended");
	}
}


VOID CRingAttackEvent::SetState_None ()
{
	StopEvent();
}


VOID CRingAttackEvent::SetState_Closed ()
{
	StopEvent();

	if (m_bDoEvent)
		CheckSync();		// 여기서 남은 시간을 계산하여 정한다.
	else
		SetState (RINGEVENT_STATE_NONE);
}


VOID CRingAttackEvent::SetState_Playing ()
{
	m_iTIME_MSEC_REMAIN = m_iTIME_MIN_PLAY * 60 * 1000;			// RINGEVENT_STATE_PLAYING -> RINGEVENT_STATE_PLAYEND 까지의 남은 시간

	StartEvent();			// 이벤트 몬스터 등장
}


VOID CRingAttackEvent::Move ()
{
	if (m_bDoEvent) {
		if (m_bMonsterToDest)		// 몬스터들이 목적지로 향했다면 더 이상 목적지 이동은 없다.
			return;

		UCHAR iRandX, iRandY;
		UCHAR iCX, iCY;

		for (INT i = 0 ; i < RINGEVENT_MAX_MAP ; i++ ) {
			INT iCount = 1000;

			if (!m_RingMonsterHerd[i].GetCurrentLocation(iCX, iCY))
				break;

			while (iCount--) {
				iRandX = rand()%m_iMOVE_RAND_SIZE * (-1*(rand()%3 -1)) + iCX;
				iRandY = rand()%m_iMOVE_RAND_SIZE * (-1*(rand()%3 -1)) + iCY;

				if (iRandX < g_RingMapPosition[i][0] || iRandX > g_RingMapPosition[i][2])
					continue;
				if (iRandY < g_RingMapPosition[i][1] || iRandY > g_RingMapPosition[i][3])
					continue;

				if (m_RingMonsterHerd[i].CheckLocation(iRandX, iRandY))
					break;
			}

			if (m_RingMonsterHerd[i].MoveHerd(iRandX, iRandY)) {
#if TESTSERVER == 1
	#ifndef FOR_ONAIR				
				PMSG_NOTICE	pNotice;

			#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 1, "군단이동 [%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), iRandX, iRandY);
//				TNotice::SendNoticeToAllUser( &pNotice );
			#else
				pNotice.type = 1;
				wsprintf(pNotice.Notice, "군단이동 [%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), iRandX, iRandY);
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));

				for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
				{
					if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
					{
//						DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
					}
				}
			#endif
	#endif
#endif
				LogAddTD("[Ring Event] - Monster Herd Move MapNumber:%d, X:%d, Y:%d",
					g_RingEventMapNum[i],
					iRandX,
					iRandY
					);
			}

			INT iDIVIDE = m_iRADIUS_MAX - m_iRADIUS_MIN;
			if (iDIVIDE <= 0)
				iDIVIDE = 1;
			INT iRADIUS = iDIVIDE + m_iRADIUS_MIN;
			if (iRADIUS <= RINGEVENT_MIN_RADIUS)
				iRADIUS = RINGEVENT_MIN_RADIUS;

			m_RingMonsterHerd[i].SetRadius(rand()%(m_iRADIUS_MAX - m_iRADIUS_MIN) + m_iRADIUS_MIN);
		}
	}
}

#ifdef EVENT_MANAGEMENT_20040711


void CRingAttackEvent::Start_Menual ()
{
	SetMenualStart(TRUE);

	StopEvent ();

	LogAddTD("○●[Event Management] [Start] RingAttack Event!");
	
	m_iTIME_MIN_PLAY	= 30;	// 이벤트 진행 시간

	CHAR szTemp[256];
	wsprintf(szTemp, lMsg.Get(1208));		// "백색의 마법사 군단이 파괴의 오크들을 이끌고 뮤 월드를 침략하고 있습니다."
	AllSendServerMsg (szTemp);

	PMSG_ANS_CL_EFFECT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
	pMsg.wEffectNum = 1;		// 시작 시의 사운드 효과를 출력하라
	DataSendAll ((LPBYTE)&pMsg, pMsg.h.size);

	SetState (RINGEVENT_STATE_PLAYING);

	
}


void CRingAttackEvent::End_Menual ()
{
	SetMenualStart(FALSE);
}


#endif


#endif



