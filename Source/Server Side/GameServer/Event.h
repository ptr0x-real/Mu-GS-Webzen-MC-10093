

#ifndef __EVENT_H__
#define __EVENT_H__


extern void LuckyBoxOpenEven(LPOBJECTSTRUCT lpObj);
extern BOOL AttackEvent53BagOpen(LPOBJECTSTRUCT lpObj);
extern BOOL AttackEvent55BagOpen(LPOBJECTSTRUCT lpObj);

extern void StarOfXMasOpenEven(LPOBJECTSTRUCT lpObj);
extern void FireCrackerOpenEven(LPOBJECTSTRUCT lpObj);
extern void HeartOfLoveOpenEven(LPOBJECTSTRUCT lpObj);

#ifdef DARK_LORD_HEART_EVENT_20040810				// 다크로드의 마음 아이템 드롭 처리
extern void DarkLordHeartItemBoxOpen(LPOBJECTSTRUCT lpObj);
#endif

#ifdef MODIFY_KUNDUN_REWARD_01_20040915				//  쿤둔이벤트 아이템 드롭 처리
//MODIFY_ITEM_DROP_MAP_OF_EROHIM_KUNDUN_20050610
extern void KundunEventItemBoxOpen(LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY);
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303	// 성사냥터 보스몹 아이템 드롭 처리
//MODIFY_ITEM_DROP_MAP_OF_EROHIM_KUNDUN_20050610
extern void CastleHuntZoneBossRewardOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
#endif

extern void GoldMedalOpenEven(LPOBJECTSTRUCT lpObj);
extern void SilverMedalOpenEven(LPOBJECTSTRUCT lpObj);

extern void EventChipOpenEven(LPOBJECTSTRUCT lpObj);
extern void EledoradoBoxOpenEven(LPOBJECTSTRUCT lpObj, int boxtype, int addlevel, int money);

#ifdef RING_EVENT_ITEMBOX_20031124
extern void RingEventItemBoxOpen(LPOBJECTSTRUCT lpObj);
#endif

#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211
extern void FriendShipItemBoxOpen(LPOBJECTSTRUCT lpObj);
#endif

#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531
extern void Japan1StAnivBoxOpen(LPOBJECTSTRUCT lpObj, int iBoxLevel);
#endif

#ifdef HAPPY_POUCH //happycat@20050201
extern void DropHappyItemOfRedPouch(LPOBJECTSTRUCT lpObj);
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
extern void HiddenTreasureBoxItemBoxOpen(LPOBJECTSTRUCT lpObj);
#endif

#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
extern void RedRibbonBoxOpen( LPOBJECTSTRUCT lpObj );
extern void GreenRibbonBoxOpen( LPOBJECTSTRUCT lpObj );
extern void BlueRibbonBoxOpen( LPOBJECTSTRUCT lpObj );
#endif

#ifdef VALENTINES_DAY_BOX_EVENT_20060124
extern void PinkChocolateBoxOpen( LPOBJECTSTRUCT lpObj );
extern void RedChocolateBoxOpen( LPOBJECTSTRUCT lpObj );
extern void BlueChocolateBoxOpen( LPOBJECTSTRUCT lpObj );
#endif

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
extern void LightPurpleCandyBoxOpen( LPOBJECTSTRUCT lpObj );
extern void VermilionCandyBoxOpen( LPOBJECTSTRUCT lpObj );
extern void DeepBlueCandyBoxOpen( LPOBJECTSTRUCT lpObj );
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
extern void CrywolfDarkElfItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
extern void CrywolfBossMonsterItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
#endif

#ifdef KANTURU_MONSTER_ITEMDROP_20060807
extern void KanturuMayaHandItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
extern void KanturuNightmareItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
#endif

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
extern void HallowinDayEventItemBoxOpen(LPOBJECTSTRUCT lpObj);
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef CHRISTMAS_STAR_20061211
extern void ChristmasStarDrop( LPOBJECTSTRUCT lpObj );
#endif
//------------->
//20081013 해외 크리스마스 이벤트 - 폭죽 아이템(grooving)
//UPDATE_XMASEVENT_MAP_20080930
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
extern void ChristmasFireCrackerDrop( LPOBJECTSTRUCT lpObj );
#endif
//<-------------

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
extern void CherryBlossomEventItemBoxOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
extern BOOL CherryBlossomEventItemBoxAOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, int* pMoney );
extern BOOL CherryBlossomEventItemBoxBOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, int* pMoney );
extern BOOL CherryBlossomEventItemBoxCOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, int* pMoney );
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef PCBANG_POINT_SYSTEM_20070206						// 혼돈 상자 아이템 드랍
extern void	BoxOfGreenChaosItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
extern void	BoxOfRedChaosItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
extern void	BoxOfPurpleChaosItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
#endif

#ifdef CHUSEOK_MONSTER_EVENT_20070823
extern void ChuseokMonsterEventItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
#endif

#ifdef ADD_RAKLION_20080408
void RaklionSelupanItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
#endif // ADD_RAKLION_20080408

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
BOOL LuckyCoinEventItemBag10Open( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY);
BOOL LuckyCoinEventItemBag20Open( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY);
BOOL LuckyCoinEventItemBag30Open( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY);
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_GAMBLING_20090120
BYTE GamblingSystemItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, BYTE btItemType );
#endif // ADD_GAMBLING_20090120

#ifdef ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610	// 여름 불꽃 유령 이벤트
extern void	FireCrackerMonsterEventItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
#endif // ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

#include "Include\cb_protocol.h"


#ifdef BLOODCASTLE_CHECK_ENTERCOUNT_20040304
// 블러드캐슬 하루 입장횟수 제한을 처리한다.
extern void	EGBloodCastleEnterCountCheck(LPPMSG_ANS_BLOODCASTLEENTERCHECK lpMsg);
extern void	EGBloodCastleEnterCountOn(INT iIndex);
#endif

#ifdef DEVILSQUARE_CHECK_ENTERCOUNT_20040715
// 데빌스퀘어 하루 입장횟수 제한을 처리한다.

// [0x0F] 데빌스퀘어의 남은 하루 입장횟수를 요청한다.
extern BOOL EGReqDevilSquareEnterCount		(INT iIndex);
// [0x0F] 데빌스퀘어의 남은 하루 입장횟수를 받는다.
extern VOID EGAnsDevilSquareEnterCount		(LPPMSG_ANS_DEVILSQUARE_ENTERCOUNT lpMsg);

// [0x10] 데빌스퀘어에 입장이 가능한지에 대해서 요청한다.
extern BOOL EGReqDevilSquareEnterCountCheck	(INT iIndex, INT iItemPos, INT iItemLevel, INT iSquareNumber);
// [0x10] 데빌스퀘어의 남은 하루 입장횟수 요청에 대한 결과를 받는다.
extern VOID EGAnsDevilSquareEnterCountCheck	(LPPMSG_ANS_DEVILSQUAREENTERCHECK lpMsg);

// [0x11] 데빌스퀘어의 입장횟수를 올려달라고 요청한다.
extern BOOL EGReqBloodCastleEnterCountOn	(INT iIndex);
#endif


#ifdef BLOODCASTLE_EVENT_3RD_20040401
// [0x0B] 블러드 캐슬의 남은 하루 입장횟수를 요청한다.
extern void EGReqBloodCastleEnterCount(INT iIndex);
// [0x0B] 블러드 캐슬의 남은 하루 입장횟수를 받는다.
extern void EGAnsBloodCastleEnterCount(LPPMSG_ANS_BLOODCASTLE_ENTERCOUNT lpMsg);
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408			// 카오스 캐슬 오프라인 경품과 관련된 함수들
// [0x15] 카오스 캐슬 오프라인 경품 등록을 요청한다.
extern VOID EGReqRegCCOfflineGift (INT iIndex);
// [0x15] 카오스 캐슬 오프라인 경품 등록 요청에 대한 응답을 받는다.
extern VOID EGAnsRegCCOfflineGift (LPPMSG_ANS_REG_CC_OFFLINE_GIFT lpMsg);
#endif

#ifdef DARK_LORD_HEART_EVENT_20040810
// [0x16] 다크로드의 마음 오프라인 경품 등록을 요청한다.
extern VOID EGReqRegDLOfflineGift (INT iIndex);
// [0x16] 다크로드의 마음 오프라인 경품 등록 요청에 대한 응답을 받는다.
extern VOID EGAnsRegDLOfflineGift (LPPMSG_ANS_REG_DL_OFFLINE_GIFT lpMsg);
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// 숨겨진 보물상자 이벤트 경품과 관련된 함수들
// [0x17] 숨겨진 보물상자 경품 등록을 요청한다.
extern VOID EGReqRegHTOfflineGift (INT iIndex);
// [0x17] 숨겨진 보물상자 경품 등록 요청에 대한 응답을 받는다.
extern VOID EGAnsRegHTOfflineGift (LPPMSG_ANS_REG_HT_OFFLINE_GIFT lpMsg);
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
// [0x18] PC방 쿠폰 아이템의 지급에 대한 응답을 받는다.
void EGReqPossiblePCBangCouponEvent(INT iIndex);
void EGAnsPossiblePCBangCouponEvent(LPPMSG_ANS_POSSIBLE_PCBANG_COUPON lpMsg);
void EGReqUsePCBangCoupon(INT iIndex);
void EGAnsUsePCBangCoupon(LPPMSG_ANS_USE_PCBANG_COUPON lpMsg);
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// EventServer와의 처리 함수 extern 선언
// [0x20] 아이템 지급 대상인지 확인
extern void GEReqCheckWhiteAngelGetItem( INT iIndex );
// [0x21] 아이템 지급 대상인지 확인에 대한 응답
extern void EGAnsCheckWhiteAngelGetItem( LPPMSG_ANS_CHECK_WHITEANGEL_GET_ITEM lpMsg );
// [0x22] 아이템 지급 요청
extern void GEReqWhiteAngelGetItem( INT iIndex );
// [0x23] 아이템 지급 요청에 대한 응답
extern void EGAnsWhiteAngelGetItem( LPPMSG_ANS_WHITEANGEL_GET_ITEM lpMsg );
#endif	// WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// EventServer와의 처리 함수 선언
// [0x27] 아이템 지급이 가능한지 확인
extern void GEReqCheckAlansTarget( INT iIndex );
// [0x27] 아이템 지급 가능/불가능에 대한 응답
extern void EGAnsCheckAlansTarget( LPPMSG_ANS_CHECK_ALANS_TARGET lpMsg );
// [0x28] 아이템 지급 요청
extern void GEReqAlansItem( INT iIndex );
// [0x28] 아이템 지급 요청에 대한 응답
extern void EGAnsAlansItem( LPPMSG_ANS_ALANS_ITEM lpMsg );
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029 
// [0x31] 등록된 동전 개수 요청
extern void GEReqGetCointCount(INT iIndex);
// [0x31] 등록된 동전 개수 응답
extern void EGAnsGetCoinCount(LPPMSG__ES_ANS_GET_COIN_COUNT lpMsg);
// [0x32] 동전 등록
extern void GEReqRegeistCoin(INT iIndex, int nInventoryPos);
// [0x32] 동전 등록 응답
extern void EGAnsRegeistCoin(LPPMSG_ES_ANS_REG_COIN lpMsg);
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029 

#ifdef ADD_PAYITEM_20081030 // by natinda
// [DBTASK_REQ_GET_PAYITEM] 아이템 지급 대상인지 확인 요청/응답
extern void GEReqCheckEventNPCTarget(INT iIndex, short nEventID);
extern void EGAnsCheckEventNPCTarget( LPPMSG_ANS_GET_PAYITEM lpMsg );
// [DBTASK_ANS_SET_PAYITEM] 아이템 지급 요청/응답
extern void GEReqEventNPCItem( INT iIndex, short nEventID );
extern void EGAnsEventNPCItem( LPPMSG_ANS_SET_PAYITEM lpMsg );
#endif //ADD_PAYITEM_20081030

#ifdef ADD_NPC_XMASEVENT_20080930
extern void CGReqCheckSnowManNPC(INT iIndex); // 눈사람에게 말을 건다. by natinda 2008-11-05
extern void CGReqSnowManMoveGate(INT iINdex); // 이동에 요청에 대한 처리 
#endif

#ifdef PCBANG_REVITALIZATION_EVENT_20060830
extern void PCBangRevitalizationEventItemDrop( LPOBJECTSTRUCT lpObj );
#endif

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119	// 용사, 전사(우정의 돌)의 반지 추가/수정(goni)
extern void RingOfHeroEventItemBoxOpen(LPOBJECTSTRUCT lpObj);
#endif

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	// 설날복주머니 아이템 드럽
extern void NewYearLuckyBagItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY );
#endif

#ifdef UPDATE_GM_FUNCTION_20070228
extern void GMPresentBoxItemBagOpen( LPOBJECTSTRUCT lpObj);
#endif

extern void EventChipEventProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen);
extern void	DataSendEventChip(char* pMsg, int size);


extern LPOBJECTSTRUCT	pEventObj;

#if defined ( FOR_KOREA ) || defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )
	#ifdef ADD_ITEM_OF_FRIENDSHIP_EVENT_20040729
		#ifdef FOR_KOREA
			#define MAX_ITEM_OF_FRIENDSHIP_EVENT			17	// 한국의 친구이벤트 아이템 최대 개수
			extern const CHAR g_sz2ANV_GIFT_NAME[MAX_ITEM_OF_FRIENDSHIP_EVENT][64];
		#else // else FOR_KOREA
			#define MAX_ITEM_OF_FRIENDSHIP_EVENT			44	// 일본의 친구이벤트 아이템 최대 개수		
			extern CHAR g_sz2ANV_GIFT_NAME[MAX_ITEM_OF_FRIENDSHIP_EVENT][64];
		#endif // FOR_KOREA
	#else // else ADD_ITEM_OF_FRIENDSHIP_EVENT_20040729
		#define MAX_ITEM_OF_FRIENDSHIP_EVENT			32	// 일본의 친구이벤트 아이템 최대 개수
		extern CHAR g_sz2ANV_GIFT_NAME[MAX_ITEM_OF_FRIENDSHIP_EVENT][64];
	#endif // ADD_ITEM_OF_FRIENDSHIP_EVENT_20040729

	extern BOOL	g_bRingEventItemTextLoad;
#endif // FOR_KOREA || EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO
	
#define  MAX_ITEM_OF_COUPON_EVENT						175//일본 행운 번호 (EventItemList.txt)

#endif

