//----------------------------------------------------------------------------------
// FileName				: RaklionSelupan.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 보스(세루판) 행동 클래스
//----------------------------------------------------------------------------------
// RaklionSelupan.h: interface for the CRaklionSelupan class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONSELUPAN_H__419985D1_7D35_46B7_A138_5FC25D99E2BD__INCLUDED_) && defined(ADD_RAKLION_20080408)
#define AFX_RAKLIONSELUPAN_H__419985D1_7D35_46B7_A138_5FC25D99E2BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RaklionObjInfo.h"

#define RAKLION_SELUPAN_SKILL_DELAY_MAX 50000

class CRaklionSelupan  
{
private:
	INT					m_iSelupanObjIndex;		// 세루판의 Object Index
	CRaklionObjInfo		m_RaklionMonster;

	INT					m_iSkillDelay;			// 스킬 시전 딜레이
	INT					m_iNowBerserkLevel;		// 현재 광폭화 단계
	INT					m_iSelupanMinDamage;	// 광폭화 이전의 세루판 최소 공격력
	INT					m_iSelupanMaxDamage;	// 광폭화 이전의 세루판 최대 공격력

public:
	CRaklionSelupan();
	virtual ~CRaklionSelupan();

	// INITIALIZE
	void	Init();

	// SELUPAN ACT
	void	RaklionSelupanAct_FirstSkill();			// 최초 등장시 밀어내기 스킬
	void	RaklionSelupanAct_PoisonAttack();		// 기본 공격(독)
	void	RaklionSelupanAct_IceStorm();			// 아이스 스톰
	void	RaklionSelupanAct_IceStrike();			// 아이스 에로우
	void	RaklionSelupanAct_SummonMonster();		// 몬스터 소환
	void	RaklionSelupanAct_Heal();				// 자기 자신 치료
	void	RaklionSelupanAct_Freeze();				// 얼리기
	void	RaklionSelupanAct_Teleport();			// 순간 이동
	void	RaklionSelupanAct_Invincibility();		// 순간 무적
	void	RaklionSelupanAct_Berserk1();			// 광폭화 1단계
	void	RaklionSelupanAct_Berserk2();			// 광폭화 2단계
	void	RaklionSelupanAct_Berserk3();			// 광폭화 3단계	
	void	RaklionSelupanAct_Berserk4();			// 광폭화 4단계	
	void	RaklionSelupanAct_Berserk_Cancel();		// 광폭화 해제

	// SELUPAN DATA
	void	SetSelupanObjIndex( INT iIndex );
	INT		GetSelupanObjIndex();
	void	InitSelupanIndex();
	DWORD	GetSelupanHP();
	void	SetSelupanHP( INT iSelupanHP );
	BOOL	RegenSelupan();
	void	HideSelupan();
	INT		GetTargetOfSelupan();
	void	SetSelupanSkillDelay( INT iDelay );
	INT		GetSelupanSkillDelay();
	INT		GetBerserkLevel();
	void	InitSelupanDamage( INT iMinDamage, INT iMaxDamage );

	// SELUPAN SUMMON SKILL
	void	RegenSummonMonster();
	void	DeleteSummonMonster();
};

#endif // !defined(AFX_RAKLIONSELUPAN_H__419985D1_7D35_46B7_A138_5FC25D99E2BD__INCLUDED_)
