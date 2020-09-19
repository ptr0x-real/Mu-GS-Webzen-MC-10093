#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

#include "TMonsterAIRuleInfo.h"


TMonsterAIRuleInfo::TMonsterAIRuleInfo()
{
	Reset();
}

void TMonsterAIRuleInfo::Reset()
{
	m_iRuleNumber			= _INVALID;
	m_iMonsterClass			= _INVALID;
	m_iMonsterAIUnit		= _ZERO;

	m_iRuleCondition		= _INVALID;
	m_iWaitTime				= _INVALID;
	m_iContinuanceTime		= _INVALID;

	m_iMonth				= _NONE;
	m_iDay					= _NONE;
	m_iWeekDay				= _NONE;
	m_iHour					= _NONE;
	m_iMinute				= _NONE;

	m_iAppliedTime			= _INVALID;
	m_iApplyingTime			= _INVALID;

	ZeroMemory( m_szRuleDesc, MAX_MONSTERAI_RULEDESC );
}

BOOL TMonsterAIRuleInfo::IsValid()
{
	// Rule Info Condition ���� ��.!

	// 1. ���� ���� �Ǵ�
	BOOL	bIsValid = FALSE;

	switch( m_iRuleCondition )
	{
	case MAR_CONDITION_SPECIFIC_DATE	:	// Ư�� Date �� ���� �Ǵ� AI Rule
		bIsValid = CheckConditionSpecificDate();
		break;
	
	case MAR_CONDITION_CRYWOLF_START	:	// ũ���̿����� ���� �ǰ� ���� �ð� ������ AI Rule 
		bIsValid = CheckConditionCrywolfStart();
		break;

	case MAR_CONDITION_CRYWOLF_END		:	// ũ���̿��� ������ ���� �ð� ������ AI Rule
		bIsValid = CheckConditionCrywolfEnd();
		break;
	}


	// 1.1 ���� ������ ���� ���� ��� Applied, Applying Time �ʱ�ȭ
	if( !bIsValid )
	{
		m_iAppliedTime	= _INVALID;
		m_iApplyingTime	= _INVALID;
		
		return FALSE;
	}

	if( m_iAppliedTime == _INVALID )
		m_iAppliedTime = GetTickCount();


	// 2. ��� �ð� �˻�
	if( m_iAppliedTime + m_iWaitTime*1000 > GetTickCount() )
	{
		return FALSE;
	}


	// 3. ���� �ð� �˻�
	if( m_iAppliedTime + m_iWaitTime*1000 + m_iContinuanceTime*1000 < GetTickCount() )
	{
		return FALSE;
	}

	
	return TRUE;
}


BOOL TMonsterAIRuleInfo::CheckConditionSpecificDate()
{
	CTime		tCurrentTime = CTime::GetCurrentTime();
	CTimeSpan	tValidRange( 0, 0, 0, m_iWaitTime + m_iContinuanceTime );

	int iYear		= tCurrentTime.GetYear();
	int iMonth		= tCurrentTime.GetMonth();
	int iDay		= tCurrentTime.GetDay();
	int iDayOfWeek	= tCurrentTime.GetDayOfWeek();
	int iHour		= tCurrentTime.GetHour();
	int iMinute		= tCurrentTime.GetMinute();


	// 1. Year
	

	// 2. Month
	if( m_iMonth != _NONE )
	{
		if( m_iMonth != iMonth )
		{
			CTime tConditionTime( iYear, m_iMonth, 0, 0, 0, 0 ); 

			if( tCurrentTime < tConditionTime || tCurrentTime > tConditionTime + tValidRange ) 
				return FALSE;
		}
	}

	// 3. Day
	if( m_iDay != _NONE )
	{
		if( m_iDay != iDay )
		{
			CTime tConditionTime( iYear, m_iMonth==_NONE?iMonth:m_iMonth, m_iDay, 0, 0, 0 ); 

			if( tCurrentTime < tConditionTime || tCurrentTime > tConditionTime + tValidRange ) 
				return FALSE;
		}
	}

	// 4. WeekDay
	if( m_iWeekDay != _NONE )
	{
		if( m_iWeekDay != iDayOfWeek )
		{
			return FALSE;
		}
	}

	// 5. Hour
	if( m_iHour != _NONE )
	{
		if( m_iHour != iHour )
		{
			CTime tConditionTime( iYear, m_iMonth==_NONE?iMonth:m_iMonth, m_iDay==_NONE?iDay:m_iDay, m_iHour, 0, 0 ); 

			if( tCurrentTime < tConditionTime || tCurrentTime > tConditionTime + tValidRange ) 
				return FALSE;
		}
	}

	// 6. Minute
	if( m_iMinute != _NONE )
	{
		if( m_iMinute != iMinute )
		{
			CTime tConditionTime( iYear, m_iMonth==_NONE?iMonth:m_iMonth, m_iDay==_NONE?iDay:m_iDay, m_iHour==_NONE?iHour:m_iHour, m_iMinute, 0 ); 

			if( tCurrentTime < tConditionTime || tCurrentTime > tConditionTime + tValidRange ) 
				return FALSE;
		}
	}
	
	
	return TRUE;
}

BOOL TMonsterAIRuleInfo::CheckConditionCrywolfStart()
{
	// ũ���̿����� ���� �Ǹ� AI Rule�� �ٲ��.
	// �� ũ���̿������� static ������ �̿��� üũ�Ѵ�.

	// or g_Crywolf ���� ���°��� �д´�.. �̷������� �ϴ°� ���ڳ�.
	// �ٸ� �͵鵵.. ���� ���� ����..
	return FALSE;
}

BOOL TMonsterAIRuleInfo::CheckConditionCrywolfEnd()
{
	return FALSE;
}
	
#endif