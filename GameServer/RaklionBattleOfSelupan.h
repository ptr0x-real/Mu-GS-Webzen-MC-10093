//----------------------------------------------------------------------------------
// FileName				: RaklionBattleOfSelupan.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 보스전 상태 처리
//----------------------------------------------------------------------------------
// RaklionBattleOfSelupan.h: interface for the CRaklionBattleOfSelupan class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONBATTLEOFSELUPAN_H__8F9D9094_205E_4106_97F8_EB95E64F8050__INCLUDED_) && defined(ADD_RAKLION_20080408)
#define AFX_RAKLIONBATTLEOFSELUPAN_H__8F9D9094_205E_4106_97F8_EB95E64F8050__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RaklionSelupan.h"

#define RAKLION_SELUPAN_PATTERN_MAX		7
#define RAKLION_SELUPAN_SKILL_TYPE_MAX	9

class CRaklionBattleOfSelupan  
{
private:
	// DATA LOAD
	BOOL						m_bFileDataLoad;

	// STATE
	int							m_iBattleOfSelupanState;	// 세루판 행동 상태
	INT							m_iSelupanPatternCondition[6];	// 세루판의 각 패턴 변이 조건(HP의 %값)

	// DELAY
	INT							m_iSelupanSkillDelay;		// 세루판의 각 스킬별 딜레이
	INT							m_iSelupanSkillStartTime;	// 스킬을 사용하기 시작한 시간

	BOOL						m_bIsRunFirstSkill;			// 첫번째 스킬을 사용 했는가?

	// SUCCESS CHECK
	BOOL						m_bIsSucccess;				// 보스전을 성공했는가?

	// MONSTER
	CRaklionSelupan				m_RaklionSelupan;
	BOOL						m_SelupanPattern[RAKLION_SELUPAN_PATTERN_MAX][RAKLION_SELUPAN_SKILL_TYPE_MAX];

public:
	CRaklionBattleOfSelupan();
	virtual ~CRaklionBattleOfSelupan();
	
	void	ResetAllData();
	BOOL	LoadData( LPSTR lpszFileName );
	void	Run();

	// PROC STATE
	void	ProcState_NONE();
	void	ProcState_STANDBY();
	void	ProcState_PATTERN_1();
	void	ProcState_PATTERN_2();
	void	ProcState_PATTERN_3();
	void	ProcState_PATTERN_4();
	void	ProcState_PATTERN_5();
	void	ProcState_PATTERN_6();
	void	ProcState_PATTERN_7();
	void	ProcState_DIE();
	
	// SET STATE
	void	SetState( int iBattleOfSelupanState );

	void	SetState_NONE();
	void	SetState_STANDBY();
	void	SetState_PATTERN_1();
	void	SetState_PATTERN_2();
	void	SetState_PATTERN_3();
	void	SetState_PATTERN_4();
	void	SetState_PATTERN_5();
	void	SetState_PATTERN_6();
	void	SetState_PATTERN_7();
	void	SetState_DIE();

	void	ActionOfSelupan( INT iAction );

	void	SetBattleOfSelupanStateAuto();
	void	SetBattleOfSelupanState( int iState )			{ m_iBattleOfSelupanState = iState; }
	int		GetBattleOfSelupanState()						{ return m_iBattleOfSelupanState; }	
	
	void	SetSuccessValue( BOOL bIsSuccess )				{ m_bIsSucccess	= bIsSuccess; }
	BOOL	GetSuccessValue()								{ return m_bIsSucccess; }

	void	SetPatternCondition( INT iPatternNum, INT iCondition );

	void	RegenSelupan();
	void	HideSelupan();
	INT		GetSelupanObjIndex();
	void	DeleteSummonMonster();
	void	SetSelupanSkillDelay( INT iDelay );
	void	GmCommandSelupanPattern( INT iPatternNumber );

	INT		GetSelupanBattleUserCount();
};

#endif // !defined(AFX_RAKLIONBATTLEOFSELUPAN_H__8F9D9094_205E_4106_97F8_EB95E64F8050__INCLUDED_)
