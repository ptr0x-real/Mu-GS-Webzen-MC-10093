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

#ifdef MAP_SERVER_WORK_20041030		// ���� �ʺ��� ���� ��Ŷ - ���Ӽ��� <-> ��������

//----------------------------------------------------------------------------
// GJ [0x7A] ������������ Ư�� ����ڰ� �ʼ����� �̵��ؾ� ���� �˸� (0xC1)
//----------------------------------------------------------------------------
extern void GJReqMapSvrMove			(INT iIndex, WORD wDestMapSvrCode, WORD wMapNumber, BYTE btX, BYTE btY);

//----------------------------------------------------------------------------
// GJ [0x7B] ������������ Ư�� ����ڰ� �ʼ��� �̵� �� ������ ��û���� �˸� (0xC1)		-> ���������� �ش� ������� ���Ӽ��� �ڵ嵵 ����
//----------------------------------------------------------------------------
extern void GJReqMapSvrAuth			(INT iIndex, LPSTR lpszAccountID, LPSTR lpszCharName, INT iJA1, INT iJA2, INT iJA3, INT iJA4);

//----------------------------------------------------------------------------
// JG [0x7C] Ư�� ���Ӽ����� ���� �ִ� ����� ���� �ֱ������� ���� (0xC1) 
//----------------------------------------------------------------------------
extern void GJNotifyMaxUserCount	();

#endif


#endif

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
// ���̷κ� ��ŷ�� ������ ���� ������������ ���� �۾�
extern void GJRequestAntiHackStatistics(char* pchFileName);
extern void JGRequestAntiHackStatistics(LPPMSG_HACKTOOL_USE_STATISTICS lpMsg);
extern void GJRequestHacktoolMapMoveBlock(BYTE btModifyType, char* pchFilename, DWORD dwReservationTime, DWORD dwBlockTerm, WORD wBlockRate);
extern void JGRequestHacktoolMapMoveBlock(LPPMSG_RESERVE_HACKTOOL_MAP_MOVE lpMsg);
extern void GJAntiHackStatisticsResult(char* pchFilename, int iHacktoolUserCount);
#endif // ADD_HACKING_TOOL_BLOCK_20090311