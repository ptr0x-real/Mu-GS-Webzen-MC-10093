#ifndef MDATE_H
#define MDATE_H

class MuDate {
public:
	MuDate(void);
	virtual ~MuDate();

	int	GetDay(int year, int month, int date);
	bool	IsLeapYear(const int year);
	int		GetDaysInMonth (const int year, const int month);
	void	GetLastDate(int iInDayOfWeek, int &iYear, int &iMonth, int &iDay);
	int		Compare(int iYear1, int iMonth1, int iDay1, int iHour1, int iYear2, int iMonth2, int iDay2, int iHour2);
};

class MWeeklyTimer : public MuDate {
public:
	typedef	VOID (*PFN_WAKECALLBACK)(MWeeklyTimer *pkMyDate, VOID *pArg);

private:
	int				m_iWakeYear, m_iWakeMonth, m_iWakeDay, m_iWakeHour;
	int				m_iCheckDayOfWeek, m_iCheckHour;
	PFN_WAKECALLBACK	m_pfnWakeCallBack;
	BOOL				m_bActive;
	SYSTEMTIME			m_kOldLocalTime;
	VOID				*m_pArg;

public:
	MWeeklyTimer(void);
	~MWeeklyTimer();

	// Init시에 Off되어 있다.
	void Init(int iWakeYear, int iWakeMonth, int iWakeDay, int iWakeHour, int iTimerDayOfWeek, int iTimerHour, PFN_WAKECALLBACK pfnWakeCallBack, VOID *pArg);
	void Release(VOID);
	void Activate(BOOL bOn);

	void GetWakeDate(int &iOutWakeYear, int &iOutWakeMonth, int &iOutWakeDay, int &iOutWakeHour);

	int  Update(void);	// 리턴 1: 성공, 2: Timer발동 Callback실행.
};

#endif