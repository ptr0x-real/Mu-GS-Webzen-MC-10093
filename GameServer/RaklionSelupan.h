//----------------------------------------------------------------------------------
// FileName				: RaklionSelupan.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ����(������) �ൿ Ŭ����
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
	INT					m_iSelupanObjIndex;		// �������� Object Index
	CRaklionObjInfo		m_RaklionMonster;

	INT					m_iSkillDelay;			// ��ų ���� ������
	INT					m_iNowBerserkLevel;		// ���� ����ȭ �ܰ�
	INT					m_iSelupanMinDamage;	// ����ȭ ������ ������ �ּ� ���ݷ�
	INT					m_iSelupanMaxDamage;	// ����ȭ ������ ������ �ִ� ���ݷ�

public:
	CRaklionSelupan();
	virtual ~CRaklionSelupan();

	// INITIALIZE
	void	Init();

	// SELUPAN ACT
	void	RaklionSelupanAct_FirstSkill();			// ���� ����� �о�� ��ų
	void	RaklionSelupanAct_PoisonAttack();		// �⺻ ����(��)
	void	RaklionSelupanAct_IceStorm();			// ���̽� ����
	void	RaklionSelupanAct_IceStrike();			// ���̽� ���ο�
	void	RaklionSelupanAct_SummonMonster();		// ���� ��ȯ
	void	RaklionSelupanAct_Heal();				// �ڱ� �ڽ� ġ��
	void	RaklionSelupanAct_Freeze();				// �󸮱�
	void	RaklionSelupanAct_Teleport();			// ���� �̵�
	void	RaklionSelupanAct_Invincibility();		// ���� ����
	void	RaklionSelupanAct_Berserk1();			// ����ȭ 1�ܰ�
	void	RaklionSelupanAct_Berserk2();			// ����ȭ 2�ܰ�
	void	RaklionSelupanAct_Berserk3();			// ����ȭ 3�ܰ�	
	void	RaklionSelupanAct_Berserk4();			// ����ȭ 4�ܰ�	
	void	RaklionSelupanAct_Berserk_Cancel();		// ����ȭ ����

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
