// AttackEvent.cpp: implementation of the CAttackEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Include\\Prodef.H"
#include "User.h"
#include "Protocol.h"
#include "MonsterSetBase.h"
#include "AttackEvent.h"
#include "GameMain.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CMonsterSetBase	gMSetBase;
extern BOOL gObjMonsterRegen(LPOBJECTSTRUCT lpObj);


CAttackEvent::CAttackEvent()
{
	Init();
	
#ifdef FOR_THAILAND	
	Start();
#endif
}

CAttackEvent::~CAttackEvent()
{

}


void CAttackEvent::Init()
{
	EventState = 0;
	EventStartTime = GetTickCount();
	m_MapNumber = 0;
}

void CAttackEvent::Start()
{
	EventState = 1;
	EventStartTime = GetTickCount();
}

void CAttackEvent::End()
{

}

void CAttackEvent::Run()
{

#ifdef EVENT_MANAGEMENT_20040711
	if (m_bMenualStart)
		return;
#endif
	
	return;
	if( !EventState ) return;

	if( EventState == 1 )
	{
		if( (GetTickCount()-EventStartTime) > 3000 )
		{
			EventState = 2;				// 습격 이벤트 발생
			//GCMapEventStateSend(m_MapNumber, 1, 1);
			Active();
			EventStartTime = GetTickCount();
			LogAdd("Attack Event Start ");
			return;
		}
	}
	else if( EventState == 2 )
	{

#if TESTSERVER == 0 
		if( (GetTickCount()-EventStartTime) > ((1000*60)*gAttackEventRegenTime) )
#else
		if( (GetTickCount()-EventStartTime) > ((1000*60)*1000) )
#endif	
		{
			End();
			EventState = 3;
			EventStartTime = GetTickCount();
			LogAdd("Attack Event End");
		}
	}
	else if( EventState == 3 )
	{
		
		if( (GetTickCount()-EventStartTime) > ((1000*60)*gAttackEventRegenTime) )
		{
			EventState = 0;
			Start();
			LogAdd("Attack Event Restart");
		}
	}
}


void CAttackEvent::Active()
{

#if TESTSERVER == 1
//	return;
#endif
	int n;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 53 || 
			gObj[n].Class == 54 )
		{
			gObj[n].Live	= 1;
			
			if( gObj[n].Class == 53 )
			{
				m_MapNumber = 2;				

				gObj[n].MapNumber = m_MapNumber;
				
				while(!gMSetBase.GetBoxPosition(m_MapNumber, 10, 10, 240, 240, gObj[n].X,gObj[n].Y)){}

				m_BossMap53  = gObj[n].MapNumber;
				m_BossMapX53 = gObj[n].X;
				m_BossMapY53 = gObj[n].Y;
				LogAdd("event %d %d %d create ", m_BossMap53, m_BossMapX53, m_BossMapY53);
			}
			else
			{
				m_MapNumber = m_BossMap53;
				gObj[n].MapNumber = m_MapNumber;

				while(!gMSetBase.GetBoxPosition(m_MapNumber, m_BossMapX53-4, m_BossMapY53-4, m_BossMapX53+4, m_BossMapY53+4, gObj[n].X,gObj[n].Y)){}
			}
			
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

			gObj[n].m_State	= OBJST_CREATE;	
			
		}
		else if( gObj[n].Class == 55 || 
			gObj[n].Class == 56 )
		{
			gObj[n].Live	= 1;
			if( gObj[n].Class == 55 )
			{
				if( rand()%2 == 0 )
				{
					m_MapNumber = 0;
				}
				else m_MapNumber = 3;
				
				gObj[n].MapNumber = m_MapNumber;			

				while(!gMSetBase.GetBoxPosition(m_MapNumber, 10, 10, 240, 240, gObj[n].X,gObj[n].Y)){}
				//gMSetBase.GetBoxPosition(m_MapNumber, 215, 167, 216, 168, gObj[n].X,gObj[n].Y);

				m_BossMap55  = gObj[n].MapNumber;
				m_BossMapX55 = gObj[n].X;
				m_BossMapY55 = gObj[n].Y;
				LogAdd("event %d %d %d create ", m_BossMap55, m_BossMapX55, m_BossMapY55);
			}
			else
			{
				m_MapNumber = m_BossMap55;
				gObj[n].MapNumber = m_MapNumber;

				while(!gMSetBase.GetBoxPosition(m_MapNumber, m_BossMapX55-4, m_BossMapY55-4, m_BossMapX55+4, m_BossMapY55+4, gObj[n].X,gObj[n].Y)){}

			}
			
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

			gObj[n].m_State	= OBJST_CREATE;					
		}	
	}
}


#ifdef EVENT_MANAGEMENT_20040711


void CAttackEvent::Start_Menual ()
{
	SetMenualStart(TRUE);

	LogAddTD("○●[Event Management] [Start] Attack Event!");

	EventState = 0;
	Start();
	Active();
}


void CAttackEvent::End_Menual ()
{
	SetMenualStart(FALSE);

	End();
}


#endif

