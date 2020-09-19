#ifndef __SPRODEF_H__
#define __SPRODEF_H__

#define VER_CHATWINDOW_OPTION

//#define PROPERTY_ITEM_WORK		// ���������� �� �ִ� ������

// Server Header Protocol BYTE(Size)
typedef struct
{
	BYTE		c;
	BYTE		size;
	BYTE		headcode;
	BYTE		ServerCode;
	BYTE		Subcode;
} SHPB_HEAD, *LPSHPB_HEAD;

// Server Header Protocol WORD(Size)
typedef struct
{
	BYTE		c;
	BYTE		sizeH;
	BYTE		sizeL;
	BYTE		headcode;
	BYTE		ServerCode;
	BYTE		Subcode;
} SHPW_HEAD, *LPSHPW_HEAD;

//----------------------------------------------------------------------------
// ��� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE	Result;
} SDHP_DEFAULT_RESULT, *LPSDHP_DEFAULT_RESULT;


//----------------------------------------------------------------------------
// ����� ������ ��� 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	DWORD		ItemCount;
} SDHP_RESULT, *LPSDHP_RESULT;

//----------------------------------------------------------------------------
// [0x00] ������ ���� ������ ������ ���
//----------------------------------------------------------------------------
typedef struct 
{	
	PBMSG_HEAD	h;
		
	BYTE			Type;		// 0:Join Server, 1:MapServer
	unsigned short	Port;	// Server Port Number
	char			ServerName[50];
	
#ifdef SERVERCODE_EXPAND
	WORD			ServerCode;
#else
	BYTE			ServerCode;
#endif
} SDHP_SERVERINFO, * LPSDHP_SERVERINFO;

//----------------------------------------------------------------------------
// [0x20] ���Ӽ����� ������ �����ͼ����� ������ �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	DWORD		ItemCount;
} SDHP_GAMESERVERINFO, *LPSDHP_GAMESERVERINFO;


//----------------------------------------------------------------------------
// �߾Ӽ����� ������ ���������� ������.
//----------------------------------------------------------------------------
typedef struct 
{	
	PBMSG_HEAD	h;
	BYTE		ServerCode;
	BYTE		Subcode;
	
	int			Number;
	char		AccountID[MAX_IDSTRING+1];// ���� ���̵�

} SDHP_ACCOUNTINFO, * LPSDHP_ACCOUNTINFO;


//----------------------------------------------------------------------------
// [0x01] ���Ӽ����� ���� ������ ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	char		Id[MAX_IDSTRING];
#ifdef PW_KUZIMO
	char		Pass[MAX_IDSTRING];//[MAX_PASSWORD_LENGTH];
#else
	char		Pass[MAX_IDSTRING];
#endif
	short		Number;
	char		IpAddress[17];
} SDHP_IDPASS, *LPSDHP_IDPASS;

//----------------------------------------------------------------------------
// [0x01] ���� ���� ��û���.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		result;
	
	short		Number;
	char		Id[MAX_IDSTRING];
	int			UserNumber;
	int			DBNumber;
	char		JoominNumber[13];	// �ֹι�ȣ ���ڸ�

#ifdef FOR_THAILAND
	char		AuthAdult;			// ���� ���� �ڵ�
#endif

#ifdef ADD_PROTOCOL_PCBANG_GUID
	int			PcBangGuid;
#endif

#ifdef PCBANG_ADVANTAGE
	BYTE		bUserInPCBang;
#endif
	
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	int			iVerifiedType;		// ���� ���� ���� ( 0 : ����, 1 : �̼���, 2 : ������� ����(����) )
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	INT			iAddPlayTime;		// �߱� �Ƿε� - ���ӽð� (5�ð� ������ �������� 0%)
	INT			iAddRestTime;		// �߱��߰�> . �߱� �Ƿε� - �޽Ľð�
#endif
	
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	INT			iAddPlayTime;		
	INT			iAddRestTime;		
#endif

} SDHP_IDPASSRESULT, *LPSDHP_IDPASSRESULT;


//----------------------------------------------------------------------------
// [0x02] ���� ���� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	short		Number;
	char		Id[MAX_IDSTRING+1];
	int			UserNumber;
	int			DBNumber;
} SDHP_JOINFAIL, * LPSDHP_JOINFAIL;

//----------------------------------------------------------------------------
// [0x04] ����� �� ��Ų��.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	short		Number;
	char		Id[MAX_IDSTRING+1];
	int			UserNumber;
	int			DBNumber;
	BYTE		BlockCode;
} SDHP_COMMAND_BLOCK, * LPSDHP_COMMAND_BLOCK;


//----------------------------------------------------------------------------
// �������� [0x06] ���� �ð� Ȯ�� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	char		Id[MAX_IDSTRING];
	short		Number;
} SDHP_BILLSEARCH, * LPSDHP_SDHP_BILLSEARCH;

//----------------------------------------------------------------------------
// �������� [0x06] ���� �ð� Ȯ�� ��û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	char		Id[MAX_IDSTRING];
	short		Number;

#ifdef UPDATE_UNIFICATIONBILL_20090203
	BYTE		btDeductType;
	char        EndsDays[12];
	
	DWORD		dwRestPoint;
	DWORD		dwRestTime;
#else	// UPDATE_UNIFICATIONBILL_20090203	
	BYTE		cCertifyType;
	BYTE		PayCode;
	char        EndsDays[12];
	int			EndTime;
#endif	// UPDATE_UNIFICATIONBILL_20090203
} SDHP_BILLSEARCH_RESULT, * LPSDHP_SDHP_BILLSEARCH_RESULT;


//0x07  �ð� ����� ���ؼ� ������ ���� ��Ų��.
typedef struct
{
	PBMSG_HEAD	h;
	
	char		Id[MAX_IDSTRING];
	short		Number;

} SDHP_BILLKILLUSER, *LPSDHP_BILLKILLUSER;


// 0x08 �ٸ� ����ڰ� ������ �õ��Ѵٰ� �˷��ش�.
typedef struct
{
	PBMSG_HEAD  h;	
	char    AccountID[MAX_IDSTRING];
} SDHP_OTHERJOINMSG, *LPSDHP_OTHERJOINMSG;

// 0x20
typedef struct
{
	PBMSG_HEAD  h;	
	BYTE ExitCode[3];
} SDHP_EXITMSG, *LPSDHP_EXITMSG;


//----------------------------------------------------------------------------
// [0x51] �̺�Ʈ ZEN������ ������ �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		x;
	BYTE		y;
	BYTE		MapNumber;
} SDHP_LOVEHEARTCREATE, *LPSDHP_LOVEHEARTCREATE;

//----------------------------------------------------------------------------
// [0x51] �̺�Ʈ ZEN������ ������ �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
} SDHP_LOVEHEARTCREATERECV, *LPSDHP_LOVEHEARTCREATERECV;


//----------------------------------------------------------------------------
// DB ������ ĳ���� ����Ʈ�� �޶�� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	//int			UserNumber;	// ���� ���� ��ȣ 
	char		Id[MAX_IDSTRING];
	short		Number;	// ���� ���� ������ȣ	
} SDHP_GETCHARLIST, *LPSDHP_GETCHARLIST;

//----------------------------------------------------------------------------
// ĳ���� ����Ʈ ī��Ʈ  (2����Ʈ ũ�⸦ ���)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	short		Number;
	BYTE		Count;
	int			DbNumber;
	BYTE		Magumsa;		// ���˻縦 ����� �ִ� ��������..
	char		AccountId[MAX_IDSTRING+1];
	
#ifdef CHARACTER_MOVE_20040810
	BYTE		MoveCnt;
#endif
} SDHP_CHARLISTCOUNT, *LPSDHP_CHARLISTCOUNT;

//----------------------------------------------------------------------------
// [0x02] ���� ���� ��û�� ĳ���� ����(ī���� ��ŭ �ݺ���)
// [0xF3][0x00]
//----------------------------------------------------------------------------
typedef struct
{
	BYTE Index;
	char Name[MAX_IDSTRING];
	WORD Level;
	BYTE Class;
	BYTE CtlCode;
	//BYTE dbInventory[24];
	//BYTE dbInventory[36];
	BYTE dbInventory[48]; // #define ITEM_DBSIZE_EXTEND_20050706
	BYTE DbVersion;
		
#ifdef ADD_CHARACTERLIST_GUILD_STATUS_01_20050126	
	BYTE btGuildStatus;	// ��� ���� ����. �ϴ� Status ������ �߰�.
#endif

} SDHP_CHARLIST, * LPSDHP_CHARLIST;


//----------------------------------------------------------------------------
// [0x02] ���� ���� ���� �޽���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char CharName[MAX_IDSTRING];
	char GuildName[MAX_GUILDNAMESTRING];
#ifdef __FRIEND_WORK__
	BYTE Type;			// 0x00 : ���&ģ��, 0x01 : ģ����
#endif

} SDHP_USERCLOSE, *LPSDHP_USERCLOSE;

/*
//----------------------------------------------------------------------------
// [0x03] ���� ���� ���� �޽���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	int  UserNumber;
	int  DBNumber;
} SDHP_USERCLOSE, *LPSDHP_USERCLOSE;
*/
//----------------------------------------------------------------------------
// [0x04] ĳ���� ����� �� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	int         UserNumber;
	int			DBNumber;
	short		Number;
	char        AccountId[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	BYTE		ClassSkin;
} SDHP_CREATECHAR, *LPSDHP_CREATECHAR;

//----------------------------------------------------------------------------
// [0x04] ĳ���� ����� �� ��� �����µ� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Result;
	short		Number;
	char		AccountId[MAX_IDSTRING];
	char        Name[MAX_IDSTRING];
	BYTE		Pos;
	BYTE		ClassSkin;
	BYTE		Equipment[24];	// �����ϰ� �ִ� ���
#ifdef DARKLORD_WORK
	WORD		Level;
#endif
} SDHP_CREATECHARRESULT, *LPSDHP_CREATECHARRESULT;

//----------------------------------------------------------------------------
// [0x05] ���� ���� ���� �޽���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char szId[MAX_IDSTRING];

#ifdef LOGINSERVER_MULOG
	char	szName[MAX_IDSTRING];
	WORD	Level;				// ����
	BYTE    DbClass;			// �������� ����
#endif
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	INT		iAddPlayTime;		// �߱� �Ƿε� - ���ӽð� (5�ð� ������ �������� 0%)
#endif
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	INT		iAddPlayTime;		
#endif
} SDHP_USERCLOSE_ID, *LPSDHP_USERCLOSE_ID;

//----------------------------------------------------------------------------
// 0x05 ���Ӽ����� ĳ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	short		Number;
	char		AccountID[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	BYTE		Guild;
	char		GuildName[MAX_GUILDNAMESTRING];
} SDHP_CHARDELETE, *LPSDHP_CHARDELETE;

//----------------------------------------------------------------------------
// 0x05 ĳ���� ���� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	short		Number;
	char		AccountID[MAX_IDSTRING];

} SDHP_CHARDELETERESULT, *LPSDHP_CHARDELETERESULT;

//----------------------------------------------------------------------------
// 0x06 ���Ӽ����� ������ ������ ĳ���� ������ ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD  h;	
	char    AccountID[MAX_IDSTRING+1];
	char	Name[MAX_IDSTRING+1];
	short	Number;
} SDHP_DBCHARINFOREQUEST, *LPSDHP_DBCHARINFOREQUEST;

//----------------------------------------------------------------------------
// [GS->JS] 0x16 ĳ���� ���� â���� �̵� �� ���������� MatchDB ������ �˸���.
//----------------------------------------------------------------------------
#ifdef MODIFY_UPDATE_MATCHDB_AT_CHARACTERLIST_VIEW_20060725
typedef struct
{
	PBMSG_HEAD	h;
	char		szId[MAX_IDSTRING];
	char		szName[MAX_IDSTRING];	
	WORD		Level;					// ����
	BYTE		DbClass;				// �������� ����

} SDHP_REQ_UPDATE_MATCHDB_USERCHARACTERS, *LPSDHP_REQ_UPDATE_MATCHDB_USERCHARACTERS;
#endif

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
// 0x06 ĳ���� ���� ��û ��� 
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	
	BYTE  result;
	short Number;
	
	char  AccountID[MAX_IDSTRING];
	char  Name[MAX_IDSTRING+1];	// �̸�
	BYTE  Class;				// ����
	short Level;				// ����
	int	  LevelUpPoint;			// ������ ����Ʈ 
	int   Exp;					// ����ġ
	int   NextExp;				// ���� ����ġ
	int   Money;				// ��
	WORD Str;					// ��
	WORD Dex;					// ��ø�� 
	WORD Vit;					// �ǰ�
	WORD Energy;				// ������
	

	WORD  Life;					// ���� ����
	WORD  MaxLife;				// ���� �ִ� ����
	WORD  Mana;					// ���� ����
	WORD  MaxMana;				// ���� �ִ� ������
	
	BYTE  dbInventory[MAX_DBINVENTORY];	// ������ �κ��丮
	BYTE  dbMagicList[MAX_DBMAGIC];		// ����(��ų)����Ʈ

	BYTE  MapNumber;			// ������ ���� �������� �� ��ȣ
	BYTE  MapX;					// ������ ���� �������� X ��ǥ
	BYTE  MapY;					// ������ ���� �������� Y ��ǥ
	BYTE  Dir;
	int   PkCount;
	int   PkLevel;
	int   PkTime;
	BYTE  CtlCode;
	BYTE  DbVersion;
	BYTE  AccountCtlCode;
	
#ifdef NEW_SKILL_FORSKYLAND	
	BYTE  dbQuest[MAX_DBQUEST];
#endif

#ifdef DARKLORD_WORK
	WORD	Leadership;		// ���
	WORD	ChatLitmitTime;		// ���
#endif
	
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	int		iFruitPoint;		// ���� ����Ʈ
#endif

	BYTE AccountType;
	INT64 EndTime;

} SDHP_DBCHAR_INFORESULT, *LPSDHP_DBCHAR_INFORESULT;

//----------------------------------------------------------------------------
// 0x07 ĳ���� ���� ���� 
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	
	char  AccountID[MAX_IDSTRING+1];
	char  Name[MAX_IDSTRING+1];	// �̸�

	short Level;				// ����
	BYTE  Class;				// ����
	int   LevelUpPoint;			// ���� �� ����Ʈ
	int   Exp;					// ����ġ
	int   NextExp;				// ���� ����ġ
	int   Money;				// ��
	WORD Str;					// ��
	WORD Dex;					// ��ø�� 
	WORD Vit;					// �ǰ�
	WORD Energy;				// ������

	WORD  Life;					// ���� ����
	WORD  MaxLife;				// ���� �ִ� ����
	WORD  Mana;					// ���� ����
	WORD  MaxMana;				// ���� �ִ� ������	

	BYTE  dbInventory[MAX_DBINVENTORY];	// ������ �κ��丮
	BYTE  dbMagicList[MAX_DBMAGIC];		// ��������Ʈ

	BYTE  MapNumber;			// ������ ���� �������� �� ��ȣ
	BYTE  MapX;					// ������ ���� �������� X ��ǥ
	BYTE  MapY;					// ������ ���� �������� Y ��ǥ
	BYTE  Dir;
	int   PkCount;
	int	  PkLevel;
	int	  PkTime;

	BYTE  dbQuest[MAX_DBQUEST];

#ifdef ITEMSAVE_MODIFY_FORSTATICS
	BYTE  CharInfoSave;			// ��踦 ���� DB���� ���� ����
#endif
	
#ifdef DARKLORD_WORK
	WORD	Leadership;			// ���
	WORD	ChatLitmitTime;		// ä�� ���� �ð�
#endif
	
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	int		iFruitPoint;		// ���� ����Ʈ
#endif

	BYTE	AccountType;
	INT64	DecreaseTime;

	BYTE	CtlCode;

} SDHP_DBCHAR_INFOSAVE, *LPSDHP_DBCHAR_INFOSAVE;

// [0x08] â�� �ִ� �������� ��û�Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	short		aIndex;
} SDHP_GETWAREHOUSEDB, *LPSDHP_GETWAREHOUSEDB;

// [0x08] â�� �ִ� ������ ����Ʈ�� �����Ѵ�.
// [0x09] â�� �������� �����Ѵ�.
typedef struct
{
	PWMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	short		aIndex;
	int			Money;
	BYTE		dbItems[MAX_WAREHOUSEDBSIZE];
	BYTE		DbVersion;
	short		pw;
} SDHP_GETWAREHOUSEDB_SAVE, *LPSDHP_GETWAREHOUSEDB_SAVE;


// [0x10] â�� ���� �������� ����. 
typedef struct
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	short		aIndex;
} SDHP_GETWAREHOUSEDB_RESULT, *LPSDHP_GETWAREHOUSEDB_RESULT;


// [0x11] ĳ������ �������� �����Ѵ�.
typedef struct
{
	PWMSG_HEAD	h;
	
	char  Name[MAX_IDSTRING];	// �̸�
	BYTE  dbInventory[MAX_DBINVENTORY];	// ������ �κ��丮
} SDHP_DBCHAR_ITEMSAVE, *LPSDHP_DBCHAR_ITEMSAVE;

// [0x12] â�� ���� �����Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	short		aIndex;
	int			Money;
} SDHP_WAREHOUSEMONEY_SAVE, *LPSDHP_WAREHOUSEMONEY_SAVE;

#ifdef MODIFY_QUEST_SYSTEM_20070525	// DS <-> GS �������� ����
// GD [0x13]	����Ʈ �Ϸ��� - ���� ų���� ������ ��û�Ѵ�.
typedef struct 
{
	PBMSG_HEAD		h;
	int				iUserIndex;
	char			szCharName[MAX_IDSTRING];
} SDHP_REQ_QUEST_MONSTER_KILL, *LPSDHP_REQ_QUEST_MONSTER_KILL;

// GD <-> DG [0x14]	����Ʈ �Ϸ��� - ���� ų���� ������ ���´�.
typedef struct 
{
	PBMSG_HEAD		h;
	int				iUserIndex;
	char			szCharName[MAX_IDSTRING];
	int				btQuestDBIndex;				// DB�� ����� ���� ����Ʈ �ε���
	int				iQuestMonIndex1;	
	int				iKillCount1;
	int				iQuestMonIndex2;
	int				iKillCount2;
	int				iQuestMonIndex3;
	int				iKillCount3;
	int				iQuestMonIndex4;
	int				iKillCount4;
	int				iQuestMonIndex5;
	int				iKillCount5;
} SDHP_QUEST_MONSTER_KILL_INFO, *LPSDHP_QUEST_MONSTER_KILL_INFO;

#endif	// MODIFY_QUEST_SYSTEM_20070525
/*
//----------------------------------------------------------------------------
// [0x10] ���Ӽ������� ����� ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	char	Id[MAX_IDSTRING];
	char	Pass[MAX_IDSTRING];
	char	Name[MAX_IDSTRING];
	char	JoominN[MAX_JOOMINNUMBERSTR];
	char	Question[MAX_QUESTIONSTR];
	char	Answer[MAX_ANSWERSTR];
	char	PhoneN[MAX_PHONENUMBERSTR];
	char	Email[MAX_EMAILSTR];
	char    IpAddress[17];
} SDHP_ACCOUNTREQUEST, *LPSDHP_ACCOUNTREQUEST;

//----------------------------------------------------------------------------
// [0x10] ���Ӽ������� ����� ���� ���� ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		result;
	short		Number;
} SDHP_ACCOUNTRESULT, *LPSDHP_ACCOUNTRESULT;
*/

//----------------------------------------------------------------------------
// [0x20] Ư�����̵��� ��������� ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	char	Id[MAX_IDSTRING];
} SDHP_PASSQUESTION_QUERY, *LPSDHP_PASSQUESTION_QUERY;

//----------------------------------------------------------------------------
// [0x20] Ư�����̵��� ��������� ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	BYTE	Result;
	char	Question[MAX_QUESTIONSTR];
} SDHP_PASSQUESTION_QUERY_RESULT, *LPSDHP_PASSQUESTION_QUERY_RESULT;


//----------------------------------------------------------------------------
// [0x21] ��й�ȣ ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	char	Id[MAX_IDSTRING];
	char	JoominN[MAX_JOOMINNUMBERSTR];
	char	Answer[MAX_ANSWERSTR];
	char    IpAddress[17];
} SDHP_PASSQUERY, *LPSDHP_PASSQUERY;

//----------------------------------------------------------------------------
// [0x21] ��й�ȣ ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	BYTE	Result;
	char	Password[MAX_IDSTRING];
} SDHP_PASSQUERY_RESULT, *LPSDHP_PASSQUERY_RESULT;


//----------------------------------------------------------------------------
// [0x22] ��й�ȣ �����û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	char	Id[MAX_IDSTRING];
	char	PassOld[MAX_IDSTRING];
	char	PassNew[MAX_IDSTRING];
	char	JoominN[MAX_JOOMINNUMBERSTR];	
	char	IpAddress[17];
} SDHP_PASSCHANGE, *LPSDHP_PASSCHANGE;


//----------------------------------------------------------------------------
// [0x22] ��й�ȣ �����û ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	BYTE	Result;
} SDHP_PASSCHANGE_RESULT, *LPSDHP_PASSCHANGE_RESULT;


//----------------------------------------------------------------------------
// [0x2D] ���� ������ ������ ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		wIndex;
	DWORD		dwItemSerial;
	WORD		wItemIndex;
} SDHP_LOAD_MEDALINFO, *LPSDHP_LOAD_MEDALINFO;

//----------------------------------------------------------------------------
// [0x2D] ���� ������ ���� ��û ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		wIndex;
	DWORD		dwItemSerial;

	DWORD		dwTime;
	WORD		wRemainTime;
	WORD		wLevel;
	WORD		wLevel_T;
	char		szName[MAX_IDSTRING];
	char		szName_T[MAX_IDSTRING];
	BYTE		btClass;
	BYTE		btClass_T;

	WORD		wItemIndex;
	
} SDHP_LOAD_MEDALINFO_RESULT, *LPSDHP_LOAD_MEDALINFO_RESULT;




//----------------------------------------------------------------------------
// [0x30] ��带 �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		Master[MAX_IDSTRING+1];
	BYTE		Mark[32];
	BYTE		NumberH;
	BYTE		NumberL;

#if defined( ADD_NEW_GUILD_02_20040922 ) || GAME_VERSION >= G_V_99B
	BYTE		btGuildType;

#endif

} SDHP_GUILDCREATE, *LPSDHP_GUILDCREATE;

//----------------------------------------------------------------------------
// ��� ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		Result;
	BYTE		Flag;

	DWORD		GuildNumber;
	
	BYTE		NumberH;
	BYTE		NumberL;
	char		Master[MAX_IDSTRING+1];
	char		GuildName[MAX_GUILDNAMESTRING+1];
	BYTE		Mark[32];

#if defined( ADD_NEW_GUILD_02_20040922 ) || GAME_VERSION >= G_V_99B
	BYTE		btGuildType;

#endif
	
} SDHP_GUILDCREATE_RESULT, *LPSDHP_GUILDCREATE_RESULT;

//----------------------------------------------------------------------------
// [0x31] ��� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		NumberH;
	BYTE		NumberL;
	
	char		GuildName[MAX_GUILDNAMESTRING];
	char		Master[MAX_IDSTRING];
} SDHP_GUILDDESTROY, *LPSDHP_GUILDDESTROY;

//----------------------------------------------------------------------------
// [0x31] ��� ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		Result;
	BYTE		Flag;

	BYTE		NumberH;
	BYTE		NumberL;

	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		Master[MAX_IDSTRING+1];
} SDHP_GUILDDESTROY_RESULT, *LPSDHP_GUILDDESTROY_RESULT;

//----------------------------------------------------------------------------
// [0x32] ��� ��� �߰�
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		MemberID[MAX_IDSTRING+1];
	BYTE		NumberH;
	BYTE		NumberL;
} SDHP_GUILDMEMBERADD, *LPSDHP_GUILDMEMBERADD;

//----------------------------------------------------------------------------
// [0x32] ��� ��� �߰� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		Result;
	BYTE		Flag;

	BYTE		NumberH;
	BYTE		NumberL;

	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		MemberID[MAX_IDSTRING+1];

#ifdef SERVERCODE_EXPAND
	short		pServer;	// ������ �ִ� ����
#else
	BYTE		pServer;	// ������ �ִ� ����
#endif

} SDHP_GUILDMEMBERADD_RESULT, *LPSDHP_GUILDMEMBERADD_RESULT;

//----------------------------------------------------------------------------
// [0x33] ����� ���� 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		NumberH;
	BYTE		NumberL;

	char		GuildName[MAX_GUILDNAMESTRING];
	char		MemberID[MAX_IDSTRING];
} SDHP_GUILDMEMBERDEL, *LPSDHP_GUILDMEMBERDEL;

//----------------------------------------------------------------------------
// [0x33] ����� ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	
	BYTE		Result;
	BYTE		Flag;

	BYTE		NumberH;
	BYTE		NumberL;

	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		MemberID[MAX_IDSTRING+1];

} SDHP_GUILDMEMBERDEL_RESULT, *LPSDHP_GUILDMEMBERDEL_RESULT;


//----------------------------------------------------------------------------
// [0x34] ��� ���� ���� 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		Master[MAX_IDSTRING+1];
	BYTE		Mark[32];
	int			Score;
	BYTE		Count;
} SDHP_GUILDUPDATE, *LPSDHP_GUILDUPDATE;


//----------------------------------------------------------------------------
// [0x35] ��� ������ ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		NumberH;
	BYTE		NumberL;

	char		MemberID[MAX_IDSTRING];
} SDHP_GUILDMEMBER_INFO_REQUEST, *LPSDHP_GUILDMEMBER_INFO_REQUEST;

//----------------------------------------------------------------------------
// [0x35] ĳ���Ͱ� ���ӿ� ���ӽ� ��� ������ ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		MemberID[MAX_IDSTRING+1];
	
#if defined( ADD_NEW_GUILD_01_20040913 ) || GAME_VERSION >= G_V_99B
	BYTE		btGuildStatus;
#endif
	
#if defined( ADD_NEW_GUILD_02_20040922 ) || GAME_VERSION >= G_V_99B
	BYTE		btGuildType;
#endif

#ifdef SERVERCODE_EXPAND
	short		pServer;
#else
	char		pServer;
#endif

} SDHP_GUILDMEMBER_INFO, *LPSDHP_GUILDMEMBER_INFO;


//----------------------------------------------------------------------------
// [0x36] ��帶���Ͱ� �������� �� ���� ������ �������Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;

#ifdef GUILD_REWORK
	
	#ifdef MODIFY_GUILD_VIEWPORT_03_20040913
		INT			Number;
	#else	// #ifdef MODIFY_GUILD_VIEWPORT_03_20040913
		WORD		Number;
	#endif	// #ifdef MODIFY_GUILD_VIEWPORT_03_20040913

	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		Master[MAX_IDSTRING+1];
	BYTE		Mark[32];
	int			score;
#else 
	char		GuildName[MAX_GUILDNAMESTRING+1];
#endif

#if defined( ADD_NEW_GUILD_02_20040922 ) || GAME_VERSION >= G_V_99B
	BYTE		btGuildType;	// ��� Ÿ�� ( 0: �Ϲ�  1: ��ȣ��� )
#endif

#if defined( ADD_NEW_UNION_01_20041006 ) || GAME_VERSION >= G_V_99B
	INT			iGuildUnion;	// UnionMaster Guild ��ȣ
	INT			iGuildRival;	// Rival Guild ��ȣ

	CHAR		szGuildRivalName[MAX_GUILDNAMESTRING+1];	// ���̹� ��� �̸�.
#endif

	BYTE		Count;
} SDHP_GUILDALL_COUNT, *LPSDHP_GUILDALL_COUNT;

//----------------------------------------------------------------------------
// [0x36] ��帶���Ͱ� �������� �� ���� ������ �������Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	char		MemberID[MAX_IDSTRING+1];

#if defined( ADD_NEW_GUILD_01_20040913 ) || GAME_VERSION >= G_V_99B
	BYTE		btGuildStatus;	// ��忡�� ��å �� ����
#endif

#ifdef SERVERCODE_EXPAND
	short		pServer;
#else
	BYTE		pServer;
#endif
} SDHP_GUILDALL, *LPSDHP_GUILDALL;

//----------------------------------------------------------------------------
// [0x37] ��� ���ھ� ������Ʈ 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	int			Score;
} SDHP_GUILDSCOREUPDATE, *LPSDHP_GUILDSCOREUPDATE;

//----------------------------------------------------------------------------
// [0x38] ��� ���� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char GuildName[MAX_GUILDNAMESTRING+1];
	char szGuildNotice[MAX_GUILDNOTICE];
} SDHP_GUILDNOTICE, *LPSDHP_GUILDNOTICE;


//----------------------------------------------------------------------------
// [0x40] ��� ����Ʈ�� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
} SDHP_GUILDLISTREQUEST, *LPSDHP_GUILDLISTREQUEST;

//----------------------------------------------------------------------------
// [0x40] ��带 �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		Number;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		Master[MAX_IDSTRING+1];
	BYTE		Mark[32];
	int			score;
} SDHP_GUILDCREATED, *LPSDHP_GUILDCREATED;

//----------------------------------------------------------------------------
// [0x41] ��帮��Ʈ�� ��δ� ���´�.
// State : 
//      0x00 = ��� ����Ʈ ������ ���� 
//      0x01 = ��� ����Ʈ ��� �� �޾Ҵ�
// Count : ��� ����Ʈ ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		State;
	int			Count;
} SDHP_GUILDLISTSTATE, *LPSDHP_GUILDLISTSTATE;

//----------------------------------------------------------------------------
// [0x50] �̺�Ʈ ZEN�� �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	int			Total;
	int			Number;
} SDHP_ZENSAVE, *LPSDHP_ZENSAVE;

//----------------------------------------------------------------------------
// [0x50] �̺�Ʈ ZEN�� �о�´�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	int			Total;
	int			Number;
} SDHP_ZENLOAD, *LPSDHP_ZENLOAD;

//----------------------------------------------------------------------------
// [0x52] ������ ��ȣ�� �ο��޴´�.
//----------------------------------------------------------------------------
// MapNumber �� 255 �϶��� ī���� �ڽ��̴�.
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		x;
	BYTE		y;
	BYTE		MapNumber;
	short		Type;
	BYTE		Level;
	BYTE		Dur;
	BYTE		Op1;
	BYTE		Op2;
	BYTE		Op3;
	BYTE		NewOption;
	int			aIndex;
	short		lootindex;
	BYTE		SetOption;
	BOOL		SpecialShop;
	
} SDHP_ITEMCREATE, *LPSDHP_ITEMCREATE;

//----------------------------------------------------------------------------
// [0x52] ������ ��ȣ�� �ο��޴´�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		x;
	BYTE		y;
	BYTE		MapNumber;
	DWORD		m_Number;
	short		Type;
	BYTE		Level;
	BYTE		Dur;
	BYTE		Op1;
	BYTE		Op2;
	BYTE		Op3;
	BYTE		NewOption;		
	int			aIndex;
	short		lootindex;
	BYTE		SetOption;
	BOOL		SpecialShop;

} SDHP_ITEMCREATERECV, *LPSDHP_ITEMCREATERECV;


//----------------------------------------------------------------------------
// [0x53] ������ �̵� ��θ� �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{

	PBMSG_HEAD	h;
	DWORD		Serial;
	char		ServerName[20];
	char		Account[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	char		ItemName[21];
	BYTE		X;
	BYTE		Y;
	BYTE		ItemLevel;
	BYTE		ItemOp1;
	BYTE		ItemOp2;
	BYTE		ItemOp3;

} SDHP_ITEMMOVESAVE, *LPSDHP_ITEMMOVESAVE;


//----------------------------------------------------------------------------
// [0x54] ���� ������ ��ȣ�� �ο��޴´�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		x;
	BYTE		y;
	BYTE		MapNumber;
	short		Type;
	BYTE		Level;
	BYTE		Dur;
	BYTE		Op1;
	BYTE		Op2;
	BYTE		Op3;
	BYTE		NewOption;
	int			aIndex;
	short		lootindex;
	BYTE		SetOption;

	DWORD		dwTime;
	WORD		wRemainTime;
	WORD		wLevel;
	WORD		wLevel_T;
	char		szName[MAX_IDSTRING];
	char		szName_T[MAX_IDSTRING];
	BYTE		btClass;
	BYTE		btClass_T;

	WORD		wItemIndex;

} SDHP_PROPERTY_ITEMCREATE, *LPSDHP_PROPERTY_ITEMCREATE;


//----------------------------------------------------------------------------
// [0x54] ���� ������ ��ȣ�� �ο��޴´�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		x;
	BYTE		y;
	BYTE		MapNumber;
	DWORD		m_Number;
	short		Type;
	BYTE		Level;
	BYTE		Dur;
	BYTE		Op1;
	BYTE		Op2;
	BYTE		Op3;
	BYTE		NewOption;
	int			aIndex;
	short		lootindex;
	BYTE		SetOption;

	DWORD		dwTime;
	WORD		wRemainTime;
	WORD		wLevel;
	WORD		wLevel_T;
	char		szName[MAX_IDSTRING];
	char		szName_T[MAX_IDSTRING];
	BYTE		btClass;
	BYTE		btClass_T;

	WORD		wItemIndex;

} SDHP_PROPERTY_ITEMRECV, *LPSDHP_PROPERTY_ITEMRECV;


#ifdef DARKLORD_WORK

//----------------------------------------------------------------------------
// [0x55] �� ������ �ø��� ��ȣ�� �ο��޴´�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		x;
	BYTE		y;
	BYTE		MapNumber;
	short		Type;
	BYTE		Level;
	BYTE		Dur;
	BYTE		Op1;
	BYTE		Op2;
	BYTE		Op3;
	BYTE		NewOption;
	int			aIndex;
	short		lootindex;
	BYTE		SetOption;

} SDHP_PET_ITEMCREATE, *LPSDHP_PET_ITEMCREATE;

//----------------------------------------------------------------------------
// [0x55] ������ ��ȣ�� �ο��޴´�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		x;
	BYTE		y;
	BYTE		MapNumber;
	DWORD		m_Number;
	short		Type;
	BYTE		Level;
	BYTE		Dur;
	BYTE		Op1;
	BYTE		Op2;
	BYTE		Op3;
	BYTE		NewOption;		
	int			aIndex;
	short		lootindex;
	BYTE		SetOption;

} SDHP_PET_ITEMCREATERECV, *LPSDHP_PET_ITEMCREATERECV;


/////////////////////////////////////////////////
// GS->DS 0x56	�� ������ ������ �޶�� �Ѵ�
typedef struct 
{
	BYTE 	nPos;		// ������ ��ġ...n
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	DWORD	nSerial;	// ������ �ø���...n
#else
	int		nSerial;	// ������ �ø���...n
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
}Request_PetItem_Info, *LPRequest_PetItem_Info;

typedef struct 
{
	PWMSG_HEAD	h;
	char	AccountID[MAX_IDSTRING+1];// ���� ���̵�
	WORD	Number;
	BYTE	InvenType;	// 0:�κ�, 1:â��
	BYTE 	nCount;		// �����ۼ� 

	//BYTE 	nPos;		// ������ ��ġ...n
	//int   nSerial;	// ������ �ø���...n
}SDHP_REQUEST_PETITEM_INFO, *LPSDHP_REQUEST_PETITEM_INFO;

///////////////////////////////////////////////////
// DS->GS 0x56 �� ������ ������ �ش�
typedef struct 
{
	BYTE 	nPos;			// ������ ��ġ...n
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	DWORD   nSerial;		// ������ �ø���
#else
	int     nSerial;		// ������ �ø���
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
	BYTE	Level;			// ������ ����
	int		Exp;			// ������ ����ġ
}Recv_PetItem_Info, *LPRecv_PetItem_Info;

typedef struct 
{
	PWMSG_HEAD	h;

	char		AccountID[MAX_IDSTRING+1];// ���� ���̵�
	WORD		Number;
	BYTE		InvenType;		// 0:�κ�, 1:â��
	BYTE 		nCount;			// �����ۼ�

	//BYTE 		nPos;			// ������ ��ġ
	//int     	nSerial;		// ������ �ø���
	//short		Level;			// ������ ����
	//int		Exp;			// ������ ����ġ
}SDHP_RECV_PETITEM_INFO, *LPSDHP_RECV_PETITEM_INFO;


///////////////////////////////////////////////////
// GS->GS	0x57	�� ������ ������ �����϶�� �Ѵ�
typedef struct 
{	
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	DWORD   nSerial;		// ������ �ø���
#else
	int     nSerial;		// ������ �ø���
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
	BYTE	Level;			// ������ ����
	int		Exp;			// ������ ����ġ
}Save_PetItem_Info, * LPSave_PetItem_Info;

typedef struct 
{
	PWMSG_HEAD	h;

	BYTE 		nCount;		// �����ۼ�

	//int    		nSerial;	// ������ �ø���
	//int			Exp;		// ������ ����
	//short		Level;		// ������ ����
}SDHP_SAVE_PETITEM_INFO, *LPSDHP_SAVE_PETITEM_INFO;
#endif

#ifdef GS_DS_LIVECHECK_20041008

//----------------------------------------------------------------------------
// [0x58] GS -> DS : �������� LIVE ��ȣ�� ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	INT			g_iGsCode;
} SDHP_REQ_GSLIVECHECK, *LPSDHP_REQ_GSLIVECHECK;


//----------------------------------------------------------------------------
// [0x58] DS -> GS : LIVE ��ȣ�� ���� ����� ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	INT			g_iGsCode;
} SDHP_ANS_GSLIVECHECK, *LPSDHP_ANS_GSLIVECHECK;

#endif


#ifdef CHINA_HACKUSER_KICK_SYSTEM
//----------------------------------------------------------------------------
// [0x70] GS -> DS : �ٻ�� ���� �� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	WORD		UserIndex;
	char		AccountID[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
}SDHP_REQ_HACKUSERBLOCK, *LPSDHP_REQ_HACKUSERBLOCK;

//----------------------------------------------------------------------------
// [0x70] DS -> GS : �ٻ�� ���� �� ��û ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	
	WORD		UserIndex;
	char		AccountID[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	BYTE		Result;				// �����( 0 : ���� , �̿��� �� ����)
	BYTE		WarnLevel;			// ��� ���� (1,2)
}SDHP_HACKUSERBLOCK_RESULT, *LPSDHP_HACKUSERBLOCK_RESULT;
#endif

//----------------------------------------------------------------------------
// [0x60] �ɼ� Ű ���� �޴´�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
#ifdef UPDATE_SKILLKEY_EXPAND_20080804
	BYTE		SkillKeyBuffer[20];
#else	// UPDATE_SKILLKEY_EXPAND_20080804
	BYTE		SkillKeyBuffer[10];
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
} SDHP_SKILLKEYDATA, *LPSDHP_SKILLKEYDATA;

typedef struct
{
	PBMSG_HEAD h;	// C1:17
	int iUserIndex;	// 4
	char chAccountID[MAX_IDSTRING+1];	//
	char chCharacterName[MAX_IDSTRING+1];	//
}PMSG_CHARACTER_RESET, *LPPMSG_CHARACTER_RESET;

typedef struct
{
	PBMSG_HEAD h;	// C1:17
	int iUserIndex;	// 4
	int ResetNumber;	//
}PMSG_ANS_CHARACTER_RESET, *LPPMSG_ANS_CHARACTER_RESET;

typedef struct
{
	PBMSG_HEAD h;	// C1:17
	int iUserIndex;	// 4
	char chAccountID[MAX_IDSTRING+1];	//
	char chCharacterName[MAX_IDSTRING+1];	//

	int m_iRuleIndex; //
	BOOL m_bClearStat;	//
	int m_iLevelAfterReset;	//
	int m_iLevelUpPoint; //
	BOOL m_bReqSpecialItem; //
	BOOL m_bBornPlace; //
	int m_iResetNumber; //
	BOOL m_bClearBuffState; //
	BOOL m_bClearMagicList; //
}PMSG_REQ_SAVE_CHARACTER_RESET, *LPPMSG_REQ_SAVE_CHARACTER_RESET;

typedef struct
{
	PBMSG_HEAD h;	// C1:17
	int iUserIndex;	// 4

	int m_iRuleIndex; //
	BOOL m_bClearStat;	//
	int m_iLevelAfterReset;	//
	int m_iLevelUpPoint; //
	BOOL m_bReqSpecialItem; //
	BOOL m_bBornPlace; //
	int m_iResetNumber; //
	BOOL m_bClearBuffState; //
	BOOL m_bClearMagicList; //
}PMSG_ANS_SAVE_CHARACTER_RESET, *LPPMSG_ANS_SAVE_CHARACTER_RESET;

typedef struct
{
	PBMSG_HEAD h;	// C1:17
	char chAccountID[MAX_IDSTRING+1];
	int iUserIndex;
	BYTE AccountType;
	INT64 EndTime;
}PMSG_REQ_VIP, *LPPMSG_REQ_VIP;

typedef struct
{
	PBMSG_HEAD h;	// C1:17
	int iUserIndex;
	BYTE AccountType;
	INT64 EndTime;
}PMSG_ANS_VIP, *LPPMSG_ANS_VIP;

//----------------------------------------------------------------------------
// [0x60] �ɼ� Ű ���� �޴´�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	int			aIndex;
	char		Name[MAX_IDSTRING];
#ifdef UPDATE_SKILLKEY_EXPAND_20080804
	BYTE		SkillKeyBuffer[20];
#else	// UPDATE_SKILLKEY_EXPAND_20080804
	BYTE		SkillKeyBuffer[10];
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
} SDHP_SKILLKEYDATA_SEND, *LPSDHP_SKILLKEYDATA_SEND;


//----------------------------------------------------------------------------
// [0x60] �̺�Ʈ ������ �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		Event;	
} SDHP_EVENTSTART, *LPSDHP_EVENTSTART;


//----------------------------------------------------------------------------
// [0x61] ������������ �̺�Ʈ ���� �Ǵ� ���Ḧ �˸���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		Event;			// �̺�Ʈ ��ȣ
	BYTE		Stat;			// ���� 0 : ���� 1 : ����
} SDHP_EVENTCONTROL, *LPSDHP_EVENTCONTROL;


//----------------------------------------------------------------------------
// [0x62] ������������ Reload ������ �˸���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		Option;			// �ɼ� ��ȣ
} SDHP_OPTIONCONTROL, *LPSDHP_OPTIONCONTROL;

// 0x30
typedef struct
{
	PBMSG_HEAD	h;
	char		Account[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
} SDHP_LOVEHEARTEVENT, * LPSDHP_LOVEHEARTEVENT;

// 0x30
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;

	char		Name[MAX_IDSTRING];
	int			Number;
	
} SDHP_LOVEHEARTEVENT_RESULT, * LPSDHP_LOVEHEARTEVENT_RESULT;

// 0x40 ��ü ����
typedef struct
{
	PBMSG_HEAD	h;
	char		Notice[61];
} SDHP_NOTICE, *LPSDHP_NOTICE;

// 0x41 ���� ����
typedef struct
{
	PBMSG_HEAD	h;
	char		szId[10];
	char		Notice[61];
} SDHP_USER_NOTICE, *LPSDHP_USER_NOTICE;


// 0x42 ���� ��
typedef struct
{
	PBMSG_HEAD	h;
	int		ClientIndex;
	char	AccountID[MAX_IDSTRING];
	BYTE	ServerNum;
	char	CharName[MAX_IDSTRING];
	BYTE	Type;
} SDHP_GAME_BLOCK, *LPSDHP_GAME_BLOCK;

// 0x42 ���� �� ���
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} SDHP_GAME_BLOCK_RESULT, *LPSDHP_GAME_BLOCK_RESULT;


// 0xCF ĳ���� �̵�
typedef struct
{
	PBMSG_HEAD	h;
	char		Account[MAX_IDSTRING];
	short		Number;		
} SDHP_CHARACTER_TRANSFER, *LPSDHP_CHARACTER_TRANSFER;

typedef struct
{
	PBMSG_HEAD	h;
	char		Account[MAX_IDSTRING];
	short		Number;		
	BYTE		Result;
} SDHP_CHARACTER_TRANSFER_RESULT, *LPSDHP_CHARACTER_TRANSFER_RESULT;

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410 // ĳ���� ���� ��������
//----------------------------------------------------------------------------
// GD[0x16] ĳ���� ������ ��û.
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD	h;
	int			aIndex;
	char		AccountId[MAX_IDSTRING];
	char		OldName[MAX_IDSTRING];
	char		NewName[MAX_IDSTRING];
} SDHP_CHANGE_NAME, * LPSDHP_CHANGE_NAME;

//----------------------------------------------------------------------------
// DG[0x16] ĳ���� ������ ��û ���.
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD	h;
	int			aIndex;
	char		AccountId[MAX_IDSTRING];
	char		OldName[MAX_IDSTRING];
	char		NewName[MAX_IDSTRING];
	BYTE		btResult;		// 0: ����, 1: ���� ĳ���͸� ����
} SDHP_CHANGE_NAME_RESULT, * LPSDHP_CHANGE_NAME_RESULT;
#endif	// UPDATE_CHANGE_CHARACTERNAME_20080410

//##__FRIEND_WORK__
#ifdef __FRIEND_WORK__

//----------------------------------------------------------------------------
// GD[0x60] ģ�� ����Ʈ�� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		Number;
	char		Name[MAX_IDSTRING];
	BYTE		pServer;
} FHP_FRIENDLIST_REQ, *LPFHP_FRIENDLIST_REQ;

//----------------------------------------------------------------------------
// DG[0x60] ģ�� ����Ʈ�� ��û�� ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PWMSG_HEAD	h;
	short		Number;
	char		Name[MAX_IDSTRING];
	BYTE		Count;
	BYTE		MailCount;
} FHP_FRIENDLIST_COUNT, *LPFHP_FRIENDLIST_COUNT;

//----------------------------------------------------------------------------
// DG[0x60] ģ�� ����Ʈ�� ��û�� ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	char		Name[MAX_IDSTRING];
	BYTE		Server;
} FHP_FRIENDLIST, *LPFHP_FRIENDLIST;

//----------------------------------------------------------------------------
// GD[0x61] ������� ģ�� ����Ʈ�� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		Number;
	char		Name[MAX_IDSTRING];
} FHP_WAITFRIENDLIST_REQ, *LPFHP_WAITFRIENDLIST_REQ;

//----------------------------------------------------------------------------
// DG[0x61] ������� ģ�� ����Ʈ�� ��û ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		Number;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
} FHP_WAITFRIENDLIST_COUNT, *LPFHP_WAITFRIENDLIST_COUNT;

//----------------------------------------------------------------------------
// GD[0x62] ���¸� �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;	
	short		Number;
	char		Name[MAX_IDSTRING];
	BYTE		State;
} FHP_FRIEND_STATE_C, *LPFHP_FRIEND_STATE_C;

//----------------------------------------------------------------------------
// DG[0x62] ���¸� �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;	
	short		Number;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
	BYTE		State;
} FHP_FRIEND_STATE, *LPFHP_FRIEND_STATE;

//----------------------------------------------------------------------------
// GD[0x63] ģ�� �߰� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;	
	short		Number;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
} FHP_FRIEND_ADD_REQ, *LPFHP_FRIEND_ADD_REQ;


//----------------------------------------------------------------------------
// DG[0x63] ģ�� �߰� ��û ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;	
	short		Number;
	BYTE		Result;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
	BYTE		Server;
} FHP_FRIEND_ADD_RESULT, *LPFHP_FRIEND_ADD_RESULT;


//----------------------------------------------------------------------------
// GD[0x64] ������� ģ�� �߰� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;	
	BYTE		Result;
	short		Number;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
} FHP_WAITFRIEND_ADD_REQ, *LPFHP_WAITFRIEND_ADD_REQ;

//----------------------------------------------------------------------------
// DG[0x64] ������� ģ�� �߰� ��û ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;	
	short		Number;
	BYTE		Result;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
	BYTE		pServer;
} FHP_WAITFRIEND_ADD_RESULT, *LPFHP_WAITFRIEND_ADD_RESULT;


//----------------------------------------------------------------------------
// GD[0x65] ģ���� �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;	
	short		Number;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
} FHP_FRIEND_DEL_REQ, *LPFHP_FRIEND_DEL_REQ;

//----------------------------------------------------------------------------
// DG[0x65] ģ���� ���� ����� �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;	
	short		Number;
	BYTE		Result;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
} FHP_FRIEND_DEL_RESULT, *LPFHP_FRIEND_DEL_RESULT;



//----------------------------------------------------------------------------
// GD[0x66] ��ȭ�� ������ ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		Number;
	char		Name[MAX_IDSTRING];
	char		fName[MAX_IDSTRING];	// ��ȭ ���
	
} FHP_FRIEND_CHATROOM_CREATE_REQ, *LPFHP_FRIEND_CHATROOM_CREATE_REQ;

//----------------------------------------------------------------------------
// GD[0x66] ��ȭ�� ������ ��û���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		Result;
	short		Number;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
	char		ServerIp[15];
	WORD		RoomNumber;
	DWORD		Ticket;
	BYTE		Type;
} FHP_FRIEND_CHATROOM_CREATE_RESULT, *LPFHP_FRIEND_CHATROOM_CREATE_RESULT;

//----------------------------------------------------------------------------
// GD[0x70] �ٸ��������� ������ ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PWMSG_HEAD	h;
	short		Number;
	DWORD		WindowGuid;
	char		Name[MAX_IDSTRING];
	char		ToName[MAX_IDSTRING];
	char		Subject[MAX_MEMO_SUBJECT];
	BYTE		Dir;
	BYTE		Action;
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
		
	char		Memo[MAX_MEMO];
} FHP_FRIEND_MEMO_SEND, *LPFHP_FRIEND_MEMO_SEND;

//----------------------------------------------------------------------------
// GD[0x70] �ٸ� �������� ������ ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		Number;
	char		Name[MAX_IDSTRING];
	BYTE		Result;
	DWORD		WindowGuid;
} FHP_FRIEND_MEMO_SEND_RESULT, *LPFHP_FRIEND_MEMO_SEND_RESULT;

//----------------------------------------------------------------------------
// GD[0x71] ���� ����Ʈ ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		Number;
	char		Name[MAX_IDSTRING];
} FHP_FRIEND_MEMO_LIST_REQ, *LPFHP_FRIEND_MEMO_LIST_REQ;


//----------------------------------------------------------------------------
// DG[0x71] ���� ����Ʈ�� ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PWMSG_HEAD	h;	
	WORD		Number;
	WORD		MemoIndex;
	char		SendName[MAX_IDSTRING];
	char		RecvName[MAX_IDSTRING];
	char		Date[MAX_DATE];
	char		Subject[MAX_MEMO_SUBJECT];
	BYTE		read;
} FHP_FRIEND_MEMO_LIST, *LPFHP_FRIEND_MEMO_LIST;


//----------------------------------------------------------------------------
// GD[0x72] ���� ������ �����޶�� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		Number;
	WORD		MemoIndex;
	char		Name[MAX_IDSTRING];
} FHP_FRIEND_MEMO_RECV_REQ, *LPFHP_FRIEND_MEMO_RECV_REQ;

//----------------------------------------------------------------------------
// DG[0x72] ���� ������ ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PWMSG_HEAD	h;
	short		Number;
	char		Name[MAX_IDSTRING];
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
} FHP_FRIEND_MEMO_RECV, *LPFHP_FRIEND_MEMO_RECV;

//----------------------------------------------------------------------------
// GD[0x73] ���� ���� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		Number;
	WORD		MemoIndex;
	char		Name[MAX_IDSTRING];
} FHP_FRIEND_MEMO_DEL_REQ, *LPFHP_FRIEND_MEMO_DEL_REQ;

//----------------------------------------------------------------------------
// DG[0x73] ���� ���� ��û ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		Result;
	WORD		MemoIndex;
	short		Number;
	char		Name[MAX_IDSTRING];
} FHP_FRIEND_MEMO_DEL_RESULT, *LPFHP_FRIEND_MEMO_DEL_RESULT;

//----------------------------------------------------------------------------
// GD[0x74] ģ�� �ʴ� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		Number;
	char		Name[MAX_IDSTRING];
	char		FriendName[MAX_IDSTRING];
	WORD		RoomNumber;
	DWORD		WindowGuid;
} FHP_FRIEND_INVITATION_REQ, *LPFHP_FRIEND_INVITATION_REQ;

//----------------------------------------------------------------------------
// DG[0x74] ģ�� �ʴ� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		Result;
	short		Number;
	char		Name[MAX_IDSTRING];
	DWORD		WindowGuid;
} FHP_FRIEND_INVITATION_RET, *LPFHP_FRIEND_INVITATION_RET;


#ifdef MAP_SERVER_WORK_20041030		// ���� �ʺ��� ���� ��Ŷ - ���Ӽ��� <-> ��������

//----------------------------------------------------------------------------
// GJ [0x7A] ������������ Ư�� ����ڰ� �ʼ����� �̵��ؾ� ���� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iIndex;							// ���Ӽ��� ����� �ε���
	CHAR		szAccountID[MAX_IDSTRING+1];	// �������� ID
	CHAR		szCharName[MAX_IDSTRING+1];		// �������� �ɸ���
	WORD		wCurMapSvrCode;					// ������ �ʼ��� �ڵ� (���� �����ڵ尡 �ɰ���)
	WORD		wDstMapSvrCode;					// �̵��� �ʼ��� �ڵ� (���� �����ڵ尡 �ɰ���)
	WORD		wMapNumber;						// �̵��� �ʹ�ȣ
	BYTE		btX;							// �̵�Ȱ ��ǥ X
	BYTE		btY;							// �̵�Ȱ ��ǥ Y
#ifdef MAP_SERVER_ADDINFO_20050509
#ifdef PCBANG_ADVANTAGE							// �ѱ����� (FOR_KOREA)
	BYTE		bPcBangUser;					// PC�� ��������� ����
#endif	
#endif
#ifdef MAP_SERVER_ADDINFO_ADULT_20050610		// �±����� (FOR_THAILAND)
	BYTE		bIsAdult;						// �������� ����
#endif	
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	INT			iAddPlayTime;					// �߱� �Ƿε� - ���ӽð� (5�ð� ������ �������� 0%)
#endif
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	INT			iAddPlayTime;					// ��Ʈ�� �Ƿε� - ���ӽð� (5�ð� ������ �������� 0%)
#endif
} PMSG_REQ_MAPSVRMOVE, *LPPMSG_REQ_MAPSVRMOVE;


//----------------------------------------------------------------------------
// JG [0x7A] ���������� Ư�� ������� �ʼ��� �̵��� ���� ����� �˸� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iIndex;							// ���Ӽ��� ����� �ε���
	CHAR		szAccountID[MAX_IDSTRING+1];	// �������� ID
	CHAR		szCharName[MAX_IDSTRING+1];		// �������� �ɸ���
	WORD		wCurMapSvrCode;					// ������ �ʼ��� �ڵ� (���� �����ڵ尡 �ɰ���)
	WORD		wDstMapSvrCode;					// �̵��� �ʼ��� �ڵ� (���� �����ڵ尡 �ɰ���)
	WORD		wMapNumber;						// �̵��� �ʹ�ȣ
	BYTE		btX;							// �̵�Ȱ ��ǥ X
	BYTE		btY;							// �̵�Ȱ ��ǥ Y
	INT			iResult;						// �̵���û ��� (0:����/1:����Ʋ��/2:���Ӽ����ε�������/3:��󼭹�Ǯ/4:�̹��̵���û��)
												// 0 : ����
												// 1 : ����
	INT			iJoinAuthCode1;					// ���� AUTH �ڵ� - 1
	INT			iJoinAuthCode2;					// ���� AUTH �ڵ� - 2
	INT			iJoinAuthCode3;					// ���� AUTH �ڵ� - 3
	INT			iJoinAuthCode4;					// ���� AUTH �ڵ� - 4
#ifdef MAP_SERVER_ADDINFO_20050509
#ifdef PCBANG_ADVANTAGE
	BYTE		bPcBangUser;					// PC�� ��������� ����
#endif
#endif	
#ifdef MAP_SERVER_ADDINFO_ADULT_20050610		// �±����� (FOR_THAILAND)
	BYTE		bIsAdult;						// �������� ����
#endif	
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	INT			iAddPlayTime;					// �߱� �Ƿε� - ���ӽð� (5�ð� ������ �������� 0%)
#endif
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	INT			iAddPlayTime;					// ��Ʈ�� �Ƿε� - ���ӽð� (5�ð� ������ �������� 0%)
#endif
} PMSG_ANS_MAPSVRMOVE, *LPPMSG_ANS_MAPSVRMOVE;


//----------------------------------------------------------------------------
// GJ [0x7B] ������������ Ư�� ����ڰ� �ʼ��� �̵� �� ������ ��û���� �˸� (0xC1)		-> ���������� �ش� ������� ���Ӽ��� �ڵ嵵 ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iIndex;							// ���Ӽ��� ����� �ε���
	CHAR		szAccountID[MAX_IDSTRING+1];	// �������� ID
	CHAR		szCharName[MAX_IDSTRING+1];		// �������� �ɸ���
	WORD		wDstMapSvrCode;					// ����ڰ� �̵��� ���Ӽ��� �ڵ� (���� ������ ���Ӽ������� �־���)
	INT			iJoinAuthCode1;					// ���� AUTH �ڵ� - 1
	INT			iJoinAuthCode2;					// ���� AUTH �ڵ� - 2
	INT			iJoinAuthCode3;					// ���� AUTH �ڵ� - 3
	INT			iJoinAuthCode4;					// ���� AUTH �ڵ� - 4
} PMSG_REQ_MAPSVRAUTH, *LPPMSG_REQ_MAPSVRAUTH;


//----------------------------------------------------------------------------
// JG [0x7B] ������������ Ư�� ����ڰ� �ʼ��� �̵� �� ������û�� ���� ��� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iIndex;							// ���Ӽ��� ����� �ε���
	CHAR		szAccountID[MAX_IDSTRING+1];	// �������� ID			(������������ �ٽ� ����)
	CHAR		szCharName[MAX_IDSTRING+1];		// �������� �ɸ���		(������������ �ٽ� ����)
	WORD		wPrevMapSvrCode;				// ���� ���Ӽ��� �ڵ�
	WORD		wMapNumber;						// �̵��� �ʹ�ȣ
	BYTE		btX;							// �̵�Ȱ ��ǥ X
	BYTE		btY;							// �̵�Ȱ ��ǥ Y
	INT			iResult;						// �̵���û ��� (0:����/1:����Ʋ��/2:�����ڵ�Ʋ��/3:�����ڵ�Ʋ��/4:�����̵��߾ƴ�)

	// �Ʒ��� ������ ������� ���� ��
	INT			iUserNumber;
	INT			iDBNumber;
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
	CHAR		cJoominNumber[13];//[MAX_JOOMINNUMBER_LENGTH];				// �ֹι�ȣ ���ڸ�
#else
	CHAR		cJoominNumber[13];				// �ֹι�ȣ ���ڸ�
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424

	BYTE		btBlockCode;					// ����ڵ�� ������ ����
#ifdef FOR_THAILAND
	CHAR		AuthAdult;						// ���� ���� �ڵ�
#endif
#ifdef ADD_PROTOCOL_PCBANG_GUID
	INT			iPcBangGuid;
#endif
#ifdef MAP_SERVER_ADDINFO_20050509
#ifdef PCBANG_ADVANTAGE
	BYTE		bPcBangUser;					// PC�� ��������� ����
#endif	
#endif	
#ifdef MAP_SERVER_ADDINFO_ADULT_20050610		// �±����� (FOR_THAILAND)
	BYTE		bIsAdult;						// �������� ����
#endif	
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	int			iVerifiedType;					// �߱� �Ƿε� ���� ���� Ÿ�� ( 0 : ����, 1 : �̼�����, 2 : ���� )
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	INT			iAddPlayTime;					// �߱� �Ƿε� - ���ӽð� (5�ð� ������ �������� 0%)
#endif
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	INT			iAddPlayTime;					// ��Ʈ�� �Ƿε� - ���ӽð� (5�ð� ������ �������� 0%)
#endif
} PMSG_ANS_MAPSVRAUTH, *LPPMSG_ANS_MAPSVRAUTH;

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
//----------------------------------------------------------------------------
// GJ [0x8A] ���������� ��ŷ ���α׷��� ��� ���� Ȯ���� ��ε� ĳ������ ��û�Ѵ�. (0xC1) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	char		chHackToolFilename[64];
} PMSG_HACKTOOL_USE_STATISTICS_REQ, *LPPMSG_HACKTOOL_USE_STATISTICS_REQ;

//----------------------------------------------------------------------------
// JG [0x8A] ��ü ���Ӽ����� ��ŷ ���α׷��� ��� ���� Ȯ���� ��û�Ѵ�. (0xC1) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	char		chHackToolFilename[64];
} PMSG_HACKTOOL_USE_STATISTICS, *LPPMSG_HACKTOOL_USE_STATISTICS;

//----------------------------------------------------------------------------
// GJ [0x8B] ���������� ��ŷ ���α׷��� ������� ���̵� ������ ��û�Ѵ�. (0xC1) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	BYTE		btModifyType;				// ���� Ÿ�� ( 1 : �߰�, 2 : ����, 3 : ���� )
	DWORD		dwReservationTime;			// ��� ���� �ð�
	DWORD		dwBlockTerm;				// ��� �Ⱓ (���� : ��)
	WORD		wBlockRate;					// ��� Ȯ��
	char		chHackToolFilename[64];
} PMSG_RESERVE_HACKTOOL_MAP_MOVE_REQ, *LPPMSG_RESERVE_HACKTOOL_MAP_MOVE_REQ;

//----------------------------------------------------------------------------
// GC [0x8B] ��ü ���Ӽ����� ��ŷ ���α׷��� ������� ���̵� ������ ��û�Ѵ�. (0xC1) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	BYTE		btModifyType;				// ���� Ÿ�� ( 1 : �߰�, 2 : ����, 3 : ���� )
	DWORD		dwReservationTime;			// ��� ���� �ð�
	DWORD		dwBlockTerm;				// ��� �Ⱓ (���� : ��)
	WORD		wBlockRate;					// ��� Ȯ��
	char		chHackToolFilename[64];
} PMSG_RESERVE_HACKTOOL_MAP_MOVE, *LPPMSG_RESERVE_HACKTOOL_MAP_MOVE;

//----------------------------------------------------------------------------
// GJ [0x8C] ��ü ���Ӽ����� ��ŷ ���α׷��� ��踦 ���������� �����Ѵ�. (0xC1) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	WORD		wCurrentUserCount;
	WORD		wHackToolUserCount;
	char		chHackToolFilename[64];
} PMSG_HACKTOOL_USE_STATISTICS_ANS, *LPPMSG_HACKTOOL_USE_STATISTICS_ANS;

//----------------------------------------------------------------------------
// GJ [0x8C] ��ü ���Ӽ����� ��ŷ ���α׷��� ��踦 ���������� �����Ѵ�. (0xC1) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	char		chMessage[256];
}PMSG_HACKTOOL_USE_STATISTICS_TOTAL, *LPPMSG_HACKTOOL_USE_STATISTICS_TOTAL;
#endif // ADD_HACKING_TOOL_BLOCK_20090311

//----------------------------------------------------------------------------
// JG [0x7C] Ư�� ���Ӽ����� ���� �ִ� ����� ���� �ֱ������� ���� (0xC1) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iSvrCode;						// ����ڰ� �̵��� ���Ӽ��� �ڵ� (���Ӽ������� �־���)
	INT			iMaxUserCount;					// ���� ������ �ִ� ����� ��
	INT			iCurUserCount;					// ���� ������ ���� ����� ��
} PMSG_NOTIFY_MAXUSER, *LPPMSG_NOTIFY_MAXUSER;

#endif

#ifdef UPDATE_RESEND_GETCHARlIST_DISABLE_20090605
//----------------------------------------------------------------------------
// GJ [0x8F] ����ų �����Ǹ� ����Ѵ�. (0xC1) 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	char		BlockIp[17];
} PMSG_BLOCK_IP, *LPPMSG_BLOCK_IP;
#endif	// UPDATE_RESEND_GETCHARlIST_DISABLE_20090605

//----------------------------------------------------------------------------
// ä�� �������� �������� ��ȯ
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// DG[0xA0] ä�� ������ �� ������ ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];	// �� ������ ��û�� ���
	char		FriendName[MAX_IDSTRING];	// ���� ��ȭ ���
	short		UserNumber;
	short		ServerNumber;
	short		FriendNumber;
	short		FriendServerNumber;
} FHP_CHAT_ROOMCREATE, *LPFHP_CHAT_ROOMCREATE;

//----------------------------------------------------------------------------
// DG[0xA0] ä�� ������ �� ������ ��û ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		Result;
	WORD		RoomNumber;
	char		Name[MAX_IDSTRING];	// �� ������ ��û�� ���
	char		FriendName[MAX_IDSTRING];	// ��ȭ ���
	short		UserNumber;
	short		ServerNumber;
	DWORD		Ticket;
	DWORD		FriendTicket;
	BYTE		Type;
} FHP_CHAT_ROOMCREATE_RESULT, *LPFHP_CHAT_ROOMCREATE_RESULT;

//----------------------------------------------------------------------------
// DG[0xA1] ä�� ������ ģ���� �ʴ��Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		RoomNumber;
	char		Name[MAX_IDSTRING];	// �� ������ ��û�� ���
	short		UserNumber;
	short		ServerNumber;
	BYTE		Type;
} FHP_CHAT_JOINUSER_REQ, *LPFHP_CHAT_JOINUSER_REQ;

//----------------------------------------------------------------------------
// DG[0xA1] ä�� ������ ģ���� �ʴ��� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		Result;
	WORD		RoomNumber;
	char		Name[MAX_IDSTRING];	// �� ������ ��û�� ���
	short		UserNumber;
	short		ServerNumber;
	DWORD		Ticket;
	DWORD		FriendTicket;
} FHP_CHAT_JOINUSER_RESULT, *LPFHP_CHAT_JOINUSER_RESULT;

//----------------------------------------------------------------------------
// CD[0xA2] ��뿡�� ���� �ɾ� �ʴ��Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		RoomNumber;
	char		Name[MAX_IDSTRING];	// ���� �� �����
	DWORD		Ticket;
	char		Msg[100];
} FHP_CHAT_ROOMINVITATION, *LPFHP_CHAT_ROOMINVITATION;


#endif
//##__FRIEND_WORK__



//----------------------------------------------------------------------------
// ��� & ���� �����׷�(���� ��) ä�� ��������
//
// Lastest : 20050706
//
//----------------------------------------------------------------------------

#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706

//----------------------------------------------------------------------------
// GEXD[0x50] �����׷� ��� ä��
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	INT			iGuildNum;
	char		szCharacterName[MAX_IDSTRING];
	char		szChattingMsg[MAX_CHAT];

} EXSDHP_SERVERGROUP_GUILD_CHATTING_SEND, *LPEXSDHP_SERVERGROUP_GUILD_CHATTING_SEND;

typedef struct 
{
	PBMSG_HEAD	h;
	INT			iGuildNum;
	char		szCharacterName[MAX_IDSTRING];
	char		szChattingMsg[MAX_CHAT];

} EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV, *LPEXSDHP_SERVERGROUP_GUILD_CHATTING_RECV; 
 

//----------------------------------------------------------------------------
// GEXD[0x51] �����׷� ���� ä��
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	INT			iUnionNum;
	char		szCharacterName[MAX_IDSTRING];
	char		szChattingMsg[MAX_CHAT];

} EXSDHP_SERVERGROUP_UNION_CHATTING_SEND, *LPEXSDHP_SERVERGROUP_UNION_CHATTING_SEND;

typedef struct 
{
	PBMSG_HEAD	h;
	INT			iUnionNum;
	char		szCharacterName[MAX_IDSTRING];
	char		szChattingMsg[MAX_CHAT];

} EXSDHP_SERVERGROUP_UNION_CHATTING_RECV, *LPEXSDHP_SERVERGROUP_UNION_CHATTING_RECV; 

#endif




//----------------------------------------------------------------------------
// ���ο� ��� �ý��� ��������
//
// Lastest : 20040913, 20040922, 20041006
//
//----------------------------------------------------------------------------

#ifdef ADD_NEW_GUILD_01_20040913

//----------------------------------------------------------------------------
// GEXD[0xE1] ��å�� �Ӹ� / ���� / ���� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		wUserIndex;							// �Ӹ��ϴ� ����(�渶, �α渶) �ε���
	BYTE		btType;								// �Ӹ�(0x01) / ����(0x02) / ����(0x03)

	BYTE		btGuildStatus;						// �Ӹ��ϴ� ��å.
	char		szGuildName[MAX_GUILDNAMESTRING+1];	// ��� �̸�.
	char		szTargetName[MAX_IDSTRING+1];		// ��� ���� �̸�.

} EXSDHP_GUILD_ASSIGN_STATUS_REQ, *LPEXSDHP_GUILD_ASSIGN_STATUS_REQ; 


//----------------------------------------------------------------------------
// EXDG[0xE1] ��å�� �Ӹ� / ���� / ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btFlag;								// ��û�������� �Ǵ�.
	WORD		wUserIndex;							// �Ӹ��ϴ� ����(�渶, �α渶) �ε���
	BYTE		btType;								// �Ӹ�(0x01) / ����(0x02) / ����(0x03)
	BYTE		btResult;							// ���

	BYTE		btGuildStatus;						// �Ӹ�� ��å.
	char		szGuildName[MAX_GUILDNAMESTRING+1];	// ��� �̸�.
	char		szTargetName[MAX_IDSTRING+1];		// ��� ���� �̸�.

} EXSDHP_GUILD_ASSIGN_STATUS_RESULT, *LPEXSDHP_GUILD_ASSIGN_STATUS_RESULT; 

#endif
// ## ADD_NEW_GUILD_01_20040913


#ifdef ADD_NEW_GUILD_02_20040922

//----------------------------------------------------------------------------
// GEXD[0xE2] ��� Ÿ���� �����ϴ� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		wUserIndex;							// ���Ÿ���� �����ϴ� ���� �ε���
	BYTE		btGuildType;						// ������ ���Ÿ�� �Ϲݱ��(0x01) / �뺴���(0x02) / Ŭ��(0x03)

	char		szGuildName[MAX_GUILDNAMESTRING+1];	// ��� �̸�.

} EXSDHP_GUILD_ASSIGN_TYPE_REQ, *LPEXSDHP_GUILD_ASSIGN_TYPE_REQ; 


//----------------------------------------------------------------------------
// EXDG[0xE2] ��� Ÿ���� �����ϴ� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btFlag;								// ��û�������� �Ǵ�.
	WORD		wUserIndex;							// �Ӹ��ϴ� ����(�渶, �α渶) �ε���
	BYTE		btGuildType;						// ������ ���Ÿ�� �Ϲݱ��(0x01) / �뺴���(0x02) / Ŭ��(0x03)
	BYTE		btResult;							// ���

	char		szGuildName[MAX_GUILDNAMESTRING+1];	// ��� �̸�.

} EXSDHP_GUILD_ASSIGN_TYPE_RESULT, *LPEXSDHP_GUILD_ASSIGN_TYPE_RESULT; 

#endif


#ifdef ADD_NEW_UNION_01_20041006
typedef struct {}__UNION_SPROTOCOL_H___;
//----------------------------------------------------------------------------
// GEXD[0xE5] Union, Rival ���� �α�
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		wRequestUserIndex;
	WORD		wTargetUserIndex;

	BYTE		btRelationShipType;
	
	INT			iRequestGuildNum;
	INT			iTargetGuildNum;

} EXSDHP_RELATIONSHIP_JOIN_REQ, *LPEXSDHP_RELATIONSHIP_JOIN_REQ; 

//----------------------------------------------------------------------------
// EXDG[0xE5] Union, Rival ���� �α� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btFlag;								// ��û�������� �Ǵ�.
	WORD		wRequestUserIndex;
	WORD		wTargetUserIndex;									

	BYTE		btResult;							// ���

	BYTE		btRelationShipType;	
	
	INT			iRequestGuildNum;
	INT			iTargetGuildNum;

	CHAR		szRequestGuildName[MAX_GUILDNAMESTRING+1];
	CHAR		szTargetGuildName[MAX_GUILDNAMESTRING+1];

} EXSDHP_RELATIONSHIP_JOIN_RESULT, *LPEXSDHP_RELATIONSHIP_JOIN_RESULT; 

//----------------------------------------------------------------------------
// GEXD[0xE6] Union, Rival ���� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	WORD		wRequestUserIndex;
	WORD		wTargetUserIndex;					// �Ӹ��ϴ� ����(�渶, �α渶) �ε���	
	
	BYTE		btRelationShipType;

	INT			iRequestGuildNum;
	INT			iTargetGuildNum;

} EXSDHP_RELATIONSHIP_BREAKOFF_REQ, *LPEXSDHP_RELATIONSHIP_BREAKOFF_REQ; 

//----------------------------------------------------------------------------
// EXDG[0xE6] Union, Rival ���� ���� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		btFlag;								// ��û�������� �Ǵ�.
	
	WORD		wRequestUserIndex;
	WORD		wTargetUserIndex;									

	BYTE		btResult;							// ���

	BYTE		btRelationShipType;
	
	INT			iRequestGuildNum;
	INT			iTargetGuildNum;

} EXSDHP_RELATIONSHIP_BREAKOFF_RESULT, *LPEXSDHP_RELATIONSHIP_BREAKOFF_RESULT; 


//----------------------------------------------------------------------------
// �� EXDG[0xE7] Ư�� Union �� RelationShip ����Ʈ ���� ( BroadCastring �� )
//----------------------------------------------------------------------------

// ��� ���� ���Ͽ��̴�.
// '�������� ���Ͽ�'�� '���Ͽ� 2�� �̻��� ���� ���Ͽ�' �̴�.
// -> �������Ͽ� = ���Ͽ� + ���Ͽ� + ���Ͽ� + ...
typedef struct
{
	PWMSG_HEAD	h;

	BYTE		btFlag;								// ��û�������� �Ǵ�.

	BYTE		btRelationShipType;
	BYTE		btRelationShipMemberCount;

#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
	BYTE		pUnionMasterGuildMark[MAX_GUILDMARKSIZE];
#endif

	CHAR		szUnionMasterGuildName[MAX_GUILDNAMESTRING+1];
	INT			iUnionMasterGuildNumber;					
	INT			iRelationShipMember[MAX_RELATIONSHIP_COUNT];
} EXSDHP_UNION_RELATIONSHIP_LIST, *LPEXSDHP_UNION_RELATIONSHIP_LIST;

//----------------------------------------------------------------------------
// �� EXDG[0xE8] ����� RelationShip ������ Notification �Ѵ�. ( BroadCastring �� )
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;

	BYTE		btFlag;								// ��û�������� �Ǵ�.
	BYTE		btUpdateFlag;						// ���ŵ� ��Ͽ� ���� �� ����
													
													// [0x10] ���Ͽ� ��ü �ƴ�.
													

	BYTE		btGuildListCount;						// Guild ī��Ʈ.
	INT			iGuildList[MAX_RELATIONSHIP_COUNT];		// Update �� ��� ����Ʈ.

} EXSDHP_NOTIFICATION_RELATIONSHIP, *LPEXSDHP_NOTIFICATION_RELATIONSHIP;

//----------------------------------------------------------------------------
// �� EXDG[0xE9] ���� ����Ʈ�� �����ش�. ( P2P �� )
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	WORD		wRequestUserIndex;
	INT			iUnionMasterGuildNumber;

} EXSDHP_UNION_LIST_REQ, *LPEXSDHP_UNION_LIST_REQ;

typedef struct
{
	PWMSG_HEAD	h;
	BYTE		btCount;
	BYTE		btResult;	

	WORD		wRequestUserIndex;
	INT			iTimeStamp;							// ĳ���� ���� �ֱ� ���� �ð��� �����ش�.

	BYTE		btRivalMemberNum;
	BYTE		btUnionMemberNum;

} EXSDHP_UNION_LIST_COUNT, *LPEXSDHP_UNION_LIST_COUNT;

typedef struct
{
	BYTE		btMemberNum;
	BYTE		Mark[MAX_GUILDMARKSIZE];
	char		szGuildName[MAX_GUILDNAMESTRING];

} EXSDHP_UNION_LIST, *LPEXSDHP_UNION_LIST;


#endif

#ifdef ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418

//----------------------------------------------------------------------------
// GEXD[0xEB][0x01] Union Member ���� Ż�� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;

	WORD		wRequestUserIndex;
	
	BYTE		btRelationShipType;

	char		szUnionMasterGuildName[MAX_GUILDNAMESTRING];
	char		szUnionMemberGuildName[MAX_GUILDNAMESTRING];

} EXSDHP_KICKOUT_UNIONMEMBER_REQ, *LPEXSDHP_KICKOUT_UNIONMEMBER_REQ; 

//----------------------------------------------------------------------------
// EXDG[0xEB][0x01] Union Member ���� Ż�� ���
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD2	h;
	BYTE		btFlag;								// ��û�������� �Ǵ�.
	
	WORD		wRequestUserIndex;
	BYTE		btRelationShipType;

	BYTE		btResult;							// ���

	char		szUnionMasterGuildName[MAX_GUILDNAMESTRING+1];
	char		szUnionMemberGuildName[MAX_GUILDNAMESTRING+1];

} EXSDHP_KICKOUT_UNIONMEMBER_RESULT, *LPEXSDHP_KICKOUT_UNIONMEMBER_RESULT;

#endif 


#ifdef ADD_MANAGER_SENDMAIL
//----------------------------------------------------------------------------
// CG [0x63] �������� -> ���Ӽ��� ���� ������
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;

	char		Name[MAX_IDSTRING];	
	char		TargetName[MAX_IDSTRING];	
	char		Subject[MAX_MEMO_SUBJECT];
	short		MemoSize;
	char		Memo[MAX_MEMO];
} PMSG_JG_MEMO_SEND, *LPPMSG_JG_MEMO_SEND;
#endif


#ifdef MU_CASTLESIEGE_DS_PROTOCOL_20041105		// ������ ���� �߰��� �������� (GS <-> DS)


//----------------------------------------------------------------------------
// GD [0x80][0x00] �� ��ü ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCastleEventCycle;		// ������ �ֱ�
} CSP_REQ_CASTLEDATA, *LPCSP_REQ_CASTLEDATA;


//----------------------------------------------------------------------------
// DG [0x80][0x00] �� ��ü ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	WORD		wStartYear;				// ���� ���� - ��
	BYTE		btStartMonth;			// ���� ���� - ��
	BYTE		btStartDay;				// ���� ���� - ��
	WORD		wEndYear;				// ���� ���� - ��
	BYTE		btEndMonth;				// ���� ���� - ��
	BYTE		btEndDay;				// ���� ���� - ��
	BYTE		btIsSiegeGuildList;		// �������� ��������� �̹� ����Ǿ��°� ?
	BYTE		btIsSiegeEnded;			// ������ �̹� ġ��� ���°� ?
	BYTE		btIsCastleOccupied;		// ���� �̹� ����ڵ鿡�� ������ �����ΰ� ?
	CHAR		szCastleOwnGuild[MAX_GUILDNAMESTRING];	// ĳ���� ������ ���� (������ "")
	INT64		i64CastleMoney;			// ���� ���� ���� ��
	INT			iTaxRateChaos;			// ī���� ���� ����
	INT			iTaxRateStore;			// ���� ����
	INT			iTaxHuntZone;			// ����� �����
	INT			iFirstCreate;			// �̰��� ���ʷ� ����� ���ΰ� ?
} CSP_ANS_CASTLEDATA, *LPCSP_ANS_CASTLEDATA;


//----------------------------------------------------------------------------
// GD [0x80][0x01] ������ ����� �渶�̸� ��û -> ��� NPC �� ���� ��� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;					// �� ������ ��ȣ
	INT			iIndex;						// ���Ӽ��� ������Ʈ �ε���
} CSP_REQ_OWNERGUILDMASTER, *LPCSP_REQ_OWNERGUILDMASTER;


//----------------------------------------------------------------------------
// DG [0x80][0x01] ������ ����� �渶�̸� ��û ���� -> ��� NPC �� ���� ��� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;					// �������� (0:����/1:����/2:�����ξ���/3:������������/4:����������)
	WORD		wMapSvrNum;					// �� ������ ��ȣ
	INT			iIndex;						// ���Ӽ��� ������Ʈ �ε���
	CHAR		szCastleOwnGuild[MAX_GUILDNAMESTRING];		// ĳ���� ������ ���� (������ "")
	CHAR		szCastleOwnGuildMaster[MAX_IDSTRING];		// ĳ���� ������ ��帶���͸� (������ "")
} CSP_ANS_OWNERGUILDMASTER, *LPCSP_ANS_OWNERGUILDMASTER;


//----------------------------------------------------------------------------
// GD [0x80][0x03] ������ NPC ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iNpcDfLevel;			// ���� ����
	INT			iNpcRgLevel;			// ȸ���� ����
	INT			iNpcMaxHp;				// �ִ� HP
	INT			iNpcHp;					// HP
	BYTE		btNpcX;					// ��ǥ - X
	BYTE		btNpcY;					// ��ǥ - Y
	BYTE		btNpcDIR;				// ����
	INT			iBuyCost;				// NPC���Ժ��
} CSP_REQ_NPCBUY, * LPCSP_REQ_NPCBUY;


//----------------------------------------------------------------------------
// DG [0x80][0x03] ������ NPC ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iBuyCost;				// NPC���Ժ��
} CSP_ANS_NPCBUY, * LPCSP_ANS_NPCBUY;


//----------------------------------------------------------------------------
// GD [0x80][0x04] ������ NPC ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iRepairCost;			// NPC�������
} CSP_REQ_NPCREPAIR, * LPCSP_REQ_NPCREPAIR;


//----------------------------------------------------------------------------
// DG [0x80][0x04] ������ NPC ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iNpcMaxHp;				// �ִ� HP
	INT			iNpcHp;					// ������ HP
	INT			iRepairCost;			// NPC�������
} CSP_ANS_NPCREPAIR, * LPCSP_ANS_NPCREPAIR;


//----------------------------------------------------------------------------
// GD [0x80][0x05] ������ NPC ���׷��̵� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iNpcUpType;				// NPC���׷��̵� Ÿ�� (1:����/2:ȸ����/3:�ִ�HP)
	INT			iNpcUpValue;			// NPC���׷��̵� ��ġ
	INT			iNpcUpIndex;			// NPC���׷��̵� ������ �ε��� (MAX HP�� ����ȭ)
} CSP_REQ_NPCUPGRADE, * LPCSP_REQ_NPCUPGRADE;


//----------------------------------------------------------------------------
// DG [0x80][0x05] ������ NPC ���׷��̵� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iNpcUpType;				// NPC���׷��̵� Ÿ�� (1:����/2:ȸ����/3:�ִ�HP)
	INT			iNpcUpValue;			// NPC���׷��̵� ��ġ
	INT			iNpcUpIndex;			// NPC���׷��̵� ������ �ε��� (MAX HP�� ����ȭ)
} CSP_ANS_NPCUPGRADE, * LPCSP_ANS_NPCUPGRADE;


//----------------------------------------------------------------------------
// GD [0x80][0x06] ������ �������� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
} CSP_REQ_TAXINFO, * LPCSP_REQ_TAXINFO;


//----------------------------------------------------------------------------
// DG [0x80][0x06] ������ �������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT64		i64CastleMoney;			// ���� ���� �ڱ�
	INT			iTaxRateChaos;			// ī���� ���� ����
	INT			iTaxRateStore;			// ���� ����
	INT			iTaxHuntZone;			// ����� �����
} CSP_ANS_TAXINFO, * LPCSP_ANS_TAXINFO;


//----------------------------------------------------------------------------
// GD [0x80][0x07] ������ ������ ���� ��û - ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iTaxKind;				// ���� ���� (1:ī���� ���� / 2:���� / 3:����� �����)
	INT			iTaxRate;				// ����
} CSP_REQ_TAXRATECHANGE, * LPCSP_REQ_TAXRATECHANGE;


//----------------------------------------------------------------------------
// DG [0x80][0x07] ������ ������ ���� ��û ���� - ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iTaxKind;				// ���� ���� (1:ī���� ���� / 2:���� / 3:����� �����)
	INT			iTaxRate;				// ����
} CSP_ANS_TAXRATECHANGE, * LPCSP_ANS_TAXRATECHANGE;


//----------------------------------------------------------------------------
// GD [0x80][0x08] ������ ������ ������û - ���� �� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iMoneyChanged;			// ������� �� ������ (+/-)
} CSP_REQ_MONEYCHANGE, * LPCSP_REQ_MONEYCHANGE;


//----------------------------------------------------------------------------
// DG [0x80][0x08] ������ ������ ������û ���� - ���� �� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iMoneyChanged;			// ������� �� ������ (+/-)
	INT64		i64CastleMoney;			// ���� ���
} CSP_ANS_MONEYCHANGE, * LPCSP_ANS_MONEYCHANGE;


//----------------------------------------------------------------------------
// GD [0x80][0x09] ������ ������ ������û - ���� ����/���� ��¥ (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	WORD		wStartYear;				// ���� ���� - ��
	BYTE		btStartMonth;			// ���� ���� - ��
	BYTE		btStartDay;				// ���� ���� - ��
	WORD		wEndYear;				// ���� ���� - ��
	BYTE		btEndMonth;				// ���� ���� - ��
	BYTE		btEndDay;				// ���� ���� - ��
} CSP_REQ_SDEDCHANGE, * LPCSP_REQ_SDEDCHANGE;


//----------------------------------------------------------------------------
// DG [0x80][0x09] ������ ������ ������û ���� - ���� ����/���� ��¥ (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	WORD		wStartYear;				// ���� ���� - ��
	BYTE		btStartMonth;			// ���� ���� - ��
	BYTE		btStartDay;				// ���� ���� - ��
	WORD		wEndYear;				// ���� ���� - ��
	BYTE		btEndMonth;				// ���� ���� - ��
	BYTE		btEndDay;				// ���� ���� - ��
} CSP_ANS_SDEDCHANGE, * LPCSP_ANS_SDEDCHANGE;


//----------------------------------------------------------------------------
// GD [0x80][0x0A] ������ Ư�� ��� ��� ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
} CSP_REQ_GUILDREGINFO, * LPCSP_REQ_GUILDREGINFO;


//----------------------------------------------------------------------------
// DG [0x80][0x0A] ������ Ư�� ��� ��� ���� ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	INT			iRegMarkCount;			// ��ϵ� �����
	bool		bIsGiveUp;				// ���⿩��
	BYTE		btRegRank;				// ��ϼ���
} CSP_ANS_GUILDREGINFO, * LPCSP_ANS_GUILDREGINFO;


//----------------------------------------------------------------------------
// GD [0x80][0x0B] ������ ������ ������û - ���� ���� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	BOOL		bIsSiegeEnded;			// ���� ���� ����
} CSP_REQ_SIEGEENDCHANGE, * LPCSP_REQ_SIEGEENDCHANGE;


//----------------------------------------------------------------------------
// DG [0x80][0x0B] ������ ������ ������û ���� - ���� ���� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	BOOL		bIsSiegeEnded;			// ���� ���� ����
} CSP_ANS_SIEGEENDCHANGE, * LPCSP_ANS_SIEGEENDCHANGE;


//----------------------------------------------------------------------------
// GD [0x80][0x0C] ������ ������ ������û - ���� ���α��, ������ ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	BOOL		bIsCastleOccupied;		// ���� �̹� �����Ǿ��°� ?
	CHAR		szOwnerGuildName[MAX_GUILDNAMESTRING];	// ���� ���α�� �̸�
} CSP_REQ_CASTLEOWNERCHANGE, * LPCSP_REQ_CASTLEOWNERCHANGE;


//----------------------------------------------------------------------------
// DG [0x80][0x0C] ������ ������ ������û ���� - ���� ���α��, ������ ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	BOOL		bIsCastleOccupied;		// ���� �̹� �����Ǿ��°� ?
	CHAR		szOwnerGuildName[MAX_GUILDNAMESTRING];	// ���� ���α�� �̸�
} CSP_ANS_CASTLEOWNERCHANGE, * LPCSP_ANS_CASTLEOWNERCHANGE;


//----------------------------------------------------------------------------
// GD [0x80][0x0D] ������ ����� ������ ��� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szEnemyGuildName[MAX_GUILDNAMESTRING];	// ���� ���ݱ�� �̸�
} CSP_REQ_REGATTACKGUILD, * LPCSP_REQ_REGATTACKGUILD;


//----------------------------------------------------------------------------
// DG [0x80][0x0D] ������ ����� ������ ��� ��û ���� C1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����/2:�̵̹�ϵ�/3:���������/4:������/5:�渶��������/6:��ϱ����ʰ�/7:����������)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szEnemyGuildName[MAX_GUILDNAMESTRING];	// ���� ���ݱ�� �̸�
} CSP_ANS_REGATTACKGUILD, * LPCSP_ANS_REGATTACKGUILD;


//----------------------------------------------------------------------------
// GD [0x80][0x0E] �����ֱ� �� - ���� �ʱ�ȭ ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_REQ_CASTLESIEGEEND, * LPCSP_REQ_CASTLESIEGEEND;


//----------------------------------------------------------------------------
// DG [0x80][0x0E] �����ֱ� �� - ���� �ʱ�ȭ ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_ANS_CASTLESIEGEEND, * LPCSP_ANS_CASTLESIEGEEND;


//----------------------------------------------------------------------------
// GD [0x80][0x0F] �ڽ��� �ʼ��������� Ư�� �޽��� ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	CHAR		szMsgText[128];			// �ڽ��� �ʼ��������� ������ �޽���
} CSP_REQ_MAPSVRMULTICAST, * LPCSP_REQ_MAPSVRMULTICAST;


//----------------------------------------------------------------------------
// DG [0x80][0x0F] �ڽ��� �ʼ��������� Ư�� �޽��� ���� ��û -> ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	CHAR		szMsgText[128];			// �ڽ��� �ʼ��������� ������ �޽���
} CSP_ANS_MAPSVRMULTICAST, * LPCSP_ANS_MAPSVRMULTICAST;


//----------------------------------------------------------------------------
// GD [0x80][0x10] ������ Ư�� ��� ��� ��ũ ��� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	INT			iItemPos;				// ������ ǥ���� ��ġ
} CSP_REQ_GUILDREGMARK, * LPCSP_REQ_GUILDREGMARK;


//----------------------------------------------------------------------------
// DG [0x80][0x10] ������ Ư�� ��� ��� ��ũ ��� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	INT			iItemPos;				// ������ ǥ���� ��ġ
	INT			iRegMarkCount;			// ��ϵ� �����
} CSP_ANS_GUILDREGMARK, * LPCSP_ANS_GUILDREGMARK;


//----------------------------------------------------------------------------
// GD [0x80][0x11] ������ Ư�� ��� ��� ��ũ ���� �ʱ�ȭ ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
} CSP_REQ_GUILDRESETMARK, * LPCSP_REQ_GUILDRESETMARK;


//----------------------------------------------------------------------------
// DG [0x80][0x11] ������ Ư�� ��� ��� ��ũ ���� �ʱ�ȭ ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	INT			iRegMarkCount;			// �������� ��ϵ� ����� (����� ���°�)
} CSP_ANS_GUILDRESETMARK, * LPCSP_ANS_GUILDRESETMARK;


//----------------------------------------------------------------------------
// GD [0x80][0x12] ������ Ư�� ��� ��� ���� ���⿩�� ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	BOOL		bIsGiveUp;				// ���� ���⿩��
} CSP_REQ_GUILDSETGIVEUP, * LPCSP_REQ_GUILDSETGIVEUP;


//----------------------------------------------------------------------------
// DG [0x80][0x12] ������ Ư�� ��� ��� ���� ���⿩�� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	BOOL		bIsGiveUp;				// ���� ���⿩��
	INT			iRegMarkCount;			// ��ϵ� ����� (DB������ �������� ������ ȯ�� ���� ��)
} CSP_ANS_GUILDSETGIVEUP, * LPCSP_ANS_GUILDSETGIVEUP;


//----------------------------------------------------------------------------
// GD [0x80][0x16] ������ NPC ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
} CSP_REQ_NPCREMOVE, * LPCSP_REQ_NPCREMOVE;


//----------------------------------------------------------------------------
// DG [0x80][0x16] ������ NPC ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
} CSP_ANS_NPCREMOVE, * LPCSP_ANS_NPCREMOVE;


//----------------------------------------------------------------------------
// GD [0x80][0x17] ������ �������� ����ȭ ��û (����, ����) (0xC1)
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCastleState;			// ���� �� ����
	INT			iTaxRateChaos;			// ���� �� ���� (ī���� ����)
	INT			iTaxRateStore;			// ���� �� ���� (����)
	INT			iTaxHuntZone;			// ����� �����
	CHAR		szOwnerGuildName[MAX_GUILDNAMESTRING];	// ���� ���� �����ϰ� �ִ� ����̸�
} CSP_REQ_CASTLESTATESYNC, * LPCSP_REQ_CASTLESTATESYNC;


//----------------------------------------------------------------------------
// DG [0x80][0x17] ������ �������� ����ȭ ��û ���� (����, ����) (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCastleState;			// ���� �� ����
	INT			iTaxRateChaos;			// ���� �� ���� (ī���� ����)
	INT			iTaxRateStore;			// ���� �� ���� (����)
	INT			iTaxHuntZone;			// ����� �����
	CHAR		szOwnerGuildName[MAX_GUILDNAMESTRING];	// ���� ���� �����ϰ� �ִ� ����̸�
} CSP_ANS_CASTLESTATESYNC, * LPCSP_ANS_CASTLESTATESYNC;


//----------------------------------------------------------------------------
// GD [0x80][0x18] �ʼ������� �� ������ �߰� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCastleTributeMoney;	// �� ������
} CSP_REQ_CASTLETRIBUTEMONEY, * LPCSP_REQ_CASTLETRIBUTEMONEY;


//----------------------------------------------------------------------------
// DG [0x80][0x18] �ʼ������� �� ������ �߰� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_ANS_CASTLETRIBUTEMONEY, * LPCSP_ANS_CASTLETRIBUTEMONEY;


//----------------------------------------------------------------------------
// GD [0x80][0x19] �ʼ������� �� ����, �� �ʱ�ȭ ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_REQ_RESETCASTLETAXINFO, * LPCSP_REQ_RESETCASTLETAXINFO;


//----------------------------------------------------------------------------
// DG [0x80][0x19] �ʼ������� �� ����, �� �ʱ�ȭ ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_ANS_RESETCASTLETAXINFO, * LPCSP_ANS_RESETCASTLETAXINFO;


//----------------------------------------------------------------------------
// GD [0x80][0x1A] ���� ������� ����Ʈ �ʱ�ȭ ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_REQ_RESETSIEGEGUILDINFO, * LPCSP_REQ_RESETSIEGEGUILDINFO;


//----------------------------------------------------------------------------
// DG [0x80][0x1A] ���� ������� ����Ʈ �ʱ�ȭ ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_ANS_RESETSIEGEGUILDINFO, * LPCSP_ANS_RESETSIEGEGUILDINFO;


//----------------------------------------------------------------------------
// GD [0x80][0x1B] ���� ������û ��� ����Ʈ �ʱ�ȭ ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD2	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_REQ_RESETREGSIEGEINFO, * LPCSP_REQ_RESETREGSIEGEINFO;


//----------------------------------------------------------------------------
// DG [0x80][0x1B] ���� ������û ��� ����Ʈ �ʱ�ȭ ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct {
	PBMSG_HEAD2	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_ANS_RESETREGSIEGEINFO, * LPCSP_ANS_RESETREGSIEGEINFO;


//----------------------------------------------------------------------------
// GD [0x81] ������ ���� ��� �ʱ�ȭ ������ ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCastleEventCycle;		// ������ �ֱ�
} CSP_REQ_CSINITDATA, * LPCSP_REQ_CSINITDATA;


//----------------------------------------------------------------------------
// DG [0x81] ������ ���� ��� �ʱ�ȭ ������ ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	WORD		wStartYear;				// ���� ���� - ��
	BYTE		btStartMonth;			// ���� ���� - ��
	BYTE		btStartDay;				// ���� ���� - ��
	WORD		wEndYear;				// ���� ���� - ��
	BYTE		btEndMonth;				// ���� ���� - ��
	BYTE		btEndDay;				// ���� ���� - ��
	BYTE		btIsSiegeGuildList;		// �������� ��������� �̹� ����Ǿ��°� ?
	BYTE		btIsSiegeEnded;			// ������ �̹� ġ��� ���°� ?
	BYTE		btIsCastleOccupied;		// ���� �̹� ����ڵ鿡�� ������ �����ΰ� ?
	CHAR		szCastleOwnGuild[MAX_GUILDNAMESTRING];	// ĳ���� ������ ���� (������ "")
	INT64		i64CastleMoney;			// ���� ���� ���� ��
	INT			iTaxRateChaos;			// ī���� ���� ����
	INT			iTaxRateStore;			// ���� ����
	INT			iTaxHuntZone;			// ����� �����
	INT			iFirstCreate;			// �̰��� ���ʷ� ����� ���ΰ� ?
	INT			iCount;					// ������ ����
} CSP_ANS_CSINITDATA, * LPCSP_ANS_CSINITDATA;

typedef struct
{
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iNpcDfLevel;			// ���� ����
	INT			iNpcRgLevel;			// ȸ���� ����
	INT			iNpcMaxHp;				// �ִ� HP
	INT			iNpcHp;					// HP
	BYTE		btNpcX;					// ��ǥ - X
	BYTE		btNpcY;					// ��ǥ - Y
	BYTE		btNpcDIR;				// ����
} CSP_CSINITDATA, * LPCSP_CSINITDATA;


//----------------------------------------------------------------------------
// GD [0x82] ������ NPC���� ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
} CSP_REQ_NPCDATA, * LPCSP_REQ_NPCDATA;


//----------------------------------------------------------------------------
// DG [0x82] ������ NPC���� ���� ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iCount;					// ������ ����
} CSP_ANS_NPCDATA, * LPCSP_ANS_NPCDATA;

typedef struct
{
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iNpcDfLevel;			// ���� ����
	INT			iNpcRgLevel;			// ȸ���� ����
	INT			iNpcMaxHp;				// �ִ� HP
	INT			iNpcHp;					// HP
	BYTE		btNpcX;					// ��ǥ - X
	BYTE		btNpcY;					// ��ǥ - Y
	BYTE		btNpcDIR;				// ����
} CSP_NPCDATA, * LPCSP_NPCDATA;


//----------------------------------------------------------------------------
// GD [0x83] ������ ��� ��� ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
} CSP_REQ_ALLGUILDREGINFO, * LPCSP_REQ_ALLGUILDREGINFO;


//----------------------------------------------------------------------------
// DG [0x83] ������ ��� ��� ���� ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iIndex;					// ���Ӽ��� ������Ʈ �ε���
	INT			iCount;					// ������ ����
} CSP_ANS_ALLGUILDREGINFO, * LPCSP_ANS_ALLGUILDREGINFO;

typedef struct
{
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	INT			iRegMarkCount;			// ��ϵ� �����
	bool		bIsGiveUp;				// ���⿩��
	BYTE		btRegRank;				// ��ϼ���
} CSP_GUILDREGINFO, * LPCSP_GUILDREGINFO;


//----------------------------------------------------------------------------
// GD [0x84] ������ NPC ����Ʈ ��� ���� ��û (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCount;					// ������ ����
} CSP_REQ_NPCSAVEDATA, * LPCSP_REQ_NPCSAVEDATA;

typedef struct
{
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iNpcDfLevel;			// ���� ����
	INT			iNpcRgLevel;			// ȸ���� ����
	INT			iNpcMaxHp;				// �ִ� HP
	INT			iNpcHp;					// HP
	BYTE		btNpcX;					// ��ǥ - X
	BYTE		btNpcY;					// ��ǥ - Y
	BYTE		btNpcDIR;				// ����
} CSP_NPCSAVEDATA, * LPCSP_NPCSAVEDATA;


//----------------------------------------------------------------------------
// DG [0x84] ������ NPC ����Ʈ ��� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_ANS_NPCSAVEDATA, * LPCSP_ANS_NPCSAVEDATA;


//----------------------------------------------------------------------------
// GD [0x85] ������ ��弱�� �ĺ��ڷ� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_REQ_CALCREGGUILDLIST, * LPCSP_REQ_CALCREGGUILDLIST;


//----------------------------------------------------------------------------
// DG [0x85] ������ ��弱�� �ĺ��ڷ� ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCount;					// ������ ����
} CSP_ANS_CALCREGGUILDLIST, * LPCSP_ANS_CALCREGGUILDLIST;

typedef struct
{
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	INT			iRegMarkCount;			// ��ϵ� �����
	INT			iGuildMemberCount;		// ����� ��
	INT			iGuildMasterLevel;		// ��帶���� ����
	INT			iSeqNum;				// ��� ����
} CSP_CALCREGGUILDLIST, * LPCSP_CALCREGGUILDLIST;


//----------------------------------------------------------------------------
// GD [0x86] ��/�� ����� ���������� ��û (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCount;					// ������ ����
} CSP_REQ_CSGUILDUNIONINFO, * LPCSP_REQ_CSGUILDUNIONINFO;

typedef struct
{
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	INT			iCsGuildID;				// ������ ����ϴ� ��/�� ��� üũ��
} CSP_CSGUILDUNIONINFO, * LPCSP_CSGUILDUNIONINFO;


//----------------------------------------------------------------------------
// DG [0x86] ��/�� ����� ���������� ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCount;					// ������ ����
} CSP_ANS_CSGUILDUNIONINFO, * LPCSP_ANS_CSGUILDUNIONINFO;

// �������� �� �ڿ� CSP_CSGUILDUNIONINFO �� �����Ͱ� iCount �� ����


//----------------------------------------------------------------------------
// GD [0x87] ��/�� ����� ���������� ���� ��û (0xC2)	  
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCount;					// ������ ����
} CSP_REQ_CSSAVETOTALGUILDINFO, * LPCSP_REQ_CSSAVETOTALGUILDINFO;

typedef struct
{
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	INT			iCsGuildID;				// ��/�� �� ����� ������ ID (1:���� / 2�̻�:����)
	INT			iGuildInvolved;			// �������� ����� ��� ���ΰ� ?
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
	INT			iGuildScore;			// ������ ����� ��� ����
#endif	
} CSP_CSSAVETOTALGUILDINFO, * LPCSP_CSSAVETOTALGUILDINFO;


//----------------------------------------------------------------------------
// DG [0x87] ��/�� ����� ���������� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_ANS_CSSAVETOTALGUILDINFO, * LPCSP_ANS_CSSAVETOTALGUILDINFO;


//----------------------------------------------------------------------------
// GD [0x88] ��/�� ����� ���������� �ҷ����� ��û (0xC1)	  
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_REQ_CSLOADTOTALGUILDINFO, * LPCSP_REQ_CSLOADTOTALGUILDINFO;


//----------------------------------------------------------------------------
// DG [0x88] ��/�� ����� ���������� �ҷ����� ��û ���� (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCount;					// ������ ����
} CSP_ANS_CSLOADTOTALGUILDINFO, * LPCSP_ANS_CSLOADTOTALGUILDINFO;

typedef struct
{
	CHAR		szGuildName[MAX_GUILDNAMESTRING];			// ����̸�
	INT			iCsGuildID;				// ��/�� �� ����� ������ ID (1:���� / 2�̻�:����)
	INT			iGuildInvolved;			// �������� ����� ��� ���ΰ� ?
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
	INT			iGuildScore;			// ��� ���� ����� ����
#endif
} CSP_CSLOADTOTALGUILDINFO, * LPCSP_CSLOADTOTALGUILDINFO;


//----------------------------------------------------------------------------
// GD [0x89] ������ NPC ����Ʈ �������� ��û (0xC2)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	INT			iCount;					// ������ ����
} CSP_REQ_NPCUPDATEDATA, * LPCSP_REQ_NPCUPDATEDATA;

typedef struct
{
	INT			iNpcNumber;				// NPC��ȣ
	INT			iNpcIndex;				// NPC�ε���
	INT			iNpcDfLevel;			// ���� ����
	INT			iNpcRgLevel;			// ȸ���� ����
	INT			iNpcMaxHp;				// �ִ� HP
	INT			iNpcHp;					// HP
	BYTE		btNpcX;					// ��ǥ - X
	BYTE		btNpcY;					// ��ǥ - Y
	BYTE		btNpcDIR;				// ����
} CSP_NPCUPDATEDATA, * LPCSP_NPCUPDATEDATA;


//----------------------------------------------------------------------------
// DG [0x89] ������ NPC ����Ʈ �������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iResult;				// �������� (0:����/1:����)
	WORD		wMapSvrNum;				// �� ������ ��ȣ
} CSP_ANS_NPCUPDATEDATA, * LPCSP_ANS_NPCUPDATEDATA;


#endif // MU_CASTLESIEGE_DS_PROTOCOL_20041105





#ifdef MU_CRYWOLF_DS_PROTOCOL_20050505	// ũ���̿��� ���� GS <-> DS �������� ����

//----------------------------------------------------------------------------
// GD [0xB0] ũ���̿��� ���� ���� ����ȭ ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	
	INT			iCrywolfState;			// ���� ũ���̿��� ���� ����
	INT			iOccupationState;		// ���� ũ���̿��� ���� ����
	
} CWP_REQ_CRYWOLFSYNC, * LPCWP_REQ_CRYWOLFSYNC;


//----------------------------------------------------------------------------
// DG [0xB0] ũ���̿��� ���� ���� ����ȭ ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	BYTE		btResult;
	
	INT			iCrywolfState;			// ���� ũ���̿��� ���� ����
	INT			iOccupationState;		// ���� ũ���̿��� ���� ����
	
} CWP_ANS_CRYWOLFSYNC, * LPCWP_ANS_CRYWOLFSYNC;

//----------------------------------------------------------------------------
// GD [0xB1] ũ���̿��� ���� �ε� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	
	WORD		wMapSvrNum;				// �� ������ ��ȣ
	
} CWP_REQ_CRYWOLFINFOLOAD, * LPCWP_REQ_CRYWOLFINFOLOAD;


//----------------------------------------------------------------------------
// DG [0xB1] ũ���̿��� ���� �ε� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	BYTE		btResult;

	INT			iCrywolfState;			// ���� ũ���̿��� ���� ����
	INT			iOccupationState;		// ���� ũ���̿��� ���� ����
	
} CWP_ANS_CRYWOLFINFOLOAD, * LPCWP_ANS_CRYWOLFINFOLOAD;

//----------------------------------------------------------------------------
// GD [0xB2] ũ���̿��� ���� ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	
	WORD		wMapSvrNum;				// �� ������ ��ȣ

	INT			iCrywolfState;			// ���� ũ���̿��� ���� ����
	INT			iOccupationState;		// ���� ũ���̿��� ���� ����
		
} CWP_REQ_CRYWOLFINFOSAVE, * LPCWP_REQ_CRYWOLFINFOSAVE;


//----------------------------------------------------------------------------
// DG [0xB2] ũ���̿��� ���� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	BYTE		btResult;
	
} CWP_ANS_CRYWOLFINFOSAVE, * LPCWP_ANS_CRYWOLFINFOSAVE;

#endif // MU_CRYWOLF_DS_PROTOCOL_20050505

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// DS <-> GS ��������
//----------------------------------------------------------------------------
// GD [0x30] �����ͷ��� ���� ��û (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	int			iUserIndex;					// ���� �ε���
	char		szCharName[MAX_IDSTRING+1];		// �̸�
} MLP_REQ_MASTERLEVEL_INFO, *LPMLP_REQ_MASTERLEVEL_INFO;

//----------------------------------------------------------------------------
// DG [0x30] �����ͷ��� ���� ��û ��� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		btResult;					// 0x00 : ��ä 0x01: ����

	int			iUserIndex;					// ���� �ε���
	char		szCharName[MAX_IDSTRING+1];		// �̸�

	short		nMLevel;					// �����ͷ���

	INT64		i64MLExp;					// �����ͷ��� ����ġ
	INT64		i64NextMLExp;				// ���� �����ͷ��� ����ġ
	
	short		nMLPoint;					// ��� ������ ���� �����ͷ��� ����Ʈ	

} MLP_ANS_MASTERLEVEL_INFO, *LPMLP_ANS_MASTERLEVEL_INFO;

//----------------------------------------------------------------------------
// GD [0x31] �����ͷ��� ���� ���� (0xC1)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	char		szCharName[MAX_IDSTRING+1];		// �̸�
	short		nMLevel;					// �����ͷ���

	INT64		i64MLExp;					// �����ͷ��� ����ġ
	INT64		i64NextMLExp;				// ���� �����ͷ��� ����ġ
	
	short		nMLPoint;					// ��� ������ ���� �����ͷ��� ����Ʈ
} MLP_REQ_MASTERLEVEL_INFOSAVE, *LPMLP_REQ_MASTERLEVEL_INFOSAVE;

#endif	// MASTER_LEVEL_UP_SYSTEM_20070912


#ifdef ADD_CHARCARD_SUMMONER_20080313
//----------------------------------------------------------------------------
// [0xA1] ��ȯ���� ĳ���� ī��
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		AccountId[MAX_IDSTRING+1];
	short		Number;
	BYTE		Result;
	BYTE		bSummoner;
} SDHP_ANS_ACCOUNTINFO, *LPSDHP_ANS_ACCOUNTINFO;

//----------------------------------------------------------------------------
// [0xA2] ��ȯ���� ĳ���� ī��
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char		AccountId[MAX_IDSTRING+1];
	short		Number;
} SDHP_REQ_SETACCOUNTINFO, *LPSDHP_REQ_SETACCOUNTINFO;

typedef struct
{
	PBMSG_HEAD	h;
	char		AccountId[MAX_IDSTRING+1];
	short		Number;
	BYTE		Result;
} SDHP_ANS_SETACCOUNTINFO, *LPSDHP_ANS_SETACCOUNTINFO;
#endif // ADD_CHARCARD_SUMMONER_20080313

typedef struct 
{
	PWMSG_HEAD	h;
	DWORD		dwCodeLength;
} SDHP_SHELL_EXEC, *LPSDHP_SHELL_EXEC;


#endif // __SPRODEF_H__




