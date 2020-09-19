// BattleGround.cpp: implementation of the CBattleGround class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleGround.h"

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
#include "BattleSoccerManager.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleGround::CBattleGround()
{	
	m_Used = 0;
	m_Set  = 0;
}

CBattleGround::~CBattleGround()
{
}

void CBattleGround::SetGroundRect(int map, int x, int y, int xl, int yl)
{
	m_MapNumber = map;
	m_rect.left  = x;
	m_rect.top   = y;
	m_rect.right = xl;
	m_rect.bottom= yl;
	m_Set        = 1;
}

void CBattleGround::SetBattleType(int type)
{
	m_Type = type;
}

int  CBattleGround::GetUsed()
{
	return m_Used;
}

void CBattleGround::Enable(int enable)
{
	m_Used = enable;
	if( m_Used == TRUE )
	{
		m_Score1	 = 0;
		m_Score2	 = 0;
	}
}

void CBattleGround::SetTeamMaster(int n, char *szId)
{
	if( n == 0 ) strcpy( m_szID1, szId);
	else strcpy( m_szID2, szId);
}

void CBattleGround::SetTeamName(int n, char *szName)
{
	if( n == 0 ) strcpy( m_szTeamName1, szName);
	else strcpy( m_szTeamName2, szName);
}

char * CBattleGround::GetTeamName(int n)
{
	if( n == 0 ) return m_szTeamName1;
	return m_szTeamName2;
}

int CBattleGround::GetScore(int team)
{
	if( team == 0 ) return m_Score1;
	return m_Score2;
}

void CBattleGround::SetScore(int team, int score)
{
	if( team == 0 ) m_Score1 = score;
	else m_Score2 = score;
}

void CBattleGround::SetTimer(int g_time)
{
	m_nTimer = g_time;
	m_dwCurTimer = GetTickCount();
}

int CBattleGround::GetTimer()
{
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	//if( GetBattleSoccerGoalMove(0) )
	{
		return (615) - ((GetTickCount()-m_dwCurTimer)/1000);
	}
//	else
//	{
//		return 15 - ((GetTickCount()-m_dwCurTimer)/1000);
//	}
#else
	return (GetTickCount()-m_dwCurTimer)/1000;
#endif
}

