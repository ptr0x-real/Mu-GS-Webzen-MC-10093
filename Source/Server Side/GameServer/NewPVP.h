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

#define NEWPVP_DEFAULT_REGEN_MAP_GATE		294		// ���� ���� �����Ǵ� ���
#define LORENCIA_DEFAULT_REGEN_MAP_GATE		17		// �η��þ� ����Ʈ �ѹ�


#define	RESERVED_TIMEOUT		(1000 * 10)		// ������û ���� �ð�
#define RESERVEDEND_TIME		1000 * 10		// �������� ��
#define STARTWAIT_TIME			1000 * 6		// ���� ��� �ð�
#define STARTTERM_TIME			1000 * 5		// ���� ���� �� ��
#define	MAX_OBSERVER			10				// ������ �ִ� ��
#define WINN_COUNT				10				// �¸� ����

enum _eDuelChannelStatus
{
	DC_IDLE				= 0,	// PVP ������
	DC_RESERVED,				// PVP ����
	DC_START_WAIT,				// PVP ������ ���(%d��)
	DC_START,					// PVP ����
	DC_START_TERM,				// PVP Round �� ��
	DC_RESERVEDEND,				// PVP ���Ό��
};

enum _eBroadcastFlag
{
	DC_DUEL			= 1,		// ������
	DC_OBSERVER		= 2,		// ������
	DC_ALL			= 3,		// ���
};

enum _eMessageType
{
	MSG_START		= 0,		// ���� ���� �޽���
	MSG_END,					// ���� ���� �޽���
	MSG_MOVE,					// ������ �̵� �޽���
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
	int				Reserve(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);	// ������û
	int				Join(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);		// ��������
	void			Cancel(OBJECTSTRUCT& lpRequester, OBJECTSTRUCT& responsor, BOOL bSendPacket = FALSE);
	int				Leave(OBJECTSTRUCT& obj);									// �������� ������� ����

	void			SetScore(OBJECTSTRUCT& obj);								// ��������
	void			CheckScore(OBJECTSTRUCT& obj, OBJECTSTRUCT& target);		// ���� �Ϸ� ��/�� üũ

	/* observer */
	int				SendChannelList(OBJECTSTRUCT& npc, OBJECTSTRUCT& obj);		// ä�θ���Ʈ ��û
	int				JoinChannel(int nId, OBJECTSTRUCT& obj);					// ������û
	int				LeaveChannel(int nId, OBJECTSTRUCT& obj);					// ��������
	void			ChatMsgSend(OBJECTSTRUCT& obj, LPBYTE Msg, int size);		// �������� ä��(ä�κ�)
	void			SetBuff(OBJECTSTRUCT& obj, BOOL bUse);

	/* common */
	int				GetChannelStatus(const OBJECTSTRUCT& obj);
	BOOL			IsSafeState(const OBJECTSTRUCT& obj);
	inline BOOL		IsDuelRequested(const OBJECTSTRUCT& obj);
	inline BOOL		IsDuelReserved(const OBJECTSTRUCT& obj);
	inline BOOL		IsDuel(const OBJECTSTRUCT& obj);
	inline BOOL		IsDuel(const OBJECTSTRUCT& obj, int nIndex);
	BOOL			IsObserver(const OBJECTSTRUCT& obj);
	void			Reset(OBJECTSTRUCT& obj);				// ��������, �����忡�� �̵��� ���

	BOOL			CheckPKPenalty(LPOBJECTSTRUCT lpObj);	// ������ üũ
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
	void			Leave(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);	// ������ �����
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
	PMSG_ANS_DUEL_CHANNELLIST	m_DuelChannelList;					// ���� ä�� ����Ʈ

	DUEL_CHANNEL				m_DuelChannel[DUEL_CHANNEL_MAX];	// ���� ä������
	_MapWaiting					m_Waiting;							// ���� �����
	_MapObserverInfo			m_ObserverInfoList;					// ������ ���
	int							m_ObserverCount[DUEL_CHANNEL_MAX];	// ä�κ� ������ ��

	CProbabilityItemBag*		m_pVulcanuProbability;				// ��ī���� ���� ��� ������
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
