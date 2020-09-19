#include "Stdafx.h"

#ifdef CRYWOLF_COMMON_PATCH_20050413

#include "CrywolfStateTimeInfo.h"


int	CCrywolfStateTimeInfo::GetLeftTime()
{
	if( CheckContinuanceTime() )
	{
		int iLeftTime = m_iContinuanceTime - (GetTickCount()-m_dwAppliedTickCount);

		if( iLeftTime < 0  )
			iLeftTime = 0;

		return iLeftTime;
	}
	
	return 0;
}

BOOL CCrywolfStateTimeInfo::CheckContinuanceTime()
{
	if( GetTickCount() - m_dwAppliedTickCount >  m_iContinuanceTime )
		return FALSE;

	return TRUE;
}

BOOL CCrywolfStateTimeInfo::CheckScheduleTime()
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	int iYear			= tCurrentTime.GetYear();
	int iMonth			= tCurrentTime.GetMonth();
	int iDay			= tCurrentTime.GetDay();
	int iDayOfWeek		= tCurrentTime.GetDayOfWeek();
	int iHour			= tCurrentTime.GetHour();
	int iMinute			= tCurrentTime.GetMinute();

	int	iCrywolfState	= _INVALID;

	CTimeSpan	tTimeRange( 0, 0, m_iContinuanceTime, 0);

	// 1. Year
	
	
	// 2. Month
	if( m_iMonth != _NONE )
	{
		if( m_iMonth != iMonth )
		{
			CTime tConditionTime( iYear, m_iMonth, 0, 0, 0, 0 ); 
			
			if( tCurrentTime < tConditionTime || tCurrentTime > tConditionTime + tTimeRange ) 
				return FALSE;
		}
	}
	// 3. Day
	if( m_iDay != _NONE )
	{
		if( m_iDay != iDay )
		{
			CTime tConditionTime( iYear, m_iMonth==_NONE?iMonth:m_iMonth, m_iDay, 0, 0, 0 ); 
			
			if( tCurrentTime < tConditionTime || tCurrentTime > tConditionTime + tTimeRange ) 
				return FALSE;
		}
	}
	// 4. DayOfWeek
	if( m_iDayOfWeek != _NONE )
	{
		// 주의!! - 다른 시간 필드들이 설정 되어 있어야 ContinuanceTime 이 적용된다.
		if( m_iDayOfWeek != iDayOfWeek )
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
			
			if( tCurrentTime < tConditionTime || tCurrentTime > tConditionTime + tTimeRange ) 
				return FALSE;
		}
	}
	// 6. Minute
	if( m_iMinute != _NONE )
	{
		if( m_iMinute != iMinute )
		{
			CTime tConditionTime( iYear, m_iMonth==_NONE?iMonth:m_iMonth, m_iDay==_NONE?iDay:m_iDay, m_iHour==_NONE?iHour:m_iHour, m_iMinute, 0 ); 
			
			if( tCurrentTime < tConditionTime || tCurrentTime > tConditionTime + tTimeRange ) 
				return FALSE;
		}
	}

	return TRUE;
}
#endif