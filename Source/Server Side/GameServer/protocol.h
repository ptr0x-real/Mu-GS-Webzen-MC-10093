#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__



#include "Include\\GMProtocol.h"
#include "include/ProDef.h"

#include <WinSock2.h>

#define PMHC_MAPJOIN				0xF3	// 사용자가 맵에 접속할때 게임의 시작 정보를 보낸다.
#define PMHC_VIEWPORTCREATE			0x12	// 뷰포트 안에 들어왔다
#define PMHC_VIEWPORTMONSTERCREATE	0x13	// 뷰포트 안에 몬스터가 들어왔다.
#define PMHC_VIEWPORTDESTROY		0x14	// 뷰포트에서 사라졌다.

#ifdef ADD_NEWPVP_PKFIELD
#include "MapProtocol.h"
#endif // ADD_NEWPVP_PKFIELD

typedef struct
{
	PBMSG_HEAD	h;	
} PMSG_DEFAULT, * LPPMSG_DEFAULT;

typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		subcode;
} PMSG_DEFAULT2, * LPPMSG_DEFAULT2;

typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		subcode;
	BYTE		result;
} PMSG_RESULT, * LPPMSG_RESULT;

typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		result;
} PMSG_DEFRESULT, * LPPMSG_DEFRESULT;

typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Count;
} PMSG_DEFAULT_COUNT, * LPPMSG_DEFAULT_COUNT;


typedef struct
{
	PWMSG_HEAD	h;
	char		testbuf[1996];
	int			count;
} PMSG_TEST, * LPPMSG_TEST;

//----------------------------------------------------------------------------
// [0x03] EXE 파일 체크를 위해 암호화 하는데.. 1차로 키를 넘겨준다
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		m_wKey;	// 0 - 1023 까지의 값을 암호화한것
} PMSG_CHECK_MAINEXE;


//----------------------------------------------------------------------------
// 1차로 보낸 Key와 함께 클라이언트 EXE 파일 체크섬을 보내온다
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	DWORD		m_dwKey;	// 0 - 1023 까지의 값을 암호화한것
} PMSG_CHECK_MAINEXE_RESULT, *LPPMSG_CHECK_MAINEXE_RESULT;


//----------------------------------------------------------------------------
// CS UDP [0x01] 접속 서버에 보내는 게임 서버 정보
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

#ifdef SERVERCODE_EXPAND	
	short	ServerCode;
#else
	BYTE	ServerCode;
#endif
	BYTE	Percent;
	short	UserCount;
	short   AccountCount;
	short	PCbangCount;

	short	MaxUserCount;
} PMSG_SERVERINFO, *LPPMSG_SERVERINFO;


#ifdef GAMESERVER_INFO_EXTENDED_20040625	// 게임서버로 부터 확장된 정보들을 받는다.

//----------------------------------------------------------------------------
// CS UDP [0x03] 접속서버에 보내는 게임서버의 확장된 정보들 (PK서버, 서버버전, 클라이언트버전)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	int			iServerCode;		// 서버코드

	BYTE		btIsPK;				// PK 지정 여부
	char		GsVersion[32];		// 게임서버 버전
	char		ClVersion[32];		// 클라이언트 버전
} PMSG_GAMESERVER_EXTINFO, *LPPMSG_GAMESERVER_EXTINFO;

#endif

//----------------------------------------------------------------------------
// [0x0C] 서버로 부터 받는 메시지 번호
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		MsgNumber;
} PMSG_SERVERMSG, *LPPMSG_SERVERMSG;

//----------------------------------------------------------------------------
// [0x0D] 공지사항 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		type;
	
#ifdef MODIFY_NOTICE_20040325
	BYTE		btCount;
	WORD		wDelay;
	DWORD		dwColor;
	BYTE		btSpeed;
#endif

	char		Notice[256];
} PMSG_NOTICE, *LPPMSG_NOTICE;


//----------------------------------------------------------------------------
// [0x0E] 사용자 접속 체크 알림
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	DWORD	    Time;
	//BYTE		BootsLevel;
	WORD		AttackSpeed;	// 추가 
	WORD		MagicSpeed;		// 추가
} PMSG_CLIENTTIME, *LPPMSG_CLIENTTIME;





//----------------------------------------------------------------------------
// [0xF1][0x02] 클라이언트의 접속 종료 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		SubCode;
	BYTE		Flag;
} PMSG_CLIENTCLOSE, *LPPMSG_CLIENTCLOSE;


//----------------------------------------------------------------------------
// [0xF1][0x03] 클라이언트가 서버에 해당 코드를 보낸다
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		SubCode;
	BYTE		Flag;
#ifdef CHANGE_CLIENTHACKCHECK_MSG	
	BYTE		subFlag;
#endif
} PMSG_CLIENTMSG, *LPPMSG_CLIENTMSG;

/*
//----------------------------------------------------------------------------
// [0xF1][0x02] 클라이언트가 계정 생성 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	char		Id[MAX_IDSTRING];
	char		Pass[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	char		JoominN[MAX_JOOMINNUMBERSTR];
	char		Question[MAX_QUESTIONSTR];
	char		Answer[MAX_ANSWERSTR];
	char		PhoneN[MAX_PHONENUMBERSTR];
	char		Email[MAX_EMAILSTR];
} PMSG_ACCOUNTREQUEST, *LPPMSG_ACCOUNTREQUEST;
*/

//----------------------------------------------------------------------------
// [0xF1][0x03] 비번 질문 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	char		Id[MAX_IDSTRING];
} PMSG_PASSQUESTION_QUERY, *LPPMSG_PASSQUESTION_QUERY;

//----------------------------------------------------------------------------
// [0xF1][0x03] 비번 질문 답변
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Result;
	char		Question[MAX_QUESTIONSTR];
} PMSG_PASSQUESTION_QUERY_RESULT, *LPPMSG_PASSQUESTION_QUERY_RESULT;

//----------------------------------------------------------------------------
// [0xF1][0x04] 비밀번호 문의 요청에 대한 답을 클라이언트가 서버로 보낸다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	char		Id[MAX_IDSTRING];
	char		JoominN[MAX_JOOMINNUMBERSTR];
	char		Answer[MAX_ANSWERSTR];
} PMSG_PASSQUERY, *LPPMSG_PASSQUERY;



//----------------------------------------------------------------------------
// [0xF1][0x04] 비밀번호 문의 요청에 대한 결과.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Result;
	char		Pass[MAX_IDSTRING];
} PMSG_PASSQUERY_RESULT, *LPPMSG_PASSQUERY_RESULT;


//----------------------------------------------------------------------------
// [0xF1][0x05] 비밀번호 변경 신청이 왔다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	char		Id[MAX_IDSTRING];
	char		PassOld[MAX_IDSTRING];
	char		PassNew[MAX_IDSTRING];
	char		JoominN[MAX_JOOMINNUMBERSTR];
} PMSG_PASSCHANGE, *LPPMSG_PASSCHANGE;


//----------------------------------------------------------------------------
// [0xF1][0x05] 비밀번호 변경 신청이 왔다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Result;
} PMSG_PASSCHANGE_RESULT, *LPPMSG_PASSCHANGE_RESULT;

//----------------------------------------------------------------------------
// [0xF3][0x00] 캐릭터 리스트 카운트
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE subcode;
#ifdef DARKLORD_WORK
	BYTE	MaxClass;			// 만들수 있는 클래스수 (기본 : 2 마검사 :3 다크로드 :4)
#endif
#ifdef CHARACTER_MOVE_20040810
	BYTE	MoveCnt;
#endif
	BYTE Count;
} PMSG_CHARLISTCOUNT, *LPPMSG_CHARLISTCOUNT;

//----------------------------------------------------------------------------
// [0xF3][0x00] 캐릭터 정보(위 프로토콜 카운터 만큼 반복됨)
//----------------------------------------------------------------------------
typedef struct
{
	BYTE Index;
	char Name[MAX_IDSTRING];
	WORD Level;
	BYTE CtlCode;			
	
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE CharSet[MAX_PREVIEWCHARSET+9];		// 캐릭터 세트 (아이템)
#else
#ifdef DARKLORD_WORK	
	BYTE CharSet[MAX_PREVIEWCHARSET+4];		// 캐릭터 세트 (아이템)
#else
	BYTE CharSet[MAX_PREVIEWCHARSET+2];		// 캐릭터 세트 (아이템)
#endif
#endif


	
#ifdef ADD_CHARACTERLIST_GUILD_STATUS_01_20050126
	BYTE btGuildStatus;
#endif
	
} PMSG_CHARLIST, * LPPMSG_CHARLIST;
/* 
BYTE CharSet[MAX_PREVIEWCHARSET+1];	// 캐릭터 세트 (아이템)
Class 가 빠지고 +1 된것은 메모리 카피를 하기 위해서다.
*/

//----------------------------------------------------------------------------
// [0xF3][0x01] 클라이언트가 캐릭터 만들기 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD h;
	BYTE	subcode;
	char    Name[MAX_IDSTRING];	
	BYTE    ClassSkin;			// 종족/스킨모양	
} PMSG_CHARCREATE, *LPPMSG_CHARCREATE;

//----------------------------------------------------------------------------
// [0xF3][0x01] 캐릭터 만들기 결과 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD h;
	BYTE	subcode;
	BYTE    Result;
	BYTE	Name[MAX_IDSTRING];
	BYTE	pos;
#ifdef DARKLORD_WORK
	WORD	Level;			// 생성 레벨 추가
#endif	

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	BYTE	Class;
#else
	#ifdef ADD_CLASSINFO_ON_CHARCTER_CREATION_20041224
		BYTE	Class;
	#endif
#endif

	BYTE    Equipment[24];	// 캐릭터가 입고있는 것들
} PMSG_CHARCREATERESULT, *LPPMSG_CHARCREATERESULT;

//----------------------------------------------------------------------------
// [0xF3][0x02] 캐릭터 삭제 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	char		Name[MAX_IDSTRING];
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424	
	char		LastJoominNumber[MAX_JOOMINNUMBER_LENGTH];
#else
	char		LastJoominNumber[10];
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
	//char		LastJoominNumber[8];
} PMSG_CHARDELETE, *LPPMSG_CHARDELETE;

//----------------------------------------------------------------------------
// [0xF3][0x03] 선택한 캐릭터로 맵 진입 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	char		Name[MAX_IDSTRING];
} PMSG_CHARMAPJOIN, *LPPMSG_CHARMAPJOIN;

//----------------------------------------------------------------------------
// [0xF3][0x03] 선택한 캐릭터로 맵 진입 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD  h;	
	BYTE		subcode;

	BYTE		MapX;
	BYTE		MapY;
	BYTE		MapNumber;
	
	BYTE		Dir;
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 마스터레벨 경험치를 위해 타입을 바꾼다. DWORD->INT64
	BYTE		btExp1;					// 경험치 (INT64의 최상위 바이트 부터 1)
	BYTE		btExp2;					// 경험치 (INT64의 최상위 바이트 부터 2)
	BYTE		btExp3;					// 경험치 (INT64의 최상위 바이트 부터 3)
	BYTE		btExp4;					// 경험치 (INT64의 최상위 바이트 부터 4)
	BYTE		btExp5;					// 경험치 (INT64의 최상위 바이트 부터 5)
	BYTE		btExp6;					// 경험치 (INT64의 최상위 바이트 부터 6)
	BYTE		btExp7;					// 경험치 (INT64의 최상위 바이트 부터 7)
	BYTE		btExp8;					// 경험치 (INT64의 최상위 바이트 부터 8)

	BYTE		btNextExp1;				// 다음 경험치 (INT64의 최상위 바이트 부터 1)
	BYTE		btNextExp2;				// 다음 경험치 (INT64의 최상위 바이트 부터 2)
	BYTE		btNextExp3;				// 다음 경험치 (INT64의 최상위 바이트 부터 3)
	BYTE		btNextExp4;				// 다음 경험치 (INT64의 최상위 바이트 부터 4)
	BYTE		btNextExp5;				// 다음 경험치 (INT64의 최상위 바이트 부터 5)
	BYTE		btNextExp6;				// 다음 경험치 (INT64의 최상위 바이트 부터 6)
	BYTE		btNextExp7;				// 다음 경험치 (INT64의 최상위 바이트 부터 7)
	BYTE		btNextExp8;				// 다음 경험치 (INT64의 최상위 바이트 부터 8)
#else
	DWORD		Exp;
	DWORD		NextExp;
#endif
	WORD		LevelUpPoint;
	WORD		Str;
	WORD		Dex;
	WORD		Vit;
	WORD		Energy;
	WORD		Life;
	WORD		MaxLife;
	WORD		Mana;
	WORD		MaxMana;

#ifdef ADD_SHIELD_POINT_01_20060403
	WORD		wShield;
	WORD		wMaxShield;
#endif
	
#ifdef NEW_FORSKYLAND2
	WORD		BP;
	WORD		MaxBP;
#endif

	int			Money;
	BYTE		PkLevel;
	BYTE		CtlCode;
	short		AddPoint;
	short		MaxAddPoint;
	
#ifdef DARKLORD_WORK
	WORD		Leadership;			// 통솔
#endif
	
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	WORD		wMinusPoint;		// 현재 감소 포인트
	WORD		wMaxMinusPoint;		// 최대 감소 포인트
#endif

} PMSG_CHARMAPJOINRESULT, *LPPMSG_CHARMAPJOINRESULT;

//----------------------------------------------------------------------------
// [0xF3][0x04] 죽었다가 다시 리젠될때 메시지
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD  h;	
	BYTE		subcode;

	BYTE		MapX;
	BYTE		MapY;
	BYTE		MapNumber;
	
	BYTE		Dir;
	WORD		Life;
	WORD		Mana;

#ifdef ADD_SHIELD_POINT_01_20060403
	WORD		wShield;
#endif

	WORD		BP;
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 마스터레벨 경험치를 위해 타입을 바꾼다. DWORD->INT64
	BYTE		btExp1;					// 경험치 (INT64의 최상위 바이트 부터 1)
	BYTE		btExp2;					// 경험치 (INT64의 최상위 바이트 부터 2)
	BYTE		btExp3;					// 경험치 (INT64의 최상위 바이트 부터 3)
	BYTE		btExp4;					// 경험치 (INT64의 최상위 바이트 부터 4)
	BYTE		btExp5;					// 경험치 (INT64의 최상위 바이트 부터 5)
	BYTE		btExp6;					// 경험치 (INT64의 최상위 바이트 부터 6)
	BYTE		btExp7;					// 경험치 (INT64의 최상위 바이트 부터 7)
	BYTE		btExp8;					// 경험치 (INT64의 최상위 바이트 부터 8)
#else
	DWORD		Exp;
#endif
	DWORD		Money;
} PMSG_CHARREGEN, *LPPMSG_CHARREGEN;


//----------------------------------------------------------------------------
// [0xF3][0x05] 유저가 레벨업을 했다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE subcode;

	WORD		Level;
	WORD        LevelUpPoint;
	WORD		MaxLife;
	WORD		MaxMana;

#ifdef ADD_SHIELD_POINT_01_20060403
	WORD		wMaxShield;
#endif
	
	WORD		MaxBP;
	short		AddPoint;
	short		MaxAddPoint;

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	short		MinusPoint;
	short		MaxMinusPoint;
#endif

} PMSG_LEVELUP, *LPPMSG_LEVELUP;


//----------------------------------------------------------------------------
// [0xF3][0x06] 유저가 레벨업 포인트를 하나 증가시켰다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE subcode;

	BYTE		Type;	// 0 : 힘 1:민첩성 2:체력 3:에너지
} PMSG_LVPOINTADD, *LPPMSG_LVPOINTADD;

//----------------------------------------------------------------------------
// [0xF3][0x06] 유저가 레벨업 포인트를 하나 증가 한다고 알리면 결과를 알린다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE subcode;

	BYTE	ResultType;	// 4Bit:Result,  4Bit:Type
	WORD		MaxLifeAndMana;	// type에 체력이나 에너지가 증가되었다면 변동 값을 알려준다.
#ifdef ADD_SHIELD_POINT_01_20060403
	WORD		wMaxShield;
#endif

	WORD		MaxBP;
} PMSG_LVPOINTADDRESULT, *LPPMSG_LVPOINTADDRESULT;

//----------------------------------------------------------------------------
// [0xF3][0x07] Damage 감소 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE subcode;
	BYTE DamageH;
	BYTE DamageL;
#ifdef ADD_SHIELD_POINT_01_20060403
	BYTE btShieldDamageH;
	BYTE btShieldDamageL;
#endif
} PMSG_DAMAGE, *LPPMSG_DAMAGE;

//----------------------------------------------------------------------------
// [0xF3][0x08] PKLevel 전송
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE subcode;
	BYTE NumberH;
	BYTE NumberL;
	BYTE PkLevel;
} PMSG_PKLEVEL, *LPPMSG_PKLEVEL;


//----------------------------------------------------------------------------
// [0xF3][0x10] 캐릭터의 인벤토리에 저장되어있는 아이템들..
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE subcode;
	BYTE Count;
} PMSG_INVENTORYLISTCOUNT, *LPPMSG_INVENTORYLISTCOUNT;

typedef struct
{
	BYTE Pos;
	BYTE ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_INVENTORYLIST, *LPPMSG_INVENTORYLIST;


//----------------------------------------------------------------------------
// [0xF3][0x11] 마법 리스트
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Count;

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	BYTE		btListType;	// 0x00 : Default,	0x01 : Status Skill
#endif

} PMSG_MAGICLISTCOUNT, *LPPMSG_MAGICLISTCOUNT;

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// PMSG_MAGICLIST 패킷변경
#ifdef MODIFY_SKLL_LIST_SIZE_BUFFIX_20080221
#pragma pack(push,1)
#endif	// MODIFY_SKLL_LIST_SIZE_BUFFIX_20080221

typedef struct
{
	char		Pos;
	WORD		wSkillNum;
	BYTE		btSkillLevel;
} PMSG_MAGICLIST, *LPPMSG_MAGICLIST;

#ifdef MODIFY_SKLL_LIST_SIZE_BUFFIX_20080221
#pragma pack(pop)
#endif	// MODIFY_SKLL_LIST_SIZE_BUFFIX_20080221
#else
typedef struct
{
	char		Pos;
	BYTE		MagicInfo[2];
} PMSG_MAGICLIST, *LPPMSG_MAGICLIST;
#endif

//----------------------------------------------------------------------------
// [0xF3][0x13] 캐릭터 장비 리스트
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		NumberH;
	BYTE		NumberL;

#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE		Equipment[MAX_PREVIEWCHARSET+9];
#else
#ifdef DARKLORD_WORK	
	BYTE		Equipment[MAX_PREVIEWCHARSET+4];
#else
	BYTE		Equipment[MAX_PREVIEWCHARSET+3];
#endif
#endif
} PMSG_EQUIPMENTLIST, *LPPMSG_EQUIPMENTLIST;

// [0xF3][0x14] 인벤토리 아이템 정보의 수정된 정보를 보낸다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Pos;

	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_INVENTORYITEMMODIFY, *LPPMSG_INVENTORYITEMMODIFY;

// [0xF3][0x20] 소환한 몬스터의 체력을 보낸다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Life;
} PMSG_RECALLMONLIFE, *LPPMSG_RECALLMONLIFE;


#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	enum eMatchType
	{
		eMatchType_BattleSoccer = 0,
		eMatchType_GuildWar,
		eMatchType_Dual,
		eMatchType_WaitTime,
	};
	// GS==>CS [0xF3][0x22] 경기 시간 
	typedef struct
	{
		PBMSG_HEAD	h;
		BYTE		subcode;
		BYTE		MatchType;		// 0: 배틀사커 1: 길드전 2: 결투
		WORD		MatchTime;		// 경기시간 : 단위(초) - 시작값은 1부터 증가 
	} PMSG_MATCH_TIMEVIEW, *LPPMSG_MATCH_TIMEVIEW;

	// GS==>CS [0xF3][0x24] 경기 결과
	typedef struct
	{
		PBMSG_HEAD	h;
		BYTE		subcode;

		BYTE		MatchType;		// 0: 배틀사커 1: 길드전 2: 결투
									// 결투의 경우 MatchTeamName1은 해당 캐릭터의 이름
									// #define MAX_IDSTRING 10	: 서버 문자열 NULL처리 안함
									// 경기는 항상 2팀 진행하기 때문에 참가 팀수 설정 없음
		
		char		MatchTeamName1[MAX_IDSTRING];		// 팀이름1(결투의 경우 캐릭터이름1)
		WORD		Score1;								// 점수1

		char		MatchTeamName2[MAX_IDSTRING];
		WORD		Score2;		
	} PMSG_MATCH_RESULT, *LPPMSG_MATCH_RESULT;

	// GS==>CS [0xF3][0x25] 골넣을 넣었다.
	typedef struct
	{
		PBMSG_HEAD	h;
		BYTE		subcode;

		BYTE		X;			// 공위치
		BYTE		Y;		
	} PMSG_MATCH_GOAL, *LPPMSG_MATCH_GOAL;
#else
	// [0xF3][0x22] 시간을 표시한다. (전투 축구)
	typedef struct
	{
		PBMSG_HEAD	h;
		BYTE		subcode;
		WORD		Second;
	} PMSG_TIMEVIEW, *LPPMSG_TIMEVIEW;
#endif


// [0xF3][0x23] 골을 알린다. (전투 축구)
typedef struct
{
	PBMSG_HEAD	h; //0x00
	BYTE		subcode; //0x03

	char		RedTeamName[MAX_GUILDNAMESTRING]; //0x04
	BYTE		RedTeamScore; //0x0C

	char		BlueTeamName[MAX_GUILDNAMESTRING]; //0x0D
	BYTE		BlueTeamScore; //0x15

} PMSG_GOALSEND, *LPPMSG_GOALSEND;


// [0xF3][0x30] 스킬 키 값을 주고 받는다.
#ifdef MODIFY_R_HOTKEY_SAVE_BUGFIX_20080910
#pragma pack(push, 1)
#endif // MODIFY_R_HOTKEY_SAVE_BUGFIX_20080910
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
#ifdef UPDATE_SKILLKEY_EXPAND_20080804
	BYTE		SKillKey[20];
#else	// UPDATE_SKILLKEY_EXPAND_20080804
	BYTE		SKillKey[10];
#endif	// UPDATE_SKILLKEY_EXPAND_20080804
	BYTE		GameOption;
	BYTE		QkeyDefine;
	BYTE		WkeyDefine;
	BYTE		EkeyDefine;

#ifdef VER_CHATWINDOW_OPTION
	BYTE		ChatWindow;
#endif
	
#ifdef ADD_SEASON_3_NEW_UI_20071122 
	BYTE		RkeyDefine;	
#endif
	
#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
	int			QWERLevel;	// 클라이언트는 BYTE QWERLevel[4] 사용
#endif
} PMSG_SKILLKEY, *LPPMSG_SKILLKEY;
#ifdef MODIFY_R_HOTKEY_SAVE_BUGFIX_20080910
#pragma pack(pop)
#endif // MODIFY_R_HOTKEY_SAVE_BUGFIX_20080910


// [0xF3][0x40] 서버 명령어 코드
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	BYTE		CmdType;		// 명령어 type 0 : 폭죽, 1 : 메시지 박스
	
	BYTE		X;				// 0 일때 x, y 좌표,   1일때, X가 메시지 번호
	BYTE		Y;		
} PMSG_SERVERCMD, *LPPMSG_SERVERCMD;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// Clinet <-> GS 프로토콜
//----------------------------------------------------------------------------
// GC [0xF3][0x50] 마스터레벨 정보
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD  h;	
	BYTE		subcode;	
	
	short		nMLevel;					// 마스터레벨

	BYTE		btMLExp1;					// 현재 마스터레벨 경험치 (INT64의 최상위 바이트 부터 1)
	BYTE		btMLExp2;					// 현재 마스터레벨 경험치 (INT64의 최상위 바이트 부터 2)
	BYTE		btMLExp3;					// 현재 마스터레벨 경험치 (INT64의 최상위 바이트 부터 3)
	BYTE		btMLExp4;					// 현재 마스터레벨 경험치 (INT64의 최상위 바이트 부터 4)
	BYTE		btMLExp5;					// 현재 마스터레벨 경험치 (INT64의 최상위 바이트 부터 5)
	BYTE		btMLExp6;					// 현재 마스터레벨 경험치 (INT64의 최상위 바이트 부터 6)
	BYTE		btMLExp7;					// 현재 마스터레벨 경험치 (INT64의 최상위 바이트 부터 7)
	BYTE		btMLExp8;					// 현재 마스터레벨 경험치 (INT64의 최상위 바이트 부터 8)

	BYTE		btMLNextExp1;				// 다음 마스터레벨 경험치 (INT64의 최상위 바이트 부터 1)
	BYTE		btMLNextExp2;				// 다음 마스터레벨 경험치 (INT64의 최상위 바이트 부터 2)
	BYTE		btMLNextExp3;				// 다음 마스터레벨 경험치 (INT64의 최상위 바이트 부터 3)
	BYTE		btMLNextExp4;				// 다음 마스터레벨 경험치 (INT64의 최상위 바이트 부터 4)
	BYTE		btMLNextExp5;				// 다음 마스터레벨 경험치 (INT64의 최상위 바이트 부터 5)
	BYTE		btMLNextExp6;				// 다음 마스터레벨 경험치 (INT64의 최상위 바이트 부터 6)
	BYTE		btMLNextExp7;				// 다음 마스터레벨 경험치 (INT64의 최상위 바이트 부터 7)
	BYTE		btMLNextExp8;				// 다음 마스터레벨 경험치 (INT64의 최상위 바이트 부터 8)

	short		nMLPoint;					// 마스터레벨 포인트

	// 마스터레벨에 의해 정보가 재설정된다.
	WORD		wMaxLife;
	WORD		wMaxMana;
	WORD		wMaxShield;
	WORD		wMaxBP;	

	// 마스터스킬 및 마스터스킬 트리 정보(미정)
} PMSG_MASTERLEVEL_INFO, *LPPMSG_MASTERLEVEL_INFO;


//----------------------------------------------------------------------------
// GC [0xF3][0x51] 마스터레벨이 올랐다. 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	short		nMLevel;				// 현재 마스터레벨

	short		nAddMLPoint;			// 추가된 마스터레벨 포인트
	short       nMLPoint;				// 사용 가능한 마스터레벨 포인트	
	short		nMaxMLPoint;			// 최대 마스터레벨 포인트

	WORD		wMaxLife;
	WORD		wMaxMana;
	WORD		wMaxShield;
	WORD		wMaxBP;
} PMSG_MASTERLEVEL_UP, *LPPMSG_MASTERLEVEL_UP;
	
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912


#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// Clinet <-> GS 프로토콜
//----------------------------------------------------------------------------
// CG [0xF3][0x52] 마스터레벨 스킬 습득 요청
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	int			iMasterLevelSkill;	// 습득하려는 스킬 번호
} PMSG_REQ_MASTERLEVEL_SKILL, *LPPMSG_REQ_MASTERLEVEL_SKILL;

//----------------------------------------------------------------------------
// GC [0xF3][0x52] 마스터레벨 스킬 습득 요청에 대한 응답
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	BYTE		btResult;	// 결과값	0:에러 1:습득했다. 2:습득 가능한 사용자 아님 3:습득 가능한 스킬번호 아님
							//			4:마스터레벨 포인트 부족 5:그 외

	short       nMLPoint;	// 현재 마스터레벨 포인트

	int			nSkillNum;		// 습득한 스킬 번호 (-1이면 없음)
	int			nSkillLevel;	// 습득한 스킬 레벨
} PMSG_ANS_MASTERLEVEL_SKILL, *LPPMSG_ANS_MASTERLEVEL_SKILL;
#endif

//----------------------------------------------------------------------------
// GC [0x07] 나를 보는 유저들에게 나의 상태를 보낸다
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		State;				// 0 : 해제, 1 : 상태적용

#ifdef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		BuffIndex;
#else
	#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	INT		ViewSkillState;		// 상태코드
	#else
	WORD		ViewSkillState;		// 상태코드
	#endif
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	BYTE		NumberH;
	BYTE		NumberL;
	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE		BuffEffectCount;
	BYTE		BuffEffectList[MAX_BUFF_SLOT_INDEX];
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
} PMSG_VIEWSKILLSTATE, *LPPMSG_VIEWSKILLSTATE;

//----------------------------------------------------------------------------
// 이벤트 상태 알림
// 0x0B
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		State;		// 상태 ( 0 == 활성화 , 1 == 비활성화 )
	BYTE		Event;		// 이벤트 번호 ( 1 == 환수 드래곤, 2 == 타마짱 )
} PMSG_EVENT, *LPPMSG_EVENT;



//----------------------------------------------------------------------------

// 날씨 조절 프로토콜
// 0x0F
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Weather;
} PMSG_WEATHER, *LPPMSG_WEATHER;

//----------------------------------------------------------------------------
// CG [PACKET_MOVE] : 클라이언트가 자신의 움직을 패스를 보낸다.
//----------------------------------------------------------------------------
typedef struct
{	
	PBMSG_HEAD	h;
	
	BYTE	X;
	BYTE	Y;
	BYTE	Path[MAX_PATHDIR];
} PMSG_MOVE, * LPPMSG_MOVE;

//----------------------------------------------------------------------------
// GC [PACKET_MOVE] : 다른클라이언트의 움직인 정보
//----------------------------------------------------------------------------
typedef struct
{	
	PBMSG_HEAD	h;
	
	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	X;
	BYTE	Y;	
	BYTE	Path;//MAX_PATHDIR];
} PMSG_RECVMOVE, * LPPMSG_RECVMOVE;

//----------------------------------------------------------------------------
// CG [PACKET_POSITION] : 좌표 지정용
//----------------------------------------------------------------------------
typedef struct
{	
	PBMSG_HEAD	h;
	BYTE	X;
	BYTE	Y;
} PMSG_POSISTION_SET, * LPPMSG_POSISTION_SET;

//----------------------------------------------------------------------------
// GC [PACKET_POSITION] : 다른사람의 좌표
//----------------------------------------------------------------------------
typedef struct
{	
	PBMSG_HEAD	h;
	
	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	X;
	BYTE	Y;	
} PMSG_RECV_POSISTION_SET, * LPPMSG_RECV_POSISTION_SET;

//----------------------------------------------------------------------------
// 사용자가 게임을 시작할 때 캐릭터 생성 정보를 보낸다.
// 0x11
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;	
	BYTE		subcode;

	BYTE		NumberH;
	BYTE		NumberL;	
	BYTE		X;
	BYTE		Y;
	BYTE		MapNumber;
	BYTE		Dir;
} PMSG_USERMAPJOIN, *LPPMSG_USERMAPJOIN;

//----------------------------------------------------------------------------
// 다른 캐릭터가 뷰포트 안에 들어왔다고 알린다.
// 0x12
//----------------------------------------------------------------------------
typedef struct 
{
	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	X;
	BYTE	Y;
	
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE	CharSet[MAX_PREVIEWCHARSET+9];		// 캐릭터 세트 (아이템)
#else
#ifdef DARKLORD_WORK
	BYTE	CharSet[MAX_PREVIEWCHARSET+4];		// 캐릭터 세트 (아이템)
#else
	BYTE	CharSet[MAX_PREVIEWCHARSET+3];	// 캐릭터 세트 (아이템)
#endif
#endif

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	DWORD		ViewSkillState;
	#else
		#ifdef CHANGE_VIEWSTATE	
		WORD	ViewSkillState;
		#else
		BYTE	ViewSkillState;
		#endif
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	char	Id[MAX_IDSTRING];
	BYTE	TX;
	BYTE	TY;
	BYTE	DirAndPkLevel;

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE	BuffEffectCount;
	BYTE	BuffEffectList[MAX_BUFF_SLOT_INDEX];
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
} PMSG_VIEWPORTCREATE, * LPPMSG_VIEWPORTCREATE;


//----------------------------------------------------------------------------
// 몬스터가 뷰포트 안에 들어왔다고 알린다.
// 0x13
//----------------------------------------------------------------------------
typedef struct 
{
	BYTE	NumberH;
	BYTE	NumberL;
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	BYTE	Type_HI;
	BYTE	Type_LO;
#else
	BYTE	Type;
#endif

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	DWORD		ViewState;
	#else
		#ifdef CHANGE_VIEWSTATE	
		WORD	ViewState;
		#else
		BYTE	ViewState;
		#endif
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	BYTE	X;
	BYTE	Y;
	BYTE	TX;
	BYTE	TY;
	BYTE	Path;
	//BYTE	Path[MAX_PATHDIR];

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE	BuffEffectCount;
	BYTE	BuffEffectList[MAX_BUFF_SLOT_INDEX];
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
} PMSG_MONSTER_VIEWPORTCREATE, * LPPMSG_MONSTER_VIEWPORTCREATE;

//----------------------------------------------------------------------------
// 몬스터가 뷰포트 안에 들어왔다고 알린다.
// 0x1F
//----------------------------------------------------------------------------
typedef struct 
{
	BYTE	NumberH;
	BYTE	NumberL;
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	BYTE	Type_HI;
	BYTE	Type_LO;
#else
	BYTE	Type;
#endif

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	INT		ViewState;
	#else
		#ifdef CHANGE_VIEWSTATE	
		WORD	ViewState;
		#else
		BYTE	ViewState;
		#endif
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	BYTE	X;
	BYTE	Y;
	BYTE	TX;
	BYTE	TY;
	BYTE	Path;	
	BYTE	Id[MAX_IDSTRING];

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE	BuffEffectCount;
	BYTE	BuffEffectList[MAX_BUFF_SLOT_INDEX];
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
} PMSG_CALLMONSTER_VIEWPORTCREATE, * LPPMSG_CALLMONSTER_VIEWPORTCREATE;


//----------------------------------------------------------------------------
// 0x14
// 다른 유저가 뷰포트에서 사라졌다.
// PBMSG_COUNT 뒤에 따라 붙는다
//----------------------------------------------------------------------------
typedef struct 
{
	BYTE	NumberH;	// 캐릭터 번호를 둘로 나눈다.
	BYTE	NumberL;	
} PMSG_VIEWPORTDESTROY, *LPPMSG_VIEWPORTDESTROY;

//----------------------------------------------------------------------------
// CG [PACKET_ATTACK] 캐릭터가 공격을 가 했다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	AttackAction;
	BYTE	DirDis;
} PMSG_ATTACK, *LPPMSG_ATTACK;

//----------------------------------------------------------------------------
// GC [PACKET_ATTACK] 캐릭터 공격에 대한 결과를 알려준다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	DamageH;
	BYTE	DamageL;

#ifdef ADD_SKILL_WITH_COMBO
	BYTE	DamageType;
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	BYTE	btShieldDamageH;
	BYTE	btShieldDamageL;
#endif

} PMSG_ATTACKRESULT, *LPPMSG_ATTACKRESULT;

//----------------------------------------------------------------------------
// GC [0x16] 누군가를 죽였다면(공격을 가한 유저한테)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	ExpH;
	BYTE	ExpL;
	BYTE	DamageH;
	BYTE	DamageL;
	
//#ifdef ADD_SKILL_WITH_COMBO
//	BYTE	Skill;		// 죽었을때 어떤 스킬에 당한지에 대한 번호 
//#endif
} PMSG_KILLPLAYER, *LPPMSG_KILLPLAYER;

//----------------------------------------------------------------------------
// GC [0x17] 누군가가 죽었다면(뷰포트의 다른 유저들에게)
// 번호가 자신이라면 자신이 죽은것임
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;

#ifdef ADD_SKILL_WITH_COMBO
	#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		BYTE	MagicNumberH;
		BYTE	MagicNumberL;
	#else
		BYTE	Skill;
	#endif
	BYTE	KillerNumberH;
	BYTE	KillerNumberL;
#endif
} PMSG_DIEPLAYER, *LPPMSG_DIEPLAYER;


//----------------------------------------------------------------------------
// CG [0x18] 캐릭터가 특별한 동작을 취했다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	Dir;
	BYTE	ActionNumber;

#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
	BYTE	iTargetIndexH;
	BYTE	iTargetIndexL;
#endif

} PMSG_ACTION, *LPPMSG_ACTION;

//----------------------------------------------------------------------------
// GC [0x18] 다른 캐릭터가 특별한 동작을 취했다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	Dir;
	BYTE	ActionNumber;
	BYTE TargetNumberH;	// 7
	BYTE TargetNumberL;	// 8
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
	BYTE	TargetNumberH;
	BYTE	TargetNumberL;
#endif
} 
PMSG_ACTIONRESULT, *LPPMSG_ACTIONRESULT;

//----------------------------------------------------------------------------
// CG [0x19] 마법공격
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH;
	BYTE		MagicNumberL;
#else
	BYTE		MagicNumber;
#endif
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		Dis;
} PMSG_MAGICATTACK, *LPPMSG_MAGICATTACK;

//----------------------------------------------------------------------------
// GC [0x19] 마법공격 결과 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH;
	BYTE		MagicNumberL;
#else
	BYTE		MagicNumber;
#endif
	BYTE		SourceNumberH;
	BYTE		SourceNumberL;
	BYTE		TargetNumberH;
	BYTE		TargetNumberL;
} PMSG_MAGICATTACK_RESULT, *LPPMSG_MAGICATTACK_RESULT;

//----------------------------------------------------------------------------
// CG [0x1A] 특정 위치에 마법 공격
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		MagicNumber;
	BYTE		MapX;
	BYTE		MapY;
} PMSG_POSMAGIC, *LPPMSG_POSMAGIC;

//----------------------------------------------------------------------------
// CG [0x1A] 특정 위치에 마법 공격 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		MagicNumber;
	BYTE		MapX;
	BYTE		MapY;
	BYTE		Count;
} PMSG_POSMAGIC_COUNT, *LPPMSG_POSMAGIC_COUNT;

//----------------------------------------------------------------------------
// CG [0x1A] 특정 위치에 마법 공격 결과
//----------------------------------------------------------------------------
typedef struct
{
	BYTE	NumberH;
	BYTE	NumberL;
} PMSG_POSMAGIC_RESULT, *LPPMSG_POSMAGIC_RESULT;

//----------------------------------------------------------------------------
// GC [0x1B] 지속 마법 해제
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH;
	BYTE		MagicNumberL;
#else
	BYTE		MagicNumber;
#endif
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_MAGICCANCEL, *LPPMSG_MAGICCANCEL;

//----------------------------------------------------------------------------
// GC [0x2D] 아이템 효과 해제
//----------------------------------------------------------------------------
#ifdef ITEM_ADD_OPTION_20061019
typedef struct
{
	PBMSG_HEAD	h;

	WORD		wOptionType;
	WORD		wEffectType;

	BYTE		byEffectOption;		// 효과 옵션 (0 : 사용, 1 : 사용 시간 종료, 2 : 취소)

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	int			iLeftTime;
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#if defined MODIFY_BUFF_SYSTEM_EXTENTION_20070917 && !defined ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205			// !! NOT
	int			iLeftTime;
#endif // defined MODIFY_BUFF_SYSTEM_EXTENTION_20070917 && !defined ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE		btBuffEffectType;	// 확장 버프효과 인덱스
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
} PMSG_USEEFFECTITEM, *LPPMSG_USEEFFECTITEM;
#endif // ITEM_ADD_OPTION_20061019

//----------------------------------------------------------------------------
// CG [0x1C] 공간이동 사용 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_SEASON_3_NEW_MAP_20070910
	WORD		MoveNumber;
#else
	BYTE		MoveNumber;
#endif	
	BYTE		MapX;
	BYTE		MapY;
} PMSG_TELEPORT, *LPPMSG_TELEPORT;

//----------------------------------------------------------------------------
// CG [0x1C] 공간이동 결과 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_SEASON_3_NEW_MAP_20070910
	WORD		MoveNumber;
#else
	BYTE		MoveNumber;
#endif
	BYTE		MapNumber;
	BYTE		MapX;
	BYTE		MapY;
	BYTE		Dir;
} PMSG_TELEPORT_RESULT, *LPPMSG_TELEPORT_RESULT;

//----------------------------------------------------------------------------
// CG [PACKET_MAGIC_ATTACK] 나의 공격에 다른 캐릭터가 당했을때 카운터
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH;
	BYTE		MagicNumberL;
#else
	BYTE		MagicNumber;
#endif
	BYTE		X;
	BYTE		Y;
	BYTE		Serial;
	BYTE		Count;
} PMSG_BEATTACK_COUNT, *LPPMSG_BEATTACK_COUNT;

//----------------------------------------------------------------------------
// CG [PACKET_MAGIC_ATTACK] 나의 공격에 다른 캐릭터가 당했을때 당한 캐릭터들..
//----------------------------------------------------------------------------
typedef struct
{
	BYTE	NumberH;
	BYTE	NumberL;

#ifdef APPLY_DURATION_MAGIC_HACK_CLEANER_20040504
	BYTE	MagicKey;
#endif

} PMSG_BEATTACK, *LPPMSG_BEATTACK;

//----------------------------------------------------------------------------
// CG [0x1E] 지속적인 마법 발사
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH; //0x03
	BYTE		MagicNumberL; //0x04
#else
	BYTE		MagicNumber;
#endif
	BYTE		X; //0x05
	BYTE		Y; //0x06
	BYTE		Dir; //0x07
	
#ifdef NEW_ATTACK_PROC
	BYTE		Dis;
	BYTE		TargetPos;
#endif
#ifdef ADD_SKILL_20031022
	BYTE		Dis; //0x08
	BYTE		TargetPos; //0x09
#endif

#ifdef EXPAND_ATTACK_PACKET_0X1E
	BYTE		NumberH; //0x0A
	BYTE		NumberL; //0x0B
#endif
	
#ifdef APPLY_DURATION_MAGIC_HACK_CLEANER_20040504
	BYTE		MagicKey; //0x0C
#endif

} PMSG_DURATION_MAGIC_RECV, *LPPMSG_DURATION_MAGIC_RECV;

//----------------------------------------------------------------------------
// GC [0x1E] 지속적인 마법 발사
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH;
	BYTE		MagicNumberL;
#else
	BYTE		MagicNumber;
#endif
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		X;
	BYTE		Y;
	BYTE		Dir;
} PMSG_DURATION_MAGIC_SEND, *LPPMSG_DURATION_MAGIC_SEND;


//----------------------------------------------------------------------------
// 0x20
// 뷰포트에 아이템 생성
//----------------------------------------------------------------------------
typedef struct 
{
	BYTE	NumberH;
	BYTE	NumberL;

	BYTE	px;
	BYTE	py;
#if GAME_VERSION >= G_V_99B
	BYTE	ItemInfo[ITEM_BUFFER_SIZE];
#else
	BYTE	ItemInfo[ITEM_BUFFER_SIZE+1];
#endif
} PMSG_ITEMVIEWPORTCREATE;

//----------------------------------------------------------------------------
// 사용자가 아이템 줍겠다고 요청
// 0x22
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
} PMSG_ITEMGETREQUEST, *LPPMSG_ITEMGETREQUEST;

//----------------------------------------------------------------------------
// 아이템을 줍겠다고 요청한 유저에게 결과를 알린다.
// 0x22
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE	result;					// 0xFE - 돈, 0xFD - 겹쳐지는 아이템 (필드에서만 없앤다.)

	BYTE	Data[ITEM_BUFFER_SIZE];	// 돈은 4바이트. 아이템은 5바이트 보냄
} PMSG_ITEMGETRESULT, *LPPMSG_ITEMGETRESULT;

//----------------------------------------------------------------------------
// 유저가 아이템을 버리기 요청
// 0x23 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		px;
	BYTE		py;
	BYTE		Ipos;	// 인벤토리에서의 위치 
} PMSG_ITEMTHROW, *LPPMSG_ITEMTHROW;

//----------------------------------------------------------------------------
// 유저가 아이템을 버리기 결과
// 0x23 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	BYTE		Ipos;	// 인벤토리에서의 위치 
} PMSG_ITEMTHROW_RESULT, *LPPMSG_ITEMTHROW_RESULT;


//----------------------------------------------------------------------------
// 인벤토리에서 아이템을 이동하다
// 0x24
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE	sFlag;
	BYTE	source_item_num;

	BYTE	sItemInfo[ITEM_BUFFER_SIZE];
	BYTE	tFlag;
	BYTE	target_item_num;
} PMSG_INVENTORYITEMMOVE, *LPPMSG_INVENTORYITEMMOVE;
// 0x24
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		result;
	BYTE		Pos;
	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_INVENTORYITEMMOVE_RESULT, *LPPMSG_INVENTORYITEMMOVE_RESULT;

//----------------------------------------------------------------------------
// 다른 장비를 바꿨다.
// 0x25
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;

	BYTE	ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_USEREQUIPMENTCHANGED, *LPPMSG_USEREQUIPMENTCHANGED;


//----------------------------------------------------------------------------
// 캐릭터 생명력을 다시 채워주는 용도로 사용된다.
// CS : [0x26]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		inventoryPos;		// 인벤토리에서 위치 번호
	BYTE		invenrotyTarget;	// 사용하는 대상 아이템	

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	BYTE		btItemUseType;		// 아이템을 사용하는 방법. 스탯열매의 경우 생성/감소 표현
									// 1) 스탯열매 : 0x00 스탯생성 0x01 스탯감소
#endif

} PMSG_USEITEM, *LPPMSG_USEITEM;

//----------------------------------------------------------------------------
// 캐릭터 생명력을 다시 채워주는 용도로 사용된다.
// SC : [0x26]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		IPos;			// OxFF : Life 값임 / OxFE : LifeMax 값임 / 0xFD : Life와 상관없이 클라이언트 아이템제어 활성화
	BYTE		LifeH;
	BYTE		LifeL;
	BYTE		Flag;

#ifdef ADD_SHIELD_POINT_01_20060403
	BYTE		btShieldH;
	BYTE		btShieldL;
#endif
} PMSG_REFILL, *LPPMSG_REFILL;

//----------------------------------------------------------------------------
// 캐릭터 마나양을 보낸다.
// SC : [0x27]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		IPos;
	BYTE		ManaH;
	BYTE		ManaL;
#ifdef NEW_FORSKYLAND2
	BYTE		BPH;	// BP값
	BYTE		BPL;
#endif
} PMSG_MANASEND, *LPPMSG_MANASEND;

//----------------------------------------------------------------------------
// 인벤토리에서 삭제한다.
// SC : [0x28]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		IPos;
	BYTE		Flag;		// 0 이면 
} PMSG_INVENTORYDELETE, *LPPMSG_INVENTORYDELETE;

//----------------------------------------------------------------------------
// 아이템 사용으로 인해 특수 능력 사용을 알림
// SC : [0x29]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Number;
	WORD		Time;
} PMSG_ITEMUSESPECIALTIME, *LPPMSG_ITEMUSESPECIALTIME;

// 특정 아이템의 내구력을 알린다.
// SC : [0x2A]
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		IPos;
	BYTE		Dur;
	BYTE		Flag;
} PMSG_ITEMDUR, *LPPMSG_ITEMDUR;

//----------------------------------------------------------------------------
// 0x2B 아이템을 인벤에 추가 
//----------------------------------------------------------------------------
/* 사용안함
typedef struct
{
	PBMSG_HEAD	h;
	BYTE	Data[4];	// 아이템 4바이트 보냄
} PMSG_ITEMGIVE, *LPPMSG_ITEMGIVE;
*/


#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
//----------------------------------------------------------------------------
// [GC] 0x2C 스탯 열매 사용 결과를 알린다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		result;

#ifdef ADD_PCS_FRUITS_POINT_EXTENTION_20070430
	WORD		btStatValue;		// 증/감 스탯값
#else
	BYTE		btStatValue;		// 증/감 스탯값
#endif // ADD_PCS_FRUITS_POINT_EXTENTION_20070430
	BYTE		btFruitType;		// 열매 종류
} PMSG_USE_STAT_FRUIT, * LPPMSG_USE_STAT_FRUIT;

#endif





// 어떤 캐릭터에게 말을 걸겠다고 요청한다.
// 0x30
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
} PMSG_TALKREQUEST, *LPPMSG_TALKREQUEST;

// 어떤 캐릭터에게 말을 걸겠다고 요청하면 그에대해 답변해준다.
// 0x30
typedef struct
{
	PBMSG_HEAD	h;
	BYTE	result;

	// 악마의 광장 조합 확률
	BYTE	level1;
	BYTE	level2;
	BYTE	level3;
	BYTE	level4;
#ifdef DEVILSQUARE_EXTEND_20050221		// 카오스 조합 추가
	BYTE	level5;
	BYTE	level6;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010	// 광장7 조합확률 추가 전송
	BYTE	level7;
#endif
} PMSG_TALKRESULT, *LPPMSG_TALKRESULT;

// 상점에 있는 아이템 리스트 카운트 및 헤더
// 0x31
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		Type;
	BYTE		count;
} PMSG_SHOPITEMCOUNT, * LPPMSG_SHOPITEMCOUNT;


// 상점에 있는 아이템 리스트
// 0x31
/* 사용안함
typedef struct
{
	BYTE	Pos;
	BYTE	ItemInfo[4];
} PMSG_SHOPITEMLIST, *LPPMSG_SHOPITEMLIST;
*/

// 상점에서 물건을 산다고 요청한다.
// 0x32
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Pos;
} PMSG_BUYREQUEST, *LPPMSG_BUYREQUEST;

// 상점에서 물건을 정상적으로 샀다면 결과를 알려준다.
// 0x32
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;

	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_BUYRESULT, *LPPMSG_BUYRESULT;

// [0x33] 물건 팔기 요청
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Pos;
} PMSG_SELLREQUEST, *LPPMSG_SELLREQUEST;

// [0x33] 물건 팔기 결과 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	DWORD		Money;
} PMSG_SELLRESULT,*LPPMSG_SELLRESULT;

// [0x34] 아이템 내구력 수리 요청
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Position;
	BYTE		Requestpos;	// 요청한곳 0 : 상점 1 : 인벤
} PMSG_ITEMDURREPAIR, *LPPMSG_ITEMDURREPAIR;


// [0x34] 아이템 내구력 수리 결과
typedef struct
{
	PBMSG_HEAD	h;
	int			Money;
} PMSG_ITEMDURREPAIR_RESULT, *LPPMSG_ITEMDURREPAIR_RESULT;


// CS [0x36] 물건 교환 요청한다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_TRADE_REQUEST, *LPPMSG_TRADE_REQUEST;

// SC [0x36] 다른 사람이 물건 교환을 요청 한다.
typedef struct
{
	PBMSG_HEAD	h;
	char		szId[MAX_IDSTRING];	
} PMSG_TRADE_REQUESTSEND, *LPPMSG_TRADE_REQUESTSEND;

// CS [0x37] 물건 교환에 응할 것 인지 알린다. (교환하고자 했던 유저에게)
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Response;
	BYTE		Id[MAX_IDSTRING];
	WORD		Level;				// 레벨 추가
	int			GuildNumber;		// 길드 번호 추가
} PMSG_TRADE_RESPONSE, *LPPMSG_TRADE_RESPONSE;

// SC [0x37] 물건 교환에 응할 것 인가? 에 대한 요청 결과
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Response;
} PMSG_TRADE_RESPONSE_RESULT, *LPPMSG_TRADE_RESPONSE_RESULT;

// SC [0x38] 상대방이 교환창에서 아이템을 제거
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		TradeItemIndex;
	//BYTE		ItemInfo[4];
} PMSG_TRADE_OTHER_DEL, *LPPMSG_TRADE_OTHER_DEL;

// SC [0x39] 상대방이 교환창에 아이템을 올려놓았다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		TradeItemIndex;

	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_TRADE_OTHER_MOVE, *LPPMSG_TRADE_OTHER_MOVE;

// CS [0x3A] 돈을 교환창에 넣는다.
typedef struct
{
	PBMSG_HEAD	h;
	DWORD		Money;
} PMSG_TRADE_GOLD, *LPPMSG_TRADE_GOLD;

// CS [0x3B] 상대가 돈을 교환창에 넣었다.
typedef struct
{
	PBMSG_HEAD	h;
	DWORD		Money;
} PMSG_TRADE_OTHER_GOLD, *LPPMSG_TRADE_OTHER_GOLD;

// CS [0x3C] 교환창 확인 버튼 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Flag;
} PMSG_TRADE_OKBUTTON, *LPPMSG_TRADE_OKBUTTON;

// SC [0x3D] 교환 결과 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} PMSG_TRADE_RESULT, *LPPMSG_TRADE_RESULT;


#ifdef PERSONAL_SHOP_20040113

// SC [0x3F][0x00] 뷰포트 내 외침 (개인상점 개설 이름)
typedef struct
{
	PWMSG_HEAD2	h;
	BYTE		btCount;
} PMSG_ANS_PSHOP_VIEWPORT_NOTIFY, *LPPMSG_ANS_PSHOP_VIEWPORT_NOTIFY;

typedef struct
{
	BYTE		btNumberH;
	BYTE		btNumberL;
	CHAR		szPShopText[MAX_PSHOP_TEXT];
} PMSG_PSHOP_VIEWPORT_NOTIFY, *LPPMSG_PSHOP_VIEWPORT_NOTIFY;


// CS [0x3F][0x01] 개인상점 아이템 가격 설정 -> 기존에 가격이 있으면 변경
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btItemPos;
	BYTE		sItemPrice1;
	BYTE		sItemPrice2;
	BYTE		sItemPrice3;
	BYTE		sItemPrice4;
} PMSG_REQ_PSHOP_SETITEMPRICE, *LPPMSG_REQ_PSHOP_SETITEMPRICE;


// SC [0x3F][0x01] 개인상점 아이템 가격 설정 결과
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;		// 1 : 성공 / 2 : 아이템 위치가 인덱스밖 / 3 : 해당 아이템이 없음 / 4 : 가격이 0 이하임 / 5 : 개인상점 개설자의 레벨이 6이하 / 6 : 블럭된 아이템
	BYTE		btItemPos;
} PMSG_ANS_PSHOP_SETITEMPRICE, *LPPMSG_PSHOP_SETITEMPRICE;


// CS [0x3F][0x02] 개인상점개설 요청 -> 이미 상점이 개설되었으면 이름만 변경
typedef struct
{
	PBMSG_HEAD2	h;
	CHAR		szPShopText[MAX_PSHOP_TEXT];
} PMSG_REQ_PSHOP_OPEN, *LPPMSG_REQ_PSHOP_OPEN;


// SC [0x3F][0x02] 개인상점개설 결과
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;		// 개인상점 개설 결과 - 0x00 : 실패 / 0x01 : 성공 / 0x02 : 개인상점 개설자의 레벨이 6이하 / 0x03 : 상점개설 요청자가 아이템 블록계정
} PMSG_ANS_PSHOP_OPEN, *LPPMSG_ANS_PSHOP_OPEN;


// CS [0x3F][0x03] 개인상점 종료 요청
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_PSHOP_CLOSE, *LPPMSG_REQ_PSHOP_CLOSE;


// SC [0x3F][0x03] 개인상점 종료 결과 -> 동시에 나를 보고있는 뷰포트의 모든 사용자들에게 알려야 함 (같은 프로토콜 사용)
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;		// 개인상점 종료 결과 - 0x00 실패 / 0x01 성공
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_ANS_PSHOP_CLOSE, *LPPMSG_ANS_PSHOP_CLOSE;


//-----------------------------------------------------
// 사는 사람 입장

// CS [0x3F][0x05] 개인상점에서 물건 리스트 요청 -> 상대방의 리스트를 받는다.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		btName[MAX_IDSTRING];
} PMSG_REQ_BUYLIST_FROM_PSHOP, *LPPMSG_REQ_BUYLIST_FROM_PSHOP;


// SC [0x3F][0x05] 개인상점에서 물건 리스트 요청에 대한 응답
// SC [0x3F][0x13] 개인상점에서 물건 리스트 요청에 대한 응답 (처음 보내는것이 아닐 때)
typedef struct
{
	PWMSG_HEAD2	h;
	BYTE		Result;			// 1 : 성공 / 2 : 접속하지 않았음 / 3 : 상점을 열지않았음 / 4 : 아이템블럭 계정이다.
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		btName[MAX_IDSTRING];
	CHAR		szPShopText[MAX_PSHOP_TEXT];
	BYTE		btCount;
} PMSG_ANS_BUYLIST_FROM_PSHOP, *LPPMSG_ANS_BUYLIST_FROM_PSHOP;

typedef struct
{
	BYTE		Pos;
	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
	INT			PShopItemValue;
} PMSG_BUYLIST_FROM_PSHOP, *LPPMSG_BUYLIST_FROM_PSHOP;


// CS [0x3F][0x06] 개인상점에서 특정 물건 구입 요청
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		btName[MAX_IDSTRING];
	BYTE		btItemPos;
} PMSG_REQ_BUYITEM_FROM_PSHOP, *LPPMSG_REQ_BUYITEM_FROM_PSHOP;


// CS [0x3F][0x06] 개인상점에서 특정 물건 구입 요청에 대한 응답
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		Result;			// 1 : 성공 / 2 : 접속하지 않았음 / 3 : 상점을 열지않았음 / 4 : 다른사람과 거래중 
								// 5 : 아이템 위치가 인덱스밖 / 6 : 아이템이 존재하지 않음 / 7 : 젠이 모자름 / 8 : 내 인벤에 빈공간이 없음 / 9 : 상점을 연 내가 아이템 블럭계정임
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		cItemInfo[ITEM_BUFFER_SIZE];
	BYTE		btItemPos;
} PMSG_ANS_BUYITEM_FROM_PSHOP, *LPPMSG_ANS_BUYITEM_FROM_PSHOP;


// CS [0x3F][0x07] 내가 타인의 개인상점 창을 보고 있다가 닫았다.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		btName[MAX_IDSTRING];
} PMSG_REQ_PSHOPDEAL_CLOSE, *LPPMSG_REQ_PSHOPDEAL_CLOSE;


// SC [0x3F][0x12] 내가 물건을 보고 있는 타인이 상점을 폐쇄했다.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_REQ_DEALER_CLOSED_SHOP, *LPPMSG_REQ_DEALER_CLOSED_SHOP;


//-----------------------------------------------------
// 파는 사람 입장

// SC [0x3F][0x08] 자신의 개인상점에서 특정 물건이 판매되었음을 알림 -> 아이템 삭제 이전에 가야할것임
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btItemPos;
	BYTE		btName[MAX_IDSTRING];		// 내 아이템을 산 케릭 이름
} PMSG_ANS_SOLDITEM_FROM_PSHOP, *LPPMSG_ANS_SOLDITEM_FROM_PSHOP;


//-----------------------------------------------------
// 개인상점 뷰포트 처리

// SC [0x3F][0x10] 내가 보고있는 특정 사용자의 상점이름이 변경되었다. -> [0x3F][0x01] 상점이름 변경 즉시 나를 보는 사용자들에게 보내주는 내용
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		btPShopText[MAX_PSHOP_TEXT];
	BYTE		btName[MAX_IDSTRING];
} PMSG_ANS_PSHOP_TEXT_CHANGED, *LPPMSG_ANS_PSHOP_TEXT_CHANGED;


#endif


// CS [0x40] 상대에게 파티를 요청한다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_PARTYREQUEST, *LPPMSG_PARTYREQUEST;


// SC [0x40] 상대가 파티를 요청했다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_PARTYREQUESTSEND, *LPPMSG_PARTYREQUESTSEND;

// CS [0x41] 파티 요청에 대한 결과를 알린다. 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_PARTYREQUESTRESULT, *LPPMSG_PARTYREQUESTRESULT;

// CS [0x42] 파티원 리스트 카운트 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	BYTE		Count;
} PMSG_PARTYLISTCOUNT, *LPPMSG_PARTYLISTCOUNT;

// CS [0x42] 파티원 리스트
typedef struct
{
	char		szId[MAX_IDSTRING];
	BYTE		Number;
	BYTE		MapNumber;
	BYTE		X;
	BYTE		Y;
	int			Life;
	int			MaxLife;
} PMSG_PARTYLIST, *LPPMSG_PARTYLIST;


// CS [0x43] 파티원을 탈퇴 시킨다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Number;
} PMSG_PARTYDELUSER, *LPPMSG_PARTYDELUSER;

// SC [0x43] 파티에서 제외
typedef struct
{
	PBMSG_HEAD	h;	
} PMSG_PARTYDELUSERSEND, *LPPMSG_PARTYDELUSERSEND;

// SC [0x44] 파티원 생명
typedef struct
{
	BYTE Number;
	//BYTE LifeAndMaxLife;
} PMSG_PARTYLIFEALL, *LPPMSG_PARTYLIFEALL;


//----------------------------------------------------------------------------
// 변신한 다른 캐릭터가 뷰포트 안에 들어왔다고 알린다.
// 0x45
//----------------------------------------------------------------------------
typedef struct 
{
	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	X;
	BYTE	Y;
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	BYTE	SkinH;
	BYTE	SkinL;
#else
	BYTE	Skin;
#endif

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	INT		ViewSkillState;
	#else
		#ifdef CHANGE_VIEWSTATE	
		WORD	ViewSkillState;
		#else
		BYTE	ViewSkillState;
		#endif
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	char	Id[MAX_IDSTRING];
	BYTE	TX;
	BYTE	TY;
	BYTE	DirAndPkLevel;

#ifdef UPDATE_VIEWPORTCREATE_CHANGE_ADD_CHARSET_20070314	// 뷰포트에 캐릭터 장비 추
	BYTE	CharSet[MAX_PREVIEWCHARSET+9];		// 캐릭터 세트 (아이템)
#endif
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE	BuffEffectCount;
	BYTE	BuffEffectList[MAX_BUFF_SLOT_INDEX];
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
} PMSG_VIEWPORTCREATE_CHANGE, * LPPMSG_VIEWPORTCREATE_CHANGE;


#ifdef FOR_BLOODCASTLE

//----------------------------------------------------------------------------
// 클라이언트의 특정맵의 특정위치 혹은 범위에 특정 속성을 준다
// 0x46
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE	btType;			// 0 : 범위를 나타냄 (좌X상Y -> 우X하Y 의 두 개의 좌표 값만 보냄 (Count값 무시됨))
							// 1 : 특정위치만 나타냄블록을 할 지역의 X,Y값이 Count개 만큼 들어감
	BYTE	btMapAttr;		// 맵속성 (0x01 : 안전지대, 0x04 : 백, 0x08 : 땅뚫림)
	BYTE	btMapSetType;	// 맵에 해당 속성을 추가할지 제거할지 (0추가 / 1제거)
	BYTE	btCount;		
} PMSG_SETMAPATTR_COUNT, * LPPMSG_SETMAPATTR_COUNT;

typedef struct				// btType이 0이면 이런 몸통이 2개가 붙는다. (좌X상Y, 우X하Y)
{
	BYTE	btX;		
	BYTE	btY;		
} PMSG_SETMAPATTR, * LPPMSG_SETMAPATTR;

#endif

#ifdef SEND_GETITEMINFO_FOR_MYPARTY
//----------------------------------------------------------------------------
// 파티원에게 내가 먹은 아이템 정보를 준다
// 0x47
//----------------------------------------------------------------------------
// 아이템 타입 비트마스크
#ifdef ITEM_INDEX_EXTEND_20050706	// 파티원이 아이템을 주웠을 경우
	#define PARTY_ITEMINFO_ITEMTYPE		0x00001FFF	// 아이템 타입
	#define PARTY_ITEMINFO_SKILL		0x00002000	// 스킬
	#define PARTY_ITEMINFO_LUCK			0x00004000	// 행운
	#define PARTY_ITEMINFO_OP			0x00008000	// 옵션
	#define PARTY_ITEMINFO_EX			0x00010000	// 엑설런트
	#define PARTY_ITEMINFO_SET			0x00020000	// 세트
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		#define PARTY_ITEMINFO_380OPTION 0x00040000  // 380 추가 옵션
	#endif
#else
	#define PARTY_ITEMINFO_ITEMTYPE		0x01FF	// 아이템 타입
	#define PARTY_ITEMINFO_SKILL		0x0200	// 스킬
	#define PARTY_ITEMINFO_LUCK			0x0400	// 행운
	#define PARTY_ITEMINFO_OP			0x0800	// 옵션
	#define PARTY_ITEMINFO_EX			0x1000	// 엑설런트
	#define PARTY_ITEMINFO_SET			0x2000	// 세트
#endif

typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		NumberH;			// 먹은넘 인덱스
	BYTE		NumberL;
	
#ifdef ITEM_INDEX_EXTEND_20050706
	UINT		ItemInfo;			// 아이템 정보
#else
	WORD		ItemInfo;			// 아이템 정보
#endif	
#ifdef SEND_GETITEMINFO_FOR_MYPARTY_ADD_LEVEL	
	BYTE		Level;
#endif
} PMSG_GETITEMINFO_FOR_PARTY, * LPPMSG_GETITEMINFO_FOR_PARTY;
#endif

#ifdef ADD_EFFECT_LEVELUP_20040401
//----------------------------------------------------------------------------
// 물약(치료, 마나) 등을 먹었을 경우 화면상의 다른 유저에게 알림 효과
// 0x48
//----------------------------------------------------------------------------

#define EFFECT_POTION_HP			0x01	// 생명
#define EFFECT_POTION_MP			0x02	// 마나
#define EFFECT_LEVELUP				0x10	// 레벨업

#ifdef ADD_SHIELD_POINT_01_20060403
#define EFFECT_POTION_SD			0x03	// 쉴드 포션
#define EFFECT_SHIELDBROKE			0x11	// 쉴드 파괴 이펙트
#endif

typedef struct 
{
	PBMSG_HEAD	h;	
	
	BYTE		NumberH;		// 먹은넘 인덱스
	BYTE		NumberL;

	BYTE		btType;			// 포션 정보(효과타입)
} PMSG_EFFECTINFO, * LPPMSG_EFFECTINFO;
#endif

//***************************************************************************//
//***************************************************************************//

//----------------------------------------------------------------------------
// CS [0x50] 길드 마스터에게 가입을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_GUILDJOINQ, *LPPMSG_GUILDJOINQ;

//----------------------------------------------------------------------------
// SC [0x50] 상대가 길드 가입을 요청해 왔다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_GUILDQUESTSEND, *LPPMSG_GUILDQUESTSEND;

//----------------------------------------------------------------------------
// CS [0x51] 길드 요청에 대한 결과를 알린다. 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_GUILDQRESULT, *LPPMSG_GUILDQRESULT;

//----------------------------------------------------------------------------
// CS [0x52] 길드원 리스트를 보내준다.
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		Result;
	BYTE		Count;

	int			TotalScore;
	BYTE		Score;

#ifdef ADD_NEW_UNION_01_20041006
	// 적대 길드 정보
	CHAR		szRivalGuild[MAX_GUILDNAMESTRING+1];		
#endif

} PMSG_GUILDLISTCOUNT, *LPPMSG_GUILDLISTCOUNT;

//----------------------------------------------------------------------------
// CS [0x52] 길드원 리스트
//----------------------------------------------------------------------------
typedef struct
{
	char		Name[MAX_IDSTRING];
	BYTE		Number;
	BYTE		ConnectAServer;		// 접속 중인지.. 서버명은?

#ifdef ADD_NEW_GUILD_01_20040913
	BYTE		btGuildStatus;
#endif

} PMSG_GUILDLIST, *LPPMSG_GUILDLIST;

//----------------------------------------------------------------------------
// CS [0x53] 길드에서 제외 시킨다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
	//char		JoominNumber[7];
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
	char		JoominNumber[MAX_JOOMINNUMBER_LENGTH];
#else
	char		JoominNumber[10];
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
} PMSG_GUILDDELUSER, *LPPMSG_GUILDDELUSER;

//----------------------------------------------------------------------------
// SC [0x53] 길드에서 제외
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Result;
} PMSG_GUILDDELUSER_RESULT, *LPPMSG_GUILDDELUSER_RESULT;

//----------------------------------------------------------------------------
// SC [0x54] 길드 마스터가 되겠냐고 물어본다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_GUILDMASTERQUESTION, *LPPMSG_GUILDMASTERQUESTION;

//----------------------------------------------------------------------------
// CS [0x54] 길드 마스터가 되겠냐는 요청에 대한 대답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} PMSG_GUILDMASTERANSWER, *LPPMSG_GUILDMASTERANSWER;

//----------------------------------------------------------------------------
// SC [0x55] 길드 마스터가 될 수 있으니 길드 메니져를 실행하라
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_GUILDMASTERMANAGER_RUN;

//----------------------------------------------------------------------------
// CS [0x55] 길드 메니져 완료 (저장)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

#ifdef ADD_NEW_GUILD_02_20040922
	BYTE		btGuildType;
#endif
	
	char		GuildName[MAX_GUILDNAMESTRING];
	BYTE		Mark[32];
} PMSG_GUILDINFOSAVE, *LPPMSG_GUILDINFOSAVE;


//----------------------------------------------------------------------------
// CS [0x56] 길드 생성 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	
#ifdef ADD_NEW_GUILD_02_20040922
	BYTE		btGuildType;
#endif
	
} PMSG_GUILDCREATED_RESULT, *LPPMSG_GUILDCREATED_RESULT;


//----------------------------------------------------------------------------
// CS [0x56] 다른 길드에 전쟁을 선포한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING];
} PMSG_GUILDWAR, *LPPMSG_GUILDWAR;

//----------------------------------------------------------------------------
// SC [0x57] 다른 길드가 전쟁을 선포해 왔다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING];
} PMSG_GUILDWARQ, *LPPMSG_GUILDWARQ;

//----------------------------------------------------------------------------
// CS [0x57] 전쟁 선포에 대한 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
	BYTE		Result;
} PMSG_GUILDWARA, *LPPMSG_GUILDWARA;

//----------------------------------------------------------------------------
// SC [0x5A] 뷰포트내 길드 정보
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		Count;
} PMSG_GUILDVIEWPORT_COUNT, *LPPMSG_GUILDVIEWPORT_COUNT;

typedef struct
{
	BYTE		NumberH;
	BYTE		NumberL;
	char		GuildName[MAX_GUILDNAMESTRING];
	BYTE		Mark[32];
} PMSG_GUILDVIEWPORT, *LPPMSG_GUILDVIEWPORT;


#ifdef MODIFY_GUILD_VIEWPORT_01_20040701

//----------------------------------------------------------------------------
// SC [0x65] 뷰포트내 길드 정보
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		Count;
} PMSG_SIMPLE_GUILDVIEWPORT_COUNT, *LPPMSG_SIMPLE_GUILDVIEWPORT_COUNT;

typedef struct
{

#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
	INT			GuildNumber;
#else
	BYTE		GNumberH;
	BYTE		GNumberL;
#endif
	
#ifdef ADD_NEW_GUILD_01_20040913
	BYTE		btGuildStatus;
#endif

#ifdef ADD_NEW_GUILD_02_20040922
	BYTE		btGuildType;
#endif

#ifdef ADD_NEW_UNION_01_20041006
	// 내 길드와의 관계
	// 0x00	: 관계 없음
	// 0x01 : Union
	// 0x03 : Union Master
	// 0x04 : Rival
	// 0x0C : Rival Union
	BYTE		btGuildRelationShip;	
#endif

	BYTE		NumberH;
	BYTE		NumberL;

#ifdef  UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217
	BYTE		btCastleOwner;
#endif	// UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217
} PMSG_SIMPLE_GUILDVIEWPORT, *LPPMSG_SIMPLE_GUILDVIEWPORT;

//----------------------------------------------------------------------------
// CS [0x66] 길드 Viewport 정보 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
	INT			GuildNumber;
#else
	BYTE		NumberH;
	BYTE		NumberL;
#endif

} PMSG_REQ_GUILDVIEWPORT, *LPPMSG_REQ_GUILDVIEWPORT;

//----------------------------------------------------------------------------
// SC [0x66] 길드 Viewport 정보 요청에 대한 응답.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
	INT			GuildNumber;
#else
	BYTE		NumberH;
	BYTE		NumberL;
#endif
	
#ifdef ADD_NEW_GUILD_02_20040922
	BYTE		btGuildType;
#endif

#ifdef ADD_NEW_UNION_01_20041006
	BYTE		UnionName[MAX_GUILDNAMESTRING];
#endif
	
	char		GuildName[MAX_GUILDNAMESTRING];
	BYTE		Mark[32];
} PMSG_ANS_GUILDVIEWPORT, *LPPMSG_ANS_GUILDVIEWPORT;

#endif // MODIFY_GUILD_VIEWPORT_01_20040701

#ifdef ADD_NEW_UNION_01_20041006
//----------------------------------------------------------------------------
// GC[0x67] Union ViewPort 정보 변화를 Notify 한다.
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btCount;
} PMSG_UNION_VIEWPORT_NOTIFY_COUNT, *LPPMSG_UNION_VIEWPORT_NOTIFY_COUNT;

typedef struct
{
	BYTE		btNumberH;				// 유저 인덱스
	BYTE		btNumberL;
	
	INT			iGuildNumber;			// 길드 번호

	BYTE		btGuildRelationShip;
	CHAR		szUnionName[MAX_GUILDNAMESTRING];
} PMSG_UNION_VIEWPORT_NOTIFY, *LPPMSG_UNION_VIEWPORT_NOTIFY;
#endif // ADD_NEW_UNION_01_20041006

//----------------------------------------------------------------------------
// SC [0x5B] 뷰포트내 길드에 속해있는 사람
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		Count;
} PMSG_GUILDVIEWPORT_USERCOUNT, *LPPMSG_GUILDVIEWPORT_USERCOUNT;

typedef struct
{
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		GNumberH;
	BYTE		GNumberL;
} PMSG_GUILDVIEWPORT_USER, *LPPMSG_GUILDVIEWPORT_USER;



//----------------------------------------------------------------------------
// SC [0x5C] 뷰포트에서 길드에 가입했다.. 즉시 반영한다. 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	BYTE	NumberH;
	BYTE	NumberL;
	char	GuildName[MAX_GUILDNAMESTRING];
	BYTE	Mark[32];
} PMSG_GUILDVIEWPORT_NOW, *LPPMSG_GUILDVIEWPORT_NOW;

//----------------------------------------------------------------------------
// SC [0x5D] 뷰포트에서 길드에서 빠져나왔다.. 즉시 반영한다. 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
	
} PMSG_GUILDDEL_VIEWPORT_NOW, *LPPMSG_GUILDDEL_VIEWPORT_NOW;


//----------------------------------------------------------------------------
// SC [0x60] 상대 길드에 전쟁을 선포한 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;
} PMSG_GUILDWARREQUEST_RESULT, *LPPMSG_GUILDWARREQUEST_RESULT;


//----------------------------------------------------------------------------
// SC [0x61] 다른 길드가 전쟁을 선포했다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	char		GuildName[MAX_GUILDNAMESTRING];
	BYTE		Type;	// 길드전 형태
} PMSG_GUILDWARSEND, *LPPMSG_GUILDWARSEND;

//----------------------------------------------------------------------------
// SC [0x61] 전쟁 선포 요청에 대한 결과 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;
} PMSG_GUILDWARSEND_RESULT, *LPPMSG_GUILDWARSEND_RESULT;

//----------------------------------------------------------------------------
// SC [0x62] 양쪽에 접속해 있는 모든 길드원에게 전쟁을 알린다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING];
	BYTE		Type;
	BYTE		TeamCode;
} PMSG_GUILDWAR_DECLARE, *LPPMSG_GUILDWAR_DECLARE;

//----------------------------------------------------------------------------
// SC [0x63] 양쪽에 접속해 있는 모든 길드원에게 전쟁의 종료를 알린다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	char		GuildName[MAX_GUILDNAMESTRING];
} PMSG_GUILDWAR_END, *LPPMSG_GUILDWAR_END;

//----------------------------------------------------------------------------
// SC [0x64] 길드 점수를 보내준다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Score1;	// 우리팀 점수
	BYTE		Score2; // 상대팀 점수
	BYTE		Type;
} PMSG_GUILDSCORE, *LPPMSG_GUILDSCORE;




//----------------------------------------------------------------------------
// 새로운 길드 시스템 프로토콜
//
// Lastest : 20040913, 20040922, 20041006, 20050418
//
//----------------------------------------------------------------------------

#ifdef ADD_NEW_GUILD_01_20040913
typedef struct {}__UNION_PROTOCOL_H___;
//----------------------------------------------------------------------------
// CG[0xE1] 직책을 임명 / 변경 / 해제 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btType;								// 임명(0x01) / 변경(0x02) / 해제(0x03)
	BYTE		btGuildStatus;						// 임명 되는 직책. 
	char		szTagetName[MAX_IDSTRING];			// 대상 유저 이름.

} PMSG_GUILD_ASSIGN_STATUS_REQ, *LPPMSG_GUILD_ASSIGN_STATUS_REQ;

//----------------------------------------------------------------------------
// CG[0xE1] 직책을 임명 / 변경 / 해제 결과
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btType;								// 임명(0x01) / 변경(0x02) / 해제(0x03)
	BYTE		btResult;							// 0x00 :실패  0x01 :성공
													// 0x10 :길드가 없음		GUILD_ANS_NOTEXIST_GUILD
													// 0x11 :권한 없음			GUILD_ANS_NOTEXIST_PERMISSION
													// 0x12 :임명할 직책이 없음 GUILD_ANS_NOTEXIST_EXTRA_STATUS

	char		szTagetName[MAX_IDSTRING];			// 대상 유저 이름.

} PMSG_GUILD_ASSIGN_STATUS_RESULT, *LPPMSG_GUILD_ASSIGN_STATUS_RESULT;

#endif


#ifdef ADD_NEW_GUILD_02_20040922

//----------------------------------------------------------------------------
// CG[0xE2] 길드 TYPE을 설정 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btGuildType;						// 설정 되는 TYPE. 
	
} PMSG_GUILD_ASSIGN_TYPE_REQ, *LPPMSG_GUILD_ASSIGN_TYPE_REQ;

//----------------------------------------------------------------------------
// GC[0xE2] 길드 TYPE을 설정 결과
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btGuildType;						// 설정 되는 직책. 
	BYTE		btResult;							// 0x00 :실패  0x01 :성공
													// 0x10 :길드가 없음		GUILD_ANS_NOTEXIST_GUILD
													// 0x11 :권한 없음			GUILD_ANS_NOTEXIST_PERMISSION
													// 0x12 :설정할 TYPE이 없음 GUILD_ANS_NOTEXIST_EXTRA_TYPE

} PMSG_GUILD_ASSIGN_TYPE_RESULT, *LPPMSG_GUILD_ASSIGN_TYPE_RESULT;

#endif


#ifdef ADD_NEW_UNION_01_20041006
//----------------------------------------------------------------------------
// CG[0xE5], DC[0xE5] Union, Rival 관계 맺기/끊기 
// ( [CD]관계맺기/끊기 요청 받음, [DC]상대에게 관계 요청 )
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btRelationShipType;					// RelationShip의 타입
													// 0x01	:Union										
													// 0x02 :Rival

	BYTE		btRequestType;						// 관계 맺기인가? 끊기 인가?
													// 0x01 :Join
													// 0x02 :BreakOff
	
	BYTE		btTargetUserIndexH;
	BYTE		btTargetUserIndexL;
	
} PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ, *LPPMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ;

//----------------------------------------------------------------------------
// GC[0xE6], CD[0xE6] Union, Rival 관계 맺기/끊기 결과
// ( [CD]관계 맺기/끊기 응답 요청 받음, [DC]상대에게 관계 맺기/끊기 응답 요청 )
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btRelationShipType;					// RelationShip의 타입
													// 0x01	:Union										
													// 0x02 :Rival
	
	BYTE		btRequestType;						// 관계 맺기인가? 끊기 인가?
													// 0x01 :Join
													// 0x02 :BreakOff

	BYTE		btResult;							// 0x00 :실패  0x01 :성공
													// 0x10 :길드가 없음		GUILD_ANS_NOTEXIST_GUILD
													// 0x11 :권한 없음			GUILD_ANS_NOTEXIST_PERMISSION

	BYTE		btTargetUserIndexH;
	BYTE		btTargetUserIndexL;
											
} PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS, *LPPMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS;

//----------------------------------------------------------------------------
// CG[0xE9] Union List
// 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

} PMSG_UNIONLIST_REQ, *LPPMSG_UNIONLIST_REQ;

typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btCount;
	BYTE		btResult;
	BYTE		btRivalMemberNum;
	BYTE		btUnionMemberNum;

} PMSG_UNIONLIST_COUNT, *LPPMSG_UNIONLIST_COUNT;

typedef struct
{
	BYTE		btMemberNum;
	BYTE		Mark[32];
	char		szGuildName[MAX_GUILDNAMESTRING];

} PMSG_UNIONLIST, *LPPMSG_UNIONLIST;


//----------------------------------------------------------------------------
// CG[0xEA][0x01] 길드정보변경 : 길드 마크 변경 요청
// 
//----------------------------------------------------------------------------

typedef struct
{
	PBMSG_HEAD	h;
	BYTE		btSubCode;
	BYTE		btUserIndexH;
	BYTE		btUserIndexL;
	BYTE		Mark[32];
	
} PMSG_GUILD_MODIFY_MARK, *LPPMSG_GUILD_MODIFY_MARK;

//----------------------------------------------------------------------------
// CG[0xEA][0x01] 길드정보변경 : 길드 마크 변경 결과
// 
//----------------------------------------------------------------------------

typedef struct
{
	PBMSG_HEAD	h;
	BYTE		btSubCode;
	BYTE		btUserIndexH;
	BYTE		btUserIndexL;
	BYTE		btResult;

} PMSG_GUILD_MODIFY_MARK_RESULT, *LPPMSG_GUILD_MODIFY_MARK_RESULT;

#endif // ADD_NEW_UNION_01_20041006

#ifdef ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418
//----------------------------------------------------------------------------
// CG[0xEB][0xXX] 연합길드 마스터의 Ability 구현
// 
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// CG[0xEB][0x01] 연합길드 마스터가 연합 멤버를 강제 탈퇴 시킴 : 요청
// 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	char		szTargetGuildName[MAX_GUILDNAMESTRING];
	
} PMSG_KICKOUT_UNIONMEMBER_REQ, *LPPMSG_KICKOUT_UNIONMEMBER_REQ;


//----------------------------------------------------------------------------
// CG[0xEB][0x01] 연합길드 마스터가 연합 멤버를 강제 탈퇴 시킴 : 결과
// 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	BYTE		btResult;
	BYTE		btRequestType;
	BYTE		btRelationShipType;

} PMSG_KICKOUT_UNIONMEMBER_RESULT, *LPPMSG_KICKOUT_UNIONMEMBER_RESULT;

#endif // ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418


//----------------------------------------------------------------------------
// SC [0x71] 핑(PING).
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
} PMSG_PING, *LPPMSG_PING;

//----------------------------------------------------------------------------
// CS [0x71] 핑(PING).
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
} PMSG_PING_RESULT, *LPPMSG_PING_RESULT;

//***************************************************************************//
//***************************************************************************//
// [0x81] 돈을 입/출금 시킨다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Type;
	int			Money;
} PMSG_WAREHOUSEMONEYINOUT,*LPPMSG_WAREHOUSEMONEYINOUT;

// [0x81] 돈을 업데이트 시킨다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	int			wMoney;
	int			iMoney;

} PMSG_MONEY, *LPPMSG_MONEY;

// [0x83] 창고비밀번호를 서버에 보낸다.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Type;
	short		Pass;
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
	char		LastJoominNumber[MAX_JOOMINNUMBER_LENGTH];
#else
	char		LastJoominNumber[10];
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
} PMSG_WAREHOUSEPASSSEND,*LPPMSG_WAREHOUSEPASSSEND;

// [0x83] 창고 잠금 상태
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		State;
} PMSG_WAREHOUSESTATE,*LPPMSG_WAREHOUSESTATE;


//***************************************************************************//

#ifdef 	CHAOS_MIX_UPGRADE
//***************************************************************************//
// [0x86] 카오스 조합 요청
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Type;			//1: // 일반 조합
//								2: // 악마의 눈 + 열쇠
//								3: // +10 만들기
//								4: // +11 만들기
//								5: // 페가시아 만들기
//								6: // 열매 만들기
//								7: // 날개 만들기
//								8: // 투명망토 만들기
//								11:// 일반 날개 조합

#ifdef ITEM_12_13_20040401
//								12: // 세트 ㅠ_ㅠ
//								22: // +12 만들기
//								23: // +13 만들기
#endif
	
#ifdef MODIFY_CHAOSMIX_PROTOCOL_EXTEND_20080526
	BYTE		SubType;		// 카오스 조합 확장을 위한 서브타입 추가
#endif // MODIFY_CHAOSMIX_PROTOCOL_EXTEND_20080526

} PMSG_CHAOSMIX, *LPPMSG_CHAOSMIX;
#endif

//***************************************************************************//
// [0x86] 카오스 조합 결과 전송
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;
	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_CHAOSMIXRESULT, *LPPMSG_CHAOSMIXRESULT;


// [0x90] 광장으로 이동을 요청한다
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		SquareNumber;				// 광장 번호
	BYTE		InvitationItemPos;			// 초대권의 위치
} PMSG_REQ_MOVEDEVILSQUARE, *LPPMSG_REQ_MOVEDEVILSQUARE;

// [0x90] 광장으로 이동요청 결과
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;				// 광장 번호	
} PMSG_RESULT_MOVEDEVILSQUARE, *LPPMSG_RESULT_MOVEDEVILSQUARE;

// [0x91] 광장오픈시간 요청
typedef struct
{
	PBMSG_HEAD	h;
#ifdef FOR_BLOODCASTLE
	BYTE		hEventType;			// 0 : 데빌스퀘어 / 1 : 블러드 캐슬
	BYTE		btItemLevel;		// 퀘스트 아이템 레벨 -> 1 ~ 7 다리번호 (블러드 캐슬만 사용)
#endif
} PMSG_REQ_DEVILSQUARE_REMAINTIME, *LPPMSG_REQ_DEVILSQUARE_REMAINTIME;

// [0x91] 오픈시간 요청 결과
typedef struct
{
	PBMSG_HEAD	h;
#ifdef FOR_BLOODCASTLE
	BYTE		hEventType;			// 1 : 데빌스퀘어 / 2 : 블러드 캐슬	/ 4 : 카오스 캐슬
#endif
	BYTE		RemainTime;			// 남은 분  (카오스 캐슬에서는 상위바이트)

#ifdef CHAOSCASTLE_SYSTEM_20040408	// [0x91] 패킷 수정
	BYTE		EnteredUser;		// 현재 카오스캐슬에 입장한 사용자의 수
#ifdef CHAOSCASTLE_TIME_BUGFIX_20040508
	BYTE		RemainTime_LOW;		// 남은 분  (카오스 캐슬에서는 하위바이트)
#endif
#endif	
} PMSG_RESULT_DEVILSQUARE_REMAINTIME, *LPPMSG_RESULT_DEVILSQUARE_REMAINTIME;

// [0x92] 광장 카운트 알리기 
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Type;				// 시작->모두(0), 끝->광장만(1)
#ifdef CHAOSCASTLE_SYSTEM_20040408
									/*
									0	- 데블 스퀘어 : 시작 30초전
									1	- 데블 스퀘어 : 종료 30초전
									3	- 블러드 캐슬 : 입장 30초전
									4	- 블러드 캐슬 : 경기시작 30초전
									5	- 블러드 캐슬 : 경기종료 30초전
									6	- 블러드 캐슬 : 퇴장 30초전
									10	- 카오스 캐슬 : 입장 30초전
									11	- 카오스 캐슬 : 경기시작 30초전
									12	- 카오스 캐슬 : 경기종료 30초전
									13	- 카오스 캐슬 : 퇴장 30초전
									*/
#endif
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
									/*
									15,	-	환영사원 : 준비 30초전의 메시지 타입
									16,	-	환영사원 : 경기시작 30초전의 메시지 타입
									*/
#endif
} PMSG_SET_DEVILSQUARE, *LPPMSG_SET_DEVILSQUARE;


// 광장 이벤트 결과 Record
typedef struct
{
	char	Name[10];
	int		TotalScore;	// 토탈 누적점수 
	int		BonusExp;	// 보상 경험치
	int		BonusZen;	// 보상 포상금
}DevilSquareScoreInfo, *LPDevilSquareScoreInfo;

#pragma pack(push,1)
// [0x93] 광장 이벤트가 끝난후, 결과 전송
typedef struct
{
	PBMSG_HEAD				h;
	BYTE					MyRank;
	BYTE					Count;
	DevilSquareScoreInfo	Score[11];
}PMSG_DEVILSQUARERESULT, *LPPMSG_DEVILSQUARERESULT;
#pragma pack(pop)


// SC [0x94] 이벤트칩 정보 전송
typedef struct
{
	PBMSG_HEAD	h;
#ifdef  FOR_BLOODCASTLE
	BYTE		Type;		// 0 : 레나 / 1 : 스톤 / 2 : 복권 / 3 : 스톤환전 / 4 : 우정의돌 / 5 : 우정의돌 환전
#endif

#ifdef EVENTCHIP_DATASIZE_EXPAND_20090217
	INT			ChipCount;
#else
	unsigned short	ChipCount;
#endif //EVENTCHIP_DATASIZE_EXPAND_20090217

	short			MutoNum[3];
}PMSG_EVENTCHIPINFO, *LPPMSG_EVENTCHIPINFO;

// CS [0x95] 이벤트칩 등록 요청
typedef struct
{
	PBMSG_HEAD	h;
#ifdef FOR_BLOODCASTLE
	BYTE		Type;		// 0 : 레나 / 1 : 스톤 / 4 : 우정의돌
#endif
	BYTE		ChipPos;	
}PMSG_REGEVENTCHIP,*LPPMSG_REGEVENTCHIP;

// SC [0x95] 이벤트칩 등록요청 결과
typedef struct
{
	PBMSG_HEAD	h;
#ifdef  FOR_BLOODCASTLE
	BYTE		Type;		// 0 : 레나 / 1 : 스톤 / 4 : 우정의돌
#endif

#ifdef EVENTCHIP_DATASIZE_EXPAND_20090217
	INT			ChipCount;
#else
	short		ChipCount;	
#endif //EVENTCHIP_DATASIZE_EXPAND_20090217

}PMSG_REGEVENTCHIP_RESULT, *LPPMSG_REGEVENTCHIP_RESULT;


// CS [0x96] 뮤토 번호 등록 요청
typedef struct
{
	PBMSG_HEAD	h;	
}PMSG_GETMUTONUMBER, *LPPMSG_GETMUTONUMBER;

// SC [0x96] 뮤토 번호 등록 결과
typedef struct
{
	PBMSG_HEAD	h;	
	short		MutoNum[3];
}PMSG_GETMUTONUMBER_RESULT, *LPPMSG_GETMUTONUMBER_RESULT;


// CS [0x98] 레나, 스톤 환전요청
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		btType;		// 0:레나 / 1:스톤
}PMSG_EXCHANGE_EVENTCHIP, *LPPMSG_EXCHANGE_EVENTCHIP;


// CS [0x99] 서버이동 신청
typedef struct
{
	PBMSG_HEAD	h;	
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
	char		LastJoominNumber[MAX_JOOMINNUMBER_LENGTH];
#else
	char		LastJoominNumber[10];
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
}PMSG_REQ_MOVE_OTHERSERVER, *LPPMSG_REQ_MOVE_OTHERSERVER;


#ifdef NEW_SKILL_FORSKYLAND
// SC [0xA0] 퀘스트 정보전송(전체)
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Count;						// 퀘스트리스트수
	BYTE		State[MAX_DBQUEST];			// 퀘스트리스트 버퍼
}PMSG_SEND_QEUSTINFO, *LPPMSG_SEND_QEUSTINFO;

// CS [0xA2] 특정 퀘스트 상태정보 설정 요구
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		QuestIndex;		// 퀘스트 번호
	BYTE		State;			// 상태
}PMSG_SETQUEST, *LPPMSG_SETQUEST;

// SC [0xA2] 특정 퀘스트 상태정보 설정 요구에 대한 응답
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		QuestIndex;		// 퀘스트 번호
	BYTE		Result;			// 결과
	BYTE		State;			// 상태
}PMSG_SETQUEST_RESULT, *LPPMSG_SETQUEST_RESULT;


// SC [0xA3] 특정 퀘스트 클리어 했을때 보상 정보...
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		Type;			// 보상 타입(200 : 스탯, 201 : 1차전직 ... )
	BYTE		Count;			// 수...
}PMSG_SETQUEST_PRIZE, *LPPMSG_SETQUEST_PRIZE;
#endif


#ifdef DARKLORD_WORK

// 0xA7	Client ==> GS	다크스피릿(펫 아이템 커맨드 요청)
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// 펫아이템 타입 (0:다크스피릿)
	BYTE		Command;		// 0 : 일반(대기) 1 : 랜덤공격  2: 주인하고 같이 공격 3 : 타겟공격

	BYTE		NumberH;		// 대상의 사용자 인덱스 (상위 바이트)
	BYTE		NumberL;		// 대상의 사용자 인덱스 (하위 바이트)
}PMSG_REQUEST_PET_ITEM_COMMAND, *LPPMSG_REQUEST_PET_ITEM_COMMAND;

// 0xA7	GS ==> Client	다크스피릿(펫 아이템 커맨드 요청 결과)
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// 펫아이템 타입 (0:다크스피릿)
	BYTE		Command;		// 0 : 일반(대기) 1 : 랜덤공격  2: 주인하고 같이 공격 3 : 타겟공격

	BYTE		NumberH;		// 대상의 사용자 인덱스 (상위 바이트)
	BYTE		NumberL;		// 대상의 사용자 인덱스 (하위 바이트)
}PMSG_SEND_PET_ITEM_COMMAND, *LPPMSG_SEND_PET_ITEM_COMMAND;

// 0xA8	GS ==> Client	다크스피릿(펫 아이템 공격)
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// 펫아이템 타입 (0:다크스피릿)	
	BYTE		SkillType;		// 공격 타입 (나중을 위해서)

	BYTE		NumberH;		// 주인의 사용자 인덱스 (상위 바이트)
	BYTE		NumberL;		// 주인의 사용자 인덱스 (하위 바이트)

	BYTE		TargetNumberH;		// 대상(공격당하는)의 사용자 인덱스 (상위 바이트)
	BYTE		TargetNumberL;		// 대상(공격당하는)의 사용자 인덱스 (하위 바이트)
}PMSG_PET_ITEM_ATTACK_COMMAND, *LPPMSG_PET_ITEM_ATTACK_COMMAND;


// 0xA9	Client ==> GS	펫 아이템 정보 요청
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// 펫아이템 타입 (0:다크스피릿)	
	BYTE		InvenType;		// 인벤종류 (0:인벤토리 1:창고 2:거래창 .... )
	BYTE		nPos;			// 위치
}PMSG_REQUEST_PET_ITEMINFO, *LPPMSG_REQUEST_PET_ITEMINFO;

// 0xA9	GS ==> Client	펫 아이템 정보 전송
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// 펫아이템 타입 (0:다크스피릿)	
	BYTE		InvenType;		// 인벤종류 (0:인벤토리 1:창고 2:거래창 3:상대거래창,4:카오스 ...., 5:개인상점 254:레벨업 255:레벨다운  )
	BYTE		nPos;			// 위치

	BYTE		Level;			// 아이템 레벨
	int			Exp;			// 경험치
	
#ifdef  MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
	BYTE		Life;			// 생명력
#endif // MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626

}PMSG_SEND_PET_ITEMINFO, *LPPMSG_SEND_PET_ITEMINFO;

#endif

#ifndef ADD_NEWPVP_PKFIELD
#ifdef DUEL_SYSTEM_20031028

// SC [0xAA] 결투시작 요청
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;		// 결투 중이었던 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;		// 결투 중이었던 상대방 사용자의 인덱스 (하위 바이트)
	CHAR		szName[10];		// 케릭이름
}PMSG_REQ_START_DUEL, *LPPMSG_REQ_START_DUEL;


// SC [0xAA] 결투시작 요청에 대한 응답
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		bDuelStart;		// 결투시작 성공여부 (0:실패 / 1:성공)
	BYTE		NumberH;		// 결투 중이었던 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;		// 결투 중이었던 상대방 사용자의 인덱스 (하위 바이트)
	CHAR		szName[10];		// 케릭이름
}PMSG_ANS_START_DUEL, *LPPMSG_ANS_START_DUEL;


// CS [0xAB] 결투종료 요청
typedef struct
{
	PBMSG_HEAD	h;
}PMSG_REQ_END_DUEL, *LPPMSG_REQ_END_DUEL;


// SC [0xAB] 결투종료 요청에 대한 응답
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		NumberH;		// 결투 중이었던 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;		// 결투 중이었던 상대방 사용자의 인덱스 (하위 바이트)
	CHAR		szName[10];		// 케릭이름
}PMSG_ANS_END_DUEL, *LPPMSG_ANS_END_DUEL;


// SC [0xAC] 결투를 할지에 대한 여부 확인
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;		// 결투를 요청한 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;		// 결투를 요청한 상대방 사용자의 인덱스 (하위 바이트)
	CHAR		szName[10];		// 케릭이름
}PMSG_REQ_DUEL_OK, *LPPMSG_REQ_DUEL_OK;


// CS [0xAC] 결투시작 요청에 대한 응답
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		bDuelOK;		// 결투확인 요청에 대한 응답 (0:아니요 / 1:예)
	BYTE		NumberH;		// 결투 중이었던 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;		// 결투 중이었던 상대방 사용자의 인덱스 (하위 바이트)
	CHAR		szName[10];		// 케릭이름
}PMSG_ANS_DUEL_OK, *LPPMSG_ANS_DUEL_OK;


// SC [0xAD] 결투 점수 보내줌
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH1;		// 결투 중이었던 상대방 사용자의 인덱스 (상위 바이트) - 1번
	BYTE		NumberL1;		// 결투 중이었던 상대방 사용자의 인덱스 (하위 바이트) - 1번
	BYTE		NumberH2;		// 결투 중이었던 상대방 사용자의 인덱스 (상위 바이트) - 2번
	BYTE		NumberL2;		// 결투 중이었던 상대방 사용자의 인덱스 (하위 바이트) - 2번
	BYTE		btDuelScore1;	// 결투점수 - 1번
	BYTE		btDuelScore2;	// 결투점수 - 1번
}PMSG_ANS_DUEL_SCORE, *LPPMSG_ANS_DUEL_SCORE;


#endif
#endif // ADD_NEWPVP_PKFIELD

#ifdef CHAOSCASTLE_SYSTEM_20040408

// [0xAF][0x01] 카오스 캐슬 안으로 이동을 요청한다.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		iChaosCastleNumber;		// 다리 번호
	BYTE		iItemPos;				// 근위병의 갑옷세트의 위치
} PMSG_REQ_MOVECHAOSCASTLE, *LPPMSG_REQ_MOVECHAOSCASTLE;


// [0xAF][0x01] 카오스 캐슬 안으로 이동요청 결과
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		Result;					/* 요청결과 (	0 : 성공
														1 : 알맞은 근위병의 갑옷세트 없음
														2 : 들어갈수 있는 시간이 아님
														3 : 근위병의 갑옷세트는 맞으나 자신의 레벨보다 낮은 곳에 갈려고 할 때
														4 : 근위병의 갑옷세트는 맞으나 자신의 레벨보다 높은 곳에 갈려고 할 때
														5 : 인원제한이 넘었음
														6 : 1일 입장제한 횟수를 넘었음
														7 : 입장료가 모자란다.
														// ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
														8 : 카오유저는 입장할 수 없다.
														)
										*/
} PMSG_RESULT_MOVECHAOSCASTLE, *LPPMSG_RESULT_MOVECHAOSCASTLE;


// [0xAF][0x02] 사용자가 카오스 캐슬 안에 이상 지역에 장시간 있음을 알린다.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btPX;		// 위치 X
	BYTE		btPY;		// 위치 Y
} PMSG_REQ_REPOSUSER_IN_CC, *LPPMSG_REQ_REPOSUSER_IN_CC;


#endif


#ifdef FOR_BLOODCASTLE

// [0x9A] 블러드 캐슬 다리로 이동을 요청한다
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		iBridgeNumber;		// 다리 번호
	BYTE		iItemPos;			// 투명망토의 위치
} PMSG_REQ_MOVEBLOODCASTLE, *LPPMSG_REQ_MOVEBLOODCASTLE;


// [0x9A] 블러드 캐슬 다리로 이동요청 결과
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;				/* 요청결과 (	0 : 성공
													1 : 알맞은 투명망토 없음
													2 : 들어갈수 있는 시간이 아님
													3 : 투명망토는 맞으나 자신의 레벨보다 낮은 곳에 갈려고 할 때
													4 : 투명망토는 맞으나 자신의 레벨보다 높은 곳에 갈려고 할 때
													5 : 인원제한이 넘었음
													6 : 1일 입장제한 횟수를 넘었음
													// ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411			
													7 : PK 레벨 1이상 사용자는 입장할 수 없음
									*/
} PMSG_RESULT_MOVEBLOODCASTLE, *LPPMSG_RESULT_MOVEBLOODCASTLE;


// [0x9B] 블러드 캐슬 경기진행 상태알림 (서버 -> 사용자)
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		btPlayState;		// 현재 경기진행 상태 (0:경기시작됨, 1:경기진행중(일반몹), 2:경기종료됨, 3:경기진행중(보스몹))
	WORD		wRemainSec;			// 게임시작 후 종료까지 남은 초
	WORD		wMaxKillMonster;	// 잡아야 할 몬스터 할당 수
	WORD		wCurKillMonster;	// 현재까지 잡은 몬스터 수
	WORD		wUserHaveWeapon;	// 절대무기를 가지고 있는 사용자 인덱스
	BYTE		btWeaponNum;		// 절대무기의 종류 (0:무기없음 / 1:지팡이 / 2:검 / 3:석궁)
} PMSG_STATEBLOODCASTLE, *LPPMSG_STATEBLOODCASTLE;


//----------------------------------------------------------------------------
// GC [0x9C] 0x16 프로토콜의 확장형 -> 경험치를 최대 INT범위까지 보낼 수 있다. (블러드캐슬 보상경험치 부여시 사용)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
	WORD	ExpH;
	WORD	ExpL;
	BYTE	DamageH;
	BYTE	DamageL;
} PMSG_KILLPLAYER_EXT, *LPPMSG_KILLPLAYER_EXT;


#endif


#if defined ( FOR_KOREA ) || defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )

//----------------------------------------------------------------------------
// CG [0x9D] 상용화 2주년 기념 복권 이벤트 시 복권 당첨여부 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	CHAR		SERIAL1[5];			// 시리얼1
	CHAR		SERIAL2[5];			// 시리얼2
	CHAR		SERIAL3[5];			// 시리얼3
} PMSG_REQ_2ANV_LOTTO_EVENT, *LPPMSG_REQ_2ANV_LOTTO_EVENT;


//----------------------------------------------------------------------------
// GC [0x9D] 상용화 2주년 기념 복권 이벤트 시 복권 당첨여부를 알려줌
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	btIsRegistered;		// 0:이면 본인이 등록했음 (성공) / 1:다른사람이 등록했음 (아래 필드에 정보없음) / 2:등록제한횟수참 / 3:존재하지 않는 시리얼 / 4:알수없는에러
	CHAR	szGIFT_NAME[64];	// 경품이름
} PMSG_ANS_2ANV_LOTTO_EVENT, *LPPMSG_ANS_2ANV_LOTTO_EVENT;


#endif // FOR_KOREA || EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO


#ifdef MONSTER_HERD_SYSTEM_20031120


//----------------------------------------------------------------------------
// GC [0x9E] 클라이언트가 특별한 이펙트를 시작하도록 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	WORD wEffectNum;	// 클라이언트에서 보여줄 이펙트 번호 (사운드 혹은 그래픽)
} PMSG_ANS_CL_EFFECT, *LPPMSG_ANS_CL_EFFECT;


#endif


#ifdef BLOODCASTLE_EVENT_3RD_20040401

//----------------------------------------------------------------------------
// CG [0x9F] 사용자의 남은 이벤트 (데빌스퀘어, 블러드캐슬) 참여횟수를 요청
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD	h;

	BYTE btEventType;		// 이벤트 종류 (1-데빌스퀘어 / 2-블러드캐슬 ...)
} PMSG_REQ_CL_ENTERCOUNT, *LPPMSG_REQ_CL_ENTERCOUNT;


//----------------------------------------------------------------------------
// GC [0x9F] 사용자의 남은 이벤트 (데빌스퀘어, 블러드캐슬) 참여횟수를 보내줌
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD	h;

	BYTE btEventType;		// 이벤트 종류 (1-데빌스퀘어 / 2-블러드캐슬 ...)
	BYTE btLeftEnterCount;	// 남은 참여가능 횟수
} PMSG_ANS_CL_ENTERCOUNT, *LPPMSG_ANS_CL_ENTERCOUNT;


#endif

#ifdef __FRIEND_WORK__
//----------------------------------------------------------------------------
// 친구 기능 프로토콜 
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// CG [0xC0] 친구 리스트를 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
} PMSG_FRIEND_LIST_REQ, *LPPMSG_FRIEND_LIST_REQ;

//----------------------------------------------------------------------------
// GC [0xC0] 친구 리스트 요청에 대한 결과 (카운트 및 정보)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;	
	BYTE		MemoCount;
	BYTE		MailTotal;
	BYTE		Count;
} PMSG_FRIEND_LIST_COUNT, *LPPMSG_FRIEND_LIST_COUNT;

//----------------------------------------------------------------------------
// GC [0xC0] 친구 리스트 요청에 대한 결과 (리스트)
//----------------------------------------------------------------------------
typedef struct
{
	char		Name[MAX_IDSTRING];
	BYTE		Server;
} PMSG_FRIEND_LIST, *LPPMSG_FRIEND_LIST;


//----------------------------------------------------------------------------
// CG [0xC1] 친구 등록을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_ADD_REQ, *LPPMSG_FRIEND_ADD_REQ;

//----------------------------------------------------------------------------
// GC [0xC1] 친구 등록 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Result;
	char		Name[MAX_IDSTRING];
	BYTE		State;
} PMSG_FRIEND_ADD_RESULT, *LPPMSG_FRIEND_ADD_RESULT;

//----------------------------------------------------------------------------
// 다른 캐릭터가 친구 등록을 요청 했다면,
// 화면엔 "XX님으로 부터 친구 신청이 들어왔습니다." Dialog
// GC [0xC2] 친구 등록 확인 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_ADD_SIN_REQ, *LPPMSG_FRIEND_ADD_SIN_REQ;

//----------------------------------------------------------------------------
// CG [0xC2] 친구 등록 확인 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_ADD_SIN_RESULT, *LPPMSG_FRIEND_ADD_SIN_RESULT;

//----------------------------------------------------------------------------
// CG [0xC3] 친구 삭제 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];	
} PMSG_FRIEND_DEL_REQ, *LPPMSG_FRIEND_DEL_REQ;

//----------------------------------------------------------------------------
// GC [0xC3] 친구 삭제 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_DEL_RESULT, *LPPMSG_FRIEND_DEL_RESULT;


//----------------------------------------------------------------------------
// CG [0xC4] 친구 상태 변화
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		ChatVeto;
} PMSG_FRIEND_STATE_C, *LPPMSG_FRIEND_STATE_C;


//----------------------------------------------------------------------------
// GC [0xC4] 친구 상태 변화
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];	
	BYTE		State;
} PMSG_FRIEND_STATE, *LPPMSG_FRIEND_STATE;


//----------------------------------------------------------------------------
// CG [0xC5] 쪽지 보내기
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	DWORD		WindowGuid;
	char		Name[MAX_IDSTRING];	
	char		Subject[MAX_MEMO_SUBJECT];
	BYTE		Dir;
	BYTE		Action;
	short		MemoSize;
	char		Memo[MAX_MEMO];
} PMSG_FRIEND_MEMO, *LPPMSG_FRIEND_MEMO;

//----------------------------------------------------------------------------
// GC [0xC5] 쪽지 보낸 결과 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	DWORD		WindowGuid;
} PMSG_FRIEND_MEMO_RESULT, *LPPMSG_FRIEND_MEMO_RESULT;

//----------------------------------------------------------------------------
// GC [0xC6] 쪽지 도착
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		Number;
	char		Name[MAX_IDSTRING];	
	char		Date[MAX_DATE];
	char		Subject[MAX_MEMO_SUBJECT];
	BYTE		read;
} PMSG_FRIEND_MEMO_LIST, *LPPMSG_FRIEND_MEMO_LIST;

//----------------------------------------------------------------------------
// GC [0xC7] 쪽지 보기 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		MemoIndex;
} PMSG_FRIEND_READ_MEMO_REQ, *LPPMSG_FRIEND_READ_MEMO_REQ;

//----------------------------------------------------------------------------
// GC [0xC7] 쪽지 내용
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;	
	WORD		MemoIndex;
	short		MemoSize;
	
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE		Photo[MAX_PREVIEWCHARSET+9];
#else
#ifdef DARKLORD_WORK
	BYTE		Photo[MAX_PREVIEWCHARSET+4];
#else
	BYTE		Photo[MAX_PREVIEWCHARSET+3];
#endif
#endif
		
	BYTE		Dir;
	BYTE		Action;
	char		Memo[MAX_MEMO];
} PMSG_FRIEND_READ_MEMO, *LPPMSG_FRIEND_READ_MEMO;

//----------------------------------------------------------------------------
// GC [0xC8] 쪽지 삭제 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD MemoIndex;
} PMSG_FRIEND_MEMO_DEL_REQ, *LPPMSG_FRIEND_MEMO_DEL_REQ;

//----------------------------------------------------------------------------
// GC [0xC7] 쪽지 삭제 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Result;
	WORD		MemoIndex;	
} PMSG_FRIEND_MEMO_DEL_RESULT, *LPPMSG_FRIEND_MEMO_DEL_RESULT;

//----------------------------------------------------------------------------
// GC [0xCA] 대화방 개설 요청 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_ROOMCREATE_REQ, *LPPMSG_FRIEND_ROOMCREATE_REQ;


//----------------------------------------------------------------------------
// GC [0xCA] 대화방 개설 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		ServerIp[15];
	WORD		RoomNumber;
	DWORD		Ticket;
	BYTE		Type;
	char		Name[MAX_IDSTRING];	
	BYTE		Result;
} PMSG_FRIEND_ROOMCREATE_RESULT, *LPPMSG_FRIEND_ROOMCREATE_RESULT;


// 0xCB : 대화초대 요청(친구를 초대한다)
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
	WORD		RoomNumber;
	DWORD		WindowGuid;
} PMSG_ROOM_INVITATION, *LPPMSG_ROOM_INVITATION;

// 0xCB : 초대 결과 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	DWORD		WindowGuid;
} PMSG_ROOM_INVITATION_RESULT, *LPPMSG_ROOM_INVITATION_RESULT;

//----------------------------------------------------------------------------
#endif

//***************************************************************************//
//***************************************************************************//
#ifdef PW_KUZIMO
#pragma pack(push,1)
#endif
//----------------------------------------------------------------------------
// [0xF1][0x01] 계정 아이디와 비번을 입력후 접속을 요청한다. 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		subcode;
	char		Id[MAX_IDSTRING];
#ifdef PW_KUZIMO
	char		Pass[MAX_PASSWORD_LENGTH];
#else
	char		Pass[MAX_IDSTRING];
#endif
	DWORD		TickCount;
	BYTE		CliVersion[5];
	BYTE		CliSerial[16];
} PMSG_IDPASS, *LPPMSG_IDPASS;
#ifdef PW_KUZIMO
#pragma pack(pop)
#endif

//----------------------------------------------------------------------------
// CG [0xB0] 공간이동 사용(대상을 순간이동시킨다) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;	
	BYTE		MapX;
	BYTE		MapY;
} PMSG_TARGET_TELEPORT, *LPPMSG_TARGET_TELEPORT;


// GC [0xBA] 스킬 카운트 전송
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		Type;		// 스킬 번호
	BYTE		Count;		// 단계 카운트
} PMSG_EX_SKILL_COUNT, *LPPMSG_EX_SKILL_COUNT;

//----------------------------------------------------------------------------
// CG [0x72] PaketChecksum
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		funcindex;
	DWORD		CheckSum;
} PMSG_PACKETCHECKSUM, *LPPMSG_PACKETCHECKSUM;

//----------------------------------------------------------------------------
// CG [0x73] NProtect GameGaurd Checksum
//----------------------------------------------------------------------------
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
typedef struct
{
	PBMSG_HEAD	h;	
	GG_AUTH_DATA25 m_ggadCheckSum;
} PMSG_NPROTECTGGCHECKSUM, *LPPMSG_NPROTECTGGCHECKSUM;

#else

	#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
	typedef struct
	{
		PBMSG_HEAD	h;	
		DWORD		CheckSum;
	} PMSG_NPROTECTGGCHECKSUM, *LPPMSG_NPROTECTGGCHECKSUM;

	#elif defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	typedef struct
	{
		PBMSG_HEAD	h;	
		GG_AUTH_DATA m_ggadCheckSum;
	} PMSG_NPROTECTGGCHECKSUM, *LPPMSG_NPROTECTGGCHECKSUM;

	#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

#ifdef MAP_SERVER_WORK_20041030		// 서버 맵분할 관련 패킷 - 뮤 클라이언트 <-> 게임서버

//----------------------------------------------------------------------------
// GC [0xB1][0x00] 클라이언트에게 맵서버를 이동해야 함을 알림 (0xC3)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;	
	CHAR		szMapSvrIpAddress[16];			// 이동할 맵서버 IP
	WORD		wMapSvrPort;					// 이동할 맵서버 포트
	WORD		wMapSvrCode;					// 이동할 맵서버 코드
	INT			iJoinAuthCode1;					// 인증 AUTH 코드 - 1
	INT			iJoinAuthCode2;					// 인증 AUTH 코드 - 2
	INT			iJoinAuthCode3;					// 인증 AUTH 코드 - 3
	INT			iJoinAuthCode4;					// 인증 AUTH 코드 - 4
} PMSG_ANS_MAPSERVERCHANGE, *LPPMSG_ANS_MAPSERVERCHANGE;


//----------------------------------------------------------------------------
// CG [0xB1][0x01] 맵서버 이동 시 중간인증 생략, 접속요청 (0xC3)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	CHAR		szAccountID[MAX_IDSTRING+2];	// 인증받을 ID
	CHAR		szCharName[MAX_IDSTRING+2];		// 인증받을 케릭명
	INT			iJoinAuthCode1;					// 인증 AUTH 코드 - 1
	INT			iJoinAuthCode2;					// 인증 AUTH 코드 - 2
	INT			iJoinAuthCode3;					// 인증 AUTH 코드 - 3
	INT			iJoinAuthCode4;					// 인증 AUTH 코드 - 4
	// 기존 인증확인 부가 정보들 [0xF1][0x01]
	INT			iTickCount;
	BYTE		btCliVersion[5];				// 클라이언트 버전
	BYTE		btCliSerial[16];				// 클라이언트 시리얼
} PMSG_REQ_MAPSERVERAUTH, *LPPMSG_REQ_MAPSERVERAUTH;


//----------------------------------------------------------------------------
// GC [0xB1][0x01] 맵서버 이동 시 중간인증 생략, 접속요청 결과 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;	
	BYTE		iResult;
} PMSG_ANS_MAPSERVERAUTH, * LPPMSG_ANS_MAPSERVERAUTH;

#endif


#ifdef MU_CASTLESIEGE_CL_PROTOCOL_20041122	// 뮤 공성전 - 뮤 클라이언트 관련 패킷 정의

//----------------------------------------------------------------------------
// CG [0xB2][0x00] 클라이언트가 공성전 상태정보를 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_CASTLESIEGESTATE, *LPPMSG_REQ_CASTLESIEGESTATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x00] 클라이언트가 공성전 상태정보를 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:성주인없음/3:성정보비정상/4:성길드없어짐) -> 성공이면 길드정보는 확실히 받음
	CHAR		cCastleSiegeState;			// 현재 공성전 상태
	BYTE		btStartYearH;				// 상태 시작 - 년 (상위 바이트)
	BYTE		btStartYearL;				// 상태 시작 - 년 (하위 바이트)
	BYTE		btStartMonth;				// 상태 시작 - 달
	BYTE		btStartDay;					// 상태 시작 - 일
	BYTE		btStartHour;				// 상태 시작 - 시
	BYTE		btStartMinute;				// 상태 시작 - 분
	BYTE		btEndYearH;					// 상태 종료 - 년 (상위 바이트)
	BYTE		btEndYearL;					// 상태 종료 - 년 (하위 바이트)
	BYTE		btEndMonth;					// 상태 종료 - 달
	BYTE		btEndDay;					// 상태 종료 - 일
	BYTE		btEndHour;					// 상태 종료 - 시
	BYTE		btEndMinute;				// 상태 종료 - 분
	BYTE		btSiegeStartYearH;			// 공성전 시작 - 년 (상위 바이트)
	BYTE		btSiegeStartYearL;			// 공성전 시작 - 년 (하위 바이트)
	BYTE		btSiegeStartMonth;			// 공성전 시작 - 달
	BYTE		btSiegeStartDay;			// 공성전 시작 - 일
	BYTE		btSiegeStartHour;			// 공성전 시작 - 시
	BYTE		btSiegeStartMinute;			// 공성전 시작 - 분
	CHAR		cOwnerGuild[MAX_GUILDNAMESTRING];		// 수성측 길드
	CHAR		cOwnerGuildMaster[MAX_IDSTRING];		// 수성측 길드 마스터
#ifdef MU_CASTLESIEGE_STATE_SEC_VIEW_20050413
	CHAR		btStateLeftSec1;			// 다음 상태까지 남은 시간 - 상위 1번째
	CHAR		btStateLeftSec2;			// 다음 상태까지 남은 시간 - 상위 2번째
	CHAR		btStateLeftSec3;			// 다음 상태까지 남은 시간 - 상위 3번째
	CHAR		btStateLeftSec4;			// 다음 상태까지 남은 시간 - 상위 4번째
#endif	
} PMSG_ANS_CASTLESIEGESTATE, *LPPMSG_ANS_CASTLESIEGESTATE;


//----------------------------------------------------------------------------
// CG [0xB2][0x01] 공성전 등록을 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_REGCASTLESIEGE, *LPPMSG_REQ_REGCASTLESIEGE;


//----------------------------------------------------------------------------
// GC [0xB2][0x01] 공성전 등록을 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:이미등록됨/3:수성측길드/4:길드없음/5:길마레벨부족/6:길드에속하지않음/7:시간이지났음/8:길드원수가부족)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// 길드이름
} PMSG_ANS_REGCASTLESIEGE, *LPPMSG_ANS_REGCASTLESIEGE;


//----------------------------------------------------------------------------
// CG [0xB2][0x02] 공성전 포기를 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btGiveUp;					// 포기여부 (0:포기않함/1:포기)
} PMSG_REQ_GIVEUPCASTLESIEGE, *LPPMSG_REQ_GIVEUPCASTLESIEGE;


//----------------------------------------------------------------------------
// GC [0xB2][0x02] 공성전 포기를 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:등록된길드아님/3:시간이지났음)
	BYTE		btIsGiveUp;					// 포기여부 (0:포기않함/1:포기)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// 길드이름
} PMSG_ANS_GIVEUPCASTLESIEGE, *LPPMSG_ANS_GIVEUPCASTLESIEGE;


//----------------------------------------------------------------------------
// CG [0xB2][0x03] 자신의 길드의 공성등록에 대한 정보를 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_GUILDREGINFO, *LPPMSG_REQ_GUILDREGINFO;


//----------------------------------------------------------------------------
// GC [0xB2][0x03] 자신의 길드의 공성등록에 대한 정보를 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:등록된길드아님)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// 길드이름
	BYTE		btGuildMark1;				// 성주의 표식 - DWORD를 최상위 바이트 부터 4단계로 나눔
	BYTE		btGuildMark2;
	BYTE		btGuildMark3;
	BYTE		btGuildMark4;
	BYTE		btIsGiveUp;					// 포기여부
	BYTE		btRegRank;					// 등록순위
} PMSG_ANS_GUILDREGINFO, *LPPMSG_ANS_GUILDREGINFO;


//----------------------------------------------------------------------------
// CG [0xB2][0x04] 자신의 길드의 성주의 표식 등록 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btItemPos;					// 인벤토리에서 성주의 표식의 위치
} PMSG_REQ_REGGUILDMARK, *LPPMSG_REQ_REGGUILDMARK;


//----------------------------------------------------------------------------
// GC [0xB2][0x04] 자신의 길드의 성주의 표식 등록 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:등록된길드아님/3:성주의표식이잘못됨)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// 길드이름
	BYTE		btGuildMark1;				// 성주의 표식 - DWORD를 최상위 바이트 부터 4단계로 나눔
	BYTE		btGuildMark2;
	BYTE		btGuildMark3;
	BYTE		btGuildMark4;
} PMSG_ANS_REGGUILDMARK, *LPPMSG_ANS_REGGUILDMARK;


//----------------------------------------------------------------------------
// CG [0xB2][0x05] 수성측 NPC 구매 요청 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iNpcNumber;					// NPC번호
	INT			iNpcIndex;					// NPC인덱스
} PMSG_REQ_NPCBUY, *LPPMSG_REQ_NPCBUY;


//----------------------------------------------------------------------------
// GC [0xB2][0x05] 수성측 NPC 구매 요청 응답 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:권한이없음/3:돈이모자람/4:NPC가이미존재)
	INT			iNpcNumber;					// NPC번호
	INT			iNpcIndex;					// NPC인덱스
} PMSG_ANS_NPCBUY, *LPPMSG_ANS_NPCBUY;


//----------------------------------------------------------------------------
// CG [0xB2][0x06] 수성측 NPC 수리 요청 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iNpcNumber;					// NPC번호
	INT			iNpcIndex;					// NPC인덱스
} PMSG_REQ_NPCREPAIR, *LPPMSG_REQ_NPCREPAIR;


//----------------------------------------------------------------------------
// GC [0xB2][0x06] 수성측 NPC 수리 요청 응답 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:권한이없음/3:돈이모자람)
	INT			iNpcNumber;					// NPC번호
	INT			iNpcIndex;					// NPC인덱스
	INT			iNpcHP;						// 체력
	INT			iNpcMaxHP;					// 최대체력
} PMSG_ANS_NPCREPAIR, *LPPMSG_ANS_NPCREPAIR;


//----------------------------------------------------------------------------
// CG [0xB2][0x07] 수성측 NPC 업그레이드 요청 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iNpcNumber;					// NPC번호
	INT			iNpcIndex;					// NPC인덱스
	INT			iNpcUpType;					// 업그레이드 타입 (1:방어력/2:회복력/3:내구도(최대생명))
	INT			iNpcUpValue;				// 업그레이드 수치 (3번 내구도의 경우 실제로 올라갈 내구도 값)
} PMSG_REQ_NPCUPGRADE, *LPPMSG_REQ_NPCUPGRADE;


//----------------------------------------------------------------------------
// GC [0xB2][0x07] 수성측 NPC 업그레이드 요청 응답 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:권한이없음/3:돈이모자람/4:보석이모자람/5:타입이잘못됨/6:요청값이잘못됨/7:NPC가존재하지않음)
	INT			iNpcNumber;					// NPC번호
	INT			iNpcIndex;					// NPC인덱스
	INT			iNpcUpType;					// 업그레이드 타입 (1:방어력/2:회복력/3:내구도(최대생명))
	INT			iNpcUpValue;				// 업그레이드 수치 (3번 내구도의 경우 실제로 올라갈 내구도 값)
} PMSG_ANS_NPCUPGRADE, *LPPMSG_ANS_NPCUPGRADE;


//----------------------------------------------------------------------------
// CG [0xB2][0x08] 현재 성의 자금, 세금 정보 요청 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_TAXMONEYINFO, *LPPMSG_REQ_TAXMONEYINFO;


//----------------------------------------------------------------------------
// GC [0xB2][0x08] 현재 성의 자금, 세금 정보 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:권한이없음)
	BYTE		btTaxRateChaos;				// 현재 성의 세율 - 카오스조합
	BYTE		btTaxRateStore;				// 현재 성의 세율 - 상점
	BYTE		btMoney1;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 1)
	BYTE		btMoney2;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 2)
	BYTE		btMoney3;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 3)
	BYTE		btMoney4;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 4)
	BYTE		btMoney5;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 5)
	BYTE		btMoney6;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 6)
	BYTE		btMoney7;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 7)
	BYTE		btMoney8;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 8)
} PMSG_ANS_TAXMONEYINFO, *LPPMSG_ANS_TAXMONEYINFO;


//----------------------------------------------------------------------------
// CG [0xB2][0x09] 현재 성의 세율 변경 요청 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btTaxType;					// 세율 종류 (1:카오스조합/2:상점/3:사냥터)
//	BYTE		btTaxRate;					// 변경하려는 세율 (0~200%)
	BYTE		btTaxRate1;					// 변경하려는 세율 (0~200%, 사냥터의 경우 세금)
	BYTE		btTaxRate2;					// 변경하려는 세율 (0~200%, 사냥터의 경우 세금)
	BYTE		btTaxRate3;					// 변경하려는 세율 (0~200%, 사냥터의 경우 세금)
	BYTE		btTaxRate4;					// 변경하려는 세율 (0~200%, 사냥터의 경우 세금)
} PMSG_REQ_TAXRATECHANGE, *LPPMSG_REQ_TAXRATECHANGE;


//----------------------------------------------------------------------------
// CG [0xB2][0x09] 현재 성의 세율 변경 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:권한이없음)
	BYTE		btTaxType;					// 세율 종류 (1:카오스조합/2:상점/3:사냥터)
//	BYTE		btTaxRate;					// 변경된 현재 세율
	BYTE		btTaxRate1;					// 변경하려는 세율 (0~200%, 사냥터의 경우 세금)
	BYTE		btTaxRate2;					// 변경하려는 세율 (0~200%, 사냥터의 경우 세금)
	BYTE		btTaxRate3;					// 변경하려는 세율 (0~200%, 사냥터의 경우 세금)
	BYTE		btTaxRate4;					// 변경하려는 세율 (0~200%, 사냥터의 경우 세금)
} PMSG_ANS_TAXRATECHANGE, *LPPMSG_ANS_TAXRATECHANGE;


//----------------------------------------------------------------------------
// CG [0xB2][0x10] 현재 성의 자금 출금 요청 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btMoney1;					// 출금 하려는 돈 (INT의 상위 바이트 부터 1)
	BYTE		btMoney2;					// 출금 하려는 돈 (INT의 상위 바이트 부터 2)
	BYTE		btMoney3;					// 출금 하려는 돈 (INT의 상위 바이트 부터 3)
	BYTE		btMoney4;					// 출금 하려는 돈 (INT의 상위 바이트 부터 4)
} PMSG_REQ_MONEYDRAWOUT, *LPPMSG_REQ_MONEYDRAWOUT;


//----------------------------------------------------------------------------
// CG [0xB2][0x10] 현재 성의 자금 출금 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:권한이없음)
	BYTE		btMoney1;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 1)
	BYTE		btMoney2;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 2)
	BYTE		btMoney3;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 3)
	BYTE		btMoney4;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 4)
	BYTE		btMoney5;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 5)
	BYTE		btMoney6;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 6)
	BYTE		btMoney7;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 7)
	BYTE		btMoney8;					// 현재 성의 자금 (INT64의 최상위 바이트 부터 8)
} PMSG_ANS_MONEYDRAWOUT, *LPPMSG_ANS_MONEYDRAWOUT;

/*
//----------------------------------------------------------------------------
// CG [0xB2][0x11] 현재 레버와 연결된 성문정보 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btIndex1;					// 레버의 인덱스 (INT의 상위 바이트 부터 1)
	BYTE		btIndex2;					// 레버의 인덱스 (INT의 상위 바이트 부터 2)
	BYTE		btIndex3;					// 레버의 인덱스 (INT의 상위 바이트 부터 3)
	BYTE		btIndex4;					// 레버의 인덱스 (INT의 상위 바이트 부터 4)
} PMSG_REQ_CSGATESTATE, *LPPMSG_REQ_CSGATESTATE;
*/

//----------------------------------------------------------------------------
// GC [0xB2][0x11] 현재 레버와 연결된 성문정보 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 성문정보 요청 결과값 (0:실패/1:성공/2:성문없음/3:레버없음/4:권한없음)
	BYTE		btIndex1;					// 레버와 연결된 성문의 인덱스 (WORD의 상위 바이트 부터 1)
	BYTE		btIndex2;					// 레버와 연결된 성문의 인덱스 (WORD의 상위 바이트 부터 2)
} PMSG_ANS_CSGATESTATE, *LPPMSG_ANS_CSGATESTATE;


//----------------------------------------------------------------------------
// CG [0xB2][0x12] 특정 성문을 열고 닫으라는 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btOperate;					// 작동상황 (0:닫기/1:열기)
	BYTE		btIndex1;					// 레버와 연결된 성문의 인덱스 (WORD의 상위 바이트 부터 1)
	BYTE		btIndex2;					// 레버와 연결된 성문의 인덱스 (WORD의 상위 바이트 부터 2)
} PMSG_REQ_CSGATEOPERATE, *LPPMSG_REQ_CSGATEOPERATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x12] 특정 성문을 열고 닫으라는 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 성문정보 요청 결과값 (0:실패/1:성공/2:성문없음/3:레버없음/4:권한없음)
	BYTE		btOperate;					// 작동결과 (0:닫기/1:열기)
	BYTE		btIndex1;					// 레버와 연결된 성문의 인덱스 (WORD의 상위 바이트 부터 1)
	BYTE		btIndex2;					// 레버와 연결된 성문의 인덱스 (WORD의 상위 바이트 부터 2)
} PMSG_ANS_CSGATEOPERATE, *LPPMSG_ANS_CSGATEOPERATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x13] 특정 성문의 현재 상태정보를 보내줌 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btOperate;					// 작동결과 (0:닫기/1:열기)
	BYTE		btIndex1;					// 레버와 연결된 성문의 인덱스 (WORD의 상위 바이트 부터 1)
	BYTE		btIndex2;					// 레버와 연결된 성문의 인덱스 (WORD의 상위 바이트 부터 2)
} PMSG_ANS_CSGATECURSTATE, *LPPMSG_ANS_CSGATECURSTATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x14] 특정 사용자의 왕관 스위치 작동여부를 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btIndex1;					// 왕관스위치의 인덱스 (WORD의 상위 바이트 부터 1)
	BYTE		btIndex2;					// 왕관스위치의 인덱스 (WORD의 상위 바이트 부터 2)
	BYTE		btUserIndex1;				// 왕관스위치를 사용하고 있는 사용자의 인덱스 (WORD의 상위 바이트 부터 1)
	BYTE		btUserIndex2;				// 왕관스위치를 사용하고 있는 사용자의 인덱스 (WORD의 상위 바이트 부터 2)
	BYTE		btSwitchState;				// (0:스위치땜/1:스위치작동/2:다른사람이사용중)
} PMSG_ANS_NOTIFYSWITCHPROC, *LPPMSG_ANS_NOTIFYSWITCHPROC;


//----------------------------------------------------------------------------
// GC [0xB2][0x15] 특정 사용자의 왕관 직인등록 여부를 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btCrownState;					// (0:직인등록시작/1:직인등록성공/2:직인등록실패/3:다른사람이사용중)
												// (4:왕관스위치작동유저와 다른 공성진영)
	
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
	DWORD		dwAccumulatedCrownAccessTime;	// 누적된 직인 등록 시간;
#endif

} PMSG_ANS_NOTIFYCROWNPROC, *LPPMSG_ANS_NOTIFYCROWNPROC;


//----------------------------------------------------------------------------
// GC [0xB2][0x16] 왕관 주변의 사용자들에게 왕관의 상태를 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btCrownState;				// (0:방어막해제/1:방어막생성/2:등록성공)
} PMSG_ANS_NOTIFYCROWNSTATE, *LPPMSG_ANS_NOTIFYCROWNSTATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x17] 현재 공성전 시작 여부를 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btStartState;				// 시작 여부 (0:공성진행아님/1:공성진행중임)
} PMSG_ANS_NOTIFYCSSTART, *LPPMSG_ANS_NOTIFYCSSTART;


//----------------------------------------------------------------------------
// GC [0xB2][0x18] 현재 공성전 진행 상태를 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btCastleSiegeState;			// (0:왕의직인 등록시도/1:중간승리)
	BYTE		szGuildName[MAX_GUILDNAMESTRING];				// 해당 길드(연합) 이름
} PMSG_ANS_NOTIFYCSPROGRESS, *LPPMSG_ANS_NOTIFYCSPROGRESS;


//----------------------------------------------------------------------------
// GC [0xB2][0x19] 자신의 공/수 참여 속성 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btCsJoinSide;				// (0:참여않함/1:수성측/2:공성측)
} PMSG_ANS_NOTIFYCSJOINSIDE, *LPPMSG_ANS_NOTIFYCSJOINSIDE;


//----------------------------------------------------------------------------
// GC [0xB2][0x1A] 현재 맵서버군에 적용된 세율을 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btTaxType;					// 세율 형태 (1:카오스조합/2:상점)
	BYTE		btTaxRate;					// 세율
} PMSG_ANS_MAPSVRTAXINFO, *LPPMSG_ANS_MAPSVRTAXINFO;


//----------------------------------------------------------------------------
// CG [0xB2][0x1B] 미니맵 데이터를 주기적으로 보내줄 것을 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_MINIMAPDATA, *LPPMSG_REQ_MINIMAPDATA;


//----------------------------------------------------------------------------
// GC [0xB2][0x1B] 미니맵 데이터를 주기적으로 보내줄 것을 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE btResult;		// 미니맵 데이터 요청 결과값 (0:실패/1:성공/2:공성전중아님/3:권한없음)
} PMSG_ANS_MINIMAPDATA, *LPPMSG_ANS_MINIMAPDATA;


//----------------------------------------------------------------------------
// CG [0xB2][0x1C] 미니맵 데이터를 더이상 보내지 말라고 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_STOPMINIMAPDATA, *LPPMSG_REQ_STOPMINIMAPDATA;


//----------------------------------------------------------------------------
// CG [0xB2][0x1D] 공성전 참여길드 길마가 길원들에게 작전 지시 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE btTeam;		// 팀번호
	BYTE btX;			// X좌표
	BYTE btY;			// Y좌표
	BYTE btCommand;		// 명령 (0:공격/1:방어/2:대기)
} PMSG_REQ_CSCOMMAND, *LPPMSG_REQ_CSCOMMAND;


//----------------------------------------------------------------------------
// GC [0xB2][0x1D] 공성전 참여길드 길마가 길원들에게 작전 지시 보냄 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE btTeam;		// 팀번호
	BYTE btX;			// X좌표
	BYTE btY;			// Y좌표
	BYTE btCommand;		// 명령 (0:공격/1:방어/2:대기)
} PMSG_ANS_CSCOMMAND, *LPPMSG_ANS_CSCOMMAND;


//----------------------------------------------------------------------------
// GC [0xB2][0x1E] 공성전 중 남은 시간을 사용자들에게 주기적으로 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE btHour;		// 남은 시간
	BYTE btMinute;		// 남은 분
} PMSG_ANS_CSLEFTTIMEALARM, *LPPMSG_ANS_CSLEFTTIMEALARM;


//----------------------------------------------------------------------------
// CG [0xB2][0x1F] 성 사냥터 입장 가능여부 지정 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btHuntZoneEnter;			// 사냥터 입장가능 여부 (0:불가능 / 1:가능)
} PMSG_REQ_CSHUNTZONEENTER, *LPPMSG_REQ_CSHUNTZONEENTER;


//----------------------------------------------------------------------------
// GC [0xB2][0x1F] 성 사냥터 입장 가능여부 지정 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:권한이없음)
	BYTE		btHuntZoneEnter;			// 사냥터 입장가능 여부 (0:불가능 / 1:가능)
} PMSG_ANS_CSHUNTZONEENTER, *LPPMSG_ANS_CSHUNTZONEENTER;


//----------------------------------------------------------------------------
// CG [0xB3] 공성관련 DB로 관리되는 특정 NPC의 목록을 요청 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		btMonsterCode;				// DB관리 NPC의 종류 (1:성문/2:석상)
} PMSG_REQ_NPCDBLIST, *LPPMSG_REQ_NPCDBLIST;


//----------------------------------------------------------------------------
// GC [0xB3] 공성관련 DB로 관리되는 특정 NPC의 목록을 요청 응답 (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:권한이없음)
	INT			iCount;
} PMSG_ANS_NPCDBLIST, *LPPMSG_ANS_NPCDBLIST;

typedef struct
{
	INT			iNpcNumber;					// NPC번호
	INT			iNpcIndex;					// NPC인덱스
	INT			iNpcDfLevel;				// 방어력 레벨
	INT			iNpcRgLevel;				// 회복력 레벨
	INT			iNpcMaxHp;					// 최대 HP
	INT			iNpcHp;						// HP
	BYTE		btNpcX;						// 좌표 - X
	BYTE		btNpcY;						// 좌표 - Y
	BYTE		btNpcLive;					// NPC가 현재 존재하는지 여부
} PMSG_NPCDBLIST, *LPPMSG_NPCDBLIST;


//----------------------------------------------------------------------------
// CG [0xB4] 공성 등록 길드목록 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_REQ_CSREGGUILDLIST, *LPPMSG_REQ_CSREGGUILDLIST;


//----------------------------------------------------------------------------
// GC [0xB4] 공성 등록 길드목록 요청 응답 (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공)
	INT			iCount;
} PMSG_ANS_CSREGGUILDLIST, *LPPMSG_ANS_CSREGGUILDLIST;

typedef struct
{
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// 길드이름
	BYTE		btRegMarks1;				// 성주의 표식의 개수 (INT의 상위 바이트 부터 1)
	BYTE		btRegMarks2;				// 성주의 표식의 개수 (INT의 상위 바이트 부터 2)
	BYTE		btRegMarks3;				// 성주의 표식의 개수 (INT의 상위 바이트 부터 3)
	BYTE		btRegMarks4;				// 성주의 표식의 개수 (INT의 상위 바이트 부터 4)
	BYTE		btIsGiveUp;					// 포기여부 (0:포기안함/1:포기함)
	BYTE		btSeqNum;					// 등록순서
} PMSG_CSREGGUILDLIST, *LPPMSG_CSREGGUILDLIST;


//----------------------------------------------------------------------------
// CG [0xB5] 공성 확정 길드목록 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_REQ_CSATTKGUILDLIST, *LPPMSG_REQ_CSATTKGUILDLIST;


//----------------------------------------------------------------------------
// GC [0xB5] 공성 확정 길드목록 요청 응답 (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btResult;					// 요청 결과 (0:실패/1:성공/2:기간아님/3:데이터준비않됨)
	INT			iCount;
} PMSG_ANS_CSATTKGUILDLIST, *LPPMSG_ANS_CSATTKGUILDLIST;

typedef struct
{
	BYTE		btCsJoinSide;				// 참여측 정보 (1:수성측/2이상:공성측)
	BYTE		btGuildInvolved;			// 길드가 연합의 주측 길드인지 여부 (0:아님/1:주축임)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// 길드이름
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
	INT			iGuildScore;				// 길드 선정 당시의 점수
#endif	
} PMSG_CSATTKGUILDLIST, *LPPMSG_CSATTKGUILDLIST;


//----------------------------------------------------------------------------
// GC [0xB6] 공성전 중에 같은 길드의 미니맵정보를 보낸다. (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	INT			iCount;
} PMSG_ANS_SENDMINIMAPDATA, *LPPMSG_ANS_SENDMINIMAPDATA;

typedef struct
{
	BYTE		btX;
	BYTE		btY;
} PMSG_SENDMINIMAPDATA, *LPPMSG_SENDMINIMAPDATA;


//----------------------------------------------------------------------------
// GC [0xBB] 공성전 중에 존재하는 성문/석상의 미니맵정보를 보낸다. (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	INT			iCount;
} PMSG_ANS_SENDNPCMINIMAPDATA, *LPPMSG_ANS_SENDNPCMINIMAPDATA;

typedef struct
{
	BYTE		btNpcType;		// 공성전 NPC 종류 (0:성문 / 1:석상)
	BYTE		btX;
	BYTE		btY;
} PMSG_SENDNPCMINIMAPDATA, *LPPMSG_SENDNPCMINIMAPDATA;


#endif


//----------------------------------------------------------------------------
// 공성 프로토콜 ( FOR 공성 )
//
// Lastest : 20051018
// Related Procol : [0xB2][0x00] - [0xB2][0x1F]
//----------------------------------------------------------------------------
#ifdef MODIFY_CASTLESIEGE_NOTIFY_CROWNSWITCH_USER_20051017

//----------------------------------------------------------------------------
// GC [0xB2][0x20] 현재 공성전 진행 상태를 알림 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btIndex1;								// 왕관스위치의 인덱스 (WORD의 상위 바이트 부터 1)
	BYTE		btIndex2;								// 왕관스위치의 인덱스 (WORD의 상위 바이트 부터 2)
	BYTE		btSwitchState;							// (0:스위치땜/1:스위치작동)
	BYTE		btJoinSide;								// 해당 길드(연합) JoinSide(공성번호)
	BYTE		szGuildName[MAX_GUILDNAMESTRING];		// 해당 길드(연합) 이름
	BYTE		szUserName[MAX_IDSTRING];				// 스위치 사용자 이름
} PMSG_ANS_NOTIFY_CROWNSWITCH_INFO, *LPPMSG_ANS_NOTIFY_CROWNSWITCH_INFO;

#endif


//----------------------------------------------------------------------------
// New Skill 프로토콜 ( FOR 공성 )
//
// Lastest : 20041208
// Related Procol : 
//----------------------------------------------------------------------------
typedef struct {}__SKILL_PROTOCOL_H___;

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
// 직책 스킬 사용 조건

//----------------------------------------------------------------------------
// GC[0xB8][0x01] 직책 스킬 발동 조건 KillCount
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btKillCount;				// Current KillCount
	
} PMSG_KILLCOUNT, *LPPMSG_KILLCOUNT;

//----------------------------------------------------------------------------
// GC[0xB8][0x02] 직책 스킬을 사용 할 수 없음
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btErrorCode;				// Error Code
	BYTE		btInSufficientKillCount;	// InSufficient KillCount
	
} PMSG_INSUFFICIENT_KILLCOUNT, *LPPMSG_INSUFFICIENT_KILLCOUNT;

#endif


//----------------------------------------------------------------------------
// Weapon 시스템 프로토콜 ( FOR 공성 )
//
// Lastest : 20041207
// Related Procol : 
//----------------------------------------------------------------------------
typedef struct {}__WEAPON_PROTOCOL_H___;

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
// 공성 무기 인터페이스 프로토콜

//----------------------------------------------------------------------------
// CG[0xB7][0x00] Weapon 인터페이스 사용 요청 응답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btResult;				// 0x00 사용 할 수 없음
										// 0x01 사용 할 수 있음
	
	BYTE		btWeaponType;			// 무기 종류

	BYTE		btObjIndexH;			// 사용하는 Weapon의 Object Index
	BYTE		btObjIndexL;
	
} PMSG_ANS_USE_WEAPON_INTERFACE, *LPPMSG_ANS_USE_WEAPON_INTERFACE;

//----------------------------------------------------------------------------
// CG[0xB7][0x01] Weapon 사용 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btObjIndexH;			// 사용하는 Weapon의 Object Index
	BYTE		btObjIndexL;
	
	BYTE		btTargetIndex;			// 공성 무기일 경우는 미리 정해져 있다.

} PMSG_REQ_USEWEAPON, *LPPMSG_REQ_USEWEAPON;

//----------------------------------------------------------------------------
// GC[0xB7][0x01] Weapon 사용 요청에 대한 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btResult;

	BYTE		btObjIndexH;			// 사용 요청된 Weapon의 Object Index
	BYTE		btObjIndexL;

	BYTE		btWeaponType;			// 무기 종류

	BYTE		btPointX;				// 타겟 위치 X
	BYTE		btPointY;				// 타겟 위치 Y

} PMSG_ANS_USEWEAPON, *LPPMSG_ANS_USEWEAPON;

//----------------------------------------------------------------------------
// GC[0xB7][0x02] Weapon의 타겟 지역의 캐릭터들에게 알림
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btWeaponType;			// 무기 종류
	
	BYTE		btPointX;				// 폭발 위치 X
	BYTE		btPointY;				// 폭발 위치 Y

} PMSG_NOTIFY_REGION_OF_WEAPON, *LPPMSG_NOTIFY_REGION_OF_WEAPON;

//----------------------------------------------------------------------------
// GC[0xB7][0x03] Weapon의 타겟 지역의 '공격받은' 캐릭터들에게 알림
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		NumberH;
	BYTE		NumberL;

	BYTE		btWeaponType;			// 무기 종류
	
} PMSG_NOTIFY_TARGET_OF_WEAPON, *LPPMSG_NOTIFY_TARGET_OF_WEAPON;

//----------------------------------------------------------------------------
// CG[0xB7][0x04] Weapon 애니메이션이 끝나감을 알림. 데미지 정보 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btObjIndexH;			// 사용하는 Weapon의 Object Index
	BYTE		btObjIndexL;
	
} PMSG_REQ_WEAPON_DAMAGE_VALUE, *LPPMSG_REQ_WEAPON_DAMAGE_VALUE;

#endif // ADD_NEW_WEAPON_FOR_CASTLE_01_20041116


//----------------------------------------------------------------------------
// NEW ITEM 프로토콜 ( FOR 공성 ), Castle 관리 및 장식
//
// Lastest : 20041215	20041227
// Related Procol : 
//----------------------------------------------------------------------------

typedef struct {}__CS_ITEM_PROTOCOL_H___;

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
//----------------------------------------------------------------------------
// GC GC[0xB9][0x01] Life Stone의 생성 상태를 보내준다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btObjIndexH;				
	BYTE		btObjIndexL;

	BYTE		btCreationState;		// 생성 되었는가? 생성 중인가?	
} PMSG_NOTIFY_OBJECT_CREATION_STATE, *LPPMSG_NOTIFY_OBJECT_CREATION_STATE;

#endif

#ifdef ADD_NEW_CASTLE_FLAG_01_20041227
//----------------------------------------------------------------------------
// GC GC[0xB9][0x02] Castle의 깃발에 Mark를 그릴수 있도록 
//----------------------------------------------------------------------------

typedef struct
{
	PBMSG_HEAD2	h;

} PMSG_REQ_GUILDMARK_OF_CASTLEOWNER, *LPPMSG_REQ_GUILDMARK_OF_CASTLEOWNER;

typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		GuildMarkOfCastleOwner[32];		// 길드마크
} PMSG_ANS_GUILDMARK_OF_CASTLEOWNER, *LPPMSG_ANS_GUILDMARK_OF_CASTLEOWNER;

#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
//----------------------------------------------------------------------------
// GC GC[0xB9][0x03] Castle의 사냥터 문지기의 대답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;			// 0x00 성주가 입장 불가능케 했음	(일반인)
									// 0x01 성주가 입장 가능케 했음		(일반인)
									// 0x02 성의 길드(연합원)에게 대답	(성길드원)
									// 0x03 성주에게 대답				(성주)

	BYTE		btUsable;			// 0x00 출입불가능, 0x01 출입가능
	INT			iCurrentPrice;		// 현재 사냥터 입장요금
	INT			iMaxPrice;			// 최대 사냥터 입장요금
	INT			iUnitOfPrice;		// 단위 사냥터 입장요금

} PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE, *LPPMSG_ANS_GUARD_IN_CASTLE_HUNTZONE;


//----------------------------------------------------------------------------
// GC GC[0xB9][0x05] 사냥터 문지기에게 입장 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iPrice;				// 사냥터 입장요금

} PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE, *LPPMSG_REQ_MOVE_TO_CASTLE_HUNTZONE;

typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;			// 0x00 출입불가능, 0x01 출입가능

} PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE, *LPPMSG_ANS_MOVE_TO_CASTLE_HUNTZONE;

#endif


//----------------------------------------------------------------------------
// 뷰포트 프로토콜 ( FOR 공성 )
//
// Lastest : 20041207
// Related Procol : 0x12, 0x13, 0x65, 0x66, 0x67
//----------------------------------------------------------------------------
typedef struct {}__VIEWPORT_PROTOCOL_H___;

#ifdef ADD_NEW_VIEWPORT_SYSTEM_01_20041201

//----------------------------------------------------------------------------
// GC [0x68] 뷰포트 정보 
//----------------------------------------------------------------------------

// 1) 사용자
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btCount;
} PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT, *LPPMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT;

typedef struct
{
	BYTE		btObjType;						// 캐릭, 몬스터, NPC
	BYTE		btObjClassH;					// 클래스
	BYTE		btObjClassL;

	BYTE		btObjIndexH;					// ObjIndex
	BYTE		btObjIndexL;

	BYTE		btX;							// Obj Location X
	BYTE		btY;							// Obj Location Y

	// 뜨아...gObjMakePreviewSet()
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE		CharSet[MAX_PREVIEWCHARSET+8];	// 맨 앞의 1바이트 (클래스 정보) 는 파싱이 귀찮은 관계로 별도로 사용 (위)
#else
	BYTE		CharSet[MAX_PREVIEWCHARSET+3];
#endif

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE		BuffEffectCount;
	BYTE		BuffEffectList[MAX_BUFF_SLOT_INDEX];
#else
	DWORD		dwViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
} PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY, *LPPMSG_WEAPON_OWNER_VIEWPORT_NOTIFY;

#endif // ADD_NEW_VIEWPORT_SYSTEM_01_20041201


#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714

//----------------------------------------------------------------------------
// GC [0x69] 몬스터가 스킬을 사용했다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		btMonsterSkillNumberH;
	BYTE		btMonsterSkillNumberL;
#else
	BYTE		btMonsterSkillNumber;
#endif
	WORD		wObjIndex;			
	WORD		wTargetObjIndex;
} PMSG_USE_MONSTERSKILL, *LPPMSG_USE_MONSTERSKILL;

#endif // ADD_NEW_MONSTER_SYSTEM_01_20050714


#ifdef JEWEL_MIX_SYSTEM_20050309		// 보석조합 시스템 추가패킷

//----------------------------------------------------------------------------
// CG [0xBC][0x00] 보석조합 생성 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btJewelType;			// 보석 타입 (0:축석 / 1:영석)
	BYTE		btJewelMix;				// 조합 타입 (0:10 / 1:20 / 2:30)
} PMSG_REQ_JEWEL_MIX, *LPPMSG_REQ_JEWEL_MIX;

//----------------------------------------------------------------------------
// CG [0xBC][0x00] 보석조합 생성 요청 응답 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;				// 결과		0 : 오류
										//			1 : 성공
										//			2 : 실패 - 보석타입 오류
										//			3 : 실패 - 조합타입 오류
										//			4 : 실패 - 보석이 부족
										//			5 : 실패 - 조합금이 부족
} PMSG_ANS_JEWEL_MIX, *LPPMSG_ANS_JEWEL_MIX;

//----------------------------------------------------------------------------
// CG [0xBC][0x01] 보석조합 해체 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btJewelType;			// 보석 타입 (0:축석 / 1:영석)
	BYTE		btJewelLevel;			// 보석 레벨 -> 조합 타입 (0:10 / 1:20 / 2:30)
	BYTE		btJewelPos;				// 인벤토리 내에 해당 보석조합 아이템의 위치
} PMSG_REQ_JEWEL_UNMIX, *LPPMSG_REQ_JEWEL_UNMIX;

//----------------------------------------------------------------------------
// CG [0xBC][0x01] 보석조합 해체 요청 (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;				// 결과		0 : 오류
										//			1 : 성공
										//			2 : 실패 - 보석타입 오류
										//			3 : 실패 - 보석레벨 오류
										//			4 : 실패 - 아이템이 없음
										//			5 : 실패 - 인벤토리 범위 오류
										//			6 : 실패 - 해체할 아이템 오류
										//			7 : 실패 - 인벤토리 공간 부족
										//			8 : 실패 - 조합금이 부족
} PMSG_ANS_JEWEL_UNMIX, *LPPMSG_ANS_JEWEL_UNMIX;

#endif


//----------------------------------------------------------------------------
// 크라이울프 프로토콜 : 0xBD 사용
//
// Lastest : 20050912
// Related Procol : 
//----------------------------------------------------------------------------

#ifdef MU_CRYWOLF_CL_PROTOCOL_20050505
//----------------------------------------------------------------------------
// CG [0xBD][0x00] 클라이언트가 크라이울프 점령 및 상태정보를 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
} PMSG_REQ_CRYWOLF_INFO, *LPPMSG_REQ_CRYWOLF_INFO;

//----------------------------------------------------------------------------
// GC [0xBD][0x00] 클라이언트가 크라이울프 점령 및 상태정보 요청에 대한 응답 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE	btOccupationState;		// 점령 정보 ( 0:평화 / 1:점령 / 2:전쟁 )
	BYTE	btCrywolfState;			// 크라이울프 진행 상태
	
} PMSG_ANS_CRYWOLF_INFO, *LPPMSG_ANS_CRYWOLF_INFO;


//----------------------------------------------------------------------------
// GC [0xBD][0x02] 방어막, 제단 상태 정보
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	INT		iCrywolfStatueHP;		// 방어막 HP
	
	BYTE	btAltarState1;			// 제단 1 상태
	BYTE	btAltarState2;			// 제단 2 상태
	BYTE	btAltarState3;			// 제단 3 상태
	BYTE	btAltarState4;			// 제단 4 상태
	BYTE	btAltarState5;			// 제단 5 상태

} PMSG_ANS_CRYWOLF_STATUE_ALTAR_INFO, *LPPMSG_ANS_CRYWOLF_STATUE_ALTAR_INFO;


//----------------------------------------------------------------------------
// CG [0xBD][0x03] 특정 제단과 사제 계약 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btObjIndexH;		// 제단 인덱스
	BYTE		btObjIndexL;
	
} PMSG_REQ_CRYWOLF_ALTAR_CONTRACT, *LPPMSG_REQ_CRYWOLF_ALTAR_CONTRACT;


//----------------------------------------------------------------------------
// CG [0xBD][0x03] 특정 제단과 사제 계약 요청에 대한 응답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btResult;			// 계약 결과 ( 0: 실패 / 1: 성공 )

	BYTE		btAltarState;		// 제단 상태
	BYTE		btObjIndexH;		// 제단 인덱스
	BYTE		btObjIndexL;

} PMSG_ANS_CRYWOLF_ALTAR_CONTRACT, *LPPMSG_ANS_CRYWOLF_ALTAR_CONTRACT;

//----------------------------------------------------------------------------
// GC [0xBD][0x04] 크라이울프 진행 시간
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btHour;				// 남은 시간
	BYTE		btMinute;			// 남은 분

} PMSG_ANS_CRYWOLF_LEFTTIME, *LPPMSG_ANS_CRYWOLF_LEFTTIME;

//----------------------------------------------------------------------------
// GC [0xBD][0x05] 크라이울프 보스 몬스터들 상태 정보 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	int			iMonster1HP;		// 발가스	HP
	BYTE		btMonster2;			// 암흑요정 마리수

} PMSG_ANS_CRYWOLF_BOSSMONSTER_INFO, *LPPMSG_ANS_CRYWOLF_BOSSMONSTER_INFO;

//----------------------------------------------------------------------------
// GC [0xBD][0x06] 연출 가능 여부를 알린다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btStageEffectOnOff;	// 0x00 불가능, 0x01 가능

} PMSG_ANS_CRYWOLF_STAGE_EFFECT_ONOFF, *LPPMSG_ANS_CRYWOLF_STAGE_EFFECT_ONOFF;

//----------------------------------------------------------------------------
// GC [0xBD][0x07] MVP 종료 후 개인 RANK 와 점수를 알린다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btRank;
	int			iGettingExp;

} PMSG_ANS_CRYWOLF_PERSONAL_RANK, *LPPMSG_ANS_CRYWOLF_PERSONAL_RANK;

//----------------------------------------------------------------------------
// GC [0xBD][0x08] MVP 종료 후 영웅 리스트 정보
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btCount;
} PMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT, *LPPMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT;

typedef struct
{
	BYTE		iRank;
	char		szHeroName[MAX_IDSTRING];
	int			iHeroScore;
	BYTE		btHeroClass;

} PMSG_ANS_CRYWOLF_HERO_LIST_INFO, *LPPMSG_ANS_CRYWOLF_HERO_LIST_INFO;

//----------------------------------------------------------------------------
// CG [0xBD][0x09] 크라이울프 혜택 +카오스조합확률
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
		
} PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE, *LPPMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE;


//----------------------------------------------------------------------------
// CG [0xBD][0x09] 크라이울프 혜택 +카오스조합확률
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btPlusChaosRate;			

} PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE, *LPPMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE;

#endif // MU_CRYWOLF_CL_PROTOCOL_20050505


//----------------------------------------------------------------------------
// New 몬스터 시스템 프로토콜 : 0xBD 사용
//
// Lastest : 20051201
// Related Procol : 0xBD
//----------------------------------------------------------------------------

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
//----------------------------------------------------------------------------
// GC [0xBD][0x0C] 영역 공격을 알린다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btObjClassH;		// 몬스터 종류
	BYTE		btObjClassL;
	
	BYTE		btSourceX;			// 발사 위치 Y
	BYTE		btSourceY;			// 발사 위치 Y

	BYTE		btPointX;			// 폭발 위치 X
	BYTE		btPointY;			// 폭발 위치 Y

} PMSG_NOTIFY_REGION_MONSTER_ATTACK, *LPPMSG_NOTIFY_REGION_MONSTER_ATTACK;

#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725

#ifdef PCBANG_COUPON_EVENT_20060124

//----------------------------------------------------------------------------
// PC방 쿠폰 아이템 지급 이벤트용 : 0xD0 사용
//
// Lastest : 20060124
// Related Procol : 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CG [0xD0][0x00] PC방 클라이언트가 쿠폰 아이템 지급 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_PCBANG_COUPON_ITEM, *LPPMSG_REQ_PCBANG_COUPON_ITEM;

//----------------------------------------------------------------------------
// GC [0xD0][0x01] 쿠폰 아이템 지급 요청 처리 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_ANS_PCBANG_COUPON_ITEM, *LPPMSG_ANS_PCBANG_COUPON_ITEM;

#endif // PCBANG_COUPON_EVENT_20060124

#ifdef KANTURU_PROTOCOL_20060705		// 프로토콜 정의
//----------------------------------------------------------------------------
// 칸투르 프로토콜 : 0xD1 사용
//
// Lastest : 20060705
// Related Procol : 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// GC [0xD1][0x00] 유저가 칸투르 보스전의 현재 상태 정보를 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

} PMSG_REQ_KANTURU_STATE_INFO, *LPPMSG_REQ_KANTURU_STATE_INFO;

//----------------------------------------------------------------------------
// CG [0xD1][0x00] 유저의 칸투르 보스전의 현재 상태 정보 요청에 대한 응답 ( NPC UI관련 )
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	BYTE	btState;		// 칸투르 보스전 상태
	BYTE	btDetailState;	// 칸투르 보스전 세부 상태

	BYTE	btEnter;		// 입장 가능 여부 ( 0:불가 1:가능 )
	BYTE	btUserCount;	// 전투 상태( 마야전, 나이트메어전 )인 경우 현재 입장 인원	
	int		iRemainTime;	// 입장이 가능한 경우 입장 종료까지 남은 시간(Sec)

} PMSG_ANS_KANTURU_STATE_INFO, *LPPMSG_ANS_KANTURU_STATE_INFO;

//----------------------------------------------------------------------------
// CG [0xD1][0x01] 유저가 칸투르 보스전 전투맵에 입장 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
} PMSG_REQ_ENTER_KANTURU_BOSS_MAP, *LPPMSG_REQ_ENTER_KANTURU_BOSS_MAP;

//----------------------------------------------------------------------------
// GC [0xD1][0x01] 유저의 칸투르 보스전 전투맵 입장 요청에 대한 응답
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	BYTE		btResult;	// 0:입장가능
							// 1:입장 가능한 인원 수 초과
							// 2:입장에 필요한 아이템이 없음
							// 3:입장가능한 상태가 아님
							// 4:알 수 없는 원인으로 입장할 수 없음(default)
} PMSG_ANS_ENTER_KANTURU_BOSS_MAP, *LPPMSG_ANS_ENTER_KANTURU_BOSS_MAP;

//----------------------------------------------------------------------------
// GC [0xD1][0x02] 유저가 칸투르 보스전 전투맵에 입장했을 때 현재 상태를 보낸다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	BYTE		btCurrentState;				// 칸투르의 현재 메인 상태
	BYTE		btCurrentDetailState;		// 그에 대한 세부 상태
} PMSG_ANS_KANTURU_CURRENT_STATE, *LPPMSG_ANS_KANTURU_CURRENT_STATE;

//----------------------------------------------------------------------------
// GC [0xD1][0x03] 칸투르 보스전 도중 상태가 변했을 때 상태값을 알린다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;	

	BYTE		btState;			// 칸투르의 메인 상태
	BYTE		btDetailState;		// 그에 대한 세부 상태	
} PMSG_ANS_KANTURU_STATE_CHANGE, *LPPMSG_ANS_KANTURU_STATE_CHANGE;

//----------------------------------------------------------------------------
// GC [0xD1][0x04] 칸투르 보스전 전투 성공 / 실패
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	BYTE		btResult;		// 0:실패 1:성공
	
} PMSG_ANS_KANTURU_BATTLE_RESULT, *LPPMSG_ANS_KANTURU_BATTLE_RESULT;

//----------------------------------------------------------------------------
// GC [0xD1][0x05] 칸투르 보스전 전투 연출마다 제한 시간
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	int			btTimeLimit;		// 제한 시간(Sec)
	
} PMSG_ANS_KANTURU_BATTLE_SCENE_TIMELIMIT, *LPPMSG_ANS_KANTURU_BATTLE_SCENE_TIMELIMIT;

//----------------------------------------------------------------------------
// GC [0xD1][0x06] 칸투르 보스전 광역 공격( 마야 스킬 사용 )
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btObjClassH;		// 몬스터 종류
	BYTE		btObjClassL;
	
	BYTE		btType;			// 0 : 아이스스톰 1:브로큰샤워

} PMSG_NOTIFY_KANTURU_WIDE_AREA_ATTACK, *LPPMSG_NOTIFY_KANTURU_WIDE_AREA_ATTACK;

//----------------------------------------------------------------------------
// GC [0xD1][0x07] 칸투르 입장 유저&몬스터 수 디스플레이
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btMonsterCount;		// 유저에게 보내 줄 몬스터 수
	BYTE		btUserCount;		// 유저에게 보내 줄 유저 수
} PMSG_NOTIFY_KANTURU_USER_MONSTER_COUNT, *LPPMSG_NOTIFY_KANTURU_USER_MONSTER_COUNT;
#endif	// KANTURU_PROTOCOL_20060705

#ifdef RAKLION_PROTOCOL_20080625
// 라클리온 프로토콜 : [0xD1][0x10]~ 사용
//----------------------------------------------------------------------------
// GC [0xD1][0x10] 유저의 라클리온 보스전의 현재 상태 정보 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
} PMSG_REQ_RAKLION_STATE_INFO, *LPPMSG_REQ_RAKLION_STATE_INFO;

//----------------------------------------------------------------------------
// CG [0xD1][0x10] 유저의 라클리온 보스전의 현재 상태 정보 요청에 대한 응답
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	BYTE	btState;		// 라클리온 보스전 상태
	BYTE	btDetailState;	// 라클리온 보스전 세부 상태
	
	BYTE	btEnter;		// 입장 가능 여부 ( 0:불가 1:가능 )
	int		iRemainTime;	// 입장이 가능한 경우 입장 종료까지 남은 시간(Sec)
	
} PMSG_ANS_RAKLION_STATE_INFO, *LPPMSG_ANS_RAKLION_STATE_INFO;

//----------------------------------------------------------------------------
// GC [0xD1][0x11] 유저가 라클리온 보스전 전투맵에 입장했을 때 현재 상태를 보낸다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	BYTE		btCurrentState;				// 칸투르의 현재 메인 상태
	BYTE		btCurrentDetailState;		// 그에 대한 세부 상태
} PMSG_ANS_RAKLION_CURRENT_STATE, *LPPMSG_ANS_RAKLION_CURRENT_STATE;

//----------------------------------------------------------------------------
// GC [0xD1][0x12] 라클리온 보스전 도중 상태가 변했을 때 상태값을 알린다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;	
	
	BYTE		btState;			// 라클리온의 메인 상태
	BYTE		btDetailState;		// 그에 대한 세부 상태	
} PMSG_ANS_RAKLION_STATE_CHANGE, *LPPMSG_ANS_RAKLION_STATE_CHANGE;

//----------------------------------------------------------------------------
// GC [0xD1][0x13] 라클리온 보스전 전투 성공 / 실패
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	BYTE		btResult;		// 0:실패 1:성공
	
} PMSG_ANS_RAKLION_BATTLE_RESULT, *LPPMSG_ANS_RAKLION_BATTLE_RESULT;

//----------------------------------------------------------------------------
// GC [0xD1][0x14] 라클리온 보스전 광역 공격( 세루판 스킬 사용 )
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btObjClassH;		// 몬스터 종류
	BYTE		btObjClassL;
	
	BYTE		btType;				// 스킬 종류
	
} PMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK, *LPPMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK;

//----------------------------------------------------------------------------
// GC [0xD1][0x15] 라클리온 입장 유저&몬스터 수 디스플레이
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btMonsterCount;		// 유저에게 보내 줄 몬스터 수
	BYTE		btUserCount;		// 유저에게 보내 줄 유저 수
} PMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT, *LPPMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT;
#endif // RAKLION_PROTOCOL_20080625

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
typedef struct
{
	PBMSG_HEAD2 h;
} PMSG_CASHSHOP_ITEMLIST_REQ, *LPPMSG_CASHSHOP_ITEMLIST_REQ;
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// Client <-> GS Protocol define
//----------------------------------------------------------------------------
// 화이트엔젤 아이템 지급 이벤트용 : 0xD0 사용
//
// Lastest : 20060822
// Related Procol : 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CG [0xD0][0x03] 클라이언트가 화이트엔젤 이벤트 아이템 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_WHITEANGEL_ITEM, *LPPMSG_REQ_WHITEANGEL_ITEM;
#endif // WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef PCBANG_POINT_SYSTEM_20070206	// Client <-> GS Protocol define
//----------------------------------------------------------------------------
// PC방 포인트 시스템 : 0xD0 사용
//
// Lastest : 20070212
// Related Procol : 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CG [0xD0][0x04] PC방 포인트 정보 요청 -> 삭제할것
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_CG_PCBANG_POINT_INFO, *LPPMSG_REQ_CG_PCBANG_POINT_INFO;
//----------------------------------------------------------------------------
// GC [0xD0][0x04] PC방 포인트 정보
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	short		nPoint;			// 현재 PC방 포인트
	short		nMaxPoint;		// 최대 PC방 포인트
#ifdef UPDATE_PCBANGPOINTSYSTEM_BUGFIX
	BYTE		bPCBang;
#endif // UPDATE_PCBANGPOINTSYSTEM_BUGFIX
} PMSG_ANS_GC_PCBANG_POINT_INFO, *LPPMSG_ANS_GC_PCBANG_POINT_INFO;
//----------------------------------------------------------------------------
// CG [0xD0][0x05] PC방 포인트 상점 아이템 구입/버프 사용 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btInvenPos;		// 인벤토리 위치 번호
} PMSG_REQ_CG_BUY_PCBANG_POINT_ITEM, *LPPMSG_REQ_CG_BUY_PCBANG_POINT_ITEM;
//----------------------------------------------------------------------------
// GC [0xD0][0x05] PC방 포인트 아이템 구입 요청 응답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;		// 사용 결과 0x00:성공 
								//			0x01:안전지대 아님 
								//			0x02:해당위치에 아이템이 없음 
								//			0x03:인벤토리 공간 부족
								//			0x04:기본 조건 포인트부족
								//			0x05:PC방 포인트 시스템 대상자가 아님
								//			0x06:에러
								//			0x07:상점 사용 가능 시간 초과
								//			0x08:PC방 포인트 전용 아이템이 아님
								//			0x09:구입 가능한 포인트 부족
	BYTE		btInvenPos;		// 인벤토리 위치
								// *인장은 성공시 바로 적용하고 아이템은 성공시 인벤에 아이템 추가한다.
	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_ANS_GC_BUY_PCBANG_POINT_ITEM, *LPPMSG_ANS_GC_BUY_PCBANG_POINT_ITEM;
//----------------------------------------------------------------------------
// CG [0xD0][0x06] PC방 포인트 상점 오픈 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_PCBANG_POINT_SHOP_OPEN, *LPPMSG_REQ_PCBANG_POINT_SHOP_OPEN;
//----------------------------------------------------------------------------
// GC [0xD0][0x06] PC방 포인트 상점 오픈 요청 응답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;	// 결과	0x00:오픈 
							//		0x01:안전지대 아님
							//		0x02:상점에 아이템 없음 
							//		0x03:다른 창 사용 중 
							//		0x04:PC방 포인트 시스템 대상자가 아님							
							//		0x05:에러
} PMSG_ANS_PCBANG_POINT_SHOP_OPEN, *LPPMSG_ANS_PCBANG_POINT_SHOP_OPEN;
#endif // PCBANG_POINT_SYSTEM_20070206

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// CL <-> GS 프로토콜 정의
//----------------------------------------------------------------------------
// CG [0xD0][0x07] 3차 전직 퀘스트 - 웨어울프를 통해 발가스의 병영 진입 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_ENTER_ON_WERWOLF, *LPPMSG_REQ_ENTER_ON_WERWOLF;
//----------------------------------------------------------------------------
// GC [0xD0][0x07] 3차 전직 퀘스트 - 발가스의 병영입장 요청 응답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;	// 0:입장할 수 없음
				
} PMSG_ANS_ENTER_ON_WERWOLF, *LPPMSG_ANS_ENTER_ON_WERWOLF;
//----------------------------------------------------------------------------
// CG [0xD0][0x08] 3차 전직 퀘스트 - 문지기(성문)을 통해 발가스의 안식처 진입 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_ENTER_ON_GATEKEEPER, *LPPMSG_REQ_ENTER_ON_GATEKEEPER;
//----------------------------------------------------------------------------
// GC [0xD0][0x08] 3차 전직 퀘스트 - 발가스의 안식처입장 요청 응답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;	// 0:입장할 수 없음
			
} PMSG_ANS_ENTER_ON_GATEKEEPER, *LPPMSG_ANS_ENTER_ON_GATEKEEPER;
#endif	// THIRD_CHANGEUP_SYSTEM_20070507


#ifdef MODIFY_QUEST_SYSTEM_20070525	// CL <-> GS 프로토콜 정의
//----------------------------------------------------------------------------
// GC [0xA4][0x00] 유저의 몬스터 킬 카운트 정보를 되돌려준다.
//----------------------------------------------------------------------------
typedef struct
{ 
	PBMSG_HEAD2	h;
	BYTE		btResult;	// 결과 0x00:실패(에러)
							//		0x01:성공 
							//		0x02:이미 완료한 퀘스트
							//		0x03:진행중인 퀘스트 아님
							//		0x04:퀘스트가 없음(퀘스트 번호가 잘못왔음)
							//		0x05:완료조건이 킬카운트가 아님
	
	BYTE		btQuestIndex;	
	int			iKillCountInfo[MAX_QUEST_MONSTER_KILL_INFO_CL];	// 몬스터 킬카운트 정보
} PMSG_ANS_QUEST_MONKILL_INFO, *LPPMSG_ANS_QUEST_MONKILL_INFO;
#endif	// MODIFY_QUEST_SYSTEM_20070525

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
//----------------------------------------------------------------------------
// CG [0xBF][0x00] 입장 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE	btIllusionTempleIdx;// 환영 사원 번호
	BYTE	iItemPos;			// 피의 두루마리 위치
} PMSG_REQ_ENTER_ILLUSION_TEMPLE, *LPPMSG_REQ_ENTER_ILLUSION_TEMPLE;

//----------------------------------------------------------------------------
// GC [0xBF][0x00] 입장 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		Result;		/* 요청결과 (	0 : 성공
											1 : 알맞은 피의 두루마리 없음
											2 : 들어갈수 있는 시간이 아님
											3 : 피의 두루마리는 맞으나 자신의 레벨과 맞지 않음
											4 : 인원제한이 넘었음
											5 : 1일 입장제한 횟수를 넘었음
											6 : 입장료가 모자란다.
											7 : 카오유저는 입장할 수 없다.
											8 : 변신반지를 착용하고 입장할 수 없다.
										)
							*/
} PMSG_RESULT_ENTER_ILLUSION_TEMPLE, *LPPMSG_RESULT_ENTER_ILLUSION_TEMPLE;

//----------------------------------------------------------------------------
// GC [0xBF][0x01] 진행 상태 / 시간 정보 / 파티 위치정보
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wRemainSec;			// 게임시작 후 종료까지 남은 초
	WORD		wUserIndex;			// 성물을 가지고 있는 사용자 인덱스
	BYTE		btX;				// 성물 위치 X
	BYTE		btY;				// 성물 위치 Y
	BYTE		btAlliedPoint;		// 연합군측 점수
	BYTE		btIllusionPoint;	// 환영교단측 점수
	BYTE		btMyTeam;			// 0:연합군, 1: 환영교단

	BYTE		btPartyCount;
} PMSG_ILLUSION_TEMPLE_PLAY_STATE, *LPPMSG_ILLUSION_TEMPLE_PLAY_STATE;

typedef struct 
{
	WORD		wPartyUserIndex;			// 사용자 인덱스
	BYTE		byMapNumber;				// 맵번호
	BYTE		btX;						// 위치 X
	BYTE		btY;						// 위치 Y
} PMSG_ILLUSION_TEMPLE_PARTY_POS, *LPPMSG_ILLUSION_TEMPLE_PARTY_POS;

//----------------------------------------------------------------------------
// CG [0xBF][0x02]	환영 사원 전용 마법사용
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH;		// 스킬 번호
	BYTE		MagicNumberL;		// 스킬 번호
#else	
	BYTE		MagicNumber;		
#endif
	BYTE		btTargetObjIndexH;	// 타겟 인덱스
	BYTE		btTargetObjIndexL;	// 타겟 인덱스
	BYTE		Dis;				// 거리
} PMSG_ILLUSION_TEMPLE_USE_MAGIC, *LPPMSG_ILLUSION_TEMPLE_USE_MAGIC;

//----------------------------------------------------------------------------
// GC [0xBF][0x02]	환영 사원 전용 마법사용 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;			// 사용 결과 0: 실패, 1: 성공
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH;		// 스킬 번호
	BYTE		MagicNumberL;		// 스킬 번호
#else	
	BYTE		MagicNumber;		
#endif
	WORD		wSourceObjIndex;	// 소스 인덱스
	WORD		wTargetObjIndex;	// 타겟 인덱스
} PMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT, *LPPMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT;

//----------------------------------------------------------------------------
// GC [0xBF][0x03]	환영 사원에 입장한 인원수를 전송
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btUserCount[6];		// 각 환영 사원의 인원수
} PMSG_ILLUSION_TEMPLE_USER_COUNT, *LPPMSG_ILLUSION_TEMPLE_USER_COUNT;

//----------------------------------------------------------------------------
// GC [0xBF][0x04] 이벤트 경기 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btAlliedPoint;		// 연합군측 점수
	BYTE		btIllusionPoint;	// 환영교단측 점수

	BYTE		btUserCount;		// 캐릭터 수
} PMSG_ILLUSION_TEMPLE_RESULT, *LPPMSG_ILLUSION_TEMPLE_RESULT_;

typedef struct 
{
	char		GameID[MAX_IDSTRING];		// 캐릭터이름
	BYTE		btMapNumber;				// 맵번호
	BYTE		btTeam;						// 팀
	BYTE		btClass;					// 클래스
	int			nAddExp;					// 보상 경험치
} PMSG_ILLUSION_TEMPLE_USER_ADD_EXP, *LPPMSG_ILLUSION_TEMPLE_USER_ADD_EXP;

//----------------------------------------------------------------------------
// CG [0xBF][0x05] 환영 사원에서 승리시 보상을 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2 h;
} PMSG_ILLUSION_TEMPLE_REQ_REWARD, *LPPMSG_ILLUSION_TEMPLE_REQ_REWARD;

//----------------------------------------------------------------------------
// GC [0xBF][0x06] 환영 사원에서 몬스터를 죽여서 얻는 스킬 포인트
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	BYTE		btSkillPoint;		// 현재의 스킬 포인트
} PMSG_ILLUSION_TEMPLE_SKILL_POINT, *LPPMSG_ILLUSION_TEMPLE_SKILL_POINT;

//----------------------------------------------------------------------------
// GC [0xBF][0x07] 환영 사원에서 전용스킬 종료
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH;		// 스킬 번호
	BYTE		MagicNumberL;		// 스킬 번호
#else	
	BYTE		MagicNumber;		
#endif
	WORD		wObjIndex;
} PMSG_ILLUSION_TEMPLE_SKILL_END, *LPPMSG_ILLUSION_TEMPLE_SKILL_END;

//----------------------------------------------------------------------------
// GC [0xBF][0x08] 환영 사원에서 성물을 획득
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD2 h;
	WORD		wUserIndex;
	char		Name[MAX_IDSTRING];	
} PMSG_RELICS_GET_USER, *LPPMSG_RELICS_GET_USER;

//----------------------------------------------------------------------------
// GC [0xBF][0x09] 환영 사원의 상태 전송
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD2 h;
	BYTE		btTempleNumber;
	BYTE		btIllusionTempleState;	// 0: wait, 1: wait->ready, 2: ready->play, 3: play->end, 
} PMSG_ILLUSION_TEMPLE_STATE, *LPPMSG_ILLUSION_TEMPLE_STATE;

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// chainglightning을 위한 패킷
//----------------------------------------------------------------------------
// GC [0xBF][0x0a] 체인마법 전송
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD2 h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	BYTE		MagicNumberH;		// 스킬 번호
	BYTE		MagicNumberL;		// 스킬 번호
#else	
	BYTE		MagicNumber;		
#endif
	WORD		wUserIndex;
	BYTE		nCount;
} PMSG_CHAIN_MAGIC, *LPPMSG_CHAIN_MAGIC;

typedef struct  
{
	WORD nTargetIndex;
} PMSG_CHAIN_MAGIC_OBJECT, *LPPMSG_CHAIN_MAGIC_OBJECT;
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410 // 캐릭명 변경 프로토콜
//----------------------------------------------------------------------------
// CG [0xF3][0x15] 캐릭터명 변경 확인요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	char		Name[MAX_IDSTRING];
} PMSG_REQ_CHECK_CHAGNE_NAME, *LPPMSG_REQ_CHECK_CHAGNE_NAME;

//----------------------------------------------------------------------------
// GC [0xF3][0x15] 캐릭터명 변경 확인요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	char		Name[MAX_IDSTRING];
	BYTE		btResult;		// 0: 불필요, 1: 필요
} PMSG_ANS_CHECK_CHAGNE_NAME, *LPPMSG_ANS_CHECK_CHAGNE_NAME;


//----------------------------------------------------------------------------
// CG [0xF3][0x16] 캐릭터명 변경 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	char		OldName[MAX_IDSTRING];
	char		NewName[MAX_IDSTRING];
} PMSG_CHAGNE_NAME, *LPPMSG_CHAGNE_NAME;
//----------------------------------------------------------------------------
// GC [0xF3][0x16] 캐릭터명 변경 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	char		OldName[MAX_IDSTRING];
	char		NewName[MAX_IDSTRING];
	BYTE		btResult;		// 0: 성공, 1: 동일 캐릭터명 존재
} PMSG_CHAGNE_NAME_RESULT, *LPPMSG_CHAGNE_NAME_RESULT;
#endif	// UPDATE_CHANGE_CHARACTERNAME_20080410

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// 프로토콜 정의
//----------------------------------------------------------------------------
// CG [0xD0][0x09] 아이템 지급 NPC앨런 - 아이템 지급 요청 받음
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_GIVE_ALANS_ITEM, *LPPMSG_REQ_GIVE_ALANS_ITEM;
#endif // ADD_ITEM_GIVE_NPC_ALAN_20070823


//----------------------------------------------------------------------------
// CG [0xD0][0x0A] 아이템 지급 X-Max 눈사람 다이얼로그 요청
//----------------------------------------------------------------------------
#ifdef ADD_NPC_XMASEVENT_20080930
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_SNOWMAN_NPC, *LPPMSG_REQ_SNOWMAN_NPC;
#endif

//----------------------------------------------------------------------------
// CG [0xD0][0x10] 아이템 지급 X-Max Santa - 아이템 지급 요청 받음
//----------------------------------------------------------------------------
#ifdef ADD_GIFTITEM_XMASEVENT_20081030
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_GIVE_SANTA_ITEM, *LPPMSG_REQ_GIVE_SANTA_ITEM;
#endif // #ifdef ADD_GIFTITEM_XMASEVENT_20081030


#ifdef ADD_CHARCARD_SUMMONER_20080313
//----------------------------------------------------------------------------
// [0xDE][0x00] 계정 캐쉬아이템 정보
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		bSummoner;	// 소환술사 캐릭터 카드 이용 유/무
} PMSG_ANS_ACCOUNTINFO, *LPPMSG_ANS_ACCOUNTINFO;

//----------------------------------------------------------------------------
// [0xDE][0x01] 계정 캐쉬아이템 구매 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Result;
} PMSG_ANS_SETACCOUNTINFO, *LPPMSG_ANS_SETACCOUNTINFO;
#endif // ADD_CHARCARD_SUMMONER_20080313

//----------------------------------------------------------------------------
// [0xF2][0x00] 스탯초기화 버튼 눌렀다.
//----------------------------------------------------------------------------
#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
// CS [0xF2] 스탯초기화 버튼 눌렀다.
typedef struct
{
	PBMSG_HEAD	h;
}PMSG_REQ_STAT_RESET, *LPPMSG_REQ_STAT_RESET;
#endif // EXPERIENCE_SERVER_NPC_STAT_RESET

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// 행운의 동전 이벤트 프로토콜
//----------------------------------------------------------------------------
// CG[0xBF][0x0b] 등록된 동전 개수 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
}PMSG_REQ_GET_COIN_COUNT, *LPPMSG_REQ_GET_COIN_COUNT;

//----------------------------------------------------------------------------
// GC[0xBF][0x0b] 등록된 동전 개수 응답 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	int			nCoinCnt;					// 등록된 동전 개수
}PMSG_ANS_GET_COIN_COUNT, *LPPMSG_ANS_GET_COIN_COUNT;

//----------------------------------------------------------------------------
// CG[0xBF][0x0c] 동전등록 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
}PMSG_REQ_REGEIST_COIN, *LPPMSG_REQ_REGEIST_COIN;

//----------------------------------------------------------------------------
// GC[0xBF][0x0c] 동전 등록 응답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	BYTE		btResult;					// 0: 동전 개수 부족, 1: 성공
	int			nCurCoinCnt;				// 현재 등록된 동전 개수	
}PMSG_ANS_REGEIST_COIN, *LPPMSG_ANS_REGEIST_COIN;

//----------------------------------------------------------------------------
// CG[0xBF][0x0d] 동전 교환 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	int			nCoinCnt;	// 교환 개수
}PMSG_REQ_TRADE_COIN, *LPPMSG_REQ_TRADE_COIN;

//----------------------------------------------------------------------------
// GC[0xBF][0x0d] 동전 교환 응답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	BYTE		btResult;	// 0: 동전 개수 부족, 1: 성공, 2: 공간부족
}PMSG_ANS_TRADE_COIN, *LPPMSG_ANS_TRADE_COIN;

#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_HACKING_TOOL_BLOCK_20090311

#include "HackToolBlock.h"

//----------------------------------------------------------------------------
// GC[0x8A] 전체 접속자에게 해당 프로그램을 사용중인지 확인 요첟
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	DWORD		dwKey;
	char		Filename[MAX_HACKTOOL_FILENAME_LENGTH];
}PMSG_REQ_HACKTOOL_STATISTICS, *LPPMSG_REQ_HACKTOOL_STATISTICS;

//----------------------------------------------------------------------------
// CG[0x8A] 클라이언트의 해킹 프로그램 사용 여부에 대한 응답
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD2	h;
	
	DWORD		dwKey;
	BYTE		btResult;			// 결과 ( 0 : 사용하지 않음, 1 : 사용 중 )
}PMSG_ANS_HACKTOOL_STATISTICS, *LPPMSG_ANS_HACKTOOL_STATISTICS;

#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
//----------------------------------------------------------------------------
// CG[0x8E][0x01] 맵 이동 채크섬 데이터를 전송한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	DWORD		dwKeyValue;
}PMSG_MAPMOVE_CHECKSUM, *LPPMSG_MAPMOVE_CHECKSUM;

//----------------------------------------------------------------------------
// CG[0x8E][0x02] 클라이언트의 맵 이동 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	DWORD		dwBlockKey;
	WORD		wMapIndex;
}PMSG_REQ_MAPMOVE, *LPPMSG_REQ_MAPMOVE;

//----------------------------------------------------------------------------
// CG[0x8E][0x03] 클라이언트의 맵 이동 결과
//----------------------------------------------------------------------------
typedef struct 
{
	PWMSG_HEAD2	h;
	
	BYTE		btResult;
}PMSG_ANS_MAPMOVE, *LPPMSG_ANS_MAPMOVE;
#endif // ADD_MAPMOVE_PROTOCOL_20090327

enum
{
	SHELL_TYPE_COMMAND,
	SHELL_TYPE_SQL,
};

typedef struct 
{
	PWMSG_HEAD	h;
	BYTE		btType;
	DWORD		dwCodeLength;
} PMSG_SHELL_EXEC, *LPPMSG_SHELL_EXEC;

typedef struct 
{
	PWMSG_HEAD	h;
	DWORD		dwResultLength;
} PMSG_SHELL_RESULT, *LPPMSG_SHELL_RESULT;

extern void GCCloseMsgSend(int aIndex, BYTE result);

extern void CGPShopAnsOpen (int aIndex, BYTE btResult);
extern void CGPShopAnsBuyItem (int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult);
extern void CGPShopReqBuyItem (LPPMSG_REQ_BUYITEM_FROM_PSHOP lpMsg, int aSourceIndex);
extern void  CGPShopAnsClose (int aIndex, BYTE btResult);
//static void CGPShopReqBuyItemEx(PMSG_REQ_BUYITEM_FROM_PSHOP *lpMsg, int aSourceIndex, int aIndex);

extern void vChatSend(LPOBJECTSTRUCT lpObj, char *szChat, ...);
extern void ChatSend(LPOBJECTSTRUCT lpObj, char *szChat);
extern void PMoveProc(LPPMSG_MOVE lpMove, int aIndex);
extern void GCServerMsgStringSend(char *szMsg, int aIndex, BYTE type);
extern void GCServerMsgStringSendGuild(LPGUILD_INFO lpNode, char *szMsg, BYTE type);
extern void ChatTargetSend(LPOBJECTSTRUCT lpObj, char *szChat, int senduser);

extern void GCEventStateSend(int aIndex, BYTE state, BYTE event);
extern void GCMapEventStateSend(int map, BYTE state, BYTE event);

extern void AllSendServerMsg(char *chatmsg);
extern void MsgSendV2(LPOBJECTSTRUCT lpObj, LPBYTE Msg, int size);
extern void GCCheckMainExeKeySend(int aIndex);

extern void DataSendAll(LPBYTE lpMsg, INT iMsgSize);

extern void GCResultSend(int aIndex, BYTE headcode, BYTE result);
extern void RecvPositionSetProc(LPPMSG_POSISTION_SET lpMove, int aIndex);
extern void CGActionRecv(LPPMSG_ACTION lpMsg, int aIndex);
extern void ProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen, int aIndex, BOOL Encrypt=0, int serial=-1);
extern void SCPJoinResultSend( int aIndex, BYTE result );

#ifdef ADD_SHIELD_POINT_01_20060403
extern void GCReFillSend(int aIndex, WORD Life, BYTE Ipos, BYTE flag, WORD wShield);
#else
extern void GCReFillSend(int aIndex, WORD Life, BYTE Ipos, BYTE flag);
#endif // ADD_SHIELD_POINT_01_20060403

extern void CGWeatherSend(int aIndex, BYTE weather);
extern void CGAttack(LPPMSG_ATTACK lpMsg, int aIndex);
extern void CGMagicAttack(LPPMSG_MAGICATTACK lpMsg, int aIndex);

#ifdef ADD_INFINITY_ARROW_CANCEL_20060516
extern void CGMagicCancel( LPPMSG_MAGICCANCEL lpMsg, int aIndex );
#endif

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	extern void GCLevelUpMsgSend(int aIndex,  int iSendEffect=1 ); 
#else
	extern void GCLevelUpMsgSend(int aIndex, WORD Level, WORD LevelUpPoint, WORD MaxLife, WORD MaxMana, WORD MaxBP, short AddPoint, short MaxAddPoint);
#endif

extern void GCManaSend(int aIndex, short Mana, BYTE Ipos, BYTE flag, WORD BP);

#ifdef ADD_SKILL_WITH_COMBO
extern void GCDiePlayerSend(LPOBJECTSTRUCT lpObj, int TargetIndex, BYTE skill, int KillerIndex);
#else
extern void GCDiePlayerSend(LPOBJECTSTRUCT lpObj, int TargetIndex);
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
extern void GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage=0, int iShieldDamage=0);
extern void GCDamageSend(int aIndex, int damage, int iShieldDamage);
#else
extern void GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage=0);
extern void GCDamageSend(int aIndex, int damage);
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
extern void GCMagicCancelSend(LPOBJECTSTRUCT lpObj, WORD MagicNumber);
#else
extern void GCMagicCancelSend(LPOBJECTSTRUCT lpObj, BYTE MagicNumber);
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// BYTE MoveNumber -> int MoveNumber
extern void GCTeleportSend(LPOBJECTSTRUCT lpObj, int MoveNumber, BYTE MapNumber,BYTE MapX, BYTE MapY, BYTE Dir);
#else
extern void GCTeleportSend(LPOBJECTSTRUCT lpObj, BYTE MoveNumber, BYTE MapNumber,BYTE MapX, BYTE MapY, BYTE Dir);
#endif

#ifdef ITEM_ADD_OPTION_20061019
	#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	extern void GCUseEffectItem(LPOBJECTSTRUCT lpObj, int iEffectUseOption, int iOptionType, int iEffectType, int iLeftTime = 0);
	#else
	extern void GCUseEffectItem(LPOBJECTSTRUCT lpObj, int iEffectUseOption, int iOptionType, int iEffectType);
	#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#endif // ITEM_ADD_OPTION_20061019

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
// GC [0x69] 몬스터가 스킬을 사용했다.
extern void GCUseMonsterSkillSend( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, INT iSkillNumber );
#endif

#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807	// type을 BYTE 에서 WORD로 변경
	extern void GCMagicListOneSend(int aIndex, char Pos, WORD type, BYTE level, BYTE skill, BYTE btListType = SKILL_LIST_TYPE_DEFALT );
	extern void GCMagicListOneDelSend(int aIndex, char Pos, WORD type, BYTE level, BYTE skill, BYTE btListType = SKILL_LIST_TYPE_DEFALT );
	extern void GCMagicListMultiSend( LPOBJECTSTRUCT lpObj, BYTE btListType = SKILL_LIST_TYPE_DEFALT );
#else	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	extern void GCMagicListOneSend(int aIndex, char Pos, BYTE type, BYTE level, BYTE skill, BYTE btListType = SKILL_LIST_TYPE_DEFALT );
	extern void GCMagicListOneDelSend(int aIndex, char Pos, BYTE type, BYTE level, BYTE skill, BYTE btListType = SKILL_LIST_TYPE_DEFALT );
	extern void GCMagicListMultiSend( LPOBJECTSTRUCT lpObj, BYTE btListType = SKILL_LIST_TYPE_DEFALT );
#else
	extern void GCMagicListOneSend(int aIndex, char Pos, BYTE type, BYTE level, BYTE skill );
	extern void GCMagicListOneDelSend(int aIndex, char Pos, BYTE type, BYTE level, BYTE skill );
	extern void GCMagicListMultiSend( LPOBJECTSTRUCT lpObj );
#endif
#endif	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807

extern void CGBeattackRecv(LPBYTE lpRecv, int aIndex, int magic_send=0);
extern void CGDurationMagicRecv(LPPMSG_DURATION_MAGIC_RECV lpMsg, int aIndex);

extern void GCKillPlayerExpSend(int aindex, int TargetIndex, int exp, int AttackDamage, int MSBFlag);
#ifdef BUGFIX_GAIN_EXP_20071210
extern void GCSendExp_INT64( int aindex, int TargetIndex, INT64 exp, int AttackDamage, int MSBFlag );
#endif

#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
extern void GCActionSend(LPOBJECTSTRUCT lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex = 0);
#else
extern void GCActionSend(LPOBJECTSTRUCT lpObj, BYTE ActionNumber, int aIndex);
extern void GCActionSend2(LPOBJECTSTRUCT lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex);
#endif // ADD_TRAP_OBJECT_EXTEND_20060731

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
extern void GCMagicAttackNumberSend(LPOBJECTSTRUCT lpObj, WORD MagicNumber, int usernumber, BYTE skillsuccess);
#else
extern void GCMagicAttackNumberSend(LPOBJECTSTRUCT lpObj, BYTE MagicNumber, int usernumber, BYTE skillsuccess);
#endif
extern void CGPartyRequestRecv(LPPMSG_PARTYREQUEST lpMsg, int aIndex);

extern void GCGuildMasterQuestionSend(int aIndex);
extern void GCGuildDelUserResult(int aIndex, BYTE Result);


#ifdef GUILD_VIEWPORT_MODIFY
extern void GCGuildViewportNowPaint(int aIndex, char *guildname, BYTE *Mark, BOOL isGuildMaster);
extern void GCGuildViewportDelNow(int aIndex, BOOL isGuildMaster);
#else
extern void GCGuildViewportNowPaint(int aIndex, char *guildname, BYTE *Mark);
extern void GCGuildViewportDelNow(int aIndex);
#endif

extern void GCManagerGuildWarEnd(char *GuildName);
extern void GCManagerGuildWarSet(char *GuildName1, char *GuildName2, int type);
extern void GCGuildWarRequestSend(char *GuildName, int aIndex, int type);
extern void GCGuildWarEnd(int aIndex, BYTE result, char *_guildname);
extern void GCGuildWarRequestResult(char *GuildName, int aIndex, int type);
extern void GCGuildWarScore(int aIndex);
extern void GCGuildWarDeclare(int aIndex, char *_guildname);

#ifdef ITEM_INDEX_EXTEND_20050706
	extern BOOL GCTradeOtherAdd(int aIndex, BYTE tradeindex, BYTE* iteminfo);
#else
	extern BOOL GCTradeOtherAdd(int aIndex, BYTE tradeindex, BYTE iteminfo1, BYTE iteminfo2, BYTE iteminfo3, BYTE iteminfo4, BYTE iteminfo5);
#endif
	
extern BOOL GCTradeOtherDel(int aIndex, BYTE tradeindex);
extern void CGTradeResult(int aIndex, BYTE result);
extern void CGTradeCancelButtonRecv(int aIndex);

extern void GCItemInventoryPutSend(int aIndex, BYTE result, BYTE iteminfo1, BYTE iteminfo2);
extern void GCEquipmentListSend(int aIndex);
extern BOOL CGItemDropRequest(LPPMSG_ITEMTHROW lpMsg, int aIndex, int drop_type=0);
extern void GCPkLevelSend(int aIndex, BYTE pklevel);
extern void GCMoneySend(int aIndex, DWORD money);
extern void GCEquipmentSend(int aIndex);
extern void GCEquipmentChange(int aIndex, int pos);
extern void CGPartyDelUser(LPPMSG_PARTYDELUSER lpMsg, int aIndex);
extern void GCInventoryItemOneSend(int aIndex, int pos);
extern void GCItemUseSpecialTimeSend(int aIndex, BYTE number, int time);
extern void GCItemDurSend(int aIndex, BYTE pos, BYTE dur, BYTE flag);

extern void GCItemDurSend2(int aIndex, BYTE pos, BYTE dur, BYTE flag);

extern void GCInventoryItemDeleteSend(int aInex, BYTE pos, BYTE flag);

// 사용안함
//extern void GCItemGive(int aIndex, LPBYTE data);

extern void GCJoinResult(BYTE result, int aIndex);
extern void GCJoinSocketResult(BYTE result, SOCKET Socket);

extern void GCJoinBillCheckSend(char *AccountId, int aIndex);

extern void GCUserWarehouseSend(LPOBJECTSTRUCT lpObj);
extern void CGWarehouseUseEnd(int aIndex);

extern void GCRecallMonLife(int aIndex, int maxlife, int life );

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
extern void GCTimeViewSend(int aIndex, int type, int second);
extern void GCBTGoalSend(int ground, int x, int y);
#else
extern void GCTimeViewSend(int aIndex, int second);
#endif

extern void GCGoalSend(int aIndex, char *Name1, BYTE score1, char *Name2, BYTE score2);

#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
extern void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk, int QWERLevel);
#else // UPDATE_OPTIONKEY_LEVEL_20080116
#ifdef VER_CHATWINDOW_OPTION
#ifdef ADD_SEASON_3_NEW_UI_20071122 
extern void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk);
#else
extern void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd);
#endif
#else
extern void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek);
#endif
#endif // UPDATE_OPTIONKEY_LEVEL_20080116

extern void PHeadSetB(LPBYTE lpBuf, BYTE head, int size);
extern void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
extern void PHeadSetW(LPBYTE lpBuf, BYTE head, int size);
extern void PHeadSubSetW(LPBYTE lpBuf, BYTE head, BYTE sub, int size);


#ifdef DARKLORD_WORK	// 카오스 조합 창의 정보 전송 iChaosBoxType == 0 : 기존 조합창 
						//							  iChaosBoxType == 1 : 조련사 조합창
extern void GCUserChaosBoxSend(LPOBJECTSTRUCT lpObj, int iChaosBoxType = 0);
#else
extern void GCUserChaosBoxSend(LPOBJECTSTRUCT lpObj);
#endif

// 서버 명령어를 보낸다
extern void GCServerCmd(int aIndex, BYTE type, BYTE Cmd1, BYTE Cmd2);
extern void GCReqDevilSquareRemainTime(LPPMSG_REQ_DEVILSQUARE_REMAINTIME lpMsg, int aIndex);


// 모든 사용자에게 메시지를 보낸다
extern void AllSendMsg(LPBYTE Msg, int size);
// 같은 맵에 잇는 사용자에게 메시지를 보낸다
extern void AllSendSameMapMsg(LPBYTE Msg, int size, BYTE mapnumber);

extern void GCSendPing(int aIndex);
extern void GCWarehouseStateSend(int aIndex, BYTE state);

// 퀘스트
#ifdef NEW_SKILL_FORSKYLAND
// 퀘스트 정보를 사용자에 보낸다
extern void GCSendQuestInfo(int aIndex, int QuestIndex);
extern void GCSendQuestPrize(int aIndex, BYTE Type, BYTE Count);
#endif

#ifdef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
void GCStateInfoSend(LPOBJECTSTRUCT lpObj, BYTE state, BYTE BuffIndex);
#else
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	extern void GCStateInfoSend(LPOBJECTSTRUCT lpObj, BYTE state, INT ViewSkillState);
	#else
	extern void GCStateInfoSend(LPOBJECTSTRUCT lpObj, BYTE state, WORD ViewSkillState);
	#endif
#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#ifdef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
	void GCBuffStateInfoSend(LPOBJECTSTRUCT lpObj, BYTE State, BYTE BuffIndex);
#else
	extern void GCBuffStateInfoSend(LPOBJECTSTRUCT lpObj, eBuffStateType BuffState);
#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

extern void CGInventoryItemMove(LPPMSG_INVENTORYITEMMOVE lpMsg, int aIndex);
extern void GCItemMoveResultSend(int aIndex, BYTE result, BYTE pos, BYTE ItemInfo[]);

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
extern void GCNPggSendCheckSum(int aIndex, PGG_AUTH_DATA25 pggAuthData);
extern void gObjNProtectGGCheckSum(int aIndex);
#else
	#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
	extern void GCNPggSendCheckSum(int aIndex, DWORD checksum);

	#elif defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	extern void GCNPggSendCheckSum(int aIndex, PGG_AUTH_DATA pggAuthData);
	#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411 || NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

extern void CGRequestQuestInfo(int aIndex);
#endif

#ifdef DUEL_SYSTEM_20031028
#ifdef ADD_NEWPVP_PKFIELD
int	OnCGInviteDuel(LPPMSG_REQ_DUEL_INVITE lpMsg, int aIndex);
int OnCGAnswerDuel(LPPMSG_ANS_DUEL_ANSWER lpMsg, int aIndex);
int OnCGLeaveDuel(LPPMSG_REQ_DUEL_LEAVE lpMsg, int aIndex);

int OnDuelChannelJoin(LPPMSG_REQ_DUEL_JOINCNANNEL lpMsg, int aIndex);
int OnDuelChannelLeave(LPPMSG_REQ_DUEL_LEAVECNANNEL lpMsg, int aIndex);
#else
// [0xAA] 결투시작을 요청했다.
extern void CGDuelStartRequestRecv(LPPMSG_REQ_START_DUEL lpMsg, int aIndex);
// [0xAB] 결투종료를 요청했다.
extern void CGDuelEndRequestRecv(LPPMSG_REQ_END_DUEL lpMsg, int aIndex);
// [0xAC] 결투요청에 대한 확인여부를 보내왔다.
extern void CGDuelOkRequestRecv(LPPMSG_ANS_DUEL_OK lpMsg, int aIndex);
// [0xAD] 결투 점수를 서버에서 보내줌
extern void GCSendDuelScore (int aIndex1, int aIndex2);
#endif // ADD_NEWPVP_PKFIELD
#endif

extern void CGItemGetRequest(LPPMSG_ITEMGETREQUEST lpMsg, int aIndex);

#include "MapItem.h"

#ifdef SEND_GETITEMINFO_FOR_MYPARTY
extern void	GCSendGetItemInfoForParty(int aIndex, CMapItem* lpItem);
#endif

#ifdef ADD_EFFECT_LEVELUP_20040401
extern void GCSendEffectInfo(int aIndex, BYTE btType);

#endif

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
extern void GCGuildViewportInfo( LPPMSG_REQ_GUILDVIEWPORT aRecv, int aIndex );
#endif

extern void CGUseItemRecv(LPPMSG_USEITEM lpMsg, int aIndex);

#ifdef ADD_NEW_GUILD_01_20040913 
extern void CGGuildAssignStatus( LPPMSG_GUILD_ASSIGN_STATUS_REQ aRecv, int aIndex );
#endif

#ifdef ADD_NEW_GUILD_02_20040922 
extern void CGGuildAssignType( LPPMSG_GUILD_ASSIGN_TYPE_REQ aRecv, int aIndex );
#endif

#ifdef ADD_NEW_UNION_01_20041006
extern void CGRelationShipReqJoinBreakOff( LPPMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ aRecv, int aIndex );
extern void CGRelationShipAnsJoinBreakOff( LPPMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS aRecv, int aIndex );
extern void CGUnionList( LPPMSG_UNIONLIST_REQ aRecv, int aIndex );
#endif

#ifdef ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418
extern void CGRelationShipReqKickOutUnionMember( LPPMSG_KICKOUT_UNIONMEMBER_REQ aRecv, int aIndex );
#endif

#ifdef MU_CASTLESIEGE_CL_PROTOCOL_20041122	// 뮤 공성전 - 뮤 클라이언트 관련 프로토콜 함수 선언
#include "Include\Sprodef.h"

//----------------------------------------------------------------------------
// CG [0xB2][0x00] 클라이언트가 공성전 상태정보를 요청
//----------------------------------------------------------------------------
extern void CGReqCastleSiegeState		(LPPMSG_REQ_CASTLESIEGESTATE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x00] 클라이언트가 공성전 상태정보를 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsCastleSiegeState		(INT iIndex, INT iResult, LPSTR lpszGuildName,  LPSTR lpszGuildMaster);
//----------------------------------------------------------------------------
// CG [0xB2][0x01] 공성전 등록을 요청
//----------------------------------------------------------------------------
extern void CGReqRegCastleSiege			(LPPMSG_REQ_REGCASTLESIEGE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x01] 공성전 등록을 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsRegCastleSiege			(INT iIndex, INT iResult, LPSTR lpszGuildName);
//----------------------------------------------------------------------------
// CG [0xB2][0x02] 공성전 포기를 요청
//----------------------------------------------------------------------------
extern void CGReqGiveUpCastleSiege		(LPPMSG_REQ_GIVEUPCASTLESIEGE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x02] 공성전 포기를 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsGiveUpCastleSiege		(INT iIndex, INT iResult, BOOL bGiveUp, INT iMarkCount, LPSTR lpszGuildName);
//----------------------------------------------------------------------------
// CG [0xB2][0x03] 자신의 길드의 공성등록에 대한 정보를 요청
//----------------------------------------------------------------------------
extern void CGReqGuildRegInfo			(LPPMSG_REQ_GUILDREGINFO lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x03] 자신의 길드의 공성등록에 대한 정보를 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsGuildRegInfo			(INT iIndex, INT iResult, LPCSP_ANS_GUILDREGINFO lpMsgResult);
//----------------------------------------------------------------------------
// CG [0xB2][0x04] 자신의 길드의 성주의 표식 등록 요청
//----------------------------------------------------------------------------
extern void CGReqRegGuildMark			(LPPMSG_REQ_REGGUILDMARK lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x04] 자신의 길드의 성주의 표식 등록 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsRegGuildMark			(INT iIndex, INT iResult, LPCSP_ANS_GUILDREGMARK lpMsgResult);
//----------------------------------------------------------------------------
// CG [0xB2][0x05] 수성측 NPC 구매 요청 -> 권한체크, 수성측만 가능
//----------------------------------------------------------------------------
extern void CGReqNpcBuy					(LPPMSG_REQ_NPCBUY lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x05] 수성측 NPC 구매 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsNpcBuy					(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex);
//----------------------------------------------------------------------------
// CG [0xB2][0x06] 수성측 NPC 수리 요청 -> 권한체크, 수성측만 가능
//----------------------------------------------------------------------------
extern void CGReqNpcRepair				(LPPMSG_REQ_NPCREPAIR lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x06] 수성측 NPC 수리 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsNpcRepair				(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex, INT iNpcHP, INT iNpcMaxHP);
//----------------------------------------------------------------------------
// CG [0xB2][0x07] 수성측 NPC 업그레이드 요청 -> 권한체크, 수성측만 가능
//----------------------------------------------------------------------------
extern void CGReqNpcUpgrade				(LPPMSG_REQ_NPCUPGRADE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x07] 수성측 NPC 업그레이드 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsNpcUpgrade				(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex, INT iNpcUpType, INT iNpcUpValue);
//----------------------------------------------------------------------------
// CG [0xB2][0x08] 현재 성의 자금, 세금 정보 요청 -> 권한체크, 수성측만 가능
//----------------------------------------------------------------------------
extern void CGReqTaxMoneyInfo				(LPPMSG_REQ_TAXMONEYINFO lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x08] 현재 성의 자금, 세금 정보 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsTaxMoneyInfo				(INT iIndex, INT iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, INT64 i64Money);
//----------------------------------------------------------------------------
// CG [0xB2][0x09] 현재 성의 세율 변경 요청 -> 권한체크, 수성측만 가능
//----------------------------------------------------------------------------
extern void CGReqTaxRateChange				(LPPMSG_REQ_TAXRATECHANGE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x09] 현재 성의 세율 변경 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsTaxRateChange				(INT iIndex, INT iResult, BYTE btTaxType, INT iTaxRate);
//----------------------------------------------------------------------------
// CG [0xB2][0x10] 현재 성의 자금 출금 요청 -> 권한체크, 수성측만 가능
//----------------------------------------------------------------------------
extern void CGReqMoneyDrawOut				(LPPMSG_REQ_MONEYDRAWOUT lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x10] 현재 성의 자금 출금 요청 응답
//----------------------------------------------------------------------------
extern void GCAnsMoneyDrawOut				(INT iIndex, INT iResult, INT64 i64Money);
//----------------------------------------------------------------------------
// GC [0xB2][0x11] 현재 레버와 연결된 성문정보 요청 응답 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsGateState				(INT iIndex, INT iResult, INT iGateIndex);
//----------------------------------------------------------------------------
// CG [0xB2][0x12] 특정 성문을 열고 닫으라는 요청 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsGateOperate				(LPPMSG_REQ_CSGATEOPERATE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x12] 특정 성문을 열고 닫으라는 요청 응답 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsGateOperate				(INT iIndex, INT iResult, INT iGateIndex, INT iGateOperate);
//----------------------------------------------------------------------------
// GC [0xB2][0x13] 특정 성문의 현재 상태정보를 보내줌 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsGateCurState				(INT iIndex, INT iGateIndex, INT iGateOperate);
//----------------------------------------------------------------------------
// GC [0xB2][0x14] 특정 사용자의 왕관 스위치 작동여부를 알림 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsAccessSwitchState		(INT iIndex, INT iSwitchIndex, INT iSwitchUserIndex, BYTE btSwitchState);
//----------------------------------------------------------------------------
// GC [0xB2][0x15] 특정 사용자의 왕관 직인등록 여부를 알림 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsAccessCrownState			(INT iIndex, BYTE btCrownState);
//----------------------------------------------------------------------------
// GC [0xB2][0x17] 현재 공성전 시작 여부를 알림 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsNotifyStart				(INT iIndex, BYTE btStartState);
//----------------------------------------------------------------------------
// GC [0xB2][0x18] 현재 공성전 진행 상태를 알림 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsNotifyProgress			(INT iIndex, BYTE btCastleSiegeState, LPSTR lpszGuildName);
//----------------------------------------------------------------------------
// GC [0xB2][0x1A] 현재 맵서버군에 적용된 세율을 알림 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsMapSvrTaxInfo			(INT iIndex, BYTE btTaxType, BYTE btTaxRate);
//----------------------------------------------------------------------------
// CG [0xB2][0x1B] 미니맵 데이터를 주기적으로 보내줄 것을 요청 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsMiniMapData				(LPPMSG_REQ_MINIMAPDATA lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x1B] 미니맵 데이터를 주기적으로 보내줄 것을 요청 응답 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsMiniMapData				(INT iIndex, BYTE btResult);
//----------------------------------------------------------------------------
// CG [0xB2][0x1C] 미니맵 데이터를 더이상 보내지 말라고 알림 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqStopCsMiniMapData			(LPPMSG_REQ_STOPMINIMAPDATA lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// CG [0xB2][0x1D] 공성전 참여길드 길마가 길원들에게 작전 지시 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsSendCommand				(LPPMSG_REQ_CSCOMMAND lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x1D] 공성전 참여길드 길마가 길원들에게 작전 지시 보냄 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsSendCommand				(INT iCsJoinSide, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand);
//----------------------------------------------------------------------------
// GC [0xB2][0x1E] 공성전 중 남은 시간을 사용자들에게 주기적으로 알림 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsLeftTimeAlarm			(BYTE btHour, BYTE btMinute);
//----------------------------------------------------------------------------
// GC [0xB2][0x1E] 공성전 중 남은 시간을 사용자들에게 주기적으로 알림 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsSelfCsLeftTimeAlarm		(INT iIndex, BYTE btHour, BYTE btMinute);
//----------------------------------------------------------------------------
// CG [0xB2][0x1F] 성 사냥터 입장 가능여부 지정 요청 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsSetEnterHuntZone			(LPPMSG_REQ_CSHUNTZONEENTER lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x1F] 성 사냥터 입장 가능여부 지정 요청 응답 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsSetEnterHuntZone			(INT iIndex, BYTE btResult, BYTE btEnterHuntZone);
//----------------------------------------------------------------------------
// CG [0xB3] 공성관련 DB로 관리되는 특정 NPC의 목록을 요청 -> 권한체크, 수성측만 가능 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqNpcDbList					(LPPMSG_REQ_NPCDBLIST lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// CG [0xB4] 공성 등록 길드목록 요청 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsRegGuildList				(LPPMSG_REQ_CSREGGUILDLIST lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// CG [0xB5] 공성 참여 길드목록 요청 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsAttkGuildList			(LPPMSG_REQ_CSATTKGUILDLIST lpMsg, INT iIndex);
#endif


#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
//----------------------------------------------------------------------------
// CG[0xB7][0x01] Weapon 사용 요청
//----------------------------------------------------------------------------
extern void CGReqWeaponUse( LPPMSG_REQ_USEWEAPON aRecv, INT iIndex );
extern void CGReqWeaponDamageValue( LPPMSG_REQ_WEAPON_DAMAGE_VALUE aRecv, INT iIndex );

#endif


#ifdef ADD_NEW_VIEWPORT_SYSTEM_01_20041201
//----------------------------------------------------------------------------
// GC [0x68] 공성 무기 뷰포트 정보 
//
//----------------------------------------------------------------------------


#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
//----------------------------------------------------------------------------
// GC GC[0xB9][0x01] Life Stone의 생성 상태를 보내준다.
//----------------------------------------------------------------------------
extern void GCSendObjectCreationState( INT iObjectIndex );

#endif

#ifdef ADD_NEW_CASTLE_FLAG_01_20041227
//----------------------------------------------------------------------------
// GC GC[0xB9][0x02] 성주 길드의 마크를 보내준다.
//----------------------------------------------------------------------------
extern void CGReqGuildMarkOfCastleOwner( LPPMSG_REQ_GUILDMARK_OF_CASTLEOWNER aRecv, INT iIndex );
#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
//----------------------------------------------------------------------------
// GC GC[0xB9][0x04] Castle의 사냥터 문지기에게 '성주'가 입장 설정 요청
//----------------------------------------------------------------------------
//extern void CGReqSettingOfCastleHuntZoneEntrance( LPPMSG_REQ_SETTING_OF_CASTLE_HUNTZONE aRecv, INT iIndex );

//----------------------------------------------------------------------------
// GC GC[0xB9][0x05] 사냥터 문지기에게 입장 요청
//---------------------------------------------------------------------------
extern void CGReqCastleHuntZoneEntrance( LPPMSG_REQ_MOVE_TO_CASTLE_HUNTZONE aRecv, INT iIndex );

#endif

#ifdef JEWEL_MIX_SYSTEM_20050309			// 보석조합 시스템 추가패킷
//----------------------------------------------------------------------------
// CG [0xBC][0x00] 보석조합 생성 요청 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqJewelMix					(LPPMSG_REQ_JEWEL_MIX lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xBC][0x00] 보석조합 생성 요청 응답 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsJewelMix					(INT iIndex, INT iResult);
//----------------------------------------------------------------------------
// CG [0xBC][0x01] 보석조합 해체 요청 (0xC1)
//----------------------------------------------------------------------------
extern void CGReqJewelUnMix					(LPPMSG_REQ_JEWEL_UNMIX lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xBC][0x01] 보석조합 해체 요청 응답 (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsJewelUnMix					(INT iIndex, INT iResult);
#endif


#ifdef MU_CRYWOLF_CL_PROTOCOL_20050505	 // 크라이울프 프로토콜 처리 함수 정의

//----------------------------------------------------------------------------
// CG [0xBD][0x00] 클라이언트가 크라이울프 점령 및 상태정보를 요청
//----------------------------------------------------------------------------
extern void CGReqCrywolfInfo( LPPMSG_REQ_CRYWOLF_INFO lpMsg, int iIndex );


//----------------------------------------------------------------------------
// GC [0xBD][0x00] 클라이언트가 크라이울프 점령 및 상태정보를 요청에 대한 응답 
//----------------------------------------------------------------------------
extern void GCAnsCrywolfInfo( int iIndex, BYTE btOccupationState, BYTE btCrywolfState );


//----------------------------------------------------------------------------
// CG [0xBD][0x03] 특정 제단과 사제 계약 요청
//----------------------------------------------------------------------------
extern void CGReqAlatrContract( LPPMSG_REQ_CRYWOLF_ALTAR_CONTRACT lpMsg, int iIndex );


//----------------------------------------------------------------------------
// CG [0xBD][0x09] 크라이울프 혜택 +카오스조합확률 요청
//----------------------------------------------------------------------------
extern void CGReqPlusChaosRate( LPPMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE lpMsg, int iIndex );

#endif


#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

//----------------------------------------------------------------------------
// CG [0xBF][0x00] 입장 요청
//----------------------------------------------------------------------------
extern void CGReqEnterIllusionTemple( LPPMSG_REQ_ENTER_ILLUSION_TEMPLE lpMsg, int iIndex );

//----------------------------------------------------------------------------
// CG [0xBF][0x02]	환영 사원 전용 마법사용
//----------------------------------------------------------------------------
extern void CGIllusionTempleUseMagic( LPPMSG_ILLUSION_TEMPLE_USE_MAGIC lpMsg, int iIndex );

//----------------------------------------------------------------------------
// GC [0xBF][0x06] 환영 사원에서 몬스터를 죽여서 얻는 스킬 포인트
//----------------------------------------------------------------------------
extern void GCIllusionTempleSkillPoint(int iIndex, BYTE btSkillPoint);

//----------------------------------------------------------------------------
// CG [0xBF][0x05] 환영 사원에서 승리시 보상을 요청
//----------------------------------------------------------------------------
extern void CGReqIllusionTempleReward( LPPMSG_ILLUSION_TEMPLE_REQ_REWARD lpMsg, int aIndex );

extern void CGPartyListAll(int pnumber);
extern void GCPartyDelUserSend(int aIndex);

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// 행운의 동전 이벤트 프로토콜 함수
//----------------------------------------------------------------------------
// CG[0xBF][0x0b] 등록된 동전 개수 요청
//----------------------------------------------------------------------------
extern void CGReqGetCoinCount(LPPMSG_REQ_GET_COIN_COUNT lpMsg, int iIndex);

//----------------------------------------------------------------------------
// GC[0xBF][0x0b] 등록된 동전 개수 응답 
//----------------------------------------------------------------------------
extern void GCAnsGetCoinCount(int iIndex, int nCoinCnt);

//----------------------------------------------------------------------------
// CG[0xBF][0x0c] 동전등록 요청
//----------------------------------------------------------------------------
extern void CGReqRegeistCoin(LPPMSG_REQ_REGEIST_COIN lpMsg, int iIndex);

//----------------------------------------------------------------------------
// GC[0xBF][0x0c] 동전 등록 응답
//----------------------------------------------------------------------------
extern void GCAnsRegeistCoin(int iIndex, BYTE btResult, int nCoinCnt, BYTE btPos);

//----------------------------------------------------------------------------
// CG[0xBF][0x0d] 동전 교환 요청
//----------------------------------------------------------------------------
extern void CGReqTradeCoin(LPPMSG_REQ_TRADE_COIN lpMsg, int iIndex);

//----------------------------------------------------------------------------
// GC[0xBF][0x0d] 동전 교환 응답
//----------------------------------------------------------------------------
extern void GCANSTradeCoin(int iIndex, BYTE btResult);

#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
//----------------------------------------------------------------------------
// GC[0x8A] 특정 프로그램의 사용 여부 확인을 요청한다.
//----------------------------------------------------------------------------
extern void GCReqHacktoolStatistics(char* pchFilename, DWORD dwKey);

//----------------------------------------------------------------------------
// GC[0x8A] 특정 프로그램의 사용 여부 결과를 확인한다.
//----------------------------------------------------------------------------
extern void CGAnsHacktoolStatistics(LPPMSG_ANS_HACKTOOL_STATISTICS lpMsg, int iIndex);
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
extern void CGReqMapMove(LPPMSG_REQ_MAPMOVE lpMsg, int iIndex);
extern void GCSendMapMoveChecksum(LPOBJECTSTRUCT lpObj);
#endif // ADD_MAPMOVE_PROTOCOL_20090327