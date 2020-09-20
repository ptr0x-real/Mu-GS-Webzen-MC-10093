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

	int  m_Score1;		// 1�� ����
	int  m_Score2;		// 2�� ����

public:
	CBattleGround();
	virtual ~CBattleGround();

	char m_szID1[MAX_IDSTRING+1];	// 
	char m_szID2[MAX_IDSTRING+1];	//


	RECT m_rect;		// ��Ʋ ����
	BOOL m_Set;			// ��Ư ������ ���õǾ� �ִ°�?
	int  m_Type;		// ��Ʋ Ÿ��
	int  m_Used;		// ������� ����� �ΰ�?
	int  m_MapNumber;	// ��Ʋ�� �� ��ȣ
	int  m_nTimer;		// ��Ʋ �ð�
	DWORD m_dwCurTimer;	// ��Ʋ 1/1000 �ð�
	



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
