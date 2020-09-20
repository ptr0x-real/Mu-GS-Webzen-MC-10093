// BaseAttackEvent.cpp: implementation of the CBaseAttackEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseAttackEvent.h"


#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014

#include "Include\ReadScript.h"
#include "Include\Prodef.h"
#include "MonsterHerd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRemainedTimer::CRemainedTimer()
{
	m_dwTick = GetTickCount();
}

CRemainedTimer::~CRemainedTimer()
{
}

void CRemainedTimer::SetSecond(int nRemainedSecond)
{
	m_dwTick = GetTickCount() + nRemainedSecond * 1000;
}

int CRemainedTimer::GetSecond()
{
	DWORD dwTick = GetTickCount();
	int nDiff = m_dwTick - dwTick;
	if (nDiff < 0) {
		m_dwTick = dwTick;
		return 0;
	}
	return (nDiff + 999)/ 1000;
}

void CRemainedTimer::SetMinute(int nReminedMinute)
{
	m_dwTick = GetTickCount() + nReminedMinute * 60000;
}

int CRemainedTimer::GetMinute()
{
	DWORD dwTick = GetTickCount();
	int nDiff = m_dwTick - dwTick;
	if (nDiff < 0) {
		m_dwTick = dwTick;
		return 0;
	}
	return (nDiff + 59999) / 60000;
}

void CRemainedTimer::SetMilliSecond(int nRemainedMilliSecond)
{
	m_dwTick = GetTickCount() + nRemainedMilliSecond;
}

int CRemainedTimer::GetMilliSecond()
{
	DWORD dwTick = GetTickCount();
	int nDiff = m_dwTick - dwTick;
	if (nDiff < 0) {
		m_dwTick = dwTick;
		return 0;
	}
	return nDiff;
}


CBaseAttackEvent::CBaseAttackEvent()
:	m_bDoEvent(FALSE), m_bLoaded(FALSE), m_bMenualStart(FALSE)
{
	InitializeCriticalSection(&m_critMonsterAddData);
}

CBaseAttackEvent::~CBaseAttackEvent()
{
	DeleteCriticalSection(&m_critMonsterAddData);
}

void CBaseAttackEvent::Initialize()
{
	EnterCriticalSection(&m_critMonsterAddData);
	{
		m_MonsterAddData.erase(m_MonsterAddData.begin(), m_MonsterAddData.end());
		m_EventTime.erase(m_EventTime.begin(), m_EventTime.end());
	}
	LeaveCriticalSection(&m_critMonsterAddData);

	m_bLoaded = FALSE;
}

BOOL CBaseAttackEvent::Load(LPSTR lpszFileName)
{
	return FALSE;
}

#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014