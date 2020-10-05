#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__

#include "MapClass.h"
#include "Shop.h"


#ifdef CHINA_HACKUSER_KICK_SYSTEM
#include "DirPath.h"
extern CDirPath	gDirPath;
#endif

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
//	#pragma comment(lib, "..\\lib\\ggsrvdll.lib") // Ŭ���̾�Ʈ ������ ���� ���̺귯��
	#pragma comment(lib, "ggsrvdll.lib") // ���� (b4nfter)
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	#pragma comment(lib, "ggsrvlib25.lib") // ���� (hnine)
#endif

extern int JoinServerPort;
extern int GameServerPort;
extern int DataServerPort;
extern int JoinServerConnected;
extern int DataServerConnected;

#if ENABLE_MC_SQL == 1
extern int  gConnectBonus;
extern int  gConnectBonusTime;
extern int  gConnectBonusCashPoints;
#endif // ENABLE_MC_SQL == 1

#ifdef ADD_GAMESERVER_PARAMETER_20050728
extern int DataServerPort2;
extern int ExDbPort;
#endif

extern int DevilSquareEventConnect;
extern int IsDevilSquareEventConnected;

extern int EventChipServerConnect;
extern int IsEventChipServerConnected;

#ifdef GAMESERVER_INFO_TCP_20031203
extern int IsConnectServerConnected;
#endif

extern short gGameServerCode;
extern BOOL SpeedHackPlayerBlock;
// �ŷ��� �����ϰ� �Ұ���..
extern BOOL bCanTrade;		
extern BOOL bCanChaosBox;							// ī���� �ڽ� ��� �����ϰ� �Ұ���..
extern BOOL gChaosEvent;
extern BOOL bCanConnectMember;						// ���� ������ ����� ������ ������..
extern BOOL bCanWarehouseLock;
extern int	gPkTime;
extern int  gEvent1;
extern int  gMerryXMasNpcEvent;
extern int  gHappyNewYearNpcEvent;
extern int	gEvent1ItemDropTodayCount;
extern int	gEvent1ItemDropTodayMax;
extern int  gEvent1ItemDropTodayPercent;

extern DWORD		dwgCheckSum[];
extern MapClass		MapC[];
extern CShop		ShopC[];
extern DWORD		gLevelExperience[];
extern DWORD		gItemNumberCount;
extern char			szClientVersion[];
extern char			szServerName[];
extern char			szGameServerExeSerial[];
//extern int			MapMinUserLevel[];
extern int			gLootingTime;
extern int			gPkItemDrop;
extern int			gItemDropPer;
extern int			gCharacterDeleteMinLevel;
extern BOOL			gCreateCharacter;
extern BOOL			gGuildCreate;
extern BOOL			gGuildDestroy;
extern int			gGuildCreateLevel;
#ifdef MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418
extern BOOL			g_bCastleGuildDestoyLimit;
#endif

extern BOOL			gItemSerialCheck;
extern float		gAddExperience;
extern DWORD		gAttackEventRegenTime;
extern int			gLanguage;
extern BOOL			gNonPK;
extern int			gYear;

#ifdef ADD_GIFTITEM_XMASEVENT_20081030
extern int			gXmasVisitorCount;
extern short         gXmasGiftForTest;
#endif

#ifdef ADD_ZEN_OPTION_20070903
extern float		gAddZen;
#endif

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
extern char			gGGModuleName[];
#endif

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
extern BOOL			gPkLimitFree;
#endif

#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
extern int			g_iApplyPCBangAdvantage;
#endif

// ĭ���� ���������Ʈ, ��ȭ�� ���� �����
#ifdef KANTURU_SPECIAL_ITEMDROP_20060808
extern BOOL g_bKanturuSpecialItemDropOn;
extern int	g_iKanturuMoonStoneDropRate;
extern int	g_iKanturuJewelOfHarmonyDropRate;
#endif

// ĭ���� ���� ������ ��
#ifdef KANTURU_MONSTER_ITEMDROP_20060807
extern BOOL g_bKanturuMayaHandItemDrop;
extern int	g_iKanturuMayaHandItemDropRate;
extern int	g_iKanturuMayaHandDropZenRate;
extern int	g_iKanturuMayaHandDropZen;

extern BOOL g_bKanturuNightmareItemDrop;
extern int	g_iKanturuNightmareItemDropRate;
extern int	g_iKanturuNightmareDropZenRate;
extern int	g_iKanturuNightmareDropZen;
#endif

#ifdef ADD_RAKLION_20080408
extern BOOL	g_bRaklionEvent;
extern BOOL	g_bRaklionSelupanItemDrop;
extern int	g_iRaklionSelupanItemDropRate;
extern int	g_iRaklionSelupanDropZenRate;
extern int	g_iRaklionSelupanDropZen;

extern BOOL	g_bBlockRaklionMapEnter;
#endif // ADD_RAKLION_20080408

#ifdef ADD_GAMBLING_20090120
extern BOOL g_bGamblingSystem;
extern int g_iGamblingValue;
extern int g_iGamblingShopTime;
#endif // ADD_GAMBLING_20090120

// ũ���̿��� ��ũ���� �����۹�
#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
extern BOOL	g_bCrywolfMonsterDarkElfItemDrop;
extern int	g_iCrywolfMonsterDarkElfItemDropRate;
extern int	g_iCrywolfMonsterDarkElfDropZenRate;
extern int	g_iCrywolfMonsterDarkElfDropZen;
#endif

// ũ���̿��� �������� �߰��� �����۹�
#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
extern BOOL	g_bCrywolfBossMonsterItemDrop;
extern int	g_iCrywolfBossMonsterItemDropRate;
extern int	g_iCrywolfBossMonsterDropZenRate;
extern int	g_iCrywolfBossMonsterDropZen;
#endif

// ũ�������� �������� �̺�Ʈ
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
extern BOOL	g_bRibbonBoxEvent;
extern int	g_iRedRibbonBoxDropLevelMin;
extern int	g_iRedRibbonBoxDropLevelMax;
extern int	g_iRedRibbonBoxDropRate;
extern int	g_iRedRibbonBoxDropZenRate;
extern int	g_iRedRibbonBoxDropZen;

extern int	g_iGreenRibbonBoxDropLevelMin;
extern int	g_iGreenRibbonBoxDropLevelMax;
extern int	g_iGreenRibbonBoxDropRate;
extern int	g_iGreenRibbonBoxDropZenRate;
extern int	g_iGreenRibbonBoxDropZen;

extern int	g_iBlueRibbonBoxDropLevelMin;
extern int	g_iBlueRibbonBoxDropLevelMax;
extern int	g_iBlueRibbonBoxDropRate;
extern int	g_iBlueRibbonBoxDropZenRate;
extern int	g_iBlueRibbonBoxDropZen;
#endif //#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212

// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ
#ifdef VALENTINES_DAY_BOX_EVENT_20060124
extern BOOL	g_bChocolateBoxEvent;
extern int	g_iPinkChocolateBoxDropLevelMin;
extern int	g_iPinkChocolateBoxDropLevelMax;
extern int	g_iPinkChocolateBoxDropRate;
extern int	g_iPinkChocolateBoxDropZenRate;
extern int	g_iPinkChocolateBoxDropZen;

extern int	g_iRedChocolateBoxDropLevelMin;
extern int	g_iRedChocolateBoxDropLevelMax;
extern int	g_iRedChocolateBoxDropRate;
extern int	g_iRedChocolateBoxDropZenRate;
extern int	g_iRedChocolateBoxDropZen;

extern int	g_iBlueChocolateBoxDropLevelMin;
extern int	g_iBlueChocolateBoxDropLevelMax;
extern int	g_iBlueChocolateBoxDropRate;
extern int	g_iBlueChocolateBoxDropZenRate;
extern int	g_iBlueChocolateBoxDropZen;
#endif //#ifdef VALENTINES_DAY_BOX_EVENT_20060124

// ȭ��Ʈ���� �������� �̺�Ʈ
#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
extern BOOL	g_bCandyBoxEvent;
extern int	g_iLightPurpleCandyBoxDropLevelMin;
extern int	g_iLightPurpleCandyBoxDropLevelMax;
extern int	g_iLightPurpleCandyBoxDropRate;
extern int	g_iLightPurpleCandyBoxDropZenRate;
extern int	g_iLightPurpleCandyBoxDropZen;

extern int	g_iVermilionCandyBoxDropLevelMin;
extern int	g_iVermilionCandyBoxDropLevelMax;
extern int	g_iVermilionCandyBoxDropRate;
extern int	g_iVermilionCandyBoxDropZenRate;
extern int	g_iVermilionCandyBoxDropZen;

extern int	g_iDeepBlueCandyBoxDropLevelMin;
extern int	g_iDeepBlueCandyBoxDropLevelMax;
extern int	g_iDeepBlueCandyBoxDropRate;
extern int	g_iDeepBlueCandyBoxDropZenRate;
extern int	g_iDeepBlueCandyBoxDropZen;
#endif //#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
extern BOOL	g_bHallowinDayEventOn;
extern int	g_iHallowinDayEventItemDropRate;
extern int	g_iHallowinDayEventJOLBlessDropRate;
extern int	g_iHallowinDayEventJOLAngerDropRaTe;
extern int	g_iHallowinDayEventJOLScreamDropRate;
extern int	g_iHallowinDayEventJOLFoodDropRate;
extern int	g_iHallowinDayEventJOLDrinkDropRate;
extern int	g_iHallowinDayEventJOLPolymorphRingDropRate;
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
extern BOOL g_bSantaPolymorphRingDropOn;
extern int	g_iSantaPolymorphRingDropRate;
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	//�̺�Ʈ ��/����
extern BOOL g_bNewYearLuckyBagMonsterEventOn;
#endif

#ifdef ADD_THIRD_WING_20070525	// �ܵ����ǺҲ� ���Ȯ�� extern
extern int	g_iCondorFlameDropRate;
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
extern BOOL g_bCherryBlossomEventOn;
extern int	g_bCherryBlossomEventItemDropRate;
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

// ��ź�� �� �̺�Ʈ
extern BOOL gXMasEvent;
extern int	g_XMasEvent_StarOfXMasDropRate;
extern int	g_XMasEvent_ItemDropRateForStarOfXMas;

// ���� �̺�Ʈ
extern BOOL	gFireCrackerEvent;
extern int	gFireCrackerDropRate;
extern int	g_ItemDropRateForgFireCracker;
extern int	gOnlyFireCrackerEffectUse;		// ������ ����߷�����, ȿ���� ������ �Ѵ�


extern BOOL	gHeartOfLoveEvent;
extern int	gHeartOfLoveDropRate;
extern int	g_ItemDropRateForgHeartOfLove;


// �޴� �̺�Ʈ
extern BOOL	gMedalEvent;	// �޴� �̺�Ʈ �����ϴ°�?
extern int	gGoldMedalDropRate;	// �ݸ޴��� ������ Ȯ��
extern int	gSilverMedalDropRate;	// ���޴��� ������ Ȯ��

extern int	g_ItemDropRateForGoldMedal;	// �ݸ޴޿��� �������� ���� Ȯ��
extern int	g_ItemDropRateForSilverMedal;	// ���޴޿��� �������� ���� Ȯ��

// �̺�ƮĨ �̺�Ʈ
extern BOOL gEventChipEvent;				// �̺�ƮĨ �̺�Ʈ �����ϴ°�?
extern int	gBoxOfGoldDropRate;			// Ȳ���� ���ڰ� ������ Ȯ��
extern int	g_ItemDropRateForBoxOfGold;	// Ȳ���� ���ڿ��� �������� ���� Ȯ��
extern int	g_EventChipDropRateForBoxOfGold;	// Ȳ���� ���ڿ����̺�ƮĨ�� ������ Ȯ��

extern int	gApplyHeroSystem;
extern int	gSpeedHackPenalty;

// �Ǹ��� ���� �̺�Ʈ
extern BOOL gDevilSquareEvent;

// �Ǹ��� �� ���� Ȯ��
extern int	 gEyesOfDevilSquareDropRate;
// �Ǹ��� ���� ���� Ȯ��
extern int	 gKeyOfDevilSquareDropRate;

extern int	 gDQChaosSuccessRateLevel1;
extern int	 gDQChaosSuccessRateLevel2;
extern int	 gDQChaosSuccessRateLevel3;
extern int	 gDQChaosSuccessRateLevel4;
#ifdef DEVILSQUARE_EXTEND_20050221		// ī���� ���� �߰�
extern int	 gDQChaosSuccessRateLevel5;
extern int	 gDQChaosSuccessRateLevel6;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010	// ī�������� Ȯ�� ���� �߰�
extern int	 gDQChaosSuccessRateLevel7;		
#endif

// ���� ĳ��
#ifdef FOR_BLOODCASTLE
extern BOOL			g_bBloodCastle;
extern int			g_iBloodCastle_StartHour;

extern BOOL			g_bStoneItemDrop;
extern int			g_iStoneDropRate;
extern int			g_iAngelKingsPaperDropRate;
extern int			g_iBloodBoneDropRate;

#ifdef UPDATE_BLOODECASTLE_SCADULE_20070702
extern int			g_iBloodCastle_odd_Even_Hour;
#endif

#endif

// ī���� ĳ��
#ifdef CHAOSCASTLE_SYSTEM_20040408
extern BOOL			g_bChaosCastle;							// ���� ���Ͽ��� �о�� ��
#endif

#ifdef MONSTER_HERD_SYSTEM_20031120
extern BOOL			g_bDoRingEvent;							// ���� �̺�Ʈ�� �� ���ΰ� ?
extern INT			g_iRingOrcKillGiftRate;					// ��ũ�� ����� �� ��ǰ�� Ż Ȯ�� (1/n)
extern INT			g_iRingDropGiftRate;					// ������ ������ �� ��ǰ�� Ż Ȯ�� (1/n)
#endif

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
extern BOOL			g_bDoXMasEvent;
extern INT			g_nXMasDropRate;
extern INT			g_nXMasDropZen;
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#ifdef ADD_XMASEVENT_LUCKNUMBER
extern INT			g_nXMasLuckNumber1st;
extern INT			g_nXMasLuckNumber2nd;
#endif // ADD_XMASEVENT_LUCKNUMBER

#ifdef EVENT_MANAGEMENT_20040711
extern BOOL			g_bEventManagerOn;						// �̺�Ʈ ������ ���� ����
#endif


// Į���� ��
#ifdef ADD_NEW_MAP_KALIMA_20040518
extern INT			g_iKundunMarkDropRate;					// ����� ǥ�� ���Ȯ��
#endif

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214						// ������ ǥ�� ���Ȯ��
extern INT			g_iMarkOfTheLord;
#endif


#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531
extern int			g_iJapan1StAnivItemDropRate;			// �Ϻ� 1�ֳ� ��� ������ ���Ȯ��
#endif
	
#ifdef DARK_LORD_HEART_EVENT_20040810						// ��ũ�ε��� ���� ���� Ȯ��������
extern INT			g_iDarkLordHeartDropRate;				// ��ũ�ε��� ���� ���Ȯ��
extern INT			g_iDarkLordHeartOffEventRate;			// ��ũ�ε��� ���� �������� ��ǰ��÷ Ȯ��
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
extern INT			g_iMysteriousBeadDropRate1;				// �ź��� ���� ��� Ȯ��
extern INT			g_iMysteriousBeadDropRate2;				// �ź��� ���� ��� Ȯ��
extern INT			g_iHiddenTreasureBoxOfflineRate;		// ������ �������� �̺�Ʈ �������� ��ǰȮ��
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029							// ����� ���� ��� Ȯ��
extern int				g_bLuckyCoinEventOn;			// ����� ���� �̺�Ʈ ����
extern int				g_iLuckyCoinDropRate;			// ����� ���� ��� Ȯ��
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
extern int				g_bEventOff;						// �ż� �̺�Ʈ ��Ȱ��ȭ
#endif // ADD_NEWSERVER_EVENTOFF_20081114

extern int	gZenDurationTime;


extern char gDevilSquareEventServerIp[20];
extern char gEventChipServerIp[20];

extern char gHackLogServerIp[20];

extern DWORD		gAttackSpeedTimeLimit;
extern int			bIsIgnorePacketSpeedHackDetect;
extern DWORD		gHackCheckCount;
extern float		gDecTimePerAttackSpeed;
extern int			gMinimumAttackSpeedTime;			// �ּ� ���� �ð�
extern int			gDetectedHackKickCount;				// ���ǵ��� ������ �����ų ī��Ʈ
extern int			gIsKickDetecHackCountLimit;			// ���ǵ��� ������ �����Ű�°�?

extern int			gQuestNPCTeleportTime;


extern int			gEnableBattleSoccer;		// ��Ʋ��Ŀ�� �Ҽ� �ִ°�?
extern int			gEnableEventNPCTalk;


#ifdef FOR_JAPAN
extern BOOL			gTamaJJangEvent;			// Ÿ��¯ �̺�Ʈ�� �����ϴ°�?
extern BOOL			gAppearTamaJJang;			// Ÿ��¯�� �����ϴ°�?
extern int			gTamaJJangKeepTime;			// Ÿ��¯ ���ӽð�

extern int			gTamaJJangDisappearTime;			// Ÿ��¯ ������� �ð�
extern int			gTamaJJangDisappearTimeRandomRange;	// Ÿ��¯�� ������� �ð��� �������� �����ϱ� ���� ����

extern int			gTamaJJangTime;
#endif

#ifdef ITEMDROP_RINGOFTRANSFORM
extern int			gIsItemDropRingOfTransform;
extern int			gItemDropRingOfTransform;
#endif

extern int			gEnableServerDivision;

extern BOOL			gIsEledoradoEvent;
extern DWORD		gEledoradoGoldGoblenRegenTime;
extern DWORD		gEledoradoTitanRegenTime;
extern DWORD		gEledoradoGoldDerconRegenTime;
extern DWORD		gEledoradoDevilLizardKingRegenTime;
extern DWORD		gEledoradoDevilTantarosRegenTime;

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
extern DWORD		gGoldenRabbitRegenTime;
extern DWORD		gGoldenDarknightRegenTime;
extern DWORD		gGoldenDevilRegenTime;
extern DWORD		gGoldenStoneMonsterRegenTIme;
extern DWORD		gGoldenCrustEventRegenTime;
extern DWORD		gGoldenSatirosRegenTime;
extern DWORD		gGoldenTwintailRegenTime;
extern DWORD		gGoldenIronKnightRegenTime;
extern DWORD		gGoldenNeipinRegenTime;
extern DWORD		gGoldenGreatDragonRegenTime;
#endif

extern int			gEledoradoGoldGoblenItemDropRate;		// ������ ��� Ȯ��
extern int			gEledoradoTitanItemDropRate;
extern int			gEledoradoGoldDerconItemDropRate;
extern int			gEledoradoDevilLizardKingItemDropRate;
extern int			gEledoradoDevilTantarosItemDropRate;

extern int			gEledoradoGoldGoblenExItemDropRate;		// ������Ʈ ������ ���Ȯ��
extern int			gEledoradoTitanExItemDropRate;
extern int			gEledoradoGoldDerconExItemDropRate;
extern int			gEledoradoDevilLizardKingExItemDropRate;
extern int			gEledoradoDevilTantarosExItemDropRate;

extern BOOL			gWriteChatLog;
extern BOOL			gDisconnectHackUser;

extern BOOL			gWriteSkillLog;

extern BOOL			gUseNPGGChecksum;


#ifdef CHECK_ATTACK_AREA
extern int			gNotAttackAreaKickCount;
extern int			gIsKickNotAttackAreaCountLimit;
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
extern int			gBlockHackUserCharacter;
extern int			gBlockHackUserAccount;
#endif

extern int			gMonsterHPAdjust;

#ifdef AUTH_GAMESERVER
extern char			szAuthKey[20];									// ���� ���� ������ ���� �˻�Ű (�ڵ� �������� üũ �Ѵ�)
#endif

#ifdef HAPPY_POUCH
// apple��20050204
extern INT			gHappyPouchRedDropRate;
extern INT			gHappyPouchBlueDropRate;
#endif

#ifdef PERSONAL_SHOP_20040113	// ���λ��� ���� ����
extern int			gDoPShopOpen;
#endif

#ifdef CHECK_PENETRATION_SKILL
extern int			gEnableCheckPenetrationSkill;
#endif

#ifdef MU_CHN_OFFLINE_GIFT_EVENT_20040908
extern BOOL			gbChnOfflineGiftEvent;							// �߱� ���� �̺�Ʈ�� �ϴ°� ?
extern int			giChnOfflineGiftEventDropRate;					// �߱� ���� ��� Ȯ�� (1/1000000)
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
extern int			giKundunRefillHPSec;							// ��� �� ȸ���� �ʴ�
extern int			giKundunRefillHP;								// ��� �� ȸ���� ( 1 / 25 ����)
extern int			giKundunRefillHPTime;							// �� ȸ�� �ð� (SEC)
#endif

#ifdef EXTEND_LOG_SYSTEM_20060202
extern int			giKundunHPLogSaveTime;
#endif

#ifdef DARKLORD_WORK
extern BOOL			gIsDropDarkLordItem;
extern int			gSleeveOfLordDropRate;		// ������ �Ҹ� ��� Ȯ��
extern int			gSleeveOfLordDropLevel;		// ������ �Ҹ� ��� ����

extern int			gSoulOfDarkHorseDropRate;	// ��ũȣ���� ��ȥ ��� Ȯ��
extern int			gSoulOfDarkHorseropLevel;	// ��ũȣ���� ��ȥ ��� ����

extern int			gSoulOfDarkSpiritDropRate;	// ��ũ���Ǹ� ��� Ȯ��
extern int			gSoulOfDarkSpiritDropLevel;	// ��ũ���Ǹ� ��� ����
extern float		gDarkSpiritAddExperience;
#endif

#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
extern int			gIsDropGemOfDefend;
extern int			gGemOfDefendDropRate;
extern int			gGemOfDefendDropLevel;
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314
extern int			gIsDropSetItemInCastleHuntZone;		// �÷��� ������ ��Ʈ������ �������
extern int			gSetItemInCastleHuntZoneDropRate;	// �÷��� ������ ��Ʈ������ ���Ȯ�� (10000 ����)
extern int			gSetItemInCastleHuntZoneDropLevel;	// �÷��� ������ ��Ʈ������ ��� ���� ����
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// �߱��߰�> . �Ƿε� �ý��� ���뿩��, ����ġ, ������Ȯ�� ����
extern BOOL			g_bChnPlayTimeLimitOn;				// �Ƿε� �ý��� ���뿩��
extern INT			g_iChnPlayTimeLimitLevel1Exp;		// �Ƿνð� ����ġ ȹ���
extern INT			g_iChnPlayTimeLimitLevel1ItemDrop;	// �Ƿνð� ������ ��ӷ�
extern INT			g_iChnPlayTimeLimitLevel2Exp;		// ���ؽð� ����ġ ȹ���
extern INT			g_iChnPlayTimeLimitLevel2ItemDrop;	// ���ؽð� ������ ��ӷ�
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// ��Ʈ�� : �ɼ�
extern BOOL			g_bVtmPlayTimeLimitOn;				// �Ƿε� �ý��� ���뿩��
extern INT			g_iVtmPlayTimeLimitNoticeTime;		// �⺻ ���� �ð� (����Ÿ�� ���±��� ���� �ð� ����)

extern INT			g_iVtmPlayTimeLimitLevel1Time;		// �Ƿνð� üũ�ð�
extern INT			g_iVtmPlayTimeLimitLevel1NoticeTime;// �Ƿνð� �����ð�
extern INT			g_iVtmPlayTimeLimitLevel1Exp;		// �Ƿνð� ����ġ ȹ���
extern INT			g_iVtmPlayTimeLimitLevel1ItemDrop;	// �Ƿνð� ������ ��ӷ�

extern INT			g_iVtmPlayTimeLimitLevel2Time;		// ���ؽð� üũ�ð�
extern INT			g_iVtmPlayTimeLimitLevel2NoticeTime;// ���ؽð� �����ð�
extern INT			g_iVtmPlayTimeLimitLevel2Exp;		// ���ؽð� ����ġ ȹ���
extern INT			g_iVtmPlayTimeLimitLevel2ItemDrop;	// ���ؽð� ������ ��ӷ�
#endif

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
extern int          g_iUseCharacterAutoRecuperationSystem;
extern int          g_iCharacterRecuperationMaxLevel;
#endif

#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706
extern int			g_iServerGroupGuildChatting;
extern int			g_iServerGroupUnionChatting;
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
extern BOOL			g_bFenrirStuffItemDrop;

extern int			g_iFenrirStuff_01_DropLv_Min;
extern int			g_iFenrirStuff_01_DropLv_Max;
extern int			g_iFenrirStuff_01_DropMap;
extern int			g_iFenrirStuff_01_DropRate;

extern int			g_iFenrirStuff_02_DropLv_Min;
extern int			g_iFenrirStuff_02_DropLv_Max;
extern int			g_iFenrirStuff_02_DropMap;
extern int			g_iFenrirStuff_02_DropRate;

extern int			g_iFenrirStuff_03_DropLv_Min;
extern int			g_iFenrirStuff_03_DropLv_Max;
extern int			g_iFenrirStuff_03_DropMap;
extern int			g_iFenrirStuff_03_DropRate;

extern int			g_iFenrirRepairRate;
extern int			g_iFenrirDefaultMaxDurSmall;
extern int			g_iFenrirElfMaxDurSmall;

extern int			g_iFenrir_01Level_MixRate;
extern int			g_iFenrir_02Level_MixRate;
extern int			g_iFenrir_03Level_MixRate;
#endif


#ifdef MU_CRYWOLF_BENEFIT_20051215
extern int			g_iCrywolfApplyMvpBenefit; 	
#endif 

#ifdef MU_CRYWOLF_PENALTY_20051215
extern int			g_iCrywolfApplyMvpPenalty; 	
#endif

#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413
extern int			g_iSkillDistanceCheck;		// ��ų ���� �Ÿ� üũ	0 : üũ ����, 1 : üũ ��
extern int			g_iSkillDistanceCheckTemp;	// ��ų ���� �Ÿ� üũ �� ���� �� : defalut 2�� ����
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
	extern	int		g_iSkillDistanceKick;		// ��ų �Ÿ� �� ����� ���� ���� ���� 0:�������� ����, 1:��������
	extern	int		g_iSkillDistanceKickCount;	// ��ų �Ÿ� �� ���� ���� ī��Ʈ.
#endif
	#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421
		extern	int		g_iSkillDiatanceKickCheckTime;	// ��ų �Ÿ� �� ����� ��ȿ ī��Ʈ �ð� 
	#endif
#endif

// CODEREVIEW_SHIELD3 ; �������� ��� ���� ��ũ��Ʈ
#ifdef ADD_SHIELD_POINT_01_20060403
extern BOOL	g_CompoundPotionDropOn;				// ���� ���� ��� On(1)/Off(0)
extern int	g_iCompoundPotionLv1DropRate;		// ���� ���� ���� ��� Ȯ�� / 10000
extern int	g_iCompoundPotionLv2DropRate;		// �߰� ���� ���� ��� Ȯ�� / 10000
extern int	g_iCompoundPotionLv3DropRate;		// ū ���� ���� ��� Ȯ�� / 10000
extern int	g_iCompoundPotionLv1DropLevel;		// ���� ���� ���� ��� ����
extern int	g_iCompoundPotionLv2DropLevel;		// �߰� ���� ���� ��� ����
extern int	g_iCompoundPotionLv3DropLevel;		// ū ���� ���� ��� ����
	#ifdef UPDATE_SHIELD_OPTION_20060427
		extern BOOL	g_bShieldComboMissOptionOn;			// PvP ���� �̽� �� �޺� ��� �ɼ� 1 : �޺� ���, 0 : �޺� ����
		extern int	g_iShieldPotionLv1MixSuccessRate;	// ���� SD ���� ���� Ȯ��
		extern int  g_iShieldPotionLv1MixMoney;			// ���� SD ���� ���� �ʿ� ��
		extern int	g_iShieldPotionLv2MixSuccessRate;	// �߰� SD ���� ���� Ȯ��
		extern int  g_iShieldPotionLv2MixMoney;			// �߰� SD ���� ���� �ʿ� ��
		extern int	g_iShieldPotionLv3MixSuccessRate;	// ū SD ���� ���� Ȯ��
		extern int  g_iShieldPotionLv3MixMoney;			// ū SD ���� ���� �ʿ� ��
	#endif // UPDATE_SHIELD_OPTION_20060427
		
	#ifdef UPDATE_SHIELD_CALCULATE_GAGE_20060428
		extern int	g_iShieldGageConstA;				// ���� ������ ���� ����� 1
		extern int	g_iShieldGageConstB;				// ���� ������ ���� ����� 2
	#endif // UPDATE_SHIELD_CALCULATE_GAGE_20060428
#endif // ADD_SHIELD_POINT_01_20060403
		
#ifdef FOR_NEW_TESTSERVER
extern BOOL			gTestServer;
#endif

#ifdef MAKE_ITEM_STOP
extern int			g_iMakeStop;
#endif

#ifdef MODIFY_KANTURU_MAPNUMBER_CHECK_20060807
extern int			g_iBlockKanturuMapEnter;
extern int			g_iBlockCastleSiegeMapEnter;
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
extern	BOOL	g_bIllusionTampleEvent;				// �̺�Ʈ ���� ����
extern	int		g_iOldScrollDropRate;				// ���� �η縶�� ���� Ȯ��
extern	int		g_iCovenantOfIllusionDropRate;		// ȯ�������� ���� ���� Ȯ��
extern	BOOL	g_bUseMaxLevelIllusionTemple;		// 400 ���� ���� ȯ�� ��� ��� ����
extern	BOOL	g_bSaveIllusionTempleRankingPoint;	// ȯ�� ����� ��ŷ ���� ���忩��

#ifdef UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710	// ȯ����� ��ũ��Ʈ�߰�
extern int		g_iIllusionTempleRewardPercent;		// ȯ����� ���� ����ġ ����
extern int		g_btIllusionTempleMinUseSkillCnt;	// ȯ����� �ּһ�뽺ų 
#endif	// UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710
#endif

#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
extern int		g_iIllusionTempleRewardPercentMaster;	// ȯ����� ���� ����ġ ����(�����ͷ���)
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
extern	int		gML_OldScrollDropRate;				// 6 ���� �η縶�� ���� Ȯ��
extern	int		gML_CovenantOfIllusionDropRate;		// 6 ȯ�������� ���� ���� Ȯ��
extern	int		gML_AngelKingsPaperDropRate;		// 8 ��õ���� �� ���� Ȯ��xbr
extern	int		gML_BloodBoneDropRate;				// 8 ���庻 ���� Ȯ��
extern	int		gML_EyesOfDevilSquareDropRate;		// 7 �Ǹ��� �� ���� Ȯ��
extern	int		gML_KeyOfDevilSquareDropRate;		// 7 �Ǹ��� ���� ���� Ȯ��
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123

#ifdef UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220
extern	int		g_ShadowPahtomMaxLevel;				// ���������� ���� �ִ� ����
#endif	// UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410
extern	int		g_bChangeCharacterName;				// ĳ���� ���� ����
#endif

extern	int		g_bCheckSpeedHack;

#ifdef MODIFY_SPEEDHACK_20080605
extern	BOOL	g_bAbilityDebug;
#endif // MODIFY_SPEEDHACK_20080605

#ifdef ADD_NEWSERVER_20081113
extern	BOOL	g_bNewServer;
#endif // ADD_NEWSERVER_20081113

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
extern	BOOL	g_bUseMapMoveCheckSum;
#endif // ADD_MAPMOVE_PROTOCOL_20090327

extern void GameMainInit(HWND hWnd);
extern void GameMainFree();
extern BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg);
extern BOOL GMJoinServerConnect(char* ConnectServer, DWORD wMsg);

#ifdef GAMESERVER_INFO_TCP_20031203
extern BOOL ConnectServerConnect(char *ServerIP, DWORD wMsg);
#endif

extern BOOL GMRankingServerConnect(char* RankingServer, DWORD wMsg);
extern BOOL GMEventChipServerConnect(char* ServerIP, DWORD wMsg);

extern void GMServerMsgProc(WPARAM wParam, LPARAM lParam);
extern void GMClientMsgProc(WPARAM wParam, LPARAM lParam);
extern void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam);
extern void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam);

#ifdef _NEW_EXDB_
//##EXDB##
extern BOOL ExDataServerConnect(char *ConnectServer, DWORD wMsg);
extern void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam);
//##EXDB##

#endif


extern void GMRankingClientMsgProc(WPARAM wParam, LPARAM lParam);
extern void GMEventChipClientMsgProc(WPARAM wParam, LPARAM lParam);

#ifdef GAMESERVER_INFO_TCP_20031203
extern void GMConnectServerClientMsgProc(WPARAM wParam, LPARAM lParam);
#endif

extern void GameMonsterAllCloseAndReLoad();
extern BOOL GMDataServerConnect(char *ConnectServer, DWORD wMsg);
extern void GameServerInfoSend();
extern void ReadCommonServerInfo();
extern BOOL gJoomin15Check(char * szJN);

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
extern void ReadGameGuardUpdateInfo();
#endif

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
BOOL gJoominCheck(char * szJN, int iLimitAge);
#endif

#ifdef GAMESERVER_INFO_EXTENDED_20040625	// ���Ӽ������� ���Ӽ����� ������ �߰����� ����
extern void GameServerExtInfoSend();
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
extern int GetWarehouseUsedHowMuch( int UserLevel,int UserMasterLevel, int IsLock );
#else
extern int GetWarehouseUsedHowMuch(int UserLevel, int IsLock);
#endif

extern void GameServerInfoSendStop();
extern void GameServerInfoSendStart();

#ifdef AUTH_GAMESERVER		// 4byte�� �� �̺�Ʈ ������ ��´�
extern int	GetEventFlag();
#endif

#ifdef ETC_OPTION_RELOAD_20040308		// ���� �ɼ� �� �̺�Ʈ ���� �͵鸸 ���ε�
extern void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType);
extern void ReadEventInfo(MU_EVENT_TYPE eEventType);
#endif

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
extern void RenderCommonServerInfo();
#endif // ADD_NEWSERVER_EVENTOFF_20081114


#endif
