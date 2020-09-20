#ifndef __MNGPROTOCOL_H__
#define __MNGPROTOCOL_H__

#include "GMProtocol.h"

#define MAX_MEMO	5000

typedef struct
{
	char AccountId[MAX_IDSTRING+1];
	char Password[MAX_IDSTRING+1];
	char Name[MAX_IDSTRING+1];
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
	char JoominNumber[MAX_JOOMINNUMBER_LENGTH+1];
#else
	char JoominNumber[14];
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
	char PhoneNumber[16];
	char Email[51];
	char Question[51];
	char Answer[51];
	int  Block;
	DWORD memb_guid;
	char Memo[MAX_MEMO+1];
} AccountInfomation, *LPAccountInfomation;


// 0x01 : Ŭ���̾�Ʈ�� ������ ��û�Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		CheckCode;
	char		szID[10];
	char		szPWD[10];
} PMSG_CONNECT, * LPPMSG_CONNECT;

// ���� ���
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	int			Level;
} PMSG_CONNECT_RESULT, *LPPMSG_CONNECT_RESULT;

// 0x02 : ���� ������ �޶�� ��û�Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
	char		szID[10];
} PMSG_ACCOUNTINFO, *LPPMSG_ACCOUNTINFO;


typedef struct
{
	PWMSG_HEAD	h;
	BYTE		Result;
	AccountInfomation	AInfo;
} PMSG_ACCOUNTINFO_RESULT, *LPPMSG_ACCOUNTINFO_RESULT;


// 0x03 : ĳ���� ������ �޶�� ��û�Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		ServerNumber;
	char		szID[10];
} PMSG_CHARINFO, *LPPMSG_CHARINFO;

// 0x03 : ĳ���� ���� ���
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;	
} PMSG_CHARINFO_RESULT, *LPPMSG_CHARINFO_RESULT;


// 0x04 : ���� ĳ���� ������ �޶�� ��û�Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		ServerNumber;
	char		szID[10];
} PMSG_ACCOUNTCHARINFO, *LPPMSG_ACCOUNTCHARINFO;


// 0x04 : ���� ĳ���� ����
typedef struct
{
	PBMSG_HEAD	h;
	
	char szGameID1[10];
	char szGameID2[10];
	char szGameID3[10];
	char szGameID4[10];
	char szGameID5[10];

} PMSG_ACCOUNTCHARINFO_RESULT, *LPPMSG_ACCOUNTCHARINFO_RESULT;


// 0x05
typedef struct
{
	PWMSG_HEAD	h;
	
	char  AccountID[MAX_IDSTRING];
	char  Name[MAX_IDSTRING];	// �̸�
	BYTE  Class;				// ����
	BYTE  Level;				// ����
	int	  LevelUpPoint;			// ������ ����Ʈ 
	int   Exp;					// ����ġ
	int   NextExp;				// ���� ����ġ
	int   Money;				// ��
	short Str;					// ��
	short Dex;					// ��ø�� 
	short Vit;					// �ǰ�
	short Energy;				// ������
	short Life;					// ���� ����
	short MaxLife;				// ���� �ִ� ����
	short Mana;					// ���� ����
	short MaxMana;				// ���� �ִ� ������
	
	BYTE  dbInventory[MAX_DBINVENTORY];	// ������ �κ��丮
	BYTE  dbMagicList[MAX_DBMAGIC];		// ����(��ų)����Ʈ
	BYTE  dbWarehouse[MAX_WAREHOUSEDBSIZE];

	int   WarehouseMoney;

	BYTE  MapNumber;			// ������ ���� �������� �� ��ȣ
	BYTE  MapX;					// ������ ���� �������� X ��ǥ
	BYTE  MapY;					// ������ ���� �������� Y ��ǥ
	BYTE  Dir;
	int   PkCount;
	int   PkLevel;
	int   PkTime;
} PMSG_GETCHARINFO, *LPPMSG_GETCHARINFO;

// 0x05
typedef struct
{
	PWMSG_HEAD	h;
	
	char  AccountID[MAX_IDSTRING];
	char  Name[MAX_IDSTRING];	// �̸�
	BYTE  Class;				// ����
	BYTE  Level;				// ����
	int	  LevelUpPoint;			// ������ ����Ʈ 
	int   Exp;					// ����ġ
	int   NextExp;				// ���� ����ġ
	int   Money;				// ��
	short Str;					// ��
	short Dex;					// ��ø�� 
	short Vit;					// �ǰ�
	short Energy;				// ������
	short Life;					// ���� ����
	short MaxLife;				// ���� �ִ� ����
	short Mana;					// ���� ����
	short MaxMana;				// ���� �ִ� ������
	
	BYTE  dbInventory[MAX_DBINVENTORY];	// ������ �κ��丮
	BYTE  dbMagicList[MAX_DBMAGIC];		// ����(��ų)����Ʈ
	BYTE  dbWarehouse[MAX_WAREHOUSEDBSIZE];

	int   WarehouseMoney;

	BYTE  MapNumber;			// ������ ���� �������� �� ��ȣ
	BYTE  MapX;					// ������ ���� �������� X ��ǥ
	BYTE  MapY;					// ������ ���� �������� Y ��ǥ
	BYTE  Dir;
	int   PkCount;
	int   PkLevel;
	int   PkTime;
} PMSG_SETCHARINFO, *LPPMSG_SETCHARINFO;


// 0x06 ĳ������ ��ġ�� �����Ѵ�.
typedef struct
{
	PBMSG_HEAD		h;
	
	char			AccountID[MAX_IDSTRING];
	char			Name[MAX_IDSTRING];

	BYTE			ServerNumber;
	BYTE			MapNumber;			// ������ ���� �������� �� ��ȣ
	BYTE			MapX;				// ������ ���� �������� X ��ǥ
	BYTE			MapY;				// ������ ���� �������� Y ��ǥ
} PMSG_POS_CHANGE, *LPPMSG_POS_CHANGE;

// 0x22 ���� ������ �����Ѵ�.
typedef struct
{
	PBMSG_HEAD		h;
	char			AccountID[MAX_IDSTRING];
	char			Pwd[MAX_IDSTRING];

	char			Name[MAX_IDSTRING];
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
	char			JoominNumber[MAX_JOOMINNUMBER_LENGTH+1];
#else
	char			JoominNumber[14];
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
	char			PhoneNumber[16];
	char			Email[51];
	char			Question[51];
	char			Answer[51];
	
	int				Block;
} PMSG_ACCOUNTCHANGE, *LPPMSG_ACCOUNTCHANGE;
	
// 0x22 ���� ������ ���� ���
typedef struct
{
	PBMSG_HEAD		h;	
	BYTE			result;
	char			AccountID[MAX_IDSTRING];
} PMSG_ACCOUNTCHANGE_RESULT, *LPPMSG_ACCOUNTCHANGE_RESULT;


// 0x24 : ���� ĳ���� ���� ����
typedef struct
{
	PBMSG_HEAD		h;
	int				server_number;
	char			szId[10];
	char			szGameID1[10];
	char			szGameID2[10];
	char			szGameID3[10];
	char			szGameID4[10];
	char			szGameID5[10];
} PMSG_ACCOUNTCHARINFOCHANGE, *LPPMSG_ACCOUNTCHARINFOCHANGE;


// 0x24 : ���� ĳ���� ���� ���� ��� 
typedef struct
{
	PBMSG_HEAD		h;
	BYTE			result;
	char			szId[MAX_IDSTRING];
} PMSG_ACCOUNTCHARINFOCHANGE_RESULT, *LPPMSG_ACCOUNTCHARINFOCHANGE__RESULT;

// 0x25 : ���� �޸� ������Ʈ ��Ų��.
typedef struct
{
	PWMSG_HEAD		h;	
	char			szId[MAX_IDSTRING];
	char			Memo[MAX_MEMO];
} PMSG_ACCOUNTMEMO_UPDATE,  *LPPMSG_ACCOUNTMEMO_UPDATE;

typedef struct
{
	PBMSG_HEAD		h;
	BYTE			result;
	char			szId[MAX_IDSTRING];
} PMSG_ACCOUNTMEMO_ID_RESULT, *LPPMSG_ACCOUNTMEMO_ID_RESULT;

// 0x26 : ���� �޸� ���� �Ѵ�.
typedef struct
{
	PBMSG_HEAD		h;
	char			szId[MAX_IDSTRING];
} PMSG_ACCOUNTMEMO_DELETE,  *LPPMSG_ACCOUNTMEMO_DELETE;

// 0x27 ���� �׼��� �����Ѵ�.
typedef struct
{
	PBMSG_HEAD		h;
	char			GameID[MAX_IDSTRING];
	int				server_number;
	DWORD			Money;
} PMSG_MONEY_MODIFY, *LPPMSG_MONEY_MODIFY;


#endif
