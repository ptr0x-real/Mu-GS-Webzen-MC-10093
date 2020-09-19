#ifndef __CHATPROTOCOL_H__
#define __CHATPROTOCOL_H__

#include "Prodef.h"

// CS : 0x00 : �� ���� 
typedef struct
{
	PBMSG_HEAD	h;
	WORD		RoomNumber;
	//DWORD		Ticket;
	char		szTicket[10];
} PMSG_ROOMLOGIN,*LPPMSG_ROOMLOGIN;

// CS : 0x00 : �� ���� 
typedef struct
{
	PBMSG_HEAD	h;
	WORD		RoomNumber;
	DWORD		Ticket;
} PMSG_ROOMLOGIN2,*LPPMSG_ROOMLOGIN2;


// SC : 0x00 : �� ���� ���
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} PMSG_ROOMLOGIN_RESULT, *LPPMSG_ROOMLOGIN_RESULT;

// CS : 0x01 : �濡�� ���� ������ ��û
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_ROOMLOGOUT, *LPPMSG_ROOMLOGOUT;

// SC : 0x01 : �ٸ� ������� ����
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Type;	// 0x00 : ����, 0x01 : ����
	BYTE		Index;	// ������ε���
	char		Name[MAX_IDSTRING];
} PMSG_ROOMLOGOUT_USER, *LPPMSG_ROOMLOGOUT_USER;

// 0x02 : ���� ����Ʈ�� ��û�Ѵ�.
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_ROOMUSERLIST_REQ, *LPPMSG_ROOMUSERLIST_REQ;

// 0x02 : ���� ����Ʈ ī��Ʈ
typedef struct
{
	PWMSG_HEAD	h;
	WORD		RoomNumber;
	BYTE		Count;
} PMSG_ROOMUSERLIST_COUNT, *LPPMSG_ROOMUSERLIST_COUNT;

// ���� ����Ʈ (�̸�)
typedef struct
{
	BYTE		Index;				// ���� �ε���
	char		Name[MAX_IDSTRING];
} PMSG_ROOMUSERLIST, *LPPMSG_ROOMUSERLIST;

// 0x03 : �ʴ� (ģ���� �ʴ��Ѵ�)
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
} PMSG_ROOM_INVITATION, *LPPMSG_ROOM_INVITATION;

// 0x03 : �ʴ� ��� 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} PMSG_ROOM_INVITATION_RESULT, *LPPMSG_ROOM_INVITATION_RESULT;

// 0x04 : ä�� �޽���
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Index;		// ���� �ε���
	BYTE		size;
	char		Msg[100];
} PMSG_ROOM_CHATMSG, *LPPMSG_ROOM_CHATMSG;



#endif
