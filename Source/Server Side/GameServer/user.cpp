//////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <winsock2.h>
#include <windef.h>
#include <crtdbg.h> 
#include <TIME.H>
#include "Include\Public.h"
#include "Include\ProDef.h"
#include "Include\SproDef.h"
#include "Include\GMProtocol.h"
#include "Common\Winutil.h"
#include "Common\zzzItem.h"
#include "Common\Classdef.h"
#include "Include\ReadScript.h"
#include "Gameserver.h"
#include "SProtocol.h"
#include "Protocol.h"
#include "GameMain.h"
#include "zzzmathlib.h"
#include "MonsterSetBase.h"
#include "wsGameServer.h"
#include "MonsterAttr.h"
#include "gObjMonster.h"
#include "MagicInf.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "User.h"
#include "MagicDamage.h"
#include "Gate.h"
#include "PartyClass.h"
#include "AcceptIp.h"
#include "IpCache.h"
#include "ViewportGuild.h"
#include "DragonEvent.h"
#include "GMMng.h"
#include "BattleGround.h"
#include "BattleSoccer.h"
#include "ConnectBonus.h"

#ifdef MODIFY_MIX_SYSTEM_20070518
//#include "ChaosBox.h"
#include "MixSystem.h"
#endif

#include "NSerialCheck.h"

#include "DevilSquare.h"

#include "BattleSoccerManager.h"

#include "MoveCommand.h"
#include "OffExp.h"

#ifdef MODIFY_WHISPER_CASH_BUGFIX_20060704
	#include "WhisperCash.h"
	extern CWhisperCash	WhisperCash;
#endif

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

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
#include "Crywolf.h"
#endif

#ifdef MU_CRYWOLF_SERVER_SYNC_20050505
#include "CrywolfSync.h"
#endif

#ifdef ADD_KANTURU_20060627
#include "Kanturu.h"
#endif
	
#ifdef ADD_RAKLION_20080408
#include "Raklion.h"
#include "RaklionUtil.h"
	
	static CRaklionUtil g_RaklionUtil;
#endif // ADD_RAKLION_20080408
	
#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704
#include "QuestUtil.h"
#endif

#include "MonsterItemMng.h"
#include "MultiAttackHackCheck.h"

#include "QuestInfo.h"
#include "StatMng.h"

#include "ObjUseSkill.h"
#include "ObjAttack.h"
#include "PacketCheckSum.h"
#include "ObjCalCharacter.h"

#include "EledoradoEvent.h"

#include "SendHackLog.h"
#include "NpcTalk.h"

#ifdef MONSTER_HERD_SYSTEM_20031120
#include "MonsterHerd.h"
#endif

#ifdef AUTH_GAMESERVER
#include "GameServerAuth.h"
#endif

#include "Common\SetItemOption.h"

#ifdef CHANGE_CHAOSCASTLE_SET_ITEM_DROP
#include "ChaosCastleSetItemDrop.h"
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

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#include "BuffEffectSlot.h"
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#include "BuffEffectSlot.h"
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"
#endif // ADD_NEWPVP_PKFIELD
	
#ifdef ADD_HACKING_TOOL_BLOCK_20090311
#include "HackToolBlock.h"
#endif // ADD_HACKING_TOOL_BLOCK_20090311

extern CwsGameServer	wsGServer;

extern CMonsterAttr		gMAttr;
extern CMonsterSetBase	gMSetBase;
extern classdef			DCInfo;
extern PartyClass		gParty;
extern int				gCurPaintPlayer;
extern CGuildClass		Guild;
extern CDragonEvent*	DragonEvent;
extern CBattleSoccer*	gBSGround[];
extern NSerialCheck	gNSerialCheck[];

#ifdef ADD_SHIELD_POINT_01_20060403
extern BOOL g_PKLevelIncreaseOff;
#endif

CViewportGuild	ViewGuildMng;

#ifdef __FRIEND_WORK__
	extern void FriendListRequest(int);
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518
#include "KalimaGate.h"
#endif

#ifdef CASTLE_NPC_CROWN_WORK_20041210
#include "CastleCrown.h"
#endif
#ifdef CASTLE_NPC_CROWN_SWITCH_WORK_20041210
#include "CastleCrownSwitch.h"
#endif
#ifdef CASTLE_NPC_GUARDIANSTATUE_WORK_20041207
#include "GuardianStatue.h"
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
#ifdef CASTLE_NPC_CANNON_TOWER_20041223
#include "CannonTower.h"
#endif
#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#include "Weapon.h"
#endif

#ifdef MAP_SERVER_WORK_20041030
#include "MapServerManager.h"
#include "Sprotocol.h"
#endif

#ifdef CASTLEDEEP_EVENT_20050413
#include "CastleDeepEvent.h"
#endif
	
#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
#include "TMonsterSkillManager.h"
#endif
	
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
#include "TMonsterAI.h"
#endif

#ifdef ADD_SKILL_ADDITION_INFO_CLASS_20060407
#include "SkillAdditionInfo.h"
#endif

#ifdef ITEM_ADD_OPTION_20061019
#include "ItemAddOption.h"
#endif // ITEM_ADD_OPTION_20061019

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "ILLUSIONTempleEvent.h"
#endif

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#include "CashShop.h"
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PREMIUMITEM_20090106
#include "CashShop.h"
#endif // PREMIUMITEM_20090106

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#include "CashItemPeriodSystem.h"
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif	

extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];

#ifdef ADD_SHIELD_POINT_01_20060403
extern int	g_ShieldSystemOn;
extern BOOL g_ShieldAutoRefillOn;
extern BOOL g_ShieldAutoRefillOnSafeZone;
#endif

#ifdef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018
	#ifdef ADD_SHIELD_POINT_01_20060403
		void	gObjCalcShieldPoint(LPOBJECTSTRUCT lpObj);
	#endif
#endif


#ifdef MEM2_KUZIMO
	void *ZeroMemBase::operator new[] (size_t stAllocateBlock)
	{
		void *pvTemp = malloc( stAllocateBlock );

		if(pvTemp != NULL)
			memset(pvTemp, 0x00, stAllocateBlock);

		return(pvTemp);
	}

	void ZeroMemBase::operator delete[] (void *pMalloc)
	{
		free(pMalloc);
	}
#endif


//////////////////////////////////////////////////////////////////////////////
#ifdef MEM_KUZIMO

/* 
OBJECTSTRUCT *gObj = NULL;

class StaticObject {
public:
	StaticObject(void);
	virtual ~StaticObject();
};

StaticObject::StaticObject(void)
{
	gObj = new OBJECTSTRUCT[MAX_OBJECT];
#ifndef MEM2_KUZIMO
	memset(gObj, 0x00, sizeof(OBJECTSTRUCT) * MAX_OBJECT);
#endif
}

StaticObject::~StaticObject()
{
	delete []gObj;
	gObj = NULL;
}

StaticObject g_kStaticObject;
*/

// by kuzimo
// temporary code, ���׸� ����. �������� -1�� �迭�� �����ϴ� ���װ� �ִ�. �� ���� ���ؼ� �׷��� �ٿ�ȴ�.
OBJECTSTRUCT *gAllocObj = NULL;
OBJECTSTRUCT *gObj = NULL;

class StaticObject {
public:
	StaticObject(void);
	virtual ~StaticObject();
};

StaticObject::StaticObject(void)
{
	gAllocObj = new OBJECTSTRUCT[MAX_OBJECT + 1];
#ifndef MEM2_KUZIMO
	memset(gAllocObj, 0x00, sizeof(OBJECTSTRUCT) * (MAX_OBJECT + 1));
#endif

	gObj = &gAllocObj[1];
}

StaticObject::~StaticObject()
{
	delete []gAllocObj;
	gAllocObj = NULL;
}

StaticObject g_kStaticObject;

#else
OBJECTSTRUCT gObj[MAX_OBJECT];
#endif

int				gObjCount;
int				gObjMonCount;
int				gObjCallMonCount;

MessageStateMachine	gSMMsg[MAX_OBJECT][MAXMESSAGE];	// �޽���

#ifdef ADD_SKILL_WITH_COMBO
ExMessageStateMachine	gSMAttackProcMsg[MAX_USER][MAXATTACKMESSAGE];	// �޽���
void gObjStateAttackProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
#elif defined(NEW_ATTACK_PROC)
ExMessageStateMachine	gSMAttackProcMsg[MAX_USER][MAXATTACKMESSAGE];	// �޽���
void gObjStateAttackProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
#endif


BILL_CLASS	m_ObjBill[MAX_OBJECT];

HANDLE	hThread_gObjMove;

#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
CRITICAL_SECTION	gAddMonCriticalSection;	// ���� ������Ʈ �ε����� ������ ���
#endif	// MODIFY_ADDMONSTER_ADDLOCK_20080117

extern int  gCheckBattleGround(LPOBJECTSTRUCT lpObj);

//////////////////////////////////////////////////////////////////////////////
int  ViewportAdd(int aIndex, int aAddIndex, int aType);
int  ViewportAdd2(int aIndex, int aAddIndex, int aType);
void ObjectMapJoinPositionSend(short aIndex);
BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int iw, int ih);
BYTE gObjInventory2RectCheck(int aIndex, int sx, int sy, int iw, int ih);
void gObjRefillProcess(int aIndex, LPOBJECTSTRUCT lpObj);
BYTE gObjInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level);
void gObjNextExpCal(LPOBJECTSTRUCT lpObj);
void gObjGiveItemSearch(LPOBJECTSTRUCT lpObj, int maxlevel);
void gObjMonsterItemLoadScript(LPOBJECTSTRUCT lpObj, char *script_file);
BYTE gObjMonsterInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level, int op1=0, int op2=0, int op3=0);
BYTE gObjWarehouseInsertItem(LPOBJECTSTRUCT lpObj, int type, int index,int level, int pos);
void gObjGiveItemWarehouseSearch(LPOBJECTSTRUCT lpObj, int maxlevel);
void gObjSkillUseProc(LPOBJECTSTRUCT lpObj);
void gObjStateProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode);

#ifdef BUGFIX_WIZARD_TELEPORT_MAGIC_20041018	
void ViewportDel (short aIndex, int aDelIndex);		// ���� ���� ���� ����Ʈ���� Ư�� �ε��� ����
void Viewport2Del (short aIndex, int aDelIndex);	// ���� ���� �����ִ� ����Ʈ���� Ư�� �ε��� ����
void gObjViewportAllDel (short aIndex);				// ���� ���ų� ���� �����ִ� ����ڵ��� ����Ʈ���� ���� ���� ����
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage);
#else

#ifdef ADD_SHIELD_POINT_01_20060403
#ifdef ADD_SKILL_WITH_COMBO
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill, int iShieldDamage);
#else
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, int iShieldDamage);
#endif
#else
#ifdef ADD_SKILL_WITH_COMBO
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill);
#else
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage);
#endif
#endif
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107



void gObjSkillBeAttackProc(LPOBJECTSTRUCT lpObj);
void gObjViewportClose(LPOBJECTSTRUCT lpObj);

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// gObjMagicAdd - Skill Type ����
int  gObjMagicAdd(LPOBJECTSTRUCT lpObj, INT aSkill, BYTE Level);
int  gObjMagicDel(LPOBJECTSTRUCT lpObj, INT aSkill, BYTE Level);
#else
int  gObjMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
int  gObjMagicDel(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
#endif

void gObjManaPotionFill(LPOBJECTSTRUCT lpObj);
void gObjRestPotionFill(LPOBJECTSTRUCT lpObj);
BOOL TradeItemInventoryPutTest(int aIndex);
void gObjInventory2ItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
void gObjSetInventory1Pointer(LPOBJECTSTRUCT lpObj);
BOOL gObjUniriaSprite(LPOBJECTSTRUCT lpObj);
void gObjMonsterDieLifePlus(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTartObj);
void gObjTimeCheckSelfDefense(LPOBJECTSTRUCT lpObj);
BYTE gObjInventoryInsertItemPos(int aIndex, CItem item, int pos, int RequestCheck);
void gObjTradeSave(LPOBJECTSTRUCT	lpObj, int index);

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
int  gObjWeaponMagicAdd(LPOBJECTSTRUCT lpObj, WORD aSkill, BYTE Level);
int  gObjMonsterMagicAdd(LPOBJECTSTRUCT lpObj, WORD aSkill, BYTE Level);
#else
int  gObjWeaponMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
int  gObjMonsterMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
#endif
void gObjAuthorityCodeSet(LPOBJECTSTRUCT lpObj);
#ifdef _NEW_EXDB_
BOOL gObjTargetGuildWarCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
BOOL gObjGuildWarCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
BOOL gObjGuildWarMasterClose(LPOBJECTSTRUCT lpObj);
BOOL gObjGuildWarItemGive(LPGUILD_INFO lpWinGuild, LPGUILD_INFO lpLoseGuild);
#endif
BOOL gObjInventorySearchSerialNumber(LPOBJECTSTRUCT lpObj, DWORD serial);
void gObjViewportListProtocolCreate(LPOBJECTSTRUCT lpObj);

#ifdef DUEL_SYSTEM_20031028
// Ư�� ����ڰ� ���������� üũ
BOOL gObjDuelCheck(LPOBJECTSTRUCT lpObj);
// ���� ���������� üũ
BOOL gObjDuelCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
// �������� �ʱ�ȭ
VOID gObjResetDuel(LPOBJECTSTRUCT lpObj);
// ������ ����Ǿ��ٴ� ��Ŷ�� ������
VOID gObjSendDuelEnd(LPOBJECTSTRUCT lpObj);
#endif

BYTE gObjWarehouseInsertItemPos(int aIndex, CItem item, int pos, int source);
BYTE gObjWarehouseDeleteItem(int aIndex, int itempos);

void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte);
BYTE gObjChaosBoxDeleteItem(int aIndex, int itempos);

void gObjChangeDurProc(LPOBJECTSTRUCT lpObj);

#ifdef MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820
void gObjChangeDurProc2(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820

void gObjMonsterCallKill(int aIndex);
void gObjWingDurProc(LPOBJECTSTRUCT lpObj);

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	void GObjExtItemApply(LPOBJECTSTRUCT lpObj, int type);  // type - 0 : ������ �ɼ�, 1 : %���� �ɼ�
#else
	void GObjExtItemApply(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519

void gObjMagicAddEnergyCheckOnOff(int flag);

void gObjCalCharacter(int aIndex);
void gObjSetPosition(int aIndex, int x, int y);

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
void gObjNProtectGGCheckSum(int aIndex);
#else
	#if defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	void gObjNProtectGGCheckSum(int aIndex);
	#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

bool gObjIsSelfDefense (LPOBJECTSTRUCT lpObj, int aTargetIndex);


#ifdef PERSONAL_SHOP_20040113
extern VOID CGPShopReqSetItemPrice		(LPPMSG_REQ_PSHOP_SETITEMPRICE lpMsg, int aIndex);
extern VOID CGPShopAnsSetItemPrice		(int aIndex, BYTE btResult, BYTE btItemPos);
extern VOID CGPShopReqOpen				(LPPMSG_REQ_PSHOP_OPEN lpMsg, int aIndex);
extern VOID CGPShopAnsOpen				(int aIndex, BYTE btResult);
extern VOID CGPShopReqClose				(int aIndex);
extern VOID CGPShopAnsClose				(int aIndex, bool bResult);
extern VOID CGPShopReqBuyList			(LPPMSG_REQ_BUYLIST_FROM_PSHOP lpMsg, int aSourceIndex);
extern VOID CGPShopAnsBuyList			(int aSourceIndex, int aTargetIndex, BYTE btResult, bool bResend = false);
extern VOID CGPShopReqBuyItem			(LPPMSG_REQ_BUYITEM_FROM_PSHOP lpMsg, int aSourceIndex);
extern VOID CGPShopAnsBuyItem			(int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult);	// A�� B�� �������� ���.
extern VOID CGPShopAnsSoldItem			(int aSourceIndex, int aTargetIndex, int iItemPos);					// A�� B���� �������� �ȾҴ�.
extern VOID CGPShopReqCloseDeal			(LPPMSG_REQ_PSHOPDEAL_CLOSE lpMsg, int aIndex);						// A�� �ٸ� ����� ���λ��� â�� �����ִ� �ݾҴ�.
extern VOID CGPShopAnsDealerClosedShop	(int aSourceIndex, int aTargetIndex);	// ���� ���λ����� ���� �ִ� ����� ������ �ݾҴ�.
#endif


#ifdef CHINA_GM_EVENT
void	gObjMakeGMEventItem( int aIndex);
void	gObjGMEventProc( int aIndex);
BOOL	gGMEventRunning = FALSE;
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
void	gObjMakeGMEventItem( int aIndex);
void	gObjGMEventProc( int aIndex);
BOOL	gGMEventRunning = FALSE;
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

int gObjInventoryRollback(int aIndex);

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ������ �Լ���
bool gObjFixInventoryPointer(int aIndex);
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518					// Į������ ���� ����� �Լ� �߰�
// Ư������ cX, cY ��ó ���� �߿� ���ڸ��� �ϳ� ã�Ƴ���.
BOOL	gObjGetRandomFreeLocation (	INT	iMapNumber,	UCHAR & cX,	UCHAR & cY, INT iRangeX, INT iRangeY, INT iLoopCount);
// �ش� �ɸ� (�����)�� ������ ������������ ��������.
INT		gObjCheckAttackTypeMagic ( INT iClass, INT iSkill);
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
void gObjShieldAutoRefill(LPOBJECTSTRUCT lpObj);
#endif

//#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
BOOL CheckInventoryEmptySpace(LPOBJECTSTRUCT lpObj, int iItemHeight, int iItemWidth);
//#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
BOOL CheckInventoryEmptySpace_(LPOBJECTSTRUCT lpObj, int iItemHeight, int iItemWidth);
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_GAMBLING_20090120
BOOL gObjGamblingInventoryCheck( int aIndex, int iWidth, int iHeight );
#endif // ADD_GAMBLING_20090120

int gObjCSFlag=0;
// 8���� ���� x,y 
short RoadPathTable[16] = {-1,-1,0,-1,1,-1,1,0,1,1,0,1,-1,1,-1,0};

int gItemLoop=0;
int gItemLoop2=0;
int gItemLoopMax=0;
int gObjTotalUser=0;
int gDisconnect=0;

#ifdef GAMESERVER_TO_CASTLESIEGESERVER				// ������ ���뼭���� �⺻ ���Ӱ��� �ο��� MAX_USER
int gServerMaxUser=MAX_USER;
#else
#ifdef FOR_THAILAND									// 2004-07-27 �±� (Ÿ��ĭ��ġ) �� �⺻ ���Ӱ��� �ο��� 300
int gServerMaxUser=300;
#else
int gServerMaxUser=500;
#endif		// FOR_THAILAND
#endif		// GAMESERVER_TO_CASTLESIEGESERVER

int aaaaaaaaa=0;

#ifdef ADD_SKILL_WITH_COMBO
void gObjSkillUseProcTime500(LPOBJECTSTRUCT lpObj)
{	
	if( lpObj->SkillHellFire2State )
	{
#ifdef MODIFY_HELLBURST_SKILL_TIME_20060522
		DWORD dwCurrentTick = GetTickCount();

		lpObj->SkillHellFire2Count = ( dwCurrentTick - lpObj->SkillHellFire2Time ) / 500;
		if( lpObj->SkillHellFire2Count > 12 )
		{
			lpObj->SkillHellFire2Count = 12;
		}

		if( dwCurrentTick >= ( lpObj->SkillHellFire2Time + ( 6 * 1000 ) ) )
		{
			CMagicInf *		lpMagic;
			lpMagic = gObjGetMagicSearch(lpObj, AT_SKILL_EXPHELL);

			if( lpMagic )
			{
				gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic);
			}
		}
#else
		lpObj->SkillHellFire2Count++;
		if( lpObj->SkillHellFire2Count >= 12 )
		{	// ������ �ܰ�� ��ų�� ����Ų��
			CMagicInf *		lpMagic;
			lpMagic = gObjGetMagicSearch(lpObj, AT_SKILL_EXPHELL);

			if( lpMagic )
			{
				gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic);	
			}
		}
#endif
		else
		{
			int reqmana = MagicDamageC.SkillGetMana(AT_SKILL_EXPHELL);
			reqmana = reqmana*20/100;

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// ���� ���� ��ȭ
			reqmana -= (reqmana * lpObj->m_MPSkillOpt.iMpsDecreaseMana) / 100;

			if(reqmana < 1)	reqmana = 1;
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
			
			int manacheck = TRUE;
			if( lpObj->Mana < reqmana )
			{
				manacheck = FALSE;

#ifdef CHECK_LOG_INFINITE_20040426 
				int _count = 0;
#endif

				while(1)
				{

#ifdef CHECK_LOG_INFINITE_20040426 
					_count++;
					if( gObjIsConnected(lpObj) == FALSE ) 
					{
						LogAdd("��[CHECK_LOG_INFINITY] gObjIsConnected() error %s %d", __FILE__, __LINE__);
						break;
					}

					if( _count > 100 )
					{
						LogAdd("��[CHECK_LOG_INFINITY] ( _count > 100 ) error %s %d", __FILE__, __LINE__);
						break;
					}
#endif

					int itempos = gObjGetManaItemPos(lpObj->m_Index);		// ���� �ѹ� ����� �ְ�

					if( itempos == -1 )
						break;

					PMSG_USEITEM pMsg;
					pMsg.inventoryPos = itempos;
					pMsg.invenrotyTarget = 0;
					
					CGUseItemRecv(&pMsg, lpObj->m_Index);

					if( lpObj->Mana >= reqmana )
					{	// Ŭ������
						manacheck = TRUE;
						break;
					}
				}
			}

			if( !manacheck )
			{	// ������ �����ϸ� skill�� ����Ų��
				CMagicInf *		lpMagic;
				lpMagic = gObjGetMagicSearch(lpObj, AT_SKILL_EXPHELL);

				if( lpMagic )
				{
					gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic);	
				}
			}
			else
			{
				// ������ ��´�
				lpObj->Mana -= (float)reqmana;
				if( lpObj->Mana < 0.f )
				{
					lpObj->Mana = 0.f;
				}
				GCManaSend(lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);		

				PMSG_EX_SKILL_COUNT	pMsg;		

				PHeadSetB((LPBYTE)&pMsg, 0xBA, sizeof( pMsg ));
				pMsg.Type = AT_SKILL_EXPHELL;
				pMsg.Count = lpObj->SkillHellFire2Count;
				pMsg.NumberH		= HIBYTE(lpObj->m_Index);
				pMsg.NumberL		= LOBYTE(lpObj->m_Index);

				DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
				MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}
#endif


void MonsterAndMsgProc()
{		
	LPOBJECTSTRUCT	lpObj;

	for( int n=0; n<MAX_OBJECT; n++)
	{
		lpObj = (LPOBJECTSTRUCT)&gObj[n];
		
		// ������ �������� ����ڸ�..
		if( lpObj->Connected == CSS_GAMEPLAYING )
		{
			if( lpObj->Type == OBJTYPE_MONSTER || lpObj->Type ==  OBJTYPE_NPC )
			{
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
				// AI�� ���� �Ǿ� ������ Continue
				if( lpObj->m_iCurrentAI )
				{
					continue;
				}
				else
#endif
				gObjMonsterProcess(lpObj);
			}
			else 
			{
#ifdef ADD_SKILL_WITH_COMBO
				gObjSkillUseProcTime500(lpObj);
#endif
				if(lpObj->m_bShell == true)
					continue;
				gObjMsgProc(lpObj);
				CreateFrustrum(lpObj->X,lpObj->Y, n);
			}

#ifdef DARKLORD_WORK	// ��ũ���Ǹ� ó��
			if( lpObj->Type == OBJTYPE_CHARACTER )
				gDarkSpirit[n].Run();
#endif
		}
		else if( lpObj->Connected >= CSS_LOGINOK )
		{
			if( lpObj->Type == OBJTYPE_CHARACTER )
			{
				gObjMsgProc(lpObj);
				if( lpObj->Connected == CSS_GAMEPLAYING )
				{
					CreateFrustrum(lpObj->X,lpObj->Y, n);
				}
			}				
		}

		
	} // for( n )

#ifdef ADD_SKILL_WITH_COMBO		// ���� ���ݿ� ���� ó��
	int aIndex;
	for( int n=0; n<MAX_USER; n++)
	{
		lpObj = (LPOBJECTSTRUCT)&gObj[n+ALLOC_USEROBJECTSTART];

		if( lpObj->Connected == CSS_GAMEPLAYING )
		{		
			aIndex = lpObj->m_Index;

			for( int k=0; k<MAXATTACKMESSAGE; k++)
			{
				if( gSMAttackProcMsg[n][k].MsgCode >= 0 )
				{
					if( GetTickCount() > gSMAttackProcMsg[n][k].MsgTime )
					{					
						gObjStateAttackProc(lpObj, 
											gSMAttackProcMsg[n][k].MsgCode, 
											gSMAttackProcMsg[n][k].SendUser, 
											gSMAttackProcMsg[n][k].SubCode,
											gSMAttackProcMsg[n][k].SubCode2);

						gSMAttackProcMsg[n][k].MsgCode = -1;
					}
				}
			}
		}
	}
#elif defined(NEW_ATTACK_PROC)
	int aIndex;
	for( int n=0; n<MAX_USER; n++)
	{
		lpObj = (LPOBJECTSTRUCT)&gObj[n+ALLOC_USEROBJECTSTART];

		if( lpObj->Connected == CSS_GAMEPLAYING )
		{		
			aIndex = lpObj->m_Index;

			for( int k=0; k<MAXATTACKMESSAGE; k++)
			{
				if( gSMAttackProcMsg[n][k].MsgCode >= 0 )
				{
					if( GetTickCount() > gSMAttackProcMsg[n][k].MsgTime )
					{					
						gObjStateAttackProc(lpObj, 
							gSMAttackProcMsg[n][k].MsgCode, 
							gSMAttackProcMsg[n][k].SendUser, 
							gSMAttackProcMsg[n][k].SubCode,
							gSMAttackProcMsg[n][k].SubCode2);

						gSMAttackProcMsg[n][k].MsgCode = -1;
					}
				}
			}
		}
	}
#endif
	
#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
	// ���� ��ų ���� ���ݿ� ���� ó��
	TMonsterSkillManager::MonsterSkillProc();
#endif

}

// ���͸� �����δ�.
void MoveMonsterProc()
{		
	DWORD MoveTime;
	int   DelayTime;
	LPOBJECTSTRUCT	lpObj;
	
	for( int n=0; n<MAX_OBJECT; n++)
	{		
		lpObj = (LPOBJECTSTRUCT)&gObj[n];

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
		// Monster AI �� ���� �� OBJECT�� ��� ���ο� �̵� �ý����� ����Ѵ�.
		// ���ο� �̵� �ý����� 100ms ���� ���� �ȴ�.
		if( lpObj->m_iCurrentAI && (lpObj->Type==OBJTYPE_MONSTER||lpObj->Type==OBJTYPE_NPC) )
		{
			//TMonsterAI::MonsterMoveProc( n );
			continue;
		}
#endif
		
		// ������ �������� ����ڸ�..
		if( lpObj->Connected == CSS_GAMEPLAYING )
		{
#ifdef ADD_NEW_MAP_KALIMA_20040518
			if (lpObj->Type == OBJTYPE_MONSTER &&
				(CHECK_KALIMAGATE(lpObj->m_Attribute)	// ������ �������� �ʴ´�.
#ifdef CASTLE_MAIN_SCHEDULER_20041111					// ������ ���� NPC �� �������� �ʴ� �͵�
				|| (lpObj->Class == 277)	// ����
				|| (lpObj->Class == 283)	// ��ȣ����
				|| (lpObj->Class == 288)	// ĳ��Ÿ��
				|| (lpObj->Class == 278)	// ����������
				|| (lpObj->Class == 216)	// �հ�
				|| (lpObj->Class == 217)	// �հ�����ġ
				|| (lpObj->Class == 218)	// �հ�����ġ
				|| (lpObj->Class == 219)	// ��������
#endif

#ifdef CRYWOLF_NPC_WORK_20050912
				|| CHECK_CRYWOLF_NPC_ALTAR(lpObj->Class)		// ������ ����
				|| CHECK_CRYWOLF_NPC_STATUE(lpObj->Class)		// ������ �Ż�
#endif
				))
			{
				CreateFrustrum(lpObj->X,lpObj->Y, n);
				continue;
			}
#endif

			if( lpObj->m_State == OBJST_PLAYING )
			{
				
#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				if( lpObj->PathCount != 0 &&
					( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == false ) &&
					( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STUN ) == false )
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
					&& ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == false )
#endif					
				  )
	#else
				if( lpObj->PathCount && !lpObj->m_SkillHarden && !lpObj->m_iSkillStunTime)
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#else

	#ifdef NEW_SKILL_FORSKYLAND
				if( lpObj->PathCount && !lpObj->m_SkillHarden )
	#else
				if( lpObj->PathCount )
	#endif

#endif
				{
					// Ŭ���̾�Ʈ�� ��Ÿ���� �̵��ϴµ� �ɸ��� �ð� 480ms
					if( lpObj->DelayLevel )
					{
						DelayTime = 300;
					}
					else DelayTime = 0;
					
					if( lpObj->Type == OBJTYPE_MONSTER && lpObj->m_RecallMon >= 100 )
						lpObj->m_MoveSpeed = 200;

					else lpObj->m_MoveSpeed = 400;

					if( (lpObj->PathDir[lpObj->PathCur]%2) == 0 )
					{
						MoveTime = (DWORD)((lpObj->m_MoveSpeed+DelayTime)*1.3);//sqrt((200*200)+(200*200));
					}
					else MoveTime = (lpObj->m_MoveSpeed+DelayTime); //400
					
					
					if( (GetTickCount()-lpObj->PathTime) > MoveTime )  
					{ 
						if( lpObj->PathCur < MAX_ROADPATH-1 ) 
						{
#ifdef MODIFY_FORCEMOVE_TO_LORENCIA_20060515
							// ���� �� ���� ������ �����̶��, �̵��� ����� �Ѵ�.
							int nextX = lpObj->PathX[lpObj->PathCur];
							int nextY = lpObj->PathY[lpObj->PathCur];
							BYTE mapnumber;
							mapnumber	= lpObj->MapNumber;
#if ENABLE_MC_CODE == 1
							if (mapnumber > g_TerrainManager.Size() - 1)
#else // ENABLE_MC_CODE == 1
							if (mapnumber > MAX_MAP - 1)
#endif // ENABLE_MC_CODE == 1
							{
								// ��ū ����;;;
							}
							BYTE attr =  MapC[mapnumber].GetAttr(nextX, nextY);
							if( lpObj->Type == OBJTYPE_CHARACTER &&
								( (attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK || 
								(attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW
								) )
							{
								// ������ �� �߰�								
								LogAddTD( "[ CHECK POSITION ] MoveMosterProc [%s][%s] Map[%d]-(%d,%d) User(%d,%d) Can not Move Position Attr[%d]", 
									lpObj->AccountID, lpObj->Name, 
									lpObj->MapNumber, nextX, nextY, 
									lpObj->X, lpObj->Y,
									attr );							
								
								// ��� �ʱ�ȭ ����� �Ѵ�.
								for( n=0; n<MAX_ROADPATH; n++)
								{
									lpObj->PathX[n] = 0;
									lpObj->PathY[n] = 0;
									lpObj->PathOri[n] = 0;
								}
								lpObj->PathCount = 0;
								lpObj->PathCur = 0;
								lpObj->PathTime = GetTickCount();
								if( lpObj->Type == OBJTYPE_MONSTER || lpObj->Type == OBJTYPE_NPC )
								{
									lpObj->PathStartEnd = 0;
								}			
								// 0524 �߰� ����
								if ( lpObj->Type == OBJTYPE_CHARACTER )
								{
									gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
								}
							}
							else
							{							
#endif //MODIFY_FORCEMOVE_TO_LORENCIA_20060515							
								
							lpObj->X   = lpObj->PathX[lpObj->PathCur];
							lpObj->Y   = lpObj->PathY[lpObj->PathCur];
							lpObj->Dir = lpObj->PathDir[lpObj->PathCur];

							lpObj->PathTime = GetTickCount();
							lpObj->PathCur++;
						
							if( lpObj->PathCur >= lpObj->PathCount ) // �� �̵��ߴٸ�..
							{
								lpObj->PathCur = 0;
								lpObj->PathCount = 0;
								if( lpObj->Type == OBJTYPE_MONSTER || lpObj->Type == OBJTYPE_NPC )
								{
									lpObj->PathStartEnd = 0;
								}
							}
#ifdef MODIFY_FORCEMOVE_TO_LORENCIA_20060515
							}	// else{
#endif
						}
					}
					
				}				
				CreateFrustrum(lpObj->X,lpObj->Y, n);
			}	
		}
		
	} // for( n )
}


void gObjRefillProcess(int aIndex, LPOBJECTSTRUCT lpObj)
{
	/*if( GetTickCount()-lpObj->LifeRefillTimer > 10000 )
	{
		if( (lpObj->Life+2) <= lpObj->MaxLife )
		{
			lpObj->Life += 2;
			GCReFillSend(aIndex, lpObj->Life, 0xff);
			lpObj->LifeRefillTimer = GetTickCount();
		}
	}*/
}

//////////////////////////////////////////////////////////////////////////////
// gObj ������ �ʱ�ȭ �Ѵ�.
void gObjInit()
{
	gObjCount = ALLOC_USEROBJECTSTART;
	gObjMonCount = 0;
	gObjCallMonCount = MAX_MONSTER;	

#ifdef UPGRADE_GAMESERVER_01_20041006
	// ��¥ �κ��丮.. ^^;
	CItem *pTempInventory		= new CItem [MAX_INVENTORY_EXTEND];
	BYTE  *pTempInventoryMap	= new BYTE  [MAX_INVENTORYMAP + MAX_PSHOPITEMMAP];

#endif

#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
	InitializeCriticalSection(&gAddMonCriticalSection);
#endif

	for( int n=0; n<MAX_OBJECT; n++)
	{
		memset(&gObj[n], 0, sizeof(OBJECTSTRUCT));
		gObj[n].Type = 255;
		gObj[n].m_socket = INVALID_SOCKET;
		gObj[n].PathTime = GetTickCount();
		gObj[n].m_RecallMon= -1;
		
		gObj[n].m_lpMagicBack = new CMagicInf [MAX_MAGIC];
		_ASSERT( gObj[n].m_lpMagicBack );		

		gObj[n].Magic	   = new CMagicInf [MAX_MAGIC];
		_ASSERT( gObj[n].Magic );

#ifdef PERSONAL_SHOP_20040113

#ifdef UPGRADE_GAMESERVER_01_20041006
		// ���ʹ� �κ��丮�� �ʿ� ����.
		if( n >= ALLOC_USEROBJECTSTART )
		{
			gObj[n].Inventory1 = new CItem [MAX_INVENTORY_EXTEND];
			_ASSERT( gObj[n].Inventory1 );		
			
			gObj[n].Inventory2 = new CItem [MAX_INVENTORY_EXTEND];
			_ASSERT( gObj[n].Inventory2 );
			
			gObj[n].InventoryMap1 = new BYTE [MAX_INVENTORYMAP + MAX_PSHOPITEMMAP];
			_ASSERT( gObj[n].InventoryMap1 );
			
			gObj[n].InventoryMap2 = new BYTE [MAX_INVENTORYMAP + MAX_PSHOPITEMMAP];
			_ASSERT( gObj[n].InventoryMap2 );
		}
		else
		{
			gObj[n].Inventory1 = pTempInventory;
			_ASSERT( gObj[n].Inventory1 );		
			
			gObj[n].Inventory2 = pTempInventory;
			_ASSERT( gObj[n].Inventory2 );
			
			gObj[n].InventoryMap1 = pTempInventoryMap;
			_ASSERT( gObj[n].InventoryMap1 );
			
			gObj[n].InventoryMap2 = pTempInventoryMap;
			_ASSERT( gObj[n].InventoryMap2 );
		}
#else
		gObj[n].Inventory1 = new CItem [MAX_INVENTORY_EXTEND];
		_ASSERT( gObj[n].Inventory1 );		
		
		gObj[n].Inventory2 = new CItem [MAX_INVENTORY_EXTEND];
		_ASSERT( gObj[n].Inventory2 );

		gObj[n].InventoryMap1 = new BYTE [MAX_INVENTORYMAP + MAX_PSHOPITEMMAP];
		_ASSERT( gObj[n].InventoryMap1 );

		gObj[n].InventoryMap2 = new BYTE [MAX_INVENTORYMAP + MAX_PSHOPITEMMAP];
		_ASSERT( gObj[n].InventoryMap2 );
#endif //UPGRADE_GAMESERVER_01_20041006

#else
		gObj[n].Inventory1 = new CItem [MAX_INVENTORY];
		_ASSERT( gObj[n].Inventory1 );
		
		gObj[n].Inventory2 = new CItem [MAX_INVENTORY];
		_ASSERT( gObj[n].Inventory2 );

		gObj[n].InventoryMap1 = new BYTE [MAX_INVENTORYMAP];
		_ASSERT( gObj[n].InventoryMap1 );

		gObj[n].InventoryMap2 = new BYTE [MAX_INVENTORYMAP];
		_ASSERT( gObj[n].InventoryMap2 );
#endif //PERSONAL_SHOP_20040113


#ifdef UPGRADE_GAMESERVER_01_20041006
		// ���ʹ� �ŷ�����, â�� �ʿ� ����.
		if( n >= ALLOC_USEROBJECTSTART )
		{
			gObj[n].Trade			= new CItem [MAX_TRADEITEM];
			_ASSERT( gObj[n].Trade );
			
			gObj[n].TradeMap		= new BYTE [MAX_TRADEITEM];
			_ASSERT( gObj[n].TradeMap );
			
			gObj[n].pWarehouse		= new CItem [MAX_WAREHOUSEITEMS];
			_ASSERT( gObj[n].pWarehouse );
			
			gObj[n].pWarehouseMap	= new BYTE [MAX_WAREHOUSEITEMS];
			_ASSERT( gObj[n].pWarehouseMap );
		}
#else
		gObj[n].Trade			= new CItem [MAX_TRADEITEM];
		_ASSERT( gObj[n].Trade );

		gObj[n].TradeMap		= new BYTE [MAX_TRADEITEM];
		_ASSERT( gObj[n].TradeMap );

		gObj[n].pWarehouse		= new CItem [MAX_WAREHOUSEITEMS];
		_ASSERT( gObj[n].pWarehouse );
	

		gObj[n].pWarehouseMap	= new BYTE [MAX_WAREHOUSEITEMS];
		_ASSERT( gObj[n].pWarehouseMap );
#endif
		
		gObj[n].WarehouseCount  = 0;
		gObj[n].pChaosBox		= NULL;

		/*gObj[n].pChaosBox       = new CItem [MAX_CHAOSBOXSIZE];
		_ASSERT( gObj[n].pChaosBox );
		gObj[n].pChaosBoxMap	= new BYTE [MAX_CHAOSBOXSIZE];
		_ASSERT( gObj[n].pChaosBoxMap );
		*/

#ifdef UPGRADE_GAMESERVER_01_20041006
		// ���ʹ� SOCKET_CONTEXT�� �ʿ����.
		if( n >= ALLOC_USEROBJECTSTART )
			gObj[n].PerSocketContext = new PER_SOCKET_CONTEXT;
#else
		gObj[n].PerSocketContext = new PER_SOCKET_CONTEXT;
#endif


#ifdef PERSONAL_SHOP_20040113	
		InitializeCriticalSection (& gObj[n].m_critPShopTrade);
#endif
#ifdef GMCOMMAND_EX
		gObj[n].m_bGMSummon = FALSE;
#endif // GMCOMMAND_EX
	}
	InitFrustrum();
}

void gObjEnd()
{	
	gObjAllDisconnect();
	gObjAllLogOut();
	//gObjAllDel();

	//CloseHandle(hThread_gObjMove);

	for( int n=0; n<MAX_OBJECT; n++)
	{
		if( gObj[n].m_lpMagicBack != NULL) 
			delete [] gObj[n].m_lpMagicBack;
#ifdef UPGRADE_GAMESERVER_01_20041006
		if( n >= ALLOC_USEROBJECTSTART )
		{
			
			if( gObj[n].Inventory1 != NULL )
				delete [] gObj[n].Inventory1;
			if( gObj[n].Inventory2 != NULL )
				delete [] gObj[n].Inventory2;
			if( gObj[n].InventoryMap1 != NULL )
				delete [] gObj[n].InventoryMap1;
			if( gObj[n].InventoryMap2 != NULL )
				delete [] gObj[n].InventoryMap2;
			if( gObj[n].PerSocketContext != NULL )
				delete gObj[n].PerSocketContext;
			if( gObj[n].Trade != NULL ) 
				delete gObj[n].Trade;
			if( gObj[n].TradeMap != NULL )
				delete gObj[n].TradeMap;
			if( gObj[n].pWarehouse != NULL )
				delete gObj[n].pWarehouse;
			if( gObj[n].pWarehouseMap != NULL )
				delete gObj[n].pWarehouseMap;
		}
#else
		if( gObj[n].Inventory1 != NULL )
			delete [] gObj[n].Inventory1;
		if( gObj[n].Inventory2 != NULL )
			delete [] gObj[n].Inventory2;
		if( gObj[n].InventoryMap1 != NULL )
			delete [] gObj[n].InventoryMap1;
		if( gObj[n].InventoryMap2 != NULL )
			delete [] gObj[n].InventoryMap2;
		if( gObj[n].PerSocketContext != NULL )
			delete gObj[n].PerSocketContext;
		if( gObj[n].Trade != NULL ) 
			delete gObj[n].Trade;
		if( gObj[n].TradeMap != NULL )
			delete gObj[n].TradeMap;
		if( gObj[n].pWarehouse != NULL )
			delete gObj[n].pWarehouse;
		if( gObj[n].pWarehouseMap != NULL )
			delete gObj[n].pWarehouseMap;
#endif
		/*
		if( gObj[n].pChaosBox != NULL )
			delete gObj[n].pChaosBox;
		if( gObj[n].pChaosBoxMap != NULL )
			delete gObj[n].pChaosBoxMap;
		*/
		
#ifdef PERSONAL_SHOP_20040113	
		DeleteCriticalSection (& gObj[n].m_critPShopTrade);
#endif
	}

#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
	DeleteCriticalSection(&gAddMonCriticalSection);
#endif
}

//#HAPPY_CAT
void	gObjClearViewportOfMine(LPOBJECTSTRUCT lpObj) 
{
	for( int i = 0; i < MAXVIEWPORTOBJECT; i++) 
	{
		lpObj->VpPlayer[i].state   = STVP_NONE;
		lpObj->VpPlayer[i].number  = -1;
	}

	lpObj->VPCount = 0;
}	


void gObjClearViewport(LPOBJECTSTRUCT lpObj)
{
	int i;
	for( i=0; i<MAXVIEWPORTOBJECT; i++) 
	{
		lpObj->VpPlayer[i].state   = STVP_NONE;
		lpObj->VpPlayer[i].number  = -1;
		lpObj->VpPlayer2[i].state  = STVP_NONE;
		lpObj->VpPlayer2[i].number = -1;
	}
	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
}


// ���Ḧ �˸���.
void gObjCloseSet(int aIndex, int Flag)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return ;
	LPOBJECTSTRUCT	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	
	if( lpObj->CloseCount > 0 ) return;

	if( lpObj->Connected == CSS_GAMEPLAYING )
	{
#ifdef FOR_BLOODCASTLE
		if (CHECK_BLOODCASTLE(lpObj->MapNumber)) {				// ����ĳ���� ����ڶ�� �ش� �������� ����ؾ� �Ѵ�.
			g_BloodCastle.SearchUserDropQuestItem(aIndex);
		}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ����� ����ϰ�� ���� ����
		if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
		{
			g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
			g_IllusionTempleEvent.LeaveIllusionTemple(lpObj->m_Index, lpObj->MapNumber);
		}
#endif

		// ���� ���� ���� �߿� �ڵ� ���ᰡ �ȵȴ�.
#ifdef MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221	// ������� �ð� 60��
		if( GetTickCount()-lpObj->MySelfDefenseTime  < (1000*60) )
#else	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221
#ifdef UPDATE_PK_SYSTEM_20070302	// ������� ���ӽð� ����

		if( GetTickCount()-lpObj->MySelfDefenseTime  < (1000*90) )
#else
		if( GetTickCount()-lpObj->MySelfDefenseTime  < (1000*30) )
#endif
#endif	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221
		{
			GCServerMsgStringSend(lMsg.Get(1133), lpObj->m_Index, 1);
#ifdef MODIFY_SELFDEFENSE_CLOSE_20070321
			GCCloseMsgSend(aIndex, 0xFF);
#endif
			return;
		}
		if( lpObj->m_IfState.use && 
			(lpObj->m_IfState.type == I_TRADE || 
			lpObj->m_IfState.type == I_WAREHOUSE||
#ifdef DARKLORD_WORK
			lpObj->m_IfState.type == I_DARK_TRAINER||
#endif
			lpObj->m_IfState.type == I_CHAOSBOX))
		{
#ifdef MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328
			gObjSaveChaosBoxItemList( lpObj );
#endif // MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328
			switch( lpObj->m_IfState.type )
			{
			case I_TRADE :
				GCServerMsgStringSend(lMsg.Get(1134), lpObj->m_Index, 1);
				break;
			case I_WAREHOUSE :
				GCServerMsgStringSend(lMsg.Get(1135), lpObj->m_Index, 1);
				break;
			case I_CHAOSBOX :
				GCServerMsgStringSend(lMsg.Get(1151), lpObj->m_Index, 1);	// ī���� �ڽ��� ���� �� �����Ͻʽÿ�.
				break;
#ifdef DARKLORD_WORK
			case I_DARK_TRAINER :
				GCServerMsgStringSend(lMsg.Get(1253), lpObj->m_Index, 1);	// ���û� â�� ���� �� �����Ͻʽÿ�
				break;
#endif
			}
			return;
		}
	}

	lpObj->CloseCount	= g_GlobalConfig.CloseCountTime;
	lpObj->CloseType	= Flag;

#ifndef MODIFY_PERIODITEM_EFFECT_DURING_CLOSETIME_20070416
	#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	g_CashItemPeriodSystem.GDReqPeriodItemUpdate( lpObj );
	g_CashItemPeriodSystem.ClearPeriodItemEffect( lpObj );
	#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#endif // MODIFY_PERIODITEM_EFFECT_DURING_CLOSETIME_20070416

#ifdef BLOCK_INVALID_CHARACTER_DELETE_01_20060327
	lpObj->bEnableDelCharacter = TRUE;
#endif
	
	/*switch( Flag )
	{
	case CL_SERVERLIST :
	case CL_EXIT :	// ���� ����
		if( lpObj->Connected == CSS_GAMEPLAYING )
		{
			if( lpObj->CloseCount < 1 )
			{
				lpObj->CloseCount = 6;
				if( Flag == CL_SERVERLIST )
					lpObj->CloseType = CL_SERVERLIST;
				else lpObj->CloseType = CL_EXIT;
			}
		}
		break;
	
	case CL_BACK :
		// �ŷ� �� â�� ��� �߿� �ڵ� ���ᰡ �ȵȴ�.
		if( lpObj->m_IfState.use == 1 && 
			(lpObj->m_IfState.type == I_TRADE || 
			lpObj->m_IfState.type == I_WAREHOUSE))
		{
			switch( lpObj->m_IfState.type )
			{
			case I_TRADE :
				GCServerMsgStringSend("�ŷ�â�� ���� �� ���� �Ͻʽÿ�.", lpObj->m_Index, 1);
				break;
			case I_WAREHOUSE :
				GCServerMsgStringSend("â�� ���� �� ���� �Ͻʽÿ�.", lpObj->m_Index, 1);
				break;
			}
			return;
		}
		if( lpObj->Connected == CSS_GAMEPLAYING )
		{
			if( lpObj->CloseCount < 1 )
			{
				lpObj->CloseCount = 6;
				lpObj->CloseType = CL_BACK;
			}
		}
		else if( lpObj->Connected == CSS_CHARACTERVIEW )
		{
			GCCloseMsgSend(aIndex, 0x02);	// ������������
		}
		break;
	}
	*/
}

// ĳ���� Ʈ���̵带 Ŭ�����Ѵ�.
void gObjCharTradeClear(LPOBJECTSTRUCT	lpObj)
{
#ifdef UPGRADE_GAMESERVER_01_20041006
	// ���ʹ� �ŷ� ������ �ʿ� ����.
	if( lpObj->Type != OBJTYPE_CHARACTER )
		return;
#endif

	memset(lpObj->TradeMap, 0xFF, 4*8);
	for( int i=0; i<MAX_TRADEITEM; i++ ) lpObj->Trade[i].Clear();
	lpObj->TradeMoney = 0;
	lpObj->TradeOk    = 0;
}

void gObjCharZeroSet(int aIndex)
{
	int i;
	LPOBJECTSTRUCT	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	
	memset(lpObj->Name,      0, MAX_IDSTRING);

#ifdef GUILDNAME_MEMSET_20050410
	memset(lpObj->GuildName, 0, sizeof(lpObj->GuildName));
#endif

#ifdef DUEL_SYSTEM_20031028
	lpObj->m_Option		= OPTION_TRADE | OPTION_DUEL;
#else
	lpObj->m_Option		= OPTION_TRADE;
#endif
	
#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
	lpObj->m_iScriptMaxLife = 0;
#endif // MODIFY_CRYWOLF_BENEFIT_01_20060315

#ifdef MODIFY_MATCHDB_STRANGE_UPDATING_20060822
	lpObj->Level		= 0;
	
	#ifdef LOGINSERVER_MULOG
	ZeroMemory( lpObj->BackName, MAX_IDSTRING + 1 );
	#endif

#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// �����ͷ��� ���� �ʱ�ȭ
	lpObj->m_nMasterLevel			= 0;	

	lpObj->m_i64MasterLevelExp		= 0;
	lpObj->m_i64NextMasterLevelExp	= 0;

	lpObj->m_iMasterLevelPoint		= 0;

	lpObj->m_bMasterLevelDBLoad		= FALSE;
#endif
	
	lpObj->Life			= 0.f;
	lpObj->MaxLife		= 0.f;
	lpObj->AddLife		= 0;
	lpObj->AddMana		= 0;
	lpObj->Live			= 0;
	lpObj->Dir			= 0;
	lpObj->X			= 0;
	lpObj->Y			= 0;
	lpObj->TX			= 0;
	lpObj->TY			= 0;
	lpObj->MTX			= 0;
	lpObj->MTY			= 0;
	lpObj->MapNumber	= 0;
	lpObj->RegenTime	= 0;
	lpObj->m_State		= OBJST_NONE;
	lpObj->RegenOk		= 0;
	lpObj->lpAttackObj	= NULL;
//	lpObj->PartyNumber	= -1;
	lpObj->PartyTargetUser	= -1;
	lpObj->m_MoveGateNumber	= -1;
	lpObj->CloseCount   = -1;
	lpObj->CloseType    = -1;
	lpObj->m_IfState.state = -1;
	lpObj->m_IfState.type  = -1;
	lpObj->m_IfState.use   = 0;
	lpObj->m_InterfaceTime = 0;
	lpObj->m_ActionNumber = 0;
	lpObj->m_State = 0;
	lpObj->m_StateSub =0;
	lpObj->m_ViewState = 0;
	lpObj->m_Rest = 0;

	//------------->
	//20080930 �ؿ� 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
	lpObj->m_siDieRegenX		=	0;			// ��Ȱ ���� X
	lpObj->m_siDieRegenY		=	0;			// ��Ȱ ���� Y
	lpObj->m_siDieRegenMapIndex	=	0;			// ��Ȱ ���� Map
	
	lpObj->m_siMoveX			=	0;			// �̵� ���� X
	lpObj->m_siMoveY			=	0;			// �̵� ���� Y
	lpObj->m_siMoveMapIndex		=	0;			// �̵� ���� Map
#endif
	//<-------------

#ifdef ADD_SHIELD_POINT_01_20060403
	lpObj->iMaxShield = 0;
	lpObj->iShield = 0;
	lpObj->iAddShield = 0;

	lpObj->iFillShield = 0;
	lpObj->iFillShieldMax = 0;
	lpObj->iFillShieldCount = 0;

	lpObj->dwShieldAutoRefillTimer = 0;
#endif

#ifdef ITEM_ADD_OPTION_20061019
	lpObj->m_wItemEffectType = 0;
	lpObj->m_iItemEffectValidTime = 0;
#endif // ITEM_ADD_OPTION_20061019

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	lpObj->m_wCashPoint = 0;
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	lpObj->m_iPeriodItemEffectIndex	= -1;
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef MODIFY_DENY_USER_DISCONNNECT_HACK_20080323
	lpObj->m_UserKillRecvCount = 0;
#endif // MODIFY_DENY_USER_DISCONNNECT_HACK_20080323
	
#ifdef ADD_PCS_MARK_OF_EXP_20070205
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	lpObj->m_wExprienceRate = 100;
	lpObj->m_wItemDropRate	= 100;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // ADD_PCS_MARK_OF_EXP_20070205

	lpObj->m_wExcellentDropRate = 2000;
#ifdef ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907
	lpObj->m_wAncientDropRate = 100;
#endif

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	gObjClearBuffEffect( lpObj, CLEAR_TYPE_LOGOUT );
#else
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	lpObj->m_ViewSkillState = STATE_NONE;
	#else
	lpObj->m_ViewSkillState = 0;
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	lpObj->m_PoisonBeattackCount = 0;
	lpObj->m_ColdBeattackCount = 0;

	lpObj->m_SkillDefense		= 0;
	lpObj->m_SkillDefenseTime	= 0;
	lpObj->m_SkillAttack		= 0;
	lpObj->m_SkillAttackTime	= 0;

	#ifdef NEW_SKILL_FORSKYLAND
	lpObj->m_SkillAddLife		= 0;
	lpObj->m_SkillAddLifeTime	= 0;

	lpObj->m_SkillHarden		= 0;
	lpObj->m_SkillHardenTime	= 0;	

	lpObj->m_SkillMagumReduceDefense		= 0;
	lpObj->m_SkillMagumReduceDefenseTime	= 0;	
	#endif

	#ifdef NEW_SKILL_FORSKYLAND
	lpObj->m_WizardSkillDefense	= 0;
	lpObj->m_WizardSkillDefenseTime = 0;
	#endif // NEW_SKILL_FORSKYLAND

	#ifdef DARKLORD_WORK
	lpObj->SkillAddCriticalDamageTime = 0;
	lpObj->SkillAddCriticalDamage = 0;
	#endif 

	#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	lpObj->m_iSkillStunTime				= 0;		// ���� ���� �ð�
	lpObj->m_iSkillBrandOfSkillTime		= 0;		// �귣����꽺ų ���ӽð�
	lpObj->m_iSkillInvisibleTime		= 0;		// �κ����� ���ӽð�
	lpObj->m_iSkillManaSwellTime		= 0;		// �������� ���ӽð�
	lpObj->m_iSkillManaSwell			= 0;		// ����������
	#endif 

	#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	lpObj->m_iPotionBlessTime			= 0;		// �ູ�� ���� ���� �ð�
	lpObj->m_iPotionSoulTime			= 0;		// ��ȥ�� ���� ���� �ð� 
	#endif

	#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
	lpObj->m_iSkillNPCHelpTime = 0;
	lpObj->m_iSkillNPCDefense = 0;
	lpObj->m_iSkillNPCAttack = 0;
	#endif

	#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	lpObj->m_iMuseElfInfinityArrowSkillTime	= 0;
	#endif

	#ifdef PCBANG_POINT_SYSTEM_20070206		// �ʱ�ȭ
	lpObj->m_iPCBangPointItemTime		= 0;
	lpObj->m_iPCBangPointItemAddExpPer	= 0;
	lpObj->m_bPCBangPointNoMoreExp		= 0;
	lpObj->m_iPCBangPointItemGiveItemPer	= 0;	
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	lpObj->m_Attribute	= 0;

#ifdef ADD_NEW_MAP_KALIMA_20040518					// ���� ������ �ʱ�ȭ
	lpObj->m_ImmuneToMagicCount			= 0;		// �������� ���� �ð�
	lpObj->m_ImmuneToHarmCount			= 0;		// �������� ���� �ð�
	lpObj->m_iMonsterBattleDelay		= 0;		// ���Ͱ� ���� �� ��� �ӹ��Ÿ� �� �����̵Ǵ� �ð� (���ݵ����� ������ ����)
	
	lpObj->m_cKalimaGateExist			= false;	// �� ������ ������ �����ϴ��� ����
	lpObj->m_iKalimaGateIndex			= -1;		// �� ������ ������ �ε���

	lpObj->m_cKalimaGateEnterCount		= 0;		// ������ ���尡�� Ƚ��
#endif

	lpObj->TradeOk = 0;
	lpObj->sHDCount = 0;
	lpObj->PathCount = 0;
	lpObj->PathCur =0 ;
	lpObj->PathStartEnd = 0;
	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
	lpObj->m_ShopTime = 0;
	lpObj->m_ManaFillCount = 0;

	lpObj->FillLife = 0.f;

#ifdef ADD_KUNDUN_CONTROL_20040923
	// ������Ʈ���� �ʸ� �����ϱ� ���� ó��
	lpObj->iObjectSecTimer = 0;
#endif

	// ����
#ifdef __FRIEND_WORK__
	lpObj->m_FriendServerOnline = FRIENDSERVER_ONLINE;
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM	
	lpObj->bWaitBlockState = FALSE;
	ChinaHackUserKick.InitUserInfo(lpObj->m_Index);
#endif
		
	lpObj->SetOpAddMaxAttackDamage	= 0;
	lpObj->SetOpAddMinAttackDamage	= 0;
	lpObj->SetOpAddDamage	= 0;	
	lpObj->SetOpIncAGValue	= 0;
	lpObj->SetOpAddCriticalDamageSuccessRate	= 0;
	lpObj->SetOpAddCriticalDamage	= 0;
	lpObj->SetOpAddExDamageSuccessRate	= 0;
	lpObj->SetOpAddExDamage	= 0;
	lpObj->SetOpAddSkillAttack	= 0;
	
	lpObj->AddStrength	= 0;
	lpObj->AddDexterity = 0;
	lpObj->AddVitality	= 0;
	lpObj->AddEnergy	= 0;

	lpObj->SetOpAddAttackDamage	= 0;
	lpObj->SetOpAddDefence		= 0;
	lpObj->SetOpAddMagicPower	= 0;
	lpObj->SetOpAddDefenceRate	= 0;

	lpObj->SetOpIgnoreDefense				= 0;
	lpObj->SetOpDoubleDamage				= 0;
	lpObj->SetOpTwoHandSwordImproveDamage	= 0;
	lpObj->SetOpImproveSuccessAttackRate	= 0;
	lpObj->SetOpReflectionDamage			= 0;
	lpObj->SetOpImproveSheldDefence			= 0;
	lpObj->SetOpDecreaseAG					= 0;
	lpObj->SetOpImproveItemDropRate			= 0;

	memset(lpObj->m_AddResistance,	0, MAX_RESISTANCE);

	lpObj->IsFullSetItem					= 0;

	lpObj->DamageMinus	      = 0;
	lpObj->DamageReflect      = 0;
	lpObj->MonsterDieGetMoney = 0;
	lpObj->MonsterDieGetLife  = 0;
	lpObj->MonsterDieGetMana  = 0;

	lpObj->m_CriticalDamage = 0;
	lpObj->m_ExcelentDamage = 0;

	lpObj->UseMagicNumber = 0;
	lpObj->UseMagicTime   = 0;
	lpObj->UseMagicCount  = 0;

	lpObj->OSAttackSerial = -1;
	lpObj->SASCount       = 0;
	lpObj->SkillAttackTime= GetTickCount();
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	lpObj->m_nSoulBarrierDefence	= 0;
	lpObj->m_nSoulBarrierManaRate	= 0;
#endif

	lpObj->m_AttackSpeed = 0;
	lpObj->m_MagicSpeed = 0;

	lpObj->m_TotalAttackTime = GetTickCount();
	lpObj->m_TotalAttackCount=0;
	lpObj->CheckSumTime		 = GetTickCount()+(1000*10);
	lpObj->CheckSumTableNum  = -1;
	
	lpObj->m_SumLastAttackTime = 0;
	lpObj->m_DetectCount = 0;
	lpObj->m_DetectSpeedHackTime = 0;
	lpObj->m_SpeedHackPenalty = 0;
	lpObj->m_DetectedHackKickCount = 0;

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	lpObj->m_Curse				= 0;
#endif

#if defined ( MODIFY_HACKLOG_OF_ATTACK_20050518 ) || defined ( MU_CHN_HACKLOG3_MODIFY_20041117 )
	lpObj->dwPlayTime = GetTickCount();
#endif	

	lpObj->IsInBattleGround = 0;
	

	lpObj->m_dwPKTimer = GetTickCount();
	lpObj->CheckTickCount = 0;

	lpObj->m_AttackSpeedHackDetectedCount = 0;
	
	for( int n=0; n<MAXVIEWPORTOBJECT; n++)
	{
		lpObj->VpPlayer[n].state  = 0;
		lpObj->VpPlayer2[n].state = 0;
		lpObj->VpPlayer[n].number = -1;
		lpObj->VpPlayer2[n].number = -1;
	}
	lpObj->GuildNumber = 0;
	lpObj->lpGuild     = NULL;
	lpObj->PartyNumber	= -1;
	lpObj->PartyTargetUser = -1;
	lpObj->m_Drink		= 0;
	lpObj->Authority    = 0;
//	lpObj->AuthorityCode= 0;
	lpObj->Penalty      = 0;
	lpObj->m_RecallMon  = -1;
	lpObj->m_Change		= -1;

#ifdef ADD_NEW_GUILD_01_20040913
	lpObj->GuildStatus  = G_NONE;
#endif

#ifdef CHECK_ATTACK_AREA
	lpObj->m_NotAttackAreaCount = 0;
#endif

#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
	lpObj->m_NotAttackAreaCountFor10Min = 0;
#endif // MODIFY_HACKLOG_OF_ATTACK_20050518
	
#ifdef FOR_BLOODCASTLE
	lpObj->LastAttackerID = -1;
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
	lpObj->m_cChaosCastleIndex = -1;
	lpObj->m_cChaosCastleSubIndex = -1;
	lpObj->m_iChaosCastleBlowTime = 0;
	lpObj->m_cKillUserCount = 0;
	lpObj->m_cKillMonsterCount = 0;
#endif

	lpObj->m_InWebzen	= 0;

#ifdef ITEM_INDEX_EXTEND_20050706
	memset(lpObj->CharSet, 0,MAX_PREVIEWCHARSET+9);
#else
#ifdef DARKLORD_WORK
	memset(lpObj->CharSet, 0,MAX_PREVIEWCHARSET+4);
#else
	memset(lpObj->CharSet, 0,MAX_PREVIEWCHARSET+3);
#endif
#endif
	
	for( i=0; i<MAXSELFDEFENSE; i++)
	{
		lpObj->SelfDefense[i]		= -1;
		lpObj->SelfDefenseTime[i]	= 0;
	}
	lpObj->MySelfDefenseTime	= 0;

	lpObj->m_MagicDefense		= 0;

	lpObj->m_SkillAttack2		= 0;
	lpObj->m_SkillAttackTime2	= 0;

	lpObj->LoadWareHouseInfo	= FALSE;

#ifdef NEW_SKILL_FORSKYLAND
	lpObj->m_SkyBossMonSheildLinkIndex		= -1;
	lpObj->m_SkyBossMonSheild				= 0;
	lpObj->m_SkyBossMonSheildTime			= 0;
#endif

#ifdef USER_DIE_MAKE_OPPOSIT_PK_20040329
	lpObj->m_bAttackerKilled				= false;		// ���� �׾��� �� ������ �ϴ� �׾����� ����
#endif


#ifdef GAMESERVER_DIVISION	
	lpObj->m_MoveOtherServer	 = 0;
#endif

	lpObj->m_BossGoldDerconMapNumber = -1;
	
	lpObj->TargetNumber			= 0;
	lpObj->TargetShopNumber		= 0;
	lpObj->WarehouseMoney		= 0;
	lpObj->WarehouseSave	    = FALSE;
	lpObj->WarehouseLock        = 255;
	lpObj->WarehouseCount		= 0;
	lpObj->WarehousePW			= 0;

	lpObj->WarehouseUnfailLock	= 0;

	lpObj->m_bDevilSquareAuth	= FALSE;

	lpObj->m_TimeCount			 = 0;

	lpObj->EventChipCount = 0;
	lpObj->MutoNumber = 0;

	lpObj->HaveWeaponInHand = 0;

#ifdef WAREHOUSE_ITEMCOPY_BUG_FIX	
	lpObj->m_ReqWarehouseOpen = 0;
#endif

#ifdef FOR_STONE_EXCHANGE
	lpObj->iStoneCount = 0;
#endif

#ifdef DUEL_SYSTEM_20031028
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_iDuelUser = -1;
	lpObj->m_btDuelScore = 0;
#endif

#ifdef DUEL_SYSTEM_FIX_20031224
	lpObj->m_iDuelTickCount = 0;
#endif

#ifdef NEW_FORSKYLAND2	
	lpObj->m_MaxLifePower  = 0;

	lpObj->m_PacketChecksumTime = 0;
	lpObj->AddBP	=  0;	
	lpObj->m_CheckLifeTime = 0;
#endif

#ifdef NEW_SKILL_FORSKYLAND
	memset(lpObj->m_Quest, 0xff, MAX_DBQUEST);
	lpObj->m_SendQuestInfo = FALSE;
#endif

#ifdef APPLY_PACKETCHECKSUM
	gPacketCheckSum.ClearCheckSum(lpObj->m_Index);
#endif
	
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	lpObj->NPggCSAuth.Init();
	lpObj->m_bSentGGAuth = false;
	lpObj->m_bSentGGAuthAgain = false;
#else
	#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
		lpObj->NPggCSAuth.Init();
		lpObj->m_NPggCheckSum = 0;	

	#elif defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
		lpObj->NPggCSAuth.Init();
		lpObj->m_bSentGGAuth = false;

	#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

	lpObj->m_NPggCheckSumSendTime = 0;

#ifdef SECOND_QUEST
	lpObj->PlusStatQuestClear = 0;
	lpObj->ComboSkillquestClear = 0;
#endif


	lpObj->m_LastTeleportTime = 0;
	lpObj->m_ClientHackLogCount = 0;

	lpObj->UseEventServer = 0;

#ifdef UPGRADE_GAMESERVER_01_20041006
	// ���ʹ� �ŷ������� â�� �ʿ����.
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		memset(lpObj->pWarehouseMap, 0xFF, MAX_WAREHOUSEITEMS);
		memset(lpObj->TradeMap, 0xFF, MAX_TRADEITEM);
	}
#else
	memset(lpObj->pWarehouseMap, 0xFF, MAX_WAREHOUSEITEMS);
	memset(lpObj->TradeMap, 0xFF, MAX_TRADEITEM);
#endif

	for( i=0; i<MAX_MAGIC; i++) lpObj->Magic[i].Clear();

	lpObj->MagicCount   = 0;
	
	for( i=0; i<MAXMESSAGE; i++)
	{
		gSMMsg[aIndex][i].MsgCode = -1;
	}

#ifdef MONSTER_HERD_SYSTEM_20031120
	lpObj->m_bIsInMonsterHerd = false;
	lpObj->m_bIsMonsterAttackFirst = true;
	lpObj->m_lpMonsterHerd = NULL;
#endif

#ifdef ADD_SKILL_WITH_COMBO	
	if( aIndex-ALLOC_USEROBJECTSTART >= 0 )
	{
		for( i=0; i<MAXATTACKMESSAGE; i++)
		{
			gSMAttackProcMsg[aIndex-ALLOC_USEROBJECTSTART][i].MsgCode = -1;
		}
	}

	lpObj->SkillHellFire2State = FALSE;
	lpObj->SkillHellFire2Count = 0;
	
	lpObj->m_PoisonType	= 0;
	lpObj->m_IceType	= 0;
#endif	

#ifdef DARKLORD_WORK		
	lpObj->Leadership = 0;
	lpObj->AddLeadership = 0;
	lpObj->ChatLitmitTime = 0;
	lpObj->ChatLimitTimeSec = 0;

	lpObj->SkillRecallParty_Time = 0;
	lpObj->SkillLongSpearChange = FALSE;
	lpObj->SkillDelay.Init();
#endif

#ifdef HAPPY_POUCH	// apple��20050203
	lpObj->m_btLifeInc	= 0;
	lpObj->m_lLifeIncTime	= 0;
	lpObj->m_dwLastCheckTick = 0;
	lpObj->m_lFreeKalimaTime = 0;
#endif

#ifdef BLOCK_INVALID_CHARACTER_DELETE_01_20060327
	lpObj->bEnableDelCharacter = TRUE;
#endif
	
#ifdef UPGRADE_GAMESERVER_01_20041006
	// ���ʹ� â�� �ʿ� ����.
	if( lpObj->Type == OBJTYPE_CHARACTER )
#endif
	for( i=0; i<MAX_WAREHOUSEITEMS; i++)	lpObj->pWarehouse[i].Clear();

#ifdef CHECK_PENETRATION_SKILL	
	gMultiAttackHackCheck[aIndex].Init();
#endif
	
	lpObj->pTransaction = FALSE;
	gObjMonsterHitDamageInit(lpObj);
	gObjSetInventory1Pointer(&gObj[aIndex]);
#ifdef PERSONAL_SHOP_20040113
	
#ifdef UPGRADE_GAMESERVER_01_20041006
	// ���ʹ� �κ��丮�� ������ �ʿ� ����.
	if( lpObj->Type == OBJTYPE_CHARACTER )
#endif
	#ifdef PERSONAL_SHOP_BUG_FIX
		memset(lpObj->pInventoryMap, 0xFF, MAX_INVENTORYMAP + MAX_PSHOPITEM);
	#else
		memset(lpObj->pInventoryMap, 0xFF, MAX_INVENTORY + MAX_PSHOPITEM);
	#endif

	lpObj->m_bPShopOpen = false;											// ���λ����� �����ߴ����� ����
	lpObj->m_bPShopTransaction = false;										// ���λ����� Ʈ�����
	lpObj->m_bPShopItemChange = false;										// ���λ����� ��ǰ�� �ȷ��� ��� �������� ����� ��� (�Ĵ� ��� ���忡��)
	lpObj->m_bPShopRedrawAbs = false;										// ���λ����� ����Ʈ�� ������ �ٽ� �׸���� �÷���
	memset(lpObj->m_szPShopText, 0, MAX_PSHOP_TEXT);						// ���λ����� �̸�
	
	lpObj->m_bPShopWantDeal = false;										// �ٸ� ����� ���λ��� â�� ���� �ִ°� ����
	lpObj->m_iPShopDealerIndex = -1;										// ���� ���� �ִ� ���� ������ �ε���
	memset(lpObj->m_szPShopDealerName, 0x00, MAX_IDSTRING);					// ���� ���� �ִ� ���� ������ �ɸ���

	memset(lpObj->m_iVpPShopPlayer, 0, sizeof(lpObj->m_iVpPShopPlayer));	// ���λ��� ����Ʈ �ʱ�ȭ
	lpObj->m_wVpPShopPlayerCount = 0;										// ���λ��� 
#else
	memset(lpObj->pInventoryMap, 0xFF, 8*8);
#endif

#ifdef ADD_NEW_UNION_01_20041006
	lpObj->iGuildUnionTimeStamp = 0;
#endif

	gObjCharTradeClear(lpObj);

#ifdef UPGRADE_GAMESERVER_01_20041006
	// ���ʹ� â��, �κ��丮, �ŷ� ������ �ʿ� ����. 
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
#endif //UPGRADE_GAMESERVER_01_20041006

	for( i=0; i<MAX_WAREHOUSEITEMS; i++)	lpObj->pWarehouse[i].Clear();	
#ifdef PERSONAL_SHOP_20040113
	for( i=0; i<MAX_INVENTORY_EXTEND; i++ )	lpObj->Inventory1[i].Clear();
	for( i=0; i<MAX_INVENTORY_EXTEND; i++ )	lpObj->Inventory2[i].Clear();
#else
	for( i=0; i<MAX_INVENTORY;		i++ )	lpObj->Inventory1[i].Clear();
	for( i=0; i<MAX_INVENTORY;		i++ )	lpObj->Inventory2[i].Clear();
#endif	
	for( i=0; i<MAX_TRADEITEM;	    i++)    lpObj->Trade[i].Clear();
	//for( i=0; i<MAX_CHAOSBOXSIZE;	i++ )	lpObj->pChaosBox[i].Clear();

#ifdef UPGRADE_GAMESERVER_01_20041006
	}
#endif //UPGRADE_GAMESERVER_01_20041006


#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
	lpObj->bIsChaosMixCompleted			= false;	// ī���� ���� �߿� ������ �Ϸ�Ǿ����� (1) / �̿Ϸ� ���� (0) �� ���°�
#endif
	
#ifdef MAP_SERVER_WORK_20041030						// �ʼ��� �̵��� �ʿ��� ����� ������
	lpObj->m_bMapSvrMoveQuit			= false;
#endif
#ifdef	CASTLE_MAIN_SCHEDULER_20041111
	// ������ NPC�鿡 �ʿ��� ������
	lpObj->m_iCsNpcExistVal				= 0;
	lpObj->m_btCsNpcType				= 0;		// CS_NPC_TYPE_NONE
	lpObj->m_btCsGateOpen				= 0;		// CS_GATE_STATE_CLOSED
	lpObj->m_iCsGateLeverLinkIndex		= -1;
	
	lpObj->m_btCsNpcDfLevel				= 0;
	lpObj->m_btCsNpcRgLevel				= 0;
	// ������ �������� �ʿ��� ������
	lpObj->m_btCsJoinSide				= 0;		// CS_SIEGESIDE_NONE
	lpObj->m_bCsGuildInvolved			= 0;
#endif

#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
	lpObj->m_bIsCastleNPCUpgradeCompleted	= false;	// �������� NPC�� ���׷��̵� �ߴ���.
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
	// ������ ���� ����
	lpObj->m_btWeaponState				= WEAPON_STATE_NORMAL;
#endif
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// ��ų�� ����ϱ� ���� �ʿ��� ���� ī����
	lpObj->m_btKillCount				= 0;
#endif
#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	lpObj->m_iAccumulatedDamage			= 0;		// ���� ������
#endif
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
	lpObj->m_iCreatedActivationTime		= 0;		// ������Ʈ ���� �� Ȱ��ȭ �ð�
	lpObj->m_btLifeStoneCount			= 0;		// ���������� ���� �� �ִ� Ƚ��
#endif
	
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
	lpObj->m_iAccumulatedCrownAccessTime = 0;
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
	lpObj->m_MonsterSkillElementInfo.Reset();		// ���� ��ų������Ʈ�� ȿ�� ���� �� ����
#endif
	
#ifdef ADD_ANTI_HACK_01_20051027
	// PACKET_ATTACKET ��Ŷ�� ���� �ð� üũ
	lpObj->m_dwLastPacketAttackTime = 0;
#endif

#ifdef CRYWOLF_MVP_REWARD_RANK_20051215
	lpObj->m_iCrywolfMVPScore = 0;
#endif
	
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
	lpObj->m_iSkillDistanceErrorCount = 0;
#endif	//MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
	
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421
	lpObj->m_dwSkillDistanceErrorTick = 0;
#endif // MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	g_kJewelOfHarmonySystem.InitEffectValue( &lpObj->m_JewelOfHarmonyEffect );		// ��ȭ�� ������ ���� ��ȭ ������ �ɼ� ���� �ʱ�ȭ
#endif	// ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
	g_kItemSystemFor380.InitEffectValue( &lpObj->m_ItemOptionExFor380 );			// 380 ������ �ɼ� ���� �ʱ�ȭ
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	g_SocketOptionSystem.ClearUserData( lpObj );
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef ADD_NEW_NPC_KANTURU_20060612
	lpObj->m_bKanturuEntranceByNPC		= 0;
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ����� ���� �ε��� �߰�
	lpObj->m_iIllusionTempleIndex	= -1;
#endif

#ifdef MODIFY_QUEST_SYSTEM_20070525	// 3�� ü������ ���� ���� �ʱ�ȭ
	lpObj->m_bQuestMonKillDBLoad	= FALSE;
	lpObj->m_iQuestMonKillDBIndex	= -1;		// �⺻���� -1 (0�� ������ ����Ʈ ����)
	
	for( i = 0; i < MAX_QUEST_MONSTER_KILL_INFO; i++ )
	{
		lpObj->m_QuestMonKillInfo[i].iQuestMonIndex	= -1;
		lpObj->m_QuestMonKillInfo[i].iKillCount		= -1;
	}
#endif

#ifdef MODIFY_0708_BUFFIX_20070820
	lpObj->m_bSkillKeyRecv			= FALSE;
#endif
	
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// �����ͷ��� ��ų ��ġ �ʱ�ȭ
	g_MasterSkillSystem.InitPassiveSkillData( lpObj );
#endif
	
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	lpObj->m_btRefillHPSocketOption = 0;
	lpObj->m_btRefillMPSocketOption = 0;
	lpObj->m_wSocketOptionMonsterDieGetHp		= 0;
	lpObj->m_wSocketOptionMonsterDieGetMana		= 0;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	lpObj->m_btAGReduceRate = 0;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	lpObj->m_dwMapMoveKeyValue = 0;
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#ifdef GMCOMMAND_EX
	lpObj->m_bGMSummon = FALSE;
#endif // GMCOMMAND_EX

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
	for( i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		lpObj->m_dwUseHacktoolList[i] = KEY_VALUE_NOT_USE;
	}
#endif // ADD_HACKING_TOOL_BLOCK_20090311

	lpObj->m_AccountType = 0;
	lpObj->m_EndTime = 0;
	lpObj->m_bVIPReload = false;

	lpObj->m_bResetState = false;
	lpObj->m_iResetNumber = -1;
	lpObj->m_bReqUpdateReset = false;

	lpObj->m_bShell = false;
	lpObj->m_stateEngagePVP = PVP_USER_NONE;
	lpObj->m_bBuffHelper = false;

	//DevGamesX 01/05/2018 - OffExp
	lpObj->DbOffExp = 0;
	lpObj->OffExp = false;
	lpObj->OffExpTick = 0;
	lpObj->OffExpSpeed = 0;

	lpObj->OffAfk = false;
	lpObj->Offtrade = false;

	lpObj->OffTradeZen = 0;
	lpObj->OffTradeCredits = 0;
	lpObj->BotStore = 0;

	//lpObj-> PickUpZen = 0;

	#ifdef PICKUP_SYSTEM
	///pick up system
	for (i = 0; i < MAX_PICKUP_ITEMS; i++)
	{
		lpObj->m_pickup[i].Item = -1;
		lpObj->m_pickup[i].Type = -1;
		lpObj->m_pickup[i].Index = -1;
	}
#endif

	gObjClearViewport(&gObj[aIndex]);
}

int gObjGetSocket(SOCKET socket)
{
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected )
		{
			if( gObj[n].m_socket == socket )
			{
				return n;
			}
		}
	}
	return -1;
}

void gObjSetTradeOption(int aIndex, int option)
{
	if( gObjIsConnected(aIndex) == TRUE ) 
	{
		if( option == 0 ) 
		{
			gObj[aIndex].m_Option &= !OPTION_TRADE;
			GCServerMsgStringSend(lMsg.Get(1121), aIndex, 1);
		}
		else {
			gObj[aIndex].m_Option |= OPTION_TRADE;
			GCServerMsgStringSend(lMsg.Get(1120), aIndex, 1);
		}		
	}
}


#ifdef DUEL_SYSTEM_20031028

void gObjSetDuelOption(int aIndex, int option)
{
	if( gObjIsConnected(aIndex) == TRUE ) 
	{
		if( option == 0 ) 
		{
			gObj[aIndex].m_Option &= !OPTION_DUEL;
//			GCServerMsgStringSend(lMsg.Get(1121), aIndex, 1);
		}
		else {
			gObj[aIndex].m_Option |= OPTION_DUEL;
//			GCServerMsgStringSend(lMsg.Get(1120), aIndex, 1);
		}		
	}
}

bool IsDuelEnable (int aIndex)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return false;

	if( gObjIsConnected(aIndex) == TRUE ) 
	{
		if( (gObj[aIndex].m_Option&OPTION_DUEL) == OPTION_DUEL ) {
			return true;
		}
	}

	return false;
}

bool IsOnDuel (int aIndex1, int aIndex2)
{
	if (!CHECK_LIMIT(aIndex1, MAX_OBJECT) || !CHECK_LIMIT(aIndex2, MAX_OBJECT))
		return false;

	if (gObj[aIndex1].Type != OBJTYPE_CHARACTER ||
		gObj[aIndex2].Type != OBJTYPE_CHARACTER
		)
		return false;

	if (gObj[aIndex1].m_iDuelUser == aIndex2 &&
		gObj[aIndex2].m_iDuelUser == aIndex1
		)
		return true;

	return false;
}

#endif

// ���̵� ���� ������ ��´�.
int GetMapMoveLevel(LPOBJECTSTRUCT lpObj, int mapnumber, int max_over)
{
	int overLevel=0;

#if ENABLE_MC_CODE == 1
	if (mapnumber > g_TerrainManager.Size() - 1)
#else // ENABLE_MC_CODE == 1
	if (mapnumber > MAX_MAP - 1)
#endif // ENABLE_MC_CODE == 1
	{
		mapnumber = 0;
		LogAdd("error-L3 : map number not %s %d", __FILE__, __LINE__);
		return 0;
	}

	if( max_over )
	{
		overLevel = max_over;
	}
	else 
	{
#if ENABLE_MC_CODE == 1
		max_over = g_TerrainManager.GetMinLevel(mapnumber);
		overLevel = g_TerrainManager.GetMinLevel(mapnumber);
#else // ENABLE_MC_CODE == 1
		max_over = gMoveCommand.GetMoveLevel(mapnumber);
		overLevel = gMoveCommand.GetMoveLevel(mapnumber);
#endif // ENABLE_MC_CODE == 1
	}
	
	// ���˻��϶��� 2/3 �� ���δ�.
#ifdef DARKLORD_WORK
	if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )	
#else
	if( lpObj->Class == CLASS_MAGUMSA )	
#endif
	{		
		if( overLevel > 0 )
		{
			if( max_over > 0 )
			{
				overLevel = ((max_over/3)*2);
			}
		}
	}

	return overLevel;
}


// DB Item ������ ĳ���Ϳ� �����Ѵ�. (DB Version :  00)
#ifdef ITEMDELETE_MODIFY_20040604
void DbItemSetInByte(LPOBJECTSTRUCT lpObj, LPSDHP_DBCHAR_INFORESULT lpMsg, int ItemDbByte, bool * bAllItemExist = NULL)
#else
void DbItemSetInByte(LPOBJECTSTRUCT lpObj, LPSDHP_DBCHAR_INFORESULT lpMsg, int ItemDbByte)
#endif
{
	int		n;
	int		itype, _type;
	CItem	item;
	BYTE	OptionData;
	WORD	hiWord, loWord;

#ifdef ITEMDELETE_MODIFY_20040604
	bool	bIsItemExist = true;
#endif
	
	// �������� �����Ѵ�.
#ifdef PERSONAL_SHOP_20040113
	for( n=0; n<MAX_INVENTORY_EXTEND; n++)
#else
	for( n=0; n<MAX_INVENTORY; n++)
#endif
	{
#ifdef ITEMDELETE_MODIFY_20040604
		bIsItemExist = true;
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
		lpObj->pInventory[n].Clear();
#endif

		itype = lpMsg->dbInventory[n*ItemDbByte];
#ifdef ITEM_INDEX_EXTEND_20050706
		if( ItemDbByte >= MAX_ITEMDBBYTE ) {
			if( (lpMsg->dbInventory[n*ItemDbByte] == 0xFF)
				&& (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x80) == 0x80
				&& (lpMsg->dbInventory[(n*ItemDbByte)+9]&0xF0) == 0xF0
				) 
			{
				// ������ �� �������� ������ ��� ��Ʈ�� 1�� set �ϹǷ� �̷� �ĺ��� ����
				continue;
			}
		}
		else if (ItemDbByte > MAX_ITEMDBBYTE_V00) {
			if( (lpMsg->dbInventory[n*ItemDbByte] == 0xFF) && (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x80) == 0x80) 
			{
				continue;
			}
		}
		else {
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF) 
			{
				continue;
			}
		}
#else
#ifdef ITEMDELETE_MODIFY_20040604		// 0xFF - ��ĭ
#ifdef INVENTORY_EMPTYSLOT_CHECK_20050217
		if( (lpMsg->dbInventory[n*ItemDbByte] == 0xFF) && (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x80) == 0x80) {
#else
		if (itype == 0xFF) {
#endif
			continue;
		}
#endif
#endif
		
#ifdef ITEM_DBSIZE_EXTEND_20050706
		if( ItemDbByte == MAX_ITEMDBBYTE ) {
#ifdef ITEM_INDEX_EXTEND_20050706
//			itype |= (BYTE)lpMsg->dbInventory[n*ItemDbByte];
			itype |= (lpMsg->dbInventory[(n*ItemDbByte)+9]&0xF0)<<5;
			itype |= (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x80)<<1;
			_type = itype;
#endif			
		}
		else if (ItemDbByte == MAX_ITEMDBBYTE_V02)
#else
		if( ItemDbByte == 10 )
#endif
		{
			if( (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x80) == 0x80 ) itype += 256;
#ifdef ITEM_INDEX_EXTEND_20050706
			_type = ((itype/32)*MAX_ITEM_INDEX)+(itype%32);
#else
			_type = itype;
#endif
		}
		else
		{
			_type = ((itype/16)*MAX_ITEM_INDEX)+(itype%16);
		}

#ifndef ITEM_INDEX_EXTEND_20050706
		lpObj->pInventory[n].Clear();
#endif
		
#ifdef ITEM_INDEX_EXTEND_20050706
		if( ItemDbByte >= MAX_ITEMDBBYTE ) {
			if( (lpMsg->dbInventory[n*ItemDbByte] == 0xFF)
				&& (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x80) == 0x80
				&& (lpMsg->dbInventory[(n*ItemDbByte)+9]&0xF0) == 0xF0
				) 
			{
				// ������ �� �������� ������ ��� ��Ʈ�� 1�� set �ϹǷ� �̷� �ĺ��� ����
				itype = -1;
			}
		}
		else if (ItemDbByte >= MAX_ITEMDBBYTE_V02) {
			if( (lpMsg->dbInventory[n*ItemDbByte] == 0xFF) && (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x80) == 0x80) 
			{
				itype = -1;
			}
		}
		else {
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF) 
			{
				itype = -1;
			}
		}
#else
		if( (lpMsg->dbInventory[n*ItemDbByte] == 0xFF) && (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x80) == 0x80) 
		{
			itype = -1;
		}
#endif

		if( !IsItem(_type) )
		{
#ifdef ITEMDELETE_MODIFY_20040604		// �������� �ƴ� ��쿡�� �������� �ʴ� �������̶�� ǥ�ø� �Ѵ�.
			bIsItemExist		= false;
			if(bAllItemExist)	(*bAllItemExist) = false;
#else
			itype = -1;
#endif
		}		

		if( _type == MAKE_ITEMNUM(14,17) || _type == MAKE_ITEMNUM(14,18) )
		{	// �Ǹ��� ����, ���� ��쿡�� �������� 0�ΰ� �� �����Ѵ�			
			if( lpMsg->dbInventory[(n*ItemDbByte)+2] == 0 )
			{
				itype = -1;
			}
		}

		if( _type == MAKE_ITEMNUM(14,19) )
		{	// �Ǹ��� �ʴ���� ��쿡�� �������� 255�ΰ� �� �����Ѵ�			
			if( lpMsg->dbInventory[(n*ItemDbByte)+2] == 255 )
			{
				itype = -1;
			}
		}

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
		// ����� ������ ��� ������ �������� 0�̸� �����Ѵ�
		if( _type == MAKE_ITEMNUM(14, 100) )
		{
			if( lpMsg->dbInventory[(n*ItemDbByte)+2] == 0 )
			{
				itype = -1;
			}
		}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifndef FOR_BLOODCASTLE_TEST
/*		if ( 
			_type == MAKE_ITEMNUM(13, 16) ||	// ��õ���Ǽ�
			_type == MAKE_ITEMNUM(13, 17)		// ���庻
//			_type == MAKE_ITEMNUM(13, 18)		// �������
			)	
		{
			if( lpMsg->dbInventory[(n*ItemDbByte)+2] != 128 )
			{
				itype = -1;
			}
		}

		if (_type == MAKE_ITEMNUM(14, 21))
		{
			int _level = ((lpMsg->dbInventory[(n*ItemDbByte)+1]>>3)&0xF);
			if (_level == 1) {	// ������
				if( lpMsg->dbInventory[(n*ItemDbByte)+2] != 128 )
				{
					itype = -1;
				}
			}
		}
*/
#endif
		
		if( itype != -1 )
		{
			item.m_Level = ((lpMsg->dbInventory[(n*ItemDbByte)+1]>>3)&0xF);

#ifdef SETITEM_BUFFER_CLEAR
			if( lpMsg->DbVersion < 2 )
			{	// �κ��� ��Ʈ������ Ŭ���� �Ǿ� ���� ������ ��Ʈ������ ������ Ŭ���� �Ѵ�
				lpMsg->dbInventory[(n*ItemDbByte)+8] = 0xFF;
				lpMsg->dbInventory[(n*ItemDbByte)+9] = 0xFF;
			}
#endif

#ifdef TEST_SETITEM_CLEAR			
			if( !(lpObj->ClearInfoForSetItem&0x01) )
			{	// �κ��� ��Ʈ������ Ŭ���� �Ǿ� ���� ������ ��Ʈ������ ������ Ŭ���� �Ѵ�
				lpMsg->dbInventory[(n*ItemDbByte)+8] = 0xFF;
				lpMsg->dbInventory[(n*ItemDbByte)+9] = 0xFF;
			}
#endif
			
			// Ȥ�� ������ �ִ� �������� ũ�ٸ�.. 0���� �����.			
			if( _type != MAKE_ITEMNUM(14,11) )	// ����� ���ڴ� ���� üũ ���Ѵ�
				if( item.m_Level > MAX_ITEMLEVEL ) item.m_Level = 0;
			
			OptionData     = lpMsg->dbInventory[(n*ItemDbByte)+1];
			item.m_Option1 = (OptionData>>7)&0x01;
			item.m_Option2 = (OptionData>>2)&0x01;
			item.m_Option3 = OptionData&0x3;

#ifdef ITEM_DBSIZE_EXTEND_20050706
			if( ItemDbByte >= 10 )
#else		
			if( ItemDbByte == 10 )
#endif
			{
				if( _type == MAKE_ITEMNUM(13,3) )
				{	// ����Ʈ�� �ɼ� 3Bit�� ��δ� ����Ѵ�
					item.m_Option3 |= (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x40)>>4;
				}
				else
				{
					if( (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x40) == 0x40 )
						item.m_Option3 += 4;
				}
			}

			item.m_Durability = lpMsg->dbInventory[(n*ItemDbByte)+2];
			
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#ifdef MODIFY_JEWELOFHARMONY_380OPTION_DBVERSION_BUGFIX_20060727 // ITEM_DBSIZE_EXTEND_20050706 define �ݵ�� On
			if ( ItemDbByte >= MAX_ITEMDBBYTE )
			{
				item.m_JewelOfHarmonyOption = lpMsg->dbInventory[(n*ItemDbByte)+10];	// 10��° ���� : ��ȭ�Ǻ��� ��ȭ �ɼ�
			}
	#else
			item.m_JewelOfHarmonyOption = lpMsg->dbInventory[(n*ItemDbByte)+10];	// 10��° ���� : ��ȭ�Ǻ��� ��ȭ �ɼ�
	#endif // MODIFY_JEWELOFHARMONY_380OPTION_DBVERSION_BUGFIX_20060727
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
	#ifdef MODIFY_JEWELOFHARMONY_380OPTION_DBVERSION_BUGFIX_20060727 // ITEM_DBSIZE_EXTEND_20050706 define �ݵ�� On
			if ( ItemDbByte >= MAX_ITEMDBBYTE )
			{
				item.m_ItemOptionEx = (lpMsg->dbInventory[(n*ItemDbByte)+9]&0x08)<<4;	// 9��° ����. ���� 5��° 1 ��Ʈ���� m_ItemOptionEx�� ���� 1��Ʈ�� ����
				// 2006.8.2 �߰�
				if ( item.m_ItemOptionEx != 0 )
				{
					// 380 �������� �ƴ� ��� �ɼ� ����
					item.m_Type = itype;	// DBVersion3�̻��̱� ������ itype���� �״�� ����
					if ( g_kItemSystemFor380.Is380Item( &item ) == FALSE )
					{
						item.m_ItemOptionEx = 0;
						LogAddTD("[380Item][%s][%s] Invalid 380 Item Option in Inventory pos[%d]", lpObj->AccountID, lpObj->Name, n);
					}
				}
			}
	#else
			item.m_ItemOptionEx = (lpMsg->dbInventory[(n*ItemDbByte)+9]&0x08)<<4;	// 9��° ����. ���� 5��° 1 ��Ʈ���� m_ItemOptionEx�� ���� 1��Ʈ�� ����
	#endif // MODIFY_JEWELOFHARMONY_380OPTION_DBVERSION_BUGFIX_20060727
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			BYTE SocketOption[MAX_SOCKET_SLOT];
			BYTE SocketBonusOption = SOCKETSLOT_NONE;

			memset( SocketOption, SOCKETSLOT_NONE, MAX_SOCKET_SLOT );

			if( g_SocketOptionSystem.IsEnableSocketItem( _type ) == true )
			{
				for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
				{
					SocketOption[i] = lpMsg->dbInventory[(n*ItemDbByte)+11+i];
				}

				SocketBonusOption = lpMsg->dbInventory[(n*ItemDbByte)+10];
			}
			else
			{
				for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
				{
					SocketOption[i] = SOCKETSLOT_NONE;
				}
				
				SocketBonusOption = 0;
			}
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422


			if( ItemDbByte == MAX_ITEMDBBYTE_V00 )
			#ifdef ADD_380ITEM_NEWOPTION_20060711
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, 0, 0, 0, 0);
			#else
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, 0, 0, 0);
			#endif // ADD_380ITEM_NEWOPTION_20060711
#ifdef ITEM_INDEX_EXTEND_20050706
			else if(ItemDbByte <= MAX_ITEMDBBYTE_V02) 
			{
		#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x3F), lpMsg->dbInventory[(n*ItemDbByte)+8], item.m_ItemOptionEx, SocketOption, SocketBonusOption, 1);
			#else
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x3F), lpMsg->dbInventory[(n*ItemDbByte)+8], item.m_ItemOptionEx, NULL, SOCKETSLOT_NONE, 1);
			#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
		#else
			#ifdef ADD_380ITEM_NEWOPTION_20060711
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x3F), lpMsg->dbInventory[(n*ItemDbByte)+8], item.m_ItemOptionEx, 1);
			#else
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x3F), lpMsg->dbInventory[(n*ItemDbByte)+8], 1);
			#endif // ADD_380ITEM_NEWOPTION_20060711
		#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			}
			else 
			{
		#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x3F), lpMsg->dbInventory[(n*ItemDbByte)+8], item.m_ItemOptionEx, SocketOption, SocketBonusOption, 3);
		#else
			#ifdef ADD_380ITEM_NEWOPTION_20060711
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x3F), lpMsg->dbInventory[(n*ItemDbByte)+8], item.m_ItemOptionEx, 3);
			#else
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x3F), lpMsg->dbInventory[(n*ItemDbByte)+8], 3);
			#endif // ADD_380ITEM_NEWOPTION_20060711
		#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			}
#else
			else
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, (lpMsg->dbInventory[(n*ItemDbByte)+7]&0x3F), lpMsg->dbInventory[(n*ItemDbByte)+8], 1);

#endif

			// ������ ��� ������ 0�ϰ�� 1��..
#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
			if( _type == ITEM_POTION + 7 )
			{
				// �������� ����
			}
			else
#endif
			if( _type >= ITEM_POTION && _type <= ITEM_POTION+8
#ifdef ADD_SHIELD_POINT_01_20060403
				|| ( _type >= ITEM_POTION+38 && _type <= ITEM_POTION+40 ) // ���� ����
#endif
				) 
			{
				if( item.m_Durability == 0 ) item.m_Durability = 1;
				if( item.m_Durability > g_GlobalConfig.m_iMaxPotionDurability  ) item.m_Durability = g_GlobalConfig.m_iMaxPotionDurability;
			}

#ifdef ADD_FULL_RECOVERY_POTION_20070719
			if( _type == MAKE_ITEMNUM( 14, 70 )			// ����Ʈ ġ�� ����
				|| _type == MAKE_ITEMNUM( 14, 71 )		// ����Ʈ ġ�� ����
			  )
			{
				if( item.m_Durability == 0 )
				{
					item.m_Durability = 1;
				}
				if( item.m_Durability > MAX_OVERLAP_ELITE_POTION )
				{
					item.m_Durability =  MAX_OVERLAP_ELITE_POTION;
				}
			}
#endif // ADD_FULL_RECOVERY_POTION_20070719

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
			if( _type == MAKE_ITEMNUM( 14, 94 )			// �߰� ����Ʈ ġ�� ����
				)
			{
				if( item.m_Durability == 0 )
				{
					item.m_Durability = 1;
				}
				if( item.m_Durability > MAX_OVERLAP_ELITE_POTION )
				{
					item.m_Durability =  MAX_OVERLAP_ELITE_POTION;
				}
			}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
			/*
#ifdef ADD_ELITE_SD_POTION_20090918
			if( _type == MAKE_ITEMNUM( 14, 133 )		// ����Ʈ SD ȸ�� ����
				)
			{
				if( item.m_Durability == 0 )
				{
					item.m_Durability = 1;
				}
				if( item.m_Durability > 255 ) // MAX_OVERLAP_ELITE_POTION
				{
					item.m_Durability =  255;
				}
			}
#endif // ADD_ELITE_SD_POTION_20090918
			*/
#ifdef ADD_PCS_IMPROVE_POTION_20070718
			if( _type == MAKE_ITEMNUM( 14, 78 )
				|| _type == MAKE_ITEMNUM( 14, 79 )
				|| _type == MAKE_ITEMNUM( 14, 80 )
				|| _type == MAKE_ITEMNUM( 14, 81 )
				|| _type == MAKE_ITEMNUM( 14, 82 )
			  )
			{
				if( item.m_Durability == 0 )
				{
					item.m_Durability = 1;
				}
				if( item.m_Durability > MAX_OVERLAP_IMPROVE_POTION )
				{
					item.m_Durability = MAX_OVERLAP_IMPROVE_POTION;
				}
			}
#endif // ADD_PCS_IMPROVE_POTION_20070718

#ifdef ITEM_ADD_OPTION_20061019
			else if( _type >= ITEM_POTION+46 && _type <= ITEM_POTION+50 )
			{
				if( item.m_Durability == 0 )
				{
					item.m_Durability = 1;
				}
				if( item.m_Durability > g_GlobalConfig.m_iMaxPotionDurability )
				{
					item.m_Durability = g_GlobalConfig.m_iMaxPotionDurability;
				}
			}
#endif 

#ifdef ADD_SHIELD_POINT_01_20060403
			else if ( _type >= ITEM_POTION+35 && _type <= ITEM_POTION+37 ) // ����SDȸ������ ~ ū SDȸ�� ����
			{
				// �ִ� 1��
				if( item.m_Durability == 0 ) 
					item.m_Durability = 1;
				if( item.m_Durability > 1  ) 
					item.m_Durability = 1;
			}
#endif // ADD_SHIELD_POINT_01_20060403
			/*
			else if( _type != MAKE_ITEMNUM(13,10)
#ifdef ADD_NEW_MAP_KALIMA_20040518
				 && _type != MAKE_ITEMNUM(14,29)
#endif
#ifdef MU_CASTLESIEGE_CS_MARKS_20041214				// ������ ǥ���� �������� �ʱⰪ�� ���� �ʱ�ȭ�Ǹ� �ʵȴ�.
				 && (_type != MAKE_ITEMNUM(14,21) && item.m_Level == 3)
#endif
#ifdef UPDATE_LUCKY_COIN_EVENT_20081029				// ����� ������ �������� �ʱⰪ�� ���� �ʱ�ȭ�Ǹ� �ʵȴ�.
				 && _type != MAKE_ITEMNUM(14, 100)
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029
				)
			{
				if( item.m_Durability > item.m_BaseDurability 
#ifdef ITEMDELETE_MODIFY_20040604					// �����ϴ� �������� ��츸 �������� ���� ��Ų��.
					&& bIsItemExist == true
#endif
					)
				{
					item.m_Durability = item.m_BaseDurability;
				}
			}*/
			//------------->
			//20080930 �ؿ� 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
			#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
			else if( _type == MAKE_ITEMNUM(13, 70) )
			{
				if( item.m_Durability == 1 )
					item.m_Durability = item.m_BaseDurability;
			}
			#endif //ADD_TELEPORT_CHARM_ITEM_20080930
			//<-------------

#ifndef FOR_BLOODCASTLE_TEST
/*			if (_type == MAKE_ITEMNUM(14,21)) {		// ������ ��쿡�� �������� 128�� ���������Ѵ� -> ������ġ�� �Ŀ� (�׼��͵��� �����ֱ� ����)
				if (item.m_Level == 1)		
					item.m_Durability = 128;				
			}
*/
#endif			

			/*
			// ���� ������ ���� 255 ���� (�ӽ�)
			if( itype >= MAKE_ITEMNUM(12,0) && itype <= MAKE_ITEMNUM(12,2) ) 
			{
				item.m_Durability = 255;
			}
			if( ItemGetDurability(itype) )
			{
				item.m_Durability = ItemGetDurability(itype, item.m_Level, item.IsExtItem());
			}
			*/
			

			lpObj->pInventory[n].m_Option1 = item.m_Option1;
			lpObj->pInventory[n].m_Option2 = item.m_Option2;
			lpObj->pInventory[n].m_Option3 = item.m_Option3;

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			lpObj->pInventory[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
#endif //	ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
			lpObj->pInventory[n].m_ItemOptionEx = item.m_ItemOptionEx;
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			lpObj->pInventory[n].m_BonusSocketOption = item.m_BonusSocketOption;
			for( int iSlotIndex = 0 ; iSlotIndex < MAX_SOCKET_SLOT ; iSlotIndex++ )
			{
				lpObj->pInventory[n].m_SocketOption[iSlotIndex] = item.m_SocketOption[iSlotIndex];
			}
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

			hiWord = MAKEWORD(lpMsg->dbInventory[(n*ItemDbByte)+4], lpMsg->dbInventory[(n*ItemDbByte)+3]);
			loWord = MAKEWORD(lpMsg->dbInventory[(n*ItemDbByte)+6], lpMsg->dbInventory[(n*ItemDbByte)+5]);

			item.m_Number		= MAKELONG(loWord, hiWord);

			gObjInventoryInsertItemPos(lpObj->m_Index, item, n, 0);

#ifdef ITEMDELETE_MODIFY_20040604
			if (bIsItemExist == false) 
			{
				lpObj->pInventory[n].m_bItemExist = false;

				LogAddTD("error-L2 : Unknown Item found [%s][%s] (Type:%d, LEV:%d, DUR:%d OP1:%d, OP2:%d, OP3:%d, NEWOP:%d, SET:%d)",
					lpObj->AccountID,
					lpObj->Name,
					lpObj->pInventory[n].m_Type,
					lpObj->pInventory[n].m_Level,
					lpObj->pInventory[n].m_Durability,
					lpObj->pInventory[n].m_Option1,
					lpObj->pInventory[n].m_Option2,
					lpObj->pInventory[n].m_Option3,
					lpObj->pInventory[n].m_NewOption,
					lpObj->pInventory[n].m_SetOption
					);
			}
#endif
		}
	}

#ifdef TEST_SETITEM_CLEAR	// ��Ʈ �����ۿ� ������ Ŭ���� �ߴ�
	lpObj->ClearInfoForSetItem |= 0x01;
#endif	

#ifdef DARKLORD_WORK
	gObjRequestPetItemInfo(lpObj->m_Index, 0);
#endif	
}

#ifdef NEW_FORSKYLAND2
// AG�Ŀ� ���� �����Ѵ�
void	gObjSetBP(int aIndex)
{
	int	Strength	= gObj[aIndex].Strength+gObj[aIndex].AddStrength;
	int	Dexterity	= gObj[aIndex].Dexterity+gObj[aIndex].AddDexterity;
	int	Vitality	= gObj[aIndex].Vitality+gObj[aIndex].AddVitality;
	int	Energy		= gObj[aIndex].Energy+gObj[aIndex].AddEnergy;
#ifdef DARKLORD_WORK	
	int Leadership	= gObj[aIndex].Leadership+gObj[aIndex].AddLeadership;
#endif

	switch(gObj[aIndex].Class)
	{
		case CLASS_WIZARD:
			gObj[aIndex].MaxBP = Strength*0.2+Dexterity*0.4+Vitality*0.3+Energy*0.2;
		break;

		case CLASS_KNIGHT:
			gObj[aIndex].MaxBP = Strength*0.15+Dexterity*0.2+Vitality*0.3+Energy*1.0;
		break;

		case CLASS_ELF:
			gObj[aIndex].MaxBP = Strength*0.3+Dexterity*0.2+Vitality*0.3+Energy*0.2;
		break;

		case CLASS_MAGUMSA:
			gObj[aIndex].MaxBP = Strength*0.2+Dexterity*0.25+Vitality*0.3+Energy*0.15;
		break;

#ifdef DARKLORD_WORK
		case CLASS_DARKLORD:
			gObj[aIndex].MaxBP = Strength*0.3+Dexterity*0.2+Vitality*0.1+Energy*0.15+Leadership*0.3;
		break;	
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� AG�� ����
		case CLASS_SUMMONER:
			gObj[aIndex].MaxBP = Strength*0.2+Dexterity*0.25+Vitality*0.3+Energy*0.15;
		break;	
#endif
	}
}
#endif

//----------------------------------------------------------------------------
// ĳ���� ������ �����Ѵ�.
//----------------------------------------------------------------------------
BOOL gObjSetCharacter(BYTE *lpdata, int aIndex)
{
	int itype;
	int n;
	char szCharName[MAX_IDSTRING+1];

	LPSDHP_DBCHAR_INFORESULT lpMsg = (LPSDHP_DBCHAR_INFORESULT)lpdata;
	
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT)  )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}
	LPOBJECTSTRUCT	lpObj;

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	if( lpObj->Connected < CSS_LOGINOK )
	{
		return FALSE;
	}
	
	// ���� �÷������ε� �ٽ� ������ �Ǿ��ٸ�.. �����Ų��.
	if( gObj[aIndex].Connected == CSS_GAMEPLAYING )	
	{
		LogAddC(LOGC_RED,lMsg.Get(498), gObj[aIndex].AccountID, gObj[aIndex].Name);
		return FALSE;
	}
		
#ifdef NEW_SKILL_FORSKYLAND
	if( (lpMsg->Class) == 0  ||	(lpMsg->Class) == 16 ||
		(lpMsg->Class) == 32 ||	(lpMsg->Class) == 48 || 

// ������ ĳ����
		(lpMsg->Class) == 1  ||	(lpMsg->Class) == 17 ||
		(lpMsg->Class) == 33 
#ifdef DARKLORD_WORK		
		|| lpMsg->Class == 64
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// ĳ���� ���� ���� - 3�� ���� ��ȣ �߰�
		|| lpMsg->Class	== GRAND_MASTER
		|| lpMsg->Class	== BLADE_MASTER
		|| lpMsg->Class	== HIGH_ELF
		|| lpMsg->Class	== DUEL_MASTER
		|| lpMsg->Class	== LORD_EMPEROR
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� '��ȯ����'
		|| lpMsg->Class	== 80
		|| lpMsg->Class	== BLOODY_SUMMONER	// 1������
		|| lpMsg->Class	== DIMENSION_MASTER	// 3������
#endif
		)
	{
		// �˷��� ĳ�����̴�.
	}
	else 
	{		
		return FALSE;
	}
#else
	if( (lpMsg->Class) == 0  ||	(lpMsg->Class) == 16 ||
		(lpMsg->Class) == 32 ||	(lpMsg->Class) == 48 )
	{
		// �˷��� ĳ�����̴�.
	}
	else 
	{		
		return FALSE;
	}
#endif


	gObjCharZeroSet(aIndex);

#ifdef MODIFY_MIX_SYSTEM_20070518
	if( g_MixSystem.ChaosBoxInit(lpObj) == FALSE )
	{
		LogAdd("error-L1 : ChaosBox Init error %s %d", __FILE__, __LINE__);
	}
#endif

	lpObj->m_PosNum     = 0;
	lpObj->Level		= lpMsg->Level;
	lpObj->LevelUpPoint = lpMsg->LevelUpPoint;
	lpObj->DbClass		= lpMsg->Class;
	lpObj->Class		= lpMsg->Class>>4;
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// �ɸ��Ϳ� ü������ ����
	int iChangeUp			= ( lpMsg->Class )&CHANGEUP_MASK;
	lpObj->ChangeUP			= iChangeUp&0x01;		// 2�� ü������
	lpObj->ThirdChangeUp	= ( iChangeUp&0x02 ) >> 1;		// 3�� ü������
#else
#ifdef NEW_SKILL_FORSKYLAND
	lpObj->ChangeUP		= (lpMsg->Class)&CHANGEUP_MASK;		// ������ ĳ���� �ΰ�?
#else
	lpObj->Skin			= lpMsg->Class&0x0F;
#endif

#endif	// THIRD_CHANGEUP_SYSTEM_20070507

	
	szCharName[MAX_IDSTRING] = '\0';
	memcpy(szCharName, lpMsg->Name, MAX_IDSTRING);
	strcpy(lpObj->Name, szCharName);
	//LogAdd("ĳ���� ���̵� ���� : [%s][%s] ", lpObj->AccountID, lpObj->Name);

	if( lpObj->Level < 1 )
	{
		LogAddC(LOGC_RED, "error : %s Level Zero", lpMsg->Name);
		return FALSE;
	}

	lpObj->X			= lpMsg->MapX;
	lpObj->Y			= lpMsg->MapY;
	lpObj->MapNumber	= lpMsg->MapNumber;

	lpObj->StartX       = (BYTE)lpObj->X;
	lpObj->StartY       = (BYTE)lpObj->Y;

#if ENABLE_MC_CODE == 1
	if (!CHECK_LIMIT(lpObj->MapNumber, g_TerrainManager.Size()))
#else // ENABLE_MC_CODE == 1
	if (!CHECK_LIMIT(lpObj->MapNumber, MAX_MAP))
#endif // ENABLE_MC_CODE == 1
	{
		LogAdd("error : Map Number max over. %s %d",__FILE__, __LINE__);
		lpObj->MapNumber = 0;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
	}
	
	// ������ 6���� ������ �������� �¾�� �� �̻��̸� ������ ������ �¾��..
	// �Ǹ��� �����̸�, ������..
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( lpObj->Level < 6 || CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
	if( lpObj->Level < 6 || lpObj->MapNumber == 9 )
#endif
	{
		if( lpObj->Class == CLASS_ELF )	{
			lpObj->MapNumber = 3;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
		}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� ������ġ
		else if( lpObj->Class == CLASS_SUMMONER )	{
			lpObj->MapNumber = MAP_INDEX_ELBELAND;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
		}
#endif
		else {
			lpObj->MapNumber = 0;
			MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y );
		}
	}

#ifdef MODIFY_SANTACLAUSVILLAGE_VISITLEVEL
	if (lpObj->MapNumber == MAP_INDEX_XMASEVENT && lpObj->Level < 15)
	{
		if( lpObj->Class == CLASS_ELF )	{
			lpObj->MapNumber = 3;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
		}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� ������ġ
		else if( lpObj->Class == CLASS_SUMMONER )	{
			lpObj->MapNumber = MAP_INDEX_ELBELAND;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
		}
#endif
		else {
			lpObj->MapNumber = 0;
			MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y );
		}
	}
#endif // MODIFY_SANTACLAUSVILLAGE_VISITLEVEL

	
#ifdef FOR_BLOODCASTLE						// ���� �� ����ĳ�� �̾��ٸ� ����ƽ��� �̵�
	if( CHECK_BLOODCASTLE(lpObj->MapNumber) ) {
		lpObj->MapNumber = 2;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
	}

	lpObj->m_cBloodCastleIndex = -1;
	lpObj->m_cBloodCastleSubIndex = -1;
	lpObj->m_iBloodCastleEXP = 0;
#endif

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	lpObj->m_bBloodCastleComplete = false;
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408			// ���� �� ī����ĳ�� �̾��ٸ� ����ƽ��� �̵�
	if( CHECK_CHAOSCASTLE(lpObj->MapNumber) ) {
		lpObj->MapNumber = 2;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
	}

	lpObj->m_cChaosCastleIndex		= -1;
	lpObj->m_cChaosCastleSubIndex	= -1;
	lpObj->m_iChaosCastleBlowTime	= 0;
	lpObj->m_cKillUserCount			= 0;
	lpObj->m_cKillMonsterCount		= 0;
#endif

#ifdef UPDATE_GM_FUNCTION_20070228	// �Ϲ������� ���� �� GM ��ȯ �����̾��ٸ� ����ƽ��� �̵�
	if( ( (lpMsg->CtlCode&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
		&& ( lpObj->MapNumber == 40 ) )
	{
		lpObj->MapNumber = 2;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
	}
#endif	// UPDATE_GM_FUNCTION_20070228

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328			// ���ӽ� ȯ������̾��ٸ� ����ƽ���
	else if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) )
	{
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ȯ����� ���� NPC�� ��������� �̵�
		lpObj->MapNumber = 51;
#else
		lpObj->MapNumber = 2;
#endif
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
	}
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518			// ���� �� Į���� ���ٸ� ����ƽ���
//	if( CHECK_KALIMAMAP(lpObj->MapNumber) ) {
//		lpObj->MapNumber = 2;
//		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
//	}
#endif	

#ifdef CASTLE_MAIN_SCHEDULER_20041111			// ���� �� �������̾��ٸ� ������ ������ҷ� �̵�
#ifdef MODIFY_LOGINUSER_SET_REGEN_COORD_AT_CASTLE_MAP_20060511
	// �������϶��� ������ҷ� �̵��ϵ��� ����
	if ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
#else
	if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE) 
#endif
	{
		SHORT sX, sY, sLVL;
		BYTE btMAPNUM	= lpObj->MapNumber; 
		BYTE btDIR;
		INT iMapNumber	= gGateC.GetGate(100, sX, sY, btMAPNUM, btDIR, sLVL);
		if( iMapNumber >= 0 ) 
		{
			lpObj->MapNumber	= btMAPNUM;
			lpObj->X			= sX;
			lpObj->Y			= sY;

#ifdef MODIFY_LOGINUSER_SET_REGEN_COORD_AT_CASTLE_MAP_20060511			
			// �����ʿ��� �α׾ƿ� �� �� �������� ��� ĳ������ ������ ��ǥ�� �α׾ƿ����� ���� ��ǥ�� �����Ǳ� ������
			// �Ʒ��� �ʼ��� �̵��� ó������ ���� ������ ��ǥ���� ������ ��ǥ������ ����ȴ�.
			lpObj->m_sDestMapNumber		= btMAPNUM;
			lpObj->m_btDestX			= sX;
			lpObj->m_btDestY			= sY;					
#endif
		}
	}
#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111		// ���� �� ����Ϳ��ٸ� ������ ������ҷ� �̵�
	if (lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE) 
	{
		SHORT sX, sY, sLVL;
		BYTE btMAPNUM	= lpObj->MapNumber; 
		BYTE btDIR;
		INT iMapNumber	= gGateC.GetGate(CASTLE_HUNTZONE_GATE_01, sX, sY, btMAPNUM, btDIR, sLVL);
		if( iMapNumber >= 0 ) 
		{
			lpObj->MapNumber	= btMAPNUM;
			lpObj->X			= sX;
			lpObj->Y			= sY;
			
#ifdef MODIFY_LOGINUSER_SET_REGEN_COORD_AT_CASTLE_MAP_20060511		
			lpObj->m_sDestMapNumber		= btMAPNUM;
			lpObj->m_btDestX			= sX;
			lpObj->m_btDestY			= sY;	
#endif	// #ifdef MODIFY_LOGINUSER_SET_REGEN_COORD_AT_CASTLE_MAP_20060511
		}
	}
#endif	// #ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111

#ifdef ADD_KANTURU_20060627		// ���ӽ� ĭ���� �������̶�� 
	if( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS )
	{
		SHORT sX, sY, sLVL;
		BYTE btMAPNUM	= lpObj->MapNumber; 
		BYTE btDIR;
		INT iMapNumber	= gGateC.GetGate( KANTURU_DEFAULT_REGEN_MAP_GATE, sX, sY, btMAPNUM, btDIR, sLVL );
		if( iMapNumber >= 0 ) 
		{
			lpObj->MapNumber	= btMAPNUM;
			lpObj->X			= sX;
			lpObj->Y			= sY;
		}		
	}
#endif	// #ifdef ADD_KANTURU_20060627

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// ���ӽ� 3������ ����Ʈ���̶��
	if( lpObj->MapNumber == MAP_INDEX_3RD_CHANGEUP_QUEST 
		|| lpObj->MapNumber == MAP_INDEX_3RD_CHANGEUP_BOSS_QUEST )
	{
		int iGateNum = GATE_REGENPLACE_OF_BALGAS;

		SHORT sX, sY, sLVL;
		BYTE btMAPNUM	= lpObj->MapNumber; 
		BYTE btDIR;
		INT iMapNumber	= gGateC.GetGate( iGateNum, sX, sY, btMAPNUM, btDIR, sLVL );
		if( iMapNumber >= 0 ) 
		{
			lpObj->MapNumber	= btMAPNUM;
			lpObj->X			= sX;
			lpObj->Y			= sY;
		}		
	}
#endif

#ifdef MAP_SERVER_WORK_20041030				// ����ڰ� �ٸ� �ʼ������� ����� �̵��� �� ���̶�� �ʰ� ��ǥ�� ���⼭ �ٲپ� �ְ�, ���� �����̶�� �ٸ� �ʼ����� �̵� ��Ų��.
	if (lpObj->m_bMapSvrMoveReq == true) {
		// �ٸ� �ʼ������� �� ���̴�.
		SHORT sSvrCode			= g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->m_sDestMapNumber, lpObj->m_sPrevMapSvrCode);
		if (sSvrCode == gGameServerCode) {
			// �ʿ����� ���� ��ġ�� �����Ѵ�.
			lpObj->MapNumber	= lpObj->m_sDestMapNumber;
			lpObj->X			= lpObj->m_btDestX;
			lpObj->Y			= lpObj->m_btDestY;

			lpObj->StartX       = (BYTE)lpObj->X;
			lpObj->StartY       = (BYTE)lpObj->Y;
		}
		else {
			// ġ������ ���� -> �̵��� ���������� �ش� �� ������ ������ ���� �ʴٰ� ��??
			LogAddC(LOGC_RED, "[MapServerMng] Dest Map Server doesn't have Map [%s][%s] : %d",
				lpObj->AccountID,
				lpObj->Name,
				lpObj->m_sDestMapNumber
				);
			// �� ������ ����ڸ� ������ �� ���� -> ���� ����
			gObjDel(lpObj->m_Index);
			return FALSE;
		}
	}
#endif
	
#ifdef ADD_NEWPVP_PKFIELD
	if( gNonPK && (g_NewPVP.IsVulcanusMap(lpObj->MapNumber) || g_NewPVP.IsPKFieldMap(lpObj->MapNumber)) )
	{
		SHORT sX, sY, sLVL;
		BYTE btMAPNUM	= lpObj->MapNumber; 
		BYTE btDIR;
		INT iMapNumber	= gGateC.GetGate( LORENCIA_DEFAULT_REGEN_MAP_GATE, sX, sY, btMAPNUM, btDIR, sLVL );
		if( iMapNumber >= 0 ) 
		{
			lpObj->MapNumber	= btMAPNUM;
			lpObj->X			= sX;
			lpObj->Y			= sY;
		}		
	}
#ifndef FOR_KOREA
	if(g_NewPVP.IsVulcanusMap(lpObj->MapNumber) || g_NewPVP.IsPKFieldMap(lpObj->MapNumber) )
	{
		SHORT sX, sY, sLVL;
		BYTE btMAPNUM	= lpObj->MapNumber; 
		BYTE btDIR;
		INT iMapNumber	= gGateC.GetGate( LORENCIA_DEFAULT_REGEN_MAP_GATE, sX, sY, btMAPNUM, btDIR, sLVL );
		if( iMapNumber >= 0 ) 
		{
			lpObj->MapNumber	= btMAPNUM;
			lpObj->X			= sX;
			lpObj->Y			= sY;
		}		
	}	
#endif // NOT_FOR_KOREA
#endif // ADD_NEWPVP_PKFIELD

#ifdef TEST_SETITEM_CLEAR
	lpObj->ClearInfoForSetItem = (lpMsg->Dir>>4);
#endif


	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
	if( ((attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) ||
		((attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW) )
	{
		LogAdd(lMsg.Get(499));
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	int  result; 	
	short x, y;
	BYTE  mapNumber=0;
	BYTE  dir;
	short level;
	int  gt;
	int  userlevel = gObj[aIndex].Level;	

	x = lpObj->X;
	y = lpObj->Y;
	dir = lpObj->Dir;
	level = lpObj->Level;
	mapNumber = lpObj->MapNumber;
	
	gt = 0;

#ifdef MODIFY_KANTURU_MAPNUMBER_CHECK_20060807
	if( g_iBlockKanturuMapEnter == 1 )
	{
		if( gObj[aIndex].MapNumber >= MAP_INDEX_KANTURU1 && gObj[aIndex].MapNumber <= MAP_INDEX_KANTURU_BOSS )
		{
			LogAddTD("[ KANTURU ][ Forced Move From Kanturu to Lorencia ] (%s)(%s) (BeforeMap:%d(%d/%d))"
				,gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y );
			gObj[aIndex].MapNumber = 0;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
		}
	}

	if( g_iBlockCastleSiegeMapEnter == 1 )
	{
		if( gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE
			|| gObj[aIndex].MapNumber == MAP_INDEX_CASTLEHUNTZONE
			|| gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE
			|| gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_SECONDZONE )
		{
			LogAddTD("[ KANTURU ][ Forced Move From CastleSiegeMap to Lorencia ] (%s)(%s) (BeforeMap:%d(%d/%d))"
				,gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y );
			gObj[aIndex].MapNumber = 0;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
		}
	}
#endif // MODIFY_KANTURU_MAPNUMBER_CHECK_20060807

#ifdef ADD_RAKLION_20080408
	if( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )
	{
		// �������� ���� �����̹Ƿ� ��������� ������.
		if( g_Raklion.GetRaklionState() == RAKLION_STATE_CLOSE_DOOR
			|| g_Raklion.GetRaklionState() == RAKLION_STATE_ALL_USER_DIE
			|| g_Raklion.GetRaklionState() == RAKLION_STATE_NOTIFY_4
			|| g_Raklion.GetRaklionState() == RAKLION_STATE_END )
		{
			gt = 286;
		}
	}
	if( lpObj->MapNumber == MAP_INDEX_RAKLION_FIELD )
	{	// ��Ŭ���� �ʵ忡�� �����Ѵٸ� ���� �������� ���¸� �������ش�.
		g_RaklionUtil.NotifyRaklionCurrentState( lpObj->m_Index, g_Raklion.GetRaklionState(),
			g_Raklion.GetRaklionStateDetail() );
	}
#endif // ADD_RAKLION_20080408

#ifdef ADD_NEWPVP_PKFIELD		// ���ӽ� PK�� �̶��
	if( lpObj->MapNumber == MAP_INDEX_PKFIELD )
	{
		SHORT sX, sY, sLVL;
		BYTE btMAPNUM	= lpObj->MapNumber; 
		BYTE btDIR;
		INT iMapNumber	= gGateC.GetGate(NEWPVP_DEFAULT_REGEN_MAP_GATE, sX, sY, btMAPNUM, btDIR, sLVL );
		if( iMapNumber >= 0 ) 
		{
			lpObj->MapNumber	= btMAPNUM;
			lpObj->X			= sX;
			lpObj->Y			= sY;
		}		
	}
#endif // ADD_NEWPVP_PKFIELD

	if( lpObj->MapNumber == 2 ) // �����ʿ� ������ ���� ������..
	{
		gt = 22;
	}
	else if( lpObj->MapNumber == 3 ) // ���� ������ ������ ����������
	{
		gt = 27;
	}
	else if( lpObj->MapNumber == 4 ) // //ž1�� ��������� �̵�
	{
		gt = 42;
	}

	if( gt > 0 )
	{
		result = gGateC.GetGate(gt, x, y, mapNumber, dir, level);
		if( result >= 0 )
		{
#ifdef DARKLORD_WORK
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108	// ����� ���� ���˻�, ��ũ�ε�� Movereq.txt ������ ���� ����
			if( mapNumber != MAP_INDEX_SWAMP_OF_CALMNESS )
#endif	// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
			if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )	
#else
			if( lpObj->Class == CLASS_MAGUMSA )
#endif
			{
				if( level > 0 )
				{
					level = ((level/3)*2);
				}
			}

			if( (level != 0) && ( lpObj->Level < level) )
			{
				if( lpObj->Class == CLASS_ELF )	{
					lpObj->MapNumber = 3;
					MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
				}
				else {
					lpObj->MapNumber = 0;
					MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y );
				}
			}			
		}
		else LogAdd("error : %d (%s %d)", result, __FILE__, __LINE__);
	}

	lpMsg->MapX         = (BYTE)lpObj->X;
	lpMsg->MapY         = (BYTE)lpObj->Y;

	lpObj->TX		  = lpMsg->MapX;
	lpObj->TY		  = lpMsg->MapY;
	lpObj->m_OldX	  = lpObj->X;
	lpObj->m_OldY	  = lpObj->Y;
	lpObj->Experience = lpMsg->Exp;
	lpObj->Strength   = lpMsg->Str > g_GlobalConfig.m_iMax_Stats ? g_GlobalConfig.m_iMax_Stats : lpMsg->Str;
	lpObj->Dexterity  = lpMsg->Dex > g_GlobalConfig.m_iMax_Stats ? g_GlobalConfig.m_iMax_Stats : lpMsg->Dex;
	lpObj->Vitality   = lpMsg->Vit > g_GlobalConfig.m_iMax_Stats ? g_GlobalConfig.m_iMax_Stats : lpMsg->Vit;
	lpObj->Energy     = lpMsg->Energy > g_GlobalConfig.m_iMax_Stats ? g_GlobalConfig.m_iMax_Stats : lpMsg->Energy;
	lpObj->Life       = (float)lpMsg->Life/10.0f;
	lpObj->MaxLife    = (float)lpMsg->MaxLife/10.0f;
	lpObj->Mana       = (float)lpMsg->Mana/10.0f;
	lpObj->MaxMana    = (float)lpMsg->MaxMana/10.0f;
	lpObj->Money	  = lpMsg->Money;

#ifdef DARKLORD_WORK
	lpObj->Leadership = lpMsg->Leadership > g_GlobalConfig.m_iMax_Stats ? g_GlobalConfig.m_iMax_Stats : lpMsg->Leadership;
	lpObj->ChatLitmitTime = lpMsg->ChatLitmitTime;
	
#endif
	
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	// ���� ����Ʈ ����
	lpObj->iFruitPoint = lpMsg->iFruitPoint;
#endif

#ifdef DARKLORD_WORK
	#ifdef FOR_NEW_TESTSERVER		// 
		if( lpObj->Level > 1  )
		{
			if( lpObj->Experience < gLevelExperience[lpObj->Level-1] )
			{
				lpObj->Experience = gLevelExperience[lpObj->Level-1]; 
			}
		}
	#endif
#endif

	if( lpObj->Mana < 1.0f )
	{
		lpObj->Mana = 1.0f;
	}

	if( lpObj->Life < 1.0f )
	{
		lpObj->Life = 1.0f;
	}

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - �ִ� ����� : ���μ����� �����ͷ����� ���� ���� ����. �ڿ��� �ٽ� �� �ٰ���.
	lpObj->MaxLife    = DCInfo.DefClass[lpObj->Class].Life+\
		((lpObj->Level+lpObj->m_nMasterLevel-1)*DCInfo.DefClass[lpObj->Class].LevelLife)+\
		((lpObj->Vitality-DCInfo.DefClass[lpObj->Class].Vitality)*DCInfo.DefClass[lpObj->Class].VitalityToLife);
#else
	lpObj->MaxLife    = DCInfo.DefClass[lpObj->Class].Life+\
		((lpObj->Level-1)*DCInfo.DefClass[lpObj->Class].LevelLife)+\
		((lpObj->Vitality-DCInfo.DefClass[lpObj->Class].Vitality)*DCInfo.DefClass[lpObj->Class].VitalityToLife);
#endif

		//(lpObj->Vitality*DCInfo.DefClass[lpObj->Class].VitalityToLife);

#ifndef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018 // not define 
	// AddLife���� �ֱ� ������ Life�� MaxLife���� ū ���·�, ����� �� �ִ�. 
	if( lpObj->Life > lpObj->MaxLife ) 
	{
		lpObj->Life = lpObj->MaxLife;
	}
#endif
	//lpObj->MaxLife    += lpObj->AddLife;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - �ִ� ������ : ���μ����� �����ͷ����� ���� ���� ����. �ڿ��� �ٽ� �� �ٰ���.
	lpObj->MaxMana    = DCInfo.DefClass[lpObj->Class].Mana+\
		((lpObj->Level+lpObj->m_nMasterLevel-1)*DCInfo.DefClass[lpObj->Class].LevelMana)+\
		((lpObj->Energy-DCInfo.DefClass[lpObj->Class].Energy)*DCInfo.DefClass[lpObj->Class].EnergyToMana);
#else
	lpObj->MaxMana    = DCInfo.DefClass[lpObj->Class].Mana+\
		((lpObj->Level-1)*DCInfo.DefClass[lpObj->Class].LevelMana)+\
		((lpObj->Energy-DCInfo.DefClass[lpObj->Class].Energy)*DCInfo.DefClass[lpObj->Class].EnergyToMana);
#endif

#ifndef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018 // not define 
	// AddMana���� �ֱ� ������ Mana�� MaxMana ū ���·�, ����� �� �ִ�. 
	if( lpObj->Mana > lpObj->MaxMana ) 
	{
		lpObj->Mana = lpObj->MaxMana;
	}
#endif

	lpObj->VitalityToLife = DCInfo.DefClass[lpObj->Class].VitalityToLife;
	lpObj->EnergyToMana   = DCInfo.DefClass[lpObj->Class].EnergyToMana;

#ifdef NEW_FORSKYLAND2
	// AG�Ŀ� ���� �����Ѵ�
	gObjSetBP(aIndex);
	lpObj->BP = lpObj->MaxBP/2;
#endif

	// �ִ� ������� ���Ѵ�
	gObjCalcMaxLifePower(aIndex);
	
	lpObj->m_PK_Count	  = lpMsg->PkCount;
	lpObj->m_PK_Level	  = lpMsg->PkLevel;
	lpObj->m_PK_Time	  = lpMsg->PkTime;


	if( gApplyHeroSystem )
	{
		if( lpObj->m_PK_Level <= PK_LEVEL_DEFAULT )
		{
			lpObj->m_PK_Level = PK_LEVEL_DEFAULT-1;
			//lpMsg->PkLevel    = PK_LEVEL_DEFAULT-2;
		}
	}

	if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT )
	{
		lpObj->m_PK_Count	  = 0;
		lpObj->m_PK_Time	  = 0;
	}
	
	lpObj->MaxRegenTime = 4000;
	lpObj->m_MoveSpeed	= 1000;


#ifdef NEW_SKILL_FORSKYLAND	
	memcpy(lpObj->m_Quest, lpMsg->dbQuest, MAX_DBQUEST);

	if( lpObj->m_Quest[0] == 0 )
	{
		memset(lpObj->m_Quest, 0xff, MAX_DBQUEST);
	}
	
	//	memset(lpObj->m_Quest, 0xff, MAX_DBQUEST);
	//#endif

	#ifdef SECOND_QUEST
		if( g_QuestInfo.GetQuestState(lpObj, 2) == QS_CLEAR )
		{	// 2�� ������ Ŭ���� �Ǿ���
#ifdef QUEST_BUG_FIX
			if( lpObj->Level < 220 )
			{	// ���� -_-	
				g_QuestInfo.ReSetQuestState(lpObj, 2);
				LogAddTD("[%s][%s] Find Invalid QuestInfo (%d)", lpObj->AccountID, lpObj->Name, 2);
			}
			else
			{
				lpObj->PlusStatQuestClear = 1;
			}
#endif
		}

		if( g_QuestInfo.GetQuestState(lpObj, 3) == QS_CLEAR )
		{	// �޺� ��ų ����Ʈ�� Ŭ���� �Ǿ���
#ifdef QUEST_BUG_FIX
			if( lpObj->Level < 220 )
			{	// ���� -_-	
				g_QuestInfo.ReSetQuestState(lpObj, 3);
				LogAddTD("[%s][%s] Find Invalid QuestInfo (%d)", lpObj->AccountID, lpObj->Name, 3);
			}
			else
			{
				lpObj->ComboSkillquestClear = 1;
			}	
#endif
		}
	#endif
#endif
	
	gObjSetInventory1Pointer(lpObj);

	// GUILD
	lpObj->GuildNumber  = 0;
	lpObj->lpGuild		= NULL;
		
	// ��ų�� �߰��Ѵ�.( ������� �����̰���? )
	if( lpObj->Class == CLASS_WIZARD ||
		lpObj->Class == CLASS_ELF	 ||
		lpObj->Class == CLASS_KNIGHT ||
#ifdef DARKLORD_WORK
		lpObj->Class == CLASS_DARKLORD || 	
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		lpObj->Class == CLASS_SUMMONER||
#endif
		lpObj->Class == CLASS_MAGUMSA )

	{
		int EnergyBall=1;

#ifdef DARKLORD_WORK
		int StrongFear=1;
#endif

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
		BOOL IsGetInitinityArrowSkill = FALSE;	// 220������ ������� ���Ǵ�Ƽ �ַο� ��ų�� ���ٸ� �߰� �� �ش�.
#endif

		// Ư�� ������(���丣�����)�� ���䱸 �������� ���⿡ ����
		// ������ üũ �κ��� �־� ��������Ʈ�� �߰� ���θ� �Ǵ��Ұ�� 
		// ��ų�� ������� �ִ�. (apple)
		gObjMagicAddEnergyCheckOnOff(FALSE);
		for( n=0; n<MAX_MAGIC; n++)
		{
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// gObjSetCharacter : DB ��ų Ȯ�� ����
			itype = lpMsg->dbMagicList[n*3];
			if( ( lpMsg->dbMagicList[( n * 3 ) + 1] & 0x07 ) > 0 )
			{
				itype = itype * ( lpMsg->dbMagicList[( n * 3 ) + 1] & 0x07 ) + lpMsg->dbMagicList[( n * 3 ) + 2];
			}
#else
			itype = lpMsg->dbMagicList[n*3];
#endif
			if( itype != 0xFF )
			{
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// gObjSetCharacter : DB ��ų Ȯ�� ���� �� �����ͷ��� ��ų ��ġ ����
	#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��Ƽ�� ��ų Ȯ�� �߰�
				if( g_MasterSkillSystem.CheckMasterLevelSkill( itype ) )
	#else
				if( MagicDamageC.GetSkillUseType( itype ) == SKILL_USETYPE_MASTERSKILL_PASSIVE )
	#endif
				{
					// �⺻�� ��ų ������ Ȯ���ϴ� ��� ����.
					BYTE btLevel = (BYTE)( lpMsg->dbMagicList[(n * 3) + 1]>>3 );
					if( gObjMagicAdd( lpObj, itype, btLevel ) > -1 )
					{
						g_MasterSkillSystem.CalcPassiveSkillData( lpObj, itype, btLevel );
					}
				}
				else
				{
#endif
					if( itype == AT_SKILL_ENERGYBALL )
					{
						EnergyBall = 0;
					}

	#ifdef	DARKLORD_WORK
					if( itype == AT_SKILL_SPEAR )
					{	// ��ũ�ε�� ���Ǿ �⺻���� ���� �ȴ�
						StrongFear = 0;
					}
	#endif

	#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
					if( itype == AT_SKILL_INFINITY_ARROW )
					{
						IsGetInitinityArrowSkill = TRUE;
						gObjMagicAdd( lpObj, itype, ( lpMsg->dbMagicList[( n * 3 ) + 1] & 0x0F ) );
					}
	#endif
		
					if( (itype != AT_SKILL_BLOCKING) && 
						(itype != AT_SKILL_SWORD1) && 
						(itype != AT_SKILL_SWORD2) && 
						(itype != AT_SKILL_SWORD3) && 
						(itype != AT_SKILL_SWORD4) && 
						(itype != AT_SKILL_SWORD5) && 
						(itype != AT_SKILL_CROSSBOW) && 
						(itype != AT_SKILL_BOW) && 
						(itype != AT_SKILL_KNIGHTDINORANT) && 
						(itype != AT_SKILL_SWORD6) && 
						//(itype != AT_SKILL_4CROSSBOW) && 
	#ifdef DARKLORD_WORK
						(itype != AT_SKILL_DARKHORSE_ATTACK) && 					
	#endif
	#ifdef ADD_ITEM_FENRIR_01_20051110
						(itype != AT_SKILL_FENRIR_ATTACK) && 					
	#endif
	#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ��� �ִ� ��ų�� ���⼭ �߰� ����
						(itype != AT_SKILL_EXPLOSION_2) && 	
						(itype != AT_SKILL_REQUIEM) && 	
	#endif
	#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
						(itype != AT_SKILL_PULLUTION) &&
	#endif						
						(itype != AT_SKILL_5CROSSBOW) )
						gObjMagicAdd(lpObj, itype, (lpMsg->dbMagicList[(n*3)+1]&0x0F));

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
				}	// if( MagicDamageC.GetSkillUseType( itype ) == SKILL_USETYPE_MASTERSKILL_PASSIVE )
#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122
			}
		}
		// �������ε��� ������ ���ٸ�.. ������ �߰��Ѵ�.
		if( lpObj->Class == CLASS_WIZARD )
		{
			if( EnergyBall == 1 )
			{
				gObjMagicAdd(lpObj, AT_SKILL_ENERGYBALL, 0);
				EnergyBall = 0;
			}
		}

#ifdef	DARKLORD_WORK
		if( lpObj->Class == CLASS_DARKLORD )
		{
			if( StrongFear == 1 )
			{
				gObjMagicAdd(lpObj, AT_SKILL_SPEAR, 0);	// ���Ǿ� �߰�
				StrongFear = 0;
			}
		}
#endif

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
		if( IsGetInitinityArrowSkill == FALSE )
		{
			// 220���� �̻��� ������� ���Ǵ�Ƽ �ַο� ��ų�� ���� ���� ��� ��ų�� �����Ѵ�.
			if( lpObj->Class == CLASS_ELF && lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel() && lpObj->ChangeUP == 1 )
			{
				int iAddSkillPosition = gObjMagicAdd( lpObj, AT_SKILL_INFINITY_ARROW, 0 );
				if( iAddSkillPosition > 0 )
				{
					LogAddTD( "[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)"
						, lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->ChangeUP );
				}
			}
		}
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116

		if( lpObj->Class == CLASS_WIZARD )
		{
			gObjMagicAdd(lpObj, AT_SKILL_JAVALIN, 0);
		}
		else if( lpObj->Class == CLASS_KNIGHT )
		{
			gObjMagicAdd(lpObj, AT_SKILL_RUSH, 0);
		}
		else if( lpObj->Class == CLASS_ELF )
		{
			gObjMagicAdd(lpObj, AT_SKILL_DEEPIMPACT, 0);
		}
		else if( lpObj->Class == CLASS_MAGUMSA )
		{
			gObjMagicAdd(lpObj, AT_SKILL_ONE_FLASH, 0);
			gObjMagicAdd(lpObj, AT_SKILL_DEATH_CANNON, 0);
		}
		else if( lpObj->Class == CLASS_DARKLORD )
		{
			gObjMagicAdd(lpObj, AT_SKILL_SPACE_SPLIT, 0);
			gObjMagicAdd(lpObj, AT_SKILL_BRAND_OF_SKILL, 0);
		}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� ������ ������ų "ǥâ"�߰�
		else if( lpObj->Class == CLASS_SUMMONER )
		{
			gObjMagicAdd(lpObj, AT_SKILL_JAVALIN, 0);
		}
#endif		


#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531	
		gObjMagicAdd(lpObj, AT_SKILL_STUN, 0);	
		gObjMagicAdd(lpObj, AT_SKILL_REMOVAL_STUN, 0);	
		gObjMagicAdd(lpObj, AT_SKILL_ADD_MANA, 0);	
		gObjMagicAdd(lpObj, AT_SKILL_INVISIBLE, 0);	
		gObjMagicAdd(lpObj, AT_SKILL_REMOVAL_INVISIBLE, 0);	
		gObjMagicAdd(lpObj, AT_SKILL_REMOVAL_MAGIC, 0);
#endif

#endif

		gObjMagicAddEnergyCheckOnOff(TRUE);
	}

#ifdef ITEMDELETE_MODIFY_20040604
	bool bAllItemExist = true;
#endif
	
	if( lpMsg->DbVersion == 0x00 ) {
#ifdef ITEMDELETE_MODIFY_20040604
		DbItemSetInByte( lpObj, lpMsg, MAX_ITEMDBBYTE_V00, &bAllItemExist);

#ifdef PERSONAL_SHOP_20040113
	#ifdef PERSONAL_SHOP_DBVER_0_FIX_200406016		// �������� ������ ����̶�� ���λ��� ������ ������ �ʱ�ȭ �Ѵ�.
		for (INT iX = MAX_INVENTORY ; iX < MAX_INVENTORY_EXTEND ; iX++) {
			lpObj->Inventory1[iX].Clear();
		}
#ifdef PSHOP_INVENTORY_CLEAR_BUGFIX_20050321
		memset(&lpObj->InventoryMap1[MAX_INVENTORYMAP], 0xFF, MAX_PSHOPITEM);
#else
		memset(&lpObj->InventoryMap1[MAX_INVENTORY], 0xFF, sizeof(BYTE) * MAX_PSHOPITEM);
#endif		
	#endif
#endif

#else
		DbItemSetInByte( lpObj, lpMsg, MAX_ITEMDBBYTE_V00);

#ifdef PERSONAL_SHOP_20040113
	#ifdef PERSONAL_SHOP_DBVER_0_FIX_200406016		// �������� ������ ����̶�� ���λ��� ������ ������ �ʱ�ȭ �Ѵ�.
		for (INT iX = MAX_INVENTORY ; iX < MAX_INVENTORY_EXTEND ; iX++) {
			lpObj->Inventory1[iX].Clear();
		}
#ifdef PSHOP_INVENTORY_CLEAR_BUGFIX_20050321
		memset(&lpObj->InventoryMap1[MAX_INVENTORYMAP], 0xFF, MAX_PSHOPITEM);
#else
		memset(&lpObj->InventoryMap1[MAX_INVENTORY], 0xFF, sizeof(BYTE) * MAX_PSHOPITEM);
#endif		
	#endif		
#endif
		
#endif
	}
#ifdef ITEM_DBSIZE_EXTEND_20050706
	else if( lpMsg->DbVersion < 0x03 ) {
#ifdef ITEMDELETE_MODIFY_20040604
		DbItemSetInByte( lpObj, lpMsg, MAX_ITEMDBBYTE_V02, &bAllItemExist);
#else
		DbItemSetInByte( lpObj, lpMsg, MAX_ITEMDBBYTE_V02);
#endif
	}
#endif
	else {
#ifdef ITEMDELETE_MODIFY_20040604
		DbItemSetInByte( lpObj, lpMsg, MAX_ITEMDBBYTE, &bAllItemExist);
#else
		DbItemSetInByte( lpObj, lpMsg, MAX_ITEMDBBYTE);
#endif
	}

#ifdef PERSONAL_SHOP_DELETE_UNDER_LV5_20050203		// ���� 5 ���ϴ� ���λ����� ������� ���ϹǷ� �� ������ �������� ������ �� ���� -> �ʱ�ȭ
	if (lpObj->Level <= 5) {
		for (INT iX = MAX_INVENTORY ; iX < MAX_INVENTORY_EXTEND ; iX++) {
			if (lpObj->Inventory1[iX].IsItem() == TRUE) {
				// ���� �������� �����ߴٸ� �����ϱ� ���� ������ �α׸� �����.
				BYTE NewOption[8];
				for( int i=0; i<8; i++) NewOption[i]=0;

				if( lpObj->Inventory1[iX].m_NewOption&0x20) NewOption[0] = 1;
				if( lpObj->Inventory1[iX].m_NewOption&0x10) NewOption[1] = 1;
				if( lpObj->Inventory1[iX].m_NewOption&0x08) NewOption[2] = 1;
				if( lpObj->Inventory1[iX].m_NewOption&0x04) NewOption[3] = 1;
				if( lpObj->Inventory1[iX].m_NewOption&0x02) NewOption[4] = 1;
				if( lpObj->Inventory1[iX].m_NewOption&0x01) NewOption[5] = 1;		

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[PShop] PShop Item Delete LV <= 5 [%s][%s] [LV:%d] : serial:%u [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
#else
				LogAddTD("[PShop] PShop Item Delete LV <= 5 [%s][%s] [LV:%d] : serial:%d [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID,
					lpObj->Name,
					lpObj->Level,
					lpObj->Inventory1[iX].m_Number,
					ItemAttribute[lpObj->Inventory1[iX].m_Type].Name,
					lpObj->Inventory1[iX].m_Level,
					lpObj->Inventory1[iX].m_Option1,
					lpObj->Inventory1[iX].m_Option2,
					lpObj->Inventory1[iX].m_Option3,
					NewOption[0],
					NewOption[1],
					NewOption[2],
					NewOption[3],
					NewOption[4],
					NewOption[5],
					NewOption[6],
					lpObj->Inventory1[iX].m_SetOption
					);
			}
			lpObj->Inventory1[iX].Clear();
		}
#ifdef PSHOP_INVENTORY_CLEAR_BUGFIX_20050321
		memset(&lpObj->InventoryMap1[MAX_INVENTORYMAP], 0xFF, MAX_PSHOPITEM);
#else
		memset(&lpObj->InventoryMap1[MAX_INVENTORY], 0xFF, sizeof(BYTE) * MAX_PSHOPITEM);
#endif		
	}
#endif
	
	//------------------------------------------------------------------------
	lpObj->Live			= 1;
	lpObj->Type			= OBJTYPE_CHARACTER;
	lpObj->TargetNumber = -1;	// ��� ��ȣ�� ó���� �������� �ʴ´�.
	lpObj->Connected	= CSS_GAMEPLAYING;
	
	gObjMakePreviewCharSet(aIndex);			// !!> �ɸ��� ����� �����並 ����鼭 �������� gObjCalCharacter() ȣ�� !!
	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpMsg->NextExp		= lpObj->NextExp;	// ���� EXP ���� �ִ´�.
	
	if( lpObj->Life == 0 )
	{
		lpObj->Live		= 1;
		lpObj->m_State  = OBJST_DYING;	// �׾�� �ִٰ� �˸���.
		lpObj->DieRegen	= 1;
		lpObj->RegenTime= GetTickCount();
	}
	else 
	{
		lpObj->m_State		= OBJST_CREATE;
	}
	
	if( (lpMsg->CtlCode&0x08) == 0x08 ) 
	{
		lpObj->Authority = AUTHORITY_EVENT_GM;
		LogAddC(LOGC_RED,"(%s)(%s) Set Event GM", lpObj->AccountID, lpObj->Name);
		//cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
#ifdef UPDATE_GM_FUNCTION_20070228	// ���ñ�� �߰�
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
#endif
	}
	else lpObj->Authority = AUTHORITY_USER;
	
#ifdef CHINA_GM_EVENT
	if( (lpMsg->CtlCode&0x10) == 0x10 ) 
	{
		lpObj->Authority = AUTHORITY_EVENT_GM;
		LogAddC(LOGC_RED,"(%s)(%s) Set Event GM", lpObj->AccountID, lpObj->Name);
		//cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	}
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
	if( (lpMsg->CtlCode&0x10) == 0x10 ) 
	{
		lpObj->Authority = AUTHORITY_EVENT_GM;
		LogAddC(LOGC_RED,"(%s)(%s) Set Event GM", lpObj->AccountID, lpObj->Name);
		//cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	}
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#ifdef ADD_AUTHORITY_CODE_OF_EVENT_GM_20050602
	if( (lpMsg->CtlCode&0x20) == 0x20 ) 
	{
		lpObj->Authority = AUTHORITY_ADMIN | AUTHORITY_EVENT_GM;
		LogAddC(LOGC_RED,lMsg.Get(501), lpObj->AccountID, lpObj->Name);
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	}
#endif

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( lpObj->Authority == AUTHORITY_EVENT_GM )
	{
		gObjAddBuffEffect( lpObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
	}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#if (TESTSERVER == 1) || defined(FOR_TEMP_SERVER)
	if( (lpMsg->CtlCode&0x40) == 0x40 ) 
	{
		lpObj->Authority = AUTHORITY_SUPER_ADMIN;
		LogAddC(LOGC_RED,"(%s)(%s) Set Super Admin", lpObj->AccountID, lpObj->Name);
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	}
#endif

	// Admin, EventGm ���� ���� ��� 20081215 (grooving)
#ifdef MODIFIY_AUTHORITY_INTERGRATION_20081215
	if( (lpMsg->CtlCode&0x20) == 0x20 ) 
	{
		lpObj->Authority = AUTHORITY_ADMIN | AUTHORITY_EVENT_GM;
		LogAddC(LOGC_RED,"(%s)(%s) Set Japan ADMIN | EVENT_GM", lpObj->AccountID, lpObj->Name);
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	}
#endif
	lpObj->Penalty = 0;


	if( lpObj->m_cAccountItemBlock )
	{	// ���������ۺ�..
		lpObj->Penalty |= CTLCODE_ACCOUNTITEMBLOCK;
	}

	if( (lpMsg->CtlCode&0x02) == 0x02 ) 
	{
		lpObj->Penalty |= CTLCODE_ITEMDONTTOUCH;
		LogAddL("Penalty : Item Don't touch  %s %s", lpObj->AccountID, lpObj->Name);
	}	
	
	/*if( (lpMsg->AccountCtlCode&0x01) == 0x01 ) 
	{
		lpObj->Penalty |= CTLCODE_ACCOUNTITEMBLOCK;
		LogAddL("Penalty : CTLCODE_ACCOUNTITEMBLOCK %s %s", lpObj->AccountID, lpObj->Name);
	}
	*/
	
	gObjAuthorityCodeSet(lpObj);

	LogAddTD(lMsg.Get(502), lpObj->m_Index, lpObj->AccountID, lpObj->Name);

	BYTE weather = MapC[lpObj->MapNumber].GetWeather();
	if( (weather>>4) > 0 ) CGWeatherSend(aIndex, weather);

	// �巡�� �̺�Ʈ�� �������̸�.. -> ���⼭ �巡�� �̺�Ʈ�� ���������� �˷��ش�.
	if( DragonEvent->GetState() > 0 )
	{
		if( DragonEvent->GetMapNumber() == lpObj->MapNumber )
		{
			GCMapEventStateSend(lpObj->MapNumber, 1, 1);
		}
	}

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �������� ���� ���̸� ����ڿ��� �˷��ش�.
	if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE) {
		GCAnsCsNotifyStart(lpObj->m_Index, (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)?1:0);
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
//			g_CastleSiege.NotifySelfCsJoinSide (lpObj->m_Index);
			g_CastleSiege.NotifyCsSelfLeftTime (lpObj->m_Index);
		}
	}
#endif

	// ��嵥���� �̺�Ʈ�� �������̸� üũ�ؼ� �˷��ش�.
	gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);

	// main.exe�� üũ���� ��ȣȭ�Ͽ� ������?
	GCCheckMainExeKeySend(aIndex);

	// Ŭ���̾�Ʈ ���� Ȯ��??
#ifdef UPDATE_UNIFICATIONBILL_20090203
	if(m_ObjBill[aIndex].GetDeductType() > BILL_TYPE_NONE && m_ObjBill[aIndex].GetDeductType() <= BILL_TYPE_COLLOR)
	{
		gLCount[m_ObjBill[aIndex].GetDeductType()].Add();
	}
#else	// UPDATE_UNIFICATIONBILL_20090203	
	if(m_ObjBill[aIndex].GetCertify()>=0&&m_ObjBill[aIndex].GetCertify()<MAX_LOGIN_COUNT)
	{
		gLCount[m_ObjBill[aIndex].GetCertify()].Add();
	}
#endif	// UPDATE_UNIFICATIONBILL_20090203
	
#ifdef DELETE_GD_REQUEST_20080110
//#ifdef MASTER_LEVEL_UP_SYSTEM_20070912					// �� ���� �̵��� DS�� �����ͷ��� ���� ��û
//	g_MasterLevelSystem.GDReqMasterLevelInfo( lpObj );
//#endif
#endif

#ifndef MODIFY_ORDER_REQUEST_FRIENDLIST_20050205
	#ifdef __FRIEND_WORK__
		FriendListRequest(aIndex);
	#endif
#endif

#ifdef ITEMDELETE_MODIFY_20040604		// ������ DB������ ��ȯ �߿� ���� �������� �Ѱ��� �����Ѵٸ� ���⼭ Ŭ���̾�Ʈ�� �˷��ش�.
	if (bAllItemExist == false) {
		PMSG_NOTICE	pNotice;
	#ifdef MODIFY_NOTICE_20040325
		TNotice::MakeNoticeMsgEx( &pNotice, 1, "���� ������ �߰� !!!");
	#else
		pNotice.type = 1;
		wsprintf(pNotice.Notice, "���� ������ �߰� !!!");
		PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
	#endif
		DataSend(lpObj->m_Index, (LPBYTE)&pNotice, pNotice.h.size);
	}
#endif

#ifdef HAPPY_POUCH //happycat@20050201
	#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
		if( IsOKPCBangBenefitAll(lpObj) )
		{
			lpObj->m_lFreeKalimaTime = 900000;
		}
	#else
		if (lpObj->m_bPCBangUser)
		{
			lpObj->m_lFreeKalimaTime = 900000;
		}
	#endif // PCBANG_ADVANTAGE_EXPAND_20060919
		else
		{
			lpObj->m_lFreeKalimaTime = 0;
		}
#endif	
	
#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
	if(lpObj->Level <= g_iCharacterRecuperationMaxLevel) {
		lpObj->m_iAutoRecuperationTime = GetTickCount();
	}
	else lpObj->m_iAutoRecuperationTime = 0;
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	lpObj->dwShieldAutoRefillTimer = GetTickCount();
#endif

#ifdef UPDATE_SHIELD_OPTION_20060427
	LogAddTD("[ShieldSystem][CalcSDPoint] [%s][%s] user SD Gage : %d", lpObj->AccountID, lpObj->Name, lpObj->iMaxShield + lpObj->iAddShield );
#endif 

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	g_CashShop.AddUser( lpObj );
	g_CashShop.CGCashPoint( lpObj );
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	g_CashItemPeriodSystem.GDReqPeriodItemList( lpObj );
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef DELETE_GD_REQUEST_20080110
//#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704	// DS�� ����Ʈ ���� ųī��Ʈ ��û
//	// ���� ������ �Ϸ�Ǹ� ���� ųī��Ʈ ������ ��û�Ѵ�.
//	g_QuestUtil.GDReqQuestMonsterKillInfo( lpObj );
//#endif
#endif

#ifdef MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_2ND_20080917	// �α��ν� �������� ��� 
//	if( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )		// ��Ʋ������ �߰����� �ʴ´�.
//	{
//		// �������� ������ �ʾ����� ��Ʋ���� ����Ʈ�� �߰��Ѵ�.
//		if( g_Raklion.GetRaklionState() != RAKLION_STATE_CLOSE_DOOR
//			&& g_Raklion.GetRaklionState() != RAKLION_STATE_ALL_USER_DIE
//			&& g_Raklion.GetRaklionState() != RAKLION_STATE_NOTIFY_4
//			&& g_Raklion.GetRaklionState() != RAKLION_STATE_END )
//		{			
//			g_RaklionBattleUserMng.AddUserData( lpObj->m_Index );
//		}
//	}
#else
#ifdef ADD_RAKLION_20080408
	if( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )
	{
		// �������� ������ �ʾ����� ��Ʋ���� ����Ʈ�� �߰��Ѵ�.
		if( g_Raklion.GetRaklionState() != RAKLION_STATE_CLOSE_DOOR
			&& g_Raklion.GetRaklionState() != RAKLION_STATE_ALL_USER_DIE
			&& g_Raklion.GetRaklionState() != RAKLION_STATE_NOTIFY_4
			&& g_Raklion.GetRaklionState() != RAKLION_STATE_END )
		{			
			g_RaklionBattleUserMng.AddUserData( lpObj->m_Index );
		}
	}
#endif // ADD_RAKLION_20080408
#endif // MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_2ND_20080917

// ��ũ ���Ǹ��� ��ų�� �⺻������ �����ش�.
#ifdef MODIFY_DARK_SPRIT_SKILL_INITIALIZE_BUGFIX_20080702
	if( lpObj->Class == CLASS_DARKLORD )
		gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::PetItem_Mode_Normal);
#endif // MODIFY_DARK_SPRIT_SKILL_INITIALIZE_BUGFIX_20080702

	lpObj->m_AccountType = lpMsg->AccountType;

	lpObj->m_EndTime = lpMsg->EndTime;
	lpObj->m_JoinTime = time(NULL);
	lpObj->m_dwRequestVip = GetTickCount()+(5*60*1000);
	lpObj->m_bRequestVip = false;

	if(lpObj->m_EndTime <= 0)
	{
		lpObj->m_AccountType = 0;
		lpObj->m_EndTime = 0;
	}

	if(lpObj->m_AccountType != 0)
		lpObj->m_bVIPReload = true;

	//g_VIPSystem.CheckVIP(lpObj);

#if ENABLE_MC_SQL == 1
	cManager.ManagementProc(lpObj,"/vipinfo",lpObj->m_Index);
	int Golds = Manager.KCredits(gObj[aIndex].AccountID);
	MsgOutput(aIndex,"[CASH SYSTEM] Tienes %d de Cash", Golds);

	if(lpObj->m_AccountType != 0)
	{
		LogAddTD("VIP User Connected: [%s][%d][%s]",lpObj->AccountID,lpObj->m_AccountType,CTime(lpObj->m_EndTime).Format("%d/%m/%Y %H:%M:%S"));
	}
	else
		LogAddTD("Free User Connected: [%s]",lpObj->AccountID);
#endif // ENABLE_MC_SQL == 1

	return TRUE;
}

// ������ �������� �ǵ�� �ִ���..
// type : 
//#define I_TRADE						1		// ��ȯ ������
//#define I_PARTY						2		// ��Ƽ ������
//#define I_SHOP						3
//#define I_GUILD						4		// ��� ���� �������̽�
//#define I_GUILDCREATE					5		// ��� ���� �������̽�
//#define I_WAREHOUSE					6		// â��
//#define I_CHAOSBOX					7		// ī���� �ڽ�
BOOL gObjCanItemTouch(LPOBJECTSTRUCT lpObj, int type)
{
	// ������ ���̸� ������ �̵� �ȵǰ�..
	if( (lpObj->Penalty&CTLCODE_ITEMDONTTOUCH) == CTLCODE_ITEMDONTTOUCH)
	{
		return FALSE;
	}

	// ���� ������ ���̸� â�� ����� �����ϰ�.. 
	if( (lpObj->Penalty&CTLCODE_ACCOUNTITEMBLOCK) == CTLCODE_ACCOUNTITEMBLOCK)
	{
		if( type == I_WAREHOUSE || type == I_INVENTORY)
		{
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}


void gObjMagicTextSave(LPOBJECTSTRUCT lpObj)
{	
	int total=0;

	for(int n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == TRUE )
		{
			LogAddTD(lMsg.Get(503), lpObj->AccountID, lpObj->Name, n, lpObj->Magic[n].m_Skill);
			total++;
		}
	}
	//LogAddTD(lMsg.Get(503), lpObj->AccountID, lpObj->Name, total);
}

void ItemIsBufExOption(LPBYTE buf, CItem * lpItem)
{	
	
	for( int i=0; i<8; i++) buf[i]=0;

	if( lpItem->IsExtLifeAdd()            ) buf[0] = 1;
	if( lpItem->IsExtManaAdd()            ) buf[1] = 1;
	if( lpItem->IsExtDamageMinus()        ) buf[2] = 1;
	if( lpItem->IsExtDamageReflect()      ) buf[3] = 1;
	if( lpItem->IsExtDefenseSuccessfull() ) buf[4] = 1;
	if( lpItem->IsExtMonsterMoney()       ) buf[5] = 1;
}

void gObjStatTextSave(LPOBJECTSTRUCT lpObj)
{	
#ifdef DARKLORD_WORK
	LogAddTD("[%s][%s] CharInfoSave : Class=%d Level=%d LVPoint=%d Exp=%u Str=%d Dex=%d Vit=%d Energy=%d Leadership:%d Map=%d Pk=%d", 
			 lpObj->AccountID, lpObj->Name, 
			 lpObj->DbClass, lpObj->Level, 
			 lpObj->LevelUpPoint, lpObj->Experience, 
			 lpObj->Strength, lpObj->Dexterity, 
			 lpObj->Vitality, lpObj->Energy, 
			 lpObj->Leadership,
			 lpObj->MapNumber, lpObj->m_PK_Level);
#else
	LogAddTD("[%s][%s] CharInfoSave : Class=%d Level=%d LVPoint=%d Exp=%u Str=%d Dex=%d Vit=%d Energy=%d Map=%d Pk=%d", 
			 lpObj->AccountID, lpObj->Name, 
			 lpObj->DbClass, lpObj->Level, 
			 lpObj->LevelUpPoint, lpObj->Experience, 
			 lpObj->Strength, lpObj->Dexterity, 
			 lpObj->Vitality, lpObj->Energy, 
			 lpObj->MapNumber, lpObj->m_PK_Level);
#endif
}

void gObjItemTextSave(LPOBJECTSTRUCT lpObj)
{	
	BYTE NewOption[8];
	
#ifdef PERSONAL_SHOP_20040113
	for(int n=0; n<MAX_INVENTORY_EXTEND; n++)
#else
	for(int n=0; n<MAX_INVENTORY; n++)
#endif
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
#ifndef EXTEND_LOG_SYSTEM_05_20060823
			// ����, �ູ�� ������ �α׿� ���Ե��� �ʴ� ���� ����
			if( lpObj->pInventory[n].m_serial )
#endif // EXTEND_LOG_SYSTEM_05_20060823
			{
				ItemIsBufExOption(NewOption, &lpObj->pInventory[n]);

#ifdef EXTEND_LOG_SYSTEM_03_20060816
				LogAddTD(lMsg.Get(504), 
					lpObj->AccountID, 
					lpObj->Name, 
					n, 
					lpObj->pInventory[n].GetName(), 
					lpObj->pInventory[n].m_Level, 
					lpObj->pInventory[n].m_Option1, 
					lpObj->pInventory[n].m_Option2, 
					lpObj->pInventory[n].m_Option3, 
					lpObj->pInventory[n].m_Number, 
					(BYTE)lpObj->pInventory[n].m_Durability,
					NewOption[0],
					NewOption[1],
					NewOption[2],
					NewOption[3],
					NewOption[4],
					NewOption[5],
					NewOption[6],
					lpObj->pInventory[n].m_SetOption,
					lpObj->pInventory[n].m_ItemOptionEx>>7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pInventory[n] ),
					g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pInventory[n])
	#ifdef ADD_SOCKET_OPTION_LOG_02_20080821
					, lpObj->pInventory[n].m_SocketOption[0],
					lpObj->pInventory[n].m_SocketOption[1],
					lpObj->pInventory[n].m_SocketOption[2],
					lpObj->pInventory[n].m_SocketOption[3],
					lpObj->pInventory[n].m_SocketOption[4]
	#endif // ADD_SOCKET_OPTION_LOG_02_20080821
	//#ifdef MODIFY_BUGFIX_20081119
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
					, lpObj->pInventory[n].m_BonusSocketOption
	#endif
	//#endif	// MODIFY_BUGFIX_20081119
					);
#else // EXTEND_LOG_SYSTEM_03_20060816
				LogAddTD(lMsg.Get(504), 
					lpObj->AccountID, 
					lpObj->Name, 
					n, 
					lpObj->pInventory[n].GetName(), 
					lpObj->pInventory[n].m_Level, 
					lpObj->pInventory[n].m_Option1, 
					lpObj->pInventory[n].m_Option2, 
					lpObj->pInventory[n].m_Option3, 
					lpObj->pInventory[n].m_Number, 
					(BYTE)lpObj->pInventory[n].m_Durability,
					NewOption[0],
					NewOption[1],
					NewOption[2],
					NewOption[3],
					NewOption[4],
					NewOption[5],
					NewOption[6],
					lpObj->pInventory[n].m_SetOption
					);
#endif // EXTEND_LOG_SYSTEM_03_20060816

			}
		}
	}
	LogAddTD(lMsg.Get(505), lpObj->AccountID, lpObj->Name, lpObj->Money);
}

void gObjWarehouseTextSave(LPOBJECTSTRUCT lpObj)
{	
	BYTE NewOption[8];
	for(int n=0; n<MAX_WAREHOUSEITEMS; n++)
	{
		if( lpObj->pWarehouse[n].IsItem() == TRUE )
		{
#ifndef EXTEND_LOG_SYSTEM_05_20060823
			if( lpObj->pWarehouse[n].m_serial )
#endif // EXTEND_LOG_SYSTEM_05_20060823
			{
				ItemIsBufExOption(NewOption, &lpObj->pWarehouse[n]);

#ifdef EXTEND_LOG_SYSTEM_03_20060816
				LogAddTD(lMsg.Get(506), 
					lpObj->AccountID, 
					lpObj->Name, 
					n, 
					lpObj->pWarehouse[n].GetName(), 
					lpObj->pWarehouse[n].m_Level, 
					lpObj->pWarehouse[n].m_Option1, 
					lpObj->pWarehouse[n].m_Option2, 
					lpObj->pWarehouse[n].m_Option3, 
					lpObj->pWarehouse[n].m_Number, 
					(BYTE)lpObj->pWarehouse[n].m_Durability,
					NewOption[0],
					NewOption[1],
					NewOption[2],
					NewOption[3],
					NewOption[4],
					NewOption[5],
					NewOption[6],
					lpObj->pWarehouse[n].m_SetOption,
	#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
					lpObj->pWarehouse[n].m_ItemOptionEx>>7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pWarehouse[n] ),
					g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pWarehouse[n] )
	#else
					lpObj->pInventory[n].m_ItemOptionEx>>7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pInventory[n] ),
					g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pInventory[n] )
	#endif // MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef ADD_SOCKET_OPTION_LOG_02_20080821
					, lpObj->pWarehouse[n].m_SocketOption[0],
					lpObj->pWarehouse[n].m_SocketOption[1],
					lpObj->pWarehouse[n].m_SocketOption[2],
					lpObj->pWarehouse[n].m_SocketOption[3],
					lpObj->pWarehouse[n].m_SocketOption[4]
	#endif // ADD_SOCKET_OPTION_LOG_02_20080821
	//#ifdef MODIFY_BUGFIX_20081119
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
					, lpObj->pWarehouse[n].m_BonusSocketOption
	#endif
	//#endif	// MODIFY_BUGFIX_20081119
					);
#else // EXTEND_LOG_SYSTEM_03_20060816
				LogAddTD(lMsg.Get(506), 
					lpObj->AccountID, 
					lpObj->Name, 
					n, 
					lpObj->pWarehouse[n].GetName(), 
					lpObj->pWarehouse[n].m_Level, 
					lpObj->pWarehouse[n].m_Option1, 
					lpObj->pWarehouse[n].m_Option2, 
					lpObj->pWarehouse[n].m_Option3, 
					lpObj->pWarehouse[n].m_Number, 
					(BYTE)lpObj->pWarehouse[n].m_Durability,
					NewOption[0],
					NewOption[1],
					NewOption[2],
					NewOption[3],
					NewOption[4],
					NewOption[5],
					NewOption[6],
					lpObj->pWarehouse[n].m_SetOption
					);
#endif // EXTEND_LOG_SYSTEM_03_20060816

			}
		}
	}
	LogAddTD(lMsg.Get(507), lpObj->AccountID, lpObj->Name, lpObj->WarehouseMoney);
}


void gObjAuthorityCodeSet(LPOBJECTSTRUCT lpObj)
{
#if 0
	if( (lpObj->Authority&AUTHORITY_USER) == AUTHORITY_USER )
	{		
	}
	else if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN)
	{
		lpObj->AuthorityCode = AUTHORITY_CODE_FULLCONTROL;
	}
/*
	else if( (lpObj->Authority&AUTHORITY_SERVERADMIN) == AUTHORITY_SERVERADMIN)
	{
	}
*/
	else if( (lpObj->Authority&AUTHORITY_GM) == AUTHORITY_GM)
	{
		lpObj->AuthorityCode |= AUTHORITY_CODE_NOTICE;
		lpObj->AuthorityCode |= AUTHORITY_CODE_ACCOUNTBLOCK;
		lpObj->AuthorityCode |= AUTHORITY_CODE_DISCONNECT;
		lpObj->AuthorityCode |= AUTHORITY_CODE_SETPOSITION;
		lpObj->AuthorityCode |= AUTHORITY_CODE_COPYCHAR;
		lpObj->AuthorityCode |= AUTHORITY_CODE_CHATBAN;
	}
/*
	else if( (lpObj->Authority&AUTHORITY_SUBGM) == AUTHORITY_SUBGM)
	{
	}
*/
#ifdef ADD_AUTHORITY_CODE_OF_EVENT_GM_20050602
	else if( (lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)
	{
		lpObj->AuthorityCode |= AUTHORITY_CODE_NOTICE;
		lpObj->AuthorityCode |= AUTHORITY_CODE_ACCOUNTBLOCK;
		lpObj->AuthorityCode |= AUTHORITY_CODE_DISCONNECT;
		lpObj->AuthorityCode |= AUTHORITY_CODE_SETPOSITION;
		lpObj->AuthorityCode |= AUTHORITY_CODE_COPYCHAR;
		lpObj->AuthorityCode |= AUTHORITY_CODE_CHATBAN;
	}
#endif
	else {}
#endif
}

BOOL gObjSetPosMonster(int aIndex, int PosTableNum)//, int x, int y, int dir)
{
	LPOBJECTSTRUCT	lpObj;

	if( !CHECK_LIMIT(aIndex, MAX_OBJECT)  )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}	

	if( !CHECK_LIMIT(PosTableNum, MAX_MONSTER)  )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}	

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];	

	lpObj->m_PosNum     = PosTableNum;
	lpObj->X			= gMSetBase.m_Mp[PosTableNum].m_X;
	lpObj->Y			= gMSetBase.m_Mp[PosTableNum].m_Y;
	lpObj->MapNumber	= gMSetBase.m_Mp[PosTableNum].m_MapNumber;
	
	lpObj->TX			= lpObj->X;
	lpObj->TY			= lpObj->Y;
	lpObj->m_OldX		= lpObj->X;
	lpObj->m_OldY		= lpObj->Y;
	lpObj->Dir			= gMSetBase.m_Mp[PosTableNum].m_Dir;
	lpObj->StartX       = (BYTE)lpObj->X;
	lpObj->StartY       = (BYTE)lpObj->Y;

	if( lpObj->Class == 44 ||
		lpObj->Class == 53 ||
		lpObj->Class == 54 ||
		lpObj->Class == 55 || 
		lpObj->Class == 56)
	{
	}
	else if( lpObj->Class >= 78 && lpObj->Class <= 83 )
	{
	}
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311	
	else if( lpObj->Class >= 493 && lpObj->Class <= 502 )
	{
	}
#endif //ADD_GOLDEN_EVENT_RENEWAL_20090311
	else
	{
		if( gMSetBase.GetPosition(PosTableNum, lpObj->MapNumber, lpObj->X, lpObj->Y) == FALSE )
		{
			LogAdd(lMsg.Get(508), __FILE__, __LINE__);
			LogAdd("error-L2 : %d class : %d", PosTableNum, lpObj->Class);
			return FALSE;
		}
	}
	lpObj->TX			= lpObj->X;
	lpObj->TY			= lpObj->Y;
	lpObj->m_OldX		= lpObj->X;
	lpObj->m_OldY		= lpObj->Y;
	lpObj->Dir			= gMSetBase.m_Mp[PosTableNum].m_Dir;
	lpObj->StartX       = (BYTE)lpObj->X;
	lpObj->StartY       = (BYTE)lpObj->Y;

	return TRUE;
}

//----------------------------------------------------------------------------
// ���� ������ �����Ѵ�.
//----------------------------------------------------------------------------
BOOL gObjSetMonster(int aIndex, int MonsterClass)
{
	int Level;

		
	LPOBJECTSTRUCT	lpObj;

	if( !CHECK_LIMIT(aIndex, MAX_OBJECT)  )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}	

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];	
	//else if( lpMsg->Index == 0 ) strcpy(gObj[aIndex].Name, "Monster");

	lpObj->ConnectCheckTime	= GetTickCount();

	
	lpObj->ShopNumber	= -1;
	lpObj->TargetNumber = -1;
	lpObj->m_RecallMon  = -1;
	//251, 255, 254, 249, 249, 249, 249
	switch(MonsterClass)
	{
	case 200 : gBSGround[0]->m_BallIndex = aIndex; break;

#ifdef ADD_SHOP_11_12	
	case 251 : lpObj->ShopNumber = 0; break;	// �η��þ� (���������ѽ�) (116,141)
	case 255 : lpObj->ShopNumber = 1; break;	// �η��þ� ���� �ư���(���Ƹ�) (123,135)
	case 254 : lpObj->ShopNumber = 2; break;	// �η��þ� �������� (�Ľ�) (118,113)
	case 250 : lpObj->ShopNumber = 3; break;	// �η��þ� (�����̻���_�����Ĵ³�) (183,137)
	case 248 : lpObj->ShopNumber = 4; break;	// �η��þ� (������ ����) (7.148)
	case 239 : 
	case 253 : lpObj->ShopNumber = 5; break;	// �η��þ� (�����Ĵ¼ҳ�) (127,86)
	case 244 : lpObj->ShopNumber = 6; break;	// ����ƽ� (��������) (226,25)
	case 245 : lpObj->ShopNumber = 7; break;	// ����ƽ� (������) (225,41)
	case 246 : lpObj->ShopNumber = 8; break;	// ����ƽ� (�������) (186,47)
	case 243 : lpObj->ShopNumber = 9; break;	// �븮�� (����) (195,124)
	case 242 : lpObj->ShopNumber = 10; break;	// �븮�� (������) (173,125)

	case 231 : lpObj->ShopNumber = 12; break;	// ����ƽ� �� ����
	case 230 : lpObj->ShopNumber = 11; break;	// �η��þ� �� ����

	case 240 : lpObj->ShopNumber = 100; break;	// â������	
	case 238 : lpObj->ShopNumber = 101; break;	// ī���� �ڽ�	
	case 236 : lpObj->ShopNumber = 102; break;	// �̺�Ʈ�ü�	
#else
	case 238 : lpObj->ShopNumber = 12; break;	// ī���� �ڽ�
	case 240 : lpObj->ShopNumber = 11; break;	// â������
	case 242 : lpObj->ShopNumber = 10; break;	// 
	case 243 : lpObj->ShopNumber = 9; break;	// 
	case 246 : lpObj->ShopNumber = 8; break;	// �ܿ� ���� ����
	case 245 : lpObj->ShopNumber = 7; break;	// �ܿ� ������
	case 244 : lpObj->ShopNumber = 6; break;	// �ܿ� ���� ����
	case 239 : 
	case 253 : lpObj->ShopNumber = 5; break;	// ����
	case 248 : lpObj->ShopNumber = 4; break;	// ������ ����	
	case 250 : lpObj->ShopNumber = 3; break;
	case 251 : lpObj->ShopNumber = 0; break;
	case 255 : lpObj->ShopNumber = 1; break;
	case 254 : lpObj->ShopNumber = 2; break;	// �Ľ�
	case 236 : lpObj->ShopNumber = 13; break;	// �̺�Ʈ�ü�
#endif
		
#ifdef ADD_SHOP_KALIMA_NPC_REIRA_20060403
	case NPC_KALIMA_REIRA:	lpObj->ShopNumber	= 5; break;		// Į���� (���̶�) (37, 100)
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	case JEWELOFHARMONY_NPC_RESTORE:
	case JEWELOFHARMONY_NPC_SMELT:
	case JEWELOFHARMONY_NPC_PURITY:
		lpObj->ShopNumber = 101; // ī�����ڽ� ���
		break;
#endif
		
#ifdef UPDATE_QUARTERMASTER_NPC_20070122 // ���ں��ް� �ĸ��, ������ �߰�(goni)
	case NPC_QUARTERMASTER_PAMELLA:	lpObj->ShopNumber = 5; break;	// (����)���� ���ް� �ĸ�� (90,44)
	case NPC_QUARTERMASTER_ANGELA:	lpObj->ShopNumber = 5; break;	// (����)���� ���ް� ������ (89,218)
#endif

#ifdef ADD_FIRECRACKER_NPC_20070306			// �����Ǹ� NPC ����
	case NPC_FRIECRACKER_STORE:		lpObj->ShopNumber = 13; break;	// ����ƽ� ���� �Ǹ� NPC (30, 31)
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910		// �������� NPC ����
	case NPC_SILVIA:	lpObj->ShopNumber = 15;		break;		// ���� ���� �Ǻ��
	case NPC_RHEA:		lpObj->ShopNumber = 16;		break;		// �߱޹��� �� �� ���� ����
	case NPC_MARCE:		lpObj->ShopNumber = 17;		break;		// ���� ���� ������
#endif
		
#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
	case NPC_SEED_MASTER:
		lpObj->ShopNumber	= 101;
		break;
	case NPC_SEED_RESEARCHER:
		lpObj->ShopNumber	= 101;
		break;
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	case NPC_CHERRY:	lpObj->ShopNumber = 101;	break;		// ī�����ڽ� ���
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ������ ī���� �ڽ� ���
	case NPC_DELGADO:	lpObj->ShopNumber = 101;	break;		// ī�����ڽ� ���
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_GAMBLING_20090120
	case NPC_GAMBLER_1:
		lpObj->ShopNumber = 18;
		break;
#endif // ADD_GAMBLING_20090120
	}
	
	//------------------------------------------------------------------------
	lpObj->Connected	= CSS_GAMEPLAYING;	// ���ʹ� ������ �ʿ������ ������ ���ƴٰ� �����Ѵ�.
	lpObj->Live			= 0;
	lpObj->m_State		= 0;
	if( (MonsterClass == 44 ||
		MonsterClass == 54 ||
		MonsterClass == 53 ||
		lpObj->Class == 55 ||
		lpObj->Class == 56 
#ifdef NEW_SKILL_FORSKYLAND
		|| MonsterClass == 76			// ���� ������ �ʴ´�
#endif

#ifdef ELEGORADO_EVENT
		|| MonsterClass == 78
		|| MonsterClass == 79
		|| MonsterClass == 80
		|| MonsterClass == 81
		|| MonsterClass == 82
		|| MonsterClass == 83
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
		|| MonsterClass == 493
		|| MonsterClass == 494
		|| MonsterClass == 495
		|| MonsterClass == 496
		|| MonsterClass == 497
		|| MonsterClass == 498
		|| MonsterClass == 499
		|| MonsterClass == 500
		|| MonsterClass == 501
		|| MonsterClass == 502
#endif //ADD_GOLDEN_EVENT_RENEWAL_20090311
#endif

#ifdef ADD_RAKLION_20080408
	//	|| MonsterClass == 459			// ��Ŭ���� ����
#endif // ADD_RAKLION_20080408
		) )	// 44: �̺�Ʈ�巡�� �̶� ó���� �׾��ִٰ� �ð��̵Ǹ� ��Ƴ���.
	{			
		
	}
	else 
	{
		lpObj->Live		= 1;
		lpObj->m_State	= OBJST_CREATE;
	}

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131		// �̺�Ʈ�� ���Ұ�� ���͸� ����
	if(MonsterClass == MON_NEWYEAR_LUCKYBAG)
	{
		if(g_bNewYearLuckyBagMonsterEventOn)
		{
			lpObj->Live		= 1;
			lpObj->m_State	= OBJST_CREATE;
		}
		else
		{
			lpObj->Live		= 0;
			lpObj->m_State	= OBJST_NONE;
		}
	}
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	if( MonsterClass >= 204 && MonsterClass <= 259		// �Ű����̳� ����.
		|| MonsterClass >= JEWELOFHARMONY_NPC_PURITY && MonsterClass <= JEWELOFHARMONY_NPC_RESTORE // ȯ��NPC���� ����
#ifdef ADD_NEW_NPC_KANTURU_20060612
		|| MonsterClass == NPC_KANTURU_MACHINE			// ĭ���� ���� NPC �߰�
#endif // #ifdef ADD_NEW_NPC_KANTURU_20060612
#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
		|| MonsterClass == NPC_CHAOSCARDMASTER
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
		|| MonsterClass == NPC_WHITEANGEL_EVENT			// ȭ��Ʈ���� ������ ���� NPC �߰�
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507		// 3�� �������� NPC OBJTYPE ����
		|| MonsterClass == NPC_PRIEST_DEVIN				// ��������	
		|| MonsterClass == NPC_WERWOLF_QUERELLE			// ������� ����
		|| MonsterClass == NPC_THIRD_CHANGEUP_BOSS_GATE	// 3�� ü������ ������ ����
#endif	
#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823		// �ٷ� NPC ���
		|| MonsterClass == 414							// �ٷ�
#endif		
#ifdef ADD_NPC_XMASEVENT_20080930
		|| MonsterClass == NPC_SANTA		// ��ŸŬ�ν�
		|| MonsterClass == NPC_SNOW_MAN		// �����
		|| (MonsterClass >= NPC_LITTLE_SANTA_1 && MonsterClass <= NPC_LITTLE_SANTA_8)	// ��Ʋ��Ÿ1~8
#endif
#ifdef ADD_NEWPVP_PKFIELD
		|| MonsterClass == NPC_TITUS
#endif // ADD_NEWPVP_PKFIELD
		)
#else
	#ifdef CRYWOLF_NPC_WORK_20050912
		if( MonsterClass >= 204 && MonsterClass <= 257) // ������ �Ż�, ������ ����
	#else
			
		#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
			if( MonsterClass >= 216 && MonsterClass <= 257) // ������ ���� �߰�
		#else
			#ifdef	ADD_NEW_NPC_CASTLE_20041115						// �հ�, �հ�����ġ1-2, ����, ������(��/��), ���ο�, ��� �߰�
				if( MonsterClass >= 216 && MonsterClass <= 256)
			#else
				#ifdef 	JEWEL_MIX_SYSTEM_20050309						// ���� ���� NPC �߰�
					if( MonsterClass >= 226 && MonsterClass <= 256)
				#else
					#ifdef 	ADD_NEW_MAP_KALIMA_20040518						// Į���� - ���û� �߰�
						if( MonsterClass >= 226 && MonsterClass <= 255)
					#else
						if( MonsterClass >= 226 )
					#endif	// ADD_NEW_MAP_KALIMA_20040518
				#endif	// JEWEL_MIX_SYSTEM_20050309
			#endif	// ADD_NEW_NPC_CASTLE_20041115
		#endif // ADD_NPC_SHADOW_PHANTOM_20050616
	#endif // CRYWOLF_NPC_WORK_20050912
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530


//#ifdef DARKLORD_WORK
//	if( MonsterClass >= 226 )
//#else
//	if( MonsterClass >= 229 )
//#endif
	{
		lpObj->Type			= OBJTYPE_NPC;		// NPC
	}
	else 
	{
		lpObj->Type			= OBJTYPE_MONSTER;	// ����
	}
	
#ifdef PCBANG_COUPON_EVENT_20060124
	if( MonsterClass == 258 )
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#endif
#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
	if( MonsterClass == 464 )
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#endif// EXPERIENCE_SERVER_NPC_STAT_RESET
	

#ifdef UPDATE_QUARTERMASTER_NPC_20070122	// ���ں��ް� �ĸ��, ������ �߰�(goni)
	if( MonsterClass == NPC_QUARTERMASTER_PAMELLA || MonsterClass == NPC_QUARTERMASTER_ANGELA )
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
	if( MonsterClass == NPC_SEED_MASTER || MonsterClass == NPC_SEED_RESEARCHER )
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#ifdef ADD_FIRECRACKER_NPC_20070306			// �����Ǹ� NPC ����
	if( MonsterClass == NPC_FRIECRACKER_STORE )
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	if( MonsterClass == NPC_FRIECRACKER_STORE )
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// �̺�Ʈ �� NPC ����
	if(MonsterClass ==  NPC_PLATFORM_MIRAGE
		|| MonsterClass ==  NPC_ILLUSION_HOLY_RELICS_STATUE
		|| MonsterClass ==  NPC_LEADER_OF_ALLIED_FORCE
		|| MonsterClass ==  NPC_ELDER_OF_ILLUSION
		|| MonsterClass ==  NPC_HOLY_RELICS_BOX_ALLIED
		|| MonsterClass ==  NPC_HOLY_RELICS_BOX_ILLUSION)
	{
		lpObj->Type			= OBJTYPE_NPC;
	}

	// �̶��� �ϰ�� �ε����� �����س��´�.
	if(MonsterClass ==  NPC_PLATFORM_MIRAGE)
	{
		g_IllusionTempleEvent.SetMirageIndex(aIndex);
	}
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// �������� NPC
	if(MonsterClass ==  NPC_SILVIA
		|| MonsterClass ==  NPC_RHEA
		|| MonsterClass ==  NPC_MARCE)
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	if (MonsterClass == NPC_CHERRY)
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#ifdef MODIFY_CHEREEY_NPC_REGISTER_NPC_GROUP_20090803
	if (MonsterClass == NPC_CHERRY_TREE)
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#endif // MODIFY_CHEREEY_NPC_REGISTER_NPC_GROUP_20090803
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� NPC ������
	if(MonsterClass ==  NPC_DELGADO)
	{
		lpObj->Type			= OBJTYPE_NPC;
	}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef NEW_SKILL_FORSKYLAND
	if( MonsterClass == 77 )
	{	// õ���� �������� ����� ������
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
		int DefenceIndex=  gObjAddMonster(MAP_INDEX_ICARUS);
#else
		int DefenceIndex=  gObjAdd();
#endif
		if( DefenceIndex >= 0 )
		{
			if( !gObjSetMonster(DefenceIndex, 76) )
			{
				MsgBox("õ�������� ���� ����");
				return FALSE;
			}

			lpObj->m_SkyBossMonSheildLinkIndex = DefenceIndex;
			LogAddTD("SkyLand Boss Monster Setting!!");
		}
		else
		{			
			MsgBox("õ�������� ���� ����");
			return FALSE;
		}
	}
	if( MonsterClass == 76 )
	{
		LogAddTD("SkyLand Boss Shield Setting!!");
	}
#endif

	lpObj->Class		= MonsterClass;

	Level = lpObj->Level;

	LPMONSTER_ATTRIBUTE		lpm;
		
	lpm = gMAttr.GetAttr(lpObj->Class);	

	if( lpm == NULL )
	{
		MsgBox(lMsg.Get(509),lpObj->Class, __FILE__,__LINE__);
		return FALSE;
	}

	strncpy(gObj[aIndex].Name, lpm->m_Name, MAX_IDSTRING);

	if( lpm->m_Level == 0 )
	{
		LogAdd(lMsg.Get(510),lpObj->Class, __FILE__,__LINE__);
	}
	lpObj->Level				= lpm->m_Level;
	lpObj->m_AttackSpeed		= lpm->m_AttackSpeed;
	lpObj->m_AttackDamageMin	= lpm->m_DamageMin;
	lpObj->m_AttackDamageMax	= lpm->m_DamageMax;
	lpObj->m_Defense			= lpm->m_Defense;

	lpObj->m_MagicDefense		= lpm->m_MagicDefense;

	lpObj->m_AttackRating		= lpm->m_AttackRating;
	lpObj->m_SuccessfulBlocking	= lpm->m_Successfulblocking;
	lpObj->Life					= (float)lpm->m_Hp;
	lpObj->MaxLife				= (float)lpm->m_Hp;
	lpObj->Mana					= (float)lpm->m_Mp;
	lpObj->MaxMana				= (float)lpm->m_Mp;
	lpObj->m_MoveRange			= lpm->m_MoveRange;
	lpObj->m_AttackSpeed		= lpm->m_AttackSpeed;
	lpObj->m_MoveSpeed			= lpm->m_MoveSpeed;
	lpObj->MaxRegenTime			= lpm->m_RegenTime*1000;
	lpObj->m_AttackRange		= lpm->m_AttackRange;
	lpObj->m_ViewRange			= lpm->m_ViewRange;
	lpObj->m_Attribute		    = lpm->m_Attribute;
	lpObj->m_AttackType			= lpm->m_AttackType;
	lpObj->m_ItemRate			= lpm->m_ItemRate;
	lpObj->m_MoneyRate			= lpm->m_MoneyRate;
	lpObj->Dexterity			= 0;

#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
	lpObj->m_iScriptMaxLife		= lpm->m_iScriptHP;
#endif // MODIFY_CRYWOLF_BENEFIT_01_20060315

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	lpObj->m_iBasicAI			= lpm->m_iAINumber;
	lpObj->m_iCurrentAI			= lpm->m_iAINumber;
	lpObj->m_iCurrentAIState	= 0;
	lpObj->m_iLastAIRunTime		= 0;
	
	lpObj->m_iGroupNumber		= 0;
	lpObj->m_iSubGroupNumber	= 0;
	lpObj->m_iGroupMemberGuid	= -1;
	lpObj->m_iRegenType			= 0;

	lpObj->m_Agro.ResetAll();
	
	#ifdef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
		lpObj->m_iLastAutomataRuntime = 0;	// ���� Automata ���� �ð�
		lpObj->m_iLastAutomataDelay	  = 0;	// ���� ���� Automata�� Delay ��
	#endif // MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725


	(*lpObj->pInventoryCount)	= 0;

	memcpy(lpObj->m_Resistance,	lpm->m_Resistance, MAX_RESISTANCE);

	gObjSetInventory1Pointer(lpObj);

	if( lpObj->m_AttackType )
	{
		if( lpObj->m_AttackType >= 100 )
		{
			gObjMonsterMagicAdd(lpObj, (BYTE)(lpObj->m_AttackType-100), 1);
		}
		else gObjMonsterMagicAdd(lpObj, (BYTE)lpObj->m_AttackType, 1);

#ifdef ADD_NEW_MAP_KALIMA_20040518		// Į���� ���������� �߰� ���ݵ�
		if (
			lpObj->Class == 144 ||		// ���� ����1
			lpObj->Class == 174 ||		// ���� ����2
			lpObj->Class == 182 ||		// ���� ����3
			lpObj->Class == 190 ||		// ���� ����4
			lpObj->Class == 260 ||		// ���� ����5
			lpObj->Class == 268			// ���� ����6
//			||	lpObj->Class == 161 ||	// ����� ȯ��1
//			lpObj->Class == 181 || 		// ����� ȯ��2
//			lpObj->Class == 189 || 		// ����� ȯ��3
//			lpObj->Class == 197 || 		// ����� ȯ��4
//			lpObj->Class == 267 || 		// ����� ȯ��5
//			lpObj->Class == 275 		// ����� ȯ��6
			)
		{
			// ���
//			gObjMonsterMagicAdd(lpObj, 3, 1);
			// ������ ��
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}


		if (
			lpObj->Class == 145 ||		// �ε� ��Ʃ����1
			lpObj->Class == 175 ||		// �ε� ��Ʃ����2
			lpObj->Class == 183 ||		// �ε� ��Ʃ����3
			lpObj->Class == 191 ||		// �ε� ��Ʃ����4
			lpObj->Class == 261 ||		// �ε� ��Ʃ����5
			lpObj->Class == 269 ||		// �ε� ��Ʃ����6
			lpObj->Class == 146 ||		// ���� ����1
			lpObj->Class == 176 || 		// ���� ����2
			lpObj->Class == 184 ||		// ���� ����3
			lpObj->Class == 192 ||		// ���� ����4
			lpObj->Class == 262 ||		// ���� ����5
			lpObj->Class == 270 ||		// ���� ����6
			lpObj->Class == 147 ||		// ���� ��1
			lpObj->Class == 177 ||		// ���� ��2
			lpObj->Class == 185 ||		// ���� ��3
			lpObj->Class == 193 ||		// ���� ��4
			lpObj->Class == 263 ||		// ���� ��5
			lpObj->Class == 271 ||		// ���� ��6
			lpObj->Class == 148 ||		// ���� ��Ʃ����1
			lpObj->Class == 178 ||		// ���� ��Ʃ����2
			lpObj->Class == 186 ||		// ���� ��Ʃ����3
			lpObj->Class == 194 ||		// ���� ��Ʃ����4
			lpObj->Class == 264 ||		// ���� ��Ʃ����5
			lpObj->Class == 272 ||		// ���� ��Ʃ����6
//			lpObj->Class == 149 ||		// ��ũ��1
//			lpObj->Class == 179 ||		// ��ũ��2
//			lpObj->Class == 187 ||		// ��ũ��3
//			lpObj->Class == 195 ||		// ��ũ��4
//			lpObj->Class == 265 ||		// ��ũ��5
//			lpObj->Class == 273 ||		// ��ũ��6
			lpObj->Class == 160 ||		// ����� �ܿ�1
			lpObj->Class == 180 ||		// ����� �ܿ�2
			lpObj->Class == 188 ||		// ����� �ܿ�3
			lpObj->Class == 196 ||		// ����� �ܿ�4
			lpObj->Class == 266 ||		// ����� �ܿ�5
			lpObj->Class == 274			// ����� �ܿ�6
//			lpObj->Class == 161 ||		// ����� ȯ��1
//			lpObj->Class == 181 || 		// ����� ȯ��2
//			lpObj->Class == 189 || 		// ����� ȯ��3
//			lpObj->Class == 197 || 		// ����� ȯ��4
//			lpObj->Class == 267 || 		// ����� ȯ��5
//			lpObj->Class == 275 		// ����� ȯ��6
			)
		{
			// ������ ��
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}
		
		if (
			lpObj->Class == 161 ||		// ����� ȯ��1
			lpObj->Class == 181 || 		// ����� ȯ��2
			lpObj->Class == 189 || 		// ����� ȯ��3
			lpObj->Class == 197 || 		// ����� ȯ��4
			lpObj->Class == 267 || 		// ����� ȯ��5
			lpObj->Class == 275 		// ����� ȯ��6
			)
		{
// !!! . �̰��� ����� ��¥ ���
			// ���� ��ȯ
			gObjMonsterMagicAdd(lpObj, AT_SKILL_SUMMON, 1);
			// ������ ��
			gObjMonsterMagicAdd(lpObj, 17, 1);
			// ������
			gObjMonsterMagicAdd(lpObj, 1, 1);
			// ������� ����
			gObjMonsterMagicAdd(lpObj, AT_SKILL_IMMUNE_TO_MAGIC, 1);
			// ������� ����
			gObjMonsterMagicAdd(lpObj, AT_SKILL_IMMUNE_TO_HARM, 1);
			// ���°��� (���� ����)
			gObjMonsterMagicAdd(lpObj, AT_SKILL_DEFENSEDOWN, 1);

		}

		if (
			lpObj->Class == 149 ||		// ��ũ��1
			lpObj->Class == 179 ||		// ��ũ��2
			lpObj->Class == 187 ||		// ��ũ��3
			lpObj->Class == 195 ||		// ��ũ��4
			lpObj->Class == 265 ||		// ��ũ��5
			lpObj->Class == 273			// ��ũ��6
			)
		{
			// ������ ��
			gObjMonsterMagicAdd(lpObj, 17, 1);
			// ������
			gObjMonsterMagicAdd(lpObj, 1, 1);
		}
#endif

#ifdef FOR_BLOODCASTLE
		if( 
			lpObj->Class == 66 || 
			lpObj->Class == 73 || 
			lpObj->Class == 77 || 
			lpObj->Class == 89 ||		// �����ذ�1
			lpObj->Class == 95 ||		// �����ذ�2
			lpObj->Class == 112 ||		// �����ذ�3
			lpObj->Class == 118 ||		// �����ذ�4
			lpObj->Class == 124 ||		// �����ذ�5
			lpObj->Class == 130			// �����ذ�6
#ifdef BLOODCASTLE_EXTEND_20040314		// ����ĳ�� �������� üũ�� �ø�
			|| lpObj->Class == 143		// �����ذ�7
#endif	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ��8 �������� üũ�� �ø�
			|| lpObj->Class == 433		// �����ذ�8
#endif			
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ���� �ü����� ������ ��� ������ ���̴�.
			|| lpObj->Class == 163		// ī���� ĳ�� - �ü�1
			|| lpObj->Class == 165		// ī���� ĳ�� - �ü�2
			|| lpObj->Class == 167		// ī���� ĳ�� - �ü�3
			|| lpObj->Class == 169		// ī���� ĳ�� - �ü�4
			|| lpObj->Class == 171		// ī���� ĳ�� - �ü�5
			|| lpObj->Class == 173		// ī���� ĳ�� - �ü�6
#endif
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ�� �ü�7
			|| lpObj->Class == 427		// ī���� ĳ�� - �ü�7
#endif			
			)
		{
			// ������ ��
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}

		if (
			lpObj->Class == 89 ||		// �����ذ�1
			lpObj->Class == 95 ||		// �����ذ�2
			lpObj->Class == 112 ||		// �����ذ�3
			lpObj->Class == 118 ||		// �����ذ�4
			lpObj->Class == 124 ||		// �����ذ�5
			lpObj->Class == 130			// �����ذ�6
#ifdef BLOODCASTLE_EXTEND_20040314		// ����ĳ�� �������� üũ�� �ø�
			|| lpObj->Class == 143		// �����ذ�7
#endif			
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ�� �������� üũ�� �ø�
			|| lpObj->Class == 433		// �����ذ�8
#endif			
			)
		{
			// ���� �ذ��� ���� �Ϲ� ������ �� �� ����� ���������� ���� 1/2�� Ȯ���� ������.
			gObjMonsterMagicAdd(lpObj, 3, 1);
		}
#else
		if( lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77)
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}
#endif

	}

	gObjGiveItemSearch(lpObj, lpm->m_MaxItemLevel);
	gObjGiveItemWarehouseSearch(lpObj, lpm->m_MaxItemLevel);
	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;

	//------------------------------------------------------------------------

#ifdef FOR_BLOODCASTLE
	lpObj->LastAttackerID = -1;
#endif
		
	return TRUE;
}

int gCurConnectUser=0;

void gObjDestroy(SOCKET aSocket, int client)
{
	if( client < 0 || client > MAX_OBJECT-1 ) return;
	gObj[client].Connected = 0;
}

#include "ConMember.h"

//////////////////////////////////////////////////////////////////////////////
// ����ڰ� ���������� gObj[]�� ����ִ� ���� ������ �߰��Ѵ�.
// �����ϸ� ���° �߰��Ǿ������� �����ϰ� �����ϸ� -1�� �����Ѵ�.
short gObjAddSearch(SOCKET aSocket,char *ip)
{
	int count, totalcount=0;	

	// ������ ���� �ǹǷ� ���̻� ������ ���� �ʴ´�.
	if( gDisconnect == 1 ) return -1;

	if( gObjTotalUser > gServerMaxUser )
	{
		if( acceptIP.IsIp(ip) == FALSE )	// �������� �ʴٸ�..
		{
			GCJoinSocketResult(0x04, aSocket);
			//LogAdd("���̻� �߰��� �� ����. %s", ip);
			return -1;
		}
	}
	
	if( bCanConnectMember == 0 )
	{
#if (LOCALCONNECT == 1 )	
	gObjTotalUser = gServerMaxUser;
#ifdef MODIFY_WEBZEN_PUBLIC_IPADDRESS_20050822
		if( strcmp(ip, WEBZEN_PUBLIC_IPADDRESS) == 0 ||  
#else
		if( strcmp(ip, "218.234.76.33") == 0 ||  
#endif
			strncmp(ip, "10.1.2.", 6) == 0 )
		{
		}
		else
		{
			return -1;
		}
#endif
	}
	count = gObjCount;
	
	while( true )
	{
		if( gObj[count].Connected == 0 ) 
		{
			return (short)count;
		}
		count++;

		if( count >= MAX_OBJECT ) count = ALLOC_USEROBJECTSTART;
		totalcount++;
		if( totalcount >= MAX_USER )
		{
			//LogAdd("���̻� �߰��� �� ����. %s %d", __FILE__, __LINE__);
			break;
		}
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
// ����ڰ� ���������� gObj[]�� ����ִ� ���� ������ �߰��Ѵ�.
// �����ϸ� ���° �߰��Ǿ������� �����ϰ� �����ϸ� -1�� �����Ѵ�.
short gObjAdd(SOCKET aSocket,char *ip, int aIndex)
{
	if( gObj[aIndex].Connected ) return -1;
	
	gObjCharZeroSet(aIndex);
	gNSerialCheck[aIndex].init();
	gObj[aIndex].LoginMsgSnd	= 0;
	gObj[aIndex].LoginMsgCount= 0;
	
	memset(gObj[aIndex].AccountID, 0, MAX_IDSTRING);
	
	gObj[aIndex].m_Index		= aIndex;
	gObj[aIndex].m_socket		= aSocket;
	gObj[aIndex].AutoSaveTime	= gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].Connected		= CSS_LOGIN;
	gObj[aIndex].CheckSpeedHack	= 0;
	gObj[aIndex].LoginMsgCount  = 0;
	gObj[aIndex].Magumsa		= 0;
	gObj[aIndex].Type			= OBJTYPE_CHARACTER;

#ifdef HACKTOOL_KUZIMO
	gObj[aIndex].m_kRecvPacketStatistics.Init();
#endif

#ifdef ITEMSAVE_MODIFY_FORSTATICS	
	gObj[aIndex].SaveTimeForStatics = GetTickCount()+(1000*60*60);	// ù��°�� ������ ����ǰ� �Ѵ�
#endif

#ifdef HAPPY_POUCH				//happycat@20050201
	gObj[aIndex].m_bPCBangUser	= FALSE;
#endif

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	gObj[aIndex].m_bPCBangFreeChaosCastleUser	= FALSE;
#endif

#ifdef PCBANG_FREE_KALIMA_EVENT_20060124
	gObj[aIndex].m_bPCBangCouponUser = FALSE;
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206	// PC�� ����Ʈ ���� �ʱ�ȭ
	gObj[aIndex].m_bPCBangPointUser = FALSE;
	gObj[aIndex].m_PCBangPointInfo.Clear();
#endif

	m_ObjBill[aIndex].Init();
	strcpy(gObj[aIndex].Ip_addr, ip);
	
	LogAddTD("connect : [%d][%s]", aIndex, ip);
	
	gObjCount++;
	
	if( gObjCount >= MAX_OBJECT ) gObjCount = ALLOC_USEROBJECTSTART;
	return (short)aIndex;
}

/*
//////////////////////////////////////////////////////////////////////////////
// ����ڰ� ���������� gObj[]�� ����ִ� ���� ������ �߰��Ѵ�.
// �����ϸ� ���° �߰��Ǿ������� �����ϰ� �����ϸ� -1�� �����Ѵ�.
short gObjAdd(SOCKET aSocket,char *ip)
{
	int count, totalcount=0;	

	// ������ ���� �ǹǷ� ���̻� ������ ���� �ʴ´�.
	if( gDisconnect == 1 ) return -1;

	if( gObjTotalUser > gServerMaxUser )
	{
		if( acceptIP.IsIp(ip) == FALSE )	// �������� �ʴٸ�..
		{
			GCJoinSocketResult(0x04, aSocket);
			//LogAdd("���̻� �߰��� �� ����. %s", ip);
			return -1;
		}
	}
	
	if( bCanConnectMember == 0 )
	{
#if (LOCALCONNECT == 1 )	
	gObjTotalUser = gServerMaxUser;
		if( strcmp(ip, "61.33.48.33") == 0 || 
#ifdef MODIFY_WEBZEN_PUBLIC_IPADDRESS_20050822
			strcmp(ip, WEBZEN_PUBLIC_IPADDRESS) == 0 )
#else
			strcmp(ip, "218.234.76.33") == 0 )
#endif
		{
		}
		else
		{
			return -1;
		}
#endif
	}
	count = gObjCount;
	
	while( true )
	{
		if( gObj[count].Connected == 0 ) 
		{
			gObjCharZeroSet(count);
			gNSerialCheck[count].init();
			gObj[count].LoginMsgSnd	= 0;
			gObj[count].LoginMsgCount= 0;
			
			memset(gObj[count].AccountID, 0, MAX_IDSTRING);

			gObj[count].m_Index			= count;
			gObj[count].m_socket		= aSocket;
			gObj[count].AutoSaveTime	= gObj[count].ConnectCheckTime = GetTickCount();
			gObj[count].Connected		= CSS_LOGIN;
			gObj[count].CheckSpeedHack	= 0;
			gObj[count].LoginMsgCount   = 0;
			gObj[count].Magumsa			= 0;
			gObj[count].Type			= OBJTYPE_CHARACTER;
			m_ObjBill[count].Init();
			strcpy(gObj[count].Ip_addr, ip);

			LogAddTD("connect : [%d][%s]", count, ip);
			
			gObjCount++;

			if( gObjCount >= MAX_OBJECT ) gObjCount = ALLOC_USEROBJECTSTART;
			return (short)count;
		}
		count++;
		if( count >= MAX_OBJECT ) count = ALLOC_USEROBJECTSTART;
		totalcount++;
		if( totalcount >= MAX_OBJECT )
		{
			//LogAdd("���̻� �߰��� �� ����. %s %d", __FILE__, __LINE__);
			break;
		}
	}
	return -1;
}
*/


#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)

short gObjMonsterRecall(int iMapNumber)
{
	int number = gObjAddMonster(iMapNumber);
	if( number < 0 ) return -1;
	//gObjSetMonster( number, );
	return -1;
}

short gObjAddMonster(int iMapNumber)
{
#ifdef MAP_SERVER_WORK_20041030
	if (!g_MapServerManager.CheckMapCanMove(iMapNumber))
		return -1;
#endif

#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
	EnterCriticalSection(&gAddMonCriticalSection);
#endif

	int count, totalcount=0;	
	
	count = gObjMonCount;
	while( true )
	{
		if( gObj[count].Connected == 0 ) 
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index  = count;
			gObj[count].Connected = CSS_LOGIN;
			gObjMonCount++;
			
#ifdef MODIFY_ADDMONSTER_BUGFIX_20080423
			if( gObjMonCount < count)
			{
				gObjMonCount = count + 1;
			}
#endif	// MODIFY_ADDMONSTER_BUGFIX_20080423

			if( gObjMonCount >= MAX_MONSTER ) gObjMonCount = 0;

#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
			LeaveCriticalSection(&gAddMonCriticalSection);
#endif

			return (short)count;
		}
		count++;
		if( count >= MAX_MONSTER ) count = 0;
		totalcount++;
		if( totalcount >= MAX_MONSTER )
		{
#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
			LeaveCriticalSection(&gAddMonCriticalSection);
#endif

			LogAdd(lMsg.Get(511),__FILE__, __LINE__);
			return -1;
		}
	}

#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
	LeaveCriticalSection(&gAddMonCriticalSection);
#endif

	return -1;
}

#else

short gObjMonsterRecall()
{
	int number = gObjAdd();
	if( number < 0 ) return -1;
	//gObjSetMonster( number, );
	return -1;
}

// ���Ӽ������� ����ϴ� NPC, MONSTER �� �̰����� �߰��Ѵ�.
short gObjAdd()
{
#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
	EnterCriticalSection(&gAddMonCriticalSection);
#endif

	int count, totalcount=0;	
	
	count = gObjMonCount;
	while( true )
	{
		if( gObj[count].Connected == 0 ) 
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index  = count;
			gObj[count].Connected = CSS_LOGIN;
			gObjMonCount++;
			if( gObjMonCount >= MAX_MONSTER ) gObjMonCount = 0;

#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
			LeaveCriticalSection(&gAddMonCriticalSection);
#endif

			return (short)count;
		}
		count++;
		if( count >= MAX_MONSTER ) count = 0;
		totalcount++;
		if( totalcount >= MAX_MONSTER )
		{
#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
			LeaveCriticalSection(&gAddMonCriticalSection);
#endif

			LogAdd(lMsg.Get(511),__FILE__, __LINE__);
			return -1;
		}
	}

#ifdef MODIFY_ADDMONSTER_ADDLOCK_20080117
			LeaveCriticalSection(&gAddMonCriticalSection);
#endif

	return -1;
}

#endif


short gObjAddCallMon()
{
	int count, totalcount=0;	
	
	count = gObjCallMonCount;

	while( true )
	{
		if( gObj[count].Connected == 0 ) 
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index  = count;
			gObj[count].Connected = CSS_LOGIN;
			gObjCallMonCount++;

#ifdef MODIFY_ADDMONSTER_BUGFIX_20080423
			if( gObjCallMonCount < count)
			{
				gObjCallMonCount = count + 1;
			}
#endif	// MODIFY_ADDMONSTER_BUGFIX_20080423
			
			if( gObjCallMonCount >= ALLOC_USEROBJECTSTART ) gObjCallMonCount = MAX_MONSTER;
			return (short)count;
		}
		count++;
		if( count >= ALLOC_USEROBJECTSTART ) count = MAX_MONSTER;
		totalcount++;
		if( totalcount >= MAX_CALLMONSTER )
		{
			LogAdd(lMsg.Get(511),__FILE__, __LINE__);
			return -1;
		}
	}
	return -1;
}

// ����ڸ� ���� �����Ų��.
void gObjUserKill(int aIndex)
{
	LPOBJECTSTRUCT	lpObj;

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	
	if( lpObj->CloseCount < 1 )
	{
		lpObj->CloseCount = 6;
		lpObj->CloseType  = CL_EXIT;
	}
}

void gObjAllLogOut()
{
	static int OneMsg=0;
	
	if( OneMsg == 0 )
	{
		OneMsg = 1;
		gObjAllDisconnect();
		/*for( n=0; n<MAX_OBJECT; n++)
		{
			if( gObj[n].Connected > 0 && (gObj[n].Type == OBJTYPE_CHARACTER) )
			{
				if( gObj[n].Connected == CSS_GAMEPLAYING )
				{
					gObjTradeSave(&gObj[n], n);
					GJSetCharacterInfo(&gObj[n], n);
					Sleep(300);
				}
				if( gObj[n].Connected > CSS_LOGIN ) GJPUserClose(gObj[n].AccountID);		// ���������� ����� ���Ḧ �˸���.
				Sleep(100);
			}
		}
		*/
		Sleep(1000);
		LogAddC(LOGC_RED, lMsg.Get(512));
		//SendMessage(ghWnd, WM_CLOSE, NULL , NULL);
	}
}

void gObjAllDisconnect()
{
	gDisconnect = 1;
	for(int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected > 0 && (gObj[n].Type == OBJTYPE_CHARACTER) )
		{
			//wsGServer.Close(gObj[n].m_socket);
			CloseClient(n);
		}
	}

}

void gObjTradeSave(LPOBJECTSTRUCT	lpObj, int index)
{
	// ��ȯ �� �̾��ٸ�..
	if( lpObj->m_IfState.use && lpObj->m_IfState.type == I_TRADE )
	{
		int tObjNum = lpObj->TargetNumber;
		if( tObjNum > 0 )
		{
			CGTradeResult(tObjNum, 0x00);
			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(1101), lpObj->Name);
			GCServerMsgStringSend(szTemp, tObjNum, 1);
			gObjTradeCancel(tObjNum);
			LogAdd(lMsg.Get(513), gObj[tObjNum].AccountID, gObj[tObjNum].Name);
		}
		gObjTradeCancel(index);
		LogAdd(lMsg.Get(514), lpObj->AccountID, lpObj->Name);
	}
}

// ��ݰ��� ����
#ifdef UPDATE_UNIFICATIONBILL_20090203
void gObjBillRequest(LPOBJECTSTRUCT lpObj)
{
	char szMsg[128]		= "";
	
	switch(m_ObjBill[lpObj->m_Index].GetDeductType())
	{
	case BILL_TYPE_PERSONAL_AMOUNT:	// ��������
		{
			char szTemp[20]		= "";
			
			szTemp[5] = '\0';
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
			int user_year = atoi(szTemp);
			
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
			szTemp[2] = '\0';
			int user_mon = atoi(szTemp);
			
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
			szTemp[2] = '\0';
			int user_day = atoi(szTemp);

			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+8, 2);
			szTemp[2] = '\0';
			int user_hour = atoi(szTemp);

			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+10, 2);
			szTemp[2] = '\0';
			int user_minute = atoi(szTemp);
			
			wsprintf(szMsg, lMsg.Get(1509),user_year, user_mon, user_day, user_hour, user_minute);
			
		}		
		break;
	case BILL_TYPE_PCBANG_AMOUNT:	// PC�� ����
		{
			char szTemp[20]		= "";
			
			szTemp[4] = '\0';
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
			int user_year = atoi(szTemp);
			
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
			szTemp[2] = '\0';
			int user_mon = atoi(szTemp);
			
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
			szTemp[2] = '\0';
			int user_day = atoi(szTemp);

			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+8, 2);
			szTemp[2] = '\0';
			int user_hour = atoi(szTemp);
			
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+10, 2);
			szTemp[2] = '\0';
			int user_minute = atoi(szTemp);
			
			wsprintf(szMsg, lMsg.Get(1510), user_year, user_mon, user_day, user_hour, user_minute);
		}
		break;
	case BILL_TYPE_PCBANG_QUANTITY:	// PC�� ����
		{
			if(m_ObjBill[lpObj->m_Index].GetRestPoint() == 0 && m_ObjBill[lpObj->m_Index].GetRestTime() != 0)
			{
				wsprintf(szMsg, lMsg.Get(1506), m_ObjBill[lpObj->m_Index].GetRestTime());			
			}
			else if(m_ObjBill[lpObj->m_Index].GetRestPoint() != 0 && m_ObjBill[lpObj->m_Index].GetRestTime() == 0 )
			{
				wsprintf(szMsg, lMsg.Get(1507), m_ObjBill[lpObj->m_Index].GetRestPoint());			
			}
			else
			{
				wsprintf(szMsg, lMsg.Get(1508), m_ObjBill[lpObj->m_Index].GetRestTime(), m_ObjBill[lpObj->m_Index].GetRestPoint());	
			}
			
		}
		break;
	case BILL_TYPE_PERSONAL_QUANTITY:	// ���� ����	
		{
			if(m_ObjBill[lpObj->m_Index].GetRestPoint() == 0 && m_ObjBill[lpObj->m_Index].GetRestTime() != 0)
			{
				wsprintf(szMsg, lMsg.Get(1503), m_ObjBill[lpObj->m_Index].GetRestTime() / 60, m_ObjBill[lpObj->m_Index].GetRestTime() % 60 );			
			}
			else if(m_ObjBill[lpObj->m_Index].GetRestPoint() != 0 && m_ObjBill[lpObj->m_Index].GetRestTime() == 0 )
			{
				wsprintf(szMsg, lMsg.Get(1504), m_ObjBill[lpObj->m_Index].GetRestPoint());			
			}
			else
			{
				wsprintf(szMsg, lMsg.Get(1505), 
					m_ObjBill[lpObj->m_Index].GetRestTime() / 60, m_ObjBill[lpObj->m_Index].GetRestTime() % 60, m_ObjBill[lpObj->m_Index].GetRestPoint());	
			}		
		}
		break;
	case BILL_TYPE_COLLOR:		// ���� �÷������
		{
			char szTemp[20]		= "";
			
			szTemp[4] = '\0';
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
			int user_year = atoi(szTemp);
			
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
			szTemp[2] = '\0';
			int user_mon = atoi(szTemp);
			
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
			szTemp[2] = '\0';
			int user_day = atoi(szTemp);
			
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+8, 2);
			szTemp[2] = '\0';
			int user_hour = atoi(szTemp);
			
			strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+10, 2);
			szTemp[2] = '\0';
			int user_minute = atoi(szTemp);
			
			if(m_ObjBill[lpObj->m_Index].GetRestPoint() == 0 && m_ObjBill[lpObj->m_Index].GetRestTime() != 0)
			{
				wsprintf(szMsg, lMsg.Get(1512),user_year, user_mon, user_day, user_hour, user_minute, 
					m_ObjBill[lpObj->m_Index].GetRestTime() / 60, m_ObjBill[lpObj->m_Index].GetRestTime() % 60);
			}
			else if(m_ObjBill[lpObj->m_Index].GetRestPoint() != 0 && m_ObjBill[lpObj->m_Index].GetRestTime() == 0 )
			{
				wsprintf(szMsg, lMsg.Get(1513),user_year, user_mon, user_day, user_hour, user_minute, 
					m_ObjBill[lpObj->m_Index].GetRestPoint());
			}
			else
			{
				wsprintf(szMsg, lMsg.Get(1511),user_year, user_mon, user_day, user_hour, user_minute, 
					m_ObjBill[lpObj->m_Index].GetRestTime() / 60, m_ObjBill[lpObj->m_Index].GetRestTime() % 60, m_ObjBill[lpObj->m_Index].GetRestPoint());
			}		
		}
		break;
	default:
		break;
	}
	LogAdd(szMsg);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}
#else	// UPDATE_UNIFICATIONBILL_20090203
void gObjBillRequest(LPOBJECTSTRUCT lpObj)
{
	char	szMsg[128]= "";
	
	if( m_ObjBill[lpObj->m_Index].GetPayCode() == BILLTYPE_JUNGYANG )
	{
		if( m_ObjBill[lpObj->m_Index].GetCertify() == CERTIFYTYPE_ACCOUNT )
		{
			wsprintf(szMsg, lMsg.Get(1109), m_ObjBill[lpObj->m_Index].GetEndTime());
		}
		else if( m_ObjBill[lpObj->m_Index].GetCertify() == CERTIFYTYPE_IP )
		{
			if( m_ObjBill[lpObj->m_Index].GetEndTime() != 0  )
				wsprintf(szMsg, lMsg.Get(1110), m_ObjBill[lpObj->m_Index].GetEndTime());
		}
	}
	else if( m_ObjBill[lpObj->m_Index].GetPayCode() == BILLTYPE_JUNGACK )
	{
		char szTemp[20];
		
		szTemp[4] = '\0';
		strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
		int user_year = atoi(szTemp);
	
		strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
		szTemp[2] = '\0';
		int user_mon = atoi(szTemp);
	
		strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
		szTemp[2] = '\0';
		int user_day = atoi(szTemp);

		if( m_ObjBill[lpObj->m_Index].GetCertify() == CERTIFYTYPE_ACCOUNT )
		{
			wsprintf(szMsg, lMsg.Get(1111), user_year, user_mon, user_day);
		}
		else if( m_ObjBill[lpObj->m_Index].GetCertify() == CERTIFYTYPE_IP )
		{
			wsprintf(szMsg, lMsg.Get(1112),user_year, user_mon, user_day);
		}
	}
	else if( m_ObjBill[lpObj->m_Index].GetPayCode() == BILLTYPE_TIME )
	{
		char year[5]="", mon[3]="", day[3]="", hour[3]="", second[3]="";
		strncpy(year, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
		strncpy(mon, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
		strncpy(day, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
		strncpy(hour, m_ObjBill[lpObj->m_Index].GetEndsDays()+8, 2);
		strncpy(second, m_ObjBill[lpObj->m_Index].GetEndsDays()+10, 2);
		wsprintf(szMsg, lMsg.Get(1500), m_ObjBill[lpObj->m_Index].GetEndTime(), year, mon, day, hour, second);

		LogAddTD("[%s][%s] BillType : (Time) RemainPoint : (%d)", lpObj->AccountID, lpObj->Name, m_ObjBill[lpObj->m_Index].GetEndTime());
	}
	else if( m_ObjBill[lpObj->m_Index].GetPayCode() == BILLTYPE_DATE )
	{
		char year[5]="", mon[3]="", day[3]="";
		strncpy(year, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
		strncpy(mon, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
		strncpy(day, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
		wsprintf(szMsg, lMsg.Get(1501), year, mon, day);

		LogAddTD("[%s][%s] BillType : (Date) RemainDate : (%s-%s-%s)", lpObj->AccountID, lpObj->Name, year, mon, day);
	}
	else if( m_ObjBill[lpObj->m_Index].GetPayCode() == BILLTYPE_NOCHARGE )
	{
		wsprintf(szMsg, lMsg.Get(1502));
		LogAddTD("[%s][%s] BillType : (NoCharge)", lpObj->AccountID, lpObj->Name);
	}
	else wsprintf(szMsg, lMsg.Get(1113));

	LogAdd(szMsg);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}
#endif // UPDATE_UNIFICATIONBILL_20090203

//////////////////////////////////////////////////////////////////////////////
// 
short gObjMemFree(int index)
{
	int i=0;
	LPOBJECTSTRUCT	lpObj;

	if( gDisconnect == 1 ) return -1;

	if( index < 0 || index > MAX_OBJECT ) {
		LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}
	
	lpObj = (LPOBJECTSTRUCT)&gObj[index];
	if( lpObj->Connected < 1
#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
		&& lpObj->Connected != CSS_LOGINWAIT
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
		)
		return 0;
	
	gObjTradeSave(lpObj, index);

	// ������ �޾Ҿ��ٸ�.
	if( lpObj->Connected >= CSS_LOGINOK
#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
		|| lpObj->Connected == CSS_LOGINWAIT
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
		)
	{
		// ����ڿ��ٸ�..
		if( lpObj->Type == OBJTYPE_CHARACTER )	
		{
			// ������ �����ϴ� ���¶��.. ĳ���͸� �����Ѵ�.
			if( lpObj->Connected == CSS_GAMEPLAYING )
			{
#ifdef FOR_BLOODCASTLE
				if (CHECK_BLOODCASTLE(lpObj->MapNumber)) {					// DB�� �����͸� �����ϱ� ������ �������� ��ӽ�Ų��.
					g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
				}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ȯ�� ����ϰ�� ���� ����
				if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
				{
					g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
					g_IllusionTempleEvent.LeaveIllusionTemple(lpObj->m_Index, lpObj->MapNumber);
				}
#endif
				GJSetCharacterInfo(lpObj, index);
			}

#ifdef LOGINSERVER_MULOG
			GJPUserClose((LPOBJECTSTRUCT)lpObj);				// ���������� ����� ���Ḧ �˸���.
#else
#if defined(CHN_PLAYTIME_LIMIT_SYSTEM_20050818) || defined(VTM_PLAYTIME_LIMIT_SYSTEM_20060626)
			GJPUserClose(lpObj->AccountID, lpObj->m_iPlayTimeLimit);
#else
			GJPUserClose(lpObj->AccountID);						// ���������� ����� ���Ḧ �˸���.
#endif

#endif
		}
	}	
	gObjViewportClose(lpObj);
	lpObj->Connected = 0;	// ��� ����
	return 1;
}

// ���� ������ �̾��ٸ�.. ���ӿ� ���õ������� ��� �����Ų��.
int gObjGameClose(int aIndex)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT ) {
		LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, aIndex);
		return 0;
	}
	LPOBJECTSTRUCT	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	
	// ���� ���� ���� �ƴϸ� ����..
	if( lpObj->Connected != CSS_GAMEPLAYING ) 
	{
		return FALSE;
	}
	if( lpObj->Type != OBJTYPE_CHARACTER ) 
	{
		LogAddTD(lMsg.Get(515), lpObj->AccountID, lpObj->Name );
		return FALSE;
	}

#ifdef PCBANG_KUZIMO
	lpObj->m_kWeeklyTimer.Release();
#endif
	

#ifdef MU_CHN_HACKLOG_MODIFY_20040930 // ���� ���� �ÿ� ��ī��Ʈ�� 5ȸ�� �ʰ��� ����� �ٷα׸� �����.
	#ifdef CHECK_ATTACK_AREA
	if( lpObj->m_NotAttackAreaCount > 5 )	{
		INT iCheckTickCount = (GetTickCount() - lpObj->AutoSaveTime) / 1000;
		if (iCheckTickCount < 1)
			iCheckTickCount = 1;
		#ifdef CHINA_HACKUSER_KICK_SYSTEM		
				ChinaHackLogFile.Output("[%s][%s] Try Attack In Not Attack Area (%d)", 
										  lpObj->AccountID, 
										  lpObj->Name, 
										  lpObj->m_NotAttackAreaCount * 1000 / iCheckTickCount);
		#else // else CHINA_HACKUSER_KICK_SYSTEM		
				LogAddTD("[%s][%s] Try Attack In Not Attack Area (%d)", 
						  lpObj->AccountID, 
						  lpObj->Name, 
						  lpObj->m_NotAttackAreaCount * 1000 / iCheckTickCount);
		#endif // CHINA_HACKUSER_KICK_SYSTEM		
	}
	#endif // CHECK_ATTACK_AREA
#endif

#ifdef MU_CHN_HACKLOG3_MODIFY_20041117  // ���� ���� �ÿ� ��ī��Ʈ�� 5ȸ�� �ʰ��� ����� �ٷα׸� �����.
	#ifdef CHECK_ATTACK_AREA
	if( lpObj->m_NotAttackAreaCount > 5 )	{
		INT iCheckTickCount = (GetTickCount() - lpObj->AutoSaveTime) / 1000;
		if (iCheckTickCount < 1)
			iCheckTickCount = 1;		
		#ifdef CHINA_HACKUSER_KICK_SYSTEM		
			ChinaHackLogFile.Output("[%s][%s] Try Attack In Not Attack Area (%d)", 
									  lpObj->AccountID, 
									  lpObj->Name, 
									  lpObj->m_NotAttackAreaCount * 1000 / iCheckTickCount
									  );
		#else // else CHINA_HACKUSER_KICK_SYSTEM
			LogAddTD("[%s][%s] Try Attack In Not Attack Area (%d, %d)", 
					  lpObj->AccountID, 
					  lpObj->Name, 
					  lpObj->m_NotAttackAreaCount,
					  (GetTickCount()-lpObj->dwPlayTime)/1000 );
		#endif // CHINA_HACKUSER_KICK_SYSTEM
	}
	#endif // CHECK_ATTACK_AREA
#endif

#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518 // ���� ���� �ÿ� ��ī��Ʈ�� 5ȸ�� �ʰ��� ����� �ٷα׸� �����.
	#ifdef CHECK_ATTACK_AREA
	if( lpObj->m_NotAttackAreaCount > 5 )	{
		
		#ifdef CHINA_HACKUSER_KICK_SYSTEM
			INT iCheckTickCount = (GetTickCount() - lpObj->AutoSaveTime) / 1000;
			if (iCheckTickCount < 1)
				iCheckTickCount = 1;
			ChinaHackLogFile.Output("[%s][%s] Try Attack In Not Attack Area (%d) @%d", 
									  lpObj->AccountID, 
									  lpObj->Name, 
									  lpObj->m_NotAttackAreaCount * 1000 / iCheckTickCount,
									  lpObj->MapNumber);
		#else // else CHINA_HACKUSER_KICK_SYSTEM
			LogAddTD("[%s][%s] Try Attack In Not Attack Area (%d, %d) @%d", 
					  lpObj->AccountID, 
					  lpObj->Name,
					  lpObj->m_NotAttackAreaCount,
					  (GetTickCount()-lpObj->dwPlayTime)/1000,
					  lpObj->MapNumber);
		#endif // CHINA_HACKUSER_KICK_SYSTEM
	}
	#endif // CHECK_ATTACK_AREA

#endif // MU_CHN_HACKLOG_MODIFY_20040930 || MU_CHN_HACKLOG3_MODIFY_20041117
	
	//if( lpObj->Class == CLASS_ELF )
	{
		if( lpObj->m_RecallMon >= 0 ) gObjMonsterCallKill(lpObj->m_Index);
	}


	gObjMagicTextSave(lpObj);

#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610		// ���� �ŷ� ���̾��ٸ� �ŷ� ���¸� Ȯ���Ͽ� RollBack / Commit �� �����ش�.
#ifdef DARKLORD_WORK
	if ( (lpObj->m_IfState.use && lpObj->m_IfState.type == I_CHAOSBOX) || 
		(lpObj->m_IfState.use && lpObj->m_IfState.type == I_DARK_TRAINER) ) {	
#else
	if (lpObj->m_IfState.use && lpObj->m_IfState.type == I_CHAOSBOX) {
#endif

#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
		if( lpObj->bIsChaosMixCompleted == true || lpObj->m_bIsCastleNPCUpgradeCompleted == true ) 
		{
#else
		if (lpObj->bIsChaosMixCompleted == true) {
#endif
			gObjInventoryCommit(aIndex);
		}
		else {
			gObjInventoryRollback(aIndex);
		}
	}
#endif

	// Trade ����..
	gObjTradeSave(lpObj, aIndex);

#ifdef SECOND_QUEST
	if( lpObj->m_IfState.use && lpObj->m_IfState.type == I_QUEST && lpObj->TargetShopNumber == NPC_QEUSTNPC2 )
	{	// ����Ʈ â�� ���� ���ְ� NPC�� 2���� ����Ʈ NPC�̸�
		gQeustNpcTeleport.TalkRefDel();
	}
#endif
/*
#ifdef UPDATE_GM_FUNCTION_20070228
	if( (lpObj->Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN
		&& (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
#else
	if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN )
#endif
*/
	if( CheckAuthority(AUTHORITY_SUPER_ADMIN | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM, lpObj) == true )
	{
		cManager.ManagerDel(lpObj->Name);
	}

#ifdef DUEL_SYSTEM_20031028

	#ifdef ADD_NEWPVP_PKFIELD
	g_NewPVP.Reset(*lpObj);
	#else
	if (CHECK_LIMIT(lpObj->m_iDuelUserRequested, MAX_OBJECT)) {
		// ���� ������ ������ ��û�ߴٸ� -> ���濡�� ������ �˸� �� �ʱ�ȭ
		gObjSendDuelEnd(&gObj[lpObj->m_iDuelUserRequested]);
	}
	
	if (gObjDuelCheck(lpObj)) {		
		// ���� ���� ���̾��ٸ� -> ���濡�� ������ �˸� �� �ʱ�ȭ
		
#ifdef WORLD_TOURNAMENT_EVENT_SETTING		
		gObj[lpObj->m_iDuelUser].m_btDuelScore = max(gObj[lpObj->m_iDuelUser].m_btDuelScore, lpObj->m_btDuelScore+1);
#endif	
		gObjSendDuelEnd(&gObj[lpObj->m_iDuelUser]);
	}
	#endif // ADD_NEWPVP_PKFIELD
	gObjResetDuel(lpObj);

#endif // DUEL_SYSTEM_20031028

#ifdef MODIFY_GAMEGUARD_NOT_AUTH_MAPSERVER_MOVE_20060513
	lpObj->m_bCheckGGAuth = true;
#endif

	// ���� ��Ƽ�� �����߾��ٸ�..
	if( lpObj->PartyNumber >= 0 )
	{
		// ��Ƽ�� ���� ����ȭ�� ���� ���� �߻� ���ɼ� ����. (apple)
		// PartyClass::PartyMemberLifeSend() �Լ� ������ 5,4,3,2,1 ���ῡ ���� ��Ƽ�� ������ ���� ��� ���� �߻�����.
		int pindex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
		if( pindex >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;
			pMsg.Number = pindex;
			CGPartyDelUser( &pMsg, lpObj->m_Index);
		}		
	}

#ifdef __FRIEND_WORK__
	// ��忡 ���ԵǾ� �ִٸ�..
	if( lpObj->GuildNumber > 0 )
	{
		if( lpObj->lpGuild->WarState == 1 )
		{
			gObjGuildWarMasterClose(lpObj);
		}
		GDCharClose(0, lpObj->lpGuild->Name, lpObj->Name);	//#BUGFIX_20040218_01
	}
	else GDCharClose(1, NULL, lpObj->Name);	//#BUGFIX_20040218_01
#endif

	// â�� ����� ����Ǿ��ٸ�...  
	if( lpObj->WarehouseSave == TRUE )
	{
		// 2002/09/09 â�� ����� �̾��ٸ� ������ �����Ų��.
		/*if( lpObj->m_IfState.type == I_WAREHOUSE && 
			lpObj->m_IfState.state == 1 )
		{*/
			GDSetWarehouseList(lpObj->m_Index);
		//}
	}	

#ifdef FOR_BLOODCASTLE
	if (CHECK_BLOODCASTLE(lpObj->MapNumber)) {					// DB�� �����͸� �����ϱ� ������ �������� ��ӽ�Ų��.
		g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
	}
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// DB�� �����͸� �����ϱ� ������ ȯ�� ����ϰ�� ���� ����
		if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
		{
			g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
			g_IllusionTempleEvent.LeaveIllusionTemple(lpObj->m_Index, lpObj->MapNumber);
		}
#endif

#ifdef MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328
	gObjSaveChaosBoxItemList( lpObj );
#endif // MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ���� - �κ��丮 ���� ��
	if (gObjFixInventoryPointer(aIndex) == false) {
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

#ifdef MODIFY_PERIODITEM_EFFECT_DURING_CLOSETIME_20070416
	#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	g_CashItemPeriodSystem.ClearPeriodItemEffect( lpObj, PERIOD_ITEM_CLEAR_ALL );
	#else
	g_CashItemPeriodSystem.GDReqPeriodItemUpdate( lpObj );
	g_CashItemPeriodSystem.ClearPeriodItemEffect( lpObj );
	#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
#endif // MODIFY_PERIODITEM_EFFECT_DURING_CLOSETIME_20070416

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
	gObjClearBuffEffect( lpObj, CLEAR_TYPE_LOGOUT );
	#else
	g_BuffEffectSlot.ClearEffect( lpObj, CLEAR_TYPE_LOGOUT );
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	// ĳ���� ����
	GJSetCharacterInfo(lpObj, aIndex);
	
	// ȭ�鿡�� ����
	gObjViewportClose(lpObj);
	LogAddTD(lMsg.Get(516), lpObj->m_Index, lpObj->AccountID, lpObj->Name);

#ifdef MODIFY_WHISPER_CASH_BUGFIX_20060704
	if ( lpObj->Name != NULL )
		WhisperCash.DelCash(lpObj->Name);
#endif // MODIFY_WHISPER_CASH_BUGFIX_20060704

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	g_CashShop.DeleteUser( lpObj );
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	if( lpObj->m_iPeriodItemEffectIndex != -1 )
	{
	#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		g_CashItemPeriodSystem.ClearPeriodItemEffect( lpObj, PERIOD_ITEM_CLEAR_ALL );
	#else
		g_CashItemPeriodSystem.GDReqPeriodItemUpdate( lpObj );
		g_CashItemPeriodSystem.ClearPeriodItemEffect( lpObj );
	#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	}
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	
#ifdef LOGINSERVER_MULOG
	memcpy(lpObj->BackName, lpObj->Name, MAX_IDSTRING);
	lpObj->BackName[MAX_IDSTRING] = 0;
#endif
	
	memset(lpObj->Name, 0, MAX_IDSTRING);

	lpObj->Connected = CSS_CHARACTERVIEW;
		
#ifdef UPDATE_RESEND_GETCHARlIST_DISABLE_20090605
	lpObj->m_bRequestPaket		= FALSE;
	lpObj->m_nRequestPacketCnt	= 0;
#endif	// UPDATE_RESEND_GETCHARlIST_DISABLE_20090605

#ifdef UPDATE_UNIFICATIONBILL_20090203
	if(m_ObjBill[aIndex].GetDeductType() > BILL_TYPE_NONE && m_ObjBill[aIndex].GetDeductType() <= BILL_TYPE_COLLOR)
		gLCount[m_ObjBill[aIndex].GetDeductType()].Delete();
#else	// UPDATE_UNIFICATIONBILL_20090203
	if(m_ObjBill[aIndex].GetCertify()>=0&&m_ObjBill[aIndex].GetCertify()<MAX_LOGIN_COUNT)
		gLCount[m_ObjBill[aIndex].GetCertify()].Delete();
#endif	// UPDATE_UNIFICATIONBILL_20090203

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
// ������ ������ �����ϸ� ����Ʈ���� �����Ѵ�.
short gObjDel(int index)
{
	int i=0;
	LPOBJECTSTRUCT	lpObj;
	int UserConnectState;

	if( index < 0 || index > MAX_OBJECT ) {
		LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}
	lpObj = (LPOBJECTSTRUCT)&gObj[index];
	if( lpObj->Connected < 1
#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
		&& lpObj->Connected != CSS_LOGINWAIT
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
		)
		return 0;
	
#ifdef CLEAR_STANDATTR_OBJDEL_20050325
#if ENABLE_MC_CODE == 1
	if (CHECK_LIMIT(lpObj->MapNumber, g_TerrainManager.Size()))
#else // ENABLE_MC_CODE == 1
	if (CHECK_LIMIT(lpObj->MapNumber, MAX_MAP))
#endif // ENABLE_MC_CODE == 1
	{
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->X, lpObj->Y);
	}
#endif
	
	UserConnectState = lpObj->Connected;
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{		
		if(lpObj->m_stateEngagePVP != PVP_USER_NONE)
		{
			g_PvpEvent.DeleteUser(index);
		}

		gObjGameClose(index);
		if( UserConnectState >= CSS_LOGINOK
#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
			|| UserConnectState == CSS_LOGINWAIT
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
			)
		{
			// ���������� ����� ���Ḧ �˸���.		
#ifdef MAP_SERVER_WORK_20041030					// �ʼ����� �̵��� ������ �ƴ� ���� ���������� �α׾ƿ� ����� �˸���.
			if (lpObj->m_bMapSvrMoveQuit == false) {
#endif
#ifdef LOGINSERVER_MULOG
			GJPUserClose(lpObj);				// ���������� ����� ���Ḧ �˸���.
#else
#if defined(CHN_PLAYTIME_LIMIT_SYSTEM_20050818) || defined(VTM_PLAYTIME_LIMIT_SYSTEM_20060626)
			GJPUserClose(lpObj->AccountID, lpObj->m_iPlayTimeLimit);
#else
			GJPUserClose(lpObj->AccountID);		// ���������� ����� ���Ḧ �˸���.
#endif
#endif
			LogAddTD("(%d)logout-send : %s [%s]", index, lpObj->AccountID, lpObj->Ip_addr);

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
			LogAddTD_TempLog(", (%d)logout-send :, Account [%s], IP [%s]", index, lpObj->AccountID, lpObj->Ip_addr);
#endif

#ifdef MAP_SERVER_WORK_20041030
			}
			else {
				LogAddTD("[MapServerMng] User Quit without JoinServer logout-send [%s][%s]",
					lpObj->AccountID,
					lpObj->Name
					);
			}
#endif

			memset(lpObj->AccountID, 0, MAX_IDSTRING);
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
			memset(lpObj->LastJoominNumber, 0, MAX_JOOMINNUMBER_LENGTH);
#else
			memset(lpObj->LastJoominNumber, 0, 14);
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424

		}
		LogAddTD("(%d)logout : %s [%s]", index, lpObj->AccountID, lpObj->Ip_addr);

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
		LogAddTD_TempLog(", (%d)logout :, Account [%s], IP [%s]", index, lpObj->AccountID, lpObj->Ip_addr);
#endif

		IpCache.DelIp(lpObj->Ip_addr);
	}
	lpObj->Connected = 0;

#ifdef MODIFY_0708_BUFFIX_20070820
	lpObj->m_bSkillKeyRecv			= FALSE;
#endif
#ifdef GMCOMMAND_EX
	lpObj->m_bGMSummon				= FALSE;
#endif // GMCOMMAND_EX

	return 1;
}


//////////////////////////////////////////////////////////////////////////////
// ����޽����� ������ �����Ŀ� ������ ������ �����Ų��.
short gObjSometimeClose(int index)
{
	if( gDisconnect == 1 ) return -1;

	if( index < 0 || index > MAX_OBJECT ) 
	{
		LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}
//	gObjAddMsgSendDelay(&gObj[index], 5, index, 1000*5);	// 5�� �Ŀ� �ڵ� �����ϰ� �Ѵ�.
	
	LPOBJECTSTRUCT	lpObj;
	lpObj = (LPOBJECTSTRUCT)&gObj[index];

	gObjTradeSave(lpObj, index);

	if( lpObj->Connected > 1 )
	{
		if( lpObj->Type == OBJTYPE_CHARACTER )	
		{
			if( lpObj->Connected == 3 )
			{
				// ���� ��Ƽ�� �����߾��ٸ�..
				if( lpObj->PartyNumber >= 0 )
				{
					//gParty.Delete(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
					PMSG_PARTYDELUSER pMsg;
					
					int pindex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
					if( pindex >= 0 )
					{
						pMsg.Number = pindex;
						CGPartyDelUser( &pMsg, lpObj->m_Index);
					}		
				}
			}
		}
	}
	return 1;
}

//----------------------------------------------------------------------------
// �����ؼ� ������ ����������?
BOOL gObjIsGamePlaing(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Connected < CSS_GAMEPLAYING ) return FALSE;
	return TRUE;
}

//----------------------------------------------------------------------------
// �����ؼ� ������ ����������?
BOOL gObjIsConnectedGP(int aIndex)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT-1 )	 return FALSE;
	if( gObj[aIndex].Type != OBJTYPE_CHARACTER ) return FALSE;
	if( gObj[aIndex].Connected < CSS_GAMEPLAYING ) return FALSE;
	if( gObj[aIndex].CloseCount > 0 ) return FALSE;
	return TRUE;
}

#ifdef __FRIEND_WORK__
//----------------------------------------------------------------------------
// �����ؼ� ������ �������ΰ�?
BOOL gObjIsConnectedGP(int aIndex, char* CharName)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT-1 )	 return FALSE;
	if( gObj[aIndex].Type != OBJTYPE_CHARACTER ) return FALSE;
	if( gObj[aIndex].Connected < CSS_GAMEPLAYING ) return FALSE;
	if( gObj[aIndex].CloseCount > 0 ) return FALSE;
	if( strcmp(gObj[aIndex].Name, CharName) != 0 ) return FALSE;
	return TRUE;
}

#endif


//----------------------------------------------------------------------------
// �����ؼ� ������ ����������?
BOOL gObjIsConnected(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;
	if( lpObj->Connected < CSS_GAMEPLAYING ) return FALSE;
	if( lpObj->CloseCount > 0 ) return FALSE;
	return TRUE;
}

//----------------------------------------------------------------------------
// ������ �ִ��� �˻�
BOOL gObjIsConnected(int aIndex)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return FALSE;
	if( gObj[aIndex].Connected < CSS_GAMEPLAYING ) return FALSE;
	return TRUE;
}

//----------------------------------------------------------------------------
// ������ �ִ��� �˻� (DB ��ȣ���� ������ �˻�)
BOOL gObjIsConnected(LPOBJECTSTRUCT lpObj, int dbnumber)
{
	if( lpObj->Connected < CSS_GAMEPLAYING ) return FALSE;
	if( lpObj->DBNumber != dbnumber ) return FALSE;
	return TRUE;
}

#ifdef ADD_NEWPVP_PKFIELD
BOOL gObjIsConnectedEx(int aIndex)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return FALSE;
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;
	if( lpObj->Connected < CSS_GAMEPLAYING ) return FALSE;
	if( lpObj->CloseCount >= 0 ) return FALSE;
	return TRUE;
}
#endif // ADD_NEWPVP_PKFIELD

//----------------------------------------------------------------------------
// üũ�ϰ��� �ϴ� �ε���/���� ���̵� �´���
BOOL gObjIsAccontConnect(int aIndex, char *accountid)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return FALSE;
	if( gObj[aIndex].Connected < 1 ) return FALSE;
	if( strlen( gObj[aIndex].AccountID ) < 1 ) return FALSE;
	if( strcmp(accountid, gObj[aIndex].AccountID) != 0 ) return FALSE;
	return TRUE;
}

// �ֹε�� ��ȣ�� ������ ���� ���� üũ�Ѵ�.
BOOL gObjJoominCheck(int aIndex, char *szInJN)
{
	if(g_GlobalConfig.m_bPersonalID == false)
		return TRUE;

	LPOBJECTSTRUCT lpObj;

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	switch( gLanguage )
	{
	case 1 :
	case 5 :		// ����, �±�
#ifdef MODIFY_JOOMINCHECK_FOR_USA
	case 8:			// �̱�(K2)
#endif	
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
		LogAddTD("[%s][%s][%s]",lpObj->AccountID, szInJN, lpObj->LastJoominNumber);
		if( strncmp( szInJN, lpObj->LastJoominNumber, MAX_JOOMINNUMBER_LENGTH) == 0 )
		{
			return TRUE;
		}
		break;
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
#ifdef MODIFY_GAMEPASSWORD_20041126
	case 6 :		// �ʸ���
#endif
		if( strncmp( szInJN, lpObj->LastJoominNumber, 7) == 0 )
		{
			return TRUE;
		}

	case 2 :		// �Ϻ�		
		if( strncmp( szInJN, lpObj->LastJoominNumber+5, 8) == 0 )
		{
			return TRUE;
		}

#ifdef BUGFIX_X_RESIDENT_REGISTRATION_OF_USER_20050419	
	case 3 : // �߱�
		if( _stricmp(szInJN, lpObj->LastJoominNumber+6) == 0 )
		{
			return TRUE;
		}
		break;

#endif // BUGFIX_X_RESIDENT_REGISTRATION_OF_USER_20050419
		
	case 4 :		// �븸

		//�系 �׽�Ʈ�� ��� �ֹι�ȣ üũ ���Ѵ�.
#if !defined(AUTH_GAMESERVER) && (defined(TESTSERVER) == 1)

		return TRUE;
#endif

		if( strlen( szInJN ) < 2 )
		{
			return FALSE;
		}
		// ù�ڰ� �������ΰ�?
		if( (szInJN[0] >= 'A' && szInJN[0] <= 'Z') || 
			(szInJN[0] >= 'a' && szInJN[0] <= 'z') )
		{
			// �빮�� �϶��� �ҹ��ڷ� 
			if( szInJN[0] >= 'A' && szInJN[0] <= 'Z' ) 
			{
				szInJN[0] = 'a'+(szInJN[0]-'A');
			}
			if( lpObj->LastJoominNumber[0] >= 'A' && lpObj->LastJoominNumber[0] <= 'Z' ) 
			{
				lpObj->LastJoominNumber[0] = 'a'+(lpObj->LastJoominNumber[0]-'A');
			}

			// ���ؼ� ���ٸ� ����
			if( strncmp(lpObj->LastJoominNumber, szInJN, 10) == 0 )
			{
				return TRUE;
			}
		}
		break;
	default :
#if TESTSERVER == 1
		return TRUE;
#endif 
		if( strncmp( szInJN, lpObj->LastJoominNumber+6, 7) == 0 )
		{
			return TRUE;
		}
		break;
	}	
	return FALSE;
}

BOOL gObjTaiwanJoominCheck(int aIndex, char *szInJN)
{
	LPOBJECTSTRUCT lpObj;

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	
	if( strlen( szInJN ) < 2 )
	{
		return FALSE;
	}	
	// ���ؼ� ���ٸ� ����
	if( strncmp(lpObj->LastJoominNumber+1, szInJN, 9) == 0 )
	{
		return TRUE;
	}	
	return FALSE;
}


void CGTeleportRecv(LPPMSG_TELEPORT lpMsg, int aIndex);

// ĳ���Ͱ� ���ִ� ���� üũ�ؼ� ������ ���̶�� ���ų�..�̵���Ų��.

#ifdef ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131
BOOL gObjCheckXYMapTile(LPOBJECTSTRUCT lpObj, int iDbgName )
#else
BOOL gObjCheckXYMapTile(LPOBJECTSTRUCT lpObj)
#endif
{	
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
		return FALSE;

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�������� ��� ���̶�� �̻����� üũ�� ������ ���� �ʴ´�. (�̻� ������ ī����ĳ�� ���� ��ƾ�� üũ�Ѵ�.)
	if (CHECK_CHAOSCASTLE(lpObj->MapNumber) && 
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ�� ��ȣ ���ϱ�
		(g_ChaosCastle.GetCurrentState( g_ChaosCastle.GetChaosCastleByMapNum( lpObj->MapNumber ) ) == CHAOSCASTLE_STATE_PLAYING)
#else
		(g_ChaosCastle.GetCurrentState(lpObj->MapNumber - MAP_INDEX_CHAOSCASTLE1) == CHAOSCASTLE_STATE_PLAYING)
#endif
		)
	{
		return FALSE;
	}
#endif

	if(lpObj->m_stateEngagePVP == PVP_USER_PLAYING)
		return FALSE;
	
#if( SPEEDHACK_CHECK == 0 ) 
		return FALSE;
#endif

#if (TESTSERVER == 1 )
	#ifndef MODIFY_FORCEMOVE_TO_LORENCIA_20060515	// not define : �系�׼��̶� üũ
		return FALSE;
	#endif
#endif

	int x, y, mapnumber;
	
	x			= lpObj->X;
	y			= lpObj->Y;
	mapnumber	= lpObj->MapNumber;

#if ENABLE_MC_CODE == 1
	if (mapnumber > g_TerrainManager.Size() - 1) mapnumber = 0;
	int mapminlevel = g_TerrainManager.GetMinLevel(mapnumber);
#else // ENABLE_MC_CODE == 1
	if (mapnumber > MAX_MAP - 1) mapnumber = 0;
	int mapminlevel = gMoveCommand.GetMoveLevel(mapnumber);
#endif // ENABLE_MC_CODE == 1

#ifdef MODIFY_CHECK_MOVELEVEL_20060731
	// ��� ���� �ּ� �䱸 ������ 0���� �����Ѵ�.
	// MapMinUserLevel ���� movereq.txt, gate.txt, MoveLevel.txt ���ϰ� 
	// ������ ���� �ʱ� ������, �̵����� ������ �ϵ��ڵ��� �ؾ��ϴ� ������ �߻�
	// �ֱ������� �� �ּ� ���� üũ�� ���� �ʴ� ������ ����.
	mapminlevel = 0;
#endif

#ifdef DARKLORD_WORK
	if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )	
#else
	if( lpObj->Class == CLASS_MAGUMSA )	// ���˻��϶��� 2/3 ������..
#endif
	{
		if( mapminlevel > 0 )
		{
#if ENABLE_MC_CODE == 1
			int minlevel = g_TerrainManager.GetMinLevel(mapnumber);
#else // ENABLE_MC_CODE == 1
			int minlevel = gMoveCommand.GetMoveLevel(mapnumber);
#endif // ENABLE_MC_CODE == 1
			if( minlevel > 0 )
			{
				mapminlevel = minlevel-((minlevel/3)*2);
			}
		}
	}
	
	// ������ ���� 6�����ε� �����ʿ� ���ٸ�..
	if( lpObj->Class == CLASS_ELF && lpObj->Level < 10 )	
	{
		if( lpObj->MapNumber != 3 )
		{
			PMSG_TELEPORT pMsg;
			LogAddTD(lMsg.Get(517), lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			pMsg.MoveNumber = 27;
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
			//CGTeleportRecv(&pMsg, lpObj->m_Index);
		}
	}
	else if( mapminlevel > lpObj->Level ) // �� �ܿ� ��� üũ�ؼ� ������.. ���´�.
	{			
		// ������ ���� 6���� ������ �����ʿ� ������ �ִ�.
		// �׷��� �ʰ� ������ �ƴϰ� ���� 6���� ũ�ٸ�.. üũ�ؼ� �̵���Ų��.
			LogAddTD(lMsg.Get(517), lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			PMSG_TELEPORT pMsg;
			pMsg.MoveNumber = 17;
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);

			//CGTeleportRecv(&pMsg, lpObj->m_Index);
	}
	
	BYTE attr =  MapC[mapnumber].GetAttr(x, y);

	if( (attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK || 
		(attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW
		)	// ������ ���� ���� ���������.
	{
		BYTE attr1 = MapC[mapnumber].GetAttr(x+2, y);
		BYTE attr2 = MapC[mapnumber].GetAttr(  x, y+2);
		BYTE attr3 = MapC[mapnumber].GetAttr(x-2, y);
		BYTE attr4 = MapC[mapnumber].GetAttr(  x, y-2);
		
		if( (attr1>MAP_ATTR_SAFTYZONE) && (attr2>MAP_ATTR_SAFTYZONE) &&
			(attr3>MAP_ATTR_SAFTYZONE) && (attr4>MAP_ATTR_SAFTYZONE) )
		{

#ifdef ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131
			LogAddTD("[ CHECK POSITION ] DbgName[%d] [%s][%s] Map[%d]-(%d,%d) Invalid location causes to force to move", 
					iDbgName, lpObj->AccountID, 
					lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y );
#else
			LogAddTD(lMsg.Get(517), lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
#endif
			
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013		// ��������� ��ǥ�̻��� ���� �ش� �� ���� ��ġ�� �̵�
			if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
				&& g_IllusionTempleEvent.GetIllusionTempleState(lpObj->MapNumber) == ILLUSION_TEMPLE_STATE_PLAYING)
			{
				BYTE btTeam = g_IllusionTempleEvent.GetUserTeam(lpObj->MapNumber, lpObj->m_Index);
				PMSG_TELEPORT pMsg;
				if( btTeam == 0xFF)
				{
					pMsg.MoveNumber = 17;	// �� �̻�� �η��þƷ�
				}
				else
				{
					if(btTeam == TEAM_ALLIED)
					{	
						pMsg.MoveNumber = GATE_ILLUSION_TEMPLE_ALLIED1 + (lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1);
					}
					else if(btTeam == TEAM_ILLUSION)
					{
						pMsg.MoveNumber = GATE_ILLUSION_TEMPLE_ILLUSION1 + (lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1);
					}
					else
					{
						pMsg.MoveNumber = 17;	// �� �̻�� �η��þƷ�
					}
				}
				
				gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);

				return TRUE;
			}
#endif	// MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013

			{
			PMSG_TELEPORT pMsg;
			pMsg.MoveNumber = 17;
			
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);

			//CGTeleportRecv(&pMsg, lpObj->m_Index);
			}
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
// ��������� ����Ѵ�.
//----------------------------------------------------------------------------
#ifdef ADD_PROTOCOL_PCBANG_GUID	

	#ifdef PCBANG_ADVANTAGE  // happycat@20050201 (PC�� ����)
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, int aPCBangGuid, char *JoominNumber, BYTE	bPCBangUser)
	#else
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, int aPCBangGuid, char *JoominNumber)
	#endif//PCBANG_ADVANTAGE

#else//ADD_PROTOCOL_PCBANG_GUID
	#if defined(PCBANG_ADVANTAGE) && defined(FOR_TAIWAN)
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, int aPCBangGuid, char *JoominNumber, BYTE	bPCBangUser)
	#else
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, char *JoominNumber)
	#endif
#endif
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) ) 
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}

#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
	if( gObj[aIndex].Connected != CSS_LOGIN && gObj[aIndex].Connected != CSS_LOGINWAIT )
#else
	if( gObj[aIndex].Connected != CSS_LOGIN )
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
	{
		LogAddTD(lMsg.Get(518), aIndex, szId, aDBNumber, __FILE__, __LINE__);
		return FALSE;
	}
	// ������ ��û�� ����ڰ� �´��� �� Ȯ�� �Ѵ�.
	if( strcmp(gObj[aIndex].AccountID, szId) != 0 )
	{
		LogAdd(lMsg.Get(519), szId);
		return FALSE;
	}

	gObj[aIndex].UserNumber = aUserNumber;
	gObj[aIndex].DBNumber   = aDBNumber;
	gObj[aIndex].Connected  = CSS_LOGINOK;
	strcpy(gObj[aIndex].LastJoominNumber, JoominNumber);


#ifdef ADD_PROTOCOL_PCBANG_GUID	
	gObj[aIndex].PcBangGuid   = aPCBangGuid;
#endif

#ifdef PCBANG_ADVANTAGE //happycat@20050201 (PC�� ����)
	// 20060918 m_bPCBangUser�� �ǹ̴� PC�� �������� �ƴ��� Ȯ�� �뵵�� ����Ѵ�.!
	// ���� ������ PCBANG_ADVANTAGE_EXPAND_20060919 �� �̿��ؼ� �и���
	gObj[aIndex].m_bPCBangUser = bPCBangUser; 

	#ifdef FOR_TAIWAN
		gObj[aIndex].m_bPCBangUser = bPCBangUser;

		//PC�� ����� Ȯ�� �α� �߰�(riverstyx)
		if (gObj[aIndex].m_bPCBangUser == TRUE) 
		{
			LogAddTD("[PCBANG_ADVANTAGE][PCBang User Connect] [%s] - %s",
				gObj[aIndex].AccountID, gObj[aIndex].Ip_addr);
		}

	#endif
#endif

#ifdef PCBANG_FREE_KALIMA_EVENT_20060124
	gObj[aIndex].m_bPCBangCouponUser = bPCBangUser;

	#if defined(PCBANG_ADVANTAGE_EXPAND_20060919) && defined(FOR_KOREA)
		// �ѱ��� PC�� ���� �������� �ʴ´�.
		g_iApplyPCBangAdvantage = 0;
	#endif 

	if( gObj[aIndex].m_bPCBangCouponUser )
	{
		LogAdd("[Coupon Event][PCBang User Connect] [%s][%s] - %s",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr);
	}
#endif

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	gObj[aIndex].m_bPCBangFreeChaosCastleUser	= bPCBangUser;
	if( gObj[aIndex].m_bPCBangFreeChaosCastleUser )
	{
		LogAdd( "[FREE CHAOSCASTLE EVENT][PCBang User Connect] [%s] - %s",
			gObj[aIndex].AccountID, gObj[aIndex].Ip_addr );
	}
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206		// PC�� ����� ����
	g_PCBangPointSystem.SetPCBangPointUser( aIndex, bPCBangUser );
#endif

#ifdef MODIFY_CASTLE_MAP_MOVE_TRACE_LOG_20080418
	LogAddTD("[BUGTRACE][gObjSetAccountLogin] (%d), Id : %s, Name : %s Login Success",
		aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name );
#endif // MODIFY_CASTLE_MAP_MOVE_TRACE_LOG_20080418

	return TRUE;
}

int gObjGetUserNumber(int aIndex)
{
	int usernumber;

	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return 0;
	}
	usernumber = gObj[aIndex].UserNumber;
	
	return usernumber;
}

//----------------------------------------------------------------------------
// ���� ��ȣ�� DB��ȣ�� �Ѱ��ش�.
//----------------------------------------------------------------------------
int gObjGetNumber(int aIndex, int & UN, int & DBN)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT-1 )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}
	UN = gObj[aIndex].UserNumber;
	DBN= gObj[aIndex].DBNumber;
	
	return 1;
}

// ���̵� �����Ѵ�.
char *gObjGetAccountId(int aIndex)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT-1 )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return NULL;
	}
	if( gObj[aIndex].Connected == CSS_LOGINOK ) // ������ ���� �����ϋ��� ���̵� �����Ѵ�.
	{
		return gObj[aIndex].AccountID;
	}
	return NULL;
}


//----------------------------------------------------------------------------
// ���� ���̵�� �ε����� ã�´�.
//----------------------------------------------------------------------------
int gObjGetIndex(char *szId)
{
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected >= CSS_GAMEPLAYING )
		{
			if( gObj[n].Name[0] == szId[0] )
			{
				if( strcmp(gObj[n].Name, szId) == 0 )
				{
					return n;
				}
			}
		}
	}
	return -1;
}

// �ش� �ε����� ����ڰ� �´���, ���� ������ üũ�Ѵ�.
BOOL gObjUserIdConnectCheck(char *szId, int index)
{
	if( index < 0 || index > MAX_OBJECT ) {
		LogAdd("error : index error !! %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if( gObj[index].Connected < CSS_GAMEPLAYING ) return FALSE;
	if( gObj[index].Name[0] != szId[0] )		return FALSE;
	if( strcmp(gObj[index].Name, szId) == 0 )	return TRUE;
	
	return FALSE;
}

// �����ǥ�� ������ ��Ŷ���� ���� ����� ��ġ�� ��´�.
BYTE GetPathPacketDirPos(int px, int py)
{
	BYTE pos=0;

	if( px <= -1 && py <= -1 ) { pos = 0;}
	else if( px <= -1 && py ==  0 ) { pos = 7; }
	else if( px <= -1 && py >=  1 ) { pos = 6; }
	else if( px ==  0 && py <= -1 ) { pos = 1; }
	else if( px ==  0 && py >=  1 ) { pos = 5; }
	else if( px >=  1 && py <= -1 ) { pos = 2; }
	else if( px >=  1 && py ==  0 ) { pos = 3; }
	else if( px >=  1 && py >=  1 ) { pos = 4; }

	return (BYTE)pos;
}


// �Ÿ� ���..
int gObjCalDistance(LPOBJECTSTRUCT lpObj1, LPOBJECTSTRUCT lpObj2)
{
	if( (lpObj1->X == lpObj2->X) && 
		(lpObj1->Y == lpObj2->Y) )
	{
		return 0;
	}
	float tx = (float)(lpObj1->X-lpObj2->X);
	float ty = (float)(lpObj1->Y-lpObj2->Y);
			
	return (int)(sqrt((tx*tx)+(ty*ty)));
}

// ��ġ�� üũ�ؼ� �������� �ʹ� ���� �̵����� ��쿣 ó������ �ʴ´�.
// ���������� ��ǥ ����
BOOL gObjPositionCheck(LPOBJECTSTRUCT lpObj)
{
	int ix = lpObj->m_OldX-lpObj->TX;
	int iy = lpObj->m_OldY-lpObj->TY;
		
	if( ix <= -15 || ix >= 15 ) return FALSE;
	if( iy <= -15 || iy >= 15 ) return FALSE;

	return TRUE;
}

// �ش� ��ġ�� ��ǥ�� �簢���� ���� ��� ������ Ȯ���Ѵ� 
BOOL gObjCheckTileArea(int aIndex, int x, int y, int dis)
{
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
			// �ϴ� �����ϰ� �簢 üũ��..

	if( (x < (lpObj->X-dis)) || (x > (lpObj->X+dis)) ||
		(y < (lpObj->Y-dis)) || (y > (lpObj->Y+dis)) )
	{
		return FALSE;
	}
	return TRUE;
}

// x, y �� w, h ���� �������� ���ԵǾ� �ִ°�?
// ���ԵǾ� ������ TRUE
// ������ FALSE
BOOL ExtentCheck(int x, int y, int w, int h)
{
	if( x >= 0 && x < w &&
		y >= 0 && y < h )
	{
		return TRUE;
	}
	return FALSE;
}

void gObjSetInventory1Pointer(LPOBJECTSTRUCT lpObj)
{
	lpObj->pInventory		= (CItem*)lpObj->Inventory1;
	lpObj->pInventoryMap	= lpObj->InventoryMap1;
	lpObj->pInventoryCount	= (char*)&lpObj->InventoryCount1;
}

void gObjSetInventory2Pointer(LPOBJECTSTRUCT lpObj)
{
	lpObj->pInventory		= (CItem*)lpObj->Inventory2;
	lpObj->pInventoryMap	= lpObj->InventoryMap2;
	lpObj->pInventoryCount	= (char*)&lpObj->InventoryCount2;
}


void gObjAddMsgSend(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode)
{
	for( int n=0; n<MAXMESSAGE; n++)
	{
		if( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser= aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			return;
		}
	}

	for( int n=0; n<MAXMESSAGE; n++)
	{	// �޽��� ���۰� �� á���� ū ���� ���� �͵� �߿��� ����Ѵ�
		if( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || 
			gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser= aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			return;
		}
	}	
}

void gObjAddMsgSendDelay(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode)
{
	for( int n=0; n<MAXMESSAGE; n++)
	{
		if( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount()+delay;
			gSMMsg[lpObj->m_Index][n].SendUser= aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			return;
		}
	}

	for( int n=0; n<MAXMESSAGE; n++)
	{	// �޽��� ���۰� �� á���� ū ���� ���� �͵� �߿��� ����Ѵ�
		if( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || 
			gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser= aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			return;
		}
	}	
}

#ifdef ADD_QPROCESSING_STATE_OF_DEAD_20050221
// ������ �޽��� ť�� ������Ʈ�� �߰��Ѵ�.
void gObjAddMsgSendDelayInSpecificQPos( LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition )
{
	if( iQPosition >= 0 && iQPosition < MAXMESSAGE )
	{
		gSMMsg[lpObj->m_Index][iQPosition].MsgCode = aMsgCode;
		gSMMsg[lpObj->m_Index][iQPosition].MsgTime = GetTickCount();
		gSMMsg[lpObj->m_Index][iQPosition].SendUser= aIndex;
		gSMMsg[lpObj->m_Index][iQPosition].SubCode = SubCode;
	}
}
#endif

#ifdef ADD_SKILL_WITH_COMBO
void gObjAddAttackProcMsgSendDelay(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2)
{
	for( int n=0; n<MAXATTACKMESSAGE; n++)
	{
		int aChgIndex = lpObj->m_Index-ALLOC_USEROBJECTSTART;

		if( gSMAttackProcMsg[aChgIndex][n].MsgCode < 0 )
		{
			gSMAttackProcMsg[aChgIndex][n].MsgCode = aMsgCode;
			gSMAttackProcMsg[aChgIndex][n].MsgTime = GetTickCount()+delay;
			gSMAttackProcMsg[aChgIndex][n].SendUser= aIndex;
			gSMAttackProcMsg[aChgIndex][n].SubCode = SubCode;
			gSMAttackProcMsg[aChgIndex][n].SubCode2 = SubCode2;	
			return;
		}
	}	
}
#elif defined(NEW_ATTACK_PROC)
void gObjAddAttackProcMsgSendDelay(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2)
{
	for( int n=0; n<MAXATTACKMESSAGE; n++)
	{
		int aChgIndex = lpObj->m_Index-ALLOC_USEROBJECTSTART;

		if( gSMAttackProcMsg[aChgIndex][n].MsgCode < 0 )
		{
			gSMAttackProcMsg[aChgIndex][n].MsgCode = aMsgCode;
			gSMAttackProcMsg[aChgIndex][n].MsgTime = GetTickCount()+delay;
			gSMAttackProcMsg[aChgIndex][n].SendUser= aIndex;
			gSMAttackProcMsg[aChgIndex][n].SubCode = SubCode;
			gSMAttackProcMsg[aChgIndex][n].SubCode2 = SubCode2;	
			return;
		}
	}	
}
#endif

// ���¸� ����� ó���Ѵ�.
void gObjMsgProc(LPOBJECTSTRUCT lpObj)
{
	int aIndex;

	aIndex = lpObj->m_Index;
	for( int n=0; n<MAXMESSAGE; n++)
	{
		if( gSMMsg[aIndex][n].MsgCode >= 0 )
		{
			if( GetTickCount() > gSMMsg[aIndex][n].MsgTime )	// �ش� �޽����� ����� �ð��� �Ǿ��ٸ�
			{
				if( lpObj->Type == OBJTYPE_MONSTER || lpObj->Type == OBJTYPE_NPC)

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
					gObjMonsterStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
#else
					gObjMonsterStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser);
#endif

				else gObjStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
				gSMMsg[aIndex][n].MsgCode = -1;
			}
		}
	}		
}

void gObjStateProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode)
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT)  )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return;
	}
	
	switch( aMsgCode )
	{
	case MSG_BEATTACK :		// ���ݴ��ߴ�.
		gObjBackSpring(lpObj, &gObj[aIndex]);
		break;
	case MSG_FILLENERGY :	// �������� ä���ش�.
		gObjMonsterDieLifePlus(lpObj, &gObj[aIndex]);
		break;
	case MSG_BILLSEND :		// ���� ������ 
		gObjBillRequest(lpObj);
		break;
#ifdef _NEW_EXDB_
	case MSG_GUILDWAREND :	// ��� ������ ���� �� 5�� �Ŀ� ���� �Ѵ�.
		if( lpObj->lpGuild != NULL && lpObj->lpGuild->lpTargetGuildNode != NULL )
		{
			gObjGuildWarEnd(lpObj->lpGuild, lpObj->lpGuild->lpTargetGuildNode);
		}
		break;
#endif
	case MSG_BSSTART :		// ��Ʋ ��Ŀ�� �����Ѵ�.
		if( lpObj->lpGuild && lpObj->lpGuild->WarState && lpObj->lpGuild->WarType == 1)
		{
			BattleSoccerGoalStart(0);
		}
		break;
	case MSG_BSSTOP :
		if( lpObj->lpGuild && lpObj->lpGuild->WarState && lpObj->lpGuild->WarType == 1)
		{
			if( lpObj->lpGuild->BattleGroundIndex >= 0 )
				//BattleSoccerGoalEnd(lpObj->lpGuild->BattleGroundIndex);
				GCManagerGuildWarEnd(lpObj->lpGuild->Name);
		}
		break;
	case MSG_REFLECT :		// �ݻ絥���� ?
		if( lpObj->DamageReflect )
		{
			gObjAttack(lpObj, &gObj[aIndex], NULL, 0, 0, SubCode);			
		}
		break;
#ifdef NEW_FORSKYLAND2
	case MSG_LIFECHECK :	// ����üũ
		{
		}
		break;
#endif
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���̾� ��ũ�� ���� ������ ����
	case MSG_ATTACKPROC_FIRESCREAM:				// ���̾� ��ũ�� ���� ������
		{
			if( lpObj->Live )
			{
				CMagicInf*	lpMagic = &DefMagicInf[AT_SKILL_EXPLOSION];				
								
				// Ÿ�ٿ� ���� �� ������ ȿ���� �ʿ��ϱ� ����.
				gObjAttack( lpObj, &gObj[aIndex], lpMagic, 1, 0, SubCode );				
			}
		}
		break;
#endif
#ifdef ADD_THIRD_WING_20070525	// 3�� ���� �����ɼ� ó��
	case MSG_ATTACK_TURNING:	// �� ���ݷ� ������
		gObjAttack( lpObj, &gObj[aIndex], NULL, 0, 0, SubCode );	
		break;
	case MSG_FULL_LIFE:			// 100% ���� ����
#ifdef MODIFY_THIRD_WING_OPTION_5_PERCENT_REFILL_ALL_BUGFIX_20080624
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
		GCReFillSend( lpObj->m_Index, ( WORD )lpObj->Life, 0xFF, 0, ( WORD )lpObj->iShield );
#else
		lpObj->Life = lpObj->MaxLife;
		GCReFillSend( lpObj->m_Index, ( WORD )lpObj->Life, 0xFF, 0, ( WORD )lpObj->iShield );
#endif // MODIFY_THIRD_WING_OPTION_5_PERCENT_REFILL_ALL_BUGFIX_20080624
		break;
	case MSG_FULL_MANA:			// 100% ���� ����
#ifdef MODIFY_THIRD_WING_OPTION_5_PERCENT_REFILL_ALL_BUGFIX_20080624
		lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		GCManaSend( lpObj->m_Index, ( short )lpObj->Mana, 0xFF, 0, lpObj->BP );
#else
		lpObj->Mana = lpObj->MaxMana;
		GCManaSend( lpObj->m_Index, ( short )lpObj->Mana, 0xFF, 0, lpObj->BP );
#endif // MODIFY_THIRD_WING_OPTION_5_PERCENT_REFILL_ALL_BUGFIX_20080624
		break;
#endif	// ADD_THIRD_WING_20070525	

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// LighingOrb�� ���� Splash Damage ó��
	case MSG_ATTACK_LIGHTNINGORB:
		{
			if( lpObj->Live )
			{
				CMagicInf*	lpMagic = &DefMagicInf[AT_SKILL_LIGHTNINGORB];				
								
				// Ÿ�ٿ� ���� �� ������ ȿ���� �ʿ��ϱ� ����.
				gObjAttack( lpObj, &gObj[aIndex], lpMagic, 1, 0, SubCode );				
			}
		}
		break;
#endif

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
	case MSG_SKILL_PULLUTION1:
		{	
			if( lpObj->Live )
			{
				BYTE PosX	= LOBYTE(LOWORD(SubCode));
				BYTE PosY	= HIBYTE(LOWORD(SubCode));
				BYTE Msg	= HIWORD(SubCode);

				CMagicInf*	lpMagic = &DefMagicInf[AT_SKILL_PULLUTION];
				gObjUseSkill.UseSkillPollution(lpObj->m_Index, lpMagic, PosX, PosY, aIndex, Msg+1);
			}
		}
		break;
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
	}
}


void gObjStateAttackProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2)
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT)  )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return;
	}
	
	switch( aMsgCode )
	{
	case MSG_ATTACKPROC_ENTIREMAGIC :			// ���ݴ��ߴ�.
		{	
			if( lpObj->Live )
			{
				CMagicInf *		lpMagic;
				lpMagic = gObjGetMagicSearch(lpObj, SubCode);		
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 0, 1, 0, SubCode2);				
			}			
		}

		break;

#ifdef ADD_ITEM_FENRIR_01_20051110
	case MSG_ATTACKPROC_FENRIR :
		{
			if( lpObj->Live )
			{
				CMagicInf *		lpMagic;
				lpMagic = gObjGetMagicSearch(lpObj, SubCode);
				
				// ������ ��� ������ �˸���.
				// �ֳ��ϸ� �渱�� ���� ��ų�� ��� Ÿ�ٿ� ���� �� ������ ȿ���� �ʿ��ϱ� �����̴�.
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 1 );		
			}
		}
		break;

#endif

#ifdef DARKLORD_WORK
	case MSG_ATTACKPROC_DARKSPRITATTACK :		// ��ũ���Ǹ� ����

#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
		gDarkSpirit[lpObj->m_Index].Attack(lpObj, &gObj[aIndex], NULL, SubCode, SubCode2);
#else
		gDarkSpirit[lpObj->m_Index].Attack(lpObj, &gObj[aIndex], NULL, SubCode);		
#endif // MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
		
	break;
#endif // DARKLORD_WORK

	}
}


// ������Ʈ(�����,����) �ڷ� �ñ��.. (�¾�����) (���� ������ �����)
BOOL gObjBackSpring(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{	
	int tdir;

#if ENABLE_MC_CODE == 1
	if (!CHECK_LIMIT(lpObj->MapNumber, g_TerrainManager.Size()))
#else // ENABLE_MC_CODE == 1
	if (!CHECK_LIMIT(lpObj->MapNumber, MAX_MAP))
#endif // ENABLE_MC_CODE == 1
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}

	if( lpObj->Type == OBJTYPE_CHARACTER )
	{	// �̵����� ���� �ð��� ƨ���� �ʴ´�
		if( lpObj->m_LastTeleportTime > 0 )
		{
			return FALSE;
		}
	}

	if(g_GlobalConfig.m_bBackSpring == false)
		return FALSE;

#ifdef CRYWOLF_NPC_WORK_20050912
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN )
		{
			return FALSE;
		}
	}
#endif

#ifdef FOR_BLOODCASTLE
	if(lpObj->Class >= 131 && lpObj->Class <= 134 )		// ����, ������ �и��� �ȵȴ�
		return FALSE;
#endif
	
#ifdef CASTLE_MAIN_SCHEDULER_20041111					// ������ NPC - ����, ���� ���� �и��� �ȵȴ�
	if ((lpObj->Class == 277)||			// ����
		(lpObj->Class == 283)||			// ��ȣ����
		(lpObj->Class == 288)||			// ĳ��Ÿ��
		(lpObj->Class == 278)||			// ����������
		(lpObj->Class == 215)||			// ��
		(lpObj->Class == 216)||			// �հ�
		(lpObj->Class == 217)||			// �հ�����ġ
		(lpObj->Class == 218)||			// �հ�����ġ
		(lpObj->Class == 219)			// ��������
		)
	{
		return FALSE;
	}

#ifdef MODIFY_CASTLESIEGE_GUILDMASTER_BACKSPRING_20051018
	// ���� ��� ���̰� ��ũȣ���� ������ ��ũ�ε�� �з����� �ʴ´�.
	if( g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index )
	{
		if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,4) )
		{
			return FALSE;
		}

	}
#endif // MODIFY_CASTLESIEGE_GUILDMASTER_BACKSPRING_20051018

#endif // CASTLE_MAIN_SCHEDULER_20041111


#ifdef CRYWOLF_NPC_WORK_20050912
	// ������ ���ܿ� ��ϵ� ������ �з����� �ʴ´�.
	if( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_START
		|| g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_READY )
	{
		if( lpObj->Type == OBJTYPE_CHARACTER )
		{		
			for( int i=CRYWOLF_NPC_ALTAR1; i<=CRYWOLF_NPC_ALTAR5; i++ )
			{
				int iUserIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(i);
				if( iUserIndex != _INVALID && iUserIndex == lpObj->m_Index )
				{
					return FALSE;
				}
					
			}
		}
		
	}
#endif // CRYWOLF_NPC_WORK_20050912


#ifdef CRYWOLF_NPC_WORK_20050912
	if(	CHECK_CRYWOLF_NPC_ALTAR(lpObj->Class)			// ������ ����
		|| CHECK_CRYWOLF_NPC_STATUE(lpObj->Class)		// ������ �Ż�
	  )
	{
		return FALSE;
	}
#endif // CRYWOLF_NPC_WORK_20050912

	
#ifdef CRYWOLF_MONSTER_WORK_20050914
	if( lpObj->Class == CRYWOLF_MON_TANKER )
	{
		return FALSE;
	}
#endif // CRYWOLF_MONSTER_WORK_20050914
	
#ifdef MODIFY_0708_BUFFIX_20070820	// ����� �и��� ����
	if( lpObj->Class == 275)
	{
		return FALSE;
	}
#endif	// MODIFY_0708_BUFFIX_20070820

#ifdef ADD_RAKLION_20080408			// ��Ŭ���� ������, �Ź̾��� �и��� ����
	if( lpObj->Class == 459 || lpObj->Class == 460 || lpObj->Class == 461 || lpObj->Class == 462 )
	{
		return FALSE;
	}
#endif // ADD_RAKLION_20080408
	
#ifdef MODIFY_BACK_SPRING_SKILL_BUGFIX_20080716		// �̵��Ұ� ��ų�� �ɷ����� �и��� ����
	INT iBuffIndex = -1;
	for( INT iCount=0; iCount<lpObj->m_BuffEffectCount; ++iCount )
	{
		iBuffIndex = lpObj->m_BuffEffectList[iCount].BuffIndex;
		
		// ���� Ÿ�ٿ��� �ɷ��ִ� ������
		if( iBuffIndex==BUFFTYPE_STONE			// �����̰ų�
			|| iBuffIndex==BUFFTYPE_STUN			// �����̰ų�
			|| iBuffIndex==BUFFTYPE_RESTRICTION )	// �ӹ��ϰ��
		{
			return FALSE;							// �и��� ó�� ���� �ʴ´�.
		}
	}
#endif // MODIFY_BACK_SPRING_SKILL_BUGFIX_20080716

	
	if( (rand()%3)==0 )
	{
		if(lpTargetObj->Dir < 4 ) 
		{
			tdir = lpTargetObj->Dir+4;
		}
		else {
			tdir = lpTargetObj->Dir-4;
		}
		tdir *= 2;
	}
	else {
		tdir = lpTargetObj->Dir*2;		
	}

	
	int x, y;
	BYTE attr;

	x = lpObj->X;
	y = lpObj->Y;
	x += RoadPathTable[tdir];
	y += RoadPathTable[tdir+1];

	// �и����� ������ ���̶�� ������..
	attr = MapC[lpObj->MapNumber].GetAttr(x, y);
	if( ((attr&MAP_ATTR_SAFTYZONE) == MAP_ATTR_SAFTYZONE) ||
		((attr&MAP_ATTR_STAND) == MAP_ATTR_STAND) ||
		((attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) ||
		((attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW) 
#ifdef CASTLE_NPC_GATE_WORK_20041206	// �ڷ� �и��� ���� ���Ӽ��̶�� ����
		|| ((attr&MAP_ATTR_WATER) == MAP_ATTR_WATER)
#endif
		)
	{
		return FALSE;
	}
	
	PMSG_POSISTION_SET	pMove;
	pMove.h.c		 = PMHC_BYTE;

#ifdef PACKET_CHANGE	
	pMove.h.headcode = PACKET_POSITION;
#else
	pMove.h.headcode = 0x11;
#endif
	pMove.h.size     = sizeof( pMove );
	pMove.X          = (BYTE)x;
	pMove.Y			 = (BYTE)y;

	lpObj->m_Rest = 0;

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;

	
	RecvPositionSetProc(&pMove, lpObj->m_Index);

	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		// ������Ʈ�� �н�ī��Ʈ�� ���ش�
		lpObj->PathCount = 0;	
	}
	
	return TRUE;
}

BOOL BackSpringCheck(int & x, int & y, int & dir, BYTE map)
{
	BYTE attr;
	int tx, ty;
	int tdir;

	tx = x;
	ty = y;
	tdir = dir/2;

	tx += RoadPathTable[dir];
	ty += RoadPathTable[dir+1];
	attr = MapC[map].GetAttr(tx,ty);
	if( ((attr&MAP_ATTR_SAFTYZONE) == MAP_ATTR_SAFTYZONE) || ((attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) ||((attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW) 
#ifdef CASTLE_NPC_GATE_WORK_20041206	// �ڷ� �и��� ���� ���Ӽ��̶�� ����
		||((attr&MAP_ATTR_WATER) == MAP_ATTR_WATER) 
#endif
		)
	{
		tdir += 4;
		if( tdir > 7 ) tdir -= 8;
		dir = tdir*2;
		return FALSE;
	}
	x = tx;
	y = ty;
	return TRUE;
}

// ������� ���� ������Ʈ �ڷ� �ñ�� ( ���� ������ �ٽ� ƨ��� )
BOOL gObjBackSpring2(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int count)
{	
	int tdir;

#if ENABLE_MC_CODE == 1
	if (!CHECK_LIMIT(lpObj->MapNumber, g_TerrainManager.Size()))
#else // ENABLE_MC_CODE == 1
	if (!CHECK_LIMIT(lpObj->MapNumber, MAX_MAP))
#endif // ENABLE_MC_CODE == 1
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}
#ifdef CRYWOLF_NPC_WORK_20050912
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN )
		{
			return FALSE;
		}
	}
#endif

#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214
	if(lpObj->Class == NPC_INDEX_SPEARMAN || lpObj->Class == NPC_INDEX_BOWMAN )
		return FALSE;
#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
	if(lpObj->Class == NPC_INDEX_LIFESTONE )
		return FALSE;
#endif

#ifdef FOR_BLOODCASTLE
	if(lpObj->Class >= 131 && lpObj->Class <= 134 )		// ����, ������ �и��� �ȵȴ�
		return FALSE;
#endif
	
#ifdef CASTLE_MAIN_SCHEDULER_20041111					// ������ NPC - ����, ���� ���� �и��� �ȵȴ�
	if ((lpObj->Class == 277)||			// ����
		(lpObj->Class == 283)||			// ��ȣ����
		(lpObj->Class == 288)||			// ĳ��Ÿ��
		(lpObj->Class == 278)||			// ����������
		(lpObj->Class == 215)||			// ��
		(lpObj->Class == 216)||			// �հ�
		(lpObj->Class == 217)||			// �հ�����ġ
		(lpObj->Class == 218)||			// �հ�����ġ
		(lpObj->Class == 219)			// ��������
		)
	{
		return FALSE;
	}

#ifdef MODIFY_CASTLESIEGE_GUILDMASTER_BACKSPRING_20051018
	// ���� ��� ���̰� ��ũȣ���� ������ ��ũ�ε�� �з����� �ʴ´�.
	if( g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index )
	{
		if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,4) )
		{
			return FALSE;
		}

	}
#endif // MODIFY_CASTLESIEGE_GUILDMASTER_BACKSPRING_20051018

#endif // CASTLE_MAIN_SCHEDULER_20041111


#ifdef CRYWOLF_NPC_WORK_20050912
	// ������ ���ܿ� ��ϵ� ������ �з����� �ʴ´�.
	if( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_START
		|| g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_READY )
	{
		if( lpObj->Type == OBJTYPE_CHARACTER )
		{		
			for( int i=CRYWOLF_NPC_ALTAR1; i<=CRYWOLF_NPC_ALTAR5; i++ )
			{
				int iUserIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(i);
				if( iUserIndex != _INVALID && iUserIndex == lpObj->m_Index )
				{
					return FALSE;
				}
					
			}
		}
		
	}
#endif // CRYWOLF_MAIN_SCHEDULER_20050505


#ifdef CRYWOLF_NPC_WORK_20050912
	// ������ ����, �Ż��� �и��� �ʴ´�.
	if(	CHECK_CRYWOLF_NPC_ALTAR(lpObj->Class) || CHECK_CRYWOLF_NPC_STATUE(lpObj->Class)	)
	{
		return FALSE;
	}
#endif // CRYWOLF_NPC_WORK_20050912

#ifdef CRYWOLF_MONSTER_WORK_20050914
	if( lpObj->Class == CRYWOLF_MON_TANKER )
	{
		return FALSE;
	}
#endif // CRYWOLF_MONSTER_WORK_20050914

#ifdef MODIFY_0708_BUFFIX_20070820	// ����� �и��� ����
	if( lpObj->Class == 275)
	{
		return FALSE;
	}
#endif	// MODIFY_0708_BUFFIX_20070820

#ifdef ADD_RAKLION_20080408			// ��Ŭ���� ������, �Ź̾��� �и��� ����
	if( lpObj->Class == 459 || lpObj->Class == 460 || lpObj->Class == 461 || lpObj->Class == 462 )
	{
		return FALSE;
	}
#endif // ADD_RAKLION_20080408

#ifdef MODIFY_BACK_SPRING_SKILL_BUGFIX_20080716		// �̵��Ұ� ��ų�� �ɷ����� �и��� ����
	INT iBuffIndex = -1;
	for( INT iCount=0; iCount<lpObj->m_BuffEffectCount; ++iCount )
	{
		iBuffIndex = lpObj->m_BuffEffectList[iCount].BuffIndex;
		
		// ���� Ÿ�ٿ��� �ɷ��ִ� ������
		if( iBuffIndex==BUFFTYPE_STONE			// �����̰ų�
			|| iBuffIndex==BUFFTYPE_STUN			// �����̰ų�
			|| iBuffIndex==BUFFTYPE_RESTRICTION )	// �ӹ��ϰ��
		{
			return FALSE;							// �и��� ó�� ���� �ʴ´�.
		}
	}
#endif // MODIFY_BACK_SPRING_SKILL_BUGFIX_20080716

	tdir = GetPathPacketDirPos( lpObj->X-lpTargetObj->X, lpObj->Y-lpTargetObj->Y)*2;
		
	int x, y;

	x = lpObj->X;
	y = lpObj->Y;

	for( int n=0; n<count; n++)
	{
		if( n >= 2 )
		{
#ifdef DARKLORD_WORK
			if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_KNIGHT )	
#else
			if( lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA )
#endif
				BackSpringCheck(x, y, tdir, lpObj->MapNumber);
		}
		else BackSpringCheck(x, y, tdir, lpObj->MapNumber);
	}	
	
	PMSG_POSISTION_SET	pMove;
	pMove.h.c		 = PMHC_BYTE;

#ifdef PACKET_CHANGE	
	pMove.h.headcode = PACKET_POSITION;
#else
	pMove.h.headcode = 0x11;
#endif
	pMove.h.size     = sizeof( pMove );
	pMove.X          = (BYTE)x;
	pMove.Y			 = (BYTE)y;
	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	RecvPositionSetProc(&pMove, lpObj->m_Index);
	return TRUE;
}


//----------------------------------------------------------------------------
// ����ġ�� üũ�ؼ� �������� �ߴٸ� ������ �ߴٰ� �˸���.
#ifdef BUGFIX_GAIN_EXP_20071210
bool gObjLevelUp( LPOBJECTSTRUCT lpObj, INT64 &addexp, int iMonsterType, int iEventType )
#else

	#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	bool gObjLevelUp(LPOBJECTSTRUCT lpObj, int addexp, int iMonsterType, int iEventType)
	#else
		#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
		bool gObjLevelUp(LPOBJECTSTRUCT lpObj, int addexp)
		#else
		void gObjLevelUp(LPOBJECTSTRUCT lpObj, int addexp)
		#endif
	#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
#endif
{
#if TESTSERVER == 1
	#ifdef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018
		// 2006.5.27 ����
		//lpObj->Experience += addexp;
	#endif
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// �����ͷ��� ���� �� ó��
	bool bEventMapReward = false;

	if( iEventType == EVENT_TYPE_DEVILSQUARE )
		bEventMapReward = true;

	if( g_MasterLevelSystem.MasterLevelUp( lpObj, addexp, bEventMapReward) )
		return false;	
#endif

#ifdef UPDATE_ADD_LOG_20080117
	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %I64u %I64d MonsterIndex : %d, EventType : %d",
		lpObj->MapNumber, lpObj->X, lpObj->Y,
		lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->Experience-addexp, addexp,
		iMonsterType, iEventType );	
#else	// UPDATE_ADD_LOG_20080117
#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906	
	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %u %d MonsterIndex : %d, EventType : %d",
		lpObj->MapNumber, lpObj->X, lpObj->Y,
		lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->Experience-addexp, addexp,
		iMonsterType, iEventType );	
#endif
#endif	// UPDATE_ADD_LOG_20080117

#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
	// �ִ� ������ MAX_LEVEL�� �Ѵ�.
	if( lpObj->Level >= g_GlobalConfig.m_iMaxLevel &&
		((DWORD)lpObj->Experience) >= gLevelExperience[lpObj->Level-1]
		)
	{
		lpObj->Experience = gLevelExperience[lpObj->Level-1];
		GCServerMsgStringSend(lMsg.Get(1136), lpObj->m_Index, 1);
		return false;
	}
#endif
	
//--------->
// ����ġ 0 �̸� ó���� �ʿ� ����. 20081124 (grooving)
if( 0 == addexp )
	return false;
//<---------

#ifdef PCBANG_ADVANTAGE
	if( addexp == 0 )
	#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
		return false;
	#else
		return;
	#endif


#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
	#ifdef FOR_TAIWAN
	if( IsOKPCBangBenefitAll(lpObj) )
	#else
	if( lpObj->Level <= 150 && IsOKPCBangBenefitAll(lpObj) )
	#endif
#else
	#ifdef FOR_TAIWAN
	if ( lpObj->m_bPCBangUser )	
	#else
	if (lpObj->Level <= 150 && lpObj->m_bPCBangUser)
	#endif
#endif //PCBANG_ADVANTAGE_EXPAND_20060919

	{

	#ifdef FOR_TAIWAN
		int iAddPCBangExp = addexp / 2;
	#else
		int iAddPCBangExp = addexp / 10;
	#endif
		
		iAddPCBangExp = min( iAddPCBangExp, gLevelExperience[lpObj->Level] );
		lpObj->Experience += iAddPCBangExp;

		addexp += iAddPCBangExp;
	}
#endif //happycat@20050211

#ifdef BONUS_EXPERIENCE_POINT_20050303
	#define BONUS_LEVEL_LIMIT                       150
    #define BONUS_INCREASE_EXPERIENCE_RATE           10

	if( addexp == 0 )
#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
		return false;
#else
		return;
#endif

	if( lpObj->Level <= BONUS_LEVEL_LIMIT )	
	{ 
		int iAddExp = addexp / BONUS_INCREASE_EXPERIENCE_RATE;
		
		iAddExp = min( iAddExp, gLevelExperience[lpObj->Level] );
		lpObj->Experience += iAddExp;

		addexp += iAddExp;
	}
#endif // BONUS_EXPERIENCE_POINT_20050303

#ifdef DARKLORD_WORK	// �� ������ ����ġ �й�	
	gObjSetExpPetItem(lpObj->m_Index, addexp);
#endif

#ifndef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
	// �ִ� ������ MAX_LEVEL�� �Ѵ�.
	if( lpObj->Level >= g_GlobalConfig.m_iMaxLevel )
	{
		GCServerMsgStringSend(lMsg.Get(1136), lpObj->m_Index, 1);
		return;
	}
#endif

#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
	if( lpObj->Experience < lpObj->NextExp ) return true;
#else
	if( lpObj->Experience < lpObj->NextExp ) return;
#endif

#ifdef BUGFIX_GAIN_EXP_20071210	
	// �������� �ߴٸ� �߰��ؾ� �� ����ġ�� ���̻� ����.
	addexp = 0;
#endif

	lpObj->Experience = lpObj->NextExp;
	lpObj->Level += 1;

#ifdef DARKLORD_WORK
	if(lpObj->Class == CLASS_DARKLORD )
		lpObj->LevelUpPoint += g_GlobalConfig.m_iSpecial_Points;
	else
#endif
	if(lpObj->Class == CLASS_MAGUMSA )
    	lpObj->LevelUpPoint += g_GlobalConfig.m_iSpecial_Points;
	else
    	lpObj->LevelUpPoint += g_GlobalConfig.m_iNormal_Points;

#ifdef SECOND_QUEST
	if( lpObj->PlusStatQuestClear )
	{	// +1���Ⱦ� ����Ʈ ������ �߰� ���� 
		lpObj->LevelUpPoint += g_GlobalConfig.m_iPowerUP_Points;
		LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d", lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint);
	}
#endif

#ifdef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018
	// AddLife�� AddMana ���� ���� ȣ�� �ؾ���.
	gObjCalCharacter(lpObj->m_Index);
#endif // MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018
	
	lpObj->MaxLife += DCInfo.DefClass[lpObj->Class].LevelLife;

	if(g_GlobalConfig.m_bLimitLife && lpObj->MaxLife > g_GlobalConfig.m_iLimitLife)
		lpObj->MaxLife = g_GlobalConfig.m_iLimitLife;

	lpObj->MaxMana += DCInfo.DefClass[lpObj->Class].LevelMana;
	lpObj->Life = lpObj->MaxLife;
	lpObj->Mana = lpObj->MaxMana;	

#ifdef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018
	// �ٽ� ����ϰ� AddLife, AddMana ó���� ���ش�.
	lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
	// SD�� ����������
	#ifdef ADD_SHIELD_POINT_01_20060403
		gObjCalcShieldPoint(lpObj);
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
		// ����� ���� �� ����
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield));
	#endif // ADD_SHIELD_POINT_01_20060403
#endif // MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018

	gObjNextExpCal(lpObj);
	
	LogAddTD(lMsg.Get(520), lpObj->AccountID, lpObj->Name, lpObj->Level);

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	// ������� 220������ �ƴٸ� ���Ǵ�Ƽ �ַο� ��ų �߰�
	if( lpObj->Class == CLASS_ELF && lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel() && lpObj->ChangeUP == 1 )
	{
		int iAddSkillPosition = gObjMagicAdd( lpObj, AT_SKILL_INFINITY_ARROW, 0 );
		if( iAddSkillPosition >= 0 )
		{					
			GCMagicListOneSend( lpObj->m_Index, iAddSkillPosition, AT_SKILL_INFINITY_ARROW, 220, 0 );
			LogAddTD( "[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)"
						, lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->ChangeUP );
		}
		else
		{
			// �Ұ���!! ����!!
			LogAddTD( "[%s][%s] Fail - Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)"
						, lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->ChangeUP );
		}
	}
#endif
	
	// ������ �ڵ� �����Ѵ�.
	//GJSetCharacterInfo(lpObj, lpObj->m_Index); //FIX DS
	//LogAdd("ĳ���� ���� �޽��� ����");

#ifdef NEW_FORSKYLAND2
	gObjSetBP(lpObj->m_Index);
#endif
	
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	GCLevelUpMsgSend(lpObj->m_Index );
#else
	short AddPoint		= 0;
	short MaxAddPoint	= 0;
	gObjGetStatPointState(lpObj->m_Index, AddPoint, MaxAddPoint);

	GCLevelUpMsgSend(lpObj->m_Index, lpObj->Level, lpObj->LevelUpPoint, 
					 (WORD)(lpObj->MaxLife+lpObj->AddLife), 
					 (WORD)(lpObj->MaxMana+lpObj->AddMana), 
					 lpObj->MaxBP+lpObj->AddBP,
					 AddPoint, MaxAddPoint);
#endif // ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712

	gObjCalcMaxLifePower(lpObj->m_Index);	// �ִ� ������� �ٽ� ����Ѵ�

#ifdef ADD_LOG_400LEVELUP_PARTY_20070628	// 400���� �޼��� ���� �ִ� ��Ƽ���� �α� �߰�
	if(lpObj->Level == 400 && lpObj->PartyNumber >=0)
	{
		int partynum = lpObj->PartyNumber;
		char partyLog[256];
		int number;
		sprintf(partyLog, "400 LevelUp (%s)(%s) Party ", lpObj->AccountID, lpObj->Name);

		for( int n=0; n<MAX_PARTYUSER; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];
			if( number >= 0 )
			{
				int noffSet = strlen(partyLog);
				sprintf(partyLog + noffSet, ",(%s)(%s) ",gObj[number].AccountID, gObj[number].Name);								
			}
		}

		LogAddTD(partyLog);
	}
#endif

#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
	return true;
#endif
}

int gObjLevelUpPointAdd(BYTE type, LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type > 3 ) {
		LogAddC(LOGC_RED, lMsg.Get(521), lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return FALSE;
	}
	if( lpObj->LevelUpPoint < 1 ) return FALSE;

	switch( type )
	{
	case 0 : // ��
		{
			if(lpObj->Strength >= g_GlobalConfig.m_iMax_Stats)
				return FALSE;

			lpObj->Strength += 1;
		}
		break;
	case 1 : // ��ø��
		{
			if(lpObj->Dexterity >= g_GlobalConfig.m_iMax_Stats)
				return FALSE;

			lpObj->Dexterity += 1;
		}
		break;
	case 2 : // ü��
		{
			if(lpObj->Vitality >= g_GlobalConfig.m_iMax_Stats)
				return FALSE;

			lpObj->Vitality += 1;
			lpObj->MaxLife += lpObj->VitalityToLife;
			gObjCalcMaxLifePower(lpObj->m_Index);	// �ִ� ������� �ٽ� ����Ѵ�
		}
		break;
	case 3 : // ������
		{
			if(lpObj->Energy >= g_GlobalConfig.m_iMax_Stats)
				return FALSE;

			lpObj->Energy += 1;
			lpObj->MaxMana += lpObj->EnergyToMana;
		}
		break;
		
#ifdef DARKLORD_WORK
	case 4 : // ���
		{
			if(lpObj->Leadership >= g_GlobalConfig.m_iMax_Stats)
				return FALSE;

			lpObj->Leadership += 1;
		}
		break;
#endif
	}
	lpObj->LevelUpPoint-=1;

	// �����Ѵ�
	gObjCalCharacter(lpObj->m_Index);
	return TRUE;
}

// ����
// type, index, level, skill, option

// ������ �������� �ε��Ѵ�.
void gObjMonsterItemLoadScript(LPOBJECTSTRUCT lpObj, char *script_file)
{
	if((SMDFile=fopen(script_file,"r")) == NULL)	
	{
		MsgBox("file open error %s", script_file);
		return ;
	}
	SMDToken Token;
	int type;
	int index;
	int level;
	int op1;
	int op2;
	int op3;
	int incount=0;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			type	= (int)TokenNumber;
			Token = (*GetToken)(); index	= (int)TokenNumber;
			Token = (*GetToken)(); level	= (int)TokenNumber;
			Token = (*GetToken)(); op1		= (int)TokenNumber;
			Token = (*GetToken)(); op2		= (int)TokenNumber;
			Token = (*GetToken)(); op3		= (int)TokenNumber;
			
			if( gObjMonsterInventoryInsertItem(lpObj, type, index, level, op1, op2, op3) != 0xFF )
			{
				//LogAdd("(%d)Item �߰�(level:%d) : type:%d index:%d level:%d op1%d  op2:%d  op3:%d", lpObj->Class, lpObj->Level, type, index, level, op1, op2,op3);
				incount++;
				if( incount > MAX_INVENTORY-1 ) break;
			}
		}
	}
	fclose(SMDFile);
}



// ���� ĳ������ ������ ����� ������ ����Ʈ�� �����.
void gObjGiveItemSearch(LPOBJECTSTRUCT lpObj, int maxlevel)
{

#ifdef UPGRADE_GAMESERVER_01_20041006
	return;
#endif

	int result;
	int count=256;
	int incount=0;
	int type;
	int index;
	
	memset(lpObj->pInventoryMap, 0xFF, MAX_INVENTORYMAP);
	while(count--)
	{
		if( (rand()%40) == 0 )	// ���� ���� Ȯ��
		{
			type = 15;
			index = rand()%MAX_ITEM_INDEX;
		}
		else
		{		
			type = rand()%MAX_ITEM_TYPE;
			index = rand()%MAX_ITEM_INDEX;
		}

		/*
		if( (type == 14) && ((index == 13)||(index == 14)) || 
			(type == 12) && (index == 15) )
		{	// �������� ������ �ʴ´�
			continue;
		}
		*/

		// type : 13, 14, 12�϶� ������ �������..
		if( 
				(	(type==13) && (index<8)	) || 

				(	(type == 14) && ((index == 9)	|| 
					(index == 10) || (index == 13)	|| 
					(index == 14) || (index == 16)	|| 
					(index == 17) || (index == 18))	) ||

				(	(type==12) && (index==15) ) 
			)
		{
			int perc = rand()%8;
			BOOL bCheckDevil = FALSE;

			if( type==12 && index==15 )
			{
				perc = rand()%4;
			}

			// �ؿܴ� �Ǹ��� ��,���� ��� �ǰ� ����
#if !defined ( UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828 ) && defined( DEVILSQARE_COMBINE_ITEM_DROP_20090114 )
			if( type==14 && index==17 )
			{	// �Ǹ��� ��
				if (gEyesOfDevilSquareDropRate <= 0)
					gEyesOfDevilSquareDropRate = 1;
				perc = rand()%gEyesOfDevilSquareDropRate;
				bCheckDevil = TRUE;
			}
			
			if( type==14 && index==18 )
			{	// �Ǹ��� ����
				perc = rand()%gKeyOfDevilSquareDropRate;
				bCheckDevil = TRUE;
			}			
#endif	// UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828

			if( perc ==0 )
			{
				if( bCheckDevil == TRUE )
				{	// �Ǹ��� ���� �������� ����� �ٸ��� �����Ѵ�
					int devilitemlevel = 0;

#ifndef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 7���� ���� gEventMonsterItemDrop() ���� ���
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// ����7 ����� ��� ��� ����
					if( lpObj->Level >= 90 )
					{
						if( rand() % 1000 < 10 )
						{
							devilitemlevel = 7;
						}
					}
					else
#endif
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
					if( rand()%5 != 0 )
					{	// 0¥�� ������
#ifdef DEVILSQUARE_EXTEND_20050221		// !!! . �ӽ� -> ���� ���� ������ ��� �߰�
						if( lpObj->Level < 3)
						{
							devilitemlevel = 0;
						}
						else if( lpObj->Level < 36)
						{
							devilitemlevel = 1;
						}
						else if( lpObj->Level < 47)
						{
							devilitemlevel = 2;
						}
						else if( lpObj->Level < 60)
						{
							devilitemlevel = 3;
						}
						else if( lpObj->Level < 70)
						{
							devilitemlevel = 4;
						}
						else if( lpObj->Level < 80)
						{
							devilitemlevel = 5;
						}
						else
						{
							devilitemlevel = 6;
						}
#else
						if( lpObj->Level <= 15)
						{
							devilitemlevel = 0;
						}
						else if( lpObj->Level <= 40)
						{
							devilitemlevel = 1;
						}
						else if( lpObj->Level <= 54)
						{
							devilitemlevel = 2;
						}
						else if( lpObj->Level <= 66)
						{
							devilitemlevel = 3;
						}
						else
						{
							devilitemlevel = 4;
						}
#endif
#ifndef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 7���� ���� gEventMonsterItemDrop() ���� ���
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
					}
#endif	
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123					

						if( devilitemlevel != 0 )
						{	// �����δ� 0¥�� �Ǹ��� ���� �������� ���� ���� �ʴ´�
							if( gObjMonsterInventoryInsertItem(lpObj, type, index, devilitemlevel) != 0xFF )
							{
								///*if( lpObj->Level >= 50 )
								//{
								//LogAdd("(%d)Devil Square Item �߰�(level:%d) : type:%d index:%d level:%d %s", lpObj->Class, lpObj->Level, type, index, devilitemlevel, lpObj->Name);
								//}
								//*/
								incount++;								
								if( incount > MAX_INVENTORY-1 ) break;
							}
						}
#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 7���� ���� gEventMonsterItemDrop() ���� ���
					}
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
				//	}
#else
				}
#endif
				}	
				else
				{
					if( zzzItemLevel(type, index, lpObj->Level) == TRUE )
					{
						if( gObjMonsterInventoryInsertItem(lpObj, type, index, 0) != 0xFF )
						{
							///*if( lpObj->Level >= 50 )
							//{
							//LogAdd("(%d)Item �߰�(level:%d) : type:%d index:%d level:%d %s", lpObj->Class, lpObj->Level, type, index, result, lpObj->Name);
							//}
							//*/
							incount++;							
							if( incount > MAX_INVENTORY-1 ) break;
						}
					}
				}
			}
		}
		else
		{
			result = GetLevelItem(type, index, lpObj->Level);
			if( result >= 0  )
			{
				if( (type == 13 && index == 10) || 
					(type == 12 && index == 11) )
				{
					if( gObjMonsterInventoryInsertItem(lpObj, type, index, result) != 0xFF )
					{
						//if( lpObj->Level >= 50 )
						{
						//LogAdd("(%d)Item �߰�(level:%d) : type:%d index:%d level:%d", lpObj->Class, lpObj->Level, type, index, result);
						}
						//*/
						incount++;						
						if( incount > MAX_INVENTORY-1 ) break;
					}
				}
				else if( result <= maxlevel )
				{
					if( type == 12)
					{
						if( index != 11 ) result = 0;
					}
					if( type == 12 && index == 11 )
					{

					}
					else if( result > maxlevel ) result = maxlevel;

					if( (type == 4 && index == 7) || (type == 4 && index == 15) )	// ����,ȭ�� ��ų ����
					{
						result = 0;
					}
					
					if( gObjMonsterInventoryInsertItem(lpObj, type, index, result) != 0xFF )
					{
						//if( lpObj->Level >= 50 )
						{
						//LogAdd("(%d)Item �߰�(level:%d) : type:%d index:%d level:%d", lpObj->Class, lpObj->Level, type, index, result);
						}
						//*/
						incount++;						
						if( incount > MAX_INVENTORY-1 ) break;
					}
				}
			}
		}
	}
}


BYTE gObjWarehouseInsertItem(LPOBJECTSTRUCT lpObj, int type, int index,int level, int pos)
{
	CItem item;
	int item_type;
		
	item_type    = ItemGetNumberMake(type, index );
	item.m_Level = level;
	item.Convert( item_type, 0, 0, 0);

	lpObj->pWarehouse[pos] = item;
//	LogAdd("������Ʈ ������ �߰� : [%d] : %s ", pos, lpObj->pWarehouse[pos].GetName());
	return pos;
}

// ���� ĳ������ ������ ����� ������ ����Ʈ�� �����.
// â�� ����� �ش�.
void gObjGiveItemWarehouseSearch(LPOBJECTSTRUCT lpObj, int maxlevel)
{

#ifdef UPGRADE_GAMESERVER_01_20041006
///////////////////////////////////////////////////////////////////
//
//	MonsterItemMng�� ��ƾ�� ����Ѵ�.
//
///////////////////////////////////////////////////////////////////
	return;
#endif

	int result;
	int count=256;
	int incount=0;
	int type;
	int index;
	int ItemPos=0;

	int	NeedLevel = lpObj->Level-25;
	if( NeedLevel <= 0 )
	{	// ������Ʈ �������� ������ ����
		return;
	}
	
//	LogAdd("������Ʈ ������ �߰� ���� : %s ", lpObj->Name);	
	
	memset(lpObj->pWarehouseMap, 0xFF, MAX_WAREHOUSEITEMS);
	lpObj->WarehouseCount = 0;
	while(count--)
	{
		if( (rand()%40) == 0 )	// ���� ���� Ȯ��
		{
			type = 15;
			index = rand()%MAX_ITEM_INDEX;
		}
		else
		{		
			type = rand()%MAX_ITEM_TYPE;
			index = rand()%MAX_ITEM_INDEX;
		}

#ifdef ADD_NEW_MAP_KALIMA_20040518			// Į����6 - ��п��Լ��� ���� 90������ �������� �������� �ʴ´�.
	if (lpObj->Class == 275) {
		int item_num	= (type*MAX_ITEM_INDEX)+index;

		if( ItemAttribute[item_num].Level == 0xFF )	continue;	// ������ �������� ����
		if( ItemAttribute[item_num].Level == 0    )	continue;
		if( ItemAttribute[item_num].Level < 125 )	continue;	// ������ 100���� ����
	}
#endif

#ifdef NEW_SKILL_FORSKYLAND
		if( type == 13 && index == 3 )
		{	// �䰡�þ��� ���� ���Ϳ��Լ� ���� ���� ����
			continue;
		}
#endif
		// type : 13, 14, 12�϶� ������ �������..
		// type : 13, 14, 12�϶� ������ �������..
		if( 
				(	(type==13) && (index<8)	) || 

				(	(type == 14) && ((index == 9)	|| 
					(index == 10) || (index == 13)	|| 
					(index == 14) || (index == 16)	|| 
					(index == 17) || (index == 18))	) ||

				(	(type==12) && (index==15) ) 
			)
		{
			int perc = rand()%8;

			if( type==12 && index==15 )
			{
				perc = 1;
				//perc = rand()%5;
			}

			if( type==14 && index==17 )
			{	// �Ǹ��� ��
				perc = 1;
			}

			if( type==14 && index==18 )
			{	// �Ǹ��� ����				
				perc = 1;
			}			

			if( perc ==0 )
			{
				if( zzzItemLevel(type, index, NeedLevel) == TRUE )
				{	
					if( gObjWarehouseInsertItem(lpObj, type, index, 0, ItemPos) != 0xFF )
					{
						//if( lpObj->Level >= 50 )
						{
						//LogAdd("(%d)(%s) Ex Item Add(level:%d) : type:%d index:%d level:%d Name : %s %s", 
						//	  lpObj->Class, lpObj->Name, lpObj->Level, type, index, result, lpObj->pWarehouse[ItemPos].GetName(), lpObj->Name);
						}
						ItemPos++;
						incount++;
						if( incount > MAX_INVENTORY-1 ) break;
					}
				}
			}
		}
		else
		{
			result = GetLevelItem(type, index, NeedLevel);
			if( result >= 0  )
			{
				if( (type == 13 && index == 10) || 
					(type == 12 && index == 11) )
				{
					if( gObjWarehouseInsertItem(lpObj, type, index, 0, ItemPos) != 0xFF )
					{
						//if( lpObj->Level >= 50 )
						{
						//LogAdd("(%d)(%s) Ex Item Add(level:%d) : type:%d index:%d level:%d Name : %s %s", 
						//	  lpObj->Class, lpObj->Name, lpObj->Level, type, index, result, lpObj->pWarehouse[ItemPos].GetName(), lpObj->Name);
						}
						ItemPos ++;
						incount++;
						if( incount > MAX_INVENTORY-1 ) break;
					}
				}
				else if( result <= maxlevel )
				{
					if( type == 12)
					{
						if( index != 11 ) result = 0;
					}
					if( type == 12 && index == 11 )
					{

					}
					else if( result > maxlevel ) result = maxlevel;

					if( (type == 4 && index == 7) || (type == 4 && index == 15) )	// ����,ȭ�� ��ų ����
					{
						result = 0;
					}
					
					if( gObjWarehouseInsertItem(lpObj, type, index, 0, ItemPos) != 0xFF )
					{
						//if( lpObj->Level >= 50 )
						{
						//LogAdd("(%d)(%s) Ex Item Add(level:%d) : type:%d index:%d level:%d Name : %s %s", 
						//	  lpObj->Class, lpObj->Name, lpObj->Level, type, index, result, lpObj->pWarehouse[ItemPos].GetName(), lpObj->Name);
						}
						
						ItemPos ++;
						incount++;
						if( incount > MAX_INVENTORY-1 ) break;
					}
				}
			}
		}
	}
	lpObj->WarehouseCount = ItemPos;
}

//----------------------------------------------------------------------------
// ��� �����Ͱ� �� �ڰ��� �ִ��� Ȯ���Ѵ�.
int gObjGuildMasterCapacityTest(LPOBJECTSTRUCT lpObj)
{	
	if( lpObj->Level < gGuildCreateLevel )
	{
		return 0;
	}
	/*
	int itype;
	if( (lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Level < 7) &&
		(lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Level < 7) ) return FALSE;
	if( lpObj->pInventory[EQUIPMENT_HELM].m_Level < 7 ) return FALSE;
	if( lpObj->pInventory[EQUIPMENT_ARMOR].m_Level < 7 ) return FALSE;
	if( lpObj->pInventory[EQUIPMENT_PANTS].m_Level < 7 ) return FALSE;
	if( lpObj->pInventory[EQUIPMENT_GLOVES].m_Level < 7 ) return FALSE;
	if( lpObj->pInventory[EQUIPMENT_BOOTS].m_Level < 7 ) return FALSE;
	*/
	return 1;
}



void gObjNextExpCal(LPOBJECTSTRUCT lpObj)
{
	lpObj->NextExp = gLevelExperience[lpObj->Level];
}


// ����� ���ݿ� ���� ���׷� ���
BOOL retResistance(LPOBJECTSTRUCT lpObj, int Resistance_Type)
{	
	BYTE r = lpObj->m_Resistance[Resistance_Type];

#ifdef MODIFY_MONSTER_RESISTANCE_IMMUNE_20060523
	// ���׷� ���� 255�� ��� 100% ���� (��� ������)
	if ( r == 255 ) 
	{
		return TRUE;
	}
#endif // MODIFY_MONSTER_RESISTANCE_IMMUNE_20060523
	
#ifdef UPDATE_GM_FUNCTION_20070228	// GM ���� ���� �������϶��� ���� Max
	if( (lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM  
		&& ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 42 )  
		|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 42 ) ) )
	{
		return TRUE;
	}
#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
// ��ȥ�� ������ ������� ��� ���׷��� ���� ��Ų��.
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( r > 0 && ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SOUL_POTION ) == true )
		&& ( Resistance_Type == RESISTANCE_THUNDER || Resistance_Type == RESISTANCE_COLD ) 
	  )
	#else
	if( r > 0 && ( lpObj->m_iPotionSoulTime > 0 ) 
		&& ( Resistance_Type == RESISTANCE_THUNDER || Resistance_Type == RESISTANCE_COLD ) )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	{
		r += r * CASTLE_SOULPOTION_ADD_RESISTANCE / 100;
	}
#endif

	if( r == 0 ) return FALSE;
	if( (rand()%(r+1)) == 0 ) return FALSE;
	return TRUE;
}

// ��ų ���׷� ���(���̽��ַο�(��ȭ), ���˻� ���°���)
BOOL retCalcSkillResistance(LPOBJECTSTRUCT lpTargetObj, BOOL isDouble)
{	
	if( isDouble )
	{
		if( (rand()%(100)) < 45 ) return TRUE;
	}
	else
	{
		if( (rand()%(100)) < 40 ) return TRUE;
	}
	return FALSE;
}

// �����ص� �Ǵ��� �����.
//BOOL gObjAttackQ(BYTE type)
BOOL gObjAttackQ(LPOBJECTSTRUCT lpObj)
{
#ifdef ADD_NEW_MAP_KALIMA_20040518		// ������ ���ݹ��� �ʴ´�.
	if (CHECK_KALIMAGATE (lpObj->m_Attribute)) 
	{
		return FALSE;
	}
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116 // ���� ����� ���� ���� �ʴ´�.
	if( lpObj->Class == NPC_SIEGE_MACHINE_ATTACK || lpObj->Class == NPC_SIEGE_MACHINE_DEFENSE )
		return FALSE;
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �������� NPC���� ���ݴ��� ����
	if (((lpObj->Class == 277) || (lpObj->Class == 283)) &&
		g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		// ��ȣ������ �������� ���۵Ǳ� �������� ���ݹ����� �ʵȴ�.
		return FALSE;
	}

	if ((lpObj->Class == 221)			// �߼���
		|| (lpObj->Class == 222)		// �߼���
//		|| (lpObj->Class == 288)		// ĳ��Ÿ��
		)
	{
		// �߼����� ĳ��Ÿ���� ���ݹ����� �ʵȴ�.
		return FALSE;
	}

	if (lpObj->Class == 277 &&
		lpObj->m_btCsGateOpen == CS_GATE_STATE_OPENED
		)
	{
		// ������ �������� ���� ���ݹ����� �ʵȴ�.
		return FALSE;
	}
#endif
	
#ifdef FOR_BLOODCASTLE
	if( CHECK_BLOODCASTLE(lpObj->MapNumber) && lpObj->Type >= OBJTYPE_MONSTER ) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ�� �ٸ� �ε��� ����
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpObj->MapNumber );
		// ���� �Ҵ緮�� ä������ �������� ������ ���ݹ޾Ƽ��� �ʵȴ�.
		if ( (lpObj->Class == 131) && (!g_BloodCastle.CheckMonsterKillSuccess(iBridgeIndex)) ) 
			return FALSE;	

		// ���� ���� �Ҵ緮�� ä������ �������� ������ ���ݹ޾�� �ʵȴ�.
		if ( CHECK_LIMIT(lpObj->Class-BC_SAINT_STATUE_1, 3) && (!g_BloodCastle.CheckBossKillSuccess(iBridgeIndex)) ) 
			return FALSE;
#else
		// ���� �Ҵ緮�� ä������ �������� ������ ���ݹ޾Ƽ��� �ʵȴ�.
		if ( (lpObj->Class == 131) && (!g_BloodCastle.CheckMonsterKillSuccess(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1)) ) 
			return FALSE;	

		// ���� ���� �Ҵ緮�� ä������ �������� ������ ���ݹ޾�� �ʵȴ�.
		if ( CHECK_LIMIT(lpObj->Class-BC_SAINT_STATUE_1, 3) && (!g_BloodCastle.CheckBossKillSuccess(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1)) ) 
			return FALSE;
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
	}


	if( lpObj->Class != 247 &&  lpObj->Class != 249 && (lpObj->Class >= 100 && lpObj->Class<= 110 ) ) return FALSE;	//100���� ũ�ٴ� ���� NPC�� Ʈ���̴�.

	
#else
	if( lpObj->Class != 247 &&  lpObj->Class != 249 && (lpObj->Class >= 100 && lpObj->Class< 150 ) ) return FALSE;	//100���� ũ�ٴ� ���� NPC�� Ʈ���̴�.
#endif
	if( lpObj->m_State != OBJST_PLAYING ) return FALSE;
	if( lpObj->Live == 0 ) return FALSE;
	if( lpObj->Teleport == 1 ) return FALSE;	// �����̵��� ��������.	
	return TRUE;
}

#define MAX_PKCOUNT		100
//----------------------------------------------------------------------------
// ����ڸ� �׿���..
// lpObj : �����Ѿ�
// lpTargetObj : ������ ���Ѿ�
#ifdef MOIDFY_PK_DAMAGE_REFLECT_NOPK_BUGFIX_20081202	// 
void gObjPlayerKiller(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int MSBDamage)
#else	// MOIDFY_PK_DAMAGE_REFLECT_NOPK_BUGFIX_20081202
void gObjPlayerKiller(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
#endif	// MOIDFY_PK_DAMAGE_REFLECT_NOPK_BUGFIX_20081202
{
	
	if( lpObj->Type != OBJTYPE_CHARACTER ) return;

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	return;		// ���� ��ʸ�Ʈ������ ī�� 
#endif

#ifdef ADD_NEW_UNION_01_20041006
	// ���� ������ ���̸� ī���� �ƴϴ�.
	if( gObjGetRelationShip(lpObj, lpTargetObj) == G_RELATIONSHIP_RIVAL )
	{
		if( lpObj->lpGuild && lpTargetObj->lpGuild )
		{
			// ���⼭ ���� ��带 ������?

			LogAddTD("[U.System][Rival][Player Kill] ( Killer [%s][%s] / %s / U:%d / R:%d ) vs ( Victim [%s][%s] / %s / U:%d / R:%d )",
				lpObj->AccountID, lpObj->Name, lpObj->lpGuild->Name, lpObj->lpGuild->iGuildUnion, lpObj->lpGuild->iGuildRival,
				lpTargetObj->AccountID, lpTargetObj->Name, lpTargetObj->lpGuild->Name, lpTargetObj->lpGuild->iGuildUnion, lpTargetObj->lpGuild->iGuildRival );
		}
		else
		{
			LogAddTD("[U.System][Rival][Player Kill][��ERROR : Can't find GuildInfo] (  [%s][%s] ) vs ( [%s][%s] )", lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name );
		}
		
		return;
	}
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408			// ī���� ĳ�� �ȿ��� ����߿� ���� ���̶�� PK�� �ƴϴ�.
	if (CHECK_CHAOSCASTLE(lpTargetObj->MapNumber) && 
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ�� ��ȣ ���ϱ�
		(g_ChaosCastle.GetCurrentState( g_ChaosCastle.GetChaosCastleByMapNum( lpTargetObj->MapNumber ) ) == CHAOSCASTLE_STATE_PLAYING)
#else
		(g_ChaosCastle.GetCurrentState(lpTargetObj->MapNumber - MAP_INDEX_CHAOSCASTLE1) == CHAOSCASTLE_STATE_PLAYING)
#endif
		)
	{
		return;
	}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// ������ �߿��� ������ ���������� ����� �׿��� PK�� �ƴϴ�.
	if (lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE)
	{
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
			if (lpObj->m_btCsJoinSide != CS_SIEGESIDE_NONE) {
				return;
			}
		}
		else 
		{


#ifdef MODIFY_CASTLE_PK_01_20050309
			// ������ ���� ���°� �ƴ� �� PK ��迡 ����  ���ֱ���� PK �г�Ƽ�� �����Ѵ�.
			if ( lpObj->Y > CASTLE_PK_BORDER 
				&& g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE) 
			{
				return;
			}
#else
			if (g_CastleSiege.GetCastleState()!=CASTLESIEGE_STATE_READYSIEGE 
				&& g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE) 
			{
				return;
			}
			else if( g_CastleSiege.GetCastleState()==CASTLESIEGE_STATE_READYSIEGE 
					&& lpObj->Y > CASTLE_PK_BORDER 
					&& g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE )
			{
				return;
			}
#endif


		}
		
		// ���� �ʿ��� ī���� �׿��� �� ���� �Ǵ� ���� �����Ѵ�. (apple)
		if( lpTargetObj->m_PK_Level >= PK_LEVEL_DEFAULT+3 )
		{
			return;
		}
	}
#endif

#ifdef JAPAN_DESTROYKING_EVENT_20040625		// �� Ȥ�� ������ �̺�Ʈ GM (�ı���) �̶�� PK�� �ƴϴ�.
	if( lpObj->Authority&CTLCODE_EVENT_GM || lpTargetObj->Authority&CTLCODE_EVENT_GM)
	{
		return;
	}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��������� pk�� �ƴϴ�
	if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) || CHECK_ILLUSION_TEMPLE(lpTargetObj->MapNumber) )
	{
		return;
	}
#endif 

	if( lpTargetObj->m_stateEngagePVP == PVP_USER_PLAYING )
	{
		g_PvpEvent.DeleteUser(lpTargetObj->m_Index);
		return;
	}

#ifdef ADD_NEWPVP_PKFIELD
	if (g_NewPVP.IsVulcanusMap(lpObj->MapNumber) && g_NewPVP.IsVulcanusMap(lpTargetObj->MapNumber)) {
		return;
	}
#endif // ADD_NEWPVP_PKFIELD

	// ��뿡�� ��������� �����Ǿ� �ִٸ�..	
#ifdef MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022	// ��뿡�� ��������� �����Ǿ� �ִٸ� .
	int partynum			= lpObj->PartyNumber;	// �������� ��Ƽ��ȣ
	int	nSelfDefenseUser	= -1;					// �����ڿ� ���漳���� ����

	for( int n=0; n<MAXSELFDEFENSE; n++)
	{
		// Ȥ�� �����ڰ� ������� ����ڶ��.. �׳� ����
		if( lpTargetObj->SelfDefense[n] >= 0)
		{
			if( lpTargetObj->SelfDefense[n] == lpObj->m_Index ) return;		

			// �����ڿ� ���漳���� �����߿� �����ڿ� ���� ��Ƽ�� ������ ����
			if( partynum >= 0 
				&& gObj[lpTargetObj->SelfDefense[n]].PartyNumber == partynum )
			{
				return;
			}	
		}
	}

#else	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022

	for( int n=0; n<MAXSELFDEFENSE; n++)
	{
		if( lpTargetObj->SelfDefense[n] >= 0 )
		{
			if( lpTargetObj->SelfDefense[n] == lpObj->m_Index )
			{
				// ���������� ��ȿ��Ų��.
				return;
			}
		}
	}
#endif	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022

	INT iOriginPkLevel = lpObj->m_PK_Level;		// �α׸� ����� ���� ������ PK���� ��� (2003.11.28 H.J.I.)
	INT iOriginPkCount = lpObj->m_PK_Count;		// �α׸� ����� ���� ������ PKȽ�� ��� (2003.11.28 H.J.I.)

#ifdef MOIDFY_PK_DAMAGE_REFLECT_NOPK_BUGFIX_20081202	// ������ �ݻ�� ���� ��� ���� �������� ����
	if( MSBDamage == DT_MIRROR)
	{
		return;
	}
#endif	// MOIDFY_PK_DAMAGE_REFLECT_NOPK_BUGFIX_20081202

// PK�ÿ� PK ���� �ȿ÷� ��
#ifdef ADD_SHIELD_POINT_01_20060403
	if( g_PKLevelIncreaseOff == TRUE )
	{
		return;
	}
#endif
	
#ifdef MODIFY_PK_SYSTEM_BUGFIX_20070323
	char TargetTmpPKLevel = PK_LEVEL_DEFAULT;	// �ӽ� PK ����
	
	if(lpTargetObj->PartyNumber >= 0)
	{
#ifdef UPDATE_PK_SYSTEM_20070302
		// ���� ĳ���� ���θ��� ��Ƽ���� ���
		if(gParty.GetPKPartyPenalty(lpTargetObj->PartyNumber) >= PK_LEVEL_DEFAULT+2)
		{
			TargetTmpPKLevel = gParty.GetPKPartyPenalty(lpTargetObj->PartyNumber);
		}
		else
#endif
		{
			TargetTmpPKLevel = lpTargetObj->m_PK_Level;
		}
	}
	else
	{
		TargetTmpPKLevel = lpTargetObj->m_PK_Level;
	}

	// �Ϲ���
	if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT )
	{
		// �Ϲ����� �׿��ٸ�..
		if( TargetTmpPKLevel == PK_LEVEL_DEFAULT )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� �����ڷ�..
		}	
		// ������ �׿��ٸ�..
		else if( TargetTmpPKLevel < PK_LEVEL_DEFAULT )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� ��� �ܰ�� �ٲ۴�.
		}
		// ����ڸ� �׿��ٸ�.
		else if( TargetTmpPKLevel == PK_LEVEL_DEFAULT+1 )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� ��� �ܰ�� �ٲ۴�.
		}
		// ���θ��� �׿��ٸ�..
		else if( (TargetTmpPKLevel >= PK_LEVEL_DEFAULT+3) &&
				 (lpTargetObj->Level > 20) )
		{
			lpObj->m_PK_Count--;
			if( lpObj->m_PK_Count < -3 ) 
			{
				lpObj->m_PK_Count = -3;
				return;
			}
		}
		else return;	// �׿��� ��쿣 ����
	}
	// ������ �϶���..
	else if( lpObj->m_PK_Level > PK_LEVEL_DEFAULT )
	{
		// �Ϲ����̳� ������ �׿�������..
		if( TargetTmpPKLevel <= PK_LEVEL_DEFAULT+1 )
		{
			lpObj->m_PK_Count++;
			if( lpObj->m_PK_Count > MAX_PKCOUNT ) 
				lpObj->m_PK_Count = MAX_PKCOUNT;
		}
		else return;
	}
	// ���� �϶���..
	else if( lpObj->m_PK_Level < PK_LEVEL_DEFAULT )
	{
		// �Ϲ����� �׿��ٸ�
		if( TargetTmpPKLevel == PK_LEVEL_DEFAULT )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� ���ε�� 1 (���)
		}	
		// ������ �׿��ٸ�..
		else if( TargetTmpPKLevel < PK_LEVEL_DEFAULT )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� ���ε�� 1 (���)
		}
		else if( TargetTmpPKLevel == PK_LEVEL_DEFAULT+1 )
		{
			lpObj->m_PK_Count = 1; // �����ڸ� ���ε�� 1 (���)
		}
		// ���θ��� �׿��ٸ�..
		else if( (TargetTmpPKLevel >= PK_LEVEL_DEFAULT+3) )//&& FIX PKBUG
				 //(lpTargetObj->Level > 20) )
		{
			lpObj->m_PK_Count--;
			if( lpObj->m_PK_Count < -3 ) 
			{
				lpObj->m_PK_Count = -3;
			}
		}
		else return;
	}
#else	// MODIFY_PK_SYSTEM_BUGFIX_20070323
	// �Ϲ���
	if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT )
	{
		// �Ϲ����� �׿��ٸ�..
		if( lpTargetObj->m_PK_Level == PK_LEVEL_DEFAULT )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� �����ڷ�..
		}	
		// ������ �׿��ٸ�..
		else if( lpTargetObj->m_PK_Level < PK_LEVEL_DEFAULT )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� ��� �ܰ�� �ٲ۴�.
		}
		// ����ڸ� �׿��ٸ�.
		else if( lpTargetObj->m_PK_Level == PK_LEVEL_DEFAULT+1 )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� ��� �ܰ�� �ٲ۴�.
		}
		// ���θ��� �׿��ٸ�..
		else if( (lpTargetObj->m_PK_Level >= PK_LEVEL_DEFAULT+3) &&
				 (lpTargetObj->Level > 20) )
		{
			lpObj->m_PK_Count--;
			if( lpObj->m_PK_Count < -3 ) 
			{
				lpObj->m_PK_Count = -3;
				return;
			}
		}
		else return;	// �׿��� ��쿣 ����
	}
	// ������ �϶���..
	else if( lpObj->m_PK_Level > PK_LEVEL_DEFAULT )
	{
		// �Ϲ����̳� ������ �׿�������..
		if( lpTargetObj->m_PK_Level <= PK_LEVEL_DEFAULT+1 )
		{
			lpObj->m_PK_Count++;
			if( lpObj->m_PK_Count > MAX_PKCOUNT ) 
				lpObj->m_PK_Count = MAX_PKCOUNT;
		}
		else return;
	}
	// ���� �϶���..
	else if( lpObj->m_PK_Level < PK_LEVEL_DEFAULT )
	{
		// �Ϲ����� �׿��ٸ�
		if( lpTargetObj->m_PK_Level == PK_LEVEL_DEFAULT )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� ���ε�� 1 (���)
		}	
		// ������ �׿��ٸ�..
		else if( lpTargetObj->m_PK_Level < PK_LEVEL_DEFAULT )
		{			
			lpObj->m_PK_Count = 1; // �����ڸ� ���ε�� 1 (���)
		}
		else if( lpTargetObj->m_PK_Level == PK_LEVEL_DEFAULT+1 )
		{
			lpObj->m_PK_Count = 1; // �����ڸ� ���ε�� 1 (���)
		}
		// ���θ��� �׿��ٸ�..
		else if( (lpTargetObj->m_PK_Level >= PK_LEVEL_DEFAULT+3) )//&& FIX PKBUG
				 //(lpTargetObj->Level > 20) )
		{
			lpObj->m_PK_Count--;
			if( lpObj->m_PK_Count < -3 ) 
			{
				lpObj->m_PK_Count = -3;
			}
		}
		else return;
	}
#endif	// MODIFY_PK_SYSTEM_BUGFIX_20070323

	if( lpObj->m_PK_Count == 0 )
	{
		lpObj->m_PK_Level = PK_LEVEL_DEFAULT;
		lpObj->m_PK_Time = 0;
	}
	else if( lpObj->m_PK_Count == 1 )
	{
		lpObj->m_PK_Level = PK_LEVEL_DEFAULT+1;
		lpObj->m_PK_Time = 0;
	}
	else if( lpObj->m_PK_Count == 2 )
	{
		lpObj->m_PK_Level = PK_LEVEL_DEFAULT+2;
		lpObj->m_PK_Time = 0;

#ifdef UPDATE_PK_SYSTEM_20070302		// ��Ƽ�� �ϰ� ������ ��Ƽ�� �г�Ƽ ����
		if(lpObj->PartyNumber >= 0)
		{
			gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
			gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
		}
#endif
	}
	else if( lpObj->m_PK_Count >= 3 )
	{
		lpObj->m_PK_Level = PK_LEVEL_DEFAULT+3;
		lpObj->m_PK_Time = 0;

#ifdef UPDATE_PK_SYSTEM_20070302		// ��Ƽ�� �ϰ� ������ ��Ƽ�� �г�Ƽ ����
		if(lpObj->PartyNumber >= 0)
		{
			gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
			gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
		}
#endif
	}
	else if( lpObj->m_PK_Count <= -3)
	{
		lpObj->m_PK_Level = PK_LEVEL_DEFAULT-2;
		lpObj->m_PK_Time = 0;
	}	
	/*else if( lpObj->m_PK_Count < -3)
	{
		lpObj->m_PK_Level = PK_LEVEL_DEFAULT-2;
		lpObj->m_PK_Time = 0;
	}
	*/
	GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
	if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1 )
	{		
		GCServerMsgStringSend(lMsg.Get(1137), lpObj->m_Index, 1);
	}

	LogAddTD("[%s][%s] PK Info Set (Killed Player) Origin(PkLevel:%d, PkCount:%d) Changed(PkLevel:%d, PkCount:%d)", 
		lpObj->AccountID, 
		lpObj->Name,
		iOriginPkLevel,
		iOriginPkCount,
		lpObj->m_PK_Level,
		lpObj->m_PK_Count
		);
}

//----------------------------------------------------------------------------
// PK�� �׾����� ó��
// lpObj : ���� ����
// lpTargetObj : ��������
void gObjUserDie(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	int count=MAX_INVENTORY;
	int itemdrop=1;
#ifdef HERO_SYSTEM_MODIFY
	int itemvanish=1;
#endif
	int number=0;
	int dropresult=0;
	
	if( lpObj->Type != OBJTYPE_CHARACTER ) return;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjClearBuffEffect( lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	gObjSetKillCount( lpObj->m_Index, KILLCOUNT_RESET );
	gObjUseSkill.RemoveAllCharacterInvalidMagicAndSkillState( lpObj );
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	lpObj->m_iPotionBlessTime = 0;
	lpObj->m_iPotionSoulTime  = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_BLESS_POTION );
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_SOUL_POTION );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif
	
#ifdef CHINA_GM_EVENT
	if( lpObj->Authority&AUTHORITY_EVENT_GM )
	{
		char tempMsg[255];
		wsprintf(tempMsg, "[GMEvent] (%s)(%s) EventGM Dead, Killed by (%s)(%s) MapInfo(%d)(%d/%d)", 
				lpObj->AccountID, lpObj->Name,				
				lpTargetObj->AccountID, lpTargetObj->Name,
				lpObj->MapNumber, lpObj->X, lpObj->Y);

		LogAddTD(tempMsg);
		//fix mensaje 100 - 200
		/*wsprintf(tempMsg, lMsg.Get(1302),	//"(%s)���� (%s)�� �׿����ϴ�"
				lpTargetObj->Name,
				lpObj->Name);
		AllSendServerMsg(tempMsg);	*/	

		gGMEventRunning = FALSE;
		return;
	}
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
	if( lpObj->Authority&AUTHORITY_EVENT_GM )
	{
		char tempMsg[255];
		wsprintf(tempMsg, "[GMEvent] (%s)(%s) EventGM Dead, Killed by (%s)(%s) MapInfo(%d)(%d/%d)", 
			lpObj->AccountID, lpObj->Name,				
			lpTargetObj->AccountID, lpTargetObj->Name,
			lpObj->MapNumber, lpObj->X, lpObj->Y);
		
		LogAddTD(tempMsg);
		
		wsprintf(tempMsg, lMsg.Get(1302),	//"(%s)���� (%s)�� �׿����ϴ�"
			lpTargetObj->Name,
			lpObj->Name);
		AllSendServerMsg(tempMsg);
		
#ifdef EVENT_SERVER_OPEN_COMMEMORATION_20050510
		gObjMakeGMEventItem( lpTargetObj->m_Index );
#endif // EVENT_SERVER_OPEN_COMMEMORATION_20050510
		
		gGMEventRunning = FALSE;
		return;
	}
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#ifdef _NEW_EXDB_
	// ����� �� �̰� ������ ������� �� �̶�� ������ �ȶ����� �ϱ�..
	if( gObjTargetGuildWarCheck(lpObj, lpTargetObj) == TRUE ) return;
#endif

	if( lpObj->m_stateEngagePVP == PVP_USER_PLAYING)
	{
		return;
	}

	// �Ǹ��� ���忡���� ����ġ �ȱ��̰�
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( CHECK_DEVILSQUARE(lpObj->MapNumber) ) 
#else
	if( lpObj->MapNumber == 9 ) 
#endif
	{
		if( lpTargetObj->Type	== OBJTYPE_MONSTER )
		{	// ���� ����� ���� �̸�			
			LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpTargetObj->Class);
			if( lpattr )
			{
				LogAddTD("[DevilSquare] Dead In DevilSquare [%s][%s][%s]", lpObj->AccountID, lpObj->Name, lpattr->m_Name);
			}
		}
		return;
	}

#ifdef FOR_BLOODCASTLE
	if(CHECK_BLOODCASTLE(lpObj->MapNumber)) {
		// ���⼭ ����������� ������ �ش�.
		g_BloodCastle.SetUserState(lpObj->m_Index, BC_USER_DEAD);
		// �׸��� ���⼭ ����Ʈ �������� ã�Ƽ� ������ �ش�.
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ�� �ٸ� �ε��� ����
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpObj->MapNumber );
		if (g_BloodCastle.GetCurrentState(iBridgeIndex) == BLOODCASTLE_STATE_PLAYING) {
#else
		if (g_BloodCastle.GetCurrentState(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1) == BLOODCASTLE_STATE_PLAYING) {
#endif
			LogAddTD("[Blood Castle] (%d) Try to drop Ultimate Weapon [%s][%s]",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ�� �ٸ� �ε��� ����
					iBridgeIndex + 1,
#else
					lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1 + 1,
#endif
					lpObj->AccountID, 
					lpObj->Name
					);
			g_BloodCastle.SearchUserDropQuestItem (lpObj->m_Index);
		}
		else {
			LogAddTD("[Blood Castle] (%d) Try to delete Ultimate Weapon [%s][%s]",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ�� �ٸ� �ε��� ����
					iBridgeIndex + 1,
#else
					lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1 + 1,
#endif
					lpObj->AccountID, 
					lpObj->Name
					);
			g_BloodCastle.SearchUserDeleteQuestItem (lpObj->m_Index);
		}

		if( lpTargetObj->Type == OBJTYPE_MONSTER )
		{	// ���� ����� ���� �̸�
			LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpTargetObj->Class);
			if( lpattr )
			{
				LogAddTD("[Blood Castle] (%d) Dead In Blood Castle, Killed by Monster [%s][%s][%s]",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ�� �ٸ� �ε��� ����
				iBridgeIndex + 1,
#else
				lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1 + 1,
#endif
					lpObj->AccountID, 
					lpObj->Name, 
					lpattr->m_Name
					);
				g_BloodCastle.SearchUserDropQuestItem (lpObj->m_Index);
			}
		}
		else if (lpTargetObj->Type == OBJTYPE_CHARACTER) {
			LogAddTD("[Blood Castle] (%d) Dead In Blood Castle, Killed by Other User [%s][%s][%s]",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ�� �ٸ� �ε��� ����
				iBridgeIndex + 1,
#else
				lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1 + 1,
#endif
				lpObj->AccountID, 
				lpObj->Name, 
				lpTargetObj->Name
				);
			g_BloodCastle.SearchUserDropQuestItem (lpObj->m_Index);
		}

		return;
	}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ȯ�� ������� �׾��� ��� ���� ����
	if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
	{
		//�׾��� ��� ó��
		g_IllusionTempleEvent.IllusionTempleUserDie(lpObj);
		return;
	}
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408			// ī���� ĳ�������� ����ġ ���Ұ� ����.
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING
		if(CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
			return;
		}
	#endif
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// ������ �߿� ������ �ʾȿ��� ����� ��� ����ġ ���Ұ� ����.
	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE &&
		lpObj->MapNumber == MAP_INDEX_CASTLESIEGE
		) 
	{
		// MODIFY_ACCUMULATED_BUG_DEAD_BY_CANNONTOWER_01_20050531 ???
		return;
	}
#endif

#ifdef JAPAN_DESTROYKING_EVENT_20040625		// �� Ȥ�� ������ �̺�Ʈ GM (�ı���) �̶�� ����ġ ���Ұ� ����.
	if( lpObj->Authority&CTLCODE_EVENT_GM || lpTargetObj->Authority&CTLCODE_EVENT_GM)
	{
		return;
	}
#endif

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	if( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CHARM_GUARDIAN))
	{
#ifdef MODIFY_DUAL_AFFECT_BUFFTYPE_CHARM_GUARDIAN_20091007
		if( FALSE == IsOnDuel(lpObj->m_Index, lpTargetObj->m_Index) )
#endif //#ifdef MODIFY_DUAL_AFFECT_BUFFTYPE_CHARM_GUARDIAN_20091007
			return;
	}
#endif

	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{	// ���Ͱ� �׿��� ���
		LogAddTD("[%s][%s] Killed [%s][%s] Pos:(%d,%d,%d)",
				 "Monster", lpTargetObj->Name, 
				 lpObj->AccountID, lpObj->Name,
				 lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
	else
	{	// ������ �׿��� ���
		LogAddTD("[%s][%s] (PkLevel = %d) Killed [%s][%s] (PkLevel = %d) Pos:(%d,%d,%d)",
				 lpTargetObj->AccountID, lpTargetObj->Name, 
				 lpTargetObj->m_PK_Level,
				 lpObj->AccountID, lpObj->Name,
				 lpObj->m_PK_Level,
				 lpObj->MapNumber, lpObj->X, lpObj->Y);

#ifdef DUEL_SYSTEM_20031028
				LogAddTD("Killed by User State Victim:[%s][%s], Murderer:[%s][%s], Dead State (Duel:%d, GuildWar:%d, SeldDefense:%d)",
						 lpObj->AccountID, lpObj->Name,
						 lpTargetObj->AccountID, lpTargetObj->Name, 							
						 IsOnDuel (lpTargetObj->m_Index, lpObj->m_Index)?1:0,
						 gObjTargetGuildWarCheck(lpObj, lpTargetObj)?1:0,
						 gObjIsSelfDefense(lpTargetObj, lpObj->m_Index)?1:0		// lpObj:������, lpTargetObj:������
						 );
#endif

	}
	
	//------------------------------------------------------------------------
	// �������� ����Ʈ����.
	//------------------------------------------------------------------------
	//if( *(lpObj->pInventoryCount) < 1 ) return;


#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
	if (!gPkLimitFree) {
			// if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-2 ) itemdrop = (rand()%64);
			 if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-1 ) itemdrop = (rand()%32);
		else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT   ) itemdrop = (rand()%16);
		else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1 ) itemdrop = (rand()%8);
		else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+2 ) itemdrop = (rand()%4);
		else if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3 ) itemdrop = (rand()%2);
		else itemdrop = (rand()%64);
	}
	else {
	#ifdef MODIFY_TAIWAN_FREE_SERVER_SETTING_20080214
			// if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-2 ) itemdrop = (rand()%256);
			 if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-1 ) itemdrop = (rand()%128);
		else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT   ) itemdrop = (rand()%64);
		else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1 ) itemdrop = (rand()%4);
		else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+2 ) itemdrop = (rand()%3);
		else if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3 )
		{
			// 66.6%
			itemdrop = rand() % 100;
			if( itemdrop > 66 )
			{
				itemdrop = 1;
			}
			else
			{
				itemdrop = 0;
			}
			//itemdrop = rand() % 3;
			//itemdrop = ( ( rand() % 100 ) 66 ) );
		}
		else itemdrop = (rand()%256);
	#else
		// if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-2 ) itemdrop = (rand()%256);
		if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-1 ) itemdrop = (rand()%128);
		else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT   ) itemdrop = (rand()%64);
		else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1 ) itemdrop = (rand()%16);
		else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+2 ) itemdrop = (rand()%4);
		else if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3 ) itemdrop = (rand()%2);
		else itemdrop = (rand()%256);
	#endif // MODIFY_TAIWAN_FREE_SERVER_SETTING_20080214
	}
#else
		// if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-2 ) itemdrop = (rand()%64);
	if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-1 ) itemdrop = (rand()%32);
	else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT   ) itemdrop = (rand()%16);
	else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1 ) itemdrop = (rand()%8);
	else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+2 ) itemdrop = (rand()%4);
	else if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3 ) itemdrop = (rand()%2);
	else itemdrop = (rand()%64);
#endif	// ADD_PK_LIMIT_FREE_OPTION_20050324

	
	// ���� �ɼǿ� ���θ����� �׾����� ������ ����� �ȵǰ� �����Ǿ� �ִٸ�...
	if( gPkItemDrop == 0 )
	{
		if( lpTargetObj->m_PK_Level >= PK_LEVEL_DEFAULT+1 )
		{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
			//if (!gPkLimitFree)
#endif
			itemdrop = 1;	// ������ 1�� �����Ѵ�.
		}
	}

	if( gLanguage == 0 )
	{
#ifdef HERO_SYSTEM_MODIFY
		
 
		// ���� ���� ��밡 �÷��̾��� �������� �����ų� ����ġ�� ������ �ʰ� �Ѵ�. (PK�� ���)
		if( lpTargetObj->Type == OBJTYPE_CHARACTER ) 
		{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
			//if (!gPkLimitFree)
#endif

			if ( lpObj->m_PK_Level <= PK_LEVEL_DEFAULT ) {
				itemvanish = 1;	// ������ 1�� �����Ѵ�.
			}
			else if ( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT + 3) {		// ������ �Ҹ�
				// PK���� 3�� ������� PK������ ��
				if (rand()%PK_LEVEL_3_ITEMERASE == 0)
					itemvanish = 0;
				else
					itemvanish = 1;
			}
			else if ( lpObj->m_PK_Level == PK_LEVEL_DEFAULT + 2) {		// ������ �Ҹ�
				// PK���� 2�� ������� PK������ ��
				if (rand()%PK_LEVEL_2_ITEMERASE == 0)
					itemvanish = 0;
				else
					itemvanish = 1;
			}
			else if ( lpObj->m_PK_Level == PK_LEVEL_DEFAULT + 1) {		// ������ �Ҹ�
				// PK���� 1�� ������� PK������ ��
				if (rand()%PK_LEVEL_1_ITEMERASE == 0)
					itemvanish = 0;
				else
					itemvanish = 1;
			}
		}
		else {
			// ���� ���� ��밡 ���Ͷ�� PK�� ��� ��ӷ����� �����Ѵ�.

			itemvanish = 1;		// ������ 1�� �����Ѵ�.

			if ( lpObj->m_PK_Level <= PK_LEVEL_DEFAULT ) {
				itemdrop = 1;	// ������ 1�� �����Ѵ�.
			}
			else if ( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT + 3) {
				if (rand()%PK_LEVEL_3_ITEMDROP == 0)
					itemdrop = 0;
				else
					itemdrop = 1;
			}
			else if ( lpObj->m_PK_Level == PK_LEVEL_DEFAULT + 2) {
				if (rand()%PK_LEVEL_2_ITEMDROP == 0)
					itemdrop = 0;
				else
					itemdrop = 1;
			}
			else if ( lpObj->m_PK_Level == PK_LEVEL_DEFAULT + 1) {
				if (rand()%PK_LEVEL_1_ITEMDROP == 0)
					itemdrop = 0;
				else
					itemdrop = 1;
			}
		}

		
#else
		// ���� ���� ��밡 �÷��̾��� �������� �����ų� ����ġ�� ������ �ʰ� �Ѵ�.
		if( lpTargetObj->Type == OBJTYPE_CHARACTER ) 
		{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
			//if (!gPkLimitFree)
#endif
			itemdrop = 1;	// ������ 1�� �����Ѵ�.
		}
#endif
	}

	if( gObjCanItemTouch(lpObj, 0) == FALSE )	// �������� �̵��� �� ���ٸ�
	{
		itemdrop = 1;
	}

#ifdef FOR_ONAIR	// ��ۿ뿡���� �׾ �������� �������� �ʴ´� (������ �ʵȴ�.)
	itemdrop = 1;
	#ifdef HERO_SYSTEM_MODIFY
		itemvanish = 1;
	#endif
#endif

#ifdef DUEL_SYSTEM_20031028
	if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER ) {
		if (IsOnDuel(lpObj->m_Index, lpTargetObj->m_Index)) {	// ���� ���̶�� � ���� �������� �����ų� ������� �ʴ´�.
			
#ifndef ADD_PK_LIMIT_FREE_OPTION_20050324
			if( lpObj->m_PK_Level < PK_LEVEL_DEFAULT+1 && gPkItemDrop == 0 ) {		// ī���� ������.
#endif
				itemdrop = 1;
#ifdef HERO_SYSTEM_MODIFY				
				itemvanish = 1;
#endif
#ifndef ADD_PK_LIMIT_FREE_OPTION_20050324
			}
#endif
		}
	}
#endif

#ifdef CRYWOLF_MVP_DISABLE_USER_ITEMDROP_20051217
	// ũ���̿��� MVP ���� ������ ����� ���� �ʴ´�.
	if( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_START
		&& CHECK_CRYWOLF_FIRSTZONE(lpObj->MapNumber) )
	{
		itemdrop	= 1;
	}
#endif

	if(lpObj->Type == OBJTYPE_CHARACTER && gPkItemDrop == 0)
	{
		itemdrop = 1;
#ifdef HERO_SYSTEM_MODIFY				
		itemvanish = 1;
#endif
	}

	if( itemdrop == 0 )
	{		
		if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+1 )
		{
			count=MAX_EQUIPMENT*2;
			while(count--)	
			{
				number = (rand()%MAX_EQUIPMENT);
				
				// ������ ������ �ϳ��� ����Ʈ����.
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// ��ȭ�Ǻ��� ��ȭ �������� ��� �� �� ����.
				if( lpObj->pInventory[number].IsItem() == TRUE
					&& g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony( &lpObj->pInventory[number] ) == FALSE )
#else
				if( lpObj->pInventory[number].IsItem() == TRUE )
#endif //ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				{
					PMSG_ITEMTHROW	pDropItem;
					pDropItem.Ipos  = number;
					pDropItem.px    = (BYTE)lpObj->X;
					pDropItem.py	= (BYTE)lpObj->Y;
					if( CGItemDropRequest((LPPMSG_ITEMTHROW)&pDropItem, lpObj->m_Index, 1) == 0x01 ) 
					{
						dropresult = 1;
						break;
					}
				}
			}
		}
		
		// ���� â���� �������� ���ٸ�..
		if( dropresult == 0 )
		{
			// �κ��丮 �߿��� ����Ʈ����.
#ifdef PERSONAL_SHOP_20040113		
			count=MAX_INVENTORY_EXTEND-MAX_EQUIPMENT;
			while(count--)	
			{
				number = MAX_EQUIPMENT+(rand()%(MAX_INVENTORY_EXTEND-MAX_EQUIPMENT));
#else
			count=MAX_INVENTORY-MAX_EQUIPMENT;
			while(count--)	
			{
				number = MAX_EQUIPMENT+(rand()%(MAX_INVENTORY-MAX_EQUIPMENT));
#endif

				// ������ �ϳ��� ����Ʈ����.
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// ��ȭ�Ǻ��� ��ȭ �������� ��� �� �� ����.
				if( lpObj->pInventory[number].IsItem() == TRUE
					&& g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony( &lpObj->pInventory[number] ) == FALSE )
#else
				if( lpObj->pInventory[number].IsItem() == TRUE )
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				{
#ifdef USER_DIE_NOT_DROP_HERORING_20040330		// ���, ������ ������ ��ӵ��� �ʴ´�.
					if( lpObj->pInventory[number].m_Type>=ITEM_HELPER+20 &&
						(	lpObj->pInventory[number].m_Level >= 1 &&		// ����ǹ���
							lpObj->pInventory[number].m_Level <= 2			// �����ǹ���
						))
					{
						continue;
					}
#endif
					
					PMSG_ITEMTHROW	pDropItem;
					pDropItem.Ipos  = number;
					pDropItem.px    = (BYTE)lpObj->X;
					pDropItem.py	= (BYTE)lpObj->Y;
					if( CGItemDropRequest((LPPMSG_ITEMTHROW)&pDropItem, lpObj->m_Index, 1) == 0x01 ) 
					{
						dropresult = 1;
						LogAddTD("[%s][%s] User Die Item Drop, ItemName:%s", lpObj->AccountID, lpObj->Name, lpObj->pInventory[number].GetName());
						break;
					}
				}
			}
		}
	}


#ifdef HERO_SYSTEM_MODIFY

	if( itemvanish == 0 )
	{		
		if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+1 )
		{
			count=MAX_EQUIPMENT*2;
			while(count--)	
			{
				number = (rand()%MAX_EQUIPMENT);
				
				// ������ ������ �ϳ��� �Ҹ��Ų��.
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// ��ȭ�Ǻ��� ��ȭ �������� �Ҹ��ų �� ����.
				if( lpObj->pInventory[number].IsItem() == TRUE
					&& g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony( &lpObj->pInventory[number] ) == FALSE )
#else
				if( lpObj->pInventory[number].IsItem() == TRUE )
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				{
#ifdef USER_DIE_NOT_DROP_HERORING_20040330		// ���, ������ ������ �Ҹ���� �ʴ´�.
					if( lpObj->pInventory[number].m_Type>=ITEM_HELPER+20 &&
						(	lpObj->pInventory[number].m_Level >= 1 &&		// ����ǹ���
							lpObj->pInventory[number].m_Level <= 2			// �����ǹ���
						))
					{
						continue;
					}
#endif

					// ������ �ϳ��� �Ҹ��Ų��.
					gObjInventoryItemSet(lpObj->m_Index, number, 0xFF);
					lpObj->pInventory[number].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, number, 1);
					dropresult = 1;

					LogAddTD("[%s][%s] User Die Item Vanish, ItemName:%s", lpObj->AccountID, lpObj->Name, lpObj->pInventory[number].GetName());
					break;
				}
			}
		}

		// ���� â���� �������� ���ٸ�..
		if( dropresult == 0 )
		{
			// �κ��丮 �߿��� ã�Ƽ� �Ҹ��Ų��.
#ifdef PERSONAL_SHOP_20040113		
			count=MAX_INVENTORY_EXTEND-MAX_EQUIPMENT;
			while(count--)	
			{
				number = MAX_EQUIPMENT+(rand()%(MAX_INVENTORY_EXTEND-MAX_EQUIPMENT));
#else
			count=MAX_INVENTORY-MAX_EQUIPMENT;
			while(count--)	
			{
				number = MAX_EQUIPMENT+(rand()%(MAX_INVENTORY-MAX_EQUIPMENT));
#endif

				// ������ ������ �ϳ��� �Ҹ��Ų��.
				if( lpObj->pInventory[number].IsItem() == TRUE )
				{
#ifdef USER_DIE_NOT_DROP_HERORING_20040330		// ���, ������ ������ ��ӵ��� �ʴ´�.
					if( lpObj->pInventory[number].m_Type>=ITEM_HELPER+20 &&
						(	lpObj->pInventory[number].m_Level >= 1 &&		// ����ǹ���
							lpObj->pInventory[number].m_Level <= 2			// �����ǹ���
						))
					{
						continue;
					}
#endif

					// ������ �ϳ��� �Ҹ��Ų��.
					gObjInventoryItemSet(lpObj->m_Index, number, 0xFF);
					lpObj->pInventory[number].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, number, 1);
					dropresult = 1;

					LogAddTD("[%s][%s] User Die Item Vanish, ItemName:%s", lpObj->AccountID, lpObj->Name, lpObj->pInventory[number].GetName());
					break;
				}
			}
		}
	}

#endif


#ifdef DUEL_SYSTEM_20031028
	// ��������� ����ġ �ȱ��̰�..
	if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER ) 
	{
		if (IsOnDuel(lpObj->m_Index, lpTargetObj->m_Index)) 
		{
#ifdef ADD_NEWPVP_PKFIELD
			g_NewPVP.SetScore(*lpTargetObj);
#else
			lpTargetObj->m_btDuelScore++;
			GCSendDuelScore(lpObj->m_Index, lpTargetObj->m_Index);
#endif // ADD_NEWPVP_PKFIELD
		}


		return;	
	}
#else
	// ��������� ����ġ �ȱ��̰�..
	if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER ) return;
#endif	
	
	//------------------------------------------------------------------------
	// ����ġ�� ���� ��Ų��.
	//------------------------------------------------------------------------
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	
	INT64 minexp = gLevelExperience[lpObj->Level-1];
	INT64 maxexp = gLevelExperience[lpObj->Level];
	INT64 subexp = 0;
	INT64 decexp = 0;
#else
	DWORD minexp = gLevelExperience[lpObj->Level-1];
	DWORD maxexp = gLevelExperience[lpObj->Level];
	DWORD subexp = 0;
	DWORD decexp = 0;
#endif

	int	decexprate = 0;		// ���� ����

	// ���θ��� ��Ƽ�� ����ġ ���� ����
	char tmpPKLevel = PK_LEVEL_DEFAULT;	// �ӽ� PK ����

	if(lpObj->PartyNumber >= 0)
	{
#ifdef UPDATE_PK_SYSTEM_20070302
		// ��Ƽ���� ���
		if(gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= PK_LEVEL_DEFAULT+2)
		{
			tmpPKLevel = gParty.GetPKPartyPenalty(lpObj->PartyNumber);
		}
		else
#endif
		{
			tmpPKLevel = lpObj->m_PK_Level;
		}
	}
	else
	{
		tmpPKLevel = lpObj->m_PK_Level;
	}


#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// ������ ��ġ ����
	// decexprate 1/1000 �� ����.
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
	{
		 // 1. min/max ����ġ ����
		minexp = g_MasterLevelSystem.GetMasterLevelExpTlbInfo( lpObj->m_nMasterLevel );
		maxexp = g_MasterLevelSystem.GetMasterLevelExpTlbInfo( lpObj->m_nMasterLevel + 1 );	
		
		// 2. ������ ����
		decexprate = g_MasterLevelSystem.GetDieDecExpRate( lpObj );
	}	
	else	// �Ϲ� ����.
	{
		if( lpObj->Level <= 10  )
		{	// 5���ϴ� ����ġ ���� ����
			decexprate = 0;
		}
		else if( lpObj->Level <= 150  )
		{
			if( tmpPKLevel == PK_LEVEL_DEFAULT-1 ) decexprate = 20;		// ����1�ܰ�
			else if( tmpPKLevel == PK_LEVEL_DEFAULT   ) decexprate = 10;	// �Ϲ���
			else if( tmpPKLevel == PK_LEVEL_DEFAULT+1 ) decexprate = 50;	// ���θ� 1�ܰ�
			else if( tmpPKLevel == PK_LEVEL_DEFAULT+2 ) decexprate = 60;	// ���θ� 2�ܰ�
			else if( tmpPKLevel >= PK_LEVEL_DEFAULT+3 ) decexprate = 70;	// ���θ� 3�ܰ�
			else decexprate = 20;					// ���� 2�ܰ�
		}
		else if( lpObj->Level <= 220 )
		{
			if( tmpPKLevel == PK_LEVEL_DEFAULT-1 ) decexprate = 10;
			else if( tmpPKLevel == PK_LEVEL_DEFAULT   ) decexprate = 10;
			else if( tmpPKLevel == PK_LEVEL_DEFAULT+1 ) decexprate = 40;
			else if( tmpPKLevel == PK_LEVEL_DEFAULT+2 ) decexprate = 50;
			else if( tmpPKLevel >= PK_LEVEL_DEFAULT+3 ) decexprate = 60;
			else decexprate = 10;
		}
		else 
		{
			if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-1 ) decexprate = 10;
			else if( tmpPKLevel == PK_LEVEL_DEFAULT   ) decexprate = 10;
			else if( tmpPKLevel == PK_LEVEL_DEFAULT+1 ) decexprate = 30;
			else if( tmpPKLevel == PK_LEVEL_DEFAULT+2 ) decexprate = 40;
			else if( tmpPKLevel >= PK_LEVEL_DEFAULT+3 ) decexprate = 50;
			else decexprate = 10;
		}
	}

#ifdef MODIFY_MINLEVEL_EXPRATE_20080630
	decexp = 0;
	if (decexprate > 0) {
		decexp = ( ( maxexp - minexp ) * decexprate ) / 1000;
	}
#else
	if( decexprate <= 0)
		decexprate = 1;
	
	decexp = ( ( maxexp - minexp ) * decexprate ) / 1000;
#endif // MODIFY_MINLEVEL_EXPRATE_20080630

#else	//MASTER_LEVEL_UP_SYSTEM_20070912
#ifdef UPDATE_PK_SYSTEM_20070302		// ���θ��� ��Ƽ�� ����ġ ���� ����
	if(lpObj->PartyNumber >= 0)
	{
		// ��Ƽ���� ���
		if(gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= PK_LEVEL_DEFAULT+2)
		{
			tmpPKLevel = gParty.GetPKPartyPenalty(lpObj->PartyNumber);
		}
		else
		{
			tmpPKLevel = lpObj->m_PK_Level;
		}
	}
	else
	{
		tmpPKLevel = lpObj->m_PK_Level;
	}

	if( lpObj->Level <= 10  )
	{	// 5���ϴ� ����ġ ���� ����
		decexprate = 0;
	}
	else if( lpObj->Level <= 150  )
	{
		if( tmpPKLevel == PK_LEVEL_DEFAULT-1 ) decexprate = 2;		// ����1�ܰ�
		else if( tmpPKLevel == PK_LEVEL_DEFAULT   ) decexprate = 1;	// �Ϲ���
		else if( tmpPKLevel == PK_LEVEL_DEFAULT+1 ) decexprate = 5;	// ���θ� 1�ܰ�
		else if( tmpPKLevel == PK_LEVEL_DEFAULT+2 ) decexprate = 6;	// ���θ� 2�ܰ�
		else if( tmpPKLevel >= PK_LEVEL_DEFAULT+3 ) decexprate = 7;	// ���θ� 3�ܰ�
		else decexprate = 2;					// ���� 2�ܰ�
	}
	else if( lpObj->Level <= 220 )
	{
		if( tmpPKLevel == PK_LEVEL_DEFAULT-1 ) decexprate = 1;
		else if( tmpPKLevel == PK_LEVEL_DEFAULT   ) decexprate = 1;
		else if( tmpPKLevel == PK_LEVEL_DEFAULT+1 ) decexprate = 4;
		else if( tmpPKLevel == PK_LEVEL_DEFAULT+2 ) decexprate = 5;
		else if( tmpPKLevel >= PK_LEVEL_DEFAULT+3 ) decexprate = 6;
		else decexprate = 1;
	}
	else 
	{
		if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT-1 ) decexprate = 1;
		else if( tmpPKLevel == PK_LEVEL_DEFAULT   ) decexprate = 1;
		else if( tmpPKLevel == PK_LEVEL_DEFAULT+1 ) decexprate = 3;
		else if( tmpPKLevel == PK_LEVEL_DEFAULT+2 ) decexprate = 4;
		else if( tmpPKLevel >= PK_LEVEL_DEFAULT+3 ) decexprate = 5;
		else decexprate = 1;
	}
#endif	// UPDATE_PK_SYSTEM_20070302

	decexp = (((maxexp-minexp)*decexprate)/100);
#endif //MASTER_LEVEL_UP_SYSTEM_20070912
	

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
	if( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_START 
		&& lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
	{
		// MVP �� ����ġ �ս��� 0% �̴�.
		decexp = decexp * 0;
	}
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	// �Ϲ� ������ �����ͷ��� ������ ���� ����� ��� �Ѵ�.
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
	{
		subexp = lpObj->m_i64MasterLevelExp - decexp;
	}
	else
	{
		subexp = lpObj->Experience-decexp;
	}
#else
	subexp = lpObj->Experience-decexp;
#endif
	if( subexp < minexp ) subexp = minexp;

//#BUGFIX_20040602_ExpLog (apple)
	LogAddTD("[%s][%s] Death reduces Experience %u - %d", lpObj->AccountID, lpObj->Name, subexp, decexp);
	//------------------------------------------------------------------------
	// ���� ���� ��Ų��.
	//------------------------------------------------------------------------
#ifdef USER_DIE_ZEN_BUG_PATCH_20040319
	DWORD submoney = 0;
#else
	int submoney = 0;
#endif	

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	// ���� �� ���� �� Ȯ�� ����
	int iDownMoneyRate = 0;
#endif
	
	// ������ �� �A�̰�
	if( lpObj->Money > 0 )
	{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912			// ������ �� ���̰�
		iDownMoneyRate = g_MasterLevelSystem.GetDieDecZenRate( lpObj );
		if( iDownMoneyRate < 0 )	// �Ϲ� �������
		{
			if(	lpObj->Level >    10 && lpObj->Level <=  99 )		iDownMoneyRate = 1;
			else if( lpObj->Level >= 100 && lpObj->Level <= 199 )	iDownMoneyRate = 2;
			else if( lpObj->Level >  200 )							iDownMoneyRate = 3;
		}

		if( iDownMoneyRate > 0 )
		{
			submoney = ( (DWORD)lpObj->Money * iDownMoneyRate ) / 100;
		}
#else
		if(		 lpObj->Level >    10 && lpObj->Level <=  99 ) submoney = ((DWORD)lpObj->Money*1)/100;
		else if( lpObj->Level >= 100 && lpObj->Level <= 199 ) submoney = ((DWORD)lpObj->Money*2)/100;
		else if( lpObj->Level >  200 )						  submoney = ((DWORD)lpObj->Money*3)/100;
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912

		lpObj->Money -= submoney;
		if( lpObj->Money < 0 ) lpObj->Money = 0;
		LogAddTD(lMsg.Get(574), lpObj->AccountID, lpObj->Name, lpObj->Money, submoney);
	}
	// â������ �� ���̰�..
	if( lpObj->WarehouseMoney > 0 )
	{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912			// â������ �� ���̰�
		submoney = 0;

		iDownMoneyRate = g_MasterLevelSystem.GetDieDecZenRate( lpObj );
		if( iDownMoneyRate < 0 )
		{
			if(		 lpObj->Level >    10 && lpObj->Level <=  99 ) iDownMoneyRate = 1;
			else if( lpObj->Level >= 100 && lpObj->Level <= 199 ) iDownMoneyRate = 2;
			else if( lpObj->Level >  200 )						  iDownMoneyRate = 3;
		}

		if( iDownMoneyRate > 0 )
		{
			submoney = ( (DWORD)lpObj->Money * iDownMoneyRate ) / 100;
		}
#else
		if(		 lpObj->Level >    10 && lpObj->Level <=  99 ) submoney = ((DWORD)lpObj->WarehouseMoney*1)/100;
		else if( lpObj->Level >= 100 && lpObj->Level <= 199 ) submoney = ((DWORD)lpObj->WarehouseMoney*2)/100;
		else if( lpObj->Level >  200 )						  submoney = ((DWORD)lpObj->WarehouseMoney*3)/100;
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912
		
		lpObj->WarehouseMoney -= submoney;
		if( lpObj->WarehouseMoney < 0 ) lpObj->WarehouseMoney = 0;
		GDSetWarehouseMoney(lpObj->m_Index);
		LogAddTD(lMsg.Get(575), lpObj->AccountID, lpObj->Name, lpObj->WarehouseMoney, submoney);
	}

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	// �Ϲ� ������ �����ͷ��� ������ ���� ����� ��� �Ѵ�.
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
	{
		lpObj->m_i64MasterLevelExp = subexp;
	}
	else
	{
		lpObj->Experience = subexp;
	}
#else	
	lpObj->Experience = subexp;
#endif
}

void gObjInterfaceTimeCheck(LPOBJECTSTRUCT lpObj)
{
	int tObjNum;
	int TimeCheck=0;
	if( !lpObj->m_IfState.use ) return;
	
	if( (GetTickCount()-lpObj->m_InterfaceTime) < 5000  ) return;

	// �������̸� ����
	if( lpObj->m_IfState.state == 1 ) return;
	
	tObjNum =lpObj->TargetNumber;
	if( tObjNum < 0 ) return;

	if( lpObj->m_IfState.type == I_TRADE )
	{
		CGTradeResult(lpObj->m_Index, 0x03);
		CGTradeResult(tObjNum, 0x03);
		GCServerMsgStringSend(lMsg.Get(1138), tObjNum, 1);
		TimeCheck = 1;
	}
	else if( lpObj->m_IfState.type == I_PARTY )
	{
		lpObj->PartyTargetUser	= -1;
		gObj[tObjNum].PartyTargetUser  = -1;
		GCResultSend(tObjNum, 0x41, 0x00);	// ��� ��Ų��.
		TimeCheck = 1;
	}
	//else if( lpObj->m_IfState.type == I_GUILD)
	//{
	//}
	// 2002/10/15 ��ġ�� ��Ƽ�� �ŷ��ÿ��� �ڵ� ��ҵ�
	if(TimeCheck)
	{
		lpObj->m_IfState.use			=  0;
		lpObj->TargetNumber				= -1;
		gObj[tObjNum].m_IfState.use		=  0;
		gObj[tObjNum].TargetNumber		= -1;
	}
}

#define PK_LEVEL_DOWN_TIME1	(60*60*1)
#define PK_LEVEL_DOWN_TIME	(60*60*3)
#define PK_LEVEL_UP_TIME	(60*60*1)

#ifdef	HERO_SYSTEM_MODIFY
#define PK_COUNT_3_TIME		(60*60*15)		// PKī��Ʈ�� 3�϶� ���ӽð�
#define PK_COUNT_2_TIME		(60*60*8)		// PKī��Ʈ�� 2�϶� ���ӽð�
#define PK_COUNT_1_TIME		(60*60*3)		// PKī��Ʈ�� 1�϶� ���ӽð�
#endif


// PK������ ������ ������� �ð��� �����Ŀ� ������ ���ҽ�Ų��.
void gObjPkDownTimeCheck(LPOBJECTSTRUCT lpObj, int TargetLevel)
{
	if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT ) return;
	

	DWORD dwtime = ((GetTickCount()-lpObj->m_dwPKTimer)/1000)*gPkTime;
	if( dwtime > 5 )
	{
		dwtime = 2;
		lpObj->m_dwPKTimer = GetTickCount();
	}
	else
	{
		lpObj->m_dwPKTimer = + dwtime*1000;
	}

	//LogAdd("PK Time = %d", dwtime);

	if( TargetLevel < 2 ) lpObj->m_PK_Time+=dwtime;
	//if( TargetLevel < 2 ) lpObj->m_PK_Time+=0;
#ifdef UPDATE_PK_SYSTEM_20070302	// ���θ� ���� �ð� ���
	else lpObj->m_PK_Time += (int)(TargetLevel / 2);
#else
	else lpObj->m_PK_Time += TargetLevel;
#endif

	if( lpObj->m_PK_Level < PK_LEVEL_DEFAULT )	// �����϶�
	{
		if( lpObj->m_PK_Time > PK_LEVEL_UP_TIME )
		{
			lpObj->m_PK_Time  = 0;
			lpObj->m_PK_Count++;
			if( lpObj->m_PK_Count > 100 ) lpObj->m_PK_Count = 100;
			lpObj->m_PK_Level++;
			GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
		}
	}
	else if( lpObj->m_PK_Level > PK_LEVEL_DEFAULT )	// ������ �� ��쿣
	{
		if( lpObj->m_PK_Count > 3 )	// ���� 3�� �̻��̸� �ѻ���� �ѽð��� ����..
		{
			if( lpObj->m_PK_Time > PK_LEVEL_DOWN_TIME1 )
			{
				lpObj->m_PK_Time  = 0;
				lpObj->m_PK_Count--;
				if( lpObj->m_PK_Count <=0 ) {
					lpObj->m_PK_Count = 0;
					lpObj->m_PK_Level = PK_LEVEL_DEFAULT;
				}
				else if( lpObj->m_PK_Count == 2 )
				{
					lpObj->m_PK_Level = PK_LEVEL_DEFAULT+2;
				}
				else if( lpObj->m_PK_Count == 1 )
				{
					lpObj->m_PK_Level = PK_LEVEL_DEFAULT+1;
				}
				// ���� ��� 2 �����϶��� �޽����� ������.
				if( lpObj->m_PK_Level <= PK_LEVEL_DEFAULT+2 )
				{
					GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
				}			
				if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1 )
				{	
					GCServerMsgStringSend(lMsg.Get(1137), lpObj->m_Index, 1);

#ifdef UPDATE_PK_SYSTEM_20070302	// ���θ� �ܰ谡 �����ϸ� ��Ƽ������ ó��
					if(lpObj->PartyNumber >= 0)	// ��Ƽ�� �ϰ� ���� ���
					{
						gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
						gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
					}
#endif
				}
			}
		}
		else 
		{
#ifdef HERO_SYSTEM_MODIFY
			switch(lpObj->m_PK_Count) {
			case 3 :	// PKī��Ʈ 3 - 15 �ð� ����
				{
					if( lpObj->m_PK_Time > PK_COUNT_3_TIME - PK_COUNT_2_TIME)
					{
						lpObj->m_PK_Time  = 0;
						lpObj->m_PK_Count--;
						lpObj->m_PK_Level = PK_LEVEL_DEFAULT+2;

						GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
					}
				}
				break;
			case 2 :	// PKī��Ʈ 2 - 8 �ð� ����
				{
					if( lpObj->m_PK_Time > PK_COUNT_2_TIME - PK_COUNT_1_TIME)
					{
						lpObj->m_PK_Time  = 0;
						lpObj->m_PK_Count--;
						lpObj->m_PK_Level = PK_LEVEL_DEFAULT+1;
						
						GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
						GCServerMsgStringSend(lMsg.Get(1137), lpObj->m_Index, 1);
					}
				}
				break;
			case 1 :	// PKī��Ʈ 1 - 3 �ð� ����
				{
					if( lpObj->m_PK_Time > PK_COUNT_1_TIME )
					{
						lpObj->m_PK_Time  = 0;
						lpObj->m_PK_Count--;
						lpObj->m_PK_Level = PK_LEVEL_DEFAULT;

						GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
					}
				}
				break;
			default:	// PKī��Ʈ 1 ���� ???
				{
					lpObj->m_PK_Count = 0;
					lpObj->m_PK_Level = PK_LEVEL_DEFAULT;

					LogAddTD("[PK_Timer] Error : PK Count < 0 = %d  [%s][%s]",
							lpObj->m_PK_Count,
							lpObj->AccountID,
							lpObj->Name
						);
				}
				break;
			}

#else
			if( lpObj->m_PK_Time > PK_LEVEL_DOWN_TIME )
			{
				lpObj->m_PK_Time  = 0;
				lpObj->m_PK_Count--;
				if( lpObj->m_PK_Count <=0 ) {
					lpObj->m_PK_Count = 0;
					lpObj->m_PK_Level = PK_LEVEL_DEFAULT;
				}
				else if( lpObj->m_PK_Count == 2 )
				{
					lpObj->m_PK_Level = PK_LEVEL_DEFAULT+2;
				}
				else if( lpObj->m_PK_Count == 1 )
				{
					lpObj->m_PK_Level = PK_LEVEL_DEFAULT+1;
				}
				// ���� ��� 2 �����϶��� �޽����� ������.
				if( lpObj->m_PK_Level <= PK_LEVEL_DEFAULT+2 )
				{
					GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
				}			
				if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1 )
				{	
					GCServerMsgStringSend(lMsg.Get(1137), lpObj->m_Index, 1);

#ifdef UPDATE_PK_SYSTEM_20070302	// ���θ� �ܰ谡 �����ϸ� ��Ƽ������ ó��
					if(lpObj->PartyNumber >= 0)	// ��Ƽ�� �ϰ� ���� ���
					{
						gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
						gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
					}
#endif
				}
			}
#endif			
		}
	}	
}

// õ�������� ������ ������ ���� ĳ���Ϳ� ���� ������ /10�� �Ѵ�..
// /10�� �ϴ� ������ (�ִ밡 256, �ѹ���Ʈ 
BOOL gObjAngelSprite(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	// �������� ������ ����
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() == FALSE ) return FALSE;

	CItem	*Angel = &lpObj->pInventory[EQUIPMENT_HELPER];
	
	// õ�������� �̸�..
	if( Angel->m_Type == MAKE_ITEMNUM(13,0)) 
	{
		if( Angel->m_Durability > 0 )
		{			
			return TRUE;
		}
	}
	return FALSE;
}

// ��ź������ ������ �����Ҷ����� ���ݷ� * 1.3 �ǰ� HP ����
BOOL gObjSatanSprite(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	// �������� ������ ����
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() == FALSE ) return FALSE;

	CItem	*Satan = &lpObj->pInventory[EQUIPMENT_HELPER];
		
	// ��ź������ �̸�..
	if( Satan->m_Type == MAKE_ITEMNUM(13,1) )
	{
		if( Satan->m_Durability > 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

#ifdef	ADD_SAFEGUARD_ITEM_20080702
// ���������� ������ ���� 40% ��� ���ݼӵ� +10
BOOL gObjDaemonSprite(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;
	
	// �������� ������ ����
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() == FALSE ) return FALSE;
	
	CItem	*Daemon = &lpObj->pInventory[EQUIPMENT_HELPER];
	
	// ���� �̸�..
	if( Daemon->m_Type == MAKE_ITEMNUM(13, 64) )
	{
		if( Daemon->m_Durability > 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

// ��ȣ������ ������ �������� 30% ���
BOOL gObjSafeGuardSprite(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;
	
	// �������� ������ ����
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() == FALSE ) return FALSE;
	
	CItem	*SafeGuard = &lpObj->pInventory[EQUIPMENT_HELPER];
	
	// ��ȣ������ �̸�..
	if( SafeGuard->m_Type == MAKE_ITEMNUM(13, 65) )
	{
		if( SafeGuard->m_Durability > 0 )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

#endif // ADD_SAFEGUARD_ITEM_20080702

// ���� ������ ������ �����Ҷ����� ���ݷ� * 1.3 �ǰ� HP ����
BOOL gObjWingSprite(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	// �������� ������ ����
	if( lpObj->pInventory[EQUIPMENT_WING].IsItem() == FALSE ) return FALSE;

	CItem	*Wing = &lpObj->pInventory[EQUIPMENT_WING];
		
	// ��ź������ �̸�..
#ifdef ADD_THIRD_WING_20070525	// gObjWingSprite(...)�� 3������ �߰�
	if( ( Wing->m_Type >= MAKE_ITEMNUM( 12, 0 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 6 ) )
		|| Wing->m_Type <= MAKE_ITEMNUM( 13, 30 ) 	// ����
		|| ( Wing->m_Type >= MAKE_ITEMNUM( 12, 36 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 40 ) )	// 3�� ����
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912 // gObjWingSprite(...)�� �ű�ĳ���� 1������(����� ����) �߰�
		|| Wing->m_Type == MAKE_ITEMNUM( 12, 41 ) 	// �ű�ĳ���� 1������ (����� ����)
#endif
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	//gObjWingSprite(...) �� ������ ����, ������ ���� 
		|| Wing->m_Type == MAKE_ITEMNUM( 12, 42 )
		|| Wing->m_Type == MAKE_ITEMNUM( 12, 43 )
#endif		
		)
#else
#ifdef DARKLORD_WORK
	if( (Wing->m_Type >= MAKE_ITEMNUM(12,0) && Wing->m_Type <= MAKE_ITEMNUM(12,6)) || 
		Wing->m_Type <= MAKE_ITEMNUM(13,30) )	// ���� ����
#else
	if( Wing->m_Type >= MAKE_ITEMNUM(12,0) && Wing->m_Type <= MAKE_ITEMNUM(12,6) )
#endif
#endif	// ADD_THIRD_WING_20070525
	{
		if( Wing->m_Durability > 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}


BOOL gObjUniriaSprite(LPOBJECTSTRUCT lpObj)
{
	// �������� ������ ����
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() == FALSE ) return FALSE;

	CItem	*Uniria = &lpObj->pInventory[EQUIPMENT_HELPER];
	
	// ��ź������ �̸�..
	if( Uniria->m_Type == MAKE_ITEMNUM(13,2))
	{
		if( Uniria->m_Durability > 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

// ���ݹ����� ���ɵ��� �������� ���� ��Ų��. 
void gObjSpriteDamage(LPOBJECTSTRUCT lpObj, int damage)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return;
	BOOL send_dur=FALSE;

	// �������� ������ ����
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() == FALSE ) return;

	float fN = 10.0;
/*
// pc�� ���� ���� 20090128 grooving
#ifdef PCBANG_ADVANTAGE //happycat@20050203 (PC������) - ������	
#ifdef PCBANG_ADVANTAGE_EXPAND_20060919	
	if( IsOKPCBangBenefitAll(lpObj) )
#else
	if (lpObj->m_bPCBangUser)
#endif//PCBANG_ADVANTAGE_EXPAND_20060919
	{
		fN = 20.0;
	}
#endif//PCBANG_ADVANTAGE

#ifdef PCBANG_FREE_KALIMA_EVENT_20060124
	if( lpObj->m_bPCBangCouponUser )
	{
		fN = 20.0;
	}
#endif //PCBANG_FREE_KALIMA_EVENT_20060124
*/
	CItem	*sprite = &lpObj->pInventory[EQUIPMENT_HELPER];
	float	fdamage = (float)damage;

	if( sprite->m_Type == MAKE_ITEMNUM(13,0)
#if GAME_VERSION >= G_V_S3_E1
		|| sprite->m_Type == MAKE_ITEMNUM(13, 67)
#endif
		)	// ��ȣõ��, �絹����
	{
		fdamage  = (float)(damage*3)/10;								
		fdamage /= fN;							// happycat@20050201

		sprite->m_Durability -= fdamage;
		send_dur = TRUE;
	}
	else if( sprite->m_Type == MAKE_ITEMNUM(13,1) )	// ��ź
	{
		fdamage  = (float)(damage*2)/10;
		fdamage /= fN;							// happycat@20050201
		sprite->m_Durability -= fdamage;
		send_dur = TRUE;
	}
#ifdef ADD_PREMIUMITEM_PANDA
	else if( sprite->m_Type == MAKE_ITEMNUM(13, 80)) // �Ҵ���
	{
		fdamage  = (float)(damage*2)/50;
		fdamage /= fN;							// happycat@20050201
		
		sprite->m_Durability -= fdamage;
		send_dur = TRUE;
	}
#endif // ADD_PREMIUMITEM_PANDA
#ifdef ADD_UNICORN_PET_20090915
	else if( sprite->m_Type == MAKE_ITEMNUM(13, 106) )	// ������ ��
	{
		fdamage  = (float)(damage*2)/10;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = TRUE;
	}
#endif // ADD_UNICORN_PET_20090915

#ifdef	ADD_SAFEGUARD_ITEM_20080702
	//------>
	//grooving �Ϻ� : ����, ��ȣ���� ������ 

#ifdef MODIFY_DEMON_AND_SPRITE_DURABILITY_20081023
	if( sprite->m_Type == MAKE_ITEMNUM(13,64) || sprite->m_Type == MAKE_ITEMNUM(13,65) ){
		// ����� ��ȣ ������ ������ ���Ҹ� �ٿ��ش�. by natinda
		fN += 10.0f;
	}
#endif // MODIFY_DEMON_AND_SPRITE_DURABILITY_20081023

	if( sprite->m_Type == MAKE_ITEMNUM(13, 64) )	// ����
	{

#if defined (PREMIUMITEM_20090106) && defined (FOR_KOREA)
		/*
		// pc�� ���� ���� 20090128 grooving
		if ( IsOKPCBangBenefitAll(lpObj) == TRUE )
		{
			//PC���� ���
			fdamage  = (float)(damage*2)/10;
			fdamage /= 200;
		}
		else
		*/
		{
			fdamage  = (float)(damage*2)/10;
			fdamage /= 100;
		}
#else //PREMIUMITEM_20090106
		fdamage  = (float)(damage*3)/10;
		fdamage /= fN;
#endif //PREMIUMITEM_20090106

		sprite->m_Durability -= fdamage;
		send_dur = TRUE;
	}
	else if( sprite->m_Type == MAKE_ITEMNUM(13, 65) )	// ��ȣ����
	{
#if defined (PREMIUMITEM_20090106) && defined (FOR_KOREA)
		{
			fdamage  = (float)(damage*2)/10;
			fdamage /= 100;
		}
#else
		fdamage  = (float)(damage*2)/10;
		fdamage /= fN;
#endif // PREMIUMITEM_20090106
		sprite->m_Durability -= fdamage;
		send_dur = TRUE;
	}
	//<------
#endif // ADD_SAFEGUARD_ITEM_20080702
	else if( sprite->m_Type == MAKE_ITEMNUM(13,2) )	// ���ϸ����� �� 
	{
		fdamage  = (float)(damage*1)/10;
		fdamage /= 10;
		sprite->m_Durability -= fdamage;
		send_dur = TRUE;
	}
#ifdef NEW_SKILL_FORSKYLAND
	else if( sprite->m_Type == MAKE_ITEMNUM(13,3) )	// ����Ʈ
	{
		fdamage  = (float)(damage*1)/20;
		fdamage /= 10;
		sprite->m_Durability -= fdamage;
		send_dur = TRUE;
	}
#endif
#ifdef DARKLORD_WORK
	else if( sprite->m_Type == MAKE_ITEMNUM(13,4) )	// ��ũȣ��
	{
		if( sprite->m_Durability < 1 )			
			return;				// �׾���

		fdamage  = (float)(damage*2)/10;
		fdamage /= 10;
		fdamage++;

		sprite->m_DurabilitySmall += (WORD)fdamage;

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �� ���� �Ҹ� �ӵ� ���� (��ũȣ��)
		if( sprite->m_DurabilitySmall > 1500 + lpObj->m_MPSkillOpt.iMpsPetDurDownSpeed)
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
		if( sprite->m_DurabilitySmall > 1500 )
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
		{
			sprite->m_Durability--;
			sprite->m_DurabilitySmall = 0;
			send_dur = TRUE;

			if( sprite->m_Durability < 1 )	
			{
				sprite->m_Durability = 0.f;
				int	exp = lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp;
				if( sprite->DecPetItemExp(10) )	// ����ġ 10% ����
				{	// ����ġ ���ҿ� ���� ���� �ٿ��� �����
					LogAddTD("[%s][%s][PetItemExpDown] [%s] Level:[%d]Exp:[%d]DecExp[%d]", 
							lpObj->AccountID, lpObj->Name,
							lpObj->pInventory[EQUIPMENT_HELPER].GetName(),
							lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Level,
							lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Exp,
							exp-lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp);
					CDarkSpirit::SendLevelmsg(lpObj->m_Index, EQUIPMENT_HELPER, 1, 255);
				}

	#if TESTSERVER == 1
				DebugOutput(lpObj->m_Index, "[%s] LevelDown Lv:%d Exp:%d", 
							lpObj->pInventory[EQUIPMENT_HELPER].GetName(),				
							lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Level, 
							lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Exp);	
	#endif

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s] ��ũȣ���ǻ� Item is Broken because durability is exhausted [%u]",
	#else
				LogAddTD("[%s][%s] ��ũȣ���ǻ� Item is Broken because durability is exhausted [%d]",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_HELPER].m_Number );
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
			}
			GCItemDurSend(lpObj->m_Index, EQUIPMENT_HELPER, (BYTE)sprite->m_Durability, 0);
		}	
		
		return;
	}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
	// �渱 ������ ����	
	else if( sprite->m_Type == MAKE_ITEMNUM(13,37) )	// �渱
	{
		if( sprite->m_Durability < 1 )			
			return;				
		
		int iMaxDurSmall = g_iFenrirDefaultMaxDurSmall;
		int iMultipleDmg = 2;

		if( lpObj->Class == CLASS_WIZARD )
		{

		}
		else if( lpObj->Class == CLASS_KNIGHT )
		{

		}
		else if( lpObj->Class == CLASS_ELF )
		{
			iMaxDurSmall = g_iFenrirElfMaxDurSmall;
			iMultipleDmg = 2;
		}
		else if( lpObj->Class == CLASS_MAGUMSA)
		{

		}
		else if( lpObj->Class == CLASS_DARKLORD)
		{

		}

		fdamage  = (float)(damage*iMultipleDmg)/10;
		fdamage /= 10;
		fdamage++;

		sprite->m_DurabilitySmall += (WORD)fdamage;
		
		if( sprite->m_DurabilitySmall > iMaxDurSmall )
		{
			sprite->m_Durability--;
			sprite->m_DurabilitySmall = 0;

			if( sprite->m_Durability < 1 )	
			{
				sprite->m_Durability = 0.f;
			}
			
			send_dur = TRUE;
			
			//GCItemDurSend(lpObj->m_Index, EQUIPMENT_HELPER, (BYTE)sprite->m_Durability, 0);
		}	
		
		// �������� �˻��ؼ� �渱�� �������ش�.
		//return;
	}
#endif // ADD_ITEM_FENRIR_01_20051110

	
	if( send_dur )
	{
		GCItemDurSend(lpObj->m_Index, EQUIPMENT_HELPER, (BYTE)sprite->m_Durability, 0);
		if( sprite->m_Durability < 1.f )
		{
			int itemtype = sprite->m_Type;

			LogAddTD(lMsg.Get(522), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_HELPER].GetName(), lpObj->pInventory[EQUIPMENT_HELPER].m_Level);

			gObjInventoryDeleteItem(lpObj->m_Index, EQUIPMENT_HELPER);			
			GCInventoryItemDeleteSend(lpObj->m_Index, EQUIPMENT_HELPER, 0);

			gObjMakePreviewCharSet(lpObj->m_Index);
			GCEquipmentChange(lpObj->m_Index, EQUIPMENT_HELPER);
			
#ifdef FOR_BLOODCASTLE
			if (CHECK_BLOODCASTLE(lpObj->MapNumber)
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �ȿ����� ��ź ��ȣ �ڵ�����
				|| CHECK_CHAOSCASTLE(lpObj->MapNumber)
#endif
				) 
			{	// ����ĳ���϶�
				if( itemtype == MAKE_ITEMNUM(13,0) 
					|| itemtype == MAKE_ITEMNUM(13,1) 
#ifdef ADD_SAFEGUARD_ITEM_20080702
					|| itemtype == MAKE_ITEMNUM( 13, 64 )	// ����
					|| itemtype == MAKE_ITEMNUM( 13, 65 )	// ��ȣ����
#endif // ADD_SAFEGUARD_ITEM_20080702
					)
				{	// ��ȣ�� ��ź�϶��� �ٽ� �������� �ش�
					int	founditem = -1;
					for(int  n=MAX_EQUIPMENT; n<MAX_INVENTORY; n++)
					{
						if( lpObj->pInventory[n].IsItem() == TRUE )
						{
							if( lpObj->pInventory[n].m_Type == itemtype )
							{
								founditem = n;
								break;
							}
						}
					}		

					if( founditem != -1 )
					{
						BYTE	sItemInfo[ITEM_BUFFER_SIZE];

						ItemByteConvert(sItemInfo, lpObj->pInventory[founditem]);

						int durSsend, durTsend;
						gObjInventoryMoveItem(lpObj->m_Index, founditem, EQUIPMENT_HELPER, durSsend, durTsend, 0, 0, sItemInfo);

						GCItemMoveResultSend(lpObj->m_Index, 0, EQUIPMENT_HELPER, sItemInfo);						
						GCInventoryItemDeleteSend(lpObj->m_Index, founditem, 0);

						GCServerMsgStringSend(lMsg.Get(1182), lpObj->m_Index, 1);	
						//GCServerMsgStringSend("��� ����Ǿ����ϴ�", lpObj->m_Index, 1);	
					}					
				}
			}
#endif

#ifdef NEW_SKILL_FORSKYLAND
			if( 
				( lpObj->MapNumber == 10 && itemtype == MAKE_ITEMNUM(13,3) )
#ifdef ADD_ITEM_FENRIR_01_20051110
				// õ������ �渱�� ��������, ������ ������ ����ƽ��� �̵�
				|| ( lpObj->MapNumber == 10 && itemtype == MAKE_ITEMNUM(13,37) )
#endif
			  )
			{	
				// õ������ ����Ʈ�� ���� ����, ������ ������ ����ƽ��� �̵�				
				int	founditem = -1;

				for(int  n=MAX_EQUIPMENT; n<MAX_INVENTORY; n++)
				{
					if( lpObj->pInventory[n].IsItem() == TRUE )
					{
						if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,3) )
						{
							founditem = n;
							break;
						}
					}
				}		

				if( founditem == -1 )
				{
					if( !lpObj->pInventory[EQUIPMENT_WING].IsItem() )
					{
						gObjMoveGate(lpObj->m_Index, 22);			
					}
				}
				else
				{

					BYTE	sItemInfo[ITEM_BUFFER_SIZE];

					ItemByteConvert(sItemInfo, lpObj->pInventory[founditem]);

					int durSsend, durTsend;
					gObjInventoryMoveItem(lpObj->m_Index, founditem, EQUIPMENT_HELPER, durSsend, durTsend, 0, 0, sItemInfo);

					GCItemMoveResultSend(lpObj->m_Index, 0, EQUIPMENT_HELPER, sItemInfo);					
					GCInventoryItemDeleteSend(lpObj->m_Index, founditem, 0);

					GCServerMsgStringSend(lMsg.Get(1182), lpObj->m_Index, 1);	// "��� ����Ǿ����ϴ�"
				}
			}
#endif
		}
	}
}

// �ʴ� ���ҵǴ� �����°���
// �ʴ� ���ҵǴ� �����°���
void gObjSecondDurDown(LPOBJECTSTRUCT lpObj)
{	
	lpObj->m_TimeCount++;
	int ret = 0, reCalCharacter = FALSE;

#if ENABLE_MC_SQL == 1
	if (gConnectBonus == 1)
	{
		lpObj->m_ConnectBonusTime++;
		AddTimeBonus(lpObj->m_Index);
	}
#endif // ENABLE_MC_SQL == 1

	// ���� ���� ������ ����
#ifdef MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820
	if( lpObj->m_Change >= 0 ) gObjChangeDurProc2(lpObj);
#else
	if( lpObj->m_Change >= 0 ) gObjChangeDurProc(lpObj);
#endif // MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

	if( (lpObj->m_TimeCount%10) != 0 )
	{
		return;
	}

	if( lpObj->pInventory[EQUIPMENT_WING].IsItem() == TRUE )
	{	// ���� ������ ����

#ifdef DURABILITY_DOWN_CHANGE
		ret = lpObj->pInventory[EQUIPMENT_WING].DurabilityDown(7, lpObj->m_Index);
#else

//#if TESTSERVER == 1
//		ret = lpObj->pInventory[EQUIPMENT_WING].DurabilityDown(100, lpObj->m_Index);
//#else
		ret = lpObj->pInventory[EQUIPMENT_WING].DurabilityDown(1, lpObj->m_Index);
//#endif
#endif
		if( ret )
		{
			GCItemDurSend2(lpObj->m_Index, EQUIPMENT_WING, lpObj->pInventory[EQUIPMENT_WING].m_Durability, 0);			
		}
		if( ret == 2 )
		{	
			reCalCharacter = TRUE;
		}		
		//gObjWingDurProc(lpObj);
	}	
	
	if( !(attr & 0x01) )
	{	// �������밡 �ƴҶ��� ������ �������� ���� �Ѵ�
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
		{
#ifdef ADD_CRITICAL_WIZARD_RING_20090915		// ġ�� ������ ���� - ������ ���� (������)
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13, 107)
				)
			{				
#ifdef PCBANG_ADVANTAGE // apple��20050203
				
#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
				if( IsOKPCBangBenefitAll(lpObj) )
#else
					if (lpObj->m_bPCBangUser)
#endif //PCBANG_ADVANTAGE_EXPAND_20060919
					{
						ret = lpObj->pInventory[EQUIPMENT_RING_RIGHT].DurabilityDown(35, lpObj->m_Index);
					}
					else
#endif
					{
						ret = lpObj->pInventory[EQUIPMENT_RING_RIGHT].DurabilityDown(70, lpObj->m_Index);	// �뷫 1�п� 1�� �⵵�� ��ġ����
					}
					
					if( ret )
					{
						GCItemDurSend2(lpObj->m_Index, EQUIPMENT_RING_RIGHT, lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability, 0);			
					}
					if( ret == 2 )
					{	
						reCalCharacter = TRUE;
					}
					
			} else
				
#endif // ADD_CRITICAL_WIZARD_RING_20090915
#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - ������ ���� (������)
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,20) &&
				lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level == 0	// �������� ����
				)
			{

	#ifdef PCBANG_ADVANTAGE // apple��20050203
				
		#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
				if( IsOKPCBangBenefitAll(lpObj) )
		#else
				if (lpObj->m_bPCBangUser)
		#endif //PCBANG_ADVANTAGE_EXPAND_20060919
				{
					ret = lpObj->pInventory[EQUIPMENT_RING_RIGHT].DurabilityDown(35, lpObj->m_Index);
				}
				else
	#endif
				{
					ret = lpObj->pInventory[EQUIPMENT_RING_RIGHT].DurabilityDown(70, lpObj->m_Index);	// �뷫 1�п� 1�� �⵵�� ��ġ����
				}
				
				if( ret )
				{
					GCItemDurSend2(lpObj->m_Index, EQUIPMENT_RING_RIGHT, lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability, 0);			
				}
				if( ret == 2 )
				{	
					reCalCharacter = TRUE;
				}

			} else

#endif
#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// �߱� �̺�Ʈ - ������ ���� 
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,20) &&
				lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level == 3	// ������ ����
				)
			{
				ret = lpObj->pInventory[EQUIPMENT_RING_RIGHT].DurabilityDown(70, lpObj->m_Index);	// �뷫 1�п� 1�� �⵵�� ��ġ����
				
				if( ret )
				{
					GCItemDurSend2(lpObj->m_Index, EQUIPMENT_RING_RIGHT, lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability, 0);			
				}
				if( ret == 2 )
				{	
					reCalCharacter = TRUE;
				}

			} else

#endif
#ifdef KANTURU_MOONSTONEPENDANT_20060804	// ������ ��´�.
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13 , 38 ) )
			{
				// �뷫 90�ʿ� 1�� �⵵�� ��ġ����
				ret = lpObj->pInventory[EQUIPMENT_RING_RIGHT].DurabilityDown( 63, lpObj->m_Index );	

				if( ret )
				{
					GCItemDurSend2( lpObj->m_Index, EQUIPMENT_RING_RIGHT, lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability, 0 );			
				}
			}
			else
#endif				
#ifdef MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820	// ���� ������ ���� ó���ϰ� �����Ƿ�
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type != MAKE_ITEMNUM(13,10)
				&& lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type != MAKE_ITEMNUM(13, 39)
				&& lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type != MAKE_ITEMNUM(13, 40)
				&& lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type != MAKE_ITEMNUM(13, 41)
				&& lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type != MAKE_ITEMNUM(13, 42)
				)
#else
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type != MAKE_ITEMNUM(13,10) )
#endif // MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820
			{
#ifdef DURABILITY_DOWN_CHANGE
				ret = lpObj->pInventory[EQUIPMENT_RING_RIGHT].DurabilityDown(7, lpObj->m_Index);
#else
//#if TESTSERVER == 1
//				ret = lpObj->pInventory[EQUIPMENT_RING_RIGHT].DurabilityDown(100, lpObj->m_Index);
//#else
				ret = lpObj->pInventory[EQUIPMENT_RING_RIGHT].DurabilityDown(1, lpObj->m_Index);
//#endif
#endif
				if( ret )
				{
					GCItemDurSend2(lpObj->m_Index, EQUIPMENT_RING_RIGHT, lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability, 0);			
				}
				if( ret == 2 )
				{	
					reCalCharacter = TRUE;
				}
			}
		}
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
		{
#ifdef ADD_CRITICAL_WIZARD_RING_20090915		// ġ�� ������ ���� - ������ ���� (�޼�)
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13, 107)
				)
			{
#ifdef PCBANG_ADVANTAGE // apple��20050203
				
#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
				if( IsOKPCBangBenefitAll(lpObj) )
#else
					if (lpObj->m_bPCBangUser)
#endif //PCBANG_ADVANTAGE_EXPAND_20060919
					{
						ret = lpObj->pInventory[EQUIPMENT_RING_LEFT].DurabilityDown(35, lpObj->m_Index);
					}
					else
#endif
					{
						ret = lpObj->pInventory[EQUIPMENT_RING_LEFT].DurabilityDown(70, lpObj->m_Index);	// �뷫 1�п� 1�� �⵵�� ��ġ����
					}
					
					if( ret )
					{
						GCItemDurSend2(lpObj->m_Index, EQUIPMENT_RING_LEFT, lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability, 0);			
					}
					if( ret == 2 )
					{	
						reCalCharacter = TRUE;
					}					
			} else				
#endif // ADD_CRITICAL_WIZARD_RING_20090915
#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - ������ ���� (�޼�)
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,20) &&
				lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level == 0	// �������� ����
				)
			{
		#ifdef PCBANG_ADVANTAGE // apple��20050203
				
			#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
				if( IsOKPCBangBenefitAll(lpObj) )
			#else
				if (lpObj->m_bPCBangUser)
			#endif //PCBANG_ADVANTAGE_EXPAND_20060919
				{
					ret = lpObj->pInventory[EQUIPMENT_RING_LEFT].DurabilityDown(35, lpObj->m_Index);
				}
				else
		#endif
				{
					ret = lpObj->pInventory[EQUIPMENT_RING_LEFT].DurabilityDown(70, lpObj->m_Index);	// �뷫 1�п� 1�� �⵵�� ��ġ����
				}
				
				if( ret )
				{
					GCItemDurSend2(lpObj->m_Index, EQUIPMENT_RING_LEFT, lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability, 0);			
				}
				if( ret == 2 )
				{	
					reCalCharacter = TRUE;
				}

			} else

#endif
#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// �߱� �̺�Ʈ - ������ ���� 
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,20) &&
				lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level == 3	// ������ ����
				)
			{
				ret = lpObj->pInventory[EQUIPMENT_RING_LEFT].DurabilityDown(70, lpObj->m_Index);	// �뷫 1�п� 1�� �⵵�� ��ġ����
				
				if( ret )
				{
					GCItemDurSend2(lpObj->m_Index, EQUIPMENT_RING_LEFT, lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability, 0);			
				}
				if( ret == 2 )
				{	
					reCalCharacter = TRUE;
				}

			} else
#endif
#ifdef KANTURU_MOONSTONEPENDANT_20060804	// ������ ��´�.
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13 , 38 ) )
			{
				// �뷫 90�ʿ� 1�� �⵵�� ��ġ����
				ret = lpObj->pInventory[EQUIPMENT_RING_LEFT].DurabilityDown( 63, lpObj->m_Index );	
				
				if( ret )
				{
					GCItemDurSend2( lpObj->m_Index, EQUIPMENT_RING_LEFT, lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability, 0 );			
				}
			}
			else
#endif
#ifdef MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820	// ���� ������ ���� ó���ϰ� �����Ƿ�
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type != MAKE_ITEMNUM(13,10)
				&& lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type != MAKE_ITEMNUM(13, 39)
				&& lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type != MAKE_ITEMNUM(13, 40)
				&& lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type != MAKE_ITEMNUM(13, 41)
				&& lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type != MAKE_ITEMNUM(13, 42)
				)
#else
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type != MAKE_ITEMNUM(13,10) )
#endif // MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820
			{
#ifdef DURABILITY_DOWN_CHANGE
				ret = lpObj->pInventory[EQUIPMENT_RING_LEFT].DurabilityDown(7, lpObj->m_Index);
#else
//#if TESTSERVER == 1
//				ret = lpObj->pInventory[EQUIPMENT_RING_LEFT].DurabilityDown(100, lpObj->m_Index);
//#else
				ret = lpObj->pInventory[EQUIPMENT_RING_LEFT].DurabilityDown(1, lpObj->m_Index);
//#endif
#endif
				if( ret )
				{
					GCItemDurSend2(lpObj->m_Index, EQUIPMENT_RING_LEFT, lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability, 0);			
				}
				if( ret == 2 )
				{	
					reCalCharacter = TRUE;
				}
			}
		}

		if( lpObj->pInventory[EQUIPMENT_AMULET].IsItem() == TRUE )
		{	// �����
			if( lpObj->pInventory[EQUIPMENT_AMULET].m_Type != MAKE_ITEMNUM(13,10) )
			{
#ifdef DURABILITY_DOWN_CHANGE
				ret = lpObj->pInventory[EQUIPMENT_AMULET].DurabilityDown(7, lpObj->m_Index);
#else
//#if TESTSERVER == 1
//				ret = lpObj->pInventory[EQUIPMENT_AMULET].DurabilityDown(100, lpObj->m_Index);
//#else
				ret = lpObj->pInventory[EQUIPMENT_AMULET].DurabilityDown(1, lpObj->m_Index);
//#endif
#endif
				if( ret )
				{
					GCItemDurSend2(lpObj->m_Index, EQUIPMENT_AMULET, lpObj->pInventory[EQUIPMENT_AMULET].m_Durability, 0);			
				}
				if( ret == 2 )
				{	
					reCalCharacter = TRUE;
				}
			}
		}

	}

#ifdef DURABILITY_DOWN_CHANGE	// ������ ���� ��� ����(������������ �ƴ�, �ʴ�)
	if( !(attr & 0x01) )
	{
		// ����� ������ ����
		int irand = rand()%2;
		CItem	*Weapon = &lpObj->pInventory[irand];
		ret = 0;
		int decdur = 50;	// 1�п� 0.3��

		if( Weapon->m_Type != MAKE_ITEMNUM(4,7) && Weapon->m_Type != MAKE_ITEMNUM(4,15) )
		{	//; Ȱ �� �ƴϸ�		
			if( (Weapon->m_Type < ITEM_SHIELD && Weapon->m_Type >= ITEM_HELM) )
			{	// ���з��� �ƴϸ�
				if( Weapon->IsItem() )
				{
					ret = Weapon->DurabilityDown2(decdur, lpObj->m_Index);
				}
			}
			else
			{			
				CItem	*Weapon = &lpObj->pInventory[irand];
				if( Weapon->IsItem() )
				{
					ret = Weapon->DurabilityDown2(decdur, lpObj->m_Index);
				}
			}		
		}
		
		decdur = 50;	// 1�п� 0.2��
		if( ret )
		{
			GCItemDurSend2(lpObj->m_Index, irand, lpObj->pInventory[irand].m_Durability, 0);			
		}
		if( ret == 2 )
		{	
			reCalCharacter = TRUE;
		}

		// ���� ������ ����
		int item_num[7] = {EQUIPMENT_WEAPON_RIGHT, EQUIPMENT_WEAPON_LEFT, EQUIPMENT_HELM, EQUIPMENT_ARMOR,EQUIPMENT_PANTS, EQUIPMENT_GLOVES, EQUIPMENT_BOOTS};
		int item_pos = rand()%7;

		CItem	*DurItem = &lpObj->pInventory[item_num[item_pos]];

		// ����� ��쿣 ���и� ������ �޴´�.
		if( item_pos < 2 )
		{
			if( (DurItem->m_Type >= ITEM_SHIELD && DurItem->m_Type < ITEM_HELM) )
			{	// �����϶���
				ret = DurItem->DurabilityDown2(decdur, lpObj->m_Index);
				if( ret )
				{	// �������� ��ȭ�Ǹ� Ŭ���̾�Ʈ�� �˸���
					GCItemDurSend2(lpObj->m_Index, item_num[item_pos], DurItem->m_Durability, 0);
					if( ret == 2 )
					{	// ĳ���� ��ġ�� �ٽ� ����Ѵ�
						reCalCharacter = TRUE;
					}
				}				
			}		
		}
		else
		{
			ret = DurItem->DurabilityDown2(decdur, lpObj->m_Index);
			if( ret )
			{	// �������� ��ȭ�Ǹ� Ŭ���̾�Ʈ�� �˸���
				GCItemDurSend2(lpObj->m_Index, item_num[item_pos], DurItem->m_Durability, 0);
				if( ret == 2 )
				{	// ĳ���� ��ġ�� �ٽ� ����Ѵ�
					reCalCharacter = TRUE;
				}
			}
		}

		if( reCalCharacter )
		{	// ������ ���� �г�Ƽ�� ����Ǹ� �ٽ� ĳ���� ��ġ������ �ٽ� ����Ѵ�
			gObjCalCharacter(lpObj->m_Index);
		}
	}
#endif


	if( reCalCharacter )
	{	// ������ ���� �г�Ƽ�� ����Ǹ� �ٽ� ĳ���� ��ġ������ �ٽ� ����Ѵ�
		gObjCalCharacter(lpObj->m_Index);
	}
}


// ���Ź����� �����ϰ� �ִٸ� �������� ���ҽ�Ų��.
void gObjChangeDurProc(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) )
		{
			int dur = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			
			if( dur > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability ) 
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 ) 
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;

				BYTE send_dur = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_RING_RIGHT, send_dur, 0);
				if( send_dur == 0 ) 
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, send_dur);
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0);					
				}
			}
			return;
		}
	}
	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) )
		{
			int dur = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.02;
			if( dur > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability ) 
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;

				BYTE send_dur = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)send_dur, 0);
				if( send_dur == 0 ) 
				{	
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, send_dur);
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, EQUIPMENT_RING_LEFT, 0);					
				}
			}
			return;
		}
	}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830		// ���� ����
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{
			int dur = ( int )lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			
			if( dur > ( int )lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability ) 
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 ) 
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;

				BYTE send_dur = ( BYTE )lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, send_dur, 0 );
				if( send_dur == 0 ) 
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, send_dur );
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
				}
			}
			return;
		}
	}
	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{
			int dur = ( int )lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= ( float )0.02;
			if( dur > ( int )lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability ) 
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;

				BYTE send_dur = ( BYTE )lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)send_dur, 0 );
				if( send_dur == 0 ) 
				{	
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD(lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, send_dur );
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );					
				}
			}
			return;
		}
	}
#endif	// ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	
	
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
				}

				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
				}
			}

			return;
		}
	}

	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
				}

				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_LEFT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );
				}
			}

			return;
		}
	}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
	
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
				}
			}
			
			return;
		}
	}
	
	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_LEFT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );
				}
			}
			
			return;
		}
	}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117	

#ifdef UPDATE_GM_FUNCTION_20070228
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 42 ) )
		{
			return;
		}
	}

	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 42 ) )
		{
			return;
		}
	}
#endif

//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
				}
			}
			
			return;
		}
	}
	
	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_LEFT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );
				}
			}
			
			return;
		}
	}
#endif //ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------

#ifdef ADD_PREMIUMITEM_PANDA
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE && lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
	{
		int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.01;
		if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
			{
				lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
			}
			
			BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, (BYTE)btSendDuration, 0 );
			if( btSendDuration == 0 )
			{
				gObjUseSkill.SkillChangeUse( lpObj->m_Index );
				LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, btSendDuration );
				lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
				GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
			}
		}
			
		return;
	}
	
	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE && lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
	{
		int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.01;
		if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
			{
				lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
			}
			
			BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)btSendDuration, 0 );
			if( btSendDuration == 0 )
			{
				gObjUseSkill.SkillChangeUse( lpObj->m_Index );
				LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_LEFT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, btSendDuration );
				lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
				GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );
			}
		}
		
		return;
	}
#endif // ADD_PREMIUMITEM_PANDA
}


// ���Ź����� �����ϰ� �ִٸ� �������� ���ҽ�Ų��.
// ������ ����ϴ� gObjChangeDurProc()�� ���ʿ� ���� ���� �������� ��� ���װ� ����
// MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820 ������ ���Ŀ��� �Ʒ� �Լ��� ��ü
#ifdef MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820
void gObjChangeDurProc2(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) )
		{
			int dur = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			
			if( dur > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability ) 
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 ) 
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
				
				BYTE send_dur = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_RING_RIGHT, send_dur, 0);
				if( send_dur == 0 ) 
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, send_dur);
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0);					
				}
			}
		}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830		// ���� ����
		else if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{
			int dur = ( int )lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			
			if( dur > ( int )lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability ) 
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 ) 
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
				
				BYTE send_dur = ( BYTE )lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, send_dur, 0 );
				if( send_dur == 0 ) 
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, send_dur );
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
				}
			}
		}
#endif // ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		else if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
				}
			}
		}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		else if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
				}
			}
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
		else if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
				}
			}
		}
#endif //ADD_XMASEVENT_POLYMORPH_RING_20080930
//<------------
#ifdef ADD_PREMIUMITEM_PANDA
		else if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability -= (float)0.01;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_RIGHT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_RIGHT, 0 );
				}
			}
		}
#endif // ADD_PREMIUMITEM_PANDA
#ifdef UPDATE_GM_FUNCTION_20070228
	else if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 42 ) )
		{	// GM ���� ������ �������� ���� �ʴ´�.
		}
#endif // UPDATE_GM_FUNCTION_20070228
	}


	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) )
		{
			int dur = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.02;
			if( dur > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability ) 
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
				
				BYTE send_dur = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)send_dur, 0);
				if( send_dur == 0 ) 
				{	
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_RIGHT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, send_dur);
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, EQUIPMENT_RING_LEFT, 0);					
				}
			}
		}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830		// ���� ����
		else if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{
			int dur = ( int )lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.02;
			
			if( dur > ( int )lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability ) 
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 ) 
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
				
				BYTE send_dur = ( BYTE )lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, send_dur, 0 );
				if( send_dur == 0 ) 
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_LEFT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, send_dur );
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );
				}
			}
		}
#endif // ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		else if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_LEFT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );
				}
			}
		}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		else if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_LEFT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );
				}
			}
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
		else if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.02;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_LEFT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );
				}
			}
		}
#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------
#ifdef ADD_PREMIUMITEM_PANDA
		else if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
		{
			int iDuration = (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability -= (float)0.01;
			if( iDuration > (int)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability = 0;
				}
				
				BYTE btSendDuration = (BYTE)lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability;
				GCItemDurSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, (BYTE)btSendDuration, 0 );
				if( btSendDuration == 0 )
				{
					gObjUseSkill.SkillChangeUse( lpObj->m_Index );
					LogAddTD( lMsg.Get( 544 ), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_RING_LEFT].GetName(), lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level, btSendDuration );
					lpObj->pInventory[EQUIPMENT_RING_LEFT].Clear();
					GCInventoryItemDeleteSend( lpObj->m_Index, EQUIPMENT_RING_LEFT, 0 );
				}
			}
		}
#endif // ADD_PREMIUMITEM_PANDA

#ifdef UPDATE_GM_FUNCTION_20070228
		else if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 42 ) )
		{	// GM ���� ������ �������� ���� �ʴ´�.
		}
#endif // UPDATE_GM_FUNCTION_20070228
	}
}
#endif // MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820


// ������ �����ϰ� �ִٸ� �������� ���ҽ�Ų��.
void gObjWingDurProc(LPOBJECTSTRUCT lpObj)
{
	BYTE send_dur=0;

	// �̹� ���������� ������ �� ���..
	return;

	if( lpObj->pInventory[EQUIPMENT_WING].IsItem() == TRUE )
	{	
		int dur = (int)lpObj->pInventory[EQUIPMENT_WING].m_Durability;
			lpObj->pInventory[EQUIPMENT_WING].m_Durability -= (float)0.0002;
			
			if( dur > (int)lpObj->pInventory[EQUIPMENT_WING].m_Durability ) 
			{
				if( lpObj->pInventory[EQUIPMENT_WING].m_Durability < 1 ) 
					lpObj->pInventory[EQUIPMENT_WING].m_Durability = 0;

				send_dur = (BYTE)lpObj->pInventory[EQUIPMENT_WING].m_Durability;
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_WING, send_dur, 0);
				if( send_dur == 0 ) 
				{
					LogAddTD(lMsg.Get(544), lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_WING].GetName(), lpObj->pInventory[EQUIPMENT_WING].m_Level, send_dur);
					lpObj->pInventory[EQUIPMENT_WING].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, EQUIPMENT_WING, 0);
				}
			}
			//LogAdd("���� ������ %f",lpObj->pInventory[EQUIPMENT_WING].m_Durability);
			return;
	}
}

// �г�Ƽ�������� ���� ������ ����
void gObjPenaltyDurDown(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	int ret = 0, reCalCharacter = FALSE;
	// ����� ������ ����
	int irand = rand()%2;
	CItem	*Weapon = &lpObj->pInventory[irand];
	int decdur = 1;

	switch(lpObj->Class)
	{
		case CLASS_WIZARD: 
			{
				decdur = (lpObj->Level - (lpTargetObj->Level*38/20));
			}
		break;

		case CLASS_KNIGHT: 
			{
				decdur = (lpObj->Level - (lpTargetObj->Level*37/20));
			}
		break;

		case CLASS_ELF: 
			{
				decdur = (lpObj->Level - (lpTargetObj->Level*36/20));
			}
		break;

#ifdef DARKLORD_WORK
		case CLASS_DARKLORD: 		
#endif
		case CLASS_MAGUMSA: 
			{
				decdur = (lpObj->Level - (lpTargetObj->Level*37/20));
			}
		break;

#ifdef MODIFY_SUMMONER_ITEM_DURABILITY_DOWN_BUGFIX_20071213
		case CLASS_SUMMONER: 
			{
				decdur = (lpObj->Level - (lpTargetObj->Level*38/20));
			}
		break;
#endif
	}

	decdur /= 10;
	

	if( Weapon->m_Type != MAKE_ITEMNUM(4,7) && Weapon->m_Type != MAKE_ITEMNUM(4,15) )
	{	//; Ȱ �� �ƴϸ�
		if( (Weapon->m_Type < ITEM_SHIELD && Weapon->m_Type >= ITEM_HELM) )
		{	// ���з��� �ƴϸ�
			if( Weapon->IsItem() )
			{
				ret = Weapon->DurabilityDown2(decdur, lpObj->m_Index);
			}
		}
		else
		{			
			CItem	*Weapon = &lpObj->pInventory[irand];
			if( Weapon->IsItem() )
			{
				ret = Weapon->DurabilityDown2(decdur, lpObj->m_Index);
			}
		}
	}
	
	if( ret )
	{
		GCItemDurSend2(lpObj->m_Index, irand, lpObj->pInventory[irand].m_Durability, 0);			
	}
	if( ret == 2 )
	{	
		reCalCharacter = TRUE;
	}

	// ���� ������ ����
	int item_num[7] = {EQUIPMENT_WEAPON_RIGHT, EQUIPMENT_WEAPON_LEFT, EQUIPMENT_HELM, EQUIPMENT_ARMOR,EQUIPMENT_PANTS, EQUIPMENT_GLOVES, EQUIPMENT_BOOTS};
	int item_pos = rand()%7;

	CItem	*DurItem = &lpObj->pInventory[item_num[item_pos]];

	if( DurItem->m_Type != MAKE_ITEMNUM(4,7) && DurItem->m_Type != MAKE_ITEMNUM(4,15) )
	{
		// ����� ��쿣 ���и� ������ �޴´�.
		if( item_pos < 2 )
		{
			if( (DurItem->m_Type >= ITEM_SHIELD && DurItem->m_Type < ITEM_HELM) )
			{	// �����϶���
				ret = DurItem->DurabilityDown2(decdur, lpObj->m_Index);
				if( ret )
				{	// �������� ��ȭ�Ǹ� Ŭ���̾�Ʈ�� �˸���
					GCItemDurSend2(lpObj->m_Index, item_num[item_pos], DurItem->m_Durability, 0);
					if( ret == 2 )
					{	// ĳ���� ��ġ�� �ٽ� ����Ѵ�
						reCalCharacter = TRUE;
					}
				}				
			}		
		}
		else
		{
			ret = DurItem->DurabilityDown2(decdur, lpObj->m_Index);
			if( ret )
			{	// �������� ��ȭ�Ǹ� Ŭ���̾�Ʈ�� �˸���
				GCItemDurSend2(lpObj->m_Index, item_num[item_pos], DurItem->m_Durability, 0);
				if( ret == 2 )
				{	// ĳ���� ��ġ�� �ٽ� ����Ѵ�
					reCalCharacter = TRUE;
				}
			}
		}
	}

	// ���� �߰� ������ ����
	CItem	*Wing = &lpObj->pInventory[EQUIPMENT_WING];	
	if( Wing->IsItem() )
	{
		ret = DurItem->DurabilityDown2(decdur, lpObj->m_Index);
		if( ret )
		{	// �������� ��ȭ�Ǹ� Ŭ���̾�Ʈ�� �˸���
			GCItemDurSend2(lpObj->m_Index, item_num[item_pos], DurItem->m_Durability, 0);
			if( ret == 2 )
			{	// ĳ���� ��ġ�� �ٽ� ����Ѵ�
				reCalCharacter = TRUE;
			}
		}				
	}	

	if( reCalCharacter )
	{	// ������ ���� �г�Ƽ�� ����Ǹ� �ٽ� ĳ���� ��ġ������ �ٽ� ����Ѵ�
		gObjCalCharacter(lpObj->m_Index);
	}
}


void gObjWeaponDurDown(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int type)
{
#ifdef DURABILITY_DOWN_CHANGE	// ������ ���� ��� ����(������������ �ƴ�, �ʴ�)
	return;
#endif

	if( lpObj->Type == OBJTYPE_MONSTER || lpObj->Type == OBJTYPE_NPC)
	{	// ���Ͱ� �����ϴ� ���� ������ ���� �ش���׾���
		return;
	}

	int	itargetdefence = 0;
	
	CItem	*Right = &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT];
	CItem	*Left  = &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT];			

	int ret = 0;

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CHARM_PROTECTITEM) ){
		return ;
	}
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129

	if( type == 0 )
	{	// ���� ����
		itargetdefence = lpTargetObj->m_Defense;

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004			// !! NOT
	#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
		itargetdefence += lpTargetObj->m_iSkillNPCDefense;
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

		//�μտ� ���⸦ �������	
#ifdef DARKLORD_WORK
		if(lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD )
#else
		if(lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA )
#endif
		{
			if( Right->m_Type >= 0 && Right->m_Type < ITEM_BOW && Left->m_Type >= 0 && Left->m_Type < ITEM_BOW )
			{
				if( rand()%2 == 0 )
				{	// �޼չ��� ������ ����
					ret = Left->NormalWeaponDurabilityDown(itargetdefence, lpObj->m_Index);
					if( ret )
					{
						GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, Left->m_Durability, 0);
						if( ret == 2 )
						{
							gObjCalCharacter(lpObj->m_Index);
						}
					}
				}
				else
				{	// �����չ��� ������ ����
					ret = Right->NormalWeaponDurabilityDown(itargetdefence, lpObj->m_Index);					
					if( ret )
					{
						GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, Right->m_Durability, 0);
						if( ret == 2 )
						{
							gObjCalCharacter(lpObj->m_Index);
						}
					}
				}
				return;
			}
		}

		//Ȱ�϶�
		if( (Left->m_Type >= ITEM_BOW && Left->m_Type < ITEM_BOW+7) 
#ifdef MODIFY_APPLYING_BOW_01_20050825
			|| Left->m_Type == ITEM_BOW + 20
			|| Left->m_Type == ITEM_BOW + 21
#endif
#ifdef MODIFY_ALBATROS_BOW_DURABILITY_BUGFIX_20080813	// �˹�Ʈ�ν����� ó�� �߰�
			|| Left->m_Type == ITEM_BOW + 22
#endif // MODIFY_ALBATROS_BOW_DURABILITY_BUGFIX_20080813
#ifdef MODIFY_DARKSTINGER_BUGFIX_20081119
			|| Left->m_Type == ITEM_BOW + 23			// ��ũ���ð� ����
#endif // MODIFY_DARKSTINGER_BUGFIX_20081119
#ifdef ADD_AERINBOW_20090309
			|| Left->m_Type == ITEM_BOW + 24			// ��� ����
#endif // ADD_AERINBOW_20090309
		  )
		{
			ret = Left->BowWeaponDurabilityDown(itargetdefence, lpObj->m_Index);
			if( ret )
			{
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, Left->m_Durability, 0);
				if( ret == 2 )
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		//�����϶�
		else if(( Right->m_Type >= ITEM_BOW+8 && Right->m_Type < ITEM_BOW+15) ||
			  ( Right->m_Type >= ITEM_BOW+16 && Right->m_Type < ITEM_BOW+MAX_ITEM_INDEX))
		{		
			
			ret = Right->BowWeaponDurabilityDown(itargetdefence, lpObj->m_Index);			
			if( ret )
			{
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, Right->m_Durability, 0);
				if( ret == 2 )
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		//�Ϲݹ���
		else if( Right->m_Type >= 0 && Right->m_Type < ITEM_BOW )
		{
			ret = Right->NormalWeaponDurabilityDown(itargetdefence, lpObj->m_Index);
			if( ret )
			{
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, Right->m_Durability, 0);
				if( ret == 2 )
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		//������
		else if( Right->m_Type >= ITEM_STAFF && Right->m_Type < ITEM_SHIELD )
		{	
			ret = Right->StaffWeaponDurabilityDown(itargetdefence, lpObj->m_Index);

			if( ret )
			{
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, Right->m_Durability, 0);
				if( ret == 2 )
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
	}
	else
	{	// ���� ����
		if( lpTargetObj->Type == OBJTYPE_MONSTER || lpTargetObj->Type == OBJTYPE_NPC)
		{	// ����� ������ ��쿡�� ���������� ����
			itargetdefence = lpTargetObj->m_MagicDefense;
		}
		else
		{	// ����� ����� ���� �Ϲ� ������ ����
			itargetdefence = lpTargetObj->m_Defense;
		}

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004			// !! NOT
	#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
		itargetdefence += lpTargetObj->m_iSkillNPCDefense;
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef MODIFY_SUMMONER_ITEM_DURABILITY_DOWN_BUGFIX_20071213	// ��ȯ����
		if( lpObj->Class == CLASS_SUMMONER )
		{
			if( Right->m_Type >= 0 && Right->m_Type >= ITEM_STAFF && Right->m_Type < ITEM_SHIELD
				&& Left->m_Type >= 0 && Left->m_Type >= ITEM_STAFF && Left->m_Type < ITEM_SHIELD )
			{
				// �Ѽտ� ��ƽ, �Ѽտ� ��ȯ��å�ϰ��
				if( rand()%2 == 0 )
				{
					ret = Right->StaffWeaponDurabilityDown(itargetdefence, lpObj->m_Index);
					
					if( ret )
					{
						GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, Right->m_Durability, 0);
						if( ret == 2 )
						{
							gObjCalCharacter(lpObj->m_Index);
						}
					}
				}
				else
				{
					ret = Left->StaffWeaponDurabilityDown(itargetdefence, lpObj->m_Index);
					
					if( ret )
					{
						GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, Left->m_Durability, 0);
						if( ret == 2 )
						{
							gObjCalCharacter(lpObj->m_Index);
						}
					}
				}
				return;
			}
			else if( Right->m_Type >= 0 && Right->m_Type >= ITEM_STAFF && Right->m_Type < ITEM_SHIELD)
			{
				// �����տ� ��ƽ
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, Right->m_Durability, 0);
				if( ret == 2 )
				{
					gObjCalCharacter(lpObj->m_Index);
				}
				return;
			}
			else if( Left->m_Type >= 0 && Left->m_Type >= ITEM_STAFF && Left->m_Type < ITEM_SHIELD)
			{
				// �޼տ� ��ȯ�� å
				ret = Left->StaffWeaponDurabilityDown(itargetdefence, lpObj->m_Index);
				
				if( ret )
				{
					GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, Left->m_Durability, 0);
					if( ret == 2 )
					{
						gObjCalCharacter(lpObj->m_Index);
					}
				}
				return;
			}
		}

#endif	// MODIFY_SUMMONER_ITEM_DURABILITY_DOWN_BUGFIX_20071213
		
		//������
		if( Right->m_Type >= ITEM_STAFF && Right->m_Type < ITEM_SHIELD )
		{	
			ret = Right->StaffWeaponDurabilityDown(itargetdefence, lpObj->m_Index);

			if( ret )
			{
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, Right->m_Durability, 0);
				if( ret == 2 )
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
	}
}

// ������ �޾� ���� �ϳ��� ������ �ٿ� ��Ų��.
void gObjArmorRandomDurDown(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpAttackObj)
{
#ifdef DURABILITY_DOWN_CHANGE	// ������ ���� ��� ����(������������ �ƴ�, �ʴ�)
	return;
#endif
	int item_num[7] = {EQUIPMENT_WEAPON_RIGHT, EQUIPMENT_WEAPON_LEFT, EQUIPMENT_HELM, EQUIPMENT_ARMOR,EQUIPMENT_PANTS, EQUIPMENT_GLOVES, EQUIPMENT_BOOTS};
	int item_pos = rand()%7;

	if( lpObj->Type != OBJTYPE_CHARACTER ) return;

	CItem	*DurItem = &lpObj->pInventory[item_num[item_pos]];

	int damagemin = lpAttackObj->m_AttackDamageMin;
	int ret;

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	
	short nType = DurItem->m_Type;
	
	if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CHARM_PROTECTITEM) ) {
		// ������ ��ȣ ���� ���� ������.. ������ ���� ����
		
		if(nType >= ITEM_SWORD && nType < ITEM_WING) {
			// ���� & �� ��
			return;
		} else if(nType >= ITEM_WING && nType < ITEM_HELPER) {
			// ���� 
			if( (MAKE_ITEMNUM_INDEX(nType) >= 0 && MAKE_ITEMNUM_INDEX(nType) <= 16) 
				|| (MAKE_ITEMNUM_INDEX(nType) >= 36 && MAKE_ITEMNUM_INDEX(nType) <= 43) ) {
				// �ش� INDEX�� ��ȣ..
				return;
			} else {
				// ��ȣ�� �ȵǴ� �������̴� �Ʒ� ���� Ÿ��
			}
		} else {
			// ��ȣ�� �ȵǴ� �������̴� �Ʒ� ���� Ÿ��
		}
	}
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129

	// ����� ��쿣 ���и� ������ �޴´�.
	if( item_pos < 2 )
	{
		if( (DurItem->m_Type >= ITEM_SHIELD && DurItem->m_Type < ITEM_HELM) )
		{
			// ȭ���̰ų� ����ȭ���� ��쿣 ������ ���� �Ƚ�Ų��.			
			ret = DurItem->ArmorDurabilityDown(damagemin, lpObj->m_Index);
			if( ret )
			{	// �������� ��ȭ�Ǹ� Ŭ���̾�Ʈ�� �˸���
				GCItemDurSend(lpObj->m_Index, item_num[item_pos], DurItem->m_Durability, 0);
				if( ret == 2 )
				{	// ĳ���� ��ġ�� �ٽ� ����Ѵ�
					gObjCalCharacter(lpObj->m_Index);
				}
			}
			return;
		}		
	}
	else
	{
		if( DurItem->m_Type != MAKE_ITEMNUM(4,7) && DurItem->m_Type != MAKE_ITEMNUM(4,15) )
		{
			ret = DurItem->ArmorDurabilityDown(damagemin, lpObj->m_Index);
			if( ret )
			{	// �������� ��ȭ�Ǹ� Ŭ���̾�Ʈ�� �˸���
				GCItemDurSend(lpObj->m_Index, item_num[item_pos], DurItem->m_Durability, 0);
				if( ret == 2 )
				{	// ĳ���� ��ġ�� �ٽ� ����Ѵ�
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}	
	}
}


// ���� ������� �������� ������� �˻��Ѵ�.
// lpObj : ������
// aTargetIndex : ������
bool gObjIsSelfDefense (LPOBJECTSTRUCT lpObj, int aTargetIndex)	
{
	if(!CHECK_LIMIT( aTargetIndex, MAX_OBJECT ) )
		return false;

	for( int n=0; n<MAXSELFDEFENSE; n++)
	{
		// Ȥ�� �����ڰ� ������� ����ڶ��.. �׳� ����
		if( lpObj->SelfDefense[n] >= 0)
		{
			if( lpObj->SelfDefense[n] == aTargetIndex ) return true;		
		}
	}

	return false;
}


// ������� �˻� �Ͽ� ��������� �����Ѵ�.
// lpObj : ������
// aTargetIndex : ������
void gObjCheckSelfDefense(LPOBJECTSTRUCT lpObj, int aTargetIndex)
{
	if(!CHECK_LIMIT( aTargetIndex, MAX_OBJECT ) )
	{
		return;
	}
#ifdef ADD_NEWPVP_PKFIELD
	// �����߿� ������� �����ȵ�
	if (g_NewPVP.IsDuel(*lpObj, aTargetIndex) == TRUE)
		return;
#endif // ADD_NEWPVP_PKFIELD

	// �����ڴ� ��������� ���Ҽ� ����.
	if( gObj[aTargetIndex].m_PK_Level > PK_LEVEL_DEFAULT+1 )
	{
		return;
	}	
	int n;

#ifdef MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022	// �����ڰ� ��Ƽ�� ���ϰ� ���� ��Ƽ���߿� �Ѹ��� ������� ����ڶ�� �׳ɸ���
	int partynum			= lpObj->PartyNumber;	// �������� ��Ƽ��ȣ
	int	nSelfDefenseUser	= -1;					// �����ڿ� ���漳���� ����

	for( n=0; n<MAXSELFDEFENSE; n++)
	{
		// Ȥ�� �����ڰ� ������� ����ڶ��.. �׳� ����
		if( gObj[aTargetIndex].SelfDefense[n] >= 0)
		{
			if( gObj[aTargetIndex].SelfDefense[n] == lpObj->m_Index ) return;		

			// �����ڿ� ���漳���� �����߿� �����ڿ� ���� ��Ƽ�� ������ ����
			if( partynum >= 0 
				&& gObj[gObj[aTargetIndex].SelfDefense[n]].PartyNumber == partynum )
			{
				return;
			}	
		}
	}

#else	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022

	for( n=0; n<MAXSELFDEFENSE; n++)
	{
		// Ȥ�� �����ڰ� ������� ����ڶ��.. �׳� ����
		if( gObj[aTargetIndex].SelfDefense[n] >= 0)
		{
			if( gObj[aTargetIndex].SelfDefense[n] == lpObj->m_Index ) return;		
		}

	}
#endif	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022

	int blank=-1;

	// ��������� �����Ǿ� �ִµ� �ٽ� �����ߴٸ�.. �޽����� �ٽ� ������� �ʴ´�.
	for( n=0; n<MAXSELFDEFENSE; n++)
	{
		if( lpObj->SelfDefense[n] >= 0 )
		{
			if( lpObj->SelfDefense[n] == aTargetIndex )
			{
#ifdef MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221	// ������� ���ӽð� 60��
				lpObj->SelfDefenseTime[n]	= GetTickCount()+(1000*60);
#else	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221					
#ifdef UPDATE_PK_SYSTEM_20070302	// ������� ���ӽð� ����
				lpObj->SelfDefenseTime[n]	= GetTickCount()+(1000*90);
#else
				lpObj->SelfDefenseTime[n]	= GetTickCount()+(1000*30);
#endif
#endif	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221
				return;
			}
		}
		else blank = n;
	}
	if( blank < 0 ) {
		LogAdd(lMsg.Get(523));
		return;
	}
	lpObj->MySelfDefenseTime = GetTickCount();

	//
	lpObj->SelfDefense[blank]	  = aTargetIndex;
#ifdef MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221	// ������� ���ӽð� 60��
	lpObj->SelfDefenseTime[blank] = GetTickCount()+(1000*60);
#else	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221	
#ifdef UPDATE_PK_SYSTEM_20070302	// ������� ���ӽð� ����
	lpObj->SelfDefenseTime[blank] = GetTickCount()+(1000*90);
#else
	lpObj->SelfDefenseTime[blank] = GetTickCount()+(1000*30);
#endif
#endif	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221

	char szTemp[MAX_CHAT+1];

#ifdef MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022	// ������ ���� ��Ƽ�������׵� ������ �ɷȴٴ� �޽����� ������.
	wsprintf(szTemp, lMsg.Get(1114), lpObj->Name, gObj[aTargetIndex].Name);
	partynum = gObj[aTargetIndex].PartyNumber;
	if(partynum >= 0 )
	{
		int nPartyUser = -1;
		for( n = 0; n < MAX_PARTYUSER; n++)
		{
			nPartyUser = gParty.m_PartyS[partynum].Number[n];
			if ( nPartyUser >= 0)
			{
				GCServerMsgStringSend(szTemp, nPartyUser, 1);
			}
		}
	}
	else
	{
		GCServerMsgStringSend(szTemp, aTargetIndex, 1);
	}
	
	GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);

#else	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022

	wsprintf(szTemp, lMsg.Get(1114), lpObj->Name, gObj[aTargetIndex].Name);
	GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
	GCServerMsgStringSend(szTemp, aTargetIndex, 1);
#endif	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022

	LogAddTD("[%s][%s] Set SelfDefence [%s][%s]", 
			 lpObj->AccountID, lpObj->Name, 
			 gObj[aTargetIndex].AccountID, gObj[aTargetIndex].Name);
}

void gObjTimeCheckSelfDefense(LPOBJECTSTRUCT lpObj)
{
	char szTemp[MAX_CHAT+1];

	for( int n=0; n<MAXSELFDEFENSE; n++)
	{
		if( lpObj->SelfDefense[n] >= 0 )
		{
			if( GetTickCount() > lpObj->SelfDefenseTime[n] )
			{
				wsprintf(szTemp, lMsg.Get(1115), lpObj->Name);
				GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				GCServerMsgStringSend(szTemp, lpObj->SelfDefense[n], 1);

				LogAddTD("[%s][%s] ReSet SelfDefence [%s][%s]", 
						 lpObj->AccountID, lpObj->Name, 
						 gObj[lpObj->SelfDefense[n]].AccountID, gObj[lpObj->SelfDefense[n]].Name);

				lpObj->SelfDefense[n] = -1;
			}
		}
	}
}



int skillSuccess=0;
//----------------------------------------------------------------------------
// � ����ڰ� Ư�� ����ڸ� ���� �ߴ�..
BOOL gObjAttack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int magicsend, BYTE MSBFlag, int AttackDamage,BOOL bCombo)
{
	return gclassObjAttack.Attack(lpObj, lpTargetObj, lpMagic, magicsend, MSBFlag, AttackDamage, bCombo);
}

#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
int gObjMonsterExpSingle(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int dmg, int tot_dmg, bool& bSendExp);
#else
int gObjMonsterExpSingle(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int dmg, int tot_dmg);
#endif

void gObjMonsterExpDivision(LPOBJECTSTRUCT lpMonObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int MSBFlag)
{
	int tObjNum;
#ifdef BUGFIX_GAIN_EXP_20071210
	INT64 exp;
#else
	int exp;
#endif
	LPOBJECTSTRUCT lpTargetObj;
	
	gObjMonsterHitDamageUserDel(lpMonObj);

	lpMonObj->Money = 0;
	for( int n=0; n<MAX_HITDAMAGE; n++)
	{
		if( lpMonObj->sHD[n].number >= 0 )
		{
			tObjNum = lpMonObj->sHD[n].number;
			lpTargetObj = &gObj[tObjNum];

			// ��������..
			if( lpMonObj->sHD[n].HitDamage > 0 )
			{
#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
				bool bSendExp = true;
				exp = gObjMonsterExpSingle(lpTargetObj, lpMonObj, lpMonObj->sHD[n].HitDamage, (int)lpMonObj->MaxLife, bSendExp);
#else
				exp = gObjMonsterExpSingle(lpTargetObj, lpMonObj, lpMonObj->sHD[n].HitDamage, (int)lpMonObj->MaxLife);
#endif

#ifdef BUGFIX_GAIN_EXP_20071210
	//			if( exp > 0 )
#else
				if( exp > 0 )
#endif
				{
#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
					if (bSendExp == true) 
					{
						if( lpTargetObj->m_Index == lpObj->m_Index )	// ������ �����ָ�..
						{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
							GCKillPlayerExpSend(lpTargetObj->m_Index, lpMonObj->m_Index, exp, AttackDamage, MSBFlag);
#else
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
							if( lpObj->m_wExprienceRate > 0 )
							{
								GCKillPlayerExpSend(lpTargetObj->m_Index, lpMonObj->m_Index, exp, AttackDamage, MSBFlag);
							}
	#else
							GCKillPlayerExpSend(lpTargetObj->m_Index, lpMonObj->m_Index, exp, AttackDamage, MSBFlag);
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
							
				#ifdef PCBANG_ADVANTAGE //happycat@20050201 

					#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
						#ifdef FOR_TAIWAN
							if( IsOKPCBangBenefitAll(lpObj) )
							{
								lpMonObj->Money += exp;
							}
						#else
							if( lpObj->Level <= 150 && IsOKPCBangBenefitAll(lpObj) )
							{
								lpMonObj->Money += exp;
							}
						#endif
					#else
						#ifdef FOR_TAIWAN
							if (lpTargetObj->m_bPCBangUser)	lpMonObj->Money += exp;
						#else
							if (lpObj->Level <= 150 && lpTargetObj->m_bPCBangUser)	lpMonObj->Money += exp;
						#endif
					#endif //PCBANG_ADVANTAGE_EXPAND_20060919

				#endif //��Ÿ ģ ���� 150 ���� ������ PC�� �����̸�? double money!
											
						}
						else
						{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
							GCKillPlayerExpSend(lpTargetObj->m_Index, lpMonObj->m_Index, exp, 0, MSBFlag);
#else
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
							if( lpTargetObj->m_wExprienceRate > 0 )
							{
								GCKillPlayerExpSend(lpTargetObj->m_Index, lpMonObj->m_Index, exp, 0, MSBFlag);
							}
	#else
							GCKillPlayerExpSend(lpTargetObj->m_Index, lpMonObj->m_Index, exp, 0, MSBFlag);
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
						}
					}
#else
					if( lpTargetObj->m_Index == lpObj->m_Index )	// ������ �����ָ�..
					{
						GCKillPlayerExpSend(lpTargetObj->m_Index, lpMonObj->m_Index, exp, AttackDamage, MSBFlag);
					
				#ifdef PCBANG_ADVANTAGE //happycat@20050201 
					#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
						if( lpObj->Level <= 150 && IsOKPCBangBenefitAll(lpObj) )
						{
							lpMonObj->Money += exp;
						}
					#else
						if (lpObj->Level <= 150 && lpTargetObj->m_bPCBangUser)	lpMonObj->Money += exp;
					#endif //PCBANG_ADVANTAGE_EXPAND_20060919
				#endif //��Ÿ ģ ���� 150 ���� ������ PC�� �����̸�? double money!
					}
					else GCKillPlayerExpSend(lpTargetObj->m_Index, lpMonObj->m_Index, exp, 0, MSBFlag);
#endif

#ifdef ADD_PREMIUMITEM_PANDA
					if( (lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() 
						&& lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13, 76) 
						&& lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability > 0) ||
						(lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() 
						&& lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13, 76) 
						&& lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability > 0) 
						)
					{
						// �� ���� +50%
						//lpMonObj->Money += exp;
						lpMonObj->Money += exp * 0.5;
					}
					
#endif // ADD_PREMIUMITEM_PANDA

#ifdef ADD_UNICORN_PET_20090915		// ������ �� ������ 50% �ɼ�
					if( (lpObj->pInventory[EQUIPMENT_HELPER].IsItem() 
						&& lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13, 106) 
						&& lpObj->pInventory[EQUIPMENT_HELPER].m_Durability > 0)
						)
					{
						// �� ���� +50%
						lpMonObj->Money += exp * 0.5;
					}					
#endif // ADD_UNICORN_PET_20090915

					// ���⼭ ���Ͱ� ����ڿ��� �� ���� ����
					lpMonObj->Money += exp;
				}
			}
		}
	}
}

// ���� �϶� ����ġ�� �й��Ѵ�.
// lpObj		: �����
// lpTargetobj  : ����
// AttackDamage : ������ ������ ��
// MSBFlag		: �ֻ��� ��Ʈ �����Ұ�����... ���õǸ� ��~(�������ֱ�) �ϱ�..^^
#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
int gObjMonsterExpSingle(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int dmg, int tot_dmg, bool& bSendExp)
#else
int gObjMonsterExpSingle(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int dmg, int tot_dmg)
#endif
{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// �����ͷ��� ������ ���� ���� ������ ���͸� ������ ����ġ ����.
	if( !g_MasterLevelSystem.CheckMLGetExp( lpObj, lpTargetObj ) )
	{
		bSendExp = false;
		return 0;
	}
#endif
	
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( CHECK_DEVILSQUARE(lpObj->MapNumber) ) 
#else
	if( lpObj->MapNumber == 9 ) 
#endif
	{	// �Ǹ��� ����
		return g_DevilSquare.gObjMonsterExpSingle(lpObj, lpTargetObj, dmg, tot_dmg);
	}

#ifdef BUGFIX_GAIN_EXP_20071210
	INT64 exp, maxexp=0;
#else
	int exp, maxexp=0;
#endif
	
#ifdef MODIFY_GETTING_EXP_RATE_20060214
	int level = ((lpTargetObj->Level+25)*lpTargetObj->Level)/3;
#else
#ifdef MODIFY_GETTING_EXP_RATE_20051116
	int level = ((lpTargetObj->Level+20)*lpTargetObj->Level)/4;
#else
	int level = ((lpTargetObj->Level+10)*lpTargetObj->Level)/4;
#endif
#endif // MODIFY_GETTING_EXP_RATE_20060214

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912				// ���Ϳ��� ���� ����.
	// �ڽ��� �������� 10 ������ ���̱� �����ؼ�..
	if( ( lpTargetObj->Level + 10) < ( lpObj->Level + lpObj->m_nMasterLevel ) )
	{
		level =  level * ( lpTargetObj->Level + 10 ) / ( lpObj->Level + lpObj->m_nMasterLevel );
	}
#else
	// �ڽ��� �������� 10 ������ ���̱� �����ؼ�..
	if( (lpTargetObj->Level+10) < lpObj->Level )
	{
		level =  level * (lpTargetObj->Level+10) / lpObj->Level;
	}
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912

#ifdef EXP_CAL_CHANGE	
	if( lpTargetObj->Level >= 65 )	
	{	// ���� ������ 65�̻��̸�
		//level += 200 - (lpObj->Level*0.2);

	#ifdef MODIFY_GETTING_EXP_RATE_20051116
		level += (lpTargetObj->Level-64)*(lpTargetObj->Level/3);
	#else
		level += (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
	#endif

	}
#endif

	if( level > 0 ) 
	{
		// 50%�� ���� �ϰ� �߰�
		maxexp = level/2;
	}
	else level = 0;
	
	if( maxexp < 1 ) {
		exp = level;
	}
	else 
	{
		exp = level+(rand()%maxexp);
	}
	
	exp = (dmg*exp)/tot_dmg;

#ifdef FOR_BLOODCASTLE2
	if( CHECK_BLOODCASTLE(lpObj->MapNumber) )		
	{	// ���� ĳ���� ���� ������ ����ġ�� 50%
		exp = exp*BC_MONSTER_KILL_EXP_PERCENT/100;
	}
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// ���� ����ġ ���� �� �ڽ��� �ִ� ����ġ ���ϱ�
	DWORD mymaxexp = 0;
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
	{
		mymaxexp = lpObj->m_i64NextMasterLevelExp;
	}
	else
	{
		mymaxexp = gLevelExperience[lpObj->Level];
	}
#else
	DWORD mymaxexp = gLevelExperience[lpObj->Level];
	// �ڽ��� ����ġ �ִ밪���� ũ��..
	if( exp > mymaxexp ) exp = mymaxexp;
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ����ġ ���� - �����ͷ��� ������ ����	
	if( !g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
		exp = (int)((float)exp*gAddExperience);
#else
	exp = (int)((float)exp*gAddExperience);

	float _fDivider = _dynamic_experience._checkCondition(lpObj);
	// ----
	if (_fDivider != 0) 
	{
		exp = _dynamic_experience._getNewExperience(exp, _fDivider);
	}

#endif

#if defined(PCBANG_POINT_SYSTEM_20070206) && defined(ADD_PCBANG_EXPRATE)
	if( !g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
		g_PCBangPointSystem.CheckPCBangAddExperience(lpObj, exp);
#endif // ADD_PCBANG_EXPRATE

#ifdef ADD_PCS_MARK_OF_EXP_20070205
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	if( lpObj->m_wExprienceRate > 0 )
	{
		exp = (int)(exp * ((float)lpObj->m_wExprienceRate / 100 ));
	}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // ADD_PCS_MARK_OF_EXP_20070205

#ifdef MU_CRYWOLF_PENALTY_20051215
	// MVP ���� : ����ġ ȹ�� ���� 
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
		&& g_iCrywolfApplyMvpPenalty
	  )
	{
		exp = exp * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
	}
#endif

	//LogAdd("ȹ�� : [%s][%s] [%s] %d (%d / %d:%d)", lpObj->AccountID, lpObj->Name, lpTargetObj->Name, exp, lpObj->MapNumber, lpObj->X, lpObj->Y);

	if( exp > 0 ) 
	{
		if( lpObj->Type == OBJTYPE_CHARACTER )
		{
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// �߱�����> . ��ü ���� ������ ��� Ȯ���� ������.
			if (g_bChnPlayTimeLimitOn)
			{
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
				if( lpObj->m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
				{
					if (lpObj->m_iPlayTimeLimit >= 5*60*60)
					{
						exp = exp * g_iChnPlayTimeLimitLevel2Exp / 100;
					}
					else if (lpObj->m_iPlayTimeLimit >= 3*60*60)
					{
						exp = exp * g_iChnPlayTimeLimitLevel1Exp / 100;
					}
				}
	#else
				if (lpObj->m_iPlayTimeLimit >= 5*60*60)
				{
					exp = exp * g_iChnPlayTimeLimitLevel2Exp / 100;
				}
				else if (lpObj->m_iPlayTimeLimit >= 3*60*60)
				{
					exp = exp * g_iChnPlayTimeLimitLevel1Exp / 100;
				}
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
			}
#endif
			
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// ��Ʈ�� : ����ġ ȹ�淮 ����
			if (g_bVtmPlayTimeLimitOn) 
			{
				if (lpObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time*60) 
				{
					exp = exp * g_iVtmPlayTimeLimitLevel2Exp / 100;
				}
				else if (lpObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time*60) 
				{
					exp = exp * g_iVtmPlayTimeLimitLevel1Exp / 100;
				}
			}
#endif

#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
			CheckItemOptForGetExpEx( lpObj, exp );
#else
//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC ���� �� �Ʒ��ڵ� ������ ����
#ifdef PCBANG_POINT_SYSTEM_20070206		// PC�� ������ ��� Ȯ�� (����ġ)
	#ifdef ADD_NEWPVP_PKFIELD
			// ��ī�������㿡�� PC�� ������(����ġ ���) ��� ����
			if (g_NewPVP.IsVulcanusMap(lpObj->MapNumber)) {
				g_NewPVP.CheckItemOptForGetExp(lpObj, exp);
			} else {
				g_PCBangPointSystem.CheckItemOptForGetExp( lpObj, exp );
			}
	#else
			g_PCBangPointSystem.CheckItemOptForGetExp( lpObj, exp );
	#endif // ADD_NEWPVP_PKFIELD
#endif
//<--
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
/*
		lpObj->Experience += (int)exp;
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004		// !!NOT
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
			if( lpObj->m_wExprienceRate > 0 )
			{
				lpObj->Experience += exp;
			}
	#else
			lpObj->Experience += exp;
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
*/
#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
			lpObj->Experience += exp;
#else
//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC ���� �� �Ʒ��ڵ� ������ ����
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
		CheckItemOptForGetExp(lpObj, exp);
	#else
		int iExpRate = 0;
		iExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK3 ) == false
			&& iExpRate == 0
			)
		{
			iExpRate = 100;
			exp = ( exp * iExpRate ) / 100;
		}
	#endif// MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
		
		lpObj->Experience += exp;
		
#else
	#ifdef PCBANG_POINT_SYSTEM_20070206			// �Ϲ� ���� ����ġ ����
		if( lpObj->m_iPCBangPointItemTime )
		{					
			if( lpObj->m_bPCBangPointNoMoreExp > 0 )
			{
				// ����ġ ���� ����
				exp = 0;
			}
			else if( lpObj->m_iPCBangPointItemAddExpPer > 0 )
			{
				// ����ġ ���
				exp = exp * lpObj->m_iPCBangPointItemAddExpPer / 100;
			}
		}
	#endif
		
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
		if( lpObj->m_wExprienceRate > 0 )
		{
			lpObj->Experience += exp;
		}
	#else
		lpObj->Experience += exp;
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
//<--
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
			if (gObjLevelUp(lpObj, exp, lpTargetObj->Class, EVENT_TYPE_NONE) == false)
				bSendExp = false;
#else
	#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
			if (gObjLevelUp(lpObj, exp) == false)
				bSendExp = false;
	#endif
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
		}
	}
	return exp;
}


void gObjExpParty(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int AttackDamage, int MSBFlag)
{
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( CHECK_DEVILSQUARE(lpObj->MapNumber) ) 
#else
	if( lpObj->MapNumber == 9 ) 
#endif
	{	// �Ǹ��� ����
		g_DevilSquare.gObjExpParty(lpObj, lpTargetObj, AttackDamage, MSBFlag);
		return;
	}

	int n;
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	INT64 exp, maxexp=0, totalexp;
#else
	int exp, maxexp=0, totalexp;
#endif

#ifdef MODIFY_GETTING_EXP_RATE_20060214
	int level = ((lpTargetObj->Level+25)*lpTargetObj->Level)/3;
#else
#ifdef MODIFY_GETTING_EXP_RATE_20051116
	int level = ((lpTargetObj->Level+20)*lpTargetObj->Level)/4;
#else
	int level = ((lpTargetObj->Level+10)*lpTargetObj->Level)/4;
#endif
#endif // MODIFY_GETTING_EXP_RATE_20060214

	int number;
	int partynum=0;
	int totallevel=0;
	int partylevel;
	int partycount;
	int dis[MAX_PARTYUSER];
	int viewplayer=0;
	int viewpercent=100;

	int bApplaySetParty = FALSE;	// ��Ʈ��Ƽ ����ġ�� �����ϴ°�?

#ifdef UPDATE_ADD_NEW_PARTY_SET_EXP_20080325
	BYTE bCheckSetParty[6];
#else
	#ifdef DARKLORD_WORK
		BYTE bCheckSetParty[5];
	#else
		BYTE bCheckSetParty[4];
	#endif //DARKLORD_WORK
#endif //UPDATE_ADD_NEW_PARTY_SET_20080325
	
	LPOBJECTSTRUCT lpPartyObj;

	partynum = lpObj->PartyNumber;

#ifdef MODIFY_PARTY_PLAY_EXPERIENCE_20070208

	// ���� ���� ������ ��Ƽ���� ����
	int toplevel = 0;
	int iTmpDis;
	// �ִ� ������ ��Ƽ�� ���ϱ�
	for( n=0; n<MAX_PARTYUSER; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];

#ifdef MODIFY_PARTY_PLAY_EXPERIENCE_2_20070320
			iTmpDis = gObjCalDistance(lpTargetObj, lpPartyObj);
#else
			iTmpDis = gObjCalDistance(lpTargetObj, &lpPartyObj[number]);
#endif
			if( iTmpDis < 10 && lpPartyObj->Level > toplevel )
			{
				toplevel = lpPartyObj->Level;
			}

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - ��Ƽ���� �ְ� ���� ���ϱ�
			if( iTmpDis < 10 && ( lpPartyObj->Level + lpPartyObj->m_nMasterLevel ) > toplevel )
			{
				toplevel = lpPartyObj->Level + lpPartyObj->m_nMasterLevel;
			}
#else
			if( iTmpDis < 10 && lpPartyObj->Level > toplevel )
			{
				toplevel = lpPartyObj->Level;
			}
#endif
		}
	}

#else	// MODIFY_PARTY_PLAY_EXPERIENCE_20070208
	
#ifdef UPDATE_LOWLEVEL_SUPPORT_1_20051215
	// ���� ���� ������ ��Ƽ���� ����
	int toplevel = 0;
	// �ִ� ������ ��Ƽ�� ���ϱ�
	for( n=0; n<MAX_PARTYUSER; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];

			if( lpPartyObj->Level > toplevel )
			{
				toplevel = lpPartyObj->Level;
			}
		}
	}
#endif	// UPDATE_LOWLEVEL_SUPPORT_1_20051215

#endif	// MODIFY_PARTY_PLAY_EXPERIENCE_20070208

	if( !CHECK_LIMIT(partynum, MAX_PARTY) )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return;
	}

	memset(bCheckSetParty, 0, sizeof(bCheckSetParty));

	partycount = gParty.m_PartyS[partynum].Count;
	for( n=0; n<MAX_PARTYUSER; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];
			
			// ���� �ʿ� �����ϰų�..
			if( lpTargetObj->MapNumber == lpPartyObj->MapNumber) 
			{
				dis[n] = gObjCalDistance(lpTargetObj, &gObj[number]);
				if( dis[n] < 10 )
				{
					lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// ���շ��� ���� - ��Ƽ����ġ ��� : ���� 200�̻� ���̳� �� ó��
					// ����ġ ��� ��� ������ ���� ���� 200�̻� ���̳��� ������ ������ 200�� ����
					if( toplevel >= ( lpPartyObj->Level + lpPartyObj->m_nMasterLevel + 200 ) )
					{
						totallevel += lpPartyObj->Level + lpPartyObj->m_nMasterLevel + 200;
					}
					else
					{
						totallevel += lpPartyObj->Level + lpPartyObj->m_nMasterLevel;
					}
#else
#ifdef UPDATE_LOWLEVEL_SUPPORT_1_20051215
					// ����ġ ��� ��� ������ ���� ���� 200�̻� ���̳��� ������ ������ 200�� ���� - hnine
					if( toplevel >= (lpPartyObj->Level + 200) )
					{
						totallevel += lpPartyObj->Level + 200;
					}
					else
					{
						totallevel += lpPartyObj->Level;
					}
#else
					totallevel += lpPartyObj->Level;
#endif	// UPDATE_LOWLEVEL_SUPPORT_1_20051215
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912
					viewplayer++;

					bCheckSetParty[lpPartyObj->Class] = 1;
				}
			}
		}
	}

#ifdef UPDATE_ADD_NEW_PARTY_SET_EXP_20080325
	if( bCheckSetParty[0] && bCheckSetParty[1] && bCheckSetParty[2]			// ����, ���, ���� ��Ʈ
		|| bCheckSetParty[3] && bCheckSetParty[4] && bCheckSetParty[5] )	// ���˻�, ��ũ�ε�, ��ȯ���� ��Ʈ
#else
	if( bCheckSetParty[0] && bCheckSetParty[1] && bCheckSetParty[2] )		// ����, ���, ���� ��Ʈ
#endif //UPDATE_ADD_NEW_PARTY_SET_EXP_20080325
	{	
		// ��Ʈ ��Ƽ ����(����, ���, ������ ������ �ȴ�)		
		bApplaySetParty = TRUE;
	}

	if( viewplayer > 1 )
	{

#ifdef MODIFY_GETTING_EXP_RATE_20060214
		if( bApplaySetParty )
		{	
			// ��Ʈ ����ġ�� ����Ǹ�
			if( viewplayer == 3 )		
			{
				viewpercent = 230;
			}
			else if( viewplayer == 4 )
			{
				viewpercent = 270;
			}
			else if( viewplayer >= 5 )	
			{
				viewpercent = 300;
			}
			else
			{
				viewpercent = 120;
			}
		}
		else
		{
			if( viewplayer == 2 )
			{
				viewpercent = 160;
			}
			else if( viewplayer == 3 )
			{
				viewpercent = 180;
			}
			else if( viewplayer == 4 )	
			{
				viewpercent = 200;
			}
			else if( viewplayer >= 5 )	
			{
				viewpercent = 220;
			}
			else
			{
				viewpercent = 150;
			}
			
		}
#else
	#ifdef MODIFY_GETTING_EXP_RATE_20051116
		if( bApplaySetParty )
		{	
			// ��Ʈ ����ġ�� ����Ǹ�
			if( viewplayer == 3 )		
			{
				viewpercent = 220;
			}
			else if( viewplayer == 4 )
			{
				viewpercent = 260;
			}
			else if( viewplayer >= 5 )	
			{
				viewpercent = 280;
			}
			else
			{
				viewpercent = 120;
			}

		}
		else
		{
			if( viewplayer == 3 )
			{
				viewpercent = 170;
			}
			else if( viewplayer == 4 )	
			{
				viewpercent = 190;
			}
			else if( viewplayer >= 5 )	
			{
				viewpercent = 210;
			}
			else
			{
				viewpercent = 150;
			}
			
		}

	#else

	#ifdef NEW_FORSKYLAND3
			if( bApplaySetParty )
			{	// ��Ʈ ����ġ�� ����Ǹ�
				if( viewplayer == 3 )	viewpercent = 200;
				else if( viewplayer == 4 )	viewpercent = 230;
				else if( viewplayer >= 5 )	viewpercent = 260;
				else viewpercent = 120;
			}	
	#else
			if( bApplaySetParty )
			{	// ��Ʈ ����ġ�� ����Ǹ�
				if( viewplayer == 3 )	viewpercent = 170;
				else if( viewplayer == 4 )	viewpercent = 190;
				else if( viewplayer >= 5 )	viewpercent = 220;
				else viewpercent = 120;
			}
	#endif
			else
			{
				if( viewplayer == 3 )	viewpercent = 150;
				else if( viewplayer == 4 )	viewpercent = 170;
				else if( viewplayer >= 5 )	viewpercent = 190;
				else viewpercent = 120;
			}

	#endif // MODIFY_GETTING_EXP_RATE_20051116
#endif // MODIFY_GETTING_EXP_RATE_20060214

	
		partylevel = totallevel/viewplayer;
		
	}
	else
	{
		partylevel = totallevel;
	}

	if((lpTargetObj->Level+10) < partylevel)
	{
		level =  level * (lpTargetObj->Level+10) / partylevel;
	}

#ifdef EXP_CAL_CHANGE
	if( lpTargetObj->Level >= 65 )	
	{	// ���� ������ 50�̻��̸�
		if( viewplayer == 1 )
		{
	#ifdef MODIFY_GETTING_EXP_RATE_20051116
			level += (lpTargetObj->Level-64)*(lpTargetObj->Level/3);
	#else
			level += (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
	#endif
		}
		else
		{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� - ���� ������ 64 �ʰ� ����
			level += 200 - ( ( lpObj->Level + lpObj->m_nMasterLevel ) * 0.2 );
#else			
			level += 200 - (lpObj->Level*0.2);
#endif
		}
	}
#endif

	if( level > 0 ) 
	{
		maxexp = level/2;
	}
	else level = 0;
	if( maxexp < 1 ) {
		totalexp = level;
	}
	else totalexp = level+(rand()%maxexp);
	
	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		lpTargetObj->Money = totalexp;
	}

	for( n=0; n<MAX_PARTYUSER; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];
			
			// ���� �ʿ� �����ϰų�..
			if( lpTargetObj->MapNumber == lpPartyObj->MapNumber) 
			{
				// ����� �Ÿ��� �����ϴ� �÷��̾..
				if( dis[n] < 10 )
				{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - ��Ƽ ����ġ ���� �� �ִ� ����ġ ���ϱ�
					INT64 mymaxexp = 0;
					if( !g_MasterLevelSystem.CheckMLGetExp( lpPartyObj, lpTargetObj ) )
					{	
						// ��Ƽ���� �����ͷ��� ������ ���ѷ��� ������ ���͸� ������ ��ġ ����.
						exp = 0;
					}
					else
					{
						if( g_MasterLevelSystem.IsMasterLevelUser( lpPartyObj ) )
						{
							mymaxexp = lpPartyObj->m_i64NextMasterLevelExp;
						}
						else
						{
							mymaxexp = gLevelExperience[lpPartyObj->Level];
						}				
						
						exp = ( totalexp * viewpercent ) * ( lpPartyObj->Level + lpPartyObj->m_nMasterLevel ) / totallevel / 100;
					}
#else
					DWORD mymaxexp = gLevelExperience[lpPartyObj->Level];
					
					exp = (totalexp*viewpercent)*lpPartyObj->Level/totallevel/100;
#endif

#ifdef FOR_BLOODCASTLE2
					if( CHECK_BLOODCASTLE(lpPartyObj->MapNumber) )		
					{	// ���� ĳ���� ���� ������ ����ġ�� 50%
						exp = exp*BC_MONSTER_KILL_EXP_PERCENT/100;
					}
#endif
					
					// �ڽ��� ����ġ �ִ밪���� ũ��..
					if( exp > mymaxexp ) exp = mymaxexp;
					
					// PK�� ����ġ�� ���ش�.
					if( lpPartyObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
					{
						exp = 0;
					}
	#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ����ġ ���� - �����ͷ��� ������ ����	
					if( !g_MasterLevelSystem.IsMasterLevelUser( lpPartyObj ) )
						exp = (int)((float)exp*gAddExperience);
	#else
					exp = (int)((float)exp*gAddExperience);
	#endif

#if defined(PCBANG_POINT_SYSTEM_20070206) && defined(ADD_PCBANG_EXPRATE)
					if( !g_MasterLevelSystem.IsMasterLevelUser( lpPartyObj ) )
						g_PCBangPointSystem.CheckPCBangAddExperience(lpPartyObj, exp);
#endif // ADD_PCBANG_EXPRATE

#ifdef ADD_PCS_MARK_OF_EXP_20070205
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
					if( lpPartyObj->m_wExprienceRate > 0 )
					{
						exp = (int)(exp * ((float)lpPartyObj->m_wExprienceRate / 100 ));
					}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // ADD_PCS_MARK_OF_EXP_20070205

#ifdef MU_CRYWOLF_PENALTY_20051215
					// MVP ���� : ����ġ ȹ�� ���� 
					if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
						&& g_iCrywolfApplyMvpPenalty	
					  )
					{
						exp = exp * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
					}
#endif
					
					if( exp > 0 ) {
						if( lpPartyObj->Type == OBJTYPE_CHARACTER ) 
						{
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// �߱�����> . ��ü ���� ������ ��� Ȯ���� ������.
							if (g_bChnPlayTimeLimitOn)
							{
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
								if( lpPartyObj->m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
								{
									if (lpPartyObj->m_iPlayTimeLimit >= 5*60*60)
									{
										exp = exp * g_iChnPlayTimeLimitLevel2Exp / 100;
									}
									else if (lpPartyObj->m_iPlayTimeLimit >= 3*60*60)
									{
										exp = exp * g_iChnPlayTimeLimitLevel1Exp / 100;
									}
								}
	#else
								if (lpPartyObj->m_iPlayTimeLimit >= 5*60*60)
								{
									exp = exp * g_iChnPlayTimeLimitLevel2Exp / 100;
								}
								else if (lpPartyObj->m_iPlayTimeLimit >= 3*60*60)
								{
									exp = exp * g_iChnPlayTimeLimitLevel1Exp / 100;
								}
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
							}
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// ��Ʈ�� : ����ġ ȹ�淮 ����
							if (g_bVtmPlayTimeLimitOn) 
							{
								if (lpPartyObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time*60) 
								{
									exp = exp * g_iVtmPlayTimeLimitLevel2Exp / 100;
								}
								else if (lpPartyObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time*60) 
								{
									exp = exp * g_iVtmPlayTimeLimitLevel1Exp / 100;
								}
							}
#endif

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
								CheckItemOptForGetExpEx( lpPartyObj, exp );
	#else
	//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC ���� �� �Ʒ��ڵ� ������ ����
	#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205

		#ifdef ADD_NEWPVP_PKFIELD
							// ��ī�������㿡�� PC�� ������(����ġ ���) ��� ����
							if (g_NewPVP.IsVulcanusMap(lpPartyObj->MapNumber)) {
								g_NewPVP.CheckItemOptForGetExp(lpPartyObj, exp);
							} else {
								g_PCBangPointSystem.CheckItemOptForGetExp( lpPartyObj, exp );
							}
		#else
							g_PCBangPointSystem.CheckItemOptForGetExp( lpPartyObj, exp );
		#endif // ADD_NEWPVP_PKFIELD
	#else
		#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
							CheckItemOptForGetExp(lpPartyObj, exp);
		#else
							int iExpRate = 0;
							iExpRate = gObjGetTotalValueOfEffect( lpPartyObj, EFFECTTYPE_EXPERIENCE );
							if( gObjCheckUsedBuffEffect( lpPartyObj, BUFFTYPE_PCBANG_POINT_MARK3 ) == false
								&& iExpRate == 0
								)
							{
								iExpRate = 100;
							}
							exp = ( exp * iExpRate ) / 100;
		#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

	#endif	// BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
	//<--
	#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

							lpPartyObj->Experience += exp;
#else
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
							if( lpPartyObj->m_wExprienceRate > 0 )
							{
								lpPartyObj->Experience += exp;
							}
	#else
							lpPartyObj->Experience += exp;
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

							if(lpTargetObj->Money < exp)
							{
								lpTargetObj->Money = exp;
							}

							/*if( exp > 1200 )
							{
								LogAddC(LOGC_RED, lMsg.Get(526), lpPartyObj->AccountID, lpPartyObj->Name, lpPartyObj->Level, exp, lpTargetObj->Name);
							}*/
							//LogAdd("ȹ�� : [%s][%s] [%s] %d (%d / %d:%d)", lpPartyObj->AccountID, lpPartyObj->Name, lpTargetObj->Name, exp, lpPartyObj->MapNumber, lpPartyObj->X, lpPartyObj->Y);

#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
							if (gObjLevelUp(lpPartyObj, exp, lpTargetObj->Class, EVENT_TYPE_PARTY) == false)
								continue;
#else
	#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
							if (gObjLevelUp(lpPartyObj, exp) == false)
								continue;
	#endif
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906

						}
					}
					if( lpPartyObj->Type == OBJTYPE_CHARACTER )
					{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			#ifdef BUGFIX_GAIN_EXP_20071210
					// *�����ͷ��� ������ ����ġ�� �̹� ��������.
						if( !g_MasterLevelSystem.IsMasterLevelUser( lpPartyObj ) )
						{
							GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
						}
			#else
						GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
			#endif
#else
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
						if( lpPartyObj->m_wExprienceRate > 0 )
						{
							GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
						}
	#else
						GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					}
				}
			}
		}
	}
}

// ���͸� �׿����� Life�� ���� ��´�.
void gObjMonsterDieLifePlus(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTartObj)
{	
	if( lpTartObj->Level < 0 || lpTartObj->Level > 255 ) return;

	lpObj->Life += lpTartObj->Level;

	if(lpObj->MonsterDieGetLife)
	{
		lpObj->Life += ((lpObj->MaxLife+lpObj->AddLife)/8)*lpObj->MonsterDieGetLife;
		//LogAddL("Life �߰� �ɼ� : %d %d", (int)lpObj->Life, (lpObj->MaxLife/8)*lpObj->MonsterDieGetLife);
	}
	if(lpObj->MonsterDieGetMana)
	{
		lpObj->Mana += ((lpObj->MaxMana+lpObj->AddMana)/8)*lpObj->MonsterDieGetMana;
	}	

#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	if( lpObj->m_wSocketOptionMonsterDieGetHp > 0 )
	{
		lpObj->Life += lpObj->m_wSocketOptionMonsterDieGetHp;
	}

	if( lpObj->m_wSocketOptionMonsterDieGetMana > 0 )
	{
		lpObj->Mana += lpObj->m_wSocketOptionMonsterDieGetMana;
	}
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804

#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	// �нú�2�� - ���� ų �� ���� ȸ��
	if( lpObj->m_MPSkillOpt.iMpsMonsterDieGetMana > 0 )
	{
		lpObj->Mana	+= (float)( ( lpObj->MaxMana + lpObj->AddMana ) / lpObj->m_MPSkillOpt.iMpsMonsterDieGetMana );
	}
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	// �нú�2�� - ���� ų �� ����� ȸ��
	if( lpObj->m_MPSkillOpt.iMpsMonsterDieGetLife > 0 )
	{
		lpObj->Life	+= (float)( ( lpObj->MaxLife + lpObj->AddLife ) / lpObj->m_MPSkillOpt.iMpsMonsterDieGetLife );
	}
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	// �нú�2�� - ���� ų �� SD ȸ��
	if( lpObj->m_MPSkillOpt.iMpsMonsterDieGetSD > 0 )
	{
		lpObj->iShield	+= (float)( ( lpObj->iMaxShield + lpObj->iAddShield ) / lpObj->m_MPSkillOpt.iMpsMonsterDieGetSD );
	}
#endif	

#ifdef MODIFY_SKILL_ML_MONSTER_KILL_SD_BUGFIX_20080514	// ��Ʈ ų �� SD ȸ�� ��ø ����
	if( lpObj->m_MPSkillOpt.iMpsMonsterDieGetSD > 0 )
	{
		if( lpObj->iShield > (lpObj->iMaxShield+lpObj->iAddShield)) 
		lpObj->iShield = (lpObj->iMaxShield+lpObj->iAddShield);
	}
#endif	// MODIFY_SKILL_ML_MONSTER_KILL_SD_BUGFIX_20080514
	
	if( lpObj->Life > (lpObj->MaxLife+lpObj->AddLife)) 
		lpObj->Life = (lpObj->MaxLife+lpObj->AddLife);

#ifdef ADD_SHIELD_POINT_01_20060403
	GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
	GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif

#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	if(lpObj->MonsterDieGetMana || lpObj->m_MPSkillOpt.iMpsMonsterDieGetMana > 0 || lpObj->m_wSocketOptionMonsterDieGetMana > 0 )
#else
	#ifdef MODIFY_SKILL_ML_MONSTER_KILL_SD_BUGFIX_20080514	//��Ʈ ų �� ���� ȸ�� ��ø ����
	if(lpObj->MonsterDieGetMana || lpObj->m_MPSkillOpt.iMpsMonsterDieGetMana > 0)
	#else	// MODIFY_SKILL_ML_MONSTER_KILL_SD_BUGFIX_20080514
	if(lpObj->MonsterDieGetMana)
	#endif	// MODIFY_SKILL_ML_MONSTER_KILL_SD_BUGFIX_20080514
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	{
		if( lpObj->Mana > (lpObj->MaxMana+lpObj->AddMana)) 
			lpObj->Mana = (lpObj->MaxMana+lpObj->AddMana);

#ifdef NEW_FORSKYLAND2
		GCManaSend(lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
#else
		GCManaSend(lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, 0);
#endif
	}
	

#ifdef TEST_MSG_MLS_ACTIVE_20080131
#if TESTSERVER == 1
	#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107
	PMSG_NOTICE	pNotice;
	CHAR szNotice[256] = {0,};
	float	fAddValueForMsg = 0.0;
	TNotice::SetNoticeProperty( &pNotice, TNOTICE_TYPE_BOTTOM, TNOTICE_COLOR_RED );
	if( lpObj->m_MPSkillOpt.iMpsMonsterDieGetMana > 0 )
	{
		fAddValueForMsg	= (float)( ( lpObj->MaxMana + lpObj->AddMana ) / lpObj->m_MPSkillOpt.iMpsMonsterDieGetMana );
		sprintf( szNotice, "[MLS_MANA][%s:%.2f]", lpObj->Name, fAddValueForMsg );
		TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, szNotice);
		TNotice::SendNoticeToUser( lpObj->m_Index, &pNotice );		
	}
	if( lpObj->m_MPSkillOpt.iMpsMonsterDieGetLife > 0 )
	{
		fAddValueForMsg	= (float)( ( lpObj->MaxLife + lpObj->AddLife ) / lpObj->m_MPSkillOpt.iMpsMonsterDieGetLife );		
		sprintf( szNotice, "[MLS_LIFE][%s:%.2f]", lpObj->Name, fAddValueForMsg );
		TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, szNotice);
		TNotice::SendNoticeToUser( lpObj->m_Index, &pNotice );		
	}
	if( lpObj->m_MPSkillOpt.iMpsMonsterDieGetSD > 0 )
	{
		fAddValueForMsg	= (int)( ( lpObj->iMaxShield + lpObj->iAddShield ) / lpObj->m_MPSkillOpt.iMpsMonsterDieGetSD );		
		sprintf( szNotice, "[MLS_SD][%s:%.0f]", lpObj->Name, fAddValueForMsg );
		TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, szNotice);
		TNotice::SendNoticeToUser( lpObj->m_Index, &pNotice );		
	}
	#endif
#endif	// TESTSERVER == 1
#endif	// TEST_MSG_MLS_ACTIVE_20080131

}

// ������ ���� �����ߴٸ� Life�� üũ�ؼ� �׿����� ó���� ���ش�.
// lpTargetObj : ������ ���� ������Ʈ(��?)
// lpObj : ������ ���� ������Ʈ(���)

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage)
#else

#ifdef ADD_SHIELD_POINT_01_20060403
#ifdef ADD_SKILL_WITH_COMBO
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill, int iShieldDamage)
#else
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, int iShieldDamage)
#endif
#else
#ifdef ADD_SKILL_WITH_COMBO
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill)
#else
void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage)
#endif
#endif
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
{	
	TNotice notice;
	//---
	if(lpTargetObj->Life <= 0 && lpTargetObj->Live != 0 )
	{
		if(lpTargetObj->Type == OBJTYPE_MONSTER && lpTargetObj->Class == 78)
		{	
			notice.SendToAllUser("[%s] derroto un Goblin Dorado.",lpObj->Name);
			//notice.SendToAllUser("[PVP Event] Inicio el evento! - %d jugadores", this->m_userList.size());

		}
    }

	if(lpTargetObj->Life <= 0 && lpTargetObj->Live != 0 )
	{
		if(lpTargetObj->Type == OBJTYPE_MONSTER && lpTargetObj->Class == 53)
		{	
			notice.SendToAllUser("[%s] derroto un Titan Dorado.",lpObj->Name);
		}
    }

	if(lpTargetObj->Life <= 0 && lpTargetObj->Live != 0 )
	{
		if(lpTargetObj->Type == OBJTYPE_MONSTER && lpTargetObj->Class == 79)
		{	
			notice.SendToAllUser("[%s] derroto un Dragon Dorado.",lpObj->Name);
		}
    }

	if(lpTargetObj->Life <= 0 && lpTargetObj->Live != 0 )
	{
		if(lpTargetObj->Type == OBJTYPE_MONSTER && lpTargetObj->Class == 80)
		{	
			notice.SendToAllUser("[%s] derroto un Lizard King Dorado.",lpObj->Name);
		}
    }

	if(lpTargetObj->Life <= 0 && lpTargetObj->Live != 0 )
	{
		if(lpTargetObj->Type == OBJTYPE_MONSTER && lpTargetObj->Class == 82)
		{	
			notice.SendToAllUser("[%s] derroto un Tantalo Dorado.",lpObj->Name);
		}
    }


	if( lpObj->Connected < CSS_GAMEPLAYING )	// ����ڰ� ������ �����ų� ������� �ƴ϶��
	{
		return;
	}

#ifdef ADD_NEW_MAP_KALIMA_20040518
	if (CHECK_KALIMAGATE(lpObj->m_Attribute) ||
		CHECK_KALIMAGATE(lpTargetObj->m_Attribute)
		) 
	{
		// ������ ������ ������ ������ �ʴ´�.
		return;
	}
#endif
	
	LPOBJECTSTRUCT lpCallObj;		// ��ȯ�� ����

	lpCallObj = lpObj;
	if( lpObj->Type == OBJTYPE_MONSTER )
	{
		if( lpObj->m_RecallMon >= 0 ) 
		{
			lpCallObj = &gObj[lpObj->m_RecallMon];
		}
	}
	// Ÿ���� �޾Ұ� ���ݹ޴°� ���Ͷ��
	if( AttackDamage && lpTargetObj->Type == OBJTYPE_MONSTER )
	{		
		if( lpTargetObj->m_RecallMon >= 0 )
		{
			GCRecallMonLife(lpTargetObj->m_RecallMon, (int)lpTargetObj->MaxLife, (int)lpTargetObj->Life);
		}
		// ��ġ �й踦 ���� Ÿ�ݰ��� �����Ѵ�.
		gObjMonsterSetHitDamage(lpTargetObj, lpCallObj->m_Index, AttackDamage );		
	}
	
	// ����� �׾���..?
	if( (lpTargetObj->Life <= 0) && lpTargetObj->Live) 
	{
		// �����ڰ� ���͸� ��븦 �׿����� �ٸ� ��븦 ã����� �˸�
		if( lpObj->Type == OBJTYPE_MONSTER)
		{
			gObjAddMsgSend(lpObj, 3, lpTargetObj->m_Index);
			CreateFrustrum(lpObj->X,lpObj->Y, lpObj->m_Index);
			lpTargetObj->KillerType = KT_MONSTER;
		}

#ifdef DARKLORD_WORK
		if( lpObj->Type == OBJTYPE_CHARACTER)
		{	// ��ũ���Ǹ��� ����� ���ش�
			gDarkSpirit[lpObj->m_Index].ReSetTarget(lpTargetObj->m_Index);
		}
#endif


#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
		// KillCount�� ���� ��Ų��.
		if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER)
		{
			gObjSetKillCount( lpObj->m_Index );

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ĳ���͸� �׿����� ��ų ����Ʈ ����
			if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
				&& g_IllusionTempleEvent.GetIllusionTempleState(lpObj->MapNumber) == ILLUSION_TEMPLE_STATE_PLAYING)
			{
				BYTE btSkillPoint = g_IllusionTempleEvent.IncSkillPoint(lpObj->m_Index, lpObj->MapNumber, OBJTYPE_CHARACTER);
				GCIllusionTempleSkillPoint(lpObj->m_Index, btSkillPoint);
				
#ifdef  ADD_LOG_ILLUSION_TEMPLE_1_20070727
				LogAddTD("[Illusion Temple] (%d) (%s)(%s) Character Killed, (point: %d)", 
					lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1 +1, lpObj->AccountID, lpObj->Name, btSkillPoint);
#endif
			}
#endif	
		}
#endif
		// ��밡 ���Ͷ��..
		if( lpTargetObj->Type == OBJTYPE_MONSTER )
		{	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ���͸� �׿����� ��ų ����Ʈ ����
			if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
				&& g_IllusionTempleEvent.GetIllusionTempleState(lpObj->MapNumber) == ILLUSION_TEMPLE_STATE_PLAYING)
			{
				BYTE btSkillPoint = g_IllusionTempleEvent.IncSkillPoint(lpObj->m_Index, lpObj->MapNumber, OBJTYPE_MONSTER);
				GCIllusionTempleSkillPoint(lpObj->m_Index, btSkillPoint);

				lpTargetObj->m_PosNum	= -1;

#ifdef  ADD_LOG_ILLUSION_TEMPLE_1_20070727
				LogAddTD("[Illusion Temple] (%d) (%s)(%s) Monster Killed, (point: %d)", 
					lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1 +1, lpObj->AccountID, lpObj->Name, btSkillPoint);
#endif
			}
#endif

#ifdef ADD_QPROCESSING_STATE_OF_DEAD_20050221
			// ��� & ������
			if( lpTargetObj->Class == 275 || lpTargetObj->Class == 295
	#ifdef CRYWOLF_MONSTER_WORK_20050914
				|| lpTargetObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE
	#endif
				)
			{
				// �޽��� ť�� ó���� ���� ���¸� ������ �ִ´�.
				gObjAddMsgSendDelayInSpecificQPos(lpTargetObj, 1, lpObj->m_Index, 800, 0);
			}
			else
			{
				gObjAddMsgSendDelay(lpTargetObj, 1, lpObj->m_Index, 800);
			}
#else
			gObjAddMsgSendDelay(lpTargetObj, 1, lpObj->m_Index, 800);
#endif

#ifdef CRYWOLF_MONSTER_WORK_20050914
			g_Crywolf.CrywolfMonsterDieProc(lpTargetObj->m_Index, lpObj->m_Index);
#endif
			
#ifdef ADD_KANTURU_20060627
			g_Kanturu.KanturuMonsterDieProc( lpTargetObj->m_Index, lpObj->m_Index );
#endif
			
#ifdef CASTLEDEEP_EVENT_20050413
			if (lpTargetObj->m_Attribute == MON_ATTR_CASTLEDEEP) {
				char szMsg[256];

				switch(lpTargetObj->Class) {
				case 300:
				case 301:
				case 302:
				case 303:
					{
						// �߰� ������ ������ ������ �༮�� ����Ѵ�.
						ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE) lpObj->X, (BYTE) lpObj->Y, ItemGetNumberMake(14, 13), 0, (BYTE)0, 0, 0, 0, lpObj->m_Index);

						// ���� ������ �ۼ��Ѵ�.
						if(gObjIsConnected(lpObj->m_Index)) {
							
#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
							// 3227 "�η������� ���� ������ %s �� %s ���� ����ġ�̽��ϴ�."
							wsprintf(szMsg, lMsg.Get(3227), lpTargetObj->Name, lpObj->Name);
#endif
							LogAddTD("[CastleDeep Event] [%s][%s] Boss Monster Killed : %s",
								lpObj->AccountID,
								lpObj->Name,
								lpTargetObj->Name
								);
						}
						else {

#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
							// 3228 "�η������� ���� ������ %s �� ����Ǿ����ϴ�."
							wsprintf(szMsg, lMsg.Get(3228), lpTargetObj->Name);
#endif
							LogAddTD("[CastleDeep Event] Boss Monster Killed : %s",
								lpTargetObj->Name
								);
						}

						PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
						TNotice::MakeNoticeMsg(&pNotice, 0, szMsg);
#else
						pNotice.type = 0;
						strcpy(pNotice.Notice, szMsg);
						PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
						
						for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
						{
							if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
							{
								DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
							}
						}
					}
					break;
				case 295:
					{
						if(gObjIsConnected(lpObj->m_Index)) {
							
#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
							// 3229 "�η������� ���� ������ ������ �δ븦 %s ���� ����ġ�̽��ϴ�."
							wsprintf(szMsg, lMsg.Get(3229), lpObj->Name);
#endif

							LogAddTD("[CastleDeep Event] [%s][%s] Boss Monster Killed : %s",
								lpObj->AccountID,
								lpObj->Name,
								lpTargetObj->Name
								);
						}
						else {

#ifdef MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013
							// 3230 "�η������� ���� ������ ������ �δ밡 ����Ǿ����ϴ�."
							wsprintf(szMsg, lMsg.Get(3230));
#endif

							LogAddTD("[CastleDeep Event] Boss Monster Killed : %s",
								lpTargetObj->Name
								);
						}
					
						PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
						TNotice::MakeNoticeMsg(&pNotice, 0, szMsg);
#else
						pNotice.type = 0;
						strcpy(pNotice.Notice, szMsg);
						PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
						for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
						{
							if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
							{
								DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
							}
						}
					}
					break;
				}
			}
#endif



#ifdef FOR_BLOODCASTLE
			if (CHECK_BLOODCASTLE(lpTargetObj->MapNumber)) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpTargetObj->MapNumber );
#endif
				if (lpTargetObj->Class == 131) {	// �������� �����̴�.
					// ����鿡�� ������ �ı��Ǿ��ٴ� ������ �Ѵ�.
					int iMaxHitIndex = gObjMonsterTopHitDamageUser(lpTargetObj);
					if( iMaxHitIndex != -1 )
					{	
						// ���� ���� ���� ����� ����
						// "%s���� ������ �ı� �ϼ̽��ϴ�"
						char msg[255];
						wsprintf(msg, lMsg.Get(1178), gObj[iMaxHitIndex].Name);
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
						g_BloodCastle.SendNoticeMessage(iBridgeIndex, msg);
#else
						g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1, msg);
#endif

						// ������ �ı��� ����ڿ� ��Ƽ��ȣ�� �����Ѵ�.
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Door_Party = gObj[iMaxHitIndex].PartyNumber;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Door_Index = iMaxHitIndex;
						memcpy (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName, gObj[iMaxHitIndex].Name, MAX_IDSTRING);
						memcpy (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID, gObj[iMaxHitIndex].AccountID, MAX_IDSTRING);
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName[MAX_IDSTRING] = '\0';
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID[MAX_IDSTRING] = '\0';

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
						LogAddTD("[Blood Castle] (%d) Door Terminated -> %d [%s][%s]", 
								iBridgeIndex+1, 
								g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT, 
								gObj[iMaxHitIndex].AccountID, 
								gObj[iMaxHitIndex].Name
								);
#else
						LogAddTD("[Blood Castle] (%d) Door Terminated -> %d [%s][%s]", 
								lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1+1, 
								g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT, 
								gObj[iMaxHitIndex].AccountID, 
								gObj[iMaxHitIndex].Name
								);
#endif
					}
					else
					{	// Ȥ�ö�.. �������� ������... �� �޽��� ��� "����ĳ�� ������ �ı��Ǿ����ϴ�."
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
						g_BloodCastle.SendNoticeMessage(iBridgeIndex, lMsg.Get(1169));	
						LogAddTD("[Blood Castle] (%d) Door Terminated -> %d)", 
							iBridgeIndex+1, 
							g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
#else
						g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1, lMsg.Get(1169));	
						LogAddTD("[Blood Castle] (%d) Door Terminated -> %d)", 
							lpObj->MapNumber-MAP_INDEX_BLOODCASTLE1+1, 
							g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT);
#endif
					}

				}

				if (CHECK_LIMIT(lpTargetObj->Class-BC_SAINT_STATUE_1, 3))		// �������� �����̴�
				{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
					int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpObj->MapNumber );
#endif
					// ����鿡�� ������ �ı��Ǿ��ٴ� ������ �Ѵ�.
					int iMaxHitIndex = gObjMonsterTopHitDamageUser(lpTargetObj);
					if( iMaxHitIndex != -1 )
					{	// ���� ���� ���� ����� ����
						// "%s���� ������ �ı� �ϼ̽��ϴ�"
						char msg[255];
						wsprintf(msg, lMsg.Get(1179), gObj[iMaxHitIndex].Name);
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����						
						g_BloodCastle.SendNoticeMessage(iBridgeIndex, msg);
#else
						g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1, msg);
#endif

						// ������ �ı��� ����ڿ� ��Ƽ��ȣ�� �����Ѵ�.
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Statue_Party = gObj[iMaxHitIndex].PartyNumber;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Statue_Index = gObj[iMaxHitIndex].m_Index;
						memcpy (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName, gObj[iMaxHitIndex].Name, MAX_IDSTRING);
						memcpy (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_AccountID, gObj[iMaxHitIndex].AccountID, MAX_IDSTRING);
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName[MAX_IDSTRING] = '\0';
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_AccountID[MAX_IDSTRING] = '\0';

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
						LogAddTD("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d [%s][%s]", 
							iBridgeIndex+1, 
							g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT, 
							gObj[iMaxHitIndex].AccountID, 
							gObj[iMaxHitIndex].Name
							);
#else
						LogAddTD("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d [%s][%s]", 
							lpObj->MapNumber-MAP_INDEX_BLOODCASTLE1+1, 
							g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT, 
							gObj[iMaxHitIndex].AccountID, 
							gObj[iMaxHitIndex].Name
							);
#endif
					}
					else
					{	// Ȥ�ö�.. �������� ������... �� �޽��� ��� "����ĳ�� ������ �ı��Ǿ����ϴ�."
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
						g_BloodCastle.SendNoticeMessage(iBridgeIndex, lMsg.Get(1170));	
						LogAddTD("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d", 
							iBridgeIndex+1, 
							g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
#else
						g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1, lMsg.Get(1170));	
						LogAddTD("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d", 
							lpObj->MapNumber-MAP_INDEX_BLOODCASTLE1+1, 
							g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT);
#endif
					}
				}

#ifdef FOR_BLOODCASTLE3
				// ������ �ı��� ������ �����ڸ� üũ�Ѵ�.
				for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
					if (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[i].m_iIndex != -1 &&
						gObj[g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[i].m_iIndex].Connected > 2
						)
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[i].m_bLiveWhenDoorBreak = true;
				}
#endif
			}
#endif			

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ������ ���� ���� �ֺ� 5x5�� ����ڵ��� ���ĳ���.
			if (CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) {
				if (rand()%100 < MAX_BLOWOUT_PERCENT) {
					g_ChaosCastle.SearchNBlowObjs(lpTargetObj->MapNumber, lpTargetObj->X, lpTargetObj->Y);
				}
				
				// ���� ����Ʈ�� ������ ���ҽ�Ų��.
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ�� ��ȣ ���ϱ�
				g_ChaosCastle.DelMonsterList( g_ChaosCastle.GetChaosCastleByMapNum( lpTargetObj->MapNumber ), lpTargetObj->m_Index);
#else
				g_ChaosCastle.DelMonsterList(lpTargetObj->MapNumber - MAP_INDEX_CHAOSCASTLE1, lpTargetObj->m_Index);
#endif

				// ���� ���� ���� ������� ����Ʈ�� �÷��ش�.
				int iMaxHitIndex = gObjMonsterTopHitDamageUser(lpTargetObj);
				if( iMaxHitIndex != -1 ) {
					if (CHECK_CHAOSCASTLE(gObj[iMaxHitIndex].MapNumber) && gObjIsConnected(iMaxHitIndex)) {
						gObj[iMaxHitIndex].m_cKillMonsterCount++;
					}
				}
			}
#endif

			// ���� ������ŭ �ð��� ���� ��Ų��.


			gObjPkDownTimeCheck(lpObj, lpTargetObj->Level);

			
			if( lpObj->Type == OBJTYPE_CHARACTER )
			{
				gObjAddMsgSendDelay(lpObj, 3, lpTargetObj->m_Index, 2000);
			}
		}
		else if( lpTargetObj->Type == OBJTYPE_CHARACTER )		// ������϶� ����ġ�� ��´�.
		{
#ifdef FOR_BLOODCASTLE
			if (CHECK_BLOODCASTLE(lpTargetObj->MapNumber)) {	// ���� ĳ�� �ȿ��� �׾���.
				// ���⼭ ����������� ������ �ش�.
				g_BloodCastle.SetUserState(lpTargetObj->m_Index, BC_USER_DEAD);
				// ����ĳ������ ������ ����Ʈ �������� ã�Ƽ� ������.
				g_BloodCastle.SearchUserDropQuestItem(lpTargetObj->m_Index);
			}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ȯ�� ������� �׾��� ��� ���� ����
			if(CHECK_ILLUSION_TEMPLE(lpTargetObj->MapNumber))
			{
				g_IllusionTempleEvent.DropRelicsItem(lpTargetObj->MapNumber, lpTargetObj->m_Index);
			}
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408								// ī���� ĳ�� �ȿ��� ������ ������ üũ�Ͽ� ������ �÷��ְ�, ���� ������� ���� �޽����� ������.
			if (CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) {	// ī���� ĳ�� �ȿ��� �׾���.
				g_ChaosCastle.SetUserState(lpTargetObj->m_Index, CC_USER_DEAD);
				if (CHECK_CHAOSCASTLE(lpObj->MapNumber) && gObjIsConnected(lpObj->m_Index)) {
					lpObj->m_cKillUserCount++;
				}

				// ���� ������� ���� �޽����� ������.
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ�� ��ȣ ���ϱ�
				g_ChaosCastle.SendFailMessage ( g_ChaosCastle.GetChaosCastleByMapNum( lpTargetObj->MapNumber ), lpTargetObj->m_Index);
#else
				g_ChaosCastle.SendFailMessage (lpTargetObj->MapNumber - MAP_INDEX_CHAOSCASTLE1, lpTargetObj->m_Index);
#endif

				LogAddTD("[Chaos Castle] (%d) [%s][%s] User Dead In Chaos Castle : Killed by [%s][%s]",
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ�� ��ȣ ���ϱ�
					( g_ChaosCastle.GetChaosCastleByMapNum( lpTargetObj->MapNumber ) + 1 ),
#else
					lpTargetObj->MapNumber - MAP_INDEX_CHAOSCASTLE1 +1,
#endif
					lpTargetObj->AccountID, 
					lpTargetObj->Name,
					lpObj->AccountID,
					lpObj->Name
					);
			}
#endif

			lpTargetObj->KillerType = KT_CHAR;
			gObjUserDie(lpTargetObj, lpCallObj);				// ���⼭ ����ġ ���ҵ� -> �� �� ����̶�� ����ġ ���� ����


#ifdef DUEL_SYSTEM_20031028
			BOOL _guildWar = gObjGuildWarCheck(lpCallObj, lpTargetObj);
			BOOL _bOnDuel = gObjDuelCheck(lpCallObj, lpTargetObj);

#ifdef CHINA_GM_EVENT
			if( lpTargetObj->Authority&AUTHORITY_EVENT_GM )
			{
				_guildWar = TRUE;
			}
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
			if( lpTargetObj->Authority&AUTHORITY_EVENT_GM )
			{
				_guildWar = TRUE;
			}
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#ifdef USER_DIE_MAKE_OPPOSIT_PK_20040329						// �����ڰ� �����ϴ� �׾����� PK�� ���� �ʴ´�.
#ifdef MOIDFY_PK_DAMAGE_REFLECT_NOPK_BUGFIX_20081202
			if( _guildWar == FALSE && _bOnDuel  == FALSE && lpTargetObj->m_bAttackerKilled == false) 
			{
				gObjPlayerKiller(lpCallObj, lpTargetObj, MSBDamage);
			}
#else	// MOIDFY_PK_DAMAGE_REFLECT_NOPK_BUGFIX_20081202
			if( _guildWar == FALSE && _bOnDuel  == FALSE && lpTargetObj->m_bAttackerKilled == false) gObjPlayerKiller(lpCallObj, lpTargetObj);
#endif	// MOIDFY_PK_DAMAGE_REFLECT_NOPK_BUGFIX_20081202
			lpTargetObj->m_bAttackerKilled = false;
#else
			if( _guildWar == FALSE && _bOnDuel  == FALSE ) gObjPlayerKiller(lpCallObj, lpTargetObj);
#endif

#else
#ifdef _NEW_EXDB_
			BOOL _guildWar = gObjGuildWarCheck(lpCallObj, lpTargetObj);
			if( _guildWar == FALSE ) gObjPlayerKiller(lpCallObj, lpTargetObj);
#else
			gObjPlayerKiller(lpCallObj, lpTargetObj);
#endif
#endif

			
			if( lpTargetObj->m_RecallMon >= 0 )
			{
				gObjMonsterCallKill(lpTargetObj->m_Index);
			}
		}
		lpTargetObj->Live	   = 0;
		lpTargetObj->m_State   = OBJST_DYING;	// �׾�� �ִٰ� �˸���.
		lpTargetObj->RegenTime = GetTickCount();
		lpTargetObj->DieRegen  = 1;
		lpTargetObj->PathCount = 0;

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
		if( lpCallObj->m_btDuelScore >= 5 )
		{
			gObjSendDuelEnd(&gObj[lpCallObj->m_iDuelUser]);
			gObjSendDuelEnd(lpObj);
			gObjResetDuel(lpObj);
		}
#endif
		
		// ��� ���� �� �̾����� 20ĭ �̳��� �������� ����..
#ifdef _NEW_EXDB_
		if( gObjTargetGuildWarCheck(lpTargetObj, lpCallObj) == TRUE )
		{
			lpTargetObj->KillerType = KT_GUILDWARCHAR;
		}
#endif

#ifdef DUEL_SYSTEM_20031028
		if (gObjDuelCheck(lpCallObj, lpTargetObj)) {
			lpTargetObj->KillerType = KT_DUELCHAR;

#ifdef ADD_NEWPVP_PKFIELD
			g_NewPVP.CheckScore(*lpObj, *lpTargetObj);
#else
			if (lpObj->m_btDuelScore >= 10) {
				// �켱 ������ �����Ų��.
				PMSG_ANS_END_DUEL pMsg;
				pMsg.h.c = PMHC_BYTE;
				pMsg.h.headcode = 0xAB;
				pMsg.h.size = sizeof( pMsg );
				pMsg.NumberH = HIBYTE(lpObj->m_Index);
				pMsg.NumberL = LOBYTE(lpObj->m_Index);
				memcpy(pMsg.szName, lpObj->Name, MAX_IDSTRING);
				DataSend(lpTargetObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
				lpTargetObj->m_iDuelUser = -1;

				pMsg.NumberH = HIBYTE(lpTargetObj->m_Index);
				pMsg.NumberL = LOBYTE(lpTargetObj->m_Index);
				memcpy(pMsg.szName, lpTargetObj->Name, MAX_IDSTRING);
				DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->m_iDuelUser = -1;

			#ifdef MODIFY_DARKSPIRTI_TARGETRELEASE_ON_DUEL_20060524
				// ��ũ ���Ǹ��� Ÿ�� ����
				gDarkSpirit[lpObj->m_Index].ReSetTarget(lpTargetObj->m_Index);
				gDarkSpirit[lpTargetObj->m_Index].ReSetTarget(lpObj->m_Index);
			#endif
				
				// ���� 10���� �ߴٸ� ���� �޽����� �����ش�.
				CHAR szText[256];
				wsprintf(szText, lMsg.Get(1216),								// "%s�԰��� �������� �¸��ϼ̽��ϴ�."
					lpTargetObj->Name
					);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);				// "%s�԰��� �������� �¸��ϼ̽��ϴ�."
				wsprintf(szText, lMsg.Get(1217),								// "%s�԰��� �������� �й��ϼ̽��ϴ�."
					lpObj->Name
					);
				GCServerMsgStringSend(szText, lpTargetObj->m_Index, 1);			// "%s�԰��� �������� �й��ϼ̽��ϴ�."
				
				
				// ���� �ʿ� ������ �Ͷ߸���.
				PMSG_SERVERCMD	ServerCmd;
				PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
				ServerCmd.CmdType = 0;
				ServerCmd.X = lpObj->X;
				ServerCmd.Y = lpObj->Y;
				MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
				DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

#if ENABLE_MC_SQL == 1
				LogAdd("[Duel] [%s][%s] Win Duel, Loser [%s][%s]",lpObj->AccountID,lpObj->Name,lpTargetObj->AccountID,lpTargetObj->Name);
				Manager.ExecFormat("UPDATE [MuOnline].[dbo].[Character] SET DuelWins = DuelWins + 1 WHERE Name='%s'", lpObj->Name);					
			    Manager.ExecFormat("UPDATE [MuOnline].[dbo].[Character] SET DuelLoser = DuelLoser + 1 WHERE Name='%s'", lpTargetObj->Name); // sql duel wins
#endif // ENABLE_MC_SQL == 1
			}
#endif // ADD_NEWPVP_PKFIELD
		}
#endif // DUEL_SYSTEM_20031028


		
		// �����̵��� ��� ��Ų��.
		if( lpTargetObj->Teleport == 1 ) lpTargetObj->Teleport = 0;

#ifdef ADD_SKILL_WITH_COMBO
		GCDiePlayerSend(lpTargetObj, lpTargetObj->m_Index, Skill, lpObj->m_Index);
#else
		GCDiePlayerSend(lpTargetObj, lpTargetObj->m_Index);
#endif
		
#ifdef ADD_RAKLION_20080408
		// �Ź̾��� ���������� ī��Ʈ���� ������Ų��.
		if( lpTargetObj->MapNumber == MAP_INDEX_RAKLION_BOSS && lpTargetObj->Type >= OBJTYPE_MONSTER )
		{
			if( lpTargetObj->Class == 460 || lpTargetObj->Class == 461 || lpTargetObj->Class == 462 ) // �Ź̾� 3��
			{
				g_Raklion.BossEggDieDecrease();
				g_RaklionUtil.NotifyRaklionUserMonsterCount( g_Raklion.GetBossEggDieCount(), 0 );
			}
		}
#endif //ADD_RAKLION_20080408

		if( lpTargetObj->Class == 79 )
		{	// ��絥������ ������ ���̻� �̺�Ʈ����� ����						
			gEledoradoEvent.m_BossGoldDerconMapNumber[lpTargetObj->m_BossGoldDerconMapNumber] = -1;
			gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);
		}

		//-------------
		// ����ġ �й�
		//-------------		
		if( lpTargetObj->m_RecallMon >= 0 ) return;	// ��ȯ�����϶��� ����ġ�� ���� �ʴ´�.
		
#ifdef MODIFY_NON_EXP_IN_CRYWOLF_MONSTER_20060530
		// ũ���̿��� ���� ���ʹ� �׿��� ����ġ�� ���� �ʴ´�.
		if( lpTargetObj->Class == 340			// ��ũ����
//			|| lpTargetObj->Class == 341		// �Ҷ�
//			|| lpTargetObj->Class == 344		// �߶�
//			|| lpTargetObj->Class == 345		// �������Ǹ�
			|| lpTargetObj->Class == 348		// �߸���Ÿ
			|| lpTargetObj->Class == 349		// �߰���
			)
		{
			return;
		}
#endif

#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214
		// �뺴�� �׿����� ��ġ�� ���� �ʴ´�.
		if( lpTargetObj->Class == NPC_INDEX_SPEARMAN 
			|| lpTargetObj->Class == NPC_INDEX_BOWMAN )
		{
			return;
		}
#endif

#ifdef DISABLE_GETTING_EXP_FROM_KUNDUN_20070801
		if( lpTargetObj->Class == 275 )
		{
			return;
		}
#endif

#ifdef DISABLE_GETTING_EXP_FROM_GUARDTOWER_20070801
		if( lpTargetObj->Class == 288 )
		{
			return;
		}
#endif

#ifdef DISABLE_GETTING_EXP_FROM_EROHIM_20050509
		if( lpTargetObj->Class == 295 )
		{
			return;
		}
#endif
		
#ifdef KANTURU_BOSSMONSTER_EXP_NONE_20060809
		if( lpTargetObj->Class == KANTURU_MONSTER_MAYA_LEFTHAND 
			|| lpTargetObj->Class == KANTURU_MONSTER_MAYA_RIGHTHAND 
			|| lpTargetObj->Class == KANTURU_MONSTER_NIGHTMARE )
		{
			return;
		}
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// ��ũ����(�Ʒú�)�� ����ġ�� ���� �ʴ´�.
		if( lpTargetObj->Class == 412 )
		{
			return;
		}
#endif

		if( lpCallObj->Type == OBJTYPE_CHARACTER )
		{
			// ������ Ÿ���� ���� ������ ��Ƽ�� �̶��..
			if( lpCallObj->PartyNumber >= 0 )
			{
				gObjExpParty(lpCallObj, lpTargetObj, AttackDamage, MSBFlag);
			}
			else {
				if( lpTargetObj->Type == OBJTYPE_MONSTER )
				{
					gObjMonsterExpDivision(lpTargetObj, lpCallObj, AttackDamage, MSBFlag);
				}
			}

#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
			if( lpTargetObj->Type == OBJTYPE_MONSTER && CHECK_DEVILSQUARE(lpTargetObj->MapNumber) )
#else
			if( lpTargetObj->Type == OBJTYPE_MONSTER && lpTargetObj->MapNumber == 9 )
#endif
			{
				g_DevilSquare.gObjMonsterScoreDivision(lpTargetObj, lpCallObj, AttackDamage, MSBFlag);
			}
		}

#ifdef MONSTER_HERD_SYSTEM_20031120
		if (
			lpTargetObj->Type == OBJTYPE_MONSTER &&
			lpTargetObj->m_bIsInMonsterHerd
			) 
		{
			if (lpTargetObj->m_lpMonsterHerd != NULL)
				lpTargetObj->m_lpMonsterHerd->MonsterDieAction(lpTargetObj);
		}
#endif
	}
	else
	{
		// �� ����� ���� �ʾ��� ��� �������� �����ش�.!
#ifdef ADD_SHIELD_POINT_01_20060403
		if( DamageSendType == 0 )
		{
			GCDamageSend(lpCallObj->m_Index, lpTargetObj->m_Index, AttackDamage, MSBFlag, MSBDamage, iShieldDamage);
		}
		else if( DamageSendType == 1 )
		{
			GCDamageSend(lpTargetObj->m_Index, AttackDamage, iShieldDamage);
		}
#ifdef ADD_SKILL_WITH_COMBO
		else if( DamageSendType == 2 )
		{
			GCDamageSend(lpCallObj->m_Index, lpTargetObj->m_Index, AttackDamage, MSBFlag, DT_POISON, iShieldDamage);
		}
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		else if( DamageSendType == 3 )
		{
			GCDamageSend(lpCallObj->m_Index, lpTargetObj->m_Index, AttackDamage, MSBFlag, DT_SUMMONDOT, iShieldDamage);
		}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#else
		if( DamageSendType == 0 )
		{
			GCDamageSend(lpCallObj->m_Index, lpTargetObj->m_Index, AttackDamage, MSBFlag, MSBDamage);
		}
		else if( DamageSendType == 1 )
		{
			GCDamageSend(lpTargetObj->m_Index, AttackDamage);
		}
#ifdef ADD_SKILL_WITH_COMBO
		else if( DamageSendType == 2 )
		{
			GCDamageSend(lpCallObj->m_Index, lpTargetObj->m_Index, AttackDamage, MSBFlag, DT_POISON);
		}
#endif
#endif

	}	
}

//----------------------------------------------------------------------------
// �κ��丮 �����۰� ������ �ӽ� ���ۿ� ���� ��Ų��.
int gObjInventoryTrans(int aIndex)
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}
	int n;

	if( gObj[aIndex].pTransaction == 1 )	// �̵� ���̴�. �׳� ����
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status error %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pTransaction, __FILE__, __LINE__);
#ifdef ITEMCOPY_BUGFIX_20040112
//		gObj[aIndex].pTransaction = 0;
		//gObjInventoryRollback(aIndex);
		return FALSE;
#endif
	}
	
	for( n=0; n<MAX_MAGIC; n++)
	{
		gObj[aIndex].m_lpMagicBack[n] = gObj[aIndex].Magic[n];
	}
#ifdef PERSONAL_SHOP_20040113		// ī���� ���� ���� �������� �ӽù��۷� ���� -> ���λ��� ������ ���� ���� (���ڱ� ƨ���� ��츦 ���)
#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610		// ���չ��� �������� ������ ���λ��� ������ �������� �ʴ´�.
	for( n=0; n<MAX_INVENTORY; n++)
#else
	for( n=0; n<MAX_INVENTORY_EXTEND; n++)
#endif	
#else
	for( n=0; n<MAX_INVENTORY; n++)
#endif
	{
		gObj[aIndex].Inventory2[n] = gObj[aIndex].Inventory1[n];
	}
	memcpy(gObj[aIndex].InventoryMap2, gObj[aIndex].InventoryMap1, MAX_INVENTORYMAP);
	gObj[aIndex].InventoryCount2 = gObj[aIndex].InventoryCount1;
	gObjSetInventory2Pointer(&gObj[aIndex]);
	gObj[aIndex].pTransaction = 1;
	
	LogAddL("Trade Transaction (%s)", gObj[aIndex].Name );
	return TRUE;
}

//----------------------------------------------------------------------------
// �ӽ� ���ۿ� ����� ������ �����Ų��.
int gObjInventoryCommit(int aIndex)
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAddTD("error : gObjInventoryCommit() - aIndex out of bound %s %d",__FILE__, __LINE__);
		return FALSE;
	}
	int n;

#ifdef INVENTORY_CLEAR_BUGFIX_20050226
	if( gObj[aIndex].pTransaction != 1 )	// Ʈ������� �ɷ����� ���� ���´� Ŀ������ ����
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status2", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pTransaction);
		return FALSE;
	}
#else
	if( gObj[aIndex].pTransaction == 0 ) 
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status error %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pTransaction, __FILE__, __LINE__);
		return FALSE;
	}
	if( gObj[aIndex].pTransaction == 2 )
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status2", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pTransaction);
		return TRUE;
	}
#endif	

#ifdef PERSONAL_SHOP_20040113		// �ŷ� �� �������� ���� ���۷� ���� -> ���λ��� �����͵� ���� ����
//	for( n=0; n<MAX_INVENTORY_EXTEND; n++)
	for( n=0; n<MAX_INVENTORY; n++)
#else
	for( n=0; n<MAX_INVENTORY; n++)
#endif
	{
		gObj[aIndex].Inventory1[n] = gObj[aIndex].Inventory2[n];
	}
	memcpy(gObj[aIndex].InventoryMap1, gObj[aIndex].InventoryMap2, MAX_INVENTORYMAP);
	gObj[aIndex].InventoryCount1 = gObj[aIndex].InventoryCount2;
	gObjSetInventory1Pointer(&gObj[aIndex]);
	gObj[aIndex].pTransaction = 2;
	
	LogAddL("Trade Commit (%s)", gObj[aIndex].Name );

	return TRUE;
}


int gObjInventoryRollback(int aIndex)
{
	int n;

	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}
	
#ifdef INVENTORY_CLEAR_BUGFIX_20050226
	if( gObj[aIndex].pTransaction != 1 )	// Ʈ������� �ɷ����� ���� ���´� �ѹ����� ����
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status2", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pTransaction);
		return FALSE;
	}
#else
	if( gObj[aIndex].pTransaction == 0 )
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status error %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pTransaction, __FILE__, __LINE__);
		return FALSE;
	}
	// �ѹ���� �Ϸ�� ����..
	if( gObj[aIndex].pTransaction == 3 ) 
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status2", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pTransaction);
		return TRUE;
	}
#endif

	for( n=0; n<MAX_MAGIC; n++)
	{
		gObj[aIndex].Magic[n] = gObj[aIndex].m_lpMagicBack[n];
	}
	
#ifdef PERSONAL_SHOP_20040113		// �ŷ� ���� �� �ӽù����� ������ ��� ����
	for( n=0; n<MAX_INVENTORY_EXTEND; n++)
//	for( n=0; n<MAX_INVENTORY; n++)
#else
	for( n=0; n<MAX_INVENTORY; n++)
#endif
	{
		gObj[aIndex].Inventory2[n].Clear();
	}
	//memcpy(gObj[aIndex].InventoryMap1, gObj[aIndex].InventoryMap2, MAX_INVENTORYMAP);
	//gObj[aIndex].InventoryCount1 = gObj[aIndex].InventoryCount2;
	
	//memcpy(gObj[aIndex].InventoryMap1, gObj[aIndex].InventoryMap2, MAX_INVENTORYMAP);
	//gObj[aIndex].InventoryCount1 = gObj[aIndex].InventoryCount2;
	gObjSetInventory1Pointer(&gObj[aIndex]);
	gObj[aIndex].pTransaction = 3;

	LogAddL("Trade Rollback (%s)", gObj[aIndex].Name );
	return TRUE;
}


// �κ��丮���� �������� ũ�⸸ŭ �ٸ������� �ִ´�.
void gObjInventoryItemSet(int aIndex, int itempos, BYTE set_byte)
{
	if( itempos < MAX_EQUIPMENT ) return;
#ifdef PERSONAL_SHOP_20040113		
	if( itempos > MAX_INVENTORY_EXTEND-1 ) return;
#else
	if( itempos > MAX_INVENTORY-1 ) return;
#endif	
	
	int width, height;

	if( gObj[aIndex].pInventory[itempos].GetSize( width, height ) == FALSE ) 
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);
		return;
	}
	gObjInventoryItemBoxSet(aIndex, itempos, width, height, set_byte);	
}

// InventoryMap �� � ������ ��ŷ�Ѵ�.
void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx, itemposy;
	int xx, yy;

	itemposx = (itempos-MAX_EQUIPMENT)%8;
	itemposy = (itempos-MAX_EQUIPMENT)/8;

	for( int y=0; y<yl; y++)
	{
		yy = itemposy+y;
		for( int x=0; x<xl; x++)
		{
			xx = itemposx+x;
#ifdef PERSONAL_SHOP_20040113		
			if( ExtentCheck(xx, yy, 8, 12) == TRUE )
#else
			if( ExtentCheck(xx, yy, 8, 8) == TRUE )
#endif
			{
				*(gObj[aIndex].pInventoryMap+((itemposy+y)*8)+(itemposx+x)) = set_byte;
			}
			else {
	
				LogAdd("error : %s %d",__FILE__, __LINE__);
				return;
			}
		}
	}
}


#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ������ �Լ���

// DESC : �������� �����Ͱ� 1�̾�� �ϴ� ��� �������� �����͸� üũ�Ͽ� �ٷ� �����.
// P.S. :	1 . �ݵ�� �����͸� ���� �����Ͽ��� ������� �κп��� ����Ѵ� -> �ŷ��� ���ÿ� �̷�� �� �� �ִ� �κп����� ����ϸ� �ʵȴ�.
//			2 . �߰��ϴ� ��ƾ���� �ŷ� ���¸� üũ�Ͽ� ������ �κ��� �ִٸ� �׻� �� ��ƾ�� ���� ���Ŀ� ����Ѵ�.
bool gObjFixInventoryPointer(int aIndex)
{
	if (!gObjIsConnected(aIndex)) {
		LogAdd("[Fix Inv.Ptr] [%s][%s] - disconnected", 
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name
			);
		return false;
	}
	
	if (gObj[aIndex].pInventory == gObj[aIndex].Inventory1) {
		// ��κ��� ���� ���⿡ �ش�� ���̴�.
		return true;
	}
	
	if (gObj[aIndex].pInventory == gObj[aIndex].Inventory2) {
#ifdef ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825		// ������ ��ȯ �� Ʈ����� üũ
		if (gObj[aIndex].pTransaction == 1) {
			// ���� Ʈ����� ���� (�ŷ�, ī�������� ��) �̴�. (0:�ʱ� / 2:COMMIT / 3:ROLLBACK)
			LogAdd("[Fix Inv.Ptr] [%s][%s] - Transaction == 1, Do not change Pointer", 
				gObj[aIndex].AccountID, 
				gObj[aIndex].Name
				);
			return false;
		}
#endif		

		// ���⿡ ������ ���� ������ �ִ� ����̴�.
		LogAdd("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was 2", 
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name
			);

#ifdef PERSONAL_SHOP_20040113
		for( int n=0; n<MAX_INVENTORY_EXTEND; n++)
//		for( int n=0; n<MAX_INVENTORY; n++)
#else
		for( int n=0; n<MAX_INVENTORY; n++)
#endif
		{
			gObj[aIndex].Inventory2[n].Clear();			// �� �κ� �� Ʈ����� (�ŷ�, ����, ���û�) �� ���ÿ� �߻��ϸ� Ʈ����� ���� �� �κ��丮 �ʱ�ȭ (���� �ϴ°��� ���� �� ����)
		}
	}
	else {
		LogAdd("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was Wrong", 
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name
			);
	}
	
	gObjSetInventory1Pointer (&gObj[aIndex]);

	return false;
}

#endif


#ifdef PERSONAL_SHOP_20040113	// ���λ��� ������ ���� ���� - gObjInventoryItemSet(), gObjInventoryItemBoxSet() �� �Լ��� ���λ��� ������ ����

// �κ��丮���� �������� ũ�⸸ŭ �ٸ������� �ִ´�.
void gObjInventoryItemSet_PShop(int aIndex, int itempos, BYTE set_byte)
{
	if( itempos < MAX_EQUIPMENT ) return;
#ifdef PERSONAL_SHOP_20040113		
	if( itempos > MAX_INVENTORY_EXTEND-1 ) return;
#else
	if( itempos > MAX_INVENTORY-1 ) return;
#endif	
	
	int width, height;

	if( gObj[aIndex].Inventory1[itempos].GetSize( width, height ) == FALSE ) 
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);
		return;
	}
	gObjInventoryItemBoxSet_PShop(aIndex, itempos, width, height, set_byte);	
}

// InventoryMap �� � ������ ��ŷ�Ѵ�.
void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx, itemposy;
	int xx, yy;

	itemposx = (itempos-MAX_EQUIPMENT)%8;
	itemposy = (itempos-MAX_EQUIPMENT)/8;

	for( int y=0; y<yl; y++)
	{
		yy = itemposy+y;
		for( int x=0; x<xl; x++)
		{
			xx = itemposx+x;
#ifdef PERSONAL_SHOP_20040113		
			if( ExtentCheck(xx, yy, 8, 12) == TRUE )
#else
			if( ExtentCheck(xx, yy, 8, 8) == TRUE )
#endif
			{
				*(gObj[aIndex].InventoryMap1+((itemposy+y)*8)+(itemposx+x)) = set_byte;
			}
			else {
				LogAdd("error : %s %d",__FILE__, __LINE__);
				return;
			}
		}
	}
}

#endif


// 
// �������� �κ��丮���� �����Ѵ�.
BYTE gObjInventoryDeleteItem(int aIndex, int itempos)
{
	//if( itempos ) ����üũ.. ���ٱ�??
	
	// ������� �������� �����(����) �̸� �׳� ������Ű��..
	if( itempos < MAX_EQUIPMENT )
	{
		int result = gObjMagicDel(&gObj[aIndex], gObj[aIndex].pInventory[itempos].m_Special[0], (BYTE)gObj[aIndex].pInventory[itempos].m_Level);
		if( result >= 0 )
		{
			GCMagicListOneDelSend(aIndex, result, gObj[aIndex].pInventory[itempos].m_Special[0], (BYTE)gObj[aIndex].pInventory[itempos].m_Level, 0 );
		}

		gObj[aIndex].pInventory[itempos].Clear();
	}
	else // �Ʒ����̸�.
	{
		// ������ �κ��丮 ������ �����.
		gObjInventoryItemSet(aIndex, itempos, 0xFF);
		gObj[aIndex].pInventory[itempos].Clear();
	}
	return TRUE;
}


// â���� �����Ѵ�.
BYTE gObjWarehouseDeleteItem(int aIndex, int itempos)
{
	// ������ �κ��丮 ������ �����.
	gObjWarehouseItemSet(aIndex, itempos, 0xFF);
	gObj[aIndex].pWarehouse[itempos].Clear();
	return TRUE;
}

//  �����Ѵ�.
BYTE gObjChaosBoxDeleteItem(int aIndex, int itempos)
{
	// ������ �κ��丮 ������ �����.
	gObjChaosItemSet(aIndex, itempos, 0xFF);
	gObj[aIndex].pChaosBox[itempos].Clear();
	return TRUE;
}


// 
// �������� �κ��丮�� �߰��Ѵ�.
BYTE gObjInventoryInsertItem(int aIndex, int item_num)
{	
	int w, h;
	int map_num;
	int iwidth, iheight;
	BYTE blank=0;
	
	map_num = gObj[aIndex].MapNumber;	
	if( MapC[map_num].m_cItem[item_num].live == 0 ) 
	{
		
		return 0xFF;
	}

	if( MapC[map_num].m_cItem[item_num].GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);
		
		return 0xFF;
	}

	for( h=0; h<8; h++)
	{		
		for( w=0; w<8; w++)	
		{
			// ����ִ��� üũ
			if( *(gObj[aIndex].pInventoryMap+(h*8)+w) == 0xFF )
			{
				blank = gObjInventoryRectCheck(aIndex, w, h, iwidth,iheight);
				// 
				if( blank == 0xFE )	goto GOTO_EndFunc;
				if( blank != 0xFF )	// �κ��丮�� �ڸ��� �ִٸ�
				{
					gObj[aIndex].pInventory[blank] = MapC[map_num].m_cItem[item_num];
					//LogAdd("������ ��ȣ : %d", gObj[aIndex].pInventory[blank].m_Number);
					gObjInventoryItemSet(aIndex, blank, (BYTE)gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc :

	
	return 0xFF;
}


DWORD gGetItemNumber()
{
	return gItemNumberCount;
}

void gPlusItemNumber()
{
	//gItemNumberCount++;
	//gItemNumberCount = 0;
	//LogAdd("������ ��ȣ ���� : %d",gItemNumberCount);
}


// �������� �κ��丮�� �߰� -> ���λ������� ������ �� ���� �� �޼���� �������� �߰�
BYTE gObjInventoryInsertItem(int aIndex, CItem item)
{
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;

	if( item.GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);		
		return 0xFF;
	}
	for( h=0; h<8; h++)
	{
		for( w=0; w<8; w++)				
		{
			// ����ִ��� üũ
			if( *(gObj[aIndex].pInventoryMap+(h*8)+w) == 0xFF )
			{				
				blank = gObjInventoryRectCheck(aIndex, w, h, iwidth,iheight);
				// 
				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	{	// �κ��丮�� �ڸ��� �ִٸ�
					//LogAdd("�ø��� �˻�");

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202	
					if( gObjCheckSerial0ItemList(&item) )
					{
						// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
						MsgOutput( aIndex, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
						return 0xFF;
					}
#endif
					
					if( gObjInventorySearchSerialNumber(&gObj[aIndex], item.m_Number) == FALSE )
					{
						return 0xFF;
					}

					gObj[aIndex].pInventory[blank] = item;
					//LogAdd("������ ��ȣ : %d", item.m_Number);
					//gObj[aIndex].pInventory[blank].m_Number = gGetItemNumber();
					//gPlusItemNumber();
					gObjInventoryItemSet(aIndex, blank, (BYTE)gObj[aIndex].pInventory[blank].m_Type);

					return blank;
				}
			}
		}
	}
GOTO_EndFunc :
	
	return 0xFF;
}


#ifdef PERSONAL_SHOP_20040113

// �������� �κ��丮 (���λ��� ����) �� �߰��Ѵ�.
BYTE gObjOnlyInventoryInsertItem(int aIndex, CItem item)
{
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;

	if( item.GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);		
		return 0xFF;
	}
	for( h=0; h<8; h++)
	{
		for( w=0; w<8; w++)				
		{
			// ����ִ��� üũ
			if( *(gObj[aIndex].pInventoryMap+(h*8)+w) == 0xFF )
			{				
				blank = gObjOnlyInventoryRectCheck(aIndex, w, h, iwidth,iheight);
				// 
				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	{	// �κ��丮�� �ڸ��� �ִٸ�
					//LogAdd("�ø��� �˻�");

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202	
					if( gObjCheckSerial0ItemList(&item) )
					{
						// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
						MsgOutput( aIndex, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
						return 0xFF;
					}
#endif
					
					if( gObjInventorySearchSerialNumber(&gObj[aIndex], item.m_Number) == FALSE )
					{
						return 0xFF;
					}

					gObj[aIndex].pInventory[blank] = item;
					//LogAdd("������ ��ȣ : %d", item.m_Number);
					//gObj[aIndex].pInventory[blank].m_Number = gGetItemNumber();
					//gPlusItemNumber();
					gObjInventoryItemSet(aIndex, blank, (BYTE)gObj[aIndex].pInventory[blank].m_Type);

					return blank;
				}
			}
		}
	}
GOTO_EndFunc :
	
	return 0xFF;
}

#endif


BYTE gObjInventoryInsertItem(int aIndex, CMapItem* item)
{
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;
	CItem copyitem;

	if( item->GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);		
		return 0xFF;
	}


	for( h=0; h<8; h++)
	{
		for( w=0; w<8; w++)				
		{
			// ����ִ��� üũ
			if( *(gObj[aIndex].pInventoryMap+(h*8)+w) == 0xFF )
			{
#ifdef PERSONAL_SHOP_20040113			// �ʻ󿡼� ������ ���� �� �κ����� �ȿ��� ������ ����Ѵ�.
				blank = gObjOnlyInventoryRectCheck(aIndex, w, h, iwidth,iheight);
#else
				blank = gObjInventoryRectCheck(aIndex, w, h, iwidth,iheight);
#endif
			
				// 
				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	
				{	// �κ��丮�� �ڸ��� �ִٸ�
					copyitem.m_Level = item->m_Level;
					copyitem.m_Durability = item->m_Durability;

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
					copyitem.Convert(item->m_Type, item->m_Option1,item->m_Option2, item->m_Option3, item->m_NewOption, item->m_SetOption, item->m_ItemOptionEx, item->m_SocketOption, item->m_BonusSocketOption );
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
					copyitem.Convert(item->m_Type, item->m_Option1,item->m_Option2, item->m_Option3, item->m_NewOption, item->m_SetOption, item->m_ItemOptionEx);
	#else
					copyitem.Convert(item->m_Type, item->m_Option1,item->m_Option2, item->m_Option3, item->m_NewOption, item->m_SetOption);
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef DARKLORD_WORK
					copyitem.SetPetItemInfo(item->m_PetItem_Level, item->m_PetItem_Exp);
#endif
					copyitem.m_Number     = item->m_Number;


					gObj[aIndex].pInventory[blank] = copyitem;
					//LogAdd("������ ��ȣ : %d", item->m_Number);
					//gObj[aIndex].pInventory[blank].m_Number = gGetItemNumber();
					//gPlusItemNumber();
					gObjInventoryItemSet(aIndex, blank, (BYTE)gObj[aIndex].pInventory[blank].m_Type);

					/*
					if( gObjInventorySearchSerialNumber(&gObj[aIndex], item->m_Number) == FALSE )
					{
						return 0xFF;
					}
					*/
					return blank;
				}
			}
		}
	}
GOTO_EndFunc :
	
	return 0xFF;
}

// �������� ã�Ƽ� �������� �ø���.
#ifdef ADD_HP_POTION_LEVEL1_FIX_20040420
BOOL gObjSearchItem(LPOBJECTSTRUCT lpObj, int item_type, int add_dur, int nLevel)	
#else
BOOL gObjSearchItem(LPOBJECTSTRUCT lpObj, int item_type, int add_dur)
#endif
{	
	for( int n=MAX_EQUIPMENT; n<MAX_INVENTORY; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == item_type )
			{
#ifdef ADD_HP_POTION_LEVEL1_FIX_20040420
				if ( lpObj->pInventory[n].m_Level == nLevel ) 
#endif
				{
					if( ((int)lpObj->pInventory[n].m_Durability+add_dur) <= 3 )
					{
						lpObj->pInventory[n].m_Durability += add_dur;
						GCItemDurSend(lpObj->m_Index, n, (int)lpObj->pInventory[n].m_Durability, 0);
						//LogAdd("%s ������ %d", lpObj->pInventory[n].GetName(), (int)lpObj->pInventory[n].m_Durability);
						return TRUE;
					}				
				}
			}			
		}
	}
	return FALSE;
}

#ifdef MODIFY_OVERLAP_ITEM_DURATION_20070730
// Ư�� �������� �������� �˻��� ��ĥ �� �ִ� ��� �ִ������� ��ġ���� �Ѵ�.
BOOL gObjOverlapItemDuration(LPOBJECTSTRUCT lpObj, int iItemCode, int iLevel, int iAddDuration, int iMaxDuration)
{
	for( int i = MAX_EQUIPMENT ; i < MAX_INVENTORY ; i++ )
	{
		if( lpObj->pInventory[i].IsItem() == TRUE )
		{
			if( lpObj->pInventory[i].m_Type == iItemCode )
			{
				if( lpObj->pInventory[i].m_Level == iLevel )
				{
					if( ( lpObj->pInventory[i].m_Durability + iAddDuration ) <= iMaxDuration )
					{
						lpObj->pInventory[i].m_Durability += iAddDuration;
						GCItemDurSend( lpObj->m_Index, i, (int)lpObj->pInventory[i].m_Durability, 0 );
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}
#endif // MODIFY_OVERLAP_ITEM_DURATION_20070730

// ���� �������� �������� ��´�.
BOOL gObjSearchItemMinus(LPOBJECTSTRUCT lpObj, int pos, int m_dur)
{	
	int n=pos;
	
	if( n < 0 || n > MAX_INVENTORY-1 ) 
	{
		LogAdd("error-L4 : %s %d", __FILE__, __LINE__);
		return TRUE;
	}
	
	if( lpObj->pInventory[n].IsItem() == TRUE )
	{
		if( ((int)lpObj->pInventory[n].m_Durability-m_dur) > 0 )
		{
			lpObj->pInventory[n].m_Durability -= m_dur;
			GCItemDurSend(lpObj->m_Index, n, (int)lpObj->pInventory[n].m_Durability, 1);
			//LogAdd("%s ������ %d", lpObj->pInventory[n].GetName(), (int)lpObj->pInventory[n].m_Durability);
			return TRUE;
		}		
		else
		{
			//LogAdd("error-L4 : %s %d ������ �Ҹ��", __FILE__, __LINE__);
		}
	}
	else
	{
		//LogAdd("error-L4 : %s %d", __FILE__, __LINE__);
	}
	return FALSE;
}


BYTE gObjShopBuyInventoryInsertItem(int aIndex, CItem item)
{
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;


	if( item.GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);		
		return 0xFF;
	}

	for( h=0; h<8; h++)
	{
		for( w=0; w<8; w++)				
		{
			// ����ִ��� üũ
			if( *(gObj[aIndex].pInventoryMap+(h*8)+w) == 0xFF )
			{
#ifdef PERSONAL_SHOP_20040113			// NPC �������� ������ �춧 �κ����� �ȿ��� ������ ����Ѵ�.
				blank = gObjOnlyInventoryRectCheck(aIndex, w, h, iwidth,iheight);
#else
				blank = gObjInventoryRectCheck(aIndex, w, h, iwidth,iheight);
#endif

				// 
				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	{	// �κ��丮�� �ڸ��� �ִٸ�
					gObj[aIndex].pInventory[blank] = item;
					
					//if( (item.m_serial >= (14<<4)) &&
					//	(item.m_Type <= (14<<4)+10) )	// ����� ���� �ø����̸�.. ��ȣ�� ���� �ʴ´�.
					if( item.m_serial == 0)  
					{
						gObj[aIndex].pInventory[blank].m_Number = 0;
					}
					else
					{
						gObj[aIndex].pInventory[blank].m_Number = gGetItemNumber();
						gPlusItemNumber();
					}
					gObjInventoryItemSet(aIndex, blank, (BYTE)gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc :
	
	return 0xFF;
}

BYTE gObjShopBuyInventoryInsertItemSerial(int aIndex, CItem item)
{
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;


	if( item.GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);		
		return 0xFF;
	}

	for( h=0; h<8; h++)
	{
		for( w=0; w<8; w++)				
		{
			// ����ִ��� üũ
			if( *(gObj[aIndex].pInventoryMap+(h*8)+w) == 0xFF )
			{
#ifdef PERSONAL_SHOP_20040113			// NPC �������� ������ �춧 �κ����� �ȿ��� ������ ����Ѵ�.
				blank = gObjOnlyInventoryRectCheck(aIndex, w, h, iwidth,iheight);
#else
				blank = gObjInventoryRectCheck(aIndex, w, h, iwidth,iheight);
#endif

				// 
				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	{	// �κ��丮�� �ڸ��� �ִٸ�
					ItemSerialCreateSend(aIndex,0,0,0,item.m_Type,item.m_Level,
						item.m_Durability,item.m_Option1,item.m_Option2,item.m_Option3,-1,
						item.m_NewOption,item.m_SetOption, TRUE);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc :

	return 0xFF;
}

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
BYTE gObjShopBuyInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level, int iSerial, int iDur)
{
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;
	CItem item;
	int item_type;

	item_type = ItemGetNumberMake(type, index );

	item.m_Level = level;
	item.m_Durability = iDur;
	item.Convert( item_type, 0 );

	if( item.GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);		
		return 0xFF;
	}

	for( h=0; h<8; h++)
	{
		for( w=0; w<8; w++)				
		{
			// ����ִ��� üũ
			if( *(lpObj->pInventoryMap+(h*8)+w) == 0xFF )
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index, w, h, iwidth,iheight);

				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	{	// �κ��丮�� �ڸ��� �ִٸ�
					lpObj->pInventory[blank] = item;
					
					if( item.m_serial == 0)  
					{
						lpObj->pInventory[blank].m_Number = 0;
					}
					else
					{
						lpObj->pInventory[blank].m_Number = gGetItemNumber();
						gPlusItemNumber();
					}
					gObjInventoryItemSet(lpObj->m_Index, blank, (BYTE)lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc :
	
	return 0xFF;
}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

// �κ��丮�� �������� �߰��Ҽ� �ִ���.. �߰��Ҽ� ������ ��ġ�� ����..
BYTE gObjInventoryInsertItemTemp(LPOBJECTSTRUCT lpObj, CMapItem *Item)
{
	CItem item;
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;
	
	if( Item->GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);
		return 0xFF;
	}

	for( h=0; h<8; h++)		
	{		
		for( w=0; w<8; w++)
		{
			// ����ִ��� üũ
			if( *(lpObj->pInventoryMap+(h*8)+w) == 0xFF )
			{				
				blank = gObjInventoryRectCheck(lpObj->m_Index, w, h, iwidth,iheight);
				// 
				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	{	// �κ��丮�� �ڸ��� �ִٸ�					
					return blank;
				}
			}
		}
	}

GOTO_EndFunc :
	return 0xFF;
}

// Ÿ�԰� �ε���, ������ �־� �κ��丮�� �������� �߰��Ѵ�.
BYTE gObjInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level)
{
	CItem item;
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;
	int item_type;

	//item_type = type<<4;
	//item_type += index;
	item_type    = ItemGetNumberMake(type, index );

	item.m_Level = level;
	item.Convert( item_type, 0);	
	
	if( item.GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);
		return 0xFF;
	}

	for( h=0; h<8; h++)		
	{		
		for( w=0; w<8; w++)
		{
			// ����ִ��� üũ
			if( *(lpObj->pInventoryMap+(h*8)+w) == 0xFF )
			{				
				blank = gObjInventoryRectCheck(lpObj->m_Index, w, h, iwidth,iheight);
				// 
				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	{	// �κ��丮�� �ڸ��� �ִٸ�
					lpObj->pInventory[blank] = item;
					lpObj->pInventory[blank].m_Number = gGetItemNumber();
					gPlusItemNumber();
					gObjInventoryItemSet(lpObj->m_Index, blank, (BYTE)lpObj->pInventory[blank].m_Type);
					return blank;
				}			
			}
		}
	}

GOTO_EndFunc :
	return 0xFF;
}


//#ifdef EVENT_LOTTO
// Ÿ�԰� �ε���, ������ �־� �κ��丮�� �������� �߰��Ѵ�.
BYTE gObjInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level, int iSerial, int iDur)
{
	CItem item;
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;
	int item_type;

	//item_type = type<<4;
	//item_type += index;
	item_type    = ItemGetNumberMake(type, index );

	item.m_Level = level;
	item.m_Durability = iDur;
	//item.Convert( item_type, 0);	
	item.Convert(item_type, 0, 0, 0, 0, 0, 0);
	
	if( item.GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);
		return 0xFF;
	}

	for( h=0; h<8; h++)		
	{		
		for( w=0; w<8; w++)
		{
			// ����ִ��� üũ
			if( *(lpObj->pInventoryMap+(h*8)+w) == 0xFF )
			{				
				blank = gObjInventoryRectCheck(lpObj->m_Index, w, h, iwidth,iheight);
				// 
				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	{	// �κ��丮�� �ڸ��� �ִٸ�
					lpObj->pInventory[blank] = item;
					lpObj->pInventory[blank].m_Number = iSerial;
					gPlusItemNumber();
					gObjInventoryItemSet(lpObj->m_Index, blank, (BYTE)lpObj->pInventory[blank].m_Type);
					return blank;
				}			
			}
		}
	}

GOTO_EndFunc :
	return 0xFF;
}
//#endif // EVENT_LOTTO

// Ÿ�԰� �ε���, ������ �־� ������ �κ��丮�� �������� �߰��Ѵ�.
BYTE gObjMonsterInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level, int op1, int op2, int op3)
{
	//g_MonsterItemMng.InsertItem(lpObj->Level, type, index, level, op1, op2, op3);
	
	CItem item;
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;
	int item_type;
	int a=0;
	
	
	//item_type = type<<4;
	//item_type += index;
	item_type    = ItemGetNumberMake(type, index );

	item.m_Level = level;

	if( type == 13 && index == 10 )
	{
		item.m_Durability = (float)100+(rand()%100);
	}

	item.Convert( item_type, op1, op2, op3);	

	lpObj->pInventory[blank] = item;	
	iwidth = 1;
	iheight = 1;

	for( h=0; h<8; h++)
	{
		for( w=0; w<8; w++)
		{
			// ����ִ��� üũ
			if( *(lpObj->pInventoryMap+(h*8)+w) == 0xFF )
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index, w, h, iwidth,iheight);

				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	
				{	// �κ��丮�� �ڸ��� �ִٸ�
					lpObj->pInventory[blank] = item;
					
					*lpObj->pInventoryCount+=1;
					
					gObjInventoryItemBoxSet(lpObj->m_Index, blank, iwidth, iwidth, (BYTE)lpObj->pInventory[blank].m_Type);

					return blank;
				}			

			}
		}
	}

GOTO_EndFunc :

	return 0xFF;
}


// � ������ �κ��丮�� ��� ���� �������� �ϴµ� ����ִ��� üũ
BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;

	// ���μ��� ũ�⸦ �ʰ� �ߴٸ� 
#ifdef PERSONAL_SHOP_20040113		// ���μ��� ũ�⸦ ���� �� �߸��ϸ� �κ��� ���λ����� ������ ��ĥ �� �ִ�.
	if( (sx+width) > 8 ) return (BYTE)0xFF;
	if( (sy+height) > 12 ) return (BYTE)0xFE;
#else
	if( (sx+width) > 8 ) return (BYTE)0xFF;
	if( (sy+height) > 8 ) return (BYTE)0xFE;
#endif

	int xx, yy;

	for( y=0; y<height; y++)
	{		
		yy = sy+y;
		for(x=0; x<width; x++)
		{
			xx = sx+x;

#ifdef PERSONAL_SHOP_20040113
			if( ExtentCheck(xx, yy, 8, 12) == TRUE )
#else
			if( ExtentCheck(xx, yy, 8, 8) == TRUE )
#endif
			{
				if( *(gObj[aIndex].pInventoryMap+((sy+y)*8)+(sx+x)) != 0xFF ) 
				{
					blank++;
					return (BYTE)0xFF;
				}
			}
			else 
			{
				LogAdd("error : %s %d",__FILE__, __LINE__);
				return (BYTE)0xFF;
			}
		}
	}

	if( blank == 0 ) return (BYTE)(MAX_EQUIPMENT+((sy*8)+sx));

	return (BYTE)0xFF;
}


#ifdef PERSONAL_SHOP_20040113

// � ������ ���� �κ��丮 (���λ���â ����) �� ��� ���� �������� �ϴµ� ����ִ��� üũ
BYTE gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;

	// ���μ��� ũ�⸦ �ʰ� �ߴٸ� 
	if( (sx+width) > 8 ) return (BYTE)0xFF;
	if( (sy+height) > 8 ) return (BYTE)0xFE;

	int xx, yy;

	for( y=0; y<height; y++)
	{		
		yy = sy+y;
		for(x=0; x<width; x++)
		{
			xx = sx+x;

			if( ExtentCheck(xx, yy, 8, 8) == TRUE )
			{
				if( *(gObj[aIndex].pInventoryMap+((sy+y)*8)+(sx+x)) != 0xFF ) 
				{
					blank++;
					return (BYTE)0xFF;
				}
			}
			else 
			{
				LogAdd("error : %s %d",__FILE__, __LINE__);
				return (BYTE)0xFF;
			}
		}
	}

	if( blank == 0 ) return (BYTE)(MAX_EQUIPMENT+((sy*8)+sx));

	return (BYTE)0xFF;
}


#endif

//#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
BOOL CheckInventoryEmptySpace(LPOBJECTSTRUCT lpObj, int iItemHeight, int iItemWidth)
{
	int h = 0;
	int w = 0;
	BYTE blank = 0;

	for( h=0; h<8; h++)
	{		
		for( w=0; w<8; w++)	
		{
			// ����ִ��� üũ
			if( *(lpObj->pInventoryMap+(h*8)+w) == 0xFF )
			{
#ifdef PERSONAL_SHOP_20040113
				blank = gObjOnlyInventoryRectCheck( lpObj->m_Index, w, h, iItemWidth, iItemHeight);
#else
				blank = gObjInventoryRectCheck( lpObj->m_Index, w, h, iItemWidth, iItemHeight);
#endif

				if( blank == 0xFE )
				{
					return FALSE;
				}

				if( blank != 0xFF )	// �κ��丮�� �ڸ��� �ִٸ�
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}
//#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
//CheckInventoryEmptySpace �� ������ �Լ��� cashshop�� ����ϴ� ��츸 ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
//�� ����ϴ� ����� ������ �Լ��� �ϳ��� ���� ����� 
//�������� �����Ǹ� CheckInventoryEmptySpace ����� ����
//2008/03/31 riverstyx 
BOOL CheckInventoryEmptySpace_(LPOBJECTSTRUCT lpObj, int iItemHeight, int iItemWidth)
{
	int h = 0;
	int w = 0;
	BYTE blank = 0;
	
	for( h=0; h<8; h++)
	{		
		for( w=0; w<8; w++)	
		{
			// ����ִ��� üũ
			if( *(lpObj->pInventoryMap+(h*8)+w) == 0xFF )
			{
				blank = gObjOnlyInventoryRectCheck( lpObj->m_Index, w, h, iItemWidth, iItemHeight);
				// 
				if( blank == 0xFE )
				{
					return FALSE;
				}
				
				if( blank != 0xFF )	// �κ��丮�� �ڸ��� �ִٸ�
				{
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

// �������� �����Ҽ� �ִ��� üũ�Ѵ�.
BOOL gObjIsItemPut(LPOBJECTSTRUCT lpObj, CItem *lpItem, int pos)
{
	// ���� �����Ϸ��� �������� ��հ��̸� �ϳ��� ��� �ִ��� üũ
	if( lpItem->m_TwoHand == TRUE )
	{		
		if( pos == EQUIPMENT_WEAPON_RIGHT ||
			pos == EQUIPMENT_WEAPON_LEFT )
		{
			if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].IsItem() == TRUE )
			{
				if( (lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(4,7)) ||
					(lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(4,15)) )
				{					
				}	
				else return FALSE;
			}
			if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].IsItem() == TRUE )
			{
				if( (lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type == MAKE_ITEMNUM(4,7)) ||
					(lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type == MAKE_ITEMNUM(4,15)) )
				{					
				}	
				else return FALSE;
			}			
		}
	}
	
	if( pos == EQUIPMENT_WEAPON_LEFT )
	{
		// �����տ� �� ���Ⱑ �μ� ������ �޼տ� ���� ����.
		if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].IsItem() == TRUE )
		{		
			// Ȱ������� �����Ҽ� �ִ�.
			if( (lpItem->m_Type == MAKE_ITEMNUM(4,7)) ||
				(lpItem->m_Type == MAKE_ITEMNUM(4,15)) )
			{					
			}
			else if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_TwoHand )
			{
				return FALSE;
			}
		}
	}

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ������ �����۵�
	// ��ũ�ε��̰� �б�� �ϰ�� �Ѽո� ����
	if( lpObj->Class == CLASS_DARKLORD 
		&& (lpItem->m_Type >> ITEM_BITSHIFT) == ITEM_MACE / MAX_ITEM_INDEX
		)
	{
		if( lpItem->m_Part != pos ) 
			return FALSE;
	}

	// �����̰� ������ �ϰ��� �Ѽո� ����
	if( lpObj->Class == CLASS_WIZARD
		&& (lpItem->m_Type >> ITEM_BITSHIFT) == ITEM_STAFF / MAX_ITEM_INDEX
		)
	{
		if( lpItem->m_Part != pos ) 
			return FALSE;
	}

	// ��ȯ���� �ϰ�� �������� ��ƽ �޼��� ����( ��ƽ�� ������ �����̷��� ����)
	if( lpObj->Class == CLASS_SUMMONER
		&& (lpItem->m_Type >> ITEM_BITSHIFT) == ITEM_STAFF / MAX_ITEM_INDEX
		)
	{
		if( lpItem->m_Part != pos ) 
			return FALSE;
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	// ��� ��ġ�� �ڽ��� ��ġ�� �ƴ϶��..
	if( lpItem->m_Part != pos ) 
	{
		if( pos == EQUIPMENT_WEAPON_LEFT ||
			pos == EQUIPMENT_WEAPON_RIGHT )
		{
			if( lpItem->m_Part == EQUIPMENT_WEAPON_LEFT ||
				lpItem->m_Part == EQUIPMENT_WEAPON_RIGHT )
			{
				
			}	
			else return FALSE;
		}
		else if( pos == EQUIPMENT_RING_LEFT )
		{
			if( lpItem->m_Part == EQUIPMENT_RING_RIGHT )
			{
			}
			else {
				return FALSE;
			}
		}
		else return FALSE;
	}
	
	// �� �䱸 �ɷ�ġ �˻�

	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		if( lpObj->Strength+lpObj->AddStrength < lpItem->m_RequireStrength - lpItem->m_HJOpStrength ) 
			return FALSE;
		if( lpObj->Dexterity+lpObj->AddDexterity < lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity ) 
			return FALSE;
	#else
		if( lpObj->Strength+lpObj->AddStrength < lpItem->m_RequireStrength ) return FALSE;			// �䱸 ��
		if( lpObj->Dexterity+lpObj->AddDexterity < lpItem->m_RequireDexterity) return FALSE;		// �䱸 ��ø
	#endif	 // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

	if( lpObj->Level < lpItem->m_RequireLevel) return FALSE;									// �䱸 ����


#ifdef EXPAND_ITEMSCRIPT_01_20050808
	if( lpItem->m_Type >= ITEM_SWORD && lpItem->m_Type < ITEM_WING )
	{
		if( lpObj->Vitality + lpObj->AddVitality< lpItem->m_RequireVitality	) return FALSE;		// �䱸 ����
		if( lpObj->Energy   + lpObj->AddEnergy	< lpItem->m_RequireEnergy	) return FALSE;		// �䱸 ������
	}
#endif

#ifdef DARKLORD_WORK
	if( (lpObj->Leadership+lpObj->AddLeadership) < lpItem->m_RequireLeaderShip ) return FALSE;	// �䱸 ���
#endif
	
	if( lpItem->m_Type >=  MAKE_ITEMNUM(12,0) && lpItem->m_Type <=  MAKE_ITEMNUM(12,6))	
	{	// ������ ������ üũ
		if( lpObj->Level < lpItem->m_RequireLevel ) return FALSE;
	}
	
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1������(����� ����) �䱸����üũ
	if( lpItem->m_Type ==  MAKE_ITEMNUM(12, 41))	
	{	// ������ ������ üũ
		if( lpObj->Level < lpItem->m_RequireLevel ) return FALSE;
	}
#endif

#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ���� ������ ���� �䱸���� üũ
	if( lpItem->m_Type ==  MAKE_ITEMNUM(12, 42)
		|| lpItem->m_Type ==  MAKE_ITEMNUM(12, 43))	
	{	// ������ ������ üũ
		if( lpObj->Level < lpItem->m_RequireLevel ) return FALSE;
	}
#endif

	// �׸���  "��ȣõ��" "��ź" "���ϸ����� ��" "������ ����" "���� ����" "������ �����" "���� �����"
	// �� ���� ������ Ȯ���Ѵ�.
	else if( lpItem->m_Type >= MAKE_ITEMNUM(13,0) && lpItem->m_Type <= MAKE_ITEMNUM(13,15))
	{
		if( lpItem->m_Type == MAKE_ITEMNUM(13,10) )	// ���� ������ ����(����)�� ���� ���� ������ ��������.
		{
			if( lpItem->m_Level  <= 2 )
			{
				if( lpObj->Level < 20 ) return FALSE;
			}
			else if( lpObj->Level < 50 ) return FALSE;
		}		
		else if( lpItem->m_Type == MAKE_ITEMNUM(13,2) )	// ���ϸ����� ��� ��Ʋ�������� ���� ���ϰ� �Ѵ�
		{
			if( lpObj->MapNumber == 7 || lpObj->Level < lpItem->m_RequireLevel )
			{
				return FALSE;
			}
		}
#ifdef NEW_SKILL_FORSKYLAND
		else if( lpItem->m_Type == MAKE_ITEMNUM(13,3) )	// ����Ʈ�� ��� ��Ʋ�������� ���� ���ϰ� �Ѵ�
		{
			if( lpObj->MapNumber == 7 || lpObj->Level < lpItem->m_RequireLevel )
			{
				return FALSE;
			}
		}
#endif
		else 
		{
			if( lpObj->Level < lpItem->m_RequireLevel ) return FALSE;
		}
	}

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��������� ���Ź����� ������ �� ����
	if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
	{
		if(g_IllusionTempleEvent.CheckChangeRing(lpItem->m_Type))
		{
			return FALSE;
		}
	}
#endif

#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - �������� ���� ���� �� �̹� ������ ���� �ִ��� Ȯ��
	if( lpItem->m_Type == MAKE_ITEMNUM(13,20) && lpItem->m_Level == 0) {
		int itemcount = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 20, 0);

#ifdef ADD_CRITICAL_WIZARD_RING_20090915	// ġ�� ������ ������ �ߺ� ��� �Ұ�
		itemcount += gObjGetItemCountInEquipment(lpObj->m_Index, 13, 107, 0);
#endif // ADD_CRITICAL_WIZARD_RING_20090915

		if (itemcount)
			return FALSE;
	}
#endif

#ifdef ADD_CRITICAL_WIZARD_RING_20090915	// ġ�� ������ ���� - �������� ���� ���� �� �̹� ������ ���� �ִ��� Ȯ��
	if( lpItem->m_Type == MAKE_ITEMNUM(13, 107) ) {

		// ������ ���� �Ǵ� ġ�� ������ ������ �ߺ� ��� �Ұ�
		int itemcount = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 20, 0);
		itemcount += gObjGetItemCountInEquipment(lpObj->m_Index, 13, 107, 0);
		
		if (itemcount)
			return FALSE;
	}
#endif // ADD_CRITICAL_WIZARD_RING_20090915
	
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
	if( lpItem->m_Type == MAKE_ITEMNUM( 13, 40 ) )
	{
		int iItemCount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 40, 0 );

		if( iItemCount > 0 )
		{
			return FALSE;
		}
	}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	// ��Ÿũ�ν� ���Ź����� 1���̻� ������� �ʴ´�.
	if( lpItem->m_Type == MAKE_ITEMNUM( 13, 41 ) )
	{
		int iItemCount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 41, 0 );

		if( iItemCount > 0 )
		{
			return FALSE;
		}
	}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	
#ifdef UPDATE_GM_FUNCTION_20070228	// GM ���Ź����� īĳ���� �������, 1�� �̻� �������
	if( lpItem->m_Type == MAKE_ITEMNUM( 13, 42 ) )
	{
		if( CHECK_CHAOSCASTLE(lpObj->MapNumber) )
		{
			return FALSE;
		}

		int iItemCount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 42, 0 );

		if( iItemCount > 0 )
		{
			return FALSE;
		}
	}
#endif

#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// �߱� �̺�Ʈ - ������ ���� 
	if( lpItem->m_Type == MAKE_ITEMNUM(13,20) && lpItem->m_Level == 3) {
		int itemcount = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 20, 3);

		if (itemcount)
			return FALSE;
	}
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī����ĳ�������� ���Ź����� ������ �� ����.
	if( lpItem->m_Type == MAKE_ITEMNUM(13, 10) )
	{
		if( CHECK_CHAOSCASTLE(lpObj->MapNumber) )
		{
			return FALSE;
		}
	}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110		// �渱�� ī����ĳ������ ������ �� ����.
	if( lpItem->m_Type == MAKE_ITEMNUM(13, 37) )
	{
		if( CHECK_CHAOSCASTLE(lpObj->MapNumber) )
		{
			return FALSE;
		}		
	}
#endif // ADD_ITEM_FENRIR_01_20051110
	
#ifdef KANTURU_MOONSTONEPENDANT_20060804	// ���������Ʈ ���� �� �̹� ������ ���� �ִ��� Ȯ�� 20060823�߰�
	if( lpItem->m_Type == MAKE_ITEMNUM( 13,38 ) )
	{
		int itemcount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 38, 0 );

		if( itemcount )
			return FALSE;
	}
#endif

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// īĳ���� ������� & �� �� ���� ����
	if( lpItem->m_Type == MAKE_ITEMNUM( 13, 39 ) )
	{
		if( CHECK_CHAOSCASTLE(lpObj->MapNumber) )
		{
			return FALSE;
		}

		// �Ϲ� ���Ź���
		int itemcount = 0;
		for( int iLevelCount = 0; iLevelCount < 7; iLevelCount++ )
		{		
			itemcount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 10, iLevelCount );
			if( itemcount )
				return FALSE;
		}

		// ����Ʈ�ذ����纯�Ź���
		itemcount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 39, 0 );
		if( itemcount )
			return FALSE;
	}

	if( lpItem->m_Type == MAKE_ITEMNUM( 13, 10 ) )
	{
		// �Ϲ� ���Ź���
		int itemcount = 0;
		for( int iLevelCount = 0; iLevelCount < 7; iLevelCount++ )
		{
			itemcount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 10, iLevelCount );
			if( itemcount )
				return FALSE;
		}

		// ����Ʈ�ذ����纯�Ź���
		itemcount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 39, 0 );
		if( itemcount )
			return FALSE;		
	}
#endif	// ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830

#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
	if( lpItem->m_Type == MAKE_ITEMNUM( 13, 40 ) )
	{
		if( CHECK_CHAOSCASTLE( lpObj->MapNumber ) )
		{
			return FALSE;
		}

		int iItemCount = 0;

		iItemCount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 40, 0 );
		if( iItemCount > 0 )
		{
			return FALSE;
		}
	}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	if( lpItem->m_Type == MAKE_ITEMNUM( 13, 41 ) )
	{
		if( CHECK_CHAOSCASTLE( lpObj->MapNumber ) )
		{
			return FALSE;
		}

		int iItemCount = 0;

		iItemCount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 41, 0 );
		if( iItemCount > 0 )
		{
			return FALSE;
		}
	}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
	// ��Ÿũ�ν� ���Ź����� 1���̻� ������� �ʴ´�.
	if( lpItem->m_Type == MAKE_ITEMNUM( 13, 68 ) )
	{
		if( CHECK_CHAOSCASTLE( lpObj->MapNumber ) )
		{
			return FALSE;
		}
		
		int iItemCount = 0;
		
		iItemCount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 68, 0 );
		if( iItemCount > 0 )
		{
			return FALSE;
		}
	}
#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------
#ifdef ADD_PREMIUMITEM_PANDA
	if( lpItem->m_Type == MAKE_ITEMNUM( 13, 76 ) )
	{
		if( CHECK_CHAOSCASTLE( lpObj->MapNumber ) )
		{
			return FALSE;
		}
		
		int iItemCount = 0;
		
		iItemCount = gObjGetItemCountInEquipment( lpObj->m_Index, 13, 76, 0 );
		if( iItemCount > 0 )
		{
			return FALSE;
		}
	}
#endif // ADD_PREMIUMITEM_PANDA

	// �������� ���� �� �� ����.
#ifdef ITEM_INDEX_EXTEND_20050706	// ������ Ÿ�� 14���� �̻��� ������ �� ���� �͵��̴�.
	else if( lpItem->m_Type >= MAKE_ITEMNUM(14,0) ) return FALSE;
#else
	else if( lpItem->m_Type > (14<<8) ) return FALSE;
#endif

	return TRUE;
}


// � ������ â�� �������� ���� �������� �ϴµ� ����ִ��� üũ
BYTE gObjWerehouseRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;
	
	// ���μ��� ũ�⸦ �ʰ� �ߴٸ� 
	if( (sx+width) > 8 ) return (BYTE)0xFF;
	if( (sx+width) < 0 ) return (BYTE)0xFF;
	if( (sy+height) > 15 ) return (BYTE)0xFF;
	if( (sy+height) < 0  ) return (BYTE)0xFF;
	int xx, yy;

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	for( y=0; y<height; y++)
	{		
		yy = sy+y;
		for(x=0; x<width; x++)
		{
			xx = sx+x;
			if( ExtentCheck(xx, yy, 8, 15) == TRUE )
			{
				if( *(gObj[aIndex].pWarehouseMap+((sy+y)*8)+(sx+x)) != 0xFF ) 
				{
					blank++;
					return (BYTE)0xFF;
				}
			}
			else 
			{
				LogAdd("error : %s %d",__FILE__, __LINE__);
				return (BYTE)0xFF;
			}
		}
	}

	if( blank == 0 ) return (BYTE)(((sy*8)+sx));

	return (BYTE)0xFF;
}


// � ������ â�� �������� ���� �������� �ϴµ� ����ִ��� üũ
BYTE gObjMapRectCheck(LPBYTE lpMapBuf, int sx, int sy, int ex, int ey, int width, int height)
{
	int x, y;
	int blank = 0;
	
	// ���μ��� ũ�⸦ �ʰ� �ߴٸ� 
	if( ((sx+width) > ex) || ((sx+width) < 0)) return (BYTE)0xFF;
	if( ((sy+height) > ey) || ((sy+height) < 0)) return (BYTE)0xFF;
	
	int xx, yy;
	
	for( y=0; y<height; y++)
	{		
		yy = sy+y;
		for(x=0; x<width; x++)
		{
			xx = sx+x;
			if( ExtentCheck(xx, yy, ex, ey) == TRUE )
			{
				if( *(lpMapBuf+((sy+y)*ex)+(sx+x)) != 0xFF ) 
				{
					blank++;
					return (BYTE)0xFF;
				}
			}
			else 
			{
				LogAdd("error : %s %d",__FILE__, __LINE__);
				return (BYTE)0xFF;
			}
		}
	}

	if( blank == 0 ) return (BYTE)(((sy*ex)+sx));

	return (BYTE)0xFF;
}


void gObjWarehouseItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx, itemposy;
	int xx, yy;

	itemposx = itempos%8;
	itemposy = itempos/8;

	for( int y=0; y<yl; y++)
	{
		yy = itemposy+y;
		for( int x=0; x<xl; x++)
		{
			xx = itemposx+x;
			if( ExtentCheck(xx, yy, 8, 15) == TRUE )
			{
				*(gObj[aIndex].pWarehouseMap+((itemposy+y)*8)+(itemposx+x)) = set_byte;
			}
			else {
				LogAdd("error : %d/%d  %s %d",xx,yy,__FILE__, __LINE__);
				return;
			}
		}
	}
}



// â�� �������� ũ�⸸ŭ �ٸ������� �ִ´�.
void gObjWarehouseItemSet(int aIndex, int itempos, BYTE set_byte)
{
	if( itempos < 0 ) return;
	if( itempos > MAX_WAREHOUSEITEMS-1 ) return;
	
	int width, height;

	if( gObj[aIndex].pWarehouse[itempos].GetSize( width, height ) == FALSE ) 
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);
		return;
	}
	gObjWarehouseItemBoxSet(aIndex, itempos, width, height, set_byte);	
}


void gObjChaosItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx, itemposy;
	int xx, yy;

	itemposx = itempos%8;
	itemposy = itempos/8;

	for( int y=0; y<yl; y++)
	{
		yy = itemposy+y;
		for( int x=0; x<xl; x++)
		{
			xx = itemposx+x;
			if( ExtentCheck(xx, yy, 8, 4) == TRUE )
			{
				*(gObj[aIndex].pChaosBoxMap+((itemposy+y)*8)+(itemposx+x)) = set_byte;
			}
			else {
				LogAdd("error : %d/%d  %s %d",xx,yy,__FILE__, __LINE__);
				return;
			}
		}
	}
}



// �������� ũ�⸸ŭ �ٸ������� �ִ´�.
void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte)
{
	if( itempos < 0 ) return;
	if( itempos > MAX_CHAOSBOXITEMS-1 ) return;
	
	int width, height;

	if( gObj[aIndex].pChaosBox[itempos].GetSize( width, height ) == FALSE ) 
	{
		LogAdd(lMsg.Get(527), __FILE__, __LINE__);
		return;
	}
	gObjChaosItemBoxSet(aIndex, itempos, width, height, set_byte);	
}


BYTE gObjChaosBoxInsertItemPos(int aIndex, CItem item, int pos, int source)
{
	if( pos < 0 || pos > MAX_CHAOSBOXITEMS-1 )
	{
		return 0xFF;
	}
	
	LPOBJECTSTRUCT	lpObj;

	BYTE			TempMap[MAX_CHAOSBOXITEMS];				// �κ��丮 �ӽ� ����
	int				w, h, iwidth, iheight;
	int				blank;
	BOOL			useClass=FALSE;

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	// �ű� ��ġ�� �������� �����ϴ°�?
	if( lpObj->pChaosBox[pos].IsItem() == TRUE ) return 0xFF;

	// �ű� �������� �����ϴ°�?
	if( item.IsItem() == FALSE ) return 0xFF;
	
	w = pos%8;
	h = pos/8;
		
	if( ExtentCheck(w, h, 8, 4) == FALSE) return 0xFF;
	item.GetSize(iwidth, iheight);

	if( source >= 0 )
	{
		memcpy(TempMap, lpObj->pChaosBoxMap, MAX_CHAOSBOXITEMS);
		
		gObjChaosItemBoxSet(lpObj->m_Index, source, iwidth, iheight, 0xFF);
	}
	// ��������ʴٸ�.. ����..
	if( *(lpObj->pChaosBoxMap+(h*8)+w) != 0xFF ) 
	{
		if( source >= 0 )
		{
			memcpy(lpObj->pChaosBoxMap, TempMap, MAX_CHAOSBOXITEMS);
		}
		return 0xFF;
	}

	blank = gObjMapRectCheck(lpObj->pChaosBoxMap, w, h, 8, 4, iwidth, iheight);
	//	gObjWerehouseRectCheck(lpObj->m_Index, w, h, iwidth, iheight);
	
	if( blank == 0xFF )
	{
		if( source >= 0 )
		{
			memcpy(lpObj->pChaosBoxMap, TempMap, MAX_CHAOSBOXITEMS);
		}
		return 0xFF;
	}
	lpObj->pChaosBox[pos] = item;
	return pos;
}


// â���� Ư�� ��ġ�� �������� �߰��Ѵ�.
BYTE gObjWarehouseInsertItemPos(int aIndex, CItem item, int pos, int source)
{
	if( pos < 0 || pos > MAX_WAREHOUSEITEMS-1 )
	{
		return 0xFF;
	}
	
	LPOBJECTSTRUCT	lpObj;

	BYTE			TempMap[MAX_WAREHOUSEITEMS];				// �κ��丮 �ӽ� ����
	int				w, h, iwidth, iheight;
	int				blank;
	BOOL			useClass=FALSE;

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	// �ű� ��ġ�� �������� �����ϴ°�?
	if( lpObj->pWarehouse[pos].IsItem() == TRUE ) return 0xFF;

	// �ű� �������� �����ϴ°�?
	if( item.IsItem() == FALSE ) return 0xFF;
	
	w = pos%8;
	h = pos/8;
		
	if( ExtentCheck(w, h, 8, 15) == FALSE) return 0xFF;
	item.GetSize(iwidth, iheight);

	if( source >= 0 )
	{
		memcpy(TempMap, lpObj->pWarehouseMap, MAX_WAREHOUSEITEMS);
		gObjWarehouseItemBoxSet(lpObj->m_Index, source, iwidth, iheight, 0xFF);
	}
	// ��������ʴٸ�.. ����..
	if( *(lpObj->pWarehouseMap+(h*8)+w) != 0xFF ) 
	{
		if( source >= 0 )
		{
			memcpy(lpObj->pWarehouseMap, TempMap, MAX_WAREHOUSEITEMS);
		}
		return 0xFF;
	}

	blank = gObjWerehouseRectCheck(lpObj->m_Index, w, h, iwidth, iheight);
	
	if( blank == 0xFF )
	{
		if( source >= 0 )
		{
			memcpy(lpObj->pWarehouseMap, TempMap, MAX_WAREHOUSEITEMS);
		}
		return 0xFF;
	}
	lpObj->pWarehouse[pos] = item;
	return pos;
}

// Ư�� ��ġ�� �������� �߰��Ѵ�.
BYTE gObjInventoryInsertItemPos(int aIndex, CItem item, int pos, int RequestCheck)
{
#ifdef PERSONAL_SHOP_20040113	// Ư�� ��ġ�� ������ �߰� -> DbItemSetInByte() ���� ȣ���ϹǷ� �ʿ�
	if( pos < 0 || pos > MAX_INVENTORY_EXTEND )
#else
	if( pos < 0 || pos > MAX_INVENTORY )
#endif
	{
		return 0xFF;
	}
	LPOBJECTSTRUCT	lpObj;

#ifdef PERSONAL_SHOP_20040113
	#ifdef PERSONAL_SHOP_BUG_FIX
		BYTE			TempInventoryMap[MAX_INVENTORYMAP + MAX_PSHOPITEM];		// �κ��丮 �ӽ� ����
	#else
		BYTE			TempInventoryMap[MAX_INVENTORY + MAX_PSHOPITEM];		// �κ��丮 �ӽ� ����
	#endif
#else
	BYTE			TempInventoryMap[8*8];				// �κ��丮 �ӽ� ����
#endif

	int				w, h, iwidth, iheight;
	int				blank;
	BOOL			useClass=FALSE;

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	// �ű� ��ġ�� �������� �����ϴ°�?
	if( lpObj->pInventory[pos].IsItem() == TRUE ) return 0xFF;

	// �ű� �������� �����ϴ°�?
	if( item.IsItem() == FALSE ) return 0xFF;

	// ����� ��� ���� �̶��
	if( pos < MAX_EQUIPMENT )
	{
#ifdef ADD_THIRD_WING_20070525	// IsClass ����
		useClass = item.IsClass( lpObj->Class, lpObj->ChangeUP, lpObj->ThirdChangeUp );	
#else
#ifdef NEW_SKILL_FORSKYLAND
		useClass = item.IsClass(lpObj->Class, lpObj->ChangeUP);	
#else
		useClass = item.IsClass(lpObj->Class);	
#endif
#endif	// ADD_THIRD_WING_20070525

		
		// �����Ϸ��� �Ҷ� �ڽ��� ��� �ƴҰ��		
		if( useClass == FALSE )	
		{
			LogAdd(lMsg.Get(528));
			return 0xFF;
		}

		if( RequestCheck )
		{
			if( gObjIsItemPut(lpObj, &item, pos) == FALSE ) return 0xFF;
		}		
	}
#ifdef PERSONAL_SHOP_20040113
	else
	{
		w = (pos-MAX_EQUIPMENT)%8;
		h = (pos-MAX_EQUIPMENT)/8;
		
		if( ExtentCheck(w, h, 8, 12) == FALSE) return 0xFF;

#ifdef PERSONAL_SHOP_BUG_FIX
		memcpy(TempInventoryMap, lpObj->pInventoryMap, MAX_INVENTORYMAP + MAX_PSHOPITEM);
#else
		memcpy(TempInventoryMap, lpObj->pInventoryMap, MAX_INVENTORY + MAX_PSHOPITEM);
#endif
		
		item.GetSize(iwidth, iheight);
		//gObjInventoryItemBoxSet(lpObj->m_Index, source, iwidth, iheight, 0xFF);	
		
		// ��������ʴٸ�.. ����..
		if( *(lpObj->pInventoryMap+(h*8)+w) != 0xFF ) {
#ifdef PERSONAL_SHOP_BUG_FIX
			memcpy(lpObj->pInventoryMap, TempInventoryMap, MAX_INVENTORYMAP + MAX_PSHOPITEM);
#else
			memcpy(lpObj->pInventoryMap, TempInventoryMap, MAX_INVENTORY + MAX_PSHOPITEM);
#endif
			return 0xFF;
		}
		
		blank = gObjInventoryRectCheck(lpObj->m_Index, w, h, iwidth,iheight);
		
		if( blank >= 0xFE )
		{
#ifdef PERSONAL_SHOP_BUG_FIX
			memcpy(lpObj->pInventoryMap, TempInventoryMap, MAX_INVENTORYMAP + MAX_PSHOPITEM);
#else
			memcpy(lpObj->pInventoryMap, TempInventoryMap, MAX_INVENTORY + MAX_PSHOPITEM);
#endif
			return 0x00;
		}
	}
#else
	else
	{
		w = (pos-MAX_EQUIPMENT)%8;
		h = (pos-MAX_EQUIPMENT)/8;
		
		if( ExtentCheck(w, h, 8, 8) == FALSE) return 0xFF;

		memcpy(TempInventoryMap, lpObj->pInventoryMap, 8*8);
		
		item.GetSize(iwidth, iheight);
		//gObjInventoryItemBoxSet(lpObj->m_Index, source, iwidth, iheight, 0xFF);	
		
		// ��������ʴٸ�.. ����..
		if( *(lpObj->pInventoryMap+(h*8)+w) != 0xFF ) {
			memcpy(lpObj->pInventoryMap, TempInventoryMap, 8*8);
			return 0xFF;
		}
		
		blank = gObjInventoryRectCheck(lpObj->m_Index, w, h, iwidth,iheight);
		
		if( blank >= 0xFE )
		{
			memcpy(lpObj->pInventoryMap, TempInventoryMap, 8*8);
			return 0x00;
		}
	}
#endif
		
	// ����ִٸ� ����
	lpObj->pInventory[pos] = item;

	// �������� �ʿ����� �������� �ִٰ� ä���.
	if( pos > MAX_EQUIPMENT-1 )
	{
		gObjInventoryItemSet(aIndex, pos, 1);
	}		
	
	// �����ۿ� ��ų�� ������ ��ų ����Ʈ�� �߰��Ѵ�.
	if( pos < MAX_EQUIPMENT )
	{
		CItem *lpItem;	
		lpItem = (CItem*)&lpObj->pInventory[pos];
		if( lpItem->m_Option1 )
		{
			int addskill = gObjWeaponMagicAdd(lpObj, lpItem->m_Special[0], (BYTE)lpItem->m_Level);
			if( addskill >= 0 ) {
				GCMagicListOneSend(lpObj->m_Index, addskill,lpItem->m_Special[0], (BYTE)lpItem->m_Level, 0);
			}
		}
	}

	/*
	// ��� ����Ǿ��ٸ� CharSet�� �ٽ� �����Ѵ�.
	if( pos < MAX_EQUIPMENT)
	{
		gObjMakePreviewCharSet(lpObj->m_Index);
	}
	
	if( pos < MAX_EQUIPMENT ) GCEquipmentChange(lpObj->m_Index, pos);
	*/
	return pos;
}

// ������Ʈ���� ���� ������ �ø��� ��ȣ�� �ִ��� �˻��Ѵ�.
BOOL gObjInventorySearchSerialNumber(LPOBJECTSTRUCT lpObj, DWORD serial)
{
	DWORD s_num;
	int   count=0;
	int   n;

	if(gItemSerialCheck==FALSE) return TRUE;

#if (TESTSERVER==1 && LOCALCONNECT==1)
	return TRUE;
#endif

	if( serial == 0 ) return TRUE;

#ifdef PERSONAL_SHOP_20040113		
	for( n=0; n<MAX_INVENTORY_EXTEND; n++)
#else
	for( n=0; n<MAX_INVENTORY; n++)
#endif
	{
		s_num = lpObj->pInventory[n].GetNumber();

		if( s_num != 0 && s_num == serial )	// 0�� �ƴϰ� �ø��� �ѹ��� ���� �������� �ִٸ�
		{			
			count++;
		}
	}
	
	if( count <= 1 ) return TRUE;
	
#ifdef PERSONAL_SHOP_20040113		
	for( n=0; n<MAX_INVENTORY_EXTEND; n++)
#else
	for( n=0; n<MAX_INVENTORY; n++)
#endif
	{
		s_num = lpObj->pInventory[n].GetNumber();
		if( s_num != 0 && s_num == serial )	// 0�� �ƴϰ� �ø��� �ѹ��� ���� �������� �ִٸ�
		{
			LogAddTD(lMsg.Get(529),lpObj->AccountID, lpObj->Name, lpObj->pInventory[n].GetName(), n, s_num);
//			LogAddC(LOGC_RED,lMsg.Get(529),lpObj->AccountID, lpObj->Name, lpObj->pInventory[n].GetName(), n, s_num);
			GCServerMsgStringSend(lMsg.Get(1102), lpObj->m_Index, 1);
			gObjUserKill(lpObj->m_Index);
			//gObjInventoryDeleteItem(lpObj->m_Index, n);
			/*ItemMovePathSave(lpObj->AccountID, lpObj->Name, lpObj->pInventory[n].m_Level, lpObj->MapNumber, 
						lpObj->X, lpObj->Y, lpObj->pInventory[n].GetName(), 
						lpObj->pInventory[n].m_Option1, 
						lpObj->pInventory[n].m_Option2,
						lpObj->pInventory[n].m_Option3, 
						lpObj->pInventory[n].m_serial);*/
		}
	}
	return FALSE;
}



// â�� ������Ʈ���� ���� ������ �ø��� ��ȣ�� �ִ��� �˻��Ѵ�.
BOOL gObjWarehouseSearchSerialNumber(LPOBJECTSTRUCT lpObj, DWORD sirial)
{
	DWORD s_num;
	int   count=0;
	int   n;

#if (TESTSERVER==1)
	#if (LOCALCONNECT==1)
		return TRUE;
	#endif
#endif

	if(gItemSerialCheck==FALSE) return TRUE;

	if( sirial == 0 ) return TRUE;

	for( n=0; n<MAX_WAREHOUSEITEMS; n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

		if( s_num != 0 && s_num == sirial )	// 0�� �ƴϰ� �ø��� �ѹ��� ���� �������� �ִٸ�
		{			
			count++;
		}
	}
	
	if( count <= 1 ) return TRUE;
	
	for( n=0; n<MAX_WAREHOUSEITEMS; n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();
		if( s_num != 0 && s_num == sirial )	// 0�� �ƴϰ� �ø��� �ѹ��� ���� �������� �ִٸ�
		{
			LogAddTD(lMsg.Get(530), lpObj->AccountID, lpObj->Name, lpObj->pWarehouse[n].GetName(), n, s_num);
//			LogAddC(LOGC_RED, lMsg.Get(530), lpObj->AccountID, lpObj->Name, lpObj->pWarehouse[n].GetName(), n, s_num);
			GCServerMsgStringSend(lMsg.Get(1102), lpObj->m_Index, 1);
			gObjUserKill(lpObj->m_Index);
			//gObjInventoryDeleteItem(lpObj->m_Index, n);
			/*ItemMovePathSave(lpObj->AccountID, lpObj->Name, lpObj->pInventory[n].m_Level, lpObj->MapNumber, 
						lpObj->X, lpObj->Y, lpObj->pInventory[n].GetName(), 
						lpObj->pInventory[n].m_Option1, 
						lpObj->pInventory[n].m_Option2,
						lpObj->pInventory[n].m_Option3, 
						lpObj->pInventory[n].m_sirial);*/
		}
	}
	return FALSE;
}


#if 1 //#ifdef PERSONAL_SHOP_20040113 

// return 
// 0xFF : ����
BYTE gObjInventoryMoveItem(int aIndex, BYTE source, BYTE target, int & durSsend, int & durTsend, BYTE sFlag, BYTE tFlag, LPBYTE siteminfo )
{
	LPOBJECTSTRUCT	lpObj;
	BYTE			TempInventoryMap[8*8];				// �κ��丮 �ӽ� ����
	int				w, h, iwidth, iheight;
	int				blank;
	DWORD			s_num;
	CItem*			sitem;
	CItem*			titem;

#ifdef PERSONAL_SHOP_20040113 
	BOOL			bPersonalShopTrans	= FALSE;
	BOOL			bSourceIsPShop		= FALSE;		// ���λ��� -> ���λ��� �̵� �� ������ �ɸ� Ʈ������� ���ϱ� ����
#endif
	
	durSsend = 0;
	durTsend = 0;

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	BOOL useClass=FALSE;

	BYTE sp_ret = 0x00;

#define LEAVE_SET(x) {sp_ret = x; __leave;}

	__try {
		if( sFlag == ITEMMOVE_WAREHOUSE || tFlag == ITEMMOVE_WAREHOUSE )
		{
			// â�� ����̶�µ�.. ���� â�� ������� �ƴϸ�.. �������� ���� �ʴ´�.
			if( lpObj->m_IfState.type != I_WAREHOUSE ) 
			{
				LogAdd(lMsg.Get(531), lpObj->AccountID, lpObj->Name);
				LEAVE_SET(0xFF);
			}
			else if( lpObj->m_IfState.state == 0 ) 
			{
				LogAdd(lMsg.Get(531), lpObj->AccountID, lpObj->Name);
				LEAVE_SET(0xFF);
			}
		}

		// �ű� �������� �����ϴ��� Ȯ��
		switch( sFlag )
		{
		case ITEMMOVE_INVENTORY :
			if( source < 0 || source > MAX_INVENTORY-1 )
			{
				LogAdd("error-L1 : %s %d", __FILE__, __LINE__);
				LEAVE_SET(0xFF);
			}
			if( lpObj->pInventory[source].IsItem() == FALSE ) 
			{
				LogAdd(lMsg.Get(532), lpObj->AccountID, lpObj->Name, __LINE__);
				LEAVE_SET(0xFF);
			}
			sitem = &lpObj->pInventory[source];

#ifdef NEW_SKILL_FORSKYLAND
			if( lpObj->MapNumber == 10 )
			{	// ����ڰ� õ���� ����
				if( source == EQUIPMENT_HELPER  )
				{
					if( sitem->m_Type == MAKE_ITEMNUM(13,3) )
					{	// �䰡�ý��� �������� �Ѵ�
						if( !lpObj->pInventory[EQUIPMENT_WING].IsItem() )
						{	// ������ ���� ������ �� ���´�
							LEAVE_SET(0xFF);
						}
					}
	#ifdef ADD_ITEM_FENRIR_01_20051110
					else if( sitem->m_Type == MAKE_ITEMNUM(13,37) )
					{	// �渱�� �������� �Ѵ�
						if( !lpObj->pInventory[EQUIPMENT_WING].IsItem() )
						{	// ������ ���� ������ �� ���´�
							LEAVE_SET(0xFF);
						}
					}
	#endif
	#ifdef MODIFY_SKYLAND_FENRIR_AND_WING_BUGFIX_20080730
					else if( sitem->m_Type == MAKE_ITEMNUM(13,4) )
					{	// ��ũȣ���� �������� �Ѵ�
						if( !lpObj->pInventory[EQUIPMENT_WING].IsItem() )
						{	// ������ ���� ������ �� ���´�
							LEAVE_SET(0xFF);
						}
					}
	#endif // MODIFY_SKYLAND_FENRIR_AND_WING_BUGFIX_20080730
				}
				else if( source == EQUIPMENT_WING )
				{
	#ifdef MODIFY_SKYLAND_FENRIR_AND_WING_BUGFIX_20080730
					// ������ �������� �Ѵ�
					if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3)	// �䰡�ý��� ���ų�
						&& lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,37)	// �渱�� ���ų�
						&& lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,4) )	// ��ũȣ���� ������
					{	// ��� ������ �Ұ����ϴ�.
						LEAVE_SET(0xFF);
					}
	#else
					if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3) )
					{	// �䰡�ý��� ���� ������ �� ���´�
						LEAVE_SET(0xFF);
					}
	#endif // MODIFY_SKYLAND_FENRIR_AND_WING_BUGFIX_20080730
				}			
			}
#endif

			break;
		case ITEMMOVE_WAREHOUSE :	// â�� 
			if( source < 0 || source > MAX_WAREHOUSEITEMS-1 )
			{
				LogAdd("error-L1 : %s %d", __FILE__, __LINE__);
				LEAVE_SET(0xFF);
			}
			if( lpObj->pWarehouse[source].IsItem() == FALSE ) 
			{
				LogAdd(lMsg.Get(532), lpObj->AccountID, lpObj->Name, __LINE__);
				LEAVE_SET(0xFF);
			}
			sitem = &lpObj->pWarehouse[source];
			break;
#ifdef DARKLORD_WORK
		case ITEMMOVE_DARK_TRAINER :	// ī�����ڽ�2(���û�)
#endif
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		case ITEMMOVE_JEWELOFHARMONY_PURITY:	// ��ȭ
		case ITEMMOVE_JEWELOFHARMONY_SMELT:		// ���ü� ����
		case ITEMMOVE_JEWELOFHARMONY_RESTORE:	// ȯ��
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
		case ITEMMOVE_LOTTERY_MIX:
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
		case ITEMMOVE_CHERRYBLOSSOM:
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312
#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
		case ITEMMOVE_SOCKET_ITEM_EXTRACT:		// ���� ����
		case ITEMMOVE_SOCKET_ITEM_COMPOSIT:		// ���� �ռ�
		case ITEMMOVE_SOCKET_ITEM_EQUIP:		// ���� ����
		case ITEMMOVE_SOCKET_ITEM_REMOVE:		// ���� ����
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415
		case ITEMMOVE_CHAOSBOX :	// ī�����ڽ�
			if( source < 0 || source > MAX_CHAOSBOXITEMS-1 )
			{
				LogAdd("error-L1 : %s %d", __FILE__, __LINE__);
				LEAVE_SET(0xFF);
			}
			if( lpObj->pChaosBox[source].IsItem() == FALSE ) 
			{
				LogAdd(lMsg.Get(532), lpObj->AccountID, lpObj->Name, __LINE__);
				LEAVE_SET(0xFF);
			}
			sitem = &lpObj->pChaosBox[source];
			//lpObj->ChaosSuccessRate -= (sitem->m_BuyMoney/20000);
			//lpObj->ChaosMoney -= lpObj->ChaosSuccessRate*100000;
			//LogAdd("[%s][%s]ī���� �ʿ� �� : %d ����Ȯ�� : %d",lpObj->AccountID, lpObj->Name, lpObj->ChaosMoney, lpObj->ChaosSuccessRate);
			break;
#ifdef PERSONAL_SHOP_20040113
		case ITEMMOVE_PSHOPWND :	// ���λ���
//			if (gDoPShopOpen == 0) {
//				return 0xFF;
//			}

			if (gObj[aIndex].m_bPShopOpen == true) {
				// ���λ����� ������ �Ŀ��� ���λ��������� ������ �̵��� �Ұ�
				LEAVE_SET(0xFF);
			}

			//---------------------------------------
			// ���⼭ ���λ��� Ʈ������� Ȯ���Ѵ�.
			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);
			if (gObj[aIndex].m_bPShopTransaction == true) {
				LogAddTD("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",
					gObj[aIndex].AccountID,
					gObj[aIndex].Name
					);
				
				LeaveCriticalSection(&lpObj->m_critPShopTrade);
				LEAVE_SET(0xFF);
			}
			else  {
				lpObj->m_bPShopTransaction	= true;
				bPersonalShopTrans			= TRUE;
				bSourceIsPShop				= TRUE;
			}
			LeaveCriticalSection(&lpObj->m_critPShopTrade);
			//---------------------------------------

			if( source < MAX_INVENTORY || source > MAX_INVENTORY_EXTEND-1 )
			{
				LogAdd("error-L1 : %s %d", __FILE__, __LINE__);
				LEAVE_SET(0xFF);
			}
			if( lpObj->pInventory[source].IsItem() == FALSE ) 
			{
				LogAdd(lMsg.Get(532), lpObj->AccountID, lpObj->Name, __LINE__);
				LEAVE_SET(0xFF);
			}
			sitem = &lpObj->pInventory[source];
			break;
#endif
		default :
			LEAVE_SET(0xFF);
		}
		
		BYTE level, op1, op2, op3, dur;
		int  type;
		
		BufferItemtoConvert3(siteminfo, type, level, op1, op2, op3, dur);
		
		if( (sitem->m_Type    != type  ) ||
			(sitem->m_Level   != level ) ||
			(sitem->m_Option1 != op1   ) ||
			(sitem->m_Option2 != op2   ) ||
			(sitem->m_Option3 != op3   ) )
		{
			LogAddC(LOGC_RED,lMsg.Get(533), type, level, op1, op2, op3, sitem->m_Type, sitem->m_Level, sitem->m_Option1, sitem->m_Option2, sitem->m_Option3);
			LEAVE_SET(0xFF);
		}

		// ��ǥ ������ �������� Ȯ���Ѵ�.
		switch( tFlag )
		{
		case ITEMMOVE_INVENTORY : 
			if( target< 0 || target > MAX_INVENTORY-1 )
			{
				LogAdd("error-L1 : %s %d", __FILE__, __LINE__);
				LEAVE_SET(0xFF);
			}
			titem = &lpObj->pInventory[target];
			break;
		case ITEMMOVE_WAREHOUSE :
			if( target< 0 || target > MAX_WAREHOUSEITEMS-1 )
			{
				LogAdd("error-L1 : %s %d", __FILE__, __LINE__);
				LEAVE_SET(0xFF);
			}
			titem = &lpObj->pWarehouse[target];
			break;
#ifdef DARKLORD_WORK
		case ITEMMOVE_DARK_TRAINER :
#endif
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		case ITEMMOVE_JEWELOFHARMONY_PURITY:
		case ITEMMOVE_JEWELOFHARMONY_SMELT:
		case ITEMMOVE_JEWELOFHARMONY_RESTORE:
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
		case ITEMMOVE_LOTTERY_MIX:
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
		case ITEMMOVE_CHERRYBLOSSOM:
#endif	// ADD_EVENT_CHERRYBLOSSOM_20080312	
#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
		case ITEMMOVE_SOCKET_ITEM_EXTRACT:			// ���� ����
		case ITEMMOVE_SOCKET_ITEM_COMPOSIT:			// ���� �ռ�
		case ITEMMOVE_SOCKET_ITEM_EQUIP:			// ���� ����
		case ITEMMOVE_SOCKET_ITEM_REMOVE:			// ���� ����
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415
		case ITEMMOVE_CHAOSBOX :
			if( target< 0 || target > MAX_CHAOSBOXITEMS-1 )
			{
				LogAdd("error-L1 : %s %d", __FILE__, __LINE__);
				LEAVE_SET(0xFF);
			}
			titem = &lpObj->pChaosBox[target];
		
			if( sFlag == ITEMMOVE_INVENTORY )
			{

#ifdef DARKLORD_WORK
				if( tFlag == ITEMMOVE_DARK_TRAINER )
				{	// ���û�� �ø��� �ִ� �������� ���� üũ �Ѵ�
					if( sitem->m_Type == MAKE_ITEMNUM(12,15) ||	// ȥ��
						sitem->m_Type == MAKE_ITEMNUM(14,13) ||	// �༮
						sitem->m_Type == MAKE_ITEMNUM(14,14) ||	// ����
						sitem->m_Type == MAKE_ITEMNUM(14,22) ||	// â��
						sitem->m_Type == MAKE_ITEMNUM(13, 31) 	// ��ũȣ���� ��ȥ
#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
						|| sitem->m_Type == MAKE_ITEMNUM( 14, 53 )	// ����� ����
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
						|| sitem->m_Type == MAKE_ITEMNUM( 13, 83) || sitem->m_Type == MAKE_ITEMNUM( 13, 84)
						|| sitem->m_Type == MAKE_ITEMNUM( 13, 85) || sitem->m_Type == MAKE_ITEMNUM( 13, 86) 
						|| sitem->m_Type == MAKE_ITEMNUM( 13, 87) || sitem->m_Type == MAKE_ITEMNUM( 13, 88)
						|| sitem->m_Type == MAKE_ITEMNUM( 13, 89) || sitem->m_Type == MAKE_ITEMNUM( 13, 90)
						|| sitem->m_Type == MAKE_ITEMNUM( 13, 91) || sitem->m_Type == MAKE_ITEMNUM( 13, 92) 
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
						)
					{
					}
					else 
						LEAVE_SET(0xFF);

				}else
#endif
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// ��ȭ�Ǻ��� ���� : ��ȭ�Ǻ��� ������ �̵� ����
				if ( tFlag == ITEMMOVE_JEWELOFHARMONY_PURITY )
				{
					if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal( sitem->m_Type ) == FALSE )
					{
						LEAVE_SET(0xFF);
					}

				} 
				else if ( tFlag == ITEMMOVE_JEWELOFHARMONY_SMELT )
				{
					// ���ü� ���� : �Ϲ� ������ / ������Ʈ �����۸� ���� (�Ϻξ����� ����)
					if ( g_kJewelOfHarmonySystem.IsEnableToMakeSmeltingStoneItem( sitem ) == FALSE )
					{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
						GCServerMsgStringSend( lMsg.Get(3377), lpObj->m_Index, 1); // "���ü����� ������ �� �ִ� �������� �ƴմϴ�"
#endif
						LEAVE_SET(0xFF);
					}
				}
				else if ( tFlag == ITEMMOVE_JEWELOFHARMONY_RESTORE )
				{
					// ��ȭ������ ȯ��
					if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony( sitem ) == FALSE )
					{
						LEAVE_SET(0xFF);
					}
				}
				else 
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
				// �ӽ÷� ��� �������� �ö� �� �ֵ��� ����
				if( tFlag == ITEMMOVE_SOCKET_ITEM_EXTRACT )
				{
				}
				else if( tFlag == ITEMMOVE_SOCKET_ITEM_COMPOSIT )
				{
				}
				else if( tFlag == ITEMMOVE_SOCKET_ITEM_EQUIP )
				{
				}
				else if( tFlag == ITEMMOVE_SOCKET_ITEM_REMOVE )
				{
				}
				else
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
				if( tFlag == ITEMMOVE_LOTTERY_MIX )
				{
#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
					if( sitem->m_Type == MAKE_ITEMNUM( 14, 54 )
						|| sitem->m_Type == MAKE_ITEMNUM( 14, 92 )
						|| sitem->m_Type == MAKE_ITEMNUM( 14, 93 )
						|| sitem->m_Type == MAKE_ITEMNUM( 14, 95 )
						)
					{
					}
					else
#else
					if( sitem->m_Type != MAKE_ITEMNUM( 14, 54 ) )
	#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
					{
						LEAVE_SET(0xFF);
					}
				}
				else
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
					if( tFlag == ITEMMOVE_CHERRYBLOSSOM )
					{
						if( sitem->m_Type != MAKE_ITEMNUM( 14, 88 ) 
							&& sitem->m_Type != MAKE_ITEMNUM( 14, 89 ) 
							&& sitem->m_Type != MAKE_ITEMNUM( 14, 90 ) 
							)
						{
							LEAVE_SET(0xFF);
						}
					}
					else
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

				if(sitem->m_Level < 4 && (sitem->m_Option3*4) < 4) 
				{
					// ī���� �ڽ��� ������ üũ�κ�

					if( sitem->m_Type == MAKE_ITEMNUM(12,15) ||
						sitem->m_Type == MAKE_ITEMNUM(14,13) ||
						sitem->m_Type == MAKE_ITEMNUM(14,14) || 
						sitem->m_Type == MAKE_ITEMNUM(14,22) || // â���� ����
						sitem->m_Type == MAKE_ITEMNUM(13,14) )
					{

					}
					else if( sitem->m_Type >= MAKE_ITEMNUM(12,0) && sitem->m_Type <= MAKE_ITEMNUM(12,2) )
					{	// ���� ����
					}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1������(����� ����) ī�����ڽ� ������üũ
					else if( sitem->m_Type == MAKE_ITEMNUM(12, 41) )
					{
					}
#endif
					else if( sitem->m_Type == MAKE_ITEMNUM(14,17) || 
							 sitem->m_Type == MAKE_ITEMNUM(14,18) )
					{	// �Ǹ��� ��������� (��, ����)
					}
					else if( sitem->m_Type == MAKE_ITEMNUM(13,2) &&		// ���ϸ����� ���� �������� 255 �� �͸� ī�����ڽ��� �÷����� �� �ִ�.
						sitem->m_Durability == 0xFF
						)
					{	// ����Ʈ ���� ������ - ���ϸ���
					}
#ifdef FOR_BLOODCASTLE
					else if( sitem->m_Type == MAKE_ITEMNUM(13,16) || 
							 sitem->m_Type == MAKE_ITEMNUM(13,17) )
					{	// ����ĳ�� ������ (��õ���Ǽ�, ���庻)
					}
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// �༮����, �������� ����â�� �ö󰡵��� ����
//#ifdef CASTLE_SPECIAL_ITEMMIX_20050425	- ����
					else if( sitem->m_Type == MAKE_ITEMNUM(12,30) || 
							 sitem->m_Type == MAKE_ITEMNUM(12,31) )
					{	// �������� ������ (�༮����, ��������)
					}
//#endif
#endif	// THIRD_CHANGEUP_SYSTEM_20070507	
#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
					else if( sitem->m_Type == MAKE_ITEMNUM(14,31) )
					{
						// ��ȣ���� �߰�
					}
#endif
#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
					else if( sitem->m_Type == MAKE_ITEMNUM(12,26) )
					{
						// �ź��� ������ �߰�
					}
#endif
#ifdef ADD_ITEM_FENRIR_01_20051110
					else if(	sitem->m_Type == MAKE_ITEMNUM(13,32)
							||	sitem->m_Type == MAKE_ITEMNUM(13,33)	
							||	sitem->m_Type == MAKE_ITEMNUM(13,34)
							||	sitem->m_Type == MAKE_ITEMNUM(13,35)
							||	sitem->m_Type == MAKE_ITEMNUM(13,36)
							||	sitem->m_Type == MAKE_ITEMNUM(13,37)

							||  sitem->m_Type == MAKE_ITEMNUM(14,16)
						   )
					{
						// �渱 ���� ������ �߰�
					}
#endif
#ifdef ADD_SHIELD_POINT_01_20060403
					else if(	sitem->m_Type == MAKE_ITEMNUM(14,3)		// ūġ�Ṱ�� 
							||	sitem->m_Type == MAKE_ITEMNUM(14,38)	// �������չ���
							||	sitem->m_Type == MAKE_ITEMNUM(14,39)	// �߰����չ���
							)
					{
						// ���� ���� ������ �߰�
					}
#endif
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#ifdef ADD_380ITEM_NEWOPTION_20060711
					else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(sitem->m_Type) == TRUE ) 
					{
						// ��ȭ�Ǻ��� (380 ������ ����)
					}
					else if ( sitem->m_Type == MAKE_ITEMNUM(14,31) )
					{
						// ��ȣ�Ǻ��� (380 ������ ����)
					}
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
					else if( sitem->m_Type == MAKE_ITEMNUM( 14, 53 ) )
					{
						// ����� ����
					}
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
#ifdef ADD_CHAOSMIXCHARM_ITEM_20080702
					else if( sitem->m_Type == MAKE_ITEMNUM( 14, 96 ) )
					{
						// ī���� ���� ����
					}					
#endif // ADD_CHAOSMIXCHARM_ITEM_20080702
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ���� �η縶�� ������ ����
					else if( sitem->m_Type == MAKE_ITEMNUM(13, 49)		// ���� �η縶��
						|| sitem->m_Type == MAKE_ITEMNUM(13, 50)			// ȯ�� ������ ��
						)
					{
					}
#endif	
#ifdef ADD_THIRD_WING_20070525	// ����â�� 3�� ���� ���� ������ ����
					else if( g_MixSystem.IsMixPossibleItem( sitem->m_Type ) == TRUE )
					{
						// ������ ���� ������ �������� ���� �Լ���...
					}
#endif
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
					else if( sitem->m_Type == MAKE_ITEMNUM( 13, 83) || sitem->m_Type == MAKE_ITEMNUM( 13, 84)
						|| sitem->m_Type == MAKE_ITEMNUM( 13, 85) || sitem->m_Type == MAKE_ITEMNUM( 13, 86) 
						|| sitem->m_Type == MAKE_ITEMNUM( 13, 87) || sitem->m_Type == MAKE_ITEMNUM( 13, 88)
						|| sitem->m_Type == MAKE_ITEMNUM( 13, 89) || sitem->m_Type == MAKE_ITEMNUM( 13, 90)
						|| sitem->m_Type == MAKE_ITEMNUM( 13, 91) || sitem->m_Type == MAKE_ITEMNUM( 13, 92)) 
					{						
					}
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
					else LEAVE_SET(0xFF);
				}
//	MU_CHN_THANKS_COOKIE_EVENT_20040908		-> ������ ������ �Ǵ� ���� ����
				else 
				{
					// ����� ���ڷ��� �÷����� �� ����.
					if (sitem->m_Type == MAKE_ITEMNUM(14,11))
						LEAVE_SET(0xFF);
				}
//	MU_CHN_THANKS_COOKIE_EVENT_20040908
				
			}

			//lpObj->ChaosSuccessRate += (titem->m_BuyMoney/20000);
			//lpObj->ChaosMoney += lpObj->ChaosSuccessRate*100000;
			//LogAdd("[%s][%s]ī���� �ʿ� �� : %d ����Ȯ�� : %d",lpObj->AccountID, lpObj->Name, lpObj->ChaosMoney, lpObj->ChaosSuccessRate);
			break;
#ifdef PERSONAL_SHOP_20040113
		case ITEMMOVE_PSHOPWND :	// ���λ���
			if (gDoPShopOpen == 0) 
			{
				LEAVE_SET(0xFF);
			}

			if (gObj[aIndex].m_bPShopOpen == true) {
				// ���λ����� ������ �Ŀ��� ���λ��������� ������ �̵��� �Ұ�
				LEAVE_SET(0xFF);
			}

			//--------------------------------------------
			// ���⿡�� ���λ����� �ø��� ���� ���ǵ��� ���´�.
#ifdef DARK_LORD_HEART_EVENT_20040810
			if (sitem->m_Type == MAKE_ITEMNUM (14, 11) &&		// ��ũ�ε��� ������ �ű� �� ����.
				sitem->m_Level == 13
				)
			{
				LEAVE_SET(0xFF);
			}
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			// ��ȭ�Ǻ������� ��ȭ�� �������� ���λ��� �Ǹ� �Ұ�
			if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony( sitem ) == TRUE )
			{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
				GCServerMsgStringSend(lMsg.Get(3367), lpObj->m_Index, 1); // ��ȭ�� �������� ���� ���� �̿��� �Ұ����մϴ�.
#endif // ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
				LEAVE_SET(0xFF);
			}
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
			if( IsCashItem( sitem->m_Type ) == TRUE )
			{
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
				switch(sitem->m_Type)
				{
					// ĳ���������̶� �ص� ���� �������� �ŷ� �����ϰ� �Ѵ�. �Ϻ� ��û
					case MAKE_ITEMNUM2(13, 83):  // ��ź���� ����
					case MAKE_ITEMNUM2(13, 84):  // õ������ ����
					case MAKE_ITEMNUM2(13, 85):  // �������� ����
					case MAKE_ITEMNUM2(13, 86):  // ��ӳ��� ����
					case MAKE_ITEMNUM2(13, 87):  // �������� ����
					case MAKE_ITEMNUM2(13, 88):  // ���ﳯ�� ����
					case MAKE_ITEMNUM2(13, 89):  // ��ȥ���� ����
					case MAKE_ITEMNUM2(13, 90):  // ���ɳ��� ����
					case MAKE_ITEMNUM2(13, 91):  // �������� ����
					case MAKE_ITEMNUM2(13, 92):  // �������� ���� 
						break;
					default:
						LEAVE_SET(0xFF);
				}
#else
				LEAVE_SET(0xFF);
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PREMIUMITEM_20090106
			// �����̾� �������� ���λ��� �Ǹ� �Ұ�
			if( IsPremiumItem( sitem->m_Type ) == TRUE )
			{
				LEAVE_SET(0xFF);
			}
#endif // PREMIUMITEM_20090106
			
#ifdef KANTURU_MOONSTONEPENDANT_20060804	// ���λ��� �Ǹ� �Ұ�
			if( sitem->m_Type == MAKE_ITEMNUM( 13, 38 ) )
			{
				GCServerMsgStringSend( lMsg.Get( 3390 ), lpObj->m_Index, 1 ); // ���������Ʈ�� ���λ��� �̿��� �Ұ����մϴ�.
				LEAVE_SET(0xFF);
			}
#endif

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ���λ��� �Ǹ� �Ұ�
			// ����Ʈ�ذ����纯�Ź���(�̺�Ʈ��) ���λ��� �Ǹ� �Ұ�
			if( sitem->m_Type == MAKE_ITEMNUM( 13, 39 ) )
			{				
				LEAVE_SET(0xFF);
			}						
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206	// ���λ��� �Ǹ� �Ұ�
			// �ʷ�, ����, ���� ȥ���� ���� ���λ��� �Ǹ� �Ұ�
			if( g_PCBangPointSystem.CheckPCBangPointItem( sitem->m_Type ) )
			{				
				LEAVE_SET(0xFF);
			}	
#endif

#ifdef MODIFY_MAGICIARINGRING_TRADE_BUGFIX_20070328		// ��� ������ ������ ���λ������� �ű� �� ����
			if( sitem->m_Type == MAKE_ITEMNUM( 13, 20 ) 
				&& (sitem->m_Level == 1 || sitem->m_Level == 2) )
			{
				LEAVE_SET(0xFF);
			}
#endif

#ifdef MODIFY_QUEST_SYSTEM_20070525	// ����Ʈ������ ���λ��� �Ǹ� �Ұ�
			if( sitem->m_QuestItem )
			{
				LEAVE_SET(0xFF);
			}
#endif
			
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723	// �������� 0�� ���ǵη縶���� ���λ��� �Ǹ� �Ұ�
			if(lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 51 ) 
				&& lpObj->pInventory[source].m_Durability == 0)
			{
				LEAVE_SET(0xFF);
			}
#endif
			//--------------------------------------------

			//---------------------------------------
			// ���⼭ ���λ��� Ʈ������� Ȯ���Ѵ�.
			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);
			if (gObj[aIndex].m_bPShopTransaction == true) {
				if (bSourceIsPShop == TRUE) {
					// ���� ������ �̵��̴�.
					lpObj->m_bPShopTransaction = true;
					bPersonalShopTrans = TRUE;
				}
				else {
					LogAddTD("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",
						gObj[aIndex].AccountID,
						gObj[aIndex].Name
						);
					
					LeaveCriticalSection(&lpObj->m_critPShopTrade);
					LEAVE_SET(0xFF);
				}
			}
			else {
				lpObj->m_bPShopTransaction = true;
				bPersonalShopTrans = TRUE;
			}
			LeaveCriticalSection(&lpObj->m_critPShopTrade);
			//---------------------------------------
			
			if( target< MAX_INVENTORY || target > MAX_INVENTORY_EXTEND-1 )
			{
				LogAdd("error-L1 : %s %d", __FILE__, __LINE__);
				LEAVE_SET(0xFF);
			}
			titem = &lpObj->pInventory[target];
			break;
#endif
		default :
			LEAVE_SET(0xFF);
		}	

		s_num = sitem->GetNumber();

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202	
		if( gObjCheckSerial0ItemList(sitem) )
		{
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			MsgOutput( lpObj->m_Index, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
			LogAddTD( "[ANTI-HACK][Serial 0 Item] [MoveItem] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID, lpObj->Name, sitem->GetName(), source );
			LEAVE_SET(0xFF);
		}
#endif
		
		if( gObjInventorySearchSerialNumber(lpObj, s_num) == FALSE )
		{
			LEAVE_SET(0xFF);
		}
		if( gObjWarehouseSearchSerialNumber(lpObj, s_num) == FALSE )
		{
			LEAVE_SET(0xFF);
		}	

		// ��� ��ġ�� �������� �ִٸ�.. 
		if( titem->IsItem() == TRUE  ) 
		{
			int max_count=0;
			
#ifdef ADD_ITEM_FENRIR_01_20051110	// �渱 ���� ������ ���� ��������.

			if( sitem->m_Type == MAKE_ITEMNUM(13, 32) )	// ���� ����
			{
				if( titem->m_Durability < MAX_OVERLAP_FENRIR_MIX_STUFF_01 ) 
					max_count = MAX_OVERLAP_FENRIR_MIX_STUFF_01;
			}

			if( sitem->m_Type == MAKE_ITEMNUM(13, 33) )	// ������ ��ȣ
			{
				if( titem->m_Durability < MAX_OVERLAP_FENRIR_MIX_STUFF_02 ) 
					max_count = MAX_OVERLAP_FENRIR_MIX_STUFF_02;
			}

			if( sitem->m_Type == MAKE_ITEMNUM(13, 34) )	// �ͼ��� ����
			{
				if( titem->m_Durability < MAX_OVERLAP_FENRIR_MIX_STUFF_03 ) 
					max_count = MAX_OVERLAP_FENRIR_MIX_STUFF_03;
			}
#endif

#ifdef ADD_SHIELD_POINT_01_20060403	// SD ���� �ִ� �������� �� 1
			if( sitem->m_Type >= MAKE_ITEMNUM(14, 35)
				|| sitem->m_Type >= MAKE_ITEMNUM(14, 36)
				|| sitem->m_Type >= MAKE_ITEMNUM(14, 37)
				)
			{
				if ( titem->m_Durability < 1 )
					max_count = 1;
			}
#endif // ADD_SHIELD_POINT_01_20060403

#ifdef ADD_PCS_FULL_RECOVERY_POTION_20070508
			if( sitem->m_Type == MAKE_ITEMNUM( 14, 63 )
				|| sitem->m_Type == MAKE_ITEMNUM( 14, 64 )
			  )
			{
				if( titem->m_Durability < MAX_OVERLAP_FULL_RECOVERY_POTION )
				{
					max_count = MAX_OVERLAP_FULL_RECOVERY_POTION;
				}
			}
#endif // ADD_PCS_FULL_RECOVERY_POTION_20070508

#ifdef ITEM_ADD_OPTION_20061019
#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
			// �� �� ���� �̺�Ʈ ������ �������� ������ 3���� ����
			if( sitem->m_Type >= ITEM_POTION+46 &&
				sitem->m_Type <= ITEM_POTION+50
				)
			{
				if( titem->m_Durability < MAX_OVERLAP_JACK_O_LANTERN_POTION )
				{
					max_count = MAX_OVERLAP_JACK_O_LANTERN_POTION;
				}
			}
#endif
#endif 

#ifdef ADD_NEW_MAP_KALIMA_20040518						// ����� ǥ���� ���� ��������.
			if (sitem->m_Type == MAKE_ITEMNUM(14, 29)
#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
				&& sitem->m_Level == titem->m_Level		// ����� ǥ���� �����ϰ� ������ ������
#endif
				) 
			{
				max_count = MAX_KALIMAGATE_STONE_OVERLAP;
			}
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� �κ��̵��� ��ġ��
			if (sitem->m_Type == MAKE_ITEMNUM(14, 100))
			{
				if( titem->m_Durability < MAX_OVERLAP_LUCKY_COIN )
				{
					max_count = MAX_OVERLAP_LUCKY_COIN;
				}
			}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
			// �ູ�� ����, ��ȥ�� ���� ��ġ�� �ϱ�
			if( sitem->m_Type == MAKE_ITEMNUM(14,7) )
			{
				if( titem->m_Durability < MAX_SPECIAL_POTION_OVERLAP_COUNT ) 
					max_count = MAX_SPECIAL_POTION_OVERLAP_COUNT;
			}
			else
#endif
			// �ҽ��� �๰�̰�..
			if( sitem->m_Type >= ITEM_POTION && 
				sitem->m_Type <= ITEM_POTION+8
#ifdef ADD_SHIELD_POINT_01_20060403
				|| ( sitem->m_Type >= ITEM_POTION+38 && sitem->m_Type <= ITEM_POTION+40 ) // ���չ��� �ִ� 3��
#endif
				) 
			{
				if( titem->m_Durability < g_GlobalConfig.m_iMaxPotionDurability ) max_count = g_GlobalConfig.m_iMaxPotionDurability;
			}
			else if( (sitem->m_Type == MAKE_ITEMNUM(4,15)) ||
    				 (sitem->m_Type == MAKE_ITEMNUM(4,7 )) )
			{
				if( titem->m_Durability < 255 ) max_count = 255;
			}

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
			else if ( (sitem->m_Type == MAKE_ITEMNUM(14, 70) 
				||  sitem->m_Type == MAKE_ITEMNUM(14, 71) || sitem->m_Type >= MAKE_ITEMNUM(14, 94) )
				&& titem->m_Durability < MAX_OVERLAP_ELITE_POTION )
			{
				max_count = MAX_OVERLAP_ELITE_POTION;
			}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318		// ����Ʈ SD ȸ�� ����
			else if ( (sitem->m_Type == MAKE_ITEMNUM(14, 133))
				&& titem->m_Durability < MAX_OVERLAP_ELITE_POTION )
			{
				max_count = MAX_OVERLAP_ELITE_POTION;
			}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312		// ���ɳ��� �̺�Ʈ
#ifdef MODIFY_ITEMDURABILITY_BUGFIX_20080408	// ������ ��ġ�� ���� �ذ�
			else if ( sitem->m_Type == MAKE_ITEMNUM(14, 88) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_WHITE )	// �Ͼ� ���ɰ���
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_WHITE;
			}
			else if ( sitem->m_Type == MAKE_ITEMNUM(14, 89) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_RED)		// ���� ���ɰ���
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_RED;
			}
			else if ( sitem->m_Type == MAKE_ITEMNUM(14, 90) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_GOLD)	// Ȳ�� ���ɰ���
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_GOLD;
			}
			else if ( sitem->m_Type == MAKE_ITEMNUM(14, 85) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT)	// ���ɼ�
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT;
			}
			else if ( sitem->m_Type == MAKE_ITEMNUM(14, 86) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT)	// ���ɳ��̰��
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT;
			}
			else if ( sitem->m_Type == MAKE_ITEMNUM(14, 87) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT)	// ���ɲ���
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT;
			}			
#else	// MODIFY_ITEMDURABILITY_BUGFIX_20080408			
			else if ( sitem->m_Type >= MAKE_ITEMNUM(14, 88) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_WHITE )	// �Ͼ� ���ɰ���
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_WHITE;
			}
			else if ( sitem->m_Type >= MAKE_ITEMNUM(14, 89) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_RED)		// ���� ���ɰ���
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_RED;
			}
			else if ( sitem->m_Type <= MAKE_ITEMNUM(14, 90) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_GOLD)	// Ȳ�� ���ɰ���
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_GOLD;
			}
			else if ( sitem->m_Type <= MAKE_ITEMNUM(14, 85) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT)	// ���ɼ�
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT;
			}
			else if ( sitem->m_Type <= MAKE_ITEMNUM(14, 86) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT)	// ���ɳ��̰��
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT;
			}
			else if ( sitem->m_Type <= MAKE_ITEMNUM(14, 87) 
				&& titem->m_Durability < MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT)	// ���ɲ���
			{
				max_count = MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT;
			}			
#endif	// MODIFY_ITEMDURABILITY_BUGFIX_20080408
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef MODIFY_MARKOFLORD_OVERLAPPED_20050530		// ������ ǥ���� ��������.
			if (sitem->m_Type == MAKE_ITEMNUM(14, 21) &&
				sitem->m_Level == 3
				)
			{
				max_count = MAX_CS_SYNC_MARKS_OVERLAP;
			}
#endif

			if( max_count )
			{			
				if( sFlag != ITEMMOVE_INVENTORY )	// ���� ��ġ��� �κ��丮������.. 
					LEAVE_SET(0xFF);

				// ����� ���ٸ�..
				if( titem->m_Type == sitem->m_Type && 
					titem->m_Level == sitem->m_Level )
				{
					// �ִ밪���� �۴ٸ� ��ġ�� �Ѵ�.
					int total = (int)titem->m_Durability;
					if( total < max_count )
					{
						int sdur = (int)max_count-(int)titem->m_Durability;
						if( sitem->m_Durability < sdur )
						{
							sdur = (int)sitem->m_Durability;
						}
						titem->m_Durability += sdur;
						sitem->m_Durability -= sdur;

#ifdef ADD_NEW_MAP_KALIMA_20040518					// ����� ǥ���� ��� ������ �������� �������� 20�� �Ѿ��ٸ� (���⼱ �׻� 20) �Ҿ���� ������ ���Ѵ�.
//######BUGFIX_CAT, Some codes in '20050204_001_bak' is changed as follows. ǥ�� ��ĥ ��, �������� ���̴� ���� ����.
						if (titem->m_Type == MAKE_ITEMNUM(14, 29) && titem->m_Durability >= max_count) {
							// �Ҿ���� ������ �ٲ۴�.

							// MAX_KALIMAGATE_STONE_OVERLAP ��ŭ �����ϰ�,
							titem->m_Durability -= max_count;

							if (titem->m_Durability == 0)
							{
								gObjInventoryItemSet(aIndex, target, 0xFF);
								gObj[aIndex].pInventory[target].Clear();
								GCInventoryItemDeleteSend(aIndex, target, 1);
								durTsend = 0;
							}
							else // ���°� ���ܾ���.
							{
								durTsend = 1;
							}

							// �Ҿ���� ������ ����ִ´�.
							ItemSerialCreateSend(
								aIndex, 
								ITEMMAKE_INVENTORY_1CELL, 
								(BYTE)gObj[aIndex].X, 
								(BYTE)gObj[aIndex].Y, 
								ItemGetNumberMake(14, 28), 
								sitem->m_Level, (BYTE)0, 0, 0, 0, aIndex);
						}						
//BUGFIX_CAT001########
#endif
						if( sitem->m_Durability > 0 )
						{
							durSsend = 1;
#ifdef MODIFY_MOVE_KUNDUN_MARK_20070307	// ����� ǥ�� �̵��� ������ �̹��� ������� ���� ����
							if( sitem->m_Type != MAKE_ITEMNUM(14, 29))	// ����� ǥ���϶��� ������ �ȵ�
								durTsend = 1;
#else	// MODIFY_MOVE_KUNDUN_MARK_20070307
							durTsend = 1;
#endif	// MODIFY_MOVE_KUNDUN_MARK_20070307
						}
						else 
						{
							switch( sFlag )
							{
							case ITEMMOVE_INVENTORY :
								gObjInventoryItemSet(lpObj->m_Index, source, 0xFF);
								sitem->Clear();
								GCInventoryItemDeleteSend(lpObj->m_Index, source, 0);
								durTsend = 1;
								break;
							}
							LEAVE_SET(0x00);
						}
					}
				}
			}
			LEAVE_SET(0xFF);
		}
		// �������� ���ٸ�.. �������� �̵���Ų��.

#ifdef UPDATE_GM_FUNCTION_20070228	// GM ĳ���� ���Ź����� ���� ������ �̵��� ���´�
		// �������� ó��
		switch( tFlag )
		{
			case ITEMMOVE_PSHOPWND :	// ����� ���λ���
			case ITEMMOVE_DARK_TRAINER :
			case ITEMMOVE_JEWELOFHARMONY_PURITY:
			case ITEMMOVE_JEWELOFHARMONY_SMELT:
			case ITEMMOVE_JEWELOFHARMONY_RESTORE:
			case ITEMMOVE_CHAOSBOX :
				if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(13, 42) )
				{
					LEAVE_SET(0xFF);
				}

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ���λ��� ����
				if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(14, 64) )
				{
					LEAVE_SET(0xFF);
				}
#endif	
				break;

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// �κ��丮 �̵��� ��ġ ����
			case ITEMMOVE_INVENTORY:			// �κ��丮 �̵��� ��ġ ����
				if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) && lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(14, 64) )
				{
					g_IllusionTempleEvent.SetInventoryPos(lpObj->MapNumber, aIndex, target);
				}
				break;
#endif	
		}
#endif

		// �������� ó��
		switch( tFlag )
		{
		case ITEMMOVE_INVENTORY :	// ����� �κ��丮
#ifdef PERSONAL_SHOP_20040113		
		case ITEMMOVE_PSHOPWND :	// ����� ���λ���
#endif
			// ����� ��� ���� �̶��		
			if( target < MAX_EQUIPMENT )
			{
				if( lpObj->MapNumber == 10 )
				{	// ����ڰ� õ���� ����
					if( target == EQUIPMENT_RING_RIGHT )
					{	// ���� ������ �������� �Ѵ�
						if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(13,10) )
						{	// ���Ź��� �ȵ�
							LEAVE_SET(0xFF);
						}
					}
					else if( target == EQUIPMENT_RING_LEFT )
					{	// ���� ������ �������� �Ѵ�
						if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(13,10) )
						{	// ���Ź��� �ȵ�
							LEAVE_SET(0xFF);
						}
					}
				}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ��ī�罺, ĭ���� ���������� ���� ����
			if( target < MAX_EQUIPMENT )
			{
				if( lpObj->MapNumber == MAP_INDEX_ICARUS || lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS ) 
				{	
					if( target == EQUIPMENT_RING_RIGHT )
					{	
						if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 39 ) ||
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 40 ) ||
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 41 ) ||
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
#ifdef MODIFY_WEARING_PENDA_SNOWMAN_IN_ICARUS_AND_CANTUR_20090914
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 68 ) ||	// ����� ���Ź���
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 76 ) ||	// �Ҵ� ���Ź���
#endif //MODIFY_WEARING_PENDA_SNOWMAN_IN_ICARUS_AND_CANTUR_20090914
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 10 ) )
						{	
							LEAVE_SET(0xFF);
						}
					}
					else if( target == EQUIPMENT_RING_LEFT )
					{	
						if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 39 ) ||
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 40 ) ||
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 41 ) ||
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
#ifdef MODIFY_WEARING_PENDA_SNOWMAN_IN_ICARUS_AND_CANTUR_20090914
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 68 ) ||	// ����� ���Ź���
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 76 ) ||	// �Ҵ� ���Ź���
#endif //MODIFY_WEARING_PENDA_SNOWMAN_IN_ICARUS_AND_CANTUR_20090914
							lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 10 ) )
						{	
							LEAVE_SET(0xFF);
						}
					}
				}	

#ifdef UPDATE_GM_FUNCTION_20070228	// GM �� �ƴҰ�� GM ĳ���� ���Ź��� �������
				if( target == EQUIPMENT_RING_RIGHT )
				{
					if( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
					{
						if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 42 ) )
							LEAVE_SET(0xFF);
					}
				}
				else if( target == EQUIPMENT_RING_LEFT )
				{
					if( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
					{
						if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 42 ) )
							LEAVE_SET(0xFF);
					}
				}
#endif // UPDATE_GM_FUNCTION_20070228
			}
#endif
			
#ifdef ADD_THIRD_WING_20070525	// IsClass ����
				useClass = sitem->IsClass( lpObj->Class, lpObj->ChangeUP, lpObj->ThirdChangeUp );
#else
	#ifdef NEW_SKILL_FORSKYLAND
				useClass = sitem->IsClass(lpObj->Class, lpObj->ChangeUP);
	#else
				useClass = sitem->IsClass(lpObj->Class);
	#endif
#endif	// ADD_THIRD_WING_20070525
				
				// �����Ϸ��� �Ҷ� �ڽ��� ��� �ƴҰ��
				if( useClass == FALSE )	LEAVE_SET(0xFF);

				if( gObjIsItemPut(lpObj, sitem, target) == FALSE ) LEAVE_SET(0xFF);
			}
			else // ����� �κ��丮��..
			{
#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// ������ �������� �̺�Ʈ ���� �����۵��� ���λ����̵� ����
				if(target >= MAX_INVENTORY) {
					if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(12,26) &&
						(lpObj->pInventory[source].m_Level == 1 ||	// ��������
						lpObj->pInventory[source].m_Level == 2 ||	// Ǫ������
						lpObj->pInventory[source].m_Level == 3 ||	// ��������
						lpObj->pInventory[source].m_Level == 4 ||	// ������ ����
						lpObj->pInventory[source].m_Level == 5		// ��¦���� (�������� ��ǰ)
						))
					{
						LEAVE_SET(0xFF);
					}
				}
#endif
				
				// gObjInventoryInsertItemPos() -> ��� ���ϰ� �ٷ� ����
				w = (target-MAX_EQUIPMENT)%8;
				h = (target-MAX_EQUIPMENT)/8;
#ifdef PERSONAL_SHOP_20040113		
				if( ExtentCheck(w, h, 8, 12) == FALSE ) LEAVE_SET(0xFF);
#else
				if( ExtentCheck(w, h, 8, 8) == FALSE ) LEAVE_SET(0xFF);
#endif			
				sitem->GetSize(iwidth, iheight);
				
				memcpy(TempInventoryMap, lpObj->pInventoryMap, 8*8);
				switch( sFlag )
				{
				case ITEMMOVE_INVENTORY :	// �κ��丮���� �κ��丮 (Ȥ�� ���λ���)
#ifdef PERSONAL_SHOP_20040113		
				case ITEMMOVE_PSHOPWND :	// ���λ������� �κ��丮 (Ȥ�� ���λ���)
#endif
					if( source > MAX_EQUIPMENT-1 )
					{
						gObjInventoryItemBoxSet(lpObj->m_Index, source, iwidth, iheight, 0xFF);	
					}
					break;
				case ITEMMOVE_WAREHOUSE :	// â���� �κ��丮 
					break;
				}
				
				// ��������ʴٸ�.. ����..
				if( *(gObj[aIndex].pInventoryMap+(h*8)+w) != 0xFF ) {
					memcpy(lpObj->pInventoryMap, TempInventoryMap, 8*8);
					LEAVE_SET(0xFF);
				}
				blank = gObjInventoryRectCheck(lpObj->m_Index, w, h, iwidth,iheight);
				if( blank >= 0xFE )
				{
					memcpy(lpObj->pInventoryMap, TempInventoryMap, 8*8);
					LEAVE_SET(0xFF);
				}
			}

			s_num = sitem->GetNumber();

			// ����ִٸ� ����
			titem = sitem;

			switch( sFlag )
			{
			case ITEMMOVE_INVENTORY :	// ������ �κ��丮
#ifdef PERSONAL_SHOP_20040113		
			case ITEMMOVE_PSHOPWND :	// ������ ���λ���
#endif
				lpObj->pInventory[target] = lpObj->pInventory[source];	// ���⼭ ���������� �������� �̵���
#ifdef MU_CHN_THANKS_RING_EVENT_20040908				// �߱� �̺�Ʈ - ������ ����
				{
					BOOL bSendEquip = FALSE;
					if(((
						source == EQUIPMENT_RING_LEFT || 
						source == EQUIPMENT_RING_RIGHT
						) &&
						sitem->m_Type == MAKE_ITEMNUM(13,20) &&
						sitem->m_Level == 3
						) ||
						((
						target == EQUIPMENT_RING_LEFT || 
						target == EQUIPMENT_RING_RIGHT
						) &&
						titem->m_Type == MAKE_ITEMNUM(13,20) &&
						sitem->m_Level == 3
						))
					{
						bSendEquip = TRUE;
					}
#endif
				gObjInventoryDeleteItem(aIndex, source);
#ifdef MU_CHN_THANKS_RING_EVENT_20040908				// �߱� �̺�Ʈ - ������ ����
					if (bSendEquip) {
						GCEquipmentSend (aIndex);		// ������ ������ Ż���ÿ� ��� ����Ʈ�� �ٽ� �����ش�.
					}
				}
#endif
				break;
			case ITEMMOVE_WAREHOUSE :
				lpObj->pInventory[target] = lpObj->pWarehouse[source];
				gObjWarehouseDeleteItem(aIndex, source);
				break;

#ifdef DARKLORD_WORK
			case ITEMMOVE_DARK_TRAINER: 
#endif
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			case ITEMMOVE_JEWELOFHARMONY_PURITY:
			case ITEMMOVE_JEWELOFHARMONY_SMELT:
			case ITEMMOVE_JEWELOFHARMONY_RESTORE:
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
			case ITEMMOVE_LOTTERY_MIX:
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
			case ITEMMOVE_SOCKET_ITEM_EXTRACT:				// ���� ����
			case ITEMMOVE_SOCKET_ITEM_COMPOSIT:				// ���� �ռ�
			case ITEMMOVE_SOCKET_ITEM_EQUIP:				// ���� ����
			case ITEMMOVE_SOCKET_ITEM_REMOVE:				// ���� ����
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
			case ITEMMOVE_CHERRYBLOSSOM:
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312
			case ITEMMOVE_CHAOSBOX : 
				lpObj->pInventory[target] = lpObj->pChaosBox[source];
				gObjChaosBoxDeleteItem(aIndex, source);
				break;
			}
			
			// �������� �ʿ����� �������� �ִٰ� ä���.
			if( target > MAX_EQUIPMENT-1 )
			{
				gObjInventoryItemSet(aIndex, target, 1);
			}	
			// �����ۿ� ��ų�� ������ ��ų ����Ʈ�� �߰��Ѵ�.
			if( target < MAX_EQUIPMENT )
			{
				CItem *lpItem;
				lpItem = (CItem*)&lpObj->pInventory[target];
				if( lpItem->m_Option1 )
				{
					int addskill = gObjWeaponMagicAdd(&gObj[aIndex], lpItem->m_Special[0], (BYTE)lpItem->m_Level);
					if( addskill >= 0 ) {
						GCMagicListOneSend(aIndex, addskill, lpItem->m_Special[0], (BYTE)lpItem->m_Level, 0);
					}
				}
			}
			break;
		case ITEMMOVE_WAREHOUSE : // ����� â���̸�..		
			{		
			BYTE ret;
			switch( sFlag )
			{
			case ITEMMOVE_INVENTORY :
#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - â���̵� ����
#ifdef MODIFY_MAGICIARINGRING_TRADE_BUGFIX_20070328		// ������ ���� â���̵� ���� �߰�
if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(13,20) &&
					(lpObj->pInventory[source].m_Level == 0	||	// �������� ����
					 lpObj->pInventory[source].m_Level == 1	||	// ����� ����
					 lpObj->pInventory[source].m_Level == 2		// ������ ����
					) )

#else
				if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(13,20) &&
					(lpObj->pInventory[source].m_Level == 0	||	// �������� ����
					 lpObj->pInventory[source].m_Level == 1		// ������ ����
					))
#endif	// MODIFY_MAGICIARINGRING_TRADE_BUGFIX_20070328
				{	// ������ ������ â��� �̵��� �� ����.
					LEAVE_SET(0xFF);
				}
#endif
#ifdef DARK_LORD_HEART_EVENT_20040810	// ��ũ�ε��� ���� �� â���̵��� �ʵȴ�.
				if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(14,11) &&
					lpObj->pInventory[source].m_Level == 13
					)
				{
					LEAVE_SET(0xFF);
				}
#endif
#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// ������ �������� �̺�Ʈ ���� �����۵��� â���̵� ����
				if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(12,26) &&
					(lpObj->pInventory[source].m_Level == 1 ||	// ��������
					lpObj->pInventory[source].m_Level == 2 ||	// Ǫ������
					lpObj->pInventory[source].m_Level == 3 ||	// ��������
					lpObj->pInventory[source].m_Level == 4 ||	// ������ ����
					lpObj->pInventory[source].m_Level == 5		// ��¦���� (�������� ��ǰ)
					))
				{
					LEAVE_SET(0xFF);
				}
#endif
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830			// â�� �̵� ����
				if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 39 ) )
				{
					LEAVE_SET(0xFF);
				}				
#endif
#ifdef UPDATE_GM_FUNCTION_20070228			// GM ĳ���� ���Ź����� â�� �̵� ����
				if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 42 ) )
				{
					LEAVE_SET(0xFF);
				}	
#endif	
#ifdef PCBANG_POINT_SYSTEM_20070206				// â�� �̵� ����
				if( g_PCBangPointSystem.CheckPCBangPointItem( lpObj->pInventory[source].m_Type ) )
				{
					LEAVE_SET(0xFF);
				}
#endif
#ifdef MODIFY_QUEST_SYSTEM_20070525			// ����Ʈ ������ â���̵� ����
				if( lpObj->pInventory[source].m_QuestItem )
				{
					LEAVE_SET(0xFF);
				}
#endif
				ret = gObjWarehouseInsertItemPos(aIndex, lpObj->pInventory[source], target, -1);
				break;
			case ITEMMOVE_WAREHOUSE :
				ret = gObjWarehouseInsertItemPos(aIndex, lpObj->pWarehouse[source], target, source);
				break;
			}
			
			if( ret == 0xFF )
			{
				LEAVE_SET(0xFF);
			}
			}

			switch( sFlag )
			{
			case ITEMMOVE_INVENTORY :	// �κ����� â���..
				lpObj->pWarehouse[target] = lpObj->pInventory[source];
				gObjInventoryDeleteItem(aIndex, source);
				break;
			case ITEMMOVE_WAREHOUSE :	// â���� â���
				lpObj->pWarehouse[target] = lpObj->pWarehouse[source];
				gObjWarehouseDeleteItem(aIndex, source);
				break;
			}		

			// �������� �ʿ����� �������� �ִٰ� ä���.
			gObjWarehouseItemSet(aIndex, target, 1);
			if( sFlag == 0 )
			{
				// ��� ����Ǿ��ٸ� CharSet�� �ٽ� �����Ѵ�.
				if( source < MAX_EQUIPMENT )
				{
					if( source == EQUIPMENT_RING_RIGHT || source == EQUIPMENT_RING_LEFT)
					{
						if( lpObj->pWarehouse[target].m_Type == MAKE_ITEMNUM(13,10) )
						{
							gObjUseSkill.SkillChangeUse(aIndex);						
						}

#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
						if( lpObj->pWarehouse[target].m_Type == MAKE_ITEMNUM( 13, 40 ) )
						{
							gObjUseSkill.SkillChangeUse( aIndex );
						}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
						if( lpObj->pWarehouse[target].m_Type == MAKE_ITEMNUM( 13, 41 ) )
						{
							gObjUseSkill.SkillChangeUse( aIndex );
						}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef MODIFY_SNOW_MAN_POLYMORPH_RING_BUGFIX_20090414
						if( lpObj->pWarehouse[target].m_Type == MAKE_ITEMNUM( 13, 68 ) )
						{
							gObjUseSkill.SkillChangeUse( aIndex );
						}
#else // MODIFY_SNOW_MAN_POLYMORPH_RING_BUGFIX_20090414
	//------------->
	//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
	#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
						if( lpObj->pWarehouse[target].m_Type == MAKE_ITEMNUM( 13, 68 ) )
						{
							gObjUseSkill.SkillChangeUse( aIndex );
						}
	#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
	//<-------------
#endif // MODIFY_SNOW_MAN_POLYMORPH_RING_BUGFIX_20090414

#ifdef ADD_PREMIUMITEM_PANDA
						if( lpObj->pWarehouse[target].m_Type == MAKE_ITEMNUM( 13, 76 ) )
						{
							gObjUseSkill.SkillChangeUse( aIndex );
						}
#endif // ADD_PREMIUMITEM_PANDA
					}
					gObjMakePreviewCharSet(aIndex);
					GCEquipmentChange(aIndex, source);
				}
			}
			LEAVE_SET(0x02);
#ifdef DARKLORD_WORK
		case ITEMMOVE_DARK_TRAINER :
#endif
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		case ITEMMOVE_JEWELOFHARMONY_PURITY:
		case ITEMMOVE_JEWELOFHARMONY_SMELT:
		case ITEMMOVE_JEWELOFHARMONY_RESTORE:
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
		case ITEMMOVE_LOTTERY_MIX:
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
		case ITEMMOVE_SOCKET_ITEM_EXTRACT:
		case ITEMMOVE_SOCKET_ITEM_COMPOSIT:
		case ITEMMOVE_SOCKET_ITEM_EQUIP:
		case ITEMMOVE_SOCKET_ITEM_REMOVE:
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
		case ITEMMOVE_CHERRYBLOSSOM:
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312
		case ITEMMOVE_CHAOSBOX :
			{		
			BYTE ret;
			switch( sFlag )
			{
			case ITEMMOVE_INVENTORY :
				ret = gObjChaosBoxInsertItemPos(aIndex, lpObj->pInventory[source], target, -1);
				break;
#ifdef DARKLORD_WORK
			case ITEMMOVE_DARK_TRAINER :
#endif
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		case ITEMMOVE_JEWELOFHARMONY_PURITY:
		case ITEMMOVE_JEWELOFHARMONY_SMELT:
		case ITEMMOVE_JEWELOFHARMONY_RESTORE:
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530		
#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
		case ITEMMOVE_LOTTERY_MIX:
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
		case ITEMMOVE_SOCKET_ITEM_EXTRACT:
		case ITEMMOVE_SOCKET_ITEM_COMPOSIT:
		case ITEMMOVE_SOCKET_ITEM_EQUIP:
		case ITEMMOVE_SOCKET_ITEM_REMOVE:
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
		case ITEMMOVE_CHERRYBLOSSOM:
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312
			case ITEMMOVE_CHAOSBOX :
				ret = gObjChaosBoxInsertItemPos(aIndex, lpObj->pChaosBox[source], target, source);
				break;
			}
			
			if( ret == 0xFF )
			{
				LEAVE_SET(0xFF);
			}
			}

			switch( sFlag )
			{
			case ITEMMOVE_INVENTORY :	// �κ����� ī�����ڽ���..
				lpObj->pChaosBox[target] = lpObj->pInventory[source];
				gObjInventoryDeleteItem(aIndex, source);
				break;

#ifdef DARKLORD_WORK
			case ITEMMOVE_DARK_TRAINER :	// ī�����ڽ�2 ���� ī�����ڽ�2�� (���ǻ�).
#endif
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			case ITEMMOVE_JEWELOFHARMONY_PURITY:
			case ITEMMOVE_JEWELOFHARMONY_SMELT:
			case ITEMMOVE_JEWELOFHARMONY_RESTORE:
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
			case ITEMMOVE_LOTTERY_MIX:
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
			case ITEMMOVE_SOCKET_ITEM_EXTRACT:
			case ITEMMOVE_SOCKET_ITEM_COMPOSIT:
			case ITEMMOVE_SOCKET_ITEM_EQUIP:
			case ITEMMOVE_SOCKET_ITEM_REMOVE:
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415
#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
			case ITEMMOVE_CHERRYBLOSSOM:
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312
			case ITEMMOVE_CHAOSBOX :	// ī�����ڽ� ���� ī�����ڽ���.
				lpObj->pChaosBox[target] = lpObj->pChaosBox[source];
				gObjChaosBoxDeleteItem(aIndex, source);
				break;
			}		

			// �������� �ʿ����� �������� �ִٰ� ä���.
			gObjChaosItemSet(aIndex, target, 1);
			if( sFlag == ITEMMOVE_INVENTORY )
			{
				// ��� ����Ǿ��ٸ� CharSet�� �ٽ� �����Ѵ�.
				if( source < MAX_EQUIPMENT )
				{
					if( source == EQUIPMENT_RING_RIGHT || source == EQUIPMENT_RING_LEFT)
					{
#ifdef ITEM_INDEX_EXTEND_20050706
						if( lpObj->pChaosBox[target].m_Type == MAKE_ITEMNUM(13,10) )
#else
						if( lpObj->pChaosBox[target].m_Type == ((13<<ITEM_BITSHIFT)+10) )
#endif
						{
							gObjUseSkill.SkillChangeUse(aIndex);						
						}

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ���� ���� ó��
						if( lpObj->pChaosBox[target].m_Type == MAKE_ITEMNUM( 13, 39 ) )
						{
							gObjUseSkill.SkillChangeUse( aIndex );						
						}							
#endif

#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
						if( lpObj->pChaosBox[target].m_Type == MAKE_ITEMNUM( 13, 40 ) )
						{
							gObjUseSkill.SkillChangeUse( aIndex );
						}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
						if( lpObj->pChaosBox[target].m_Type == MAKE_ITEMNUM( 13, 41 ) )
						{
							gObjUseSkill.SkillChangeUse( aIndex );
						}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
					}
					gObjMakePreviewCharSet(aIndex);
					GCEquipmentChange(aIndex, source);
				}
			}

#ifdef DARKLORD_WORK
			LEAVE_SET(tFlag);
#else
			LEAVE_SET(0x03);
#endif
		}

		if( sFlag == ITEMMOVE_INVENTORY )
		{
			// ��� ����Ǿ��ٸ� CharSet�� �ٽ� �����Ѵ�.
			if( source < MAX_EQUIPMENT )
			{
				if( lpObj->pInventory[source].IsItem()== TRUE)
				{
					if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(13,10) )
					{
						gObjUseSkill.SkillChangeUse(aIndex);
						LogAdd(lMsg.Get(534), gObj[aIndex].Name, lpObj->pInventory[source].m_Level);
					}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ���� ���� ó��
					if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 39 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
						LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[source].m_Level );
					}
#endif
					
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
					if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 40 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
						LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[source].m_Level );
					}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
					if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 41 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
						LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[source].m_Level );
					}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
#ifdef MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
					if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 68 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
						LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[source].m_Level );
					}
					if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 76 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
						LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[source].m_Level );
					}
#endif // MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
				}
				else if( source == EQUIPMENT_RING_RIGHT || source == EQUIPMENT_RING_LEFT)
				{
					if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(13,10) )
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ���� ���� ó��
					if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 39 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
					}
#endif

#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
					if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 40 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
					}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
					if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 41 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
					}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef UPDATE_GM_FUNCTION_20070228
					if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 42 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
						LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[target].m_Level );
					}
#endif // UPDATE_GM_FUNCTION_20070228	
//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
					if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 68 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
						LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[target].m_Level );
					}
#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------
#ifdef ADD_PREMIUMITEM_PANDA
					if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 76 ) )
					{
						gObjUseSkill.SkillChangeUse( aIndex );
						LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[target].m_Level );
					}
#endif // ADD_PREMIUMITEM_PANDA
				}
				gObjMakePreviewCharSet(aIndex);
				GCEquipmentChange(aIndex, source);
			}
		}

		if( tFlag == ITEMMOVE_INVENTORY )
		{
			// ��� ����Ǿ��ٸ� CharSet�� �ٽ� �����Ѵ�.
			if( target < MAX_EQUIPMENT )
			{
				if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(13,10) )
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534), gObj[aIndex].Name, lpObj->pInventory[target].m_Level);
				}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ���� ���� ó��
				if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 39 ) )
				{
					gObjUseSkill.SkillChangeUse( aIndex );
					LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[target].m_Level );
				}
#endif
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
				if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 40 ) )
				{
					gObjUseSkill.SkillChangeUse( aIndex );
					LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[target].m_Level );
				}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
				if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 41 ) )
				{
					gObjUseSkill.SkillChangeUse( aIndex );
					LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[target].m_Level );
				}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef UPDATE_GM_FUNCTION_20070228	// ���� ����� ���� ��ų ���
				if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 42 ) )
				{
					gObjUseSkill.SkillChangeUse( aIndex );
					LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[target].m_Level );
				}
#endif // UPDATE_GM_FUNCTION_20070228
//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
				if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 68 ) )
				{
					gObjUseSkill.SkillChangeUse( aIndex );
					LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[target].m_Level );
				}
#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------
#ifdef ADD_PREMIUMITEM_PANDA
				if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 76 ) )
				{
					gObjUseSkill.SkillChangeUse( aIndex );
					LogAdd( lMsg.Get( 534 ), gObj[aIndex].Name, lpObj->pInventory[target].m_Level );
				}
#endif // ADD_PREMIUMITEM_PANDA
				gObjMakePreviewCharSet(aIndex);
				GCEquipmentChange(aIndex, target);
			}
		}
		
		//if( target < MAX_EQUIPMENT ) GCEquipmentChange(aIndex, target);
		//if( source < MAX_EQUIPMENT ) GCEquipmentChange(aIndex, source);

	}
	__finally {
#ifdef PERSONAL_SHOP_20040113
		if (bPersonalShopTrans == TRUE) {
			// ������ ���λ��� Ʈ������� �ɷ��־��ٸ� ���⼭ Ǯ���־�� ���� �ŷ��� ��ǰ �̵��� �����ϴ�.
			gObj[aIndex].m_bPShopTransaction = false;
		}
#endif
	}

	return sp_ret;
}


#else
	/*
	//	
	//	20051110 PERSONAL_SHOP_20040113 �� ELSE �κ� ���� (apple)
	// 
	*/
#endif


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// ��ȯâ�� üũ�ؼ� ������ ����ִ��� üũ
BYTE gObjTradeRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;

	// ���μ��� ũ�⸦ �ʰ� �ߴٸ� 
	if( (sx+width) > 8 ) return (BYTE)0xFF;
	if( (sy+height) > 4 ) return (BYTE)0xFF;
	
	int xx, yy;

	
	for( y=0; y<height; y++)
	{		
		yy = sy+y;
		for(x=0; x<width; x++)
		{
			xx = sx+x;
			if( ExtentCheck(xx, yy, 8, 4) == TRUE)
			{
				//*(gObj[aIndex].pWarehouseMap+((itemposy+y)*8)+(itemposx+x)) = set_byte;

				if( *(gObj[aIndex].TradeMap+(yy*8)+xx) != 0xFF ) 
				{
					blank++;
					return (BYTE)0xFF;
				}
			}
			else
			{
				LogAdd("error : %s %d", __FILE__, __LINE__);
				return (BYTE)0xFF;
			}
		}
	}

	if( blank == 0 ) return (BYTE)((sy*8)+sx);
	
	return (BYTE)(0xFF);
}



// TradeMap �� � ������ �����Ѵ�.
BOOL gObjTradeItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx, itemposy;
	int xx, yy;

	itemposx = itempos%8;
	itemposy = itempos/8;

	for( int y=0; y<yl; y++)
	{
		yy = itemposy+y;
		for( int x=0; x<xl; x++)
		{
			xx = itemposx+x;

			if( ExtentCheck(xx, yy, 8, 4) == TRUE )
			{
				*(gObj[aIndex].TradeMap+(yy*8)+xx) = set_byte;
			}
			else
			{
				LogAdd("error : %s %d",__FILE__, __LINE__);
				return FALSE;
			}
		}
	}
	return TRUE;
}

// Ʈ���̵忡�� �κ��丮��..
BYTE gObjTradeInventoryMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target)
{
	int iwidth, iheight;

	if( source > MAX_TRADEITEM )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return 0xFF;
	}
	
	if( target > MAX_INVENTORY )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return 0xFF;
	}

	if( lpObj->TargetNumber < 0 ) return 0xFF;

	// �ű���� �������� �����ϴ°�?
	if( lpObj->Trade[source].IsItem() == FALSE )
	{
		return 0xFF;
	}
	
	// �ŷ��� ��ҵǰų� �ŷ����� �ƴ϶�� �������� �ű�� ����.
	if( (lpObj->m_IfState.use == 0) || ( lpObj->m_IfState.type != I_TRADE ))
	{
		return 0xFF;
	}	
	if( gObjInventoryInsertItemPos(lpObj->m_Index, lpObj->Trade[source], target, 1) == 0xFF ) return 0xFF;
	
	DWORD s_num=0;
	s_num = lpObj->Trade[source].m_Number;
	lpObj->Trade[source].GetSize(iwidth, iheight);
	
	gObjTradeItemBoxSet(lpObj->m_Index, source, iwidth, iheight, 0xFF);
	
	lpObj->Trade[source].Clear();

	BYTE itembuf[ITEM_BUFFER_SIZE];

	ItemByteConvert(itembuf, lpObj->Trade[source]);	
	GCTradeOtherDel(lpObj->TargetNumber, source);

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202	
	if( gObjCheckSerial0ItemList(&lpObj->Trade[source]) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( lpObj->m_Index, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
		LogAddTD( "[ANTI-HACK][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID, lpObj->Name, lpObj->Trade[source].GetName(), source );
		return 0xFF;
	}
#endif
	
	if( gObjInventorySearchSerialNumber(lpObj, s_num) == FALSE )
	{
		return 0xFF;
	}

	if( target < MAX_EQUIPMENT ) 
	{
		if( lpObj->pInventory[target].IsItem()== TRUE)
		{
			if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(13,10) )
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);				
				LogAdd(lMsg.Get(534), lpObj->Name, lpObj->pInventory[target].m_Level);
			}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ���� ���� ó��
			if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 39 ) )
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);				
				LogAdd(lMsg.Get(534), lpObj->Name, lpObj->pInventory[target].m_Level);
			}
#endif

#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
			if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 40 ) )
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);				
				LogAdd(lMsg.Get(534), lpObj->Name, lpObj->pInventory[target].m_Level);
			}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
			if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 41 ) )
			{
				gObjUseSkill.SkillChangeUse( lpObj->m_Index );
				LogAdd( lMsg.Get( 534 ), lpObj->Name, lpObj->pInventory[target].m_Level );
			}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
#ifdef MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
			if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 68 ) )
			{
				gObjUseSkill.SkillChangeUse( lpObj->m_Index );
				LogAdd( lMsg.Get( 534 ), lpObj->Name, lpObj->pInventory[target].m_Level );
			}
			if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 13, 76 ) )
			{
				gObjUseSkill.SkillChangeUse( lpObj->m_Index );
				LogAdd( lMsg.Get( 534 ), lpObj->Name, lpObj->pInventory[target].m_Level );
			}
#endif // MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715			
		}
		gObjMakePreviewCharSet(lpObj->m_Index);
		GCEquipmentChange(lpObj->m_Index, target);
	}
	return 0x00;
}

// �κ��丮���� Ʈ���̵�� �̵�
BYTE gObjInventoryTradeMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target)
{
	int h, w;
	int iwidth, iheight;

	if( source > MAX_INVENTORY )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return 0xFF;
	}
	
	// �ű���� �������� �����ϴ°�?
	if( lpObj->pInventory[source].IsItem() == FALSE )
	{
		return 0xFF;
	}

	if(g_GlobalConfig.m_bLimitTrade == TRUE && lpObj->pInventory[source].CheckExcOption(g_GlobalConfig.m_iLimitOption) == TRUE)
	{
		return 0xFF;
	}
	
	if( lpObj->TargetNumber < 0 ) return 0xFF;
	// �ŷ��� ��ҵǰų� �ŷ����� �ƴ϶�� �������� �ű�� ����.
	if( (lpObj->m_IfState.use == 0) || ( lpObj->m_IfState.type != I_TRADE ))
	{
		return 0xFF;
	}
	
#ifdef MODIFY_MAGICIARINGRING_TRADE_BUGFIX_20070328		// �������� ������ �ŷ��� �Ҽ� �ֵ��� ����
	if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[source].m_Level == 1			// ������ ������ �ŷ��� �ʵȴ�.
		)
	{	// �̺�Ʈ �������� �ŷ� �ȵ�
		return 0xFF;
	}
#else	// MODIFY_MAGICIARINGRING_TRADE_BUGFIX_20070328
#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - �ŷ� ����
	if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(13,20) &&
		(lpObj->pInventory[source].m_Level == 0 ||		// �������� ������ �ŷ��� �ʵȴ�.
		lpObj->pInventory[source].m_Level == 1			// ������ ������ �ŷ��� �ʵȴ�.
		) )
	{	// �̺�Ʈ �������� �ŷ� �ȵ�
		return 0xFF;
	}
#endif
#endif	// MODIFY_MAGICIARINGRING_TRADE_BUGFIX_20070328

#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211	// (�� ģ�� �̺�Ʈ) ������ ���� - �ŷ� ����
	if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[source].m_Level == 2			// ������ ������ �ŷ��� �ʵȴ�.
		)
	{	// �̺�Ʈ �������� �ŷ� �ȵ�
		return 0xFF;
	}
#endif

#ifdef DARK_LORD_HEART_EVENT_20040810		// ��ũ�ε��� ���� �� �ŷ��� �ʵȴ�.
	if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(14,11) &&
		lpObj->pInventory[source].m_Level == 13
		)
	{
		return 0xFF;
	}
#endif

#ifdef UPDATE_GM_FUNCTION_20070228	//  GM ĳ���� ���Ź����� �ŷ� ����
	if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 42 ) )
	{
		return 0xFF;
	}
#endif 

#ifdef UPDATE_GM_FUNCTION_20070228			// GM ���� ���ڴ� �ŷ� ����
	if( ( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
		&& ( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 14, 52 ) ) )
	{
		return 0xFF;
	}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// �ŷ� ����
	// �������� 0�� ���ǵη縶���� �ŷ� �Ұ�
	if(lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 51 ) 
		&& lpObj->pInventory[source].m_Durability == 0)
	{
		return 0xFF;
	}

	// ������ �ŷ� ����
	if(lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 14, 64 ))
	{
		return 0xFF;
	}
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// ������ �������� �̺�Ʈ ���� �����۵��� �ŷ� ����
	if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM(12,26) &&
		(lpObj->pInventory[source].m_Level == 1 ||		// ��������
		lpObj->pInventory[source].m_Level == 2 ||		// Ǫ������
		lpObj->pInventory[source].m_Level == 3 ||		// ��������
		lpObj->pInventory[source].m_Level == 4 ||		// ������ ����
		lpObj->pInventory[source].m_Level == 5			// ��¦���� (�������� ��ǰ)
		))
	{
		return 0xFF;
	}
#endif

#ifdef KANTURU_MOONSTONEPENDANT_20060804	// ���������Ʈ - �ŷ� ����	20060823�߰�
	if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 38 ) )
	{	
		return 0xFF;
	}
#endif

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// �ŷ� �Ұ�
	if( lpObj->pInventory[source].m_Type == MAKE_ITEMNUM( 13, 39 ) )
	{	
		return 0xFF;
	}
#endif

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	if( IsCashItem( lpObj->pInventory[source].m_Type ) == TRUE )
	{
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		switch(lpObj->pInventory[source].m_Type)
		{
			//ĳ�� �������̶� �Ʒ� �������� �ŷ� �����ϰ� �ش޶�� �Ϻ� ��û 2009/04/07 by natinda
			case MAKE_ITEMNUM2(13, 83):  // ��ź���� ����
				break;
			case MAKE_ITEMNUM2(13, 84):  // õ������ ����
				break;
			case MAKE_ITEMNUM2(13, 85):  // �������� ����
				break;
			case MAKE_ITEMNUM2(13, 86):  // ��ӳ��� ����
				break;
			case MAKE_ITEMNUM2(13, 87):  // �������� ����
				break;
			case MAKE_ITEMNUM2(13, 88):  // ���ﳯ�� ����
				break;
			case MAKE_ITEMNUM2(13, 89):  // ��ȥ���� ����
				break;
			case MAKE_ITEMNUM2(13, 90):  // ���ɳ��� ����
				break;
			case MAKE_ITEMNUM2(13, 91):  // �������� ����
				break;
			case MAKE_ITEMNUM2(13, 92):  // �������� ���� 
				break;
			default:
				return 0xFF;
		}
#else
		return 0xFF;
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PREMIUMITEM_20090106
	// �����̾� �������� �ŷ��Ұ�
	if( IsPremiumItem( lpObj->pInventory[source].m_Type ) == TRUE )
	{
		return 0xFF;
	}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117


#ifdef PCBANG_POINT_SYSTEM_20070206	// �ŷ� �Ұ�
	if( g_PCBangPointSystem.CheckPCBangPointItem( lpObj->pInventory[source].m_Type ) )
	{	
		return 0xFF;
	}
#endif

#ifdef MODIFY_QUEST_SYSTEM_20070525	// ����Ʈ������ �ŷ� �Ұ�
	if( lpObj->pInventory[source].m_QuestItem )
	{
		return 0xFF;
	}
#endif


	lpObj->pInventory[source].GetSize(iwidth, iheight);
	
	DWORD s_num = lpObj->pInventory[source].GetNumber();

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202	
	if( gObjCheckSerial0ItemList(&lpObj->pInventory[source]) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( lpObj->m_Index, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
		LogAddTD( "[ANTI-HACK][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID, lpObj->Name, lpObj->pInventory[source].GetName(), source );
		return 0xFF;
	}
#endif
	
	if( gObjInventorySearchSerialNumber(lpObj, s_num) == FALSE )
	{
		return 0xFF;
	}

	// �����Ѵٸ� Ʈ���̵忡 �����ִ´�.
	// Ʈ���̵��� ��ġ�� ����ִ��� üũ�Ѵ�.
	
	int blank;

	w = target%8;
	h = target/8;

	if( ExtentCheck(w, h, 8, 4) == FALSE ) return 0xFF;

	// ����ִ��� üũ
	if( *(lpObj->TradeMap+(h*8)+w) == 0xFF )
	{
		blank = gObjTradeRectCheck(lpObj->m_Index, w, h, iwidth,iheight);
		
		if( blank == 0xFF ) return 0xFF;
		else 	// �κ��丮�� �ڸ��� �ִٸ�
		{	
			lpObj->Trade[blank] = lpObj->pInventory[source];
			
//			LogAdd("Trade : %d inventory:%d", lpObj->Trade[blank].m_Number, lpObj->pInventory[source].m_Number);

			gObjInventoryDeleteItem(lpObj->m_Index, source);
			//gObjInventoryItemSet(lpObj->m_Index, source, 0xFF);
			//lpObj->pInventory[source].Clear();

			gObjTradeItemBoxSet(lpObj->m_Index, blank, iwidth, iheight, (BYTE)lpObj->Trade[blank].m_Type);
		
			BYTE itembuf[ITEM_BUFFER_SIZE];

			ItemByteConvert(itembuf, lpObj->Trade[blank]);
			
#ifdef ITEM_INDEX_EXTEND_20050706
			GCTradeOtherAdd(lpObj->TargetNumber, blank, itembuf);			
#else
			GCTradeOtherAdd(lpObj->TargetNumber, blank, itembuf[0],itembuf[1],itembuf[2], itembuf[3], itembuf[4]);			
#endif


#ifdef MODEFY_CHAGNE_RING_TRADE_INVENTORY_20070213
			if( source < MAX_EQUIPMENT ) 
			{				
				if( source == EQUIPMENT_RING_RIGHT || source == EQUIPMENT_RING_LEFT)
				{
					if( lpObj->Trade[blank].m_Type == MAKE_ITEMNUM(13,10)
						|| lpObj->Trade[blank].m_Type == MAKE_ITEMNUM(13,39)
						|| lpObj->Trade[blank].m_Type == MAKE_ITEMNUM(13,40)
						|| lpObj->Trade[blank].m_Type == MAKE_ITEMNUM(13,41)
#ifdef MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
						|| lpObj->Trade[blank].m_Type == MAKE_ITEMNUM(13,68)
						|| lpObj->Trade[blank].m_Type == MAKE_ITEMNUM(13,76)
#endif // MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
						)
					{						
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					}
				}				
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCEquipmentChange(lpObj->m_Index, source);
			}
#else	// MODEFY_CHAGNE_RING_TRADE_INVENTORY_20070213
			if( source < MAX_EQUIPMENT ) 
			{				
				if( source == EQUIPMENT_RING_RIGHT || source == EQUIPMENT_RING_LEFT)
				{
					if( lpObj->Trade[blank].m_Type == MAKE_ITEMNUM(13,10) )
					{						
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					}
				}				
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCEquipmentChange(lpObj->m_Index, source);
			}
#endif	// MODEFY_CHAGNE_RING_TRADE_INVENTORY_20070213
			return 0x01;
		}			
	}
	return 0xFF;
}

// Ʈ���̵忡�� Ʈ���̵�� �̵�
BYTE gObjTradeTradeMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target)
{
	int h, w;
	int iwidth, iheight;

	BYTE	TempTradeMap[4*8];				// ��ȯâ üũ�� ����

	if( source > MAX_TRADEITEM )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return 0xFF;
	}
	if( source == target ) 
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return 0xFF;
	}
	// �ű���� �������� �����ϴ°�?
	if( lpObj->Trade[source].IsItem() == FALSE )
	{
		return 0xFF;
	}
	// �ű���� ���� �������� �����ϴ°�?
	if( lpObj->Trade[target].IsItem() == TRUE )
	{
		return 0xFF;
	}
	if( lpObj->TargetNumber < 0 ) return 0xFF;
	
	// �ŷ��� ��ҵǰų� �ŷ����� �ƴ϶�� �������� �ű�� ����.
	if( (lpObj->m_IfState.use == 0) || ( lpObj->m_IfState.type != I_TRADE ))
	{
		return 0xFF;
	}
	
	lpObj->Trade[source].GetSize(iwidth, iheight);

	memcpy(TempTradeMap, lpObj->TradeMap, 4*8);
	// �ϴ��� �����.
	
	gObjTradeItemBoxSet(lpObj->m_Index, source, iwidth, iheight, 0xFF);

	// �����Ѵٸ� Ʈ���̵忡 �����ִ´�.
	// Ʈ���̵��� ��ġ�� ����ִ��� üũ�Ѵ�.
	int blank;

	w = target%8;
	h = target/8;

	if( ExtentCheck(w, h, 8, 4) == FALSE ) {
		memcpy(lpObj->TradeMap, TempTradeMap, 4*8);
		return 0xFF;
	}

	// ����ִ��� üũ
	if( *(lpObj->TradeMap+(h*8)+w) == 0xFF )
	{
		blank = gObjTradeRectCheck(lpObj->m_Index, w, h, iwidth,iheight);
		
		if( blank == 0xFF ) {
			memcpy(lpObj->TradeMap, TempTradeMap, 4*8);
			return 0xFF;
		}
		else 	// �κ��丮�� �ڸ��� �ִٸ�
		{	
			lpObj->Trade[blank] = lpObj->Trade[source];
			
			// �κ��丮�� ������ �����.			
			lpObj->Trade[source].Clear();
			GCTradeOtherDel(lpObj->TargetNumber, source);
			gObjTradeItemBoxSet(lpObj->m_Index, blank, iwidth, iheight, (BYTE)lpObj->Trade[blank].m_Type);

			BYTE itembuf[ITEM_BUFFER_SIZE];			

			ItemByteConvert(itembuf, lpObj->Trade[blank]);

#ifdef ITEM_INDEX_EXTEND_20050706
			GCTradeOtherAdd(lpObj->TargetNumber, blank, itembuf);
#else
			GCTradeOtherAdd(lpObj->TargetNumber, blank, itembuf[0],itembuf[1],itembuf[2], itembuf[3], itembuf[4]);
#endif


			return 0x01;
		}
	}
	memcpy(lpObj->TradeMap, TempTradeMap, 4*8);
	return 0xFF;
}
/*
// â���� â��� ������ �̵�
BOOL gObjTradeTradeMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target)
{
	int		h, w;
	int		iwidth, iheight;
	BYTE	TempTradeMap[4*8];				// ��ȯâ üũ�� ����

	// â�� �������� �ƴ϶�� �����Ų��.
	//if( lpObj->TargetNumber < 0 ) return 0x00;

	// �����ۿ� ������ �Ѿ����� üũ�Ѵ�.
	if( source > MAX_WAREHOUSEITEMS-1 )
	{
		LogAdd("error-L1 : %s %d", __FILE__, __LINE__);
		return FALSE;
	}
	if( source == target ) 
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		return FLASE;
	}	
	
	// �ű���� �������� �����ϴ°�?
	if( lpObj->pWarehouse[source].IsItem() == FALSE )
	{
		return FALSE;
	}
	
	// �ű���� ���� �������� �����ϴ°�?
	if( lpObj->pWarehouse[target].IsItem() == TRUE )
	{
		return FALSE;
	}	
	
	
	return 0x00;
}
*/


/*
void gObjTradeCancel(int aIndex)
{
	int number;

	if( gObj[aIndex].m_IfState.use != 1 ) return;

	number = gObj[aIndex].TargetNumber;
	if( number < 0 ) return;
	if( gObjIsConnected(number) == FALSE ) return;

	// ��ȯ���� ����� ���� �ƴϿ��ٸ�..
	if( gObj[number].TargetNumber != aIndex ) return;

	gObj[aIndex].TargetNumber  = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObj[number].TargetNumber  = -1;
	gObj[number].m_IfState.use = 0;

	gObjSetInventory1Pointer(&gObj[aIndex]);
	gObjSetInventory1Pointer(&gObj[number]);

	GCMoneySend(aIndex, gObj[aIndex].Money);
	GCMoneySend(number, gObj[number].Money);

	CGTradeResult(aIndex, 0x00);
	CGTradeResult(number, 0x00);
	GCItemListSend(aIndex);
	GCItemListSend(number);

	GCEquipmentSend(aIndex);
	GCEquipmentSend(number);
}
*/

// �ŷ��� ��ҽ�Ų��.
void gObjTradeCancel(int aIndex)
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT)  )
	{
		LogAdd("error : index error %s %d",__FILE__, __LINE__);
		return;
	}
	// ���� �÷������� �ƴ϶��.. �׳� ����..
	if( gObj[aIndex].Type != OBJTYPE_CHARACTER || gObj[aIndex].Connected != CSS_GAMEPLAYING ) 
	{
		LogAdd(lMsg.Get(535), gObj[aIndex].AccountID, gObj[aIndex].Name);
		return ;
	}	
	
#ifndef TRADE_SKILLBUG_FIX
	gObjInventoryRollback(aIndex);
#endif

	if( gObj[aIndex].m_IfState.use != 1 ) 
	{
		LogAdd(lMsg.Get(536), gObj[aIndex].AccountID, gObj[aIndex].Name);
		// �ŷ� Cancel �� [�ŷ���1]�� �ƴϾ ���� [%s][%s]
		// �ŷ� ��� �޽����� �Դµ�..  ���� �ŷ�â�� ������� �ƴϴ�.
		return;
	}
	if( gObj[aIndex].m_IfState.type != I_TRADE ) 
	{
		LogAdd(lMsg.Get(537),gObj[aIndex].AccountID, gObj[aIndex].Name);
		// �ŷ� Cancel �� [�ŷ���2]�� �ƴϾ ���� [%s][%s]
		return;	// Ʈ���̵� ���� �ƴϾ�����..
	}

#ifdef TRADE_SKILLBUG_FIX
	gObjInventoryRollback(aIndex);
#endif

	// Ʈ���̵� ���̾����� ��ҽ�Ų��.
	gObj[aIndex].TargetNumber  = -1;
	gObj[aIndex].m_IfState.use = 0;

#ifdef MODIFY_SOCKET_CHAOSMIX_ITEM_MOVE_BUGFIX_200801021
	gObj[aIndex].m_IfState.state = 0;
#endif	// MODIFY_SOCKET_CHAOSMIX_ITEM_MOVE_BUGFIX_200801021
	
	//int n;
	/*for( n=0; n<MAX_MAGIC; n++)
	{		
		gObj[aIndex].Magic[n] = gObj[aIndex].m_lpMagicBack[n];
	}
	*/
	gObjCharTradeClear(&gObj[aIndex]);
	//gObjSetInventory1Pointer(&gObj[aIndex]);
	GCMoneySend(aIndex, gObj[aIndex].Money);

	GCItemListSend(aIndex);
	GCEquipmentSend(aIndex);

	if( gObj[aIndex].Class == CLASS_KNIGHT )
	{
	}

	GCMagicListMultiSend(&gObj[aIndex]);
	gObjUseSkill.SkillChangeUse(aIndex);

#ifdef MODIFY_NEW_UNION_01_20041122
	// ��尡 ������..
	if( gObj[aIndex].GuildNumber > 0 )
	{
		GCGuildViewportNowPaint( aIndex, gObj[aIndex].GuildName, NULL, FALSE);

#ifdef MODIFY_NEW_UNION_02_20041201
		gObjNotifyUpdateUnionV1( &gObj[aIndex] );
		gObjNotifyUpdateUnionV2( &gObj[aIndex] );
#endif
		
	}
#endif

	LogAddTD(lMsg.Get(514),gObj[aIndex].AccountID, gObj[aIndex].Name);	// �ŷ����
}

// InventoryMap �� � ������ ��ŷ�Ѵ�.
// �߿�!!!!  ũ��Ƽ�� ������ �ɸ� ���¿��� �Լ�ȣ���ؾ� �Ѵ�.
void gObjTempInventoryItemBoxSet(BYTE * TempMap, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx, itemposy;

	itemposx = (itempos-MAX_EQUIPMENT)%8;
	itemposy = (itempos-MAX_EQUIPMENT)/8;

	for( int y=0; y<yl; y++)
	{
		for( int x=0; x<xl; x++)
		{
			*(TempMap+((itemposy+y)*8)+(itemposx+x)) = set_byte;
		}
	}
}


// � ������ �κ��丮�� ��� ���� �������� �ϴµ� ����ִ��� üũ
BYTE gObjTempInventoryRectCheck(BYTE * TempMap, int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;

	// ���μ��� ũ�⸦ �ʰ� �ߴٸ� 
	if( (sx+width) > 8 ) return (BYTE)0xFF;
	if( (sy+height) > 8 ) return (BYTE)0xFE;

	for( y=0; y<height; y++)
	{		
		for(x=0; x<width; x++)
		{
			if( *(TempMap+((sy+y)*8)+(sx+x)) != 0xFF ) 
			{
				blank++;
				return (BYTE)0xFF;
			}
		}
	}

	if( blank == 0 ) return (BYTE)(MAX_EQUIPMENT+((sy*8)+sx));

	return (BYTE)0xFF;
}


// Ÿ�԰� �ε���, ������ �־� �κ��丮�� �������� �߰��Ѵ�.
BYTE gObjTempInventoryInsertItem(LPOBJECTSTRUCT lpObj, CItem item, BYTE * TempMap)
{
	int w, h;
	int iwidth, iheight;
	BYTE blank=0;

	if( item.GetSize(iwidth, iheight) == FALSE )
	{
		LogAdd(lMsg.Get(532), __FILE__, __LINE__, item.m_Type);
		return 0xFF;
	}

	for( h=0; h<8; h++)
	{
		for( w=0; w<8; w++)
		{
			// ����ִ��� üũ
			if( *(TempMap+(h*8)+(w)) == 0xFF )
			{				
				blank = gObjTempInventoryRectCheck(TempMap, w, h, iwidth,iheight);
				// 
				if( blank == 0xFE ) goto GOTO_EndFunc;
				if( blank != 0xFF )	{	// �κ��丮�� �ڸ��� �ִٸ�
					gObjTempInventoryItemBoxSet(TempMap, blank, iwidth, iheight, (BYTE)item.m_Type);
					return blank;
				}			
			}
		}
	}

GOTO_EndFunc :
	return 0xFF;
}


// ����� ��ȯâ �������� �� �κ��丮�� �־ �Ǵ��� �׽�Ʈ �Ѵ�.
BOOL TradeItemInventoryPutTest(int aIndex)
{
	BYTE	TempInventoryMap[8][8];			// �κ��丮 ���� üũ�� ����
	int		number;
	int		result=TRUE;
	
	number = gObj[aIndex].TargetNumber;

	if( gObjIsConnected(number) == FALSE ) return FALSE;

	memcpy( TempInventoryMap, gObj[aIndex].pInventoryMap, 8*8);
	
	for( int n=0; n<MAX_TRADEITEM; n++)
	{
		// �������� �ִٸ�..
		if( gObj[number].Trade[n].IsItem() == TRUE )
		{
			if( gObjTempInventoryInsertItem(&gObj[aIndex], gObj[number].Trade[n], (LPBYTE)TempInventoryMap) == 0xFF )
			{
				return FALSE;
			}
			else {
				result = TRUE;
			}
		}
	}
	if( result == TRUE ) return TRUE;
	return FALSE;
}

BOOL TradeitemInventoryPut(int aIndex)
{
	int		number;
	BYTE	insert;
	char    szItemName[50]="Item";
	BYTE	ExOption[8];
	
	number = gObj[aIndex].TargetNumber;
	if( gObjIsConnected(number) == FALSE ) return FALSE;
	for( int n=0; n<MAX_TRADEITEM; n++)
	{
		// �������� �ִٸ�..
		if( gObj[number].Trade[n].IsItem() == TRUE )
		{
			insert = gObjInventoryInsertItem(aIndex, gObj[number].Trade[n]);
			if( insert != 0xFF )
			{
				strcpy(szItemName, gObj[number].Trade[n].GetName());

				//[%s][%s] Trade Item  Opposite :[%s][%s] (Item:%s/Serial:%d [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]

				ItemIsBufExOption(ExOption, &gObj[number].Trade[n]);

				LogAddTD(lMsg.Get(538), 
					gObj[number].AccountID, gObj[number].Name, 
					gObj[number].MapNumber, gObj[number].X, gObj[number].Y,
					gObj[aIndex].AccountID, gObj[aIndex].Name,
					gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y,
					szItemName, 
					gObj[number].Trade[n].m_Number, 
					gObj[number].Trade[n].m_Level,
					gObj[number].Trade[n].m_Option1, 
					gObj[number].Trade[n].m_Option2, 
					gObj[number].Trade[n].m_Option3,
					ExOption[0],
					ExOption[1],
					ExOption[2],
					ExOption[3],
					ExOption[4],
					ExOption[5],
					ExOption[6],
					gObj[number].Trade[n].m_SetOption
#ifdef ADD_SOCKET_OPTION_LOG_02_20080821
					, gObj[number].Trade[n].m_SocketOption[0],
					gObj[number].Trade[n].m_SocketOption[1],
					gObj[number].Trade[n].m_SocketOption[2],
					gObj[number].Trade[n].m_SocketOption[3],
					gObj[number].Trade[n].m_SocketOption[4]
#endif // ADD_SOCKET_OPTION_LOG_02_20080821
					);
			}
			else 
			{
				LogAdd("error : �̰� ū�ϳ�����!!");
				return FALSE;
			}
		}
	}
	return TRUE;
}

// OK ��ư �������� ó��
void gObjTradeOkButton(int aIndex)
{
	BYTE result=0x01;

	int number = gObj[aIndex].TargetNumber;
	if( number < 0 ) 
	{
		return;
	}
	if( gObjIsConnected(number) == FALSE ) return;

	if( gObj[number].TargetNumber != aIndex )
	{
		LogAdd(lMsg.Get(539), gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].TargetNumber, gObj[number].TargetNumber);
		return;
	}

	// �� �� �ѻ���̶� OK�� �ȴ��ȴٸ�.. ����
	if( gObj[aIndex].TradeOk != 1 || gObj[number].TradeOk != 1 )
	{
		return;
	}

	if( TradeItemInventoryPutTest(aIndex) == TRUE )
	{
		if( TradeItemInventoryPutTest(number) == TRUE )
		{			
			result = 0x01;
		}
		else result = 0x02;
	}
	else result = 0x02;

#ifdef MODIFY_ZEN_MAX_20040414
	// �� MAX �ʰ� üũ!
	if( !gObjCheckMaxZen( aIndex, gObj[number].TradeMoney ) )
	{
		result = 0x02;
	}

	if( !gObjCheckMaxZen( number, gObj[aIndex].TradeMoney ) )
	{
		result = 0x02;
	}
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// �ŷ��ÿ� ��ȭ�������� ������ ��ȯ���� �ʴ´�.
	if ( g_kJewelOfHarmonySystem.IsEnableToTrade( &gObj[number] ) == FALSE 
		|| g_kJewelOfHarmonySystem.IsEnableToTrade( &gObj[aIndex] ) == FALSE )
	{
		result = 0x04;				// ��ȭ �ŷ� ���� ��� �� 0x04
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		CGTradeResult(aIndex, result);
		CGTradeResult(number, result);
		return;
	}
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	
	// ������ ��ȯ ����
	if( result == 0x01 )
	{
		TradeitemInventoryPut(aIndex);
		TradeitemInventoryPut(number);
		gObjInventoryCommit(aIndex);
		gObjInventoryCommit(number);

		gObjMakePreviewCharSet(aIndex);
		gObjMakePreviewCharSet(number);

		gObj[aIndex].Money -= gObj[aIndex].TradeMoney;
		if( gObj[aIndex].Money < 0 ) gObj[aIndex].Money = 0;
		gObj[number].Money -= gObj[number].TradeMoney;
		if( gObj[number].Money < 0 ) gObj[number].Money = 0;
		gObj[aIndex].Money += gObj[number].TradeMoney;
		gObj[number].Money += gObj[aIndex].TradeMoney;
		LogAddTD(lMsg.Get(540), gObj[aIndex].AccountID,gObj[aIndex].Name, gObj[number].AccountID, gObj[number].Name, gObj[aIndex].TradeMoney, gObj[number].TradeMoney);
		GJSetCharacterInfo(&gObj[aIndex], aIndex);
		GJSetCharacterInfo(&gObj[number], number);
	}
	else {
		result = 0x02;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		CGTradeResult(aIndex, result);
		CGTradeResult(number, result);
		return;
	}

	gObj[aIndex].TargetNumber  = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObj[number].TargetNumber  = -1;
	gObj[number].m_IfState.use = 0;
	gObj[aIndex].TradeOk = 0;
	gObj[number].TradeOk = 0;
	gObj[aIndex].TradeMoney = 0;
	gObj[number].TradeMoney = 0;

#ifdef MODIFY_SOCKET_CHAOSMIX_ITEM_MOVE_BUGFIX_200801021
	gObj[aIndex].m_IfState.state = 0;
	gObj[number].m_IfState.state = 0;
#endif	// MODIFY_SOCKET_CHAOSMIX_ITEM_MOVE_BUGFIX_200801021

	GCMoneySend(aIndex, gObj[aIndex].Money);
	GCMoneySend(number, gObj[number].Money);

	CGTradeResult(aIndex, result);
	CGTradeResult(number, result);

	GCItemListSend(aIndex);
	GCItemListSend(number);
	
	for( int n=0; n<MAX_TRADEITEM; n++)
	{
		gObj[aIndex].Trade[n].Clear();
		gObj[number].Trade[n].Clear();
	}

#ifdef MODIFY_NEW_UNION_02_20041201
	gObjNotifyUpdateUnionV1( &gObj[aIndex] );
	gObjNotifyUpdateUnionV2( &gObj[aIndex] );
#endif
	
}

// ������ 0-3������ ������ ��ҽ�Ų��.
// �̰��� ����Ʈ�� �ٸ� ĳ������ ���(LEVEL)�� �����ٶ� ��Ʈ���� ���̱� ���� ���..
// ����!! : �̰��� ������ ����ȭ�� ���������� ó������ �ʴ´�.
BYTE LevelSmallConvert(int aIndex, int inventoryindex)
{
	int level = gObj[aIndex].pInventory[inventoryindex].m_Level;
	
#ifdef ITEM_12_13_20040401
	#ifdef DARKLORD_WORK
		if( level >= 13 ) return 7;
		else if( level == 12  ) return 6;
		else if( level == 11  ) return 5;
		else if( level >= 9 && level <= 10 ) return 4;
		else if( level >= 7 && level <= 8 ) return 3;
		else if( level >= 5 && level <= 6 ) return 2;
		else if( level >= 3 && level <= 4 ) return 1;
	#else	// #ifdef DARKLORD_WORK
		if( level >= 13 ) return 6;
		else if( level >= 11 && level <= 12 ) return 5;
		else if( level >= 9 && level <= 10 ) return 4;
		else if( level >= 7 && level <= 8 ) return 3;
		else if( level >= 5 && level <= 6 ) return 2;
		else if( level >= 3 && level <= 4 ) return 1;
	#endif	// #ifdef DARKLORD_WORK
#else
	if( level >= 11 ) return 7;
	else if( level == 10 ) return 6;
	else if( level == 9 ) return 5;
	else if( level == 8 ) return 4;
	else if( level == 7 ) return 3;
	else if( level >= 5 && level <= 6 ) return 2;
	else if( level >= 3 && level <= 4 ) return 1;
#endif

	return 0;
}

BYTE LevelSmallConvert(int level)
{
#ifdef ITEM_12_13_20040401
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ĳ���͸���Ʈ���� 13������ ���̰�
	if( level >= 13 ) return 7;
	else if( level == 12  ) return 6;
	else if( level == 11  ) return 5;
	else if( level >= 9 && level <= 10 ) return 4;
	else if( level >= 7 && level <= 8 ) return 3;
	else if( level >= 5 && level <= 6 ) return 2;
	else if( level >= 3 && level <= 4 ) return 1;
#else	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	if( level >= 13 ) return 6;
	else if( level >= 11 && level <= 12 ) return 5;
	else if( level >= 9 && level <= 10 ) return 4;
	else if( level >= 7 && level <= 8 ) return 3;
	else if( level >= 5 && level <= 6 ) return 2;
	else if( level >= 3 && level <= 4 ) return 1;
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
#else
	if( level >= 11 ) return 7;
	else if( level == 10 ) return 6;
	else if( level == 9 ) return 5;
	else if( level == 8 ) return 4;
	else if( level == 7 ) return 3;
	else if( level >= 5 && level <= 6 ) return 2;
	else if( level >= 3 && level <= 4 ) return 1;
#endif
	return 0;
}


// ����� ���ݵ����� ���� ����Ѵ�.
void gObjAttackDamageCalc(int aIndex)
{
	
}

// CharSet�� �ٸ� ����Ʈ�� ����ڵ鿡�� ���� ����� �����ֱ� ���� ����ȭ�� 
// �����̴�... ���� �Լ��� �� ���۸� �����ϴµ� ���ȴ�.
// ����!!!  ������ ����ȭ�� ���� ������ ����ؾ��Ѵ�.
void gObjMakePreviewCharSet(int aIndex)
{
	BYTE index;

	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return;
	}
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

#ifdef ITEM_INDEX_EXTEND_20050706
	memset(lpObj->CharSet, 0, sizeof(lpObj->CharSet));
#endif
	
	lpObj->CharSet[0]  = (lpObj->Class<<5)&0xE0;

#ifdef NEW_SKILL_FORSKYLAND	
	lpObj->CharSet[0] |= (lpObj->ChangeUP<<4)&0x10;
#else
	lpObj->CharSet[0] |= (lpObj->Skin<<4)&0x10;
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507
	lpObj->CharSet[0] |= ( lpObj->ThirdChangeUp << 3 )&0x08;
#endif

	
	if( lpObj->m_ActionNumber == AT_SIT1 )
		lpObj->CharSet[0] |= 2;
	else if( lpObj->m_ActionNumber == AT_POSE1 )
		lpObj->CharSet[0] |= 3;
	else lpObj->CharSet[0] |= 0;
	
#ifdef ITEM_INDEX_EXTEND_20050706
	// !!! . �߿�> ������� �ڵ尡 4096�� ���� �� ���ٴ� �����Ͽ� �ֻ��� 1��Ʈ�� ������� �ʴ´�.

	// ������ ����
	if( lpObj->pInventory[0].m_Type < 0 ) 
	{
		lpObj->CharSet[12] |= 0xF0;
		lpObj->CharSet[1] = 0xFF;
	}
	else 
	{
		// ���� 4��Ʈ (4096,2048,1024,512) ����
		lpObj->CharSet[12] |= (BYTE)((lpObj->pInventory[0].m_Type&0x0F00)>>4);
		// ���� 4��Ʈ ����
		lpObj->CharSet[1] = (BYTE)(lpObj->pInventory[0].m_Type&0x00FF);
	}

	// �޼� ����
	if( lpObj->pInventory[1].m_Type < 0 ) 
	{
		lpObj->CharSet[13] |= 0xF0;
		lpObj->CharSet[2] = 0xFF;
	}
	else 
	{
		// ���� 4��Ʈ (4096,2048,1024,512) ����
		lpObj->CharSet[13] |= (BYTE)((lpObj->pInventory[1].m_Type&0x0F00)>>4);
		// ���� 4��Ʈ ����
		lpObj->CharSet[2] = (BYTE)(lpObj->pInventory[1].m_Type&0x00FF);
	}

	// ���
	if( lpObj->pInventory[2].m_Type < 0 ) 
	{
		lpObj->CharSet[13] |= 0x0F;
		lpObj->CharSet[9] |= 0x80;
		lpObj->CharSet[3] |= 0xF0;
	}
	else 
	{
		// ���� 4��Ʈ (256,128,64,32) ����
		lpObj->CharSet[13] |= (BYTE)((lpObj->pInventory[2].m_Type&0x01E0)>>5);
		// ���� 5��° ��Ʈ (32) ����
		lpObj->CharSet[9] |= (BYTE)((lpObj->pInventory[2].m_Type&0x0010)<<3);
		// ���� 4��Ʈ ����
		lpObj->CharSet[3] |= (BYTE)((lpObj->pInventory[2].m_Type&0x000F)<<4);
	}
	
	// ����
	if( lpObj->pInventory[3].m_Type < 0 ) 
	{
		lpObj->CharSet[14] |= 0xF0;
		lpObj->CharSet[9] |= 0x40;
		lpObj->CharSet[3] |= 0x0F;
	}
	else 
	{
		// ���� 4��Ʈ (256,128,64,32) ����
		lpObj->CharSet[14] |= (BYTE)((lpObj->pInventory[3].m_Type&0x01E0)>>1);
		// ���� 5��° ��Ʈ (32) ����
		lpObj->CharSet[9] |= (BYTE)((lpObj->pInventory[3].m_Type&0x0010)<<2);
		// ���� 4��Ʈ ����
		lpObj->CharSet[3] |= (BYTE)((lpObj->pInventory[3].m_Type&0x000F));
	}

	// ����
	if( lpObj->pInventory[4].m_Type < 0 ) 
	{
		lpObj->CharSet[14] |= 0x0F;
		lpObj->CharSet[9] |= 0x20;
		lpObj->CharSet[4] |= 0xF0;
	}
	else 
	{
		// ���� 4��Ʈ (256,128,64,32) ����
		lpObj->CharSet[14] |= (BYTE)((lpObj->pInventory[4].m_Type&0x01E0)>>5);
		// ���� 5��° ��Ʈ (32) ����
		lpObj->CharSet[9] |= (BYTE)((lpObj->pInventory[4].m_Type&0x0010)<<1);
		// ���� 4��Ʈ ����
		lpObj->CharSet[4] |= (BYTE)((lpObj->pInventory[4].m_Type&0x000F)<<4);
	}

	// �尩
	if( lpObj->pInventory[5].m_Type < 0 ) 
	{
		lpObj->CharSet[15] |= 0xF0;
		lpObj->CharSet[9] |= 0x10;
		lpObj->CharSet[4] |= 0x0F;
	}
	else 
	{
		// ���� 4��Ʈ (256,128,64,32) ����
		lpObj->CharSet[15] |= (BYTE)((lpObj->pInventory[5].m_Type&0x01E0)>>1);
		// ���� 5��° ��Ʈ (32) ����
		lpObj->CharSet[9] |= (BYTE)((lpObj->pInventory[5].m_Type&0x0010));
		// ���� 4��Ʈ ����
		lpObj->CharSet[4] |= (BYTE)((lpObj->pInventory[5].m_Type&0x000F));
	}
	
	// �Ź�
	if( lpObj->pInventory[6].m_Type < 0 ) 
	{
		lpObj->CharSet[15] |= 0x0F;
		lpObj->CharSet[9] |= 0x08;
		lpObj->CharSet[5] |= 0xF0;
	}
	else 
	{
		// ���� 4��Ʈ (256,128,64,32) ����
		lpObj->CharSet[15] |= (BYTE)((lpObj->pInventory[6].m_Type&0x01E0)>>5);
		// ���� 5��° ��Ʈ (32) ����
		lpObj->CharSet[9] |= (BYTE)((lpObj->pInventory[6].m_Type&0x0010)>>1);
		// ���� 4��Ʈ ����
		lpObj->CharSet[5] |= (BYTE)((lpObj->pInventory[6].m_Type&0x000F)<<4);
	}
	
	index = 0;	// ����ó�� �������� �ʴ´�.
#else
	short TempType[10];
	if( lpObj->pInventory[0].m_Type < 0 )
		index = 0xFF;
	else index = (BYTE)lpObj->pInventory[0].m_Type;
	lpObj->CharSet[1] = index;

	if( lpObj->pInventory[1].m_Type < 0 )
		index = 0xFF;
	else index = (BYTE)lpObj->pInventory[1].m_Type;
	lpObj->CharSet[2] = index;

	TempType[2] = MAKE_ITEMNUM_INDEX(lpObj->pInventory[2].m_Type);
	if( lpObj->pInventory[2].m_Type < 0 ) TempType[2] = 31;
	TempType[3] = MAKE_ITEMNUM_INDEX(lpObj->pInventory[3].m_Type);
	if( lpObj->pInventory[3].m_Type < 0 ) TempType[3] = 31;
	TempType[4] = MAKE_ITEMNUM_INDEX(lpObj->pInventory[4].m_Type);
	if( lpObj->pInventory[4].m_Type < 0 ) TempType[4] = 31;
	TempType[5] = MAKE_ITEMNUM_INDEX(lpObj->pInventory[5].m_Type);
	if( lpObj->pInventory[5].m_Type < 0 ) TempType[5] = 31;
	TempType[6] = MAKE_ITEMNUM_INDEX(lpObj->pInventory[6].m_Type);
	if( lpObj->pInventory[6].m_Type < 0 ) TempType[6] = 31;

	
	lpObj->CharSet[3] = ((TempType[2]%16)<<4)+(TempType[3]%16);
	lpObj->CharSet[4] = ((TempType[4]%16)<<4)+(TempType[5]%16);
	index             = ((TempType[6]%16)<<4);
#endif
	
	// ������ ���۴� 2��Ʈ��
	if( lpObj->pInventory[7].m_Type < 0 ) index |= 0x0C;
	else index |=(lpObj->pInventory[7].m_Type&0x03)<<2;


#ifdef DARKLORD_WORK
	if( lpObj->pInventory[8].m_Type != MAKE_ITEMNUM(13,4) )
	{	// ��ũȣ���� �ƴϸ�
		if( lpObj->pInventory[8].m_Type < 0 )
		{
			index |= 0x03;
		}
//------------->
//20080930 ũ�������� �̺�Ʈ �絹Ǫ ��(grooving)
#ifdef ADD_XMASEVENT_PET_20080930
		else if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM( 13, 67 ) )//grooving
		{
			index |=(lpObj->pInventory[8].m_Type&0x03);
		}
#else //ADD_XMASEVENT_PET_20080930
		else
		{
			index |=(lpObj->pInventory[8].m_Type&0x03);
		}
#endif //ADD_XMASEVENT_PET_20080930
//<-------------
	}	
	else
	{
		index |= 0x03;
	}
#else
	if( lpObj->pInventory[8].m_Type < 0 ) index |= 0x03;
	else index |=(lpObj->pInventory[8].m_Type&0x03);
#endif	

#ifdef ITEM_INDEX_EXTEND_20050706
	lpObj->CharSet[5] |= index;
#else
	lpObj->CharSet[5] = index;
#endif
	
	int levelindex;

	levelindex  = ((int)LevelSmallConvert(aIndex, 0));
	levelindex |= ((int)LevelSmallConvert(aIndex, 1)<<3);
	levelindex |= ((int)LevelSmallConvert(aIndex, 2)<<6);
	levelindex |= ((int)LevelSmallConvert(aIndex, 3)<<9);
	levelindex |= ((int)LevelSmallConvert(aIndex, 4)<<12);
	levelindex |= ((int)LevelSmallConvert(aIndex, 5)<<15);
	levelindex |= ((int)LevelSmallConvert(aIndex, 6)<<18);
		
	lpObj->CharSet[6] = (BYTE)((levelindex>>16)&255);
	lpObj->CharSet[7] = (BYTE)((levelindex>> 8)&255);
	lpObj->CharSet[8] = (BYTE)((levelindex    )&255);
	 
#ifndef ITEM_INDEX_EXTEND_20050706		// ������ Ȯ���Ʈ�� ����ϴ� �κ� ����
	lpObj->CharSet[9] = 0;
	if( TempType[2] > 15 ) lpObj->CharSet[9]  = 0x80;
	if( TempType[3] > 15 ) lpObj->CharSet[9] |= 0x40;
	if( TempType[4] > 15 ) lpObj->CharSet[9] |= 0x20;
	if( TempType[5] > 15 ) lpObj->CharSet[9] |= 0x10;
	if( TempType[6] > 15 ) lpObj->CharSet[9] |= 0x08;
#endif

#ifdef NEW_FORSKYLAND2
	// ����....
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1,2�� ���� gObjMakePreviewCharSet �� �߰� 
	if( (lpObj->pInventory[7].m_Type >= MAKE_ITEMNUM(12,3) && lpObj->pInventory[7].m_Type <= MAKE_ITEMNUM(12,6)) ||
			lpObj->pInventory[7].m_Type == MAKE_ITEMNUM(13,30) 	// ���� �߰�
			|| lpObj->pInventory[7].m_Type == MAKE_ITEMNUM(12,41)	// ����� ���� 
			|| lpObj->pInventory[7].m_Type == MAKE_ITEMNUM(12,42)	// ������ ����
			)
#else // ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	#ifdef DARKLORD_WORK
		if( (lpObj->pInventory[7].m_Type >= MAKE_ITEMNUM(12,3) && lpObj->pInventory[7].m_Type <= MAKE_ITEMNUM(12,6)) ||
			lpObj->pInventory[7].m_Type == MAKE_ITEMNUM(13,30) )	// ���� �߰�
	#else	// ���� ����(0x07���� ����ŷ�� �ǹǷ� ���� ���� ������ �ΰ� �� ���� ����)
		if( lpObj->pInventory[7].m_Type >= MAKE_ITEMNUM(12,3) && lpObj->pInventory[7].m_Type <= MAKE_ITEMNUM(12,6))
	#endif	// #ifdef DARKLORD_WORK
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	{
		lpObj->CharSet[5] |= 0x0c;

#ifdef DARKLORD_WORK
		if( lpObj->pInventory[7].m_Type == MAKE_ITEMNUM(13,30) )
			lpObj->CharSet[9] |= 0x05;
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1,2�� ���� gObjMakePreviewCharSet �� �߰� 
		else if( lpObj->pInventory[7].m_Type == MAKE_ITEMNUM(12,41) )
		{
			lpObj->CharSet[9] |= 0x06;
		}
		else if( lpObj->pInventory[7].m_Type == MAKE_ITEMNUM(12,42) )
		{
			lpObj->CharSet[9] |= 0x07;
		}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		else
#endif
			lpObj->CharSet[9] |= ((lpObj->pInventory[7].m_Type-2)&0x7);
	}
#else
	#ifdef NEW_SKILL_FORSKYLAND
		// �䰡�þƿ�...
		if( lpObj->pInventory[8].m_Type & 0x03 && lpObj->pInventory[8].m_Type > 0 )
		{
			lpObj->CharSet[9] |= (BYTE)0x04;
		}	
	#endif
#endif

	lpObj->CharSet[10] = 0;
	if( lpObj->pInventory[EQUIPMENT_HELM  ].IsExtItem()) lpObj->CharSet[10] = 0x80;
	if( lpObj->pInventory[EQUIPMENT_ARMOR ].IsExtItem()) lpObj->CharSet[10] |= 0x40;
	if( lpObj->pInventory[EQUIPMENT_PANTS ].IsExtItem()) lpObj->CharSet[10] |= 0x20;
	if( lpObj->pInventory[EQUIPMENT_GLOVES].IsExtItem()) lpObj->CharSet[10] |= 0x10;
	if( lpObj->pInventory[EQUIPMENT_BOOTS ].IsExtItem()) lpObj->CharSet[10] |= 0x08;
	if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].IsExtItem()) lpObj->CharSet[10] |= 0x04;
	if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT ].IsExtItem()) lpObj->CharSet[10] |= 0x02;

	lpObj->CharSet[11] = 0;
	if( lpObj->pInventory[EQUIPMENT_HELM  ].IsSetItem()) lpObj->CharSet[11] = 0x80;
	if( lpObj->pInventory[EQUIPMENT_ARMOR ].IsSetItem()) lpObj->CharSet[11] |= 0x40;
	if( lpObj->pInventory[EQUIPMENT_PANTS ].IsSetItem()) lpObj->CharSet[11] |= 0x20;
	if( lpObj->pInventory[EQUIPMENT_GLOVES].IsSetItem()) lpObj->CharSet[11] |= 0x10;
	if( lpObj->pInventory[EQUIPMENT_BOOTS ].IsSetItem()) lpObj->CharSet[11] |= 0x08;
	if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].IsSetItem()) lpObj->CharSet[11] |= 0x04;
	if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT ].IsSetItem()) lpObj->CharSet[11] |= 0x02;

	gObjCalCharacter(aIndex);

	if( lpObj->IsFullSetItem )
	{	// Ǯ��
		lpObj->CharSet[11] |= (BYTE)0x01;
	}

#ifdef NEW_FORSKYLAND2
	// �䰡�þƿ�...
	if( lpObj->pInventory[8].m_Type & 0x03 && lpObj->pInventory[8].m_Type > 0 )
	{
		lpObj->CharSet[10] |= (BYTE)0x01;
	}
#endif

#ifdef DARKLORD_WORK
#ifndef ITEM_INDEX_EXTEND_20050706
	lpObj->CharSet[12] = 0;
#endif	
	if( lpObj->pInventory[8].m_Type == MAKE_ITEMNUM(13,4) )
	{	// ��ũȣ��
		lpObj->CharSet[12] |= (BYTE)0x01;
	}
#endif

#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// �߱� �̺�Ʈ - ������ ���� 
//	lpObj->CharSet[12] = 0;
	if( (
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level == 3 &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability > 0.f
		) ||
		(
		lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level == 3 &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability > 0.f
		) )
	{	
		// �� ���� �� �տ� ������ ������ ���� �ִٸ�
		lpObj->CharSet[12] |= (BYTE)0x02;
	}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
	// HELPER�� �渱�� ���
	if( lpObj->pInventory[8].m_Type == MAKE_ITEMNUM(13,37) )
	{
		lpObj->CharSet[10] &= ~ (BYTE)0x01; // ����Ʈ �ƴ�
		lpObj->CharSet[12] &= ~ (BYTE)0x01; // ��ũȣ�� �ƴ�
		lpObj->CharSet[12] |=   (BYTE)0x04; // �渱

		// ���� ����Ʈ 
		// CharSet[16], CharSet[17] ��
		// CharSet[16]�� ����Ѵ�.

		lpObj->CharSet[16] = 0;
		lpObj->CharSet[17] = 0;

		if( lpObj->pInventory[8].IsFenrirIncLastAttackDamage() )
		{
			lpObj->CharSet[16] |= (BYTE)0x01;	// �渱 - ���� ������ ����
		}

		if( lpObj->pInventory[8].IsFenrirDecLastAttackDamage() )
		{
			lpObj->CharSet[16] |= (BYTE)0x02;	// �渱 - ���� ������ ����
		}
		
#ifdef ADD_ILLUSION_FENRIR_20070703	// (CharSet[17]�� 2��° Bit)
		if( lpObj->pInventory[8].IsIllusionFenrirOption() )
		{
			lpObj->CharSet[17] |= (BYTE)0x01;	// ȯ���渱 �ɼ�
		}
#endif
	}	
#endif // ADD_ITEM_FENRIR_01_20051110

	// 3�� ����
#ifdef ADD_THIRD_WING_20070525	// gObjMakePreviewCharSet 3������ �߰�
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130 // gObjMakePreviewCharSet�� ������ ����
	if(( lpObj->pInventory[7].m_Type >= MAKE_ITEMNUM( 12, 36 ) && lpObj->pInventory[7].m_Type <= MAKE_ITEMNUM( 12, 40 ) )
		|| lpObj->pInventory[7].m_Type == MAKE_ITEMNUM( 12, 43 ) )
#else
	if( lpObj->pInventory[7].m_Type >= MAKE_ITEMNUM( 12, 36 ) && lpObj->pInventory[7].m_Type <= MAKE_ITEMNUM( 12, 40 ) )
#endif
	{
		lpObj->CharSet[5] |= 0x0C;	// 1�� ���� �ʱ�ȭ
		lpObj->CharSet[16] |= ( (( lpObj->pInventory[7].m_Type - 35 )&0x7 ) << 2 );
	#ifdef MODIFY_SUMMONER_THIRD_WING_BUFGIX_20090109	// ������ ������ 0x06���� ����
		if(lpObj->pInventory[7].m_Type == MAKE_ITEMNUM( 12, 43 ) )
		{
		lpObj->CharSet[16] |= ( (( 0x06 )&0x7 ) << 2 );
		}
	#endif	// MODIFY_SUMMONER_THIRD_WING_BUFGIX_20090109
	}
#endif // ADD_THIRD_WING_20070525

#ifdef ADD_SAFEGUARD_ITEM_20080702
	{
		BYTE btItemIndex = 0;
		switch(lpObj->pInventory[EQUIPMENT_HELPER].m_Type)
		{
		case MAKE_ITEMNUM2( 13, 64 )://����
			{
				btItemIndex = (BYTE)(0x01 << 5);
				break;
			}
		case MAKE_ITEMNUM2( 13, 65 )://��ȣ����
			{
				btItemIndex = (BYTE)(0x02 << 5);
				break;
			}
//------------->
//20080930 ũ�������� �̺�Ʈ �絹Ǫ ��(grooving)
#ifdef ADD_XMASEVENT_PET_20080930
		case MAKE_ITEMNUM2( 13, 67 )://
			{
				btItemIndex = (BYTE)(0x04 << 5);
				break;
			}
#endif // ADD_XMASEVENT_PET_20080930
//<-------------
#ifdef ADD_UNICORN_PET_20090915
		case MAKE_ITEMNUM2( 13, 106 ):// ������ ��
			{
				btItemIndex = (BYTE)(0x05 << 5);
				break;
			}
#endif // ADD_UNICORN_PET_20090915
#ifdef ADD_PREMIUMITEM_PANDA
		case MAKE_ITEMNUM2( 13, 80 ):// �Ҵ���
			{
				btItemIndex = (BYTE)(0x07 << 5);
				break;
			}
#endif // ADD_PREMIUMITEM_PANDA
		}		
		lpObj->CharSet[16] |= btItemIndex;
	}
#endif // ADD_SAFEGUARD_ITEM_20080702
}


#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
#endif

void gObjViewportPaint(HWND hWnd, short aIndex)
{
	int n;
	HDC hdc;
	char szTemp[256];
	int count=0, count2=0, count3=0, playerc=0, totalplayer=0;
	//RECT rect;

	if( !CHECK_LIMIT(aIndex, MAX_OBJECT ))
	{
		
		return;
	}

	hdc = GetDC(hWnd);
	
/*
	for( n=0; n<MAXVIEWPORTOBJECT; n++)
	{
		if( gObj[aIndex].VpPlayer[n].state )
		{
			count3++;
		}
	}
*/

/*
	for( n=0; n<MAXVIEWPORTOBJECT; n++)
	{
		if( gObj[aIndex].VpPlayer2[n].state )
		//if( gObj[aIndex].VpPlayer[n].number != -1 )
		{
			number = gObj[aIndex].VpPlayer2[n].number;
			rect.left = 400+gObj[number].X;
			rect.right= 400+gObj[number].X+3;
			rect.top  = 50+gObj[number].Y;			
			rect.bottom= 50+gObj[number].Y+3;
			FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
			wsprintf(szTemp, "%d", n);
			TextOut(hdc, 400, 0, szTemp, strlen(szTemp));
			
			count2++;
		}
	}	
*/

/*
	if( gCurPaintType == 1 )
	{
		COLORREF prgb;
		int x, y;
		
		prgb = RGB(210,210,210);
		if( MapC[gCurPaintMapNumber].m_attrbuf != NULL )
		{
			for( y=0;y<255;y++)
			{
				for( x=0;x<255;x++)
				{
					if( //MapC[gCurPaintMapNumber].m_attrbuf[(y*MAX_TERRAIN_SIZE)+x])	
						//(MapC[gCurPaintMapNumber].m_attrbuf[(y*MAX_TERRAIN_SIZE)+x]&MAP_ATTR_SAFTYZONE) == MAP_ATTR_SAFTYZONE )
						(MapC[gCurPaintMapNumber].m_attrbuf[(y*MAX_TERRAIN_SIZE)+x]&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK ||
						(MapC[gCurPaintMapNumber].m_attrbuf[(y*MAX_TERRAIN_SIZE)+x]&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW )
					{
						rect.left = 400+y;
						rect.right= 400+y+2;
						rect.top  = 51+x;
						rect.bottom= 51+x+2;
						
						//FillRect(hdc, &rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
						SetPixel(hdc, rect.left, rect.top, prgb);
					}
				}
			}
		}
	}	
	else
	{
		COLORREF prgb;
				
		prgb = RGB(255,0,0);
		for( int n=0; n<MAX_MAPITEM; n++)
		{
			// ����ִ� ������ ã�Ƽ�..		
			// �ð��� ���� �������� �����Ѵ�.
			if( MapC[gCurPaintMapNumber].m_cItem[n].IsItem() )
			{
				rect.left = 400+MapC[gCurPaintMapNumber].m_cItem[n].py;
				rect.right= 400+MapC[gCurPaintMapNumber].m_cItem[n].py+2;
				rect.top  = 51+MapC[gCurPaintMapNumber].m_cItem[n].px;
				rect.bottom= 51+MapC[gCurPaintMapNumber].m_cItem[n].px+2;
						
				//FillRect(hdc, &rect, (HBRUSH)RGB(255,0,0));//(HBRUSH)GetStockObject(LTGRAY_BRUSH));
				SetPixel(hdc, rect.left, rect.top, prgb);
				SetPixel(hdc, rect.left+1, rect.top, prgb);
				SetPixel(hdc, rect.left, rect.top+1, prgb);
				SetPixel(hdc, rect.left+1, rect.top+1, prgb);
			}
		}
	}
*/



	for( n=0; n<MAX_OBJECT; n++)
	{	
		if( gObj[n].Live && gObj[n].Connected && gCurPaintMapNumber == gObj[n].MapNumber)
		{				
			if( gObj[n].Type == OBJTYPE_CHARACTER )	
			{				
				
				//rect.left  = 400+gObj[n].Y;
				//rect.right = 400+gObj[n].Y+4;
				//rect.top   = 51+gObj[n].X;			
				//rect.bottom= 51+gObj[n].X+4;
				//FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
				
				playerc++;
			}
			else 
			{
				
				//FillRect(hdc, &rect, (HBRUSH)GetStockObject(GRAY_BRUSH));
				//if( gObj[n].m_RecallMon >= 0 )
				{
				//	rect.left  = 400+gObj[n].Y;
				//	rect.right = 400+gObj[n].Y+2;
				//	rect.top   = 51+gObj[n].X;			
				//	rect.bottom= 51+gObj[n].X+2;			
				//	FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
				}
								
			}
			
		}
		if( gObj[n].Type == OBJTYPE_CHARACTER && gObj[n].Connected )
		{
			totalplayer ++;
		}
		else if( gObj[n].Connected )
		{
			count++;
		}
	}
	


/*
	for( y=0; y<8; y++)
	{
		for( x=0; x<8; x++)
		{
			if( gObj[274].InventoryMap[y][x] == 0xFF )
			{
				rect.left = 350+(x*4);
				rect.right= 350+(x*4)+2;
				rect.top  = 51+(y*4);
				rect.bottom= 51+(y*4)+2;
				FillRect(hdc, &rect, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
			}
			else FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		}
	}
*/

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
// Ŭ������ ������ ����!

	if( gCurPaintType == 1 )
	{
		RECT rect;

		int iStartX		= 100;
		int iStartY		= 50;
		int iWidth		= 1;
		int iHeight		= 1;
		int iMagnifying = 3;

		hdc = GetDC(hWnd);
		
		// 1. > �� �׸���
		int x, y;

		HBRUSH hCharacterBrush	= CreateSolidBrush( RGB(255,0,0) );
		HBRUSH hMonsterBrush	= CreateSolidBrush( RGB(128,128,128) );
		HBRUSH hNpcBrush		= CreateSolidBrush( RGB(0,255,255) );
		HBRUSH hItemBrush		= CreateSolidBrush( RGB(0,0,255) );
		HBRUSH hCrywolfMovePath	= CreateSolidBrush( RGB(0,255,0) );
		


		int iOldBkMode = SetBkMode(hdc, TRANSPARENT); 

		if( MapC[gCurPaintMapNumber].m_attrbuf != NULL )
		{
			for( y=0;y<255;y++)
			{
				for( x=0;x<255;x++)
				{
					if( (MapC[gCurPaintMapNumber].m_attrbuf[(y*MAX_TERRAIN_SIZE)+x]&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK 
						|| (MapC[gCurPaintMapNumber].m_attrbuf[(y*MAX_TERRAIN_SIZE)+x]&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW )
					{
						rect.left	= iStartX  + y * iWidth * iMagnifying;
						rect.right	= iStartX  + y * iWidth * iMagnifying + iWidth * iMagnifying;

						rect.top	= iStartY  + x * iHeight * iMagnifying;
						rect.bottom = iStartY  + x * iHeight * iMagnifying + iHeight * iMagnifying;

						
						FillRect(hdc, &rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
					}
				}
			}
		}
		// 4. > ũ���̿��� ���� ��� ���� �̵� ��� �׸���
		
		if( TMonsterAIElement::s_MonsterAIMovePath != NULL && TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].m_bDataLoad )
		{
			TMonsterAIMovePath &MovePath = TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE];	

			for( int i=0; i<MovePath.m_iMovePathSpotCount; i++ )
			{
				rect.left	= iStartX  + MovePath.m_MovePathInfo[i].m_iPathY * iWidth * iMagnifying;
				rect.right	= iStartX  + MovePath.m_MovePathInfo[i].m_iPathY * iWidth * iMagnifying + iWidth * iMagnifying;
				
				rect.top	= iStartY  + MovePath.m_MovePathInfo[i].m_iPathX * iHeight * iMagnifying;
				rect.bottom = iStartY  + MovePath.m_MovePathInfo[i].m_iPathX * iHeight * iMagnifying + iHeight * iMagnifying;			
				FillRect(hdc, &rect, hCrywolfMovePath );

				CString szDesc;
				szDesc.Format("(%d,%d)", MovePath.m_MovePathInfo[i].m_iPathX, MovePath.m_MovePathInfo[i].m_iPathY );
				//TextOut( hdc, rect.left, rect.bottom, szDesc, szDesc.GetLength());
			}
			
		}

		// 4. > ũ���̿��� ���� ��� ���� �̵� ��� �׸���
		
	

		// 2. > OBJECT �׸���
		for( n=0; n<MAX_OBJECT; n++)
		{		
			if( gObj[n].Live && gObj[n].Connected && gCurPaintMapNumber == gObj[n].MapNumber)
			{				
				if( gObj[n].Type == OBJTYPE_CHARACTER )	
				{				
					rect.left	= iStartX  + gObj[n].Y * iWidth * iMagnifying;
					rect.right	= iStartX  + gObj[n].Y * iWidth * iMagnifying + (iWidth+2) * iMagnifying;

					rect.top	= iStartY  + gObj[n].X * iHeight * iMagnifying;
					rect.bottom = iStartY  + gObj[n].X * iHeight * iMagnifying + (iHeight+2) * iMagnifying;

					FillRect(hdc, &rect, hCharacterBrush );
					CString szName;
					szName.Format("%s (%d,%d)", gObj[n].Name, gObj[n].X, gObj[n].Y );
					TextOut( hdc, rect.left, rect.bottom, szName, szName.GetLength());
					
				}
				else if( gObj[n].Type == OBJTYPE_MONSTER )
				{
					rect.left	= iStartX  + gObj[n].Y * iWidth * iMagnifying;
					rect.right	= iStartX  + gObj[n].Y * iWidth * iMagnifying + iWidth * iMagnifying;

					rect.top	= iStartY  + gObj[n].X * iHeight * iMagnifying;
					rect.bottom = iStartY  + gObj[n].X * iHeight * iMagnifying + iHeight * iMagnifying;			

					FillRect(hdc, &rect, hMonsterBrush );

					if( gObj[n].m_iCurrentAI )
					{
						// ������ ���
						if( gObj[n].m_iGroupMemberGuid == 0 )
						{
							FillRect(hdc, &rect, hCharacterBrush );
							CString szDesc;
							//szDesc.Format("%d (%d,%d)", gObj[n].m_Index, gObj[n].X, gObj[n].Y );
							szDesc.Format("G(%d,%d)", gObj[n].m_iGroupNumber, gObj[n].m_iGroupMemberGuid );
							TextOut( hdc, rect.left, rect.bottom, szDesc, szDesc.GetLength());
						}
						
					}
				
				}
				else if( gObj[n].Type == OBJTYPE_NPC )
				{
					rect.left	= iStartX  + gObj[n].Y * iWidth * iMagnifying;
					rect.right	= iStartX  + gObj[n].Y * iWidth * iMagnifying + iWidth * iMagnifying;

					rect.top	= iStartY  + gObj[n].X * iHeight * iMagnifying;
					rect.bottom = iStartY  + gObj[n].X * iHeight * iMagnifying + iHeight * iMagnifying;			
					FillRect(hdc, &rect, hNpcBrush );
				}
				
			}
		}

		// 3. > Ư�� ������ ���������� �׸���
		for( int n=0; n<MAX_MAPITEM; n++)
		{
			if( MapC[gCurPaintMapNumber].m_cItem[n].IsItem() )
			{
				rect.left	= iStartX  + MapC[gCurPaintMapNumber].m_cItem[n].py * iWidth * iMagnifying;
				rect.right	= iStartX  + MapC[gCurPaintMapNumber].m_cItem[n].py * iWidth * iMagnifying + iWidth * iMagnifying;
				
				rect.top	= iStartY  + MapC[gCurPaintMapNumber].m_cItem[n].px * iHeight * iMagnifying;
				rect.bottom = iStartY  + MapC[gCurPaintMapNumber].m_cItem[n].px * iHeight * iMagnifying + iHeight * iMagnifying;			
				FillRect(hdc, &rect, hItemBrush );
				CString szDesc;
				szDesc.Format("%s (%d,%d)", 
					MapC[gCurPaintMapNumber].m_cItem[n].GetName(),
					MapC[gCurPaintMapNumber].m_cItem[n].px,
					MapC[gCurPaintMapNumber].m_cItem[n].py	);
				//TextOut( hdc, rect.left, rect.bottom, szDesc, szDesc.GetLength());
				
			}
		}

		


		DeleteObject( hCharacterBrush );
		DeleteObject( hMonsterBrush );
		DeleteObject( hNpcBrush );
		DeleteObject( hItemBrush );
		DeleteObject( hCrywolfMovePath );

		SetBkMode(hdc, iOldBkMode);

	}
#endif


	gObjTotalUser = totalplayer;
	//wsprintf(szTemp, "recv:%10d  send:%10d  COUNT:%d  VpCount:%d/%d", wsGServer.m_RecvSec,wsGServer.m_SendSec, count, gObj[0].VPCount, count2);

	wsprintf(szTemp, "COUNT:%d  TotalPlayer : %d  Player(%d):%d VpCount:%d(%d/%d) : item count:%d max:%d", count, totalplayer, aIndex, playerc, gObj[aIndex].VPCount, count3, count2, gItemLoop,gServerMaxUser);

	if( gXMasEvent )
	{
		strcat(szTemp, ":StarOfXMas");
	}

	if( gFireCrackerEvent )
	{
		strcat(szTemp, ":FireCracker");
	}	

	if( gHeartOfLoveEvent )
	{
		strcat(szTemp, ":HeartOfLove");
	}

	if( gMedalEvent )
	{
		strcat(szTemp, ":MedalEvent");
	}

	TextOut(hdc, 200, 0, szTemp, strlen(szTemp));
	ReleaseDC(hWnd, hdc);
}


static int FrustrumX[4];
static int FrustrumY[4];

//----------------------------------------------------------------------------
// x,y �߽����� Frustrum�� �����Ѵ�.
void InitFrustrum()
{
	
	float CameraViewFar    = 2400.f;	// ���� ��
	float CameraViewNear   = CameraViewFar*0.19f;// �Ʒ� ��
	float CameraViewTarget = CameraViewFar*0.53f;// ĳ������ �߾� ��ġ
	float WidthFar  = 1190.f;// ������ ���� ��
	float WidthNear = 550.f;// �Ʒ����� ���� ��
		
	vec3_t p[4];
	Vector(-WidthFar ,CameraViewFar -CameraViewTarget,0.f,p[0]);
	Vector( WidthFar ,CameraViewFar -CameraViewTarget,0.f,p[1]);
	Vector( WidthNear,CameraViewNear-CameraViewTarget,0.f,p[2]);
	Vector(-WidthNear,CameraViewNear-CameraViewTarget,0.f,p[3]);
	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f,0.f,45.f,Angle);
	AngleMatrix(Angle,Matrix);
    vec3_t Frustrum[4];
	for(int i=0;i<4;i++)
	{
		VectorRotate(p[i],Matrix,Frustrum[i]);
		FrustrumX[i] = (int)(Frustrum[i][0]*0.01f);
		FrustrumY[i] = (int)(Frustrum[i][1]*0.01f);
	}
}

void CreateFrustrum(int x,int y, int aIndex)
{
	LPOBJECTSTRUCT lpObj=(LPOBJECTSTRUCT)&gObj[aIndex];
	
	for(int i=0;i<4;i++)
	{
		lpObj->FrustrumX[i] = FrustrumX[i] + x;
		lpObj->FrustrumY[i] = FrustrumY[i] + y;
	}
}

// 
bool TestFrustrum2(int x,int y, int aIndex)
{
	int j = 3;
	LPOBJECTSTRUCT lpObj=(LPOBJECTSTRUCT)&gObj[aIndex];
	for(int i=0;i<4;j=i,i++)
	{
		int d = (lpObj->FrustrumX[i]-x) * (lpObj->FrustrumY[j]-y) -
	      		(lpObj->FrustrumX[j]-x) * (lpObj->FrustrumY[i]-y);
		if(d < 0.f) return false;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// px,py�� ���� ����Ʈ�ȿ� �ִ��� üũ���ش�.
BOOL gObjCheckViewport(int aIndex, int x, int y)
{
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
			// �ϴ� �����ϰ� �簢 üũ��..

//#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	//return TRUE;
//#endif

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	if( (x < (lpObj->X-30)) || (x > (lpObj->X+30)) ||
		(y < (lpObj->Y-30)) || (y > (lpObj->Y+30)) )
#else
	if( (x < (lpObj->X-15)) || (x > (lpObj->X+15)) ||
		(y < (lpObj->Y-15)) || (y > (lpObj->Y+15)) )
#endif
	{
		return FALSE;
	}
	
	int j = 3;
	for(int i=0;i<4;j=i,i++)
	{
		int d = (lpObj->FrustrumX[i]-x) * (lpObj->FrustrumY[j]-y) -
			(lpObj->FrustrumX[j]-x) * (lpObj->FrustrumY[i]-y);
		if(d < 0) return FALSE;
	}
		
	return TRUE;
}

// ���� ���� ������� ����Ʈ���� ���� ������ �����Ų��.
void gObjViewportClose(LPOBJECTSTRUCT lpObj)
{
	int tObjNum;
	int i;

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	if( lpObj->Connected == CSS_GAMEPLAYING )	// ���� �������̾�����..
	{
		for( int n=0; n<MVL; n++) 
		{
			if( lpObj->VpPlayer2[n].state )
			{
				if( lpObj->VpPlayer2[n].type != OBJTYPE_ITEM ) 
				{
					// ���� ���� ������� ����Ʈ�� ���� ������ �����Ų��.
					tObjNum = lpObj->VpPlayer2[n].number;
					i = 0;
					while(1)
					{
						if( gObj[tObjNum].VpPlayer[i].number == lpObj->m_Index && gObj[tObjNum].VpPlayer[i].type == lpObj->Type )
						{
							gObj[tObjNum].VpPlayer[i].state  = STVP_DESTROY;
							break;
						}
						if(i< MVL-1 ) i++;
						else {
							#if _DEBUG
							LogAdd(lMsg.Get(541), lpObj->m_Index,tObjNum);
							#endif
							break;
						}
					}
					i = 0;
					while(1)
					{
						if( gObj[tObjNum].VpPlayer2[i].number == lpObj->m_Index && gObj[tObjNum].VpPlayer2[i].type == lpObj->Type )
						{
							gObj[tObjNum].VpPlayer2[i].state  = 0;
							break;
						}
						if(i< MVL-1 ) i++;
						else {
							#if _DEBUG
							LogAdd(lMsg.Get(541),lpObj->m_Index,tObjNum);
							#endif
							break;
						}
					}

				}
			}
		}
	}
	gObjClearViewport(lpObj);
}

//////////////////////////////////////////////////////////////////////////////
// ������Ʈ���� ����Ʈ�� ���� ����Ʈ�� �����Ѵ�.
void gObjViewportListCreate(short aIndex)
{
	int result;
	int n;
	LPOBJECTSTRUCT	lpObj;
	
	if(!CHECK_LIMIT(aIndex, MAX_OBJECT) ) return;

	lpObj = &gObj[aIndex];

	if( lpObj->Connected < CSS_GAMEPLAYING ) return;
	if( lpObj->RegenOk > 0 ) return;
	
	int mapnum = lpObj->MapNumber;
	
	gItemLoop2 = 0;
	
	
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		MapClass * lpMapC = &MapC[mapnum];;
		// ������ üũ
		for( n=0; n<MAX_MAPITEM; n++)
		{
			if( lpMapC->m_cItem[n].live )
			{
				gItemLoop2++;
				if( lpMapC->m_cItem[n].m_State == OBJST_CREATE ||
					lpMapC->m_cItem[n].m_State == OBJST_PLAYING )
				{
					//
					// �������� ������ ��ó�� �ִ��� üũ�ؼ�
					if( gObjCheckViewport(aIndex, lpMapC->m_cItem[n].px, lpMapC->m_cItem[n].py) == TRUE )
					{
						// ���� ��ó�� �ִٸ� ������ ����Ʈ�� �߰��Ѵ�.
						result = ViewportAdd(aIndex, n, OBJTYPE_ITEM);
					}
				}
			}
		}
	}

	if( aIndex == 0 )
	{
	//if( gItemLoop2 > gItemLoopMax )
	//{
		gItemLoopMax    = gItemLoop2;
		gItemLoop       = gItemLoop2;
		gCurPaintPlayer = aIndex;
	}
	/*if( aIndex == gCurPaintPlayer )	
	{
		gItemLoop = gItemLoop2;
	}
	*/

	int a = 1;
	LPOBJECTSTRUCT	lpTempObj;
		// ������϶��� ��� ������Ʈ üũ
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		for( n=0; n<MAX_OBJECT; n++)
		{	
			lpTempObj = &gObj[n];
			if( (lpTempObj->Connected == CSS_GAMEPLAYING) &&  ( aIndex != n ) )
			{ // ������ �������̰�.. �ڽ��� ��..
				if( lpTempObj->m_State == OBJST_CREATE ||
					lpTempObj->m_State == OBJST_PLAYING )
				{
					if( mapnum == lpTempObj->MapNumber ) 
					{ // ���� �ʿ� �ִٸ�
						if( gObjCheckViewport(aIndex, lpTempObj->X, lpTempObj->Y) == TRUE )
						{ // �� ����Ʈ�� �ִٸ�...
							result = ViewportAdd(aIndex, n, lpTempObj->Type);
	#ifdef MODIFY_MOVEMAP_RESET_VIEWPORT_BUGFIX_20060511
							result = ViewportAdd2(n, aIndex, gObj[aIndex].Type);
	#endif
						}
					}
				}
			}			
		}// for
	}
	else if( lpObj->Type == OBJTYPE_MONSTER 
#ifdef GAMESERVER_TO_CASTLESIEGESERVER				// ������ ���뼭�� �� ���� NPC�� ����Ʈ�� ���´�.
			|| lpObj->Type == OBJTYPE_NPC
#endif
#ifdef CRYWOLF_NPC_WORK_20050912
			|| lpObj->Type == OBJTYPE_NPC			// ũ���̿��� ���� �� ���� NPC�� ����Ʈ�� ���´�.
#endif
		)
	{	
		for( n = MAX_MONSTER; n < MAX_OBJECT; n++ )
		{
			lpTempObj = &gObj[n];
			// ������ �������̰�.. �ڽ��� ��..						
			if( (lpTempObj->Connected == CSS_GAMEPLAYING) &&  ( aIndex != n ) )
			{	
				// ��ȯ�� �� �ϰ�� 
				if( lpTempObj->m_State == OBJST_CREATE ||
					lpTempObj->m_State == OBJST_PLAYING )
				{
					if( mapnum == lpTempObj->MapNumber )
					{ // ���� �ʿ� �ִٸ�
						if( gObjCheckViewport(aIndex, lpTempObj->X, lpTempObj->Y) == TRUE )
						{ // �� ����Ʈ�� �ִٸ�...
							result = ViewportAdd(aIndex, n, gObj[n].Type);
	#ifdef MODIFY_MOVEMAP_RESET_VIEWPORT_BUGFIX_20060511
							result = ViewportAdd2(n, aIndex, gObj[aIndex].Type );
	#endif
						}
					}
				}
			}
		}// for
	}
	
}

//----------------------------------------------------------------------------
// ���� ���� ȭ�鿡 ����ڸ� �߰��Ѵ�.
int ViewportAdd(int aIndex, int aAddIndex, int aType)
{
	int blank = -1;
	int count =  0;

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	LPOBJECTSTRUCT lpAddObj = &gObj[aAddIndex];

	if( (lpObj->m_stateEngagePVP == PVP_USER_PLAYING && lpAddObj->m_stateEngagePVP != PVP_USER_PLAYING) || (lpAddObj->m_stateEngagePVP == PVP_USER_PLAYING && lpObj->m_stateEngagePVP != PVP_USER_PLAYING) )
		return -1;

// 1. ���� ������ ��ϵǾ� �ִ� �������� �˾Ƴ���.
// 2. ��ϵǾ� �ִ� ������� �����Ѵ�.
// 3. ����� �ȵ� ������� ����Ѵ�.
	for( int n=0; n<MVL; n++)
	{
		if( lpObj->VpPlayer[n].state )
		{ // ������ ��ϵǾ��ִٸ�
			if( lpObj->VpPlayer[n].number == aAddIndex &&
				lpObj->VpPlayer[n].type == aType	) 
			{   // ��ϵǾ��ִٸ�
				//if( lpObj->VpPlayer[n].state == 1 ) lpObj->VpPlayer[n].state = 2;
				return -1;
			}
		} 
		else 
		{
			if( blank < 0 )	blank = n;
//			break;		//	���⼭ break�� �ɸ� �ʵȴ�. (���� �ε����� �����ϴ��� ã�ƾ� ��)
		} 
	}
	if( blank < 0 ) 
	{
		//LogAdd("(%d)���� ����Ʈ�� ��á��. MAX:%d [%s][%d]", aIndex, MVL, __FILE__, __LINE__);
		return -1;
	}
	lpObj->VpPlayer[blank].state     = STVP_CREATE;
	lpObj->VpPlayer[blank].number    = aAddIndex;
	lpObj->VpPlayer[blank].type      = aType;
	lpObj->VPCount++;

	// �߰��Ϸ��� ������Ʈ�� �������� �ƴ϶��..
	if( aType != OBJTYPE_ITEM )
	{
		// ������ ������ �����޶�� �˸���.
#ifndef MODIFY_MOVEMAP_RESET_VIEWPORT_BUGFIX_20060511
		ViewportAdd2( aAddIndex, aIndex, lpObj->Type );
#endif
	}

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	if( (lpObj->Type==OBJTYPE_MONSTER || lpObj->Type==OBJTYPE_NPC)
		&& aType==OBJTYPE_CHARACTER
	  )
	{
		// Agro System
		if( lpObj->m_iCurrentAI )
		{
			lpObj->m_Agro.SetAgro( aAddIndex, 1000 );
		}
	}
#endif

	return blank;
}


//----------------------------------------------------------------------------
// ���� ���� �ִ� ������ ����Ʈ�� ���� �߰��Ѵ�.??
int ViewportAdd2(int aIndex, int aAddIndex, int aType)
{
	int blank = -1;
	int count =  0;

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	LPOBJECTSTRUCT lpAddObj = &gObj[aAddIndex];

	if( (lpObj->m_stateEngagePVP == PVP_USER_PLAYING && lpAddObj->m_stateEngagePVP != PVP_USER_PLAYING) || (lpAddObj->m_stateEngagePVP == PVP_USER_PLAYING && lpObj->m_stateEngagePVP != PVP_USER_PLAYING) )
		return -1;

// 1. ���� ������ ��ϵǾ� �ִ� �������� �˾Ƴ���.
// 2. ��ϵǾ� �ִ� ������� �����Ѵ�.
// 3. ����� �ȵ� ������� ����Ѵ�.
	for( int n=0; n<MVL; n++)
	{
		if( lpObj->VpPlayer2[n].state )
		{ // ������ ��ϵǾ��ִٸ�
			if( lpObj->VpPlayer2[n].number == aAddIndex &&
				lpObj->VpPlayer2[n].type == aType	) 
			{   // ��ϵǾ��ִٸ�
				return -1;
			}
		} 
		else 
		{
			if( blank < 0 )	blank = n;
//			break;		//	���⼭ break�� �ɸ� �ʵȴ�. (���� �ε����� �����ϴ��� ã�ƾ� ��)
		} 
	}
	if( blank < 0 ) 
	{
		//LogAdd("(%d)(%s)����2 ����Ʈ�� ��á��. MAX:%d [%s][%d]", aIndex, gObj[aIndex].Name, MVL, __FILE__, __LINE__);
		return -1;
	}

	lpObj->VpPlayer2[blank].state  = STVP_CREATE;
	lpObj->VpPlayer2[blank].number = aAddIndex;
	lpObj->VpPlayer2[blank].type   = aType;
	lpObj->VPCount2++;
	return blank;
}

#ifdef BUGFIX_WIZARD_TELEPORT_MAGIC_20041018	// ���� �����ִ� ����ڵ��� ����Ʈ���� ���� ���� ����

void ViewportDel (short aIndex, int aDelIndex) {
	if (!gObjIsConnected(aIndex)) {
		return;
	}
	if (!gObjIsConnected(aDelIndex )) {
		return;
	}

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	for( int n = 0; n < MVL ; n++)
	{
		// ���� �����ִ� ������Ʈ��
		if( (lpObj->VpPlayer[n].state == STVP_CREATE ||
			lpObj->VpPlayer[n].state == STVP_PLAYING)) 
		{
			if (lpObj->VpPlayer[n].number == aDelIndex) {
				lpObj->VpPlayer[n].state   = STVP_NONE;
				lpObj->VpPlayer[n].number  = -1;
				lpObj->VPCount--;
			}
		}
	}	
}


void Viewport2Del (short aIndex, int aDelIndex) {
	if (!gObjIsConnected(aIndex)) {
		return;
	}
	if (!gObjIsConnected(aDelIndex )) {
		return;
	}

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	for( int n = 0; n < MVL ; n++)
	{
		// ���� �����ִ� ������Ʈ��
		if( (lpObj->VpPlayer2[n].state == STVP_CREATE ||
			lpObj->VpPlayer2[n].state == STVP_PLAYING)) 
		{
			if (lpObj->VpPlayer2[n].number == aDelIndex) {
				lpObj->VpPlayer2[n].state   = STVP_NONE;
				lpObj->VpPlayer2[n].number  = -1;
				lpObj->VPCount2--;
			}
		}
	}
}


void gObjViewportAllDel (short aIndex) {
	if (!gObjIsConnected(aIndex)) {
		return;
	}

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	
	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	for( int n = 0; n < MVL ; n++)
	{
		// ���� �����ִ� ������Ʈ��
		if( (lpObj->VpPlayer[n].state == STVP_CREATE ||
			lpObj->VpPlayer[n].state == STVP_PLAYING)) 
		{
			Viewport2Del (lpObj->VpPlayer[n].number, aIndex);
		}

		// ���� �����ִ� ������Ʈ��
		if( (lpObj->VpPlayer2[n].state == STVP_CREATE ||
			lpObj->VpPlayer2[n].state == STVP_PLAYING)) 
		{
			ViewportDel (lpObj->VpPlayer2[n].number, aIndex);
		}
	}
}

#endif


//////////////////////////////////////////////////////////////////////////////
// ����Ʈ���� �������� ������Ʈ�� ����Ʈ���� ���شٰ� �˸���.
void gObjViewportListDestroy(short aIndex)
{
	short tObjNum;
	int   n;
	
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) ) return;

	if( gObj[aIndex].Connected < CSS_GAMEPLAYING ) return;
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	
	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	for( n=0; n<MVL; n++)
	{
		// �� ����Ʈ�� ������Ʈ��..
		if( (lpObj->VpPlayer[n].state == STVP_CREATE ||
			lpObj->VpPlayer[n].state == STVP_PLAYING)) 
		{
			tObjNum = lpObj->VpPlayer[n].number;

			switch( lpObj->VpPlayer[n].type )
			{
			case OBJTYPE_ITEM :	// �������̴�.
				{
				int mapnum = lpObj->MapNumber;
				// �������� �ִ�...
				
				if( MapC[mapnum].m_cItem[tObjNum].IsItem() == TRUE )
				{
					//�������� �ִµ�...  ������ �Ѵٸ�.. �����..
					if( MapC[mapnum].m_cItem[tObjNum].m_State == OBJST_DIECMD )//||
					{
						lpObj->VpPlayer[n].state = STVP_DESTROY;
					}
					else
					{
						if( gObjCheckViewport(aIndex, MapC[mapnum].m_cItem[tObjNum].px, MapC[mapnum].m_cItem[tObjNum].py) == FALSE )
						{ // ����Ʈ �ȿ� ���ٸ�
							lpObj->VpPlayer[n].state = STVP_DESTROY;
						}
					}
				}
				else if( MapC[mapnum].m_cItem[tObjNum].live == 0 ||
						 MapC[mapnum].m_cItem[tObjNum].Give == 0 )
				{
					lpObj->VpPlayer[n].state = STVP_DESTROY;					
				}
				else 
				{					
					// ������ ���ش�.
					lpObj->VpPlayer[n].state = STVP_DESTROY;
				}
				}
				break;
			//case OBJTYPE_CHARACTER :
			default:
				// ������ �����ų� ��밡 ��¥ �׾����� ������Ų��.
				if( gObj[tObjNum].m_State == OBJST_CREATE && 
					gObj[tObjNum].Live == 0 ) 
				{
					lpObj->VpPlayer[n].state = STVP_DESTROY;
				}
				// OBJST_DELCMD
				if( gObj[tObjNum].Connected == 0 || 
					gObj[tObjNum].m_State == OBJST_DIECMD ||
#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724
					gObj[tObjNum].Teleport != 0 || // �ڷ���Ʈ ���ΰ�� �����������
#endif
					gObj[tObjNum].m_State == OBJST_DELCMD )	
				{
					lpObj->VpPlayer[n].state = STVP_DESTROY;
					//LogAddC(LOGC_RED,"�����̵� ��븦 ������Ų��. %s %s %d", gObj[aIndex].Name, gObj[tObjNum].Name, __LINE__);
				}
				else
				{
					if( gObjCheckViewport(aIndex, gObj[tObjNum].X, gObj[tObjNum].Y) == FALSE )
					{ // ��밡 ����Ʈ �ȿ� ���ٸ�
						lpObj->VpPlayer[n].state = STVP_DESTROY;
						
#ifdef 	ADD_NEW_MONSTER_SYSTEM_02_20050725
						// Agro System - �����̵��� ������ �� �� ����.
					  	if( (lpObj->Type == OBJTYPE_MONSTER || lpObj->Type == OBJTYPE_NPC)
							&& gObj[tObjNum].Type == OBJTYPE_CHARACTER )
						{
							if( lpObj->m_iCurrentAI )
								lpObj->m_Agro.DelAgro(tObjNum);
						}	
#endif

					}
				}
				break;
			}
			
		}
		
		// ���� ���� ����ڸ� üũ�ؼ� 
		if( (lpObj->VpPlayer2[n].state == STVP_CREATE ||
			lpObj->VpPlayer2[n].state == STVP_PLAYING)) 
		{
			tObjNum = lpObj->VpPlayer2[n].number;
			if( gObj[tObjNum].Connected < 3 )
			{
				lpObj->VpPlayer2[n].state = STVP_NONE;
				lpObj->VpPlayer2[n].number= -1;
				lpObj->VPCount2--;
			}
			else if( gObjCheckViewport(tObjNum, lpObj->X, lpObj->Y) == FALSE )
			{ // ��밡 ���� ����Ʈ �ȿ� ���ٸ� ���� ������ ������ ����Ʈ���� ����
				lpObj->VpPlayer2[n].state = STVP_NONE;//STVP_DESTROY;
				lpObj->VpPlayer2[n].number= -1;
				lpObj->VPCount2--;
			}
		}
	} // for
}


#ifdef PERSONAL_SHOP_20040113	// ���λ��� ���� ���θ� üũ�ϰ� ���λ����� ����Ʈ�� �߰��Ѵ�.

// ���λ����� ����Ʈ ����Ʈ�� �籸�� �Ѵ�. -> �߰��� ���λ����� ���� ��Ŷ�� ������.
void PShop_ViewportListRegenarate(short aIndex)
{
	if (!gObjIsConnected(aIndex)) {
		return;
	}
	
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	
	if( lpObj->Type != OBJTYPE_CHARACTER ) {	// ���ͳ� NPC�� �̰��� ���� �ʿ䰡 ����.
		return;
	}
	if( lpObj->RegenOk > 0 ) {					// ���� ���� ����ڴ� �����Ѵ�.
		return;
	}
	if( lpObj->CloseCount > -1) {				// ���� ���� ����ڴ� �����Ѵ�.
		return;
	}

	// *> . �߰��� ���λ��� �������� ���۰���
	CHAR cBUFFER [MAX_BUFF_SIZE];
	INT iVpOpenCount = 0;		// ��ü ���� ���λ���
	INT iVpAddCount = 0;		// �߰��� ���λ���
	LPPMSG_ANS_PSHOP_VIEWPORT_NOTIFY lpMsg = (LPPMSG_ANS_PSHOP_VIEWPORT_NOTIFY) cBUFFER;
	LPPMSG_PSHOP_VIEWPORT_NOTIFY lpMsgBody = (LPPMSG_PSHOP_VIEWPORT_NOTIFY) (cBUFFER + sizeof(PMSG_ANS_PSHOP_VIEWPORT_NOTIFY));


	// *> . ��ü ���λ��� �ε��� ����
	int iPShopOpenIndex[MAXVIEWPORTOBJECT];		// ������ ���� iVpOpenCount ����


	// *> . ������ �ٽ� �׸���� �÷��װ� ������ ���� �������� �����Ѵ�.
	if (lpObj->m_bPShopRedrawAbs) {
		memset(lpObj->m_iVpPShopPlayer, 0, sizeof(lpObj->m_iVpPShopPlayer));	// ���λ��� ����Ʈ �ʱ�ȭ
		lpObj->m_wVpPShopPlayerCount = 0;										// ���λ��� ����Ʈ�� ��ϵ� ������ �ʱ�ȭ
		lpObj->m_bPShopRedrawAbs = false;
	}


	// 1 . ����Ʈ ��ϰ� set �� -> set�� ������ ���λ��� �߰� �������ݿ� �߰�
	int n;
	for( n = 0 ; n < MAXVIEWPORTOBJECT ; n++ )
	{
		if( lpObj->VpPlayer[n].state == STVP_PLAYING &&
			lpObj->VpPlayer[n].type == OBJTYPE_CHARACTER
			)
		{
			if (gObj[lpObj->VpPlayer[n].number].m_bPShopOpen == true) {
				iPShopOpenIndex[iVpOpenCount] = lpObj->VpPlayer[n].number;
				iVpOpenCount++;

				if (lpObj->m_wVpPShopPlayerCount > 0 && lpObj->m_wVpPShopPlayerCount <= MAXVIEWPORTOBJECT) {
					BOOL bVpFound = FALSE;
					for (INT iVP = 0 ; iVP < lpObj->m_wVpPShopPlayerCount ; iVP++) {
						if (lpObj->m_iVpPShopPlayer[iVP] == lpObj->VpPlayer[n].number) {
							bVpFound = TRUE;
							break;
						}
					}
					
					if (bVpFound == FALSE) {
						// ã�� �� ����. -> ���� �߰��� ���λ����̹Ƿ� �������ݿ� �߰��Ѵ�.
						lpMsgBody[iVpAddCount].btNumberL = LOBYTE(WORD(lpObj->VpPlayer[n].number));
						lpMsgBody[iVpAddCount].btNumberH = HIBYTE(WORD(lpObj->VpPlayer[n].number));
						memcpy (lpMsgBody[iVpAddCount].szPShopText, gObj[lpObj->VpPlayer[n].number].m_szPShopText, MAX_PSHOP_TEXT);
						iVpAddCount++;
					}
				}
				else if (lpObj->m_wVpPShopPlayerCount == 0) {
					// ���� �߰��� ���λ����̰� ���� ����Ʈ�� ������Ƿ� �׳� ������ �������ݿ� �߰��Ѵ�.
					lpMsgBody[iVpAddCount].btNumberL = LOBYTE(WORD(lpObj->VpPlayer[n].number));
					lpMsgBody[iVpAddCount].btNumberH = HIBYTE(WORD(lpObj->VpPlayer[n].number));
					memcpy (lpMsgBody[iVpAddCount].szPShopText, gObj[lpObj->VpPlayer[n].number].m_szPShopText, MAX_PSHOP_TEXT);
					iVpAddCount++;
				}
				else {
					// �ε����� �̻��ϴ� -> ����
					LogAddTD("[PShop] ERROR : lpObj->m_wVpPShopPlayerCount is OUT of BOUND : %d", lpObj->m_wVpPShopPlayerCount);
				}
			}
		}
	}


	// 2 . ���λ��� ����Ʈ �迭�� �ʱ�ȭ�ϰ� ���� ���� ���λ����� �ϳ��� �����Ѵٸ� �迭�� �߰�
	memset(lpObj->m_iVpPShopPlayer, 0, sizeof(lpObj->m_iVpPShopPlayer));	// ���λ��� ����Ʈ �ʱ�ȭ
	lpObj->m_wVpPShopPlayerCount = 0;										// ���λ��� ����Ʈ�� ��ϵ� ������ �ʱ�ȭ
	

	if (iVpOpenCount > 0 && iVpOpenCount <= MAXVIEWPORTOBJECT) {
		for ( n = 0 ; n < iVpOpenCount ; n++) {
			lpObj->m_iVpPShopPlayer[n] = iPShopOpenIndex[n];
		}
		lpObj->m_wVpPShopPlayerCount = iVpOpenCount;
	}
	else if (iVpOpenCount != 0) {
		LogAddTD("[PShop] ERROR : iVpOpenCount is OUT of BOUND: %d", iVpOpenCount);
	}


	// 3 . ���� �߰��� ������ �ִٸ� �߰��� ���������� ����
	if (iVpAddCount > 0 && iVpAddCount <= MAXVIEWPORTOBJECT) {
		lpMsg->btCount = iVpAddCount;
		PHeadSubSetW((LPBYTE) lpMsg, 0x3F, 0x00, sizeof(PMSG_ANS_PSHOP_VIEWPORT_NOTIFY) + (sizeof(PMSG_PSHOP_VIEWPORT_NOTIFY) * iVpAddCount));
		DataSend (aIndex, (LPBYTE) lpMsg, MAKEWORD(lpMsg->h.sizeL, lpMsg->h.sizeH));
/*
		LogAddTD ("�������� ���� ������ INDEX:%d, �̸�:%s, ī��Ʈ:%d",
			aIndex,
			gObj[aIndex].Name,
			lpMsg->btCount
			);
		for ( n = 0 ; n < iVpAddCount ; n++) {
			LogAddTD("����Ʈ ���� INDEX:%d, TEXT:%s", 
				MAKEWORD(lpMsgBody[n].btNumberL ,lpMsgBody[n].btNumberH),
				lpMsgBody[n].szPShopText
				);
		}
*/
	}
	else if (iVpAddCount != 0) {
		// 
		LogAddTD("[PShop] ERROR : iVpAddCount is OUT of BOUND: %d", iVpAddCount);
	}


	// 4 . ���� ���� �ٸ� ���λ����� ���� �־��ٸ� ������ �ݰ���� Ȯ���� ��. ���� ������ ���λ��� �������� ����Ǿ��ٸ� �ٽ� ����
	if (gObj[aIndex].m_bPShopWantDeal == true) {
		if (gObjIsConnected(gObj[aIndex].m_iPShopDealerIndex)) {
			if (gObj[gObj[aIndex].m_iPShopDealerIndex].Type == OBJTYPE_CHARACTER) {
				if (gObj[gObj[aIndex].m_iPShopDealerIndex].m_bPShopOpen == false) {
					// ������ �ݾҴ�.
					gObj[aIndex].m_bPShopWantDeal = false;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset (gObj[aIndex].m_szPShopDealerName, 0, MAX_IDSTRING);

					CGPShopAnsDealerClosedShop (aIndex, gObj[aIndex].m_iPShopDealerIndex);
				}
#ifdef BUGFIX_PERSONALSHOP_ZENCOPY_20050426
				else if (
					gObj[gObj[aIndex].m_iPShopDealerIndex].CloseCount >= 0 ||
					gObj[aIndex].CloseCount >= 0
					)
				{
					// �����ϴ� ���̴�.
					gObj[aIndex].m_bPShopWantDeal = false;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset (gObj[aIndex].m_szPShopDealerName, 0, MAX_IDSTRING);

					CGPShopAnsDealerClosedShop (aIndex, gObj[aIndex].m_iPShopDealerIndex);
				}
#endif
				else {
					// ������ ���λ����� ������ �����ִ�.
					if (gObj[gObj[aIndex].m_iPShopDealerIndex].m_bPShopItemChange == true) {
						// ������ ���λ��� �������� ����Ǿ����Ƿ� �ٽ� �޾ƾ� �Ѵ�.
						CGPShopAnsBuyList (aIndex, gObj[aIndex].m_iPShopDealerIndex, true, true);
					}
				}


/*
				// �� ��ƾ ������ ������ ������ ���̴�.
				else {
					if (memcmp(gObj[gObj[aIndex].m_iPShopDealerIndex].Name, gObj[aIndex].m_szPShopDealerName, MAX_IDSTRING)) {
						// �ɸ����� �ٲ����. (���� �幰����)
						gObj[aIndex].m_bPShopWantDeal = false;
						gObj[aIndex].m_iPShopDealerIndex = -1;
						memset (gObj[aIndex].m_szPShopDealerName, 0, MAX_IDSTRING);

						CGPShopAnsDealerClosedShop (aIndex, gObj[aIndex].m_iPShopDealerIndex);
					}
				}
*/
			}
			else {
				// ����� �ƴϴ�.
				gObj[aIndex].m_bPShopWantDeal = false;
				gObj[aIndex].m_iPShopDealerIndex = -1;
				memset (gObj[aIndex].m_szPShopDealerName, 0, MAX_IDSTRING);

				CGPShopAnsDealerClosedShop (aIndex, gObj[aIndex].m_iPShopDealerIndex);
			}
		}
		else {
			// �� ����� ������ ������.
			gObj[aIndex].m_bPShopWantDeal = false;
			gObj[aIndex].m_iPShopDealerIndex = -1;
			memset (gObj[aIndex].m_szPShopDealerName, 0, MAX_IDSTRING);
			
			CGPShopAnsDealerClosedShop (aIndex, gObj[aIndex].m_iPShopDealerIndex);
		}
	}
}


// �ش� �ε����� ���λ����� ������� Ȯ���Ѵ�. (true - ����� / false - ������ �ִ�.)
bool PShop_CheckInventoryEmpty(short aIndex)
{
	// aIndex �� �������� �ε����̰� ���� ���� ���̶�� �����Ѵ�.
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	for (INT i = MAX_INVENTORY ; i < MAX_INVENTORY_EXTEND ; i++) {
		if (lpObj->Inventory1[i].IsItem() == TRUE)
			return false;
	}
	
	return true;
}


#endif



//----------------------------------------------------------------------------
// �׾��ִ� ������ �ٽ� �����..
void gObjStateSetCreate(int aIndex)
{	
	LPOBJECTSTRUCT	lpObj=&gObj[aIndex];

	if( lpObj->Connected < 3 )
	{
		return;
	}

	DWORD dwNowTick = GetTickCount();	// happycat@20050201

	// �׾�� �ִٸ�..
	if( lpObj->DieRegen == 1 )
	{
		if ((dwNowTick - lpObj->RegenTime) > (lpObj->MaxRegenTime+1000)) // ���� �׾��ٰ� �ٽ� ��Ƴ�����..
		{
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			gObjClearBuffEffect( lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT );
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			lpObj->DieRegen = 2;
			lpObj->m_State = OBJST_DIECMD;	// ���� ��¥ �׾��
		}
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
		//if( lpObj->Type == OBJTYPE_MONSTER && CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
		//if( lpObj->Type == OBJTYPE_MONSTER && lpObj->MapNumber == 9 )
#endif
		//{	// �Ǹ��� ������ �ٷ� ������Ų��
		//	lpObj->DieRegen = 2;
		//	lpObj->m_State = OBJST_DIECMD;	// ���� ��¥ �׾��
		//}
	}	

	#ifdef	HAPPY_CAT
	// Object �˻� ȸ���� ���̱� ���ؼ� 1�ʸ��� ȣ��Ǹ�, �ٸ� �������� ȣ�� ���� �ʴ� �� �Լ����� üũ �ϱ�� �Ѵ�.
	if (OBJTYPE_CHARACTER == lpObj->Type)
	{
		if (OBJST_DIECMD == lpObj->m_State)
		{
			gObjReady4Relife(lpObj);
		}
		else
		{
			gObjCheckTimeOutValue(lpObj, dwNowTick);
		}
	}
	#endif	//HAPPY_CAT

#ifdef MODIFY_SEASON_01_20060317

	#ifndef HAPPY_CAT
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( OBJST_DIECMD == lpObj->m_State )
		{
			gObjReady4Relife( lpObj );
		}
		else
		{
			gObjCheckTimeOutValue( lpObj, dwNowTick );
		}
	}
	#endif // HAPPY_CAT

#endif // MODIFY_SEASON_01_20060317

	//if( lpObj->m_State == OBJST_CREATE && lpObj->m_StateSub == 1 )
	if( lpObj->Teleport == 1 )
	{
		if( lpObj->DieRegen )
		{
			lpObj->Teleport = 0;
			//LogAddC(LOGC_RED,"�����̵� ��ҽ�Ų��. %s %d", lpObj->Name, __LINE__);
		}
		else
		{
			if( GetTickCount()-lpObj->TeleportTime > 100 )
			{
				lpObj->Teleport = 2;
				//lpObj->m_State = OBJST_CREATE;
				//LogAddC(LOGC_RED,"�����̵� 2 %s %d", lpObj->Name, __LINE__);
			}
		}
	}
	else if( lpObj->Teleport == 2 )
	{
		lpObj->Teleport = 3;
		//LogAddC(LOGC_RED,"�����̵� 3 %s %d", lpObj->Name, __LINE__);
		if( lpObj->Type == OBJTYPE_CHARACTER )
		{
			lpObj->TX   = lpObj->X;
			lpObj->TY   = lpObj->Y;
			
#ifdef MODIFY_WIZARD_TELEPORT_VIEWPORT_20060526
				CreateFrustrum(lpObj->X,lpObj->Y, aIndex);
				GCTeleportSend(&gObj[aIndex], 0, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);
				gObjViewportListProtocolCreate(lpObj);
#else
	#ifdef BUGFIX_WIZARD_TELEPORT_MAGIC_20041018
				GCTeleportSend(&gObj[aIndex], 0, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);
				gObjViewportAllDel(lpObj->m_Index);
				gObjViewportListDestroy(lpObj->m_Index);
				CreateFrustrum(lpObj->X,lpObj->Y, lpObj->m_Index);
				gObjClearViewport(lpObj);
				gObjViewportListCreate(lpObj->m_Index);
				gObjViewportListProtocolCreate(lpObj);
	#else
				CreateFrustrum(lpObj->X,lpObj->Y, aIndex);
				GCTeleportSend(&gObj[aIndex], 0, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, lpObj->Dir);
				gObjViewportListProtocolCreate(lpObj);
	#endif
#endif // MODIFY_WIZARD_TELEPORT_VIEWPORT_20060526

			
#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �����ʿ����� �ڷ���Ʈ ���Ŀ� �ڽ��� �������� ���¸� �ٽ� ������
			if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE &&
				g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
				)
			{
				g_CastleSiege.NotifySelfCsJoinSide (aIndex);
				g_CastleSiege.NotifyCsSelfLeftTime (aIndex);
			}
#endif

			lpObj->Teleport = 0;			
		}
#ifdef	NPC_QEUSTNPC2
		else if( lpObj->Type == OBJTYPE_NPC && lpObj->Class == NPC_QEUSTNPC2 )
		{
			int x, y;
			
			x = lpObj->X;
			y = lpObj->Y;
			
			lpObj->X = x;
			lpObj->Y = y;
			lpObj->TX   = lpObj->X;
			lpObj->TY   = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y, aIndex);			
			//gObjViewportListProtocolCreate(lpObj);
			lpObj->m_State	= OBJST_CREATE;
			lpObj->PathStartEnd       = 0;
		}
#endif
#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724
		else if ( lpObj->Type == OBJTYPE_MONSTER )
		{
			// �ڷ���Ʈ �Ϸ� �� ó��
			lpObj->TX   = lpObj->X;
			lpObj->TY   = lpObj->Y;
			CreateFrustrum(lpObj->X,lpObj->Y, aIndex);
		
			// ����Ʈ ������ ����
			gObjViewportListCreate(lpObj->m_Index);
			gObjViewportListProtocolCreate(lpObj);

			// �Ϸ�
			lpObj->Teleport = 0;	

			// �ϴ� ���߾����.
			lpObj->PathStartEnd = 0;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
		}
#endif // MODIFY_NEW_MONSTER_SKILL_01_20060724	
		else 
		{
			int x, y;

			if( lpObj->m_RecallMon>=0)
			{
				x = lpObj->X;
				y = lpObj->Y;
			}
			else
			{
				x = (lpObj->X-3)+((rand()%6));
				y = (lpObj->Y-3)+((rand()%6));
			}
			lpObj->X = x;
			lpObj->Y = y;
			lpObj->TX   = lpObj->X;
			lpObj->TY   = lpObj->Y;
			//CreateFrustrum(lpObj->X,lpObj->Y, aIndex);
			if( lpObj->m_RecallMon>=0)
			{
				gObjViewportListProtocolCreate(lpObj);
				lpObj->PathStartEnd       = 0;
				//lpObj->m_ActState.Emotion = 1;
			}
		}
	}

	if( lpObj->RegenOk == 2 ) // �ʰ��� �̵��� ���
	{
		if( lpObj->Type == OBJTYPE_CHARACTER )
		{
			lpObj->m_State = OBJST_CREATE;
			lpObj->RegenOk = 3;
			
			lpObj->X = lpObj->RegenMapX;
			lpObj->Y = lpObj->RegenMapY;
			lpObj->MapNumber = lpObj->RegenMapNumber;

			lpObj->TX   = lpObj->X;
			lpObj->TY   = lpObj->Y;
			CreateFrustrum(lpObj->X,lpObj->Y, aIndex);
			// �巡�� �̺�Ʈ�� �������̸�..
			if( DragonEvent->GetState() > 0 )
			{
				if( DragonEvent->GetMapNumber() == lpObj->MapNumber )
				{
					GCMapEventStateSend(lpObj->MapNumber, 1, 1);
				}
			}			
#ifdef CASTLE_MAIN_SCHEDULER_20041111				// �������� ���� ���̸� ����ڿ��� �˷��ش�.
			if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE) {
				GCAnsCsNotifyStart(lpObj->m_Index, (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)?1:0);
				if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
					g_CastleSiege.NotifySelfCsJoinSide (lpObj->m_Index);
					g_CastleSiege.NotifyCsSelfLeftTime (lpObj->m_Index);
				}
			}
#endif
			gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);


#ifdef FOR_JAPAN	
	// Ÿ��¯ �̺�Ʈ�� �������̸�
			if( gTamaJJangEvent )
			{
				if( gAppearTamaJJang && lpObj->MapNumber == 0 )
				{	// Ÿ��¯�� ���� �ְ�, �з��þƿ� ������
					GCMapEventStateSend(0, gAppearTamaJJang, 2);	
				}
			}
#endif
			
#ifdef PCBANG_ADVANTAGE	// happycat@20050201 (PC�� ����)
			LPOBJECTSTRUCT	lpObj	= (LPOBJECTSTRUCT)&gObj[aIndex];

	#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
			if( IsOKPCBangBenefitAll(lpObj) )
	#else
			if (lpObj->m_bPCBangUser)
	#endif //PCBANG_ADVANTAGE_EXPAND_20060919
			{
				gObjCalCharacter(lpObj->m_Index);
				
	#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
	#else
				GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0);
	#endif
				GCManaSend(lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);		
			}
#endif//PCBANG_ADVANTAGE

		}//if( lpObj->Type == OBJTYPE_CHARACTER )

	}//if( lpObj->RegenOk == 2 )
	
}

//----------------------------------------------------------------------------
// ����Ʈ ����� ���¸� �÷��� ���·� �ٲ۴�.
// �ð� �ʰ��� ����ڸ� ���� ��Ű�⵵ �Ѵ�.
//----------------------------------------------------------------------------
void gObjSetState()
{
	int n;
	LPOBJECTSTRUCT	lpObj;
		
	for( n=0; n<MAX_OBJECT; n++ )
	{
		lpObj = &gObj[n];

		if( lpObj->Connected > 2 )
		{
			if( lpObj->m_State == OBJST_CREATE )
			{
				lpObj->m_State = OBJST_PLAYING;
			
				if (lpObj->RegenOk == 3 )
				{
					lpObj->RegenOk = 0;
				}
			}	
			if( lpObj->Teleport == 3 )
			{
				lpObj->Teleport = 0;
				//LogAddC(LOGC_RED,"�����̵� 0 %s %d", lpObj->Name, __LINE__);
				//gObjClearViewport(lpObj);
			}
		

			if( lpObj->DieRegen == 2 )	// �׾���.. �ٽ� ��Ȱ�ض�..
			{
#ifdef GMCOMMAND_EX
				if (lpObj->m_bGMSummon == TRUE) {
					gObjDel(lpObj->m_Index);
					continue;
				}
#endif // GMCOMMAND_EX

#ifdef MAP_SERVER_WORK_20041030								// �׾��ٰ� ��Ƴ� �� ������ ��ǥ�� ���� �ʼ��� �̵����� Ȯ��
				INT iObjSrcMap				= lpObj->MapNumber;
				INT iObjSrcX				= lpObj->X;
				INT iObjSrcY				= lpObj->Y;
#endif
				//------------->
				//20080930 �ؿ� 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
				#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
				lpObj->m_siDieRegenX		=	lpObj->X;			// ��Ȱ ���� X
				lpObj->m_siDieRegenY		=	lpObj->Y;			// ��Ȱ ���� Y
				lpObj->m_siDieRegenMapIndex	=	lpObj->MapNumber;	// ��Ȱ ���� Map
				#endif //ADD_TELEPORT_CHARM_ITEM_20080930
				//<-------------
#ifdef USER_DIE_MAKE_OPPOSIT_PK_20040329
				lpObj->m_bAttackerKilled	= false;		// ���� �׾��� �� ������ �ϴ� �׾����� ����
#endif

#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
				if( CHECK_DEVILSQUARE(lpObj->MapNumber) && lpObj->Type >= OBJTYPE_MONSTER )
#else
				if( lpObj->MapNumber == 9 && lpObj->Type >= OBJTYPE_MONSTER )
#endif
				{
					g_DevilSquare.gDevilSquareMonsterRegen(lpObj);
					continue;
				}

#ifdef ADD_KANTURU_20060627
				// ĭ���� �������� ���ʹ� �������� �ʴ´�.
				else if( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS && lpObj->Type == OBJTYPE_MONSTER )
				{
					continue;
				}
#endif
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	   // �׷� ���ʹ� Regen Ÿ�Կ� ���� ��ȯ�� ���� ���� �ϰų�, �� ��ƾ�� �̿��� ���� ��Ų��.
				if( lpObj->m_iCurrentAI )
				{
					if( lpObj->m_iRegenType != 0 )
					{
						// -1�� ��� �������� �ʴ´�.
						// 0 �� ��� �Ϲݸ���
						// 1 �� ��� ��ȯ�� ���� ���� - ũ���̿�����
						continue;
					}
				}
				

#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725


#ifdef CRYWOLF_MONSTER_WORK_20050914
				if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
				{

// �δ� ���͵��� ���� ��Ű�� �ʴ´�. - ��ũ������ ���� ��Ŵ.

// �Ϲ� ���͵��� ũ���̿��� ���� �� ���� ��Ű�� �ʴ´�.
					
					if( g_CrywolfSync.GetCrywolfState() == CRYWOLF_STATE_READY 
						&& lpObj->Type >= OBJTYPE_MONSTER)
					{
						continue;
					}

				}
#endif
				

#ifdef MONSTER_HERD_SYSTEM_20031120
				if (lpObj->m_bIsInMonsterHerd) {			// ���� ���ʹ� ��Ȱ��Ű�� �ʴ´�.
					if (lpObj->m_lpMonsterHerd != NULL)
						lpObj->m_lpMonsterHerd->MonsterRegenAction(lpObj);
					continue;
				}
#endif

#ifdef FOR_BLOODCASTLE		// 2004.02.02 ����ĳ�� ���� �� ���� ���� - ���� �� üũ�κ� (�� ��ƾ���� ���� ��)
				if( CHECK_BLOODCASTLE(lpObj->MapNumber) && lpObj->Type >= OBJTYPE_MONSTER )
				{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
					int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpObj->MapNumber );
#endif
					if (lpObj->Class == 131) {		// ������ ���� -> �ı��Ǿ��� ���� üũ
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bCASTLE_DOOR_LIVE = FALSE;

						// ������ �μ������� üũ �Ѵ�
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bBC_DOOR_TERMINATE_COMPLETE = TRUE;

						// �������� �Ҵ緮�� ����Ѵ�. -> ���Ⱑ ���� ���Ǵ� �� 
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(iBridgeIndex) * BC_PERUSER_BOSS_MONSTER_KILL;
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;

						if( g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT > 10 )
						{
							g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
						}
#else
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1) * BC_PERUSER_BOSS_MONSTER_KILL;
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;

						if( g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT > 10 )
						{
							g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
						}
#endif
						
						// �ش� ���� ����鿡�� ���� ���Ӽ� ������ �˸���.
						g_BloodCastle.SendCastleDoorBlockInfo(lpObj->m_cBloodCastleIndex, false);

						// ������ ��ϻ��µ� �����Ѵ�.
						g_BloodCastle.ReleaseCastleDoor(lpObj->m_cBloodCastleIndex);

						// ���� ������ ���� ���̶�� �� ������ �������� ������ ������ �����Ѵ�.
						if (g_BloodCastle.GetCurrentState(lpObj->m_cBloodCastleIndex) == BLOODCASTLE_STATE_PLAYING)
							g_BloodCastle.SetBossMonster(lpObj->m_cBloodCastleIndex);
					}


					if (CHECK_LIMIT(lpObj->Class-BC_SAINT_STATUE_1, 3))		// �����Ǵ� ���� ������ �����̸� ���⼭ ���빫���� �ϳ��� ������.
					{
						if (CHECK_BLOODCASTLE(lpObj->MapNumber)) {
							// ��ġ�� ���� ĳ���� Ȯ���� ��쿡�� ������ ���빫���� �ϳ��� ������.
							int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
							int iLevel = lpObj->Class-BC_SAINT_STATUE_1;
 							int iType = ItemGetNumberMake(13, 19);
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
							int iUltimateWeaponMapNum;
							switch( lpObj->MapNumber )
							{
							case MAP_INDEX_BLOODCASTLE1:	iUltimateWeaponMapNum = BC_MAPNUMBER_ULTIMATEWEAPON1; break;
							case MAP_INDEX_BLOODCASTLE2:	iUltimateWeaponMapNum = BC_MAPNUMBER_ULTIMATEWEAPON2; break;
							case MAP_INDEX_BLOODCASTLE3:	iUltimateWeaponMapNum = BC_MAPNUMBER_ULTIMATEWEAPON3; break;
							case MAP_INDEX_BLOODCASTLE4:	iUltimateWeaponMapNum = BC_MAPNUMBER_ULTIMATEWEAPON4; break;
							case MAP_INDEX_BLOODCASTLE5:	iUltimateWeaponMapNum = BC_MAPNUMBER_ULTIMATEWEAPON5; break;
							case MAP_INDEX_BLOODCASTLE6:	iUltimateWeaponMapNum = BC_MAPNUMBER_ULTIMATEWEAPON6; break;
							case MAP_INDEX_BLOODCASTLE7:	iUltimateWeaponMapNum = BC_MAPNUMBER_ULTIMATEWEAPON7; break;
							case MAP_INDEX_BLOODCASTLE8:	iUltimateWeaponMapNum = BC_MAPNUMBER_ULTIMATEWEAPON8; break;
							}
							ItemSerialCreateSend(lpObj->m_Index, iUltimateWeaponMapNum, (BYTE)lpObj->X, (BYTE)lpObj->Y, iType, iLevel, (BYTE)0, 0, 0, 0, iMaxHitUser);
#else
							ItemSerialCreateSend(lpObj->m_Index, BC_MAPNUMBER_BASE_ULTIMATEWEAPON + lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, iType, iLevel, (BYTE)0, 0, 0, 0, iMaxHitUser);
#endif
							
							// �׸��� �ش� �������� ������ ����Ѵ�. (0:������ / 1:�� / 2:����)
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
							g_BloodCastle.m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER = iLevel;

							LogAddTD("[Blood Castle] (%d) Ultimate Weapon Dropped -> (%s)", 
								iBridgeIndex + 1,
								(iLevel)?((iLevel==1)?"Sword":"Crossbow"):"Staff"								
								);
#else
							g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_btBC_QUEST_ITEM_NUMBER = iLevel;

							LogAddTD("[Blood Castle] (%d) Ultimate Weapon Dropped -> (%s)", 
								lpObj->MapNumber-MAP_INDEX_BLOODCASTLE1+1,
								(iLevel)?((iLevel==1)?"Sword":"Crossbow"):"Staff"								
								);
#endif
						}
					}

					if (lpObj->m_PosNum == -1) {	// ���� ĳ�� �������̶�� ���� ��Ű�� �ʴ´�.
						gObjDel (lpObj->m_Index);
						continue;
					}

#ifdef BLOODCASTLE_EVENT_5TH_20050531
					if (lpObj->Type == OBJTYPE_MONSTER &&
						(lpObj->Class != 131 && !CHECK_LIMIT(lpObj->Class-BC_SAINT_STATUE_1, 3))
						)
					{
						// �Ϲ� ������ ��� ������ ��ġ�� �ٸ��� ����ð��� ���� ���ݷ�, ���� ���� ��ġ�� ����
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
						g_BloodCastle.ChangeMonsterState(iBridgeIndex, lpObj->m_Index);
#else
						g_BloodCastle.ChangeMonsterState(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1, lpObj->m_Index);
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
					}
#endif
				}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ����� ������ ���Ҷ��� ����
				if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
				{
					
					if( (lpObj->Class == NPC_ILLUSION_HOLY_RELICS_STATUE				// ����
						|| ( lpObj->Class >= MON_GHOST_OF_ILLUSION_BROTHERHOOD_A1	// ȯ�������� ����
						&& lpObj->Class <= MON_GHOST_OF_ILLUSION_BROTHERHOOD_C6) )
						&& lpObj->m_PosNum == -1)
					{
						continue;
					}
				}
#endif
				
#ifdef ADD_NEW_MAP_KALIMA_20040518					// ���Ϳ��� ��ȯ�� ���ʹ� ���� ��Ű�� �ʴ´�.
				if (lpObj->m_Attribute == MON_ATTR_SUMMONED) {
					gObjDel (lpObj->m_Index);
					continue;
				}
#endif

#ifdef CASTLEDEEP_EVENT_20050413
				if (lpObj->m_Attribute == MON_ATTR_INSTANCE) {
					gObjDel (lpObj->m_Index);
					continue;
				}

				if (lpObj->m_Attribute == MON_ATTR_CASTLEDEEP) {
					switch(lpObj->Class) {
					case 300:
					case 301:
					case 302:
					case 303:
						{
							// �η����� �̺�Ʈ�� �߰��������� �ٷ� ����
							gObjDel (lpObj->m_Index);
							continue;
						}
						break;
					default:
						{
							// �η����� �̺�Ʈ�� �Ϲݸ��͵��� N�� ���� ��������
							if (lpObj->m_dwLastCheckTick + CASTLEDEEP_DELTICKCOUNT < GetTickCount()) {
								gObjDel (lpObj->m_Index);
								continue;
							}
						}
						break;
					}
				}
#endif

#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214		
				if (lpObj->Type >= OBJTYPE_MONSTER 
					&& ( lpObj->Class == NPC_INDEX_SPEARMAN || lpObj->Class == NPC_INDEX_BOWMAN) ) 
				{
					g_CsNPC_Mercenary.DeleteMercenary( lpObj->m_Index );
					gObjDel (lpObj->m_Index);
					continue;
				}
#endif
				
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
				if( lpObj->Type >= OBJTYPE_MONSTER && lpObj->Class == NPC_INDEX_LIFESTONE )
				{
					g_CsNPC_LifeStone.DeleteLifeStone( lpObj->m_Index );
					gObjDel (lpObj->m_Index);
					continue;
				}
#endif

				int iregen = 1;

				if( lpObj->Type >= OBJTYPE_MONSTER  )
				{
					if( lpObj->Class == 44 )
					{
						if( (DragonEvent->GetState()) == 0 )
						{
							iregen = 0;
						}
					}
					if( lpObj->Class == 53 || 
						lpObj->Class == 54 || 
						lpObj->Class == 55 || 
						lpObj->Class == 56 )
					{
						iregen = 0;
						lpObj->Life		= lpObj->MaxLife;
						lpObj->DieRegen	= 0;
					}

					if( lpObj->Class >= 78 && lpObj->Class <= 83 )
					{
						iregen = 0;
						lpObj->Life		= lpObj->MaxLife;
						lpObj->DieRegen	= 0;
					}					
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311	
					if( lpObj->Class >= 493 && lpObj->Class <= 502 )
					{	
						iregen = 0;
						lpObj->Life		= lpObj->MaxLife;
						lpObj->DieRegen	= 0;
					}
#endif //ADD_GOLDEN_EVENT_RENEWAL_20090311
#ifdef NEW_SKILL_FORSKYLAND
					if( lpObj->Class == 77 )
					{	// õ���� ������ ���� �츰��
						int sheildindex = lpObj->m_SkyBossMonSheildLinkIndex;

						if( sheildindex != -1 )
						{
							gObj[sheildindex].Life = gObj[sheildindex].MaxLife;
						}
					}
#endif
				}

				if( iregen )
				{
					PMSG_CHARREGEN	pMsg;
					lpObj->Life		= lpObj->MaxLife+lpObj->AddLife;
					lpObj->Mana		= lpObj->MaxMana+lpObj->AddMana;
#ifdef ADD_SHIELD_POINT_01_20060403
					lpObj->iShield	= lpObj->iMaxShield+lpObj->iAddShield;
#endif
					lpObj->Live		= 1;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					gObjRemoveBuffEffect( lpObj, BUFFTYPE_POISON );
					gObjRemoveBuffEffect( lpObj, BUFFTYPE_FREEZE );
					gObjRemoveBuffEffect( lpObj, BUFFTYPE_STONE );
#else
					lpObj->m_PoisonBeattackCount= 0;
					lpObj->m_ColdBeattackCount	= 0;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					lpObj->m_ViewState			= 0;
					lpObj->Teleport				= 0;
					
					for( int i=0; i<MAXSELFDEFENSE; i++)
					{
						//lpObj->SelfDefense[i]		= -1;
						lpObj->SelfDefenseTime[i]	= 0;
					}
					
					gObjTimeCheckSelfDefense(lpObj);
					
#ifdef MODIFY_PK_SYSTEM_BUGFIX_20070323	// �׾��� ��� ���� ���� �ʱ�ȭ
					lpObj->MySelfDefenseTime = 0;
#endif
					
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					gObjClearBuffEffect( lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT );
#else
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
					DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_POISON );
					DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_FREEZE );
	#else
					lpObj->m_ViewSkillState &= 0xFC;
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					
					gObjClearViewport(lpObj);
					

					if( lpObj->Type == OBJTYPE_CHARACTER )
					{

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
						if(g_iUseCharacterAutoRecuperationSystem && lpObj->Level <= g_iCharacterRecuperationMaxLevel)
							lpObj->m_iAutoRecuperationTime = GetTickCount();
						else lpObj->m_iAutoRecuperationTime = 0;
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
						lpObj->dwShieldAutoRefillTimer = GetTickCount();
#endif

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
						if(lpObj->m_iSkillNPCHelpTime > 0)
						{
							lpObj->m_iSkillNPCHelpTime = 0;
							lpObj->m_iSkillNPCDefense = 0; 
							lpObj->m_iSkillNPCAttack = 0;

							DEL_VIEWSKILLSTATE(lpObj->m_ViewSkillState, STATE_NPC_HELP);
							GCStateInfoSend(lpObj, 0, STATE_NPC_HELP);			
						}	
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef DARKLORD_WORK
						gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::PetItem_Mode_Normal);
#endif
						// ����ڰ� ������ ���� ��ǥ����
#ifdef DUEL_SYSTEM_20031028
						// ���� �� ������ ��ǥ
						if( lpObj->KillerType == KT_DUELCHAR ) {
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	#ifdef ADD_NEWPVP_PKFIELD
							if (!g_NewPVP.IsDuel(*lpObj))
								MapC[lpObj->MapNumber].GetRandomLengthPos(lpObj->X, lpObj->Y, 8);
	#else
							MapC[lpObj->MapNumber].GetRandomLengthPos(lpObj->X, lpObj->Y, 8);
	#endif // ADD_NEWPVP_PKFIELD
#else
	#ifdef ADD_NEWPVP_PKFIELD
							if (!g_NewPVP.IsDuel(*lpObj))
								MapC[lpObj->MapNumber].GetRandomLengthPos(lpObj->X, lpObj->Y, 18);
	#else
							MapC[lpObj->MapNumber].GetRandomLengthPos(lpObj->X, lpObj->Y, 18);
	#endif // ADD_NEWPVP_PKFIELD
#endif
						} else
#endif
						// ����� �� ������ ��ǥ
						if( lpObj->KillerType == KT_GUILDWARCHAR )
						{
							int wtype=0;
							if( (lpObj->lpGuild != NULL) )
							{
								if( lpObj->lpGuild->WarType == BTT_SOCCER )
								{
									wtype = 1;
								}
							}
							if( wtype == 0 )	// �Ϲ� �����
								MapC[lpObj->MapNumber].GetRandomLengthPos(lpObj->X, lpObj->Y, 18);
							else				// ��Ʋ��Ŀ
							{
								if( lpObj->lpGuild->WarState )
								{
									int bgi = lpObj->lpGuild->BattleGroundIndex;
									gBattleGetTeamPosition(bgi, lpObj->lpGuild->BattleTeamCode, lpObj->X, lpObj->Y);
									//LogAdd("��Ʋ�౸ �׾ ���� : %d %d", lpObj->X, lpObj->Y);
								}
							}

						}
						else
						{
							// ������ �η��þƿ��� �η��þƷ�..
							if( lpObj->MapNumber == 0 ||	// �η��þ�
								lpObj->MapNumber == 1 )		// ����
							{
								lpObj->MapNumber = 0;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
							}
							else if( lpObj->m_stateEngagePVP == PVP_USER_PLAYING )
							{
								g_PvpEvent.DeleteUser(lpObj->m_Index);
							}
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
							else if( CHECK_DEVILSQUARE(lpObj->MapNumber) )	// �Ǹ��� ����
#else
							else if( lpObj->MapNumber == 9 )				// �Ǹ��� ����
#endif
							{
								g_DevilSquare.DieProcDevilSquare(lpObj);
								lpObj->MapNumber = 3;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
							}
#ifdef NEW_SKILL_FORSKYLAND
							else if( lpObj->MapNumber == 10 )
							{
								lpObj->MapNumber = 2;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
							}
#endif
#ifdef FOR_BLOODCASTLE
							else if( CHECK_BLOODCASTLE(lpObj->MapNumber))	// ����ĳ������ �׾��ٸ� ����ƽ���
							{
								lpObj->MapNumber = 2;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
							}
#endif
#ifdef CHAOSCASTLE_SYSTEM_20040408
							else if( CHECK_CHAOSCASTLE(lpObj->MapNumber))	// ī����ĳ������ �׾��ٸ� ����ƽ���
							{
								lpObj->MapNumber = 2;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
							}
#endif
#ifdef ADD_NEW_MAP_KALIMA_20040518
							else if( CHECK_KALIMAMAP(lpObj->MapNumber))		// Į�������� �׾��ٸ� ����ƽ�
							{
								lpObj->MapNumber = 2;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
							}
#endif
							
#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
							else if( lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE)	// ĳ�� ����Ϳ��� �׾��� ���
							{
								INT iGateNum	= CASTLE_HUNTZONE_GATE_01;
								
								SHORT sX, sY, sLVL;
								BYTE btMAPNUM	= lpObj->MapNumber; 
								BYTE btDIR;
								INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
								if( iMapNumber >= 0 ) 
								{
									lpObj->MapNumber	= btMAPNUM;
									lpObj->X			= sX;
									lpObj->Y			= sY;
								}
							}
#endif
							
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208	// ���� �ʿ��� �׾��� ��� ���������� üũ �� ����
							else if( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )		
							{
								if( g_CsNPC_LifeStone.SetReSpawnUserXY( lpObj->m_Index ) == FALSE )
								{
									INT iGateNum	= 100;
									if (lpObj->m_btCsJoinSide == CS_SIEGESIDE_DEFEND) 
									{
										iGateNum	= 105;		// ������ �⺻ �ڷ���Ʈ ���
									}
									else 
									{
										iGateNum	= 100;		// ������ �⺻ �ڷ���Ʈ ���
									}

									SHORT sX, sY, sLVL;
									BYTE btMAPNUM	= lpObj->MapNumber; 
									BYTE btDIR;
									INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
									if( iMapNumber >= 0 ) {
										lpObj->MapNumber	= btMAPNUM;
										lpObj->X			= sX;
										lpObj->Y			= sY;
									}
									// �����ص� ���ڸ����� �ٽ� �����ϵ��� ����
								}
							}
#endif

#ifdef ADD_NEW_MAP_AIDA_20050617
							// ���̴ٿ��� �׾��� ���
							else if( lpObj->MapNumber == MAP_INDEX_AIDA )		
							{
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
							}
#endif
							
#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
							// ũ���̿��� 1�� �������� �׾��� ���
							else if( lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )		
							{
								
#ifdef CRYWOLF_MODIFY_REGEN_POS_ON_MVP_20060116
								if( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_START )
								{
									// ũ���̿��� (/�̵� ũ���̿���) ����Ʈ ��ȣ
									INT iGateNum	= 118;
									
									SHORT sX, sY, sLVL;
									BYTE btMAPNUM	= lpObj->MapNumber; 
									BYTE btDIR;
									INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
									if( iMapNumber >= 0 ) 
									{
										lpObj->MapNumber	= btMAPNUM;
										lpObj->X			= sX;
										lpObj->Y			= sY;
									}
									else
									{
										MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
									}
								}
								else
								{
									MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
								}
#else
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
#endif // CRYWOLF_MODIFY_REGEN_POS_ON_MVP_20060116
								
							}

#endif

#ifdef ADD_KANTURU_20060627
							// ĭ���� �����ʿ��� �׾��� ���
							else if( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS )		
							{
								INT iGateNum	= KANTURU_DEFAULT_REGEN_MAP_GATE;
								
								SHORT sX, sY, sLVL;
								BYTE btMAPNUM	= lpObj->MapNumber; 
								BYTE btDIR;
								INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
								if( iMapNumber >= 0 ) 
								{
									lpObj->MapNumber	= btMAPNUM;
									lpObj->X			= sX;
									lpObj->Y			= sY;
								}			
								
								g_KanturuBattleUserMng.DeleteUserData( lpObj->m_Index );
							}
#endif	// #ifdef ADD_KANTURU_20060627

#ifdef ADD_KANTURU_CONDITION_20060725
							// ĭ���� �Ϲݸʿ��� ���� ���
							else if( lpObj->MapNumber == MAP_INDEX_KANTURU1 )
							{
								int iGateNum = KANTURU_DIEREGEN_GATE_MAP_1;

								SHORT sX, sY, sLVL;
								BYTE btMAPNUM	= lpObj->MapNumber; 
								BYTE btDIR;
								INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
								if( iMapNumber >= 0 ) 
								{
									lpObj->MapNumber	= btMAPNUM;
									lpObj->X			= sX;
									lpObj->Y			= sY;
								}
							}
							else if( lpObj->MapNumber == MAP_INDEX_KANTURU2 ) 
							{
								int iGateNum = KANTURU_DIEREGEN_GATE_MAP_2;

								SHORT sX, sY, sLVL;
								BYTE btMAPNUM	= lpObj->MapNumber; 
								BYTE btDIR;
								INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
								if( iMapNumber >= 0 ) 
								{
									lpObj->MapNumber	= btMAPNUM;
									lpObj->X			= sX;
									lpObj->Y			= sY;
								}
							}
#endif		//ADD_KANTURU_CONDITION_20060725
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� ���� ����Ʈ�ʿ��� ���� ��� �������� ����						
							else if( lpObj->MapNumber == MAP_INDEX_3RD_CHANGEUP_QUEST 
								|| lpObj->MapNumber == MAP_INDEX_3RD_CHANGEUP_BOSS_QUEST )
							{
								int iGateNum = GATE_REGENPLACE_OF_BALGAS;

								SHORT sX, sY, sLVL;
								BYTE btMAPNUM	= lpObj->MapNumber; 
								BYTE btDIR;
								INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
								if( iMapNumber >= 0 ) 
								{
									lpObj->MapNumber	= btMAPNUM;
									lpObj->X			= sX;
									lpObj->Y			= sY;
								}
							}
#endif	// THIRD_CHANGEUP_SYSTEM_20070507
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ������� ������ ó�� ������ġ��
							else if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
							{
								BYTE btTeam = g_IllusionTempleEvent.GetUserTeam(lpObj->MapNumber, lpObj->m_Index);
								if( btTeam == 0xFF)
								{
									lpObj->MapNumber = 2;
									MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
								}
								else
								{
									if(btTeam == TEAM_ALLIED)
									{	
										int iGateNum = GATE_ILLUSION_TEMPLE_ALLIED1 + (lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1);
										
										SHORT sX, sY, sLVL;
										BYTE btMAPNUM	= lpObj->MapNumber; 
										BYTE btDIR;
										INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
										if( iMapNumber >= 0 ) 
										{
											lpObj->MapNumber	= btMAPNUM;
											lpObj->X			= sX;
											lpObj->Y			= sY;
										}

										// ��ȣ��ų ���
										g_IllusionTempleEvent.IllusionTempleUserDieRegen(lpObj);
									}
									else if(btTeam == TEAM_ILLUSION)
									{
										int iGateNum = GATE_ILLUSION_TEMPLE_ILLUSION1 + (lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1);
										
										SHORT sX, sY, sLVL;
										BYTE btMAPNUM	= lpObj->MapNumber; 
										BYTE btDIR;
										INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
										if( iMapNumber >= 0 ) 
										{
											lpObj->MapNumber	= btMAPNUM;
											lpObj->X			= sX;
											lpObj->Y			= sY;
										}

										// ��ȣ��ų ���
										g_IllusionTempleEvent.IllusionTempleUserDieRegen(lpObj);
									}
									else
									{
										lpObj->MapNumber = 2;
										MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
									}
								}
							}
#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108					// ����Ǵ˿��� �������
							else if( lpObj->MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS )	
							{
								int iGateNum = 273;	// ����� �� �������� ����Ʈ ��ȣ
								
								SHORT sX, sY, sLVL;
								BYTE btMAPNUM	= lpObj->MapNumber; 
								BYTE btDIR;
								INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
								if( iMapNumber >= 0 ) 
								{
									lpObj->MapNumber	= btMAPNUM;
									lpObj->X			= sX;
									lpObj->Y			= sY;
								}
							}
#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328							
#ifdef ADD_RAKLION_20080408							// ��Ŭ���¿��� ���� ���
							else if( lpObj->MapNumber == MAP_INDEX_RAKLION_FIELD 
								|| lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )	
							{
								int iGateNum = 286;	// ��Ŭ���� ��������
								
								SHORT sX, sY, sLVL;
								BYTE btMAPNUM	= lpObj->MapNumber; 
								BYTE btDIR;
								INT iMapNumber	= gGateC.GetGate(iGateNum, sX, sY, btMAPNUM, btDIR, sLVL);
								if( iMapNumber >= 0 ) 
								{
									lpObj->MapNumber	= btMAPNUM;
									lpObj->X			= sX;
									lpObj->Y			= sY;
								}
							}
#endif	// ADD_RAKLION_20080408
							else // �ٸ����� ������ ���� ��������..
							{
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y );
							}

						}
						
#ifdef MAP_SERVER_WORK_20041030								// �ʼ����� �̵����� üũ -> �׾��ٰ� ��Ƴ� �� ������ ��ǥ�� ���� �ʼ��� �̵����� Ȯ��
						SHORT sSvrCode	= g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->MapNumber, lpObj->m_sPrevMapSvrCode);
						if (sSvrCode != gGameServerCode) {
							if (sSvrCode == -1) {
								// ġ������ ���� -> ���� ����
								LogAddC(LOGC_RED, "[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",
									lpObj->AccountID,
									lpObj->Name,
									lpObj->m_Index
									);
								continue;
							}

							// ���������� �ʼ��� �̵� ��û�� �Ѵ�.
							INT iObjDstMap				= lpObj->MapNumber;
							INT iObjDstX				= lpObj->X;
							INT iObjDstY				= lpObj->Y;
							lpObj->MapNumber			= iObjSrcMap;
							lpObj->X					= iObjSrcX;
							lpObj->Y					= iObjSrcY;

							GJReqMapSvrMove (lpObj->m_Index, sSvrCode, iObjDstMap, iObjDstX, iObjDstY);
							LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",
								sSvrCode,
								lpObj->AccountID,
								lpObj->Name,
								lpObj->m_Index
								);
							continue;
						}
#endif

						//LogAdd("MapNumber2 %d", lpObj->MapNumber);
						
						lpObj->TX   = lpObj->X;
						lpObj->TY   = lpObj->Y;						
						CreateFrustrum(lpObj->X,lpObj->Y, n);
						
						pMsg.h.c          = PMHCE_BYTE;
						pMsg.h.headcode   = 0xF3;
						pMsg.h.size       = sizeof( pMsg );
						pMsg.subcode      = 0x04;
						pMsg.Dir          = lpObj->Dir;
						pMsg.MapX         = (BYTE)lpObj->X;
						pMsg.MapY         = (BYTE)lpObj->Y;
						pMsg.Life         = (WORD)lpObj->Life;
						pMsg.Mana         = (WORD)lpObj->Mana;
						
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ĳ���� ����� ����ġ�� �����ش�.
						INT64 i64Exp = lpObj->Experience;
						if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
						{
							i64Exp = lpObj->m_i64MasterLevelExp;
						}
						else
						{
							i64Exp = lpObj->Experience;
						}
						// �����ͷ��� ������ ���� Ÿ�� ����						
						pMsg.btExp1			= HIBYTE(HIWORD(HILONG( i64Exp )));
						pMsg.btExp2			= LOBYTE(HIWORD(HILONG( i64Exp )));
						pMsg.btExp3			= HIBYTE(LOWORD(HILONG( i64Exp )));
						pMsg.btExp4			= LOBYTE(LOWORD(HILONG( i64Exp )));
						pMsg.btExp5			= HIBYTE(HIWORD(LOLONG( i64Exp )));
						pMsg.btExp6			= LOBYTE(HIWORD(LOLONG( i64Exp )));
						pMsg.btExp7			= HIBYTE(LOWORD(LOLONG( i64Exp )));
						pMsg.btExp8			= LOBYTE(LOWORD(LOLONG( i64Exp )));						
#else
						pMsg.Exp		  = lpObj->Experience;
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912
						pMsg.MapNumber    = lpObj->MapNumber;
						pMsg.Money		  = lpObj->Money;

#ifdef ADD_SHIELD_POINT_01_20060403
						pMsg.wShield	  = lpObj->iShield;
#endif

#ifdef NEW_FORSKYLAND2
						pMsg.BP			  = lpObj->BP;	
#endif
						lpObj->PathCount  = 0;
						
						DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);

#ifdef BUGFIX_UPDATE_VIEWPORT_INFO_FOR_SELF_20050510
						gObjViewportListProtocolCreate(lpObj);
#endif // BUGFIX_UPDATE_VIEWPORT_INFO_FOR_SELF_20050510

						// �巡�� �̺�Ʈ�� �������̸�..
						if( DragonEvent->GetState() != 0 )
						{
							if( DragonEvent->GetMapNumber() == lpObj->MapNumber )
							{
								GCMapEventStateSend(lpObj->MapNumber, 1, 1);
							}
						}
						gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);

#ifdef GMCOMMAND_EX_1ST_BUGFIX
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
						if( lpObj->Authority == AUTHORITY_EVENT_GM )
						{
							gObjAddBuffEffect( lpObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
						}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // GMCOMMAND_EX_1ST_BUGFIX

#ifdef CASTLE_MAIN_SCHEDULER_20041111				// �������� ���� ���̸� ����ڿ��� �˷��ش�.
						if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE) {
							GCAnsCsNotifyStart(lpObj->m_Index, (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)?1:0);
							if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
								g_CastleSiege.NotifySelfCsJoinSide (lpObj->m_Index);
								g_CastleSiege.NotifyCsSelfLeftTime (lpObj->m_Index);
							}
						}
#endif

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
						// ũ���̿��� ���� �߿� ������ �߻��Ѵٸ�,
						// ������ ó�� Ư�� ������ �˷���� �ϴ°�?
						
#endif

#ifdef FOR_JAPAN	
						// Ÿ��¯ �̺�Ʈ�� �������̸�
						if( gTamaJJangEvent )
						{
							if( gAppearTamaJJang && lpObj->MapNumber == 0 )
							{	// Ÿ��¯�� ���� �ְ�, �з��þƿ� ������
								GCMapEventStateSend(0, gAppearTamaJJang, 2);	
							}
						}
#endif

					}
					else 
					{	
						// �����Ϸ��� ���� ����� �̿��� ���̴�.

						/*if( lpObj->m_RecallMon >= 0 )
						{
							gObjMonsterCallKill(lpObj->m_RecallMon);
						}
						else */
						{
							if( !gObjMonsterRegen(lpObj) )
								continue;

							CreateFrustrum(lpObj->X,lpObj->Y, n);
						}
						
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
						if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
						{
							if(lpObj->Class == NPC_ILLUSION_HOLY_RELICS_STATUE)
							{
								LogAddTD("[Illusion Temple] (%d) Status Regen OK (%d: %d/%d)",
									lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1 +1, lpObj->MapNumber, lpObj->X, lpObj->Y);
							}
							
						}
#endif
					}
					lpObj->DieRegen	= 0;
					lpObj->m_State	= OBJST_CREATE;
				}
			}
		}
	}

#if ENABLE_MC_CODE == 1
	for (n = 0; n < g_TerrainManager.Size(); n++)
#else // ENABLE_MC_CODE == 1
	for (n = 0; n < MAX_MAP; n++)
#endif // ENABLE_MC_CODE == 1
	{
		MapC[n].StateSetDestroy();
	}
}


#ifdef GS_DS_LIVECHECK_20041008			// [0x58] GS -> DS �� LIVE ��ȣ�� ���������� ������.
#include "wsJoinServerCli.h"
#include "DBSockMng.h"
extern void GS_GDReqLiveCheck();
#endif


// 1�ʴ����� ����Ǵ� ����
void gObjSecondProc()
{
	int n;
	LPOBJECTSTRUCT	lpObj;
	
	if( gDisconnect == 1 ) return;	

#ifdef GS_DS_LIVECHECK_20041008			// [0x58] GS -> DS �� LIVE ��ȣ�� ���������� ������.
	// 1�� ������ DS üũ
	cDBSMng.CheckDBServerLivePerSec();
	// 1�� ������ DS ��ȣ ����
	GS_GDReqLiveCheck();
#endif

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
	// ���� ������ ��ŷ�� ��� ���� Ȯ�� ��û�� ���� ��� �ջ�
	g_HackToolBlock.CheckStatisticsResult();
	g_HackToolBlock.CheckBlockActive();
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef FOR_THAILAND	
	struct tm * today;
	time_t		ltime;				
	
	time( &ltime );
	today = localtime( &ltime );
#endif


#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	gObjSendTimeView(1);
	GuildWarAndBattleSoccerTimeCheck();
#endif
	
	for( n=0; n<MAX_OBJECT; n++ )
	{
		lpObj = &gObj[n];
				
		if( lpObj->Connected > 2 )
		{
#ifdef MAP_SERVER_WORK_20041030		// �ʼ��� �̵� �� ����üũ
			if (lpObj->m_bMapSvrMoveQuit == true) {
				if (GetTickCount() - lpObj->m_dwMapSvrQuitTick > 30*1000) {
					// �ʼ��� �̵��� ���۵��� 30�ʰ� ������ ������ ������ �ʴ´ٸ� ������ �����ش�. -> �������������� �˾Ƽ� ������ �˸��� �ʴ´�.
					gObjDel(lpObj->m_Index);
					continue;
				}
			}
#endif
			if(lpObj->m_bShell == true)
				continue;

			gObjSkillUseProc(lpObj);
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			gObjCheckBuffEffectList( lpObj );
#else
			gObjSkillBeAttackProc(lpObj);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef SECOND_QUEST
			if( lpObj->Type == OBJTYPE_NPC &&  lpObj->Class == NPC_QEUSTNPC2 )
			{
				gQeustNpcTeleport.Run(n);
			}
#endif

			if( lpObj->Type == OBJTYPE_MONSTER )
			{
#ifdef ADD_ABILLITY_KUNDUN_01_20040920
				// 6Į���� ����� ��� HP ȸ��
				if( lpObj->Class == 275 )
				{		
					// 0.01% �⺻ ü��ȸ��
#ifdef ADD_KUNDUN_CONTROL_20040923
					gObjRefillMonsterHP(lpObj, giKundunRefillHPSec);
#else
					gObjRefillMonsterHP(lpObj, 1);
#endif

				}
#endif
				
#ifdef CASTLE_NPC_GUARDIANSTATUE_WORK_20041207			// ��ȣ������ ����
				if (lpObj->Class == 283) {
					g_CsNPC_GuardianStatue.GuardianStatueAct(lpObj->m_Index);
					continue;
				}
#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208				// ���������� (����)�� ����
				if (lpObj->Class == NPC_INDEX_LIFESTONE ) 
				{
					g_CsNPC_LifeStone.LifeStoneAct(lpObj->m_Index);
					continue;
				}
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116			// ������� ����
				if( lpObj->Class == NPC_INDEX_GUARDIAN )
				{
					g_CsNPC_Guardian.GuardianAct(lpObj->m_Index);
					continue;
				}
#endif

#ifdef CASTLE_NPC_CANNON_TOWER_20041223					// ĳ��Ÿ���� ����
				if (lpObj->Class == 288) {
					g_CsNPC_CannonTower.CannonTowerAct(lpObj->m_Index);
					continue;
				}
#endif

#ifdef CRYWOLF_MONSTER_WORK_20050914
				g_Crywolf.CrywolfMonsterAct( lpObj->m_Index );
#endif
				
			}		
			
			if( lpObj->Type == OBJTYPE_NPC )
			{
				
#ifdef CRYWOLF_NPC_WORK_20050912						// ũ���̿��� NPC ����
				if( CHECK_CRYWOLF_SPECIAL_NPC(lpObj->Class) )
					g_Crywolf.CrywolfNpcAct( lpObj->m_Index );
#endif
				
#ifdef CASTLE_NPC_CROWN_WORK_20041210					// �հ��� ����
				if (lpObj->Class == CS_NPC_CROWN)
				{
					g_CsNPC_CastleCrown.CastleCrownAct(lpObj->m_Index);
					continue;
				}
#endif

#ifdef CASTLE_NPC_CROWN_SWITCH_WORK_20041210			// �հ� ����ġ�� ����
				if (CHECK_NPC_CROWN_SWITCH(lpObj->Class))
				{
					g_CsNPC_CastleCrownSwitch.CastleCrownSwitchAct(lpObj->m_Index);
					continue;
				}
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
				if( lpObj->Class == NPC_SIEGE_MACHINE_ATTACK || lpObj->Class == NPC_SIEGE_MACHINE_DEFENSE )
					g_CsNPC_Weapon.WeaponAct(lpObj->m_Index);
#endif
			}
	
			if( lpObj->Type == OBJTYPE_CHARACTER )
			{
				gObjManaPotionFill(lpObj);
				gObjRestPotionFill(lpObj);

#ifdef ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131
				gObjCheckXYMapTile(lpObj, DBGNAME_gObjSecondProc);
#else
				gObjCheckXYMapTile(lpObj);
#endif // ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131

#ifdef DARKLORD_WORK
				if( lpObj->ChatLitmitTime > 0 )
				{
					lpObj->ChatLimitTimeSec++;
					if( lpObj->ChatLimitTimeSec > 60 )
					{
						lpObj->ChatLimitTimeSec = 0;
						lpObj->ChatLitmitTime--;
						if( lpObj->ChatLitmitTime < 1 )
						{
							lpObj->ChatLitmitTime = 0;							
							MsgOutput(lpObj->m_Index,lMsg.Get(1141));	// ä�ñ����� �����Ǿ����ϴ�
						}
					}
				}
#endif

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
				if(g_iUseCharacterAutoRecuperationSystem && lpObj->Level <= g_iCharacterRecuperationMaxLevel) {
					gProcessAutoRecuperation(lpObj);
				}
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
				if( lpObj->Type == OBJTYPE_CHARACTER )
				{
					gObjShieldAutoRefill( lpObj );
				}
#endif
				if( lpObj->Type == OBJTYPE_CHARACTER )
				{
					if( lpObj->m_LastTeleportTime > 0 )
					{
						lpObj->m_LastTeleportTime--;
					}
				}
				
#ifdef NEW_FORSKYLAND3
				gObjDelayLifeCheck(n);
#endif

#ifdef APPLY_PACKETCHECKSUM_T_T
				lpObj->m_PacketChecksumTime++;
				if( lpObj->m_PacketChecksumTime > 300 )
				{
					lpObj->m_PacketChecksumTime = 0;
					gPacketCheckSum.Check(lpObj->m_Index);
				}
#endif
				// �ʴ� ������ ���� ó��(20030308)(foguy) 
				gObjSecondDurDown(lpObj);
				
				if( lpObj->PartyNumber>=0)
					gParty.PartyMemberLifeSend(lpObj->PartyNumber);
				
				// ������ �ŷ����̸� �������� �ִ� �ð��� ������Ų��.
				if( lpObj->m_ShopTime >= 1 ) 
				{
					if( lpObj->m_ShopTime < 255 ) lpObj->m_ShopTime++;
				}
				/*if( lpObj->m_TotalAttackCount > 0 )
				{
					if( (GetTickCount()-lpObj->m_TotalAttackTime) > (1000*5))
					{
						lpObj->m_TotalAttackTime = GetTickCount();
						//LogAdd("%s lpObj->m_TotalAttackCount : %d (%d %d %d)", lpObj->Name, lpObj->m_TotalAttackCount, lpObj->MapNumber, lpObj->X, lpObj->Y);
						lpObj->m_TotalAttackCount = 0;
					}
				}*/
				int GroundOutCheck = 1;
				
				int bt_time = gCheckBattleGroundTimer();

				//if( bt_time > 0 )
				{
					if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
					{
						GroundOutCheck = 0;					
				#ifdef WORLD_TOURNAMENT_EVENT_SETTING
						GCTimeViewSend(lpObj->m_Index, eMatchType_BattleSoccer, bt_time);
				#else
						GCTimeViewSend(lpObj->m_Index, bt_time);
				#endif
					}
				}

#ifdef FOR_ONAIR	// ��ۿ��϶� ����� �ȿ� �־ ������ ������ �ʴ´�
				GroundOutCheck = 0;
#endif

				if( lpObj->MapNumber == 6 && GroundOutCheck == 1 )
				{
					GroundOutCheck = 1;
					if( lpObj->lpGuild != NULL )
					{
						if( lpObj->lpGuild->WarState ) 
						{
							int ground = gCheckBattleGround(lpObj);
							if( ground != lpObj->lpGuild->BattleGroundIndex )
							{
								if( lpObj->lpGuild->WarType == BTT_SOCCER )
								{
									GroundOutCheck = 0;
								}
								else
								{
									GroundOutCheck = 0;
								}
							}
							else GroundOutCheck = 0;
						}
					}
					else 
					{
						GroundOutCheck = 1;
					}
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
					GroundOutCheck = 0;
#endif
					if( GroundOutCheck )
					{
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
						int ground = gCheckBattleGround(lpObj);
						if ( ground >= 0 )
						{
							gObjMoveGate(lpObj->m_Index, 82);
							//gObjTeleport(lpObj->m_Index, 6, 72, 161);
						}
#else
						int ground = gCheckBattleGround(lpObj);
						if ( ground >= 0 )
						{
							gObjMoveGate(lpObj->m_Index, 17);
							//gObjTeleport(lpObj->m_Index, 6, 72, 161);
						}
#endif
					}
				}				
			}
		}
		
		if( (lpObj->Connected >= CSS_LOGINOK
#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
			|| lpObj->Connected == CSS_LOGINWAIT
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
			) && (lpObj->Type == OBJTYPE_CHARACTER ) )
		{
			if( lpObj->CloseCount > 0 )
			{
				if( lpObj->CloseCount == 1 )	// ����
				{
					if( lpObj->CloseType == CL_BACK )
					{
						if( gObjGameClose(lpObj->m_Index) == TRUE)
						{
							GCCloseMsgSend(lpObj->m_Index, 0x01);
#ifdef MODIFY_UPDATE_MATCHDB_AT_CHARACTERLIST_VIEW_20060725
							GJUpdateMatchDBUserCharacters(lpObj);
#endif
						}
					}
					else if( lpObj->CloseType == CL_EXIT )
					{
						GCCloseMsgSend(lpObj->m_Index, 0x00);	// ���α׷��� �����϶�� �Ѵ�.
						CloseClient(lpObj->m_Index);
					}
					else if( lpObj->CloseType == CL_SERVERLIST )
					{
#ifdef MODIFY_NEWPVP_GAME_CLOSING_BUGFIX_20070715
						gObjGameClose(lpObj->m_Index);
#endif // MODIFY_NEWPVP_GAME_CLOSING_BUGFIX_20070715
						GCCloseMsgSend(lpObj->m_Index, 0x02);
					}
				}
				else
				{
					char szTemp[128];
					wsprintf(szTemp, lMsg.Get(1116), lpObj->CloseCount-1);
					GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				}
				lpObj->CloseCount--;
			}
		}
		
		if( (lpObj->Connected > 2 ) && (lpObj->Type == OBJTYPE_CHARACTER ) )
		{
#ifdef CHINA_HACKUSER_KICK_SYSTEM		// 1�п� �ѹ��� ����
			ChinaHackUserKick.ResetNCount(lpObj->m_Index);
#endif
			//DevGamesX 01/05/2018 - OffExp
			OffExp.TickTimes(n);
			// 10�п� �ѹ��� �ڵ� ����
			if( (GetTickCount()-lpObj->AutoSaveTime) > (1000*60*10) )
			{				
				GJSetCharacterInfo(lpObj, n);

#ifdef CHECK_ATTACK_AREA
	#ifdef MU_CHN_HACKLOG_MODIFY_20040930
				if( lpObj->m_NotAttackAreaCount > 5 )
				{
					INT iCheckTickCount = (GetTickCount() - lpObj->AutoSaveTime) / 1000;
					if (iCheckTickCount < 1)
						iCheckTickCount = 1;

		#ifdef CHINA_HACKUSER_KICK_SYSTEM		
					ChinaHackLogFile.Output("[%s][%s] Try Attack In Not Attack Area (%d)", 
										  lpObj->AccountID, 
										  lpObj->Name, 
										  lpObj->m_NotAttackAreaCount * 1000 / iCheckTickCount
										  );
		#else // else CHINA_HACKUSER_KICK_SYSTEM		
					LogAddTD("[%s][%s] Try Attack In Not Attack Area (%d)", 
							  lpObj->AccountID, 
							  lpObj->Name, 
							  lpObj->m_NotAttackAreaCount * 1000 / iCheckTickCount
							  );
		#endif // CHINA_HACKUSER_KICK_SYSTEM

				}			
				lpObj->m_NotAttackAreaCount = 0;
	#endif // MU_CHN_HACKLOG_MODIFY_20040930		
	
	#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
				if( lpObj->m_NotAttackAreaCount > 5 )
				{	
					INT iCheckTickCount = (GetTickCount() - lpObj->AutoSaveTime) / 1000;
					if (iCheckTickCount < 1)
						iCheckTickCount = 1;

		#ifdef CHINA_HACKUSER_KICK_SYSTEM		
					ChinaHackLogFile.Output("[%s][%s] Try Attack In Not Attack Area (%d) @%d", 
										    lpObj->AccountID, 
										    lpObj->Name, 
										    lpObj->m_NotAttackAreaCountFor10Min * 1000 / iCheckTickCount,
										    lpObj->MapNumber);
		#else // else CHINA_HACKUSER_KICK_SYSTEM	
					LogAddTD("[%s][%s] Try Attack In Not Attack Area (%d) @%d", 
							 lpObj->AccountID, 
							 lpObj->Name, 
							 lpObj->m_NotAttackAreaCount,
							 lpObj->MapNumber);
		#endif // CHINA_HACKUSER_KICK_SYSTEM

				}
				lpObj->m_NotAttackAreaCountFor10Min = 0;
	#endif // MODIFY_HACKLOG_OF_ATTACK_20050518
#endif // CHECK_ATTACK_AREA
				lpObj->AutoSaveTime = GetTickCount();
			}
			
			if(lpObj->CheckSumTime > 0)
			{
#ifdef MODIFY_JOIN_ERROR_BUGFIX_20071211
				if( (GetTickCount()-lpObj->CheckSumTime) > 5000 )
#else
				if( (GetTickCount()-lpObj->CheckSumTime) > 3000 )
#endif
				{
					if(lpObj->m_bShell == false)
					{
#ifdef MODIFY_JOIN_ERROR_BUGFIX_20071211
						LogAddTD("[%s][%s] CheckSumTime Error", lpObj->AccountID, lpObj->Name);
#endif
						CloseClient(n);
					}
				}
			}


			gObjPkDownTimeCheck(lpObj, 1);


			gObjInterfaceTimeCheck(lpObj);
			gObjTimeCheckSelfDefense(lpObj);	

			//DevGamesX 01/05/2018 - OffExp
			OffExp.Update(lpObj->m_Index, 1 );

			if(lpObj->m_bShell == false)
			{
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
				gObjNProtectGGCheckSum(n);
#else
	#if defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
				gObjNProtectGGCheckSum(n);
	#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
			}
#ifdef CHINA_GM_EVENT
			gObjGMEventProc(n);			
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
			gObjGMEventProc(n);
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
		}

#ifdef PCBANG_POINT_SYSTEM_20070206		// gObjSecondProc()
		g_PCBangPointSystem.Run( lpObj );
#endif

#ifdef FOR_THAILAND				
		if( (lpObj->Connected > 1 ) && (lpObj->Type == OBJTYPE_CHARACTER ) )
		{
			if( !lpObj->m_AuthAdult )
			{	// �̼������� ���
				char Msg[255];
				if( today->tm_hour >= 22 || today->tm_hour < 6)
				{	// �̼����ڰ� �����Ҽ� ���� �ð��̸� ������ ���´�
					if( lpObj->m_DisplayTeenLogOutMsg )
					{
						wsprintf(Msg, lMsg.Get(1609));
						lpObj->m_DisplayTeenLogOutMsg = FALSE;
					}					
							
					GCServerMsgStringSend(Msg, n, 0);
					lpObj->m_DisplayTeenLogOutMsg = TRUE;
					gObjCloseSet(n, CL_EXIT);
				}
				else if( today->tm_hour == 21 )
				{	// 9�� ���� 15�� ���� ���� �޽����� ������� �ȴ�
					if( today->tm_min >= 45 )
					{
						if( !lpObj->m_DisplayTeenLogOutMsg )
						{							
							wsprintf(Msg, lMsg.Get(1608), 60-today->tm_min);
							
							GCServerMsgStringSend(Msg, n, 0);
							lpObj->m_DisplayTeenLogOutMsg = TRUE;
						}
					}
				}
			}
		}
#endif
	
		if( lpObj->Connected == 1 || lpObj->Connected == 2 || lpObj->Connected == 3
#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
			|| lpObj->Connected == CSS_LOGINWAIT
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
			)
		{
			if( lpObj->Type == OBJTYPE_CHARACTER )
			{
				// ĳ���� ���� �� ���� �������� �Ѿ�ٸ�...
				if( lpObj->Connected >= CSS_CHARACTERVIEW )
				{

					//DevGamesX 01/05/2018 - OffExp
					OffExp.Update(lpObj->m_Index, 1 );

					if( (GetTickCount()-lpObj->ConnectCheckTime) > (1000*60) )	// 60�ʸ� �Ѿ�� �������� ����
					{	
						ResponErrorCloseClient(n);
						//CloseClient(n);
						//LogAddTD(lMsg.Get(542), lpObj->m_Index, lpObj->AccountID, lpObj->Name, lpObj->Ip_addr);
					}
				}
				else 
				{
					if( (GetTickCount()-lpObj->ConnectCheckTime) > (1000*30) )	// 30�ʸ� �Ѿ�� �������� ����
					{	
						ResponErrorCloseClient(n);
						//CloseClient(n);
						//LogAddTD(lMsg.Get(543), lpObj->m_Index, lpObj->AccountID, lpObj->Name, lpObj->Ip_addr);
					}
				}
			}
		}
	}

#ifdef FOR_JAPAN
	if( gTamaJJangEvent )
	{
		gTamaJJangTime--;
		if( gTamaJJangTime <= 0 )
		{
			if( gAppearTamaJJang == FALSE )
			{
				gAppearTamaJJang = TRUE;
				gTamaJJangTime = gTamaJJangKeepTime;
				LogAddTD("Appear TamaJJang : Keep Time (%d)", gTamaJJangTime);
			}
			else
			{
				gAppearTamaJJang = FALSE;
				gTamaJJangTime = gTamaJJangDisappearTime+(rand()%gTamaJJangDisappearTimeRandomRange);
				LogAddTD("DisAppear TamaJJang : Keep Time (%d)", gTamaJJangTime);
			}
			GCMapEventStateSend(0, gAppearTamaJJang, 2);
		}
	}
#endif

#ifdef AUTH_GAMESERVER	// �������������� ������ ����
	gGameServerAuth.SendInfo();	
#endif
}

//----------------------------------------------------------------------------
// ���� �ð����� ������ �÷��ش�.
void gObjManaPotionFill(LPOBJECTSTRUCT lpObj)
{
	lpObj->m_ManaFillCount++;
	float tmana;
	BOOL  ChangeMana = FALSE;
	if( lpObj->m_ManaFillCount > 2 )
	{
		lpObj->m_ManaFillCount = 0;
		if( lpObj->Mana < lpObj->MaxMana+lpObj->AddMana )
		{
#ifdef ADD_CASHITEM_4RD_MARK_20080723
			{
				float fPercent = 0.037f;
				if ( !CHECK_BLOODCASTLE(lpObj->MapNumber) && !CHECK_CHAOSCASTLE(lpObj->MapNumber) 
					&& !CHECK_KALIMAMAP(lpObj->MapNumber) && !CHECK_DEVILSQUARE(lpObj->MapNumber)
					&& !CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
					&& (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
					)
				{
					int nPercent = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_RECOVERY_MANA );
					if (nPercent > 0)
						fPercent += ((float)nPercent/100);
				}
				
				tmana = lpObj->Mana;
				tmana += (lpObj->MaxMana+lpObj->AddMana)*fPercent;
			}
#else
			tmana = lpObj->Mana;
			tmana += (lpObj->MaxMana+lpObj->AddMana)*0.037f;
#endif // ADD_CASHITEM_4RD_MARK_20080723

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			// ��ȭ�� ���� ��ȭ ������ �ɼ� : ���� �ڵ� ������ ���
			tmana += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP;
#endif	// ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
			if( lpObj->m_btRefillMPSocketOption > 0 )
			{
#ifdef MODIFY_BUGFIX_20081119	// ���� ���� �ڵ�ȸ������ �ɼ� ��ġ�� ����
				tmana += lpObj->m_btRefillMPSocketOption;
#else	// MODIFY_BUGFIX_20081119
				tmana += ( ( lpObj->MaxMana + lpObj->AddMana ) * lpObj->m_btRefillMPSocketOption ) / 100;
#endif	// MODIFY_BUGFIX_20081119
			}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710

			if( tmana > lpObj->MaxMana+lpObj->AddMana ) tmana = lpObj->MaxMana+lpObj->AddMana;
			lpObj->Mana = tmana;

			ChangeMana = TRUE;			
		}

#ifdef NEW_FORSKYLAND2
		int maxbp = lpObj->MaxBP+lpObj->AddBP;
		if( lpObj->BP < maxbp )
		{
			int addbp;
			BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

			int iaddbppoint = 2;
			if( attr&MAP_ATTR_SAFTYZONE )
			{	// �������뿡���� 5%����
				iaddbppoint = 5;				
			}
//----------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ����(grooving)
// AG ȸ�� ����
#ifdef ADD_BUFF_XMASEVENT_20080930
			if( true == gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SANTA_LUCKY) )
			{
				iaddbppoint = 5;

				if( attr&MAP_ATTR_SAFTYZONE )
				{	// �������뿡���� 10%����
					iaddbppoint = 10;				
				}
			}
#endif //ADD_BUFF_XMASEVENT_20080930
//<-----------
			if( lpObj->Class == CLASS_KNIGHT )
			{	
				addbp = iaddbppoint+maxbp*5/100;
			}
			else
			{
				addbp = iaddbppoint+maxbp*3/100;
			}
			
#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
			// ��ȥ�� ������ ����� ��� 
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SOUL_POTION ) == true )
	#else
			if( lpObj->m_iPotionSoulTime > 0  ) 
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			{
				addbp += CASTLE_SOULPOTION_AG_REFILL_POINT;
			}
#endif

			addbp += lpObj->SetOpIncAGValue;			// ��Ʈ�ɼ� AG������ ����

			if( addbp < 1 )	addbp = 1;			

			int bp = lpObj->BP;
			bp += addbp;
			if( bp > maxbp)
			{
				bp = maxbp;
			}
			lpObj->BP = bp;
			ChangeMana = TRUE;
		}
#endif

		if( ChangeMana )
		{
#ifdef NEW_FORSKYLAND2
			GCManaSend(lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
#else
			GCManaSend(lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, 0);
#endif
		}
	}
}

//----------------------------------------------------------------------------
// �޽��� ���Ҷ� ���� �ð����� HP�� �÷��ش�.
void gObjRestPotionFill(LPOBJECTSTRUCT lpObj)
{
	// ü���� ���ٸ� ����
	//if( (lpObj->MaxLife+lpObj->AddLife) == lpObj->Life ) return;

	if( lpObj->m_Rest )
	{
		float tlife;
		float tmana;
		int   percent=3;

		lpObj->m_LifeFillCount++;
		
		if( lpObj->m_LifeFillCount > 4 )
		{
			lpObj->m_LifeFillCount = 0;
			tlife  = lpObj->Life;
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Option3 )
				{
#ifdef MODIFY_ACCUMULATED_BUG_AMULETRING_WRONG_OPTION_01_20050531
					// ������ ������ �ڻ� �ɼ��� �ƴϴ�.
					if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type != MAKE_ITEMNUM(13,24) )
					{
						percent+=lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Option3;
					}
#else
					percent+=lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Option3;
#endif

				}
			}
			else if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Option3 )
				{
#ifdef MODIFY_ACCUMULATED_BUG_AMULETRING_WRONG_OPTION_01_20050531
					// ������ ������ �ڻ� �ɼ��� �ƴϴ�.
					if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type != MAKE_ITEMNUM(13,24) )
					{
						percent+=lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Option3;
					}
#else
					percent+=lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Option3;
#endif
				}
			}
			else if( lpObj->pInventory[EQUIPMENT_AMULET].IsItem() == TRUE )
			{
				if( lpObj->pInventory[EQUIPMENT_AMULET].m_Option3 )
				{
#ifdef MODIFY_ACCUMULATED_BUG_AMULETRING_WRONG_OPTION_01_20050531
					// ����� ����̴� �ڻ� �ɼ��� �ƴϴ�.
					if( lpObj->pInventory[EQUIPMENT_AMULET].m_Type != MAKE_ITEMNUM(13,28) )
					{
						percent+=lpObj->pInventory[EQUIPMENT_AMULET].m_Option3;
					}
#else
					percent+=lpObj->pInventory[EQUIPMENT_AMULET].m_Option3;
#endif
				}
			}
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// �нú� 1�� - ����� �ڵ� ȸ�� ��ȭ(�޽���)
			if( percent > 0 && lpObj->m_MPSkillOpt.iMpsAutoRecoverLife > 0 )
			{	// �ڻ����� �������� �����ϰ� ������� ����.
				percent += lpObj->m_MPSkillOpt.iMpsAutoRecoverLife;
			}		
#endif

#ifdef ADD_CASHITEM_4RD_MARK_20080723
			//�޽����� ���
			{
				int nPercent = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_RECOVERY_HP );
				if( nPercent > 0 )
				{
					percent += nPercent;
				}
			}
#endif // ADD_CASHITEM_4RD_MARK_20080723

#ifndef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818					// !! NOT
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
			if( lpObj->m_btRefillHPSocketOption > 0 )
			{
				percent += lpObj->m_btRefillHPSocketOption;
			}
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
			
			tlife += ((((lpObj->MaxLife+lpObj->AddLife))*percent)/100);
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			// ��ȭ�� ���� ��ȭ ������ �ɼ� : ����� �ڵ� ������ ���
			tlife += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP;
#endif	// ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
			if( lpObj->m_btRefillHPSocketOption > 0 )
			{
				tlife += lpObj->m_btRefillHPSocketOption;
			}
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

			if( tlife > (lpObj->MaxLife+lpObj->AddLife) ) tlife = (lpObj->MaxLife+lpObj->AddLife);
			lpObj->Life = tlife;

#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif

#ifdef ADD_CASHITEM_4RD_MARK_20080723
			{
				int nPercentMana = 3;
				int nPercent = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_RECOVERY_MANA );
				if (nPercent > 0)
					nPercentMana += nPercent;
				
				tmana = (lpObj->Mana+lpObj->AddMana);
				tmana += (((lpObj->MaxMana+lpObj->AddMana)*nPercentMana)/100);
			}
#else
			tmana = (lpObj->Mana+lpObj->AddMana);
			tmana += (((lpObj->MaxMana+lpObj->AddMana)*3)/100);
#endif // ADD_CASHITEM_4RD_MARK_20080723

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			// ��ȭ�� ���� ��ȭ ������ �ɼ� : ���� �ڵ� ������ ���
			tmana += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP;
#endif	// ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			if( tmana > (lpObj->MaxMana+lpObj->AddMana) ) tmana = lpObj->MaxMana+lpObj->AddMana;
			lpObj->Mana = tmana;

#ifdef NEW_FORSKYLAND2
			GCManaSend(lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
#else
			GCManaSend(lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, 0);
#endif
		}

	}
	else	// �޽��� ������ �ʴ��� ������ �÷��ִ� ������ ���� �ִٸ�..
	{
		int  percent=0;

		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Option3 )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability != 0.f )
				{
#ifdef MODIFY_ACCUMULATED_BUG_AMULETRING_WRONG_OPTION_01_20050531
					// ������ ������ �ڻ� �ɼ��� �ƴϴ�.
					if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type != MAKE_ITEMNUM(13,24) )
					{
						percent+=lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Option3;
					}
#else
					percent+=lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Option3;
#endif
				}
			}
		}
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Option3 )
			{
				if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability != 0.f )
				{
#ifdef MODIFY_ACCUMULATED_BUG_AMULETRING_WRONG_OPTION_01_20050531
					// ������ ������ �ڻ� �ɼ��� �ƴϴ�.
					if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type != MAKE_ITEMNUM(13,24) )
					{
						percent+=lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Option3;
					}
#else
					percent+=lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Option3;
#endif
				}
			}
		}
		if( lpObj->pInventory[EQUIPMENT_AMULET].IsItem() == TRUE )
		{
			if( lpObj->pInventory[EQUIPMENT_AMULET].m_Option3 )
			{
				if( lpObj->pInventory[EQUIPMENT_AMULET].m_Durability != 0.f )
				{
#ifdef MODIFY_ACCUMULATED_BUG_AMULETRING_WRONG_OPTION_01_20050531
					// ����� ����̴� �ڻ� �ɼ��� �ƴϴ�.
					if( lpObj->pInventory[EQUIPMENT_AMULET].m_Type != MAKE_ITEMNUM(13,28) )
					{
						percent+=lpObj->pInventory[EQUIPMENT_AMULET].m_Option3;
					}
#else
					percent+=lpObj->pInventory[EQUIPMENT_AMULET].m_Option3;
#endif;
				}
			}
		}
		if( lpObj->pInventory[EQUIPMENT_WING].IsItem() == TRUE )
		{
			if( lpObj->pInventory[EQUIPMENT_WING].m_Option3 )
			{
				if( lpObj->pInventory[EQUIPMENT_WING].m_Special[2] == AT_LIFE_REGENERATION)
				{
					if( lpObj->pInventory[EQUIPMENT_WING].m_Durability != 0.f )
					{
						percent+=lpObj->pInventory[EQUIPMENT_WING].m_Option3;
					}
				}
			}
		}

#ifdef ADD_CASHITEM_4RD_MARK_20080723
		{
			if ( !CHECK_BLOODCASTLE(lpObj->MapNumber) && !CHECK_CHAOSCASTLE(lpObj->MapNumber) 
				&& !CHECK_KALIMAMAP(lpObj->MapNumber) && !CHECK_DEVILSQUARE(lpObj->MapNumber)
				&& !CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
				&& (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
				)
			{
				int nPercent = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_RECOVERY_HP );
				if( nPercent > 0 )
					percent += nPercent;
			}
		}
#endif // ADD_CASHITEM_4RD_MARK_20080723

#ifndef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818						// !! NOT
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
		if( lpObj->m_btRefillHPSocketOption > 0 )
		{
			percent += lpObj->m_btRefillHPSocketOption;
		}
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

		if( percent  )
		{
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// �нú� 1�� - ����� �ڵ� ȸ�� ��ȭ
			if( lpObj->m_MPSkillOpt.iMpsAutoRecoverLife > 0 )
			{	// �ڻ����� �������� �����ϰ� ������� ����.
				percent += lpObj->m_MPSkillOpt.iMpsAutoRecoverLife;
			}		
#endif
			float tlife;
			//float tmana;
			
			lpObj->m_LifeFillCount++;
			if( lpObj->m_LifeFillCount > 6 )
			{
				lpObj->m_LifeFillCount = 0;
				tlife  = lpObj->Life;
				tlife += (((lpObj->MaxLife+lpObj->AddLife)*percent)/100);
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// ��ȭ�� ���� ��ȭ ������ �ɼ� : ����� �ڵ� ������ ���
				// ����� ȸ�� ������(����/�����)�� ���� �ϰ� ���� �ÿ��� ���� ��.
				tlife += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP;
#endif	// ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
				if( lpObj->m_btRefillHPSocketOption > 0 )
				{
					tlife += lpObj->m_btRefillHPSocketOption;
				}
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

				if( tlife > (lpObj->MaxLife+lpObj->AddLife) ) tlife = (lpObj->MaxLife+lpObj->AddLife);
				lpObj->Life = tlife;

#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
				GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif
		
				/*
				int tmana = 0;
				tmana = (lpObj->Mana+lpObj->AddMana);
				tmana += (((lpObj->MaxMana+lpObj->AddMana)*3)/100);
				if( tmana > (lpObj->MaxMana+lpObj->AddMana) ) tmana = lpObj->MaxMana+lpObj->AddMana;
				lpObj->Mana = tmana;
				GCManaSend(lpObj->m_Index, (WORD)lpObj->Mana, 0xFF, 0);		
				*/
			}
		}
	}
	
#ifdef ADD_SHIELD_POINT_01_20060403

	// CODEREVIEW_SHIELD : iFillShieldCount ��� �ڵ� ����
	if( lpObj->iFillShield > 0 )
	{
		int iFillSDPoint = 0;

		if ( lpObj->iFillShieldCount <= 0 )
		{
			lpObj->iFillShieldCount = 1;
		}

		iFillSDPoint= ( lpObj->iFillShieldMax / lpObj->iFillShieldCount );
		lpObj->iShield		= lpObj->iShield + iFillSDPoint;
		lpObj->iFillShield	= lpObj->iFillShield - iFillSDPoint;

		if( lpObj->iFillShield < iFillSDPoint )
		{
			iFillSDPoint = lpObj->iFillShield;
			lpObj->iFillShield = 0;
			if( iFillSDPoint < 0 )
			{
				iFillSDPoint = 0;
			}
		}

		if( lpObj->iShield > ( lpObj->iMaxShield + lpObj->iAddShield ) )
		{
			lpObj->iShield = ( lpObj->iMaxShield + lpObj->iAddShield );
			lpObj->iFillShield = 0;
			lpObj->iFillShieldMax = 0;
		}

		GCReFillSend( lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield );
	}
#endif
	
	if( lpObj->FillLife > 0.f )
	{
#ifdef ADD_HP_POTION_LEVEL1_20040401
		float cur = (lpObj->FillLifeMax*34)/100;

		if( lpObj->FillLifeCount == 2 )
		{
			cur = (lpObj->FillLifeMax*54)/100;
		}
#else
		float cur = (lpObj->FillLifeMax*30)/100;
#endif
#ifdef ADD_SHIELD_POINT_01_20060403
		if( lpObj->FillLifeCount == 4 )
		{
			cur = (lpObj->FillLifeMax*25)/100;
		}
#endif
		if( lpObj->FillLife < cur )
		{
			//lpObj->FillLife -= cur;
			cur = lpObj->FillLife;
			lpObj->FillLife = 0;
			if( cur < 0 ) cur = 0;
		}
		else
		{
			lpObj->FillLife -= cur;
		}
		lpObj->Life += cur;
		if( lpObj->Life > (lpObj->MaxLife+lpObj->AddLife) ) 
		{
			lpObj->Life = (lpObj->MaxLife+lpObj->AddLife);
			lpObj->FillLife = 0;	// �ѹ� �� ä������ ������.
		}
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
		GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif
	}
}

//----------------------------------------------------------------------------
// ���� ����Ѵ�.
void gObjUseDrink(LPOBJECTSTRUCT lpObj, int level)
{	
	int time;
	if( level == 2 )
	{	// ����� ����
		time = 90;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 1, time);
		lpObj->m_SkillAttack2 = 1;
		lpObj->m_SkillAttackTime2 = 90;
	}
	else if( level == 1 )
	{	// ����� �ø���	
		time = 180;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	// ��ȥ�� ���� ����
	else if( level == 77 )
	{
		time = CASTLE_SOULPOTION_ACTIVETIME;
		// Ŭ���̾�Ʈ���� �ٸ� ���� �����ش�.
		GCItemUseSpecialTimeSend(lpObj->m_Index, 2, time);
	}
#endif
	else
	{	// �Ϲ� ��
		time = 80;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
}

//----------------------------------------------------------------------------
// ���� �̵� ��Ŷ�� �����Ѵ�.
//----------------------------------------------------------------------------
int gObjCurMoveMake(BYTE path[], LPOBJECTSTRUCT lpObj)
{
	int cur      = lpObj->PathCur;
	int total    = lpObj->PathCount;
	int sendbyte = 1;
	
//	memset(path, 0, 8);

	path[0] = (lpObj->Dir<<4);			// ����
	path[0] |= (BYTE)((total-cur)&0x0f);		// �ڽ��� ��ġ ���� ����

	if( (total-cur) < 0 ) {
		MsgBox("error : %s %d",__FILE__, __LINE__);
		path[0] &= 0xF0;
		return sendbyte;
	}
	
	if( total == 0 ) {
		path[0] &= 0xF0;
		return sendbyte;	// �̵��� ���ߴٸ�
	}
	if( total == cur ) {
		path[0] &= 0xF0;
		return sendbyte;	// �̵��� �����ٸ�
	}
	
	for( int n=cur, i=1; n<total; n++, i++)
	{
		if( i%2 == 1 ) {
			path[(i+1)/2] = (lpObj->PathOri[n]<<4);
		}
		else { 
			path[(i+1)/2] |= (lpObj->PathOri[n]&0x0f);
		}
	}

	sendbyte += ((path[0]&0x0f)+1)/2;

//	LogAddHeadHex(1,(char*)path, 8);
//	LogAdd("path[0] : %x sendbyte:%d ",path[0], sendbyte);	
	return sendbyte;
}


BYTE  GuildInfoBuf[10000];
int	  GuildInfoOfs=0;
int   GuildInfoCount=0;
BYTE  GuildUserBuf[10000];
int	  GuildUserOfs=0;
int   GuildUserCount=0;
BYTE  SendGBufChange[5000];
int	  lOfsChange=0;
int   ChangeCount=0;

PMSG_VIEWPORTCREATE				pViewportCreate;
PMSG_VIEWPORTCREATE_CHANGE		pViewportCreateChange;
PMSG_VIEWPORTDESTROY			pViewportDestroy;
PMSG_MONSTER_VIEWPORTCREATE		pMonsterViewportCreate;
PMSG_CALLMONSTER_VIEWPORTCREATE pCallMonsterViewportCreate;

PWMSG_COUNT				pItemCount;
PMSG_ITEMVIEWPORTCREATE	pItemViewportCreate;
PMSG_VIEWPORTDESTROY	pItemViewportDestroy;


// �ڽ��� ���� �ִ� ��������� ������ �˸���.
void gObjViewportListProtocolDestroy(LPOBJECTSTRUCT lpObj)
{
	//if( lpObj->Type != OBJTYPE_CHARACTER ) return;
	BYTE			sendBuf[5000];
	int				lOfs;
	int				count      = 0;
	PBMSG_COUNT		pCount;
	
	lOfs			= sizeof( PBMSG_COUNT );

	pViewportDestroy.NumberH = HIBYTE(lpObj->m_Index);
	pViewportDestroy.NumberL = LOBYTE(lpObj->m_Index);
	memcpy((sendBuf+lOfs), (char*)&pViewportDestroy, sizeof(PMSG_VIEWPORTDESTROY));
	lOfs += sizeof(PMSG_VIEWPORTDESTROY);
	count++;

	pCount.h.c        = PMHC_BYTE;
	pCount.h.headcode = PMHC_VIEWPORTDESTROY;
	pCount.h.size     = lOfs;
	pCount.count      = count;
	memcpy(sendBuf, (char*)&pCount, sizeof(PBMSG_COUNT));
	
	if( count < 1 )
	{
		return;
	}
	MsgSendV2(lpObj, sendBuf, lOfs);

	//LogAdd("�信�� ����");
}


// �ڽ��� ����Ʈ ���� �۽�
void gObjViewportListProtocolCreate(LPOBJECTSTRUCT lpObj)
{
	int		n;
	int		tObjNum;
	BYTE	sendBuf[5000];
	int		lOfs;
	int		count      = 0;

#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724
	int		moncount = 0;
#endif
	
	PWMSG_COUNT			pwCount;
	lOfs       = sizeof( pwCount);

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	int iViewportSize = 0;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	if( lpObj->Type == OBJTYPE_CHARACTER ) 
	{
		if( lpObj->m_Change >= 0 )
		{
			pViewportCreateChange.NumberH	= HIBYTE(lpObj->m_Index);
			pViewportCreateChange.NumberL	= LOBYTE(lpObj->m_Index);
			
			pViewportCreateChange.NumberH |= (0x01<<7);	// ���� ������ ���̸�..

			//LogAdd("��ġ ���� : %d %d",lpObj->X, lpObj->Y);
			pViewportCreateChange.X			= (BYTE)lpObj->X;
			pViewportCreateChange.Y			= (BYTE)lpObj->Y;
			pViewportCreateChange.TX		= (BYTE)lpObj->TX;
			pViewportCreateChange.TY		= (BYTE)lpObj->TY;
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
			pViewportCreateChange.SkinH		= HIBYTE(LOWORD(lpObj->m_Change));
			pViewportCreateChange.SkinL		= LOBYTE(LOWORD(lpObj->m_Change));
#else
			pViewportCreateChange.Skin		= lpObj->m_Change;
#endif

#ifdef UPDATE_GM_FUNCTION_20070228
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_GM_MARK ) == true )
			{
				gObjAddBuffEffect( lpObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
			}
	#else
			if((lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)
			{
				pViewportCreateChange.ViewSkillState	|= STATE_ADD_GM_MARK;
				lpObj->m_ViewSkillState					|= STATE_ADD_GM_MARK;
			}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
			pViewportCreateChange.ViewSkillState = lpObj->m_ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			pViewportCreateChange.DirAndPkLevel = lpObj->Dir<<4;

			if(lpObj->m_stateEngagePVP == PVP_USER_PLAYING)
				pViewportCreateChange.DirAndPkLevel |= ((PK_LEVEL_DEFAULT+3)&0x0F);
			else
				pViewportCreateChange.DirAndPkLevel |= (lpObj->m_PK_Level&0x0F);

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3������ �߰��� m_ViewState ����
		lpObj->CharSet[0] &= 0xF8;						// 3�� �������� ������ ��ȣ
		lpObj->CharSet[0] |= ( lpObj->m_ViewState & 0x07 );	// m_ViewState�� ���� 3bit�� ����Ѵ�.
#else
#ifdef UPDATE_VIEWPORTCREATE_CHANGE_ADD_CHARSET_20070314
		lpObj->CharSet[0] &= 0xF0;
		lpObj->CharSet[0] |= (lpObj->m_ViewState&0x0F);
#endif
#endif	// THIRD_CHANGEUP_SYSTEM_20070507

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	#ifdef CHAOSCASTLE_SYSTEM_20040408		// !!> . ����ڰ� ī���� ĳ���ȿ� �ִٸ� ����Ʈ�� �����ϱ� ���� ��ų����Ʈ���� ����� �����Ѵ�.
			if (CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
//				pViewportCreateChange.ViewSkillState &= ~STATE_REDUCE_MAGICDEFENSE;		// �ҿ�ٸ���
//				pViewportCreateChange.ViewSkillState &= ~STATE_ADD_LIFE;				// ����������
//				pViewportCreateChange.ViewSkillState &= ~STATE_ATTACK;					// ���ݷ����
//				pViewportCreateChange.ViewSkillState &= ~STATE_DEFENSE;					// �������
				pViewportCreateChange.ViewSkillState = 0;
			}
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef UPDATE_VIEWPORTCREATE_CHANGE_ADD_CHARSET_20070314	// gObjViewportListProtocolCreate() ����Ʈ�� ĳ���� ��� �߰�
			memcpy(&pViewportCreateChange.CharSet, lpObj->CharSet, MAX_PREVIEWCHARSET+9);
#endif	

			memcpy(pViewportCreateChange.Id, lpObj->Name, MAX_IDSTRING);
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			iViewportSize = gObjMakeViewportState( lpObj, pViewportCreateChange.BuffEffectList );
			pViewportCreateChange.BuffEffectCount = iViewportSize;
			memcpy((sendBuf+lOfs), (char*)&pViewportCreateChange, ( sizeof(PMSG_VIEWPORTCREATE_CHANGE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) ) );
			//memcpy((sendBuf+lOfs), (char*)&pViewportCreateChange, ( sizeof(PMSG_VIEWPORTCREATE_CHANGE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) ) );
			lOfs += ( sizeof(PMSG_VIEWPORTCREATE_CHANGE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) );
#else
			memcpy((sendBuf+lOfs), (char*)&pViewportCreateChange, sizeof(PMSG_VIEWPORTCREATE_CHANGE));
			lOfs += sizeof(PMSG_VIEWPORTCREATE_CHANGE);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			count++;

			if( count < 1 )
			{
				return;
			}
			pwCount.h.headcode  = 0x45;
		}
		else
		{			
			pViewportCreate.NumberH = HIBYTE(lpObj->m_Index);
			pViewportCreate.NumberL = LOBYTE(lpObj->m_Index);
			
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// ����Ʈ ������ �� �� 3�� ������ ���¸� �����͸� �ʱ�ȭ���� �ʴ´�.
			lpObj->CharSet[0] &= 0xF8;	// 3�� ü���������� ȣ��
#else
			lpObj->CharSet[0] &= 0xF0;	
#endif
			
			if( lpObj->m_State == OBJST_CREATE )
			{
				if( !lpObj->Teleport )
				{
					pViewportCreate.NumberH |= (0x01<<7);	// ���� ������ ���̸�..
				}
			}

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
			pViewportCreate.ViewSkillState = lpObj->m_ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// m_ViewState ����
			lpObj->CharSet[0] |= (lpObj->m_ViewState&0x07);	// 3�� ü���������� ��ȣ
#else
			lpObj->CharSet[0] |= (lpObj->m_ViewState&0x0F);
#endif

			//LogAdd("��ġ ����2 : %d %d",lpObj->X, lpObj->Y);

			pViewportCreate.X       = (BYTE)lpObj->X;
			pViewportCreate.Y       = (BYTE)lpObj->Y;
			pViewportCreate.TX		= (BYTE)lpObj->TX;
			pViewportCreate.TY		= (BYTE)lpObj->TY;
			pViewportCreate.DirAndPkLevel = lpObj->Dir<<4;

			if(lpObj->m_stateEngagePVP == PVP_USER_PLAYING && lpObj->MapNumber != PVP_EVENT_MAP)
			{
				g_PvpEvent.DeleteUser(lpObj->m_Index);
			}

			if(lpObj->m_stateEngagePVP == PVP_USER_PLAYING)
				pViewportCreate.DirAndPkLevel |= ((PK_LEVEL_DEFAULT+3)&0x0F);
			else
				pViewportCreate.DirAndPkLevel |= (lpObj->m_PK_Level&0x0F);

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917			// !! NOT
	#ifdef CHAOSCASTLE_SYSTEM_20040408		// !!> . ����ڰ� ī���� ĳ���ȿ� �ִٸ� ����Ʈ�� �����ϱ� ���� ��ų����Ʈ���� ����� �����Ѵ�.
			if (CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
//				pViewportCreateChange.ViewSkillState &= ~STATE_REDUCE_MAGICDEFENSE;		// �ҿ�ٸ���
//				pViewportCreateChange.ViewSkillState &= ~STATE_ADD_LIFE;				// ����������
//				pViewportCreateChange.ViewSkillState &= ~STATE_ATTACK;					// ���ݷ����
//				pViewportCreateChange.ViewSkillState &= ~STATE_DEFENSE;					// �������
				pViewportCreate.ViewSkillState = 0;
			}
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef ITEM_INDEX_EXTEND_20050706
				memcpy(&pViewportCreate.CharSet, lpObj->CharSet, MAX_PREVIEWCHARSET+9);
#else
#ifdef DARKLORD_WORK
				memcpy(&pViewportCreate.CharSet, lpObj->CharSet, MAX_PREVIEWCHARSET+4);
#else
				memcpy(&pViewportCreate.CharSet, lpObj->CharSet, MAX_PREVIEWCHARSET+3);
#endif
#endif

			memcpy(pViewportCreate.Id, lpObj->Name, MAX_IDSTRING);
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			iViewportSize = gObjMakeViewportState( lpObj, pViewportCreate.BuffEffectList );
			pViewportCreate.BuffEffectCount = iViewportSize;
			//memcpy((sendBuf+lOfs), (char*)&pViewportCreate, ( sizeof(PMSG_VIEWPORTCREATE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) ) );
			memcpy((sendBuf+lOfs), (char*)&pViewportCreate, sizeof(PMSG_VIEWPORTCREATE) );
			lOfs += ( sizeof(PMSG_VIEWPORTCREATE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) );
#else
			memcpy((sendBuf+lOfs), (char*)&pViewportCreate, sizeof(PMSG_VIEWPORTCREATE));
			lOfs += sizeof(PMSG_VIEWPORTCREATE);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			count++;

			if( count < 1 )
			{
				return;
			}
			pwCount.h.headcode  = PMHC_VIEWPORTCREATE;
		}

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004			// !! NOT
	#ifdef UPDATE_GM_FUNCTION_20070228
		if((lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)
		{
			pViewportCreate.ViewSkillState			|= STATE_ADD_GM_MARK;
			lpObj->m_ViewSkillState					|= STATE_ADD_GM_MARK;
		}
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

		pwCount.h.c         = PMHC_WORD;
		pwCount.h.sizeH     = HIBYTE(lOfs);
		pwCount.h.sizeL     = LOBYTE(lOfs);
		pwCount.count       = count;
		memcpy(sendBuf, (char*)&pwCount, sizeof(PWMSG_COUNT));
		DataSend(lpObj->m_Index, (LPBYTE)sendBuf, lOfs);
		
#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
// �Լ��� ���ӽ����̽��� ���´�.
		if( lpObj->GuildNumber )
		{
			GuildInfoOfs=0;
			GuildInfoOfs=sizeof(PMSG_SIMPLE_GUILDVIEWPORT_COUNT);

			PMSG_SIMPLE_GUILDVIEWPORT		pMsg;

#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
			pMsg.GuildNumber= lpObj->lpGuild->Number;
#else
			pMsg.GNumberH	= HIBYTE(lpObj->lpGuild->Number);
			pMsg.GNumberL	= LOBYTE(lpObj->lpGuild->Number);
#endif
			pMsg.NumberH	= HIBYTE(lpObj->m_Index)&0x7f;
			pMsg.NumberL	= LOBYTE(lpObj->m_Index);
			if(!strcmp(lpObj->lpGuild->Names[0], lpObj->Name))
			{	// ��� ������ �̸�
				pMsg.NumberH |= 0x80;
			}
		
#ifdef ADD_NEW_GUILD_02_20040922
			pMsg.btGuildType = lpObj->lpGuild->btGuildType;
#endif
			
#ifdef ADD_NEW_GUILD_01_20040913
			pMsg.btGuildStatus = lpObj->GuildStatus;
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

			memcpy(GuildInfoBuf+GuildInfoOfs, &pMsg, sizeof(pMsg));
			GuildInfoOfs += sizeof( pMsg );
			
			PMSG_SIMPLE_GUILDVIEWPORT_COUNT pGVCount;
			
			pGVCount.h.c		= PMHC_WORD;
			pGVCount.h.headcode = 0x65;
			pGVCount.h.sizeH	= HIBYTE( GuildInfoOfs );
			pGVCount.h.sizeL	= LOBYTE( GuildInfoOfs );
			pGVCount.Count		= 1;
			memcpy(GuildInfoBuf, &pGVCount, sizeof(pGVCount));
			DataSend(lpObj->m_Index, (LPBYTE)GuildInfoBuf, GuildInfoOfs);
		}
		
#else // MODIFY_GUILD_VIEWPORT_01_20040701
		
		//ViewGuildMng.Init();
		GuildInfoOfs=0;
		GuildInfoCount=0;
		GuildUserOfs=0;
		GuildUserCount=0;
		
		
		GuildInfoOfs=sizeof(PMSG_GUILDVIEWPORT_COUNT);
		GuildUserOfs=sizeof(PMSG_GUILDVIEWPORT_USERCOUNT);

		// ���� ��忡 ���ԵǾ� �ִٸ�..
		if( lpObj->GuildNumber )
		{
			PMSG_GUILDVIEWPORT_USER pGVUMsg;
			PMSG_GUILDVIEWPORT		pMsg;
			
			pMsg.NumberH = HIBYTE(lpObj->lpGuild->Number);
			pMsg.NumberL = LOBYTE(lpObj->lpGuild->Number);		
			strcpy(pMsg.GuildName, lpObj->lpGuild->Name);
			memcpy(pMsg.Mark, lpObj->lpGuild->Mark, 32);
			memcpy(GuildInfoBuf+GuildInfoOfs, &pMsg, sizeof(pMsg));
			GuildInfoOfs += sizeof(pMsg);
			GuildInfoCount++;
		

	#ifdef GUILD_VIEWPORT_MODIFY
			pGVUMsg.NumberH = HIBYTE(lpObj->m_Index)&0x7f;
			pGVUMsg.NumberL = LOBYTE(lpObj->m_Index);
			if(!strcmp(lpObj->lpGuild->Names[0], lpObj->Name))
			{	// ��� ������ �̸�
				pGVUMsg.NumberH |= 0x80;
			}
	#else
			pGVUMsg.NumberH = HIBYTE(lpObj->m_Index);
			pGVUMsg.NumberL = LOBYTE(lpObj->m_Index);
	#endif

			pGVUMsg.GNumberH = HIBYTE(lpObj->lpGuild->Number);
			pGVUMsg.GNumberL = LOBYTE(lpObj->lpGuild->Number);						
			memcpy(GuildUserBuf+GuildUserOfs, &pGVUMsg, sizeof(pGVUMsg));
			GuildUserOfs += sizeof(pGVUMsg);
			GuildUserCount++;

			// ��� ������ �̸� ������..
			if ( GuildInfoCount )
			{
				PMSG_GUILDVIEWPORT_COUNT pGVCount;
				pGVCount.h.c		= PMHC_WORD;
				pGVCount.h.headcode = 0x5A;
				pGVCount.h.sizeH	= HIBYTE( GuildInfoOfs );
				pGVCount.h.sizeL	= LOBYTE( GuildInfoOfs );
				pGVCount.Count		= GuildInfoCount;
				memcpy(GuildInfoBuf, &pGVCount, sizeof(pGVCount));
			}
			// ��� ������ ������.
			if( GuildUserCount )
			{
				PMSG_GUILDVIEWPORT_USERCOUNT pGUCount;
				pGUCount.h.c		= PMHC_WORD;
				pGUCount.h.headcode = 0x5B;
				pGUCount.h.sizeH	= HIBYTE( GuildUserOfs );
				pGUCount.h.sizeL	= LOBYTE( GuildUserOfs );
				pGUCount.Count		= GuildUserCount;
				memcpy(GuildUserBuf, &pGUCount, sizeof(pGUCount));
				DataSend(lpObj->m_Index, (LPBYTE)GuildUserBuf, GuildUserOfs);
			}
		}
#endif //MODIFY_GUILD_VIEWPORT_01_20040701


// �ѱ�, �߱�, �۷ι��� ������ ������ �Ⱥ����� �ȴ�		
#ifdef FOR_KOREA
#ifdef UPDATE_GM_FUNCTION_20070228	// '/����' '/��������'
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( lpObj->Authority != AUTHORITY_ADMIN
			&& ( ( ( lpObj->Authority & AUTHORITY_EVENT_GM ) != AUTHORITY_EVENT_GM )
			|| ( ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INVISABLE ) ) ) ) )
	#else
		if( lpObj->Authority != AUTHORITY_ADMIN
			&&  ( ( ( lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM) 
			|| ( (lpObj->m_ViewSkillState&STATE_CLOAKING) != STATE_CLOAKING ) ))
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#else
		//if( lpObj->Authority != AUTHORITY_ADMIN )
#endif	// UPDATE_GM_FUNCTION_20070228
#endif
#ifdef FOR_CHINA
		if( lpObj->Authority != AUTHORITY_ADMIN )
#endif
#ifdef FOR_GLOBAL
		if( lpObj->Authority != AUTHORITY_ADMIN )
#endif
#ifdef FOR_USA
		if( lpObj->Authority != AUTHORITY_ADMIN )
#endif

#ifdef FOR_PHILIPPINE	// �ʸ���
		if( lpObj->Authority != AUTHORITY_ADMIN )
#endif

#ifdef FOR_VIETNAM		// ��Ʈ��
		if( lpObj->Authority != AUTHORITY_ADMIN )
#endif

#ifdef FOR_TAIWAN
		if( lpObj->Authority != AUTHORITY_ADMIN )
#endif
		{		
			for( n=0; n<MVL; n++)
			{
				if( lpObj->VpPlayer2[n].state ) 
				{
					tObjNum = lpObj->VpPlayer2[n].number;
					if( tObjNum >= 0 )
					{
						if(lpObj->VpPlayer2[n].type == OBJTYPE_CHARACTER)
						{
							DataSend(tObjNum, (LPBYTE)sendBuf, lOfs);
							if( GuildInfoCount )
							{
								DataSend(tObjNum, (LPBYTE)GuildInfoBuf, GuildInfoOfs);
							}
							if( GuildUserCount )
							{
								DataSend(tObjNum, (LPBYTE)GuildUserBuf, GuildUserOfs);
							}
						}
					}
				}
			}
		}
	}
	else
	{	
		count = 0;
		if( lpObj->m_RecallMon >= 0 )
		{
			pCallMonsterViewportCreate.NumberH = HIBYTE(lpObj->m_Index);
			pCallMonsterViewportCreate.NumberL = LOBYTE(lpObj->m_Index);
			if( lpObj->m_State == OBJST_CREATE )
				pCallMonsterViewportCreate.NumberH |= (0x01<<7);	// ���� ������ ���̸�..
								
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
			pCallMonsterViewportCreate.Type_HI    = HIBYTE(lpObj->Class);
			pCallMonsterViewportCreate.Type_LO    = LOBYTE(lpObj->Class);
#else
			pCallMonsterViewportCreate.Type		  = lpObj->Class;
#endif
#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					 // !! NOT
			pCallMonsterViewportCreate.ViewState = lpObj->m_ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			pCallMonsterViewportCreate.X       = (BYTE)lpObj->X;
			pCallMonsterViewportCreate.Y       = (BYTE)lpObj->Y;
			pCallMonsterViewportCreate.TX      = (BYTE)lpObj->TX;
			pCallMonsterViewportCreate.TY      = (BYTE)lpObj->TY;
			pCallMonsterViewportCreate.Path    = lpObj->Dir<<4;

			if( lpObj->m_RecallMon >= 0 && lpObj->m_RecallMon < MAX_OBJECT-1 )
			{
				memcpy(pCallMonsterViewportCreate.Id, gObj[lpObj->m_RecallMon].Name, MAX_IDSTRING);
			}
			else memset(pCallMonsterViewportCreate.Id, 0, MAX_IDSTRING);								
									
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			iViewportSize = gObjMakeViewportState( lpObj, pCallMonsterViewportCreate.BuffEffectList );
			pCallMonsterViewportCreate.BuffEffectCount = iViewportSize;
			memcpy((sendBuf+lOfs), (char*)&pCallMonsterViewportCreate, sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE) );
			lOfs += ( sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) );
#else
			memcpy((sendBuf+lOfs), (char*)&pCallMonsterViewportCreate, sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE));
			lOfs += sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			count++;
		}
#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724		
		else
		{
			pMonsterViewportCreate.NumberH = HIBYTE(lpObj->m_Index);
			pMonsterViewportCreate.NumberL = LOBYTE(lpObj->m_Index);
			if( lpObj->m_State == OBJST_CREATE )
			{
				if ( !lpObj->Teleport )
				{
					pMonsterViewportCreate.NumberH |= (0x01<<7);	// ���� ������ ���̸�..

					// �ڷ���Ʈ �� ���¿��ٸ�. NumberH�� ���� 2bit���� 1�����Ѵ�. ���� 1bit�� ���λ������η� ���. 
					// ���� 2bit������ ��밡����. 2bit | 4bit : 0 ~ 16383 ��ȣ ����.
					pMonsterViewportCreate.NumberH |= (0x01<<6);
				}

			}
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
			pMonsterViewportCreate.Type_HI    = HIBYTE(lpObj->Class);
			pMonsterViewportCreate.Type_LO    = LOBYTE(lpObj->Class);
#else
			pMonsterViewportCreate.Type		  = lpObj->Class;
#endif						


			//pMonsterViewportCreate.ViewState = lpTargetObj->m_ViewState;
#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					 // !! NOT
			pMonsterViewportCreate.ViewState = lpObj->m_ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			pMonsterViewportCreate.X       = (BYTE)lpObj->X;
			pMonsterViewportCreate.Y       = (BYTE)lpObj->Y;
			pMonsterViewportCreate.TX      = (BYTE)lpObj->TX;
			pMonsterViewportCreate.TY      = (BYTE)lpObj->TY;
			pMonsterViewportCreate.Path    = lpObj->Dir<<4;

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			iViewportSize = gObjMakeViewportState( lpObj, pMonsterViewportCreate.BuffEffectList );
			pMonsterViewportCreate.BuffEffectCount = iViewportSize;
			memcpy((sendBuf+lOfs), (char*)&pMonsterViewportCreate, sizeof(PMSG_MONSTER_VIEWPORTCREATE) );
			lOfs += ( sizeof(PMSG_MONSTER_VIEWPORTCREATE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) );
#else 
			memcpy((sendBuf+lOfs), (char*)&pMonsterViewportCreate, sizeof(PMSG_MONSTER_VIEWPORTCREATE));
			lOfs += sizeof(PMSG_MONSTER_VIEWPORTCREATE);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
			moncount++;
		}
#endif // MODIFY_NEW_MONSTER_SKILL_01_20060724
		if( count > 0 )
		{
			PWMSG_COUNT			pwCount;
			pwCount.h.c         = PMHC_WORD;
			pwCount.h.headcode  = 0x1F;
			pwCount.h.sizeH     = HIBYTE(lOfs);
			pwCount.h.sizeL     = LOBYTE(lOfs);
			pwCount.count       = count;
			memcpy(sendBuf, (char*)&pwCount, sizeof(PWMSG_COUNT));
			MsgSendV2(lpObj, sendBuf, lOfs);
		}
#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724
		// ���Ͱ� �����Ǿ��ٸ�
		if( moncount > 0 )
		{
			PWMSG_COUNT			pwCount;
			pwCount.h.c         = PMHC_WORD;
			pwCount.h.headcode  = PMHC_VIEWPORTMONSTERCREATE;
			pwCount.h.sizeH     = HIBYTE(lOfs);
			pwCount.h.sizeL     = LOBYTE(lOfs);
			pwCount.count       = moncount;
			memcpy(sendBuf, (char*)&pwCount, sizeof(PWMSG_COUNT));
			MsgSendV2(lpObj, sendBuf, lOfs);
		}

#endif // MODIFY_NEW_MONSTER_SKILL_01_20060724
	}
}


//////////////////////////////////////////////////////////////////////////////
// ����Ʈ�� ���õ� ���������� ó���Ѵ�.
void gObjViewportListProtocol(short aIndex)
{
	int n;
	
	if( gObj[aIndex].Connected < 3 ) return;

	PBMSG_COUNT				pCount;
	
	LPOBJECTSTRUCT			lpTargetObj;
	LPOBJECTSTRUCT			lpObj;

	lpObj = &gObj[aIndex];

	int	  lOfs_Item;
	int   count_Item;

	int   ItemStructSize = sizeof(pItemViewportCreate);
	int   lOfs;
	int	  MonlOfs;
	int   callMonlOfs;

	int   pMsgSize;
	short tObjNum;
	BYTE  count;
	BYTE  monstercount;
	BYTE  callmonstercount;
	BYTE  sendBuf[5000];
	BYTE  MonstersendBuf[5000];
	BYTE  callMonstersendBuf[5000];
	BYTE  ItemBuf[5000];
	
	GuildInfoOfs=0;
	GuildInfoCount=0;
	GuildUserOfs=0;
	GuildUserCount=0;
	ChangeCount=0;

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
	GuildInfoOfs=sizeof(PMSG_SIMPLE_GUILDVIEWPORT_COUNT);
#else
	GuildInfoOfs=sizeof(PMSG_GUILDVIEWPORT_COUNT);
#endif

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	int		iViewportSize = 0;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	GuildUserOfs=sizeof(PMSG_GUILDVIEWPORT_USERCOUNT);

	lOfsChange=sizeof( PWMSG_COUNT );

	//////////////////////////////////////////////////////////////////////////////
	// ����Ʈ���� ����� ����ڸ� �˸���.
	lOfs       = sizeof( PBMSG_COUNT );
	lOfs_Item  = sizeof( PWMSG_COUNT );
	count      = 0;
	count_Item = 0;

	if( lpObj->Type == OBJTYPE_CHARACTER ) // ����� �϶�..
	{
		for( n=0; n<MAXVIEWPORTOBJECT; n++)
		{
			if( lpObj->VpPlayer[n].state == STVP_DESTROY ) 
			{
				tObjNum = lpObj->VpPlayer[n].number;
				if( tObjNum >= 0 )
				{
					switch( lpObj->VpPlayer[n].type )
					{
					case OBJTYPE_CHARACTER :
					case OBJTYPE_MONSTER :
					case OBJTYPE_NPC :
						pViewportDestroy.NumberH = HIBYTE(tObjNum);
						pViewportDestroy.NumberL = LOBYTE(tObjNum);
						memcpy((sendBuf+lOfs), (char*)&pViewportDestroy, sizeof(PMSG_VIEWPORTDESTROY));
						lOfs += sizeof(PMSG_VIEWPORTDESTROY);
						count++;
						break;
					case OBJTYPE_ITEM :
						pItemViewportDestroy.NumberH = HIBYTE(tObjNum);
						pItemViewportDestroy.NumberL = LOBYTE(tObjNum);
						memcpy((ItemBuf+lOfs_Item), (char*)&pItemViewportDestroy, sizeof(pItemViewportDestroy));
						lOfs_Item += sizeof(PMSG_VIEWPORTDESTROY);
						count_Item++;
						break;
					}				
				}
				lpObj->VpPlayer[n].state = STVP_NONE;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount--;
			}
		}
	}
	else if( lpObj->Type == OBJTYPE_MONSTER || lpObj->Type ==  OBJTYPE_NPC)	// ���� �� NPC �϶�
	{
		for( n=0; n<MAX_MONVIEWPORTOBJECT; n++)
		{
			if( lpObj->VpPlayer[n].state == STVP_DESTROY ) 
			{
				lpObj->VpPlayer[n].state = STVP_NONE;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount--;
			}
		}
	}

	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		// �����Ȱ��� �ִٸ�
		if( count > 0 )	
		{	
			pCount.h.c        = PMHC_BYTE;
			pCount.h.headcode = PMHC_VIEWPORTDESTROY;
			pCount.h.size     = lOfs;
			pCount.count      = count;
			memcpy(sendBuf, (char*)&pCount, sizeof(PBMSG_COUNT));
			DataSend(aIndex, sendBuf, lOfs);
		}
		// ������ �������� �ִٸ�
		if( count_Item > 0 )
		{		
			pItemCount.h.c        = PMHC_WORD;
			pItemCount.h.headcode = 0x21;
			
			pItemCount.h.sizeH    = HIBYTE(lOfs_Item);
			pItemCount.h.sizeL    = LOBYTE(lOfs_Item);
			pItemCount.count      = count_Item;
			memcpy(ItemBuf, (char*)&pItemCount, sizeof(pItemCount));
			DataSend(aIndex, ItemBuf, lOfs_Item);
		}	
	}

	if( lpObj->Type == OBJTYPE_MONSTER || 
		lpObj->Type == OBJTYPE_NPC)
	{
		for( n=0; n<MAX_MONVIEWPORTOBJECT; n++)
		{
			if( lpObj->VpPlayer[n].state == STVP_CREATE ) 
			{
				lpObj->VpPlayer[n].state = STVP_PLAYING;
			}
		}
		return;
	}

	//------------------------------------------------------------------------
	// ���� viewport �ȿ��ִ� �ٸ� �÷��̾�鿡�� �˸���.
	//------------------------------------------------------------------------
	count        = 0;
	monstercount = 0;
	count_Item   = 0;
	callmonstercount = 0;
	
	callMonlOfs = MonlOfs = lOfs  = sizeof( PWMSG_COUNT );
	lOfs_Item		= sizeof( PWMSG_COUNT );
	pMsgSize		= sizeof( PMSG_VIEWPORTCREATE );

	if( lpObj->Type == OBJTYPE_CHARACTER )
	{

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
		// �ʿ� ����.
#else
		ViewGuildMng.Init();
#endif

		for( n=0; n<MAXVIEWPORTOBJECT; n++)
		{
			if( lpObj->VpPlayer[n].state == STVP_CREATE ) 
			{
				tObjNum = lpObj->VpPlayer[n].number;
#ifdef FOR_KOREA	// �ѱ��� ������ ������ ���̸� �ȵȴ� 
				//if( gObj[tObjNum].Authority == AUTHORITY_ADMIN )
				//	continue;
#endif

#ifdef FOR_CHINA	// �߱��� ������ ������ ���̸� �ȵȴ� 
				if( gObj[tObjNum].Authority == AUTHORITY_ADMIN )
					continue;
#endif

#ifdef FOR_GLOBAL	// �۷ι��� ������ ������ ���̸� �ȵȴ� 
				if( gObj[tObjNum].Authority == AUTHORITY_ADMIN )
					continue;
#endif

#ifdef FOR_USA		// �̱��� ������ ������ ���̸� �ȵȴ� 
				if( gObj[tObjNum].Authority == AUTHORITY_ADMIN )
					continue;
#endif

#ifdef FOR_PHILIPPINE	// �ʸ���
				if( gObj[tObjNum].Authority == AUTHORITY_ADMIN )
					continue;
#endif

#ifdef FOR_VIETNAM		// ��Ʈ��
				if( gObj[tObjNum].Authority == AUTHORITY_ADMIN )
					continue;
#endif

#ifdef FOR_TAIWAN	// �߱��� ������ ������ ���̸� �ȵȴ� 
				if( gObj[tObjNum].Authority == AUTHORITY_ADMIN )
					continue;
#endif

#ifdef UPDATE_GM_FUNCTION_20070228		// "/����" �ϰ�� ���̸� �ȵ�
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				if( ( ( gObj[tObjNum].Authority&AUTHORITY_EVENT_GM ) == AUTHORITY_EVENT_GM )
					&& gObjCheckUsedBuffEffect( &gObj[tObjNum], BUFFTYPE_INVISABLE ) == true 
				  )
				{
					continue;
				}
	#else
				if( ( (gObj[tObjNum].Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM) 
					&&( (gObj[tObjNum].m_ViewSkillState&STATE_CLOAKING) == STATE_CLOAKING ) )
					continue;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif

#ifdef ADD_NEWPVP_PKFIELD
				if (g_NewPVP.IsObserver(gObj[tObjNum]))
					continue;
#endif // ADD_NEWPVP_PKFIELD

				if( tObjNum >= 0 )
				{
					switch( lpObj->VpPlayer[n].type )
					{
					case OBJTYPE_CHARACTER :
						lpTargetObj = &gObj[tObjNum];
						
						if( lpTargetObj->m_Change >= 0 )
						{
							pViewportCreateChange.NumberH	= HIBYTE(tObjNum);
							pViewportCreateChange.NumberL	= LOBYTE(tObjNum);

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� �������� CharSet[0] ������ ��ȣ
							lpTargetObj->CharSet[0] &= 0xF8;
#elif GAME_VERSION >= G_V_99B
							lpTargetObj->CharSet[0] &= 0xF0;
#endif	// THIRD_CHANGEUP_SYSTEM_20070507
							if( lpTargetObj->m_State == OBJST_CREATE )
							{
								if( !lpTargetObj->Teleport )
								{
									pViewportCreateChange.NumberH |= (0x01<<7);	// ���� ������ ���̸�..
								}
							}
							pViewportCreateChange.X			= (BYTE)lpTargetObj->X;
							pViewportCreateChange.Y			= (BYTE)lpTargetObj->Y;
							pViewportCreateChange.TX		= (BYTE)lpTargetObj->TX;
							pViewportCreateChange.TY		= (BYTE)lpTargetObj->TY;
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
							pViewportCreateChange.SkinH		= HIBYTE(LOWORD(lpTargetObj->m_Change));
							pViewportCreateChange.SkinL		= LOBYTE(LOWORD(lpTargetObj->m_Change));
#else
							pViewportCreateChange.Skin		= lpTargetObj->m_Change;
#endif
	
#ifdef UPDATE_GM_FUNCTION_20070228
							if((lpTargetObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)
							{
	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
								gObjAddBuffEffect( lpTargetObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
	#else
								pViewportCreate.ViewSkillState	|= STATE_ADD_GM_MARK;
								lpTargetObj->m_ViewSkillState	|= STATE_ADD_GM_MARK;
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
							}
#endif
							
#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
							pViewportCreateChange.ViewSkillState	= lpTargetObj->m_ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
							pViewportCreateChange.DirAndPkLevel  = lpTargetObj->Dir<<4;

							if(lpTargetObj->m_stateEngagePVP == PVP_USER_PLAYING && lpTargetObj->MapNumber != PVP_EVENT_MAP)
							{
								g_PvpEvent.DeleteUser(lpTargetObj->m_Index);
							}

							if(lpTargetObj->m_stateEngagePVP == PVP_USER_PLAYING)
								pViewportCreateChange.DirAndPkLevel |= ((PK_LEVEL_DEFAULT+3)&0x0F);
							else
								pViewportCreateChange.DirAndPkLevel |= (lpTargetObj->m_PK_Level&0x0F);

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3������ �߰��� m_ViewState ����
							lpObj->CharSet[0] &= 0xF8;						// 3�� �������� ������ ��ȣ
							lpObj->CharSet[0] |= (lpObj->m_ViewState&0x07);	// m_ViewState�� ���� 3bit�� ����Ѵ�.
#else
#ifdef UPDATE_VIEWPORTCREATE_CHANGE_ADD_CHARSET_20070314
							lpObj->CharSet[0] &= 0xF0;
							lpObj->CharSet[0] |= (lpObj->m_ViewState&0x0F);
#endif
#endif	// THIRD_CHANGEUP_SYSTEM_20070507

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917			// !! NOT
	#ifdef CHAOSCASTLE_SYSTEM_20040408		// !!> . ����ڰ� ī���� ĳ���ȿ� �ִٸ� ����Ʈ�� �����ϱ� ���� ��ų����Ʈ���� ����� �����Ѵ�.
							if (CHECK_CHAOSCASTLE(lpTargetObj->MapNumber))
							{
//								pViewportCreateChange.ViewSkillState &= ~STATE_REDUCE_MAGICDEFENSE;		// �ҿ�ٸ���
//								pViewportCreateChange.ViewSkillState &= ~STATE_ADD_LIFE;				// ����������
//								pViewportCreateChange.ViewSkillState &= ~STATE_ATTACK;					// ���ݷ����
//								pViewportCreateChange.ViewSkillState &= ~STATE_DEFENSE;					// �������
								pViewportCreateChange.ViewSkillState = 0;
							}
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917


#ifdef MODIFY_CASTLESIEGE_VIEWPORT_BY_CANGE_RING_20051103
#ifdef MU_CASTLESIEGE_JOINSIDE_VIEWPORT_20041209
	#ifndef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204					// !! NOT
							if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) 
							{
								BYTE btCsJoinSide							= lpTargetObj->m_btCsJoinSide;
								if (btCsJoinSide < CS_SIEGESIDE_NONE)		btCsJoinSide	= CS_SIEGESIDE_NONE;
								if (btCsJoinSide > CS_SIEGESIDE_ATTACK)		btCsJoinSide	= CS_SIEGESIDE_ATTACK;

								switch(btCsJoinSide) 
								{
								case CS_SIEGESIDE_DEFEND :
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_DEFENSE_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
	#else
									pViewportCreate.ViewSkillState	|= STATE_JOIN_SIDE_DEFEND;
									lpTargetObj->m_ViewSkillState	|= STATE_JOIN_SIDE_DEFEND;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									break;
								case CS_SIEGESIDE_ATTACK :
		
	#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
									{
		#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
										if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK )
										{
											gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_ATTACK_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
										}
										else if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK_2 )
										{
											gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_ATTACK_MARK_2, 0, 0, 0, 0, BUFFTIME_INFINITE );
										}
										else if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK_3 )
										{
											gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_ATTACK_MARK_3, 0, 0, 0, 0, BUFFTIME_INFINITE );
										}
		#else
										DWORD btCSMarkViewSkillState = 0;
										if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK )
										{
											btCSMarkViewSkillState = STATE_JOIN_SIDE_ATTACK;
										}
										else if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK_2 )
										{
											btCSMarkViewSkillState = STATE_JOIN_SIDE_ATTACK_2;
										}
										else if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK_3 )
										{
											btCSMarkViewSkillState = STATE_JOIN_SIDE_ATTACK_3;
										}

										pViewportCreate.ViewSkillState	|= btCSMarkViewSkillState;
										lpTargetObj->m_ViewSkillState	|= btCSMarkViewSkillState;
		#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									}
	#else
									pViewportCreate.ViewSkillState	|= STATE_JOIN_SIDE_ATTACK;
									lpTargetObj->m_ViewSkillState	|= STATE_JOIN_SIDE_DEFEND;
	#endif
									break;
								case CS_SIEGESIDE_NONE :
								default :
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									gObjClearBuffEffect( lpTargetObj, CLEAR_TYPE_CASTLESIEGE );
	#else
									pViewportCreate.ViewSkillState	&= ~(STATE_JOIN_SIDE_DEFEND | STATE_JOIN_SIDE_ATTACK);
									lpTargetObj->m_ViewSkillState	&= ~(STATE_JOIN_SIDE_DEFEND | STATE_JOIN_SIDE_ATTACK);
		#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
									pViewportCreate.ViewSkillState	&= ~(STATE_JOIN_SIDE_ATTACK_2 | STATE_JOIN_SIDE_ATTACK_3);
									lpTargetObj->m_ViewSkillState	&= ~(STATE_JOIN_SIDE_ATTACK_2 | STATE_JOIN_SIDE_ATTACK_3);
		#endif
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									break;
								}
							}
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
#endif // MU_CASTLESIEGE_JOINSIDE_VIEWPORT_20041209
#endif // MODIFY_CASTLESIEGE_VIEWPORT_BY_CANGE_RING_20051103

#ifdef UPDATE_VIEWPORTCREATE_CHANGE_ADD_CHARSET_20070314	// gObjViewportListProtocolCreate() ����Ʈ�� ĳ���� ��� �߰�
							memcpy(&pViewportCreateChange.CharSet, lpTargetObj->CharSet, MAX_PREVIEWCHARSET+9);
#endif
							memcpy(pViewportCreateChange.Id, lpTargetObj->Name, MAX_IDSTRING);
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
							iViewportSize = gObjMakeViewportState( lpTargetObj, pViewportCreateChange.BuffEffectList );
							pViewportCreateChange.BuffEffectCount = iViewportSize;
							memcpy((SendGBufChange+lOfsChange), (char*)&pViewportCreateChange, sizeof(PMSG_VIEWPORTCREATE_CHANGE) );
							lOfsChange += ( sizeof(PMSG_VIEWPORTCREATE_CHANGE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) );
#else
							memcpy((SendGBufChange+lOfsChange), (char*)&pViewportCreateChange, sizeof(PMSG_VIEWPORTCREATE_CHANGE));
							lOfsChange += sizeof(PMSG_VIEWPORTCREATE_CHANGE);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
							ChangeCount++;
						}
						else
						{
							pViewportCreate.NumberH = HIBYTE(tObjNum);
							pViewportCreate.NumberL = LOBYTE(tObjNum);
							
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// CharSet[0] 3�� ���� ��Ʈ�������� ��ȣ
							lpTargetObj->CharSet[0] &= 0xF8;
#elif GAME_VERSION >= G_V_99B
							lpTargetObj->CharSet[0] &= 0xF0;
#endif	// THIRD_CHANGEUP_SYSTEM_20070507
							if( lpTargetObj->m_State == OBJST_CREATE )
							{
								if( !lpTargetObj->Teleport )
								{
									pViewportCreate.NumberH |= (0x01<<7);	// ���� ������ ���̸�..
								}
							}

#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
							pViewportCreate.ViewSkillState = lpTargetObj->m_ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
							lpTargetObj->CharSet[0] |= (lpTargetObj->m_ViewState&0x0F);
							pViewportCreate.X       = (BYTE)lpTargetObj->X;
							pViewportCreate.Y       = (BYTE)lpTargetObj->Y;
							pViewportCreate.TX		= (BYTE)lpTargetObj->TX;
							pViewportCreate.TY		= (BYTE)lpTargetObj->TY;
							pViewportCreate.DirAndPkLevel = lpTargetObj->Dir<<4;

							if(lpTargetObj->m_stateEngagePVP == PVP_USER_PLAYING && lpTargetObj->MapNumber != PVP_EVENT_MAP)
							{
								g_PvpEvent.DeleteUser(lpTargetObj->m_Index);
							}

							if(lpTargetObj->m_stateEngagePVP == PVP_USER_PLAYING)
								pViewportCreate.DirAndPkLevel |= ((PK_LEVEL_DEFAULT+3)&0x0F);
							else
								pViewportCreate.DirAndPkLevel |= (lpTargetObj->m_PK_Level&0x0F);

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004			// !! NOT
	#ifdef CHAOSCASTLE_SYSTEM_20040408		// !!> . ����ڰ� ī���� ĳ���ȿ� �ִٸ� ����Ʈ�� �����ϱ� ���� ��ų����Ʈ���� ����� �����Ѵ�.
							if (CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) {
//								pViewportCreateChange.ViewSkillState &= ~STATE_REDUCE_MAGICDEFENSE;		// �ҿ�ٸ���
//								pViewportCreateChange.ViewSkillState &= ~STATE_ADD_LIFE;				// ����������
//								pViewportCreateChange.ViewSkillState &= ~STATE_ATTACK;					// ���ݷ����
//								pViewportCreateChange.ViewSkillState &= ~STATE_DEFENSE;					// �������
								pViewportCreate.ViewSkillState = 0;
							}
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef MU_CASTLESIEGE_JOINSIDE_VIEWPORT_20041209	// ������ �� �ȿ����� ����Ʈ�� ��ų ����Ʈ�� �߰��ؼ� �����ش�.
	#ifndef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204					// !! NOT
							if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) 
							{
								BYTE btCsJoinSide							= lpTargetObj->m_btCsJoinSide;
								if (btCsJoinSide < CS_SIEGESIDE_NONE)		btCsJoinSide	= CS_SIEGESIDE_NONE;
								if (btCsJoinSide > CS_SIEGESIDE_ATTACK)		btCsJoinSide	= CS_SIEGESIDE_ATTACK;

								switch(btCsJoinSide) 
								{
								case CS_SIEGESIDE_DEFEND :
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_DEFENSE_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
	#else
									pViewportCreate.ViewSkillState	|= STATE_JOIN_SIDE_DEFEND;
									lpTargetObj->m_ViewSkillState	|= STATE_JOIN_SIDE_DEFEND;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									break;
								case CS_SIEGESIDE_ATTACK :
		
	#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
									{
		#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
										if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK )
										{
											gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_ATTACK_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
										}
										else if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK_2 )
										{
											gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_ATTACK_MARK_2, 0, 0, 0, 0, BUFFTIME_INFINITE );
										}
										else if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK_3 )
										{
											gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_ATTACK_MARK_3, 0, 0, 0, 0, BUFFTIME_INFINITE );
										}
		#else
										DWORD btCSMarkViewSkillState = 0;
										if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK )
										{
											btCSMarkViewSkillState = STATE_JOIN_SIDE_ATTACK;
										}
										else if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK_2 )
										{
											btCSMarkViewSkillState = STATE_JOIN_SIDE_ATTACK_2;
										}
										else if( lpTargetObj->m_btCsJoinSide == CS_SIEGESIDE_ATTACK_3 )
										{
											btCSMarkViewSkillState = STATE_JOIN_SIDE_ATTACK_3;
										}
										
										pViewportCreate.ViewSkillState	|= btCSMarkViewSkillState;
										lpTargetObj->m_ViewSkillState	|= btCSMarkViewSkillState;
		#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									}
	#else
									pViewportCreate.ViewSkillState	|= STATE_JOIN_SIDE_ATTACK;
									lpTargetObj->m_ViewSkillState	|= STATE_JOIN_SIDE_DEFEND;
	#endif
									break;
								case CS_SIEGESIDE_NONE :
								default :
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									gObjClearBuffEffect( lpTargetObj, CLEAR_TYPE_CASTLESIEGE );
	#else
									pViewportCreate.ViewSkillState	&= ~(STATE_JOIN_SIDE_DEFEND | STATE_JOIN_SIDE_ATTACK);
									lpTargetObj->m_ViewSkillState	&= ~(STATE_JOIN_SIDE_DEFEND | STATE_JOIN_SIDE_ATTACK);
		#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
									pViewportCreate.ViewSkillState	&= ~(STATE_JOIN_SIDE_ATTACK_2 | STATE_JOIN_SIDE_ATTACK_3);
									lpTargetObj->m_ViewSkillState	&= ~(STATE_JOIN_SIDE_ATTACK_2 | STATE_JOIN_SIDE_ATTACK_3);
		#endif
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									break;
								}
							}
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
							memcpy(&pViewportCreate.CharSet, lpTargetObj->CharSet, MAX_PREVIEWCHARSET+9);
#else
#ifdef DARKLORD_WORK
							memcpy(&pViewportCreate.CharSet, lpTargetObj->CharSet, MAX_PREVIEWCHARSET+4);
#else
							memcpy(&pViewportCreate.CharSet, lpTargetObj->CharSet, MAX_PREVIEWCHARSET+3);
#endif
#endif
							
							memcpy(pViewportCreate.Id, lpTargetObj->Name, MAX_IDSTRING);
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
							iViewportSize = gObjMakeViewportState( lpTargetObj, pViewportCreate.BuffEffectList );
							pViewportCreate.BuffEffectCount = iViewportSize;
							memcpy((sendBuf+lOfs), (char*)&pViewportCreate, sizeof(PMSG_VIEWPORTCREATE) );
							lOfs += ( sizeof(PMSG_VIEWPORTCREATE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) );
#else
							memcpy((sendBuf+lOfs), (char*)&pViewportCreate, sizeof(PMSG_VIEWPORTCREATE));
							lOfs += sizeof(PMSG_VIEWPORTCREATE);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917							
							count++;
						}
						if( lpObj->Type == OBJTYPE_CHARACTER )
						{

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
							if( lpTargetObj->lpGuild )								
							{
								// �Լ��� ���ӽ����̽��� ���´�.
				
								PMSG_SIMPLE_GUILDVIEWPORT	pMsg;

		#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
								pMsg.GuildNumber= lpTargetObj->lpGuild->Number;
		#else
								pMsg.GNumberH = HIBYTE(lpTargetObj->lpGuild->Number);
								pMsg.GNumberL = LOBYTE(lpTargetObj->lpGuild->Number);
		#endif
								pMsg.NumberH  = HIBYTE(lpTargetObj->m_Index)&0x7f;
								pMsg.NumberL  = LOBYTE(lpTargetObj->m_Index);

		#ifdef ADD_NEW_GUILD_01_20040913
								// ��� ��å�� ������.
								pMsg.btGuildStatus = lpTargetObj->GuildStatus;
		#endif

		#ifdef ADD_NEW_GUILD_02_20040922
								pMsg.btGuildType = lpTargetObj->lpGuild->btGuildType;
		#endif

		#ifdef ADD_NEW_UNION_01_20041006
								// ���� �˻�
								if( lpObj->lpGuild != NULL )
								{
									pMsg.btGuildRelationShip = gObjGetRelationShip( lpObj, lpTargetObj );
								}
								else
								{
									pMsg.btGuildRelationShip = G_RELATIONSHIP_NONE;
								}
		#endif

								if(!strcmp(lpTargetObj->lpGuild->Names[0], lpTargetObj->Name))
								{	// ��� ������ �̸�
									pMsg.NumberH |= 0x80;

								}
								
		#ifdef UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217
								if (g_CastleSiegeSync.CheckCastleOwnerMember(lpTargetObj->m_Index) == TRUE ||
									g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpTargetObj->m_Index) == TRUE)
								{
									pMsg.btCastleOwner = 0x01;
								}
								else
								{
									pMsg.btCastleOwner = 0x00;
								}						
		#endif	// UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217

								memcpy(GuildInfoBuf+GuildInfoOfs, &pMsg, sizeof(pMsg));
								GuildInfoOfs += sizeof(pMsg);
								GuildInfoCount++;
							}
#endif
							
#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
							// �Ʒ� ��� ���� ���� �κ��� �� �����Ѵ�.
							if( FALSE )
#else
							if( lpTargetObj->GuildNumber )
#endif
							{
								PMSG_GUILDVIEWPORT_USER pGVUMsg;
								if( ViewGuildMng.Add(lpTargetObj->GuildNumber, lpTargetObj->m_Index) == TRUE)
								{
#ifdef CHINA_GUILD_POINTER_REF_ERROR
									if( lpTargetObj->lpGuild )
#endif
									{
										PMSG_GUILDVIEWPORT	pMsg;
										pMsg.NumberH = HIBYTE(lpTargetObj->lpGuild->Number);
										pMsg.NumberL = LOBYTE(lpTargetObj->lpGuild->Number);		
										strcpy(pMsg.GuildName, lpTargetObj->lpGuild->Name);
										memcpy(pMsg.Mark, lpTargetObj->lpGuild->Mark, 32);
										memcpy(GuildInfoBuf+GuildInfoOfs, &pMsg, sizeof(pMsg));
										GuildInfoOfs += sizeof(pMsg);
										GuildInfoCount++;
										//LogAdd("�� : %s (%s)", lpTargetObj->lpGuild->Name, gObj[aIndex].Name);
									}
								}

						#ifdef GUILD_VIEWPORT_MODIFY
								pGVUMsg.NumberH = HIBYTE(lpTargetObj->m_Index)&0x7f;
								pGVUMsg.NumberL = LOBYTE(lpTargetObj->m_Index);
#ifdef CHINA_GUILD_POINTER_REF_ERROR
								if( lpTargetObj->lpGuild )								
#endif
								{
									if(!strcmp(lpTargetObj->lpGuild->Names[0], lpTargetObj->Name))
									{	// ��� ������ �̸�
										pGVUMsg.NumberH |= 0x80;
									}
								}
						#else
								pGVUMsg.NumberH = HIBYTE(lpTargetObj->m_Index);
								pGVUMsg.NumberL = LOBYTE(lpTargetObj->m_Index);
						#endif						

#ifdef CHINA_GUILD_POINTER_REF_ERROR
								if(lpTargetObj->lpGuild)								
#endif
								{
									pGVUMsg.GNumberH = HIBYTE(lpTargetObj->lpGuild->Number);
									pGVUMsg.GNumberL = LOBYTE(lpTargetObj->lpGuild->Number);						
									memcpy(GuildUserBuf+GuildUserOfs, &pGVUMsg, sizeof(pGVUMsg));
									GuildUserOfs += sizeof(pGVUMsg);
									GuildUserCount++;
								}
							}
						}
						break;
					case OBJTYPE_MONSTER :
					case OBJTYPE_NPC :
						if( lpObj->Type == OBJTYPE_CHARACTER )
						{							
							lpTargetObj = &gObj[tObjNum];

							if( lpTargetObj->m_RecallMon >= 0 )
							{
								pCallMonsterViewportCreate.NumberH = HIBYTE(tObjNum);
								pCallMonsterViewportCreate.NumberL = LOBYTE(tObjNum);
								if( lpTargetObj->m_State == OBJST_CREATE )
									pCallMonsterViewportCreate.NumberH |= (0x01<<7);	// ���� ������ ���̸�..
								
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
								pCallMonsterViewportCreate.Type_HI = HIBYTE(lpTargetObj->Class);
								pCallMonsterViewportCreate.Type_LO = LOBYTE(lpTargetObj->Class);
#else
								pCallMonsterViewportCreate.Type	   = lpTargetObj->Class;
#endif
#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
								pCallMonsterViewportCreate.ViewState = lpTargetObj->m_ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
								pCallMonsterViewportCreate.X       = (BYTE)lpTargetObj->X;
								pCallMonsterViewportCreate.Y       = (BYTE)lpTargetObj->Y;
								pCallMonsterViewportCreate.TX      = (BYTE)lpTargetObj->TX;
								pCallMonsterViewportCreate.TY      = (BYTE)lpTargetObj->TY;
								pCallMonsterViewportCreate.Path    = lpTargetObj->Dir<<4;								
								
								if( lpTargetObj->m_RecallMon >= 0 && lpTargetObj->m_RecallMon < MAX_OBJECT-1 )
								{
									memcpy(pCallMonsterViewportCreate.Id, gObj[lpTargetObj->m_RecallMon].Name, MAX_IDSTRING);
								}
								else memset(pCallMonsterViewportCreate.Id, 0, MAX_IDSTRING);								
								
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
								iViewportSize = gObjMakeViewportState( lpTargetObj, pCallMonsterViewportCreate.BuffEffectList );
								pCallMonsterViewportCreate.BuffEffectCount = iViewportSize;
								memcpy((callMonstersendBuf+callMonlOfs), (char*)&pCallMonsterViewportCreate, sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE) );
								callMonlOfs += ( sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) );
#else
								memcpy((callMonstersendBuf+callMonlOfs), (char*)&pCallMonsterViewportCreate, sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE));
								callMonlOfs += sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
								callmonstercount++;
							}
							else
							{
								pMonsterViewportCreate.NumberH = HIBYTE(tObjNum);
								pMonsterViewportCreate.NumberL = LOBYTE(tObjNum);
								if( lpTargetObj->m_State == OBJST_CREATE )
								{
									pMonsterViewportCreate.NumberH |= (0x01<<7);	// ���� ������ ���̸�..

#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724
									if ( lpTargetObj->Teleport != 0 )
									{
										// �ڷ���Ʈ �� ���¿��ٸ�. NumberH�� ���� 2bit���� 1�����Ѵ�. ���� 1bit�� ���λ������η� ���. 
										// ���� 2bit������ ��밡����. 2bit | 4bit : 0 ~ 16383 ��ȣ ����.
										pMonsterViewportCreate.NumberH |= (0x01<<6);
									}
#endif // MODIFY_NEW_MONSTER_SKILL_01_20060724

								}
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
								pMonsterViewportCreate.Type_HI    = HIBYTE(lpTargetObj->Class);
								pMonsterViewportCreate.Type_LO    = LOBYTE(lpTargetObj->Class);
#else
								pMonsterViewportCreate.Type		  = lpTargetObj->Class;
#endif
								
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
								// ���� ����� ������ �������� üũ.
								if( lpTargetObj->Class == NPC_INDEX_LIFESTONE )
								{

									// 1. ���� ���ձ��� (���� ����Ʈ�� ���� 1��° ��Ʈ ���)
									if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) 
									{							
										if( lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide )
										{
											// ���� Class �� �ֻ��� Bit�� 1�� ��쿡�� ���� ��
											pMonsterViewportCreate.Type_HI |= 0x80;
										}	
									}
									
									// 1-1. ���� ��������
									if( lpObj->lpGuild != NULL && lpTargetObj->lpGuild != NULL )
									{
										if( lpObj->lpGuild == lpTargetObj->lpGuild )
										{
											// ���� Class �� �ֻ��� Bit�� 1�� ��쿡�� ���� ��
											pMonsterViewportCreate.Type_HI |= 0x80;
										}
									}

									// 2. ������ ���� ���� �ܰ� (���� ����Ʈ�� ���� 2��° 3��° 4��° ��Ʈ ���)
									pMonsterViewportCreate.Type_HI |= ((lpTargetObj->m_btCreationState<<4)&0x70);

								}
#endif	

								//pMonsterViewportCreate.ViewState = lpTargetObj->m_ViewState;
#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_20070917					// !! NOT
								pMonsterViewportCreate.ViewState = lpTargetObj->m_ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
								pMonsterViewportCreate.X       = (BYTE)lpTargetObj->X;
								pMonsterViewportCreate.Y       = (BYTE)lpTargetObj->Y;
								pMonsterViewportCreate.TX      = (BYTE)lpTargetObj->TX;
								pMonsterViewportCreate.TY      = (BYTE)lpTargetObj->TY;
								pMonsterViewportCreate.Path    = lpTargetObj->Dir<<4;

#ifdef CASTLE_NPC_GATE_WORK_20041206	// ������ ��� ������ ����Ʈ ó��
								if (lpTargetObj->Class == 277) {
									// ������ ��� ��ų���� �� ��� �������� ǥ�õȴ�
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
									if (lpTargetObj->m_btCsGateOpen == CS_GATE_STATE_CLOSED)
									{
										gObjRemoveBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_DOOR_STATE );
									}
									else
									{
										gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_DOOR_STATE );
									}
	#else
									if (lpTargetObj->m_btCsGateOpen == CS_GATE_STATE_CLOSED)
										pMonsterViewportCreate.ViewState &= ~STATE_CASTLE_GATE;
									else
										pMonsterViewportCreate.ViewState |= STATE_CASTLE_GATE;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
								}
#endif
#ifdef CASTLE_NPC_CROWN_WORK_20041210
								if (lpTargetObj->Class == 216)
								{
									// �հ��� ��� ��ų���� �� ��� ��� �� ���°� ǥ�õȴ�
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
									if (g_CastleSiege.GetRegCrownAvailable() == FALSE )
		#else
									if (g_CastleSiege.GetRegCrownAvailable() == TRUE )
		#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
									{
										gObjRemoveBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_CROWN_STATE );
									}
									else
									{
										gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CASTLE_CROWN_STATE );
									}
	#else
									if (g_CastleSiege.GetRegCrownAvailable() == FALSE)
										pMonsterViewportCreate.ViewState &= ~STATE_CASTLE_CROWN;
									else
										pMonsterViewportCreate.ViewState |= STATE_CASTLE_CROWN;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
								}
#endif

#ifdef CRYWOLF_NPC_WORK_20050912
								// ������ �Ż� �� ���� / ���� ���� ǥ��
								// ADD_VIEWSKILLSTATE
								// DEL_VIEWSKILLSTATE( x, y )	GCStateInfoSend( lpObj, 1, STATE_BLESS_POTION );

								// ���� ��� ���� / �Ұ��� / ���� ���� ǥ�� 
#endif
								
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
								iViewportSize = gObjMakeViewportState( lpTargetObj, pMonsterViewportCreate.BuffEffectList );
								pMonsterViewportCreate.BuffEffectCount = iViewportSize;
								memcpy((MonstersendBuf+MonlOfs), (char*)&pMonsterViewportCreate, sizeof(PMSG_MONSTER_VIEWPORTCREATE) );
								MonlOfs += ( sizeof(PMSG_MONSTER_VIEWPORTCREATE) - ( MAX_BUFF_SLOT_INDEX - iViewportSize ) );
#else
								memcpy((MonstersendBuf+MonlOfs), (char*)&pMonsterViewportCreate, sizeof(PMSG_MONSTER_VIEWPORTCREATE));
								MonlOfs += sizeof(PMSG_MONSTER_VIEWPORTCREATE);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
								monstercount++;
							}
						}
						break;
					case OBJTYPE_ITEM :
						if( lpObj->Type == OBJTYPE_CHARACTER )
						{
							pItemViewportCreate.NumberH		= HIBYTE(tObjNum);
							pItemViewportCreate.NumberL		= LOBYTE(tObjNum);
							if( MapC[lpObj->MapNumber].m_cItem[tObjNum].m_State == OBJST_CREATE )
							{
								pItemViewportCreate.NumberH |= (0x01<<7);
							}
							pItemViewportCreate.px			= MapC[lpObj->MapNumber].m_cItem[tObjNum].px;
							pItemViewportCreate.py			= MapC[lpObj->MapNumber].m_cItem[tObjNum].py;
							// ���� ���϶�
							if( MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type == GOLD_TYPE_INDEX )
							{
								WORD moneyh, moneyl;
								moneyh = HIWORD(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
								moneyl = LOWORD(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
								
								pItemViewportCreate.ItemInfo[0] = (BYTE)MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type%255;
								pItemViewportCreate.ItemInfo[1] = LOBYTE(moneyh);
								pItemViewportCreate.ItemInfo[2] = HIBYTE(moneyl);
								pItemViewportCreate.ItemInfo[4] = LOBYTE(moneyl);
#ifdef ITEM_INDEX_EXTEND_20050706	// ���� ��� Ư�� ������ �ؾ� �Ѵ�.
								pItemViewportCreate.ItemInfo[3] = 0x00;
								pItemViewportCreate.ItemInfo[5] = (BYTE)((MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type&0x1E00)>>5);
								pItemViewportCreate.ItemInfo[6] = 0;
#else
								pItemViewportCreate.ItemInfo[3] = 0x80;
#endif
								memcpy(ItemBuf+lOfs_Item, (char*)&pItemViewportCreate, sizeof(pItemViewportCreate));
								lOfs_Item += ItemStructSize;
							}
							else 
							{
								ItemByteConvert(pItemViewportCreate.ItemInfo, MapC[lpObj->MapNumber].m_cItem[tObjNum]);
								memcpy(ItemBuf+lOfs_Item, (char*)&pItemViewportCreate, sizeof(pItemViewportCreate));

								lOfs_Item += ItemStructSize;
																
							}
							count_Item++;



						}				
						break;
					}
				}			
				lpObj->VpPlayer[n].state = STVP_PLAYING;
			}
		}
	}
	// �ϴ��� �����϶��� �������� ������ �ʴ´�.
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( count > 0 ) 
		{
			PWMSG_COUNT			pwCount;
			pwCount.h.c         = PMHC_WORD;
			pwCount.h.headcode  = PMHC_VIEWPORTCREATE;
			pwCount.h.sizeH     = HIBYTE(lOfs);
			pwCount.h.sizeL     = LOBYTE(lOfs);
			pwCount.count       = count;
			memcpy(sendBuf, (char*)&pwCount, sizeof(PWMSG_COUNT));
			DataSend(aIndex, sendBuf, lOfs);
		}	
		if( ChangeCount > 0 )
		{
			PWMSG_COUNT			pwCount;
			pwCount.h.c         = PMHC_WORD;
			pwCount.h.headcode  = 0x45;
			pwCount.h.sizeH     = HIBYTE(lOfsChange);
			pwCount.h.sizeL     = LOBYTE(lOfsChange);
			pwCount.count       = ChangeCount;
			memcpy(SendGBufChange, (char*)&pwCount, sizeof(PWMSG_COUNT));
			DataSend(aIndex, SendGBufChange, lOfsChange);
		}			
		// ���Ͱ� �����Ǿ��ٸ�
		if( monstercount > 0 )
		{
			PWMSG_COUNT			pwCount;
			pwCount.h.c         = PMHC_WORD;
			pwCount.h.headcode  = PMHC_VIEWPORTMONSTERCREATE;
			pwCount.h.sizeH     = HIBYTE(MonlOfs);
			pwCount.h.sizeL     = LOBYTE(MonlOfs);
			pwCount.count       = monstercount;
			memcpy(MonstersendBuf, (char*)&pwCount, sizeof(PWMSG_COUNT));
			DataSend(aIndex, MonstersendBuf, MonlOfs);
		}
		if( callmonstercount > 0 )
		{
			PWMSG_COUNT			pwCount;
			pwCount.h.c         = PMHC_WORD;
			pwCount.h.headcode  = 0x1F;
			pwCount.h.sizeH     = HIBYTE(callMonlOfs);
			pwCount.h.sizeL     = LOBYTE(callMonlOfs);
			pwCount.count       = callmonstercount;
			memcpy(callMonstersendBuf, (char*)&pwCount, sizeof(PWMSG_COUNT));
			DataSend(aIndex, callMonstersendBuf, callMonlOfs);
		}
		
		// �������� �����Ǿ��ٸ� 
		if( count_Item > 0 )
		{
			PWMSG_COUNT			pwCount;
			pwCount.h.c         = PMHC_WORD;
			pwCount.h.headcode  = 0x20;
			pwCount.count       = count_Item;
			pwCount.h.sizeH     = HIBYTE(lOfs_Item);
			pwCount.h.sizeL     = LOBYTE(lOfs_Item);
			memcpy(ItemBuf, (char*)&pwCount, sizeof(pwCount));						
			DataSend(aIndex, ItemBuf, lOfs_Item);
		}
		// ��� ������ �̸� ������..
		if ( GuildInfoCount )
		{
#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
			PMSG_SIMPLE_GUILDVIEWPORT_COUNT pGVCount;
			pGVCount.h.headcode = 0x65;
#else
			PMSG_GUILDVIEWPORT_COUNT pGVCount;
			pGVCount.h.headcode = 0x5A;
#endif
			
			pGVCount.h.c		= PMHC_WORD;		
			pGVCount.h.sizeH	= HIBYTE( GuildInfoOfs );
			pGVCount.h.sizeL	= LOBYTE( GuildInfoOfs );
			pGVCount.Count		= GuildInfoCount;
			memcpy(GuildInfoBuf, &pGVCount, sizeof(pGVCount));
			DataSend(aIndex, GuildInfoBuf, GuildInfoOfs);
		}

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
		// 0x65�� ���� ��.
#else
		// ��� ������ ������.
		if( GuildUserCount )
		{
			PMSG_GUILDVIEWPORT_USERCOUNT pGUCount;
			pGUCount.h.c		= PMHC_WORD;
			pGUCount.h.headcode = 0x5B;
			pGUCount.h.sizeH	= HIBYTE( GuildUserOfs );
			pGUCount.h.sizeL	= LOBYTE( GuildUserOfs );
			pGUCount.Count		= GuildUserCount;
			memcpy(GuildUserBuf, &pGUCount, sizeof(pGUCount));
			DataSend(aIndex, GuildUserBuf, GuildUserOfs);			
		}
#endif
		
	}	
}


// ��ų ���� ó��
void gObjSkillUseProc(LPOBJECTSTRUCT lpObj)
{
	// ����� ���� ó��
	if( lpObj->m_SkillAttackTime2 > 0 )
	{
		lpObj->m_SkillAttackTime2--;
		if( lpObj->m_SkillAttackTime2 <= 0 )
		{
			lpObj->m_SkillAttack2 = 0;
			lpObj->m_SkillAttackTime2 = 0;
		}
	}

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	if( lpObj->m_SkillAttackTime <= GetTickCount() )
	{
// 		lpObj->m_SkillAttackTime--;
// 		if( lpObj->m_SkillAttackTime == 0 )
		{
			lpObj->m_SkillAttack = 0;
			lpObj->m_SkillAttackTime = 0;

#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ATTACK );
#else
			lpObj->m_ViewSkillState &= 0xF7;
#endif

			GCMagicCancelSend(lpObj, AT_SKILL_ATTACK);
		}
	}

	if( lpObj->m_SkillDefenseTime <= GetTickCount() )
	{
// 		lpObj->m_SkillDefenseTime--;
// 		if( lpObj->m_SkillDefenseTime == 0 )
		{			
			lpObj->m_SkillDefense = 0;
			lpObj->m_SkillDefenseTime = 0;

#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_DEFENSE );
#else
			lpObj->m_ViewSkillState &= 0xFB;
#endif

			GCMagicCancelSend(lpObj, AT_SKILL_DEFENSE);
		}
	}	

#ifdef NEW_FORSKYLAND2	
	if( lpObj->m_WizardSkillDefenseTime > 0 )
	{
		lpObj->m_WizardSkillDefenseTime--;
		if( lpObj->m_WizardSkillDefenseTime == 0 )
		{
			lpObj->m_WizardSkillDefense = 0;

#ifdef CHANGE_VIEWSTATE
			lpObj->m_ViewSkillState &= ~STATE_REDUCE_MAGICDEFENSE; //	9��° ��Ʈ : ���� ���� ����
#else
			lpObj->m_ViewSkillState |= 0;						  
#endif;
			GCMagicCancelSend(lpObj, AT_SKILL_MAGICDEFENSE);
		}
	}
#endif

#ifdef NEW_SKILL_FORSKYLAND
	if( lpObj->m_SkillAddLifeTime > 0 )
	{
		lpObj->m_SkillAddLifeTime--;
		if( lpObj->m_SkillAddLifeTime == 0 )
		{	
			lpObj->AddLife -= lpObj->m_SkillAddLife;	// �ִ� ����� ������ ������ �ش�
			if( lpObj->AddLife < 0 )
				lpObj->AddLife = 0;

#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
#else
			GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0);
#endif

			if( lpObj->Life > lpObj->MaxLife+lpObj->AddLife  )
			{
				lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)lpObj->iShield);
#else
				GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0);
#endif
			}

			lpObj->m_SkillAddLife = 0;
			lpObj->m_ViewSkillState &= ~STATE_ADD_LIFE;
			GCMagicCancelSend(lpObj, AT_SKILL_KNIGHTADDLIFE);			
		}
	}	

	if( lpObj->m_SkillHardenTime > 0 )
	{
		lpObj->m_SkillHardenTime--;
		if( lpObj->m_SkillHardenTime <= 0 )
		{			
			lpObj->m_SkillHarden = 0;
			lpObj->m_ViewSkillState &= ~STATE_HARDEN;
			GCMagicCancelSend(lpObj, AT_SKILL_ELFHARDEN);			
		}
	}

	if( lpObj->m_SkillMagumReduceDefenseTime > 0 )
	{
		lpObj->m_SkillMagumReduceDefenseTime--;
		if( lpObj->m_SkillMagumReduceDefenseTime == 0 )
		{			
			lpObj->m_SkillMagumReduceDefense = 0;
			lpObj->m_ViewSkillState &= ~STATE_REDUCE_DEFENSE;
			GCMagicCancelSend(lpObj, AT_SKILL_DEFENSEDOWN);
		}
	}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef NEW_SKILL_FORSKYLAND
	if( lpObj->Class == 77 )
	{	// õ�� ���� ó��
		lpObj->m_SkyBossMonSheildTime++;
		if( lpObj->m_SkyBossMonSheildTime > 5 )
		{
			lpObj->m_SkyBossMonSheildTime = 0;
			if( lpObj->m_SkyBossMonSheild )
			{
				lpObj->m_SkyBossMonSheild = FALSE;				
				lpObj->m_ViewSkillState &= ~STATE_REDUCE_ATTACKDAMAGE;
				GCMagicCancelSend(lpObj, AT_SKILL_MAGICDEFENSE);
				//STATE_REDUCE_MAGICDEFENSE
			}
			else
			{
				lpObj->m_SkyBossMonSheild = TRUE;				
				lpObj->m_ViewSkillState |= STATE_REDUCE_ATTACKDAMAGE; 
				GCMagicAttackNumberSend(lpObj, AT_SKILL_MAGICDEFENSE, lpObj->m_Index, 1);
			}
		}
	}
#endif
#endif


#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
	if(lpObj->m_iSkillNPCHelpTime > 0)
	{
		// 30�� ���� �ʰ� ��ų ����
	#ifdef UPDATE_SHADOW_PHANTOM_20070302		// ���� �ð� ����
		// ������ ���� �������� ���� -- ����ð�(��) = 30 + (ĳ���ͷ���/6)
		if( GetTickCount() > lpObj->m_iSkillNPCHelpTime + ( 1000 * 60 * (30 + (int)(lpObj->Level / 6) ) ) )
	#else
		// 30�� ���� �ʰ� ��ų ����
		if(GetTickCount() > lpObj->m_iSkillNPCHelpTime + (1000 * 60 * 30))
	#endif
		{
			lpObj->m_iSkillNPCHelpTime = 0;
			lpObj->m_iSkillNPCDefense = 0; 
			lpObj->m_iSkillNPCAttack = 0;

			DEL_VIEWSKILLSTATE(lpObj->m_ViewSkillState, STATE_NPC_HELP);
			GCStateInfoSend(lpObj, 0, STATE_NPC_HELP);			
		}
	}
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef DARKLORD_WORK	
	if( lpObj->SkillRecallParty_Time && lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( --lpObj->SkillRecallParty_Time <= 0 )
		{
			lpObj->SkillRecallParty_Time = 0;

#ifdef MODIFY_DARKLORD_RECALL_BUGFIX_20051103
			// 0. �ֿ� ���� ���� �Ǵ� - ���� ĳ��, ī���� ĳ��, ����������, Į���� ��
			BOOL bCheckMainToMove = gMoveCommand.CheckMainToMove( lpObj );
			
			// 2. �̵� ������ ���� ���� �Ǵ�
			BOOL bCheckEquipmentToMove = gMoveCommand.CheckEquipmentToMove( lpObj, lpObj->SkillRecallParty_MapNumber );

			// 3. �̵� �������̽� ���� �Ǵ�
			BOOL bCheckInterfaceToMove = gMoveCommand.CheckInterfaceToMove( lpObj );
	
			if( !bCheckMainToMove 
				|| !bCheckEquipmentToMove 
				|| !bCheckInterfaceToMove	
			  )
			{
				char msg[255];
				wsprintf(msg, lMsg.Get(1254));	// "��ȯ���"

				GCServerMsgStringSend(msg, lpObj->m_Index, 1);
			}
			else
#endif
			gObjRecall(lpObj->m_Index, lpObj->SkillRecallParty_MapNumber, 
					   lpObj->SkillRecallParty_X, lpObj->SkillRecallParty_Y);

#ifdef MODIFY_RECALLSKIL_BUGFIX_20081107
			// ��ȯ�� ���� �� �ش� ���� �ʱ�ȭ ���ش�. by natinda
			lpObj->SkillRecallParty_MapNumber	= 0;
			lpObj->SkillRecallParty_Time		= 0;
			lpObj->SkillRecallParty_X			= 0;
			lpObj->SkillRecallParty_Y			= 0;
#endif


		}
		else
		{
			if( lpObj->SkillRecallParty_Time <= 5 )
			{
				char msg[255];
				wsprintf(msg, lMsg.Get(1243), lpObj->SkillRecallParty_Time);	// "%d�� �Ŀ� ��ȯ �˴ϴ�"
				GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
			}
		}
	}

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	if( lpObj->SkillAddCriticalDamageTime && lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( --lpObj->SkillAddCriticalDamageTime <= 0 )
		{
			lpObj->SkillAddCriticalDamageTime = 0;			
			lpObj->SkillAddCriticalDamage = 0;

			MsgOutput(lpObj->m_Index, lMsg.Get(1242));	// "ũ��Ƽ�� ������ ���� ������ ȿ���� ��������ϴ�"

			lpObj->m_ViewSkillState &= ~STATE_ADD_CRITICAL_DAMAGE; // ��ų ũ��Ƽ�� ������ ����
			GCMagicCancelSend(lpObj, AT_SKILL_ADD_CRITICALDAMAGE);
		}
	}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
	TMonsterSkillElementInfo::CheckSkillElementInfoProc(lpObj);
#endif

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	if( lpObj->m_iSkillStunTime && lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( --lpObj->m_iSkillStunTime <= 0 )
		{
			lpObj->m_iSkillStunTime = 0;			
	
			MsgOutput(lpObj->m_Index, lMsg.Get(1636));	// "���� ȿ���� ��������ϴ�."

			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_STUNED );
			GCStateInfoSend( lpObj, 0, STATE_STUNED );
		}
	}

	if( lpObj->m_iSkillBrandOfSkillTime && lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( --lpObj->m_iSkillBrandOfSkillTime <= 0 )
		{
			lpObj->m_iSkillBrandOfSkillTime = 0;			
	
			MsgOutput(lpObj->m_Index, lMsg.Get(1637));	// "�귣����꽺ų ȿ���� ��������ϴ�."

			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_BRAND_OF_SKILL );
			GCStateInfoSend( lpObj, 0, STATE_BRAND_OF_SKILL );
		}
	}

	if( lpObj->m_iSkillInvisibleTime && lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( --lpObj->m_iSkillInvisibleTime <= 0 )
		{
			lpObj->m_iSkillInvisibleTime = 0;			
	
			MsgOutput(lpObj->m_Index, lMsg.Get(1638) );	// "�κ����� ȿ���� ��������ϴ�."

			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_CLOAKING );
			GCStateInfoSend( lpObj, 0, STATE_CLOAKING );
		}
	}

	if( lpObj->m_iSkillManaSwellTime && lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( --lpObj->m_iSkillManaSwellTime <= 0 )
		{
			lpObj->AddMana -= lpObj->m_iSkillManaSwell;

			if( lpObj->AddMana < 0 )
				lpObj->AddMana = 0;

			GCManaSend(lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
			
			if( lpObj->Mana > (lpObj->MaxMana+lpObj->AddMana) )
			{	// ���� ������ �ִ� �������� ũ�� ������ ������
				lpObj->Mana = (lpObj->MaxMana+lpObj->AddMana);
				GCManaSend(lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, lpObj->BP);		
			}

			lpObj->m_iSkillManaSwellTime = 0;
			lpObj->m_iSkillManaSwell = 0;
	
			MsgOutput(lpObj->m_Index, lMsg.Get(1639) );	// "�������� ȿ���� ��������ϴ�."

			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ADD_MANA );
			GCStateInfoSend( lpObj, 0, STATE_ADD_MANA );
		}
	}
	#endif

	#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	if( lpObj->m_iPotionBlessTime && lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( --lpObj->m_iPotionBlessTime <= 0 )
		{
			lpObj->m_iPotionBlessTime = 0;			
	
			MsgOutput(lpObj->m_Index, lMsg.Get(1634));	// "�ູ�� ���� ȿ���� ��������ϴ�."

			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_BLESS_POTION );
			GCStateInfoSend( lpObj, 0, STATE_BLESS_POTION );
		}
	}

	if( lpObj->m_iPotionSoulTime && lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( --lpObj->m_iPotionSoulTime <= 0 )
		{
			lpObj->m_iPotionSoulTime = 0;			
	
			MsgOutput(lpObj->m_Index, lMsg.Get(1635) );	// "��ȥ�� ���� ȿ���� ��������ϴ�."

			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_SOUL_POTION );
			GCStateInfoSend( lpObj, 0, STATE_SOUL_POTION );
		}
	}
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef ITEM_ADD_OPTION_20061019
	if( lpObj->Type == OBJTYPE_CHARACTER && lpObj->m_iItemEffectValidTime > 0 )
	{
		lpObj->m_iItemEffectValidTime--;

		if( lpObj->m_iItemEffectValidTime <= 0 )
		{
			lpObj->m_iItemEffectValidTime = 0;
			// ���� ������� ������ �ɷ��� �ʱ�ȭ ��Ų��.
			g_ItemAddOption.ClearItemEffect( lpObj, ITEMUSE_EFFECT_OPTION_END );
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
#else
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0);
#endif
			GCManaSend(lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP);
		}
	}
#endif // ITEM_ADD_OPTION_20061019

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��� ��ų �ð� ó��
	if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) && lpObj->m_iIllusionTempleIndex != -1 )
	{
		g_IllusionTempleEvent.EventSkillProc(lpObj);
	}
#endif

	if( lpObj->m_SkillNumber < 1 ) return;

	if( lpObj->m_SkillNumber ==  AT_SKILL_BLOCKING )
	{
		if( GetTickCount() > lpObj->m_SkillTime)
		{
			lpObj->m_SkillNumber = 0;
		}
	}	

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	if( lpObj->m_iMuseElfInfinityArrowSkillTime && lpObj->Type == OBJTYPE_CHARACTER )
	{
		lpObj->m_iMuseElfInfinityArrowSkillTime -= 1;
		if( lpObj->m_iMuseElfInfinityArrowSkillTime <= 0 )
		{
			lpObj->m_iMuseElfInfinityArrowSkillTime = 0;			

			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_INFINITY_ARROW );
			GCStateInfoSend( lpObj, 0, STATE_INFINITY_ARROW );
		}
	}
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
}

// 1�ʿ� �ѹ��� ȣ��ȴ�.
// ��ų�̳� �������� �������� ó��
void gObjSkillBeAttackProc(LPOBJECTSTRUCT lpObj)
{
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	int ad;
#ifdef ADD_SHIELD_POINT_01_20060403
	int iShieldDamage = 0;
#endif
	if( lpObj->m_PoisonBeattackCount > 0)	// �ߵ� ������ �������� 
	{
		//lpObj->m_ViewState = 4;
		lpObj->m_PoisonBeattackCount--;
		if( (lpObj->m_PoisonBeattackCount%3) == 0 )
		{

#ifdef ADD_SKILL_WITH_COMBO
			if( lpObj->Life > 0 )
			{
				if( lpObj->m_PoisonType == AT_SKILL_EXPPOISON )
				{	// ���� �������� 5%
					ad = (int)((lpObj->Life*3)/100);
				}
				else
				{
					ad = (int)((lpObj->Life*3)/100);
				}
			}
#else
			if( lpObj->Life > 0 )
			{
				ad = (int)((lpObj->Life*3)/100);
			}
#endif
			
			else ad = 1;
			if( ad < 0 ) ad = 0;

#ifdef ADD_SHIELD_POINT_01_20060403
			// �� ���ݽ� ���� ��밡 ĳ���� �� ���� Shield�� ���ҽ��Ѿ� ��.
			if( g_ShieldSystemOn == TRUE )
			{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
				if( lpObj->lpAttackObj != NULL && 
					lpObj->Type == OBJTYPE_CHARACTER &&lpObj->lpAttackObj->Type == OBJTYPE_CHARACTER )
#else
				if( lpObj->Type == OBJTYPE_CHARACTER && lpObj->lpAttackObj->Type == OBJTYPE_CHARACTER )
#endif
				{
					if( lpObj->iShield > ad )
					{
						lpObj->iShield -= ad;
						iShieldDamage = ad;
						ad = 0;
					}
					else
					{
						ad -= lpObj->iShield;
						iShieldDamage = lpObj->iShield;
						lpObj->iShield = 0;
						lpObj->Life -= ad;
						if( lpObj->Life < 0 )
						{
							lpObj->Life = 0;
						}
					}

	#ifdef SHIELD_POINT_DEBUGLOG
					char Msg[256];
					sprintf(Msg, "[Shield][PoisonDamage]HP:%f,SD:%d,Damage:%d", lpObj->Life, lpObj->iShield, ad );
					GCServerMsgStringSend( Msg, lpObj->m_Index, 1 );
					GCServerMsgStringSend( Msg, lpObj->lpAttackObj->m_Index, 1 );
	#endif
				}
				else
				{
					lpObj->Life -= ad;
					if( lpObj->Life < 0 ) lpObj->Life = 0;
				}
			}
			else
			{
				lpObj->Life -= ad;
				if( lpObj->Life < 0 ) lpObj->Life = 0;
			}
#else
			lpObj->Life -= ad;
			if( lpObj->Life < 0 ) lpObj->Life = 0;
#endif // ADD_SHIELD_POINT_01_20060403
			
#ifdef ADD_SKILL_WITH_COMBO
			if( lpObj->lpAttackObj != NULL )
			{
	#ifdef ADD_SHIELD_POINT_01_20060403
				gObjLifeCheck(lpObj, lpObj->lpAttackObj, ad, 2, 0, 0, AT_SKILL_POISON, iShieldDamage);
	#else
				gObjLifeCheck(lpObj, lpObj->lpAttackObj, ad, 2, 0, 0, AT_SKILL_POISON);
	#endif // ADD_SHIELD_POINT_01_20060403
			}
#else
			if( lpObj->lpAttackObj != NULL )
			{
	#ifdef ADD_SHIELD_POINT_01_20060403
				gObjLifeCheck(lpObj, lpObj->lpAttackObj, ad, 1, 0, 0, iShieldDamage);
	#else
				gObjLifeCheck(lpObj, lpObj->lpAttackObj, ad, 1, 0, 0);
	#endif
			}

#endif
			// �ð��� ������ �����Ѵ�.
			if( lpObj->m_PoisonBeattackCount <= 0 )
			{
				lpObj->lpAttackObj = NULL;

#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
				DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_POISON );
#else
				lpObj->m_ViewSkillState &= 0xFE; //	1��° ��Ʈ
#endif

				lpObj->m_ViewState = 0;

#ifdef ADD_SKILL_WITH_COMBO
				if( lpObj->m_PoisonType == AT_SKILL_EXPPOISON )
				{
					// ���� ������ ���� ����
					GCStateInfoSend(lpObj, 0, 0x01);
				}
				else
				{
					// ���� �������� �ƴ� ��� �� ���� ���� �˸�. 
					GCMagicCancelSend(lpObj, AT_SKILL_POISON);
				}
				lpObj->m_PoisonType = 0;
#else
				GCMagicCancelSend(lpObj, AT_SKILL_POISON);
#endif
			}
		}
	}
	else if( lpObj->m_ColdBeattackCount > 0 ) // �������� ������ �޾Ҵ�..
	{
		//lpObj->m_ViewState = 5;
		lpObj->m_ColdBeattackCount--;

		// �ð��� ������ �����Ѵ�.
		if( lpObj->m_ColdBeattackCount <= 0 )
		{
			lpObj->lpAttackObj = NULL;
			lpObj->DelayActionTime = 0;
			lpObj->DelayLevel = 0;
			
#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_FREEZE );
#else
			lpObj->m_ViewSkillState &= 0xFD; //	2��° ��Ʈ
#endif
			lpObj->m_ViewState = 0;

#ifdef ADD_SKILL_WITH_COMBO					
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428			// ���� ���̽� ��ȭ
			if( lpObj->m_IceType == AT_SKILL_EXPICE 
				|| lpObj->m_IceType == AT_SKILL_ML_STRENGTHEN_EXPICE	// ���� ���̽� ��ȭ
				)
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
			if( lpObj->m_IceType == AT_SKILL_EXPICE )
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
			{
				// ���� ���̽� ���� ����.
				GCStateInfoSend(lpObj, 0, 0x02);
			}
			else
			{
				// ���� ���̽��� �ƴҶ��� ���� ���� ���� ����.
				GCMagicCancelSend(lpObj, AT_SKILL_SLOW);			
			}
			lpObj->m_IceType = 0;
#else
			GCMagicCancelSend(lpObj, AT_SKILL_SLOW);			
#endif
		}
	}
#ifdef ADD_NEW_MAP_KALIMA_20040518
	else if( lpObj->m_ImmuneToMagicCount > 0 ) {	// �������� ���� �ð� �̴�.
		lpObj->m_ImmuneToMagicCount--;

		if( lpObj->m_ImmuneToMagicCount <= 0 )
		{
			lpObj->m_ViewSkillState	&= ~STATE_IMMUNE_TO_MAGIC;
			GCStateInfoSend(lpObj, 1, STATE_IMMUNE_TO_MAGIC);
		}
	}
	else if( lpObj->m_ImmuneToHarmCount > 0 ) {		// �������� ���� �ð� �̴�.
		lpObj->m_ImmuneToHarmCount--;

		if( lpObj->m_ImmuneToHarmCount <= 0 )
		{
			lpObj->m_ViewSkillState	&= ~STATE_IMMUNE_TO_HARM;
			GCStateInfoSend(lpObj, 0, STATE_IMMUNE_TO_HARM);
		}
	}
#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
}

// �����̵��� ��������� 
void gObjTeleportMagicUse(int aIndex, BYTE x, BYTE y)
{
	LPOBJECTSTRUCT lpObj;
	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	
#ifdef MODIFY_TELEPORT_USING_HELLBURST_BUGFIX_20080822
	// �����Ʈ �������̶�� �����Ʈ ���
	// Ŭ���̾�Ʈ���� �̷� ��Ȳ�� �߻����� ���ϵ��� �����Ͽ���
	if( lpObj->Type == OBJTYPE_CHARACTER && lpObj->Class == CLASS_WIZARD && lpObj->SkillHellFire2State )
	{
		lpObj->SkillHellFire2State = FALSE;
		lpObj->SkillHellFire2Time = 0;
	}
#endif // MODIFY_TELEPORT_USING_HELLBURST_BUGFIX_20080822

	if( lpObj->Teleport ) return;

	lpObj->TeleportTime = GetTickCount();
	lpObj->PathCount	= 0;
	//lpObj->m_State		= OBJST_DELCMD;
	lpObj->Teleport     = 1;
	lpObj->m_StateSub   = 1;
	lpObj->m_ViewState  = 1;
	lpObj->X			= x;
	lpObj->Y			= y;
	lpObj->TX			= x;
	lpObj->TY			= y;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);
	lpObj->m_OldX		= lpObj->TX;
	lpObj->m_OldY		= lpObj->TY;
	//LogAddC(LOGC_RED,"�����̵� ���� %s %d", lpObj->Name, __LINE__);

	gObjViewportListProtocolDestroy(lpObj);

	/*
	int tObjNum;
	for( int n=0; n<MAXVIEWPORTOBJECT; n++)
	{
		// �� ����Ʈ�� ������Ʈ��..
		if( (lpObj->VpPlayer[n].state == STVP_CREATE ||
			lpObj->VpPlayer[n].state == STVP_PLAYING)) 
		{
			tObjNum = lpObj->VpPlayer[n].number;
			gObjViewportListDestroy(tObjNum);
		}
	}
	*/
}

void GetMoveArea(short &x, short &y, BYTE MapNumber, BYTE x1, BYTE y1, BYTE x2, BYTE y2, BYTE & dir)
{
	int tx, ty;
	int loopcount=10;
	BYTE attr;

	while( loopcount-- )
	{
		if( (x2-x1) > 0 )
		{
			tx = x1+(rand()%((x2-x1)));
		}
		else tx = x1;

		if( (y2-y1) > 0 )
		{
			ty = y1+(rand()%((y2-y1)));
		}
		else ty = y1;

		attr = MapC[MapNumber].GetAttr(tx, ty);
		if( ((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) && 
			((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) &&
			((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) 
		{			
			break;
		}
	}
	x			= tx;
	y			= ty;
	dir			= 0;
}

// ���̵��� ��������� 
BOOL gObjMovePlayer(int aIndex, BYTE mapnumber, BYTE x1, BYTE y1, BYTE x2, BYTE y2)
{
	LPOBJECTSTRUCT lpObj;
	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	int  result; 	
	short x, y;
	BYTE  dir;
	int  movefail=0;


#ifdef DARKLORD_WORK	
	if( gObj[aIndex].SkillRecallParty_Time )
	{
		gObj[aIndex].SkillRecallParty_Time = 0;
		char msg[255];
		wsprintf(msg, lMsg.Get(1254));	// "��ȯ���"
		GCServerMsgStringSend(msg, aIndex, 1);	
	}
#endif

#ifdef DARKLORD_WORK
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{	// �̵� �߿��� ��ũ���Ǹ� ��� ����
		gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::PetItem_Mode_Normal);
	}
#endif

#ifdef FOR_BLOODCASTLE
	if( CHECK_BLOODCASTLE(lpObj->MapNumber))
	{	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpObj->MapNumber );
		if (g_BloodCastle.GetCurrentState(iBridgeIndex) == BLOODCASTLE_STATE_PLAYING) {
#else
		if (g_BloodCastle.GetCurrentState(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1) == BLOODCASTLE_STATE_PLAYING) {
#endif
			// ���� ��� ���̶�� ���⼭ ����Ʈ �������� ã�Ƽ� ������ �ش�.
			g_BloodCastle.SearchUserDropQuestItem (lpObj->m_Index);
		}
		else {
			// ���� ��� ���� �ƴ϶�� ���⼭ ����Ʈ �������� ã�Ƽ� �����ش�.
			g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
		}
	}
#endif
	
	//if( gObj[aIndex].m_MoveGateNumber>=0 ) return;
	if( gObj[aIndex].RegenOk ) 
	{
		LogAdd("Regen OK %s %d", __FILE__, __LINE__);
		x = lpObj->X;
		y = lpObj->Y;
		mapnumber = lpObj->MapNumber;
		dir  = lpObj->Dir;
		movefail = 1;		
		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = (BYTE)x;
		lpObj->RegenMapY = (BYTE)y;		
		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

		if( lpObj->m_Change >= 0 )
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
		gObj[aIndex].RegenOk   = 1;
		return FALSE;
	}

	GetMoveArea(x, y, mapnumber, x1, y1, x2, y2, dir);
	
#ifdef MODIFY_ITEM_DUPLICATION_IN_MAPSERVER_MOVING_20051219
	if( lpObj->m_bPShopOpen == TRUE )
	{
		BOOL bMove = TRUE;

		// ���λ��� ���� : ������ -> �η��þ�
		if( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE 
			&& mapNumber == MAP_INDEX_RORENCIA )
		{
			bMove = FALSE;
		}
		// ���λ��� ���� : �η��þ� -> ������
		if( lpObj->MapNumber == MAP_INDEX_RORENCIA
			&& mapNumber == MAP_INDEX_CASTLESIEGE )
		{
			bMove = FALSE;
		}

		if( bMove == FALSE )
		{
			LogAddTD("[PSHOP ITEM CLONNING] Move Failed : %d (%s %s)", result, lpObj->AccountID, lpObj->Name);
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir  = lpObj->Dir;
			movefail = 1;

#ifdef MODIFY_MAPMOVE_SET_REGEN_POSITION_20060905
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			lpObj->RegenMapNumber = mapNumber;
#endif // MODIFY_MAPMOVE_SET_REGEN_POSITION_20060905

	#ifdef FOR_KOREA
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			MsgOutput( lpObj->m_Index, lMsg.Get(3356) );			// "���λ����� ���� �̵� �� �� �����ϴ�."
	#endif

#ifdef MODIFY_MOVEGATE_CHECK_PERSONALSHOP_OPEN_20060704
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = (BYTE)x;
			lpObj->RegenMapY = (BYTE)y;		
			gObjClearViewport(&gObj[aIndex]);
			GCTeleportSend(&gObj[aIndex], -1, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

			if( lpObj->m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}

			gObj[aIndex].RegenOk   = 1;
			return FALSE;
#endif // MODIFY_MOVEGATE_CHECK_PERSONALSHOP_OPEN_20060704

		}
	}
		
#endif // MODIFY_ITEM_DUPLICATION_IN_MAPSERVER_MOVING_20051219

// ����, ����ĳ��, ī����ĳ�������� �������� ���� ȿ���� �����Ѵ�.(apple)
#ifdef MODIFY_NPC_SHADOW_PAHTOM_20050628
	if( CHECK_BLOODCASTLE(mapnumber) 
		|| CHECK_CHAOSCASTLE(mapnumber) 
		|| CHECK_DEVILSQUARE(mapnumber)
#ifdef MODIFY_SHADOW_PHANTOM_BUFF_IN_ILLUSION_TEMPLE_BUGFIX_20080731
		|| CHECK_ILLUSION_TEMPLE(mapnumber)
#endif // MODIFY_SHADOW_PHANTOM_BUFF_IN_ILLUSION_TEMPLE_BUGFIX_20080731
	  )
	{
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_ATTDEF_POWER_INC ) == true )
		{
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_ATTDEF_POWER_INC );
		}
	#else
		if(lpObj->m_iSkillNPCHelpTime > 0)
		{
			lpObj->m_iSkillNPCHelpTime = 0;
			lpObj->m_iSkillNPCDefense = 0; 
			lpObj->m_iSkillNPCAttack = 0;
			
			DEL_VIEWSKILLSTATE(lpObj->m_ViewSkillState, STATE_NPC_HELP);
			GCStateInfoSend(lpObj, 0, STATE_NPC_HELP);
		}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	#ifdef ADD_EVENT_MAP_BUFF_EFFECT_DELETE_20090421
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_ATTACK_POWER_INC ) == true )
		{	// ���ݷ� ���
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_ATTACK_POWER_INC );
		}
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_DEFENSE_POWER_INC ) == true )
		{	// ���� ���
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_DEFENSE_POWER_INC );
		}
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_CRITICAL_DMG_INC ) == true )
		{	// ũ��Ƽ�� Ȯ�� ����
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_CRITICAL_DMG_INC );
		}
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_HP_INC ) == true )
		{	// ����������
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_HP_INC );
		}
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_MELEE_DEFENSE_INC ) == true )
		{	// �ҿ� �ٸ���
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_MELEE_DEFENSE_INC );
		}
	#endif // ADD_EVENT_MAP_BUFF_EFFECT_DELETE_20090421
	}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��� �ʿ��� ����
	if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
		&& mapnumber != lpObj->MapNumber )
	{
		g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
		g_IllusionTempleEvent.LeaveIllusionTemple(lpObj->m_Index, lpObj->MapNumber);
	}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// �� ó�� - �� �Ա��� ������ �߿��� ������ �¾ƾ� ������ �� �ִ�.
	if (result == 97) {
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
			if (lpObj->m_btCsJoinSide != CS_SIEGESIDE_DEFEND && g_CastleSiege.GetCastleTowerAccessable() == FALSE) {
				// ��ȣ������ �ı����� �ʾ����Ƿ� ������ �� ����.
				GCServerMsgStringSend(lMsg.Get(1624), lpObj->m_Index, 1);	// "��ȣ������ �ı����� �ʾ����Ƿ� ������ �� �����ϴ�."
				x = lpObj->X;
				y = lpObj->Y;
				mapnumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
			else if (lpObj->m_btCsJoinSide == CS_SIEGESIDE_NONE) {
				// ������ �����ڰ� �ƴϸ� ������ �� ����.
				GCServerMsgStringSend(lMsg.Get(1625), lpObj->m_Index, 1);	// "������ �����ڸ� ���� �����մϴ�."
				x = lpObj->X;
				y = lpObj->Y;
				mapnumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
		else {
			if (g_CastleSiege.CheckCastleOwnerUnionMember (lpObj->m_Index) == FALSE) {
				// ���� ����� �Ͽ��� �ƴϸ� ������ �� �����ϴ�.
				GCServerMsgStringSend(lMsg.Get(1626), lpObj->m_Index, 1);	// "���� ����� �Ͽ��� �ƴϸ� ������ �� �����ϴ�."
				x = lpObj->X;
				y = lpObj->Y;
				mapnumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
	}
#endif	

#ifdef NEW_SKILL_FORSKYLAND
	if( mapnumber == MAP_INDEX_ATHLANSE )	// ��Ʋ���� �Ǵ� õ���ʿ����� ���ϸ��Ƹ� Ÿ�� �̵��Ҽ� ����
#else
	if( mapnumber == MAP_INDEX_ATHLANSE )
#endif
	{	// ��Ʋ������ �̵� �Ұ�� ���ϸ��Ƹ� Ÿ�� �̵��Ҽ� ����
		if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem())
		{							
			if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) )
			{
				GCServerMsgStringSend(lMsg.Get(702), lpObj->m_Index, 1);
				x = lpObj->X;
				y = lpObj->Y;
				mapnumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}

			if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) )
			{
				GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);
				x = lpObj->X;
				y = lpObj->Y;
				mapnumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
	}

#ifdef NEW_SKILL_FORSKYLAND	
	if( mapnumber == MAP_INDEX_ICARUS )
	{
		if( (lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3) && 
#ifdef DARKLORD_WORK
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(13,30) &&	// ����
#endif
#ifdef ADD_ITEM_FENRIR_01_20051110
			lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,37) &&	// �渱
#endif
#ifdef MODIFY_SKYLAND_FENRIR_AND_WING_BUGFIX_20080730
			lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,4) &&	// ��ũȣ��
#endif // MODIFY_SKYLAND_FENRIR_AND_WING_BUGFIX_20080730
#ifdef ADD_THIRD_WING_20070525	// ��ī�罺�̵��� �ʿ� �����ۿ� 3������ �߰�
			(  lpObj->pInventory[EQUIPMENT_WING].IsItem() == FALSE ))	|| // ������ ����
#else
			(lpObj->pInventory[EQUIPMENT_WING].m_Type < MAKE_ITEMNUM(12,0) || lpObj->pInventory[EQUIPMENT_WING].m_Type > MAKE_ITEMNUM(12,6) )) ||
#endif
			 lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) ||
			 lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) ||
			 lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) 
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ��ī�罺 �̵� �Ұ�
			 || lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 )
			 || lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) 	
#endif	
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
			 || lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 )
			 || lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 )
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
			 || lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 )
			 || lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 )
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
#ifdef MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
			 || lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 )
			 || lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 )
			 || lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 )
			 || lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 )
#endif // MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
		  )
		{	// �䰡�þƳ� ������ �����ϰ� ���� ������ �̵��Ҽ� ����
			GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);
			x = lpObj->X;
			y = lpObj->Y;
			mapnumber = lpObj->MapNumber;
			dir  = lpObj->Dir;
			movefail = 1;
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = (BYTE)x;
			lpObj->RegenMapY = (BYTE)y;		

	//####### BUGFIX_CAT001 �� �̵� ���� ��, �þ� ó�� �ȵǴ� ���� �ذ�
			//gObjClearViewport(&gObj[aIndex]); // ���� �ִ� �� ������ �ּ� ó���ϰ�, 
			GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

			//�Ʒ� �Լ��� ȣ�� �ϵ��� �ٲ�			
			gObjClearViewportOfMine( lpObj );

	//BUGFIX_CAT001 ########

			if( lpObj->m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}

			gObj[aIndex].RegenOk   = 1;
			return FALSE;
		}

		if( lpObj->m_RecallMon >= 0 ) 
		{
			GCRecallMonLife(aIndex, 60, 0);
			gObjMonsterCallKill(aIndex);
		}
	}
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518					// Į�������� �ٸ� ������ �̵��� ���� ������ ���ش�.
	if( !CHECK_KALIMAMAP(mapnumber) &&
		CHECK_KALIMAMAP(lpObj->MapNumber)
		) 
	{
		// ������ ���ش�.
		INT iKalimaGateIndex = lpObj->m_iKalimaGateIndex;			// �α� ����� �� �ӽú���
		if (g_KalimaGate.DeleteKalimaGate(lpObj->m_Index)) {
			LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - User MoveGate (SummonIndex:%d, EnterCount:%d)", 
				lpObj->AccountID, 
				lpObj->Name,
				iKalimaGateIndex,
				gObj[iKalimaGateIndex].m_cKalimaGateEnterCount
				);
		}
	}
#endif

#ifdef ADD_KANTURU_CONDITION_20060725			
	if( mapnumber == MAP_INDEX_KANTURU_BOSS )
	{
		if( ( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3)			// ����Ʈ
#ifdef ADD_ITEM_FENRIR_01_20051110
			&& lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,37)			// �渱
#endif			
#ifdef MODIFY_KANTURU_ENTERANCE_DARKHORSE_BUGFIX_20080627
			&& lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,4)				// ��ũȣ��
#endif // MODIFY_KANTURU_ENTERANCE_DARKHORSE_BUGFIX_20080627
#ifdef ADD_THIRD_WING_20070525	// ĭ���� ������ �������ǿ� 3������ �߰�	
			&& ( lpObj->pInventory[EQUIPMENT_WING].IsItem() == FALSE ) )	||	// ������ ����.
#else
			) ||
#endif
			 lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) ||		// ���Ź���
			 lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10)			// ���Ź���
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ĭ������ ���� �Ұ�
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 )	
#endif
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 )
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 )
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
#ifdef MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 )
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 )
#endif // MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
			  )
		{
			// ���� ���н� ���� �޽��� ����
			GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);

			// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
			x = lpObj->X;
			y = lpObj->Y;
			mapnumber = lpObj->MapNumber;
			dir  = lpObj->Dir;
			movefail = 1;
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = (BYTE)x;
			lpObj->RegenMapY = (BYTE)y;

			//####### BUGFIX_CAT001 �� �̵� ���� ��, �þ� ó�� �ȵǴ� ���� �ذ�
			//gObjClearViewport(&gObj[aIndex]); // ���� �ִ� �� ������ �ּ� ó���ϰ�, 
			GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

			//�Ʒ� �Լ��� ȣ�� �ϵ��� �ٲ�
			gObjClearViewportOfMine( lpObj );

			//BUGFIX_CAT001 ########

			if( lpObj->m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}

			gObj[aIndex].RegenOk   = 1;
			return FALSE;	

		}
	}
#endif	// ADD_KANTURU_CONDITION_20060725
	


#ifdef MODIFY_KANTURU_MAPNUMBER_CHECK_20060807
	if( g_iBlockKanturuMapEnter == 1 )
	{
		x			= lpObj->X;
		y			= lpObj->Y;
		mapnumber	= lpObj->MapNumber;
		dir			= lpObj->Dir;
		int level		= lpObj->Level;

		GetMoveArea(x, y, mapnumber, x1, y1, x2, y2, dir);
		
		{
			if( mapnumber >= MAP_INDEX_KANTURU1 && mapnumber <= MAP_INDEX_KANTURU_BOSS )
			{
				// ���� ���н� ���� �޽��� ����
				GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);

				// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
				x = lpObj->X;
				y = lpObj->Y;
				mapnumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;

				//####### BUGFIX_CAT001 �� �̵� ���� ��, �þ� ó�� �ȵǴ� ���� �ذ�
				//gObjClearViewport(&gObj[aIndex]); // ���� �ִ� �� ������ �ּ� ó���ϰ�, 
				GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				//�Ʒ� �Լ��� ȣ�� �ϵ��� �ٲ�
				gObjClearViewportOfMine( lpObj );

				//BUGFIX_CAT001 ########

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
	}

	if( g_iBlockCastleSiegeMapEnter == 1 )
	{
		x			= lpObj->X;
		y			= lpObj->Y;
		mapnumber	= lpObj->MapNumber;
		dir			= lpObj->Dir;
		int level		= lpObj->Level;
		
		GetMoveArea(x, y, mapnumber, x1, y1, x2, y2, dir);
		{
			if( mapnumber == MAP_INDEX_CASTLESIEGE
				|| mapnumber == MAP_INDEX_CASTLEHUNTZONE
				|| mapnumber == MAP_INDEX_CRYWOLF_FIRSTZONE
				|| mapnumber == MAP_INDEX_CRYWOLF_SECONDZONE )
			{
				// ���� ���н� ���� �޽��� ����
				GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);

				// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
				x = lpObj->X;
				y = lpObj->Y;
				mapnumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;

				//####### BUGFIX_CAT001 �� �̵� ���� ��, �þ� ó�� �ȵǴ� ���� �ذ�
				//gObjClearViewport(&gObj[aIndex]); // ���� �ִ� �� ������ �ּ� ó���ϰ�, 
				GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				//�Ʒ� �Լ��� ȣ�� �ϵ��� �ٲ�
				gObjClearViewportOfMine( lpObj );

				//BUGFIX_CAT001 ########

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
	}
#endif // MODIFY_KANTURU_MAPNUMBER_CHECK_20060807

#if 0 //def ADD_RAKLION_20080408
	if( mapnumber == MAP_INDEX_RAKLION_BOSS )
	{
		if( g_Raklion.GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR )
		{
			// ���� ���н� ���� �޽��� ����
			GCServerMsgStringSend(lMsg.Get(3415), lpObj->m_Index, 1);
			
			// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
			x = lpObj->X;
			y = lpObj->Y;
			mapnumber = lpObj->MapNumber;
			dir  = lpObj->Dir;
			movefail = 1;
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = (BYTE)x;
			lpObj->RegenMapY = (BYTE)y;
			
			GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);
			
			gObjClearViewportOfMine( lpObj );
			
			if( lpObj->m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			
			gObj[aIndex].RegenOk   = 1;
			return FALSE;
		}
		else
		{
			if( gt == 290 )
				g_RaklionBattleUserMng.AddUserData( aIndex );
		}
	}
	if( mapnumber == MAP_INDEX_RAKLION_FIELD )
	{
		if( gt == 292 )	// ������ -> ��Ŭ���� �ʵ� �̵���
		{
			if( g_Raklion.GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR )	// ������ ��� ����
			{
				// ���� ���н� ���� �޽��� ����
				GCServerMsgStringSend(lMsg.Get(3416), lpObj->m_Index, 1);
				
				// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
				x = lpObj->X;
				y = lpObj->Y;
				mapnumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;
				
				GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);
				
				gObjClearViewportOfMine( lpObj );
				
				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				
				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
			else
			{
				g_RaklionBattleUserMng.DeleteUserData( aIndex );
			}
		}
		else
		{	// �ٸ��� -> ��Ŭ�������� ���Դ�. ��Ŭ���� �ʵ忡 ���� �������� ���� ������ ���¸� �����Ѵ�.
			g_RaklionUtil.NotifyRaklionCurrentState( aIndex, g_Raklion.GetRaklionState(),
				g_Raklion.GetRaklionStateDetail() );
		}
	}
#endif // ADD_RAKLION_20080408

	if( gObj[aIndex].m_IfState.use )
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapnumber = lpObj->MapNumber;
		dir  = lpObj->Dir;
		movefail = 1;
		//LogAddL("�� �̵��� �������̽� ��������� �̵� ��ҵ�");
	}
	if( gObj[aIndex].DieRegen ) 
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapnumber = lpObj->MapNumber;
		dir  = lpObj->Dir;
		movefail = 1;
		//LogAddL("�׾ �̵� %d", mapNumber);
	}

	lpObj->m_State = OBJST_DELCMD;
	if( movefail == 0 )
	{
#ifdef MAP_SERVER_WORK_20041030		// �ʼ����� �̵����� üũ
		GetMoveArea(x,y,mapnumber,x1,y1,x2,y2,dir);
		{
			if (gObj[aIndex].MapNumber != mapnumber) {
				// �ٸ� ������ �̵��ϰ��� �� ���� �ʼ��� �̵������� üũ�Ѵ�.
				if( !MapNumberCheck(mapnumber) ) 
				{
					LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
					return FALSE;
				}
				
				SHORT sSvrCode	= g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, mapnumber, lpObj->m_sPrevMapSvrCode);
				if (sSvrCode != gGameServerCode) {
					if (sSvrCode == -1) {
						// ġ������ ���� -> ���� ����
						LogAddC(LOGC_RED, "[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",
							lpObj->AccountID,
							lpObj->Name,
							lpObj->m_Index
							);
						return FALSE;
					}

					// ������� ������ �̵��� �������� ���θ� üũ�Ѵ�. (b4nfter)
/*					if (gGateC.CheckGateLevel(aIndex, gt) == FALSE) {
						// ������ ���Ƽ� �̵� �Ұ���
						x = lpObj->X;
						y = lpObj->Y;
						mapnumber = lpObj->MapNumber;
						dir  = lpObj->Dir;
						movefail = 1;
						lpObj->RegenMapNumber = lpObj->MapNumber;
						lpObj->RegenMapX = (BYTE)x;
						lpObj->RegenMapY = (BYTE)y;		
						gObjClearViewport(&gObj[aIndex]);
						GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

						if( lpObj->m_Change >= 0 )
						{
							gObjViewportListProtocolCreate(&gObj[aIndex]);
						}

						gObj[aIndex].RegenOk   = 1;

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �����ʿ����� �ڷ���Ʈ ���Ŀ� �ڽ��� �������� ���¸� �ٽ� ������
						if (gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE &&
							g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
							)
						{
							g_CastleSiege.NotifySelfCsJoinSide (aIndex);
							g_CastleSiege.NotifyCsSelfLeftTime (aIndex);
						}
#endif
						return FALSE;
					}*/

					// ���������� �ʼ��� �̵� ��û�� �Ѵ�.
					GJReqMapSvrMove (lpObj->m_Index, sSvrCode, mapnumber, x, y);
					LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",
						sSvrCode,
						lpObj->AccountID,
						lpObj->Name,
						lpObj->m_Index
						);
					return FALSE;
				}
			}
		}
#endif

#ifdef EXTEND_LOG_SYSTEM_20060202
		LogAddTD("[MoveMap][%s][%s] : (Before:Map:%d X:%d Y:%d) -> (New Map:%d X:%d Y:%d)",
			lpObj->AccountID,	lpObj->Name,
			lpObj->MapNumber,	lpObj->X,	lpObj->Y, 
			mapnumber, x, y );
#endif

		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;
		gObj[aIndex].TX = x;
#ifdef MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObj[aIndex].TY = y;
#else	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObj[aIndex].TX = y;
#endif	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObj[aIndex].MapNumber = mapnumber;
		gObj[aIndex].Dir = dir; 
		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport  = 0;

#ifdef MODIFY_AFTERIMAGE_BUGFIX_20060215
		// ���� �̺κ� �ּ�ó�� �Ǿ� �־���.
		// �������� : Map �̵��� ����Ʈ ����Ʈ ���������� ����
		gObjViewportListProtocolDestroy(&gObj[aIndex]);
#endif

#ifdef MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObjViewportListProtocolDestroy(&gObj[aIndex]);
		//gObjViewportListProtocolCreate(&gObj[aIndex]);
#endif	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024

#ifndef MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObjClearViewport(&gObj[aIndex]);
#endif	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

#ifdef MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObjViewportListProtocolCreate(&gObj[aIndex]);
		gObjClearViewport(&gObj[aIndex]);
#endif	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		
		if( lpObj->m_Change >= 0 )
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenMapNumber = mapnumber;
		gObj[aIndex].RegenMapX = (BYTE)x;
		gObj[aIndex].RegenMapY = (BYTE)y;		
		gObj[aIndex].RegenOk   = 1;
	}
	else
	{
		GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);
		gObj[aIndex].RegenOk   = 1;
	

#ifdef CASTLE_MAIN_SCHEDULER_20041111						// �����ʿ����� �ڷ���Ʈ ���Ŀ� �ڽ��� �������� ���¸� �ٽ� ������
		if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE &&
			g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
			)
		{
			g_CastleSiege.NotifySelfCsJoinSide (aIndex);
			g_CastleSiege.NotifyCsSelfLeftTime (aIndex);
		}
#endif
	}
	//LogAddL("�̵� �޽��� �Ϸ�");

	if( movefail )
		return FALSE;

	if( gObj[aIndex].Type == OBJTYPE_CHARACTER )
	{
		gObj[aIndex].m_LastTeleportTime = 10;				// �̵��� ����, ���� �ð� ��������� ���� �ڷ� �ȹи��� �����ϱ� ���� ��
	}
	
#ifdef ADD_NEWPVP_PKFIELD
	g_NewPVP.Reset(*lpObj);
#endif // ADD_NEWPVP_PKFIELD

	return TRUE;
}

BOOL gObjMoveGate(int aIndex, int gt)
{
	LPOBJECTSTRUCT lpObj;
	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	int  result; 	
	short x, y;
	BYTE  mapNumber=0;
	BYTE  dir;
	short level;
	int  userlevel = gObj[aIndex].Level;
	int  movefail=0;


#ifdef DARKLORD_WORK	
	if( gObj[aIndex].SkillRecallParty_Time )
	{
		gObj[aIndex].SkillRecallParty_Time = 0;
		char msg[255];
		wsprintf(msg, lMsg.Get(1254));	// "��ȯ���"
		GCServerMsgStringSend(msg, aIndex, 1);	
	}
#endif

#ifdef DARKLORD_WORK
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{	// �̵� �߿��� ��ũ���Ǹ� ��� ����
		gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::PetItem_Mode_Normal);
	}
#endif

#ifdef FOR_BLOODCASTLE
	if( CHECK_BLOODCASTLE(lpObj->MapNumber))
	{	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpObj->MapNumber );
		if (g_BloodCastle.GetCurrentState(iBridgeIndex) == BLOODCASTLE_STATE_PLAYING) {
#else
		if (g_BloodCastle.GetCurrentState(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1) == BLOODCASTLE_STATE_PLAYING) {
#endif
			// ���� ��� ���̶�� ���⼭ ����Ʈ �������� ã�Ƽ� ������ �ش�.
			g_BloodCastle.SearchUserDropQuestItem (lpObj->m_Index);
		}
		else {
			// ���� ��� ���� �ƴ϶�� ���⼭ ����Ʈ �������� ã�Ƽ� �����ش�.
			g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
		}
	}
#endif

	//if( gObj[aIndex].m_MoveGateNumber>=0 ) return;
	if( gObj[aIndex].RegenOk ) 
	{
		LogAdd("Regen OK %s %d", __FILE__, __LINE__);
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir  = lpObj->Dir;
		movefail = 1;		
		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = (BYTE)x;
		lpObj->RegenMapY = (BYTE)y;		
		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

		if( lpObj->m_Change >= 0 )
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
		gObj[aIndex].RegenOk   = 1;
		return FALSE;
	}

	if( gGateC.IsGate(gt) == FALSE ) 
	{
		LogAdd(lMsg.Get(545), gt, __FILE__, __LINE__);
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir  = lpObj->Dir;
		movefail = 1;
		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = (BYTE)x;
		lpObj->RegenMapY = (BYTE)y;		
		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

		if( lpObj->m_Change >= 0 )
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
		gObj[aIndex].RegenOk   = 1;
		return FALSE;
	}	

	if(gt == 58 && gObj[aIndex].m_stateEngagePVP == PVP_USER_PLAYING)
	{

	}
#if TESTSERVER == 0
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// �Ǹ��� ���� �����ڵ� �߰�
	else if( ( gt >= 58 && gt <= 61 ) || gt == 111 || gt == 112 || gt == 270 )
#else
	else if( gt >= 58 && gt <= 61 )
#endif
	{	// �Ǹ��� �������� �̵��Ҷ�.. �����ڵ尡 ������, �̵��Ҽ� ����
		if( gObj[aIndex].m_bDevilSquareAuth == FALSE )
		{
			LogAddTD("[DevilSquare] [%s][%s] Not Used DevilSquare Invitation In DevilSquare",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
			return FALSE;
		}

		gObj[aIndex].m_bDevilSquareAuth = FALSE;
	}
#endif


#ifdef ADD_SEASON_3_NEW_MAP_20070910
	lpObj->m_MoveGateNumber = (short)gt;
#else
	lpObj->m_MoveGateNumber = (BYTE)gt;
#endif	
	result = gGateC.GetGate(gt, x, y, mapNumber, dir, level);
	if( result < 0 )	
	{
		LogAdd("error : %d (%s %d)", result, __FILE__, __LINE__);
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir  = lpObj->Dir;
		movefail = 1;
	}

#ifdef MODIFY_ITEM_DUPLICATION_IN_MAPSERVER_MOVING_20051219
	if( lpObj->m_bPShopOpen == TRUE )
	{
		BOOL bMove = TRUE;

		// ���λ��� ���� : ������ -> �η��þ�
		if( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE 
			&& mapNumber == MAP_INDEX_RORENCIA )
		{
			bMove = FALSE;
		}
		// ���λ��� ���� : �η��þ� -> ������
		if( lpObj->MapNumber == MAP_INDEX_RORENCIA
			&& mapNumber == MAP_INDEX_CASTLESIEGE )
		{
			bMove = FALSE;
		}

		if( bMove == FALSE )
		{
			LogAddTD("[PSHOP ITEM CLONNING] Move Failed : %d (%s %s)", result, lpObj->AccountID, lpObj->Name);
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir  = lpObj->Dir;
			movefail = 1;

#ifdef MODIFY_MAPMOVE_SET_REGEN_POSITION_20060905
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			lpObj->RegenMapNumber = mapNumber;
#endif // MODIFY_MAPMOVE_SET_REGEN_POSITION_20060905

#ifdef FOR_KOREA
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			MsgOutput( lpObj->m_Index, lMsg.Get(3356) );			// "���λ����� ���� �̵� �� �� �����ϴ�."
#endif

#ifdef MODIFY_MOVEGATE_CHECK_PERSONALSHOP_OPEN_20060704
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = (BYTE)x;
			lpObj->RegenMapY = (BYTE)y;		
			gObjClearViewport(&gObj[aIndex]);
			GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

			if( lpObj->m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}

			gObj[aIndex].RegenOk   = 1;
			return FALSE;
#endif // MODIFY_MOVEGATE_CHECK_PERSONALSHOP_OPEN_20060704

		}
	}

#endif // MODIFY_ITEM_DUPLICATION_IN_MAPSERVER_MOVING_20051219

	// ����, ����ĳ��, ī����ĳ�������� �������� ���� ȿ���� �����Ѵ�.(apple)
#ifdef MODIFY_NPC_SHADOW_PAHTOM_20050628
	if( CHECK_BLOODCASTLE(mapNumber) 
		|| CHECK_CHAOSCASTLE(mapNumber) 
		|| CHECK_DEVILSQUARE(mapNumber)
#ifdef MODIFY_SHADOW_PHANTOM_BUFF_IN_ILLUSION_TEMPLE_BUGFIX_20080731
		|| CHECK_ILLUSION_TEMPLE(mapNumber)
#endif // MODIFY_SHADOW_PHANTOM_BUFF_IN_ILLUSION_TEMPLE_BUGFIX_20080731
		)
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_ATTDEF_POWER_INC ) == true )
		{
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_ATTDEF_POWER_INC );
		}
#else
		if(lpObj->m_iSkillNPCHelpTime > 0)
		{
			lpObj->m_iSkillNPCHelpTime = 0;
			lpObj->m_iSkillNPCDefense = 0; 
			lpObj->m_iSkillNPCAttack = 0;

			DEL_VIEWSKILLSTATE(lpObj->m_ViewSkillState, STATE_NPC_HELP);
			GCStateInfoSend(lpObj, 0, STATE_NPC_HELP);
		}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifdef ADD_EVENT_MAP_BUFF_EFFECT_DELETE_20090421
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_ATTACK_POWER_INC ) == true )
		{	// ���ݷ� ���
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_ATTACK_POWER_INC );
		}
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_DEFENSE_POWER_INC ) == true )
		{	// ���� ���
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_DEFENSE_POWER_INC );
		}
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_CRITICAL_DMG_INC ) == true )
		{	// ũ��Ƽ�� Ȯ�� ����
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_CRITICAL_DMG_INC );
		}
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_HP_INC ) == true )
		{	// ����������
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_HP_INC );
		}
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_MELEE_DEFENSE_INC ) == true )
		{	// �ҿ� �ٸ���
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_MELEE_DEFENSE_INC );
		}
#endif // ADD_EVENT_MAP_BUFF_EFFECT_DELETE_20090421
	}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��� �ʿ��� ����
	if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
		&& mapNumber != lpObj->MapNumber )
	{
		g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
		g_IllusionTempleEvent.LeaveIllusionTemple(lpObj->m_Index, lpObj->MapNumber);
	}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// �� ó�� - �� �Ա��� ������ �߿��� ������ �¾ƾ� ������ �� �ִ�.
	if (result == 97) {
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
			if (lpObj->m_btCsJoinSide != CS_SIEGESIDE_DEFEND && g_CastleSiege.GetCastleTowerAccessable() == FALSE) {
				// ��ȣ������ �ı����� �ʾ����Ƿ� ������ �� ����.
				GCServerMsgStringSend(lMsg.Get(1624), lpObj->m_Index, 1);	// "��ȣ������ �ı����� �ʾ����Ƿ� ������ �� �����ϴ�."
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
			else if (lpObj->m_btCsJoinSide == CS_SIEGESIDE_NONE) {
				// ������ �����ڰ� �ƴϸ� ������ �� ����.
				GCServerMsgStringSend(lMsg.Get(1625), lpObj->m_Index, 1);	// "������ �����ڸ� ���� �����մϴ�."
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
		else {
			if (g_CastleSiege.CheckCastleOwnerUnionMember (lpObj->m_Index) == FALSE) {
				// ���� ����� �Ͽ��� �ƴϸ� ������ �� �����ϴ�.
				GCServerMsgStringSend(lMsg.Get(1626), lpObj->m_Index, 1);	// "���� ����� �Ͽ��� �ƴϸ� ������ �� �����ϴ�."
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
	}
#endif	

#ifdef NEW_SKILL_FORSKYLAND
	if( mapNumber == MAP_INDEX_ATHLANSE )	// ��Ʋ���� �Ǵ� õ���ʿ����� ���ϸ��Ƹ� Ÿ�� �̵��Ҽ� ����
#else
	if( mapNumber == MAP_INDEX_ATHLANSE )
#endif
	{	// ��Ʋ������ �̵� �Ұ�� ���ϸ��Ƹ� Ÿ�� �̵��Ҽ� ����
		if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem())
		{							
			if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) )
			{
				GCServerMsgStringSend(lMsg.Get(702), lpObj->m_Index, 1);
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}

			if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) )
			{
				GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;		
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
	}

#ifdef NEW_SKILL_FORSKYLAND	
	if( mapNumber == MAP_INDEX_ICARUS )
	{
		if( (lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3) && 
#ifdef DARKLORD_WORK
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(13,30) &&	// ����
#endif
#ifdef ADD_ITEM_FENRIR_01_20051110
			lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,37) &&	// �渱
#endif
#ifdef MODIFY_SKYLAND_FENRIR_AND_WING_BUGFIX_20080730
			lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,4) &&	// ��ũȣ��
#endif // MODIFY_SKYLAND_FENRIR_AND_WING_BUGFIX_20080730
#ifdef ADD_THIRD_WING_20070525	// ��ī�罺�̵��� �ʿ� �����ۿ� 3������ �߰�
			(  lpObj->pInventory[EQUIPMENT_WING].IsItem() == FALSE ))	|| // ������ ����
#else
			(lpObj->pInventory[EQUIPMENT_WING].m_Type < MAKE_ITEMNUM(12,0) || lpObj->pInventory[EQUIPMENT_WING].m_Type > MAKE_ITEMNUM(12,6) )) ||
#endif
			lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) ||
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) ||
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) 
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ��ī�罺 �̵� �Ұ�
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 )
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) 	
#endif	
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 )
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 )
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
#ifdef MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 )
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 )
#endif // MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
			)
		{	// �䰡�þƳ� ������ �����ϰ� ���� ������ �̵��Ҽ� ����
			GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir  = lpObj->Dir;
			movefail = 1;
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = (BYTE)x;
			lpObj->RegenMapY = (BYTE)y;		

			//####### BUGFIX_CAT001 �� �̵� ���� ��, �þ� ó�� �ȵǴ� ���� �ذ�
			//gObjClearViewport(&gObj[aIndex]); // ���� �ִ� �� ������ �ּ� ó���ϰ�, 
			GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

			//�Ʒ� �Լ��� ȣ�� �ϵ��� �ٲ�			
			gObjClearViewportOfMine( lpObj );

			//BUGFIX_CAT001 ########

			if( lpObj->m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}

			gObj[aIndex].RegenOk   = 1;
			return FALSE;
		}

		if( lpObj->m_RecallMon >= 0 ) 
		{
			GCRecallMonLife(aIndex, 60, 0);
			gObjMonsterCallKill(aIndex);
		}
	}
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518					// Į�������� �ٸ� ������ �̵��� ���� ������ ���ش�.
	if( !CHECK_KALIMAMAP(mapNumber) &&
		CHECK_KALIMAMAP(lpObj->MapNumber)
		) 
	{
		// ������ ���ش�.
		INT iKalimaGateIndex = lpObj->m_iKalimaGateIndex;			// �α� ����� �� �ӽú���
		if (g_KalimaGate.DeleteKalimaGate(lpObj->m_Index)) {
			LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - User MoveGate (SummonIndex:%d, EnterCount:%d)", 
				lpObj->AccountID, 
				lpObj->Name,
				iKalimaGateIndex,
				gObj[iKalimaGateIndex].m_cKalimaGateEnterCount
				);
		}
	}
#endif

#ifdef ADD_KANTURU_CONDITION_20060725			
	if( mapNumber == MAP_INDEX_KANTURU_BOSS )
	{
		if( ( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3)			// ����Ʈ
#ifdef ADD_ITEM_FENRIR_01_20051110
			&& lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,37)			// �渱
#endif			
#ifdef MODIFY_KANTURU_ENTERANCE_DARKHORSE_BUGFIX_20080627
			&& lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,4)				// ��ũȣ��
#endif // MODIFY_KANTURU_ENTERANCE_DARKHORSE_BUGFIX_20080627
#ifdef ADD_THIRD_WING_20070525	// ĭ���� ������ �������ǿ� 3������ �߰�	
			&& ( lpObj->pInventory[EQUIPMENT_WING].IsItem() == FALSE ) )	||	// ������ ����.
#else
			) ||
#endif
			lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) ||		// ���Ź���
			lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10)			// ���Ź���
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ĭ������ ���� �Ұ�
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 )	
#endif
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 )
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 )
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
#ifdef MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 )
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 )
			|| lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 )
#endif // MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715
			)
		{
			// ���� ���н� ���� �޽��� ����
			GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);

			// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir  = lpObj->Dir;
			movefail = 1;
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = (BYTE)x;
			lpObj->RegenMapY = (BYTE)y;

			//####### BUGFIX_CAT001 �� �̵� ���� ��, �þ� ó�� �ȵǴ� ���� �ذ�
			//gObjClearViewport(&gObj[aIndex]); // ���� �ִ� �� ������ �ּ� ó���ϰ�, 
			GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

			//�Ʒ� �Լ��� ȣ�� �ϵ��� �ٲ�
			gObjClearViewportOfMine( lpObj );

			//BUGFIX_CAT001 ########

			if( lpObj->m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}

			gObj[aIndex].RegenOk   = 1;
			return FALSE;	

		}
	}
#endif	// ADD_KANTURU_CONDITION_20060725



#ifdef MODIFY_KANTURU_MAPNUMBER_CHECK_20060807
	if( g_iBlockKanturuMapEnter == 1 )
	{
		x			= lpObj->X;
		y			= lpObj->Y;
		mapNumber	= lpObj->MapNumber;
		dir			= lpObj->Dir;
		level		= lpObj->Level;

		if( gGateC.GetGate(gt, x, y, mapNumber, dir, level ) != -1 )
		{
			if( mapNumber >= MAP_INDEX_KANTURU1 && mapNumber <= MAP_INDEX_KANTURU_BOSS )
			{
				// ���� ���н� ���� �޽��� ����
				GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);

				// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;

				//####### BUGFIX_CAT001 �� �̵� ���� ��, �þ� ó�� �ȵǴ� ���� �ذ�
				//gObjClearViewport(&gObj[aIndex]); // ���� �ִ� �� ������ �ּ� ó���ϰ�, 
				GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				//�Ʒ� �Լ��� ȣ�� �ϵ��� �ٲ�
				gObjClearViewportOfMine( lpObj );

				//BUGFIX_CAT001 ########

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
	}

	if( g_iBlockCastleSiegeMapEnter == 1 )
	{
		x			= lpObj->X;
		y			= lpObj->Y;
		mapNumber	= lpObj->MapNumber;
		dir			= lpObj->Dir;
		level		= lpObj->Level;

		if( gGateC.GetGate(gt, x, y, mapNumber, dir, level ) != -1 )
		{
			if( mapNumber == MAP_INDEX_CASTLESIEGE
				|| mapNumber == MAP_INDEX_CASTLEHUNTZONE
				|| mapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE
				|| mapNumber == MAP_INDEX_CRYWOLF_SECONDZONE )
			{
				// ���� ���н� ���� �޽��� ����
				GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);

				// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;

				//####### BUGFIX_CAT001 �� �̵� ���� ��, �þ� ó�� �ȵǴ� ���� �ذ�
				//gObjClearViewport(&gObj[aIndex]); // ���� �ִ� �� ������ �ּ� ó���ϰ�, 
				GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				//�Ʒ� �Լ��� ȣ�� �ϵ��� �ٲ�
				gObjClearViewportOfMine( lpObj );

				//BUGFIX_CAT001 ########

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
		}
	}
#endif // MODIFY_KANTURU_MAPNUMBER_CHECK_20060807

#ifdef ADD_RAKLION_20080408
	if( mapNumber == MAP_INDEX_RAKLION_BOSS )
	{
		if( g_Raklion.GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR )
		{
			// ���� ���н� ���� �޽��� ����
			GCServerMsgStringSend(lMsg.Get(3415), lpObj->m_Index, 1);

			// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir  = lpObj->Dir;
			movefail = 1;
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = (BYTE)x;
			lpObj->RegenMapY = (BYTE)y;

			GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

			gObjClearViewportOfMine( lpObj );

			if( lpObj->m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}

			gObj[aIndex].RegenOk   = 1;
			return FALSE;
		}
		else
		{
			if( gt == 290 )
				g_RaklionBattleUserMng.AddUserData( aIndex );
		}
	}
	if( mapNumber == MAP_INDEX_RAKLION_FIELD )
	{
		if( gt == 292 )	// ������ -> ��Ŭ���� �ʵ� �̵���
		{
			if( g_Raklion.GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR )	// ������ ��� ����
			{
				// ���� ���н� ���� �޽��� ����
				GCServerMsgStringSend(lMsg.Get(3416), lpObj->m_Index, 1);

				// ���� ���и� ������ ���� ��ġ�� �״�� ���ش�.
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir  = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = (BYTE)x;
				lpObj->RegenMapY = (BYTE)y;

				GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

				gObjClearViewportOfMine( lpObj );

				if( lpObj->m_Change >= 0 )
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk   = 1;
				return FALSE;
			}
			else
			{
				g_RaklionBattleUserMng.DeleteUserData( aIndex );
			}
		}
		else
		{	// �ٸ��� -> ��Ŭ�������� ���Դ�. ��Ŭ���� �ʵ忡 ���� �������� ���� ������ ���¸� �����Ѵ�.
			g_RaklionUtil.NotifyRaklionCurrentState( aIndex, g_Raklion.GetRaklionState(),
				g_Raklion.GetRaklionStateDetail() );
		}
	}
#endif // ADD_RAKLION_20080408


#ifdef DARKLORD_WORK
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108	// ����� ���� ���˻�, ��ũ�ε�� Movereq.txt ������ ���� ����
	if( mapNumber != MAP_INDEX_SWAMP_OF_CALMNESS )
#endif	// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108	
		if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )
#else
	if( lpObj->Class == CLASS_MAGUMSA )
#endif
	{
		if( level > 0 )
		{
			level = ((level/3)*2);
		}
	}	

	if( (level != 0) && ( userlevel < level) ) {
		char szTemp[256];
		wsprintf(szTemp, lMsg.Get(1117), level);
		GCServerMsgStringSend(szTemp, aIndex, 1);
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir  = lpObj->Dir;
		movefail = 1;
	}

	if( gObj[aIndex].m_IfState.use )
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir  = lpObj->Dir;
		movefail = 1;
		//LogAddL("�� �̵��� �������̽� ��������� �̵� ��ҵ�");
	}
	if( gObj[aIndex].DieRegen ) 
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir  = lpObj->Dir;
		movefail = 1;
		//LogAddL("�׾ �̵� %d", mapNumber);
	}

	lpObj->m_State = OBJST_DELCMD;
	if( movefail == 0 )
	{
#ifdef MAP_SERVER_WORK_20041030		// �ʼ����� �̵����� üũ
		if (gGateC.GetGate(gt, x, y, mapNumber, dir, level) != -1) {
			if (gObj[aIndex].MapNumber != mapNumber) {
				// �ٸ� ������ �̵��ϰ��� �� ���� �ʼ��� �̵������� üũ�Ѵ�.
				if( !MapNumberCheck(mapNumber) ) 
				{
					LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
					return FALSE;
				}

				SHORT sSvrCode	= g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, mapNumber, lpObj->m_sPrevMapSvrCode);
				if (sSvrCode != gGameServerCode) {
					if (sSvrCode == -1) {
						// ġ������ ���� -> ���� ����
						LogAddC(LOGC_RED, "[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",
							lpObj->AccountID,
							lpObj->Name,
							lpObj->m_Index
							);
						return FALSE;
					}

					// ������� ������ �̵��� �������� ���θ� üũ�Ѵ�. (b4nfter)
					if (gGateC.CheckGateLevel(aIndex, gt) == FALSE) {
						// ������ ���Ƽ� �̵� �Ұ���
						x = lpObj->X;
						y = lpObj->Y;
						mapNumber = lpObj->MapNumber;
						dir  = lpObj->Dir;
						movefail = 1;
						lpObj->RegenMapNumber = lpObj->MapNumber;
						lpObj->RegenMapX = (BYTE)x;
						lpObj->RegenMapY = (BYTE)y;		
						gObjClearViewport(&gObj[aIndex]);
						GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

						if( lpObj->m_Change >= 0 )
						{
							gObjViewportListProtocolCreate(&gObj[aIndex]);
						}

						gObj[aIndex].RegenOk   = 1;

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �����ʿ����� �ڷ���Ʈ ���Ŀ� �ڽ��� �������� ���¸� �ٽ� ������
						if (gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE &&
							g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
							)
						{
							g_CastleSiege.NotifySelfCsJoinSide (aIndex);
							g_CastleSiege.NotifyCsSelfLeftTime (aIndex);
						}
#endif
						return FALSE;
					}

					// ���������� �ʼ��� �̵� ��û�� �Ѵ�.
					GJReqMapSvrMove (lpObj->m_Index, sSvrCode, mapNumber, x, y);
					LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",
						sSvrCode,
						lpObj->AccountID,
						lpObj->Name,
						lpObj->m_Index
						);
					return FALSE;
				}
			}
		}
#endif

#ifdef EXTEND_LOG_SYSTEM_20060202
		LogAddTD("[MoveMap][%s][%s] : (Before:Map:%d X:%d Y:%d) -> (New Map:%d X:%d Y:%d)",
			lpObj->AccountID,	lpObj->Name,
			lpObj->MapNumber,	lpObj->X,	lpObj->Y, 
			mapNumber, x, y );
#endif

		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;
		gObj[aIndex].TX = x;
#ifdef MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObj[aIndex].TY = y;
#else	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObj[aIndex].TX = y;
#endif	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObj[aIndex].MapNumber = mapNumber;
		gObj[aIndex].Dir = dir; 
		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport  = 0;

#ifdef MODIFY_AFTERIMAGE_BUGFIX_20060215
		// ���� �̺κ� �ּ�ó�� �Ǿ� �־���.
		// �������� : Map �̵��� ����Ʈ ����Ʈ ���������� ����
		gObjViewportListProtocolDestroy(&gObj[aIndex]);
#endif

#ifdef MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObjViewportListProtocolDestroy(&gObj[aIndex]);
		//gObjViewportListProtocolCreate(&gObj[aIndex]);
#endif	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024

#ifndef MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObjClearViewport(&gObj[aIndex]);
#endif	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

#ifdef MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024
		gObjViewportListProtocolCreate(&gObj[aIndex]);
		gObjClearViewport(&gObj[aIndex]);
#endif	// MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024

		if( lpObj->m_Change >= 0 )
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenMapNumber = mapNumber;
		gObj[aIndex].RegenMapX = (BYTE)x;
		gObj[aIndex].RegenMapY = (BYTE)y;		
		gObj[aIndex].RegenOk   = 1;
	}
	else
	{
		GCTeleportSend(&gObj[aIndex], gt, mapNumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);
		gObj[aIndex].RegenOk   = 1;


#ifdef CASTLE_MAIN_SCHEDULER_20041111						// �����ʿ����� �ڷ���Ʈ ���Ŀ� �ڽ��� �������� ���¸� �ٽ� ������
		if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE &&
			g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
			)
		{
			g_CastleSiege.NotifySelfCsJoinSide (aIndex);
			g_CastleSiege.NotifyCsSelfLeftTime (aIndex);
		}
#endif
	}
	//LogAddL("�̵� �޽��� �Ϸ�");

	if( movefail )
		return FALSE;

	if( gObj[aIndex].Type == OBJTYPE_CHARACTER )
	{
		gObj[aIndex].m_LastTeleportTime = 10;				// �̵��� ����, ���� �ð� ��������� ���� �ڷ� �ȹи��� �����ϱ� ���� ��
	}

#ifdef ADD_NEWPVP_PKFIELD
	g_NewPVP.Reset(*lpObj);
#endif // ADD_NEWPVP_PKFIELD

	return TRUE;
}

// ������ map, x, y ��ǥ�� �̵� ��Ų��.
void gObjTeleport(int aIndex, int map, int x, int y)
{
	if( aIndex < 0 || aIndex > MAX_OBJECT-1 ) return;
	
	LPOBJECTSTRUCT lpObj;
	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( !MapNumberCheck(map) ) 
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		return;
	}

#ifdef MAP_SERVER_WORK_20041030		// �ʼ����� �̵����� üũ

	if (gObj[aIndex].MapNumber != map) {
		// �ٸ� ������ �̵��ϰ��� �� ���� �ʼ��� �̵������� üũ�Ѵ�.
		SHORT sSvrCode	= g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, map, lpObj->m_sPrevMapSvrCode);
		if (sSvrCode != gGameServerCode) {
			if (sSvrCode == -1) {
				// ġ������ ���� -> ���� ����
				LogAddC(LOGC_RED, "[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",
					lpObj->AccountID,
					lpObj->Name,
					lpObj->m_Index
					);
				return;
			}

			// ���������� �ʼ��� �̵� ��û�� �Ѵ�.
			GJReqMapSvrMove (lpObj->m_Index, sSvrCode, map, x, y);
			LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",
				sSvrCode,
				lpObj->AccountID,
				lpObj->Name,
				lpObj->m_Index
				);
				return;
		}
	}
	
#endif

	if( lpObj->m_IfState.use==1 && lpObj->m_IfState.type == I_TRADE)
	{
		CGTradeCancelButtonRecv(lpObj->m_Index);
		//LogAddL("�ŷ� ĵ��");
	}
	if( lpObj->m_IfState.use==1 && lpObj->m_IfState.type == I_WAREHOUSE)
	{
		CGWarehouseUseEnd(lpObj->m_Index);
		//LogAddL("â�� ĵ��");
	}
	//if( lpObj->m_IfState.use==1 && lpObj->m_IfState.type == I_GUILDCREATE )
	//if( lpObj->m_IfState.use==1 && lpObj->m_IfState.type == I_CHAOSBOX )
	//{
	//}
	
	//gObjViewportListProtocolDestroy(&gObj[aIndex]);
	lpObj->m_State = OBJST_DELCMD;
	gObj[aIndex].X = x;
	gObj[aIndex].Y = y;
	gObj[aIndex].TX = x;
#ifdef MODIFY_TELEPORT_TARGETPOSITION_BUGFIX
	gObj[aIndex].TY = y;
#else
	gObj[aIndex].TX = y;
#endif // MODIFY_TELEPORT_TARGETPOSITION_BUGFIX
	gObj[aIndex].MapNumber = map;
	gObj[aIndex].PathCount = 0;
	gObj[aIndex].Teleport  = 0;

	
	//gObjViewportListProtocolDestroy(&gObj[aIndex]);		
	//gObjClearViewport(&gObj[aIndex]);
	//gObjViewportClose(&gObj[aIndex]);
	gObjClearViewport(&gObj[aIndex]);
	GCTeleportSend(&gObj[aIndex], 1, map, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);
	
	if( lpObj->m_Change >= 0 )
	{
		gObjViewportListProtocolCreate(&gObj[aIndex]);
	}
	gObj[aIndex].RegenMapNumber = map;
	gObj[aIndex].RegenMapX = (BYTE)x;
	gObj[aIndex].RegenMapY = (BYTE)y;		
	gObj[aIndex].RegenOk   = 1;
	//LogAddC(LOGC_RED,"%s �̵� ����", gObj[aIndex].Name);

	//char szTemp[256];
	//wsprintf(szTemp, "%d %d �� �̵� ��Ų��.", x, y);
	//GCServerMsgStringSend(szTemp, aIndex, 1);
}





//----------------------------------------------------------------------------
//[0xF3][0x12] �� �̵��� ������ �ε��� ���ļ� ����Ʈ ����Ʈ�� ��� �϶�� �˸���.
void gObjMoveDataLoadingOK(int aIndex)
{
	if( gObj[aIndex].RegenOk == 1 )
	{
		gObj[aIndex].RegenOk = 2;
	}
}

// ������ ������ ����ϰ��� �ϴ� ������ ��´�.
CMagicInf *gObjGetMagic(LPOBJECTSTRUCT lpObj, int mIndex)
{
	if( mIndex < 0 || mIndex > MAX_MAGIC-1 )
	{
		LogAdd(lMsg.Get(546), __FILE__, __LINE__);
		return NULL;
	}
	
//	if( lpObj->Type != OBJTYPE_CHARACTER ) return NULL;

	/*
	if( lpObj->m_Change >= 0 )
	{
		if( lpObj->m_Change == 9 && mIndex == 2 )	
		{
			return &DefMagicInf[3];
		}
	}
	*/
	if( lpObj->Magic[mIndex].IsMagic() == FALSE ) return NULL;
	
	/*reqeng = MagicDamageC.SkillGetRequireEnergy(lpObj->Magic[mIndex].m_Skill);
	if( reqeng < 0 ) return NULL;
	if( lpObj->Energy < reqeng ) return NULL;
	*/	
	return (CMagicInf*)(&lpObj->Magic[mIndex]);
}

// ������ ������ ����ϰ��� �ϴ� ������ ��´�.
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
CMagicInf *gObjGetMagicSearch(LPOBJECTSTRUCT lpObj, int skillnumber)
#else
CMagicInf *gObjGetMagicSearch(LPOBJECTSTRUCT lpObj, BYTE skillnumber)
#endif
{
	/*int reqeng = MagicDamageC.SkillGetRequireEnergy(skillnumber);
	if( reqeng < 0 ) return NULL;

	if( lpObj->Energy < reqeng ) return NULL;
	*/
#ifdef ADD_SKILL_WITH_COMBO
	if( skillnumber == AT_SKILL_EXPHELL_START )
	{
		skillnumber = AT_SKILL_EXPHELL;
	}
#endif

	for( int n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == TRUE )
		{
			if( lpObj->Magic[n].m_Skill == skillnumber )
			{
				if( !gObjMagicEnergyCheck(lpObj, lpObj->Magic[n].m_Skill) )
				{					
					GCServerMsgStringSend(lMsg.Get(1255), lpObj->m_Index, 1);	// ��ų�� ����Ҽ� �����ϴ�
					return NULL;
				}
				return (CMagicInf*)&lpObj->Magic[n];
			}
		}
	}	
	return NULL;
}


// ������ ����Ѵ�. 
int gObjMagicManaUse(LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic)
{
	float mana = lpObj->Mana;
	
	// ���ʹ� MP�� ���� �ʴ´�.
	if( lpObj->Type != OBJTYPE_CHARACTER ) return 0;
	if( lpMagic == NULL ) return FALSE;

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// ���� ���� ��ȭ
	float DecreaseMana = 0.0f;
	DecreaseMana = MagicDamageC.SkillGetMana(lpMagic->m_Skill);
	DecreaseMana -= (DecreaseMana * lpObj->m_MPSkillOpt.iMpsDecreaseMana) / 100;

	if(DecreaseMana < 1.0f)	DecreaseMana = 1.0f;
	mana -= DecreaseMana;
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	mana -= MagicDamageC.SkillGetMana(lpMagic->m_Skill);
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
	
	// ������ ���� ����� �� ���ٸ� �׳� ����
	if( mana < 0 ) return -1;

	return (int)mana;
}

#ifdef NEW_FORSKYLAND2
// AG�Ŀ��� ����Ѵ�
int gObjMagicBPUse(LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic)
{	
	int bp = lpObj->BP;
	
	// ���ʹ� MP�� ���� �ʴ´�.
	if( lpObj->Type != OBJTYPE_CHARACTER ) return 0;
	if( lpMagic == NULL ) return FALSE;

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	int iUsedBP = MagicDamageC.SkillGetBP(lpMagic->m_Skill);
	if( lpObj->m_btAGReduceRate > 0 )
	{
		iUsedBP -= ( iUsedBP * lpObj->m_btAGReduceRate ) / 100;
	}

	bp -= iUsedBP;
#else
	bp -= MagicDamageC.SkillGetBP(lpMagic->m_Skill);
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	
	// ������ ���� ����� �� ���ٸ� �׳� ����
	if( bp < 0 ) return -1;

	return (int)bp;
}
#endif

// Ư�� ��ġ�� ���� ������ �ߴ�.
BOOL gObjPosMagicAttack(LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic, BYTE x, BYTE y)
{
	int tObjNum;
	int dis;
	int tx, ty;
	int MagicDistance=3;
	BYTE sbuf[256];
	int lOfs=0;
	BYTE MagicNumber = lpMagic->m_Skill;

	//��ġ ������ ���� ������ �����Ѵ�.
	switch( MagicNumber )
	{
	case AT_SKILL_EVIL : MagicDistance = 3; break;
	default : MagicDistance = 3; break;
	}

	PMSG_POSMAGIC_COUNT		pCount;
	PMSG_POSMAGIC_RESULT	pResult;

	lOfs = sizeof(pCount);

	pCount.h.c          = PMHC_BYTE;
	pCount.h.headcode   = 0x1A;
	pCount.h.size       = 0;
	pCount.MapX			= x;
	pCount.MapY			= y;
	pCount.MagicNumber	= MagicNumber;
	pCount.NumberH		= HIBYTE(lpObj->m_Index);
	pCount.NumberL		= LOBYTE(lpObj->m_Index);
	pCount.Count		= 0;

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

	for(int n=0; n<MVL; n++)
	{
		if( lpObj->VpPlayer[n].state )
		{
			tObjNum = lpObj->VpPlayer[n].number;
			if( tObjNum >= 0 )
			{
				if( gObj[tObjNum].Live )	// ������ �Ҵ�Ǿ� �ִٸ�..
				{
					tx = lpObj->X-gObj[tObjNum].X;
					ty = lpObj->Y-gObj[tObjNum].Y;
					dis = (int)sqrt((double)(tx*tx)+(ty*ty));
					
					if( dis <= MagicDistance )	// ���� ��ġ�� �����ϸ�
					{
						if( gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 0) == TRUE)
						{
							pResult.NumberH = HIBYTE(tObjNum);
							pResult.NumberL = LOBYTE(tObjNum);
							memcpy(sbuf+lOfs, &pResult, sizeof(pResult));
							pCount.Count++;
							lOfs += sizeof(pResult);
							if( lOfs > 250 ) 
							{
								LogAdd(lMsg.Get(547), __FILE__, __LINE__);
								return FALSE;
							}
						}
					}
				}
			}
		}
	}
	pCount.h.size = (BYTE)lOfs;
	memcpy(sbuf, &pCount, sizeof(pCount));	

	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(lpObj->m_Index, sbuf, lOfs);

	MsgSendV2(lpObj, sbuf, lOfs);
	/*
	for( n=0; n<MAXVIEWPORTOBJECT; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_CHARACTER )
		{
			if( lpObj->VpPlayer2[n].state != OBJST_NONE &&
				lpObj->VpPlayer[n].state != OBJST_DIED &&
				lpObj->VpPlayer[n].state != OBJST_DIECMD )
			{
				DataSend(lpObj->VpPlayer2[n].number, (char*)sbuf, lOfs);
			}
		}
	}
	*/
	return TRUE;
}


int g_EnergyCheckOff=1;

void gObjMagicAddEnergyCheckOnOff(int flag)
{
	g_EnergyCheckOff = flag;
}

// ������ �߰��Ѵ�.
int gObjMagicAdd(LPOBJECTSTRUCT lpObj, BYTE Type, BYTE Index, BYTE Level, BYTE & SkillNumber)
{
	int skill=-1;
	int n;
		
	int reqeng;
	
	skill = GetSkillNumberInex(Type, Index, Level);
	if( skill < 0 ) {
		LogAdd(lMsg.Get(548));
		return -1;
	}
	if( g_EnergyCheckOff == TRUE )
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy(skill);
		if( reqeng < 0 ) 
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}

#ifdef MODIFY_SKILL_REQUIREENERGY_20080423
	if (lpObj->Class == CLASS_KNIGHT)	reqeng -= 10;
	if (lpObj->Class == CLASS_ELF)		reqeng -= 5;
#endif // MODIFY_SKILL_REQUIREENERGY_20080423
	
		if( lpObj->Energy+lpObj->AddEnergy < reqeng ) 
		{
			
			LogAdd(lMsg.Get(549));
			return -1;
		}		
	}
	
#ifdef DARKLORD_WORK
		int reqleadership = MagicDamageC.GetskillRequireLeadership(skill);
		if( reqleadership < 0 ) 
		{
			LogAdd("error-L2: Skill Leadership not enough");
			return -1;
		}

		if( (lpObj->Leadership+lpObj->AddLeadership) < reqleadership ) 
		{			
			LogAdd("error-L2: Skill Leadership not enough");
			return -1;
		}		
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3������ �߰�
	if( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpObj->ThirdChangeUp, skill) < 1 )
#else
#ifdef NEW_SKILL_FORSKYLAND
	if( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, skill) < 1 ) 
#else
	if( MagicDamageC.SkillGetRequireClass(lpObj->Class, skill) < 1 ) 
#endif
#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122
	{
		LogAdd(lMsg.Get(550));
		return -1;
	}	

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ȭ�� ��ų�� �⺻��ų�� ��� �� ����.
	if( g_MasterSkillSystem.IsExistMLSBasedSkill( lpObj, skill ) == TRUE )
	{		
		return -1;
	}
#endif

	// ���� ������ �ִ��� �˻��ؼ� ���� ������ ����� �ȵǰ� �Ѵ�.
	for( n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == TRUE )
		{
			skill = GetSkillNumberInex(Type, Index, Level);
			if( skill < 0 ) 
			{
				LogAdd(lMsg.Get(551),__FILE__, __LINE__);
				return -1;
			}
			else {
				if( lpObj->Magic[n].m_Skill == skill )
				{
					LogAdd(lMsg.Get(552));
					return -1;
				}
			}
		}
	}
	skill = -1;
	for( n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == FALSE )
		{
			skill = lpObj->Magic[n].Set(Type, Index, Level);
			if( skill < 0 ) 
			{
				LogAdd(lMsg.Get(553),__FILE__, __LINE__);
				return -1;
			}			
			lpObj->MagicCount++;
			SkillNumber = (BYTE)skill;
			return n;
		}
	}

	LogAdd(lMsg.Get(554), __FILE__, __LINE__);
	return -1;
}

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122		// gObjMagicAdd - Skill Type ����
int gObjMagicDel(LPOBJECTSTRUCT lpObj, int aSkill, BYTE Level)
#else
int gObjMagicDel(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level)
#endif
{
#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514
	//////////////////////////////////////////////////////////////////////////
	// ��ȹ ������ �������� �Ʒ��� ó���� ���־� �Ѵ�.
	//////////////////////////////////////////////////////////////////////////
#else // MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514
	#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
	if( g_MasterSkillSystem.MLS_WeaponSkillDel(lpObj, aSkill, Level) )
	{
		return -1;
	}
	#endif	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
#endif // MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514
	
	int n;

	for( n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == TRUE )
		{
			if( lpObj->Magic[n].m_Skill == aSkill )
			{
				lpObj->Magic[n].Clear();				
				//LogAdd("��ų����");
				return n;
			}
		}
	}
	return -1;
}

// ������ �߰��Ѵ�.
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// gObjMagicAdd - Skill Type ����
int gObjMagicAdd(LPOBJECTSTRUCT lpObj, INT aSkill, BYTE Level)
#else
int gObjMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level)
#endif
{
	int skill=-1;
	int n;

	skill = -1;

	int reqeng;

	if( g_EnergyCheckOff == TRUE )
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);
		if( reqeng < 0 ) 
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}

#ifdef MODIFY_SKILL_REQUIREENERGY_20080423
	if (lpObj->Class == CLASS_KNIGHT)	reqeng -= 10;
	if (lpObj->Class == CLASS_ELF)		reqeng -= 5;
#endif // MODIFY_SKILL_REQUIREENERGY_20080423

		if( (lpObj->Energy+lpObj->AddEnergy) < reqeng ) 
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}
	}
	
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3������ �߰�
	if( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpObj->ThirdChangeUp, aSkill) < 1 )
#else
#ifdef NEW_SKILL_FORSKYLAND
	if( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, aSkill) < 1 ) 
#else
	if( MagicDamageC.SkillGetRequireClass(lpObj->Class, aSkill) < 1 ) 
#endif
#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122
	{
		LogAdd(lMsg.Get(550));
		return -1;
	}

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ȭ�� ��ų�� �⺻��ų�� ��� �� ����.
	if( g_MasterSkillSystem.IsExistMLSBasedSkill( lpObj, skill ) == TRUE )
	{		
		return -1;
	}
#endif

	// ���� ������ �ִ��� �˻��ؼ� ���� ������ ����� �ȵǰ� �Ѵ�.
	for( n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == TRUE )
		{			
			if( lpObj->Magic[n].m_Skill == aSkill)
			{
				LogAdd(lMsg.Get(555), lpObj->Magic[n].m_Skill , aSkill);
				return -1;
			}
		}
	}
	for( n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == FALSE )
		{
			skill = lpObj->Magic[n].Set(aSkill, Level);
			if( skill < 0 ) 
			{
				LogAdd(lMsg.Get(553), __FILE__, __LINE__);
				return -1;
			}			
			lpObj->MagicCount++;
			return n;
		}
	}

	LogAdd(lMsg.Get(554), __FILE__, __LINE__);
	return -1;
}

// ������ �߰��Ѵ�.
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
int gObjWeaponMagicAdd(LPOBJECTSTRUCT lpObj, WORD aSkill, BYTE Level)
#else
int gObjWeaponMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level)
#endif
{
	int skill=-1;
	int n;

	skill = -1;

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3������ �߰�
	if( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpObj->ThirdChangeUp, aSkill) < 1 ) return -1;
#else
#ifdef NEW_SKILL_FORSKYLAND
	if( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, aSkill) < 1 ) return -1;
#else
	if( MagicDamageC.SkillGetRequireClass(lpObj->Class, aSkill) < 1 ) return -1;
#endif
#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122

#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514
	//////////////////////////////////////////////////////////////////////////
	// ��ȹ ������ �������� �Ʒ��� ó���� ���־� �Ѵ�.
	//////////////////////////////////////////////////////////////////////////
#else // MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514
	#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
	if( g_MasterSkillSystem.MLS_WeaponSkillAdd(lpObj, aSkill, Level) )
	{
		return -1;
	}
	#else	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
	#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ���⽺ų �߰� ��������
	// ���⽺ų �� �����ͷ�����ų��ȭ�� ����� ��ų�� �⺻ ��ų�� �߰��Ǹ� �ȵȴ�.
	// ���⿡ ������ �����ϴ� ���� �ƴ϶� ���� �����ִ� ��ų ����(��������Ʈ)�� �Ǵ��Ѵ�.
	if( g_MasterSkillSystem.IsExistMLSBasedSkill( lpObj, aSkill ) )
		return -1;
	#endif
	#endif	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
#endif // MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514

	for( n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == FALSE )
		{
			skill = lpObj->Magic[n].Set(aSkill, Level);
			if( skill < 0 ) 
			{
				LogAdd(lMsg.Get(553),__FILE__, __LINE__);
				return -1;
			}			
			lpObj->MagicCount++;
			return n;
		}
	}
	LogAdd(lMsg.Get(554),__FILE__, __LINE__);
	return -1;
}

// ������ �߰��Ѵ�.
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
int gObjMonsterMagicAdd(LPOBJECTSTRUCT lpObj, WORD aSkill, BYTE Level)
#else
int gObjMonsterMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level)
#endif
{
	int skill=-1;
	int n;

	for( n=0; n<MAX_MAGIC; n++)
	{
		if( lpObj->Magic[n].IsMagic() == FALSE )
		{
			skill = lpObj->Magic[n].Set(aSkill, Level);
			if( skill < 0 ) 
			{
				LogAdd(lMsg.Get(553),__FILE__, __LINE__);
				return -1;
			}			
			lpObj->MagicCount++;
			return n;
		}
	}
	LogAdd(lMsg.Get(554),__FILE__, __LINE__);
	return -1;
}


// ������ ����Ҷ� ������ üũ�� �Ѵ�
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
BOOL	gObjMagicEnergyCheck(LPOBJECTSTRUCT lpObj, WORD aSkill)
#else
BOOL	gObjMagicEnergyCheck(LPOBJECTSTRUCT lpObj, BYTE aSkill)
#endif
{
	int reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);
	if( reqeng < 0 ) 
	{	
		return FALSE;
	}

#ifdef MODIFY_SKILL_REQUIREENERGY_20080423
	if (lpObj->Class == CLASS_KNIGHT)	reqeng -= 10;
	if (lpObj->Class == CLASS_ELF)		reqeng -= 5;
#endif // MODIFY_SKILL_REQUIREENERGY_20080423	

	if( (lpObj->Energy+lpObj->AddEnergy) < reqeng ) 
	{
		return FALSE;
	}

#ifdef ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
	int reqlevel = MagicDamageC.SkillGetRequireLevel(aSkill);
	if( (lpObj->Level) < reqlevel && g_GlobalConfig.m_bCheckSkill == TRUE ) 
	{
		return FALSE;
	}
#endif
	
	return TRUE;
}


// ����ڰ� �ູ�� ������ ����Ͽ� �������� ������ �Ѵܰ� �÷ȴ�.
BOOL gObjItemLevelUp(LPOBJECTSTRUCT lpObj, int source, int target)
{
	if( source < 0 || source > MAX_INVENTORY-1 ) return FALSE;
	if( target < 0 || target > MAX_INVENTORY-1 ) return FALSE;

	// �������� �ִ��� üũ
	if( lpObj->pInventory[source].IsItem() == FALSE ) return FALSE;
	if( lpObj->pInventory[target].IsItem() == FALSE ) return FALSE;

#ifdef ADD_ITEM_FENRIR_01_20051110
	// �渱�� �ູ�� ������ ����Ѵٸ�, �渱 ������ �����Ѵ�.
	if( lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(13,37) )
	{
		CItem &ItemFenrir = lpObj->pInventory[target];
		
		if( ItemFenrir.m_Durability >= 255 )
			return FALSE;

		if( rand()%10000 < g_iFenrirRepairRate )
		{
			int iAddDur = 50 + rand()%150;

			if( iAddDur + ItemFenrir.m_Durability > 255 )
				ItemFenrir.m_Durability = 255;
			else
				ItemFenrir.m_Durability += iAddDur;

			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			MsgOutput( lpObj->m_Index, lMsg.Get(3342) , (int)ItemFenrir.m_Durability );			// "�渱�� �������� %d �� �Ǿ����ϴ�."
#ifdef EXTEND_LOG_SYSTEM_20060202
			LogAddTD("[FENRIR REPAIR][SUCCESS] [%s][%s] - %d/255 (+%d)",
				lpObj->AccountID, lpObj->Name, ItemFenrir.m_Durability, iAddDur);
#endif
		}
		else
		{
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			MsgOutput( lpObj->m_Index, lMsg.Get(3343) );			// "�渱�� ������ ȸ���� ���� �߽��ϴ�."
#ifdef EXTEND_LOG_SYSTEM_20060202
			LogAddTD("[FENRIR REPAIR][FAILED] [%s][%s] - %d/255 (+%d)",
				lpObj->AccountID, lpObj->Name, ItemFenrir.m_Durability);
#endif
		}
		
		return TRUE;
	}
#endif // ADD_ITEM_FENRIR_01_20051110

#ifdef ADD_THIRD_WING_20070525	// 3������ �ູ�Ǻ������� ������ ����
	if( ( lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM( 12, 7 ) && lpObj->pInventory[target].m_Type < MAKE_ITEMNUM( 12, 36 ) ) || 
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1������ �ູ�� �������� ������ ����
		( lpObj->pInventory[target].m_Type > MAKE_ITEMNUM( 12, 43 ) && lpObj->pInventory[target].m_Type != MAKE_ITEMNUM( 13, 30 ) ) || 		
#else	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912		
		( lpObj->pInventory[target].m_Type > MAKE_ITEMNUM( 12, 40 ) && lpObj->pInventory[target].m_Type != MAKE_ITEMNUM( 13, 30 ) ) || 		
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 4, 7 ) ||
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 4, 15 ) )
		return FALSE;			// ������ �Ҽ� ���� �����۵�
#else
#ifdef DARKLORD_WORK
	if( (lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM(12,7) && lpObj->pInventory[target].m_Type != MAKE_ITEMNUM(13,30)) || 
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 7) ||
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 15) )
		return FALSE;			// ������ �Ҽ� ���� �����۵�
#else
	if( lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM(12,7) || 
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 7) ||
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 15) )
		return FALSE;			// ������ �Ҽ� ���� �����۵�
#endif
#endif	// ADD_THIRD_WING_20070525	

	LogAddTD(lMsg.Get(556), lpObj->AccountID, lpObj->Name, lpObj->pInventory[target].GetName(), lpObj->pInventory[target].m_Number, lpObj->pInventory[target].m_Level);

	lpObj->pInventory[target].m_Level+=1;
	if( lpObj->pInventory[target].m_Level > 6 ) 
	{
		lpObj->pInventory[target].m_Level = 6;
	}
	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur =  (float)ItemGetDurability(lpObj->pInventory[target].m_Type, 
												   lpObj->pInventory[target].m_Level, 
												   lpObj->pInventory[target].IsExtItem(),
												   lpObj->pInventory[target].IsSetItem());
	
	lpObj->pInventory[target].m_Durability = levelitemdur*lpObj->pInventory[target].m_Durability/lpObj->pInventory[target].m_BaseDurability;
	// ������ ������ �缳���Ѵ�	

	#ifdef ADD_380ITEM_NEWOPTION_20060711
		lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type, 
										  lpObj->pInventory[target].m_Option1, 
										  lpObj->pInventory[target].m_Option2,
										  lpObj->pInventory[target].m_Option3,
										  lpObj->pInventory[target].m_NewOption,
										  lpObj->pInventory[target].m_SetOption,
										  lpObj->pInventory[target].m_ItemOptionEx);
	#else
		lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type, 
										  lpObj->pInventory[target].m_Option1, 
										  lpObj->pInventory[target].m_Option2,
										  lpObj->pInventory[target].m_Option3,
										  lpObj->pInventory[target].m_NewOption,
										  lpObj->pInventory[target].m_SetOption);
	#endif // ADD_380ITEM_NEWOPTION_20060711

	LogAddTD(lMsg.Get(557),
			 lpObj->AccountID, 
			 lpObj->Name, 
			 lpObj->pInventory[source].m_Number,
			 lpObj->pInventory[target].GetName(), 
			 lpObj->pInventory[target].m_Number, 
			 lpObj->pInventory[target].m_Level);
	return TRUE;
}

// ����ڰ� ������ ����Ͽ� �������� ������ �����ϰ� �÷ȴ�.
BOOL gObjItemRandomLevelUp(LPOBJECTSTRUCT lpObj, int source, int target)
{
	if( source < 0 || source > MAX_INVENTORY-1 ) return FALSE;
	if( target < 0 || target > MAX_INVENTORY-1 ) return FALSE;

	// �������� �ִ��� üũ
	if( lpObj->pInventory[source].IsItem() == FALSE ) return FALSE;
	if( lpObj->pInventory[target].IsItem() == FALSE ) return FALSE;

#ifdef ADD_THIRD_WING_20070525	// 3������ ��ȥ�Ǻ������� ������ ����
	if( ( lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM( 12, 7 ) && lpObj->pInventory[target].m_Type < MAKE_ITEMNUM( 12, 36 ) ) || 
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1������ �ູ�� �������� ������ ����
		( lpObj->pInventory[target].m_Type > MAKE_ITEMNUM( 12, 43 ) && lpObj->pInventory[target].m_Type != MAKE_ITEMNUM( 13, 30 ) ) || 		
#else	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		( lpObj->pInventory[target].m_Type > MAKE_ITEMNUM( 12, 40 ) && lpObj->pInventory[target].m_Type != MAKE_ITEMNUM( 13, 30 ) ) || 		
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 4, 7 ) ||
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 4, 15 ) )
		return FALSE;			// ������ �Ҽ� ���� �����۵�
#else
#ifdef DARKLORD_WORK
	if( (lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM(12,7) && lpObj->pInventory[target].m_Type != MAKE_ITEMNUM(13,30)) || 
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 7) ||
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 15) )
		return FALSE;			// ������ �Ҽ� ���� �����۵�
#else
	if( lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM(12,7) || 
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 7) ||
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 15) )
		return FALSE;			// ������ �Ҽ� ���� �����۵�
#endif
#endif	// ADD_THIRD_WING_20070525

	// ������ +9�̻��̸� �ȹ߶�����
	if( lpObj->pInventory[target].m_Level >= 9 ) 
	{
		return FALSE;
	}

	LogAddTD(lMsg.Get(558), lpObj->AccountID, lpObj->Name, lpObj->pInventory[target].GetName(), lpObj->pInventory[target].m_Number, lpObj->pInventory[target].m_Level);
	
	int _r=rand()%100;
	
#ifdef MODIFY_ITEM_UPGRADE_MIX_RATE_FIX_20080627
	bool bSuccessUpgrade = false;

	if( lpObj->pInventory[target].m_Option2 )
	{
		// ��� �ɼ��� �ִ°�� ����Ȯ�� 20% ����
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
		_r += g_GlobalConfig.m_iSoulRate-g_GlobalConfig.m_iSoulRateWithLuck;
#else
		_r += g_GlobalConfig.m_iSoulRate-g_GlobalConfig.m_iSoulRateWithLuck;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
	}

	// ������ ����� ��� ������ ��޿� ���� ����� Ȯ���� �����Ѵ�.
	// ������Ʈ, ��Ʈ, 380 �������� ��� 50%Ȯ�� ����
	if( lpObj->pInventory[target].IsExtItem() == TRUE
		|| lpObj->pInventory[target].IsSetItem() == TRUE
		|| g_kItemSystemFor380.Is380OptionItem( &lpObj->pInventory[target] ) == TRUE 
	  )
	{
		if( _r < g_GlobalConfig.m_iSoulRate+10 )
		{
			bSuccessUpgrade = true;
		}
	}
	// ���� �������� ��� 40% Ȯ�� ����
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	else if( g_SocketOptionSystem.IsSocketItem( &lpObj->pInventory[target] ) == true )
#else
	else if( g_SocketOptionSystem.IsEnableSocketItem( &lpObj->pInventory[target] ) == true )
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	{
		if( _r < g_GlobalConfig.m_iSoulRate )
		{
			bSuccessUpgrade = true;
		}
	}
	// �Ϲ� �������� ��� 60% Ȯ�� ����
	else
	{
		if( _r < g_GlobalConfig.m_iSoulRate+20 )
		{
			bSuccessUpgrade = true;
		}
	}

	if( bSuccessUpgrade == true )
	{
		lpObj->pInventory[target].m_Level++;
	}
	else
	{
		if( lpObj->pInventory[target].m_Level >= 7 )
		{
			lpObj->pInventory[target].m_Level = 0;
		}
		else
		{
			lpObj->pInventory[target].m_Level--;
			if( lpObj->pInventory[target].m_Level < 0 )
			{
				lpObj->pInventory[target].m_Level = 0;
			}
		}
	}
#else
	
	if( lpObj->pInventory[target].m_Option2 )	// �����ۿ� ���� ����� �߶��� �ִٸ�..
	{
		if( _r < g_GlobalConfig.m_iSoulRateWithLuck ) // �ܺ������δ� 25% ���������δ� 20% ����
		{
			lpObj->pInventory[target].m_Level++;	
		}		
		else 
		{
			if( lpObj->pInventory[target].m_Level >= 7 )
			{
				lpObj->pInventory[target].m_Level = 0;
			}
			else
			{

				lpObj->pInventory[target].m_Level--;
				if( lpObj->pInventory[target].m_Level < 0 ) 
					lpObj->pInventory[target].m_Level = 0;
			}
		}
	}
	else	// 
	{		
		if( _r >= g_GlobalConfig.m_iSoulRate )	// 55% ����
		{
			if( lpObj->pInventory[target].m_Level >= 7 )
			{
				lpObj->pInventory[target].m_Level = 0;
			}
			else
			{
				lpObj->pInventory[target].m_Level--;
				if( lpObj->pInventory[target].m_Level < 0 ) 
					lpObj->pInventory[target].m_Level = 0;
			}
		}
		else lpObj->pInventory[target].m_Level++;
	}
#endif // MODIFY_ITEM_UPGRADE_MIX_RATE_FIX_20080627
	
	if( lpObj->pInventory[target].m_Level > 9 ) 
		lpObj->pInventory[target].m_Level = 9;
	else gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur =  (float)ItemGetDurability(lpObj->pInventory[target].m_Type, 
												   lpObj->pInventory[target].m_Level, 
												   lpObj->pInventory[target].IsExtItem(),
												   lpObj->pInventory[target].IsSetItem());
	
	lpObj->pInventory[target].m_Durability = levelitemdur*lpObj->pInventory[target].m_Durability/lpObj->pInventory[target].m_BaseDurability;

	// ������ ������ �缳���Ѵ�	
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type, 
										  lpObj->pInventory[target].m_Option1, 
										  lpObj->pInventory[target].m_Option2,
										  lpObj->pInventory[target].m_Option3,
										  lpObj->pInventory[target].m_NewOption,
										  lpObj->pInventory[target].m_SetOption,
										  lpObj->pInventory[target].m_ItemOptionEx);
	#else
		lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type, 
										  lpObj->pInventory[target].m_Option1, 
										  lpObj->pInventory[target].m_Option2,
										  lpObj->pInventory[target].m_Option3,
										  lpObj->pInventory[target].m_NewOption,
										  lpObj->pInventory[target].m_SetOption);
	#endif // ADD_380ITEM_NEWOPTION_20060711

	LogAddTD(lMsg.Get(559),  
			 lpObj->AccountID, 
			 lpObj->Name, 
			 lpObj->pInventory[source].m_Number,
			 lpObj->pInventory[target].GetName(), 
			 lpObj->pInventory[target].m_Number, 
			 lpObj->pInventory[target].m_Level);

#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
	// ��ȭ������ ���׷��̵尡 �����Ͽ� ������ ������ ���, ��Ȱ�� �޽���
	if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony( &lpObj->pInventory[target] ) == TRUE )
	{
		if ( g_kJewelOfHarmonySystem.IsActive( &lpObj->pInventory[target] ) == FALSE )
		{
			GCServerMsgStringSend(lMsg.Get(3370), lpObj->m_Index, 1); // ������ ������ ��ȭ �ɼ� ��� ���Ǻ��� ���� ���� ������� �ʽ��ϴ�.
		}
	}
#endif

	return TRUE;
}

// ����ڰ� �����Ǻ��� �� ����Ͽ� �������� ���� �÷ȴ�.
BOOL gObjItemRandomOption3Up(LPOBJECTSTRUCT lpObj, int source, int target)
{
	if( source < 0 || source > MAX_INVENTORY-1 ) return FALSE;
	if( target < 0 || target > MAX_INVENTORY-1 ) return FALSE;

	// �������� �ִ��� üũ
	if( lpObj->pInventory[source].IsItem() == FALSE ) return FALSE;
	if( lpObj->pInventory[target].IsItem() == FALSE ) return FALSE;

#ifdef ADD_THIRD_WING_20070525	// 3������ �����Ǻ������� �ɷ����� ����
	if( ( lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM( 12, 7 ) && lpObj->pInventory[target].m_Type < MAKE_ITEMNUM( 12, 36 ) ) || 
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1������ �ູ�� �������� ������ ����
		( lpObj->pInventory[target].m_Type > MAKE_ITEMNUM( 12, 43 ) && lpObj->pInventory[target].m_Type != MAKE_ITEMNUM( 13, 30 ) ) || 		
#else	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		( lpObj->pInventory[target].m_Type > MAKE_ITEMNUM( 12, 40 ) && lpObj->pInventory[target].m_Type != MAKE_ITEMNUM( 13, 30 ) ) || 		
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 4, 7 ) ||
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 4, 15 ) )
		return FALSE;			// ������ �Ҽ� ���� �����۵�	
#else
#ifdef DARKLORD_WORK
	if( (lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM(12,7) && lpObj->pInventory[target].m_Type != MAKE_ITEMNUM(13,30)) || 
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 7) ||
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 15) )
		return FALSE;			// ������ �Ҽ� ���� �����۵�
#else
	if( lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM(12,7) || 
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 7) ||
		lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(4, 15) )
		return FALSE;			// ������ �Ҽ� ���� �����۵�
#endif
#endif	// ADD_THIRD_WING_20070525

	LogAddTD(lMsg.Get(576), lpObj->AccountID, lpObj->Name, lpObj->pInventory[target].GetName(), lpObj->pInventory[target].m_Number, lpObj->pInventory[target].m_Option3);
	
	int _r = rand()%100;

	if( lpObj->pInventory[target].m_Option3 == 0 )
	{	// �ɼ��� ����, 
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
		if( (lpObj->pInventory[target].m_Type>=MAKE_ITEMNUM(12,3) && lpObj->pInventory[target].m_Type<=MAKE_ITEMNUM(12,6) )
			|| lpObj->pInventory[target].m_Type == MAKE_ITEMNUM(12,42))	// ������ ����
#else
		if( lpObj->pInventory[target].m_Type>=MAKE_ITEMNUM(12,3) && lpObj->pInventory[target].m_Type<=MAKE_ITEMNUM(12,6) )
#endif
		{	// �߰� ���� ���					
			lpObj->pInventory[target].m_NewOption &= ~PLUS_WING_OP1_TYPE;			
			if( rand()%2 )
			{
				lpObj->pInventory[target].m_NewOption |= PLUS_WING_OP1_TYPE;
			}
		}
	}

	// �ɼ��� 0 �̰ų� 4 �̸� ������ �� ��Ų��. (m_Option3 �� *4 �̴�)
	
/*/if( lpObj->pInventory[target].m_Option3 < 2 )
	{
		lpObj->pInventory[target].m_Option3++;
	}
	else */
	/*if( lpObj->pInventory[target].m_Option3 == 2 )	// Op = 8
	{
		if( _r < 50 ) lpObj->pInventory[target].m_Option3 = 1;
		else lpObj->pInventory[target].m_Option3++;
	}
	else 
	*/if( lpObj->pInventory[target].m_Option3 < 7 ) // Op = 12
	{
#ifdef ADD_THIRD_WING_20070525	// 3�������� �������� �����ɼ� ���̱�
		if( _r < (100 - g_GlobalConfig.m_iLifeRate) ) 
		{
			lpObj->pInventory[target].m_Option3 = 0;
		}
		else 
		{			
			if( lpObj->pInventory[target].m_Option3 == 0 )
			{
				// 3������ �ɼ��� �������� �ٴ´�.
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
				if( ( lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM( 12, 36 ) 
					&& lpObj->pInventory[target].m_Type <= MAKE_ITEMNUM( 12, 40 ) )
					|| lpObj->pInventory[target].m_Type == MAKE_ITEMNUM( 12, 43 ))
#else
				if( lpObj->pInventory[target].m_Type >= MAKE_ITEMNUM( 12, 36 ) 
					&& lpObj->pInventory[target].m_Type <= MAKE_ITEMNUM( 12, 40 ) )
#endif
				{		
					// ���� �ɼ��� ������
					lpObj->pInventory[target].m_NewOption &= ~PLUS_THIRD_WING_RAND_OPT_1;			
					lpObj->pInventory[target].m_NewOption &= ~PLUS_THIRD_WING_RAND_OPT_2;	

					int iOpt3TypeRand = rand()%2;
					int iOpt3RandRate;
					switch( iOpt3TypeRand )
					{
					case 0:
						iOpt3RandRate = rand()%1000;
						if( iOpt3RandRate < 400 ) 
							lpObj->pInventory[target].m_NewOption |= PLUS_THIRD_WING_RAND_OPT_1;
						break;
					case 1:
						iOpt3RandRate = rand()%1000;
						if( iOpt3TypeRand < 300 ) 
							lpObj->pInventory[target].m_NewOption |= PLUS_THIRD_WING_RAND_OPT_2;
						break;			
					}
				}
			}
			lpObj->pInventory[target].m_Option3++;
		}
#else
		if( _r < (100 - g_GlobalConfig.m_iLifeRate) ) lpObj->pInventory[target].m_Option3 = 0;
		else lpObj->pInventory[target].m_Option3++;
#endif
	}
	else // Op = 16
	{
		return FALSE;
	}

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur =  (float)ItemGetDurability(lpObj->pInventory[target].m_Type, 
												   lpObj->pInventory[target].m_Level, 
												   lpObj->pInventory[target].IsExtItem(),
												   lpObj->pInventory[target].IsSetItem());
	
	
	lpObj->pInventory[target].m_Durability = levelitemdur*lpObj->pInventory[target].m_Durability/lpObj->pInventory[target].m_BaseDurability;
	// ������ ������ �缳���Ѵ�
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type, 
										  lpObj->pInventory[target].m_Option1, 
										  lpObj->pInventory[target].m_Option2,
										  lpObj->pInventory[target].m_Option3,
										  lpObj->pInventory[target].m_NewOption,
										  lpObj->pInventory[target].m_SetOption, 
										  lpObj->pInventory[target].m_ItemOptionEx);
	#else
		lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type, 
										  lpObj->pInventory[target].m_Option1, 
										  lpObj->pInventory[target].m_Option2,
										  lpObj->pInventory[target].m_Option3,
										  lpObj->pInventory[target].m_NewOption,
										  lpObj->pInventory[target].m_SetOption);
	#endif // ADD_380ITEM_NEWOPTION_20060711

	LogAddTD(lMsg.Get(577),  
			 lpObj->AccountID, 
			 lpObj->Name, 
			 lpObj->pInventory[source].m_Number,
			 lpObj->pInventory[target].GetName(), 
			 lpObj->pInventory[target].m_Number, 
			 lpObj->pInventory[target].m_Option3);

	BYTE NewOption[8];

	ItemIsBufExOption(NewOption, &lpObj->pInventory[target]);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[%s][%s] JewelofLife Result [%d,%s,%d,%d,%d,%d]serial:[%u] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
#else
	LogAddTD("[%s][%s] JewelofLife Result [%d,%s,%d,%d,%d,%d]serial:[%d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			 lpObj->AccountID, 
			 lpObj->Name, 
			 target, 
			 lpObj->pInventory[target].GetName(), 
			 lpObj->pInventory[target].m_Level, 
			 lpObj->pInventory[target].m_Option1, 
			 lpObj->pInventory[target].m_Option2, 
			 lpObj->pInventory[target].m_Option3, 
			 lpObj->pInventory[target].m_Number, 
			 (BYTE)lpObj->pInventory[target].m_Durability,
			 NewOption[0],
			 NewOption[1],
			 NewOption[2],
			 NewOption[3],
			 NewOption[4],
			 NewOption[5],
			 NewOption[6]
			);
	
	return TRUE;
}



// ĳ������ �ɷ�ġ�� �ٽ� �����Ѵ�.
// ������ ��� ���Ѵ�.
void gObjAbilityReSet(LPOBJECTSTRUCT lpObj)
{
	lpObj->LevelUpPoint = (lpObj->Level-1)*5;
	
	lpObj->Strength		= DCInfo.DefClass[lpObj->Class].Strength;
	lpObj->Dexterity	= DCInfo.DefClass[lpObj->Class].Dexterity;
	lpObj->Vitality		= DCInfo.DefClass[lpObj->Class].Vitality;
	lpObj->Energy		= DCInfo.DefClass[lpObj->Class].Energy;
		
	lpObj->MaxLife		= DCInfo.DefClass[lpObj->Class].MaxLife;
	lpObj->MaxMana		= DCInfo.DefClass[lpObj->Class].MaxMana;	

	if( lpObj->Class == 0 )
	{
		lpObj->MaxLife		+= (float)(lpObj->Level-1)*0.5f;
		lpObj->MaxMana		+= (float)(lpObj->Level-1)*2;
		lpObj->Life			= lpObj->MaxLife;
		lpObj->Mana			= lpObj->MaxMana;
	}
	else
	{
		lpObj->MaxLife		+= (float)(lpObj->Level-1)*2;
		lpObj->MaxMana		+= (float)(lpObj->Level-1)*0.5f;
		lpObj->Life			= lpObj->MaxLife;
		lpObj->Mana			= lpObj->MaxMana;
	}
	CloseClient ( lpObj->m_Index );
}

#ifdef _NEW_EXDB_
// ���� ���� ����ڰ� �������� ����ΰ�? üũ
// RETURN ------------------------------------
// 0 : �ƹ� ��� ����.
// 1 : ���� ���� ����ڰ� ���� �������� ����̴�.
BOOL gObjTargetGuildWarCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	if( lpObj->Type       != OBJTYPE_CHARACTER )					return FALSE;
	if( lpTargetObj->Type != OBJTYPE_CHARACTER )					return FALSE;
	if( lpObj->GuildNumber < 1 || lpTargetObj->GuildNumber < 1 )	return FALSE;
	
	if( lpObj->lpGuild->WarState != 1 || lpTargetObj->lpGuild->WarState != 1 )		return FALSE;
	if( strcmp(lpObj->lpGuild->TargetGuildName, lpTargetObj->lpGuild->Name) != 0 )	return FALSE;

	return TRUE;
}


// ������ ����Ǿ��ٰ� �ι�° ��忡�� �˸���.
// Result1 : lpObj���� ������ ���..
// Result2 : �� ��忡�� ������ ���..
void gObjGuildWarEndSend(LPOBJECTSTRUCT lpObj, BYTE Result1, BYTE Result2)
{
	if( lpObj == NULL ) 
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		return;
	}

	if( lpObj->lpGuild == NULL ) 
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		return;
	}

	if( lpObj->lpGuild->WarState != 1 )
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		return;
	}
	
	if (lpObj->lpGuild->lpTargetGuildNode == NULL)			//#BUGFIX_20040831		(MEMORY ACCESS VIOLATION �߻�, b4nfter)
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		return;
	}

		
	// �¸� ��带 ù��° ���ڷ�..
	if( Result1 == 0x01 )
	{		
		gObjGuildWarItemGive(lpObj->lpGuild, lpObj->lpGuild->lpTargetGuildNode);
	}
	else if( Result1 == 0x03 )
	{
		gObjGuildWarItemGive(lpObj->lpGuild->lpTargetGuildNode, lpObj->lpGuild);
	}
	else if( Result1 == 0x01 )
	{		
		gObjGuildWarItemGive(lpObj->lpGuild->lpTargetGuildNode, lpObj->lpGuild);
	}

	int n;
	for( n=0; n<MAX_GUILD; n++)
	{
		if( lpObj->lpGuild->Use[n] )
		{
			if( lpObj->lpGuild->Index[n] >=0 )
			{
				gObj[lpObj->lpGuild->Index[n]].IsInBattleGround = 0;
				GCGuildWarEnd(lpObj->lpGuild->Index[n], Result1, lpObj->lpGuild->lpTargetGuildNode->Name);
				//LogAdd("���� ���� �˸� : %s", lpObj->lpGuild->Names[n]);
			}
		}
	}
	for( n=0; n<MAX_GUILD; n++)
	{
		if( lpObj->lpGuild->lpTargetGuildNode->Use[n] )
		{
			
			if( lpObj->lpGuild->lpTargetGuildNode->Index[n] >=0 )
			{
				gObj[lpObj->lpGuild->lpTargetGuildNode->Index[n]].IsInBattleGround = 0;

				GCGuildWarEnd(lpObj->lpGuild->lpTargetGuildNode->Index[n], Result2, lpObj->lpGuild->Name);
				//LogAdd("���� ���� �˸� : %s", lpObj->lpGuild->lpTargetGuildNode->Names[n]);
			}
		}
	}	
	
}


void gObjGuildWarEndSend(LPGUILD_INFO lpGuild1, LPGUILD_INFO lpGuild2, BYTE Result1, BYTE Result2)
{
	if( lpGuild1 == NULL || lpGuild2 == NULL ) return;

	if( lpGuild1->WarState != 1 || lpGuild2->WarState != 1 )
	{
		return;
	}
			
	int n;
	for( n=0; n<MAX_GUILD; n++)
	{
		if( lpGuild1->Use[n] )
		{
			if( lpGuild1->Index[n] >=0 )
			{
				gObj[lpGuild1->Index[n]].IsInBattleGround = 0;
				
				GCGuildWarEnd(lpGuild1->Index[n], Result1, lpGuild2->Name);
			}
		}
	}
	for( n=0; n<MAX_GUILD; n++)
	{
		if( lpGuild2->Use[n] )
		{
			if( lpGuild2->Index[n] >=0 )
			{
				gObj[lpGuild2->Index[n]].IsInBattleGround = 0;

				GCGuildWarEnd(lpGuild2->Index[n], Result2, lpGuild1->Name);
			}
		}
	}
}

void gObjGuildWarEnd(LPGUILD_INFO lpGuild, LPGUILD_INFO lpTargetGuild)
{
	if( lpGuild == NULL || lpTargetGuild == NULL ) 
	{
		//LogAddTD("error-L3 : ��� ����Ʈ ���� %s %d", __FILE__, __LINE__);
		return;
	}

	lpGuild->WarDeclareState = 0;
	lpGuild->WarState		 = 0;
	
	//LogAddTD("��� ���� ���� �޽��� : %s", lpGuild->Name);
	
	if( lpTargetGuild != NULL )
	{
		lpTargetGuild->WarDeclareState	= 0;
		lpTargetGuild->WarState			= 0;
		//LogAdd("��� ���� ���� �޽��� : %s", lpTargetGuild->Name);
		LPGUILD_INFO	lpTguild;
		
		if (lpGuild->lpTargetGuildNode != NULL) {		//#BUGFIX_20040831		(MEMORY ACCESS VIOLATION �߻�, b4nfter)
			lpTguild = lpGuild->lpTargetGuildNode;
			lpGuild->lpTargetGuildNode = NULL;
			lpTguild->lpTargetGuildNode = NULL;
		}
	}
	else 
	{
		lpGuild->lpTargetGuildNode = NULL;
	}
}

int gObjGuildWarProc(LPGUILD_INFO lpGuild1, LPGUILD_INFO lpGuild2, int score)
{
	if( lpGuild1 == NULL || lpGuild2 == NULL ) return 0;

	int totalscore=g_GlobalConfig.m_iGuildWarScore;
	int n;
	int maxscore=20;
	
	lpGuild1->PlayScore += score;

	char szTemp[256];
	wsprintf(szTemp, "%s ( %u ) VS %s ( %u )",lpGuild1->Name, (DWORD)lpGuild1->PlayScore, lpGuild2->Name, (DWORD)lpGuild2->PlayScore);
	LogAddTD(szTemp);
//	cManager.ManagerSendData(szTemp, 1);

	if( lpGuild1->WarType == BTT_SOCCER )
	{
		SetBattleTeamScore(lpGuild1->BattleGroundIndex, lpGuild1->BattleTeamCode, lpGuild1->PlayScore);
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
		maxscore = 100;
#else
		maxscore = 100;
#endif
		//maxscore = 4;
	}
	else 
	{
		maxscore = 20;
		//maxscore = 4;
	}

	cManager.BattleInfoSend(GetBattleTeamName(0, 0), GetBattleTeamScore(0, 0), GetBattleTeamName(0, 1), GetBattleTeamScore(0, 1));

	if( lpGuild1->PlayScore >= maxscore  ) // ���� ����
	{
		if( lpGuild1->PlayScore > maxscore &&		// 20:0 �̸�..
			lpGuild2->PlayScore == 0 )
		{
			totalscore += 2;
		}
		else if( lpGuild1->PlayScore > maxscore &&	// 20�� 10 ���ϸ� 
			lpGuild2->PlayScore <= 10 )
		{
			totalscore += 1;
		}		
		lpGuild1->TotalScore += totalscore;

		DGGuildScoreUpdate(lpGuild1->Name, lpGuild1->TotalScore);
		DGGuildScoreUpdate(lpGuild2->Name, lpGuild2->TotalScore);
		return 1;
	}
	else
	{
		for( n=0; n<MAX_GUILD; n++)
		{
			if( lpGuild1->Use[n] )
			{
				if( lpGuild1->Index[n] >=0 )
				{
					GCGuildWarScore(lpGuild1->Index[n]);
				}
			}
		}
		for( n=0; n<MAX_GUILD; n++)
		{
			if( lpGuild2->Use[n] )
			{
				if( lpGuild2->Index[n] >=0 )
				{
					GCGuildWarScore(lpGuild2->Index[n]);
				}
			}
		}
	}
	return 0;
}


// ���� ���� ����ڰ� �������� ����ΰ�? üũ
BOOL gObjGuildWarCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	int score=1;
	LPGUILD_INFO lpGuild;
	LPGUILD_INFO lpTargetGuild;


	if( gObjTargetGuildWarCheck(lpObj, lpTargetObj) == FALSE ) return FALSE;

	// ��帶���͸� �׿������� 2��, ������ �׿������� 1��
	if( strcmp(lpTargetObj->Name, lpTargetObj->lpGuild->Names[0]) == 0 )
	{
		score = 2;
	}
	lpGuild = lpObj->lpGuild;
	lpTargetGuild = lpTargetObj->lpGuild;
	
	
	if( gObjGuildWarProc(lpGuild, lpTargetGuild, score) == 1 )
	{
		// ��Ⱑ ���� ��ٸ�..
		if( lpGuild->WarType == BTT_SOCCER )
		{
			gObjGuildWarEndSend(lpGuild, lpTargetGuild, 0x01, 0x00);
			gBattleGroundEnable(lpGuild->BattleGroundIndex, 0);
			gObjGuildWarEnd(lpGuild, lpTargetGuild);
			cManager.BattleInfoSend(GetBattleTeamName(0, 0), 255, GetBattleTeamName(0, 1), 255);

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
			PMSG_MATCH_RESULT	pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x24, sizeof( pMsg) );
			pMsg.MatchType	= eMatchType_BattleSoccer;

			memcpy(pMsg.MatchTeamName1, lpGuild->Name, MAX_IDSTRING);							
			pMsg.Score1 = lpGuild->PlayScore;

			memcpy(pMsg.MatchTeamName2, lpTargetGuild->Name, MAX_IDSTRING);
			pMsg.Score2 = lpTargetGuild->PlayScore;

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

			for( UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
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

			LogAddTD("[WTEvent] BattleSoccer Result Send = %s-%d %s-%d", 
					 lpGuild->Name , lpGuild->PlayScore, 
					 lpTargetGuild->Name , lpTargetGuild->PlayScore);

#endif
		}
		else
		{
			gObjGuildWarEndSend(lpObj, 0x01, 0x00);
#ifdef CHANGE_GUILD_WAR_END_DELAYTIME
			gObjAddMsgSendDelay(lpObj, MSG_GUILDWAREND, lpObj->m_Index, 1000*2);
#else
			gObjAddMsgSendDelay(lpObj, MSG_GUILDWAREND, lpObj->m_Index, 1000*50);
#endif

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
			PMSG_MATCH_RESULT	pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x24, sizeof( pMsg) );
			pMsg.MatchType	= eMatchType_GuildWar;

			memcpy(pMsg.MatchTeamName1, lpGuild->Name, MAX_IDSTRING);							
			pMsg.Score1 = lpGuild->PlayScore;

			memcpy(pMsg.MatchTeamName2, lpTargetGuild->Name, MAX_IDSTRING);
			pMsg.Score2 = lpTargetGuild->PlayScore;

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

			for( UserIndex=0; UserIndex<MAX_GUILD; UserIndex++)
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

			LogAddTD("[WTEvent] GuildWar Result Send = %s-%d %s-%d", 
					 lpGuild->Name , lpGuild->PlayScore, 
					 lpTargetGuild->Name , lpTargetGuild->PlayScore);

#endif


		}
	}
	return TRUE;
}



// ����� ���ε�.. �����Ͱ� �����ߴٸ� ������ �ߴ� ��Ų��.
BOOL gObjGuildWarMasterClose(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->GuildNumber < 1 ) return FALSE;
	
	// �渶�̸�..
	if( strcmp(lpObj->lpGuild->Names[0], lpObj->Name) != 0 ) return FALSE;
	
	// ������ �������� ���..
	gObjGuildWarEndSend(lpObj, 0x03, 0x2);
	if( lpObj->lpGuild->WarType == BTT_SOCCER )
	{
#ifdef MODIFY_GUILDWAR_REQUEST_PROCESS_20050704
		if( lpObj->lpGuild->BattleGroundIndex < MAX_BATTLESOCCERGROUND )
		{
			gBattleGroundEnable(lpObj->lpGuild->BattleGroundIndex, 0);
			cManager.BattleInfoSend(GetBattleTeamName(0, 0), 255, GetBattleTeamName(0, 1), 255);
		}
#else
		gBattleGroundEnable(lpObj->lpGuild->BattleGroundIndex, 0);
		cManager.BattleInfoSend(GetBattleTeamName(0, 0), 255, GetBattleTeamName(0, 1), 255);
#endif
	}
	gObjGuildWarEnd(lpObj->lpGuild, lpObj->lpGuild->lpTargetGuildNode);
	return TRUE;
}

// ������� ������  �� ����� ������� �Ѹ��� �������� ��� �̱����� ���� �ش�.
BOOL gObjGuildWarItemGive(LPGUILD_INFO lpWinGuild, LPGUILD_INFO lpLoseGuild)
{
	return TRUE;
	int n;
	int r_userindex[MAX_GUILD];	//
	int r_usercount=0;
	int lose_user=-1;
	int win_user=-1;

	for( n=0; n<MAX_GUILD; n++)
	{
		if( lpLoseGuild->Use[n] > 0 )	// ������� �����
		{
			r_userindex[r_usercount] = lpLoseGuild->Index[n];
			r_usercount++;
		}
	}
	if( r_usercount < 1 ) 
	{
		LogAdd(lMsg.Get(560), __FILE__, __LINE__);
		return FALSE;
	}
	lose_user = r_userindex[rand()%r_usercount];
	if( lose_user < 0 )
	{
		LogAdd(lMsg.Get(560),__FILE__, __LINE__);
		return FALSE;
	}
	if( gObj[lose_user].Connected < 3 ) 
	{
		LogAdd(lMsg.Get(560),__FILE__, __LINE__);
		return FALSE;
	}
	r_usercount = 0;
	for( n=0; n<MAX_GUILD; n++)
	{
		if( lpWinGuild->Use[n] > 0 )	// ������� �����
		{
			r_userindex[r_usercount] = lpWinGuild->Index[n];
			r_usercount++;
		}
	}
	if( r_usercount < 1 ) 
	{
		LogAdd(lMsg.Get(561));
		return FALSE;
	}
	win_user = r_userindex[rand()%r_usercount];
	if( win_user  < 0 )
	{
		LogAdd(lMsg.Get(561));
		return FALSE;
	}
	if( gObj[win_user].Connected < 3 ) 
	{
		LogAdd(lMsg.Get(561));
		return FALSE;
	}

	LPOBJECTSTRUCT	lpObj;
	lpObj = &gObj[lose_user];

	int count = MAX_EQUIPMENT*2;
	int number;
	
	while( count-- )
	{
		number = (rand()%MAX_EQUIPMENT);
		
		// ������ ������ �ϳ��� ����Ʈ����.
		if( lpObj->pInventory[number].IsItem() == TRUE )
		{			
			if( gObjInventoryInsertItem(win_user, lpObj->pInventory[number]) != 0xFF )
			{
				/*
				LogAdd("%s ������ �ֱ�", lpObj->pInventory[number].GetName());
				char szTemp[256];
				wsprintf(szTemp, "������ �й�� %s�������� ��� ������ �Ѱ���ϴ�.", lpObj->pInventory[number].GetName());
				GCServerMsgStringSend(szTemp, lose_user, 1);
				
				wsprintf(szTemp, "������ �¸��� %s�������� ��� �������� �޾ҽ��ϴ�.", lpObj->pInventory[number].GetName());
				GCServerMsgStringSend(szTemp, win_user, 1);

				gObjInventoryDeleteItem(lose_user, number);
				GCInventoryItemDeleteSend(lose_user, number, 0);

				GCEquipmentSend(lose_user);
				GCItemListSend(win_user);
				*/
				return TRUE;
			}
			break;
		}
	}
	
	// �κ��丮 �߿��� ����Ʈ����.
	count=MAX_INVENTORY-MAX_EQUIPMENT;
	while(count--)	
	{
		number = MAX_EQUIPMENT+(rand()%(MAX_INVENTORY-MAX_EQUIPMENT));
		
		// ������ �ϳ��� ����Ʈ����.
		if( lpObj->pInventory[number].IsItem() == TRUE )
		{			
			if( gObjInventoryInsertItem(win_user, lpObj->pInventory[number]) != 0xFF )
			{
				/*LogAdd("%s ������ �ֱ�", lpObj->pInventory[number].GetName());
				char szTemp[256];
				wsprintf(szTemp, "������ �й�� %s�������� ��� ������ �Ѱ���ϴ�.", lpObj->pInventory[number].GetName());
				GCServerMsgStringSend(szTemp, lose_user, 1);
				
				wsprintf(szTemp, "������ �¸��� %s�������� ��� �������� �޾ҽ��ϴ�.", lpObj->pInventory[number].GetName());
				GCServerMsgStringSend(szTemp, win_user, 1);

				gObjInventoryDeleteItem(lose_user, number);
				GCInventoryItemDeleteSend(lose_user, number, 0);

				GCEquipmentSend(lose_user);
				GCItemListSend(win_user);
				*/
				return TRUE;
			}
			break;
		}
	}

	return TRUE;
}
#endif

// �Ϲ��α��� ���� �ð��� �˸���.
BOOL gObjGetPkTime(LPOBJECTSTRUCT lpObj, int & hour, int & min)
{
	int  pktime=0;

	if( lpObj->m_PK_Time > 0 )
	{
		pktime = lpObj->m_PK_Time/60;
	}
#ifdef HERO_SYSTEM_MODIFY
	else {
		hour = 0;
		min = 0;
		return TRUE;
	}

	if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1  )
	{
		hour = (PK_COUNT_1_TIME/60-pktime)/60;
		min  = (PK_COUNT_1_TIME/60-pktime)%60;
		return TRUE;	
	}
	else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+2  )
	{
		hour = (PK_COUNT_2_TIME/60-pktime)/60;
		min  = (PK_COUNT_2_TIME/60-pktime)%60;
		return TRUE;
	}
	else if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3  )
	{
		hour = (((PK_COUNT_3_TIME/60)+((lpObj->m_PK_Count-3)*60))-pktime)/60;
		min  = (((PK_COUNT_3_TIME/60)+((lpObj->m_PK_Count-3)*60))-pktime)%60;
		return TRUE;
	}
#else
	if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1  )
	{
		hour = ((60*3)-pktime)/60;
		min  = ((60*3)-pktime)%60;
		return TRUE;	
	}
	else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+2  )
	{
		hour = (((60*3)*2)-pktime)/60;
		min  = (((60*3)*2)-pktime)%60;
		return TRUE;
	}
	else if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3  )
	{
		hour = ((((60*3)*3)+((lpObj->m_PK_Count-3)*60))-pktime)/60;
		min  = ((((60*3)*3)+((lpObj->m_PK_Count-3)*60))-pktime)%60;
		return TRUE;
	}



#endif	

	return FALSE;
}

// ���͸� ��ȯ�Ѵ�.
BOOL gObjMonsterCall(int aIndex, int MonsterType, int x, int y)
{
	int result;

#ifdef NEW_SKILL_FORSKYLAND
	if( gObj[aIndex].MapNumber == 10 )
	{	// õ�������� ��ȯ�Ҽ� ����		
		return FALSE;
	}
#endif
	if( gObj[aIndex].m_RecallMon >= 0 ) 
	{
		GCRecallMonLife(aIndex, 60, 0);
		gObjMonsterCallKill(aIndex);
		return FALSE;
		//return;
	}
	result = gObjAddCallMon();
		
	if( result >= 0 )
	{
		gObj[result].X = x;
		gObj[result].Y = y;
		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;
		gObj[result].MapNumber = gObj[aIndex].MapNumber;
		
		gObjSetMonster(result, MonsterType);
		gObj[result].m_RecallMon = aIndex;

		gObj[result].m_Attribute	= 100;
		//gObj[result].m_MoveSpeed	= 100;
		gObj[result].TargetNumber	= -1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;
		gObj[result].PathCount		= 0;				
		gObj[aIndex].m_RecallMon	= result;
		
		gObj[result].m_MoveRange = 15;

		GCRecallMonLife(gObj[result].m_RecallMon, (int)gObj[result].MaxLife, (int)gObj[result].Life);

		LogAddTD("[Summon Monster] [%s][%s] Try to Summon Monster - Succeed (SummonIndex:%d)", 
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name,
			result
			);
		return TRUE;
	}
	return FALSE;
}

void gObjMonsterCallKill(int aIndex)
{
	LogAddTD("[Summon Monster] [%s][%s] Try to Kill Summoned Monster", 
		gObj[aIndex].AccountID, 
		gObj[aIndex].Name
		);

	int callmon;
	if( gObj[aIndex].m_RecallMon < 0 ) return;	

	callmon = gObj[aIndex].m_RecallMon;

	if( callmon < 0 || callmon > MAX_OBJECT-1 ) return;
	if( gObj[callmon].Type != OBJTYPE_MONSTER ) return;
	if( gObj[callmon].m_RecallMon != aIndex ) return;
	
	if( gObj[aIndex].m_RecallMon >= 0 )
	{
		if( gObjIsConnected(gObj[aIndex].m_RecallMon)== TRUE)
		{
			LogAddTD("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Found Summoned Monster (SummonIndex:%d)", 
				gObj[aIndex].AccountID, 
				gObj[aIndex].Name,
				gObj[aIndex].m_RecallMon
				);

			gObj[gObj[aIndex].m_RecallMon].m_RecallMon = -1;
		}
	}
	gObj[aIndex].m_RecallMon = -1;
	gObjDel(callmon);

	LogAddTD("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Finished to Kill Summoned Monster (SummonIndex:%d)", 
		gObj[aIndex].AccountID, 
		gObj[aIndex].Name,
		callmon
		);
}

BOOL	gObjCheckTeleportArea(int aIndex, BYTE x, BYTE y)
{	

	if( (x < (gObj[aIndex].X-8)) || (x > (gObj[aIndex].X+8)) ||
	    (y < (gObj[aIndex].Y-8)) || (y > (gObj[aIndex].Y+8)) )
	{	// ���� �̵� ������ ���� ����
		return FALSE;
	}

	// �̵� ������ �������� �Ǻ�
	int mapnumber = gObj[aIndex].MapNumber;

#if ENABLE_MC_CODE == 1
	if (mapnumber < 0 || mapnumber >= g_TerrainManager.Size())
#else // ENABLE_MC_CODE == 1
	if (mapnumber < 0 || mapnumber >= MAX_MAP)
#endif // ENABLE_MC_CODE == 1
	{
		return FALSE;
	}	

	BYTE attr = MapC[mapnumber].GetAttr(x, y);
	if( attr != MAP_ATTR_NONE )
	{	// �̵������� ������ �ƴϸ� (�������� ����)
		return FALSE;
	}

	attr = MapC[mapnumber].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);
	if( attr & MAP_ATTR_SAFTYZONE )
	{	// �������뿡���� �̵��Ҽ� ����
		return FALSE;
	}

	return TRUE;
}

BOOL	gObjCheckAttackAreaUsedPath(int aIndex, int TarObjIndex)
{
	int X, Y;

	if( gObj[aIndex].PathCount > 1 )
	{
		X = gObj[aIndex].PathX[gObj[aIndex].PathCount-1];
		Y = gObj[aIndex].PathY[gObj[aIndex].PathCount-1];
	}	
	else
	{
		return FALSE;
	}

	if( (gObj[TarObjIndex].X < (X-10)) || (gObj[TarObjIndex].X > (X+10)) ||
	    (gObj[TarObjIndex].Y < (Y-10)) || (gObj[TarObjIndex].Y > (Y+10)) )
	{	// ���ݰŸ��� �����
		return FALSE;
	}

	return TRUE;
}

BOOL	gObjCheckattackAreaUsedViewPort(int aIndex, int TarObjIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj = &gObj[TarObjIndex];
	

	if( lpTargetObj->m_Index == lpObj->m_Index )
		return TRUE;

	int MVL = MAXVIEWPORTOBJECT;
	if( lpObj->Type == OBJTYPE_MONSTER ) MVL = MAX_MONVIEWPORTOBJECT;

// 1. ���� ������ ��ϵǾ� �ִ� �������� �˾Ƴ���.
// 2. ��ϵǾ� �ִ� ������� �����Ѵ�.
// 3. ����� �ȵ� ������� ����Ѵ�.
	for( int n=0; n<MVL; n++)
	{
		if( lpObj->VpPlayer[n].state )
		{ // ������ ��ϵǾ��ִٸ�
			if( lpObj->VpPlayer[n].number == lpTargetObj->m_Index ) 
			{   // ��ϵǾ��ִٸ�
				//if( lpObj->VpPlayer[n].state == 1 ) lpObj->VpPlayer[n].state = 2;
				return TRUE;
			}
		} 

#ifdef MODIFY_KUNDUN_VIEWPORT_20060523
		// ���� ���� �ִ� ���� ��Ͽ� �־ ��ϵ� ������ ����
		if ( lpObj->VpPlayer2[n].state )
		{
			if ( lpObj->VpPlayer2[n].number == lpTargetObj->m_Index )
			{
				return TRUE;
			}
		}
#endif // MODIFY_KUNDUN_VIEWPORT_20060523

	}
	return FALSE;
}

BOOL	gObjCheckAttackArea(int aIndex, int TarObjIndex)
{
#ifdef WORLD_TOURNAMENT_EVENT_SETTING	
	return 0;
#endif
	if( !gObjCheckattackAreaUsedViewPort(aIndex, TarObjIndex) )
	{	// �� ����Ʈ �ȿ� ���� ������̴�
		return 1;
	}

		/*
	if( (gObj[TarObjIndex].X < (gObj[aIndex].X-10)) || (gObj[TarObjIndex].X > (gObj[aIndex].X+10)) ||
	    (gObj[TarObjIndex].Y < (gObj[aIndex].Y-10)) || (gObj[TarObjIndex].Y > (gObj[aIndex].Y+10)) )
	{	// ���ݰŸ��� �����
		if( !gObjCheckAttackAreaUsedPath(aIndex, TarObjIndex) )
		{	// �������� ��ó ������ ��������...
			return 1;
		}
		return 7;
	}
	*/

	// �̵� ������ �������� �Ǻ�

	// ��..
	int mapnumber = gObj[aIndex].MapNumber;

#if ENABLE_MC_CODE == 1
	if (mapnumber < 0 || mapnumber >= g_TerrainManager.Size())
#else // ENABLE_MC_CODE == 1
	if (mapnumber < 0 || mapnumber >= MAX_MAP)
#endif // ENABLE_MC_CODE == 1
	{
		return 2;
	}	

	// ���� ���
	int tarmapnumber = gObj[TarObjIndex].MapNumber;

#if ENABLE_MC_CODE == 1
	if (tarmapnumber < 0 || tarmapnumber >= g_TerrainManager.Size())
#else // ENABLE_MC_CODE == 1
	if (tarmapnumber < 0 || tarmapnumber >= MAX_MAP)
#endif // ENABLE_MC_CODE == 1
	{
		return 3;
	}	

	if( tarmapnumber != mapnumber )
	{	// ���� ���� �ƴϸ�, 
		return 4;
	}

#ifdef CASTLE_NPC_GATE_WORK_20041206		// ������ �ÿ��� ���� ������ ���� ���Ӽ� ���ݺҰ��� �����Ѵ�.
	BYTE attr = 0;
#else
	BYTE attr = MapC[mapnumber].GetAttr(gObj[TarObjIndex].X, gObj[TarObjIndex].Y);
	if( (attr != MAP_ATTR_NONE) && !(attr & MAP_ATTR_STAND) )
	{	// ����� ���ݰ����� ������ �ƴϸ�
		return 5;
	}
#endif

	attr = MapC[mapnumber].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);
	if( (attr != MAP_ATTR_NONE) && !(attr & MAP_ATTR_STAND) )
	{	// ���� ���ݰ����� ������ �ƴϸ�
		return 6;
	}	

	return 0;
}
// �Ǹ��� ���� �ʴ���� �ִ��� ã�´�
BOOL	gUserFindDevilSquareInvitation(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	for(int  n=0; n<MAX_INVENTORY; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(14, 19) )
			{
				return TRUE;
			}
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
			// �Ǹ��� ���� ���� ����� �߰�
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM( 13, 46 ) )
			{
				return TRUE;
			}
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		}
	}
	return FALSE;
}


// �Ǹ��� ����, ���� �ִ��� ã�´�
BOOL	gUserFindDevilSquareKeyEyes(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	BOOL bFindKey = FALSE, bFindEyes = FALSE;

	for(int  n=0; n<MAX_INVENTORY; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(14, 17) )
			{
				bFindEyes = TRUE;
			}
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(14, 18) )
			{
				bFindKey = TRUE;
			}
		}
	}

	if( bFindKey && bFindEyes )
		return TRUE;

	return FALSE;
}


void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel)
{
	int n;
	LPOBJECTSTRUCT	lpObj;
	
	if( gDisconnect == 1 ) return;

	if( endLevel < startLevel )
	{
		endLevel = g_GlobalConfig.m_iMaxLevel;
	}
#ifdef GMCOMMAND_EX_1ST_BUGFIX
	int sClassCount[6] = {0,0,0,0,0,0};
	int nTotal = 0;
	
	for( n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++ )
	{
		lpObj = &gObj[n];
		
		if( lpObj->Connected > 2 )
		{				
			if( lpObj->Class >= 6 && lpObj->Class < 0 )
				continue;
			
			if( lpObj->Level >= startLevel && lpObj->Level <= endLevel )
			{
				sClassCount[lpObj->Class]++;				
			}
			nTotal++;
		}
	}

	MsgOutput(gObj[aIndex].m_Index, "[%d]-[%d] Total %d %s %d %s %d %s %d %s %d %s %d %s %d", 
		startLevel, endLevel, nTotal, 
		lMsg.Get(1900), sClassCount[0],
		lMsg.Get(1901), sClassCount[1],
		lMsg.Get(1902), sClassCount[2],
		lMsg.Get(1903), sClassCount[3],
		lMsg.Get(1904), sClassCount[4], 
		lMsg.Get(1905), sClassCount[5]);

#else

#ifdef MODIFY_GMCOMMAND_USERSTAT_20041127
	int sClassCount[6] = {0,0,0,0,0,0};

	for( n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++ )
	{
		lpObj = &gObj[n];
		
		if( lpObj->Connected > 2 )
		{				
			if( lpObj->Class >= 6 && lpObj->Class < 0 )
				continue;

			if( lpObj->Level >= startLevel && lpObj->Level <= endLevel )
			{
				sClassCount[lpObj->Class]++;				
			}
			sClassCount[5]++;
		}
	}

	MsgOutput(gObj[aIndex].m_Index, "[%d]-[%d] Total %d %s %d %s %d %s %d %s %d %s %d", 
				 startLevel, endLevel, sClassCount[5], 
				 lMsg.Get(1900), sClassCount[0],
				 lMsg.Get(1901), sClassCount[1],
				 lMsg.Get(1902), sClassCount[2],
				 lMsg.Get(1903), sClassCount[3],
				 lMsg.Get(1904), sClassCount[4]);
#else
	int sClassCount[5] = {0,0,0,0,0};

	for( n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++ )
	{
		lpObj = &gObj[n];
		
		if( lpObj->Connected > 2 )
		{				
			if( lpObj->Level >= startLevel && lpObj->Level <= endLevel )
			{
				sClassCount[lpObj->Class]++;				
			}
			sClassCount[4]++;
			}
	}

	MsgOutput(gObj[aIndex].m_Index, "[%d]-[%d] Total %d %s %d %s %d %s %d %s %d", 
				 startLevel, endLevel, sClassCount[4], 
				 lMsg.Get(1900), sClassCount[0],
				 lMsg.Get(1901), sClassCount[1],
				 lMsg.Get(1902), sClassCount[2],
				 lMsg.Get(1903), sClassCount[3]);
#endif

#endif // GMCOMMAND_EX_1ST_BUGFIX
}


LPOBJECTSTRUCT	 gObjFind(char* targetcharname)
{	
	if( gDisconnect == 1 ) return NULL;

	int n;
	LPOBJECTSTRUCT	lpObj;

#if TESTSERVER == 1
	for( n=0; n<MAX_OBJECT; n++ )
#else
	for( n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++ )
#endif
	{
		lpObj = &gObj[n];
		
		if( lpObj->Connected > 2 )
		{	
			if( lpObj->Name[0] == targetcharname[0] )
			{
				if( !strcmp(lpObj->Name, targetcharname ) )
				{
					return lpObj;
				}
			}			
		}
	}
	return NULL;
}

int	gObjFind10EventChip(int aIndex)
{	
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int count = 0;
	for(int  n=0; n<MAX_INVENTORY; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(14,21) )
			{
				count++;
				if( count >= 10 )
				{
					return TRUE;
				}
			}
		}
	}	
	return FALSE;
}

int	gObjDelete10EventChip(int aIndex)
{	
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int count = 0;
	for(int  n=0; n<MAX_INVENTORY; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(14,21) )
			{
				gObjInventoryDeleteItem(aIndex, n);				
				GCInventoryItemDeleteSend(aIndex, n, 1);

				LogAddTD("[EventChip] [%s][%s] Delete Rena (%d)", lpObj->AccountID, lpObj->Name, n);
				count++;
				if( count >= 10 )
				{
					return TRUE;
				}
			}
		}
	}	
	return FALSE;
}


void gObjSetPosition(int aIndex, int x, int y)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	PMSG_POSISTION_SET	pMove;
	pMove.h.c		 = PMHC_BYTE;

#ifdef PACKET_CHANGE	
	pMove.h.headcode = PACKET_POSITION;
#else
	pMove.h.headcode = 0x11;
#endif

	pMove.h.size     = sizeof( pMove );
	pMove.X          = (BYTE)x;
	pMove.Y			 = (BYTE)y;
	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	RecvPositionSetProc(&pMove, lpObj->m_Index);
}

#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - ������ ����߿��� �ش� �������� ������ �ľ�
int	 gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int count = 0;
	for(int  n=0; n<MAX_EQUIPMENT; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(itemtype,itemindex) )
			{
#ifdef SECOND_QUEST
				if( itemlevel != -1 && lpObj->pInventory[n].m_Level != itemlevel )
				{
					continue;
				}
#endif
				count++;
			}
		}
	}
	return count;
}
#endif


#ifdef SECOND_QUEST
int	 gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel)
#else
int	 gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex)
#endif
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int count = 0;
	for(int  n=0; n<MAX_INVENTORY; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(itemtype,itemindex) )
			{
#ifdef SECOND_QUEST
				if( itemlevel != -1 && lpObj->pInventory[n].m_Level != itemlevel )
				{
					continue;
				}
#endif
				count++;
			}
		}
	}
	return count;
}

int		gObjGetItemCountInIventory(int aIndex, int itemnum)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int count = 0;
	for(int  n=0; n<MAX_INVENTORY; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == itemnum )
			{
				count++;
			}
		}
	}
	return count;
}


int	gObjGetManaItemPos(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int count = 0;
	for(int  n=0; n<MAX_INVENTORY; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(14,4) || 
				lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(14,5) || 
				lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(14,6) )
			{
				return n;
			}
		}
	}
	return -1;
}

void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int delcount = 0;
	for(int  n=0; n<MAX_INVENTORY; n++)
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(itemtype,itemindex) )
			{
				gObjInventoryDeleteItem(aIndex, n);				
				GCInventoryItemDeleteSend(aIndex, n, 1);

				LogAddTD("[DeleteItem] [%s][%s] Delete (%d,%d)(%d)", 
						 lpObj->AccountID, lpObj->Name, 
						 itemtype,itemindex, n);

				delcount++;

				if( delcount >= count )
				{
					return;
				}
			}
		}
	}	
}

// ���� �ø��� �ִ� ����Ʈ ��Ȳ�� ��� �´�

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint)
#else
void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint)
#endif
{
	if( gObj[aIndex].Level <= 5 )
	{
		AddPoint = 0;
		MaxAddPoint = 0;
	}

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
#ifdef DARKLORD_WORK
	int total_point = lpObj->Strength+lpObj->Dexterity+lpObj->Vitality+lpObj->Energy+lpObj->Leadership+lpObj->LevelUpPoint;
#else
	int total_point = lpObj->Strength+lpObj->Dexterity+lpObj->Vitality+lpObj->Energy+lpObj->LevelUpPoint;
#endif
	int ori_point;
	

	// �� �ɸ����� �⺻�ɷ�ġ ���� + ���� * ������ ����Ʈ�� ���������� �ش� Ŭ����, ������ ������ ����Ʈ ������ ���.
#ifdef DARKLORD_WORK
	if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )
#else
	if( lpObj->Class == CLASS_MAGUMSA )
#endif
	{	// ���˻�� ����������Ʋ�� 7�̴�
		ori_point   = DCInfo.GetDefPoint(lpObj->Class)+((lpObj->Level-1)*7);
	}
	else
	{
		ori_point   = DCInfo.GetDefPoint(lpObj->Class)+((lpObj->Level-1)*5);
	}

#ifdef NEW_FORSKYLAND2
	// 1�� ������ �� ĳ���ʹ� +10���� ����
	if( g_QuestInfo.GetQuestState(lpObj, 0) == QS_CLEAR )
	{
		ori_point += 10;
	}

	// 2�� ������ �� ĳ���ʹ� +10���� ����
	if( g_QuestInfo.GetQuestState(lpObj, 1) == QS_CLEAR )
	{
		ori_point += 10;
	}
#endif
#ifdef SECOND_QUEST
	if( g_QuestInfo.GetQuestState(lpObj, 2) == QS_CLEAR )
	{
		ori_point += lpObj->Level-220;
	}
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� ���� ����Ʈ�� ������ �ɸ����� �ø� �� �ִ� �ִ� ��� ���� ����
	if( g_QuestInfo.GetQuestState(lpObj, 4) == QS_CLEAR )
	{
		ori_point += 20;
	}
	if( g_QuestInfo.GetQuestState(lpObj, 5) == QS_CLEAR )
	{
		ori_point += 20;
	}
	if( g_QuestInfo.GetQuestState(lpObj, 6) == QS_CLEAR )
	{
		ori_point += 30;
	}
#endif

	int addpoint = ori_point;

	// �ø��� �ִ� ��� ���� ���
	addpoint += gStatMng.GetMaxStat(lpObj->Level, lpObj->Class);

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	AddPoint		= total_point - ori_point;
	MaxAddPoint		= gStatMng.GetMaxStat(lpObj->Level, lpObj->Class);

	MinusPoint		= lpObj->iFruitPoint;
	MaxMinusPoint	= gStatMng.GetMaxMinusStat(lpObj->Level, lpObj->Class);
#else
	AddPoint = total_point - ori_point;
	MaxAddPoint = addpoint - ori_point;
#endif

}

// ��������Ʈ�� �ø��� �ִ���
int	gObjCheckStatPointUp(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	
#ifdef MODIFY_DARKLORD_ACQUIRING_EXTRA_POINT_20041117
	int total_point = lpObj->Strength+lpObj->Dexterity+lpObj->Vitality+lpObj->Energy+lpObj->Leadership+lpObj->LevelUpPoint;
#else
	int total_point = lpObj->Strength+lpObj->Dexterity+lpObj->Vitality+lpObj->Energy+lpObj->LevelUpPoint;
#endif

	int ori_point;
	

#ifdef DARKLORD_WORK
	if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )
#else
	if( lpObj->Class == CLASS_MAGUMSA )
#endif
	{	// ���˻�� ����������Ʋ�� 7�̴�
		ori_point   = DCInfo.GetDefPoint(lpObj->Class)+((lpObj->Level-1)*7);
	}
	else
	{
		ori_point   = DCInfo.GetDefPoint(lpObj->Class)+((lpObj->Level-1)*5);
	}

	// 1�� ������ �� ĳ���ʹ� +10���� ����
#ifdef NEW_FORSKYLAND2
	if( g_QuestInfo.GetQuestState(lpObj, 0) == QS_CLEAR )
	{
		ori_point += 10;
	}

	// 2�� ������ �� ĳ���ʹ� +10���� ����
	if( g_QuestInfo.GetQuestState(lpObj, 1) == QS_CLEAR )
	{
		ori_point += 10;
	}
#endif

#ifdef SECOND_QUEST
	if( g_QuestInfo.GetQuestState(lpObj, 2) == QS_CLEAR )
	{
		ori_point += lpObj->Level-220;
	}
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� ���� ����Ʈ�� ������ �ɸ����� �ø� �� �ִ� �ִ� ��� ���� ����
	if( g_QuestInfo.GetQuestState(lpObj, 4) == QS_CLEAR )
	{
		ori_point += 20;
	}
	if( g_QuestInfo.GetQuestState(lpObj, 5) == QS_CLEAR )
	{
		ori_point += 20;
	}
	if( g_QuestInfo.GetQuestState(lpObj, 6) == QS_CLEAR )
	{
		ori_point += 30;
	}
#endif	

	// �ø��� �ִ� ��� ���� ���
	ori_point += gStatMng.GetMaxStat(lpObj->Level, lpObj->Class);
	//ori_point += ((lpObj->Level*3)/100+1);
	
	if( total_point < ori_point )
	{	
		return TRUE;
	}

	return FALSE;
}

// ��������Ʈ�� ������ �ִ���
int	gObjCheckStatPointDown(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int total_point = lpObj->Strength+lpObj->Dexterity+lpObj->Vitality+lpObj->Energy+lpObj->LevelUpPoint;
	int ori_point;
	

#ifdef DARKLORD_WORK
	if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )
#else
	if( lpObj->Class == CLASS_MAGUMSA )
#endif
	{	// ���˻�� ����������Ʋ�� 7�̴�
		ori_point   = DCInfo.GetDefPoint(lpObj->Class)+((lpObj->Level-1)*7);
	}
	else
	{
		ori_point   = DCInfo.GetDefPoint(lpObj->Class)+((lpObj->Level-1)*5);
	}

	// 1�� ������ �� ĳ���ʹ� +10���� ����
#ifdef NEW_FORSKYLAND2
	if( g_QuestInfo.GetQuestState(lpObj, 0) == QS_CLEAR )
	{
		ori_point += 10;
	}

	// 2�� ������ �� ĳ���ʹ� +10���� ����
	if( g_QuestInfo.GetQuestState(lpObj, 1) == QS_CLEAR )
	{
		ori_point += 10;
	}
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� ���� ����Ʈ�� ������ �ɸ����� �ø� �� �ִ� �ִ� ��� ���� ����
	if( g_QuestInfo.GetQuestState(lpObj, 4) == QS_CLEAR )
	{
		ori_point += 20;
	}
	if( g_QuestInfo.GetQuestState(lpObj, 5) == QS_CLEAR )
	{
		ori_point += 20;
	}
	if( g_QuestInfo.GetQuestState(lpObj, 6) == QS_CLEAR )
	{
		ori_point += 30;
	}
#endif

	// �ø��� �ִ� ��� ���� ���
	ori_point += gStatMng.GetMaxStat(lpObj->Level, lpObj->Class);
	//ori_point -= ((lpObj->Level*3)/100+1);
	
	if( total_point > ori_point )
	{	
		return TRUE;
	}

	return FALSE;
}


// ���Ÿ� ����Ѵ�
void	gObjUseCircle(int aIndex, int pos)
{	
	int level = gObj[aIndex].pInventory[pos].m_Level;
	int iSerial = gObj[aIndex].pInventory[pos].m_Number;

	PMSG_DEFRESULT	pResult;
	PHeadSetB((LPBYTE)&pResult, 0x2c, sizeof( pResult ));
	pResult.result = 0;

	if( gObj[aIndex].Level <= 10 || level > 3 )
	{	// ���� 5���ϴ� �����Ҽ� ����
		pResult.result |= (0x03<<6);
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if( !gObjCheckStatPointUp(aIndex) )
	{	// ���̻� ������ �ø��� ����
		pResult.result |= (0x03<<6);
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
		
	// ������ ����

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%u", 
#else
	LogAddTD("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%d", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			 gObj[aIndex].AccountID, gObj[aIndex].Name, 
			 level, pos, iSerial);

	gObjInventoryItemSet(aIndex, pos, 0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex, pos, 1);


	short	AddPoint, MaxAddPoint;
	
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	short	MinusPoint;
	short	MaxMinusPoint;
	gObjGetStatPointState(aIndex, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
#else
	gObjGetStatPointState(aIndex, AddPoint, MaxAddPoint);
#endif

	int iSuccessRate = 0;;

	if( AddPoint <= 10 )
	{	// 10����Ʈ ������ 100%
		iSuccessRate = 100;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.1 )
	{
		iSuccessRate = 90;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.3 )
	{
		iSuccessRate = 80;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.5 )
	{
		iSuccessRate = 70;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.8 )
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}
	
	int incvalue = 0;

	if( (rand()%100) < iSuccessRate )
	{	// ���� (���� ����)
		iSuccessRate = rand()%100;
		int AddStat =  0;
		if( iSuccessRate < 70 )
		{	// 1���� ����
			AddStat = 1;
		}
		else if( iSuccessRate < 95 )
		{	// 2���� ����
			AddStat = 2;
		}		
		else
		{
			AddStat = 3;
		}

		if( AddStat+AddPoint > MaxAddPoint )
		{
			AddStat = 1;
		}

		//pResult.result |= (0x00<<6);
		// ���� Ÿ��
		pResult.result |= (level<<4);
		pResult.result |= AddStat;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		incvalue = AddStat;
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[StatUp] [%s][%s] Success [%d][%d] %u", gObj[aIndex].AccountID, gObj[aIndex].Name, level, incvalue, iSerial);
#else
		LogAddTD("[StatUp] [%s][%s] Success [%d][%d] %d", gObj[aIndex].AccountID, gObj[aIndex].Name, level, incvalue, iSerial);
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
	}
	else
	{	// ���д� ������ �������� ����	
		pResult.result |= (0x01<<6);
		// ���� Ÿ��
		pResult.result |= (level<<4);		
		pResult.result |= 0;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[StatUp] [%s][%s] Fail [%d] %u", gObj[aIndex].AccountID, gObj[aIndex].Name, level, iSerial);
#else
		LogAddTD("[StatUp] [%s][%s] Fail [%d] %d", gObj[aIndex].AccountID, gObj[aIndex].Name, level, iSerial);
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		return;
	}	

	switch( level )
	{
	case 3 : // ��
		gObj[aIndex].Strength += incvalue;
		break;
	case 2 : // ��ø
		gObj[aIndex].Dexterity += incvalue;
		break;
	case 1 : // ü��
		gObj[aIndex].Vitality += incvalue;
		gObj[aIndex].MaxLife += (gObj[aIndex].VitalityToLife*incvalue);
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(gObj[aIndex].m_Index, (WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife), 0xFE, 0, (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield));
#else
		GCReFillSend(gObj[aIndex].m_Index, (WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife), 0xFE, 0);
#endif
		break;
	case 0 : // ������
		gObj[aIndex].Energy += incvalue;
		gObj[aIndex].MaxMana += (gObj[aIndex].EnergyToMana*incvalue);

#ifdef NEW_FORSKYLAND2
		gObjSetBP(aIndex);
		GCManaSend(gObj[aIndex].m_Index, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP);
#else
		GCManaSend(gObj[aIndex].m_Index, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, 0);
#endif
		break;
	}
	// �����Ѵ�
	gObjCalCharacter(aIndex);
}

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712

void gObjUsePlusStatFruit(int aIndex, int pos)
{	
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
		return;

	if( gObj[aIndex].Class >= MAX_CLASSTYPE || gObj[aIndex].Class < 0 )
		return;
	
	int iItemLevel		= gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial		= gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;

	PMSG_USE_STAT_FRUIT	pResult;
	PHeadSetB((LPBYTE)&pResult, 0x2C, sizeof( pResult ));
	pResult.result		= STAT_PLUS_PREVENTED;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue	= 0;

	// 1. >  ���� ��� ĳ������ �˻� & ���� ���� �˻�
	if( gObj[aIndex].Level <= 9 || iItemLevel > FRUIT_LEADERSHIP )
	{	
		pResult.result = STAT_PLUS_PREVENTED;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// 1.1 > ��ũ�ε尡 �ƴ� Ŭ������ ��ֿ��� ���� �� ������ ����
	if( iItemLevel==FRUIT_LEADERSHIP && gObj[aIndex].Class!=CLASS_DARKLORD )
	{
		pResult.result = STAT_PLUS_PREVENTED;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// 1.2 > �������� �����ϰ� �ִ��� �˻�.
	for( int iItemIndex = 0; iItemIndex < MAX_EQUIPMENT; iItemIndex++ )
	{
		if( gObj[aIndex].pInventory[iItemIndex].IsItem() )
		{
			pResult.result = STAT_FRUIT_USING_PREVENTED_BY_ITEMEQUIPMENT;
			DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}


	// 2. >  ���� ������ ���Ÿ� ����� �� �ִ��� �˻�
	if( !gObjCheckStatPointUp(aIndex) )
	{	
		pResult.result = STAT_PLUS_PREVENTED_BY_MAX;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
		
	// 3. >  ������ ����
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%u", 
#else
	LogAddTD("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%d", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			 gObj[aIndex].AccountID, gObj[aIndex].Name, 
			 iItemLevel, pos, iItemSerial);

	gObjInventoryItemSet(aIndex, pos, 0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex, pos, 1);

	// 4. >	 ���� ��� ���� Ȯ�� ó��
	short	AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint;
	gObjGetStatPointState(aIndex, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);

	int iSuccessRate = 0;

#ifdef UPDATE_FRUITPOINT_SUCCESSRATE_FIX_200809017	// �÷������� ����Ȯ��
	if( AddPoint <= 10 )
	{	// 10 ����Ʈ ������ 100%
		iSuccessRate = 100;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.1 )
	{
		iSuccessRate = 70;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.3 )
	{
		iSuccessRate = 60;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.5 )
	{
		iSuccessRate = 50;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.8 )
	{
		iSuccessRate = 40;
	}
	else
	{
		iSuccessRate = 30;
	}
#else	// UPDATE_FRUITPOINT_SUCCESSRATE_FIX_200809017	
	if( AddPoint <= 10 )
	{	// 10 ����Ʈ ������ 100%
		iSuccessRate = 100;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.1 )
	{
		iSuccessRate = 90;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.3 )
	{
		iSuccessRate = 80;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.5 )
	{
		iSuccessRate = 70;
	}
	else if( (AddPoint-10) < MaxAddPoint*0.8 )
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}
#endif	// UPDATE_FRUITPOINT_SUCCESSRATE_FIX_200809017
	
	INT  iIncStat = 0;
	BOOL bSuccess = FALSE;

	if( (rand()%100) < iSuccessRate )
		bSuccess = TRUE;

	if( bSuccess )
	{	// ���� (���� ����)
		iSuccessRate = rand()%100;
		int AddStat =  0;
		if( iSuccessRate < 70 )
		{	// 1���� ����
			AddStat = 1;
		}
		else if( iSuccessRate < 95 )
		{	// 2���� ����
			AddStat = 2;
		}		
		else
		{
			AddStat = 3;
		}

		if( AddStat+AddPoint > MaxAddPoint )
		{
			AddStat = 1;
		}

		pResult.result		= STAT_PLUS_SUCCESS;
		pResult.btStatValue = AddStat;
		pResult.btFruitType = iItemLevel;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		iIncStat = AddStat;
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[StatUp] [%s][%s] Success [%d][%d] %u", gObj[aIndex].AccountID, gObj[aIndex].Name, iItemLevel, iIncStat, iItemSerial);
#else
		LogAddTD("[StatUp] [%s][%s] Success [%d][%d] %d", gObj[aIndex].AccountID, gObj[aIndex].Name, iItemLevel, iIncStat, iItemSerial);
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
	}
	else
	{		
		pResult.result		= STAT_PLUS_FAILED;	
		pResult.btStatValue = 0;
		pResult.btFruitType = iItemLevel;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[StatUp] [%s][%s] Fail [%d] %u", gObj[aIndex].AccountID, gObj[aIndex].Name, iItemLevel, iItemSerial);
#else
		LogAddTD("[StatUp] [%s][%s] Fail [%d] %d", gObj[aIndex].AccountID, gObj[aIndex].Name, iItemLevel, iItemSerial);
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		return;
	}	

	// 6. >  ĳ���Ϳ� ���� ���Ȱ��� �����Ѵ�.
	switch( iItemLevel )
	{
	case FRUIT_LEADERSHIP	: // ���
		gObj[aIndex].Leadership += iIncStat;
		break;
	case FRUIT_STRENGTH		: // ��
		gObj[aIndex].Strength	+= iIncStat;
		break;
	case FRUIT_DEXTERITY	: // ��ø
		gObj[aIndex].Dexterity	+= iIncStat;
		break;
	case FRUIT_VITALITY		: // ü��
		gObj[aIndex].Vitality	+= iIncStat;
		gObj[aIndex].MaxLife	+= (gObj[aIndex].VitalityToLife*iIncStat);
#ifndef ADD_SHIELD_POINT_01_20060403
		// �ߺ��Ǵ� �ڵ� ����
		GCReFillSend(gObj[aIndex].m_Index, (WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife), 0xFE, 0);
#endif
		break;
	case FRUIT_ENERGY		: // ������
		gObj[aIndex].Energy		+= iIncStat;
		gObj[aIndex].MaxMana	+= (gObj[aIndex].EnergyToMana*iIncStat);
		gObjSetBP(aIndex);
#ifndef ADD_SHIELD_POINT_01_20060403
		GCManaSend(gObj[aIndex].m_Index, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP);				
#endif
		break;
	}
	
	// 8. >  ĳ���� ������ �����Ѵ�
	gObjCalCharacter(aIndex);

// CODEREVIEW_SHIELD : #ifndef����  GCReFillSend(), gObjSetBP(), GCManaSend() �ؿ��� �ϱ�
#ifdef ADD_SHIELD_POINT_01_20060403
	GCReFillSend( gObj[aIndex].m_Index, (WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife), 0xFE, 0, (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield));
	gObjSetBP(aIndex);
	GCManaSend(gObj[aIndex].m_Index, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, (gObj[aIndex].MaxBP+gObj[aIndex].AddBP));
#endif
}

void gObjUseMinusStatFruit(int aIndex, int pos )
{
	if( !CHECK_LIMIT(aIndex, MAX_OBJECT) )
		return;

	if( gObj[aIndex].Class >= MAX_CLASSTYPE || gObj[aIndex].Class < 0 )
		return;
	
	int iItemLevel		= gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial		= gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;

	PMSG_USE_STAT_FRUIT	pResult;
	PHeadSetB((LPBYTE)&pResult, 0x2C, sizeof( pResult ));
	pResult.result		= STAT_MINUS_PREVENTED;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue	= 0;

	// 1. >  ���� ��� ĳ������ �˻� & ���� ���� �˻�
	if( gObj[aIndex].Level <= 9 || iItemLevel > FRUIT_LEADERSHIP )
	{	
		pResult.result = STAT_MINUS_PREVENTED;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// 1.1 > ��ũ�ε尡 �ƴ� Ŭ������ ��ֿ��� ���� �� ������ ����
	if( iItemLevel==FRUIT_LEADERSHIP && gObj[aIndex].Class!=CLASS_DARKLORD )
	{
		pResult.result = STAT_MINUS_PREVENTED;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	// 1.2 > �������� �����ϰ� �ִ��� �˻�.
	for( int iItemIndex = 0; iItemIndex < MAX_EQUIPMENT; iItemIndex++ )
	{
		if( gObj[aIndex].pInventory[iItemIndex].IsItem() )
		{
			pResult.result = STAT_FRUIT_USING_PREVENTED_BY_ITEMEQUIPMENT;
			DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	// 2. >  ���� ���ҿ� ���Ÿ� ����� �� �ִ��� �˻�
	INT iDefaultStat  = 0;
	INT iPresentStat  = 0;
	BOOL bEnableUseFruit = TRUE;

	if( iItemLevel == FRUIT_ENERGY )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Energy;
		iPresentStat = gObj[aIndex].Energy;
		
		if( iPresentStat <= iDefaultStat )
			bEnableUseFruit = FALSE;
	}
	else if( iItemLevel == FRUIT_VITALITY )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Vitality;
		iPresentStat = gObj[aIndex].Vitality;

		if( iPresentStat <= iDefaultStat )
			bEnableUseFruit = FALSE;
	}
	else if( iItemLevel == FRUIT_DEXTERITY )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Dexterity;
		iPresentStat = gObj[aIndex].Dexterity;
			
		if( iPresentStat <= iDefaultStat )
			bEnableUseFruit = FALSE;
	}
	else if( iItemLevel == FRUIT_STRENGTH )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Strength;
		iPresentStat = gObj[aIndex].Strength;

		if( iPresentStat <= iDefaultStat )
			bEnableUseFruit = FALSE;
	}
	else if( iItemLevel == FRUIT_LEADERSHIP )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Leadership;
		iPresentStat = gObj[aIndex].Leadership;

		if( iPresentStat <= iDefaultStat )
			bEnableUseFruit = FALSE;
	}

	if( bEnableUseFruit == FALSE )
	{
//		MsgOutput( aIndex, "�ش� ������Ʈ�� �� �̻� ���� ��ų �� �����ϴ�.");
		pResult.result = STAT_MINUS_PREVENTED_BY_DEFAULT;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	// 2.2 >  ���� ���ҿ� ���Ÿ� ����� �� �ִ��� �˻�
	short	AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint = 0;
	gObjGetStatPointState(aIndex, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);

	if( MinusPoint >= MaxMinusPoint || MinusPoint < 0 )
	{
		pResult.result = STAT_MINUS_PREVENTED_BY_MAX;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	
	// 3. >  ������ ����
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[StatDown] [%s][%s] Use StatDownItem Level:%d Pos:%d serial:%u", 
#else
	LogAddTD("[StatDown] [%s][%s] Use StatDownItem Level:%d Pos:%d serial:%d", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			 gObj[aIndex].AccountID, gObj[aIndex].Name, iItemLevel, pos, iItemSerial);

	gObjInventoryItemSet(aIndex, pos, 0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex, pos, 1);
	

	
	// 4. >	 ���� ��� ���� Ȯ�� ó��
	INT  iDecStat = 0;
	BOOL bSuccess = FALSE;

#ifdef UPDATE_FRUITPOINT_SUCCESSRATE_FIX_200809017	// �÷������� ����Ȯ��
	int iSuccessRate = 0;

	if( MinusPoint <= 10 )
	{	// 10 ����Ʈ ������ 100%
		iSuccessRate = 100;
	}
	else if( (MinusPoint-10) < MaxMinusPoint*0.1 )
	{
		iSuccessRate = 70;
	}
	else if( (MinusPoint-10) < MaxMinusPoint*0.3 )
	{
		iSuccessRate = 60;
	}
	else if( (MinusPoint-10) < MaxMinusPoint*0.5 )
	{
		iSuccessRate = 50;
	}
	else if( (MinusPoint-10) < MaxMinusPoint*0.8 )
	{
		iSuccessRate = 40;
	}
	else
	{
		iSuccessRate = 30;
	}

	if( (rand()%100) < iSuccessRate )
		bSuccess = TRUE;
#else	// UPDATE_FRUITPOINT_SUCCESSRATE_FIX_200809017
	INT	iSuccessRate = 100 - (gObj[aIndex].Level/6);
	if( iSuccessRate < 0 ) iSuccessRate = 0;

	// 4.1 > ���� ���ɼ� üũ
	if( (rand()%100) < iSuccessRate )
		bSuccess = TRUE;

	// 4.2 > -10 ����Ʈ������ 100% ����
	if( MinusPoint <= 10 )
		bSuccess = TRUE;
#endif	// UPDATE_FRUITPOINT_SUCCESSRATE_FIX_200809017
	
	// 5. >	 ���� ��� ó��
	if( bSuccess )
	{	
		iSuccessRate = rand()%100;

#ifdef MODIFY_MINUS_FRUIT_POINT_20060622
		if( iSuccessRate < 50 )
		{	// 1 ���� ����
			iDecStat = 1;
		}
		else if( iSuccessRate < 75 )
		{	// 3 ���� ����
			iDecStat = 3;
		}		
		else if( iSuccessRate < 91 )
		{	// 5 ���� ����
			iDecStat = 5;
		}
		else if( iSuccessRate < 98 )
		{
			// 7 ���� ����
			iDecStat = 7;
		}
		else
		{
			// 9 ���� ����
			iDecStat = 9;
		}
#else
		if( iSuccessRate < 50 )
		{	// 1 ���� ����
			iDecStat = 1;
		}
		else if( iSuccessRate < 75 )
		{	// 2 ���� ����
			iDecStat = 2;
		}		
		else if( iSuccessRate < 91 )
		{	// 3 ���� ����
			iDecStat = 3;
		}
		else if( iSuccessRate < 98 )
		{
			// 4 ���� ����
			iDecStat = 4;
		}
		else
		{
			// 5 ���� ����
			iDecStat = 5;
		}
#endif // MODIFY_MINUS_FRUIT_POINT_20060622


		// 5.1 > �ּ� �� ó�� 
		if( MinusPoint + iDecStat >= MaxMinusPoint )
		{
			iDecStat = MaxMinusPoint - MinusPoint;
 		}
		
		// 5.2 > �⺻ ���� �ּ� �� ó��
		if( iPresentStat - iDecStat < iDefaultStat )
		{
			iDecStat = iPresentStat - iDefaultStat;
		}

		pResult.result = STAT_MINUS_SUCCESS;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = iDecStat;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[StatDown] [%s][%s] Success [%d][%d] %u", gObj[aIndex].AccountID, gObj[aIndex].Name, iItemLevel, iDecStat, iItemSerial);
#else
		LogAddTD("[StatDown] [%s][%s] Success [%d][%d] %d", gObj[aIndex].AccountID, gObj[aIndex].Name, iItemLevel, iDecStat, iItemSerial);
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
	}
	else
	{			
		pResult.result = STAT_MINUS_FAILED;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = 0;
		DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[StatDown] [%s][%s] Fail [%d] %u", gObj[aIndex].AccountID, gObj[aIndex].Name, iItemLevel, iItemSerial);
#else
		LogAddTD("[StatDown] [%s][%s] Fail [%d] %d", gObj[aIndex].AccountID, gObj[aIndex].Name, iItemLevel, iItemSerial);
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		return;
	}	

	

	// 6. >  ĳ���Ϳ� ���ҽ��Ȱ��� �����Ѵ�.
	switch( iItemLevel )
	{
	case FRUIT_LEADERSHIP	: // ���
		gObj[aIndex].Leadership -= iDecStat;
		break;
	case FRUIT_STRENGTH		: // ��
		gObj[aIndex].Strength	-= iDecStat;
		break;
	case FRUIT_DEXTERITY	: // ��ø
		gObj[aIndex].Dexterity	-= iDecStat;
		break;
	case FRUIT_VITALITY		: // ü��
		gObj[aIndex].Vitality	-= iDecStat;
		gObj[aIndex].MaxLife	-= (gObj[aIndex].VitalityToLife*iDecStat);
#ifndef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(gObj[aIndex].m_Index, (WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife), 0xFE, 0);
#endif
		break;
	case FRUIT_ENERGY		: // ������
		gObj[aIndex].Energy		-= iDecStat;
		gObj[aIndex].MaxMana	-= (gObj[aIndex].EnergyToMana*iDecStat);
		gObjSetBP(aIndex);
#ifndef ADD_SHIELD_POINT_01_20060403
		GCManaSend(gObj[aIndex].m_Index, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP);
#endif
		break;
	}



	// 7. >  LevelUpPoint �� �ٲ��ش�.
	INT iOldLevelUpPoint = gObj[aIndex].LevelUpPoint;
	gObj[aIndex].LevelUpPoint += iDecStat;

	// 8. >	 FruitPoint �� ���� ���� �ش�.
#ifdef EXTEND_LOG_SYSTEM_08_20070810
	INT iOldFruitPoint = gObj[aIndex].iFruitPoint;
#else
	INT iOldFruitPoint = gObj[aIndex].LevelUpPoint;
#endif // EXTEND_LOG_SYSTEM_08_20070810
	gObj[aIndex].iFruitPoint += iDecStat;

	LogAddTD("[StatDown] [%s][%s] MinusStat[%d] -> LevelUpPoint Old(%d)/New(%d)  FruitPoint Old(%d)/New(%d)", 
		gObj[aIndex].AccountID, gObj[aIndex].Name, iDecStat, iOldLevelUpPoint, gObj[aIndex].LevelUpPoint, iOldFruitPoint, gObj[aIndex].iFruitPoint);


	
	// 8. >  ĳ���� ������ �����Ѵ�
	gObjCalCharacter(aIndex);	

#ifdef ADD_SHIELD_POINT_01_20060403
	GCReFillSend(gObj[aIndex].m_Index, (WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife), 0xFE, 0, (WORD)(gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield));
	gObjSetBP(aIndex);
	GCManaSend(gObj[aIndex].m_Index, (short)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana), 0xFE, 0, gObj[aIndex].MaxBP);
#endif

}
#endif

#ifdef ADD_PCS_FRUITS_POINT_EXTENTION_20070430
void gObjUseExMinusFruit(LPOBJECTSTRUCT lpObj, int iPos)
{
	if( lpObj->Class >= MAX_CLASSTYPE || lpObj->Class < 0 )
	{
		return;
	}

	// ������ Ÿ��, �������� * 10���� ���� ��ġ�� �����Ѵ�.
	int iItemLevel			= 0;
	int iItemSerial			= lpObj->pInventory[iPos].m_Number;
	int iDecreaseStatPoint	= ( lpObj->pInventory[iPos].m_Durability * 10 );
	int iCharacterClass		= lpObj->Class;

	// ������ �������� ���ҵǴ� ������ ������ Ʋ����.
	switch( lpObj->pInventory[iPos].m_Type )
	{
	case MAKE_ITEMNUM2( 13, 54 ):
		iItemLevel = FRUIT_STRENGTH;
		break;
	case MAKE_ITEMNUM2( 13, 55 ):
		iItemLevel = FRUIT_DEXTERITY;
		break;
	case MAKE_ITEMNUM2( 13, 56 ):
		iItemLevel = FRUIT_VITALITY;
		break;
	case MAKE_ITEMNUM2( 13, 57 ):
		iItemLevel = FRUIT_ENERGY;
		break;
	case MAKE_ITEMNUM2( 13, 58 ):
		iItemLevel = FRUIT_LEADERSHIP;
		break;
	}

	PMSG_USE_STAT_FRUIT	pResult;
	PHeadSetB((LPBYTE)&pResult, 0x2C, sizeof( pResult ));
#ifdef MODIFY_RESET_FRUITS_RESULT_TYPE_20070821
	pResult.result		= STAT_MINUS_EX_PREVENTED;
#else
	pResult.result		= STAT_MINUS_PREVENTED;
#endif // MODIFY_RESET_FRUITS_RESULT_TYPE_20070821
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue	= 0;

	// 1-1. �������� �����ϰ� �ִ��� �˻�.
	for( int iItemIndex = 0; iItemIndex < MAX_EQUIPMENT; iItemIndex++ )
	{
		if( lpObj->pInventory[iItemIndex].IsItem() )
		{
			pResult.result = STAT_FRUIT_USING_PREVENTED_BY_ITEMEQUIPMENT;
			DataSend( lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	// 1-2. ���� ���ҿ� ���Ÿ� ����� �� �ִ��� �˻�
	int iDefaultStat  = 0;
	int iPresentStat  = 0;
	BOOL bEnableUseFruit = TRUE;

#ifdef MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
	switch( iItemLevel )
	{
	case FRUIT_STRENGTH:
		if( lpObj->Strength == DCInfo.DefClass[iCharacterClass].Strength )
		{
			bEnableUseFruit = FALSE;
		}
		break;
	case FRUIT_DEXTERITY:
		if( lpObj->Dexterity == DCInfo.DefClass[iCharacterClass].Dexterity )
		{
			bEnableUseFruit = FALSE;
		}
		break;
	case FRUIT_VITALITY:
		if( lpObj->Vitality == DCInfo.DefClass[iCharacterClass].Vitality )
		{
			bEnableUseFruit = FALSE;
		}
		break;
	case FRUIT_ENERGY:
		if( lpObj->Energy == DCInfo.DefClass[iCharacterClass].Energy )
		{
			bEnableUseFruit = FALSE;
		}
		break;
	case FRUIT_LEADERSHIP:
		if( lpObj->Leadership == DCInfo.DefClass[iCharacterClass].Leadership )
		{
			bEnableUseFruit = FALSE;
		}
		break;
	}
#endif // MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806

	if( iItemLevel == FRUIT_ENERGY )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Energy;
		iPresentStat = lpObj->Energy;

		if( ( iPresentStat - iDecreaseStatPoint ) < iDefaultStat )
		{
#ifdef MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
			iDecreaseStatPoint = iPresentStat - iDefaultStat;
#else
			bEnableUseFruit = FALSE;
#endif // MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
		}
	}
	else if( iItemLevel == FRUIT_VITALITY )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Vitality;
		iPresentStat = lpObj->Vitality;

		if( ( iPresentStat - iDecreaseStatPoint ) < iDefaultStat )
		{
#ifdef MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
			iDecreaseStatPoint = iPresentStat - iDefaultStat;
#else
			bEnableUseFruit = FALSE;
#endif // MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
		}
	}
	else if( iItemLevel == FRUIT_DEXTERITY )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Dexterity;
		iPresentStat = lpObj->Dexterity;
			
		if( ( iPresentStat - iDecreaseStatPoint ) < iDefaultStat )
		{
#ifdef MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
			iDecreaseStatPoint = iPresentStat - iDefaultStat;
#else
			bEnableUseFruit = FALSE;
#endif // MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
		}
	}
	else if( iItemLevel == FRUIT_STRENGTH )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Strength;
		iPresentStat = lpObj->Strength;

		if( ( iPresentStat - iDecreaseStatPoint ) < iDefaultStat )
		{
#ifdef MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
			iDecreaseStatPoint = iPresentStat - iDefaultStat;
#else
			bEnableUseFruit = FALSE;
#endif // MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
		}
	}
	else if( iItemLevel == FRUIT_LEADERSHIP )
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Leadership;
		iPresentStat = lpObj->Leadership;

		if( ( iPresentStat - iDecreaseStatPoint ) < iDefaultStat )
		{
#ifdef MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
			iDecreaseStatPoint = iPresentStat - iDefaultStat;
#else
			bEnableUseFruit = FALSE;
#endif // MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
		}
	}

	if( bEnableUseFruit == FALSE )
	{
		pResult.result = STAT_MINUS_PREVENTED_BY_DEFAULT;
		DataSend( lpObj->m_Index , (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// 2. ������ ����
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[CashShop][ExMinusStatFruit] [%s][%s] Use StatDownItem Level:%d,Pos:%d,Dur:%d,Serial:%u",
#else
	LogAddTD("[CashShop][ExMinusStatFruit] [%s][%s] Use StatDownItem Level:%d,Pos:%d,Dur:%d,Serial:%d",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			 lpObj->AccountID, lpObj->Name, iItemLevel, iPos, lpObj->pInventory[iPos].m_Durability, iItemSerial);

	gObjInventoryItemSet(lpObj->m_Index, iPos, 0xFF);
	lpObj->pInventory[iPos].Clear();
	GCInventoryItemDeleteSend(lpObj->m_Index, iPos, 1);

#ifdef MODIFY_RESET_FRUITS_RESULT_TYPE_20070821
	pResult.result = STAT_MINUS_EX_SUCCESS;
#else
	pResult.result = STAT_MINUS_SUCCESS;
#endif // MODIFY_RESET_FRUITS_RESULT_TYPE_20070821
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = iDecreaseStatPoint;
	DataSend( lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

	// 3. ĳ���Ϳ� ���ҽ��Ȱ��� �����Ѵ�.
	switch( iItemLevel )
	{
	case FRUIT_LEADERSHIP	: // ���
		lpObj->Leadership	-= iDecreaseStatPoint;
		break;
	case FRUIT_STRENGTH		: // ��
		lpObj->Strength		-= iDecreaseStatPoint;
		break;
	case FRUIT_DEXTERITY	: // ��ø
		lpObj->Dexterity	-= iDecreaseStatPoint;
		break;
	case FRUIT_VITALITY		: // ü��
		lpObj->Vitality		-= iDecreaseStatPoint;
		lpObj->MaxLife		-= (lpObj->VitalityToLife*iDecreaseStatPoint);
#ifndef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0);
#endif
		break;
	case FRUIT_ENERGY		: // ������
		lpObj->Energy		-= iDecreaseStatPoint;
		lpObj->MaxMana		-= (lpObj->EnergyToMana*iDecreaseStatPoint);
		gObjSetBP(lpObj->m_Index);
#ifndef ADD_SHIELD_POINT_01_20060403
		GCManaSend(lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP);
#endif
		break;
	}

	// 4. LevelUpPoint �� �ٲ��ش�.
	INT iOldLevelUpPoint	= lpObj->LevelUpPoint;
	lpObj->LevelUpPoint		+= iDecreaseStatPoint;

	LogAddTD("[CashShop][ExMinusStatFruit] [%s][%s] MinusStat[%d] -> LevelUpPoint Old(%d)/New(%d)", 
		lpObj->AccountID, lpObj->Name, iDecreaseStatPoint, iOldLevelUpPoint, lpObj->LevelUpPoint );

	// 5. ĳ���� ������ �����Ѵ�
	gObjCalCharacter(lpObj->m_Index);	

#ifdef ADD_SHIELD_POINT_01_20060403
	GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
	gObjSetBP(lpObj->m_Index);
	GCManaSend(lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP);
#endif
}
#endif // ADD_PCS_FRUITS_POINT_EXTENTION_20070430

#ifdef ADD_PCS_IMPROVE_POTION_20070718
void gObjUseImprovePotion(LPOBJECTSTRUCT lpObj, int iPos)
{
	if( lpObj == NULL )
	{
		return;
	}

	if( lpObj->Type != OBJTYPE_CHARACTER )
	{
		return;
	}

	int iItemCode = 0;

	iItemCode = lpObj->pInventory[iPos].m_Type;

	// ����� ����� ��ũ�ε尡 �ƴϸ� ����� �ȵǵ��� ����
	if( iItemCode == MAKE_ITEMNUM( 14, 82 ) )
	{
		if( lpObj->Class != CLASS_DARKLORD )
		{
			// ��� ��� ������
			GCItemDurSend( lpObj->m_Index, iPos, (BYTE)lpObj->pInventory[iPos].m_Durability, 1);
			return;
		}
	}

	g_CashItemPeriodSystem.GDReqPeriodItemInsert( lpObj, iItemCode, ITEM_EFFECTDATE_USE_FILE );

	// ������ ���� ���� �� ����
	lpObj->pInventory[iPos].m_Durability--;
	if( lpObj->pInventory[iPos].m_Durability > 0 )
	{
		GCItemDurSend( lpObj->m_Index, iPos, (BYTE)lpObj->pInventory[iPos].m_Durability, 1);
	}
	else
	{
		gObjInventoryItemSet(lpObj->m_Index, iPos, 0xFF);
		lpObj->pInventory[iPos].Clear();
		GCInventoryItemDeleteSend(lpObj->m_Index, iPos, 1);
	}
}
#endif // ADD_PCS_IMPROVE_POTION_20070718

#ifdef ADD_FULL_RECOVERY_POTION_20070719
void gObjUseFullRecoveryPotion(LPOBJECTSTRUCT lpObj, int iPos)
{
	if( lpObj == NULL )
	{
		return;
	}

	if( lpObj->Type != OBJTYPE_CHARACTER )
	{
		return;
	}

	if( lpObj->pInventory[iPos].m_Type == MAKE_ITEMNUM( 14, 70 ) )		// ä�� ȸ��
	{
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
		GCReFillSend( lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 1, (WORD)lpObj->iShield);
	}
	else																// ���� ȸ��
	{
		lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		GCManaSend( lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP );
	}

	// ������ ���� ���� �� ����
	lpObj->pInventory[iPos].m_Durability--;
	if( lpObj->pInventory[iPos].m_Durability > 0 )
	{
		GCItemDurSend( lpObj->m_Index, iPos, (BYTE)lpObj->pInventory[iPos].m_Durability, 1);
	}
	else
	{
		gObjInventoryItemSet(lpObj->m_Index, iPos, 0xFF);
		lpObj->pInventory[iPos].Clear();
		GCInventoryItemDeleteSend(lpObj->m_Index, iPos, 1);
	}
}
#endif // ADD_FULL_RECOVERY_POTION_20070719

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
// (100% - 1)�ۼ�Ʈ�� ������ ���
void gObjUseRecoveryPotion(LPOBJECTSTRUCT lpObj, int iPos, double dRate)
{
	if( lpObj == NULL )
	{
		return;
	}
	
	if( lpObj->Type != OBJTYPE_CHARACTER )
	{
		return;
	}
	
	if( lpObj->pInventory[iPos].m_Type == MAKE_ITEMNUM( 14, 94 ) )		// ü�� ȸ��
	{
		float fMaxLife = lpObj->MaxLife + lpObj->AddLife;
		lpObj->Life += fMaxLife * dRate;
		if (lpObj->Life > fMaxLife)	lpObj->Life = fMaxLife;
		GCReFillSend( lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 1, (WORD)lpObj->iShield);
	}
#ifdef ADD_ELITE_SD_POTION_20090918
	else if( lpObj->pInventory[iPos].m_Type == MAKE_ITEMNUM( 14, 133 ) )	// SD ȸ��
	{
		float fMaxShield = lpObj->iMaxShield + lpObj->iAddShield;
		lpObj->iShield += fMaxShield * dRate;
		if (lpObj->iShield > fMaxShield)	lpObj->iShield = fMaxShield;
		GCReFillSend( lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 1, (WORD)lpObj->iShield);
	}
#endif // ADD_ELITE_SD_POTION_20090918
	else																// ���� ȸ��
	{
		float fMaxMana = lpObj->MaxMana + lpObj->AddMana;
		lpObj->Mana += fMaxMana * dRate;
		if (lpObj->Mana > fMaxMana)	lpObj->Mana = fMaxMana;
		GCManaSend( lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP );
	}
	
	// ������ ���� ���� �� ����
	lpObj->pInventory[iPos].m_Durability--;
	if( lpObj->pInventory[iPos].m_Durability > 0 )
	{
		GCItemDurSend( lpObj->m_Index, iPos, (BYTE)lpObj->pInventory[iPos].m_Durability, 1);
	}
	else
	{
		gObjInventoryItemSet(lpObj->m_Index, iPos, 0xFF);
		lpObj->pInventory[iPos].Clear();
		GCInventoryItemDeleteSend(lpObj->m_Index, iPos, 1);
	}
}
#else // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318


#ifdef ADD_PCS_INDULGENCE_20070504
void gObjUseIndulgence(LPOBJECTSTRUCT lpObj, int iPos)
{
	// 1. ����� �˻縦 �Ѵ�.
	if( lpObj->pInventory[iPos].m_Type != MAKE_ITEMNUM( 14, 72 ) )
	{
		return;
	}

	if( lpObj->pInventory[iPos].m_Durability <= 0 )
	{
		return;
	}

	// ���� ��ġ�� �������� �����Ѵ�. (PK_Time�� �� ������ ������ 60�� �����ش�.)
	int iReducePoint = 0;

	iReducePoint = (int)lpObj->pInventory[iPos].m_Durability * 60;

	// PK������ ���� ����ڴ� ������� ���Ѵ�.
	if( lpObj->m_PK_Level <= PK_LEVEL_DEFAULT )
	{
		return;
	}

	gObjPkDownTimeCheck( lpObj, iReducePoint );
/*
	// ���� ��ġ�� ���� ��ġ���� ������ ������ ���߰� ���� ���� �ð����� ���ҽ�Ų��.
	if( lpObj->m_PK_Count > 3 )		// PK_Count�� 3 �̻��� ��� �ѽð� ���� ���� ��Ų��.
	{
		lpObj->m_PK_Time += iReducePoint;
		if( lpObj->m_PK_Time > PK_LEVEL_DOWN_TIME1 )
		{
			lpObj->m_PK_Count--;
			lpObj->m_PK_Time = lpObj->m_PK_Time - PK_LEVEL_DOWN_TIME1;
		}
	}
	else							// PK_Count�� 3 ������ ��� 3�ð� ���� ���� ��Ų��.
	{
		lpObj->m_PK_Time += iReducePoint;
		if( lpObj->m_PK_Time > PK_LEVEL_DOWN_TIME )
		{
			lpObj->m_PK_Count--;
			lpObj->m_PK_Time = lpObj->m_PK_Time - PK_LEVEL_DOWN_TIME;
		}
	}

	// 2. ������� PK ī��Ʈ�� 
	lpObj->m_PK_Level;
	lpObj->m_PK_Time;
	lpObj->m_PK_Count;
*/
	gObjInventoryItemSet(lpObj->m_Index, iPos, 0xFF);
	lpObj->pInventory[iPos].Clear();
	GCInventoryItemDeleteSend(lpObj->m_Index, iPos, 1);

	// �ʱ� ����
	//lpObj->m_PK_Count	= 0;
	//lpObj->m_PK_Level	= PK_LEVEL_DEFAULT;
	//lpObj->m_PK_Time	= 0;
}
#endif // ADD_PCS_INDULGENCE_20070504

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - m_MaxLifePower
void gObjCalcMaxLifePower( int aIndex )
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int iTotalLevel = lpObj->Level + lpObj->m_nMasterLevel;

	switch( lpObj->Class )
	{
		case CLASS_WIZARD:
			lpObj->m_MaxLifePower = 110 + iTotalLevel + lpObj->Vitality;			
			break;

		case CLASS_KNIGHT:
			lpObj->m_MaxLifePower = 60 + ( iTotalLevel * 2 ) + lpObj->Vitality;
			break;

		case CLASS_ELF:			
			lpObj->m_MaxLifePower = 80 + iTotalLevel + lpObj->Vitality;		
			break;

		case CLASS_DARKLORD:		
		case CLASS_MAGUMSA:			
			lpObj->m_MaxLifePower = 110 + iTotalLevel + lpObj->Vitality;			
		break;

		default:
			lpObj->m_MaxLifePower = 0;
		break;
	}
}

#else	// MASTER_LEVEL_UP_SYSTEM_20070912
void gObjCalcMaxLifePower(int aIndex)
{
#ifdef NEW_FORSKYLAND2
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	switch( lpObj->Class )
	{
		case CLASS_WIZARD:
				lpObj->m_MaxLifePower = 110+lpObj->Level+lpObj->Vitality;			
		break;

		case CLASS_KNIGHT:
				lpObj->m_MaxLifePower = 60+lpObj->Level*2+lpObj->Vitality;
		break;

		case CLASS_ELF:			
				lpObj->m_MaxLifePower = 80+lpObj->Level+lpObj->Vitality;		
		break;

#ifdef DARKLORD_WORK
		case CLASS_DARKLORD:		
#endif	
		case CLASS_MAGUMSA:			
				lpObj->m_MaxLifePower = 110+lpObj->Level+lpObj->Vitality;			
		break;

		default:
			lpObj->m_MaxLifePower = 0;
		break;
	}
#endif
}
#endif // MASTER_LEVEL_UP_SYSTEM_20070912

#ifdef NEW_FORSKYLAND2
// �ɸ��Ͱ� ��� �ִ��� üũ �Ѵ�
void	gObjDelayLifeCheck(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj->m_CheckLifeTime > 0 )
	{	// 
		lpObj->m_CheckLifeTime--;
		if( lpObj->m_CheckLifeTime <= 0 )
		{
			lpObj->m_CheckLifeTime = 0;
			
			if( lpObj->Life < 0 ) lpObj->Life = 0;

			if( lpObj->lpAttackObj != NULL )
#ifdef ADD_SKILL_WITH_COMBO
				gObjLifeCheck(lpObj, lpObj->lpAttackObj, 0, 1, 0, 0, 0);
#else
				gObjLifeCheck(lpObj, lpObj->lpAttackObj, 0, 1, 0, 0);
#endif
		}
	}
}
#endif


#ifdef DUEL_SYSTEM_20031028


// Ư�� ����ڰ� ���������� üũ
BOOL gObjDuelCheck(LPOBJECTSTRUCT lpObj)
{
	BOOL bRetVal = FALSE;

	if (lpObj->Type == OBJTYPE_CHARACTER) {
		INT iDuelIndex = lpObj->m_iDuelUser;

		if (CHECK_LIMIT(iDuelIndex, MAX_OBJECT)) {
			bRetVal = TRUE;
		}
	}

	return bRetVal;
}


// ���� ���������� üũ
BOOL gObjDuelCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	BOOL bRetVal = FALSE;

	if (lpObj->Type == OBJTYPE_CHARACTER &&
		lpTargetObj->Type == OBJTYPE_CHARACTER
		) 
	{
		if ( (lpObj->m_iDuelUser == lpTargetObj->m_Index) && (lpTargetObj->m_iDuelUser == lpObj->m_Index) ) {
			bRetVal = TRUE;
		}
	}

	return bRetVal;
}


// �������� �ʱ�ȭ
VOID gObjResetDuel(LPOBJECTSTRUCT lpObj)
{
	INT iDuelIndex = lpObj->m_iDuelUser;

	if (CHECK_LIMIT(iDuelIndex, MAX_OBJECT)) {
		lpObj->m_iDuelUser = -1;
		lpObj->m_iDuelUserReserved = -1;
		lpObj->m_btDuelScore = 0;
#ifdef DUEL_SYSTEM_FIX_20031224
		lpObj->m_iDuelTickCount = 0;
#endif

#ifdef MODIFY_DARKSPIRTI_TARGETRELEASE_ON_DUEL_20060524
		// ��ũ ���Ǹ��� Ÿ�� ����
		gDarkSpirit[lpObj->m_Index].ReSetTarget(iDuelIndex);
		gDarkSpirit[iDuelIndex].ReSetTarget(lpObj->m_Index);
#endif

		gObj[iDuelIndex].m_iDuelUser = -1;
		gObj[iDuelIndex].m_iDuelUserReserved = -1;
		gObj[iDuelIndex].m_iDuelUserRequested = -1;
#ifdef DUEL_SYSTEM_FIX_20031224
		gObj[iDuelIndex].m_iDuelTickCount = 0;
#endif
	}
}


// ������ ����Ǿ��ٴ� ��Ŷ�� ������
VOID gObjSendDuelEnd(LPOBJECTSTRUCT lpObj)
{
	// ����ڰ� �ִ��� �˻�.
	if( gObjIsConnected(lpObj->m_Index) == FALSE ) return;
	if( lpObj->Type == OBJTYPE_MONSTER ) return;
	if( lpObj->CloseCount >= 0 ) return;

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	int	iTargetIndex = lpObj->m_iDuelUser;
	if( gObjDuelCheck(&gObj[iTargetIndex], lpObj) ) 
	{	// ���� ������ ù��° ���ᰡ ��� �ö�, �̺�Ʈ ���� �޽��� ����				
		PMSG_MATCH_RESULT	pMsg;

		PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x24, sizeof( pMsg) );
		pMsg.MatchType	= eMatchType_Dual;

		memcpy(pMsg.MatchTeamName1, lpObj->Name, MAX_IDSTRING);
		pMsg.Score1 = lpObj->m_btDuelScore;		

		memcpy(pMsg.MatchTeamName2, gObj[iTargetIndex].Name, MAX_IDSTRING);
		pMsg.Score2 = gObj[iTargetIndex].m_btDuelScore;

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		DataSend(iTargetIndex, (LPBYTE)&pMsg, pMsg.h.size);

		LogAddTD("[WTEvent] Duel Result Send = %s-%d %s-%d", 
				 lpObj->Name , lpObj->m_btDuelScore, 
				 gObj[iTargetIndex].Name, gObj[iTargetIndex].m_btDuelScore);
	}
#endif


	lpObj->m_iDuelUser = -1;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_btDuelScore = 0;
#ifdef DUEL_SYSTEM_FIX_20031224
	lpObj->m_iDuelTickCount = 0;
#endif

#ifdef ADD_NEWPVP_PKFIELD
#else
	PMSG_ANS_END_DUEL pMsg;
	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode = 0xAB;
	pMsg.h.size = sizeof( pMsg );

	pMsg.NumberH = HIBYTE(lpObj->m_iDuelUser);
	pMsg.NumberL = LOBYTE(lpObj->m_iDuelUser);
	memcpy(pMsg.szName, gObj[lpObj->m_iDuelUser].Name, MAX_IDSTRING);
	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#endif // ADD_NEWPVP_PKFIELD
}


#endif


#ifdef DUEL_SYSTEM_FIX_20031224

VOID gObjCheckAllUserDuelStop ()
{
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	return;
#endif
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			// ����ڰ� ���������� �켱 üũ
			if (CHECK_LIMIT(gObj[i].m_iDuelUser, MAX_OBJECT)) {
				if ((GetTickCount() - gObj[i].m_iDuelTickCount) > (60 * 1000)) {
					// ���� �� 1���� �����ٸ� ���� ���� ��Ų��.
					gObjSendDuelEnd(&gObj[gObj[i].m_iDuelUser]);
					gObjSendDuelEnd(&gObj[i]);
					gObjResetDuel(&gObj[i]);
				}
			}
		}
	}
}

#endif

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
VOID	gObjSendTimeView(int aIndex)
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			// ����ڰ� ���������� �켱 üũ
			if (CHECK_LIMIT(gObj[i].m_iDuelUser, MAX_OBJECT)) 
			{
				int DuelPlayTime = (5*60) - (GetTickCount()-gObj[i].m_iDuelTimer)/1000;
				GCTimeViewSend(gObj[i].m_Index, eMatchType_Dual, DuelPlayTime);
				
				if ((GetTickCount() - gObj[i].m_iDuelTimer) > (60 * 1000 * 5)) 
				{
					// ���� �� 10���� �����ٸ� ���� ���� ��Ų��.
					gObjSendDuelEnd(&gObj[gObj[i].m_iDuelUser]);
					gObjSendDuelEnd(&gObj[i]);
					gObjResetDuel(&gObj[i]);
				}
			}
		}
	}
}
#endif


#if defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
void gObjNProtectGGCheckSum(int aIndex)
{
	if( !gUseNPGGChecksum )
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];


	if( lpObj->m_InWebzen )
		return;

	if( lpObj->NPggCSAuth.IsAuth() )
	{	// üũ���� ������ �ȴ�
#if TESTSERVER == 1
		if( GetTickCount() - lpObj->m_NPggCheckSumSendTime > 1000 )
#else
		if( GetTickCount() - lpObj->m_NPggCheckSumSendTime > (1000*60)*5 )
#endif
		{	// 5�� �������� üũ���� ������
			lpObj->m_NPggCheckSumSendTime = GetTickCount();
			DWORD checksum = lpObj->NPggCSAuth.GetAuthDword();			

			LogAddTD("[NPgg] Send Checksum %x [%s][%s]", checksum, lpObj->AccountID, lpObj->Name);

			DWORD dwResults[8];
			lpObj->NPggCSAuth.GetAuthResults(checksum, dwResults, 8);

//			for( int i = 0; i < 8; i++)
//			{
//				LogAddTD("[NPgg] Send Checksum Result %x [%s][%s]", 
//						dwResults[i], lpObj->AccountID, lpObj->Name);
//			}			
			GCNPggSendCheckSum(aIndex, checksum);
		}
	}
	else
	{	// üũ���� ���� ���´�
		if( GetTickCount() - lpObj->m_NPggCheckSumSendTime > (1000*30) )
		{	// ������ �ð����� üũ���� �ȵ�� �Դ�(���� ����)
			LogAddTD("[NPgg] Checksum Time Out [%s][%s]", lpObj->AccountID, lpObj->Name);

#ifdef MODIFY_NOTICE_20040325
			PMSG_NOTICE	pNotice;
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);

			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1201) );
			TNotice::SendNoticeToUser( aIndex, &pNotice );

			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1202) );
			TNotice::SendNoticeToUser( aIndex, &pNotice );
			
			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1203) );
			TNotice::SendNoticeToUser( aIndex, &pNotice );
#else
			GCServerMsgStringSend(lMsg.Get(1201), aIndex, 0);		//"������ �ٽ� ������ ���ð� ��� ���� ������ �߻��ϸ�, "
			GCServerMsgStringSend(lMsg.Get(1202), aIndex, 0);		//"game@inca.co.kr �� GameGuard ������ erl ���ϵ��� ÷���ϰ�, "
			GCServerMsgStringSend(lMsg.Get(1203), aIndex, 0);		//"����ó�� ��� ���Ϸ� �����ֽñ� �ٶ��ϴ�."
#endif

			gObjCloseSet(aIndex, CL_EXIT);
		}
	}
}

#elif defined NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
void gObjNProtectGGCheckSum(int aIndex)
{
	if( !gUseNPGGChecksum )
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj->m_InWebzen )
		return;

	if( !lpObj->m_bSentGGAuth )
	{	// üũ���� ������ �ȴ�
#if TESTSERVER == 1
		if( GetTickCount() - lpObj->m_NPggCheckSumSendTime > 1000 )
#else
		if( GetTickCount() - lpObj->m_NPggCheckSumSendTime > (1000*60)*5 )
#endif
		{	// 5�� �������� üũ���� ������
			lpObj->m_NPggCheckSumSendTime = GetTickCount();
			
			DWORD dwGGErrCode = lpObj->NPggCSAuth.GetAuthQuery();			

			if( dwGGErrCode != ERROR_SUCCESS )
			{
				LogAddTD("[NPgg] Failed Send Checksum %x [%s][%s] errorcode: %d", 
					     dwGGErrCode, lpObj->AccountID, lpObj->Name, dwGGErrCode);

				lpObj->m_bSentGGAuth = false;
				gObjCloseSet(aIndex, CL_EXIT);
			}
			else
			{
				LogAddTD("[NPgg] Send Checksum %x [%s][%s]", dwGGErrCode, lpObj->AccountID, lpObj->Name);
			
				GCNPggSendCheckSum(aIndex, &lpObj->NPggCSAuth.m_AuthQuery);

				lpObj->m_bSentGGAuth = true;
			}
		}
	}
	else
	{	// üũ���� ���� ���´�
		if( GetTickCount() - lpObj->m_NPggCheckSumSendTime > (1000*30) )
		{	// ������ �ð����� üũ���� �ȵ�� �Դ�(���� ����)
			LogAddTD("[NPgg] Checksum Time Out [%s][%s]", lpObj->AccountID, lpObj->Name);

#ifdef MODIFY_NOTICE_20040325
			PMSG_NOTICE	pNotice;
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);

			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1201) );
			TNotice::SendNoticeToUser( aIndex, &pNotice );

			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1202) );
			TNotice::SendNoticeToUser( aIndex, &pNotice );
			
			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1203) );
			TNotice::SendNoticeToUser( aIndex, &pNotice );
#else
			GCServerMsgStringSend(lMsg.Get(1201), aIndex, 0);		//"������ �ٽ� ������ ���ð� ��� ���� ������ �߻��ϸ�, "
			GCServerMsgStringSend(lMsg.Get(1202), aIndex, 0);		//"game@inca.co.kr �� GameGuard ������ erl ���ϵ��� ÷���ϰ�, "
			GCServerMsgStringSend(lMsg.Get(1203), aIndex, 0);		//"����ó�� ��� ���Ϸ� �����ֽñ� �ٶ��ϴ�."
#endif
			gObjCloseSet(aIndex, CL_EXIT);
		}
	}
}
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411 || NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
void gObjNProtectGGCheckSum(int aIndex)
{
	if( !gUseNPGGChecksum )
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj->m_InWebzen )
		return;
	
#ifdef MODIFY_GAMEGUARD_NOT_AUTH_MAPSERVER_MOVE_20060513
	if( lpObj->m_bCheckGGAuth == false )
	{
#ifdef MODIFY_GAMEGARD_AUTH_MAPSERVER_MOVE_20080723
		// m_bCheckGGAuth ���� false �� �ٲ��� �ð��� 30�ʰ� ������ �ٽ� true�� ����
		if( GetTickCount() -lpObj->m_dwNPGGMapSvrMoveTime > 1000 * 30)
		{
			lpObj->m_bCheckGGAuth = true;
			if(lpObj->m_bSentGGAuth)
			{
				lpObj->m_bSentGGAuth = false;
			}
		}
#endif	// MODIFY_GAMEGARD_AUTH_MAPSERVER_MOVE_20080723
		return;
	}
#endif

	if( !lpObj->m_bSentGGAuth )
	{	// üũ���� ������ �ȴ�
#if TESTSERVER == 1
		if( GetTickCount() - lpObj->m_NPggCheckSumSendTime > 1000 )
#else
		if( GetTickCount() - lpObj->m_NPggCheckSumSendTime > (1000*60)*5 )
#endif
		{	// 5�� �������� üũ���� ������
			lpObj->m_NPggCheckSumSendTime = GetTickCount();

			DWORD dwGGErrCode = lpObj->NPggCSAuth.GetAuthQuery();			

			if( dwGGErrCode != ERROR_SUCCESS )
			{
				LogAddTD("[NPgg] Failed Send Checksum %x [%s][%s] errorcode: %d", 
					     dwGGErrCode, lpObj->AccountID, lpObj->Name, dwGGErrCode);

				lpObj->m_bSentGGAuth = false;
				gObjCloseSet(aIndex, CL_EXIT);
			}
			else
			{
				LogAddTD("[NPgg] Send Checksum %x [%s][%s]", dwGGErrCode, lpObj->AccountID, lpObj->Name);

				GCNPggSendCheckSum(aIndex, &lpObj->NPggCSAuth.m_AuthQuery);

				lpObj->m_bSentGGAuth = true;
			}
		}
	}
	else
	{	// üũ���� ���� ���´�
		if( GetTickCount() - lpObj->m_NPggCheckSumSendTime > (1000*30) )
		{	// ������ �ð����� üũ���� �ȵ�� �Դ�(���� ����)
			LogAddTD("[NPgg] Checksum Time Out [%s][%s]", lpObj->AccountID, lpObj->Name);

#ifdef MODIFY_NOTICE_20040325
			PMSG_NOTICE	pNotice;
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);

			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1201) );
			TNotice::SendNoticeToUser( aIndex, &pNotice );

			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1202) );
			TNotice::SendNoticeToUser( aIndex, &pNotice );
			
			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1203) );
			TNotice::SendNoticeToUser( aIndex, &pNotice );
#else
			GCServerMsgStringSend(lMsg.Get(1201), aIndex, 0);		//"������ �ٽ� ������ ���ð� ��� ���� ������ �߻��ϸ�, "
			GCServerMsgStringSend(lMsg.Get(1202), aIndex, 0);		//"game@inca.co.kr �� GameGuard ������ erl ���ϵ��� ÷���ϰ�, "
			GCServerMsgStringSend(lMsg.Get(1203), aIndex, 0);		//"����ó�� ��� ���Ϸ� �����ֽñ� �ٶ��ϴ�."
#endif
			gObjCloseSet(aIndex, CL_EXIT);
		}
	}
}
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

void SkillFrustrum(BYTE bangle, int aIndex)	// ĳ����..
{	
	LPOBJECTSTRUCT lpObj=(LPOBJECTSTRUCT)&gObj[aIndex];

	vec3_t p[4];
	Vector(-6 ,6.f ,0.f,p[0]);
	Vector( 6 ,6.f ,0.f,p[1]);
	Vector( 1 ,0.f ,0.f,p[2]);
	Vector(-1 ,0.f ,0.f,p[3]);
	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f,0.f,(float)(bangle*360/255),Angle);
	AngleMatrix(Angle,Matrix);
    vec3_t vFrustrum[4];
	
	for(int i=0;i<4;i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);
		lpObj->fSkillFrustrumX[i] = (int)(vFrustrum[i][0])+lpObj->X;
		lpObj->fSkillFrustrumY[i] = (INT)(vFrustrum[i][1])+lpObj->Y;
	}
}

// 
BOOL SkillTestFrustrum(int x,int y, int aIndex)// x,y ���� ��ǥ
{
	LPOBJECTSTRUCT lpObj=(LPOBJECTSTRUCT)&gObj[aIndex];

	int j = 3;
	for(int i=0;i<4;j=i,i++)
	{
		int d = (lpObj->fSkillFrustrumX[i]-x) * (lpObj->fSkillFrustrumY[j]-y) -
	      		(lpObj->fSkillFrustrumX[j]-x) * (lpObj->fSkillFrustrumY[i]-y);
		if(d < 0.f) return FALSE;
	}
	return TRUE;
}

#ifdef MODIFY_ZEN_MAX_20040414
BOOL gObjCheckMaxZen( int aIndex, int nAddZen )
{
	if	(!CHECK_LIMIT(aIndex, MAX_OBJECT)) 
		return FALSE;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	__int64 _Money = 0;
	_Money = (__int64)lpObj->Money + (__int64)nAddZen;

	if( _Money > MAX_ZEN )
	{
		GCServerMsgStringSend(lMsg.Get(1250), aIndex, 1);	// �ִ� ������ �� �ִ� ZEN �� �Ѿ����ϴ�.
		return FALSE;
	}
	
	return TRUE;
}
#endif

#ifdef CHINA_GM_EVENT
int		gEventGMNoticeTime = 0;

void	gObjMakeGMEventItem( int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	int		ExItemList[4] = 
	{
		MAKE_ITEMNUM(13,8), MAKE_ITEMNUM(13,9), 
		MAKE_ITEMNUM(13,12), MAKE_ITEMNUM(13,13)
	};

	int iItemIndex = rand()%4;

	BYTE NOption=0, NOption2 = 0;
	NOption = 1<<(rand()%6);

	while(1)
	{	// ������Ʈ �ɼ��� �ΰ�
		NOption2 = 1<<(rand()%6);
		if( NOption != NOption2 )
			break;
	}
	NOption |= NOption2;

	ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, 
						 ExItemList[iItemIndex], 0, 0, 0, 0, 3, lpObj->m_Index, NOption);

	LogAddTD("[GMEvent] (%s)(%s) ItemDrop", lpObj->AccountID, lpObj->Name);
}

void	gObjGMEventProc( int aIndex)
{
	if( !gGMEventRunning )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( lpObj->Authority&AUTHORITY_EVENT_GM )
	{
		gEventGMNoticeTime++;
		if( gEventGMNoticeTime > 60 )
		{
			gEventGMNoticeTime = 0;
			char tempMsg[255];
			wsprintf(tempMsg, lMsg.Get(1301),			// "(%s) �� ���� ��ġ (%s)(%d/%d)�� �ֽ��ϴ�"
					lpObj->Name, g_TerrainManager.GetMapName(lpObj->MapNumber), lpObj->X, lpObj->Y);
			
			AllSendServerMsg(tempMsg);
			return;
		}		
	}

	// ��Ÿ �������� ��ǥ
	RECT	rc;	
	rc.left = 203;
	rc.top = 70;
	rc.right = 213;
	rc.bottom = 81;
	POINT	pt;
	
	pt.x = lpObj->X;
	pt.y = lpObj->Y;

	if( ::PtInRect(&rc, pt) )
	{
		char tempMsg[255];
		wsprintf(tempMsg, lMsg.Get(1303), lpObj->Name);		//"%s ��������!!
		AllSendServerMsg(tempMsg);

		gGMEventRunning = FALSE;
		LogAddTD("[GMEvent] [%s][%s] Successful Runaway", lpObj->AccountID, lpObj->Name);					
	}
}
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
int		gEventGMNoticeTime = 0;

void	gObjMakeGMEventItem( int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	
	int		ExItemList[4] = 
	{
		MAKE_ITEMNUM(13,8), MAKE_ITEMNUM(13,9), 
			MAKE_ITEMNUM(13,12), MAKE_ITEMNUM(13,13)
	};
	
	int iItemIndex = rand()%4;
	
	BYTE NOption=0, NOption2 = 0;
	NOption = 1<<(rand()%6);
	
	while(1)
	{	// ������Ʈ �ɼ��� �ΰ�
		NOption2 = 1<<(rand()%6);
		if( NOption != NOption2 )
			break;
	}
	NOption |= NOption2;
	
	ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, 
		ExItemList[iItemIndex], 0, 0, 0, 0, 3, lpObj->m_Index, NOption);
	
	LogAddTD("[GMEvent] (%s)(%s) ItemDrop", lpObj->AccountID, lpObj->Name);
}

void	gObjGMEventProc( int aIndex)
{
	if( !gGMEventRunning )
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	
	if( lpObj->Authority&AUTHORITY_EVENT_GM )
	{
		gEventGMNoticeTime++;
		if( gEventGMNoticeTime > 60 )
		{
			gEventGMNoticeTime = 0;
			char tempMsg[255];
			wsprintf(tempMsg, lMsg.Get(1301),			// "(%s) �� ���� ��ġ (%s)(%d/%d)�� �ֽ��ϴ�"
				lpObj->Name, g_TerrainManager.GetMapName(lpObj->MapNumber), lpObj->X, lpObj->Y);
			
			AllSendServerMsg(tempMsg);
			return;
		}		
	}
	
	// ��Ÿ �������� ��ǥ
	RECT	rc;	
	rc.left = 203;
	rc.top = 70;
	rc.right = 213;
	rc.bottom = 81;
	POINT	pt;
	
	pt.x = lpObj->X;
	pt.y = lpObj->Y;
	
	if( ::PtInRect(&rc, pt) )
	{
		char tempMsg[255];
		wsprintf(tempMsg, lMsg.Get(1303), lpObj->Name);		//"%s ��������!!
		AllSendServerMsg(tempMsg);
		
		gGMEventRunning = FALSE;
		LogAddTD("[GMEvent] [%s][%s] Successful Runaway", lpObj->AccountID, lpObj->Name);					
	}
}
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#if GAME_VERSION >= G_V_99B
void		MakeRandomSetItem(int aIndex)
{
#ifdef CHANGE_CHAOSCASTLE_SET_ITEM_DROP
	gChaosCastleSetItemDrop.MakeRandomSetItem(aIndex);
#else
	int itemnum = gSetItemOption.GenRandomItemNum();
	int	SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand, option2rand, option3rand, optionc;	
	int	Option1 = 0, Option2 = 0, Option3 = 0;

	
	option1rand = 6;				// �Ϲ����� �ɼ�1 (��ų) ���� Ȯ�� 6/100
	option2rand = 4;				// �Ϲ����� �ɼ�2 (���) ���� Ȯ�� 4/100
	option3rand = (rand()%100);		// �Ϲ����� �ɼ�3 (�߰�����Ʈ) ���� Ȯ�� ����/100
	optionc		= (rand()%3);

	// ��ų������ Ȯ���� ���� �����.	
	if((rand()%100) < option2rand ) Option2 = 1;

	switch( optionc )
	{
	case 0 :
		if( option3rand < 4 ) Option3 = 3;	// �ɼ�
		break;
	case 1 :
		if( option3rand < 8 ) Option3 = 2;	// �ɼ�
		break;
	case 2 :
		if( option3rand < 12 ) Option3 = 1;	// �ɼ�
		break;
	}

	Option1 = 1;
	ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, itemnum, 
						 0, (BYTE)0, Option1, Option2, Option3, aIndex, 0, SetOption);

#ifdef UPDATE_SET_ITEM_DROP_LOG_20080420
	int tmpSetOption = 0;

	if( SetOption & 0x01)
	{
		tmpSetOption = 1;
	}
	else if( SetOption & 0x02 )
	{
		tmpSetOption = 2;
	}

	LogAddTD("[Chaos Castle] [%s][%s] Winner Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",			 
			 gObj[aIndex].AccountID, 
			 gObj[aIndex].Name,
			 itemnum, Option1, Option2, Option3,SetOption,
			 gSetItemOption.GetSetOptionName(itemnum, tmpSetOption));
#else	// UPDATE_SET_ITEM_DROP_LOG_20080420
	LogAddTD("[Chaos Castle] [%s][%s] Winner Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",			 
			 gObj[aIndex].AccountID, 
			 gObj[aIndex].Name,
			 itemnum, Option1, Option2, Option3,SetOption);
#endif	// UPDATE_SET_ITEM_DROP_LOG_20080420
#endif
}
#endif


#ifdef ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907
// Reward Type = 1 : ��п� ���� ����
#ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
void MakeRewardSetItem(int aIndex, UCHAR cDropX, UCHAR cDropY, int iRewardType, int iMapnumber)
#else
void MakeRewardSetItem(int aIndex, UCHAR cDropX, UCHAR cDropY, int iRewardType)
#endif // #ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
{
	int itemnum = gSetItemOption.GenRandomItemNum();
	int	SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand, option2rand, option3rand, optionc;	
	int	Option1 = 0, Option2 = 0, Option3 = 0;

	
	option1rand = 6;				// �Ϲ����� �ɼ�1 (��ų) ���� Ȯ�� 6/100
	option2rand = 4;				// �Ϲ����� �ɼ�2 (���) ���� Ȯ�� 4/100
	option3rand = (rand()%100);		// �Ϲ����� �ɼ�3 (�߰�����Ʈ) ���� Ȯ�� ����/100
	optionc		= (rand()%3);

	// ��ų������ Ȯ���� ���� �����.
	if((rand()%100) < option2rand ) Option2 = 1;

	switch( optionc )
	{
	case 0 :
		if( option3rand < 4 ) Option3 = 3;	// �ɼ�
		break;
	case 1 :
		if( option3rand < 8 ) Option3 = 2;	// �ɼ�
		break;
	case 2 :
		if( option3rand < 12 ) Option3 = 1;	// �ɼ�
		break;
	}

	// ������ ��ų�� �ٴ´�.
	Option1 = 1;

	// �����ġ ����
	if( cDropX == 0 && cDropY == 0 )
	{
		cDropX = (BYTE)gObj[aIndex].X;
		cDropY = (BYTE)gObj[aIndex].Y;
	}

#ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216	
	ItemSerialCreateSend(aIndex, iMapnumber, cDropX, cDropY, itemnum, 
						 0, (BYTE)0, Option1, Option2, Option3, aIndex, 0, SetOption);
#else
	ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, cDropX, cDropY, itemnum, 
						 0, (BYTE)0, Option1, Option2, Option3, aIndex, 0, SetOption);
#endif // #ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216

	// ��п� ���� ����
#ifdef UPDATE_SET_ITEM_DROP_LOG_20080420	// ��� ��Ʈ �����۷α�
	int tmpSetOption = 0;

	if( SetOption & 0x01)
	{
		tmpSetOption = 1;
	}
	else if( SetOption & 0x02 )
	{
		tmpSetOption = 2;
	}

	if( iRewardType == 1 )
	{
		LogAddTD("[�ڡ�Reward][KUNDUN] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",			 
			 gObj[aIndex].AccountID, 
			 gObj[aIndex].Name,
			 itemnum, Option1, Option2, Option3,SetOption,
			 gSetItemOption.GetSetOptionName(itemnum, tmpSetOption));
	}
	else
	{
		LogAddTD("[Reward][Etc] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",			 
			 gObj[aIndex].AccountID, 
			 gObj[aIndex].Name,
			 itemnum, Option1, Option2, Option3,SetOption,
			 gSetItemOption.GetSetOptionName(itemnum, tmpSetOption));
	}
#else	// UPDATE_SET_ITEM_DROP_LOG_20080420
	if( iRewardType == 1 )
	{
		LogAddTD("[�ڡ�Reward][KUNDUN] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",			 
			 gObj[aIndex].AccountID, 
			 gObj[aIndex].Name,
			 itemnum, Option1, Option2, Option3,SetOption);
	}
	else
	{
		LogAddTD("[Reward][Etc] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",			 
			 gObj[aIndex].AccountID, 
			 gObj[aIndex].Name,
			 itemnum, Option1, Option2, Option3,SetOption);
	}
#endif	// UPDATE_SET_ITEM_DROP_LOG_20080420
	
}
#endif


#ifdef DARKLORD_WORK	
void	gObjRecall(int aIndex, int mapnumber, int x, int y)
{
	if( gObj[aIndex].MapNumber != mapnumber )
	{
		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;
		gObj[aIndex].TX = x;
		gObj[aIndex].TX = y;
		gObj[aIndex].MapNumber = mapnumber;	
		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport  = 0;

		//gObjViewportListProtocolDestroy(&gObj[aIndex]);		
		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex], -1, mapnumber, (BYTE)gObj[aIndex].X, (BYTE)gObj[aIndex].Y, gObj[aIndex].Dir);

		if( gObj[aIndex].m_Change >= 0 )
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenMapNumber = mapnumber;
		gObj[aIndex].RegenMapX = (BYTE)x;
		gObj[aIndex].RegenMapY = (BYTE)y;		
		gObj[aIndex].RegenOk   = 1;
	}
	else
	{
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

		DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);
		MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);

		//gObj[aIndex].MapNumber			= mapnumber;

		gObjTeleportMagicUse(aIndex, x, y);	
	}
}

// �� ������ ����ġ ����
void	gObjSetExpPetItem(int aIndex, int exp)
{

// MODIFY_ACCUMULATED_BUG_DARKLORD_PET_EXP_01_20050603 ?

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	if(lpObj->Class != CLASS_DARKLORD )
		return;

	exp = (int)((float)exp*gDarkSpiritAddExperience);

	if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,4) && 
		lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type == MAKE_ITEMNUM(13,5) )
	{	// ��ũȣ���� ��ũ���Ǹ� ��� ������ 5%�� ����ġ �й�
		int addexp = exp*10/100;

		if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].AddPetItemExp(addexp) )
		{	
			if( !gObjIsItemPut(lpObj, &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT], EQUIPMENT_WEAPON_LEFT) )
			{	// ������ ������ �ȵǸ� ���� �ٿ�
				lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].PetItemLevelDown(addexp);

				// ��ũ���Ǹ��� ������ �� ���� ����ġ ȹ���� �Ұ����մϴ�
				MsgOutput(aIndex, lMsg.Get(1245));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
						lpObj->AccountID,
						lpObj->Name,
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].GetName(), 
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Level,
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp,
						addexp);

				CDarkSpirit::SendLevelmsg(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, 0, 254);
			}
		}

#if TESTSERVER == 1
		DebugOutput(aIndex, "[%s] Lv:%d Exp:%d", 
					lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].GetName(),				
					lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Level, 
					lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp);
#endif

		if( lpObj->pInventory[EQUIPMENT_HELPER].AddPetItemExp(addexp) )
		{
			if( !gObjIsItemPut(lpObj, &lpObj->pInventory[EQUIPMENT_HELPER], EQUIPMENT_HELPER) )
			{	// ������ ������ �ȵǸ� ���� �ٿ�
				lpObj->pInventory[EQUIPMENT_HELPER].PetItemLevelDown(addexp);

				// ��ũȣ���� ������ �� ���� ����ġ ȹ���� �Ұ����մϴ�
				MsgOutput(aIndex, lMsg.Get(1246));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
						lpObj->AccountID,
						lpObj->Name,
						lpObj->pInventory[EQUIPMENT_HELPER].GetName(), 
						lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Level,
						lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Exp,
						addexp);

				CDarkSpirit::SendLevelmsg(lpObj->m_Index, EQUIPMENT_HELPER, 1, 254);
			}
		}

#if TESTSERVER == 1
		DebugOutput(aIndex, "[%s] Lv:%d Exp:%d", 
					lpObj->pInventory[EQUIPMENT_HELPER].GetName(),				
					lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Level, 
					lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Exp);
#endif
	}
	else if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,4) )
	{	// ��ũȣ�� ����ġ �й�
		int addexp = exp*20/100;
		if( lpObj->pInventory[EQUIPMENT_HELPER].AddPetItemExp(addexp) )
		{
			if( !gObjIsItemPut(lpObj, &lpObj->pInventory[EQUIPMENT_HELPER], EQUIPMENT_HELPER) )
			{	// ������ ������ �ȵǸ� ���� �ٿ�
				lpObj->pInventory[EQUIPMENT_HELPER].PetItemLevelDown(addexp);
				// ��ũȣ���� ������ �� ���� ����ġ ȹ���� �Ұ����մϴ�
				MsgOutput(aIndex, lMsg.Get(1246));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
						lpObj->AccountID,
						lpObj->Name,
						lpObj->pInventory[EQUIPMENT_HELPER].GetName(), 
						lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Level,
						lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Exp,
						addexp);

				CDarkSpirit::SendLevelmsg(lpObj->m_Index, EQUIPMENT_HELPER, 1, 254);
			}
		}

#if TESTSERVER == 1		
		DebugOutput(aIndex, "[%s] Lv:%d Exp:%d", 
					lpObj->pInventory[EQUIPMENT_HELPER].GetName(),				
					lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Level, 
					lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Exp);
#endif

	}
	else if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type == MAKE_ITEMNUM(13,5) )
	{	// ��ũ���Ǹ� ����ġ �й�
		int addexp = exp*20/100;

		if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].AddPetItemExp(addexp) )
		{
			if( !gObjIsItemPut(lpObj, &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT], EQUIPMENT_WEAPON_LEFT) )
			{	// ������ ������ �ȵǸ� ���� �ٿ�
				lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].PetItemLevelDown(addexp);
				// ��ũ���Ǹ��� ������ �� ���� ����ġ ȹ���� �Ұ����մϴ�
				MsgOutput(aIndex, lMsg.Get(1245));				
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
						lpObj->AccountID,
						lpObj->Name,
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].GetName(), 
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Level,
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp,
						addexp);

				CDarkSpirit::SendLevelmsg(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, 0, 254);
			}
		}

#if TESTSERVER == 1
		DebugOutput(aIndex, "[%s] Lv:%d Exp:%d", 
					lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].GetName(),				
					lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Level, 
					lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp);
#endif
	}

}
#endif

#ifdef ADD_MONSTER_DIE_REWARD_ITEMS_20040915
BOOL gObjGetRandomItemDropLocation ( INT iMapNumber, UCHAR & cX,	UCHAR & cY, INT iRangeX, INT iRangeY, INT iLoopCount)
{
	INT iUX = cX;
	INT iUY = cY;

	if (iRangeX <= 0)		iRangeX	= 1;
	if (iRangeY <= 0)		iRangeY	= 1;
	if (iLoopCount <= 0)	iLoopCount	= 1;

	while (iLoopCount-- > 0) {
		cX = (rand() % (iRangeX + 1)) * ((rand() % 2) ? 1 : -1) + iUX;
		cY = (rand() % (iRangeY + 1)) * ((rand() % 2) ? 1 : -1) + iUY;

		BYTE btMapAttr = MapC[iMapNumber].GetAttr(cX, cY);
	
		if( ((btMapAttr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) &&
			((btMapAttr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW) ) 
		{		
			return TRUE;
		}
	}

	return FALSE;
}
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518

// Ư������ cX, cY ��ó ���� �߿� ���ڸ��� �ϳ� ã�Ƴ���.
BOOL gObjGetRandomFreeLocation (	INT	iMapNumber,	UCHAR & cX,	UCHAR & cY, INT iRangeX, INT iRangeY, INT iLoopCount)
{
	INT iUX = cX;
	INT iUY = cY;

	if (iRangeX <= 0)		iRangeX	= 1;
	if (iRangeY <= 0)		iRangeY	= 1;
	if (iLoopCount <= 0)	iLoopCount	= 1;

	while (iLoopCount-- > 0) {
		cX = (rand() % (iRangeX + 1)) * ((rand() % 2) ? 1 : -1) + iUX;
		cY = (rand() % (iRangeY + 1)) * ((rand() % 2) ? 1 : -1) + iUY;

		BYTE btMapAttr = MapC[iMapNumber].GetAttr(cX, cY);
		if( btMapAttr == MAP_ATTR_NONE )	// �̵������� �����̶�� ����
			return TRUE;
	}

	return FALSE;
}


// �ش� �ɸ� (�����)�� ������ ������������ ��������.
INT	gObjCheckAttackTypeMagic ( INT iClass, INT iSkill)
{
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	if (iSkill == 0)
		return CMagicDamage::SKILL_TYPE_PHYSICS;
	else
		return MagicDamageC.GetSkillType(iSkill);

	return CMagicDamage::SKILL_TYPE_NONE;
#else
	if (iClass == CLASS_KNIGHT || iClass == CLASS_ELF) {
		// ���� ������ ������ ������ ����������
		return FALSE;
	}

	if (iClass == CLASS_WIZARD) {
		// ������ ������ ��ų�� ����� �̻� ������ ��������
		if (iSkill > 0) {
			return TRUE;
		} else {
			return FALSE;
		}
	}

	if (iClass == CLASS_MAGUMSA) {
		// ���˻��� ������ ��ų �߿��� ���������� �����Ѵ�.
		if (iSkill > 0) {
			switch(iSkill) {
			case AT_SKILL_DEFENSEDOWN :					// �������
			case AT_SKILL_SWORD6 :						// �Ŀ�������
				return FALSE;
				break;
			default:
				return TRUE;
				break;
			}
		} else {
			return FALSE;
		}
	}

	if (iClass == CLASS_DARKLORD) {
		// ��ũ�ε��� ������ ��ų �߿��� ���������� �����Ѵ�.
		if (iSkill > 0) {
			switch(iSkill) {
			case AT_SKILL_SPEAR :						// �������̺�
			case AT_SKILL_DARKHORSE_ATTACK :			// �����ũ
				return FALSE;
				break;
			default:
				return TRUE;
				break;
			}
		} else {
			return FALSE;
		}
	}

	return FALSE;
#endif
}

#endif

#ifdef CASTLEDEEP_EVENT_20050413

BOOL gObjGetRandomFreeArea (INT	iMapNumber,	UCHAR & cX,	UCHAR & cY, INT iSX, INT iSY, INT iDX, INT iDY, INT iLoopCount)
{
	if (iSX <= 0)					iSX	= 1;
	if (iSY <= 0)					iSY	= 1;
	if (iDX <= 0)					iDX	= 1;
	if (iDY <= 0)					iDY	= 1;
	if (iSX >= MAX_TERRAIN_SIZE)	iSX	= MAX_TERRAIN_SIZE-1;
	if (iSY >= MAX_TERRAIN_SIZE)	iSY	= MAX_TERRAIN_SIZE-1;
	if (iDX >= MAX_TERRAIN_SIZE)	iDX	= MAX_TERRAIN_SIZE-1;
	if (iDY >= MAX_TERRAIN_SIZE)	iDY	= MAX_TERRAIN_SIZE-1;

	if (iLoopCount <= 0)	iLoopCount	= 1;

	while (iLoopCount-- > 0) {
		cX = rand() % (iDX - iSX) + iSX;
		cY = rand() % (iDY - iSY) + iSY;

		BYTE btMapAttr = MapC[iMapNumber].GetAttr(cX, cY);
		if( btMapAttr == MAP_ATTR_NONE )	// �̵������� �����̶�� ����
			return TRUE;
	}

	return FALSE;
}

#endif

#ifdef ADD_NEW_UNION_01_20041006

INT gObjGetGuildUnionNumber( LPOBJECTSTRUCT lpObj )
{
	INT iUnion = 0;

	if( lpObj->lpGuild != NULL )
		iUnion = lpObj->lpGuild->iGuildUnion==G_RELATIONSHIP_NONE ? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;

	return iUnion;
}

void gObjGetGuildUnionName( LPOBJECTSTRUCT lpObj, CHAR* szUnionName, INT iUnionNameLen )
{
	szUnionName[0] = '\0';

	if( lpObj->lpGuild == NULL )	
		return;

	if( lpObj->lpGuild->iGuildUnion == G_RELATIONSHIP_NONE )	
		return;
	
	TUnionInfo* pUnionInfo = UnionManager.SearchUnion( lpObj->lpGuild->iGuildUnion );
		
	if( pUnionInfo != NULL )
	{
		CopyMemory ( szUnionName, pUnionInfo->m_szMasterGuild, iUnionNameLen );
	}	
}

BOOL gObjCheckRival( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj )
{
	if( lpTargetObj->Type != OBJTYPE_CHARACTER )	
		return FALSE;

	if( lpObj->Type != OBJTYPE_CHARACTER )		
		return FALSE;

	if( lpTargetObj->lpGuild==NULL || lpObj->lpGuild==NULL )
		return FALSE;

	if( lpTargetObj->lpGuild->iGuildRival == G_RELATIONSHIP_NONE 
		|| lpObj->lpGuild->iGuildRival == G_RELATIONSHIP_NONE )
		return FALSE;

	INT iUnion = (lpObj->lpGuild->iGuildUnion==G_RELATIONSHIP_NONE) ? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;

	if( UnionManager.GetGuildRelationShip ( iUnion, lpTargetObj->lpGuild->Number ) == G_RELATIONSHIP_RIVAL )
		return TRUE;

	return FALSE;

}

INT gObjGetRelationShip( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj )
{
	// 0. ĳ���� üũ
	if( lpObj==NULL || lpTargetObj==NULL )	
	{
		return G_RELATIONSHIP_NONE;
	}

	// 1. ��� üũ
	if( lpObj->lpGuild==NULL || lpTargetObj->lpGuild==NULL )
	{
		return G_RELATIONSHIP_NONE;
	}

	// 3. ���� üũ
	INT iUnion = (lpObj->lpGuild->iGuildUnion==G_RELATIONSHIP_NONE) ? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;

	return UnionManager.GetGuildRelationShip ( iUnion, lpTargetObj->lpGuild->Number );
}

// [ 0x67 ] ���� ���� �ִ� ĳ���͵��� Union ���� ����.
void gObjNotifyUpdateUnionV1( LPOBJECTSTRUCT lpObj )
{
	if( lpObj == NULL )	return;

	// ��. ���� ���� �ִ� ���� ���� ����
	CHAR cBUFFER_V1 [MAX_BUFF_SIZE];
	INT	iVp1Count = 0;
	
	ZeroMemory( cBUFFER_V1, MAX_BUFF_SIZE );
	
	LPPMSG_UNION_VIEWPORT_NOTIFY_COUNT	lpMsg		= (LPPMSG_UNION_VIEWPORT_NOTIFY_COUNT) cBUFFER_V1;
	LPPMSG_UNION_VIEWPORT_NOTIFY		lpMsgBody	= (LPPMSG_UNION_VIEWPORT_NOTIFY) ( cBUFFER_V1 + sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT) );
	
	for( int n = 0 ; n < MAXVIEWPORTOBJECT ; n++ )
	{
		if( !CHECK_LIMIT( lpObj->VpPlayer[n].number, MAX_OBJECT ) ) 
			continue;

		if( lpObj->VpPlayer[n].state == STVP_PLAYING && lpObj->VpPlayer[n].type == OBJTYPE_CHARACTER )
		{
			LPOBJECTSTRUCT lpTargetObj = &gObj[lpObj->VpPlayer[n].number];

			if( lpTargetObj == NULL )			continue;
			if( lpTargetObj->lpGuild == NULL )	continue;
	
			gObjGetGuildUnionName( lpTargetObj, lpMsgBody[iVp1Count].szUnionName, MAX_GUILDNAMESTRING );

			lpMsgBody[iVp1Count].btGuildRelationShip	= gObjGetRelationShip( lpObj, lpTargetObj );

			lpMsgBody[iVp1Count].btNumberL				= LOBYTE(WORD(lpTargetObj->m_Index));
			lpMsgBody[iVp1Count].btNumberH				= HIBYTE(WORD(lpTargetObj->m_Index));

			lpMsgBody[iVp1Count].iGuildNumber			= lpTargetObj->lpGuild->Number;
		
			iVp1Count++;
		}
	}

	if (iVp1Count > 0 && iVp1Count <= MAXVIEWPORTOBJECT) 
	{
		lpMsg->btCount = iVp1Count;
		PHeadSetW((LPBYTE) lpMsg, 0x67, sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT) + (sizeof(PMSG_UNION_VIEWPORT_NOTIFY) * iVp1Count));
		DataSend (lpObj->m_Index, (LPBYTE) lpMsg, MAKEWORD(lpMsg->h.sizeL, lpMsg->h.sizeH));
	}
	else if (iVp1Count != 0) 
	{
		LogAddTD("[Union ViewPort] ERROR : iVp1Count is OUT of BOUND: %d", iVp1Count);
	}
	
}

// [ 0x67 ] ���� ���� �ִ� ĳ���͵��� Union ���� ����.
void gObjNotifyUpdateUnionV2( LPOBJECTSTRUCT lpObj )
{
	if( lpObj == NULL )	return;
	if( lpObj->lpGuild == NULL ) return;

	// ��. ���� ���� �ִ� View ����
	CHAR cBUFFER_V2 [100] = {0,};

	LPPMSG_UNION_VIEWPORT_NOTIFY_COUNT	lpMsg2		= (LPPMSG_UNION_VIEWPORT_NOTIFY_COUNT) cBUFFER_V2;
	LPPMSG_UNION_VIEWPORT_NOTIFY		lpMsgBody2	= (LPPMSG_UNION_VIEWPORT_NOTIFY) ( cBUFFER_V2 + sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT) );

	gObjGetGuildUnionName( lpObj, lpMsgBody2->szUnionName, MAX_GUILDNAMESTRING );
		
	lpMsgBody2->btNumberL			 = LOBYTE(WORD(lpObj->m_Index));
	lpMsgBody2->btNumberH			 = HIBYTE(WORD(lpObj->m_Index));
	
	lpMsgBody2->iGuildNumber		 = lpObj->lpGuild->Number;
	lpMsgBody2->btGuildRelationShip  = G_RELATIONSHIP_NONE;

	lpMsg2->btCount = 1;
	PHeadSetW((LPBYTE) lpMsg2, 0x67, sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT) + (sizeof(PMSG_UNION_VIEWPORT_NOTIFY) * 1));

	// ��. �ڽ��� View ����
   	DataSend( lpObj->m_Index, (LPBYTE)lpMsg2, MAKEWORD(lpMsg2->h.sizeL, lpMsg2->h.sizeH));

	for( int n=0; n<MAXVIEWPORTOBJECT; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_CHARACTER )
		{
			if( lpObj->VpPlayer2[n].state )
			{
				LPOBJECTSTRUCT lpTargetObj = &gObj[lpObj->VpPlayer2[n].number];

				if( lpTargetObj->lpGuild != NULL )
				{
					lpMsgBody2->btGuildRelationShip = gObjGetRelationShip( lpTargetObj, lpObj );
				}
				
				// ���� Union �� ��� ������ �ʴ´�. V1���� ���� ���� �Ǿ���.
				if( lpMsgBody2->btGuildRelationShip != G_RELATIONSHIP_UNION )
					DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)lpMsg2, MAKEWORD(lpMsg2->h.sizeL, lpMsg2->h.sizeH));
			}
		}
	}
}

void gObjUnionUpdateProc( INT aIndex )
{
	if (!gObjIsConnected(aIndex)) 
	{
		return;
	}

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	if( lpObj->Type != OBJTYPE_CHARACTER ) 
	{	// ���ͳ� NPC�� �̰��� ���� �ʿ䰡 ����.
		return;
	}

	if( lpObj->RegenOk > 0 ) 
	{	// ���� ���� ����ڴ� �����Ѵ�.
		return;
	}

	if( lpObj->CloseCount > -1) 
	{	// ���� ���� ����ڴ� �����Ѵ�.
		return;
	}

	// ��� ������ ������ ó������ �ʴ´�.
	if( lpObj->lpGuild == NULL )
		return;

	// ��� ���� ������Ʈ �÷��װ� ������ �Ѿ��.
	if( lpObj->lpGuild->CheckTimeStamp( lpObj->iGuildUnionTimeStamp ) )
		return;

	lpObj->iGuildUnionTimeStamp = lpObj->lpGuild->GetTimeStamp();

	gObjNotifyUpdateUnionV1( lpObj );
	gObjNotifyUpdateUnionV2( lpObj );
}

#endif


#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116

void gObjSetKillCount( INT aIndex, INT iOption )
{
	if (!gObjIsConnected(aIndex)) 
	{
		return;
	}
	
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	if( iOption == KILLCOUNT_INC )
	{
		if( lpObj->m_btKillCount <= 254 )
			lpObj->m_btKillCount++;

#if TESTSERVER == 1
		// 20060908 RKFKA ����
		//lpObj->m_btKillCount = 100;
#endif


	}
	else if( iOption == KILLCOUNT_DEC)
	{
		if( lpObj->m_btKillCount > 0 )
			lpObj->m_btKillCount--;	
	}
	else if( iOption == KILLCOUNT_RESET )
	{
		lpObj->m_btKillCount = 0;
#if TESTSERVER == 1
		// 20060908 RKFKA ����
		//lpObj->m_btKillCount = 100;
#endif
	}

	PMSG_KILLCOUNT pMsg = { 0, };
	PHeadSubSetB((LPBYTE)&pMsg, 0xB8, 0x01, sizeof( pMsg) );

	pMsg.btKillCount = lpObj->m_btKillCount;
	DataSend( aIndex, (LPBYTE)&pMsg, sizeof( pMsg) );
}

#endif

#ifdef ADD_NEW_VIEWPORT_SYSTEM_01_20041201

// ���⸦ ����� ����� Viewport
void gObjNotifyUseWeaponV1( LPOBJECTSTRUCT lpOwnerObj, LPOBJECTSTRUCT lpWeaponObj, INT iTargetX, INT iTargetY  )
{
	if( lpOwnerObj == NULL )	return;
	if( lpWeaponObj == NULL )	return;

	// ��. Weapon�� Ÿ�� ������ �ִ� ĳ���͵鿡�� ������� ��� ������ �˸���
	PMSG_NOTIFY_REGION_OF_WEAPON	pNotifyRegionMsg = { 0, };

	PHeadSubSetB((LPBYTE)&pNotifyRegionMsg, 0xB7, 0x02, sizeof( pNotifyRegionMsg) );

	pNotifyRegionMsg.btPointX		= iTargetX;
	pNotifyRegionMsg.btPointY		= iTargetY;

	// ��. Weapon�� Ÿ�� ĳ���͵� (������ ���� ���� ĳ����)���� �˸�
	PMSG_NOTIFY_TARGET_OF_WEAPON	pNotifyTargetMsg = { 0, };

	PHeadSubSetB((LPBYTE)&pNotifyTargetMsg, 0xB7, 0x03, sizeof( pNotifyTargetMsg) );

	if( lpWeaponObj->Class == NPC_SIEGE_MACHINE_ATTACK )
	{
		pNotifyRegionMsg.btWeaponType = WEAPON_TYPE_CASTLE_ATTACKER;
		pNotifyTargetMsg.btWeaponType = WEAPON_TYPE_CASTLE_ATTACKER;
	}
	else if( lpWeaponObj->Class == NPC_SIEGE_MACHINE_DEFENSE )
	{
		pNotifyRegionMsg.btWeaponType = WEAPON_TYPE_CASTLE_DEFENDER;
		pNotifyTargetMsg.btWeaponType = WEAPON_TYPE_CASTLE_DEFENDER;
	}


	// ��. Weapon�� Ÿ�� ���� ����Ʈ
	CHAR cBUFFER_V1 [MAX_BUFF_SIZE];
	INT	iVp1Count = 0;
	
	ZeroMemory( cBUFFER_V1, MAX_BUFF_SIZE );
	
	LPPMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT	lpMsg	= (LPPMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT) cBUFFER_V1;
	LPPMSG_WEAPON_OWNER_VIEWPORT_NOTIFY		lpMsgBody	= (LPPMSG_WEAPON_OWNER_VIEWPORT_NOTIFY) ( cBUFFER_V1 + sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT) );
	
	for( int n = 0 ; n < MAX_OBJECT ; n++ )
	{
		LPOBJECTSTRUCT lpTargetObj = &gObj[n];

		if (!gObjIsConnected(n)) 
		{
			continue;
		}

		// ��ġ �˻�.
		if( lpOwnerObj->MapNumber != lpTargetObj->MapNumber )
			continue;

		if( abs(lpTargetObj->X - iTargetX) > 6 )
			continue;

		if( abs(lpTargetObj->Y - iTargetY) > 6 )
			continue;

		lpMsgBody[iVp1Count].btObjClassH		= HIBYTE(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjClassL		= LOBYTE(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjIndexH		= HIBYTE(lpTargetObj->m_Index);
		lpMsgBody[iVp1Count].btObjIndexH		= LOBYTE(lpTargetObj->m_Index);
		lpMsgBody[iVp1Count].btX				= lpTargetObj->X;
		lpMsgBody[iVp1Count].btY				= lpTargetObj->Y;
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
		lpMsgBody[iVp1Count].BuffEffectCount	= gObjMakeViewportState( lpTargetObj, lpMsgBody[iVp1Count].BuffEffectList );
#else
		lpMsgBody[iVp1Count].dwViewSkillState	= lpTargetObj->m_ViewSkillState;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
		
		if( lpTargetObj->Type == OBJTYPE_CHARACTER )
		{
			lpMsgBody[iVp1Count].btObjType	 = OBJTYPE_CHARACTER;
			lpMsgBody[iVp1Count].btObjClassH = (lpTargetObj->Class<<5)&0xE0;
			lpMsgBody[iVp1Count].btObjClassL = 0x00;

			// CharSet + 1�� �� ������ btObjClassH,L �� ���� ������ Ŭ���̾�Ʈ �Ľ� ���ϰ� �ϱ� ����.
			// �̷� ���Ͽ�.. MAX_PREVIEWCHARSET+3 ���� ����. (apple)
			// Oh~ Shit... MAX_PREVIEWCHARSET+3 , MAX_PREVIEWCHARSET+4? Oh~
#ifdef ITEM_INDEX_EXTEND_20050706	// Ŭ���� ������ ���� ������ ������ �Ǿ� 1����Ʈ ����
			CopyMemory( lpMsgBody[iVp1Count].CharSet, lpTargetObj->CharSet + 1, MAX_PREVIEWCHARSET+8 ); 			
#else
			CopyMemory( lpMsgBody[iVp1Count].CharSet, lpTargetObj->CharSet + 1, MAX_PREVIEWCHARSET+3 ); 			
#endif
		}
		else if( lpTargetObj->Type == OBJTYPE_MONSTER )
		{
			lpMsgBody[iVp1Count].btObjType	 = OBJTYPE_MONSTER;
#ifdef ITEM_INDEX_EXTEND_20050706
			ZeroMemory( lpMsgBody[iVp1Count].CharSet, MAX_PREVIEWCHARSET+8 );
#else
			ZeroMemory( lpMsgBody[iVp1Count].CharSet, MAX_PREVIEWCHARSET+3 );
#endif
		}
		else
		{
			lpMsgBody[iVp1Count].btObjType	 = OBJTYPE_NPC;
#ifdef ITEM_INDEX_EXTEND_20050706
			ZeroMemory( lpMsgBody[iVp1Count].CharSet, MAX_PREVIEWCHARSET+8);
#else
			ZeroMemory( lpMsgBody[iVp1Count].CharSet, MAX_PREVIEWCHARSET+3);
#endif
		}

		iVp1Count++;

		if( lpTargetObj->Type == OBJTYPE_CHARACTER )
		{
			// �� Weapon�� Ÿ�� ���� ĳ���͵鿡�� �˸�
			DataSend( lpTargetObj->m_Index, (LPBYTE)&pNotifyRegionMsg, sizeof(pNotifyRegionMsg) );
			
			// �� Weapon���� ���� ���� ĳ���͵鿡�� �˸�
			if( abs(lpTargetObj->X - iTargetX) > 3  )
				continue;
			
			if( abs(lpTargetObj->Y - iTargetY) > 3 )
				continue;
			
			pNotifyTargetMsg.NumberH = HIBYTE(lpTargetObj->m_Index);
			pNotifyTargetMsg.NumberL = LOBYTE(lpTargetObj->m_Index);
			
			// ������ ������ �����.
//			DataSend( lpTargetObj->m_Index, (LPBYTE)&pNotifyTargetMsg, sizeof(pNotifyTargetMsg) );
//			MsgSendV2( lpTargetObj, (LPBYTE)&pNotifyTargetMsg, sizeof(pNotifyTargetMsg) );
		}
		
	}

	if (iVp1Count > 0 && iVp1Count <= 100 ) 
	{
		lpMsg->btCount = iVp1Count;
		PHeadSetW((LPBYTE) lpMsg, 0x68, sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT) + (sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY) * iVp1Count));
		DataSend (lpOwnerObj->m_Index, (LPBYTE) lpMsg, MAKEWORD(lpMsg->h.sizeL, lpMsg->h.sizeH));
	}
}

void gObjNotifyUseWeaponDamage( LPOBJECTSTRUCT lpWeaponObj, INT iTargetX, INT iTargetY )
{

	// 1. �������� ���Ǵ� �߼���, �����⿡ �ش��ϴ� ������ ����.
	if( lpWeaponObj->Class == NPC_SIEGE_MACHINE_ATTACK || lpWeaponObj->Class == NPC_SIEGE_MACHINE_DEFENSE )
	{
		for( int n = 0 ; n < MAX_OBJECT ; n++ )
		{
			LPOBJECTSTRUCT lpTargetObj = &gObj[n];
			
			// ��ġ �˻�.
			if( lpWeaponObj->MapNumber != lpTargetObj->MapNumber )
				continue;
			
			if( abs(lpTargetObj->X - iTargetX) > 3  )
				continue;
			
			if( abs(lpTargetObj->Y - iTargetY) > 3  )
				continue;
			
			g_CsNPC_Weapon.AddWeaponDamagedTargetInfo( lpWeaponObj->m_Index, n, 100 );
			
		}	
	}

	// 2. �ٸ� ���⿡ �ش��ϴ� Damage ó�� 
	
}

#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116

#define LEVEL_USEDRINK_NUMBER	77

void gObjUseBlessAndSoulPotion( INT aIndex, INT iItemLevel )
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( iItemLevel == 0 ) // �ູ�� ����
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjAddBuffEffect( lpObj, BUFFTYPE_BLESS_POTION, 0, 0, 0, 0, CASTLE_BLESSPOTION_ACTIVETIME );
#else
		lpObj->m_iPotionBlessTime = CASTLE_BLESSPOTION_ACTIVETIME;
		//((���� �� ������*2)/100) >= 500
		
		// ����, ���� ���ݽ� ������ ���� +2%

		ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_BLESS_POTION );
		GCStateInfoSend( lpObj, 1, STATE_BLESS_POTION );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		//LogAddTD("[CastleSiege] Using BlessPotion - [%s][%s]", lpObj->AccountID, lpObj->Name );
	}
	else if( iItemLevel == 1 ) // ��ȥ�� ����
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjAddBuffEffect( lpObj, BUFFTYPE_SOUL_POTION, 0, 0, 0, 0, CASTLE_SOULPOTION_ACTIVETIME );
#else
		lpObj->m_iPotionSoulTime = CASTLE_SOULPOTION_ACTIVETIME;

		ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_SOUL_POTION );
		GCStateInfoSend( lpObj, 1, STATE_SOUL_POTION );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

		// AG ȸ������ 8 Point �� ���Ѵ�.
		
		// ������ ȿ��
		gObjUseDrink( lpObj, LEVEL_USEDRINK_NUMBER );

		//LogAddTD("[CastleSiege] Using SoulPotion - [%s][%s]", lpObj->AccountID, lpObj->Name );
	}

}

void gObjWeaponDurDownInCastle( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, INT iDecValue )
{

	if( lpObj->Type == OBJTYPE_MONSTER || lpObj->Type == OBJTYPE_NPC)
	{	// ���Ͱ� �����ϴ� ���� ������ ���� �ش���׾���
		return;
	}

	int	itargetdefence = 0;
	
	CItem	*Right = &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT];
	CItem	*Left  = &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT];	
	
	
	BOOL	bIsRightDurDown = FALSE;
	BOOL	bIsLeftDurDown = FALSE;

	//��� ���� �϶�
	if(lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD )
	{
		if( Right->m_Type >= 0 && Right->m_Type < ITEM_BOW && Left->m_Type >= 0 && Left->m_Type < ITEM_BOW )
		{
			bIsRightDurDown = TRUE;
			bIsLeftDurDown = TRUE;
		}
	}

	//Ȱ�϶�
	if( (Left->m_Type >= ITEM_BOW && Left->m_Type < ITEM_BOW+7)
#ifdef MODIFY_APPLYING_BOW_01_20050825
			|| Left->m_Type == ITEM_BOW + 20
			|| Left->m_Type == ITEM_BOW + 21
#endif
#ifdef MODIFY_DARKSTINGER_BUGFIX_20081119
			|| Left->m_Type == ITEM_BOW + 22
			|| Left->m_Type == ITEM_BOW + 23
#endif // MODIFY_DARKSTINGER_BUGFIX_20081119
#ifdef ADD_AERINBOW_20090309
			|| Left->m_Type == ITEM_BOW + 24
#endif // ADD_AERINBOW_20090309
		)
	{
		bIsLeftDurDown = TRUE;
	}
	//�����϶�
	else if(( Right->m_Type >= ITEM_BOW+8 && Right->m_Type < ITEM_BOW+15) ||
			  ( Right->m_Type >= ITEM_BOW+16 && Right->m_Type < ITEM_BOW+MAX_ITEM_INDEX))
	{
		bIsRightDurDown = TRUE;
	}
	//�Ϲݹ���
	else if( Right->m_Type >= 0 && Right->m_Type < ITEM_BOW )
	{
		bIsRightDurDown = TRUE;
	}
	//������
	else if( Right->m_Type >= ITEM_STAFF && Right->m_Type < ITEM_SHIELD )
	{
		bIsRightDurDown = TRUE;
	}

	if( bIsRightDurDown )
	{
		INT iRet = Right->SimpleDurabilityDown( iDecValue );
		if( iRet )
		{
			GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, Right->m_Durability, 0);
			if( iRet == 2 )
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
	}

	if( bIsLeftDurDown )
	{
		INT iRet = Right->SimpleDurabilityDown( iDecValue );
		if( iRet )
		{
			GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, Left->m_Durability, 0);
			if( iRet == 2 )
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
	}
}

#endif


#ifdef HAPPY_CAT
// �ٽ� �츮�� ���� ó���ؾ� �� �ϵ� �߰�
void	gObjReady4Relife(LPOBJECTSTRUCT lpObj)
{
#ifdef HAPPY_POUCH
	lpObj->m_lFreeKalimaTime	= 0;	
	lpObj->m_btLifeInc			= 0;
	lpObj->m_lLifeIncTime		= 0;

#ifdef ITEM_ADD_OPTION_20061019
	g_ItemAddOption.ClearItemEffect( lpObj, ITEMUSE_EFFECT_OPTION_CANCEL );
#endif // ITEM_ADD_OPTION_20061019

	gObjCalCharacter(lpObj->m_Index);//???

// CODEREVIEW_SHIELD : iAddShield �߰�
#ifdef ADD_SHIELD_POINT_01_20060403
	GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
#else
	GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0 );
#endif

#endif
}
#endif

#ifdef MODIFY_SEASON_01_20060317

	#ifndef HAPPY_CAT
		// �ٽ� �츮�� ���� ó���ؾ� �� �ϵ� �߰�
		void	gObjReady4Relife(LPOBJECTSTRUCT lpObj)
		{
	#ifdef HAPPY_POUCH
			lpObj->m_lFreeKalimaTime	= 0;	
			lpObj->m_btLifeInc			= 0;
			lpObj->m_lLifeIncTime		= 0;
	#endif

			gObjCalCharacter(lpObj->m_Index);//???
		#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
		#else
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0 );
		#endif
	}
	#endif // HAPPY_CAT

#endif // MODIFY_SEASON_01_20060317

#ifdef PCBANG_ADVANTAGE
BOOL	IsOKPCBangBenefit(LPOBJECTSTRUCT lpObj)
{

#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
	// PC�� ���� ���θ� �˻��Ѵ�.
	if( g_iApplyPCBangAdvantage == 0 )
	{
		return FALSE;
	}

	if( lpObj->m_bPCBangUser == FALSE )
	{
		return FALSE;
	}
#endif //PCBANG_ADVANTAGE_EXPAND_20060919

	if( CHECK_BLOODCASTLE(lpObj->MapNumber) ) 
		return FALSE;

	if( CHECK_CHAOSCASTLE(lpObj->MapNumber) )
		return FALSE;

#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
	if( lpObj->MapNumber == MAP_INDEX_DEVILSQUARE )
#endif
		return FALSE;	

	return lpObj->m_bPCBangUser;
}
#endif //PCBANG_ADVANTAGE

#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
BOOL IsOKPCBangBenefitAll(LPOBJECTSTRUCT lpObj)
{
	// PC�� ���� ���θ� �˻��Ѵ�.
	if( g_iApplyPCBangAdvantage == 0 )
	{
		return FALSE;
	}

	// PC�� �������� �˻��Ѵ�.
	if( lpObj->m_bPCBangUser == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}
#endif //PCBANG_ADVANTAGE_EXPAND_20060919


#ifdef HAPPY_CAT
void	gObjCheckTimeOutValue(LPOBJECTSTRUCT lpObj, DWORD & rNowTick)
{
	LONG lTick = rNowTick - lpObj->m_dwLastCheckTick;
	LONG lSecond = lTick / 1000;	

	if (lSecond == 0) return;
#ifdef HAPPY_POUCH
	if (lpObj->m_lFreeKalimaTime)
	{
		if (lpObj->m_lFreeKalimaTime > lSecond)
		{
			lpObj->m_lFreeKalimaTime -= lSecond;
		}
		else
		{
			lpObj->m_lFreeKalimaTime = 0;		
		}
	}

	if (lpObj->m_lLifeIncTime)
	{	
		if (lpObj->m_lLifeIncTime > lSecond)
		{
			lpObj->m_lLifeIncTime -= lSecond;
		}
		else
		{
			lpObj->m_lLifeIncTime		= 0;
			lpObj->m_btLifeInc			= 0;

			gObjCalCharacter(lpObj->m_Index); //alicecat@
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
#else
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0 );
#endif
		}
	}
#endif//HAPPY_POUCH

	lpObj->m_dwLastCheckTick = rNowTick;
}
#endif//HAPPY_CAT

#ifdef MODIFY_SEASON_01_20060317
	#ifndef HAPPY_CAT
	void	gObjCheckTimeOutValue(LPOBJECTSTRUCT lpObj, DWORD & rNowTick)
	{
		LONG lTick = rNowTick - lpObj->m_dwLastCheckTick;
		LONG lSecond = lTick / 1000;	

		if (lSecond == 0) return;
	#ifdef HAPPY_POUCH
		if (lpObj->m_lFreeKalimaTime)
		{
			if (lpObj->m_lFreeKalimaTime > lSecond)
			{
				lpObj->m_lFreeKalimaTime -= lSecond;
			}
			else
			{
				lpObj->m_lFreeKalimaTime = 0;		
			}
		}

		if (lpObj->m_lLifeIncTime)
		{	
			if (lpObj->m_lLifeIncTime > lSecond)
			{
				lpObj->m_lLifeIncTime -= lSecond;
			}
			else
			{
				lpObj->m_lLifeIncTime		= 0;
				lpObj->m_btLifeInc			= 0;

				gObjCalCharacter(lpObj->m_Index); //alicecat@
				GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0 );
			}
		}
	#endif//HAPPY_POUCH

		lpObj->m_dwLastCheckTick = rNowTick;
	}
	#endif // HAPPY_CAT
#endif // MODIFY_SEASON_01_20060317

#if TESTSERVER == 1
void	DebugOutput(int aIndex, char* msg, ...)
{
//#if TESTSERVER == 1
	char		szBuffer[512]="";
	va_list		pArguments;
	
	va_start(pArguments, msg);
    vsprintf(szBuffer, msg, pArguments);
    va_end(pArguments);
			
	GCServerMsgStringSend(szBuffer, aIndex, 1);	
//#endif
}
#endif



void	MsgOutput(int aIndex, char* msg, ...)
{
	char		szBuffer[512]="";
	va_list		pArguments;
	
	va_start(pArguments, msg);
    vsprintf(szBuffer, msg, pArguments);
    va_end(pArguments);
			
	GCServerMsgStringSend(szBuffer, aIndex, 1);	
}

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
void gProcessAutoRecuperation(LPOBJECTSTRUCT lpObj)
{
	_ASSERT(lpObj);

	if(lpObj->Life == lpObj->MaxLife + lpObj->AddLife &&
		   lpObj->Mana == lpObj->MaxMana + lpObj->AddMana &&
		   lpObj->BP == lpObj->MaxBP + lpObj->AddBP) {
		
		lpObj->m_iAutoRecuperationTime = GetTickCount();
		return;
	}
	
	// 10 Sec - 15 Sec
	if( (GetTickCount() - lpObj->m_iAutoRecuperationTime) >= CART_10_SEC &&
		(GetTickCount() - lpObj->m_iAutoRecuperationTime) <  CART_15_SEC ) {
		
		// ���� ����
		if(lpObj->Life < lpObj->MaxLife + lpObj->AddLife) {
			
			lpObj->Life += VPT_HP_MP_AG_1_PER_1_SEC;

			if(lpObj->Life > lpObj->MaxLife + lpObj->AddLife)
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;

#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0);
#endif
		}
		// ���� �� ��ų ���� ����
		if(lpObj->Mana < lpObj->MaxMana + lpObj->AddMana ||
		   lpObj->BP < lpObj->MaxBP + lpObj->AddBP) {

			lpObj->Mana += VPT_HP_MP_AG_1_PER_1_SEC;

			if(lpObj->Mana > lpObj->MaxMana + lpObj->AddMana)
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;

			lpObj->BP += VPT_HP_MP_AG_1_PER_1_SEC;

			if(lpObj->BP > lpObj->MaxBP + lpObj->AddBP)
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;

			GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
		}	
	}
	// 15 sec - 25 sec
	else if( (GetTickCount() - lpObj->m_iAutoRecuperationTime) >= CART_15_SEC &&
			 (GetTickCount() - lpObj->m_iAutoRecuperationTime) <  CART_25_SEC ) {
		
		// ���� ����
		if(lpObj->Life < lpObj->MaxLife + lpObj->AddLife) {
			
			lpObj->Life += VPT_HP_MP_AG_5_PER_1_SEC;

			if(lpObj->Life > lpObj->MaxLife + lpObj->AddLife)
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;

#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0);
#endif
		}
		// ���� �� ��ų ���� ����
		if(lpObj->Mana < lpObj->MaxMana + lpObj->AddMana ||
		   lpObj->BP < lpObj->MaxBP + lpObj->AddBP) {

			lpObj->Mana += VPT_HP_MP_AG_5_PER_1_SEC;

			if(lpObj->Mana > lpObj->MaxMana + lpObj->AddMana)
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;

			lpObj->BP += VPT_HP_MP_AG_5_PER_1_SEC;

			if(lpObj->BP > lpObj->MaxBP + lpObj->AddBP)
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;

			GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
		}	
	}
	// over 25 sec
	else if( (GetTickCount() - lpObj->m_iAutoRecuperationTime) >= CART_25_SEC ) {

		// ���� ����
		if(lpObj->Life < lpObj->MaxLife + lpObj->AddLife) {
			
			lpObj->Life += VPT_HP_MP_AG_10_PER_1_SEC;

			if(lpObj->Life > lpObj->MaxLife + lpObj->AddLife)
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;

#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0);
#endif
		}
		// ���� �� ��ų ���� ����
		if(lpObj->Mana < lpObj->MaxMana + lpObj->AddMana ||
		   lpObj->BP < lpObj->MaxBP + lpObj->AddBP) {

			lpObj->Mana += VPT_HP_MP_AG_10_PER_1_SEC;

			if(lpObj->Mana > lpObj->MaxMana + lpObj->AddMana)
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;

			lpObj->BP += VPT_HP_MP_AG_10_PER_1_SEC;

			if(lpObj->BP > lpObj->MaxBP + lpObj->AddBP)
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;

			GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
		}	
	}
}
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
void gObjShieldAutoRefill(LPOBJECTSTRUCT lpObj)
{
	int iRefillPoint = 0.0f;
	// iShieldRefillOption ���� �����ۿ� ���� ȸ�� �ɼ��� ���� ��� ���ȴ�. (���� ������)
	int iShieldRefillOption = 0;

	if( g_ShieldSystemOn == FALSE )
	{
		return;
	}

	// ���� �ڵ� ȸ�� �ɼ� �߰�
	if( g_ShieldAutoRefillOn == FALSE )
	{
		return;
	}

	// ���� ���뿡���� ȸ���� �����ϵ���
	if( g_ShieldAutoRefillOnSafeZone == TRUE )
	{
		BYTE btMapAttribute;
		btMapAttribute = MapC[lpObj->MapNumber].GetAttr( lpObj->X, lpObj->Y );
		if( ( btMapAttribute&MAP_ATTR_SAFTYZONE ) != MAP_ATTR_SAFTYZONE )
		{
	#ifdef ADD_380ITEM_NEWOPTION_20060711
			// 380 �߰� �ɼ� �� SD �ڵ� ȸ�� �ɼ��� �ִ� ��� ó��
			if ( lpObj->m_ItemOptionExFor380.OpRefillOn == 0 )
			{
				// ���� Ƽ�� �ʱ�ȭ �ʿ�.
				lpObj->dwShieldAutoRefillTimer = GetTickCount();
				return;
			}
	#else
			return;
	#endif // ADD_380ITEM_NEWOPTION_20060711
		}
	}

	if( lpObj->iShield >= ( lpObj->iMaxShield + lpObj->iAddShield ) )
	{
		lpObj->dwShieldAutoRefillTimer = GetTickCount();
		return;
	}

#ifdef ADD_380ITEM_NEWOPTION_20060711
	// 380 �߰� �ɼ� ������ : SD ȸ���� ���� �ɼ� ����
	iShieldRefillOption = lpObj->m_ItemOptionExFor380.OpAddRefillSD;
#endif // ADD_380ITEM_NEWOPTION_20060711

	int iRefillExpression = 0;

	float fRefillExpressionA = ( lpObj->iMaxShield + lpObj->iAddShield ) / 30;
	float fRefillExpressionB = ( iShieldRefillOption  + 100 );

#ifdef ADD_380ITEM_NEWOPTION_20060711
	// ���� ���� : +1 �� ���������� ó���Ѵ�.
	iRefillExpression = fRefillExpressionA * fRefillExpressionB / 100 / 25;
#else
	iRefillExpression = fRefillExpressionA * fRefillExpressionB / 100 / 25 + 1;
#endif // ADD_380ITEM_NEWOPTION_20060711

	//iRefillExpression = ( ( lpObj->iMaxShield + lpObj->iAddShield ) / 10 ) * ( iShieldRefillOption / 100 + 1 ) / 25;
	//iRefillExpression = ( ( 2 * ( lpObj->iMaxShield + lpObj->iAddShield ) / 30 ) * ( ( ( iShieldRefillOption / 100 ) + 1 ) / 25 ) + 1;

// CODEREVIEW_SHIELD : iShieldChargeOption ��뿡 ���� �ּ�

	DWORD dwTick = GetTickCount() - lpObj->dwShieldAutoRefillTimer;
	if( dwTick >= 25 * 1000 )
	{
		iRefillPoint = iRefillExpression * 3;
	}
	else if( dwTick >= 15 * 1000 )
	{
		iRefillPoint = iRefillExpression * 25 / 10;
	}
	else if( dwTick >= 10 * 1000 )
	{
		iRefillPoint = iRefillExpression * 2;
	}
#ifdef ADD_380ITEM_NEWOPTION_20060711
	else
	{
		// �ּ� �ð� 10�ʰ� ���� ���� ��� ȸ���� ���� �ʴ´�.
		return;
	}

	// ���� ���� : +1�� �������� ó��.
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// SD ���� �ӵ�
	iRefillPoint += 1;
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	iRefillExpression += 1;
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
#endif // ADD_380ITEM_NEWOPTION_20060711

	if( iRefillPoint == 0 )
	{
		return;
	}

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �����ͷ��� 4��ũ - SD ȸ�� �ӵ� ���
	iRefillPoint += iRefillPoint * lpObj->m_MPSkillOpt.iMpsSDSpeed / 100;
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428

#ifdef SHIELD_POINT_DEBUGLOG
	char Msg[256];
	sprintf(Msg,"[Shield] Shield Charging %d/%d", lpObj->iShield, iRefillPoint );
	GCServerMsgStringSend( Msg, lpObj->m_Index, 1 );
#endif

	lpObj->iShield = lpObj->iShield + iRefillPoint;

	// CODEREVIEW_SHIELD : iMaxShield �� �� ã�� iAddShield �߰� �� ��!
	if( lpObj->iShield > ( lpObj->iMaxShield + lpObj->iAddShield ) )
	{
		lpObj->iShield = ( lpObj->iMaxShield + lpObj->iAddShield );
	}

#ifdef ADD_380ITEM_NEWOPTION_20060711
	#if TESTSERVER == 1
		char Msg[256];
		sprintf(Msg,"[Shield] ���� %d ����:%d", lpObj->iShield, iRefillPoint );
		GCServerMsgStringSend( Msg, lpObj->m_Index, 1 );
	#endif
#endif

	GCReFillSend( lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield );
}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
int gObjCheckOverlapItemUsingDur( int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel )
{
	for(int x=MAX_EQUIPMENT; x<MAX_INVENTORY; x++)
	{
		if( gObj[iUserIndex].pInventory[x].IsItem() == TRUE )
		{
			if( 
				gObj[iUserIndex].pInventory[x].m_Type == (short)iItemType 
				&& gObj[iUserIndex].pInventory[x].m_Level == (short) iItemLevel
			  )
			{
				// ������ ǥ���� �����Ѵٸ�
				INT iITEM_DUR	= (INT) gObj[iUserIndex].pInventory[x].m_Durability;	// ������ ���� (������) �� ��´�.

				if (CHECK_LIMIT(iITEM_DUR, iMaxOverlapped)) 
				{
					// ���� M�� �̸��̶�� ���⿡ �� ���� �� �ִ�.
					return x;
				}
			}
		}
	}

	return -1;
}

int gObjOverlapItemUsingDur( CItem *lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel )
{
	for (INT iLoop = 0 ; iLoop < MAX_INVENTORY ; iLoop++) 
	{
		INT iInventoryIndex	= gObjCheckOverlapItemUsingDur(iUserIndex, iMaxOverlapped, iItemType, iItemLevel);
		
		if (CHECK_LIMIT(iInventoryIndex, MAX_INVENTORY)) 
		{
			INT iItemDur	= gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability + lpItem->m_Durability;
			
			if (iItemDur <= iMaxOverlapped) 
			{
				// �� �ڸ��� �� ��������.
				if( MapC[iMapNumber].ItemGive(iUserIndex, iItemNumber, true) == TRUE )
				{
					// ! ���� �ڵ���� ȣȯ���� ���� �Ʒ� �ּ� ó�� �κ��� �� �Լ��� ȣ���� �κп��� ó���ϵ��� �Ѵ�. (apple)

					// �������� �������̹Ƿ� �ʵ忡���� ������� �Ѵ�.
//					pResult.result	= 0xFD;				
//					DataSend(iUserIndex, (LPBYTE)&pResult, pResult.h.size);
//					gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;
					
//					GCItemDurSend(iUserIndex, iInventoryIndex, (int)gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability, 0);

					return iInventoryIndex;
				}
			}
			else 
			{
				// ��ġ�� ���� ���´�.
				lpItem->m_Durability	= iItemDur - iMaxOverlapped;
				gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability = iMaxOverlapped;
				
				GCItemDurSend(iUserIndex, iInventoryIndex, (int)gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability, 0);
			}
		}
		else 
		{
			// ��ĥ �� �ִ� �ڸ��� �����Ƿ� ���� �������� �ִ� ������� �ִ´�.
			return -1;
		}
		
	}

	return -1;	
}
#endif



#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202
BOOL gObjCheckSerial0ItemList( CItem *lpItem )
{
#ifdef MODIFY_SERIAL_CHECK_OPTION_RELOAD_20080212
	if(gItemSerialCheck==FALSE) return FALSE;
#else
	if(gItemSerialCheck==FALSE) return TRUE;
#endif // MODIFY_SERIAL_CHECK_OPTION_RELOAD_20080212
	
	if( lpItem->m_Type == MAKE_ITEMNUM(14, 13)		// �༮
		|| lpItem->m_Type == MAKE_ITEMNUM(14, 14)	// ����
		|| lpItem->m_Type == MAKE_ITEMNUM(14, 16)	// ����
		|| lpItem->m_Type == MAKE_ITEMNUM(14, 22)	// â��
		|| lpItem->m_Type == MAKE_ITEMNUM(12, 15)	// ȥ��
		|| lpItem->m_Type == MAKE_ITEMNUM(12, 30)	// �༮ ����
		|| lpItem->m_Type == MAKE_ITEMNUM(12, 31)	// ���� ����
		|| lpItem->m_Type == MAKE_ITEMNUM(14, 31)	// ��ȣ ����
	  )
	{
		if( lpItem->GetNumber() == 0 )
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}

BOOL gObjCheckInventorySerial0Item(LPOBJECTSTRUCT lpObj)
{

#ifdef MODIFY_SERIAL_CHECK_OPTION_RELOAD_20080212
	if(gItemSerialCheck==FALSE) return FALSE;
#else
	if(gItemSerialCheck==FALSE) return TRUE;
#endif // MODIFY_SERIAL_CHECK_OPTION_RELOAD_20080212

	int iItemSerial;
	int iCount = 0;
	
#ifdef PERSONAL_SHOP_20040113
	for( int i=0; i<MAX_INVENTORY_EXTEND; i++)
#else
	for( int i=0; i<MAX_INVENTORY; i++)
#endif
	{
		// 1. > �����ϴ� ���������� �˻�
		if( !lpObj->pInventory[i].IsItem() )
		{
			continue;
		}

		// 2. > �ø��� üũ�� ���ϴ� ���������� �˻�
		if( !gObjCheckSerial0ItemList(&lpObj->pInventory[i]) )
		{
			continue;
		}
		
		// 3. > �ø��� 0 ���� �˻�
		iItemSerial = lpObj->pInventory[i].GetNumber();

		// 4. > �α� ���
		if( iItemSerial==0 )	
		{	
			LogAddTD( "[ANTI-HACK][Serial 0 Item] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID, lpObj->Name, lpObj->pInventory[i].GetName(), i );

			//gObjUserKill(lpObj->m_Index);
			iCount++;
		}
	}
	
	if( iCount > 0 )
		return TRUE;
	
	return FALSE;
}


#endif // ANTIHACKING_SERIAL_0_ITEM_20051202


// ��ų ���� ��, ������ ���� ��ġ���� Ȯ��
#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413
BOOL gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum)
{
	// üũ �ý��� ������� ������ ������ true ��ȯ
	if ( g_iSkillDistanceCheck == 0 )
		return TRUE;

	// �����Ʈ ��ų�� �Ÿ� üũ ���� 
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
	if ( iSkillNum == AT_SKILL_EXPHELL )
	{
		return TRUE;
	}
#endif
	
	// ��ų ��ȿ �Ÿ� ��
	int iSkillDistance = MagicDamageC.GetSkillDistance(iSkillNum);
	if ( iSkillDistance == -1 )
		return FALSE;

	// ���� ��
	iSkillDistance += g_iSkillDistanceCheckTemp;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj = &gObj[aTargetIndex];

	// �Ÿ� ��� 
	if ( abs(lpObj->X - lpTargetObj->X) > iSkillDistance 
		|| abs(lpObj->Y - lpTargetObj->Y) > iSkillDistance )
	{
		// ��ȿ �Ÿ� ���.
		LogAddTD("[SKILL DISTANCE CHECK] [%s][%s] Invalid Skill attacker(%d, %d), defender(%d, %d), skillnum:%d skilldistance:%d", 
				  lpObj->AccountID, 
				  lpObj->Name, 
				  lpObj->X,
				  lpObj->Y,
				  lpTargetObj->X,
				  lpTargetObj->Y,
				  iSkillNum,
				  iSkillDistance);
		return FALSE;
	}

	return TRUE;
}
#endif //MODIFY_SKILL_CHECK_DISTANCE_20060413

#ifdef MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328
void gObjSaveChaosBoxItemList(LPOBJECTSTRUCT lpObj)
{
	BYTE ExOption[8];

	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[ChaosBoxItemList][Lost ItemList] [%s][%s] [%d,%s,%d,%d,%d,%d] Serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]", 
#else
			LogAddTD("[ChaosBoxItemList][Lost ItemList] [%s][%s] [%d,%s,%d,%d,%d,%d] Serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, 
					n, 
					lpObj->pChaosBox[n].GetName(),
					lpObj->pChaosBox[n].m_Level,
					lpObj->pChaosBox[n].m_Option1,
					lpObj->pChaosBox[n].m_Option2,
					lpObj->pChaosBox[n].m_Option3, 
					lpObj->pChaosBox[n].m_Number, 
					(int)lpObj->pChaosBox[n].m_Durability,				
					ExOption[0],
					ExOption[1],
					ExOption[2],
					ExOption[3],
					ExOption[4],
					ExOption[5],
					ExOption[6],
					lpObj->pChaosBox[n].m_SetOption,
					lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pChaosBox[n] ),
					g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pChaosBox[n] )
					);
		}
	}
}
#endif // MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
// �����ʱ�ȭ
void gStatReset( int iIndex )
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	
	// ���� ����
	int Class = lpObj->Class;
	
	lpObj->ChangeUP = 0;
	//lpObj->Experience = 0;
	
	lpObj->Strength = DCInfo.DefClass[Class].Strength;
	lpObj->Dexterity = DCInfo.DefClass[Class].Dexterity;
	lpObj->Vitality = DCInfo.DefClass[Class].Vitality;
	lpObj->Energy = DCInfo.DefClass[Class].Energy;
	lpObj->Life = DCInfo.DefClass[Class].Life;
	lpObj->MaxLife = DCInfo.DefClass[Class].MaxLife;
	
	lpObj->Mana = DCInfo.DefClass[Class].Mana;
	lpObj->MaxMana = DCInfo.DefClass[Class].MaxMana;			
	
#ifdef DARKLORD_WORK
	lpObj->Leadership = DCInfo.DefClass[Class].Leadership;
#endif
	
#ifdef DARKLORD_WORK
	if (lpObj->Class == CLASS_DARKLORD||lpObj->Class == CLASS_MAGUMSA) 
#else			
		if( lpObj->Class == CLASS_MAGUMSA )
#endif
			lpObj->LevelUpPoint = (lpObj->Level-1)*7;
		else
			lpObj->LevelUpPoint = (lpObj->Level-1)*5;
		
		GCServerMsgStringSend("ĳ���� ������ ���µǾ����ϴ�. ���� ���� �ϼ���", lpObj->m_Index, 1);
		gObjCloseSet(lpObj->m_Index, CL_BACK);

}
#endif //EXPERIENCE_SERVER_NPC_STAT_RESET

//------------->
//20080930 �ؿ� 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
BOOL	gIsMoveCharmEnable(int iMapNum)
{	
	switch(iMapNum)
	{
	case		MAP_INDEX_BATTLESOCCER:		//	=	6,	//�����
	case		MAP_INDEX_RORENCIA:			//	=	0,	//�η��þ�
	case		MAP_INDEX_NORIA:			//	=	3,	//�븮��
	case		MAP_INDEX_ELBELAND:			//	=	51,	//��������
	case		MAP_INDEX_DEVIAS:			//	=	2,	//����ƽ�
	case		MAP_INDEX_DUNGEON:			//	=	1,	����
	case		MAP_INDEX_ATHLANSE:			//	=	7,	��Ʋ����
	case		MAP_INDEX_LOSTTOWER :		//	=	4,	�ν�ƮŸ��
	case		MAP_INDEX_TARKAN:			//	=	8,	Ÿ��ĭ.
	case		MAP_INDEX_AIDA:				//	=	33, ���̴� ��
	case		MAP_INDEX_ICARUS:			//	=	10,	��ī�罺
	case		MAP_INDEX_KANTURU1:			//	=	37,	//ĭ���� 1����
	case		MAP_INDEX_KANTURU2:			//	=	38,	//ĭ���� 2����
#ifdef ADD_RAKLION_20080408
	case		MAP_INDEX_RAKLION_FIELD:	//	=	57,	//���� ȣ�� �ʵ�
#endif
	case		MAP_INDEX_SWAMP_OF_CALMNESS://	=	56,	//����Ǵ�
		return TRUE;
	}
	
	return FALSE;
}
#endif //ADD_TELEPORT_CHARM_ITEM_20080930
//<-------------

#ifdef ADD_GAMBLING_20090120	// �κ��丮�� �Է� ���� width * height�� ������ �ִ��� üũ
BOOL gObjGamblingInventoryCheck( int aIndex, int iWidth, int iHeight )
{
	int h = 0;
	int w = 0;
	BYTE blank = 0;
	
	for( h=0; h<8; h++)
	{		
		for( w=0; w<8; w++)	
		{
			// ����ִ��� üũ
			if( *(gObj[aIndex].pInventoryMap+(h*8)+w) == 0xFF )
			{
				blank = gObjOnlyInventoryRectCheck( aIndex, w, h, iWidth, iHeight);
				// 
				if( blank == 0xFE )
				{
					return FALSE;
				}
				
				if( blank != 0xFF )	// �κ��丮�� �ڸ��� �ִٸ�
				{
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}
#endif // ADD_GAMBLING_20090120
