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

#ifdef UPDATE_QUARTERMASTER_NPC_20070122	// ���ں��ް� �ĸ��, ������ �߰�(goni)
BOOL NpcQuartermasterPamella(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
BOOL NpcQuartermasterAngela(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif

#ifdef ADD_FIRECRACKER_NPC_20070306			// �����Ǹ� NPC ��ȭ
BOOL NPcFireCrackerStore(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328				// �̺�Ʈ�� NPC ��ȭ �Լ�
BOOL NpcIllusionHolyRelicsStatue(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);	// ����
BOOL NpcHolyRelicsBoxAllied(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// ����������(���ձ�)
BOOL NpcHolyRelicsBoxIllusion(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// ����������(ȯ����)
BOOL NpcPlatformMirage(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);				// �̶���
#endif

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// ������ ���� NPC �ٷ� ��ȭ �Լ�
BOOL NpcAlan( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// �������� NPC ��ȭ
BOOL NpcSilviaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// �Ǻ��
BOOL NpcRheaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// ����
BOOL NpcMarceTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// ������
#endif

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
BOOL NpcCherryTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// ������ ����
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
BOOL NpcSeedMaster(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// �õ� ������
BOOL NpcSeedResearcher(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);	// �õ� ������
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#define NPC_POSIONGIRL				253		// �����Ĵ� �ҳ�
#define NPC_DEVIASWIZARD			245		// ����ƽ� ������
#define NPC_DEVIASGUARD				247		// ����ƽ� ���
#define NPC_DEVIASWAREHOUSEMEN		240		// ����ƽ�&&�븮�� â������

#define NPC_NORIARARA				242		// �븮�� ������

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
BOOL NpcChaosCardMaster( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
BOOL NpcStatResetTalk( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj );	//���� 4 ü�輭�� �����ʱ�ȭ NPC
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
// 0 : ���
// 1 : ���� ���
int NpcTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	int npcnum = lpNpc->Class;
	if( npcnum < 0 ) return 0;
	
#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
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
	// ����Ʈ�� ���õ� NPC�� üũ�Ѵ�
	if( NpcQuestCheck(lpNpc, lpObj) )
		return 1;
#endif

	switch( npcnum )
	{
	case NPC_FASI : //	������ �Ľ�
		if( NpcFasi(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_GUILDMASTER :
		if( NpcGuildMasterTalk(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_BATTLEANNOUNCER :
		if( NpcBattleAnnouncer(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_ROLENSIAGUARD: // �η��þ� ���
		if( NpcRolensiaGuard(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_CHAOSGOBLELIN: // ī���� ���
		if( NpcChaosGoblelin(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_RUSIPHER: // �����(ī��)
		if( NpcRusipher(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_POSIONGIRL: // �����Ĵ� �ҳ�
		if( NpcPosionGirl(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_DEVIASWIZARD: // ����ƽ� ������
		if( NpcDeviasWizard(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_DEVIASGUARD: // ����ƽ� ���
		if( NpcDeviasGuard(lpNpc, lpObj) == TRUE ) return 1;
		break;

		
	case NPC_DEVIASWAREHOUSEMEN: // ����ƽ�&&�븮�� â������
		//if( NpcDeviasWareHousemen(lpNpc, lpObj) == TRUE ) return 1;
		if( NpcWarehouse(lpNpc, lpObj) == TRUE ) return 1;
		break;

#ifdef DARKLORD_WORK
	case NPC_DARKSPRIT_TRAINER:		// ��ũ���Ǹ� ���û�
		if( NpcDarkSpiritTrainer(lpNpc, lpObj) == TRUE) return 1;		
		break;
#endif
		

	case NPC_NORIARARA: // �븮�� ������
		if( NpcNoriaRara(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_DEVIASMADAM: // ����ƽ� ��������
		if( NpcDeviasMadam(lpNpc, lpObj) == TRUE ) return 1;
		break;

	case NPC_EVETNCHIPNPC: // �̺�ƮĨ NPC
		if( NpcEventChipNPC(lpNpc, lpObj) == TRUE ) return 1;
		break;

#ifdef JAPAN_NPC_NEWTALK
	case NPC_RELENSIASMITH: // �η��þ� ��������
		if( NpcRorensiaSmith(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_NORIAJANGIN:
		if( NpcNoriJangIn(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef GAMESERVER_DIVISION
		case NPC_SERVERDIVISION: // �η��þ� ��������
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
	case NPC_ROLENSIASMITH: // �η��þ� �������� <- �Ϻ��ſ��� ����
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
	case NPC_KALIMA_REIRA:	// Į���� �Ű� ���̶�
		if( NpcReira( lpNpc, lpObj ) == TRUE ) return 1;
		break;
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	case JEWELOFHARMONY_NPC_PURITY:
	case JEWELOFHARMONY_NPC_SMELT:
	case JEWELOFHARMONY_NPC_RESTORE:
		// ��ȭ�Ǻ��� ���� NPC�� ���� ó���Ѵ�.
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
	case NPC_KANTURU_MACHINE:	// ĭ���� NPC - ���԰�����ġ
		if( NpcMainatenceMachine( lpNpc, lpObj ) == TRUE ) return 1;
		break;		
#endif

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822
	case NPC_WHITEANGEL_EVENT:	// ȭ��Ʈ���� ������ ���� �̺�Ʈ
		if( NpcWhiteAngelEvent( lpNpc, lpObj ) == TRUE )	return 1;
		break;
#endif

#ifdef UPDATE_QUARTERMASTER_NPC_20070122	// ���ں��ް� �ĸ��, ������ �߰�(goni)
	case NPC_QUARTERMASTER_PAMELLA:
		if(NpcQuartermasterPamella(lpNpc, lpObj) == TRUE ) return 1;
		break;
	case NPC_QUARTERMASTER_ANGELA:
		if(NpcQuartermasterAngela(lpNpc, lpObj) == TRUE ) return 1;
		break;
#endif

#ifdef ADD_FIRECRACKER_NPC_20070306			// �����Ǹ� NPC ��ȭ
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

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// ������ ���� NPC �ٷ��� ��ȭ
	case NPC_ALAN:				
		if( NpcAlan( lpNpc, lpObj ) == TRUE )	return 1;
		break;
#endif		

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// �������� NPC ��ȭ
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
		//20081013 �ؿ� ũ�������� �̺�Ʈ - NPC (grooving)
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
	case NPC_LITTLE_SANTA_3://ü�� 100% ȸ��
		if( Npc_LittleSantaTalk(lpNpc, lpObj, LIFE_RECOVERY) == TRUE)
		{
			return 1;
		}
		break;
	case NPC_LITTLE_SANTA_4://���� 100% ȸ��
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

			// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
			lpObj->m_IfState.use    = 1;
			lpObj->m_IfState.type   = I_ETCWINDOW;

			return 1;
		}
		break;
#endif // ADD_NEWPVP_PKFIELD

	}
	return 0;
}

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823	// �ٷ����� ��ȭ
BOOL NpcAlan( LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj )
{
	// �ŷ��� �̰ų� ���� �̿� �� �϶��� ����� �� ����.
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
	// �ŷ��� �̰ų� ���� �̿� �� �϶��� ����� �� ����.
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
#ifdef CASTLE_POTION_NPC_20041222			// ������ �����Ĵ� NPC
	if (lpNpc->MapNumber == MAP_INDEX_CASTLESIEGE) {
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
			if (lpObj->m_btCsJoinSide != CS_SIEGESIDE_DEFEND) {
				INT iChatRnd	= rand() % 3;
				switch (iChatRnd) {
				case 0 :
					ChatTargetSend(lpNpc, "������ ������ �ȼ��� ���� !!", lpObj->m_Index);
					break;
				case 1 :
					ChatTargetSend(lpNpc, "������ ���� ������������ ���Ұ��̴� !!", lpObj->m_Index);
					break;
				case 2 :
					ChatTargetSend(lpNpc, "���� ������ �ִ� !! ��� !!", lpObj->m_Index);
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
			case 0 : ChatTargetSend(lpNpc, lMsg.Get(1122), lpObj->m_Index);	// "�����ڿ��� �ƹ��͵� �� �� ����!!"
				break;
			case 1 : ChatTargetSend(lpNpc, lMsg.Get(1123), lpObj->m_Index);	// ������ ���̻� ������ �ִ°�?
				break;
			case 2 : ChatTargetSend(lpNpc, lMsg.Get(1124), lpObj->m_Index);	// �ٸ��� ���� �˾ƺ���
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
		ChatTargetSend(lpNpc, "�������� �Ⱓ���� ��带 �����Ҽ��� �����ϴ�", lpObj->m_Index); // ��带 ���̻� ������� ����
		//ChatTargetSend(lpNpc, lMsg.Get(1119), lpObj->m_Index); // ��带 ���̻� ������� ����
		return TRUE;
	}

	/*if( lpObj->Level < 100 )
	{
		int t=rand()%3;
		switch( t )
		{
		case 0 : ChatTargetSend(lpNpc, "����� ��帶���Ͱ� �ǰڴٰ�? ����~", lpObj->m_Index);	break;
		case 1 : ChatTargetSend(lpNpc, "��帶���Ͱ� �Ƿ��� ���� �־���..", lpObj->m_Index);	break;
		case 2 : ChatTargetSend(lpNpc, "��帶���Ͱ� �Ƿ��� ������ 100�� �Ǿ�� �Ѵٳ�..", lpObj->m_Index);	break;
		}
	}
	else*/
	{
		//ChatTargetSend(lpNpc, "111", lpObj->m_Index);
		//ChatTargetSend(lpNpc, "�������� ��� +7������ �Ǿ�� �ϳ�.", lpObj->m_Index);
		if( lpObj->GuildNumber > 0 )
		{
			ChatTargetSend(lpNpc, lMsg.Get(1140), lpObj->m_Index);	// "����� �̹� ��忡 ���� ���� �ʽ��ϱ�?"
			return TRUE;
		}
#ifdef _NEW_EXDB_
		int nRet = gObjGuildMasterCapacityTest(lpObj);
		if( nRet == 0 ) 
		{
			ChatTargetSend(lpNpc, lMsg.Get(1141), lpObj->m_Index); // ������ 100 �̻��� �Ǿ�� �մϴ�
		}
		else if( nRet == 2 ) 
		{
			ChatTargetSend(lpNpc, lMsg.Get(1142), lpObj->m_Index);	// ������ �������� ��� +7������ �Ǿ�� �մϴ�
		}
		else 
#endif
		{
			GCGuildMasterQuestionSend(lpObj->m_Index);			
		}
	}
	return TRUE;
}


//�η��þ� ���
BOOL NpcRolensiaGuard(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	// Message�ڽ��� �����޶�� ��û�Ѵ�
	// MsgBox Index = 0 : ���� �������� ���ƴٴϰ� �ִٴ� �ҹ��� ������ �־�, �������� �����ϴ� �� �˾Ҵµ�.. �׵��� �� ���� �� ���� ��Ÿ���ɱ�?
	// MsgBox Index = 1 : �Ǹ��� ���忡 ���� �˰� �ʹٸ� ����۸� ã�ƺ���.. 
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
	{	// �̺�Ʈ �������� ������
		// MsgBox Index = 3 :"������ ������ �������̱���. ��ſ��� ����� ������ �Բ� ���شٸ� ����� ��� �˴ϴ�."

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
		GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "���λ��� ���� �߿����� ī���� ������ ����Ͻ� �� �����ϴ�."
		return FALSE;
	}	
#endif // MODIFY_PCS_CHAOSMIX_PSHOP_BUGFIX

	if( bCanChaosBox == TRUE )
	{
		if( lpObj->m_IfState.use > 0 )
		{	// â�� ����ϰ� ��������.. ����Ҽ� ����
			return FALSE;
		}

	#ifdef MODIFY_CHAOSCARD_MIX_BLOCK_PSHOP_USE_20070713
		#ifdef PERSONAL_SHOP_20040113		// �ڽ��̳� ������ ���λ����� �������̸� ī���� ������ �� �� ����.
		if (lpObj->m_bPShopOpen == true) {
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "���λ��� ���� �߿����� ī���� ������ ����Ͻ� �� �����ϴ�."
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

		// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
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
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
		return TRUE;
	}
#endif

	if( !gUserFindDevilSquareInvitation(aIndex) )
	{	// �ʴ���� ����
		GCServerCmd(lpObj->m_Index, 1, NPC_RUSIPHER_MSGBOX, 0);
	}
	else
	{
		if( g_DevilSquare.GetState() == DevilSquare_OPEN )
		{	// �Ǹ��� �������� ���� �ִ� â�� ����� �Ѵ�
			PMSG_TALKRESULT		pResult;

			pResult.h.c				= PMHCE_BYTE;
			pResult.h.headcode		= 0x30;
			pResult.h.size			= sizeof( pResult );
			pResult.result			= 0x04;
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
			// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
			lpObj->m_IfState.use    = 1;
			lpObj->m_IfState.type   = I_ETCWINDOW;
#endif
		}		
		else
		{	// �Ǹ��� ���忡 ��� ���� ���� �ð��̴�
			GCServerCmd(lpObj->m_Index, 1, NPC_RUSIPHER_MSGBOX+1, 0);
		}
	}
	return TRUE;
}

BOOL NpcPosionGirl(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef CASTLE_POTION_NPC_20041222			// ������ �����Ĵ� NPC
	if (lpNpc->MapNumber == MAP_INDEX_CASTLESIEGE) {
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
			if (lpObj->m_btCsJoinSide == CS_SIEGESIDE_DEFEND) {
				INT iChatRnd	= rand() % 3;
				switch (iChatRnd) {
				case 0 :
					ChatTargetSend(lpNpc, "������ �� ������ �����.", lpObj->m_Index);
					break;
				case 1 :
					ChatTargetSend(lpNpc, "���� ������� ������ �Ҹ��� �����ſ���.", lpObj->m_Index);
					break;
				case 2 :
					ChatTargetSend(lpNpc, "���� ������ �־��. ���� �� �����ּ��� !!", lpObj->m_Index);
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

// â�� ���� ����
BOOL NpcWarehouse(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef JAPAN_NPC_NEWTALK
	if( lpNpc->MapNumber == 0 )
	{	// �η��þ� â������
		GCServerCmd(lpObj->m_Index, 5, 2, 0);
	}
	else if( lpNpc->MapNumber == 3 )
	{	// ����ƽ� â������
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
		wsprintf(szTemp, "â�� ����ϱ� ���ؼ� %d���� �ʿ��ϳ�..", rZen);
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
	// �ŷ��� �̰ų� ���� �̿� �� �϶��� ����� �� ����.
	if( lpObj->m_IfState.use > 0 )
	{
		return TRUE;
	}
	
// comment_alice_050113 : NO JAPAN!!!! See the define.h
#ifdef EVENT_RENA_FOR_CASTLE_SIEGE_20041116
	PMSG_REQ_VIEW_EC_MN	pForCastleMsg;	// ������ �������� ��� ������ �̺�Ʈ ����
	PHeadSetB((LPBYTE)&pForCastleMsg, 0x01, sizeof(pForCastleMsg));	
	
	pForCastleMsg.iINDEX = lpObj->m_Index;
	strcpy(pForCastleMsg.szUID, lpObj->AccountID);
	
	DataSendEventChip((char*)&pForCastleMsg, sizeof(pForCastleMsg));

	lpObj->m_IfState.type = I_EVENTCHIP;	
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;

	return TRUE;

#endif // EVENT_RENA_FOR_CASTLE_SIEGE_20041116

#ifdef EVENT_RENA_20040909 // ���� �̺�Ʈ
	if (lpNpc->MapNumber == MAP_INDEX_RORENCIA) // �η��þ�
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
	
#ifdef EVENT_STONE_OF_FRIENDSHIP_20040206  // ������ �� �̺�Ʈ
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
	if (lpNpc->MapNumber == MAP_INDEX_DEVIAS) // ����ƽ�
	{
		PMSG_EVENTCHIPINFO	Result;
		PHeadSetB((LPBYTE)&Result, 0x94, sizeof(Result));

		Result.Type = 2; // ���� �̺�Ʈ ����
		
		DataSend(lpObj->m_Index, (LPBYTE)&Result, Result.h.size);

		lpObj->m_IfState.type = I_EVENTCHIP;	
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}

#endif // EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO

#ifdef EVENT_STONE // ���� �̺�Ʈ
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

#ifdef JAPAN_NPC_NEWTALK	// 2004-01-15 0.97 �Ϻ� �߰�
	// �η��þƴ� ó���ΰ�? ...
	GCServerCmd(lpObj->m_Index, 5, 7, 0);	
#endif

//	GCServerCmd(lpObj->m_Index, 5, 0, 0);		// �Ϻ� 0.97 ��ġ�� ���� �ּ�ó�� (2004.01.15)	
	return FALSE;
}

#ifdef PCBANG_COUPON_EVENT_20060124
BOOL NpcCouponEvent(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	// �ŷ��� �̰ų� ���� �̿� �� �϶��� ����� �� ����.
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

// �������� NPC
BOOL NpcServerDivision(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( !gEnableServerDivision )
		return TRUE;

	if( lpObj->lpGuild )
	{	// ��忡 ���ԵǾ� �ִ� ���¿����� �̵��Ҽ� ����
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
		GCServerCmd(lpObj->m_Index, 1, 24, 0);								// �ٸ��� �ε����� Ʋ���ٸ� �� ã�ƺ���� ���Ѵ�.
		return FALSE;
	}

	LogAddTD ("[Blood Castle] (%d) [%s][%s] Talk to Angel King",
		lpObj->m_cBloodCastleIndex+1,
		lpObj->AccountID,
		lpObj->Name
		);

	if (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bBC_REWARDED) {
		// �� �ٸ��� �̹� ������ �̷�����ٸ� ����Ʈ�� �Ϸ�� ���̴�. -> �����ߴٰ� �����ش�.
		GCServerCmd(lpObj->m_Index, 1, 46, 0);
		return FALSE;
	}

	if ((g_BloodCastle.GetCurrentState(lpObj->m_cBloodCastleIndex) != BLOODCASTLE_STATE_PLAYING) ||
		(!g_BloodCastle.CheckPlayStart(lpObj->m_cBloodCastleIndex))
		) 
	{
		GCServerCmd(lpObj->m_Index, 1, 24, 0);								// ���ð��� �ƴϸ� �� ã�ƺ���� ���Ѵ�.
		return FALSE;
	}

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	if (lpObj->m_bBloodCastleComplete == true) {							// �̹� �Ϸ��ߴٸ� �����ߴٰ� �����ش�.
		// �̶� ������ �̷������ �ʾ��� ���� �ִ�.
		GCServerCmd(lpObj->m_Index, 1, 46, 0);			// !!! . ���ο� �޽���
//		GCServerCmd(lpObj->m_Index, 1, 49, 0);			// !!! . ���ο� �޽���
		return FALSE;
	}
#endif
	
	if ( CHECK_LIMIT(iITEM_LEVEL = g_BloodCastle.CheckQuestItem(lpObj->m_Index), 3) ) {
		if (lpNpc->m_cBloodCastleIndex != lpObj->m_cBloodCastleIndex) {		// ���� ����ĳ�� ������ �ƴ϶�� �� ã�ƺ���� ���Ѵ�.
			GCServerCmd(lpObj->m_Index, 1, 24, 0);
			return FALSE;
		}

		if (!g_BloodCastle.CheckUserBridgeMember(lpObj->m_cBloodCastleIndex, lpObj->m_Index)) {
			GCServerCmd(lpObj->m_Index, 1, 24, 0);							// �ش� �ٸ��� ����� �ƴϸ� �� ã�ƺ���� ���Ѵ�.
			return FALSE;
		}

		// �ش� �ɸ��� ����ڷ� �����Ѵ�.
		g_BloodCastle.SetUserState(lpObj->m_Index, BC_USER_WINNER);

#ifndef BLOODCASTLE_EVENT_5TH_20050531
		// �����ߴٰ� �����ش�.
		GCServerCmd(lpObj->m_Index, 1, 23, 0);
#endif

		// ����Ʈ�� ������ ����ڿ� ��Ƽ��ȣ�� �����Ѵ�.
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

			// ���� ������� �ٸ��� ����� ��Ƽ���� �ִ��� Ȯ���Ѵ�.
			if (g_BloodCastle.CheckPartyExist(lpObj->m_Index) == false) {
				// �� ����� ��Ƽ���� ������ ����ó��
				
/*
				// ��� �ɸ��� �ִ� �ٸ��� ������ �Ѵ�.
				g_BloodCastle.GiveReward_Win(lpObj->m_Index, lpObj->m_cBloodCastleIndex);

				// ����Ʈ�� �����Ѵ�.
				g_BloodCastle.SetState(lpObj->m_cBloodCastleIndex, BLOODCASTLE_STATE_PLAYEND);
*/

				// �����ߴٰ� �����ش�.
				GCServerCmd(lpObj->m_Index, 1, 23, 0);		// !!! . ���ο� �޽���
//				GCServerCmd(lpObj->m_Index, 1, 47, 0);		// !!! . ���ο� �޽���

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
					g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
					lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
					lpObj->AccountID,
					lpObj->Name
					);
			}
			else {
				// �� ����ؾ� �� ��Ƽ���� ����

				// �����ߴµ� ��ٷ��� �Ѵٰ� �����ش�.
				GCServerCmd(lpObj->m_Index, 1, 23, 0);		// !!! . ���ο� �޽���
//				GCServerCmd(lpObj->m_Index, 1, 47, 0);		// !!! . ���ο� �޽���

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
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
			// �̹� ����� ���� ? -> �ɰ��� ����
			LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Winner Already Exist !!",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
				lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
				lpObj->AccountID,
				lpObj->Name
				);
			return FALSE;
		}

		// ����Ʈ �������� ã�Ƽ� �����ش�.
		g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
#else
		// ��� �ɸ��� �ִ� �ٸ��� ������ �Ѵ�.
		g_BloodCastle.GiveReward_Win(lpObj->m_Index, lpObj->m_cBloodCastleIndex);

		// ����Ʈ�� �����Ѵ�.
		g_BloodCastle.SetState(lpObj->m_cBloodCastleIndex, BLOODCASTLE_STATE_PLAYEND);

		// ����Ʈ �������� ã�Ƽ� �����ش�.
		g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);

		LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
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
		// ���� ���빫�⸦ ������ ���� �ʴٸ� ������ �ִ� ����� ���� ��Ƽ������ Ȯ���Ѵ�.
		if (g_BloodCastle.CheckUserWinnerParty(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == true) {
			lpObj->m_bBloodCastleComplete = true;

			// ���� ������� �ٸ��� ����� ��Ƽ���� �ִ��� Ȯ���Ѵ�.
			if (g_BloodCastle.CheckPartyExist(lpObj->m_Index) == false) {
				// �� ����� ��Ƽ���� ������ ����ó��
				
/*
				// ��� �ɸ��� �ִ� �ٸ��� ������ �Ѵ�.
				g_BloodCastle.GiveReward_Win(lpObj->m_Index, lpObj->m_cBloodCastleIndex);

				// ����Ʈ�� �����Ѵ�.
				g_BloodCastle.SetState(lpObj->m_cBloodCastleIndex, BLOODCASTLE_STATE_PLAYEND);
*/

				// �����ߴٰ� �����ش�.
				GCServerCmd(lpObj->m_Index, 1, 23, 0);		// !!! . ���ο� �޽���
//				GCServerCmd(lpObj->m_Index, 1, 48, 0);		// !!! . ���ο� �޽���

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
					g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
					lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
					lpObj->AccountID,
					lpObj->Name
					);
			}
			else {
				// �� ����ؾ� �� ��Ƽ���� ����

				// �����ߴµ� ��ٷ��� �Ѵٰ� �����ش�.
				GCServerCmd(lpObj->m_Index, 1, 23, 0);		// !!! . ���ο� �޽���
//				GCServerCmd(lpObj->m_Index, 1, 48, 0);		// !!! . ���ο� �޽���

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register",
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
					g_BloodCastle.GetBridgeIndexByMapNum( lpNpc->MapNumber ) + 1,
#else
					lpNpc->MapNumber - MAP_INDEX_BLOODCASTLE1 +1,
#endif
					lpObj->AccountID,
					lpObj->Name
					);
			}

			// �ٸ� �޽����� �ѷ��־�� �� ��
			
			// ����Ʈ�� �����ߴ��� Ȯ��
			
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
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
		return TRUE;
	}
#endif
	
#ifdef ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411
	// ī�� ������ ������ �� ����.
	int iIndex = lpObj->m_Index;
	if( gObj[iIndex].m_PK_Level >= PK_LEVEL_DEFAULT + 1 )
	{		
#ifdef MODIFY_BLOODCASTLE_PK_USER_ENTER_MESSAGE_20070321	// ī������ ����� ������ �ּ�
		GCServerCmd( iIndex, MSGBOX_ENTERLIMIT_PKUSER_BLOODCASTLE, 0, 0 );	// ī�� ������ ������ �� ���ٴ� �޼����� ������.
#else
		GCServerCmd( iIndex, MSGBOX_ENTERLIMIT_PKUSER_CHAOSCASTLE, 0, 0 );	// ī�� ������ ������ �� ���ٴ� �޼����� ������.
#endif
		return TRUE;
	}
#endif
	

	INT iITEM_LEVEL = 0;

	if ( iITEM_LEVEL = g_BloodCastle.CheckEnterItem(lpObj->m_Index) )
	{
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

#ifdef MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
		// ���� ���䰡 �ִ� ��� ������ äũ�Ͽ� ������ ������ �������� Ȯ���Ѵ�.
		if( iITEM_LEVEL != 10 && g_BloodCastle.CheckEnterLevel( lpObj->m_Index, iITEM_LEVEL-1 ) != 0 )
		{
			// ������ ���� �ʴ� ��� ������䰡 �ִ��� Ȯ���Ѵ�.
			if( g_BloodCastle.CheckEnterFreeTicket( lpObj->m_Index ) == true )
			{
				iITEM_LEVEL = 10;
			}
		}
	#endif // MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627

		if( iITEM_LEVEL == 10 )			// ���� ����� ������ ���
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

		if ((g_BloodCastle.GetCurrentState(iITEM_LEVEL-1) == BLOODCASTLE_STATE_CLOSED) && g_BloodCastle.CheckCanEnter(iITEM_LEVEL-1)) {	// ������ �� ����
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
		else {		// �������� �ִµ� �ð��� �ʵǾ���.
			GCServerCmd(lpObj->m_Index, 1, 20, 0);	
		}
	}
	else {			// �������� ����.
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
#ifdef PERSONAL_SHOP_20040113		// �ڽ��̳� ������ ���λ����� �������̸� ī���� ������ �� �� ����.
		if (lpObj->m_bPShopOpen == true) {
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "���λ��� ���� �߿����� ī���� ������ ����Ͻ� �� �����ϴ�."
			return TRUE;
		}
#endif
		lpObj->m_IfState.type   = I_DARK_TRAINER;
		lpObj->m_IfState.state	= 0;
		lpObj->m_IfState.use    = 1;
		pResult.result     = 0x07;
		
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
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// PMSG_TALKRESULT ����7 ���� ����
		pResult.level7	= gDQChaosSuccessRateLevel7;
#endif

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		
#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219				// ���� ����� ���� (ī��������) �� �����ش�. (���û�� ������ ����)
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
		{	// â�� ����ϰ� ��������.. ����Ҽ� ����
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
		// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
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
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
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
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
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
		
		
		// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
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
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
		return TRUE;
	}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
	// ���� �ð��̶�� �����Ѵ�.
	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
		ChatTargetSend(lpNpc, lMsg.Get(1621), lpObj->m_Index);	//	"����� ���� ��� �� �ִ� �ð��� �ƴմϴ�."
		return TRUE;
	}

	// ��忡 ���ԵǾ����� �ʴٸ� �����Ѵ�.
	if (!strcmp(lpObj->GuildName, "")) {
		ChatTargetSend(lpNpc, lMsg.Get(1622), lpObj->m_Index);	//	"����� ���� ��� ������ �����ϴ�."
		return TRUE;
	}

	// �ʿ��ϴٸ� ��û���� ������ üũ�Ѵ�. (������ Ư�� ������ �ƴ϶�� �Ұ�)
	if (g_CastleSiege.CheckGuildOwnCastle(lpObj->GuildName) == FALSE || 
		(lpObj->GuildStatus != G_MASTER && lpObj->GuildStatus != G_SUB_MASTER)
		) 
	{
		ChatTargetSend(lpNpc, lMsg.Get(1622), lpObj->m_Index);	//	"����� ���� ��� ������ �����ϴ�."
		return TRUE;
	}
#endif

	PMSG_TALKRESULT			pResult;
	pResult.h.c				= PMHCE_BYTE;
	pResult.h.headcode		= 0x30;
	pResult.h.size			= sizeof( pResult );
	pResult.result			= 0x0C;

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
	lpObj->m_IfState.use    = 1;
	lpObj->m_IfState.type   = I_ETCWINDOW;
#endif

#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
	// �Ǹ��� ���� ���� Ȯ�� -> level1�� ī���� ���� ���ɿ��η� ������ ������ ����
	pResult.level1	= 0;
	pResult.level2	= 0;
	pResult.level3	= 0;
	pResult.level4	= 0;
#ifdef DEVILSQUARE_EXTEND_20050221		// !!! . �ӽ� -> ī���� ���� �߰�
	pResult.level5	= 0;
	pResult.level6	= 0;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010 // PMSG_TALKRESULT ����7 ���� �߰�
	pResult.level7	= 0;
#endif

	if( bCanChaosBox == TRUE )
	{
		// ���� ������ ������ �ʵǴ��� �� ��� �⺻����� ����� �� �־�� �ϹǷ� �̷��� ó��

		lpObj->m_IfState.type   = I_CHAOSBOX;

#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
		lpObj->bIsChaosMixCompleted = false;			// ī���� ���� ���� '���� ������ �Ϸ���� �ʾҴ�' �� �ʱⰪ�� ����
#endif
		
#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
		lpObj->m_bIsCastleNPCUpgradeCompleted	= false;	// �������� 'NPC�� ���׷��̵� ���� �ʾ���' ���� �ʱⰪ ����
#endif

		pResult.level1	= 1;							// ī���� ������ ��� �����ϴٰ� �˷���

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219				// ���� ����� ���� (ī��������) �� �����ش�. (���û�� ������ ����)
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
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
		return TRUE;
	}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
	// ���� �ð��̶�� �����Ѵ�.
	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
		ChatTargetSend(lpNpc, lMsg.Get(1623), lpObj->m_Index);		//	"����� �� ������ �� �� �ִ� �ð��� �ƴմϴ�."
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
	// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
	lpObj->m_IfState.use    = 1;
	lpObj->m_IfState.type   = I_ETCWINDOW;
#endif

	return TRUE;
}


BOOL NpcCastleGateLever(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
		return TRUE;
	}
#endif

	// 1 . ���� �������̽��� �� ������ �Ǵ��� Ȯ���Ѵ�.
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	BOOL bControlEnable			= FALSE;
	BYTE btResult				= 0;
	INT	iGateIndex				= -1;

	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
		// ������ ���� ��
		if (lpObj->m_btCsJoinSide != CS_SIEGESIDE_DEFEND 
//			|| lpObj->m_bCsGuildInvolved != 1
			) 
		{
			// ������ ����ڰ� �ƴϴ�.
			btResult			= 4;
			bControlEnable		= FALSE;
		}
		else {
			// ������ ������ �� �ִ��� üũ
			bControlEnable		= TRUE;
		}
	}
	else {
		// ������ ���� �ƴ� ��
		if (g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) == FALSE &&
			g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE
			) 
		{
			// ����� ����� �ƴϴ�.
			btResult			= 4;
			bControlEnable		= FALSE;
		}
		else {
			// ������ ������ �� �ִ��� üũ
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
				// ������ �������� �ʴ´�.
				btResult		= 2;
			}
		}
		else {
			// ������ �������� �ʴ´�.
			btResult			= 3;
		}
	}
	
	GCAnsCsGateState (lpObj->m_Index, btResult, iGateIndex);
	
	if (btResult != 1) {
		// ����� ������ ���� ��ȭ ����� ������ �ʴ´�.
		return TRUE;
	}
#else
	// Ȥ�ö� ���� ������ �ʵǾ��� ���� �׳� ����
	return TRUE;
#endif

#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
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

	// 1 . ���� �ð����� Ȯ���Ѵ�.
	if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE) {
		return TRUE;
	}

	// 2 . ���������� Ȯ���Ѵ�.
	if (lpObj->m_btCsJoinSide < CS_SIEGESIDE_ATTACK || lpObj->m_bCsGuildInvolved == 0) {
		return TRUE;
	}
	
	// 3 . ��忬���� ���������� Ȯ���Ѵ�.
	if (!g_CastleSiege.CheckUnionGuildMaster(lpObj->m_Index)) {
		return TRUE;
	}

	// 4 . �հ����� �Ÿ��� üũ�Ѵ�.
	if (abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3 ) {
		return TRUE;
	}

	// 5 . ���� �����Ǿ����� üũ�Ѵ�.
	if (g_CastleSiege.GetRegCrownAvailable() == FALSE) {
		return TRUE;
	}

	// 6 . ���� �հ��� ���� ��� ���� ����ڰ� ���ٸ� �� �ɸ��� ����Ѵ�.
	INT iUserIndex	= g_CastleSiege.GetCrownUserIndex();
	if (!gObjIsConnected(iUserIndex)) 
	{

#ifdef MODIFY_CASTLE_CROWN_ACCESS_ALLOWED_BY_CROWNSWITCH_20051102
		// �հ� ����ġ �۵� ������ ���� �������� �˻��Ѵ�.
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
		
		// ������ ���� ����鿡�� ���� ������ ��� ������ �˸���.
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
		{		// ����� ����� �ڽ��̶�� ��Ŷ�� ������ �ʴ´�.
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

	// 1 . ���� �ð����� Ȯ���Ѵ�.
	if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE) {
		return TRUE;
	}

	// 2 . ������ ������� ���� Ȯ���Ѵ�.
	if (lpObj->m_btCsJoinSide < CS_SIEGESIDE_ATTACK // || lpObj->m_bCsGuildInvolved == 0
		) 
	{
		return TRUE;
	}

	// 3 . �հ� ����ġ���� �Ÿ��� üũ�Ѵ�.
	if (abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3 ) {
		return TRUE;
	}

#ifdef MODIFY_CASTLE_CROWN_ACCESS_BY_CHECKING_STATUE_20051219
	// ��ȣ ������ ���� ������, ��� �� �� ����
	if( g_CastleSiege.CheckGuardianStatueExist() == TRUE )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( lpObj->m_Index, lMsg.Get(1736) );			// "��ȣ ������ ���� �ֽ��ϴ�."
		return TRUE;
	}
#endif

	// 4 . ���� ����ġ�� ��� ���� ����ڰ� ���ٸ� �� �ɸ��� ����Ѵ�.
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
		if (lpObj->m_Index != iUserIndex) {		// ����� ����� �ڽ��̶�� ��Ŷ�� ������ �ʴ´�.
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
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
		return TRUE;
	}

//	MsgOutput( lpObj->m_Index, "[�׽�Ʈ]����� �����⿡�� ��ȭ ��û");

	PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE pResult = {0,};

	PHeadSubSetB((LPBYTE)&pResult, 0xB9, 0x03, sizeof( pResult) );
	pResult.btResult		= CASTLE_HUNTZONE_ENTRANCE_ENABLE;		// �Ϲ���
	pResult.iMaxPrice		= MAX_CS_TAX_HUNT_ZONE;
	pResult.iUnitOfPrice	= UNIT_CS_TAX_HUNT_ZONE;
	pResult.btUsable		= CASTLE_HUNTZONE_USAGE_DISABLE;

	// 1. ���� ����� ��� ���
	pResult.iCurrentPrice	= 0;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
	pResult.iCurrentPrice	= g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index, FALSE);
#endif

	// 2. �Ϲ��� ����� ��� ���� ����
	if( g_CastleSiege.GetHuntZoneEnter() )
		pResult.btUsable	= CASTLE_HUNTZONE_USAGE_ENABLE;

	// 3. �� ����� ��� ��û�� ���� ���
	if( lpObj->lpGuild != NULL )
	{
		// 3-1. ���� ���տ� �˻�
		if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) )
		{
			pResult.btResult = CASTLE_HUNTZONE_ENTRANCE_ENABLE_TO_MEMBER;
		}

		// 3-2. ���� ���ո��������� �˻�
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

#ifdef UPDATE_SHADOW_PHANTOM_20070302		// ���������� ���� ����
#ifdef UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220	// ���������� �ִ뷹�� ����
	if(lpObj->Level > g_ShadowPahtomMaxLevel || lpObj->ThirdChangeUp)	// �����ͷ����� ���� ����
#else	// UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220
	if(lpObj->Level > 220)
#endif	// UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220
	{
		// ����� �̹� �� �̻��� ���� ������ �ֽ��ϴ�...
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
	else	// 181���� ~ 220���� ������ ���� ��ġ�� 180������ ����
	{
		lpObj->m_iSkillNPCHelpTime = GetTickCount();
		lpObj->m_iSkillNPCDefense	= 86;
		lpObj->m_iSkillNPCAttack	= 105;
	}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#else	// UPDATE_SHADOW_PHANTOM_20070302

#ifdef UPDATE_LOWLEVEL_SUPPORT_1_20051215
	// 20051209 ���� NPC ���� ���� �������� - hnine
	if(lpObj->Level > 180)
	{
		// ����� �̹� �� �̻��� ���� ������ �ֽ��ϴ�...
		GCServerCmd(lpObj->m_Index, NPC_SHADOW_PHANTOM_MSGBOX, 0, 0);
		return TRUE;
	}

	// 20051209 : ���� NPC ���� ���� ������ ���� ��/���� ��� ���� ����
	lpObj->m_iSkillNPCHelpTime = GetTickCount();
	lpObj->m_iSkillNPCDefense = (lpObj->Level / 5) + 50;
	lpObj->m_iSkillNPCAttack = (lpObj->Level / 3) + 45;
#else
	if(lpObj->Level > 100)
	{
		// ����� �̹� �� �̻��� ���� ������ �ֽ��ϴ�...
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

	// ���⼭ ó���� �ʿ� ����! Protocol�� ��û ���� ó��!!


	// ũ���̿��� ���� �ð����� üũ
//	g_CrywolfNPC_Altar.SetAltarUserIndex( lpNpc->m_Index, lpNpc->Class, lpObj->m_Index );


	return TRUE;
}
#endif

#ifdef UPDATE_QUARTERMASTER_NPC_20070122	// ���ں��ް� �ĸ��, ������ �߰�(goni)
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

#ifdef ADD_FIRECRACKER_NPC_20070306			// �����Ǹ� NPC ��ȭ
BOOL NPcFireCrackerStore(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	return FALSE;
}
#endif	// ADD_FIRECRACKER_NPC_20070306

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328			// ȯ�� ��� �̺�Ʈ  NPC ��ȭ

// ����
BOOL NpcIllusionHolyRelicsStatue(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	g_IllusionTempleEvent.IllusionHolyRelicsStatusAct(lpObj->MapNumber, lpNpc, lpObj);

	return TRUE;
}

// ����������(���ձ�)
BOOL NpcHolyRelicsBoxAllied(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	g_IllusionTempleEvent.HolyRelicsBoxAlliedAct(lpObj->MapNumber,lpNpc, lpObj);

	return TRUE;
}

// ����������(ȯ����)
BOOL NpcHolyRelicsBoxIllusion(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	g_IllusionTempleEvent.HolyRelicsBoxIllusionAct(lpObj->MapNumber, lpNpc, lpObj);

	return TRUE;
}

// �̶���
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

#ifdef ADD_SEASON_3_NEW_MAP_20070910	// �������� NPC ��ȭ
// �Ǻ��
BOOL NpcSilviaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)		
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	return FALSE;
}

// ����
BOOL NpcRheaTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)		
{
	if (!gObjIsConnected(lpObj->m_Index)) 
	{
		return TRUE;
	}

	return FALSE;
}

// ������
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
		{	// â�� ����ϰ� ��������.. ����Ҽ� ����
			return TRUE;
		}
#endif

#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		if (lpObj->m_bPShopOpen == true) {
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "���λ��� ���� �߿����� ī���� ������ ����Ͻ� �� �����ϴ�."
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
		// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
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
		{	// â�� ����ϰ� ��������.. ����Ҽ� ����
			return TRUE;
		}
#endif
		
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
		if (lpObj->m_bPShopOpen == true) {
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "���λ��� ���� �߿����� ī���� ������ ����Ͻ� �� �����ϴ�."
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
		// ����������. �������̽� ����Ѵٴ� üũ�� �صд�
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
//���� 4 ü�輭�� �����ʱ�ȭ NPC
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
//20081013 �ؿ� ũ�������� �̺�Ʈ - NPC (grooving)
#ifdef ADD_NPC_XMASEVENT_20080930
// ��Ÿ���� �湮�� ���� ���� NPC
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
// ����ƽ� ������ ���� �����ִ� NPC
BOOL Npc_SnowManTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index))
	{
		return TRUE;
	}
	
	CGReqCheckSnowManNPC( lpObj->m_Index );
	
	return TRUE;
}
// ��Ÿ�� ���� ���� NPC
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
			//��������
			//gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_HEAL, EFFECTTYPE_MAXLIFE_UP, 500, EFFECTTYPE_NONE, 0, 1800);
			gObjAddBuffEffect( lpObj, BUFFTYPE_SANTA_HEAL, EFFECTTYPE_HP, 500, EFFECTTYPE_NONE, 0, 1800);
		}
		break;
	case BUFFTYPE_SANTA_PROTECT:
		{
			//��������
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