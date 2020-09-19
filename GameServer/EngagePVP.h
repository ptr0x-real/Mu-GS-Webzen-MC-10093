#ifndef __ENGAGEPVP_H__
#define __ENGAGEPVP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#include <list>

#define DAYS_OF_WEEK 7
#define PVP_EVENT_MAP 9

struct REWARD_ITEM
{
	DWORD m_dwType;
	BYTE m_btLevel;
	BOOL m_bSkill;
	BOOL m_bLuck;
	BYTE m_btOption;
	BYTE m_btExcOption;
	BYTE m_btSetOption;
	INT	 m_iMaxRate;
};

struct EVENT_TIME
{
	int iHour, iMin;
};

typedef std::list<int>			USER_INDEX_LIST;
typedef std::list<REWARD_ITEM>	REWARD_ITEM_LIST;
typedef std::list<EVENT_TIME>	EVENT_TIME_LIST;

enum EVENT_STATE
{
	EVENT_NONE,
	EVENT_CLOSED,
	EVENT_RUNNING,
	EVENT_FINISH,
};

enum PVP_USER_STATE
{
	PVP_USER_NONE,
	PVP_USER_REGISTERED,
	PVP_USER_PLAYING,
};

class CEngagePVP
{
public:
	CEngagePVP();

	~CEngagePVP();

	void Load(const char *filename = "..\\data\\PVPEvent.cfg");

	void Run();

	void ProcessUserRequest(LPOBJECTSTRUCT lpObj);

	void DeleteUser(int aIndex);

private:
	void Clear();

	void ClearUser(int aIndex);

	//Process Events
	void ProcessClosed();
	void ProcessRunning();
	void ProcessFinish();

private:
	USER_INDEX_LIST		m_userList;
	REWARD_ITEM_LIST	m_rewardList;
	EVENT_TIME_LIST		m_timeList[DAYS_OF_WEEK];

	EVENT_STATE			m_eventState;

	bool				m_reloadConfig;

	CRITICAL_SECTION	m_criticalSection;

	EVENT_TIME			*m_nextTime;

	DWORD				m_tickCount;

	DWORD				m_minToAdd;
	DWORD				m_secToAdd;
};

extern CEngagePVP g_PvpEvent;

#endif