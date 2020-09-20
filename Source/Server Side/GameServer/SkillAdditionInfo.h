// SkillAdditionInfo.h: interface for the CSkillAdditionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLADDITIONINFO_H__2FF4FCE4_D58F_422F_A577_ED5E9209B1EA__INCLUDED_)
#define AFX_SKILLADDITIONINFO_H__2FF4FCE4_D58F_422F_A577_ED5E9209B1EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillAdditionInfo  
{
private:
	// ����� - ���Ǵ�Ƽ �ַο� ��ų
	int m_iInfinityArrowSkillTime;				// ���Ǵ�Ƽ �ַο� ��ų ���� ���ӽð�
	int m_iInfinityArrowUseLevel;				// ���Ǵ�Ƽ �ַο� ��ų ��� ������ ����	
	int	m_iInfinityArrowMPConsumptionPlus0;		// +0 ȭ�� ���� MP �Ҹ�
	int	m_iInfinityArrowMPConsumptionPlus1;		// +1 ȭ�� ���� MP �Ҹ�
	int	m_iInfinityArrowMPConsumptionPlus2;		// +2 ȭ�� ���� MP �Ҹ�
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	int	m_iInfinityArrowMPConsumptionPlus3;		// +3 ȭ�� ���� MP �Ҹ�
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// SkillAdditionInfo���� �о���� ��
	// ���̾� ��ũ�� ��ų - ��ũ�ε�
	BOOL	m_bFireScreamSkill;									// ���̾� ��ũ���� ��� ����
	int		m_iFireScreamExplosionAttackDistance;			// ���̾� ��ũ�� ���� �����ϴ� �Ÿ�(ĳ���͸� �߽����� ���Ÿ�)
	int		m_iFireScreamExplosionRate;						// ���̾� ��ũ�� ���� Ȯ��
	int		m_iFireScreamMaxAttackCountSameSerial;			// ���� �ø��󿡼� ���� ������ �ִ� ��
	int		m_iFireScreamSerialInitCount;					// �ִ� �ø��� ī��Ʈ

	BOOL	m_bCheckFireScreamSerial;						// ���̾� ��ũ�� �ø��� üũ ����
#endif
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	// �ҿ�ٸ��� ��ȭ ����
	short	m_nSoulBarrierManaRate_Level[6];			// �ҿ�ٸ��� ��ȭ ������ �����Ҹ� ��ġ(1/1000)
#endif

public:
	CSkillAdditionInfo();
	virtual ~CSkillAdditionInfo();

	void	Init();
	BOOL	Load( LPSTR lpszFileName );

	void	SetInfinityArrowSkillTime( int iValue ) { m_iInfinityArrowSkillTime = iValue; }
	int		GetInfinityArrowSkillTime() { return m_iInfinityArrowSkillTime; }
	void	SetInfinityArrowUseLevel( int iValue ) { m_iInfinityArrowUseLevel = iValue; }
	int		GetInfinityArrowUseLevel() { return m_iInfinityArrowUseLevel; }
	void	SetInfinityArrowMPConsumptionPlus0( int iValue ) { m_iInfinityArrowMPConsumptionPlus0 = iValue; }
	int		GetInfinityArrowMPConsumptionPlus0() { return m_iInfinityArrowMPConsumptionPlus0; }
	void	SetInfinityArrowMPConsumptionPlus1( int iValue ) { m_iInfinityArrowMPConsumptionPlus1 = iValue; }
	int		GetInfinityArrowMPConsumptionPlus1() { return m_iInfinityArrowMPConsumptionPlus1; }
	void	SetInfinityArrowMPConsumptionPlus2( int iValue ) { m_iInfinityArrowMPConsumptionPlus2 = iValue; }
	int		GetInfinityArrowMPConsumptionPlus2() { return m_iInfinityArrowMPConsumptionPlus2; }
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	void	SetInfinityArrowMPConsumptionPlus3( int iValue ) { m_iInfinityArrowMPConsumptionPlus3 = iValue; }
	int		GetInfinityArrowMPConsumptionPlus3() { return m_iInfinityArrowMPConsumptionPlus3; }
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	
	// ���̾� ��ũ�� ���
	void	SetFireScreamSkill( BOOL bValue )					{ m_bFireScreamSkill = bValue; }
	BOOL	GetFireScreamSkill()								{ return m_bFireScreamSkill; }

	// ���̾� ��ũ�� ���� ����
	void	SetFireScreamExplosionAttackDistance( int iValue )	{ m_iFireScreamExplosionAttackDistance = iValue; }
	int		GetFireScreamExplosionAttackDistance()				{ return m_iFireScreamExplosionAttackDistance; }
	void	SetFireScreamExplosionRate( int iValue )			{ m_iFireScreamExplosionRate = iValue; }
	int		GetFireScreamExplosionRate()						{ return m_iFireScreamExplosionRate; }

	// ���̾� ��ũ�� ��ų �ø��� ����
	void	SetFireScreamMaxAttackCountSameSerial( int iValue ) { m_iFireScreamMaxAttackCountSameSerial = iValue; }
	int		GetFireScreamMaxAttackCountSameSerial()				{ return m_iFireScreamMaxAttackCountSameSerial; }
	void	SetFireScreamSerialInitCount( int iValue )			{ m_iFireScreamSerialInitCount = iValue; }
	int		GetFireScreamSerialInitCount()						{ return m_iFireScreamSerialInitCount; }

	void	SetCheckFireScreamSkill( BOOL bValue )				{ m_bCheckFireScreamSerial = bValue; }
	BOOL	GetCheckFireScreamSkill()							{ return m_bCheckFireScreamSerial; }
	
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �ҿ�ٸ��� ��ȭ ����
	short	GetSoulBarrierManaRate_Level( int iLevel );
#endif	

};

extern CSkillAdditionInfo	g_SkillAdditionInfo;
#endif // !defined(AFX_SKILLADDITIONINFO_H__2FF4FCE4_D58F_422F_A577_ED5E9209B1EA__INCLUDED_)
