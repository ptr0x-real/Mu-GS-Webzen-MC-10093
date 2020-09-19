// EledoradoEvent.cpp: implementation of the CEledoradoEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Include\Prodef.H"
#include "User.h"
#include "Protocol.h"
#include "Gamemain.h"
#include "MonsterSetBase.h"

#include "EledoradoEvent.h"
#include "time.h"


extern CMonsterSetBase	gMSetBase;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEledoradoEvent gEledoradoEvent;

CEledoradoEvent::CEledoradoEvent()
{
	GoldGoblenEventStartTime = 0;
	TitanEventStartTime = 0;
	GoldDerconEventStartTime = 0;
	DevilLizardKingEventStartTime = 0;
	KanturEventStartTime = 0;
	
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
	GoldenRabbitEventStartTime = 0;
	GoldenDarkKnightEventStartTime = 0;
	GoldenDevilEventStartTime = 0;
	GoldenStoneMonsterEventStartTime = 0;
	GoldenCrustEventStartTime = 0;
	GoldenSatirosEventStartTime = 0;
	GoldenTwintailEventStartTime = 0;
	GoldenIronKnightEventStartTime = 0;
	GoldenNeipinEventStartTime = 0;
	GoldenGreatDragonEventStartTime = 0;
#endif
	m_BossGoldDerconMapNumber[0] = -1;
	m_BossGoldDerconMapNumber[1] = -1;
	m_BossGoldDerconMapNumber[2] = -1;

	m_BossTitanMapNumber = -1;
	m_BossDevilLizardKingMapNumber = -1;
	m_BossKanturMapNumber = -1;
}

CEledoradoEvent::~CEledoradoEvent()
{

}

void CEledoradoEvent::Init()
{
}

void CEledoradoEvent::SetEventState(BOOL State)
{
	EventState = State;
}

void CEledoradoEvent::Run()
{

#ifdef EVENT_MANAGEMENT_20040711
	if (m_bMenualStart)
		return;
#endif
	
	if( !EventState )
		return;

	if( GoldGoblenEventStartTime == 0 )
	{	// 매시간 30분대에 실행되게 한다
		struct tm *today;
		time_t ltime;
		
		time( &ltime );
		today = localtime( &ltime );

#if TESTSERVER == 0
		if( today->tm_min >= 30 && today->tm_min <= 35 )
#endif
		{
			DWORD TickCount = GetTickCount();
			GoldGoblenEventStartTime = TickCount;
			RegenGoldGoblen();
			TitanEventStartTime = TickCount;
			RegenTitan();
			GoldDerconEventStartTime = TickCount;
			RegenGoldDercon();
			DevilLizardKingEventStartTime = TickCount;
			RegenDevilLizardKing();
			KanturEventStartTime = TickCount;
			RegenKantur();
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
			GoldenRabbitEventStartTime = TickCount;
			RegenGoldenRabbit();
			GoldenDarkKnightEventStartTime = TickCount;
			RegenGoldenDarkKnight();
			GoldenDevilEventStartTime = TickCount;
			RegenGoldenDevil();
			GoldenStoneMonsterEventStartTime = TickCount;
			RegenGoldenStoneMonster();
			GoldenCrustEventStartTime = TickCount;
			RegenGoldenCrust();
			GoldenSatirosEventStartTime = TickCount;
			RegenGoldenSatiros();
			GoldenTwintailEventStartTime = TickCount;
			RegenGoldenTwintail();
			GoldenIronKnightEventStartTime = TickCount;
			RegenGoldenIronKnight();
			GoldenNeipinEventStartTime = TickCount;
			RegenGoldenNeipin();
			GoldenGreatDragonEventStartTime = TickCount;
			RegenGoldenGreatDragon();
#endif //ADD_GOLDEN_EVENT_RENEWAL_20090311
		}
	}
	else
	{
		DWORD TickCount = GetTickCount();

		if( TickCount - GoldGoblenEventStartTime > (1000*60)*gEledoradoGoldGoblenRegenTime )
		{
			GoldGoblenEventStartTime = TickCount;
			RegenGoldGoblen();
		}

		if( TickCount - TitanEventStartTime > (1000*60)*gEledoradoTitanRegenTime )
		{
			TitanEventStartTime = TickCount;
			RegenTitan();
		}

		if( TickCount - GoldDerconEventStartTime > (1000*60)*gEledoradoGoldDerconRegenTime )
		{
			GoldDerconEventStartTime = TickCount;
			RegenGoldDercon();
		}

		if( TickCount - DevilLizardKingEventStartTime > (1000*60)*gEledoradoDevilLizardKingRegenTime)
		{
			DevilLizardKingEventStartTime = TickCount;
			RegenDevilLizardKing();
		}

		if( TickCount - KanturEventStartTime > (1000*60)*gEledoradoDevilTantarosRegenTime )
		{
			KanturEventStartTime = TickCount;
			RegenKantur();
		}

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
		if( TickCount - GoldenRabbitEventStartTime > (1000*60) * gGoldenRabbitRegenTime)
		{
			GoldenRabbitEventStartTime = TickCount;
			RegenGoldenRabbit();
		}
		
		if( TickCount - GoldenDarkKnightEventStartTime > (1000 * 60) * gGoldenDarknightRegenTime )
		{
			GoldenDarkKnightEventStartTime = TickCount;
			RegenGoldenDarkKnight();
		}
		
		if( TickCount - GoldenDevilEventStartTime > (1000 * 60) * gGoldenRabbitRegenTime)
		{
			GoldenDevilEventStartTime = TickCount;
			RegenGoldenDevil();
		}
		
		if( TickCount - GoldenStoneMonsterEventStartTime > (1000 * 60) * gGoldenStoneMonsterRegenTIme)
		{
			GoldenStoneMonsterEventStartTime = TickCount;
			RegenGoldenStoneMonster();
		}
		
		if( TickCount - GoldenCrustEventStartTime > (1000 * 60) * gGoldenCrustEventRegenTime)
		{
			GoldenCrustEventStartTime = TickCount;
			RegenGoldenCrust();
		}
		
		if( TickCount - GoldenSatirosEventStartTime > (1000 * 60) * gGoldenSatirosRegenTime)
		{
			GoldenSatirosEventStartTime = TickCount;
			RegenGoldenSatiros();
		}
		
		if( TickCount - GoldenTwintailEventStartTime > (1000 * 60) * gGoldenTwintailRegenTime)
		{
			GoldenTwintailEventStartTime = TickCount;
			RegenGoldenTwintail();
		}
		
		if( TickCount - GoldenIronKnightEventStartTime > (1000 * 60) * gGoldenIronKnightRegenTime)
		{
			GoldenIronKnightEventStartTime = TickCount;
			RegenGoldenIronKnight();
		}
		
		if( TickCount - GoldenNeipinEventStartTime > (1000 * 60) * gGoldenNeipinRegenTime)
		{
			GoldenNeipinEventStartTime = TickCount;
			RegenGoldenNeipin();
		}
		
		if( TickCount - GoldenGreatDragonEventStartTime > (1000 * 60) * gGoldenGreatDragonRegenTime)
		{
			GoldenGreatDragonEventStartTime = TickCount;
			RegenGoldenGreatDragon();
		}
		
		
#endif
	}
}

void CEledoradoEvent::RegenGoldGoblen()
{
	int n  = 0;
	int MapNumber;

#ifndef ADD_GOLDEN_EVENT_RENEWAL_20090311
	int Map[2] = {MAP_INDEX_RORENCIA, MAP_INDEX_NORIA};
#endif// ADD_GOLDEN_EVENT_RENEWAL_20090311

	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 78 )
		{	// 황금 고블린
			gObj[n].Live	= 1;
			
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
			MapNumber = MAP_INDEX_NORIA;
#else
			MapNumber = Map[rand()%2];
#endif

			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			

#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금 고블린 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			AllSendServerMsg(msg);
#endif
#endif

			LogAddTD("Make GoldGoblen : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenTitan()
{
	int n  = 0;
	int MapNumber;

	if(m_BossTitanMapNumber == -1)
		m_BossTitanMapNumber = MAP_INDEX_DEVIAS;

	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 53 )
		{	// 타이탄
			gObj[n].Live	= 1;
			
			MapNumber = MAP_INDEX_DEVIAS;

			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X,gObj[n].Y)){}

			m_BossTitanMapNumber = gObj[n].MapNumber;		// 
			m_BossTitanMapX = gObj[n].X;
			m_BossTitanMapY = gObj[n].Y;

			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			

			LogAddTD("Make GoldTaitan : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);

#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"타이탄 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			AllSendServerMsg(msg);
#endif
#endif

		}
		else if( gObj[n].Class == 54 )
		{	// 솔저
			gObj[n].Live	= 1;

			MapNumber = m_BossTitanMapNumber;
			gObj[n].MapNumber = MapNumber;
			gMSetBase.GetBoxPosition(MapNumber, m_BossTitanMapX-4, m_BossTitanMapY-4, m_BossTitanMapX+4, m_BossTitanMapY+4, gObj[n].X,gObj[n].Y);

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
		}
	}
}

void CEledoradoEvent::RegenGoldDercon()
{
	int n  = 0;
	int MapNumber = -1;

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
	int Map[2] = {MAP_INDEX_RORENCIA, MAP_INDEX_LOSTTOWER};
#else
	int Map[3] = {MAP_INDEX_RORENCIA, MAP_INDEX_NORIA, MAP_INDEX_DEVIAS};
#endif // ADD_GOLDEN_EVENT_RENEWAL_20090311
	int	SelMap = -1;

	int count = 0;	


	CheckGoldDercon(MapNumber);

	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 79 )
		{	// 골드 데르콘
			gObj[n].Live	= 1;						

			if(SelMap == -1 )
			{				
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
				MapNumber = Map[rand()%2];
#else
				MapNumber = Map[rand()%3];				
#endif
			}
			else
			{
				MapNumber = SelMap;
			}

			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 80, 80, 170, 170, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			

			LogAddTD("Make GoldDercon : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);

			if( SelMap == -1 )
			{		
				char szTemp[256];
#ifdef FOR_JAPAN
				strcpy(szTemp, " ");
#else
				strcpy(szTemp, "☆ ");
#endif

				switch( MapNumber )
				{
				case 0 : strcat(szTemp, lMsg.Get(2000)); break;
				case 2 : strcat(szTemp, lMsg.Get(2002)); break;
				case 3 : strcat(szTemp, lMsg.Get(2003)); break;
				}
				
				strcat(szTemp, lMsg.Get(1158));

				//strcat(szTemp, " 황금데르콘 습격");
#ifndef FOR_JAPAN
				strcat(szTemp, " ☆");
#endif
//				AllSendServerMsg(szTemp);
			}

			m_BossGoldDerconMapNumber[count] = MapNumber;
			gObj[n].m_BossGoldDerconMapNumber = count;
			count++;		

			SelMap = MapNumber;

#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"골드 데르콘 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			AllSendServerMsg(msg);
#endif
#endif

		}
	}

	CheckGoldDercon(MapNumber);
}

void CEledoradoEvent::RegenDevilLizardKing()
{
	int n  = 0;
	int MapNumber;

	if(m_BossDevilLizardKingMapNumber == -1)
		m_BossDevilLizardKingMapNumber = MAP_INDEX_ATHLANSE;

	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 80 )
		{	// 데블 리자드킹
			gObj[n].Live	= 1;
			
			MapNumber = MAP_INDEX_ATHLANSE;

			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X,gObj[n].Y)){}

			m_BossDevilLizardKingMapNumber = gObj[n].MapNumber;		// 
			m_BossDevilLizardKingMapX = gObj[n].X;
			m_BossDevilLizardKingMapY = gObj[n].Y;

			gObj[n].Life = gObj[n].MaxLife;			
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			

			LogAddTD("Make GoldLizarKing : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);

#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"데블 리자드킹 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			AllSendServerMsg(msg);
#endif
#endif
		}
		else if( gObj[n].Class == 81 )
		{	// 이블 베파르
			gObj[n].Live	= 1;

			MapNumber = m_BossDevilLizardKingMapNumber;
			gObj[n].MapNumber = MapNumber;
			gMSetBase.GetBoxPosition(MapNumber, m_BossDevilLizardKingMapX-4, m_BossDevilLizardKingMapY-4, m_BossDevilLizardKingMapX+4, m_BossDevilLizardKingMapY+4, gObj[n].X,gObj[n].Y);

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
		}
	}
}

void CEledoradoEvent::RegenKantur()
{
	int n  = 0;
	int MapNumber;

	if(m_BossKanturMapNumber == -1)
		m_BossKanturMapNumber = MAP_INDEX_TARKAN;

	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 82 )
		{	// 칸투르 1
			gObj[n].Live	= 1;
			
			MapNumber = MAP_INDEX_TARKAN;

			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X,gObj[n].Y)){}

			m_BossKanturMapNumber = gObj[n].MapNumber;		// 
			m_BossKanturMapX = gObj[n].X;
			m_BossKanturMapY = gObj[n].Y;

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjClearBuffEffect( &gObj[n], CLEAR_TYPE_DIEREGEN );
#else
			gObj[n].m_PoisonBeattackCount	= 0;
			gObj[n].m_ColdBeattackCount		= 0;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObj[n].m_ViewState				= 0;
			gObj[n].Teleport				= 0;

			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			

			LogAddTD("Make GoldTantarus : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금 탄탈로스 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			AllSendServerMsg(msg);
#endif
#endif
		}
		else if( gObj[n].Class == 83 )
		{	// 칸투르 2
			gObj[n].Live	= 1;

			MapNumber = m_BossKanturMapNumber;
			gObj[n].MapNumber = MapNumber;
			gMSetBase.GetBoxPosition(MapNumber, m_BossKanturMapX-10, m_BossKanturMapY-10, m_BossKanturMapX+10, m_BossKanturMapY+10, gObj[n].X,gObj[n].Y);

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;

			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
		}
	}
}

void CEledoradoEvent::CheckGoldDercon(int MapNumber)
{
	if( !EventState )
		return;

	BOOL EventOn = FALSE;
	int  EventClearMapNumber = -1;

	for( int i = 0; i < 3; i++ )
	{ 
		if( m_BossGoldDerconMapNumber[i] != -1  )
		{
			if( m_BossGoldDerconMapNumber[i] == MapNumber )
			{
				EventOn = TRUE;
			}		
			else
			{
				EventClearMapNumber = m_BossGoldDerconMapNumber[i];
			}
		}
	}

	if( EventClearMapNumber != -1 )
	{
		GCMapEventStateSend(EventClearMapNumber, 0, 3);
	}

	if( EventOn )
	{
		GCMapEventStateSend(MapNumber, 1, 3);
	}
	else
	{
		GCMapEventStateSend(MapNumber, 0, 3);
	}
}


#ifdef EVENT_MANAGEMENT_20040711


void CEledoradoEvent::Start_Menual ()
{
	SetMenualStart(TRUE);

	LogAddTD("○●[Event Management] [Start] EledoradoEvent Event!");

	DWORD TickCount = GetTickCount();
	GoldGoblenEventStartTime = TickCount;
	RegenGoldGoblen();
	TitanEventStartTime = TickCount;
	RegenTitan();
	GoldDerconEventStartTime = TickCount;
	RegenGoldDercon();
	DevilLizardKingEventStartTime = TickCount;
	RegenDevilLizardKing();
	KanturEventStartTime = TickCount;
	RegenKantur();

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
	GoldenRabbitEventStartTime = TickCount;
	RegenGoldenRabbit();
	GoldenDarkKnightEventStartTime = TickCount;
	RegenGoldenDarkKnight();
	GoldenDevilEventStartTime = TickCount;
	RegenGoldenDevil();
	GoldenStoneMonsterEventStartTime = TickCount;
	RegenGoldenStoneMonster();
	GoldenCrustEventStartTime = TickCount;
	RegenGoldenCrust();
	GoldenSatirosEventStartTime = TickCount;
	RegenGoldenSatiros();
	GoldenTwintailEventStartTime = TickCount;
	RegenGoldenTwintail();
	GoldenIronKnightEventStartTime = TickCount;
	RegenGoldenIronKnight();
	GoldenNeipinEventStartTime = TickCount;
	RegenGoldenNeipin();
	GoldenGreatDragonEventStartTime = TickCount;
	RegenGoldenGreatDragon();
#endif	
	
}


void CEledoradoEvent::End_Menual ()
{
	SetMenualStart(FALSE);
}


#endif

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
void CEledoradoEvent::RegenGoldenRabbit()
{
	int n  = 0;
	int MapNumber;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 502 )
		{	// 황금 토끼
			gObj[n].Live	= 1;
			
#ifdef GAMESERVER_VERSION == G_V_99B
			MapNumber = MAP_INDEX_RORENCIA;
#else
			MapNumber = /*MAP_INDEX_ELBELAND;*/MAP_INDEX_NORIA;
#endif
			
			gObj[n].MapNumber = MapNumber;

			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금 토끼 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			
			LogAddTD("Make Golden rabbit : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}

}
void CEledoradoEvent::RegenGoldenDarkKnight()
{
	int n  = 0;
	int MapNumber;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 493 )
		{	// 황금 다크나이트
			gObj[n].Live	= 1;
			
			MapNumber = MAP_INDEX_DUNGEON;
			
			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금다크나이트 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			
			LogAddTD("Make Golden DarkKnight : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}
void CEledoradoEvent::RegenGoldenDevil()
{
	int n  = 0;
	int MapNumber;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 494 )
		{	// 황금데빌
			gObj[n].Live	= 1;
			
			MapNumber = MAP_INDEX_LOSTTOWER;
			
			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금데빌 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			
			LogAddTD("Make Golden Devil : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}

}
void CEledoradoEvent::RegenGoldenStoneMonster()
{
	
	int n  = 0;
	int MapNumber;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 495 )
		{	// 황금돌괴물
			gObj[n].Live	= 1;
			
#ifdef GAME_VERSION < G_V_S4_5
			MapNumber = MAP_INDEX_RORENCIA;
#else
			MapNumber = MAP_INDEX_AIDA;
#endif
			
			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금돌괴물 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			
			LogAddTD("Make Golden StoneMonster : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}

}
void CEledoradoEvent::RegenGoldenCrust()
{

	int n  = 0;
	int MapNumber;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 496 )
		{	// 황금크러스트
			gObj[n].Live	= 1;
			
#ifdef GAME_VERSION < G_V_S4_5
			MapNumber = MAP_INDEX_RORENCIA;
#else
			MapNumber = MAP_INDEX_ICARUS;
#endif
			
			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금크러스트 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			
			LogAddTD("Make Golden Crust : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}

}

void CEledoradoEvent::RegenGoldenSatiros()
{
	int n  = 0;
	int MapNumber;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 497 )
		{	// 황금사티로스
			gObj[n].Live	= 1;

#ifdef GAME_VERSION < G_V_S4_5
			MapNumber = MAP_INDEX_RORENCIA;
#else
			MapNumber = MAP_INDEX_KANTURU1;
#endif
			
			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금사티로스 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			
			LogAddTD("Make Golden Satiros : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}


}
void CEledoradoEvent::RegenGoldenTwintail()
{
	int n  = 0;
	int MapNumber;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 498 )
		{	// 황글돌괴물
			gObj[n].Live	= 1;

#ifdef GAME_VERSION < G_V_S4_5
			MapNumber = MAP_INDEX_RORENCIA;
#else
			MapNumber = MAP_INDEX_KANTURU2;
#endif
			
			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금트윈테일 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			
			LogAddTD("Make Golden Twintail : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}


}
void CEledoradoEvent::RegenGoldenIronKnight()
{
	int n  = 0;
	int MapNumber;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 499 )
		{	// 황금아이언나이트
			gObj[n].Live	= 1;

#ifdef GAME_VERSION < G_V_S4_5
			MapNumber = MAP_INDEX_RORENCIA;
#else
			MapNumber = MAP_INDEX_RAKLION_FIELD;
#endif
			
			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금아이언나이트 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			
			LogAddTD("Make Golden IronKnight : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}


}
void CEledoradoEvent::RegenGoldenNeipin()
{
	int n  = 0;
	int MapNumber;
	
	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 500 )
		{	// 황금네이핀 
			gObj[n].Live	= 1;
			
#ifdef GAME_VERSION < G_V_S4_5
			MapNumber = MAP_INDEX_RORENCIA;
#else
			MapNumber = MAP_INDEX_SWAMP_OF_CALMNESS;
#endif
			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"황금네이핀 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			
			LogAddTD("Make Golden Neipin : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}


}
void CEledoradoEvent::RegenGoldenGreatDragon()
{
	int n  = 0;
	int MapNumber;
#ifdef GAME_VERSION < G_V_S4_5
	int Map[2] = {MAP_INDEX_RORENCIA, MAP_INDEX_NORIA};
#else
	int Map[2] = {MAP_INDEX_KANTURU1, MAP_INDEX_RAKLION_FIELD};
#endif

	for( n=0; n<MAX_MONSTER; n++)
	{		
		if( gObj[n].Class == 501 )
		{	// 황금 그레이트 드레곤 
			gObj[n].Live	= 1;
			
			MapNumber = Map[rand()%2];
			
			gObj[n].MapNumber = MapNumber;
			while(!gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X,gObj[n].Y)){}
			
			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = (BYTE)gObj[n].X;
			gObj[n].StartY = (BYTE)gObj[n].Y;
			
			gObj[n].m_State	= OBJST_CREATE;
			gObj[n].PathCount = 0;			
			
#if TESTSERVER == 1
#ifndef FOR_ONAIR
			char msg[255];
			wsprintf(msg,"그레이트 황금드래곤 : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
			//AllSendServerMsg(msg);
#endif
#endif
			LogAddTD("Make Golden Great Dragon : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}


}
#endif
