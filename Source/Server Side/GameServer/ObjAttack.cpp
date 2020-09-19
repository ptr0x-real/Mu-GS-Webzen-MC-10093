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
// *	Main Attack �Լ�
//
// *	lpObj �� lpTargetObj �� ���� �ߴ�.
//
//
//----------------------------------------------------------------------------
BOOL CObjAttack::Attack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int magicsend, BYTE MSBFlag, int AttackDamage,BOOL bCombo)
{
	

	//---------------------------------------------------------------------------------
	//
	//  [] Attack �Լ� ���� ...
	//
	//
	//---------------------------------------------------------------------------------

	int				skillSuccess=0;

	LPOBJECTSTRUCT	lpCallObj;
	LPOBJECTSTRUCT	lpCallTargetObj;
	BYTE			MsgDamage = 0;

	int				ManaChange = FALSE;

#ifdef ADD_SHIELD_POINT_01_20060403
	// CODEREVIEW_SHIELD : iShieldDamage -> Temp �ǹ� �߰�
	int				iTempShieldDamage = 0;
	int				iTotalShieldDamage = 0;
#endif

	if( (lpTargetObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
	{	// ��� ĳ���� ������ ���� �ʴ´�
		return FALSE;
	}

#ifdef GMCOMMAND_EX_1ST_BUGFIX
	if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
	{	// ��� ĳ���� ������ ���Ѵ�.
		return FALSE;
	}
	
	if( ((lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)
		|| ((lpTargetObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)) 
	{
		// AUTHORITY_EVENT_GM �����̰� ������ ���¿��� ���� �ްų� ���� �ʴ´�.
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
	{	// ���� ���� �ƴϸ�, 
		return FALSE;
	}

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
	// ũ���̿��� �غ�, ���� �ð����� ���Ϳ��� ���ظ� ���� �� ����.
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
		// CODEREVIEW_SHIELD : ������ �̻�? - dw Shield Auto Refill Timer
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
		// ���� ������ ������ų�� ����ȵ�
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
	// (õ��)����Ǻһ��� �� ��� 
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
				// �Ϲ� ���� �� ���. ���� ���� ������ �ȵ���.
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
			// �ӹ��Ÿ��� ������ ���� �ʴ´�.
			return TRUE;
		}
#endif

		// �౸�� �� ��쿣 ƨ��� �Ѵ�.
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
		// �����̳� �������� �������� üũ�Ѵ�.
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
				// ���������� ������ �ʴ´�.
				GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
				return TRUE;
			}
		}
	#else
		if( lpTargetObj->m_ImmuneToMagicCount > 0 )	
		{	
			// �������� ���� �ð� �̴�.
			INT bCheckAttackIsMagicType = gObjCheckAttackTypeMagic (lpObj->Class, skill);

			if (bCheckAttackIsMagicType == CMagicDamage::SKILL_TYPE_MAGIC) 
			{		
				// ���������� ������ �ʴ´�.
				GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, FALSE);
				return TRUE;
			}
		}
		
		if( lpTargetObj->m_ImmuneToHarmCount > 0 ) 
		{	
			// �������� ���� �ð� �̴�.
			INT bCheckAttackIsMagicType = gObjCheckAttackTypeMagic (lpObj->Class, skill);

			if (bCheckAttackIsMagicType == CMagicDamage::SKILL_TYPE_PHYSICS) 
			{		
				// ���������� ������ �ʴ´�.
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
		// ���� Immune ó���� �Ѵ�.
		if( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneTime > 0 )
		{
			if(lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneNumber == skill)
			{
#if TESTSERVER == 1
				MsgOutput(lpTargetObj->m_Index, "%d ����Immune����", skill);
#endif
				return TRUE;
			}
		}
#endif
		
#ifdef ADD_RAKLION_20080408
		// ��������, ���� Immune ó���� �Ѵ�.
		if( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneAllTime > 0 )
		{
			
#if TESTSERVER == 1
			MsgOutput(lpTargetObj->m_Index, "��������, ����Immune����");
#endif
			//////////////////////////////////////////////////////////////////////////
			// All Miss ó��
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
		// ���� �ڵ忡�� �� �̰� �־������� ������(��ȹ�ǵ�������...;;) ���� ���¿����� �����ϰ� �����ϰ� �Ѵ�.
		// �ؿ��� �����񾾰� ��ȹ���� Ȯ���� ��� ���� ��ü���� �������� ����� ���ϰ� ��ų�� �ٸ� �κп��� ���� ������ ���ٰ� �Ͽ�. ������.
		// ���� ����Ʈ���� ü�ζ���Ʈ�׿� ������ �ִٰ� ������.. �׿� combo ���� ��ų�� ������ �ٰ����� ���� �ƿ� ���Ź��� ó���� ���� �ʴ´�.
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
		// �����ڰ� �÷��̾�� ��밡 ���Ͷ�� ��ȯ������ üũ�Ѵ�.
		// �̶� �ڽ��� ��ȯ�� �����ϰ�� ������ �ȵȴ�.
		if( lpObj->m_RecallMon >= 0 )
		{
			if( lpObj->m_RecallMon == lpTargetObj->m_Index ) return FALSE;
		}
	}
	
	// �����ص� �Ǵ��� �����.
	if( gObjAttackQ(lpTargetObj) == FALSE ) return FALSE;
	
#ifdef CASTLE_MAIN_SCHEDULER_20041111		
// �������� ����� ���ݴ��� ���� -> ������ �߿� ���� ���� ���ݹ����� �ʵȴ�.
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

	// ���� ��ȯ���� �����Ѵٸ�.. ���� üũ�Ѵ�
	if( lpObj->m_RecallMon >= 0 )
		gObjCallMonsterSetEnemy(lpObj, lpTargetObj->m_Index);

	lpObj->m_TotalAttackCount++;

#ifdef MODIFY_ARROW_DECREASE_ELF_20040503
	if( !AttackDamage )
	{
#endif
		// ���� ȭ�� ����
		
	#ifdef ADD_ITEM_FENRIR_01_20051110
		// �渱�� �ö�������� ��쿡�� ȭ���� ���ҽ�Ű�� �ʴ´�.
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

	// ���� ������ üũ �Ѵ�
	if( !CheckAttackArea(lpObj, lpTargetObj) )
		return FALSE;
	
	// ��ȯ������ ��� ���μ���
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

	// ������ üũ
	if( !PkCheck(lpCallObj, lpTargetObj) )
		return FALSE;

	int	Strength	= lpObj->Strength+lpObj->AddStrength;
	int	Dexterity	= lpObj->Dexterity+lpObj->AddDexterity;
	int	Vitality	= lpObj->Vitality+lpObj->AddVitality;
	int	Energy		= lpObj->Energy+lpObj->AddEnergy;

#ifdef DUEL_SYSTEM_20031028
	BOOL bIsOnDuel = gObjDuelCheck(lpObj, lpTargetObj);

#ifdef DUEL_SYSTEM_FIX_20031224		// �ο�� ������ ���� ���� ���̶�� ����ī��Ʈ�� �����Ѵ�.
	if (bIsOnDuel) {
		lpObj->m_iDuelTickCount = GetTickCount();
		lpTargetObj->m_iDuelTickCount = GetTickCount();
	}
#endif
#endif

	// ũ����Ż ���� ��� �־��ٸ�.. 1/20 �� �󸮱�..
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

	// ���׷� üũ
	skillSuccess = ResistanceCheck(lpObj, lpTargetObj, skill);

#ifdef MODIFY_ELF_ICEARROWSKILL_BUGFIX_20060914
	int skillIceArrowSuccess = skillSuccess;	// skillSuccess ���� ����صд�. 
	if ( skill == AT_SKILL_ELFHARDEN )
		skillSuccess = 0;						// MissCheck���� �� ���� ����ϱ� ������ 0���� �����ص�.
#endif
	
#ifdef NEW_FORSKYLAND3
	int bAllMiss = FALSE;
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	// �����ڰ� ���� ���¿� �ִٸ� ������¸� �����Ѵ�.
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
	//  [] �������� ������ ���� ��´�.
	//
	//	1. AttackDamage �� 0 �� ��� �Ϲ� ����
	// 	2. AttackDamage �� 0 �� �ƴ� ��� �ݻ� ����
	//
	//
	//---------------------------------------------------------------------------------
	if( AttackDamage == 0 )
	{
		// �̽� üũ
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
		// ���̽��ַο� ��ų�� Miss üũ �Ŀ� ������ �Ѵ�.
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
				lpTargetObj->m_ViewSkillState |= STATE_HARDEN; //	��ȭ
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				lpTargetObj->PathCount = 0;

	#ifdef MODIFY_MONSTER_ACT_BY_ICEARROW_20050914
				lpTargetObj->PathStartEnd = 0;
	#endif
				skillSuccess = 1;	// �� ����
				gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
		}
#endif // MODIFY_ELF_ICEARROWSKILL_BUGFIX_20060914

		// ����� ������ ��´�
		int targetdefense = GetTargetDefense(lpObj, lpTargetObj, MsgDamage);

#ifdef SEASON4_PATCH_GROOVING
		//------>
		// ���� 4.0 �߰� ��ų  grooving 2008/04/24
		// grooving ���� ����
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
	

#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111	// �̳ʺ��̼�(���� �϶�)
		int iCurseValue = 0;
		gObjGetValueOfBuffIndex( lpTargetObj, BUFFTYPE_CURSED_DEFENSE_DOWN, &iCurseValue );
		targetdefense	-= targetdefense * iCurseValue / 100;
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111		


		//---------------------------------------------------------------------------------
		//	
		//	[][] �⺻ ������
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
		if( skill == AT_SKILL_SWORD1  ||	//�������
			skill == AT_SKILL_SWORD2  ||	//���
			skill == AT_SKILL_SWORD3  ||	//�÷�ġ��
			skill == AT_SKILL_SWORD4  ||	//����ġ�� 
			skill == AT_SKILL_SWORD5  ||	//����
#ifdef ADD_SKILL_20031022
			skill == AT_SKILL_SWORD6  ||	//�Ŀ�������
#endif			
			skill == AT_SKILL_WHEEL   ||	//ȸ����

			// ��� �߰� ��ų
			skill == AT_SKILL_KNIGHTSPEAR ||	// ����â
			skill == AT_SKILL_BLOWOFFURY  ||	// �г��� �ϰ�
			skill == AT_SKILL_KNIGHTDINORANT || // ����Ʈ ����
			skill == AT_SKILL_STRIKE	  ||	// ��ο�
			
			// ���˻� �߰� ��ų
			skill == AT_SKILL_DEFENSEDOWN  // ���� ����
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
			|| skill ==AT_SKILL_RUSH			// �ݿ�����(���)
			|| skill ==AT_SKILL_ONE_FLASH		// �������� (����)
			|| skill ==AT_SKILL_SPACE_SPLIT		// ���̾�귡��Ʈ (��ũ)
#endif
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ȸ�������� ��ȭ - �⺻ �������� ���ϱ�(����� ���˻� ��ų�� �߰� �� �ָ� �ȴ�;)
			|| ( g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT )	// ȸ�������� ��ȭ(���)
			|| ( g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_WHEEL_MAGUMSA )	// ȸ�������� ��ȭ(���˻�)
#endif	
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �����ͷ��� 4��ũ ��ų �⺻ ������: ��ο� ��ȭ, �г��� �ϰ� ��ȭ
			||g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_STRIKE			// ��ο� ��ȭ(���)
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_BLOWOFFURY	// �г��� �ϰ� ��ȭ(���)
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN	// ���� ���� ��ȭ(����)
			|| g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_SWORD6		// �Ŀ� ������ ��ȭ(����)
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428			
#ifdef ADD_SEASON4_NEW_SKILL_20080522
			|| skill == AT_SKILL_MULTISHOT		// ��Ƽ��(����)
			|| skill == AT_SKILL_DESTRUCTION	// �ı��� �ϰ�
			|| skill == AT_SKILL_FLAMESTRIKE	// �÷��� ��Ʈ����ũ
			|| skill == AT_SKILL_CHAOTICDESAIR	// ī��ƽ ���̾�(��ũ�ε�)
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

			// �������� ������ �������� ��´�
			if( AttackDamage > 0 )
			{
				gObjWeaponDurDown(lpObj, lpTargetObj, 0);
			}
		}
#ifdef ADD_ITEM_FENRIR_01_20051110
		else if( skill ==AT_SKILL_FENRIR_ATTACK )
		{
			int iFenrirAttackDmg = 0;

			// �� �������� �ö������ �������� ���Ѵ�.
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
			
			// ũ��Ƽ��, ������Ʈ ������ ���θ� �����Ѵ�.
			if( lpObj->m_CriticalDamage > 0 ) 
			{
				if( (rand()%100) < lpObj->m_CriticalDamage )
				{
					MsgDamage = DT_CRITICAL;		// ũ��Ƽ�� ������
				}
			}
			if( lpObj->m_ExcelentDamage > 0 )
			{
				// ��Ʈ �ɼ� ; ������Ʈ ������ Ȯ�� ��������
				if( (rand()%100) < lpObj->m_ExcelentDamage )
				{
					MsgDamage = DT_EXCELLENT;		// ������Ʈ ������
				}
			}

			// ũ��Ƽ��, ������Ʈ ������ ����
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
				// ��ȭ�� ���� ��ȭ ������ �ɼ� : ũ��Ƽ�� ������ ����
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
				MsgOutput(lpObj->m_Index, "[�ö������⺻DMG] %d", AttackDamage);

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

			// �������� ������ �������� ��´�
			if( AttackDamage > 0 )
			{
				gObjWeaponDurDown(lpObj, lpTargetObj, 1);
			}
		}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ��ȯ���� ��ų ������
		else if ( lpObj->Class == CLASS_SUMMONER  && skill)
		{
			AttackDamage = GetAttackDamageSummoner(lpObj, targetdefense, lpMagic, MsgDamage, bIsOnDuel);

#if TESTSERVER == 1
			//TRACE_LOG("\n1.Calc AttackDamageSummoner %d \n",AttackDamage);
#endif
			
			// �������� ������ �������� ��´�
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

			// �������� ������ �������� ��´�
			if( AttackDamage > 0 )
			{
				gObjWeaponDurDown(lpObj, lpTargetObj, 0);
			}
		}

#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
		//����, ��ȭ�� ��ũ�� (ũ��Ƽ��, ������Ʈ ������)ó��
		//��� Class �� ����ǵ��� ���� �Լ��� �ƴ� �Ʒ��� ó�� �߰�
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
		//	[][] ALL Miss ó��
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
		//	[][] ������ ���� �ɼ�
		//
		//---------------------------------------------------------------------------------

		// �������� ���� ��Ų��.
		if( lpTargetObj->DamageMinus )
		{
			int beforeDamage=AttackDamage;
			AttackDamage -=  AttackDamage*lpTargetObj->DamageMinus/100;
			//if( AttackDamage < 0 ) AttackDamage = 0;
			//LogAdd("������ ���� : %d -> %d (%d)",beforeDamage, AttackDamage,lpTargetObj->DamageMinus);
		}

		//---------------------------------------------------------------------------------
		//	
		//	[][] ������ ���� �ּ� ������ ����
		//
		//---------------------------------------------------------------------------------

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// ���շ��� ���� - ������ ���� �ּ� ������ ����
		// �����༮�Ƿ���/10 ���� ���� ��� AttackDamage ���� 
		int tlevel = ( lpObj->Level + lpObj->m_nMasterLevel ) / 10;
		if( AttackDamage < tlevel )
		{
			if( tlevel < 1 ) tlevel = 1;
			AttackDamage = tlevel;
		}
#else
		// �����༮�Ƿ���/10 ���� ���� ��� AttackDamage ���� 
		int tlevel = lpObj->Level/10;
		if( AttackDamage < tlevel )
		{
			if( tlevel < 1 ) tlevel = 1;
			AttackDamage = tlevel;
		}
#endif

		//---------------------------------------------------------------------------------
		//	
		//	[][] ��� ��ų ���
		//
		//---------------------------------------------------------------------------------

		// ��ų ����
		if( lpTargetObj->m_SkillNumber == AT_SKILL_BLOCKING )
			if( AttackDamage > 1 ) AttackDamage >>= 1;

		//---------------------------------------------------------------------------------
		//	
		//	[][] ��ź, ��ȣõ��
		//
		//---------------------------------------------------------------------------------

		// ����� ������ �˻��ؼ� �������� ���� ��Ų��.
		gObjSpriteDamage(lpTargetObj, AttackDamage);					

		// ��ź������ ������ �ִٸ� ���ݷ� ����..
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
		// �׽�Ʈ �ڵ�
		{
			if(lpObj->Type == OBJTYPE_CHARACTER && skill == 0)
				TRACE_LOG("2. Hand AttackDamage %d \n",AttackDamage);

			if(lpObj->Type == OBJTYPE_CHARACTER && skill != 0)
				TRACE_LOG("2. Skill AttackDamage %d Skill Index: %d \n",AttackDamage, skill);
		}
*/
#endif // TESTSERVER

		// ��밡 õ�������� ������ �ִٸ�.. 
		if( gObjAngelSprite(lpTargetObj) == TRUE )
		{
			if( AttackDamage > 1 ) 
			{
#ifdef NEW_FORSKYLAND3	// ��ȣõ�� ������ ��� 30%���� 20%�� ����
				float damage = (float)(AttackDamage*8)/10;
#else
				float damage = (float)(AttackDamage*7)/10;
#endif
				AttackDamage = (int)damage;// 10���� 7�� ����ڰ�..  
			}
		}

		//---------------------------------------------------------------------------------
		//	
		//	[][] ����, ��ȣ����
		//
		//---------------------------------------------------------------------------------
		
#ifdef	ADD_SAFEGUARD_ITEM_20080702

		// grooving ���� ���� 40% ����
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
				
				AttackDamage = (int)damage;// 10���� 7�� ����ڰ�..  
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
				// ���ݷ� 30 ����
				AttackDamage += 30;
			}
		}
#endif // ADD_PREMIUMITEM_PANDA


		//---------------------------------------------------------------------------------
		//	
		//	[][] ����
		//
		//---------------------------------------------------------------------------------

		if( gObjWingSprite(lpObj) == TRUE )
		{
			CItem	*Wing = &lpObj->pInventory[EQUIPMENT_WING];
			// ���� ���� �����(20030721)
#ifdef NEW_FORSKYLAND3
			if( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_ELF)
#else
			if( lpObj->Class == CLASS_WIZARD )				
#endif
				lpObj->Life -= 1;
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ��ȯ���簡 ������ ���� ������ ���ݼ����� hp 1�� ����
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
#ifdef ADD_THIRD_WING_20070525	// 3������ ������ ����
				if( Wing->m_Type >= MAKE_ITEMNUM( 12, 36 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 40 ) )
				{
					// ������ ���� : 39+Level*2
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
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ���� ������ ���� ������ ����
				if(Wing->m_Type == MAKE_ITEMNUM(12,41))		// ����� ����
				{
					// ������ ���� : 12+Level
					AttackDamage = AttackDamage*(112+(Wing->m_Level*2))/100;	// 12% + level
				}
				else if(Wing->m_Type == MAKE_ITEMNUM(12,42))	// ������ ����
				{
					// ������ ���� : 32+Level*1
					AttackDamage = AttackDamage * ( 132 + ( Wing->m_Level * 1)) / 100;
				}
				else if(Wing->m_Type == MAKE_ITEMNUM(12,43))	// ������ ����
				{
					// ������ ���� : 39+Level*2
					AttackDamage = AttackDamage * ( 139 + ( Wing->m_Level * 2 )) / 100;
				}
				else
#endif	// ADD_SUMMONER_SECOND_THIRD_WING_20071130					
				if( Wing->m_Type > MAKE_ITEMNUM(12,2) )
				{	// �߰� ���� (������ ����)	
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
#ifdef ADD_THIRD_WING_20070525	// 3������ ������ ���, �����ɼ� - 100%����/���� ȸ��
					// �ϴ� ������ ���
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ���� ������ ���
					if( ( Wing->m_Type >= MAKE_ITEMNUM( 12, 36 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 40 ) )
						|| Wing->m_Type == MAKE_ITEMNUM( 12, 43 ) )	// ������ ����
#else
					if( Wing->m_Type >= MAKE_ITEMNUM( 12, 36 ) && Wing->m_Type <= MAKE_ITEMNUM( 12, 40 ) )
#endif
					{
						if( Wing->m_Type == MAKE_ITEMNUM( 12, 40 ) )	// �����Ǹ���
						{
							// 24% + ( level * 2 )
							float damage = ( float )( AttackDamage * ( 76 - ( Wing->m_Level * 2 )) ) / 100;
							AttackDamage = ( int )damage;
						}
						else
						{
							// 3�� ������ 	39% + ( level * 2 )
							float damage = ( float )( AttackDamage * ( 61 - ( Wing->m_Level * 2 )) ) / 100;
							AttackDamage = ( int )damage;	
						}			

						// ���� �ɼ�
						if( rand() % 100 < 5 )
						{
							BYTE bt3rdWingOpt = lpTargetObj->pInventory[EQUIPMENT_WING].m_NewOption;								
							if( ( bt3rdWingOpt & PLUS_THIRD_WING_RECOVER_100PER_LIFE ) == PLUS_THIRD_WING_RECOVER_100PER_LIFE ) 
							{
								// 5%Ȯ���� 100% ���� ����
								gObjAddMsgSendDelay( lpTargetObj, MSG_FULL_LIFE, lpObj->m_Index, 100, 0 );			
							}
							else if( ( bt3rdWingOpt & PLUS_THIRD_WING_RECOVER_100PER_MASIC ) == PLUS_THIRD_WING_RECOVER_100PER_MASIC )
							{
								// 5%Ȯ���� 100% ���� ����
								gObjAddMsgSendDelay( lpTargetObj, MSG_FULL_MANA, lpObj->m_Index, 100, 0 );
							}						
						}				
					}
					else
#endif
#ifdef NEW_FORSKYLAND3
					if( Wing->m_Type > MAKE_ITEMNUM(12,2) )
					{	// ���� ������ ��� �߰� ������ 25% + level
						float damage = (float)(AttackDamage*(75-(Wing->m_Level*2)))/100;
						AttackDamage = (int)damage;
					}
					else
#endif			
					{	// ���� ������ ��� ���� 12% + level*2
						float damage = (float)(AttackDamage*(88-(Wing->m_Level*2)))/100;
						AttackDamage = (int)damage;	
					}
					//LogAdd("[%s][%s] ���� ���� ȿ��2 %d", lpObj->AccountID, lpObj->Name, AttackDamage);
				}		
			}
		}

		//---------------------------------------------------------------------------------
		//	
		//	[][] ����Ʈ
		//
		//---------------------------------------------------------------------------------

#ifdef NEW_FORSKYLAND3
		if( gObjDenorantSprite(lpObj) )
		{	// ����Ʈ ������ ���� 15%
			
			// ����Ʈ�� Ÿ�� �����ϰ� ������ �������� �������� 1�� ��´�.
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
		{	// ����Ʈ ������ ��� 10%
			CItem	*Dinorant = &lpTargetObj->pInventory[EQUIPMENT_HELPER];

			// 5%�߰� ������ ��� �ɼ� ����
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
		//	[][] ��ũ ȣ�� 
		//
		//---------------------------------------------------------------------------------

#ifdef DARKLORD_WORK
		if( gObjDarkHorse(lpTargetObj) )
		{	
			// ��ũȣ�� ������ ��� (10+(���/100))
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
			//	[][][] ��ų ������ �� ���ϱ�
			//
			//---------------------------------------------------------------------------------

#if defined( ADD_SKILL_WITH_COMBO ) || GAME_VERSION == G_V_97D

			switch(skill)
			{
				case AT_SKILL_SWORD1:			//	�������
				case AT_SKILL_SWORD2:			//	���
				case AT_SKILL_SWORD3:			//	�÷�ġ��
				case AT_SKILL_SWORD4:			//	����ġ��
				case AT_SKILL_SWORD5:			//	����
				case AT_SKILL_WHEEL:			//	ȸ��������		
				case AT_SKILL_BLOWOFFURY:		//	�г��� �ϰ�
				case AT_SKILL_STRIKE:			//	��ο�(���)
				case AT_SKILL_KNIGHTDINORANT:	//	����Ʈ ����
				case AT_SKILL_DEFENSEDOWN:		//	���� ����(����)
	#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
				case AT_SKILL_RUSH:				//	�ݿ�����(���)
				case AT_SKILL_ONE_FLASH:		//	�������� (����)
	#endif
				{
					// ��ų ������(������/10)
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
				
				case AT_SKILL_KNIGHTSPEAR:	// ����â
					{	
						if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) || 
							lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) 
	#ifdef ADD_ITEM_FENRIR_01_20051110
							// �渱 ����ÿ��� â��� ��� ����
							|| lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,37)
	#endif
						 )

						{	// ���ϸ��� ����Ʈ
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

				case AT_SKILL_SWORD6:			// ���˽�ų
				{	
					//AttackDamage = AttackDamage*190/100;
					AttackDamage = AttackDamage*2;
					break;
				}

	#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
				case AT_SKILL_DEEPIMPACT:		//������Ʈ(�����)
	#endif
				case AT_SKILL_PENETRATION:		//����(����)
				case AT_SKILL_ELFHARDEN:		// ��ȭ(����)
					{	
						AttackDamage = AttackDamage*2;
					}
				break;	
				
	#ifdef DARKLORD_WORK
		#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
				case AT_SKILL_SPACE_SPLIT:		// ���̾�귡��Ʈ (��ũ)
		#endif
				case AT_SKILL_SPEAR:			// ���˽�ų
				case AT_SKILL_FIREBURST:				
				case AT_SKILL_DARKHORSE_ATTACK:
				case AT_SKILL_ELECTRICSPARK:
		#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
				case AT_SKILL_FIRESCREAM:		// ���̾ũ�� (��ũ)
		#endif
					AttackDamage = AttackDamage*(200+(lpObj->Energy+lpObj->AddEnergy)/20)/100;
					break;
	#endif // DARKLORD_WORK

	#ifdef ADD_ITEM_FENRIR_01_20051110
				case AT_SKILL_FENRIR_ATTACK:	// �渱 ��ų - �ö�� ����
					{
						// �渱 �ּ� ���� ���� ���� ���� �󸶳� ������ ū�� ���Ѵ�.
						int iDamageInc = lpObj->Level - MIN_FENRIR_EQUIP_LEVEL ;

		#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// ���շ��� ���� - �ö������ ���� Ư�� �������� ������ ������ ���.
						iDamageInc += lpObj->m_nMasterLevel;
		#endif

						if( iDamageInc < 0 )
						{
							iDamageInc = 0;
						}

						// Ư�� ���� ���� ������ ���� ���� ����Ѵ�.
						iDamageInc = iDamageInc / MAX_FENRIR_DAMAGE_INC_LEVEL;

						AttackDamage = AttackDamage * (200+iDamageInc) / 100;

					}
					break;
	#endif // ADD_ITEM_FENRIR_01_20051110

	#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ��ų ������
				case AT_SKILL_LIGHTNINGORB:
					{
						// �ֺ� ���÷���������
						gObjUseSkill.SplashDamage(lpObj, lpTargetObj, AT_SKILL_LIGHTNINGORB, AttackDamage);
					}
					break;
					
				case AT_SKILL_DRAINLIFE:
					{
						
						//int nAddHP = AttackDamage * 20 / 100;
						int nAddHP = 0;
						if(lpTargetObj->Type == OBJTYPE_MONSTER)
						{
							// ������/15 + ���ͷ��� / 2.5�� ü������ ���
							nAddHP = (lpObj->Energy / 15) + (lpTargetObj->Level / 2.5);
						}
						else if( lpTargetObj->Type == OBJTYPE_CHARACTER )
						{
							// (���������� * 10%) + (������ / 23 )
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
						// �������� ������ �����ð� ������ ����
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_FIREDOT, EFFECTTYPE_GIVE_DMG_TICK, DotDamage , 0, 0, 5);
					}
					break;
				case AT_SKILL_REQUIEM:
					{
						int DotDamage = AttackDamage * 60 / 100;
						// �������� ������ �����ð� ������ ����
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_BLOODDOT, EFFECTTYPE_GIVE_DMG_TICK, DotDamage, 0, 0, 5);
					}
					break;
				case AT_SKILL_SLEEP:
					{
						// ���⼭ �ٽ� ������ ���
						int skillSuccessRate = 0;
#ifdef MODIFY_SLEEP_SKILL_SUCCESSRATE_BUGFIX_20080125	// ������ ����
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
#ifdef MODIFY_SLEEP_SKILL_SUCCESSRATE_BUGFIX_20080125	// �ð� ����
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
						
						// ���� ������ ���۴� �������
						if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_SLEEP, nEffectTime, 0 ) == true )
						{
							GCMagicAttackNumberSend( lpObj, AT_SKILL_SLEEP, lpTargetObj->m_Index, 0 );	// ��ų��� ����
							return FALSE;
						}
						// ��������
						gObjAddBuffEffect(lpTargetObj, BUFFTYPE_SLEEP, EFFECTTYPE_SLEEP, nEffectTime, 0, 0, nEffectTime);

						return TRUE;
					}
					break;

	#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
				case AT_SKILL_WEAKNESS:
					{
						int nSkillSuccessRate = 0;
						// ������ ���
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
								GCMagicAttackNumberSend( lpObj, AT_SKILL_WEAKNESS, lpTargetObj->m_Index, 0 );	// ��ų��� ����
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
						// ������ ���
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
								GCMagicAttackNumberSend( lpObj, AT_SKILL_ENERVATION, lpTargetObj->m_Index, 0 );	// ��ų��� ����
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
					// ���� 4.0 �߰� ��ų  grooving 2008/04/24
					// �ܼ��� Ŭ���̾�Ʈ ����Ʈ ǥ���� ���ؼ�.
				case	AT_SKILL_LIGHTING_SHOCK: // ����Ʈ�� ��ũ 
					{
						gObjAddBuffEffect( lpTargetObj, BUFFTYPE_LIGHTNINGSHOCK, 0, 0, 0, 0, 1 );
					}
					break;
					// <---
#endif

	#ifdef ADD_SEASON4_NEW_SKILL_20080522	// �ű� ��ų �߰�
				case  AT_SKILL_DESTRUCTION:		// �ı��� �ϰ�
					{
						lpTargetObj->lpAttackObj = lpObj;
						lpTargetObj->DelayActionTime = 1000;
						lpTargetObj->DelayLevel = 1;
						gObjAddBuffEffect( lpTargetObj, BUFFTYPE_COLD, EFFECTTYPE_REDUCE_MOVE_SPEED, 0, 0, 0, 10 );
						AttackDamage = AttackDamage*(200+Energy/10)/100;
					}
					break;
				case AT_SKILL_MULTISHOT:		// ��Ƽ��
					break;
				case AT_SKILL_FLAMESTRIKE:		// ������ ��Ʈ����ũ
					{
						AttackDamage = AttackDamage * 2;
						gObjAddBuffEffect( lpTargetObj, BUFFTYPE_FLAMESTRKE, EFFECTTYPE_NONE, 0, 0, 0, 1 );
					}
					break;
				case AT_SKILL_GIGANTICSTORM:	// �Ⱓƽ ����
					{
						gObjAddBuffEffect( lpTargetObj, BUFFTYPE_GIGANTICSTORM, EFFECTTYPE_NONE, 0, 0, 0, 1 );
					}
					break;
				case AT_SKILL_CHAOTICDESAIR:	// ī���� �����̾�
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
			
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �����ͷ��� ��Ƽ�� ��ų ������ �� ���ϱ�
			//---------------------------------------------------------------------------------
			//	
			//	[][][] �����ͷ��� ��Ƽ�� ��ų ������ �� ���ϱ�
			//
			//---------------------------------------------------------------------------------	
			if( g_MasterSkillSystem.CheckMasterLevelSkill( skill ) )
			{
				INT iSkillValue = MagicDamageC.SkillGet( lpMagic->m_Skill );
				INT iMLSBase	= g_MasterSkillSystem.GetBaseMasterLevelSkill( skill );
				switch( iMLSBase ) 
				{
				case AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT:			// ȸ�������� ��ȭ(���)
					AttackDamage = AttackDamage * ( 200 + Energy / 10 ) / 100;					
					break;

				case AT_SKILL_ML_STRENGTHEN_WHEEL_MAGUMSA:			// ȸ�������� ��ȭ(���˻�)
					AttackDamage = AttackDamage * 2;					
					break;
					
				case AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK:		// �����ũ ��ȭ
					AttackDamage = AttackDamage * ( 200 + ( lpObj->Energy + lpObj->AddEnergy ) / 20 ) / 100;					
					break;

				case AT_SKILL_ML_STRENGTHEN_FIREBURST:				// ���̾����Ʈ ��ȭ
					AttackDamage = AttackDamage * ( 200 + ( lpObj->Energy + lpObj->AddEnergy ) / 20 ) / 100;					
					break;

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// ������ ���� 4��ũ
				case AT_SKILL_ML_STRENGTHEN_EVIL:					// �Ƿ� ��ȭ
					break;
				case AT_SKILL_ML_STRENGTHEN_EXPICE:					// ���� ���̽� ��ȭ
					AttackDamage += iSkillValue;
					break;
				case AT_SKILL_ML_STRENGTHEN_STRIKE:					// ��ο� ��ȭ(���)
					AttackDamage = AttackDamage*(200+Energy/10)/100;
					break;
				case AT_SKILL_ML_STRENGTHEN_BLOWOFFURY:				// �г��� �ϰ� ��ȭ(���)
					AttackDamage = AttackDamage*(200+Energy/10)/100;
					break;
				case AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN:			// ���� ���� ��ȭ(����)
					AttackDamage = AttackDamage * 2;
					break;
				case AT_SKILL_ML_STRENGTHEN_SWORD6:					// �Ŀ� ������ ��ȭ(����)
					AttackDamage = AttackDamage * 2;
					break;
				case AT_SKILL_ML_STRENGTHEN_FIRESCREAM:				// ���̾� ��ũ�� ��ȭ(��ũ)
					AttackDamage = AttackDamage*(200+(lpObj->Energy+lpObj->AddEnergy)/20)/100;
					break;
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428

#ifdef UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808	// 
				case AT_SKILL_ML_STRENGTHEN_MAGUMSA_EVIL:			// �Ƿ� ��ȭ(���˻�)
					break;
#endif	// UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
				}
			}
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107

#else
			
	/*
	//
	//	
	//	20051110 ADD_SKILL_WITH_COMBO �� ELSE �κ� ���� �� Attack �Լ� ����ȭ (apple)
	// 
	//  [] Depth 1
	//	[][] Depth 2
	//	[][][] Depth 3
	//
	*/

#endif // ADD_SKILL_WITH_COMBO

			
			//---------------------------------------------------------------------------------
			//	
			//	[][][] ��ų�� �ƴ� ���� �϶� �������� ���Ѵ�.
			//
			//---------------------------------------------------------------------------------

			if( !skill )
			{
				if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) )
				{	// ����Ʈ �Ϲ� ���ݵ� ������ �߰�
					AttackDamage = AttackDamage*130/100;
				}
				
			}

			//---------------------------------------------------------------------------------
			//	
			//	[][][] ���� �ҿ� �ٸ��� ������ ��� ����
			//
			//---------------------------------------------------------------------------------

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �ҿ�ٸ��� : �������� ������ ���(Attack)
			// ���� �ҿ�ٸ���(�������� ������ ��� ��Ų��)
			if( ( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC ) == true ) && AttackDamage > 0 )
			{	
				// �������� ������
				int iWizardSkillDefense = 0;
				int iManaRate			= 0;
				gObjGetValueOfBuffIndex( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, &iWizardSkillDefense, &iManaRate );
				// ���� ����			
				int iReplacementMana = 0;
				if( iManaRate > 0 )
					iReplacementMana = (WORD)lpTargetObj->Mana * iManaRate / 1000;	
				else
					iReplacementMana = (WORD)lpTargetObj->Mana * 2 / 100;	

				if( lpTargetObj->Mana > iReplacementMana )
				{	
					// ��밡���� ������ �־�� �ȴ�
					lpTargetObj->Mana -= iReplacementMana;				

					int decattackdamage = 0;
					if( iWizardSkillDefense > 0 )
						decattackdamage = AttackDamage * iWizardSkillDefense / 100;	

					// ������ ����
					AttackDamage -= decattackdamage;
					ManaChange = TRUE;
				}
			}

#else
			if( ( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC ) == true ) && AttackDamage > 0 )
			{	// ���� �ҿ�ٸ���(�������� ������ ��� ��Ų��)
				int replacemana = (WORD)lpTargetObj->Mana*2/100;		// ���� 2%����
				if( lpTargetObj->Mana > replacemana )
				{	// ��밡���� ������ �־�� �ȴ�
					lpTargetObj->Mana -= replacemana;				// �������� ������
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
			{	// ���� �ҿ�ٸ���(�������� ������ ��� ��Ų��)
				int replacemana = (WORD)lpTargetObj->Mana*2/100;		// ���� 2%����
				if( lpTargetObj->Mana > replacemana )
				{	// ��밡���� ������ �־�� �ȴ�
					lpTargetObj->Mana -= replacemana;				// �������� ������
						int decattackdamage = AttackDamage*lpTargetObj->m_WizardSkillDefense/100;	// ������ ����

					AttackDamage -= decattackdamage;
					ManaChange = TRUE;
				}
			}
#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			
			//---------------------------------------------------------------------------------
			//	
			//	[][][] ��Ʈ �ɼ� ������ ���� ����
			//
			//---------------------------------------------------------------------------------
			
			AttackDamage += lpObj->SetOpAddDamage;			// ��Ʈ�ɼ� ����������

#ifdef ADD_380ITEM_NEWOPTION_20060711
			// 380 ������ �߰� �ɼ� : ���� ������ ���� +%d
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				AttackDamage +=lpObj->m_ItemOptionExFor380.OpAddDamage;
			}
#endif // ADD_380ITEM_NEWOPTION_20060711
			
			//NEW BALANCE DEVGAMESX
			gBalanceSystem.Main(lpObj, lpTargetObj, AttackDamage);

			//---------------------------------------------------------------------------------
			//	
			//	[][][] ī���� ĳ�� ������ ����
			//
			//---------------------------------------------------------------------------------

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �ȿ��� ����� ���� ���� �� ������ ����
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
			//	[][][] ������ ������ ����
			//
			//---------------------------------------------------------------------------------

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// ������ �߿� ������ �ȿ����� ����� ���� ���� �� ������ ������ 80% ����, �ٸ��� 60% ����
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

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116 // �ູ�� ���� ��� ȿ��!
			if( lpObj->Type == OBJTYPE_CHARACTER 
				&& lpTargetObj->Type == OBJTYPE_MONSTER )
			{
				// ��ȣ ���� ���� ��
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
						// �߰� ������ ���� ����.
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

						// ������ 20%�� ���� -> 5% ����(20050303)
						AttackDamage = AttackDamage * 5 / 100;

					}

				}

				// ���� ���� ��
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
						// �߰� ������ ���� ����.
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

						// ������ 20%�� ���� -> 5% ����(20050303)
						AttackDamage = AttackDamage * 5 / 100;

					}
				}

			}
#endif
			

			//---------------------------------------------------------------------------------
			//	
			//	[][][] �渱 ���� ������ ����, ��� ����
			//
			//---------------------------------------------------------------------------------

#ifdef ADD_ITEM_FENRIR_01_20051110
			// Main Attack �Լ�
			
			// �渱�� ���� ������ ���� ����
			if( gObjFenrir(lpObj) )
			{
				int iIncPercent = lpObj->pInventory[EQUIPMENT_HELPER].IsFenrirIncLastAttackDamage();

				if( iIncPercent > 0 )
				{
					AttackDamage += AttackDamage * iIncPercent / 100;
				}
			}
			
			// �渱�� ���� ������ ���� ����
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

			// �渱 �ö�����迡 ���� ������ ���� ���� - ���� �� ���� ���� ���� ���� �ʴ´�.
			if( 
				skill == AT_SKILL_FENRIR_ATTACK 
				&& lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER
				&& AttackDamage > 0  
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	#ifdef MODIFY_CASTLEMAP_FENRIR_DURATION_20060522
				// ����/���� ���϶��� �ش���. �ƹ� ���� �ƴ� ���, ������ ���Ҹ� �� �� ����.
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
					
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// �нú� 1�� - ��������1 : �ö�����迡 ���ݹ��� ��� ���� ����
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
					MsgOutput( lpObj->m_Index, "[�渱��ų] (%s) �� %s ������ (%d->%d)", 
						lpEquipment->GetName(), lpEquipment->m_Durability, iDurEquipment);
					
					MsgOutput( lpTargetObj->m_Index, "[�渱��ų] %s ������ (%d->%d)", 
						lpEquipment->GetName(), lpEquipment->m_Durability, iDurEquipment);
#endif
#ifdef MODIFY_NOT_EFFECT_FENRIRSKILL_IF_CHARM_PROTECTITEM_20090924
					// ������ ��ȣ ���� ���� ������ ������ ���� ����
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
			//	[][][] ���� �޴� Target�� SD�� HP�� ����
			//
			//---------------------------------------------------------------------------------

			// CODEREVIEW_SHIELD : ����!!
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				// CODEREVIEW_SHIELD : ������ ��� ���� ����
				// CODEREVIEW_SHIELD : ���� ������ ���� ���ϴ� ��� ����
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
			//	[][][] ���� �޴� Target ���� AttackDamage ���� 
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
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ����Ʈ�� ������ ���÷����������� ���� MissCheck
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

#ifndef MODIFY_CC_PVP_COMBO_DAMAGE_LOWER_50_20040917		// ���� ī����ĳ�� PVP ������ ����
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �ȿ��� ����� ���� ���� �� ������ ����
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

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// ������ �߿� ������ �ȿ����� ����� ���� ���� �� ������ ������ 80% ����, �ٸ��� 60% ����
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

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���̾ũ�� ������ �������ݻ� ������ ���� �ʴ´�.
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ����Ʈ�׿��� ���÷��� �������� �������ݻ� ����
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
		MsgDamage = DT_MIRROR;		// �ݻ� ����
#else
		MsgDamage = 0x20;
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	
		}
#endif
		
#ifdef ADD_SHIELD_POINT_01_20060403
		if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
		{
			// CODEREVIEW_SHIELD : ���� ������ ���� ���ϴ� ��� ����
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
	//  [] Target �� ������ ��� ���� �޼��� Q�� ���� ���� �޼��� �߰�
	//
	//
	//---------------------------------------------------------------------------------
	
	// ���ϴ°� �����϶�..
	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		// ���ݴ��ߴ�..
		gObjAddMsgSendDelay(lpTargetObj, 0, lpObj->m_Index, 100);
		
#ifdef FOR_BLOODCASTLE
		lpTargetObj->LastAttackerID = lpObj->m_Index;	// ������ ������ ����� �ε����� ����
#endif
		
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
		// ���⼭ Agro�� ���� �����ش�.
		if( lpTargetObj->m_iCurrentAI )
		{
			lpTargetObj->m_Agro.IncAgro(lpObj->m_Index, AttackDamage/50);
		}
#endif
		
	}
	
	//---------------------------------------------------------------------------------
	//
	//  [] ���� ���� ����, ������, ���̽��ַο�(��ȭ) ȿ�� ó��
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
		// �÷��̾�鳢��..
		// ������ �Ͽ� �����ϸ� ��������� �����Ѵ�.
		if( lpObj->Type	== OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
		{			
#ifdef DUEL_SYSTEM_20031028
			if (gObjDuelCheck(lpObj, lpTargetObj)) {	// ���� ���� ���̶�� ������� �����ʵ�
				selfdefense = FALSE;
			}

#ifdef ADD_NEWPVP_PKFIELD
			else if (g_NewPVP.IsVulcanusMap(lpObj->MapNumber) && g_NewPVP.IsVulcanusMap(lpTargetObj->MapNumber)) {
				selfdefense = FALSE;
			}
#endif // ADD_NEWPVP_PKFIELD

#ifdef CHAOSCASTLE_SYSTEM_20040408						// ī���� ĳ�� �ȿ����� ��������� �������� �ʴ´�.
			else if (CHECK_CHAOSCASTLE(lpObj->MapNumber) || CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) {
				selfdefense = FALSE;
			}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��� �ȿ����� ��������� �������� �ʴ´�.
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
			// ���� ��� �̸� ���� ���� ������ ���� �ʴ´�.
			// �ƹ����� ���� ����. (���� ���� �϶���..)
			if( gObjGetRelationShip( lpObj, lpTargetObj ) == G_RELATIONSHIP_RIVAL )
			{
				selfdefense = FALSE;
			}
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111					// ������ �� ������ �������� ������� �����ʵ�
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
		{	// ������ �����ϸ� �������� ���� ��Ų��			
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ������ �ɷ������� Ǯ���ش�
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_SLEEP ) == true )
		{
			gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_SLEEP);
		}
#endif

	#else
		if( lpTargetObj->m_SkillHarden != 0 )
		{	// ������ �������� ��ȭ�� �ɷ� ������, ������ Ǯ�� �ش�

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

#ifdef MODIFY_0708_BUFFIX_20070820	// ���� ��Ƽ�������� ������� ���� ����
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
#ifdef UPDATE_PK_SYSTEM_20070302	// ��Ƽ�� ��Ƽ�� ��ü�� ������� ����
#ifdef MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022	// ���濡 �ɸ� ������ SelfDefense�� �ִ´�
			if( lpCallObj->PartyNumber >= 0 )
			{
				int number = 0;
				int partynum = lpCallObj->PartyNumber;
				
				if( gParty.GetPKPartyPenalty(partynum) < PK_LEVEL_DEFAULT+2)		// PK �г�Ƽ ����� ����
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
				
				if( gParty.GetPKPartyPenalty(partynum) < PK_LEVEL_DEFAULT+2)		// PK �г�Ƽ ����� ����
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
	//  [] Ư�� ���Ͱ� ���� �޾��� �� �ൿ ó��
	//
	//
	//---------------------------------------------------------------------------------

#ifdef ADD_NEW_MAP_KALIMA_20040518
	// ����� ȯ���� ���� 1/5 Ȯ���� �ӹ��Ÿ���.
	if (
//		lpTargetObj->Class == 161 ||		// ����� ȯ��1
//		lpTargetObj->Class == 181 || 		// ����� ȯ��2
//		lpTargetObj->Class == 189 || 		// ����� ȯ��3
//		lpTargetObj->Class == 197 || 		// ����� ȯ��4
//		lpTargetObj->Class == 267 || 		// ����� ȯ��5
		lpTargetObj->Class == 275 			// ����� ȯ��6
		)
	{
		if (lpTargetObj->m_iMonsterBattleDelay <= 0) 
		{
			if (rand()%15 < 1) 
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100);
				lpTargetObj->m_iMonsterBattleDelay = 10;

				// �ӹ��Ÿ��ٴ� �ൿ�� ������.
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
		//�����̳� �����Ǽ����̶�� ������ �ӹ��Ÿ���.
		gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100);
		gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
	}
	else if( AttackDamage >= 5 )
#else
	if( AttackDamage >= 5 )
#endif
	{		
		if( lpTargetObj->Type == OBJTYPE_MONSTER ) // ����ڰ� ����
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
	//  [] Target�� �����̰�, �ҿ�ٸ��� ����Կ� ���� ������ ��ü ó��
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
	//  [] ������ ��� ������ ������. (����/��ų, ��ų��ȣ)
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
	//  [] Attack �Լ� ������ ...
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
			//LogAdd("������ �ݻ� %x ", atd_reflect);
			
			gObjAddMsgSendDelay(lpTargetObj, MSG_REFLECT, lpObj->m_Index, 10, atd_reflect);
		}

		if( rand()%100 < lpObj->SetOpReflectionDamage )
		{	// ��Ʈ �ɼ� 100% ������ �ݻ� 
			gObjAddMsgSendDelay(lpTargetObj, MSG_REFLECT, lpObj->m_Index, 10, AttackDamage);
		}

#ifdef ADD_THIRD_WING_20070525	// 3������ �����ɼ� ó�� - �� ���ݷ� �ǵ�����
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
						// 5% Ȯ���� �� ���ݷ� ������
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
				// CODEREVIEW_SHIELD : ���� ������ ���� ���ϴ� ��� ���� (�޺� �������� ���� ������ ���)
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
		{	// ���� ������

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���� �������� ��� ������ ���Ͱ� �������� �޴´�.
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// ���̾� ��ũ�� ��ȭ �߰�
			if( skill == AT_SKILL_FIRESCREAM 	// ���̾� ��ũ�� ��ų�� ���
				||  g_MasterSkillSystem.GetBaseMasterLevelSkill( skill ) == AT_SKILL_ML_STRENGTHEN_FIRESCREAM	// ���̾� ��ũ�� ��ȭ
				)
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
			if( skill == AT_SKILL_FIRESCREAM )	// ���̾� ��ũ�� ��ų�� ���
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
			{
				gObjUseSkill.FireScreamExplosionAttack( lpObj, lpTargetObj, AttackDamage );		
			}
#endif	// #ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601

#ifdef ADD_SHIELD_POINT_01_20060403
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				// CODEREVIEW_SHIELD : ���� ������ ���� ���ϴ� ��� ����
				iTempShieldDamage		= GetShieldDamage( lpObj, lpTargetObj, AttackDamage );
				lpTargetObj->iShield	= lpTargetObj->iShield - iTempShieldDamage;
				lpTargetObj->Life		= lpTargetObj->Life - (AttackDamage - iTempShieldDamage);
				iTotalShieldDamage		= iTotalShieldDamage + iTempShieldDamage;
				// SHIELD_DEBUGING_HJLIM_20060420 : ���� ������ �ÿ� AttackDamage 2�� ����.
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
			// Ŭ���̾�Ʈ���� ������ ���� ������ (HP ���ҷ�) ���
			AttackDamage	= AttackDamage - iTotalShieldDamage;
		}
#endif
		
		
#ifdef MODIFY_CC_PVP_COMBO_DAMAGE_LOWER_50_20040917		// ���� ī����ĳ�� PVP ������ ����
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �ȿ��� ����� ���� ���� �� ������ ����
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
#ifdef MODIFY_CC_PVP_COMBO_DAMAGE_LOWER_50_20040917		// ���� ī����ĳ�� PVP ������ ����
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �ȿ��� ����� ���� ���� �� ������ ����
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
	{	// ������ ���� �ɼǿ� ���� ĳ���� ü���� ���̸� 3���Ŀ� �ٽ� ���� üũ�� �ؼ� ���δ�
		if( lpObj->m_CheckLifeTime <= 0 )
		{
#ifdef USER_DIE_MAKE_OPPOSIT_PK_20040329
			lpObj->lpAttackObj = lpTargetObj;

			if (lpTargetObj->Type == OBJTYPE_CHARACTER) 
			{
				lpObj->m_bAttackerKilled = true;		// ���� ������ �����ϴ� �׾����Ƿ� ������ ���� ���� �����ڴ� �ƴϴ�.
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

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// Ư�� ��ų�� ���� �߰� ó��
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
// CODEREVIEW_SHIELD : bIsOnDuel �� TRUE �� ��� ���������� FALSE
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
				// ��ų ������
#ifdef DARKLORD_WORK
			if( lpMagic->m_Skill == AT_SKILL_SPEAR && lpObj->SkillLongSpearChange )
			{	// �ս��Ǿ� ���� �������� ������ �ش�
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
				
				// ��Ʈ�ɼ� : ��ų ���ݷ� ����
				SkillRightMaxDamage += lpObj->SetOpAddSkillAttack;
				SkillRightMinDamage += lpObj->SetOpAddSkillAttack;
				
				SkillLeftMaxDamage += lpObj->SetOpAddSkillAttack;
				SkillLeftMinDamage += lpObj->SetOpAddSkillAttack;

				int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
				if( SkillAttr != -1 )
				{
#ifdef UPDATE_GM_FUNCTION_20070228	// GM ĳ���� ���Ź��� ���� �� Resistance���� MAX��
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
				// ��ȭ�Ǻ��� ��ȭ ������ �ɼ� : ��ų ���ݷ� ����
				SkillRightMaxDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
				SkillRightMinDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
				
				SkillLeftMaxDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
				SkillLeftMinDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
	#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530			

#ifdef DARKLORD_WORK			
			if( lpMagic->m_Skill == AT_SKILL_ELECTRICSPARK )
			{	// �Ϸ�Ʈ���� ��ų ���ݷ� �ܿ� ��Ƽ���� ���� �߰� �������� �ִ�				
				int iPartyCount = GetPartyMemberCount(lpObj);
				int addskilldamage = (lpObj->Leadership+lpObj->AddLeadership)/10+iPartyCount*50;

				SkillRightMaxDamage += addskilldamage;
				SkillRightMinDamage += addskilldamage;
				
				SkillLeftMaxDamage += addskilldamage;
				SkillLeftMinDamage += addskilldamage;
			}
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��Ƽ�꽺ų - �����ũ��ȭ : ��ũȣ���� Ÿ�� �־�� �ȴ�.
			else if( lpMagic->m_Skill == AT_SKILL_DARKHORSE_ATTACK 
				|| g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK 
				)
#else
			else if( lpMagic->m_Skill == AT_SKILL_DARKHORSE_ATTACK )
#endif
			{	// � ����ũ
				if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,4) )
				{	// ��ũȣ���� Ÿ�� �־�� �ȴ�
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
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// ī��ƽ ���̾� ��ų��(��ũ�ε�)
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
				effect = DT_CRITICAL;		// ũ��Ƽ�� ������
#else
				effect = 0x80;
#endif
			}
		}
		if( lpObj->m_ExcelentDamage > 0 )
		{
			// ��Ʈ �ɼ� ; ������Ʈ ������ Ȯ�� ��������
			if( (rand()%100) < lpObj->m_ExcelentDamage )
			{
				cDamage = 1;
#ifdef ADD_SKILL_WITH_COMBO
				effect = DT_EXCELLENT;		// ������Ʈ ������
#else
				effect = 0x40;
#endif
			}
		}
#ifdef SEASON4_PATCH_GROOVING
		// ����Ŀ ���� ���� ���ݷ� �������� �̸� ������ �ش�.
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
			// ���� 4.0 �߰� ��ų  grooving 2008/04/24
			//grooving ó�� ���� ������ ���
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
			// ���� 4.0 �߰� ��ų  grooving 2008/04/24
			//grooving ó�� ���� ������ ���
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

			// ��Ʈ �ɼ� ; ũ��Ƽ�� ������ ����
			lpObj->m_AttackDamageRight += lpObj->SetOpAddCriticalDamage;
			lpObj->m_AttackDamageLeft += lpObj->SetOpAddCriticalDamage;

			#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// ��ȭ�� ���� ��ȭ ������ �ɼ� : ũ��Ƽ�� ������ ����
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
			{	// ������Ʈ �������϶��� 20%�߰�
				lpObj->m_AttackDamageRight += (lpObj->m_AttackDamageMaxRight+SkillRightMaxDamage)*20/100;
				lpObj->m_AttackDamageLeft  += (lpObj->m_AttackDamageMaxLeft+SkillLeftMaxDamage)*20/100;

				// ��Ʈ �ɼ� ; ������Ʈ ������ ����
				lpObj->m_AttackDamageRight += lpObj->SetOpAddExDamage;
				lpObj->m_AttackDamageLeft += lpObj->SetOpAddExDamage;
			}
		}

#if TESTSERVER == 1
		LogAddC(LOGC_RED, "LEFT : %d, CDamage = %d, Effect = %d", lpObj->m_AttackDamageLeft, cDamage, effect);
#endif
	}	

	//�μտ� ���⸦ �������	
	if( bTwoHandWeapon )
	{
#ifdef DUEL_SYSTEM_20031028
// 			if (bIsOnDuel)
// 				ad = (lpObj->m_AttackDamageRight+lpObj->m_AttackDamageLeft) * 60 /100 -targetDefense;
// 			else
#endif
				ad = (lpObj->m_AttackDamageRight+lpObj->m_AttackDamageLeft)-targetDefense;			
			
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
			// ��տ� ���⸦ ����� ��� ũ��Ƽ�� ������ ���� �ɼ��� 2��� ����Ǵ� ������ ����
			ad -= lpObj->SetOpAddCriticalDamage;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	}
	//Ȱ�϶�
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
			|| Left->m_Type == ITEM_BOW + 24	// ��� ����
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
	//�����϶�
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
	//�Ϲݹ���
	else if( Right->m_Type >= 0 && Right->m_Type < ITEM_BOW )
	{
#ifdef DUEL_SYSTEM_20031028
// 		if (bIsOnDuel)
// 			ad = (lpObj->m_AttackDamageRight) * 60 /100 - targetDefense;
// 		else
#endif
			ad = (lpObj->m_AttackDamageRight)-targetDefense;
	}
	//������
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
	
	// ��Ʈ�ɼ� �μհ� ���� ������ ����
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
	// ���ݷ� ����&����
	if( lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttackTime > 0 )
	{
#if TESTSERVER == 1
		MsgOutput(lpObj->m_Index, "%d ����&������", lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack);
#endif
		
		ad += lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

		if( ad < 0 ) ad = 0;
	}
#endif

#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111	// ��ũ�Ͻ�(���ݷ� �϶�)
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
	{	// �����̾��� ��� �߰� ������ ����� �־�� �ȴ�
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

		// ��Ʈ�ɼ� : ��ų ���ݷ� ����
		damagemin += lpObj->SetOpAddSkillAttack;
		damagemax += lpObj->SetOpAddSkillAttack;

		int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
		if( SkillAttr != -1 )
		{
#ifdef UPDATE_GM_FUNCTION_20070228	// GM ĳ���� ���Ź��� ���� �� Resistance���� MAX��
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
			// ��ȭ�Ǻ��� ��ȭ ������ �ɼ� : ��ų ���ݷ� ����
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

		// ��Ʈ�ɼ� : ��ų ���ݷ� ����
		damagemin += lpObj->SetOpAddSkillAttack;
		damagemax += lpObj->SetOpAddSkillAttack;

		int SkillAttr = MagicDamageC.GetSkillAttr(lpMagic->m_Skill);
		if( SkillAttr != -1 )
		{
#ifdef UPDATE_GM_FUNCTION_20070228	// GM ĳ���� ���Ź��� ���� �� Resistance���� MAX��
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
		// ��ȭ�Ǻ��� ��ȭ ������ �ɼ� : ��ų ���ݷ� ����
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
		  ( Right->m_Type >= MAKE_ITEMNUM(5,0) && Right->m_Type < MAKE_ITEMNUM(5,MAX_ITEM_INDEX) ) // �����̷�
		  || Right->m_Type == MAKE_ITEMNUM(0,31)  // ���� ��ε�
	
	#ifdef ADD_CRITICAL_DAMAGE_OF_DEATH_BLADE_20050419
		  || Right->m_Type == MAKE_ITEMNUM(0,21)  // ���� ���̵�
	#endif // ADD_CRITICAL_DAMAGE_OF_DEATH_BLADE_20050419
		  
	#ifdef ADD_NEW_ITEM_01_20050823
		  || Right->m_Type == MAKE_ITEMNUM(0,23)  // �ͽ��÷��� ���̵�
	#endif
		  
	#ifdef MODIFY_350ITEM_BUGFIX2_20060823
		  || Right->m_Type == MAKE_ITEMNUM(0,25)  // 350������. �ҵ��
	#endif
		  
	#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
		  || Right->m_Type == MAKE_ITEMNUM( 0, 28 ) // ��ٽ�Ÿ��
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
			// �����̴� �������� ��귮���� ������ ���ҿ� ���� �г�Ƽ�� ����ȴ�
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
				//LogAdd("ũ��Ƽ�� : %d %d : %d",damagemax,targetDefense, ad);

				ad += lpObj->SetOpAddCriticalDamage;

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
				// ��ȭ�� ���� ��ȭ ������ �ɼ� : ũ��Ƽ�� ������ ����
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
				effect = DT_CRITICAL;			// ũ��Ƽ�� ������
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
				effect = DT_EXCELLENT;		// ������Ʈ ������
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
	{	// ����� ������ ����Ǹ� ���� +10
		ad += 10;
	}

	return ad;
}


// ����Ʈ�� Ÿ�� ������ ������ ���� 15% ������ ��� 15%
BOOL gObjDenorantSprite(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	// �������� ������ ����
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
// ��ũȣ���� Ÿ�� ������ ������ ��� (10+(���/100)
BOOL gObjDarkHorse(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	// �������� ������ ����
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
// �渱�� ���� ���� Ȯ��
BOOL gObjFenrir(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return FALSE;

	// �������� ������ ����
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

	// PVP�� ��
	int iReduceLife = 0;
	int iReduceShield = 0;
	int iReduceLifeForEffect = 0;
	bool bReduceShieldGage = false;

// CODEREVIEW_SHIELD :  SD �������� Life ���� ����
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// ��ȭ�� ���� ��ȭ ������ �ɼ� : SD������ ��� / SD ���� ��� / SD ���� Ȯ�� ���

	// 0. SD ���� �� �ʱ� ����
	int iDamageDevideToSDRate = g_iDamageDevideToSDRate;

	// 1. SD ������ ��� �ɼ� : ������ SD������ ���ߴ� ����.
	iDamageDevideToSDRate -= lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate;

	// 2. SD ���� ��� �ɼ� : SD�� ������ ����. �ڽſ��� �ش�.
	iDamageDevideToSDRate += lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate;
	
	// 3. ���� Ȯ��
	if ( iDamageDevideToSDRate < 0 ) 
		iDamageDevideToSDRate = 0;
	if ( iDamageDevideToSDRate > 100 ) 
		iDamageDevideToSDRate = 100;
	
	// 4. SD���� Ȯ��
	if ( lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate > 0 )
	{
		int iRand = rand()%100;
		if ( iRand < lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
		{
			iDamageDevideToSDRate = 0;
		}
	}

	// 5. �α�
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

	// 6. ���� ���
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
// CODEREVIEW_SHIELD :  #if ��� ��� �ڵ� ����
	if( bReduceShieldGage == true 
// CODEREVIEW_SHIELD :  lpTargetObj->m_SkillAddLife  ���� HP ���� �ƴ�
//					 :  lpTargetObj->MaxLife + lpTargetObj->AddLife �� ��ȿ
//					 :  ���ǹ��� ��� ���� ���� ���� 
		&& ( iReduceLife > iReduceLifeForEffect )
		)
	{
	#ifdef MODIFY_NO_SHOW_SHIELDBROKE_EFFECT_IN_CHAOSCASTLE_20060523
		if( CHECK_CHAOSCASTLE(lpTargetObj->MapNumber) == FALSE )
	#endif
		{
			// �� �ı� ����Ʈ
			GCSendEffectInfo( lpTargetObj->m_Index, EFFECT_SHIELDBROKE );
		}
	}

	return iReduceShield;
}
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ��ȯ���� ���� ���� ������ ���
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
	// ���� 4.0 �߰� ��ų  grooving 2008/04/24
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
	// ȯ����ų�� ������� ���ַ��� ������ �޴´�.
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
			// ���� 4.0 �߰� ��ų  grooving 2008/04/24
			// grooving ���� ���ַ�
			// ���ַ¿� ������ �޴� ��ų
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
				( Left->m_Type >= MAKE_ITEMNUM(5,0) && Left->m_Type < MAKE_ITEMNUM(5,MAX_ITEM_INDEX) ) // �����̷�
				)
			{
				
				if( Left->m_IsValidItem )
				{
					int damage = (Left->m_Curse/2)+(Left->m_Level*2);
					// �����̴� �������� ��귮���� ������ ���ҿ� ���� �г�Ƽ�� ����ȴ�
					damage -= (WORD)(damage*Left->m_CurrentDurabilityState);		
					
					damagemin += damagemin * damage / 100;
					damagemax += damagemax * damage / 100;
				}
			}
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
			// ���ַ¿� ������ �޴� ��ų
			iIncreaseDamage += gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_CURSED_DAMAGE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		}
		break;
	default:
		{
#ifdef SEASON4_PATCH_GROOVING
			//------>
			// ���� 4.0 �߰� ��ų  grooving 2008/04/24
			// grooving ���� ���ַ�
			// ���¿� ������ �޴� ��ų
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
				( Right->m_Type >= MAKE_ITEMNUM(5,0) && Right->m_Type < MAKE_ITEMNUM(5,MAX_ITEM_INDEX) ) // �����̷�
				)
			{
				
				if( Right->m_IsValidItem )
				{
					int damage = (Right->m_Magic/2)+(Right->m_Level*2);
					// �����̴� �������� ��귮���� ������ ���ҿ� ���� �г�Ƽ�� ����ȴ�
					damage -= (WORD)(damage*Right->m_CurrentDurabilityState);		
					
					damagemin += damagemin * damage / 100;
					damagemax += damagemax * damage / 100;
				}
			}
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
			// ���¿� ������ �޴� ��ų
			iIncreaseDamage += gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_MAGICDAMAGE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		}
		break;
	}
	
	// ��Ʈ�ɼ� : ��ų ���ݷ� ����
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
	
	// ��ȭ�Ǻ��� ��ȭ ������ �ɼ� : ��ų ���ݷ� ����
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
				//LogAdd("ũ��Ƽ�� : %d %d : %d",damagemax,targetDefense, ad);

				ad += lpObj->SetOpAddCriticalDamage;

				// ��ȭ�� ���� ��ȭ ������ �ɼ� : ũ��Ƽ�� ������ ����
				ad += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;

				int iValue = 0;
				gObjGetValueOfBuffIndex( lpObj, BUFFTYPE_CRITICAL_DMG_INC, &iValue );
				ad += iValue;				
				

				effect = DT_CRITICAL;			// ũ��Ƽ�� ������
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

				effect = DT_EXCELLENT;		// ������Ʈ ������
			}
		}
	}
	__except( subd=1, EXCEPTION_EXECUTE_HANDLER )
	{
	}

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	// �⺻ ������ ���� ȿ��
	iIncreaseDamage += gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_IMPROVE_DAMAGE );
	ad += iIncreaseDamage;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211

	if( lpObj->m_SkillAttack2 )
	{	// ����� ������ ����Ǹ� ���� +10
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
