
//----------------------------------------------------------------------------
// GAME SERVER - GAMEMAIN.CPP
// ���Ӽ����� ���� ���
//----------------------------------------------------------------------------
#include "stdafx.h"
#include <time.h>
#include <stdio.h> 
#include "Include\Prodef.h"
#include "Include\SProdef.h"
#include "Common\winutil.h"
#include "Common\zzzItem.h"
#include "Common\Classdef.h"
#include "wsGameServer.h"
#include "wsJoinServerCli.h"
#include "User.h"
#include "Sprotocol.h"
#include "DSprotocol.h"
#include "News.h"
#include "CalCharacter.h"
#include "OffExp.h"
#include "HappyHour.h"
#include "PickUpSystem.h"

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
//	#include "Include\ggsrv.h"			// Ŭ���̾�Ʈ ���� ������
	#include "ggsrv.h"						// ���� ��Ʈ�� ���� (b4nfter)
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	#include "ggsrv25.h"						// ���� ��Ʈ�� ���� (hnine)
#endif

#ifdef _NEW_EXDB_

#include "Protocol.h"

//##EXDB##
#include "EDSprotocol.h"
//##EXDB##
#endif


#include "Protocol.h"
#include "MapClass.h"
#include "Shop.h"
#include "MonsterAttr.h"
#include "MonsterSetBase.h"
#include "WhisperCash.h"
#include "MagicDamage.h"
#include "Gate.h"
#include "GameServer.h"
#include "PartyClass.h"
#include "AcceptIp.h"
#include "Dbsave.h"
#include "WzUdp.h"
#include "GuildClass.h"
#include "DragonEvent.h"
#include "Gamemain.h"
#include "ItemBag.h"
#include "DirPath.h"
#include "GMMng.h"
#include "BattleGround.h"
#include "BattleSoccer.h"
#include "ConMember.h"

#ifdef MODIFY_MIX_SYSTEM_20070518
//#include "ChaosBox.h"
#include "MixSystem.h"
#endif

#include "AttackEvent.h"
#include "SimpleModulus.h"
#include "NSerialCheck.h"

#include "DevilSquare.h"

#include "BattleSoccerManager.h"

#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#include "MonsterItemMng.h"
#include "Event.h"
#include "EledoradoEvent.h"

#ifdef NEW_SKILL_FORSKYLAND
	#include "QuestInfo.h"
#endif

#ifdef MULTI_CHECKSUM
#include "MultiCheckSum.h"
#endif

#include "SendHackLog.h"
#include "PacketCheckSum.h"

#ifdef EVENT_ITEMBAG_UPGRADE
#include "ItemBagEx.h"
#endif

#ifdef UPDATE_EVENT_ITEM_BAG_EXTEND_20070131
#include "ProbabilityItemBag.h"
#endif

#include "Common\SetItemOption.h"

#ifdef ADD_MOVE_COMMAND_SCRIPT
#include "MoveCommand.h"
#endif

#ifdef AUTH_GAMESERVER
#include "GameServerAuth.h"
#endif

#ifdef CHANGE_CHAOSCASTLE_SET_ITEM_DROP
#include "ChaosCastleSetItemDrop.h"
#endif

#ifdef DARKLORD_WORK
#include "SkillHitBox.h"
#endif

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
#include "WTEventItemList.h"
#endif

#ifdef FOR_CASTLE_TESTSERVER
#include "CCastleEventItemList.h"
#endif

#ifdef MAP_SERVER_WORK_20041030			// �ʼ��� ���� ���� Ŭ����
#include "MapServerManager.h"
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// �������� �� ���������� ����
#include "CastleSiege.h"
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
#include "ChinaHackUserKick.h"
#endif

#ifdef HAPPY_POUCH //happycat@20050201
#include "GameEvent.h" 
#endif


#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
#include "TMonsterSkillElement.h"
#include "TMonsterSkillUnit.h"
#include "TMonsterSkillManager.h"

#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
#include "TMonsterAI.h"
#include "TMonsterAIUnit.h"
#include "TMonsterAIAutomata.h"
#include "TMonsterAIGroup.h"
#endif

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
#include "Crywolf.h"
#endif

#ifdef MU_CRYWOLF_SERVER_SYNC_20050505
#include "CrywolfSync.h"
#endif

#ifdef ADD_SKILL_ADDITION_INFO_CLASS_20060407
#include "SkillAdditionInfo.h"
#endif

#ifdef ADD_MACRO_FOR_SETITEM_20060424
#include "SetItemMacro.h"
CSetItemMacro gSetItemMacro;
#endif

#ifdef ADD_KANTURU_20060627
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#endif

#ifdef ADD_RAKLION_20080408
#include "Raklion.h"
#endif // ADD_RAKLION_20080408

#ifdef ITEM_ADD_OPTION_20061019
#include "itemaddoption.h"
#endif // ITEM_ADD_OPTION_20061019

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#include "CashShop.h"
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	#include "EventItemBagExMng.h"
	#else
	#include "CashLotterySystem.h"
	#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#include "CashItemPeriodSystem.h"
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
#include "MasterSkillSystem.h"
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#include "ItemSocketOptionSystem.h"
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"
#endif // ADD_NEWPVP_PKFIELD

#ifdef ADD_GAMBLING_20090120
#include "GamblingSystem.h"
#endif // ADD_GAMBLING_20090120

#ifdef MODIFY_LUA_SCRIPT_20090430
#include "..\\Lua\\MULua.h"
#endif

// �� �̸����� ����
#ifdef FOR_BLOODCASTLE
CHAR	g_szMapName [MAX_MAP][32] = {
	"�η��þ�",
	"����",
	"����ƽ�",
	"�븮��",
	"�ν�Ʈ Ÿ��",
	"�ν�Ʈ Ÿ��",
	"�����",
	"��Ʋ����",
	"Ÿ��ĭ",
	"����������",
	"��ī�罺",
	"����ĳ��1",
	"����ĳ��2",
	"����ĳ��3",
	"����ĳ��4",
	"����ĳ��5",
	"����ĳ��6",
};
#else
	#ifdef NEW_SKILL_FORSKYLAND
CHAR	g_szMapName [MAX_MAP][32] = {
	"�η��þ�",
	"����",
	"����ƽ�",
	"�븮��",
	"�ν�Ʈ Ÿ��",
	"�ν�Ʈ Ÿ��",
	"�����",
	"��Ʋ����",
	"Ÿ��ĭ",
	"����������",
	"��ī�罺",
};
	#else
CHAR	g_szMapName [MAX_MAP][32] = {
	"�η��þ�",
	"����",
	"����ƽ�",
	"�븮��",
	"�ν�Ʈ Ÿ��",
	"�ν�Ʈ Ÿ��",
	"�����",
	"��Ʋ����",
	"Ÿ��ĭ",
	"����������",
};
	#endif
#endif



char *szGameServerVersion	= GAMESEVER_VERSION;		// ���Ӽ��� ����

//----------------------------------------------------------------------------
int GameServerPort;
int JoinServerPort;
int DataServerPort;

#ifdef ADD_GAMESERVER_PARAMETER_20050728
int DataServerPort2;
int ExDbPort;
#endif

int JoinServerConnected=0;
int DataServerConnected=0;
int GameServerCreated=0;

int DevilSquareEventConnect=0;
int IsDevilSquareEventConnected=0;


int EventChipServerConnect=0;
int IsEventChipServerConnected=0;


CwsGameServer	wsGServer;
wsJoinServerCli wsJServerCli;
wsJoinServerCli wsDataCli;

#ifdef GAMESERVER_INFO_TCP_20031203
int IsConnectServerConnected=0;
wsJoinServerCli	wsConnectServerCli;
#endif

#ifdef GAMESERVER_INFO_TO_CSS_20040317			// ���Ӽ����л� - ��������
#include <IO.H>									// _access()

#define			CSLIST_FILENAME					"CSList.dat"	// ���Ӽ��� ����Ʈ�� ���� ������ �̸�

WzUdp *			gpUdpSocCSS[MAX_CS_COUNT]		= {NULL,};		// ���Ӽ����� �����͸� ���� UDP ����
CHAR			g_szCSIPADDR[MAX_CS_COUNT][32]	= {0,};			// ���Ӽ����� �ּ� ����Ʈ
WORD			g_wCSPORT[MAX_CS_COUNT]			= {0,};			// ���Ӽ����� ��Ʈ ����Ʈ
int				g_iConnectServerCount			= 0;			// ���Ӽ����� ����
BOOL			g_bSendGSInfoToCSS				= false;		// �л�� ���Ӽ����� GS������ �����⸦ ������� ���� (FALSE�̸� ����������� �ѱ����� ������.)

VOID			GSINFO_INITCSLIST();				// ���Ӽ����� ����Ʈ�� �а� ������ �ʱ�ȭ�Ѵ�.
#endif


#ifdef _NEW_EXDB_
//##EXDB##
wsJoinServerCli wsExDbCli;
//##EXDB##
#endif


wsJoinServerCli wsRServerCli;			//  ��ŷ����

wsJoinServerCli wsEvenChipServerCli;	//  �̺�Ʈ Ĩ ����

CDirPath		gDirPath;
MapClass		*MapC;
CMonsterAttr	gMAttr;
CMonsterSetBase	gMSetBase;
classdef		DCInfo;
CWhisperCash	WhisperCash;
PartyClass		gParty;
CDbSave			gDbSave;
WzUdp			gUdpSoc;

WzUdp			gUdpSocCE;		// Chaos Event 
WzUdp			gUdpSocCER;		// Chaos Event Recv

WzUdp			gUdpSocCRank;		// Rank Server
WzUdp			gUdpSocCRankR;		// Rank ServerRecv


CGuildClass		Guild;
CDragonEvent*	DragonEvent		= NULL;
CAttackEvent*   AttackEvent		= NULL;
CItemBag	*   LuckboxItemBag	= NULL;	// ����ǻ��� ������ ����(?)
CItemBag	*   Mon55			= NULL;	// �η��þ� ���� ������ ����(?)
CItemBag	*   Mon53			= NULL;	// ����ƽ� ����  ������ ����(?)

#ifdef EVENT_ITEMBAG_UPGRADE
CItemBagEx	*   StarOfXMasItemBag = NULL;	// ��ź�� �� ������ ����(?)
#else
CItemBag	*   StarOfXMasItemBag = NULL;	// ��ź�� �� ������ ����(?)
#endif

CItemBag	*   FireCrackerItemBag = NULL;	// ����
CItemBag	*   HeartOfLoveItemBag = NULL;	// ����� ��Ʈ


CItemBag	*   GoldMedalItemBag = NULL;	// ������
CItemBag	*   SilverMedalItemBag = NULL;	// ������

CItemBag	*   EventChipItemBag = NULL;	// �̺�Ʈ Ĩ

#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
CProbabilityItemBag	*   GoldGoblenItemBag = NULL;	//	Ȳ�ݰ��
CProbabilityItemBag	*   TitanItemBag = NULL;		//	Ÿ��ź
CProbabilityItemBag	*   GoldDerconItemBag = NULL;	//  Ȳ�ݵ�����
CProbabilityItemBag	*   DevilLizardKingItemBag = NULL;	// ���� ���ڵ�ŷ
CProbabilityItemBag	*   KanturItemBag = NULL;	// ī����
#else //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
CItemBag	*   GoldGoblenItemBag = NULL;	//	Ȳ�ݰ��
CItemBag	*   TitanItemBag = NULL;		//	Ÿ��ź
CItemBag	*   GoldDerconItemBag = NULL;	//  Ȳ�ݵ�����
CItemBag	*   DevilLizardKingItemBag = NULL;	// ���� ���ڵ�ŷ
CItemBag	*   KanturItemBag = NULL;	// ī����
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04

#ifdef RING_EVENT_ITEMBOX_20031124
CItemBag	*   RingEventItemBag = NULL;			// ���������� �̺�Ʈ
#endif

#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211
CItemBag	*   FriendShipItemBag = NULL;			// ��ģ�� �̺�Ʈ - �����ǹ���
#endif

#ifdef DARK_LORD_HEART_EVENT_20040810
CItemBag	*   DarkLordHeartItemBag = NULL;	// ��ũ�ε��� ���� �̺�Ʈ
#endif

#ifdef MODIFY_KUNDUN_REWARD_01_20040915
CItemBagEx	*	KundunEventItemBag = NULL;	// ��� �̺�Ʈ
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303
CItemBagEx	*	CastleHuntZoneBossItemBag = NULL;		// �� ����� ����(������)
#endif

#ifdef MU_CHN_THANKS_COOKIE_EVENT_20040908 
CItemBagEx  *	ChnThanksCookieEvent = NULL;	// �߱� ���� �̺�Ʈ
#endif

#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
INT				g_iCastleItemMixLimit	= CASTLE_ITEM_MIX_MAXCOUNT;	// �������� Ư�� ������ ���� �Ϸ� ���� Ƚ��
CItemBagEx	*	CastleItemMixItemBag	= NULL;						// �������� Ư�� ������ ����
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
CItemBagEx *	HiddenTreasureBoxItemBag	= NULL;			// ������ �������� ������
#endif

#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212						// ũ�������� �������� �̺�Ʈ
CItemBagEx *	RedRibbonBoxEventItemBag	= NULL;				// ������������ 
CItemBagEx *	GreenRibbonBoxEventItemBag	= NULL;				// �ʷϸ������� 
CItemBagEx *	BlueRibbonBoxEventItemBag	= NULL;				// �Ķ���������
#endif

#ifdef VALENTINES_DAY_BOX_EVENT_20060124						// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ
CItemBagEx *	PinkChocolateBoxEventItemBag	= NULL;			// ��ȫ���ݸ����� 
CItemBagEx *	RedChocolateBoxEventItemBag		= NULL;			// �������ݸ����� 
CItemBagEx *	BlueChocolateBoxEventItemBag	= NULL;			// �Ķ����ݸ�����
#endif

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222						// ȭ��Ʈ���� �������� �̺�Ʈ
CItemBagEx *	LightPurpleCandyBoxEventItemBag	= NULL;			// ������������� 
CItemBagEx *	VermilionCandyBoxEventItemBag	= NULL;			// ��ȫ�������� 
CItemBagEx *	DeepBlueCandyBoxEventItemBag	= NULL;			// ��û��������
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
CItemBagEx *	CrywolfDarkElfItemBag			= NULL;			// ũ���̿��� ��ũ���� ������ ��
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
CItemBagEx *	CrywolfBossMonsterItemBag		= NULL;			// ũ���̿��� ��ũ���� ������ ��
#endif

#ifdef KANTURU_MONSTER_ITEMDROP_20060807
CItemBagEx *	KanturuMayaHandItemBag			= NULL;			// ĭ���� ���� �� ������ ��
CItemBagEx *	KanturuNightmareItemBag			= NULL;			// ĭ���� ����Ʈ�޾� ������ ��
#endif

#ifdef ADD_RAKLION_20080408
CProbabilityItemBag*	pRaklionSelupanItemBag	= NULL;			// ��Ŭ���� ������ ������ ��
#endif // ADD_RAKLION_20080408

#ifdef ADD_GAMBLING_20090120
CGamblingItemBag*	pGamblingSystemItemBag	= NULL;			// �׺� �ý��� ������ ��
#endif // ADD_GAMBLING_20090120

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
CItemBagEx *	HallowinDayEventItemBag			= NULL;			// �ҷ��� ���� �̺�Ʈ ������ ��
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119
CItemBag*	RingOfHeroEventItemBag				= NULL;			// New ����� ���� ������ ��
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
CItemBagEx*		PCBangEventNPCItemBag;					// PC�� �̺�Ʈ - NPC�� �����ϴ� ������ ����� ItemBag���� ��ü
#endif

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131				// �������ָӴ� ������ ����
CProbabilityItemBag*	NewYearLuckyBagItemBag;
#endif

#ifdef UPDATE_GM_FUNCTION_20070228							// GM ���� ���� ������ ����
CProbabilityItemBag*	GMPresentBoxItemBag;
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206						// ȥ�� ���� ������ ��
CProbabilityItemBag*	BoxOfGreenChaosItemBag;		// �ʷ�ȥ�� ����
CProbabilityItemBag*	BoxOfRedChaosItemBag;		// ����ȥ�� ����
CProbabilityItemBag*	BoxOfPurpleChaosItemBag;	// ����ȥ�� ����
#endif

#ifdef UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408	// ���� 
CProbabilityItemBag*	ItemGiveReoEventItemBag;	// ���� �ű� �����۹�
#else	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// ���� �� ������ ���� NPC - ���� �̺�Ʈ�� �߰�
CItemBagEx*		ItemGiveEventItemBag;
#endif
#endif	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ȯ�� ��� ���� ������ ��
CProbabilityItemBag* IllusionTempleGhostItemBagA;	// 1~2������ �����۹�
CProbabilityItemBag* IllusionTempleGhostItemBagB;	// 3~4������ �����۹�
CProbabilityItemBag* IllusionTempleGhostItemBagC;	// 5~6������ �����۹�
#endif

#ifdef CHUSEOK_MONSTER_EVENT_20070823	// �߼� ���� �̺�Ʈ ������ ��
CProbabilityItemBag*	ChuseokMonsterEventItemBag;
#endif

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// NPC �ٷ� ������ ��
CProbabilityItemBag*	NpcAlanEventItemBag;
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
CProbabilityItemBag*	pCherryBlossomEventItemBag;
CProbabilityItemBag*	pCherryBlossomEventItemBagA;
CProbabilityItemBag*	pCherryBlossomEventItemBagB;
CProbabilityItemBag*	pCherryBlossomEventItemBagC;
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_GIFTITEM_XMASEVENT_20081030
CProbabilityItemBag* pSantaNpcEventItem;    // ���� ���� �Ϸ翡 �ѹ��� ~
CProbabilityItemBag* pSantaNpcEventItem100; // 100��° �湮�ڿ� �����۹�
CProbabilityItemBag* pSantaNpcEventItem1000;  // 1000��° �湮�ڿ� �����۹� 
#endif //ADD_GIFTITEM_XMASEVENT_20081030

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� ��ȯ ������ ��
CProbabilityItemBag*	pLuckyCoinEventItemBag10 = NULL;
CProbabilityItemBag*	pLuckyCoinEventItemBag20 = NULL;
CProbabilityItemBag*	pLuckyCoinEventItemBag30 = NULL;
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610	// ���� �Ҳ� ���� �̺�Ʈ ������ ��
CProbabilityItemBag*	FireCrackerMonsterEventItemBag;
#endif // ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

CMsg			lMsg;	// ���� �޽���

NSerialCheck	gNSerialCheck[MAX_OBJECT]; 


CLoginCount	    gLCount[MAX_LOGIN_COUNT];

#ifdef HERO_SYSTEM_MODIFY
DWORD			gLevelExperience[MAX_LEVEL+1];					// ������ ����ġ
#else
DWORD			gLevelExperience[MAX_LEVEL+1];					// ������ ����ġ
#endif

//char			szClientVersion[3] = {'0','0','0'};				// ���Ӽ����� ���� ������ Ŭ���̾�Ʈ ����
char			szClientVersion[5] = {'0','0','0','0','0'};		// ���Ӽ����� ���� ������ Ŭ���̾�Ʈ ����

char			szGameServerExeSerial[21];						// ���Ӽ����� ���� ������ �ø��� ��ȣ
char			szServerName[51];								// ���� ������ �̸�

// �ѱ� �̸�
char	szKorItemTextFileName[256];					// ������ ���� �ؽ�Ʈ ���� �̸�
char	szKorSkillTextFileName[256];				// ��ų ���� �ؽ�Ʈ ���� �̸�

char			szItemTextFileName[256];							// ������ ���� �ؽ�Ʈ ���� �̸�
char			szSkillTextFileName[256];						// ��ų ���� �ؽ�Ʈ ���� �̸�

char			szQuestTextFileName[256];						// ��ų ���� �ؽ�Ʈ ���� �̸�

char			szMoveReqFileName[255];

char			szCommonlocIniFileName[256];					// ������ ini ���� ���� ���� �̸�
BOOL			SpeedHackPlayerBlock=0;							// ���ǵ��� ����ڸ� ����ų������
BOOL			bCanTrade =	TRUE;								// �ŷ��� �����ϰ� �Ұ���..
BOOL			bCanChaosBox = TRUE;							// ī���� �ڽ� ��� �����ϰ� �Ұ���..
BOOL			bCanConnectMember = FALSE;						// ���� ������ ����� ������ ������..

BOOL			bCanWarehouseLock = TRUE;						// â�� ��� ����� ����� ���ΰ� �����ΰ�?


#ifdef AUTH_GAMESERVER
char			szAuthKey[20];									// ���� ���� ������ ���� �˻�Ű (�ڵ� �������� üũ �Ѵ�)
int				gServerType = 0;								// 	0:MainServer, 1:TestServer, 2:Internal Server
int				gPartition = 0;								// 	0:MainServer, 1:TestServer, 2:Internal Server
#endif

#ifdef DARKLORD_WORK
BOOL			gIsDropDarkLordItem;
int				gSleeveOfLordDropRate;		// ������ �Ҹ� ��� Ȯ��
int				gSleeveOfLordDropLevel;		// ������ �Ҹ� ��� ����

int				gSoulOfDarkHorseDropRate;	// ��ũȣ���� ��ȥ ��� Ȯ��
int				gSoulOfDarkHorseropLevel;	// ��ũȣ���� ��ȥ ��� ����

int				gSoulOfDarkSpiritDropRate;	// ��ũ���Ǹ� ��� Ȯ��
int				gSoulOfDarkSpiritDropLevel;	// ��ũ���Ǹ� ��� ����

float			gDarkSpiritAddExperience;
#endif

#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
int				gIsDropGemOfDefend;			// ��ȣ���� �������
int				gGemOfDefendDropRate;		// ��ȣ���� ��� Ȯ�� (10000 ����)
int				gGemOfDefendDropLevel;		// ��ȣ���� ��� ���� ����
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314
int				gIsDropSetItemInCastleHuntZone;		// �÷��� ������ ��Ʈ������ �������
int				gSetItemInCastleHuntZoneDropRate;	// �÷��� ������ ��Ʈ������ ���Ȯ�� (10000 ����)
int				gSetItemInCastleHuntZoneDropLevel;	// �÷��� ������ ��Ʈ������ ��� ���� ����
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// �߱��߰�> . �Ƿε� �ý��� ���뿩��, ����ġ, ������Ȯ�� ����
BOOL			g_bChnPlayTimeLimitOn;				// �Ƿε� �ý��� ���뿩��
INT				g_iChnPlayTimeLimitLevel1Exp;		// �Ƿνð� ����ġ ȹ���
INT				g_iChnPlayTimeLimitLevel1ItemDrop;	// �Ƿνð� ������ ��ӷ�
INT				g_iChnPlayTimeLimitLevel2Exp;		// ���ؽð� ����ġ ȹ���
INT				g_iChnPlayTimeLimitLevel2ItemDrop;	// ���ؽð� ������ ��ӷ�
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// ��Ʈ�� �ɼ� ������
BOOL			g_bVtmPlayTimeLimitOn;				// �Ƿε� �ý��� ���뿩��
INT				g_iVtmPlayTimeLimitNoticeTime;		// �⺻ ���� �ð�

INT				g_iVtmPlayTimeLimitLevel1Time;		// �Ƿνð� üũ�ð� (min)
INT				g_iVtmPlayTimeLimitLevel1NoticeTime;// �Ƿνð� �����ð� (min)	
INT				g_iVtmPlayTimeLimitLevel1Exp;		// �Ƿνð� ����ġ ȹ���
INT				g_iVtmPlayTimeLimitLevel1ItemDrop;	// �Ƿνð� ������ ��ӷ�

INT				g_iVtmPlayTimeLimitLevel2Time;		// ���ؽð� üũ�ð� (min)
INT				g_iVtmPlayTimeLimitLevel2NoticeTime;// ���ؽð� �����ð� (min)
INT				g_iVtmPlayTimeLimitLevel2Exp;		// ���ؽð� ����ġ ȹ���
INT				g_iVtmPlayTimeLimitLevel2ItemDrop;	// ���ؽð� ������ ��ӷ�
#endif

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
int             g_iUseCharacterAutoRecuperationSystem;
int             g_iCharacterRecuperationMaxLevel;
#endif

#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706
int				g_iServerGroupGuildChatting;
int				g_iServerGroupUnionChatting;
#endif

#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
#ifdef FOR_TAIWAN
	int				g_iApplyPCBangAdvantage = 1;	// �⺻��: �븸 ���� ����
#else
	int				g_iApplyPCBangAdvantage = 0;	// �⺻��: �ѱ� ���� ���� ����
#endif
#endif //PCBANG_ADVANTAGE_EXPAND_20060919


#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
BOOL			g_bRibbonBoxEvent;
int				g_iRedRibbonBoxDropLevelMin;
int				g_iRedRibbonBoxDropLevelMax;
int				g_iRedRibbonBoxDropRate;
int				g_iRedRibbonBoxDropZenRate;
int				g_iRedRibbonBoxDropZen;

int				g_iGreenRibbonBoxDropLevelMin;
int				g_iGreenRibbonBoxDropLevelMax;
int				g_iGreenRibbonBoxDropRate;
int				g_iGreenRibbonBoxDropZenRate;
int				g_iGreenRibbonBoxDropZen;

int				g_iBlueRibbonBoxDropLevelMin;
int				g_iBlueRibbonBoxDropLevelMax;
int				g_iBlueRibbonBoxDropRate;
int				g_iBlueRibbonBoxDropZenRate;
int				g_iBlueRibbonBoxDropZen;
#endif //#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212

#ifdef VALENTINES_DAY_BOX_EVENT_20060124
BOOL			g_bChocolateBoxEvent;
int				g_iPinkChocolateBoxDropLevelMin;
int				g_iPinkChocolateBoxDropLevelMax;
int				g_iPinkChocolateBoxDropRate;
int				g_iPinkChocolateBoxDropZenRate;
int				g_iPinkChocolateBoxDropZen;

int				g_iRedChocolateBoxDropLevelMin;
int				g_iRedChocolateBoxDropLevelMax;
int				g_iRedChocolateBoxDropRate;
int				g_iRedChocolateBoxDropZenRate;
int				g_iRedChocolateBoxDropZen;

int				g_iBlueChocolateBoxDropLevelMin;
int				g_iBlueChocolateBoxDropLevelMax;
int				g_iBlueChocolateBoxDropRate;
int				g_iBlueChocolateBoxDropZenRate;
int				g_iBlueChocolateBoxDropZen;
#endif //#ifdef VALENTINES_DAY_BOX_EVENT_20060124

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
BOOL			g_bCandyBoxEvent;
int				g_iLightPurpleCandyBoxDropLevelMin;
int				g_iLightPurpleCandyBoxDropLevelMax;
int				g_iLightPurpleCandyBoxDropRate;
int				g_iLightPurpleCandyBoxDropZenRate;
int				g_iLightPurpleCandyBoxDropZen;

int				g_iVermilionCandyBoxDropLevelMin;
int				g_iVermilionCandyBoxDropLevelMax;
int				g_iVermilionCandyBoxDropRate;
int				g_iVermilionCandyBoxDropZenRate;
int				g_iVermilionCandyBoxDropZen;

int				g_iDeepBlueCandyBoxDropLevelMin;
int				g_iDeepBlueCandyBoxDropLevelMax;
int				g_iDeepBlueCandyBoxDropRate;
int				g_iDeepBlueCandyBoxDropZenRate;
int				g_iDeepBlueCandyBoxDropZen;
#endif //#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222

#ifdef ADD_ITEM_FENRIR_01_20051110
BOOL			g_bFenrirStuffItemDrop;

int				g_iFenrirStuff_01_DropLv_Min;
int				g_iFenrirStuff_01_DropLv_Max;
int				g_iFenrirStuff_01_DropMap;
int				g_iFenrirStuff_01_DropRate;

int				g_iFenrirStuff_02_DropLv_Min;
int				g_iFenrirStuff_02_DropLv_Max;
int				g_iFenrirStuff_02_DropMap;
int				g_iFenrirStuff_02_DropRate;

int				g_iFenrirStuff_03_DropLv_Min;
int				g_iFenrirStuff_03_DropLv_Max;
int				g_iFenrirStuff_03_DropMap;
int				g_iFenrirStuff_03_DropRate;

int				g_iFenrirRepairRate;

int				g_iFenrirDefaultMaxDurSmall;
int				g_iFenrirElfMaxDurSmall;

int				g_iFenrir_01Level_MixRate;
int				g_iFenrir_02Level_MixRate;
int				g_iFenrir_03Level_MixRate;

#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
BOOL			g_bCrywolfMonsterDarkElfItemDrop;
int				g_iCrywolfMonsterDarkElfItemDropRate;
int				g_iCrywolfMonsterDarkElfDropZenRate;
int				g_iCrywolfMonsterDarkElfDropZen;
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
BOOL			g_bCrywolfBossMonsterItemDrop;
int				g_iCrywolfBossMonsterItemDropRate;
int				g_iCrywolfBossMonsterDropZenRate;
int				g_iCrywolfBossMonsterDropZen;
#endif

#ifdef MU_CRYWOLF_BENEFIT_20051215
int				g_iCrywolfApplyMvpBenefit; 		
#endif 

#ifdef MU_CRYWOLF_PENALTY_20051215
int				g_iCrywolfApplyMvpPenalty; 	
#endif

#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413
int			g_iSkillDistanceCheck;		// ��ų ���� �Ÿ� üũ	0 : üũ ����, 1 : üũ ��
int			g_iSkillDistanceCheckTemp;	// ��ų ���� �Ÿ� üũ �� ���� �� : defalut 2�� ����
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
int			g_iSkillDistanceKick;		// ��ų �Ÿ� �� ����� ���� ���� ���� 0:�������� ����, 1:��������
int			g_iSkillDistanceKickCount;	// ��ų �Ÿ� �� ���� ���� ī��Ʈ.
#endif
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421
int			g_iSkillDiatanceKickCheckTime;	// ��ų �Ÿ� �� ����� ��ȿ ī��Ʈ �ð� 
#endif
#endif

#ifdef FOR_NEW_TESTSERVER
BOOL			gTestServer = 0;
#endif

#ifdef MAKE_ITEM_STOP
int				g_iMakeStop;
#endif

#ifdef KANTURU_MONSTER_ITEMDROP_20060807
BOOL		g_bKanturuMayaHandItemDrop;
int			g_iKanturuMayaHandItemDropRate;
int			g_iKanturuMayaHandDropZenRate;
int			g_iKanturuMayaHandDropZen;

BOOL		g_bKanturuNightmareItemDrop;
int			g_iKanturuNightmareItemDropRate;
int			g_iKanturuNightmareDropZenRate;
int			g_iKanturuNightmareDropZen;
#endif	// #ifdef KANTURU_MONSTER_ITEMDROP_20060807

#ifdef KANTURU_SPECIAL_ITEMDROP_20060808
BOOL		g_bKanturuSpecialItemDropOn;
int			g_iKanturuMoonStoneDropRate;
int			g_iKanturuJewelOfHarmonyDropRate;
#endif

#ifdef ADD_GAMBLING_20090120
BOOL		g_bGamblingSystem = TRUE;
int			g_iGamblingValue;
int			g_iGamblingShopTime;
#endif // ADD_GAMBLING_20090120

#ifdef ADD_RAKLION_20080408
BOOL		g_bRaklionEvent = TRUE;
BOOL		g_bRaklionSelupanItemDrop;
int			g_iRaklionSelupanItemDropRate;
int			g_iRaklionSelupanDropZenRate;
int			g_iRaklionSelupanDropZen;

BOOL		g_bBlockRaklionMapEnter;
#endif	// ADD_RAKLION_20080408

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
BOOL		g_bHallowinDayEventOn;
int			g_iHallowinDayEventItemDropRate;
int			g_iHallowinDayEventJOLBlessDropRate;
int			g_iHallowinDayEventJOLAngerDropRaTe;
int			g_iHallowinDayEventJOLScreamDropRate;
int			g_iHallowinDayEventJOLFoodDropRate;
int			g_iHallowinDayEventJOLDrinkDropRate;
int			g_iHallowinDayEventJOLPolymorphRingDropRate;
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
BOOL		g_bSantaPolymorphRingDropOn;
int			g_iSantaPolymorphRingDropRate;
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131		//�̺�Ʈ ��/����
BOOL		g_bNewYearLuckyBagMonsterEventOn;
#endif

#ifdef ADD_THIRD_WING_20070525	// �ܵ����ǺҲ� ���Ȯ�� 
int			g_iCondorFlameDropRate;
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
BOOL		g_bCherryBlossomEventOn;
int			g_bCherryBlossomEventItemDropRate;
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef MONSTER_HERD_SYSTEM_20031120
#include "RingAttackEvent.h"

extern CRingMonsterHerd		g_MonsterHerdTest;
extern CRingAttackEvent		g_RingAttackEvent;
#endif // MONSTER_HERD_SYSTEM_20031120

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
#include "XMasAttackEvent.h"
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#ifdef CASTLEDEEP_EVENT_20050413
#include "CastleDeepEvent.h"
#endif

#ifdef EVENT_MANAGEMENT_20040711
#include "EventManagement.h"
#endif


//#ifdef CHAOSCASTLE_SYSTEM_20040408

/*int				MapMinUserLevel[MAX_MAP]={						// �ʿ� �����Ҽ� �ִ� �ּ� ���� ����
						0,			//	�η��þ�
						20,			//	����
						15,			//	����ƽ�
						10,			//	�븮��
						80,			//	�ν�ƮŸ��1
						0,			//	�ν�ƮŸ��2
						0,			//	�ν�ƮŸ��3
						60,			//	�ν�ƮŸ��4
						130,		//	�ν�ƮŸ��5
						0,			//	����������
						160,		//	õ��
						10,			//	����ĳ��1		(���˻� ����)
						36,			//	����ĳ��2		(���˻� ����)
						80,			//	����ĳ��3		(���˻� ����)
						130,		//	����ĳ��4		(���˻� ����)
						170,		//	����ĳ��5		(���˻� ����)
						210,		//	����ĳ��6		(���˻� ����)
						310,		//	����ĳ��7		(���˻� ����)
						15,			//	ī���� ĳ��1	(���˻� ����)
						30,			//	ī���� ĳ��2	(���˻� ����)
						100,		//	ī���� ĳ��3	(���˻� ����)
						160,		//	ī���� ĳ��4	(���˻� ����)
						220,		//	ī���� ĳ��5	(���˻� ����)
						280,		//	ī���� ĳ��6	(���˻� ����)
#ifdef ADD_NEW_MAP_KALIMA_20040518
						0,			//	Į���� 1
						0,			//	Į���� 2 
						0,			//	Į���� 3
						0,			//	Į���� 4
						0,			//	Į���� 5
						0,			//	Į���� 6
#endif
#ifdef ADD_NEW_MAP_CASTLE_20041115
						10,			//	�� �� (���� �ּҷ��� ����)
#endif
#ifdef ADD_NEW_MAP_CASTLE_HUNTZONE_20041115
						10,
#endif
#ifdef DEVILSQUARE_EXTEND_20050221	// ���������� ���߰�
						0,
#endif
#ifdef ADD_NEW_MAP_AIDA_20050617
						0,			// ���̴� �� �߰�
#endif
#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
						0,			// ũ���̿��� 1�� �� �߰�
#endif
#ifdef ADD_NEW_MAP_CRYWOLF_SECONDZONE_20050414
						0,			// ũ���̿��� 2�� �� �߰�
#endif
#ifdef HIDDEN_KALIMA_20050706
						0,			// ���� Į���� �߰�
#endif
#ifdef ADD_NEW_MAP_KANTURU_COMMON_20060523
						150,			// ĭ���� 1�� �� �߰�
						220,			// ĭ���� 2�� �� �߰�
#endif
#ifdef ADD_NEW_MAP_KANTURU_BOSS_20060627
						0,				// ĭ���� ������ �߰�						
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� ���� �� �߰�
						0,				// GM ��ȯ ����
						380,			// 3�� ���� ����Ʈ ��
						380,			// 3�� ���� ����Ʈ ���� ��
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// �������� �� �߰�
						0,				// ������
						0,				// ������

						220,			// �� 1 ȯ�� ���
						271,			// �� 2 ȯ�� ���
						321,			// �� 4 ȯ�� ���
						351,			// �� 4 ȯ�� ���
						380,			// �� 5 ȯ�� ���
						400,			// �� 6 ȯ�� ���

						10,				// ��������
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ��8 ���� ���� ���� �߰�
						400,
#endif		
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ��7 ���� ���� ���� �߰�
						400,
#endif		
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108		// ����Ǵ� ���� ���ɷ��� �߰�
						400,
#endif							
#ifdef ADD_RAKLION_20080408							// ��Ŭ���� ���� ���� ���� �߰�
						270,
#endif // ADD_RAKLION_20080408
						};*/
//#endif	// CHAOSCASTLE_SYSTEM_20040408


int				gApplyHeroSystem = FALSE;
int				gSpeedHackPenalty = 0;

int				gEnableBattleSoccer = 1;
int				gEnableEventNPCTalk = 0;

int				gEnableServerDivision = 0;

DWORD dwgCheckSum[1024];
char connectserverip[20];
int  connectserverport;

#ifdef GAMESERVER_INFO_TCP_20031203
int  connectserverport_tcp;
int	g_ConServerInfoSendType=0;		// ���Ӽ����� ������ ���� Ÿ���� ���� (0:UDP / 1:TCP)
#endif


short gGameServerCode;
int	 gPkTime;
int  gEvent1=0;
int  gMonsterHp=0;
int  gMerryXMasNpcEvent=0;
int  gHappyNewYearNpcEvent=0;
int	 gLootingTime=3;
int  gPkItemDrop=1;
int  gItemDropPer=10;
int	 gEvent1ItemDropTodayCount=0;
int	 gEvent1ItemDropTodayMax=1;
int  gEvent1ItemDropTodayPercent=10000000;

int  gLanguage=0;
char gCountryName[20] = "Kor";

int  gCharacterDeleteMinLevel=40;
BOOL gCreateCharacter=1;
BOOL gGuildCreate=1;
BOOL gGuildDestroy=1;
int	 gGuildCreateLevel=100;
#ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
BOOL g_bCastleGuildDestoyLimit;
#endif

DWORD gItemNumberCount;
BOOL gItemSerialCheck=1;
float gAddExperience=1;
BOOL gChaosEvent=0;
char gChaosEventServerIp[20] = "";
char gDevilSquareEventServerIp[20] = "";
char gHackLogServerIp[20] = "";
char gEventChipServerIp[20] = "";

#ifdef ADD_ZEN_OPTION_20070903
float gAddZen = 1;
#endif

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
char gGGModuleName[20] = "";
#endif

#ifdef MUTE_EVENT_MESSAGE
bool g_bMuteServerMessage	= 0;
#endif

char gStalkProtocolId[MAX_IDSTRING+1]="";
BOOL gStalkProtocol;
DWORD  gAttackEventRegenTime;
BOOL gNonPK=0;
int  gYear;

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
BOOL	gPkLimitFree	= 0;
#endif

#ifdef ADD_NEWSERVER_20081113
BOOL	g_bNewServer	= 0;
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
BOOL	g_bEventOff		= 0;
#endif // ADD_NEWSERVER_EVENTOFF_20081114

// ��ź�� �� �̺�Ʈ
BOOL gXMasEvent = 0;		// ��ź�� �� �̺�Ʈ �����ϴ°�?
int	 g_XMasEvent_StarOfXMasDropRate = 5000;		// ��ź�� ���� ������ Ȯ��
int	 g_XMasEvent_ItemDropRateForStarOfXMas = 2; // ��ź�� ������ �������� ���� Ȯ��


// ���� �̺�Ʈ
BOOL gFireCrackerEvent = 0;	// ���� �̺�Ʈ �����ϴ°�?
int	 gFireCrackerDropRate = 5000;	// ������ ������ Ȯ��
int	 g_ItemDropRateForgFireCracker = 2;	// ���׿��� �������� ���� Ȯ��

int  gOnlyFireCrackerEffectUse;	// ������ ����߷�����, ȿ���� ������ �Ѵ�


// ����� ��Ʈ �̺�Ʈ
BOOL gHeartOfLoveEvent = 0;	// ����� ��Ʈ �̺�Ʈ �����ϴ°�?
int	 gHeartOfLoveDropRate = 5000;	// ����� ��Ʈ�� ������ Ȯ��
int	 g_ItemDropRateForgHeartOfLove = 2;	// ����� ��Ʈ���� �������� ���� Ȯ��


// �޴� �̺�Ʈ
BOOL gMedalEvent = 0;	// �޴� �̺�Ʈ �����ϴ°�?
int	 gGoldMedalDropRate = 2;	// �ݸ޴��� ������ Ȯ��
int	 gSilverMedalDropRate = 2;	// ���޴��� ������ Ȯ��

int	 g_ItemDropRateForGoldMedal = 2;	// �ݸ޴޿��� �������� ���� Ȯ��
int	 g_ItemDropRateForSilverMedal = 2;	// ���޴޿��� �������� ���� Ȯ��

// �̺�ƮĨ �̺�Ʈ
BOOL gEventChipEvent = 0;					// �޴� �̺�Ʈ �����ϴ°�?
int	 gBoxOfGoldDropRate = 2;				// �ݸ޴��� ������ Ȯ��
int	 g_ItemDropRateForBoxOfGold = 2;		// �ݸ޴޿��� �������� ���� Ȯ��
int	 g_EventChipDropRateForBoxOfGold = 50;	// �ݸ޴޿��� �̺�Ʈ Ĩ�� ���� Ȯ��

#ifdef ADD_GIFTITEM_XMASEVENT_20081030
int	 gXmasVisitorCount = 0;           // ũ�������� �̺�Ʈ �湮 ī��Ʈ 
short gXmasGiftForTest = 0;  // 0 - normal , 1 - ������ ù�湮 , 2 - ������ ù�湮 ���� �湮 
#endif //ADD_GIFTITEM_XMASEVENT_20081030

// �Ǹ��� ���� �̺�Ʈ
BOOL gDevilSquareEvent = 0;
// �Ǹ��� �� ���� Ȯ��
int	 gEyesOfDevilSquareDropRate = 2;
// �Ǹ��� ���� ���� Ȯ��
int	 gKeyOfDevilSquareDropRate = 3;

#ifdef DEVILSQUARE_EXTEND_20050221		// ī���� ���� �߰�
int	 gDQChaosSuccessRateLevel1 = 75;
int	 gDQChaosSuccessRateLevel2 = 70;
int	 gDQChaosSuccessRateLevel3 = 65;
int	 gDQChaosSuccessRateLevel4 = 60;
int	 gDQChaosSuccessRateLevel5 = 55;
int	 gDQChaosSuccessRateLevel6 = 50;
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// ����������7 ����� ī�������� Ȯ��
int	 gDQChaosSuccessRateLevel7 = 45;	
#endif
#else
int	 gDQChaosSuccessRateLevel1 = 40;
int	 gDQChaosSuccessRateLevel2 = 40;
int	 gDQChaosSuccessRateLevel3 = 30;
int	 gDQChaosSuccessRateLevel4 = 30;
#endif

BOOL gWriteSkillLog = 0;

// ���� ĳ��
#ifdef FOR_BLOODCASTLE
BOOL			g_bBloodCastle = TRUE;					// ���� ���Ͽ��� �о�� ��
int				g_iBloodCastle_StartHour = 1;			// 1:1�ð����� (�⺻) / 2:2�ð�����

BOOL			g_bStoneItemDrop = 0;
int				g_iStoneDropRate = 50;
int				g_iAngelKingsPaperDropRate = 10;
int				g_iBloodBoneDropRate = 20;

#ifdef UPDATE_BLOODECASTLE_SCADULE_20070702
int				g_iBloodCastle_odd_Even_Hour	= 0;	// ¦�� �Ǵ� Ȧ�� �ð��� ���� (0:¦�� �ð���, 1: Ȧ�� �ð���)
#endif

#endif

// ī���� ĳ��
#ifdef CHAOSCASTLE_SYSTEM_20040408
BOOL			g_bChaosCastle = TRUE;					// ���� ���Ͽ��� �о�� ��
#endif

#ifdef MONSTER_HERD_SYSTEM_20031120
BOOL			g_bDoRingEvent = 0;						// ���� �̺�Ʈ�� �� ���ΰ� ?
INT				g_iRingOrcKillGiftRate;					// ��ũ�� ����� �� ��ǰ�� Ż Ȯ�� (1/n)
INT				g_iRingDropGiftRate;					// ������ ������ �� ��ǰ�� Ż Ȯ�� (1/n)
#endif

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
BOOL			g_bDoXMasEvent = 0;						// ũ��������(��Ÿ�� ģ����) �̺�Ʈ ��/��
int				g_nXMasDropRate;
int				g_nXMasDropZen;
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#ifdef ADD_XMASEVENT_LUCKNUMBER
#define XMASEVENT_LUCKNUMBER1ST			100
#define	XMASEVENT_LUCKNUMBER2ND			200

INT				g_nXMasLuckNumber1st = XMASEVENT_LUCKNUMBER1ST;
INT				g_nXMasLuckNumber2nd = XMASEVENT_LUCKNUMBER2ND;
#endif // ADD_XMASEVENT_LUCKNUMBER

#ifdef CASTLEDEEP_EVENT_20050413
BOOL			g_bDoCastleDeepEvent	= 0;			// �η����� ���� �̺�Ʈ�� �� ���ΰ�?
#endif

#ifdef EVENT_MANAGEMENT_20040711
BOOL			g_bEventManagerOn = 0;
#endif

// Į���� ��
#ifdef ADD_NEW_MAP_KALIMA_20040518
INT				g_iKundunMarkDropRate		= 0;		// ����� ǥ�� ���Ȯ��
#endif

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214					// ������ ǥ�� ���Ȯ��
INT				g_iMarkOfTheLord			= 0;
#endif

#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531
int				g_iJapan1StAnivItemDropRate = 0;		// �Ϻ� 1�ֳ� ��� ������ ���Ȯ��
#endif

#ifdef DARK_LORD_HEART_EVENT_20040810					// ��ũ�ε��� ���� ���� Ȯ��������
INT				g_iDarkLordHeartDropRate = 0;			// ��ũ�ε��� ���� ���Ȯ��
INT				g_iDarkLordHeartOffEventRate = 0;		// ��ũ�ε��� ���� �������� ��ǰ��÷ Ȯ��
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
INT				g_iMysteriousBeadDropRate1 = 0;			// �ź��� ���� ���Ȯ�� (���̴�, ũ���̿��� ������)
INT				g_iMysteriousBeadDropRate2 = 0;			// �ź��� ���� ���Ȯ�� (���̴�, ũ���̿��� ����)
INT				g_iHiddenTreasureBoxOfflineRate = 0;	// ������ �������� �̺�Ʈ �������� ��ǰȮ��
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029							// ����� ���� ��� Ȯ��
int				g_bLuckyCoinEventOn		= 0;	// ����� ���� �̺�Ʈ ����
int				g_iLuckyCoinDropRate	= 0;	// ����� ���� ��� Ȯ��
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

// ���ݼӵ� üũ ���� �ð�
DWORD		gAttackSpeedTimeLimit = 500;
// ���ǵ� �� üũ�� �Ÿ� ��Ŷ�� ���� �Ѵ�
int			bIsIgnorePacketSpeedHackDetect = FALSE;
DWORD		gHackCheckCount = 5;
float		gDecTimePerAttackSpeed = 5.33f;
int			gMinimumAttackSpeedTime = 200;			// �ּ� ���� �ð�
int			gDetectedHackKickCount = 10;			// ���ǵ��� ������ �����ų ī��Ʈ
int			gIsKickDetecHackCountLimit = 0;			// ���ǵ��� ������ �����Ű�°�?

#ifdef FOR_JAPAN
BOOL			gTamaJJangEvent	= FALSE;					// Ÿ��¯ �̺�Ʈ�� �����ϴ°�?
BOOL			gAppearTamaJJang = FALSE;					// Ÿ��¯�� �����ϴ°�?
int				gTamaJJangKeepTime = 100;					// Ÿ��¯ ���ӽð�


int				gTamaJJangDisappearTime = 1800;				// Ÿ��¯ ������� �ð�
int				gTamaJJangDisappearTimeRandomRange = 1000;	// Ÿ��¯�� ������� �ð��� �������� �����ϱ� ���� ����

int				gTamaJJangTime = 0;
#endif

#ifdef CHECK_ATTACK_AREA
int				gNotAttackAreaKickCount;
int				gIsKickNotAttackAreaCountLimit;
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
int				gBlockHackUserCharacter;					// ������ ĳ�� �� ����
int				gBlockHackUserAccount;						// ������ ���� �� ����
#endif

#ifdef ITEMDROP_RINGOFTRANSFORM	
int				gIsItemDropRingOfTransform = 0;				// ���� ������ ����߸��°�?
int				gItemDropRingOfTransform = 1;				// ���� ���� ��� Ȯ�� gItemDropRingOfTransform/10000
#endif


int				gQuestNPCTeleportTime = 60;

CSimpleModulus g_SimpleModulusCS;
CSimpleModulus g_SimpleModulusSC;

extern int		gCurPaintPlayer;
extern int		gObjTotalUser;
extern int		gObjMonCount;

// �������� ���� �̺�Ʈ
BOOL			gIsEledoradoEvent = 0;

DWORD			gEledoradoGoldGoblenRegenTime = 60;
DWORD			gEledoradoTitanRegenTime = 60;
DWORD			gEledoradoGoldDerconRegenTime = 240;
DWORD			gEledoradoDevilLizardKingRegenTime = 120;
DWORD			gEledoradoDevilTantarosRegenTime = 120;

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
DWORD			gGoldenRabbitRegenTime = 60;
DWORD			gGoldenDarknightRegenTime = 60;
DWORD			gGoldenDevilRegenTime = 60;
DWORD			gGoldenStoneMonsterRegenTIme = 60;
DWORD			gGoldenCrustEventRegenTime = 60;
DWORD			gGoldenSatirosRegenTime = 60;
DWORD			gGoldenTwintailRegenTime = 60;
DWORD			gGoldenIronKnightRegenTime = 60;
DWORD			gGoldenNeipinRegenTime = 60;
DWORD			gGoldenGreatDragonRegenTime = 60;
#endif

int				gEledoradoGoldGoblenItemDropRate;		// ������ ��� Ȯ��
int				gEledoradoTitanItemDropRate;
int				gEledoradoGoldDerconItemDropRate;
int				gEledoradoDevilLizardKingItemDropRate;
int				gEledoradoDevilTantarosItemDropRate;

int				gEledoradoGoldGoblenExItemDropRate;		// ������Ʈ ������ ���Ȯ��
int				gEledoradoTitanExItemDropRate;
int				gEledoradoGoldDerconExItemDropRate;
int				gEledoradoDevilLizardKingExItemDropRate;
int				gEledoradoDevilTantarosExItemDropRate;

int  gConnectBonus;
int  gConnectBonusTime;
int  gConnectBonusCredits;
int  gConnectBonusCashPoints;
int  gConnectBonusZen;

#ifdef HAPPY_POUCH
INT				gHappyPouchRedDropRate = 1000;
INT				gHappyPouchBlueDropRate = 1000;
#endif

#ifdef PERSONAL_SHOP_20040113	// ���λ��� ���� ����
int				gDoPShopOpen = 0;
#endif

#ifdef MU_CHN_OFFLINE_GIFT_EVENT_20040908
BOOL			gbChnOfflineGiftEvent = FALSE;			// �߱� �������� ��ǰ �̺�Ʈ�� �ϴ°� ?
int				giChnOfflineGiftEventDropRate = 0;		// �߱� �������� ��ǰ ��� Ȯ�� (1/1000000)
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
int				giKundunRefillHPSec;							// ��� �� �ʴ�ȸ���� 
int				giKundunRefillHP;								// ��� �� ȸ���� 
int				giKundunRefillHPTime;							// �� ȸ�� �ð� (SEC)
#endif

#ifdef EXTEND_LOG_SYSTEM_20060202
int				giKundunHPLogSaveTime;
#endif

// �� ��� �ð�
int				gZenDurationTime = 30;

BOOL			gWriteChatLog = FALSE;
BOOL			gDisconnectHackUser = FALSE;

BOOL			gUseNPGGChecksum;

#ifdef MODIFY_JAPAN_MONSTER_VALUE
int				gMonsterHPAdjust = 100;
#endif

#ifdef CHECK_PENETRATION_SKILL
int				gEnableCheckPenetrationSkill = 1;
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
// CODEREVIEW_SHIELD : Commonserver.cfg �׽�Ʈ
BOOL			g_ShieldSystemOn;
int				g_iDamageDevideToSDRate;
int				g_iDamageDevideToHPRate;
float			g_fSuccessAttackRateOption;
int				g_iSDChargingOption;
int				g_iConstNumberOfShieldPoint;

BOOL			g_ShieldAutoRefillOn;
BOOL			g_ShieldAutoRefillOnSafeZone;
BOOL			g_PKLevelIncreaseOff;

// CODEREVIEW_SHIELD3 : ���� ���� ��� Ȯ�� �߰�
BOOL			g_CompoundPotionDropOn;				// ���� ���� ��� On(1)/Off(0)
int				g_iCompoundPotionLv1DropRate;		// ���� ���� ���� ��� Ȯ�� / 10000
int				g_iCompoundPotionLv2DropRate;		// �߰� ���� ���� ��� Ȯ�� / 10000
int				g_iCompoundPotionLv3DropRate;		// ū ���� ���� ��� Ȯ�� / 10000
int				g_iCompoundPotionLv1DropLevel;		// ���� ���� ���� ��� ����
int				g_iCompoundPotionLv2DropLevel;		// �߰� ���� ���� ��� ����
int				g_iCompoundPotionLv3DropLevel;		// ū ���� ���� ��� ����

	#ifdef UPDATE_SHIELD_OPTION_20060427
		BOOL	g_bShieldComboMissOptionOn;			// PvP ���� �̽� �� �޺� ��� �ɼ� 1 : �޺� ���, 0 : �޺� ����
		int		g_iShieldPotionLv1MixSuccessRate;	// ���� SD ���� ���� Ȯ��
		int		g_iShieldPotionLv1MixMoney;			// ���� SD ���� ���� �ʿ� ��
		int		g_iShieldPotionLv2MixSuccessRate;	// �߰� SD ���� ���� Ȯ��
		int		g_iShieldPotionLv2MixMoney;			// �߰� SD ���� ���� �ʿ� ��
		int		g_iShieldPotionLv3MixSuccessRate;	// ū SD ���� ���� Ȯ��
		int		g_iShieldPotionLv3MixMoney;			// ū SD ���� ���� �ʿ� ��
	#endif
		
	#ifdef UPDATE_SHIELD_CALCULATE_GAGE_20060428
		int		g_iShieldGageConstA;				// ���� ������ ���� ����� 1
		int		g_iShieldGageConstB;				// ���� ������ ���� ����� 2
	#endif // UPDATE_SHIELD_CALCULATE_GAGE_20060428

#endif
		
#ifdef MODIFY_KANTURU_MAPNUMBER_CHECK_20060807
int			g_iBlockKanturuMapEnter					= 0;
int			g_iBlockCastleSiegeMapEnter				= 0;
#endif // MODIFY_KANTURU_MAPNUMBER_CHECK_20060807

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#include "JewelOfHarmonySystem.h"
#endif

#ifdef ADD_380ITEM_NEWOPTION_20060711
	#include "ItemSystemFor380.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	BOOL	g_bIllusionTampleEvent				= TRUE;		// �̺�Ʈ ���� ����
	int		g_iOldScrollDropRate				= 0;		// ���� �η縶�� ���� Ȯ��
	int		g_iCovenantOfIllusionDropRate		= 0;		// ȯ�������� ���� ���� Ȯ��
	BOOL	g_bUseMaxLevelIllusionTemple		= FALSE;	// 400 ���� ���� ȯ�� ��� ��� ����
	BOOL	g_bSaveIllusionTempleRankingPoint	= FALSE;	// ȯ�� ����� ��ŷ ���� ���忩��
	
#ifdef UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710	// ȯ����� ��ũ��Ʈ�߰�
	int		g_iIllusionTempleRewardPercent		= 100;		// ȯ����� ���� ����ġ ����
	int		g_btIllusionTempleMinUseSkillCnt	= 3;		// ȯ����� �ּһ�뽺ų 
#endif	// UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710
#endif

#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	int		g_iIllusionTempleRewardPercentMaster	= 100;	// ȯ����� ���� ����ġ ����(�����ͷ���)
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	
#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
int		gML_OldScrollDropRate				= 10;		// 6 ���� �η縶�� ���� Ȯ��
int		gML_CovenantOfIllusionDropRate		= 10;		// 6 ȯ�������� ���� ���� Ȯ��
int		gML_AngelKingsPaperDropRate			= 10;		// 8 ��õ���� �� ���� Ȯ��xbr
int		gML_BloodBoneDropRate				= 10;		// 8 ���庻 ���� Ȯ��
int		gML_EyesOfDevilSquareDropRate		= 10;		// 7 �Ǹ��� �� ���� Ȯ��
int		gML_KeyOfDevilSquareDropRate		= 10;		// 7 �Ǹ��� ���� ���� Ȯ��
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
	
#ifdef UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220
int		g_ShadowPahtomMaxLevel				= 220;		// ���������� ���� �ִ� ����
#endif	// UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220

int		g_bCheckSpeedHack					= 1;

#ifdef MODIFY_SPEEDHACK_20080605
int		g_bAbilityDebug						= FALSE;
#endif // MODIFY_SPEEDHACK_20080605

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410
int		g_bChangeCharacterName				= 0;		// ĳ���� ���� ����
#endif

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
BOOL	g_bUseMapMoveCheckSum				= 0;		// ���� �� �̵��� äũ�� ��� ����
#endif // ADD_MAPMOVE_PROTOCOL_20090327


void GameMonsterAllAdd();
void ReadCommonServerInfo();
void ReadServerInfo();
void CheckSumFileLoad(char *name);
void LoadItemBag();
void SetMapName();

#ifdef EVENT_OPTION_RELOAD_20040305				// ���� �̺�Ʈ �ɼǸ� �ٽ� �б� ����
void ReadEventInfo(MU_EVENT_TYPE eEventType);
#endif

#ifdef ETC_OPTION_RELOAD_20040308				// ���� �ɼ� �� �̺�Ʈ ���� �͵鸸 ���ε�
void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType);
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
int gConnectHackIPBlockTime		= 0;// ������ �� �ð� 
int gConnectIntervalTime		= 0;// ���� �ð� ����
int gConnectRenewBlockTime		= 0;// ���� ���� �ϸ� �� �ð��� ���� �Ѵ�.
#endif


void gSetDate()
{
	struct tm *today;
	time_t ltime;
	
	time( &ltime );
	today = localtime( &ltime );
	
	today->tm_year += 1900;
	gYear = today->tm_year;
}

BOOL gJoomin15Check(char * szJN)
{
	int  tyear=1900;
	char szTyear[3]="";

	if( szJN[6] == '3' || szJN[6] == '4' )
		tyear = 2000;
	
	memcpy(szTyear, szJN, 2);
	tyear += atoi(szTyear);
	
	if( (gYear-15) < tyear )
	{
		return FALSE;
	}
	return TRUE;
}


#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324

BOOL gJoominCheck(char * szJN, int iLimitAge)
{
	if (iLimitAge < 0)
		return TRUE;

	int  tyear=1900;
	char szTyear[3]="";

	if( szJN[6] == '3' || szJN[6] == '4' )
		tyear = 2000;
	
	memcpy(szTyear, szJN, 2);
	tyear += atoi(szTyear);
	
	if( (gYear-iLimitAge) < tyear )
	{
		return FALSE;
	}
	return TRUE;
}

#endif


//----------------------------------------------------------------------------
// ���Ӽ����� ���Ǵ� ���� ����ü �� �����͸� �ʱ�ȭ �Ѵ�.
void GameMainInit(HWND hWnd)
{	
	int n;

	srand( (unsigned)time( NULL ) );

	ReadServerInfo();
	LogInit(LOG_PRINT);		// Log ��Ͽ� �Լ� �ʱ�ȭ
	gSetDate();				// ���� ��¥ �б�

	gDirPath.SetFirstPath("..\\Data\\");

#ifdef AUTH_GAMESERVER
	gServerType = GetPrivateProfileInt("GameServerInfo", "ServerType", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gPartition = GetPrivateProfileInt("GameServerInfo", "Partition", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gLanguage = GetPrivateProfileInt("GameServerInfo", "Language", 0, gDirPath.GetNewPath("commonserver.cfg"));	

#ifdef FOR_CHINA
	int iWzFsGateIndex = GetPrivateProfileInt("GameServerInfo", "FsGateIndex", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gServerType |= (iWzFsGateIndex<<16);
#endif
#endif

#ifdef FOR_NEW_TESTSERVER
	// �׼��� ��� T�� ���δ�.
	int nLen = strlen(GAMESEVER_VERSION);
	GAMESEVER_VERSION[nLen] = 'T';
#endif // FOR_NEW_TESTSERVER
	
#ifdef FOR_NEW_LIVESEVER
	// �ż��� ��� N�� ���δ�.
	int nLen = strlen(GAMESEVER_VERSION);
	GAMESEVER_VERSION[nLen] = 'N';
#endif	// FOR_NEW_LIVESEVER

#ifdef AUTH_GAMESERVER					// ���� ���� �ʱ�ȭ
	gGameServerAuth.Init();
	gGameServerAuth.SetInfo(gLanguage, gPartition, 0, GAMESEVER_VERSION, szServerName, gServerType, GameServerAuthCallBackFunc);		
	gGameServerAuth.GetKey(szAuthKey, 0, 5);
#endif
	
#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#1 ���� ���� �ʱ�ȭ ����");
#endif

#ifdef EVENT_MANAGEMENT_20040711
	DragonEvent = new CDragonEvent;
	if( DragonEvent == NULL )
	{
		MsgBox("CDragonEvent %s", lMsg.Get(110));
		return;
	}

	AttackEvent = new CAttackEvent;
	if( AttackEvent == NULL )
	{
		MsgBox("AttackEvent %s", lMsg.Get(110));
		return;
	}
#endif

	gObjInit();

	if(g_TerrainManager.Size() < 1)
		g_TerrainManager.Load();

	MapC = new MapClass[g_TerrainManager.Size()];

	// ���� ������ �̸����� �ʼӼ����� �ʱ�ȭ
	for( n=0; n<g_TerrainManager.Size(); n++)
	{
		MapC[n].ItemInit();

		if(g_TerrainManager.CheckTerrain(n) == true)
			MapC[n].LoadMapAttr(gDirPath.GetNewPath(g_TerrainManager.GetTerrainName(n)), n);
	}
		
	ReadCommonServerInfo();

#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#2 ReadCommonServerInfo ����");
#endif

#ifdef MAP_SERVER_WORK_20041030			// �ʼ����� ������ ����
	g_MapServerManager.LoadData(gDirPath.GetNewPath("MapServerInfo.dat"));
#endif
	
#ifdef CASTLE_MAIN_SCHEDULER_20041111	// �������� �� ���������� ����
#ifdef MAP_SERVER_WORK_20041030
	if (g_CastleSiege.Ready(g_MapServerManager.GetMapSvrGroup()) == TRUE) {
		if (g_CastleSiege.LoadData(gDirPath.GetNewPath("MuCastleData.dat"))) {
			g_CastleSiege.LoadPreFixData(gDirPath.GetNewPath("commonserver.cfg"));
			g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);		// DB �����͸� ���� �غ� �Ѵ�.
		}
	}
#endif
#endif

#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#3 �������� ���� �б� ����");
#endif

#ifdef AUTH_GAMESERVER	
	gGameServerAuth.GetKey(szAuthKey, 10, 5);
#endif

	if( gEnableServerDivision )
	{
		MessageBox(NULL, "���������� ������ �����Դϴ�.", "Warning", MB_OK);
	}

#ifdef MODIFY_MIX_SYSTEM_20070518
	if( gUdpSoc.CreateSocket() == FALSE ){ MsgBox("UDP Socket create error"); return; }
	if( gUdpSocCER.CreateSocket() == FALSE ){ MsgBox("UDP Socket create error"); return; }
	gUdpSocCER.RecvSet(g_GlobalConfig.m_iUDPPort);
	gUdpSocCER.Run();
	
	if( gUdpSocCE.CreateSocket() == FALSE ){ MsgBox("UDP Socket create error"); return; }

	gUdpSocCER.SetProtocolCore(ChaosEventProtocolCore);
	gUdpSocCE.SetProtocolCore(ChaosEventProtocolCore);
#endif

	// Ranking Server
	//if( gUdpSocCRank.CreateSocket() == FALSE ){ MsgBox("UDP Socket create error"); return; }
	//if( gUdpSocCRankR.CreateSocket() == FALSE ){ MsgBox("UDP Socket create error"); return; }

	//gUdpSocCRankR.RecvSet(44455);
	//gUdpSocCRankR.Run();
	//gUdpSocCRankR.SetProtocolCore(DevilSquareEventProtocolCore);
	
	if( !SPEEDHACK_CHECK ) MsgBox(lMsg.Get(210));

	#if (LOCALCONNECT == 1 )  
		MsgBox(lMsg.Get(418));
	#endif
	if( DATANOSAVE == 1 )
	{
		MsgBox("����!! ĳ���� �����͸� �������� �ʽ��ϴ�.");
	}
#ifdef FOR_ONAIR
	MsgBox("����!! ��ۿ� ���� �Դϴ�");
#endif

	InitBattleSoccer();

#ifdef AUTH_GAMESERVER
	int DataBufferSize;
	char* DataBuffer;

	gGameServerAuth.RequestData(FILE_MONSTER);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMAttr.LoadAttr(DataBuffer, DataBufferSize);						// ���� �Ӽ����� �ε�	

	gGameServerAuth.RequestData(FILE_MONSTERSETBASE);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMSetBase.LoadSetBase(DataBuffer, DataBufferSize);	// ���� ��ġ���� �ε�
#else
	#ifdef SCRIPT_DECODE_WORK
		gMSetBase.LoadSetBaseDecode(gDirPath.GetNewPath("MonsterSetBase.cwz"));	// ���� ��ġ���� �ε�
		gMAttr.LoadAttrDecode(gDirPath.GetNewPath("Monster.cwz"));				// ���� �Ӽ����� �ε�
		gGateC.LoadDecode(gDirPath.GetNewPath("gate.cwz"));						// ����Ʈ ���� �ε�
	#else	
		gMSetBase.LoadSetBase(gDirPath.GetNewPath("MonsterSetBase.txt"));	// ���� ��ġ���� �ε�
		gMAttr.LoadAttr(gDirPath.GetNewPath("Monster.txt"));				// ���� �Ӽ����� �ε�
		gGateC.Load(gDirPath.GetNewPath("gate.txt"));						// ����Ʈ ���� �ε�
	#endif
#endif


	// ���Ϳ��� �� ������ ����Ʈ �ۼ�
	g_MonsterItemMng.Init();	

#ifdef HAPPY_POUCH //happycat@20050201
	g_GameEvent.Init();
#endif
	
	// ���� �� ����ġ ���
	gLevelExperience[0] = 0;	
	int LevelOver_N=1;
#ifdef HERO_SYSTEM_MODIFY
	for( n=1; n<MAX_LEVEL+1; n++)
#else
	for( n=1; n<MAX_LEVEL+1; n++)
#endif
	{
		gLevelExperience[n] = (9+n)*n*n*10;
		if(n > 400)
		{
			gLevelExperience[n] += (9+LevelOver_N)*LevelOver_N*LevelOver_N*0.8f;
			LevelOver_N++;
		}
		else if( n > 255 )
		{
			gLevelExperience[n] += (9+LevelOver_N)*LevelOver_N*LevelOver_N*1000;
			LevelOver_N++;
		}		
//		LogAdd("%d\t%u", n, gLevelExperience[n]);
//		LogAdd("%3d\t%u\t%u", n, gLevelExperience[n], gLevelExperience[n]-gLevelExperience[n-1]);
	}

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// �����ͷ��� ����ġ ���̺� ���
	g_MasterLevelSystem.SetMasterLevelExpTlb();	
#endif

	// ������ �̸� �ʱ�ȭ
	char MapAttrName[MAX_MAP][15] = {
		"terrain1.att",		// �η��þ�
		"terrain2.att",		// ���� 
		"terrain3.att",		// ����ƽ�
		"terrain4.att",		// �븮��
		"terrain5.att",		// �ν�Ʈ Ÿ��
		"terrain6.att",		// 
		"terrain7.att",		// �����
		"terrain8.att",		// ��Ʋ����
		"terrain9.att",		// Ÿ��ĭ
		"terrain10.att",	// ����������
#ifdef FOR_BLOODCASTLE
		"terrain11.att",	// õ��.
		"terrain12.att",	// ����ĳ��1
		"terrain12.att",	// ����ĳ��2
		"terrain12.att",	// ����ĳ��3
		"terrain12.att",	// ����ĳ��4
		"terrain12.att",	// ����ĳ��5
		"terrain12.att",	// ����ĳ��6
#endif
#ifdef BLOODCASTLE_EXTEND_20040314
		"terrain12.att",	// ����ĳ��7
#endif
#ifdef CHAOSCASTLE_SYSTEM_20040408
		"terrain19.att",	// ī���� ĳ��1
		"terrain19.att",	// ī���� ĳ��2
		"terrain19.att",	// ī���� ĳ��3
		"terrain19.att",	// ī���� ĳ��4
		"terrain19.att",	// ī���� ĳ��5
		"terrain19.att",	// ī���� ĳ��6
#endif
#ifdef ADD_NEW_MAP_KALIMA_20040518
		"terrain25.att",	// Į���� 1
		"terrain25.att",	// Į���� 2
		"terrain25.att",	// Į���� 3
		"terrain25.att",	// Į���� 4
		"terrain25.att",	// Į���� 5
		"terrain25.att",	// Į���� 6
#endif
#ifdef ADD_NEW_MAP_CASTLE_20041115
		"terrain31.att",	// �� ��
#endif
#ifdef ADD_NEW_MAP_CASTLE_HUNTZONE_20041115
		"terrain32.att",	// �� ����� ��
#endif
#ifdef DEVILSQUARE_EXTEND_20050221			// ���������� ���߰�
		"terrain33.att",	// ���������� �� Ȯ��
#endif
#ifdef ADD_NEW_MAP_AIDA_20050617
		"terrain34.att",	// ���̴� ��
#endif
#ifdef	ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414 
		"terrain35.att",	// ũ���̿��� 1����
#endif
#ifdef	ADD_NEW_MAP_CRYWOLF_SECONDZONE_20050414 
		"terrain36.att",	// ũ���̿��� 2����
#endif
#ifdef HIDDEN_KALIMA_20050706
		"terrain37.att",	// ���� Į����
#endif
#ifdef ADD_NEW_MAP_KANTURU_COMMON_20060523
		"terrain38.att",	// ĭ���� 1�� ��
		"terrain39.att",	// ĭ���� 2�� ��		
#endif
#ifdef ADD_NEW_MAP_KANTURU_BOSS_20060627
		"terrain40.att",	// ĭ���� ���� ��		
#endif
#ifdef UPDATE_GM_FUNCTION_20070228
		"terrain41.att",	// GM ��ȯ ����
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� ���� ��(att) �߰�
		"terrain42.att",	// 3�� ���� ����Ʈ ��
		"terrain43.att",	// 3�� ���� ����Ʈ ������
#endif		
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
		"terrain46.att",	// ������
		"terrain46.att",	// ������
		"terrain46.att",	// ȯ�����1
		"terrain46.att",	// ȯ�����2
		"terrain46.att",	// ȯ�����3
		"terrain46.att",	// ȯ�����4
		"terrain46.att",	// ȯ�����5
		"terrain46.att",	// ȯ�����6
#endif
#ifdef ADD_SEASON_3_NEW_MAP_20070910
		"terrain52.att",	// ���� 3 �ű� ��
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
		"terrain12.att",	// ����ĳ��8
#endif
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
		"terrain19.att",	// ī���� ĳ��7
#endif
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
		"terrain57.att",	// ������!
		"terrain57.att",	// ������!
		"terrain57.att",	// ����Ǵ�
#endif
#ifdef ADD_RAKLION_20080408
		"terrain58.att",	// ��Ŭ���� �ʵ�
		"terrain59.att",	// ��Ŭ���� ������
#endif // ADD_RAKLION_20080408

#ifdef UPDATE_XMASEVENT_MAP_20080930
		"terrain62.att",	// �ؿ� ũ�������� �̺�Ʈ��
		"terrain62.att",	// �ؿ� ũ�������� �̺�Ʈ��
		"terrain62.att",	// �ؿ� ũ�������� �̺�Ʈ��
		"terrain62.att",	// �ؿ� ũ�������� �̺�Ʈ��
#endif // UPDATE_XMASEVENT_MAP_20080930	
#ifdef ADD_NEWPVP_PKFIELD
		"terrain63.att",	// ��ī��������
		"terrain64.att",	// ������
#endif // ADD_NEWPVP_PKFIELD
	};

#ifdef CRYWOLF_MAP_SETTING_20050915
	g_Crywolf.LoadCrywolfMapAttr( gDirPath.GetNewPath("terrain35_PEACE.att"), CRYWOLF_OCCUPATION_STATE_PEACE );
	g_Crywolf.LoadCrywolfMapAttr( gDirPath.GetNewPath("terrain35_OCCUPIED.att"), CRYWOLF_OCCUPATION_STATE_OCCUPIED );
	g_Crywolf.LoadCrywolfMapAttr( gDirPath.GetNewPath("terrain35_WAR.att"), CRYWOLF_OCCUPATION_STATE_WAR );
#endif

#ifdef ADD_KANTURU_20060627
	g_Kanturu.LoadKanturuMapAttr( gDirPath.GetNewPath( "terrain40_CLOSE.att" ), KANTURU_MAP_TOWEROFREFINEMENT_CLOSE );
	g_Kanturu.LoadKanturuMapAttr( gDirPath.GetNewPath( "terrain40_OPEN.att" ), KANTURU_MAP_TOWEROFREFINEMENT_OPEN );
#endif

	DCInfo.Init();	
	ShopDataLoad();	
	wsGServer.CreateSocket(hWnd);		// ??? ���� ������
	wsJServerCli.CreateSocket(hWnd);
	wsDataCli.CreateSocket(hWnd);

#ifdef AUTH_GAMESERVER	
	gGameServerAuth.GetKey(szAuthKey, 5, 5);
#endif

#ifdef GAMESERVER_INFO_TCP_20031203
	wsConnectServerCli.CreateSocket(hWnd);
#endif


	//#ifdef _NEW_EXDB_
	//##EXDB##
	//wsExDbCli.CreateSocket(hWnd);
	//##EXDB##
	//#endif

#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#4 ��Ÿ ���� �б� ����");
#endif

	// ���������� ��ŷ ����
	wsRServerCli.CreateSocket(hWnd);

#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#5 ���� ������ ����");
#endif	

	// �̺�ƮĨ ����
	wsEvenChipServerCli.CreateSocket(hWnd);

#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#6 �̺�ƮĨ ����");
#endif
	
	GameServerInfoSend();		// ���Ӽ����� ���Ӽ��� ���� ����
#ifdef GAMESERVER_INFO_EXTENDED_20040625	
	GameServerExtInfoSend();	// ���Ӽ����� ���Ӽ��� �߰����� ���� ����
#endif

	GameMonsterAllAdd();
	acceptIP.Load(gDirPath.GetNewPath("Iplist.dat"));
	ConMember.Load(gDirPath.GetNewPath("ConnectMember.txt"));	

#ifndef EVENT_MANAGEMENT_20040711
	DragonEvent = new CDragonEvent;
	if( DragonEvent == NULL )
	{
		MsgBox("CDragonEvent %s", lMsg.Get(110));
		return;
	}

	AttackEvent = new CAttackEvent;
	if( AttackEvent == NULL )
	{
		MsgBox("AttackEvent %s", lMsg.Get(110));
		return;
	}
#endif
	
	GCTeleportSend(&gObj[0], 1, 1, 1, 2, 3);		// ??? �׽�Ʈ ��

#ifdef AUTH_GAMESERVER	
	gGameServerAuth.GetKey(szAuthKey, 15, 5);
#endif

#ifndef GAMESERVER_TO_CASTLESIEGESERVER
#ifdef ADD_RAKLION_20080408
	g_Raklion.SetState( RAKLION_STATE_END );
#endif // ADD_RAKLION_20080408
#endif // GAMESERVER_TO_CASTLESIEGESERVER

#ifdef MODIFY_LUA_SCRIPT_20090430
	g_MuLua.DoFile(gDirPath.GetNewPath("Lua_Test.lua"));

	///////// ���� �ڵ� �Դϴ�. ���߿� �� ���� �ּ���///////
	//----------------->
	int result = 0;
	lua_State *L = g_MuLua.GetLua();
	g_Generic_Call(L, "LuaBindReturn", ">i", &result);
	
	TRACE_LOG("\n result %d",result);	
	//<-----------------
#endif

}


//----------------------------------------------------------------------------
// â�� ���ᰡ ������ ��´�. "�󸶿���?"
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
int GetWarehouseUsedHowMuch( int UserLevel, int UserMasterLevel, int IsLock )
#else
int GetWarehouseUsedHowMuch(int UserLevel, int IsLock)
#endif
{
	int  rZen=0;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// ���շ��� - â�� ���� ����
	int iTotalLevel = UserLevel + UserMasterLevel;
	rZen = iTotalLevel * iTotalLevel * 0.1 * 0.4 ;
#else
	rZen = UserLevel*UserLevel*0.1*0.4;
#endif

	if( bCanWarehouseLock == TRUE )
	{
		if( IsLock )
		{	// â�� ��� ������ level*2�� �ݾ��� �߰�
			rZen += UserLevel*2;
		}
	}

	if( rZen < 1 ) rZen = 1;

	if( rZen >= 1000 )
	{
		rZen = rZen/100*100;
	}
	else if( rZen >= 100 )
	{
		rZen = rZen/10*10;
	}

	if( rZen == 0 )
		rZen = 1;
	
	return rZen;
}

void GraphPaint(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	RECT rect;
	int  val[10];
	HBRUSH colBrush[4];

	colBrush[0] = (HBRUSH)CreateSolidBrush(RGB(100,100,100));
	colBrush[1] = (HBRUSH)CreateSolidBrush(RGB(200,100,100));
	colBrush[2] = (HBRUSH)CreateSolidBrush(RGB(200,200,200));
	colBrush[3] = (HBRUSH)CreateSolidBrush(RGB(200,200,100));
 
	// Class,Str,Dex,Vit,Energy,Life,Mana,LevelLife,LevelMana,VitalityToLife,EnergyToMana 
	for( int n=0; n<4; n++)
	{
		val[0] = DCInfo.DefClass[n].Strength;
		val[1] = DCInfo.DefClass[n].Dexterity;
		val[2] = DCInfo.DefClass[n].Vitality;
		val[3] = DCInfo.DefClass[n].Energy;
		val[4] = (int)(DCInfo.DefClass[n].Life);
		val[5] = (int)(DCInfo.DefClass[n].Mana);
		val[6] = (int)(DCInfo.DefClass[n].LevelLife*10);
		val[7] = (int)(DCInfo.DefClass[n].LevelMana*10);
		val[8] = (int)(DCInfo.DefClass[n].VitalityToLife*10);
		val[9] = (int)(DCInfo.DefClass[n].EnergyToMana*10);

		for( int i=0; i<10; i++)
		{
			rect.left = 0+(i*45)+(n*10);
			rect.right = 10+(i*45)+(n*10);
			rect.top  = 0;
			rect.bottom = val[i];
			FillRect(hdc, &rect, (HBRUSH)colBrush[n]);
		}
	}
	ReleaseDC(hWnd, hdc);

	DeleteObject(colBrush[0]);
	DeleteObject(colBrush[1]);
	DeleteObject(colBrush[2]);
	DeleteObject(colBrush[3]);
}

// ��� ���͸� �����Ѵ�.
void GameMonsterAllAdd()
{
	int result;

	gCurPaintPlayer = 0;
	for( int n=0; n<gMSetBase.m_Count; n++)
	{
#ifdef FOR_BLOODCASTLE
		
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
		if( CHECK_DEVILSQUARE(gMSetBase.m_Mp[n].m_MapNumber) )
#else
		if (gMSetBase.m_Mp[n].m_MapNumber == 9 )	// �Ǹ��� ������ ���� �߰����ش�.
#endif
			continue;

		if (CHECK_BLOODCASTLE(gMSetBase.m_Mp[n].m_MapNumber))	// ����ĳ���� ���� �߰����ش�. (��õ�簡 �ƴ� ��츸)
		{
			if (gMSetBase.m_Mp[n].m_Type != 232)
				continue;
		}

#ifdef CHAOSCASTLE_SYSTEM_20040408					// ���� ���� �߰� �� ī����ĳ�� ����
		if (CHECK_CHAOSCASTLE(gMSetBase.m_Mp[n].m_MapNumber))	// ī����ĳ���� ���� �߰����ش�.
		{
			continue;
		}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ���� �̺�Ʈ�� ������Ʈ�� ���� �߰�
		if (CHECK_ILLUSION_TEMPLE(gMSetBase.m_Mp[n].m_MapNumber))
		{
			// ��ġ���̺� ��ȣ �̸� ����
			if (gMSetBase.m_Mp[n].m_Type != NPC_LEADER_OF_ALLIED_FORCE		// ���ձ� �屳
				&& gMSetBase.m_Mp[n].m_Type != NPC_ELDER_OF_ILLUSION)		// ȯ���� ���
			{
				if(gMSetBase.m_Mp[n].m_Type == NPC_ILLUSION_HOLY_RELICS_STATUE		// NPC
					|| gMSetBase.m_Mp[n].m_Type == NPC_HOLY_RELICS_BOX_ALLIED
					|| gMSetBase.m_Mp[n].m_Type == NPC_HOLY_RELICS_BOX_ILLUSION)
				{
					g_IllusionTempleEvent.AddNpcPosNum(gMSetBase.m_Mp[n].m_MapNumber, gMSetBase.m_Mp[n].m_Type, n);
				}
				else if(gMSetBase.m_Mp[n].m_Type >= MON_GHOST_OF_ILLUSION_BROTHERHOOD_A1	 // Monster
					|| gMSetBase.m_Mp[n].m_Type <= MON_GHOST_OF_ILLUSION_BROTHERHOOD_C6)
				{
					g_IllusionTempleEvent.AddMonsterPosNum(gMSetBase.m_Mp[n].m_MapNumber, gMSetBase.m_Mp[n].m_Type, n);
				}

				
				continue;
			}
		}
#endif
		
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
		result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);
#else
		result = gObjAdd();
#endif
	
		if( result >= 0 )
		{
			gObjSetPosMonster(result, n);
			gObjSetMonster(result, gMSetBase.m_Mp[n].m_Type);
			gCurPaintPlayer++;

			// ��õ��� �ڽ��� ����ĳ�� ���ι�ȣ�� ���⼭ �߰��� �־�� �Ѵ�.
			if (CHECK_BLOODCASTLE(gObj[result].MapNumber) && (gObj[result].Class == 232)) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				gObj[result].m_cBloodCastleIndex = g_BloodCastle.GetBridgeIndexByMapNum( gObj[result].MapNumber );
#else
				gObj[result].m_cBloodCastleIndex = gObj[result].MapNumber - MAP_INDEX_BLOODCASTLE1;
#endif
			}

#ifdef CASTLE_MAIN_SCHEDULER_20041111				// �հ��� �ε����� �����Ѵ�.
			if (gObj[result].Class == 216) {
				g_CastleSiege.SetCrownIndex (result);
			}
#endif

#ifdef CRYWOLF_NPC_WORK_20050912
			
			if( CHECK_CRYWOLF_FIRSTZONE(gObj[result].MapNumber) 
				&& gObj[result].Type == OBJTYPE_NPC 
			  )
			{
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3������ NPC�� ũ���̿��� NPC���� �����Ѵ�.
				if( gObj[result].Class == 406		// ���� ����
					|| gObj[result].Class == 407	// ������� ����
					)
				{
					// �н�.
				}
				else
#endif				
				// ũ���̿��� Ư�� NPC�� ����Ѵ�.	- ����, ����
				if( CHECK_CRYWOLF_SPECIAL_NPC( gObj[result].Class ) )
				{
					g_Crywolf.m_ObjSpecialNPC.AddObj( result );
				}
				// ũ���̿��� �Ϲ� NPC�� ����Ѵ�.	���ջ�
				else
				{
					g_Crywolf.m_ObjCommonNPC.AddObj( result );
				}
			}
#endif

#ifdef CRYWOLF_MONSTER_WORK_20050914
			if( CHECK_CRYWOLF_FIRSTZONE(gObj[result].MapNumber) 
				&& gObj[result].Type == OBJTYPE_MONSTER
			  )
			{
				// ũ���̿��� �Ϲ� MONSTER�� ����Ѵ�.
				g_Crywolf.m_ObjCommonMonster.AddObj( result );
			}
#endif
			
		}

#else
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
		if( !CHECK_DEVILSQUARE(gMSetBase.m_Mp[n].m_MapNumber) )
#else
		if( gMSetBase.m_Mp[n].m_MapNumber != 9 )	// �Ǹ��� ������ ���� �߰����ش� ??
#endif
		{
			result = gObjAdd();
		
			if( result >= 0 )
			{
				gObjSetPosMonster(result, n);
				gObjSetMonster(result, gMSetBase.m_Mp[n].m_Type);
				gCurPaintPlayer++;
			}
		}
#endif
	}

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	// �׷� ���� ������ ��� �ϰ� �ʱ�ȭ �Ѵ�.
//	TMonsterAIGroup::InitAll();
#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725


	
#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// ������ ���뼭�� �϶��� ����������, ����ĳ��, ī����ĳ��, ��������� �̺�Ʈ�� �����ȴ�.
	gDevilSquareEvent	= FALSE;
	g_bChaosCastle		= FALSE;
#ifdef ADD_RAKLION_20080408
	g_bRaklionEvent		= FALSE;
#endif // ADD_RAKLION_20080408
#endif	
	
#if TESTSERVER == 1		// �׽�Ʈ �������� ���� & ī����ĳ�� ���� �׽�Ʈ �ϱ� ���ؼ�
//	g_bChaosCastle		= TRUE;
#endif

	g_DevilSquare.Init();

#ifdef FOR_BLOODCASTLE
	g_BloodCastle.LoadItemDropRate();

#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// ������ ���뼭�� �϶��� ����������, ����ĳ��, ī����ĳ��, ��������� �̺�Ʈ�� �����ȴ�.
	g_bBloodCastle		= FALSE;
#endif	
	
	if(g_bBloodCastle) {
		g_BloodCastle.Init();
	}
	else {
		g_BloodCastle.Init(false);
	}
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
//	g_ChaosCastle.LoadItemDropRate();

	if(g_bChaosCastle) {
		g_ChaosCastle.Init();
	}
	else {
		g_ChaosCastle.Init(false);
	}
#endif

#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// ���������� ��� ����ȵ�
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	g_bIllusionTampleEvent	= FALSE;
#endif
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ���� �̺�Ʈ�� �ʱ�ȭ
	if(g_bIllusionTampleEvent)
	{
		g_IllusionTempleEvent.IllusionTempleEventInit();
	}
#endif

	g_PkClearSystem.Load(gDirPath.GetNewPath("PkClearSystem.cfg"));
	g_ResetSystem.Load(gDirPath.GetNewPath("ResetSystem.cfg"));
	g_BuffHelper.Load(gDirPath.GetNewPath("BuffHelperSystem.cfg"));

#if 0
	CreateThread(NULL, 0, CNewsSystem::Process, NULL, 0, NULL);
#endif
}

// ��� ���͸� �����Ѵ�.
void GameMonsterAllCloseAndReLoad()
{
	g_PkClearSystem.ClearPkClearNPCData();
	g_ResetSystem.ClearResetNPCData();
	g_BuffHelper.ClearNPCData();

	for( int n=0; n<MAX_MONSTER; n++)
	{
		if( gObj[n].Type == OBJTYPE_MONSTER || gObj[n].Type == OBJTYPE_NPC )
		{
#ifdef ADD_KANTURU_20060627		// ĭ���� ���� ���ʹ� ���� ���ε�� ������� �ȵȴ�.
			if( gObj[n].MapNumber == MAP_INDEX_KANTURU_BOSS )
				continue;

#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// ������ ���� ���ʹ� ���� ���ε�� ������� �ʵȴ�.
			if (gObj[n].m_btCsNpcType != CS_NPC_TYPE_NONE)
				continue;
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
			if ( gObj[n].m_iCurrentAI )
			{
				gObj[n].Live = 0;
				gObjViewportListProtocolDestroy(&gObj[n]);
				gObjViewportClose(&gObj[n]);
			}
#endif
			gObjDel(n);
		}
	
	}

#ifdef CRYWOLF_NPC_WORK_20050912
	// ũ���̿��� ������ NPC ���� �ʱ�ȭ
	g_Crywolf.m_ObjCommonNPC.Reset();
	g_Crywolf.m_ObjSpecialNPC.Reset();
#endif

#ifdef CRYWOLF_MONSTER_WORK_20050914
	// ũ���̿��� ������ ���� ���� �ʱ�ȭ
	g_Crywolf.m_ObjCommonMonster.Reset();
	g_Crywolf.m_ObjSpecialMonster.Reset();
#endif

#ifdef AUTH_GAMESERVER
	int DataBufferSize;
	char* DataBuffer;
	
	gGameServerAuth.RequestData(FILE_MONSTER);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMAttr.LoadAttr(DataBuffer, DataBufferSize);						// ���� �Ӽ����� �ε�	

	gGameServerAuth.RequestData(FILE_MONSTERSETBASE);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMSetBase.LoadSetBase(DataBuffer, DataBufferSize);	// ���� ��ġ���� �ε�
#else
	#ifdef SCRIPT_DECODE_WORK	
		gMAttr.LoadAttrDecode(gDirPath.GetNewPath("Monster.cwz"));
		gMSetBase.LoadSetBaseDecode(gDirPath.GetNewPath("MonsterSetBase.cwz"));
	#else
		gMAttr.LoadAttr(gDirPath.GetNewPath("Monster.txt"));
		gMSetBase.LoadSetBase(gDirPath.GetNewPath("MonsterSetBase.txt"));
	#endif
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// NPC, ������ ��ġ���� ����
		g_IllusionTempleEvent.ResetPosNum();
#endif

	// ���Ϳ��� �� ������ ����Ʈ �ۼ�
	g_MonsterItemMng.Init();
	
	gObjMonCount	= 0; 
	GameMonsterAllAdd();

#ifdef MODIFY_SELUPAN_REGEN_MONSTER_RELOADING_BUGFIX_20090128
	g_Raklion.SetState( RAKLION_STATE_END );
#endif // MODIFY_SELUPAN_REGEN_MONSTER_RELOADING_BUGFIX_20090128
}


//----------------------------------------------------------------------------
// ���Ӽ����� ���� �������� �����Ų��.
void GameMainFree()
{		
	gDbSave.End();
	GDGameServerInfoSave();
	gObjEnd();

	ClearBattleSoccer();	
	LogClose();
	
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	// Ŭ���̾�Ʈ ������ �޸� ����
	CleanupGameguardAuth();
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	// Ŭ���̾�Ʈ ������ �޸� ����
	CleanupGameguardAuth25();
#endif
}

//----------------------------------------------------------------------------
// ���������� �����Ѵ�.
BOOL GMJoinServerConnect(char* ConnectServer, DWORD wMsg)
{
	BOOL  result;
	
	wsJServerCli.SetProtocolCore( SProtocolCore );
	result = wsJServerCli.Connect(ConnectServer, JoinServerPort, wMsg);
	if( result == FALSE ) return FALSE;
	
	GJServerLogin(); // ���Ӽ����� �α��� ������ ������.
	LogAdd(lMsg.Get(401), ConnectServer);
	return TRUE;
}

// ���������� ��ŷ ������ �����Ѵ�
BOOL GMRankingServerConnect(char* RankingServer, DWORD wMsg)
{
	BOOL  result;
	
	wsRServerCli.SetProtocolCore( DevilSquareEventProtocolCore );
	result = wsRServerCli.Connect(RankingServer, 44455, wMsg);
	//result = wsRServerCli.Connect(RankingServer, 60001, wMsg);
	if( result == FALSE ) return FALSE;	
	
	LogAdd("RankingServer Connect IP [ %s ]", RankingServer);
	return TRUE;
}

// �̺�ƮĨ ������ �����Ѵ�
BOOL GMEventChipServerConnect(char* ServerIP, DWORD wMsg)
{
	BOOL  result;
	
	wsEvenChipServerCli.SetProtocolCore( EventChipEventProtocolCore );
	result = wsEvenChipServerCli.Connect(ServerIP, 44456, wMsg);
	
	if( result == FALSE ) return FALSE;	
	
	LogAdd("EventChip Connect IP [ %s ]", ServerIP);
	return TRUE;
}


//----------------------------------------------------------------------------
// �����ͼ����� �����Ѵ�.
BOOL GMDataServerConnect(char *ConnectServer, DWORD wMsg)
{
	BOOL result;

	wsDataCli.SetProtocolCore(DataServerProtocolCore);
	result = wsDataCli.Connect(ConnectServer, DataServerPort, wMsg);
	if( result == FALSE ) 
	{
		LogAdd(lMsg.Get(403), ConnectServer, DataServerPort);
		return FALSE;
	}
	
	DataServerLogin(0);

	LogAdd(lMsg.Get(402), ConnectServer, DataServerPort);
	return TRUE;
}

#ifdef _NEW_EXDB_
//##EXDB##
//----------------------------------------------------------------------------
// Ȯ�� �����ͼ����� �����Ѵ�.
BOOL ExDataServerConnect(char *ConnectServer, DWORD wMsg)
{
	BOOL result;
	wsExDbCli.SetProtocolCore(ExDataServerProtocolCore);
#ifdef ADD_GAMESERVER_PARAMETER_20050728
	result = wsExDbCli.Connect(ConnectServer, ExDbPort, wMsg);
#else
	result = wsExDbCli.Connect(ConnectServer, 55906, wMsg);
#endif
	if( result == FALSE ) 
	{
#ifdef ADD_GAMESERVER_PARAMETER_20050728
		LogAdd(lMsg.Get(403), ConnectServer, ExDbPort);
#else
		LogAdd(lMsg.Get(403), ConnectServer, 55906);
#endif
		return FALSE;
	}
	
	ExDataServerLogin();

	LogAddC(LOGC_RED, "ExDB Server Online");
	//LogAdd(lMsg.Get(402), ConnectServer, DataServerPort);
	return TRUE;
}
//##EXDB##
#endif


#ifdef GAMESERVER_INFO_TCP_20031203

void ConnectSeverProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen)
{
	switch( protoNum )
	{
		case 0x01 :
		break;
	}
}

//----------------------------------------------------------------------------
// ���Ӽ����� �����Ѵ�.
BOOL ConnectServerConnect(char *ServerIP, DWORD wMsg)
{
	BOOL  result;
	
	wsConnectServerCli.SetProtocolCore( ConnectSeverProtocolCore );
	result = wsConnectServerCli.Connect(ServerIP, connectserverport_tcp, wMsg);
	
	if( result == FALSE ) return FALSE;	
	
	LogAdd("ConnectServer Connect IP [ %s ]", ServerIP);
	return TRUE;
}

#endif


//----------------------------------------------------------------------------
// ������ ��������, �� �Լ��� �̿��� ���� ������ �����Ѵ�.
BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg)
{	
	gDbSave.Begin();
	LogAdd(lMsg.Get(404),GameServerPort);
	return TRUE;
}

//----------------------------------------------------------------------------
// ���Ӽ��� �޽��� (����� ������ ó���Ѵ�)
void GMServerMsgProc(WPARAM wParam, LPARAM lParam)
{
//	SOCKET cSocket;
//	IN_ADDR cInAddr;

	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_ACCEPT :
		/*//LogAdd("SFD_ACCEPT MESSAGE");
		if( wsGServer.AcceptSocket(cSocket, cInAddr) == TRUE )
		{
			int addnumber = gObjAdd(cSocket, inet_ntoa(cInAddr));
			if( addnumber >= 0 ) 
			{
				SCPJoinResultSend( addnumber, 0x01);
			}
			else 
			{// ����ڸ� ©��� ��..
				wsGServer.Close(cSocket);
			}
		}
		else {
			return;
		}
		*/
		break;
	}
}

//----------------------------------------------------------------------------
// ���Ӽ������� Ŭ���̾�Ʈ���� �޽��� ó��
void GMClientMsgProc(WPARAM wParam, LPARAM lParam)
{	
	WPARAM wp=wParam;
	LPARAM lp=lParam;

	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_READ :
		wsGServer.DataRecv(wParam);
		break;
	case FD_WRITE :
		wsGServer.FDWRITE_MsgDataSend((SOCKET)wParam);
		break;
	case FD_CLOSE :
		{
			//int index = wsGServer.GetSocketIndex((SOCKET)wParam);
			int index = gObjGetSocket( wParam );
			if( index >= 0 ) 
			{
				//wsGServer.Close(gObj[index].m_socket);
				//gObjDel((short)index);
				//CloseClient ( index );
			}
		}
		break;
	}
}

//----------------------------------------------------------------------------
// ������������ ��� �޽��� ó��
void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_READ :
		wsJServerCli.DataRecv();
		break;
	case FD_WRITE :
		wsJServerCli.FDWRITE_MsgDataSend();
		break;
	case FD_CLOSE :
		LogAddC(LOGC_RED, lMsg.Get(405));
#ifdef MODIFY_AUTHSERVER_CLOSE_20041207
		gbAuthServerClosed	= TRUE;
#endif
		SendMessage(ghWnd, WM_CLOSE, 0,0);		
		break;
	}
}

//----------------------------------------------------------------------------
// ��ŷ�������� ��� �޽��� ó��
void GMRankingClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_READ :
		wsRServerCli.DataRecv();
		break;
	case FD_WRITE :
		wsRServerCli.FDWRITE_MsgDataSend();
		break;
	case FD_CLOSE :
		IsDevilSquareEventConnected = 0;		
		//SendMessage(ghWnd, WM_CLOSE, 0,0);
		break;
	}
}

//----------------------------------------------------------------------------
// �̺�Ʈ Ĩ�������� ��� �޽��� ó��
void GMEventChipClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_READ :
		wsEvenChipServerCli.DataRecv();
		break;
	case FD_WRITE :
		wsEvenChipServerCli.FDWRITE_MsgDataSend();
		break;
	case FD_CLOSE :
		IsEventChipServerConnected = 0;		
		//SendMessage(ghWnd, WM_CLOSE, 0,0);
		break;
	}
}


#ifdef GAMESERVER_INFO_TCP_20031203
//----------------------------------------------------------------------------
// ���Ӽ������� ��� �޽��� ó��
void GMConnectServerClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_READ :
		wsConnectServerCli.DataRecv();
		break;
	case FD_WRITE :
		wsConnectServerCli.FDWRITE_MsgDataSend();
		break;
	case FD_CLOSE :
		IsConnectServerConnected = 0;		
		break;
	}
}
#endif

#ifdef _NEW_EXDB_
//##EXDB##
//----------------------------------------------------------------------------
// Ȯ�嵥���� �������� ��� �޽��� ó��
void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_READ :
		wsExDbCli.DataRecv();
		break;
	case FD_WRITE :
		wsExDbCli.FDWRITE_MsgDataSend();
		break;
	case FD_CLOSE :
		LogAddC(LOGC_RED,lMsg.Get(406));
		wsExDbCli.Close();

#ifdef __FRIEND_WORK__
		// DxDB�� �ٿ�Ǿ���.. ģ���� ��� �������� ���·� �����.
		for( int n=0; n<MAX_OBJECT; n++)
		{
			if( gObjIsConnectedGP(n)==TRUE)
			{
				PMSG_FRIEND_STATE pMsg;
				
				pMsg.h.set((LPBYTE)&pMsg, 0xC4, sizeof(pMsg));
				pMsg.State = 252;
				
				DataSend(n, (LPBYTE)&pMsg, sizeof(pMsg));

				gObj[n].m_FriendServerOnline = FRIENDSERVER_OFFLINE;
			}
		}
		LogAddTD("Error-L1 : Friend Server Down (State Send Ok)");
		LogAddC(LOGC_RED, "Error-L1 : Friend Server Down (State Send Ok)");
#endif
		break;
	}
}
//##EXDB##
#endif

//----------------------------------------------------------------------------
// ������ �������� ��� �޽��� ó��
void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_READ :
		wsDataCli.DataRecv();
		break;
	case FD_WRITE :
		wsDataCli.FDWRITE_MsgDataSend();
		break;
	case FD_CLOSE :
		LogAddC(LOGC_RED,lMsg.Get(406));
		//SendMessage(ghWnd, WM_CLOSE, 0,0);
		break;
	}
}
       
// ���� ���� ������ ��´�.
void ReadServerInfo()
{
	if( IsFile(".\\data\\Serverinfo.dat") == FALSE )
	{
		MsgBox("ServerInfo.dat file not found");
	}
	GetPrivateProfileString("GameServerInfo", "ServerName", "", szServerName, 50, ".\\data\\Serverinfo.dat");
	gGameServerCode = GetPrivateProfileInt("GameServerInfo", "ServerCode", 0, ".\\data\\Serverinfo.dat");
	bCanConnectMember = GetPrivateProfileInt("GameServerInfo", "ConnectMemberLoad", 0, ".\\data\\Serverinfo.dat");

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410	// ĳ���� ���� ����
	g_bChangeCharacterName	= GetPrivateProfileInt("GameServerInfo", "ChangeCharacterName", 0, ".\\data\\Serverinfo.dat");
#endif	// UPDATE_CHANGE_CHARACTERNAME_20080410
}

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
// ���Ӱ��� ������Ʈ ������ �д´�.
void ReadGameGuardUpdateInfo()
{
	if( IsFile(gDirPath.GetNewPath("GameGuardModule.dat")) == FALSE )
	{
		MsgBox("GameGuardModule.dat file not found");
	}
	GetPrivateProfileString("GameGuardModule", "GGModuleName", "", gGGModuleName, 20, gDirPath.GetNewPath("GameGuardModule.dat"));
}
#endif

// ���� ���� ������ ��´�.
void ReadCommonServerInfo()
{
	char	szTemp[256];	
	char    szCheckSum[255];

	char *	cvstr;

	ReadServerInfo();

#ifdef AUTH_GAMESERVER	
	int DataBufferSize;
	char* DataBuffer;
	gGameServerAuth.RequestData(FILE_GATE);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gGateC.Load(DataBuffer, DataBufferSize);						// ����Ʈ ���� �ε�
#else
	#ifdef SCRIPT_DECODE_WORK
		gGateC.LoadDecode(gDirPath.GetNewPath("gate.cwz"));
	#else
		gGateC.Load(gDirPath.GetNewPath("gate.txt"));
	#endif
#endif

	if( IsFile(gDirPath.GetNewPath("commonserver.cfg")) == FALSE )
	{
		MsgBox("[commonserver.cfg] file not found");
	}

	g_GlobalConfig.Load(gDirPath.GetNewPath("commonserver.cfg"),gDirPath.GetNewPath("CalCharacter.cfg"));
	g_PostSystem.Load(gDirPath.GetNewPath("commonserver.cfg"));

	GetPrivateProfileString("GameServerInfo", "Language", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gLanguage	= atoi(szTemp);

	
	gStalkProtocol = GetPrivateProfileInt("GameServerInfo", "StalkProtocol", 0, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "StalkProtocolId", "0", gStalkProtocolId, MAX_IDSTRING, gDirPath.GetNewPath("commonserver.cfg"));
	if( gStalkProtocol )
	{
		LogAdd("Stalk Protocol ID = '%s' ", gStalkProtocolId);
	}

	char szlMsgName[256];

	if( g_SimpleModulusCS.LoadDecryptionKey(gDirPath.GetNewPath("Dec1.dat")) == FALSE )
	{
		MsgBox("Dec1.dat file not found");
	}
	if( g_SimpleModulusSC.LoadEncryptionKey(gDirPath.GetNewPath("Enc2.dat")) == FALSE )
	{
		MsgBox("Enc2.dat file not found");
	}

#ifdef SCRIPT_DECODE_WORK
	strcpy(szKorItemTextFileName, gDirPath.GetNewPath("lang\\kor\\item(kor).cwz"));
	strcpy(szKorSkillTextFileName, gDirPath.GetNewPath("lang\\kor\\skill(kor).cwz"));
#else
	strcpy(szKorItemTextFileName, gDirPath.GetNewPath("lang\\main\\item.txt"));
	strcpy(szKorSkillTextFileName, gDirPath.GetNewPath("lang\\main\\skill.txt"));
#endif

	strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\main\\movereq.txt"));

	switch( gLanguage ) 
	{
	case 0 : // ���� ����
		strcpy(szlMsgName, gDirPath.GetNewPath("message_kor.wtf"));		

#ifdef SCRIPT_DECODE_WORK
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\kor\\item(kor).cwz"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\kor\\skill(kor).cwz"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\kor\\Quest(Kor).cwz"));		
#else
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\kor\\item(kor).txt"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\kor\\skill(kor).txt"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\kor\\Quest(Kor).txt"));		
#endif
		strcpy(szCheckSum, gDirPath.GetNewPath("lang\\kor\\CheckSum.dat"));

		//strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\kor\\movereq(Kor).txt"));

#ifdef MULTI_CHECKSUM
		g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\kor"));
#endif

		strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\kor\\commonloc.cfg"));



		gNonPK = GetPrivateProfileInt("GameServerInfo", "NonPK", 0, ".\\data\\Serverinfo.dat");
		if( gNonPK ) LogAdd("NON-PK Server");
		
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		gPkLimitFree = GetPrivateProfileInt("GameServerInfo", "PkLimitFree", 0, ".\\data\\Serverinfo.dat");
		if( gPkLimitFree ) LogAdd("PK Limit Free Server");
#endif

#ifdef ADD_NEWSERVER_20081113
		g_bNewServer = GetPrivateProfileInt("GameServerInfo", "NewServer", 0, ".\\data\\Serverinfo.dat");
		if( g_bNewServer ) LogAdd("New Server");
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
		g_bEventOff = GetPrivateProfileInt("GameServerInfo", "EventOff", 0, ".\\data\\Serverinfo.dat");
		if( g_bEventOff ) LogAdd("EventOff");
#endif // ADD_NEWSERVER_EVENTOFF_20081114
		break;

	case 1 : // ���� ����
		strcpy(szlMsgName, gDirPath.GetNewPath("message_eng.wtf"));

#ifdef SCRIPT_DECODE_WORK
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\eng\\item(eng).cwz"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\eng\\skill(eng).cwz"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\eng\\Quest(eng).cwz"));
#else
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\eng\\item(eng).txt"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\eng\\skill(eng).txt"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\eng\\Quest(eng).txt"));
#endif

		//strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\eng\\movereq(Eng).txt"));

		strcpy(szCheckSum, gDirPath.GetNewPath("lang\\eng\\CheckSum.dat"));

#ifdef MULTI_CHECKSUM
		g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\eng"));
#endif

		strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\eng\\commonloc.cfg"));

		
		strcpy(gCountryName, "Eng");

		gNonPK = GetPrivateProfileInt("GameServerInfo", "NonPK", 0, ".\\data\\Serverinfo.dat");
		if( gNonPK ) LogAdd("NON-PK Server");

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		gPkLimitFree = GetPrivateProfileInt("GameServerInfo", "PkLimitFree", 0, ".\\data\\Serverinfo.dat");
		if( gPkLimitFree ) LogAdd("PK Limit Free Server");
#endif

#ifdef ADD_NEWSERVER_20081113
		g_bNewServer = GetPrivateProfileInt("GameServerInfo", "NewServer", 0, ".\\data\\Serverinfo.dat");
		if( g_bNewServer ) LogAdd("New Server");
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
		g_bEventOff = GetPrivateProfileInt("GameServerInfo", "EventOff", 0, ".\\data\\Serverinfo.dat");
		if( g_bEventOff ) LogAdd("EventOff");
#endif // ADD_NEWSERVER_EVENTOFF_20081114
		break;

	case 2 : // �Ϻ� ����
		strcpy(szlMsgName, gDirPath.GetNewPath("message_jpn.wtf"));

#ifdef SCRIPT_DECODE_WORK
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\jpn\\item(jpn).cwz"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\jpn\\skill(jpn).cwz"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\jpn\\Quest(jpn).cwz"));
#else
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\jpn\\item(jpn).txt"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\jpn\\skill(jpn).txt"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\jpn\\Quest(jpn).txt"));		
#endif
		//strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\jpn\\movereq(jpn).txt"));
		strcpy(szCheckSum, gDirPath.GetNewPath("lang\\jpn\\CheckSum.dat"));

#ifdef MULTI_CHECKSUM
		g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\jpn"));
#endif

		strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\jpn\\commonloc.cfg"));
		
		strcpy(gCountryName, "Jpn");

		gNonPK = GetPrivateProfileInt("GameServerInfo", "NonPK", 0, ".\\data\\Serverinfo.dat");
		if( gNonPK ) LogAdd("NON-PK Server");

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		gPkLimitFree = GetPrivateProfileInt("GameServerInfo", "PkLimitFree", 0, ".\\data\\Serverinfo.dat");
		if( gPkLimitFree ) LogAdd("PK Limit Free Server");
#endif

#ifdef ADD_NEWSERVER_20081113
		g_bNewServer = GetPrivateProfileInt("GameServerInfo", "NewServer", 0, ".\\data\\Serverinfo.dat");
		if( g_bNewServer ) LogAdd("New Server");
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
		g_bEventOff = GetPrivateProfileInt("GameServerInfo", "EventOff", 0, ".\\data\\Serverinfo.dat");
		if( g_bEventOff ) LogAdd("EventOff");
#endif // ADD_NEWSERVER_EVENTOFF_20081114
		break;

	case 3 : // �߱� ����
		strcpy(szlMsgName, gDirPath.GetNewPath("message_chs.wtf"));

#ifdef SCRIPT_DECODE_WORK
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\chs\\item(chs).cwz"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\chs\\skill(chs).cwz"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\chs\\Quest(chs).cwz"));
#else
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\chs\\item(chs).txt"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\chs\\skill(chs).txt"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\chs\\Quest(chs).txt"));
#endif

		//strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\chs\\movereq(chs).txt"));
		strcpy(szCheckSum, gDirPath.GetNewPath("lang\\chs\\CheckSum.dat"));

#ifdef MULTI_CHECKSUM
		g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\chs"));
#endif

		strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\chs\\commonloc.cfg"));
		
		strcpy(gCountryName, "Chs");

		gNonPK = GetPrivateProfileInt("GameServerInfo", "NonPK", 0, ".\\data\\Serverinfo.dat");
		if( gNonPK ) LogAdd("NON-PK Server");

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		gPkLimitFree = GetPrivateProfileInt("GameServerInfo", "PkLimitFree", 0, ".\\data\\Serverinfo.dat");
		if( gPkLimitFree ) LogAdd("PK Limit Free Server");
#endif

#ifdef ADD_NEWSERVER_20081113
		g_bNewServer = GetPrivateProfileInt("GameServerInfo", "NewServer", 0, ".\\data\\Serverinfo.dat");
		if( g_bNewServer ) LogAdd("New Server");
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
		g_bEventOff = GetPrivateProfileInt("GameServerInfo", "EventOff", 0, ".\\data\\Serverinfo.dat");
		if( g_bEventOff ) LogAdd("EventOff");
#endif // ADD_NEWSERVER_EVENTOFF_20081114
		break;

	case 4 : // �븸 ����
		strcpy(szlMsgName, gDirPath.GetNewPath("message_tai.wtf"));

#ifdef SCRIPT_DECODE_WORK
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\tai\\item(tai).cwz"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\tai\\skill(tai).cwz"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\tai\\Quest(tai).cwz"));
#else
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\tai\\item(tai).txt"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\tai\\skill(tai).txt"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\tai\\Quest(tai).txt"));
#endif

		//strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\tai\\movereq(tai).txt"));
		strcpy(szCheckSum, gDirPath.GetNewPath("lang\\tai\\CheckSum.dat"));

#ifdef MULTI_CHECKSUM
		g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\tai"));
#endif

		strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\tai\\commonloc.cfg"));		
		strcpy(gCountryName, "Tai");

		gNonPK = GetPrivateProfileInt("GameServerInfo", "NonPK", 0, ".\\data\\Serverinfo.dat");
		if( gNonPK ) LogAdd("NON-PK Server");

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		gPkLimitFree = GetPrivateProfileInt("GameServerInfo", "PkLimitFree", 0, ".\\data\\Serverinfo.dat");
		if( gPkLimitFree ) LogAdd("PK Limit Free Server");
#endif

#ifdef ADD_NEWSERVER_20081113
		g_bNewServer = GetPrivateProfileInt("GameServerInfo", "NewServer", 0, ".\\data\\Serverinfo.dat");
		if( g_bNewServer ) LogAdd("New Server");
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
		g_bEventOff = GetPrivateProfileInt("GameServerInfo", "EventOff", 0, ".\\data\\Serverinfo.dat");
		if( g_bEventOff ) LogAdd("EventOff");
#endif // ADD_NEWSERVER_EVENTOFF_20081114
		break;

	case 5 : // �±� ����
		strcpy(szlMsgName, gDirPath.GetNewPath("message_tha.wtf"));

#ifdef SCRIPT_DECODE_WORK
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\tha\\item(tha).cwz"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\tha\\skill(tha).cwz"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\tha\\Quest(tha).cwz"));
#else
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\tha\\item(tha).txt"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\tha\\skill(tha).txt"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\tha\\Quest(tha).txt"));
#endif

		//strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\tha\\movereq(tha).txt"));
		strcpy(szCheckSum, gDirPath.GetNewPath("lang\\tha\\CheckSum.dat"));

#ifdef MULTI_CHECKSUM
		g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\tha"));
#endif

		strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\tha\\commonloc.cfg"));		
		strcpy(gCountryName, "Tha");
		
		gNonPK = GetPrivateProfileInt("GameServerInfo", "NonPK", 0, ".\\data\\Serverinfo.dat");
		if( gNonPK ) LogAdd("NON-PK Server");
		
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		gPkLimitFree = GetPrivateProfileInt("GameServerInfo", "PkLimitFree", 0, ".\\data\\Serverinfo.dat");
		if( gPkLimitFree ) LogAdd("PK Limit Free Server");
#endif

#ifdef ADD_NEWSERVER_20081113
		g_bNewServer = GetPrivateProfileInt("GameServerInfo", "NewServer", 0, ".\\data\\Serverinfo.dat");
		if( g_bNewServer ) LogAdd("New Server");
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
		g_bEventOff = GetPrivateProfileInt("GameServerInfo", "EventOff", 0, ".\\data\\Serverinfo.dat");
		if( g_bEventOff ) LogAdd("EventOff");
#endif // ADD_NEWSERVER_EVENTOFF_20081114
		break;
	
#ifdef FOR_PHILIPPINE		
	case 6 : // �ʸ��� ����
		strcpy(szlMsgName, gDirPath.GetNewPath("message_phi.wtf"));

#ifdef SCRIPT_DECODE_WORK
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\phi\\item(phi).cwz"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\phi\\skill(phi).cwz"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\phi\\Quest(phi).cwz"));
#else
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\phi\\item(phi).txt"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\phi\\skill(phi).txt"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\phi\\Quest(phi).txt"));
#endif

		//strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\phi\\movereq(phi).txt"));
		strcpy(szCheckSum, gDirPath.GetNewPath("lang\\phi\\CheckSum.dat"));

#ifdef MULTI_CHECKSUM
		g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\phi"));
#endif

		strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\phi\\commonloc.cfg"));		
		strcpy(gCountryName, "Phi");
		
		gNonPK = GetPrivateProfileInt("GameServerInfo", "NonPK", 0, ".\\data\\Serverinfo.dat");
		if( gNonPK ) LogAdd("NON-PK Server");
		
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		gPkLimitFree = GetPrivateProfileInt("GameServerInfo", "PkLimitFree", 0, ".\\data\\Serverinfo.dat");
		if( gPkLimitFree ) LogAdd("PK Limit Free Server");
#endif

#ifdef ADD_NEWSERVER_20081113
		g_bNewServer = GetPrivateProfileInt("GameServerInfo", "NewServer", 0, ".\\data\\Serverinfo.dat");
		if( g_bNewServer ) LogAdd("New Server");
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
		g_bEventOff = GetPrivateProfileInt("GameServerInfo", "EventOff", 0, ".\\data\\Serverinfo.dat");
		if( g_bEventOff ) LogAdd("EventOff");
#endif // ADD_NEWSERVER_EVENTOFF_20081114
		break;
#endif

#ifdef FOR_VIETNAM		// ��Ʈ��
	case 7 :
		strcpy(szlMsgName, gDirPath.GetNewPath("message_vtm.wtf"));

#ifdef SCRIPT_DECODE_WORK
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\vtm\\item(vtm).cwz"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\vtm\\skill(vtm).cwz"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\vtm\\Quest(vtm).cwz"));
#else
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\vtm\\item(vtm).txt"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\vtm\\skill(vtm).txt"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\vtm\\Quest(vtm).txt"));
#endif

		//strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\vtm\\movereq(vtm).txt"));
		strcpy(szCheckSum, gDirPath.GetNewPath("lang\\vtm\\CheckSum.dat"));

#ifdef MULTI_CHECKSUM
		g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\vtm"));
#endif

		strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\vtm\\commonloc.cfg"));		
		strcpy(gCountryName, "Vtm");
		
		gNonPK = GetPrivateProfileInt("GameServerInfo", "NonPK", 0, ".\\data\\Serverinfo.dat");
		if( gNonPK ) LogAdd("NON-PK Server");
		
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		gPkLimitFree = GetPrivateProfileInt("GameServerInfo", "PkLimitFree", 0, ".\\data\\Serverinfo.dat");
		if( gPkLimitFree ) LogAdd("PK Limit Free Server");
#endif

#ifdef ADD_NEWSERVER_20081113
		g_bNewServer = GetPrivateProfileInt("GameServerInfo", "NewServer", 0, ".\\data\\Serverinfo.dat");
		if( g_bNewServer ) LogAdd("New Server");
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
		g_bEventOff = GetPrivateProfileInt("GameServerInfo", "EventOff", 0, ".\\data\\Serverinfo.dat");
		if( g_bEventOff ) LogAdd("EventOff");
#endif // ADD_NEWSERVER_EVENTOFF_20081114
		break;
#endif

#ifdef FOR_USA		// �̱�
	case 8 :
		strcpy(szlMsgName, gDirPath.GetNewPath("message_usa.wtf"));

#ifdef SCRIPT_DECODE_WORK
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\usa\\item(usa).cwz"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\usa\\skill(usa).cwz"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\usa\\Quest(usa).cwz"));
#else
		strcpy(szItemTextFileName, gDirPath.GetNewPath("lang\\usa\\item(usa).txt"));
		strcpy(szSkillTextFileName, gDirPath.GetNewPath("lang\\usa\\skill(usa).txt"));
		strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\usa\\Quest(usa).txt"));
#endif

		//strcpy(szMoveReqFileName, gDirPath.GetNewPath("lang\\usa\\movereq(usa).txt"));
		strcpy(szCheckSum, gDirPath.GetNewPath("lang\\usa\\CheckSum.dat"));

#ifdef MULTI_CHECKSUM
		g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\usa"));
#endif

		strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\usa\\commonloc.cfg"));		
		strcpy(gCountryName, "usa");
		
		gNonPK = GetPrivateProfileInt("GameServerInfo", "NonPK", 0, ".\\data\\Serverinfo.dat");
		if( gNonPK ) LogAdd("NON-PK Server");
		
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
		gPkLimitFree = GetPrivateProfileInt("GameServerInfo", "PkLimitFree", 0, ".\\data\\Serverinfo.dat");
		if( gPkLimitFree ) LogAdd("PK Limit Free Server");
#endif

#ifdef ADD_NEWSERVER_20081113
		g_bNewServer = GetPrivateProfileInt("GameServerInfo", "NewServer", 0, ".\\data\\Serverinfo.dat");
		if( g_bNewServer ) LogAdd("New Server");
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
		g_bEventOff = GetPrivateProfileInt("GameServerInfo", "EventOff", 0, ".\\data\\Serverinfo.dat");
		if( g_bEventOff ) LogAdd("EventOff");
#endif // ADD_NEWSERVER_EVENTOFF_20081114
		break;
#endif // FOR_USA

	}	

#ifdef MULTI_CHECKSUM
	g_MultiChecksum.SetFilePath(gDirPath.GetNewPath("lang\\main"));
#endif

	strcpy(szCheckSum, gDirPath.GetNewPath("lang\\main\\CheckSum.dat"));
	strcpy(szCommonlocIniFileName, gDirPath.GetNewPath("lang\\main\\commonloc.cfg"));

	CheckSumFileLoad(szCheckSum);

#ifdef CHANGE_CHAOSCASTLE_SET_ITEM_DROP
	if( !gChaosCastleSetItemDrop.Load(gDirPath.GetNewPath("CCSetItemDropRate.txt")) )
	{
		MsgBox("CCSetItemDropRate.txt Load Fail");
	}
#endif


#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
	int bret = LoadAuthTable(gDirPath.GetNewPath("CSAuth.tab"));
	if( bret != 8 )
	{
		MsgBox("CSAuth File Load Fail");
	}

	#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_15_20050411
	bret = LoadAuthIndex(gDirPath.GetNewPath("CSAuth.idx"));
	if( bret != 1 )
	{
		MsgBox("CSAuth.idx File Load Fail");
	}	
	#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_15_20050411
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	DWORD dwGGErrCode;

	// Ŭ���̾�Ʈ ���� �˰��� ���� �б� �� �ִ� ����� ���� 
//	dwGGErrCode = InitGameguardAuth(FP_DLL_GAME_GAURD_AUTHENTICATION, MAX_USER);
	dwGGErrCode = InitGameguardAuth("", MAX_USER);
	if(dwGGErrCode != ERROR_SUCCESS)
	{
		MsgBox("Failed initialization of GameGaurd !!! , Error: %d", dwGGErrCode);
		return;
	}

#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	DWORD dwGGErrCode;

	// Ŭ���̾�Ʈ ���� �˰��� ���� �б� �� �ִ� ����� ���� 
	// M ����̺��� ������� Ȯ�� �ʿ�.
#ifdef MODIFY_GAMEGUARD_AUTH_MAX_USER_20060517
	dwGGErrCode = InitGameguardAuth25( gDirPath.GetNewPath(""), 10, false, NPLOG_ERROR | NPLOG_DEBUG );
#else
	dwGGErrCode = InitGameguardAuth25( gDirPath.GetNewPath(""), MAX_USER_FOR_GAMEGUARD_AUTH, false, NPLOG_ERROR);
#endif
	if(dwGGErrCode != ERROR_SUCCESS)
	{
		MsgBox("Failed initialization of GameGaurd !!! , Error: %d", dwGGErrCode);
		return;
	}

	LogAddTD("[GameGuard][Init] - GameGuard 2.5 Version Loaded.");
	char szGameGuardModuleInfo[256];
	ModuleInfo25( szGameGuardModuleInfo, 256);
	LogAddTD("[GameGuard][Module Info] - %s", szGameGuardModuleInfo);
#endif

#ifdef MULTI_CHECKSUM
	g_MultiChecksum.LoadFile();
#endif

	lMsg.LoadWTF(szlMsgName);

	SetMapName();

#ifdef AUTH_GAMESERVER		
	gGameServerAuth.RequestData(FILE_ITEM);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	OpenItemScript(DataBuffer, DataBufferSize);
#else
	#ifdef SCRIPT_DECODE_WORK
		if( OpenItemScriptDecode(szKorItemTextFileName) == FALSE )	
			MsgBox(lMsg.Get(419));
	#else
		// ���� �ѱ��� ������ �а� 
		if( OpenItemScript(szKorItemTextFileName) == FALSE )	
			MsgBox(lMsg.Get(419));
	#endif
#endif

#if 0
#ifdef AUTH_GAMESERVER		
	if( gLanguage != 0 )
	{
		gGameServerAuth.RequestData(FILE_ITEMLOCAL);
		DataBufferSize = gGameServerAuth.GetDataBufferSize();
		DataBuffer = gGameServerAuth.GetDataBuffer();
		OpenItemNameScript(DataBuffer, DataBufferSize);
	}
#else
	#ifdef SCRIPT_DECODE_WORK
		if( gLanguage != 0 )
		{	// �ؿ� ������ ��쿡�� ������ �̸��� �ٽ� �ε��Ѵ�
			if( OpenItemNameScriptDecode(szItemTextFileName) == FALSE )
				MsgBox(lMsg.Get(419));
		}
	#else
		if( gLanguage != 0 )
		{	// �ؿ� ������ ��쿡�� ������ �̸��� �ٽ� �ε��Ѵ�
			if( OpenItemNameScript(szItemTextFileName) == FALSE )
				MsgBox(lMsg.Get(419));
		}
	#endif
#endif
#endif

#ifdef AUTH_GAMESERVER		
	gGameServerAuth.RequestData(FILE_SKILL);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	MagicDamageC.LogSkillList(DataBuffer, DataBufferSize);

#if 0
	if( gLanguage != 0 )
	{	// �ؿ� ������ ��쿡�� ��ų �̸��� �ٽ� �ε��Ѵ�
		gGameServerAuth.RequestData(FILE_SKILLLOCAL);
		DataBufferSize = gGameServerAuth.GetDataBufferSize();
		DataBuffer = gGameServerAuth.GetDataBuffer();
		MagicDamageC.LogSkillNameList(DataBuffer, DataBufferSize);		
	}
#endif
#else
	#ifdef SCRIPT_DECODE_WORK
		// Skill.txt �ε�
		MagicDamageC.LogSkillListDecode(szKorSkillTextFileName);
		//MagicDamageC.LogSkillList(szSkillTextFileName);	
#if 0
		if( gLanguage != 0 )

		{	// �ؿ� ������ ��쿡�� ��ų �̸��� �ٽ� �ε��Ѵ�
			MagicDamageC.LogSkillNameListDecode(szSkillTextFileName);
		}
#endif
	#else
		// Skill.txt �ε�
		MagicDamageC.LogSkillList(szKorSkillTextFileName);
		//MagicDamageC.LogSkillList(szSkillTextFileName);	
#if 0
		if( gLanguage != 0 )
		
		{	// �ؿ� ������ ��쿡�� ��ų �̸��� �ٽ� �ε��Ѵ�
			MagicDamageC.LogSkillNameList(szSkillTextFileName);
		}
#endif
	#endif
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM		
		ChinaHackUserKick.Init();
#endif

#ifdef DARKLORD_WORK
		SkillSpearHitBox.Load(gDirPath.GetNewPath("skillSpear.hit"));
		SkillElectricSparkHitBox.Load(gDirPath.GetNewPath("skillElect.hit"));
#endif
		
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
		gWTEventItemList.Load(gDirPath.GetNewPath("wt_ItemList.txt"));
#endif

#ifdef FOR_CASTLE_TESTSERVER
		g_CastleEventItemList.Load( gDirPath.GetNewPath("CastleEvent_ItemList.txt") );
#endif

#ifdef AUTH_GAMESERVER
	gGameServerAuth.RequestData(FILE_SETITEMOPTION);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();	
	gSetItemOption.LoadOptionInfo(DataBuffer, DataBufferSize);

	gGameServerAuth.RequestData(FILE_SETITEMTYPE);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gSetItemOption.LoadTypeInfo(DataBuffer, DataBufferSize);
#elif GAME_VERSION >= G_V_99B
	gSetItemOption.LoadOptionInfo(gDirPath.GetNewPath("lang\\main\\itemsetoption.txt"));
	gSetItemOption.LoadTypeInfo(gDirPath.GetNewPath("lang\\main\\itemsettype.txt"));
#endif

#ifdef ADD_MACRO_FOR_SETITEM_20060424
	gSetItemMacro.Load(gDirPath.GetNewPath("SetMacro_ItemList.txt"));
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		g_kJewelOfHarmonySystem.LoadScript( gDirPath.GetNewPath("JewelOfHarmonyOption.txt"));
		g_kJewelOfHarmonySystem.LoadScriptOfSmelt( gDirPath.GetNewPath("JewelOfHarmonySmelt.txt"));
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
		g_kItemSystemFor380.Load380ItemOptionInfo( gDirPath.GetNewPath("ItemAddOption.txt"));
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ITEM_ADD_OPTION_20061019
		g_ItemAddOption.Load( gDirPath.GetNewPath("ItemAddOption.txt"));
#endif // ITEM_ADD_OPTION_20061019

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// MasterSkillTree.txt Load
		g_MasterSkillSystem.Load( gDirPath.GetNewPath( "MasterSkillTree.txt" ) );
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
		g_SocketOptionSystem.LoadOptionScript( gDirPath.GetNewPath( "socketitem.txt" ) );
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#ifdef ADD_MOVE_COMMAND_SCRIPT

#ifdef AUTH_GAMESERVER
	gGameServerAuth.RequestData(FILE_MOVEREQ);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();	
	gMoveCommand.Load(DataBuffer, DataBufferSize);
#else
	gMoveCommand.Load(szMoveReqFileName);
#endif
#endif

#ifdef DARKLORD_WORK
	gMoveCommand.LoadMoveLevel(gDirPath.GetNewPath("MoveLevel.txt"));
#endif
	
	ConMember.Load(gDirPath.GetNewPath("ConnectMember.txt"));

	strcpy(szQuestTextFileName, gDirPath.GetNewPath("lang\\main\\quest.txt"));

#ifdef AUTH_GAMESERVER
	gGameServerAuth.RequestData(FILE_QUEST);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();	
	g_QuestInfo.LoadQuestInfo(DataBuffer, DataBufferSize);
#else
	#ifdef SCRIPT_DECODE_WORK
		g_QuestInfo.LoadQuestInfoDecode(szQuestTextFileName);
	#elif defined(NEW_SKILL_FORSKYLAND)
		g_QuestInfo.LoadQuestInfo(szQuestTextFileName);
	#endif
#endif

	//GetPrivateProfileString("GameServerInfo", "ItemFile", "item(Kor).txt", szItemTextFileName, 20, gDirPath.GetNewPath("commonserver.cfg"));
	//GetPrivateProfileString("GameServerInfo", "SkillFile", "skill(Kor).txt", szSkillTextFileName, 20, gDirPath.GetNewPath("commonserver.cfg"));
	
	//ItemFile

#ifdef AUTH_GAMESERVER	
		// ���� ���� ���� ������ �����Ŵ�
	// Ŭ���̾�Ʈ ���� ������ ��´�		
	gGameServerAuth.GetClientVersion(szTemp, szGameServerExeSerial);
	cvstr				= strtok(szTemp, ".");
	szClientVersion[0]	= *cvstr;
	
	cvstr				= strtok(NULL, ".");
	szClientVersion[1]	= *(cvstr);
	szClientVersion[2]	= *(cvstr+1);

	cvstr				= strtok(NULL, ".");
	szClientVersion[3]	= *(cvstr);
	szClientVersion[4]	= *(cvstr+1);
#else
	// Ŭ���̾�Ʈ ���� ���� ���
	GetPrivateProfileString("GameServerInfo", "ClientExeVersion", "", szTemp, 8, szCommonlocIniFileName);
		
	cvstr				= strtok(szTemp, ".");
	szClientVersion[0]	= *cvstr;
	
	cvstr				= strtok(NULL, ".");
	szClientVersion[1]	= *(cvstr);
	szClientVersion[2]	= *(cvstr+1);

	cvstr				= strtok(NULL, ".");
	szClientVersion[3]	= *(cvstr);
	szClientVersion[4]	= *(cvstr+1);

	memset(szGameServerExeSerial, 0, 20);
	GetPrivateProfileString("GameServerInfo", "ClientExeSerial", "1", szGameServerExeSerial, 20, szCommonlocIniFileName);

#endif


#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
// ���� ��ų ������ �����Ѵ�.gDirPath.GetNewPath("MoveLevel.txt")
TMonsterSkillElement::LoadData( gDirPath.GetNewPath("MonsterSkillElement.txt") );
TMonsterSkillUnit::LoadData( gDirPath.GetNewPath("MonsterSkillUnit.txt") );
TMonsterSkillManager::LoadData( gDirPath.GetNewPath("MonsterSkill.txt") );
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

// ���� AI ������ �����Ѵ�.

TMonsterAIElement::LoadData( gDirPath.GetNewPath("MonsterAIElement.txt") );
TMonsterAIAutomata::LoadData( gDirPath.GetNewPath("MonsterAutomata.txt") );
TMonsterAIUnit::LoadData( gDirPath.GetNewPath("MonsterAIUnit.txt") );
TMonsterAIRule::LoadData( gDirPath.GetNewPath("MonsterAIRule.txt") );
TMonsterAIGroup::LoadData( gDirPath.GetNewPath("MonsterAIGroup.txt") );

#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725

#ifdef CRYWOLF_ONLY_PATCH_20050413

g_Crywolf.LoadData( gDirPath.GetNewPath("Crywolf.dat") );

#endif // CRYWOLF_ONLY_PATCH_20050413

#ifdef ADD_SKILL_ADDITION_INFO_CLASS_20060407
	g_SkillAdditionInfo.Load( "SkillAdditionInfo.dat" );
#endif
	
#ifdef ADD_KANTURU_20060627
	g_Kanturu.LoadData( gDirPath.GetNewPath( "Kanturu.dat" ) );
	g_KanturuMonsterMng.LoadData( gDirPath.GetNewPath( "KanturuMonsterSetBase.txt" ) );
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206	// GameMain -> LoadData()
	g_PCBangPointSystem.LoadData( gDirPath.GetNewPath( "PCBangPointItemOpt.txt" ) );	
#endif

#ifdef ADD_RAKLION_20080408
	g_Raklion.LoadData( gDirPath.GetNewPath( "Raklion.dat" ) );
#endif // ADD_RAKLION_20080408

	GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gCreateCharacter = atoi(szTemp);
	
	GetPrivateProfileString("GameServerInfo", "CharacterDeleteMinLevel", "40", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gCharacterDeleteMinLevel = atoi(szTemp);

#ifdef MUTE_EVENT_MESSAGE
	INT iMuteOption = GetPrivateProfileInt("GameServerInfo", "MuteServerMessage", 0, gDirPath.GetNewPath("commonserver.cfg"));	

	if( iMuteOption )
		g_bMuteServerMessage = true;
	else
		g_bMuteServerMessage = false;
#endif
	
#ifdef ADD_CONTROL_MAXUSER_20050420

#ifdef GAMESERVER_TO_CASTLESIEGESERVER
	INT iMaxUser = GetPrivateProfileInt("GameServerInfo", "NumberOfCastleSiegeMaxUser", MAX_USER, gDirPath.GetNewPath("commonserver.cfg"));	
#else
	INT iMaxUser = GetPrivateProfileInt("GameServerInfo", "NumberOfMaxUser", 500, gDirPath.GetNewPath("commonserver.cfg"));	
#endif

	if( iMaxUser>=0 && iMaxUser<=MAX_USER )
	{
		gServerMaxUser = iMaxUser;
		LogAddTD( "[Option Reload] �ڡڡ� MaxUser �١١�  : %d", gServerMaxUser );	
	}


#endif
	
#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706
	g_iServerGroupGuildChatting = GetPrivateProfileInt("GameServerInfo", "ServerGroupGuildChatting", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	g_iServerGroupUnionChatting = GetPrivateProfileInt("GameServerInfo", "ServerGroupUnionChatting", 0, gDirPath.GetNewPath("commonserver.cfg"));	
#endif

	// ��� ����
	GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gGuildCreate = atoi(szTemp);
	LogAdd(lMsg.Get(569), gGuildCreate);


	GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gGuildDestroy = atoi(szTemp);
	LogAdd(lMsg.Get(570), gGuildDestroy);	// "��� ���� �ɼ� %d"


	GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gGuildCreateLevel = atoi(szTemp);
	LogAdd(lMsg.Get(571), gGuildCreateLevel);	// ������ �ּ� ���� %d

#ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
	// ������� ��ü ����
	g_bCastleGuildDestoyLimit	= GetPrivateProfileInt( "GameServerInfo", "CastleOwnerGuildDestroyLimit", 1, gDirPath.GetNewPath("commonserver.cfg"));
#endif

	// ���ǵ��� ����ڸ� �� ��ų���ΰ�?
	GetPrivateProfileString("GameServerInfo", "SpeedHackPlayerBlock", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	SpeedHackPlayerBlock	= atoi(szTemp);

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	GetPrivateProfileString("GameServerInfo", "ConnectHackIPBlockTime", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gConnectHackIPBlockTime	= atoi(szTemp);
	
	GetPrivateProfileString("GameServerInfo", "ConnectIntervalTime", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gConnectIntervalTime	= atoi(szTemp);
	
	GetPrivateProfileString("GameServerInfo", "ConnectRenewBlockTime", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gConnectRenewBlockTime	= atoi(szTemp);
#endif	//MODIFY_CONNNECT_HACK_ATTACK_20090409

	GetPrivateProfileString("GameServerInfo", "ItemSerialCheck", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gItemSerialCheck = atoi(szTemp);
	
	GetPrivateProfileString("GameServerInfo", "AddExperience", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gAddExperience = (float)atof(szTemp);

#ifdef ADD_ZEN_OPTION_20070903	// commonserver���� �б�
	GetPrivateProfileString( "GameServerInfo", "AddZen", "1", szTemp, 5, gDirPath.GetNewPath( "commonserver.cfg" ) );
	gAddZen = ( float )atof( szTemp );
#endif

	// �ŷ������ Ȱ��ȭ ���� �����Ѵ�.
	GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	bCanTrade				= atoi(szTemp);
	
	if( bCanTrade ) 
		LogAddTD(lMsg.Get(407));
	else LogAddTD(lMsg.Get(408));


	bCanChaosBox = GetPrivateProfileInt("GameServerInfo", "ChaosBox", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gChaosEvent  = GetPrivateProfileInt("GameServerInfo", "ChaosEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	
	GetPrivateProfileString("GameServerInfo", "ChaosEventServer", "210.181.89.241", gChaosEventServerIp, 20, gDirPath.GetNewPath("commonserver.cfg"));

	LogAdd("[Option] ChaosBox = %d", bCanChaosBox);
	LogAdd("[Option] ChaosEvent = %d", gChaosEvent);
	LogAdd("[Option] ChaosEventServer = %s", gChaosEventServerIp);
	LogAdd("[Option] AttackEventRegenTime = %d", gAttackEventRegenTime);

	// ���θ� ȸ�� Tick ����(��) 1�̸� 1�ʾ� ���� 2�� 2�ʾ� ����
	GetPrivateProfileString("GameServerInfo", "PKTIME", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gPkTime					= atoi(szTemp);
	if( gPkTime )
		LogAddTD(lMsg.Get(409), gPkTime);

	GetPrivateProfileString("GameServerInfo", "MonsterHp", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gMonsterHp				= atoi(szTemp);
	if( gMonsterHp )
		LogAddTD(lMsg.Get(410), gMonsterHp);

	GetPrivateProfileString("GameServerInfo", "LootingTime", "3", szTemp, 5, szCommonlocIniFileName);	
	gLootingTime = atoi(szTemp);
	if( gLootingTime )
		LogAddTD("[Option] Auto Looting Time Set : %d", gLootingTime);

	GetPrivateProfileString("GameServerInfo", "PKItemDrop", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gPkItemDrop  = atoi(szTemp);
	
	if( gPkItemDrop )
		LogAddTD(lMsg.Get(411));
	else LogAddTD(lMsg.Get(412));

	GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gItemDropPer = atoi(szTemp);
	LogAddTD(lMsg.Get(413), gItemDropPer);

	// �� ��� �ð� ����
	gZenDurationTime = GetPrivateProfileInt("GameServerInfo", "ZenDurationTime", 30, gDirPath.GetNewPath("commonserver.cfg"));	
	gLootingTime = GetPrivateProfileInt("GameServerInfo", "ItemDurationTime", 30, gDirPath.GetNewPath("commonserver.cfg"));	

	// �̺�Ʈ Ĩ ������ ��´�
	gEventChipEvent = GetPrivateProfileInt("GameServerInfo", "EventChipEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gBoxOfGoldDropRate = GetPrivateProfileInt("GameServerInfo", "BoxOfGoldDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
	g_ItemDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo", "ItemDropRateForBoxOfGold", 2, gDirPath.GetNewPath("commonserver.cfg"));
	g_EventChipDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo", "EventChipDropRateForBoxOfGold", 2, gDirPath.GetNewPath("commonserver.cfg"));	

#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// ������ ���뼭�� �϶��� ����������, ����ĳ��, ī����ĳ��, ��������� �̺�Ʈ�� �����ȴ�.
	gDevilSquareEvent	= 0;
#endif

	if( !gDevilSquareEvent )
	{	// �Ǹ��� ���� �̺�Ʈ�� �ƴϸ�, ������ �ݴ´�
		g_DevilSquare.SetClose();
	}

	// ���� üũ�� ���� ���� ����
	gAttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo", "AttackSpeedTimeLimit", 800, gDirPath.GetNewPath("commonserver.cfg"));
	bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, gDirPath.GetNewPath("commonserver.cfg"));		
	gHackCheckCount = GetPrivateProfileInt("GameServerInfo", "HackCheckCount", 5.5, gDirPath.GetNewPath("commonserver.cfg"));	
	gMinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo", "MinimumAttackSpeedTime", 200, gDirPath.GetNewPath("commonserver.cfg"));	

	gDetectedHackKickCount = GetPrivateProfileInt("GameServerInfo", "DetectedHackKickCount", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gIsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickDetecHackCountLimit", 0, gDirPath.GetNewPath("commonserver.cfg"));	

#ifdef CHECK_ATTACK_AREA
	gNotAttackAreaKickCount = GetPrivateProfileInt("GameServerInfo", "NotAttackAreaKickCount", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	gIsKickNotAttackAreaCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickNotAttackAreaCountLimit", 0, gDirPath.GetNewPath("commonserver.cfg"));	
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
	gBlockHackUserCharacter = GetPrivateProfileInt("GameServerInfo", "BlockHackUserCharacter", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	gBlockHackUserAccount = GetPrivateProfileInt("GameServerInfo", "BlockHackUserAccount", 0, gDirPath.GetNewPath("commonserver.cfg"));	
#endif
	
#ifdef CHINA_HACKUSER_KICK_SYSTEM
	ChinaHackLogFile.Output("[HLS] AttackSpeedTimeLimit = %d", gAttackSpeedTimeLimit);
	ChinaHackLogFile.Output("[HLS] IsIgnorePacketHackDetect = %d", bIsIgnorePacketSpeedHackDetect);
	ChinaHackLogFile.Output("[HLS] HackCheckCount = %d", gHackCheckCount);
	ChinaHackLogFile.Output("[HLS] MinimumAttackSpeedTime = %d", gMinimumAttackSpeedTime);
	ChinaHackLogFile.Output("[HLS] DetectedHackKickCount = %d", gDetectedHackKickCount);
	ChinaHackLogFile.Output("[HLS] IsKickDetecHackCountLimit = %d", gIsKickDetecHackCountLimit);

	ChinaHackLogFile.Output("[HLS] NotAttackAreaKickCount = %d", gNotAttackAreaKickCount);
	ChinaHackLogFile.Output("[HLS] IsKickNotAttackAreaCountLimit = %d", gIsKickNotAttackAreaCountLimit);

	ChinaHackLogFile.Output("[HLS] BlockHackUserCharacter = %d", gBlockHackUserCharacter);
	ChinaHackLogFile.Output("[HLS] BlockHackUserAccount = %d", gBlockHackUserAccount);
#endif

	// Ÿ��¯ �̺�Ʈ
#ifdef FOR_JAPAN
	gTamaJJangEvent = GetPrivateProfileInt("GameServerInfo", "TamaJJangEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	gTamaJJangKeepTime = GetPrivateProfileInt("GameServerInfo", "TamaJJangKeepTime", 0, gDirPath.GetNewPath("commonserver.cfg"));	

	gTamaJJangDisappearTime = GetPrivateProfileInt("GameServerInfo", "TamaJJangDisappearTime", 1800, gDirPath.GetNewPath("commonserver.cfg"));	
	gTamaJJangDisappearTimeRandomRange = GetPrivateProfileInt("GameServerInfo", "TamaJJangDisappearTimeRandomRange", 1800, gDirPath.GetNewPath("commonserver.cfg"));	

	gTamaJJangTime = 10;
#endif	

#ifdef ITEMDROP_RINGOFTRANSFORM
	gIsItemDropRingOfTransform = GetPrivateProfileInt("GameServerInfo", "IsItemDropRingOfTransform", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	gItemDropRingOfTransform = GetPrivateProfileInt("GameServerInfo", "ItemDropRingOfTransform", 1, gDirPath.GetNewPath("commonserver.cfg"));	
#endif

	// ä�� �α׸� ����ϴ°�?
	gWriteChatLog = GetPrivateProfileInt("GameServerInfo", "WriteChatLog", 0, gDirPath.GetNewPath("commonserver.cfg"));

	gDisconnectHackUser = GetPrivateProfileInt("GameServerInfo", "DisconnectHackUser", 0, gDirPath.GetNewPath("commonserver.cfg"));

	gUseNPGGChecksum = GetPrivateProfileInt("GameServerInfo", "UseNPGGChecksum", 0, gDirPath.GetNewPath("commonserver.cfg"));

	

	GetPrivateProfileString("GameServerInfo", "DecTimePerAttackSpeed", "5.33", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gDecTimePerAttackSpeed = atof(szTemp);

	// �����ٿ� ���� �г�Ƽ ���� ��
	gSpeedHackPenalty = GetPrivateProfileInt("GameServerInfo", "SpeedHackPenalty", 0, gDirPath.GetNewPath("commonserver.cfg"));	


	// ���������� �ʴ�� ����Ȯ��
#ifdef DEVILSQUARE_EXTEND_20050221		// ī���� ���� �߰�
	gDQChaosSuccessRateLevel1 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel1", 75, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel2 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel2", 70, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel3 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel3", 65, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel4 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel4", 60, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel5 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel5", 55, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel6 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel6", 50, gDirPath.GetNewPath("commonserver.cfg"));
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// ����7 ī�������� Ȯ�� ���Ͽ��� �б�
	gDQChaosSuccessRateLevel7 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel7", 45, gDirPath.GetNewPath("commonserver.cfg"));
#endif
#else
	gDQChaosSuccessRateLevel1 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel1", 80, gDirPath.GetNewPath("commonserver.cfg"));	
	gDQChaosSuccessRateLevel2 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel2", 75, gDirPath.GetNewPath("commonserver.cfg"));	
	gDQChaosSuccessRateLevel3 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel3", 70, gDirPath.GetNewPath("commonserver.cfg"));	
	gDQChaosSuccessRateLevel4 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel4", 60, gDirPath.GetNewPath("commonserver.cfg"));
#endif

	gWriteSkillLog = GetPrivateProfileInt("GameServerInfo", "WriteSkillLog", 0, gDirPath.GetNewPath("commonserver.cfg"));

	gQuestNPCTeleportTime = GetPrivateProfileInt("GameServerInfo", "QuestNPCTeleportTime", 900, gDirPath.GetNewPath("commonserver.cfg"));

#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531
	g_iJapan1StAnivItemDropRate = GetPrivateProfileInt("GameServerInfo", "Japan1StAnivItemDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214						// ������ ǥ�� ���Ȯ��
	g_iMarkOfTheLord			= GetPrivateProfileInt("GameServerInfo", "MarkOfTheLord", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif
	
#ifndef EVENT_OPTION_RELOAD_20040305
	// 1 . ���� ������
	g_DevilSquare.Load(gDirPath.GetNewPath("DevilSquare.dat"));
	gDevilSquareEvent			= GetPrivateProfileInt("GameServerInfo", "DevilSquareEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gEyesOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "EyesOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
	gKeyOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "KeyOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));	   
	
	// 2 . ����ĳ��
#ifdef FOR_BLOODCASTLE
	g_BloodCastle.Load(gDirPath.GetNewPath("BloodCastle.dat"));
	g_bBloodCastle				= GetPrivateProfileInt("GameServerInfo", "BloodCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBloodCastle_StartHour	= GetPrivateProfileInt("GameServerInfo", "BloodCastleStartHour", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_iAngelKingsPaperDropRate	= GetPrivateProfileInt("GameServerInfo", "AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBloodBoneDropRate		= GetPrivateProfileInt("GameServerInfo", "BloodBoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iStoneDropRate			= GetPrivateProfileInt("GameServerInfo", "StoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

#ifdef UPDATE_BLOODECASTLE_SCADULE_20070702
			g_iBloodCastle_odd_Even_Hour= GetPrivateProfileInt("GameServerInfo", "BloodCastleOddEvenHour", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#endif

#ifdef DARKLORD_WORK
	gIsDropDarkLordItem	= GetPrivateProfileInt("GameServerInfo", "IsDropDarkLordItem", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gSleeveOfLordDropRate	= GetPrivateProfileInt("GameServerInfo", "SleeveOfLordDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
	gSleeveOfLordDropLevel	= GetPrivateProfileInt("GameServerInfo", "SleeveOfLordDropLevel", 86, gDirPath.GetNewPath("commonserver.cfg"));
	gSoulOfDarkHorseDropRate	= GetPrivateProfileInt("GameServerInfo", "SoulOfDarkHorseDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
	gSoulOfDarkHorseropLevel	= GetPrivateProfileInt("GameServerInfo", "SoulOfDarkHorseropLevel", 125, gDirPath.GetNewPath("commonserver.cfg"));g_iAngelKingsPaperDropRate	= GetPrivateProfileInt("GameServerInfo", "AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gSoulOfDarkSpiritDropRate	= GetPrivateProfileInt("GameServerInfo", "SoulOfDarkSpiritDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
	gSoulOfDarkSpiritDropLevel	= GetPrivateProfileInt("GameServerInfo", "SoulOfDarkSpiritDropLevel", 96, gDirPath.GetNewPath("commonserver.cfg"));	

	GetPrivateProfileString("GameServerInfo", "DarkSpiritAddExperience", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gDarkSpiritAddExperience = (float)atof(szTemp);
#endif

#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
	// ��ȣ���� �������
	gIsDropGemOfDefend  = GetPrivateProfileInt("GameServerInfo", "IsDropGemOfDefend", 0, gDirPath.GetNewPath("commonserver.cfg"));
	// ��ȣ���� ��� Ȯ�� (10000 ����)
	gGemOfDefendDropRate = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropRate", 5, gDirPath.GetNewPath("commonserver.cfg"));
	// ��ȣ���� ��� ���� ����
	gGemOfDefendDropLevel = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));;		
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314
	// �÷��� ������ ��Ʈ������ �������
	gIsDropSetItemInCastleHuntZone  = GetPrivateProfileInt("GameServerInfo", "IsDropSetItemInCastleHuntZone", 0, gDirPath.GetNewPath("commonserver.cfg"));
	// �÷��� ������ ��Ʈ������ ���Ȯ�� (10000 ����)
	gSetItemInCastleHuntZoneDropRate = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropRate", 1, gDirPath.GetNewPath("commonserver.cfg"));
	// �÷��� ������ ��Ʈ������ ��� ���� ����
	gSetItemInCastleHuntZoneDropLevel = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));;		
#endif

#ifdef FOR_NEW_TESTSERVER
	gTestServer	= GetPrivateProfileInt("GameServerInfo", "TestServer", 0, gDirPath.GetNewPath("commonserver.cfg"));	
#endif

	// 3 . ���� ����
	gAttackEventRegenTime = GetPrivateProfileInt("GameServerInfo", "AttackEventRegenTime", 10, gDirPath.GetNewPath("commonserver.cfg"));

	// 4 . Ȳ�� ����
	gIsEledoradoEvent				= GetPrivateProfileInt("GameServerInfo", "IsEledoradoEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoGoldGoblenRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldGoblenRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoTitanRegenTime		= GetPrivateProfileInt("GameServerInfo", "EledoradoTitanRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoGoldDerconRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldDerconRegenTime", 240*3, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoDevilLizardKingRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilLizardKingRegenTime", 120*3, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoDevilTantarosRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilTantarosRegenTime", 120*3, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoGoldGoblenItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldGoblenItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoTitanItemDropRate			= GetPrivateProfileInt("GameServerInfo", "EledoradoTitanItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoGoldDerconItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldDerconItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoDevilLizardKingItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilLizardKingItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoDevilTantarosItemDropRate		= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilTantarosItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoGoldGoblenExItemDropRate		= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldGoblenExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoTitanExItemDropRate			= GetPrivateProfileInt("GameServerInfo", "EledoradoTitanExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoGoldDerconExItemDropRate		= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldDerconExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoDevilLizardKingExItemDropRate = GetPrivateProfileInt("GameServerInfo", "EledoradoDevilLizardKingExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
	gEledoradoDevilTantarosExItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilTantarosExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
	gGoldenRabbitRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenRabbitRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
	gGoldenDarknightRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenDarkKnightRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
	gGoldenDevilRegenTime  = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenDevilRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
	
	gGoldenStoneMonsterRegenTIme = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenMonsterRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldenCrustEventRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenCrustRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldenSatirosRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenSatirosRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldenTwintailRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenTwintailRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldenIronKnightRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenIronKnightRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldenNeipinRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenNeipinRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldenGreatDragonRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenGreatDragonRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
#endif

	// 5 . ��������� ����
#ifdef MONSTER_HERD_SYSTEM_20031120
	// ���� �̺�Ʈ�� ���� ���θ� ���´�.
	GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	g_bDoRingEvent				= atoi(szTemp);
	// ��ũ�� ����� �� ��ǰ�� Ż Ȯ�� (1/n)
	GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRingOrcKillGiftRate		= atoi(szTemp);
	// ������ ������ �� ��ǰ�� Ż Ȯ�� (1/n)
	GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRingDropGiftRate			= atoi(szTemp);
	g_RingAttackEvent.Load(gDirPath.GetNewPath("RingAttackEvent.dat"));
#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// ������ ���뼭�� �϶��� ����������, ����ĳ��, ī����ĳ��, ��������� �̺�Ʈ�� �����ȴ�.
	g_bDoRingEvent				= FALSE;
#endif	
	g_RingAttackEvent.EnableEvent (g_bDoRingEvent);
#endif

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
	g_bDoXMasEvent				= GetPrivateProfileInt("GameServerInfo", "XMasAttackEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_nXMasDropRate				= GetPrivateProfileInt("GameServerInfo", "XMasAttackEvent_DropRate", XMASDROPRATE_ITEM, gDirPath.GetNewPath("commonserver.cfg"));
	g_nXMasDropZen				= GetPrivateProfileInt("GameServerInfo", "XMasAttackEvent_DropZen", XMASDROP_ZEN, gDirPath.GetNewPath("commonserver.cfg"));
	g_XMasAttackEvent.Load(gDirPath.GetNewPath("XMasAttackEvent.dat"));
	g_XMasAttackEvent.EnableEvent(g_bDoXMasEvent)
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#ifdef ADD_XMASEVENT_LUCKNUMBER
	g_nXMasLuckNumber1st		= GetPrivateProfileInt("GameServerInfo", "XMasEvent_LuckNumber1st", XMASEVENT_LUCKNUMBER1ST, gDirPath.GetNewPath("commonserver.cfg"));
	g_nXMasLuckNumber2nd		= GetPrivateProfileInt("GameServerInfo", "XMasEvent_LuckNumber2nd", XMASEVENT_LUCKNUMBER2ND, gDirPath.GetNewPath("commonserver.cfg"));
#endif // ADD_XMASEVENT_LUCKNUMBER

#ifdef CASTLEDEEP_EVENT_20050413
	GetPrivateProfileString("GameServerInfo", "CastleDeepEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	g_bDoCastleDeepEvent		= atoi(szTemp);
	g_CastleDeepEvent.Load(gDirPath.GetNewPath("CastleDeepEvent.dat"));
	g_CastleDeepEvent.EnableEvent(g_bDoCastleDeepEvent);
#endif	// #ifdef CASTLEDEEP_EVENT_20050413

	// 6 . �������� (���귯��) �̺�Ʈ

	// # REMOVE_LOVELOVE_EVENT_COUPLING_WITH_JOINSERVER_20060319
	// # ���� ������ �����ؼ� �̺�Ʈ ó���� �� ������ ����(apple)
	//		- ���� ������ ��ȯ�ϴ� 0x30, 0x31 �������� ����
	//		- ���� ������ ������Ʈ ���� DB �� ����.
	//		- ���� �������� ������Ʈ �ڵ� ���� �� ����.
	GetPrivateProfileString("GameServerInfo", "EVENT1", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gEvent1						= atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayMax", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gEvent1ItemDropTodayCount	= 0;
	gEvent1ItemDropTodayMax		= atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayPercent", "10000000", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gEvent1ItemDropTodayPercent = atoi(szTemp);

	// 7 . ���� �̺�Ʈ
	gFireCrackerEvent			= GetPrivateProfileInt("GameServerInfo", "FireCrackerEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gFireCrackerDropRate		= GetPrivateProfileInt("GameServerInfo", "FireCrackerDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
	g_ItemDropRateForgFireCracker	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForFireCracker", 2, gDirPath.GetNewPath("commonserver.cfg"));
	gOnlyFireCrackerEffectUse	= GetPrivateProfileInt("GameServerInfo", "OnlyFireCrackerEffectUse", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// 8 . �޴� �̺�Ʈ
	gMedalEvent					= GetPrivateProfileInt("GameServerInfo", "MedalEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldMedalDropRate			= GetPrivateProfileInt("GameServerInfo", "GoldMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));						// �ݸ޴��� ������ Ȯ��
	gSilverMedalDropRate		= GetPrivateProfileInt("GameServerInfo", "SilverMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));					// ���޴��� ������ Ȯ��
	g_ItemDropRateForGoldMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForGoldMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));		// �ݸ޴޿��� �������� ������ Ȯ��
	g_ItemDropRateForSilverMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForSilverMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));	// ���޴޿��� �������� ������ Ȯ��

	// 9 . ��ź�Ǻ� �̺�Ʈ
	gXMasEvent					= GetPrivateProfileInt("GameServerInfo", "XMasEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_XMasEvent_StarOfXMasDropRate	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_StarOfXMasDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
	g_XMasEvent_ItemDropRateForStarOfXMas	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_ItemDropRateForStarOfXMas", 2, gDirPath.GetNewPath("commonserver.cfg"));

	// 10 . �������Ʈ �̺�Ʈ
	gHeartOfLoveEvent			= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gHeartOfLoveDropRate		= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
	g_ItemDropRateForgHeartOfLove	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForHeartOfLove", 2, gDirPath.GetNewPath("commonserver.cfg"));

	// 11 . NPC�� �����λ�
	GetPrivateProfileString("GameServerInfo", "HappyNewYearTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gHappyNewYearNpcEvent		= atoi(szTemp);
	if( gHappyNewYearNpcEvent )	LogAddTD(lMsg.Get(573));	// [�ɼ�] ���� �λ� NPC ��ȭ ���

	// 12 . NPC�� �޸�ũ��������
	GetPrivateProfileString("GameServerInfo", "MerryXMasTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gMerryXMasNpcEvent			= atoi(szTemp);
	if( gMerryXMasNpcEvent )	LogAddTD(lMsg.Get(572));	// [�ɼ�] ũ�������� NPC ��ȭ ���

#ifdef CHAOSCASTLE_SYSTEM_20040408 
	// 13 . ī���� ĳ��
	g_ChaosCastle.Load(gDirPath.GetNewPath("ChaosCastle.dat"));
	g_bChaosCastle				= GetPrivateProfileInt("GameServerInfo", "ChaosCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518							// ����� ǥ�� ���Ȯ���� �д´�.
	// 14 . Į���� - �����ǥ��
	g_iKundunMarkDropRate= GetPrivateProfileInt("GameServerInfo", "KundunMarkDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804					// �ź��� ���� ��� Ȯ��
	// 15 . �ź��� ���� ��ӷ�
	g_iMysteriousBeadDropRate1 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate1", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iMysteriousBeadDropRate2 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate2", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iHiddenTreasureBoxOfflineRate = GetPrivateProfileInt("GameServerInfo", "HiddenTreasureBoxOfflineRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef EVENT_MANAGEMENT_20040711
	// 16 . �̺�Ʈ �Ŵ��� ���� ����
	GetPrivateProfileString("GameServerInfo", "EventManagerOn", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	g_bEventManagerOn			= atoi(szTemp);

	g_EventManager.Load(gDirPath.GetNewPath("EventManagement.dat"));

	g_EventManager.RegEvent (CEventManagement::EVENT_ID_DRAGONHERD,		(LPVOID) DragonEvent);
	g_EventManager.RegEvent (CEventManagement::EVENT_ID_UNDERTROOP,		(LPVOID) AttackEvent);
	g_EventManager.RegEvent (CEventManagement::EVENT_ID_ELDORADO,		(LPVOID) &gEledoradoEvent);
	g_EventManager.RegEvent (CEventManagement::EVENT_ID_WHITEMAGE,		(LPVOID) &g_RingAttackEvent);
#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
	g_EventManager.RegEvent (CEventManagement::EVENT_ID_SANTACLAUS,		(LPVOID) &g_XMasAttackEvent);
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

	g_EventManager.Init(g_bEventManagerOn);
#endif

#ifdef HAPPY_POUCH
	// apple��20050204
	// 17 . �Ķ�, ���� ���ָӴ� �̺�Ʈ
	gHappyPouchRedDropRate	= GetPrivateProfileInt("GameServerInfo", "HappyPouchRedDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));							
	gHappyPouchBlueDropRate	= GetPrivateProfileInt("GameServerInfo", "HappyPouchBlueDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));							
#endif

	// 18. ��ũ�ε��� ���� �̺�Ʈ
#ifdef DARK_LORD_HEART_EVENT_20040810						// ��ũ�ε��� ���� �̺�Ʈ ���� Ȯ���� �д´�.
	g_iDarkLordHeartDropRate		= GetPrivateProfileInt("GameServerInfo", "DarkLordHeartDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iDarkLordHeartOffEventRate	= GetPrivateProfileInt("GameServerInfo", "DarkLoadHeartOffEventRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#endif

	// 19. �߱� ���� �̺�Ʈ
#ifdef MU_CHN_OFFLINE_GIFT_EVENT_20040908
	gbChnOfflineGiftEvent			= GetPrivateProfileInt("GameServerInfo", "ChnOfflineGiftEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	giChnOfflineGiftEventDropRate	= GetPrivateProfileInt("GameServerInfo", "ChnOfflineGiftEventDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

	// 20. ũ�������� �������� �̺�Ʈ( RKFKA )
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
	g_bRibbonBoxEvent					= GetPrivateProfileInt( "GameServerInfo", "RibbonBoxEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iRedRibbonBoxDropLevelMin			= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedRibbonBoxDropLevelMax			= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	g_iRedRibbonBoxDropRate				= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedRibbonBoxDropZenRate			= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedRibbonBoxDropZen				= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iGreenRibbonBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iGreenRibbonBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iGreenRibbonBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iGreenRibbonBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iGreenRibbonBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iBlueRibbonBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));		
	g_iBlueRibbonBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueRibbonBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueRibbonBoxDropZenRate			= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueRibbonBoxDropZen				= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif //#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212

	// 21. �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ( RKFKA )
#ifdef VALENTINES_DAY_BOX_EVENT_20060124
	g_bChocolateBoxEvent				= GetPrivateProfileInt( "GameServerInfo", "ChocolateEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iPinkChocolateBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iPinkChocolateBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	g_iPinkChocolateBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iPinkChocolateBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iPinkChocolateBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iRedChocolateBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedChocolateBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedChocolateBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedChocolateBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedChocolateBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iBlueChocolateBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));		
	g_iBlueChocolateBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueChocolateBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueChocolateBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueChocolateBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif //#ifdef VALENTINES_DAY_BOX_EVENT_20060124

	// 22. ȭ��Ʈ���� �������� �̺�Ʈ( RKFKA )
#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
	g_bCandyBoxEvent					= GetPrivateProfileInt( "GameServerInfo", "CandyBoxEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iLightPurpleCandyBoxDropLevelMin	= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iLightPurpleCandyBoxDropLevelMax	= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	g_iLightPurpleCandyBoxDropRate		= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iLightPurpleCandyBoxDropZenRate	= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iLightPurpleCandyBoxDropZen		= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iVermilionCandyBoxDropLevelMin	= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iVermilionCandyBoxDropLevelMax	= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iVermilionCandyBoxDropRate		= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iVermilionCandyBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iVermilionCandyBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iDeepBlueCandyBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));		
	g_iDeepBlueCandyBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iDeepBlueCandyBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iDeepBlueCandyBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iDeepBlueCandyBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif //#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
	
#ifdef ADD_KUNDUN_CONTROL_20040923
	giKundunRefillHPSec				= GetPrivateProfileInt("GameServerInfo", "KundunRefillHPSec", 500, gDirPath.GetNewPath("commonserver.cfg"));							
	giKundunRefillHP				= GetPrivateProfileInt("GameServerInfo", "KundunRefillHP", 10000, gDirPath.GetNewPath("commonserver.cfg"));								
	giKundunRefillHPTime			= GetPrivateProfileInt("GameServerInfo", "KundunRefillHPTime", 600, gDirPath.GetNewPath("commonserver.cfg"));								
#endif

#ifdef EXTEND_LOG_SYSTEM_20060202
	giKundunHPLogSaveTime			= GetPrivateProfileInt("GameServerInfo", "KundunHPLogSaveTime", 120, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
	g_bFenrirStuffItemDrop			= GetPrivateProfileInt("GameServerInfo", "FenrirStuffItemDrop", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iFenrirStuff_01_DropLv_Min	= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_01_DropLv_Min",	0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirStuff_01_DropLv_Max	= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_01_DropLv_Max",	0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirStuff_01_DropMap		= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_01_DropMap",		0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirStuff_01_DropRate		= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_01_DropRate",		0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iFenrirStuff_02_DropLv_Min	= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_02_DropLv_Min",	0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirStuff_02_DropLv_Max	= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_02_DropLv_Max",	0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirStuff_02_DropMap		= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_02_DropMap",		0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirStuff_02_DropRate		= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_02_DropRate",		0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iFenrirStuff_03_DropLv_Min	= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_03_DropLv_Min",	0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirStuff_03_DropLv_Max	= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_03_DropLv_Max",	0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirStuff_03_DropMap		= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_03_DropMap",		0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirStuff_03_DropRate		= GetPrivateProfileInt("GameServerInfo", "FenrirStuff_03_DropRate",		0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iFenrirRepairRate				= GetPrivateProfileInt("GameServerInfo", "FenrirRepairRate", 4000, gDirPath.GetNewPath("commonserver.cfg"));
	
	g_iFenrirDefaultMaxDurSmall		= GetPrivateProfileInt("GameServerInfo", "FenrirDefaultMaxDurSmall", 200, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrirElfMaxDurSmall			= GetPrivateProfileInt("GameServerInfo", "FenrirElfMaxDurSmall", 160, gDirPath.GetNewPath("commonserver.cfg"));

	g_iFenrir_01Level_MixRate		= GetPrivateProfileInt("GameServerInfo", "Fenrir_01Level_MixRate", 70, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrir_02Level_MixRate		= GetPrivateProfileInt("GameServerInfo", "Fenrir_02Level_MixRate", 50, gDirPath.GetNewPath("commonserver.cfg"));
	g_iFenrir_03Level_MixRate		= GetPrivateProfileInt("GameServerInfo", "Fenrir_03Level_MixRate", 30, gDirPath.GetNewPath("commonserver.cfg"));
#endif // ADD_ITEM_FENRIR_01_20051110

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
	g_bCrywolfMonsterDarkElfItemDrop		= GetPrivateProfileInt( "GameServerInfo", "CrywolfMonsterDarkElfItemDrop", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iCrywolfMonsterDarkElfItemDropRate	= GetPrivateProfileInt( "GameServerInfo", "CrywolfMonsterDarkElfItemDropRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iCrywolfMonsterDarkElfDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "CrywolfMonsterDarkElfDropZenRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iCrywolfMonsterDarkElfDropZen			= GetPrivateProfileInt( "GameServerInfo", "CrywolfMonsterDarkElfDropZen", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
	g_bCrywolfBossMonsterItemDrop			= GetPrivateProfileInt( "GameServerInfo", "CrywolfBossMonsterItemDrop", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iCrywolfBossMonsterItemDropRate		= GetPrivateProfileInt( "GameServerInfo", "CrywolfBossMonsterItemDropRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iCrywolfBossMonsterDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "CrywolfBossMonsterDropZenRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iCrywolfBossMonsterDropZen			= GetPrivateProfileInt( "GameServerInfo", "CrywolfBossMonsterDropZen", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif

#ifdef MU_CRYWOLF_BENEFIT_20051215
	g_iCrywolfApplyMvpBenefit 			= GetPrivateProfileInt("GameServerInfo", "CrywolfApplyMvpBenefit",			0,		gDirPath.GetNewPath("commonserver.cfg"));
	
	int	iCrywolfPlusChaosRateBenefit	= GetPrivateProfileInt("GameServerInfo", "CrywolfPlusChaosRateBenefit",		0,		gDirPath.GetNewPath("commonserver.cfg"));
	int iCrywolfMonHPRateBenefit 		= GetPrivateProfileInt("GameServerInfo", "CrywolfMonHPRateBenefit",			100,	gDirPath.GetNewPath("commonserver.cfg"));

#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
	int iCrywolfKundunHPRefillState		= GetPrivateProfileInt("GameServerInfo", "CrywolfKundunHPRefillBenefit",	1,		gDirPath.GetNewPath("commonserver.cfg"));
	g_CrywolfSync.SetKundunHPRefillState( iCrywolfKundunHPRefillState );
#endif // MODIFY_CRYWOLF_BENEFIT_01_20060315

	g_CrywolfSync.SetPlusChaosRate( iCrywolfPlusChaosRateBenefit );
	g_CrywolfSync.SetMonHPBenefitRate( iCrywolfMonHPRateBenefit );

#endif // MU_CRYWOLF_BENEFIT_20051215

#ifdef MU_CRYWOLF_PENALTY_20051215
	g_iCrywolfApplyMvpPenalty 			= GetPrivateProfileInt("GameServerInfo", "CrywolfApplyMvpPenalty",			0,		gDirPath.GetNewPath("commonserver.cfg"));

	int	iCrwyolfGemDropPenaltyRate 		= GetPrivateProfileInt("GameServerInfo", "CrwyolfGemDropPenaltyRate",		100,	gDirPath.GetNewPath("commonserver.cfg"));
	int	iCrwyolfGettingExpPenaltyRate	= GetPrivateProfileInt("GameServerInfo", "CrwyolfGettingExpPenaltyRate",	100,	gDirPath.GetNewPath("commonserver.cfg"));

	g_CrywolfSync.SetGemDropPenaltiyRate( iCrwyolfGemDropPenaltyRate );
	g_CrywolfSync.SetGettingExpPenaltyRate( iCrwyolfGettingExpPenaltyRate );

#endif // MU_CRYWOLF_PENALTY_20051215

#ifdef ADD_KANTURU_20060627
	int iKanturuEnableValue 			= GetPrivateProfileInt( "GameServerInfo", "KanturuEvent", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_Kanturu.SetKanturuEnable( iKanturuEnableValue );
#endif
	
#ifdef MODIFY_KANTURU_MAPNUMBER_CHECK_20060807
	g_iBlockKanturuMapEnter		= GetPrivateProfileInt("GameServerInfo", "BlockKanturuMapEnter",		0,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlockCastleSiegeMapEnter	= GetPrivateProfileInt("GameServerInfo", "BlockCastleSiegeMapEnter",	0,		gDirPath.GetNewPath("commonserver.cfg"));
#endif // MODIFY_KANTURU_MAPNUMBER_CHECK_20060807

#ifdef KANTURU_MONSTER_ITEMDROP_20060807
	g_bKanturuMayaHandItemDrop			= GetPrivateProfileInt( "GameServerInfo", "KanturuMayaHandItemDrop", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iKanturuMayaHandItemDropRate		= GetPrivateProfileInt( "GameServerInfo", "KanturuMayaHandItemDropRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iKanturuMayaHandDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "KanturuMayaHandDropZenRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iKanturuMayaHandDropZen			= GetPrivateProfileInt( "GameServerInfo", "KanturuMayaHandDropZen", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );

	g_bKanturuNightmareItemDrop			= GetPrivateProfileInt( "GameServerInfo", "KanturuNightmareItemDrop", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iKanturuNightmareItemDropRate		= GetPrivateProfileInt( "GameServerInfo", "KanturuNightmareItemDropRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iKanturuNightmareDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "KanturuNightmareDropZenRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iKanturuNightmareDropZen			= GetPrivateProfileInt( "GameServerInfo", "KanturuNightmareDropZen", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif	// #ifdef KANTURU_MONSTER_ITEMDROP_20060807

#ifdef KANTURU_SPECIAL_ITEMDROP_20060808
	g_bKanturuSpecialItemDropOn			= GetPrivateProfileInt( "GameServerInfo", "KanturuSpecialItemDropOn", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iKanturuMoonStoneDropRate			= GetPrivateProfileInt( "GameServerInfo", "KanturuMoonStoneDropRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iKanturuJewelOfHarmonyDropRate	= GetPrivateProfileInt( "GameServerInfo", "KanturuJewelOfHarmonyDropRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif

#ifdef ADD_GAMBLING_20090120
	g_bGamblingSystem					= GetPrivateProfileInt( "GameServerInfo", "GamblingSystem", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iGamblingValue					= GetPrivateProfileInt( "GameServerInfo", "GamblingValue", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iGamblingShopTime					= GetPrivateProfileInt( "GameServerInfo", "GamblingShopTime", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif // ADD_GAMBLING_20090120

#ifdef ADD_RAKLION_20080408
	int iRaklionEnableValue 			= GetPrivateProfileInt( "GameServerInfo", "RaklionEvent", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_Raklion.SetRaklionBossEnable( iRaklionEnableValue );
	
	g_bRaklionSelupanItemDrop			= GetPrivateProfileInt( "GameServerInfo", "RaklionSelupanItemDrop", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iRaklionSelupanItemDropRate		= GetPrivateProfileInt( "GameServerInfo", "RaklionSelupanItemDropRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iRaklionSelupanDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "RaklionSelupanDropZenRate", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iRaklionSelupanDropZen			= GetPrivateProfileInt( "GameServerInfo", "RaklionSelupanDropZen", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
	
	g_bBlockRaklionMapEnter				= GetPrivateProfileInt( "GameServerInfo", "BlockRaklionMapEnter", 0, gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif // ADD_RAKLION_20080408

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
	g_bHallowinDayEventOn						= GetPrivateProfileInt( "GameServerInfo", "HallowinEventOn",						0,	gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iHallowinDayEventItemDropRate				= GetPrivateProfileInt( "GameServerInfo", "HallowinEventPumpkinOfLuckDropRate",		100,gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iHallowinDayEventJOLBlessDropRate			= GetPrivateProfileInt( "GameServerInfo", "HallowinEventJOLBlessDropRate",			10,	gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iHallowinDayEventJOLAngerDropRaTe			= GetPrivateProfileInt( "GameServerInfo", "HallowinEventJOLAngerDropRate",			15,	gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iHallowinDayEventJOLScreamDropRate		= GetPrivateProfileInt( "GameServerInfo", "HallowinEventJOLScreamDropRate",			15,	gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iHallowinDayEventJOLFoodDropRate			= GetPrivateProfileInt( "GameServerInfo", "HallowinEventJOLFoodDropRate",			30,	gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iHallowinDayEventJOLDrinkDropRate			= GetPrivateProfileInt( "GameServerInfo", "HallowinEventJOLDrinkDropRate",			25,	gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iHallowinDayEventJOLPolymorphRingDropRate	= GetPrivateProfileInt( "GameServerInfo", "HallowinEventJOLPolymorphRingDropRate",	5,	gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif //HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	g_bSantaPolymorphRingDropOn					= GetPrivateProfileInt( "GameServerInfo", "SantaPolymorphRingDropOn",				0,	gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iSantaPolymorphRingDropRate				= GetPrivateProfileInt( "GameServerInfo", "SantaPolymorphRingDropRate",				10,	gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131
	g_bNewYearLuckyBagMonsterEventOn			= GetPrivateProfileInt( "GameServerInfo", "NewYearLuckyBagMonsterEventOn",		0,	gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��� �̺�Ʈ������ �д´�
	g_IllusionTempleEvent.LoadIllusionTempleEventInfo();
#endif

#ifdef ADD_THIRD_WING_20070525	// �ܵ����ǺҲ� ���� �б�
 	g_iCondorFlameDropRate		= GetPrivateProfileInt( "GameServerInfo", "CondorFlameDropRate", 10, gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	g_bCherryBlossomEventOn						= GetPrivateProfileInt( "GameServerInfo", "CherryBlossomEventOn",					1,		gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_bCherryBlossomEventItemDropRate			= GetPrivateProfileInt( "GameServerInfo", "CherryBlossomEventItemDropRate",			8000,	gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029							// ����� ���� ��� Ȯ��
	g_bLuckyCoinEventOn		= GetPrivateProfileInt( "GameServerInfo", "LuckyCoinEventOn",			0,		gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iLuckyCoinDropRate	= GetPrivateProfileInt( "GameServerInfo", "LuckyCoinDropRate",			1000,	gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// �����ͷ��� ���� �б�
	g_MasterLevelSystem.LoadData();
#endif

#ifdef ADD_NEWPVP_PKFIELD
	g_NewPVP.LoadData();
#endif // ADD_NEWPVP_PKFIELD

#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
	gML_OldScrollDropRate			= GetPrivateProfileInt( "GameServerInfo", "ML_OldScrollDropRate",			0,	gDirPath.GetNewPath( "commonserver.cfg" ) );			
	gML_CovenantOfIllusionDropRate	= GetPrivateProfileInt( "GameServerInfo", "ML_CovenantOfIllusionDropRate",	0,	gDirPath.GetNewPath( "commonserver.cfg" ) );	
	gML_AngelKingsPaperDropRate		= GetPrivateProfileInt( "GameServerInfo", "ML_AngelKingsPaperDropRate",		0,	gDirPath.GetNewPath( "commonserver.cfg" ) );	
	gML_BloodBoneDropRate			= GetPrivateProfileInt( "GameServerInfo", "ML_BloodBoneDropRate",			0,	gDirPath.GetNewPath( "commonserver.cfg" ) );			
	gML_EyesOfDevilSquareDropRate	= GetPrivateProfileInt( "GameServerInfo", "ML_EyesOfDevilSquareDropRate ",	0,	gDirPath.GetNewPath( "commonserver.cfg" ) );	
	gML_KeyOfDevilSquareDropRate	= GetPrivateProfileInt( "GameServerInfo", "ML_KeyOfDevilSquareDropRate ",	0,	gDirPath.GetNewPath( "commonserver.cfg" ) );	
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123

#ifdef UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220
	g_ShadowPahtomMaxLevel			= GetPrivateProfileInt( "GameServerInfo", "ShadowPhantomMaxLevel",		220,	gDirPath.GetNewPath( "commonserver.cfg" ) );	
#endif	// UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220

	// ���Ӽ��� ������ ���
	GetPrivateProfileString("ConnectServerInfo", "IP", "", connectserverip, 20, szCommonlocIniFileName);
	GetPrivateProfileString("ConnectServerInfo", "PORT", "", szTemp, 10, szCommonlocIniFileName);
	connectserverport = atoi(szTemp);
	LogAddTD(lMsg.Get(414), connectserverip, connectserverport);

#ifdef GAMESERVER_INFO_TCP_20031203
	GetPrivateProfileString("ConnectServerInfo", "PORT_TCP", "", szTemp, 10, szCommonlocIniFileName);
	connectserverport_tcp = atoi(szTemp);

	// ���Ӽ����� ������ ��� ���� ���ΰ� ���� 
	GetPrivateProfileString("ConnectServerInfo", "CONNTYPE", "0", szTemp, 10, szCommonlocIniFileName);
	g_ConServerInfoSendType = atoi(szTemp);

	if (g_ConServerInfoSendType!= 0 &&
		g_ConServerInfoSendType!= 1
		)
	{
		// ���� ����� UDP / TCP �� �� �ϳ��� �ƴϸ� �⺻�� UDP
		g_ConServerInfoSendType = 0;
	}
#endif

	gUdpSoc.SendSet(connectserverip, connectserverport);
	gUdpSocCE.SendSet(gChaosEventServerIp, 60005);

#ifdef GAMESERVER_INFO_TO_CSS_20040317
	// �л��� �� ������ üũ�Ѵ�.
	g_bSendGSInfoToCSS  = GetPrivateProfileInt("GameServerInfo", "GSInfoSendToCSS", 0, gDirPath.GetNewPath("commonserver.cfg"));
	if (g_bSendGSInfoToCSS) {
		// ���� �л��� �Ѵٸ� ���� �����Ѵ�.
		GSINFO_INITCSLIST();
	}
#endif	

	DevilSquareEventConnect = GetPrivateProfileInt("GameServerInfo", "DevilSquareEventConnect", 1, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "DevilSquareEventServer", "210.181.89.241", gDevilSquareEventServerIp, 20, gDirPath.GetNewPath("commonserver.cfg"));

	EventChipServerConnect = GetPrivateProfileInt("GameServerInfo", "EventChipServerConnect", 0, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "EventChipServerIp", "192.168.10.150", gEventChipServerIp, 20, gDirPath.GetNewPath("commonserver.cfg"));

	// ������� ������ ���� ���� �ý����� �����ϴ°�?
	gApplyHeroSystem = GetPrivateProfileInt("GameServerInfo", "ApplyHeroSystem", 0, gDirPath.GetNewPath("commonserver.cfg"));	

	// ��Ʋ��Ŀ�� �Ҽ� �ִ°�?
	gEnableBattleSoccer = GetPrivateProfileInt("GameServerInfo", "EnableBattleSoccer", 1, gDirPath.GetNewPath("commonserver.cfg"));	

	// �̺�Ʈ���� NPC���� ���� �ϴ°�?
	gEnableEventNPCTalk = GetPrivateProfileInt("GameServerInfo", "EnableEventNPCTalk", 0, gDirPath.GetNewPath("commonserver.cfg"));	

	gEnableServerDivision = GetPrivateProfileInt("GameServerInfo", "EnableServerDivision", 0, gDirPath.GetNewPath("commonserver.cfg"));		

#ifdef MODIFY_JAPAN_MONSTER_VALUE
	gMonsterHPAdjust = GetPrivateProfileInt("GameServerInfo", "MonsterHPAdjust", 100, gDirPath.GetNewPath("commonserver.cfg"));
#endif	
	//gUdpSocCRank.SendSet(gDevilSquareEventServerIp, 44455);		

	cManager.Init();

#ifdef HACK_LOG
	// ���Ӽ��� ������ ���
	GetPrivateProfileString("GameServerInfo", "HackLogServer", "10.1.2.69", gHackLogServerIp, 20, gDirPath.GetNewPath("commonserver.cfg"));	
	gSendHackLog.SendSet(gHackLogServerIp, 60005);		// HackLog
#endif


#ifdef CHECK_PENETRATION_SKILL
	gEnableCheckPenetrationSkill = GetPrivateProfileInt("GameServerInfo", "EnableCheckPenetrationSkill", 1, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	g_ShieldSystemOn				= GetPrivateProfileInt("GameServerInfo",	"ShieldSystemOn",			0,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iDamageDevideToSDRate			= GetPrivateProfileInt("GameServerInfo",	"DamageDevideToSD",			90,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iDamageDevideToHPRate			= GetPrivateProfileInt("GameServerInfo",	"DamageDevideToHP",			10,		gDirPath.GetNewPath("commonserver.cfg"));
	// CODEREVIEW_SHIELD : Float ������ ���� SuccessAttackRateOption
	// CODEREVIEW_SHIELD : PreFix ���̱�
	g_fSuccessAttackRateOption		= GetPrivateProfileInt("GameServerInfo",	"SuccessAttackRateOption",	20000,	gDirPath.GetNewPath("commonserver.cfg")) / 10000;
	g_iSDChargingOption				= GetPrivateProfileInt("GameServerInfo",	"SDChargingOption",			1,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iConstNumberOfShieldPoint		= GetPrivateProfileInt("GameServerInfo",	"ConstNumberOfShieldPoint", 20,		gDirPath.GetNewPath("commonserver.cfg"));

	g_ShieldAutoRefillOn			= GetPrivateProfileInt("GameServerInfo",	"ShieldAutoRefillOn",		0,		gDirPath.GetNewPath("commonserver.cfg"));
	g_ShieldAutoRefillOnSafeZone	= GetPrivateProfileInt("GameServerInfo",	"ShieldAutoRefilOnSafeZone",1,		gDirPath.GetNewPath("commonserver.cfg"));
	g_PKLevelIncreaseOff			= GetPrivateProfileInt("GameServerInfo",	"PKLevelIncreaseOff",		0,		gDirPath.GetNewPath("commonserver.cfg"));

	// CODEREVIEW_SHIELD3 : �������� ���� ��ũ��Ʈ �߰�
	g_CompoundPotionDropOn				= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionDropOn",			0,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv1DropRate		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv1DropRate",	100,	gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv2DropRate		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv2DropRate",	80,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv3DropRate		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv3DropRate",	50,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv1DropLevel		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv1DropLevel",	68,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv2DropLevel		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv2DropLevel",	96,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv3DropLevel		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv3DropLevel",	118,	gDirPath.GetNewPath("commonserver.cfg"));

	#ifdef UPDATE_SHIELD_OPTION_20060427
	g_bShieldComboMissOptionOn			= GetPrivateProfileInt("GameServerInfo",	"ShieldComboMissOptionOn",			0,			gDirPath.GetNewPath("commonserver.cfg"));	// PvP ���� �̽� �� �޺� ��� �ɼ� 1 : �޺� ���, 0 : �޺� ����
	g_iShieldPotionLv1MixSuccessRate	= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv1MixSuccessRate",	50,			gDirPath.GetNewPath("commonserver.cfg"));	// ���� SD ���� ���� Ȯ��
	g_iShieldPotionLv1MixMoney			= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv1MixMoney",			100000,		gDirPath.GetNewPath("commonserver.cfg"));	// ���� SD ���� ���� ���
	g_iShieldPotionLv2MixSuccessRate	= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv2MixSuccessRate",	30,			gDirPath.GetNewPath("commonserver.cfg"));	// �߰� SD ���� ���� Ȯ��
	g_iShieldPotionLv2MixMoney			= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv2MixMoney",			500000,		gDirPath.GetNewPath("commonserver.cfg"));	// �߰� SD ���� ���� ���
	g_iShieldPotionLv3MixSuccessRate	= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv3MixSuccessRate",	30,			gDirPath.GetNewPath("commonserver.cfg"));	// ū SD ���� ���� Ȯ��
	g_iShieldPotionLv3MixMoney			= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv3MixMoney",			1000000,	gDirPath.GetNewPath("commonserver.cfg"));	// ū SD ���� ���� ���
	#endif // UPDATE_SHIELD_OPTION_20060427

	#ifdef UPDATE_SHIELD_CALCULATE_GAGE_20060428
	g_iShieldGageConstA					= GetPrivateProfileInt("GameServerInfo",	"ShieldGageConstA",					12,			gDirPath.GetNewPath("commonserver.cfg"));	// ���� ������ ���� ����� 1
	g_iShieldGageConstB					= GetPrivateProfileInt("GameServerInfo",	"ShieldGageConstB",					30,			gDirPath.GetNewPath("commonserver.cfg"));	// ���� ������ ���� ����� 2
	#endif // UPDATE_SHIELD_CALCULATE_GAGE_20060428

#endif // ADD_SHIELD_POINT_01_20060403

	g_bCheckSpeedHack					= GetPrivateProfileInt("GameServerInfo",	"CheckSpeedHack",					1,			gDirPath.GetNewPath("commonserver.cfg"));

#ifdef UPDATE_TITLEBAR_VERSION_INFO_120071004
	char tmpSzGameServerVersion[15];
	int tmpServerType = 0;

#ifdef AUTH_GAMESERVER
	tmpServerType = gServerType;
#endif

	wsprintf(tmpSzGameServerVersion, "%s_%d",GAMESEVER_VERSION, tmpServerType);

	#if (TESTSERVER == 1)
		wsprintf(szTemp, lMsg.Get(415), tmpSzGameServerVersion, szServerName, 
				 szClientVersion[0], szClientVersion[1], szClientVersion[2], 
				 szClientVersion[3], szClientVersion[4],
				 szGameServerExeSerial, gCountryName);
		//MsgBox(lMsg.Get(417));
	#else 
		wsprintf(szTemp, lMsg.Get(416), tmpSzGameServerVersion, szServerName, 
				 szClientVersion[0], szClientVersion[1], szClientVersion[2], 
				 szClientVersion[3], szClientVersion[4],
				 szGameServerExeSerial, gCountryName);
	#endif	
#else	// UPDATE_TITLEBAR_VERSION_INFO_120071004
	#if (TESTSERVER == 1)
		wsprintf(szTemp, lMsg.Get(415), GAMESEVER_VERSION, szServerName, 
				 szClientVersion[0], szClientVersion[1], szClientVersion[2], 
				 szClientVersion[3], szClientVersion[4],
				 szGameServerExeSerial, gCountryName);
		MsgBox(lMsg.Get(417));
	#else 
		wsprintf(szTemp, lMsg.Get(416), GAMESEVER_VERSION, szServerName, 
				 szClientVersion[0], szClientVersion[1], szClientVersion[2], 
				 szClientVersion[3], szClientVersion[4],
				 szGameServerExeSerial, gCountryName);
	#endif
#endif	// UPDATE_TITLEBAR_VERSION_INFO_120071004
	LogAdd("User Object Size = %d", sizeof( OBJECTSTRUCT ) );
	
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
	strcat(szTemp, "[CastleSiege]");
#endif

#ifdef ADD_DISPLAY_CHECKSUM_INFO_20041127
	char szDiplayGSInfo[512] = {0,};
	wsprintf( szDiplayGSInfo, "%s [%u][%u][%u]", szTemp, dwgCheckSum[1]%1000, dwgCheckSum[17]%1000, dwgCheckSum[1004]%1000 );
	SetWindowText(ghWnd, szDiplayGSInfo);
#else
	SetWindowText(ghWnd, szTemp);
#endif

	LoadItemBag();

#ifdef HAPPY_CAT
	g_GameEvent.LoadItem(gDirPath.GetNewPath("EVENT_ITEM"));
#endif	

#ifndef MODIFY_ELDORADO_EVENT_OPTION_LOAD_SEQUEANCE_20070821
	// ������ �̺�Ʈ �ʱ�ȭ ����
	gEledoradoEvent.SetEventState(gIsEledoradoEvent);
	gEledoradoEvent.Init();
#endif // MODIFY_ELDORADO_EVENT_OPTION_LOAD_SEQUEANCE_20070821
	
	gPacketCheckSum.Init();

#ifdef PERSONAL_SHOP_20040113 
	gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef EVENT_FRIENDSHIP_20040204
	// �Ϻ� ��ģ�� �̺�Ʈ - ��ǰ �ؽ�Ʈ �б�
	FILE * hFile = fopen(gDirPath.GetNewPath("RingEventItem_JpnText.txt"), "r");
	if (hFile != NULL) {

		for (INT i = 0 ; i < MAX_ITEM_OF_FRIENDSHIP_EVENT ; i++) {
			INT iResult = fscanf(hFile, "%s\n", g_sz2ANV_GIFT_NAME[i]);
			if (iResult == EOF)
				break;
		}
		g_bRingEventItemTextLoad = TRUE;
	}
	else {
		g_bRingEventItemTextLoad = FALSE;
	}
#endif // EVENT_FRIENDSHIP_20040204

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// �߱��߰�> . �Ƿε� �ý��� ���뿩��, ����ġ, ������Ȯ�� ����
	g_bChnPlayTimeLimitOn = GetPrivateProfileInt("GameServerInfo", "ChnPlayTimeLimitOn", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iChnPlayTimeLimitLevel1Exp = GetPrivateProfileInt("GameServerInfo", "ChnPlayTimeLimitLevel1Exp", 50, gDirPath.GetNewPath("commonserver.cfg"));
	g_iChnPlayTimeLimitLevel1ItemDrop = GetPrivateProfileInt("GameServerInfo", "ChnPlayTimeLimitLevel1ItemDrop", 50, gDirPath.GetNewPath("commonserver.cfg"));
	g_iChnPlayTimeLimitLevel2Exp = GetPrivateProfileInt("GameServerInfo", "ChnPlayTimeLimitLevel2Exp", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iChnPlayTimeLimitLevel2ItemDrop = GetPrivateProfileInt("GameServerInfo", "ChnPlayTimeLimitLevel2ItemDrop", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	g_bVtmPlayTimeLimitOn				= GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitOn",					0,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iVtmPlayTimeLimitNoticeTime		= GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitNoticeTime",			60,		gDirPath.GetNewPath("commonserver.cfg"));	

	g_iVtmPlayTimeLimitLevel1Exp		= GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitLevel1Exp",			50,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iVtmPlayTimeLimitLevel1Time		= GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitLevel1Time",			180,	gDirPath.GetNewPath("commonserver.cfg"));	
	g_iVtmPlayTimeLimitLevel1NoticeTime	= GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitLevel1NoticeTime",	30,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iVtmPlayTimeLimitLevel1ItemDrop	= GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitLevel1ItemDrop",		50,		gDirPath.GetNewPath("commonserver.cfg"));

	g_iVtmPlayTimeLimitLevel2Exp		= GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitLevel2Exp",			0,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iVtmPlayTimeLimitLevel2Time		= GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitLevel2Time",			300,	gDirPath.GetNewPath("commonserver.cfg"));		
	g_iVtmPlayTimeLimitLevel2NoticeTime = GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitLevel2NoticeTime",	15,		gDirPath.GetNewPath("commonserver.cfg"));		
	g_iVtmPlayTimeLimitLevel2ItemDrop	= GetPrivateProfileInt("GameServerInfo", "VtmPlayTimeLimitLevel2ItemDrop",		0,		gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef EVENT_OPTION_RELOAD_20040305				
	ReadEventInfo(MU_EVENT_ALL);		// �����̹Ƿ� ��� �̺�Ʈ ������ �� ����
#endif

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
	g_iUseCharacterAutoRecuperationSystem = GetPrivateProfileInt("GameServerInfo", "UseCharacterAutoRecuperationSystem", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iCharacterRecuperationMaxLevel = GetPrivateProfileInt("GameServerInfo", "CharacterRecuperationMaxLevel", 100, gDirPath.GetNewPath("commonserver.cfg"));

#endif

#ifdef MAKE_ITEM_STOP
	// ������ '�����' ��ũ�� �� ������ �ɼ�
	g_iMakeStop = GetPrivateProfileInt("GameServerInfo", "MacroMakeStop", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413
	g_iSkillDistanceCheck 			= GetPrivateProfileInt("GameServerInfo", "SkillDistanceCheck",			0,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iSkillDistanceCheckTemp		= GetPrivateProfileInt("GameServerInfo", "SkillDistanceCheckTemp",		2,		gDirPath.GetNewPath("commonserver.cfg"));
	#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
		g_iSkillDistanceKick 			= GetPrivateProfileInt("GameServerInfo", "SkillDistanceKick",			0,		gDirPath.GetNewPath("commonserver.cfg"));
		g_iSkillDistanceKickCount		= GetPrivateProfileInt("GameServerInfo", "SkillDistanceKickCount",		5,		gDirPath.GetNewPath("commonserver.cfg"));
	#endif	//MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417	
	#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421
		g_iSkillDiatanceKickCheckTime   = GetPrivateProfileInt("GameServerInfo", "SkillDistanceKickCheckTime",	10,		gDirPath.GetNewPath("commonserver.cfg"));
	#endif
#endif // MODIFY_SKILL_CHECK_DISTANCE_20060413

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	g_CashShop.CashShopOptioNReload();
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	g_CashItemPeriodSystem.Initialize();
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	
#ifdef ADD_HACKING_TOOL_BLOCK_20090311
	g_HackToolBlock.Initialize();
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	g_EventItemBagExMng.AddEventItemBagEx( "ChaosCard", gDirPath.GetNewPath("ChaosCardProbability.txt") );
	g_EventItemBagExMng.AddEventItemBagEx( "ChaosCardMini", gDirPath.GetNewPath("ChaosCardProbability1.txt") );
	g_EventItemBagExMng.AddEventItemBagEx( "ChaosCardGold", gDirPath.GetNewPath("ChaosCardProbability2.txt") );
	g_EventItemBagExMng.AddEventItemBagEx( "ChaosCardRare", gDirPath.GetNewPath("ChaosCardProbability3.txt") );
	#else
	g_CashLotterySystem.Load( gDirPath.GetNewPath("ChaosCardProbability.txt") );
	#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

#if TESTSERVER == 1
//	g_CashLotterySystem.SimulationLotterySystem();
#endif
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
	
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	g_BuffScript.Load( gDirPath.GetNewPath( "BuffEffect.txt" ) );
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef COUPON_EVENT_ITEMLIST_20081022
	g_CouponEventItemLIst.Load( gDirPath.GetNewPath( "EventItemList.txt" ) );
#endif //COUPON_EVENT_ITEMLIST_20081022

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	g_bUseMapMoveCheckSum 			= GetPrivateProfileInt("GameServerInfo", "UseMapMoveCheckSum",			0,		gDirPath.GetNewPath("commonserver.cfg"));
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
	RenderCommonServerInfo();
#endif // ADD_NEWSERVER_EVENTOFF_20081114

	g_PkClearSystem.Load(gDirPath.GetNewPath("PkClearSystem.cfg"));
	g_ResetSystem.Load( gDirPath.GetNewPath("ResetSystem.cfg"));
	g_BuffHelper.Load(gDirPath.GetNewPath("BuffHelperSystem.cfg"));

	gConnectBonus = GetPrivateProfileInt("Bonus","BonusSystem",1,gDirPath.GetNewPath("OnlineBonus.ini"));
	gConnectBonusTime = GetPrivateProfileInt("Bonus", "BonusTime", 1, gDirPath.GetNewPath("OnlineBonus.ini"));
	if (gConnectBonusTime <= 0)
	{
		gConnectBonusTime = 60;
		gConnectBonus = 0;
	}
	else
	{
		gConnectBonusTime = gConnectBonusTime * 60;
	}
	gConnectBonusCashPoints = GetPrivateProfileInt("Bonus", "BonusCash", 0, gDirPath.GetNewPath("OnlineBonus.ini"));


	g_PvpEvent.Load();

	//g_VIPSystem.LoadVIP();

	g_ArrowSystem.Load();

#ifdef __HAPPYHOUR_H__
	g_HappyHour.Load();
#endif

	gBalanceSystem.Load(gDirPath.GetNewPath(BALANCESYSTEM_DIR));
	gSettings = new CSettings; gSettings->Loading(); _dynamic_experience._loading();
	Balancer.Load();
	Manager.Initialize();
	//DevGamesX 01/05/2018 - OffExp
	OffExp.LoadConfig();
	YDTeamTopOnline();
	//s_PickUp.LoadFile(gDirPath.GetNewPath("..\\data\\PickupSystem.txt"));

}

//----------------------------------------------------------------------------
// ���Ӽ����� ������ ���Ӽ��� ������ �����Ѵ�.
int GSInfoSendFlag=0;

// ���Ӻ����� ������ ��� �����Ѵ�.
void GameServerInfoSendStop()
{
	GSInfoSendFlag = 0;	
}

void GameServerInfoSendStart()
{
	GSInfoSendFlag = 1;
}

extern int gDisconnect;


#ifdef GAMESERVER_INFO_TO_CSS_20040317

VOID GSINFO_INITCSLIST()		// ���Ӽ����� ����Ʈ�� �а� ������ �ʱ�ȭ�Ѵ�.
{
	// �켱 ������ �ڿ����� ��� �����Ѵ�.
	g_iConnectServerCount = 0;
	for (INT i = 0 ; i < MAX_CS_COUNT ; i++) {
		if (gpUdpSocCSS[i] != NULL) {
			delete gpUdpSocCSS[i];
			gpUdpSocCSS[i] = NULL;
		}
		memset (g_szCSIPADDR[i], 0, sizeof(g_szCSIPADDR[i]));
	}

	// ���Ӽ��� ����Ʈ ������ ã�´�.
	INT iRES = _access(gDirPath.GetNewPath(CSLIST_FILENAME), 0);
	if (!iRES) {
		// ������ �����Ѵ�.
		FILE * pFile = fopen (gDirPath.GetNewPath(CSLIST_FILENAME), "rt");
		if (pFile) {
			for (INT j = 0 ; j < MAX_CS_COUNT ; j++) {
				INT iRETVAL = fscanf (pFile, "%s %d\n", g_szCSIPADDR[j], &g_wCSPORT[j]);
				if (iRETVAL == EOF)
					break;
				else {
					g_iConnectServerCount++;
					gpUdpSocCSS[j] = new WzUdp;
					gpUdpSocCSS[j]->CreateSocket();
				}
			}
		}
		fclose(pFile);
	}
	else {
		// ������ �������� �ʴ´�. �־��� ��Ȱ (�׳� ������ ������ ���� ������.)
		g_bSendGSInfoToCSS = 0;
		return;
	}

	// ���Ӽ����� ������ ���� ���ϵ��� �����Ѵ�.
	for (i = 0 ; i < g_iConnectServerCount ; i++) {
		if (gpUdpSocCSS[i] != NULL) {
			gpUdpSocCSS[i]->SendSet(g_szCSIPADDR[i], g_wCSPORT[i]);
		}
	}
}


void GameServerInfoSend()		// ���� TCP����� ���Ӽ��� ��������
{
	if( gDisconnect == 1 ) return;
	if( GSInfoSendFlag == 0 ) return;

	PMSG_SERVERINFO pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x01;
	pMsg.h.size			= sizeof( pMsg );
		
	if( gObjTotalUser == 0 ) pMsg.Percent = 0;
	else pMsg.Percent =	gObjTotalUser*100/gServerMaxUser;

	
	pMsg.ServerCode		= gGameServerCode;
	pMsg.UserCount		= gObjTotalUser;

	pMsg.AccountCount	= gLCount[CERTIFYTYPE_ACCOUNT].Get();
	pMsg.PCbangCount	= gLCount[CERTIFYTYPE_IP].Get();

	pMsg.MaxUserCount	= gServerMaxUser;

	if (g_bSendGSInfoToCSS == FALSE) {
		// ������ ������� ���Ӽ��� ������ ������.
		gUdpSoc.SendData((LPBYTE)&pMsg, pMsg.h.size);			
	}
	else {
		// ���Ӽ��� ���� ���Ӽ��� ������ ��κ�����.
		if (!CHECK_LIMIT(g_iConnectServerCount, MAX_CS_COUNT)) {
			LogAddC (2, "Send GS Info Error - g_iConnectServerCount is Out of Bound : %d", g_iConnectServerCount);
			gUdpSoc.SendData((LPBYTE)&pMsg, pMsg.h.size);			
		}
		else {
			for (INT i = 0 ; i < g_iConnectServerCount ; i++) {
				gpUdpSocCSS[i]->SendData((LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}

#else

#ifdef GAMESERVER_INFO_TCP_20031203

void GameServerInfoSend()		// ���� TCP����� ���Ӽ��� ��������
{
	if( gDisconnect == 1 ) return;
#ifndef GUILD_REWORK
	if( GSInfoSendFlag == 0 ) return;
#endif

	switch(g_ConServerInfoSendType) {
	case 0 :		// ���Ӽ����� ���Ӽ��� ������ UDP�� ���� ���̴�.
		{
			if( gDisconnect == 1 ) return;
#ifndef GUILD_REWORK
			if( GSInfoSendFlag == 0 ) return;
#endif

			PMSG_SERVERINFO pMsg;

			pMsg.h.c			= PMHC_BYTE;
			pMsg.h.headcode		= 0x01;
			pMsg.h.size			= sizeof( pMsg );
				
			if( gObjTotalUser == 0 ) pMsg.Percent = 0;
			else pMsg.Percent =	gObjTotalUser*100/gServerMaxUser;

			
			pMsg.ServerCode		= gGameServerCode;
			pMsg.UserCount		= gObjTotalUser;

			pMsg.AccountCount	= gLCount[CERTIFYTYPE_ACCOUNT].Get();
			pMsg.PCbangCount	= gLCount[CERTIFYTYPE_IP].Get();

			pMsg.MaxUserCount	= gServerMaxUser;

			gUdpSoc.SendData((LPBYTE)&pMsg, pMsg.h.size);			
		}
		break;
	case 1 :		// ���Ӽ����� ���Ӽ��� ������ TCP�� ���� ���̴�.
		{
			if( !IsConnectServerConnected)
			{
				wsConnectServerCli.Close();
				wsConnectServerCli.CreateSocket(ghWnd);
				if( ConnectServerConnect(connectserverip, WM_ASS_CSERVERMSG) == FALSE )
				{
					IsConnectServerConnected = 0;
					LogAddC(2, "Can't Connect to Connect Server");
					return;
				}

				IsConnectServerConnected = 1;
			}

			if( IsConnectServerConnected ) {
				PMSG_SERVERINFO pMsg;

				pMsg.h.c			= PMHC_BYTE;
				pMsg.h.headcode		= 0x01;
				pMsg.h.size			= sizeof( pMsg );
					
				if( gObjTotalUser == 0 ) pMsg.Percent = 0;
				else pMsg.Percent =	gObjTotalUser*100/gServerMaxUser;

				
				pMsg.ServerCode		= gGameServerCode;
				pMsg.UserCount		= gObjTotalUser;

				pMsg.AccountCount	= gLCount[CERTIFYTYPE_ACCOUNT].Get();
				pMsg.PCbangCount	= gLCount[CERTIFYTYPE_IP].Get();

				pMsg.MaxUserCount	= gServerMaxUser;


				wsConnectServerCli.DataSend((CHAR*)&pMsg, pMsg.h.size);
			}
		}
		break;
	}
}

#else

void GameServerInfoSend()		// ���� UDP����� ���Ӽ��� ��������
{
	if( gDisconnect == 1 ) return;
#ifndef GUILD_REWORK
	if( GSInfoSendFlag == 0 ) return;
#endif

	PMSG_SERVERINFO pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x01;
	pMsg.h.size			= sizeof( pMsg );

	if( gObjTotalUser == 0 ) pMsg.Percent = 0;
	else pMsg.Percent =	gObjTotalUser*100/gServerMaxUser;

	
	pMsg.ServerCode		= gGameServerCode;
	pMsg.UserCount		= gObjTotalUser;

#ifdef UPDATE_UNIFICATIONBILL_20090203
	pMsg.AccountCount	= gLCount[BILL_TYPE_PERSONAL_AMOUNT].Get() + gLCount[BILL_TYPE_PERSONAL_QUANTITY].Get() + gLCount[BILL_TYPE_COLLOR].Get();
	pMsg.PCbangCount	= gLCount[BILL_TYPE_PCBANG_AMOUNT].Get() + gLCount[BILL_TYPE_PCBANG_QUANTITY].Get();
#else	// UPDATE_UNIFICATIONBILL_20090203
	pMsg.AccountCount	= gLCount[CERTIFYTYPE_ACCOUNT].Get();
	pMsg.PCbangCount	= gLCount[CERTIFYTYPE_IP].Get();
#endif	// UPDATE_UNIFICATIONBILL_20090203

	pMsg.MaxUserCount	= gServerMaxUser;

	gUdpSoc.SendData((LPBYTE)&pMsg, pMsg.h.size);
}

#endif

#endif


#ifdef GAMESERVER_INFO_EXTENDED_20040625	// ���Ӽ������� ���Ӽ����� ������ �߰����� ����

void GameServerExtInfoSend()
{
	if( gDisconnect == 1 ) return;
#ifndef GUILD_REWORK
	if( GSInfoSendFlag == 0 ) return;
#endif

	PMSG_GAMESERVER_EXTINFO pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x03;
	pMsg.h.size			= sizeof( pMsg );

	pMsg.iServerCode	= gGameServerCode;
	pMsg.btIsPK			= (gNonPK) ? 0 : 1;
	strcpy (pMsg.GsVersion, GAMESEVER_VERSION);
	wsprintf(pMsg.ClVersion, "%c.%c%c.%c%c",
			szClientVersion[0], 
			szClientVersion[1], 
			szClientVersion[2], 
			szClientVersion[3], 
			szClientVersion[4]
		);

	gUdpSoc.SendData((LPBYTE)&pMsg, pMsg.h.size);
}

#endif


void CheckSumFileLoad(char *name)
{
#ifdef AUTH_GAMESERVER
	gGameServerAuth.RequestData(FILE_CHECKSUM);
	int DataBufferSize = gGameServerAuth.GetDataBufferSize();
	char* DataBuffer = gGameServerAuth.GetDataBuffer();	
	memcpy((char*)dwgCheckSum, DataBuffer, DataBufferSize);
#else
	FILE * fp;

	fp = fopen(name, "rb");
	if( fp == NULL )
	{
		MsgBox("%s file not found", name);
		return;
	}
	fread( dwgCheckSum, 1024 * sizeof ( DWORD), 1, fp);
	fclose(fp);
#endif
}


void LoadItemBag()
{
	if( LuckboxItemBag ){delete LuckboxItemBag;}

	LuckboxItemBag = new CItemBag;
	if( LuckboxItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
	LuckboxItemBag->Init("eventitembag.cwz");
#else
	LuckboxItemBag->Init("eventitembag.txt");
#endif

	if( Mon55 ){delete Mon55;}
	Mon55			= new CItemBag;
	if( Mon55 == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
	Mon55->Init("eventitembag2.cwz");
#else
	Mon55->Init("eventitembag2.txt");
#endif

	if( Mon53 ){delete Mon53;}
	Mon53			= new CItemBag;
	if( Mon53 == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
	Mon53->Init("eventitembag3.cwz");
#else
	Mon53->Init("eventitembag3.txt");
#endif

	// ��ź�� �� �����۹� ���� �ε�
	if( StarOfXMasItemBag ){delete StarOfXMasItemBag;}

#ifdef EVENT_ITEMBAG_UPGRADE
	StarOfXMasItemBag = new CItemBagEx;
#else
	StarOfXMasItemBag = new CItemBag;
#endif
	
	if( StarOfXMasItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
	StarOfXMasItemBag->Init("eventitembag4.cwz");
#else
	StarOfXMasItemBag->Init("eventitembag4.txt");
#endif

	// ���� �����۹� ���� �ε�
	if( FireCrackerItemBag ){delete FireCrackerItemBag;}
	FireCrackerItemBag = new CItemBag;
	if( FireCrackerItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
	FireCrackerItemBag->Init("eventitembag5.cwz");
#else
	FireCrackerItemBag->Init("eventitembag5.txt");
#endif

	// ���� �����۹� ���� �ε�
	if( HeartOfLoveItemBag ){delete HeartOfLoveItemBag;}
	HeartOfLoveItemBag = new CItemBag;
	if( HeartOfLoveItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
	HeartOfLoveItemBag->Init("eventitembag5.cwz");
#else
	HeartOfLoveItemBag->Init("eventitembag5.txt");
#endif

	// �ݸ޴� �����۹� ���� �ε�
	if( GoldMedalItemBag ){delete GoldMedalItemBag;}
	GoldMedalItemBag = new CItemBag;
	if( GoldMedalItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
	GoldMedalItemBag->Init("eventitembag6.cwz");
#else
	GoldMedalItemBag->Init("eventitembag6.txt");
#endif

	// ���޴� �����۹� ���� �ε�
	if( SilverMedalItemBag ){delete SilverMedalItemBag;}
	SilverMedalItemBag = new CItemBag;
	if( SilverMedalItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
	SilverMedalItemBag->Init("eventitembag7.cwz");	
#else
	SilverMedalItemBag->Init("eventitembag7.txt");	
#endif

	if( GoldGoblenItemBag ){delete GoldGoblenItemBag;}
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	GoldGoblenItemBag = new CProbabilityItemBag;
#else // MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	GoldGoblenItemBag = new CItemBag;
#endif // MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	if( GoldGoblenItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	GoldGoblenItemBag->ProbabilityItemBagInit("eventitembag8.cwz");	
#else
		GoldGoblenItemBag->Init("eventitembag8.cwz");	
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04

#else
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	GoldGoblenItemBag->ProbabilityItemBagInit("eventitembag8.txt");	
#else
	GoldGoblenItemBag->Init("eventitembag8.txt");	
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04

#endif

	if( TitanItemBag ){delete TitanItemBag;}
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	TitanItemBag = new CProbabilityItemBag;
#else
	TitanItemBag = new CItemBag;
#endif
	if( TitanItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	TitanItemBag->ProbabilityItemBagInit("eventitembag9.cwz");	
#else
	TitanItemBag->Init("eventitembag9.cwz");	
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
#else
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	TitanItemBag->ProbabilityItemBagInit("eventitembag9.txt");	
#else
	TitanItemBag->Init("eventitembag9.txt");	
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
#endif

	if( GoldDerconItemBag ){delete GoldDerconItemBag;}
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	GoldDerconItemBag = new CProbabilityItemBag;
#else
	GoldDerconItemBag = new CItemBag;
#endif
	if( GoldDerconItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	GoldDerconItemBag->ProbabilityItemBagInit("eventitembag10.cwz");
#else
	GoldDerconItemBag->Init("eventitembag10.cwz");
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04

#else
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	GoldDerconItemBag->ProbabilityItemBagInit("eventitembag10.txt");
#else
	GoldDerconItemBag->Init("eventitembag10.txt");
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
#endif

	if( DevilLizardKingItemBag ){delete DevilLizardKingItemBag;}
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	DevilLizardKingItemBag = new CProbabilityItemBag;
#else
	DevilLizardKingItemBag = new CItemBag;
#endif
	if( DevilLizardKingItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	DevilLizardKingItemBag->ProbabilityItemBagInit("eventitembag11.cwz");
#else
	DevilLizardKingItemBag->Init("eventitembag11.cwz");
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
#else
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	DevilLizardKingItemBag->ProbabilityItemBagInit("eventitembag11.txt");
#else
	DevilLizardKingItemBag->Init("eventitembag11.txt");
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
#endif

	if( KanturItemBag ){delete KanturItemBag;}
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	KanturItemBag = new CProbabilityItemBag;
#else
	KanturItemBag = new CItemBag;
#endif
	if( KanturItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

#ifdef SCRIPT_DECODE_WORK
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	KanturItemBag->ProbabilityItemBagInit("eventitembag12.cwz");
#else
	KanturItemBag->Init("eventitembag12.cwz");
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04

#else
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
	KanturItemBag->ProbabilityItemBagInit("eventitembag12.txt");
#else
	KanturItemBag->Init("eventitembag12.txt");
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
#endif


#ifdef RING_EVENT_ITEMBOX_20031124
	if( RingEventItemBag ){delete RingEventItemBag;}		// ������ ���� �̺�Ʈ �����۹�
	RingEventItemBag = new CItemBag;
	if( RingEventItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		RingEventItemBag->Init("eventitembag13.cwz");
	#else
		RingEventItemBag->Init("eventitembag13.txt");
	#endif
#endif


#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211		// ��ģ�� �̺�Ʈ - ������ ����
	if( FriendShipItemBag ){delete FriendShipItemBag;}
	FriendShipItemBag = new CItemBag;
	if( FriendShipItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		FriendShipItemBag->Init("eventitembag14.cwz");
	#else
		FriendShipItemBag->Init("eventitembag14.txt");
	#endif
#endif


#ifdef DARK_LORD_HEART_EVENT_20040810
	if( DarkLordHeartItemBag ){delete DarkLordHeartItemBag;}
	DarkLordHeartItemBag = new CItemBag;
	if( DarkLordHeartItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		DarkLordHeartItemBag->Init("eventitembag15.cwz");
	#else
		DarkLordHeartItemBag->Init("eventitembag15.txt");
	#endif
#endif


#ifdef MODIFY_KUNDUN_REWARD_01_20040915
	if( KundunEventItemBag ){delete KundunEventItemBag;}
	KundunEventItemBag = new CItemBagEx;
	if( KundunEventItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		KundunEventItemBag->Init("eventitembag17.cwz");
	#else
		KundunEventItemBag->Init("eventitembag17.txt");
	#endif
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303
	if( CastleHuntZoneBossItemBag ){delete CastleHuntZoneBossItemBag;}
	CastleHuntZoneBossItemBag = new CItemBagEx;
	if( CastleHuntZoneBossItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		CastleHuntZoneBossItemBag->Init("eventitembag18.cwz");
	#else
		CastleHuntZoneBossItemBag->Init("eventitembag18.txt");
	#endif
#endif

#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
	if( CastleItemMixItemBag ){delete CastleItemMixItemBag;}
	CastleItemMixItemBag = new CItemBagEx;
	if( CastleItemMixItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		CastleItemMixItemBag->Init("eventitembag19.cwz");
	#else
		CastleItemMixItemBag->Init("eventitembag19.txt");
	#endif
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
	if( HiddenTreasureBoxItemBag ){delete HiddenTreasureBoxItemBag;}
	HiddenTreasureBoxItemBag = new CItemBagEx;
	if( HiddenTreasureBoxItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		HiddenTreasureBoxItemBag->Init("eventitembag20.cwz");
	#else
		HiddenTreasureBoxItemBag->Init("eventitembag20.txt");
	#endif
#endif
		
#ifdef MU_CHN_THANKS_COOKIE_EVENT_20040908 
	ChnThanksCookieEvent = new CItemBagEx;

	if( ChnThanksCookieEvent == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	ChnThanksCookieEvent->Init("eventitembag16.txt");
#endif

#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
	// ������������ 
	if( RedRibbonBoxEventItemBag )			
		delete RedRibbonBoxEventItemBag;

	RedRibbonBoxEventItemBag	= new CItemBagEx;			
	if( RedRibbonBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		RedRibbonBoxEventItemBag->Init("eventitembag21.cwz");
	#else
		RedRibbonBoxEventItemBag->Init("eventitembag21.txt");
	#endif
		
	// �ʷϸ������� 
	if( GreenRibbonBoxEventItemBag )	
		delete GreenRibbonBoxEventItemBag;

	GreenRibbonBoxEventItemBag = new CItemBagEx;
	if( GreenRibbonBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		GreenRibbonBoxEventItemBag->Init("eventitembag22.cwz");
	#else
		GreenRibbonBoxEventItemBag->Init("eventitembag22.txt");
	#endif


	// �Ķ���������
	if( BlueRibbonBoxEventItemBag )	
		delete BlueRibbonBoxEventItemBag;

	BlueRibbonBoxEventItemBag	= new CItemBagEx;		
	if( BlueRibbonBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		BlueRibbonBoxEventItemBag->Init("eventitembag23.cwz");
	#else
		BlueRibbonBoxEventItemBag->Init("eventitembag23.txt");
	#endif

#endif // #ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212

#ifdef VALENTINES_DAY_BOX_EVENT_20060124	// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ
	// ��ȫ���ݸ�����  
	if( PinkChocolateBoxEventItemBag )			
		delete PinkChocolateBoxEventItemBag;

	PinkChocolateBoxEventItemBag	= new CItemBagEx;			
	if( PinkChocolateBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		PinkChocolateBoxEventItemBag->Init("eventitembag24.cwz");
	#else
		PinkChocolateBoxEventItemBag->Init("eventitembag24.txt");
	#endif
		
	// �������ݸ����� 
	if( RedChocolateBoxEventItemBag )	
		delete RedChocolateBoxEventItemBag;

	RedChocolateBoxEventItemBag = new CItemBagEx;
	if( RedChocolateBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		RedChocolateBoxEventItemBag->Init("eventitembag25.cwz");
	#else
		RedChocolateBoxEventItemBag->Init("eventitembag25.txt");
	#endif


	// �Ķ���������
	if( BlueChocolateBoxEventItemBag )	
		delete BlueChocolateBoxEventItemBag;

	BlueChocolateBoxEventItemBag	= new CItemBagEx;		
	if( BlueChocolateBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		BlueChocolateBoxEventItemBag->Init("eventitembag26.cwz");
	#else
		BlueChocolateBoxEventItemBag->Init("eventitembag26.txt");
	#endif
		
#endif // #ifdef VALENTINES_DAY_BOX_EVENT_20060124

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222	// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ
	// �������������  
	if( LightPurpleCandyBoxEventItemBag )			
		delete LightPurpleCandyBoxEventItemBag;

	LightPurpleCandyBoxEventItemBag	= new CItemBagEx;			
	if( LightPurpleCandyBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		LightPurpleCandyBoxEventItemBag->Init("eventitembag27.cwz");
	#else
		LightPurpleCandyBoxEventItemBag->Init("eventitembag27.txt");
	#endif
		
	// ��ȫ���ݸ����� 
	if( VermilionCandyBoxEventItemBag )	
		delete VermilionCandyBoxEventItemBag;

	VermilionCandyBoxEventItemBag = new CItemBagEx;
	if( VermilionCandyBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		VermilionCandyBoxEventItemBag->Init("eventitembag28.cwz");
	#else
		VermilionCandyBoxEventItemBag->Init("eventitembag28.txt");
	#endif


	// ��û���ݸ�����
	if( DeepBlueCandyBoxEventItemBag )	
		delete DeepBlueCandyBoxEventItemBag;

	DeepBlueCandyBoxEventItemBag	= new CItemBagEx;		
	if( DeepBlueCandyBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		DeepBlueCandyBoxEventItemBag->Init("eventitembag29.cwz");
	#else
		DeepBlueCandyBoxEventItemBag->Init("eventitembag29.txt");
	#endif
		
#endif // #ifdef WHITEDAY_CANDY_BOX_EVENT_20060222

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404	// ��ũ���� ������ ��(eventitembag30.txt) �ε�	
	if( CrywolfDarkElfItemBag )	
		delete CrywolfDarkElfItemBag;

	CrywolfDarkElfItemBag	= new CItemBagEx;		
	if( CrywolfDarkElfItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		CrywolfDarkElfItemBag->Init("eventitembag30.cwz");
	#else
		CrywolfDarkElfItemBag->Init("eventitembag30.txt");
	#endif
#endif	// #ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404		// �߰��� ������ ��(eventitembag31.txt) �ε�
	if( CrywolfBossMonsterItemBag )	
		delete CrywolfBossMonsterItemBag;

	CrywolfBossMonsterItemBag	= new CItemBagEx;		
	if( CrywolfBossMonsterItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		CrywolfBossMonsterItemBag->Init("eventitembag31.cwz");
	#else
		CrywolfBossMonsterItemBag->Init("eventitembag31.txt");
	#endif
#endif	// #ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404

#ifdef KANTURU_MONSTER_ITEMDROP_20060807		// ĭ���� ���� ������ ��(eventitembag32.txt, eventitembag33.txt) �ε�
	// ���� ��
	if( KanturuMayaHandItemBag )	
		delete KanturuMayaHandItemBag;

	KanturuMayaHandItemBag	= new CItemBagEx;		
	if( KanturuMayaHandItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		KanturuMayaHandItemBag->Init("eventitembag32.cwz");
	#else
		KanturuMayaHandItemBag->Init("eventitembag32.txt");
	#endif

	// ����Ʈ�޾�
	if( KanturuNightmareItemBag )	
		delete KanturuNightmareItemBag;

	KanturuNightmareItemBag	= new CItemBagEx;		
	if( KanturuNightmareItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		KanturuNightmareItemBag->Init("eventitembag33.cwz");
	#else
		KanturuNightmareItemBag->Init("eventitembag33.txt");
	#endif
#endif	// #ifdef KANTURU_MONSTER_ITEMDROP_20060807

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
	if( HallowinDayEventItemBag )
	{
		delete HallowinDayEventItemBag;
	}

	HallowinDayEventItemBag = new CItemBagEx;
	if( HallowinDayEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(110) );
		return;
	}
	#ifdef SCRIPT_DECODE_WORK
		HallowinDayEventItemBag->Init("eventitembag34.cwz");
	#else
		HallowinDayEventItemBag->Init("eventitembag34.txt");
	#endif
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119	// ���, ����(������ ��)�� ���� �߰�/����
	if( RingOfHeroEventItemBag ){delete RingOfHeroEventItemBag;}
	RingOfHeroEventItemBag = new CItemBag;
	if( RingOfHeroEventItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		RingOfHeroEventItemBag->Init("eventitembag35.cwz");
	#else
		RingOfHeroEventItemBag->Init("eventitembag35.txt");
	#endif
#endif	// UPDATE_LOWLEVEL_SUPPORT_RING_20070119

#ifdef PCBANG_COUPON_EVENT_20060124
	if( PCBangEventNPCItemBag )
		delete PCBangEventNPCItemBag;

	PCBangEventNPCItemBag = new CItemBagEx;
	if( PCBangEventNPCItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get( 110 ) );
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		PCBangEventNPCItemBag->Init( "eventitembag36.cwz" );
	#else
		PCBangEventNPCItemBag->Init( "eventitembag36.txt" );
	#endif		
#endif

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131				// �������ָӴ� ������ ����
	if( NewYearLuckyBagItemBag ){delete NewYearLuckyBagItemBag;}
	NewYearLuckyBagItemBag = new CProbabilityItemBag;
	if( NewYearLuckyBagItemBag == NULL )
	{
		MsgBox("CItemBag %s", lMsg.Get(110));
		return;
	}

	//#ifdef SCRIPT_DECODE_WORK
	//	NewYearLuckyBagItemBag->Init("eventitembag37.cwz");
	//#else
		NewYearLuckyBagItemBag->ProbabilityItemBagInit("eventitembag37.txt");
	//#endif
#endif

#ifdef UPDATE_GM_FUNCTION_20070228							// GM ���� ���� ������ ���� �ε�
		if( GMPresentBoxItemBag ) { delete GMPresentBoxItemBag; }
		GMPresentBoxItemBag	= new CProbabilityItemBag;
		if( GMPresentBoxItemBag == NULL )
		{
			MsgBox("CItemBag %s", lMsg.Get(110));
			return;
		}

		GMPresentBoxItemBag->ProbabilityItemBagInit("eventitembag41.txt");
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206						// ȥ�� ���� ������ �� �ε�
	if( BoxOfGreenChaosItemBag )
		delete BoxOfGreenChaosItemBag;

	BoxOfGreenChaosItemBag = new CProbabilityItemBag;
	if( BoxOfGreenChaosItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get( 110 ) );
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		BoxOfGreenChaosItemBag->ProbabilityItemBagInit( "eventitembag38.cwz" );
	#else
		BoxOfGreenChaosItemBag->ProbabilityItemBagInit( "eventitembag38.txt" );
	#endif

	// ����ȥ�� ����
	if( BoxOfRedChaosItemBag )
		delete BoxOfRedChaosItemBag;

	BoxOfRedChaosItemBag = new CProbabilityItemBag;
	if( BoxOfRedChaosItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get( 110 ) );
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		BoxOfRedChaosItemBag->ProbabilityItemBagInit( "eventitembag39.cwz" );
	#else
		BoxOfRedChaosItemBag->ProbabilityItemBagInit( "eventitembag39.txt" );
	#endif

	// ����ȥ�� ����
	if( BoxOfPurpleChaosItemBag )
		delete BoxOfPurpleChaosItemBag;

	BoxOfPurpleChaosItemBag = new CProbabilityItemBag;
	if( BoxOfPurpleChaosItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get( 110 ) );
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		BoxOfPurpleChaosItemBag->ProbabilityItemBagInit( "eventitembag40.cwz" );
	#else
		BoxOfPurpleChaosItemBag->ProbabilityItemBagInit( "eventitembag40.txt" );
	#endif		
#endif	// PCBANG_POINT_SYSTEM_20070206

#ifdef UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
		if( ItemGiveReoEventItemBag ) { delete ItemGiveReoEventItemBag; }
		ItemGiveReoEventItemBag	= new CProbabilityItemBag;
		if( ItemGiveReoEventItemBag == NULL )
		{
			MsgBox("CItemBag %s", lMsg.Get(110));
			return;
		}
		
		ItemGiveReoEventItemBag->ProbabilityItemBagInit("eventitembag45.txt");
#else	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408		
#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
	if( ItemGiveEventItemBag )
		delete ItemGiveEventItemBag;

	ItemGiveEventItemBag = new CItemBagEx;
	if( ItemGiveEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get( 110 ) );
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		ItemGiveEventItemBag->Init( "eventitembag45.cwz" );
	#else
		ItemGiveEventItemBag->Init( "eventitembag45.txt" );
	#endif		
#endif	// WHITEANGEL_GET_ITEM_EVENT_20060822
#endif	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408

#ifdef ADD_RAKLION_20080408
		// eventitembag52.txt �ε�
		if( pRaklionSelupanItemBag )
			delete pRaklionSelupanItemBag;
		
		pRaklionSelupanItemBag = new CProbabilityItemBag;
		if( pRaklionSelupanItemBag == NULL )
		{
			MsgBox( "CProbabilityItemBag %s", lMsg.Get( 110 ) );
			return;
		}
		
		pRaklionSelupanItemBag->ProbabilityItemBagInit( "eventitembag52.txt" );
#endif // ADD_RAKLION_20080408

#ifdef ADD_GAMBLING_20090120
		// eventitembag59.txt �ε�
		if( pGamblingSystemItemBag )
			delete pGamblingSystemItemBag;
		
		pGamblingSystemItemBag = new CGamblingItemBag;
		if( pGamblingSystemItemBag == NULL )
		{
			MsgBox( "CGamblingItemBag %s", lMsg.Get( 110 ) );
			return;
		}
		
		pGamblingSystemItemBag->GamblingItemBagInit( "eventitembag59.txt" );
#endif // ADD_GAMBLING_20090120

#ifdef ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610	// ���� �Ҳ� ���� �̺�Ʈ
		if( FireCrackerMonsterEventItemBag )
			delete FireCrackerMonsterEventItemBag;
		
		FireCrackerMonsterEventItemBag = new CProbabilityItemBag;
		if( FireCrackerMonsterEventItemBag == NULL )
		{
			MsgBox( "CProbabilityItemBag %s", lMsg.Get( 110 ) );
			return;
		}
		
	#ifdef SCRIPT_DECODE_WORK
		#ifdef MODIFY_FIRECRACKER_EVENTITEMBAG_NUMBER_CHANGE_20090930
		FireCrackerMonsterEventItemBag->ProbabilityItemBagInit( "eventitembag64.cwz" );
		#else // MODIFY_FIRECRACKER_EVENTITEMBAG_NUMBER_CHANGE_20090930
		FireCrackerMonsterEventItemBag->ProbabilityItemBagInit( "eventitembag61.cwz" );
		#endif // MODIFY_FIRECRACKER_EVENTITEMBAG_NUMBER_CHANGE_20090930
	#else
		#ifdef MODIFY_FIRECRACKER_EVENTITEMBAG_NUMBER_CHANGE_20090930
		FireCrackerMonsterEventItemBag->ProbabilityItemBagInit( "eventitembag64.txt" );
		#else // MODIFY_FIRECRACKER_EVENTITEMBAG_NUMBER_CHANGE_20090930
		FireCrackerMonsterEventItemBag->ProbabilityItemBagInit( "eventitembag61.txt" );
		#endif // MODIFY_FIRECRACKER_EVENTITEMBAG_NUMBER_CHANGE_20090930
	#endif	
#endif // ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029					// ����� ���� ��ȯ ������ �ε�
		// eventitembag56.txt �ε�
		if( pLuckyCoinEventItemBag10 )
			delete pLuckyCoinEventItemBag10;
		
		pLuckyCoinEventItemBag10 = new CProbabilityItemBag;
		if( pLuckyCoinEventItemBag10 == NULL )
		{
			MsgBox( "CProbabilityItemBag %s", lMsg.Get( 110 ) );
			return;
		}
		
		pLuckyCoinEventItemBag10->ProbabilityItemBagInit( "eventitembag56.txt" );

		// eventitembag57.txt �ε�
		if( pLuckyCoinEventItemBag20 )
			delete pLuckyCoinEventItemBag20;
		
		pLuckyCoinEventItemBag20 = new CProbabilityItemBag;
		if( pLuckyCoinEventItemBag20 == NULL )
		{
			MsgBox( "CProbabilityItemBag %s", lMsg.Get( 110 ) );
			return;
		}
		
		pLuckyCoinEventItemBag20->ProbabilityItemBagInit( "eventitembag57.txt" );

		// eventitembag58.txt �ε�
		if( pLuckyCoinEventItemBag30 )
			delete pLuckyCoinEventItemBag30;
		
		pLuckyCoinEventItemBag30 = new CProbabilityItemBag;
		if( pLuckyCoinEventItemBag30 == NULL )
		{
			MsgBox( "CProbabilityItemBag %s", lMsg.Get( 110 ) );
			return;
		}
		
		pLuckyCoinEventItemBag30->ProbabilityItemBagInit( "eventitembag58.txt" );
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��� ���� ������ ���� �ε�
		if( IllusionTempleGhostItemBagA ) { delete IllusionTempleGhostItemBagA; }
		IllusionTempleGhostItemBagA	= new CProbabilityItemBag;
		if( IllusionTempleGhostItemBagA == NULL )
		{
			MsgBox("CItemBag %s", lMsg.Get(110));
			return;
		}
		IllusionTempleGhostItemBagA->ProbabilityItemBagInit("eventitembag42.txt");
		
		if( IllusionTempleGhostItemBagB ) { delete IllusionTempleGhostItemBagB; }
		IllusionTempleGhostItemBagB	= new CProbabilityItemBag;
		if( IllusionTempleGhostItemBagB == NULL )
		{
			MsgBox("CItemBag %s", lMsg.Get(110));
			return;
		}
		IllusionTempleGhostItemBagB->ProbabilityItemBagInit("eventitembag43.txt");

		if( IllusionTempleGhostItemBagC ) { delete IllusionTempleGhostItemBagC; }
		IllusionTempleGhostItemBagC	= new CProbabilityItemBag;
		if( IllusionTempleGhostItemBagC == NULL )
		{
			MsgBox("CItemBag %s", lMsg.Get(110));
			return;
		}
		IllusionTempleGhostItemBagC->ProbabilityItemBagInit("eventitembag44.txt");
#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef CHUSEOK_MONSTER_EVENT_20070823	// eventitembag46.txt
	if( ChuseokMonsterEventItemBag )
		delete ChuseokMonsterEventItemBag;

	ChuseokMonsterEventItemBag = new CProbabilityItemBag;
	if( ChuseokMonsterEventItemBag == NULL )
	{
		MsgBox( "CProbabilityItemBag %s", lMsg.Get( 110 ) );
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		ChuseokMonsterEventItemBag->ProbabilityItemBagInit( "eventitembag46.cwz" );
	#else
		ChuseokMonsterEventItemBag->ProbabilityItemBagInit( "eventitembag46.txt" );
	#endif	
#endif	// CHUSEOK_MONSTER_EVENT_20070823

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// eventitembag47.txt
	if( NpcAlanEventItemBag )
		delete NpcAlanEventItemBag;

	NpcAlanEventItemBag = new CProbabilityItemBag;
	if( NpcAlanEventItemBag == NULL )
	{
		MsgBox( "CProbabilityItemBag %s", lMsg.Get( 110 ) );
		return;
	}

	#ifdef SCRIPT_DECODE_WORK
		NpcAlanEventItemBag->ProbabilityItemBagInit( "eventitembag47.cwz" );
	#else
		NpcAlanEventItemBag->ProbabilityItemBagInit( "eventitembag47.txt" );
	#endif		
#endif	// ADD_ITEM_GIVE_NPC_ALAN_20070823

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
		{
			//eventitembag48.txt
			if (pCherryBlossomEventItemBag)
				delete pCherryBlossomEventItemBag;
			
			pCherryBlossomEventItemBag = new CProbabilityItemBag;
			if (pCherryBlossomEventItemBag == NULL)
			{
				MsgBox("CProbabilityItemBag %s", lMsg.Get( 110 ));
				return;
			}
			
#ifdef SCRIPT_DECODE_WORK
			pCherryBlossomEventItemBag->ProbabilityItemBagInit( "eventitembag48.cwz" );
#else
			pCherryBlossomEventItemBag->ProbabilityItemBagInit( "eventitembag48.txt" );
#endif
			
			
			//eventitembag49.txt
			if (pCherryBlossomEventItemBagA)
				delete pCherryBlossomEventItemBagA;
			
			pCherryBlossomEventItemBagA = new CProbabilityItemBag;
			if (pCherryBlossomEventItemBagA == NULL)
			{
				MsgBox("CProbabilityItemBag-A %s", lMsg.Get( 110 ));
				return;
			}
			
#ifdef SCRIPT_DECODE_WORK
			pCherryBlossomEventItemBagA->ProbabilityItemBagInit( "eventitembag49.cwz" );
#else
			pCherryBlossomEventItemBagA->ProbabilityItemBagInit( "eventitembag49.txt" );
#endif
			
			//eventitembag50.txt
			if (pCherryBlossomEventItemBagB)
				delete pCherryBlossomEventItemBagB;
			
			pCherryBlossomEventItemBagB = new CProbabilityItemBag;
			if (pCherryBlossomEventItemBagB == NULL)
			{
				MsgBox("CProbabilityItemBag-B %s", lMsg.Get( 110 ));
				return;
			}
			
#ifdef SCRIPT_DECODE_WORK
			pCherryBlossomEventItemBagB->ProbabilityItemBagInit( "eventitembag50.cwz" );
#else
			pCherryBlossomEventItemBagB->ProbabilityItemBagInit( "eventitembag50.txt" );
#endif
			
			//eventitembag51.txt
			if (pCherryBlossomEventItemBagC)
				delete pCherryBlossomEventItemBagC;
			
			pCherryBlossomEventItemBagC = new CProbabilityItemBag;
			if (pCherryBlossomEventItemBagC == NULL)
			{
				MsgBox("CProbabilityItemBag-C %s", lMsg.Get( 110 ));
				return;
			}
			
#ifdef SCRIPT_DECODE_WORK
			pCherryBlossomEventItemBagC->ProbabilityItemBagInit( "eventitembag51.cwz" );
#else
			pCherryBlossomEventItemBagC->ProbabilityItemBagInit( "eventitembag51.txt" );
#endif
		}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312		


#ifdef ADD_GIFTITEM_XMASEVENT_20081030
		{
			
			if(pSantaNpcEventItem)
				delete pSantaNpcEventItem;
			
			pSantaNpcEventItem = new CProbabilityItemBag;
			if(pSantaNpcEventItem == NULL) {
				MsgBox("CProbabilityItemBag %s", lMsg.Get( 110 ));
				return;
			}
			
#ifdef SCRIPT_DECODE_WORK
			pSantaNpcEventItem->ProbabilityItemBagInit("eventitembag53.cwz");
#else
			pSantaNpcEventItem->ProbabilityItemBagInit("eventitembag53.txt");
#endif //SCRIPT_DECODE_WORK
			
			if(pSantaNpcEventItem100)
				delete pSantaNpcEventItem100;
			
			pSantaNpcEventItem100 = new CProbabilityItemBag;
			if(pSantaNpcEventItem100 == NULL) {
				MsgBox("CProbabilityItemBag100 %s", lMsg.Get( 110 ));
				return;
			}
#ifdef SCRPIPT_DECODE_WORK
			pSantaNpcEventItem100->ProbabilityItemBagInit("eventitembag54.cwz");
#else 
			pSantaNpcEventItem100->ProbabilityItemBagInit("eventitembag54.txt");
#endif //SCRPIPT_DECODE_WORK
			
			if(pSantaNpcEventItem1000)
				delete pSantaNpcEventItem1000;
			
			pSantaNpcEventItem1000 = new CProbabilityItemBag;
			if(pSantaNpcEventItem1000 == NULL) {
				MsgBox("CProbabilityItemBag1000 %s", lMsg.Get( 110 ));
				return;
			}
#ifdef SCRIPT_DECODE_WORK
			pSantaNpcEventItem1000->ProbabilityItemBagInit("eventitembag55.cwz");
#else 
			pSantaNpcEventItem1000->ProbabilityItemBagInit("eventitembag55.txt");
#endif //SCRIPT_DECODE_WORK
			
		}
#endif //ADD_GIFTITEM_XMASEVENT_20081030
}


void	SetMapName()
{
	/*strcpy(g_TerrainManager.GetMapName(MAP_INDEX_RORENCIA), lMsg.Get(2000));
	strcpy(g_TerrainManager.GetMapName(MAP_INDEX_DUNGEON], lMsg.Get(2001));
	strcpy(gMapName[MAP_INDEX_DEVIAS], lMsg.Get(2002));
	strcpy(gMapName[MAP_INDEX_NORIA], lMsg.Get(2003));
	strcpy(gMapName[MAP_INDEX_LOSTTOWER], lMsg.Get(2004));
	strcpy(gMapName[MAP_INDEX_RESERVED], "Reserved");
	strcpy(gMapName[MAP_INDEX_BATTLESOCCER], lMsg.Get(2006));
	strcpy(gMapName[MAP_INDEX_ATHLANSE], lMsg.Get(2005));
	strcpy(gMapName[MAP_INDEX_TARKAN], lMsg.Get(2008));
	strcpy(gMapName[MAP_INDEX_DEVILSQUARE], lMsg.Get(2009));
	strcpy(gMapName[MAP_INDEX_ICARUS], lMsg.Get(2013));
	strcpy(gMapName[MAP_INDEX_BLOODCASTLE1], lMsg.Get(2014));
	strcat(gMapName[MAP_INDEX_BLOODCASTLE1], "1");
	strcpy(gMapName[MAP_INDEX_BLOODCASTLE2], lMsg.Get(2014));
	strcat(gMapName[MAP_INDEX_BLOODCASTLE2], "2");
	strcpy(gMapName[MAP_INDEX_BLOODCASTLE3], lMsg.Get(2014));
	strcat(gMapName[MAP_INDEX_BLOODCASTLE3], "3");
	strcpy(gMapName[MAP_INDEX_BLOODCASTLE4], lMsg.Get(2014));
	strcat(gMapName[MAP_INDEX_BLOODCASTLE4], "4");
	strcpy(gMapName[MAP_INDEX_BLOODCASTLE5], lMsg.Get(2014));
	strcat(gMapName[MAP_INDEX_BLOODCASTLE5], "5");
	strcpy(gMapName[MAP_INDEX_BLOODCASTLE6], lMsg.Get(2014));
	strcat(gMapName[MAP_INDEX_BLOODCASTLE6], "6");*/
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� Ȯ��� �̸��߰�? !!! �ӽ� (�����ʵ�)
#endif
}


#ifdef AUTH_GAMESERVER
int	GetEventFlag()
{
	int flag = 0;

	if( gDevilSquareEvent )	flag |= EVENTMASK_DevilSquareEvent;	// ����������
	if( g_bBloodCastle )	flag |= EVENTMASK_BloodCastleEvent;	// ����ĳ��
	if( gXMasEvent )		flag |= EVENTMASK_XMasEvent;		// ��ź�Ǻ�
	if( gFireCrackerEvent )	flag |= EVENTMASK_FireCrackerEvent;	// ����
	if( gHeartOfLoveEvent )	flag |= EVENTMASK_HeartOfLoveEvent;	// ����� ��Ʈ
	if( gMedalEvent )		flag |= EVENTMASK_MedalEvent;		// �޴��̺�Ʈ
	if( g_bDoRingEvent )	flag |= EVENTMASK_RingAttackEvent;	// �����̺�Ʈ
	if( gEventChipEvent )	flag |= EVENTMASK_EventChipEvent;	// �����̺�Ʈ
	if( gIsEledoradoEvent )	flag |= EVENTMASK_IsEledoradoEvent;	// Ȳ�������̺�Ʈ
	if( gUseNPGGChecksum )	flag |= EVENTMASK_UseNPGGChecksum;	// ���Ӱ��� ��� 

	return flag;
}
#endif


#ifdef EVENT_OPTION_RELOAD_20040305				// ���� �̺�Ʈ �ɼǸ� �ٽ� �б� ����

void ReadEventInfo(MU_EVENT_TYPE eEventType)
{
	char szTemp[256];	

	switch(eEventType) {

	case MU_EVENT_ALL :					// ��� �̺�Ʈ�� �ɼ��� ����
		{
			// 1 . ���� ������
			g_DevilSquare.Load(gDirPath.GetNewPath("DevilSquare.dat"));
			gDevilSquareEvent			= GetPrivateProfileInt("GameServerInfo", "DevilSquareEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gEyesOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "EyesOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			gKeyOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "KeyOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));	   
			
			// 2 . ����ĳ��
#ifdef FOR_BLOODCASTLE
			g_BloodCastle.Load(gDirPath.GetNewPath("BloodCastle.dat"));
			g_bBloodCastle				= GetPrivateProfileInt("GameServerInfo", "BloodCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBloodCastle_StartHour	= GetPrivateProfileInt("GameServerInfo", "BloodCastleStartHour", 1, gDirPath.GetNewPath("commonserver.cfg"));
			g_iAngelKingsPaperDropRate	= GetPrivateProfileInt("GameServerInfo", "AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBloodBoneDropRate		= GetPrivateProfileInt("GameServerInfo", "BloodBoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iStoneDropRate			= GetPrivateProfileInt("GameServerInfo", "StoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

#ifdef UPDATE_BLOODECASTLE_SCADULE_20070702
			g_iBloodCastle_odd_Even_Hour= GetPrivateProfileInt("GameServerInfo", "BloodCastleOddEvenHour", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#endif

#ifdef DARKLORD_WORK
			gIsDropDarkLordItem	= GetPrivateProfileInt("GameServerInfo", "IsDropDarkLordItem", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gSleeveOfLordDropRate	= GetPrivateProfileInt("GameServerInfo", "SleeveOfLordDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			gSleeveOfLordDropLevel	= GetPrivateProfileInt("GameServerInfo", "SleeveOfLordDropLevel", 86, gDirPath.GetNewPath("commonserver.cfg"));
			gSoulOfDarkHorseDropRate	= GetPrivateProfileInt("GameServerInfo", "SoulOfDarkHorseDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			gSoulOfDarkHorseropLevel	= GetPrivateProfileInt("GameServerInfo", "SoulOfDarkHorseropLevel", 125, gDirPath.GetNewPath("commonserver.cfg"));g_iAngelKingsPaperDropRate	= GetPrivateProfileInt("GameServerInfo", "AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gSoulOfDarkSpiritDropRate	= GetPrivateProfileInt("GameServerInfo", "SoulOfDarkSpiritDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
			gSoulOfDarkSpiritDropLevel	= GetPrivateProfileInt("GameServerInfo", "SoulOfDarkSpiritDropLevel", 96, gDirPath.GetNewPath("commonserver.cfg"));	

			GetPrivateProfileString("GameServerInfo", "DarkSpiritAddExperience", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gDarkSpiritAddExperience = (float)atof(szTemp);
#endif

#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
			// ��ȣ���� �������
			gIsDropGemOfDefend  = GetPrivateProfileInt("GameServerInfo", "IsDropGemOfDefend", 0, gDirPath.GetNewPath("commonserver.cfg"));
			// ��ȣ���� ��� Ȯ�� (10000 ����)
			gGemOfDefendDropRate = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropRate", 5, gDirPath.GetNewPath("commonserver.cfg"));
			// ��ȣ���� ��� ���� ����
			gGemOfDefendDropLevel = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));;		
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314
			// �÷��� ������ ��Ʈ������ �������
			gIsDropSetItemInCastleHuntZone  = GetPrivateProfileInt("GameServerInfo", "IsDropSetItemInCastleHuntZone", 0, gDirPath.GetNewPath("commonserver.cfg"));
			// �÷��� ������ ��Ʈ������ ���Ȯ�� (10000 ����)
			gSetItemInCastleHuntZoneDropRate = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropRate", 1, gDirPath.GetNewPath("commonserver.cfg"));
			// �÷��� ������ ��Ʈ������ ��� ���� ����
			gSetItemInCastleHuntZoneDropLevel = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));;		
#endif

#ifdef FOR_NEW_TESTSERVER
	gTestServer	= GetPrivateProfileInt("GameServerInfo", "TestServer", 0, gDirPath.GetNewPath("commonserver.cfg"));	
#endif

			// 3 . ���� ����
			gAttackEventRegenTime = GetPrivateProfileInt("GameServerInfo", "AttackEventRegenTime", 10, gDirPath.GetNewPath("commonserver.cfg"));

			// 4 . Ȳ�� ����
			gIsEledoradoEvent				= GetPrivateProfileInt("GameServerInfo", "IsEledoradoEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldGoblenRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldGoblenRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoTitanRegenTime		= GetPrivateProfileInt("GameServerInfo", "EledoradoTitanRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldDerconRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldDerconRegenTime", 240*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilLizardKingRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilLizardKingRegenTime", 120*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilTantarosRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilTantarosRegenTime", 120*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldGoblenItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldGoblenItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoTitanItemDropRate			= GetPrivateProfileInt("GameServerInfo", "EledoradoTitanItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldDerconItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldDerconItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilLizardKingItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilLizardKingItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilTantarosItemDropRate		= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilTantarosItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldGoblenExItemDropRate		= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldGoblenExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoTitanExItemDropRate			= GetPrivateProfileInt("GameServerInfo", "EledoradoTitanExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldDerconExItemDropRate		= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldDerconExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilLizardKingExItemDropRate = GetPrivateProfileInt("GameServerInfo", "EledoradoDevilLizardKingExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilTantarosExItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilTantarosExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
			gGoldenRabbitRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenRabbitRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gGoldenDarknightRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenDarkKnightRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gGoldenDevilRegenTime  = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenDevilRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
			
			gGoldenStoneMonsterRegenTIme = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenMonsterRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldenCrustEventRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenCrustRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldenSatirosRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenSatirosRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldenTwintailRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenTwintailRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldenIronKnightRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenIronKnightRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldenNeipinRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenNeipinRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldenGreatDragonRegenTime = GetPrivateProfileInt("GameServerInfo", "EledoradoGoldenGreatDragonRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef MODIFY_ELDORADO_EVENT_OPTION_LOAD_SEQUEANCE_20070821
			// ������ �̺�Ʈ �ʱ�ȭ ����
			gEledoradoEvent.SetEventState(gIsEledoradoEvent);
			gEledoradoEvent.Init();
#endif // MODIFY_ELDORADO_EVENT_OPTION_LOAD_SEQUEANCE_20070821

			// 5 . ��������� ����
#ifdef MONSTER_HERD_SYSTEM_20031120
			// ���� �̺�Ʈ�� ���� ���θ� ���´�.
			GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			g_bDoRingEvent				= atoi(szTemp);
			// ��ũ�� ����� �� ��ǰ�� Ż Ȯ�� (1/n)
			GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRingOrcKillGiftRate		= atoi(szTemp);
			// ������ ������ �� ��ǰ�� Ż Ȯ�� (1/n)
			GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRingDropGiftRate			= atoi(szTemp);
			g_RingAttackEvent.Load(gDirPath.GetNewPath("RingAttackEvent.dat"));
#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// ������ ���뼭�� �϶��� ����������, ����ĳ��, ī����ĳ��, ��������� �̺�Ʈ�� �����ȴ�.
			g_bDoRingEvent				= FALSE;
#endif	
			g_RingAttackEvent.EnableEvent (g_bDoRingEvent);
#endif

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
			g_bDoXMasEvent				= GetPrivateProfileInt("GameServerInfo", "XMasAttackEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_nXMasDropRate				= GetPrivateProfileInt("GameServerInfo", "XMasAttackEvent_DropRate", XMASDROPRATE_ITEM, gDirPath.GetNewPath("commonserver.cfg"));
			g_nXMasDropZen				= GetPrivateProfileInt("GameServerInfo", "XMasAttackEvent_DropZen", XMASDROP_ZEN, gDirPath.GetNewPath("commonserver.cfg"));
			g_XMasAttackEvent.Load(gDirPath.GetNewPath("XMasAttackEvent.dat"));
			g_XMasAttackEvent.EnableEvent(g_bDoXMasEvent);
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#ifdef ADD_XMASEVENT_LUCKNUMBER
			g_nXMasLuckNumber1st		= GetPrivateProfileInt("GameServerInfo", "XMasEvent_LuckNumber1st", XMASEVENT_LUCKNUMBER1ST, gDirPath.GetNewPath("commonserver.cfg"));
			g_nXMasLuckNumber2nd		= GetPrivateProfileInt("GameServerInfo", "XMasEvent_LuckNumber2nd", XMASEVENT_LUCKNUMBER2ND, gDirPath.GetNewPath("commonserver.cfg"));
#endif // ADD_XMASEVENT_LUCKNUMBER

#ifdef CASTLEDEEP_EVENT_20050413
			GetPrivateProfileString("GameServerInfo", "CastleDeepEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			g_bDoCastleDeepEvent		= atoi(szTemp);
			g_CastleDeepEvent.Load(gDirPath.GetNewPath("CastleDeepEvent.dat"));
			g_CastleDeepEvent.EnableEvent(g_bDoCastleDeepEvent);
#endif	// #ifdef CASTLEDEEP_EVENT_20050413

			// 6 . �������� (���귯��) �̺�Ʈ

			// # REMOVE_LOVELOVE_EVENT_COUPLING_WITH_JOINSERVER_20060319
			// # ���� ������ �����ؼ� �̺�Ʈ ó���� �� ������ ����(apple)
			//		- ���� ������ ��ȯ�ϴ� 0x30, 0x31 �������� ����
			//		- ���� ������ ������Ʈ ���� DB �� ����.
			//		- ���� �������� ������Ʈ �ڵ� ���� �� ����.
			GetPrivateProfileString("GameServerInfo", "EVENT1", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gEvent1						= atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayMax", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gEvent1ItemDropTodayCount	= 0;
			gEvent1ItemDropTodayMax		= atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayPercent", "10000000", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gEvent1ItemDropTodayPercent = atoi(szTemp);

			// 7 . ���� �̺�Ʈ
			gFireCrackerEvent			= GetPrivateProfileInt("GameServerInfo", "FireCrackerEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gFireCrackerDropRate		= GetPrivateProfileInt("GameServerInfo", "FireCrackerDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_ItemDropRateForgFireCracker	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForFireCracker", 2, gDirPath.GetNewPath("commonserver.cfg"));
			gOnlyFireCrackerEffectUse	= GetPrivateProfileInt("GameServerInfo", "OnlyFireCrackerEffectUse", 0, gDirPath.GetNewPath("commonserver.cfg"));

			// 8 . �޴� �̺�Ʈ
			gMedalEvent					= GetPrivateProfileInt("GameServerInfo", "MedalEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldMedalDropRate			= GetPrivateProfileInt("GameServerInfo", "GoldMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));						// �ݸ޴��� ������ Ȯ��
			gSilverMedalDropRate		= GetPrivateProfileInt("GameServerInfo", "SilverMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));					// ���޴��� ������ Ȯ��
			g_ItemDropRateForGoldMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForGoldMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));		// �ݸ޴޿��� �������� ������ Ȯ��
			g_ItemDropRateForSilverMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForSilverMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));	// ���޴޿��� �������� ������ Ȯ��

			// 9 . ��ź�Ǻ� �̺�Ʈ
			gXMasEvent					= GetPrivateProfileInt("GameServerInfo", "XMasEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_XMasEvent_StarOfXMasDropRate	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_StarOfXMasDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_XMasEvent_ItemDropRateForStarOfXMas	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_ItemDropRateForStarOfXMas", 2, gDirPath.GetNewPath("commonserver.cfg"));

			// 10 . �������Ʈ �̺�Ʈ
			gHeartOfLoveEvent			= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gHeartOfLoveDropRate		= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_ItemDropRateForgHeartOfLove	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForHeartOfLove", 2, gDirPath.GetNewPath("commonserver.cfg"));

			// 11 . NPC�� �����λ�
			GetPrivateProfileString("GameServerInfo", "HappyNewYearTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gHappyNewYearNpcEvent		= atoi(szTemp);
			if( gHappyNewYearNpcEvent )	LogAddTD(lMsg.Get(573));	// [�ɼ�] ���� �λ� NPC ��ȭ ���

			// 12 . NPC�� �޸�ũ��������
			GetPrivateProfileString("GameServerInfo", "MerryXMasTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gMerryXMasNpcEvent			= atoi(szTemp);
			if( gMerryXMasNpcEvent )	LogAddTD(lMsg.Get(572));	// [�ɼ�] ũ�������� NPC ��ȭ ���

#ifdef CHAOSCASTLE_SYSTEM_20040408 
			// 13 . ī���� ĳ��
			g_ChaosCastle.Load(gDirPath.GetNewPath("ChaosCastle.dat"));
			g_bChaosCastle				= GetPrivateProfileInt("GameServerInfo", "ChaosCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518									// ����� ǥ�� ���Ȯ���� �д´�.
			// 14 . Į���� - �����ǥ��
			g_iKundunMarkDropRate		= GetPrivateProfileInt("GameServerInfo", "KundunMarkDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804							// �ź��� ���� ��� Ȯ��
			// 15 . �ź��� ���� ��ӷ�
			g_iMysteriousBeadDropRate1 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate1", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iMysteriousBeadDropRate2 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate2", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iHiddenTreasureBoxOfflineRate = GetPrivateProfileInt("GameServerInfo", "HiddenTreasureBoxOfflineRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef EVENT_MANAGEMENT_20040711
			// 16 . �̺�Ʈ �Ŵ��� ���� ����
			GetPrivateProfileString("GameServerInfo", "EventManagerOn", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			g_bEventManagerOn				= (BOOL)atoi(szTemp);

			g_EventManager.Load(gDirPath.GetNewPath("EventManagement.dat"));

			g_EventManager.RegEvent (CEventManagement::EVENT_ID_DRAGONHERD,		(LPVOID) DragonEvent);
			g_EventManager.RegEvent (CEventManagement::EVENT_ID_UNDERTROOP,		(LPVOID) AttackEvent);
			g_EventManager.RegEvent (CEventManagement::EVENT_ID_ELDORADO,		(LPVOID) &gEledoradoEvent);
#ifdef MONSTER_HERD_SYSTEM_20031120
			g_EventManager.RegEvent (CEventManagement::EVENT_ID_WHITEMAGE,		(LPVOID) &g_RingAttackEvent);
#endif
#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
			g_EventManager.RegEvent (CEventManagement::EVENT_ID_SANTACLAUS,		(LPVOID) &g_XMasAttackEvent);
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

			g_EventManager.Init(g_bEventManagerOn);
#endif

#ifdef HAPPY_POUCH
			// apple��20050204
			// 17 . �Ķ�, ���� ���ָӴ� �̺�Ʈ
			gHappyPouchRedDropRate	= GetPrivateProfileInt("GameServerInfo", "HappyPouchRedDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));							
			gHappyPouchBlueDropRate	= GetPrivateProfileInt("GameServerInfo", "HappyPouchBlueDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));							
#endif
	
			// 18. ��ũ�ε��� ���� �̺�Ʈ
#ifdef DARK_LORD_HEART_EVENT_20040810						// ��ũ�ε��� ���� �̺�Ʈ ���� Ȯ���� �д´�.
			g_iDarkLordHeartDropRate		= GetPrivateProfileInt("GameServerInfo", "DarkLordHeartDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iDarkLordHeartOffEventRate	= GetPrivateProfileInt("GameServerInfo", "DarkLoadHeartOffEventRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif		
			// 19. ũ�������� �������� �̺�Ʈ
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
			g_bRibbonBoxEvent					= GetPrivateProfileInt( "GameServerInfo", "RibbonBoxEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iRedRibbonBoxDropLevelMin			= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedRibbonBoxDropLevelMax			= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));	
			g_iRedRibbonBoxDropRate				= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedRibbonBoxDropZenRate			= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedRibbonBoxDropZen				= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iGreenRibbonBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iGreenRibbonBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iGreenRibbonBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iGreenRibbonBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iGreenRibbonBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iBlueRibbonBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));		
			g_iBlueRibbonBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueRibbonBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueRibbonBoxDropZenRate			= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueRibbonBoxDropZen				= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif //#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
			
		// 20. �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ( RKFKA )
#ifdef VALENTINES_DAY_BOX_EVENT_20060124
			g_bChocolateBoxEvent				= GetPrivateProfileInt( "GameServerInfo", "ChocolateEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iPinkChocolateBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iPinkChocolateBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));	
			g_iPinkChocolateBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iPinkChocolateBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iPinkChocolateBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iRedChocolateBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedChocolateBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedChocolateBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedChocolateBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedChocolateBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iBlueChocolateBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));		
			g_iBlueChocolateBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueChocolateBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueChocolateBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueChocolateBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif //#ifdef VALENTINES_DAY_BOX_EVENT_20060124

		// 21. ȭ��Ʈ���� �������� �̺�Ʈ( RKFKA )
#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
			g_bCandyBoxEvent					= GetPrivateProfileInt( "GameServerInfo", "CandyBoxEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iLightPurpleCandyBoxDropLevelMin	= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iLightPurpleCandyBoxDropLevelMax	= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));	
			g_iLightPurpleCandyBoxDropRate		= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iLightPurpleCandyBoxDropZenRate	= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iLightPurpleCandyBoxDropZen		= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iVermilionCandyBoxDropLevelMin	= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iVermilionCandyBoxDropLevelMax	= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iVermilionCandyBoxDropRate		= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iVermilionCandyBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iVermilionCandyBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iDeepBlueCandyBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));		
			g_iDeepBlueCandyBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iDeepBlueCandyBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iDeepBlueCandyBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iDeepBlueCandyBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif //#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
			
		}
		break;

	case MU_EVENT_DEVILSQUARE :			// ����������
		{
			g_DevilSquare.Load(gDirPath.GetNewPath("DevilSquare.dat"));
			gDevilSquareEvent			= GetPrivateProfileInt("GameServerInfo", "DevilSquareEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gEyesOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "EyesOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			gKeyOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "KeyOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));	   
		}
		break;
	case MU_EVENT_BLOODCASTLE :			// ����ĳ��
		{
#ifdef FOR_BLOODCASTLE
			g_BloodCastle.Load(gDirPath.GetNewPath("BloodCastle.dat"));
			g_bBloodCastle				= GetPrivateProfileInt("GameServerInfo", "BloodCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBloodCastle_StartHour	= GetPrivateProfileInt("GameServerInfo", "BloodCastleStartHour", 1, gDirPath.GetNewPath("commonserver.cfg"));
			g_iAngelKingsPaperDropRate	= GetPrivateProfileInt("GameServerInfo", "AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBloodBoneDropRate		= GetPrivateProfileInt("GameServerInfo", "BloodBoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iStoneDropRate			= GetPrivateProfileInt("GameServerInfo", "StoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

#ifdef UPDATE_BLOODECASTLE_SCADULE_20070702
			g_iBloodCastle_odd_Even_Hour= GetPrivateProfileInt("GameServerInfo", "BloodCastleOddEvenHour", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#endif
		}
		break;
	case MU_EVENT_ATTACKEVENTTROOP :	// ���� ����
		{
			gAttackEventRegenTime		= GetPrivateProfileInt("GameServerInfo", "AttackEventRegenTime", 10, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_EVENT_GOLDENTROOP :			// Ȳ�� ����
		{
			gIsEledoradoEvent				= GetPrivateProfileInt("GameServerInfo", "IsEledoradoEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldGoblenRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldGoblenRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoTitanRegenTime		= GetPrivateProfileInt("GameServerInfo", "EledoradoTitanRegenTime", 60*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldDerconRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldDerconRegenTime", 240*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilLizardKingRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilLizardKingRegenTime", 120*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilTantarosRegenTime	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilTantarosRegenTime", 120*3, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldGoblenItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldGoblenItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoTitanItemDropRate			= GetPrivateProfileInt("GameServerInfo", "EledoradoTitanItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldDerconItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldDerconItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilLizardKingItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilLizardKingItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilTantarosItemDropRate		= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilTantarosItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldGoblenExItemDropRate		= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldGoblenExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoTitanExItemDropRate			= GetPrivateProfileInt("GameServerInfo", "EledoradoTitanExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoGoldDerconExItemDropRate		= GetPrivateProfileInt("GameServerInfo", "EledoradoGoldDerconExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilLizardKingExItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilLizardKingExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gEledoradoDevilTantarosExItemDropRate	= GetPrivateProfileInt("GameServerInfo", "EledoradoDevilTantarosExItemDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_EVENT_WHITEMAGETROOP :		// ��������� ����
		{
#ifdef MONSTER_HERD_SYSTEM_20031120
			char szTemp[256];	
			// ���� �̺�Ʈ�� ���� ���θ� ���´�.
			GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			g_bDoRingEvent					= atoi(szTemp);
			// ��ũ�� ����� �� ��ǰ�� Ż Ȯ�� (1/n)
			GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRingOrcKillGiftRate			= atoi(szTemp);
			// ������ ������ �� ��ǰ�� Ż Ȯ�� (1/n)
			GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRingDropGiftRate				= atoi(szTemp);
			g_RingAttackEvent.Load(gDirPath.GetNewPath("RingAttackEvent.dat"));
#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// ������ ���뼭�� �϶��� ����������, ����ĳ��, ī����ĳ��, ��������� �̺�Ʈ�� �����ȴ�.
			g_bDoRingEvent					= FALSE;
#endif	
			g_RingAttackEvent.EnableEvent (g_bDoRingEvent);
#endif
		}
		break;
	case MU_EVENT_LOVEPANGPANG :		// �������� �̺�Ʈ (���Ϳ��Լ� �Ϸ� ���������� �������Ʈ (14,12) ��ӵ�)
		{
			GetPrivateProfileString("GameServerInfo", "EVENT1", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gEvent1							= atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayMax", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gEvent1ItemDropTodayCount		= 0;
			gEvent1ItemDropTodayMax			= atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayPercent", "10000000", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gEvent1ItemDropTodayPercent		= atoi(szTemp);
		}
		break;
	case MU_EVENT_FIRECRACKER :			// ���� �̺�Ʈ (���� ���� �� ���� (14,11) ��ӵ� -> �ѱ��� ��Ʋ�� ���ϵ� Ư��������)
		{
			gFireCrackerEvent				= GetPrivateProfileInt("GameServerInfo", "FireCrackerEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gFireCrackerDropRate			= GetPrivateProfileInt("GameServerInfo", "FireCrackerDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_ItemDropRateForgFireCracker	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForFireCracker", 2, gDirPath.GetNewPath("commonserver.cfg"));
			gOnlyFireCrackerEffectUse		= GetPrivateProfileInt("GameServerInfo", "OnlyFireCrackerEffectUse", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_EVENT_MEDALION :			// �޴� �̺�Ʈ (�¿��� �� ������ ���� ���޴�, �ݸ޴� ���� ���´�.)
		{
			gMedalEvent						= GetPrivateProfileInt("GameServerInfo", "MedalEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldMedalDropRate				= GetPrivateProfileInt("GameServerInfo", "GoldMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));						// �ݸ޴��� ������ Ȯ��
			gSilverMedalDropRate			= GetPrivateProfileInt("GameServerInfo", "SilverMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));					// ���޴��� ������ Ȯ��
			g_ItemDropRateForGoldMedal		= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForGoldMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));		// �ݸ޴޿��� �������� ������ Ȯ��
			g_ItemDropRateForSilverMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForSilverMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));	// ���޴޿��� �������� ������ Ȯ��
		}
		break;
	case MU_EVENT_XMASSTAR :			// ��ź�Ǻ� �̺�Ʈ (���Ϳ��Լ� ��ź�Ǻ��� ��ӵ�)
		{
			gXMasEvent						= GetPrivateProfileInt("GameServerInfo", "XMasEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_XMasEvent_StarOfXMasDropRate	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_StarOfXMasDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_XMasEvent_ItemDropRateForStarOfXMas	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_ItemDropRateForStarOfXMas", 2, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_EVENT_HEARTOFLOVE :			// �������Ʈ �̺�Ʈ (���Ϳ��Լ� �������Ʈ�� ��ӵ�)
		{
			gHeartOfLoveEvent				= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gHeartOfLoveDropRate			= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_ItemDropRateForgHeartOfLove	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForHeartOfLove", 2, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_EVENT_SAY_HAPPYNEWYEAR :	// NPC�� �����λ縦 �Ѵ�.
		{
			GetPrivateProfileString("GameServerInfo", "HappyNewYearTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gHappyNewYearNpcEvent			= atoi(szTemp);
			if( gHappyNewYearNpcEvent )	LogAddTD(lMsg.Get(573));	// [�ɼ�] ���� �λ� NPC ��ȭ ���
		}
		break;
	case MU_EVENT_SAY_MERRYXMAS :		// NPC�� �޸� ũ����������� ���Ѵ�.
		{
			GetPrivateProfileString("GameServerInfo", "MerryXMasTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gMerryXMasNpcEvent				= atoi(szTemp);
			if( gMerryXMasNpcEvent )	LogAddTD(lMsg.Get(572));	// [�ɼ�] ũ�������� NPC ��ȭ ���
		}
		break;
#ifdef CHAOSCASTLE_SYSTEM_20040408 
	case MU_EVENT_CHAOSCASTLE :			// ī���� ĳ��
		{
			g_ChaosCastle.Load(gDirPath.GetNewPath("ChaosCastle.dat"));
			g_bChaosCastle				= GetPrivateProfileInt("GameServerInfo", "ChaosCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
#endif
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
	case MU_EVENT_CHRISTMAS_RIBBONBOX:	// ũ�������� �������� �̺�Ʈ. ( ���Ϳ��Լ� ����,�ʷ�,�Ķ� �������ڰ� ��ӵ� )
		{
			g_bRibbonBoxEvent					= GetPrivateProfileInt( "GameServerInfo", "RibbonBoxEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iRedRibbonBoxDropLevelMin			= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedRibbonBoxDropLevelMax			= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));	
			g_iRedRibbonBoxDropRate				= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedRibbonBoxDropZenRate			= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedRibbonBoxDropZen				= GetPrivateProfileInt( "GameServerInfo", "RedRibbonBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iGreenRibbonBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iGreenRibbonBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iGreenRibbonBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iGreenRibbonBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iGreenRibbonBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "GreenRibbonBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iBlueRibbonBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));		
			g_iBlueRibbonBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueRibbonBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueRibbonBoxDropZenRate			= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueRibbonBoxDropZen				= GetPrivateProfileInt( "GameServerInfo", "BlueRibbonBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
#endif //#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212	
		
#ifdef VALENTINES_DAY_BOX_EVENT_20060124
	case MU_EVENT_VALENTINESDAY_CHOCOLATEBOX:	// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ( ���Ϳ��Լ� ��ȫ,����,�Ķ� ���ݸ����ڰ� ��ӵ� )
		{
			g_bChocolateBoxEvent				= GetPrivateProfileInt( "GameServerInfo", "ChocolateEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iPinkChocolateBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iPinkChocolateBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));	
			g_iPinkChocolateBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iPinkChocolateBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iPinkChocolateBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "PinkChocolateBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iRedChocolateBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedChocolateBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedChocolateBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedChocolateBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRedChocolateBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "RedChocolateBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iBlueChocolateBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));		
			g_iBlueChocolateBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueChocolateBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueChocolateBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iBlueChocolateBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "BlueChocolateBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
#endif //#ifdef VALENTINES_DAY_BOX_EVENT_20060124

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
	case MU_EVENT_WHITEDAY_CANDYBOX:	// ȭ��Ʈ���� �������� �̺�Ʈ( ���Ϳ��Լ� ������,��Ȳ,��û �������ڰ� ��ӵ� )
		{
			g_bCandyBoxEvent					= GetPrivateProfileInt( "GameServerInfo", "CandyBoxEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iLightPurpleCandyBoxDropLevelMin	= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iLightPurpleCandyBoxDropLevelMax	= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));	
			g_iLightPurpleCandyBoxDropRate		= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iLightPurpleCandyBoxDropZenRate	= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iLightPurpleCandyBoxDropZen		= GetPrivateProfileInt( "GameServerInfo", "LightPurpleCandyBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iVermilionCandyBoxDropLevelMin	= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iVermilionCandyBoxDropLevelMax	= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iVermilionCandyBoxDropRate		= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iVermilionCandyBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iVermilionCandyBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "VermilionCandyBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			g_iDeepBlueCandyBoxDropLevelMin		= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));		
			g_iDeepBlueCandyBoxDropLevelMax		= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iDeepBlueCandyBoxDropRate			= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iDeepBlueCandyBoxDropZenRate		= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropZenRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iDeepBlueCandyBoxDropZen			= GetPrivateProfileInt( "GameServerInfo", "DeepBlueCandyBoxDropZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
#endif //#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
		
	default:
		{
		}
		break;
	}
}


#endif

#ifdef ETC_OPTION_RELOAD_20040308		// ���� �ɼ� �� �̺�Ʈ ���� �͵鸸 ���ε�

void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType)
{
	char szTemp[256];	

	switch (eGameEtcType) {
	case MU_ETC_ALL :					// ��� �̺�Ʈ ���� �ɼ� ���ε�
		{
			// 1 . �ɸ��� ���� ON/OFF
			GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gCreateCharacter = atoi(szTemp);

			// 2 . ��� ����, ����, ��������
			GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildCreate = atoi(szTemp);
			LogAdd(lMsg.Get(569), gGuildCreate);
			GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildDestroy = atoi(szTemp);
			LogAdd(lMsg.Get(570), gGuildDestroy);		// "��� ���� �ɼ� %d"
			GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildCreateLevel = atoi(szTemp);
			LogAdd(lMsg.Get(571), gGuildCreateLevel);	// ������ �ּ� ���� %d
#ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
			// ������� ��ü ����
			g_bCastleGuildDestoyLimit	= GetPrivateProfileInt( "GameServerInfo", "CastleOwnerGuildDestroyLimit", 1, gDirPath.GetNewPath("commonserver.cfg"));
#endif
			
			// 3 . �ŷ� ���� ���� ON/OFF
			GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			bCanTrade				= atoi(szTemp);

			// 4 . ī�����ڽ� ��밡�� ���� ON/OFF
			bCanChaosBox = GetPrivateProfileInt("GameServerInfo", "ChaosBox", 0, gDirPath.GetNewPath("commonserver.cfg"));

			// 5 . PK�� ������ ��� ���� ON/OFF
			GetPrivateProfileString("GameServerInfo", "PKItemDrop", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gPkItemDrop  = atoi(szTemp);

			// 6 . ������ ���Ȯ�� ����
			GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gItemDropPer = atoi(szTemp);

			// 7 . ���λ��� �������� ����
#ifdef PERSONAL_SHOP_20040113			
			gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

			// 8 . ���ǵ��� ���� �ɼ�
			gAttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo", "AttackSpeedTimeLimit", 800, gDirPath.GetNewPath("commonserver.cfg"));
			bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, gDirPath.GetNewPath("commonserver.cfg"));		
			gHackCheckCount = GetPrivateProfileInt("GameServerInfo", "HackCheckCount", 5.5, gDirPath.GetNewPath("commonserver.cfg"));	
			gMinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo", "MinimumAttackSpeedTime", 200, gDirPath.GetNewPath("commonserver.cfg"));	
			gDetectedHackKickCount = GetPrivateProfileInt("GameServerInfo", "DetectedHackKickCount", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gIsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickDetecHackCountLimit", 0, gDirPath.GetNewPath("commonserver.cfg"));	
#ifdef CHINA_HACKUSER_KICK_SYSTEM
	ChinaHackLogFile.Output("[HLS] AttackSpeedTimeLimit = %d", gAttackSpeedTimeLimit);
	ChinaHackLogFile.Output("[HLS] IsIgnorePacketHackDetect = %d", bIsIgnorePacketSpeedHackDetect);
	ChinaHackLogFile.Output("[HLS] HackCheckCount = %d", gHackCheckCount);
	ChinaHackLogFile.Output("[HLS] MinimumAttackSpeedTime = %d", gMinimumAttackSpeedTime);
	ChinaHackLogFile.Output("[HLS] DetectedHackKickCount = %d", gDetectedHackKickCount);
	ChinaHackLogFile.Output("[HLS] IsKickDetecHackCountLimit = %d", gIsKickDetecHackCountLimit);

	ChinaHackLogFile.Output("[HLS] NotAttackAreaKickCount = %d", gNotAttackAreaKickCount);
	ChinaHackLogFile.Output("[HLS] IsKickNotAttackAreaCountLimit = %d", gIsKickNotAttackAreaCountLimit);

	ChinaHackLogFile.Output("[HLS] BlockHackUserCharacter = %d", gBlockHackUserCharacter);
	ChinaHackLogFile.Output("[HLS] BlockHackUserAccount = %d", gBlockHackUserAccount);
#endif
		}
		break;
	case MU_ETC_CREATECHARACTER :		// �ɸ��� ����
		{
			GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gCreateCharacter = atoi(szTemp);
		}
		break;
	case MU_ETC_GUILD :					// ��� ����
		{
			GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildCreate = atoi(szTemp);
			LogAdd(lMsg.Get(569), gGuildCreate);
			GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildDestroy = atoi(szTemp);
			LogAdd(lMsg.Get(570), gGuildDestroy);		// "��� ���� �ɼ� %d"
			GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildCreateLevel = atoi(szTemp);
			LogAdd(lMsg.Get(571), gGuildCreateLevel);	// ������ �ּ� ���� %d

#ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
			// ������� ��ü ����
			g_bCastleGuildDestoyLimit	= GetPrivateProfileInt( "GameServerInfo", "CastleOwnerGuildDestroyLimit", 1, gDirPath.GetNewPath("commonserver.cfg"));
#endif
		}
		break;
	case MU_ETC_TRADE :					// �ŷ�
		{
			GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			bCanTrade				= atoi(szTemp);
		}
		break;
	case MU_ETC_USECHAOSBOX :			// ī�����ڽ� ���
		{
			bCanChaosBox = GetPrivateProfileInt("GameServerInfo", "ChaosBox", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_ETC_PERSONALSHOP :			// ���λ���
		{
#ifdef PERSONAL_SHOP_20040113			
			gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif
		}
		break;
	case MU_ETC_PKITEMDROP :			// PK�� ������ ��ӿ���
		{
			GetPrivateProfileString("GameServerInfo", "PKItemDrop", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gPkItemDrop  = atoi(szTemp);
		}
		break;
	case MU_ETC_ITEMDROPRATE :			// ������ ���Ȯ��
		{
			GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gItemDropPer = atoi(szTemp);
		}
		break;
	case MU_ETC_SPEEDHACK :				// ���ǵ���
		{
			gAttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo", "AttackSpeedTimeLimit", 800, gDirPath.GetNewPath("commonserver.cfg"));
			bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, gDirPath.GetNewPath("commonserver.cfg"));		
			gHackCheckCount = GetPrivateProfileInt("GameServerInfo", "HackCheckCount", 5.5, gDirPath.GetNewPath("commonserver.cfg"));	
			gMinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo", "MinimumAttackSpeedTime", 200, gDirPath.GetNewPath("commonserver.cfg"));	
			gDetectedHackKickCount = GetPrivateProfileInt("GameServerInfo", "DetectedHackKickCount", 10, gDirPath.GetNewPath("commonserver.cfg"));	
			gIsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickDetecHackCountLimit", 0, gDirPath.GetNewPath("commonserver.cfg"));	

#ifdef CHINA_HACKUSER_KICK_SYSTEM
	ChinaHackLogFile.Output("[HLS] AttackSpeedTimeLimit = %d", gAttackSpeedTimeLimit);
	ChinaHackLogFile.Output("[HLS] IsIgnorePacketHackDetect = %d", bIsIgnorePacketSpeedHackDetect);
	ChinaHackLogFile.Output("[HLS] HackCheckCount = %d", gHackCheckCount);
	ChinaHackLogFile.Output("[HLS] MinimumAttackSpeedTime = %d", gMinimumAttackSpeedTime);
	ChinaHackLogFile.Output("[HLS] DetectedHackKickCount = %d", gDetectedHackKickCount);
	ChinaHackLogFile.Output("[HLS] IsKickDetecHackCountLimit = %d", gIsKickDetecHackCountLimit);

	ChinaHackLogFile.Output("[HLS] NotAttackAreaKickCount = %d", gNotAttackAreaKickCount);
	ChinaHackLogFile.Output("[HLS] IsKickNotAttackAreaCountLimit = %d", gIsKickNotAttackAreaCountLimit);

	ChinaHackLogFile.Output("[HLS] BlockHackUserCharacter = %d", gBlockHackUserCharacter);
	ChinaHackLogFile.Output("[HLS] BlockHackUserAccount = %d", gBlockHackUserAccount);
#endif
		}
		break;
	case MU_ETC_GAMEGUARD :				// ���Ӱ��� üũ�� üũ����
		{
			gUseNPGGChecksum = GetPrivateProfileInt("GameServerInfo", "UseNPGGChecksum", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	}

}


#endif

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
void RenderCommonServerInfo()
{
	// �ش� �̺�Ʈ�� commonserverinfo���� �̺�Ʈ�� Ȱ��ȭ �Ǿ 
	// �̺�Ʈ�� �������� �ʴ´�
	// g_bEventOff ("Serverinfo.dat", "EventOff")
	if (g_bEventOff == TRUE)
	{
		// ����� ���� �̺�Ʈ
#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
		g_bLuckyCoinEventOn					= FALSE;
		g_iLuckyCoinDropRate				= 0;
#endif // UPDATE_LUCKY_COIN_EVENT_20081029
		
		// 20. ũ�������� �������� �̺�Ʈ( RKFKA )
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
		g_bRibbonBoxEvent					= FALSE;
		
		g_iRedRibbonBoxDropLevelMin			= 0;
		g_iRedRibbonBoxDropLevelMax			= 0;
		g_iRedRibbonBoxDropRate				= 0;
		g_iRedRibbonBoxDropZenRate			= 0;
		g_iRedRibbonBoxDropZen				= 0;
		
		g_iGreenRibbonBoxDropLevelMin		= 0;
		g_iGreenRibbonBoxDropLevelMax		= 0;
		g_iGreenRibbonBoxDropRate			= 0;
		g_iGreenRibbonBoxDropZenRate		= 0;
		g_iGreenRibbonBoxDropZen			= 0;
		
		g_iBlueRibbonBoxDropLevelMin		= 0;
		g_iBlueRibbonBoxDropLevelMax		= 0;
		g_iBlueRibbonBoxDropRate			= 0;
		g_iBlueRibbonBoxDropZenRate			= 0;
		g_iBlueRibbonBoxDropZen				= 0;
#endif //#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
		
		// ���� ���ָӴ� ���� �̺�Ʈ
#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131
		g_bNewYearLuckyBagMonsterEventOn	= FALSE;
#endif // UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131
		
		// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ
#ifdef VALENTINES_DAY_BOX_EVENT_20060124
		g_bChocolateBoxEvent				= FALSE;
		g_iPinkChocolateBoxDropLevelMin		= 0;
		g_iPinkChocolateBoxDropLevelMax		= 0;
		g_iPinkChocolateBoxDropRate			= 0;
		g_iPinkChocolateBoxDropZenRate		= 0;
		g_iPinkChocolateBoxDropZen			= 0;
		
		g_iRedChocolateBoxDropLevelMin		= 0;
		g_iRedChocolateBoxDropLevelMax		= 0;
		g_iRedChocolateBoxDropRate			= 0;
		g_iRedChocolateBoxDropZenRate		= 0;
		g_iRedChocolateBoxDropZen			= 0;
		
		g_iBlueChocolateBoxDropLevelMin		= 0;
		g_iBlueChocolateBoxDropLevelMax		= 0;
		g_iBlueChocolateBoxDropRate			= 0;
		g_iBlueChocolateBoxDropZenRate		= 0;
		g_iBlueChocolateBoxDropZen			= 0;
#endif // VALENTINES_DAY_BOX_EVENT_20060124
		
		// ȭ��Ʈ���� �������� �̺�Ʈ
#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
		g_bCandyBoxEvent					= FALSE;
		
		g_iLightPurpleCandyBoxDropLevelMin	= 0;
		g_iLightPurpleCandyBoxDropLevelMax	= 0;
		g_iLightPurpleCandyBoxDropRate		= 0;
		g_iLightPurpleCandyBoxDropZenRate	= 0;
		g_iLightPurpleCandyBoxDropZen		= 0;
		
		g_iVermilionCandyBoxDropLevelMin	= 0;
		g_iVermilionCandyBoxDropLevelMax	= 0;
		g_iVermilionCandyBoxDropRate		= 0;
		g_iVermilionCandyBoxDropZenRate		= 0;
		g_iVermilionCandyBoxDropZen			= 0;
		
		g_iDeepBlueCandyBoxDropLevelMin		= 0;
		g_iDeepBlueCandyBoxDropLevelMax		= 0;
		g_iDeepBlueCandyBoxDropRate			= 0;
		g_iDeepBlueCandyBoxDropZenRate		= 0;
		g_iDeepBlueCandyBoxDropZen			= 0;
#endif //#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
	}
}
#endif // ADD_NEWSERVER_EVENTOFF_20081114
