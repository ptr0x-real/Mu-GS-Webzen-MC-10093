// ObjAttack.cpp: implementation of the CObjAttack class.
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

#include "BattleSoccerManager.h"

#include "User.h"
#include "protocol.h"
#include "BattleGround.h"
#include "Gamemain.h"
#include "gObjMonster.h"
#include "MagicDamage.h"

#include "ObjAttack.h"

#ifdef DARKLORD_WORK			
#include "PartyClass.h"
extern PartyClass gParty;
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

#ifdef DARKLORD_WORK
#include "DarkSpirit.h"
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
#include "ObjUseSkill.h"
#endif

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
#include "Crywolf.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"
#endif // ADD_NEWPVP_PKFIELD

CObjAttack gclassObjAttack;

BOOL gObjDenorantSprite(LPOBJECTSTRUCT lpObj);
#ifdef DARKLORD_WORK
BOOL	gObjDarkHorse(LPOBJECTSTRUCT lpObj);
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
extern int g_ShieldSystemOn;
extern int g_iDamageDevideToSDRate;
extern int g_iDamageDevideToHPRate;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CObjAttack::CObjAttack()
{

}

CObjAttack::~CObjAttack()
{

}

//----------------------------------------------------------------------------
//
// *	Main Attack 함수
//
// *	lpObj 가 lpTargetObj 를 공격 했다.
//
//
//----------------------------------------------------------------------------
BOOL CObjAttack::Attack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int magicsend, BYTE MSBFlag, int AttackDamage,BOOL bCombo)
{
	

	//---------------------------------------------------------------------------------
	//
	//  [] Attack 함수 시작 ...
	//
	//
	//---------------------------------------------------------------------------------

	int				skillSuccess=0;

	LPOBJECTSTRUCT	lpCallObj;
	LPOBJECTSTRUCT	lpCallTargetObj;
	BYTE			MsgDamage = 0;

	int				ManaChange = FALSE;

#ifdef ADD_SHIELD_POINT_01_20060403
	// CODEREVIEW_SHIELD : iShieldDamage -> Temp 의미 추가
	int				iTempShieldDamage = 0;
	int				iTotalShieldDamage = 0;
#endif

	if( (lpTargetObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
	{	// 운영자 캐릭은 공격을 받지 않는다
		return FALSE;
	}

#ifdef GMCOMMAND_EX_1ST_BUGFIX
	if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
	{	// 운영자 캐릭은 공격을 못한다.
		return FALSE;
	}
	
	if( ((lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)
		|| ((lpTargetObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)) 
	{
		// AUTHORITY_EVENT_GM 계정이고 투명인 상태에선 공격 받거나 받지 않는다.
		if (gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INVISABLE ) == TRUE)
			return FALSE;
	}
#endif // GMCOMMAND_EX_1ST_BUGFIX
	
#ifdef ADD_NEWPVP_PKFIELD
	if (lpObj->Type == OBJTYPE_CHARACTER && g_NewPVP.IsDuel(*lpObj) && g_NewPVP.IsSafeState(*lpObj))
	{
		return FALSE;
	}

	if (lpTargetObj->Type == OBJTYPE_CHARACTER && g_NewPVP.IsDuel(*lpTargetObj) && g_NewPVP.IsSafeState(*lpTargetObj))
	{
		return FALSE;
	}
#endif // ADD_NEWPVP_PKFIELD

	if( lpObj->MapNumber != lpTargetObj->MapNumber )
	{	// 같은 맵이 아니면, 
		return FALSE;
	}

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
	// 크라이울프 준비, 종료 시간에는 몬스터에게 피해를 입힐 수 없다.
	if( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_READY 
		|| g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_END )
	{
		if( CHECK_CRYWOLF_FIRSTZONE(lpTargetObj->MapNumber) )
		{
			if( lpTargetObj->Type == OBJTYPE_MONSTER )
				return FALSE;
		}	
	}
#endif
	
#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
	if(g_iUseCharacterAutoRecuperationSystem && lpObj->Level <= g_iCharacterRecuperationMaxLevel) 
	{
		if(lpObj->Type == OBJTYPE_CHARACTER) 
		{
			if(lpObj->Level <= g_iCharacterRecuperationMaxLevel)
				lpObj->m_iAutoRecuperationTime = GetTickCount();
		}
		if(lpTargetObj->Type == OBJTYPE_CHARACTER) 
		{
			if(lpTargetObj->Level <= g_iCharacterRecuperationMaxLevel)
				lpTargetObj->m_iAutoRecuperationTime = GetTickCount();
		}
	}
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		// CODEREVIEW_SHIELD : 변수명 이상? - dw Shield Auto Refill Timer
		lpObj->dwShieldAutoRefillTimer = GetTickCount();
	}
	if( lpTargetObj->Type == OBJTYPE_CHARACTER )
	{
		lpTargetObj->dwShieldAutoRefillTimer = GetTickCount();
	}
#endif

	int skill=0;
	if( lpMagic != NULL )
	{
		skill = lpMagic->m_Skill;
	}

#ifdef ADD_NEWPVP_PKFIELD
/*
	if (lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER)
	{
		BOOL bWideAreaSkill = FALSE;
		// 같은 길드원은 광역스킬이 적용안됨
		switch (skill)
		{
		case AT_SKILL_DEFENSEDOWN:
			bWideAreaSkill = TRUE;
			break;
		}
		if (bWideAreaSkill == TRUE && skill == AT_SKILL_DEFENSEDOWN)
		{
			if ((lpObj->PartyNumber>=0 && lpTargetObj->PartyNumber>=0)
				&& lpObj->PartyNumber == lpTargetObj->PartyNumber)
			{
				return FALSE;
			}
			if ((lpObj->lpGuild && lpTargetObj->lpGuild) 
				&& lpObj->lpGuild->Number == lpTargetObj->lpGuild->Number)
			{
				return FALSE;
			}

		}
	}
*/
#endif // ADD_NEWPVP_PKFIELD

#ifdef NEW_SKILL_FORSKYLAND
	// (천공)어둠의불사조 일 경우 
	if( lpObj->Class == 77 )
	{
		if( lpObj->m_SkyBossMonSheild && skill == 17 )
			skill = 3;
	}
#endif

	skillSuccess = 1;

	if( lpObj->GuildNumber > 0 )
	{
		if( lpObj->lpGuild != NULL )
		{
			if( lpObj->lpGuild->WarState && lpObj->lpGuild->WarType == BT_SOCCER )
			{
				if( GetBattleSoccerGoalMove(0) == 0 )
				{
					return TRUE;
				}				
			}
			
//#ifdef ADD_NEW_UNION_01_20041006
			if( lpObj->lpGuild->WarState && lpObj->lpGuild->WarType == BT_NONE && g_GlobalConfig.m_bMonsterKill_War == false )
			{
				// 일반 길전 일 경우. 몬스터 한테 데미지 안들어간다.
				if( lpTargetObj->Type == OBJTYPE_MONSTER )
					return TRUE;
			}
//#endif

		}
	}

	if(g_GlobalConfig.m_bMonsterKill_Duel == false && lpTargetObj->Type == OBJTYPE_MONSTER && gObjDuelCheck(lpObj))
	{
		return TRUE;
	}

	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
#ifdef ADD_NEW_MAP_KALIMA_20040518
		if (lpTargetObj->m_iMonsterBattleDelay > 0) 
		{
			// 머뭇거리는 동안은 맞지 않는다.
			return TRUE;
		}
#endif

		// 축구공 일 경우엔 튕기게 한다.
		if( lpTargetObj->Class == 200 )	
		{
#ifdef FOR_ONAIR
			return TRUE;
#else
			if( skill )
			{
				gObjMonsterStateProc(lpTargetObj, 7, lpObj->m_Index);
			}
			else gObjMonsterStateProc(lpTargetObj, 6, lpObj->m_Index);
			if( magicsend )
			{
				GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
			}
			return TRUE;
#endif
		}

/*#ifdef GMCOMMAND_EX
		DWORD dwAuthority = AUTHORITY_EVENT_GM | AUTHORITY_SUPER_ADMIN;
		if( CheckAuthority(dwAuthority, lpObj) == TRUE 
			&& gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INVISABLE ) == TRUE )
		{
			return FALSE;
		}
#endif // GMCOMMAND_EX*/
		
#ifdef ADD_NEW_MAP_KALIMA_20040518						
		// 마법이나 물리공격 무적인지 체크한다.
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE ) == true )
		{
			INT bCheckAttackIsMagicType = gObjCheckAttackTypeMagic (lpObj->Class, skill);
			if( bCheckAttackIsMagicType == CMagicDamage::SKILL_TYPE_MAGIC )
			{
				GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, FALSE);
				return TRUE;
			}
		}

		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_MONSTER_MELEE_IMMUNE ) == true )
		{
			INT bCheckAttackIsMagicType = gObjCheckAttackTypeMagic (lpObj->Class, skill);
			if (bCheckAttackIsMagicType == CMagicDamage::SKILL_TYPE_PHYSICS) 
			{		
				// 물리공격은 통하지 않는다.
				GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
				return TRUE;
			}
		}
	#else
		if( lpTargetObj->m_ImmuneToMagicCount > 0 )	
		{	
			// 마법공격 무적 시간 이다.
			INT bCheckAttackIsMagicType = gObjCheckAttackTypeMagic (lpObj->Class, skill);

			if (bCheckAttackIsMagicType == CMagicDamage::SKILL_TYPE_MAGIC) 
			{		
				// 마법공격은 통하지 않는다.
				GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, FALSE);
				return TRUE;
			}
		}
		
		if( lpTargetObj->m_ImmuneToHarmCount > 0 ) 
		{	
			// 물리공격 무적 시간 이다.
			INT bCheckAttackIsMagicType = gObjCheckAttackTypeMagic (lpObj->Class, skill);

			if (bCheckAttackIsMagicType == CMagicDamage::SKILL_TYPE_PHYSICS) 
			{		
				// 물리공격은 통하지 않는다.
#ifdef ADD_SHIELD_POINT_01_20060403
				GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
#else
				GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0);
#endif
				return TRUE;
			}
		}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif
		
#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
		// 마법 Immune 처리를 한다.
		if( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneTime > 0 )
		{
			if(lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneNumber == skill)
			{
#if TESTSERVER == 1
				MsgOutput(lpTargetObj->m_Index, "%d 마법Immune상태", skill);
#endif
				return TRUE;
			}
		}
#endif
		
#ifdef ADD_RAKLION_20080408
		// 물리공격, 마법 Immune 처리를 한다.
		if( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneAllTime > 0 )
		{
			
#if TESTSERVER == 1
			MsgOutput(lpTargetObj->m_Index, "물리공격, 마법Immune상태");
#endif
			//////////////////////////////////////////////////////////////////////////
			// All Miss 처리
			int MsgDamage = 0x20;
			
#ifdef ADD_SHIELD_POINT_01_20060403
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage, 0);
#else
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage);
#endif //ADD_SHIELD_POINT_01_20060403
			//////////////////////////////////////////////////////////////////////////
			
			return TRUE;
		}
#endif // ADD_RAKLION_20080408

	}
		
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( gObjIsConnected(lpObj) == FALSE ) return FALSE;

#ifdef MODIFY_CHAIN_LIGHTNING_BUGFIX_20081022
		// 기존 코드에서 왜 이걸 넣었는지는 모르지만(기획의도였겠지...;;) 변신 상태에서도 동일하게 진행하게 한다.
		// 해외팀 허진희씨가 기획팀에 확인한 결과 변신 자체에는 외형적인 모습만 변하고 스킬등 다른 부분에는 전혀 연관이 없다고 하여. 수정함.
		// 버그 레포트에는 체인라이트닝에 문제가 있다고 하지만.. 그외 combo 적용 스킬에 영향을 줄것으로 보여 아예 변신반지 처리는 하지 않는다.
		// by natinda 2008/10/22
#else
		if( lpObj->m_Change == 8 ) 
		{
			skill = 1;
			lpMagic = &DefMagicInf[1];
			magicsend = 1;
		}
#endif //MODIFY_CHAIN_LIGHTNING_BUGFIX_20081022
		

#ifdef DARKLORD_WORK
		gDarkSpirit[lpObj->m_Index].SetTarget(lpTargetObj->m_Index);
#endif
	}
	
	if( lpTargetObj->Type == OBJTYPE_CHARACTER )
	{
		if( gObjIsConnected(lpTargetObj) == FALSE ) return FALSE;
		else if(g_GlobalConfig.m_bDisableTwisting == TRUE && skill == AT_SKILL_WHEEL) return FALSE;
		else if(g_GlobalConfig.m_bDisableBlow == TRUE && skill == AT_SKILL_BLOWOFFURY) return FALSE;
#ifdef ADD_SKILL_WITH_COMBO
		else if(g_GlobalConfig.m_bDisableDecay == TRUE && skill == AT_SKILL_EXPPOISON) return FALSE;
#endif
	}
	
	if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_MONSTER)
	{
		// 공격자가 플레이어고 상대가 몬스터라면 소환수인지 체크한다.
		// 이때 자신이 소환한 몬스터일경우 공격이 안된다.
		if( lpObj->m_RecallMon >= 0 )
		{
			if( lpObj->m_RecallMon == lpTargetObj->m_Index ) return FALSE;
		}
	}
	
	// 공격해도 되는지 물어본다.
	if( gObjAttackQ(lpTargetObj) == FALSE ) return FALSE;
	
#ifdef CASTLE_MAIN_SCHEDULER_20041111		
// 공성관련 사람의 공격당함 여부 -> 공성전 중에 같은 편은 공격받으면 않된다.
//	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) 
//	{
//		if ((lpObj->m_btCsJoinSide > CS_SIEGESIDE_NONE) &&
//			(lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
//			) 
//		{
//			return FALSE;
//		}
//	}
#endif

	// 나의 소환수가 존재한다면.. 적을 체크한다
	if( lpObj->m_RecallMon >= 0 )
		gObjCallMonsterSetEnemy(lpObj, lpTargetObj->m_Index);

	lpObj->m_TotalAttackCount++;

#ifdef MODIFY_ARROW_DECREASE_ELF_20040503
	if( !AttackDamage )
	{
#endif
		// 요정 화살 감소
		
	#ifdef ADD_ITEM_FENRIR_01_20051110
		// 펜릴의 플라즈마스톰의 경우에는 화살을 감소시키지 않는다.
		if( skill != AT_SKILL_FENRIR_ATTACK )
		{
	#endif
#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INFINITY_ARROW ) == false )
	#else
			if( lpObj->m_iMuseElfInfinityArrowSkillTime == 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			{
#endif
			if( !DecreaseArrow(lpObj) )
			return FALSE;
#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
			}
#endif


	#ifdef ADD_ITEM_FENRIR_01_20051110
		}
	#endif

#ifdef MODIFY_ARROW_DECREASE_ELF_20040503
	}
#endif

	// 공격 지역을 체크 한다
	if( !CheckAttackArea(lpObj, lpTargetObj) )
		return FALSE;
	
	// 소환몬스터일 경우 주인설정
	lpCallObj = lpObj;
	if( lpObj->Type == OBJTYPE_MONSTER )
	{
		if( lpObj->m_RecallMon >= 0 ) 
		{
			lpCallObj = &gObj[lpObj->m_RecallMon];
		}
	}
	lpCallTargetObj = lpTargetObj;
	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		if( lpTargetObj->m_RecallMon >= 0 ) 
		{
			lpCallTargetObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	// 피케이 체크
	if( !PkCheck(lpCallObj, lpTargetObj) )
		return FALSE;

	int	Strength	= lpObj->Strength+lpObj->AddStrength;
	int	Dexterity	= lpObj->Dexterity+lpObj->AddDexterity;
	int	Vitality	= lpObj->Vitality+lpObj->AddVitality;
	int	Energy		= lpObj->Energy+lpObj->AddEnergy;

#ifdef DUEL_SYSTEM_20031028
	BOOL bIsOnDuel = gObjDuelCheck(lpObj, lpTargetObj);

#ifdef DUEL_SYSTEM_FIX_20031224		// 싸우는 양쪽이 서로 결투 중이라면 결투카운트를 갱신한다.
	if (bIsOnDuel) {
		lpObj->m_iDuelTickCount = GetTickCount();
		lpTargetObj->m_iDuelTickCount = GetTickCount();
	}
#endif
#endif

	// 크리스탈 검을 들고 있었다면.. 1/20 로 얼리기..
	if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,5) && 
		lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_IsValidItem )
	{
		if( rand()%20 == 0 )
		{
			skill = AT_SKILL_SLOW;
			lpMagic = &DefMagicInf[7];
			magicsend = 1;
		}
	}
	else if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type == MAKE_ITEMNUM(2,5) && 
			 lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_IsValidItem )
	{
		if( rand()%20 == 0 )
		{
			skill = AT_SKILL_SLOW;
			lpMagic = &DefMagicInf[7];
			magicsend = 1;
		}
	}

	MSBFlag = 0;
	MsgDamage = 0;

	// 저항력 체크
	skillSuccess = ResistanceCheck(lpObj, lpTargetObj, skill);

#ifdef MODIFY_ELF_ICEARROWSKILL_BUGFIX_20060914
	int skillIceArrowSuccess = skillSuccess;	// skillSuccess 값을 기억해둔다. 
	if ( skill == AT_SKILL_ELFHARDEN )
		skillSuccess = 0;						// MissCheck에서 이 값을 사용하기 때문에 0으로 설정해둠.
#endif
	
#ifdef NEW_FORSKYLAND3
	int bAllMiss = FALSE;
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	// 공격자가 투명 상태에 있다면 투명상태를 해제한다.
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INVISABLE ) == true )
	#else
	if( lpObj->m_iSkillInvisibleTime > 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	{
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
		if( ( lpObj->Authority & AUTHORITY_EVENT_GM ) != AUTHORITY_EVENT_GM )
		{
			gObjUseSkill.RemoveCloakingEffect(lpObj->m_Index);
		}
	#else
		gObjUseSkill.RemoveCloakingEffect(lpObj->m_Index);
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
	}
#endif

	BOOL bDamageReflect = FALSE;


	//---------------------------------------------------------------------------------
	//
	//  [] 공격자의 데미지 값을 얻는다.
	//
	//	1. AttackDamage 가 0 일 경우 일반 공격
	// 	2. AttackDamage 가 0 이 아닐 경우 반사 공격
	//
	//
	//---------------------------------------------------------------------------------
	if( AttackDamage == 0 )
	{
		// 미스 체크
#ifdef ADD_SHIELD_POINT_01_20060403
		if( g_ShieldSystemOn == TRUE )
		{
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				if( !MissCheckPvP( lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss ) )
				{
					return FALSE;
				}
			}
			else
			{
				if( !MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss) )
					return FALSE;
			}
		}
		else
		{
			if( !MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss) )
				return FALSE;
		}
#elif defined(NEW_FORSKYLAND3)
		if( !MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss) )
			return FALSE;
#endif

#ifdef MODIFY_ELF_ICEARROWSKILL_BUGFIX_20060914
		// 아이스애로우 스킬은 Miss 체크 후에 적용을 한다.
		if ( skill == AT_SKILL_ELFHARDEN && skillIceArrowSuccess == 1)
		{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjAddBuffEffect( lpTargetObj, BUFFTYPE_STONE, 0, 0, 0, 0, 7 );
			lpTargetObj->lpAttackObj = lpObj;
			lpTargetObj->PathCount = 0;
			lpTargetObj->PathStartEnd = 0;
			skillSuccess = 1;
#else
				lpTargetObj->m_SkillHarden = 7;
				lpTargetObj->m_SkillHardenTime = 7;

				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->m_ViewSkillState |= STATE_HARDEN; //	석화
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				lpTargetObj->PathCount = 0;

	#ifdef MODIFY_MONSTER_ACT_BY_ICEARROW_20050914
				lpTargetObj->PathStartEnd = 0;
	#endif
				skillSuccess = 1;	// 값 복구
				gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
		}
#endif // MODIFY_ELF_ICEARROWSKILL_BUGFIX_20060914

		// 대상의 방어력을 얻는다
		int targetdefense = GetTargetDefense(lpObj, lpTargetObj, MsgDamage);

#ifdef SEASON4_PATCH_GROOVING
		//------>
		// 시즌 4.0 추가 스킬  grooving 2008/04/24
		// grooving 버프 방어력
		if( true == gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_BERSERKER) )
		{
			float fDefense = (lpTargetObj->Dexterity + lpTargetObj->AddDexterity) / 3;
			float fValue = gObjGetTotalValueOfEffect( lpTargetObj, EFFECTTYPE_BERSERKER_DOWN );
			float fPercent = (40.0f - fValue) / 100.0f;

			fPercent = max(0.1f, fPercent);
			
			targetdefense -= (fDefense * fPercent);
		}
		//<-----------
#endif
	

#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111	// 이너베이션(방어력 하락)
		int iCurseValue = 0;
		gObjGetValueOfBuffIndex( lpTargetObj, BUFFTYPE_CURSED_DEFENSE_DOWN, &iCurseValue );
		targetdefense	-= targetdefense * iCurseValue / 100;
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111		


		//---------------------------------------------------------------------------------
		//	
		//	[][] 기본 데미지
		//
		//---------------------------------------------------------------------------------
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
		if( lpTargetObj->m_iSkillNPCDefense ) 
		{
			targetdefense += lpTargetObj->m_iSkillNPCDefense;			
		}
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( skill == AT_SKILL_SWORD1  ||	//내려찍기
			skill == AT_SKILL_SWORD2  ||	//찌르기
			skill == AT_SKILL_SWORD3  ||	//올려치기
			skill == AT_SKILL_SWORD4  ||	//돌려치기 
			skill == AT_SKILL_SWORD5  ||	//베기
#ifdef ADD_SKILL_20031022
			skill == AT_SKILL_SWORD6  ||	//파워슬래쉬
#endif			
			skill == AT_SKILL_WHEEL   ||	//회오리

			// 기사 추가 스킬
			skill == AT_SKILL_KNIGHTSPEAR ||	// 유니창
			skill == AT_SKILL_BLOWOFFURY  ||	// 분노의 일격
			skill == AT_SKILL_KNIGHTDINORANT || // 디노란트 공격
			skill == AT_SKILL_STRIKE	  ||	// 블로우
			
			// 마검사 추가 스킬
			skill == AT_SKILL_DEFENSEDOWN  // 블러드 어택
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
			|| skill ==AT_SKILL_RUSH			// 반월베기(기사)
			|| skill ==AT_SKILL_ONE_FLASH		// 나선베기 (마검)
			|| skill ==AT_SKILL_SPACE_SPLIT		// 파이어브래스트 (다크)
#endif
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 회오리베기 강화 - 기본 데미지값 구하기(여기는 마검사 스킬만 추가 해 주면 된다;)
			|| ( g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT )	// 회오리베기 강화(기사)
			|| ( g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_WHEEL_MAGUMSA )	// 회오리베기 강화(마검사)
#endif	
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 마스터레벨 4랭크 스킬 기본 데미지: 블로우 강화, 분노의 일격 강화
			||g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_STRIKE			// 블로우 강화(기사)
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_BLOWOFFURY	// 분노의 일격 강화(기사)
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN	// 블러드 어택 강화(마검)
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_SWORD6		// 파워 슬래쉬 강화(마검)
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428			
#ifdef ADD_SEASON4_NEW_SKILL_20080522
			|| skill == AT_SKILL_MULTISHOT		// 멀티샷(요정)
			|| skill == AT_SKILL_DESTRUCTION	// 파괴의 일격
			|| skill == AT_SKILL_FLAMESTRIKE	// 플레임 스트라이크
			|| skill == AT_SKILL_CHAOTICDESAIR	// 카오틱 디세이어(다크로드)
#endif	// ADD_SEASON4_NEW_SKILL_20080522			
			)
		{
#ifdef DUEL_SYSTEM_20031028
			#ifdef ADD_SKILL_WITH_COMBO
				AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage, bIsOnDuel, lpMagic);
			#else
				AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage, bIsOnDuel);
			#endif
#else
			#if defined( ADD_SKILL_WITH_COMBO ) || GAME_VERSION == G_V_97D
				AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage, lpMagic);
			#else
				AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage);
			#endif
#endif
				
#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
			AttackDamage += lpObj->m_iSkillNPCAttack;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif

			// 데미지가 있으면 내구도를 깍는다
			if( AttackDamage > 0 )
			{
				gObjWeaponDurDown(lpObj, lpTargetObj, 0);
			}
		}
#ifdef ADD_ITEM_FENRIR_01_20051110
		else if( skill ==AT_SKILL_FENRIR_ATTACK )
		{
			int iFenrirAttackDmg = 0;

			// 각 직업별로 플라즈마스톰 데미지를 구한다.
			if( lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA )
			{
				iFenrirAttackDmg = lpObj->Strength/3 + lpObj->Dexterity/5 + lpObj->Vitality/5 + lpObj->Energy/7; 
			}
			else if( lpObj->Class == CLASS_WIZARD )
			{
				iFenrirAttackDmg = lpObj->Strength/5 + lpObj->Dexterity/5 + lpObj->Vitality/7 + lpObj->Energy/3; 
			}
			else if( lpObj->Class == CLASS_ELF )
			{
				iFenrirAttackDmg = lpObj->Strength/5 + lpObj->Dexterity/3 + lpObj->Vitality/7 + lpObj->Energy/5; 
			}
			else
			{
				iFenrirAttackDmg = lpObj->Strength/5 + lpObj->Dexterity/5 + lpObj->Vitality/7 + lpObj->Energy/3 + lpObj->Leadership/3; 
			}

			if( iFenrirAttackDmg < 0 )
			{
				iFenrirAttackDmg = 0;
			}
			
			// 크리티컬, 엑설런트 데미지 여부를 결정한다.
			if( lpObj->m_CriticalDamage > 0 ) 
			{
				if( (rand()%100) < lpObj->m_CriticalDamage )
				{
					MsgDamage = DT_CRITICAL;		// 크리티컬 데미지
				}
			}
			if( lpObj->m_ExcelentDamage > 0 )
			{
				// 세트 옵션 ; 엑설런트 데미지 확률 증가적용
				if( (rand()%100) < lpObj->m_ExcelentDamage )
				{
					MsgDamage = DT_EXCELLENT;		// 엑설런트 데미지
				}
			}

			// 크리티컬, 엑설런트 데미지 결정
			if( MsgDamage == DT_CRITICAL )
			{
				AttackDamage = iFenrirAttackDmg + lpMagic->m_DamageMax;
				AttackDamage += lpObj->SetOpAddCriticalDamage;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				int iValue = 0;
				gObjGetValueOfBuffIndex( lpObj, BUFFTYPE_CRITICAL_DMG_INC, &iValue );
				AttackDamage += iValue;
#else
				AttackDamage += lpObj->SkillAddCriticalDamage;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// 조화의 보석 강화 아이템 옵션 : 크리티컬 데미지 증가
				AttackDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530				
				AttackDamage -= targetdefense;
			}
			else if( MsgDamage == DT_EXCELLENT )
			{
				AttackDamage = iFenrirAttackDmg + lpMagic->m_DamageMax;
				AttackDamage += AttackDamage * 20 / 100;
				AttackDamage += lpObj->SetOpAddExDamage;
				AttackDamage -= targetdefense;
			}
			else
			{
				AttackDamage = iFenrirAttackDmg + lpMagic->m_DamageMin + rand()%(lpMagic->m_DamageMax-lpMagic->m_DamageMin+1);
				AttackDamage -= targetdefense;
			}

#if TESTSERVER == 1
			if( gObjFenrir(lpObj) )
			{
				MsgOutput(lpObj->m_Index, "[플라즈마스톰기본DMG] %d", AttackDamage);

			}
#endif
			
		}
#endif
		else if( ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_MAGUMSA ) && skill )
		{
#ifdef DUEL_SYSTEM_20031028
			AttackDamage = GetAttackDamageWizard(lpObj, targetdefense, lpMagic, MsgDamage, bIsOnDuel);
#else
			AttackDamage = GetAttackDamageWizard(lpObj, targetdefense, lpMagic, MsgDamage);
#endif

#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
			AttackDamage += lpObj->m_iSkillNPCAttack;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif

			// 데미지가 있으면 내구도를 깍는다
			if( AttackDamage > 0 )
			{
				gObjWeaponDurDown(lpObj, lpTargetObj, 1);
			}
		}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 소환술사 스킬 데미지
		else if ( lpObj->Class == CLASS_SUMMONER  && skill)
		{
			AttackDamage = GetAttackDamageSummoner(lpObj, targetdefense, lpMagic, MsgDamage, bIsOnDuel);

#if TESTSERVER == 1
			//TRACE_LOG("\n1.Calc AttackDamageSummoner %d \n",AttackDamage);
#endif
			
			// 데미지가 있으면 내구도를 깍는다
			if( AttackDamage > 0 )
			{
				gObjWeaponDurDown(lpObj, lpTargetObj, 1);
			}
		}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		else
		{
#ifdef DUEL_SYSTEM_20031028
			#ifdef ADD_SKILL_WITH_COMBO
				AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage, bIsOnDuel, lpMagic);
			#else
				AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage, bIsOnDuel);
			#endif
#else
			#if defined( ADD_SKILL_WITH_COMBO ) || GAME_VERSION == G_V_97D
				AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage, lpMagic);
			#else
				AttackDamage = GetAttackDamage(lpObj, targetdefense, MsgDamage);
			#endif
#endif

#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
			AttackDamage += lpObj->m_iSkillNPCAttack;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif

			// 데미지가 있으면 내구도를 깍는다
			if( AttackDamage > 0 )
			{
				gObjWeaponDurDown(lpObj, lpTargetObj, 0);
			}
		}

#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
		//전투, 강화의 스크롤 (크리티컬, 엑설런트 데미지)처리
		//모든 Class 가 적용되도록 서브 함수가 아닌 아래에 처리 추가
		//////////////////////////////////////////////////////////////////////////
		{
			int nBuffIndex = -1;
			switch (MsgDamage) {
			case DT_CRITICAL:	nBuffIndex = BUFFTYPE_PCS_SCROLL_CRITICAL;		break;
			case DT_EXCELLENT:	nBuffIndex = BUFFTYPE_PCS_SCROLL_EXCELLENT;		break;
			default:			nBuffIndex = -1;
			}
			
			int nValue = 0;
			int _AttackDamage = AttackDamage;
			if (nBuffIndex > -1) {
				gObjGetValueOfBuffIndex( lpObj, nBuffIndex, &nValue );
				if (nValue > 0) {
					AttackDamage += (AttackDamage * nValue/100);
				}
			}
			
			/*if (g_bAbilityDebug == TRUE && (MsgDamage == DT_CRITICAL || MsgDamage == DT_EXCELLENT)) {
				char szMsg[256];
				sprintf(szMsg, "%s AttackDamage: %d %d", (MsgDamage == DT_CRITICAL)?"CRITICAL":"EXCELLENT", _AttackDamage, AttackDamage);
				GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
			}*/
		}
		//////////////////////////////////////////////////////////////////////////
#endif // ADD_CASHITEM_4RD_SCROLL_20080723


		//---------------------------------------------------------------------------------
		//	
		//	[][] ALL Miss 처리
		//
		//---------------------------------------------------------------------------------
#ifdef NEW_FORSKYLAND3
		if( bAllMiss )
		{

#ifdef MODIFY_SUCCESSBLOCKING_ALLMISS_20040408
			AttackDamage =  AttackDamage*30/100;
#else
			AttackDamage =  AttackDamage*20/100;
#endif

		}
#endif
	
		//---------------------------------------------------------------------------------
		//	
		//	[][] 데미지 감소 옵션
		//
		//---------------------------------------------------------------------------------

		// 데미지를 감소 시킨다.
		if( lpTargetObj->DamageMinus )
		{
			int beforeDamage=AttackDamage;
			AttackDamage -=  AttackDamage*lpTargetObj->DamageMinus/100;
			//if( AttackDamage < 0 ) AttackDamage = 0;
			//LogAdd("데미지 감소 : %d -> %d (%d)",beforeDamage, AttackDamage,lpTargetObj->DamageMinus);
		}

		//---------------------------------------------------------------------------------
		//	
		//	[][] 레벨에 따른 최소 데미지 적용
		//
		//---------------------------------------------------------------------------------

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 종합레벨 적용 - 레벨에 따른 최소 데미지 적용
		// 때린녀석의레벨/10 보다 작을 경우 AttackDamage 수정 
		int tlevel = ( lpObj->Level + lpObj->m_nMasterLevel ) / 10;
		if( AttackDamage < tlevel )
		{
			if( tlevel < 1 ) tlevel = 1;
			AttackDamage = tlevel;
		}
#else
		// 때린녀석의레벨/10 보다 작을 경우 AttackDamage 수정 
		int tlevel = lpObj->Level/10;
		if( AttackDamage < tlevel )
		{
			if( tlevel < 1 ) tlevel = 1;
			AttackDamage = tlevel;
		}
#endif

		//---------------------------------------------------------------------------------
		//	
		//	[][] 방어 스킬 사용
		//
		//---------------------------------------------------------------------------------

		// 방어스킬 사용시
		if( lpTargetObj->m_SkillNumber == AT_SKILL_BLOCKING )
			if( AttackDamage > 1 ) AttackDamage >>= 1;

		//---------------------------------------------------------------------------------
		//	
		//	[][] 사탄, 수호천사
		//
		//---------------------------------------------------------------------------------

		// 상대의 정령을 검사해서 에너지를 감소 시킨다.
		gObjSpriteDamage(lpTargetObj, AttackDamage);					

		// 사탄정령을 가지고 있다면 공격력 증가..
		if( gObjSatanSprite(lpObj) == TRUE )
		{		
			lpObj->Life -= 3;
			if( lpObj->Life < 0 ) 
			{
				lpObj->Life = 0;
			}
			else
			{
				AttackDamage = AttackDamage*13/10;
			}
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif
		}

#if TESTSERVER == 1
/*
		// 테스트 코드
		{
			if(lpObj->Type == OBJTYPE_CHARACTER && skill == 0)
				TRACE_LOG("2. Hand AttackDamage %d \n",AttackDamage);

			if(lpObj->Type == OBJTYPE_CHARACTER && skill != 0)
				TRACE_LOG("2. Skill AttackDamage %d Skill Index: %d \n",AttackDamage, skill);
		}
*/
#endif // TESTSERVER

		// 상대가 천사정령을 가지고 있다면.. 
		if( gObjAngelSprite(lpTargetObj) == TRUE )
		{
			if( AttackDamage > 1 ) 
			{
#ifdef NEW_FORSKYLAND3	// 수호천사 데미지 흡수 30%에서 20%로 조정
				float damage = (float)(AttackDamage*8)/10;
#else
				float damage = (float)(AttackDamage*7)/10;
#endif
				AttackDamage = (int)damage;// 10분의 7은 사용자가..  
			}
		}

		//---------------------------------------------------------------------------------
		//	
		//	[][] 데몬, 수호정령
		//
		//---------------------------------------------------------------------------------
		
#ifdef	ADD_SAFEGUARD_ITEM_20080702

		// grooving 데몬 공격 40% 증가
		if( gObjDaemonSprite(lpObj) == TRUE )
		{
			if (AttackDamage > 1) 
			{
				AttackDamage = AttackDamage*14/10;
			}
		}
		
		if( gObjSafeGuardSprite(lpTargetObj) == TRUE )
		{		
			if( AttackDamage > 1 ) 
			{
				float damage = (float)(AttackDamage*7)/10;
				
				AttackDamage = (int)damage;// 10분의 7은 사용자가..  
			}
		}
		
#endif // ADD_SAFEGUARD_ITEM_20080702
		
#ifdef ADD_PREMIUMITEM_PANDA
		if( (lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() 
			&& lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13, 76)
			&& lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability > 0) ||
			(lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() 
			&& lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13, 76)
			&& lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability > 0)
		  )
		{
			if (AttackDamage > 1) 
			{
				// 공격력 30 증가
				AttackDamage += 30;
			}
		}
#endif // ADD_PREMIUMITEM_PANDA


		//---------------------------------------------------------------------------------
		//	
		//	[][] 날개
		//
		//---------------------------------------------------------------------------------

		if( gObjWingSprite(lpObj) == TRUE )
		{
			CItem	*Wing = &lpObj->pInventory[EQUIPMENT_WING];
			// 날개 법사 적용됨(20030721)
#ifdef NEW_FORSKYLAND3
			if( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_ELF)
#else
			if( lpObj->Class == CLASS_WIZARD )				
#endif
				lpObj->Life -= 1;
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 소환술사가 날개를 차고 있으면 공격성공시 hp 1씩 감소
			else if( lpObj->Class == CLASS_SUMMONER )
			{
				lpObj->Life -= 1;
			}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
			else
				lpObj->Life -= 3;
			
			if( lpObj->Life < 0 ) 
			{
				lpObj->Life = 0;
			}
			else
			{
#ifdef ADD_THIRD_WING_20070525	// 3차날개 데미지 증가
				if( Wing->m_Type >= MAKE_ITEMNUM( 12, 36 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 40 ) )
				{
					// 데미지 증가 : 39+Level*2
					AttackDamage = AttackDamage * ( 139 + ( Wing->m_Level * 2 )) / 100;
				}
				else				
#endif	// ADD_THIRD_WING_20070525	
#ifdef NEW_FORSKYLAND3				
#ifdef DARKLORD_WORK
				if( Wing->m_Type == MAKE_ITEMNUM(13,30) )
				{	// 20+Level*2
					AttackDamage = AttackDamage*(120+(Wing->m_Level*2))/100;
				}
				else
#endif
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// 절망의 날개 차원의 날개 데미지 증가
				if(Wing->m_Type == MAKE_ITEMNUM(12,41))		// 재앙의 날개
				{
					// 데미지 증가 : 12+Level
					AttackDamage = AttackDamage*(112+(Wing->m_Level*2))/100;	// 12% + level
				}
				else if(Wing->m_Type == MAKE_ITEMNUM(12,42))	// 절망의 날개
				{
					// 데미지 증가 : 32+Level*1
					AttackDamage = AttackDamage * ( 132 + ( Wing->m_Level * 1)) / 100;
				}
				else if(Wing->m_Type == MAKE_ITEMNUM(12,43))	// 차원의 날개
				{
					// 데미지 증가 : 39+Level*2
					AttackDamage = AttackDamage * ( 139 + ( Wing->m_Level * 2 )) / 100;
				}
				else
#endif	// ADD_SUMMONER_SECOND_THIRD_WING_20071130					
				if( Wing->m_Type > MAKE_ITEMNUM(12,2) )
				{	// 추가 날개 (데미지 증가)	
					AttackDamage = AttackDamage*(132+(Wing->m_Level))/100;
//					AttackDamage = AttackDamage*(132+(Wing->m_Level*2))/100;
				}
				else
#endif					
					AttackDamage = AttackDamage*(112+(Wing->m_Level*2))/100;	// 12% + level
			}
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif
			
		}

		if( gObjWingSprite(lpTargetObj) == TRUE )
		{
			CItem	*Wing = &lpTargetObj->pInventory[EQUIPMENT_WING];

#ifdef DARKLORD_WORK
			if( Wing->m_Type != MAKE_ITEMNUM(13,30) )
#endif
			{
				if( AttackDamage > 1 ) 
				{
#ifdef ADD_THIRD_WING_20070525	// 3차날개 데미지 흡수, 랜덤옵션 - 100%마나/생명 회복
					// 일단 데미지 흡수
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// 차원의 날개 데미지 흡수
					if( ( Wing->m_Type >= MAKE_ITEMNUM( 12, 36 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 40 ) )
						|| Wing->m_Type == MAKE_ITEMNUM( 12, 43 ) )	// 차원의 날개
#else
					if( Wing->m_Type >= MAKE_ITEMNUM( 12, 36 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 40 ) )
#endif
					{
						if( Wing->m_Type == MAKE_ITEMNUM( 12, 40 ) )	// 제왕의망토
						{
							// 24% + ( level * 2 )
							float damage = ( float )( AttackDamage * ( 76 - ( Wing->m_Level * 2 )) ) / 100;
							AttackDamage = ( int )damage;
						}
						else
						{
							// 3차 날개는 	39% + ( level * 2 )
							float damage = ( float )( AttackDamage * ( 61 - ( Wing->m_Level * 2 )) ) / 100;
							AttackDamage = ( int )damage;	
						}			

						// 랜덤 옵션
						if( rand() % 100 < 5 )
						{
							BYTE bt3rdWingOpt = lpTargetObj->pInventory[EQUIPMENT_WING].m_NewOption;								
							if( ( bt3rdWingOpt & PLUS_THIRD_WING_RECOVER_100PER_LIFE ) == PLUS_THIRD_WING_RECOVER_100PER_LIFE ) 
							{
								// 5%확률로 100% 생명 증가
								gObjAddMsgSendDelay( lpTargetObj, MSG_FULL_LIFE, lpObj->m_Index, 100, 0 );			
							}
							else if( ( bt3rdWingOpt & PLUS_THIRD_WING_RECOVER_100PER_MASIC ) == PLUS_THIRD_WING_RECOVER_100PER_MASIC )
							{
								// 5%확률로 100% 마나 증가
								gObjAddMsgSendDelay( lpTargetObj, MSG_FULL_MANA, lpObj->m_Index, 100, 0 );
							}						
						}				
					}
					else
#endif
#ifdef NEW_FORSKYLAND3
					if( Wing->m_Type > MAKE_ITEMNUM(12,2) )
					{	// 날개 데미지 흡수 추가 날개는 25% + level
						float damage = (float)(AttackDamage*(75-(Wing->m_Level*2)))/100;
						AttackDamage = (int)damage;
					}
					else
#endif			
					{	// 날개 데미지 흡수 날개 12% + level*2
						float damage = (float)(AttackDamage*(88-(Wing->m_Level*2)))/100;
						AttackDamage = (int)damage;	
					}
					//LogAdd("[%s][%s] 날개 착용 효과2 %d", lpObj->AccountID, lpObj->Name, AttackDamage);
				}		
			}
		}

		//---------------------------------------------------------------------------------
		//	
		//	[][] 디노란트
		//
		//---------------------------------------------------------------------------------

#ifdef NEW_FORSKYLAND3
		if( gObjDenorantSprite(lpObj) )
		{	// 디노란트 데미지 증가 15%
			
			// 디노란트에 타고 공격하고 있으면 공격자의 라이프를 1씩 깎는다.
			lpObj->Life -= 1;
			
			if( lpObj->Life < 0 ) 
			{
				lpObj->Life = 0;
			}
			else
			{
				AttackDamage = AttackDamage*115/100;
			}
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif
		}

		if( gObjDenorantSprite(lpTargetObj) )
		{	// 디노란트 데미지 흡수 10%
			CItem	*Dinorant = &lpTargetObj->pInventory[EQUIPMENT_HELPER];

			// 5%추가 데미지 흡수 옵션 적용
			int	dinorantdecdamage = 90-Dinorant->IsDinorantReduceAttackDamaege();			
			lpObj->Life -= 1;
			
			if( lpObj->Life < 0 ) 
			{
				lpObj->Life = 0;
			}
			else
			{
				AttackDamage = AttackDamage*dinorantdecdamage/100;
			}
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
			GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif
		}
#endif

		//---------------------------------------------------------------------------------
		//	
		//	[][] 다크 호스 
		//
		//---------------------------------------------------------------------------------

#ifdef DARKLORD_WORK
		if( gObjDarkHorse(lpTargetObj) )
		{	
			// 다크호스 데미지 흡수 (10+(통솔/100))
			CItem	*Darkhorse = &lpTargetObj->pInventory[EQUIPMENT_HELPER];	

			int	decdamage = 100-((30+Darkhorse->m_PetItem_Level)/2);			

			lpTargetObj->Life -= 1;
			
			if( lpTargetObj->Life < 0 ) 
			{
				lpTargetObj->Life = 0;
			}
			else
			{
				AttackDamage = AttackDamage*decdamage/100;
			}
			
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0, (WORD)lpTargetObj->iShield);
#else
			GCReFillSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0);
#endif
		}
#endif

		if( lpTargetObj->Live ) 
		{

			//---------------------------------------------------------------------------------
			//	
			//	[][][] 스킬 데미지 값 구하기
			//
			//---------------------------------------------------------------------------------

#if defined( ADD_SKILL_WITH_COMBO ) || GAME_VERSION == G_V_97D

			switch(skill)
			{
				case AT_SKILL_SWORD1:			//	내려찍기
				case AT_SKILL_SWORD2:			//	찌르기
				case AT_SKILL_SWORD3:			//	올려치기
				case AT_SKILL_SWORD4:			//	돌려치기
				case AT_SKILL_SWORD5:			//	베기
				case AT_SKILL_WHEEL:			//	회오리베기		
				case AT_SKILL_BLOWOFFURY:		//	분노의 일격
				case AT_SKILL_STRIKE:			//	블로우(기사)
				case AT_SKILL_KNIGHTDINORANT:	//	디노란트 공격
				case AT_SKILL_DEFENSEDOWN:		//	블러드 어택(마검)
	#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
				case AT_SKILL_RUSH:				//	반월베기(기사)
				case AT_SKILL_ONE_FLASH:		//	나선베기 (마검)
	#endif
				{
					// 스킬 데미지(에너지/10)
	#ifdef DARKLORD_WORK					
					if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )
	#else
					if( lpObj->Class == CLASS_MAGUMSA )
	#endif	// DARKLORD_WORK					
						AttackDamage = AttackDamage*2;
					else 
						AttackDamage = AttackDamage*(200+Energy/10)/100;

				}
				break;
				
				case AT_SKILL_KNIGHTSPEAR:	// 유니창
					{	
						if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) || 
							lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) 
	#ifdef ADD_ITEM_FENRIR_01_20051110
							// 펜릴 착용시에도 창찌르기 기술 가능
							|| lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,37)
	#endif
						 )

						{	// 유니리아 디노란트
	#ifdef DARKLORD_WORK					
							if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )
	#else
							if( lpObj->Class == CLASS_MAGUMSA )
	#endif	//DARKLORD_WORK
								AttackDamage = AttackDamage*2;
							else 
								AttackDamage = AttackDamage*(200+Energy/10)/100;
						}						
					}
				break;

				case AT_SKILL_SWORD6:			// 마검스킬
				{	
					//AttackDamage = AttackDamage*190/100;
					AttackDamage = AttackDamage*2;
					break;
				}

	#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
				case AT_SKILL_DEEPIMPACT:		//딥임팩트(뮤즈엘프)
	#endif
				case AT_SKILL_PENETRATION:		//관통(요정)
				case AT_SKILL_ELFHARDEN:		// 석화(요정)
					{	
						AttackDamage = AttackDamage*2;
					}
				break;	
				
	#ifdef DARKLORD_WORK
		#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
				case AT_SKILL_SPACE_SPLIT:		// 파이어브래스트 (다크)
		#endif
				case AT_SKILL_SPEAR:			// 마검스킬
				case AT_SKILL_FIREBURST:				
				case AT_SKILL_DARKHORSE_ATTACK:
				case AT_SKILL_ELECTRICSPARK:
		#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
				case AT_SKILL_FIRESCREAM:		// 파이어스크림 (다크)
		#endif
					AttackDamage = AttackDamage*(200+(lpObj->Energy+lpObj->AddEnergy)/20)/100;
					break;
	#endif // DARKLORD_WORK

	#ifdef ADD_ITEM_FENRIR_01_20051110
				case AT_SKILL_FENRIR_ATTACK:	// 펜릴 스킬 - 플라즈마 스톰
					{
						// 펜릴 최소 착용 가능 레벨 보다 얼마나 레벨이 큰지 구한다.
						int iDamageInc = lpObj->Level - MIN_FENRIR_EQUIP_LEVEL ;

		#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 종합레벨 적용 - 플라즈마스톰 사용시 특정 레벨마다 데미지 증가분 상승.
						iDamageInc += lpObj->m_nMasterLevel;
		#endif

						if( iDamageInc < 0 )
						{
							iDamageInc = 0;
						}

						// 특정 레벨 마다 데미지 증가 분이 상승한다.
						iDamageInc = iDamageInc / MAX_FENRIR_DAMAGE_INC_LEVEL;

						AttackDamage = AttackDamage * (200+iDamageInc) / 100;

					}
					break;
	#endif // ADD_ITEM_FENRIR_01_20051110

	#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 스킬 데미지
				case AT_SKILL_LIGHTNINGORB:
					{
						// 주변 스플레쉬데미지
						gObjUseSkill.SplashDamage(lpObj, lpTargetObj, AT_SKILL_LIGHTNINGORB, AttackDamage);
					}
					break;
					
				case AT_SKILL_DRAINLIFE:
					{
						
						//int nAddHP = AttackDamage * 20 / 100;
						int nAddHP = 0;
						if(lpTargetObj->Type == OBJTYPE_MONSTER)
						{
							// 에너지/15 + 몬스터레벨 / 2.5를 체력으로 흡수
							nAddHP = (lpObj->Energy / 15) + (lpTargetObj->Level / 2.5);
						}
						else if( lpTargetObj->Type == OBJTYPE_CHARACTER )
						{
							// (최종데미지 * 10%) + (에너지 / 23 )
							nAddHP = (AttackDamage * 10 / 100) + (lpObj->Energy / 23);
						}

						int tmpLife = lpObj->Life + nAddHP;
						if(tmpLife > lpObj->MaxLife+lpObj->AddLife)
						{
							lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
						}
						else
						{
							lpObj->Life = tmpLife;
						}

						GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
					}
					break;
				case AT_SKILL_CHAINLIGHTNING:
					{
						switch(bCombo)
						{
						case 1:	// 100 %
							break;
						case 2:	// 80 %
							AttackDamage = AttackDamage * 70 / 100;
							break;
						case 3:	// 50 %
							AttackDamage = AttackDamage * 50 / 100;
							break;
						default:
							// error
							return FALSE;
							break;
						}

						bCombo = 0;
					}
					break;
				case AT_SKILL_EXPLOSION_2:
					{
						int DotDamage = AttackDamage * 60 / 100;
						// 데미지가 들어갔으면 일정시간 데미지 감소
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_FIREDOT, EFFECTTYPE_GIVE_DMG_TICK, DotDamage , 0, 0, 5);
					}
					break;
				case AT_SKILL_REQUIEM:
					{
						int DotDamage = AttackDamage * 60 / 100;
						// 데미지가 들어갔으면 일정시간 데미지 감소
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLOODDOT, EFFECTTYPE_GIVE_DMG_TICK, DotDamage, 0, 0, 5);
					}
					break;
				case AT_SKILL_SLEEP:
					{
						// 여기서 다시 성공률 계산
						int skillSuccessRate = 0;
#ifdef MODIFY_SLEEP_SKILL_SUCCESSRATE_BUGFIX_20080125	// 성공률 수정
						if( lpTargetObj->Type == OBJTYPE_MONSTER)
						{
							skillSuccessRate = 20 + ( lpObj->Energy / 30 ) + ( lpObj->m_Curse / 6 );
						}
						else if(lpTargetObj->Type == OBJTYPE_CHARACTER)
						{
							skillSuccessRate = 15 + (lpObj->Energy / 37) + (lpObj->m_Curse / 6);
						}
#else	// MODIFY_SLEEP_SKILL_SUCCESSRATE_BUGFIX_20080125
						if( lpTargetObj->Type == OBJTYPE_MONSTER)
						{
							skillSuccessRate = 15 + (( lpObj->Energy / 9 * lpObj->m_Curse) / 8 ) / 100;
						}
						else if(lpTargetObj->Type == OBJTYPE_CHARACTER)
						{
							skillSuccessRate = 10 + (lpObj->m_Curse / 3) / 100;
						}
#endif	// MODIFY_SLEEP_SKILL_SUCCESSRATE_BUGFIX_20080125

						if( skillSuccessRate < rand()%100)
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
							return FALSE;
						}
						
						int nEffectTime = 0;
#ifdef MODIFY_SLEEP_SKILL_SUCCESSRATE_BUGFIX_20080125	// 시간 수정
						if( lpTargetObj->Type == OBJTYPE_MONSTER)
						{
							nEffectTime	= 5 + (lpObj->Energy / 100) - (lpTargetObj->Level / 20);
						}
						else if (lpTargetObj->Type == OBJTYPE_CHARACTER)
						{
							nEffectTime = 4 + (lpObj->Energy / 250) + ((lpObj->Level - lpTargetObj->Level) / 100);
						}
#else	// MODIFY_SLEEP_SKILL_SUCCESSRATE_BUGFIX_20080125
						if( lpTargetObj->Type == OBJTYPE_MONSTER)
						{
							nEffectTime	= 10 + (lpObj->Energy / 150) - (lpTargetObj->Level / 10);
						}
						else if (lpTargetObj->Type == OBJTYPE_CHARACTER)
						{
							nEffectTime = (10 / 2) + (lpObj->Energy / 250) + ((lpObj->Level - lpTargetObj->Level) / 100);
						}
#endif	// MODIFY_SLEEP_SKILL_SUCCESSRATE_BUGFIX_20080125

						if(nEffectTime < 1)
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
							return FALSE;
						}
						
						// 낮은 수준의 버퍼는 적용안함
						if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_SLEEP, nEffectTime, 0 ) == true )
						{
							GCMagicAttackNumberSend( lpObj, AT_SKILL_SLEEP, lpTargetObj->m_Index, 0 );	// 스킬사용 실패
							return FALSE;
						}
						// 버프적용
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_SLEEP, EFFECTTYPE_SLEEP, nEffectTime, 0, 0, nEffectTime);

						return TRUE;
					}
					break;

	#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
				case AT_SKILL_WEAKNESS:
					{
						int nSkillSuccessRate = 0;
						// 성공률 계산
						if( lpTargetObj->Type == OBJTYPE_MONSTER)
						{
							nSkillSuccessRate = 32 + (lpObj->Energy / 50) + (lpObj->m_Curse / 6);
						}
						else if(lpTargetObj->Type == OBJTYPE_CHARACTER)
						{
							nSkillSuccessRate = 17 + (lpObj->Energy / 50) + (lpObj->m_Curse / 6);
						}

						if( nSkillSuccessRate >= rand()%100)
						{
							int nEffectValue	= 0;
							int nEffectTime		= 0;
						
							if( lpTargetObj->Type == OBJTYPE_MONSTER)
							{
								int tmpEnergy = 0;
								if(lpObj->Energy > 4000)
								{
									tmpEnergy = 4000;
								}
								nEffectValue	= 4 + (lpObj->Energy / 58) ;
								nEffectTime		= (4 + (lpObj->Energy / 100) - (lpTargetObj->Level / 20) );
							}
							else if(lpTargetObj->Type == OBJTYPE_CHARACTER)
							{
								int tmpEnergy = 0;
								if(lpObj->Energy > 4000)
								{
									tmpEnergy = 4000;
								}
								nEffectValue	= 3 + (lpObj->Energy / 93);
#ifdef MODIFY_SUMMONER_WEAKENESS_ENERVATION_PVPEFFECTTIME_BUGFIX_20080404
								nEffectTime		= (5 + (lpObj->Energy / 300) + ((lpObj->Level -  lpTargetObj->Level) / 150) );
#else	// MODIFY_SUMMONER_WEAKENESS_ENERVATION_PVPEFFECTTIME_BUGFIX_20080404
								nEffectTime		= (5 + (lpObj->Energy / 300) - ((lpObj->Level -  lpTargetObj->Level) / 150) );
#endif	// MODIFY_SUMMONER_WEAKENESS_ENERVATION_PVPEFFECTTIME_BUGFIX_20080404
							}
							
							if(nEffectTime < 1)
							{
								GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
								return FALSE;
							}

							if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_CURSED_ATTACK_DOWN, nEffectValue, 0 ) == true )
							{
								GCMagicAttackNumberSend( lpObj, AT_SKILL_WEAKNESS, lpTargetObj->m_Index, 0 );	// 스킬사용 실패
								return FALSE;
							}
							
							gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CURSED_ATTACK_DOWN, EFFECTTYPE_CURSED_ATTACKDOWN, nEffectValue, 0, 0, nEffectTime );
							return TRUE;
						}
						else
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
							return FALSE;
						}
					}
					break;
				case AT_SKILL_ENERVATION:
					{
						int nSkillSuccessRate = 0;
						// 성공률 계산
						if( lpTargetObj->Type == OBJTYPE_MONSTER)
						{
							nSkillSuccessRate = 32 + (lpObj->Energy / 50) + (lpObj->m_Curse / 6);
						}
						else if(lpTargetObj->Type == OBJTYPE_CHARACTER)
						{
							nSkillSuccessRate = 17 + (lpObj->Energy / 50) + (lpObj->m_Curse / 6);
						}

						if( nSkillSuccessRate >= rand()%100)
						{
							int nEffectValue	= 0;
							int nEffectTime		= 0;
						
							if( lpTargetObj->Type == OBJTYPE_MONSTER)
							{
								int tmpEnergy = 0;
								if(lpObj->Energy > 4000)
								{
									tmpEnergy = 4000;
								}
								nEffectValue	= 20 + (lpObj->Energy / 90) ;
								nEffectTime		= (4 + (lpObj->Energy / 100) - (lpTargetObj->Level / 20) );
								
							}
							else if(lpTargetObj->Type == OBJTYPE_CHARACTER)
							{
								int tmpEnergy = 0;
								if(lpObj->Energy > 4000)
								{
									tmpEnergy = 4000;
								}
								nEffectValue	= 12 + (lpObj->Energy / 110);
#ifdef MODIFY_SUMMONER_WEAKENESS_ENERVATION_PVPEFFECTTIME_BUGFIX_20080404
								nEffectTime		= (5 + (lpObj->Energy / 300) + ((lpObj->Level -  lpTargetObj->Level) / 150) );
#else	// MODIFY_SUMMONER_WEAKENESS_ENERVATION_PVPEFFECTTIME_BUGFIX_20080404
								nEffectTime		= (5 + (lpObj->Energy / 300) - ((lpObj->Level -  lpTargetObj->Level) / 150) );
#endif	// MODIFY_SUMMONER_WEAKENESS_ENERVATION_PVPEFFECTTIME_BUGFIX_20080404
							}
							
							if(nEffectTime < 1)
							{
								GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
								return FALSE;
							}

							if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_CURSED_DEFENSE_DOWN, nEffectValue, 0 ) == true )
							{
								GCMagicAttackNumberSend( lpObj, AT_SKILL_ENERVATION, lpTargetObj->m_Index, 0 );	// 스킬사용 실패
								return FALSE;
							}
							
							gObjAddBuffEffect( lpTargetObj, BUFFTYPE_CURSED_DEFENSE_DOWN, EFFECTTYPE_CURSED_DEFENSEDOWN, nEffectValue, 0, 0, nEffectTime );
							return TRUE;
						}
						else
						{
							GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
							return FALSE;
						}
					}
					break;
	#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111

	#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
				case AT_SKILL_PULLUTION:
					{
						if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_FREEZE ) == false )
						{
							//if( retResistance( lpTargetObj, RESISTANCE_COLD ) == FALSE )
							{
								lpTargetObj->lpAttackObj = lpObj;
								lpTargetObj->DelayActionTime = 1000;
								lpTargetObj->DelayLevel = 1;
								gObjAddBuffEffect( lpTargetObj, BUFFTYPE_FREEZE, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 2 );
							}
						}
					}
					break;
	#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
					
#ifdef SEASON4_PATCH_GROOVING
					// --->
					// 시즌 4.0 추가 스킬  grooving 2008/04/24
					// 단순히 클라이어트 이펙트 표현을 위해서.
				case	AT_SKILL_LIGHTING_SHOCK: // 라이트닝 쇼크 
					{
						gObjAddBuffEffect( lpTargetObj, BUFFTYPE_LIGHTNINGSHOCK, 0, 0, 0, 0, 1 );
					}
					break;
					// <---
#endif

	#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 신규 스킬 추가
				case  AT_SKILL_DESTRUCTION:		// 파괴의 일격
					{
						lpTargetObj->lpAttackObj = lpObj;
						lpTargetObj->DelayActionTime = 1000;
						lpTargetObj->DelayLevel = 1;
						gObjAddBuffEffect( lpTargetObj, BUFFTYPE_COLD, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 10 );
						AttackDamage = AttackDamage*(200+Energy/10)/100;
					}
					break;
				case AT_SKILL_MULTISHOT:		// 멀티샷
					break;
				case AT_SKILL_FLAMESTRIKE:		// 프레임 스트라이크
					{
						AttackDamage = AttackDamage * 2;
						gObjAddBuffEffect( lpTargetObj, BUFFTYPE_FLAMESTRKE, EFFECTTYPE_NONE, 0, 0, 0, 1 );
					}
					break;
				case AT_SKILL_GIGANTICSTORM:	// 기간틱 스톰
					{
						gObjAddBuffEffect( lpTargetObj, BUFFTYPE_GIGANTICSTORM, EFFECTTYPE_NONE, 0, 0, 0, 1 );
					}
					break;
				case AT_SKILL_CHAOTICDESAIR:	// 카오스 디자이어
					{
						AttackDamage = AttackDamage*(200+Energy/25)/100;
					}
					break;
	#endif	// ADD_SEASON4_NEW_SKILL_20080522
					
#ifdef ADD_RAKLION_20080408
				case AT_SKILL_SELUPAN_POISON_ATTACK:
					AttackDamage = AttackDamage * 2;
					break;
				case AT_SKILL_SELUPAN_ICE_STORM:
					AttackDamage = (float)(AttackDamage * 2.2);
					break;
				case AT_SKILL_SELUPAN_ICE_STRIKE:
					AttackDamage = (float)(AttackDamage * 2.3);
					break;
				case AT_SKILL_SELUPAN_FIRST_SKILL:
					AttackDamage = (float)(AttackDamage * 2.5);
					break;
#endif // ADD_RAKLION_20080408
			}
			
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 마스터레벨 액티브 스킬 데미지 값 구하기
			//---------------------------------------------------------------------------------
			//	
			//	[][][] 마스터레벨 액티브 스킬 데미지 값 구하기
			//
			//---------------------------------------------------------------------------------	
			if( g_MasterSkillSystem.CheckMasterLevelSkill( skill ) )
			{
				INT iSkillValue = MagicDamageC.SkillGet( lpMagic->m_Skill );
				INT iMLSBase	= g_MasterSkillSystem.GetBaseMasterLevelSkill( skill );
				switch( iMLSBase ) 
				{
				case AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT:			// 회오리베기 강화(기사)
					AttackDamage = AttackDamage * ( 200 + Energy / 10 ) / 100;					
					break;

				case AT_SKILL_ML_STRENGTHEN_WHEEL_MAGUMSA:			// 회오리베기 강화(마검사)
					AttackDamage = AttackDamage * 2;					
					break;
					
				case AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK:		// 어스쉐이크 강화
					AttackDamage = AttackDamage * ( 200 + ( lpObj->Energy + lpObj->AddEnergy ) / 20 ) / 100;					
					break;

				case AT_SKILL_ML_STRENGTHEN_FIREBURST:				// 파이어버스트 강화
					AttackDamage = AttackDamage * ( 200 + ( lpObj->Energy + lpObj->AddEnergy ) / 20 ) / 100;					
					break;

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 마스터 레벨 4랭크
				case AT_SKILL_ML_STRENGTHEN_EVIL:					// 악령 강화
					break;
				case AT_SKILL_ML_STRENGTHEN_EXPICE:					// 서든 아이스 강화
					AttackDamage += iSkillValue;
					break;
				case AT_SKILL_ML_STRENGTHEN_STRIKE:					// 블로우 강화(기사)
					AttackDamage = AttackDamage*(200+Energy/10)/100;
					break;
				case AT_SKILL_ML_STRENGTHEN_BLOWOFFURY:				// 분노의 일격 강화(기사)
					AttackDamage = AttackDamage*(200+Energy/10)/100;
					break;
				case AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN:			// 블러드 어택 강화(마검)
					AttackDamage = AttackDamage * 2;
					break;
				case AT_SKILL_ML_STRENGTHEN_SWORD6:					// 파워 슬래쉬 강화(마검)
					AttackDamage = AttackDamage * 2;
					break;
				case AT_SKILL_ML_STRENGTHEN_FIRESCREAM:				// 파이어 스크림 강화(다크)
					AttackDamage = AttackDamage*(200+(lpObj->Energy+lpObj->AddEnergy)/20)/100;
					break;
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428

#ifdef UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808	// 
				case AT_SKILL_ML_STRENGTHEN_MAGUMSA_EVIL:			// 악령 강화(마검사)
					break;
#endif	// UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
				}
			}
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107

#else
			
	/*
	//
	//	
	//	20051110 ADD_SKILL_WITH_COMBO 의 ELSE 부분 삭제 및 Attack 함수 간략화 (apple)
	// 
	//  [] Depth 1
	//	[][] Depth 2
	//	[][][] Depth 3
	//
	*/

#endif // ADD_SKILL_WITH_COMBO

			
			//---------------------------------------------------------------------------------
			//	
			//	[][][] 스킬이 아닌 공격 일때 데미지를 구한다.
			//
			//---------------------------------------------------------------------------------

			if( !skill )
			{
				if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) )
				{	// 디노란트 일반 공격도 데미지 추가
					AttackDamage = AttackDamage*130/100;
				}
				
			}

			//---------------------------------------------------------------------------------
			//	
			//	[][][] 법사 소울 바리어 데미지 흡수 적용
			//
			//---------------------------------------------------------------------------------

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 소울바리어 : 데미지를 마나로 흡수(Attack)
			// 법사 소울바리어(데미지를 마나로 흡수 시킨다)
			if( ( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC ) == true ) && AttackDamage > 0 )
			{	
				// 데미지를 마나로
				int iWizardSkillDefense = 0;
				int iManaRate			= 0;
				gObjGetValueOfBuffIndex( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, &iWizardSkillDefense, &iManaRate );
				// 마나 감소			
				int iReplacementMana = 0;
				if( iManaRate > 0 )
					iReplacementMana = (WORD)lpTargetObj->Mana * iManaRate / 1000;	
				else
					iReplacementMana = (WORD)lpTargetObj->Mana * 2 / 100;	

				if( lpTargetObj->Mana > iReplacementMana )
				{	
					// 사용가능한 마나가 있어야 된다
					lpTargetObj->Mana -= iReplacementMana;				

					int decattackdamage = 0;
					if( iWizardSkillDefense > 0 )
						decattackdamage = AttackDamage * iWizardSkillDefense / 100;	

					// 데미지 감소
					AttackDamage -= decattackdamage;
					ManaChange = TRUE;
				}
			}

#else
			if( ( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC ) == true ) && AttackDamage > 0 )
			{	// 법사 소울바리어(데미지를 마나로 흡수 시킨다)
				int replacemana = (WORD)lpTargetObj->Mana*2/100;		// 마나 2%감소
				if( lpTargetObj->Mana > replacemana )
				{	// 사용가능한 마나가 있어야 된다
					lpTargetObj->Mana -= replacemana;				// 데미지를 마나로
					int iWizardSkillDefense = 0;
					gObjGetValueOfBuffIndex( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, &iWizardSkillDefense );
					int decattackdamage = AttackDamage * iWizardSkillDefense / 100;
					AttackDamage -= decattackdamage;
					ManaChange = TRUE;
				}
			}
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
#else
#ifdef NEW_FORSKYLAND2
			if( lpTargetObj->m_WizardSkillDefense && AttackDamage > 0 )
			{	// 법사 소울바리어(데미지를 마나로 흡수 시킨다)
				int replacemana = (WORD)lpTargetObj->Mana*2/100;		// 마나 2%감소
				if( lpTargetObj->Mana > replacemana )
				{	// 사용가능한 마나가 있어야 된다
					lpTargetObj->Mana -= replacemana;				// 데미지를 마나로
						int decattackdamage = AttackDamage*lpTargetObj->m_WizardSkillDefense/100;	// 데미지 감소

					AttackDamage -= decattackdamage;
					ManaChange = TRUE;
				}
			}
#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			
			//---------------------------------------------------------------------------------
			//	
			//	[][][] 세트 옵션 데미지 증가 적용
			//
			//---------------------------------------------------------------------------------
			
			AttackDamage += lpObj->SetOpAddDamage;			// 세트옵션 데미지증가

#ifdef ADD_380ITEM_NEWOPTION_20060711
			// 380 아이템 추가 옵션 : 대인 데미지 증가 +%d
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				AttackDamage +=lpObj->m_ItemOptionExFor380.OpAddDamage;
			}
#endif // ADD_380ITEM_NEWOPTION_20060711
			
			//NEW BALANCE DEVGAMESX
			gBalanceSystem.Main(lpObj, lpTargetObj, AttackDamage);

			//---------------------------------------------------------------------------------
			//	
			//	[][][] 카오스 캐슬 데미지 적용
			//
			//---------------------------------------------------------------------------------

#ifdef CHAOSCASTLE_SYSTEM_20040408		// 카오스 캐슬 안에서 사용자 끼리 공격 시 데미지 절반
#ifndef WORLD_TOURNAMENT_EVENT_SETTING			
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				if (CHECK_CHAOSCASTLE(lpObj->MapNumber) && CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) {
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
#endif
#endif
			
			//---------------------------------------------------------------------------------
			//	
			//	[][][] 공성전 데미지 적용
			//
			//---------------------------------------------------------------------------------

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// 공성전 중에 공성맵 안에서는 사용자 끼리 공격 시 데미지 같은편 80% 감소, 다른편 60% 감소
			if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
				if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
				{
					if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE) {
						if (lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide) {
							AttackDamage = AttackDamage * 20 / 100;
						}
						else {
							AttackDamage = AttackDamage * 40 / 100;
						}
					}
				}
			}
#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116 // 축복의 물약 사용 효과!
			if( lpObj->Type == OBJTYPE_CHARACTER 
				&& lpTargetObj->Type == OBJTYPE_MONSTER )
			{
				// 수호 석상 때릴 때
				if( lpTargetObj->Class == 283 )
				{
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_BLESS_POTION ) == true )
					{
						AttackDamage += ( AttackDamage * ( 20 / 100 ) );
					}
					else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SOUL_POTION ) == true )
					{
						AttackDamage = AttackDamage;
					}
	#else
					if( lpObj->m_iPotionBlessTime > 0 )
					{
						// 추가 내구력 저하 없음.
						AttackDamage = AttackDamage + AttackDamage * 20 / 100;
					}
					else if( lpObj->m_iPotionSoulTime > 0 )
					{
						AttackDamage = AttackDamage;
					}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					else
					{
						if( lpObj->m_iAccumulatedDamage > 100 )
						{
							gObjWeaponDurDownInCastle( lpObj, lpTargetObj, 1 );	
							lpObj->m_iAccumulatedDamage = 0;
						}
						else
						{
							lpObj->m_iAccumulatedDamage += AttackDamage;
						}

						// 데미지 20%만 적용 -> 5% 변경(20050303)
						AttackDamage = AttackDamage * 5 / 100;

					}

				}

				// 성문 때릴 때
				if( lpTargetObj->Class == 277 )
				{
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_BLESS_POTION ) == true )
					{
						AttackDamage += ( AttackDamage * ( 20 / 100 ) );
					}
					else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SOUL_POTION ) == true )
					{
						AttackDamage = AttackDamage;
					}
	#else
					if( lpObj->m_iPotionBlessTime > 0 )
					{
						// 추가 내구력 저하 없음.
						AttackDamage = AttackDamage + AttackDamage * 20 / 100;
					}
					else if( lpObj->m_iPotionSoulTime > 0 )
					{
						AttackDamage = AttackDamage;
					}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					else
					{
						if( lpObj->m_iAccumulatedDamage > 100 )
						{
							gObjWeaponDurDownInCastle( lpObj, lpTargetObj, 1 );	
							lpObj->m_iAccumulatedDamage = 0;
						}
						else
						{
							lpObj->m_iAccumulatedDamage += AttackDamage;
						}

						// 데미지 20%만 적용 -> 5% 변경(20050303)
						AttackDamage = AttackDamage * 5 / 100;

					}
				}

			}
#endif
			

			//---------------------------------------------------------------------------------
			//	
			//	[][][] 펜릴 최종 데미지 증가, 흡수 적용
			//
			//---------------------------------------------------------------------------------

#ifdef ADD_ITEM_FENRIR_01_20051110
			// Main Attack 함수
			
			// 펜릴의 최종 데미지 증가 적용
			if( gObjFenrir(lpObj) )
			{
				int iIncPercent = lpObj->pInventory[EQUIPMENT_HELPER].IsFenrirIncLastAttackDamage();

				if( iIncPercent > 0 )
				{
					AttackDamage += AttackDamage * iIncPercent / 100;
				}
			}
			
			// 펜릴의 최종 데미지 감소 적용
			if( gObjFenrir(lpTargetObj) )
			{
				int iDecPercent = lpTargetObj->pInventory[EQUIPMENT_HELPER].IsFenrirDecLastAttackDamage();

				if( iDecPercent > 0 )
				{
					AttackDamage -= AttackDamage * iDecPercent / 100;
				}
			}
			
			if( AttackDamage < 0 )
				AttackDamage = 0;

			// 펜릴 플라즈마스톰에 의한 내구력 감소 적용 - 공성 시 같은 팀은 적용 되지 않는다.
			if( 
				skill == AT_SKILL_FENRIR_ATTACK 
				&& lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER
				&& AttackDamage > 0  
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	#ifdef MODIFY_CASTLEMAP_FENRIR_DURATION_20060522
				// 공성/수성 측일때만 해당함. 아무 측도 아닌 경우, 내구도 감소를 할 수 있음.
				&& ( lpObj->m_btCsJoinSide == 0 || (lpObj->m_btCsJoinSide != lpTargetObj->m_btCsJoinSide) )
	#else
				&& lpObj->m_btCsJoinSide != lpTargetObj->m_btCsJoinSide
	#endif // MODIFY_CASTLEMAP_FENRIR_DURATION_20060522
#endif
			  ) 
			{
				int iEquipmentPos = rand()%(EQUIPMENT_WING-EQUIPMENT_HELM) + EQUIPMENT_HELM;

				CItem	*lpEquipment = &lpTargetObj->pInventory[iEquipmentPos];
				
				if( lpEquipment != NULL && lpEquipment->IsItem() )
				{
					
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// 패시브 1차 - 내구감소1 : 플라즈마스톰에 공격받은 경우 내구 감소
					int iDurDecValue = 50;
					if( lpTargetObj->m_MPSkillOpt.iMpsDownDur1 > 0 )
					{
						iDurDecValue += ( lpTargetObj->m_MPSkillOpt.btMpsDownDur1Level  * 7 );
					}
					
					if( iDurDecValue < 0 )
						iDurDecValue = 1;

					int	iDurEquipment	= lpEquipment->m_Durability * iDurDecValue / 100;
#else
					int	iDurEquipment	= lpEquipment->m_Durability * 50 / 100;
#endif
							
#if TESTSERVER == 1
					MsgOutput( lpObj->m_Index, "[펜릴스킬] (%s) 의 %s 내구력 (%d->%d)", 
						lpEquipment->GetName(), lpEquipment->m_Durability, iDurEquipment);
					
					MsgOutput( lpTargetObj->m_Index, "[펜릴스킬] %s 내구력 (%d->%d)", 
						lpEquipment->GetName(), lpEquipment->m_Durability, iDurEquipment);
#endif
#ifdef MODIFY_NOT_EFFECT_FENRIRSKILL_IF_CHARM_PROTECTITEM_20090924
					// 아이템 보호 부적 사용시 아이템 내구도 감소 안함
					for( int iBuffIndex = 0 ; iBuffIndex < lpTargetObj->m_BuffEffectCount ; iBuffIndex++ )
					{						
						if( lpTargetObj->m_BuffEffectList[iBuffIndex].BuffIndex == BUFFTYPE_CHARM_PROTECTITEM )
						{
							iDurEquipment = lpEquipment->m_Durability;
							break;
						}
					}
#endif // MODIFY_NOT_EFFECT_FENRIRSKILL_IF_CHARM_PROTECTITEM_20090924				
					
					lpEquipment->m_Durability = iDurEquipment;
					
					if( lpEquipment->m_Durability < 0 )
						lpEquipment->m_Durability = 0;
					
					
					GCItemDurSend(lpTargetObj->m_Index, iEquipmentPos, lpEquipment->m_Durability, 0);
				}
				
			}
#endif
			


#ifdef ADD_SHIELD_POINT_01_20060403
			//---------------------------------------------------------------------------------
			//	
			//	[][][] 공격 받는 Target의 SD와 HP를 감소
			//
			//---------------------------------------------------------------------------------

			// CODEREVIEW_SHIELD : 묶기!!
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				// CODEREVIEW_SHIELD : 연산자 사용 형식 변경
				// CODEREVIEW_SHIELD : 쉴드 데미지 차이 구하는 방법 변경
				iTempShieldDamage		= GetShieldDamage( lpObj, lpTargetObj, AttackDamage );
				lpTargetObj->iShield	= lpTargetObj->iShield - iTempShieldDamage;
				lpTargetObj->Life		= lpTargetObj->Life - (AttackDamage - iTempShieldDamage);
				iTotalShieldDamage		= iTotalShieldDamage + iTempShieldDamage;



				if( lpTargetObj->Life < 0 )
				{
					lpTargetObj->Life = 0;
				}
			}
			else
			{
				lpTargetObj->Life = lpTargetObj->Life - AttackDamage;

				if( lpTargetObj->Life < 0 )
					lpTargetObj->Life = 0;
			}
#else
			//---------------------------------------------------------------------------------
			//	
			//	[][][] 공격 받는 Target 에게 AttackDamage 적용 
			//
			//---------------------------------------------------------------------------------

			lpTargetObj->Life -= AttackDamage;

			if( lpTargetObj->Life < 0 )
				lpTargetObj->Life = 0;
#endif // ADD_SHIELD_POINT_01_20060403		

		} // if( lpTargetObj->Live )


	} // if( AttackDamage == 0 )
	else 
	{
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 라이트닝 오브의 스플레쉬데미지에 대한 MissCheck
		if( skill == AT_SKILL_LIGHTNINGORB )
		{
			if( g_ShieldSystemOn == TRUE )
			{
				if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
				{
					if( !MissCheckPvP( lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss ) )
					{
						return FALSE;
					}
				}
				else
				{
					if( !MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss) )
						return FALSE;
				}
			}
			else
			{
				if( !MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss) )
					return FALSE;
			}			
		}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#ifndef MODIFY_CC_PVP_COMBO_DAMAGE_LOWER_50_20040917		// 기존 카오스캐슬 PVP 데미지 감소
#ifdef CHAOSCASTLE_SYSTEM_20040408		// 카오스 캐슬 안에서 사용자 끼리 공격 시 데미지 절반
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING
#ifdef ADD_SHIELD_POINT_01_20060403
		if( g_ShieldSystemOn == FALSE )
#endif // ADD_SHIELD_POINT_01_20060403
		{
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				if (CHECK_CHAOSCASTLE(lpObj->MapNumber) && CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) {
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}
	#endif
#endif
#endif		

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// 공성전 중에 공성맵 안에서는 사용자 끼리 공격 시 데미지 같은편 80% 감소, 다른편 60% 감소
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE) {
					if (lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide) {
						AttackDamage = AttackDamage * 20 / 100;
					}
					else 
					{
		#ifdef ADD_SHIELD_POINT_01_20060403
						if( g_ShieldSystemOn == FALSE )
		#endif // ADD_SHIELD_POINT_01_20060403
						{
							AttackDamage = AttackDamage * 40 / 100;
						}
					}
				}
			}
		}
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// 파이어스크림 폭발은 데미지반사 설정을 하지 않는다.
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 라이트닝오브 스플레쉬 데미지도 데미지반사 제외
		if( skill != AT_SKILL_EXPLOSION 
			&& skill != AT_SKILL_LIGHTNINGORB)
#else
		if( skill != AT_SKILL_EXPLOSION )
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		{
#endif

		//NEW ADD BALANCE SYSTEM
		gBalanceSystem.Main(lpObj, lpTargetObj, AttackDamage);

		bDamageReflect = TRUE;

#ifdef ADD_SKILL_WITH_COMBO
		MsgDamage = DT_MIRROR;		// 반사 공격
#else
		MsgDamage = 0x20;
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	
		}
#endif
		
#ifdef ADD_SHIELD_POINT_01_20060403
		if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
		{
			// CODEREVIEW_SHIELD : 쉴드 데미지 차이 구하는 방법 변경
			iTempShieldDamage		= GetShieldDamage( lpObj, lpTargetObj, AttackDamage );
			lpTargetObj->iShield	= lpTargetObj->iShield - iTempShieldDamage;
			lpTargetObj->Life		= lpTargetObj->Life - (AttackDamage-iTempShieldDamage);
			iTotalShieldDamage		= iTotalShieldDamage + iTempShieldDamage;

			if( lpTargetObj->Life < 0 )
			{
				lpTargetObj->Life = 0;
			}
		}
		else
		{
			lpTargetObj->Life -= AttackDamage;
			if( lpTargetObj->Life < 0 )
				lpTargetObj->Life = 0;
		}
#else
		lpTargetObj->Life -= AttackDamage;
		if( lpTargetObj->Life < 0 )
			lpTargetObj->Life = 0;
#endif			
	}
	//---------------------------------------------------------------------------------
	//
	//	if( AttackDamage == 0 )
	//	{
	//
	// 	}
	//	else
	//	{
	//
	//	}
	//
	//
	//
	//
	//---------------------------------------------------------------------------------


	


	//---------------------------------------------------------------------------------
	//
	//  [] Target 이 몬스터인 경우 공격 메세지 Q에 공격 당한 메세지 추가
	//
	//
	//---------------------------------------------------------------------------------
	
	// 당하는게 몬스터일때..
	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		// 공격당했다..
		gObjAddMsgSendDelay(lpTargetObj, 0, lpObj->m_Index, 100);
		
#ifdef FOR_BLOODCASTLE
		lpTargetObj->LastAttackerID = lpObj->m_Index;	// 마지막 공격한 사람의 인덱스를 저장
#endif
		
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
		// 여기서 Agro를 증가 시켜준다.
		if( lpTargetObj->m_iCurrentAI )
		{
			lpTargetObj->m_Agro.IncAgro(lpObj->m_Index, AttackDamage/50);
		}
#endif
		
	}
	
	//---------------------------------------------------------------------------------
	//
	//  [] 정당 방위 설정, 내구도, 아이스애로우(석화) 효과 처리
	//
	//
	//---------------------------------------------------------------------------------
	
	BOOL selfdefense = FALSE;

	lpCallObj = lpTargetObj;

	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		if( lpTargetObj->m_RecallMon >= 0 ) 
		{
			lpCallObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if( AttackDamage >= 1 )
	{	
		// 플레이어들끼리..
		// 공격을 하여 성공하면 정당방위를 설정한다.
		if( lpObj->Type	== OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
		{			
#ifdef DUEL_SYSTEM_20031028
			if (gObjDuelCheck(lpObj, lpTargetObj)) {	// 서로 결투 중이라면 정당방위 성립않됨
				selfdefense = FALSE;
			}

#ifdef ADD_NEWPVP_PKFIELD
			else if (g_NewPVP.IsVulcanusMap(lpObj->MapNumber) && g_NewPVP.IsVulcanusMap(lpTargetObj->MapNumber)) {
				selfdefense = FALSE;
			}
#endif // ADD_NEWPVP_PKFIELD

#ifdef CHAOSCASTLE_SYSTEM_20040408						// 카오스 캐슬 안에서는 정당방위가 성립되지 않는다.
			else if (CHECK_CHAOSCASTLE(lpObj->MapNumber) || CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) {
				selfdefense = FALSE;
			}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영 사원 안에서는 정당방위가 성립되지 않는다.
			else if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) || CHECK_ILLUSION_TEMPLE(lpTargetObj->MapNumber) )
			{
				selfdefense = FALSE;
			}
#endif

			else {
				selfdefense = TRUE;
			}
#else
			selfdefense = TRUE;
#endif
			
#ifdef ADD_NEW_UNION_01_20041006
			// 적대 길드 이면 정당 방위 설정이 되지 않는다.
			// 아무때나 난입 가능. (결투 상태 일때도..)
			if( gObjGetRelationShip( lpObj, lpTargetObj ) == G_RELATIONSHIP_RIVAL )
			{
				selfdefense = FALSE;
			}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111					// 공성전 시 공성전 참여길드는 정당방위 성립않됨
			if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) 
			{
				if (lpObj->m_btCsJoinSide > CS_SIEGESIDE_NONE) 
				{
					selfdefense	= FALSE;
				}
			}
#endif
		}
		else if( lpTargetObj->Type == OBJTYPE_MONSTER && lpObj->Type == OBJTYPE_CHARACTER)
		{
			if( lpTargetObj->m_RecallMon >= 0 ) 
				selfdefense = TRUE;
		}
		if( lpTargetObj->Type == OBJTYPE_CHARACTER )
		{	// 공격이 성공하면 내구도를 감소 시킨다			
			gObjArmorRandomDurDown(lpTargetObj, lpObj);
		}

#ifdef NEW_FORSKYLAND2						
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_STONE ) == true )
		{
		#ifdef MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
			gObjChangeBuffValidTime( lpTargetObj, BUFFTYPE_STONE, -1 );
			if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_STONE ) == false )
			{
				GCMagicCancelSend(lpTargetObj, AT_SKILL_ELFHARDEN);
			}
		#else
			gObjChangeBuffValidTime( lpTargetObj, BUFFTYPE_STONE, -1 );
			GCMagicCancelSend(lpTargetObj, AT_SKILL_ELFHARDEN);
		#endif // MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
		}

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 슬립에 걸려있으면 풀어준다
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_SLEEP ) == true )
		{
			gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_SLEEP);
		}
#endif

	#else
		if( lpTargetObj->m_SkillHarden != 0 )
		{	// 공격을 당했을때 석화에 걸려 있으면, 공격을 풀어 준다

			lpTargetObj->m_SkillHarden--;

			if( lpTargetObj->m_SkillHarden <= 0 )
			{
				lpTargetObj->m_SkillHardenTime = 0;
				lpTargetObj->m_SkillHarden = 0;			 
				lpTargetObj->m_ViewSkillState &= ~STATE_HARDEN;
				GCMagicCancelSend(lpTargetObj, AT_SKILL_ELFHARDEN);
			}			
		}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif
	}
	

#ifdef CHINA_GM_EVENT
	if( lpTargetObj->Authority&AUTHORITY_EVENT_GM )
	{
		selfdefense = FALSE;
	}
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
	if( lpTargetObj->Authority&AUTHORITY_EVENT_GM )
	{
		selfdefense = FALSE;
	}
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#ifdef MODIFY_0708_BUFFIX_20070820	// 같은 파티원끼리는 정당방위 설정 안함
#ifdef MODIFY_SELFDEFENSE_20070904
	if( lpObj->PartyNumber >= 0 
		&& lpCallObj->PartyNumber >= 0
		&& lpObj->PartyNumber == lpCallObj->PartyNumber )
#else
	if( lpObj->PartyNumber == lpCallObj->PartyNumber)
#endif	// MODIFY_SELFDEFENSE_20070904
	{
		selfdefense = FALSE;
	}
#endif
	
	if( selfdefense == TRUE && !bDamageReflect )
	{
#ifdef _NEW_EXDB_
		if( gObjTargetGuildWarCheck(lpObj, lpCallObj) == FALSE )
#endif
		{
#ifdef UPDATE_PK_SYSTEM_20070302	// 파티시 파티원 전체에 정당방위 설정
#ifdef MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022	// 정방에 걸린 유저만 SelfDefense에 넣는다
			if( lpCallObj->PartyNumber >= 0 )
			{
				int number = 0;
				int partynum = lpCallObj->PartyNumber;
				
				if( gParty.GetPKPartyPenalty(partynum) < PK_LEVEL_DEFAULT+2)		// PK 패널티 적용시 제외
				{
					gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
				}
			}
			else
			{
				gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
			}
#else	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022
			if( lpCallObj->PartyNumber >= 0 )
			{
				int number = 0;
				int partynum = lpCallObj->PartyNumber;
				
				if( gParty.GetPKPartyPenalty(partynum) < PK_LEVEL_DEFAULT+2)		// PK 패널티 적용시 제외
				{
					for(int n = 0; n < MAX_PARTYUSER; n++)
					{
						number = gParty.m_PartyS[partynum].Number[n];
						if( number >= 0 )
						{
							gObjCheckSelfDefense(lpObj, number);
						}
					}
				}
			}
			else
			{
				gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
			}
#endif	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022
#else	// UPDATE_PK_SYSTEM_20070302
			gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
#endif	// UPDATE_PK_SYSTEM_20070302
		}
	}
	

	//---------------------------------------------------------------------------------
	//
	//  [] 특정 몬스터가 공격 받았을 때 행동 처리
	//
	//
	//---------------------------------------------------------------------------------

#ifdef ADD_NEW_MAP_KALIMA_20040518
	// 쿤둔의 환영의 경우는 1/5 확률로 머뭇거린다.
	if (
//		lpTargetObj->Class == 161 ||		// 쿤둔의 환영1
//		lpTargetObj->Class == 181 || 		// 쿤둔의 환영2
//		lpTargetObj->Class == 189 || 		// 쿤둔의 환영3
//		lpTargetObj->Class == 197 || 		// 쿤둔의 환영4
//		lpTargetObj->Class == 267 || 		// 쿤둔의 환영5
		lpTargetObj->Class == 275 			// 쿤둔의 환영6
		)
	{
		if (lpTargetObj->m_iMonsterBattleDelay <= 0) 
		{
			if (rand()%15 < 1) 
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100);
				lpTargetObj->m_iMonsterBattleDelay = 10;

				// 머뭇거린다는 행동을 보낸다.
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
				GCActionSend(lpTargetObj, AT_DAMAGE1, lpTargetObj->m_Index, lpObj->m_Index );
#else
				GCActionSend(lpTargetObj, AT_DAMAGE1, lpTargetObj->m_Index);
#endif // ADD_TRAP_OBJECT_EXTEND_20060731
			}
		}
	}
#endif


#ifdef FOR_BLOODCASTLE
	if ( (lpTargetObj->Class == 131) || (CHECK_LIMIT(lpTargetObj->Class-BC_SAINT_STATUE_1, 3))) 
	{	
		//성문이나 성자의석상이라면 언제나 머뭇거린다.
		gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100);
		gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
	}
	else if( AttackDamage >= 5 )
#else
	if( AttackDamage >= 5 )
#endif
	{		
		if( lpTargetObj->Type == OBJTYPE_MONSTER ) // 대상자가 몬스터
		{
			if( (rand()%26) == 0 )	
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100);
			}
		}
		else 
		{
			if( (rand()%4) == 0 )	
			{
				if( gObjUniriaSprite(lpTargetObj) == FALSE )
				{
					MSBFlag = 1;
				}
			}
		}
	}

	//---------------------------------------------------------------------------------
	//
	//  [] Target이 법사이고, 소울바리어 사용함에 따라 마나통 교체 처리
	//
	//
	//---------------------------------------------------------------------------------

#ifdef NEW_FORSKYLAND2
	if( ManaChange )
	{
		GCManaSend(lpTargetObj->m_Index, (short)lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);				
	}
#endif
	
	//---------------------------------------------------------------------------------
	//
	//  [] 마법이 사용 됐음을 보낸다. (성공/스킬, 스킬번호)
	//
	//
	//---------------------------------------------------------------------------------

	if( magicsend )
	{
		GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
	}

	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( lpObj->m_Change == 9 )
		{
			GCMagicAttackNumberSend(lpObj, 3, lpTargetObj->m_Index, 1);	
		}
	}
	
	if( lpObj->Class == CLASS_ELF )
	{	
		if( lpObj->Level == 1 && (AttackDamage>10))
		{
			LogAdd("error-Level1 : [%s][%s] Str:%d %d %d %d %d %d %d", lpObj->AccountID, lpObj->Name, lpObj->Strength,
				lpObj->m_AttackDamageMinRight, 
				lpObj->m_AttackDamageMaxRight,
				lpObj->m_AttackDamageMinLeft, 
				lpObj->m_AttackDamageMaxLeft,
				lpObj->m_AttackDamageMax,
				lpObj->m_AttackDamageMin				
				);
		}
	}

	lpObj->m_Rest = 0;


	//---------------------------------------------------------------------------------
	//
	//  [] Attack 함수 마무리 ...
	//
	//
	//---------------------------------------------------------------------------------
	
	if( AttackDamage > 0 )
	{
#ifdef FOR_ANTI_HACK
		if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_MONSTER )
		{
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
		}		
#endif

		int atd_reflect;
		atd_reflect = (int)((float)AttackDamage*lpTargetObj->DamageReflect/100);

		if( atd_reflect )
		{
			//LogAdd("데미지 반사 %x ", atd_reflect);
			
			gObjAddMsgSendDelay(lpTargetObj, MSG_REFLECT, lpObj->m_Index, 10, atd_reflect);
		}

		if( rand()%100 < lpObj->SetOpReflectionDamage )
		{	// 세트 옵션 100% 데미지 반사 
			gObjAddMsgSendDelay(lpTargetObj, MSG_REFLECT, lpObj->m_Index, 10, AttackDamage);
		}

#ifdef ADD_THIRD_WING_20070525	// 3차날개 랜덤옵션 처리 - 적 공격력 되돌려줌
		if( rand() % 100 < 5 )
		{
			if( gObjWingSprite( lpTargetObj ) == TRUE )
			{
				CItem	*Wing = &lpTargetObj->pInventory[EQUIPMENT_WING];
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
				if( ( Wing->m_Type >= MAKE_ITEMNUM( 12, 36 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 40 ) ) 
					|| Wing->m_Type == MAKE_ITEMNUM( 12, 43 ))
#else
				if( Wing->m_Type >= MAKE_ITEMNUM( 12, 36 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 40 ) )
#endif
				{					
					BYTE bt3rdWingOpt = lpTargetObj->pInventory[EQUIPMENT_WING].m_NewOption;						
					if( ( bt3rdWingOpt & PLUS_THIRD_WING_5PER_ATTACK_TURNING ) == PLUS_THIRD_WING_5PER_ATTACK_TURNING ) 
					{	
						// 5% 확률로 적 공격력 돌려줌
						if( lpObj->Type == OBJTYPE_MONSTER )
						{							
							gObjAddMsgSendDelay( lpTargetObj, MSG_ATTACK_TURNING, lpObj->m_Index, 10, lpObj->m_AttackDamageMax );
						}
						else if( lpObj->Type == OBJTYPE_CHARACTER )
						{
							gObjAddMsgSendDelay( lpTargetObj, MSG_ATTACK_TURNING, lpObj->m_Index, 10, AttackDamage );
						}						
					}
				}				
			}
		}
#endif	// ADD_THIRD_WING_20070525

#ifdef ADD_SKILL_WITH_COMBO
		if( bCombo )
		{
			//AttackDamage += AttackDamage+(lpObj->Strength+lpObj->Dexterity)/20;
			int		iComboDamage = (Strength+Dexterity+Energy)/g_GlobalConfig.iComboDamage;

			AttackDamage += iComboDamage;
#ifdef ADD_SHIELD_POINT_01_20060403
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				// CODEREVIEW_SHIELD : 쉴드 데미지 차이 구하는 방법 변경 (콤보 데미지로 쉴드 데미지 계산)
				iTempShieldDamage		= GetShieldDamage( lpObj, lpTargetObj, iComboDamage );
				lpTargetObj->iShield	= lpTargetObj->iShield - iTempShieldDamage;
				lpTargetObj->Life		= lpTargetObj->Life - (iComboDamage - iTempShieldDamage);
				iTotalShieldDamage		= iTotalShieldDamage + iTempShieldDamage;

				if( lpTargetObj->Life < 0 )
				{
					lpTargetObj->Life = 0;
				}
			}
			else
			{
				lpTargetObj->Life -= iComboDamage;
				if( lpTargetObj->Life < 0 ) 
				{
					lpTargetObj->Life = 0;
				}
			}
#else
			lpTargetObj->Life -= iComboDamage;
			if( lpTargetObj->Life < 0 ) 
			{
				lpTargetObj->Life = 0;
			}
#endif
			
			MsgDamage |= DT_COMBO;
			skill = AT_SKILL_COMBO;
		}


		if( rand()%100 < lpObj->SetOpDoubleDamage )
		{	// 더블 데미지

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// 더블 데미지인 경우 주위에 몬스터가 데미지를 받는다.
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 파이어 스크림 강화 추가
			if( skill == AT_SKILL_FIRESCREAM 	// 파이어 스크림 스킬인 경우
				||  g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_FIRESCREAM	// 파이어 스크림 강화
				)
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
			if( skill == AT_SKILL_FIRESCREAM )	// 파이어 스크림 스킬인 경우
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
			{
				gObjUseSkill.FireScreamExplosionAttack( lpObj, lpTargetObj, AttackDamage );		
			}
#endif	// #ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601

#ifdef ADD_SHIELD_POINT_01_20060403
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				// CODEREVIEW_SHIELD : 쉴드 데미지 차이 구하는 방법 변경
				iTempShieldDamage		= GetShieldDamage( lpObj, lpTargetObj, AttackDamage );
				lpTargetObj->iShield	= lpTargetObj->iShield - iTempShieldDamage;
				lpTargetObj->Life		= lpTargetObj->Life - (AttackDamage - iTempShieldDamage);
				iTotalShieldDamage		= iTotalShieldDamage + iTempShieldDamage;
				// SHIELD_DEBUGING_HJLIM_20060420 : 더블 데미지 시에 AttackDamage 2배 안함.
				AttackDamage			= AttackDamage + AttackDamage;

				if( lpTargetObj->Life < 0 )
				{
					lpTargetObj->Life = 0;
				}
			}
			else
			{
				lpTargetObj->Life -= AttackDamage;
				AttackDamage += AttackDamage;
				if( lpTargetObj->Life < 0 )
				{
					lpTargetObj->Life = 0;
				}
			}
#else
			lpTargetObj->Life -= AttackDamage;
			AttackDamage += AttackDamage;
			if( lpTargetObj->Life < 0 )
			{
				lpTargetObj->Life = 0;
			}
#endif
			MsgDamage |= DT_DOUBLEDAMAGE;
		}

#ifdef ADD_SHIELD_POINT_01_20060403
		if ( g_ShieldSystemOn == TRUE )
		{
			// 클라이언트에게 전송할 공격 데미지 (HP 감소량) 계산
			AttackDamage	= AttackDamage - iTotalShieldDamage;
		}
#endif
		
		
#ifdef MODIFY_CC_PVP_COMBO_DAMAGE_LOWER_50_20040917		// 기존 카오스캐슬 PVP 데미지 감소
#ifdef CHAOSCASTLE_SYSTEM_20040408		// 카오스 캐슬 안에서 사용자 끼리 공격 시 데미지 절반
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING
	#ifdef ADD_SHIELD_POINT_01_20060403
		if( g_ShieldSystemOn == FALSE )
	#endif
		{
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				if (CHECK_CHAOSCASTLE(lpObj->MapNumber) && CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) 
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}
	#endif
#endif
#endif		

#ifdef ADD_SHIELD_POINT_01_20060403
		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, skill, iTotalShieldDamage);
	#ifdef EXTEND_LOG_SYSTEM_02_20060516
		if( iTotalShieldDamage > 0 )
		{
			LogAddTD("[PvP System] Victim:[%s][%s], Attacker:[%s][%s] - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				lpTargetObj->AccountID, lpTargetObj->Name, lpObj->AccountID, lpObj->Name,
				lpTargetObj->iShield+iTotalShieldDamage,
				lpTargetObj->Life+AttackDamage,
				lpTargetObj->iShield,
				lpTargetObj->Life);
		}
	#endif // EXTEND_LOG_SYSTEM_02_20060516
#else
		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, skill);
#endif
		
#else
#ifdef MODIFY_CC_PVP_COMBO_DAMAGE_LOWER_50_20040917		// 기존 카오스캐슬 PVP 데미지 감소
#ifdef CHAOSCASTLE_SYSTEM_20040408		// 카오스 캐슬 안에서 사용자 끼리 공격 시 데미지 절반
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING
	#ifdef ADD_SHIELD_POINT_01_20060403
		if( g_ShieldSystemOn == FALSE )
	#endif // ADD_SHIELD_POINT_01_20060403
		{
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				if (CHECK_CHAOSCASTLE(lpObj->MapNumber) && CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) 
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}
	#endif
#endif
#endif		
		
#ifdef ADD_SHIELD_POINT_01_20060403
		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, iTotalShieldDamage);
#else
		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage);
#endif
		
#endif
	}	
	else 
	{
#ifdef ADD_SHIELD_POINT_01_20060403
		GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage, 0);
#else
		GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage);
#endif
	}

#ifdef NEW_FORSKYLAND3
	if( lpObj->Life <= 0 && lpObj->Type == OBJTYPE_CHARACTER )
	{	// 데미지 증가 옵션에 따라서 캐릭터 체력이 깍이면 3초후에 다시 생명 체크를 해서 죽인다
		if( lpObj->m_CheckLifeTime <= 0 )
		{
#ifdef USER_DIE_MAKE_OPPOSIT_PK_20040329
			lpObj->lpAttackObj = lpTargetObj;

			if (lpTargetObj->Type == OBJTYPE_CHARACTER) 
			{
				lpObj->m_bAttackerKilled = true;		// 내가 상대방을 공격하다 죽었으므로 상대방이 직접 나를 죽인자는 아니다.
			}
			else 
			{
				lpObj->m_bAttackerKilled = false;
			}
#else
			lpObj->lpAttackObj = lpTargetObj;
#endif			
			lpObj->m_CheckLifeTime = 3;
		}
		
	}
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// 특정 스킬에 대한 추가 처리
	if( lpMagic != NULL )
	{
		gObjUseSkill.SpecificSkillAdditionTreat( lpObj, lpTargetObj, lpMagic, AttackDamage );
	}	
#endif
	
	
	return TRUE;
}


#ifdef DUEL_SYSTEM_20031028
#ifdef ADD_SKILL_WITH_COMBO
	int  CObjAttack::GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BYTE & effect, BOOL bIsOnDuel, CMagicInf *lpMagic)
#else
	int  CObjAttack::GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BYTE & effect, BOOL bIsOnDuel)
#endif
#else
#if defined( ADD_SKILL_WITH_COMBO ) || GAME_VERSION == G_V_97D
	int  CObjAttack::GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BYTE & effect, CMagicInf *lpMagic)
#else
	int  CObjAttack::GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BYTE & effect)
#endif
#endif
{
// CODEREVIEW_SHIELD : bIsOnDuel 이 TRUE 일 경우 강제적으로 FALSE
#ifdef DUEL_SYSTEM_20031028
	#ifdef ADD_SHIELD_POINT_01_20060403
		if( g_ShieldSystemOn == TRUE )
		{
			if( bIsOnDuel == TRUE )
			{
				bIsOnDuel = FALSE;
			}
		}
	#endif
#endif
		
	int ad;
	int sub;

	int SkillRightMaxDamage = 0;
	int SkillRightMinDamage = 0;

	int SkillLeftMaxDamage = 0;
	int SkillLeftMinDamage = 0;


	CItem	*Right = &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT];
	CItem	*Left  = &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT];
	BOOL	bTwoHandWeapon = FALSE;

	if( lpObj->Type == OBJTYPE_MONSTER || lpObj->Type == OBJTYPE_NPC)
	{
		int AttackMin = 0;
		sub = (lpObj->m_AttackDamageMax)-(lpObj->m_AttackDamageMin);
		AttackMin = (lpObj->m_AttackDamageMin)+(rand()%(sub+1));

		lpObj->m_AttackDamageRight = AttackMin;
		lpObj->m_AttackDamageLeft  = AttackMin;
	}
	else
	{
		int		cDamage=0;	

	#ifdef DARKLORD_WORK					
		if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA )		
	#else
		if(lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA )
	#endif
		{
			if( Right->m_Type >= 0 && Right->m_Type < ITEM_BOW && Left->m_Type >= 0 && Left->m_Type < ITEM_BOW ) 
			{
				if( Right->m_IsValidItem && Left->m_IsValidItem )				
					bTwoHandWeapon = TRUE;
			}
		}
		
#if defined( ADD_SKILL_WITH_COMBO ) || GAME_VERSION == G_V_97D
		if( lpMagic )
		{
				// 스킬 데미지
#ifdef DARKLORD_WORK
			if( lpMagic->m_Skill == AT_SKILL_SPEAR && lpObj->SkillLongSpearChange )
			{	// 롱스피어 어택 데미지를 적용해 준다
				SkillRightMaxDamage = DefMagicInf[AT_SKILL_LONGSPEAR].m_DamageMax;
				SkillRightMinDamage = DefMagicInf[AT_SKILL_LONGSPEAR].m_DamageMin;
			}
			else
#endif
			{
				SkillRightMaxDamage = lpMagic->m_DamageMax;
				SkillRightMinDamage = lpMagic->m_DamageMin;
			}

				if( !bTwoHandWeapon )
				{
					SkillLeftMaxDamage = SkillRightMaxDamage;
					SkillLeftMinDamage = SkillRightMinDamage;
				}
				
				// 세트옵션 : 스킬 공격력 증가
				SkillRightMaxDamage += lpObj->SetOpAddSkillAttack;
				SkillRightMinDamage += lpObj->SetOpAddSkillAttack;
				
				SkillLeftMaxDamage += lpObj->SetOpAddSkillAttack;
				SkillLeftMinDamage += lpObj->SetOpAddSkillAttack;

				int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
				if( SkillAttr != -1 )
				{
#ifdef UPDATE_GM_FUNCTION_20070228	// GM 캐릭터 변신반지 착용 시 Resistance값은 MAX값
					if( (lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM  
						&& ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 42 )  
						|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 42 ) ) )
					{
						SkillRightMaxDamage += 255;
						SkillRightMinDamage += 255;
						
						SkillLeftMaxDamage += 255;
						SkillLeftMinDamage += 255;
					}
					else
					{
						SkillRightMaxDamage += lpObj->m_AddResistance[SkillAttr];
						SkillRightMinDamage += lpObj->m_AddResistance[SkillAttr];

						SkillLeftMaxDamage += lpObj->m_AddResistance[SkillAttr];
						SkillLeftMinDamage += lpObj->m_AddResistance[SkillAttr];
					}
#else
					SkillRightMaxDamage += lpObj->m_AddResistance[SkillAttr];
					SkillRightMinDamage += lpObj->m_AddResistance[SkillAttr];

					SkillLeftMaxDamage += lpObj->m_AddResistance[SkillAttr];
					SkillLeftMinDamage += lpObj->m_AddResistance[SkillAttr];
#endif	// UPDATE_GM_FUNCTION_20070228
				}

	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// 조화의보석 강화 아이템 옵션 : 스킬 공격력 증가
				SkillRightMaxDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
				SkillRightMinDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
				
				SkillLeftMaxDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
				SkillLeftMinDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
	#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530			

#ifdef DARKLORD_WORK			
			if( lpMagic->m_Skill == AT_SKILL_ELECTRICSPARK )
			{	// 일렉트릭은 스킬 공격력 외에 파티원에 따른 추가 데미지가 있다				
				int iPartyCount = GetPartyMemberCount(lpObj);
				int addskilldamage = (lpObj->Leadership+lpObj->AddLeadership)/10+iPartyCount*50;

				SkillRightMaxDamage += addskilldamage;
				SkillRightMinDamage += addskilldamage;
				
				SkillLeftMaxDamage += addskilldamage;
				SkillLeftMinDamage += addskilldamage;
			}
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 액티브스킬 - 어스퀘이크강화 : 다크호스를 타고 있어야 된다.
			else if( lpMagic->m_Skill == AT_SKILL_DARKHORSE_ATTACK 
				|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK 
				)
#else
			else if( lpMagic->m_Skill == AT_SKILL_DARKHORSE_ATTACK )
#endif
			{	// 어스 퀘이크
				if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,4) )
				{	// 다크호스를 타고 있어야 된다
					int addskilldamage = (lpObj->Strength+lpObj->AddStrength)/10+
										 (lpObj->Leadership+lpObj->AddLeadership)/5+
										 lpObj->pInventory[EQUIPMENT_HELPER].m_PetItem_Level*10;
					
					//int addpetdamage = lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Magic/2;
					//addpetdamage = addskilldamage*addpetdamage/100;
					//addpetdamage -= (addpetdamage*lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_CurrentDurabilityState);						
					//addskilldamage += addpetdamage;

					SkillRightMaxDamage += addskilldamage;
					SkillRightMinDamage += addskilldamage;
					
					SkillLeftMaxDamage += addskilldamage;
					SkillLeftMinDamage += addskilldamage;
				}				
			}
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 카오틱 디세이어 스킬시(다크로드)
			else if( lpObj->Class == CLASS_DARKLORD && lpMagic->m_Skill == AT_SKILL_CHAOTICDESAIR )
			{
				int iadddamage = (lpObj->Strength+lpObj->AddStrength)/30+(lpObj->Energy+lpObj->AddEnergy)/55;

				SkillRightMaxDamage += iadddamage;
				SkillRightMinDamage += iadddamage;
				
				SkillLeftMaxDamage += iadddamage;
				SkillLeftMinDamage += iadddamage;
			}
#endif	// ADD_SEASON4_NEW_SKILL_20080522
			else  if( lpObj->Class == CLASS_DARKLORD )
			{
				//int ipartycount = GetPartyMemberCount(lpObj)*2;
				int iadddamage = (lpObj->Strength+lpObj->AddStrength)/25+(lpObj->Energy+lpObj->AddEnergy)/50;

				SkillRightMaxDamage += iadddamage;
				SkillRightMinDamage += iadddamage;
				
				SkillLeftMaxDamage += iadddamage;
				SkillLeftMinDamage += iadddamage;
			}
			
#endif	// DARKLORD_WORK

#ifdef ADD_SEASON4_NEW_SKILL_20080522
		if( lpObj->Class == CLASS_ELF && lpMagic->m_Skill == AT_SKILL_MULTISHOT )
		{
			SkillLeftMaxDamage = SkillLeftMaxDamage * 80 / 100;
			SkillLeftMinDamage = SkillLeftMinDamage * 80 / 100;
		}
#endif	// ADD_SEASON4_NEW_SKILL_20080522
		}
	#endif

		if( lpObj->m_CriticalDamage > 0 ) 
		{
			if( (rand()%100) < lpObj->m_CriticalDamage )
			{
				cDamage = 1;

#ifdef ADD_SKILL_WITH_COMBO
				effect = DT_CRITICAL;		// 크리티컬 데미지
#else
				effect = 0x80;
#endif
			}
		}
		if( lpObj->m_ExcelentDamage > 0 )
		{
			// 세트 옵션 ; 엑설런트 데미지 확률 증가적용
			if( (rand()%100) < lpObj->m_ExcelentDamage )
			{
				cDamage = 1;
#ifdef ADD_SKILL_WITH_COMBO
				effect = DT_EXCELLENT;		// 엑설런트 데미지
#else
				effect = 0x40;
#endif
			}
		}
#ifdef SEASON4_PATCH_GROOVING
		// 버서커 사용시 물리 공격력 증가값을 미리 설정해 준다.
		int	iAttackBerserkerMin = 0;
		int	iAttackBerserkerMax = 0;

		if( true == gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER) )
		{
			GetBuffTypePhysicalIncrease(lpObj, iAttackBerserkerMin, iAttackBerserkerMax);	
		}
#endif
		__try
		{
			
#ifdef SEASON4_PATCH_GROOVING
			//------>
			// 시즌 4.0 추가 스킬  grooving 2008/04/24
			//grooving 처음 공격 데미지 계산
				sub = (lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage + iAttackBerserkerMax)-(lpObj->m_AttackDamageMinRight+SkillRightMinDamage+iAttackBerserkerMin);
				lpObj->m_AttackDamageRight = (lpObj->m_AttackDamageMinRight+SkillRightMinDamage+iAttackBerserkerMin)+(rand()%(sub+1));
			//<------
#else
				sub = (lpObj->m_AttackDamageMaxRight+SkillRightMaxDamage)-(lpObj->m_AttackDamageMinRight+SkillRightMinDamage);
				lpObj->m_AttackDamageRight = (lpObj->m_AttackDamageMinRight+SkillRightMinDamage)+(rand()%(sub+1));
#endif
		}
		__except( sub = 1, EXCEPTION_EXECUTE_HANDLER )
		{
		}
		__try
		{	
#ifdef SEASON4_PATCH_GROOVING
			//------>
			// 시즌 4.0 추가 스킬  grooving 2008/04/24
			//grooving 처음 공격 데미지 계산
			sub = (lpObj->m_AttackDamageMaxLeft+SkillLeftMaxDamage+iAttackBerserkerMax) - (lpObj->m_AttackDamageMinLeft+SkillLeftMinDamage+iAttackBerserkerMin);
			lpObj->m_AttackDamageLeft  = (lpObj->m_AttackDamageMinLeft+SkillLeftMinDamage+iAttackBerserkerMin)+(rand()%(sub+1));
			//<------
#else
			sub = (lpObj->m_AttackDamageMaxLeft+SkillLeftMaxDamage) - (lpObj->m_AttackDamageMinLeft+SkillLeftMinDamage);
			lpObj->m_AttackDamageLeft  = (lpObj->m_AttackDamageMinLeft+SkillLeftMinDamage)+(rand()%(sub+1));			
#endif
		}
		__except( sub = 1, EXCEPTION_EXECUTE_HANDLER )
		{
		}

		if( cDamage )
		{
#ifdef SEASON4_PATCH_GROOVING

			lpObj->m_AttackDamageRight = lpObj->m_AttackDamageMaxRight+SkillRightMaxDamage + iAttackBerserkerMax;
			lpObj->m_AttackDamageLeft  = lpObj->m_AttackDamageMaxLeft+SkillLeftMaxDamage + iAttackBerserkerMax;
#else
			lpObj->m_AttackDamageRight = lpObj->m_AttackDamageMaxRight+SkillRightMaxDamage;
			lpObj->m_AttackDamageLeft  = lpObj->m_AttackDamageMaxLeft+SkillLeftMaxDamage;

#endif //SEASON4_PATCH_GROOVING

			// 세트 옵션 ; 크리티컬 데미지 증가
			lpObj->m_AttackDamageRight += lpObj->SetOpAddCriticalDamage;
			lpObj->m_AttackDamageLeft += lpObj->SetOpAddCriticalDamage;

			#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// 조화의 보석 강화 아이템 옵션 : 크리티컬 데미지 증가
				lpObj->m_AttackDamageRight += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
				lpObj->m_AttackDamageLeft  += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
			#endif

#ifdef DARKLORD_WORK
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			int iValue = 0;
			gObjGetValueOfBuffIndex( lpObj, BUFFTYPE_CRITICAL_DMG_INC, &iValue );
			lpObj->m_AttackDamageRight += iValue;
			lpObj->m_AttackDamageLeft += iValue;
	#else
			lpObj->m_AttackDamageRight += lpObj->SkillAddCriticalDamage;
			lpObj->m_AttackDamageLeft += lpObj->SkillAddCriticalDamage;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif
			

			if( effect == DT_EXCELLENT )
			{	// 엑설런트 데미지일때는 20%추가
				lpObj->m_AttackDamageRight += (lpObj->m_AttackDamageMaxRight+SkillRightMaxDamage)*20/100;
				lpObj->m_AttackDamageLeft  += (lpObj->m_AttackDamageMaxLeft+SkillLeftMaxDamage)*20/100;

				// 세트 옵션 ; 엑설런트 데미지 증가
				lpObj->m_AttackDamageRight += lpObj->SetOpAddExDamage;
				lpObj->m_AttackDamageLeft += lpObj->SetOpAddExDamage;
			}
		}

#if TESTSERVER == 1
		LogAddC(LOGC_RED, "LEFT : %d, CDamage = %d, Effect = %d", lpObj->m_AttackDamageLeft, cDamage, effect);
#endif
	}	

	//두손에 무기를 들었을때	
	if( bTwoHandWeapon )
	{
#ifdef DUEL_SYSTEM_20031028
// 			if (bIsOnDuel)
// 				ad = (lpObj->m_AttackDamageRight+lpObj->m_AttackDamageLeft) * 60 /100 -targetDefense;
// 			else
#endif
				ad = (lpObj->m_AttackDamageRight+lpObj->m_AttackDamageLeft)-targetDefense;			
			
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
			// 양손에 무기를 들었을 경우 크리티컬 데미지 증가 옵션이 2배로 적용되는 문제를 수정
			ad -= lpObj->SetOpAddCriticalDamage;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	}
	//활일때
	else if( (Left->m_Type >= ITEM_BOW && Left->m_Type < ITEM_BOW+7)
#ifdef MODIFY_APPLYING_BOW_01_20050825
			|| Left->m_Type == ITEM_BOW + 20
			|| Left->m_Type == ITEM_BOW + 21
#endif
#ifdef MODIFY_ALBATROS_BOW_BUGFIX_20070105
			|| Left->m_Type == ITEM_BOW + 22
#endif			
#ifdef MODIFY_DARKSTINGER_BUGFIX_20081119
			|| Left->m_Type == ITEM_BOW + 23
#endif // MODIFY_DARKSTINGER_BUGFIX_20081119
#ifdef ADD_AERINBOW_20090309
			|| Left->m_Type == ITEM_BOW + 24	// 에어린 보우
#endif // ADD_AERINBOW_20090309
		)
	{
#ifdef DUEL_SYSTEM_20031028
// 		if (bIsOnDuel)
// 			ad = (lpObj->m_AttackDamageLeft) * 60 /100 - targetDefense;
// 		else
#endif
			ad = (lpObj->m_AttackDamageLeft)-targetDefense;
	}
	//석궁일때
	else if(( Right->m_Type >= ITEM_BOW+8 && Right->m_Type < ITEM_BOW+15) ||
	      ( Right->m_Type >= ITEM_BOW+16 && Right->m_Type < ITEM_BOW+MAX_ITEM_INDEX))
	{
#ifdef DUEL_SYSTEM_20031028
// 		if (bIsOnDuel)
// 			ad = (lpObj->m_AttackDamageRight) * 60 /100 - targetDefense;
// 		else
#endif
			ad = (lpObj->m_AttackDamageRight)-targetDefense;
	}
	//일반무기
	else if( Right->m_Type >= 0 && Right->m_Type < ITEM_BOW )
	{
#ifdef DUEL_SYSTEM_20031028
// 		if (bIsOnDuel)
// 			ad = (lpObj->m_AttackDamageRight) * 60 /100 - targetDefense;
// 		else
#endif
			ad = (lpObj->m_AttackDamageRight)-targetDefense;
	}
	//지팡이
	else if( Right->m_Type >= ITEM_STAFF && Right->m_Type < ITEM_SHIELD )
	{
#ifdef DUEL_SYSTEM_20031028
// 		if (bIsOnDuel)
// 			ad = (lpObj->m_AttackDamageRight) * 60 /100 - targetDefense;
// 		else
#endif
			ad = (lpObj->m_AttackDamageRight)-targetDefense;
		}
	else
	{
#ifdef DUEL_SYSTEM_20031028
// 		if (bIsOnDuel)
// 			ad = (lpObj->m_AttackDamageLeft) * 60 /100 - targetDefense;
// 		else
#endif
			ad = (lpObj->m_AttackDamageLeft)-targetDefense;	
	}
	
	// 세트옵션 두손검 장비시 데미지 증가
	if( lpObj->SetOpTwoHandSwordImproveDamage )
	{
		ad += ad*lpObj->SetOpTwoHandSwordImproveDamage/100;
	}

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	ad += lpObj->m_SkillAttack;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	int iIncreaseDamage = 0;
	iIncreaseDamage = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_IMPROVE_DAMAGE );
	iIncreaseDamage += gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_MELEEDAMAGE );

	ad += iIncreaseDamage;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211

	if( lpObj->m_SkillAttack2 )
	{
		ad += 15;
	}

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
	// 공격력 증가&감소
	if( lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttackTime > 0 )
	{
#if TESTSERVER == 1
		MsgOutput(lpObj->m_Index, "%d 공증&감상태", lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack);
#endif
		
		ad += lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

		if( ad < 0 ) ad = 0;
	}
#endif

#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111	// 위크니스(공격력 하락)
		int iValue = 0;
		gObjGetValueOfBuffIndex( lpObj, BUFFTYPE_CURSED_ATTACK_DOWN, &iValue );
		ad	-= ad * iValue / 100;
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111
		
	return ad;
}

#ifdef DUEL_SYSTEM_20031028	
int CObjAttack::GetAttackDamageWizard(LPOBJECTSTRUCT lpObj, int targetDefense, CMagicInf *lpMagic, BYTE & effect, BOOL bIsOnDuel)	
#else
int CObjAttack::GetAttackDamageWizard(LPOBJECTSTRUCT lpObj, int targetDefense, CMagicInf *lpMagic, BYTE & effect)
#endif
{
#ifdef DUEL_SYSTEM_20031028
	#ifdef ADD_SHIELD_POINT_01_20060403
		if( g_ShieldSystemOn == TRUE )
		{
			if( bIsOnDuel == TRUE )
			{
				bIsOnDuel = FALSE;
			}
		}
	#endif
#endif

	int damagemin;
	int damagemax;

#ifdef ADD_SKILL_WITH_COMBO
	int ad;
	if( lpMagic->m_Skill == AT_SKILL_EXPHELL )
	{	// 헬파이어의 경우 추가 데미지 계산이 있어야 된다
		if( lpObj->SkillHellFire2Count >= 0 )
		{	
			int	SkillHellFire2CountDamageTable[] = {0, 20,50,99,160,225,325,425,550,700,880,1090,1320};

			int CountDamage;
			
			if( lpObj->SkillHellFire2Count > 12 )
				CountDamage = 0;
			else
				CountDamage = SkillHellFire2CountDamageTable[lpObj->SkillHellFire2Count];

			ad = (lpObj->Strength+lpObj->AddStrength)/2+CountDamage;

			damagemin = ad+(lpObj->m_MagicDamageMin);
			damagemax = ad+(lpObj->m_MagicDamageMax);

		// 세트옵션 : 스킬 공격력 증가
		damagemin += lpObj->SetOpAddSkillAttack;
		damagemax += lpObj->SetOpAddSkillAttack;

		int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
		if( SkillAttr != -1 )
		{
#ifdef UPDATE_GM_FUNCTION_20070228	// GM 캐릭터 변신반지 착용 시 Resistance값은 MAX값
			if( (lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM  
				&& ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 42 )  
				|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 42 ) ) )
			{
				damagemin += 255;
				damagemax += 255;
			}
#else
			damagemin += lpObj->m_AddResistance[SkillAttr];
			damagemax += lpObj->m_AddResistance[SkillAttr];
#endif
		}


#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			// 조화의보석 강화 아이템 옵션 : 스킬 공격력 증가
			damagemin += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			damagemax += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		}
	}
	else
	{
		ad = lpMagic->GetDamage();
		damagemin = lpMagic->m_DamageMin+(lpObj->m_MagicDamageMin);
		damagemax = lpMagic->m_DamageMax+(lpObj->m_MagicDamageMax);

		// 세트옵션 : 스킬 공격력 증가
		damagemin += lpObj->SetOpAddSkillAttack;
		damagemax += lpObj->SetOpAddSkillAttack;

		int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
		if( SkillAttr != -1 )
		{
#ifdef UPDATE_GM_FUNCTION_20070228	// GM 캐릭터 변신반지 착용 시 Resistance값은 MAX값
			if( (lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM  
				&& ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 42 )  
				|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 42 ) ) )
			{
				damagemin += 255;
				damagemax += 255;
			}
#else
			damagemin += lpObj->m_AddResistance[SkillAttr];
			damagemax += lpObj->m_AddResistance[SkillAttr];
#endif
		}

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		// 조화의보석 강화 아이템 옵션 : 스킬 공격력 증가
		damagemin += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
		damagemax += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

	}
#else
	int ad = lpMagic->GetDamage();
	damagemin = lpMagic->m_DamageMin+(lpObj->m_MagicDamageMin);
	damagemax = lpMagic->m_DamageMax+(lpObj->m_MagicDamageMax);
#endif	// ADD_SKILL_WITH_COMBO

	CItem	*Right = &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT];


#ifdef ADD_ITEM
	if( Right->IsItem() && 
	    ( 
		  ( Right->m_Type >= MAKE_ITEMNUM(5,0) && Right->m_Type < MAKE_ITEMNUM(5,MAX_ITEM_INDEX) ) // 지팡이류
		  || Right->m_Type == MAKE_ITEMNUM(0,31)  // 데스 브로드
	
	#ifdef ADD_CRITICAL_DAMAGE_OF_DEATH_BLADE_20050419
		  || Right->m_Type == MAKE_ITEMNUM(0,21)  // 데스 블레이드
	#endif // ADD_CRITICAL_DAMAGE_OF_DEATH_BLADE_20050419
		  
	#ifdef ADD_NEW_ITEM_01_20050823
		  || Right->m_Type == MAKE_ITEMNUM(0,23)  // 익스플로전 블레이드
	#endif
		  
	#ifdef MODIFY_350ITEM_BUGFIX2_20060823
		  || Right->m_Type == MAKE_ITEMNUM(0,25)  // 350아이템. 소드댄서
	#endif
		  
	#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
		  || Right->m_Type == MAKE_ITEMNUM( 0, 28 ) // 룬바스타드
	#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
		)
	  )

#else // else ADD_ITEM
	if( Right->IsItem() && (Right->m_Type >= MAKE_ITEMNUM(5,0) && Right->m_Type < MAKE_ITEMNUM(5,MAX_ITEM_INDEX)) )
#endif // ADD_ITEM
	{

		if( Right->m_IsValidItem )
		{
			int damage = (Right->m_Magic/2)+(Right->m_Level*2);
			// 지팡이는 마력증가 계산량에서 내구력 감소에 대한 패널티가 적용된다
			damage -= (WORD)(damage*Right->m_CurrentDurabilityState);		

			damagemin += damagemin * damage / 100;
			damagemax += damagemax * damage / 100;
		}
	}

	int subd = damagemax-damagemin;
	
	__try
	{
#ifdef DUEL_SYSTEM_20031028
// 		if (bIsOnDuel)
// 			ad = (damagemin+(rand()%(subd+1))) * 60 / 100 -targetDefense;
// 		else
#endif
			ad = (damagemin+(rand()%(subd+1)))-targetDefense;

		if( lpObj->m_CriticalDamage > 0 ) 
		{
			if( (rand()%100) < lpObj->m_CriticalDamage )
			{
#ifdef DUEL_SYSTEM_20031028
// 				if (bIsOnDuel)
// 					ad = damagemax * 60 / 100 -targetDefense;
// 				else
#endif
					ad = damagemax-targetDefense;
				//LogAdd("크리티컬 : %d %d : %d",damagemax,targetDefense, ad);

				ad += lpObj->SetOpAddCriticalDamage;

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// 조화의 보석 강화 아이템 옵션 : 크리티컬 데미지 증가
				ad += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
#endif

#ifdef DARKLORD_WORK
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				int iValue = 0;
				gObjGetValueOfBuffIndex( lpObj, BUFFTYPE_CRITICAL_DMG_INC, &iValue );
				ad += iValue;
	#else
				ad += lpObj->SkillAddCriticalDamage;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004			
#endif


				
#ifdef ADD_SKILL_WITH_COMBO
				effect = DT_CRITICAL;			// 크리티컬 데미지
#else
				effect = 0x80;
#endif
			}
		}
		if( lpObj->m_ExcelentDamage > 0 )
		{

			if( (rand()%100) < lpObj->m_ExcelentDamage )
			{
#ifdef DUEL_SYSTEM_20031028
// 				if (bIsOnDuel)					
// 					ad = damagemax * 60 /100 -targetDefense;										
// 				else
#endif
					ad = damagemax-targetDefense;
					
				ad += damagemax*20/100;

				ad += lpObj->SetOpAddExDamage;

#ifdef ADD_SKILL_WITH_COMBO
				effect = DT_EXCELLENT;		// 엑설런트 데미지
#else
				effect = 0x40;
#endif
			}
		}
	}
	__except( subd=1, EXCEPTION_EXECUTE_HANDLER )
	{
	}

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	ad += lpObj->m_SkillAttack;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	int iIncreaseDamage = 0;
	iIncreaseDamage = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_IMPROVE_DAMAGE );
	iIncreaseDamage += gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_MAGICDAMAGE );
	
	ad += iIncreaseDamage;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211

	if( lpObj->m_SkillAttack2 )
	{	// 사랑의 묘약이 적용되면 마력 +10
		ad += 10;
	}

	return ad;
}


// 디노란트를 타고 있으면 데미지 증가 15% 데미지 흡수 15%
BOOL gObjDenorantSprite(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	// 아이템이 없으면 종료
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() == FALSE ) return FALSE;

	CItem	*Denorant = &lpObj->pInventory[EQUIPMENT_HELPER];		
	
	if( Denorant->m_Type == MAKE_ITEMNUM(13,3) )
	{
		if( Denorant->m_Durability > 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

#ifdef DARKLORD_WORK
// 다크호스를 타고 있으면 데미지 흡수 (10+(통솔/100)
BOOL gObjDarkHorse(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	// 아이템이 없으면 종료
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() == FALSE ) return FALSE;

	CItem	*Denorant = &lpObj->pInventory[EQUIPMENT_HELPER];		
	
	if( Denorant->m_Type == MAKE_ITEMNUM(13,4) )
	{
		if( Denorant->m_Durability > 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
// 펜릴을 착용 여부 확인
BOOL gObjFenrir(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	// 아이템이 없으면 종료
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() == FALSE ) return FALSE;

	CItem	*lpFenrir = &lpObj->pInventory[EQUIPMENT_HELPER];		
	
	if( lpFenrir->m_Type == MAKE_ITEMNUM(13,37) )
	{
		if( lpFenrir->m_Durability > 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

#endif

#ifdef ADD_SHIELD_POINT_01_20060403
int CObjAttack::GetShieldDamage(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int iAttackDamage)
{
	int iShieldDamage = 0;

	if( g_ShieldSystemOn == FALSE )
	{
		return 0;
	}

	if( iAttackDamage <= 0 )
	{
		return 0;
	}

	// PVP일 때
	int iReduceLife = 0;
	int iReduceShield = 0;
	int iReduceLifeForEffect = 0;
	bool bReduceShieldGage = false;

// CODEREVIEW_SHIELD :  SD 기준으로 Life 감소 산출
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// 조화의 보석 강화 아이템 옵션 : SD감소율 상승 / SD 비율 상승 / SD 무시 확률 상승

	// 0. SD 비율 값 초기 설정
	int iDamageDevideToSDRate = g_iDamageDevideToSDRate;

	// 1. SD 감소율 상승 옵션 : 상대방의 SD비율을 낮추는 것임.
	iDamageDevideToSDRate -= lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate;

	// 2. SD 비율 상승 옵션 : SD의 비율을 높임. 자신에게 해당.
	iDamageDevideToSDRate += lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate;
	
	// 3. 범위 확인
	if ( iDamageDevideToSDRate < 0 ) 
		iDamageDevideToSDRate = 0;
	if ( iDamageDevideToSDRate > 100 ) 
		iDamageDevideToSDRate = 100;
	
	// 4. SD무시 확률
	if ( lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate > 0 )
	{
		int iRand = rand()%100;
		if ( iRand < lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
		{
			iDamageDevideToSDRate = 0;
		}
	}

	// 5. 로그
	if ( lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate != 0 
		|| lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate != 0
		|| lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
	{
			LogAddTD("[JewelOfHarmony][PvP System] Attacker:[%s][%s]-SD Decrease[%d] SD Ignore[%d] Defender:[%s][%s] SD Increase Option[%d] - SD Rate[%d]",
				lpObj->AccountID, lpObj->Name,
				lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate,
				lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate,
				lpTargetObj->AccountID, lpTargetObj->Name,
				lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate,
				iDamageDevideToSDRate);
	}

	// 6. 최종 계산
	iReduceShield = ( iAttackDamage * iDamageDevideToSDRate ) / 100;
#else
	iReduceShield = ( iAttackDamage * g_iDamageDevideToSDRate ) / 100;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

	iReduceLife = iAttackDamage - iReduceShield;

	if( lpTargetObj->iShield - iReduceShield < 0 )
	{
		iReduceLife += ( iReduceShield - lpTargetObj->iShield );
		iReduceShield = lpTargetObj->iShield;
		if( lpTargetObj->iShield > 0 )
		{
			bReduceShieldGage = true;
		}
	}

#ifdef SHIELD_POINT_DEBUGLOG
	char Msg[256];
	sprintf(Msg, "[Shield] Damage : %d, HP : %d, SD :%d", iAttackDamage, iReduceLife, iReduceShield );
	GCServerMsgStringSend( Msg, lpObj->m_Index, 1 );
	GCServerMsgStringSend( Msg, lpTargetObj->m_Index, 1 );
#endif

	iReduceLifeForEffect = ( lpTargetObj->MaxLife + lpTargetObj->AddLife ) * 20 / 100;
// CODEREVIEW_SHIELD :  #if 사용 고려 코드 변경
	if( bReduceShieldGage == true 
// CODEREVIEW_SHIELD :  lpTargetObj->m_SkillAddLife  실제 HP 값이 아님
//					 :  lpTargetObj->MaxLife + lpTargetObj->AddLife 만 유효
//					 :  조건문에 계산 공식 포함 지양 
		&& ( iReduceLife > iReduceLifeForEffect )
		)
	{
	#ifdef MODIFY_NO_SHOW_SHIELDBROKE_EFFECT_IN_CHAOSCASTLE_20060523
		if( CHECK_CHAOSCASTLE(lpTargetObj->MapNumber) == FALSE )
	#endif
		{
			// 방어막 파괴 이펙트
			GCSendEffectInfo( lpTargetObj->m_Index, EFFECT_SHIELDBROKE );
		}
	}

	return iReduceShield;
}
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 소환술사 마법 공격 데미지 계산
int CObjAttack::GetAttackDamageSummoner(LPOBJECTSTRUCT lpObj, int targetDefense, CMagicInf *lpMagic, BYTE & effect, BOOL bIsOnDuel)
{
	if( g_ShieldSystemOn == TRUE )
	{
		if( bIsOnDuel == TRUE )
		{
			bIsOnDuel = FALSE;
		}
	}

#ifdef SEASON4_PATCH_GROOVING
	// 시즌 4.0 추가 스킬  grooving 2008/04/24
	float fCurseDamageMin = 0;
	float fCurseDamageMax = 0;
	float fMagicDamageMin = 0;
	float fMagicDamageMax = 0;
#endif

	int damagemin;
	int damagemax;

	int ad;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	int iIncreaseDamage = 0;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	
	ad = lpMagic->GetDamage();
	// 환수스킬과 디버프는 저주력의 영향을 받는다.
	switch( lpMagic->m_Skill )
	{
	case AT_SKILL_SLEEP:
	case AT_SKILL_BLIND:
	case AT_SKILL_EXPLOSION_2:
	case AT_SKILL_REQUIEM:
#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
	case AT_SKILL_WEAKNESS:
	case AT_SKILL_ENERVATION:
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111
#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
	case AT_SKILL_PULLUTION:
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
		{
#ifdef SEASON4_PATCH_GROOVING
			//------>
			// 시즌 4.0 추가 스킬  grooving 2008/04/24
			// grooving 버프 저주력
			// 저주력에 영향을 받는 스킬
			if( true == gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER) )
			{
				int iValue = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_BERSERKER_UP );
				fCurseDamageMax = ( (lpObj->Energy + lpObj->AddEnergy) / 4 + 1.5/100 ) * iValue / 100;
				fCurseDamageMin = ( (lpObj->Energy + lpObj->AddEnergy) / 9 + 1/100 ) * iValue / 100;
			}

			damagemin = lpMagic->m_DamageMin + lpObj->m_CurseDamgeMin + fCurseDamageMin;
			damagemax = lpMagic->m_DamageMax + lpObj->m_CurseDamgeMax + fCurseDamageMax;
			//<-------
#else
			damagemin = lpMagic->m_DamageMin + lpObj->m_CurseDamgeMin;
			damagemax = lpMagic->m_DamageMax + lpObj->m_CurseDamgeMax;
#endif
			
			CItem	*Left = &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT];
			
			if( Left->IsItem() && 
				( Left->m_Type >= MAKE_ITEMNUM(5,0) && Left->m_Type < MAKE_ITEMNUM(5,MAX_ITEM_INDEX) ) // 지팡이류
				)
			{
				
				if( Left->m_IsValidItem )
				{
					int damage = (Left->m_Curse/2)+(Left->m_Level*2);
					// 지팡이는 마력증가 계산량에서 내구력 감소에 대한 패널티가 적용된다
					damage -= (WORD)(damage*Left->m_CurrentDurabilityState);		
					
					damagemin += damagemin * damage / 100;
					damagemax += damagemax * damage / 100;
				}
			}
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
			// 저주력에 영향을 받는 스킬
			iIncreaseDamage += gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_CURSED_DAMAGE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		}
		break;
	default:
		{
#ifdef SEASON4_PATCH_GROOVING
			//------>
			// 시즌 4.0 추가 스킬  grooving 2008/04/24
			// grooving 버프 저주력
			// 마력에 영향을 받는 스킬
			if( true == gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BERSERKER) )
			{
				int iValue = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_BERSERKER_UP );

				fMagicDamageMin = ( (lpObj->Energy + lpObj->AddEnergy) / 9 ) * iValue / 100;
				fMagicDamageMax = ( (lpObj->Energy + lpObj->AddEnergy) / 4 ) * iValue / 100;
				
			}

			damagemin = lpMagic->m_DamageMin + lpObj->m_MagicDamageMin + fMagicDamageMin;
			damagemax = lpMagic->m_DamageMax + lpObj->m_MagicDamageMax + fMagicDamageMax;
			//<-------
#else
			damagemin = lpMagic->m_DamageMin + lpObj->m_MagicDamageMin;
			damagemax = lpMagic->m_DamageMax + lpObj->m_MagicDamageMax;
#endif

			CItem	*Right = &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT];
			
			if( Right->IsItem() && 
				( Right->m_Type >= MAKE_ITEMNUM(5,0) && Right->m_Type < MAKE_ITEMNUM(5,MAX_ITEM_INDEX) ) // 지팡이류
				)
			{
				
				if( Right->m_IsValidItem )
				{
					int damage = (Right->m_Magic/2)+(Right->m_Level*2);
					// 지팡이는 마력증가 계산량에서 내구력 감소에 대한 패널티가 적용된다
					damage -= (WORD)(damage*Right->m_CurrentDurabilityState);		
					
					damagemin += damagemin * damage / 100;
					damagemax += damagemax * damage / 100;
				}
			}
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
			// 마력에 영향을 받는 스킬
			iIncreaseDamage += gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_MAGICDAMAGE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		}
		break;
	}
	
	// 세트옵션 : 스킬 공격력 증가
	damagemin += lpObj->SetOpAddSkillAttack;
	damagemax += lpObj->SetOpAddSkillAttack;
	
	int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
	if( SkillAttr != -1 )
	{
		if( (lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM  
			&& ( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 42 )  
			|| lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 42 ) ) )
		{
			damagemin += 255;
			damagemax += 255;
		}
		else
		{
			damagemin += lpObj->m_AddResistance[SkillAttr];
			damagemax += lpObj->m_AddResistance[SkillAttr];
		}
	}
	
	// 조화의보석 강화 아이템 옵션 : 스킬 공격력 증가
	damagemin += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
	damagemax += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;


	
	int subd = damagemax-damagemin;
	
	__try
	{
// 		if (bIsOnDuel)
// 			ad = (damagemin+(rand()%(subd+1))) * 60 / 100 -targetDefense;
// 		else
			ad = (damagemin+(rand()%(subd+1)))-targetDefense;
		
		if( lpObj->m_CriticalDamage > 0 ) 
		{
			if( (rand()%100) < lpObj->m_CriticalDamage )
			{
// 				if (bIsOnDuel)
// 					ad = damagemax * 60 / 100 -targetDefense;
// 				else
					ad = damagemax-targetDefense;
				//LogAdd("크리티컬 : %d %d : %d",damagemax,targetDefense, ad);

				ad += lpObj->SetOpAddCriticalDamage;

				// 조화의 보석 강화 아이템 옵션 : 크리티컬 데미지 증가
				ad += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;

				int iValue = 0;
				gObjGetValueOfBuffIndex( lpObj, BUFFTYPE_CRITICAL_DMG_INC, &iValue );
				ad += iValue;				
				

				effect = DT_CRITICAL;			// 크리티컬 데미지
			}
		}
		if( lpObj->m_ExcelentDamage > 0 )
		{

			if( (rand()%100) < lpObj->m_ExcelentDamage )
			{
// 				if (bIsOnDuel)					
// 					ad = damagemax * 60 /100 -targetDefense;										
// 				else
					ad = damagemax-targetDefense;
					
				ad += damagemax*20/100;

				ad += lpObj->SetOpAddExDamage;

				effect = DT_EXCELLENT;		// 엑설런트 데미지
			}
		}
	}
	__except( subd=1, EXCEPTION_EXECUTE_HANDLER )
	{
	}

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	// 기본 데미지 증가 효과
	iIncreaseDamage += gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_IMPROVE_DAMAGE );
	ad += iIncreaseDamage;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211

	if( lpObj->m_SkillAttack2 )
	{	// 사랑의 묘약이 적용되면 마력 +10
		ad += 10;
	}

	return ad;
}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#ifdef SEASON4_PATCH_GROOVING

bool CObjAttack::GetBuffTypePhysicalIncrease(LPOBJECTSTRUCT lpObj, int &iAttackBerserkerMin, int &iAttackBerserkerMax)
{
	iAttackBerserkerMin	= (lpObj->Strength+lpObj->AddStrength + lpObj->Dexterity+lpObj->AddDexterity) / 7;
	iAttackBerserkerMax	= (lpObj->Strength+lpObj->AddStrength + lpObj->Dexterity+lpObj->AddDexterity) / 4;
	
	int iValue = 0;
	gObjGetValueOfBuffIndex( lpObj, BUFFTYPE_BERSERKER, &iValue );
	
	iAttackBerserkerMin = iAttackBerserkerMin * iValue / 100;
	iAttackBerserkerMax = iAttackBerserkerMax * iValue / 100;
	
	return true;
}
#endif
