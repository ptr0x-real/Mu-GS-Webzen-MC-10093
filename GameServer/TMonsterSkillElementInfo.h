#if !defined(__TMONSTERSKILLELEMENTINFO_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_01_20050714)
#define __TMONSTERSKILLELEMENTINFO_DEF__

#include "TMonsterSkillElement.h"

class TMonsterSkillElementInfo
{

public:
	TMonsterSkillElementInfo()
	{
		Reset();
	}

public:
	// 캐릭터나 몬스터에게 적용되고 있는 스킬엘리먼트를 체크하고 처리한다.
	static void CheckSkillElementInfoProc( LPOBJECTSTRUCT lpObj );


public:
	void Reset();
	
	// R E S E T
	void ResetDefense();
	void ResetAttack();
	void ResetAutoHP();
	void ResetAutoMP();
	void ResetAutoAG();
	void ResetImmune();
	void ResetResist();
	void ResetModifyStat();
	
#ifdef ADD_RAKLION_20080408
	void ResetImmuneAll();
#endif // ADD_RAKLION_20080408


	// C H E C K
	BOOL CheckDefenseTime();
	BOOL CheckAttackTime();
	BOOL CheckAutoHPTime();
	BOOL CheckAutoMPTime();
	BOOL CheckAutoAGTime();
	BOOL CheckImmuneTime();
	BOOL CheckResistTime();
	BOOL CheckModifyStatTime();

#ifdef ADD_RAKLION_20080408
	BOOL CheckImmuneAllTime();
#endif // ADD_RAKLION_20080408

	// D E C R E A S E
	BOOL DecDefenseTime();
	BOOL DecAttackTime();
	BOOL DecAutoHPTime();
	BOOL DecAutoMPTime();
	BOOL DecAutoAGTime();
	BOOL DecImmuneTime();
	BOOL DecResistTime();
	BOOL DecModifyStatTime();

#ifdef ADD_RAKLION_20080408
	BOOL DecImmuneAllTime();
#endif // ADD_RAKLION_20080408


public:
	// 지속시간 필요한 Monster Skill Elements

	// 스턴 -공성에서 사용했던 것 사용

	// 방어력 증가&감소
	INT		m_iSkillElementDefense;
	INT		m_iSkillElementDefenseTime;

	// 공격력 증가&감소
	INT		m_iSkillElementAttack;
	INT		m_iSkillElementAttackTime;

	// HP 자동 증가&감소
	INT		m_iSkillElementAutoHP;
	INT		m_iSkillElementAutoHPCycle;
	INT		m_iSkillElementAutoHPTime;

	// MP 자동 증가&감소
	INT		m_iSkillElementAutoMP;
	INT		m_iSkillElementAutoMPCycle;
	INT		m_iSkillElementAutoMPTime;

	// AG 자동 증가&감소
	INT		m_iSkillElementAutoAG;
	INT		m_iSkillElementAutoAGCycle;
	INT		m_iSkillElementAutoAGTime;

	// 마법Immune
	INT		m_iSkillElementImmuneNumber;
	INT		m_iSkillElementImmuneTime;

	// 마법저항
	INT		m_iSkillElementResistNumber;
	INT		m_iSkillElementResistTime;

	// STAT 일시적 감소
	INT		m_iSkillElementModifyStat;
	INT		m_iSkillElementModifyStatType;
	INT		m_iSkillElementModifyStatTime;

#ifdef ADD_RAKLION_20080408
	// 모든 물리공격, 마법에 Immune
	INT		m_iSkillElementImmuneAllTime;
#endif // ADD_RAKLION_20080408
};

#endif // __TMONSTERSKILLELEMENTINFO_DEF__