// BattleSoccerManager.h: interface for the CBattleSoccerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLESOCCERMANAGER_H__5EFB2A1F_9F31_41EC_93F1_67358C44BF48__INCLUDED_)
#define AFX_BATTLESOCCERMANAGER_H__5EFB2A1F_9F31_41EC_93F1_67358C44BF48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBattleSoccerManager  
{
public:
	CBattleSoccerManager();
	virtual ~CBattleSoccerManager();

};

#include "BattleSoccer.h"

#define MAX_BATTLESOCCERGROUND		1

#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
#define CHECK_BATTLESOCCERGROUND_INDEX(C_VALUE)		(C_VALUE<0?FALSE:C_VALUE>=MAX_BATTLESOCCERGROUND?FALSE:TRUE)
#endif

extern CBattleSoccer*  gBSGround[MAX_BATTLESOCCERGROUND];

extern void	InitBattleSoccer();
extern void	ClearBattleSoccer();

extern int  gCheckBlankBattleGround();
extern void gBattleGroundEnable(int n, int enable);
extern int  gCheckBattleGroundTimer();
extern void gBattleGetTeamPosition(int ground, int team, SHORT & X, SHORT & Y);
extern char *GetBattleTeamName(int ground, int team);
extern void SetBattleTeamScore(int ground, int team, int score);
extern int  GetBattleTeamScore(int ground, int team);
extern void BattleSoccerGoalStart(int ground);
extern int  GetBattleSoccerGoalMove(int ground);
extern void BattleSoccerGoalEnd(int ground);

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
extern	void		GuildWarAndBattleSoccerTimeCheck();
#endif


#endif // !defined(AFX_BATTLESOCCERMANAGER_H__5EFB2A1F_9F31_41EC_93F1_67358C44BF48__INCLUDED_)
