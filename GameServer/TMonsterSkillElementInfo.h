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
	// ĳ���ͳ� ���Ϳ��� ����ǰ� �ִ� ��ų������Ʈ�� üũ�ϰ� ó���Ѵ�.
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
	// ���ӽð� �ʿ��� Monster Skill Elements

	// ���� -�������� ����ߴ� �� ���

	// ���� ����&����
	INT		m_iSkillElementDefense;
	INT		m_iSkillElementDefenseTime;

	// ���ݷ� ����&����
	INT		m_iSkillElementAttack;
	INT		m_iSkillElementAttackTime;

	// HP �ڵ� ����&����
	INT		m_iSkillElementAutoHP;
	INT		m_iSkillElementAutoHPCycle;
	INT		m_iSkillElementAutoHPTime;

	// MP �ڵ� ����&����
	INT		m_iSkillElementAutoMP;
	INT		m_iSkillElementAutoMPCycle;
	INT		m_iSkillElementAutoMPTime;

	// AG �ڵ� ����&����
	INT		m_iSkillElementAutoAG;
	INT		m_iSkillElementAutoAGCycle;
	INT		m_iSkillElementAutoAGTime;

	// ����Immune
	INT		m_iSkillElementImmuneNumber;
	INT		m_iSkillElementImmuneTime;

	// ��������
	INT		m_iSkillElementResistNumber;
	INT		m_iSkillElementResistTime;

	// STAT �Ͻ��� ����
	INT		m_iSkillElementModifyStat;
	INT		m_iSkillElementModifyStatType;
	INT		m_iSkillElementModifyStatTime;

#ifdef ADD_RAKLION_20080408
	// ��� ��������, ������ Immune
	INT		m_iSkillElementImmuneAllTime;
#endif // ADD_RAKLION_20080408
};

#endif // __TMONSTERSKILLELEMENTINFO_DEF__