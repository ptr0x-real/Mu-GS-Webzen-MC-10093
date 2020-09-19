// ObjCalCharacter.cpp: implementation of the CObjCalCharacter class.
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
#include "CalCharacter.h"

#include "User.h"
#include "protocol.h"
#include "Gamemain.h"

#include "ObjCalCharacter.h"

#ifdef DARKLORD_WORK
#include "DarkSpirit.h"
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#include "JewelOfHarmonySystem.h"
#endif

#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
#include "ItemAddOption.h"
#endif

#ifdef MODIFY_SETITEM_BUGFIX_20080731

#define MAX_OPTIONTABLE			5
#define MAX_SETOPTION			10
#define MAX_FULL_SETOPTION		5

enum _ADD_ABILITY
{
	ADDABILITY_STRENGTH		= 0, 
		ADDABILITY_DEXTERITY, 
		ADDABILITY_ENERGY, 
		ADDABILITY_VITALITY, 
		ADDABILITY_END, 
};

typedef struct _tagSetItemOption
{
	int		nOptionTableIndex;
	int		nOptionTableCount;
	BYTE	bFullSetItem;
	struct {
		int	nOption1;
		int	nOption2;
		int nValue1;
		int nValue2;
	} option[MAX_SETOPTION];
	struct {
		int	nOption;
		int nValue;
	} fulloption[MAX_FULL_SETOPTION];
} MU_SETITEMOPTION, *LPMU_SETITEMOPTION;

typedef struct _tagSetItemInfo
{
	int					nCount;
	MU_SETITEMOPTION	table[MAX_OPTIONTABLE];
} MU_SETITEMINFO, *LPMU_SETITEMINFO;

short	GetItemAddAbility(LPOBJECTSTRUCT lpObj, CItem *lpItem, int nOptionType, LPMU_SETITEMINFO lpSetItemInfo);
void	GetSetItemInfo(LPOBJECTSTRUCT lpObj, LPMU_SETITEMINFO lpSetItemInfo);
#endif // MODIFY_SETITEM_BUGFIX_20080731

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "Common\SetItemOption.h"

extern classdef			DCInfo;
#if GAME_VERSION >= G_V_99B
void	gObjGetSetItemOption(LPOBJECTSTRUCT lpObj, LPBYTE pSetOptionTable, LPBYTE pSetOptionCountTable, int* pSetOptionCount);
void	gObjCalcSetItemOption(LPOBJECTSTRUCT lpObj);
void	gObjSetItemStatPlusSpecial(LPOBJECTSTRUCT lpObj, int option, int ivalue);
void	gObjSetItemPlusSpecial(LPOBJECTSTRUCT lpObj, int option, int ivalue);

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	void	gObjSetItemApply(LPOBJECTSTRUCT lpObj, int type = 0 );		// 0 : 값증가 옵션, 1 : %증가 옵션
#else
	void	gObjSetItemApply(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
#endif

#ifdef MODIFY_SETITEM_BUGFIX_20080731
BOOL	gObjValidItem(LPOBJECTSTRUCT lpObj, CItem *lpItem, int pos, LPMU_SETITEMINFO lpSetItemInfo);
#else
BOOL	gObjValidItem(LPOBJECTSTRUCT lpObj, CItem *lpItem, int pos);
#endif // MODIFY_SETITEM_BUGFIX_20080731

#ifdef ADD_SHIELD_POINT_01_20060403
extern int g_iConstNumberOfShieldPoint;
void	gObjCalcShieldPoint(LPOBJECTSTRUCT lpObj);
#endif

extern ITEM_ATTRIBUTE      ItemAttribute [];

//----------------------------------------------------------------------------
// 캐릭터의 능력치를 계산한다.
void gObjCalCharacter(int aIndex)
{
	LPOBJECTSTRUCT	lpObj;

	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	int	Strength = 0;
	int	Dexterity = 0;
	int	Vitality = 0;
	int	Energy = 0;	

	CItem	*Right = &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT];
	CItem	*Left  = &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT];
	CItem	*Gloves  = &lpObj->pInventory[EQUIPMENT_GLOVES];
	CItem	*Amulet = &lpObj->pInventory[EQUIPMENT_AMULET];
	CItem	*Helper = &lpObj->pInventory[EQUIPMENT_HELPER];

	lpObj->HaveWeaponInHand = TRUE;
	// 손에 무기를 들고 있는지 체크
	if( !Right->IsItem() && !Left->IsItem() )
	{	// 모두 안차고 있거나..
		lpObj->HaveWeaponInHand = FALSE;
	}
	else
	{
		if( !Left->IsItem() && Right->m_Type == MAKE_ITEMNUM(4,15) )
		{	// 화살이나 
			lpObj->HaveWeaponInHand = FALSE;
		}
		else if( !Right->IsItem() )
		{
			int itemtype = Left->m_Type/MAX_ITEM_INDEX;
			if( Left->m_Type == MAKE_ITEMNUM(4,7) )
			{				
				lpObj->HaveWeaponInHand = FALSE;
			} 
			else if( itemtype == 6 )
			{	// 방패류만 차고 있으면
				lpObj->HaveWeaponInHand = FALSE;
			}			
		}
	}
	
	lpObj->AddLife = 0;
	lpObj->AddMana = 0;
	lpObj->MonsterDieGetMoney = 0;
	lpObj->MonsterDieGetLife = 0;
	lpObj->MonsterDieGetMana = 0;
	lpObj->DamageReflect = 0;
	lpObj->DamageMinus = 0;

#ifndef MODIFY_SWELLLIFE_BUG_FIXAGIAN_20060628	// not define : 초기화 해주면 안된다.	
	#ifdef MODIFY_SWELLLIFE_INIT_BUGFIX_20060411
		lpObj->m_SkillAddLife = 0;
		lpObj->m_SkillAddLifeTime = 0;
	#endif
#endif // MODIFY_SWELLLIFE_BUG_FIXAGIAN_20060628
		
#ifdef DARKLORD_WORK
	lpObj->SkillLongSpearChange = FALSE;
#endif
	
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
	if( lpObj->m_iItemEffectValidTime > 0 )
	{
		g_ItemAddOption.PrevSetItemLastEffectForHallowin( lpObj );
	}
#endif 

	int iItemIndex;
	// 장착 가능한 아이템인가?
	for( iItemIndex = 0; iItemIndex < MAX_EQUIPMENT; iItemIndex++ )
	{
		if( lpObj->pInventory[iItemIndex].IsItem() )		
			lpObj->pInventory[iItemIndex].m_IsValidItem = TRUE;				
	}

	// 세트 아이템 옵션 추가 스탯 계산
	BOOL	bIsChangeItem;
	do
	{	
		lpObj->SetOpAddMaxAttackDamage	= 0;
		lpObj->SetOpAddMinAttackDamage	= 0;
		lpObj->SetOpAddDamage	= 0;	
		lpObj->SetOpIncAGValue	= 0;
		lpObj->SetOpAddCriticalDamageSuccessRate	= 0;
		lpObj->SetOpAddCriticalDamage	= 0;
		lpObj->SetOpAddExDamageSuccessRate	= 0;
		lpObj->SetOpAddExDamage	= 0;
		lpObj->SetOpAddSkillAttack	= 0;

		lpObj->AddStrength	= 0;
		lpObj->AddDexterity = 0;
		lpObj->AddVitality	= 0;
		lpObj->AddEnergy	= 0;
#ifdef MODIFY_SETITEM_LEADERSHIP_BUGFIX
		lpObj->AddLeadership = 0;
#endif // MODIFY_SETITEM_LEADERSHIP_BUGFIX
#ifdef NEW_FORSKYLAND2
		lpObj->AddBP = 0;
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
		lpObj->iAddShield = 0;
#endif // ADD_SHIELD_POINT_01_20060403

		lpObj->SetOpAddAttackDamage	= 0;
		lpObj->SetOpAddDefence		= 0;
		lpObj->SetOpAddMagicPower	= 0;
		lpObj->SetOpAddDefenceRate	= 0;

		lpObj->SetOpIgnoreDefense				= 0;
		lpObj->SetOpDoubleDamage				= 0;
		lpObj->SetOpTwoHandSwordImproveDamage	= 0;
		lpObj->SetOpImproveSuccessAttackRate	= 0;
		lpObj->SetOpReflectionDamage			= 0;
		lpObj->SetOpImproveSheldDefence			= 0;
		lpObj->SetOpDecreaseAG					= 0;
		lpObj->SetOpImproveItemDropRate			= 0;

		lpObj->IsFullSetItem = 0;

		memset(lpObj->m_AddResistance,	0, MAX_RESISTANCE);

		bIsChangeItem = FALSE;

// 스탯이 빠졌을 경우만 생각해준다!!!!!!
		
#ifndef MODIFY_BUFFITEM_MEDICINE_SETITEM_20080424
#if GAME_VERSION >= G_V_99B
		gObjCalcSetItemStat(lpObj);	
		gObjCalcSetItemOption(lpObj);
#endif
#endif // MODIFY_BUFFITEM_MEDICINE_SETITEM_20080424

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		g_ItemAddOption.PrevSetItemEffect( lpObj );
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		g_BuffEffect.SetPrevEffect( lpObj );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128

#ifdef MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709
		//해외 능력치를 올려주는 아이템 추가로 비약 및 셋트아이템의 추가 옵션을
		//먼저 계산하도록 변경 (riverstyx)
		gObjCalcSetItemStat(lpObj);
		gObjCalcSetItemOption(lpObj);
#endif // MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709

#ifdef MODIFY_SETITEM_BUGFIX_20080731
		//MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709 디파인과 같이 사용해야 함
		//비약 및 셋트 아이템의 추가 옵션을 먼저 계산할 경우 같이 사용해야 함 (riverstyx)
		
		//셋트 아이템의 정보를 가져온다.
		MU_SETITEMINFO	SetItemInfo = {0};
		GetSetItemInfo(lpObj, &SetItemInfo);
#endif // MODIFY_SETITEM_BUGFIX_20080731

		// 장착 가능한 아이템인가?
		for( iItemIndex = 0; iItemIndex < MAX_EQUIPMENT; iItemIndex++ )
		{
			if( lpObj->pInventory[iItemIndex].IsItem() && lpObj->pInventory[iItemIndex].m_IsValidItem )
			{
#ifdef MODIFY_SETITEM_BUGFIX_20080731
				if( gObjValidItem(lpObj, &lpObj->pInventory[iItemIndex], iItemIndex, &SetItemInfo) )
#else
				if( gObjValidItem(lpObj, &lpObj->pInventory[iItemIndex], iItemIndex) )
#endif // MODIFY_SETITEM_BUGFIX_20080731
				{
					lpObj->pInventory[iItemIndex].m_IsValidItem = TRUE;					
				}	
				else
				{
					lpObj->pInventory[iItemIndex].m_IsValidItem = FALSE;
#if TESTSERVER == 1
					LogAddC(LOGC_GREEN, "[%s][%s] 아이템 무효 스탯 안됨 [%s]", 
							 lpObj->AccountID, lpObj->Name, 
							 lpObj->pInventory[iItemIndex].GetName());

					char szText[255];
					wsprintf(szText, "아이템 무효 스탯 안됨 [%s]", lpObj->pInventory[iItemIndex].GetName());
					GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
					
#ifdef MODIFY_INCREASE_LIFE_AND_SETITEM_BUGFIX_20080829
					g_BuffEffect.ClearAllBuffEffect( lpObj );					
#endif // MODIFY_INCREASE_LIFE_AND_SETITEM_BUGFIX_20080829

					bIsChangeItem = TRUE;
				}
			}
		}



	}while(bIsChangeItem);

#ifndef MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709		// !! NOT
#ifdef MODIFY_BUFFITEM_MEDICINE_SETITEM_20080424
	//잘못된 위치에 추가된 코드로 삭제해야함 riverstyx
	gObjCalcSetItemStat(lpObj);	
	gObjCalcSetItemOption(lpObj);
#endif // MODIFY_BUFFITEM_MEDICINE_SETITEM_20080424
#endif // MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709

	Strength	= lpObj->Strength+lpObj->AddStrength;
	Dexterity	= lpObj->Dexterity+lpObj->AddDexterity;
	Vitality	= lpObj->Vitality+lpObj->AddVitality;
	Energy		= lpObj->Energy+lpObj->AddEnergy;

#ifdef MODIFY_SPEEDHACK_20080605
	if (g_bAbilityDebug == TRUE) {
		char szMsg[256];
		sprintf(szMsg, "Strength: %d,%d    Dexterity: %d,%d", lpObj->Strength, lpObj->AddStrength, lpObj->Dexterity, lpObj->AddDexterity);
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);		
		ZeroMemory(szMsg, 256);
		sprintf(szMsg, "Vitality: %d,%d    Energy: %d,%d", lpObj->Vitality, lpObj->AddVitality, lpObj->Energy, lpObj->AddEnergy);
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
#ifdef GMCOMMAND_EX_1ST_BUGFIX
		ZeroMemory(szMsg, 256);
		sprintf(szMsg, "Leadership: %d,%d", lpObj->Leadership, lpObj->AddLeadership);
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
#endif // GMCOMMAND_EX_1ST_BUGFIX
	}
#endif // MODIFY_SPEEDHACK_20080605	

	if( lpObj->Class == CLASS_ELF )
	{
		if((Right->m_Type >= ITEM_BOW+8 && Right->m_Type<ITEM_BOW+15) || 
		   (Left->m_Type  >= ITEM_BOW   && Left->m_Type <ITEM_BOW+7) || 
		   Right->m_Type  == ITEM_BOW+16 
#ifdef MODIFY_APPLYING_BOW_01_20050825
		   || Left->m_Type  == ITEM_BOW+20 
		   || Left->m_Type  == ITEM_BOW+21 
#endif
#ifdef MODIFY_ALBATROS_BOW_BUGFIX_20070105
		   || Left->m_Type == ITEM_BOW + 22
#endif		   
#ifdef MODIFY_DARKSTINGER_BUGFIX_20081119
		   || Left->m_Type == ITEM_BOW + 23
#endif // MODIFY_DARKSTINGER_BUGFIX_20081119
#ifdef ADD_AERINBOW_20090309
		   || Left->m_Type == ITEM_BOW + 24
#endif // ADD_AERINBOW_20090309
		   || Right->m_Type  == ITEM_BOW+18 
		   || Right->m_Type  == ITEM_BOW+19 
		   || Left->m_Type  == ITEM_BOW+17 )
		{	// 요정 활을 차고 있으면 민첩만 적용
			if( (Right->IsItem()&&!Right->m_IsValidItem) || (Left->IsItem()&&!Left->m_IsValidItem) )
			{	// 장착 가능한 아이템이 아니면 적용하지 않는다
				lpObj->m_AttackDamageMinRight = (Dexterity+Strength)/Balancer.AttackDamageMin_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMaxRight = (Dexterity+Strength)/Balancer.AttackDamageMax_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMinLeft  = (Dexterity+Strength)/Balancer.AttackDamageMin_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMaxLeft  = (Dexterity+Strength)/Balancer.AttackDamageMax_Bow_StrDexDiv_Elf;
			}
			else
			{
				lpObj->m_AttackDamageMinRight = Dexterity/Balancer.AttackDamageMin_NoBow_DexDiv_Elf+Strength/Balancer.AttackDamageMin_NoBow_StrDiv_Elf;
				lpObj->m_AttackDamageMaxRight = Dexterity/Balancer.AttackDamageMax_NoBow_DexDiv_Elf+Strength/Balancer.AttackDamageMax_NoBow_StrDiv_Elf;
				lpObj->m_AttackDamageMinLeft  = Dexterity/Balancer.AttackDamageMin_NoBow_DexDiv_Elf+Strength/Balancer.AttackDamageMin_NoBow_StrDiv_Elf;
				lpObj->m_AttackDamageMaxLeft  = Dexterity/Balancer.AttackDamageMax_NoBow_DexDiv_Elf+Strength/Balancer.AttackDamageMax_NoBow_StrDiv_Elf;
			}
		}
		else
		{
			lpObj->m_AttackDamageMinRight = (Dexterity+Strength)/Balancer.AttackDamageMin_DexStrDiv_Elf;
			lpObj->m_AttackDamageMaxRight = (Dexterity+Strength)/Balancer.AttackDamageMax_DexStrDiv_Elf;
			lpObj->m_AttackDamageMinLeft  = (Dexterity+Strength)/Balancer.AttackDamageMin_DexStrDiv_Elf;
			lpObj->m_AttackDamageMaxLeft  = (Dexterity+Strength)/Balancer.AttackDamageMax_DexStrDiv_Elf;
		}
	}
	else if( lpObj->Class == CLASS_KNIGHT )
	{
		lpObj->m_AttackDamageMinRight = Strength/Balancer.AttackDamageMin_StrDiv_DK;
		lpObj->m_AttackDamageMaxRight = Strength/Balancer.AttackDamageMax_StrDiv_DK;
		lpObj->m_AttackDamageMinLeft  = Strength/Balancer.AttackDamageMin_StrDiv_DK;
		lpObj->m_AttackDamageMaxLeft  = Strength/Balancer.AttackDamageMax_StrDiv_DK;	
	}
	else if( lpObj->Class == CLASS_MAGUMSA )
	{
		lpObj->m_AttackDamageMinRight = Strength/Balancer.AttackDamageMin_StrDiv_MG+Energy/Balancer.AttackDamageMin_EneDiv_MG;
		lpObj->m_AttackDamageMaxRight = Strength/Balancer.AttackDamageMax_StrDiv_MG+Energy/Balancer.AttackDamageMax_EneDiv_MG;
		lpObj->m_AttackDamageMinLeft  = Strength/Balancer.AttackDamageMin_StrDiv_MG+Energy/Balancer.AttackDamageMin_EneDiv_MG;
		lpObj->m_AttackDamageMaxLeft  = Strength/Balancer.AttackDamageMax_StrDiv_MG+Energy/Balancer.AttackDamageMax_EneDiv_MG;	
	}
#ifdef DARKLORD_WORK	
	else if( lpObj->Class == CLASS_DARKLORD )
		{
			lpObj->m_AttackDamageMinRight = Strength/Balancer.AttackDamageMin_StrDiv_DL+Energy/Balancer.AttackDamageMin_EneDiv_DL;
			lpObj->m_AttackDamageMaxRight = Strength/Balancer.AttackDamageMax_StrDiv_DL+Energy/Balancer.AttackDamageMax_EneDiv_DL;
			lpObj->m_AttackDamageMinLeft  = Strength/Balancer.AttackDamageMin_StrDiv_DL+Energy/Balancer.AttackDamageMin_EneDiv_DL;
			lpObj->m_AttackDamageMaxLeft  = Strength/Balancer.AttackDamageMax_StrDiv_DL+Energy/Balancer.AttackDamageMax_EneDiv_DL;
		}	
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 신규캐릭터 최소, 최대 공격력
	else if( lpObj->Class == CLASS_SUMMONER)
	{
		lpObj->m_AttackDamageMinRight = ( Strength + Dexterity ) / 7;
		lpObj->m_AttackDamageMaxRight = ( Strength + Dexterity ) / 4;
		lpObj->m_AttackDamageMinLeft  = ( Strength + Dexterity ) / 7;
		lpObj->m_AttackDamageMaxLeft  = ( Strength + Dexterity ) / 4;
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	
	else
	{
		lpObj->m_AttackDamageMinRight = Strength/Balancer.AttackDamageMin_StrDiv_DW;
		lpObj->m_AttackDamageMaxRight = Strength/Balancer.AttackDamageMax_StrDiv_DW;
		lpObj->m_AttackDamageMinLeft  = Strength/Balancer.AttackDamageMin_StrDiv_DW;
		lpObj->m_AttackDamageMaxLeft  = Strength/Balancer.AttackDamageMax_StrDiv_DW;
	}	

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 최대 공격력 상승, 최소 공격력 상승( 흑기사, 요정, 다크로드)
	if( lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_ELF || lpObj->Class == CLASS_DARKLORD)
	{
		// 최대 공격력 상승
		lpObj->m_AttackDamageMaxRight	+= lpObj->m_MPSkillOpt.iMpsMaxAttackDamage;
		lpObj->m_AttackDamageMaxLeft	+= lpObj->m_MPSkillOpt.iMpsMaxAttackDamage;

		// 최소 공격력 상승
		lpObj->m_AttackDamageMinRight	+= lpObj->m_MPSkillOpt.iMpsMinAttackDamage;
		lpObj->m_AttackDamageMinLeft	+= lpObj->m_MPSkillOpt.iMpsMinAttackDamage;
	}

	if(lpObj->Class == CLASS_MAGUMSA)
	{
		// 최대 공격력/마력 상승
		lpObj->m_AttackDamageMaxRight	+= lpObj->m_MPSkillOpt.iMpsMaxAttackMagicDamage;
		lpObj->m_AttackDamageMaxLeft	+= lpObj->m_MPSkillOpt.iMpsMaxAttackMagicDamage;

		// 최소 공격력/마력 상승
		lpObj->m_AttackDamageMinRight	+= lpObj->m_MPSkillOpt.iMpsMinAttackMagicDamage;
		lpObj->m_AttackDamageMinLeft	+= lpObj->m_MPSkillOpt.iMpsMinAttackMagicDamage;
	}
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
	
#if TESTSERVER == 1
	if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type >= MAKE_ITEMNUM(2,8) && 
		lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type <= MAKE_ITEMNUM(2,12)
#ifdef ADD_WEAPON_OF_ARCHANGEL_SCEPTER_20050706
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,13) )
#endif
#ifdef ADD_NEW_ITEM_01_20050823
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,14) )
#endif
#ifdef MODIFY_350ITEM_BUGFIX2_20060823
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,15) ) // 350아이템.샤이닝셉터
#endif
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,17) ) // 엡솔루트셉터
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
#ifdef ADD_STRIKER_SCEPTER_20090317
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,17) ) // 스트라이커셉터
#endif // ADD_STRIKER_SCEPTER_20090317

	  )
	{
		DebugOutput(lpObj->m_Index, "셉터추가 공격력 %d", lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Magic/2);
	}
#endif
	
	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&lpObj->m_AttackDamageMinRight,AT_IMPROVE_DAMAGE);
	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&lpObj->m_AttackDamageMaxRight,AT_IMPROVE_DAMAGE);

	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&lpObj->m_AttackDamageMinLeft,AT_IMPROVE_DAMAGE);
	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&lpObj->m_AttackDamageMaxLeft,AT_IMPROVE_DAMAGE);

#ifdef DARKLORD_WORK
	int	AddLeadership = 0;
	if( lpObj->pInventory[EQUIPMENT_WING].IsItem() && lpObj->pInventory[EQUIPMENT_WING].m_IsValidItem )
		AddLeadership += lpObj->pInventory[EQUIPMENT_WING].m_Leadership;
#endif

	/*if( Right->m_Type == MAKE_ITEMNUM(1,0) )
	{
	ITEM_ATTRIBUTE *p = &ItemAttribute[Right->m_Type];
	LogAdd("error-[%s][%d:%d [%d:%d:%d]",lpObj->Name, Right->m_DamageMin, Right->m_DamageMax, p->Level, p->DamageMin, p->DamageMax);
	}
	*/

	if( Right->m_Type != -1 )
	{
		if( Right->m_IsValidItem )
		{
			if (Right->m_Type >= ITEM_STAFF && Right->m_Type <= ITEM_STAFF + MAX_ITEM_INDEX)
			{
				lpObj->m_AttackDamageMinRight += Right->m_DamageMin/Balancer.RightDamageMinDW;
				lpObj->m_AttackDamageMaxRight += Right->m_DamageMax/Balancer.RightDamageMaxDW;
			}
			else
			{
				lpObj->m_AttackDamageMinRight += Right->m_DamageMin;
				lpObj->m_AttackDamageMaxRight += Right->m_DamageMax;
			}
		}
		
#ifdef DARKLORD_WORK
		if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_SkillChange )
		{
#if TESTSERVER == 1
			LogAddTD("롱스피어 전환 %s", lpObj->Name);
#endif
			lpObj->SkillLongSpearChange = TRUE;
		}
#endif

		lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].PlusSpecial(&lpObj->m_AttackDamageMinRight,AT_IMPROVE_DAMAGE);
		lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].PlusSpecial(&lpObj->m_AttackDamageMaxRight,AT_IMPROVE_DAMAGE);	
	}
	
	if( Left->m_Type != -1 )
	{
		if( Left->m_IsValidItem )
		{
			lpObj->m_AttackDamageMinLeft += Left->m_DamageMin;
			lpObj->m_AttackDamageMaxLeft += Left->m_DamageMax;
		}

		lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].PlusSpecial(&lpObj->m_AttackDamageMinLeft,AT_IMPROVE_DAMAGE);
		lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].PlusSpecial(&lpObj->m_AttackDamageMaxLeft,AT_IMPROVE_DAMAGE);
	}	

	//LogAdd("error-(m_AttackDamageMinLeft6)[%s][%d]:%d",lpObj->Name, lpObj->Dexterity, lpObj->m_AttackDamageMinLeft);
	lpObj->m_CriticalDamage = 0;
	lpObj->m_ExcelentDamage = 0;
	lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].PlusSpecial(&lpObj->m_CriticalDamage,AT_LUCK);
	lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].PlusSpecial(&lpObj->m_CriticalDamage,AT_LUCK);
	lpObj->pInventory[EQUIPMENT_HELM  ].PlusSpecial(&lpObj->m_CriticalDamage,AT_LUCK);
	lpObj->pInventory[EQUIPMENT_ARMOR ].PlusSpecial(&lpObj->m_CriticalDamage,AT_LUCK);
	lpObj->pInventory[EQUIPMENT_PANTS ].PlusSpecial(&lpObj->m_CriticalDamage,AT_LUCK);
    lpObj->pInventory[EQUIPMENT_GLOVES].PlusSpecial(&lpObj->m_CriticalDamage,AT_LUCK);
	lpObj->pInventory[EQUIPMENT_BOOTS ].PlusSpecial(&lpObj->m_CriticalDamage,AT_LUCK);
	lpObj->pInventory[EQUIPMENT_WING  ].PlusSpecial(&lpObj->m_CriticalDamage,AT_LUCK);
	
	// Magic Damage // DW
	//------------------------------------------------------------------------
	lpObj->m_MagicDamageMin = Energy/Balancer.MagicDamageMin;
	lpObj->m_MagicDamageMax = Energy/Balancer.MagicDamageMax;

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 신규캐릭터 최소, 최대 마력
	if( lpObj->Class == CLASS_SUMMONER )
	{
		lpObj->m_MagicDamageMin = Energy/9 + 1/100;
		lpObj->m_MagicDamageMax = Energy/4 + 1.5/100;

		lpObj->m_CurseDamgeMin	= Energy/9 + 1/100;
		lpObj->m_CurseDamgeMax	= Energy/4 + 1.5/100;
	}
#endif // ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428
	if(lpObj->Class == CLASS_MAGUMSA)
	{
		// 최대 공격력/마력 상승
		lpObj->m_MagicDamageMax += lpObj->m_MPSkillOpt.iMpsMaxAttackMagicDamage;

		// 최소 공격력/마력 상승
		lpObj->m_MagicDamageMin += lpObj->m_MPSkillOpt.iMpsMinAttackMagicDamage;
	}

	if( lpObj->Class == CLASS_WIZARD )
	{
		// 최대 마력 상승
		lpObj->m_MagicDamageMax += lpObj->m_MPSkillOpt.iMpsMaxMagicDamage;
		
		// 최소 마력 상승
		lpObj->m_MagicDamageMin += lpObj->m_MPSkillOpt.iMpsMinMagicDamage;
	}
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428

	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&lpObj->m_MagicDamageMin,AT_IMPROVE_MAGIC);
	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&lpObj->m_MagicDamageMax,AT_IMPROVE_MAGIC);

#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&lpObj->m_CurseDamgeMin,AT_IMPROVE_CURSE);
	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&lpObj->m_CurseDamgeMax,AT_IMPROVE_CURSE);
#endif
	
	if( Right->m_Type != -1 )
	{	
#ifdef ADD_ITEM
		if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(0, 31)    // 데스 브로드

	#ifdef ADD_CRITICAL_DAMAGE_OF_DEATH_BLADE_20050419		
			|| lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(0, 21) // 데스 블레이드
	
    #endif // ADD_CRITICAL_DAMAGE_OF_DEATH_BLADE_20050419

	#ifdef MODIFY_EXPLOSION_BLADE_WEAPON_MAGICDAMAGE_BUG_20060623
			|| lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(0, 23) // 익스플로전 블레이드
	#endif // MODIFY_EXPLOSION_BLADE_WEAPON_MAGICDAMAGE_BUG_20060623
			
	#ifdef MODIFY_350ITEM_BUGFIX2_20060823
			|| lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(0, 25) // 380아이템. 소드댄서
	#endif // MODIFY_350ITEM_BUGFIX2_20060823
			
	#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
			|| lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM( 0, 28 ) // 룬 바스타드
	#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
		  )
		{	// 마검사 마법검의 경우에는 마력으로 공격력으로 올린다
			lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].PlusSpecial(&lpObj->m_MagicDamageMin,AT_IMPROVE_DAMAGE);
			lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].PlusSpecial(&lpObj->m_MagicDamageMax,AT_IMPROVE_DAMAGE);
		}
		else

#endif // ADD_ITEM
		{		
			lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].PlusSpecial(&lpObj->m_MagicDamageMin,AT_IMPROVE_MAGIC);
			lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].PlusSpecial(&lpObj->m_MagicDamageMax,AT_IMPROVE_MAGIC);
		}
//		lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].PlusSpecialPercent(&lpObj->m_MagicDamageMin,AT_IMPROVE_MAGIC, 2);
//		lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].PlusSpecialPercent(&lpObj->m_MagicDamageMax,AT_IMPROVE_MAGIC, 2);
	}

#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
	if( Left->m_Type != -1 )
	{
		lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].PlusSpecial(&lpObj->m_CurseDamgeMin,AT_IMPROVE_CURSE);
		lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].PlusSpecial(&lpObj->m_CurseDamgeMax,AT_IMPROVE_CURSE);
	}
#endif

	//------------------------------------------------------------------------
	// AttackRating 
	lpObj->m_AttackRating  = (Strength+Dexterity)/2;	
	lpObj->m_AttackRating += lpObj->pInventory[EQUIPMENT_GLOVES].ItemDefense();

	//------------------------------------------------------------------------
	// AttackSpeed
	// AttackSpeed 계산
	if(lpObj->Class == CLASS_ELF)
    {
		lpObj->m_AttackSpeed = Dexterity/Balancer.AttackSpeed_Elf;
        lpObj->m_MagicSpeed = Dexterity/Balancer.MagicSpeed_Elf;
    }
    //  기사. 마검사.
	else if(lpObj->Class == CLASS_KNIGHT || 
            lpObj->Class == CLASS_MAGUMSA )  //  20 --> 18
    {
		lpObj->m_AttackSpeed = Dexterity/Balancer.AttackSpeed_DK_MG;
        lpObj->m_MagicSpeed = Dexterity/Balancer.MagicSpeed_DK_MG;
    }
    //  법사.
#ifdef DARKLORD_WORK	
		else if(lpObj->Class == CLASS_DARKLORD )
		{
			lpObj->m_AttackSpeed = Dexterity/Balancer.AttackSpeed_DL;
			lpObj->m_MagicSpeed = Dexterity/Balancer.MagicSpeed_DL;
		}
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 신규캐릭터 공격속도
	else if(lpObj->Class == CLASS_SUMMONER)
    {
		lpObj->m_AttackSpeed = Dexterity/20;
        lpObj->m_MagicSpeed = Dexterity/20;
    }
#endif
	else
    {
		lpObj->m_AttackSpeed = Dexterity/Balancer.AttackSpeed_DW;
		lpObj->m_MagicSpeed = Dexterity/Balancer.MagicSpeed_DW;
    }

	bool bRight = false;
	bool bLeft = false;

    if((Right->m_Type!=ITEM_BOW+7 && Right->m_Type!=ITEM_BOW+15 ) && Right->m_Type>=ITEM_SWORD && Right->m_Type<ITEM_STAFF+MAX_ITEM_INDEX)
	{
#ifdef MODIFY_EQUIPITEM_CHECKDURABILITY_20080811
		if( Right->m_IsValidItem && Right->m_Durability > 0.f)
#else
		if( Right->m_IsValidItem )
#endif // MODIFY_EQUIPITEM_CHECKDURABILITY_20080811
		{
			bRight = true;
		}
	}

    if((Left->m_Type!=ITEM_BOW+7 && Left->m_Type!=ITEM_BOW+15 ) && Left->m_Type>=ITEM_SWORD && Left->m_Type<ITEM_STAFF+MAX_ITEM_INDEX)
	{
#ifdef MODIFY_EQUIPITEM_CHECKDURABILITY_20080811
		if( Left->m_IsValidItem && Left->m_Durability > 0.f)
#else
		if( Left->m_IsValidItem )
#endif // MODIFY_EQUIPITEM_CHECKDURABILITY_20080811
		{
			bLeft = true;
		}
	}

    if(bRight && bLeft)
    {
     	lpObj->m_AttackSpeed += (Right->m_AttackSpeed+Left->m_AttackSpeed)/2;
     	lpObj->m_MagicSpeed += (Right->m_AttackSpeed+Left->m_AttackSpeed)/2;
    }
	else if(bRight)
    {
        lpObj->m_AttackSpeed += Right->m_AttackSpeed;
        lpObj->m_MagicSpeed += Right->m_AttackSpeed;
    }
	else if(bLeft)
    {
        lpObj->m_AttackSpeed += Left->m_AttackSpeed;
        lpObj->m_MagicSpeed += Left->m_AttackSpeed;
    }
	if(Gloves->m_Type != -1)
    {
		if( Gloves->m_IsValidItem )
		{
			lpObj->m_AttackSpeed += Gloves->m_AttackSpeed;
			lpObj->m_MagicSpeed += Gloves->m_AttackSpeed;
		}
    }

	// 디노란트
	if(Helper->m_Type != -1)
	{
		if( Helper->m_IsValidItem )
		{		
			lpObj->m_AttackSpeed += Helper->m_AttackSpeed;
			lpObj->m_MagicSpeed += Helper->m_AttackSpeed;
		}
	}
	if(Amulet->m_Type != -1)
	{
		if( Amulet->m_IsValidItem )
		{		
			lpObj->m_AttackSpeed += Amulet->m_AttackSpeed;
			lpObj->m_MagicSpeed += Amulet->m_AttackSpeed;
		}
	}
	

#ifdef RING_EVENT_ITEMBOX_20031124		// 반지이벤트 - 마법사의 반지 착용 시 공격력10%, 마력10%, 공속10 증가
	if( (
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level == 0 &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability > 0.f
		) ||
		(
		lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level == 0 &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability > 0.f
		) )
	{	// 둘 중의 한 손에 마법사의 반지를 차고 있다면
		lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * 10 / 100;
		lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * 10 / 100;

		lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * 10 / 100;
		lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * 10 / 100;

		lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * 10 /100;
		lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * 10 /100;

		lpObj->m_AttackSpeed += 10;
		lpObj->m_MagicSpeed += 10;
	}
#endif
	
#ifdef ADD_CRITICAL_WIZARD_RING_20090915	// 치명 마법사의 반지 착용 시 공격력15%, 마력15%, 공속10 증가
	if( (
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13, 107) &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level == 0 &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability > 0.f
		) ||
		(
		lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13, 107) &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level == 0 &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability > 0.f
		) )
	{	// 둘 중의 한 손에 치명 마법사의 반지를 차고 있다면
		lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * 15 / 100;
		lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * 15 / 100;
		
		lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * 15 / 100;
		lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * 15 / 100;
		
		lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * 15 /100;
		lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * 15 /100;
		
		lpObj->m_AttackSpeed += 10;
		lpObj->m_MagicSpeed += 10;
	}
#endif // ADD_CRITICAL_WIZARD_RING_20090915

#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// 중국 이벤트 - 영예의 반지 
	if( (
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level == 3 &&
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability > 0.f
		) ||
		(
		lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,20) &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level == 3 &&
		lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability > 0.f
		) )
	{	// 둘 중의 한 손에 영예의 반지를 차고 있다면
		lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * 10 / 100;
		lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * 10 / 100;

		lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * 10 / 100;
		lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * 10 / 100;

		lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * 10 /100;
		lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * 10 /100;

		lpObj->m_AttackSpeed += 10;
		lpObj->m_MagicSpeed += 10;
	}
#endif

#if TESTSERVER == 1
	LogAdd("공속 : %d,%d", lpObj->m_AttackSpeed, lpObj->m_MagicSpeed);
#endif
	
	// 공격 속도 체크 시간 설정
	if( lpObj->Class == CLASS_WIZARD )
	{	// 법사는 공격 속도*2로 적용된다
		lpObj->m_DetectSpeedHackTime = (int)(gAttackSpeedTimeLimit-((lpObj->m_MagicSpeed*2)*gDecTimePerAttackSpeed));
	}
	else
	{
		lpObj->m_DetectSpeedHackTime = (int)(gAttackSpeedTimeLimit-((lpObj->m_AttackSpeed)*gDecTimePerAttackSpeed));
	}

	if( lpObj->m_DetectSpeedHackTime < gMinimumAttackSpeedTime )
	{
		lpObj->m_DetectSpeedHackTime = gMinimumAttackSpeedTime;
	}
	
	//------------------------------------------------------------------------
	// SuccessfulBlocking
	if( lpObj->Class == CLASS_ELF )
	{
		lpObj->m_SuccessfulBlocking = Dexterity/Balancer.SuccessfulBlocking_Elf;
	}
#ifdef DARKLORD_WORK
	else if( lpObj->Class == CLASS_DARKLORD )
	{
		lpObj->m_SuccessfulBlocking = Dexterity/Balancer.SuccessfulBlocking_DL;
	}
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 방어 성공률
#ifdef MODIFY_SUMMONER_BUGFIX_1_20071221	// 방어성공률 조건 수정
	else if( lpObj->Class == CLASS_SUMMONER )
#else	// MODIFY_SUMMONER_BUGFIX_1_20071221
	if( lpObj->Class == CLASS_SUMMONER )
#endif	// MODIFY_SUMMONER_BUGFIX_1_20071221
	{
		lpObj->m_SuccessfulBlocking = Dexterity/4;
	}
#endif
	else if( lpObj->Class == CLASS_WIZARD )
	{
		lpObj->m_SuccessfulBlocking = Dexterity/Balancer.SuccessfulBlocking_DW;
	}
	else if( lpObj->Class == CLASS_KNIGHT )
	{
		lpObj->m_SuccessfulBlocking = Dexterity/Balancer.SuccessfulBlocking_DK;
	}
	else if( lpObj->Class == CLASS_MAGUMSA )
	{
		lpObj->m_SuccessfulBlocking = Dexterity/Balancer.SuccessfulBlocking_MG;
	}
	/*else
	{
		lpObj->m_SuccessfulBlocking = Dexterity/Balancer.SuccessfulBlocking_DW_DK_MG;
	}*/

	if( Left->m_Type != -1 )
	{
		if( Left->m_IsValidItem )
		{
			lpObj->m_SuccessfulBlocking += Left->m_SuccessfulBlocking;
			lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].PlusSpecial(&lpObj->m_SuccessfulBlocking,AT_IMPROVE_BLOCKING);
		}
	}

	bool Success = true;


	if( lpObj->Class == CLASS_MAGUMSA )
	{
		for(int i=EQUIPMENT_HELM+1;i<=EQUIPMENT_BOOTS;i++)
		{
			if(lpObj->pInventory[i].m_Type == -1)
			{
				Success = false;
				break;
			}
			else if( !lpObj->pInventory[i].m_IsValidItem )
			{
				Success = false;
				break;
			}
		}
	}
	else
	{
		for(int i=EQUIPMENT_HELM;i<=EQUIPMENT_BOOTS;i++)
		{
			if(lpObj->pInventory[i].m_Type == -1)
			{
				Success = false;
				break;
			}
			else if( !lpObj->pInventory[i].m_IsValidItem )
			{
				Success = false;
				break;
			}
		}
	}

	int	Level11Count = 0, Level10Count = 0;

#ifdef ITEM_12_13_20040401
	int	Level12Count = 0, Level13Count = 0;
#endif
	
	// 세트 추가 방어율 10% 올린다.
	if(Success)
	{
		int Type;		

		// 마검사일 경우엔 헬멧은 체크 안한다
		if( lpObj->Class == CLASS_MAGUMSA )
		{
			Type = lpObj->pInventory[EQUIPMENT_ARMOR].m_Type%MAX_ITEM_INDEX;
			
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
				// 마검사 : 아틀란스, 선더호크, 아스릴, 썬라이트, 볼케이노 세트만 인정. 발리언트세트 추가. 펜텀,디스트로이 추가
				if( Type != 15 && Type != 20 && Type != 23 && Type != 33 && Type != 32 && Type != 37 && Type != 47 && Type != 48)
#else
	#ifdef MODIFY_350ITEM_BUGFIX_20060811
				// 마검사 : 아틀란스, 선더호크, 아스릴, 썬라이트, 볼케이노 세트만 인정. 발리언트세트 추가.
				if( Type != 15 && Type != 20 && Type != 23 && Type != 33 && Type != 32 && Type != 37 )
	#else
		#ifdef MODIFY_MAGUMSA_VOLCANOSET_DEFENSE_20060515
				// 마검사 : 아틀란스, 선더호크, 아스릴, 썬라이트, 볼케이노 세트만 인정.
				if( Type != 15 && Type != 20 && Type != 23 && Type != 33 && Type != 32 )
		#else
			#ifdef ADD_NEW_ITEM_01_20050823
				if( Type != 15 && Type != 20 && Type != 23 && Type != 33 )
			#else
				#ifdef MODIFY_10_11_12_13_SET_DEFENSE_20040916
					if( Type != 15 && Type != 20 && Type != 23)
				#else
				if( Type != 15 && Type != 20 )  
				#endif // MODIFY_10_11_12_13_SET_DEFENSE_20040916
			#endif // ADD_NEW_ITEM_01_20050823
		#endif // MODIFY_MAGUMSA_VOLCANOSET_DEFENSE_20060515
	#endif // MODIFY_350ITEM_BUGFIX_20060811
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
			{	// 마검사는 아틀란스셋와 만 적용되어야 한다
				Success = false;				
			}
			else
			{

#ifdef MODIFY_10_11_12_13_SET_DEFENSE_20040421
#ifdef ITEM_12_13_20040401
				Level13Count++;
#endif
#else
				Level11Count++;
#endif
				for(int i=EQUIPMENT_HELM+1;i<=EQUIPMENT_BOOTS;i++)
				{
					if(Type != lpObj->pInventory[i].m_Type%MAX_ITEM_INDEX)
					{
						Success = false;						
					}
#ifdef ITEM_12_13_20040401
					if( lpObj->pInventory[i].m_Level > 12 )
					{
						Level13Count++;
					}
					else if( lpObj->pInventory[i].m_Level > 11 )
					{
						Level12Count++;
					}
					else
#endif
					if( lpObj->pInventory[i].m_Level > 10 )
					{
						Level11Count++;
					}
					else if( lpObj->pInventory[i].m_Level > 9 )
					{
						Level10Count++;
					}
				}
			}		
		}
		else
		{
			Type = lpObj->pInventory[EQUIPMENT_HELM].m_Type%MAX_ITEM_INDEX;
			for(int i=EQUIPMENT_HELM;i<=EQUIPMENT_BOOTS;i++)
			{
				if(Type != lpObj->pInventory[i].m_Type%MAX_ITEM_INDEX)
				{
					Success = false;					
				}
#ifdef ITEM_12_13_20040401
				if( lpObj->pInventory[i].m_Level > 12 )
				{
					Level13Count++;
				}
				else if( lpObj->pInventory[i].m_Level > 11 )
				{
					Level12Count++;
				}
				else
#endif
				if( lpObj->pInventory[i].m_Level > 10 )
				{
					Level11Count++;
				}
				else if( lpObj->pInventory[i].m_Level > 9 )
				{
					Level10Count++;
				}
			}
		}
		if(Success)
		{
			lpObj->m_SuccessfulBlocking += lpObj->m_SuccessfulBlocking/Balancer.OtherSuccessfulBlocking; // other succes bloking item
		}		
	}
	
	//------------------------------------------------------------------------
	// Defense
		if(lpObj->Class == CLASS_ELF)
     		lpObj->m_Defense  = Dexterity/Balancer.Defense_Elf;
		else if(lpObj->Class == CLASS_KNIGHT )
     		lpObj->m_Defense  = Dexterity/Balancer.Defense_DK;
	#ifdef MODIFY_CALC_MAGUMSA_DEFENSE_20040511
		else if(lpObj->Class == CLASS_MAGUMSA )
			lpObj->m_Defense  = Dexterity/5;
	#endif
		#ifdef DARKLORD_WORK
		else if(lpObj->Class == CLASS_DARKLORD )
			lpObj->m_Defense  = Dexterity/Balancer.Defense_DL;
		#endif	// DARKLORD_WORK
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 신규캐릭터 방어력
		else if(lpObj->Class == CLASS_SUMMONER )
		{
			lpObj->m_Defense  = Dexterity/3;
		}
#endif
		else 
			lpObj->m_Defense  = Dexterity/Balancer.Defense_DW_MG;
	
	lpObj->m_Defense += lpObj->pInventory[EQUIPMENT_HELM ].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[EQUIPMENT_ARMOR].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[EQUIPMENT_PANTS].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[EQUIPMENT_GLOVES].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[EQUIPMENT_BOOTS ].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[EQUIPMENT_WING].ItemDefense();

#ifdef DARKLORD_WORK
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() )
	{
		if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13, 4) && 
			lpObj->pInventory[EQUIPMENT_HELPER].m_Durability > 0.f )
		{	// 다크호스 방어력 증가
			
			lpObj->m_Defense += (5+Dexterity/20+Helper->m_PetItem_Level*2);
		}
	}
#endif

#ifdef ADD_ILLUSION_FENRIR_20070703
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() )
	{
		if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13, 37) && 
			lpObj->pInventory[EQUIPMENT_HELPER].m_Durability > 0.f &&
			lpObj->pInventory[EQUIPMENT_HELPER].IsIllusionFenrirOption())
		{
			// 환영펜릴 추가 옵션 적용			
			// HP 증가 (+캐릭터레벨 / 2)
	#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 종합레벨 적용 - 환영펜릴
			int iTotalLevel = lpObj->Level + lpObj->m_nMasterLevel;
			lpObj->AddLife += iTotalLevel / 2;

			// MP 증가 (+캐릭터레벨 / 2)
			lpObj->AddMana += iTotalLevel / 2;

			// 공격력 증가 (+캐릭터레벨 / 12)
			lpObj->m_AttackDamageMinRight	+= iTotalLevel / 12;
			lpObj->m_AttackDamageMaxRight	+= iTotalLevel / 12;
			lpObj->m_AttackDamageMinLeft	+= iTotalLevel / 12;
			lpObj->m_AttackDamageMaxLeft	+= iTotalLevel / 12;
			
			// 마력 증가 (+캐릭터레벨 / 25)
			lpObj->m_MagicDamageMax			+= iTotalLevel / 25;
			lpObj->m_MagicDamageMin			+= iTotalLevel / 25;
	#else			
			lpObj->AddLife += lpObj->Level / 2;

			// MP 증가 (+캐릭터레벨 / 2)
			lpObj->AddMana += lpObj->Level / 2;

			// 공격력 증가 (+캐릭터레벨 / 12)
			lpObj->m_AttackDamageMinRight	+= lpObj->Level / 12;
			lpObj->m_AttackDamageMaxRight	+= lpObj->Level / 12;
			lpObj->m_AttackDamageMinLeft	+= lpObj->Level / 12;
			lpObj->m_AttackDamageMaxLeft	+= lpObj->Level / 12;
			
			// 마력 증가 (+캐릭터레벨 / 25)
			lpObj->m_MagicDamageMax			+= lpObj->Level / 25;
			lpObj->m_MagicDamageMin			+= lpObj->Level / 25;
	#endif	// MASTER_LEVEL_UP_SYSTEM_20070912			
		}
	}
#endif	// ADD_ILLUSION_FENRIR_20070703
	
	// +10, 11짜리 아이템을 5개이상 장착하고 세트로 입은 경우
#ifdef ITEM_12_13_20040401
	if( Level13Count+Level12Count+Level11Count+Level10Count >= 5 && Success )
#else
	if( Level11Count+Level10Count >= 5 && Success )
#endif
	{	// 
#ifdef ITEM_12_13_20040401
		if( Level13Count == 5 )
		{
			lpObj->m_Defense += lpObj->m_Defense*20/100;
		}
		else if( (Level12Count == 5) || (Level12Count+Level13Count == 5) )
		{
			lpObj->m_Defense += lpObj->m_Defense*15/100;
		}
		else if( (Level11Count == 5) || (Level11Count+Level12Count+Level13Count == 5) )
		{
			lpObj->m_Defense += lpObj->m_Defense*10/100;
		}
		else if( (Level10Count == 5) || (Level10Count+Level11Count+Level12Count+Level13Count == 5) )
		{
			lpObj->m_Defense += lpObj->m_Defense*5/100;
		}
#else
		if( Level11Count < 5 )
		{
			// +11 짜리가 5개 이하이면 추가방어력 5%
			lpObj->m_Defense += lpObj->m_Defense/20;
			//LogAdd("%s 님 +Level10 추가 방어력 5% 상승", lpObj->Name);
		}
		else
		{
			// +11 짜리가 5개 이상이면 추가방어력 10%
			lpObj->m_Defense += lpObj->m_Defense/10;
			//LogAdd("%s 님 +Level11 추가 방어력 10% 상승", lpObj->Name);
		}
#endif
	}

#ifdef BUGFIX_MASTERSKILL_INC_DEFENSE_BUGFIX_20080917	// 방어력 증가값을 여기서 적용
	// ( 기본 방어력 + 아이템 방어력 ) + 패시브 스킬값
	lpObj->m_Defense += lpObj->m_MPSkillOpt.iMpsDefence;
#endif	// BUGFIX_MASTERSKILL_INC_DEFENSE_BUGFIX_20080917
	
	lpObj->m_Defense  = lpObj->m_Defense*10/20;

	if( lpObj->m_Change == 9 )	// 선더리치로 변신했다면.. 장거리 공격 가능 (기사에게 용이)
	{
		
	}
	else if( lpObj->m_Change == 41)	// 데스 카우로 변신했다면.. MAX HP + 20% 상승
	{	
		lpObj->AddLife =  (int)(lpObj->MaxLife*20)/100;		// 20% 계산후 추가
		
	}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// 방어력, HP증가 처리(캐릭터의 레벨만큼 증가)
	else if( lpObj->m_Change == 372 )	// 엘리트해골전사로 변신했다면 HP + CLevel
	{
		// 방어력 10%
		lpObj->m_Defense += lpObj->m_Defense / 10;

		// HP + 캐릭터레벨
	#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 종합레벨 적용 - 엘리트해골전사
		lpObj->AddLife += ( lpObj->Level + lpObj->m_nMasterLevel );
	#else
		lpObj->AddLife += lpObj->Level;
	#endif
	}
#endif	
	
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	// 산타 변신반지 착용
	else if( lpObj->m_Change == 374 )
	{
		lpObj->m_AttackDamageMaxLeft	+= 20;
		lpObj->m_AttackDamageMinLeft	+= 20;
		lpObj->m_AttackDamageMaxRight	+= 20;
		lpObj->m_AttackDamageMinRight	+= 20;
		lpObj->m_MagicDamageMin			+= 20;
		lpObj->m_MagicDamageMax			+= 20;
	}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef ADD_SAFEGUARD_ITEM_20080702
	//데몬 공격력, 마력 40%, 공속 10 증가
	if( gObjDaemonSprite(lpObj) == TRUE )
	{		
		lpObj->m_AttackSpeed	+= 10;
		lpObj->m_MagicSpeed		+= 10;
	}
	
	//grooving 수호정령 생명력 올리기14, 52
	if( gObjSafeGuardSprite(lpObj) == TRUE )
	{
		lpObj->AddLife += 50;
	}	
#endif //ADD_SAFEGUARD_ITEM_20080702

#ifdef ADD_PREMIUMITEM_PANDA
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() 
		&& lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13, 80) 
		&& lpObj->pInventory[EQUIPMENT_HELPER].m_Durability > 0)
	{	// 팬더펫 천사 +50 방어력 증가
		lpObj->m_Defense += 50;
		lpObj->m_MagicDefense += 50;
	}
#endif // ADD_PREMIUMITEM_PANDA
	
#ifdef ADD_UNICORN_PET_20090915
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() 
		&& lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13, 106) 
		&& lpObj->pInventory[EQUIPMENT_HELPER].m_Durability > 0)
	{	// 유니콘 펫 +50 방어력 증가
		lpObj->m_Defense += 50;
		lpObj->m_MagicDefense += 50;
	}
#endif // ADD_UNICORN_PET_20090915

#ifdef NEW_FORSKYLAND2
	if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,0) )
	{	// 수호 천사 +50 HP증가
		lpObj->AddLife += 50;
	}

	int addlife = 0, addmana = 0;
	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&addlife, AT_PLUS_WING_MAXLIFE);
	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&addmana, AT_PLUS_WING_MAXMANA);

#ifdef DARKLORD_WORK
	lpObj->pInventory[EQUIPMENT_WING].PlusSpecial(&AddLeadership, AT_PLUS_WING_ADD_LEADERSHIP);
#ifdef MODIFY_SETITEM_LEADERSHIP_BUGFIX
	lpObj->AddLeadership += AddLeadership;
#else
	lpObj->AddLeadership = AddLeadership;
#endif // MODIFY_SETITEM_LEADERSHIP_BUGFIX
#endif

	lpObj->AddLife += addlife;
	lpObj->AddMana += addmana;

	if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) )
	{	// 디노란트 옵션 적용
		lpObj->pInventory[EQUIPMENT_HELPER].PlusSpecial(&lpObj->AddBP, AT_IMPROVE_AG_MAX);
	}

#ifdef DARKLORD_WORK
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		gDarkSpirit[lpObj->m_Index].Set(lpObj->m_Index, &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT]);
	}

#endif

#endif // NEW_FORSKYLAND2


#ifdef PCBANG_ADVANTAGE // happycat@20050201 (PC방 혜택)
	if (IsOKPCBangBenefit(lpObj))
	{
	#if defined(PCBANG_ADVANTAGE) && defined(FOR_TAIWAN)
		lpObj->AddLife	+= 100;
		lpObj->AddMana	+= 100;	
		lpObj->AddBP	+= 20;
		lpObj->m_Defense+= 100;	//Client에서 보여주는 값과 차이가 있어요. -0-
	#else
		lpObj->AddLife	+= 40;
		lpObj->AddMana	+= 40;	
		lpObj->AddBP	+= 20;
		lpObj->m_Defense+= 40;	//Client에서 보여주는 값과 차이가 있어요. -0-
	#endif
	}
#endif

#ifdef HAPPY_POUCH// happycat@20050201 (복주머니, 국내 설 이벤트)
	lpObj->AddLife	+= lpObj->m_btLifeInc;
#endif

#ifdef NEW_FORSKYLAND2
	lpObj->pInventory[EQUIPMENT_AMULET].PlusSpecialPercentEx(&lpObj->AddBP, lpObj->MaxBP, AT_SET_OPTION_IMPROVE_MAXAG);
	lpObj->pInventory[EQUIPMENT_RING_RIGHT].PlusSpecialPercentEx(&lpObj->AddMana, lpObj->MaxMana, AT_SET_OPTION_IMPROVE_MAXMANA);
	lpObj->pInventory[EQUIPMENT_RING_LEFT].PlusSpecialPercentEx(&lpObj->AddMana, lpObj->MaxMana, AT_SET_OPTION_IMPROVE_MAXMANA);	
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 저주력
	//------------------------------------------------------------------------
	// Curse
	if( lpObj->Class == CLASS_SUMMONER)
	{
		lpObj->m_Curse	= lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].ItemCurse();
	}
#endif

	//------------------------------------------------------------------------
	// 저항력 계산
	CItem	*rItem[3];
	int     comparecount=0;
	
	rItem[0] = &lpObj->pInventory[EQUIPMENT_RING_RIGHT];
	rItem[1] = &lpObj->pInventory[EQUIPMENT_RING_LEFT];
	rItem[2] = &lpObj->pInventory[EQUIPMENT_AMULET];	

	lpObj->m_Resistance[RESISTANCE_POISON] = Max3(rItem[0]->m_Resistance[RESISTANCE_POISON], rItem[1]->m_Resistance[RESISTANCE_POISON],  rItem[2]->m_Resistance[RESISTANCE_POISON]);
	lpObj->m_Resistance[RESISTANCE_COLD]   = Max3(rItem[0]->m_Resistance[RESISTANCE_COLD],   rItem[1]->m_Resistance[RESISTANCE_COLD],    rItem[2]->m_Resistance[RESISTANCE_COLD]);
	lpObj->m_Resistance[RESISTANCE_THUNDER]= Max3(rItem[0]->m_Resistance[RESISTANCE_THUNDER],rItem[1]->m_Resistance[RESISTANCE_THUNDER], rItem[2]->m_Resistance[RESISTANCE_THUNDER]);
	lpObj->m_Resistance[RESISTANCE_FIRE]   = Max3(rItem[0]->m_Resistance[RESISTANCE_FIRE],   rItem[1]->m_Resistance[RESISTANCE_FIRE],    rItem[2]->m_Resistance[RESISTANCE_FIRE]);

	lpObj->m_Resistance[RESISTANCE_EARTH]   = Max3(rItem[0]->m_Resistance[RESISTANCE_EARTH],   rItem[1]->m_Resistance[RESISTANCE_EARTH],    rItem[2]->m_Resistance[RESISTANCE_EARTH]);
	lpObj->m_Resistance[RESISTANCE_WIND]   = Max3(rItem[0]->m_Resistance[RESISTANCE_WIND],   rItem[1]->m_Resistance[RESISTANCE_WIND],    rItem[2]->m_Resistance[RESISTANCE_WIND]);
	lpObj->m_Resistance[RESISTANCE_WATER]   = Max3(rItem[0]->m_Resistance[RESISTANCE_WATER],   rItem[1]->m_Resistance[RESISTANCE_WATER],    rItem[2]->m_Resistance[RESISTANCE_WATER]);

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// 패시브 1차 - 저항력 적용 
	lpObj->m_Resistance[RESISTANCE_POISON]	+= lpObj->m_MPSkillOpt.iMpsResistancePoison;
	lpObj->m_Resistance[RESISTANCE_THUNDER]	+= lpObj->m_MPSkillOpt.iMpsResistanceThunder;
	lpObj->m_Resistance[RESISTANCE_COLD]	+= lpObj->m_MPSkillOpt.iMpsResistanceIce;
#endif
	
	//-------------------------------------------------------------------------
	// 380 추가 옵션 아이템 적용 20060711
	//-------------------------------------------------------------------------
#ifdef ADD_380ITEM_NEWOPTION_20060711
	// 기본 옵션 값 초기화 및 계산
	g_kItemSystemFor380.ApplyFor380Option( lpObj );
#endif // ADD_380ITEM_NEWOPTION_20060711

	//-------------------------------------------------------------------------
	// 조화의 보석 강화 아이템 적용 20060612
	//-------------------------------------------------------------------------
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// 기본 옵션 값 초기화 및 계산
	g_kJewelOfHarmonySystem.SetApplyStrengthenItem( lpObj );
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	
#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
	g_SocketOptionSystem.SetApplySocketEffect( lpObj );
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107		// 패시브 2차 - 캐릭터 최대HP 증가
	//-------------------------------------------------------------------------
	// 마스터레벨 패시브 2차 적용
	//-------------------------------------------------------------------------
	lpObj->AddLife += lpObj->m_MPSkillOpt.iMpsMaxHP;
#endif	
	
#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107		// 패시브 2차 - 최대AG(BP) 증가
	lpObj->AddBP	+= lpObj->m_MPSkillOpt.iMpsMaxBP;
#endif

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428			// 마스터레벨 4랭크 - 최대 SD 증가
	lpObj->iAddShield += lpObj->m_MPSkillOpt.iMpsMaxSD;
#endif

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	// 값 증가 처리
	// 엑셀런트 아이템들의 수치를 적용시킴
	GObjExtItemApply(lpObj, 0);   // 값증가
	// 세트 아이템들의 수치를 적용시킴 : 값 증가
	gObjSetItemApply(lpObj, 0);

	// % 증가 처리
	GObjExtItemApply(lpObj, 1);
	// 세트 아이템들의 수치를 적용시킴
	gObjSetItemApply(lpObj, 1);

	// 최대값 처리
	if( lpObj->Life > (lpObj->MaxLife+lpObj->AddLife) )
	{
		lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)lpObj->iShield);
#endif // ADD_SHIELD_POINT_01_20060403
	}
	if( lpObj->Mana > (lpObj->MaxMana+lpObj->AddMana) )
	{
		// 현재 마나가 최대 생명보다 크면 원래로 돌린다
		lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
		GCManaSend(lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, lpObj->BP);		
	}

	gObjSetBP(lpObj->m_Index);
	if ( lpObj->BP > lpObj->MaxBP + lpObj->AddBP )
	{
		lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
		GCManaSend(lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, lpObj->BP);		
	}

#else
	// 엑셀런트 아이템들의 수치를 적용시킴
	GObjExtItemApply(lpObj);
#if GAME_VERSION >= G_V_99B
	// 세트 아이템들의 수치를 적용시킴
	gObjSetItemApply(lpObj);
#endif
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519

	//------------------------------------------------------------------------
	// NextExperince -> 캐릭터의 다음 경험치 계산
	gObjNextExpCal(lpObj);

	if( Left->m_Type >= ITEM_BOW && Left->m_Type < ITEM_BOW+7 || Left->m_Type == ITEM_BOW+17 
#ifdef MODIFY_ARROWVIPER_2PERCENT_DAMAGE_20050816
		|| Left->m_Type == ITEM_BOW+20
#endif
#ifdef ADD_NEW_ITEM_01_20050823
		|| Left->m_Type == ITEM_BOW+21
#endif
#ifdef MODIFY_350ITEM_BUGFIX2_20060823
		|| Left->m_Type == ITEM_BOW+22	// 350아이템.알바트로스보우
#endif
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
		|| Left->m_Type == ITEM_BOW+23	// 다크스팅거
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
#ifdef ADD_AERINBOW_20090309
		|| Left->m_Type == ITEM_BOW+24	// 에어린보우
#endif // ADD_AERINBOW_20090309
		)
	{
		// 20030228 화살 레벨 추가		
		// 화살을 검사해서 +1 레벨이면 공격력+ 2% + 1 을 적용시킨다.
		if( (Right->m_Type == MAKE_ITEMNUM(4,15)) && Right->m_Level == 1 )
		{	// 화살일 경우
			lpObj->m_AttackDamageMinLeft	+= (WORD)(lpObj->m_AttackDamageMinLeft*(0.03f)+1);	
			lpObj->m_AttackDamageMaxLeft	+= (WORD)(lpObj->m_AttackDamageMaxLeft*(0.03f)+1);	
		}		
		else if( (Right->m_Type == MAKE_ITEMNUM(4,15)) && Right->m_Level == 2 )
		{
			lpObj->m_AttackDamageMinLeft	+= (WORD)(lpObj->m_AttackDamageMinLeft*(0.05f)+1);	
			lpObj->m_AttackDamageMaxLeft	+= (WORD)(lpObj->m_AttackDamageMaxLeft*(0.05f)+1);
		}	
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
		else if( (Right->m_Type == MAKE_ITEMNUM(4,15)) && Right->m_Level == 3 )
		{
			lpObj->m_AttackDamageMinLeft	+= (WORD)(lpObj->m_AttackDamageMinLeft*(0.07f)+1);	
			lpObj->m_AttackDamageMaxLeft	+= (WORD)(lpObj->m_AttackDamageMaxLeft*(0.07f)+1);
		}
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	}
	//석궁일때
	else if(( Right->m_Type >= ITEM_BOW+8 && Right->m_Type < ITEM_BOW+15) ||
	      ( Right->m_Type >= ITEM_BOW+16 && Right->m_Type < ITEM_BOW+MAX_ITEM_INDEX))
	
	{
		if( (Left->m_Type == MAKE_ITEMNUM(4,7)) && Left->m_Level == 1 )
		{
			lpObj->m_AttackDamageMinRight	+= (WORD)(lpObj->m_AttackDamageMinRight*(0.03f)+1);	
			lpObj->m_AttackDamageMaxRight	+= (WORD)(lpObj->m_AttackDamageMaxRight*(0.03f)+1);	
			
		}
		else if( (Left->m_Type == MAKE_ITEMNUM(4,7)) && Left->m_Level == 2 )
		{
			lpObj->m_AttackDamageMinRight	+= (WORD)(lpObj->m_AttackDamageMinRight*(0.05f)+1);	
			lpObj->m_AttackDamageMaxRight	+= (WORD)(lpObj->m_AttackDamageMaxRight*(0.05f)+1);	
		}
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
		else if( (Left->m_Type == MAKE_ITEMNUM(4,7)) && Left->m_Level == 3 )
		{
			lpObj->m_AttackDamageMinRight	+= (WORD)(lpObj->m_AttackDamageMinRight*(0.07f)+1);	
			lpObj->m_AttackDamageMaxRight	+= (WORD)(lpObj->m_AttackDamageMaxRight*(0.07f)+1);	
		}
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	}

	//LogAddTD("error-(m_AttackDamageMinLeft5)[%s][%d/%d]:%d",lpObj->Name, Left->m_DamageMin, Left->m_DamageMax, lpObj->m_AttackDamageMinLeft);
#ifdef DARKLORD_WORK
	if(lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD )
#else
	if(lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA )
#endif
	{
		if( (Right->m_Type != -1) && (Left->m_Type != -1) )
		{
			if(Right->m_Type >= 0 && Right->m_Type <ITEM_BOW&& Left->m_Type >= 0 && Left->m_Type < ITEM_BOW)
			{
				lpObj->m_AttackDamageMinRight = lpObj->m_AttackDamageMinRight*55/100;
				lpObj->m_AttackDamageMaxRight = lpObj->m_AttackDamageMaxRight*55/100;
				lpObj->m_AttackDamageMinLeft  = lpObj->m_AttackDamageMinLeft *55/100;
				lpObj->m_AttackDamageMaxLeft  = lpObj->m_AttackDamageMaxLeft *55/100;
			}
			/*if(Right->m_Type && Left->m_Type)
			{
			lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight/10;
			lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight/10;
		}*/
		}
	}

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	g_ItemAddOption.NextSetItemEffect( lpObj );
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
	g_BuffEffect.SetNextEffect( lpObj );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128

#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
	if( lpObj->m_iItemEffectValidTime > 0 )
	{
		g_ItemAddOption.NextSetItemLastEffectForHallowin( lpObj );
	}
#endif 

#ifdef ADD_SHIELD_POINT_01_20060403
	gObjCalcShieldPoint( lpObj );

	#ifdef MODIFY_LIFE_CHANGEMAXVALUE
	{
		BOOL bChangeMaxValue_Life = FALSE;
		BOOL bChangeMaxValue_Shield = FALSE;
		if ( lpObj->Life > (lpObj->MaxLife + lpObj->AddLife) )
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			bChangeMaxValue_Life = TRUE;
		}

		if ( lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield) )
		{
			lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
			bChangeMaxValue_Shield = TRUE;
		}

		if (bChangeMaxValue_Life || bChangeMaxValue_Shield)
		{
			GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield));
		}
	}
	#else
#ifdef MODIFY_SHIELD_MANA_CHANGEMAXVALUE_20060526
	// 스탯이 줄어들면서 MaxShield가 줄어들게 되면, 현재 Shield값도 업데이트 해야함.
	if ( lpObj->iShield > lpObj->iMaxShield + lpObj->iAddShield )
	{
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)(lpObj->iShield));
	}
#endif // MODIFY_SHIELD_MANA_CHANGEMAXVALUE_20060526
	#endif // MODIFY_LIFE_CHANGEMAXVALUE

	// HP, 쉴드, 마나, AG 최대값 모두 보낸다.
	GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
	GCManaSend(lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
#endif
	
	//LogAdd("%s 님 lpObj->m_SuccessfulBlocking %d ", lpObj->Name, lpObj->m_SuccessfulBlocking);
//
//#if TESTSERVER == 1 
//	char msg[255];	
//	wsprintf(msg, "Left : %d,%d  Right : %d,%d", 
//			lpObj->m_AttackDamageMinLeft, lpObj->m_AttackDamageMaxLeft,
//			lpObj->m_AttackDamageMinRight, lpObj->m_AttackDamageMaxRight);
//
//	GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
//
//	wsprintf(msg, "%d,%d", 
//			lpObj->m_AttackDamageMinLeft+lpObj->m_AttackDamageMinRight, 
//			lpObj->m_AttackDamageMaxLeft+lpObj->m_AttackDamageMaxRight);
//
//	GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
//
//	LogAddC(LOGC_RED, "Left : %d,%d  Right : %d,%d", 
//			lpObj->m_AttackDamageMinLeft, lpObj->m_AttackDamageMaxLeft,
//			lpObj->m_AttackDamageMinRight, lpObj->m_AttackDamageMaxRight);
//
//	LogAddC(LOGC_RED, "%d,%d", 
//			lpObj->m_AttackDamageMinLeft+lpObj->m_AttackDamageMinRight, 
//			lpObj->m_AttackDamageMaxLeft+lpObj->m_AttackDamageMaxRight);
//#endif
}

// 오브젝트에 확장 아이템 수치를 적용시킨다. (엑셀런트 장비 수치 계산 -> gObjCalCharacter())
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
void GObjExtItemApply(LPOBJECTSTRUCT lpObj, int type)  // type - 0 : 값증가 옵션, 1 : %증가 옵션
#else
void GObjExtItemApply(LPOBJECTSTRUCT lpObj)
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
{
	int n, i, max_count=0;
	int temp_n[3]={EQUIPMENT_WEAPON_RIGHT, EQUIPMENT_WEAPON_LEFT, EQUIPMENT_AMULET};
	int temp_n2[10]={EQUIPMENT_HELM, EQUIPMENT_ARMOR, EQUIPMENT_PANTS, EQUIPMENT_GLOVES, 
					EQUIPMENT_BOOTS, EQUIPMENT_WING, EQUIPMENT_HELPER, EQUIPMENT_RING_RIGHT, 
					EQUIPMENT_RING_LEFT, EQUIPMENT_WEAPON_LEFT};

	int maxn2_loop=9;
	// 방패를 착용했다면 왼손의 방패까지 체크한다. 
	// 그렇지 않을 경우엔 방패는 무시한다.

	// 무기, 목걸이
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519_2
	int iItemTotalMagicDamage = 0;
	int iItemTotalAttackDamageLeftValue = 0;
	int iItemTotalAttackDamageRightValue = 0;
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519_2
	for( i=0; i<3; i++)
	{
		n = temp_n[i];
		if( lpObj->pInventory[n].m_Type >= ITEM_SHIELD && lpObj->pInventory[n].m_Type < ITEM_HELM )
			continue;
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
		if( lpObj->pInventory[n].IsItem() == TRUE 
			&& lpObj->pInventory[n].m_Durability != 0.f 
			&& lpObj->pInventory[n].m_IsValidItem == TRUE )
#else
		if( lpObj->pInventory[n].IsItem() == TRUE && lpObj->pInventory[n].m_Durability != 0.f )
#endif
		{			
		#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
			if( lpObj->pInventory[n].IsExtAttackRate() && type == 0 )	// 값 증가 옵션처리 인경우.
		#else
			if( lpObj->pInventory[n].IsExtAttackRate() ) 
		#endif
			{
				if((lpObj->pInventory[n].m_Type >=ITEM_STAFF && lpObj->pInventory[n].m_Type <ITEM_STAFF+MAX_ITEM_INDEX) ||//지팡이
				   (lpObj->pInventory[n].m_Type ==ITEM_HELPER+12)
					// 얼음의 목걸이, 물의 목걸이는 마력증가..
					|| (lpObj->pInventory[n].m_Type == ITEM_HELPER+25) || (lpObj->pInventory[n].m_Type == ITEM_HELPER+27)
				   )//목걸이
				{	// 마력 증가 + 레벨/20					
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 종합레벨 - 마력증가 조절	: 마스터레벨 정보를 받으면 재설정 됨^^
					lpObj->m_MagicDamageMax			+= ( lpObj->Level + lpObj->m_nMasterLevel ) / 20;
					lpObj->m_MagicDamageMin			+= ( lpObj->Level + lpObj->m_nMasterLevel ) / 20;
#else
					lpObj->m_MagicDamageMax			+= lpObj->Level/20;
					lpObj->m_MagicDamageMin			+= lpObj->Level/20;
#endif
				}
				else
				{	// 공격력 증가 + 레벨/20
					if( n == EQUIPMENT_WEAPON_RIGHT || n == EQUIPMENT_AMULET )
					{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 종합레벨 - 무기(오)/목걸이 공격력 증가 조절 
						lpObj->m_AttackDamageMinRight	+= ( lpObj->Level + lpObj->m_nMasterLevel ) / 20;
						lpObj->m_AttackDamageMaxRight	+= ( lpObj->Level + lpObj->m_nMasterLevel ) / 20;
#else
						lpObj->m_AttackDamageMinRight	+= lpObj->Level/20;
						lpObj->m_AttackDamageMaxRight	+= lpObj->Level/20;
#endif
					}

					if( n == EQUIPMENT_WEAPON_LEFT || n == EQUIPMENT_AMULET )
					{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 종합레벨 - 무기(왼)/목걸이 공격력 증가 조절	
						lpObj->m_AttackDamageMinLeft	+= ( lpObj->Level + lpObj->m_nMasterLevel ) / 20;
						lpObj->m_AttackDamageMaxLeft	+= lpObj->Level/20;
#else
						lpObj->m_AttackDamageMinLeft	+= lpObj->Level / 20;
						lpObj->m_AttackDamageMaxLeft	+= lpObj->Level/20;
#endif
					}
				}
			}

			// 공격력 증가, 마력 증가 + 2%
		#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
			if( lpObj->pInventory[n].IsExtAttackRate2() && type == 1 )  // % 증가인 경우만 처리
		#else
			if( lpObj->pInventory[n].IsExtAttackRate2() )
		#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
			{
				if((lpObj->pInventory[n].m_Type >=ITEM_STAFF && lpObj->pInventory[n].m_Type <ITEM_STAFF+MAX_ITEM_INDEX) ||//지팡이
				   (lpObj->pInventory[n].m_Type ==ITEM_HELPER+12)//목걸이
					// 얼음의 목걸이, 물의 목걸이는 마력증가..
					|| (lpObj->pInventory[n].m_Type == ITEM_HELPER+25) || (lpObj->pInventory[n].m_Type == ITEM_HELPER+27)
				   )
				{	// 마력 증가 + 2%
					#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519_2
						iItemTotalMagicDamage += lpObj->pInventory[n].IsExtAttackRate2();
					#else
						lpObj->m_MagicDamageMax		 += lpObj->m_MagicDamageMax*lpObj->pInventory[n].IsExtAttackRate2()/100;
						lpObj->m_MagicDamageMin		 += lpObj->m_MagicDamageMin*lpObj->pInventory[n].IsExtAttackRate2()/100;
					#endif
				}
				else
				{	// 공격력 증가, 마력 증가 + 2%
					if( n == EQUIPMENT_WEAPON_RIGHT || n == EQUIPMENT_AMULET )
					{
						#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519_2
							iItemTotalAttackDamageRightValue += lpObj->pInventory[n].IsExtAttackRate2();
						#else
							lpObj->m_AttackDamageMinRight+= lpObj->m_AttackDamageMinRight*lpObj->pInventory[n].IsExtAttackRate2()/100;
							lpObj->m_AttackDamageMaxRight+= lpObj->m_AttackDamageMaxRight*lpObj->pInventory[n].IsExtAttackRate2()/100;
						#endif
					}

					if( n == EQUIPMENT_WEAPON_LEFT || n == EQUIPMENT_AMULET )
					{
						#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519_2
							iItemTotalAttackDamageLeftValue += lpObj->pInventory[n].IsExtAttackRate2();
						#else
							lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft*lpObj->pInventory[n].IsExtAttackRate2()/100;
							lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft*lpObj->pInventory[n].IsExtAttackRate2()/100;
						#endif
					}
				}
			}

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
			if ( type == 0 ) // 값 증가 처리인 경우만.
			{
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
			lpObj->m_ExcelentDamage		+= lpObj->pInventory[n].IsExtExcellentDamage();
			lpObj->m_AttackSpeed		+= lpObj->pInventory[n].IsExtAttackSpeed();
			lpObj->m_MagicSpeed			+= lpObj->pInventory[n].IsExtAttackSpeed();
			
			lpObj->MonsterDieGetLife	+= lpObj->pInventory[n].IsExtMonsterDieLife();
			lpObj->MonsterDieGetMana	+= lpObj->pInventory[n].IsExtMonsterDieMana();

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
			}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
		}
	}

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519_2
	if ( type == 1 ) // % 증가 옵션 처리. 마력. 공격력
	{
		lpObj->m_MagicDamageMax		 += lpObj->m_MagicDamageMax*iItemTotalMagicDamage/100;
		lpObj->m_MagicDamageMin		 += lpObj->m_MagicDamageMin*iItemTotalMagicDamage/100;

		lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft*iItemTotalAttackDamageLeftValue/100;
		lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft*iItemTotalAttackDamageLeftValue/100;

		lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight*iItemTotalAttackDamageRightValue/100;
		lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight*iItemTotalAttackDamageRightValue/100;
	}
#endif

	//LogAdd("error-(m_AttackDamageMinLeft7)[%s][%d]:%d",lpObj->Name, lpObj->Dexterity, lpObj->m_AttackDamageMinLeft);

	if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type >= MAKE_ITEMNUM(6,0)
#ifdef MODIFY_EXCELLENT_SHILED_BUG_20060615
			&& lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type <= MAKE_ITEMNUM(6,16)	// 엘리멘탈 방패 포함
#else
			&& lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type <= MAKE_ITEMNUM(6,15)
#endif
#ifdef MODIFY_CROSS_SHIELD_EX_OPTION_APPLY_20090326
			|| lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type == MAKE_ITEMNUM(6,21)	// 크로스 실드 포함
#endif // MODIFY_CROSS_SHIELD_EX_OPTION_APPLY_20090326			
			)
		{
			maxn2_loop = 10;
		}
	}

	// 방어구, 방패, 반지 
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	int iItemTotalLifeAddValue = 0;
	int iItemTotalManaAddValue = 0;
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
	for( i=0; i<maxn2_loop; i++)
	{	
		n = temp_n2[i];
		if( max_count < 8 )
		{

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
			if( lpObj->pInventory[n].IsItem() == TRUE 
				&& lpObj->pInventory[n].IsExtItem() 
				&& lpObj->pInventory[n].m_Durability != 0.f 
				&& lpObj->pInventory[n].m_IsValidItem == TRUE )
#else
			if( lpObj->pInventory[n].IsItem() == TRUE && lpObj->pInventory[n].IsExtItem() &&  lpObj->pInventory[n].m_Durability != 0.f )
#endif
			{	
				if( n != EQUIPMENT_WING )
				{
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
					if ( type == 1 ) // % 증가 옵션. 아이템에 의한 증가 값을 1차로 계산한다.
					{
						iItemTotalLifeAddValue += lpObj->pInventory[n].IsExtLifeAdd();
						iItemTotalManaAddValue += lpObj->pInventory[n].IsExtManaAdd();
					}
#else
					lpObj->AddLife += (short)(lpObj->MaxLife*lpObj->pInventory[n].IsExtLifeAdd()/100);
					lpObj->AddMana += (short)(lpObj->MaxMana*lpObj->pInventory[n].IsExtManaAdd()/100);
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
				
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
					if ( type == 1 ) // % 증가 옵션 처리
					{
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
					lpObj->m_SuccessfulBlocking += lpObj->m_SuccessfulBlocking*lpObj->pInventory[n].IsExtDefenseSuccessfull()/100;
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
					}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
					
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
					if ( type == 0 ) // 값 증가 옵션 처리
					{
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
					lpObj->MonsterDieGetMoney   += lpObj->pInventory[n].IsExtMonsterMoney();
					lpObj->DamageMinus		    += lpObj->pInventory[n].IsExtDamageMinus();
					lpObj->DamageReflect	    += lpObj->pInventory[n].IsExtDamageReflect();
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
					}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
					max_count++;
				}
			}
		}
	}


#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	if ( type == 0 ) // 값 증가 옵션 처리
	{
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	#ifdef NEW_FORSKYLAND2
	lpObj->AddLife += lpObj->m_SkillAddLife;	// 최대 생명력 증가도 적용해 준다
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519


#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	if ( type == 1 ) // % 증가 옵션 처리. Life와 Mana
	{
		lpObj->AddLife += (short)( (lpObj->MaxLife+lpObj->AddLife)*iItemTotalLifeAddValue/100);
		lpObj->AddMana += (short)( (lpObj->MaxMana+lpObj->AddMana)*iItemTotalManaAddValue/100);
	}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519


	//---------------------------------------- 아래 처리는 type이 1. %증가가 된 후에 처리한다. ----
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	if ( type == 1 )
	{
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
}

void	gObjCalcSetItemStat(LPOBJECTSTRUCT lpObj)
{
	for( int i = 0; i < MAX_EQUIPMENT; i++ )
	{

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
		if( lpObj->pInventory[i].IsSetItem() 
			&& lpObj->pInventory[i].m_IsValidItem
			&& lpObj->pInventory[i].m_Durability != 0.f )
#else
		if( lpObj->pInventory[i].IsSetItem() && lpObj->pInventory[i].m_IsValidItem )
#endif
		{
			int AddStatType  = lpObj->pInventory[i].GetAddStatType();

			switch(AddStatType)
			{
				case 1: 
					{					
						lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddStrength, AT_SET_IMPROVE_STRENGTH);
				
					}
					break;
				case 2: 
					{					
						lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddDexterity, AT_SET_IMPROVE_DEXTERITY);					
					}
					break;
				case 3: 
					{					
						lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddEnergy, AT_SET_IMPROVE_ENERGY);					
					}
					break;
				case 4: 
					{					
						lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddVitality, AT_SET_IMPROVE_VITALITY);					
					}
					break;
			}
		}
		//int setoption = lpObj->pInventory[i].IsSetItem();
	}
}

#if GAME_VERSION >= G_V_99B
// 세트 아이템 옵션을 찾아 준다...
// 각각의 테이블에 대한 초기화는 콜하기전에 해 줘야 된다
void	gObjGetSetItemOption(LPOBJECTSTRUCT lpObj, LPBYTE pSetOptionTable, LPBYTE pSetOptionCountTable, int* pSetOptionCount)
{
	(*pSetOptionCount) = 0;

	int	RightSetOptionIndex = -1;
	int	RightRingSetOptionIndex = -1;

	for( int i = 0; i < MAX_EQUIPMENT; i++ )
	{
		int setoption = lpObj->pInventory[i].IsSetItem();

		if( setoption && lpObj->pInventory[i].m_IsValidItem )
		{
			int OptionTableIndex = gSetItemOption.GetOptionTableIndex(lpObj->pInventory[i].m_Type, setoption);
			if( OptionTableIndex != -1 )
			{	// 해당 옵션을 값을 넣는다

				if( i == EQUIPMENT_WEAPON_RIGHT )
				{
					RightSetOptionIndex = OptionTableIndex;
				}
				else if( i == EQUIPMENT_WEAPON_LEFT )
				{
					if( RightSetOptionIndex == OptionTableIndex )
					{	// 양손의 경우 같은 세트는 적용되지 않는다
						if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].GetWeaponType() == 
							lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].GetWeaponType() && 
							lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].IsSetItem() )
						{
							LogAddTD("양손에 같은 세트가 적용되었다 무시...");
							continue;
						}
					}
				}

				if( i == EQUIPMENT_RING_RIGHT )
				{
					RightRingSetOptionIndex = OptionTableIndex;
				}
				else if( i == EQUIPMENT_RING_LEFT )
				{
					// 같은 타입의 반지 착용시 세트 옵션 적용되지 않는 버그 수정 20081202 (grooving)
					if( RightRingSetOptionIndex == OptionTableIndex )
					{
						if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == 
							lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type 
							&& lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsSetItem()
							)					
						{	// 양손의 경우 같은 세트는 적용되지 않는다						
							LogAddTD("반지에 같은 세트가 적용되었다 무시...");
							continue;
						}
					}
				}

				BOOL	bfoundindex = FALSE;
				for( int foundoptioncount = 0;  foundoptioncount < (*pSetOptionCount); foundoptioncount++ )
				{
					if( pSetOptionTable[foundoptioncount] == OptionTableIndex )
					{
						pSetOptionCountTable[foundoptioncount]++;
						bfoundindex = TRUE;

						LogAddTD("옵션 찾았다 %s, Count = %d", 
								 gSetItemOption.GetSetOptionName(OptionTableIndex),
								 pSetOptionCountTable[foundoptioncount]);

						break;
					}
				}

				if( !bfoundindex )
				{
					pSetOptionTable[(*pSetOptionCount)] = OptionTableIndex;
					pSetOptionCountTable[(*pSetOptionCount)]++;					

					LogAddTD("옵션 찾았다 %s, Count = %d", 
							 gSetItemOption.GetSetOptionName(OptionTableIndex),
							 pSetOptionCountTable[(*pSetOptionCount)]);

					(*pSetOptionCount)++;
				}
			}
		}
	}
}

// 세트 아이템 옵션을 적용한다
void gObjCalcSetItemOption(LPOBJECTSTRUCT lpObj)
{
	BYTE SetOptionTable[MAX_EQUIPMENT*2+5], SetOptionCountTable[MAX_EQUIPMENT*2+5];
	int  SetOptionCount = 0;

	memset(SetOptionTable, 0, sizeof(SetOptionTable));
	memset(SetOptionCountTable, 0, sizeof(SetOptionCountTable));

	gObjGetSetItemOption(lpObj, SetOptionTable, SetOptionCountTable, &SetOptionCount);

	int	op1, op2, op3, op4, op5;
	int	opvalue1, opvalue2, opvalue3, opvalue4, opvalue5;

//	LogAddC(LOGC_RED, "-------------스탯적용------------------");
	// 옵션들 중에 스탯이 먼저 올라 가야 되는 것들이 있기 때문에.. 스탯 먼저 적용후에
	// 나머지 옵션들을 적용한다
	// 세트 아이템 옵션 적용 (스탯 먼저 적용)
	for( int optioncount = 0; optioncount < SetOptionCount; optioncount++ )
	{
		int OptionTableIndex = SetOptionTable[optioncount];
		int	OptionTableCount = SetOptionCountTable[optioncount];		

		if( OptionTableCount >= 2 )
		{
			if( gSetItemOption.IsRequireClass(OptionTableIndex, lpObj->Class, lpObj->ChangeUP) )
			{
			}

#ifndef EXTEND_LOG_SYSTEM_20060202
			LogAdd("[%s][%s] 세트 %s옵션 적용", 
				 lpObj->AccountID, lpObj->Name, gSetItemOption.GetSetOptionName(OptionTableIndex));				
#endif // EXTEND_LOG_SYSTEM_20060202			

			for( int  tablecnt = 0; tablecnt < OptionTableCount-1; tablecnt++ )
			{
				if( gSetItemOption.GetSetOption(OptionTableIndex, tablecnt, op1, op2, opvalue1, opvalue2, lpObj->Class, lpObj->ChangeUP) )
				{
					gObjSetItemStatPlusSpecial(lpObj, op1, opvalue1);
					gObjSetItemStatPlusSpecial(lpObj, op2, opvalue2);
				}
			}

			if( gSetItemOption.GetMaxSetOptionCount(OptionTableIndex) < OptionTableCount )
			{	// 풀옵션 처리
				lpObj->IsFullSetItem = TRUE;
				LogAddC(LOGC_RED, "풀옵션 적용");
				gSetItemOption.GetGetFullSetOption(OptionTableIndex, op1, op2, op3, op4, op5,  
												   opvalue1, opvalue2, opvalue3, opvalue4, opvalue5,
												   lpObj->Class, lpObj->ChangeUP);
				gObjSetItemStatPlusSpecial(lpObj, op1, opvalue1);
				gObjSetItemStatPlusSpecial(lpObj, op2, opvalue2);
				gObjSetItemStatPlusSpecial(lpObj, op3, opvalue3);
				gObjSetItemStatPlusSpecial(lpObj, op4, opvalue4);
				gObjSetItemStatPlusSpecial(lpObj, op5, opvalue5);
			}
		}
	}
//	LogAddC(LOGC_RED, "-------------------------------------");

//	LogAddC(LOGC_RED, "--------------기타적용----------------");
	// 세트 아이템 옵션 적용 (기타 옵션들 적용
	for( int optioncount = 0; optioncount < SetOptionCount; optioncount++ )
	{
		int OptionTableIndex = SetOptionTable[optioncount];
		int	OptionTableCount = SetOptionCountTable[optioncount];
		
		if( OptionTableCount >= 2 )
		{
			LogAdd("[%s][%s] 세트 %s옵션 적용", lpObj->AccountID, lpObj->Name,  gSetItemOption.GetSetOptionName(OptionTableIndex));
			for( int  tablecnt = 0; tablecnt < OptionTableCount-1; tablecnt++ )
			{
				if( gSetItemOption.GetSetOption(OptionTableIndex, tablecnt, op1, op2, opvalue1, opvalue2, lpObj->Class, lpObj->ChangeUP) )
				{
					gObjSetItemPlusSpecial(lpObj, op1, opvalue1);
					gObjSetItemPlusSpecial(lpObj, op2, opvalue2);
				}
			}

			if( gSetItemOption.GetMaxSetOptionCount(OptionTableIndex) < OptionTableCount )
			{	// 풀옵션 처리
				LogAddC(LOGC_RED, "풀옵션 적용");
				lpObj->IsFullSetItem = TRUE;
				gSetItemOption.GetGetFullSetOption(OptionTableIndex, op1, op2, op3, op4, op5, 
												   opvalue1, opvalue2, opvalue3, opvalue4, opvalue5,
												   lpObj->Class, lpObj->ChangeUP);
				gObjSetItemPlusSpecial(lpObj, op1, opvalue1);
				gObjSetItemPlusSpecial(lpObj, op2, opvalue2);
				gObjSetItemPlusSpecial(lpObj, op3, opvalue3);
				gObjSetItemPlusSpecial(lpObj, op4, opvalue4);
				gObjSetItemPlusSpecial(lpObj, op5, opvalue5);
			}
		}
	}
//	LogAdd("-------------------------------------");
}

void gObjSetItemStatPlusSpecial(LPOBJECTSTRUCT lpObj, int option, int ivalue)
{
	if( option == -1 )
		return;	

#if TESTSERVER == 1
	char szText[255];	
#endif

	switch(option)
	{
		case AT_SET_OPTION_IMPROVE_STRENGTH:
			{	//	힘 증가
				lpObj->AddStrength += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 힘증가 +%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->AddStrength);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "힘증가 +%d %d", ivalue, lpObj->AddStrength);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif

			}
		break;

		case AT_SET_OPTION_IMPROVE_DEXTERITY:
			{	//	민첩성 증가
				lpObj->AddDexterity += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 민첩성 증가 +%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->AddDexterity);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "민첩성 증가 +%d %d", ivalue, lpObj->AddDexterity);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif				
			}
		break;

		case AT_SET_OPTION_IMPROVE_ENERGY:
			{	//	에너지 증가
				lpObj->AddEnergy += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 에너지 증가 +%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->AddEnergy);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "에너지 증가 +%d %d", ivalue, lpObj->AddEnergy);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;
		
		case AT_SET_OPTION_IMPROVE_VITALITY:
			{	//	체력 증가
				lpObj->AddVitality += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 체력 증가 +%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->AddVitality);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "체력 증가 +%d %d", ivalue, lpObj->AddVitality);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

#ifdef MODIFY_SETITEM_LEADERSHIP_BUGFIX
		case AT_SET_OPTION_IMPROVE_LEADERSHIP:
			{	//	통솔 증가
				lpObj->AddLeadership += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 통솔 증가 +%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->AddVitality);
#endif // EXTEND_LOG_SYSTEM_20060202
				
#if TESTSERVER == 1
				wsprintf(szText, "통솔 증가 +%d %d", ivalue, lpObj->AddLeadership);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif //TESTSERVER
			}
		break;
#endif // MODIFY_SETITEM_LEADERSHIP_BUGFIX			
		
	}
}


void gObjSetItemPlusSpecial(LPOBJECTSTRUCT lpObj, int option, int ivalue)
{
	if( option == -1 )
		return;	

#if TESTSERVER == 1
	char szText[255];	
#endif
	
	// !!! . 세트 아이템 옵션 수정 - b4nfter
//	option += 20;

	switch(option)
	{
		case AT_SET_OPTION_DOUBLE_DAMAGE:
			{	//	더블 데미지 확률
				lpObj->SetOpDoubleDamage += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 더블 데미지 확률 %d퍼센트 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpDoubleDamage);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "더블 데미지 확률 %d퍼센트 %d", ivalue, lpObj->SetOpDoubleDamage);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE:
			{	//	방패 장비시 방어력 5% 증가          (●)
				lpObj->SetOpImproveSheldDefence += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 방패 장비시 방어력 %d퍼센트 증가 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpImproveSheldDefence);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "방패 장비시 방어력 5퍼센트 증가 %d", lpObj->SetOpImproveSheldDefence);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;	

		case AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE:
			{	//	양손검 장비시 데미지 15% 증가       ( 서버만 )
				lpObj->SetOpTwoHandSwordImproveDamage += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 양손검 장비시 데미지 %d퍼센트 증가 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpTwoHandSwordImproveDamage);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "양손검 장비시 데미지 15페센트 증가 %d", lpObj->SetOpTwoHandSwordImproveDamage);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;


		case AT_SET_OPTION_IMPROVE_MINATTACKDAMAGE:
			{	//	최소 공격력 증가
				lpObj->SetOpAddMinAttackDamage += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 최소 공격력 증가 %d, %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddMinAttackDamage);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "최소 공격력 증가 %d, %d", ivalue, lpObj->SetOpAddMinAttackDamage);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_MAXATTACKDAMAGE:
			{	//	최대 공격력 증가
				lpObj->SetOpAddMaxAttackDamage += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 최대 공격력 증가 %d, %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddMaxAttackDamage);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "최대 공격력 증가 %d, %d", ivalue, lpObj->SetOpAddMaxAttackDamage);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_MAGICDAMAGE:
			{	//	마력 증가
				lpObj->SetOpAddMagicPower += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 마력 증가 %d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddMagicPower);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "마력 증가 %d %d", ivalue, lpObj->SetOpAddMagicPower);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_DAMAGE:
			{	//	데미지 증가
				lpObj->SetOpAddDamage += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 데미지 증가 %d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddDamage);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "데미지 증가 %d %d", ivalue, lpObj->SetOpAddDamage);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_ATTACKRATE:
			{	//	공격율 증가
				lpObj->SetOpImproveSuccessAttackRate += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 공격율 증가 %d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpImproveSuccessAttackRate);
#endif


#if TESTSERVER == 1
				wsprintf(szText, "공격율 증가 %d %d", ivalue, lpObj->SetOpImproveSuccessAttackRate);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_DEFENCE:
			{	//	방어력 증가
				lpObj->SetOpAddDefence += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 방어력 증가 %d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddDefence);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "방어력 증가 %d %d", ivalue, lpObj->SetOpAddDefence);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_DEFENCE_IGNORE:
			{	//	적 방어력 무시
				lpObj->SetOpIgnoreDefense += ivalue;			
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 적 방어력 무시 %d퍼센트 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpIgnoreDefense);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "적 방어력 무시 %d퍼센트 %d", ivalue, lpObj->SetOpIgnoreDefense);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_MAXLIFE:
			{	//	최대 생명력 증가
				lpObj->AddLife += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 최대 생명력 증가 +%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->AddLife);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "최대 생명력 증가 +%d %d", ivalue, lpObj->AddLife);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;
		
		case AT_SET_OPTION_IMPROVE_MAXMANA:
			{	//	최대 마나 증가
				lpObj->AddMana += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 최대 마나 증가 +%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->AddMana);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "최대 마나 증가 +%d %d", ivalue, lpObj->AddMana);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;
		
		case AT_SET_OPTION_IMPROVE_MAXAG:
			{	//	최대 AG 증가		            	(서버에서 정보를 받는다.)
				lpObj->AddBP += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 최대 AG %d퍼센트 증가 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->AddBP);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "최대 AG %d퍼센트 증가 %d", ivalue, lpObj->AddBP);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_AG_VALUE:
			{	//	AG증가량 증가
				lpObj->SetOpIncAGValue += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] AG증가량 %d 증가 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpIncAGValue);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "AG증가량 %d 증가 %d", ivalue, lpObj->SetOpIncAGValue);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		
		case AT_SET_OPTION_IMPROVE_CRITICALDAMAGE:
			{	//	크리티컬 데미지 증가
				lpObj->SetOpAddCriticalDamage += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 크리티컬 데미지 %d증가 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddCriticalDamage);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "크리티컬 데미지 %d증가 %d", ivalue, lpObj->SetOpAddCriticalDamage);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_CRITICALDAMAGE_SUCCESS:
			{	//	크리티컬 데미지 확률 증가
				lpObj->SetOpAddCriticalDamageSuccessRate += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 크리티컬 데미지 확률 %d증가 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddCriticalDamageSuccessRate);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "크리티컬 데미지 확률 %d증가 %d", ivalue, lpObj->SetOpAddCriticalDamageSuccessRate);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;


		case AT_SET_OPTION_IMPROVE_EX_DAMAGE:
			{	//	엑설런트  데미지 증가
				lpObj->SetOpAddExDamage += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 엑설런트 데미지 %d증가 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddExDamage);
#endif


#if TESTSERVER == 1
				wsprintf(szText, "엑설런트 데미지 %d증가 %d", ivalue, lpObj->SetOpAddExDamage);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_EX_DAMAGE_SUCCESS:
			{	//	엑설런트 데미지 확률 증가
				lpObj->SetOpAddExDamageSuccessRate += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 엑설런트 데미지 확률 %d증가 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddExDamageSuccessRate);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "엑설런트 데미지 확률 %d증가 %d", ivalue, lpObj->SetOpAddExDamageSuccessRate);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_SKILLDAMAGE:
			{	//	스킬 공격력 증가
				lpObj->SetOpAddSkillAttack += ivalue;
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 스킬 공격력 %d증가 %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddSkillAttack);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "스킬 공격력 %d증가 %d", ivalue, lpObj->SetOpAddSkillAttack);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;


		case AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_DEX:
			{	//	특정 캐릭터 공격력 증가 민첩성/X
				lpObj->SetOpAddAttackDamage += ((lpObj->Dexterity+lpObj->AddDexterity)/ivalue);
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 공격력 증가 민첩성/%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddAttackDamage);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 공격력 증가 민첩성/%d %d", ivalue, lpObj->SetOpAddAttackDamage);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_STR:
			{	//	특정 캐릭터 공격력 증가 힘/x
				lpObj->SetOpAddAttackDamage += ((lpObj->Strength+lpObj->AddStrength)/ivalue);
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 공격력 증가 힘/%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddAttackDamage);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 공격력 증가 힘/%d %d", ivalue, lpObj->SetOpAddAttackDamage);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_MAGICDAMAGE_WITH_ENERGY:
			{	//	특정 캐릭터 마력 증가 에너지/X
				lpObj->SetOpAddMagicPower += ((lpObj->Energy+lpObj->AddEnergy)/ivalue);
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 마력 증가 에너지/%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddMagicPower);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 마력 증가 에너지/%d %d", ivalue, lpObj->SetOpAddMagicPower);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_DEFENCE_WITH_DEX:
			{	//	특정 캐릭터 방어력 증가 민첩/X
				lpObj->SetOpAddDefence += (lpObj->Dexterity+lpObj->AddDexterity/ivalue);
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 방어력 증가 레벨/%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddDefence);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 방어력 증가 레벨/%d %d", ivalue, lpObj->SetOpAddDefence);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_IMPROVE_DEFENCE_WITH_VIT:
			{	//	특정 캐릭터 방어력 증가 체력/40	    (●)
				lpObj->SetOpAddDefence += ((lpObj->Vitality+lpObj->AddVitality)/ivalue);
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 방어력 증가 체력/%d %d", lpObj->AccountID, lpObj->Name, ivalue, lpObj->SetOpAddDefence);
#endif

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 방어력 증가 체력/%d %d", ivalue, lpObj->SetOpAddDefence);
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_FIRE_MASTERY:
			{	//	특정 캐릭터 불속성 추가
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 불속성 추가", lpObj->AccountID, lpObj->Name);
#endif
				lpObj->m_AddResistance[RESISTANCE_FIRE] += ivalue;

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 불속성 추가");
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_ICE_MASTERY:
			{	//	특정 캐릭터 얼음속성 추가
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 얼음속성 추가", lpObj->AccountID, lpObj->Name);
#endif
				lpObj->m_AddResistance[RESISTANCE_COLD] += ivalue;

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 얼음속성 추가");
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_THUNDER_MASTERY:
			{	//	특정 캐릭터 번개속성 추가
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 번개속성 추가", lpObj->AccountID, lpObj->Name);
#endif
				lpObj->m_AddResistance[RESISTANCE_THUNDER] += ivalue;

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 번개속성 추가");
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_POSION_MASTERY:
			{	//	특정 캐릭터 독속성 추가		      
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 독속성 추가", lpObj->AccountID, lpObj->Name);
#endif
				lpObj->m_AddResistance[RESISTANCE_POISON] += ivalue;

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 독속성 추가");
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_WATER_MASTERY:
			{	//	특정 캐릭터 물속성 추가			
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 물속성 추가", lpObj->AccountID, lpObj->Name);
#endif
				lpObj->m_AddResistance[RESISTANCE_WATER] += ivalue;

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 물속성 추가");
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_WIND_MASTERY:
			{	//	특정 캐릭터 바람속성 추가			    
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 바람속성 추가", lpObj->AccountID, lpObj->Name);
#endif
				lpObj->m_AddResistance[RESISTANCE_WIND] += ivalue;

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 바람속성 추가");
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;

		case AT_SET_OPTION_EARTH_MASTERY:
			{	//	특정 캐릭터 땅속성 추가
#ifndef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[%s][%s] 특정 캐릭터 땅속성 추가", lpObj->AccountID, lpObj->Name);
#endif
				lpObj->m_AddResistance[RESISTANCE_EARTH] += ivalue;

#if TESTSERVER == 1
				wsprintf(szText, "특정 캐릭터 땅속성 추가");
				GCServerMsgStringSend(szText, lpObj->m_Index, 1);	
#endif
			}
		break;
	}
}

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	void gObjSetItemApply(LPOBJECTSTRUCT lpObj, int type)		// type - 0 : 값증가 옵션 처리, 1 : %증가 옵션 처리
#else
	void gObjSetItemApply(LPOBJECTSTRUCT lpObj)
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
{
	// 추가 공격력 적용
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	if ( type == 0 )
	{
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
	lpObj->m_AttackDamageMinLeft += lpObj->SetOpAddAttackDamage;	
	lpObj->m_AttackDamageMaxLeft += lpObj->SetOpAddAttackDamage;
	lpObj->m_AttackDamageMinRight += lpObj->SetOpAddAttackDamage;
	lpObj->m_AttackDamageMaxRight += lpObj->SetOpAddAttackDamage;


	lpObj->m_AttackDamageMinLeft += lpObj->SetOpAddMinAttackDamage;	
	lpObj->m_AttackDamageMinRight += lpObj->SetOpAddMinAttackDamage;

	lpObj->m_AttackDamageMaxLeft += lpObj->SetOpAddMaxAttackDamage;	
	lpObj->m_AttackDamageMaxRight += lpObj->SetOpAddMaxAttackDamage;

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	if ( type == 1 )
	{
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
	lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin*lpObj->SetOpAddMagicPower/100;
	lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax*lpObj->SetOpAddMagicPower/100;
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519

	
#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	if ( type == 0 )
	{
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
	lpObj->AddLife += (int)(lpObj->AddVitality*DCInfo.DefClass[lpObj->Class].VitalityToLife);
	lpObj->AddMana += (int)(lpObj->AddEnergy*DCInfo.DefClass[lpObj->Class].EnergyToMana);

	lpObj->m_CriticalDamage += lpObj->SetOpAddCriticalDamageSuccessRate;
	lpObj->m_ExcelentDamage += lpObj->SetOpAddExDamageSuccessRate;
	
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsSetItem() )
	{
		lpObj->pInventory[EQUIPMENT_RING_RIGHT].PlusSpecialSetRing(lpObj->m_AddResistance);
	}

	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsSetItem() )
	{
		lpObj->pInventory[EQUIPMENT_RING_LEFT].PlusSpecialSetRing(lpObj->m_AddResistance);
	}

	if( lpObj->pInventory[EQUIPMENT_AMULET].IsSetItem() )
	{
		lpObj->pInventory[EQUIPMENT_AMULET].PlusSpecialSetRing(lpObj->m_AddResistance);
	}

	if( lpObj->SetOpTwoHandSwordImproveDamage )
	{	// 두손검 장착시 데미지 증가
		if( !lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].IsItem() && 
			!lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].IsItem() )
		{	// 무기를 장착안하고 있거나
			lpObj->SetOpTwoHandSwordImproveDamage = 0;
		}

		if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].IsItem() && 
				 !lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_TwoHand )
		{	// 아이템을 차고 있는데 두손 무기가 아니라면 
			lpObj->SetOpTwoHandSwordImproveDamage = 0;
		}

		if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].IsItem() && 
				 !lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_TwoHand )
		{
			lpObj->SetOpTwoHandSwordImproveDamage = 0;
		}
	}

#ifndef MODIFY_ITEM_EFFECT_FLOW_20060519 // not define : 나중에 최종적으로 계산하게 되어 있음
#ifdef MODIFY_ADDLIFE_BUGFIX_20080730
	if( lpObj->m_bMasterLevelDBLoad == TRUE && lpObj->ThirdChangeUp == 0)
#endif	// MODIFY_ADDLIFE_BUGFIX_20080730
	if( lpObj->Life > (lpObj->MaxLife+lpObj->AddLife) )
	{
		lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)lpObj->iShield);
#else
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0);
#endif
	}

#ifndef ADD_SHIELD_POINT_01_20060403	// not define 주의
	GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0);
#endif

	gObjSetBP(lpObj->m_Index);

	#ifndef ADD_SHIELD_POINT_01_20060403	// not define 주의
	GCManaSend(lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
	#endif // ADD_SHIELD_POINT_01_20060403

	if( lpObj->Mana > (lpObj->MaxMana+lpObj->AddMana) )
	{	// 현재 마나가 최대 생명보다 크면 원래로 돌린다
		lpObj->Mana = (lpObj->MaxMana+lpObj->AddMana);
		GCManaSend(lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, lpObj->BP);		
	}	
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519


#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519


#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	if ( type == 1 )
	{
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519
		
	// 추가 방어력 적용
	lpObj->m_Defense += (lpObj->SetOpAddDefence*10/20);
	// 추가 방어력 (%) 적용
	lpObj->m_Defense += (lpObj->m_Defense*lpObj->SetOpAddDefenceRate/100);

	if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type  >= ITEM_SHIELD && 
		lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type  < ITEM_HELM )
	{	// 세트 옵션 방패 장비시 방어력 5% 증가
		lpObj->m_Defense += lpObj->m_Defense*lpObj->SetOpImproveSheldDefence/100;
	}

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	}
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519

#ifndef BUGFIX_MASTERSKILL_INC_DEFENSE_BUGFIX_20080917	// 방어력 증가값을 위에서 적용
#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107		// 패시브 2차 - 방어력 증가
	// ( 기본 방어력 + 아이템 방어력 ) + 패시브 스킬값
	lpObj->m_Defense += lpObj->m_MPSkillOpt.iMpsDefence;
#endif
#endif	// BUGFIX_MASTERSKILL_INC_DEFENSE_BUGFIX_20080917
}
#endif

#ifdef MODIFY_SETITEM_BUGFIX_20080731
BOOL gObjValidItem(LPOBJECTSTRUCT lpObj, CItem *lpItem, int pos, LPMU_SETITEMINFO lpSetItemInfo)
#else
BOOL gObjValidItem(LPOBJECTSTRUCT lpObj, CItem *lpItem, int pos)
#endif // MODIFY_SETITEM_BUGFIX_20080731
{	
	if(g_GlobalConfig.m_bCheckItem == false)
		return TRUE;

	// 검부터 부츠까지는 힘과 민첩만 체크한다.
	// 능력치 체크
	ITEM_ATTRIBUTE *p = &ItemAttribute[lpItem->m_Type];	
	if(p->RequireStrength)
	{
#ifdef MODIFY_SETITEM_BUGFIX_20080731
		short nAddStrength = GetItemAddAbility(lpObj, lpItem, AT_SET_OPTION_IMPROVE_STRENGTH, lpSetItemInfo);
		if( (lpObj->Strength+lpObj->AddStrength - nAddStrength) < lpItem->m_RequireStrength - lpItem->m_HJOpStrength ) 
			return FALSE;
#else
	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		if( lpObj->Strength+lpObj->AddStrength < lpItem->m_RequireStrength - lpItem->m_HJOpStrength ) 
			return FALSE;
	#else
		if( lpObj->Strength+lpObj->AddStrength < lpItem->m_RequireStrength ) 
			return FALSE;
	#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#endif // MODIFY_SETITEM_BUGFIX_20080731
	}
	
	if(p->RequireDexterity)
	{
#ifdef MODIFY_SETITEM_BUGFIX_20080731
		short nAddDexterity = GetItemAddAbility(lpObj, lpItem, AT_SET_OPTION_IMPROVE_DEXTERITY, lpSetItemInfo);
		if( (lpObj->Dexterity+lpObj->AddDexterity - nAddDexterity) < lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity ) 
			return FALSE;
#else
	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		if( lpObj->Dexterity+lpObj->AddDexterity < lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity ) 
			return FALSE;
	#else
		if( lpObj->Dexterity+lpObj->AddDexterity < lpItem->m_RequireDexterity) 
			return FALSE;
	#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#endif // MODIFY_SETITEM_BUGFIX_20080731
	}	
	
	if(p->RequireEnergy)
	{
		if( lpObj->Energy+lpObj->AddEnergy < lpItem->m_RequireEnergy ) return FALSE;
	}	

#ifdef MODIFY_SETITEM_LEADERSHIP_BUGFIX
	//통솔 요구치 적용(다크스피릿만)
	if (p->RequireLeadership)
	{
		if (lpObj->Leadership+lpObj->AddLeadership < lpItem->m_RequireLeaderShip) return FALSE;
	}
#endif // MODIFY_SETITEM_LEADERSHIP_BUGFIX

	if(p->RequireLevel)
	{
		if( lpObj->Level < lpItem->m_RequireLevel ) return FALSE;
	}
	
#ifdef EXPAND_ITEMSCRIPT_01_20050808
	if( lpItem->m_Type >= ITEM_SWORD && lpItem->m_Type <= ITEM_BOOT )
	{
		if(p->RequireVitality)
		{
			if( lpObj->Vitality+lpObj->AddVitality < lpItem->m_RequireVitality ) return FALSE;
		}

		if(p->RequireLeadership)
		{
			if( lpObj->Leadership+lpObj->AddLeadership < lpItem->m_RequireLeaderShip ) return FALSE;
		}
	}
#endif


	return TRUE;
}

#ifdef ADD_SHIELD_POINT_01_20060403
void gObjCalcShieldPoint(LPOBJECTSTRUCT lpObj)
{
#ifdef UPDATE_SHIELD_CALCULATE_GAGE_20060428
	// 공식 : (힘+민첩+체력+에너지+통솔)*1.2 + (레벨*레벨)/30 + 총방어력

	int iMaxShieldPoint = 0;
	int iExpressionA	= (lpObj->Strength+lpObj->AddStrength) 
		+ (lpObj->Dexterity+lpObj->AddDexterity)
		+ (lpObj->Vitality+lpObj->AddVitality)
		+ (lpObj->Energy+lpObj->AddEnergy);

	#ifdef DARKLORD_WORK
		if ( lpObj->Class == CLASS_DARKLORD )
		{
			iExpressionA += ( lpObj->Leadership + lpObj->AddLeadership );
		}
	#endif

	int iExpressionB;
	if ( g_iShieldGageConstB == 0 )
		g_iShieldGageConstB = 30;
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 종합레벨 적용 - 최대 SD
	int iTotalLevel = lpObj->Level + lpObj->m_nMasterLevel;
	iExpressionB = ( iTotalLevel * iTotalLevel ) / g_iShieldGageConstB;
#else
	iExpressionB = ( lpObj->Level * lpObj->Level ) / g_iShieldGageConstB;
#endif

	if ( g_iShieldGageConstA == 0 )
		g_iShieldGageConstA = 12;

	iMaxShieldPoint		= iExpressionA*g_iShieldGageConstA/10 + iExpressionB + lpObj->m_Defense;
	lpObj->iMaxShield	= iMaxShieldPoint;

#else
	int iMaxShieldPoint = 0;
	int iExpressionA	= int( ( lpObj->Vitality + lpObj->AddVitality ) * 5 );
	int iExpressionB	= int( ( lpObj->Level * lpObj->Level ) / g_iConstNumberOfShieldPoint );
	iMaxShieldPoint		= iExpressionA + iExpressionB + lpObj->m_Defense;
	lpObj->iMaxShield	= iMaxShieldPoint;
#endif // UPDATE_SHIELD_CALCULATE_GAGE_20060428
	
// CODEREVIEW_SHIELD : gObjSetCharacter 에서만 AG와 마찬가지로 한번 세팅 되면 된다.
	//lpObj->iShield = lpObj->iMaxShield;

#ifndef UPDATE_SHIELD_OPTION_20060427
	LogAddTD("[ShieldSystem][CalcSDPoint] [%s][%s] user SD Gage : %d", lpObj->AccountID, lpObj->Name, lpObj->iMaxShield );
#endif 

#ifdef SHIELD_POINT_DEBUGLOG
	char Msg[256];
	sprintf(Msg, "[Shield] Shield Point : %d", lpObj->iMaxShield);
	GCServerMsgStringSend( Msg, lpObj->m_Index, 1 );
#endif
}
#endif

#ifdef MODIFY_SETITEM_BUGFIX_20080731
short GetItemAddAbility(LPOBJECTSTRUCT lpObj, CItem *lpItem, int nOptionType, LPMU_SETITEMINFO lpSetItemInfo)
{
	short nAddValue = 0;
	LPMU_SETITEMOPTION lpSetOption = NULL;

	int nSetOption = lpItem->IsSetItem();
	if (nSetOption) 
	{
		int OptionTableIndex = gSetItemOption.GetOptionTableIndex(lpItem->m_Type, nSetOption);
		if (OptionTableIndex != -1)
		{
			for (int i = 0; i < lpSetItemInfo->nCount; ++i)
			{
				if (lpSetItemInfo->table[i].nOptionTableIndex == OptionTableIndex) {
					lpSetOption = &lpSetItemInfo->table[i];
					break;
				}
			}
		}

		if (lpSetOption == NULL)
			return nAddValue;

		for (int i = 0; i < MAX_SETOPTION; ++i)
		{
			if (lpSetOption->option[i].nOption1 == nOptionType) {
				nAddValue += lpSetOption->option[i].nValue1;
			}

			if (lpSetOption->option[i].nOption2 == nOptionType) {
				nAddValue += lpSetOption->option[i].nValue2;
			}
		}
		
		if (lpSetOption->bFullSetItem == TRUE) {
			for (int i = 0; i < MAX_FULL_SETOPTION; ++i)
			{
				if (lpSetOption->fulloption[i].nOption == nOptionType) {
					nAddValue += lpSetOption->fulloption[i].nValue;
				}
			}
		}
	} 
/*
	else 
	{
		lpItem->SetItemPlusSpecialStat(&nAddValue, AT_SET_IMPROVE_STRENGTH);
	}
*/
	return nAddValue;
}

void GetSetItemInfo(LPOBJECTSTRUCT lpObj, LPMU_SETITEMINFO lpSetItemInfo)
{
	BYTE SetOptionTable[MAX_EQUIPMENT*2+5], SetOptionCountTable[MAX_EQUIPMENT*2+5];
	int  SetOptionCount = 0;
	int  nIndex = 0;

	memset(SetOptionTable, 0, sizeof(SetOptionTable));
	memset(SetOptionCountTable, 0, sizeof(SetOptionCountTable));

	gObjGetSetItemOption(lpObj, SetOptionTable, SetOptionCountTable, &SetOptionCount);

	int	op1, op2, op3, op4, op5;
	int	opvalue1, opvalue2, opvalue3, opvalue4, opvalue5;
	for( int optioncount = 0; optioncount < SetOptionCount; optioncount++ )
	{
		int OptionTableIndex = SetOptionTable[optioncount];
		int	OptionTableCount = SetOptionCountTable[optioncount];		

		if( OptionTableCount >= 2 )
		{
			LPMU_SETITEMOPTION lpSetItemOption = &lpSetItemInfo->table[nIndex];

			lpSetItemOption->nOptionTableIndex = OptionTableIndex;
			lpSetItemOption->nOptionTableCount = OptionTableCount;

			for( int  tablecnt = 0; tablecnt < OptionTableCount-1; tablecnt++ )
			{
				if( gSetItemOption.GetSetOption(OptionTableIndex, tablecnt, op1, op2, opvalue1, opvalue2, lpObj->Class, lpObj->ChangeUP) )
				{
					lpSetItemOption->option[tablecnt].nOption1 = op1;
					lpSetItemOption->option[tablecnt].nValue1 = opvalue1;
					lpSetItemOption->option[tablecnt].nOption2 = op2;
					lpSetItemOption->option[tablecnt].nValue2 = opvalue2;
				}
			}

			if( gSetItemOption.GetMaxSetOptionCount(OptionTableIndex) < OptionTableCount )
			{
				gSetItemOption.GetGetFullSetOption(OptionTableIndex, op1, op2, op3, op4, op5,  
												   opvalue1, opvalue2, opvalue3, opvalue4, opvalue5,
												   lpObj->Class, lpObj->ChangeUP);

				lpSetItemOption->fulloption[0].nOption	= op1;
				lpSetItemOption->fulloption[0].nValue	= opvalue1;
				lpSetItemOption->fulloption[1].nOption	= op2;
				lpSetItemOption->fulloption[1].nValue	= opvalue2;
				lpSetItemOption->fulloption[2].nOption	= op3;
				lpSetItemOption->fulloption[2].nValue	= opvalue3;
				lpSetItemOption->fulloption[3].nOption	= op4;
				lpSetItemOption->fulloption[3].nValue	= opvalue4;
				lpSetItemOption->fulloption[4].nOption	= op5;
				lpSetItemOption->fulloption[4].nValue	= opvalue5;

				lpSetItemOption->bFullSetItem = TRUE;
			}

			++nIndex;
			lpSetItemInfo->nCount = nIndex;
		}
	}
}

#endif // MODIFY_SETITEM_BUGFIX_20080731