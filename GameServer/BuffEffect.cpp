// BuffEffect.cpp: implementation of the CBuffEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuffEffect.h"

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuffEffect g_BuffEffect;

CBuffEffect::CBuffEffect()
{

}

CBuffEffect::~CBuffEffect()
{

}

void CBuffEffect::SetBuffEffect(LPOBJECTSTRUCT lpObj, BYTE EffectType, int EffectValue)
{
	if( lpObj == NULL || EffectType < 0 )
	{
		return;
	}

	if( lpObj->Connected < CSS_GAMEPLAYING )
	{
		return;
	}

	switch( EffectType )
	{
	case EFFECTTYPE_NONE:
		break;
	case EFFECTTYPE_ATTACKSPEED:
		lpObj->m_AttackSpeed			+= EffectValue;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
		lpObj->m_MagicSpeed				+= EffectValue;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
		break;
	case EFFECTTYPE_IMPROVE_DAMAGE:
#ifndef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211					// !! NOT
		lpObj->m_AttackDamageLeft		+= EffectValue;
		lpObj->m_AttackDamageRight		+= EffectValue;
		lpObj->m_AttackDamageMin		+= EffectValue;
		lpObj->m_AttackDamageMax		+= EffectValue;
		lpObj->m_AttackDamageMinLeft	+= EffectValue;
		lpObj->m_AttackDamageMaxLeft	+= EffectValue;
		lpObj->m_AttackDamageMinRight	+= EffectValue;
		lpObj->m_AttackDamageMaxRight	+= EffectValue;
		lpObj->m_MagicDamageMin			+= EffectValue;
		lpObj->m_MagicDamageMax			+= EffectValue;
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 공방에 저주력추가
		lpObj->m_CurseDamgeMin			+= EffectValue;
		lpObj->m_CurseDamgeMax			+= EffectValue;
#endif		
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		break;
	case EFFECTTYPE_IMPROVE_DEFENSE:
		lpObj->m_Defense				+= EffectValue;
		lpObj->m_MagicDefense			+= EffectValue;
		break;
	case EFFECTTYPE_HP:
		lpObj->AddLife					+= EffectValue;
		GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield + lpObj->iAddShield) );
		GCReFillSend( lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield) );
		break;
	case EFFECTTYPE_MANA:
		lpObj->AddMana					+= EffectValue;
		GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP + lpObj->AddBP) );
		GCManaSend( lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, (WORD)(lpObj->BP) );
		break;
#ifdef ADD_PCS_MARK_OF_EXP_20070205
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	case EFFECTTYPE_EXPERIENCE:
		lpObj->m_wExprienceRate			= EffectValue;
		lpObj->m_btMoveMapBound			= 1;
		break;
	case EFFECTTYPE_ITEMDROPRATE:
		lpObj->m_wItemDropRate			= EffectValue;
		lpObj->m_btMoveMapBound			= 1;
		break;
	case EFFECTTYPE_EXPERIENCE_ZERO:
		lpObj->m_wExprienceRate			= 0;
		lpObj->m_btMoveMapBound			= 1;
		break;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // ADD_PCS_MARK_OF_EXP_20070205

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	case EFFECTTYPE_STRENGTH:
		lpObj->AddStrength				+= EffectValue;
		break;
	case EFFECTTYPE_DEXTERITY:
		lpObj->AddDexterity				+= EffectValue;
		break;
	case EFFECTTYPE_VITALITY:
		lpObj->AddVitality				+= EffectValue;
		break;
	case EFFECTTYPE_ENERGY:
		lpObj->AddEnergy				+= EffectValue;
		break;
	case EFFECTTYPE_LEADERSHIP:
		lpObj->AddLeadership			+= EffectValue;
		break;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
		
	case EFFECTTYPE_MELEEDAMAGE:
#ifndef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211					// !! NOT
		lpObj->m_AttackDamageLeft		+= EffectValue;
		lpObj->m_AttackDamageRight		+= EffectValue;
		lpObj->m_AttackDamageMin		+= EffectValue;
		lpObj->m_AttackDamageMax		+= EffectValue;
		lpObj->m_AttackDamageMinLeft	+= EffectValue;
		lpObj->m_AttackDamageMaxLeft	+= EffectValue;
		lpObj->m_AttackDamageMinRight	+= EffectValue;
		lpObj->m_AttackDamageMaxRight	+= EffectValue;
	#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 공방에 저주력추가
		lpObj->m_CurseDamgeMin			+= EffectValue;
		lpObj->m_CurseDamgeMax			+= EffectValue;
	#endif
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		break;
	case EFFECTTYPE_MAGICDAMAGE:
#ifndef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211					// !! NOT
		lpObj->m_MagicDamageMin			+= EffectValue;
		lpObj->m_MagicDamageMax			+= EffectValue;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		break;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// 소울바리어 버프시스템 수정
	case EFFECTTYPE_IMPROVE_MELEE_DEFENSE:
		lpObj->m_nSoulBarrierDefence	= EffectValue;
		break;
#else
	case EFFECTTYPE_IMPROVE_MELEE_DEFENSE:
		lpObj->m_Defense				+= EffectValue;
		break;
#endif
	case EFFECTTYPE_IMPROVE_MAGIC_DEFENSE:
		lpObj->m_MagicDefense			+= EffectValue;
		break;
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	case EFFECTTYPE_DAMAGEREFLECT:
		lpObj->DamageReflect			+= EffectValue;
		break;
	case EFFECTTYPE_SLEEP:
		break;
	case EFFECTTYPE_BLIND:
		lpObj->m_AttackRating			-= EffectValue;
		break;
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912		
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	case EFFECTTYPE_CURSED_DAMAGE:
		break;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 소울바리어 마나소모량 버프시스템에 추가
	case EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA:
		lpObj->m_nSoulBarrierManaRate	= EffectValue;
		break;
#endif	
#ifdef ADD_CASHITEM_4RD_MARK_20080723
	case EFFECTTYPE_RECOVERY_HP:
		break;
	case EFFECTTYPE_RECOVERY_MANA:
		break;
#endif // ADD_CASHITEM_4RD_MARK_20080723
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
	case EFFECTTYPE_CRITICALDAMAGE:
		break;
	case EFFECTTYPE_EXCELLENTDAMAGE:
		break;
#endif // ADD_CASHITEM_4RD_SCROLL_20080723

#ifdef SEASON4_PATCH_GROOVING
	// 시즌 4.0 추가 스킬  grooving 2008/04/24 : 버서커
	case EFFECTTYPE_BERSERKER_UP:
		{			
			lpObj->AddMana += (int)(lpObj->MaxMana * EffectValue / 100);
			
			GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP + lpObj->AddBP) );
			GCManaSend( lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, (WORD)(lpObj->BP) );
		}
		break;
	case EFFECTTYPE_BERSERKER_DOWN:
		{
			float fAddLife = 0;
			float fPer = (40.0f - EffectValue) / 100.0f;
			
			fPer = max(fPer, 0.1f);

			fAddLife =	lpObj->MaxLife * fPer;

			lpObj->AddLife -= (int)fAddLife;

			lpObj->Life = min( lpObj->Life, (lpObj->MaxLife + lpObj->AddLife) );
			
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield + lpObj->iAddShield) );
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield) );
		}		
		break;
#endif
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 마력증가
	case EFFECTTYPE_MAGICPOWER_INC:
		lpObj->m_MagicDamageMin			+= EffectValue;
		lpObj->m_MagicDamageMax			+= EffectValue;
		break;
#endif	// ADD_SEASON4_NEW_SKILL_20080522
		//------------->
		//20081013 해외 크리스마스 이벤트 - 버프(grooving)
#ifdef ADD_BUFF_XMASEVENT_20080930
	case EFFECTTYPE_POWER_UP:
		{
			lpObj->m_AttackDamageMaxLeft	+= EffectValue;
			lpObj->m_AttackDamageMinLeft	+= EffectValue;
			lpObj->m_AttackDamageMaxRight	+= EffectValue;
			lpObj->m_AttackDamageMinRight	+= EffectValue;
#ifdef MDOFIY_BUFF_POWER_UP
			lpObj->m_MagicDamageMin			+= EffectValue;
			lpObj->m_MagicDamageMax			+= EffectValue;
	#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 공방에 저주력추가
			lpObj->m_CurseDamgeMin			+= EffectValue;
			lpObj->m_CurseDamgeMax			+= EffectValue;
	#endif
#endif // MDOFIY_BUFF_POWER_UP
		}
		break;
	case EFFECTTYPE_GUARD_UP:
		{
			lpObj->m_Defense				+= EffectValue;
		}
		break;
		
#ifdef ADD_AURA_CASH_ITEM_20090915
	case EFFECTTYPE_MAX_AG_INCREASE:
		{
			lpObj->AddBP					+= (lpObj->Level+lpObj->m_nMasterLevel) * EffectValue;
			
			GCManaSend( lpObj->m_Index, (WORD)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP + lpObj->AddBP) );
			GCManaSend( lpObj->m_Index, (WORD)(lpObj->Mana), 0xFF, 0, (WORD)(lpObj->BP) );			
		}
		break;
	case EFFECTTYPE_MAX_SD_INCREASE:
		{
			lpObj->iAddShield				+= (lpObj->Level+lpObj->m_nMasterLevel) * EffectValue;
			
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield + lpObj->iAddShield) );
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield) );
		}
		break;
#endif // ADD_AURA_CASH_ITEM_20090915

/* 삭제예정
	case EFFECTTYPE_MAXLIFE_UP:
		{
			lpObj->MaxLife					+= EffectValue;
			
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield + lpObj->iAddShield) );
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield) );
		}
		break;
	case EFFECTTYPE_MAXMANA_UP:
		{
			lpObj->MaxMana					+= EffectValue;
			
			GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP + lpObj->AddBP) );
			GCManaSend( lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, (WORD)(lpObj->BP) );
		}
		break;
*/

	case EFFECTTYPE_AG_RECOVERY_UP:
		{
			
		}
		break;
#endif //ADD_BUFF_XMASEVENT_20080930
//<-------------

	}
		

	
}

void CBuffEffect::ClearBuffEffect(LPOBJECTSTRUCT lpObj, BYTE EffectType, int EffectValue)
{
	if( lpObj == NULL || EffectType < 0 )
	{
		return;
	}

	if( lpObj->Connected < CSS_GAMEPLAYING )
	{
		return;
	}

	switch( EffectType )
	{
	case EFFECTTYPE_NONE:
		break;
	case EFFECTTYPE_ATTACKSPEED:
		lpObj->m_AttackSpeed			-= EffectValue;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
		lpObj->m_MagicSpeed				-= EffectValue;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
		break;
	case EFFECTTYPE_IMPROVE_DAMAGE:
#ifndef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211					// !! NOT
		lpObj->m_AttackDamageLeft		-= EffectValue;
		lpObj->m_AttackDamageRight		-= EffectValue;
		lpObj->m_AttackDamageMin		-= EffectValue;
		lpObj->m_AttackDamageMax		-= EffectValue;
		lpObj->m_AttackDamageMinLeft	-= EffectValue;
		lpObj->m_AttackDamageMaxLeft	-= EffectValue;
		lpObj->m_AttackDamageMinRight	-= EffectValue;
		lpObj->m_AttackDamageMaxRight	-= EffectValue;
		lpObj->m_MagicDamageMin			-= EffectValue;
		lpObj->m_MagicDamageMax			-= EffectValue;
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 공방에 저주력추가
		lpObj->m_CurseDamgeMin			-= EffectValue;
		lpObj->m_CurseDamgeMax			-= EffectValue;
#endif
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		break;
	case EFFECTTYPE_IMPROVE_DEFENSE:
		lpObj->m_Defense				-= EffectValue;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_06_20080102
		lpObj->m_MagicDefense			-= EffectValue;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_06_20080102
		break;
	case EFFECTTYPE_HP:
		lpObj->AddLife					-= EffectValue;
		GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield + lpObj->iAddShield) );
		GCReFillSend( lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield) );
		break;
	case EFFECTTYPE_MANA:
		lpObj->AddMana					-= EffectValue;
		GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP + lpObj->AddBP) );
		GCManaSend( lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, (WORD)(lpObj->BP) );
		break;
#ifdef ADD_PCS_MARK_OF_EXP_20070205
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	case EFFECTTYPE_EXPERIENCE:
		lpObj->m_wExprienceRate			= 100;
		lpObj->m_btMoveMapBound			= 0;
		break;
	case EFFECTTYPE_ITEMDROPRATE:
		lpObj->m_wItemDropRate			= 100;
		lpObj->m_btMoveMapBound			= 0;
		break;
	case EFFECTTYPE_EXPERIENCE_ZERO:
		lpObj->m_wExprienceRate			= 100;
		lpObj->m_btMoveMapBound			= 0;
		break;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // ADD_PCS_MARK_OF_EXP_20070205

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	case EFFECTTYPE_STRENGTH:
		lpObj->AddStrength				-= EffectValue;
		break;
	case EFFECTTYPE_DEXTERITY:
		lpObj->AddDexterity				-= EffectValue;
		break;
	case EFFECTTYPE_VITALITY:
		lpObj->AddVitality				-= EffectValue;
		break;
	case EFFECTTYPE_ENERGY:
		lpObj->AddEnergy				-= EffectValue;
		break;
	case EFFECTTYPE_LEADERSHIP:
		lpObj->AddLeadership			-= EffectValue;
		break;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

	case EFFECTTYPE_MELEEDAMAGE:
#ifndef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211					// !! NOT
		lpObj->m_AttackDamageLeft		-= EffectValue;
		lpObj->m_AttackDamageRight		-= EffectValue;
		lpObj->m_AttackDamageMin		-= EffectValue;
		lpObj->m_AttackDamageMax		-= EffectValue;
		lpObj->m_AttackDamageMinLeft	-= EffectValue;
		lpObj->m_AttackDamageMaxLeft	-= EffectValue;
		lpObj->m_AttackDamageMinRight	-= EffectValue;
		lpObj->m_AttackDamageMaxRight	-= EffectValue;
	#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 공방에 저주력추가
		lpObj->m_CurseDamgeMin			-= EffectValue;
		lpObj->m_CurseDamgeMax			-= EffectValue;
	#endif // ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		break;
	case EFFECTTYPE_MAGICDAMAGE:
#ifndef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211					// !! NOT
		lpObj->m_MagicDamageMin			-= EffectValue;
		lpObj->m_MagicDamageMax			-= EffectValue;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		break;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// 소울바리어 버프시스템 수정
	case EFFECTTYPE_IMPROVE_MELEE_DEFENSE:
		lpObj->m_nSoulBarrierDefence	-= EffectValue;
		break;
#else
	case EFFECTTYPE_IMPROVE_MELEE_DEFENSE:
		lpObj->m_Defense				-= EffectValue;
		break;
#endif
	case EFFECTTYPE_IMPROVE_MAGIC_DEFENSE:
		lpObj->m_MagicDefense			-= EffectValue;
		break;
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	case EFFECTTYPE_DAMAGEREFLECT:
		lpObj->DamageReflect			-= EffectValue;
		break;
	case EFFECTTYPE_SLEEP:
		break;
	case EFFECTTYPE_BLIND:
		lpObj->m_AttackRating			+= EffectValue;
		break;
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	case EFFECTTYPE_CURSED_DAMAGE:
		break;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	case EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA:
		lpObj->m_nSoulBarrierManaRate	= 0;
		break;
#endif
#ifdef ADD_CASHITEM_4RD_MARK_20080723
	case EFFECTTYPE_RECOVERY_HP:
		break;
	case EFFECTTYPE_RECOVERY_MANA:
		break;
#endif // ADD_CASHITEM_4RD_MARK_20080723
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
	case EFFECTTYPE_CRITICALDAMAGE:
		break;
	case EFFECTTYPE_EXCELLENTDAMAGE:
		break;
#endif // ADD_CASHITEM_4RD_SCROLL_20080723

#ifdef SEASON4_PATCH_GROOVING
		// 시즌 4.0 추가 스킬  grooving 2008/04/24 : 버서커
	case EFFECTTYPE_BERSERKER_UP:
		{
			lpObj->AddMana -= (int)(lpObj->MaxMana * EffectValue / 100);

			lpObj->Mana = min(lpObj->Mana, lpObj->MaxMana + lpObj->AddMana);

			GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP + lpObj->AddBP) );
			GCManaSend( lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, (WORD)(lpObj->BP) );
		}
		break;
	case EFFECTTYPE_BERSERKER_DOWN:
		{
			float fAddLife = 0;
			float fPer = (40.0f - EffectValue) / 100.0f;
			
			fPer = max(fPer, 0.1f);
			
			fAddLife =	lpObj->MaxLife * fPer;

			lpObj->AddLife += (int)fAddLife;
			
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield + lpObj->iAddShield) );
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield) );
		}
		break;	
#endif
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 마력증가
	case EFFECTTYPE_MAGICPOWER_INC:
		lpObj->m_MagicDamageMin			-= EffectValue;
		lpObj->m_MagicDamageMax			-= EffectValue;
		break;
#endif	// ADD_SEASON4_NEW_SKILL_20080522
//------------->
//20081013 해외 크리스마스 이벤트 - 버프(grooving)
#ifdef ADD_BUFF_XMASEVENT_20080930
	case EFFECTTYPE_POWER_UP:
		{
			lpObj->m_AttackDamageMaxLeft	-= EffectValue;
			lpObj->m_AttackDamageMinLeft	-= EffectValue;
			lpObj->m_AttackDamageMaxRight	-= EffectValue;
			lpObj->m_AttackDamageMinRight	-= EffectValue;
#ifdef MDOFIY_BUFF_POWER_UP
			lpObj->m_MagicDamageMin			-= EffectValue;
			lpObj->m_MagicDamageMax			-= EffectValue;
	#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 공방에 저주력추가
			lpObj->m_CurseDamgeMin			-= EffectValue;
			lpObj->m_CurseDamgeMax			-= EffectValue;
	#endif
#endif // MDOFIY_BUFF_POWER_UP
		}
		break;
	case EFFECTTYPE_GUARD_UP:
		{
			lpObj->m_Defense				-= EffectValue;
		}
		break;
#ifdef ADD_AURA_CASH_ITEM_20090915
	case EFFECTTYPE_MAX_AG_INCREASE:
		{
			lpObj->AddBP					-= (lpObj->Level+lpObj->m_nMasterLevel) * EffectValue;
			
			GCManaSend( lpObj->m_Index, (WORD)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP + lpObj->AddBP) );
			GCManaSend( lpObj->m_Index, (WORD)(lpObj->Mana), 0xFF, 0, (WORD)(lpObj->BP) );
		}
		break;
	case EFFECTTYPE_MAX_SD_INCREASE:
		{
			lpObj->iAddShield				-= (lpObj->Level+lpObj->m_nMasterLevel) * EffectValue;
			
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield + lpObj->iAddShield) );
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield) );
		}
		break;
#endif // ADD_AURA_CASH_ITEM_20090915

/* 삭제예정
	case EFFECTTYPE_MAXLIFE_UP:
		{
			lpObj->MaxLife					-= EffectValue;
			
			if( lpObj->MaxLife + lpObj->AddLife < lpObj->Life)
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield + lpObj->iAddShield) );
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield) );
		}
		break;
	case EFFECTTYPE_MAXMANA_UP:
		{
			lpObj->MaxMana					-= EffectValue;

			if( lpObj->MaxMana + lpObj->AddMana < lpObj->Mana)
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			
			GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana + lpObj->AddMana), 0xFE, 0, (WORD)(lpObj->MaxBP + lpObj->AddBP) );
			GCManaSend( lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, (WORD)(lpObj->BP) );
		}
		break;
*/
	case EFFECTTYPE_AG_RECOVERY_UP:
		{
			
		}
		break;
#endif //ADD_BUFF_XMASEVENT_20080930
//<-------------
	}
}

void CBuffEffect::SetActiveBuffEffect(LPOBJECTSTRUCT lpObj, BYTE EffectType, int EffectValue)
{
	if( EffectType <= 0 )
	{
		return;
	}

	switch( EffectType )
	{
	// 틱당 데미지가 들어온다.
	case EFFECTTYPE_GIVE_DMG_TICK:
		GiveDamageEffect( lpObj, EffectValue );
		break;
	case EFFECTTYPE_POISON_DMG_TICK:
		PoisonEffect( lpObj, EffectValue );
		break;
	}
}

void CBuffEffect::GiveDamageEffect(LPOBJECTSTRUCT lpObj, int Damage)
{
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	int DecreaseHealthPoint = 0;
	int DecreaseShiledPoint = 0;

	DecreaseHealthPoint = (int)Damage;
	DecreaseShiledPoint = DecreaseHealthPoint;

	if( lpObj->Live == 0)
	{
		return;
	}
	
	if( lpObj->lpAttackObj != NULL &&
		lpObj->Type == OBJTYPE_CHARACTER && lpObj->lpAttackObj->Type == OBJTYPE_CHARACTER )
	{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_04_20071216
		DecreaseShiledPoint = (int)( ( DecreaseHealthPoint * 90 ) / 100 );
		DecreaseHealthPoint = DecreaseHealthPoint - DecreaseShiledPoint;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_04_20071216
		// 쉴드 감소
		if( lpObj->iShield - DecreaseShiledPoint > 0 )
		{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_04_20071216
			lpObj->iShield -= DecreaseShiledPoint;
			lpObj->Life -= DecreaseHealthPoint;
#else
			lpObj->iShield -= DecreaseShiledPoint;
			DecreaseHealthPoint = 0;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_04_20071216
		}
		else
		{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_04_20071216
			DecreaseHealthPoint += DecreaseShiledPoint - lpObj->iShield;
			DecreaseShiledPoint = lpObj->iShield;
			lpObj->Life -= DecreaseHealthPoint;
			lpObj->iShield = 0;
#else
			DecreaseHealthPoint = DecreaseShiledPoint - lpObj->iShield;
			DecreaseShiledPoint = lpObj->iShield;
			lpObj->Life -= DecreaseHealthPoint;
			lpObj->iShield = 0;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_04_20071216
		}
	}
	else
	{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_04_20071216
		DecreaseShiledPoint = 0;

		lpObj->Life -= DecreaseHealthPoint;
#else
		DecreaseShiledPoint	= 0;
		// HP 감소
		lpObj->Life -= DecreaseHealthPoint;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_04_20071216
	}

	if( lpObj->Life < 0 )
	{
		lpObj->Life = 0;
	}

	if( lpObj->lpAttackObj != NULL )
	{
		gObjLifeCheck(lpObj, lpObj->lpAttackObj, DecreaseHealthPoint, 3, 0, 0, 0, DecreaseShiledPoint );
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
}


void CBuffEffect::PoisonEffect(LPOBJECTSTRUCT lpObj, BYTE PoisonRate)
{
	int DecreaseHealthPoint = 0;
	int DecreaseShiledPoint = 0;

	DecreaseHealthPoint = (int)( ( lpObj->Life * PoisonRate ) / 100 );
	DecreaseShiledPoint = DecreaseHealthPoint;

	if( lpObj->lpAttackObj != NULL &&
		lpObj->Type == OBJTYPE_CHARACTER && lpObj->lpAttackObj->Type == OBJTYPE_CHARACTER )
	{
		// 쉴드 감소
		if( lpObj->iShield - DecreaseShiledPoint > 0 )
		{
			lpObj->iShield -= DecreaseShiledPoint;
			DecreaseHealthPoint = 0;
		}
		else
		{
			DecreaseHealthPoint = DecreaseShiledPoint - lpObj->iShield;
			DecreaseShiledPoint = lpObj->iShield;
			lpObj->Life -= DecreaseHealthPoint;
			lpObj->iShield = 0;
		}
	}
	else
	{
		// HP 감소
		lpObj->Life -= DecreaseHealthPoint;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		DecreaseShiledPoint = 0;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
	}

	if( lpObj->Life < 0 )
	{
		lpObj->Life = 0;
	}

	if( lpObj->lpAttackObj != NULL )
	{
		gObjLifeCheck(lpObj, lpObj->lpAttackObj, DecreaseHealthPoint, 2, 0, 0, AT_SKILL_POISON, DecreaseShiledPoint );
	}
}

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
void CBuffEffect::SetPrevEffect(LPOBJECTSTRUCT lpObj)
{
	if( lpObj == NULL )
	{
		return;
	}

	int BuffCount = 0;

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
		if( lpObj->m_BuffEffectList[i].BuffIndex == BUFFTYPE_NONE )
		{
			continue;
		}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204

#ifndef MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
		if( lpObj->m_BuffEffectCount <= BuffCount )
		{
			return;
		}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_07_20080109

		switch( lpObj->m_BuffEffectList[i].EffectType1 )
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		case EFFECTTYPE_DAMAGEREFLECT:
#endif // ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
#ifdef ADD_AURA_CASH_ITEM_20090915
		case EFFECTTYPE_MAX_AG_INCREASE:
		case EFFECTTYPE_MAX_SD_INCREASE:
#endif // ADD_AURA_CASH_ITEM_20090915
			BuffCount++;
			SetBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1 );
			break;
		}

		switch( lpObj->m_BuffEffectList[i].EffectType2 )
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		case EFFECTTYPE_DAMAGEREFLECT:
#endif // ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
#ifdef ADD_AURA_CASH_ITEM_20090915
		case EFFECTTYPE_MAX_AG_INCREASE:
		case EFFECTTYPE_MAX_SD_INCREASE:
#endif // ADD_AURA_CASH_ITEM_20090915
			BuffCount++;
			SetBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2 );
			break;
		}
	}
}

void CBuffEffect::SetNextEffect(LPOBJECTSTRUCT lpObj)
{
	if( lpObj == NULL )
	{
		return;
	}

	int BuffCount = 0;

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
		if( lpObj->m_BuffEffectList[i].BuffIndex == BUFFTYPE_NONE )
		{
			continue;
		}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204

#ifndef MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
		if( lpObj->m_BuffEffectCount <= BuffCount )
		{
			return;
		}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_07_20080109

		switch( lpObj->m_BuffEffectList[i].EffectType1 )
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
#ifdef SEASON4_PATCH_GROOVING
		//case EFFECTTYPE_BERSERKER_UP:
		//case EFFECTTYPE_BERSERKER_DOWN:
#endif // SEASON4_PATCH_GROOVING
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		case EFFECTTYPE_DAMAGEREFLECT:
#endif // ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
#ifdef ADD_AURA_CASH_ITEM_20090915
		case EFFECTTYPE_MAX_AG_INCREASE:
		case EFFECTTYPE_MAX_SD_INCREASE:
#endif // ADD_AURA_CASH_ITEM_20090915
			break;
		default:
			BuffCount++;
			SetBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1 );
			break;
		}

		switch( lpObj->m_BuffEffectList[i].EffectType2 )
		{
		case EFFECTTYPE_HP:
		case EFFECTTYPE_MANA:
		case EFFECTTYPE_STRENGTH:
		case EFFECTTYPE_DEXTERITY:
		case EFFECTTYPE_VITALITY:
		case EFFECTTYPE_ENERGY:
		case EFFECTTYPE_LEADERSHIP:
#ifdef SEASON4_PATCH_GROOVING
		//case EFFECTTYPE_BERSERKER_UP:
		//case EFFECTTYPE_BERSERKER_DOWN:
#endif // SEASON4_PATCH_GROOVING
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		case EFFECTTYPE_DAMAGEREFLECT:
#endif // ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
#ifdef ADD_AURA_CASH_ITEM_20090915
		case EFFECTTYPE_MAX_AG_INCREASE:
		case EFFECTTYPE_MAX_SD_INCREASE:
#endif // ADD_AURA_CASH_ITEM_20090915
			break;
		default:
			BuffCount++;
			SetBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2 );
			break;
		}
	}
}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128

#ifdef MODIFY_INCREASE_LIFE_AND_SETITEM_BUGFIX_20080829
void CBuffEffect::ClearAllBuffEffect( LPOBJECTSTRUCT lpObj )
{
	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( lpObj->m_BuffEffectList[i].BuffIndex == BUFFTYPE_NONE )
		{
			continue;
		}
		
		ClearBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1 );
		ClearBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2 );
	}
}
#endif // MODIFY_INCREASE_LIFE_AND_SETITEM_BUGFIX_20080829

#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917