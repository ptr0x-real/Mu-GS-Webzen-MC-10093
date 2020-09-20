#if !defined(__CRYWOLFSYNC_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLFSYNC_DEF__

// 1. ũ���̿��� ������ �Ϲ� �������� ó�� ����ȭ�� �Ѵ�.

// 2. ũ���̿��� ����/���п� ���� ���� ó�� ����� �� ���� ���⸦ ����Ѵ�.

// 3. DB �������� �����ϴ� ������ ������. -> ������ SP �̿�!!!

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
	
	// �� �����, ī���������� ���շ� ����� Commonserver.cfg ���� �ε��Ѵ�!
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
