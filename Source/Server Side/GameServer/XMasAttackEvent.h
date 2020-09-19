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

#define TIME_MINUTE_10	(10 * 60 * 1000)	// 10��
#define TIME_MINUTE_1	(1 * 60 * 1000)		// 1��

#define	XMASDROPRATE_ITEM			50		// 50% Ȯ���� ���
#define XMASDROP_ZEN				10000
#define XMASRANGE_BUFF				10		// ��ŸŬ�ν��� �Ѹ� �������� ����


enum _eAttackEvent_Token
{
	TOKEN_EVENT_SCHEDULE			= 0,	// �̺�Ʈ�� �⺻ ����������
	TOKEN_EVENT_MONSTERRAND			= 1,	// �̺�Ʈ�� ������ �����ӿ� ���� ����
	TOKEN_EVENT_MONSTER				= 2,	// �̺�Ʈ�� ������ �߰��Ǵ� ���͵��� �Ӽ�
	TOKEN_EVENT_STARTTIME			= 3,	// �̺�Ʈ�� ������ �ð�����
	TOKEN_EVENT_MAPPOSITION			= 4,	// ���͵��� �����ϰ� ������ ��ǥ
};

#define MONSTER_GRADE_BOSS			476		// ��������(��Ÿ)
#define MONSTER_GRADE_NORMAL		466		// �ϴ��
//#define MONSTER_GRADE_BOSS			135
//#define MONSTER_GRADE_NORMAL		136


#define	IS_XMASMONSTER_BOSS(c)		(c == MONSTER_GRADE_BOSS) ? TRUE : FALSE
#define	IS_XMASMONSTER_NORMAL(c)	(c == MONSTER_GRADE_NORMAL) ? TRUE : FALSE

static int	g_XMasMapDestPosition[3][8] = {		// �� ��ȣ �� ������ ���������� ���� X, Y ��ǥ
	133,	79,							// �η��þ� 1
	87,		126,						// �η��þ� 2
	133,	178,						// �η��þ� 3
	180,	126,						// �η��þ� 4
	164,	42,							// ����ƽ� 1
	221,	85,							// ����ƽ� 2
	164,	42,							// ����ƽ� 3
	221,	85,							// ����ƽ� 4
	160,	75,							// �븮�� 1
	160,	75,							// �븮�� 2
	152,	117,						// �븮�� 3
	209,	133,						// �븮�� 4
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
	virtual VOID	MonsterAttackAction(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);	// ���� ���� �� Ư�� ���Ͱ� �������� ���� ó��
#endif
	UCHAR			GetX()			{	return m_iCUR_X;		}							// ������ �߽� X
	UCHAR			GetY()			{	return m_iCUR_Y;		}							// ������ �߽� Y
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
	virtual VOID	StartEvent();					// �̺�Ʈ�� ��� ������
	virtual VOID	StopEvent();					// �̺�Ʈ�� ��� ������
	virtual VOID	EnableEvent(BOOL bDoEvent);		// �̺�Ʈ�� Ȱ��ȭ ���� ����

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

	VOID			CheckSync();					// Ư�� �ٸ��� ��ũ�� �����.



	VOID			SetState(int nSate);			// Ư�� �ٸ��� �����췯�� Ư�� ���·� �����Ѵ�.
	VOID			ProcState_None();
	VOID			ProcState_Closed();
	VOID			ProcState_Playing();
	
	VOID			SetState_None();
	VOID			SetState_Closed();
	VOID			SetState_Playing();
	VOID			Move();


private:
	_VTMapPosition	m_MapPosition;

	int				m_nEventState;				// ������ �̺�Ʈ ����
	BOOL			m_bMonsterToDest;			// ���͵��� �������� ���ϴ°� ?
	int				m_nMoveRandSize;			// �ѹ� ������ ���� �ִ� ũ��
	int				m_nRadiusMin;				// �ѹ� ������ ���� �ּ� �ݰ�
	int				m_nRadiusMax;				// �ѹ� ������ ���� �ִ� �ݰ�

	int				m_nOpenMsec;				// �̺�Ʈ�� �����ϱ� �� ������ ������ ��
	int				m_nPlayMsec;				// �̺�Ʈ�� ������ �ð�
	int				m_nToDestMsec;				// ���͵��� �������� ���� ��� ���� ���� ���ΰ�?
	int				m_nNotifyCount;				// ���� ī��Ʈ

	BOOL			m_bExecuteDirect;			// �̺�Ʈ �ٷν���
};

VOID CXMasAttackEvent::_SendNotice(char* lpszMsg, WORD wSoundEffectNum)
{
	CHAR szTemp[256];
	wsprintf(szTemp, lpszMsg);
	AllSendServerMsg(szTemp);

	// ���� ȿ�� ���
	PMSG_ANS_CL_EFFECT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
	pMsg.wEffectNum = wSoundEffectNum;
	DataSendAll((LPBYTE)&pMsg, pMsg.h.size);
}

// n ���� �޽���, ���� ȿ�� ��½� ���
VOID CXMasAttackEvent::_SendNoticeEx(char* lpszMsg, int nTime,  WORD wSoundEffectNum)
{
	CHAR szTemp[256];
	wsprintf(szTemp, lpszMsg, nTime);
	AllSendServerMsg(szTemp);

	// ���� ȿ�� ���
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
