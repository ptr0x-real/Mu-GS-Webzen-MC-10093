// BaseAttackEvent.h: interface for the CBaseAttackEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEATTACKEVENT_H__43FB67CA_43D1_450F_ACE2_8F07B0A6719F__INCLUDED_)
#define AFX_BASEATTACKEVENT_H__43FB67CA_43D1_450F_ACE2_8F07B0A6719F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014

#include <VECTOR>
#include <map>

using namespace std;

#define ATTACKEVENT_MIN_RADIUS		3		// ������ �ּ� ������
#define ATTACKEVENT_MAX_RADIUS		6		// ������ �ִ� ������
#define ATTACKEVENT_MAX_MOVE		15		// ������ �ѹ��� �ִ�� ������ �� �ִ� X, Y ũ��

enum _eATTACKEVENT_STATE 
{
	ATTACKEVENT_STATE_NONE		= 0,
	ATTACKEVENT_STATE_CLOSED	= 1,
	ATTACKEVENT_STATE_PLAYING	= 2,
};

typedef struct tagATTACKEVENT_TIME {
	INT		m_iHour;					// �̺�Ʈ�� ������ �ð�
	INT		m_iMinute;					// �̺�Ʈ�� ������ ��
} ATTACKEVENT_TIME, *LPATTACKEVENT_TIME;


// ���� ������ �ε��Ͽ� ������ ���� ����ü
typedef struct _tagMONSTER_DATA {
	INT		m_iType;					// ���� ��ȣ
	BOOL	m_bDoRegen;					// ���� ����
	BOOL	m_bDoAttackFirst;			// ���� ����
} MONSTER_DATA, *LPMONSTER_DATA;


typedef vector<ATTACKEVENT_TIME>			_VTEventTime;
typedef vector<ATTACKEVENT_TIME>::iterator	_IterEventTime;

typedef vector<MONSTER_DATA>				_VTMonsterData; 
typedef vector<MONSTER_DATA>::iterator		_IterMonsterData;

typedef std::vector<MonsterHerd*>			_VTMonsterHerd;
typedef std::vector<MonsterHerd*>::iterator	_IterMonsterHerd;

class CRemainedTimer
{
public:
	CRemainedTimer();
	~CRemainedTimer();
	
public:
	void	SetSecond(int nRemainedSecond);
	int		GetSecond();
	void	SetMinute(int nReminedMinute);
	int		GetMinute();
	void	SetMilliSecond(int nRemainedMilliSecond);
	int		GetMilliSecond();
	
private:
	DWORD	m_dwTick;
};

class CBaseAttackEvent
{
public:
	CBaseAttackEvent();
	virtual ~CBaseAttackEvent();

public:
	virtual BOOL		Load(LPSTR lpszFileName);
	virtual VOID		StartEvent() = 0;					// �̺�Ʈ�� ��� ������
	virtual VOID		StopEvent() = 0;					// �̺�Ʈ�� ��� ������
	virtual VOID		EnableEvent(BOOL bDoEvent) = 0;		// �̺�Ʈ�� Ȱ��ȭ ���� ����

	virtual void		SetMenualStart(BOOL bMenualStart)	{	m_bMenualStart = bMenualStart;	}
	virtual void		Start_Menual() = 0;
	virtual void		End_Menual() = 0;


protected:
	void				Initialize();


protected:
	BOOL				m_bDoEvent;
	BOOL				m_bLoaded;
	BOOL				m_bMenualStart;

	CRITICAL_SECTION	m_critMonsterAddData;		// ���� ������ �߰��� ���͵��� �Ӽ����� ���͸� ��ȣ�ϴ� �Ӱ豸��
	_VTEventTime		m_EventTime;				// ���� �̺�Ʈ�� ������ �ð��� ������ ����
	_VTMonsterData		m_MonsterAddData;			// ���� ������ �߰��� ���͵��� �Ӽ������� ������ ����
	_VTMonsterHerd		m_MonsterHerd;

	CRemainedTimer		m_RemainedTimer;			// ���� ���·� ���� ������ ���� ��
};

#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#endif // !defined(AFX_BASEATTACKEVENT_H__43FB67CA_43D1_450F_ACE2_8F07B0A6719F__INCLUDED_)
