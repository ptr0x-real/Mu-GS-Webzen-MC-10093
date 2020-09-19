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
	// Rule Info Condition 별로 비교.!

	// 1. 적용 조건 판단
	BOOL	bIsValid = FALSE;

	switch( m_iRuleCondition )
	{
	case MAR_CONDITION_SPECIFIC_DATE	:	// 특정 Date 에 적용 되는 AI Rule
		bIsValid = CheckConditionSpecificDate();
		break;
	
	case MAR_CONDITION_CRYWOLF_START	:	// 크라이울프가 시작 되고 일정 시간 동안의 AI Rule 
		bIsValid = CheckConditionCrywolfStart();
		break;

	case MAR_CONDITION_CRYWOLF_END		:	// 크라이울프 끝나고 일정 시간 동안의 AI Rule
		bIsValid = CheckConditionCrywolfEnd();
		break;
	}


	// 1.1 적용 조건이 맞지 않을 경우 Applied, Applying Time 초기화
	if( !bIsValid )
	{
		m_iAppliedTime	= _INVALID;
		m_iApplyingTime	= _INVALID;
		
		return FALSE;
	}

	if( m_iAppliedTime == _INVALID )
		m_iAppliedTime = GetTickCount();


	// 2. 대기 시간 검사
	if( m_iAppliedTime + m_iWaitTime*1000 > GetTickCount() )
	{
		return FALSE;
	}


	// 3. 지속 시간 검사
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
	// 크라이울프가 시작 되면 AI Rule이 바뀐다.
	// 이 크라이울프값은 static 변수를 이용해 체크한다.

	// or g_Crywolf 에서 상태값을 읽는다.. 이런식으로 하는게 낫겠네.
	// 다른 것들도.. 일케 구현 하자..
	return FALSE;
}

BOOL TMonsterAIRuleInfo::CheckConditionCrywolfEnd()
{
	return FALSE;
}
	
#endif