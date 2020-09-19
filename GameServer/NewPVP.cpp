// NewPVP.cpp: implementation of the CNewPVP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewPVP.h"

#ifdef ADD_NEWPVP_PKFIELD
#include "Gamemain.h"
#include "PartyClass.h"
#ifdef DARKLORD_WORK
#include "DarkSpirit.h"
#endif
#include "Gate.h"
#include "ProbabilityItemBag.h"

#include "DirPath.h"
#include "DSProtocol.h"
#include "MoveCommand.h"

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif


extern CDirPath			gDirPath;
extern PartyClass		gParty;
CNewPVP					g_NewPVP;

const static int g_GateObserver[DUEL_CHANNEL_MAX] = {
	303, 304, 305, 306
};

const static int g_GateRequester[DUEL_CHANNEL_MAX] = {
	295, 297, 299, 301 
};

const static int g_GateResponsor[DUEL_CHANNEL_MAX] = {
	296, 298, 300, 302, 
};

//movereq(kor).txt 42	"불카누스"	"Vulcanus"	15000	30	294
const static int g_MoveRequestIndex = 42;

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


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewPVP::CNewPVP()
:	m_bNewPVP(FALSE), m_pVulcanuProbability(NULL)
{
	ZeroMemory(&m_DuelChannelList, sizeof(m_DuelChannelList));
	ZeroMemory(m_DuelChannel, sizeof(m_DuelChannel));
	ZeroMemory(m_ObserverCount, sizeof(m_ObserverCount));

	{
		// 채널정보 프로토콜 초기화
		m_DuelChannelList.h.c = PMHC_BYTE;
		m_DuelChannelList.h.headcode = HIBYTE(MSG_DUEL_CHANNELLIST);
		m_DuelChannelList.h.subcode = LOBYTE(MSG_DUEL_CHANNELLIST);
		m_DuelChannelList.h.size = sizeof(m_DuelChannelList);
	}
}

CNewPVP::~CNewPVP()
{
	if (m_pVulcanuProbability)
		delete m_pVulcanuProbability;

	m_ObserverInfoList.clear();
	m_Waiting.clear();
}

void CNewPVP::LoadData()
{
	m_bNewPVP = GetPrivateProfileInt( "GameServerInfo", "NewPVP", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );

	LogAddTD("[NewPVP] Info Set MinMonsterKillLevel:%d", m_bNewPVP);

	if (m_bNewPVP)
	{
		if(m_pVulcanuProbability)
			delete m_pVulcanuProbability;

		m_pVulcanuProbability = new CProbabilityItemBag;
		if (m_pVulcanuProbability == NULL)
		{
			MsgBox("[NewPVP] VulcanuProbability.txt %s", lMsg.Get( 110 ));
			return;
		}
		m_pVulcanuProbability->ProbabilityItemBagInit("EventItembag60.txt");
	}
}

void CNewPVP::Run()
{
	// 결투예약된 정보 처리
	_IterWaiting iter = m_Waiting.begin();
	for (;iter != m_Waiting.end(); )
	{
		SWAITING& waiting = iter->second;
		if (waiting.bExpired == TRUE)
		{
			iter = m_Waiting.erase(iter);
			continue;
		}
		else 
		{
			// 결투요청 제한시간을 초과한 경우 타임아웃 처리한다.
			DWORD dwRemainMsec = GetTickCount() - waiting.dwTime;
			if (dwRemainMsec > RESERVED_TIMEOUT)
			{
				OBJECTSTRUCT& requester = gObj[waiting.nRequester];
				OBJECTSTRUCT& responsor = gObj[waiting.nResponsor];
				
				Cancel(requester, responsor, TRUE);
			}
		}

		iter++;
	}

	for (int i = 0; i < DUEL_CHANNEL_MAX; ++i)
	{
		int nStatus = m_DuelChannel[i].nStatus;
		switch (nStatus)
		{
		case DC_START:
			{
				if (CheckUsersOnConnect(i) == FALSE)
					continue;
			}
			break;

		case DC_START_WAIT:
			{
				DWORD dwRemainMsec = GetTickCount() - m_DuelChannel[i].dwTime;
				{
					int nNotifySec = STARTWAIT_TIME/1000 - dwRemainMsec/1000;
					if ( nNotifySec < 5+1 && (m_DuelChannel[i].nNotifyCount != nNotifySec) )
					{
						m_DuelChannel[i].nNotifyCount = nNotifySec;
						BroadcastMessage(i, DC_ALL, MSG_START, nNotifySec);
					}
				}
				if (dwRemainMsec > STARTWAIT_TIME)
				{
					if (CheckUsersOnConnect(i) == FALSE)
						continue;

					OBJECTSTRUCT& requester = gObj[m_DuelChannel[i].nIndex1];
					OBJECTSTRUCT& responsor = gObj[m_DuelChannel[i].nIndex2];
					{
						gObjClearBuffEffect(&requester, CLEAR_TYPE_NON_PCS_ITEM_EFFECT );
						gObjClearBuffEffect(&responsor, CLEAR_TYPE_NON_PCS_ITEM_EFFECT );

						// 경기 시작전 결투자의 HP, Shield를 다시 채운다.
						ReFill(requester);
						ReFill(responsor);
					}
					BroadcastRound(i, DC_OBSERVER, 0);
					SetStatus(DC_START, i, requester, responsor);
				}
			}
			break;

		case DC_START_TERM:
			{
				DWORD dwRemainMsec = GetTickCount() - m_DuelChannel[i].dwTime;
				if (dwRemainMsec > STARTTERM_TIME)
				{
					if (CheckUsersOnConnect(i) == FALSE)
						continue;

					OBJECTSTRUCT& requester = gObj[m_DuelChannel[i].nIndex1];
					OBJECTSTRUCT& responsor = gObj[m_DuelChannel[i].nIndex2];
					{
						// 라운드 시작전 결투자는 시작위치로 이동 시킨다.
						MoveGate(requester.m_Index, g_GateRequester[i]);
						MoveGate(responsor.m_Index, g_GateResponsor[i]);

						// 라운드 시작전 결투자의 HP, Shield를 다시 채운다.
						ReFill(requester);
						ReFill(responsor);
					}
					BroadcastRound(i, DC_OBSERVER, 0);
					SetStatus(DC_START, i, requester, responsor);
				}
			}
			break;

		case DC_RESERVEDEND:
			{
				DWORD dwRemainMsec = GetTickCount() - m_DuelChannel[i].dwTime;
				{
					int nNotifySec = RESERVEDEND_TIME/1000 - dwRemainMsec/1000;
					if ( nNotifySec < 5+1 && (m_DuelChannel[i].nNotifyCount != nNotifySec) )
					{
						m_DuelChannel[i].nNotifyCount = nNotifySec;
						BroadcastMessage(i, DC_ALL, MSG_MOVE, nNotifySec);
					}
				}
				if (dwRemainMsec > RESERVEDEND_TIME)
				{
					OBJECTSTRUCT& requester = gObj[m_DuelChannel[i].nIndex1];
					OBJECTSTRUCT& responsor = gObj[m_DuelChannel[i].nIndex2];

					Leave(requester, responsor);
					LeaveChannelObserver(i);
					SetStatus(DC_IDLE, i, requester, responsor);
				}
			}
			break;
		}

		if (IS_START(nStatus))
			BroadcastDuelInfo(i, DC_OBSERVER);
	}
}

int CNewPVP::GetDuelChannelId()
{
	for (int i = 0; i < DUEL_CHANNEL_MAX; ++i)
	{
		if (m_DuelChannel[i].nStatus == DC_IDLE)
		{
			return i;
		}
	}

	return -1;
}

int CNewPVP::GetDuelChannelId(int nIndex)
{
	for (int i = 0; i < DUEL_CHANNEL_MAX; ++i)
	{
		if (m_DuelChannel[i].nIndex1 == nIndex ||
			m_DuelChannel[i].nIndex2 == nIndex)
		{
			return i;
		}
	}

	return -1;
}

int	CNewPVP::GetDuelChannelId(LPCSTR lpszName)
{
	for (int i = 0; i < DUEL_CHANNEL_MAX; ++i)
	{
		if (strcmp(m_DuelChannel[i].szName1, lpszName) == 0 ||
			strcmp(m_DuelChannel[i].szName2, lpszName) == 0)
		{
			return i;
		}
	}

	return -1;
}

LPDUEL_CHANNEL CNewPVP::GetDuelChannel()
{
	for (int i = 0; i < DUEL_CHANNEL_MAX; ++i)
	{
		if (m_DuelChannel[i].nStatus == DC_IDLE)
		{
			return &m_DuelChannel[i];
		}
	}

	return NULL;
}

LPDUEL_CHANNEL CNewPVP::GetDuelChannel(LPCSTR lpszName)
{
	for (int i = 0; i < DUEL_CHANNEL_MAX; ++i)
	{
		if (strcmp(m_DuelChannel[i].szName1, lpszName) == 0 ||
			strcmp(m_DuelChannel[i].szName2, lpszName) == 0)
		{
			return &m_DuelChannel[i];
		}
	}
	
	return NULL;
}

void CNewPVP::SetStatus(int nStatus, int nId, OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor)
{
	LPDUEL_CHANNEL lpDuelChannel = &m_DuelChannel[nId];

	switch (nStatus)
	{
	case DC_IDLE:
		{
			lpDuelChannel->nStatus = nStatus;
			lpDuelChannel->dwTime = 0;
			lpDuelChannel->nIndex1 = 0;
			lpDuelChannel->nIndex2 = 0;
			memset(lpDuelChannel->szName1, 0, MAX_IDSTRING);
			memset(lpDuelChannel->szName2, 0, MAX_IDSTRING);
			{
				m_DuelChannelList.channel[nId].bStart = FALSE;
				m_DuelChannelList.channel[nId].bWatch = FALSE;
				memset(m_DuelChannelList.channel[nId].szName1, 0, MAX_IDSTRING);
				memset(m_DuelChannelList.channel[nId].szName2, 0, MAX_IDSTRING);
			}

			SetDuelStatus(requester, responsor, nStatus);
		}
		break;

	case DC_START_WAIT:
		{
			lpDuelChannel->nStatus = nStatus;
			lpDuelChannel->dwTime = GetTickCount();
			lpDuelChannel->nIndex1 = requester.m_Index;
			lpDuelChannel->nIndex2 = responsor.m_Index;
			memcpy(lpDuelChannel->szName1, requester.Name, MAX_IDSTRING);
			memcpy(lpDuelChannel->szName2, responsor.Name, MAX_IDSTRING);
			{
				m_DuelChannelList.channel[nId].bStart = TRUE;
				m_DuelChannelList.channel[nId].bWatch = TRUE;
				memcpy(m_DuelChannelList.channel[nId].szName1, requester.Name, MAX_IDSTRING);
				memcpy(m_DuelChannelList.channel[nId].szName2, responsor.Name, MAX_IDSTRING);
			}

			SetDuelStatus(requester, responsor, nStatus);
		}
		break;

	case DC_START:
		{
			lpDuelChannel->nStatus = nStatus;
			lpDuelChannel->dwTime = GetTickCount();
		}
		break;

	case DC_START_TERM:
		{
			lpDuelChannel->nStatus = nStatus;
			lpDuelChannel->dwTime = GetTickCount();
		}
		break;

	case DC_RESERVEDEND:
		{
			lpDuelChannel->nStatus = nStatus;
			lpDuelChannel->dwTime = GetTickCount();
		}
		break;
	}
}

void CNewPVP::SetDuelStatus(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor, int nStatus)
{
	switch (nStatus)
	{
	case DC_IDLE:
		{
			requester.m_iDuelUser = -1;
			requester.m_iDuelUserRequested = -1;
			requester.m_iDuelUserReserved = -1;
			
			responsor.m_iDuelUser = -1;
			responsor.m_iDuelUserRequested = -1;
			responsor.m_iDuelUserReserved = -1;
		}
		break;

	case DC_RESERVED:
		{
			// 나는 결투 대기 중으로 설정한다.
			requester.m_iDuelUserReserved = responsor.m_Index;
			requester.m_iDuelUser = -1;
			
			// 상대방은 결투 신청 받음으로 설정한다.
			responsor.m_iDuelUserRequested = requester.m_Index;
		}
		break;

	case DC_START_WAIT:
		{
			// 결투를 한다고 알림
			requester.m_iDuelUserReserved = -1;
			requester.m_btDuelScore = 0;
			requester.m_iDuelUser = responsor.m_Index;
			requester.m_iDuelTickCount = GetTickCount();
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
			requester.m_iDuelTimer = GetTickCount();
#endif
			
			responsor.m_iDuelUserRequested = -1;
			responsor.m_iDuelUserReserved = -1;
			responsor.m_btDuelScore = 0;
			responsor.m_iDuelUser = requester.m_Index;
			responsor.m_iDuelTickCount = GetTickCount();
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
			responsor.m_iDuelTimer = GetTickCount();
#endif
		}
		break;
	}
}

/* duel */
int CNewPVP::Reserve(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor)
{
	int nRet = GetDuelStatus(requester);
	ERROR_RETURN_INT(E_SUCCESSED(nRet), nRet, NULL);
	nRet = GetDuelStatusByResponsor(requester);
	ERROR_RETURN_INT(E_SUCCESSED(nRet), nRet, NULL);

	// 결투장 입장전에 입장 요구 조건을 체크한다.
	LPMOVE_COMMAND_DATA lpData = gMoveCommand.GetMoveCommandData(g_MoveRequestIndex);
	ERROR_RETURN_INT_LOG(lpData, ENEWPVP::E_NOT_FOUND_MOVEREQUESTDATA, NULL);
	ERROR_RETURN_INT(IsVulcanusMap(requester.MapNumber) 
		|| (IsVulcanusMap(requester.MapNumber)==FALSE&&requester.Money-lpData->NeedZen>=0) , ENEWPVP::E_NOT_ENOUGH_MONEY, NULL);
	ERROR_RETURN_INT(IsVulcanusMap(responsor.MapNumber) 
		|| (IsVulcanusMap(responsor.MapNumber)==FALSE&&responsor.Money-lpData->NeedZen>=0) , ENEWPVP::E_NOT_ENOUGH_MONEY, NULL);

	// 결투요청 레벨제한은 현재 모든맵이 동일하며 레벨제한 체크를 임시로 첫번째 채널 정보로 한다.
	ERROR_RETURN_INT(CheckLimitLevel(requester.m_Index, g_GateRequester[0]), ENEWPVP::E_LIMIT_LEVEL, NULL);
	ERROR_RETURN_INT(CheckLimitLevel(responsor.m_Index, g_GateResponsor[0]), ENEWPVP::E_LIMIT_LEVEL, NULL);

	{
		// 결투 신청정보를 등록한다.
		SWAITING waiting = {0,};
		waiting.nRequester = requester.m_Index;
		waiting.nResponsor = responsor.m_Index;
		waiting.bExpired = FALSE;
		waiting.dwTime = GetTickCount();
		std::pair<_IterWaiting,bool> pair = m_Waiting.insert(std::make_pair(requester.m_Index, waiting));
		ERROR_RETURN_INT_LOG(pair.second, ENEWPVP::E_FAILED_ENTER, NULL);
	}

	SetDuelStatus(requester, responsor, DC_RESERVED);

	{
		// 상대방에게 결투 요청을 보낸다.
		PMSG_REQ_DUEL_ANSWER req;
		req.h.c = PMHC_BYTE;
		req.h.size = sizeof(req);
		req.h.headcode = HIBYTE(MSG_DUEL_ANSWER);
		req.h.subcode = LOBYTE(MSG_DUEL_ANSWER);
		req.NumberH = HIBYTE(requester.m_Index);
		req.NumberL = LOBYTE(requester.m_Index);
		memcpy(req.szName, requester.Name, MAX_IDSTRING);
		DataSend(responsor.m_Index, (LPBYTE)&req, req.h.size);
	}

	return ENEWPVP::E_NO_ERROR;
}

int	CNewPVP::Join(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor)
{
	ERROR_RETURN_INT(!IsDuel(requester), ENEWPVP::E_ALREADY_DUELLING_1, NULL);
	ERROR_RETURN_INT(!IsDuelReserved(responsor), ENEWPVP::E_ALREADY_DUELRESERVED_1, NULL);
	ERROR_RETURN_INT(requester.m_iDuelUserReserved==responsor.m_Index, ENEWPVP::E_INVITE_MYSELF, NULL);

	{
		// 결투 신청정보가 있는지 검사한다.
		_IterWaiting iter = m_Waiting.find(requester.m_Index);
		ERROR_RETURN_INT_LOG(iter!=m_Waiting.end(), ENEWPVP::E_NOT_EXIST_USER, NULL);
		SWAITING& waiting = iter->second;
		waiting.bExpired = TRUE;
	}

	int nId = GetDuelChannelId();
	ERROR_RETURN_INT(nId>=0 && nId<DUEL_CHANNEL_MAX, ENEWPVP::E_DUEL_MAX, NULL);
	ERROR_RETURN_INT_LOG(m_DuelChannel[nId].nStatus==DC_IDLE, ENEWPVP::E_INVALID_STATUS, NULL);

	// 결투장 입장전에 입장 요구 조건을 체크하고 불카노스맵 이동에 필요한 젠을 차감한다.
	LPMOVE_COMMAND_DATA lpData = gMoveCommand.GetMoveCommandData(g_MoveRequestIndex);
	ERROR_RETURN_INT_LOG(lpData, ENEWPVP::E_NOT_FOUND_MOVEREQUESTDATA, NULL);
	ERROR_RETURN_INT(CheckLimitLevel(requester.m_Index, g_GateRequester[nId]), ENEWPVP::E_LIMIT_LEVEL, NULL);
	ERROR_RETURN_INT(CheckLimitLevel(responsor.m_Index, g_GateResponsor[nId]), ENEWPVP::E_LIMIT_LEVEL, NULL);

	if (IsVulcanusMap(requester.MapNumber) == FALSE) {
		// 불카노스맵 이동에 필요한 젠을 차감한다.
		if ((requester.Money-lpData->NeedZen>=0))
		{
			requester.Money -= lpData->NeedZen;
			GCMoneySend(requester.m_Index, requester.Money);
		}
		else
			return ENEWPVP::E_NOT_ENOUGH_MONEY;
	}

	if (IsVulcanusMap(responsor.MapNumber) == FALSE) {
		// 불카노스맵 이동에 필요한 젠을 차감한다.
		if ((responsor.Money-lpData->NeedZen>=0))
		{
			responsor.Money -= lpData->NeedZen;
			GCMoneySend(responsor.m_Index, responsor.Money);
		}
		else
			return ENEWPVP::E_NOT_ENOUGH_MONEY;
	}

	SetStatus(DC_START_WAIT, nId, requester, responsor);

	{
		// 클라이언트에게 결과를 알려줄 패킷
		PMSG_ANS_DUEL_INVITE res = {0,};
		res.h.c = PMHC_BYTE;
		res.h.size = sizeof(res);
		res.h.headcode = HIBYTE(MSG_DUEL_INVITE);
		res.h.subcode = LOBYTE(MSG_DUEL_INVITE);
		res.nResult = ENEWPVP::E_NO_ERROR;
		memcpy(res.szName, responsor.Name, MAX_IDSTRING);

		// 결투를 요청한 사람
		res.NumberH = HIBYTE(requester.m_Index);
		res.NumberL = LOBYTE(requester.m_Index);
		memcpy(res.szName, requester.Name, MAX_IDSTRING);
		DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
		
		// 결투를 허가한 사람
		res.NumberH = HIBYTE(responsor.m_Index);
		res.NumberL = LOBYTE(responsor.m_Index);
		memcpy(res.szName, responsor.Name, MAX_IDSTRING);
		DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);
	}

	{
		// 결투자의 결투점수를 보낸다.
		BroadcastScore(nId, DC_DUEL);
	}

	{
		MoveGate(requester.m_Index, g_GateRequester[nId]);
		MoveGate(responsor.m_Index, g_GateResponsor[nId]);
	}
	
	LogAddTD("[NewPVP] [%s][%s] Duel Started [%s][%s]",
		responsor.AccountID,
		responsor.Name,
		requester.AccountID,
		requester.Name
	);

	return ENEWPVP::E_NO_ERROR;
}

void CNewPVP::Cancel(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor, BOOL bSendPacket)
{
	{
		// 결투 신청정보를 삭제한다.
		_IterWaiting iter = m_Waiting.find(requester.m_Index);
		ERROR_RETURN_LOG(iter!=m_Waiting.end(), NULL);
		SWAITING& waiting = iter->second;
		waiting.bExpired = TRUE;
	}

	SetDuelStatus(requester, responsor, DC_IDLE);

	if (bSendPacket == TRUE)
	{
		PMSG_ANS_DUEL_INVITE res = {0,};
		res.h.c = PMHC_BYTE;
		res.h.size = sizeof(res);
		res.h.headcode = HIBYTE(MSG_DUEL_INVITE);
		res.h.subcode = LOBYTE(MSG_DUEL_INVITE);
		memcpy(res.szName, responsor.Name, MAX_IDSTRING);
		res.nResult = ENEWPVP::E_REFUSE_INVATE;
		
		if (gObjIsConnected(&requester))
		{
			DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);
		}
		
		if (gObjIsConnected(&responsor))
		{
			DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
		}
	}
}

int CNewPVP::Leave(OBJECTSTRUCT& obj)
{
	int nId = GetDuelChannelId(obj.m_Index);
	ERROR_RETURN_INT_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, ENEWPVP::E_INVALID_CHANNELID, NULL);
	ERROR_RETURN_INT_LOG(IS_START(m_DuelChannel[nId].nStatus)||m_DuelChannel[nId].nStatus==DC_RESERVEDEND, ENEWPVP::E_INVALID_STATUS, NULL);

	LPOBJECTSTRUCT lpTargetObj = NULL;

	// 이전에 이미 결투를 신청했다면
	obj.m_iDuelUserReserved = -1;
	if (IsDuel(obj)) {
		lpTargetObj = &gObj[obj.m_iDuelUser];
	}
	ERROR_RETURN_INT_LOG(lpTargetObj, ENEWPVP::E_INVALID_INDEX, NULL);
	
#ifdef MODIFY_NEWPVP_ENDING_COUNTDOWN_BUGFIX_20090715
	if( m_DuelChannel[nId].nStatus != DC_RESERVEDEND )
	{
		SetStatus(DC_RESERVEDEND, nId, obj, *lpTargetObj);
	
		BroadcastMessage(nId, DC_DUEL, MSG_END, 10);
	}
#else // MODIFY_NEWPVP_ENDING_COUNTDOWN_BUGFIX_20090715
	SetStatus(DC_RESERVEDEND, nId, obj, *lpTargetObj);
	BroadcastMessage(nId, DC_DUEL, MSG_END, 10);
#endif // MODIFY_NEWPVP_ENDING_COUNTDOWN_BUGFIX_20090715

	return ENEWPVP::E_NO_ERROR;
}

void CNewPVP::Leave(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor)
{
	int nId = GetDuelChannelId(requester.m_Index);
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	ERROR_RETURN_LOG(IS_START(m_DuelChannel[nId].nStatus)||m_DuelChannel[nId].nStatus==DC_RESERVEDEND, NULL);

	{
		PMSG_ANS_DUEL_LEAVE res = {0,};
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_LEAVE);
		res.h.subcode = LOBYTE(MSG_DUEL_LEAVE);
		res.h.size = sizeof(res);
		res.nResult = ENEWPVP::E_NO_ERROR;
		
		if (gObjIsConnected(&requester))
		{
			requester.m_iDuelUser = -1;
			res.NumberH = HIBYTE(responsor.m_Index);
			res.NumberL = LOBYTE(responsor.m_Index);
			memcpy(res.szName, responsor.Name, MAX_IDSTRING);
			DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);

			if (IsPKFieldMap(requester.MapNumber)) {
#ifndef FOR_KOREA	// 한국 이외에선 로랜시아로 보낸다.
				gObjMoveGate( requester.m_Index, LORENCIA_DEFAULT_REGEN_MAP_GATE );
#else // NOT_FOR_KOREA
				MoveGate(requester.m_Index, NEWPVP_DEFAULT_REGEN_MAP_GATE);
#endif // NOT_FOR_KOREA
			}
		}
		
		if (gObjIsConnected(&responsor))
		{
			responsor.m_iDuelUser = -1;
			res.NumberH = HIBYTE(responsor.m_Index);
			res.NumberL = LOBYTE(responsor.m_Index);
			memcpy(res.szName, requester.Name, MAX_IDSTRING);
			DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);

			if (IsPKFieldMap(responsor.MapNumber)) {
#ifndef FOR_KOREA	// 한국 이외에선 로랜시아로 보낸다.
				gObjMoveGate( responsor.m_Index, LORENCIA_DEFAULT_REGEN_MAP_GATE );
#else // NOT_FOR_KOREA
				MoveGate(responsor.m_Index, NEWPVP_DEFAULT_REGEN_MAP_GATE);
#endif // NOT_FOR_KOREA
			}
		}
	}
}

void CNewPVP::SetScore(OBJECTSTRUCT& obj)
{
	int nId = GetDuelChannelId(obj.m_Index);
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	ERROR_RETURN_LOG(IS_START(m_DuelChannel[nId].nStatus), NULL);

	obj.m_btDuelScore++;
}

void CNewPVP::CheckScore(OBJECTSTRUCT& obj, OBJECTSTRUCT& target)
{
	int nId = GetDuelChannelId(obj.m_Index);
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	ERROR_RETURN_LOG(IS_START(m_DuelChannel[nId].nStatus), NULL);

	LPDUEL_CHANNEL lpChannel = &m_DuelChannel[nId];
	OBJECTSTRUCT& requester = gObj[lpChannel->nIndex1];
	OBJECTSTRUCT& responsor = gObj[lpChannel->nIndex2];

	if (obj.m_btDuelScore >= WINN_COUNT) 
	{
		//결투가 종료된 경우
		BroadcastDuelInfo(nId, DC_OBSERVER);
		BroadcastScore(nId, DC_ALL);

		SetStatus(DC_RESERVEDEND, nId, requester, responsor);
		BroadcastResult(nId, DC_ALL, obj);

		//승자에겐 검투사의 명예 버프를 걸어준다.
		gObjAddBuffEffect(&obj, BUFFTYPE_GLORYOFGLADIATOR);

		//메시지 클라이언트에서 출력
		//BroadcastMessage(nId, DC_DUEL, MSG_END, 10);

#ifdef MODIFY_DARKSPIRTI_TARGETRELEASE_ON_DUEL_20060524
		// 다크 스피릿의 타겟 해제
		gDarkSpirit[requester.m_Index].ReSetTarget(responsor.m_Index);
		gDarkSpirit[responsor.m_Index].ReSetTarget(requester.m_Index);
#endif // MODIFY_DARKSPIRTI_TARGETRELEASE_ON_DUEL_20060524
		
		LogAdd("[NewPVP] [%s][%s] Win Duel, Loser [%s][%s]",
			obj.AccountID,
			obj.Name,
			target.AccountID,
			target.Name
		);
	} 
	else 
	{
		BroadcastScore(nId, DC_ALL);
		SetStatus(DC_START_TERM, nId, requester, responsor);
	}
}

void CNewPVP::ReFill(OBJECTSTRUCT& obj)
{
	ERROR_RETURN_LOG(gObjIsConnected(&obj), NULL);
	
	obj.BP = obj.MaxBP+obj.AddBP;
	obj.Mana = obj.MaxMana+obj.AddMana;
	GCManaSend(obj.m_Index, (short)obj.Mana, 0xFF, 0, (WORD)obj.BP);

	obj.Life = obj.MaxLife+obj.AddLife;
	obj.iShield = obj.iMaxShield+obj.iAddShield;
	GCReFillSend(obj.m_Index, (WORD)obj.Life, 0xFF, 0, (WORD)obj.iShield);
}

BOOL CNewPVP::MoveGate(int nIndex, int nGateNum)
{
	BYTE btMapNum = MAP_INDEX_PKFIELD;
	BYTE btDir;
	SHORT sX, sY, sLevel;
	int nMapNumber = gGateC.GetGate(nGateNum, sX, sY, btMapNum, btDir, sLevel);
	if (nMapNumber < 0) {
		return FALSE;
	}

	gObjTeleport(nIndex, btMapNum, sX, sY);

#ifdef MODIFY_PKFIELD_DARK_SPRIT_BUGFIX_20090804
	gObjViewportListProtocolCreate(&gObj[nIndex]);
#endif // MODIFY_PKFIELD_DARK_SPRIT_BUGFIX_20090804

	return TRUE;
}

BOOL CNewPVP::CheckLimitLevel(int nIndex, int nGateNum)
{
	BYTE btMapNum = MAP_INDEX_PKFIELD;
	BYTE btDir;
	SHORT sX, sY, sLevel;
	int nMapNumber = gGateC.GetGate(nGateNum, sX, sY, btMapNum, btDir, sLevel);
	if (nMapNumber < 0)
		return FALSE;

	if (gObj[nIndex].Level < sLevel)
		return FALSE;

	return TRUE;
}

void CNewPVP::SetBuff(OBJECTSTRUCT& obj, BOOL bUse)
{
	if (bUse)
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjAddBuffEffect(&obj, BUFFTYPE_INVISABLE, 0, 0, 0, 0, BUFFTIME_INFINITE );
		gObjAddBuffEffect(&obj, BUFFTYPE_OBSERVER, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
		obj.m_iSkillInvisibleTime = 0;	// 0이면 시간 체크를 하지 않음

		ADD_VIEWSKILLSTATE(obj.m_ViewSkillState, STATE_CLOAKING );
		GCStateInfoSend(&obj, 1, STATE_CLOAKING);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjViewportListProtocolDestroy(&obj);
	} 
	else 
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjRemoveBuffEffect(&obj, BUFFTYPE_INVISABLE );
		gObjRemoveBuffEffect(&obj, BUFFTYPE_OBSERVER );
#else
		DEL_VIEWSKILLSTATE(obj.m_ViewSkillState, STATE_CLOAKING );
		GCStateInfoSend(&obj, 0, STATE_CLOAKING );
#endif // GCStateInfoSend(lpObj, 1, STATE_CLOAKING);
		gObjViewportListProtocolCreate(&obj);
	}
}

/* observer */
int CNewPVP::SendChannelList(OBJECTSTRUCT& npc, OBJECTSTRUCT& obj)
{
	DataSend(obj.m_Index, (LPBYTE)&m_DuelChannelList, m_DuelChannelList.h.size);

	return ENEWPVP::E_NO_ERROR;
}

int CNewPVP::GetChannelStatus(const OBJECTSTRUCT& obj)
{
	LPDUEL_CHANNEL lpChannel = GetDuelChannel(obj.Name);
	if (lpChannel)
		return lpChannel->nStatus;

	return DC_IDLE;
}

BOOL CNewPVP::IsSafeState(const OBJECTSTRUCT& obj)
{
	LPDUEL_CHANNEL lpChannel = GetDuelChannel(obj.Name);
	if (!lpChannel)
		return TRUE;

	if (lpChannel->nStatus==DC_START_WAIT 
		|| lpChannel->nStatus==DC_START_TERM 
		|| lpChannel->nStatus==DC_RESERVEDEND)
		return TRUE;
	else
		return FALSE;
}

BOOL CNewPVP::IsObserver(const OBJECTSTRUCT& obj)
{
	return gObjCheckUsedBuffEffect(const_cast<LPOBJECTSTRUCT>(&obj), BUFFTYPE_OBSERVER );
}

int CNewPVP::GetChannelIdByObserver(OBJECTSTRUCT& obj)
{
	_IterObserverInfo iter = m_ObserverInfoList.find(obj.m_Index);
	ERROR_RETURN_INT(iter!=m_ObserverInfoList.end(), -1, NULL);
	
	DUEL_OBSERVERINFO& info = iter->second;
	ERROR_RETURN_INT_LOG(info.nId>=0 && info.nId<DUEL_CHANNEL_MAX, -1, NULL);

	return info.nId;
}

BOOL CNewPVP::CheckUsersOnConnect(int nId)
{
	ERROR_RETURN_INT_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, FALSE, NULL);
	ERROR_RETURN_INT_LOG(IS_START(m_DuelChannel[nId].nStatus), FALSE, NULL);

	OBJECTSTRUCT& requester = gObj[m_DuelChannel[nId].nIndex1];
	OBJECTSTRUCT& responsor = gObj[m_DuelChannel[nId].nIndex2];

	if (!gObjIsConnected(requester.m_Index) || !IsPKFieldMap(requester.MapNumber) )
	{
		Leave(requester);
		return FALSE;
	}

	if (!gObjIsConnected(responsor.m_Index) || !IsPKFieldMap(responsor.MapNumber) )
	{
		Leave(responsor);
		return FALSE;
	}

	return TRUE;
}

void CNewPVP::ChatMsgSend(OBJECTSTRUCT& obj, LPBYTE Msg, int size)
{
	int nId = -1;
	if (IsDuel(obj) == TRUE)
		nId = GetDuelChannelId(obj.m_Index);
	else
		nId = GetChannelIdByObserver(obj);

	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	ERROR_RETURN_LOG(m_DuelChannel[nId].nStatus!=DC_IDLE, NULL);

	LPDUEL_CHANNEL lpChannel = &m_DuelChannel[nId];
	OBJECTSTRUCT& requester = gObj[lpChannel->nIndex1];
	OBJECTSTRUCT& responsor = gObj[lpChannel->nIndex2];

	if (gObjIsConnected(&requester))
	{ 
		DataSend(requester.m_Index, Msg, size);
	}
	
	if (gObjIsConnected(&responsor))
	{
		DataSend(responsor.m_Index, Msg, size);
	}

	BroadcastToObserver(nId, Msg, size);
}

void CNewPVP::Reset(OBJECTSTRUCT& obj)
{
	// 결투 요청중이면 결투요청을 취소한다.
	if (IsDuelRequested(obj))
	{
		Cancel(obj, gObj[obj.m_iDuelUserRequested], TRUE);
	}
	
	// 결투중인경우 결투를 해제한다.
	if (IsDuel(obj))
	{
		Leave(obj);
	}
	
	// 관전자인경우 과전을 종료한다.
	if (IsObserver(obj))
	{
		_IterObserverInfo iter = m_ObserverInfoList.find(obj.m_Index);
		ERROR_RETURN(iter!=m_ObserverInfoList.end(), NULL);

		DUEL_OBSERVERINFO& info = iter->second;
		LeaveChannel(info.nId, obj);
	}
}

void CNewPVP::GetObserverList(int nId, PMSG_DUEL_OBSERVERLIST_BROADCAST& res)
{
	int i = 0;
	_IterObserverInfo iter = m_ObserverInfoList.begin();
	for (;iter != m_ObserverInfoList.end(); ++iter)
	{
		DUEL_OBSERVERINFO& info = iter->second;
		if (info.nId == nId)
		{
			memcpy(res.user[i].szName, info.szName, MAX_IDSTRING);
			i++;

			if (i >= DUEL_OBSERVER_MAX)
			{
				break;
			}
		}
	}
	res.nCount = i;
}

int CNewPVP::JoinChannel(int nId, OBJECTSTRUCT& obj)
{
	// 결투중, 결투요청중이거나 결투신청을 받은 경우 관전 입장 불가
	ERROR_RETURN_INT_LOG(!IsDuel(obj), ENEWPVP::E_ALREADY_DUELLING, NULL);
	ERROR_RETURN_INT_LOG(!IsDuelRequested(obj), ENEWPVP::E_ALREADY_DUELREQUESTED, NULL);
	ERROR_RETURN_INT_LOG(!IsDuelReserved(obj), ENEWPVP::E_ALREADY_DUELRESERVED, NULL);
	
	ERROR_RETURN_INT_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, ENEWPVP::E_INVALID_CHANNELID, NULL);
	ERROR_RETURN_INT_LOG(IS_START(m_DuelChannel[nId].nStatus), ENEWPVP::E_INVALID_STATUS, NULL);
	ERROR_RETURN_INT((m_ObserverCount[nId]+1)<=MAX_OBSERVER, ENEWPVP::E_OBSERVER_MAX , NULL);
	ERROR_RETURN_INT(CheckLimitLevel(obj.m_Index, g_GateObserver[nId]), ENEWPVP::E_LIMIT_LEVEL, NULL);

	_IterObserverInfo iter = m_ObserverInfoList.find(obj.m_Index);
	ERROR_RETURN_INT(iter==m_ObserverInfoList.end(), ENEWPVP::E_FAILED_ENTER, NULL);


	OBJECTSTRUCT& requester = gObj[m_DuelChannel[nId].nIndex1];
	OBJECTSTRUCT& responsor = gObj[m_DuelChannel[nId].nIndex2];

	{
		BroadcastJoinChannel(nId, obj);

		DUEL_OBSERVERINFO info = {0,};
		info.nId = nId;
		info.nIndex = obj.m_Index;
		memcpy(info.szName, obj.Name, MAX_IDSTRING);
		std::pair<_IterObserverInfo,bool> pair = m_ObserverInfoList.insert(std::make_pair(obj.m_Index, info));
		ERROR_RETURN_INT(pair.second, ENEWPVP::E_FAILED_ENTER, NULL);
		
		m_ObserverCount[nId]++;
	}

	{
		PMSG_ANS_DUEL_JOINCNANNEL res = {0,};
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_JOINCNANNEL);
		res.h.subcode = LOBYTE(MSG_DUEL_JOINCNANNEL);
		res.h.size = sizeof(res);
		res.nResult = ENEWPVP::E_NO_ERROR;
		res.nChannelId = nId;
		res.NumberH1 = HIBYTE(requester.m_Index);
		res.NumberL1 = LOBYTE(requester.m_Index);
		res.NumberH2 = HIBYTE(responsor.m_Index);
		res.NumberL2 = LOBYTE(responsor.m_Index);
		memcpy(res.szName1, requester.Name, MAX_IDSTRING);
		memcpy(res.szName2, responsor.Name, MAX_IDSTRING);
		DataSend(obj.m_Index, (LPBYTE)&res, res.h.size);
	}

	{
		PMSG_DUEL_OBSERVERLIST_BROADCAST res = {0,};
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_OBSERVERLIST_BROADCAST);
		res.h.subcode = LOBYTE(MSG_DUEL_OBSERVERLIST_BROADCAST);
		res.h.size = sizeof(res);
		GetObserverList(nId, res);
		DataSend(obj.m_Index, (LPBYTE)&res, res.h.size);

		if (res.nCount != m_ObserverCount[nId])
		{
			LogAddTD("[NewPVP] ChannelId[%d] Count[%d][%d]", nId, res.nCount, m_ObserverCount[nId]);
			DebugLog("%s(%d):", obj.Name, res.nCount);
			for (int n=0; n<res.nCount; ++n)
			{
				DebugLog("%s,", res.user[n].szName);
			}
			DebugLog("\r\n");
		}
	}

	CheckChannelListInfo(nId);

	BroadcastDuelInfo(nId, DC_OBSERVER);

	BroadcastScore(nId, DC_OBSERVER);

	SetBuff(obj, TRUE);

	MoveGate(obj.m_Index, g_GateObserver[nId]);

	return ENEWPVP::E_NO_ERROR;
}

int CNewPVP::LeaveChannel(int nId, OBJECTSTRUCT& obj)
{
	ERROR_RETURN_INT_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, ENEWPVP::E_INVALID_CHANNELID, NULL);
	ERROR_RETURN_INT_LOG(IS_START(m_DuelChannel[nId].nStatus), ENEWPVP::E_INVALID_STATUS, NULL);

	{
		_IterObserverInfo iter = m_ObserverInfoList.find(obj.m_Index);
		ERROR_RETURN_INT(iter!=m_ObserverInfoList.end(), ENEWPVP::E_NOT_EXIST_USER, NULL);
		m_ObserverInfoList.erase(iter);

		m_ObserverCount[nId]--;

		BroadcastLeaveChannel(nId, obj);
	}

	if (gObjIsConnected(&obj))
	{
		PMSG_ANS_DUEL_LEAVECNANNEL res = {0,};
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_LEAVECNANNEL);
		res.h.subcode = LOBYTE(MSG_DUEL_LEAVECNANNEL);
		res.h.size = sizeof(res);
		res.nResult = ENEWPVP::E_NO_ERROR;
		DataSend(obj.m_Index, (LPBYTE)&res, res.h.size);

		if (IsPKFieldMap(obj.MapNumber)) {
			MoveGate(obj.m_Index, NEWPVP_DEFAULT_REGEN_MAP_GATE);
		}
		SetBuff(obj, FALSE);
	}

	CheckChannelListInfo(nId);

	return ENEWPVP::E_NO_ERROR;
}

void CNewPVP::LeaveChannelObserver(int nId)
{
	_IterObserverInfo iter = m_ObserverInfoList.begin();
	for (;iter != m_ObserverInfoList.end();)
	{
		DUEL_OBSERVERINFO& info = iter->second;
		if (info.nId == nId)
		{
			LPOBJECTSTRUCT lpObj = &gObj[info.nIndex];

			if (IsPKFieldMap(lpObj->MapNumber)) {
				MoveGate(lpObj->m_Index, NEWPVP_DEFAULT_REGEN_MAP_GATE);
			}
			SetBuff(*lpObj, FALSE);

			iter = m_ObserverInfoList.erase(iter);
			continue;
		}

		iter++;
	}

	m_ObserverCount[nId] = 0;
}

void CNewPVP::CheckChannelListInfo(int nId)
{
	ERROR_RETURN(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);

	//m_DuelChannelList.channel[nId].bWatch = (m_ObserverCount[nId]<MAX_OBSERVER) ? TRUE : FALSE;
	m_DuelChannelList.channel[nId].bWatch = TRUE;
}



/* common */
void CNewPVP::BroadcastScore(int nId, BYTE nFlag)
{
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	ERROR_RETURN_LOG(IS_START(m_DuelChannel[nId].nStatus), NULL);

	OBJECTSTRUCT& requester = gObj[m_DuelChannel[nId].nIndex1];
	OBJECTSTRUCT& responsor = gObj[m_DuelChannel[nId].nIndex2];
	
	PMSG_DUEL_SCORE_BROADCAST res = {0,};
	{
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_SCORE_BROADCAST);
		res.h.subcode = LOBYTE(MSG_DUEL_SCORE_BROADCAST);
		res.h.size = sizeof(res);
		res.NumberH1 = HIBYTE(requester.m_Index);
		res.NumberL1 = LOBYTE(requester.m_Index);
		res.NumberH2 = HIBYTE(responsor.m_Index);
		res.NumberL2 = LOBYTE(responsor.m_Index);
		res.btDuelScore1 = requester.m_btDuelScore;
		res.btDuelScore2 = responsor.m_btDuelScore;
	}

	if ((nFlag&DC_DUEL) == DC_DUEL)
	{
		DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);
		DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
	}

	if ((nFlag&DC_OBSERVER) == DC_OBSERVER)
	{
		BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
	}
}

void CNewPVP::BroadcastResult(int nId, BYTE nFlag, const OBJECTSTRUCT& obj)
{	
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	ERROR_RETURN_LOG(m_DuelChannel[nId].nStatus==DC_RESERVEDEND, NULL);
	ERROR_RETURN_LOG(gObjIsConnected(obj.m_Index), NULL);
	ERROR_RETURN_LOG(IsDuel(obj), NULL);
	OBJECTSTRUCT& targetObj = gObj[obj.m_iDuelUser];

	PMSG_DUEL_RESULT_BROADCAST res = {0,};
	{
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_RESULT_BROADCAST);
		res.h.subcode = LOBYTE(MSG_DUEL_RESULT_BROADCAST);
		res.h.size = sizeof(res);
		memcpy(res.szWinner, obj.Name, MAX_IDSTRING);
		memcpy(res.szLoser, targetObj.Name, MAX_IDSTRING);
	}

	if ((nFlag&DC_DUEL) == DC_DUEL)
	{
		DataSend(obj.m_Index, (LPBYTE)&res, res.h.size);
		DataSend(targetObj.m_Index, (LPBYTE)&res, res.h.size);
	}

	if ((nFlag&DC_OBSERVER) == DC_OBSERVER)
	{
		BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
	}
}

void CNewPVP::BroadcastDuelInfo(int nId, BYTE nFlag)
{	
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	ERROR_RETURN(IS_START(m_DuelChannel[nId].nStatus), NULL);
	
	OBJECTSTRUCT& requester = gObj[m_DuelChannel[nId].nIndex1];
	OBJECTSTRUCT& responsor = gObj[m_DuelChannel[nId].nIndex2];

	int nLifePer1 = 0, nLifePer2 = 0;
	int ShieldPer1 = 0, ShieldPer2 = 0;

#ifdef MODIFY_LIFE_CHANGEMAXVALUE_BUGFIX
	if ( requester.Life > (requester.MaxLife + requester.AddLife) ) {
		requester.Life = requester.MaxLife + requester.AddLife;
	}	
	if ( requester.iShield > (requester.iMaxShield + requester.iAddShield) ) {
		requester.iShield = requester.iMaxShield + requester.iAddShield;
	}

	if ( responsor.Life > (responsor.MaxLife + responsor.AddLife) ) {
		responsor.Life = responsor.MaxLife + responsor.AddLife;
	}	
	if ( responsor.iShield > (responsor.iMaxShield + responsor.iAddShield) ) {
		responsor.iShield = responsor.iMaxShield + responsor.iAddShield;
	}
#endif // MODIFY_LIFE_CHANGEMAXVALUE_BUGFIX

	nLifePer1 = (requester.Life/(requester.MaxLife+requester.AddLife)) * 100;
	nLifePer2 = (responsor.Life/(responsor.MaxLife+responsor.AddLife)) * 100;
	ShieldPer1 = ((float)requester.iShield/(requester.iMaxShield+requester.iAddShield)) * 100;
	ShieldPer2 = ((float)responsor.iShield/(responsor.iMaxShield+responsor.iAddShield)) * 100;

	PMSG_DUEL_HP_BROADCAST res = {0,};
	{
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_HP_BROADCAST);
		res.h.subcode = LOBYTE(MSG_DUEL_HP_BROADCAST);
		res.h.size = sizeof(res);
		res.NumberH1 = HIBYTE(requester.m_Index);
		res.NumberL1 = LOBYTE(requester.m_Index);
		res.NumberH2 = HIBYTE(responsor.m_Index);
		res.NumberL2 = LOBYTE(responsor.m_Index);
		res.nHP1 = nLifePer1;
		res.nHP2 = nLifePer2;
		res.nShield1 = ShieldPer1;
		res.nShield2 = ShieldPer2;
	}

	if ((nFlag&DC_DUEL) == DC_DUEL)
	{
		DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);
		DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
	}

	if ((nFlag&DC_OBSERVER) == DC_OBSERVER)
	{
		BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
	}

/*
#if TESTSERVER == 1
	{
		PMSG_NOTICE	pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, "NPVP-Life: %d-%d", nLifePer1, nLifePer2);
		DataSend(lpRequester->m_Index, (LPBYTE)&pNotice, pNotice.h.size);
		DataSend(lpResponsor->m_Index, (LPBYTE)&pNotice, pNotice.h.size);
	}

	{
		PMSG_NOTICE	pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, "NPVP-Shield: %d-%d", ShieldPer1, ShieldPer2);
		DataSend(lpRequester->m_Index, (LPBYTE)&pNotice, pNotice.h.size);
		DataSend(lpResponsor->m_Index, (LPBYTE)&pNotice, pNotice.h.size);
	}
#endif
*/
}

void CNewPVP::BroadcastJoinChannel(int nId, const OBJECTSTRUCT& obj)
{
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	ERROR_RETURN_LOG(IS_START(m_DuelChannel[nId].nStatus), NULL);

	PMSG_DUEL_JOINCNANNEL_BROADCAST res = {0,};
	{
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_JOINCNANNEL_BROADCAST);
		res.h.subcode = LOBYTE(MSG_DUEL_JOINCNANNEL_BROADCAST);
		res.h.size = sizeof(res);
		memcpy(res.szName, obj.Name, MAX_IDSTRING);
	}

	BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);

}

void CNewPVP::BroadcastLeaveChannel(int nId, const OBJECTSTRUCT& obj)
{
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	ERROR_RETURN_LOG(IS_START(m_DuelChannel[nId].nStatus), NULL);

	PMSG_DUEL_LEAVECNANNEL_BROADCAST res = {0,};
	{
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_LEAVECNANNEL_BROADCAST);
		res.h.subcode = LOBYTE(MSG_DUEL_LEAVECNANNEL_BROADCAST);
		res.h.size = sizeof(res);
		memcpy(res.szName, obj.Name, MAX_IDSTRING);
	}

	BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
}

void CNewPVP::BroadcastToObserver(int nId, LPBYTE lpData, int nSize)
{
	_IterObserverInfo iter = m_ObserverInfoList.begin();
	for (;iter != m_ObserverInfoList.end(); ++iter)
	{
		DUEL_OBSERVERINFO& info = iter->second;
		if (info.nId == nId && gObjIsConnected(info.nIndex))
		{
			DataSend(info.nIndex, lpData, nSize);
		}
	}
}

void CNewPVP::BroadcastMessage(int nId, BYTE nFlag, BYTE nMsgType, int nNotifySec)
{
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	//ERROR_RETURN(IS_START(m_DuelChannel[nId].nStatus)||, NULL);
	ERROR_RETURN(nNotifySec>0, NULL);
	
	OBJECTSTRUCT& requester = gObj[m_DuelChannel[nId].nIndex1];
	OBJECTSTRUCT& responsor = gObj[m_DuelChannel[nId].nIndex2];

	PMSG_NOTICE	res = {0,};
	switch(nMsgType)
	{
	case MSG_START:
		{
			// "%d초 후에 결투가 시작됩니다."
			TNotice::MakeNoticeMsgEx(&res, 1, lMsg.Get(3419), nNotifySec);
		}
		break;
	case MSG_END:
		{
			// "결투가 종료되었습니다. %d초 후에 마을로 이동됩니다."
			TNotice::MakeNoticeMsgEx(&res, 1, lMsg.Get(3420), nNotifySec);
		}
		break;
	case MSG_MOVE:
		{
			// "%d초 후에 마을로 이동됩니다."
			TNotice::MakeNoticeMsgEx(&res, 1, lMsg.Get(3421), nNotifySec);
		}
		break;

	default:
		return;
	}

	if ((nFlag&DC_DUEL) == DC_DUEL)
	{
		if (gObjIsConnected(&requester))
			DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);

		if (gObjIsConnected(&responsor))
			DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
	}

	if ((nFlag&DC_OBSERVER) == DC_OBSERVER)
	{
		BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
	}
}

void CNewPVP::BroadcastRound(int nId, BYTE nFlag, BOOL bEnd)
{
	ERROR_RETURN_LOG(nId>=0 && nId<DUEL_CHANNEL_MAX, NULL);
	//ERROR_RETURN(IS_START(m_DuelChannel[nId].nStatus), NULL);
	
	OBJECTSTRUCT& requester = gObj[m_DuelChannel[nId].nIndex1];
	OBJECTSTRUCT& responsor = gObj[m_DuelChannel[nId].nIndex2];
	
	PMSG_DUEL_ROUNDSTART_BROADCAST res = {0,};
	{
		res.h.c = PMHC_BYTE;
		res.h.headcode = HIBYTE(MSG_DUEL_ROUNDSTART_BROADCAST);
		res.h.subcode = LOBYTE(MSG_DUEL_ROUNDSTART_BROADCAST);
		res.h.size = sizeof(res);
		res.nFlag = bEnd;
	}

	if ((nFlag&DC_DUEL) == DC_DUEL)
	{
		DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);
		DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
	}
	
	if ((nFlag&DC_OBSERVER) == DC_OBSERVER)
	{
		BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
	}
}

BOOL CNewPVP::CheckPKPenalty(LPOBJECTSTRUCT lpObj)
{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	if (!gPkLimitFree) {
#endif // ADD_PK_LIMIT_FREE_OPTION_20050324

// 무법자는 결투를 선청할 수 없습니다
#ifdef UPDATE_PK_SYSTEM_20070302
		if(lpObj->PartyNumber >= 0)	// 파티에 속할경우
		{
			if( gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= PK_LEVEL_DEFAULT+3)
				return TRUE;
		}
		else
		{
			if(lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3)
				return TRUE;
		}
#else	// UPDATE_PK_SYSTEM_20070302
		if (lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3)
			return TRUE;
#endif	// UPDATE_PK_SYSTEM_20070302
		
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	}
#endif // ADD_PK_LIMIT_FREE_OPTION_20050324
	
	return FALSE;
}

BOOL CNewPVP::IsSelfDefense(LPOBJECTSTRUCT lpObj)
{
	// 상대방이 정당방위라면 결투 신청을 할 수 없다.
	BOOL bRetVal = FALSE;
	for( int i = 0; i < MAXSELFDEFENSE; ++i) 
	{
		// 내가 누군가를 때렸다면 결투 신청을 할 수 없다.
		if( lpObj->SelfDefense[i] >= 0) {
			bRetVal = TRUE;
			break;
		}
	}

	return bRetVal;
}

BOOL CNewPVP::IsGuildWar(LPOBJECTSTRUCT lpObj)
{
	if (lpObj->lpGuild && lpObj->lpGuild->WarState == 1) {
		return TRUE;
	}

	return FALSE;
}

BOOL CNewPVP::DropItem(LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, int nMaxHitUser)
{
	//ERROR_RETURN_INT_LOG(IsVulcanusMonster(lpObj->Class), NULL);
	ERROR_RETURN_INT_LOG(IsVulcanusMap(lpObj->MapNumber), FALSE, NULL);
	ERROR_RETURN_INT_LOG(m_bNewPVP==TRUE, FALSE, NULL);
	ERROR_RETURN_INT(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_GLORYOFGLADIATOR), FALSE, NULL);
	ERROR_RETURN_INT(gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_VULCANUS_EXPERIENCE), FALSE, NULL);

	CItem dropItem;
#if 0
	if( m_pVulcanuProbability->GetItem(&dropItem) == -1 )
	{
		LogAddTD("[NewPVP] VulcanusMonster Item Drop Failed. Can't Get Item from List.");
		return FALSE;
	}
	else
	{
		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, cX, cY
			, dropItem.m_Type, dropItem.m_Level, dropItem.m_Durability
			, dropItem.m_Option1, dropItem.m_Option2, dropItem.m_Option3, nMaxHitUser, dropItem.m_NewOption);

		LogAddTD( "[NewPVP] VulcanusMonster Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d"
			, lpObj->AccountID, lpObj->Name,  btMapNumber, cX, cY
			, dropItem.GetName(), dropItem.m_Type, dropItem.m_Level
			, dropItem.m_Option1, dropItem.m_Option2, dropItem.m_Option3, dropItem.m_NewOption);
	}
#endif
	if( m_pVulcanuProbability->DropEventItemEx(lpObj->m_Index, btMapNumber, cX, cY) == FALSE)
	{
		LogAddTD("[NewPVP] VulcanusMonster Item Drop Failed. Can't Get Item from List.");
		return FALSE;
	}

	return TRUE;
}

#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
void CNewPVP::CheckItemOptForGetExp(LPOBJECTSTRUCT lpObj, INT64 &iExp)
{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	// 마스터 레벨 유저에게는 해당되지 않는다.
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
		return;
#endif // MASTER_LEVEL_UP_SYSTEM_20070912

	INT64 iRetExp = iExp;
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) )
	{
		// 경험치 증가 막기
		iRetExp = 0;
	}
	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_GLORYOFGLADIATOR))
	{
		int iAddExpRate = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_VULCANUS_EXPERIENCE);
		iRetExp += iRetExp * iAddExpRate / 100;
	}

	iExp = iRetExp;
}
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#endif // ADD_NEWPVP_PKFIELD