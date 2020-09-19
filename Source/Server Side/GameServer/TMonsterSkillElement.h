#if !defined(__TMONSTERSKILLELEMENT_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_01_20050714)
#define __TMONSTERSKILLELEMENT_DEF__

#include "Common\TSync.h"
#include "TSkillElement.h"

#define MAX_MONSTER_SKILL_ELEMENT	100


enum MONSTER_SKILL_ELEMENT_TYPE
{
	MSE_TYPE_STUN				=	0,		// ����
	MSE_TYPE_MOVE				=	1,		// �̵�
	MSE_TYPE_HP					=	2,		// HP
	MSE_TYPE_MP					=	3,		// MP
	MSE_TYPE_AG					=	4,		// AG
	MSE_TYPE_DEFENSE			=	5,		// ���
	MSE_TYPE_ATTACK				=	6,		// ����
	MSE_TYPE_DURABILITY			=	7,		// ������
	MSE_TYPE_SUMMON				=	8,		// ��ȯ
	MSE_TYPE_PUSH				=	9,		// �о��
	MSE_TYPE_STAT_ENERGY		=	10,		// ������ ����
	MSE_TYPE_STAT_STRENGTH		=	11,		// �� ����
	MSE_TYPE_STAT_DEXITERITY	=	12,		// ��ø ����
	MSE_TYPE_STAT_VITALITY		=	13,		// ü�� ����
	MSE_TYPE_REMOVE_SKILL		=	14,		// ���� ��ȿȭ
	MSE_TYPE_RESIST_SKILL		=	15,		// ���� ����
	MSE_TYPE_IMMUNE_SKILL		=	16,		// ���� Immune

#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724
	MSE_TYPE_TELEPORT_SKILL		=	17,		// �����̵�
	MSE_TYPE_DOUBLE_HP			=	18,		// Double HP
	MSE_TYPE_POISON				=	19,		// �� ����
	MSE_TYPE_NORMALATTACK		=	20,		// �Ϲ� ���� : gObjAttack()
#endif	
	
#ifdef ADD_RAKLION_20080408
	MSE_TYPE_IMMUNE_ALL_SKILL	=	21,		// ��� ��������, ������ immune
#endif // ADD_RAKLION_20080408
};

enum MONSTER_SKILL_ELEMENT_INCDEC_TYPE
{
	MSE_INCDEC_TYPE_NONE				=	0,		// ����&���� ����

	MSE_INCDEC_TYPE_PERCENTINC			=	1,		// �ۼ�Ʈ�� ����
	MSE_INCDEC_TYPE_PERCENTDEC			=	2,		// �ۼ�Ʈ�� ����

	MSE_INCDEC_TYPE_CONSTANTINC			=	11,		// ��������� ����
	MSE_INCDEC_TYPE_CONSTANTDEC			=	12,		// ��������� ����

	MSE_INCDEC_TYPE_CYCLE_PERCENT		=	100,	// �ۼ�Ʈ �ֱ�
	MSE_INCDEC_TYPE_CYCLE_PERCENTINC	=	101,	// �ֱ������� �ۼ�Ʈ�� ����
	MSE_INCDEC_TYPE_CYCLE_PERCENTDEC	=	102,	// �ֱ������� �ۼ�Ʈ�� ����

	MSE_INCDEC_TYPE_CYCLE_CONSTANT		=	110,	// ����� �ֱ�
	MSE_INCDEC_TYPE_CYCLE_CONSTANTINC	=	111,	// �ֱ������� ��������� ����
	MSE_INCDEC_TYPE_CYCLE_CONSTANTDEC	=   112,	// �ֱ������� ��������� ����
};


class TMonsterSkillElement : public TSkillElement
{

public:
	TMonsterSkillElement();
	virtual ~TMonsterSkillElement();


public:
	static	int							LoadData( LPSTR lpszFileName );
	static	int							DelAllSkillElement();
	static	TMonsterSkillElement*		FindSkillElement( int iElementNumber );

public:
	static	BOOL						s_bDataLoad;
	static	TMonsterSkillElement		s_MonsterSkillElementArray[MAX_MONSTER_SKILL_ELEMENT];


public:
	void	ForceSkillElement			( int iIndex, int iTargetIndex );

	int		ApplyElementStun			( int iIndex, int iTargetIndex );
	int		ApplyElementMove			( int iIndex, int iTargetIndex );
	int		ApplyElementHP				( int iIndex, int iTargetIndex );
	int		ApplyElementMP				( int iIndex, int iTargetIndex );
	int		ApplyElementAG				( int iIndex, int iTargetIndex );
	int		ApplyElementDefense			( int iIndex, int iTargetIndex );
	int		ApplyElementAttack			( int iIndex, int iTargetIndex );
	int		ApplyElementDurability		( int iIndex, int iTargetIndex );
	int		ApplyElementSummon			( int iIndex, int iTargetIndex );
	int		ApplyElementPush			( int iIndex, int iTargetIndex );
	int		ApplyElementStatEnergy		( int iIndex, int iTargetIndex );
	int		ApplyElementStatStrength	( int iIndex, int iTargetIndex );
	int		ApplyElementStatDexiterity	( int iIndex, int iTargetIndex );
	int		ApplyElementStatVitality	( int iIndex, int iTargetIndex );
	int		ApplyElementRemoveSkill		( int iIndex, int iTargetIndex );
	int		ApplyElementResistSkill		( int iIndex, int iTargetIndex );
	int		ApplyElementImmuneSkill		( int iIndex, int iTargetIndex );

#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724
	int		ApplyElementTeleportSkill	( int iIndex, int iTargetIndex );
	int		ApplyElementDoubleHP		( int iIndex, int iTargetIndex );	
	int		ApplyElementPoison			( int iIndex, int iTargetIndex );
	int		ApplyElementNormalAttack	( int iIndex, int iTargetIndex );
#endif // MODIFY_NEW_MONSTER_SKILL_01_20060724

#ifdef ADD_RAKLION_20080408
	int		ApplyElementImmuneAllSkill	( int iIndex, int iTargetIndex );
#endif // ADD_RAKLION_20080408

	void	Reset();

public:
	char*	GetElementName()			{ return m_szElementName; };
	
	
private:
	char	m_szElementName[MAX_SKILLELEMENTNAME];
	int		m_iElementNumber;
	int		m_iElementType;

	int		m_iSuccessRate;
	int		m_iContinuanceTime;	// ���� �ð��� -1 ��� �ֱ������� Element ����.

	int		m_iIncAndDecType;
	int		m_iIncAndDecValue;

	int		m_iNullifiedSkill;

	int		m_iCharacterClass;
	int		m_iCharacterLevel;
	
	
private:
	

};


#endif // __TMONSTERSKILLELEMENT_DEF__