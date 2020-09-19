
//----------------------------------------------------------------------------
// GAME SERVER - GAMEMAIN.CPP
// 게임서버의 메인 모듈
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
//	#include "Include\ggsrv.h"			// 클라이언트 인증 서버용
	#include "ggsrv.h"						// 기존 루트와 맞춤 (b4nfter)
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	#include "ggsrv25.h"						// 기존 루트와 맞춤 (hnine)
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

#ifdef MAP_SERVER_WORK_20041030			// 맵서버 분할 관리 클래스
#include "MapServerManager.h"
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// 공성서버 만 공성정보를 읽음
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

// 맵 이름들을 정의
#ifdef FOR_BLOODCASTLE
CHAR	g_szMapName [MAX_MAP][32] = {
	"로랜시아",
	"던전",
	"데비아스",
	"노리아",
	"로스트 타워",
	"로스트 타워",
	"경기장",
	"아틀란스",
	"타르칸",
	"데빌스퀘어",
	"이카루스",
	"블러드캐슬1",
	"블러드캐슬2",
	"블러드캐슬3",
	"블러드캐슬4",
	"블러드캐슬5",
	"블러드캐슬6",
};
#else
	#ifdef NEW_SKILL_FORSKYLAND
CHAR	g_szMapName [MAX_MAP][32] = {
	"로랜시아",
	"던전",
	"데비아스",
	"노리아",
	"로스트 타워",
	"로스트 타워",
	"경기장",
	"아틀란스",
	"타르칸",
	"데빌스퀘어",
	"이카루스",
};
	#else
CHAR	g_szMapName [MAX_MAP][32] = {
	"로랜시아",
	"던전",
	"데비아스",
	"노리아",
	"로스트 타워",
	"로스트 타워",
	"경기장",
	"아틀란스",
	"타르칸",
	"데빌스퀘어",
};
	#endif
#endif



char *szGameServerVersion	= GAMESEVER_VERSION;		// 게임서버 버전

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

#ifdef GAMESERVER_INFO_TO_CSS_20040317			// 접속서버분산 - 변수선언
#include <IO.H>									// _access()

#define			CSLIST_FILENAME					"CSList.dat"	// 접속서버 리스트를 가진 파일의 이름

WzUdp *			gpUdpSocCSS[MAX_CS_COUNT]		= {NULL,};		// 접속서버에 데이터를 보낼 UDP 소켓
CHAR			g_szCSIPADDR[MAX_CS_COUNT][32]	= {0,};			// 접속서버의 주소 리스트
WORD			g_wCSPORT[MAX_CS_COUNT]			= {0,};			// 접속서버의 포트 리스트
int				g_iConnectServerCount			= 0;			// 접속서버의 개수
BOOL			g_bSendGSInfoToCSS				= false;		// 분산된 접속서버에 GS데이터 보내기를 사용할지 여부 (FALSE이면 기존방식으로 한군데만 보낸다.)

VOID			GSINFO_INITCSLIST();				// 접속서버의 리스트를 읽고 소켓을 초기화한다.
#endif


#ifdef _NEW_EXDB_
//##EXDB##
wsJoinServerCli wsExDbCli;
//##EXDB##
#endif


wsJoinServerCli wsRServerCli;			//  랭킹서버

wsJoinServerCli wsEvenChipServerCli;	//  이벤트 칩 서버

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
CItemBag	*   LuckboxItemBag	= NULL;	// 행운의상자 아이템 가방(?)
CItemBag	*   Mon55			= NULL;	// 로랜시아 보스 아이템 가방(?)
CItemBag	*   Mon53			= NULL;	// 데비아스 보스  아이템 가방(?)

#ifdef EVENT_ITEMBAG_UPGRADE
CItemBagEx	*   StarOfXMasItemBag = NULL;	// 성탄의 별 아이템 가방(?)
#else
CItemBag	*   StarOfXMasItemBag = NULL;	// 성탄의 별 아이템 가방(?)
#endif

CItemBag	*   FireCrackerItemBag = NULL;	// 폭죽
CItemBag	*   HeartOfLoveItemBag = NULL;	// 사랑의 하트


CItemBag	*   GoldMedalItemBag = NULL;	// 금훈장
CItemBag	*   SilverMedalItemBag = NULL;	// 은훈장

CItemBag	*   EventChipItemBag = NULL;	// 이벤트 칩

#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
CProbabilityItemBag	*   GoldGoblenItemBag = NULL;	//	황금고블린
CProbabilityItemBag	*   TitanItemBag = NULL;		//	타이탄
CProbabilityItemBag	*   GoldDerconItemBag = NULL;	//  황금데르콘
CProbabilityItemBag	*   DevilLizardKingItemBag = NULL;	// 데블 리자드킹
CProbabilityItemBag	*   KanturItemBag = NULL;	// 카투르
#else //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
CItemBag	*   GoldGoblenItemBag = NULL;	//	황금고블린
CItemBag	*   TitanItemBag = NULL;		//	타이탄
CItemBag	*   GoldDerconItemBag = NULL;	//  황금데르콘
CItemBag	*   DevilLizardKingItemBag = NULL;	// 데블 리자드킹
CItemBag	*   KanturItemBag = NULL;	// 카투르
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04

#ifdef RING_EVENT_ITEMBOX_20031124
CItemBag	*   RingEventItemBag = NULL;			// 반지의제왕 이벤트
#endif

#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211
CItemBag	*   FriendShipItemBag = NULL;			// 내친구 이벤트 - 제왕의반지
#endif

#ifdef DARK_LORD_HEART_EVENT_20040810
CItemBag	*   DarkLordHeartItemBag = NULL;	// 다크로드의 마음 이벤트
#endif

#ifdef MODIFY_KUNDUN_REWARD_01_20040915
CItemBagEx	*	KundunEventItemBag = NULL;	// 쿤둔 이벤트
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303
CItemBagEx	*	CastleHuntZoneBossItemBag = NULL;		// 성 사냥터 보스(에로힘)
#endif

#ifdef MU_CHN_THANKS_COOKIE_EVENT_20040908 
CItemBagEx  *	ChnThanksCookieEvent = NULL;	// 중국 월병 이벤트
#endif

#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
INT				g_iCastleItemMixLimit	= CASTLE_ITEM_MIX_MAXCOUNT;	// 성주혜택 특별 아이템 조합 하루 제한 횟수
CItemBagEx	*	CastleItemMixItemBag	= NULL;						// 성주혜택 특별 아이템 조합
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
CItemBagEx *	HiddenTreasureBoxItemBag	= NULL;			// 숨겨진 보물상자 아이템
#endif

#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212						// 크리스마스 리본상자 이벤트
CItemBagEx *	RedRibbonBoxEventItemBag	= NULL;				// 빨간리본상자 
CItemBagEx *	GreenRibbonBoxEventItemBag	= NULL;				// 초록리본상자 
CItemBagEx *	BlueRibbonBoxEventItemBag	= NULL;				// 파란리본상자
#endif

#ifdef VALENTINES_DAY_BOX_EVENT_20060124						// 발렌타인데이 초콜릿상자 이벤트
CItemBagEx *	PinkChocolateBoxEventItemBag	= NULL;			// 분홍초콜릿상자 
CItemBagEx *	RedChocolateBoxEventItemBag		= NULL;			// 빨간초콜릿상자 
CItemBagEx *	BlueChocolateBoxEventItemBag	= NULL;			// 파란초콜릿상자
#endif

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222						// 화이트데이 사탕상자 이벤트
CItemBagEx *	LightPurpleCandyBoxEventItemBag	= NULL;			// 연보라사탕상자 
CItemBagEx *	VermilionCandyBoxEventItemBag	= NULL;			// 주홍사탕상자 
CItemBagEx *	DeepBlueCandyBoxEventItemBag	= NULL;			// 감청사탕상자
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
CItemBagEx *	CrywolfDarkElfItemBag			= NULL;			// 크라이울프 다크엘프 아이템 백
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
CItemBagEx *	CrywolfBossMonsterItemBag		= NULL;			// 크라이울프 다크엘프 아이템 백
#endif

#ifdef KANTURU_MONSTER_ITEMDROP_20060807
CItemBagEx *	KanturuMayaHandItemBag			= NULL;			// 칸투르 마야 손 아이템 백
CItemBagEx *	KanturuNightmareItemBag			= NULL;			// 칸투르 나이트메어 아이템 백
#endif

#ifdef ADD_RAKLION_20080408
CProbabilityItemBag*	pRaklionSelupanItemBag	= NULL;			// 라클리온 세루판 아이템 백
#endif // ADD_RAKLION_20080408

#ifdef ADD_GAMBLING_20090120
CGamblingItemBag*	pGamblingSystemItemBag	= NULL;			// 겜블링 시스템 아이템 백
#endif // ADD_GAMBLING_20090120

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
CItemBagEx *	HallowinDayEventItemBag			= NULL;			// 할로윈 데이 이벤트 아이템 백
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119
CItemBag*	RingOfHeroEventItemBag				= NULL;			// New 용사의 반지 아이템 백
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
CItemBagEx*		PCBangEventNPCItemBag;					// PC방 이벤트 - NPC가 지급하는 아이템 목록을 ItemBag으로 교체
#endif

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131				// 설날복주머니 아이템 가방
CProbabilityItemBag*	NewYearLuckyBagItemBag;
#endif

#ifdef UPDATE_GM_FUNCTION_20070228							// GM 선물 상자 아이템 가방
CProbabilityItemBag*	GMPresentBoxItemBag;
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206						// 혼돈 상자 아이템 백
CProbabilityItemBag*	BoxOfGreenChaosItemBag;		// 초록혼돈 상자
CProbabilityItemBag*	BoxOfRedChaosItemBag;		// 빨강혼돈 상자
CProbabilityItemBag*	BoxOfPurpleChaosItemBag;	// 보라혼돈 상자
#endif

#ifdef UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408	// 레오 
CProbabilityItemBag*	ItemGiveReoEventItemBag;	// 레오 신규 아이템백
#else	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// 게임 내 아이템 지급 NPC - 레오 이벤트백 추가
CItemBagEx*		ItemGiveEventItemBag;
#endif
#endif	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// 환영 사원 몬스터 아이템 백
CProbabilityItemBag* IllusionTempleGhostItemBagA;	// 1~2까지의 아이템백
CProbabilityItemBag* IllusionTempleGhostItemBagB;	// 3~4까지의 아이템백
CProbabilityItemBag* IllusionTempleGhostItemBagC;	// 5~6까지의 아이템백
#endif

#ifdef CHUSEOK_MONSTER_EVENT_20070823	// 추석 몬스터 이벤트 아이템 백
CProbabilityItemBag*	ChuseokMonsterEventItemBag;
#endif

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// NPC 앨런 아이템 백
CProbabilityItemBag*	NpcAlanEventItemBag;
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
CProbabilityItemBag*	pCherryBlossomEventItemBag;
CProbabilityItemBag*	pCherryBlossomEventItemBagA;
CProbabilityItemBag*	pCherryBlossomEventItemBagB;
CProbabilityItemBag*	pCherryBlossomEventItemBagC;
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_GIFTITEM_XMASEVENT_20081030
CProbabilityItemBag* pSantaNpcEventItem;    // 매일 매일 하루에 한번씩 ~
CProbabilityItemBag* pSantaNpcEventItem100; // 100번째 방문자용 아이템백
CProbabilityItemBag* pSantaNpcEventItem1000;  // 1000번째 방문자용 아이템백 
#endif //ADD_GIFTITEM_XMASEVENT_20081030

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// 행운의 동전 교환 아이템 백
CProbabilityItemBag*	pLuckyCoinEventItemBag10 = NULL;
CProbabilityItemBag*	pLuckyCoinEventItemBag20 = NULL;
CProbabilityItemBag*	pLuckyCoinEventItemBag30 = NULL;
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610	// 여름 불꽃 유령 이벤트 아이템 백
CProbabilityItemBag*	FireCrackerMonsterEventItemBag;
#endif // ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

CMsg			lMsg;	// 공용 메시지

NSerialCheck	gNSerialCheck[MAX_OBJECT]; 


CLoginCount	    gLCount[MAX_LOGIN_COUNT];

#ifdef HERO_SYSTEM_MODIFY
DWORD			gLevelExperience[MAX_LEVEL+1];					// 레벨당 경험치
#else
DWORD			gLevelExperience[MAX_LEVEL+1];					// 레벨당 경험치
#endif

//char			szClientVersion[3] = {'0','0','0'};				// 게임서버에 접속 가능한 클라이언트 버젼
char			szClientVersion[5] = {'0','0','0','0','0'};		// 게임서버에 접속 가능한 클라이언트 버젼

char			szGameServerExeSerial[21];						// 게임서버에 접속 가능한 시리얼 번호
char			szServerName[51];								// 현재 서버의 이름

// 한국 이름
char	szKorItemTextFileName[256];					// 아이템 정보 텍스트 파일 이름
char	szKorSkillTextFileName[256];				// 스킬 정보 텍스트 파일 이름

char			szItemTextFileName[256];							// 아이템 정보 텍스트 파일 이름
char			szSkillTextFileName[256];						// 스킬 정보 텍스트 파일 이름

char			szQuestTextFileName[256];						// 스킬 정보 텍스트 파일 이름

char			szMoveReqFileName[255];

char			szCommonlocIniFileName[256];					// 국가별 ini 파일 정보 파일 이름
BOOL			SpeedHackPlayerBlock=0;							// 스피드핵 사용자를 블럭시킬것인지
BOOL			bCanTrade =	TRUE;								// 거래를 가능하게 할건지..
BOOL			bCanChaosBox = TRUE;							// 카오스 박스 사용 가능하게 할건지..
BOOL			bCanConnectMember = FALSE;						// 접속 가능한 멤버를 구성할 것인지..

BOOL			bCanWarehouseLock = TRUE;						// 창고 잠금 기능을 사용할 것인가 말것인가?


#ifdef AUTH_GAMESERVER
char			szAuthKey[20];									// 게임 서버 인증을 위한 검사키 (코드 곳곳에서 체크 한다)
int				gServerType = 0;								// 	0:MainServer, 1:TestServer, 2:Internal Server
int				gPartition = 0;								// 	0:MainServer, 1:TestServer, 2:Internal Server
#endif

#ifdef DARKLORD_WORK
BOOL			gIsDropDarkLordItem;
int				gSleeveOfLordDropRate;		// 군주의 소매 드롭 확률
int				gSleeveOfLordDropLevel;		// 군주의 소매 드롭 레벨

int				gSoulOfDarkHorseDropRate;	// 다크호스의 영혼 드롭 확률
int				gSoulOfDarkHorseropLevel;	// 다크호스의 영혼 드롭 레벨

int				gSoulOfDarkSpiritDropRate;	// 다크스피릿 드롭 확률
int				gSoulOfDarkSpiritDropLevel;	// 다크스피릿 드롭 레벨

float			gDarkSpiritAddExperience;
#endif

#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
int				gIsDropGemOfDefend;			// 수호보석 드랍여부
int				gGemOfDefendDropRate;		// 수호보석 드랍 확률 (10000 기준)
int				gGemOfDefendDropLevel;		// 수호보석 드랍 몬스터 레벨
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314
int				gIsDropSetItemInCastleHuntZone;		// 시련의 땅에서 세트아이템 드랍여부
int				gSetItemInCastleHuntZoneDropRate;	// 시련의 땅에서 세트아이템 드랍확률 (10000 기준)
int				gSetItemInCastleHuntZoneDropLevel;	// 시련의 땅에서 세트아이템 드랍 몬스터 레벨
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// 중국추가> . 피로도 시스템 적용여부, 경험치, 아이템확률 읽음
BOOL			g_bChnPlayTimeLimitOn;				// 피로도 시스템 적용여부
INT				g_iChnPlayTimeLimitLevel1Exp;		// 피로시간 경험치 획득률
INT				g_iChnPlayTimeLimitLevel1ItemDrop;	// 피로시간 아이템 드롭률
INT				g_iChnPlayTimeLimitLevel2Exp;		// 유해시간 경험치 획득률
INT				g_iChnPlayTimeLimitLevel2ItemDrop;	// 유해시간 아이템 드롭률
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// 베트남 옵션 데이터
BOOL			g_bVtmPlayTimeLimitOn;				// 피로도 시스템 적용여부
INT				g_iVtmPlayTimeLimitNoticeTime;		// 기본 공지 시간

INT				g_iVtmPlayTimeLimitLevel1Time;		// 피로시간 체크시간 (min)
INT				g_iVtmPlayTimeLimitLevel1NoticeTime;// 피로시간 공지시간 (min)	
INT				g_iVtmPlayTimeLimitLevel1Exp;		// 피로시간 경험치 획득률
INT				g_iVtmPlayTimeLimitLevel1ItemDrop;	// 피로시간 아이템 드롭률

INT				g_iVtmPlayTimeLimitLevel2Time;		// 유해시간 체크시간 (min)
INT				g_iVtmPlayTimeLimitLevel2NoticeTime;// 유해시간 공지시간 (min)
INT				g_iVtmPlayTimeLimitLevel2Exp;		// 유해시간 경험치 획득률
INT				g_iVtmPlayTimeLimitLevel2ItemDrop;	// 유해시간 아이템 드롭률
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
	int				g_iApplyPCBangAdvantage = 1;	// 기본값: 대만 혜택 적용
#else
	int				g_iApplyPCBangAdvantage = 0;	// 기본값: 한국 혜택 적용 않함
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
int			g_iSkillDistanceCheck;		// 스킬 공격 거리 체크	0 : 체크 안함, 1 : 체크 함
int			g_iSkillDistanceCheckTemp;	// 스킬 공격 거리 체크 시 여유 값 : defalut 2로 설정
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
int			g_iSkillDistanceKick;		// 스킬 거리 핵 사용자 강제 종료 여부 0:강제종료 안함, 1:강제종료
int			g_iSkillDistanceKickCount;	// 스킬 거리 핵 강제 종료 카운트.
#endif
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421
int			g_iSkillDiatanceKickCheckTime;	// 스킬 거리 핵 사용자 유효 카운트 시간 
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

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131		//이벤트 온/오프
BOOL		g_bNewYearLuckyBagMonsterEventOn;
#endif

#ifdef ADD_THIRD_WING_20070525	// 콘도르의불꽃 드롭확률 
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

/*int				MapMinUserLevel[MAX_MAP]={						// 맵에 진입할수 있는 최소 유저 레벨
						0,			//	로랜시아
						20,			//	던전
						15,			//	데비아스
						10,			//	노리아
						80,			//	로스트타워1
						0,			//	로스트타워2
						0,			//	로스트타워3
						60,			//	로스트타워4
						130,		//	로스트타워5
						0,			//	데빌스퀘어
						160,		//	천공
						10,			//	블러드캐슬1		(마검사 기준)
						36,			//	블러드캐슬2		(마검사 기준)
						80,			//	블러드캐슬3		(마검사 기준)
						130,		//	블러드캐슬4		(마검사 기준)
						170,		//	블러드캐슬5		(마검사 기준)
						210,		//	블러드캐슬6		(마검사 기준)
						310,		//	블러드캐슬7		(마검사 기준)
						15,			//	카오스 캐슬1	(마검사 기준)
						30,			//	카오스 캐슬2	(마검사 기준)
						100,		//	카오스 캐슬3	(마검사 기준)
						160,		//	카오스 캐슬4	(마검사 기준)
						220,		//	카오스 캐슬5	(마검사 기준)
						280,		//	카오스 캐슬6	(마검사 기준)
#ifdef ADD_NEW_MAP_KALIMA_20040518
						0,			//	칼리마 1
						0,			//	칼리마 2 
						0,			//	칼리마 3
						0,			//	칼리마 4
						0,			//	칼리마 5
						0,			//	칼리마 6
#endif
#ifdef ADD_NEW_MAP_CASTLE_20041115
						10,			//	성 맵 (진입 최소레벨 없음)
#endif
#ifdef ADD_NEW_MAP_CASTLE_HUNTZONE_20041115
						10,
#endif
#ifdef DEVILSQUARE_EXTEND_20050221	// 데빌스퀘어 맵추가
						0,
#endif
#ifdef ADD_NEW_MAP_AIDA_20050617
						0,			// 아이다 맵 추가
#endif
#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
						0,			// 크라이울프 1차 맵 추가
#endif
#ifdef ADD_NEW_MAP_CRYWOLF_SECONDZONE_20050414
						0,			// 크라이울프 2차 맵 추가
#endif
#ifdef HIDDEN_KALIMA_20050706
						0,			// 히든 칼리마 추가
#endif
#ifdef ADD_NEW_MAP_KANTURU_COMMON_20060523
						150,			// 칸투르 1차 맵 추가
						220,			// 칸투르 2차 맵 추가
#endif
#ifdef ADD_NEW_MAP_KANTURU_BOSS_20060627
						0,				// 칸투르 보스맵 추가						
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3차 전직 맵 추가
						0,				// GM 소환 지역
						380,			// 3차 전직 퀘스트 맵
						380,			// 3차 전직 퀘스트 보스 맵
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// 엘베란드 맵 추가
						0,				// 사용안함
						0,				// 사용안함

						220,			// 제 1 환영 사원
						271,			// 제 2 환영 사원
						321,			// 제 4 환영 사원
						351,			// 제 4 환영 사원
						380,			// 제 5 환영 사원
						400,			// 제 6 환영 사원

						10,				// 엘베란드
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬8 입장 가능 레벨 추가
						400,
#endif		
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬7 입장 가능 레벨 추가
						400,
#endif		
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108		// 평온의늪 입장 가능레벨 추가
						400,
#endif							
#ifdef ADD_RAKLION_20080408							// 라클리온 입장 가능 레벨 추가
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
int	g_ConServerInfoSendType=0;		// 접속서버에 정보를 보낼 타입을 설정 (0:UDP / 1:TCP)
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

// 성탄의 별 이벤트
BOOL gXMasEvent = 0;		// 성탄의 별 이벤트 진행하는가?
int	 g_XMasEvent_StarOfXMasDropRate = 5000;		// 성탄의 별이 떨어질 확률
int	 g_XMasEvent_ItemDropRateForStarOfXMas = 2; // 성탄의 별에서 아이템이 나올 확률


// 폭죽 이벤트
BOOL gFireCrackerEvent = 0;	// 폭죽 이벤트 진행하는가?
int	 gFireCrackerDropRate = 5000;	// 폭죽이 떨어질 확률
int	 g_ItemDropRateForgFireCracker = 2;	// 폭죽에서 아이템이 나올 확률

int  gOnlyFireCrackerEffectUse;	// 폭죽을 떨어뜨렸을때, 효과만 나오게 한다


// 사랑의 하트 이벤트
BOOL gHeartOfLoveEvent = 0;	// 사랑의 하트 이벤트 진행하는가?
int	 gHeartOfLoveDropRate = 5000;	// 사랑의 하트이 떨어질 확률
int	 g_ItemDropRateForgHeartOfLove = 2;	// 사랑의 하트에서 아이템이 나올 확률


// 메달 이벤트
BOOL gMedalEvent = 0;	// 메달 이벤트 진행하는가?
int	 gGoldMedalDropRate = 2;	// 금메달이 떨어질 확률
int	 gSilverMedalDropRate = 2;	// 은메달이 떨어질 확률

int	 g_ItemDropRateForGoldMedal = 2;	// 금메달에서 아이템이 나올 확률
int	 g_ItemDropRateForSilverMedal = 2;	// 은메달에서 아이템이 나올 확률

// 이벤트칩 이벤트
BOOL gEventChipEvent = 0;					// 메달 이벤트 진행하는가?
int	 gBoxOfGoldDropRate = 2;				// 금메달이 떨어질 확률
int	 g_ItemDropRateForBoxOfGold = 2;		// 금메달에서 아이템이 나올 확률
int	 g_EventChipDropRateForBoxOfGold = 50;	// 금메달에서 이벤트 칩이 나올 확률

#ifdef ADD_GIFTITEM_XMASEVENT_20081030
int	 gXmasVisitorCount = 0;           // 크리스마스 이벤트 방문 카운트 
short gXmasGiftForTest = 0;  // 0 - normal , 1 - 무조건 첫방문 , 2 - 무조건 첫방문 이후 방문 
#endif //ADD_GIFTITEM_XMASEVENT_20081030

// 악마의 광장 이벤트
BOOL gDevilSquareEvent = 0;
// 악마의 눈 나올 확률
int	 gEyesOfDevilSquareDropRate = 2;
// 악마의 열쇠 나올 확률
int	 gKeyOfDevilSquareDropRate = 3;

#ifdef DEVILSQUARE_EXTEND_20050221		// 카오스 조합 추가
int	 gDQChaosSuccessRateLevel1 = 75;
int	 gDQChaosSuccessRateLevel2 = 70;
int	 gDQChaosSuccessRateLevel3 = 65;
int	 gDQChaosSuccessRateLevel4 = 60;
int	 gDQChaosSuccessRateLevel5 = 55;
int	 gDQChaosSuccessRateLevel6 = 50;
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// 데빌스퀘어7 입장권 카오스조합 확률
int	 gDQChaosSuccessRateLevel7 = 45;	
#endif
#else
int	 gDQChaosSuccessRateLevel1 = 40;
int	 gDQChaosSuccessRateLevel2 = 40;
int	 gDQChaosSuccessRateLevel3 = 30;
int	 gDQChaosSuccessRateLevel4 = 30;
#endif

BOOL gWriteSkillLog = 0;

// 블러드 캐슬
#ifdef FOR_BLOODCASTLE
BOOL			g_bBloodCastle = TRUE;					// 원래 파일에서 읽어야 함
int				g_iBloodCastle_StartHour = 1;			// 1:1시간마다 (기본) / 2:2시간마다

BOOL			g_bStoneItemDrop = 0;
int				g_iStoneDropRate = 50;
int				g_iAngelKingsPaperDropRate = 10;
int				g_iBloodBoneDropRate = 20;

#ifdef UPDATE_BLOODECASTLE_SCADULE_20070702
int				g_iBloodCastle_odd_Even_Hour	= 0;	// 짝수 또는 홀수 시간대 시작 (0:짝수 시간대, 1: 홀수 시간대)
#endif

#endif

// 카오스 캐슬
#ifdef CHAOSCASTLE_SYSTEM_20040408
BOOL			g_bChaosCastle = TRUE;					// 원래 파일에서 읽어야 함
#endif

#ifdef MONSTER_HERD_SYSTEM_20031120
BOOL			g_bDoRingEvent = 0;						// 반지 이벤트를 할 것인가 ?
INT				g_iRingOrcKillGiftRate;					// 오크를 잡았을 때 경품을 탈 확률 (1/n)
INT				g_iRingDropGiftRate;					// 반지를 던졌을 때 경품을 탈 확률 (1/n)
#endif

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
BOOL			g_bDoXMasEvent = 0;						// 크리스마스(산타와 친구들) 이벤트 유/무
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
BOOL			g_bDoCastleDeepEvent	= 0;			// 로랜협곡 습격 이벤트를 할 것인가?
#endif

#ifdef EVENT_MANAGEMENT_20040711
BOOL			g_bEventManagerOn = 0;
#endif

// 칼리마 맵
#ifdef ADD_NEW_MAP_KALIMA_20040518
INT				g_iKundunMarkDropRate		= 0;		// 쿤둔의 표식 드롭확률
#endif

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214					// 성주의 표식 드롭확률
INT				g_iMarkOfTheLord			= 0;
#endif

#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531
int				g_iJapan1StAnivItemDropRate = 0;		// 일본 1주년 기념 아이템 드롭확률
#endif

#ifdef DARK_LORD_HEART_EVENT_20040810					// 다크로드의 마음 관련 확률변수들
INT				g_iDarkLordHeartDropRate = 0;			// 다크로드의 마음 드롭확률
INT				g_iDarkLordHeartOffEventRate = 0;		// 다크로드의 마음 오프라인 경품당첨 확률
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
INT				g_iMysteriousBeadDropRate1 = 0;			// 신비의 구슬 드롭확률 (아이다, 크라이울프 저렙몹)
INT				g_iMysteriousBeadDropRate2 = 0;			// 신비의 구슬 드롭확률 (아이다, 크라이울프 고렙몹)
INT				g_iHiddenTreasureBoxOfflineRate = 0;	// 숨겨진 보물상자 이벤트 오프라인 경품확률
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029							// 행운의 동전 드롭 확률
int				g_bLuckyCoinEventOn		= 0;	// 행운의 동전 이벤트 여부
int				g_iLuckyCoinDropRate	= 0;	// 행운의 동전 드롭 확률
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

// 공격속도 체크 제한 시간
DWORD		gAttackSpeedTimeLimit = 500;
// 스피드 핵 체크에 거린 패킷은 무시 한다
int			bIsIgnorePacketSpeedHackDetect = FALSE;
DWORD		gHackCheckCount = 5;
float		gDecTimePerAttackSpeed = 5.33f;
int			gMinimumAttackSpeedTime = 200;			// 최소 공격 시간
int			gDetectedHackKickCount = 10;			// 스피드핵 감지시 퇴출시킬 카운트
int			gIsKickDetecHackCountLimit = 0;			// 스피드핵 감지시 퇴출시키는가?

#ifdef FOR_JAPAN
BOOL			gTamaJJangEvent	= FALSE;					// 타마짱 이벤트를 진행하는가?
BOOL			gAppearTamaJJang = FALSE;					// 타마짱이 등장하는가?
int				gTamaJJangKeepTime = 100;					// 타마짱 지속시간


int				gTamaJJangDisappearTime = 1800;				// 타마짱 사라지는 시간
int				gTamaJJangDisappearTimeRandomRange = 1000;	// 타마짱이 사라지는 시간을 랜덤으로 설정하기 위한 영역

int				gTamaJJangTime = 0;
#endif

#ifdef CHECK_ATTACK_AREA
int				gNotAttackAreaKickCount;
int				gIsKickNotAttackAreaCountLimit;
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
int				gBlockHackUserCharacter;					// 핵유저 캐릭 블럭 여부
int				gBlockHackUserAccount;						// 핵유저 게정 블럭 여부
#endif

#ifdef ITEMDROP_RINGOFTRANSFORM	
int				gIsItemDropRingOfTransform = 0;				// 변신 반지를 떨어뜨리는가?
int				gItemDropRingOfTransform = 1;				// 변신 반지 드랍 확률 gItemDropRingOfTransform/10000
#endif


int				gQuestNPCTeleportTime = 60;

CSimpleModulus g_SimpleModulusCS;
CSimpleModulus g_SimpleModulusSC;

extern int		gCurPaintPlayer;
extern int		gObjTotalUser;
extern int		gObjMonCount;

// 엘도라도의 상자 이벤트
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

int				gEledoradoGoldGoblenItemDropRate;		// 아이템 드랍 확률
int				gEledoradoTitanItemDropRate;
int				gEledoradoGoldDerconItemDropRate;
int				gEledoradoDevilLizardKingItemDropRate;
int				gEledoradoDevilTantarosItemDropRate;

int				gEledoradoGoldGoblenExItemDropRate;		// 엑설런트 아이템 드랍확률
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

#ifdef PERSONAL_SHOP_20040113	// 개인상점 개설 여부
int				gDoPShopOpen = 0;
#endif

#ifdef MU_CHN_OFFLINE_GIFT_EVENT_20040908
BOOL			gbChnOfflineGiftEvent = FALSE;			// 중국 오프라인 경품 이벤트를 하는가 ?
int				giChnOfflineGiftEventDropRate = 0;		// 중국 오프라인 경품 드롭 확률 (1/1000000)
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
int				giKundunRefillHPSec;							// 쿤둔 피 초당회복량 
int				giKundunRefillHP;								// 쿤둔 피 회복량 
int				giKundunRefillHPTime;							// 피 회복 시간 (SEC)
#endif

#ifdef EXTEND_LOG_SYSTEM_20060202
int				giKundunHPLogSaveTime;
#endif

// 젠 드롭 시간
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
// CODEREVIEW_SHIELD : Commonserver.cfg 테스트
BOOL			g_ShieldSystemOn;
int				g_iDamageDevideToSDRate;
int				g_iDamageDevideToHPRate;
float			g_fSuccessAttackRateOption;
int				g_iSDChargingOption;
int				g_iConstNumberOfShieldPoint;

BOOL			g_ShieldAutoRefillOn;
BOOL			g_ShieldAutoRefillOnSafeZone;
BOOL			g_PKLevelIncreaseOff;

// CODEREVIEW_SHIELD3 : 복합 물약 드롭 확률 추가
BOOL			g_CompoundPotionDropOn;				// 복합 물약 드롭 On(1)/Off(0)
int				g_iCompoundPotionLv1DropRate;		// 작은 복합 물약 드롭 확률 / 10000
int				g_iCompoundPotionLv2DropRate;		// 중간 복합 물약 드롭 확률 / 10000
int				g_iCompoundPotionLv3DropRate;		// 큰 복합 물약 드롭 확률 / 10000
int				g_iCompoundPotionLv1DropLevel;		// 작은 복합 물약 드롭 레벨
int				g_iCompoundPotionLv2DropLevel;		// 중간 복합 물약 드롭 레벨
int				g_iCompoundPotionLv3DropLevel;		// 큰 복합 물약 드롭 레벨

	#ifdef UPDATE_SHIELD_OPTION_20060427
		BOOL	g_bShieldComboMissOptionOn;			// PvP 공격 미스 시 콤보 취소 옵션 1 : 콤보 취소, 0 : 콤보 유지
		int		g_iShieldPotionLv1MixSuccessRate;	// 작은 SD 물약 조합 확률
		int		g_iShieldPotionLv1MixMoney;			// 작은 SD 물약 조합 필요 젠
		int		g_iShieldPotionLv2MixSuccessRate;	// 중간 SD 물약 조합 확률
		int		g_iShieldPotionLv2MixMoney;			// 중간 SD 물약 조합 필요 젠
		int		g_iShieldPotionLv3MixSuccessRate;	// 큰 SD 물약 조합 확률
		int		g_iShieldPotionLv3MixMoney;			// 큰 SD 물약 조합 필요 젠
	#endif
		
	#ifdef UPDATE_SHIELD_CALCULATE_GAGE_20060428
		int		g_iShieldGageConstA;				// 쉴드 게이지 공식 상수값 1
		int		g_iShieldGageConstB;				// 쉴드 게이지 공식 상수값 2
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
	BOOL	g_bIllusionTampleEvent				= TRUE;		// 이벤트 진행 여부
	int		g_iOldScrollDropRate				= 0;		// 낡은 두루마리 나올 확률
	int		g_iCovenantOfIllusionDropRate		= 0;		// 환영교단의 서약 나올 확률
	BOOL	g_bUseMaxLevelIllusionTemple		= FALSE;	// 400 레벨 전용 환영 사원 사용 여부
	BOOL	g_bSaveIllusionTempleRankingPoint	= FALSE;	// 환영 사원의 랭킹 점수 저장여부
	
#ifdef UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710	// 환영사원 스크립트추가
	int		g_iIllusionTempleRewardPercent		= 100;		// 환영사원 보상 경험치 비율
	int		g_btIllusionTempleMinUseSkillCnt	= 3;		// 환영사원 최소사용스킬 
#endif	// UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710
#endif

#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	int		g_iIllusionTempleRewardPercentMaster	= 100;	// 환영사원 보상 경험치 비율(마스터레벨)
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	
#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
int		gML_OldScrollDropRate				= 10;		// 6 낡은 두루마리 나올 확율
int		gML_CovenantOfIllusionDropRate		= 10;		// 6 환영교단의 서약 나올 확율
int		gML_AngelKingsPaperDropRate			= 10;		// 8 대천사의 서 나올 확율xbr
int		gML_BloodBoneDropRate				= 10;		// 8 블러드본 나올 확율
int		gML_EyesOfDevilSquareDropRate		= 10;		// 7 악마의 눈 나올 확율
int		gML_KeyOfDevilSquareDropRate		= 10;		// 7 악마의 열쇠 나올 확율
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
	
#ifdef UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220
int		g_ShadowPahtomMaxLevel				= 220;		// 쉐도우팬텀 공방 최대 레벨
#endif	// UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220

int		g_bCheckSpeedHack					= 1;

#ifdef MODIFY_SPEEDHACK_20080605
int		g_bAbilityDebug						= FALSE;
#endif // MODIFY_SPEEDHACK_20080605

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410
int		g_bChangeCharacterName				= 0;		// 캐릭명 변경 여부
#endif

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
BOOL	g_bUseMapMoveCheckSum				= 0;		// 서버 맵 이동시 채크섬 사용 여부
#endif // ADD_MAPMOVE_PROTOCOL_20090327


void GameMonsterAllAdd();
void ReadCommonServerInfo();
void ReadServerInfo();
void CheckSumFileLoad(char *name);
void LoadItemBag();
void SetMapName();

#ifdef EVENT_OPTION_RELOAD_20040305				// 뮤의 이벤트 옵션만 다시 읽기 위함
void ReadEventInfo(MU_EVENT_TYPE eEventType);
#endif

#ifdef ETC_OPTION_RELOAD_20040308				// 뮤의 옵션 중 이벤트 외의 것들만 리로드
void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType);
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
int gConnectHackIPBlockTime		= 0;// 아이피 블럭 시간 
int gConnectIntervalTime		= 0;// 접속 시간 간격
int gConnectRenewBlockTime		= 0;// 블럭시 접속 하면 블럭 시간을 갱신 한다.
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
// 게임서버에 사용되는 각종 구조체 및 데이터를 초기화 한다.
void GameMainInit(HWND hWnd)
{	
	int n;

	srand( (unsigned)time( NULL ) );

	ReadServerInfo();
	LogInit(LOG_PRINT);		// Log 기록용 함수 초기화
	gSetDate();				// 오늘 날짜 읽기

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
	// 테섭인 경우 T를 붙인다.
	int nLen = strlen(GAMESEVER_VERSION);
	GAMESEVER_VERSION[nLen] = 'T';
#endif // FOR_NEW_TESTSERVER
	
#ifdef FOR_NEW_LIVESEVER
	// 신섭인 경우 N를 붙인다.
	int nLen = strlen(GAMESEVER_VERSION);
	GAMESEVER_VERSION[nLen] = 'N';
#endif	// FOR_NEW_LIVESEVER

#ifdef AUTH_GAMESERVER					// 서버 인증 초기화
	gGameServerAuth.Init();
	gGameServerAuth.SetInfo(gLanguage, gPartition, 0, GAMESEVER_VERSION, szServerName, gServerType, GameServerAuthCallBackFunc);		
	gGameServerAuth.GetKey(szAuthKey, 0, 5);
#endif
	
#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#1 서버 인증 초기화 성공");
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

	// 읽은 맵파일 이름으로 맵속성정보 초기화
	for( n=0; n<g_TerrainManager.Size(); n++)
	{
		MapC[n].ItemInit();

		if(g_TerrainManager.CheckTerrain(n) == true)
			MapC[n].LoadMapAttr(gDirPath.GetNewPath(g_TerrainManager.GetTerrainName(n)), n);
	}
		
	ReadCommonServerInfo();

#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#2 ReadCommonServerInfo 성공");
#endif

#ifdef MAP_SERVER_WORK_20041030			// 맵서버의 정보를 읽음
	g_MapServerManager.LoadData(gDirPath.GetNewPath("MapServerInfo.dat"));
#endif
	
#ifdef CASTLE_MAIN_SCHEDULER_20041111	// 공성서버 만 공성정보를 읽음
#ifdef MAP_SERVER_WORK_20041030
	if (g_CastleSiege.Ready(g_MapServerManager.GetMapSvrGroup()) == TRUE) {
		if (g_CastleSiege.LoadData(gDirPath.GetNewPath("MuCastleData.dat"))) {
			g_CastleSiege.LoadPreFixData(gDirPath.GetNewPath("commonserver.cfg"));
			g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);		// DB 데이터를 읽을 준비를 한다.
		}
	}
#endif
#endif

#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#3 공성서버 정보 읽기 성공");
#endif

#ifdef AUTH_GAMESERVER	
	gGameServerAuth.GetKey(szAuthKey, 10, 5);
#endif

	if( gEnableServerDivision )
	{
		MessageBox(NULL, "서버분할이 가능한 서버입니다.", "Warning", MB_OK);
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
		MsgBox("주의!! 캐릭터 데이터를 저장하지 않습니다.");
	}
#ifdef FOR_ONAIR
	MsgBox("주의!! 방송용 서버 입니다");
#endif

	InitBattleSoccer();

#ifdef AUTH_GAMESERVER
	int DataBufferSize;
	char* DataBuffer;

	gGameServerAuth.RequestData(FILE_MONSTER);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMAttr.LoadAttr(DataBuffer, DataBufferSize);						// 몬스터 속성정보 로드	

	gGameServerAuth.RequestData(FILE_MONSTERSETBASE);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMSetBase.LoadSetBase(DataBuffer, DataBufferSize);	// 몬스터 위치정보 로드
#else
	#ifdef SCRIPT_DECODE_WORK
		gMSetBase.LoadSetBaseDecode(gDirPath.GetNewPath("MonsterSetBase.cwz"));	// 몬스터 위치정보 로드
		gMAttr.LoadAttrDecode(gDirPath.GetNewPath("Monster.cwz"));				// 몬스터 속성정보 로드
		gGateC.LoadDecode(gDirPath.GetNewPath("gate.cwz"));						// 게이트 정보 로드
	#else	
		gMSetBase.LoadSetBase(gDirPath.GetNewPath("MonsterSetBase.txt"));	// 몬스터 위치정보 로드
		gMAttr.LoadAttr(gDirPath.GetNewPath("Monster.txt"));				// 몬스터 속성정보 로드
		gGateC.Load(gDirPath.GetNewPath("gate.txt"));						// 게이트 정보 로드
	#endif
#endif


	// 몬스터에게 갈 아이템 리스트 작성
	g_MonsterItemMng.Init();	

#ifdef HAPPY_POUCH //happycat@20050201
	g_GameEvent.Init();
#endif
	
	// 레벨 별 경험치 계산
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

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 마스터레벨 경험치 테이블 계산
	g_MasterLevelSystem.SetMasterLevelExpTlb();	
#endif

	// 맵파일 이름 초기화
	char MapAttrName[MAX_MAP][15] = {
		"terrain1.att",		// 로랜시아
		"terrain2.att",		// 던전 
		"terrain3.att",		// 데비아스
		"terrain4.att",		// 노리아
		"terrain5.att",		// 로스트 타워
		"terrain6.att",		// 
		"terrain7.att",		// 경기장
		"terrain8.att",		// 아틀란스
		"terrain9.att",		// 타르칸
		"terrain10.att",	// 데빌스퀘어
#ifdef FOR_BLOODCASTLE
		"terrain11.att",	// 천공.
		"terrain12.att",	// 블러드캐슬1
		"terrain12.att",	// 블러드캐슬2
		"terrain12.att",	// 블러드캐슬3
		"terrain12.att",	// 블러드캐슬4
		"terrain12.att",	// 블러드캐슬5
		"terrain12.att",	// 블러드캐슬6
#endif
#ifdef BLOODCASTLE_EXTEND_20040314
		"terrain12.att",	// 블러드캐슬7
#endif
#ifdef CHAOSCASTLE_SYSTEM_20040408
		"terrain19.att",	// 카오스 캐슬1
		"terrain19.att",	// 카오스 캐슬2
		"terrain19.att",	// 카오스 캐슬3
		"terrain19.att",	// 카오스 캐슬4
		"terrain19.att",	// 카오스 캐슬5
		"terrain19.att",	// 카오스 캐슬6
#endif
#ifdef ADD_NEW_MAP_KALIMA_20040518
		"terrain25.att",	// 칼리마 1
		"terrain25.att",	// 칼리마 2
		"terrain25.att",	// 칼리마 3
		"terrain25.att",	// 칼리마 4
		"terrain25.att",	// 칼리마 5
		"terrain25.att",	// 칼리마 6
#endif
#ifdef ADD_NEW_MAP_CASTLE_20041115
		"terrain31.att",	// 성 맵
#endif
#ifdef ADD_NEW_MAP_CASTLE_HUNTZONE_20041115
		"terrain32.att",	// 성 사냥터 맵
#endif
#ifdef DEVILSQUARE_EXTEND_20050221			// 데빌스퀘어 맵추가
		"terrain33.att",	// 데빌스퀘어 맵 확장
#endif
#ifdef ADD_NEW_MAP_AIDA_20050617
		"terrain34.att",	// 아이다 맵
#endif
#ifdef	ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414 
		"terrain35.att",	// 크라이울프 1차맵
#endif
#ifdef	ADD_NEW_MAP_CRYWOLF_SECONDZONE_20050414 
		"terrain36.att",	// 크라이울프 2차맵
#endif
#ifdef HIDDEN_KALIMA_20050706
		"terrain37.att",	// 히든 칼리마
#endif
#ifdef ADD_NEW_MAP_KANTURU_COMMON_20060523
		"terrain38.att",	// 칸투르 1차 맵
		"terrain39.att",	// 칸투르 2차 맵		
#endif
#ifdef ADD_NEW_MAP_KANTURU_BOSS_20060627
		"terrain40.att",	// 칸투르 보스 맵		
#endif
#ifdef UPDATE_GM_FUNCTION_20070228
		"terrain41.att",	// GM 소환 지역
#endif
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3차 전직 맵(att) 추가
		"terrain42.att",	// 3차 전직 퀘스트 맵
		"terrain43.att",	// 3차 전직 퀘스트 보스맵
#endif		
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
		"terrain46.att",	// 사용안함
		"terrain46.att",	// 사용안함
		"terrain46.att",	// 환영사원1
		"terrain46.att",	// 환영사원2
		"terrain46.att",	// 환영사원3
		"terrain46.att",	// 환영사원4
		"terrain46.att",	// 환영사원5
		"terrain46.att",	// 환영사원6
#endif
#ifdef ADD_SEASON_3_NEW_MAP_20070910
		"terrain52.att",	// 시즌 3 신규 맵
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
		"terrain12.att",	// 블러드캐슬8
#endif
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
		"terrain19.att",	// 카오스 캐슬7
#endif
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
		"terrain57.att",	// 사용안함!
		"terrain57.att",	// 사용안함!
		"terrain57.att",	// 평온의늪
#endif
#ifdef ADD_RAKLION_20080408
		"terrain58.att",	// 라클리온 필드
		"terrain59.att",	// 라클리온 보스존
#endif // ADD_RAKLION_20080408

#ifdef UPDATE_XMASEVENT_MAP_20080930
		"terrain62.att",	// 해외 크리스마스 이벤트맵
		"terrain62.att",	// 해외 크리스마스 이벤트맵
		"terrain62.att",	// 해외 크리스마스 이벤트맵
		"terrain62.att",	// 해외 크리스마스 이벤트맵
#endif // UPDATE_XMASEVENT_MAP_20080930	
#ifdef ADD_NEWPVP_PKFIELD
		"terrain63.att",	// 불카누스폐허
		"terrain64.att",	// 결투장
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
	wsGServer.CreateSocket(hWnd);		// ??? 현재 사용않함
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
	MsgBox("#4 기타 정보 읽기 성공");
#endif

	// 데빌스퀘어 랭킹 서버
	wsRServerCli.CreateSocket(hWnd);

#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#5 데빌 스퀘어 성공");
#endif	

	// 이벤트칩 서버
	wsEvenChipServerCli.CreateSocket(hWnd);

#ifdef TEST_LOADING_STEP_20060211
	MsgBox("#6 이벤트칩 성공");
#endif
	
	GameServerInfoSend();		// 접속서버에 게임서버 정보 보냄
#ifdef GAMESERVER_INFO_EXTENDED_20040625	
	GameServerExtInfoSend();	// 접속서버에 게임서버 추가적인 정보 보냄
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
	
	GCTeleportSend(&gObj[0], 1, 1, 1, 2, 3);		// ??? 테스트 용

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

	///////// 샘플 코드 입니다. 나중에 꼭 지워 주세요///////
	//----------------->
	int result = 0;
	lua_State *L = g_MuLua.GetLua();
	g_Generic_Call(L, "LuaBindReturn", ">i", &result);
	
	TRACE_LOG("\n result %d",result);	
	//<-----------------
#endif

}


//----------------------------------------------------------------------------
// 창고 사용료가 얼마인지 얻는다. "얼마에여?"
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
int GetWarehouseUsedHowMuch( int UserLevel, int UserMasterLevel, int IsLock )
#else
int GetWarehouseUsedHowMuch(int UserLevel, int IsLock)
#endif
{
	int  rZen=0;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 종합레벨 - 창고 사용료 조정
	int iTotalLevel = UserLevel + UserMasterLevel;
	rZen = iTotalLevel * iTotalLevel * 0.1 * 0.4 ;
#else
	rZen = UserLevel*UserLevel*0.1*0.4;
#endif

	if( bCanWarehouseLock == TRUE )
	{
		if( IsLock )
		{	// 창고가 잠겨 있으면 level*2의 금액이 추가
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

// 모든 몬스터를 세팅한다.
void GameMonsterAllAdd()
{
	int result;

	gCurPaintPlayer = 0;
	for( int n=0; n<gMSetBase.m_Count; n++)
	{
#ifdef FOR_BLOODCASTLE
		
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( CHECK_DEVILSQUARE(gMSetBase.m_Mp[n].m_MapNumber) )
#else
		if (gMSetBase.m_Mp[n].m_MapNumber == 9 )	// 악마의 광장은 따로 추가해준다.
#endif
			continue;

		if (CHECK_BLOODCASTLE(gMSetBase.m_Mp[n].m_MapNumber))	// 블러드캐슬은 따로 추가해준다. (대천사가 아닐 경우만)
		{
			if (gMSetBase.m_Mp[n].m_Type != 232)
				continue;
		}

#ifdef CHAOSCASTLE_SYSTEM_20040408					// 최초 몬스터 추가 시 카오스캐슬 제외
		if (CHECK_CHAOSCASTLE(gMSetBase.m_Mp[n].m_MapNumber))	// 카오스캐슬은 따로 추가해준다.
		{
			continue;
		}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// 신전 이벤트의 오브젝트는 따로 추가
		if (CHECK_ILLUSION_TEMPLE(gMSetBase.m_Mp[n].m_MapNumber))
		{
			// 위치테이블 번호 미리 저장
			if (gMSetBase.m_Mp[n].m_Type != NPC_LEADER_OF_ALLIED_FORCE		// 연합군 장교
				&& gMSetBase.m_Mp[n].m_Type != NPC_ELDER_OF_ILLUSION)		// 환영교 장로
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
		
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
		result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);
#else
		result = gObjAdd();
#endif
	
		if( result >= 0 )
		{
			gObjSetPosMonster(result, n);
			gObjSetMonster(result, gMSetBase.m_Mp[n].m_Type);
			gCurPaintPlayer++;

			// 대천사는 자신의 블러드캐슬 색인번호를 여기서 추가해 주어야 한다.
			if (CHECK_BLOODCASTLE(gObj[result].MapNumber) && (gObj[result].Class == 232)) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경
				gObj[result].m_cBloodCastleIndex = g_BloodCastle.GetBridgeIndexByMapNum( gObj[result].MapNumber );
#else
				gObj[result].m_cBloodCastleIndex = gObj[result].MapNumber - MAP_INDEX_BLOODCASTLE1;
#endif
			}

#ifdef CASTLE_MAIN_SCHEDULER_20041111				// 왕관의 인덱스를 세팅한다.
			if (gObj[result].Class == 216) {
				g_CastleSiege.SetCrownIndex (result);
			}
#endif

#ifdef CRYWOLF_NPC_WORK_20050912
			
			if( CHECK_CRYWOLF_FIRSTZONE(gObj[result].MapNumber) 
				&& gObj[result].Type == OBJTYPE_NPC 
			  )
			{
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3차전직 NPC는 크라이울프 NPC에서 제외한다.
				if( gObj[result].Class == 406		// 사제 데빈
					|| gObj[result].Class == 407	// 웨어울프 쿼렐
					)
				{
					// 패스.
				}
				else
#endif				
				// 크라이울프 특수 NPC를 등록한다.	- 석상, 제단
				if( CHECK_CRYWOLF_SPECIAL_NPC( gObj[result].Class ) )
				{
					g_Crywolf.m_ObjSpecialNPC.AddObj( result );
				}
				// 크라이울프 일반 NPC를 등록한다.	조합사
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
				// 크라이울프 일반 MONSTER를 등록한다.
				g_Crywolf.m_ObjCommonMonster.AddObj( result );
			}
#endif
			
		}

#else
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( !CHECK_DEVILSQUARE(gMSetBase.m_Mp[n].m_MapNumber) )
#else
		if( gMSetBase.m_Mp[n].m_MapNumber != 9 )	// 악마의 광장은 따로 추가해준다 ??
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
	// 그룹 몬스터 정보를 등록 하고 초기화 한다.
//	TMonsterAIGroup::InitAll();
#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725


	
#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// 공성전 전용서버 일때는 데빌스퀘어, 블러드캐슬, 카오스캐슬, 백색마법사 이벤트가 중지된다.
	gDevilSquareEvent	= FALSE;
	g_bChaosCastle		= FALSE;
#ifdef ADD_RAKLION_20080408
	g_bRaklionEvent		= FALSE;
#endif // ADD_RAKLION_20080408
#endif	
	
#if TESTSERVER == 1		// 테스트 서버에서 공성 & 카오스캐슬 같이 테스트 하기 위해서
//	g_bChaosCastle		= TRUE;
#endif

	g_DevilSquare.Init();

#ifdef FOR_BLOODCASTLE
	g_BloodCastle.LoadItemDropRate();

#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// 공성전 전용서버 일때는 데빌스퀘어, 블러드캐슬, 카오스캐슬, 백색마법사 이벤트가 중지된다.
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

#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// 공성에서는 사원 진행안됨
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	g_bIllusionTampleEvent	= FALSE;
#endif
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 신전 이벤트맵 초기화
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

// 모든 몬스터를 세팅한다.
void GameMonsterAllCloseAndReLoad()
{
	g_PkClearSystem.ClearPkClearNPCData();
	g_ResetSystem.ClearResetNPCData();
	g_BuffHelper.ClearNPCData();

	for( int n=0; n<MAX_MONSTER; n++)
	{
		if( gObj[n].Type == OBJTYPE_MONSTER || gObj[n].Type == OBJTYPE_NPC )
		{
#ifdef ADD_KANTURU_20060627		// 칸투르 관련 몬스터는 몬스터 리로드로 사라지면 안된다.
			if( gObj[n].MapNumber == MAP_INDEX_KANTURU_BOSS )
				continue;

#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// 공성전 관련 몬스터는 몬스터 리로드로 사라지면 않된다.
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
	// 크라이울프 관리용 NPC 정보 초기화
	g_Crywolf.m_ObjCommonNPC.Reset();
	g_Crywolf.m_ObjSpecialNPC.Reset();
#endif

#ifdef CRYWOLF_MONSTER_WORK_20050914
	// 크라이울프 관리용 몬스터 정보 초기화
	g_Crywolf.m_ObjCommonMonster.Reset();
	g_Crywolf.m_ObjSpecialMonster.Reset();
#endif

#ifdef AUTH_GAMESERVER
	int DataBufferSize;
	char* DataBuffer;
	
	gGameServerAuth.RequestData(FILE_MONSTER);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMAttr.LoadAttr(DataBuffer, DataBufferSize);						// 몬스터 속성정보 로드	

	gGameServerAuth.RequestData(FILE_MONSTERSETBASE);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();
	gMSetBase.LoadSetBase(DataBuffer, DataBufferSize);	// 몬스터 위치정보 로드
#else
	#ifdef SCRIPT_DECODE_WORK	
		gMAttr.LoadAttrDecode(gDirPath.GetNewPath("Monster.cwz"));
		gMSetBase.LoadSetBaseDecode(gDirPath.GetNewPath("MonsterSetBase.cwz"));
	#else
		gMAttr.LoadAttr(gDirPath.GetNewPath("Monster.txt"));
		gMSetBase.LoadSetBase(gDirPath.GetNewPath("MonsterSetBase.txt"));
	#endif
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// NPC, 몬스터의 위치정보 리셋
		g_IllusionTempleEvent.ResetPosNum();
#endif

	// 몬스터에게 갈 아이템 리스트 작성
	g_MonsterItemMng.Init();
	
	gObjMonCount	= 0; 
	GameMonsterAllAdd();

#ifdef MODIFY_SELUPAN_REGEN_MONSTER_RELOADING_BUGFIX_20090128
	g_Raklion.SetState( RAKLION_STATE_END );
#endif // MODIFY_SELUPAN_REGEN_MONSTER_RELOADING_BUGFIX_20090128
}


//----------------------------------------------------------------------------
// 게임서버에 사용된 정보들을 종료시킨다.
void GameMainFree()
{		
	gDbSave.End();
	GDGameServerInfoSave();
	gObjEnd();

	ClearBattleSoccer();	
	LogClose();
	
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	// 클라이언트 인증용 메모리 해제
	CleanupGameguardAuth();
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	// 클라이언트 인증용 메모리 해제
	CleanupGameguardAuth25();
#endif
}

//----------------------------------------------------------------------------
// 인증서버에 접속한다.
BOOL GMJoinServerConnect(char* ConnectServer, DWORD wMsg)
{
	BOOL  result;
	
	wsJServerCli.SetProtocolCore( SProtocolCore );
	result = wsJServerCli.Connect(ConnectServer, JoinServerPort, wMsg);
	if( result == FALSE ) return FALSE;
	
	GJServerLogin(); // 게임서버에 로그인 정보를 보낸다.
	LogAdd(lMsg.Get(401), ConnectServer);
	return TRUE;
}

// 데빌스퀘어 랭킹 서버에 접속한다
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

// 이벤트칩 서버에 접속한다
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
// 데이터서버에 접속한다.
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
// 확장 데이터서버에 접속한다.
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
// 접속서버에 접속한다.
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
// 인증을 마쳤을때, 이 함수를 이용해 서버 소켓을 생성한다.
BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg)
{	
	gDbSave.Begin();
	LogAdd(lMsg.Get(404),GameServerPort);
	return TRUE;
}

//----------------------------------------------------------------------------
// 게임서버 메시지 (사용자 접속을 처리한다)
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
			{// 사용자를 짤라야 돼..
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
// 게임서버에서 클라이언트와의 메시지 처리
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
// 인증서버와의 통신 메시지 처리
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
// 랭킹서버와의 통신 메시지 처리
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
// 이벤트 칩서버와의 통신 메시지 처리
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
// 접속서버와의 통신 메시지 처리
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
// 확장데이터 서버와의 통신 메시지 처리
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
		// DxDB가 다운되었다.. 친구를 모두 오프라인 상태로 만든다.
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
// 데이터 서버와의 통신 메시지 처리
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
       
// 게임 서버 정보를 얻는다.
void ReadServerInfo()
{
	if( IsFile(".\\data\\Serverinfo.dat") == FALSE )
	{
		MsgBox("ServerInfo.dat file not found");
	}
	GetPrivateProfileString("GameServerInfo", "ServerName", "", szServerName, 50, ".\\data\\Serverinfo.dat");
	gGameServerCode = GetPrivateProfileInt("GameServerInfo", "ServerCode", 0, ".\\data\\Serverinfo.dat");
	bCanConnectMember = GetPrivateProfileInt("GameServerInfo", "ConnectMemberLoad", 0, ".\\data\\Serverinfo.dat");

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410	// 캐릭명 변경 여부
	g_bChangeCharacterName	= GetPrivateProfileInt("GameServerInfo", "ChangeCharacterName", 0, ".\\data\\Serverinfo.dat");
#endif	// UPDATE_CHANGE_CHARACTERNAME_20080410
}

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
// 게임가드 업데이트 정보를 읽는다.
void ReadGameGuardUpdateInfo()
{
	if( IsFile(gDirPath.GetNewPath("GameGuardModule.dat")) == FALSE )
	{
		MsgBox("GameGuardModule.dat file not found");
	}
	GetPrivateProfileString("GameGuardModule", "GGModuleName", "", gGGModuleName, 20, gDirPath.GetNewPath("GameGuardModule.dat"));
}
#endif

// 공용 서버 정보를 얻는다.
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
	gGateC.Load(DataBuffer, DataBufferSize);						// 게이트 정보 로드
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
	case 0 : // 국내 버전
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

	case 1 : // 영어 버전
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

	case 2 : // 일본 버전
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

	case 3 : // 중국 버전
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

	case 4 : // 대만 버전
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

	case 5 : // 태국 버전
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
	case 6 : // 필리핀 버전
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

#ifdef FOR_VIETNAM		// 베트남
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

#ifdef FOR_USA		// 미국
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

	// 클라이언트 인증 알고리즘 파일 읽기 및 최대 사용자 설정 
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

	// 클라이언트 인증 알고리즘 파일 읽기 및 최대 사용자 설정 
	// M 드라이브의 실행권한 확인 필요.
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
		// 먼저 한국의 정보를 읽고 
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
		{	// 해외 버전의 경우에는 아이템 이름만 다시 로드한다
			if( OpenItemNameScriptDecode(szItemTextFileName) == FALSE )
				MsgBox(lMsg.Get(419));
		}
	#else
		if( gLanguage != 0 )
		{	// 해외 버전의 경우에는 아이템 이름만 다시 로드한다
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
	{	// 해외 버전의 경우에는 스킬 이름만 다시 로드한다
		gGameServerAuth.RequestData(FILE_SKILLLOCAL);
		DataBufferSize = gGameServerAuth.GetDataBufferSize();
		DataBuffer = gGameServerAuth.GetDataBuffer();
		MagicDamageC.LogSkillNameList(DataBuffer, DataBufferSize);		
	}
#endif
#else
	#ifdef SCRIPT_DECODE_WORK
		// Skill.txt 로드
		MagicDamageC.LogSkillListDecode(szKorSkillTextFileName);
		//MagicDamageC.LogSkillList(szSkillTextFileName);	
#if 0
		if( gLanguage != 0 )

		{	// 해외 버전의 경우에는 스킬 이름만 다시 로드한다
			MagicDamageC.LogSkillNameListDecode(szSkillTextFileName);
		}
#endif
	#else
		// Skill.txt 로드
		MagicDamageC.LogSkillList(szKorSkillTextFileName);
		//MagicDamageC.LogSkillList(szSkillTextFileName);	
#if 0
		if( gLanguage != 0 )
		
		{	// 해외 버전의 경우에는 스킬 이름만 다시 로드한다
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
		// 여기 까지 오면 인증이 끝난거다
	// 클라이언트 버젼 정보를 얻는다		
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
	// 클라이언트 버젼 정보 얻고
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
// 몬스터 스킬 정보를 설정한다.gDirPath.GetNewPath("MoveLevel.txt")
TMonsterSkillElement::LoadData( gDirPath.GetNewPath("MonsterSkillElement.txt") );
TMonsterSkillUnit::LoadData( gDirPath.GetNewPath("MonsterSkillUnit.txt") );
TMonsterSkillManager::LoadData( gDirPath.GetNewPath("MonsterSkill.txt") );
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

// 몬스터 AI 정보를 설정한다.

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
		LogAddTD( "[Option Reload] ★★★ MaxUser ☆☆☆  : %d", gServerMaxUser );	
	}


#endif
	
#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706
	g_iServerGroupGuildChatting = GetPrivateProfileInt("GameServerInfo", "ServerGroupGuildChatting", 0, gDirPath.GetNewPath("commonserver.cfg"));	
	g_iServerGroupUnionChatting = GetPrivateProfileInt("GameServerInfo", "ServerGroupUnionChatting", 0, gDirPath.GetNewPath("commonserver.cfg"));	
#endif

	// 길드 관련
	GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gGuildCreate = atoi(szTemp);
	LogAdd(lMsg.Get(569), gGuildCreate);


	GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gGuildDestroy = atoi(szTemp);
	LogAdd(lMsg.Get(570), gGuildDestroy);	// "길드 삭제 옵션 %d"


	GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gGuildCreateLevel = atoi(szTemp);
	LogAdd(lMsg.Get(571), gGuildCreateLevel);	// 길드생성 최소 레벨 %d

#ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
	// 수성길드 해체 제한
	g_bCastleGuildDestoyLimit	= GetPrivateProfileInt( "GameServerInfo", "CastleOwnerGuildDestroyLimit", 1, gDirPath.GetNewPath("commonserver.cfg"));
#endif

	// 스피드핵 사용자를 블럭 시킬것인가?
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

#ifdef ADD_ZEN_OPTION_20070903	// commonserver에서 읽기
	GetPrivateProfileString( "GameServerInfo", "AddZen", "1", szTemp, 5, gDirPath.GetNewPath( "commonserver.cfg" ) );
	gAddZen = ( float )atof( szTemp );
#endif

	// 거래기능을 활성화 할지 결정한다.
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

	// 살인마 회복 Tick 설정(초) 1이면 1초씩 증가 2면 2초씩 증가
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

	// 젠 드롭 시간 설정
	gZenDurationTime = GetPrivateProfileInt("GameServerInfo", "ZenDurationTime", 30, gDirPath.GetNewPath("commonserver.cfg"));	
	gLootingTime = GetPrivateProfileInt("GameServerInfo", "ItemDurationTime", 30, gDirPath.GetNewPath("commonserver.cfg"));	

	// 이벤트 칩 정보를 얻는다
	gEventChipEvent = GetPrivateProfileInt("GameServerInfo", "EventChipEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gBoxOfGoldDropRate = GetPrivateProfileInt("GameServerInfo", "BoxOfGoldDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
	g_ItemDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo", "ItemDropRateForBoxOfGold", 2, gDirPath.GetNewPath("commonserver.cfg"));
	g_EventChipDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo", "EventChipDropRateForBoxOfGold", 2, gDirPath.GetNewPath("commonserver.cfg"));	

#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// 공성전 전용서버 일때는 데빌스퀘어, 블러드캐슬, 카오스캐슬, 백색마법사 이벤트가 중지된다.
	gDevilSquareEvent	= 0;
#endif

	if( !gDevilSquareEvent )
	{	// 악마의 광장 이벤트가 아니면, 광장을 닫는다
		g_DevilSquare.SetClose();
	}

	// 공속 체크를 위한 설정 값들
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

	// 타마짱 이벤트
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

	// 채팅 로그를 기록하는가?
	gWriteChatLog = GetPrivateProfileInt("GameServerInfo", "WriteChatLog", 0, gDirPath.GetNewPath("commonserver.cfg"));

	gDisconnectHackUser = GetPrivateProfileInt("GameServerInfo", "DisconnectHackUser", 0, gDirPath.GetNewPath("commonserver.cfg"));

	gUseNPGGChecksum = GetPrivateProfileInt("GameServerInfo", "UseNPGGChecksum", 0, gDirPath.GetNewPath("commonserver.cfg"));

	

	GetPrivateProfileString("GameServerInfo", "DecTimePerAttackSpeed", "5.33", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gDecTimePerAttackSpeed = atof(szTemp);

	// 공속핵에 대한 패널티 적용 값
	gSpeedHackPenalty = GetPrivateProfileInt("GameServerInfo", "SpeedHackPenalty", 0, gDirPath.GetNewPath("commonserver.cfg"));	


	// 데빌스퀘어 초대권 조합확률
#ifdef DEVILSQUARE_EXTEND_20050221		// 카오스 조합 추가
	gDQChaosSuccessRateLevel1 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel1", 75, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel2 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel2", 70, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel3 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel3", 65, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel4 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel4", 60, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel5 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel5", 55, gDirPath.GetNewPath("commonserver.cfg"));
	gDQChaosSuccessRateLevel6 = GetPrivateProfileInt("GameServerInfo", "DQChaosSuccessRateLevel6", 50, gDirPath.GetNewPath("commonserver.cfg"));
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// 광장7 카오스조합 확률 파일에서 읽기
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

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214						// 성주의 표식 드롭확률
	g_iMarkOfTheLord			= GetPrivateProfileInt("GameServerInfo", "MarkOfTheLord", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif
	
#ifndef EVENT_OPTION_RELOAD_20040305
	// 1 . 데빌 스퀘어
	g_DevilSquare.Load(gDirPath.GetNewPath("DevilSquare.dat"));
	gDevilSquareEvent			= GetPrivateProfileInt("GameServerInfo", "DevilSquareEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gEyesOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "EyesOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
	gKeyOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "KeyOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));	   
	
	// 2 . 블러드캐슬
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
	// 수호보석 드랍여부
	gIsDropGemOfDefend  = GetPrivateProfileInt("GameServerInfo", "IsDropGemOfDefend", 0, gDirPath.GetNewPath("commonserver.cfg"));
	// 수호보석 드랍 확률 (10000 기준)
	gGemOfDefendDropRate = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropRate", 5, gDirPath.GetNewPath("commonserver.cfg"));
	// 수호보석 드랍 몬스터 레벨
	gGemOfDefendDropLevel = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));;		
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314
	// 시련의 땅에서 세트아이템 드랍여부
	gIsDropSetItemInCastleHuntZone  = GetPrivateProfileInt("GameServerInfo", "IsDropSetItemInCastleHuntZone", 0, gDirPath.GetNewPath("commonserver.cfg"));
	// 시련의 땅에서 세트아이템 드랍확률 (10000 기준)
	gSetItemInCastleHuntZoneDropRate = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropRate", 1, gDirPath.GetNewPath("commonserver.cfg"));
	// 시련의 땅에서 세트아이템 드랍 몬스터 레벨
	gSetItemInCastleHuntZoneDropLevel = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));;		
#endif

#ifdef FOR_NEW_TESTSERVER
	gTestServer	= GetPrivateProfileInt("GameServerInfo", "TestServer", 0, gDirPath.GetNewPath("commonserver.cfg"));	
#endif

	// 3 . 지하 군단
	gAttackEventRegenTime = GetPrivateProfileInt("GameServerInfo", "AttackEventRegenTime", 10, gDirPath.GetNewPath("commonserver.cfg"));

	// 4 . 황금 군단
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

	// 5 . 백색마법사 군단
#ifdef MONSTER_HERD_SYSTEM_20031120
	// 반지 이벤트를 할지 여부를 얻어온다.
	GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	g_bDoRingEvent				= atoi(szTemp);
	// 오크를 잡았을 때 경품을 탈 확률 (1/n)
	GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRingOrcKillGiftRate		= atoi(szTemp);
	// 반지를 던졌을 때 경품을 탈 확률 (1/n)
	GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRingDropGiftRate			= atoi(szTemp);
	g_RingAttackEvent.Load(gDirPath.GetNewPath("RingAttackEvent.dat"));
#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// 공성전 전용서버 일때는 데빌스퀘어, 블러드캐슬, 카오스캐슬, 백색마법사 이벤트가 중지된다.
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

	// 6 . 러브팡팡 (러브러브) 이벤트

	// # REMOVE_LOVELOVE_EVENT_COUPLING_WITH_JOINSERVER_20060319
	// # 인증 서버와 연동해서 이벤트 처리를 한 것으로 보임(apple)
	//		- 인증 서버와 교환하는 0x30, 0x31 프로토콜 참고
	//		- 인증 서버에 러브하트 관련 DB 도 없음.
	//		- 인증 서버에서 러브하트 코드 제거 할 것임.
	GetPrivateProfileString("GameServerInfo", "EVENT1", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gEvent1						= atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayMax", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gEvent1ItemDropTodayCount	= 0;
	gEvent1ItemDropTodayMax		= atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayPercent", "10000000", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gEvent1ItemDropTodayPercent = atoi(szTemp);

	// 7 . 폭죽 이벤트
	gFireCrackerEvent			= GetPrivateProfileInt("GameServerInfo", "FireCrackerEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gFireCrackerDropRate		= GetPrivateProfileInt("GameServerInfo", "FireCrackerDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
	g_ItemDropRateForgFireCracker	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForFireCracker", 2, gDirPath.GetNewPath("commonserver.cfg"));
	gOnlyFireCrackerEffectUse	= GetPrivateProfileInt("GameServerInfo", "OnlyFireCrackerEffectUse", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// 8 . 메달 이벤트
	gMedalEvent					= GetPrivateProfileInt("GameServerInfo", "MedalEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldMedalDropRate			= GetPrivateProfileInt("GameServerInfo", "GoldMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));						// 금메달이 떨어질 확률
	gSilverMedalDropRate		= GetPrivateProfileInt("GameServerInfo", "SilverMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));					// 은메달이 떨어질 확률
	g_ItemDropRateForGoldMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForGoldMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));		// 금메달에서 아이템이 떨어질 확률
	g_ItemDropRateForSilverMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForSilverMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));	// 은메달에서 아이템이 떨어질 확률

	// 9 . 성탄의별 이벤트
	gXMasEvent					= GetPrivateProfileInt("GameServerInfo", "XMasEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_XMasEvent_StarOfXMasDropRate	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_StarOfXMasDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
	g_XMasEvent_ItemDropRateForStarOfXMas	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_ItemDropRateForStarOfXMas", 2, gDirPath.GetNewPath("commonserver.cfg"));

	// 10 . 사랑의하트 이벤트
	gHeartOfLoveEvent			= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gHeartOfLoveDropRate		= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
	g_ItemDropRateForgHeartOfLove	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForHeartOfLove", 2, gDirPath.GetNewPath("commonserver.cfg"));

	// 11 . NPC의 새해인사
	GetPrivateProfileString("GameServerInfo", "HappyNewYearTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gHappyNewYearNpcEvent		= atoi(szTemp);
	if( gHappyNewYearNpcEvent )	LogAddTD(lMsg.Get(573));	// [옵션] 새해 인사 NPC 대화 출력

	// 12 . NPC의 메리크리스마스
	GetPrivateProfileString("GameServerInfo", "MerryXMasTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
	gMerryXMasNpcEvent			= atoi(szTemp);
	if( gMerryXMasNpcEvent )	LogAddTD(lMsg.Get(572));	// [옵션] 크리스마스 NPC 대화 출력

#ifdef CHAOSCASTLE_SYSTEM_20040408 
	// 13 . 카오스 캐슬
	g_ChaosCastle.Load(gDirPath.GetNewPath("ChaosCastle.dat"));
	g_bChaosCastle				= GetPrivateProfileInt("GameServerInfo", "ChaosCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518							// 쿤둔의 표식 드롭확률을 읽는다.
	// 14 . 칼리마 - 쿤둔의표식
	g_iKundunMarkDropRate= GetPrivateProfileInt("GameServerInfo", "KundunMarkDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804					// 신비의 구슬 드롭 확률
	// 15 . 신비의 구슬 드롭률
	g_iMysteriousBeadDropRate1 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate1", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iMysteriousBeadDropRate2 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate2", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iHiddenTreasureBoxOfflineRate = GetPrivateProfileInt("GameServerInfo", "HiddenTreasureBoxOfflineRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef EVENT_MANAGEMENT_20040711
	// 16 . 이벤트 매니저 가동 여부
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
	// apple★20050204
	// 17 . 파란, 빨강 복주머니 이벤트
	gHappyPouchRedDropRate	= GetPrivateProfileInt("GameServerInfo", "HappyPouchRedDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));							
	gHappyPouchBlueDropRate	= GetPrivateProfileInt("GameServerInfo", "HappyPouchBlueDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));							
#endif

	// 18. 다크로드의 마음 이벤트
#ifdef DARK_LORD_HEART_EVENT_20040810						// 다크로드의 마음 이벤트 관련 확률을 읽는다.
	g_iDarkLordHeartDropRate		= GetPrivateProfileInt("GameServerInfo", "DarkLordHeartDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iDarkLordHeartOffEventRate	= GetPrivateProfileInt("GameServerInfo", "DarkLoadHeartOffEventRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#endif

	// 19. 중국 월병 이벤트
#ifdef MU_CHN_OFFLINE_GIFT_EVENT_20040908
	gbChnOfflineGiftEvent			= GetPrivateProfileInt("GameServerInfo", "ChnOfflineGiftEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	giChnOfflineGiftEventDropRate	= GetPrivateProfileInt("GameServerInfo", "ChnOfflineGiftEventDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

	// 20. 크리스마스 리본상자 이벤트( RKFKA )
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

	// 21. 발렌타인데이 초콜릿상자 이벤트( RKFKA )
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

	// 22. 화이트데이 사탕상자 이벤트( RKFKA )
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

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영 사원 이벤트정보를 읽는다
	g_IllusionTempleEvent.LoadIllusionTempleEventInfo();
#endif

#ifdef ADD_THIRD_WING_20070525	// 콘도르의불꽃 설정 읽기
 	g_iCondorFlameDropRate		= GetPrivateProfileInt( "GameServerInfo", "CondorFlameDropRate", 10, gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	g_bCherryBlossomEventOn						= GetPrivateProfileInt( "GameServerInfo", "CherryBlossomEventOn",					1,		gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_bCherryBlossomEventItemDropRate			= GetPrivateProfileInt( "GameServerInfo", "CherryBlossomEventItemDropRate",			8000,	gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029							// 행운의 동전 드롭 확률
	g_bLuckyCoinEventOn		= GetPrivateProfileInt( "GameServerInfo", "LuckyCoinEventOn",			0,		gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_iLuckyCoinDropRate	= GetPrivateProfileInt( "GameServerInfo", "LuckyCoinDropRate",			1000,	gDirPath.GetNewPath( "commonserver.cfg" ) );
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 마스터레벨 설정 읽기
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

	// 접속서버 정보를 얻고
	GetPrivateProfileString("ConnectServerInfo", "IP", "", connectserverip, 20, szCommonlocIniFileName);
	GetPrivateProfileString("ConnectServerInfo", "PORT", "", szTemp, 10, szCommonlocIniFileName);
	connectserverport = atoi(szTemp);
	LogAddTD(lMsg.Get(414), connectserverip, connectserverport);

#ifdef GAMESERVER_INFO_TCP_20031203
	GetPrivateProfileString("ConnectServerInfo", "PORT_TCP", "", szTemp, 10, szCommonlocIniFileName);
	connectserverport_tcp = atoi(szTemp);

	// 접속서버에 정보를 어떻게 보낼 것인가 결정 
	GetPrivateProfileString("ConnectServerInfo", "CONNTYPE", "0", szTemp, 10, szCommonlocIniFileName);
	g_ConServerInfoSendType = atoi(szTemp);

	if (g_ConServerInfoSendType!= 0 &&
		g_ConServerInfoSendType!= 1
		)
	{
		// 전송 방식이 UDP / TCP 둘 중 하나가 아니면 기본은 UDP
		g_ConServerInfoSendType = 0;
	}
#endif

	gUdpSoc.SendSet(connectserverip, connectserverport);
	gUdpSocCE.SendSet(gChaosEventServerIp, 60005);

#ifdef GAMESERVER_INFO_TO_CSS_20040317
	// 분산을 할 것인지 체크한다.
	g_bSendGSInfoToCSS  = GetPrivateProfileInt("GameServerInfo", "GSInfoSendToCSS", 0, gDirPath.GetNewPath("commonserver.cfg"));
	if (g_bSendGSInfoToCSS) {
		// 만약 분산을 한다면 새로 세팅한다.
		GSINFO_INITCSLIST();
	}
#endif	

	DevilSquareEventConnect = GetPrivateProfileInt("GameServerInfo", "DevilSquareEventConnect", 1, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "DevilSquareEventServer", "210.181.89.241", gDevilSquareEventServerIp, 20, gDirPath.GetNewPath("commonserver.cfg"));

	EventChipServerConnect = GetPrivateProfileInt("GameServerInfo", "EventChipServerConnect", 0, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "EventChipServerIp", "192.168.10.150", gEventChipServerIp, 20, gDirPath.GetNewPath("commonserver.cfg"));

	// 정정당당 서버를 위한 영웅 시스템을 적용하는가?
	gApplyHeroSystem = GetPrivateProfileInt("GameServerInfo", "ApplyHeroSystem", 0, gDirPath.GetNewPath("commonserver.cfg"));	

	// 배틀사커를 할수 있는가?
	gEnableBattleSoccer = GetPrivateProfileInt("GameServerInfo", "EnableBattleSoccer", 1, gDirPath.GetNewPath("commonserver.cfg"));	

	// 이벤트관련 NPC들이 말을 하는가?
	gEnableEventNPCTalk = GetPrivateProfileInt("GameServerInfo", "EnableEventNPCTalk", 0, gDirPath.GetNewPath("commonserver.cfg"));	

	gEnableServerDivision = GetPrivateProfileInt("GameServerInfo", "EnableServerDivision", 0, gDirPath.GetNewPath("commonserver.cfg"));		

#ifdef MODIFY_JAPAN_MONSTER_VALUE
	gMonsterHPAdjust = GetPrivateProfileInt("GameServerInfo", "MonsterHPAdjust", 100, gDirPath.GetNewPath("commonserver.cfg"));
#endif	
	//gUdpSocCRank.SendSet(gDevilSquareEventServerIp, 44455);		

	cManager.Init();

#ifdef HACK_LOG
	// 접속서버 정보를 얻고
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
	// CODEREVIEW_SHIELD : Float 형으로 변경 SuccessAttackRateOption
	// CODEREVIEW_SHIELD : PreFix 붙이기
	g_fSuccessAttackRateOption		= GetPrivateProfileInt("GameServerInfo",	"SuccessAttackRateOption",	20000,	gDirPath.GetNewPath("commonserver.cfg")) / 10000;
	g_iSDChargingOption				= GetPrivateProfileInt("GameServerInfo",	"SDChargingOption",			1,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iConstNumberOfShieldPoint		= GetPrivateProfileInt("GameServerInfo",	"ConstNumberOfShieldPoint", 20,		gDirPath.GetNewPath("commonserver.cfg"));

	g_ShieldAutoRefillOn			= GetPrivateProfileInt("GameServerInfo",	"ShieldAutoRefillOn",		0,		gDirPath.GetNewPath("commonserver.cfg"));
	g_ShieldAutoRefillOnSafeZone	= GetPrivateProfileInt("GameServerInfo",	"ShieldAutoRefilOnSafeZone",1,		gDirPath.GetNewPath("commonserver.cfg"));
	g_PKLevelIncreaseOff			= GetPrivateProfileInt("GameServerInfo",	"PKLevelIncreaseOff",		0,		gDirPath.GetNewPath("commonserver.cfg"));

	// CODEREVIEW_SHIELD3 : 쉴드포션 관련 스크립트 추가
	g_CompoundPotionDropOn				= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionDropOn",			0,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv1DropRate		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv1DropRate",	100,	gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv2DropRate		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv2DropRate",	80,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv3DropRate		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv3DropRate",	50,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv1DropLevel		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv1DropLevel",	68,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv2DropLevel		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv2DropLevel",	96,		gDirPath.GetNewPath("commonserver.cfg"));
	g_iCompoundPotionLv3DropLevel		= GetPrivateProfileInt("GameServerInfo",	"CompoundPotionLv3DropLevel",	118,	gDirPath.GetNewPath("commonserver.cfg"));

	#ifdef UPDATE_SHIELD_OPTION_20060427
	g_bShieldComboMissOptionOn			= GetPrivateProfileInt("GameServerInfo",	"ShieldComboMissOptionOn",			0,			gDirPath.GetNewPath("commonserver.cfg"));	// PvP 공격 미스 시 콤보 취소 옵션 1 : 콤보 취소, 0 : 콤보 유지
	g_iShieldPotionLv1MixSuccessRate	= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv1MixSuccessRate",	50,			gDirPath.GetNewPath("commonserver.cfg"));	// 작은 SD 물약 조합 확률
	g_iShieldPotionLv1MixMoney			= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv1MixMoney",			100000,		gDirPath.GetNewPath("commonserver.cfg"));	// 작은 SD 물약 조합 비용
	g_iShieldPotionLv2MixSuccessRate	= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv2MixSuccessRate",	30,			gDirPath.GetNewPath("commonserver.cfg"));	// 중간 SD 물약 조합 확률
	g_iShieldPotionLv2MixMoney			= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv2MixMoney",			500000,		gDirPath.GetNewPath("commonserver.cfg"));	// 중간 SD 물약 조합 비용
	g_iShieldPotionLv3MixSuccessRate	= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv3MixSuccessRate",	30,			gDirPath.GetNewPath("commonserver.cfg"));	// 큰 SD 물약 조합 확률
	g_iShieldPotionLv3MixMoney			= GetPrivateProfileInt("GameServerInfo",	"ShieldPotionLv3MixMoney",			1000000,	gDirPath.GetNewPath("commonserver.cfg"));	// 큰 SD 물약 조합 비용
	#endif // UPDATE_SHIELD_OPTION_20060427

	#ifdef UPDATE_SHIELD_CALCULATE_GAGE_20060428
	g_iShieldGageConstA					= GetPrivateProfileInt("GameServerInfo",	"ShieldGageConstA",					12,			gDirPath.GetNewPath("commonserver.cfg"));	// 쉴드 게이지 공식 상수값 1
	g_iShieldGageConstB					= GetPrivateProfileInt("GameServerInfo",	"ShieldGageConstB",					30,			gDirPath.GetNewPath("commonserver.cfg"));	// 쉴드 게이지 공식 상수값 2
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
	// 엘도라도 이벤트 초기화 시작
	gEledoradoEvent.SetEventState(gIsEledoradoEvent);
	gEledoradoEvent.Init();
#endif // MODIFY_ELDORADO_EVENT_OPTION_LOAD_SEQUEANCE_20070821
	
	gPacketCheckSum.Init();

#ifdef PERSONAL_SHOP_20040113 
	gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef EVENT_FRIENDSHIP_20040204
	// 일본 내친구 이벤트 - 상품 텍스트 읽기
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

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// 중국추가> . 피로도 시스템 적용여부, 경험치, 아이템확률 읽음
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
	ReadEventInfo(MU_EVENT_ALL);		// 최초이므로 모든 이벤트 정보를 다 읽음
#endif

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
	g_iUseCharacterAutoRecuperationSystem = GetPrivateProfileInt("GameServerInfo", "UseCharacterAutoRecuperationSystem", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iCharacterRecuperationMaxLevel = GetPrivateProfileInt("GameServerInfo", "CharacterRecuperationMaxLevel", 100, gDirPath.GetNewPath("commonserver.cfg"));

#endif

#ifdef MAKE_ITEM_STOP
	// 아이템 '만들기' 매크로 등 사용금지 옵션
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
// 접속서버에 보내는 게임서버 정보를 관리한다.
int GSInfoSendFlag=0;

// 게임보내는 정보를 잠시 정지한다.
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

VOID GSINFO_INITCSLIST()		// 접속서버의 리스트를 읽고 소켓을 초기화한다.
{
	// 우선 기존의 자원들을 모두 제거한다.
	g_iConnectServerCount = 0;
	for (INT i = 0 ; i < MAX_CS_COUNT ; i++) {
		if (gpUdpSocCSS[i] != NULL) {
			delete gpUdpSocCSS[i];
			gpUdpSocCSS[i] = NULL;
		}
		memset (g_szCSIPADDR[i], 0, sizeof(g_szCSIPADDR[i]));
	}

	// 접속서버 리스트 파일을 찾는다.
	INT iRES = _access(gDirPath.GetNewPath(CSLIST_FILENAME), 0);
	if (!iRES) {
		// 파일이 존재한다.
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
		// 파일이 존재하지 않는다. 최악의 상활 (그냥 이전에 보내던 데로 보낸다.)
		g_bSendGSInfoToCSS = 0;
		return;
	}

	// 접속서버에 정보를 보낼 소켓들을 세팅한다.
	for (i = 0 ; i < g_iConnectServerCount ; i++) {
		if (gpUdpSocCSS[i] != NULL) {
			gpUdpSocCSS[i]->SendSet(g_szCSIPADDR[i], g_wCSPORT[i]);
		}
	}
}


void GameServerInfoSend()		// 현재 TCP방식의 게임서버 정보전달
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
		// 기존의 방식으로 게임서버 정보를 보낸다.
		gUdpSoc.SendData((LPBYTE)&pMsg, pMsg.h.size);			
	}
	else {
		// 접속서버 군에 게임서버 정보를 모두보낸다.
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

void GameServerInfoSend()		// 현재 TCP방식의 게임서버 정보전달
{
	if( gDisconnect == 1 ) return;
#ifndef GUILD_REWORK
	if( GSInfoSendFlag == 0 ) return;
#endif

	switch(g_ConServerInfoSendType) {
	case 0 :		// 접속서버에 게임서버 정보를 UDP로 보낼 것이다.
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
	case 1 :		// 접속서버에 게임서버 정보를 TCP로 보낼 것이다.
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

void GameServerInfoSend()		// 예전 UDP방식의 게임서버 정보전달
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


#ifdef GAMESERVER_INFO_EXTENDED_20040625	// 게임서버에서 접속서버로 보내는 추가적인 정보

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

	// 성탄의 별 아이템백 정보 로드
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

	// 폭죽 아이템백 정보 로드
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

	// 폭죽 아이템백 정보 로드
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

	// 금메달 아이템백 정보 로드
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

	// 은메달 아이템백 정보 로드
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
	if( RingEventItemBag ){delete RingEventItemBag;}		// 반지의 제왕 이벤트 아이템백
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


#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211		// 내친구 이벤트 - 전사의 반지
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
	// 빨간리본상자 
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
		
	// 초록리본상자 
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


	// 파란리본상자
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

#ifdef VALENTINES_DAY_BOX_EVENT_20060124	// 발렌타인데이 초콜릿상자 이벤트
	// 분홍초콜릿상자  
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
		
	// 빨간초콜릿상자 
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


	// 파란리본상자
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

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222	// 발렌타인데이 초콜릿상자 이벤트
	// 연보라사탕상자  
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
		
	// 주홍초콜릿상자 
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


	// 감청초콜릿상자
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

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404	// 다크엘프 아이템 백(eventitembag30.txt) 로드	
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

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404		// 발가스 아이템 백(eventitembag31.txt) 로드
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

#ifdef KANTURU_MONSTER_ITEMDROP_20060807		// 칸투르 몬스터 아이템 백(eventitembag32.txt, eventitembag33.txt) 로드
	// 마야 손
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

	// 나이트메어
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

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119	// 용사, 전사(우정의 돌)의 반지 추가/수정
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

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131				// 설날복주머니 아이템 가방
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

#ifdef UPDATE_GM_FUNCTION_20070228							// GM 선물 상자 아이템 가방 로드
		if( GMPresentBoxItemBag ) { delete GMPresentBoxItemBag; }
		GMPresentBoxItemBag	= new CProbabilityItemBag;
		if( GMPresentBoxItemBag == NULL )
		{
			MsgBox("CItemBag %s", lMsg.Get(110));
			return;
		}

		GMPresentBoxItemBag->ProbabilityItemBagInit("eventitembag41.txt");
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206						// 혼돈 상자 아이템 백 로드
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

	// 빨강혼돈 상자
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

	// 보라혼돈 상자
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
		// eventitembag52.txt 로드
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
		// eventitembag59.txt 로드
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

#ifdef ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610	// 여름 불꽃 유령 이벤트
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

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029					// 행운의 동전 교환 아이템 로드
		// eventitembag56.txt 로드
		if( pLuckyCoinEventItemBag10 )
			delete pLuckyCoinEventItemBag10;
		
		pLuckyCoinEventItemBag10 = new CProbabilityItemBag;
		if( pLuckyCoinEventItemBag10 == NULL )
		{
			MsgBox( "CProbabilityItemBag %s", lMsg.Get( 110 ) );
			return;
		}
		
		pLuckyCoinEventItemBag10->ProbabilityItemBagInit( "eventitembag56.txt" );

		// eventitembag57.txt 로드
		if( pLuckyCoinEventItemBag20 )
			delete pLuckyCoinEventItemBag20;
		
		pLuckyCoinEventItemBag20 = new CProbabilityItemBag;
		if( pLuckyCoinEventItemBag20 == NULL )
		{
			MsgBox( "CProbabilityItemBag %s", lMsg.Get( 110 ) );
			return;
		}
		
		pLuckyCoinEventItemBag20->ProbabilityItemBagInit( "eventitembag57.txt" );

		// eventitembag58.txt 로드
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

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영 사원 몬스터 아이템 가방 로드
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
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 확장맵 이름추가? !!! 임시 (구현않됨)
#endif
}


#ifdef AUTH_GAMESERVER
int	GetEventFlag()
{
	int flag = 0;

	if( gDevilSquareEvent )	flag |= EVENTMASK_DevilSquareEvent;	// 데빌스퀘어
	if( g_bBloodCastle )	flag |= EVENTMASK_BloodCastleEvent;	// 블러드캐슬
	if( gXMasEvent )		flag |= EVENTMASK_XMasEvent;		// 성탄의별
	if( gFireCrackerEvent )	flag |= EVENTMASK_FireCrackerEvent;	// 폭죽
	if( gHeartOfLoveEvent )	flag |= EVENTMASK_HeartOfLoveEvent;	// 사랑의 하트
	if( gMedalEvent )		flag |= EVENTMASK_MedalEvent;		// 메달이벤트
	if( g_bDoRingEvent )	flag |= EVENTMASK_RingAttackEvent;	// 반지이벤트
	if( gEventChipEvent )	flag |= EVENTMASK_EventChipEvent;	// 레나이벤트
	if( gIsEledoradoEvent )	flag |= EVENTMASK_IsEledoradoEvent;	// 황금정예이벤트
	if( gUseNPGGChecksum )	flag |= EVENTMASK_UseNPGGChecksum;	// 게임가드 사용 

	return flag;
}
#endif


#ifdef EVENT_OPTION_RELOAD_20040305				// 뮤의 이벤트 옵션만 다시 읽기 위함

void ReadEventInfo(MU_EVENT_TYPE eEventType)
{
	char szTemp[256];	

	switch(eEventType) {

	case MU_EVENT_ALL :					// 모든 이벤트의 옵션을 읽음
		{
			// 1 . 데빌 스퀘어
			g_DevilSquare.Load(gDirPath.GetNewPath("DevilSquare.dat"));
			gDevilSquareEvent			= GetPrivateProfileInt("GameServerInfo", "DevilSquareEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gEyesOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "EyesOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			gKeyOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "KeyOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));	   
			
			// 2 . 블러드캐슬
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
			// 수호보석 드랍여부
			gIsDropGemOfDefend  = GetPrivateProfileInt("GameServerInfo", "IsDropGemOfDefend", 0, gDirPath.GetNewPath("commonserver.cfg"));
			// 수호보석 드랍 확률 (10000 기준)
			gGemOfDefendDropRate = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropRate", 5, gDirPath.GetNewPath("commonserver.cfg"));
			// 수호보석 드랍 몬스터 레벨
			gGemOfDefendDropLevel = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));;		
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314
			// 시련의 땅에서 세트아이템 드랍여부
			gIsDropSetItemInCastleHuntZone  = GetPrivateProfileInt("GameServerInfo", "IsDropSetItemInCastleHuntZone", 0, gDirPath.GetNewPath("commonserver.cfg"));
			// 시련의 땅에서 세트아이템 드랍확률 (10000 기준)
			gSetItemInCastleHuntZoneDropRate = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropRate", 1, gDirPath.GetNewPath("commonserver.cfg"));
			// 시련의 땅에서 세트아이템 드랍 몬스터 레벨
			gSetItemInCastleHuntZoneDropLevel = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));;		
#endif

#ifdef FOR_NEW_TESTSERVER
	gTestServer	= GetPrivateProfileInt("GameServerInfo", "TestServer", 0, gDirPath.GetNewPath("commonserver.cfg"));	
#endif

			// 3 . 지하 군단
			gAttackEventRegenTime = GetPrivateProfileInt("GameServerInfo", "AttackEventRegenTime", 10, gDirPath.GetNewPath("commonserver.cfg"));

			// 4 . 황금 군단
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
			// 엘도라도 이벤트 초기화 시작
			gEledoradoEvent.SetEventState(gIsEledoradoEvent);
			gEledoradoEvent.Init();
#endif // MODIFY_ELDORADO_EVENT_OPTION_LOAD_SEQUEANCE_20070821

			// 5 . 백색마법사 군단
#ifdef MONSTER_HERD_SYSTEM_20031120
			// 반지 이벤트를 할지 여부를 얻어온다.
			GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			g_bDoRingEvent				= atoi(szTemp);
			// 오크를 잡았을 때 경품을 탈 확률 (1/n)
			GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRingOrcKillGiftRate		= atoi(szTemp);
			// 반지를 던졌을 때 경품을 탈 확률 (1/n)
			GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRingDropGiftRate			= atoi(szTemp);
			g_RingAttackEvent.Load(gDirPath.GetNewPath("RingAttackEvent.dat"));
#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// 공성전 전용서버 일때는 데빌스퀘어, 블러드캐슬, 카오스캐슬, 백색마법사 이벤트가 중지된다.
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

			// 6 . 러브팡팡 (러브러브) 이벤트

			// # REMOVE_LOVELOVE_EVENT_COUPLING_WITH_JOINSERVER_20060319
			// # 인증 서버와 연동해서 이벤트 처리를 한 것으로 보임(apple)
			//		- 인증 서버와 교환하는 0x30, 0x31 프로토콜 참고
			//		- 인증 서버에 러브하트 관련 DB 도 없음.
			//		- 인증 서버에서 러브하트 코드 제거 할 것임.
			GetPrivateProfileString("GameServerInfo", "EVENT1", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gEvent1						= atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayMax", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gEvent1ItemDropTodayCount	= 0;
			gEvent1ItemDropTodayMax		= atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayPercent", "10000000", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gEvent1ItemDropTodayPercent = atoi(szTemp);

			// 7 . 폭죽 이벤트
			gFireCrackerEvent			= GetPrivateProfileInt("GameServerInfo", "FireCrackerEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gFireCrackerDropRate		= GetPrivateProfileInt("GameServerInfo", "FireCrackerDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_ItemDropRateForgFireCracker	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForFireCracker", 2, gDirPath.GetNewPath("commonserver.cfg"));
			gOnlyFireCrackerEffectUse	= GetPrivateProfileInt("GameServerInfo", "OnlyFireCrackerEffectUse", 0, gDirPath.GetNewPath("commonserver.cfg"));

			// 8 . 메달 이벤트
			gMedalEvent					= GetPrivateProfileInt("GameServerInfo", "MedalEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldMedalDropRate			= GetPrivateProfileInt("GameServerInfo", "GoldMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));						// 금메달이 떨어질 확률
			gSilverMedalDropRate		= GetPrivateProfileInt("GameServerInfo", "SilverMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));					// 은메달이 떨어질 확률
			g_ItemDropRateForGoldMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForGoldMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));		// 금메달에서 아이템이 떨어질 확률
			g_ItemDropRateForSilverMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForSilverMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));	// 은메달에서 아이템이 떨어질 확률

			// 9 . 성탄의별 이벤트
			gXMasEvent					= GetPrivateProfileInt("GameServerInfo", "XMasEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_XMasEvent_StarOfXMasDropRate	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_StarOfXMasDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_XMasEvent_ItemDropRateForStarOfXMas	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_ItemDropRateForStarOfXMas", 2, gDirPath.GetNewPath("commonserver.cfg"));

			// 10 . 사랑의하트 이벤트
			gHeartOfLoveEvent			= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gHeartOfLoveDropRate		= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_ItemDropRateForgHeartOfLove	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForHeartOfLove", 2, gDirPath.GetNewPath("commonserver.cfg"));

			// 11 . NPC의 새해인사
			GetPrivateProfileString("GameServerInfo", "HappyNewYearTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gHappyNewYearNpcEvent		= atoi(szTemp);
			if( gHappyNewYearNpcEvent )	LogAddTD(lMsg.Get(573));	// [옵션] 새해 인사 NPC 대화 출력

			// 12 . NPC의 메리크리스마스
			GetPrivateProfileString("GameServerInfo", "MerryXMasTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gMerryXMasNpcEvent			= atoi(szTemp);
			if( gMerryXMasNpcEvent )	LogAddTD(lMsg.Get(572));	// [옵션] 크리스마스 NPC 대화 출력

#ifdef CHAOSCASTLE_SYSTEM_20040408 
			// 13 . 카오스 캐슬
			g_ChaosCastle.Load(gDirPath.GetNewPath("ChaosCastle.dat"));
			g_bChaosCastle				= GetPrivateProfileInt("GameServerInfo", "ChaosCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518									// 쿤둔의 표식 드롭확률을 읽는다.
			// 14 . 칼리마 - 쿤둔의표식
			g_iKundunMarkDropRate		= GetPrivateProfileInt("GameServerInfo", "KundunMarkDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804							// 신비의 구슬 드롭 확률
			// 15 . 신비의 구슬 드롭률
			g_iMysteriousBeadDropRate1 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate1", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iMysteriousBeadDropRate2 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate2", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iHiddenTreasureBoxOfflineRate = GetPrivateProfileInt("GameServerInfo", "HiddenTreasureBoxOfflineRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

#ifdef EVENT_MANAGEMENT_20040711
			// 16 . 이벤트 매니저 가동 여부
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
			// apple★20050204
			// 17 . 파란, 빨강 복주머니 이벤트
			gHappyPouchRedDropRate	= GetPrivateProfileInt("GameServerInfo", "HappyPouchRedDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));							
			gHappyPouchBlueDropRate	= GetPrivateProfileInt("GameServerInfo", "HappyPouchBlueDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));							
#endif
	
			// 18. 다크로드의 마음 이벤트
#ifdef DARK_LORD_HEART_EVENT_20040810						// 다크로드의 마음 이벤트 관련 확률을 읽는다.
			g_iDarkLordHeartDropRate		= GetPrivateProfileInt("GameServerInfo", "DarkLordHeartDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_iDarkLordHeartOffEventRate	= GetPrivateProfileInt("GameServerInfo", "DarkLoadHeartOffEventRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif		
			// 19. 크리스마스 리본상자 이벤트
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
			
		// 20. 발렌타인데이 초콜릿상자 이벤트( RKFKA )
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

		// 21. 화이트데이 사탕상자 이벤트( RKFKA )
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

	case MU_EVENT_DEVILSQUARE :			// 데빌스퀘어
		{
			g_DevilSquare.Load(gDirPath.GetNewPath("DevilSquare.dat"));
			gDevilSquareEvent			= GetPrivateProfileInt("GameServerInfo", "DevilSquareEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gEyesOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "EyesOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
			gKeyOfDevilSquareDropRate	= GetPrivateProfileInt("GameServerInfo", "KeyOfDevilSquareDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));	   
		}
		break;
	case MU_EVENT_BLOODCASTLE :			// 블러드캐슬
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
	case MU_EVENT_ATTACKEVENTTROOP :	// 지하 군단
		{
			gAttackEventRegenTime		= GetPrivateProfileInt("GameServerInfo", "AttackEventRegenTime", 10, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_EVENT_GOLDENTROOP :			// 황금 군단
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
	case MU_EVENT_WHITEMAGETROOP :		// 백색마법사 군단
		{
#ifdef MONSTER_HERD_SYSTEM_20031120
			char szTemp[256];	
			// 반지 이벤트를 할지 여부를 얻어온다.
			GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			g_bDoRingEvent					= atoi(szTemp);
			// 오크를 잡았을 때 경품을 탈 확률 (1/n)
			GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRingOrcKillGiftRate			= atoi(szTemp);
			// 반지를 던졌을 때 경품을 탈 확률 (1/n)
			GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
			g_iRingDropGiftRate				= atoi(szTemp);
			g_RingAttackEvent.Load(gDirPath.GetNewPath("RingAttackEvent.dat"));
#ifdef GAMESERVER_TO_CASTLESIEGESERVER	// 공성전 전용서버 일때는 데빌스퀘어, 블러드캐슬, 카오스캐슬, 백색마법사 이벤트가 중지된다.
			g_bDoRingEvent					= FALSE;
#endif	
			g_RingAttackEvent.EnableEvent (g_bDoRingEvent);
#endif
		}
		break;
	case MU_EVENT_LOVEPANGPANG :		// 러브팡팡 이벤트 (몬스터에게서 하루 일정개수의 사랑의하트 (14,12) 드롭됨)
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
	case MU_EVENT_FIRECRACKER :			// 폭죽 이벤트 (몬스터 죽일 때 폭죽 (14,11) 드롭됨 -> 한국은 아틀라스 이하등 특정지역만)
		{
			gFireCrackerEvent				= GetPrivateProfileInt("GameServerInfo", "FireCrackerEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gFireCrackerDropRate			= GetPrivateProfileInt("GameServerInfo", "FireCrackerDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_ItemDropRateForgFireCracker	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForFireCracker", 2, gDirPath.GetNewPath("commonserver.cfg"));
			gOnlyFireCrackerEffectUse		= GetPrivateProfileInt("GameServerInfo", "OnlyFireCrackerEffectUse", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_EVENT_MEDALION :			// 메달 이벤트 (뮤월드 각 지역에 따라 은메달, 금메달 등이 나온다.)
		{
			gMedalEvent						= GetPrivateProfileInt("GameServerInfo", "MedalEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gGoldMedalDropRate				= GetPrivateProfileInt("GameServerInfo", "GoldMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));						// 금메달이 떨어질 확률
			gSilverMedalDropRate			= GetPrivateProfileInt("GameServerInfo", "SilverMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));					// 은메달이 떨어질 확률
			g_ItemDropRateForGoldMedal		= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForGoldMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));		// 금메달에서 아이템이 떨어질 확률
			g_ItemDropRateForSilverMedal	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForSilverMedal", 2, gDirPath.GetNewPath("commonserver.cfg"));	// 은메달에서 아이템이 떨어질 확률
		}
		break;
	case MU_EVENT_XMASSTAR :			// 성탄의별 이벤트 (몬스터에게서 성탄의별이 드롭됨)
		{
			gXMasEvent						= GetPrivateProfileInt("GameServerInfo", "XMasEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			g_XMasEvent_StarOfXMasDropRate	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_StarOfXMasDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_XMasEvent_ItemDropRateForStarOfXMas	= GetPrivateProfileInt("GameServerInfo", "XMasEvent_ItemDropRateForStarOfXMas", 2, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_EVENT_HEARTOFLOVE :			// 사랑의하트 이벤트 (몬스터에게서 사랑의하트가 드롭됨)
		{
			gHeartOfLoveEvent				= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gHeartOfLoveDropRate			= GetPrivateProfileInt("GameServerInfo", "HeartOfLoveDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
			g_ItemDropRateForgHeartOfLove	= GetPrivateProfileInt("GameServerInfo", "ItemDropRateForHeartOfLove", 2, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_EVENT_SAY_HAPPYNEWYEAR :	// NPC가 새해인사를 한다.
		{
			GetPrivateProfileString("GameServerInfo", "HappyNewYearTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gHappyNewYearNpcEvent			= atoi(szTemp);
			if( gHappyNewYearNpcEvent )	LogAddTD(lMsg.Get(573));	// [옵션] 새해 인사 NPC 대화 출력
		}
		break;
	case MU_EVENT_SAY_MERRYXMAS :		// NPC가 메리 크리스마스라고 말한다.
		{
			GetPrivateProfileString("GameServerInfo", "MerryXMasTalkNpc", "0", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gMerryXMasNpcEvent				= atoi(szTemp);
			if( gMerryXMasNpcEvent )	LogAddTD(lMsg.Get(572));	// [옵션] 크리스마스 NPC 대화 출력
		}
		break;
#ifdef CHAOSCASTLE_SYSTEM_20040408 
	case MU_EVENT_CHAOSCASTLE :			// 카오스 캐슬
		{
			g_ChaosCastle.Load(gDirPath.GetNewPath("ChaosCastle.dat"));
			g_bChaosCastle				= GetPrivateProfileInt("GameServerInfo", "ChaosCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
#endif
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
	case MU_EVENT_CHRISTMAS_RIBBONBOX:	// 크리스마스 리본상자 이벤트. ( 몬스터에게서 빨강,초록,파랑 리본상자가 드롭됨 )
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
	case MU_EVENT_VALENTINESDAY_CHOCOLATEBOX:	// 발렌타인데이 초콜릿상자 이벤트( 몬스터에게서 분홍,빨강,파랑 초콜릿상자가 드롭됨 )
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
	case MU_EVENT_WHITEDAY_CANDYBOX:	// 화이트데이 사탕상자 이벤트( 몬스터에게서 연보라,주황,감청 사탕상자가 드롭됨 )
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

#ifdef ETC_OPTION_RELOAD_20040308		// 뮤의 옵션 중 이벤트 외의 것들만 리로드

void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType)
{
	char szTemp[256];	

	switch (eGameEtcType) {
	case MU_ETC_ALL :					// 모든 이벤트 외의 옵션 리로드
		{
			// 1 . 케릭터 생성 ON/OFF
			GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gCreateCharacter = atoi(szTemp);

			// 2 . 길드 생성, 삭제, 개설레벨
			GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildCreate = atoi(szTemp);
			LogAdd(lMsg.Get(569), gGuildCreate);
			GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildDestroy = atoi(szTemp);
			LogAdd(lMsg.Get(570), gGuildDestroy);		// "길드 삭제 옵션 %d"
			GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildCreateLevel = atoi(szTemp);
			LogAdd(lMsg.Get(571), gGuildCreateLevel);	// 길드생성 최소 레벨 %d
#ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
			// 수성길드 해체 제한
			g_bCastleGuildDestoyLimit	= GetPrivateProfileInt( "GameServerInfo", "CastleOwnerGuildDestroyLimit", 1, gDirPath.GetNewPath("commonserver.cfg"));
#endif
			
			// 3 . 거래 가능 여부 ON/OFF
			GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			bCanTrade				= atoi(szTemp);

			// 4 . 카오스박스 사용가능 여부 ON/OFF
			bCanChaosBox = GetPrivateProfileInt("GameServerInfo", "ChaosBox", 0, gDirPath.GetNewPath("commonserver.cfg"));

			// 5 . PK시 아이템 드롭 여부 ON/OFF
			GetPrivateProfileString("GameServerInfo", "PKItemDrop", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gPkItemDrop  = atoi(szTemp);

			// 6 . 아이템 드롭확률 조정
			GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gItemDropPer = atoi(szTemp);

			// 7 . 개인상점 개설가능 여부
#ifdef PERSONAL_SHOP_20040113			
			gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif

			// 8 . 스피드핵 관련 옵션
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
	case MU_ETC_CREATECHARACTER :		// 케릭터 생성
		{
			GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gCreateCharacter = atoi(szTemp);
		}
		break;
	case MU_ETC_GUILD :					// 길드 생성
		{
			GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildCreate = atoi(szTemp);
			LogAdd(lMsg.Get(569), gGuildCreate);
			GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildDestroy = atoi(szTemp);
			LogAdd(lMsg.Get(570), gGuildDestroy);		// "길드 삭제 옵션 %d"
			GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			gGuildCreateLevel = atoi(szTemp);
			LogAdd(lMsg.Get(571), gGuildCreateLevel);	// 길드생성 최소 레벨 %d

#ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
			// 수성길드 해체 제한
			g_bCastleGuildDestoyLimit	= GetPrivateProfileInt( "GameServerInfo", "CastleOwnerGuildDestroyLimit", 1, gDirPath.GetNewPath("commonserver.cfg"));
#endif
		}
		break;
	case MU_ETC_TRADE :					// 거래
		{
			GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
			bCanTrade				= atoi(szTemp);
		}
		break;
	case MU_ETC_USECHAOSBOX :			// 카오스박스 사용
		{
			bCanChaosBox = GetPrivateProfileInt("GameServerInfo", "ChaosBox", 0, gDirPath.GetNewPath("commonserver.cfg"));
		}
		break;
	case MU_ETC_PERSONALSHOP :			// 개인상점
		{
#ifdef PERSONAL_SHOP_20040113			
			gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));
#endif
		}
		break;
	case MU_ETC_PKITEMDROP :			// PK시 아이템 드롭여부
		{
			GetPrivateProfileString("GameServerInfo", "PKItemDrop", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gPkItemDrop  = atoi(szTemp);
		}
		break;
	case MU_ETC_ITEMDROPRATE :			// 아이템 드롭확률
		{
			GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));	
			gItemDropPer = atoi(szTemp);
		}
		break;
	case MU_ETC_SPEEDHACK :				// 스피드핵
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
	case MU_ETC_GAMEGUARD :				// 게임가드 체크섬 체크여부
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
	// 해당 이벤트는 commonserverinfo에서 이벤트가 활성화 되어도 
	// 이벤트를 진행하지 않는다
	// g_bEventOff ("Serverinfo.dat", "EventOff")
	if (g_bEventOff == TRUE)
	{
		// 행운의 동전 이벤트
#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
		g_bLuckyCoinEventOn					= FALSE;
		g_iLuckyCoinDropRate				= 0;
#endif // UPDATE_LUCKY_COIN_EVENT_20081029
		
		// 20. 크리스마스 리본상자 이벤트( RKFKA )
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
		
		// 설날 복주머니 몬스터 이벤트
#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131
		g_bNewYearLuckyBagMonsterEventOn	= FALSE;
#endif // UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131
		
		// 발렌타인데이 초콜릿상자 이벤트
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
		
		// 화이트데이 사탕상자 이벤트
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
