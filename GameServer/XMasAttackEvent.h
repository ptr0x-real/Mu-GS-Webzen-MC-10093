// XMasAttackEvent.h: interface for the CXMasAttackEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMASEVENT_H__6A3395B1_00E6_434F_9E94_DC57992FF72F__INCLUDED_)
#define AFX_XMASEVENT_H__6A3395B1_00E6_434F_9E94_DC57992FF72F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014

#include "BaseAttackEvent.h"
#include "MonsterHerd.h"

#define TIME_MINUTE_10	(10 * 60 * 1000)	// 10분
#define TIME_MINUTE_1	(1 * 60 * 1000)		// 1분

#define	XMASDROPRATE_ITEM			50		// 50% 확률로 드랍
#define XMASDROP_ZEN				10000
#define XMASRANGE_BUFF				10		// 산타클로스가 뿌릴 광영버프 범위


enum _eAttackEvent_Token
{
	TOKEN_EVENT_SCHEDULE			= 0,	// 이벤트의 기본 스케쥴정보
	TOKEN_EVENT_MONSTERRAND			= 1,	// 이벤트의 무리의 움직임에 관한 정보
	TOKEN_EVENT_MONSTER				= 2,	// 이벤트의 무리에 추가되는 몬스터들의 속성
	TOKEN_EVENT_STARTTIME			= 3,	// 이벤트가 시작할 시간정보
	TOKEN_EVENT_MAPPOSITION			= 4,	// 몬스터들이 랜덤하게 등장할 좌표
};

#define MONSTER_GRADE_BOSS			476		// 보스몬스터(산타)
#define MONSTER_GRADE_NORMAL		466		// 일당들
//#define MONSTER_GRADE_BOSS			135
//#define MONSTER_GRADE_NORMAL		136


#define	IS_XMASMONSTER_BOSS(c)		(c == MONSTER_GRADE_BOSS) ? TRUE : FALSE
#define	IS_XMASMONSTER_NORMAL(c)	(c == MONSTER_GRADE_NORMAL) ? TRUE : FALSE

static int	g_XMasMapDestPosition[3][8] = {		// 맵 번호 별 무리가 최종적으로 모일 X, Y 좌표
	133,	79,							// 로랜시아 1
	87,		126,						// 로랜시아 2
	133,	178,						// 로랜시아 3
	180,	126,						// 로랜시아 4
	164,	42,							// 데비아스 1
	221,	85,							// 데비아스 2
	164,	42,							// 데비아스 3
	221,	85,							// 데비아스 4
	160,	75,							// 노리아 1
	160,	75,							// 노리아 2
	152,	117,						// 노리아 3
	209,	133,						// 노리아 4
};


typedef struct _tagAttackEventMapPosition {
	BYTE	byMapNumber;
	int		nMapPositionMinX;
	int		nMapPositionMinY;
	int		nMapPositionMaxX;
	int		nMapPositionMaxY;
} MAPPOSITION_DATA, *LPMAPPOSITION_DATA;




class CXMasMonsterHerd : public MonsterHerd
{
public:
	CXMasMonsterHerd() {}
	virtual ~CXMasMonsterHerd() {}
	
	virtual BOOL	Start();
	virtual BOOL	MonsterHerdItemDrop(LPOBJECTSTRUCT lpObj);
#if 0
	virtual VOID	MonsterAttackAction(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);	// 무리 몬스터 중 특정 몬스터가 공격했을 때의 처리
#endif
	UCHAR			GetX()			{	return m_iCUR_X;		}							// 현재의 중심 X
	UCHAR			GetY()			{	return m_iCUR_Y;		}							// 현재의 중심 Y
	int				GetMapNumber()	{	return m_iMapNumber;	}
};


typedef vector<MAPPOSITION_DATA>			_VTMapPosition;
typedef vector<MAPPOSITION_DATA>::iterator	_IterMapPosition;


class CXMasAttackEvent  : public CBaseAttackEvent
{
public:
	CXMasAttackEvent();
	virtual ~CXMasAttackEvent();

	virtual BOOL	Load(LPSTR lpszFileName);
	virtual VOID	StartEvent();					// 이벤트를 즉시 시작함
	virtual VOID	StopEvent();					// 이벤트를 즉시 중지함
	virtual VOID	EnableEvent(BOOL bDoEvent);		// 이벤트를 활성화 할지 여부

	virtual void	Start_Menual();
	virtual void	End_Menual();
	VOID			Run();

#if TESTSERVER == 1
	inline BOOL		IsPossibleExecuteDirect();
	void			ExecuteDirect(BOOL bExecuteDirect) {	m_bExecuteDirect = bExecuteDirect;	}
#endif


private:
	inline VOID		_SendNotice(char* lpszMsg, WORD wSoundEffectNum);
	inline VOID		_SendNoticeEx(char* lpszMsg, int nTime,  WORD wSoundEffectNum);
	inline VOID		_SendDebugMsg(char *szNoticeMsg, ... );

	VOID			_SendMessageMonsterHerd(char* szMsg, BYTE nMapNumber, UCHAR iRandX, UCHAR iRandY);
	VOID			_InitMonsterHerd();
	VOID			_AddMonsterHerd(CXMasMonsterHerd* pMonsterHerd);
	VOID			_DeleteMonsterHerd();
	BOOL			_MoveMonsterHerd(CXMasMonsterHerd* pMonsterHerd, MAPPOSITION_DATA& position);

	VOID			CheckSync();					// 특정 다리의 싱크를 맞춘다.



	VOID			SetState(int nSate);			// 특정 다리의 스케쥴러를 특정 상태로 변경한다.
	VOID			ProcState_None();
	VOID			ProcState_Closed();
	VOID			ProcState_Playing();
	
	VOID			SetState_None();
	VOID			SetState_Closed();
	VOID			SetState_Playing();
	VOID			Move();


private:
	_VTMapPosition	m_MapPosition;

	int				m_nEventState;				// 현재의 이벤트 상태
	BOOL			m_bMonsterToDest;			// 몬스터들이 목적지로 향하는가 ?
	int				m_nMoveRandSize;			// 한번 움직일 때의 최대 크기
	int				m_nRadiusMin;				// 한번 움직일 때의 최소 반경
	int				m_nRadiusMax;				// 한번 움직일 때의 최대 반경

	int				m_nOpenMsec;				// 이벤트가 시작하기 전 공지를 시작할 분
	int				m_nPlayMsec;				// 이벤트를 진행할 시간
	int				m_nToDestMsec;				// 몬스터들이 목적지로 종료 몇분 전에 향할 것인가?
	int				m_nNotifyCount;				// 공지 카운트

	BOOL			m_bExecuteDirect;			// 이벤트 바로실행
};

VOID CXMasAttackEvent::_SendNotice(char* lpszMsg, WORD wSoundEffectNum)
{
	CHAR szTemp[256];
	wsprintf(szTemp, lpszMsg);
	AllSendServerMsg(szTemp);

	// 사운드 효과 출력
	PMSG_ANS_CL_EFFECT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
	pMsg.wEffectNum = wSoundEffectNum;
	DataSendAll((LPBYTE)&pMsg, pMsg.h.size);
}

// n 분전 메시지, 사운드 효과 출력시 사용
VOID CXMasAttackEvent::_SendNoticeEx(char* lpszMsg, int nTime,  WORD wSoundEffectNum)
{
	CHAR szTemp[256];
	wsprintf(szTemp, lpszMsg, nTime);
	AllSendServerMsg(szTemp);

	// 사운드 효과 출력
	PMSG_ANS_CL_EFFECT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
	pMsg.wEffectNum = wSoundEffectNum;
	DataSendAll((LPBYTE)&pMsg, pMsg.h.size);
}

#if TESTSERVER == 1
BOOL CXMasAttackEvent::IsPossibleExecuteDirect()
{
	if ( m_nEventState == ATTACKEVENT_STATE_CLOSED 
		&& m_RemainedTimer.GetMilliSecond() >= m_nOpenMsec + (TIME_MINUTE_1 * 2) )
	{
		return TRUE;
	}
	
	return FALSE;
}
#endif

void CXMasAttackEvent::_SendDebugMsg(char *szDebugMsg, ... )
{
	char szMsg[256] ={0,};
	
	va_list	pArguments;
	va_start(pArguments, szDebugMsg);
    vsprintf(szMsg, szDebugMsg, pArguments);
    va_end(pArguments);
	
	AllSendServerMsg(szMsg);
}

extern CXMasAttackEvent		g_XMasAttackEvent;

#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#endif // !defined(AFX_XMASEVENT_H__6A3395B1_00E6_434F_9E94_DC57992FF72F__INCLUDED_)
