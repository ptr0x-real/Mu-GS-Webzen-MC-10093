#include "Stdafx.h"

#ifdef CRYWOLF_COMMON_PATCH_20050413

#include "CrywolfSync.h"

CCrywolfSync	g_CrywolfSync;

CCrywolfSync::CCrywolfSync()
{
	m_bCrywolfEnable			= TRUE;

	m_bApplyBenefit				= FALSE;
	m_bApplyPenalty				= FALSE;

	m_iOccupationState			= CRYWOLF_OCCUPATION_STATE_OCCUPIED;
	m_iCrywolfState				= CRYWOLF_STATE_NONE;
	
	m_iPlusChaosRate			= _ZERO;

	m_iGemDropPenaltyRate		= 100;
	m_iGettingExpPenaltyRate	= 100;

	m_iMinusMonHPBenefitRate	= 100;

#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
	m_iKundunHPRefillState		= CRYWOLF_BENEFIT_KUNDUN_HP_REFILL_OFF;
#endif
}

CCrywolfSync::~CCrywolfSync()
{

}

BOOL CCrywolfSync::CheckEnableCrywolf()
{ 
	return m_bCrywolfEnable; 
}

void CCrywolfSync::SetEnableCrywolf( BOOL bEnable ) 
{ 
	m_bCrywolfEnable = bEnable; 
}

int	 CCrywolfSync::GetOccupationState()
{ 
	return m_iOccupationState; 
}

int	 CCrywolfSync::GetCrywolfState()
{ 
	return m_iCrywolfState; 
}

void CCrywolfSync::SetOccupationState( int iOccupationState ) 
{ 
	m_iOccupationState = iOccupationState;	
}

void CCrywolfSync::SetCrywolfState( int iCrywolfState ) 
{ 
	m_iCrywolfState = iCrywolfState;	
}

int	 CCrywolfSync::GetPlusChaosRate()
{
	return m_iPlusChaosRate;
}

void CCrywolfSync::SetPlusChaosRate( int iPlusChaosRate )
{
	m_iPlusChaosRate = iPlusChaosRate;
}

int	 CCrywolfSync::GetGemDropPenaltiyRate()
{
	return m_iGemDropPenaltyRate;
}

void CCrywolfSync::SetGemDropPenaltiyRate( int iGemDropPenaltyRate )
{
	m_iGemDropPenaltyRate = iGemDropPenaltyRate;
}
int	 CCrywolfSync::GetGettingExpPenaltyRate()
{
	return m_iGettingExpPenaltyRate;
}
void CCrywolfSync::SetGettingExpPenaltyRate( int iGettingExpPenaltyRate )
{
	m_iGettingExpPenaltyRate = iGettingExpPenaltyRate;
}

int	 CCrywolfSync::GetMonHPBenefitRate()
{
	return m_iMinusMonHPBenefitRate;
}

void CCrywolfSync::SetMonHPBenefitRate( int iMinusMonHPBenefitRate )
{
	m_iMinusMonHPBenefitRate = iMinusMonHPBenefitRate;
}

#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
int	CCrywolfSync::GetKundunHPRefillState()
{
	return m_iKundunHPRefillState;
}

void CCrywolfSync::SetKundunHPRefillState( int iKundunHPRefillState )
{
	m_iKundunHPRefillState = iKundunHPRefillState;
}
#endif // MODIFY_CRYWOLF_BENEFIT_01_20060315

#endif