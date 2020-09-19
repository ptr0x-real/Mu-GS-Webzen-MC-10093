#ifndef __SPROTOCOL_H__
#define __SPROTOCOL_H__


#include "User.h"

extern void GJServerLogin();
extern void SProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen);

extern void JGPAccountRequest(LPBYTE lpData, short aIndex);
extern void GJPGetCharListRequest(short aIndex);
#if defined(CHN_PLAYTIME_LIMIT_SYSTEM_20050818) || defined(VTM_PLAYTIME_LIMIT_SYSTEM_20060626)
extern void GJPUserClose(char * szAccountID, INT iPlayTimeLimit);
extern void GJPUserClose2(char * szAccountID);
#else
extern void GJPUserClose(char * szAccountID);
#endif

#ifndef MODIFY_MAPSERVERMOVE_FLAG_BUGFIX_20060817	// not define
	extern void GJSetCharacterInfo(LPOBJECTSTRUCT lpObj, int aIndex);
#endif

extern void GCItemListSend(int aIndex);
extern void GJPAccountBlock(short number, char * Id, int DBNumber, int UserNumber, BYTE BlockCode );
extern void LoveHeartEventSend(int aIndex, char * AcountId, char* Name );
extern void LoveHeartCreateSend();

extern void GJPUserClose(LPOBJECTSTRUCT lpObj);

#ifdef MODIFY_UPDATE_MATCHDB_AT_CHARACTERLIST_VIEW_20060725
extern void GJUpdateMatchDBUserCharacters(LPOBJECTSTRUCT lpObj);
#endif

#ifdef MAP_SERVER_WORK_20041030		// 서버 맵분할 관련 패킷 - 게임서버 <-> 인증서버

//----------------------------------------------------------------------------
// GJ [0x7A] 인증서버에게 특정 사용자가 맵서버를 이동해야 함을 알림 (0xC1)
//----------------------------------------------------------------------------
extern void GJReqMapSvrMove			(INT iIndex, WORD wDestMapSvrCode, WORD wMapNumber, BYTE btX, BYTE btY);

//----------------------------------------------------------------------------
// GJ [0x7B] 인증서버에게 특정 사용자가 맵서버 이동 후 인증을 요청함을 알림 (0xC1)		-> 인증서버는 해당 사용자의 게임서버 코드도 갱신
//----------------------------------------------------------------------------
extern void GJReqMapSvrAuth			(INT iIndex, LPSTR lpszAccountID, LPSTR lpszCharName, INT iJA1, INT iJA2, INT iJA3, INT iJA4);

//----------------------------------------------------------------------------
// JG [0x7C] 특정 게임서버의 현재 최대 사용자 수를 주기적으로 보냄 (0xC1) 
//----------------------------------------------------------------------------
extern void GJNotifyMaxUserCount	();

#endif


#endif

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
// 아이로봇 해킹툴 차단을 위한 인증서버와의 연동 작업
extern void GJRequestAntiHackStatistics(char* pchFileName);
extern void JGRequestAntiHackStatistics(LPPMSG_HACKTOOL_USE_STATISTICS lpMsg);
extern void GJRequestHacktoolMapMoveBlock(BYTE btModifyType, char* pchFilename, DWORD dwReservationTime, DWORD dwBlockTerm, WORD wBlockRate);
extern void JGRequestHacktoolMapMoveBlock(LPPMSG_RESERVE_HACKTOOL_MAP_MOVE lpMsg);
extern void GJAntiHackStatisticsResult(char* pchFilename, int iHacktoolUserCount);
#endif // ADD_HACKING_TOOL_BLOCK_20090311