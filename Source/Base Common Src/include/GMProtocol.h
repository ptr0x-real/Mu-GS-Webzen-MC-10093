#ifndef __GMPROTOCOL_H__
#define __GMPROTOCOL_H__
// ���Ӽ����� ���ͼ������� ���Ǵ� ��������

#include "../include/ProDef.h"

// [0x00] ��ȭ��������
typedef struct
{
	PBMSG_HEAD	h;
	char chatid[10];
	char chatmsg[MAX_CHAT];
} PMSG_CHATDATA, *LPPMSG_CHATDATA;


// ��ȭ��������
typedef struct
{
	PBMSG_HEAD	h;
	
	BYTE NumberH;
	BYTE NumberL;
	char chatmsg[MAX_CHAT];
} PMSG_CHATDATA_NUMBER, *LPPMSG_CHATDATA_NUMBER;


// [0x02] ��ȭ��������
typedef struct
{
	PBMSG_HEAD	h;
	char id[10];
	char chatmsg[MAX_CHAT];
} PMSG_CHATDATA_WHISPER, *LPPMSG_CHATDATA_WHISPER;



//----------------------------------------------------------------------------
// [0xF1][0x00] Join Result Protocol
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		scode;
	BYTE		result;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		CliVersion[5];
} PMSG_JOINRESULT, *LPPMSG_JOINRESULT;

//----------------------------------------------------------------------------
// [0xF1][0x30] ���� ������ ��û�Ͽ� ���Ӽ����� ������ �Ҵ�޴´�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD  h;	
	BYTE		subcode;

	BYTE	Index;		// �ε��� ��ȣ 
	BYTE	Level;		// ���� 
	BYTE	MapNumber;	// �����ϰ��� �ϴ� �ʹ�ȣ
	BYTE	MapX;		// �� X
	BYTE	MapY;		// �� Y
	BYTE	Dir;		// �����ִ� ����..
} PMSG_MONSTERJOIN, * LPPMSG_MONSTERJOIN;

//----------------------------------------------------------------------------
// [0xF1][0x30] ���� ������ ��û���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD  h;	
	BYTE	subcode;
	BYTE	Result;	// ���
	BYTE	NumberH;
	BYTE	NumberL;
} PMSG_MONSTERJOINRESULT,* LPPMSG_MONSTERJOINRESULT;



#endif