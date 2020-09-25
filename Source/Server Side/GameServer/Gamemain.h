#ifndef __GAMEMAIN_H__
#define __GAMEMAIN_H__

#include "MapClass.h"
#include "Shop.h"


#ifdef CHINA_HACKUSER_KICK_SYSTEM
#include "DirPath.h"
extern CDirPath	gDirPath;
#endif

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
//	#pragma comment(lib, "..\\lib\\ggsrvdll.lib") // 클라이언트 인증용 서버 라이브러리
	#pragma comment(lib, "ggsrvdll.lib") // 수정 (b4nfter)
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	#pragma comment(lib, "ggsrvlib25.lib") // 수정 (hnine)
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
// 거래를 가능하게 할건지..
extern BOOL bCanTrade;		
extern BOOL bCanChaosBox;							// 카오스 박스 사용 가능하게 할건지..
extern BOOL gChaosEvent;
extern BOOL bCanConnectMember;						// 접속 가능한 멤버를 구성할 것인지..
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

// 칸투르 문스톤펜던트, 조화의 보석 드랍률
#ifdef KANTURU_SPECIAL_ITEMDROP_20060808
extern BOOL g_bKanturuSpecialItemDropOn;
extern int	g_iKanturuMoonStoneDropRate;
extern int	g_iKanturuJewelOfHarmonyDropRate;
#endif

// 칸투르 몬스터 아이템 백
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

// 크라이울프 다크엘프 아이템백
#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
extern BOOL	g_bCrywolfMonsterDarkElfItemDrop;
extern int	g_iCrywolfMonsterDarkElfItemDropRate;
extern int	g_iCrywolfMonsterDarkElfDropZenRate;
extern int	g_iCrywolfMonsterDarkElfDropZen;
#endif

// 크라이울프 보스몬스터 발가스 아이템백
#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
extern BOOL	g_bCrywolfBossMonsterItemDrop;
extern int	g_iCrywolfBossMonsterItemDropRate;
extern int	g_iCrywolfBossMonsterDropZenRate;
extern int	g_iCrywolfBossMonsterDropZen;
#endif

// 크리스마스 리본상자 이벤트
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

// 발렌타인데이 초콜릿상자 이벤트
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

// 화이트데이 사탕상자 이벤트
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

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	//이벤트 온/오프
extern BOOL g_bNewYearLuckyBagMonsterEventOn;
#endif

#ifdef ADD_THIRD_WING_20070525	// 콘도르의불꽃 드롭확률 extern
extern int	g_iCondorFlameDropRate;
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
extern BOOL g_bCherryBlossomEventOn;
extern int	g_bCherryBlossomEventItemDropRate;
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

// 성탄의 별 이벤트
extern BOOL gXMasEvent;
extern int	g_XMasEvent_StarOfXMasDropRate;
extern int	g_XMasEvent_ItemDropRateForStarOfXMas;

// 폭죽 이벤트
extern BOOL	gFireCrackerEvent;
extern int	gFireCrackerDropRate;
extern int	g_ItemDropRateForgFireCracker;
extern int	gOnlyFireCrackerEffectUse;		// 폭죽을 떨어뜨렸을때, 효과만 나오게 한다


extern BOOL	gHeartOfLoveEvent;
extern int	gHeartOfLoveDropRate;
extern int	g_ItemDropRateForgHeartOfLove;


// 메달 이벤트
extern BOOL	gMedalEvent;	// 메달 이벤트 진행하는가?
extern int	gGoldMedalDropRate;	// 금메달이 떨어질 확률
extern int	gSilverMedalDropRate;	// 은메달이 떨어질 확률

extern int	g_ItemDropRateForGoldMedal;	// 금메달에서 아이템이 나올 확률
extern int	g_ItemDropRateForSilverMedal;	// 은메달에서 아이템이 나올 확률

// 이벤트칩 이벤트
extern BOOL gEventChipEvent;				// 이벤트칩 이벤트 진행하는가?
extern int	gBoxOfGoldDropRate;			// 황금의 상자가 떨어질 확률
extern int	g_ItemDropRateForBoxOfGold;	// 황금의 상자에서 아이템이 나올 확률
extern int	g_EventChipDropRateForBoxOfGold;	// 황금의 상자에서이벤트칩이 떨어질 확룰

extern int	gApplyHeroSystem;
extern int	gSpeedHackPenalty;

// 악마의 광장 이벤트
extern BOOL gDevilSquareEvent;

// 악마의 눈 나올 확률
extern int	 gEyesOfDevilSquareDropRate;
// 악마의 열쇠 나올 확률
extern int	 gKeyOfDevilSquareDropRate;

extern int	 gDQChaosSuccessRateLevel1;
extern int	 gDQChaosSuccessRateLevel2;
extern int	 gDQChaosSuccessRateLevel3;
extern int	 gDQChaosSuccessRateLevel4;
#ifdef DEVILSQUARE_EXTEND_20050221		// 카오스 조합 추가
extern int	 gDQChaosSuccessRateLevel5;
extern int	 gDQChaosSuccessRateLevel6;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010	// 카오스조합 확률 변수 추가
extern int	 gDQChaosSuccessRateLevel7;		
#endif

// 블러드 캐슬
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

// 카오스 캐슬
#ifdef CHAOSCASTLE_SYSTEM_20040408
extern BOOL			g_bChaosCastle;							// 원래 파일에서 읽어야 함
#endif

#ifdef MONSTER_HERD_SYSTEM_20031120
extern BOOL			g_bDoRingEvent;							// 반지 이벤트를 할 것인가 ?
extern INT			g_iRingOrcKillGiftRate;					// 오크를 잡았을 때 경품을 탈 확률 (1/n)
extern INT			g_iRingDropGiftRate;					// 반지를 던졌을 때 경품을 탈 확률 (1/n)
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
extern BOOL			g_bEventManagerOn;						// 이벤트 관리자 가동 여부
#endif


// 칼리마 맵
#ifdef ADD_NEW_MAP_KALIMA_20040518
extern INT			g_iKundunMarkDropRate;					// 쿤둔의 표식 드롭확률
#endif

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214						// 성주의 표식 드롭확률
extern INT			g_iMarkOfTheLord;
#endif


#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531
extern int			g_iJapan1StAnivItemDropRate;			// 일본 1주년 기념 아이템 드롭확률
#endif
	
#ifdef DARK_LORD_HEART_EVENT_20040810						// 다크로드의 마음 관련 확률변수들
extern INT			g_iDarkLordHeartDropRate;				// 다크로드의 마음 드롭확률
extern INT			g_iDarkLordHeartOffEventRate;			// 다크로드의 마음 오프라인 경품당첨 확률
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
extern INT			g_iMysteriousBeadDropRate1;				// 신비의 구슬 드롭 확률
extern INT			g_iMysteriousBeadDropRate2;				// 신비의 구슬 드롭 확률
extern INT			g_iHiddenTreasureBoxOfflineRate;		// 숨겨진 보물상자 이벤트 오프라인 경품확률
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029							// 행운의 동전 드롭 확률
extern int				g_bLuckyCoinEventOn;			// 행운의 동전 이벤트 여부
extern int				g_iLuckyCoinDropRate;			// 행운의 동전 드롭 확률
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
extern int				g_bEventOff;						// 신섭 이벤트 비활성화
#endif // ADD_NEWSERVER_EVENTOFF_20081114

extern int	gZenDurationTime;


extern char gDevilSquareEventServerIp[20];
extern char gEventChipServerIp[20];

extern char gHackLogServerIp[20];

extern DWORD		gAttackSpeedTimeLimit;
extern int			bIsIgnorePacketSpeedHackDetect;
extern DWORD		gHackCheckCount;
extern float		gDecTimePerAttackSpeed;
extern int			gMinimumAttackSpeedTime;			// 최소 공격 시간
extern int			gDetectedHackKickCount;				// 스피드핵 감지시 퇴출시킬 카운트
extern int			gIsKickDetecHackCountLimit;			// 스피드핵 감지시 퇴출시키는가?

extern int			gQuestNPCTeleportTime;


extern int			gEnableBattleSoccer;		// 배틀사커를 할수 있는가?
extern int			gEnableEventNPCTalk;


#ifdef FOR_JAPAN
extern BOOL			gTamaJJangEvent;			// 타마짱 이벤트를 진행하는가?
extern BOOL			gAppearTamaJJang;			// 타마짱이 등장하는가?
extern int			gTamaJJangKeepTime;			// 타마짱 지속시간

extern int			gTamaJJangDisappearTime;			// 타마짱 사라지는 시간
extern int			gTamaJJangDisappearTimeRandomRange;	// 타마짱이 사라지는 시간을 랜덤으로 설정하기 위한 영역

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

extern int			gEledoradoGoldGoblenItemDropRate;		// 아이템 드랍 확률
extern int			gEledoradoTitanItemDropRate;
extern int			gEledoradoGoldDerconItemDropRate;
extern int			gEledoradoDevilLizardKingItemDropRate;
extern int			gEledoradoDevilTantarosItemDropRate;

extern int			gEledoradoGoldGoblenExItemDropRate;		// 엑설런트 아이템 드랍확률
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
extern char			szAuthKey[20];									// 게임 서버 인증을 위한 검사키 (코드 곳곳에서 체크 한다)
#endif

#ifdef HAPPY_POUCH
// apple★20050204
extern INT			gHappyPouchRedDropRate;
extern INT			gHappyPouchBlueDropRate;
#endif

#ifdef PERSONAL_SHOP_20040113	// 개인상점 개설 여부
extern int			gDoPShopOpen;
#endif

#ifdef CHECK_PENETRATION_SKILL
extern int			gEnableCheckPenetrationSkill;
#endif

#ifdef MU_CHN_OFFLINE_GIFT_EVENT_20040908
extern BOOL			gbChnOfflineGiftEvent;							// 중국 월병 이벤트를 하는가 ?
extern int			giChnOfflineGiftEventDropRate;					// 중국 월병 드롭 확률 (1/1000000)
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
extern int			giKundunRefillHPSec;							// 쿤둔 피 회복량 초당
extern int			giKundunRefillHP;								// 쿤둔 피 회복량 ( 1 / 25 이하)
extern int			giKundunRefillHPTime;							// 피 회복 시간 (SEC)
#endif

#ifdef EXTEND_LOG_SYSTEM_20060202
extern int			giKundunHPLogSaveTime;
#endif

#ifdef DARKLORD_WORK
extern BOOL			gIsDropDarkLordItem;
extern int			gSleeveOfLordDropRate;		// 군주의 소매 드롭 확률
extern int			gSleeveOfLordDropLevel;		// 군주의 소매 드롭 레벨

extern int			gSoulOfDarkHorseDropRate;	// 다크호스의 영혼 드롭 확률
extern int			gSoulOfDarkHorseropLevel;	// 다크호스의 영혼 드롭 레벨

extern int			gSoulOfDarkSpiritDropRate;	// 다크스피릿 드롭 확률
extern int			gSoulOfDarkSpiritDropLevel;	// 다크스피릿 드롭 레벨
extern float		gDarkSpiritAddExperience;
#endif

#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
extern int			gIsDropGemOfDefend;
extern int			gGemOfDefendDropRate;
extern int			gGemOfDefendDropLevel;
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314
extern int			gIsDropSetItemInCastleHuntZone;		// 시련의 땅에서 세트아이템 드랍여부
extern int			gSetItemInCastleHuntZoneDropRate;	// 시련의 땅에서 세트아이템 드랍확률 (10000 기준)
extern int			gSetItemInCastleHuntZoneDropLevel;	// 시련의 땅에서 세트아이템 드랍 몬스터 레벨
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// 중국추가> . 피로도 시스템 적용여부, 경험치, 아이템확률 읽음
extern BOOL			g_bChnPlayTimeLimitOn;				// 피로도 시스템 적용여부
extern INT			g_iChnPlayTimeLimitLevel1Exp;		// 피로시간 경험치 획득률
extern INT			g_iChnPlayTimeLimitLevel1ItemDrop;	// 피로시간 아이템 드롭률
extern INT			g_iChnPlayTimeLimitLevel2Exp;		// 유해시간 경험치 획득률
extern INT			g_iChnPlayTimeLimitLevel2ItemDrop;	// 유해시간 아이템 드롭률
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// 베트남 : 옵션
extern BOOL			g_bVtmPlayTimeLimitOn;				// 피로도 시스템 적용여부
extern INT			g_iVtmPlayTimeLimitNoticeTime;		// 기본 공지 시간 (누적타임 리셋까지 남은 시간 공지)

extern INT			g_iVtmPlayTimeLimitLevel1Time;		// 피로시간 체크시간
extern INT			g_iVtmPlayTimeLimitLevel1NoticeTime;// 피로시간 공지시간
extern INT			g_iVtmPlayTimeLimitLevel1Exp;		// 피로시간 경험치 획득률
extern INT			g_iVtmPlayTimeLimitLevel1ItemDrop;	// 피로시간 아이템 드롭률

extern INT			g_iVtmPlayTimeLimitLevel2Time;		// 유해시간 체크시간
extern INT			g_iVtmPlayTimeLimitLevel2NoticeTime;// 유해시간 공지시간
extern INT			g_iVtmPlayTimeLimitLevel2Exp;		// 유해시간 경험치 획득률
extern INT			g_iVtmPlayTimeLimitLevel2ItemDrop;	// 유해시간 아이템 드롭률
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
extern int			g_iSkillDistanceCheck;		// 스킬 공격 거리 체크	0 : 체크 안함, 1 : 체크 함
extern int			g_iSkillDistanceCheckTemp;	// 스킬 공격 거리 체크 시 여유 값 : defalut 2로 설정
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
	extern	int		g_iSkillDistanceKick;		// 스킬 거리 핵 사용자 강제 종료 여부 0:강제종료 안함, 1:강제종료
	extern	int		g_iSkillDistanceKickCount;	// 스킬 거리 핵 강제 종료 카운트.
#endif
	#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421
		extern	int		g_iSkillDiatanceKickCheckTime;	// 스킬 거리 핵 사용자 유효 카운트 시간 
	#endif
#endif

// CODEREVIEW_SHIELD3 ; 쉴드포션 드롭 관련 스크립트
#ifdef ADD_SHIELD_POINT_01_20060403
extern BOOL	g_CompoundPotionDropOn;				// 복합 물약 드롭 On(1)/Off(0)
extern int	g_iCompoundPotionLv1DropRate;		// 작은 복합 물약 드롭 확률 / 10000
extern int	g_iCompoundPotionLv2DropRate;		// 중간 복합 물약 드롭 확률 / 10000
extern int	g_iCompoundPotionLv3DropRate;		// 큰 복합 물약 드롭 확률 / 10000
extern int	g_iCompoundPotionLv1DropLevel;		// 작은 복합 물약 드롭 레벨
extern int	g_iCompoundPotionLv2DropLevel;		// 중간 복합 물약 드롭 레벨
extern int	g_iCompoundPotionLv3DropLevel;		// 큰 복합 물약 드롭 레벨
	#ifdef UPDATE_SHIELD_OPTION_20060427
		extern BOOL	g_bShieldComboMissOptionOn;			// PvP 공격 미스 시 콤보 취소 옵션 1 : 콤보 취소, 0 : 콤보 유지
		extern int	g_iShieldPotionLv1MixSuccessRate;	// 작은 SD 물약 조합 확률
		extern int  g_iShieldPotionLv1MixMoney;			// 작은 SD 물약 조합 필요 젠
		extern int	g_iShieldPotionLv2MixSuccessRate;	// 중간 SD 물약 조합 확률
		extern int  g_iShieldPotionLv2MixMoney;			// 중간 SD 물약 조합 필요 젠
		extern int	g_iShieldPotionLv3MixSuccessRate;	// 큰 SD 물약 조합 확률
		extern int  g_iShieldPotionLv3MixMoney;			// 큰 SD 물약 조합 필요 젠
	#endif // UPDATE_SHIELD_OPTION_20060427
		
	#ifdef UPDATE_SHIELD_CALCULATE_GAGE_20060428
		extern int	g_iShieldGageConstA;				// 쉴드 게이지 공식 상수값 1
		extern int	g_iShieldGageConstB;				// 쉴드 게이지 공식 상수값 2
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
extern	BOOL	g_bIllusionTampleEvent;				// 이벤트 진행 여부
extern	int		g_iOldScrollDropRate;				// 피의 두루마리 나올 확률
extern	int		g_iCovenantOfIllusionDropRate;		// 환영교단의 서약 나올 확률
extern	BOOL	g_bUseMaxLevelIllusionTemple;		// 400 레벨 전용 환영 사원 사용 여부
extern	BOOL	g_bSaveIllusionTempleRankingPoint;	// 환영 사원의 랭킹 점수 저장여부

#ifdef UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710	// 환영사원 스크립트추가
extern int		g_iIllusionTempleRewardPercent;		// 환영사원 보상 경험치 비율
extern int		g_btIllusionTempleMinUseSkillCnt;	// 환영사원 최소사용스킬 
#endif	// UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710
#endif

#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
extern int		g_iIllusionTempleRewardPercentMaster;	// 환영사원 보상 경험치 비율(마스터레벨)
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
extern	int		gML_OldScrollDropRate;				// 6 낡은 두루마리 나올 확율
extern	int		gML_CovenantOfIllusionDropRate;		// 6 환영교단의 서약 나올 확율
extern	int		gML_AngelKingsPaperDropRate;		// 8 대천사의 서 나올 확율xbr
extern	int		gML_BloodBoneDropRate;				// 8 블러드본 나올 확율
extern	int		gML_EyesOfDevilSquareDropRate;		// 7 악마의 눈 나올 확율
extern	int		gML_KeyOfDevilSquareDropRate;		// 7 악마의 열쇠 나올 확율
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123

#ifdef UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220
extern	int		g_ShadowPahtomMaxLevel;				// 쉐도우팬텀 공방 최대 레벨
#endif	// UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410
extern	int		g_bChangeCharacterName;				// 캐릭명 변경 여부
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

#ifdef GAMESERVER_INFO_EXTENDED_20040625	// 게임서버에서 접속서버로 보내는 추가적인 정보
extern void GameServerExtInfoSend();
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
extern int GetWarehouseUsedHowMuch( int UserLevel,int UserMasterLevel, int IsLock );
#else
extern int GetWarehouseUsedHowMuch(int UserLevel, int IsLock);
#endif

extern void GameServerInfoSendStop();
extern void GameServerInfoSendStart();

#ifdef AUTH_GAMESERVER		// 4byte로 된 이벤트 정보를 얻는다
extern int	GetEventFlag();
#endif

#ifdef ETC_OPTION_RELOAD_20040308		// 뮤의 옵션 중 이벤트 외의 것들만 리로드
extern void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType);
extern void ReadEventInfo(MU_EVENT_TYPE eEventType);
#endif

#ifdef ADD_NEWSERVER_EVENTOFF_20081114
extern void RenderCommonServerInfo();
#endif // ADD_NEWSERVER_EVENTOFF_20081114


#endif
