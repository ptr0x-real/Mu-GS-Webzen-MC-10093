// ObjBaseAttack.cpp: implementation of the CObjBaseAttack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Include\Public.h"
#include "Include\ProDef.h"
#include "Include\SproDef.h"
#include "Include\GMProtocol.h"
#include "Common\Winutil.h"
#include "Common\zzzItem.h"
#include "Common\Classdef.h"
#include "Include\ReadScript.h"

#include "ObjBaseAttack.h"

#include "User.h"
#include "protocol.h"
#include "BattleGround.h"
#include "Gamemain.h"
#include "gObjMonster.h"
#include "MagicDamage.h"

#ifdef DEVILSQUARE_EXTEND_20050221
#include "DevilSquare.h"
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

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
#include "Crywolf.h"
#endif

#ifdef DARKLORD_WORK			
#include "PartyClass.h"
extern PartyClass gParty;
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
#include "ChinaHackUserKick.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
extern int g_ShieldSystemOn;
extern float g_fSuccessAttackRateOption;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjBaseAttack::CObjBaseAttack()
{

}

CObjBaseAttack::~CObjBaseAttack()
{

}

BOOL CObjBaseAttack::DecreaseArrow(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Class == CLASS_ELF && lpObj->Type == OBJTYPE_CHARACTER )
	{
		CItem	*Right = &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT];
		CItem	*Left  = &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT];

		if(g_ArrowSystem.CheckArrow(Right->m_Type) == true || g_ArrowSystem.CheckArrow(Left->m_Type) == true)
			return TRUE;
		
		// 석궁을 착용하고 있다면..
		if( (Right->m_Type >= MAKE_ITEMNUM(4,8)) && (Right->m_Type <= MAKE_ITEMNUM(4,14)) ||
			(Right->m_Type == MAKE_ITEMNUM(4,16)) || (Right->m_Type == MAKE_ITEMNUM(4,18)) || 
			(Right->m_Type == MAKE_ITEMNUM(4,19)) )
		{
			if( (Left->m_Type == MAKE_ITEMNUM(4,7)) )
			{
				if ( Left->m_Durability < 1 ) 
					return FALSE;
				Left->m_Durability--;					// 석궁용 화살 수량을 줄인다.
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, (BYTE)Left->m_Durability, 0);
				if( Left->m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, 0);
				}
			}
			else return FALSE;
		}
		else if( (Left->m_Type >= MAKE_ITEMNUM(4,0)) && (Left->m_Type <= MAKE_ITEMNUM(4,6)) || 
				 (Left->m_Type == MAKE_ITEMNUM(4,17)) 
#ifdef ADD_NEW_MAP_KALIMA_20040518
				 || (Left->m_Type == MAKE_ITEMNUM(4,20))		// 칼리마에 추가된 요정활
#endif				 
				 
#ifdef ADD_NEW_ITEM_01_20050823
				 || (Left->m_Type == MAKE_ITEMNUM(4,21))		// 아이템 확장시 추가된 요정활
#endif
				 
#ifdef MODIFY_350ITEM_BUGFIX_20060811
				 || (Left->m_Type == MAKE_ITEMNUM(4,22))		// 알바트로스보우 활 추가
#endif
				 
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
				 || (Left->m_Type == MAKE_ITEMNUM(4,23))		// 다크스팅거 활 추가
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
#ifdef ADD_AERINBOW_20090309
				 || (Left->m_Type == MAKE_ITEMNUM(4,24))		// 에어린보우 추가
#endif // ADD_AERINBOW_20090309

				 )	// 활을 착용하고 있다면..
		{
			if( (Right->m_Type == MAKE_ITEMNUM(4,15)))	// 반대편 손에는 화살
			{
				if ( Right->m_Durability < 1 ) 
					return FALSE;
				Right->m_Durability--;					// 화살 수량을 줄인다.
				GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, (BYTE)Right->m_Durability, 0);
				if( Right->m_Durability < 1 )
				{
					lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, EQUIPMENT_WEAPON_RIGHT, 0);
				}
			}
			else return FALSE;
		}
	}
	return TRUE;
}

BOOL CObjBaseAttack::CheckAttackArea(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
#ifdef WORLD_TOURNAMENT_EVENT_SETTING		
	return TRUE;
#endif
	////////////////////////////////////////////////////////////////////////////////
	if( lpTargetObj->Type == OBJTYPE_CHARACTER || (lpTargetObj->m_RecallMon>=0))
	{
		// 캐릭터는 안전지대에 있을때에는 죽지 않게한다.
		BYTE attr= MapC[lpTargetObj->MapNumber].GetAttr(lpTargetObj->X, lpTargetObj->Y);

		if( (attr&MAP_ATTR_SAFTYZONE) == MAP_ATTR_SAFTYZONE) // 안전지대일땐..
			return FALSE;
	}

	// 캐릭터도 안전지대와 못가는 지역에선 공격이 안된다.
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		//attr= MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
		//if( (attr&MAP_ATTR_SAFTYZONE) == MAP_ATTR_SAFTYZONE || (attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK ) return FALSE;
		// 공격 가능한 지역인지 검사
		int attackret = gObjCheckAttackArea(lpObj->m_Index, lpTargetObj->m_Index);
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
			{	// 시도 횟수가 일정이상 넘어 가면 킥이 가능하면
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
					return FALSE;
				}			
	#else // else MODIFY_HACKLOG_OF_ATTACK_20050518
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
					return FALSE;
				}
	#endif // MODIFY_HACKLOG_OF_ATTACK_20050518
			}
#else // else CHECK_ATTACK_AREA

	#ifdef MODIFY_LOG_OF_HACK_20050607
			lpObj->m_NotAttackAreaCount++;
			if( lpObj->m_NotAttackAreaCount % 100 == 0 )
			{
				LogAddTD("[%s][%s] Try Attack In Not Attack Area (%d) (%s,%d,%d) errortype = %d", 
				         lpObj->AccountID, lpObj->Name, lpObj->m_NotAttackAreaCount, lMsg.Get(lpObj->MapNumber+2000), lpObj->X, lpObj->Y, attackret);
			}
	#else
			LogAddTD("[%s][%s] Try Attack In Not Attack Area (%s,%d,%d) errortype = %d", 
				    lpObj->AccountID, lpObj->Name, lMsg.Get(lpObj->MapNumber+2000), lpObj->X, lpObj->Y, attackret);
	#endif

#endif // CHECK_ATTACK_AREA

			if( bIsIgnorePacketSpeedHackDetect )
			{
				return FALSE;
			}
		}
	}	

	return TRUE;
}

BOOL CObjBaseAttack::PkCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
#ifndef FOR_CHINA
//#ifdef FOR_CHINA
	if( gNonPK )		// 중국도 논피케이 서버에서는 몬스터 공격이 안되게 한다
#else
//	#ifdef FOR_TAIWAN	// 대만도 한국 설정과 같이 가도록 요청옴 20050914 (b4nfter)
//		if( gNonPK )	// 대만은 논피케이 서버에서만 살인마가 몬스터 공격이 안되게 한다
//	#else
			if( gLanguage == 0 || gLanguage == 2 || gLanguage == 4)
//	#endif
#endif
	{
		// 살인마는 몬스터도 공격도 할 수 없다.
		if( lpObj->Type == OBJTYPE_CHARACTER )
		{
#ifdef UPDATE_PK_SYSTEM_20070302	// 살인마는 몬스터를 공격할수도 공격 받을 수도 없음
			BOOL	PKFlag = FALSE;
			if(lpObj->PartyNumber >= 0)	// 파티에 속할경우
			{
				if( gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= PK_LEVEL_DEFAULT+3)
				{
					PKFlag = TRUE;
				}
			}
			else
			{
				if(lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3)
				{
					PKFlag = TRUE;
				}
			}

			if(PKFlag == TRUE)
#else
			if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3 ) 
#endif
			{
#ifdef UPDATE_PK_SYSTEM_20070302	//파티에 속해 있해 있고 파티 PK레벨3일 경우 공격 안됨
				if(lpObj->PartyNumber >= 0)	// 파티에 속할경우
				{
					if (!gPkLimitFree)
						return FALSE;
				}
				else
				{
					if( lpObj->m_PK_Count >= 3 ) 
					{
	#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
						if (!gPkLimitFree)
	#endif
						return FALSE;
					}
				}
#else	// UPDATE_PK_SYSTEM_20070302
				if( lpObj->m_PK_Count >= 3 ) 
				{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
					if (!gPkLimitFree)
#endif
					return FALSE;
				}
#endif	// UPDATE_PK_SYSTEM_20070302
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// 사용자끼리 싸울때 
	if( lpObj->Type	  == OBJTYPE_CHARACTER && 
		lpTargetObj->Type == OBJTYPE_CHARACTER )
	{
		// PK최소 레벨 이하는 죽이지 못한다.	
		if( lpTargetObj->Level  <= PK_MINIMUM_LEVEL || 
			lpObj->Level    <= PK_MINIMUM_LEVEL )
		{
			return FALSE;
		}


#ifdef ADD_NEW_UNION_01_20041006
		// ★★★ 적대일 경우 공격 할 수 있음.
		if( gObjGetRelationShip(lpObj, lpTargetObj) == G_RELATIONSHIP_RIVAL )
		{
			// NON PVP 서버에서는 적대 공격 안됨.
			if( !gNonPK ) return TRUE;
		}
#endif


//#ifdef APPLY_TESTSERVER
		// 길드전 시 같은 길드원 공격 않되게 (b4nfter - 2003-09-30)
		if( lpObj->lpGuild && lpTargetObj->lpGuild )
		{
			if (( lpObj->lpGuild->WarState == 1 && lpTargetObj->lpGuild->WarState == 1 ) &&
				(lpObj->lpGuild->Number == lpTargetObj->lpGuild->Number)
				)
			{
				// 현재 길드전 중 이고 같은 길드원이라면 공격않됨
				return FALSE;
			}
		}

		// 길드전 중인 사람도 못 때리게
		if (
#ifdef _NEW_EXDB_
			!gObjTargetGuildWarCheck(lpObj, lpTargetObj) &&
#endif
			lpTargetObj->lpGuild && 
			lpTargetObj->lpGuild->WarState
			) 
		{

#ifdef MODIFY_BATTLESOCCER_PKCHECK_20050714
			// 배틀사커 걸어 놓고 다른 맵에서 공격 안당하는 버그 수정
			if ( lpTargetObj->lpGuild->WarType == BTT_SOCCER )
			{
				 if( lpTargetObj->MapNumber != 6 )
					 if( !gNonPK ) return TRUE;
			}
#endif
			
#ifdef CHAOSCASTLE_SYSTEM_20040408		// 길드전 중이라고 해도 카오스 캐슬 안에 있다면 공격이 되어야 한다.
			if (!CHECK_CHAOSCASTLE(lpTargetObj->MapNumber))
#endif			
			return FALSE;

		}
//#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영 사원에서는 pk가 허용됨
		if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
		{
			
		}
		else
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING
		if(CHECK_CHAOSCASTLE(lpObj->MapNumber))
		{	// 카오스 캐슬에서는 경기시간이 아니면 죽일 수 없다
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 번호 구하기
			if (g_ChaosCastle.GetCurrentState( g_ChaosCastle.GetChaosCastleByMapNum( lpObj->MapNumber ) ) != CHAOSCASTLE_STATE_PLAYING)
				return FALSE;
#else
			if (g_ChaosCastle.GetCurrentState(lpObj->MapNumber - MAP_INDEX_CHAOSCASTLE1) != CHAOSCASTLE_STATE_PLAYING)
				return FALSE;
#endif
		}
		else	// NON PK 서버에서도 죽일 수 있어야 한다.
	#endif
#endif
		// NonPK 서버는 죽이지 못한다
		if( gNonPK ) return FALSE;

#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( CHECK_DEVILSQUARE(lpObj->MapNumber) && lpObj->m_stateEngagePVP != PVP_USER_PLAYING )
#else
		if( lpObj->MapNumber == MAP_INDEX_DEVILSQUARE && lpObj->m_stateEngagePVP != PVP_USER_PLAYING )
#endif
		{	// 악마의 광장에서는 죽일수 없다
			return FALSE;
		}

#ifdef FOR_BLOODCASTLE
		if(CHECK_BLOODCASTLE(lpObj->MapNumber))
		{	// 블러드 캐슬에서도 역시 죽일수 없다
			return FALSE;
		}
#endif

		
#ifdef DUEL_SYSTEM_20031028
		if (!gObjDuelCheck(lpObj, lpTargetObj) &&
			gObjDuelCheck(lpTargetObj)
			) 
		{
			// 결투 중인 사용자를 공격할 수 없다.
			return FALSE;
		}

#endif

		if( gLanguage > 6)
		{
			// 살인마는 살인을 할 수 없다.
#ifdef UPDATE_PK_SYSTEM_20070302	// 살인마는 살인을 못함
			BOOL	PKFlag = FALSE;
			if(lpObj->PartyNumber >= 0)	// 파티에 속할경우
			{
				if( gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= PK_LEVEL_DEFAULT+3)
				{
					PKFlag = TRUE;
				}
			}
			else
			{
				if(lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3)
				{
					PKFlag = TRUE;
				}
			}
			
			if(PKFlag == TRUE)
#else	// UPDATE_PK_SYSTEM_20070302
			if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3 ) 
#endif	// UPDATE_PK_SYSTEM_20070302
			{
#ifdef UPDATE_PK_SYSTEM_20070302	//파티에 속해 있해 있고 파티 PK레벨3일 경우
				if(lpObj->PartyNumber >= 0)	// 파티에 속할경우
				{
					if (!gPkLimitFree)
						return FALSE;
				}
				else
				{
					if( lpObj->m_PK_Count >= 3 ) 
					{
	#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
						if (!gPkLimitFree)
	#endif
						return FALSE;
					}
				}
#else	// UPDATE_PK_SYSTEM_20070302
				if( lpObj->m_PK_Count >= 3 ) 
				{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
					if (!gPkLimitFree)
#endif
					return FALSE;
				}
#endif	// UPDATE_PK_SYSTEM_20070302
			}
#ifndef HERO_SYSTEM_MODIFY
#ifdef UPDATE_PK_SYSTEM_20070302	// 살인마는 살인을 못함
			PKFlag = FALSE;
			if(lpTargetObj->PartyNumber >= 0)	// 파티에 속할경우
			{
				if( gParty.GetPKPartyPenalty(lpTargetObj->PartyNumber) >= PK_LEVEL_DEFAULT+3)
				{
					PKFlag = TRUE;
				}
			}
			else
			{
				if(lpTargetObj->m_PK_Level >= PK_LEVEL_DEFAULT+3)
				{
					PKFlag = TRUE;
				}
			}
			
			if(PKFlag == TRUE)
#else	// UPDATE_PK_SYSTEM_20070302
			if( lpTargetObj->m_PK_Level >= PK_LEVEL_DEFAULT+3 ) 
#endif	// UPDATE_PK_SYSTEM_20070302
			{ 
#ifdef UPDATE_PK_SYSTEM_20070302
				if(lpTargetObj->PartyNumber >= 0)	// 파티에 속할경우
				{
#ifdef MODIFY_PK_SYSTEM_BUGFIX_2_20070731	// 공성전 중에는 풀카오 사용자도 죽일 수 있다.
					if (!gPkLimitFree)	
					{
#ifdef GAMESERVER_TO_CASTLESIEGESERVER			// 공성전 중에는 풀카오 사용자도 죽일 수 있다.
						if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
#endif	
							return FALSE;
					}
#else					
					if (!gPkLimitFree)
						return FALSE;
#endif
				}
				else
				{
					if( lpTargetObj->m_PK_Count >= 3 ) 
					{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
						if (!gPkLimitFree)	{
#endif
#ifdef GAMESERVER_TO_CASTLESIEGESERVER			// 공성전 중에는 풀카오 사용자도 죽일 수 있다.
							if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
#endif			
								return FALSE;
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
						}
#endif
					}
				}
#else	// UPDATE_PK_SYSTEM_20070302
				if( lpTargetObj->m_PK_Count >= 3 ) 
				{
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
					if (!gPkLimitFree)	{
#endif
#ifdef GAMESERVER_TO_CASTLESIEGESERVER			// 공성전 중에는 풀카오 사용자도 죽일 수 있다.
					if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
#endif			
					return FALSE;
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
					}
#endif
				}
#endif	// UPDATE_PK_SYSTEM_20070302
			}
#endif			
		}
	}
	return TRUE;
}

int CObjBaseAttack::ResistanceCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111			// 공성전 NPC - 석상, 성문 등은 각종 중독마법에 걸리면 않된다.
	if (lpTargetObj->Type != OBJTYPE_CHARACTER) 
	{
		if ((lpTargetObj->Class == 277)||			// 성문
			(lpTargetObj->Class == 283)||			// 수호석상
			(lpTargetObj->Class == 288)||			// 캐논타워
			(lpTargetObj->Class == 278)||			// 라이프스톤
			(lpTargetObj->Class == 215)||			// 방어막
			(lpTargetObj->Class == 216)||			// 왕관
			(lpTargetObj->Class == 217)||			// 왕관스위치
			(lpTargetObj->Class == 218)||			// 왕관스위치
			(lpTargetObj->Class == 219)				// 성문레버
			)
		{
			return 0;
		}
	}
#endif

#ifdef CRYWOLF_NPC_WORK_20050912
	// 크라이울프 NPC들 저항처리
	if( lpTargetObj->Type != OBJTYPE_CHARACTER ) 
	{
		if( 
			CHECK_CRYWOLF_NPC_ALTAR(lpTargetObj->Class)			// 늑대의 제단
			|| CHECK_CRYWOLF_NPC_STATUE(lpTargetObj->Class)		// 늑대의 신상
		  )
		{
			return 0;
		}
	}

#endif

#ifdef DARKLORD_WORK
	if( skill == AT_SKILL_DARKHORSE_ATTACK )
	{
		gObjBackSpring2(lpTargetObj, lpObj,3);
	}
#endif
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 어스쉐이크 강화 - 뒤로 밀리게.
	if( g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK )
	{
		gObjBackSpring2( lpTargetObj, lpObj, 3 );
	}
#endif
	
	////////////////////////////////////////////////////////////////////////////////
	// 스킬 공격일땐 뒤로 물러나게 한다.	-> 사람 몬스터 둘 다
	if( skill == AT_SKILL_SWORD1  || 
		skill == AT_SKILL_SWORD2  || 
		skill == AT_SKILL_SWORD3  || 
		skill == AT_SKILL_SWORD4  || 
		skill == AT_SKILL_SWORD5 )
	{		
		gObjAddMsgSendDelay(lpTargetObj, 2, lpObj->m_Index, 150);
	}
	// 번개 저항력
	else if( skill == AT_SKILL_THUNDER )
	{	// 상대가 번개에 대한 저항력 없다면
		if( retResistance(lpTargetObj, RESISTANCE_THUNDER) == FALSE ) 
		{
			gObjAddMsgSendDelay(lpTargetObj, 2, lpObj->m_Index, 150);
			return 1;
		}
		else return 0;
	}	
	else if( skill == AT_SKILL_POISON )	// 중독 공격 세팅
	{	// 독저항력
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_POISON ) == false )
		{
			if( retResistance( lpTargetObj, RESISTANCE_POISON ) == FALSE )
			{
				lpTargetObj->lpAttackObj = lpObj;
				gObjAddBuffEffect( lpTargetObj, BUFFTYPE_POISON, EFFECTTYPE_POISON_DMG_TICK, 3, 0, 0, 20 );
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
#else
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_POISON ) == false )
		{
			if( retResistance( lpTargetObj, RESISTANCE_POISON ) == FALSE )
			{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
				lpTargetObj->lpAttackObj = lpObj;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
				gObjAddBuffEffect( lpTargetObj, BUFFTYPE_POISON, EFFECTTYPE_POISON_DMG_TICK, 3, 0, 0, 20 );
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
#else
	#ifdef ADD_SKILL_WITH_COMBO		
		if( lpTargetObj->m_PoisonType == 0 ) // 현재 중독 마법에 걸려있다면 다시 걸리지 않게..
	#else
		if( lpTargetObj->m_PoisonBeattackCount == 0 ) // 현재 중독 마법에 걸려있다면 다시 걸리지 않게..
	#endif	//#ifdef ADD_SKILL_WITH_COMBO
		{
			if( retResistance(lpTargetObj, RESISTANCE_POISON) == FALSE ) 
			{
#ifdef ADD_SKILL_WITH_COMBO
				lpTargetObj->m_PoisonType = AT_SKILL_POISON;
#endif	//#ifdef ADD_SKILL_WITH_COMBO
				lpTargetObj->m_PoisonBeattackCount	= 20;	// 10초 동안 중독 상태로..
				lpTargetObj->lpAttackObj			= lpObj;
				lpTargetObj->m_ViewSkillState		|= 0x01; //	1번째 비트
			}
			else return 0;	// 독에 대한 저항력이 있다면 공격은 실패
		}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		else
		{
			return 0;
		}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}
#ifdef ADD_SKILL_WITH_COMBO		
	// 독공격2(데쓰포이즌)
	else if( skill == AT_SKILL_EXPPOISON )	// 중독 공격 세팅(데쓰포이즌)
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_POISON ) == false )
		{
			if( retResistance( lpTargetObj, RESISTANCE_POISON ) == FALSE )
			{
				lpTargetObj->lpAttackObj = lpObj;
				gObjAddBuffEffect( lpTargetObj, BUFFTYPE_POISON, EFFECTTYPE_POISON_DMG_TICK, 3, 0, 0, 10 );
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
#else
		if( lpTargetObj->m_PoisonType == 0 || lpTargetObj->m_PoisonType == AT_SKILL_POISON )		// 현재 중독 마법에 걸려있다면 다시 걸리지 않게..
		{
			if( retResistance(lpTargetObj, RESISTANCE_POISON) == FALSE ) 
			{	
				lpTargetObj->m_PoisonType = AT_SKILL_EXPPOISON;

				lpTargetObj->m_PoisonBeattackCount	= 10;	// 10초 동안 중독 상태로..
				lpTargetObj->lpAttackObj			= lpObj;
				lpTargetObj->m_ViewSkillState		|= 0x01; //	1번째 비트

				GCStateInfoSend(lpTargetObj, 1, lpTargetObj->m_ViewSkillState);
			}
			else return 0;	// 독에 대한 저항력이 있다면 공격은 실패
		}
		else
		{
			return 0;
		}
		return 1;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}
#endif	// #ifdef ADD_SKILL_WITH_COMBO		
#ifdef ADD_SKILL_WITH_COMBO		// 얼음 공격
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428			// 서든 아이스 강화
	else if( skill == AT_SKILL_SLOW || skill == AT_SKILL_EXPICE 
		|| g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_EXPICE // 서든 아이스 강화 (grooving)
		)
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	else if( skill == AT_SKILL_SLOW || skill == AT_SKILL_EXPICE )
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
#else
	else if( skill == AT_SKILL_SLOW )
#endif	//#ifdef ADD_SKILL_WITH_COMBO		
	{	
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_FREEZE ) == false )
		{
			if( retResistance( lpTargetObj, RESISTANCE_COLD ) == FALSE )
			{
				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->DelayActionTime = 800;
				lpTargetObj->DelayLevel = 1;
				gObjAddBuffEffect( lpTargetObj, BUFFTYPE_FREEZE, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 10 );
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
#else
		if( lpTargetObj->m_ColdBeattackCount == 0 )
		{
			if( retResistance(lpTargetObj, RESISTANCE_COLD ) == FALSE )
			{
				lpTargetObj->m_ColdBeattackCount = 10;
				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->DelayActionTime = 800;
				lpTargetObj->DelayLevel = 1;
				
#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
				ADD_VIEWSKILLSTATE( lpTargetObj->m_ViewSkillState, STATE_FREEZE );
#else
				lpTargetObj->m_ViewSkillState |= 0x02; //	2번째 비트
#endif

#ifdef ADD_SKILL_WITH_COMBO		
				lpTargetObj->m_IceType = skill;
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428			// 서든 아이스 강화
				if( skill == AT_SKILL_EXPICE 
					|| skill == AT_SKILL_ML_STRENGTHEN_EXPICE	// 서든 아이스 강화
					)
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
				if( skill == AT_SKILL_EXPICE )
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
				{
					GCStateInfoSend(lpTargetObj, 1, 0x02);
				}
#endif	//#ifdef ADD_SKILL_WITH_COMBO
			}
			else {
				return 0;
			}
		}
		else
		{
			return 0;
		}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004	
	}
	else if( skill == AT_SKILL_ELFHARDEN )	// 아이스에로우
	{	
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_STONE ) == false )
		{
			if( retResistance( lpTargetObj, RESISTANCE_COLD ) == FALSE )
			{
				gObjAddBuffEffect( lpTargetObj, BUFFTYPE_STONE, 0, 0, 0, 0, 7 );
				lpTargetObj->PathCount = 0;
				lpTargetObj->PathStartEnd = 0;
				gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
#elif defined(NEW_SKILL_FORSKYLAND)
		if( lpTargetObj->m_SkillHardenTime <= 0 )
		{	
			if( retResistance(lpTargetObj, RESISTANCE_COLD) == FALSE )
			{	
				// 아이스 애로우 스킬은 이곳에서 성공여부만 판단하고, Miss 체크 후에 
				// 아래 내용을 적용하도록 한다.
#ifndef MODIFY_ELF_ICEARROWSKILL_BUGFIX_20060914 // not define
				lpTargetObj->m_SkillHarden = 7;
				lpTargetObj->m_SkillHardenTime = 7;

				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->m_ViewSkillState |= STATE_HARDEN; //	석화
				lpTargetObj->PathCount = 0;

	#ifdef MODIFY_MONSTER_ACT_BY_ICEARROW_20050914
				lpTargetObj->PathStartEnd = 0;
	#endif

				gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
#endif // MODIFY_ELF_ICEARROWSKILL_BUGFIX_20060914
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}

	return 1;
}

BOOL CObjBaseAttack::MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int magicsend, int& bAllMiss )
{
	BOOL bRet = FALSE;

#define LEAVE_SET(x) {bRet = x; __leave;}

	__try
	{
		int SuccessAttackRate = 0;
		int TargetSuccessfulBlocking = lpTargetObj->m_SuccessfulBlocking;
		int	MsgDamage = 0;

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// 환영 사원 전용스킬 보호의 주문 적용
	if(CHECK_ILLUSION_TEMPLE(lpTargetObj->MapNumber) 
		&& g_IllusionTempleEvent.GetIllusionTempleState(lpTargetObj->MapNumber) == ILLUSION_TEMPLE_STATE_PLAYING)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
		if(lpTargetObj->Type == OBJTYPE_CHARACTER 
			&& g_IllusionTempleEvent.CheckSkillProdection(lpTargetObj->m_iIllusionTempleIndex, lpTargetObj->MapNumber))
#else
		if((lpTargetObj->m_ViewSkillState & STATE_ILLUSION_TEMPLE_PRODECTION) == STATE_ILLUSION_TEMPLE_PRODECTION)
#endif
		{
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
			LEAVE_SET(FALSE);
		}

		// 같은 파티원끼리는 데미지 없게
		if(lpObj->PartyNumber == lpTargetObj->PartyNumber)
		{
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
			LEAVE_SET(FALSE);
		}
	}
#endif

		if( lpObj->Type == OBJTYPE_CHARACTER )
		{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 종합레벨 - 공격 성공률 조정
			int iTotalLevel = lpObj->Level + lpObj->m_nMasterLevel;
			if( lpObj->Class == CLASS_DARKLORD )
			{	// 다크로드만 공격 성공률이 틀리다				
				SuccessAttackRate = (iTotalLevel*5)+(( (lpObj->Dexterity+lpObj->AddDexterity)*6)/2)+((lpObj->Strength+lpObj->AddStrength)/4)+((lpObj->Leadership+lpObj->AddLeadership)/10);
				// 세트 옵션 공격 성공률 증가
				SuccessAttackRate += SuccessAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
			}
			else
			{
				SuccessAttackRate = (iTotalLevel*5)+(( (lpObj->Dexterity+lpObj->AddDexterity)*3)/2)+((lpObj->Strength+lpObj->AddStrength)/4);
				// 세트 옵션 공격 성공률 증가
				SuccessAttackRate += SuccessAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
			}			
#else			
#ifdef DARKLORD_WORK
			if( lpObj->Class == CLASS_DARKLORD )
			{	// 다크로드만 공격 성공률이 틀리다
				SuccessAttackRate = (lpObj->Level*5)+(( (lpObj->Dexterity+lpObj->AddDexterity)*6)/2)+((lpObj->Strength+lpObj->AddStrength)/4)+((lpObj->Leadership+lpObj->AddLeadership)/10);
				// 세트 옵션 공격 성공률 증가
				SuccessAttackRate += SuccessAttackRate*lpObj->SetOpImproveSuccessAttackRate/100;
			}
			else
			{
				SuccessAttackRate = (lpObj->Level*5)+(( (lpObj->Dexterity+lpObj->AddDexterity)*3)/2)+((lpObj->Strength+lpObj->AddStrength)/4);
				// 세트 옵션 공격 성공률 증가
				SuccessAttackRate += SuccessAttackRate*lpObj->SetOpImproveSuccessAttackRate/100;
			}
#else
				SuccessAttackRate = (lpObj->Level*5)+(( (lpObj->Dexterity+lpObj->AddDexterity)*3)/2)+((lpObj->Strength+lpObj->AddStrength)/4);
				// 세트 옵션 공격 성공률 증가
				SuccessAttackRate += SuccessAttackRate*lpObj->SetOpImproveSuccessAttackRate/100;
#endif
#endif // MASTER_LEVEL_UP_SYSTEM_20070912
		}
		else
		{
			SuccessAttackRate = lpObj->m_AttackRating;
		}
		
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// 패시브 1차 - 공격성공률 상승
		SuccessAttackRate += lpObj->m_MPSkillOpt.iMpsAttackSuccessRate;		
#endif	

		if(TargetSuccessfulBlocking > g_GlobalConfig.m_iMaxMissRate)
			TargetSuccessfulBlocking = g_GlobalConfig.m_iMaxMissRate;

#ifdef NEW_FORSKYLAND3
		if( SuccessAttackRate < TargetSuccessfulBlocking )
		{
			bAllMiss = TRUE;
		}
#endif

#ifdef NEW_FORSKYLAND3
		if( bAllMiss )
		{	

#ifdef MODIFY_SUCCESSBLOCKING_ALLMISS_20040408
			if( rand()%100 >= 5 )
#else
			if( rand()%100 >= 3 )
#endif
			{	// all 미스는 3%의 확률로 데미지가 들어 온다

#ifdef ADD_SKILL_WITH_COMBO
#else
				MsgDamage = 0x20;
#endif
#ifdef ADD_SHIELD_POINT_01_20060403
				GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage, 0);
#else
				GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage);
#endif //ADD_SHIELD_POINT_01_20060403
				if( magicsend )
				{
					GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
				}

				LEAVE_SET(FALSE);
			}
		}
		else 
#endif
		if( (rand()%SuccessAttackRate) < TargetSuccessfulBlocking) 
		{

#ifdef ADD_SKILL_WITH_COMBO
#else
			int MsgDamage = 0x20;
#endif
			
#ifdef ADD_SHIELD_POINT_01_20060403
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage, 0);
#else
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage);
#endif
			if( magicsend )
			{
				GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
			}
			LEAVE_SET(FALSE);
		}

		bRet = TRUE;
	}
	__except(lpObj->Level =1, EXCEPTION_CONTINUE_EXECUTION  )
	{
		LogAdd(lMsg.Get(524), lpObj->Name);
		return FALSE;
	}	

	return bRet;
}

#ifdef ADD_SHIELD_POINT_01_20060403
BOOL CObjBaseAttack::MissCheckPvP(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int magicsend, int& bAllMiss )
{
	float iAttackRate = 0;
	float iDefenseRate = 0;
	int iAttackSuccessRate = 0;
	// 각 직업별 AttackRating/DefenseRating

	// CODEREVIEW_SHIELD : 소수점 사용없음
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// 환영 사원 전용스킬 보호의 주문 적용
	if(CHECK_ILLUSION_TEMPLE(lpTargetObj->MapNumber) 
		&& g_IllusionTempleEvent.GetIllusionTempleState(lpTargetObj->MapNumber) == ILLUSION_TEMPLE_STATE_PLAYING)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
		if(lpTargetObj->Type == OBJTYPE_CHARACTER 
			&& g_IllusionTempleEvent.CheckSkillProdection(lpTargetObj->m_iIllusionTempleIndex, lpTargetObj->MapNumber))
#else
		if((lpTargetObj->m_ViewSkillState & STATE_ILLUSION_TEMPLE_PRODECTION) == STATE_ILLUSION_TEMPLE_PRODECTION)
#endif
		{
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
			return FALSE;
		}

		// 같은 파티원끼리는 데미지 없게
		if(lpObj->PartyNumber == lpTargetObj->PartyNumber)
		{
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
			return FALSE;
		}
	}
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 종합레벨 적용 - 대인 공격 성공률/대인 방어 성공률1
	int iTotalLevel = lpObj->Level + lpObj->m_nMasterLevel;
	if( lpObj->Class == CLASS_KNIGHT )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 45 / 10 );
	}
	else if( lpObj->Class == CLASS_DARKLORD )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 4 );
	}
	else if( lpObj->Class == CLASS_ELF )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 6 / 10 );
	}
	else if( lpObj->Class == CLASS_MAGUMSA )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 35 / 10 );
	}
	else if( lpObj->Class == CLASS_WIZARD )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 4 );
	}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 대인 공격 성공률
	else if( lpObj->Class == CLASS_SUMMONER )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 35 / 10 );
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	
	int iTargetTotalLevel = lpTargetObj->Level + lpTargetObj->m_nMasterLevel;
	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
	else if( lpTargetObj->Class == CLASS_DARKLORD )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
	else if( lpTargetObj->Class == CLASS_ELF )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 10 );
	}
	else if( lpTargetObj->Class == CLASS_MAGUMSA )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 4 );
	}
	else if( lpTargetObj->Class == CLASS_WIZARD )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 4 );
	}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 대인 방어 성공률
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	// 버그수정
	if( iAttackRate <= 0 || iDefenseRate <= 0 || lpObj->Level <= 0 || lpTargetObj->Level <= 0 )
	{
		// iAttackRate나 iDefenseRate가 0이 나온경우의 처리
		return FALSE;
	}

#else	// MASTER_LEVEL_UP_SYSTEM_20070912
	if( lpObj->Class == CLASS_KNIGHT )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 45 / 10 );
	}
	else if( lpObj->Class == CLASS_DARKLORD )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 4 );
	}
	else if( lpObj->Class == CLASS_ELF )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 6 / 10 );
	}
	else if( lpObj->Class == CLASS_MAGUMSA )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 35 / 10 );
	}
	else if( lpObj->Class == CLASS_WIZARD )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 4 );
	}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 대인 공격 성공률
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 35 / 10 );
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
	else if( lpTargetObj->Class == CLASS_DARKLORD )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
	else if( lpTargetObj->Class == CLASS_ELF )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 10 );
	}
	else if( lpTargetObj->Class == CLASS_MAGUMSA )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 4 );
	}
	else if( lpTargetObj->Class == CLASS_WIZARD )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 4 );
	}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 대인 방어 성공률
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	if( iAttackRate <= 0 || iDefenseRate <= 0 || lpObj->Level <= 0 || lpTargetObj <= 0 )
	{
		// iAttackRate나 iDefenseRate가 0이 나온경우의 처리
		return FALSE;
	}
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912


	// 최종 공격 성공률
	// iAttackSuccessRate = 100 * iAttackRate / ( iAttackRate + iDefenseRate ) * 2 * lpObj->Level / ( lpObj->Level + lpTargetObj->Level );

#ifdef ADD_380ITEM_NEWOPTION_20060711
	// 380 추가 아이템 옵션 : 공격성공율, 방어 성공율 상승
	iAttackRate		+= lpObj->m_ItemOptionExFor380.OpAddAttackSuccessRatePVP;
	iDefenseRate	+= lpTargetObj->m_ItemOptionExFor380.OpAddDefenseSuccessRatePvP;
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// 조화의 보석 강화 아이템 옵션 : 공격 성공율, 방어 성공율 상승 
	iAttackRate		+= lpObj->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP;
	iDefenseRate	+= lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddDefenseSuccessRatePvP;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// 패시브 1차 - 대인공격성공률, 대인방어 성공률 상승
	iAttackRate		+= lpObj->m_MPSkillOpt.iMpsPVPAttackDmgRate;
	iDefenseRate	+= lpTargetObj->m_MPSkillOpt.iMpsPVPBlockingRate;
#endif
	
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 종합레벨 적용 - 대인 공격 성공률/대인 방어 성공률2
	// CODEREVIEW_SHIELD : 코드 보기 좋게 하기
	float iExpressionA = ( ( iAttackRate * 10000 ) / ( iAttackRate + iDefenseRate ) );
	float iExpressionB = ( ( iTotalLevel * 10000 ) / ( iTotalLevel + iTargetTotalLevel ) );
#else
	// CODEREVIEW_SHIELD : 코드 보기 좋게 하기
	float iExpressionA = ( ( iAttackRate * 10000 ) / ( iAttackRate + iDefenseRate ) );
	float iExpressionB = ( ( lpObj->Level * 10000 ) / ( lpObj->Level + lpTargetObj->Level ) );
#endif

	iExpressionA = iExpressionA / 10000;
	iExpressionB = iExpressionB / 10000;

	iAttackSuccessRate = 100 * iExpressionA * g_fSuccessAttackRateOption * iExpressionB;

	// 레벨 차에 따른 공격 성공률 조정
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 종합레벨 적용 - 레벨 차에 따른 공격 성공률 조정
	if( iTargetTotalLevel - iTotalLevel >= 100  )
	{
		iAttackSuccessRate = iAttackSuccessRate - 5;
	}
	else if( iTargetTotalLevel - iTotalLevel >= 200 )
	{
		iAttackSuccessRate = iAttackSuccessRate - 10;
	}
	else if( iTargetTotalLevel - iTotalLevel >= 300 )
	{
		iAttackSuccessRate = iAttackSuccessRate - 15;
	}
#else
	if( lpTargetObj->Level - lpObj->Level >= 100  )
	{
		iAttackSuccessRate = iAttackSuccessRate - 5;
	}
	else if( lpTargetObj->Level - lpObj->Level >= 200 )
	{
		iAttackSuccessRate = iAttackSuccessRate - 10;
	}
	else if( lpTargetObj->Level - lpObj->Level >= 300 )
	{
		iAttackSuccessRate = iAttackSuccessRate - 15;
	}
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912

#ifdef SHIELD_POINT_DEBUGLOG
	char Msg[256];
	sprintf(Msg, "[SD Attack] AR:%5.1f/DR:%5.1f/ASR:%d", iAttackRate, iDefenseRate, iAttackSuccessRate);
	GCServerMsgStringSend( Msg, lpObj->m_Index, 1 );
	GCServerMsgStringSend( Msg, lpTargetObj->m_Index, 1 );
#endif

	// 공격 실패?
	DWORD dwRate = rand() % 100;
	if( dwRate > iAttackSuccessRate )
	{
		// Miss 정보를 날린다.
		GCDamageSend( lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
		
		#ifdef UPDATE_SHIELD_OPTION_20060427
			if ( g_bShieldComboMissOptionOn == TRUE )
		#endif // UPDATE_SHIELD_OPTION_20060427
			{
				// 콤보 스킬 사용중이면 콤보를 취소해 준다.
				if( lpObj->comboSkill.ProgressIndex >= 0 )
				{
					LogAddTD("[Shield] ComboSkill Cancel! [%s][%s]", lpObj->AccountID, lpObj->Name );
					lpObj->comboSkill.Init();
				}
			}
		return FALSE;
	}

	return TRUE;
}
#endif // ADD_SHIELD_POINT_01_20060403

int CObjBaseAttack::GetTargetDefense(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, BYTE& MsgDamage)
{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_06_20080102
	int targetdefense = lpTargetObj->m_Defense;
#else
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	int skilldefense = gObjGetTotalValueOfEffect( lpTargetObj, EFFECTTYPE_IMPROVE_DEFENSE );
	skilldefense += gObjGetTotalValueOfEffect( lpTargetObj, EFFECTTYPE_IMPROVE_MELEE_DEFENSE );
	int targetdefense = lpTargetObj->m_Defense + skilldefense;
	#else
	int skilldefense = lpTargetObj->m_SkillDefense;
	int targetdefense = lpTargetObj->m_Defense+skilldefense;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // MODIFY_BUFFSYSTEM_BUGFIX_06_20080102

#ifdef ADD_380ITEM_NEWOPTION_20060711
	// 380 추가 아이템 옵션 : 대인간 방어력 증가 +%d. 서버계산은 /2를 유지.
	if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
	{
		targetdefense += lpTargetObj->m_ItemOptionExFor380.OpAddDefense/2;
	}
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	int iDecreaseDefense = 0;
	iDecreaseDefense = gObjGetTotalValueOfEffect( lpTargetObj, EFFECTTYPE_DECREASE_DEFENSE );
	targetdefense -= ( targetdefense * iDecreaseDefense ) / 100;
#elif defined(NEW_SKILL_FORSKYLAND)
	targetdefense -= targetdefense*lpTargetObj->m_SkillMagumReduceDefense/100;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
// 방어력 증가&감소
	if( lpObj->m_MonsterSkillElementInfo.m_iSkillElementDefenseTime > 0 )
	{
#if TESTSERVER == 1
		MsgOutput(lpObj->m_Index, "%d 방증&감상태", lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack);
#endif
		
		targetdefense += lpObj->m_MonsterSkillElementInfo.m_iSkillElementDefense;

		if( targetdefense < 0 ) targetdefense = 0;
	}
#endif

	int percentdamage = 0;
	if( lpObj->pInventory[EQUIPMENT_WING].IsItem() )
	{
		percentdamage = lpObj->pInventory[EQUIPMENT_WING].IsWingOpGetOnePercentDamage();		
		
#ifdef ADD_THIRD_WING_20070525	// 3차 날개 
		// 5% 확률로 적의 방어력 무시 옵션이 적용되어 있으면.
		percentdamage += lpObj->pInventory[EQUIPMENT_WING].GetIgnoreDefencePer();
#endif
	}	
	percentdamage += lpObj->SetOpIgnoreDefense;		// 세트 옵션 적 방어 무시
	if( percentdamage )
	{	// 3%확률로 적의 방어력 무시 공격옵션이 적용되어 있으면

		if( rand()%100 <= percentdamage )
		{
			targetdefense = 0;
			MsgDamage = DT_PERFECT;	// 방어율 무시 공격
		}
	}

	return targetdefense;
}

#ifdef DARKLORD_WORK

int CObjBaseAttack::GetPartyMemberCount(LPOBJECTSTRUCT lpObj)
{
	LPOBJECTSTRUCT lpPartyObj;

	int partynum = lpObj->PartyNumber;
	
	if( !CHECK_LIMIT(partynum, MAX_PARTY) )
	{
		LogAdd("error : DecPartyMemberHPandMP %s %d",__FILE__, __LINE__);
		return 0;
	}

	int partycount = gParty.m_PartyS[partynum].Count;
	int retcount = 0;
	for( int n=0; n<MAX_PARTYUSER; n++)
	{
		int number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];
			
			// 같은 맵에 존재하거나..
			if( lpObj->MapNumber == lpPartyObj->MapNumber) 
			{
				int dis = gObjCalDistance(lpObj, &gObj[number]);
				if( dis < 10 )
				{	
					retcount++;
				}
			}
		}
	}
	return retcount;
}
#endif