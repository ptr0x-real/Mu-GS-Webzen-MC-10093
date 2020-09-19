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
	Ŭ���� ���� : MonsterHerd Ŭ������ ��ӹ��� ������ ���� 
		���� ���� Ŭ����
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
	if (lpObj->Class == 135) {		// ����� �������� ������ �༮
		int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		int iType = ItemGetNumberMake(14, 13);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE) lpObj->X, (BYTE) lpObj->Y, iType, 0, (BYTE)0, 0, 0, 0, iMaxHitUser);

		// �̺�Ʈ�� �����ٴ� ������ �Ѵ�.
		CHAR szTemp[256];
		wsprintf(szTemp, lMsg.Get(1205), gObj[iMaxHitUser].Name, g_TerrainManager.GetMapName(lpObj->MapNumber));	// "%s���� %s�� ħ���� ����� ������ ������ �����ƽ��ϴ�."
		AllSendServerMsg (szTemp);

		LogAddTD("[Ring Event] White Wizard Killed by [%s][%s], MapNumber:%d", gObj[iMaxHitUser].AccountID, gObj[iMaxHitUser].Name, gObj[iMaxHitUser].MapNumber);
		return TRUE;
	}
	else if (
		lpObj->Class == 136 ||
		lpObj->Class == 137			// ��ũ���̶�� 30% Ȯ���� ������ ����
		) 
	{
		if (rand()%100 < 30) {
			int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			int iType = ItemGetNumberMake(13, 20);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE) lpObj->X, (BYTE) lpObj->Y, iType, 0, (BYTE)30, 0, 0, 0, iMaxHitUser);
			return TRUE;
		}
		else if (rand()%g_iRingOrcKillGiftRate == 0) {			//  1/n Ȯ���� ��÷�� �ȴٸ�
			int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

			SendEventGiftWinner (iMaxHitUser, 1);				// ��ũ�� ���� ��ǰ�� ź��.
			return TRUE;
		}
		else {
			// �������� ���� 10000�� ������.
			MapC[lpObj->MapNumber].MoneyItemDrop((int)10000, (BYTE) lpObj->X ,(BYTE) lpObj->Y);
			return TRUE;
		}
	}

	return FALSE;
}


VOID CRingMonsterHerd::MonsterAttackAction(	// ���� ���� �� Ư�� ���Ͱ� �������� ���� ó��
	LPOBJECTSTRUCT lpObj,					// ��
	LPOBJECTSTRUCT lpTargetObj				// ���� ������ �ΰ�?
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �������¿����� ������ �� �� ����
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
		OrderAttack(lpObj, lpTargetObj, 90);	// ����� �����簡 ���� �������� ������ ���
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
	Ŭ���� ���� : MonsterHerd Ŭ������ ��ӹ��� ������ ���� 
		���� ���� Ŭ����
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
	// ��� ������ ���� ���� ����
	// �η��þ�, �븮��, ����ƽ� �� �� �� ������ �����ϰ� ���� �ϰ� ����
	INT iSkipMap = 0;
	iSkipMap = rand() % 3;
	//<---

#endif

	for (INT i = 0 ; i < RINGEVENT_MAX_MAP ; i++ ) {

#ifdef SEASON4_PATCH_GROOVING
		//--->
		// grooving 20080624
		// ��� ������ ���� ���� ����
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
			// �߽���� ����
		}
		else {
			if (m_vtMonsterAddData.empty()) {
				// ���� �����Ͱ� ����. - ����
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
	// *> . ��ũ��Ʈ�� �о �ʿ��� �����͸� �����Ѵ�.
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
			{	// �����̺�Ʈ�� �⺻ ����������
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iTIME_MIN_OPEN = (int)TokenNumber;		// �̺�Ʈ ������ ��� ������ �����ϴ°�?
				Token = (*GetToken)(); m_iTIME_MIN_PLAY = (int)TokenNumber;				// �̺�Ʈ ����ð�
				Token = (*GetToken)(); m_iTIME_MONSTER_TO_DEST = (int)TokenNumber;		// ���͵��� ���� ��� ���� �������� ���� ���ΰ�?
			}
			else if( type == 1 )
			{	// �����̺�Ʈ�� ������ �����ӿ� ���� ����
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iMOVE_RAND_SIZE = (int)TokenNumber;									// �ѹ� ������ ���� �ִ� ũ��
				Token = (*GetToken)(); m_iRADIUS_MIN			= TokenNumber;			// �ѹ� ������ ���� �ּ� �ݰ�
				Token = (*GetToken)(); m_iRADIUS_MAX			= TokenNumber;			// �ѹ� ������ ���� �ִ� �ݰ�

			}
			else if( type == 2 )
			{	// �����̺�Ʈ�� ������ �߰��Ǵ� ���͵��� �Ӽ�
				RINGMONSTER_DATA pRMD;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				pRMD.m_iType = (int)TokenNumber;										// ���� Ÿ�Թ�ȣ			
				Token = (*GetToken)(); pRMD.m_bDoRegen			= (BOOL)TokenNumber;	// ������ ��������
				Token = (*GetToken)(); pRMD.m_bDoAttackFirst	= (BOOL)TokenNumber;	// ������ ��������
				
				EnterCriticalSection (& m_critMonsterAddData);
				m_vtMonsterAddData.push_back(pRMD);
				LeaveCriticalSection (& m_critMonsterAddData);
			}
			else if( type == 3 )
			{	// �����̺�Ʈ�� ������ �ð������� �� ����
				RINGMONSTER_EVENT_TIME pRET;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				pRET.m_iHour = (int)TokenNumber;										// �̺�Ʈ ���� �ð�
				Token = (*GetToken)(); pRET.m_iMinute			= (int)TokenNumber;		// �̺�Ʈ ���� ��
				
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
	// �׽�Ʈ�� ��ƾ
	m_iTIME_MSEC_REMAIN = (m_iTIME_MIN_OPEN + 1) * 60 * 1000;
	m_iTIME_TICK_COUNT = GetTickCount();

	// ���� �ӽ� !!!!!
	m_iTIME_MSEC_REMAIN = 5000;
#else
	// ������ �ð��� �����ϴ� ��ƾ

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
	case TRUE :			// ���� �ð����� ������ ����� �ð��� ũ��.
		m_iTIME_MSEC_REMAIN = ( ((60*iMIN_HOUR) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) * 1000;
		break;
	case TRUE + 1 :		// ���� �ð����� ������ ����� �ð��� �۴� (���� �� ����)
		m_iTIME_MSEC_REMAIN = ( ((60*(iMIN_HOUR+24)) * 60 + iMIN_MINUTE*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond))) * 1000;
		break;
	default:			// ����
		LogAddC(2, "[Ring Event] No Schedule Time Data");
		SetState(RINGEVENT_STATE_NONE);
		return;
	}
	
	m_iTIME_TICK_COUNT = GetTickCount();
	
	LogAddTD("[Ring Event] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)", m_iTIME_MSEC_REMAIN / (60 * 1000), iMIN_HOUR, iMIN_MINUTE);
	
/*	
	// ������ 4�ð� ������ �ð��� �����ϴ� ��ƾ

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
			// ����> . ������ �� �ʵ� �� �ִ�. !!
			if ((m_iTIME_MSEC_REMAIN <= m_iTIME_MIN_OPEN * 60 * 1000) && (m_iTIME_MSEC_REMAIN > 0) && (m_iTIME_MSEC_REMAIN / (60*1000) != m_iTIME_NOTIFY_COUNT )) {	
				m_iTIME_NOTIFY_COUNT = m_iTIME_MSEC_REMAIN / (60*1000);

#if TESTSERVER == 1
				CHAR szTemp[256];
				wsprintf(szTemp, lMsg.Get(1206), m_iTIME_NOTIFY_COUNT+1);	// "����� ������ ������ �� ���带 �븮�� �ֽ��ϴ�. (%d����)"
				AllSendServerMsg (szTemp);
				
				PMSG_ANS_CL_EFFECT pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
				pMsg.wEffectNum = 0;			// ���� n�� ���� ���� ȿ���� ����϶�
				DataSendAll ((LPBYTE)&pMsg, pMsg.h.size);
#else
				if (m_iTIME_NOTIFY_COUNT + 1 == 3) {	// 3�� ���� �� �� �������ش�.
					CHAR szTemp[256];
					wsprintf(szTemp, lMsg.Get(1207));	// "����� ������ ������ �� ���带 �븮�� �ֽ��ϴ�."
					AllSendServerMsg (szTemp);

					PMSG_ANS_CL_EFFECT pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
					pMsg.wEffectNum = 0;		// ���� n�� ���� ���� ȿ���� ����϶�
					DataSendAll ((LPBYTE)&pMsg, pMsg.h.size);

					LogAddTD("[Ring Event] - Before 3 minutes - Advertise");
				}
#endif
			}
		}
	}
	
	if (m_iTIME_MSEC_REMAIN <= 0) {
		// ���ð��� �� �Ǿ���. -> �̺�Ʈ ����
		CHAR szTemp[256];
		wsprintf(szTemp, lMsg.Get(1208));		// "����� ������ ������ �ı��� ��ũ���� �̲��� �� ���带 ħ���ϰ� �ֽ��ϴ�."
		AllSendServerMsg (szTemp);

		PMSG_ANS_CL_EFFECT pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
		pMsg.wEffectNum = 1;		// ���� ���� ���� ȿ���� ����϶�
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
//#if TESTSERVER == 1		// 30�� ���� ������ ��ġ�� ����ϱ� ����
	static INT	iTIME_SEC_NOTIFY = -1;
//#endif

	INT iTICK_MSEC = (GetTickCount() - m_iTIME_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		m_iTIME_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
		//LogAddTD("[Ring Event] - PLAYING Check Time. [%d] sec remain", m_iTIME_MSEC_REMAIN / 1000);
#endif

		if (rand()%10 == 0) {	// ���� �ѹ��� ������ �ش�.
			Move ();
		}
	}

//#if TESTSERVER == 1		// 30�� ���� ������ ��ġ�� ����ϱ� ����
	if ((m_iTIME_MSEC_REMAIN/1000) % 30 == 0 &&
		(m_iTIME_MSEC_REMAIN/1000) / 30 != iTIME_SEC_NOTIFY
		)
	{
		iTIME_SEC_NOTIFY = (m_iTIME_MSEC_REMAIN/1000) / 30;
		
		for (INT i = 0 ; i < RINGEVENT_MAX_MAP ; i++ ) {
			BOOL bLive = FALSE;
			
			for( int n=0; n<ALLOC_USEROBJECTSTART; n++) {
				if (gObj[n].Class == 135) {				// ��� ������ �߿�
					if (gObj[n].m_bIsInMonsterHerd) {	// ������ �����ְ�
						if (gObj[n].Live &&				// ����ִ� �ڰ� �ִ��� üũ
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

#if TESTSERVER == 1			// 30�� ���� ������ ��ġ�� ����ϱ� ����
	#ifndef FOR_ONAIR
			PMSG_NOTICE	pNotice;

			#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 1, "���ܸ�����[%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), m_RingMonsterHerd[i].GetX(), m_RingMonsterHerd[i].GetY());
//				TNotice::SendNoticeToAllUser( &pNotice );
			#else
				pNotice.type = 1;
				wsprintf(pNotice.Notice, "���ܸ�����[%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), m_RingMonsterHerd[i].GetX(), m_RingMonsterHerd[i].GetY());
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


	if (m_iTIME_MSEC_REMAIN <= m_iTIME_MONSTER_TO_DEST * 60 * 1000 &&		// n �� ���Ҵٸ� �������� ���Ѵ�.
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
				TNotice::MakeNoticeMsgEx( &pNotice, 1, "�����̵� [%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), iRandX, iRandY);
//				TNotice::SendNoticeToAllUser( &pNotice );
			#else
				pNotice.type = 1;
				wsprintf(pNotice.Notice, "�����̵� [%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), iRandX, iRandY);
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
		// �̺�Ʈ�� ����Ǿ���. -> ����
		CHAR szTemp[256];
		wsprintf(szTemp, lMsg.Get(1209));		// "��� ������ ������ ��������ϴ�."
		AllSendServerMsg (szTemp);
	
		PMSG_ANS_CL_EFFECT pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
		pMsg.wEffectNum = 2;		// ���� ���� ���� ȿ���� ����϶�
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
		CheckSync();		// ���⼭ ���� �ð��� ����Ͽ� ���Ѵ�.
	else
		SetState (RINGEVENT_STATE_NONE);
}


VOID CRingAttackEvent::SetState_Playing ()
{
	m_iTIME_MSEC_REMAIN = m_iTIME_MIN_PLAY * 60 * 1000;			// RINGEVENT_STATE_PLAYING -> RINGEVENT_STATE_PLAYEND ������ ���� �ð�

	StartEvent();			// �̺�Ʈ ���� ����
}


VOID CRingAttackEvent::Move ()
{
	if (m_bDoEvent) {
		if (m_bMonsterToDest)		// ���͵��� �������� ���ߴٸ� �� �̻� ������ �̵��� ����.
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
				TNotice::MakeNoticeMsgEx( &pNotice, 1, "�����̵� [%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), iRandX, iRandY);
//				TNotice::SendNoticeToAllUser( &pNotice );
			#else
				pNotice.type = 1;
				wsprintf(pNotice.Notice, "�����̵� [%s] X:%d, Y:%d", g_TerrainManager.GetMapName(g_RingEventMapNum[i]), iRandX, iRandY);
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

	LogAddTD("�ۡ�[Event Management] [Start] RingAttack Event!");
	
	m_iTIME_MIN_PLAY	= 30;	// �̺�Ʈ ���� �ð�

	CHAR szTemp[256];
	wsprintf(szTemp, lMsg.Get(1208));		// "����� ������ ������ �ı��� ��ũ���� �̲��� �� ���带 ħ���ϰ� �ֽ��ϴ�."
	AllSendServerMsg (szTemp);

	PMSG_ANS_CL_EFFECT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
	pMsg.wEffectNum = 1;		// ���� ���� ���� ȿ���� ����϶�
	DataSendAll ((LPBYTE)&pMsg, pMsg.h.size);

	SetState (RINGEVENT_STATE_PLAYING);

	
}


void CRingAttackEvent::End_Menual ()
{
	SetMenualStart(FALSE);
}


#endif


#endif



