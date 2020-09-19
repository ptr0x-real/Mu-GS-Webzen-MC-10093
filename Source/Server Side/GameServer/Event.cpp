#include "StdAfx.h"
#include "Include\Public.h"
#include "Include\ProDef.h"
#include "Include\SproDef.h"
#include "Common\Winutil.h"
#include "User.h"
#include "ItemBag.h"
#include "Protocol.h"
#include "Gamemain.h"
#include "DSProtocol.h"
#include "gObjMonster.h"

#include "GameServer.h"
#include "Event.h"

#include "Include\cb_protocol.h"
#include "wsJoinServerCli.h"
#include "MasterLevelSystem.h"

#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531		// �Ϻ� �� ���� 1�ֳ� ��� ����ǻ��� (����� ���� +1 ~ +5 �� ����)
#include "time.h"
#endif

#ifdef EVENT_ITEMBAG_UPGRADE
#include "ItemBagEx.h"
#endif

#ifdef UPDATE_EVENT_ITEM_BAG_EXTEND_20070131
#include "ProbabilityItemBag.h"
#endif

#ifdef BLOODCASTLE_EVENT_5TH_20050531
#include "PartyClass.h"
extern PartyClass	gParty;
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
extern void ItemIsBufExOption(LPBYTE buf, CItem * lpItem);
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206		// ���� ����
#include "PCBangPointSystem.h"
#endif

#ifdef ADD_XMASEVENT_LUCKNUMBER
#include "Gamemain.h"
#endif // ADD_XMASEVENT_LUCKNUMBER

#ifdef ADD_GAMBLING_20090120
#include "GamblingSystem.h"
#endif // ADD_GAMBLING_20090120

#if defined ( FOR_KOREA ) || defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )
	#ifdef FOR_KOREA
		// �� ���ȭ 2�ֳ� ��� ���� �̸� - ������ ������� ��ȣ 1~16
		const CHAR g_sz2ANV_GIFT_NAME [MAX_ITEM_OF_FRIENDSHIP_EVENT][64] = {
					"�Ϻ� ���Ϲ��� ��Ʃ���",
					"��Ƽ�� 4 PC",
					"�� �ǱԾ�",
					"�� �׷��� ī��(FX 5200)",
					"�� ���콺",
					"����Ż ī�޶�",
					"�� ���θ� ���̹��Ӵ�(3����)",
					"��ȭ ���ű�(2��)",
					"�ູ�� ����",
					"��ȥ�� ����",
					"ȥ���� ����",
					"50����",
					"10����",
					"5����",
					"�Ե����� ���� �̿��",
					"�Ե����� ���� �̿��(2��)",
					"100�� �� �ݱ�",
					};
	#else // else FOR_KOREA
		CHAR	g_sz2ANV_GIFT_NAME [MAX_ITEM_OF_FRIENDSHIP_EVENT][64] = { 0, };
	#endif // FOR_KOREA

	BOOL g_bRingEventItemTextLoad = FALSE;

#ifndef MODIFY_SEASON_01_20060317
	extern ITEM_ATTRIBUTE   ItemAttribute[];
#endif

#endif // FOR_KOREA || EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO
	
#ifdef MODIFY_SEASON_01_20060317
	extern ITEM_ATTRIBUTE	ItemAttribute[];
#endif

extern wsJoinServerCli wsEvenChipServerCli;	//  �̺�Ʈ Ĩ ����

LPOBJECTSTRUCT	pEventObj = NULL;

// 0x01 �̺�Ʈ ������ ���� �̺�ƮĨ ������ �޴´�
void	EGRecvEventChipInfo(LPPMSG_ANS_VIEW_EC_MN aRecv);
// 0x02 �̺�Ʈ ������ ���� �̺�ƮĨ�� ����� ��� ���� �޴´�
void	EGResultRegEventChip(LPPMSG_ANS_REGISTER_EVENTCHIP aRecv);
// 0x03 �̺�Ʈ ������ ���� �����ȣ ��Ͽ� ���� ��� ���� �޴´�
void	EGRecvRegMutoNum(LPPMSG_ANS_REGISTER_MUTONUM aRecv);
// 0x04 �̺�Ʈ ������ ���� ������ ���ذͿ� ���� ����� �޴´�
void	EGRecvChangeRena(LPPMSG_ANS_RESET_EVENTCHIP aRecv);

#ifdef FOR_BLOODCASTLE
// 0x05 �̺�Ʈ ������ ���� ���� ������ �޴´�. (���� ĳ��)
void	EGRecvStoneInfo(LPPMSG_ANS_VIEW_STONES aRecv);
// 0x06 �̺�Ʈ ������ ���� ������ ����� ����� �޴´�. (���� ĳ��)
void	EGRecvRegStone(LPPMSG_ANS_REGISTER_STONES aRecv);
// 0x07 �̺�Ʈ ������ ���� ������ ������ ����� �޴´�. (���� ĳ��)
void	EGRecvDeleteStone(LPPMSG_ANS_DELETE_STONES aRecv);
#endif

#ifdef FOR_STONE_EXCHANGE
// 0x09 �̺�Ʈ ������ ���� ���渦 ���ذͿ� ���� ����� �޴´�
void	EGRecvChangeStones(LPPMSG_ANS_RESET_EVENTCHIP aRecv);
#endif

#if defined ( FOR_KOREA ) || defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )
// 0x08 �̺�Ʈ ������ ���� �ø����� ����� ����� �޴´�. (�� ���ȭ 2�ֳ� ���)
void	EGRecv2AnvRegSerial(LPPMSG_ANS_2ANIV_SERIAL aRecv);
#endif

#ifdef MONSTER_HERD_SYSTEM_20031120
// 0x08 �̺�Ʈ ������ ���� �����̺�Ʈ ��ǰ��� ���θ� �޴´�.
void	EGRecvRegRingGift(LPPMSG_ANS_REG_RINGGIFT aRecv);
#endif

#ifdef EVENT_STONE_OF_FIENDSHIP_20040206
// 0x11 �̺�Ʈ ������ ���� �����ǵ� ������ �޴´�.			(��ģ�� �̺�Ʈ)
void	EGRecvFriendShipStoneInfo		(LPPMSG_ANS_VIEW_STONES aRecv);
// 0x12 �̺�Ʈ ������ ���� �����ǵ��� ����� ����� �޴´�. (��ģ�� �̺�Ʈ)
void	EGRecvRegFriendShipStone		(LPPMSG_ANS_REGISTER_STONES aRecv);
// 0x13 �̺�Ʈ ������ ���� �����ǵ��� ������ ����� �޴´�. (��ģ�� �̺�Ʈ)
void	EGRecvDeleteFriendShipStone		(LPPMSG_ANS_DELETE_STONES aRecv);
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408			// ī���� ĳ�� �������� ��ǰ�� ���õ� �Լ���
// [0x15] ī���� ĳ�� �������� ��ǰ ����� ��û�Ѵ�.
VOID EGReqRegCCOfflineGift (INT iIndex);
// [0x15] ī���� ĳ�� �������� ��ǰ ��� ��û�� ���� ������ �޴´�.
VOID EGAnsRegCCOfflineGift (LPPMSG_ANS_REG_CC_OFFLINE_GIFT lpMsg);
#endif

#ifdef DARK_LORD_HEART_EVENT_20040810		// ��ũ�ε��� ���� �������� ��ǰ�� ���õ� �Լ���
// [0x16] ��ũ�ε��� ���� �������� ��ǰ ����� ��û�Ѵ�.
VOID EGReqRegDLOfflineGift (INT iIndex);
// [0x16] ��ũ�ε��� ���� �������� ��ǰ ��� ��û�� ���� ������ �޴´�.
VOID EGAnsRegDLOfflineGift (LPPMSG_ANS_REG_DL_OFFLINE_GIFT lpMsg);
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// ������ �������� �̺�Ʈ ��ǰ�� ���õ� �Լ���
// [0x17] ������ �������� ��ǰ ����� ��û�Ѵ�.
VOID EGReqRegHTOfflineGift (INT iIndex);
// [0x17] ������ �������� ��ǰ ��� ��û�� ���� ������ �޴´�.
VOID EGAnsRegHTOfflineGift (LPPMSG_ANS_REG_HT_OFFLINE_GIFT lpMsg);
#endif

#ifdef PCBANG_COUPON_EVENT_20060124	// PC�� ���� �̺�Ʈ
// [0x18] PC�� ������ ���� ���¸� Ȯ���Ѵ�.
void EGReqPossiblePCBangCouponEvent(INT iIndex);
// [0x18] PC�� ������ ���� ���
void EGAnsPossiblePCBangCouponEvent(LPPMSG_ANS_POSSIBLE_PCBANG_COUPON lpMsg);
// [0x19] PC�� ���� �������� ������ ��û�Ѵ�.
void EGReqUsePCBangCoupon(INT iIndex);
// [0x19] PC�� ���� �������� ���� ���
void EGAnsUsePCBangCoupon(LPPMSG_ANS_USE_PCBANG_COUPON lpMsg);
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef BLOODCASTLE_CHECK_ENTERCOUNT_20040304
// ����ĳ�� �Ϸ� ����Ƚ�� ������ ó���Ѵ�.
void	EGBloodCastleEnterCountCheck(LPPMSG_ANS_BLOODCASTLEENTERCHECK lpMsg);
#endif


#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// EventServer���� ó�� �Լ� ����
// [0x20] ������ ���� ������� Ȯ��
void GEReqCheckWhiteAngelGetItem( INT iIndex );
// [0x20] ������ ���� ������� Ȯ�ο� ���� ����
void EGAnsCheckWhiteAngelGetItem( LPPMSG_ANS_CHECK_WHITEANGEL_GET_ITEM lpMsg );
// [0x21] ������ ���� ��û
void GEReqWhiteAngelGetItem( INT iIndex );
// [0x21] ������ ���� ��û�� ���� ����
void EGAnsWhiteAngelGetItem( LPPMSG_ANS_WHITEANGEL_GET_ITEM lpMsg );
#endif	// WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// EventServer���� ó�� �Լ� ����
// [0x27] ������ ������ �������� Ȯ��
void GEReqCheckAlansTarget( INT iIndex );
// [0x27] ������ ���� ����/�Ұ��ɿ� ���� ����
void EGAnsCheckAlansTarget( LPPMSG_ANS_CHECK_ALANS_TARGET lpMsg );
// [0x28] ������ ���� ��û
void GEReqAlansItem( INT iIndex );
// [0x28] ������ ���� ��û�� ���� ����
void EGAnsAlansItem( LPPMSG_ANS_ALANS_ITEM lpMsg );
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029 
// [0x31] ��ϵ� ���� ���� ��û
void GEReqGetCointCount(INT iIndex);
// [0x31] ��ϵ� ���� ���� ����
void EGAnsGetCoinCount(LPPMSG__ES_ANS_GET_COIN_COUNT lpMsg);
// [0x32] ���� ���
void GEReqRegeistCoin(INT iIndex, int nInventoryPos);
// [0x32] ���� ��� ����
void EGAnsRegeistCoin(LPPMSG_ES_ANS_REG_COIN lpMsg);
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029 

#ifdef ADD_PAYITEM_20081030 // by natinda
void GEReqCheckEventNPCTarget(INT iIndex, short nEventID);
void EGAnsCheckEventNPCTarget( LPPMSG_ANS_GET_PAYITEM lpMsg );
void GEReqEventNPCItem( INT iIndex, short nEventID );
void EGAnsEventNPCItem( LPPMSG_ANS_SET_PAYITEM lpMsg );
#endif //ADD_PAYITEM_20081030

#ifdef ADD_NPC_XMASEVENT_20080930
void CGReqCheckSnowManNPC(INT iIndex); // ��������� ���� �Ǵ�. by natinda 2008-11-05
void CGReqSnowManMoveGate(INT iIndex); // �̵� ��û�� ���� ó�� by natinda 2008-11-05
#endif

#ifdef COUPON_EVENT_ITEMLIST_20081022
#include "EventManagement.h"
#endif

void EventChipEventProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen)
{
	switch( protoNum )
	{
		case 0x01 :
			EGRecvEventChipInfo((LPPMSG_ANS_VIEW_EC_MN)aRecv);
		break;

		case 0x02 :
			EGResultRegEventChip((LPPMSG_ANS_REGISTER_EVENTCHIP)aRecv);
		break;

		case 0x03 :
			EGRecvRegMutoNum((LPPMSG_ANS_REGISTER_MUTONUM)aRecv);
		break;

		case 0x04 :
			EGRecvChangeRena((LPPMSG_ANS_RESET_EVENTCHIP)aRecv);
		break;

#ifdef FOR_BLOODCASTLE
		case 0x05 : 
			EGRecvStoneInfo((LPPMSG_ANS_VIEW_STONES)aRecv);
			break;
		case 0x06 : 
			EGRecvRegStone((LPPMSG_ANS_REGISTER_STONES)aRecv);
			break;
		case 0x07 : 
			EGRecvDeleteStone((LPPMSG_ANS_DELETE_STONES)aRecv);
			break;
#endif
			
#ifdef FOR_STONE_EXCHANGE
		case 0x09 :
			EGRecvChangeStones((LPPMSG_ANS_RESET_EVENTCHIP)aRecv);
			break;
#endif

#if defined ( FOR_KOREA ) || defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )
		case 0x08 :
			EGRecv2AnvRegSerial((LPPMSG_ANS_2ANIV_SERIAL)aRecv);
			break;
#endif // FOR_KOREA || EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO

#ifdef MONSTER_HERD_SYSTEM_20031120
		case 0x10 :
			EGRecvRegRingGift((LPPMSG_ANS_REG_RINGGIFT)aRecv);
			break;
#endif

#ifdef EVENT_STONE_OF_FIENDSHIP_20040206
		case 0x11 : 
			EGRecvFriendShipStoneInfo		((LPPMSG_ANS_VIEW_STONES)aRecv);
			break;
		case 0x12 : 
			EGRecvRegFriendShipStone		((LPPMSG_ANS_REGISTER_STONES)aRecv);
			break;
		case 0x13 : 
			EGRecvDeleteFriendShipStone		((LPPMSG_ANS_DELETE_STONES)aRecv);
			break;
#endif // EVENT_STONE_OF_FIENDSHIP_20040206

#ifdef CHAOSCASTLE_SYSTEM_20040408			// ī���� ĳ�� �������� ��ǰ�� ���õ� �Լ���
		case 0x15 :
			EGAnsRegCCOfflineGift			((LPPMSG_ANS_REG_CC_OFFLINE_GIFT)aRecv);
			break;;
#endif

#ifdef DARK_LORD_HEART_EVENT_20040810		// ��ũ�ε��� ���� �������� ��ǰ�� ���õ� �Լ���
		case 0x16 :
			EGAnsRegDLOfflineGift			((LPPMSG_ANS_REG_DL_OFFLINE_GIFT)aRecv);
			break;;
#endif
			
#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// ������ �������� �̺�Ʈ ��ǰ�� ���õ� �Լ���
		case 0x17 :
			EGAnsRegHTOfflineGift			((LPPMSG_ANS_REG_HT_OFFLINE_GIFT)aRecv);
			break;
#endif
#ifdef PCBANG_COUPON_EVENT_20060124
		case 0x18:
			EGAnsPossiblePCBangCouponEvent	((LPPMSG_ANS_POSSIBLE_PCBANG_COUPON)aRecv);
			break;
		case 0x19:
			EGAnsUsePCBangCoupon			((LPPMSG_ANS_USE_PCBANG_COUPON)aRecv);
			break;
#endif // PCBANG_COUPON_EVENT_20060124	
#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
		case 0x20:
			EGAnsCheckWhiteAngelGetItem( ( LPPMSG_ANS_CHECK_WHITEANGEL_GET_ITEM )aRecv );
			break;
		case 0x21:
			EGAnsWhiteAngelGetItem( ( LPPMSG_ANS_WHITEANGEL_GET_ITEM )aRecv );
			break;
#endif			
#ifdef PCBANG_POINT_SYSTEM_20070206	// EventServer�κ��� �����͸� ����
		case 0x23:
			g_PCBangPointSystem.EGRecvPCBangPointInfo( aRecv );
			break;
		case 0x24:
			g_PCBangPointSystem.EGRecvUpdatePCBangPoint( aRecv );
			break;
#endif
#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// EventServer�κ��� �����͸� ����
		case 0x27:
			EGAnsCheckAlansTarget( ( LPPMSG_ANS_CHECK_ALANS_TARGET )aRecv );
			break;
		case 0x28:
			EGAnsAlansItem( ( LPPMSG_ANS_ALANS_ITEM )aRecv );
			break;
#endif
#ifdef PCBANG_KUZIMO	// EventServer�κ��� �����͸� ����
		case 0x29:
			g_PCBangPointSystem.EGRecvPCBangPointInfo2( aRecv );
			break;
		case 0x30:
			g_PCBangPointSystem.EGRecvUpdatePCBangPoint2( aRecv );
			break;
#endif
#ifdef ADD_PAYITEM_20081030 // by natinda
		case DBTASK_REQ_GET_PAYITEM:
			EGAnsCheckEventNPCTarget((LPPMSG_ANS_GET_PAYITEM) aRecv );
			break;
		case DBTASK_ANS_SET_PAYITEM:
			EGAnsEventNPCItem((LPPMSG_ANS_SET_PAYITEM) aRecv );
			break;
#endif
#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
		case 0x33:
			EGAnsGetCoinCount( ( LPPMSG__ES_ANS_GET_COIN_COUNT )aRecv );
			break;
		case 0x34:
			EGAnsRegeistCoin( ( LPPMSG_ES_ANS_REG_COIN )aRecv );
			break;			
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029			
	}
}

void	DataSendEventChip(char* pMsg, int size)
{
	if( !IsEventChipServerConnected && EventChipServerConnect )
	{
		wsEvenChipServerCli.Close();
		wsEvenChipServerCli.CreateSocket(ghWnd);
		if( GMEventChipServerConnect(gEventChipServerIp, WM_ASS_EVENTCHIPCLIMSG) == FALSE )
		{
			IsEventChipServerConnected = 0;
			LogAddTD("Can not connect EventChip Server");
			return;
		}	
		IsEventChipServerConnected = 1;
	}

	if( IsEventChipServerConnected && EventChipServerConnect )
		wsEvenChipServerCli.DataSend(pMsg, size);
}


extern CItemBag	*   LuckboxItemBag;	// ����ǻ��� ������ ����(?)
extern CItemBag	*   Mon55;	// �η��þ� ���� ������ ����(?)
extern CItemBag	*   Mon53;	// ����ƽ� ����  ������ ����(?)

#ifdef EVENT_ITEMBAG_UPGRADE
extern CItemBagEx	*   StarOfXMasItemBag;		// ��ź�� �� ������ ����(?)
#else
extern CItemBag	*   StarOfXMasItemBag;			// ��ź�� �� ������ ����(?)
#endif

extern CItemBag	*   FireCrackerItemBag;			// ����

extern CItemBag	*   HeartOfLoveItemBag;			// ����� ��Ʈ �̺�Ʈ

extern CItemBag	*   GoldMedalItemBag;			// ������
extern CItemBag	*   SilverMedalItemBag;			// ������

extern CItemBag	*   EventChipItemBag;			// �̺�Ʈ Ĩ


#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
extern CProbabilityItemBag	*   GoldGoblenItemBag;			//	Ȳ�ݰ��
extern CProbabilityItemBag	*   TitanItemBag;				//	Ÿ��ź
extern CProbabilityItemBag	*   GoldDerconItemBag;			//  Ȳ�ݵ�����
extern CProbabilityItemBag	*   DevilLizardKingItemBag;		// ���� ���ڵ�ŷ
extern CProbabilityItemBag	*   KanturItemBag;	            // ī����
#else 
extern CItemBag	*   GoldGoblenItemBag;			//	Ȳ�ݰ��
extern CItemBag	*   TitanItemBag;				//	Ÿ��ź
extern CItemBag	*   GoldDerconItemBag;			//  Ȳ�ݵ�����
extern CItemBag	*   DevilLizardKingItemBag;		// ���� ���ڵ�ŷ
extern CItemBag	*   KanturItemBag;	            // ī����
#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04


#ifdef RING_EVENT_ITEMBOX_20031124
extern CItemBag	*   RingEventItemBag;				// ���������� �̺�Ʈ
#endif

#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211
extern CItemBag	*   FriendShipItemBag;				// ��ģ�� �̺�Ʈ - �����ǹ���
#endif

#ifdef DARK_LORD_HEART_EVENT_20040810		
extern CItemBag	*   DarkLordHeartItemBag;			// ��ũ�ε��� ���� �̺�Ʈ
#endif

#ifdef MODIFY_KUNDUN_REWARD_01_20040915
extern CItemBagEx	*	KundunEventItemBag;			// ��� �̺�Ʈ
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303
extern CItemBagEx	*	CastleHuntZoneBossItemBag;	// �� ����� ����(������)
#endif

#ifdef MU_CHN_THANKS_COOKIE_EVENT_20040908 
extern CItemBagEx *	ChnThanksCookieEvent;			// �߱� ���� �̺�Ʈ
#endif


#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
extern CItemBagEx	*	CastleItemMixItemBag;		// �������� Ư�� ������ ����
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
extern CItemBagEx *		HiddenTreasureBoxItemBag;	// ������ �������� ������
#endif

#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212				// ũ�������� �������� �̺�Ʈ
extern CItemBagEx *	RedRibbonBoxEventItemBag;			// ������������ 
extern CItemBagEx *	GreenRibbonBoxEventItemBag;			// �ʷϸ������� 
extern CItemBagEx *	BlueRibbonBoxEventItemBag;			// �Ķ���������
#endif

#ifdef VALENTINES_DAY_BOX_EVENT_20060124				// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ
extern CItemBagEx *	PinkChocolateBoxEventItemBag;		// ��ȫ���ݸ����� 
extern CItemBagEx *	RedChocolateBoxEventItemBag;		// �������ݸ�����
extern CItemBagEx *	BlueChocolateBoxEventItemBag;		// �Ķ����ݸ�����
#endif

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222				// ȭ��Ʈ���� �������� �̺�Ʈ
extern CItemBagEx *	LightPurpleCandyBoxEventItemBag;	// ������������� 
extern CItemBagEx *	VermilionCandyBoxEventItemBag;		// ��ȫ��������
extern CItemBagEx *	DeepBlueCandyBoxEventItemBag;		// ��û��������
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404		// ��ũ���� ������ ��
extern CItemBagEx* CrywolfDarkElfItemBag;
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404			// ũ���̿��� �������� �߰��� ������ ��
extern CItemBagEx* CrywolfBossMonsterItemBag;
#endif

#ifdef KANTURU_MONSTER_ITEMDROP_20060807
extern CItemBagEx*	KanturuMayaHandItemBag;				// ĭ���� ���� �� ������ ��
extern CItemBagEx*	KanturuNightmareItemBag;			// ĭ���� ����Ʈ�޾� ������ ��
#endif

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
extern CItemBagEx*	HallowinDayEventItemBag;			// �ҷ��� ���� �̺�Ʈ ������ ��
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119				// ���, ����(������ ��)�� ���� �߰�/����
extern CItemBag*	RingOfHeroEventItemBag;
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
extern CItemBagEx*	PCBangEventNPCItemBag;				// PC�� �̺�Ʈ - NPC�� �����ϴ� ������ ����� ItemBag���� ��ü
#endif

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131				// �������ָӴ� ������ ����
extern CProbabilityItemBag*	NewYearLuckyBagItemBag;
#endif

#ifdef UPDATE_GM_FUNCTION_20070228							// GM ���� ���� ������ ����
extern CProbabilityItemBag*	GMPresentBoxItemBag;
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206						// ȥ�� ���� ������ ��
extern CProbabilityItemBag*	BoxOfGreenChaosItemBag;		// �ʷ�ȥ�� ����
extern CProbabilityItemBag*	BoxOfRedChaosItemBag;		// ����ȥ�� ����
extern CProbabilityItemBag*	BoxOfPurpleChaosItemBag;	// ����ȥ�� ����
#endif

#ifdef UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408	// ���� 
extern CProbabilityItemBag*	ItemGiveReoEventItemBag;	// ���� �ű� �����۹�
#else	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// ���� �� ������ ���� NPC - ���� �̺�Ʈ�� �߰� Extern����
extern  CItemBagEx*		ItemGiveEventItemBag;
#endif
#endif	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408

#ifdef CHUSEOK_MONSTER_EVENT_20070823	// �߼� ���� �̺�Ʈ ������ �� Extern����
extern CProbabilityItemBag*	ChuseokMonsterEventItemBag;
#endif

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// NPC �ٷ� ������ ���� �̺�Ʈ ������ �� Extern����
extern CProbabilityItemBag*	NpcAlanEventItemBag;	
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
extern CProbabilityItemBag* pCherryBlossomEventItemBag;
extern CProbabilityItemBag*	pCherryBlossomEventItemBagA;
extern CProbabilityItemBag*	pCherryBlossomEventItemBagB;
extern CProbabilityItemBag*	pCherryBlossomEventItemBagC;
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_RAKLION_20080408
extern CProbabilityItemBag* pRaklionSelupanItemBag;
#endif // ADD_RAKLION_20080408


#ifdef ADD_GIFTITEM_XMASEVENT_20081030
extern CProbabilityItemBag* pSantaNpcEventItem;
extern CProbabilityItemBag* pSantaNpcEventItem100;
extern CProbabilityItemBag* pSantaNpcEventItem1000;
#endif //ADD_GIFTITEM_XMASEVENT_20081030

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� ��ȯ ������ ��
extern CProbabilityItemBag* pLuckyCoinEventItemBag10;
extern CProbabilityItemBag*	pLuckyCoinEventItemBag20;
extern CProbabilityItemBag*	pLuckyCoinEventItemBag30;
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_GAMBLING_20090120
extern CGamblingItemBag* pGamblingSystemItemBag;
#endif // ADD_GAMBLING_20090120

#ifdef ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610	// ���� �Ҳ� ���� �̺�Ʈ
extern CProbabilityItemBag*	FireCrackerMonsterEventItemBag;
#endif // ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

// �������� ����(Ȳ�ݰ��)
#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
void EledoradoBoxOpenEven(LPOBJECTSTRUCT lpObj, int boxtype, int addlevel, int money)
{	
	
	CProbabilityItemBag* EledoradoBox = NULL;
	
	
	switch(boxtype)
	{
	case 8:
		{
			EledoradoBox = GoldGoblenItemBag;
		}
		break;
	case 9:
		{
			EledoradoBox = TitanItemBag;
		}
		break;
	case 10:
		{
			
			EledoradoBox = GoldDerconItemBag;
		}
		break;
	case 11:
		{
			EledoradoBox = DevilLizardKingItemBag;
		}
		break;
	case 12:
		{
			EledoradoBox = KanturItemBag;
		}
		break;
	}
	
	if( !EledoradoBox )
		return;	
	
	
	EledoradoBox->DropEventItem(lpObj->m_Index,lpObj->MapNumber,lpObj->X,lpObj->Y);
	//EledoradoBox->DropKundunRenewalItem(lpObj->m_Index);
	
}
#else //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
// �������� ����(Ȳ�ݰ��)
void EledoradoBoxOpenEven(LPOBJECTSTRUCT lpObj, int boxtype, int addlevel, int money)
{	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum = -1;
	int ExOption = 0;

	int ItemDropRate = 0;
	int ExItemDropRate = 0;

	CItemBag* EledoradoBox = NULL;

	switch(boxtype)
	{
		case 8:
			{
				EledoradoBox = GoldGoblenItemBag;
				ItemDropRate = gEledoradoGoldGoblenItemDropRate;
				ExItemDropRate = gEledoradoGoldGoblenExItemDropRate;
			}
		break;
		case 9:
			{
				EledoradoBox = TitanItemBag;
				ItemDropRate = gEledoradoTitanItemDropRate;
				ExItemDropRate = gEledoradoTitanExItemDropRate;
			}
		break;
		case 10:
			{
				EledoradoBox = GoldDerconItemBag;
				ItemDropRate = gEledoradoGoldDerconItemDropRate;
				ExItemDropRate = gEledoradoGoldDerconExItemDropRate;
			}
		break;
		case 11:
			{
				EledoradoBox = DevilLizardKingItemBag;
				ItemDropRate = gEledoradoDevilLizardKingItemDropRate;
				ExItemDropRate = gEledoradoDevilLizardKingExItemDropRate;
			}
		break;
		case 12:
			{
				EledoradoBox = KanturItemBag;
				ItemDropRate = gEledoradoDevilTantarosItemDropRate;
				ExItemDropRate = gEledoradoDevilTantarosExItemDropRate;
			}
		break;
	}
	
	if( !EledoradoBox )
		return;	

	if( EledoradoBox->GetBagCount() > 0 )
	{
		// ������ ��� Ȯ�� ����
		if( (rand()%100) < ItemDropRate )
		{
			int NormalItemCount = EledoradoBox->GetNormalItemCount();
			int ExItemCount = EledoradoBox->GetExItemCount();

			if( ExItemCount > 0 && (rand()%100) < ExItemDropRate )
			{	// ������Ʈ ������
				DropItemNum = (rand()%ExItemCount)+NormalItemCount;
				dur  = 0;
				x    = lpObj->X;
				y    = lpObj->Y;
				
				level= 0;
				
				type = ItemGetNumberMake(EledoradoBox->GetType(DropItemNum), EledoradoBox->GetIndex(DropItemNum));
				
				Option1 = 1;
				Option2 = rand()%2;

				if( !Option2 || !Option1 )
				{	// ������ �ϳ��� 0�̸� 
					if( (rand()%5) < 1 )
					{
						Option3 = 3;
					}
					else Option3 = (rand()%3);
				}
#ifdef NEW_SKILL_FORSKYLAND
				ExOption = NewOptionRand(0);
#endif
				// ȥ��, �༮, ������ ������ ����
				if( (type == MAKE_ITEMNUM(12, 15)) ||
					(type == MAKE_ITEMNUM(14, 13)) ||
					(type == MAKE_ITEMNUM(14, 14)) )				
				{	
					Option1 = 0; 
					Option2 = 0;
					Option3 = 0;
					level   = 0;
					ExOption = 0;
				}
				
				if( (type == MAKE_ITEMNUM(13, 0)) ||
					(type == MAKE_ITEMNUM(13, 1)) ||				
					(type == MAKE_ITEMNUM(13, 2)) ||
					(type == MAKE_ITEMNUM(13, 8)) ||
					(type == MAKE_ITEMNUM(13, 9)) ||
					(type == MAKE_ITEMNUM(13, 12)) ||
					(type == MAKE_ITEMNUM(13, 13)) )
				{	// �������� ���� 0
					level   = 0;
				}			
			}
			else if(NormalItemCount > 0)
			{	// �Ϲ� ������
				DropItemNum = (rand()%NormalItemCount);
				dur  = 0;
				x    = lpObj->X;
				y    = lpObj->Y;
				
				if( addlevel != 0 )
					level= EledoradoBox->GetLevel(DropItemNum)+(rand()%addlevel);
				else
					level= EledoradoBox->GetLevel(DropItemNum);

				
				type = ItemGetNumberMake(EledoradoBox->GetType(DropItemNum), EledoradoBox->GetIndex(DropItemNum));
				
				Option1 = rand()%2;
				Option2 = rand()%2;

				if( !Option2 || !Option1 )
				{	// ������ �ϳ��� 0�̸� 
					if( (rand()%5) < 1 )
					{
						Option3 = 3;
					}
					else Option3 = (rand()%3);
				}

				// ȥ��, �༮, ����, ������ ������ ����
				if( (type == MAKE_ITEMNUM(12, 15)) ||
					(type == MAKE_ITEMNUM(14, 13)) ||
					(type == MAKE_ITEMNUM(14, 14)) || 
					(type == MAKE_ITEMNUM(14, 16)) )				
				{	
					Option1 = 0; 
					Option2 = 0;
					Option3 = 0;
					level   = 0;
					ExOption = 0;
				}
				
				if( (type == MAKE_ITEMNUM(13, 0)) ||
					(type == MAKE_ITEMNUM(13, 1)) ||				
					(type == MAKE_ITEMNUM(13, 2)) ||
					(type == MAKE_ITEMNUM(13, 8)) ||
					(type == MAKE_ITEMNUM(13, 9)) ||
					(type == MAKE_ITEMNUM(13, 12)) ||
					(type == MAKE_ITEMNUM(13, 13)) )
				{	// �������� ���� 0
					level   = 0;
				}			
			}

			if(DropItemNum != -1)
			{
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index, ExOption);

#ifdef EXTEND_LOG_SYSTEM_20060202
				CItem EventItem;
				EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

				LogAddTD("[%s][%s][Lucky Box Item Drop][Eledorado Box] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
				LogAddTD("EleDoradoBox-%d Event ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d ex:%d", boxtype, type, level, Option1, Option2, Option3, ExOption);
#endif
				return;
			}
		}
	}
	x    = lpObj->X;
	y    = lpObj->Y;	
	MapC[lpObj->MapNumber].MoneyItemDrop(money, x ,y);	
}

#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04

// �̺�Ʈ Ĩ
void EventChipOpenEven(LPOBJECTSTRUCT lpObj)
{	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;

	if( (rand()%100) < g_EventChipDropRateForBoxOfGold )
	{
		dur  = 255;
		x    = lpObj->X;
		y    = lpObj->Y;
		level= 0;	// Ȳ���� ����
		type = ItemGetNumberMake(14, 21);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);		
		LogAdd("EventChip Event ItemDrop Rena");
		return;
	}

	if( !LuckboxItemBag )
		return;

	if( LuckboxItemBag->GetBagCount() > 0 )
	{
		// 2/20 �� Ȯ���� �̺�Ʈ�� ������ ����
		if( (rand()%20) < g_ItemDropRateForBoxOfGold )
		{
			DropItemNum = (rand()%LuckboxItemBag->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;			

			// ItemBag������ ������ 4���� �����Ǿ� �ִ� (4 - 5)
			level= LuckboxItemBag->GetLevel(DropItemNum)+(rand()%2);
			
			type = ItemGetNumberMake(LuckboxItemBag->GetType(DropItemNum), LuckboxItemBag->GetIndex(DropItemNum));
			
			Option1 = rand()%2;
			Option2 = rand()%2;

			if( !Option2 || !Option1 )
			{	// ������ �ϳ��� 0�̸� 
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}			

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )				
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
				ExOption = 0;
			}
			
			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}			

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index, ExOption);

			// ���ķ�..
			PMSG_SERVERCMD	ServerCmd;
			PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

			ServerCmd.CmdType = 2;	// ���ķ�
			ServerCmd.X = x;
			ServerCmd.Y = y;
			MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

			LogAddTD("[%s][%s][Lucky Box Item Drop][Event Chip] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAdd("EventChip Event BoxOfGold ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	x    = lpObj->X;
	y    = lpObj->Y;	
	MapC[lpObj->MapNumber].MoneyItemDrop(1000, x ,y);	
}

// �ݸ޴�
void GoldMedalOpenEven(LPOBJECTSTRUCT lpObj)
{	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;

	if( !GoldMedalItemBag )
		return;

	if( GoldMedalItemBag->GetBagCount() > 0 )
	{
		// 2/20 �� Ȯ���� �̺�Ʈ�� ������ ����
		if( (rand()%20) < g_ItemDropRateForGoldMedal )
		{
			DropItemNum = (rand()%GoldMedalItemBag->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;			

			// ItemBag������ ������ 3���� �����Ǿ� �ִ� (3 - 6)
//			level= GoldMedalItemBag->GetLevel(DropItemNum)+(rand()%3);		// 20050901 - �±� �̺�Ʈ ���� (b4nfter)
			level= GoldMedalItemBag->GetLevel(DropItemNum)+(rand()%4);
			
			type = ItemGetNumberMake(GoldMedalItemBag->GetType(DropItemNum), GoldMedalItemBag->GetIndex(DropItemNum));
			
			Option1 = rand()%2;
			Option2 = rand()%2;

			if( !Option2 || !Option1 )
			{	// ������ �ϳ��� 0�̸� 
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}			

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )				
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
				ExOption = 0;
			}
			
			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}			

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index, ExOption);

			// ���ķ�..
			PMSG_SERVERCMD	ServerCmd;
			PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

			ServerCmd.CmdType = 2;	// ���ķ�
			ServerCmd.X = x;
			ServerCmd.Y = y;
			MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

			LogAddTD("[%s][%s][Lucky Box Item Drop][Gold Medal] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAdd("Medal Event GoldMedal ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	x    = lpObj->X;
	y    = lpObj->Y;	
#ifdef MODIFY_MEDAL_EVENT_ZEN_20050829
	MapC[lpObj->MapNumber].MoneyItemDrop(2005, x ,y);	
#else
	MapC[lpObj->MapNumber].MoneyItemDrop(100000, x ,y);	
//	MapC[lpObj->MapNumber].MoneyItemDrop(5100, x ,y);	
#endif
}


// ���޴�
void SilverMedalOpenEven(LPOBJECTSTRUCT lpObj)
{	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;

	if( !SilverMedalItemBag )
		return;

	if( SilverMedalItemBag->GetBagCount() > 0 )
	{
		// 2/20 �� Ȯ���� �̺�Ʈ�� ������ ����
		if( (rand()%20) < g_ItemDropRateForSilverMedal )
		{
			DropItemNum = (rand()%SilverMedalItemBag->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;			

			// ItemBag������ ������ 3���� �����Ǿ� �ִ� (3 - 6)
			level= SilverMedalItemBag->GetLevel(DropItemNum)+(rand()%4);
			
			type = ItemGetNumberMake(SilverMedalItemBag->GetType(DropItemNum), SilverMedalItemBag->GetIndex(DropItemNum));
			
			
			Option1 = rand()%2;
			Option2 = rand()%2;

			if( !Option2 || !Option1 )
			{	// ������ �ϳ��� 0�̸� 
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}			

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )				
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
				ExOption = 0;
			}
			
			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}			

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

			// ���ķ�..
			PMSG_SERVERCMD	ServerCmd;
			PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

			ServerCmd.CmdType = 2;	// ���ķ�
			ServerCmd.X = x;
			ServerCmd.Y = y;
			MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

			LogAddTD("[%s][%s][Lucky Box Item Drop][Silver Medal] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAdd("Medal Event SilverMedal ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	x    = lpObj->X;
	y    = lpObj->Y;	
#ifdef MODIFY_MEDAL_EVENT_ZEN_20050829
	MapC[lpObj->MapNumber].MoneyItemDrop(2005, x ,y);	
#else
	MapC[lpObj->MapNumber].MoneyItemDrop(100000, x ,y);	
//	MapC[lpObj->MapNumber].MoneyItemDrop(5100, x ,y);	
#endif
}

// ����� ��Ʈ ����
void HeartOfLoveOpenEven(LPOBJECTSTRUCT lpObj)
{	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

	if( !HeartOfLoveItemBag )
		return;	


	if( HeartOfLoveItemBag->GetBagCount() > 0 )
	{
		// 2/20 �� Ȯ���� �̺�Ʈ�� ������ ����
		if( (rand()%20) < g_ItemDropRateForgHeartOfLove )
		{
			DropItemNum = (rand()%HeartOfLoveItemBag->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;			

			// Option�� 3-7���� (ItemBag4���� ���� 3���� �����Ǿ� �ִ�
			if( gLanguage == 0 )
			{	// �ѱ��� ���� 5 - 7����
				level= HeartOfLoveItemBag->GetLevel(DropItemNum)+(rand()%2);
			}
			else
			{
				level= HeartOfLoveItemBag->GetLevel(DropItemNum)+(rand()%5);
			}
			
			type = ItemGetNumberMake(HeartOfLoveItemBag->GetType(DropItemNum), HeartOfLoveItemBag->GetIndex(DropItemNum));
			
			Option1 = rand()%2;
			Option2 = rand()%2;

			if( !Option2 || !Option1 )
			{	// ������ �ϳ��� 0�̸� 
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )				
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}
			
			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

			// ����..
			PMSG_SERVERCMD	ServerCmd;
			PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

			ServerCmd.CmdType = 0;
			ServerCmd.X = x;
			ServerCmd.Y = y;
			MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

			LogAddTD("[%s][%s][Lucky Box Item Drop][Heart Of Love] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAdd("FireCracker Event ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	x    = lpObj->X;
	y    = lpObj->Y;	
	MapC[lpObj->MapNumber].MoneyItemDrop(1004, x ,y);	
}

// ���� ���� �̺�Ʈ ����
void FireCrackerOpenEven(LPOBJECTSTRUCT lpObj)
{	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

#ifdef MODIFY_DROP_ITEM_OF_FIRE_CRACKER_EVENT_20050316
	if( gOnlyFireCrackerEffectUse )
	{	// ���� ȿ���� ����Ѵ�
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

		ServerCmd.CmdType = 0;
		ServerCmd.X = (BYTE)lpObj->X;
		ServerCmd.Y = (BYTE)lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		return;
	}

	if( gFireCrackerEvent )
	{
		if( !FireCrackerItemBag )
			return;

		if( FireCrackerItemBag->GetBagCount() > 0 )
		{
			// 2/10 �� Ȯ���� �̺�Ʈ�� ������ ����
			if( (rand()%10) < g_ItemDropRateForgFireCracker )
			{
				DropItemNum = (rand()%FireCrackerItemBag->GetBagCount());
				dur  = 0;
				x    = lpObj->X;
				y    = lpObj->Y;			

				// Option�� 3-7���� (ItemBag4���� ���� 3���� �����Ǿ� �ִ�
				if( gLanguage == 0 )
				{	// �ѱ��� ���� 5 - 7����
					level= FireCrackerItemBag->GetLevel(DropItemNum)+(rand()%2);
				}
				else
				{
					level= FireCrackerItemBag->GetLevel(DropItemNum)+(rand()%5);
				}
				
				type = ItemGetNumberMake(FireCrackerItemBag->GetType(DropItemNum), FireCrackerItemBag->GetIndex(DropItemNum));
				
				Option1 = rand()%2;
				Option2 = rand()%2;

				if( !Option2 || !Option1 )
				{	// ������ �ϳ��� 0�̸� 
					if( (rand()%5) < 1 )
					{
						Option3 = 3;
					}
					else Option3 = (rand()%3);
				}

				// ȥ��, �༮, ������ ������ ����
				if( (type == MAKE_ITEMNUM(12, 15)) ||
					(type == MAKE_ITEMNUM(14, 13)) ||
					(type == MAKE_ITEMNUM(14, 14)) )				
				{	
					Option1 = 0;
					Option2 = 0;
					Option3 = 0;
					level   = 0;
				}
				
				if( (type == MAKE_ITEMNUM(13, 0)) ||
					(type == MAKE_ITEMNUM(13, 1)) ||				
					(type == MAKE_ITEMNUM(13, 2)) ||
					(type == MAKE_ITEMNUM(13, 8)) ||
					(type == MAKE_ITEMNUM(13, 9)) ||
					(type == MAKE_ITEMNUM(13, 12)) ||
					(type == MAKE_ITEMNUM(13, 13)) )
				{	// �������� ���� 0
					level = 0;
				}

				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

				// ����..
				PMSG_SERVERCMD	ServerCmd;
				PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

				ServerCmd.CmdType = 0;
				ServerCmd.X = x;
				ServerCmd.Y = y;
				MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
				DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

#ifdef EXTEND_LOG_SYSTEM_20060202
				CItem EventItem;
				EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

				LogAddTD("[%s][%s][Lucky Box Item Drop][Fire Cracker] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
				LogAdd("FireCracker Event ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
				return;
			}
		}
	}

#else // else MODIFY_DROP_ITEM_OF_FIRE_CRACKER_EVENT_20050316
	if( gOnlyFireCrackerEffectUse )
	{	// ���� ȿ���� ����Ѵ�
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

		ServerCmd.CmdType = 0;
		ServerCmd.X = (BYTE)lpObj->X;
		ServerCmd.Y = (BYTE)lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		return;
	}

	if( !FireCrackerItemBag )
		return;

	if( FireCrackerItemBag->GetBagCount() > 0 )
	{
		// 2/10 �� Ȯ���� �̺�Ʈ�� ������ ����
		if( (rand()%10) < g_ItemDropRateForgFireCracker )
		{
			DropItemNum = (rand()%FireCrackerItemBag->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;			

			// Option�� 3-7���� (ItemBag4���� ���� 3���� �����Ǿ� �ִ�
			if( gLanguage == 0 )
			{	// �ѱ��� ���� 5 - 7����
				level= FireCrackerItemBag->GetLevel(DropItemNum)+(rand()%2);
			}
			else
			{
				level= FireCrackerItemBag->GetLevel(DropItemNum)+(rand()%5);
			}
			
			type = ItemGetNumberMake(FireCrackerItemBag->GetType(DropItemNum), FireCrackerItemBag->GetIndex(DropItemNum));
			
			Option1 = rand()%2;
			Option2 = rand()%2;

			if( !Option2 || !Option1 )
			{	// ������ �ϳ��� 0�̸� 
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )				
			{	
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}
			
			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

			// ����..
			PMSG_SERVERCMD	ServerCmd;
			PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

			ServerCmd.CmdType = 0;
			ServerCmd.X = x;
			ServerCmd.Y = y;
			MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

#ifdef EXTEND_LOG_SYSTEM_20060202
				CItem EventItem;
				EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

				LogAddTD("[%s][%s][Lucky Box Item Drop][Fire Cracker] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
				LogAdd("FireCracker Event ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	
#endif // MODIFY_DROP_ITEM_OF_FIRE_CRACKER_EVENT_20050316

	x    = lpObj->X;
	y    = lpObj->Y;	

#ifdef FOR_THAILAND
	#ifdef EVENT_SONGKRAN_2005_20050316

	if( HeartOfLoveItemBag->GetBagCount() > 0 )
	{
		// 2/20 �� Ȯ���� �̺�Ʈ�� ������ ����
		if( (rand()%20) < g_ItemDropRateForgHeartOfLove )
		{
			DropItemNum = (rand()%HeartOfLoveItemBag->GetBagCount());
			dur  = 0;

			// Option�� 3-7���� (ItemBag4���� ���� 3���� �����Ǿ� �ִ�
			if( gLanguage == 0 )
			{	// �ѱ��� ���� 5 - 7����
				level= HeartOfLoveItemBag->GetLevel(DropItemNum)+(rand()%2);
			}
			else
			{
				level= HeartOfLoveItemBag->GetLevel(DropItemNum)+(rand()%5);
			}
			
			type = ItemGetNumberMake(HeartOfLoveItemBag->GetType(DropItemNum), HeartOfLoveItemBag->GetIndex(DropItemNum));
			
			Option1 = rand()%2;
			Option2 = rand()%2;

			if( !Option2 || !Option1 )
			{	// ������ �ϳ��� 0�̸� 
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )				
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}
			
			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

			// ����..
			PMSG_SERVERCMD	ServerCmd;
			PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

			ServerCmd.CmdType = 0;
			ServerCmd.X = x;
			ServerCmd.Y = y;
			MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

#ifdef EXTEND_LOG_SYSTEM_20060202
				CItem EventItem;
				EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

				LogAddTD("[%s][%s][Lucky Box Item Drop][Fire Cracker] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
				LogAdd("FireCracker Event ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	
	// ���� ����ϴ� ���

	// ����..
	PMSG_SERVERCMD	ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));

	ServerCmd.CmdType = 0;
	ServerCmd.X = (BYTE)lpObj->X;
	ServerCmd.Y = (BYTE)lpObj->Y;
	MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

	MapC[lpObj->MapNumber].MoneyItemDrop(2548, x ,y);
	
	#else // else EVENT_SONGKRAN_2005_20050316
		MapC[lpObj->MapNumber].MoneyItemDrop(50000, x ,y);
	
	#endif // EVENT_SONGKRAN_2005_20050316

#else // else FOR_THAILAND

	MapC[lpObj->MapNumber].MoneyItemDrop(2004, x ,y);

#endif // FOR_THAILAND
}


// ��ź�Ǻ� ���� �̺�Ʈ ����
void StarOfXMasOpenEven(LPOBJECTSTRUCT lpObj)
{	

#ifdef EVENT_ITEMBAG_UPGRADE
	StarOfXMasItemBag->DropItem(lpObj->m_Index);

#else // else EVENT_ITEMBAG_UPGRADE
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

	if( !StarOfXMasItemBag )
		return;

	if( StarOfXMasItemBag->GetBagCount() > 0 )
	{
		// 2/10 �� Ȯ���� �̺�Ʈ�� ������ ����
		if( (rand()%10) < g_XMasEvent_ItemDropRateForStarOfXMas )
		{
			DropItemNum = (rand()%StarOfXMasItemBag->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;			

			// Option�� 3-7���� (ItemBag4���� ���� 3���� �����Ǿ� �ִ�
	#ifdef FOR_CHINA
			if( rand()%100 < 2 )
			{
				level= 8;
			}
			else
			{
				level= StarOfXMasItemBag->GetLevel(DropItemNum)+(rand()%5);
			}
	#else // else FOR_CHINA
			level= StarOfXMasItemBag->GetLevel(DropItemNum)+(rand()%5);
	#endif // FOR_CHINA
			
			type = ItemGetNumberMake(StarOfXMasItemBag->GetType(DropItemNum), StarOfXMasItemBag->GetIndex(DropItemNum));
			
			Option1 = rand()%2;
			Option2 = rand()%2;

			if( (rand()%5) < 1 )
			{
				Option3 = 3;
			}
			else Option3 = (rand()%3);

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

			LogAddTD("[%s][%s][Lucky Box Item Drop][Star Of XMas] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAdd("XMas Event ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	x    = lpObj->X;
	y    = lpObj->Y;	
	MapC[lpObj->MapNumber].MoneyItemDrop(1225, x ,y);	
#endif // EVENT_ITEMBAG_UPGRADE
}


#ifdef RING_EVENT_ITEMBOX_20031124

void RingEventItemBoxOpen(LPOBJECTSTRUCT lpObj)
{
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

//	if( !g_bDoRingEvent)	return;								// ���� �̺�Ʈ�� ������ �����ϵ���


#ifndef FRIENDSHIP_EVENT_RING_BOX_20040211
	if (rand()%g_iRingDropGiftRate == 0) {
		// --- SendEventGiftWinner() ---						// ������ ���� ��ǰ�� ź��.
		if( lpObj->UseEventServer )
		return;

		lpObj->UseEventServer = TRUE;

		PMSG_REQ_REG_RINGGIFT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x10, sizeof(pMsg));

		pMsg.iINDEX = lpObj->m_Index;
		memcpy (pMsg.szUID, lpObj->AccountID, MAX_IDSTRING);
		pMsg.szUID[sizeof(pMsg.szUID)] = 0;
		pMsg.btGiftKind = 0;

		DataSendEventChip((char*)&pMsg, sizeof(pMsg));

		LogAddTD("[Ring Event] [%s][%s] Request to Register Gift - Gift Kind (%d)", lpObj->AccountID, lpObj->Name, 0);
		// --- SendEventGiftWinner() ---
		return;
	}
#else // else FRIENDSHIP_EVENT_RING_BOX_20040211
	// ����� ������ ���� ���� ������ �����ǵ� ��� (�ѱ� ~ 2004.03.30)
//	int iMaxHitUser = lpObj->m_Index;
//	int iType = ItemGetNumberMake(14, 21);	
//	int iItemLevel = 2;
//	ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE) lpObj->X, (BYTE) lpObj->Y, iType, iItemLevel, (BYTE)0, 0, 0, 0, iMaxHitUser);
#endif // FRIENDSHIP_EVENT_RING_BOX_20040211

	if( RingEventItemBag->GetBagCount() > 0 )
	{
		// 9/10 �� Ȯ���� �̺�Ʈ�� ������ ����
//		if( (rand()%10) < g_XMasEvent_ItemDropRateForStarOfXMas )
		if( (rand()%10) < 9 )
		{
			DropItemNum = (rand()%RingEventItemBag->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;

			INT iRND = rand()%100;

			if (iRND < 5) {			// 5%
				level = 9;
			}
			else if (iRND < 20) {	// 15%
				level = 8;
			}
			else if (iRND < 50) {	// 30%
				level = 7;
			}
			else {					// 50%
				level = 6;
			}

/*			
			if (rand()%100 < 30) {
				// + 6
				level = 6;
			}
			else if (rand()%100 < 12) {
				// + 7
				level = 7;
			}
			else if (rand()%100 < 5) {
				// + 8
				level = 8;
			}
			else if (rand()%100 < 2) {
				// + 9
				level = 9;
			}
			else {
				// + 5
				level = 5;
			}
*/
			type = ItemGetNumberMake(RingEventItemBag->GetType(DropItemNum), RingEventItemBag->GetIndex(DropItemNum));

			Option1 = 1;			// ��ų
			Option2 = rand()%2;		// ���

			if( (rand()%5) < 1 )	// �߰��ɼ�
			{
				Option3 = 3;
			}
			else Option3 = (rand()%3);
			
/*			
			if( (rand()%100) < 70 ) {
				Option1 = 1;	// ��ų
			}

			if( (rand()%100) < 4 ) {
				Option2 = 1;	// ���
			}
			
			int option3rand = (rand()%100);
			int	optionc		= (rand()%3);

			switch( optionc )
			{	// �� 80%
			case 0 :
				if( option3rand < 4 ) Option3 = 3;	// �ɼ�
				break;
			case 1 :
				if( option3rand < 10 ) Option3 = 2;	// �ɼ�
				break;
			case 2 :
				if( option3rand < 20 ) Option3 = 1;	// �ɼ�
				break;
			}
*/

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );
			LogAddTD("[%s][%s][Ring Event](%d,%d,%d) ItemDrop : Item:%s %d Level:%d op1:%d op2:%d op3:%d",
				lpObj->AccountID, lpObj->Name,
				lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAddTD("[Ring Event] ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	x    = lpObj->X;
	y    = lpObj->Y;	
	MapC[lpObj->MapNumber].MoneyItemDrop(100000, x ,y);
}

#endif // RING_EVENT_ITEMBOX_20031124


#ifdef FRIENDSHIP_EVENT_RING_BOX_20040211		// �� ģ�� �̺�Ʈ - ������ ���� ������ ��� ��ƾ

void FriendShipItemBoxOpen(LPOBJECTSTRUCT lpObj)
{
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

//	if( !g_bDoRingEvent)	return;								// ���� �̺�Ʈ�� ������ �����ϵ���


	// ������ ������ ���� ���� ������ �����ǵ� ���  (�ѱ� ~ 2004.03.30)
//	int iMaxHitUser = lpObj->m_Index;
//	int iType = ItemGetNumberMake(14, 21);	
//	int iItemLevel = 2;
//	ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE) lpObj->X, (BYTE) lpObj->Y, iType, iItemLevel, (BYTE)0, 0, 0, 0, iMaxHitUser);


#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119	// ���, ����(������ ��)�� ���� �߰�/����
	if( FriendShipItemBag->GetBagCount() > 0 )
#else
	if( RingEventItemBag->GetBagCount() > 0 )
#endif
	{
		// 9/10 �� Ȯ���� �̺�Ʈ�� ������ ����
		//if( (rand()%10) < 9 )
		if(1)
		{
#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119	// ���, ����(������ ��)�� ���� �߰�/����
			DropItemNum = (rand()%FriendShipItemBag->GetBagCount());
#else
			DropItemNum = (rand()%RingEventItemBag->GetBagCount());
#endif			
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119	// ���, ����(������ ��)�� ���� �߰�/����
			// ������ 7 ~ 9 ����
			level= FriendShipItemBag->GetLevel(DropItemNum)+(rand()%3);
#else
			INT iRND = rand()%100;

			if (iRND < 5) {			// 5%
				level = 9;
			}
			else if (iRND < 20) {	// 15%
				level = 8;
			}
			else if (iRND < 50) {	// 30%
				level = 7;
			}
			else {					// 50%
				level = 6;
			}
#endif	// UPDATE_LOWLEVEL_SUPPORT_RING_20070119

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119	// ���, ����(������ ��)�� ���� �߰�/����
			type = ItemGetNumberMake(FriendShipItemBag->GetType(DropItemNum), FriendShipItemBag->GetIndex(DropItemNum));
#else
			type = ItemGetNumberMake(RingEventItemBag->GetType(DropItemNum), RingEventItemBag->GetIndex(DropItemNum));
#endif

			Option1 = 1;			// ��ų
			Option2 = rand()%2;		// ���

			if( (rand()%5) < 1 )	// �߰��ɼ�
			{
				Option3 = 3;
			}
			else Option3 = (rand()%3);
			
			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );
			LogAddTD("[%s][%s][FriendShip Ring Event](%d,%d,%d) ItemDrop : Item:%s %d Level:%d op1:%d op2:%d op3:%d",
				lpObj->AccountID, lpObj->Name,
				lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAddTD("[FriendShip Ring Event] ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
			return;
#endif // EXTEND_LOG_SYSTEM_20060202
			return;
		}
	}
	//x    = lpObj->X;
	//y    = lpObj->Y;	
	//MapC[lpObj->MapNumber].MoneyItemDrop(100000, x ,y);	
}

#endif // FRIENDSHIP_EVENT_RING_BOX_20040211


#ifdef DARK_LORD_HEART_EVENT_20040810				// ��ũ�ε��� ���� ������ ��� ó��

void DarkLordHeartItemBoxOpen(LPOBJECTSTRUCT lpObj)
{
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int DropItemRate	= 0;						// ������ ��� Ȯ���� ��� �ٸ���.
	

	if (((DWORD)rand())%60000 < g_iDarkLordHeartOffEventRate) {
		// �������� ��ǰ�� ��÷�Ǿ��� -> ����� �õ��Ѵ�.

		if (lpObj->Level > 100) {					// ������ 100�� ������ ��ǰ�� ��÷�� �� ����.
			return;
		}

		EGReqRegDLOfflineGift (lpObj->m_Index);
	}

	if( DarkLordHeartItemBag->GetBagCount() > 0 )
	{
		DropItemNum		= (rand()%DarkLordHeartItemBag->GetBagCount());
		DropItemRate	= DarkLordHeartItemBag->GetOp1(DropItemNum);			// Option1 (��ų) �κ��� ���Ȯ���� ����Ѵ�.

		// �����ۿ� ������ ���Ȯ�� ����
	#ifndef FOR_NEW_TESTSERVER		
		if( (rand()%100) < DropItemRate && lpObj->Level <= 100)
	#else // else FOR_NEW_TESTSERVER
		if( (rand()%100) < DropItemRate)
	#endif // FOR_NEW_TESTSERVER
//		if( (rand()%100) < DropItemRate)
		{
			level			= DarkLordHeartItemBag->GetLevel(DropItemNum);

			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;

			type = ItemGetNumberMake(DarkLordHeartItemBag->GetType(DropItemNum), DarkLordHeartItemBag->GetIndex(DropItemNum));

			Option1 = (rand()%100 < 70) ? 1 : 0;		// ��ų
			Option2 = (rand()%100 < 10) ? 1 : 0;		// ���

			int iOption3Rand = rand()%20;
			if (iOption3Rand < 1) {
				Option3 = 3;							// + 12
			}
			else if (iOption3Rand < 5) {
				Option3 = 2;							// + 8
			}
			else if (iOption3Rand < 9) {
				Option3 = 1;							// + 4
			}
			else {
				Option3 = 0;							// ����
			}
			
			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

			LogAddTD("[%s][%s][Lucky Box Item Drop][Darklord Heart] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAdd("[DarkLord Heart Event] ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	
	x    = lpObj->X;
	y    = lpObj->Y;	
	MapC[lpObj->MapNumber].MoneyItemDrop(10000, x ,y);	
}

#endif // DARK_LORD_HEART_EVENT_20040810


#ifdef HIDDEN_TREASUREBOX_EVENT_20050804

void HiddenTreasureBoxItemBoxOpen(LPOBJECTSTRUCT lpObj)
{
	// CItemBagEx::DropItem() �� ������� �ʰ� �������� �̾Ƽ� ���� ����Ѵ�.
	CItem objTempItem;
	int nItemIndex;
	if (HiddenTreasureBoxItemBag->PickItem(objTempItem, nItemIndex) == FALSE) {
		LogAddTD("[Hidden TreasureBox Event] [%s][%s] Item Pick Failed - Data Error",
			lpObj->AccountID,
			lpObj->Name
			);
		
		// �� ���
		MapC[lpObj->MapNumber].MoneyItemDrop(200000, lpObj->X, lpObj->Y);
		return;
	}

	int iItemSuccessRate	= HiddenTreasureBoxItemBag->GetMinLevel(nItemIndex);
	if (rand()%100 < iItemSuccessRate) {
		// ������ ���
		objTempItem.m_Level = 0;

		if (objTempItem.m_Option1) {
			objTempItem.m_Option1 = rand()%2;
		}
		if (objTempItem.m_Option2) {
			objTempItem.m_Option2 = rand()%2;
		}
		if (objTempItem.m_Option3) {
			if( (rand()%5) < 1 )
			{
				objTempItem.m_Option3 = 3;
			}
			else objTempItem.m_Option3 = (rand()%3);
		}
		if (objTempItem.m_NewOption) {
			objTempItem.m_NewOption =  NewOptionRand(objTempItem.m_Level);
			objTempItem.m_Option1 = 1;
			objTempItem.m_Option2 = 0;
		}
		
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, objTempItem.m_Type, objTempItem.m_Level, (BYTE)objTempItem.m_Durability, objTempItem.m_Option1, objTempItem.m_Option2, objTempItem.m_Option3, lpObj->m_Index, objTempItem.m_NewOption);

		BYTE ExOption[8];		
		ItemIsBufExOption(ExOption, &objTempItem);
		LogAddTD("[Hidden TreasureBox Event] [%s][%s] Event ItemDrop : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d exop:[%d,%d,%d,%d,%d,%d]", 
				lpObj->AccountID, lpObj->Name,
				lpObj->MapNumber, lpObj->X, lpObj->Y,
				ItemAttribute[objTempItem.m_Type].Name, objTempItem.m_Type, objTempItem.m_Level, objTempItem.m_Option1, objTempItem.m_Option2, objTempItem.m_Option3, 
				ExOption[0],
				ExOption[1],
				ExOption[2],
				ExOption[3],
				ExOption[4],
				ExOption[5]
				);
	}
	else {
		// �� ���
		INT iDropZen = 200000;
		MapC[lpObj->MapNumber].MoneyItemDrop(iDropZen, lpObj->X, lpObj->Y);
		LogAddTD("[Hidden TreasureBox Event] [%s][%s] Event ZenDrop : %d : (%d)(%d/%d)", 
				lpObj->AccountID, lpObj->Name, 
				iDropZen,
				lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
}

#endif	// HIDDEN_TREASUREBOX_EVENT_20050804

#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212		// ũ�������� �������� �̺�Ʈ ������ ��� ó��
// ������������ ���
void RedRibbonBoxOpen( LPOBJECTSTRUCT lpObj )
{	
	RedRibbonBoxEventItemBag->DropRedRibbonBoxEventItem( lpObj->m_Index );		
}

// �ʷϸ������� ���
void GreenRibbonBoxOpen( LPOBJECTSTRUCT lpObj )
{
	GreenRibbonBoxEventItemBag->DropGreenRibbonBoxEventItem( lpObj->m_Index );		
}

// �Ķ��������� ���
void BlueRibbonBoxOpen( LPOBJECTSTRUCT lpObj )
{
	BlueRibbonBoxEventItemBag->DropBlueRibbonBoxEventItem( lpObj->m_Index );		
}
#endif // #ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212

#ifdef VALENTINES_DAY_BOX_EVENT_20060124		// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ ������ ��� ó��
// ��ȫ���ݸ����� ���
void PinkChocolateBoxOpen( LPOBJECTSTRUCT lpObj )
{	
	PinkChocolateBoxEventItemBag->DropPinkChocolateBoxEventItem( lpObj->m_Index );		
}

// �������ݸ����� ���
void RedChocolateBoxOpen( LPOBJECTSTRUCT lpObj )
{
	RedChocolateBoxEventItemBag->DropRedChocolateBoxEventItem( lpObj->m_Index );		
}

// �Ķ����ݸ����� ���
void BlueChocolateBoxOpen( LPOBJECTSTRUCT lpObj )
{
	BlueChocolateBoxEventItemBag->DropBlueChocolateBoxEventItem( lpObj->m_Index );		
}
#endif // #ifdef VALENTINES_DAY_BOX_EVENT_20060124

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222		// ȭ��Ʈ���� �������� �̺�Ʈ ������ ��� ó��
// ������������� ���
void LightPurpleCandyBoxOpen( LPOBJECTSTRUCT lpObj )
{	
	LightPurpleCandyBoxEventItemBag->DropLightPurpleCandyBoxEventItem( lpObj->m_Index );		
}

// ��ȫ���ݸ����� ���
void VermilionCandyBoxOpen( LPOBJECTSTRUCT lpObj )
{
	VermilionCandyBoxEventItemBag->DropVermilionCandyBoxEventItem( lpObj->m_Index );		
}

// ��û���ݸ����� ���
void DeepBlueCandyBoxOpen( LPOBJECTSTRUCT lpObj )
{
	DeepBlueCandyBoxEventItemBag->DropDeepBlueCandyBoxEventItem( lpObj->m_Index );		
}
#endif // #ifdef WHITEDAY_CANDY_BOX_EVENT_20060222

#ifdef MODIFY_KUNDUN_REWARD_01_20040915				//  ����̺�Ʈ ������ ��� ó��
void KundunEventItemBoxOpen(LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY)
{

	KundunEventItemBag->DropKundunEventItem(lpObj->m_Index, btMapNumber, cX, cY);

}
#endif // MODIFY_KUNDUN_REWARD_01_20040915

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
void HallowinDayEventItemBoxOpen(LPOBJECTSTRUCT lpObj)
{
	HallowinDayEventItemBag->DropHallowinEventItem( lpObj );
}
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef CHRISTMAS_STAR_20061211
void ChristmasStarDrop( LPOBJECTSTRUCT lpObj )
{
	// �������� ����߸��� ������ �Ͷ߸���.
	PMSG_SERVERCMD	ServerCmd;
	PHeadSubSetB( ( LPBYTE )&ServerCmd, 0xF3, 0x40, sizeof( ServerCmd ) );			
	ServerCmd.CmdType = 0;
	ServerCmd.X = lpObj->X;
	ServerCmd.Y = lpObj->Y;
	MsgSendV2( lpObj, ( LPBYTE )&ServerCmd, sizeof( ServerCmd ) );
	DataSend( lpObj->m_Index, ( LPBYTE )&ServerCmd, sizeof( ServerCmd ) );
}
#endif
//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���� ������(grooving)
//UPDATE_XMASEVENT_MAP_20080930
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
void ChristmasFireCrackerDrop( LPOBJECTSTRUCT lpObj )
{
	// �������� ����߸��� ������ �Ͷ߸���.
	PMSG_SERVERCMD	ServerCmd;
	PHeadSubSetB( ( LPBYTE )&ServerCmd, 0xF3, 0x40, sizeof( ServerCmd ) );			
	ServerCmd.CmdType = 59;
	ServerCmd.X = lpObj->X;
	ServerCmd.Y = lpObj->Y;
	MsgSendV2( lpObj, ( LPBYTE )&ServerCmd, sizeof( ServerCmd ) );
	DataSend( lpObj->m_Index, ( LPBYTE )&ServerCmd, sizeof( ServerCmd ) );
}
#endif //MODIFY_ITEM_SERIAL_EXPAND_20080227
//<-------------

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
void CherryBlossomEventItemBoxOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	if ( pCherryBlossomEventItemBag->DropCherryBlossomEventItem( lpObj->m_Index, btMapNumber, cX, cY) )
	{
		// �������� ����߸��� ���� �ֳ����� ����Ʈ ���
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB( ( LPBYTE )&ServerCmd, 0xF3, 0x40, sizeof( ServerCmd ) );
		ServerCmd.CmdType = 58;
		/*
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;*/
		ServerCmd.X = HIBYTE(lpObj->m_Index);
		ServerCmd.Y = LOBYTE(lpObj->m_Index);
		MsgSendV2( lpObj, ( LPBYTE )&ServerCmd, sizeof( ServerCmd ) );
		DataSend( lpObj->m_Index, ( LPBYTE )&ServerCmd, sizeof( ServerCmd ) );
	}
}

BOOL CherryBlossomEventItemBoxAOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, int* pMoney)
{
	return pCherryBlossomEventItemBagA->DropCherryBlossomEventItem( lpObj->m_Index, btMapNumber, cX, cY, pMoney );
}

BOOL CherryBlossomEventItemBoxBOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, int* pMoney)
{
	return pCherryBlossomEventItemBagB->DropCherryBlossomEventItem( lpObj->m_Index, btMapNumber, cX, cY, pMoney );
}

BOOL CherryBlossomEventItemBoxCOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, int* pMoney)
{
	return pCherryBlossomEventItemBagC->DropCherryBlossomEventItem( lpObj->m_Index, btMapNumber, cX, cY, pMoney );
}

#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303
void CastleHuntZoneBossRewardOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{

	CastleHuntZoneBossItemBag->DropCastleHuntZoneBossReward(lpObj->m_Index, btMapNumber, cX, cY);

}

#endif // ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
void CrywolfDarkElfItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	CrywolfDarkElfItemBag->DropCrywolfDarkElfItem( lpObj->m_Index, btMapNumber, cX, cY );
}
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
void CrywolfBossMonsterItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	CrywolfBossMonsterItemBag->DropCrywolfBossMonsterItem( lpObj->m_Index, btMapNumber, cX, cY );
}
#endif

#ifdef KANTURU_MONSTER_ITEMDROP_20060807
void KanturuMayaHandItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	// 20060828 (RKFKA) ���� �� ������ �ϳ��� ��� �ǵ��� ����
	KanturuMayaHandItemBag->DropKanturuMayaHandItem( lpObj->m_Index, btMapNumber, cX, cY );
}

void KanturuNightmareItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	// 20060828 (RKFKA) ����Ʈ�޾� ������ �ϳ��� ��� �ǵ��� ����
	KanturuNightmareItemBag->DropKanturuNightmareItem( lpObj->m_Index, btMapNumber, cX, cY );
}
#endif

#ifdef ADD_RAKLION_20080408
void RaklionSelupanItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	pRaklionSelupanItemBag->DropRaklionSelupanItem( lpObj->m_Index, btMapNumber, cX, cY );
}
#endif // ADD_RAKLION_20080408

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ���� ��ȯ �̺�Ʈ ��� ������
BOOL LuckyCoinEventItemBag10Open( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY)
{
	return pLuckyCoinEventItemBag10->DropEventItem( lpObj->m_Index, btMapNumber, cX, cY );
}

BOOL LuckyCoinEventItemBag20Open( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY)
{
	return pLuckyCoinEventItemBag20->DropEventItem( lpObj->m_Index, btMapNumber, cX, cY );
}

BOOL LuckyCoinEventItemBag30Open( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY)
{
	return pLuckyCoinEventItemBag30->DropEventItem( lpObj->m_Index, btMapNumber, cX, cY );
}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029 

#ifdef ADD_GAMBLING_20090120
BYTE GamblingSystemItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY, BYTE btItemType )
{
	return pGamblingSystemItemBag->GetGamblingSystemItem( lpObj->m_Index, btMapNumber, cX, cY, btItemType );
}
#endif // ADD_GAMBLING_20090120

// ����� ���ڴ� ����� �ְ� ������ �ֿ���� ����.
// ����� ���� ���� �̺�Ʈ ����
void LuckyBoxOpenEven(LPOBJECTSTRUCT lpObj)
{	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

	if( !LuckboxItemBag )
		return;

	if( LuckboxItemBag->GetBagCount() > 0 )
	{
		// 2/10 �� Ȯ���� �̺�Ʈ�� ������ ����
		if( (rand()%10) < 2 )
		{
			DropItemNum = (rand()%LuckboxItemBag->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;
			

			level= LuckboxItemBag->GetLevel(DropItemNum)+(rand()%2);
			
			type = ItemGetNumberMake(LuckboxItemBag->GetType(DropItemNum), LuckboxItemBag->GetIndex(DropItemNum));
			
			Option1 = rand()%2;
			Option2 = rand()%2;
			if( (rand()%5) < 1 )
			{
				Option3 = 3;
			}
			else Option3 = 1+(rand()%2);

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12,15)) ||
				(type == MAKE_ITEMNUM(14,13)) ||
				(type == MAKE_ITEMNUM(14,14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );

			LogAddTD("[%s][%s][Lucky Box Item Drop] : (%d)(X:%d/Y:%d) Item:%s(%d) Level:%d Op1:%d Op2:%d Op3:%d",
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAdd("Event ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	
	for( int n=0; n<3+(rand()%4); n++)
	{
		x    = lpObj->X-2;
		y    = lpObj->Y-2;
		x   += rand()%3;
		y   += rand()%3;
		MapC[lpObj->MapNumber].MoneyItemDrop(1000, x ,y);
	}
}

// ����ƽ� ���� ���� �� ���� ����(?)
BOOL AttackEvent53BagOpen(LPOBJECTSTRUCT lpObj)
{	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

	if( !Mon53 )
		return FALSE;

	if( Mon53->GetBagCount() > 0 )
	{
		// 2/10 �� Ȯ���� �̺�Ʈ�� ������ ����
		//if( (rand()%10) < 2 )
		{
			DropItemNum = (rand()%Mon53->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;

			level= Mon53->GetLevel(DropItemNum);
			type = ItemGetNumberMake(Mon53->GetType(DropItemNum), Mon53->GetIndex(DropItemNum));
			
			if((rand()%100) < 6 ) Option1 = 1;
			if((rand()%100) < 4 ) Option2 = 1;

			int optionrand, optionc;

			optionc = (rand()%3);
			optionrand = (rand()%100);
			
			switch( optionc )
			{
			case 0 :
				if( optionrand < 4 ) Option3 = 3;	// �ɼ�
				break;
			case 1 :
				if( optionrand < 8 ) Option3 = 2;	// �ɼ�
				break;
			case 2 :
				if( optionrand < 12 ) Option3 = 1;	// �ɼ�
				break;
			}
			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12,15)) ||
				(type == MAKE_ITEMNUM(14,13)) ||
				(type == MAKE_ITEMNUM(14,14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}
			
			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}
			
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			LogAdd("Event ItemDrop(53) : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
			return TRUE;
		}
	}
	//LogAdd("���濡�� �ֱ� ���� %d", Mon53->GetBagCount());
	return FALSE;
}


BOOL AttackEvent55BagOpen(LPOBJECTSTRUCT lpObj)
{	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

	if( !Mon55 )
		return FALSE;

	if( Mon55->GetBagCount() > 0 )
	{
		// 2/10 �� Ȯ���� �̺�Ʈ�� ������ ����
		//if( (rand()%10) < 2 )
		{
			DropItemNum = (rand()%Mon55->GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;

			level= Mon55->GetLevel(DropItemNum)+(rand()%2);
			type = ItemGetNumberMake(Mon55->GetType(DropItemNum), Mon55->GetIndex(DropItemNum));
			
			if((rand()%100) < 6 ) Option1 = 1;
			if((rand()%100) < 4 ) Option2 = 1;

			int optionrand, optionc;

			optionc = (rand()%3);
			optionrand = (rand()%100);
			
			switch( optionc )
			{
			case 0 :
				if( optionrand < 4 ) Option3 = 3;	// �ɼ�
				break;
			case 1 :
				if( optionrand < 8 ) Option3 = 2;	// �ɼ�
				break;
			case 2 :
				if( optionrand < 12 ) Option3 = 1;	// �ɼ�
				break;
			}


			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12,15)) ||
				(type == MAKE_ITEMNUM(14,13)) ||
				(type == MAKE_ITEMNUM(14,14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			LogAdd("Event ItemDrop(55) : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
			return TRUE;
		}
	}
	//LogAdd("���濡�� �ֱ� ���� %d", Mon55->GetBagCount());
	return FALSE;
}

#define GETMUTONUM1(MutoNum)	(MutoNum/1000000)							// �Ǿ� ���ڸ�
#define GETMUTONUM2(MutoNum)	((MutoNum/1000)-(MutoNum/1000000)*1000)		// �߰� ���ڸ�
#define GETMUTONUM3(MutoNum)	(MutoNum%1000)								// �Ǿ� ���ڸ�

// 0x01 �̺�Ʈ ������ ���� �̺�ƮĨ ������ �޴´�
void	EGRecvEventChipInfo(LPPMSG_ANS_VIEW_EC_MN aRecv)
{	
	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iINDEX];	

	PMSG_EVENTCHIPINFO	eventchipeventinfo;
	PHeadSetB((LPBYTE)&eventchipeventinfo, 0x94, sizeof(eventchipeventinfo));

	eventchipeventinfo.Type	= 0; // bugfix_alice_050113 : Related keyword ' FOR_BLOODCASTLE & PMSG_EVENTCHIPINFO '
	eventchipeventinfo.ChipCount = aRecv->nEVENT_CHIPS;

	lpObj->EventChipCount = aRecv->nEVENT_CHIPS;
	lpObj->MutoNumber = aRecv->iMUTO_NUM;

#ifdef FOR_CHINA
	eventchipeventinfo.Type = 0;
#endif // FOR CHINA
	
#ifdef EVENT_RENA_FOR_CASTLE_SIEGE_20041116
	eventchipeventinfo.Type = 0;
	DataSend(lpObj->m_Index, (LPBYTE)&eventchipeventinfo, eventchipeventinfo.h.size);
	return;
#endif // EVENT_RENA_FOR_CASTLE_SIEGE_20041116

#ifdef EVENT_RENA_20040909
	DataSend(lpObj->m_Index, (LPBYTE)&eventchipeventinfo, eventchipeventinfo.h.size);			
	lpObj->UseEventServer = FALSE; // bugfix_alice_050113
	return;
#endif // EVENT_RENA_20040909

#ifdef EVENT_STONE
	DataSend(lpObj->m_Index, (LPBYTE)&eventchipeventinfo, eventchipeventinfo.h.size);
	lpObj->UseEventServer = FALSE; // bugfix_alice_050113
	return;
#endif // EVENT_STONE

#if defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )
	char msg[255];

	if( aRecv->iMUTO_NUM != 0 )
	{
		eventchipeventinfo.MutoNum[0] = GETMUTONUM1(aRecv->iMUTO_NUM);
		eventchipeventinfo.MutoNum[1] = GETMUTONUM2(aRecv->iMUTO_NUM);
		eventchipeventinfo.MutoNum[2] = GETMUTONUM3(aRecv->iMUTO_NUM);

		if( pEventObj )
		{
			wsprintf(msg, "����� ���ڴ� %03d-%03d-%03d �Դϴ�", 
					 eventchipeventinfo.MutoNum[0], eventchipeventinfo.MutoNum[1], eventchipeventinfo.MutoNum[2]);
			ChatTargetSend(pEventObj, msg, lpObj->m_Index);	// "�����ڿ��� �ƹ��͵� �� �� ����!!"
		}
	}
	else
	{
		if( pEventObj )
		{
			wsprintf(msg, "��ϵ� ����� ���ڰ� �����ϴ�");
			ChatTargetSend(pEventObj, msg, lpObj->m_Index);	// "�����ڿ��� �ƹ��͵� �� �� ����!!"
		}
		
		eventchipeventinfo.MutoNum[0] = -1;
		eventchipeventinfo.MutoNum[1] = -1;
		eventchipeventinfo.MutoNum[2] = -1;
	}

	DataSend(lpObj->m_Index, (LPBYTE)&eventchipeventinfo, eventchipeventinfo.h.size);			
	lpObj->UseEventServer = FALSE; // bugfix_alice_050113
	return;

#endif // EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO
}

// 0x02 �̺�Ʈ ������ ���� �̺�ƮĨ�� ����� ��� ���� �޴´�
void	EGResultRegEventChip(LPPMSG_ANS_REGISTER_EVENTCHIP aRecv)
{
	PMSG_REGEVENTCHIP_RESULT Result;
	PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iINDEX];	
	int aIndex = aRecv->iINDEX;

	if( aRecv->bSUCCESS == FALSE )
	{
		Result.ChipCount = -1;
		LogAddTD("[EventChip] [%s][%s] RegEventServer Fail (RegEventchip) %d", 
			   lpObj->AccountID, lpObj->Name, aRecv->Pos);
	}
	else
	{
		Result.ChipCount = aRecv->nEVENT_CHIPS;
		gObjInventoryDeleteItem(aIndex, aRecv->Pos);
		GCInventoryItemDeleteSend(aIndex, aRecv->Pos, 1);
		LogAddTD("[EventChip] [%s][%s] Delete EventChip (%d)", lpObj->AccountID, lpObj->Name, aRecv->Pos);
	}

	Result.Type = 0;
	
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

#ifdef FOR_CHINA
	lpObj->Money += 3300;
	GCMoneySend(aIndex, lpObj->Money);
#endif // FOR CHINA

	lpObj->UseEventServer = FALSE;
}

// 0x03 �̺�Ʈ ������ ���� �����ȣ ��Ͽ� ���� ��� ���� �޴´�
void	EGRecvRegMutoNum(LPPMSG_ANS_REGISTER_MUTONUM aRecv)
{
	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iINDEX];	
	int aIndex = aRecv->iINDEX;

	PMSG_GETMUTONUMBER_RESULT Result;
	PHeadSetB((LPBYTE)&Result, 0x96, sizeof(Result));	

	if( !gObjFind10EventChip(aIndex) )
	{	// ������ ������ ����		
		Result.MutoNum[0] = -1;
		Result.MutoNum[1] = 0;
		Result.MutoNum[2] = 0;
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
		lpObj->UseEventServer = FALSE;
		return;
	}

	// �̺�Ʈ Ĩ ������ �����
	gObjDelete10EventChip(aIndex);
	
	Result.MutoNum[0] = GETMUTONUM1(aRecv->iMUTO_NUM);
	Result.MutoNum[1] = GETMUTONUM2(aRecv->iMUTO_NUM);
	Result.MutoNum[2] = GETMUTONUM3(aRecv->iMUTO_NUM);	

	lpObj->MutoNumber = aRecv->iMUTO_NUM;

	LogAddTD("[EventChip] [%s][%s] Make MutoNumber %d,%d,%d", 
			 lpObj->AccountID, lpObj->Name, Result.MutoNum[0], Result.MutoNum[1], Result.MutoNum[2]);

	DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
	
#ifdef FOR_CHINA
	lpObj->Money += (3300*10);
	GCMoneySend(aIndex, lpObj->Money);
#endif // FOR CHINA

	lpObj->UseEventServer = FALSE;
}

// 0x04 �̺�Ʈ ������ ���� ������ ���ذͿ� ���� ����� �޴´�
void	EGRecvChangeRena(LPPMSG_ANS_RESET_EVENTCHIP aRecv)
{
	PMSG_REGEVENTCHIP_RESULT Result;
	PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iINDEX];	
	int aIndex = aRecv->iINDEX;

	if( aRecv->bSUCCESS )
	{	// ���� ȯ��
		lpObj->Money += lpObj->EventChipCount*3000;

		GCMoneySend(aIndex, lpObj->Money);
		LogAddTD("[EventChip] [%s][%s] ChangeRena AddMoney(%d)", 
			      lpObj->AccountID, lpObj->Name, lpObj->EventChipCount*3000);
	}
	else
	{
		LogAddTD("[EventChip] [%s][%s] ChangeRena Fail", 
			      lpObj->AccountID, lpObj->Name);
	}
	
	Result.ChipCount = 0;
	lpObj->EventChipCount = 0;

	DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
	lpObj->UseEventServer = FALSE;
}


#ifdef FOR_BLOODCASTLE

// 0x05 �̺�Ʈ ������ ���� ���� ������ �޴´�. (���� ĳ��)
void	EGRecvStoneInfo(LPPMSG_ANS_VIEW_STONES aRecv)
{
	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iINDEX];	

	PMSG_EVENTCHIPINFO	Result;
	PHeadSetB((LPBYTE)&Result, 0x94, sizeof(Result));

	#ifdef FOR_STONE_EXCHANGE
	if (aRecv->bSUCCESS)
		lpObj->iStoneCount = aRecv->iStoneCount;
	else
		lpObj->iStoneCount = 0;

	lpObj->MutoNumber = 0;

	Result.Type = 3;	// ����ȯ�� ������
	#else // else FOR_STONE_EXCHANGE
	if (aRecv->bSUCCESS)
		lpObj->EventChipCount = aRecv->iStoneCount;
	else
		lpObj->EventChipCount = 0;

	lpObj->MutoNumber = 0;

	Result.Type = 1;	// ���� ������
	#endif // FOR_STONE_EXCHANGE
	Result.ChipCount = aRecv->iStoneCount;

	DataSend(lpObj->m_Index, (LPBYTE)&Result, Result.h.size);

	char msg [128];
	wsprintf(msg, lMsg.Get(1177), Result.ChipCount);		// "����� ����� ���氳���� ���� %d�� �Դϴ�."
	ChatTargetSend(pEventObj, msg, lpObj->m_Index);
}


// 0x06 �̺�Ʈ ������ ���� ������ ����� ����� �޴´�. (���� ĳ��)
void	EGRecvRegStone(LPPMSG_ANS_REGISTER_STONES aRecv)
{
	PMSG_REGEVENTCHIP_RESULT Result;
	PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iINDEX];	
	int aIndex = aRecv->iINDEX;

	if(aRecv->bSUCCESS) {
		Result.ChipCount = aRecv->iStoneCount;
		gObjInventoryDeleteItem(aIndex, aRecv->iPosition);
		GCInventoryItemDeleteSend(aIndex, aRecv->iPosition, 1);
		LogAddTD("[Stone] [%s][%s] Delete Stones", lpObj->AccountID, lpObj->Name);
	}
	else {
		Result.ChipCount = -1;
		LogAddTD("[Stone] [%s][%s] RegEventServer Fail (Stones : %d)", lpObj->AccountID, lpObj->Name, aRecv->iStoneCount);
	}

	DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
	lpObj->UseEventServer = FALSE;
}


// 0x07 �̺�Ʈ ������ ���� ������ ������ ����� �޴´�. (���� ĳ��)
void	EGRecvDeleteStone(LPPMSG_ANS_DELETE_STONES aRecv)
{

}


#ifdef FOR_STONE_EXCHANGE

// 0x09 �̺�Ʈ ������ ���� ������ ȯ���� �Ϳ� ���� ����� �޴´�
void	EGRecvChangeStones(LPPMSG_ANS_RESET_EVENTCHIP aRecv)
{
	PMSG_REGEVENTCHIP_RESULT Result;
	PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iINDEX];	
	int aIndex = aRecv->iINDEX;

	if( aRecv->bSUCCESS )
	{	// ���� ȯ��
		lpObj->Money += lpObj->iStoneCount*3000;

		GCMoneySend(aIndex, lpObj->Money);
		LogAddTD("[Stones] [%s][%s] ChangeRena AddMoney(%d)", 
			      lpObj->AccountID, lpObj->Name, lpObj->iStoneCount*3000);
	}
	else
	{
		LogAddTD("[Stones] [%s][%s] ChangeRena Fail", 
			      lpObj->AccountID, lpObj->Name);
	}
	
	Result.ChipCount = 0;
	lpObj->iStoneCount = 0;

	DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
	lpObj->UseEventServer = FALSE;
}

#endif // FOR_STONE_EXCHANGE


#if defined ( FOR_KOREA ) || defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )

// 0x08 �̺�Ʈ ������ ���� �ø����� ����� ����� �޴´�. (�� ���ȭ 2�ֳ� ���)
void EGRecv2AnvRegSerial(LPPMSG_ANS_2ANIV_SERIAL aRecv)
{
#ifdef ADD_COUPON_EVENTITEM_LOG
	int nItemNumber = -1;
#endif // ADD_COUPON_EVENTITEM_LOG

	PMSG_ANS_2ANV_LOTTO_EVENT Result;
	PHeadSetB((LPBYTE)&Result, 0x9D, sizeof(Result));

	if(!CHECK_LIMIT(aRecv->iINDEX, MAX_OBJECT)) {
		LogAddTD("[Mu_2Anv_Event] Error : Index is out of bound [%d]",
			aRecv->iINDEX
			);
		return;
	}

	if (gObj[aRecv->iINDEX].Connected <= 2) {
		LogAddTD("[Mu_2Anv_Event] Error : Index is out of bound [%d]",
			aRecv->iINDEX
			);
		return;
	}

	#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// �߱� �̺�Ʈ - �ø����� ������ ������ ������ ����
	Result.btIsRegistered = aRecv->btIsRegistered;
	if (aRecv->btIsRegistered == 0) {		// ��ϼ���
		// ������ �Ͷ߸���.
		if (gObj[aRecv->iINDEX].Connected > 2) {
			PMSG_SERVERCMD	ServerCmd;
			PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

			ServerCmd.CmdType = 0;
			ServerCmd.X = (BYTE) gObj[aRecv->iINDEX].X;
			ServerCmd.Y = (BYTE) gObj[aRecv->iINDEX].Y;
			MsgSendV2(&gObj[aRecv->iINDEX], (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			DataSend(aRecv->iINDEX, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		}

		ItemSerialCreateSend(
			gObj[aRecv->iINDEX].m_Index, 
			ITEMMAKE_INVENTORY_1CELL, 
			(BYTE)gObj[aRecv->iINDEX].X, 
			(BYTE)gObj[aRecv->iINDEX].Y, 
			ItemGetNumberMake(13, 20),		// ������ ����
			3, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
	}

	LogAddTD("[Mu_2Anv_Event] Register Serial Result : %d [%s][%s]",
		aRecv->btIsRegistered,
		gObj[aRecv->iINDEX].AccountID,
		gObj[aRecv->iINDEX].Name
		);
	
	strcpy (Result.szGIFT_NAME, "Ring of Glory");
	DataSend(aRecv->iINDEX, (LPBYTE)&Result, Result.h.size);
	gObj[aRecv->iINDEX].UseEventServer = FALSE;
	return;
	#endif // MU_CHN_THANKS_RING_EVENT_20040908

	#if defined(FOR_KOREA) && !defined (COUPON_EVENT_ITEMLIST_20081022)
	Result.szGIFT_NAME[0] = 0;

	if (aRecv->btIsRegistered == 0) {		// ��ϼ���
		Result.btIsRegistered = 0;
		if (!CHECK_LIMIT(aRecv->iGiftNumber-1, 17)) {
			LogAddTD("[Mu_2Anv_Event] Error : Gift Index is out of bound [%d]",
				aRecv->iGiftNumber
				);
			Result.btIsRegistered = 2;
		}
		else {
			strcpy (Result.szGIFT_NAME, g_sz2ANV_GIFT_NAME[aRecv->iGiftNumber-1]);
		}

		if ( aRecv->iGiftNumber != 13 &&		// 10����
			 aRecv->iGiftNumber != 14			// 5����
			) 
		{
			// ���� �� ��츦 ������ �������� ������ �Ͷ߸���.
			if (gObj[aRecv->iINDEX].Connected > 2) {
				PMSG_SERVERCMD	ServerCmd;
				PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

				ServerCmd.CmdType = 0;
				ServerCmd.X = (BYTE) gObj[aRecv->iINDEX].X;
				ServerCmd.Y = (BYTE) gObj[aRecv->iINDEX].Y;
				MsgSendV2(&gObj[aRecv->iINDEX], (LPBYTE)&ServerCmd, sizeof(ServerCmd));
				DataSend(aRecv->iINDEX, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			}
		}


		if ( aRecv->iGiftNumber == 12 ) {		// 50���� 
			gObj[aRecv->iINDEX].Money += 500000;
			GCMoneySend(aRecv->iINDEX, gObj[aRecv->iINDEX].Money);
		}
		else if ( aRecv->iGiftNumber == 13 ) {	// 10���� 
			gObj[aRecv->iINDEX].Money += 100000;
			GCMoneySend(aRecv->iINDEX, gObj[aRecv->iINDEX].Money);
		}
		else if ( aRecv->iGiftNumber == 14 ) {	// 5���� 
			gObj[aRecv->iINDEX].Money += 50000;
			GCMoneySend(aRecv->iINDEX, gObj[aRecv->iINDEX].Money);
		}
		else if (aRecv->iGiftNumber == 9) {		// �༮
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 13), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
		}
		else if (aRecv->iGiftNumber == 10) {	// ����
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 14), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
		}
		else if (aRecv->iGiftNumber == 11) {	// ȥ��
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(12, 15), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
		}

	}
	else if (
		aRecv->btIsRegistered == 1 ||	// �̹� ��ϵ� �ø���
		aRecv->btIsRegistered == 2 ||	// ��� Ƚ�� �ʰ�
		aRecv->btIsRegistered == 3 ||	// ���� �ø���
		aRecv->btIsRegistered == 4 ||	// �� �� ���� ����
		aRecv->btIsRegistered == 5		// ���� ���� �ø����� ����߾���
		)
	{
		Result.btIsRegistered = aRecv->btIsRegistered;
	}
	else {
		Result.btIsRegistered = 4;
		LogAddTD("[Mu_2Anv_Event] Error : Result Value is Wrong [%d]",
			aRecv->btIsRegistered
			);
	}
	#endif // #if defined(FOR_KOREA) && !defined (COUPON_EVENT_ITEMLIST_20081022)

	Result.szGIFT_NAME[0] = 0;

	if (aRecv->btIsRegistered == 0) {					// ��ϼ���
		Result.btIsRegistered = 0;
		
#if defined(FOR_JAPAN) && defined (COUPON_EVENT_ITEMLIST_20081022)
		if (!CHECK_LIMIT(aRecv->iGiftNumber-1, MAX_ITEM_OF_COUPON_EVENT)) // ��ǰ��ȣ�� ���� �ε����� ����
#else
		if (!CHECK_LIMIT(aRecv->iGiftNumber-1, MAX_ITEM_OF_FRIENDSHIP_EVENT)) // ��ǰ��ȣ�� ���� �ε����� ����
#endif
		{	// ��ǰ��ȣ�� ���� �ε����� ����
			LogAddTD("[Mu_2Anv_Event] Error : Gift Index is out of bound [%d]",
				aRecv->iGiftNumber
				);
			Result.btIsRegistered = 2;
		}

		// ������ �Ͷ߸���.
		if (gObj[aRecv->iINDEX].Connected > 2) {
			PMSG_SERVERCMD	ServerCmd;
			PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

			ServerCmd.CmdType = 0;
			ServerCmd.X = (BYTE) gObj[aRecv->iINDEX].X;
			ServerCmd.Y = (BYTE) gObj[aRecv->iINDEX].Y;
			MsgSendV2(&gObj[aRecv->iINDEX], (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			DataSend(aRecv->iINDEX, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		}

		if (g_bRingEventItemTextLoad)
			strcpy (Result.szGIFT_NAME, g_sz2ANV_GIFT_NAME[aRecv->iGiftNumber - 1]);
// �Ϻ� ���� �̺�Ʈ ��ũ��Ʈ�� ����(20081022 - grooving)
#ifdef COUPON_EVENT_ITEMLIST_20081022
		// ������ ��ȣ �����´�.
		BYTE btItemLevel = 0;
		BYTE btItemDur = 0;
		int iItemNumber = g_CouponEventItemLIst.GetItemNum(aRecv->iGiftNumber, btItemLevel, btItemDur);
#ifdef ADD_COUPON_EVENTITEM_LOG
		nItemNumber = iItemNumber;
#endif // ADD_COUPON_EVENTITEM_LOG
		
		if( 0 == iItemNumber || -1 == iItemNumber )
		{
			LogAddTD("[Mu_2Anv_Event] Error : iGiftNumber is Out of Boud [%d]",
				aRecv->iGiftNumber
				);
		}
		else
		{
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				iItemNumber,
				btItemLevel, btItemDur, 0, 0, 0, aRecv->iINDEX);
			
			if (g_bRingEventItemTextLoad == FALSE)
			{
				strcpy (Result.szGIFT_NAME, ItemAttribute[iItemNumber].Name);
			}
		}
#else // COUPON_EVENT_ITEMLIST_20081022

		switch(aRecv->iGiftNumber) {
		case 1 :		// �ູ�Ǻ���
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 13), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14,13)].Name);
			break;
		case 2 :		// ��ȥ�Ǻ���
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 14), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 14)].Name);
			break;
		case 3 :		// ȥ���Ǻ���
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(12, 15), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(12, 15)].Name);
			break;
		case 4 :		// ����ǻ���
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 11), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 11)].Name);
			break;
		case 5 :		// �������Ʈ
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 11), 
				3, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, "Heart of Love");
			break;
		case 6 :		// 50����
			gObj[aRecv->iINDEX].Money += 500000;
			GCMoneySend(aRecv->iINDEX, gObj[aRecv->iINDEX].Money);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, "500,000 Zen");
			break;
		case 7 :		// 5����
			gObj[aRecv->iINDEX].Money += 50000;
			GCMoneySend(aRecv->iINDEX, gObj[aRecv->iINDEX].Money);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, "50,000 Zen");
			break;
		case 8 :		// 3����
		case 9 :		// 3����
		case 10 :		// 3����
			gObj[aRecv->iINDEX].Money += 30000;
			GCMoneySend(aRecv->iINDEX, gObj[aRecv->iINDEX].Money);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, "30,000 Zen");
			break;
		case 11 :		// ��ο� ����
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(12, 19), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(12, 11)].Name);
			break;
		case 12 :		// ���̽��ַο� ����
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(12, 18), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(12, 18)].Name);
			break;
		case 13 :		// ���뱸��
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(12, 17), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(12, 17)].Name);
			break;
		case 14 :		// ���� ���ñ���
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(12, 16), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(12, 16)].Name);
			break;
		case 15 :		// ���������� ����
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(12, 14), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(12, 14)].Name);
			break;
		case 16 :		// â ��� ����
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(12, 13), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(12, 13)].Name);
			break;
		case 17 :		// �г����ϰ� ����
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(12, 12), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(12, 12)].Name);
			break;
		case 18 :		// ���ϸ����� ��
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(13, 2), 
				0, (BYTE)255, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(13, 2)].Name);
			break;
		case 19 :		// ����Ʈ�� ��
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(13, 3), 
				0, (BYTE)255, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(13, 2)].Name);
			break;
		case 20 :		// ������ ����
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 16), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 16)].Name);
			break;
		case 21 :		// ��ȣõ��
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(13, 0), 
				0, (BYTE)255, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(13, 0)].Name);
			break;
		case 22 :		// ��ź
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(13, 1), 
				0, (BYTE)255, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(13, 1)].Name);
			break;
		case 23 :		// ��ź�Ǻ�
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 11), 
				1, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 11)].Name);
			break;
		case 24 :		// ����� ���� + 2
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 11), 
				9, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 11)].Name);
			break;
		case 25 :		// ����� ���� + 3
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 11), 
				10, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 11)].Name);
			break;
		case 26 :		// ���������� �ʴ�� + 0
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 19), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 19)].Name);
			break;
		case 27 :		// ���������� �ʴ�� + 1
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 19), 
				1, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 19)].Name);
			break;
		case 28 :		// ���������� �ʴ�� + 2
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 19), 
				2, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 19)].Name);
			break;
		case 29 :		// ���������� �ʴ�� + 3
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 19), 
				3, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 19)].Name);
			break;
		case 30 :		// ���������� �ʴ�� + 4
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 19), 
				4, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 19)].Name);
			break;
		case 31 :		// ����
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 11), 
				2, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 11)].Name);
			break;
		case 32 :		// ����� ����
			ItemSerialCreateSend(
				gObj[aRecv->iINDEX].m_Index, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[aRecv->iINDEX].X, 
				(BYTE)gObj[aRecv->iINDEX].Y, 
				ItemGetNumberMake(14, 20), 
				0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
			if (g_bRingEventItemTextLoad == FALSE)
				strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 20)].Name);
			break;

			#ifdef ADD_ITEM_OF_FRIENDSHIP_EVENT_20040729
			case 33 :		// â���� ����
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 22), 
					0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 22)].Name);
				break;
			case 34 :		// ���� (������)
			case 35 :		// ���� (ü��)
			case 36 :		// ���� (��ø)
			case 37 :		// ���� (��)
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(13, 15), 
					aRecv->iGiftNumber - 34, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(13, 15)].Name);
				break;
			case 38 :		// ����� ���� +4
			case 39 :		// ����� ���� +5
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 11), 
					11 + aRecv->iGiftNumber - 38, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 11)].Name);
				break;
			case 40 :		// �ִϹ����� �ڽ� +1
			case 41 :		// �ִϹ����� �ڽ� +2
			case 42 :		// �ִϹ����� �ڽ� +3
			case 43 :		// �ִϹ����� �ڽ� +4
			case 44 :		// �ִϹ����� �ڽ� +5
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 20), 
					1 + aRecv->iGiftNumber - 40, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 20)].Name);
				break;
			#endif // ADD_ITEM_OF_FRIENDSHIP_EVENT_20040729
			#ifdef MODIFY_GOLDEN_ARCHER_EVENT_ITEMLIST_20061109
			case 45:		// ����� ���� +1 (14,11,8)
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 11), 
					8, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 11)].Name);
				break;
			case 46:		// ���� ���� (14,41)
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 41), 
					0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 41)].Name);
				break;
			case 47:		// ��ȭ�� ���� (14,42)
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 42), 
					0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 42)].Name);
				break;
			case 48:		// ��� ���ü� (14,44)
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 44), 
					0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 44)].Name);
				break;
			case 49:		// �ϱ� ���ü� (14,43)
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 43), 
					0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 43)].Name);
				break;
			case 50:		// ��ȣ�� ���� (14,31)
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 31), 
					0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 31)].Name);
				break;
/*
			case 51:		// �������� ���� (13,20)
				int iRingCountinInventory;
				int iRingCountinEquip;
				iRingCountinInventory = gObjGetItemCountInIventory(gObj[aRecv->iINDEX].m_Index, 13, 20, 0);
				iRingCountinEquip = gObjGetItemCountInEquipment(gObj[aRecv->iINDEX].m_Index, 13, 20, 0);
				if( iRingCountinEquip > 0 || iRingCountinInventory )
				{
					break;
				}

				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(13, 20), 
					0, (BYTE)30, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(13, 20)].Name);
				break;
*/
			#endif // MODIFY_GOLDEN_ARCHER_EVENT_ITEMLIST_20061109
			#ifdef MODIFY_GOLDEN_ARCHER_EVENT_ITEMLIST_2_20070408
			case 51:		// ī����ī��̴� (14, 95)
				ItemSerialCreateSend(
					gObj[aRecv->iINDEX].m_Index, 
					ITEMMAKE_INVENTORY_1CELL, 
					(BYTE)gObj[aRecv->iINDEX].X, 
					(BYTE)gObj[aRecv->iINDEX].Y, 
					ItemGetNumberMake(14, 95), 
					0, (BYTE)0, 0, 0, 0, aRecv->iINDEX);
				if (g_bRingEventItemTextLoad == FALSE)
					strcpy (Result.szGIFT_NAME, ItemAttribute[MAKE_ITEMNUM(14, 95)].Name);
				break;
			#endif // MODIFY_GOLDEN_ARCHER_EVENT_ITEMLIST_2_20070408				
		default :		// ����
			LogAddTD("[Mu_2Anv_Event] Error : iGiftNumber is Out of Boud [%d]",
				aRecv->iGiftNumber
				);
			break;
		}
#endif // COUPON_EVENT_ITEMLIST_20081022
	}
	else if (
		aRecv->btIsRegistered == 1 ||	// �̹� ��ϵ� �ø���
		aRecv->btIsRegistered == 2 ||	// ��� Ƚ�� �ʰ�
		aRecv->btIsRegistered == 3 ||	// ���� �ø���
		aRecv->btIsRegistered == 4 ||	// �� �� ���� ����
		aRecv->btIsRegistered == 5		// ���� ���� �ø����� ����߾���
		)
	{
		Result.btIsRegistered = aRecv->btIsRegistered;
	}
	else {
		Result.btIsRegistered = 4;
		LogAddTD("[Mu_2Anv_Event] Error : Result Value is Wrong [%d]",
			aRecv->btIsRegistered
			);
	}

#ifdef ADD_COUPON_EVENTITEM_LOG
	LogAddTD("[Mu_2Anv_Event] Register Serial Result : %d [%s][%s] GiftNumber: %d Item : %d",
		aRecv->btIsRegistered,
		gObj[aRecv->iINDEX].AccountID,
		gObj[aRecv->iINDEX].Name, 
		aRecv->iGiftNumber, 
		nItemNumber
		);
#else
	LogAddTD("[Mu_2Anv_Event] Register Serial Result : %d [%s][%s]",
		aRecv->btIsRegistered,
		gObj[aRecv->iINDEX].AccountID,
		gObj[aRecv->iINDEX].Name
		);
#endif // ADD_COUPON_EVENTITEM_LOG
	
	DataSend(aRecv->iINDEX, (LPBYTE)&Result, Result.h.size);
	gObj[aRecv->iINDEX].UseEventServer = FALSE;
}
#endif // FOR_KOREA || EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO

#ifdef MONSTER_HERD_SYSTEM_20031120

const CHAR g_szRingEventOfflineGift[4][32] = {
	"100�� �¹���",
	"10�� �¹���",
	"5�� �¹���",
	"2�� �¹���",
};


// 0x10 �̺�Ʈ ������ ���� �����̺�Ʈ ��ǰ��� ���θ� �޴´�.
void EGRecvRegRingGift(LPPMSG_ANS_REG_RINGGIFT aRecv)
{
	gObj[aRecv->iINDEX].UseEventServer = FALSE;

	if (gObjIsConnected(aRecv->iINDEX) == FALSE)
		return;

	if (strcmp(aRecv->szUID, gObj[aRecv->iINDEX].AccountID))
		return;

	if (aRecv->btIsRegistered == 1) {
		if (CHECK_LIMIT(aRecv->btGiftKind-1, 4)) {
			// ���������� ��÷��
			CHAR szTemp[256];
			wsprintf(szTemp, "%s�� ���� %s�� ��÷�Ǽ̽��ϴ�.", gObj[aRecv->iINDEX].Name, g_szRingEventOfflineGift[aRecv->btGiftKind-1]);
			AllSendServerMsg (szTemp);

			LogAddTD("[Ring Event] [%s][%s] Register Succeeded Result:%d, Gift:%d",
				gObj[aRecv->iINDEX].AccountID,
				gObj[aRecv->iINDEX].Name,
				aRecv->btIsRegistered,
				aRecv->btGiftKind
				);
		}
		else {
			LogAddTD("[Ring Event] [%s][%s] Register Failed Result:%d, Gift:%d (out of bound, 1~4)",
				gObj[aRecv->iINDEX].AccountID,
				gObj[aRecv->iINDEX].Name,
				aRecv->btIsRegistered,
				aRecv->btGiftKind
				);
		}
		return;
	}
	
#if TESTSERVER == 1
	else if (aRecv->btIsRegistered == 2) {
		CHAR szTemp[256];
		wsprintf(szTemp, "%s�� ���� ��ǰ�� �̹� ��÷�Ǽ̽��ϴ�.", gObj[aRecv->iINDEX].Name);
		AllSendServerMsg (szTemp);
	}
	else if (aRecv->btIsRegistered == 3) {
		CHAR szTemp[256];
		wsprintf(szTemp, "%s��. ��ǰ��� Ƚ�� �ʰ�", gObj[aRecv->iINDEX].Name);
		AllSendServerMsg (szTemp);
	}
	else if (aRecv->btIsRegistered == 0) {
		CHAR szTemp[256];
		wsprintf(szTemp, "%s���� ��ǰ��Ͽ� �����Ͽ����ϴ�.", gObj[aRecv->iINDEX].Name);
		AllSendServerMsg (szTemp);
	}
#endif // TESTSERVER == 1

	LogAddTD("[Ring Event] [%s][%s] Register Failed Result : %d",
		gObj[aRecv->iINDEX].AccountID,
		gObj[aRecv->iINDEX].Name,
		aRecv->btIsRegistered
		);

	// ��ǰ��� ���� �� ���� 100000�� ������. -> �� ���
	if( gObjIsConnected(aRecv->iINDEX) == TRUE )
		MapC[gObj[aRecv->iINDEX].MapNumber].MoneyItemDrop((int)100000, (BYTE) gObj[aRecv->iINDEX].X ,(BYTE) gObj[aRecv->iINDEX].Y);

}

#endif // MONSTER_HERD_SYSTEM_20031120


#ifdef EVENT_STONE_OF_FIENDSHIP_20040206
// 0x11 �̺�Ʈ ������ ���� �����ǵ� ������ �޴´�. (��ģ�� �̺�Ʈ)
void EGRecvFriendShipStoneInfo(LPPMSG_ANS_VIEW_STONES aRecv)
{
	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iINDEX];	

	PMSG_EVENTCHIPINFO	Result;
	PHeadSetB((LPBYTE)&Result, 0x94, sizeof(Result));

	#ifdef FRIENDSHIP_STONE_EXCHANGE_20040206
	if (aRecv->bSUCCESS)
		lpObj->iStoneCount = aRecv->iStoneCount;
	else
		lpObj->iStoneCount = 0;

	lpObj->MutoNumber = 0;
	Result.Type = 5;	// �����ǵ� ȯ�� ������
	
	#else // else FRIENDSHIP_STONE_EXCHANGE_20040206
	if (aRecv->bSUCCESS)
		lpObj->EventChipCount = aRecv->iStoneCount;
	else
		lpObj->EventChipCount = 0;

	lpObj->MutoNumber = 0;

	Result.Type = 4;	// �����ǵ� ������
	#endif // FRIENDSHIP_STONE_EXCHANGE_20040206

	Result.ChipCount = aRecv->iStoneCount;

	DataSend(lpObj->m_Index, (LPBYTE)&Result, Result.h.size);

	char msg [128];
	wsprintf(msg, lMsg.Get(1214), Result.ChipCount);		// "����� ����� �����ǵ� ������ ���� %d�� �Դϴ�."
	ChatTargetSend(pEventObj, msg, lpObj->m_Index);
}

// 0x12 �̺�Ʈ ������ ���� �����ǵ��� ����� ����� �޴´�. (��ģ�� �̺�Ʈ)
void	EGRecvRegFriendShipStone(LPPMSG_ANS_REGISTER_STONES aRecv)
{
	PMSG_REGEVENTCHIP_RESULT Result;
	PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));

	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iINDEX];	
	int aIndex = aRecv->iINDEX;

	if(aRecv->bSUCCESS) {
		Result.ChipCount = aRecv->iStoneCount;
		gObjInventoryDeleteItem(aIndex, aRecv->iPosition);
		GCInventoryItemDeleteSend(aIndex, aRecv->iPosition, 1);
		LogAddTD("[FriendShip Stone] [%s][%s] Delete FriendShip Stones", lpObj->AccountID, lpObj->Name);
	}
	else {
		Result.ChipCount = -1;
		LogAddTD("[FriendShip Stone] [%s][%s] RegEventServer Fail (FriendShip Stones : %d)", lpObj->AccountID, lpObj->Name, aRecv->iStoneCount);
	}

	DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
	lpObj->UseEventServer = FALSE;
}

// 0x13 �̺�Ʈ ������ ���� �����ǵ��� ������ ����� �޴´�. (��ģ�� �̺�Ʈ)
void EGRecvDeleteFriendShipStone(LPPMSG_ANS_DELETE_STONES aRecv)
{
	

}

#endif // EVENT_STONE_OF_FIENDSHIP_20040206

#endif // FOR_BLOODCASTLE

extern wsJoinServerCli	wsRServerCli;			//  ��ŷ����

#ifdef BLOODCASTLE_CHECK_ENTERCOUNT_20040304
// ����ĳ�� �Ϸ� ����Ƚ�� ������ ó���Ѵ�.
#include "BloodCastle.h"
extern CBloodCastle g_BloodCastle;

void EGBloodCastleEnterCountCheck(LPPMSG_ANS_BLOODCASTLEENTERCHECK lpMsg);
void EGBloodCastleEnterCountOn(INT iIndex);


void	EGBloodCastleEnterCountCheck(LPPMSG_ANS_BLOODCASTLEENTERCHECK lpMsg)
{
	INT iIndex = lpMsg->iObjIndex;

	if (gObjIsConnected(iIndex) == FALSE) {
		// �������� ������ �޾Ҵµ� ������ ���� �����ߴ�.
		return;
	}

	if (iIndex < ALLOC_USEROBJECTSTART) {
		// �� �ε��� ���ϴ� ����� �ƴϹǷ� ������.
		return;
	}

	BYTE btBridgeNumber = lpMsg->iBridgeNumber-1;
	BYTE btInvisibleCourtItemPos = lpMsg->iItemPos;
	INT	 iITEM_LEVEL = 0;
	DWORD dwITEM_SERIAL = 0;

	PMSG_RESULT_MOVEBLOODCASTLE pResult;
	PHeadSetB((LPBYTE)&pResult, 0x9A, sizeof(pResult));
	pResult.Result = 0;

	
#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
	// *>. ī�� ������ ������ �� ����.
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

	// *> . ���� ���� Ƚ���� �ʰ��Ͽ���.
	if (lpMsg->bEnterCheck == 0) {
		pResult.Result = 6;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	
	// P.S.> . �� �Ʒ����� ������ ��� ĳ��������� ������ -1 ���־�� �Ѵ�.

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
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ������� ����üũ ����
			if (!CHECK_LIMIT(iITEM_LEVEL, MAX_BLOODCASTLE_BRIDGE_COUNT+1)) {		// 0 < iITEM_LEVEL < 8
#else
			if (!CHECK_LIMIT(iITEM_LEVEL, 8)) {		// 0 < iITEM_LEVEL < 8
#endif
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
			iITEM_LEVEL = 10;

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

#ifdef MODIFY_NOTICE_20040325
		TNotice::MakeNoticeMsg( &pNotice, 1, "���빫�⸦ �����ϰ� �����Ƿ� ����ĳ���� �����Ͻ� �� �����ϴ�.");
		TNotice::SendNoticeToUser( iIndex, &pNotice )
#else
		PMSG_NOTICE	pNotice;
		pNotice.type = 1;
		strcpy(pNotice.Notice, "���빫�⸦ �����ϰ� �����Ƿ� ����ĳ���� �����Ͻ� �� �����ϴ�.");
		PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
#endif
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
	if( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 18 )
		|| ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 47 ) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability == 1 )
		)
	{
		gObjInventoryDeleteItem(iIndex, btInvisibleCourtItemPos);
		GCInventoryItemDeleteSend(iIndex, btInvisibleCourtItemPos, 1);
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	// ���� ĳ�� ���� �����
	else if( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == MAKE_ITEMNUM( 13, 47 )
		&& gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 1 )
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

	// *> . ����ĳ���� �̵��ϱ� ���� DB�� �̵�ī��Ʈ ������Ŵ
	EGBloodCastleEnterCountOn (iIndex);

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
	#else // else USER_ENTER_BLOODCASTLE7_WITH6_20040330
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
	#endif // USER_ENTER_BLOODCASTLE7_WITH6_20040330
#else // else BLOODCASTLE_EXTEND_20040314
	gObjMoveGate(iIndex, GATE_BLOODCASTLE_1 + iITEM_LEVEL - 1);	// �����δ� �ش� �ٸ��� ���Ƿ� �̵�
#endif // BLOODCASTLE_EXTEND_20040314

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


void EGBloodCastleEnterCountOn(INT iIndex)
{
	if (gObjIsConnected(iIndex) == FALSE) {
		// �������� ������ �޾Ҵµ� ������ ���� �����ߴ�.
		return;
	}

	if (!DevilSquareEventConnect) {
		if (IsDevilSquareEventConnected) {
			// ��ŷ������ ���� ī��Ʈ�� ������Ų��
			PMSG_REQ_BLOODCASTLECOUNTON pMsg;
			pMsg.h.c = 0xC1;
			pMsg.h.headcode = 0x09;
			pMsg.h.size = sizeof(pMsg);
			memcpy (pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
			memcpy (pMsg.GameID, gObj[iIndex].Name, MAX_IDSTRING);
			pMsg.ServerCode = gGameServerCode;

			wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
		}
		else {
			BOOL bRETVAL = GMRankingServerConnect(gDevilSquareEventServerIp, WM_USER+12);	// WM_ASS_RANKINGCLIMSG
			if (bRETVAL == TRUE) {
				// ��ŷ������ ���� ī��Ʈ�� ������Ų��
				PMSG_REQ_BLOODCASTLECOUNTON pMsg;
				pMsg.h.c = 0xC1;
				pMsg.h.headcode = 0x09;
				pMsg.h.size = sizeof(pMsg);
				memcpy (pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
				memcpy (pMsg.GameID, gObj[iIndex].Name, MAX_IDSTRING);
				pMsg.ServerCode = gGameServerCode;

				wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
			}
		}
	}
}

#endif // BLOODCASTLE_CHECK_ENTERCOUNT_20040304


#ifdef BLOODCASTLE_EVENT_3RD_20040401

// [0x0B] ���� ĳ���� ���� �Ϸ� ����Ƚ���� ��û�Ѵ�.
void EGReqBloodCastleEnterCount(INT iIndex);
// [0x0B] ���� ĳ���� ���� �Ϸ� ����Ƚ���� �޴´�.
void EGAnsBloodCastleEnterCount(LPPMSG_ANS_BLOODCASTLE_ENTERCOUNT lpMsg);


void EGReqBloodCastleEnterCount(INT iIndex)
{
	if (!gObjIsConnected(iIndex)) {
		return;
	}

	PMSG_REQ_BLOODCASTLE_ENTERCOUNT pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x0B;
	pMsg.h.size = sizeof(pMsg);

	memcpy (pMsg.AccountID, gObj[iIndex].AccountID, sizeof(pMsg.AccountID));
	memcpy (pMsg.GameID, gObj[iIndex].Name, sizeof(pMsg.GameID));
	pMsg.ServerCode = gGameServerCode / 20;
	pMsg.iObjIndex = iIndex;


	// ������ ��ŷ������ �����ϴ� �κ�
	if( !IsDevilSquareEventConnected && !DevilSquareEventConnect )
	{
		wsRServerCli.Close();
		wsRServerCli.CreateSocket(ghWnd);
		if( GMRankingServerConnect(gDevilSquareEventServerIp, WM_ASS_RANKINGCLIMSG) == FALSE )
		{
			IsDevilSquareEventConnected = 0;
			LogAddTD("Can not connect Ranking Server");
			return;
		}
		IsDevilSquareEventConnected = 1;
	}

	if( !DevilSquareEventConnect && IsDevilSquareEventConnected )
		wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}


void EGAnsBloodCastleEnterCount(LPPMSG_ANS_BLOODCASTLE_ENTERCOUNT lpMsg)
{
	if (lpMsg == NULL) {
		return;
	}

	if (!gObjIsConnected(lpMsg->iObjIndex)) {
		return;
	}

	CHAR szAccountID[MAX_IDSTRING+1] = {0,};
	CHAR szName[MAX_IDSTRING+1] = {0,};
	memcpy (szAccountID, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	memcpy (szName, lpMsg->GameID, sizeof(lpMsg->GameID));

	if (strcmp(gObj[lpMsg->iObjIndex].AccountID, szAccountID) ||
		strcmp(gObj[lpMsg->iObjIndex].Name, szName)
		)
	{
		// ���� ����ĳ�� ���� Ƚ���� ��û�ߴ� �ɸ��� �ƴ϶�� ������ ����
		return;
	}

	// Ŭ���̾�Ʈ���� ���� Ƚ���� �����ش�.
	PMSG_ANS_CL_ENTERCOUNT pMsgSend;
	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0x9F;
	pMsgSend.h.size = sizeof(pMsgSend);
	pMsgSend.btEventType = 2;	// ����ĳ��
	pMsgSend.btLeftEnterCount = lpMsg->iLeftCount;

	DataSend (lpMsg->iObjIndex, (LPBYTE) &pMsgSend, sizeof(pMsgSend));
}


#endif // BLOODCASTLE_EVENT_3RD_20040401


#ifdef DEVILSQUARE_CHECK_ENTERCOUNT_20040715
// ���������� �Ϸ� ����Ƚ�� ������ ó���Ѵ�.
#include "DevilSquare.h"
extern CDevilSquare g_DevilSquare;

//////////////////////////////////////////////////////////////////
//	DECLARATION
//////////////////////////////////////////////////////////////////

// [0x0F] ������������ ���� �Ϸ� ����Ƚ���� ��û�Ѵ�.
BOOL EGReqDevilSquareEnterCount			(INT iIndex);
// [0x0F] ������������ ���� �Ϸ� ����Ƚ���� �޴´�.
VOID EGAnsDevilSquareEnterCount			(LPPMSG_ANS_DEVILSQUARE_ENTERCOUNT lpMsg);

// [0x10] ��������� ���尡�� ���θ� ��û�Ѵ�.
BOOL EGReqDevilSquareEnterCountCheck	(INT iIndex, INT iItemPos, INT iItemLevel, INT iSquareNumber);
// [0x10] ��������� ���尡�� ���ο� ���� ����� �޴´�.
VOID EGAnsDevilSquareEnterCountCheck	(LPPMSG_ANS_DEVILSQUAREENTERCHECK lpMsg);

// [0x11] ������������ ����Ƚ���� �÷��޶�� ��û�Ѵ�.
BOOL EGReqBloodCastleEnterCountOn		(INT iIndex);


//////////////////////////////////////////////////////////////////
//	DEFINITION
//////////////////////////////////////////////////////////////////

// [0x0F] ������������ ���� �Ϸ� ����Ƚ���� ��û�Ѵ�.
BOOL EGReqDevilSquareEnterCount			(INT iIndex)
{
	BOOL bRET_VAL = FALSE;

	if (!DevilSquareEventConnect) {
		if (IsDevilSquareEventConnected) {
			PMSG_REQ_DEVILSQUARE_ENTERCOUNT pMsg;
			pMsg.h.c = 0xC1;
			pMsg.h.headcode = 0x0F;
			pMsg.h.size = sizeof(pMsg);

			memcpy (pMsg.AccountID, gObj[iIndex].AccountID, sizeof(pMsg.AccountID));
			memcpy (pMsg.GameID, gObj[iIndex].Name, sizeof(pMsg.GameID));
			pMsg.ServerCode = gGameServerCode / 20;
			pMsg.iObjIndex = iIndex;

			if (wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size))
				bRET_VAL = TRUE;
			else
				bRET_VAL = FALSE;
		}
		else {
			wsRServerCli.Close();
			wsRServerCli.CreateSocket(ghWnd);
			bRET_VAL = GMRankingServerConnect(gDevilSquareEventServerIp, WM_USER+12);	// WM_ASS_RANKINGCLIMSG
			if (bRET_VAL == TRUE) {
				PMSG_REQ_DEVILSQUARE_ENTERCOUNT pMsg;
				pMsg.h.c = 0xC1;
				pMsg.h.headcode = 0x0F;
				pMsg.h.size = sizeof(pMsg);

				memcpy (pMsg.AccountID, gObj[iIndex].AccountID, sizeof(pMsg.AccountID));
				memcpy (pMsg.GameID, gObj[iIndex].Name, sizeof(pMsg.GameID));
				pMsg.ServerCode = gGameServerCode / 20;
				pMsg.iObjIndex = iIndex;

				if (wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size))
					bRET_VAL = TRUE;
				else
					bRET_VAL = FALSE;
			}
		}
	}
	
	return bRET_VAL;
}


// [0x0F] ������������ ���� �Ϸ� ����Ƚ���� �޴´�.
VOID EGAnsDevilSquareEnterCount			(LPPMSG_ANS_DEVILSQUARE_ENTERCOUNT lpMsg)
{
	if (gObjIsConnected(lpMsg->iObjIndex) == FALSE) {
		// ����Ƚ���� �޾Ҵµ� ������ �����.
		return;
	}
	
	CHAR szAccountID[MAX_IDSTRING+1] = {0,};
	CHAR szName[MAX_IDSTRING+1] = {0,};
	memcpy (szAccountID, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	memcpy (szName, lpMsg->GameID, sizeof(lpMsg->GameID));

	if (strcmp(gObj[lpMsg->iObjIndex].AccountID, szAccountID) ||
		strcmp(gObj[lpMsg->iObjIndex].Name, szName)
		)
	{
		// ���� ���������� ���� Ƚ���� ��û�ߴ� �ɸ��� �ƴ϶�� ������ ����
		return;
	}

	// Ŭ���̾�Ʈ���� ���� Ƚ���� �����ش�.
	PMSG_ANS_CL_ENTERCOUNT pMsgSend;
	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0x9F;
	pMsgSend.h.size = sizeof(pMsgSend);
	pMsgSend.btEventType = 1;	// ����������					
	pMsgSend.btLeftEnterCount = lpMsg->iLeftCount;

	DataSend (lpMsg->iObjIndex, (LPBYTE) &pMsgSend, sizeof(pMsgSend));
}


// [0x10] ��������� ���尡�� ���θ� ��û�Ѵ�.
BOOL EGReqDevilSquareEnterCountCheck	(INT iIndex, INT iItemPos, INT iItemLevel, INT iSquareNumber)
{
	BOOL bRET_VAL = FALSE;

	if (!DevilSquareEventConnect) {
		if (IsDevilSquareEventConnected) {
			PMSG_REQ_DEVILSQUAREENTERCHECK pMsg;
			pMsg.h.c			= 0xC1;
			pMsg.h.headcode		= 0x10;
			pMsg.h.size			= sizeof(pMsg);
			memcpy (pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
			memcpy (pMsg.GameID, gObj[iIndex].Name, MAX_IDSTRING);
			pMsg.ServerCode		= gGameServerCode;
			pMsg.iObjIndex		= iIndex;
			pMsg.iItemPos		= iItemPos;
			pMsg.iItemLevel		= iItemLevel;
			pMsg.iSquareNumber	= iSquareNumber;

			if (wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size))
				bRET_VAL = TRUE;
			else
				bRET_VAL = FALSE;
		}
		else {
			wsRServerCli.Close();
			wsRServerCli.CreateSocket(ghWnd);
			bRET_VAL = GMRankingServerConnect(gDevilSquareEventServerIp, WM_USER+12);	// WM_ASS_RANKINGCLIMSG
			if (bRET_VAL == TRUE) {
				PMSG_REQ_DEVILSQUAREENTERCHECK pMsg;
				pMsg.h.c			= 0xC1;
				pMsg.h.headcode		= 0x10;
				pMsg.h.size			= sizeof(pMsg);
				memcpy (pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
				memcpy (pMsg.GameID, gObj[iIndex].Name, MAX_IDSTRING);
				pMsg.ServerCode		= gGameServerCode;
				pMsg.iObjIndex		= iIndex;
				pMsg.iItemPos		= iItemPos;
				pMsg.iItemLevel		= iItemLevel;
				pMsg.iSquareNumber	= iSquareNumber;

				if (wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size))
					bRET_VAL = TRUE;
				else
					bRET_VAL = FALSE;
			}
		}
	}
	
	return bRET_VAL;
}


// [0x10] ��������� ���尡�� ���ο� ���� ����� �޴´�.
VOID EGAnsDevilSquareEnterCountCheck	(LPPMSG_ANS_DEVILSQUAREENTERCHECK lpMsg)
{
	if (gObjIsConnected(lpMsg->iObjIndex) == FALSE) {
		// ���尡�� ���ο� ���� ����� �޾Ҵµ� ������ �����.
		return;
	}

	CHAR szAccountID[MAX_IDSTRING+1] = {0,};
	CHAR szName[MAX_IDSTRING+1] = {0,};
	memcpy (szAccountID, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	memcpy (szName, lpMsg->GameID, sizeof(lpMsg->GameID));

	if (strcmp(gObj[lpMsg->iObjIndex].AccountID, szAccountID) ||
		strcmp(gObj[lpMsg->iObjIndex].Name, szName)
		)
	{
		// ���� ���������� ���� Ƚ���� ��û�ߴ� �ɸ��� �ƴ϶�� ������ ����
		return;
	}

	LPOBJECTSTRUCT lpObj = &gObj[lpMsg->iObjIndex];

	BYTE cSquareNumber = lpMsg->iSquareNumber;
	BYTE cInvitationItemPos = lpMsg->iItemPos;

	PMSG_RESULT_MOVEDEVILSQUARE pResult;
	PHeadSetB((LPBYTE)&pResult, 0x90, sizeof(pResult));
	pResult.Result = 0;

	if (lpMsg->bEnterCheck == FALSE) {
		// ������ �ִ� ���� Ƚ���� �Ѿ���.
		pResult.Result = 5;						// !!! . �ӽ� -> ����ڵ� �߰��ؾ� ��
		DataSend(lpMsg->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail (Enter Count Over) [%d][%d][%d]",
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}
	
	if( g_DevilSquare.m_DevilSquareGround[cSquareNumber].GetObjCount() >= MAX_DEVILSQUARE_OBJCOUNT )
	{	// ���� ���� �ο����� �Ѿ���
		pResult.Result = 5;
		DataSend(lpMsg->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}
	
	CItem* sitem = &lpObj->pInventory[cInvitationItemPos];		

	if( sitem->m_Type != MAKE_ITEMNUM(14, 19)
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		// ���� ���� ������� �ִ���, �׸��� ���� ���� Ƚ���� ���� �ִ��� Ȯ���Ѵ�.
		&& ( sitem->m_Type != MAKE_ITEMNUM( 13, 46 ) || sitem->m_Durability <= 0 )
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		)
	{	// �������� ����
		pResult.Result = 1;
		DataSend(lpMsg->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	int	ItemLevel = sitem->m_Level;

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
		if( ItemLevel != cSquareNumber+1 )
		{	// �ʴ���� ������ �������� ���� ��ȣ�� �� �ؾ� �ȴ�
			pResult.Result = 1;
			DataSend(lpMsg->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
			LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Invitation LevelError [%d][%d][%d][%d]", 
				   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1, ItemLevel);
			return;
		}
	}

	BOOL bEnterCheck = 0;	// 0�̸� ���尡�� 1�̸� �ڽ��� �������� ���� ���� ������ �Ҷ� 
							// 2�� �ڽ��� �������� ���� ���� ������ �Ҷ�

#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
	INT movegateindex = 61;
#else
	BYTE movegateindex = 61;
#endif

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
	
	if( bEnterCheck == 1)
	{
		pResult.Result = 3;
		DataSend(lpMsg->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);
		return;
	}
	else if( bEnterCheck == 2)
	{
		pResult.Result = 4;
		DataSend(lpMsg->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
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
		// ���� ���� ������� ������ 1�̸� ���� �����ش�.
		|| ( sitem->m_Type == MAKE_ITEMNUM( 13, 46 ) && sitem->m_Durability == 1 )
	  )
	{
		gObjInventoryDeleteItem(lpMsg->iObjIndex, cInvitationItemPos);
		GCInventoryItemDeleteSend(lpMsg->iObjIndex, cInvitationItemPos, 1);
		DataSend(lpMsg->iObjIndex, (LPBYTE)&pResult, pResult.h.size);	
	}
	else if( sitem->m_Type == MAKE_ITEMNUM( 13, 46 ) && sitem->m_Durability > 1 )
	{
		sitem->m_Durability = sitem->m_Durability - 1;

		// ���ҵ� ������� �������� �����ش�.
		GCItemDurSend2( lpObj->m_Index, lpMsg->iItemPos, sitem->m_Durability, 0 );
	}
#else
	gObjInventoryDeleteItem(lpMsg->iObjIndex, cInvitationItemPos);
	GCInventoryItemDeleteSend(lpMsg->iObjIndex, cInvitationItemPos, 1);
	DataSend(lpMsg->iObjIndex, (LPBYTE)&pResult, pResult.h.size);	
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

	// ���⼭ ���� ī��Ʈ�� �÷��ش�.
	EGReqBloodCastleEnterCountOn (lpObj->m_Index);
}


// [0x11] ������������ ����Ƚ���� �÷��޶�� ��û�Ѵ�.
BOOL EGReqBloodCastleEnterCountOn		(INT iIndex)
{
	BOOL bRET_VAL = FALSE;

	if (!DevilSquareEventConnect) {
		if (IsDevilSquareEventConnected) {
			// ��ŷ������ ���� ī��Ʈ�� ������Ų��
			PMSG_REQ_BLOODCASTLECOUNTON pMsg;
			pMsg.h.c = 0xC1;
			pMsg.h.headcode = 0x11;
			pMsg.h.size = sizeof(pMsg);
			memcpy (pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
			memcpy (pMsg.GameID, gObj[iIndex].Name, MAX_IDSTRING);
			pMsg.ServerCode = gGameServerCode;

			if (wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size))
				bRET_VAL = TRUE;
			else
				bRET_VAL = FALSE;
		}
		else {
			wsRServerCli.Close();
			wsRServerCli.CreateSocket(ghWnd);
			bRET_VAL = GMRankingServerConnect(gDevilSquareEventServerIp, WM_USER+12);	// WM_ASS_RANKINGCLIMSG
			if (bRET_VAL == TRUE) {
				PMSG_REQ_BLOODCASTLECOUNTON pMsg;
				pMsg.h.c = 0xC1;
				pMsg.h.headcode = 0x11;
				pMsg.h.size = sizeof(pMsg);
				memcpy (pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
				memcpy (pMsg.GameID, gObj[iIndex].Name, MAX_IDSTRING);
				pMsg.ServerCode = gGameServerCode;

				if (wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size))
					bRET_VAL = TRUE;
				else
					bRET_VAL = FALSE;
			}
		}
	}
	
	return bRET_VAL;
}


#endif // DEVILSQUARE_CHECK_ENTERCOUNT_20040715


#ifdef CHAOSCASTLE_SYSTEM_20040408			// ī���� ĳ�� �������� ��ǰ�� ���õ� �Լ���


VOID EGReqRegCCOfflineGift (INT iIndex)
{
	if (!gObjIsConnected(iIndex)) {
		return;
	}

	PMSG_REQ_REG_CC_OFFLINE_GIFT pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x15;
	pMsg.h.size = sizeof(pMsg);

	memcpy (pMsg.szUID, gObj[iIndex].AccountID, sizeof(pMsg.szUID));
	memcpy (pMsg.szNAME, gObj[iIndex].Name, sizeof(pMsg.szNAME));
	pMsg.wServerCode = gGameServerCode / 20;
	pMsg.iINDEX = iIndex;

	pMsg.szUID[sizeof(pMsg.szUID)-1]		= 0;
	pMsg.szNAME[sizeof(pMsg.szNAME)-1]		= 0;

	DataSendEventChip((char *) &pMsg, sizeof(pMsg));
}


VOID EGAnsRegCCOfflineGift (LPPMSG_ANS_REG_CC_OFFLINE_GIFT lpMsg)
{
	if (lpMsg == NULL) {
		return;
	}

	if (lpMsg->iResultCode == 0) {					// ���� ��Ͽ� �����Ͽ����� �׳� ������.
		return;
	}

	CHAR szAccountID[MAX_IDSTRING+1] = {0,};
	CHAR szName[MAX_IDSTRING+1] = {0,};
	CHAR szGIFT_NAME[50] = {0,};
	
	memset (szAccountID, 0, sizeof(szAccountID));
	memset (szName, 0, sizeof(szName));
	memset (szGIFT_NAME, 0, sizeof(szGIFT_NAME));

	memcpy (szAccountID, lpMsg->szUID, sizeof(szAccountID));
	memcpy (szName, lpMsg->szNAME, sizeof(szName));
	memcpy (szGIFT_NAME, lpMsg->szGIFT_NAME, sizeof(szGIFT_NAME));

	szAccountID[sizeof(szAccountID)-1]	= 0;
	szName[sizeof(szName)-1]			= 0;
	szGIFT_NAME[sizeof(szGIFT_NAME)-1]	= 0;

	// ������ ��� ����ڵ鿡�� ������ �����ش�.
	CHAR szText[256] = {0,};
	wsprintf(szText, lMsg.Get(1610),				// "%s �Բ��� %s ��ǰ�� ��÷�Ǽ̽��ϴ�."
		szName,
		szGIFT_NAME
		);

	AllSendServerMsg(szText);

	LogAddTD("[Chaos Castle] [%s][%s] Success to Register OffLine Gift (GIFT:%s)",
		szAccountID,
		szName,
		szGIFT_NAME
		);
}

#endif


#ifdef DARK_LORD_HEART_EVENT_20040810

// [0x16] ��ũ�ε��� ���� �������� ��ǰ ����� ��û�Ѵ�.
VOID EGReqRegDLOfflineGift (INT iIndex)
{
	if (!gObjIsConnected(iIndex)) {
		return;
	}

	PMSG_REQ_REG_DL_OFFLINE_GIFT pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x16;
	pMsg.h.size = sizeof(pMsg);

	memcpy (pMsg.szUID, gObj[iIndex].AccountID, sizeof(pMsg.szUID));
	memcpy (pMsg.szNAME, gObj[iIndex].Name, sizeof(pMsg.szNAME));
	pMsg.wServerCode = gGameServerCode / 20;
	pMsg.iINDEX = iIndex;

	pMsg.szUID[sizeof(pMsg.szUID)-1]		= 0;
	pMsg.szNAME[sizeof(pMsg.szNAME)-1]		= 0;

	DataSendEventChip((char *) &pMsg, sizeof(pMsg));	
}


// [0x16] ��ũ�ε��� ���� �������� ��ǰ ��� ��û�� ���� ������ �޴´�.
VOID EGAnsRegDLOfflineGift (LPPMSG_ANS_REG_DL_OFFLINE_GIFT lpMsg)
{
	if (lpMsg == NULL) {
		return;
	}

	if (lpMsg->iResultCode == 0) {		// ���� ��Ͽ� �����Ͽ����� �׳� ������.
		return;
	}

	CHAR szAccountID[MAX_IDSTRING+1] = {0,};
	CHAR szName[MAX_IDSTRING+1] = {0,};
	CHAR szGIFT_NAME[50] = {0,};
	
	memset (szAccountID, 0, sizeof(szAccountID));
	memset (szName, 0, sizeof(szName));
	memset (szGIFT_NAME, 0, sizeof(szGIFT_NAME));

	memcpy (szAccountID, lpMsg->szUID, sizeof(szAccountID));
	memcpy (szName, lpMsg->szNAME, sizeof(szName));
	memcpy (szGIFT_NAME, lpMsg->szGIFT_NAME, sizeof(szGIFT_NAME));

	szAccountID[sizeof(szAccountID)-1]	= 0;
	szName[sizeof(szName)-1]			= 0;
	szGIFT_NAME[sizeof(szGIFT_NAME)-1]	= 0;

	// ������ ��� ����ڵ鿡�� ������ �����ش�.
	CHAR szText[256] = {0,};
	wsprintf(szText, "[��ũ�ε� ��� �̺�Ʈ] %s �Բ��� %s ��ǰ�� ��÷�Ǽ̽��ϴ�.",
		szName,
		szGIFT_NAME
		);

	AllSendServerMsg(szText);

	LogAddTD("[DarkLord Heart Event] [%s][%s] Success to Register OffLine Gift (GIFT:%s)",
		szAccountID,
		szName,
		szGIFT_NAME
		);
}


#endif


#ifdef HIDDEN_TREASUREBOX_EVENT_20050804

// [0x17] ������ �������� �̺�Ʈ ��ǰ ����� ��û�Ѵ�.
VOID EGReqRegHTOfflineGift (INT iIndex)
{
	if (!gObjIsConnected(iIndex)) {
		return;
	}

	PMSG_REQ_REG_HT_OFFLINE_GIFT pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x17;
	pMsg.h.size = sizeof(pMsg);

	memcpy (pMsg.szUID, gObj[iIndex].AccountID, sizeof(pMsg.szUID));
	memcpy (pMsg.szNAME, gObj[iIndex].Name, sizeof(pMsg.szNAME));
	pMsg.wServerCode = gGameServerCode / 20;
	pMsg.iINDEX = iIndex;

	pMsg.szUID[sizeof(pMsg.szUID)-1]		= 0;
	pMsg.szNAME[sizeof(pMsg.szNAME)-1]		= 0;

	DataSendEventChip((char *) &pMsg, sizeof(pMsg));	
}


// [0x17] ������ �������� �̺�Ʈ ��ǰ ��� ��û�� ���� ������ �޴´�.
VOID EGAnsRegHTOfflineGift (LPPMSG_ANS_REG_HT_OFFLINE_GIFT lpMsg)
{
	if (lpMsg == NULL) {
		return;
	}

	if (lpMsg->iResultCode == 0) {		// ���� ��Ͽ� �����Ͽ����� �׳� ������.
		// ������ ����
		if (gObjIsConnected(lpMsg->iINDEX)) {
			LPOBJECTSTRUCT lpObj = &gObj[lpMsg->iINDEX];
			MapC[lpObj->MapNumber].MoneyItemDrop(1000000, lpObj->X, lpObj->Y);
		}
		return;
	}

	CHAR szAccountID[MAX_IDSTRING+1] = {0,};
	CHAR szName[MAX_IDSTRING+1] = {0,};
	CHAR szGIFT_NAME[50] = {0,};
	
	memset (szAccountID, 0, sizeof(szAccountID));
	memset (szName, 0, sizeof(szName));
	memset (szGIFT_NAME, 0, sizeof(szGIFT_NAME));

	memcpy (szAccountID, lpMsg->szUID, sizeof(szAccountID));
	memcpy (szName, lpMsg->szNAME, sizeof(szName));
	memcpy (szGIFT_NAME, lpMsg->szGIFT_NAME, sizeof(szGIFT_NAME));

	szAccountID[sizeof(szAccountID)-1]	= 0;
	szName[sizeof(szName)-1]			= 0;
	szGIFT_NAME[sizeof(szGIFT_NAME)-1]	= 0;

	// ������ ��� ����ڵ鿡�� ������ �����ش�.
	CHAR szText[256] = {0,};
	wsprintf(szText, "[������ �������� �̺�Ʈ] %s �Բ��� %s ��ǰ�� ��÷�Ǽ̽��ϴ�.",
		szName,
		szGIFT_NAME
		);

	AllSendServerMsg(szText);

	LogAddTD("[Hidden TreasureBox Event] [%s][%s] Success to Register OffLine Gift (GIFT:%s)",
		szAccountID,
		szName,
		szGIFT_NAME
		);
}

#endif


#ifdef PCBANG_COUPON_EVENT_20060124
void EGReqPossiblePCBangCouponEvent(INT iIndex)
{
	if (!gObjIsConnected(iIndex)) {
		return;
	}

	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	if( !lpObj->m_bPCBangCouponUser )
	{
		GCServerCmd(lpObj->m_Index, 7, 4, 0);
		return;
	}

	PMSG_REQ_POSSIBLE_PCBANG_COUPON pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0x18, 0x00, sizeof(pMsg) );

	memcpy (pMsg.szUID, lpObj->AccountID, sizeof(pMsg.szUID));
	pMsg.wServerCode = gGameServerCode / 20;
	pMsg.nINDEX = iIndex;

	pMsg.szUID[sizeof(pMsg.szUID)-1]	= 0;

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));
}

void EGAnsPossiblePCBangCouponEvent(LPPMSG_ANS_POSSIBLE_PCBANG_COUPON lpMsg)
{
	int iIndex = lpMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	PMSG_ANS_PCBANG_COUPON_ITEM pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x01, sizeof(pMsg) );

	// �ش� ������� �߸� ���۵Ǿ���.
//	if( lpMsg->wServerCode != gGameServerCode )
//		return;

	switch( lpMsg->iResultCode )
	{
	case 0x00:		// ����
		GCServerCmd(lpObj->m_Index, 7, 0, 0);
		break;
	case 0x01:		// ���� �̵��
		GCServerCmd(lpObj->m_Index, 7, 1, 0);
		break;
	case 0x02:		// �̹� �߱޵� ����
		GCServerCmd(lpObj->m_Index, 7, 2, 0);
		break;
	case 0x03:		// ����
		GCServerCmd(lpObj->m_Index, 7, 3, 0);
		break;
	}

	DataSend( iIndex, (LPBYTE)&pMsg, sizeof(pMsg) );
}

void EGReqUsePCBangCoupon(INT iIndex)
{
	if (!gObjIsConnected(iIndex)) {
		return;
	}

	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	PMSG_REQ_USE_PCBANG_COUPON pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0x19, 0x00, sizeof(pMsg) );

	memcpy (pMsg.szUID, lpObj->AccountID, sizeof(pMsg.szUID));
	pMsg.wServerCode = gGameServerCode / 20;
	pMsg.nINDEX = iIndex;

	pMsg.szUID[sizeof(pMsg.szUID)-1]	= 0;

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));
}

void EGAnsUsePCBangCoupon(LPPMSG_ANS_USE_PCBANG_COUPON lpMsg)
{
	INT iIndex = lpMsg->nINDEX;
	int iItemType = 0;
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !gObjIsConnected(iIndex))
		return;

	switch(lpMsg->iResultCode)
	{
	case 0x00:		// ����
		PCBangEventNPCItemBag->DropPCBangRevitalizationEventItem( iIndex, lpObj->MapNumber, lpObj->X, lpObj->Y );
		break;
	case 0x01:		// ����
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		GCServerMsgStringSend( lMsg.Get(3355) , lpObj->m_Index, 1);			// "������ �߱޿� �����Ͽ����ϴ�."
		break;
	}
}
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531		// �Ϻ� �� ���� 1�ֳ� ��� ����ǻ��� (����� ���� +1 ~ +5 �� ����)

	#ifdef MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
void Japan1StAnivBoxOpen(LPOBJECTSTRUCT lpObj, int iBoxLevel)
{
	// �켱 �Ϻ� 1�ֳ� ��� ������ �Ͷ߸���.
	PMSG_SERVERCMD	ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
	ServerCmd.CmdType = 0;
	ServerCmd.X = lpObj->X;
	ServerCmd.Y = lpObj->Y;
	MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	
	CProbabilityItemBag* Japan1StAnivBox = NULL;
	
	switch(iBoxLevel)
	{
	case 1:
		{
			Japan1StAnivBox = GoldGoblenItemBag;
		}
		break;
	case 2:
		{
			Japan1StAnivBox = TitanItemBag;
		}
		break;
	case 3:
		{
			Japan1StAnivBox = GoldDerconItemBag;
		}
		break;
	case 4:
		{
			Japan1StAnivBox = DevilLizardKingItemBag;
		}
		break;
	case 5:
		{
			Japan1StAnivBox = KanturItemBag;
		}
		break;
	}
	
	
	Japan1StAnivBox->DropEventItem(lpObj->m_Index,lpObj->MapNumber,lpObj->X,lpObj->Y);
	
}
	#else //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
void Japan1StAnivBoxOpen(LPOBJECTSTRUCT lpObj, int iBoxLevel)
{
	// �켱 �Ϻ� 1�ֳ� ��� ������ �Ͷ߸���.
	PMSG_SERVERCMD	ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
	ServerCmd.CmdType = 0;
	ServerCmd.X = lpObj->X;
	ServerCmd.Y = lpObj->Y;
	MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

	
	// ��ǰ�� ����Ѵ�.
	srand (time(0));

	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;
	int iMinLevel = 0;
	int iAddLevel = 0;

	int iItemDropRate = 30;						// ������ ��� Ȯ�� - n/100
	int iDropMoney = 50000;						// ��� ��

	CItemBag* Japan1StAnivBox = NULL;

	switch(iBoxLevel)
	{
		case 1:
			{
				Japan1StAnivBox = GoldGoblenItemBag;
				iMinLevel = 5;
				iAddLevel = 2;
			}
		break;
		case 2:
			{
				Japan1StAnivBox = TitanItemBag;
				iMinLevel = 4;
				iAddLevel = 2;
			}
		break;
		case 3:
			{
				Japan1StAnivBox = GoldDerconItemBag;
				iMinLevel = 4;
				iAddLevel = 2;
			}
		break;
		case 4:
			{
				Japan1StAnivBox = DevilLizardKingItemBag;
				iMinLevel = 4;
				iAddLevel = 1;
			}
		break;
		case 5:
			{
				Japan1StAnivBox = KanturItemBag;
				iMinLevel = 4;
				iAddLevel = 0;
			}
		break;
	}
	
	if (rand()%100 < iItemDropRate) {
		if( Japan1StAnivBox->GetBagCount() > 0 )
		{
			int NormalItemCount = Japan1StAnivBox->GetNormalItemCount();
			int ExItemCount = Japan1StAnivBox->GetExItemCount();

			// ���⼭ ������ �ƾ��۵��� ��� �Ϲ� ������
			DropItemNum = (rand()%NormalItemCount);
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;
			
			if (iAddLevel)
				level = iMinLevel + rand()%iAddLevel;
			else 
				level = iMinLevel;
			
			type = ItemGetNumberMake(Japan1StAnivBox->GetType(DropItemNum), Japan1StAnivBox->GetIndex(DropItemNum));
			
			Option1 = rand()%2;
			Option2 = rand()%2;

			if( !Option2 || !Option1 )
			{	// ������ �ϳ��� 0�̸� 
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// ȥ��, �༮, ����, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) || 
				(type == MAKE_ITEMNUM(14, 16)) )				
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
				ExOption = 0;
			}
			
			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}			

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[Japan1StAnivBox] BoxLevel-%d Event ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d ex:%d", iBoxLevel, type, level, Option1, Option2, Option3, ExOption);
			return;
		}
	}
	else 
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(iDropMoney*iBoxLevel, lpObj->X, lpObj->Y);	
	}
}

	#endif //MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
#endif //JAPAN_1ST_ANIVERSARY_BOX_20040531


// happycat@20050201 (���ָӴ�, ���� �� �̺�Ʈ) alicecat@
#ifdef HAPPY_POUCH
void DropHappyItemOfRedPouch(LPOBJECTSTRUCT lpObj)
{
	//������ ������ ������ ������ ����� ����� �޶� �ؼ� �׷��� ��� �߾��. ��.��.��.
	int	x    = lpObj->X;
	int	y    = lpObj->Y;

	float dur = 0.0;
	int type = 0, level = 0;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

	// �������� ���� Ȯ���� ���� ������ �Ѵ�. 1/1000
	int iRand = rand() % 1000;
	
	if (iRand == 1)
	{	
		type	= ItemGetNumberMake(13, 20);
		dur		= 255;
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);
		LogAddTD("[Corea Happy New Year] ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
		return;
	}
	
	if( RingEventItemBag->GetBagCount() > 0 )
	{
		DropItemNum = (iRand % RingEventItemBag->GetBagCount());
		dur  = 0;

			INT iRND = rand()%100;

			if (iRND < 5) 
			{			// 5%
				level = 9;
			}
			else if (iRND < 20) 
			{	// 15%
				level = 8;
			}
			else if (iRND < 50) 
			{	// 30%
				level = 7;
			}
			else 
			{					// 50%
				level = 6;
			}

			type = ItemGetNumberMake(RingEventItemBag->GetType(DropItemNum), RingEventItemBag->GetIndex(DropItemNum));

			Option1 = 1;			// ��ų
			Option2 = rand()%2;		// ���

			if( (rand()%5) < 1 )	// �߰��ɼ�
			{
				Option3 = 3;
			}
			else Option3 = (rand()%3);
			
			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);
			LogAddTD("[Corea Happy New Year] ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
		
	}	
}

#endif//HAPPY_POUCH

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
#include "NpcTalk.h"
// [0x20] ������ ���� ������� Ȯ��
void GEReqCheckWhiteAngelGetItem( INT iIndex )
{
	if( !gObjIsConnected( iIndex ) ) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	PMSG_REQ_CHECK_WHITEANGEL_GET_ITEM pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0x20, 0x00, sizeof( pMsg ) );

	memcpy( pMsg.szUID, lpObj->AccountID, sizeof( pMsg.szUID ) );
	pMsg.wServerCode						= gGameServerCode / 20;
	pMsg.nINDEX								= iIndex;
	pMsg.szUID[sizeof( pMsg.szUID ) - 1]	= 0;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

// [0x20] ������ ���� ������� Ȯ�ο� ���� ����
void EGAnsCheckWhiteAngelGetItem( LPPMSG_ANS_CHECK_WHITEANGEL_GET_ITEM lpMsg )
{
	int iIndex = lpMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	switch( lpMsg->iResultCode )
	{
	case 0x00:		// ������ ���� �����
		GCServerCmd( lpObj->m_Index, NPC_WHITEANGEL_EVENT_MSGBOX, 0, 0 );
		break;
	case 0x01:		// ������ ���� ����ڰ� �ƴ�
		GCServerCmd( lpObj->m_Index, NPC_WHITEANGEL_EVENT_MSGBOX, 1, 0 );
		break;
	case 0x02:		// �̹� ���� �Ǿ���
		GCServerCmd( lpObj->m_Index, NPC_WHITEANGEL_EVENT_MSGBOX, 2, 0 );
		break;
	case 0x03:		// ����
		GCServerCmd( lpObj->m_Index, NPC_WHITEANGEL_EVENT_MSGBOX, 3, 0 );
		break;
	}
}

// [0x21] ������ ���� ��û
void GEReqWhiteAngelGetItem( INT iIndex )
{
	if( !gObjIsConnected( iIndex ) ) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	PMSG_REQ_WHITEANGEL_GET_ITEM pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0x21, 0x00, sizeof( pMsg ) );

	memcpy( pMsg.szUID, lpObj->AccountID, sizeof( pMsg.szUID ) );
	pMsg.wServerCode						= gGameServerCode / 20;
	pMsg.nINDEX								= iIndex;
	pMsg.szUID[sizeof( pMsg.szUID ) - 1]	= 0;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

// [0x21] ������ ���� ��û�� ���� ����
void EGAnsWhiteAngelGetItem( LPPMSG_ANS_WHITEANGEL_GET_ITEM lpMsg )
{
	int iIndex			= lpMsg->nINDEX;
	int iItemType		= 0;
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !gObjIsConnected( iIndex ) )
		return;

	switch( lpMsg->iResultCode )
	{
	case 0x00:		// ����		
		GCServerCmd( lpObj->m_Index, NPC_WHITEANGEL_EVENT_MSGBOX, 5, 0 );
#ifdef UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
		LogAddTD( "[ %s ] Fail [%s][%s]", ItemGiveReoEventItemBag->GetEventName(), lpObj->AccountID, lpObj->Name );
#else	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
		LogAddTD( "[ %s ] Fail [%s][%s]", ItemGiveEventItemBag->GetEventName(), lpObj->AccountID, lpObj->Name );
#endif	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
		break;

	case 0x01:		// ���� - ������ �鿡 �ִ� �������� �ش�.
#ifdef UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
		ItemGiveReoEventItemBag->DropEventItem( iIndex, lpObj->MapNumber, lpObj->X, lpObj->Y );
#else	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
		ItemGiveEventItemBag->DropItemGiveEventItem( iIndex, lpObj->MapNumber, lpObj->X, lpObj->Y );
#endif	// UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
		break;
	}	
}
#endif	// WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef UPDATE_LOWLEVEL_SUPPORT_RING_20070119	// ���, ����(������ ��)�� ���� �߰�/����(goni)
void RingOfHeroEventItemBoxOpen(LPOBJECTSTRUCT lpObj)
{
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;

	if( RingOfHeroEventItemBag->GetBagCount() > 0 )
	{
		// 9/10 �� Ȯ���� �̺�Ʈ�� ������ ����
		if( (rand()%10) < 9 )
		{
			DropItemNum = (rand()%RingOfHeroEventItemBag->GetBagCount());

			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;

			// ������ 7 ~ 9 ����
			level= RingOfHeroEventItemBag->GetLevel(DropItemNum)+(rand()%3);

			type = ItemGetNumberMake(RingOfHeroEventItemBag->GetType(DropItemNum), RingOfHeroEventItemBag->GetIndex(DropItemNum));

			Option1 = 1;			// ��ų
			Option2 = rand()%2;		// ���

			if( (rand()%5) < 1 )	// �߰��ɼ�
			{
				Option3 = 3;
			}
			else Option3 = (rand()%3);
			

			// ȥ��, �༮, ������ ������ ����
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// �������� ���� 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index);

#ifdef EXTEND_LOG_SYSTEM_20060202
			CItem EventItem;
			EventItem.Convert( type, Option1, Option2, Option3, 0, 1 );
			LogAddTD("[%s][%s][RingOfHero Event](%d,%d,%d) ItemDrop : Item:%s %d Level:%d op1:%d op2:%d op3:%d",
				lpObj->AccountID, lpObj->Name,
				lpObj->MapNumber, lpObj->X, lpObj->Y,
				EventItem.GetName(), type, level, Option1, Option2, Option3 );
#else
			LogAddTD("[RingOfHero Event] ItemDrop : Item:%d Level:%d op1:%d op2:%d op3:%d", type, level, Option1, Option2, Option3 );
#endif
			return;
		}
	}
	x    = lpObj->X;
	y    = lpObj->Y;	
	MapC[lpObj->MapNumber].MoneyItemDrop(100000, x ,y);
}
#endif	// UPDATE_LOWLEVEL_SUPPORT_RING_20070119

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	// �������ָӴ� ������ �巴
void NewYearLuckyBagItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	NewYearLuckyBagItemBag->DropNewYearLuckyBagEventItem(lpObj->m_Index, btMapNumber, cX, cY);
}
#endif

#ifdef UPDATE_GM_FUNCTION_20070228	// GM �������ڸ� ����
void GMPresentBoxItemBagOpen( LPOBJECTSTRUCT lpObj)
{
	GMPresentBoxItemBag->DropGMPresentBoxEventItem(lpObj->m_Index, lpObj->MapNumber);
}
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206						// ȥ�� ���� ������ �� ����
// �ʷ�ȥ�� ����
void	BoxOfGreenChaosItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	BoxOfGreenChaosItemBag->DropBoxOfGreenChaosItem( lpObj->m_Index, btMapNumber, cX, cY );
}
// ����ȥ�� ����
void	BoxOfRedChaosItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	BoxOfRedChaosItemBag->DropBoxOfRedChaosItem( lpObj->m_Index, btMapNumber, cX, cY );
}
// ����ȥ�� ����
void	BoxOfPurpleChaosItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	BoxOfPurpleChaosItemBag->DropBoxOfPurpleChaosItem( lpObj->m_Index, btMapNumber, cX, cY );
}
#endif

#ifdef CHUSEOK_MONSTER_EVENT_20070823		// �߼� ���� �̺�Ʈ �����۹� ����
void	ChuseokMonsterEventItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	ChuseokMonsterEventItemBag->DropEventItem( lpObj->m_Index, btMapNumber, cX, cY );
}
#endif

#ifdef ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610	// ���� �Ҳ� ���� �̺�Ʈ
void	FireCrackerMonsterEventItemBagOpen( LPOBJECTSTRUCT lpObj, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	FireCrackerMonsterEventItemBag->DropEventItem( lpObj->m_Index, btMapNumber, cX, cY );
}
#endif // ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823		// EventServer���� ���� ó��
// [0x27] ������ ������ �������� Ȯ��
void GEReqCheckAlansTarget( INT iIndex )
{
	if( !gObjIsConnected( iIndex ) ) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	PMSG_REQ_CHECK_WHITEANGEL_GET_ITEM pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0x27, 0x00, sizeof( pMsg ) );

	memcpy( pMsg.szUID, lpObj->AccountID, sizeof( pMsg.szUID ) );
	pMsg.wServerCode						= gGameServerCode / 20;
	pMsg.nINDEX								= iIndex;
	pMsg.szUID[sizeof( pMsg.szUID ) - 1]	= 0;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

// [0x27] ������ ���� ����/�Ұ��ɿ� ���� ����
void EGAnsCheckAlansTarget( LPPMSG_ANS_CHECK_ALANS_TARGET lpMsg )
{
	int iIndex = lpMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	switch( lpMsg->iResultCode )
	{
	case 0x00:		// �̹� ���� �Ǿ���
		GCServerCmd( lpObj->m_Index, NPC_ALANS_MSGBOX, 0, 0 );
		break;
	case 0x01:		// ������ ���
		GCServerCmd( lpObj->m_Index, NPC_ALANS_MSGBOX, 1, 0 );
		break;
	case 0x02:		// ����
		GCServerCmd( lpObj->m_Index, NPC_ALANS_MSGBOX, 2, 0 );
		break;
	}
}

// [0x28] ������ ���� ��û
void GEReqAlansItem( INT iIndex )
{
	if( !gObjIsConnected( iIndex ) ) 
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	PMSG_REQ_WHITEANGEL_GET_ITEM pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0x28, 0x00, sizeof( pMsg ) );

	memcpy( pMsg.szUID, lpObj->AccountID, sizeof( pMsg.szUID ) );
	pMsg.wServerCode						= gGameServerCode / 20;
	pMsg.nINDEX								= iIndex;
	pMsg.szUID[sizeof( pMsg.szUID ) - 1]	= 0;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

// [0x28] ������ ���� ��û�� ���� ����
void EGAnsAlansItem( LPPMSG_ANS_ALANS_ITEM lpMsg )
{
	int iIndex			= lpMsg->nINDEX;
	int iItemType		= 0;
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !gObjIsConnected( iIndex ) )
		return;

	switch( lpMsg->iResultCode )
	{
	case 0x00:		// ����		
		GCServerCmd( lpObj->m_Index, NPC_ALANS_MSGBOX, 2, 0 );
		LogAddTD( "[ %s ] Fail [%s][%s]", NpcAlanEventItemBag->GetEventName(), lpObj->AccountID, lpObj->Name );
		break;

	case 0x01:		// ���� - ������ �鿡 �ִ� �������� �ش�.		
		NpcAlanEventItemBag->DropEventItem( iIndex, lpObj->MapNumber, lpObj->X, lpObj->Y );
		break;
	}	
}
#endif	// ADD_ITEM_GIVE_NPC_ALAN_20070823

#ifdef ADD_PAYITEM_20081030	// EventServer���� ���� ó��
// [DBTASK_REQ_GET_PAYITEM] ������ ������ �������� Ȯ��
#include "NpcTalk.h"

void GEReqCheckEventNPCTarget( INT iIndex , short nEventID)
{
	if( !gObjIsConnected( iIndex ) ) 
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	
	PMSG_REQ_GET_PAYITEM pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, DBTASK_REQ_GET_PAYITEM, 0x00, sizeof( pMsg ) );
	
	memcpy( pMsg.szUID, lpObj->AccountID, sizeof( pMsg.szUID ) );
	pMsg.wServerCode						= gGameServerCode / 20;
	pMsg.nINDEX								= iIndex;
	pMsg.szUID[sizeof( pMsg.szUID ) - 1]	= 0;
	pMsg.nEventID							= nEventID;
	
	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

// [DBTASK_REQ_GET_PAYITEM] ������ ���� ����/�Ұ��ɿ� ���� ����
void EGAnsCheckEventNPCTarget( LPPMSG_ANS_GET_PAYITEM lpMsg )
{
	int iIndex = lpMsg->nINDEX;
	
	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

#if TESTSERVER == 1 //�׽�Ʈ ������ 
	if(lpMsg->nEventID == EVENT_XMAS) {
		if(gXmasGiftForTest == 1) {
			lpMsg->nResult = EPAYITEM::E_NO_ERROR;
		} else if(gXmasGiftForTest == 2) {
			lpMsg->nResult = EPAYITEM::E_NO_ERROR;
			lpMsg->bFirst = 1;
		}
	}
#endif //TESTSERVER
	
	switch( lpMsg->nResult )
	{
	case EPAYITEM::E_NO_ERROR:		
		if(lpMsg->nEventID == EVENT_XMAS){
			if(lpMsg->bFirst) {
				// ���� ���� 
				GCServerCmd( lpObj->m_Index, NPC_SANTA_MSGBOX, 0, 0 );
			} else {
				// �Ϲ� ���� (�Ϸ翡 �ѹ���)
				GCServerCmd( lpObj->m_Index, NPC_SANTA_MSGBOX, 1, 0 );
			}
		}
		break;
	case EPAYITEM::E_ALREADY_SETITEM:
		if(lpMsg->nEventID == EVENT_XMAS){
			GCServerCmd( lpObj->m_Index, NPC_SANTA_MSGBOX, 2, 0 );
		}
		break;
	case EPAYITEM::E_INVALID_EVENTID:
	case EPAYITEM::E_FAILED_RESULT:
		if(lpMsg->nEventID == EVENT_XMAS){
			GCServerCmd( lpObj->m_Index, NPC_SANTA_MSGBOX, 3, 0 );
		}
		break;
	default:
		break;
	}
}

// [DBTASK_ANS_SET_PAYITEM] ������ ���� ��û
void GEReqEventNPCItem( INT iIndex, short nEventID )
{
	if( !gObjIsConnected( iIndex ) ) 
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	
	PMSG_REQ_SET_PAYITEM pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, DBTASK_ANS_SET_PAYITEM, 0x00, sizeof( pMsg ) );
	
	memcpy( pMsg.szUID, lpObj->AccountID, sizeof( pMsg.szUID ) );
	pMsg.wServerCode						= gGameServerCode / 20;
	pMsg.nINDEX								= iIndex;
	pMsg.szUID[sizeof( pMsg.szUID ) - 1]	= 0;
	pMsg.nEventID							= nEventID;
		
	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

// [DBTASK_ANS_SET_PAYITEM] ������ ���� ��û�� ���� ����
void EGAnsEventNPCItem( LPPMSG_ANS_SET_PAYITEM lpMsg )
{
	int iIndex			= lpMsg->nINDEX;
	int iItemType		= 0;
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	
	if( !gObjIsConnected( iIndex ) )
		return;

#if TESTSERVER == 1 //�׽�Ʈ ������ 
	
	if(gXmasGiftForTest > 0 && lpMsg->nEventID == EVENT_XMAS) {
		lpMsg->nResult = EPAYITEM::E_NO_ERROR;
	}
#endif // TESTSERVER

	switch(lpMsg->nResult)
	{
	case EPAYITEM::E_NO_ERROR:		// ���� 
		if(lpMsg->nEventID == EVENT_XMAS){
			// ũ�������� ���� �̺�Ʈ �϶�..
			CHAR szText[256] = {0,};
			gXmasVisitorCount++;
			wsprintf(szText,lMsg.Get(2584),gXmasVisitorCount);

			// Ư�� ���ǿ� ���� ���� �з� 
#ifdef ADD_XMASEVENT_LUCKNUMBER
			if( (gXmasVisitorCount % g_nXMasLuckNumber1st == 0 ) ) 
			{				
				if((gXmasVisitorCount % g_nXMasLuckNumber2nd == 0)) {
					// �� 1000��° �湮��
					pSantaNpcEventItem1000->DropEventItem(iIndex, lpObj->MapNumber, lpObj->X, lpObj->Y);
				} else {
					// �� 100��° �湮��
					pSantaNpcEventItem100->DropEventItem(iIndex, lpObj->MapNumber, lpObj->X, lpObj->Y);
				}
				GCServerMsgStringSend(szText,lpObj->m_Index, 0);
			}
#else
			if( (gXmasVisitorCount % 100 == 0 ) ) {
				//������ 1000��° �湮�ڸ� 200���� �����Ѵ�.
				int nLuckCount = 1000;
				if (gLanguage == 0)
					nLuckCount = 200;

				if((gXmasVisitorCount % nLuckCount == 0)) {
					// �� 1000��° �湮��
					pSantaNpcEventItem1000->DropEventItem(iIndex, lpObj->MapNumber, lpObj->X, lpObj->Y);
				} else {
					// �� 100��° �湮��
					pSantaNpcEventItem100->DropEventItem(iIndex, lpObj->MapNumber, lpObj->X, lpObj->Y);
				}
				GCServerMsgStringSend(szText,lpObj->m_Index, 0);
			}
#endif // ADD_XMASEVENT_LUCKNUMBER

			// ���� �ִ³�..
			if( (gXmasVisitorCount % 100 != 0 ) ) {
				// �ż��� ���� �ӽ��ڵ� 
				// �ż��� �Ϸ� �ѹ� �湮�ڿ��� �������� �������� �ʴ´�.
				if (g_bNewServer != TRUE)
					pSantaNpcEventItem->DropEventItem( iIndex, lpObj->MapNumber, lpObj->X, lpObj->Y );
			}
			
			LogAddTD( "[X-MAX Event] [AccountID]: %s , [VISITCOUNT]:%d",lpObj->AccountID, gXmasVisitorCount);
		}
		break;
	case EPAYITEM::E_ALREADY_SETITEM:
		if(lpMsg->nEventID == EVENT_XMAS) {
			GCServerCmd( lpObj->m_Index, NPC_SANTA_MSGBOX, 2, 0 );
		}
		break;
	case EPAYITEM::E_INVALID_EVENTID:	
	case EPAYITEM::E_FAILED_RESULT:
		if(lpMsg->nEventID == EVENT_XMAS){
			GCServerCmd( lpObj->m_Index, NPC_SANTA_MSGBOX, 3, 0 );
		}
		break;
	default:
		break;
	}	
}
#endif	//ADD_PAYITEM_20081030


#ifdef ADD_NPC_XMASEVENT_20080930
void CGReqCheckSnowManNPC( INT iIndex )
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	
	if( !gObjIsConnected( iIndex ) )
		return;
	
    // ��Ÿ Ŭ���� �޽��� �ڽ��� �����ش� 
	GCServerCmd( lpObj->m_Index, NPC_SNOWMAN_MSGBOX, 1, 0 );
}

void CGReqSnowManMoveGate(INT iIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	
	if( !gObjIsConnected( iIndex ) )
		return;
	gObjMoveGate(lpObj->m_Index, 22);
}

#endif // ADD_NPC_XMASEVENT_20080930

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029 
// [0x31] ��ϵ� ���� ���� ��û
void GEReqGetCointCount(INT iIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !gObjIsConnected( iIndex ) )
		return;

	PMSG_ES_REQ_GET_COIN_COUNT pReqMsg = { 0, };

	pReqMsg.h.c			= PMHC_BYTE;
	pReqMsg.h.headcode	= 0x33;
	pReqMsg.h.size		= sizeof( pReqMsg );

	pReqMsg.nINDEX		= lpObj->m_Index;
	pReqMsg.wServerCode	= gGameServerCode / 20;

	memcpy(pReqMsg.szAccountID, lpObj->AccountID, MAX_IDSTRING);
	pReqMsg.szAccountID[sizeof( pReqMsg.szAccountID )-1] = 0;
	memcpy(pReqMsg.szGameID,    lpObj->Name, MAX_IDSTRING);
	pReqMsg.szGameID[sizeof(pReqMsg.szGameID)-1] = 0;		

	DataSendEventChip( ( char* )&pReqMsg, sizeof( pReqMsg ) );
}

// [0x31] ��ϵ� ���� ���� ����
void EGAnsGetCoinCount(LPPMSG__ES_ANS_GET_COIN_COUNT lpMsg)
{
	int iIndex			= lpMsg->nINDEX;
	int iItemType		= 0;
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !gObjIsConnected( iIndex ) )
		return;

	if( strcmp(lpMsg->szGameID, lpObj->Name) != 0)
	{
		return;
	}

	GCAnsGetCoinCount(iIndex, lpMsg->nCurCoinCnt);
}

// [0x32] ���� ���
void GEReqRegeistCoin(INT iIndex, int nInventoryPos)
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !gObjIsConnected( iIndex ) )
		return;

	PMSG_ES_REQ_REG_COIN  pReqMsg = { 0, };

	pReqMsg.h.c			= PMHC_BYTE;
	pReqMsg.h.headcode	= 0x34;
	pReqMsg.h.size		= sizeof( pReqMsg );

	pReqMsg.nINDEX		= lpObj->m_Index;
	pReqMsg.wServerCode	= gGameServerCode / 20;

	memcpy(pReqMsg.szAccountID, lpObj->AccountID, MAX_IDSTRING);
	pReqMsg.szAccountID[sizeof( pReqMsg.szAccountID )-1] = 0;
	memcpy(pReqMsg.szGameID,    lpObj->Name, MAX_IDSTRING);
	pReqMsg.szGameID[sizeof(pReqMsg.szGameID)-1] = 0;		

	pReqMsg.btPos = nInventoryPos;

	DataSendEventChip( ( char* )&pReqMsg, sizeof( pReqMsg ) );
}

// [0x32] ���� ��� ����
void EGAnsRegeistCoin(LPPMSG_ES_ANS_REG_COIN lpMsg)
{
	int iIndex			= lpMsg->nINDEX;
	int iItemType		= 0;
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !gObjIsConnected( iIndex ) )
		return;

	if( strcmp(lpMsg->szGameID, lpObj->Name) != 0)
	{
		return;
	}

	GCAnsRegeistCoin(iIndex, lpMsg->btResult, lpMsg->nCurCoinCnt, lpMsg->btPos);
}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029 