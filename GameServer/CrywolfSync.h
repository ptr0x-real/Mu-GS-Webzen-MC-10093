#if !defined(__CRYWOLFSYNC_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLFSYNC_DEF__

// 1. 크라이울프 서버와 일반 서버간에 처리 동기화를 한다.

// 2. 크라이울프 성공/실패에 따른 보상 처리 등등의 전 서버 동기를 담당한다.

// 3. DB 서버에서 폴링하는 구조를 따른다. -> 무조건 SP 이용!!!

#include "CrywolfDefine.h"

class CCrywolfSync
{

public:
	CCrywolfSync();
	virtual ~CCrywolfSync();

public:
	BOOL	CheckEnableCrywolf();
	void	SetEnableCrywolf( BOOL bEnable ); 
	
	int		GetOccupationState();
	int		GetCrywolfState();
	
	void	SetOccupationState( int iOccupationState ); 
	void	SetCrywolfState( int iCrywolfState );
	
	// 석 드랍률, 카오스아이템 조합률 등등은 Commonserver.cfg 에서 로드한다!
	int		GetPlusChaosRate();
	void	SetPlusChaosRate( int iPlusChaosRate );

	int		GetGemDropPenaltiyRate();
	void	SetGemDropPenaltiyRate( int iGemDropPenaltyRate );
	
	int		GetGettingExpPenaltyRate();
	void	SetGettingExpPenaltyRate( int iGettingExpPenaltyRate );

	int		GetMonHPBenefitRate();
	void	SetMonHPBenefitRate( int iMinusMonHPBenefitRate );
	
#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
	int		GetKundunHPRefillState();
	void	SetKundunHPRefillState( int iKundunHPRefillState );
#endif
	
private:
	BOOL	m_bCrywolfEnable;
	BOOL	m_bApplyBenefit;
	BOOL	m_bApplyPenalty;
	
	int		m_iCrywolfState;
	int		m_iOccupationState;

	int		m_iPlusChaosRate;
	int		m_iGemDropPenaltyRate;
	int		m_iGettingExpPenaltyRate;

	int		m_iMinusMonHPBenefitRate;

#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
	int		m_iKundunHPRefillState;
#endif

};

extern CCrywolfSync	g_CrywolfSync;

#endif 
