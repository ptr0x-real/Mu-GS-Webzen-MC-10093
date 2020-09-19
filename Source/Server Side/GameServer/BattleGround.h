// BattleGround.h: interface for the CBattleGround class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEGROUND_H__AF5F161F_9B49_4DB5_B4A6_9F70C974EB4E__INCLUDED_)
#define AFX_BATTLEGROUND_H__AF5F161F_9B49_4DB5_B4A6_9F70C974EB4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BT_NONE		0
#define BT_SOCCER	1

class CBattleGround  
{
	char m_szTeamName1[MAX_GUILDNAMESTRING+1];	// 
	char m_szTeamName2[MAX_GUILDNAMESTRING+1];	//

	int  m_Score1;		// 1팀 점수
	int  m_Score2;		// 2팀 점수

public:
	CBattleGround();
	virtual ~CBattleGround();

	char m_szID1[MAX_IDSTRING+1];	// 
	char m_szID2[MAX_IDSTRING+1];	//


	RECT m_rect;		// 배틀 영역
	BOOL m_Set;			// 배특 영역이 세팅되어 있는가?
	int  m_Type;		// 배틀 타입
	int  m_Used;		// 경기장을 사용중 인가?
	int  m_MapNumber;	// 배틀장 맵 번호
	int  m_nTimer;		// 배틀 시계
	DWORD m_dwCurTimer;	// 배틀 1/1000 시계
	



	void SetGroundRect(int map, int x, int y, int xl, int yl);
	void SetBattleType(int type);
	void Enable(int enable);
	int  GetUsed();
	void SetTeamMaster(int n, char *szId);
	void SetTeamName(int n, char *szName);
	char *GetTeamName(int n);
	void SetTimer(int g_time);
	int  GetTimer();
	int  GetScore(int team);
	void SetScore(int team, int score);

};

#endif // !defined(AFX_BATTLEGROUND_H__AF5F161F_9B49_4DB5_B4A6_9F70C974EB4E__INCLUDED_)
