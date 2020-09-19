#if !defined(__CRYWOLFSTATUE_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define	__CRYWOLFSTATUE_DEF__

// 0. > 방어막을 관리 한다.

#include "CrywolfDefine.h"

#define MIN_PRIEST_COUNT_TO_CREATE_SHIELD	1

enum CRYWOLF_STATUE_SHIELD_STATE
{
	CRYWOLF_SHILED_STATE_NONE		= 0,	// 방어막 해제 상태
	CRYWOLF_SHILED_STATE_CREATED	= 1,	// 방어막 생성 상태
};

class CCrywolfShield
{
public:
	CCrywolfShield()
	{
		Reset();
	}

	void Reset()
	{
		m_iPriestNumber = _ZERO;

		m_iShieldHP		= _ZERO;
		m_iShieldMaxHP	= _ZERO;
		m_iShieldState	= CRYWOLF_SHILED_STATE_NONE;
	}

	int GetHPPercentage()
	{
		if( m_iShieldMaxHP != 0 )
		{
			return m_iShieldHP*100 / m_iShieldMaxHP;
		}
		else
		{
			return 0;
		}
	}

public:
	int	m_iShieldMaxHP;		// 방어막 최대 HP
	int m_iShieldHP;		// 방어막 HP
	int	m_iShieldState;		// 방어막 상태
	int m_iPriestNumber;	// 사제의 명수
};

class CCrywolfStatue
{

public:
	CCrywolfStatue();
	virtual ~CCrywolfStatue();

public:
	void CrywolfStatueAct( int iIndex );

	int	 GetStatueViewState( int iPriestNumber );
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	void SetStatueViewState( LPOBJECTSTRUCT lpObj, int iPriestNumber );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
public:
	CCrywolfShield	m_Shield;

};

extern CCrywolfStatue	g_CrywolfNPC_Statue;

#endif