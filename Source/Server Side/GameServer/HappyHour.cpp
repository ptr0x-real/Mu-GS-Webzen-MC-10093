#include "StdAfx.h"
#include "HappyHour.h"
#include "GameMain.h"
#include "Include\ReadScript.h"

#include "TNotice.h"

#ifdef __HAPPYHOUR_H__

CHappyHour g_HappyHour;

CHappyHour::CHappyHour() : m_defaultExperience(0.0f), m_usedConfig(NULL), m_enable(false), m_init(false)
{
	this->m_configList.clear();
}

void CHappyHour::Load(const char *filename)
{
	this->Clear();

	this->m_configList.clear();

	SMDToken Token;

	SMDFile = fopen(filename,"r");

	if(SMDFile == NULL) return;

	GetToken();
	this->m_enable = TokenNumber > 0;

	GetToken();
	this->m_messages[HH_START_MESSAGE] = TokenString;

	GetToken();
	this->m_messages[HH_END_MESSAGE] = TokenString;

	GetToken();
	this->m_messages[HH_TIME_MESSAGE] = TokenString;

	GetToken();
	this->m_messages[HH_TIME_END_MESSAGE] = TokenString;

	while(true)
	{
		Token = GetToken();

		if(Token != SMDToken::NUMBER)
			break;
		else if(Token == SMDToken::NUMBER)
		{
			HAPPY_HOUR_CONFIG config;

			config.ExperienceType = TokenNumber;

			GetToken();
			config.ExperienceFormat = TokenNumber;

			GetToken();
			config.Experience = TokenNumber;

			GetToken();
			config.Time.Month = TokenNumber;

			GetToken();
			config.Time.DayOfWeek = TokenNumber > 0;

			GetToken();

			if(config.Time.DayOfWeek)
			{
				config.Time.Day = (((int)TokenNumber)%7)+1;

				if(config.Time.Day == 8)
					config.Time.Day = 1;
			}
			else
				config.Time.Day = TokenNumber;

			GetToken();
			config.Time.Hour = TokenNumber;

			GetToken();
			config.Time.Minutes = TokenNumber;

			GetToken();
			config.Time.Duration = TokenNumber;

			config.init = false;

			this->m_configList.push_back(config);
		}
	};

	fclose(SMDFile);
	LogAdd("[Happy Hour] %s file load!", filename);
}

void CHappyHour::Process()
{
	if(this->m_enable)
	{
		CTime current_time = CTime::GetCurrentTime();

		if(this->m_usedConfig == NULL)
		{
			for(HAPPY_HOUR_VECTOR::iterator it = this->m_configList.begin();it != this->m_configList.end();++it)
			{
				HAPPY_HOUR_CONFIG &config = (*it);

				CTime time_start, time_end;

				if(config.init == false)
				{
					if(config.Time.DayOfWeek)
					{
						time_start = CTime(current_time.GetYear(),
							(config.Time.Month != -1 ? config.Time.Month : current_time.GetMonth()),
							(current_time.GetDayOfWeek() == config.Time.Day ? current_time.GetDay() : current_time.GetDay()+GetDayOfWeek(current_time.GetDayOfWeek(), config.Time.Day) ),
							(config.Time.Hour != -1 ? config.Time.Hour : current_time.GetHour()),
							(config.Time.Minutes != -1 ? config.Time.Minutes : current_time.GetMinute()),
							0);
					}
					else
					{
						time_start = CTime(current_time.GetYear(),
							(config.Time.Month != -1 ? config.Time.Month : current_time.GetMonth()),
							(config.Time.Day != -1 ? config.Time.Day : current_time.GetDay()),
							(config.Time.Hour != -1 ? config.Time.Hour : current_time.GetHour()),
							(config.Time.Minutes != -1 ? config.Time.Minutes : current_time.GetMinute()),
							0);
					}

					time_end = time_start;
					time_end += CTimeSpan(0, 0, config.Time.Duration+TIME_TO_INIT, 0);
				}
				else
				{
					time_start = config.time_start;
					time_end = config.time_end;
				}

				if(current_time >= time_start && current_time <= time_end)
				{
					LogAddTD("[Happy Hour] Event begin");
					config.time_start = time_start;
					config.time_end = time_end;
					this->m_msgTime = time_end-CTimeSpan(0, 0, config.Time.Duration+TIME_TO_INIT, 0);
					this->m_msgTimeSpan = CTimeSpan(0, 0, 0, 0);
					this->m_usedConfig = &config;
					this->m_defaultExperience = gAddExperience;
					this->m_init = false;

					break;
				}
				else if(config.init == false || current_time > time_start)
				{
					config.init = true;

					if(current_time > time_start)
					{
						int time[5];
						time[0] = config.Time.Minutes, time[1] = config.Time.Hour, time[2] = config.Time.Day, time[3] = config.Time.Month, time[4] = current_time.GetYear();

						for(int i = 3;i >= 0;i--)
						{
							if(time[i] != -1)
							{
								if(i <= 2)
								{
									switch(i)
									{
									case 0:
										time[i+1] = current_time.GetHour()+1;
										break;
									case 1:
										time[i+1] = current_time.GetDay()+1;
										break;
									case 2:
										time[i+1] = current_time.GetMonth()+1;
										break;
									case 3:
										time[i+1]++;
										break;
									}
								}
								break;
							}
						}

						for(int i = 3;i >= 0;i--)
						{
							if(time[i] == -1)
							{
								switch(i)
								{
								case 0:
									time[i] = current_time.GetMinute();
									break;
								case 1:
									time[i] = current_time.GetHour();
									break;
								case 2:
									time[i] = current_time.GetDay();
									break;
								case 3:
									time[i] = current_time.GetMonth();
									break;
								}
							}
						}

						if(config.Time.DayOfWeek)
						{
							config.time_start = CTime(time[4],
							time[3],
							current_time.GetDay()+GetDayOfWeek(current_time.GetDayOfWeek(), config.Time.Day),
							time[1],
							time[0],
							0);
						}
						else
						{
							config.time_start = CTime(time[4],
							time[3],
							time[2],
							time[1],
							time[0],
							0);
						}

						config.time_end = config.time_start;
						config.time_end += CTimeSpan(0, 0, config.Time.Duration+TIME_TO_INIT, 0);
					}
					else
					{
						config.time_start = time_start;
						config.time_end = time_end;
					}
				}
			}
		}
		else
		{
			TNotice notice;

			if(this->m_init == false)
			{
				CTimeSpan time_changed = CTimeSpan(CTime::GetCurrentTime().GetTime()) - CTimeSpan(this->m_msgTime.GetTime());

				if(time_changed.GetTotalMinutes() >= TIME_TO_INIT)
					this->m_msgTimeSpan = time_changed+CTimeSpan(0, 0, 1, 0);

				if(time_changed.GetTotalMinutes() < TIME_TO_INIT && this->m_msgTimeSpan.GetTotalMinutes() <= time_changed.GetTotalMinutes())
				{
					this->m_msgTimeSpan = time_changed+CTimeSpan(0, 0, 1, 0);

					notice.SendToAllUser((char*)this->m_messages[HH_TIME_MESSAGE].c_str(), TIME_TO_INIT-time_changed.GetTotalMinutes());
				}
				else if(time_changed.GetTotalMinutes() >= TIME_TO_INIT)
				{
					this->m_msgTimeSpan = CTimeSpan(0, 0, 0, 0);
					this->m_init = true;

					notice.SendToAllUser((char*)this->m_messages[HH_START_MESSAGE].c_str());
				}
			}
			else
			{
				if(current_time > this->m_usedConfig->time_end)
				{
					if(this->m_usedConfig->Time.DayOfWeek == false)
					{
						CTime time_start = CTime(current_time.GetYear(),
							(this->m_usedConfig->Time.Month != -1 ? this->m_usedConfig->Time.Month : current_time.GetMonth()),
							(this->m_usedConfig->Time.Day != -1 ? this->m_usedConfig->Time.Day : current_time.GetDay()),
							(this->m_usedConfig->Time.Hour != -1 ? this->m_usedConfig->Time.Hour : current_time.GetHour()),
							(this->m_usedConfig->Time.Minutes != -1 ? this->m_usedConfig->Time.Minutes : current_time.GetMinute()),
							0);

						CTime time_end = time_start;
						time_end += CTimeSpan(0, 0, this->m_usedConfig->Time.Duration, 0);

						if(current_time < time_start || current_time > time_end)
							this->Clear();
						else
						{
							this->m_usedConfig->time_end = time_end;
						}
					}
					else
						this->Clear();
				}
				else
				{
					CTimeSpan time_changed = CTimeSpan(this->m_usedConfig->time_end.GetTime()) + CTimeSpan(0,0,1,0) - CTimeSpan(CTime::GetCurrentTime().GetTime());

					if(time_changed.GetTotalMinutes() <= TIME_TO_INIT && this->m_msgTimeSpan.GetTotalSeconds() == 0)
						this->m_msgTimeSpan = time_changed;

					if(time_changed.GetTotalMinutes() <= TIME_TO_INIT && time_changed.GetTotalMinutes() <= this->m_msgTimeSpan.GetTotalMinutes())
					{
						this->m_msgTimeSpan = time_changed-CTimeSpan(0, 0, 1, 0);

						notice.SendToAllUser((char*)this->m_messages[HH_TIME_END_MESSAGE].c_str(), time_changed.GetTotalMinutes());
					}

					ProcessExperience();
				}
			}
		}
	}
}

void CHappyHour::ProcessExperience()
{
	if(this->m_usedConfig != NULL)
	{
		switch(this->m_usedConfig->ExperienceType)
		{
		case EXP_TYPE_ABSOLUTE:
			{
				if(gAddExperience != this->m_usedConfig->Experience)
					gAddExperience = this->m_usedConfig->Experience;
			}
			break;

		case EXP_TYPE_ADDITION:
			{
				switch(this->m_usedConfig->ExperienceFormat)
				{
				case EXP_FORMAT_ABSOLUTE:
					{
						if(gAddExperience != this->m_defaultExperience+this->m_usedConfig->Experience)
							gAddExperience = this->m_defaultExperience+this->m_usedConfig->Experience;
					}
					break;

				case EXP_FORMAT_PERCENT:
					{
						if(gAddExperience != this->m_defaultExperience+(this->m_defaultExperience*this->m_usedConfig->Experience/100.0f))
							gAddExperience = this->m_defaultExperience+(this->m_defaultExperience*this->m_usedConfig->Experience/100.0f);
					}
					break;
				}
			}
			break;
		}
	}
}

void CHappyHour::Clear()
{
	if(this->m_usedConfig != NULL)
	{
		LogAddTD("[Happy Hour] Event end");
		gAddExperience = this->m_defaultExperience;
		this->m_usedConfig = NULL;
		this->m_init = false;
		TNotice notice;

		notice.SendToAllUser((char*)this->m_messages[HH_END_MESSAGE].c_str());
	}
}

#endif