#if !defined(__TMONSTERAISTATE_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAISTATE_DEF__

#define MAX_MONSTERAI_STATE_TRANS_DESC			100
#define MAX_MONSTERAI_STATE_TRANSITION_INFO		30

// Transition Type - ���� ���� Ÿ��
//  
enum MONSTER_AI_STATE_TRANSITION_TYPE
{
	MAI_STATE_TRANS_NO_ENEMY		= 0,		// �� ����
	MAI_STATE_TRANS_IN_ENEMY		= 1,		// ���� ���� �� �� �߰�	
	MAI_STATE_TRANS_OUT_ENEMY		= 2,		// ���� ���� �� �� �߰�
	MAI_STATE_TRANS_DIE_ENEMY		= 3,		// �� óġ
	MAI_STATE_TRANS_DEC_HP			= 4,		// HP ����

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	MAI_STATE_TRANS_DEC_HP_PER		= 5,		// HP % ����
	MAI_STATE_TRANS_IMMEDIATELY		= 6,		// ���Ǿ��� ������ ����
#endif
	
	MAI_STATE_TRANS_AGRO_UP			= 7,		// Agro ��ġ�� Ư�� �� �̻� �� �� ���� ����
	MAI_STATE_TRANS_AGRO_DOWN		= 8,		// Agro ��ġ�� Ư�� �� ���� �� �� ���� ����
	
	MAI_STATE_TRANS_GROUP_SOMMON	= 10,		// �׷� ����� ��ȯ�� �ʿ��� �� ���� ����
	MAI_STATE_TRANS_GROUP_HEAL		= 11,		// �׷� ����� ġ�ᰡ �ʿ��� �� ���� ����
};

// Transition Value Type - ���� ���̸� �Ǵ� �ϱ� ���� �� Ÿ��
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