// GuildClass.h: interface for the CGuildClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDCLASS_H__03CDE82E_6D6F_4A93_B874_C1CF57F0C171__INCLUDED_)
#define AFX_GUILDCLASS_H__03CDE82E_6D6F_4A93_B874_C1CF57F0C171__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_GUILDMARKBUFFER		32						// ��帶ũ �̹��� ���� ũ��
#ifdef DARKLORD_WORK
	#define MAX_GUILD				(80)				// ����� �ִ� �ο� ��
#else
	#define MAX_GUILD				(MAX_LEVEL/10)		// ����� �ִ� �ο� ��
#endif


#define BTT_WAR					0						// �����
#define BTT_SOCCER				1						// ��� ��Ʋ �౸

typedef unsigned char BYTE;
typedef unsigned char* LPBYTE;

#ifdef MODIFY_GUILD_CLASS_20040608
#include <map>
#include <iostream>
#include <string>
using namespace std;
#endif


#ifdef MEM2_KUZIMO
	class OBJECTSTRUCT;
#else
	struct OBJECTSTRUCT;
#endif

typedef OBJECTSTRUCT	*LPOBJECTSTRUCT;



typedef struct _GUILD_INFO_STRUCT
{
	int			Number;									// ��� ��ȣ
	char		Name[MAX_GUILDNAMESTRING+1];			// ��� ��Ī
	BYTE		Mark[MAX_GUILDMARKBUFFER];				// ��� ��ũ �̹���
	BYTE		Count;									// ���� ��
	BYTE		TotalCount;
	char		Names[MAX_GUILD][MAX_IDSTRING+1];		// ���� �̸�
	short		Index[MAX_GUILD];						// ������ �ε���
	BYTE		Use[MAX_GUILD];							// Names[n] ���� ��������� Ȯ�ο� (��, ������ �̸��� ������ΰ�? �׷��ٸ� ���Ե� �̸� ����)
	char		pServer[MAX_GUILD];						// ������� ����
	char		TargetGuildName[MAX_GUILDNAMESTRING+1];	// ��� ��� �̸�
	short		TargetIndex[MAX_GUILD];
	_GUILD_INFO_STRUCT* lpTargetGuildNode;
	BYTE		WarDeclareState;						// ����� ���� ����
	BYTE		WarState;								// ����� ����
	BYTE		WarType;								// ����� Ÿ�� : 0 : �Ϲݱ����, 1:�����౸
	BYTE		BattleGroundIndex;						// ����� ��ȣ
	BYTE		BattleTeamCode;							// ��⶧ �� �ڵ� 0 : ������, 1 : �����
	
	BYTE		PlayScore;								// ���� ���� ���ϰ�� ����
	int			TotalScore;								// ���� ����
	char		Notice[MAX_GUILDNOTICE];				// ��� ���� 

#ifdef ADD_NEW_GUILD_01_20040913
	INT			GuildStatus[MAX_GUILD];					// ��� ��å �� ����
#endif
	
#ifdef ADD_NEW_GUILD_02_20040922
	BYTE		btGuildType;							// ��� ���� ( �Ϲ�,�뺴,Ŭ��,�߸� ��)
#endif
	
#ifdef ADD_NEW_UNION_01_20041006
	INT			iGuildUnion;							// UnionMaster Guild ��ȣ
	INT			iGuildRival;							// Rival Guild ��ȣ
	INT			iTimeStamp;
	char		szGuildRivalName[MAX_GUILDNAMESTRING+1];// Rival Guild Name
#endif
	
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	int			GuildWarTickCount;
#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
	LPOBJECTSTRUCT	lpLifeStone;						// ����� ����������
	INT			btLifeStoneCount;						// ��� ���������� ���Ƚ��
#endif

	_GUILD_INFO_STRUCT *back;
	_GUILD_INFO_STRUCT *next;

#ifdef ADD_NEW_UNION_01_20041006
	
	_GUILD_INFO_STRUCT()
	{
		iTimeStamp	= 0;
		iGuildUnion	= 0;
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
		btLifeStoneCount = 0;
#endif
	}

	void SetGuildUnion( INT iGuildNumber )
	{
		iGuildUnion = iGuildNumber;
		SetTimeStamp();
	}

	void SetGuildRival( INT iGuildNumber )
	{
		iGuildRival = iGuildNumber;
		SetTimeStamp();
	}

	INT	GetGuildUnion() { return iGuildUnion; }
	INT	GetGuildRival() { return iGuildRival; }

	void SetTimeStamp()
	{
		iTimeStamp++;
	}

	INT GetTimeStamp()
	{
		return iTimeStamp;
	}

	BOOL CheckTimeStamp( INT iTime )
	{
		return iTime==iTimeStamp ? TRUE : FALSE;
	}
#endif

} GUILD_INFO, * LPGUILD_INFO;

#ifdef MODIFY_GUILD_CLASS_20040608
typedef map<string, LPGUILD_INFO>			MapGuildName;
typedef MapGuildName::iterator				MapGuildNameItor;
#endif

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
typedef map<INT, LPGUILD_INFO>				MapGuildNumber;
typedef MapGuildNumber::iterator			MapGuildNumberItor;
#endif

class CGuildClass  
{
	LPGUILD_INFO head, tail;

#ifdef MODIFY_GUILD_CLASS_20040608
	MapGuildName	m_GuildMap;
#endif

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
	MapGuildNumber	m_GuildNumberMap;
#endif

public:
	CGuildClass();
	virtual ~CGuildClass();

	void Init();
	LPGUILD_INFO AddGuild(int number, char * guildname, LPBYTE mark, char * mastername, int score );
	
#ifdef ADD_NEW_GUILD_01_20040913
	LPGUILD_INFO AddMember(char* guild_name, char *player_name, int aIndex, int totalcount, int iGuildStatus, int pServer=-1);
#else
	LPGUILD_INFO AddMember(char* guild_name, char *player_name, int aIndex, int totalcount, int pServer=-1);
#endif

#ifdef ADD_NEW_GUILD_01_20040913
	// ��峻 ��å�� ��´�.
	INT	GetGuildMemberStatus(char* szGuildName, char* szMemberName);
	// ��峻 ��å�� �����Ѵ�.
	BOOL SetGuildMemberStatus(char* szGuildName, char* szMemberName, INT iGuildStatus );
#endif

#ifdef ADD_NEW_GUILD_02_20040922
	// ��� Type �� ��´�.
	INT GetGuildType(char* szGuildName);
	// ��� Type �� �����Ѵ�.
	BOOL SetGuildType(char* szGuildName, int iGuildType );
#endif

	LPGUILD_INFO AddMember(LPGUILD_INFO pNode, char *player_name, int aIndex, int totalcount, int pServer=-1);
	BOOL BuildMemberTotal(LPGUILD_INFO lpNode);
	BOOL DelMember(char *guild_name, char *player_name);
	void AddTail(LPGUILD_INFO pNewNode);
	int  DeleteGuild(char * guildname, char *commader);
	void AllDelete();
	void Print();
	void PrintGuild(char * guildname);
	BOOL ConnectUser(LPGUILD_INFO	lpNode, char *guild_name, char *player_name, int aIndex, int pServer);
	BOOL SetServer(char *guild_name, char *player_name, int pServer);
	
	LPGUILD_INFO SearchGuild(char *guildname);
	LPGUILD_INFO SearchGuild_Number(int number);
	LPGUILD_INFO SearchGuild_NumberAndId(int number, char *Id);
	BOOL CloseMember(char *guild_name, char *player_name);

};

extern BOOL	gGuildNoticeStringCheck(char* notice);

#endif // !defined(AFX_GUILDCLASS_H__03CDE82E_6D6F_4A93_B874_C1CF57F0C171__INCLUDED_)


