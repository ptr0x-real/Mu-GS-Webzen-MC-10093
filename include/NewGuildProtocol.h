#ifndef __NEWGUILDPROTOCOL_DEF__
#define __NEWGUILDPROTOCOL_DEF__

#include "..\\include\\define.h"
#include "..\\include\\prodef.h"

//----------------------------------------------------------------------------
// GX[0xB0] ��� â�� ������ ����Ʈ�� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	CHAR		szGuildName[MAX_GUILDNAMESTRING];		// ��� �̸�
	BYTE		btWarehouseNum;							// ��� â�� ��ȣ
	WORD		wIndex;									// ���� �ε���
	DWORD		dwTimeStamp;							// Ÿ�ӽ����� (���� ����ð�)
} GHP_REQ_GUILD_WAREHOUSE, *LPGHP_REQ_GUILD_WAREHOUSE;

//----------------------------------------------------------------------------
// XG[0xB0] ��� â�� ������ ����Ʈ�� �����ش�.
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	CHAR		szGuildName[MAX_GUILDNAMESTRING];		// ��� �̸�
	BYTE		btWarehouseNum;							// ��� â�� ��ȣ
	WORD		wIndex;									// ���� �ε���
	INT			iMoney;									// ��� â�� ��
	BYTE		btDbItems[MAX_GUILDE_WAREHOUSEDBSIZE];	// ��� â�� ������ ����Ʈ
	BYTE		btDbVersion;							// ��� â�� DB ����
	SHORT		wPassword;								// ��� â�� ��ȣ
	DWORD		dwTimeStamp;							// Ÿ�ӽ����� (���� ����ð�)
} GHP_ANS_GUILD_WAREHOUSE, *LPGHP_ANS_GUILD_WAREHOUSE;



#endif