// KanturuBattleStanby.h: interface for the CKanturuBattleStanby class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUBATTLESTANBY_H__077AD7C4_D47B_4FDD_9297_D18A4EEF84B7__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUBATTLESTANBY_H__077AD7C4_D47B_4FDD_9297_D18A4EEF84B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuStateInfo.h"

class CKanturuBattleStanby  
{
private:
	int							m_iBattleStanbyState;	
	BOOL						m_bIsSucccess;

	BOOL						m_bFileDataLoad;					// 데이터 파일 로드 유/무

	CKanturuStateInfo			m_StateInfo[MAX_KANTURU_BATTLE_STANBY_STATE];
	int							m_StateInfoCount;
	
public:
	CKanturuBattleStanby();
	virtual ~CKanturuBattleStanby();

	void	Init();
	BOOL	LoadData( LPSTR lpszFileName );
	void	ResetAllData();
	void	Run();

	void	SetState( int iBattleStanbyState );
	void	SetNextState( int iCurrentState );

	void	SetState_NONE();	
	void	SetState_START();	
	void	SetState_NOTIFY();
	void	SetState_END();
	//void	SetState_ENDCYCLE();

	void	ProcState_NONE();	
	void	ProcState_START();
	void	ProcState_NOTIFY();
	void	ProcState_END();
	//void	ProcState_ENDCYCLE(); 

	// BATTLE STANBY INFORMATION	
	void	SetBattleStanbyState( int iBattleStanbyState )	{ m_iBattleStanbyState = iBattleStanbyState; }
	int		GetBattleStanbyState()						{ return m_iBattleStanbyState; }	

	// SCHEDULE
	void	CheckStateTime();
	int		GetRemainTime();

};


#endif // !defined(AFX_KANTURUBATTLESTANBY_H__077AD7C4_D47B_4FDD_9297_D18A4EEF84B7__INCLUDED_)
