// DragonEvent.cpp: implementation of the CDragonEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Include\\Prodef.H"
#include "User.h"
#include "Protocol.h"
#include "MonsterSetBase.h"
#include "DragonEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CMonsterSetBase	gMSetBase;
extern BOOL gObjMonsterRegen(LPOBJECTSTRUCT lpObj);

CDragonEvent::CDragonEvent()
{
	Init();
}

CDragonEvent::~CDragonEvent()
{
}

void CDragonEvent::Init()
{
	EventState = 0;
	EventStartTime = 0;
	m_MapNumber	= 0;

#ifdef EVENT_MANAGEMENT_20040711
	m_bMenualStart	 = FALSE;
#endif
	
}

void CDragonEvent::Start()
{	
	if( EventState ) return;
	Init();

	char szTemp[256];	
	EventState = 1;
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
	char map[2]={MAP_INDEX_NORIA, MAP_INDEX_RORENCIA};
	m_MapNumber = map[rand()%2];
#else
	char map[3]={0, 2, 3};
	m_MapNumber = map[rand()%3];
#endif
	
	strcpy(szTemp, "☆ ");

	
	/*switch( m_MapNumber )
	{
	case 0 : strcat(szTemp, "로랜시아에 "); break;
	case 2 : strcat(szTemp, "데비아스에 "); break;
	case 3 : strcat(szTemp, "노리아에 "); break;
	}
	strcat(szTemp, "드래곤 습격 ☆");
	*/
	switch( m_MapNumber )
	{
	case 0 : strcat(szTemp, lMsg.Get(2000)); break;
	case 2 : strcat(szTemp, lMsg.Get(2002)); break;
	case 3 : strcat(szTemp, lMsg.Get(2003)); break;
	}
	strcat(szTemp, lMsg.Get(1156));
	strcat(szTemp, " ☆");
#ifdef FOR_JAPAN	// 일본에는 드래곤 습격 시 양쪽에 ☆ 표시가 깨져서 보임 (2004.10.19)
	AllSendServerMsg(lMsg.Get(1156));
#else
	AllSendServerMsg(szTemp);
#endif	
	EventStartTime = GetTickCount();
}

void CDragonEvent::End()
{
	GCMapEventStateSend(m_MapNumber, 0, 1);
	LogAdd("Event 1 End");
	EventState = 0;
}

void CDragonEvent::DragonActive()
{
	int n;
	
	for( n=0; n<MAX_MONSTER; n++)
	{
		
		if( gObj[n].Class == 44 )	// 환수 드래곤 일경우
		{
			gObj[n].Live	= 1;
			gObj[n].MapNumber = m_MapNumber;
			
			switch( m_MapNumber )
			{
			case 0 :
				if( (rand()%3) == 0 )
					gMSetBase.GetBoxPosition(m_MapNumber, 135, 61, 146, 70, gObj[n].X,gObj[n].Y);
				else if( (rand()%3) == 1 )
					gMSetBase.GetBoxPosition(m_MapNumber, 120, 204, 126, 219, gObj[n].X, gObj[n].Y);
				else 
					gMSetBase.GetBoxPosition(m_MapNumber, 67, 116, 77, 131, gObj[n].X, gObj[n].Y);
				break;
			case 2 :
				if( (rand()%2) == 0 )
					gMSetBase.GetBoxPosition(m_MapNumber, 155, 16, 169, 55, gObj[n].X, gObj[n].Y);
				else 
					gMSetBase.GetBoxPosition(m_MapNumber, 193, 110, 232, 126, gObj[n].X, gObj[n].Y);
				break;
			case 3 :
				if( (rand()%2) == 0 )						
					gMSetBase.GetBoxPosition(m_MapNumber, 198, 161, 223, 175, gObj[n].X, gObj[n].Y);
				else 
					gMSetBase.GetBoxPosition(m_MapNumber, 136, 50, 164, 59, gObj[n].X, gObj[n].Y);
				break;
			}
			gObj[n].m_State	= OBJST_CREATE;				
		}		
	}
}

void CDragonEvent::Run()
{

#ifdef EVENT_MANAGEMENT_20040711
	if (m_bMenualStart)
		return;
#endif

	if( !EventState ) return;


	if( EventState == 1 )
	{
		if( (GetTickCount()-EventStartTime) > 3000 )
		{
			EventState = 2;
			// 드래곤 월드에 뿌림..
			GCMapEventStateSend(m_MapNumber, 1, 1);
			LogAdd("Event Start 1");
			DragonActive();
			EventStartTime = GetTickCount();
			return;
		}
	}
	else if( EventState == 2 )
	{
		if( (GetTickCount()-EventStartTime) > ((1000*60)*5) )
		{
			End();
		}
	}	
}


#ifdef EVENT_MANAGEMENT_20040711


void CDragonEvent::Start_Menual ()
{
	SetMenualStart(TRUE);

	LogAddTD("○●[Event Management] [Start] Dragon Event!");
	
	EventState = 0;
	Start();
}


void CDragonEvent::End_Menual ()
{
	SetMenualStart(FALSE);

	End();
}


#endif