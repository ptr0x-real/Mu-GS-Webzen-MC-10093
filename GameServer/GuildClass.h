// GuildClass.h: interface for the CGuildClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDCLASS_H__03CDE82E_6D6F_4A93_B874_C1CF57F0C171__INCLUDED_)
#define AFX_GUILDCLASS_H__03CDE82E_6D6F_4A93_B874_C1CF57F0C171__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_GUILDMARKBUFFER		32						// 길드마크 이미지 버퍼 크기
#ifdef DARKLORD_WORK
	#define MAX_GUILD				(80)				// 길드의 최대 인원 수
#else
	#define MAX_GUILD				(MAX_LEVEL/10)		// 길드의 최대 인원 수
#endif


#define BTT_WAR					0						// 길드전
#define BTT_SOCCER				1						// 길드 배틀 축구

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
	int			Number;									// 길드 번호
	char		Name[MAX_GUILDNAMESTRING+1];			// 길드 명칭
	BYTE		Mark[MAX_GUILDMARKBUFFER];				// 길드 마크 이미지
	BYTE		Count;									// 길드원 수
	BYTE		TotalCount;
	char		Names[MAX_GUILD][MAX_IDSTRING+1];		// 길드원 이름
	short		Index[MAX_GUILD];						// 길드원의 인덱스
	BYTE		Use[MAX_GUILD];							// Names[n] 원소 사용중인지 확인용 (즉, 길드원의 이름이 사용중인가? 그렇다면 가입된 이름 버퍼)
	char		pServer[MAX_GUILD];						// 사용중인 서버
	char		TargetGuildName[MAX_GUILDNAMESTRING+1];	// 상대 길드 이름
	short		TargetIndex[MAX_GUILD];
	_GUILD_INFO_STRUCT* lpTargetGuildNode;
	BYTE		WarDeclareState;						// 길드전 선포 상태
	BYTE		WarState;								// 길드전 상태
	BYTE		WarType;								// 길드전 타입 : 0 : 일반길드전, 1:전투축구
	BYTE		BattleGroundIndex;						// 경기장 번호
	BYTE		BattleTeamCode;							// 경기때 팀 코드 0 : 레드팀, 1 : 블루팀
	
	BYTE		PlayScore;								// 현재 전쟁 중일경우 점수
	int			TotalScore;								// 누적 점수
	char		Notice[MAX_GUILDNOTICE];				// 길드 공지 

#ifdef ADD_NEW_GUILD_01_20040913
	INT			GuildStatus[MAX_GUILD];					// 길드 직책 및 상태
#endif
	
#ifdef ADD_NEW_GUILD_02_20040922
	BYTE		btGuildType;							// 길드 종류 ( 일반,용병,클랜,중립 등)
#endif
	
#ifdef ADD_NEW_UNION_01_20041006
	INT			iGuildUnion;							// UnionMaster Guild 번호
	INT			iGuildRival;							// Rival Guild 번호
	INT			iTimeStamp;
	char		szGuildRivalName[MAX_GUILDNAMESTRING+1];// Rival Guild Name
#endif
	
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	int			GuildWarTickCount;
#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
	LPOBJECTSTRUCT	lpLifeStone;						// 길드의 라이프스톤
	INT			btLifeStoneCount;						// 길드 라이프스톤 사용횟수
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
	// 길드내 직책을 얻는다.
	INT	GetGuildMemberStatus(char* szGuildName, char* szMemberName);
	// 길드내 직책을 설정한다.
	BOOL SetGuildMemberStatus(char* szGuildName, char* szMemberName, INT iGuildStatus );
#endif

#ifdef ADD_NEW_GUILD_02_20040922
	// 길드 Type 을 얻는다.
	INT GetGuildType(char* szGuildName);
	// 길드 Type 을 설정한다.
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


