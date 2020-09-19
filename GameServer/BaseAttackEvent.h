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

#define ATTACKEVENT_MIN_RADIUS		3		// 무리의 최소 반지름
#define ATTACKEVENT_MAX_RADIUS		6		// 무리의 최대 반지름
#define ATTACKEVENT_MAX_MOVE		15		// 무리가 한번에 최대로 움직일 수 있는 X, Y 크기

enum _eATTACKEVENT_STATE 
{
	ATTACKEVENT_STATE_NONE		= 0,
	ATTACKEVENT_STATE_CLOSED	= 1,
	ATTACKEVENT_STATE_PLAYING	= 2,
};

typedef struct tagATTACKEVENT_TIME {
	INT		m_iHour;					// 이벤트가 시작할 시간
	INT		m_iMinute;					// 이벤트가 시작할 분
} ATTACKEVENT_TIME, *LPATTACKEVENT_TIME;


// 몬스터 정보를 로드하여 가지고 있을 구조체
typedef struct _tagMONSTER_DATA {
	INT		m_iType;					// 몬스터 번호
	BOOL	m_bDoRegen;					// 리젠 여부
	BOOL	m_bDoAttackFirst;			// 선공 여부
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
	virtual VOID		StartEvent() = 0;					// 이벤트를 즉시 시작함
	virtual VOID		StopEvent() = 0;					// 이벤트를 즉시 중지함
	virtual VOID		EnableEvent(BOOL bDoEvent) = 0;		// 이벤트를 활성화 할지 여부

	virtual void		SetMenualStart(BOOL bMenualStart)	{	m_bMenualStart = bMenualStart;	}
	virtual void		Start_Menual() = 0;
	virtual void		End_Menual() = 0;


protected:
	void				Initialize();


protected:
	BOOL				m_bDoEvent;
	BOOL				m_bLoaded;
	BOOL				m_bMenualStart;

	CRITICAL_SECTION	m_critMonsterAddData;		// 몬스터 무리에 추가할 몬스터들의 속성정보 벡터를 보호하는 임계구역
	_VTEventTime		m_EventTime;				// 몬스터 이벤트가 시작할 시간을 가지고 있음
	_VTMonsterData		m_MonsterAddData;			// 몬스터 무리에 추가할 몬스터들의 속성정보를 가지고 저장
	_VTMonsterHerd		m_MonsterHerd;

	CRemainedTimer		m_RemainedTimer;			// 다음 상태로 가지 전까지 남는 초
};

#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#endif // !defined(AFX_BASEATTACKEVENT_H__43FB67CA_43D1_450F_ACE2_8F07B0A6719F__INCLUDED_)
