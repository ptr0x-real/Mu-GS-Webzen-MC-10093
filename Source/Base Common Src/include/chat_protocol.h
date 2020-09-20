#ifndef __CHATPROTOCOL_H__
#define __CHATPROTOCOL_H__

#include "Prodef.h"

// CS : 0x00 : 방 접속 
typedef struct
{
	PBMSG_HEAD	h;
	WORD		RoomNumber;
	//DWORD		Ticket;
	char		szTicket[10];
} PMSG_ROOMLOGIN,*LPPMSG_ROOMLOGIN;

// CS : 0x00 : 방 접속 
typedef struct
{
	PBMSG_HEAD	h;
	WORD		RoomNumber;
	DWORD		Ticket;
} PMSG_ROOMLOGIN2,*LPPMSG_ROOMLOGIN2;


// SC : 0x00 : 방 접속 결과
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} PMSG_ROOMLOGIN_RESULT, *LPPMSG_ROOMLOGIN_RESULT;

// CS : 0x01 : 방에서 빠져 나가기 요청
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_ROOMLOGOUT, *LPPMSG_ROOMLOGOUT;

// SC : 0x01 : 다른 사용자의 상태
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Type;	// 0x00 : 접속, 0x01 : 종료
	BYTE		Index;	// 사용자인덱스
	char		Name[MAX_IDSTRING];
} PMSG_ROOMLOGOUT_USER, *LPPMSG_ROOMLOGOUT_USER;

// 0x02 : 유저 리스트를 요청한다.
typedef struct
{
	PBMSG_HEAD	h;
} PMSG_ROOMUSERLIST_REQ, *LPPMSG_ROOMUSERLIST_REQ;

// 0x02 : 유저 리스트 카우트
typedef struct
{
	PWMSG_HEAD	h;
	WORD		RoomNumber;
	BYTE		Count;
} PMSG_ROOMUSERLIST_COUNT, *LPPMSG_ROOMUSERLIST_COUNT;

// 유저 리스트 (이름)
typedef struct
{
	BYTE		Index;				// 유저 인덱스
	char		Name[MAX_IDSTRING];
} PMSG_ROOMUSERLIST, *LPPMSG_ROOMUSERLIST;

// 0x03 : 초대 (친구를 초대한다)
typedef struct
{
	PBMSG_HEAD	h;
	char		Name[MAX_IDSTRING];
} PMSG_ROOM_INVITATION, *LPPMSG_ROOM_INVITATION;

// 0x03 : 초대 결과 
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} PMSG_ROOM_INVITATION_RESULT, *LPPMSG_ROOM_INVITATION_RESULT;

// 0x04 : 채팅 메시지
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Index;		// 유저 인덱스
	BYTE		size;
	char		Msg[100];
} PMSG_ROOM_CHATMSG, *LPPMSG_ROOM_CHATMSG;



#endif
