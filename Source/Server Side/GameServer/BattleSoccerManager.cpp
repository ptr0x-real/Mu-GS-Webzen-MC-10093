// BattleSoccerManager.cpp: implementation of the CBattleSoccerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSoccerManager.h"
#include "protocol.h"

#include "GMMng.h"
#ifdef AUTH_GAMESERVER
#include "GameServerAuth.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleSoccerManager::CBattleSoccerManager()
{

}

CBattleSoccerManager::~CBattleSoccerManager()
{

}



CBattleSoccer*  gBSGround[MAX_BATTLESOCCERGROUND];


void	InitBattleSoccer()
{
	// 배틀 사커 정보 세팅
	for( int n=0; n<MAX_BATTLESOCCERGROUND; n++)
	{
		gBSGround[n] = new CBattleSoccer;		
		gBSGround[n]->m_BlueTeam = NULL;
		gBSGround[n]->m_RedTeam = NULL;

	}
	gBSGround[0]->SetBattleType(BT_SOCCER);
	gBSGround[0]->SetGroundRect(6, 54, 141, 70, 180);
	gBSGround[0]->SetGoalLeftRect(60, 139, 64, 140);
	gBSGround[0]->SetGoalRightRect(60, 181, 64, 182);	
}

void	ClearBattleSoccer()
{
	for( int n=0; n<MAX_BATTLESOCCERGROUND; n++)
	{
		delete gBSGround[n];
	}
}

// 비어있는 경기장을 리턴한다.
int gCheckBlankBattleGround()
{
	for( int n=0; n<MAX_BATTLESOCCERGROUND; n++)
	{	
		if( gBSGround[n]->GetUsed() == FALSE )
		{
			return n;
		}
	}	
	return 0xFF;
}

// 사용중인 경기장의 사용자들에게 현재 시간을 알린다.
int gCheckBattleGroundTimer()
{
	int time=0;
	int  i;

#ifndef WORLD_TOURNAMENT_EVENT_SETTING	
	return 0;
#endif
	return gBSGround[0]->GetTimer();

	//for( int n=0; n<MAX_BATTLESOCCERGROUND; n++)
	int n = 0;
	{	
		if( gBSGround[n]->GetUsed() == TRUE )
		{
			time = gBSGround[n]->GetTimer();
			//wsprintf(szTemp, "현재 시간 %d",time);

			if( gBSGround[n]->m_BlueTeam == NULL ) return 0;
			if( gBSGround[n]->m_RedTeam == NULL ) return 0;

			if( gBSGround[n]->m_BlueTeam )
			{
				for( i=0; i<MAX_GUILD; i++)
				{
					int index = gBSGround[n]->m_BlueTeam->Index[i];					
					if( index != -1 )
					{
						//GCServerMsgStringSend(szTemp, index, 1);
				#ifdef WORLD_TOURNAMENT_EVENT_SETTING
						GCTimeViewSend(index, eMatchType_BattleSoccer, time);
				#else
						GCTimeViewSend(index, time);
				#endif
					}
				}
			}
			if( gBSGround[n]->m_RedTeam )
			{
				for( i=0; i<MAX_GUILD; i++)
				{
					int index = gBSGround[n]->m_RedTeam->Index[i];					
					if( index != -1 )
					{
						//GCServerMsgStringSend(szTemp, index, 1);
				#ifdef WORLD_TOURNAMENT_EVENT_SETTING
						GCTimeViewSend(index, eMatchType_BattleSoccer, time);
				#else
						GCTimeViewSend(index, time);
				#endif
					}
				}
			}
		}
	}	
	return time;
}


void gBattleGroundEnable(int n, int enable)
{
#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(n) )
		return;
#endif

	if( enable == 1 )
	{
		gBSGround[n]->Enable(enable);
		gBSGround[n]->SetTimer(1000*60*10);	// 10분 세팅
	}
	else 
	{
		gBSGround[n]->Enable(0);
		gBSGround[n]->SetTeamMaster(0, "none");
		gBSGround[n]->SetTeamMaster(1, "none");
		gBSGround[n]->m_RedTeam = NULL;
		gBSGround[n]->m_BlueTeam = NULL;
	}
}


void gSetBattleTeamMaster(int ground, int team, char * szname, LPGUILD_INFO lpGuild)
{

#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(ground) )
		return;
#endif

	gBSGround[ground]->SetTeamMaster(team, szname);
	gBSGround[ground]->SetTeamName(team, lpGuild->Name);
	if( team == 0 ) gBSGround[ground]->m_RedTeam = lpGuild;
	else gBSGround[ground]->m_BlueTeam = lpGuild;

}

void gBattleGetTeamPosition(int ground, int team, SHORT & X, SHORT & Y)
{

#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(ground) )
		return;
#endif
	
	int w, h;
	
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	int left = gBSGround[ground]->m_rect.left+3;
	int right = gBSGround[ground]->m_rect.right-3;
	int top = gBSGround[ground]->m_rect.top+3;
	int bottom = gBSGround[ground]->m_rect.bottom-3;

	w = right-left;
	h = bottom-top;

	if( w < 1 ) w = 1;
	if( h < 1 ) h = 1;

	X = left+(rand()%w);
	Y = top+(rand()%h);
#else
	w = gBSGround[ground]->m_rect.right-gBSGround[ground]->m_rect.left;
	h = gBSGround[ground]->m_rect.bottom-gBSGround[ground]->m_rect.top;

	if( w < 1 ) w = 1;
	if( h < 1 ) h = 1;

	X = gBSGround[ground]->m_rect.left+(rand()%w);
	Y = gBSGround[ground]->m_rect.top+(rand()%h);
#endif

	
}

char *GetBattleTeamName(int ground, int team)
{
#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(ground) )
		return NULL;
#endif

	return gBSGround[ground]->GetTeamName(team);

}

void SetBattleTeamScore(int ground, int team, int score)
{
#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(ground) )
		return;
#endif
	gBSGround[ground]->SetScore(team, score);
}

int GetBattleTeamScore(int ground, int team)
{
#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(ground) )
		return 0;
#endif
	return gBSGround[ground]->GetScore(team);
}

void BattleSoccerGoalStart(int ground)
{
#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(ground) )
		return;
#endif
	gBSGround[ground]->SetGoalMove(1);
}

void BattleSoccerGoalEnd(int ground)
{
#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(ground) )
		return;
#endif
	gBSGround[ground]->SetGoalMove(0);
}


int GetBattleSoccerGoalMove(int ground)
{
#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(ground) )
		return 0;
#endif
	return gBSGround[ground]->GetGoalMove();
}
// 전투축구 점수 업데이트
// team은 골대를 가진 팀 번호 (골대)
BOOL gBattleSoccerScoreUpdate(int ground, int team)
{

#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	if( !CHECK_BATTLESOCCERGROUND_INDEX(ground) )
		return FALSE;
#endif

	int score=20;
	int totalscore=1;
	LPGUILD_INFO lpGuild;
	LPGUILD_INFO lpTargetGuild;
	BYTE bGoalTeam;

	if( team == 0 ) {
		lpGuild = gBSGround[ground]->m_BlueTeam;
		lpTargetGuild = gBSGround[ground]->m_RedTeam;
		bGoalTeam = 1;
	}
	else {
		lpGuild = gBSGround[ground]->m_RedTeam;
		lpTargetGuild = gBSGround[ground]->m_BlueTeam;
		bGoalTeam = 0;
	}

	if( lpGuild == NULL )
	{
		return FALSE;
	}

#ifdef _NEW_EXDB_
	if( gObjGuildWarProc(lpGuild, lpTargetGuild, score) == 1 )
	{
		if( lpGuild->WarType == BTT_SOCCER )
		{
			gObjGuildWarEndSend(lpGuild, lpTargetGuild, 0x01, 0x00);
			gBattleGroundEnable(lpGuild->BattleGroundIndex, 0);
			gObjGuildWarEnd(lpGuild, lpTargetGuild);
			cManager.BattleInfoSend(GetBattleTeamName(0, 0), 255, GetBattleTeamName(0, 1), 255);			

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
			PMSG_MATCH_RESULT	pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x24, sizeof( pMsg) );
			pMsg.MatchType	= eMatchType_BattleSoccer;

			memcpy(pMsg.MatchTeamName1, lpGuild->Name, MAX_IDSTRING);							
			pMsg.Score1 = lpGuild->PlayScore;

			memcpy(pMsg.MatchTeamName2, lpTargetGuild->Name, MAX_IDSTRING);
			pMsg.Score2 = lpTargetGuild->PlayScore;

			for( int UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
			{
				if( lpGuild->Use[UserIndex] )
				{
					int number = lpGuild->Index[UserIndex];
					if( number >= 0 )
					{
						DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
					}
				}
			}

			for( UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
			{
				if( lpTargetGuild->Use[UserIndex] )
				{
					int number = lpTargetGuild->Index[UserIndex];
					if( number >= 0 )
					{
						DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
					}
				}
			}

			LogAddTD("[WTEvent] BattleSoccer Result Send = %s-%d %s-%d", 
					 lpGuild->Name , lpGuild->PlayScore, 
					 lpTargetGuild->Name , lpTargetGuild->PlayScore);

#endif
			/*lpGuild->WarDeclareState = 0;
			lpGuild->WarState		 = 0;
			LogAdd("길드 전투 종료 메시지 : %s", lpGuild->Name);
			
			if( lpTargetGuild != NULL )
			{
				lpTargetGuild->WarDeclareState	= 0;
				lpTargetGuild->WarState			= 0;
				LogAdd("길드 전투 종료 메시지 : %s", lpTargetGuild->Name);
				LPGUILD_INFO	lpTguild;
				
				lpTguild = lpGuild->lpTargetGuildNode;
				lpGuild->lpTargetGuildNode = NULL;
				lpTguild->lpTargetGuildNode = NULL;
			}
			else 
			{
				lpGuild->lpTargetGuildNode = NULL;
			}
			*/
		}
	}
#endif

	/*
	lpGuild->PlayScore += score;

	char szTemp[256];

	wsprintf(szTemp,"골~~~인!! %s 팀이 %d점을 획득하였습니다", lpGuild->Name, score);
	
	//GCServerMsgStringSend(szTemp, aIndex, BYTE type);
	//GCServerMsgStringSend("", , BYTE type);
	
	if( lpGuild->PlayScore >= 100  ) // 게임 종료
	{		
		LPOBJECTSTRUCT	lpObj = (LPOBJECTSTRUCT)&gObj[lpGuild->Index[0]];
		
		gObjGuildWarEndSend(lpObj, 0x01, 0x00);
		//DGGuildScoreUpdate(lpObj->lpGuild->Name, lpObj->lpGuild->TotalScore);
		//DGGuildScoreUpdate(lpTargetObj->lpGuild->Name, lpTargetObj->lpGuild->TotalScore);
	}
	else
	{
		for( n=0; n<MAX_GUILD; n++)
		{
			if( lpGuild->Use[n] )
			{
				if( lpGuild->Index[n] >=0 )
				{
					GCGuildWarScore(lpGuild->Index[n]);
					GCServerMsgStringSend(szTemp, lpGuild->Index[n], 1);
				}
			}
		}
		for( n=0; n<MAX_GUILD; n++)
		{
			if( lpGuild->lpTargetGuildNode!= NULL )
			{
				if( lpGuild->lpTargetGuildNode->Use[n] )
				{
					if( lpGuild->lpTargetGuildNode->Index[n] >=0 )
					{
						GCGuildWarScore(lpGuild->lpTargetGuildNode->Index[n]);
						//GCGoalSend(lpGuild->lpTargetGuildNode->Index[n], bGoalTeam);
						GCServerMsgStringSend(szTemp, lpGuild->lpTargetGuildNode->Index[n], 1);
					}
				}
			}
		}
	}
	wsprintf(szTemp, "%s ( %d ) VS %s ( %d )",lpGuild->Name, lpGuild->PlayScore, lpGuild->lpTargetGuildNode->Name, lpGuild->lpTargetGuildNode->PlayScore);
	cManager.ManagerSendData(szTemp, 1);
	*/
	return TRUE;
}

// 골을 체크한다.
int gCheckGoal(int x, int y, int & ground)
{
	for( int n=0; n<MAX_BATTLESOCCERGROUND; n++)
	{
		if( gBSGround[n]->m_Set == TRUE )
		{
			if( x >= gBSGround[n]->m_GoalLeft.left && x <= gBSGround[n]->m_GoalLeft.right &&
				y >= gBSGround[n]->m_GoalLeft.top  && y <= gBSGround[n]->m_GoalLeft.bottom )
			{
				ground = n;
				return 0;
			}
			if( x >= gBSGround[n]->m_GoalRight.left && x <= gBSGround[n]->m_GoalRight.right &&
				y >= gBSGround[n]->m_GoalRight.top  && y <= gBSGround[n]->m_GoalRight.bottom )
			{
				ground = n;
				return 1;
			}
		}
	}
	return -1;
}

// 해당 오브젝트가 경기장에 있는지 체크해준다.
// 
int gCheckBattleGround(LPOBJECTSTRUCT lpObj)
{

	for( int n=0; n<MAX_BATTLESOCCERGROUND; n++)
	{
		if( gBSGround[n]->m_Set == TRUE )
		{
			if( lpObj->X >= gBSGround[n]->m_rect.left && lpObj->X <= gBSGround[n]->m_rect.right &&
				lpObj->Y >= gBSGround[n]->m_rect.top  && lpObj->Y <= gBSGround[n]->m_rect.bottom )
			{
				return n;
			}
			if( lpObj->X >= gBSGround[n]->m_GoalLeft.left && lpObj->X <= gBSGround[n]->m_GoalLeft.right &&
				lpObj->Y >= gBSGround[n]->m_GoalLeft.top  && lpObj->Y <= gBSGround[n]->m_GoalLeft.bottom )
			{
				return n;
			}
			if( lpObj->X >= gBSGround[n]->m_GoalRight.left && lpObj->X <= gBSGround[n]->m_GoalRight.right &&
				lpObj->Y >= gBSGround[n]->m_GoalRight.top  && lpObj->Y <= gBSGround[n]->m_GoalRight.bottom )
			{
				return n;
			}
		}
	}
	return -1;
}


#ifdef WORLD_TOURNAMENT_EVENT_SETTING
void		GuildWarAndBattleSoccerTimeCheck()
{

	LPGUILD_INFO lpMyNode;
	LPGUILD_INFO lpTargetNode;
	LPOBJECTSTRUCT lpObj;
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++ )
	{
		if( gObj[n].Connected > 2 )
		{	
			lpObj = &gObj[n];
			lpMyNode = gObj[n].lpGuild;			

			if( lpMyNode )
			{
				lpTargetNode = lpMyNode->lpTargetGuildNode;
				if( lpTargetNode )
				{
					if( lpMyNode->WarState == 1 && lpMyNode->WarType != BTT_SOCCER )
					{	// 길전 처리
						int playtime = (GetTickCount()-lpMyNode->GuildWarTickCount)/1000;

						if( playtime >= 600 )
						{
							gObjGuildWarEndSend(lpObj, 0x01, 0x00);
							#ifdef CHANGE_GUILD_WAR_END_DELAYTIME
										gObjAddMsgSendDelay(lpObj, MSG_GUILDWAREND, lpObj->m_Index, 1000*2);
							#else
										gObjAddMsgSendDelay(lpObj, MSG_GUILDWAREND, lpObj->m_Index, 1000*50);
							#endif

							PMSG_MATCH_RESULT	pMsg;
							PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x24, sizeof( pMsg) );
							pMsg.MatchType	= eMatchType_GuildWar;

							memcpy(pMsg.MatchTeamName1, lpMyNode->Name, MAX_IDSTRING);							
							pMsg.Score1 = lpMyNode->PlayScore;

							memcpy(pMsg.MatchTeamName2, lpTargetNode->Name, MAX_IDSTRING);
							pMsg.Score2 = lpTargetNode->PlayScore;

							for( int UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
							{
								if( lpMyNode->Use[UserIndex] )
								{
									int number = lpMyNode->Index[UserIndex];
									if( number >= 0 )
									{
										DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
									}
								}
							}

							for( UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
							{
								if( lpTargetNode->Use[UserIndex] )
								{
									int number = lpTargetNode->Index[UserIndex];
									if( number >= 0 )
									{
										DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
									}
								}
							}

							LogAddTD("[WTEvent] GuildWar Result Send = %s-%d %s-%d", 
									 lpMyNode->Name , lpMyNode->PlayScore, 
									 lpTargetNode->Name , lpTargetNode->PlayScore);
						}
						else GCTimeViewSend(n, eMatchType_GuildWar, 600-playtime);
					}
					else if( lpMyNode->WarState == 1 && lpMyNode->WarType == BTT_SOCCER )
					{	// 배틀 사커 처리
						int playtime = (GetTickCount()-lpMyNode->GuildWarTickCount)/1000;

						if( playtime >= 605 )
						{
							gObjGuildWarEndSend(lpMyNode, lpTargetNode, 0x01, 0x00);
							gBattleGroundEnable(lpMyNode->BattleGroundIndex, 0);
							gObjGuildWarEnd(lpMyNode, lpTargetNode);
							cManager.BattleInfoSend(GetBattleTeamName(0, 0), 255, GetBattleTeamName(0, 1), 255);

							PMSG_MATCH_RESULT	pMsg;
							PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x24, sizeof( pMsg) );
							pMsg.MatchType	= eMatchType_BattleSoccer;

							memcpy(pMsg.MatchTeamName1, lpMyNode->Name, MAX_IDSTRING);							
							pMsg.Score1 = lpMyNode->PlayScore;

							memcpy(pMsg.MatchTeamName2, lpTargetNode->Name, MAX_IDSTRING);
							pMsg.Score2 = lpTargetNode->PlayScore;

							for( int UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
							{
								if( lpMyNode->Use[UserIndex] )
								{
									int number = lpMyNode->Index[UserIndex];
									if( number >= 0 )
									{
										DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
									}
								}
							}

							for( UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
							{
								if( lpTargetNode->Use[UserIndex] )
								{
									int number = lpTargetNode->Index[UserIndex];
									if( number >= 0 )
									{
										DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
									}
								}
							}

							LogAddTD("[WTEvent] BattleSoccer Result Send = %s-%d %s-%d", 
									 lpMyNode->Name , lpMyNode->PlayScore, 
									 lpTargetNode->Name , lpTargetNode->PlayScore);
						}
					}
				}
			}
		}	
	}

	int time;
	//for( int n=0; n<MAX_BATTLESOCCERGROUND; n++)
	n = 0;
	{	
		if( gBSGround[n]->GetUsed() == TRUE )
		{
			time = gBSGround[n]->GetTimer();
			//wsprintf(szTemp, "현재 시간 %d",time);

			if( gBSGround[n]->m_BlueTeam != NULL && gBSGround[n]->m_RedTeam != NULL )
			{
				if( gBSGround[n]->m_BlueTeam )
				{
					for( int i=0; i<MAX_GUILD; i++)
					{
						int index = gBSGround[n]->m_BlueTeam->Index[i];					
						if( index != -1 )
						{
							//GCServerMsgStringSend(szTemp, index, 1);
					#ifdef WORLD_TOURNAMENT_EVENT_SETTING
							GCTimeViewSend(index, eMatchType_BattleSoccer, time);
					#else
							GCTimeViewSend(index, time);
					#endif
						}
					}
				}
				if( gBSGround[n]->m_RedTeam )
				{
					for( int i=0; i<MAX_GUILD; i++)
					{
						int index = gBSGround[n]->m_RedTeam->Index[i];					
						if( index != -1 )
						{
							//GCServerMsgStringSend(szTemp, index, 1);
					#ifdef WORLD_TOURNAMENT_EVENT_SETTING
							GCTimeViewSend(index, eMatchType_BattleSoccer, time);
					#else
							GCTimeViewSend(index, time);
					#endif
						}
					}
				}
			}
		}
	}		
}
#endif