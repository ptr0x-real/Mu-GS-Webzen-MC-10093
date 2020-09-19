#if !defined(__CRYWOLFSTATETIMEINFO_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLFSTATETIMEINFO_DEF__

class CCrywolfStateTimeInfo
{
	
public:
	CCrywolfStateTimeInfo()
	{
		Reset();
	}
	
	void	Reset()
	{
		m_bUsed				= FALSE;
		
		m_iStateNumber		= _NONE;
		m_iYear				= _NONE;
		m_iMonth			= _NONE;
		m_iDay				= _NONE;
		m_iDayOfWeek		= _NONE;
		m_iHour				= _NONE;
		m_iMinute			= _NONE;
		m_iSecond			= _NONE;
		m_iContinuanceTime	= _ZERO;

		m_dwAppliedTickCount= _ZERO;
	}

	void	SetAppliedTime()
	{ 
		m_dwAppliedTickCount = GetTickCount(); 
	}

	void	ResetAppliedTime()
	{ 
		m_dwAppliedTickCount = _ZERO; 
	} 

	BOOL	CheckContinuanceTime();
	BOOL	CheckScheduleTime();
	
	int		GetLeftTime();

public:
	BOOL	m_bUsed;

	int		m_iStateNumber;
	int		m_iYear;
	int		m_iMonth;
	int		m_iDay;
	int		m_iDayOfWeek;
	int		m_iHour;
	int		m_iMinute;
	int		m_iSecond;
	int		m_iContinuanceTime;

	DWORD	m_dwAppliedTickCount;
	
};


#endif