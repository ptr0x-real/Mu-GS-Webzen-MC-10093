// DragonEvent.h: interface for the CDragonEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGONEVENT_H__0554F210_2FC5_4DD4_9196_A7E5499707D3__INCLUDED_)
#define AFX_DRAGONEVENT_H__0554F210_2FC5_4DD4_9196_A7E5499707D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDragonEvent  
{	
	BYTE    EventState;
	DWORD	EventStartTime;
	BYTE	m_MapNumber;

#ifdef EVENT_MANAGEMENT_20040711
	BOOL	m_bMenualStart;
#endif


public:


	CDragonEvent();
	virtual ~CDragonEvent();

	BYTE GetState()
	{
		return EventState;
	}

	BYTE GetMapNumber()
	{
		return m_MapNumber;
	}

	void Init();
	void Start();
	void End();
	void Run();
	void DragonActive();

#ifdef EVENT_MANAGEMENT_20040711
	void SetMenualStart (BOOL bMenualStart)	{	m_bMenualStart = bMenualStart;	}
	
	void Start_Menual ();
	void End_Menual ();
#endif

};

#endif // !defined(AFX_DRAGONEVENT_H__0554F210_2FC5_4DD4_9196_A7E5499707D3__INCLUDED_)
