#if !defined(__TMONSTERAIELEMENT_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAIELEMENT_DEF__

#include "TSkillDefine.h"
#include "TMonsterAIState.h"
#include "TMonsterAIMovePath.h"

#define MAX_AIELEMENTNAME				50
#define MAX_MONSTER_AI_ELEMENT			100
#define MAX_MONSTER_AI_ELEMENT_STATE	8

enum MONSTER_AI_ELEMENT_STATE
{
	MAE_STATE_NORMAL				= 0,		// �Ϲ� �ൿ �Ӽ�
	MAE_STATE_MOVE					= 1,		// �̵� �ൿ �Ӽ�
	MAE_STATE_ATTACK				= 2,		// ���� �ൿ �Ӽ�
	MAE_STATE_HEAL					= 3,		// ġ�� �ൿ �Ӽ�
	MAE_STATE_AVOID					= 4,		// ȸ�� �ൿ �Ӽ�
	MAE_STATE_HELP					= 5,		// ���� �ൿ �Ӽ�
	MAE_STATE_SPECIAL				= 6,		// Ư�� �ൿ �Ӽ�
	MAE_STATE_EVENT					= 7,		// �̺�Ʈ �ൿ �Ӽ� ( �ó�������� )
};

enum MONSTER_AI_ELEMENT_TYPE
{
	MAE_TYPE_COMMON_NORMAL			= 1,		// �Ϲ� - �Ϲ�

	MAE_TYPE_MOVE_NORMAL			= 11,		// �̵� - �Ϲ�
	MAE_TYPE_MOVE_TARGET			= 12,		// �̵� - Ư�� Ÿ�ٿ��� �̵�
	MAE_TYPE_GROUP_MOVE				= 13,		// �׷� �̵� - �Ϲ�
	MAE_TYPE_GROUP_MOVE_TARGET		= 14,		// �׷� �̵� - Ÿ��
	
	MAE_TYPE_ATTACK_NORMAL			= 21,		// ���� - �Ϲ� ����
	MAE_TYPE_ATTACK_AREA			= 22,		// �׷� ���� - �Ϲ� ����

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	MAE_TYPE_ATTACK_PENETRATION		= 23,		// ���� - ����
#endif
	
	MAE_TYPE_HEAL_SELF				= 31,		// ġ�� - �ڽ� ġ��
	MAE_TYPE_HEAL_GROUP				= 32,		// ġ�� - �׷� ġ��
	
	MAE_TYPE_AVOID_NORMAL			= 41,		// ȸ��

	MAE_TYPE_HELP_HP				= 51,		// ���� - HP ȸ��
	MAE_TYPE_HELP_BUFF				= 52,		// ���� - Ư�� ����
	MAE_TYPE_HELP_TARGET			= 53,		// ���� - Ÿ�� ����

	MAE_TYPE_SPECIAL				= 61,		// Ư��
	MAE_TYPE_SPECIAL_SOMMON			= 62,		// Ư�� - ��ȯ

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	MAE_TYPE_SPECIAL_IMMUNE			= 64,		// Ư�� - ����( ���� �������� ���� ���� )

	MAE_TYPE_SPECIAL_NIGHTMARE_SUMMON = 65,		// Ư�� - ����Ʈ�޾� ��ȯ(������)
	MAE_TYPE_SPECIAL_WARP			  = 66,		// ���� - (����Ʈ�޾� ����. ������)
	MAE_TYPE_SPECIAL_SKILLATTACK	  = 67,		// ��ų ���� ���� (Ȯ���� ����)
	MAE_TYPE_SPECIAL_CHANGEAI		  = 68,		// AI Unit ����
#endif
	
	MAE_TYPE_EVENT					= 71,		// �̺�Ʈ
};

enum MONSTER_AI_ELELMENT_TARGET_TYPE
{
	MAE_TARGET_TYPE_CHARACTER		= 1,		// ĳ����
	MAE_TARGET_TYPE_XY				= 2,		// ��ǥ
	MAE_TARGET_TYPE_MONSTER			= 3,		// ���� - ���� ���� ������� ���� ��� �߰�		
};

class TMonsterAIElement
{

public:
	TMonsterAIElement();
	virtual ~TMonsterAIElement();

public:
	static	int							LoadData( LPSTR lpszFileName );
	static	int							DelAllAIElement();
	static	TMonsterAIElement*			FindAIElement( int iElementNumber );

public:
	static	BOOL						s_bDataLoad;
	static	TMonsterAIElement			s_MonsterAIElementArray[MAX_MONSTER_SKILL_ELEMENT];
	
public:
	static	TMonsterAIMovePath			s_MonsterAIMovePath[];

public:
	int		ForceAIElement				( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );

	int		ApplyElementCommon			( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );

	int		ApplyElementMove			( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
	int		ApplyElementMoveTarget		( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
	int		ApplyElementGroupMove		( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
	
	int		ApplyElementAttack			( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
	int		ApplyElementAttackArea		( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	int		ApplyElementAttackPenetration	( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
#endif

	int		ApplyElementAvoid			( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );

	int		ApplyElementHealSelf		( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
	int		ApplyElementHealGroup		( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );

	int		ApplyElementSpecialSommon	( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	int		ApplyElementSpecialImmune		( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
	int		ApplyElementNightmareSummon		( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
	int		ApplyElementNightmareWarp		( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
	int		ApplyElementSkillAttack			( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
	int		ApplyElementAIChange			( int iIndex, int iTargetIndex, TMonsterAIState* pAIState );
#endif

	void	Reset();

public:
	char*	GetElementName()			{ return m_szElementName; };

private:
	char	m_szElementName[MAX_AIELEMENTNAME];
	int		m_iElementNumber;
	int		m_iElementClass;
	int		m_iElementType;
	int		m_iSuccessRate;
	int		m_iDelayTime;
	int		m_iTargetType;
	int		m_iX;
	int		m_iY;

};


#endif	// __TMONSTERAIELEMENT_DEF__