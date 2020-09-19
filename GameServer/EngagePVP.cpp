#include "stdafx.h"
#include "EngagePVP.h"
#include "..\include\readscript.h"
#include "TNotice.h"
#include "GMMng.h"
#include "..\\common\\zzzitem.h"
#include "DSProtocol.h"
#include "MapClass.h"

#include "BloodCastle.h"
#include "DevilSquare.h"

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#ifdef MODIFY_ILLUSION_TEMPLE_20070629
#include "IllusionTempleEvent.h"
#endif

extern ITEM_ATTRIBUTE	ItemAttribute[];

CEngagePVP g_PvpEvent;

CEngagePVP::CEngagePVP()
{
	InitializeCriticalSection(&this->m_criticalSection);

	this->Clear();

	this->m_reloadConfig = false;

	this->m_eventState = EVENT_NONE;
}

CEngagePVP::~CEngagePVP()
{
	DeleteCriticalSection(&this->m_criticalSection);
}

void CEngagePVP::Clear()
{
	if(this->m_eventState != EVENT_RUNNING && this->m_eventState != EVENT_FINISH)
	{
		for(int i = 0;i < DAYS_OF_WEEK;++i)
			this->m_timeList[i].clear();

		this->m_rewardList.clear();
		this->m_rewardList.clear();

		this->m_nextTime = NULL;

		this->m_tickCount = 0;
		this->m_minToAdd = 0;
		this->m_secToAdd = 0;
	}
}

void CEngagePVP::ClearUser(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if(lpObj->m_stateEngagePVP != PVP_USER_NONE)
	{
		lpObj->m_stateEngagePVP = PVP_USER_NONE;

		if(lpObj->MapNumber == PVP_EVENT_MAP)
			gObjMoveGate(lpObj->m_Index, 22);
	}
}

void CEngagePVP::DeleteUser(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	EnterCriticalSection(&this->m_criticalSection);

	USER_INDEX_LIST::iterator it = this->m_userList.begin();

	for(;it != this->m_userList.end();++it)
	{
		if(*it == aIndex)
		{
			break;
		}
	}

	if(it != this->m_userList.end())
	{
		this->m_userList.erase(it);
	}

	if(lpObj->m_stateEngagePVP != PVP_USER_NONE)
	{
		this->ClearUser(aIndex);
	}

	LeaveCriticalSection(&this->m_criticalSection);
}

void CEngagePVP::ProcessUserRequest(LPOBJECTSTRUCT lpObj)
{
	EnterCriticalSection(&this->m_criticalSection);
	if(this->m_eventState == EVENT_CLOSED && this->m_tickCount != 0)
	{
		if(lpObj->m_stateEngagePVP == PVP_USER_NONE && !CHECK_BLOODCASTLE(lpObj->MapNumber)
#ifdef CHAOSCASTLE_SYSTEM_20040408
			&& !CHECK_CHAOSCASTLE(lpObj->MapNumber) 
#endif
#ifdef DEVILSQUARE_EXTEND_20050221
			&& !CHECK_DEVILSQUARE(lpObj->MapNumber)
#else
			&& lpObj->MapNumber != 9
#endif
#ifdef MODIFY_ILLUSION_TEMPLE_20070629
			&& !CHECK_ILLUSION_TEMPLE(lpObj->MapNumber)
#endif
			)
		{
			lpObj->m_stateEngagePVP = PVP_USER_REGISTERED;
			this->m_userList.push_back(lpObj->m_Index);

			TNotice notice;

			notice.SendToUser(lpObj->m_Index,"[PVP Event] Registrado en el evento");
		}
	}
	LeaveCriticalSection(&this->m_criticalSection);
}

void CEngagePVP::Load(const char *filename)
{
	EnterCriticalSection(&this->m_criticalSection);

	if(this->m_eventState == EVENT_RUNNING || this->m_eventState == EVENT_FINISH)
	{
		this->m_reloadConfig = true;

		LeaveCriticalSection(&this->m_criticalSection);

		return;
	}

	this->Clear();

	if ( !filename || !strcmp(filename , "") )
	{
		MsgBox("[PVP System] - File load error : File Name Error");

		LeaveCriticalSection(&this->m_criticalSection);

		return;
	}

	try
	{
		SMDFile = fopen(filename, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[PVP System] Info file Load Fail [%s]", filename);

			LeaveCriticalSection(&this->m_criticalSection);

			return;
		}

		int Token;
		int type = -1;

		bool eventEnabled = true;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END || eventEnabled == false )
			{
				break;
			}

			type = TokenNumber;

			while ( true )
			{
				if ( type == 0 ) //Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					eventEnabled = TokenNumber > 0;
				}

				else if ( type == 1 ) //Time
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					int dayOfWeek = TokenNumber;

					while(true)
					{
						Token = GetToken();

						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}

						EVENT_TIME eventTime;

						eventTime.iHour = TokenNumber;

						Token = GetToken();
						eventTime.iMin = TokenNumber;

						if(dayOfWeek == 0)
						{
							for(int i = 0;i < DAYS_OF_WEEK;++i)
								this->m_timeList[i].push_back(eventTime);
						}
						else
							this->m_timeList[dayOfWeek-1].push_back(eventTime);
					};
				}

				else if ( type == 2 ) //Reward List
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					REWARD_ITEM rewardItem;

					rewardItem.m_dwType = TokenNumber;

					if(rewardItem.m_dwType < 0 || rewardItem.m_dwType > MAX_ITEM_TYPE-1)
						throw "Item type out of range";

					Token = GetToken();

					if(TokenNumber < 0 || TokenNumber > MAX_ITEM_INDEX-1)
						throw "Item index out of range";

					rewardItem.m_dwType = ItemGetNumberMake(rewardItem.m_dwType,TokenNumber);

					Token = GetToken();
					rewardItem.m_btLevel = TokenNumber;

					Token = GetToken();
					rewardItem.m_bSkill = TokenNumber > 0;

					Token = GetToken();
					rewardItem.m_bLuck = TokenNumber > 0;

					Token = GetToken();
					rewardItem.m_btOption = TokenNumber;

					Token = GetToken();
					rewardItem.m_btExcOption = TokenNumber;

					Token = GetToken();
					rewardItem.m_iMaxRate = TokenNumber;

					this->m_rewardList.push_back(rewardItem);
				}
			}
		}

		fclose(SMDFile);
		LogAdd("[PVP System] %s file load!", filename);

		if(eventEnabled)
		{
			LogAdd("[PVP System] Event Enabled");
			this->m_eventState = EVENT_CLOSED;
		}
		else
		{
			LogAdd("[PVP System] Event Disabled");
			this->m_eventState = EVENT_NONE;
		}
	}	// __try
	catch ( const char *exception )
	{
		MsgBox("[PVP System] - Loading Exception Error (%s) - (%s) File. ", filename, exception);
	}

	LeaveCriticalSection(&this->m_criticalSection);
}

void CEngagePVP::Run()
{
	EnterCriticalSection(&this->m_criticalSection);

	switch(this->m_eventState)
	{
	case EVENT_NONE:
		break;

	case EVENT_CLOSED:
		{
			if(this->m_reloadConfig)
			{
				this->m_eventState = EVENT_NONE;

				this->Load();

				LeaveCriticalSection(&this->m_criticalSection);

				return;
			}

			this->ProcessClosed();
		}
		break;

	case EVENT_RUNNING:
		{
			this->ProcessRunning();
		}
		break;

	case EVENT_FINISH:
		{
			this->ProcessFinish();
		}
		break;
	};

	LeaveCriticalSection(&this->m_criticalSection);
}

void CEngagePVP::ProcessClosed()
{
	tm * today;
	time_t ltime;

	time( &ltime );
	today = localtime( &ltime );

	if(this->m_nextTime == NULL)
	{
		for(EVENT_TIME_LIST::iterator it = this->m_timeList[today->tm_wday].begin();it != this->m_timeList[today->tm_wday].end();++it)
		{
			EVENT_TIME &eventTime = *it;

			if(this->m_nextTime == NULL)
			{
				if(eventTime.iHour >= today->tm_hour && eventTime.iMin >= today->tm_min)
					this->m_nextTime = &eventTime;
			}
			else
			{
				if(eventTime.iHour >= today->tm_hour && eventTime.iMin >= today->tm_min && (eventTime.iHour < this->m_nextTime->iHour || (eventTime.iHour < this->m_nextTime->iHour && eventTime.iMin < today->tm_min)))
					this->m_nextTime = &eventTime;
			}
		}
	}

	DWORD dwTickCount = GetTickCount();

	TNotice notice;

	if(this->m_tickCount != 0 && this->m_tickCount+(5*60*1000) <= dwTickCount)
	{
		for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();)
		{
			if(gObjIsConnected(*it) == FALSE || gObj[*it].m_stateEngagePVP == PVP_USER_NONE)
			{
				it = this->m_userList.erase(it);
			}
			else
				++it;
		}

		if(this->m_userList.size() >= 2)
		{
			LogAdd("[PVP System] Event Start");

			notice.SendToAllUser("[PVP Event] Inicio el evento! - %d jugadores", this->m_userList.size());

			for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();++it)
			{
				LPOBJECTSTRUCT lpObj = &gObj[*it];

				lpObj->m_stateEngagePVP = PVP_USER_PLAYING;

				//gObjMoveGate(*it, 58);
				gObjMoveGate(*it, 58);

				//gObjTeleport(lpObj->m_Index, PVP_EVENT_MAP, 30+rand()%40, 30+rand()%40);
			}

			this->m_minToAdd = 0;
			this->m_secToAdd = 0;
			this->m_tickCount = dwTickCount;

			this->m_eventState = EVENT_RUNNING;
		}
		else
		{
			for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();++it)
			{
				this->ClearUser(*it);
			}

			this->m_userList.clear();

			this->m_nextTime = NULL;
			this->m_tickCount = 0;
			this->m_minToAdd = 0;
			this->m_secToAdd = 0;
		}
	}
	else if(this->m_tickCount != 0 || (this->m_nextTime != NULL && (today->tm_hour == this->m_nextTime->iHour && today->tm_min >= this->m_nextTime->iMin-5)))
	{
		if(this->m_tickCount == 0)
		{
			this->m_tickCount = dwTickCount;
			this->m_minToAdd = 0;
			this->m_secToAdd = 0;
		}

		if(this->m_minToAdd < 4)
		{
			if(this->m_tickCount+(this->m_minToAdd*60*1000) <= dwTickCount)
			{
				notice.SendToAllUser("[PVP Event] Faltan %d minutos para el inicio", 5-this->m_minToAdd);

				this->m_minToAdd++;
			}
		}
		else
		{
			if(this->m_tickCount+(this->m_minToAdd*60*1000)+(this->m_secToAdd*1000) <= dwTickCount)
			{
				notice.SendToAllUser("[PVP Event] Faltan %d segundos para el inicio", 60-this->m_secToAdd);

				this->m_secToAdd += 15;
			}
		}
	}
}

void CEngagePVP::ProcessRunning()
{
	for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();)
	{
		LPOBJECTSTRUCT lpObj = &gObj[*it];

		if(lpObj->MapNumber != PVP_EVENT_MAP)
		{
			this->ClearUser(*it);

			it = this->m_userList.erase(it);
		}
		else
		{
			++it;
		}
	}

	TNotice notice;

	DWORD dwTickCount = GetTickCount();

	if(this->m_userList.size() <= 1)
	{
		USER_INDEX_LIST::iterator it = this->m_userList.begin();

		this->m_minToAdd = 0;
		this->m_secToAdd = 0;
		this->m_tickCount = dwTickCount;

		this->m_eventState = EVENT_FINISH;

		if(it != this->m_userList.end())
		{
			LPOBJECTSTRUCT lpObj = &gObj[*it];

			for(REWARD_ITEM_LIST::iterator it = this->m_rewardList.begin();it != this->m_rewardList.end();++it)
			{
				REWARD_ITEM &item = *it;

				if(rand()%1000 < item.m_iMaxRate)
				{
					BYTE NewOption = 0;

					for(int i = 0;i < item.m_btExcOption;i++)
						NewOption |= 1 << i;

					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)lpObj->X+rand()%5, (BYTE)lpObj->Y+rand()%5, item.m_dwType, item.m_btLevel, (BYTE)ItemAttribute[item.m_dwType].Durability, item.m_bSkill, item.m_bLuck, item.m_btOption, lpObj->m_Index, NewOption, 0, TRUE);
				}
			}

			notice.SendToAllUser("[PVP Event] El ganador del evento es %s - Felicidades!", lpObj->Name);
		}
		else
		{
			notice.SendToAllUser("[PVP Event] No hubo ganador!");
		}
	}
	else
	{
		if(this->m_minToAdd == 60 && this->m_secToAdd == 60)
		{
			this->m_minToAdd = 0;
			this->m_secToAdd = 0;
			this->m_tickCount = dwTickCount;

			this->m_eventState = EVENT_FINISH;

			notice.SendToAllUser("[PVP Event] Se termino el tiempo, no hubo ganador!");

			for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();it = this->m_userList.erase(it))
			{
				this->ClearUser(*it);
			}
		}
		else
		{
			if(this->m_minToAdd < 60)
			{
				if(this->m_tickCount+(this->m_minToAdd*60*1000) <= dwTickCount)
				{
					for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();++it)
					{
						notice.SendToUser(*it, "[PVP Event] quedan %d minutos", 60-this->m_minToAdd);
					}

					this->m_minToAdd += 15;
				}
			}
			else
			{
				if(this->m_tickCount+((this->m_minToAdd-1)*60*1000)+(this->m_secToAdd*1000) <= dwTickCount)
				{
					for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();++it)
					{
						notice.SendToUser(*it, "[PVP Event] quedan %d segundos", 60-this->m_secToAdd);
					}

					this->m_secToAdd += 15;
				}
			}
		}
	}
}

void CEngagePVP::ProcessFinish()
{
	if(this->m_userList.size() == 0)
	{
		this->m_eventState = EVENT_CLOSED;
		this->m_nextTime = NULL;
		this->m_tickCount = 0;
		this->m_minToAdd = 0;
		this->m_secToAdd = 0;
		return;
	}

	TNotice notice;

	if(this->m_secToAdd == 60)
	{
		for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();it = this->m_userList.erase(it))
		{
			this->ClearUser(*it);
		}
	}
	else
	{
		for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();)
		{
			LPOBJECTSTRUCT lpObj = &gObj[*it];

			if(gObjIsConnected(*it) == FALSE || lpObj->MapNumber != PVP_EVENT_MAP)
			{
				this->ClearUser(*it);
				it = this->m_userList.erase(it);
			}
			else
				++it;
		}

		DWORD dwTickCount = GetTickCount();

		if(this->m_userList.size() > 0)
		{
			if(this->m_tickCount+(this->m_secToAdd*1000) <= dwTickCount)
			{
				for(USER_INDEX_LIST::iterator it = this->m_userList.begin();it != this->m_userList.end();++it)
				{
					cManager.ManagementProc(&gObj[*it], "/fire", *it);
					notice.SendToUser(*it, "[PVP Event] La arena se cerrara en %d segundos", 60-this->m_secToAdd);
				}

				this->m_secToAdd += 15;
			}
		}
	}
}