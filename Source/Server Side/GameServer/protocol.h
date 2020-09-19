#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__



#include "Include\\GMProtocol.h"
#include "include/ProDef.h"

#include <WinSock2.h>

#define PMHC_MAPJOIN				0xF3	// ����ڰ� �ʿ� �����Ҷ� ������ ���� ������ ������.
#define PMHC_VIEWPORTCREATE			0x12	// ����Ʈ �ȿ� ���Դ�
#define PMHC_VIEWPORTMONSTERCREATE	0x13	// ����Ʈ �ȿ� ���Ͱ� ���Դ�.
#define PMHC_VIEWPORTDESTROY		0x14	// ����Ʈ���� �������.

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
// [0x03] EXE ���� üũ�� ���� ��ȣȭ �ϴµ�.. 1���� Ű�� �Ѱ��ش�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		m_wKey;	// 0 - 1023 ������ ���� ��ȣȭ�Ѱ�
} PMSG_CHECK_MAINEXE;


//----------------------------------------------------------------------------
// 1���� ���� Key�� �Բ� Ŭ���̾�Ʈ EXE ���� üũ���� �����´�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	DWORD		m_dwKey;	// 0 - 1023 ������ ���� ��ȣȭ�Ѱ�
} PMSG_CHECK_MAINEXE_RESULT, *LPPMSG_CHECK_MAINEXE_RESULT;


//----------------------------------------------------------------------------
// CS UDP [0x01] ���� ������ ������ ���� ���� ����
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


#ifdef GAMESERVER_INFO_EXTENDED_20040625	// ���Ӽ����� ���� Ȯ��� �������� �޴´�.

//----------------------------------------------------------------------------
// CS UDP [0x03] ���Ӽ����� ������ ���Ӽ����� Ȯ��� ������ (PK����, ��������, Ŭ���̾�Ʈ����)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	int			iServerCode;		// �����ڵ�

	BYTE		btIsPK;				// PK ���� ����
	char		GsVersion[32];		// ���Ӽ��� ����
	char		ClVersion[32];		// Ŭ���̾�Ʈ ����
} PMSG_GAMESERVER_EXTINFO, *LPPMSG_GAMESERVER_EXTINFO;

#endif

//----------------------------------------------------------------------------
// [0x0C] ������ ���� �޴� �޽��� ��ȣ
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		MsgNumber;
} PMSG_SERVERMSG, *LPPMSG_SERVERMSG;

//----------------------------------------------------------------------------
// [0x0D] �������� 
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
// [0x0E] ����� ���� üũ �˸�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	DWORD	    Time;
	//BYTE		BootsLevel;
	WORD		AttackSpeed;	// �߰� 
	WORD		MagicSpeed;		// �߰�
} PMSG_CLIENTTIME, *LPPMSG_CLIENTTIME;





//----------------------------------------------------------------------------
// [0xF1][0x02] Ŭ���̾�Ʈ�� ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		SubCode;
	BYTE		Flag;
} PMSG_CLIENTCLOSE, *LPPMSG_CLIENTCLOSE;


//----------------------------------------------------------------------------
// [0xF1][0x03] Ŭ���̾�Ʈ�� ������ �ش� �ڵ带 ������
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
// [0xF1][0x02] Ŭ���̾�Ʈ�� ���� ���� ��û
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
// [0xF1][0x03] ��� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	char		Id[MAX_IDSTRING];
} PMSG_PASSQUESTION_QUERY, *LPPMSG_PASSQUESTION_QUERY;

//----------------------------------------------------------------------------
// [0xF1][0x03] ��� ���� �亯
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Result;
	char		Question[MAX_QUESTIONSTR];
} PMSG_PASSQUESTION_QUERY_RESULT, *LPPMSG_PASSQUESTION_QUERY_RESULT;

//----------------------------------------------------------------------------
// [0xF1][0x04] ��й�ȣ ���� ��û�� ���� ���� Ŭ���̾�Ʈ�� ������ ������.
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
// [0xF1][0x04] ��й�ȣ ���� ��û�� ���� ���.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Result;
	char		Pass[MAX_IDSTRING];
} PMSG_PASSQUERY_RESULT, *LPPMSG_PASSQUERY_RESULT;


//----------------------------------------------------------------------------
// [0xF1][0x05] ��й�ȣ ���� ��û�� �Դ�.
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
// [0xF1][0x05] ��й�ȣ ���� ��û�� �Դ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Result;
} PMSG_PASSCHANGE_RESULT, *LPPMSG_PASSCHANGE_RESULT;

//----------------------------------------------------------------------------
// [0xF3][0x00] ĳ���� ����Ʈ ī��Ʈ
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE subcode;
#ifdef DARKLORD_WORK
	BYTE	MaxClass;			// ����� �ִ� Ŭ������ (�⺻ : 2 ���˻� :3 ��ũ�ε� :4)
#endif
#ifdef CHARACTER_MOVE_20040810
	BYTE	MoveCnt;
#endif
	BYTE Count;
} PMSG_CHARLISTCOUNT, *LPPMSG_CHARLISTCOUNT;

//----------------------------------------------------------------------------
// [0xF3][0x00] ĳ���� ����(�� �������� ī���� ��ŭ �ݺ���)
//----------------------------------------------------------------------------
typedef struct
{
	BYTE Index;
	char Name[MAX_IDSTRING];
	WORD Level;
	BYTE CtlCode;			
	
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE CharSet[MAX_PREVIEWCHARSET+9];		// ĳ���� ��Ʈ (������)
#else
#ifdef DARKLORD_WORK	
	BYTE CharSet[MAX_PREVIEWCHARSET+4];		// ĳ���� ��Ʈ (������)
#else
	BYTE CharSet[MAX_PREVIEWCHARSET+2];		// ĳ���� ��Ʈ (������)
#endif
#endif


	
#ifdef ADD_CHARACTERLIST_GUILD_STATUS_01_20050126
	BYTE btGuildStatus;
#endif
	
} PMSG_CHARLIST, * LPPMSG_CHARLIST;
/* 
BYTE CharSet[MAX_PREVIEWCHARSET+1];	// ĳ���� ��Ʈ (������)
Class �� ������ +1 �Ȱ��� �޸� ī�Ǹ� �ϱ� ���ؼ���.
*/

//----------------------------------------------------------------------------
// [0xF3][0x01] Ŭ���̾�Ʈ�� ĳ���� ����� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD h;
	BYTE	subcode;
	char    Name[MAX_IDSTRING];	
	BYTE    ClassSkin;			// ����/��Ų���	
} PMSG_CHARCREATE, *LPPMSG_CHARCREATE;

//----------------------------------------------------------------------------
// [0xF3][0x01] ĳ���� ����� ��� 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD h;
	BYTE	subcode;
	BYTE    Result;
	BYTE	Name[MAX_IDSTRING];
	BYTE	pos;
#ifdef DARKLORD_WORK
	WORD	Level;			// ���� ���� �߰�
#endif	

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	BYTE	Class;
#else
	#ifdef ADD_CLASSINFO_ON_CHARCTER_CREATION_20041224
		BYTE	Class;
	#endif
#endif

	BYTE    Equipment[24];	// ĳ���Ͱ� �԰��ִ� �͵�
} PMSG_CHARCREATERESULT, *LPPMSG_CHARCREATERESULT;

//----------------------------------------------------------------------------
// [0xF3][0x02] ĳ���� ���� ��û
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
// [0xF3][0x03] ������ ĳ���ͷ� �� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	char		Name[MAX_IDSTRING];
} PMSG_CHARMAPJOIN, *LPPMSG_CHARMAPJOIN;

//----------------------------------------------------------------------------
// [0xF3][0x03] ������ ĳ���ͷ� �� ���� ��û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD  h;	
	BYTE		subcode;

	BYTE		MapX;
	BYTE		MapY;
	BYTE		MapNumber;
	
	BYTE		Dir;
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// �����ͷ��� ����ġ�� ���� Ÿ���� �ٲ۴�. DWORD->INT64
	BYTE		btExp1;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 1)
	BYTE		btExp2;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 2)
	BYTE		btExp3;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 3)
	BYTE		btExp4;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 4)
	BYTE		btExp5;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 5)
	BYTE		btExp6;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 6)
	BYTE		btExp7;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 7)
	BYTE		btExp8;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 8)

	BYTE		btNextExp1;				// ���� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 1)
	BYTE		btNextExp2;				// ���� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 2)
	BYTE		btNextExp3;				// ���� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 3)
	BYTE		btNextExp4;				// ���� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 4)
	BYTE		btNextExp5;				// ���� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 5)
	BYTE		btNextExp6;				// ���� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 6)
	BYTE		btNextExp7;				// ���� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 7)
	BYTE		btNextExp8;				// ���� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 8)
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
	WORD		Leadership;			// ���
#endif
	
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	WORD		wMinusPoint;		// ���� ���� ����Ʈ
	WORD		wMaxMinusPoint;		// �ִ� ���� ����Ʈ
#endif

} PMSG_CHARMAPJOINRESULT, *LPPMSG_CHARMAPJOINRESULT;

//----------------------------------------------------------------------------
// [0xF3][0x04] �׾��ٰ� �ٽ� �����ɶ� �޽���
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
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// �����ͷ��� ����ġ�� ���� Ÿ���� �ٲ۴�. DWORD->INT64
	BYTE		btExp1;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 1)
	BYTE		btExp2;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 2)
	BYTE		btExp3;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 3)
	BYTE		btExp4;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 4)
	BYTE		btExp5;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 5)
	BYTE		btExp6;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 6)
	BYTE		btExp7;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 7)
	BYTE		btExp8;					// ����ġ (INT64�� �ֻ��� ����Ʈ ���� 8)
#else
	DWORD		Exp;
#endif
	DWORD		Money;
} PMSG_CHARREGEN, *LPPMSG_CHARREGEN;


//----------------------------------------------------------------------------
// [0xF3][0x05] ������ �������� �ߴ�.
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
// [0xF3][0x06] ������ ������ ����Ʈ�� �ϳ� �������״�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE subcode;

	BYTE		Type;	// 0 : �� 1:��ø�� 2:ü�� 3:������
} PMSG_LVPOINTADD, *LPPMSG_LVPOINTADD;

//----------------------------------------------------------------------------
// [0xF3][0x06] ������ ������ ����Ʈ�� �ϳ� ���� �Ѵٰ� �˸��� ����� �˸���.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE subcode;

	BYTE	ResultType;	// 4Bit:Result,  4Bit:Type
	WORD		MaxLifeAndMana;	// type�� ü���̳� �������� �����Ǿ��ٸ� ���� ���� �˷��ش�.
#ifdef ADD_SHIELD_POINT_01_20060403
	WORD		wMaxShield;
#endif

	WORD		MaxBP;
} PMSG_LVPOINTADDRESULT, *LPPMSG_LVPOINTADDRESULT;

//----------------------------------------------------------------------------
// [0xF3][0x07] Damage ���� 
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
// [0xF3][0x08] PKLevel ����
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
// [0xF3][0x10] ĳ������ �κ��丮�� ����Ǿ��ִ� �����۵�..
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
// [0xF3][0x11] ���� ����Ʈ
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

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// PMSG_MAGICLIST ��Ŷ����
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
// [0xF3][0x13] ĳ���� ��� ����Ʈ
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

// [0xF3][0x14] �κ��丮 ������ ������ ������ ������ ������.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Pos;

	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_INVENTORYITEMMODIFY, *LPPMSG_INVENTORYITEMMODIFY;

// [0xF3][0x20] ��ȯ�� ������ ü���� ������.
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
	// GS==>CS [0xF3][0x22] ��� �ð� 
	typedef struct
	{
		PBMSG_HEAD	h;
		BYTE		subcode;
		BYTE		MatchType;		// 0: ��Ʋ��Ŀ 1: ����� 2: ����
		WORD		MatchTime;		// ���ð� : ����(��) - ���۰��� 1���� ���� 
	} PMSG_MATCH_TIMEVIEW, *LPPMSG_MATCH_TIMEVIEW;

	// GS==>CS [0xF3][0x24] ��� ���
	typedef struct
	{
		PBMSG_HEAD	h;
		BYTE		subcode;

		BYTE		MatchType;		// 0: ��Ʋ��Ŀ 1: ����� 2: ����
									// ������ ��� MatchTeamName1�� �ش� ĳ������ �̸�
									// #define MAX_IDSTRING 10	: ���� ���ڿ� NULLó�� ����
									// ���� �׻� 2�� �����ϱ� ������ ���� ���� ���� ����
		
		char		MatchTeamName1[MAX_IDSTRING];		// ���̸�1(������ ��� ĳ�����̸�1)
		WORD		Score1;								// ����1

		char		MatchTeamName2[MAX_IDSTRING];
		WORD		Score2;		
	} PMSG_MATCH_RESULT, *LPPMSG_MATCH_RESULT;

	// GS==>CS [0xF3][0x25] ����� �־���.
	typedef struct
	{
		PBMSG_HEAD	h;
		BYTE		subcode;

		BYTE		X;			// ����ġ
		BYTE		Y;		
	} PMSG_MATCH_GOAL, *LPPMSG_MATCH_GOAL;
#else
	// [0xF3][0x22] �ð��� ǥ���Ѵ�. (���� �౸)
	typedef struct
	{
		PBMSG_HEAD	h;
		BYTE		subcode;
		WORD		Second;
	} PMSG_TIMEVIEW, *LPPMSG_TIMEVIEW;
#endif


// [0xF3][0x23] ���� �˸���. (���� �౸)
typedef struct
{
	PBMSG_HEAD	h; //0x00
	BYTE		subcode; //0x03

	char		RedTeamName[MAX_GUILDNAMESTRING]; //0x04
	BYTE		RedTeamScore; //0x0C

	char		BlueTeamName[MAX_GUILDNAMESTRING]; //0x0D
	BYTE		BlueTeamScore; //0x15

} PMSG_GOALSEND, *LPPMSG_GOALSEND;


// [0xF3][0x30] ��ų Ű ���� �ְ� �޴´�.
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
	int			QWERLevel;	// Ŭ���̾�Ʈ�� BYTE QWERLevel[4] ���
#endif
} PMSG_SKILLKEY, *LPPMSG_SKILLKEY;
#ifdef MODIFY_R_HOTKEY_SAVE_BUGFIX_20080910
#pragma pack(pop)
#endif // MODIFY_R_HOTKEY_SAVE_BUGFIX_20080910


// [0xF3][0x40] ���� ��ɾ� �ڵ�
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	BYTE		CmdType;		// ��ɾ� type 0 : ����, 1 : �޽��� �ڽ�
	
	BYTE		X;				// 0 �϶� x, y ��ǥ,   1�϶�, X�� �޽��� ��ȣ
	BYTE		Y;		
} PMSG_SERVERCMD, *LPPMSG_SERVERCMD;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// Clinet <-> GS ��������
//----------------------------------------------------------------------------
// GC [0xF3][0x50] �����ͷ��� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD  h;	
	BYTE		subcode;	
	
	short		nMLevel;					// �����ͷ���

	BYTE		btMLExp1;					// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 1)
	BYTE		btMLExp2;					// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 2)
	BYTE		btMLExp3;					// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 3)
	BYTE		btMLExp4;					// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 4)
	BYTE		btMLExp5;					// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 5)
	BYTE		btMLExp6;					// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 6)
	BYTE		btMLExp7;					// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 7)
	BYTE		btMLExp8;					// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 8)

	BYTE		btMLNextExp1;				// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 1)
	BYTE		btMLNextExp2;				// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 2)
	BYTE		btMLNextExp3;				// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 3)
	BYTE		btMLNextExp4;				// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 4)
	BYTE		btMLNextExp5;				// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 5)
	BYTE		btMLNextExp6;				// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 6)
	BYTE		btMLNextExp7;				// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 7)
	BYTE		btMLNextExp8;				// ���� �����ͷ��� ����ġ (INT64�� �ֻ��� ����Ʈ ���� 8)

	short		nMLPoint;					// �����ͷ��� ����Ʈ

	// �����ͷ����� ���� ������ �缳���ȴ�.
	WORD		wMaxLife;
	WORD		wMaxMana;
	WORD		wMaxShield;
	WORD		wMaxBP;	

	// �����ͽ�ų �� �����ͽ�ų Ʈ�� ����(����)
} PMSG_MASTERLEVEL_INFO, *LPPMSG_MASTERLEVEL_INFO;


//----------------------------------------------------------------------------
// GC [0xF3][0x51] �����ͷ����� �ö���. 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	short		nMLevel;				// ���� �����ͷ���

	short		nAddMLPoint;			// �߰��� �����ͷ��� ����Ʈ
	short       nMLPoint;				// ��� ������ �����ͷ��� ����Ʈ	
	short		nMaxMLPoint;			// �ִ� �����ͷ��� ����Ʈ

	WORD		wMaxLife;
	WORD		wMaxMana;
	WORD		wMaxShield;
	WORD		wMaxBP;
} PMSG_MASTERLEVEL_UP, *LPPMSG_MASTERLEVEL_UP;
	
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912


#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// Clinet <-> GS ��������
//----------------------------------------------------------------------------
// CG [0xF3][0x52] �����ͷ��� ��ų ���� ��û
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	int			iMasterLevelSkill;	// �����Ϸ��� ��ų ��ȣ
} PMSG_REQ_MASTERLEVEL_SKILL, *LPPMSG_REQ_MASTERLEVEL_SKILL;

//----------------------------------------------------------------------------
// GC [0xF3][0x52] �����ͷ��� ��ų ���� ��û�� ���� ����
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD	h;
	BYTE		subcode;

	BYTE		btResult;	// �����	0:���� 1:�����ߴ�. 2:���� ������ ����� �ƴ� 3:���� ������ ��ų��ȣ �ƴ�
							//			4:�����ͷ��� ����Ʈ ���� 5:�� ��

	short       nMLPoint;	// ���� �����ͷ��� ����Ʈ

	int			nSkillNum;		// ������ ��ų ��ȣ (-1�̸� ����)
	int			nSkillLevel;	// ������ ��ų ����
} PMSG_ANS_MASTERLEVEL_SKILL, *LPPMSG_ANS_MASTERLEVEL_SKILL;
#endif

//----------------------------------------------------------------------------
// GC [0x07] ���� ���� �����鿡�� ���� ���¸� ������
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		State;				// 0 : ����, 1 : ��������

#ifdef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		BuffIndex;
#else
	#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	INT		ViewSkillState;		// �����ڵ�
	#else
	WORD		ViewSkillState;		// �����ڵ�
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
// �̺�Ʈ ���� �˸�
// 0x0B
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		State;		// ���� ( 0 == Ȱ��ȭ , 1 == ��Ȱ��ȭ )
	BYTE		Event;		// �̺�Ʈ ��ȣ ( 1 == ȯ�� �巡��, 2 == Ÿ��¯ )
} PMSG_EVENT, *LPPMSG_EVENT;



//----------------------------------------------------------------------------

// ���� ���� ��������
// 0x0F
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Weather;
} PMSG_WEATHER, *LPPMSG_WEATHER;

//----------------------------------------------------------------------------
// CG [PACKET_MOVE] : Ŭ���̾�Ʈ�� �ڽ��� ������ �н��� ������.
//----------------------------------------------------------------------------
typedef struct
{	
	PBMSG_HEAD	h;
	
	BYTE	X;
	BYTE	Y;
	BYTE	Path[MAX_PATHDIR];
} PMSG_MOVE, * LPPMSG_MOVE;

//----------------------------------------------------------------------------
// GC [PACKET_MOVE] : �ٸ�Ŭ���̾�Ʈ�� ������ ����
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
// CG [PACKET_POSITION] : ��ǥ ������
//----------------------------------------------------------------------------
typedef struct
{	
	PBMSG_HEAD	h;
	BYTE	X;
	BYTE	Y;
} PMSG_POSISTION_SET, * LPPMSG_POSISTION_SET;

//----------------------------------------------------------------------------
// GC [PACKET_POSITION] : �ٸ������ ��ǥ
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
// ����ڰ� ������ ������ �� ĳ���� ���� ������ ������.
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
// �ٸ� ĳ���Ͱ� ����Ʈ �ȿ� ���Դٰ� �˸���.
// 0x12
//----------------------------------------------------------------------------
typedef struct 
{
	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	X;
	BYTE	Y;
	
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE	CharSet[MAX_PREVIEWCHARSET+9];		// ĳ���� ��Ʈ (������)
#else
#ifdef DARKLORD_WORK
	BYTE	CharSet[MAX_PREVIEWCHARSET+4];		// ĳ���� ��Ʈ (������)
#else
	BYTE	CharSet[MAX_PREVIEWCHARSET+3];	// ĳ���� ��Ʈ (������)
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
// ���Ͱ� ����Ʈ �ȿ� ���Դٰ� �˸���.
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
// ���Ͱ� ����Ʈ �ȿ� ���Դٰ� �˸���.
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
// �ٸ� ������ ����Ʈ���� �������.
// PBMSG_COUNT �ڿ� ���� �ٴ´�
//----------------------------------------------------------------------------
typedef struct 
{
	BYTE	NumberH;	// ĳ���� ��ȣ�� �ѷ� ������.
	BYTE	NumberL;	
} PMSG_VIEWPORTDESTROY, *LPPMSG_VIEWPORTDESTROY;

//----------------------------------------------------------------------------
// CG [PACKET_ATTACK] ĳ���Ͱ� ������ �� �ߴ�.
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
// GC [PACKET_ATTACK] ĳ���� ���ݿ� ���� ����� �˷��ش�.
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
// GC [0x16] �������� �׿��ٸ�(������ ���� ��������)
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
//	BYTE	Skill;		// �׾����� � ��ų�� �������� ���� ��ȣ 
//#endif
} PMSG_KILLPLAYER, *LPPMSG_KILLPLAYER;

//----------------------------------------------------------------------------
// GC [0x17] �������� �׾��ٸ�(����Ʈ�� �ٸ� �����鿡��)
// ��ȣ�� �ڽ��̶�� �ڽ��� ��������
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;

#ifdef ADD_SKILL_WITH_COMBO
	#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
// CG [0x18] ĳ���Ͱ� Ư���� ������ ���ߴ�.
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
// GC [0x18] �ٸ� ĳ���Ͱ� Ư���� ������ ���ߴ�.
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
// CG [0x19] ��������
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
// GC [0x19] �������� ��� 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
// CG [0x1A] Ư�� ��ġ�� ���� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		MagicNumber;
	BYTE		MapX;
	BYTE		MapY;
} PMSG_POSMAGIC, *LPPMSG_POSMAGIC;

//----------------------------------------------------------------------------
// CG [0x1A] Ư�� ��ġ�� ���� ���� ���
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
// CG [0x1A] Ư�� ��ġ�� ���� ���� ���
//----------------------------------------------------------------------------
typedef struct
{
	BYTE	NumberH;
	BYTE	NumberL;
} PMSG_POSMAGIC_RESULT, *LPPMSG_POSMAGIC_RESULT;

//----------------------------------------------------------------------------
// GC [0x1B] ���� ���� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	BYTE		MagicNumberH;
	BYTE		MagicNumberL;
#else
	BYTE		MagicNumber;
#endif
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_MAGICCANCEL, *LPPMSG_MAGICCANCEL;

//----------------------------------------------------------------------------
// GC [0x2D] ������ ȿ�� ����
//----------------------------------------------------------------------------
#ifdef ITEM_ADD_OPTION_20061019
typedef struct
{
	PBMSG_HEAD	h;

	WORD		wOptionType;
	WORD		wEffectType;

	BYTE		byEffectOption;		// ȿ�� �ɼ� (0 : ���, 1 : ��� �ð� ����, 2 : ���)

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	int			iLeftTime;
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#if defined MODIFY_BUFF_SYSTEM_EXTENTION_20070917 && !defined ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205			// !! NOT
	int			iLeftTime;
#endif // defined MODIFY_BUFF_SYSTEM_EXTENTION_20070917 && !defined ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE		btBuffEffectType;	// Ȯ�� ����ȿ�� �ε���
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
} PMSG_USEEFFECTITEM, *LPPMSG_USEEFFECTITEM;
#endif // ITEM_ADD_OPTION_20061019

//----------------------------------------------------------------------------
// CG [0x1C] �����̵� ��� 
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
// CG [0x1C] �����̵� ��� 
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
// CG [PACKET_MAGIC_ATTACK] ���� ���ݿ� �ٸ� ĳ���Ͱ� �������� ī����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
// CG [PACKET_MAGIC_ATTACK] ���� ���ݿ� �ٸ� ĳ���Ͱ� �������� ���� ĳ���͵�..
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
// CG [0x1E] �������� ���� �߻�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
// GC [0x1E] �������� ���� �߻�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
// ����Ʈ�� ������ ����
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
// ����ڰ� ������ �ݰڴٰ� ��û
// 0x22
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
} PMSG_ITEMGETREQUEST, *LPPMSG_ITEMGETREQUEST;

//----------------------------------------------------------------------------
// �������� �ݰڴٰ� ��û�� �������� ����� �˸���.
// 0x22
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE	result;					// 0xFE - ��, 0xFD - �������� ������ (�ʵ忡���� ���ش�.)

	BYTE	Data[ITEM_BUFFER_SIZE];	// ���� 4����Ʈ. �������� 5����Ʈ ����
} PMSG_ITEMGETRESULT, *LPPMSG_ITEMGETRESULT;

//----------------------------------------------------------------------------
// ������ �������� ������ ��û
// 0x23 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		px;
	BYTE		py;
	BYTE		Ipos;	// �κ��丮������ ��ġ 
} PMSG_ITEMTHROW, *LPPMSG_ITEMTHROW;

//----------------------------------------------------------------------------
// ������ �������� ������ ���
// 0x23 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	BYTE		Ipos;	// �κ��丮������ ��ġ 
} PMSG_ITEMTHROW_RESULT, *LPPMSG_ITEMTHROW_RESULT;


//----------------------------------------------------------------------------
// �κ��丮���� �������� �̵��ϴ�
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
// �ٸ� ��� �ٲ��.
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
// ĳ���� ������� �ٽ� ä���ִ� �뵵�� ���ȴ�.
// CS : [0x26]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		inventoryPos;		// �κ��丮���� ��ġ ��ȣ
	BYTE		invenrotyTarget;	// ����ϴ� ��� ������	

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	BYTE		btItemUseType;		// �������� ����ϴ� ���. ���ȿ����� ��� ����/���� ǥ��
									// 1) ���ȿ��� : 0x00 ���Ȼ��� 0x01 ���Ȱ���
#endif

} PMSG_USEITEM, *LPPMSG_USEITEM;

//----------------------------------------------------------------------------
// ĳ���� ������� �ٽ� ä���ִ� �뵵�� ���ȴ�.
// SC : [0x26]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		IPos;			// OxFF : Life ���� / OxFE : LifeMax ���� / 0xFD : Life�� ������� Ŭ���̾�Ʈ ���������� Ȱ��ȭ
	BYTE		LifeH;
	BYTE		LifeL;
	BYTE		Flag;

#ifdef ADD_SHIELD_POINT_01_20060403
	BYTE		btShieldH;
	BYTE		btShieldL;
#endif
} PMSG_REFILL, *LPPMSG_REFILL;

//----------------------------------------------------------------------------
// ĳ���� �������� ������.
// SC : [0x27]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		IPos;
	BYTE		ManaH;
	BYTE		ManaL;
#ifdef NEW_FORSKYLAND2
	BYTE		BPH;	// BP��
	BYTE		BPL;
#endif
} PMSG_MANASEND, *LPPMSG_MANASEND;

//----------------------------------------------------------------------------
// �κ��丮���� �����Ѵ�.
// SC : [0x28]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		IPos;
	BYTE		Flag;		// 0 �̸� 
} PMSG_INVENTORYDELETE, *LPPMSG_INVENTORYDELETE;

//----------------------------------------------------------------------------
// ������ ������� ���� Ư�� �ɷ� ����� �˸�
// SC : [0x29]
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Number;
	WORD		Time;
} PMSG_ITEMUSESPECIALTIME, *LPPMSG_ITEMUSESPECIALTIME;

// Ư�� �������� �������� �˸���.
// SC : [0x2A]
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		IPos;
	BYTE		Dur;
	BYTE		Flag;
} PMSG_ITEMDUR, *LPPMSG_ITEMDUR;

//----------------------------------------------------------------------------
// 0x2B �������� �κ��� �߰� 
//----------------------------------------------------------------------------
/* ������
typedef struct
{
	PBMSG_HEAD	h;
	BYTE	Data[4];	// ������ 4����Ʈ ����
} PMSG_ITEMGIVE, *LPPMSG_ITEMGIVE;
*/


#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
//----------------------------------------------------------------------------
// [GC] 0x2C ���� ���� ��� ����� �˸���.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		result;

#ifdef ADD_PCS_FRUITS_POINT_EXTENTION_20070430
	WORD		btStatValue;		// ��/�� ���Ȱ�
#else
	BYTE		btStatValue;		// ��/�� ���Ȱ�
#endif // ADD_PCS_FRUITS_POINT_EXTENTION_20070430
	BYTE		btFruitType;		// ���� ����
} PMSG_USE_STAT_FRUIT, * LPPMSG_USE_STAT_FRUIT;

#endif





// � ĳ���Ϳ��� ���� �ɰڴٰ� ��û�Ѵ�.
// 0x30
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
} PMSG_TALKREQUEST, *LPPMSG_TALKREQUEST;

// � ĳ���Ϳ��� ���� �ɰڴٰ� ��û�ϸ� �׿����� �亯���ش�.
// 0x30
typedef struct
{
	PBMSG_HEAD	h;
	BYTE	result;

	// �Ǹ��� ���� ���� Ȯ��
	BYTE	level1;
	BYTE	level2;
	BYTE	level3;
	BYTE	level4;
#ifdef DEVILSQUARE_EXTEND_20050221		// ī���� ���� �߰�
	BYTE	level5;
	BYTE	level6;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010	// ����7 ����Ȯ�� �߰� ����
	BYTE	level7;
#endif
} PMSG_TALKRESULT, *LPPMSG_TALKRESULT;

// ������ �ִ� ������ ����Ʈ ī��Ʈ �� ���
// 0x31
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		Type;
	BYTE		count;
} PMSG_SHOPITEMCOUNT, * LPPMSG_SHOPITEMCOUNT;


// ������ �ִ� ������ ����Ʈ
// 0x31
/* ������
typedef struct
{
	BYTE	Pos;
	BYTE	ItemInfo[4];
} PMSG_SHOPITEMLIST, *LPPMSG_SHOPITEMLIST;
*/

// �������� ������ ��ٰ� ��û�Ѵ�.
// 0x32
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Pos;
} PMSG_BUYREQUEST, *LPPMSG_BUYREQUEST;

// �������� ������ ���������� ��ٸ� ����� �˷��ش�.
// 0x32
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;

	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_BUYRESULT, *LPPMSG_BUYRESULT;

// [0x33] ���� �ȱ� ��û
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Pos;
} PMSG_SELLREQUEST, *LPPMSG_SELLREQUEST;

// [0x33] ���� �ȱ� ��� 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	DWORD		Money;
} PMSG_SELLRESULT,*LPPMSG_SELLRESULT;

// [0x34] ������ ������ ���� ��û
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Position;
	BYTE		Requestpos;	// ��û�Ѱ� 0 : ���� 1 : �κ�
} PMSG_ITEMDURREPAIR, *LPPMSG_ITEMDURREPAIR;


// [0x34] ������ ������ ���� ���
typedef struct
{
	PBMSG_HEAD	h;
	int			Money;
} PMSG_ITEMDURREPAIR_RESULT, *LPPMSG_ITEMDURREPAIR_RESULT;


// CS [0x36] ���� ��ȯ ��û�Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_TRADE_REQUEST, *LPPMSG_TRADE_REQUEST;

// SC [0x36] �ٸ� ����� ���� ��ȯ�� ��û �Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
	char		szId[MAX_IDSTRING];	
} PMSG_TRADE_REQUESTSEND, *LPPMSG_TRADE_REQUESTSEND;

// CS [0x37] ���� ��ȯ�� ���� �� ���� �˸���. (��ȯ�ϰ��� �ߴ� ��������)
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Response;
	BYTE		Id[MAX_IDSTRING];
	WORD		Level;				// ���� �߰�
	int			GuildNumber;		// ��� ��ȣ �߰�
} PMSG_TRADE_RESPONSE, *LPPMSG_TRADE_RESPONSE;

// SC [0x37] ���� ��ȯ�� ���� �� �ΰ�? �� ���� ��û ���
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Response;
} PMSG_TRADE_RESPONSE_RESULT, *LPPMSG_TRADE_RESPONSE_RESULT;

// SC [0x38] ������ ��ȯâ���� �������� ����
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		TradeItemIndex;
	//BYTE		ItemInfo[4];
} PMSG_TRADE_OTHER_DEL, *LPPMSG_TRADE_OTHER_DEL;

// SC [0x39] ������ ��ȯâ�� �������� �÷����Ҵ�.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		TradeItemIndex;

	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_TRADE_OTHER_MOVE, *LPPMSG_TRADE_OTHER_MOVE;

// CS [0x3A] ���� ��ȯâ�� �ִ´�.
typedef struct
{
	PBMSG_HEAD	h;
	DWORD		Money;
} PMSG_TRADE_GOLD, *LPPMSG_TRADE_GOLD;

// CS [0x3B] ��밡 ���� ��ȯâ�� �־���.
typedef struct
{
	PBMSG_HEAD	h;
	DWORD		Money;
} PMSG_TRADE_OTHER_GOLD, *LPPMSG_TRADE_OTHER_GOLD;

// CS [0x3C] ��ȯâ Ȯ�� ��ư 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Flag;
} PMSG_TRADE_OKBUTTON, *LPPMSG_TRADE_OKBUTTON;

// SC [0x3D] ��ȯ ��� 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} PMSG_TRADE_RESULT, *LPPMSG_TRADE_RESULT;


#ifdef PERSONAL_SHOP_20040113

// SC [0x3F][0x00] ����Ʈ �� ��ħ (���λ��� ���� �̸�)
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


// CS [0x3F][0x01] ���λ��� ������ ���� ���� -> ������ ������ ������ ����
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btItemPos;
	BYTE		sItemPrice1;
	BYTE		sItemPrice2;
	BYTE		sItemPrice3;
	BYTE		sItemPrice4;
} PMSG_REQ_PSHOP_SETITEMPRICE, *LPPMSG_REQ_PSHOP_SETITEMPRICE;


// SC [0x3F][0x01] ���λ��� ������ ���� ���� ���
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;		// 1 : ���� / 2 : ������ ��ġ�� �ε����� / 3 : �ش� �������� ���� / 4 : ������ 0 ������ / 5 : ���λ��� �������� ������ 6���� / 6 : ���� ������
	BYTE		btItemPos;
} PMSG_ANS_PSHOP_SETITEMPRICE, *LPPMSG_PSHOP_SETITEMPRICE;


// CS [0x3F][0x02] ���λ������� ��û -> �̹� ������ �����Ǿ����� �̸��� ����
typedef struct
{
	PBMSG_HEAD2	h;
	CHAR		szPShopText[MAX_PSHOP_TEXT];
} PMSG_REQ_PSHOP_OPEN, *LPPMSG_REQ_PSHOP_OPEN;


// SC [0x3F][0x02] ���λ������� ���
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;		// ���λ��� ���� ��� - 0x00 : ���� / 0x01 : ���� / 0x02 : ���λ��� �������� ������ 6���� / 0x03 : �������� ��û�ڰ� ������ ��ϰ���
} PMSG_ANS_PSHOP_OPEN, *LPPMSG_ANS_PSHOP_OPEN;


// CS [0x3F][0x03] ���λ��� ���� ��û
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_PSHOP_CLOSE, *LPPMSG_REQ_PSHOP_CLOSE;


// SC [0x3F][0x03] ���λ��� ���� ��� -> ���ÿ� ���� �����ִ� ����Ʈ�� ��� ����ڵ鿡�� �˷��� �� (���� �������� ���)
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;		// ���λ��� ���� ��� - 0x00 ���� / 0x01 ����
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_ANS_PSHOP_CLOSE, *LPPMSG_ANS_PSHOP_CLOSE;


//-----------------------------------------------------
// ��� ��� ����

// CS [0x3F][0x05] ���λ������� ���� ����Ʈ ��û -> ������ ����Ʈ�� �޴´�.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		btName[MAX_IDSTRING];
} PMSG_REQ_BUYLIST_FROM_PSHOP, *LPPMSG_REQ_BUYLIST_FROM_PSHOP;


// SC [0x3F][0x05] ���λ������� ���� ����Ʈ ��û�� ���� ����
// SC [0x3F][0x13] ���λ������� ���� ����Ʈ ��û�� ���� ���� (ó�� �����°��� �ƴ� ��)
typedef struct
{
	PWMSG_HEAD2	h;
	BYTE		Result;			// 1 : ���� / 2 : �������� �ʾ��� / 3 : ������ �����ʾ��� / 4 : �����ۺ� �����̴�.
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


// CS [0x3F][0x06] ���λ������� Ư�� ���� ���� ��û
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		btName[MAX_IDSTRING];
	BYTE		btItemPos;
} PMSG_REQ_BUYITEM_FROM_PSHOP, *LPPMSG_REQ_BUYITEM_FROM_PSHOP;


// CS [0x3F][0x06] ���λ������� Ư�� ���� ���� ��û�� ���� ����
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		Result;			// 1 : ���� / 2 : �������� �ʾ��� / 3 : ������ �����ʾ��� / 4 : �ٸ������ �ŷ��� 
								// 5 : ������ ��ġ�� �ε����� / 6 : �������� �������� ���� / 7 : ���� ���ڸ� / 8 : �� �κ��� ������� ���� / 9 : ������ �� ���� ������ ��������
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		cItemInfo[ITEM_BUFFER_SIZE];
	BYTE		btItemPos;
} PMSG_ANS_BUYITEM_FROM_PSHOP, *LPPMSG_ANS_BUYITEM_FROM_PSHOP;


// CS [0x3F][0x07] ���� Ÿ���� ���λ��� â�� ���� �ִٰ� �ݾҴ�.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		btName[MAX_IDSTRING];
} PMSG_REQ_PSHOPDEAL_CLOSE, *LPPMSG_REQ_PSHOPDEAL_CLOSE;


// SC [0x3F][0x12] ���� ������ ���� �ִ� Ÿ���� ������ ����ߴ�.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_REQ_DEALER_CLOSED_SHOP, *LPPMSG_REQ_DEALER_CLOSED_SHOP;


//-----------------------------------------------------
// �Ĵ� ��� ����

// SC [0x3F][0x08] �ڽ��� ���λ������� Ư�� ������ �ǸŵǾ����� �˸� -> ������ ���� ������ �����Ұ���
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btItemPos;
	BYTE		btName[MAX_IDSTRING];		// �� �������� �� �ɸ� �̸�
} PMSG_ANS_SOLDITEM_FROM_PSHOP, *LPPMSG_ANS_SOLDITEM_FROM_PSHOP;


//-----------------------------------------------------
// ���λ��� ����Ʈ ó��

// SC [0x3F][0x10] ���� �����ִ� Ư�� ������� �����̸��� ����Ǿ���. -> [0x3F][0x01] �����̸� ���� ��� ���� ���� ����ڵ鿡�� �����ִ� ����
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		btPShopText[MAX_PSHOP_TEXT];
	BYTE		btName[MAX_IDSTRING];
} PMSG_ANS_PSHOP_TEXT_CHANGED, *LPPMSG_ANS_PSHOP_TEXT_CHANGED;


#endif


// CS [0x40] ��뿡�� ��Ƽ�� ��û�Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_PARTYREQUEST, *LPPMSG_PARTYREQUEST;


// SC [0x40] ��밡 ��Ƽ�� ��û�ߴ�.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_PARTYREQUESTSEND, *LPPMSG_PARTYREQUESTSEND;

// CS [0x41] ��Ƽ ��û�� ���� ����� �˸���. 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_PARTYREQUESTRESULT, *LPPMSG_PARTYREQUESTRESULT;

// CS [0x42] ��Ƽ�� ����Ʈ ī��Ʈ 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	BYTE		Count;
} PMSG_PARTYLISTCOUNT, *LPPMSG_PARTYLISTCOUNT;

// CS [0x42] ��Ƽ�� ����Ʈ
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


// CS [0x43] ��Ƽ���� Ż�� ��Ų��.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Number;
} PMSG_PARTYDELUSER, *LPPMSG_PARTYDELUSER;

// SC [0x43] ��Ƽ���� ����
typedef struct
{
	PBMSG_HEAD	h;	
} PMSG_PARTYDELUSERSEND, *LPPMSG_PARTYDELUSERSEND;

// SC [0x44] ��Ƽ�� ����
typedef struct
{
	BYTE Number;
	//BYTE LifeAndMaxLife;
} PMSG_PARTYLIFEALL, *LPPMSG_PARTYLIFEALL;


//----------------------------------------------------------------------------
// ������ �ٸ� ĳ���Ͱ� ����Ʈ �ȿ� ���Դٰ� �˸���.
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

#ifdef UPDATE_VIEWPORTCREATE_CHANGE_ADD_CHARSET_20070314	// ����Ʈ�� ĳ���� ��� ��
	BYTE	CharSet[MAX_PREVIEWCHARSET+9];		// ĳ���� ��Ʈ (������)
#endif
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE	BuffEffectCount;
	BYTE	BuffEffectList[MAX_BUFF_SLOT_INDEX];
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
} PMSG_VIEWPORTCREATE_CHANGE, * LPPMSG_VIEWPORTCREATE_CHANGE;


#ifdef FOR_BLOODCASTLE

//----------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� Ư������ Ư����ġ Ȥ�� ������ Ư�� �Ӽ��� �ش�
// 0x46
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE	btType;			// 0 : ������ ��Ÿ�� (��X��Y -> ��X��Y �� �� ���� ��ǥ ���� ���� (Count�� ���õ�))
							// 1 : Ư����ġ�� ��Ÿ������� �� ������ X,Y���� Count�� ��ŭ ��
	BYTE	btMapAttr;		// �ʼӼ� (0x01 : ��������, 0x04 : ��, 0x08 : ���ո�)
	BYTE	btMapSetType;	// �ʿ� �ش� �Ӽ��� �߰����� �������� (0�߰� / 1����)
	BYTE	btCount;		
} PMSG_SETMAPATTR_COUNT, * LPPMSG_SETMAPATTR_COUNT;

typedef struct				// btType�� 0�̸� �̷� ������ 2���� �ٴ´�. (��X��Y, ��X��Y)
{
	BYTE	btX;		
	BYTE	btY;		
} PMSG_SETMAPATTR, * LPPMSG_SETMAPATTR;

#endif

#ifdef SEND_GETITEMINFO_FOR_MYPARTY
//----------------------------------------------------------------------------
// ��Ƽ������ ���� ���� ������ ������ �ش�
// 0x47
//----------------------------------------------------------------------------
// ������ Ÿ�� ��Ʈ����ũ
#ifdef ITEM_INDEX_EXTEND_20050706	// ��Ƽ���� �������� �ֿ��� ���
	#define PARTY_ITEMINFO_ITEMTYPE		0x00001FFF	// ������ Ÿ��
	#define PARTY_ITEMINFO_SKILL		0x00002000	// ��ų
	#define PARTY_ITEMINFO_LUCK			0x00004000	// ���
	#define PARTY_ITEMINFO_OP			0x00008000	// �ɼ�
	#define PARTY_ITEMINFO_EX			0x00010000	// ������Ʈ
	#define PARTY_ITEMINFO_SET			0x00020000	// ��Ʈ
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		#define PARTY_ITEMINFO_380OPTION 0x00040000  // 380 �߰� �ɼ�
	#endif
#else
	#define PARTY_ITEMINFO_ITEMTYPE		0x01FF	// ������ Ÿ��
	#define PARTY_ITEMINFO_SKILL		0x0200	// ��ų
	#define PARTY_ITEMINFO_LUCK			0x0400	// ���
	#define PARTY_ITEMINFO_OP			0x0800	// �ɼ�
	#define PARTY_ITEMINFO_EX			0x1000	// ������Ʈ
	#define PARTY_ITEMINFO_SET			0x2000	// ��Ʈ
#endif

typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		NumberH;			// ������ �ε���
	BYTE		NumberL;
	
#ifdef ITEM_INDEX_EXTEND_20050706
	UINT		ItemInfo;			// ������ ����
#else
	WORD		ItemInfo;			// ������ ����
#endif	
#ifdef SEND_GETITEMINFO_FOR_MYPARTY_ADD_LEVEL	
	BYTE		Level;
#endif
} PMSG_GETITEMINFO_FOR_PARTY, * LPPMSG_GETITEMINFO_FOR_PARTY;
#endif

#ifdef ADD_EFFECT_LEVELUP_20040401
//----------------------------------------------------------------------------
// ����(ġ��, ����) ���� �Ծ��� ��� ȭ����� �ٸ� �������� �˸� ȿ��
// 0x48
//----------------------------------------------------------------------------

#define EFFECT_POTION_HP			0x01	// ����
#define EFFECT_POTION_MP			0x02	// ����
#define EFFECT_LEVELUP				0x10	// ������

#ifdef ADD_SHIELD_POINT_01_20060403
#define EFFECT_POTION_SD			0x03	// ���� ����
#define EFFECT_SHIELDBROKE			0x11	// ���� �ı� ����Ʈ
#endif

typedef struct 
{
	PBMSG_HEAD	h;	
	
	BYTE		NumberH;		// ������ �ε���
	BYTE		NumberL;

	BYTE		btType;			// ���� ����(ȿ��Ÿ��)
} PMSG_EFFECTINFO, * LPPMSG_EFFECTINFO;
#endif

//***************************************************************************//
//***************************************************************************//

//----------------------------------------------------------------------------
// CS [0x50] ��� �����Ϳ��� ������ ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_GUILDJOINQ, *LPPMSG_GUILDJOINQ;

//----------------------------------------------------------------------------
// SC [0x50] ��밡 ��� ������ ��û�� �Դ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_GUILDQUESTSEND, *LPPMSG_GUILDQUESTSEND;

//----------------------------------------------------------------------------
// CS [0x51] ��� ��û�� ���� ����� �˸���. 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	BYTE		NumberH;
	BYTE		NumberL;
} PMSG_GUILDQRESULT, *LPPMSG_GUILDQRESULT;

//----------------------------------------------------------------------------
// CS [0x52] ���� ����Ʈ�� �����ش�.
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		Result;
	BYTE		Count;

	int			TotalScore;
	BYTE		Score;

#ifdef ADD_NEW_UNION_01_20041006
	// ���� ��� ����
	CHAR		szRivalGuild[MAX_GUILDNAMESTRING+1];		
#endif

} PMSG_GUILDLISTCOUNT, *LPPMSG_GUILDLISTCOUNT;

//----------------------------------------------------------------------------
// CS [0x52] ���� ����Ʈ
//----------------------------------------------------------------------------
typedef struct
{
	char		Name[MAX_IDSTRING];
	BYTE		Number;
	BYTE		ConnectAServer;		// ���� ������.. ��������?

#ifdef ADD_NEW_GUILD_01_20040913
	BYTE		btGuildStatus;
#endif

} PMSG_GUILDLIST, *LPPMSG_GUILDLIST;

//----------------------------------------------------------------------------
// CS [0x53] ��忡�� ���� ��Ų��.
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
// SC [0x53] ��忡�� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Result;
} PMSG_GUILDDELUSER_RESULT, *LPPMSG_GUILDDELUSER_RESULT;

//----------------------------------------------------------------------------
// SC [0x54] ��� �����Ͱ� �ǰڳİ� �����.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_GUILDMASTERQUESTION, *LPPMSG_GUILDMASTERQUESTION;

//----------------------------------------------------------------------------
// CS [0x54] ��� �����Ͱ� �ǰڳĴ� ��û�� ���� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} PMSG_GUILDMASTERANSWER, *LPPMSG_GUILDMASTERANSWER;

//----------------------------------------------------------------------------
// SC [0x55] ��� �����Ͱ� �� �� ������ ��� �޴����� �����϶�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_GUILDMASTERMANAGER_RUN;

//----------------------------------------------------------------------------
// CS [0x55] ��� �޴��� �Ϸ� (����)
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
// CS [0x56] ��� ���� ��û ���
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
// CS [0x56] �ٸ� ��忡 ������ �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING];
} PMSG_GUILDWAR, *LPPMSG_GUILDWAR;

//----------------------------------------------------------------------------
// SC [0x57] �ٸ� ��尡 ������ ������ �Դ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING];
} PMSG_GUILDWARQ, *LPPMSG_GUILDWARQ;

//----------------------------------------------------------------------------
// CS [0x57] ���� ������ ���� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
	BYTE		Result;
} PMSG_GUILDWARA, *LPPMSG_GUILDWARA;

//----------------------------------------------------------------------------
// SC [0x5A] ����Ʈ�� ��� ����
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
// SC [0x65] ����Ʈ�� ��� ����
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
	// �� ������ ����
	// 0x00	: ���� ����
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
// CS [0x66] ��� Viewport ���� ��û
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
// SC [0x66] ��� Viewport ���� ��û�� ���� ����.
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
// GC[0x67] Union ViewPort ���� ��ȭ�� Notify �Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btCount;
} PMSG_UNION_VIEWPORT_NOTIFY_COUNT, *LPPMSG_UNION_VIEWPORT_NOTIFY_COUNT;

typedef struct
{
	BYTE		btNumberH;				// ���� �ε���
	BYTE		btNumberL;
	
	INT			iGuildNumber;			// ��� ��ȣ

	BYTE		btGuildRelationShip;
	CHAR		szUnionName[MAX_GUILDNAMESTRING];
} PMSG_UNION_VIEWPORT_NOTIFY, *LPPMSG_UNION_VIEWPORT_NOTIFY;
#endif // ADD_NEW_UNION_01_20041006

//----------------------------------------------------------------------------
// SC [0x5B] ����Ʈ�� ��忡 �����ִ� ���
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
// SC [0x5C] ����Ʈ���� ��忡 �����ߴ�.. ��� �ݿ��Ѵ�. 
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
// SC [0x5D] ����Ʈ���� ��忡�� �������Դ�.. ��� �ݿ��Ѵ�. 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	NumberH;
	BYTE	NumberL;
	
} PMSG_GUILDDEL_VIEWPORT_NOW, *LPPMSG_GUILDDEL_VIEWPORT_NOW;


//----------------------------------------------------------------------------
// SC [0x60] ��� ��忡 ������ ������ ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;
} PMSG_GUILDWARREQUEST_RESULT, *LPPMSG_GUILDWARREQUEST_RESULT;


//----------------------------------------------------------------------------
// SC [0x61] �ٸ� ��尡 ������ �����ߴ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	char		GuildName[MAX_GUILDNAMESTRING];
	BYTE		Type;	// ����� ����
} PMSG_GUILDWARSEND, *LPPMSG_GUILDWARSEND;

//----------------------------------------------------------------------------
// SC [0x61] ���� ���� ��û�� ���� ��� 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;
} PMSG_GUILDWARSEND_RESULT, *LPPMSG_GUILDWARSEND_RESULT;

//----------------------------------------------------------------------------
// SC [0x62] ���ʿ� ������ �ִ� ��� �������� ������ �˸���.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING];
	BYTE		Type;
	BYTE		TeamCode;
} PMSG_GUILDWAR_DECLARE, *LPPMSG_GUILDWAR_DECLARE;

//----------------------------------------------------------------------------
// SC [0x63] ���ʿ� ������ �ִ� ��� �������� ������ ���Ḧ �˸���.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	char		GuildName[MAX_GUILDNAMESTRING];
} PMSG_GUILDWAR_END, *LPPMSG_GUILDWAR_END;

//----------------------------------------------------------------------------
// SC [0x64] ��� ������ �����ش�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Score1;	// �츮�� ����
	BYTE		Score2; // ����� ����
	BYTE		Type;
} PMSG_GUILDSCORE, *LPPMSG_GUILDSCORE;




//----------------------------------------------------------------------------
// ���ο� ��� �ý��� ��������
//
// Lastest : 20040913, 20040922, 20041006, 20050418
//
//----------------------------------------------------------------------------

#ifdef ADD_NEW_GUILD_01_20040913
typedef struct {}__UNION_PROTOCOL_H___;
//----------------------------------------------------------------------------
// CG[0xE1] ��å�� �Ӹ� / ���� / ���� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btType;								// �Ӹ�(0x01) / ����(0x02) / ����(0x03)
	BYTE		btGuildStatus;						// �Ӹ� �Ǵ� ��å. 
	char		szTagetName[MAX_IDSTRING];			// ��� ���� �̸�.

} PMSG_GUILD_ASSIGN_STATUS_REQ, *LPPMSG_GUILD_ASSIGN_STATUS_REQ;

//----------------------------------------------------------------------------
// CG[0xE1] ��å�� �Ӹ� / ���� / ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btType;								// �Ӹ�(0x01) / ����(0x02) / ����(0x03)
	BYTE		btResult;							// 0x00 :����  0x01 :����
													// 0x10 :��尡 ����		GUILD_ANS_NOTEXIST_GUILD
													// 0x11 :���� ����			GUILD_ANS_NOTEXIST_PERMISSION
													// 0x12 :�Ӹ��� ��å�� ���� GUILD_ANS_NOTEXIST_EXTRA_STATUS

	char		szTagetName[MAX_IDSTRING];			// ��� ���� �̸�.

} PMSG_GUILD_ASSIGN_STATUS_RESULT, *LPPMSG_GUILD_ASSIGN_STATUS_RESULT;

#endif


#ifdef ADD_NEW_GUILD_02_20040922

//----------------------------------------------------------------------------
// CG[0xE2] ��� TYPE�� ���� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btGuildType;						// ���� �Ǵ� TYPE. 
	
} PMSG_GUILD_ASSIGN_TYPE_REQ, *LPPMSG_GUILD_ASSIGN_TYPE_REQ;

//----------------------------------------------------------------------------
// GC[0xE2] ��� TYPE�� ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btGuildType;						// ���� �Ǵ� ��å. 
	BYTE		btResult;							// 0x00 :����  0x01 :����
													// 0x10 :��尡 ����		GUILD_ANS_NOTEXIST_GUILD
													// 0x11 :���� ����			GUILD_ANS_NOTEXIST_PERMISSION
													// 0x12 :������ TYPE�� ���� GUILD_ANS_NOTEXIST_EXTRA_TYPE

} PMSG_GUILD_ASSIGN_TYPE_RESULT, *LPPMSG_GUILD_ASSIGN_TYPE_RESULT;

#endif


#ifdef ADD_NEW_UNION_01_20041006
//----------------------------------------------------------------------------
// CG[0xE5], DC[0xE5] Union, Rival ���� �α�/���� 
// ( [CD]����α�/���� ��û ����, [DC]��뿡�� ���� ��û )
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btRelationShipType;					// RelationShip�� Ÿ��
													// 0x01	:Union										
													// 0x02 :Rival

	BYTE		btRequestType;						// ���� �α��ΰ�? ���� �ΰ�?
													// 0x01 :Join
													// 0x02 :BreakOff
	
	BYTE		btTargetUserIndexH;
	BYTE		btTargetUserIndexL;
	
} PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ, *LPPMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ;

//----------------------------------------------------------------------------
// GC[0xE6], CD[0xE6] Union, Rival ���� �α�/���� ���
// ( [CD]���� �α�/���� ���� ��û ����, [DC]��뿡�� ���� �α�/���� ���� ��û )
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btRelationShipType;					// RelationShip�� Ÿ��
													// 0x01	:Union										
													// 0x02 :Rival
	
	BYTE		btRequestType;						// ���� �α��ΰ�? ���� �ΰ�?
													// 0x01 :Join
													// 0x02 :BreakOff

	BYTE		btResult;							// 0x00 :����  0x01 :����
													// 0x10 :��尡 ����		GUILD_ANS_NOTEXIST_GUILD
													// 0x11 :���� ����			GUILD_ANS_NOTEXIST_PERMISSION

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
// CG[0xEA][0x01] ����������� : ��� ��ũ ���� ��û
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
// CG[0xEA][0x01] ����������� : ��� ��ũ ���� ���
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
// CG[0xEB][0xXX] ���ձ�� �������� Ability ����
// 
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// CG[0xEB][0x01] ���ձ�� �����Ͱ� ���� ����� ���� Ż�� ��Ŵ : ��û
// 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	char		szTargetGuildName[MAX_GUILDNAMESTRING];
	
} PMSG_KICKOUT_UNIONMEMBER_REQ, *LPPMSG_KICKOUT_UNIONMEMBER_REQ;


//----------------------------------------------------------------------------
// CG[0xEB][0x01] ���ձ�� �����Ͱ� ���� ����� ���� Ż�� ��Ŵ : ���
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
// SC [0x71] ��(PING).
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
} PMSG_PING, *LPPMSG_PING;

//----------------------------------------------------------------------------
// CS [0x71] ��(PING).
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
} PMSG_PING_RESULT, *LPPMSG_PING_RESULT;

//***************************************************************************//
//***************************************************************************//
// [0x81] ���� ��/��� ��Ų��.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Type;
	int			Money;
} PMSG_WAREHOUSEMONEYINOUT,*LPPMSG_WAREHOUSEMONEYINOUT;

// [0x81] ���� ������Ʈ ��Ų��.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	int			wMoney;
	int			iMoney;

} PMSG_MONEY, *LPPMSG_MONEY;

// [0x83] â���й�ȣ�� ������ ������.
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

// [0x83] â�� ��� ����
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		State;
} PMSG_WAREHOUSESTATE,*LPPMSG_WAREHOUSESTATE;


//***************************************************************************//

#ifdef 	CHAOS_MIX_UPGRADE
//***************************************************************************//
// [0x86] ī���� ���� ��û
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Type;			//1: // �Ϲ� ����
//								2: // �Ǹ��� �� + ����
//								3: // +10 �����
//								4: // +11 �����
//								5: // �䰡�þ� �����
//								6: // ���� �����
//								7: // ���� �����
//								8: // ������� �����
//								11:// �Ϲ� ���� ����

#ifdef ITEM_12_13_20040401
//								12: // ��Ʈ ��_��
//								22: // +12 �����
//								23: // +13 �����
#endif
	
#ifdef MODIFY_CHAOSMIX_PROTOCOL_EXTEND_20080526
	BYTE		SubType;		// ī���� ���� Ȯ���� ���� ����Ÿ�� �߰�
#endif // MODIFY_CHAOSMIX_PROTOCOL_EXTEND_20080526

} PMSG_CHAOSMIX, *LPPMSG_CHAOSMIX;
#endif

//***************************************************************************//
// [0x86] ī���� ���� ��� ����
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;
	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_CHAOSMIXRESULT, *LPPMSG_CHAOSMIXRESULT;


// [0x90] �������� �̵��� ��û�Ѵ�
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		SquareNumber;				// ���� ��ȣ
	BYTE		InvitationItemPos;			// �ʴ���� ��ġ
} PMSG_REQ_MOVEDEVILSQUARE, *LPPMSG_REQ_MOVEDEVILSQUARE;

// [0x90] �������� �̵���û ���
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;				// ���� ��ȣ	
} PMSG_RESULT_MOVEDEVILSQUARE, *LPPMSG_RESULT_MOVEDEVILSQUARE;

// [0x91] ������½ð� ��û
typedef struct
{
	PBMSG_HEAD	h;
#ifdef FOR_BLOODCASTLE
	BYTE		hEventType;			// 0 : ���������� / 1 : ���� ĳ��
	BYTE		btItemLevel;		// ����Ʈ ������ ���� -> 1 ~ 7 �ٸ���ȣ (���� ĳ���� ���)
#endif
} PMSG_REQ_DEVILSQUARE_REMAINTIME, *LPPMSG_REQ_DEVILSQUARE_REMAINTIME;

// [0x91] ���½ð� ��û ���
typedef struct
{
	PBMSG_HEAD	h;
#ifdef FOR_BLOODCASTLE
	BYTE		hEventType;			// 1 : ���������� / 2 : ���� ĳ��	/ 4 : ī���� ĳ��
#endif
	BYTE		RemainTime;			// ���� ��  (ī���� ĳ�������� ��������Ʈ)

#ifdef CHAOSCASTLE_SYSTEM_20040408	// [0x91] ��Ŷ ����
	BYTE		EnteredUser;		// ���� ī����ĳ���� ������ ������� ��
#ifdef CHAOSCASTLE_TIME_BUGFIX_20040508
	BYTE		RemainTime_LOW;		// ���� ��  (ī���� ĳ�������� ��������Ʈ)
#endif
#endif	
} PMSG_RESULT_DEVILSQUARE_REMAINTIME, *LPPMSG_RESULT_DEVILSQUARE_REMAINTIME;

// [0x92] ���� ī��Ʈ �˸��� 
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Type;				// ����->���(0), ��->���常(1)
#ifdef CHAOSCASTLE_SYSTEM_20040408
									/*
									0	- ���� ������ : ���� 30����
									1	- ���� ������ : ���� 30����
									3	- ���� ĳ�� : ���� 30����
									4	- ���� ĳ�� : ������ 30����
									5	- ���� ĳ�� : ������� 30����
									6	- ���� ĳ�� : ���� 30����
									10	- ī���� ĳ�� : ���� 30����
									11	- ī���� ĳ�� : ������ 30����
									12	- ī���� ĳ�� : ������� 30����
									13	- ī���� ĳ�� : ���� 30����
									*/
#endif
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
									/*
									15,	-	ȯ����� : �غ� 30������ �޽��� Ÿ��
									16,	-	ȯ����� : ������ 30������ �޽��� Ÿ��
									*/
#endif
} PMSG_SET_DEVILSQUARE, *LPPMSG_SET_DEVILSQUARE;


// ���� �̺�Ʈ ��� Record
typedef struct
{
	char	Name[10];
	int		TotalScore;	// ��Ż �������� 
	int		BonusExp;	// ���� ����ġ
	int		BonusZen;	// ���� �����
}DevilSquareScoreInfo, *LPDevilSquareScoreInfo;

#pragma pack(push,1)
// [0x93] ���� �̺�Ʈ�� ������, ��� ����
typedef struct
{
	PBMSG_HEAD				h;
	BYTE					MyRank;
	BYTE					Count;
	DevilSquareScoreInfo	Score[11];
}PMSG_DEVILSQUARERESULT, *LPPMSG_DEVILSQUARERESULT;
#pragma pack(pop)


// SC [0x94] �̺�ƮĨ ���� ����
typedef struct
{
	PBMSG_HEAD	h;
#ifdef  FOR_BLOODCASTLE
	BYTE		Type;		// 0 : ���� / 1 : ���� / 2 : ���� / 3 : ����ȯ�� / 4 : �����ǵ� / 5 : �����ǵ� ȯ��
#endif

#ifdef EVENTCHIP_DATASIZE_EXPAND_20090217
	INT			ChipCount;
#else
	unsigned short	ChipCount;
#endif //EVENTCHIP_DATASIZE_EXPAND_20090217

	short			MutoNum[3];
}PMSG_EVENTCHIPINFO, *LPPMSG_EVENTCHIPINFO;

// CS [0x95] �̺�ƮĨ ��� ��û
typedef struct
{
	PBMSG_HEAD	h;
#ifdef FOR_BLOODCASTLE
	BYTE		Type;		// 0 : ���� / 1 : ���� / 4 : �����ǵ�
#endif
	BYTE		ChipPos;	
}PMSG_REGEVENTCHIP,*LPPMSG_REGEVENTCHIP;

// SC [0x95] �̺�ƮĨ ��Ͽ�û ���
typedef struct
{
	PBMSG_HEAD	h;
#ifdef  FOR_BLOODCASTLE
	BYTE		Type;		// 0 : ���� / 1 : ���� / 4 : �����ǵ�
#endif

#ifdef EVENTCHIP_DATASIZE_EXPAND_20090217
	INT			ChipCount;
#else
	short		ChipCount;	
#endif //EVENTCHIP_DATASIZE_EXPAND_20090217

}PMSG_REGEVENTCHIP_RESULT, *LPPMSG_REGEVENTCHIP_RESULT;


// CS [0x96] ���� ��ȣ ��� ��û
typedef struct
{
	PBMSG_HEAD	h;	
}PMSG_GETMUTONUMBER, *LPPMSG_GETMUTONUMBER;

// SC [0x96] ���� ��ȣ ��� ���
typedef struct
{
	PBMSG_HEAD	h;	
	short		MutoNum[3];
}PMSG_GETMUTONUMBER_RESULT, *LPPMSG_GETMUTONUMBER_RESULT;


// CS [0x98] ����, ���� ȯ����û
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		btType;		// 0:���� / 1:����
}PMSG_EXCHANGE_EVENTCHIP, *LPPMSG_EXCHANGE_EVENTCHIP;


// CS [0x99] �����̵� ��û
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
// SC [0xA0] ����Ʈ ��������(��ü)
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Count;						// ����Ʈ����Ʈ��
	BYTE		State[MAX_DBQUEST];			// ����Ʈ����Ʈ ����
}PMSG_SEND_QEUSTINFO, *LPPMSG_SEND_QEUSTINFO;

// CS [0xA2] Ư�� ����Ʈ �������� ���� �䱸
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		QuestIndex;		// ����Ʈ ��ȣ
	BYTE		State;			// ����
}PMSG_SETQUEST, *LPPMSG_SETQUEST;

// SC [0xA2] Ư�� ����Ʈ �������� ���� �䱸�� ���� ����
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		QuestIndex;		// ����Ʈ ��ȣ
	BYTE		Result;			// ���
	BYTE		State;			// ����
}PMSG_SETQUEST_RESULT, *LPPMSG_SETQUEST_RESULT;


// SC [0xA3] Ư�� ����Ʈ Ŭ���� ������ ���� ����...
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		Type;			// ���� Ÿ��(200 : ����, 201 : 1������ ... )
	BYTE		Count;			// ��...
}PMSG_SETQUEST_PRIZE, *LPPMSG_SETQUEST_PRIZE;
#endif


#ifdef DARKLORD_WORK

// 0xA7	Client ==> GS	��ũ���Ǹ�(�� ������ Ŀ�ǵ� ��û)
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// ������� Ÿ�� (0:��ũ���Ǹ�)
	BYTE		Command;		// 0 : �Ϲ�(���) 1 : ��������  2: �����ϰ� ���� ���� 3 : Ÿ�ٰ���

	BYTE		NumberH;		// ����� ����� �ε��� (���� ����Ʈ)
	BYTE		NumberL;		// ����� ����� �ε��� (���� ����Ʈ)
}PMSG_REQUEST_PET_ITEM_COMMAND, *LPPMSG_REQUEST_PET_ITEM_COMMAND;

// 0xA7	GS ==> Client	��ũ���Ǹ�(�� ������ Ŀ�ǵ� ��û ���)
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// ������� Ÿ�� (0:��ũ���Ǹ�)
	BYTE		Command;		// 0 : �Ϲ�(���) 1 : ��������  2: �����ϰ� ���� ���� 3 : Ÿ�ٰ���

	BYTE		NumberH;		// ����� ����� �ε��� (���� ����Ʈ)
	BYTE		NumberL;		// ����� ����� �ε��� (���� ����Ʈ)
}PMSG_SEND_PET_ITEM_COMMAND, *LPPMSG_SEND_PET_ITEM_COMMAND;

// 0xA8	GS ==> Client	��ũ���Ǹ�(�� ������ ����)
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// ������� Ÿ�� (0:��ũ���Ǹ�)	
	BYTE		SkillType;		// ���� Ÿ�� (������ ���ؼ�)

	BYTE		NumberH;		// ������ ����� �ε��� (���� ����Ʈ)
	BYTE		NumberL;		// ������ ����� �ε��� (���� ����Ʈ)

	BYTE		TargetNumberH;		// ���(���ݴ��ϴ�)�� ����� �ε��� (���� ����Ʈ)
	BYTE		TargetNumberL;		// ���(���ݴ��ϴ�)�� ����� �ε��� (���� ����Ʈ)
}PMSG_PET_ITEM_ATTACK_COMMAND, *LPPMSG_PET_ITEM_ATTACK_COMMAND;


// 0xA9	Client ==> GS	�� ������ ���� ��û
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// ������� Ÿ�� (0:��ũ���Ǹ�)	
	BYTE		InvenType;		// �κ����� (0:�κ��丮 1:â�� 2:�ŷ�â .... )
	BYTE		nPos;			// ��ġ
}PMSG_REQUEST_PET_ITEMINFO, *LPPMSG_REQUEST_PET_ITEMINFO;

// 0xA9	GS ==> Client	�� ������ ���� ����
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		PetType;		// ������� Ÿ�� (0:��ũ���Ǹ�)	
	BYTE		InvenType;		// �κ����� (0:�κ��丮 1:â�� 2:�ŷ�â 3:���ŷ�â,4:ī���� ...., 5:���λ��� 254:������ 255:�����ٿ�  )
	BYTE		nPos;			// ��ġ

	BYTE		Level;			// ������ ����
	int			Exp;			// ����ġ
	
#ifdef  MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
	BYTE		Life;			// �����
#endif // MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626

}PMSG_SEND_PET_ITEMINFO, *LPPMSG_SEND_PET_ITEMINFO;

#endif

#ifndef ADD_NEWPVP_PKFIELD
#ifdef DUEL_SYSTEM_20031028

// SC [0xAA] �������� ��û
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ)
	CHAR		szName[10];		// �ɸ��̸�
}PMSG_REQ_START_DUEL, *LPPMSG_REQ_START_DUEL;


// SC [0xAA] �������� ��û�� ���� ����
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		bDuelStart;		// �������� �������� (0:���� / 1:����)
	BYTE		NumberH;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ)
	CHAR		szName[10];		// �ɸ��̸�
}PMSG_ANS_START_DUEL, *LPPMSG_ANS_START_DUEL;


// CS [0xAB] �������� ��û
typedef struct
{
	PBMSG_HEAD	h;
}PMSG_REQ_END_DUEL, *LPPMSG_REQ_END_DUEL;


// SC [0xAB] �������� ��û�� ���� ����
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		NumberH;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ)
	CHAR		szName[10];		// �ɸ��̸�
}PMSG_ANS_END_DUEL, *LPPMSG_ANS_END_DUEL;


// SC [0xAC] ������ ������ ���� ���� Ȯ��
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;		// ������ ��û�� ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;		// ������ ��û�� ���� ������� �ε��� (���� ����Ʈ)
	CHAR		szName[10];		// �ɸ��̸�
}PMSG_REQ_DUEL_OK, *LPPMSG_REQ_DUEL_OK;


// CS [0xAC] �������� ��û�� ���� ����
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		bDuelOK;		// ����Ȯ�� ��û�� ���� ���� (0:�ƴϿ� / 1:��)
	BYTE		NumberH;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ)
	CHAR		szName[10];		// �ɸ��̸�
}PMSG_ANS_DUEL_OK, *LPPMSG_ANS_DUEL_OK;


// SC [0xAD] ���� ���� ������
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH1;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ) - 1��
	BYTE		NumberL1;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ) - 1��
	BYTE		NumberH2;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ) - 2��
	BYTE		NumberL2;		// ���� ���̾��� ���� ������� �ε��� (���� ����Ʈ) - 2��
	BYTE		btDuelScore1;	// �������� - 1��
	BYTE		btDuelScore2;	// �������� - 1��
}PMSG_ANS_DUEL_SCORE, *LPPMSG_ANS_DUEL_SCORE;


#endif
#endif // ADD_NEWPVP_PKFIELD

#ifdef CHAOSCASTLE_SYSTEM_20040408

// [0xAF][0x01] ī���� ĳ�� ������ �̵��� ��û�Ѵ�.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		iChaosCastleNumber;		// �ٸ� ��ȣ
	BYTE		iItemPos;				// �������� ���ʼ�Ʈ�� ��ġ
} PMSG_REQ_MOVECHAOSCASTLE, *LPPMSG_REQ_MOVECHAOSCASTLE;


// [0xAF][0x01] ī���� ĳ�� ������ �̵���û ���
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		Result;					/* ��û��� (	0 : ����
														1 : �˸��� �������� ���ʼ�Ʈ ����
														2 : ���� �ִ� �ð��� �ƴ�
														3 : �������� ���ʼ�Ʈ�� ������ �ڽ��� �������� ���� ���� ������ �� ��
														4 : �������� ���ʼ�Ʈ�� ������ �ڽ��� �������� ���� ���� ������ �� ��
														5 : �ο������� �Ѿ���
														6 : 1�� �������� Ƚ���� �Ѿ���
														7 : ����ᰡ ���ڶ���.
														// ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
														8 : ī�������� ������ �� ����.
														)
										*/
} PMSG_RESULT_MOVECHAOSCASTLE, *LPPMSG_RESULT_MOVECHAOSCASTLE;


// [0xAF][0x02] ����ڰ� ī���� ĳ�� �ȿ� �̻� ������ ��ð� ������ �˸���.
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btPX;		// ��ġ X
	BYTE		btPY;		// ��ġ Y
} PMSG_REQ_REPOSUSER_IN_CC, *LPPMSG_REQ_REPOSUSER_IN_CC;


#endif


#ifdef FOR_BLOODCASTLE

// [0x9A] ���� ĳ�� �ٸ��� �̵��� ��û�Ѵ�
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		iBridgeNumber;		// �ٸ� ��ȣ
	BYTE		iItemPos;			// ��������� ��ġ
} PMSG_REQ_MOVEBLOODCASTLE, *LPPMSG_REQ_MOVEBLOODCASTLE;


// [0x9A] ���� ĳ�� �ٸ��� �̵���û ���
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;				/* ��û��� (	0 : ����
													1 : �˸��� ������� ����
													2 : ���� �ִ� �ð��� �ƴ�
													3 : �������� ������ �ڽ��� �������� ���� ���� ������ �� ��
													4 : �������� ������ �ڽ��� �������� ���� ���� ������ �� ��
													5 : �ο������� �Ѿ���
													6 : 1�� �������� Ƚ���� �Ѿ���
													// ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411			
													7 : PK ���� 1�̻� ����ڴ� ������ �� ����
									*/
} PMSG_RESULT_MOVEBLOODCASTLE, *LPPMSG_RESULT_MOVEBLOODCASTLE;


// [0x9B] ���� ĳ�� ������� ���¾˸� (���� -> �����)
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		btPlayState;		// ���� ������� ���� (0:�����۵�, 1:���������(�Ϲݸ�), 2:��������, 3:���������(������))
	WORD		wRemainSec;			// ���ӽ��� �� ������� ���� ��
	WORD		wMaxKillMonster;	// ��ƾ� �� ���� �Ҵ� ��
	WORD		wCurKillMonster;	// ������� ���� ���� ��
	WORD		wUserHaveWeapon;	// ���빫�⸦ ������ �ִ� ����� �ε���
	BYTE		btWeaponNum;		// ���빫���� ���� (0:������� / 1:������ / 2:�� / 3:����)
} PMSG_STATEBLOODCASTLE, *LPPMSG_STATEBLOODCASTLE;


//----------------------------------------------------------------------------
// GC [0x9C] 0x16 ���������� Ȯ���� -> ����ġ�� �ִ� INT�������� ���� �� �ִ�. (����ĳ�� �������ġ �ο��� ���)
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
// CG [0x9D] ���ȭ 2�ֳ� ��� ���� �̺�Ʈ �� ���� ��÷���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	CHAR		SERIAL1[5];			// �ø���1
	CHAR		SERIAL2[5];			// �ø���2
	CHAR		SERIAL3[5];			// �ø���3
} PMSG_REQ_2ANV_LOTTO_EVENT, *LPPMSG_REQ_2ANV_LOTTO_EVENT;


//----------------------------------------------------------------------------
// GC [0x9D] ���ȭ 2�ֳ� ��� ���� �̺�Ʈ �� ���� ��÷���θ� �˷���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE	btIsRegistered;		// 0:�̸� ������ ������� (����) / 1:�ٸ������ ������� (�Ʒ� �ʵ忡 ��������) / 2:�������Ƚ���� / 3:�������� �ʴ� �ø��� / 4:�˼����¿���
	CHAR	szGIFT_NAME[64];	// ��ǰ�̸�
} PMSG_ANS_2ANV_LOTTO_EVENT, *LPPMSG_ANS_2ANV_LOTTO_EVENT;


#endif // FOR_KOREA || EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO


#ifdef MONSTER_HERD_SYSTEM_20031120


//----------------------------------------------------------------------------
// GC [0x9E] Ŭ���̾�Ʈ�� Ư���� ����Ʈ�� �����ϵ��� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	WORD wEffectNum;	// Ŭ���̾�Ʈ���� ������ ����Ʈ ��ȣ (���� Ȥ�� �׷���)
} PMSG_ANS_CL_EFFECT, *LPPMSG_ANS_CL_EFFECT;


#endif


#ifdef BLOODCASTLE_EVENT_3RD_20040401

//----------------------------------------------------------------------------
// CG [0x9F] ������� ���� �̺�Ʈ (����������, ����ĳ��) ����Ƚ���� ��û
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD	h;

	BYTE btEventType;		// �̺�Ʈ ���� (1-���������� / 2-����ĳ�� ...)
} PMSG_REQ_CL_ENTERCOUNT, *LPPMSG_REQ_CL_ENTERCOUNT;


//----------------------------------------------------------------------------
// GC [0x9F] ������� ���� �̺�Ʈ (����������, ����ĳ��) ����Ƚ���� ������
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD	h;

	BYTE btEventType;		// �̺�Ʈ ���� (1-���������� / 2-����ĳ�� ...)
	BYTE btLeftEnterCount;	// ���� �������� Ƚ��
} PMSG_ANS_CL_ENTERCOUNT, *LPPMSG_ANS_CL_ENTERCOUNT;


#endif

#ifdef __FRIEND_WORK__
//----------------------------------------------------------------------------
// ģ�� ��� �������� 
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// CG [0xC0] ģ�� ����Ʈ�� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
} PMSG_FRIEND_LIST_REQ, *LPPMSG_FRIEND_LIST_REQ;

//----------------------------------------------------------------------------
// GC [0xC0] ģ�� ����Ʈ ��û�� ���� ��� (ī��Ʈ �� ����)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;	
	BYTE		MemoCount;
	BYTE		MailTotal;
	BYTE		Count;
} PMSG_FRIEND_LIST_COUNT, *LPPMSG_FRIEND_LIST_COUNT;

//----------------------------------------------------------------------------
// GC [0xC0] ģ�� ����Ʈ ��û�� ���� ��� (����Ʈ)
//----------------------------------------------------------------------------
typedef struct
{
	char		Name[MAX_IDSTRING];
	BYTE		Server;
} PMSG_FRIEND_LIST, *LPPMSG_FRIEND_LIST;


//----------------------------------------------------------------------------
// CG [0xC1] ģ�� ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_ADD_REQ, *LPPMSG_FRIEND_ADD_REQ;

//----------------------------------------------------------------------------
// GC [0xC1] ģ�� ��� ��û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Result;
	char		Name[MAX_IDSTRING];
	BYTE		State;
} PMSG_FRIEND_ADD_RESULT, *LPPMSG_FRIEND_ADD_RESULT;

//----------------------------------------------------------------------------
// �ٸ� ĳ���Ͱ� ģ�� ����� ��û �ߴٸ�,
// ȭ�鿣 "XX������ ���� ģ�� ��û�� ���Խ��ϴ�." Dialog
// GC [0xC2] ģ�� ��� Ȯ�� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_ADD_SIN_REQ, *LPPMSG_FRIEND_ADD_SIN_REQ;

//----------------------------------------------------------------------------
// CG [0xC2] ģ�� ��� Ȯ�� ��û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_ADD_SIN_RESULT, *LPPMSG_FRIEND_ADD_SIN_RESULT;

//----------------------------------------------------------------------------
// CG [0xC3] ģ�� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];	
} PMSG_FRIEND_DEL_REQ, *LPPMSG_FRIEND_DEL_REQ;

//----------------------------------------------------------------------------
// GC [0xC3] ģ�� ���� ��û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_DEL_RESULT, *LPPMSG_FRIEND_DEL_RESULT;


//----------------------------------------------------------------------------
// CG [0xC4] ģ�� ���� ��ȭ
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		ChatVeto;
} PMSG_FRIEND_STATE_C, *LPPMSG_FRIEND_STATE_C;


//----------------------------------------------------------------------------
// GC [0xC4] ģ�� ���� ��ȭ
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];	
	BYTE		State;
} PMSG_FRIEND_STATE, *LPPMSG_FRIEND_STATE;


//----------------------------------------------------------------------------
// CG [0xC5] ���� ������
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
// GC [0xC5] ���� ���� ��� 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	DWORD		WindowGuid;
} PMSG_FRIEND_MEMO_RESULT, *LPPMSG_FRIEND_MEMO_RESULT;

//----------------------------------------------------------------------------
// GC [0xC6] ���� ����
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
// GC [0xC7] ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		MemoIndex;
} PMSG_FRIEND_READ_MEMO_REQ, *LPPMSG_FRIEND_READ_MEMO_REQ;

//----------------------------------------------------------------------------
// GC [0xC7] ���� ����
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
// GC [0xC8] ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD MemoIndex;
} PMSG_FRIEND_MEMO_DEL_REQ, *LPPMSG_FRIEND_MEMO_DEL_REQ;

//----------------------------------------------------------------------------
// GC [0xC7] ���� ���� ��û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Result;
	WORD		MemoIndex;	
} PMSG_FRIEND_MEMO_DEL_RESULT, *LPPMSG_FRIEND_MEMO_DEL_RESULT;

//----------------------------------------------------------------------------
// GC [0xCA] ��ȭ�� ���� ��û 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	char		Name[MAX_IDSTRING];
} PMSG_FRIEND_ROOMCREATE_REQ, *LPPMSG_FRIEND_ROOMCREATE_REQ;


//----------------------------------------------------------------------------
// GC [0xCA] ��ȭ�� ���� ��û ���
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


// 0xCB : ��ȭ�ʴ� ��û(ģ���� �ʴ��Ѵ�)
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
	WORD		RoomNumber;
	DWORD		WindowGuid;
} PMSG_ROOM_INVITATION, *LPPMSG_ROOM_INVITATION;

// 0xCB : �ʴ� ��� 
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
// [0xF1][0x01] ���� ���̵�� ����� �Է��� ������ ��û�Ѵ�. 
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
// CG [0xB0] �����̵� ���(����� �����̵���Ų��) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;	
	BYTE		MapX;
	BYTE		MapY;
} PMSG_TARGET_TELEPORT, *LPPMSG_TARGET_TELEPORT;


// GC [0xBA] ��ų ī��Ʈ ����
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		Type;		// ��ų ��ȣ
	BYTE		Count;		// �ܰ� ī��Ʈ
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

#ifdef MAP_SERVER_WORK_20041030		// ���� �ʺ��� ���� ��Ŷ - �� Ŭ���̾�Ʈ <-> ���Ӽ���

//----------------------------------------------------------------------------
// GC [0xB1][0x00] Ŭ���̾�Ʈ���� �ʼ����� �̵��ؾ� ���� �˸� (0xC3)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;	
	CHAR		szMapSvrIpAddress[16];			// �̵��� �ʼ��� IP
	WORD		wMapSvrPort;					// �̵��� �ʼ��� ��Ʈ
	WORD		wMapSvrCode;					// �̵��� �ʼ��� �ڵ�
	INT			iJoinAuthCode1;					// ���� AUTH �ڵ� - 1
	INT			iJoinAuthCode2;					// ���� AUTH �ڵ� - 2
	INT			iJoinAuthCode3;					// ���� AUTH �ڵ� - 3
	INT			iJoinAuthCode4;					// ���� AUTH �ڵ� - 4
} PMSG_ANS_MAPSERVERCHANGE, *LPPMSG_ANS_MAPSERVERCHANGE;


//----------------------------------------------------------------------------
// CG [0xB1][0x01] �ʼ��� �̵� �� �߰����� ����, ���ӿ�û (0xC3)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	CHAR		szAccountID[MAX_IDSTRING+2];	// �������� ID
	CHAR		szCharName[MAX_IDSTRING+2];		// �������� �ɸ���
	INT			iJoinAuthCode1;					// ���� AUTH �ڵ� - 1
	INT			iJoinAuthCode2;					// ���� AUTH �ڵ� - 2
	INT			iJoinAuthCode3;					// ���� AUTH �ڵ� - 3
	INT			iJoinAuthCode4;					// ���� AUTH �ڵ� - 4
	// ���� ����Ȯ�� �ΰ� ������ [0xF1][0x01]
	INT			iTickCount;
	BYTE		btCliVersion[5];				// Ŭ���̾�Ʈ ����
	BYTE		btCliSerial[16];				// Ŭ���̾�Ʈ �ø���
} PMSG_REQ_MAPSERVERAUTH, *LPPMSG_REQ_MAPSERVERAUTH;


//----------------------------------------------------------------------------
// GC [0xB1][0x01] �ʼ��� �̵� �� �߰����� ����, ���ӿ�û ��� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;	
	BYTE		iResult;
} PMSG_ANS_MAPSERVERAUTH, * LPPMSG_ANS_MAPSERVERAUTH;

#endif


#ifdef MU_CASTLESIEGE_CL_PROTOCOL_20041122	// �� ������ - �� Ŭ���̾�Ʈ ���� ��Ŷ ����

//----------------------------------------------------------------------------
// CG [0xB2][0x00] Ŭ���̾�Ʈ�� ������ ���������� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_CASTLESIEGESTATE, *LPPMSG_REQ_CASTLESIEGESTATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x00] Ŭ���̾�Ʈ�� ������ ���������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�����ξ���/3:������������/4:����������) -> �����̸� ��������� Ȯ���� ����
	CHAR		cCastleSiegeState;			// ���� ������ ����
	BYTE		btStartYearH;				// ���� ���� - �� (���� ����Ʈ)
	BYTE		btStartYearL;				// ���� ���� - �� (���� ����Ʈ)
	BYTE		btStartMonth;				// ���� ���� - ��
	BYTE		btStartDay;					// ���� ���� - ��
	BYTE		btStartHour;				// ���� ���� - ��
	BYTE		btStartMinute;				// ���� ���� - ��
	BYTE		btEndYearH;					// ���� ���� - �� (���� ����Ʈ)
	BYTE		btEndYearL;					// ���� ���� - �� (���� ����Ʈ)
	BYTE		btEndMonth;					// ���� ���� - ��
	BYTE		btEndDay;					// ���� ���� - ��
	BYTE		btEndHour;					// ���� ���� - ��
	BYTE		btEndMinute;				// ���� ���� - ��
	BYTE		btSiegeStartYearH;			// ������ ���� - �� (���� ����Ʈ)
	BYTE		btSiegeStartYearL;			// ������ ���� - �� (���� ����Ʈ)
	BYTE		btSiegeStartMonth;			// ������ ���� - ��
	BYTE		btSiegeStartDay;			// ������ ���� - ��
	BYTE		btSiegeStartHour;			// ������ ���� - ��
	BYTE		btSiegeStartMinute;			// ������ ���� - ��
	CHAR		cOwnerGuild[MAX_GUILDNAMESTRING];		// ������ ���
	CHAR		cOwnerGuildMaster[MAX_IDSTRING];		// ������ ��� ������
#ifdef MU_CASTLESIEGE_STATE_SEC_VIEW_20050413
	CHAR		btStateLeftSec1;			// ���� ���±��� ���� �ð� - ���� 1��°
	CHAR		btStateLeftSec2;			// ���� ���±��� ���� �ð� - ���� 2��°
	CHAR		btStateLeftSec3;			// ���� ���±��� ���� �ð� - ���� 3��°
	CHAR		btStateLeftSec4;			// ���� ���±��� ���� �ð� - ���� 4��°
#endif	
} PMSG_ANS_CASTLESIEGESTATE, *LPPMSG_ANS_CASTLESIEGESTATE;


//----------------------------------------------------------------------------
// CG [0xB2][0x01] ������ ����� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_REGCASTLESIEGE, *LPPMSG_REQ_REGCASTLESIEGE;


//----------------------------------------------------------------------------
// GC [0xB2][0x01] ������ ����� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�̵̹�ϵ�/3:���������/4:������/5:�渶��������/6:��忡����������/7:�ð���������/8:������������)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// ����̸�
} PMSG_ANS_REGCASTLESIEGE, *LPPMSG_ANS_REGCASTLESIEGE;


//----------------------------------------------------------------------------
// CG [0xB2][0x02] ������ ���⸦ ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btGiveUp;					// ���⿩�� (0:�������/1:����)
} PMSG_REQ_GIVEUPCASTLESIEGE, *LPPMSG_REQ_GIVEUPCASTLESIEGE;


//----------------------------------------------------------------------------
// GC [0xB2][0x02] ������ ���⸦ ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:��ϵȱ��ƴ�/3:�ð���������)
	BYTE		btIsGiveUp;					// ���⿩�� (0:�������/1:����)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// ����̸�
} PMSG_ANS_GIVEUPCASTLESIEGE, *LPPMSG_ANS_GIVEUPCASTLESIEGE;


//----------------------------------------------------------------------------
// CG [0xB2][0x03] �ڽ��� ����� ������Ͽ� ���� ������ ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_GUILDREGINFO, *LPPMSG_REQ_GUILDREGINFO;


//----------------------------------------------------------------------------
// GC [0xB2][0x03] �ڽ��� ����� ������Ͽ� ���� ������ ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:��ϵȱ��ƴ�)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// ����̸�
	BYTE		btGuildMark1;				// ������ ǥ�� - DWORD�� �ֻ��� ����Ʈ ���� 4�ܰ�� ����
	BYTE		btGuildMark2;
	BYTE		btGuildMark3;
	BYTE		btGuildMark4;
	BYTE		btIsGiveUp;					// ���⿩��
	BYTE		btRegRank;					// ��ϼ���
} PMSG_ANS_GUILDREGINFO, *LPPMSG_ANS_GUILDREGINFO;


//----------------------------------------------------------------------------
// CG [0xB2][0x04] �ڽ��� ����� ������ ǥ�� ��� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btItemPos;					// �κ��丮���� ������ ǥ���� ��ġ
} PMSG_REQ_REGGUILDMARK, *LPPMSG_REQ_REGGUILDMARK;


//----------------------------------------------------------------------------
// GC [0xB2][0x04] �ڽ��� ����� ������ ǥ�� ��� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:��ϵȱ��ƴ�/3:������ǥ�����߸���)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// ����̸�
	BYTE		btGuildMark1;				// ������ ǥ�� - DWORD�� �ֻ��� ����Ʈ ���� 4�ܰ�� ����
	BYTE		btGuildMark2;
	BYTE		btGuildMark3;
	BYTE		btGuildMark4;
} PMSG_ANS_REGGUILDMARK, *LPPMSG_ANS_REGGUILDMARK;


//----------------------------------------------------------------------------
// CG [0xB2][0x05] ������ NPC ���� ��û -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iNpcNumber;					// NPC��ȣ
	INT			iNpcIndex;					// NPC�ε���
} PMSG_REQ_NPCBUY, *LPPMSG_REQ_NPCBUY;


//----------------------------------------------------------------------------
// GC [0xB2][0x05] ������ NPC ���� ��û ���� -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�����̾���/3:���̸��ڶ�/4:NPC���̹�����)
	INT			iNpcNumber;					// NPC��ȣ
	INT			iNpcIndex;					// NPC�ε���
} PMSG_ANS_NPCBUY, *LPPMSG_ANS_NPCBUY;


//----------------------------------------------------------------------------
// CG [0xB2][0x06] ������ NPC ���� ��û -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iNpcNumber;					// NPC��ȣ
	INT			iNpcIndex;					// NPC�ε���
} PMSG_REQ_NPCREPAIR, *LPPMSG_REQ_NPCREPAIR;


//----------------------------------------------------------------------------
// GC [0xB2][0x06] ������ NPC ���� ��û ���� -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�����̾���/3:���̸��ڶ�)
	INT			iNpcNumber;					// NPC��ȣ
	INT			iNpcIndex;					// NPC�ε���
	INT			iNpcHP;						// ü��
	INT			iNpcMaxHP;					// �ִ�ü��
} PMSG_ANS_NPCREPAIR, *LPPMSG_ANS_NPCREPAIR;


//----------------------------------------------------------------------------
// CG [0xB2][0x07] ������ NPC ���׷��̵� ��û -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iNpcNumber;					// NPC��ȣ
	INT			iNpcIndex;					// NPC�ε���
	INT			iNpcUpType;					// ���׷��̵� Ÿ�� (1:����/2:ȸ����/3:������(�ִ����))
	INT			iNpcUpValue;				// ���׷��̵� ��ġ (3�� �������� ��� ������ �ö� ������ ��)
} PMSG_REQ_NPCUPGRADE, *LPPMSG_REQ_NPCUPGRADE;


//----------------------------------------------------------------------------
// GC [0xB2][0x07] ������ NPC ���׷��̵� ��û ���� -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�����̾���/3:���̸��ڶ�/4:�����̸��ڶ�/5:Ÿ�����߸���/6:��û�����߸���/7:NPC��������������)
	INT			iNpcNumber;					// NPC��ȣ
	INT			iNpcIndex;					// NPC�ε���
	INT			iNpcUpType;					// ���׷��̵� Ÿ�� (1:����/2:ȸ����/3:������(�ִ����))
	INT			iNpcUpValue;				// ���׷��̵� ��ġ (3�� �������� ��� ������ �ö� ������ ��)
} PMSG_ANS_NPCUPGRADE, *LPPMSG_ANS_NPCUPGRADE;


//----------------------------------------------------------------------------
// CG [0xB2][0x08] ���� ���� �ڱ�, ���� ���� ��û -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_TAXMONEYINFO, *LPPMSG_REQ_TAXMONEYINFO;


//----------------------------------------------------------------------------
// GC [0xB2][0x08] ���� ���� �ڱ�, ���� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�����̾���)
	BYTE		btTaxRateChaos;				// ���� ���� ���� - ī��������
	BYTE		btTaxRateStore;				// ���� ���� ���� - ����
	BYTE		btMoney1;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 1)
	BYTE		btMoney2;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 2)
	BYTE		btMoney3;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 3)
	BYTE		btMoney4;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 4)
	BYTE		btMoney5;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 5)
	BYTE		btMoney6;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 6)
	BYTE		btMoney7;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 7)
	BYTE		btMoney8;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 8)
} PMSG_ANS_TAXMONEYINFO, *LPPMSG_ANS_TAXMONEYINFO;


//----------------------------------------------------------------------------
// CG [0xB2][0x09] ���� ���� ���� ���� ��û -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btTaxType;					// ���� ���� (1:ī��������/2:����/3:�����)
//	BYTE		btTaxRate;					// �����Ϸ��� ���� (0~200%)
	BYTE		btTaxRate1;					// �����Ϸ��� ���� (0~200%, ������� ��� ����)
	BYTE		btTaxRate2;					// �����Ϸ��� ���� (0~200%, ������� ��� ����)
	BYTE		btTaxRate3;					// �����Ϸ��� ���� (0~200%, ������� ��� ����)
	BYTE		btTaxRate4;					// �����Ϸ��� ���� (0~200%, ������� ��� ����)
} PMSG_REQ_TAXRATECHANGE, *LPPMSG_REQ_TAXRATECHANGE;


//----------------------------------------------------------------------------
// CG [0xB2][0x09] ���� ���� ���� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�����̾���)
	BYTE		btTaxType;					// ���� ���� (1:ī��������/2:����/3:�����)
//	BYTE		btTaxRate;					// ����� ���� ����
	BYTE		btTaxRate1;					// �����Ϸ��� ���� (0~200%, ������� ��� ����)
	BYTE		btTaxRate2;					// �����Ϸ��� ���� (0~200%, ������� ��� ����)
	BYTE		btTaxRate3;					// �����Ϸ��� ���� (0~200%, ������� ��� ����)
	BYTE		btTaxRate4;					// �����Ϸ��� ���� (0~200%, ������� ��� ����)
} PMSG_ANS_TAXRATECHANGE, *LPPMSG_ANS_TAXRATECHANGE;


//----------------------------------------------------------------------------
// CG [0xB2][0x10] ���� ���� �ڱ� ��� ��û -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btMoney1;					// ��� �Ϸ��� �� (INT�� ���� ����Ʈ ���� 1)
	BYTE		btMoney2;					// ��� �Ϸ��� �� (INT�� ���� ����Ʈ ���� 2)
	BYTE		btMoney3;					// ��� �Ϸ��� �� (INT�� ���� ����Ʈ ���� 3)
	BYTE		btMoney4;					// ��� �Ϸ��� �� (INT�� ���� ����Ʈ ���� 4)
} PMSG_REQ_MONEYDRAWOUT, *LPPMSG_REQ_MONEYDRAWOUT;


//----------------------------------------------------------------------------
// CG [0xB2][0x10] ���� ���� �ڱ� ��� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�����̾���)
	BYTE		btMoney1;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 1)
	BYTE		btMoney2;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 2)
	BYTE		btMoney3;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 3)
	BYTE		btMoney4;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 4)
	BYTE		btMoney5;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 5)
	BYTE		btMoney6;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 6)
	BYTE		btMoney7;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 7)
	BYTE		btMoney8;					// ���� ���� �ڱ� (INT64�� �ֻ��� ����Ʈ ���� 8)
} PMSG_ANS_MONEYDRAWOUT, *LPPMSG_ANS_MONEYDRAWOUT;

/*
//----------------------------------------------------------------------------
// CG [0xB2][0x11] ���� ������ ����� �������� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btIndex1;					// ������ �ε��� (INT�� ���� ����Ʈ ���� 1)
	BYTE		btIndex2;					// ������ �ε��� (INT�� ���� ����Ʈ ���� 2)
	BYTE		btIndex3;					// ������ �ε��� (INT�� ���� ����Ʈ ���� 3)
	BYTE		btIndex4;					// ������ �ε��� (INT�� ���� ����Ʈ ���� 4)
} PMSG_REQ_CSGATESTATE, *LPPMSG_REQ_CSGATESTATE;
*/

//----------------------------------------------------------------------------
// GC [0xB2][0x11] ���� ������ ����� �������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// �������� ��û ����� (0:����/1:����/2:��������/3:��������/4:���Ѿ���)
	BYTE		btIndex1;					// ������ ����� ������ �ε��� (WORD�� ���� ����Ʈ ���� 1)
	BYTE		btIndex2;					// ������ ����� ������ �ε��� (WORD�� ���� ����Ʈ ���� 2)
} PMSG_ANS_CSGATESTATE, *LPPMSG_ANS_CSGATESTATE;


//----------------------------------------------------------------------------
// CG [0xB2][0x12] Ư�� ������ ���� ������� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btOperate;					// �۵���Ȳ (0:�ݱ�/1:����)
	BYTE		btIndex1;					// ������ ����� ������ �ε��� (WORD�� ���� ����Ʈ ���� 1)
	BYTE		btIndex2;					// ������ ����� ������ �ε��� (WORD�� ���� ����Ʈ ���� 2)
} PMSG_REQ_CSGATEOPERATE, *LPPMSG_REQ_CSGATEOPERATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x12] Ư�� ������ ���� ������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// �������� ��û ����� (0:����/1:����/2:��������/3:��������/4:���Ѿ���)
	BYTE		btOperate;					// �۵���� (0:�ݱ�/1:����)
	BYTE		btIndex1;					// ������ ����� ������ �ε��� (WORD�� ���� ����Ʈ ���� 1)
	BYTE		btIndex2;					// ������ ����� ������ �ε��� (WORD�� ���� ����Ʈ ���� 2)
} PMSG_ANS_CSGATEOPERATE, *LPPMSG_ANS_CSGATEOPERATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x13] Ư�� ������ ���� ���������� ������ (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btOperate;					// �۵���� (0:�ݱ�/1:����)
	BYTE		btIndex1;					// ������ ����� ������ �ε��� (WORD�� ���� ����Ʈ ���� 1)
	BYTE		btIndex2;					// ������ ����� ������ �ε��� (WORD�� ���� ����Ʈ ���� 2)
} PMSG_ANS_CSGATECURSTATE, *LPPMSG_ANS_CSGATECURSTATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x14] Ư�� ������� �հ� ����ġ �۵����θ� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btIndex1;					// �հ�����ġ�� �ε��� (WORD�� ���� ����Ʈ ���� 1)
	BYTE		btIndex2;					// �հ�����ġ�� �ε��� (WORD�� ���� ����Ʈ ���� 2)
	BYTE		btUserIndex1;				// �հ�����ġ�� ����ϰ� �ִ� ������� �ε��� (WORD�� ���� ����Ʈ ���� 1)
	BYTE		btUserIndex2;				// �հ�����ġ�� ����ϰ� �ִ� ������� �ε��� (WORD�� ���� ����Ʈ ���� 2)
	BYTE		btSwitchState;				// (0:����ġ��/1:����ġ�۵�/2:�ٸ�����̻����)
} PMSG_ANS_NOTIFYSWITCHPROC, *LPPMSG_ANS_NOTIFYSWITCHPROC;


//----------------------------------------------------------------------------
// GC [0xB2][0x15] Ư�� ������� �հ� ���ε�� ���θ� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btCrownState;					// (0:���ε�Ͻ���/1:���ε�ϼ���/2:���ε�Ͻ���/3:�ٸ�����̻����)
												// (4:�հ�����ġ�۵������� �ٸ� ��������)
	
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
	DWORD		dwAccumulatedCrownAccessTime;	// ������ ���� ��� �ð�;
#endif

} PMSG_ANS_NOTIFYCROWNPROC, *LPPMSG_ANS_NOTIFYCROWNPROC;


//----------------------------------------------------------------------------
// GC [0xB2][0x16] �հ� �ֺ��� ����ڵ鿡�� �հ��� ���¸� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btCrownState;				// (0:������/1:������/2:��ϼ���)
} PMSG_ANS_NOTIFYCROWNSTATE, *LPPMSG_ANS_NOTIFYCROWNSTATE;


//----------------------------------------------------------------------------
// GC [0xB2][0x17] ���� ������ ���� ���θ� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btStartState;				// ���� ���� (0:��������ƴ�/1:������������)
} PMSG_ANS_NOTIFYCSSTART, *LPPMSG_ANS_NOTIFYCSSTART;


//----------------------------------------------------------------------------
// GC [0xB2][0x18] ���� ������ ���� ���¸� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btCastleSiegeState;			// (0:�������� ��Ͻõ�/1:�߰��¸�)
	BYTE		szGuildName[MAX_GUILDNAMESTRING];				// �ش� ���(����) �̸�
} PMSG_ANS_NOTIFYCSPROGRESS, *LPPMSG_ANS_NOTIFYCSPROGRESS;


//----------------------------------------------------------------------------
// GC [0xB2][0x19] �ڽ��� ��/�� ���� �Ӽ� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btCsJoinSide;				// (0:��������/1:������/2:������)
} PMSG_ANS_NOTIFYCSJOINSIDE, *LPPMSG_ANS_NOTIFYCSJOINSIDE;


//----------------------------------------------------------------------------
// GC [0xB2][0x1A] ���� �ʼ������� ����� ������ �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btTaxType;					// ���� ���� (1:ī��������/2:����)
	BYTE		btTaxRate;					// ����
} PMSG_ANS_MAPSVRTAXINFO, *LPPMSG_ANS_MAPSVRTAXINFO;


//----------------------------------------------------------------------------
// CG [0xB2][0x1B] �̴ϸ� �����͸� �ֱ������� ������ ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_MINIMAPDATA, *LPPMSG_REQ_MINIMAPDATA;


//----------------------------------------------------------------------------
// GC [0xB2][0x1B] �̴ϸ� �����͸� �ֱ������� ������ ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE btResult;		// �̴ϸ� ������ ��û ����� (0:����/1:����/2:�������߾ƴ�/3:���Ѿ���)
} PMSG_ANS_MINIMAPDATA, *LPPMSG_ANS_MINIMAPDATA;


//----------------------------------------------------------------------------
// CG [0xB2][0x1C] �̴ϸ� �����͸� ���̻� ������ ����� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_STOPMINIMAPDATA, *LPPMSG_REQ_STOPMINIMAPDATA;


//----------------------------------------------------------------------------
// CG [0xB2][0x1D] ������ ������� �渶�� ����鿡�� ���� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE btTeam;		// ����ȣ
	BYTE btX;			// X��ǥ
	BYTE btY;			// Y��ǥ
	BYTE btCommand;		// ��� (0:����/1:���/2:���)
} PMSG_REQ_CSCOMMAND, *LPPMSG_REQ_CSCOMMAND;


//----------------------------------------------------------------------------
// GC [0xB2][0x1D] ������ ������� �渶�� ����鿡�� ���� ���� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE btTeam;		// ����ȣ
	BYTE btX;			// X��ǥ
	BYTE btY;			// Y��ǥ
	BYTE btCommand;		// ��� (0:����/1:���/2:���)
} PMSG_ANS_CSCOMMAND, *LPPMSG_ANS_CSCOMMAND;


//----------------------------------------------------------------------------
// GC [0xB2][0x1E] ������ �� ���� �ð��� ����ڵ鿡�� �ֱ������� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE btHour;		// ���� �ð�
	BYTE btMinute;		// ���� ��
} PMSG_ANS_CSLEFTTIMEALARM, *LPPMSG_ANS_CSLEFTTIMEALARM;


//----------------------------------------------------------------------------
// CG [0xB2][0x1F] �� ����� ���� ���ɿ��� ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btHuntZoneEnter;			// ����� ���尡�� ���� (0:�Ұ��� / 1:����)
} PMSG_REQ_CSHUNTZONEENTER, *LPPMSG_REQ_CSHUNTZONEENTER;


//----------------------------------------------------------------------------
// GC [0xB2][0x1F] �� ����� ���� ���ɿ��� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�����̾���)
	BYTE		btHuntZoneEnter;			// ����� ���尡�� ���� (0:�Ұ��� / 1:����)
} PMSG_ANS_CSHUNTZONEENTER, *LPPMSG_ANS_CSHUNTZONEENTER;


//----------------------------------------------------------------------------
// CG [0xB3] �������� DB�� �����Ǵ� Ư�� NPC�� ����� ��û -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		btMonsterCode;				// DB���� NPC�� ���� (1:����/2:����)
} PMSG_REQ_NPCDBLIST, *LPPMSG_REQ_NPCDBLIST;


//----------------------------------------------------------------------------
// GC [0xB3] �������� DB�� �����Ǵ� Ư�� NPC�� ����� ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�����̾���)
	INT			iCount;
} PMSG_ANS_NPCDBLIST, *LPPMSG_ANS_NPCDBLIST;

typedef struct
{
	INT			iNpcNumber;					// NPC��ȣ
	INT			iNpcIndex;					// NPC�ε���
	INT			iNpcDfLevel;				// ���� ����
	INT			iNpcRgLevel;				// ȸ���� ����
	INT			iNpcMaxHp;					// �ִ� HP
	INT			iNpcHp;						// HP
	BYTE		btNpcX;						// ��ǥ - X
	BYTE		btNpcY;						// ��ǥ - Y
	BYTE		btNpcLive;					// NPC�� ���� �����ϴ��� ����
} PMSG_NPCDBLIST, *LPPMSG_NPCDBLIST;


//----------------------------------------------------------------------------
// CG [0xB4] ���� ��� ����� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_REQ_CSREGGUILDLIST, *LPPMSG_REQ_CSREGGUILDLIST;


//----------------------------------------------------------------------------
// GC [0xB4] ���� ��� ����� ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����)
	INT			iCount;
} PMSG_ANS_CSREGGUILDLIST, *LPPMSG_ANS_CSREGGUILDLIST;

typedef struct
{
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// ����̸�
	BYTE		btRegMarks1;				// ������ ǥ���� ���� (INT�� ���� ����Ʈ ���� 1)
	BYTE		btRegMarks2;				// ������ ǥ���� ���� (INT�� ���� ����Ʈ ���� 2)
	BYTE		btRegMarks3;				// ������ ǥ���� ���� (INT�� ���� ����Ʈ ���� 3)
	BYTE		btRegMarks4;				// ������ ǥ���� ���� (INT�� ���� ����Ʈ ���� 4)
	BYTE		btIsGiveUp;					// ���⿩�� (0:�������/1:������)
	BYTE		btSeqNum;					// ��ϼ���
} PMSG_CSREGGUILDLIST, *LPPMSG_CSREGGUILDLIST;


//----------------------------------------------------------------------------
// CG [0xB5] ���� Ȯ�� ����� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_REQ_CSATTKGUILDLIST, *LPPMSG_REQ_CSATTKGUILDLIST;


//----------------------------------------------------------------------------
// GC [0xB5] ���� Ȯ�� ����� ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btResult;					// ��û ��� (0:����/1:����/2:�Ⱓ�ƴ�/3:�������غ�ʵ�)
	INT			iCount;
} PMSG_ANS_CSATTKGUILDLIST, *LPPMSG_ANS_CSATTKGUILDLIST;

typedef struct
{
	BYTE		btCsJoinSide;				// ������ ���� (1:������/2�̻�:������)
	BYTE		btGuildInvolved;			// ��尡 ������ ���� ������� ���� (0:�ƴ�/1:������)
	CHAR		szGuildName[MAX_GUILDNAMESTRING];				// ����̸�
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
	INT			iGuildScore;				// ��� ���� ����� ����
#endif	
} PMSG_CSATTKGUILDLIST, *LPPMSG_CSATTKGUILDLIST;


//----------------------------------------------------------------------------
// GC [0xB6] ������ �߿� ���� ����� �̴ϸ������� ������. (0xC2)
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
// GC [0xBB] ������ �߿� �����ϴ� ����/������ �̴ϸ������� ������. (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	INT			iCount;
} PMSG_ANS_SENDNPCMINIMAPDATA, *LPPMSG_ANS_SENDNPCMINIMAPDATA;

typedef struct
{
	BYTE		btNpcType;		// ������ NPC ���� (0:���� / 1:����)
	BYTE		btX;
	BYTE		btY;
} PMSG_SENDNPCMINIMAPDATA, *LPPMSG_SENDNPCMINIMAPDATA;


#endif


//----------------------------------------------------------------------------
// ���� �������� ( FOR ���� )
//
// Lastest : 20051018
// Related Procol : [0xB2][0x00] - [0xB2][0x1F]
//----------------------------------------------------------------------------
#ifdef MODIFY_CASTLESIEGE_NOTIFY_CROWNSWITCH_USER_20051017

//----------------------------------------------------------------------------
// GC [0xB2][0x20] ���� ������ ���� ���¸� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btIndex1;								// �հ�����ġ�� �ε��� (WORD�� ���� ����Ʈ ���� 1)
	BYTE		btIndex2;								// �հ�����ġ�� �ε��� (WORD�� ���� ����Ʈ ���� 2)
	BYTE		btSwitchState;							// (0:����ġ��/1:����ġ�۵�)
	BYTE		btJoinSide;								// �ش� ���(����) JoinSide(������ȣ)
	BYTE		szGuildName[MAX_GUILDNAMESTRING];		// �ش� ���(����) �̸�
	BYTE		szUserName[MAX_IDSTRING];				// ����ġ ����� �̸�
} PMSG_ANS_NOTIFY_CROWNSWITCH_INFO, *LPPMSG_ANS_NOTIFY_CROWNSWITCH_INFO;

#endif


//----------------------------------------------------------------------------
// New Skill �������� ( FOR ���� )
//
// Lastest : 20041208
// Related Procol : 
//----------------------------------------------------------------------------
typedef struct {}__SKILL_PROTOCOL_H___;

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
// ��å ��ų ��� ����

//----------------------------------------------------------------------------
// GC[0xB8][0x01] ��å ��ų �ߵ� ���� KillCount
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btKillCount;				// Current KillCount
	
} PMSG_KILLCOUNT, *LPPMSG_KILLCOUNT;

//----------------------------------------------------------------------------
// GC[0xB8][0x02] ��å ��ų�� ��� �� �� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btErrorCode;				// Error Code
	BYTE		btInSufficientKillCount;	// InSufficient KillCount
	
} PMSG_INSUFFICIENT_KILLCOUNT, *LPPMSG_INSUFFICIENT_KILLCOUNT;

#endif


//----------------------------------------------------------------------------
// Weapon �ý��� �������� ( FOR ���� )
//
// Lastest : 20041207
// Related Procol : 
//----------------------------------------------------------------------------
typedef struct {}__WEAPON_PROTOCOL_H___;

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
// ���� ���� �������̽� ��������

//----------------------------------------------------------------------------
// CG[0xB7][0x00] Weapon �������̽� ��� ��û ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btResult;				// 0x00 ��� �� �� ����
										// 0x01 ��� �� �� ����
	
	BYTE		btWeaponType;			// ���� ����

	BYTE		btObjIndexH;			// ����ϴ� Weapon�� Object Index
	BYTE		btObjIndexL;
	
} PMSG_ANS_USE_WEAPON_INTERFACE, *LPPMSG_ANS_USE_WEAPON_INTERFACE;

//----------------------------------------------------------------------------
// CG[0xB7][0x01] Weapon ��� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btObjIndexH;			// ����ϴ� Weapon�� Object Index
	BYTE		btObjIndexL;
	
	BYTE		btTargetIndex;			// ���� ������ ���� �̸� ������ �ִ�.

} PMSG_REQ_USEWEAPON, *LPPMSG_REQ_USEWEAPON;

//----------------------------------------------------------------------------
// GC[0xB7][0x01] Weapon ��� ��û�� ���� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btResult;

	BYTE		btObjIndexH;			// ��� ��û�� Weapon�� Object Index
	BYTE		btObjIndexL;

	BYTE		btWeaponType;			// ���� ����

	BYTE		btPointX;				// Ÿ�� ��ġ X
	BYTE		btPointY;				// Ÿ�� ��ġ Y

} PMSG_ANS_USEWEAPON, *LPPMSG_ANS_USEWEAPON;

//----------------------------------------------------------------------------
// GC[0xB7][0x02] Weapon�� Ÿ�� ������ ĳ���͵鿡�� �˸�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btWeaponType;			// ���� ����
	
	BYTE		btPointX;				// ���� ��ġ X
	BYTE		btPointY;				// ���� ��ġ Y

} PMSG_NOTIFY_REGION_OF_WEAPON, *LPPMSG_NOTIFY_REGION_OF_WEAPON;

//----------------------------------------------------------------------------
// GC[0xB7][0x03] Weapon�� Ÿ�� ������ '���ݹ���' ĳ���͵鿡�� �˸�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		NumberH;
	BYTE		NumberL;

	BYTE		btWeaponType;			// ���� ����
	
} PMSG_NOTIFY_TARGET_OF_WEAPON, *LPPMSG_NOTIFY_TARGET_OF_WEAPON;

//----------------------------------------------------------------------------
// CG[0xB7][0x04] Weapon �ִϸ��̼��� �������� �˸�. ������ ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btObjIndexH;			// ����ϴ� Weapon�� Object Index
	BYTE		btObjIndexL;
	
} PMSG_REQ_WEAPON_DAMAGE_VALUE, *LPPMSG_REQ_WEAPON_DAMAGE_VALUE;

#endif // ADD_NEW_WEAPON_FOR_CASTLE_01_20041116


//----------------------------------------------------------------------------
// NEW ITEM �������� ( FOR ���� ), Castle ���� �� ���
//
// Lastest : 20041215	20041227
// Related Procol : 
//----------------------------------------------------------------------------

typedef struct {}__CS_ITEM_PROTOCOL_H___;

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
//----------------------------------------------------------------------------
// GC GC[0xB9][0x01] Life Stone�� ���� ���¸� �����ش�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btObjIndexH;				
	BYTE		btObjIndexL;

	BYTE		btCreationState;		// ���� �Ǿ��°�? ���� ���ΰ�?	
} PMSG_NOTIFY_OBJECT_CREATION_STATE, *LPPMSG_NOTIFY_OBJECT_CREATION_STATE;

#endif

#ifdef ADD_NEW_CASTLE_FLAG_01_20041227
//----------------------------------------------------------------------------
// GC GC[0xB9][0x02] Castle�� ��߿� Mark�� �׸��� �ֵ��� 
//----------------------------------------------------------------------------

typedef struct
{
	PBMSG_HEAD2	h;

} PMSG_REQ_GUILDMARK_OF_CASTLEOWNER, *LPPMSG_REQ_GUILDMARK_OF_CASTLEOWNER;

typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		GuildMarkOfCastleOwner[32];		// ��帶ũ
} PMSG_ANS_GUILDMARK_OF_CASTLEOWNER, *LPPMSG_ANS_GUILDMARK_OF_CASTLEOWNER;

#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
//----------------------------------------------------------------------------
// GC GC[0xB9][0x03] Castle�� ����� �������� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;			// 0x00 ���ְ� ���� �Ұ����� ����	(�Ϲ���)
									// 0x01 ���ְ� ���� ������ ����		(�Ϲ���)
									// 0x02 ���� ���(���տ�)���� ���	(������)
									// 0x03 ���ֿ��� ���				(����)

	BYTE		btUsable;			// 0x00 ���ԺҰ���, 0x01 ���԰���
	INT			iCurrentPrice;		// ���� ����� ������
	INT			iMaxPrice;			// �ִ� ����� ������
	INT			iUnitOfPrice;		// ���� ����� ������

} PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE, *LPPMSG_ANS_GUARD_IN_CASTLE_HUNTZONE;


//----------------------------------------------------------------------------
// GC GC[0xB9][0x05] ����� �����⿡�� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iPrice;				// ����� ������

} PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE, *LPPMSG_REQ_MOVE_TO_CASTLE_HUNTZONE;

typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;			// 0x00 ���ԺҰ���, 0x01 ���԰���

} PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE, *LPPMSG_ANS_MOVE_TO_CASTLE_HUNTZONE;

#endif


//----------------------------------------------------------------------------
// ����Ʈ �������� ( FOR ���� )
//
// Lastest : 20041207
// Related Procol : 0x12, 0x13, 0x65, 0x66, 0x67
//----------------------------------------------------------------------------
typedef struct {}__VIEWPORT_PROTOCOL_H___;

#ifdef ADD_NEW_VIEWPORT_SYSTEM_01_20041201

//----------------------------------------------------------------------------
// GC [0x68] ����Ʈ ���� 
//----------------------------------------------------------------------------

// 1) �����
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btCount;
} PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT, *LPPMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT;

typedef struct
{
	BYTE		btObjType;						// ĳ��, ����, NPC
	BYTE		btObjClassH;					// Ŭ����
	BYTE		btObjClassL;

	BYTE		btObjIndexH;					// ObjIndex
	BYTE		btObjIndexL;

	BYTE		btX;							// Obj Location X
	BYTE		btY;							// Obj Location Y

	// �߾�...gObjMakePreviewSet()
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE		CharSet[MAX_PREVIEWCHARSET+8];	// �� ���� 1����Ʈ (Ŭ���� ����) �� �Ľ��� ������ ����� ������ ��� (��)
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
// GC [0x69] ���Ͱ� ��ų�� ����ߴ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	BYTE		btMonsterSkillNumberH;
	BYTE		btMonsterSkillNumberL;
#else
	BYTE		btMonsterSkillNumber;
#endif
	WORD		wObjIndex;			
	WORD		wTargetObjIndex;
} PMSG_USE_MONSTERSKILL, *LPPMSG_USE_MONSTERSKILL;

#endif // ADD_NEW_MONSTER_SYSTEM_01_20050714


#ifdef JEWEL_MIX_SYSTEM_20050309		// �������� �ý��� �߰���Ŷ

//----------------------------------------------------------------------------
// CG [0xBC][0x00] �������� ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btJewelType;			// ���� Ÿ�� (0:�༮ / 1:����)
	BYTE		btJewelMix;				// ���� Ÿ�� (0:10 / 1:20 / 2:30)
} PMSG_REQ_JEWEL_MIX, *LPPMSG_REQ_JEWEL_MIX;

//----------------------------------------------------------------------------
// CG [0xBC][0x00] �������� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;				// ���		0 : ����
										//			1 : ����
										//			2 : ���� - ����Ÿ�� ����
										//			3 : ���� - ����Ÿ�� ����
										//			4 : ���� - ������ ����
										//			5 : ���� - ���ձ��� ����
} PMSG_ANS_JEWEL_MIX, *LPPMSG_ANS_JEWEL_MIX;

//----------------------------------------------------------------------------
// CG [0xBC][0x01] �������� ��ü ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btJewelType;			// ���� Ÿ�� (0:�༮ / 1:����)
	BYTE		btJewelLevel;			// ���� ���� -> ���� Ÿ�� (0:10 / 1:20 / 2:30)
	BYTE		btJewelPos;				// �κ��丮 ���� �ش� �������� �������� ��ġ
} PMSG_REQ_JEWEL_UNMIX, *LPPMSG_REQ_JEWEL_UNMIX;

//----------------------------------------------------------------------------
// CG [0xBC][0x01] �������� ��ü ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;				// ���		0 : ����
										//			1 : ����
										//			2 : ���� - ����Ÿ�� ����
										//			3 : ���� - �������� ����
										//			4 : ���� - �������� ����
										//			5 : ���� - �κ��丮 ���� ����
										//			6 : ���� - ��ü�� ������ ����
										//			7 : ���� - �κ��丮 ���� ����
										//			8 : ���� - ���ձ��� ����
} PMSG_ANS_JEWEL_UNMIX, *LPPMSG_ANS_JEWEL_UNMIX;

#endif


//----------------------------------------------------------------------------
// ũ���̿��� �������� : 0xBD ���
//
// Lastest : 20050912
// Related Procol : 
//----------------------------------------------------------------------------

#ifdef MU_CRYWOLF_CL_PROTOCOL_20050505
//----------------------------------------------------------------------------
// CG [0xBD][0x00] Ŭ���̾�Ʈ�� ũ���̿��� ���� �� ���������� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
} PMSG_REQ_CRYWOLF_INFO, *LPPMSG_REQ_CRYWOLF_INFO;

//----------------------------------------------------------------------------
// GC [0xBD][0x00] Ŭ���̾�Ʈ�� ũ���̿��� ���� �� �������� ��û�� ���� ���� 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE	btOccupationState;		// ���� ���� ( 0:��ȭ / 1:���� / 2:���� )
	BYTE	btCrywolfState;			// ũ���̿��� ���� ����
	
} PMSG_ANS_CRYWOLF_INFO, *LPPMSG_ANS_CRYWOLF_INFO;


//----------------------------------------------------------------------------
// GC [0xBD][0x02] ��, ���� ���� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	INT		iCrywolfStatueHP;		// �� HP
	
	BYTE	btAltarState1;			// ���� 1 ����
	BYTE	btAltarState2;			// ���� 2 ����
	BYTE	btAltarState3;			// ���� 3 ����
	BYTE	btAltarState4;			// ���� 4 ����
	BYTE	btAltarState5;			// ���� 5 ����

} PMSG_ANS_CRYWOLF_STATUE_ALTAR_INFO, *LPPMSG_ANS_CRYWOLF_STATUE_ALTAR_INFO;


//----------------------------------------------------------------------------
// CG [0xBD][0x03] Ư�� ���ܰ� ���� ��� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btObjIndexH;		// ���� �ε���
	BYTE		btObjIndexL;
	
} PMSG_REQ_CRYWOLF_ALTAR_CONTRACT, *LPPMSG_REQ_CRYWOLF_ALTAR_CONTRACT;


//----------------------------------------------------------------------------
// CG [0xBD][0x03] Ư�� ���ܰ� ���� ��� ��û�� ���� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btResult;			// ��� ��� ( 0: ���� / 1: ���� )

	BYTE		btAltarState;		// ���� ����
	BYTE		btObjIndexH;		// ���� �ε���
	BYTE		btObjIndexL;

} PMSG_ANS_CRYWOLF_ALTAR_CONTRACT, *LPPMSG_ANS_CRYWOLF_ALTAR_CONTRACT;

//----------------------------------------------------------------------------
// GC [0xBD][0x04] ũ���̿��� ���� �ð�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btHour;				// ���� �ð�
	BYTE		btMinute;			// ���� ��

} PMSG_ANS_CRYWOLF_LEFTTIME, *LPPMSG_ANS_CRYWOLF_LEFTTIME;

//----------------------------------------------------------------------------
// GC [0xBD][0x05] ũ���̿��� ���� ���͵� ���� ���� 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	int			iMonster1HP;		// �߰���	HP
	BYTE		btMonster2;			// ������� ������

} PMSG_ANS_CRYWOLF_BOSSMONSTER_INFO, *LPPMSG_ANS_CRYWOLF_BOSSMONSTER_INFO;

//----------------------------------------------------------------------------
// GC [0xBD][0x06] ���� ���� ���θ� �˸���.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btStageEffectOnOff;	// 0x00 �Ұ���, 0x01 ����

} PMSG_ANS_CRYWOLF_STAGE_EFFECT_ONOFF, *LPPMSG_ANS_CRYWOLF_STAGE_EFFECT_ONOFF;

//----------------------------------------------------------------------------
// GC [0xBD][0x07] MVP ���� �� ���� RANK �� ������ �˸���.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btRank;
	int			iGettingExp;

} PMSG_ANS_CRYWOLF_PERSONAL_RANK, *LPPMSG_ANS_CRYWOLF_PERSONAL_RANK;

//----------------------------------------------------------------------------
// GC [0xBD][0x08] MVP ���� �� ���� ����Ʈ ����
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
// CG [0xBD][0x09] ũ���̿��� ���� +ī��������Ȯ��
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
		
} PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE, *LPPMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE;


//----------------------------------------------------------------------------
// CG [0xBD][0x09] ũ���̿��� ���� +ī��������Ȯ��
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btPlusChaosRate;			

} PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE, *LPPMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE;

#endif // MU_CRYWOLF_CL_PROTOCOL_20050505


//----------------------------------------------------------------------------
// New ���� �ý��� �������� : 0xBD ���
//
// Lastest : 20051201
// Related Procol : 0xBD
//----------------------------------------------------------------------------

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
//----------------------------------------------------------------------------
// GC [0xBD][0x0C] ���� ������ �˸���.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btObjClassH;		// ���� ����
	BYTE		btObjClassL;
	
	BYTE		btSourceX;			// �߻� ��ġ Y
	BYTE		btSourceY;			// �߻� ��ġ Y

	BYTE		btPointX;			// ���� ��ġ X
	BYTE		btPointY;			// ���� ��ġ Y

} PMSG_NOTIFY_REGION_MONSTER_ATTACK, *LPPMSG_NOTIFY_REGION_MONSTER_ATTACK;

#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725

#ifdef PCBANG_COUPON_EVENT_20060124

//----------------------------------------------------------------------------
// PC�� ���� ������ ���� �̺�Ʈ�� : 0xD0 ���
//
// Lastest : 20060124
// Related Procol : 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CG [0xD0][0x00] PC�� Ŭ���̾�Ʈ�� ���� ������ ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_PCBANG_COUPON_ITEM, *LPPMSG_REQ_PCBANG_COUPON_ITEM;

//----------------------------------------------------------------------------
// GC [0xD0][0x01] ���� ������ ���� ��û ó�� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_ANS_PCBANG_COUPON_ITEM, *LPPMSG_ANS_PCBANG_COUPON_ITEM;

#endif // PCBANG_COUPON_EVENT_20060124

#ifdef KANTURU_PROTOCOL_20060705		// �������� ����
//----------------------------------------------------------------------------
// ĭ���� �������� : 0xD1 ���
//
// Lastest : 20060705
// Related Procol : 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// GC [0xD1][0x00] ������ ĭ���� �������� ���� ���� ������ ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

} PMSG_REQ_KANTURU_STATE_INFO, *LPPMSG_REQ_KANTURU_STATE_INFO;

//----------------------------------------------------------------------------
// CG [0xD1][0x00] ������ ĭ���� �������� ���� ���� ���� ��û�� ���� ���� ( NPC UI���� )
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	BYTE	btState;		// ĭ���� ������ ����
	BYTE	btDetailState;	// ĭ���� ������ ���� ����

	BYTE	btEnter;		// ���� ���� ���� ( 0:�Ұ� 1:���� )
	BYTE	btUserCount;	// ���� ����( ������, ����Ʈ�޾��� )�� ��� ���� ���� �ο�	
	int		iRemainTime;	// ������ ������ ��� ���� ������� ���� �ð�(Sec)

} PMSG_ANS_KANTURU_STATE_INFO, *LPPMSG_ANS_KANTURU_STATE_INFO;

//----------------------------------------------------------------------------
// CG [0xD1][0x01] ������ ĭ���� ������ �����ʿ� ���� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
} PMSG_REQ_ENTER_KANTURU_BOSS_MAP, *LPPMSG_REQ_ENTER_KANTURU_BOSS_MAP;

//----------------------------------------------------------------------------
// GC [0xD1][0x01] ������ ĭ���� ������ ������ ���� ��û�� ���� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	BYTE		btResult;	// 0:���尡��
							// 1:���� ������ �ο� �� �ʰ�
							// 2:���忡 �ʿ��� �������� ����
							// 3:���尡���� ���°� �ƴ�
							// 4:�� �� ���� �������� ������ �� ����(default)
} PMSG_ANS_ENTER_KANTURU_BOSS_MAP, *LPPMSG_ANS_ENTER_KANTURU_BOSS_MAP;

//----------------------------------------------------------------------------
// GC [0xD1][0x02] ������ ĭ���� ������ �����ʿ� �������� �� ���� ���¸� ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	BYTE		btCurrentState;				// ĭ������ ���� ���� ����
	BYTE		btCurrentDetailState;		// �׿� ���� ���� ����
} PMSG_ANS_KANTURU_CURRENT_STATE, *LPPMSG_ANS_KANTURU_CURRENT_STATE;

//----------------------------------------------------------------------------
// GC [0xD1][0x03] ĭ���� ������ ���� ���°� ������ �� ���°��� �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;	

	BYTE		btState;			// ĭ������ ���� ����
	BYTE		btDetailState;		// �׿� ���� ���� ����	
} PMSG_ANS_KANTURU_STATE_CHANGE, *LPPMSG_ANS_KANTURU_STATE_CHANGE;

//----------------------------------------------------------------------------
// GC [0xD1][0x04] ĭ���� ������ ���� ���� / ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	BYTE		btResult;		// 0:���� 1:����
	
} PMSG_ANS_KANTURU_BATTLE_RESULT, *LPPMSG_ANS_KANTURU_BATTLE_RESULT;

//----------------------------------------------------------------------------
// GC [0xD1][0x05] ĭ���� ������ ���� ���⸶�� ���� �ð�
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	int			btTimeLimit;		// ���� �ð�(Sec)
	
} PMSG_ANS_KANTURU_BATTLE_SCENE_TIMELIMIT, *LPPMSG_ANS_KANTURU_BATTLE_SCENE_TIMELIMIT;

//----------------------------------------------------------------------------
// GC [0xD1][0x06] ĭ���� ������ ���� ����( ���� ��ų ��� )
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btObjClassH;		// ���� ����
	BYTE		btObjClassL;
	
	BYTE		btType;			// 0 : ���̽����� 1:���ū����

} PMSG_NOTIFY_KANTURU_WIDE_AREA_ATTACK, *LPPMSG_NOTIFY_KANTURU_WIDE_AREA_ATTACK;

//----------------------------------------------------------------------------
// GC [0xD1][0x07] ĭ���� ���� ����&���� �� ���÷���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;

	BYTE		btMonsterCount;		// �������� ���� �� ���� ��
	BYTE		btUserCount;		// �������� ���� �� ���� ��
} PMSG_NOTIFY_KANTURU_USER_MONSTER_COUNT, *LPPMSG_NOTIFY_KANTURU_USER_MONSTER_COUNT;
#endif	// KANTURU_PROTOCOL_20060705

#ifdef RAKLION_PROTOCOL_20080625
// ��Ŭ���� �������� : [0xD1][0x10]~ ���
//----------------------------------------------------------------------------
// GC [0xD1][0x10] ������ ��Ŭ���� �������� ���� ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
} PMSG_REQ_RAKLION_STATE_INFO, *LPPMSG_REQ_RAKLION_STATE_INFO;

//----------------------------------------------------------------------------
// CG [0xD1][0x10] ������ ��Ŭ���� �������� ���� ���� ���� ��û�� ���� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	BYTE	btState;		// ��Ŭ���� ������ ����
	BYTE	btDetailState;	// ��Ŭ���� ������ ���� ����
	
	BYTE	btEnter;		// ���� ���� ���� ( 0:�Ұ� 1:���� )
	int		iRemainTime;	// ������ ������ ��� ���� ������� ���� �ð�(Sec)
	
} PMSG_ANS_RAKLION_STATE_INFO, *LPPMSG_ANS_RAKLION_STATE_INFO;

//----------------------------------------------------------------------------
// GC [0xD1][0x11] ������ ��Ŭ���� ������ �����ʿ� �������� �� ���� ���¸� ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	BYTE		btCurrentState;				// ĭ������ ���� ���� ����
	BYTE		btCurrentDetailState;		// �׿� ���� ���� ����
} PMSG_ANS_RAKLION_CURRENT_STATE, *LPPMSG_ANS_RAKLION_CURRENT_STATE;

//----------------------------------------------------------------------------
// GC [0xD1][0x12] ��Ŭ���� ������ ���� ���°� ������ �� ���°��� �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;	
	
	BYTE		btState;			// ��Ŭ������ ���� ����
	BYTE		btDetailState;		// �׿� ���� ���� ����	
} PMSG_ANS_RAKLION_STATE_CHANGE, *LPPMSG_ANS_RAKLION_STATE_CHANGE;

//----------------------------------------------------------------------------
// GC [0xD1][0x13] ��Ŭ���� ������ ���� ���� / ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	BYTE		btResult;		// 0:���� 1:����
	
} PMSG_ANS_RAKLION_BATTLE_RESULT, *LPPMSG_ANS_RAKLION_BATTLE_RESULT;

//----------------------------------------------------------------------------
// GC [0xD1][0x14] ��Ŭ���� ������ ���� ����( ������ ��ų ��� )
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btObjClassH;		// ���� ����
	BYTE		btObjClassL;
	
	BYTE		btType;				// ��ų ����
	
} PMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK, *LPPMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK;

//----------------------------------------------------------------------------
// GC [0xD1][0x15] ��Ŭ���� ���� ����&���� �� ���÷���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	
	BYTE		btMonsterCount;		// �������� ���� �� ���� ��
	BYTE		btUserCount;		// �������� ���� �� ���� ��
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
// ȭ��Ʈ���� ������ ���� �̺�Ʈ�� : 0xD0 ���
//
// Lastest : 20060822
// Related Procol : 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CG [0xD0][0x03] Ŭ���̾�Ʈ�� ȭ��Ʈ���� �̺�Ʈ ������ ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_WHITEANGEL_ITEM, *LPPMSG_REQ_WHITEANGEL_ITEM;
#endif // WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef PCBANG_POINT_SYSTEM_20070206	// Client <-> GS Protocol define
//----------------------------------------------------------------------------
// PC�� ����Ʈ �ý��� : 0xD0 ���
//
// Lastest : 20070212
// Related Procol : 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CG [0xD0][0x04] PC�� ����Ʈ ���� ��û -> �����Ұ�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_CG_PCBANG_POINT_INFO, *LPPMSG_REQ_CG_PCBANG_POINT_INFO;
//----------------------------------------------------------------------------
// GC [0xD0][0x04] PC�� ����Ʈ ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	short		nPoint;			// ���� PC�� ����Ʈ
	short		nMaxPoint;		// �ִ� PC�� ����Ʈ
#ifdef UPDATE_PCBANGPOINTSYSTEM_BUGFIX
	BYTE		bPCBang;
#endif // UPDATE_PCBANGPOINTSYSTEM_BUGFIX
} PMSG_ANS_GC_PCBANG_POINT_INFO, *LPPMSG_ANS_GC_PCBANG_POINT_INFO;
//----------------------------------------------------------------------------
// CG [0xD0][0x05] PC�� ����Ʈ ���� ������ ����/���� ��� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btInvenPos;		// �κ��丮 ��ġ ��ȣ
} PMSG_REQ_CG_BUY_PCBANG_POINT_ITEM, *LPPMSG_REQ_CG_BUY_PCBANG_POINT_ITEM;
//----------------------------------------------------------------------------
// GC [0xD0][0x05] PC�� ����Ʈ ������ ���� ��û ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;		// ��� ��� 0x00:���� 
								//			0x01:�������� �ƴ� 
								//			0x02:�ش���ġ�� �������� ���� 
								//			0x03:�κ��丮 ���� ����
								//			0x04:�⺻ ���� ����Ʈ����
								//			0x05:PC�� ����Ʈ �ý��� ����ڰ� �ƴ�
								//			0x06:����
								//			0x07:���� ��� ���� �ð� �ʰ�
								//			0x08:PC�� ����Ʈ ���� �������� �ƴ�
								//			0x09:���� ������ ����Ʈ ����
	BYTE		btInvenPos;		// �κ��丮 ��ġ
								// *������ ������ �ٷ� �����ϰ� �������� ������ �κ��� ������ �߰��Ѵ�.
	BYTE		ItemInfo[ITEM_BUFFER_SIZE];
} PMSG_ANS_GC_BUY_PCBANG_POINT_ITEM, *LPPMSG_ANS_GC_BUY_PCBANG_POINT_ITEM;
//----------------------------------------------------------------------------
// CG [0xD0][0x06] PC�� ����Ʈ ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_PCBANG_POINT_SHOP_OPEN, *LPPMSG_REQ_PCBANG_POINT_SHOP_OPEN;
//----------------------------------------------------------------------------
// GC [0xD0][0x06] PC�� ����Ʈ ���� ���� ��û ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;	// ���	0x00:���� 
							//		0x01:�������� �ƴ�
							//		0x02:������ ������ ���� 
							//		0x03:�ٸ� â ��� �� 
							//		0x04:PC�� ����Ʈ �ý��� ����ڰ� �ƴ�							
							//		0x05:����
} PMSG_ANS_PCBANG_POINT_SHOP_OPEN, *LPPMSG_ANS_PCBANG_POINT_SHOP_OPEN;
#endif // PCBANG_POINT_SYSTEM_20070206

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// CL <-> GS �������� ����
//----------------------------------------------------------------------------
// CG [0xD0][0x07] 3�� ���� ����Ʈ - ��������� ���� �߰����� ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_ENTER_ON_WERWOLF, *LPPMSG_REQ_ENTER_ON_WERWOLF;
//----------------------------------------------------------------------------
// GC [0xD0][0x07] 3�� ���� ����Ʈ - �߰����� �������� ��û ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;	// 0:������ �� ����
				
} PMSG_ANS_ENTER_ON_WERWOLF, *LPPMSG_ANS_ENTER_ON_WERWOLF;
//----------------------------------------------------------------------------
// CG [0xD0][0x08] 3�� ���� ����Ʈ - ������(����)�� ���� �߰����� �Ƚ�ó ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_ENTER_ON_GATEKEEPER, *LPPMSG_REQ_ENTER_ON_GATEKEEPER;
//----------------------------------------------------------------------------
// GC [0xD0][0x08] 3�� ���� ����Ʈ - �߰����� �Ƚ�ó���� ��û ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;	// 0:������ �� ����
			
} PMSG_ANS_ENTER_ON_GATEKEEPER, *LPPMSG_ANS_ENTER_ON_GATEKEEPER;
#endif	// THIRD_CHANGEUP_SYSTEM_20070507


#ifdef MODIFY_QUEST_SYSTEM_20070525	// CL <-> GS �������� ����
//----------------------------------------------------------------------------
// GC [0xA4][0x00] ������ ���� ų ī��Ʈ ������ �ǵ����ش�.
//----------------------------------------------------------------------------
typedef struct
{ 
	PBMSG_HEAD2	h;
	BYTE		btResult;	// ��� 0x00:����(����)
							//		0x01:���� 
							//		0x02:�̹� �Ϸ��� ����Ʈ
							//		0x03:�������� ����Ʈ �ƴ�
							//		0x04:����Ʈ�� ����(����Ʈ ��ȣ�� �߸�����)
							//		0x05:�Ϸ������� ųī��Ʈ�� �ƴ�
	
	BYTE		btQuestIndex;	
	int			iKillCountInfo[MAX_QUEST_MONSTER_KILL_INFO_CL];	// ���� ųī��Ʈ ����
} PMSG_ANS_QUEST_MONKILL_INFO, *LPPMSG_ANS_QUEST_MONKILL_INFO;
#endif	// MODIFY_QUEST_SYSTEM_20070525

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
//----------------------------------------------------------------------------
// CG [0xBF][0x00] ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE	btIllusionTempleIdx;// ȯ�� ��� ��ȣ
	BYTE	iItemPos;			// ���� �η縶�� ��ġ
} PMSG_REQ_ENTER_ILLUSION_TEMPLE, *LPPMSG_REQ_ENTER_ILLUSION_TEMPLE;

//----------------------------------------------------------------------------
// GC [0xBF][0x00] ���� ��û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		Result;		/* ��û��� (	0 : ����
											1 : �˸��� ���� �η縶�� ����
											2 : ���� �ִ� �ð��� �ƴ�
											3 : ���� �η縶���� ������ �ڽ��� ������ ���� ����
											4 : �ο������� �Ѿ���
											5 : 1�� �������� Ƚ���� �Ѿ���
											6 : ����ᰡ ���ڶ���.
											7 : ī�������� ������ �� ����.
											8 : ���Ź����� �����ϰ� ������ �� ����.
										)
							*/
} PMSG_RESULT_ENTER_ILLUSION_TEMPLE, *LPPMSG_RESULT_ENTER_ILLUSION_TEMPLE;

//----------------------------------------------------------------------------
// GC [0xBF][0x01] ���� ���� / �ð� ���� / ��Ƽ ��ġ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wRemainSec;			// ���ӽ��� �� ������� ���� ��
	WORD		wUserIndex;			// ������ ������ �ִ� ����� �ε���
	BYTE		btX;				// ���� ��ġ X
	BYTE		btY;				// ���� ��ġ Y
	BYTE		btAlliedPoint;		// ���ձ��� ����
	BYTE		btIllusionPoint;	// ȯ�������� ����
	BYTE		btMyTeam;			// 0:���ձ�, 1: ȯ������

	BYTE		btPartyCount;
} PMSG_ILLUSION_TEMPLE_PLAY_STATE, *LPPMSG_ILLUSION_TEMPLE_PLAY_STATE;

typedef struct 
{
	WORD		wPartyUserIndex;			// ����� �ε���
	BYTE		byMapNumber;				// �ʹ�ȣ
	BYTE		btX;						// ��ġ X
	BYTE		btY;						// ��ġ Y
} PMSG_ILLUSION_TEMPLE_PARTY_POS, *LPPMSG_ILLUSION_TEMPLE_PARTY_POS;

//----------------------------------------------------------------------------
// CG [0xBF][0x02]	ȯ�� ��� ���� �������
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	BYTE		MagicNumberH;		// ��ų ��ȣ
	BYTE		MagicNumberL;		// ��ų ��ȣ
#else	
	BYTE		MagicNumber;		
#endif
	BYTE		btTargetObjIndexH;	// Ÿ�� �ε���
	BYTE		btTargetObjIndexL;	// Ÿ�� �ε���
	BYTE		Dis;				// �Ÿ�
} PMSG_ILLUSION_TEMPLE_USE_MAGIC, *LPPMSG_ILLUSION_TEMPLE_USE_MAGIC;

//----------------------------------------------------------------------------
// GC [0xBF][0x02]	ȯ�� ��� ���� ������� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btResult;			// ��� ��� 0: ����, 1: ����
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	BYTE		MagicNumberH;		// ��ų ��ȣ
	BYTE		MagicNumberL;		// ��ų ��ȣ
#else	
	BYTE		MagicNumber;		
#endif
	WORD		wSourceObjIndex;	// �ҽ� �ε���
	WORD		wTargetObjIndex;	// Ÿ�� �ε���
} PMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT, *LPPMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT;

//----------------------------------------------------------------------------
// GC [0xBF][0x03]	ȯ�� ����� ������ �ο����� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btUserCount[6];		// �� ȯ�� ����� �ο���
} PMSG_ILLUSION_TEMPLE_USER_COUNT, *LPPMSG_ILLUSION_TEMPLE_USER_COUNT;

//----------------------------------------------------------------------------
// GC [0xBF][0x04] �̺�Ʈ ��� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	BYTE		btAlliedPoint;		// ���ձ��� ����
	BYTE		btIllusionPoint;	// ȯ�������� ����

	BYTE		btUserCount;		// ĳ���� ��
} PMSG_ILLUSION_TEMPLE_RESULT, *LPPMSG_ILLUSION_TEMPLE_RESULT_;

typedef struct 
{
	char		GameID[MAX_IDSTRING];		// ĳ�����̸�
	BYTE		btMapNumber;				// �ʹ�ȣ
	BYTE		btTeam;						// ��
	BYTE		btClass;					// Ŭ����
	int			nAddExp;					// ���� ����ġ
} PMSG_ILLUSION_TEMPLE_USER_ADD_EXP, *LPPMSG_ILLUSION_TEMPLE_USER_ADD_EXP;

//----------------------------------------------------------------------------
// CG [0xBF][0x05] ȯ�� ������� �¸��� ������ ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2 h;
} PMSG_ILLUSION_TEMPLE_REQ_REWARD, *LPPMSG_ILLUSION_TEMPLE_REQ_REWARD;

//----------------------------------------------------------------------------
// GC [0xBF][0x06] ȯ�� ������� ���͸� �׿��� ��� ��ų ����Ʈ
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	BYTE		btSkillPoint;		// ������ ��ų ����Ʈ
} PMSG_ILLUSION_TEMPLE_SKILL_POINT, *LPPMSG_ILLUSION_TEMPLE_SKILL_POINT;

//----------------------------------------------------------------------------
// GC [0xBF][0x07] ȯ�� ������� ���뽺ų ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	BYTE		MagicNumberH;		// ��ų ��ȣ
	BYTE		MagicNumberL;		// ��ų ��ȣ
#else	
	BYTE		MagicNumber;		
#endif
	WORD		wObjIndex;
} PMSG_ILLUSION_TEMPLE_SKILL_END, *LPPMSG_ILLUSION_TEMPLE_SKILL_END;

//----------------------------------------------------------------------------
// GC [0xBF][0x08] ȯ�� ������� ������ ȹ��
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD2 h;
	WORD		wUserIndex;
	char		Name[MAX_IDSTRING];	
} PMSG_RELICS_GET_USER, *LPPMSG_RELICS_GET_USER;

//----------------------------------------------------------------------------
// GC [0xBF][0x09] ȯ�� ����� ���� ����
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD2 h;
	BYTE		btTempleNumber;
	BYTE		btIllusionTempleState;	// 0: wait, 1: wait->ready, 2: ready->play, 3: play->end, 
} PMSG_ILLUSION_TEMPLE_STATE, *LPPMSG_ILLUSION_TEMPLE_STATE;

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// chainglightning�� ���� ��Ŷ
//----------------------------------------------------------------------------
// GC [0xBF][0x0a] ü�θ��� ����
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD2 h;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	BYTE		MagicNumberH;		// ��ų ��ȣ
	BYTE		MagicNumberL;		// ��ų ��ȣ
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

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410 // ĳ���� ���� ��������
//----------------------------------------------------------------------------
// CG [0xF3][0x15] ĳ���͸� ���� Ȯ�ο�û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	char		Name[MAX_IDSTRING];
} PMSG_REQ_CHECK_CHAGNE_NAME, *LPPMSG_REQ_CHECK_CHAGNE_NAME;

//----------------------------------------------------------------------------
// GC [0xF3][0x15] ĳ���͸� ���� Ȯ�ο�û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	char		Name[MAX_IDSTRING];
	BYTE		btResult;		// 0: ���ʿ�, 1: �ʿ�
} PMSG_ANS_CHECK_CHAGNE_NAME, *LPPMSG_ANS_CHECK_CHAGNE_NAME;


//----------------------------------------------------------------------------
// CG [0xF3][0x16] ĳ���͸� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	char		OldName[MAX_IDSTRING];
	char		NewName[MAX_IDSTRING];
} PMSG_CHAGNE_NAME, *LPPMSG_CHAGNE_NAME;
//----------------------------------------------------------------------------
// GC [0xF3][0x16] ĳ���͸� ���� ��û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	char		OldName[MAX_IDSTRING];
	char		NewName[MAX_IDSTRING];
	BYTE		btResult;		// 0: ����, 1: ���� ĳ���͸� ����
} PMSG_CHAGNE_NAME_RESULT, *LPPMSG_CHAGNE_NAME_RESULT;
#endif	// UPDATE_CHANGE_CHARACTERNAME_20080410

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// �������� ����
//----------------------------------------------------------------------------
// CG [0xD0][0x09] ������ ���� NPC�ٷ� - ������ ���� ��û ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_GIVE_ALANS_ITEM, *LPPMSG_REQ_GIVE_ALANS_ITEM;
#endif // ADD_ITEM_GIVE_NPC_ALAN_20070823


//----------------------------------------------------------------------------
// CG [0xD0][0x0A] ������ ���� X-Max ����� ���̾�α� ��û
//----------------------------------------------------------------------------
#ifdef ADD_NPC_XMASEVENT_20080930
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_SNOWMAN_NPC, *LPPMSG_REQ_SNOWMAN_NPC;
#endif

//----------------------------------------------------------------------------
// CG [0xD0][0x10] ������ ���� X-Max Santa - ������ ���� ��û ����
//----------------------------------------------------------------------------
#ifdef ADD_GIFTITEM_XMASEVENT_20081030
typedef struct
{
	PBMSG_HEAD2	h;
} PMSG_REQ_GIVE_SANTA_ITEM, *LPPMSG_REQ_GIVE_SANTA_ITEM;
#endif // #ifdef ADD_GIFTITEM_XMASEVENT_20081030


#ifdef ADD_CHARCARD_SUMMONER_20080313
//----------------------------------------------------------------------------
// [0xDE][0x00] ���� ĳ�������� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		bSummoner;	// ��ȯ���� ĳ���� ī�� �̿� ��/��
} PMSG_ANS_ACCOUNTINFO, *LPPMSG_ANS_ACCOUNTINFO;

//----------------------------------------------------------------------------
// [0xDE][0x01] ���� ĳ�������� ���� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		Result;
} PMSG_ANS_SETACCOUNTINFO, *LPPMSG_ANS_SETACCOUNTINFO;
#endif // ADD_CHARCARD_SUMMONER_20080313

//----------------------------------------------------------------------------
// [0xF2][0x00] �����ʱ�ȭ ��ư ������.
//----------------------------------------------------------------------------
#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
// CS [0xF2] �����ʱ�ȭ ��ư ������.
typedef struct
{
	PBMSG_HEAD	h;
}PMSG_REQ_STAT_RESET, *LPPMSG_REQ_STAT_RESET;
#endif // EXPERIENCE_SERVER_NPC_STAT_RESET

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� �̺�Ʈ ��������
//----------------------------------------------------------------------------
// CG[0xBF][0x0b] ��ϵ� ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
}PMSG_REQ_GET_COIN_COUNT, *LPPMSG_REQ_GET_COIN_COUNT;

//----------------------------------------------------------------------------
// GC[0xBF][0x0b] ��ϵ� ���� ���� ���� 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	int			nCoinCnt;					// ��ϵ� ���� ����
}PMSG_ANS_GET_COIN_COUNT, *LPPMSG_ANS_GET_COIN_COUNT;

//----------------------------------------------------------------------------
// CG[0xBF][0x0c] ������� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
}PMSG_REQ_REGEIST_COIN, *LPPMSG_REQ_REGEIST_COIN;

//----------------------------------------------------------------------------
// GC[0xBF][0x0c] ���� ��� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	BYTE		btResult;					// 0: ���� ���� ����, 1: ����
	int			nCurCoinCnt;				// ���� ��ϵ� ���� ����	
}PMSG_ANS_REGEIST_COIN, *LPPMSG_ANS_REGEIST_COIN;

//----------------------------------------------------------------------------
// CG[0xBF][0x0d] ���� ��ȯ ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	int			nCoinCnt;	// ��ȯ ����
}PMSG_REQ_TRADE_COIN, *LPPMSG_REQ_TRADE_COIN;

//----------------------------------------------------------------------------
// GC[0xBF][0x0d] ���� ��ȯ ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2 h;
	BYTE		btResult;	// 0: ���� ���� ����, 1: ����, 2: ��������
}PMSG_ANS_TRADE_COIN, *LPPMSG_ANS_TRADE_COIN;

#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_HACKING_TOOL_BLOCK_20090311

#include "HackToolBlock.h"

//----------------------------------------------------------------------------
// GC[0x8A] ��ü �����ڿ��� �ش� ���α׷��� ��������� Ȯ�� ��I
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	DWORD		dwKey;
	char		Filename[MAX_HACKTOOL_FILENAME_LENGTH];
}PMSG_REQ_HACKTOOL_STATISTICS, *LPPMSG_REQ_HACKTOOL_STATISTICS;

//----------------------------------------------------------------------------
// CG[0x8A] Ŭ���̾�Ʈ�� ��ŷ ���α׷� ��� ���ο� ���� ����
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD2	h;
	
	DWORD		dwKey;
	BYTE		btResult;			// ��� ( 0 : ������� ����, 1 : ��� �� )
}PMSG_ANS_HACKTOOL_STATISTICS, *LPPMSG_ANS_HACKTOOL_STATISTICS;

#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
//----------------------------------------------------------------------------
// CG[0x8E][0x01] �� �̵� äũ�� �����͸� �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	DWORD		dwKeyValue;
}PMSG_MAPMOVE_CHECKSUM, *LPPMSG_MAPMOVE_CHECKSUM;

//----------------------------------------------------------------------------
// CG[0x8E][0x02] Ŭ���̾�Ʈ�� �� �̵� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	
	DWORD		dwBlockKey;
	WORD		wMapIndex;
}PMSG_REQ_MAPMOVE, *LPPMSG_REQ_MAPMOVE;

//----------------------------------------------------------------------------
// CG[0x8E][0x03] Ŭ���̾�Ʈ�� �� �̵� ���
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

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
// GC [0x69] ���Ͱ� ��ų�� ����ߴ�.
extern void GCUseMonsterSkillSend( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, INT iSkillNumber );
#endif

#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807	// type�� BYTE ���� WORD�� ����
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

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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

// ������
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


#ifdef DARKLORD_WORK	// ī���� ���� â�� ���� ���� iChaosBoxType == 0 : ���� ����â 
						//							  iChaosBoxType == 1 : ���û� ����â
extern void GCUserChaosBoxSend(LPOBJECTSTRUCT lpObj, int iChaosBoxType = 0);
#else
extern void GCUserChaosBoxSend(LPOBJECTSTRUCT lpObj);
#endif

// ���� ��ɾ ������
extern void GCServerCmd(int aIndex, BYTE type, BYTE Cmd1, BYTE Cmd2);
extern void GCReqDevilSquareRemainTime(LPPMSG_REQ_DEVILSQUARE_REMAINTIME lpMsg, int aIndex);


// ��� ����ڿ��� �޽����� ������
extern void AllSendMsg(LPBYTE Msg, int size);
// ���� �ʿ� �մ� ����ڿ��� �޽����� ������
extern void AllSendSameMapMsg(LPBYTE Msg, int size, BYTE mapnumber);

extern void GCSendPing(int aIndex);
extern void GCWarehouseStateSend(int aIndex, BYTE state);

// ����Ʈ
#ifdef NEW_SKILL_FORSKYLAND
// ����Ʈ ������ ����ڿ� ������
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
// [0xAA] ���������� ��û�ߴ�.
extern void CGDuelStartRequestRecv(LPPMSG_REQ_START_DUEL lpMsg, int aIndex);
// [0xAB] �������Ḧ ��û�ߴ�.
extern void CGDuelEndRequestRecv(LPPMSG_REQ_END_DUEL lpMsg, int aIndex);
// [0xAC] ������û�� ���� Ȯ�ο��θ� �����Դ�.
extern void CGDuelOkRequestRecv(LPPMSG_ANS_DUEL_OK lpMsg, int aIndex);
// [0xAD] ���� ������ �������� ������
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

#ifdef MU_CASTLESIEGE_CL_PROTOCOL_20041122	// �� ������ - �� Ŭ���̾�Ʈ ���� �������� �Լ� ����
#include "Include\Sprodef.h"

//----------------------------------------------------------------------------
// CG [0xB2][0x00] Ŭ���̾�Ʈ�� ������ ���������� ��û
//----------------------------------------------------------------------------
extern void CGReqCastleSiegeState		(LPPMSG_REQ_CASTLESIEGESTATE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x00] Ŭ���̾�Ʈ�� ������ ���������� ��û ����
//----------------------------------------------------------------------------
extern void GCAnsCastleSiegeState		(INT iIndex, INT iResult, LPSTR lpszGuildName,  LPSTR lpszGuildMaster);
//----------------------------------------------------------------------------
// CG [0xB2][0x01] ������ ����� ��û
//----------------------------------------------------------------------------
extern void CGReqRegCastleSiege			(LPPMSG_REQ_REGCASTLESIEGE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x01] ������ ����� ��û ����
//----------------------------------------------------------------------------
extern void GCAnsRegCastleSiege			(INT iIndex, INT iResult, LPSTR lpszGuildName);
//----------------------------------------------------------------------------
// CG [0xB2][0x02] ������ ���⸦ ��û
//----------------------------------------------------------------------------
extern void CGReqGiveUpCastleSiege		(LPPMSG_REQ_GIVEUPCASTLESIEGE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x02] ������ ���⸦ ��û ����
//----------------------------------------------------------------------------
extern void GCAnsGiveUpCastleSiege		(INT iIndex, INT iResult, BOOL bGiveUp, INT iMarkCount, LPSTR lpszGuildName);
//----------------------------------------------------------------------------
// CG [0xB2][0x03] �ڽ��� ����� ������Ͽ� ���� ������ ��û
//----------------------------------------------------------------------------
extern void CGReqGuildRegInfo			(LPPMSG_REQ_GUILDREGINFO lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x03] �ڽ��� ����� ������Ͽ� ���� ������ ��û ����
//----------------------------------------------------------------------------
extern void GCAnsGuildRegInfo			(INT iIndex, INT iResult, LPCSP_ANS_GUILDREGINFO lpMsgResult);
//----------------------------------------------------------------------------
// CG [0xB2][0x04] �ڽ��� ����� ������ ǥ�� ��� ��û
//----------------------------------------------------------------------------
extern void CGReqRegGuildMark			(LPPMSG_REQ_REGGUILDMARK lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x04] �ڽ��� ����� ������ ǥ�� ��� ��û ����
//----------------------------------------------------------------------------
extern void GCAnsRegGuildMark			(INT iIndex, INT iResult, LPCSP_ANS_GUILDREGMARK lpMsgResult);
//----------------------------------------------------------------------------
// CG [0xB2][0x05] ������ NPC ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
extern void CGReqNpcBuy					(LPPMSG_REQ_NPCBUY lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x05] ������ NPC ���� ��û ����
//----------------------------------------------------------------------------
extern void GCAnsNpcBuy					(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex);
//----------------------------------------------------------------------------
// CG [0xB2][0x06] ������ NPC ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
extern void CGReqNpcRepair				(LPPMSG_REQ_NPCREPAIR lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x06] ������ NPC ���� ��û ����
//----------------------------------------------------------------------------
extern void GCAnsNpcRepair				(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex, INT iNpcHP, INT iNpcMaxHP);
//----------------------------------------------------------------------------
// CG [0xB2][0x07] ������ NPC ���׷��̵� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
extern void CGReqNpcUpgrade				(LPPMSG_REQ_NPCUPGRADE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x07] ������ NPC ���׷��̵� ��û ����
//----------------------------------------------------------------------------
extern void GCAnsNpcUpgrade				(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex, INT iNpcUpType, INT iNpcUpValue);
//----------------------------------------------------------------------------
// CG [0xB2][0x08] ���� ���� �ڱ�, ���� ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
extern void CGReqTaxMoneyInfo				(LPPMSG_REQ_TAXMONEYINFO lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x08] ���� ���� �ڱ�, ���� ���� ��û ����
//----------------------------------------------------------------------------
extern void GCAnsTaxMoneyInfo				(INT iIndex, INT iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, INT64 i64Money);
//----------------------------------------------------------------------------
// CG [0xB2][0x09] ���� ���� ���� ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
extern void CGReqTaxRateChange				(LPPMSG_REQ_TAXRATECHANGE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x09] ���� ���� ���� ���� ��û ����
//----------------------------------------------------------------------------
extern void GCAnsTaxRateChange				(INT iIndex, INT iResult, BYTE btTaxType, INT iTaxRate);
//----------------------------------------------------------------------------
// CG [0xB2][0x10] ���� ���� �ڱ� ��� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
extern void CGReqMoneyDrawOut				(LPPMSG_REQ_MONEYDRAWOUT lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x10] ���� ���� �ڱ� ��� ��û ����
//----------------------------------------------------------------------------
extern void GCAnsMoneyDrawOut				(INT iIndex, INT iResult, INT64 i64Money);
//----------------------------------------------------------------------------
// GC [0xB2][0x11] ���� ������ ����� �������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsGateState				(INT iIndex, INT iResult, INT iGateIndex);
//----------------------------------------------------------------------------
// CG [0xB2][0x12] Ư�� ������ ���� ������� ��û (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsGateOperate				(LPPMSG_REQ_CSGATEOPERATE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x12] Ư�� ������ ���� ������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsGateOperate				(INT iIndex, INT iResult, INT iGateIndex, INT iGateOperate);
//----------------------------------------------------------------------------
// GC [0xB2][0x13] Ư�� ������ ���� ���������� ������ (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsGateCurState				(INT iIndex, INT iGateIndex, INT iGateOperate);
//----------------------------------------------------------------------------
// GC [0xB2][0x14] Ư�� ������� �հ� ����ġ �۵����θ� �˸� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsAccessSwitchState		(INT iIndex, INT iSwitchIndex, INT iSwitchUserIndex, BYTE btSwitchState);
//----------------------------------------------------------------------------
// GC [0xB2][0x15] Ư�� ������� �հ� ���ε�� ���θ� �˸� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsAccessCrownState			(INT iIndex, BYTE btCrownState);
//----------------------------------------------------------------------------
// GC [0xB2][0x17] ���� ������ ���� ���θ� �˸� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsNotifyStart				(INT iIndex, BYTE btStartState);
//----------------------------------------------------------------------------
// GC [0xB2][0x18] ���� ������ ���� ���¸� �˸� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsNotifyProgress			(INT iIndex, BYTE btCastleSiegeState, LPSTR lpszGuildName);
//----------------------------------------------------------------------------
// GC [0xB2][0x1A] ���� �ʼ������� ����� ������ �˸� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsMapSvrTaxInfo			(INT iIndex, BYTE btTaxType, BYTE btTaxRate);
//----------------------------------------------------------------------------
// CG [0xB2][0x1B] �̴ϸ� �����͸� �ֱ������� ������ ���� ��û (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsMiniMapData				(LPPMSG_REQ_MINIMAPDATA lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x1B] �̴ϸ� �����͸� �ֱ������� ������ ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsMiniMapData				(INT iIndex, BYTE btResult);
//----------------------------------------------------------------------------
// CG [0xB2][0x1C] �̴ϸ� �����͸� ���̻� ������ ����� �˸� (0xC1)
//----------------------------------------------------------------------------
extern void CGReqStopCsMiniMapData			(LPPMSG_REQ_STOPMINIMAPDATA lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// CG [0xB2][0x1D] ������ ������� �渶�� ����鿡�� ���� ���� (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsSendCommand				(LPPMSG_REQ_CSCOMMAND lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x1D] ������ ������� �渶�� ����鿡�� ���� ���� ���� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsSendCommand				(INT iCsJoinSide, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand);
//----------------------------------------------------------------------------
// GC [0xB2][0x1E] ������ �� ���� �ð��� ����ڵ鿡�� �ֱ������� �˸� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsLeftTimeAlarm			(BYTE btHour, BYTE btMinute);
//----------------------------------------------------------------------------
// GC [0xB2][0x1E] ������ �� ���� �ð��� ����ڵ鿡�� �ֱ������� �˸� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsSelfCsLeftTimeAlarm		(INT iIndex, BYTE btHour, BYTE btMinute);
//----------------------------------------------------------------------------
// CG [0xB2][0x1F] �� ����� ���� ���ɿ��� ���� ��û (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsSetEnterHuntZone			(LPPMSG_REQ_CSHUNTZONEENTER lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x1F] �� ����� ���� ���ɿ��� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsCsSetEnterHuntZone			(INT iIndex, BYTE btResult, BYTE btEnterHuntZone);
//----------------------------------------------------------------------------
// CG [0xB3] �������� DB�� �����Ǵ� Ư�� NPC�� ����� ��û -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
extern void CGReqNpcDbList					(LPPMSG_REQ_NPCDBLIST lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// CG [0xB4] ���� ��� ����� ��û (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsRegGuildList				(LPPMSG_REQ_CSREGGUILDLIST lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// CG [0xB5] ���� ���� ����� ��û (0xC1)
//----------------------------------------------------------------------------
extern void CGReqCsAttkGuildList			(LPPMSG_REQ_CSATTKGUILDLIST lpMsg, INT iIndex);
#endif


#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
//----------------------------------------------------------------------------
// CG[0xB7][0x01] Weapon ��� ��û
//----------------------------------------------------------------------------
extern void CGReqWeaponUse( LPPMSG_REQ_USEWEAPON aRecv, INT iIndex );
extern void CGReqWeaponDamageValue( LPPMSG_REQ_WEAPON_DAMAGE_VALUE aRecv, INT iIndex );

#endif


#ifdef ADD_NEW_VIEWPORT_SYSTEM_01_20041201
//----------------------------------------------------------------------------
// GC [0x68] ���� ���� ����Ʈ ���� 
//
//----------------------------------------------------------------------------


#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
//----------------------------------------------------------------------------
// GC GC[0xB9][0x01] Life Stone�� ���� ���¸� �����ش�.
//----------------------------------------------------------------------------
extern void GCSendObjectCreationState( INT iObjectIndex );

#endif

#ifdef ADD_NEW_CASTLE_FLAG_01_20041227
//----------------------------------------------------------------------------
// GC GC[0xB9][0x02] ���� ����� ��ũ�� �����ش�.
//----------------------------------------------------------------------------
extern void CGReqGuildMarkOfCastleOwner( LPPMSG_REQ_GUILDMARK_OF_CASTLEOWNER aRecv, INT iIndex );
#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
//----------------------------------------------------------------------------
// GC GC[0xB9][0x04] Castle�� ����� �����⿡�� '����'�� ���� ���� ��û
//----------------------------------------------------------------------------
//extern void CGReqSettingOfCastleHuntZoneEntrance( LPPMSG_REQ_SETTING_OF_CASTLE_HUNTZONE aRecv, INT iIndex );

//----------------------------------------------------------------------------
// GC GC[0xB9][0x05] ����� �����⿡�� ���� ��û
//---------------------------------------------------------------------------
extern void CGReqCastleHuntZoneEntrance( LPPMSG_REQ_MOVE_TO_CASTLE_HUNTZONE aRecv, INT iIndex );

#endif

#ifdef JEWEL_MIX_SYSTEM_20050309			// �������� �ý��� �߰���Ŷ
//----------------------------------------------------------------------------
// CG [0xBC][0x00] �������� ���� ��û (0xC1)
//----------------------------------------------------------------------------
extern void CGReqJewelMix					(LPPMSG_REQ_JEWEL_MIX lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xBC][0x00] �������� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsJewelMix					(INT iIndex, INT iResult);
//----------------------------------------------------------------------------
// CG [0xBC][0x01] �������� ��ü ��û (0xC1)
//----------------------------------------------------------------------------
extern void CGReqJewelUnMix					(LPPMSG_REQ_JEWEL_UNMIX lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xBC][0x01] �������� ��ü ��û ���� (0xC1)
//----------------------------------------------------------------------------
extern void GCAnsJewelUnMix					(INT iIndex, INT iResult);
#endif


#ifdef MU_CRYWOLF_CL_PROTOCOL_20050505	 // ũ���̿��� �������� ó�� �Լ� ����

//----------------------------------------------------------------------------
// CG [0xBD][0x00] Ŭ���̾�Ʈ�� ũ���̿��� ���� �� ���������� ��û
//----------------------------------------------------------------------------
extern void CGReqCrywolfInfo( LPPMSG_REQ_CRYWOLF_INFO lpMsg, int iIndex );


//----------------------------------------------------------------------------
// GC [0xBD][0x00] Ŭ���̾�Ʈ�� ũ���̿��� ���� �� ���������� ��û�� ���� ���� 
//----------------------------------------------------------------------------
extern void GCAnsCrywolfInfo( int iIndex, BYTE btOccupationState, BYTE btCrywolfState );


//----------------------------------------------------------------------------
// CG [0xBD][0x03] Ư�� ���ܰ� ���� ��� ��û
//----------------------------------------------------------------------------
extern void CGReqAlatrContract( LPPMSG_REQ_CRYWOLF_ALTAR_CONTRACT lpMsg, int iIndex );


//----------------------------------------------------------------------------
// CG [0xBD][0x09] ũ���̿��� ���� +ī��������Ȯ�� ��û
//----------------------------------------------------------------------------
extern void CGReqPlusChaosRate( LPPMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE lpMsg, int iIndex );

#endif


#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

//----------------------------------------------------------------------------
// CG [0xBF][0x00] ���� ��û
//----------------------------------------------------------------------------
extern void CGReqEnterIllusionTemple( LPPMSG_REQ_ENTER_ILLUSION_TEMPLE lpMsg, int iIndex );

//----------------------------------------------------------------------------
// CG [0xBF][0x02]	ȯ�� ��� ���� �������
//----------------------------------------------------------------------------
extern void CGIllusionTempleUseMagic( LPPMSG_ILLUSION_TEMPLE_USE_MAGIC lpMsg, int iIndex );

//----------------------------------------------------------------------------
// GC [0xBF][0x06] ȯ�� ������� ���͸� �׿��� ��� ��ų ����Ʈ
//----------------------------------------------------------------------------
extern void GCIllusionTempleSkillPoint(int iIndex, BYTE btSkillPoint);

//----------------------------------------------------------------------------
// CG [0xBF][0x05] ȯ�� ������� �¸��� ������ ��û
//----------------------------------------------------------------------------
extern void CGReqIllusionTempleReward( LPPMSG_ILLUSION_TEMPLE_REQ_REWARD lpMsg, int aIndex );

extern void CGPartyListAll(int pnumber);
extern void GCPartyDelUserSend(int aIndex);

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� �̺�Ʈ �������� �Լ�
//----------------------------------------------------------------------------
// CG[0xBF][0x0b] ��ϵ� ���� ���� ��û
//----------------------------------------------------------------------------
extern void CGReqGetCoinCount(LPPMSG_REQ_GET_COIN_COUNT lpMsg, int iIndex);

//----------------------------------------------------------------------------
// GC[0xBF][0x0b] ��ϵ� ���� ���� ���� 
//----------------------------------------------------------------------------
extern void GCAnsGetCoinCount(int iIndex, int nCoinCnt);

//----------------------------------------------------------------------------
// CG[0xBF][0x0c] ������� ��û
//----------------------------------------------------------------------------
extern void CGReqRegeistCoin(LPPMSG_REQ_REGEIST_COIN lpMsg, int iIndex);

//----------------------------------------------------------------------------
// GC[0xBF][0x0c] ���� ��� ����
//----------------------------------------------------------------------------
extern void GCAnsRegeistCoin(int iIndex, BYTE btResult, int nCoinCnt, BYTE btPos);

//----------------------------------------------------------------------------
// CG[0xBF][0x0d] ���� ��ȯ ��û
//----------------------------------------------------------------------------
extern void CGReqTradeCoin(LPPMSG_REQ_TRADE_COIN lpMsg, int iIndex);

//----------------------------------------------------------------------------
// GC[0xBF][0x0d] ���� ��ȯ ����
//----------------------------------------------------------------------------
extern void GCANSTradeCoin(int iIndex, BYTE btResult);

#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
//----------------------------------------------------------------------------
// GC[0x8A] Ư�� ���α׷��� ��� ���� Ȯ���� ��û�Ѵ�.
//----------------------------------------------------------------------------
extern void GCReqHacktoolStatistics(char* pchFilename, DWORD dwKey);

//----------------------------------------------------------------------------
// GC[0x8A] Ư�� ���α׷��� ��� ���� ����� Ȯ���Ѵ�.
//----------------------------------------------------------------------------
extern void CGAnsHacktoolStatistics(LPPMSG_ANS_HACKTOOL_STATISTICS lpMsg, int iIndex);
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
extern void CGReqMapMove(LPPMSG_REQ_MAPMOVE lpMsg, int iIndex);
extern void GCSendMapMoveChecksum(LPOBJECTSTRUCT lpObj);
#endif // ADD_MAPMOVE_PROTOCOL_20090327