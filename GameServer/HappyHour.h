#if !defined( __HAPPYHOUR_H__) && defined(__DAKA_CUSTOM)
#define __HAPPYHOUR_H__

#include <vector>
#include <atltime.h>

enum
{
	DAY_OF_MONTH_METHOD,
	DAY_OF_WEEK_METHOD,
};

enum
{
	EXP_TYPE_ABSOLUTE,
	EXP_TYPE_ADDITION,
};

enum
{
	EXP_FORMAT_ABSOLUTE,
	EXP_FORMAT_PERCENT,
};

enum
{
	HH_START_MESSAGE = 0,
	HH_END_MESSAGE,
	HH_TIME_MESSAGE,
	HH_TIME_END_MESSAGE,
	HH_MAX_MESSAGES,
	TIME_TO_INIT = 5,
};

struct HAPPY_HOUR_CONFIG
{
	struct T_TIME
	{
		int Month, Day, Hour, Minutes;
		bool DayOfWeek;
		int Duration;
	} Time;

	char ExperienceType;
	char ExperienceFormat;
	float Experience;
	bool init;
	CTime time_start;
	CTime time_end;
};

typedef std::vector<HAPPY_HOUR_CONFIG> HAPPY_HOUR_VECTOR;

class CHappyHour
{
public:
	CHappyHour();

	void Load(const char *filename="..\\Data\\HappyHour.dat");

	void Process();

private:
	void ProcessExperience();

	void Clear();

	int GetDayOfWeek(int Day, int DayOfWeek)
	{
		int day = DayOfWeek-Day;

		if(day < 0)
			day = day+7;

		return day;
	}

	HAPPY_HOUR_VECTOR	m_configList;

	float				m_defaultExperience;
	HAPPY_HOUR_CONFIG	*m_usedConfig;
	CTime				m_msgTime;
	CTimeSpan			m_msgTimeSpan;

	std::string			m_messages[HH_MAX_MESSAGES];

	bool				m_enable;
	bool				m_init;
};

extern CHappyHour g_HappyHour;

#endif