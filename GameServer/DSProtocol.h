#ifndef __DSPROTOCOL_H__
#define __DSPROTOCOL_H__

#include "user.h"

extern void DataServerProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen);

extern void DataServerLogin(int server);
extern void DataServerGetCharListRequest(short aIndex);
extern void GDGameServerInfoSave();

#ifdef MODIFY_MAPSERVERMOVE_FLAG_BUGFIX_20060817
	extern void GJSetCharacterInfo(LPOBJECTSTRUCT lpObj, int aIndex, BOOL bMapServerMove = FALSE);
#else
	extern void GJSetCharacterInfo(LPOBJECTSTRUCT lpObj, int aIndex);
#endif

extern void GDUserItemSave(LPOBJECTSTRUCT lpObj);
extern void JGCharacterCreateFailSend(int aIndex, char *id);


extern void ItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int lootindex=-1, BYTE NewOption=0, BYTE SetOption = 0, BOOL SpecialShop = FALSE);

#ifdef DARKLORD_WORK	// 0x55	PetItem ����
void PetItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
#endif	// DARKLORD_WORK

extern void ItemMovePathSave(char* ActID, char* Name, BYTE level, BYTE mapnumber, 
						BYTE x, BYTE y, char* Item, BYTE op1, BYTE op2, BYTE op3, 
						DWORD serial);


// â��
extern void GDGetWarehouseList(int aIndex, char *AccountID);
extern void GDSetWarehouseList(int aIndex);
extern void GDSetWarehouseMoney(int aIndex);


#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
extern void DGOptionDataSend(int aIndex, char * szName, LPBYTE KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk, int QWERLevel);
#else	// UPDATE_OPTIONKEY_LEVEL_20080116
#ifdef VER_CHATWINDOW_OPTION
#ifdef ADD_SEASON_3_NEW_UI_20071122
extern void DGOptionDataSend(int aIndex, char * szName, LPBYTE KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk);
#else
extern void DGOptionDataSend(int aIndex, char * szName, LPBYTE KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd);
#endif
#else
extern void DGOptionDataSend(int aIndex, char * szName, LPBYTE KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek);
#endif
#endif	// UPDATE_OPTIONKEY_LEVEL_20080116

#ifdef DARKLORD_WORK	
extern void gObjRequestPetItemInfo(int aIndex, int inventype);
extern void gObjSavePetItemInfo(int aIndex, int inventype);
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
extern void	GDRequestBlockHackUser(int aIndex, char* AccountID, char* Name);
#endif

#ifdef ADD_CHARCARD_SUMMONER_20080313
extern void DataServerSetAccountInfo(LPOBJECTSTRUCT lpObj, int aIndex);
#endif // ADD_CHARCARD_SUMMONER_20080313

#ifdef MU_CASTLESIEGE_DS_PROTOCOL_20041105		// ������ ���� GS <-> DS �������� ����

// < GS -> DS >
// [0x80][0x00] ���� ���� ��ü���� ���� ��û
extern void GS_GDReqCastleTotalInfo			(int iMapSvrGroup, int iCastleEventCycle);
// [0x80][0x01] ������ ����� �渶�̸� ��û -> ��� NPC �� ���� ���
extern void GS_GDReqOwnerGuildMaster		(int iMapSvrGroup, int aIndex);
// [0x80][0x03] ������ NPC ���� ��û
extern void GS_GDReqCastleNpcBuy			(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcDfLevel, int iNpcRgLevel, int iNpcMaxHP, int iNpcHP, BYTE btNpcX, BYTE btNpcY, BYTE btNpcDIR, int iBuyCost);
// [0x80][0x04] ������ NPC ���� ��û
extern void GS_GDReqCastleNpcRepair			(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iRepairCost);
// [0x80][0x05] ������ NPC ���׷��̵� ��û
extern void GS_GDReqCastleNpcUpgrade		(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex);
// [0x80][0x06] ������ �������� ��û (���� ���� �Բ� ��������)
extern void GS_GDReqTaxInfo					(int iMapSvrGroup, int aIndex);
// [0x80][0x07] ������ ���� ���� ��û
extern void GS_GDReqTaxRateChange			(int iMapSvrGroup, int aIndex, int iTaxType, int iTaxRate);
// [0x80][0x08] ������ ���� �� ���� ��û
extern void GS_GDReqCastleMoneyChange		(int iMapSvrGroup, int aIndex, int iMoneyChange);
// [0x80][0x09] ������ ���� ����/���� �ð� ���� ��û
extern void GS_GDReqSiegeDateChange			(int iMapSvrGroup, int aIndex, WORD wStartYear, BYTE btStartMonth, BYTE btStartDay, WORD wEndYear, BYTE btEndMonth, BYTE btEndDay);
// [0x80][0x0A] ������ Ư�� ����� ���� ��û
extern void GS_GDReqGuildMarkRegInfo		(int iMapSvrGroup, int aIndex);
// [0x80][0x0B] ���� ���� ���Ῡ�� ���� ��û
extern void GS_GDReqSiegeEndedChange		(int iMapSvrGroup, BOOL bIsSiegeEnded);
// [0x80][0x0C] ���� �� �������� ���� ��û
extern void GS_GDReqCastleOwnerChange		(int iMapSvrGroup, BOOL bIsCastleOccupied, LPSTR lpszGuildName);
// [0x80][0x0D] ������ ����� ������ ��� ��û
extern void GS_GDReqRegAttackGuild			(int iMapSvrGroup, INT aIndex);
// [0x80][0x0E] ������ ���� �ֱⰡ �������Ƿ� ���� ���� ���� ������ ���� ��û
extern void GS_GDReqRestartCastleState		(int iMapSvrGroup);
// [0x80][0x0F] Ư�� �ʼ������� �޽����� ��Ƽ ĳ���� ��û
extern void GS_GDReqMapSvrMsgMultiCast		(int iMapSvrGroup, LPSTR lpszMsgText);
// [0x80][0x10] Ư�� ����� ���� ����� ��û
extern void GS_GDReqRegGuildMark			(int iMapSvrGroup, int aIndex, int iItemPos);
// [0x80][0x11] ������ Ư�� ��� ��ũ ���� �ʱ�ȭ ��û
extern void GS_GDReqGuildMarkReset			(int iMapSvrGroup, int aIndex, LPSTR lpszGuildName);
// [0x80][0x12] ������ Ư�� ��� ���� ���⿩�� ���� ��û
extern void GS_GDReqGuildSetGiveUp			(int iMapSvrGroup, int aIndex, BOOL bIsGiveUp);
// [0x80][0x16] ������ NPC ���� ��û
extern void GS_GDReqNpcRemove				(int iMapSvrGroup, int iNpcNumber, int iNpcIndex);
// [0x80][0x17] ������ �������� ����ȭ ��û
extern void GS_GDReqCastleStateSync			(int iMapSvrGroup, int iCastleState, int iTaxRateChaos, int iTaxRateStore, INT iTaxHuntZone, LPSTR lpszOwnerGuild);
// [0x80][0x18] �ʼ������� �� ������ �߰� ��û
extern void GS_GDReqCastleTributeMoney		(int iMapSvrGroup, int iCastleTributeMoney);
// [0x80][0x19] �ʼ������� �� ����, �� �ʱ�ȭ ��û
extern void GS_GDReqResetCastleTaxInfo		(int iMapSvrGroup);
// [0x80][0x1A] ���� ������� ����Ʈ �ʱ�ȭ ��û
extern void GS_GDReqResetSiegeGuildInfo		(int iMapSvrGroup);
// GD [0x80][0x1B] ���� ������û ��� ����Ʈ �ʱ�ȭ ��û (0xC1)
extern void GS_GDReqResetRegSiegeInfo		(int iMapSvrGroup);
// [0x81] ������ ���� ��� �ʱ�ȭ ������ ��û
extern void GS_GDReqCastleInitData			(int iMapSvrGroup, int iCastleEventCycle);
// [0x82] ������ NPC ���� ��û
extern void GS_GDReqCastleNpcInfo			(int iMapSvrGroup, int aIndex);
// [0x83] ������ ����� ���� ��û (��� ���� N�� ����)
extern void GS_GDReqAllGuildMarkRegInfo		(int iMapSvrGroup, int aIndex);
// [0x85] ������ ��弱�� �ĺ��ڷ� ��û
extern void GS_GDReqCalcRegGuildList		(int iMapSvrGroup);
// [0x88] ��/�� ����� ���������� �ҷ����� ��û
extern void GS_GDReqCsLoadTotalGuildInfo	(int iMapSvrGroup);


// < DS -> GS >
// [0x80][0x00] ���� ���� ��ü���� ���� ��û ����
extern void GS_DGAnsCastleTotalInfo			(LPBYTE lpRecv);
// [0x80][0x01] ������ ����� �渶�̸� ��û ���� -> ��� NPC �� ���� ���
extern void GS_DGAnsOwnerGuildMaster		(LPBYTE lpRecv);
// [0x80][0x03] ������ NPC ���� ��û ����
extern void GS_DGAnsCastleNpcBuy			(LPBYTE lpRecv);
// [0x80][0x04] ������ NPC ���� ��û ����
extern void GS_DGAnsCastleNpcRepair			(LPBYTE lpRecv);
// [0x80][0x05] ������ NPC ���׷��̵� ��û ����
extern void GS_DGAnsCastleNpcUpgrade		(LPBYTE lpRecv);
// [0x80][0x06] ������ �������� ��û ���� (���� ���� �Բ� ��������)
extern void GS_DGAnsTaxInfo					(LPBYTE lpRecv);
// [0x80][0x07] ������ ���� ���� ��û ����
extern void GS_DGAnsTaxRateChange			(LPBYTE lpRecv);
// [0x80][0x08] ������ ���� �� ���� ��û ����
extern void GS_DGAnsCastleMoneyChange		(LPBYTE lpRecv);
// [0x80][0x09] ������ ���� ����/���� �ð� ���� ��û ����
extern void GS_DGAnsSiegeDateChange			(LPBYTE lpRecv);
// [0x80][0x0A] ������ Ư�� ����� ���� ��û ����
extern void GS_DGAnsGuildMarkRegInfo		(LPBYTE lpRecv);
// [0x80][0x0B] ���� ���� ���Ῡ�� ���� ��û ����
extern void GS_DGAnsSiegeEndedChange		(LPBYTE lpRecv);
// [0x80][0x0C] ���� �� �������� ���� ��û ����
extern void GS_DGAnsCastleOwnerChange		(LPBYTE lpRecv);
// [0x80][0x0D] ������ ����� ������ ��� ��û ����
extern void GS_DGAnsRegAttackGuild			(LPBYTE lpRecv);
// [0x80][0x0E] ������ ���� �ֱⰡ �������Ƿ� ���� ���� ���� ������ ���� ��û ����
extern void GS_DGAnsRestartCastleState		(LPBYTE lpRecv);
// [0x80][0x0F] Ư�� �ʼ������� �޽����� ��Ƽ ĳ���� ��û ����
extern void GS_DGAnsMapSvrMsgMultiCast		(LPBYTE lpRecv);
// [0x80][0x10] Ư�� ����� ���� ����� ��û ����
extern void GS_DGAnsRegGuildMark			(LPBYTE lpRecv);
// [0x80][0x11] ������ Ư�� ��� ��ũ ���� �ʱ�ȭ ��û ����
extern void GS_DGAnsGuildMarkReset			(LPBYTE lpRecv);
// [0x80][0x12] ������ Ư�� ��� ���� ���⿩�� ���� ��û ����
extern void GS_DGAnsGuildSetGiveUp			(LPBYTE lpRecv);
// [0x80][0x16] ������ NPC ���� ��û ����
extern void GS_DGAnsNpcRemove				(LPBYTE lpRecv);
// [0x80][0x17] ������ �������� ����ȭ ��û ����
extern void GS_DGAnsCastleStateSync			(LPBYTE lpRecv);
// [0x80][0x18] �ʼ������� �� ������ �߰� ��û
extern void GS_DGAnsCastleTributeMoney		(LPBYTE lpRecv);
// [0x80][0x19] �ʼ������� �� ����, �� �ʱ�ȭ ��û ����
extern void GS_DGAnsResetCastleTaxInfo		(LPBYTE lpRecv);
// [0x80][0x1A] ���� ������� ����Ʈ �ʱ�ȭ ��û ����
extern void GS_DGAnsResetSiegeGuildInfo		(LPBYTE lpRecv);
// [0x80][0x1B] ���� ������û ��� ����Ʈ �ʱ�ȭ ��û ����
extern void GS_DGAnsResetRegSiegeInfo		(LPBYTE lpRecv);
// [0x81] ������ ���� ��� �ʱ�ȭ ������ ��û ����
extern void GS_DGAnsCastleInitData			(LPBYTE lpRecv);
// [0x82] ������ NPC ���� ��û ����
extern void GS_DGAnsCastleNpcInfo			(LPBYTE lpRecv);
// [0x83] ������ ����� ���� ��û ���� (��� ���� N�� ����)
extern void GS_DGAnsAllGuildMarkRegInfo		(LPBYTE lpRecv);
// [0x84] ������ NPC ����Ʈ ��� ���� ��û ����
extern void GS_DGAnsFirstCreateNPC			(LPBYTE lpRecv);
// [0x85] ������ ��弱�� �ĺ��ڷ� ��û ����
extern void GS_DGAnsCalcREgGuildList		(LPBYTE lpRecv);
// [0x86] ��/�� ����� ���������� ��û ����
extern void GS_DGAnsCsGulidUnionInfo		(LPBYTE lpRecv);
// [0x87] ��/�� ����� ���������� ���� ��û ����
extern void GS_DGAnsCsSaveTotalGuildInfo	(LPBYTE lpRecv);
// [0x88] ��/�� ����� ���������� �ҷ����� ��û ����
extern void GS_DGAnsCsLoadTotalGuildInfo	(LPBYTE lpRecv);
// [0x89] ������ NPC ����Ʈ �������� ��û ����
extern void GS_DGAnsCastleNpcUpdate			(LPBYTE lpRecv);

#endif	// MU_CASTLESIEGE_DS_PROTOCOL_20041105


#ifdef MU_CRYWOLF_DS_PROTOCOL_20050505	// ũ���̿��� ���� GS <-> DS �������� ����

// < GS -> DS >
extern void GDReqCrywolfSync				( int iMapSvrGroup, int iCrywolfState, int iOccupationState );
extern void GDReqCrywolfInfoLoad			( int iMapSvrGroup );
extern void GDReqCrywolfInfoSave			( int iMapSvrGroup, int iCrywolfState, int iOccupationState );

// < DS -> GS >
extern void DGAnsCrywolfSync				( LPBYTE lpRecv );
extern void DGAnsCrywolfInfoLoad			( LPBYTE lpRecv );
extern void DGAnsCrywolfInfoSave			( LPBYTE lpRecv );

#endif // MU_CRYWOLF_DS_PROTOCOL_20050505

#endif

