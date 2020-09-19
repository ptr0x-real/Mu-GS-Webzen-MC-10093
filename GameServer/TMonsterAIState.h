#if !defined(__TMONSTERAISTATE_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAISTATE_DEF__

#define MAX_MONSTERAI_STATE_TRANS_DESC			100
#define MAX_MONSTERAI_STATE_TRANSITION_INFO		30

// Transition Type - 상태 전이 타입
//  
enum MONSTER_AI_STATE_TRANSITION_TYPE
{
	MAI_STATE_TRANS_NO_ENEMY		= 0,		// 적 없음
	MAI_STATE_TRANS_IN_ENEMY		= 1,		// 공격 범위 안 적 발견	
	MAI_STATE_TRANS_OUT_ENEMY		= 2,		// 공격 범위 밖 적 발견
	MAI_STATE_TRANS_DIE_ENEMY		= 3,		// 적 처치
	MAI_STATE_TRANS_DEC_HP			= 4,		// HP 감소

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	MAI_STATE_TRANS_DEC_HP_PER		= 5,		// HP % 감소
	MAI_STATE_TRANS_IMMEDIATELY		= 6,		// 조건없이 무조건 전이
#endif
	
	MAI_STATE_TRANS_AGRO_UP			= 7,		// Agro 수치가 특정 값 이상 일 때 상태 전이
	MAI_STATE_TRANS_AGRO_DOWN		= 8,		// Agro 수치가 특정 값 이하 일 때 상태 전이
	
	MAI_STATE_TRANS_GROUP_SOMMON	= 10,		// 그룹 멤버의 소환이 필요할 때 상태 전이
	MAI_STATE_TRANS_GROUP_HEAL		= 11,		// 그룹 멤버의 치료가 필요할 때 상태 전이
};

// Transition Value Type - 상태 전이를 판단 하기 위한 값 타입
// 
enum MONSTER_AI_STATE_TRANSITION_VALUE_TYPE
{
	MAI_STATE_TRANS_VALUE_		= 0,
};

class TMonsterAIState
{

public:
	TMonsterAIState()
	{
			Reset();
	}

	void	Reset()
	{
			m_iPriority				= _INVALID;
			m_iCurrentState			= _INVALID;
			m_iNextState			= _INVALID;
			m_iTransitionType		= _INVALID;
			m_iTransitionRate		= _INVALID;
			m_iTransitionValueType	= _INVALID;
			m_iTransitionValue		= _INVALID;
			m_iDelayTime			= _INVALID;

			ZeroMemory( m_szStateTransitionDesc, MAX_MONSTERAI_STATE_TRANS_DESC );
	}

public:
	int		m_iPriority;
	int		m_iCurrentState;
	int		m_iNextState;
	int		m_iTransitionType;
	int		m_iTransitionRate;
	int		m_iTransitionValueType;
	int		m_iTransitionValue;
	int		m_iDelayTime;

	char	m_szStateTransitionDesc[MAX_MONSTERAI_STATE_TRANS_DESC];

};

#endif