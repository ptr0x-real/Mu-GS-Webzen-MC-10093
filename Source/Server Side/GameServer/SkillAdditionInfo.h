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
	// 뮤즈엘프 - 인피니티 애로우 스킬
	int m_iInfinityArrowSkillTime;				// 인피니티 애로우 스킬 사용시 지속시간
	int m_iInfinityArrowUseLevel;				// 인피니티 애로우 스킬 사용 가능한 레벨	
	int	m_iInfinityArrowMPConsumptionPlus0;		// +0 화살 사용시 MP 소모량
	int	m_iInfinityArrowMPConsumptionPlus1;		// +1 화살 사용시 MP 소모량
	int	m_iInfinityArrowMPConsumptionPlus2;		// +2 화살 사용시 MP 소모량
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	int	m_iInfinityArrowMPConsumptionPlus3;		// +3 화살 사용시 MP 소모량
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// SkillAdditionInfo에서 읽어들인 값
	// 파이어 스크림 스킬 - 다크로드
	BOOL	m_bFireScreamSkill;									// 파이어 스크림을 사용 여부
	int		m_iFireScreamExplosionAttackDistance;			// 파이어 스크림 폭발 적용하는 거리(캐릭터를 중심으로 사방거리)
	int		m_iFireScreamExplosionRate;						// 파이어 스크림 폭발 확률
	int		m_iFireScreamMaxAttackCountSameSerial;			// 같은 시리얼에서 공격 가능한 최대 수
	int		m_iFireScreamSerialInitCount;					// 최대 시리얼 카운트

	BOOL	m_bCheckFireScreamSerial;						// 파이어 스크림 시리얼 체크 여부
#endif
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	// 소울바리어 강화 관련
	short	m_nSoulBarrierManaRate_Level[6];			// 소울바리어 강화 레벨별 마나소모 수치(1/1000)
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
	// 파이어 스크림 사용
	void	SetFireScreamSkill( BOOL bValue )					{ m_bFireScreamSkill = bValue; }
	BOOL	GetFireScreamSkill()								{ return m_bFireScreamSkill; }

	// 파이어 스크림 폭발 관련
	void	SetFireScreamExplosionAttackDistance( int iValue )	{ m_iFireScreamExplosionAttackDistance = iValue; }
	int		GetFireScreamExplosionAttackDistance()				{ return m_iFireScreamExplosionAttackDistance; }
	void	SetFireScreamExplosionRate( int iValue )			{ m_iFireScreamExplosionRate = iValue; }
	int		GetFireScreamExplosionRate()						{ return m_iFireScreamExplosionRate; }

	// 파이어 스크림 스킬 시리얼 관련
	void	SetFireScreamMaxAttackCountSameSerial( int iValue ) { m_iFireScreamMaxAttackCountSameSerial = iValue; }
	int		GetFireScreamMaxAttackCountSameSerial()				{ return m_iFireScreamMaxAttackCountSameSerial; }
	void	SetFireScreamSerialInitCount( int iValue )			{ m_iFireScreamSerialInitCount = iValue; }
	int		GetFireScreamSerialInitCount()						{ return m_iFireScreamSerialInitCount; }

	void	SetCheckFireScreamSkill( BOOL bValue )				{ m_bCheckFireScreamSerial = bValue; }
	BOOL	GetCheckFireScreamSkill()							{ return m_bCheckFireScreamSerial; }
	
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 소울바리어 강화 관련
	short	GetSoulBarrierManaRate_Level( int iLevel );
#endif	

};

extern CSkillAdditionInfo	g_SkillAdditionInfo;
#endif // !defined(AFX_SKILLADDITIONINFO_H__2FF4FCE4_D58F_422F_A577_ED5E9209B1EA__INCLUDED_)
