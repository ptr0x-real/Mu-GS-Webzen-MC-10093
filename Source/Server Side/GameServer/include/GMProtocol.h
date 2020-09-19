#ifndef __GMPROTOCOL_H__
#define __GMPROTOCOL_H__
// 게임서버와 몬스터서버에서 사용되는 프로토콜

#include "../include/ProDef.h"

// [0x00] 대화프로토콜
typedef struct
{
	PBMSG_HEAD	h;
	char chatid[10];
	char chatmsg[MAX_CHAT];
} PMSG_CHATDATA, *LPPMSG_CHATDATA;


// 대화프로토콜
typedef struct
{
	PBMSG_HEAD	h;
	
	BYTE NumberH;
	BYTE NumberL;
	char chatmsg[MAX_CHAT];
} PMSG_CHATDATA_NUMBER, *LPPMSG_CHATDATA_NUMBER;


// [0x02] 대화프로토콜
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
// [0xF1][0x30] 몬스터 접속을 요청하여 게임서버의 공간을 할당받는다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD  h;	
	BYTE		subcode;

	BYTE	Index;		// 인덱스 번호 
	BYTE	Level;		// 레벨 
	BYTE	MapNumber;	// 접속하고자 하는 맵번호
	BYTE	MapX;		// 맵 X
	BYTE	MapY;		// 맵 Y
	BYTE	Dir;		// 보고있는 방향..
} PMSG_MONSTERJOIN, * LPPMSG_MONSTERJOIN;

//----------------------------------------------------------------------------
// [0xF1][0x30] 몬스터 접속을 요청결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD  h;	
	BYTE	subcode;
	BYTE	Result;	// 결과
	BYTE	NumberH;
	BYTE	NumberL;
} PMSG_MONSTERJOINRESULT,* LPPMSG_MONSTERJOINRESULT;



#endif