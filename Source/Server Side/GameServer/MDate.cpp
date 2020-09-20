#include "stdafx.h"
#include "MDate.h"

//--------------------------------------------------

MuDate::MuDate(void)
{
}

//--------------------------------------------------

MuDate::~MuDate()
{
}

//--------------------------------------------------
//-- 무슨 요일
int MuDate::GetDay(int year, int month, int date)
{
	int year1;
	int year2;
	int day;

	if(month < 3)
	{
		year--;
		month += 12;
	}

	year1 = year / 100;
	year2 = year % 100;

	day = 21*year1/4 + 5*year2/4 + 26*(month+1)/10 + date - 1;
	day %= 7;

	return(day);
}

//--------------------------------------------------

bool MuDate::IsLeapYear(const int year)
{
   return ((year%400==0) || (year%4==0 && year%100!=0));
}

//--------------------------------------------------
 
//-- 한달은 몇일?
int MuDate::GetDaysInMonth (const int year, const int month)
{
	switch (month)
	{
		case 2: return IsLeapYear(year) ? 29 : 28;
		case 4:
		case 6:
		case 9:
		case 11: return 30;
		default: return 31;
	}   
}

//--------------------------------------------------
//-- 해당일을 포함한 과거 시간 중에 마지막 요일의 날짜를 구한다.
void MuDate::GetLastDate(int iInDayOfWeek, int &iYear, int &iMonth, int &iDay)
{
	int wWeekOfDay;
	int minusday;

	wWeekOfDay = GetDay(iYear, iMonth, iDay);
	minusday = ((wWeekOfDay + 7) - iInDayOfWeek) % 7;

	if(iDay > minusday)
	{
		iDay -= minusday;
	}
	else
	{
		minusday -= iDay;
		if(iMonth == 1)
		{
			iMonth = 12;
			iYear--;
		}
		else
		{
			iMonth--;
		}
		iDay = GetDaysInMonth(iYear, iMonth) - minusday;
	}
}

//--------------------------------------------------

int MuDate::Compare(int iYear1, int iMonth1, int iDay1, int iHour1, int iYear2, int iMonth2, int iDay2, int iHour2)
{
/*
	if((iYear1 == iYear2) && (iMonth1 == iMonth2) && (iDay1 == iDay2))
	{
		return 0;
	}

	if( ((iYear1 == iYear2) && (iMonth1 == iMonth2) && (iDay1 > iDay2)) ||
		((iYear1 == iYear2) && (iMonth1 > iMonth2)) ||
		((iYear1 > iYear2)) )
	{
		return 1;
	}

	return(-1);
*/

	if((iYear1 == iYear2) && (iMonth1 == iMonth2) && (iDay1 == iDay2) && (iHour1 == iHour2))
	{
		return 0;
	}

	if( ((iYear1 == iYear2) && (iMonth1 == iMonth2) && (iDay1 == iDay2) && (iHour1 > iHour2)) ||
		((iYear1 == iYear2) && (iMonth1 == iMonth2) && (iDay1 > iDay2)) ||
		((iYear1 == iYear2) && (iMonth1 > iMonth2)) ||
		((iYear1 > iYear2)) )
	{
		return 1;
	}

	return(-1);
}

//--------------------------------------------------

MWeeklyTimer::MWeeklyTimer(void)
:m_iWakeYear(2000), m_iWakeMonth(1), m_iWakeDay(1), m_iCheckDayOfWeek(0), m_pfnWakeCallBack(NULL), m_bActive(FALSE), m_pArg(NULL)
{
}

//--------------------------------------------------

MWeeklyTimer::~MWeeklyTimer()
{
}

//--------------------------------------------------

void MWeeklyTimer::Init(int iWakeYear, int iWakeMonth, int iWakeDay, int iWakeHour, int iTimerDayOfWeek, int iTimerHour, PFN_WAKECALLBACK pfnWakeCallBack, VOID *pArg)
{
	m_iWakeYear		= iWakeYear;
	m_iWakeMonth	= iWakeMonth;
	m_iWakeDay		= iWakeDay;
	m_iWakeHour		= iWakeHour;

	m_iCheckDayOfWeek	= iTimerDayOfWeek;
	m_iCheckHour		= iTimerHour;
	m_pfnWakeCallBack	= pfnWakeCallBack;
	m_pArg				= pArg;
}

//--------------------------------------------------

void MWeeklyTimer::Release(VOID)
{
	Activate(FALSE);
}

//--------------------------------------------------

int MWeeklyTimer::Update(void)
{
	int iYear, iMonth, iDay, iHour;
	SYSTEMTIME	m_kLocalTime;

	if(m_bActive == FALSE) return(1);

	GetLocalTime(&m_kLocalTime);

	if( (m_kOldLocalTime.wYear == m_kLocalTime.wYear)	&&
		(m_kOldLocalTime.wMonth == m_kLocalTime.wMonth)	&&
		(m_kOldLocalTime.wDay == m_kLocalTime.wDay)	&&
		(m_kOldLocalTime.wHour == m_kLocalTime.wHour)
		)
		return(1);

	m_kOldLocalTime = m_kLocalTime;

	iYear	= m_kLocalTime.wYear;
	iMonth	= m_kLocalTime.wMonth;
	iDay	= m_kLocalTime.wDay;
	iHour	= m_iCheckHour;

	if(m_iCheckHour > m_kLocalTime.wHour) iDay--;

	GetLastDate(m_iCheckDayOfWeek, iYear, iMonth, iDay);

	if(Compare(m_iWakeYear, m_iWakeMonth, m_iWakeDay, m_iWakeHour, iYear, iMonth, iDay, iHour) < 0)
	{
		m_iWakeYear		= m_kLocalTime.wYear;
		m_iWakeMonth	= m_kLocalTime.wMonth;
		m_iWakeDay		= m_kLocalTime.wDay;
		m_iWakeHour		= m_kLocalTime.wHour;
		m_pfnWakeCallBack(this, m_pArg);
//		printf("%d/%d/%d\n", m_iWakeYear, m_iWakeMonth, m_iWakeDay);
		return(2);
	}

	return(1);
}

//--------------------------------------------------

void MWeeklyTimer::Activate(BOOL bOn)
{
	m_bActive = bOn;
}

//--------------------------------------------------

void MWeeklyTimer::GetWakeDate(int &iOutWakeYear, int &iOutWakeMonth, int &iOutWakeDay, int &iOutWakeHour)
{
	iOutWakeYear	= m_iWakeYear;
	iOutWakeMonth	= m_iWakeMonth;
	iOutWakeDay		= m_iWakeDay;
	iOutWakeHour	= m_iWakeHour;
}

//--------------------------------------------------
