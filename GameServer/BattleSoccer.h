// BattleSoccer.h: interface for the CBattleSoccer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLESOCCER_H__4D042550_2C4E_46F7_BC62_14D1D89926A5__INCLUDED_)
#define AFX_BATTLESOCCER_H__4D042550_2C4E_46F7_BC62_14D1D89926A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BattleGround.h"
#include "GuildClass.h"

class CBattleSoccer : public CBattleGround  
{

	int m_goalmove;

public:
	int m_BallIndex;


	CBattleSoccer();
	virtual ~CBattleSoccer();

	RECT m_GoalLeft;
	RECT m_GoalRight;

	LPGUILD_INFO	m_RedTeam;
	LPGUILD_INFO	m_BlueTeam;

	void SetGoalLeftRect(int x, int y, int xl, int yl);
	void SetGoalRightRect(int x, int y, int xl, int yl);

	void SetGoalMove(int set_code);
	int  GetGoalMove();
};

#endif // !defined(AFX_BATTLESOCCER_H__4D042550_2C4E_46F7_BC62_14D1D89926A5__INCLUDED_)
