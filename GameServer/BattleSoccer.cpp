// BattleSoccer.cpp: implementation of the CBattleSoccer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSoccer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleSoccer::CBattleSoccer()
{
	m_goalmove = 0;
	m_BallIndex = 0;
}

CBattleSoccer::~CBattleSoccer()
{

}

void CBattleSoccer::SetGoalMove(int set_code)
{
	m_goalmove = set_code;
}

int CBattleSoccer::GetGoalMove()
{
	return m_goalmove;
}

void CBattleSoccer::SetGoalLeftRect(int x, int y, int xl, int yl)
{
	m_GoalLeft.left = x;
	m_GoalLeft.top  = y;
	m_GoalLeft.right = xl;
	m_GoalLeft.bottom = yl;
}

void CBattleSoccer::SetGoalRightRect(int x, int y, int xl, int yl)
{
	m_GoalRight.left = x;
	m_GoalRight.top  = y;
	m_GoalRight.right = xl;
	m_GoalRight.bottom = yl;
}


