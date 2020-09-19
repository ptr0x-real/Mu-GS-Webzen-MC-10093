// XMasAttackEvent.cpp: implementation of the CXMasAttackEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMasAttackEvent.h"


#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
#include "DSProtocol.h"

#include <time.h>
#include "Include\ReadScript.h"
#include "Include\Prodef.h"
#include "GameMain.h"
#include "Protocol.h"
#include "Include\cb_protocol.h"
#include "Event.h"
#include "Common\Winutil.h"


#define ERROR_RETURN_INT(t, err_code, func) \
if(!(t)) \
{ \
	func; \
	return err_code; \
}

#define ERROR_RETURN(t, func) \
if(!(t)) \
{ \
	func; \
	return; \
}

#define ERROR_RETURN_INT_LOG(t, err_code, func) \
if(!(t)) \
{ \
	func; \
	LogAddTD("%s\t%s\t%s\t%s\t%d", #t, #err_code, #func, __FILE__, __LINE__); \
	return err_code; \
}

#define ERROR_RETURN_LOG(t, func) \
if(!(t)) \
{ \
	func; \
	LogAddTD("%s\t%s\t%s\t%s\t%d", #t, "", #func, __FILE__, __LINE__); \
	return; \
} 


CXMasAttackEvent		g_XMasAttackEvent;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL CXMasMonsterHerd::Start()
{
	return MonsterHerd::Start();
}

BOOL CXMasMonsterHerd::MonsterHerdItemDrop(LPOBJECTSTRUCT lpObj)
{
	ERROR_RETURN_INT((lpObj->Class == MONSTER_GRADE_BOSS || lpObj->Class == MONSTER_GRADE_NORMAL), FALSE, NULL);
	
	if (lpObj->Class == MONSTER_GRADE_BOSS)
	{
		int nAffectedCount = 0;
		int nRand = rand() % 3;
		switch (nRand)
		{
		case 0:
			{
				int nIncleaseAttackPower = 0;
				int nIncleaseDefensePower = 0;
				if( lpObj->Level <= 180 )
				{
					nIncleaseAttackPower = ( lpObj->Level / 3 ) + 45;
					nIncleaseDefensePower = ( lpObj->Level / 5 ) + 50;
				}
				else
				{
					nIncleaseAttackPower = 105;
					nIncleaseDefensePower = 86;
				}
				// ũ�����������ູ(���ݷ� + ���� ���)
				gObjAddBuffEffectWideArea( lpObj, XMASRANGE_BUFF, nAffectedCount
					, BUFFTYPE_CHRISTMAS_BLESS, EFFECTTYPE_IMPROVE_DAMAGE, nIncleaseAttackPower, EFFECTTYPE_IMPROVE_DEFENSE, nIncleaseDefensePower, 1800);
			}
			break;
		case 1:
			{
				// ��������
				//gObjAddBuffEffectWideArea( lpObj, XMASRANGE_BUFF, nAffectedCount
				//	, BUFFTYPE_SANTA_HEAL, EFFECTTYPE_MAXLIFE_UP, 500, EFFECTTYPE_NONE, 0, 1800);

				// ��Ÿ��ġ��
				gObjAddBuffEffectWideArea( lpObj, XMASRANGE_BUFF, nAffectedCount
					, BUFFTYPE_SANTA_HEAL, EFFECTTYPE_HP, 500, EFFECTTYPE_NONE, 0, 1800);
			}
			break;
		case 2:
			{
				// ��������
				//gObjAddBuffEffectWideArea( lpObj, XMASRANGE_BUFF, nAffectedCount
				//	, BUFFTYPE_SANTA_PROTECT, EFFECTTYPE_MAXMANA_UP, 500, EFFECTTYPE_NONE, 0, 1800);

				// ��Ÿ�Ǻ�ȣ
				gObjAddBuffEffectWideArea( lpObj, XMASRANGE_BUFF, nAffectedCount
					, BUFFTYPE_SANTA_PROTECT, EFFECTTYPE_MANA, 500, EFFECTTYPE_NONE, 0, 1800);
			}
			break;
		}

		LogAddTD("[XMasAttackEvent] White Wizard Killed, MapNumber:%d", lpObj->MapNumber);

		return TRUE;
	} 
	else if (lpObj->Class == MONSTER_GRADE_NORMAL)
	{
		if (rand()%100 < g_nXMasDropRate) 
		{
			int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			int iType = ItemGetNumberMake(13, 66);	// ��Ÿ�� ���� �ʴ���
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, iType, 0, (BYTE)10, 0, 0, 0, iMaxHitUser);

			return TRUE;
		}
		else 
		{
			// �������� ���� 10000�� ������.
			MapC[lpObj->MapNumber].MoneyItemDrop(g_nXMasDropZen, (BYTE)lpObj->X ,(BYTE)lpObj->Y);
			
			return TRUE;
		}
	}

	return FALSE;
}

#if 0
VOID CXMasMonsterHerd::MonsterAttackAction(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	ERROR_RETURN(lpObj, NULL);
	ERROR_RETURN(lpTargetObj, NULL);
	ERROR_RETURN((lpObj->Connected == CSS_GAMEPLAYING && lpObj->Type == OBJTYPE_MONSTER), NULL);
	ERROR_RETURN(lpTargetObj->Connected == CSS_GAMEPLAYING, NULL);

	if (lpObj->Class == MONSTER_GRADE_BOSS) 
	{
		// ��Ÿ ũ�ν� �����簡 ���� �������� ������ ���
		OrderAttack(lpObj, lpTargetObj, 1);
	}
}
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMasAttackEvent::CXMasAttackEvent()
:	CBaseAttackEvent()
,	m_nEventState(ATTACKEVENT_STATE_NONE)
,	m_nMoveRandSize(ATTACKEVENT_MAX_MOVE), m_nRadiusMin(ATTACKEVENT_MIN_RADIUS), m_nRadiusMax(ATTACKEVENT_MAX_RADIUS)

,	m_nOpenMsec(0), m_nPlayMsec(0), m_nToDestMsec(0)
,	m_nNotifyCount(0)
,	m_bExecuteDirect(FALSE)
{

}

CXMasAttackEvent::~CXMasAttackEvent()
{
	_DeleteMonsterHerd();
}

BOOL CXMasAttackEvent::Load(LPSTR lpszFileName)
{
	if((SMDFile=fopen(lpszFileName, "r")) == NULL)
	{
		MsgBox("[XMasAttackEvent] Info file Load Fail [%s]", lpszFileName);
		return FALSE;
	}
	
	Initialize();
	
	
	SMDToken Token;
	
	int type = -1;
	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{
			if( type == TOKEN_EVENT_SCHEDULE )
			{
				// �̺�Ʈ�� �⺻ ����������
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_nOpenMsec = ((int)TokenNumber * TIME_MINUTE_1);		// �̺�Ʈ ������ ��� ������ �����ϴ°�?
				Token = (*GetToken)(); 
				m_nPlayMsec = ((int)TokenNumber * TIME_MINUTE_1);		// �̺�Ʈ ����ð�
				Token = (*GetToken)(); 
				m_nToDestMsec = ((int)TokenNumber * TIME_MINUTE_1);		// ���͵��� ���� ��� ���� �������� ���� ���ΰ�?
			}
			else if( type == TOKEN_EVENT_MONSTERRAND )
			{
				// �̺�Ʈ�� ������ �����ӿ� ���� ����
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;
				
				m_nMoveRandSize = (int)TokenNumber;						// �ѹ� ������ ���� �ִ� ũ��
				Token = (*GetToken)(); 
				m_nRadiusMin = TokenNumber;								// �ѹ� ������ ���� �ּ� �ݰ�
				Token = (*GetToken)();
				m_nRadiusMax = TokenNumber;								// �ѹ� ������ ���� �ִ� �ݰ�
				
			}
			else if( type == TOKEN_EVENT_MONSTER )
			{
				// �̺�Ʈ�� ������ �߰��Ǵ� ���͵��� �Ӽ�
				MONSTER_DATA pRMD;
				
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;
				
				pRMD.m_iType = (int)TokenNumber;						// ���� Ÿ�Թ�ȣ			
				Token = (*GetToken)(); 
				pRMD.m_bDoRegen = (BOOL)TokenNumber;					// ������ ��������
				Token = (*GetToken)(); 
				pRMD.m_bDoAttackFirst = (BOOL)TokenNumber;				// ������ ��������
				
				EnterCriticalSection(&m_critMonsterAddData);
				m_MonsterAddData.push_back(pRMD);
				LeaveCriticalSection(&m_critMonsterAddData);
			}
			else if( type == TOKEN_EVENT_STARTTIME )
			{
				// �̺�Ʈ�� ������ �ð�����
				ATTACKEVENT_TIME pRET;
				
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;
				
				pRET.m_iHour = (int)TokenNumber;						// �̺�Ʈ ���� �ð�
				Token = (*GetToken)(); 
				pRET.m_iMinute = (int)TokenNumber;						// �̺�Ʈ ���� ��

				m_EventTime.push_back(pRET);
			}
			else if( type == TOKEN_EVENT_MAPPOSITION )
			{
				// ���͵��� �����ϰ� ������ ��ǥ
				MAPPOSITION_DATA mp;

				Token = (*GetToken)();
				if (strcmp("end", TokenString)==NULL) break;
				mp.byMapNumber = (BYTE)TokenNumber;
				Token = (*GetToken)();	mp.nMapPositionMinX		= (int)TokenNumber;
				Token = (*GetToken)();	mp.nMapPositionMinY		= (int)TokenNumber;
				Token = (*GetToken)();	mp.nMapPositionMaxX		= (int)TokenNumber;
				Token = (*GetToken)();	mp.nMapPositionMaxY		= (int)TokenNumber;
				
				m_MapPosition.push_back(mp);
			}
		}
	}
	fclose(SMDFile);
	
	LogAdd("[XMasAttackEvent] - %s file load!", lpszFileName);
	
	m_bLoaded = TRUE;
	
	return TRUE;
}

VOID CXMasAttackEvent::StartEvent()
{
	ERROR_RETURN(m_bDoEvent, NULL);
	ERROR_RETURN(m_bLoaded, NULL);
	ERROR_RETURN(!m_MonsterAddData.empty(), NULL);

	_DeleteMonsterHerd();
	_InitMonsterHerd();

	for (int i = 0; i < m_MapPosition.size(); ++i)
	{
		MAPPOSITION_DATA mp = m_MapPosition[i];
		CXMasMonsterHerd* pMonsterHerd = static_cast<CXMasMonsterHerd*>(m_MonsterHerd[i]);

		int iCount = 1000;
		int iRandX = 0, iRandY = 0;
		while (iCount--) {
			iRandX = rand() % (mp.nMapPositionMaxX - mp.nMapPositionMinX) + mp.nMapPositionMinX;
			iRandY = rand() % (mp.nMapPositionMaxY - mp.nMapPositionMinX) + mp.nMapPositionMinY;

			if (pMonsterHerd->SetTotalInfo(mp.byMapNumber, ATTACKEVENT_MIN_RADIUS, iRandX, iRandY) == TRUE) {
				break;
			}
		}

		if (!iCount) {
			// �߽���� ����
		}
		else 
		{
			if (m_MonsterAddData.empty()) {
				LogAddTD("[XMasAttackEvent] - Error : No Monster Data Exist");
				continue;
			}
			else {
				LogAddTD("[XMasAttackEvent] - Monster Start Position MapNumber:%d, X:%d, Y:%d", mp.byMapNumber, iRandX, iRandY);
			}
		}

		// ���������� �����Ѵ�.
		_AddMonsterHerd(pMonsterHerd);
		pMonsterHerd->Start();
	}
}


VOID CXMasAttackEvent::StopEvent()
{
	_IterMonsterHerd iter;
	for (iter = m_MonsterHerd.begin(); iter != m_MonsterHerd.end(); ++iter)
	{
		CXMasMonsterHerd* pMonsterHerd = static_cast<CXMasMonsterHerd*>(*iter);
		pMonsterHerd->Stop();
	}

	m_bMonsterToDest = FALSE;
}

VOID CXMasAttackEvent::EnableEvent(BOOL bDoEvent)
{
	m_bDoEvent = bDoEvent;
	
	if (m_bDoEvent) {
		SetState(ATTACKEVENT_STATE_CLOSED);
	}
	else {
		SetState(ATTACKEVENT_STATE_NONE);
	}
}



VOID CXMasAttackEvent::SetState(int nSate)
{
	ERROR_RETURN_LOG((nSate >= ATTACKEVENT_STATE_NONE) || (nSate <= ATTACKEVENT_STATE_PLAYING), NULL);
	ERROR_RETURN_LOG((m_nEventState >= ATTACKEVENT_STATE_NONE) || (m_nEventState <= ATTACKEVENT_STATE_PLAYING), NULL);
	
	m_nEventState = nSate;
	switch(m_nEventState) 
	{
	case ATTACKEVENT_STATE_NONE :
		SetState_None();
		break;
	case ATTACKEVENT_STATE_CLOSED :
		SetState_Closed();
		break;
	case ATTACKEVENT_STATE_PLAYING :
		SetState_Playing();
		break;
	default:
		break;
	}
}


VOID CXMasAttackEvent::ProcState_None()
{

}

VOID CXMasAttackEvent::ProcState_Closed()
{
	ERROR_RETURN_LOG(m_bDoEvent, SetState(ATTACKEVENT_STATE_NONE));

	int nRemainMsec = m_RemainedTimer.GetMilliSecond();
	if ( (nRemainMsec >= 1000) && (nRemainMsec / TIME_MINUTE_1 != m_nNotifyCount))
	{
		m_nNotifyCount = nRemainMsec / TIME_MINUTE_1;

#if TESTSERVER == 1
		//DEBUG
		_SendDebugMsg("XMasAttackEvent Before %d minutes", m_nNotifyCount + 1);
#endif

		// n�� ���� ���� ���
		if (m_nNotifyCount + 1 == m_nOpenMsec / TIME_MINUTE_1)
		{
			// ��Ÿ�� �븮�� ���� ������ �� ���� ���� ħ���Ͽ����ϴ�.
			//_SendNoticeEx(lMsg.Get(2583), m_nNotifyCount + 1, 0);
			
			LogAddTD("[XMasAttackEvent] - Before %d minutes - Advertise", m_nNotifyCount + 1);
		}
	}

	if (nRemainMsec == 0) {
		// �̺�Ʈ ��� -> �̺�Ʈ ����
		// ��Ÿ�� �븮�� ���� ������ �� ���� ���� ħ���Ͽ����ϴ�.
		_SendNotice(lMsg.Get(2583), 1);

#if TESTSERVER == 1
		//DEBUG
		_SendDebugMsg("XMasAttackEvent Start");
#endif

		if (m_bDoEvent)
			SetState(ATTACKEVENT_STATE_PLAYING);
		else
			SetState(ATTACKEVENT_STATE_NONE);
		
		LogAddTD("[XMasAttackEvent] - Event Started");
	}
}

VOID CXMasAttackEvent::ProcState_Playing()
{
	static int nNotifySec = -1;

	int nRemainMsec = m_RemainedTimer.GetMilliSecond();
	int nRemainSec = (nRemainMsec/1000);
	if (nRemainMsec >= 1000)
	{
		// ���� �ѹ��� ������ �ش�.
		if (rand()%20 == 0) 
			Move();
	}

	// 30�� ���� ������ ��ġ�� ����ϱ� ����
	if ( (nRemainSec / 30) != nNotifySec && (nRemainSec % 30) == 0)
	{
		nNotifySec = nRemainSec / 30;

		for (int i = 0; i < m_MonsterHerd.size(); ++i)
		{
			CXMasMonsterHerd* pMonsterHerd = static_cast<CXMasMonsterHerd*>(m_MonsterHerd[i]);
			
			BOOL bLive = FALSE;
			for( int n = 0; n < ALLOC_USEROBJECTSTART; n++) {
				// ��� ������ �߿�
				if (gObj[n].Class == MONSTER_GRADE_BOSS && gObj[n].m_bIsInMonsterHerd) 
				{
					if (gObj[n].Live && gObj[n].MapNumber == pMonsterHerd->GetMapNumber() ) 
					{
						bLive = TRUE;
						break;
					}
				}
			}

			LogAddTD("[XMasAttackEvent] - per 30'S Inform MapNumber:%d, X:%d, Y:%d, Boss Live:%d"
				, pMonsterHerd->GetMapNumber(), pMonsterHerd->GetX(), pMonsterHerd->GetY(), bLive);

			// 30�� ���� ������ ��ġ�� ����ϱ� ����
			_SendMessageMonsterHerd("���ܸ�����[%s] X:%d, Y:%d"
				, pMonsterHerd->GetMapNumber(), pMonsterHerd->GetX(), pMonsterHerd->GetY());

#if TESTSERVER == 1
			//DEBUG
			_SendDebugMsg("XMasAttackEvent - Monster Herd Move MapNumber:%d, X:%d, Y:%d"
				, pMonsterHerd->GetMapNumber(), pMonsterHerd->GetX(), pMonsterHerd->GetY());
#endif
		}

		LogAddTD("[XMasAttackEvent] - NotifySec: %d", nNotifySec);
	}

	// n �� ���Ҵٸ� �������� ���Ѵ�.
	if (!m_bMonsterToDest && (nRemainMsec <= m_nToDestMsec))
	{
		for (int i = 0; i < m_MonsterHerd.size(); ++i)
		{
			CXMasMonsterHerd* pMonsterHerd = static_cast<CXMasMonsterHerd*>(m_MonsterHerd[i]);

			UCHAR ucRAND = rand()%4;
			UCHAR iRandX = g_XMasMapDestPosition[i][ucRAND*2];
			UCHAR iRandY = g_XMasMapDestPosition[i][ucRAND*2+1];
			if (pMonsterHerd->MoveHerd(iRandX, iRandY)) 
			{	
				LogAddTD("[XMasAttackEvent] - Monster Herd Move MapNumber:%d, X:%d, Y:%d"
					, pMonsterHerd->GetMapNumber(), iRandX, iRandY);
				
				_SendMessageMonsterHerd("�����̵� [%s] X:%d, Y:%d"
					, pMonsterHerd->GetMapNumber(), iRandX, iRandY);
			}
		}
		
		m_bMonsterToDest = TRUE;
	}
	
	if (nRemainMsec == 0) {
		// �̺�Ʈ�� ����Ǿ���. -> ����
		// "��� ������ ������ ��������ϴ�."
		//_SendNotice(lMsg.Get(1209), 2);
#if TESTSERVER == 1
		//DEBUG
		_SendDebugMsg("XMasAttackEvent End");
#endif

		if (m_bDoEvent)
			SetState(ATTACKEVENT_STATE_CLOSED);
		else
			SetState(ATTACKEVENT_STATE_NONE);
		
		LogAddTD("[XMasAttackEvent] - Event Ended");
	}
}

VOID CXMasAttackEvent::SetState_None()
{
	StopEvent();
}

VOID CXMasAttackEvent::SetState_Closed()
{
	StopEvent();
	
	if (m_bDoEvent)
		CheckSync();		// ���⼭ ���� �ð��� ����Ͽ� ���Ѵ�.
	else
		SetState(ATTACKEVENT_STATE_NONE);
}

VOID CXMasAttackEvent::SetState_Playing()
{
	// AttackEvent ����ð�
	m_RemainedTimer.SetMilliSecond(m_nPlayMsec);
	
	// �̺�Ʈ ���� ����
	StartEvent();
}

VOID CXMasAttackEvent::Move()
{
	ERROR_RETURN(m_bDoEvent, NULL);
	ERROR_RETURN(!m_bMonsterToDest, NULL);	// ���͵��� �������� ���ߴٸ� �� �̻� ������ �̵��� ����.


	for (int i = 0; i < m_MonsterHerd.size(); ++i)
	{
		MAPPOSITION_DATA mp = m_MapPosition[i];
		CXMasMonsterHerd* pMonsterHerd = static_cast<CXMasMonsterHerd*>(m_MonsterHerd[i]);

		if (_MoveMonsterHerd(pMonsterHerd, mp) == FALSE)
			break;

		int nDivide = m_nRadiusMax - m_nRadiusMin;
		if (nDivide <= 0)
			nDivide = 1;
		int nRadius = nDivide + m_nRadiusMin;
		if (nRadius <= ATTACKEVENT_MIN_RADIUS)
			nRadius = ATTACKEVENT_MIN_RADIUS;
		
		pMonsterHerd->SetRadius(rand()%(m_nRadiusMax - m_nRadiusMin) + m_nRadiusMin);
	}

}

VOID CXMasAttackEvent::_SendMessageMonsterHerd(char* szMsg, BYTE nMapNumber, UCHAR iRandX, UCHAR iRandY)
{
	ERROR_RETURN(szMsg, NULL);

#if TESTSERVER == 1 && !defined(FOR_ONAIR)
	PMSG_NOTICE	pNotice;
	TNotice::MakeNoticeMsgEx(&pNotice, 1, szMsg, g_TerrainManager.GetMapName(nMapNumber), iRandX, iRandY);
#endif

}

VOID CXMasAttackEvent::_InitMonsterHerd()
{
	EnterCriticalSection(&m_critMonsterAddData);
	{
		_IterMapPosition iter;
		for (iter = m_MapPosition.begin(); iter != m_MapPosition.end(); ++iter)
		{
			CXMasMonsterHerd* pMonsterHerd = new CXMasMonsterHerd;
			m_MonsterHerd.push_back(pMonsterHerd);
		}
	}
	LeaveCriticalSection(&m_critMonsterAddData);
}

VOID CXMasAttackEvent::_AddMonsterHerd(CXMasMonsterHerd* pMonsterHerd)
{
	EnterCriticalSection(&m_critMonsterAddData);
	{
		_IterMonsterData iter;
		for (iter = m_MonsterAddData.begin(); iter !=m_MonsterAddData.end(); ++iter) 
		{
			MONSTER_DATA &pRMD = *iter;
			
			int iCount = 100;
			while (iCount--) {
				if (pMonsterHerd->AddMonster(pRMD.m_iType, pRMD.m_bDoRegen, pRMD.m_bDoAttackFirst))
					break;
			}
		}
	}
	LeaveCriticalSection(&m_critMonsterAddData);
}

VOID CXMasAttackEvent::_DeleteMonsterHerd()
{
	EnterCriticalSection(&m_critMonsterAddData);
	{
		_IterMonsterHerd iter;
		for (iter = m_MonsterHerd.begin(); iter != m_MonsterHerd.end();)
		{
			CXMasMonsterHerd* pMonsterHerd = static_cast<CXMasMonsterHerd*>(*iter);
			m_MonsterHerd.erase(iter);

			delete pMonsterHerd;
		}
	}
	LeaveCriticalSection(&m_critMonsterAddData);
}

BOOL CXMasAttackEvent::_MoveMonsterHerd(CXMasMonsterHerd* pMonsterHerd, MAPPOSITION_DATA& position)
{
	INT iCount = 1000;
	UCHAR iRandX, iRandY;
	UCHAR iCX, iCY;

	if (!pMonsterHerd->GetCurrentLocation(iCX, iCY))
		return FALSE;

	while (iCount--) 
	{
		iRandX = rand()%m_nMoveRandSize * (-1*(rand()%3 -1)) + iCX;
		iRandY = rand()%m_nMoveRandSize * (-1*(rand()%3 -1)) + iCY;

		if (iRandX < position.nMapPositionMinX || iRandX > position.nMapPositionMaxX)
			continue;
		if (iRandY < position.nMapPositionMinY || iRandY > position.nMapPositionMaxY)
			continue;
		
		if (pMonsterHerd->CheckLocation(iRandX, iRandY))
			break;
	}

	if (pMonsterHerd->MoveHerd(iRandX, iRandY)) 
	{
		//LogAddTD("[XMasAttackEvent] - Monster Herd Move MapNumber:%d, X:%d, Y:%d"
		//	, position.byMapNumber, iRandX, iRandY);

		//_SendMessageMonsterHerd("�����̵� [%s] X:%d, Y:%d", position.byMapNumber, iRandX, iRandY);
	}

	return TRUE;
}

VOID CXMasAttackEvent::Run()
{
	ERROR_RETURN(!m_bMenualStart, NULL);
	ERROR_RETURN(m_bDoEvent, NULL);

#if TESTSERVER == 1
	if (m_bExecuteDirect == TRUE && IsPossibleExecuteDirect())
	{
		m_RemainedTimer.SetMilliSecond(m_nOpenMsec - 1000);
	}
#endif
	
	switch (m_nEventState) 
	{
		case ATTACKEVENT_STATE_NONE :
			ProcState_None();
			break;

		case ATTACKEVENT_STATE_CLOSED :
			ProcState_Closed();
			break;

		case ATTACKEVENT_STATE_PLAYING :
			ProcState_Playing();
			break;

		default:
			break;
	}

}

VOID CXMasAttackEvent::CheckSync()
{
	if (m_EventTime.empty()) {
		LogAddC(2, "[XMasAttackEvent] No Schedule Time Data");
		SetState(ATTACKEVENT_STATE_NONE);
		return;
	}

	BOOL bChangedTime = FALSE;
	int nHour = 24;
	int nMinute = 60;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	_IterEventTime iter;
	for (iter = m_EventTime.begin(); iter != m_EventTime.end(); ++iter)
	{
		ATTACKEVENT_TIME& et = *iter;
		if (sysTime.wHour * 60 + sysTime.wMinute < et.m_iHour * 60 + et.m_iMinute)
		{
			if (nHour * 60 + nMinute > et.m_iHour * 60 + et.m_iMinute) {
				bChangedTime = TRUE;

				nHour = et.m_iHour;
				nMinute = et.m_iMinute;
			}
		}
	}

	if (bChangedTime == FALSE) {
		nHour = 24;
		nMinute = 60;

		for (iter = m_EventTime.begin(); iter != m_EventTime.end(); ++iter) 
		{
			ATTACKEVENT_TIME& et = *iter;
			if (nHour * 60 + nMinute > et.m_iHour * 60 +et.m_iMinute)
			{
				bChangedTime = TRUE + 1;
				
				nHour = et.m_iHour;
				nMinute = et.m_iMinute;
			}
		}
	}
	
	switch(bChangedTime) {
	case TRUE :
		{
			// ���� �ð����� ������ ����� �ð��� ũ��.
			int nRemainSec = ((60*nHour) * 60 + nMinute*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond));
			m_RemainedTimer.SetSecond(nRemainSec);
		}
		break;
	case TRUE + 1 :
		{
			// ���� �ð����� ������ ����� �ð��� �۴� (���� �� ����)
			int nRemainSec = ((60*(nHour+24)) * 60 + nMinute*60) - ((((60*sysTime.wHour) * 60) + sysTime.wMinute * 60 + sysTime.wSecond));
			m_RemainedTimer.SetSecond(nRemainSec);
		}
		break;
	default:
		{
			LogAddC(2, "[XMasAttackEvent] No Schedule Time Data");
			SetState(ATTACKEVENT_STATE_NONE);
		}
		return;
	}

	LogAddTD("[XMasAttackEvent] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)", m_RemainedTimer.GetMinute(), nHour, nMinute);

	m_nNotifyCount = m_nOpenMsec;
}









void CXMasAttackEvent::Start_Menual()
{
	SetMenualStart(TRUE);
	
	StopEvent();
	
	LogAddTD("�ۡ�[Event Management] [Start] XMasAttackEvent!");
	
	// AttackEvent ����ð�
	m_nPlayMsec	= 30 * TIME_MINUTE_1;

	// �̺�Ʈ ��� -> �̺�Ʈ ����
	// ��Ÿ�� �븮�� ���� ������ �� ���� ���� ħ���Ͽ����ϴ�.
	_SendNotice(lMsg.Get(2583), 1);
	
	SetState(ATTACKEVENT_STATE_PLAYING);
}

void CXMasAttackEvent::End_Menual()
{
	SetMenualStart(FALSE);
}

#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014