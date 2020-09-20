// NewPVP.h: interface for the CNewPVP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWPVP_H__A0BEEA3A_D1B7_42AC_AA3A_5EECA6640F2F__INCLUDED_)
#define AFX_NEWPVP_H__A0BEEA3A_D1B7_42AC_AA3A_5EECA6640F2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_NEWPVP_PKFIELD

#include "Common\winutil.h"
//#include <vector>
#include <list>

#define NEWPVP_DEFAULT_REGEN_MAP_GATE		294		// 결투 종료 리젠되는 경우
#define LORENCIA_DEFAULT_REGEN_MAP_GATE		17		// 로랜시아 게이트 넘버


#define	RESERVED_TIMEOUT		(1000 * 10)		// 결투신청 제한 시간
#define RESERVEDEND_TIME		1000 * 10		// 결투종료 텀
#define STARTWAIT_TIME			1000 * 6		// 결투 대기 시간
#define STARTTERM_TIME			1000 * 5		// 결투 라운드 간 텀
#define	MAX_OBSERVER			10				// 관전자 최대 수
#define WINN_COUNT				10				// 승리 조건

enum _eDuelChannelStatus
{
	DC_IDLE				= 0,	// PVP 대기상태
	DC_RESERVED,				// PVP 예약
	DC_START_WAIT,				// PVP 시작전 대기(%d초)
	DC_START,					// PVP 시작
	DC_START_TERM,				// PVP Round 간 텀
	DC_RESERVEDEND,				// PVP 종료예약
};

enum _eBroadcastFlag
{
	DC_DUEL			= 1,		// 결투자
	DC_OBSERVER		= 2,		// 관전자
	DC_ALL			= 3,		// 모두
};

enum _eMessageType
{
	MSG_START		= 0,		// 결투 시작 메시지
	MSG_END,					// 결투 종료 메시지
	MSG_MOVE,					// 마을로 이동 메시지
};

#define IS_START(n)		(n>=DC_START_WAIT&&n<=DC_START_TERM)

/* Channel */
typedef struct _tagDUEL_CHANNEL {
	int		nId;
	int		nIndex1;
	int		nIndex2;
	CHAR	szName1[MAX_IDSTRING+1];
	CHAR	szName2[MAX_IDSTRING+1];
	BYTE	nStatus;				// _eDuelChannelStatus
	DWORD	dwTime;
	short	nNotifyCount;
} DUEL_CHANNEL, *LPDUEL_CHANNEL;


/* Observer Start */
typedef struct _tagObserverInfo {
	int		nId;
	int		nIndex;
	CHAR	szName[MAX_IDSTRING+1];
} DUEL_OBSERVERINFO, *LPDUEL_OBSERVERINFO;

typedef map<int, DUEL_OBSERVERINFO>		_MapObserverInfo;
typedef _MapObserverInfo::iterator		_IterObserverInfo;
/* Observer End */


/* Reserved Start */
typedef struct _tagWaiting {
	int		nRequester;
	int		nResponsor;
	BOOL	bExpired;
	DWORD	dwTime;
} SWAITING, *LPSWAITING;

typedef map<int, SWAITING>			_MapWaiting;
typedef _MapWaiting::iterator		_IterWaiting;
/* Reserved End */

class CProbabilityItemBag;

class CNewPVP
{
public:
	CNewPVP();
	virtual ~CNewPVP();
	void	LoadData();
	void	Run();

public:
	/* duel */
	int				Reserve(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);	// 결투요청
	int				Join(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);		// 결투승인
	void			Cancel(OBJECTSTRUCT& lpRequester, OBJECTSTRUCT& responsor, BOOL bSendPacket = FALSE);
	int				Leave(OBJECTSTRUCT& obj);									// 결투해제 명령으로 종료

	void			SetScore(OBJECTSTRUCT& obj);								// 점수설정
	void			CheckScore(OBJECTSTRUCT& obj, OBJECTSTRUCT& target);		// 결투 완료 유/무 체크

	/* observer */
	int				SendChannelList(OBJECTSTRUCT& npc, OBJECTSTRUCT& obj);		// 채널리스트 요청
	int				JoinChannel(int nId, OBJECTSTRUCT& obj);					// 관전요청
	int				LeaveChannel(int nId, OBJECTSTRUCT& obj);					// 관전종료
	void			ChatMsgSend(OBJECTSTRUCT& obj, LPBYTE Msg, int size);		// 결투전용 채팅(채널별)
	void			SetBuff(OBJECTSTRUCT& obj, BOOL bUse);

	/* common */
	int				GetChannelStatus(const OBJECTSTRUCT& obj);
	BOOL			IsSafeState(const OBJECTSTRUCT& obj);
	inline BOOL		IsDuelRequested(const OBJECTSTRUCT& obj);
	inline BOOL		IsDuelReserved(const OBJECTSTRUCT& obj);
	inline BOOL		IsDuel(const OBJECTSTRUCT& obj);
	inline BOOL		IsDuel(const OBJECTSTRUCT& obj, int nIndex);
	BOOL			IsObserver(const OBJECTSTRUCT& obj);
	void			Reset(OBJECTSTRUCT& obj);				// 접속종료, 결투장에서 이동한 경우

	BOOL			CheckPKPenalty(LPOBJECTSTRUCT lpObj);	// 무법자 체크
	BOOL			IsSelfDefense(LPOBJECTSTRUCT lpObj);
	BOOL			IsGuildWar(LPOBJECTSTRUCT lpObj);

	inline BOOL		IsVulcanusMap(int nMapNumber);
	inline BOOL		IsPKFieldMap(int nMapNumber);
	inline BOOL		IsVulcanusMonster(WORD wClass);

	BOOL			IsEnable()	{ return m_bNewPVP;	}
	/* VulcanusMonster Item */
	BOOL			DropItem(LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, int nMaxHitUser);
#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC	
	void			CheckItemOptForGetExp(LPOBJECTSTRUCT lpObj, INT64 &iExp);
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC	

private:
	/* duel */
	void			Leave(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);	// 결투가 종료됨
	/* observer */
	void			LeaveChannelObserver(int nId);
	int				GetChannelIdByObserver(OBJECTSTRUCT& obj);
	BOOL			CheckUsersOnConnect(int nId);


private:
	int				GetDuelChannelId();
	int				GetDuelChannelId(int nIndex);
	int				GetDuelChannelId(LPCSTR lpszName);
	LPDUEL_CHANNEL	GetDuelChannel();
	LPDUEL_CHANNEL	GetDuelChannel(const LPCSTR lpszName);

	inline int		GetDuelStatus(const OBJECTSTRUCT& obj);
	inline int		GetDuelStatusByResponsor(const OBJECTSTRUCT& obj);
	void			SetStatus(int nStatus, int nId, OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);
	void			SetDuelStatus(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor, int nStatus);
	void			GetObserverList(int nId, PMSG_DUEL_OBSERVERLIST_BROADCAST& res);
	void			CheckChannelListInfo(int nId);
	void			ReFill(OBJECTSTRUCT& obj);
	BOOL			MoveGate(int nIndex, int nGateNum);
	BOOL			CheckLimitLevel(int nIndex, int nGateNum);

	/* broadcast */
	void			BroadcastScore(int nId, BYTE nFlag = DC_ALL);
	void			BroadcastResult(int nId, BYTE nFlag, const OBJECTSTRUCT& obj);
	void			BroadcastDuelInfo(int nId, BYTE nFlag = DC_ALL);
	void			BroadcastJoinChannel(int nId, const OBJECTSTRUCT& obj);
	void			BroadcastLeaveChannel(int nId, const OBJECTSTRUCT& obj);
	void			BroadcastToObserver(int nId, LPBYTE lpData, int nSize);
	void			BroadcastMessage(int nId, BYTE nFlag, BYTE nMsgType, int nNotifySec);
	void			BroadcastRound(int nId, BYTE nFlag, BOOL bEnd);



private:
	BOOL			m_bNewPVP;



private:
	PMSG_ANS_DUEL_CHANNELLIST	m_DuelChannelList;					// 결투 채널 리스트

	DUEL_CHANNEL				m_DuelChannel[DUEL_CHANNEL_MAX];	// 결투 채널정보
	_MapWaiting					m_Waiting;							// 결투 대기자
	_MapObserverInfo			m_ObserverInfoList;					// 관전자 목록
	int							m_ObserverCount[DUEL_CHANNEL_MAX];	// 채널별 관전자 수

	CProbabilityItemBag*		m_pVulcanuProbability;				// 불카누스 몬스터 드랍 아이템
};

int CNewPVP::GetDuelStatus(const OBJECTSTRUCT& obj)
{
	if (CHECK_LIMIT(obj.m_iDuelUserRequested, MAX_OBJECT))
		return ENEWPVP::E_ALREADY_DUELREQUESTED;

	if (CHECK_LIMIT(obj.m_iDuelUserReserved, MAX_OBJECT))
		return ENEWPVP::E_ALREADY_DUELRESERVED;

	if (CHECK_LIMIT(obj.m_iDuelUser, MAX_OBJECT))
		return ENEWPVP::E_ALREADY_DUELLING;

	return ENEWPVP::E_NO_ERROR;
}

int CNewPVP::GetDuelStatusByResponsor(const OBJECTSTRUCT& obj)
{
	if (CHECK_LIMIT(obj.m_iDuelUserRequested, MAX_OBJECT))
		return ENEWPVP::E_ALREADY_DUELREQUESTED_1;
	
	if (CHECK_LIMIT(obj.m_iDuelUserReserved, MAX_OBJECT))
		return ENEWPVP::E_ALREADY_DUELRESERVED_1;
	
	if (CHECK_LIMIT(obj.m_iDuelUser, MAX_OBJECT))
		return ENEWPVP::E_ALREADY_DUELLING_1;
	
	return ENEWPVP::E_NO_ERROR;
}

BOOL CNewPVP::IsDuelRequested(const OBJECTSTRUCT& obj)
{
	return CHECK_LIMIT(obj.m_iDuelUserRequested, MAX_OBJECT);
}

BOOL CNewPVP::IsDuelReserved(const OBJECTSTRUCT& obj)
{
	return CHECK_LIMIT(obj.m_iDuelUserReserved, MAX_OBJECT);
}

BOOL CNewPVP::IsDuel(const OBJECTSTRUCT& obj)
{
	return CHECK_LIMIT(obj.m_iDuelUser, MAX_OBJECT);
}

BOOL CNewPVP::IsDuel(const OBJECTSTRUCT& obj, int nIndex)
{
	if (CHECK_LIMIT(obj.m_iDuelUser, MAX_OBJECT))
	{
		if (obj.m_iDuelUser == nIndex)
			return TRUE;
	}

	return FALSE;
}

BOOL CNewPVP::IsVulcanusMap(int nMapNumber)
{
	return (nMapNumber==MAP_INDEX_VULCANUS) ? TRUE : FALSE;
}

BOOL CNewPVP::IsPKFieldMap(int nMapNumber)
{
	return (nMapNumber==MAP_INDEX_PKFIELD) ? TRUE : FALSE;
}

BOOL CNewPVP::IsVulcanusMonster(WORD wClass)
{
	return (wClass==481 || wClass==483 || wClass==485 || wClass==488 || wClass==491) ? TRUE : FALSE;
}

extern CNewPVP	g_NewPVP;
#endif // ADD_NEWPVP_PKFIELD

#endif // !defined(AFX_NEWPVP_H__A0BEEA3A_D1B7_42AC_AA3A_5EECA6640F2F__INCLUDED_)
