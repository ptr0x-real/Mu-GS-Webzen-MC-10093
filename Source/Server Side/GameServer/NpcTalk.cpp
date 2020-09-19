#include "StdAfx.h"
#include "Include\Public.h"
#include "Include\ProDef.h"
#include "Include\Readscript.h"
#include "user.h"
#include "Protocol.h"
#include "NPCTalk.h"
#include "Gamemain.h"
#include "DevilSquare.h"
#include "gObjMonster.h"

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

#include "Event.h"
#include "Include\cb_protocol.h"

#ifdef NEW_SKILL_FORSKYLAND
#include "QuestInfo.h"
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#include "Weapon.h"
#endif

#ifdef CRYWOLF_NPC_WORK_20050912
#include "Crywolf.h"
#endif

#ifdef ADD_KANTURU_20060627
#include "Kanturu.h"
#endif

#ifdef UPDATE_PK_SYSTEM_20070302
#include "PartyClass.h"
extern PartyClass		gParty;
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
#include "CashShop.h"
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	#include "EventItemBagExMng.h"
#else
	#include "CashLotterySystem.h"
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"
#endif // ADD_NEWPVP_PKFIELD


BOOL NpcGuildMasterTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcFasi(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcBattleAnnouncer(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);


BOOL NpcRolensiaGuard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcChaosGoblelin(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcRusipher(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

BOOL NpcPosionGirl(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcDeviasWizard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcDeviasGuard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcDeviasWareHousemen(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcNoriaRara(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcDeviasMadam(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

BOOL NpcEventChipNPC(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

#ifdef JAPAN_NPC_NEWTALK
BOOL NpcRorensiaSmith(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcNoriJangIn(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
BOOL NpcCouponEvent(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef	FOR_BLOODCASTLE
BOOL NpcAngelKing(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcAngelMessanger(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcRolensiaMadam(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcRoadMerchant(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcDeviasWeapon(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

BOOL NpcRorensiaSmith(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcNoriJangIn(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif

BOOL NpcWarehouse(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

#ifdef NEW_SKILL_FORSKYLAND
BOOL NpcQuestCheck(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif


BOOL NpcServerDivision(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

#ifdef DARKLORD_WORK
BOOL NpcDarkSpiritTrainer(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif


#ifdef JEWEL_MIX_SYSTEM_20050309
BOOL NpcJewelMixDealer(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif

#ifdef ADD_NEW_NPC_CASTLE_20041115
BOOL NpcCastleCrown		(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);			// 216
BOOL NpcCastleSwitch	(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);			// 217, 218
BOOL NpcCastleGateLever	(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);			// 219
BOOL NpcElderCircle		(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);			// 223
BOOL NpcCastleGuard		(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);			// 224
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
BOOL NpcSiegeMachine_Attack(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 221
BOOL NpcSiegeMachine_Defense(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 222
#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
BOOL NpcCastleHuntZoneGuard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 220
#endif

#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
BOOL NpcShadowPhantom(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif

#ifdef CRYWOLF_NPC_WORK_20050912
BOOL NpcCrywolfAltar(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif

#ifdef ADD_SHOP_KALIMA_NPC_REIRA_20060403
BOOL NpcReira( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );
#endif

#ifdef ADD_NEW_NPC_KANTURU_20060612
BOOL NpcMainatenceMachine( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );
#endif

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
BOOL NpcWhiteAngelEvent( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );
#endif

#ifdef UPDATE_QUARTERMASTER_NPC_20070122	// 물자보급관 파멜라, 안젤라 추가(goni)
BOOL NpcQuartermasterPamella(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcQuartermasterAngela(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif

#ifdef ADD_FIRECRACKER_NPC_20070306			// 폭죽판매 NPC 대화
BOOL NPcFireCrackerStore(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328				// 이벤트맵 NPC 대화 함수
BOOL NpcIllusionHolyRelicsStatue(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);	// 석상
BOOL NpcHolyRelicsBoxAllied(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 성물보관함(연합군)
BOOL NpcHolyRelicsBoxIllusion(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 성물보관함(환영교)
BOOL NpcPlatformMirage(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);				// 미라쥬
#endif

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// 아이템 지급 NPC 앨런 대화 함수
BOOL NpcAlan( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// 엘베란드 NPC 대화
BOOL NpcSilviaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 실비아
BOOL NpcRheaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 레아
BOOL NpcMarceTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 마르세
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
BOOL NpcCherryTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 벚꽃의 정령
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
BOOL NpcSeedMaster(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 시드 마스터
BOOL NpcSeedResearcher(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);	// 시드 연구가
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#define NPC_POSIONGIRL				253		// 물약파는 소녀
#define NPC_DEVIASWIZARD			245		// 데비아스 마법사
#define NPC_DEVIASGUARD				247		// 데비아스 경비병
#define NPC_DEVIASWAREHOUSEMEN		240		// 데비아스&&노리아 창고지기

#define NPC_NORIARARA				242		// 노리아 라라요정

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
BOOL NpcChaosCardMaster( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
BOOL NpcStatResetTalk( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );	//시즌 4 체험서버 스탯초기화 NPC
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
BOOL NpcLuckyCoinDelgado( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_NPC_XMASEVENT_20080930
BOOL Npc_SantaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL Npc_SnowManTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL Npc_LittleSantaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj, int iBuffType);

#define LIFE_RECOVERY					0
#define MANA_RECOVERY					1
#endif //ADD_NPC_XMASEVENT_20080930

BOOL NpcPkClearCheck(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcResetCheck(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcBuffHelperCheck(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

//#define MAX_TALKTABLE	10
//CTalkTableClass	CTalkTable[MAX_TALKTABLE];

// RETURN 
// 0 : 통과
// 1 : 말만 출력
int NpcTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	int npcnum = lpNpc->Class;
	if( npcnum < 0 ) return 0;
	
#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}
#endif

	if ( NpcResetCheck(lpNpc, lpObj) != FALSE )
	{
		return TRUE;
	}

	if ( NpcPkClearCheck(lpNpc, lpObj) != FALSE )
	{
		return TRUE;
	}

	if ( NpcBuffHelperCheck(lpNpc, lpObj) != FALSE )
	{
		return TRUE;
	}

#ifdef SECOND_QUEST
	if( npcnum == NPC_QEUSTNPC2 )
	{
		gQeustNpcTeleport.TalkRefAdd();
		lpObj->TargetShopNumber = npcnum;
	}
#endif

#ifdef PCBANG_COUPON_EVENT_20060124
	if( npcnum == NPC_COUPONNPC )
	{
		if( NpcCouponEvent(lpNpc, lpObj) )
			return 1;
	}
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef NEW_SKILL_FORSKYLAND
	// 퀘스트에 관련된 NPC를 체크한다
	if( NpcQuestCheck(lpNpc, lpObj) )
		return 1;
#endif

	switch( npcnum )
	{
	case NPC_FASI : //	마법사 파시
		if( NpcFasi(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_GUILDMASTER :
		if( NpcGuildMasterTalk(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_BATTLEANNOUNCER :
		if( NpcBattleAnnouncer(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_ROLENSIAGUARD: // 로랜시아 경비병
		if( NpcRolensiaGuard(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_CHAOSGOBLELIN: // 카오스 고블린
		if( NpcChaosGoblelin(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_RUSIPHER: // 루시퍼(카론)
		if( NpcRusipher(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_POSIONGIRL: // 물약파는 소녀
		if( NpcPosionGirl(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_DEVIASWIZARD: // 데비아스 마법사
		if( NpcDeviasWizard(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_DEVIASGUARD: // 데비아스 경비병
		if( NpcDeviasGuard(lpNpc, lpObj) == TRUE ) return 1;
		break;

		
	case NPC_DEVIASWAREHOUSEMEN: // 데비아스&&노리아 창고지기
		//if( NpcDeviasWareHousemen(lpNpc, lpObj) == TRUE ) return 1;
		if( NpcWarehouse(lpNpc, lpObj) == TRUE ) return 1;
		break;

#ifdef DARKLORD_WORK
	case NPC_DARKSPRIT_TRAINER:		// 다크스피릿 조련사
		if( NpcDarkSpiritTrainer(lpNpc, lpObj) == TRUE) return 1;		
		break;
#endif
		

	case NPC_NORIARARA: // 노리아 라라요정
		if( NpcNoriaRara(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_DEVIASMADAM: // 데비아스 술집마담
		if( NpcDeviasMadam(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_EVETNCHIPNPC: // 이벤트칩 NPC
		if( NpcEventChipNPC(lpNpc, lpObj) == TRUE ) return 1;
		break;

#ifdef JAPAN_NPC_NEWTALK
	case NPC_RELENSIASMITH: // 로랜시아 대장장이
		if( NpcRorensiaSmith(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_NORIAJANGIN:
		if( NpcNoriJangIn(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef GAMESERVER_DIVISION
		case NPC_SERVERDIVISION: // 로랜시아 대장장이
			if( NpcServerDivision(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif
		
#ifdef FOR_BLOODCASTLE
	case NPC_ANGELKING :
		if( NpcAngelKing(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_ANGELMESSANGER :
		if( NpcAngelMessanger(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_ROLENSIAMADAM :
		if( NpcRolensiaMadam(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_ROADMERCHANT :
		if( NpcRoadMerchant(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_DEVIASWEAPON :
		if( NpcDeviasWeapon(lpNpc, lpObj) == TRUE ) return 1;
		break;
#ifndef JAPAN_NPC_NEWTALK
	case NPC_ROLENSIASMITH: // 로랜시아 대장장이 <- 일본거에도 있음
		if( NpcRorensiaSmith(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_NORIAJANGIN:
		if( NpcNoriJangIn(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif
#endif	

#ifdef JEWEL_MIX_SYSTEM_20050309
	case NPC_JEWEL_MIX_DEALER :
		if( NpcJewelMixDealer(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif
#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
	case NPC_SIEGE_MACHINE_ATTACK :
		if( NpcSiegeMachine_Attack(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_SIEGE_MACHINE_DEFENSE :
		if( NpcSiegeMachine_Defense(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif
#ifdef ADD_NEW_NPC_CASTLE_20041115
	case NPC_ELDER_CIRCLE :
		if( NpcElderCircle(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_CASTLE_GUARD :
		if( NpcCastleGuard(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_CASTLE_GATE_LEVER :
		if( NpcCastleGateLever(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_CASTLE_CROWN :
		if( NpcCastleCrown(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_CASTLE_SWITCH1 :
	case NPC_CASTLE_SWITCH2 :
		if( NpcCastleSwitch(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
	case NPC_CASTLE_HUNTZONE_GUARD :
		if( NpcCastleHuntZoneGuard(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
	case NPC_SHADOW_PHANTOM :
		if( NpcShadowPhantom(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef CRYWOLF_NPC_WORK_20050912
	case CRYWOLF_NPC_ALTAR1 :
		if( NpcCrywolfAltar(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case CRYWOLF_NPC_ALTAR2 :
		if( NpcCrywolfAltar(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case CRYWOLF_NPC_ALTAR3 :
		if( NpcCrywolfAltar(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case CRYWOLF_NPC_ALTAR4 :
		if( NpcCrywolfAltar(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case CRYWOLF_NPC_ALTAR5 :
		if( NpcCrywolfAltar(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef ADD_SHOP_KALIMA_NPC_REIRA_20060403
	case NPC_KALIMA_REIRA:	// 칼리마 신관 레이라
		if( NpcReira( lpNpc, lpObj ) == TRUE ) return 1;
		break;
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	case JEWELOFHARMONY_NPC_PURITY:
	case JEWELOFHARMONY_NPC_SMELT:
	case JEWELOFHARMONY_NPC_RESTORE:
		// 조화의보석 관련 NPC는 따로 처리한다.
		if( g_kJewelOfHarmonySystem.NpcJewelOfHarmony( lpNpc, lpObj ) == TRUE ) 
			return 1;
		break;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	case NPC_CHAOSCARDMASTER:
		if( NpcChaosCardMaster( lpNpc, lpObj ) == TRUE )
		{
			return 1;
		}
		break;
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef ADD_NEW_NPC_KANTURU_20060612
	case NPC_KANTURU_MACHINE:	// 칸투르 NPC - 출입관리장치
		if( NpcMainatenceMachine( lpNpc, lpObj ) == TRUE ) return 1;
		break;		
#endif

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
	case NPC_WHITEANGEL_EVENT:	// 화이트엔젤 아이템 지급 이벤트
		if( NpcWhiteAngelEvent( lpNpc, lpObj ) == TRUE )	return 1;
		break;
#endif

#ifdef UPDATE_QUARTERMASTER_NPC_20070122	// 물자보급관 파멜라, 안젤라 추가(goni)
	case NPC_QUARTERMASTER_PAMELLA:
		if(NpcQuartermasterPamella(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_QUARTERMASTER_ANGELA:
		if(NpcQuartermasterAngela(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef ADD_FIRECRACKER_NPC_20070306			// 폭죽판매 NPC 대화
	case NPC_FRIECRACKER_STORE:
		if(NPcFireCrackerStore(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	case NPC_PLATFORM_MIRAGE:
		if( NpcPlatformMirage(lpNpc, lpObj)	== TRUE )	return 1;
		break;

	case NPC_ILLUSION_HOLY_RELICS_STATUE:
		if( NpcIllusionHolyRelicsStatue(lpNpc, lpObj)	== TRUE )	return 1;
		break;

	case NPC_HOLY_RELICS_BOX_ALLIED:
		if( NpcHolyRelicsBoxAllied(lpNpc, lpObj)	== TRUE )	return 1;
		break;

	case NPC_HOLY_RELICS_BOX_ILLUSION:
		if( NpcHolyRelicsBoxIllusion(lpNpc, lpObj)	== TRUE )	return 1;
		break;	
#endif

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// 아이템 지급 NPC 앨런의 대화
	case NPC_ALAN:				
		if( NpcAlan( lpNpc, lpObj ) == TRUE )	return 1;
		break;
#endif		

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// 엘베란드 NPC 대화
	case NPC_SILVIA:
		if( NpcSilviaTalk(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_RHEA:
		if( NpcRheaTalk(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_MARCE:
		if( NpcMarceTalk(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	case NPC_CHERRY:
		if( NpcCherryTalk(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
	case NPC_SEED_MASTER:
		if( NpcSeedMaster( lpNpc, lpObj ) == TRUE )
		{
			return 1;
		}
		break;
	case NPC_SEED_RESEARCHER:
		if( NpcSeedResearcher( lpNpc, lpObj ) == TRUE )
		{
			return 1;
		}
		break;
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET 
	case NPC_STAT_RESET:
		if( NpcStatResetTalk(lpNpc, lpObj) == TRUE)
		{
			return 1;
		}
		break;
#endif //EXPERIENCE_SERVER_NPC_STAT_RESET

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
	case NPC_DELGADO:
		if( NpcLuckyCoinDelgado(lpNpc, lpObj) == TRUE )
		{
			return 1;
		}
		break;
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029
		//------------->
		//20081013 해외 크리스마스 이벤트 - NPC (grooving)
#ifdef ADD_NPC_XMASEVENT_20080930
	case NPC_SANTA:
		if( Npc_SantaTalk(lpNpc, lpObj) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_SNOW_MAN:
		if( Npc_SnowManTalk(lpNpc, lpObj) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_LITTLE_SANTA_1:
		if( Npc_LittleSantaTalk(lpNpc, lpObj, BUFFTYPE_SANTA_POWER) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_LITTLE_SANTA_2:
		if( Npc_LittleSantaTalk(lpNpc, lpObj, BUFFTYPE_SANTA_GUARD) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_LITTLE_SANTA_3://체력 100% 회복
		if( Npc_LittleSantaTalk(lpNpc, lpObj, LIFE_RECOVERY) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_LITTLE_SANTA_4://마나 100% 회복
		if( Npc_LittleSantaTalk(lpNpc, lpObj, MANA_RECOVERY) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_LITTLE_SANTA_5:
		if( Npc_LittleSantaTalk(lpNpc, lpObj, BUFFTYPE_SANTA_HEAL) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_LITTLE_SANTA_6:
		if( Npc_LittleSantaTalk(lpNpc, lpObj, BUFFTYPE_SANTA_PROTECT) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_LITTLE_SANTA_7:
		if( Npc_LittleSantaTalk(lpNpc, lpObj, BUFFTYPE_SANTA_SPEED) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_LITTLE_SANTA_8:
		if( Npc_LittleSantaTalk(lpNpc, lpObj, BUFFTYPE_SANTA_LUCKY) == TRUE)
		{
			return 1;
		}
		break;
#endif //ADD_NPC_XMASEVENT_20080930
#ifdef ADD_NEWPVP_PKFIELD
	case NPC_TITUS:
		{
			if( lpObj->m_IfState.use > 0 )
			{
				return TRUE;
			}

			g_NewPVP.SendChannelList(*lpNpc, *lpObj);

			PMSG_TALKRESULT pResult;
			pResult.h.c = PMHCE_BYTE;
			pResult.h.headcode = 0x30;
			pResult.h.size = sizeof( pResult );
			pResult.result = 0x21;
			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

			// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
			lpObj->m_IfState.use    = 1;
			lpObj->m_IfState.type   = I_ETCWINDOW;

			return 1;
		}
		break;
#endif // ADD_NEWPVP_PKFIELD

	}
	return 0;
}

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// 앨런과의 대화
BOOL NpcAlan( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj )
{
	// 거래중 이거나 상점 이용 중 일때는 사용할 수 없다.
	if( lpObj->m_IfState.use > 0 )
	{
		return TRUE;
	}
	
	GEReqCheckAlansTarget( lpObj->m_Index );	
	return TRUE;
}
#endif

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
BOOL NpcWhiteAngelEvent( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj )
{
	// 거래중 이거나 상점 이용 중 일때는 사용할 수 없다.
	if( lpObj->m_IfState.use > 0 )
	{
		return TRUE;
	}
	
	GEReqCheckWhiteAngelGetItem( lpObj->m_Index );	
	return TRUE;
}
#endif // WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef ADD_NEW_NPC_KANTURU_20060612
BOOL NpcMainatenceMachine( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj )
{
	g_KanturuEntranceNPC.NotifyEntranceInfo( lpObj->m_Index );
	return TRUE;
}
#endif

#ifdef ADD_SHOP_KALIMA_NPC_REIRA_20060403
BOOL NpcReira( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj )
{
	return FALSE;
}
#endif

BOOL NpcBattleAnnouncer(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef CASTLE_POTION_NPC_20041222			// 수성측 물약파는 NPC
	if (lpNpc->MapNumber == MAP_INDEX_CASTLESIEGE) {
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
			if (lpObj->m_btCsJoinSide != CS_SIEGESIDE_DEFEND) {
				INT iChatRnd	= rand() % 3;
				switch (iChatRnd) {
				case 0 :
					ChatTargetSend(lpNpc, "적에게 물건을 팔수는 없지 !!", lpObj->m_Index);
					break;
				case 1 :
					ChatTargetSend(lpNpc, "무사히 여길 빠져나가지는 못할것이다 !!", lpObj->m_Index);
					break;
				case 2 :
					ChatTargetSend(lpNpc, "여기 적군이 있다 !! 경비병 !!", lpObj->m_Index);
					break;
				}
				return TRUE;
			}
		}
	}
#endif

	return 0;
}

BOOL NpcFasi(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifndef DISABLE_NPC_MESSAGE_20050506
#ifdef JAPAN_NPC_NEWTALK
	if( !gDevilSquareEvent )
	{
		GCServerCmd(lpObj->m_Index, 5, 1, 0);
	}
#endif
#endif
	if( gLanguage != 0 )
	{
		if( gDevilSquareEvent )
			GCServerCmd(lpObj->m_Index, 1, NPC_FASI_MSGBOX, 0);
	}
	
#ifdef	BLOOCASTLE_NPC_TALK_20031215
	if (g_bBloodCastle) {
		GCServerCmd(lpObj->m_Index, 1, 26, 0);
	}
#endif

#ifdef UPDATE_PK_SYSTEM_20070302		// NPC FASI
	BOOL	PKFlag = FALSE;
	if(lpObj->PartyNumber >= 0)	// 파티에 속할경우
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
	if( PKFlag == TRUE)
#else
	if( lpObj->m_PK_Level > PK_LEVEL_DEFAULT+1 )
#endif	// UPDATE_PK_SYSTEM_20070302
	{
		int hour=0, min=0;
		char szTalk[100];
		if( lpNpc->TargetNumber != lpObj->m_Index )
		{
			if( gObjGetPkTime(lpObj, hour, min) == TRUE )
			{
				wsprintf(szTalk, lMsg.Get(1103), hour, min);
				ChatTargetSend(lpNpc, szTalk, lpObj->m_Index);
				lpNpc->TargetNumber = lpObj->m_Index;			

#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
				if (!gPkLimitFree) {
#endif
				return TRUE;
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
				}
#endif
			}
		}
		else
		{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
			if (!gPkLimitFree) {
#endif
			int talk = (rand()%3);
			switch( talk )
			{
			case 0 : ChatTargetSend(lpNpc, lMsg.Get(1122), lpObj->m_Index);	// "살인자에겐 아무것도 팔 수 없네!!"
				break;
			case 1 : ChatTargetSend(lpNpc, lMsg.Get(1123), lpObj->m_Index);	// 나에게 더이상 볼일이 있는가?
				break;
			case 2 : ChatTargetSend(lpNpc, lMsg.Get(1124), lpObj->m_Index);	// 다른데 가서 알아보게
				break;
			}			
			lpNpc->TargetNumber = 0;
			return TRUE;
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
			}
#endif
		}
	}
	return FALSE;
}


BOOL NpcGuildMasterTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( gGuildCreate == 0 )
	{
		ChatTargetSend(lpNpc, "서버분할 기간에는 길드를 생성할수가 없습니다", lpObj->m_Index); // 길드를 더이상 만들수가 없소
		//ChatTargetSend(lpNpc, lMsg.Get(1119), lpObj->m_Index); // 길드를 더이상 만들수가 없소
		return TRUE;
	}

	/*if( lpObj->Level < 100 )
	{
		int t=rand()%3;
		switch( t )
		{
		case 0 : ChatTargetSend(lpNpc, "당신이 길드마스터가 되겠다고? 설마~", lpObj->m_Index);	break;
		case 1 : ChatTargetSend(lpNpc, "길드마스터가 되려면 아직 멀었네..", lpObj->m_Index);	break;
		case 2 : ChatTargetSend(lpNpc, "길드마스터가 되려면 레벨이 100은 되어야 한다네..", lpObj->m_Index);	break;
		}
	}
	else*/
	{
		//ChatTargetSend(lpNpc, "111", lpObj->m_Index);
		//ChatTargetSend(lpNpc, "아이템이 모두 +7레벨은 되어야 하네.", lpObj->m_Index);
		if( lpObj->GuildNumber > 0 )
		{
			ChatTargetSend(lpNpc, lMsg.Get(1140), lpObj->m_Index);	// "당신은 이미 길드에 속해 있지 않습니까?"
			return TRUE;
		}
#ifdef _NEW_EXDB_
		int nRet = gObjGuildMasterCapacityTest(lpObj);
		if( nRet == 0 ) 
		{
			ChatTargetSend(lpNpc, lMsg.Get(1141), lpObj->m_Index); // 레벨이 100 이상은 되어야 합니다
		}
		else if( nRet == 2 ) 
		{
			ChatTargetSend(lpNpc, lMsg.Get(1142), lpObj->m_Index);	// 착용한 아이템이 모두 +7레벨은 되어야 합니다
		}
		else 
#endif
		{
			GCGuildMasterQuestionSend(lpObj->m_Index);			
		}
	}
	return TRUE;
}


//로렌시아 경비병
BOOL NpcRolensiaGuard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	// Message박스를 보여달라고 요청한다
	// MsgBox Index = 0 : 요즘 마족들이 도아다니고 있다는 소문이 나돌고 있어, 전설에만 존재하는 줄 알았는데.. 그들이 왜 지상에 왜 지상에 나타난걸까?
	// MsgBox Index = 1 : 악마의 광장에 대해 알고 싶다면 루시퍼를 찾아보게.. 
	//GCServerCmd(lpObj->m_Index, 1, (rand()%2)+NPC_ROLENSIAGUARD_MSGBOX, 0);
	if( gLanguage != 0 )
	{
#ifndef DISABLE_NPC_MESSAGE_20050506
		if( gDevilSquareEvent )
			GCServerCmd(lpObj->m_Index, 1, (rand()%2)+NPC_ROLENSIAGUARD_MSGBOX, 0);	
#endif
	}
	else
	{
		if( gEnableEventNPCTalk )
			GCServerCmd(lpObj->m_Index, 4, 0, 0);	

#ifdef	BLOOCASTLE_NPC_TALK_20031215
		if (g_bBloodCastle) {
			GCServerCmd(lpObj->m_Index, 1, 25, 0);
		}
#endif
	}
	return TRUE;
}
BOOL NpcChaosGoblelin(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef JAPAN_NPC_NEWTALK
	if( !gDevilSquareEvent )
	{
		GCServerCmd(lpObj->m_Index, 5, 5, 0);
	}
#endif

	if( gUserFindDevilSquareKeyEyes(lpObj->m_Index) )
	{	// 이벤트 아이템이 있으면
		// MsgBox Index = 3 :"마족의 보물을 가져오셨군요. 당신에게 행운의 여신이 함께 해준다면 놀라운걸 얻게 됩니다."

#ifndef REMOVE_CHAOSMIX_NPC_MESSAGE_20050125

#ifndef GAMESERVER_TO_CASTLESIEGESERVER
		if( gDevilSquareEvent )
#endif
			GCServerCmd(lpObj->m_Index, 1, NPC_CHAOSGOBLELIN_MSGBOX+1, 0);

#endif

	}		
	else
	{
		if( gLanguage != 0 )
		{
#ifndef REMOVE_CHAOSMIX_NPC_MESSAGE_20050125

#ifndef GAMESERVER_TO_CASTLESIEGESERVER
			if( gDevilSquareEvent )
#endif
				GCServerCmd(lpObj->m_Index, 1, NPC_CHAOSGOBLELIN_MSGBOX, 0);
#endif
		}
	}
	
	return FALSE;
}

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
BOOL NpcChaosCardMaster(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj )
{
#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	if( g_bUseLotteryEvent == FALSE )
#else
	if( g_bUseLotterySystem == FALSE )
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	{
		return FALSE;
	}

#ifdef MODIFY_PCS_CHAOSMIX_PSHOP_BUGFIX
	if (lpObj->m_bPShopOpen == true) 
	{
		GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "개인상점 개설 중에서는 카오스 조합을 사용하실 수 없습니다."
		return FALSE;
	}	
#endif // MODIFY_PCS_CHAOSMIX_PSHOP_BUGFIX

	if( bCanChaosBox == TRUE )
	{
		if( lpObj->m_IfState.use > 0 )
		{	// 창을 사용하고 있을때는.. 사용할수 없다
			return FALSE;
		}

	#ifdef MODIFY_CHAOSCARD_MIX_BLOCK_PSHOP_USE_20070713
		#ifdef PERSONAL_SHOP_20040113		// 자신이나 상대방이 개인상점을 개설중이면 카오스 조합을 할 수 없다.
		if (lpObj->m_bPShopOpen == true) {
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "개인상점 개설 중에서는 카오스 조합을 사용하실 수 없습니다."
			return TRUE;
		}
		#endif
	#endif // MODIFY_CHAOSCARD_MIX_BLOCK_PSHOP_USE_20070713

		PMSG_TALKRESULT			pResult;

		pResult.h.c				= PMHCE_BYTE;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof( pResult );
		pResult.result			= 0x15;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);	

		// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
		lpObj->m_IfState.use    = 1;
		lpObj->m_IfState.type   = I_CHAOSBOX;

		LogAddTD("[JewelMix] [%s][%s] Jewel Mix Dealer Start", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

BOOL NpcRusipher(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( !gDevilSquareEvent )
		return TRUE;

	int aIndex = lpObj->m_Index;


#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}
#endif

	if( !gUserFindDevilSquareInvitation(aIndex) )
	{	// 초대권이 없다
		GCServerCmd(lpObj->m_Index, 1, NPC_RUSIPHER_MSGBOX, 0);
	}
	else
	{
		if( g_DevilSquare.GetState() == DevilSquare_OPEN )
		{	// 악마의 광장으로 갈수 있는 창을 열라고 한다
			PMSG_TALKRESULT		pResult;

			pResult.h.c				= PMHCE_BYTE;
			pResult.h.headcode		= 0x30;
			pResult.h.size			= sizeof( pResult );
			pResult.result			= 0x04;
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
			// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
			lpObj->m_IfState.use    = 1;
			lpObj->m_IfState.type   = I_ETCWINDOW;
#endif
		}		
		else
		{	// 악마의 광장에 들어 갈수 없는 시간이다
			GCServerCmd(lpObj->m_Index, 1, NPC_RUSIPHER_MSGBOX+1, 0);
		}
	}
	return TRUE;
}

BOOL NpcPosionGirl(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef CASTLE_POTION_NPC_20041222			// 공성측 물약파는 NPC
	if (lpNpc->MapNumber == MAP_INDEX_CASTLESIEGE) {
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
			if (lpObj->m_btCsJoinSide == CS_SIEGESIDE_DEFEND) {
				INT iChatRnd	= rand() % 3;
				switch (iChatRnd) {
				case 0 :
					ChatTargetSend(lpNpc, "적에게 팔 물건은 없어요.", lpObj->m_Index);
					break;
				case 1 :
					ChatTargetSend(lpNpc, "빨리 사라지지 않으면 소리를 지를거에요.", lpObj->m_Index);
					break;
				case 2 :
					ChatTargetSend(lpNpc, "여기 적군이 있어요. 누가 좀 도와주세요 !!", lpObj->m_Index);
					break;
				}
				return TRUE;
			}
		}
	}
#endif

	if( gLanguage != 0 )		
	{
#ifndef DISABLE_NPC_MESSAGE_20050506
		if( gDevilSquareEvent )
			GCServerCmd(lpObj->m_Index, 1, NPC_POSIONGIRL_MSGBOX, 0);	
#endif
	}

#ifdef	BLOOCASTLE_NPC_TALK_20031215
	if (g_bBloodCastle) {
		GCServerCmd(lpObj->m_Index, 1, 29, 0);
	}
#endif
	
	return FALSE;
}

BOOL NpcDeviasWizard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( gLanguage != 0 )		
	{
#ifndef DISABLE_NPC_MESSAGE_20050506
		if( gDevilSquareEvent )
			GCServerCmd(lpObj->m_Index, 1, NPC_DEVIASWIZARD_MSGBOX, 0);	
#endif
	}
	
#ifdef	BLOOCASTLE_NPC_TALK_20031215
	if (g_bBloodCastle) {
		GCServerCmd(lpObj->m_Index, 1, 32, 0);
	}
#endif
	return FALSE;
}

BOOL NpcDeviasGuard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	//GCServerCmd(lpObj->m_Index, 1, NPC_DEVIASGUARD_MSGBOX, 0);		
	if( gLanguage != 0 )		
	{
#ifndef DISABLE_NPC_MESSAGE_20050506
		if( gDevilSquareEvent )
			GCServerCmd(lpObj->m_Index, 1, NPC_DEVIASGUARD_MSGBOX, 0);
#endif
	}
		
#ifdef	BLOOCASTLE_NPC_TALK_20031215
	if (g_bBloodCastle) {
		GCServerCmd(lpObj->m_Index, 1, 25, 0);
	}
#endif
	return TRUE;
}

BOOL NpcDeviasWareHousemen(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifndef DISABLE_NPC_MESSAGE_20050506
	if( gDevilSquareEvent )
		GCServerCmd(lpObj->m_Index, 1, NPC_DEVIASWAREHOUSEMEN_MSGBOX, 0);	
#endif
	return FALSE;	
}

// 창고 지기 공통
BOOL NpcWarehouse(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef JAPAN_NPC_NEWTALK
	if( lpNpc->MapNumber == 0 )
	{	// 로랜시아 창고지기
		GCServerCmd(lpObj->m_Index, 5, 2, 0);
	}
	else if( lpNpc->MapNumber == 3 )
	{	// 데비아스 창고지기
		GCServerCmd(lpObj->m_Index, 5, 4, 0);
	}
#endif
	/*
	char szTemp[256];
	int  rZen;

	rZen = lpObj->Level*lpObj->Level*0.1;
	if( rZen < 1 ) rZen = 1;

	if( (lpObj->Money-rZen) < 1 ) 
	{
		wsprintf(szTemp, "창고를 사용하기 위해선 %d젠이 필요하네..", rZen);
		ChatTargetSend(lpNpc, szTemp, lpObj->m_Index);
		return TRUE;
	}
	*/
	return FALSE;
}

BOOL NpcNoriaRara(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( gLanguage != 0 )
	{
#ifndef DISABLE_NPC_MESSAGE_20050506
		if( gDevilSquareEvent )
			GCServerCmd(lpObj->m_Index, 1, NPC_NORIARARA_MSGBOX, 0);	
#ifdef JAPAN_NPC_NEWTALK
		else
		{
			GCServerCmd(lpObj->m_Index, 5, 3, 0);
		}
#endif
#endif
	}
	else
	{
		if( gEnableEventNPCTalk )
			GCServerCmd(lpObj->m_Index, 4, 2, 0);	

#ifdef	BLOOCASTLE_NPC_TALK_20031215
		if (g_bBloodCastle) {
			GCServerCmd(lpObj->m_Index, 1, 30, 0);
		}
#endif
	}
	return FALSE;	
}

BOOL NpcDeviasMadam(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( gLanguage != 0 )
	{
#ifndef DISABLE_NPC_MESSAGE_20050506
		if( gDevilSquareEvent )
			GCServerCmd(lpObj->m_Index, 1, NPC_DEVIASMADAM_MSGBOX, 0);	
#endif
	}
	else
	{
		if( gEnableEventNPCTalk )
			GCServerCmd(lpObj->m_Index, 4, 1, 0);	

#ifdef	BLOOCASTLE_NPC_TALK_20031215
		if (g_bBloodCastle) {
			GCServerCmd(lpObj->m_Index, 1, 33, 0);
		}
#endif
	}
	return FALSE;	
}


BOOL NpcEventChipNPC(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	// 거래중 이거나 상점 이용 중 일때는 사용할 수 없다.
	if( lpObj->m_IfState.use > 0 )
	{
		return TRUE;
	}
	
// comment_alice_050113 : NO JAPAN!!!! See the define.h
#ifdef EVENT_RENA_FOR_CASTLE_SIEGE_20041116
	PMSG_REQ_VIEW_EC_MN	pForCastleMsg;	// 공성전 왕의직인 대신 레나로 이벤트 진행
	PHeadSetB((LPBYTE)&pForCastleMsg, 0x01, sizeof(pForCastleMsg));	
	
	pForCastleMsg.iINDEX = lpObj->m_Index;
	strcpy(pForCastleMsg.szUID, lpObj->AccountID);
	
	DataSendEventChip((char*)&pForCastleMsg, sizeof(pForCastleMsg));

	lpObj->m_IfState.type = I_EVENTCHIP;	
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;

	return TRUE;

#endif // EVENT_RENA_FOR_CASTLE_SIEGE_20041116

#ifdef EVENT_RENA_20040909 // 레나 이벤트
	if (lpNpc->MapNumber == MAP_INDEX_RORENCIA) // 로렌시아
	{
		PMSG_REQ_VIEW_EC_MN	pMsgTha;
		PHeadSetB((LPBYTE)&pMsgTha, 0x01, sizeof(pMsgTha));	

		pMsgTha.iINDEX = lpObj->m_Index;
		strcpy(pMsgTha.szUID, lpObj->AccountID);

		pEventObj =  lpNpc;
		
		DataSendEventChip((char*)&pMsgTha, sizeof(pMsgTha));
		
		lpObj->m_IfState.type = I_EVENTCHIP;	
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}

#endif // EVENT_RENA_20040909
	
#ifdef EVENT_STONE_OF_FRIENDSHIP_20040206  // 우정의 돌 이벤트
	PMSG_REQ_VIEW_FRIENDSHIP_STONES	pMsg;	

	PHeadSetB((LPBYTE)&pMsg, 0x11, sizeof(pMsg));	

	pMsg.iINDEX = lpObj->m_Index;
	memcpy(pMsg.szUID, lpObj->AccountID, MAX_IDSTRING);
	memcpy(pMsg.szNAME, lpObj->Name, MAX_IDSTRING);
	pMsg.wServerCode = gGameServerCode / 20;
	pMsg.szUID[MAX_IDSTRING] = 0;
	pMsg.szNAME[MAX_IDSTRING] = 0;

	pEventObj =  lpNpc;
	
	DataSendEventChip((char*)&pMsg, sizeof(pMsg));

	lpObj->m_IfState.type = I_EVENTCHIP;	
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;

	return TRUE;
	
#endif // EVENT_STONE_OF_FRIENDSHIP_20040206

#if defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )
	if (lpNpc->MapNumber == MAP_INDEX_DEVIAS) // 데비아스
	{
		PMSG_EVENTCHIPINFO	Result;
		PHeadSetB((LPBYTE)&Result, 0x94, sizeof(Result));

		Result.Type = 2; // 복권 이벤트 설정
		
		DataSend(lpObj->m_Index, (LPBYTE)&Result, Result.h.size);

		lpObj->m_IfState.type = I_EVENTCHIP;	
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}

#endif // EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO

#ifdef EVENT_STONE // 스톤 이벤트
	PMSG_REQ_VIEW_EC_MN	pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x05, sizeof(pMsg));	

	pMsg.iINDEX = lpObj->m_Index;
	strcpy(pMsg.szUID, lpObj->AccountID);

	pEventObj = lpNpc;
	
	DataSendEventChip((char*)&pMsg, sizeof(pMsg));
	
	lpObj->m_IfState.type = I_EVENTCHIP;	
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;

	return TRUE;

#endif // EVENT_STONE

	return FALSE;
}


BOOL NpcRorensiaSmith(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOCASTLE_NPC_TALK_20031215
	if (g_bBloodCastle) {
		GCServerCmd(lpObj->m_Index, 1, 27, 0);
		return FALSE;
	}
#endif

#ifdef JAPAN_NPC_NEWTALK	// 2004-01-15 0.97 일본 추가
	// 로랜시아는 처음인가? ...
	GCServerCmd(lpObj->m_Index, 5, 7, 0);	
#endif

//	GCServerCmd(lpObj->m_Index, 5, 0, 0);		// 일본 0.97 패치로 인해 주석처리 (2004.01.15)	
	return FALSE;
}

#ifdef PCBANG_COUPON_EVENT_20060124
BOOL NpcCouponEvent(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	// 거래중 이거나 상점 이용 중 일때는 사용할 수 없다.
	if( lpObj->m_IfState.use > 0 )
	{
		return TRUE;
	}

	EGReqPossiblePCBangCouponEvent(lpObj->m_Index);

	return TRUE;
}
#endif // PCBANG_COUPON_EVENT_20060124

BOOL NpcNoriJangIn(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOCASTLE_NPC_TALK_20031215
	if (g_bBloodCastle) {
		GCServerCmd(lpObj->m_Index, 1, 31, 0);
	}
#endif	
	//GCServerCmd(lpObj->m_Index, 5, 3, 0);
	return FALSE;
}


#ifdef NEW_SKILL_FORSKYLAND
BOOL NpcQuestCheck(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( g_QuestInfo.NpcTalk(lpNpc, lpObj) )
		return TRUE;

	return FALSE;
}
#endif

// 서버분할 NPC
BOOL NpcServerDivision(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( !gEnableServerDivision )
		return TRUE;

	if( lpObj->lpGuild )
	{	// 길드에 가입되어 있는 상태에서는 이동할수 없다
		GCServerCmd(lpObj->m_Index, 6, 0, 0);	
		return TRUE;
	}	

	lpObj->m_IfState.type   = I_SERVERDIVISION;	
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;
	
	return FALSE;
}


#ifdef FOR_BLOODCASTLE

BOOL NpcRoadMerchant(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOCASTLE_NPC_TALK_20031215
	if (g_bBloodCastle) {
		GCServerCmd(lpObj->m_Index, 1, 28, 0);
	}
#endif
	return FALSE;
}


BOOL NpcAngelKing(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	INT iITEM_LEVEL = 0;

	if (!CHECK_LIMIT(lpObj->m_cBloodCastleIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		GCServerCmd(lpObj->m_Index, 1, 24, 0);								// 다리의 인덱스가 틀리다면 더 찾아보라고 말한다.
		return FALSE;
	}

	LogAddTD ("[Blood Castle] (%d) [%s][%s] Talk to Angel King",
		lpObj->m_cBloodCastleIndex+1,
		lpObj->AccountID,
		lpObj->Name
		);

	if (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bBC_REWARDED) {
		// 이 다리에 이미 보상이 이루어졌다면 퀘스트는 완료된 것이다. -> 수고했다고 말해준다.
		GCServerCmd(lpObj->m_Index, 1, 46, 0);
		return FALSE;
	}

	if ((g_BloodCastle.GetCurrentState(lpObj->m_cBloodCastleIndex) != BLOODCASTLE_STATE_PLAYING) ||
		(!g_BloodCastle.CheckPlayStart(lpObj->m_cBloodCastleIndex))
		) 
	{
		GCServerCmd(lpObj->m_Index, 1, 24, 0);								// 경기시간이 아니면 더 찾아보라고 말한다.
		return FALSE;
	}

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	if (lpObj->m_bBloodCastleComplete == true) {							// 이미 완료했다면 수고했다고 말해준다.
		// 이때 보상은 이루어지지 않았을 수도 있다.
		GCServerCmd(lpObj->m_Index, 1, 46, 0);			// !!! . 새로운 메시지
//		GCServerCmd(lpObj->m_Index, 1, 49, 0);			// !!! . 새로운 메시지
		return FALSE;
	}
#endif
	
	if ( CHECK_LIMIT(iITEM_LEVEL = g_BloodCastle.CheckQuestItem(lpObj->m_Index), 3) ) {
		if (lpNpc->m_cBloodCastleIndex != lpObj->m_cBloodCastleIndex) {		// 같은 블러드캐슬 레벨이 아니라면 더 찾아보라고 말한다.
			GCServerCmd(lpObj->m_Index, 1, 24, 0);
			return FALSE;
		}

		if (!g_BloodCastle.CheckUserBridgeMember(lpObj->m_cBloodCastleIndex, lpObj->m_Index)) {
			GCServerCmd(lpObj->m_Index, 1, 24, 0);							// 해당 다리의 멤버가 아니면 더 찾아보라고 말한다.
			return FALSE;
		}

		// 해당 케릭을 우승자로 지정한다.
		g_BloodCastle.SetUserState(lpObj->m_Index, BC_USER_WINNER);

#ifndef BLOODCASTLE_EVENT_5TH_20050531
		// 성공했다고 말해준다.
		GCServerCmd(lpObj->m_Index, 1, 23, 0);
#endif

		// 퀘스트를 성공한 사용자와 파티번호를 저장한다.
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Win_Quest_Party = lpObj->PartyNumber;
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Win_Quest_Index = lpObj->m_Index;
		memcpy (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName, lpObj->Name, MAX_IDSTRING);
		memcpy (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID, lpObj->AccountID, MAX_IDSTRING);
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName[MAX_IDSTRING] = '\0';
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID[MAX_IDSTRING] = '\0';

		LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Destroy Castle Door [%s][%s]", 
			lpObj->m_cBloodCastleIndex+1, 
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID,
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName
			);
		LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Destroy Saint Status [%s][%s]",
			lpObj->m_cBloodCastleIndex+1, 
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID,
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName
			);
		LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Win Quest [%s][%s]", 
			lpObj->m_cBloodCastleIndex+1, 
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID,
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName
			);

#ifdef BLOODCASTLE_EVENT_5TH_20050531
		if (g_BloodCastle.SetBridgeWinner(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == true) {
			lpObj->m_bBloodCastleComplete = true;

			// 현재 우승자의 다리에 등록할 파티원이 있는지 확인한다.
			if (g_BloodCastle.CheckPartyExist(lpObj->m_Index) == false) {
				// 더 등록할 파티원이 없으면 성공처리
				
/*
				// 우승 케릭이 있는 다리에 보상을 한다.
				g_BloodCastle.GiveReward_Win(lpObj->m_Index, lpObj->m_cBloodCastleIndex);

				// 퀘스트를 종료한다.
				g_BloodCastle.SetState(lpObj->m_cBloodCastleIndex, BLOODCASTLE_STATE_PLAYEND);
*/

				// 성공했다고 말해준다.
				GCServerCmd(lpObj->m_Index, 1, 23, 0);		// !!! . 새로운 메시지
//				GCServerCmd(lpObj->m_Index, 1, 47, 0);		// !!! . 새로운 메시지

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경
					g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
					lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
					lpObj->AccountID,
					lpObj->Name
					);
			}
			else {
				// 더 등록해야 할 파티원이 있음

				// 성공했는데 기다려야 한다고 말해준다.
				GCServerCmd(lpObj->m_Index, 1, 23, 0);		// !!! . 새로운 메시지
//				GCServerCmd(lpObj->m_Index, 1, 47, 0);		// !!! . 새로운 메시지

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경
					g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
					lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
					lpObj->AccountID,
					lpObj->Name
					);
			}

		}
		else {
			// 이미 우승자 있음 ? -> 심각한 에러
			LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Winner Already Exist !!",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경
				g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
				lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
				lpObj->AccountID,
				lpObj->Name
				);
			return FALSE;
		}

		// 퀘스트 아이템을 찾아서 없애준다.
		g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
#else
		// 우승 케릭이 있는 다리에 보상을 한다.
		g_BloodCastle.GiveReward_Win(lpObj->m_Index, lpObj->m_cBloodCastleIndex);

		// 퀘스트를 종료한다.
		g_BloodCastle.SetState(lpObj->m_cBloodCastleIndex, BLOODCASTLE_STATE_PLAYEND);

		// 퀘스트 아이템을 찾아서 없애준다.
		g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);

		LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경
			g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
			lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
			lpObj->AccountID,
			lpObj->Name
			);
#endif
	}
	else {
#ifdef BLOODCASTLE_EVENT_5TH_20050531
		// 만약 절대무기를 가지고 있지 않다면 가지고 있는 사람과 같은 파티원인지 확인한다.
		if (g_BloodCastle.CheckUserWinnerParty(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == true) {
			lpObj->m_bBloodCastleComplete = true;

			// 현재 우승자의 다리에 등록할 파티원이 있는지 확인한다.
			if (g_BloodCastle.CheckPartyExist(lpObj->m_Index) == false) {
				// 더 등록할 파티원이 없으면 성공처리
				
/*
				// 우승 케릭이 있는 다리에 보상을 한다.
				g_BloodCastle.GiveReward_Win(lpObj->m_Index, lpObj->m_cBloodCastleIndex);

				// 퀘스트를 종료한다.
				g_BloodCastle.SetState(lpObj->m_cBloodCastleIndex, BLOODCASTLE_STATE_PLAYEND);
*/

				// 성공했다고 말해준다.
				GCServerCmd(lpObj->m_Index, 1, 23, 0);		// !!! . 새로운 메시지
//				GCServerCmd(lpObj->m_Index, 1, 48, 0);		// !!! . 새로운 메시지

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경
					g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
					lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
					lpObj->AccountID,
					lpObj->Name
					);
			}
			else {
				// 더 등록해야 할 파티원이 있음

				// 성공했는데 기다려야 한다고 말해준다.
				GCServerCmd(lpObj->m_Index, 1, 23, 0);		// !!! . 새로운 메시지
//				GCServerCmd(lpObj->m_Index, 1, 48, 0);		// !!! . 새로운 메시지

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경
					g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
					lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
					lpObj->AccountID,
					lpObj->Name
					);
			}

			// 다른 메시지를 뿌려주어야 할 것
			
			// 퀘스트가 종료했는지 확인
			
			return FALSE;
		}
#endif		

		GCServerCmd(lpObj->m_Index, 1, 24, 0);
	}
	
	return FALSE;
}


BOOL NpcAngelMessanger(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}
#endif
	
#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
	// 카오 유저는 입장할 수 없다.
	int iIndex = lpObj->m_Index;
	if( gObj[iIndex].m_PK_Level >= PK_LEVEL_DEFAULT + 1 )
	{		
#ifdef MODIFY_BLOODCASTLE_PK_USER_ENTER_MESSAGE_20070321	// 카오유저 입장시 메지시 주성
		GCServerCmd( iIndex, MSGBOX_ENTERLIMIT_PKUSER_BLOODCASTLE, 0, 0 );	// 카오 유저는 입장할 수 없다는 메세지를 보낸다.
#else
		GCServerCmd( iIndex, MSGBOX_ENTERLIMIT_PKUSER_CHAOSCASTLE, 0, 0 );	// 카오 유저는 입장할 수 없다는 메세지를 보낸다.
#endif
		return TRUE;
	}
#endif
	

	INT iITEM_LEVEL = 0;

	if ( iITEM_LEVEL = g_BloodCastle.CheckEnterItem(lpObj->m_Index) )
	{
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

#ifdef MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
		// 투명 망토가 있는 경우 레벨을 채크하여 입장이 가능한 상태인지 확인한다.
		if( iITEM_LEVEL != 10 && g_BloodCastle.CheckEnterLevel( lpObj->m_Index, iITEM_LEVEL-1 ) != 0 )
		{
			// 레벨이 맞지 않는 경우 투명망토가 있는지 확인한다.
			if( g_BloodCastle.CheckEnterFreeTicket( lpObj->m_Index ) == true )
			{
				iITEM_LEVEL = 10;
			}
		}
	#endif // MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627

		if( iITEM_LEVEL == 10 )			// 자유 입장권 아이템 사용
		{
			for( int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT + 1 ; i++ )
			{
				if( g_BloodCastle.CheckEnterLevel( lpObj->m_Index, i ) == 0 )
				{
#ifdef MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
					iITEM_LEVEL = i;
#else
					if( g_BloodCastle.GetCurrentState( i - 1 ) == BLOODCASTLE_STATE_CLOSED && g_BloodCastle.CheckCanEnter( i - 1 ) )
					{
						PMSG_TALKRESULT		pResult;
						pResult.h.c				= PMHCE_BYTE;
						pResult.h.headcode		= 0x30;
						pResult.h.size			= sizeof( pResult );
						pResult.result			= 0x06;
						DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
						lpObj->m_IfState.use    = 1;
						lpObj->m_IfState.type   = I_ETCWINDOW;
#endif
					}
					else
					{
						GCServerCmd(lpObj->m_Index, 1, 20, 0);
						break;
					}
#endif // MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
				}
			}
		}
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

		if ((g_BloodCastle.GetCurrentState(iITEM_LEVEL-1) == BLOODCASTLE_STATE_CLOSED) && g_BloodCastle.CheckCanEnter(iITEM_LEVEL-1)) {	// 조건을 다 충족
			PMSG_TALKRESULT		pResult;
			pResult.h.c				= PMHCE_BYTE;
			pResult.h.headcode		= 0x30;
			pResult.h.size			= sizeof( pResult );
			pResult.result			= 0x06;
			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
			lpObj->m_IfState.use    = 1;
			lpObj->m_IfState.type   = I_ETCWINDOW;
#endif
		}
		else {		// 아이템은 있는데 시간이 않되었다.
			GCServerCmd(lpObj->m_Index, 1, 20, 0);	
		}
	}
	else {			// 아이템은 없다.
		GCServerCmd(lpObj->m_Index, 1, 21, 0);	
	}
	
	return FALSE;
}

#ifdef EVENT_LOTTO
extern void CGRequestLottoRegister(LPPMSG_REQ_2ANV_LOTTO_EVENT lpMsg, int aIndex);
#endif

BOOL NpcRolensiaMadam(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOCASTLE_NPC_TALK_20031215
	if( g_bBloodCastle )
		GCServerCmd(lpObj->m_Index, 1, 35, 0);
#endif	
	return FALSE;
}


BOOL NpcDeviasWeapon(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOCASTLE_NPC_TALK_20031215
	if( g_bBloodCastle )
		GCServerCmd(lpObj->m_Index, 1, 34, 0);	
#endif	
	return FALSE;
}

#endif

#ifdef DARKLORD_WORK
BOOL NpcDarkSpiritTrainer(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( bCanChaosBox == TRUE )
	{
		PMSG_TALKRESULT		pResult;

		pResult.h.c				= PMHCE_BYTE;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof( pResult );
#ifdef PERSONAL_SHOP_20040113		// 자신이나 상대방이 개인상점을 개설중이면 카오스 조합을 할 수 없다.
		if (lpObj->m_bPShopOpen == true) {
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "개인상점 개설 중에서는 카오스 조합을 사용하실 수 없습니다."
			return TRUE;
		}
#endif
		lpObj->m_IfState.type   = I_DARK_TRAINER;
		lpObj->m_IfState.state	= 0;
		lpObj->m_IfState.use    = 1;
		pResult.result     = 0x07;
		
#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
		lpObj->bIsChaosMixCompleted = false;			// 카오스 조합 전에 '아직 조합이 완료되지 않았다' 로 초기값을 세팅
#endif

		// 악마의 광장 조합 확률 추가
		pResult.level1	= gDQChaosSuccessRateLevel1;
		pResult.level2	= gDQChaosSuccessRateLevel2;
		pResult.level3	= gDQChaosSuccessRateLevel3;
		pResult.level4	= gDQChaosSuccessRateLevel4;
#ifdef DEVILSQUARE_EXTEND_20050221		// !!! . 임시 -> 카오스 조합 추가
		pResult.level5	= gDQChaosSuccessRateLevel5;
		pResult.level6	= gDQChaosSuccessRateLevel6;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// PMSG_TALKRESULT 광장7 정보 전송
		pResult.level7	= gDQChaosSuccessRateLevel7;
#endif

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		
#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219				// 현재 적용된 세율 (카오스조합) 을 보내준다. (조련사는 별도로 보냄)
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
		GCAnsCsMapSvrTaxInfo (lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
#endif
#endif

		gObjInventoryTrans(lpObj->m_Index);
		LogAddTD("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
	return TRUE;
}
#endif



#ifdef JEWEL_MIX_SYSTEM_20050309

BOOL NpcJewelMixDealer(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( bCanChaosBox == TRUE )
	{

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
		if( lpObj->m_IfState.use > 0 )
		{	// 창을 사용하고 있을때는.. 사용할수 없다
			return TRUE;
		}
#endif

		PMSG_TALKRESULT			pResult;

		pResult.h.c				= PMHCE_BYTE;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof( pResult );
		pResult.result			= 0x09;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);	

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
		// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
		lpObj->m_IfState.use    = 1;
	#ifdef MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020
		lpObj->m_IfState.type   = I_JEWEL;
	#else
		lpObj->m_IfState.type   = I_ETCWINDOW;
	#endif // MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020		
#endif

		LogAddTD("[JewelMix] [%s][%s] Jewel Mix Dealer Start", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
	return TRUE;
}

#endif


#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116

BOOL NpcSiegeMachine_Attack(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{

	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}

	PMSG_ANS_USE_WEAPON_INTERFACE pResult = {0,};

	PHeadSubSetB((LPBYTE)&pResult, 0xB7, 0x00, sizeof( pResult) );
	pResult.btResult		= WEAPON_USE_FAIL;
	pResult.btWeaponType	= WEAPON_TYPE_CASTLE_ATTACKER;
	pResult.btObjIndexH		= HIBYTE( lpNpc->m_Index );
	pResult.btObjIndexL		= LOBYTE( lpNpc->m_Index );

	if( gObjIsConnectedGP(lpNpc->m_iWeaponUser) == FALSE ) 
	{
		lpNpc->m_iWeaponUser	= 0;
		lpNpc->m_btWeaponState	= WEAPON_STATE_USABLE;
	}

	lpNpc->m_btWeaponState = WEAPON_STATE_USABLE;

	if( lpNpc->m_btWeaponState == WEAPON_STATE_USABLE )
	{
		pResult.btResult		= WEAPON_USE_SUCCESS;
		lpNpc->m_btWeaponState	= WEAPON_STATE_USING;
		lpNpc->m_iWeaponUser	= lpObj->m_Index;

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->m_IfState.use    = 1;
		lpObj->m_IfState.type   = I_CS_MACHINE_ATTACK;
	}
	else
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}
	
	
	
	return TRUE;
}


BOOL NpcSiegeMachine_Defense(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{

	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}


	PMSG_ANS_USE_WEAPON_INTERFACE pResult = {0,};

	PHeadSubSetB((LPBYTE)&pResult, 0xB7, 0x00, sizeof( pResult) );
	pResult.btResult		= WEAPON_USE_FAIL;
	pResult.btWeaponType	= WEAPON_TYPE_CASTLE_DEFENDER;
	pResult.btObjIndexH		= HIBYTE( lpNpc->m_Index );
	pResult.btObjIndexL		= LOBYTE( lpNpc->m_Index );

	if( gObjIsConnectedGP(lpNpc->m_iWeaponUser) == FALSE ) 
	{
		lpNpc->m_iWeaponUser	= 0;
		lpNpc->m_btWeaponState	= WEAPON_STATE_USABLE;
	}

	lpNpc->m_btWeaponState = WEAPON_STATE_USABLE;

	if( lpNpc->m_btWeaponState == WEAPON_STATE_USABLE )
	{
		pResult.btResult		= WEAPON_USE_SUCCESS;
		lpNpc->m_btWeaponState	= WEAPON_STATE_USING;
		lpNpc->m_iWeaponUser	= lpObj->m_Index;

		
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		
		
		// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
		lpObj->m_IfState.use    = 1;
		lpObj->m_IfState.type   = I_CS_MACHINE_DEFENSE;
	}
	else
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}
	
	return TRUE;
}
#endif


#ifdef ADD_NEW_NPC_CASTLE_20041115

BOOL NpcElderCircle(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
	// 공성 시간이라면 무시한다.
	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
		ChatTargetSend(lpNpc, lMsg.Get(1621), lpObj->m_Index);	//	"현재는 성을 운영할 수 있는 시간이 아닙니다."
		return TRUE;
	}

	// 길드에 가입되어있지 않다면 무시한다.
	if (!strcmp(lpObj->GuildName, "")) {
		ChatTargetSend(lpNpc, lMsg.Get(1622), lpObj->m_Index);	//	"당신은 성을 운영할 권한이 없습니다."
		return TRUE;
	}

	// 필요하다면 요청자의 권한을 체크한다. (수성측 특정 지위가 아니라면 불가)
	if (g_CastleSiege.CheckGuildOwnCastle(lpObj->GuildName) == FALSE || 
		(lpObj->GuildStatus != G_MASTER && lpObj->GuildStatus != G_SUB_MASTER)
		) 
	{
		ChatTargetSend(lpNpc, lMsg.Get(1622), lpObj->m_Index);	//	"당신은 성을 운영할 권한이 없습니다."
		return TRUE;
	}
#endif

	PMSG_TALKRESULT			pResult;
	pResult.h.c				= PMHCE_BYTE;
	pResult.h.headcode		= 0x30;
	pResult.h.size			= sizeof( pResult );
	pResult.result			= 0x0C;

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
	lpObj->m_IfState.use    = 1;
	lpObj->m_IfState.type   = I_ETCWINDOW;
#endif

#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
	// 악마의 광장 조합 확률 -> level1을 카오스 조합 가능여부로 보내고 나머지 무시
	pResult.level1	= 0;
	pResult.level2	= 0;
	pResult.level3	= 0;
	pResult.level4	= 0;
#ifdef DEVILSQUARE_EXTEND_20050221		// !!! . 임시 -> 카오스 조합 추가
	pResult.level5	= 0;
	pResult.level6	= 0;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010 // PMSG_TALKRESULT 광장7 정보 추가
	pResult.level7	= 0;
#endif

	if( bCanChaosBox == TRUE )
	{
		// 성주 아이템 조합이 않되더라도 성 운영의 기본기능은 사용할 수 있어야 하므로 이렇게 처리

		lpObj->m_IfState.type   = I_CHAOSBOX;

#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
		lpObj->bIsChaosMixCompleted = false;			// 카오스 조합 전에 '아직 조합이 완료되지 않았다' 로 초기값을 세팅
#endif
		
#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
		lpObj->m_bIsCastleNPCUpgradeCompleted	= false;	// 공성관련 'NPC를 업그레이드 하지 않았음' 으로 초기값 세팅
#endif

		pResult.level1	= 1;							// 카오스 조합이 사용 가능하다고 알려줌

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219				// 현재 적용된 세율 (카오스조합) 을 보내준다. (조련사는 별도로 보냄)
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
		GCAnsCsMapSvrTaxInfo (lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
#endif
#endif

		gObjInventoryTrans(lpObj->m_Index);
		LogAddTD("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
#endif

	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		
	return TRUE;
}


BOOL NpcCastleGuard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
	// 공성 시간이라면 무시한다.
	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
		ChatTargetSend(lpNpc, lMsg.Get(1623), lpObj->m_Index);		//	"현재는 성 정보를 볼 수 있는 시간이 아닙니다."
		return TRUE;
	}
#endif

	PMSG_TALKRESULT			pResult;

	pResult.h.c				= PMHCE_BYTE;
	pResult.h.headcode		= 0x30;
	pResult.h.size			= sizeof( pResult );
	pResult.result			= 0x0D;
	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);	

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
	lpObj->m_IfState.use    = 1;
	lpObj->m_IfState.type   = I_ETCWINDOW;
#endif

	return TRUE;
}


BOOL NpcCastleGateLever(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}
#endif

	// 1 . 성문 인터페이스를 볼 권한이 되는지 확인한다.
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	BOOL bControlEnable			= FALSE;
	BYTE btResult				= 0;
	INT	iGateIndex				= -1;

	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
		// 공성전 중일 때
		if (lpObj->m_btCsJoinSide != CS_SIEGESIDE_DEFEND 
//			|| lpObj->m_bCsGuildInvolved != 1
			) 
		{
			// 수성측 당사자가 아니다.
			btResult			= 4;
			bControlEnable		= FALSE;
		}
		else {
			// 성문을 제아헐 수 있는지 체크
			bControlEnable		= TRUE;
		}
	}
	else {
		// 공성전 중이 아닐 때
		if (g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) == FALSE &&
			g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE
			) 
		{
			// 성길드 멤버가 아니다.
			btResult			= 4;
			bControlEnable		= FALSE;
		}
		else {
			// 성문을 제아헐 수 있는지 체크
			bControlEnable		= TRUE;
		}
	}

	if (bControlEnable) {
		if (g_CastleSiege.CheckLeverAlive(lpNpc->m_Index) == TRUE) {
			if (g_CastleSiege.CheckCsGateAlive(lpNpc->m_iCsGateLeverLinkIndex) == TRUE) {
				btResult		= 1;
				iGateIndex		= lpNpc->m_iCsGateLeverLinkIndex;
			}
			else {
				// 성문이 존재하지 않는다.
				btResult		= 2;
			}
		}
		else {
			// 레버가 존재하지 않는다.
			btResult			= 3;
		}
	}
	
	GCAnsCsGateState (lpObj->m_Index, btResult, iGateIndex);
	
	if (btResult != 1) {
		// 결과가 실패일 때는 대화 결과를 보내지 않는다.
		return TRUE;
	}
#else
	// 혹시라도 공성 적용이 않되었을 때는 그냥 나감
	return TRUE;
#endif

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
	lpObj->m_IfState.use    = 1;
	lpObj->m_IfState.type   = I_ETCWINDOW;
#endif

	return TRUE;
}


BOOL NpcCastleCrown		(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{

#ifdef CASTLE_MAIN_SCHEDULER_20041111
	if (!gObjIsConnected(lpObj->m_Index)) {
		return TRUE;
	}

	// 1 . 공성 시간인지 확인한다.
	if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE) {
		return TRUE;
	}

	// 2 . 공성측인지 확인한다.
	if (lpObj->m_btCsJoinSide < CS_SIEGESIDE_ATTACK || lpObj->m_bCsGuildInvolved == 0) {
		return TRUE;
	}
	
	// 3 . 길드연합의 마스터인지 확인한다.
	if (!g_CastleSiege.CheckUnionGuildMaster(lpObj->m_Index)) {
		return TRUE;
	}

	// 4 . 왕관과의 거리를 체크한다.
	if (abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3 ) {
		return TRUE;
	}

	// 5 . 방어막이 해제되었는지 체크한다.
	if (g_CastleSiege.GetRegCrownAvailable() == FALSE) {
		return TRUE;
	}

	// 6 . 현재 왕관에 직인 등록 중인 사용자가 없다면 이 케릭을 등록한다.
	INT iUserIndex	= g_CastleSiege.GetCrownUserIndex();
	if (!gObjIsConnected(iUserIndex)) 
	{

#ifdef MODIFY_CASTLE_CROWN_ACCESS_ALLOWED_BY_CROWNSWITCH_20051102
		// 왕관 스위치 작동 유저가 같은 연합인지 검사한다.
		INT iCrownSwitchUserIndex_1 = g_CastleSiege.GetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH1);
		INT iCrownSwitchUserIndex_2 = g_CastleSiege.GetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH2);

		if( !gObjIsConnected(iCrownSwitchUserIndex_1) || !gObjIsConnected(iCrownSwitchUserIndex_2) )
		{
			GCAnsCsAccessCrownState (lpObj->m_Index, 4);

			LogAddTD("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",
			lpObj->AccountID,
			lpObj->Name,
			lpObj->GuildName
			);

			return TRUE;
		}

		if( lpObj->m_btCsJoinSide != gObj[iCrownSwitchUserIndex_1].m_btCsJoinSide 
			|| lpObj->m_btCsJoinSide != gObj[iCrownSwitchUserIndex_2].m_btCsJoinSide )
		{
			GCAnsCsAccessCrownState (lpObj->m_Index, 4);

			LogAddTD("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s) (S1:%s/%s)(S2:%s/%s)",
			lpObj->AccountID,
			lpObj->Name,
			lpObj->GuildName,
			gObj[iCrownSwitchUserIndex_1].Name,
			gObj[iCrownSwitchUserIndex_1].GuildName,
			gObj[iCrownSwitchUserIndex_2].Name,
			gObj[iCrownSwitchUserIndex_2].GuildName
			);

			return TRUE;
		}
		
#endif
		GCAnsCsAccessCrownState (lpObj->m_Index, 0);
		g_CastleSiege.SetCrownUserIndex(lpObj->m_Index);
		g_CastleSiege.SetCrownAccessUserX(lpObj->X);
		g_CastleSiege.SetCrownAccessUserY(lpObj->Y);
		
		g_CastleSiege.SetCrownAccessTickCount();
		
		// 공성맵 안의 사람들에게 왕의 직인이 등록 중임을 알린다.
		g_CastleSiege.NotifyAllUserCsProgState (0, lpObj->GuildName);

		LogAddTD("[CastleSiege] [%s][%s] Start to Register Castle Crown (GUILD:%s)",
			lpObj->AccountID,
			lpObj->Name,
			lpObj->GuildName
			);
	}
	else 
	{
		if (lpObj->m_Index != iUserIndex) 
		{		// 등록한 사람이 자신이라면 패킷을 보내지 않는다.
			GCAnsCsAccessCrownState (lpObj->m_Index, 3);
		}
	}
#endif

	return TRUE;
}


BOOL NpcCastleSwitch	(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	if (!gObjIsConnected(lpObj->m_Index)) {
		return TRUE;
	}

	// 1 . 공성 시간인지 확인한다.
	if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE) {
		return TRUE;
	}

	// 2 . 공성측 주측길드 인지 확인한다.
	if (lpObj->m_btCsJoinSide < CS_SIEGESIDE_ATTACK // || lpObj->m_bCsGuildInvolved == 0
		) 
	{
		return TRUE;
	}

	// 3 . 왕관 스위치와의 거리를 체크한다.
	if (abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3 ) {
		return TRUE;
	}

#ifdef MODIFY_CASTLE_CROWN_ACCESS_BY_CHECKING_STATUE_20051219
	// 수호 석상이 남아 있으면, 등록 할 수 없음
	if( g_CastleSiege.CheckGuardianStatueExist() == TRUE )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( lpObj->m_Index, lMsg.Get(1736) );			// "수호 석상이 남아 있습니다."
		return TRUE;
	}
#endif

	// 4 . 현재 스위치에 등록 중인 사용자가 없다면 이 케릭을 등록한다.
	INT iUserIndex	= g_CastleSiege.GetCrownSwitchUserIndex(lpNpc->Class);
	if (!gObjIsConnected(iUserIndex)) {
		GCAnsCsAccessSwitchState (lpObj->m_Index, lpNpc->m_Index, -1, 1);
		g_CastleSiege.SetCrownSwitchUserIndex(lpNpc->Class, lpObj->m_Index);

		// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
		LogAddTD("[CastleSiege] [%s][%s] Start to Push Castle Crown Switch (GUILD:%s) - CS X:%d/Y:%d",
			lpObj->AccountID,
			lpObj->Name,
			lpObj->GuildName,
			lpNpc->X,
			lpNpc->Y
			);
	}
	else {
		if (lpObj->m_Index != iUserIndex) {		// 등록한 사람이 자신이라면 패킷을 보내지 않는다.
			GCAnsCsAccessSwitchState (lpObj->m_Index, lpNpc->m_Index, iUserIndex, 2);
		}
	}
#endif

	return TRUE;
}


#endif


#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111

BOOL NpcCastleHuntZoneGuard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}

//	MsgOutput( lpObj->m_Index, "[테스트]사냥터 문지기에게 대화 요청");

	PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE pResult = {0,};

	PHeadSubSetB((LPBYTE)&pResult, 0xB9, 0x03, sizeof( pResult) );
	pResult.btResult		= CASTLE_HUNTZONE_ENTRANCE_ENABLE;		// 일반인
	pResult.iMaxPrice		= MAX_CS_TAX_HUNT_ZONE;
	pResult.iUnitOfPrice	= UNIT_CS_TAX_HUNT_ZONE;
	pResult.btUsable		= CASTLE_HUNTZONE_USAGE_DISABLE;

	// 1. 현재 사냥터 사용 비용
	pResult.iCurrentPrice	= 0;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
	pResult.iCurrentPrice	= g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index, FALSE);
#endif

	// 2. 일반인 사냥터 사용 가능 여부
	if( g_CastleSiege.GetHuntZoneEnter() )
		pResult.btUsable	= CASTLE_HUNTZONE_USAGE_ENABLE;

	// 3. 성 사냥터 사용 요청에 대한 결과
	if( lpObj->lpGuild != NULL )
	{
		// 3-1. 성주 연합원 검사
		if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) )
		{
			pResult.btResult = CASTLE_HUNTZONE_ENTRANCE_ENABLE_TO_MEMBER;
		}

		// 3-2. 성주 연합마스터인지 검사
		if( g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) )
		{
			if( lpObj->GuildStatus == G_MASTER )
			{
				pResult.btResult = CASTLE_HUNTZONE_ENTRANCE_ENABLE_TO_OWNER;			
			}
		}
	}
	else
	{
		if( g_CastleSiege.GetHuntZoneEnter() )
		{
			pResult.btResult = CASTLE_HUNTZONE_ENTRANCE_ENABLE;
		}
	}


	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

//	lpObj->m_IfState.use    = 1;
//	lpObj->m_IfState.type   = I_CS_HUNTZONE_GUARD;

	return TRUE;
}

#endif

#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
BOOL NpcShadowPhantom(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

#ifdef UPDATE_SHADOW_PHANTOM_20070302		// 쉐도우팬텀 공방 보완
#ifdef UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220	// 쉐도우팬텀 최대레벨 수정
	if(lpObj->Level > g_ShadowPahtomMaxLevel || lpObj->ThirdChangeUp)	// 마스터레벨은 공방 제외
#else	// UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220
	if(lpObj->Level > 220)
#endif	// UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220
	{
		// 당신은 이미 그 이상의 힘을 가지고 있습니다...
		GCServerCmd(lpObj->m_Index, NPC_SHADOW_PHANTOM_MSGBOX, 0, 0);
		return TRUE;
	}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	int iIncleaseAttackPower = 0;
	int iIncleaseDefensePower = 0;
	int iUsedTime = 0;

	if( lpObj->Level <= 180 )
	{
		iIncleaseAttackPower = ( lpObj->Level / 3 ) + 45;
		iIncleaseDefensePower = ( lpObj->Level / 5 ) + 50;
	}
	else
	{
		iIncleaseAttackPower = 105;
		iIncleaseDefensePower = 86;
	}

	iUsedTime = ( BUFFTIME_MINUTE * ( 30 + (int)(lpObj->Level / 6) ) );

	gObjAddBuffEffect( lpObj, BUFFTYPE_ATTDEF_POWER_INC, EFFECTTYPE_IMPROVE_DAMAGE, iIncleaseAttackPower, EFFECTTYPE_IMPROVE_DEFENSE, iIncleaseDefensePower, iUsedTime );
#else
	if(lpObj->Level <= 180)
	{
		lpObj->m_iSkillNPCHelpTime	= GetTickCount();
		lpObj->m_iSkillNPCDefense	= (lpObj->Level / 5) + 50;
		lpObj->m_iSkillNPCAttack	= (lpObj->Level / 3) + 45;
	}
	else	// 181레벨 ~ 220레벨 까지의 공방 수치는 180레벨과 동일
	{
		lpObj->m_iSkillNPCHelpTime = GetTickCount();
		lpObj->m_iSkillNPCDefense	= 86;
		lpObj->m_iSkillNPCAttack	= 105;
	}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#else	// UPDATE_SHADOW_PHANTOM_20070302

#ifdef UPDATE_LOWLEVEL_SUPPORT_1_20051215
	// 20051209 공방 NPC 레벨 제한 상향조정 - hnine
	if(lpObj->Level > 180)
	{
		// 당신은 이미 그 이상의 힘을 가지고 있습니다...
		GCServerCmd(lpObj->m_Index, NPC_SHADOW_PHANTOM_MSGBOX, 0, 0);
		return TRUE;
	}

	// 20051209 : 공방 NPC 레벨 상향 조정에 따른 공/방어력 향상 공식 조정
	lpObj->m_iSkillNPCHelpTime = GetTickCount();
	lpObj->m_iSkillNPCDefense = (lpObj->Level / 5) + 50;
	lpObj->m_iSkillNPCAttack = (lpObj->Level / 3) + 45;
#else
	if(lpObj->Level > 100)
	{
		// 당신은 이미 그 이상의 힘을 가지고 있습니다...
		GCServerCmd(lpObj->m_Index, NPC_SHADOW_PHANTOM_MSGBOX, 0, 0);
		return TRUE;
	}

	lpObj->m_iSkillNPCHelpTime = GetTickCount();
	lpObj->m_iSkillNPCDefense = (lpObj->Level / 2) + 20;
	lpObj->m_iSkillNPCAttack = (lpObj->Level / 3) + 40;
#endif
#endif	// UPDATE_SHADOW_PHANTOM_20070302

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004			// !! NOT
	ADD_VIEWSKILLSTATE(lpObj->m_ViewSkillState, STATE_NPC_HELP);
	GCStateInfoSend(lpObj, 1, STATE_NPC_HELP);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	return TRUE;
}
#endif


#ifdef CRYWOLF_NPC_WORK_20050912
BOOL NpcCrywolfAltar(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	// 여기서 처리할 필요 없음! Protocol로 요청 오면 처리!!


	// 크라이울프 전투 시간인지 체크
//	g_CrywolfNPC_Altar.SetAltarUserIndex( lpNpc->m_Index, lpNpc->Class, lpObj->m_Index );


	return TRUE;
}
#endif

#ifdef UPDATE_QUARTERMASTER_NPC_20070122	// 물자보급관 파멜라, 안젤라 추가(goni)
BOOL NpcQuartermasterPamella(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	return FALSE;
}

BOOL NpcQuartermasterAngela(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	return FALSE;
}

#endif	// UPDATE_QUARTERMASTER_NPC_20070122

#ifdef ADD_FIRECRACKER_NPC_20070306			// 폭죽판매 NPC 대화
BOOL NPcFireCrackerStore(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	return FALSE;
}
#endif	// ADD_FIRECRACKER_NPC_20070306

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328			// 환영 사원 이벤트  NPC 대화

// 석상
BOOL NpcIllusionHolyRelicsStatue(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	g_IllusionTempleEvent.IllusionHolyRelicsStatusAct(lpObj->MapNumber, lpNpc, lpObj);

	return TRUE;
}

// 성물보관함(연합군)
BOOL NpcHolyRelicsBoxAllied(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	g_IllusionTempleEvent.HolyRelicsBoxAlliedAct(lpObj->MapNumber,lpNpc, lpObj);

	return TRUE;
}

// 성물보관함(환영교)
BOOL NpcHolyRelicsBoxIllusion(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	g_IllusionTempleEvent.HolyRelicsBoxIllusionAct(lpObj->MapNumber, lpNpc, lpObj);

	return TRUE;
}

// 미라쥬
BOOL NpcPlatformMirage(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	g_IllusionTempleEvent.PlatformMirageAct(lpNpc, lpObj);

	return TRUE;
}
#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// 엘베란드 NPC 대화
// 실비아
BOOL NpcSilviaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)		
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	return FALSE;
}

// 레아
BOOL NpcRheaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)		
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	return FALSE;
}

// 마르세
BOOL NpcMarceTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)		
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	return FALSE;
}
#endif	// ADD_SEASON_3_NEW_MAP_20070910

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
BOOL NpcSeedMaster(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( bCanChaosBox == TRUE )
	{

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
		if( lpObj->m_IfState.use > 0 )
		{	// 창을 사용하고 있을때는.. 사용할수 없다
			return TRUE;
		}
#endif

#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		if (lpObj->m_bPShopOpen == true) {
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "개인상점 개설 중에서는 카오스 조합을 사용하실 수 없습니다."
			return TRUE;
		}
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804

		PMSG_TALKRESULT			pResult;

		pResult.h.c				= PMHCE_BYTE;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof( pResult );
		pResult.result			= 0x17;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);	

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
		// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
		lpObj->m_IfState.use    = 1;
		lpObj->m_IfState.type   = I_CHAOSBOX;
#endif

		LogAddTD("[JewelMix] [%s][%s] Socket Item Mix Start", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}

BOOL NpcSeedResearcher(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( bCanChaosBox == TRUE )
	{

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
		if( lpObj->m_IfState.use > 0 )
		{	// 창을 사용하고 있을때는.. 사용할수 없다
			return TRUE;
		}
#endif
		
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		if (lpObj->m_bPShopOpen == true) {
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "개인상점 개설 중에서는 카오스 조합을 사용하실 수 없습니다."
			return TRUE;
		}
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804

		PMSG_TALKRESULT			pResult;

		pResult.h.c				= PMHCE_BYTE;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof( pResult );
		pResult.result			= 0x18;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);	

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
		// 성공했을때. 인터페이스 사용한다는 체크를 해둔다
		lpObj->m_IfState.use    = 1;
		lpObj->m_IfState.type   = I_CHAOSBOX;
#endif

		LogAddTD("[JewelMix] [%s][%s] Socket Item Mix Start", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
BOOL NpcCherryTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index))
	{
		return TRUE;
	}
	return FALSE;
}
#endif	// ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
//시즌 4 체험서버 스탯초기화 NPC
BOOL NpcStatResetTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index))
	{
		return TRUE;
	}
	
	PMSG_TALKRESULT			pResult;
	
	pResult.h.c				= PMHCE_BYTE;
	pResult.h.headcode		= 0x30;
	pResult.h.size			= sizeof( pResult );
	pResult.result			= 0x19;
	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);	
	
	return TRUE;
}
#endif // EXPERIENCE_SERVER_NPC_STAT_RESET

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
BOOL NpcLuckyCoinDelgado( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj )
{
	if (!gObjIsConnected(lpObj->m_Index))
	{
		return TRUE;
	}

	return FALSE;
}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

//------------->
//20081013 해외 크리스마스 이벤트 - NPC (grooving)
#ifdef ADD_NPC_XMASEVENT_20080930
// 산타마을 방문시 선물 지급 NPC
BOOL Npc_SantaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index))
	{
		return TRUE;
	}
#ifdef ADD_PAYITEM_20081030
	GEReqCheckEventNPCTarget( lpObj->m_Index, EVENT_XMAS);
	
#endif
	return TRUE;
}
// 데비아스 마을로 돌려 보내주는 NPC
BOOL Npc_SnowManTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index))
	{
		return TRUE;
	}
	
	CGReqCheckSnowManNPC( lpObj->m_Index );
	
	return TRUE;
}
// 산타의 마을 버프 NPC
BOOL Npc_LittleSantaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj, int iBuffType)
{
	if (!gObjIsConnected(lpObj->m_Index))
	{
		return TRUE;
	}
	
	switch(iBuffType)
	{
	case LIFE_RECOVERY:
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield) );
		}
		
		break;
	case MANA_RECOVERY:
		{
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			GCManaSend( lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, (WORD)(lpObj->BP) );
		}
		break;
	case BUFFTYPE_SANTA_POWER:
		{
			gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_POWER, EFFECTTYPE_POWER_UP, 30, EFFECTTYPE_NONE, 0, 1800);
		}
		break;
	case BUFFTYPE_SANTA_GUARD:
		{
			gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_GUARD, EFFECTTYPE_GUARD_UP, 100, EFFECTTYPE_NONE, 0, 1800);
		}
		break;
	case BUFFTYPE_SANTA_HEAL:
		{
			//삭제예정
			//gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_HEAL, EFFECTTYPE_MAXLIFE_UP, 500, EFFECTTYPE_NONE, 0, 1800);
			gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_HEAL, EFFECTTYPE_HP, 500, EFFECTTYPE_NONE, 0, 1800);
		}
		break;
	case BUFFTYPE_SANTA_PROTECT:
		{
			//삭제예정
			//gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_PROTECT, EFFECTTYPE_MAXMANA_UP, 500, EFFECTTYPE_NONE, 0, 1800);
			gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_PROTECT, EFFECTTYPE_MANA, 500, EFFECTTYPE_NONE, 0, 1800);
		}
		break;
	case BUFFTYPE_SANTA_SPEED:
		{
			gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_SPEED, EFFECTTYPE_ATTACKSPEED, 15, EFFECTTYPE_NONE, 0, 1800);
		}
		break;
	case BUFFTYPE_SANTA_LUCKY:
		{
			gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_LUCKY, EFFECTTYPE_AG_RECOVERY_UP, 10, EFFECTTYPE_NONE, 0, 1800);
		}
		break;	
	}
	
	return TRUE;
}

#endif //ADD_NPC_XMASEVENT_20080930
//<-------------

BOOL NpcPkClearCheck(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if ( g_PkClearSystem.GetNPCPkClearData(lpNpc->m_Index) != FALSE )
	{
		g_PkClearSystem.CheckUserCondition(lpObj);
		return TRUE;
	}

	return FALSE;
}

BOOL NpcResetCheck(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if ( g_ResetSystem.GetNPCResetData(lpNpc->m_Index) != FALSE )
	{
		g_ResetSystem.CheckUserCondition(lpObj);
		return TRUE;
	}

	return FALSE;
}

BOOL NpcBuffHelperCheck(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if ( g_BuffHelper.GetNPCData(lpNpc->m_Index) != FALSE )
	{
		g_BuffHelper.CheckUserCondition(lpObj, lpNpc->m_Index);
		return TRUE;
	}

	return FALSE;
}