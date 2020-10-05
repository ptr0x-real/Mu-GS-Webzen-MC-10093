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
	MAE_STATE_NORMAL				= 0,		// 일반 행동 속성
	MAE_STATE_MOVE					= 1,		// 이동 행동 속성
	MAE_STATE_ATTACK				= 2,		// 공격 행동 속성
	MAE_STATE_HEAL					= 3,		// 치료 행동 속성
	MAE_STATE_AVOID					= 4,		// 회피 행동 속성
	MAE_STATE_HELP					= 5,		// 도움 행동 속성
	MAE_STATE_SPECIAL				= 6,		// 특수 행동 속성
	MAE_STATE_EVENT					= 7,		// 이벤트 행동 속성 ( 시나리오모드 )
};

enum MONSTER_AI_ELEMENT_TYPE
{
	MAE_TYPE_COMMON_NORMAL			= 1,		// 일반 - 일반

	MAE_TYPE_MOVE_NORMAL			= 11,		// 이동 - 일반
	MAE_TYPE_MOVE_TARGET			= 12,		// 이동 - 특정 타겟에게 이동
	MAE_TYPE_GROUP_MOVE				= 13,		// 그룹 이동 - 일반
	MAE_TYPE_GROUP_MOVE_TARGET		= 14,		// 그룹 이동 - 타겟
	
	MAE_TYPE_ATTACK_NORMAL			= 21,		// 공격 - 일반 공격
	MAE_TYPE_ATTACK_AREA			= 22,		// 그룹 공격 - 일반 공격

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	MAE_TYPE_ATTACK_PENETRATION		= 23,		// 공격 - 관통
#endif
	
	MAE_TYPE_HEAL_SELF				= 31,		// 치료 - 자신 치료
	MAE_TYPE_HEAL_GROUP				= 32,		// 치료 - 그룹 치료
	
	MAE_TYPE_AVOID_NORMAL			= 41,		// 회피

	MAE_TYPE_HELP_HP				= 51,		// 도움 - HP 회복
	MAE_TYPE_HELP_BUFF				= 52,		// 도움 - 특정 버프
	MAE_TYPE_HELP_TARGET			= 53,		// 도움 - 타겟 공유

	MAE_TYPE_SPECIAL				= 61,		// 특수
	MAE_TYPE_SPECIAL_SOMMON			= 62,		// 특수 - 소환

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	MAE_TYPE_SPECIAL_IMMUNE			= 64,		// 특수 - 무적( 공격 데미지를 받지 않음 )

	MAE_TYPE_SPECIAL_NIGHTMARE_SUMMON = 65,		// 특수 - 나이트메어 소환(종속적)
	MAE_TYPE_SPECIAL_WARP			  = 66,		// 워프 - (나이트메어 워프. 범용적)
	MAE_TYPE_SPECIAL_SKILLATTACK	  = 67,		// 스킬 공격 패턴 (확률로 조절)
	MAE_TYPE_SPECIAL_CHANGEAI		  = 68,		// AI Unit 변경
#endif
	
	MAE_TYPE_EVENT					= 71,		// 이벤트
};

enum MONSTER_AI_ELELMENT_TARGET_TYPE
{
	MAE_TARGET_TYPE_CHARACTER		= 1,		// 캐릭터
	MAE_TARGET_TYPE_XY				= 2,		// 좌표
	MAE_TARGET_TYPE_MONSTER			= 3,		// 몬스터 - 몬스터 별로 적대관계 있을 경우 추가		
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