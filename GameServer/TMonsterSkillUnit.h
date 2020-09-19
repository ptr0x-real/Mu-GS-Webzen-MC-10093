#if !defined(__TMONSTERSKILLUNIT_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_01_20050714)
#define __TMONSTERSKILLUNIT_DEF__

#include "TSkillUnit.h"
#include "TMonsterSkillElement.h"


enum MONSTER_SKILL_UNIT_TARGET
{
	MSU_TARGET_ALL				=	0,		// ��ü
	MSU_TARGET_CHARACTER		=	1,		// ĳ����
	MSU_TARGET_MONSTER			=	2,		// ����
	MSU_TARGET_ONE_CHARACTER	=	3,		// �� ĳ����
	MSU_TARGET_ONE_MONSTER		=	4,		// �� ����
};

enum MONSTER_SKILL_UNIT_SCOPE_TYPE
{
	MSU_SCOPE_NONE				=	-1,		// Ÿ�� �ϳ���.
	MSU_SCOPE_DISTANCE			=	0,		// ���� �Ÿ�
	MSU_SCOPE_TABLE_01			=	1,		// ���� ���̺� 01
	MSU_SCOPE_TABLE_02			=	2,		// ���� ���̺� 02
	MSU_SCOPE_TABLE_03			=	3,		// ���� ���̺� 03
};

class TMonsterSkillUnit : public TSkillUnit
{

public:
	TMonsterSkillUnit();
	virtual ~TMonsterSkillUnit();

	
public:
	static	int						LoadData( LPSTR lpszFileName );
	static	int						DelAllSkillUnit();
	static	TMonsterSkillUnit*		FindSkillUnit( int iUnitNumber );

public:
	static	BOOL					s_bDataLoad;
	static	TMonsterSkillUnit		s_MonsterSkillUnitArray[MAX_MONSTER_SKILL_UNIT];

public:
	void							RunSkill( int iIndex, int iTargetIndex );
	void							Reset();

public:
	char							m_szUnitName[MAX_SKILLUNITNAME];
	int								m_iUnitNumber;
	int								m_iUnitTargetType;
	int								m_iUnitScopeType;
	int								m_iUnitScopeValue;
	int								m_iDelay;

	int								m_iElementsCount;
	TMonsterSkillElement*			m_lpElementsSlot[MAX_MONSTER_SKILLELEMENT_SLOT];
	
};


#endif // __TMONSTERSKILLUNIT_DEF__