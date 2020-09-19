#ifndef __NEWGUILDPROTOCOL_DEF__
#define __NEWGUILDPROTOCOL_DEF__

#include "..\\include\\define.h"
#include "..\\include\\prodef.h"

//----------------------------------------------------------------------------
// GX[0xB0] 길드 창고 아이템 리스트를 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	CHAR		szGuildName[MAX_GUILDNAMESTRING];		// 길드 이름
	BYTE		btWarehouseNum;							// 길드 창고 번호
	WORD		wIndex;									// 유저 인덱스
	DWORD		dwTimeStamp;							// 타임스템프 (최종 변경시간)
} GHP_REQ_GUILD_WAREHOUSE, *LPGHP_REQ_GUILD_WAREHOUSE;

//----------------------------------------------------------------------------
// XG[0xB0] 길드 창고 아이템 리스트를 보내준다.
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	CHAR		szGuildName[MAX_GUILDNAMESTRING];		// 길드 이름
	BYTE		btWarehouseNum;							// 길드 창고 번호
	WORD		wIndex;									// 유저 인덱스
	INT			iMoney;									// 길드 창고 젠
	BYTE		btDbItems[MAX_GUILDE_WAREHOUSEDBSIZE];	// 길드 창고 아이템 리스트
	BYTE		btDbVersion;							// 길드 창고 DB 버전
	SHORT		wPassword;								// 길드 창고 암호
	DWORD		dwTimeStamp;							// 타임스템프 (최종 변경시간)
} GHP_ANS_GUILD_WAREHOUSE, *LPGHP_ANS_GUILD_WAREHOUSE;



#endif