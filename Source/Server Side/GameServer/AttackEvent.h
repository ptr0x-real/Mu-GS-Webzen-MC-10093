// AttackEvent.h: interface for the CAttackEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTACKEVENT_H__718E3B93_5E4C_4374_972A_E30115605571__INCLUDED_)
#define AFX_ATTACKEVENT_H__718E3B93_5E4C_4374_972A_E30115605571__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAttackEvent  
{
	BYTE    EventState;
	DWORD	EventStartTime;
	BYTE	m_MapNumber;
	int     m_BossMap55;
	int     m_BossMapX55;
	int     m_BossMapY55;

	int     m_BossMap53;
	int     m_BossMapX53;
	int     m_BossMapY53;

#ifdef EVENT_MANAGEMENT_20040711
	BOOL	m_bMenualStart;
#endif

public:
	void Active();
	void Run();
	void End();
	void Start();
	void Init(void);
	CAttackEvent();
	virtual ~CAttackEvent();

#ifdef EVENT_MANAGEMENT_20040711
	void SetMenualStart (BOOL bMenualStart)	{	m_bMenualStart = bMenualStart;	}
	
	void Start_Menual ();
	void End_Menual ();
#endif

};

#endif // !defined(AFX_ATTACKEVENT_H__718E3B93_5E4C_4374_972A_E30115605571__INCLUDED_)
