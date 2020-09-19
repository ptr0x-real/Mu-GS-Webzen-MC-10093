// KanturuBattleOfNightmare.h: interface for the CKanturuBattleOfNightmare class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUBATTLEOFNIGHTMARE_H__E16F924B_4DB4_4CEF_B3AB_6BC2F221A3E8__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUBATTLEOFNIGHTMARE_H__E16F924B_4DB4_4CEF_B3AB_6BC2F221A3E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "KanturuStateInfo.h"

class CKanturuBattleOfNightmare  
{
private:
	int							m_iBattleOfNightmareState;	
	BOOL						m_bIsSucccess;

	BOOL						m_bFileDataLoad;					// 데이터 파일 로드 유/무

	CKanturuStateInfo			m_StateInfo[MAX_BATTLE_OF_NIGHTMARE_STATE];
	int							m_StateInfoCount;	
	
	int							m_iNightmareAIGroupNumber;			// 나이트메어 AI Group 번호
	int							m_iNightmareObjIndex;				// 나이트메어 Object Index
	
public:
	CKanturuBattleOfNightmare();
	virtual ~CKanturuBattleOfNightmare();

	BOOL	LoadData( LPSTR lpszFileName );
	void	ResetAllData();
	void	Run();

	void	SetState( int iBattleOfNightmareState );
	void	SetNextState( int iCurrentState );

	void	SetState_NONE();
	void	SetState_IDLE();
	void	SetState_NOTIFY();
	void	SetState_START();	
	void	SetState_END();
	void	SetState_ENDCYCLE();

	void	ProcState_NONE();
	void	ProcState_IDLE();
	void	ProcState_NOTIFY();
	void	ProcState_START();
	void	ProcState_END();
	void	ProcState_ENDCYCLE(); 

	// BATTLE OF NIGHTMARE INFORMATION	
	void	SetBattleOfNightmareState( int iBattleOfNightmareState )	{ m_iBattleOfNightmareState = iBattleOfNightmareState; }
	int		GetBattleOfNightmareState()						{ return m_iBattleOfNightmareState; }	

	void	SetSuccessValue( BOOL	bIsSuccess )	{ m_bIsSucccess	= bIsSuccess; }
	BOOL	GetSuccessValue()						{ return m_bIsSucccess; }
	
	// SCHEDULE
	void	CheckStateTime();
	int		GetRemainTime();
	void	CheckNightmareBattleUser();
	
};

#endif // !defined(AFX_KANTURUBATTLEOFNIGHTMARE_H__E16F924B_4DB4_4CEF_B3AB_6BC2F221A3E8__INCLUDED_)
