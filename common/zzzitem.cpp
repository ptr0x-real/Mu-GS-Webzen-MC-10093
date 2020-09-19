#include "stdafx.h"
#include <math.h>
#include "..\\Common\\LogProc.h"
#include "..\\Include\\Define.h"
#include "..\\Include\\ItemDef.h"
#include "..\\Include\\ReadScript.h"
#include "..\\common\\Winutil.h"
#include "ZzzItem.h"

#ifdef SCRIPT_DECODE_WORK
#include "..\\common\\WZScriptEncode.h"
#endif

#include "..\\common\\SetItemOption.h"

#ifdef AUTH_GAMESERVER
#include "..\\common\\WzMemScript.h"
#endif

#ifndef CHARACTERDB_SERVER
#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#include "ItemSocketOptionDefine.h"
extern CItemSocketOptionSystem g_SocketOptionSystem;
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#endif

#ifdef DARKLORD_WORK
CPetItemExp	gPetItemExp;
#endif

#ifdef ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808
int g_MaxItemIndexOfEachItemType[MAX_ITEM_TYPE] = { 0, };
#endif


//extern void GCServerMsgStringSend(char *szMsg, int aIndex, BYTE type);

ITEM_ATTRIBUTE      ItemAttribute[MAX_ITEM];

//----------------------------------------------------------------------------
// ������ ������ �ʱ�ȭ �Ѵ�.
CItem::CItem()
{
	Clear();
}

void CItem::Clear()
{
	m_Type = -1;
	m_Level = 0;
	m_Part = 0;
	m_Class = 0;
	m_TwoHand = 0;
	m_AttackSpeed = 0;
	m_DamageMin = 0;
	m_DamageMax = 0;
	m_SuccessfulBlocking = 0;
	m_Defense = 0;
	m_MagicDefense = 0;
	m_Durability = 0;
	m_SpecialNum = 0;
	m_Value = 0;
	m_Option1 = 0;
	m_Option2 = 0;
	m_Option3 = 0;
	m_NewOption = 0;
	m_Number = 0;
	m_DurabilitySmall = 0;

#ifdef PERSONAL_SHOP_20040113
	m_iPShopValue = -1;			// �� ���� -1 �̸� ������ �ȸ��� �ʴ´�. (����ڰ� ���� �������� ���� ���̹Ƿ�)
#endif

#ifdef ITEMDELETE_MODIFY_20040604	// ������ �Ӽ� �� �������� �����ϴ� �������ΰ� �ƴѰ��� ���� �÷��� (�ʱ�ȭ)
	m_bItemExist = true;
#endif


	m_CurrentDurabilityState = -1.f;

	m_SetOption = 0;

#ifdef NEW_SKILL_FORSKYLAND
	m_QuestItem = 0;		// ����Ʈ ������?
#endif	


	memset(m_Special, 0, MAX_ITEM_SPECIAL);
	memset(m_SpecialValue, 0, MAX_ITEM_SPECIAL);

	memset(m_Resistance, 0, MAX_RESISTANCE);

#ifdef DARKLORD_WORK	// ��ũ���Ǹ�, ��ũȣ�� ���� ��(�����װ���ġ�� ������) ������ ���� 
	m_IsLoadPetItemInfo = FALSE;
	m_PetItem_Level = 1;
	m_PetItem_Exp = 0;
	m_Leadership = 0;
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	m_JewelOfHarmonyOption = 0;		// ��ȭ�Ǻ����� ���� �߰� �ɼ�
#endif

#ifdef ADD_380ITEM_NEWOPTION_20060711
	m_ItemOptionEx = 0; // ������ �߰� �ɼ� : ���� 1bit 380 ������ �߰��ɼ�
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	m_BonusSocketOption = SOCKETSLOT_NONE;
	memset(m_SocketOption, SOCKETSLOT_NONE, MAX_SOCKET_SLOT);
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	m_ImproveDurabilityRate = 0;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
}

//----------------------------------------------------------------------------
// �������� �����ϴ°�?
BOOL CItem::IsItem()
{
	if (m_Type < 0) return FALSE;

	return TRUE;
}

// ��Ʈ�������ΰ�?
BOOL CItem::IsSetItem()
{
	if (m_Type < 0) return FALSE;

	return m_SetOption & 0x03;
}

//----------------------------------------------------------------------------
// �⺻�Ӽ��� ������� �������� �����.
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
void CItem::Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE Attribute2, BYTE SetOption, BYTE ItemEffectEx, BYTE SocketOption[], BYTE SocketBonusOption, BYTE DbVersion)
#else
#ifdef ADD_380ITEM_NEWOPTION_20060711
void CItem::Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE Attribute2, BYTE SetOption, BYTE ItemEffectEx, BYTE DbVersion)
#else	
void CItem::Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE Attribute2, BYTE SetOption, BYTE DbVersion)
#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
{
	int _type = type;
	int ItemLevel;

	if (DbVersion == 0x00)
	{
		_type = ((type / 16)*MAX_ITEM_INDEX) + (type % 16);
	}

#ifdef ITEM_DBSIZE_EXTEND_20050706
#ifdef ITEM_INDEX_EXTEND_20050706
	else if (DbVersion <= 0x02)
	{
		_type = ((type / 32)*MAX_ITEM_INDEX) + (type % 32);
	}
#endif
#endif

	if (_type > MAX_ITEM - 1)
	{
		LogAddC(LOGC_RED, "error-L1 : ItemIndex error %d", _type);
	}

	ITEM_ATTRIBUTE *p = &ItemAttribute[_type];

	m_serial = ItemAttribute[_type].Serial;
	m_Type = type;

#ifdef ITEM_INDEX_EXTEND_20050706
	if (DbVersion <= 0x02)
#else
	if (DbVersion == 0x00)
#endif
		m_Type = _type;

	m_NewOption = Attribute2;
	if (p->OptionFlag == 0)
	{
		m_NewOption = 0;
	}
#ifdef NEW_FORSKYLAND2
#ifdef ADD_THIRD_WING_20070525	// 3������ ������Ʈ ó�� ���� �ʴ´�.
	if ((_type >= ITEM_WING + 3 && _type <= ITEM_WING + 6) || (_type == ITEM_HELPER + 30)
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ������ ������Ʈ ó������
		|| (_type == ITEM_WING + 41)	// �ű�ĳ���� 1������(����� ����)
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
		|| (_type == ITEM_WING + 42)	// �ű�ĳ���� 2������(������ ����)
		|| (_type == ITEM_WING + 43)	// �ű�ĳ���� 3������(������ ����)
#endif		
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912		
		|| (_type >= ITEM_WING + 36 && _type <= ITEM_WING + 40)) // ����
#else	
#ifdef DARKLORD_WORK
	if ((_type >= ITEM_WING + 3 && _type <= ITEM_WING + 6) || (_type == ITEM_HELPER + 30)) // ����	    
#else
	if (_type >= ITEM_WING + 3 && _type <= ITEM_WING + 6) // ����	    
#endif
#endif	// ADD_THIRD_WING_20070525
	{
		Attribute2 = 0;
	}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
	// �渱�� ��� �׼���Ʈ ó�� ( ������ ���뷹��, ������ ��) ���� �ʴ´�.
	if (_type == MAKE_ITEMNUM(13, 37))
	{
		Attribute2 = 0;
	}
#endif

#if GAME_VERSION >= G_V_99B
	SetOption = SetOption & 0x0f;

	int	SOption = SetOption & 0x03;					// ��Ʈ �ɼ� Ÿ�� (1:AŸ��, 2:BŸ��)
	int	SOptionStatValue = 0;						// ��Ʈ �ɼ� ���� �� (1:5, 2:10)
	if (SOption != 1 && SOption != 2)
	{	// SetOption == 1 A Option || SetOption == 2 B Option
		SOption = 0;
	}

	if (((SetOption >> 2) & 0x03) != 1 && ((SetOption >> 2) & 0x03) != 2)
	{	// SetOption == 1 A Option || SetOption == 2 B Option
		SOption = 0;
	}

	if (gSetItemOption.IsSetItem(_type) && SetOption != 0xFF && SOption)
	{	// ��Ʈ ������������ �������� �����Ѵ�
		m_SetOption = SetOption;

		if (m_SetOption)
		{	// ��Ʈ�ɼ��϶��� ������Ʈ �ɼ��� ����..
			m_NewOption = 0;
			Attribute2 = 0;

			SOptionStatValue = (SetOption >> 2) & 0x03;		// ��Ʈ �ɼ� ���� �� (1:5, 2:10)
		}
	}
	else
	{
		m_SetOption = 0;
	}
#endif

#ifdef ADD_380ITEM_NEWOPTION_20060711
	m_ItemOptionEx = ItemEffectEx;			// �߰��ɼ� ����. 380�߰��ɼ� ���� 1bit.
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifndef CHARACTERDB_SERVER
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	if (g_SocketOptionSystem.IsEnableSocketItem(this) == true)
	{
		if (SocketOption != NULL)
		{
			m_BonusSocketOption = SocketBonusOption;

			for (int i = 0; i < MAX_SOCKET_SLOT; i++)
			{
				m_SocketOption[i] = SocketOption[i];
			}
		}

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
		if (g_SocketOptionSystem.IsSocketItem(this) == true)
		{
			m_ItemOptionEx = 0;
			m_SetOption = 0;
			Attribute2 = 0;
			m_NewOption = 0;
			m_JewelOfHarmonyOption = 0;
		}
		else
		{
			m_BonusSocketOption = 0;
		}
#else
		// ���� �������� ���� ������Ʈ, ��Ʈ, 380, ��ȭ�ɼ��� ���� �ʴ´�.
		m_ItemOptionEx = 0;
		m_SetOption = 0;
		Attribute2 = 0;
		m_NewOption = 0;
		m_JewelOfHarmonyOption = 0;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	}
	else
	{
		m_BonusSocketOption = 0;
		for (int i = 0; i < MAX_SOCKET_SLOT; i++)
		{
			m_SocketOption[i] = SOCKETSLOT_NONE;
		}
	}
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#endif

	memset(m_SkillResistance, 0, sizeof(m_SkillResistance));

	if (_type == MAKE_ITEMNUM(0, 19) ||
		_type == MAKE_ITEMNUM(4, 18) ||
		_type == MAKE_ITEMNUM(5, 10)
#ifdef ADD_WEAPON_OF_ARCHANGEL_SCEPTER_20050706
		|| _type == MAKE_ITEMNUM(2, 13)
#endif
		)

	{	// ���� ������ ��� ������ ������Ʈ ó���� ���Ѵ�
		Attribute2 = 0;
	}

	int	iChaosItem = 0;
	if (m_Type == MAKE_ITEMNUM(2, 6))		// ī�������ﵵ��

	{
		iChaosItem = 15;
	}
	else if (m_Type == MAKE_ITEMNUM(5, 7))	// ī��������������
	{
		iChaosItem = 25;
	}
	else if (m_Type == MAKE_ITEMNUM(4, 6))	// ī���������ĺ���
	{
		iChaosItem = 30;
	}

	if (ItemGetDurability(m_Type, 0, 0, 0))
	{
		m_BaseDurability = (float)ItemGetDurability(m_Type, m_Level, Attribute2 & 0x7F, m_SetOption);
	}
	else
	{
		m_BaseDurability = 0.f;
	}
	m_DurabilityState[0] = m_BaseDurability * 0.5f;
	m_DurabilityState[1] = m_BaseDurability * 0.3f;
	m_DurabilityState[2] = m_BaseDurability * 0.2f;


	m_Value = p->Value;
	m_AttackSpeed = p->AttackSpeed;
	m_TwoHand = p->TwoHand;
	m_DamageMin = p->DamageMin;
	m_DamageMax = p->DamageMax;
	m_SuccessfulBlocking = p->SuccessfulBlocking;
	m_Defense = p->Defense;
	m_MagicDefense = p->MagicDefense;
	m_WalkSpeed = p->WalkSpeed;
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// å�ϰ�� ���´�� ��������
	// �����̷����� ���ʿ� �����ϸ� å�̴�..
	if ((_type >> ITEM_BITSHIFT) == ITEM_STAFF / MAX_ITEM_INDEX
		&& p->ItemSlot == EQUIPMENT_WEAPON_LEFT
		)
	{
		m_Magic = 0;
		// å�̸� ���ַ·�����
		m_Curse = p->MagicPW;

	}
	else
	{
		m_Magic = p->MagicPW;
		m_Curse = 0;
	}
#else	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	m_Magic = p->MagicPW;
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#ifdef CHARACTERDB_SERVER
	m_Durability = p->Durability;
#endif

	m_serial = p->Serial;

#ifdef NEW_SKILL_FORSKYLAND
	m_QuestItem = p->QuestItem;		// ����Ʈ�� �����۰�?
#endif	

#ifdef EXPAND_ITEMSCRIPT_01_20050808
	m_RequireLeaderShip = 0;
#endif

#ifdef MODIFY_ITEMDURABILITY_BUGFIX_20080408	// �������� ����, ������ ���Ʈ ������ ��� �������� ��� ������ �ִ븦 ������ �ִ밪����
	if (m_BaseDurability > 10
		&& _type != MAKE_ITEMNUM(13, 20)
		&& (_type < MAKE_ITEMNUM(14, 0)
			|| _type > MAKE_ITEMNUM(14, 8))
#if GAME_VERSION >= G_V_S2_2
		&& (_type < MAKE_ITEMNUM(14, 35)
			|| _type > MAKE_ITEMNUM(14, 40))
#endif
		)
	{
		if (m_Durability > m_BaseDurability)
		{
			m_Durability = m_BaseDurability;
		}
	}
#endif	// MODIFY_ITEMDURABILITY_BUGFIX_20080408

#ifdef MODIFY_CALC_DURABILITY_20060523 
	// �ִ� ������ ���� ū ��� ���� �������� �ִ� ������ ����
	// �⺻ �������� 10 �̻��� ��츸 ó��. ������ ����� �������� �������� ������.
	// �Ϲ� ����� �������� �⺻ 10�� �Ѵ´�.
	if (m_BaseDurability > 10)
	{
		if (m_Durability > m_BaseDurability)
		{
			m_Durability = m_BaseDurability;
		}
	}
#endif // MODIFY_CALC_DURABILITY_20060523

	// ������ ���¼���	
	if (m_Durability == 0.f)
		m_CurrentDurabilityState = 1.f;
	else if (m_Durability < m_DurabilityState[2])
		m_CurrentDurabilityState = 0.5f;
	else if (m_Durability < m_DurabilityState[1])
		m_CurrentDurabilityState = 0.3f;
	else if (m_Durability < m_DurabilityState[0])
		m_CurrentDurabilityState = 0.2f;
	else m_CurrentDurabilityState = 0.f;

	memcpy(m_RequireClass, p->RequireClass, MAX_CLASSTYPE);
	//memcpy(m_Resistance,   p->Resistance,   MAX_RESISTANCE);

	for (int i = 0; i < MAX_RESISTANCE; i++)
	{
		m_Resistance[i] = p->Resistance[i] * m_Level;
	}


	//-------------------------------------------------------------------------
	//
	// �� ������ ��밡�� ���� �䱸ġ �⺻ ����
	//		- ������ �߰� �ɼǿ� ���� �䱸ġ ������ �Ʒ��κп��� ó��.
	//
	//-------------------------------------------------------------------------
	ItemLevel = p->Level;

	// 1. > ������Ʈ ������ Ȯ��
	if ((Attribute2 & 63) > 0)
		ItemLevel = p->Level + 25;

	// 2. > ��Ʈ ������ Ȯ��
	else if (m_SetOption)
	{
		ItemLevel = p->Level + 25;
	}

	// 3. > �Ϲ� / ������Ʈ / ��Ʈ ������ ���ο� ���� �䱸ġ ����.
	if (p->RequireStrength)
		m_RequireStrength = 20 + p->RequireStrength *(ItemLevel + m_Level * 3) * 3 / 100;
	else
		m_RequireStrength = 0;

	if (p->RequireDexterity)
		m_RequireDexterity = 20 + p->RequireDexterity*(ItemLevel + m_Level * 3) * 3 / 100;
	else
		m_RequireDexterity = 0;

	if (p->RequireEnergy)
		m_RequireEnergy = 20 + p->RequireEnergy   *(ItemLevel + m_Level * 3) * 4 / 100;
	else
		m_RequireEnergy = 0;

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� å(��)�� ���� �䱸������ ���
	if (p->RequireEnergy)
	{
		if ((_type >> ITEM_BITSHIFT) == ITEM_STAFF / MAX_ITEM_INDEX
			&& p->ItemSlot == EQUIPMENT_WEAPON_LEFT
			)
		{
			m_RequireEnergy = 20 + p->RequireEnergy   *(ItemLevel + m_Level * 1) * 3 / 100;
		}
	}
	else
	{
		m_RequireEnergy = 0;
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#ifdef EXPAND_ITEMSCRIPT_01_20050808
	if (_type >= ITEM_SWORD && _type < ITEM_WING)
	{
		if (p->RequireVitality)
			m_RequireVitality = 20 + p->RequireVitality *(ItemLevel + m_Level * 3) * 3 / 100;
		else
			m_RequireVitality = 0;

		if (p->RequireLeadership)
			m_RequireLeaderShip = 20 + p->RequireVitality *(ItemLevel + m_Level * 3) * 3 / 100;
		else
			m_RequireLeaderShip = 0;
	}
#endif

	// 4. > ��ũ ���Ǹ� �䱸 ��� ���� ó��
#ifdef EXPAND_ITEMSCRIPT_01_20050808
	// ��ũ���Ǹ��� �ڵ忡 ���� �ִ�.
#ifdef MODIFY_SETITEM_LEADERSHIP_BUGFIX
	if (_type == MAKE_ITEMNUM(13, 5)) {
		// gObjValidItem ���� ��ֿ䱸ġ üũ ���� ����
		// ��ũ���Ǹ��� item(kor).txt �� �䱸��ġ�� �������� ���Ŀ� ���� ���� ó���Ѵ�.
		m_RequireLeaderShip = 185 + (m_PetItem_Level * 15);
		p->RequireLeadership = 185 + (m_PetItem_Level * 15);
	}
#else
	if (_type == MAKE_ITEMNUM(13, 5))
		m_RequireLeaderShip = 185 + (m_PetItem_Level * 15);
#endif // MODIFY_SETITEM_LEADERSHIP_BUGFIX


#else
#ifdef DARKLORD_WORK
	if (_type == MAKE_ITEMNUM(13, 5)) // ��ũ���Ǹ��� �䱸 ����� �ʿ� �ϴ�
		m_RequireLeaderShip = 185 + (m_PetItem_Level * 15);
	else
		m_RequireLeaderShip = 0;
#endif
#endif // EXPAND_ITEMSCRIPT_01_20050808

#ifndef CHARACTERDB_SERVER
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// ��ȭ�� ���� ��ȭ ������ �ɼ� : �ʿ���/��ø ����
	if (g_kJewelOfHarmonySystem.IsActive(this) == TRUE)
	{
		BYTE btOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(this);
#ifdef MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729
		int iItemType = g_kJewelOfHarmonySystem._GetItemType(this);

		if (iItemType == JEWELOFHARMONY_ITEM_TYPE_WEAPON)
		{
			if (btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR)
				m_HJOpStrength = g_kJewelOfHarmonySystem.GetItemEffectValue(this, AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR);
			else if (btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX)
				m_HJOpDexterity = g_kJewelOfHarmonySystem.GetItemEffectValue(this, AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX);
		}
		else if (iItemType == JEWELOFHARMONY_ITEM_TYPE_STAFF)
		{
			if (btOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR)
				m_HJOpStrength = g_kJewelOfHarmonySystem.GetItemEffectValue(this, AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR);
			else if (btOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX)
				m_HJOpDexterity = g_kJewelOfHarmonySystem.GetItemEffectValue(this, AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX);
		}
#else
		if (btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR)
			m_HJOpStrength = g_kJewelOfHarmonySystem.GetItemEffectValue(this, AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR);
		else if (btOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR)
			m_HJOpStrength = g_kJewelOfHarmonySystem.GetItemEffectValue(this, AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR);
		else if (btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX)
			m_HJOpDexterity = g_kJewelOfHarmonySystem.GetItemEffectValue(this, AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX);
		else if (btOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX)
			m_HJOpDexterity = g_kJewelOfHarmonySystem.GetItemEffectValue(this, AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX);
#endif // MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729
	}
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
	else
	{
		m_HJOpStrength = 0;
		m_HJOpDexterity = 0;
	}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	g_SocketOptionSystem.SetRequireStatOption(this);
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
#endif
	// 5. > �䱸 ���� ó��
	if (p->RequireLevel)
	{

#ifdef DARKLORD_WORK
		// ��ũȣ��
		if (_type == MAKE_ITEMNUM(13, 4))
		{
			m_RequireLevel = 218 + m_PetItem_Level * 2;
		}
		else
#endif
#ifdef ADD_THIRD_WING_20070525	// 3�� ���������� ���������� ������� 400���� ����
			if (_type >= ITEM_WING + 36 && _type <= ITEM_WING + 40)
			{
				m_RequireLevel = p->RequireLevel;
			}
			else
#endif
				// �߰� ����
				if (_type >= ITEM_WING + 3 && _type <= ITEM_WING + 6)
				{
					m_RequireLevel = p->RequireLevel + m_Level * 5;
				}
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ����, ������ ���� ��������
				else if (_type == ITEM_WING + 42) // ������ ����
				{
					m_RequireLevel = p->RequireLevel + m_Level * 5;
				}
				else if (_type == ITEM_WING + 43) // ������ ����
				{
					m_RequireLevel = p->RequireLevel;
				}
#endif
#ifdef ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
		// ���� ������ �ʸ�..
				else
					if ((_type >= MAKE_ITEMNUM(12, 7)) && (_type <= MAKE_ITEMNUM(12, 24)))
					{

						m_RequireLevel = p->RequireLevel;
					}
#endif
#ifdef EXPAND_ITEMSCRIPT_01_20050808
		// ����, ��
					else
						if (_type >= ITEM_SWORD && _type < ITEM_WING)
						{
							if (p->RequireLevel)
							{
								//m_RequireLevel = 10+p->RequireLevel + (ItemLevel+m_Level*4)*5/100;
								//m_RequireLevel = 10+p->RequireLevel + (p->Level+m_Level*4)*5/100;

								// ���� ������ ��� �Ϲ�/��Ʈ/�׼� ��� ����
								m_RequireLevel = p->RequireLevel;
							}
						}
#endif
		// ����,�����;
						else
						{
							m_RequireLevel = p->RequireLevel + m_Level * 4;
						}

	}
	else
		m_RequireLevel = 0;


	if (m_Type == ITEM_HELPER + 10)//���Ź���
	{
		if (ItemLevel <= 2)
			m_RequireLevel = 20;
		else
			m_RequireLevel = 50;
	}

	if ((Attribute2 & 63) > 0)		// ������Ʈ �ɼ��� �ϳ��� �پ� �ִٸ�
	{
		if (m_RequireLevel > 0)
		{

#ifdef EXPAND_ITEMSCRIPT_01_20050808
			if (_type < ITEM_SWORD || _type >= ITEM_WING)
			{
				// ���� ������ ��� �Ϲ�/��Ʈ/�׼� ��� ����
				m_RequireLevel += 20;
			}
#else
			m_RequireLevel += 20;
#endif

		}
	}
	//-------------------------------------------------------------------------



	//-------------------------------------------------------------------------
	//
	// �� ������ �⺻ ��ġ ����
	//
	//	��Ʈ �������� ��� 
	//	- �䱸�ɷ�ġ ��� �� + 25 ����
	//	- ��ġ ��� �� + 30 ����
	//
	//-------------------------------------------------------------------------

	if (m_SetOption)
	{
		ItemLevel = p->Level + 30;
	}

#ifdef DARKLORD_WORK
	m_Leadership = 0;
#endif

	if (m_DamageMax > 0)
	{
		if (m_SetOption && ItemLevel)
		{	// ��Ʈ ������ ���ݷ�
			m_DamageMax += m_DamageMin * 25 / p->Level + 5;
			m_DamageMax += ItemLevel / 40 + 5;
		}
		else
			if ((Attribute2 & 63) > 0)
			{
				if (iChaosItem)
				{
					m_DamageMax += iChaosItem;
				}
				else if (p->Level)
				{
					m_DamageMax += m_DamageMin * 25 / p->Level + 5;
				}
			}

		m_DamageMax += m_Level * 3;

		if (m_Level >= 10)
		{	// +10 �̻��� ���ݷ� +
#ifdef ITEM_12_13_20040401
			m_DamageMax += (m_Level - 9)*(m_Level - 9 + 1) / 2;
#else
			m_DamageMax += (m_Level - 10) * 2 + 1;
#endif
		}
	}

	if (m_DamageMin > 0)
	{
		if (m_SetOption && ItemLevel)
		{
			m_DamageMin += m_DamageMin * 25 / p->Level + 5;
			m_DamageMin += ItemLevel / 40 + 5;
		}
		else
			if ((Attribute2 & 63) > 0)
			{
				if (iChaosItem)
				{
					m_DamageMin += iChaosItem;
				}
				else if (p->Level)
					m_DamageMin += m_DamageMin * 25 / p->Level + 5;
			}
		m_DamageMin += m_Level * 3;

		if (m_Level >= 10)
		{	// +10 �̻��� ���ݷ� +
#ifdef ITEM_12_13_20040401
			m_DamageMin += (m_Level - 9)*(m_Level - 9 + 1) / 2;
#else
			m_DamageMin += (m_Level - 10) * 2 + 1;
#endif
		}
	}

	if (m_Magic > 0)
	{
		if (m_SetOption && ItemLevel)
		{
			m_Magic += m_Magic * 25 / p->Level + 5;
			m_Magic += ItemLevel / 60 + 2;
		}
		else
			if ((Attribute2 & 63) > 0)
			{
				if (iChaosItem)
				{
					m_Magic += iChaosItem;
				}
				else if (p->Level)
					m_Magic += m_Magic * 25 / p->Level + 5;
			}

		m_Magic += m_Level * 3;

		if (m_Level >= 10)
		{	// +10 �̻��� ���ݷ� +
#ifdef ITEM_12_13_20040401
			m_Magic += (m_Level - 9)*(m_Level - 9 + 1) / 2;
#else
			m_Magic += (m_Level - 10) * 2 + 1;
#endif
		}
	}

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// å�� ���� ���ַ� ���
	if (m_Curse > 0)
	{
		if (m_SetOption && ItemLevel)
		{
			m_Curse += m_Curse * 25 / p->Level + 5;
			m_Curse += ItemLevel / 60 + 2;
		}
		else
			if ((Attribute2 & 63) > 0)
			{
				if (iChaosItem)
				{
					m_Curse += iChaosItem;
				}
				else if (p->Level)
					m_Curse += m_Curse * 25 / p->Level + 5;
			}

		m_Curse += m_Level * 3;

		if (m_Level >= 10)
		{	// +10 �̻��� ���ݷ� +
#ifdef ITEM_12_13_20040401
			m_Curse += (m_Level - 9)*(m_Level - 9 + 1) / 2;
#else
			m_Curse += (m_Level - 10) * 2 + 1;
#endif
		}
	}
#endif

	if (p->SuccessfulBlocking > 0)
	{
		if (m_SetOption && ItemLevel)
		{
			m_SuccessfulBlocking += m_SuccessfulBlocking * 25 / p->Level + 5;
			m_SuccessfulBlocking += ItemLevel / 40 + 5;
		}
		else
			if ((Attribute2 & 63) > 0)
			{
				if (p->Level)
					m_SuccessfulBlocking += m_SuccessfulBlocking * 25 / p->Level + 5;
			}
		m_SuccessfulBlocking += m_Level * 3;

		if (m_Level >= 10)
		{	// +10 �̻��� ���ݷ� +
#ifdef ITEM_12_13_20040401
			m_SuccessfulBlocking += (m_Level - 9)*(m_Level - 9 + 1) / 2;
#else
			m_SuccessfulBlocking += (m_Level - 10) * 2 + 1;
#endif
		}
	}

	if (p->Defense > 0)
	{
		if (m_Type >= ITEM_SHIELD && m_Type < ITEM_SHIELD + MAX_ITEM_INDEX)
		{
			m_Defense += m_Level;

			if (m_SetOption && ItemLevel)
			{
				m_Defense += (m_Defense * 20 / ItemLevel + 2);
			}
		}
		else
		{
			if (m_SetOption && ItemLevel)
			{
				m_Defense += m_Defense * 12 / p->Level + 4 + p->Level / 5;
				m_Defense += m_Defense * 3 / ItemLevel + 2 + ItemLevel / 30;
			}
			else
				if ((Attribute2 & 63) > 0)
				{
					if (p->Level)
						m_Defense += m_Defense * 12 / p->Level + 4 + p->Level / 5;
				}

#ifdef ADD_THIRD_WING_20070525	// 3�������� ���� ����
			// _MODIFY_ADD_THIRD_WING_20070623
			if (_type >= MAKE_ITEMNUM(12, 36) && _type <= MAKE_ITEMNUM(12, 40))
			{
				m_Defense += m_Level * 4;

				if (m_Level >= 10)
				{	// 10�������� �߰� �濩�� ���� +5 ~ +8
					m_Defense += m_Level - 9;
				}
			}
			else
#endif	// ADD_THIRD_WING_20070525
#ifdef DARKLORD_WORK
				if ((_type >= ITEM_WING + 3 && _type <= ITEM_WING + 6)
					|| _type == ITEM_HELPER + 30 || _type == ITEM_HELPER + 4) // �߰� ����&&����&&��ũȣ��
#else
				if (_type >= ITEM_WING + 3 && _type <= ITEM_WING + 6) // �߰� ����
#endif
				{
					m_Defense += m_Level * 2;
				}
				else
				{
					m_Defense += m_Level * 3;
				}

			if (m_Level >= 10)
			{	// +10 �̻��� ���ݷ� +
#ifdef ITEM_12_13_20040401
				m_Defense += (m_Level - 9)*(m_Level - 9 + 1) / 2;
#else
				m_Defense += (m_Level - 10) * 2 + 1;
#endif
			}
		}
	}

#ifdef DARKLORD_WORK
	if (_type == MAKE_ITEMNUM(13, 30))
	{	// ���� ���� ����
		m_Defense = 15 + m_Level * 2;
#ifdef MODIFY_ACCUMULATED_BUG_SLEEVEOFLORD_01_20050531			
		if (m_Level >= 10)
		{
			m_Defense += (m_Level - 9)*(m_Level - 9 + 1) / 2;
		}
#endif

	}
#endif

#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ����� ����, ������ ����, ������ ���� ���� ���
	if (_type == MAKE_ITEMNUM(12, 41))
	{	// ����� ����
		m_Defense += m_Level * 3;
		if (m_Level >= 10)
		{
			m_Defense += (m_Level - 9)*(m_Level - 9 + 1) / 2;
		}
	}
	else if (_type == MAKE_ITEMNUM(12, 42))
	{	// ������ ����
		m_Defense += m_Level * 2;
		if (m_Level >= 10)
		{
			m_Defense += (m_Level - 9)*(m_Level - 9 + 1) / 2;
		}
	}
	else if (_type == MAKE_ITEMNUM(12, 43))
	{	// ������ ����
		m_Defense += m_Level * 4;

		if (m_Level >= 10)
		{	// 10�������� �߰� �濩�� ���� +5 ~ +8
			m_Defense += m_Level - 9;

#ifndef MODIFY_SUMMONER_THIRD_WING_BUFGIX_20090109
			m_Defense += (m_Level - 9)*(m_Level - 9 + 1) / 2;
#endif	// MODIFY_SUMMONER_THIRD_WING_BUFGIX_20090109
		}
	}
#endif	// ADD_SUMMONER_SECOND_THIRD_WING_20071130

	if (p->MagicDefense > 0)
	{
		m_MagicDefense += m_Level * 3;
		if (m_Level >= 10)
		{	// +10 �̻��� ���ݷ� +
#ifdef ITEM_12_13_20040401
			m_MagicDefense += (m_Level - 9)*(m_Level - 9 + 1) / 2;
#else
			m_MagicDefense += (m_Level - 10) * 2 + 1;
#endif
		}
	}
	//-------------------------------------------------------------------------



	//if(p->RequireStrength    > 0)  m_RequireStrength    += (10+(ItemLevel+m_Level*3)*15/10)*p->RequireStrength /10;
	//if(p->RequireDexterity   > 0)  m_RequireDexterity   += (10+(ItemLevel+m_Level*3)*15/10)*p->RequireDexterity/10;

	//int EnableSpecial = m_Level>>4;
	m_Level &= 0xf;

	memset(m_Special, 0, MAX_ITEM_SPECIAL);
	memset(m_SpecialValue, 0, MAX_ITEM_SPECIAL);
	m_SpecialNum = 0;
	m_Option1 = 0;
	m_Option2 = 0;
	m_Option3 = 0;

#ifdef DARKLORD_WORK
	m_SkillChange = FALSE;
	//�ɼ�1(��ų)
	if (Option1)
	{	// ITem.txt�� ��ų���� ��
		if (p->SkillType)
		{
			if (p->SkillType == AT_SKILL_LONGSPEAR)
			{
				m_SkillChange = TRUE;
				m_Special[m_SpecialNum] = 0;
				m_Option1 = 1;
			}
			else
			{
				m_Special[m_SpecialNum] = p->SkillType;
				m_Option1 = 1;
			}
		}
	}
#else
	/*
	//
	//
	//	20051110 DARKLORD_WORK �� ELSE �κ� ���� (apple)
	//
	//
	//
	*/
	if (Option1)
	{	// ITem.txt�� ��ų���� ��
		if (p->SkillType)
		{
			m_Special[m_SpecialNum] = p->SkillType;
			m_Option1 = 1;
		}
	}

#endif // DARKLORD_WORK


	if (m_Type == MAKE_ITEMNUM(13, 3))
	{	// ����Ʈ�� ������ �ɼ��� ��� ����
		m_Option1 = 1;
		m_Special[m_SpecialNum] = AT_SKILL_KNIGHTDINORANT;
	}


#ifdef DARKLORD_WORK
	if (m_Type == MAKE_ITEMNUM(13, 4))
	{	// ��ũȣ���� ������ �ɼ��� ��� ����
		m_Option1 = 1;
		m_Special[m_SpecialNum] = AT_SKILL_DARKHORSE_ATTACK;
	}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
	// �渱�� ������ �ɼ��� ����.
	// item.txt �� SkillType�� ��ų �� �� ������, Convert �Լ��� �������� ��Ȳ�� ȣ��� ���� ����Ѵ�.
	if (m_Type == MAKE_ITEMNUM(13, 37))
	{	// �渱�� ������ �ɼ��� ��� ����
		m_Option1 = 1;
		m_Special[m_SpecialNum] = AT_SKILL_FENRIR_ATTACK;
	}
#endif

	m_SpecialNum++;

	//�ɼ�2(���)
	if (Option2 != 0)
	{
		if (_type >= MAKE_ITEMNUM(0, 0) && _type < MAKE_ITEMNUM(12, 0))
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}

		if (_type >= MAKE_ITEMNUM(12, 0) && _type <= MAKE_ITEMNUM(12, 6))	// Wings
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}

		if (_type >= MAKE_ITEMNUM(12, 36) && _type <= MAKE_ITEMNUM(12, 40))	// Wings s3
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}
#ifdef CUSTOM_WINGS
		if (_type >= MAKE_ITEMNUM(12, 41) && _type <= MAKE_ITEMNUM(12, 45))	// New Wings
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}
#endif
		if (_type == MAKE_ITEMNUM(13, 30))	// Cape of Lord
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_Option2 = 1;
		}
	}
	m_SpecialNum++;

	//�ɼ�3(�߰�����Ʈ)
	if (Option3)
	{

		// ��Ʈ ������ �ɼ�
		{
			if (_type >= ITEM_SWORD && _type < ITEM_BOW + MAX_ITEM_INDEX)
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				m_Option3 = Option3;
				m_RequireStrength += Option3 * 4;
			}
			if (_type >= ITEM_STAFF && _type < ITEM_STAFF + MAX_ITEM_INDEX)
			{
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// å(���� ������) �ϰ�� ���ַ� ����
				if (p->ItemSlot == EQUIPMENT_WEAPON_LEFT)
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_CURSE;
				}
				else
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
				}
#else
				m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
#endif
				m_Option3 = Option3;
				m_RequireStrength += Option3 * 4;
			}
			if (_type >= ITEM_SHIELD && _type < ITEM_SHIELD + MAX_ITEM_INDEX)
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_BLOCKING;
				m_Option3 = Option3;
				m_RequireStrength += Option3 * 4;
			}
			if (_type >= ITEM_HELM && _type < ITEM_BOOT + MAX_ITEM_INDEX)
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_DEFENSE;
				m_Option3 = Option3;
				m_RequireStrength += Option3 * 4;
			}
			if ((_type >= ITEM_HELPER + 8 && _type < ITEM_HELPER + 14) ||
				(_type >= ITEM_HELPER + 20 && _type <= ITEM_HELPER + 28)) //����,�����
			{
				if (_type == ITEM_HELPER + 24)
				{	// ������ ����
					m_Special[m_SpecialNum] = AT_SET_OPTION_IMPROVE_MAXMANA;
					m_Option3 = Option3;
				}
				else if (_type == ITEM_HELPER + 28)
				{	// ����� �����
					m_Special[m_SpecialNum] = AT_SET_OPTION_IMPROVE_MAXAG;
					m_Option3 = Option3;
				}
				else
				{
					m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
					m_Option3 = Option3;
				}
			}


#ifdef DARKLORD_WORK
			if (_type == ITEM_HELPER + 30)
			{	// ����� ���ݷ� ����.
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;
			}
#endif

			if (_type == ITEM_WING)
			{
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
				m_Option3 = Option3;
			}
			else if (_type == ITEM_WING + 1)
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;
			}
			else if (_type == ITEM_WING + 2)
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;
			}
			else if (_type == ITEM_WING + 3)
			{	// ������ ����(����)
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;

				if ((m_NewOption)&PLUS_WING_OP1_TYPE)
				{
					m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
				}
				else
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				}
			}
			else if (_type == ITEM_WING + 4)
			{	// ��ȥ�� ����(����)			
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;

				if ((m_NewOption)&PLUS_WING_OP1_TYPE)
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
				}
				else
				{
					m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
				}
			}
			else if (_type == ITEM_WING + 5)
			{	// ������ ����(���)			
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;

				if ((m_NewOption)&PLUS_WING_OP1_TYPE)
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				}
				else
				{
					m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
				}
			}
			else if (_type == ITEM_WING + 6)
			{	// ����� ����(����)			
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;

				if ((m_NewOption)&PLUS_WING_OP1_TYPE)
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				}
				else
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
				}
			}
			else if (_type == ITEM_HELPER + 3)
			{	// ����Ʈ
				m_Option3 = Option3;

				if ((m_Option3)&PLUS_DINORANT_PLUS_MAX_AG)
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_AG_MAX;	m_SpecialNum++;
				}

				if ((m_Option3)&PLUS_DINORANT_PLUS_ATTACKSPEED)
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_ATTACK_SPEED;	m_SpecialNum++;
					m_AttackSpeed += 5;
				}

				if ((m_Option3)&PLUS_DINORANT_PLUS_REDUCE_DAMAGE)
				{
					m_Special[m_SpecialNum] = AT_DAMAGE_ABSORB;
				}
			}
#ifdef DARKLORD_WORK
			if (_type == ITEM_HELPER + 30)
			{	// ����
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;

#ifdef SLEEVEOFLORD_OPTION_BUG_PATCH
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
#else
				if ((m_NewOption)&PLUS_WING_OP1_TYPE)
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				}
				else
				{
					m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
				}
#endif
			}
#endif
#ifdef ADD_THIRD_WING_20070525	// 3������ �߰��ɼ� ����
			if (_type == ITEM_WING + 36)	// ��ǳ�ǳ���
			{
				m_Option3 = Option3;
				// �⺻�� ���� �ڵ� ȸ��
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;

				if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_1)
				{	// �߰� ���ݷ�
					m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				}
				else if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_2)
				{	// �߰� ����
					m_Special[m_SpecialNum] = AT_IMPROVE_DEFENSE;
				}
			}
			if (_type == ITEM_WING + 37)	// �ð��ǳ���
			{
				m_Option3 = Option3;
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;

				if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_1)
				{	// �߰� ����
					m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
				}
				else if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_2)
				{	// �߰� ����
					m_Special[m_SpecialNum] = AT_IMPROVE_DEFENSE;
				}
			}
			if (_type == ITEM_WING + 38)	// ȯ���ǳ���
			{
				m_Option3 = Option3;
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;

				if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_1)
				{	// �߰� ���ݷ�
					m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				}
				else if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_2)
				{	// �߰� ����
					m_Special[m_SpecialNum] = AT_IMPROVE_DEFENSE;
				}
			}
			if (_type == ITEM_WING + 39)	// �ĸ��ǳ���
			{
				m_Option3 = Option3;
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;

				if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_1)
				{	// �߰� ���ݷ�
					m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				}
				else if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_2)
				{	// �߰� ����
					m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
				}
			}
			if (_type == ITEM_WING + 40)	// �����Ǹ���
			{
				m_Option3 = Option3;
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;

				if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_1)
				{	// �߰� ���ݷ�
					m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
				}
				else if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_2)
				{	// �߰� ����
					m_Special[m_SpecialNum] = AT_IMPROVE_DEFENSE;
				}
			}
#endif	// ADD_THIRD_WING_20070525	
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� ����� ���� 3��° �ɼ�ó��
			else if (_type == ITEM_WING + 41)
			{	// ����� ����(��ȯ����)			
				m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;
			}
#endif		
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	//  ������ ����3��° �ɼ�
			else if (_type == ITEM_WING + 42)
			{	// ������ ����(����)			
				m_Option3 = Option3;
				//m_RequireStrength += Option3*4;

				if ((m_NewOption)&PLUS_WING_OP1_TYPE)
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
				}
				else
				{
					m_Special[m_SpecialNum] = AT_IMPROVE_CURSE;
				}
			}
			else if (_type == ITEM_WING + 43)	// ������ ����
			{
				m_Option3 = Option3;
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;

				if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_1)
				{	// �߰� ���ݷ�
					m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
				}
				else if ((m_NewOption)&PLUS_THIRD_WING_RAND_OPT_2)
				{	// �߰� ����
					m_Special[m_SpecialNum] = AT_IMPROVE_CURSE;
				}
			}
#endif	// ADD_SUMMONER_SECOND_THIRD_WING_20071130			
		}
	}

	m_SpecialNum++;

	if ((_type >= ITEM_SHIELD && _type < ITEM_BOOT + MAX_ITEM_INDEX) ||//��
		(_type >= ITEM_HELPER + 8 && _type <= ITEM_HELPER + 9)
		|| (_type >= ITEM_HELPER + 21 && _type <= ITEM_HELPER + 24)
		)//����
	{
		//��������
		if ((m_NewOption >> 5) & 1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_LIFE; m_SpecialNum++;
		}
		//��������
		if ((m_NewOption >> 4) & 1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_MANA; m_SpecialNum++;
		}
		//����������
		if ((m_NewOption >> 3) & 1)
		{
			m_Special[m_SpecialNum] = AT_DECREASE_DAMAGE; m_SpecialNum++;
		}
		//�������ݻ�
		if ((m_NewOption >> 2) & 1)
		{
			m_Special[m_SpecialNum] = AT_REFLECTION_DAMAGE; m_SpecialNum++;
		}
		//������
		if ((m_NewOption >> 1) & 1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_BLOCKING_PERCENT; m_SpecialNum++;
		}
		//ȹ��������
		if ((m_NewOption) & 1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_GAIN_GOLD; m_SpecialNum++;
		}
	}
	if ((_type >= ITEM_SWORD && _type < ITEM_STAFF + MAX_ITEM_INDEX) ||//����
		(_type >= ITEM_HELPER + 12 && _type <= ITEM_HELPER + 13)
		|| (_type >= ITEM_HELPER + 25 && _type <= ITEM_HELPER + 28)
		)//�����
	{
		//������뷮����
		if ((m_NewOption >> 5) & 1)
		{
			m_Special[m_SpecialNum] = AT_EXCELLENT_DAMAGE; m_SpecialNum++;
		}
		if ((_type >= ITEM_STAFF && _type < ITEM_STAFF + MAX_ITEM_INDEX) ||//������
			(_type == ITEM_HELPER + 12)
			//�����Ǹ����,���Ǹ���̸� ����
			//�ٶ�,����� ����� ����
			|| (_type == ITEM_HELPER + 25) || (_type == ITEM_HELPER + 27)
			)//�����
		{
			//��������(����)
			if ((m_NewOption >> 4) & 1)
			{
				//		m_SpecialValue[m_SpecialNum] = CharacterAttribute->Level/20;
				m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC_LEVEL; m_SpecialNum++;
			}
			//��������(�ۼ�Ʈ)
			if ((m_NewOption >> 3) & 1)
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC_PERCENT; m_SpecialNum++;
			}
		}
		else
		{
			//���ݷ�����(����)
			if ((m_NewOption >> 4) & 1)
			{
				//m_SpecialValue[m_SpecialNum] = CharacterAttribute->Level/20;
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE_LEVEL; m_SpecialNum++;
			}
			//���ݷ�����(�ۼ�Ʈ)
			if ((m_NewOption >> 3) & 1)
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE_PERCENT; m_SpecialNum++;
			}
		}
		//���ݼӵ�
		if ((m_NewOption >> 2) & 1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_ATTACK_SPEED; m_SpecialNum++;
			m_AttackSpeed += 7;
		}
		//ȹ���������
		if ((m_NewOption >> 1) & 1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_GAIN_LIFE; m_SpecialNum++;
		}
		//ȹ�渶������
		if ((m_NewOption) & 1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_GAIN_MANA; m_SpecialNum++;
		}
	}

#ifdef NEW_FORSKYLAND2	
#ifdef DARKLORD_WORK
	if ((_type >= ITEM_WING + 3 && _type <= ITEM_WING + 6) ||
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
		_type == ITEM_WING + 42 ||	// ������ ���� �ɼ�
#endif
		_type == ITEM_HELPER + 30) // ���� & ����	    
#else
	if (_type >= ITEM_WING + 3 && _type <= ITEM_WING + 6) // ����	    
#endif
	{
		if ((m_NewOption)&PLUS_WING_MAXLIFE)
		{	// �ִ� HP+50
			m_Special[m_SpecialNum] = AT_PLUS_WING_MAXLIFE; m_SpecialNum++;
		}

		if ((m_NewOption)&PLUS_WING_MAXMANA)
		{	// �ִ� MP+50
			m_Special[m_SpecialNum] = AT_PLUS_WING_MAXMANA; m_SpecialNum++;
		}

		if ((m_NewOption)&PLUS_WING_ONE_PERCENT_DAMAGE)
		{	// ������ HP->MP�� �ս�
			m_Special[m_SpecialNum] = AT_PLUS_ONE_PERCENT_DAMAGE; m_SpecialNum++;
		}

#ifdef DARKLORD_WORK
		if ((m_NewOption)&PLUS_WING_ADD_LEADERSHIP)
		{	// �������
			m_Special[m_SpecialNum] = AT_PLUS_WING_ADD_LEADERSHIP; m_SpecialNum++;
		}
#endif			
	}
#endif


	// �渱 ��� Ư�� �ɼ� �߰�!
#ifdef ADD_ITEM_FENRIR_01_20051110

	// �渱 �ɼǿ� ���� ��ų �߰�
	if (_type == ITEM_HELPER + 37)
	{
		if ((m_NewOption)&PLUS_FENRIR_INCREASE_LAST_DAMAGE)
		{
			m_Special[m_SpecialNum] = AT_PLUS_FENRIR_INCREASE_LAST_DAMAGE;
			m_SpecialNum++;
		}
		else if ((m_NewOption)&PLUS_FENRIR_DECREASE_LAST_DAMAGE)
		{
			m_Special[m_SpecialNum] = AT_PLUS_FENRIR_DECREASE_LAST_DAMAGE;
			m_SpecialNum++;
		}
#ifdef ADD_ILLUSION_FENRIR_20070703	// ȯ�� �渱 �ɼǿ� ���� ��ų �߰�
		else if ((m_NewOption)&PLUS_ILLUSION_FENRIR_OPTION)
		{
			m_Special[m_SpecialNum] = AT_PLUS_ILLUSION_FENRIR_OPTION;
			m_SpecialNum++;
		}
#endif
	}

#endif

#ifdef ADD_THIRD_WING_20070525	// 3�� ���� ���� �ɼ� ����
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ���� ���� �ɼ�����
	if (_type >= ITEM_WING + 36 && _type <= ITEM_WING + 40
		|| _type == ITEM_WING + 43)	// ������ ����
#else
	if (_type >= ITEM_WING + 36 && _type <= ITEM_WING + 40)
#endif
	{
		if ((m_NewOption)&PLUS_THIRD_WING_5PER_IGNORE_DEFENSE)
		{	// %5 Ȯ���� �� ���� ����
			m_Special[m_SpecialNum] = AT_5PER_IGNORE_DEFENSE;
			m_SpecialNum++;
		}
		if ((m_NewOption)&PLUS_THIRD_WING_5PER_ATTACK_TURNING)
		{	// 5% Ȯ���� �� ���ݷ� 50% ������
			m_Special[m_SpecialNum] = AT_5PER_ATTACK_TURNING;
			m_SpecialNum++;
		}
		if ((m_NewOption)&PLUS_THIRD_WING_RECOVER_100PER_LIFE)
		{	// 5% Ȯ���� ���� ���� 100% ���� ȸ��
			m_Special[m_SpecialNum] = AT_RECOVER_100PER_LIFE;
			m_SpecialNum++;
		}
		if ((m_NewOption)&PLUS_THIRD_WING_RECOVER_100PER_MASIC)
		{	// 5% Ȯ���� ���� ���� 100% ���� ȸ��
			m_Special[m_SpecialNum] = AT_RECOVER_100PER_MASIC;
			m_SpecialNum++;
		}
	}
#endif

#if GAME_VERSION >= G_V_99B
	if (SOptionStatValue)
	{	// ��Ʈ �Ӽ� ���� �ִ´�
		if (p->SetAttr)
		{	// ��Ʈ �Ӽ����� ������ ��Ʈ �Ӽ� ���� 1���� ���� �ǹǷ� -1�� �ʿ�			
			m_Special[m_SpecialNum] = AT_SET_IMPROVE_STRENGTH + p->SetAttr - 1;
			m_SetAddStat = SOptionStatValue * 5;
			m_SpecialNum++;
		}
	}
#endif

	//#ifdef DARKLORD_WORK	//Item.txt�� ��ġ �� �����
	m_Part = p->ItemSlot;
	//#else
		/*
		//
		//
		//	20051110 DARKLORD_WORK �� ELSE �κ� ���� (apple)
		//
		//
		//
		*/
		//#endif // DARKLORD_WORK

	Value();

	if (m_Type != ITEM_BOW + 7 && m_Type != ITEM_BOW + 15)
	{	// ȭ��, ���ÿ�ȭ���� �ƴϸ�
		m_DamageMinOrigin = m_DamageMin;
		m_DefenseOrigin = m_Defense;
		m_DamageMin -= (WORD)(m_DamageMin*m_CurrentDurabilityState);
		m_DamageMax -= (WORD)(m_DamageMax*m_CurrentDurabilityState);
		m_Defense -= (WORD)(m_Defense*m_CurrentDurabilityState);
		m_SuccessfulBlocking -= (BYTE)(m_SuccessfulBlocking*m_CurrentDurabilityState);

		if (m_Durability < 1.f)
			m_AttackSpeed = 0;
	}

	if (m_Durability == 0.f)
	{	// �������� 0�̸� ���׷µ� ����
		memset(m_Resistance, 0, MAX_RESISTANCE);
	}
}

int CItem::GetLevel(void)
{
	return ItemAttribute[m_Type].Level;
}

DWORD CItem::GetNumber()
{
	return m_Number;
}

#include "..\\gameserver\\Global.h"

void CItem::Value()
{
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	m_SellMoney = 0;		// WT �̺�Ʈ ���� ������ ������ 0
	m_BuyMoney = 0;
	return;
#endif

	if (m_Type == -1) return;

	ITEM_ATTRIBUTE *p = &ItemAttribute[m_Type];

	if (p->BuyMoney)
	{
		m_SellMoney = p->BuyMoney;
		m_BuyMoney = p->BuyMoney;

		m_SellMoney = m_SellMoney / 3;

		if (m_SellMoney >= 1000)
			m_SellMoney = m_SellMoney / 100 * 100;
		else if (m_SellMoney >= 100)
			m_SellMoney = m_SellMoney / 10 * 10;

		if (m_BuyMoney >= 1000)
			m_BuyMoney = m_BuyMoney / 100 * 100;
		else if (m_BuyMoney >= 100)
			m_BuyMoney = m_BuyMoney / 10 * 10;

		return;
	}

	int Level = (int)sqrt((double)m_Level);


#ifdef MODIFY_ITEM_PRICE_MAXFIX
	__int64 Gold = 0;
#else
	DWORD Gold = 0;
#endif

	int Type = m_Type / MAX_ITEM_INDEX;

	int Level2 = p->Level + m_Level * 3;
	int excellent = FALSE;

	for (int i = 0; i < m_SpecialNum; i++)
	{
		switch (m_Special[i])
		{
		case AT_IMPROVE_LIFE:
		case AT_IMPROVE_MANA:
		case AT_DECREASE_DAMAGE:
		case AT_REFLECTION_DAMAGE:
		case AT_IMPROVE_BLOCKING_PERCENT:
		case AT_IMPROVE_GAIN_GOLD:
		case AT_EXCELLENT_DAMAGE:
		case AT_IMPROVE_DAMAGE_LEVEL:
		case AT_IMPROVE_DAMAGE_PERCENT:
		case AT_IMPROVE_MAGIC_LEVEL:
		case AT_IMPROVE_MAGIC_PERCENT:
		case AT_IMPROVE_ATTACK_SPEED:
		case AT_IMPROVE_GAIN_LIFE:
		case AT_IMPROVE_GAIN_MANA:
			excellent = TRUE;
			break;
		}
	}
	if (excellent)
	{
		Level2 += 25;
	}
	if (m_Type == ITEM_BOW + 15)
	{	// ȭ��
		int levelgold = 70;
		if (m_Level == 1)
		{
			levelgold = 1200;
		}
		else if (m_Level == 2)
		{
			levelgold = 2000;
		}
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
		else if (m_Level == 3)
		{
			levelgold = 2800;
		}
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

		if (m_Durability > 0)
		{
#ifdef MODIFY_ITEM_PRICE_MAXFIX
			Gold = (__int64)(levelgold * m_Durability / p->Durability);
#else
			Gold = (DWORD)(levelgold * m_Durability / p->Durability);
#endif
		}
		//  	Gold = (int)(70 * m_Durability / p->Durability)+(80*(m_Level*2));
	}
	else if (m_Type == ITEM_BOW + 7)
	{
		if (m_Durability > 0)
		{
			int levelgold = 100;
			if (m_Level == 1)
			{
				levelgold = 1400;
			}
			else if (m_Level == 2)
			{
				levelgold = 2200;
			}
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
			else if (m_Level == 3)
			{
				levelgold = 3000;
			}
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

#ifdef MODIFY_ITEM_PRICE_MAXFIX
			Gold = (__int64)(levelgold * m_Durability / p->Durability);
#else
			Gold = (DWORD)(levelgold * m_Durability / p->Durability);
#endif
		}
		//Gold = (int)(100 * m_Durability / p->Durability)+(170*(m_Level*2));
	}
#ifdef FOR_TEMP_SERVER
	//------------------------------------------>
	//grooving SEASON4 20080728 : ���� �Ǹ� ���� 1000 ���� ����(����4 ü�� ���� �ӽ�)
	else if (m_Type == ITEM_POTION + 13)	// �༮
	{
		//Gold = 9000000;
		Gold = 1000;
	}
	else if (m_Type == ITEM_POTION + 14)	// ����
	{
		//Gold = 6000000;
		Gold = 1000;
	}
	else if (m_Type == ITEM_WING + 15)	// ȥ��
	{
		//Gold = 810000;
		Gold = 1000;
	}
	else if (m_Type == ITEM_POTION + 16)	// ����
	{
		//Gold = 45000000;
		Gold = 1000;
	}
	else if (m_Type == ITEM_POTION + 22)	// â��
	{
		//Gold = 36000000;
		Gold = 1000;
	}
#else
	else if (m_Type == ITEM_POTION + 13)	// �༮
	{
		Gold = 9000000;
	}
	else if (m_Type == ITEM_POTION + 14)	// ����
	{
		Gold = 6000000;
	}
	else if (m_Type == ITEM_WING + 15)	// ȥ��
	{
		Gold = 810000;
	}
	else if (m_Type == ITEM_POTION + 16)	// ����
	{
		Gold = 45000000;
	}
	else if (m_Type == ITEM_POTION + 22)	// â��
	{
		Gold = 36000000;
	}
#endif //FOR_TEMP_SERVER 
	//<------------------------------------------
#ifdef JEWEL_MIX_SYSTEM_20050309	// ���� ���վ����� �ǸŰ���
	else if (m_Type == ITEM_WING + 30)	// �༮ ����
	{
		Gold = 9000000 * (m_Level + 1) * 10;
	}
	else if (m_Type == ITEM_WING + 31)	// ���� ����
	{
		Gold = 6000000 * (m_Level + 1) * 10;
	}
#endif
	else if (m_Type == ITEM_HELPER + 15)	// ����
	{
		Gold = 33000000;
	}
	else if (m_Type == ITEM_HELPER + 14)	// ����
	{
#ifdef DARKLORD_WORK
		if (m_Level == 1)
			Gold = 2500000 * 3;		// ������ �Ҹ�

#ifdef MODIFY_PRICE_FEATHE_OF_LOCH_20041129		
		else  Gold = 180000;
#endif

#else
		Gold = 180000;
#endif
	}
#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	else if (m_Type == ITEM_POTION + 31)	// ��ȣ ����
	{
		Gold = 60000000;
	}
	else if (m_Type == ITEM_POTION + 7)  // ���� ����
	{
		if (m_Level == 0)	// �ູ�� ����
		{
#ifdef MODIFY_CASTLE_POTION_20051017
			Gold = 900000 * (int)m_Durability;
#else
			Gold = 2100000 * (int)m_Durability;
#endif
		}
		else if (m_Level == 1)	// ��ȥ�� ����
		{
#ifdef MODIFY_CASTLE_POTION_20051017
			Gold = 450000 * (int)m_Durability;
#else
			Gold = 1500000 * (int)m_Durability;
#endif
		}
	}
	else if (m_Type == ITEM_HELPER + 11) // ����������
	{
		if (m_Level == 1)	// ����������
			Gold = 2400000;
	}
	else if (m_Type == ITEM_HELPER + 7)  // �뺴
	{
		if (m_Level == 0) // Ȱ�뺴
			Gold = 1500000;
		else if (m_Level == 1)// â�뺴
			Gold = 1200000;
	}
#endif
#ifdef ADD_ITEM_FENRIR_01_20051110
	// �渱 ���� ������ ����
	else if (m_Type == ITEM_HELPER + 32)
	{
		Gold = 50 * 3 * (int)m_Durability;
	}
	else if (m_Type == ITEM_HELPER + 33)
	{
		Gold = 100 * 3 * (int)m_Durability;
	}
	else if (m_Type == ITEM_HELPER + 34)
	{
		Gold = 1000 * 3 * (int)m_Durability;
	}
	else if (m_Type == ITEM_HELPER + 35)
	{
		Gold = 10000 * 3;
	}
	else if (m_Type == ITEM_HELPER + 36)
	{
		Gold = 30000 * 3;
	}
	else if (m_Type == ITEM_HELPER + 37)
	{
		Gold = 50000 * 3;
	}
#endif

	// CODEREVIEW_SHIELD : ����ٰ� ���� �Ǹ� ���� '�����' �������� ������.
#ifdef ADD_SHIELD_POINT_01_20060403
	else if (m_Type == MAKE_ITEMNUM(14, 35))
	{
		Gold = (2000 * (int)m_Durability);
	}
	else if (m_Type == MAKE_ITEMNUM(14, 36))
	{
		Gold = (4000 * (int)m_Durability);
	}
	else if (m_Type == MAKE_ITEMNUM(14, 37))
	{
		Gold = (6000 * (int)m_Durability);
	}
	else if (m_Type == MAKE_ITEMNUM(14, 38))
	{
		Gold = (2500 * (int)m_Durability);
	}
	else if (m_Type == MAKE_ITEMNUM(14, 39))
	{
		Gold = (5000 * (int)m_Durability);
	}
	else if (m_Type == MAKE_ITEMNUM(14, 40))
	{
		Gold = (7500 * (int)m_Durability);
	}
#endif
	//
	else if (m_Type == ITEM_HELPER + 3)	// ����Ʈ
	{
		Gold = 960000;
		for (int i = 0; i < 3; i++)
		{
			if (m_Option3 & (1 << i))
			{	// �ɼ� �ϳ��� 100000�߰�
				Gold += 300000;
			}
		}
	}
	else if (m_Type == ITEM_POTION + 17)	// �Ǹ��� ��
	{
		switch (m_Level)
		{
#ifdef UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808	// �Ǹ��� �� ����
		case 1: Gold = 10000;	break;
		case 2: Gold = 50000;	break;
		case 3: Gold = 100000;	break;
		case 4: Gold = 300000;	break;
		case 5: Gold = 500000;	break;
		case 6: Gold = 800000;	break;
		case 7: Gold = 1000000;	break;
#else	// UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808			
		case 1:
			Gold = 15000;
			break;

		case 2:
			Gold = 21000;
			break;

		case 3:
			Gold = 30000;
			break;

		case 4:
			Gold = 45000;
			break;

#ifdef DEVILSQUARE_EXTEND_20050221		// �Ǹ��� �� �ǸŰ���
		case 5:
			Gold = 60000;
			break;

		case 6:
			Gold = 75000;
			break;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// �Ǹ��� �� 7 �ǸŰ���
		case 7:
			Gold = 30000 * 3;
			break;
#endif	
#endif	// UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808	
		default:
			Gold = 30000;
			break;

		}
	}
	else if (m_Type == ITEM_POTION + 18)	// �Ǹ��� ����
	{
		switch (m_Level)
		{
#ifdef UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808	// �Ǹ��� ���� ����
		case 1: Gold = 15000;	break;
		case 2: Gold = 75000;	break;
		case 3: Gold = 150000;	break;
		case 4: Gold = 450000;	break;
		case 5: Gold = 750000;	break;
		case 6: Gold = 1200000;	break;
		case 7: Gold = 1500000;	break;
#else	// UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808
		case 1:
			Gold = 15000;
			break;

		case 2:
			Gold = 21000;
			break;

		case 3:
			Gold = 30000;
			break;

		case 4:
			Gold = 45000;
			break;

#ifdef DEVILSQUARE_EXTEND_20050221		// �Ǹ��� ���� �ǸŰ���
		case 5:
			Gold = 60000;
			break;

		case 6:
			Gold = 75000;
			break;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// �Ǹ��ǿ���7 �ǸŰ���
		case 7:
			Gold = 30000 * 3;
			break;
#endif			
#endif	// UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808
		default:
			Gold = 30000;
			break;
		}
	}
	else if (m_Type == ITEM_POTION + 19)	// �Ǹ��� ���� �ʴ��
	{
		switch (m_Level)
		{
		case 1:
			Gold = 60000;
			break;

		case 2:
			Gold = 84000;
			break;

		case 3:
			Gold = 120000;
			break;

		case 4:
			Gold = 180000;
			break;

#ifdef DEVILSQUARE_EXTEND_20050221		// �Ǹ��� ���� �ʴ�� �ǸŰ���
		case 5:
			Gold = 240000;
			break;

		case 6:
			Gold = 300000;
			break;
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// ����7 �ʴ�� �ǸŰ���
		case 7:
			Gold = 60000 * 3;
			break;
#endif

		default:
			Gold = 120000;
			break;
		}
	}
	else if (m_Type == ITEM_POTION + 20)	// ����� ����
	{
		Gold = 900;
	}
	else if (m_Type == ITEM_POTION + 21)	// ����
	{
#ifdef FOR_BLOODCASTLE3
		switch (m_Level)
		{
		case 0:						// ����
			Gold = 3000 * 3;
			m_Durability = m_BaseDurability;
		case 1:						// ����
			Gold = 3000 * 3;
			m_Durability = m_BaseDurability;
			break;
#ifdef EVENT_STONE_OF_FRIENDSHIP_20040206	// �����ǵ� �����Ǹ� - 3000��
		case 2:						// �����ǵ�
			Gold = 3000 * 3;
			m_Durability = m_BaseDurability;
			break;
#endif
#ifdef MU_CASTLESIEGE_CS_MARKS_20041214
		case 3:						// ������ ǥ��
#ifdef MODIFY_ACCUMULATED_BUG_MARKOFLORD_SELL_VALUE_01_20050531
			Gold = 1300 * 3 * (int)m_Durability;
#else
			Gold = 3000 * 3;
#endif
			break;
#endif
		default:
			Gold = 3000 * 3;
			m_Durability = m_BaseDurability;
			break;
		}
#else
		Gold = 3000 * 3;
		m_Durability = m_BaseDurability;
#endif
	}
#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
	else if (m_Type == ITEM_POTION + 100)	// ����� ����
	{
		Gold = 100 * 3 * (int)m_Durability;
	}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029	
	else  if (m_Type == ITEM_POTION + 9 && m_Level == 1)	// �� 
	{	// ����� ����(�ø���)
		Gold = 1000;
	}
#ifdef FOR_BLOODCASTLE
	else if (m_Type == ITEM_HELPER + 18)	// �������
	{
		switch (m_Level)
		{
		case 1:
			Gold = 50000 * 3;
			break;

		case 2:
			Gold = 220000 * 3;
			break;

		case 3:
			Gold = 240000 * 3;
			break;

		case 4:
			Gold = 260000 * 3;
			break;

		case 5:
			Gold = 280000 * 3;
			break;

		case 6:
			Gold = 300000 * 3;
			break;

		case 7:
			Gold = 320000 * 3;
			break;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ������� +8 ����
		case 8:
			Gold = 340000 * 3;
			break;
#endif			

		default:
			Gold = 200000 * 3;
			break;
		}
	}
	else if ((m_Type == ITEM_HELPER + 16) || (m_Type == ITEM_HELPER + 17))	// ���庻 �̳� ��õ���Ǽ�
	{

		switch (m_Level)
		{
#ifdef UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808	// ���庻, ��õ���Ǽ� ����
		case 1: Gold = 10000;	break;
		case 2: Gold = 50000;	break;
		case 3: Gold = 100000;	break;
		case 4: Gold = 300000;	break;
		case 5: Gold = 500000;	break;
		case 6: Gold = 800000;	break;
		case 7: Gold = 1000000;	break;
		case 8: Gold = 1200000;	break;
#else	// UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808
		case 1:
			Gold = 5000 * 3;
			break;

		case 2:
			Gold = 7000 * 3;
			break;

		case 3:
			Gold = 10000 * 3;
			break;

		case 4:
			Gold = 13000 * 3;
			break;

		case 5:
			Gold = 16000 * 3;
			break;

		case 6:
			Gold = 20000 * 3;
			break;

		case 7:
			Gold = 25000 * 3;
			break;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ���庻+8, ��õ���Ǽ�+8 ����
		case 8:
			Gold = 30000 * 3;
			break;
#endif
#endif	// UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808				
		default:
			Gold = 5000 * 3;
			break;
		}
	}
#endif
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ������ ���ʼ�Ʈ �ǸŰ���
	else if (m_Type == ITEM_HELPER + 29)
	{
		Gold = 5000;
	}
#endif	
#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - �������� ���� �ǸŰ���
	else if (m_Type == ITEM_HELPER + 20)
	{
		switch (m_Level)
		{
		case 0:						// �������� ����
			Gold = 10000 * 3;
			break;
#ifdef MU_CHN_THANKS_RING_EVENT_20040908	// �߱� �̺�Ʈ - ������ ���� 
		case 3:						// ������ ����
			Gold = 10000 * 3;
			break;
#endif
		default:
			Gold = 0 * 3;
			break;
		}
	}
#endif
#ifdef DARKLORD_WORK
	else if (m_Type == ITEM_HELPER + 31)
	{
		switch (m_Level)
		{
		case 0:						// ��ũȣ���� ��ȥ
			Gold = 10000000 * 3;
			break;

		case 1:						// ��ũ���Ǹ��� ��ȥ
			Gold = 5000000 * 3;
			break;
		}
	}
#endif
#ifdef ADD_NEW_MAP_KALIMA_20040518
	else if (m_Type == ITEM_POTION + 28)		// �Ҿ���� ����
	{
		Gold = 200000 * 3;
		//#ifdef FOR_NEW_TESTSERVER
		//		Gold = 20000;
		//#else
		//		Gold = 20000 * 3;
		//#endif
	}
	else if (m_Type == ITEM_POTION + 29)		// �����ǥ��
	{
		Gold = 10000 * m_Durability * 3;
		//#ifdef FOR_NEW_TESTSERVER
		//		Gold = 1000 * m_Durability;
		//#else
		//		Gold = 1000 * m_Durability * 3;
		//#endif
	}
#endif
#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
	else if (m_Type == MAKE_ITEMNUM(14, 45)
		|| m_Type == MAKE_ITEMNUM(14, 46)
		|| m_Type == MAKE_ITEMNUM(14, 47)
		|| m_Type == MAKE_ITEMNUM(14, 48)
		|| m_Type == MAKE_ITEMNUM(14, 49)
		|| m_Type == MAKE_ITEMNUM(14, 50)
		)
	{
		Gold = 50 * m_Durability * 3;
	}

#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
	else if (m_Type == ITEM_WING + 26)
	{
		switch (m_Level) {
		case 0:							// �ź��� ����
			Gold = 60000;
			break;
		}
	}
#endif	
#ifdef CHRISTMAS_STAR_20061211
	else if (m_Type == MAKE_ITEMNUM(14, 51))	// ũ���������� ��
	{
		Gold = 200000;
	}
#endif	
#ifdef PCBANG_POINT_SYSTEM_20070206		// ������ �� �� ���� ����
	else if (m_Type == MAKE_ITEMNUM(14, 55) 		// ȥ���� ����
		|| m_Type == MAKE_ITEMNUM(14, 56)
		|| m_Type == MAKE_ITEMNUM(14, 57))
	{
		//#ifdef MODIFY_PCBANGPOINTSHOP_CLOSEWINDOWS_BUGFIX_20080218	// ȥ���ǻ��� ���� �缳��
		//		Gold = 30000000 * 3;
		//#else
		Gold = 3000 * 3;
		//#endif	// MODIFY_PCBANGPOINTSHOP_CLOSEWINDOWS_BUGFIX_20080218
	}
#endif
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ������ �ȶ� ����
	else if ((m_Type == MAKE_ITEMNUM(13, 49))		// ���� �η縶��
		|| (m_Type == MAKE_ITEMNUM(13, 50)) 		// ȯ�������� ����
		|| (m_Type == MAKE_ITEMNUM(13, 51)))		// ���� �η縶��
	{
		switch (m_Level)
		{
#ifdef UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808	// ���ǵη縶��, ȯ�汳���� ���� ����
		case 1: Gold = 500000;	break;
		case 2: Gold = 600000;	break;
		case 3: Gold = 800000;	break;
		case 4: Gold = 1000000;	break;
		case 5: Gold = 1200000;	break;
		case 6: Gold = 1400000;	break;
#else	// UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808
		case 1:
			Gold = 3000 * 3;
#endif	// UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808
		default:
			Gold = 3000 * 3;
		}
	}
#endif
#ifdef ADD_THIRD_WING_20070525	// �ܵ����� ����, �Ҳ� ���� ����
	else if (m_Type == MAKE_ITEMNUM(13, 52)	// �ܵ����� ����
		|| m_Type == MAKE_ITEMNUM(13, 53)) 	// �ܵ����� �Ҳ�	
	{
		Gold = 1000000 * 3;
	}
#endif	
#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704	// ����Ʈ ������ ���ݼ���
	else if (m_Type == MAKE_ITEMNUM(14, 23)	// �����Ǽ�
		|| m_Type == MAKE_ITEMNUM(14, 24)		// �η�����
		|| m_Type == MAKE_ITEMNUM(14, 25)		// �����Ǵ���
		|| m_Type == MAKE_ITEMNUM(14, 26)		// ��������ȥ
		|| m_Type == MAKE_ITEMNUM(14, 65)		// ����������Ʈ�ǺҲ�
		|| m_Type == MAKE_ITEMNUM(14, 66)		// �︶�̳��ǻ�
		|| m_Type == MAKE_ITEMNUM(14, 67)		// ����Ǻһ����Ǳ���
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912// ��ȯ���� ����Ʈ ������ ����
		|| m_Type == MAKE_ITEMNUM(14, 68)		// �ɿ��Ǵ�����
#endif
		)
	{
		Gold = 3000 * 3;
	}
#endif	

	else if (p->Value > 0)
	{
		Gold += p->Value*p->Value * 10 / 12;
		if (m_Type >= ITEM_POTION && m_Type <= ITEM_POTION + 8)
		{
#ifdef ITEM_12_13_20040401
#ifdef FOR_CHINA
			if (m_Level > 0)
			{
#ifdef MODIFY_ITEM_PRICE_MAXFIX
				Gold *= (__int64)(pow(3.0f, Level));
				//Gold *= (__int64)pow(3, Level);
#else
				Gold *= (DWORD)(pow(3.0f, Level));
				//Gold *= (DWORD)pow(3, Level);
#endif	// MODIFY_ITEM_PRICE_MAXFIX				
			}
#else
#ifdef UPDATE_ZEN_DROP_DOWN_20080806	// ū ġ�Ṱ��, ū �������� ���� 2��
			if (m_Type == ITEM_POTION + 3 || m_Type == ITEM_POTION + 6)
			{
				Gold *= 2;
			}
#endif	// UPDATE_ZEN_DROP_DOWN_20080806
			if (m_Level > 0)
#ifdef MODIFY_ITEM_PRICE_MAXFIX
				Gold *= (__int64)pow((double)2, (double)Level);
#else
				Gold *= (DWORD)pow((double)2, (double)Level);
#endif	// MODIFY_ITEM_PRICE_MAXFIX
#endif	// FOR_CHINA
#endif

#ifdef HP_POTION_VALUE_CHANGE_20040618		
			Gold = Gold / 10 * 10;
#endif

#ifdef MODIFY_ITEM_PRICE_MAXFIX
			Gold *= (__int64)m_Durability;
#else
			Gold *= (DWORD)m_Durability;
#endif

#ifdef HP_POTION_VALUE_CHANGE_20040618
			m_BuyMoney = Gold;
			m_SellMoney = Gold / 3;
			m_SellMoney = m_SellMoney / 10 * 10;
			return;
#endif
		}
	}
#ifdef ADD_THIRD_WING_20070525	// 3������ ��� - type 12���� 3������ ����
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// �ű�ĳ���� ���� type 12���� ����
	else if ((Type == 12 && ((m_Type > ITEM_WING + 6) && (m_Type < ITEM_WING + 36) && (m_Type > ITEM_WING + 43)))
#else
	else if ((Type == 12 && ((m_Type > ITEM_WING + 6) && (m_Type < ITEM_WING + 36) && (m_Type > ITEM_WING + 40)))
#endif
		|| Type == 13 || Type == 15)	//12(����,����) 13(��ź,����) 15(����)
#else	
	else if ((Type == 12 && m_Type > ITEM_WING + 6) || Type == 13 || Type == 15)//12(����,����) 13(��ź,����) 15(����)
#endif
	{
		//int Level2 = p->Level+m_Level*3;
		Gold = 100 + Level2 * Level2*Level2;
			for (int i = 0; i < m_SpecialNum; i++)
			{
				switch (m_Special[i])
				{
				case AT_LIFE_REGENERATION:
					Gold += Gold * m_Option3;
					break;
				}
			}
	}
#ifdef ADD_FIRECRACKER_NPC_20070306	// ���װ��� ����
	else if (m_Type == MAKE_ITEMNUM(14, 63))	// ����
	{
		Gold = 200000;
	}
#endif
#ifdef MODIFY_CHERRYBLOSSOM_SELL_MONEY_BUGFIX_20090325
	// ���� ���� ������ ����
	else if (m_Type == MAKE_ITEMNUM(14, 85)		// ���ɼ�
		|| m_Type == MAKE_ITEMNUM(14, 86)		// ���ɳ��� ���
		|| m_Type == MAKE_ITEMNUM(14, 87)		// ���� ����
		|| m_Type == MAKE_ITEMNUM(14, 90)		// Ȳ�� ���� ����
		)
	{
		Gold = 100 * 3 * (int)m_Durability;
	}
#endif // MODIFY_CHERRYBLOSSOM_SELL_MONEY_BUGFIX_20090325
	else
	{
		//Level2 = p->Level+m_Level*3;
		switch (m_Level)     //+�����ϼ��� ����ġ ����
		{
		case 5:Level2 += 4; break;
		case 6:Level2 += 10; break;
		case 7:Level2 += 25; break;
		case 8:Level2 += 45; break;
		case 9:Level2 += 65; break;
		case 10:Level2 += 95; break;
		case 11:Level2 += 135; break;
#ifdef ITEM_12_13_20040401
		case 12:Level2 += 185; break;
		case 13:Level2 += 245; break;
#endif
		}
		if (Type == 12 && m_Type <= ITEM_WING + 6)
		{
			Gold = 40000000 + ((40 + Level2)*Level2*Level2 * 11);
		}
#ifdef ADD_THIRD_WING_20070525	// 3������ �⺻���� ����
		else if (Type == 12 && (m_Type >= ITEM_WING + 36 && m_Type <= ITEM_WING + 40))
		{
			Gold = 40000000 + ((40 + Level2) * Level2 * Level2 * 11);
		}
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� ���� ����
		else if (Type == 12 && (m_Type >= ITEM_WING + 41 && m_Type <= ITEM_WING + 43))
		{
			Gold = 40000000 + ((40 + Level2)*Level2*Level2 * 11);
		}
#endif
		else
		{
			Gold = 100 + (40 + Level2)*Level2*Level2 / 8;
		}
		if (Type >= 0 && Type <= 6) //�����϶�
		{
			if (!p->TwoHand) //�Ѽչ����϶� 80%
				Gold = Gold * 80 / 100;
		}
		for (int i = 0; i < m_SpecialNum; i++)
		{
			switch (m_Special[i])
			{
			case AT_SKILL_BLOCKING:
			case AT_SKILL_SWORD1:
			case AT_SKILL_SWORD2:
			case AT_SKILL_SWORD3:
			case AT_SKILL_SWORD4:
			case AT_SKILL_SWORD5:
#ifdef ADD_SKILL_20031022
			case AT_SKILL_SWORD6:
#endif
			case AT_SKILL_CROSSBOW:
#ifdef MODIFY_ITEM_PRICE_MAXFIX
				Gold += (__int64)((double)Gold*1.5f);
#else
				Gold += (DWORD)((float)Gold*1.5f);
#endif
				break;
			case AT_IMPROVE_DAMAGE:
			case AT_IMPROVE_MAGIC:
			case AT_IMPROVE_DEFENSE:
			case AT_LIFE_REGENERATION:
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
			case AT_IMPROVE_CURSE:
#endif				
				switch (m_Option3)
				{
#ifdef MODIFY_ITEM_PRICE_MAXFIX
				case 1:Gold += (__int64)((double)Gold * 6 / 10); break;
				case 2:Gold += (__int64)((double)Gold * 14 / 10); break;
				case 3:Gold += (__int64)((double)Gold * 28 / 10); break;
				case 4:Gold += (__int64)((double)Gold * 56 / 10); break;
#else
				case 1:Gold += (DWORD)((float)Gold * 6 / 10); break;
				case 2:Gold += (DWORD)((float)Gold * 14 / 10); break;
				case 3:Gold += (DWORD)((float)Gold * 28 / 10); break;
				case 4:Gold += (DWORD)((float)Gold * 56 / 10); break;
#endif
				}
				break;
			case AT_IMPROVE_BLOCKING:
				switch (m_Option3)
				{
#ifdef MODIFY_ITEM_PRICE_MAXFIX
				case 1: Gold += (__int64)((double)Gold * 6 / 10); break;
				case 2: Gold += (__int64)((double)Gold * 14 / 10); break;
				case 3: Gold += (__int64)((double)Gold * 28 / 10); break;
				case 4: Gold += (__int64)((double)Gold * 56 / 10); break;
#else
				case 1: Gold += (DWORD)((float)Gold * 6 / 10); break;
				case 2: Gold += (DWORD)((float)Gold * 14 / 10); break;
				case 3: Gold += (DWORD)((float)Gold * 28 / 10); break;
				case 4: Gold += (DWORD)((float)Gold * 56 / 10); break;
#endif
				}
				break;
			case AT_LUCK:
#ifdef MODIFY_ITEM_PRICE_MAXFIX
				Gold += (__int64)((double)Gold * 25 / 100);
#else
				Gold += (DWORD)((float)Gold * 25 / 100);
#endif
				break;
			case AT_IMPROVE_LIFE:
			case AT_IMPROVE_MANA:
			case AT_DECREASE_DAMAGE:
			case AT_REFLECTION_DAMAGE:
			case AT_IMPROVE_BLOCKING_PERCENT:
			case AT_IMPROVE_GAIN_GOLD:
			case AT_EXCELLENT_DAMAGE:
			case AT_IMPROVE_DAMAGE_LEVEL:
			case AT_IMPROVE_DAMAGE_PERCENT:
			case AT_IMPROVE_MAGIC_LEVEL:
			case AT_IMPROVE_MAGIC_PERCENT:
			case AT_IMPROVE_ATTACK_SPEED:
			case AT_IMPROVE_GAIN_LIFE:
			case AT_IMPROVE_GAIN_MANA:
				Gold += Gold;
				break;

				//  �߰� ����.(2003.09.23)
			case AT_PLUS_ONE_PERCENT_DAMAGE:
			case AT_PLUS_WING_MAXLIFE:
			case AT_PLUS_WING_MAXMANA:
			case AT_IMPROVE_AG_MAX:
			case AT_DAMAGE_ABSORB:
#ifdef ADD_THIRD_WING_20070525	// 3�� ���� ���� ���� - �ɼǿ� ���� �߰��ݾ�
			case AT_5PER_IGNORE_DEFENSE:
			case AT_5PER_ATTACK_TURNING:
			case AT_RECOVER_100PER_LIFE:
			case AT_RECOVER_100PER_MASIC:
#endif				
#ifdef MODIFY_ITEM_PRICE_MAXFIX
				Gold += (__int64)((double)Gold * 25 / 100);
#else
				Gold += (DWORD)((float)Gold * 25 / 100);
#endif
				break;
			}
		}

	}

#ifndef CHARACTERDB_SERVER
#ifdef ADD_380ITEM_NEWOPTION_20060711
	// �߰� �ɼǿ� ���� ���� ���� : 380 �߰� �ɼ�
	if (g_kItemSystemFor380.Is380OptionItem(this) == TRUE)
	{
		// �߰��ɼ� �� 380 �߰� �ɼ��� �ִ� ��� ���� 1.6��
		Gold += (__int64)((double)Gold * 16 / 100);
	}
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
	// ���� �������� �ǸŰ��� ���� �κ� �߰�
	if (g_SocketOptionSystem.IsSocketItem(this) == true)
	{
		int iSocketSlotCount = 0;
		int iSeedSphereValue = 0;
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
		DWORD dwSeedSphereValue = 0;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715

		for (int i = 0; i < MAX_SOCKET_SLOT; i++)
		{
			if (m_SocketOption[i] != SOCKETSLOT_NONE)
			{
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
				if (m_SocketOption[i] != SOCKETSLOT_EMPTY)
				{
					dwSeedSphereValue += g_SocketOptionSystem.GetSeedSphereValue(m_SocketOption[i]);
				}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
				iSocketSlotCount++;
			}
		}
		Gold += (DWORD)((float)Gold * (8 * iSocketSlotCount) / 10);
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
		Gold += dwSeedSphereValue;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	}
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif

#ifdef MODIFY_ITEM_PRICE_MAXFIX

	if (Gold > 3000000000)
	{
		Gold = 3000000000;
	}

#else
	if (Gold > 1200000000)
	{
		Gold = 1200000000;
	}
#endif

#ifdef FOR_TEMP_SERVER 
	//------------------------------------------>
	//grooving SEASON4 20080728 : ���� �Ǹ� ���� 1000 ���� ����(����4 ü�� ���� �ӽ� ���߿� ����)
	if (m_Type == MAKE_ITEMNUM(14, 42))	// ����
	{
		Gold = 1000;
	}
	//<------------------------------------------
#endif //FOR_TEMP_SERVER
	m_BuyMoney = (DWORD)Gold;
	m_SellMoney = (DWORD)Gold;
	m_SellMoney = m_SellMoney / 3;

#ifdef _GAMESERVER
	if (g_GlobalConfig.m_bBlessBug == true && this->m_Type == ITEM_POTION + 13)
#else
	if (this->m_Type == ITEM_POTION + 13)
#endif
	{
		this->m_SellMoney *= this->m_Durability;

		if (this->m_SellMoney > MAX_ZEN)
			this->m_SellMoney = MAX_ZEN;
	}

	if ((m_Type < ITEM_POTION || m_Type > ITEM_POTION + 8) &&
#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - �������� ���� �ǸŰ���
		m_Type != ITEM_HELPER + 20 &&		// �������� ����
#endif
#ifdef ADD_NEW_MAP_KALIMA_20040518
		m_Type != ITEM_POTION + 28 &&		// �Ҿ���� ����
		m_Type != ITEM_POTION + 29 &&		// ����� ǥ��
#endif

#ifdef MODIFY_ACCUMULATED_BUG_MARKOFLORD_SELL_VALUE_01_20050531
		m_Type != ITEM_POTION + 21 &&
#endif
		m_Type != ITEM_HELPER + 15 &&		// ����
		m_Type != ITEM_HELPER + 16 &&		// ��õ���Ǽ�
		m_Type != ITEM_HELPER + 17 &&		// ���庻
		m_Type != ITEM_HELPER + 18			// �������
#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
		&&m_Type != ITEM_HELPER + 11		// ����������
		&& m_Type != ITEM_HELPER + 7		// �뺴
#endif
#ifdef ADD_ITEM_FENRIR_01_20051110
		&&m_Type != ITEM_HELPER + 32		// ���� ����
		&& m_Type != ITEM_HELPER + 33		// ���� ����
		&& m_Type != ITEM_HELPER + 34		// �ͼ� ����
		&& m_Type != ITEM_HELPER + 35		// ���Ǹ� ����
		&& m_Type != ITEM_HELPER + 36		// �η��� ���Ǹ�
		&& m_Type != ITEM_HELPER + 37		// �渱�� ���Ǹ�
#endif
#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
		&& m_Type != MAKE_ITEMNUM(14, 45)
		&& m_Type != MAKE_ITEMNUM(14, 46)
		&& m_Type != MAKE_ITEMNUM(14, 47)
		&& m_Type != MAKE_ITEMNUM(14, 48)
		&& m_Type != MAKE_ITEMNUM(14, 49)
		&& m_Type != MAKE_ITEMNUM(14, 50)
#endif
#ifdef CHRISTMAS_STAR_20061211
		&& m_Type != MAKE_ITEMNUM(14, 51)	// ũ���������� ��
#endif	
#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
		&& m_Type != MAKE_ITEMNUM(14, 100)
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029		
#ifdef MODIFY_SELLITEM_WITHOUT_DURABILITY_20090914
#ifdef MODIFY_CHERRYBLOSSOM_SELL_MONEY_BUGFIX_20090325
		&& m_Type != MAKE_ITEMNUM(14, 85)		// ���ɼ�
		&& m_Type != MAKE_ITEMNUM(14, 86)		// ���ɳ��� ���
		&& m_Type != MAKE_ITEMNUM(14, 87)		// ���� ����
		&& m_Type != MAKE_ITEMNUM(14, 90)		// Ȳ�� ���� ����
#endif // MODIFY_CHERRYBLOSSOM_SELL_MONEY_BUGFIX_20090325
#else
#ifdef MODIFY_CHERRYBLOSSOM_SELL_MONEY_BUGFIX_20090325
		&& m_Type == MAKE_ITEMNUM(14, 85)		// ���ɼ�
		&& m_Type == MAKE_ITEMNUM(14, 86)		// ���ɳ��� ���
		&& m_Type == MAKE_ITEMNUM(14, 87)		// ���� ����
		&& m_Type == MAKE_ITEMNUM(14, 90)		// Ȳ�� ���� ����
#endif // MODIFY_CHERRYBLOSSOM_SELL_MONEY_BUGFIX_20090325
#endif // MODIFY_SELLITEM_WITHOUT_DURABILITY_20090914
		)
	{
		float  persent = 1.f - m_Durability / m_BaseDurability;
		DWORD  repairGold = (DWORD)(m_SellMoney * 0.6 * persent);
		m_SellMoney -= repairGold;
	}



	//if(p->Durability > 0)
	//	m_SellMoney += m_SellMoney*m_Durability/p->Durability;
	if (m_SellMoney >= 1000)
		m_SellMoney = m_SellMoney / 100 * 100;
	else if (m_SellMoney >= 100)
		m_SellMoney = m_SellMoney / 10 * 10;

	if (m_BuyMoney >= 1000)
		m_BuyMoney = m_BuyMoney / 100 * 100;
	else if (m_BuyMoney >= 100)
		m_BuyMoney = m_BuyMoney / 10 * 10;

}

void CItem::OldValue()
{
	if (m_Type == -1) return;

	int Gold = 0;
	/*ITEM_ATTRIBUTE *p = &ItemAttribute[m_Type];
	int Level = (int)sqrt(m_Level);

	int Type = m_Type/MAX_ITEM_INDEX;

	int Level2 = p->Level+m_Level*3;
	int excellent=FALSE;*/

	if (m_Type == ITEM_POTION + 13)	// �༮
	{
		Gold = 100000;
	}
	else if (m_Type == ITEM_POTION + 14)	// ����
	{
		Gold = 70000;
	}
	else if (m_Type == ITEM_WING + 15)	// ȥ��
	{
		Gold = 40000;
	}
	else if (m_Type == ITEM_POTION + 22)	// â��
	{
		Gold = 450000;
	}
	else if (m_Type == ITEM_POTION + 16)	// ����
	{
		Gold = 450000;
	}
	else
	{
		//m_OldSellMoney = m_SellMoney;
		m_OldBuyMoney = m_BuyMoney;
		return;
	}

	//m_OldSellMoney = Gold;
	m_OldBuyMoney = Gold;

	//m_OldSellMoney = m_OldSellMoney/3;		

	/*if( m_Type < ITEM_POTION || m_Type > ITEM_POTION+8)
	{
		float  persent = 1.f - m_Durability/m_BaseDurability;
		int	   repairGold = (int)(m_OldSellMoney * 0.6 * persent);
		m_OldSellMoney -= repairGold;
	}
	*/

	//if(p->Durability > 0)
	//	m_OldSellMoney += m_SellMoney*m_Durability/p->Durability;

	/*
	if(m_OldSellMoney>= 1000)
		m_OldSellMoney = m_OldSellMoney/100*100;
	else if(m_OldSellMoney >= 100)
		m_OldSellMoney = m_OldSellMoney/10*10;
		*/

	if (m_OldBuyMoney >= 1000)
		m_OldBuyMoney = m_OldBuyMoney / 100 * 100;
	else if (m_OldBuyMoney >= 100)
		m_OldBuyMoney = m_OldBuyMoney / 10 * 10;
}

//----------------------------------------------------------------------------
// �������� ���� ���� ũ�⸦ ��´�.
//----------------------------------------------------------------------------
BOOL CItem::GetSize(int &w, int &h)
{
#ifdef _DEBUG
	if (m_Type < 0 || m_Type > MAX_ITEM - 1)
	{
		LogAdd("error: %s %d (m_Type:%d)", __FILE__, __LINE__, m_Type);
		return FALSE;
	}
#endif
	w = ItemAttribute[m_Type].Width;
	h = ItemAttribute[m_Type].Height;

	return TRUE;
	//LogAdd("Item:%d %d", w, h);
}

int CItem::ItemDamageMin()
{
	if (m_Type < 0) return 0;

	int damagemin = m_DamageMinOrigin;
	PlusSpecial(&damagemin, AT_IMPROVE_DAMAGE);
	return damagemin;
}

int CItem::ItemDefense()
{
	if (m_Type < 0) return 0;

	if (!m_IsValidItem)
		return 0;

	int Defense = m_Defense;
	PlusSpecial(&Defense, AT_IMPROVE_DEFENSE);
	return Defense;
}

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ���ַ��� ����
int CItem::ItemCurse()
{
	if (m_Type < 0) return 0;

	if (!m_IsValidItem)
		return 0;

	int Curse = m_Curse;
	//PlusSpecial(&Defense,AT_IMPROVE_DEFENSE);
	return Curse;
}
#endif

#ifdef ADD_THIRD_WING_20070525	// IsClass() ���� ó��
BOOL CItem::IsClass(char aClass, int ChangeUP, int ThirdChangeUp)
{
	if (aClass < 0 || aClass >= MAX_CLASSTYPE)
	{
		LogAdd("Ŭ���� ���� : %d (%s %d)", aClass, __FILE__, __LINE__);
		return FALSE;
	}

	int requireclass = m_RequireClass[aClass];

	if (requireclass == 0) return FALSE;
	if (requireclass >= 3)	// 3�������̻�
	{
		if (requireclass != ThirdChangeUp + 2)
			return FALSE;
	}
	else if (requireclass > 1)
	{	// ���������̸�
		if (requireclass != ChangeUP + 1)
			return FALSE;
	}

	return TRUE;
}
#else
#ifdef NEW_SKILL_FORSKYLAND
BOOL CItem::IsClass(char aClass, int ChangeUP)
{
	if (aClass < 0 || aClass >= MAX_CLASSTYPE)
	{
		LogAdd("Ŭ���� ���� : %d (%s %d)", aClass, __FILE__, __LINE__);
		return FALSE;
	}

	int requireclass = m_RequireClass[aClass];

	if (requireclass == 0) return FALSE;
	if (requireclass > 1)
	{	// ���������̸�
		if (requireclass != ChangeUP + 1)
			return FALSE;
	}
	return TRUE;
}
#else
// �������� ����Ҽ� �ִ� �������� 
BOOL CItem::IsClass(char aClass)
{
	if (aClass < 0 || aClass >= MAX_CLASSTYPE)
	{
		LogAdd("Ŭ���� ���� : %d (%s %d)", aClass, __FILE__, __LINE__);
		return FALSE;
	}
	if (m_RequireClass[aClass] == 0) return FALSE;
	return TRUE;
}
#endif	// NEW_SKILL_FORSKYLAND
#endif	// ADD_THIRD_WING_20070525

//----------------------------------------------------------------------------
// �������� �̸��� ��´�.
//----------------------------------------------------------------------------
char *CItem::GetName()
{
#ifdef _DEBUG
	if (m_Type < 0 || m_Type > MAX_ITEM - 1)
	{
		//LogAdd("error: %s %d (m_Type:%d)", __FILE__, __LINE__, m_Type);
		return NULL;
	}
#endif	
	return ItemAttribute[m_Type].Name;
	//LogAdd("Item:%d %d", w, h);
}


void CItem::PlusSpecialSetRing(BYTE* Value)
{
	if (m_Type < 0) return;

	if (!IsSetItem())		// ���� �߰� ��ų ������ ����
		return;

	if (m_Durability == 0.f)
	{	// �������� 0�̸�  �߰� �ɼ� �������� �ʴ´�
		return;
	}

	if (!m_IsValidItem)		// ���� ������ �������� �ƴϸ� �������� �ʴ´�
		return;

	ITEM_ATTRIBUTE *p = &ItemAttribute[m_Type];

	if (p->ResistanceType == -1)
		return;

	//BYTE iValue = m_Option3*5;
	BYTE iValue = 5;
	Value[p->ResistanceType] += iValue - (BYTE)(iValue*m_CurrentDurabilityState);
}

void CItem::PlusSpecial(int* Value, int Special)
{
	if (m_Type < 0) return;

	if (m_Durability == 0.f)
	{	// �������� 0�̸�  �߰� �ɼ� �������� �ʴ´�
		return;
	}

	if (!m_IsValidItem)		// ���� ������ �������� �ƴϸ� �������� �ʴ´�
		return;

	for (int i = 0; i < m_SpecialNum; i++)
	{
		if (m_Special[i] == Special)
		{
			switch (Special)
			{
			case AT_IMPROVE_BLOCKING:
			{
				int iValue = m_Option3 * 5;
				(*Value) += iValue - (int)(iValue*m_CurrentDurabilityState);
			}
			break;
			case AT_LUCK:
				*Value += 4;
				break;
			case AT_IMPROVE_MAGIC:
			case AT_IMPROVE_DEFENSE:
			case AT_IMPROVE_DAMAGE:
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
			case AT_IMPROVE_CURSE:
#endif	
			{
				int iValue = m_Option3 * 4;
				(*Value) += iValue - (int)(iValue*m_CurrentDurabilityState);
			}
			break;

#ifdef NEW_FORSKYLAND2
			case AT_PLUS_WING_MAXLIFE:		// �ִ� HP+50����			
			case AT_PLUS_WING_MAXMANA:		// �ִ� MP+50����
			{
				(*Value) += 50 + m_Level * 5;
			}
			break;

			case AT_IMPROVE_AG_MAX:		// �ִ� AG+50����
			{
				(*Value) += 50;
			}
			break;
#endif

#ifdef DARKLORD_WORK		// ��� ����(����)
			case AT_PLUS_WING_ADD_LEADERSHIP:
			{
				(*Value) += 10 + m_Level * 5;
			}
			break;
#endif

			default:	// AT_IMPROVE_DAMAGE
						// AT_IMPROVE_MAGIC 
						// AT_IMPROVE_DEFENSE
				*Value += m_Option3 * 4;
				//(*Value)
				break;
			}
		}
	}
}

void CItem::PlusSpecialPercent(int* Value, int Special, WORD Percent)
{
	if (m_Type < 0) return;


	for (int i = 0; i < m_SpecialNum; i++)
	{
		if (m_Special[i] == Special)
		{
			*Value += *Value*Percent / 100;
		}
	}
}

// ��������, AG���� % ����� ���� Ȯ��
void CItem::PlusSpecialPercentEx(int* Value, int SourceValue, int Special)
{
	if (m_Type < 0) return;

	if (m_Durability == 0.f)
	{	// �������� 0�̸�  �߰� �ɼ� �������� �ʴ´�
		return;
	}

	if (!m_IsValidItem)		// ���� ������ �������� �ƴϸ� �������� �ʴ´�
		return;


	for (int i = 0; i < m_SpecialNum; i++)
	{
		if (m_Special[i] == Special)
		{
			int iValue = SourceValue * m_Option3 / 100;
			(*Value) += iValue - (int)(iValue*m_CurrentDurabilityState);
		}
	}
}


// ��Ʈ ������ ���� �߰�
void CItem::SetItemPlusSpecialStat(WORD* Value, int Special)
{
	if (m_Type < 0) return;

	if (m_Durability == 0.f)
	{	// �������� 0�̸�  �߰� �ɼ� �������� �ʴ´�
		return;
	}

	for (int i = 0; i < m_SpecialNum; i++)
	{
		if (m_Special[i] == Special)
		{
			switch (Special)
			{
			case AT_SET_IMPROVE_STRENGTH:
			case AT_SET_IMPROVE_DEXTERITY:
			case AT_SET_IMPROVE_VITALITY:
			case AT_SET_IMPROVE_ENERGY:
			{
				int iValue = m_SetAddStat;
				(*Value) += iValue - (int)(iValue*m_CurrentDurabilityState);	// ������ ����
			}
			break;

			default:
				*Value += m_Option3 * 3;
				break;
			}
		}
	}
}

int CItem::GetAddStatType()
{
	ITEM_ATTRIBUTE *p = &ItemAttribute[m_Type];
	return p->SetAttr;
}

int CItem::GetWeaponType()
{
	if (m_Type <= ITEM_STAFF)
	{	// �˷�
		return 1;
	}
	else if (m_Type <= ITEM_SHIELD)
	{	// �����̷�
		return 2;
	}
	return 0;
}

#ifdef DARKLORD_WORK
void CItem::SetPetItemInfo(int petlevel, int petexp)
{
	//|| m_Type == MAKE_ITEMNUM(13,5) )
	if (m_Type == MAKE_ITEMNUM(13, 4))
	{	// ��ũȣ��
		m_PetItem_Level = petlevel;
		m_PetItem_Exp = petexp;

		if (m_PetItem_Exp < gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level] ||
			m_PetItem_Exp >= gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level + 1])
		{
			m_PetItem_Exp = gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level];
		}

#ifdef ADD_380ITEM_NEWOPTION_20060711
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
		PetValue();

		m_IsLoadPetItemInfo = TRUE;
	}
	else if (m_Type == MAKE_ITEMNUM(13, 5))
	{	// ��ũ���Ǹ�
		m_PetItem_Level = petlevel;
		m_PetItem_Exp = petexp;

		if (m_PetItem_Exp < gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level] ||
			m_PetItem_Exp >= gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level + 1])
		{
			m_PetItem_Exp = gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level];
		}

#ifdef ADD_380ITEM_NEWOPTION_20060711
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif


		PetValue();

		m_IsLoadPetItemInfo = TRUE;
	}
}

int	CItem::AddPetItemExp(int petexp)
{
	if (m_Durability <= 0.f)
		return FALSE;	// �������� 0�̶��� ����ġ ��� ����

	if (!m_IsLoadPetItemInfo)
		return FALSE;	// �� ������ ������ �� ��� �Ա� ������ ����ġ ���� ����

	if (m_PetItem_Level < 1)
	{
		m_PetItem_Level = 1;
		m_PetItem_Exp = 0;


#ifdef ADD_380ITEM_NEWOPTION_20060711
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif

		PetValue();

		return FALSE;
	}

	if (m_Type == MAKE_ITEMNUM(13, 4))
	{	// ��ũȣ��		
		if (m_PetItem_Level >= MAX_DARK_HORSE_LEVEL)
			return FALSE;	// �ִ� �����̸� ����ġ ��� ����

		BOOL	IsLevelUp = FALSE;

		int nextexp = gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level + 1];
		m_PetItem_Exp += petexp;

		while (m_PetItem_Exp >= nextexp)
		{
			m_PetItem_Level++;
			nextexp = gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level + 1];
			IsLevelUp = TRUE;
		}

		if (IsLevelUp)
		{
#ifdef ADD_380ITEM_NEWOPTION_20060711
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif

			PetValue();
			return TRUE;
		}
	}
	else if (m_Type == MAKE_ITEMNUM(13, 5))
	{	// ��ũ���Ǹ�
		if (m_PetItem_Level >= MAX_DARK_SPIRIT_LEVEL)
			return FALSE;	// �ִ� �����̸� ����ġ ��� ����

		BOOL	IsLevelUp = FALSE;

		int nextexp = gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level + 1];
		m_PetItem_Exp += petexp;

		while (m_PetItem_Exp >= nextexp)
		{
			m_PetItem_Level++;
			nextexp = gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level + 1];
			IsLevelUp = TRUE;
			//m_PetItem_Exp = gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level+1];			
		}

		if (IsLevelUp)
		{
#ifdef ADD_380ITEM_NEWOPTION_20060711
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
			PetValue();
			return TRUE;
		}
	}
	return FALSE;
}

int	CItem::DecPetItemExp(int percent)
{
	if (!m_IsLoadPetItemInfo)
		return FALSE;	// �� ������ ������ �� ��� �Ա� ������ ����ġ ���� ����

	if (m_PetItem_Level < 1)
	{
		m_PetItem_Level = 1;
		m_PetItem_Exp = 0;
#ifdef ADD_380ITEM_NEWOPTION_20060711
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif

		PetValue();
		return FALSE;
	}

	if (m_Type == MAKE_ITEMNUM(13, 4))
	{	// ��ũȣ��		
		int decexp = gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level + 1] - gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level];
		decexp = decexp * percent / 100;
		m_PetItem_Exp -= decexp;

		if (m_PetItem_Exp < 0)
		{
			m_PetItem_Level = 1;
			m_PetItem_Exp = 0;
#ifdef ADD_380ITEM_NEWOPTION_20060711
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
			PetValue();
		}
		else
		{	// ���� �ٿ�
			if (m_PetItem_Exp < gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level])
			{
				m_PetItem_Level--;
#ifdef ADD_380ITEM_NEWOPTION_20060711
				Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
				Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
				PetValue();
				return TRUE;
			}
		}
	}
	else if (m_Type == MAKE_ITEMNUM(13, 5))
	{	// ��ũ���Ǹ�
		int decexp = gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level + 1] - gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level];
		decexp = decexp * percent / 100;
		m_PetItem_Exp -= decexp;

		if (m_PetItem_Exp < 0)
		{
			m_PetItem_Level = 1;
			m_PetItem_Exp = 0;

#ifdef ADD_380ITEM_NEWOPTION_20060711
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
			PetValue();
		}
		else
		{	// ���� �ٿ�
			if (m_PetItem_Exp < gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level])
			{
				m_PetItem_Level--;
#ifdef ADD_380ITEM_NEWOPTION_20060711
				Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
				Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
				PetValue();
				return TRUE;
			}
		}
	}
	return FALSE;
}

int	CItem::PetItemLevelDown(int exp)
{
	if (!m_IsLoadPetItemInfo)
		return FALSE;	// �� ������ ������ �� ��� �Ա� ������ ����ġ ���� ����

	// MODIFY_ACCUMULATED_BUG_DARKLORD_PET_EXP_01_20050603
	// ���� :	Exp�� ���� 1 ���� ���Ҹ� ����.
	//			������ ī����ĳ��, ����, ����ĳ�������� ���� �극��ġ�� ��ġ ȹ���� �����ϴ�.
	//
#ifdef MODIFY_ACCUMULATED_BUG_DARKLORD_PET_EXP_01_20050603
	if (m_PetItem_Level < 2)
		return FALSE;

	m_PetItem_Exp -= exp;
	m_PetItem_Level--;

	// ��ũ ȣ�� 
	if (m_Type == MAKE_ITEMNUM(13, 4))
	{
		int iPrevPetItemLevel = m_PetItem_Level;
		int iPrevExp = gPetItemExp.m_DarkHorseExpTable[iPrevPetItemLevel];

		while (m_PetItem_Exp <= iPrevExp)
		{
			if (iPrevPetItemLevel <= 1)
				break;

			iPrevPetItemLevel--;
			iPrevExp = gPetItemExp.m_DarkHorseExpTable[iPrevPetItemLevel];
		}

		m_PetItem_Level = iPrevPetItemLevel;
	}
	// ��ũ ���Ǹ�
	else if (m_Type == MAKE_ITEMNUM(13, 5))
	{
		int iPrevPetItemLevel = m_PetItem_Level;
		int iPrevExp = gPetItemExp.m_DarkSpiritExpTable[iPrevPetItemLevel];

		while (m_PetItem_Exp <= iPrevExp)
		{
			if (iPrevPetItemLevel <= 1)
				break;

			iPrevPetItemLevel--;
			iPrevExp = gPetItemExp.m_DarkSpiritExpTable[iPrevPetItemLevel];
		}

		m_PetItem_Level = iPrevPetItemLevel;
	}
#else

	m_PetItem_Level--;
	m_PetItem_Exp -= exp;

#endif

#ifdef ADD_380ITEM_NEWOPTION_20060711
	Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
	Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
	PetValue();
	return TRUE;
}

// �� �����ۿ� ���� ����(�� ������ ������, �������ٿ�� ��)
void CItem::PetValue()
{
	if (m_Type == MAKE_ITEMNUM(13, 4))
	{	//��ũ ȣ��
		m_SellMoney = m_PetItem_Level * 2000000;
	}
	else if (m_Type == MAKE_ITEMNUM(13, 5))
	{	// ��ũ���Ǹ�
		m_SellMoney = m_PetItem_Level * 1000000;
	}


	m_BuyMoney = m_SellMoney;

#ifdef BUGFIX_SELL_PRICE_OF_DARKLOAD_PETS_20050310
	m_SellMoney /= 3;
#endif

	if (m_SellMoney >= 1000)
		m_SellMoney = m_SellMoney / 100 * 100;
	else if (m_SellMoney >= 100)
		m_SellMoney = m_SellMoney / 10 * 10;

	if (m_BuyMoney >= 1000)
		m_BuyMoney = m_BuyMoney / 100 * 100;
	else if (m_BuyMoney >= 100)
		m_BuyMoney = m_BuyMoney / 10 * 10;
}
#endif

int CItem::IsExtItem()
{
#ifdef MODIFY_FENRIR_EXOPTION_BUG_20060522	
	// �渱�� ��� : �ı��� ��ȣ������ ������Ʈ �������� �ƴϴ�
	if (m_Type == MAKE_ITEMNUM(13, 37))
		return FALSE;
#endif

	if (m_NewOption & 0x7F) return TRUE;
	return FALSE;
}

//-----------------------------------------------------------------------------
// �������� �Ӽ��� �ִ°�?
int CItem::IsExtLifeAdd()
{
	if ((m_NewOption & 0x20) == 0x20) return 4;
	return 0;
}

// �������� �Ӽ��� �ִ°�?
int CItem::IsExtManaAdd()
{
	if ((m_NewOption & 0x10) == 0x10) return 4;
	return 0;
}

// ������ ���� �Ӽ��� �ִ°�?
int CItem::IsExtDamageMinus()
{
	if ((m_NewOption & 0x08) == 0x08) return 4;
	return 0;
}

// ������ �ݻ� �Ӽ��� �ִ°�?
int CItem::IsExtDamageReflect()
{
#ifdef NEW_FORSKYLAND3
	if ((m_NewOption & 0x04) == 0x04) return 5;
#else
	if ((m_NewOption & 0x04) == 0x04) return 4;
#endif
	return 0;
}

// ��� ������ 
int CItem::IsExtDefenseSuccessfull()
{
	if ((m_NewOption & 0x02) == 0x02) return 10;
	return 0;
}

// ���� ų ������ ��� �� 
int CItem::IsExtMonsterMoney()
{
#ifdef UPDATE_ZEN_DROP_DOWN_20080806	// �� ���� 30% �� ����
	if ((m_NewOption & 0x01) == 0x01) return 30;
#else	// UPDATE_ZEN_DROP_DOWN_20080806
	if ((m_NewOption & 0x01) == 0x01) return 40;
#endif	// UPDATE_ZEN_DROP_DOWN_20080806
	return 0;
}

// ���� �� ����� Ȯ�� ������ �Ӽ�
// ������Ʈ ������ �߰�Ȯ��
int CItem::IsExtExcellentDamage()
{
	// ���и� ������Ʈ �������� �������� �ʴ´�.
	if (m_Type >= ITEM_SHIELD && m_Type < ITEM_HELM) return 0;

	if ((m_NewOption & 0x20) == 0x20) return 10;
	return 0;
}

// ���ݷ� ���� + ����/20 
// ���� ���� + ����/20
int CItem::IsExtAttackRate()
{
	if ((m_NewOption & 0x10) == 0x10) return 1;
	return 0;
}

// ���ݷ� ���� + 2%, 
// ���� ���� + 2%
int CItem::IsExtAttackRate2()
{
	if ((m_NewOption & 0x08) == 0x08) return 2;
	//if( (m_NewOption&0x08) == 0x08 ) return 100;
	return 0;
}

// ���� ���ǵ� ����
int CItem::IsExtAttackSpeed()
{
	if ((m_NewOption & 0x04) == 0x04) return 7;
	return 0;
}

// ���� �׿����� ��� ���� + ����/8
int CItem::IsExtMonsterDieLife()
{
	if ((m_NewOption & 0x02) == 0x02) return 1;
	return 0;
}

// ���� �׿����� ��� ���� + ����/8
int CItem::IsExtMonsterDieMana()
{
	// ���и� ������Ʈ �������� �������� �ʴ´�.
	if (m_Type >= ITEM_SHIELD && m_Type < ITEM_HELM) return 0;

	if ((m_NewOption & 0x01) == 0x01) return 1;
	return 0;
}


// ���� 3%Ȯ���� ���� ���� ���� ����
int CItem::IsWingOpGetOnePercentDamage()
{
	if (m_Durability < 1.f)
		return 0;

#ifdef ADD_THIRD_WING_20070525 // ���湫�� 3%�� 2������ �ɼ�
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ���� ���� ����
	if ((m_Type >= MAKE_ITEMNUM(12, 3) && m_Type <= MAKE_ITEMNUM(12, 6)) || m_Type == MAKE_ITEMNUM(13, 30)
		|| m_Type == MAKE_ITEMNUM(12, 42))
#else
	if ((m_Type >= MAKE_ITEMNUM(12, 3) && m_Type <= MAKE_ITEMNUM(12, 6)) || m_Type == MAKE_ITEMNUM(13, 30))
#endif
	{
		if ((m_NewOption & PLUS_WING_ONE_PERCENT_DAMAGE))
			return 3;
	}
#else
	if ((m_NewOption&PLUS_WING_ONE_PERCENT_DAMAGE)) return 3;
#endif	

	return 0;
}

// ���� ���Ϳ��� �޴� �ִ� Ÿ�� ���������� HP 10%ȹ��
/*
int CItem::IsWingOpGetLifeToMonster()
{
	if( m_Durability < 1.f )
		return 0;

	if( (m_NewOption&PLUS_WING_GETLIFE_TO_MONSTER)  == 0x01 ) return 10;
	return 0;
}*/


// ���� ���Ϳ��� �޴� �ִ� Ÿ�� ���������� MP 10%ȹ��	
int CItem::IsWingOpGetManaToMoster()
{
	if (m_Durability < 1.f)
		return 0;

#ifdef ADD_THIRD_WING_20070525	// MP 10%ȹ���� 2������ �ɼ�
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
	if ((m_Type >= MAKE_ITEMNUM(12, 3) && m_Type <= MAKE_ITEMNUM(12, 6)) || m_Type == MAKE_ITEMNUM(13, 30)
		|| m_Type == MAKE_ITEMNUM(12, 42))
#else
	if ((m_Type >= MAKE_ITEMNUM(12, 3) && m_Type <= MAKE_ITEMNUM(12, 6)) || m_Type == MAKE_ITEMNUM(13, 30))
#endif
#endif
		if ((m_NewOption&PLUS_WING_GETMANA_TO_MONSTER) == 0x01) return 10;

	return 0;
}

// ����Ʈ 5% ��������� �߰�
int CItem::IsDinorantReduceAttackDamaege()
{
	if (m_Durability < 1.f)
		return 0;

	if ((m_NewOption&PLUS_DINORANT_PLUS_REDUCE_DAMAGE) == 0x01) return 5;
	return 0;
}

#ifdef ADD_ITEM_FENRIR_01_20051110
// �渱 ���� ����������, ���� �߰�
int CItem::IsFenrirIncLastAttackDamage()
{
	if (m_Durability < 1.f)
		return 0;

	if ((m_NewOption&PLUS_FENRIR_INCREASE_LAST_DAMAGE) == PLUS_FENRIR_INCREASE_LAST_DAMAGE) return 10;
	return 0;

}

int CItem::IsFenrirDecLastAttackDamage()
{
	if (m_Durability < 1.f)
		return 0;

	if ((m_NewOption&PLUS_FENRIR_DECREASE_LAST_DAMAGE) == PLUS_FENRIR_DECREASE_LAST_DAMAGE) return 10;
	return 0;
}
#endif

#ifdef ADD_ILLUSION_FENRIR_20070703
int CItem::IsIllusionFenrirOption()
{
	if (m_Durability < 1.f)
		return 0;

	if ((m_NewOption&PLUS_ILLUSION_FENRIR_OPTION) == PLUS_ILLUSION_FENRIR_OPTION) return 1;
	return 0;
}
#endif

#ifdef ADD_THIRD_WING_20070525	// 3�� ���� �߰��ɼ� �� �Լ�����
// ���湫�� Ȯ���� �ǵ����ش�.
int CItem::GetIgnoreDefencePer()
{
	// _MODIFY_ADD_THIRD_WING_20070623
	// 3�� ������ �����Ѵ�.
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
	if ((m_Type >= MAKE_ITEMNUM(12, 36) && m_Type <= MAKE_ITEMNUM(12, 40))
		|| m_Type == MAKE_ITEMNUM(12, 43))
#else
	if (m_Type >= MAKE_ITEMNUM(12, 36) && m_Type <= MAKE_ITEMNUM(12, 40))
#endif
	{
		if (m_Durability < 1.f)
			return 0;

		if ((m_NewOption & PLUS_THIRD_WING_5PER_IGNORE_DEFENSE) == PLUS_THIRD_WING_5PER_IGNORE_DEFENSE)
			return 5;
	}
	return 0;
}
#endif	// ADD_THIRD_WING_20070525

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
int CItem::SimpleDurabilityDown(int iDur)
{
	m_Durability -= 1;

	if (m_Durability < 0.f)
	{
		m_Durability = 0.f;
		return 0;
	}

	if (CheckDurabilityState())
	{	// ������ ���ҿ� ���� �г�Ƽ�� ����ȴ� 
		return 2;
	}

	return (int)m_Durability;
}
#endif

int CItem::DurabilityDown(int dur, int aIndex)
{
	if (m_Durability <= 0.f)
	{
		return 0;
	}

	m_DurabilitySmall += dur;

#if defined(MASTER_LEVEL_SKILL_SYSTEM_20071122) && !defined(CHARACTERDB_SERVER)	// �нú� 1�� - ��������2 : ����/�����/���� ���� ����
	int iBaseDurSmall = 564;
	if (gObj[aIndex].m_MPSkillOpt.iMpsDownDur2 > 0)
	{
		iBaseDurSmall += (iBaseDurSmall * gObj[aIndex].m_MPSkillOpt.iMpsDownDur2) / 100;
	}

	if (m_DurabilitySmall > iBaseDurSmall)
#else
	if (m_DurabilitySmall > 564)
#endif
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1;

		if (m_Durability < 0.f)
		{
			m_Durability = 0.f;
		}

#if TESTSERVER == 1 

		char msg[255];
		wsprintf(msg, "%s:���������� %d/%d", GetName(), (int)m_Durability, (int)m_BaseDurability);
		//GCServerMsgStringSend(msg, aIndex, 1);	
		LogAddTD(msg);
#endif

		if (CheckDurabilityState())
		{	// ������ ���ҿ� ���� �г�Ƽ�� ����ȴ� 
			return 2;
		}
		return 1;
	}
	return (int)m_Durability;
}

int CItem::DurabilityDown2(int dur, int aIndex)
{
	if (m_Durability <= 0.f)
	{
		return 0;
	}

	m_DurabilitySmall += dur;

#if defined(MASTER_LEVEL_SKILL_SYSTEM_20071122) && !defined(CHARACTERDB_SERVER)	// �нú� 1�� - ��������1 : �⺻ ���� ����
	int iBaseDurSmall = 1000;
	if (gObj[aIndex].m_MPSkillOpt.iMpsDownDur1 > 0)
	{
		iBaseDurSmall += (iBaseDurSmall * gObj[aIndex].m_MPSkillOpt.iMpsDownDur1) / 100;
	}

	if (m_DurabilitySmall > iBaseDurSmall)
#else
	if (m_DurabilitySmall > 1000)
#endif
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1;

		if (m_Durability < 0.f)
		{
			m_Durability = 0.f;
		}
#if TESTSERVER == 1 

		char msg[255];
		wsprintf(msg, "%s:���������� %d/%d", GetName(), (int)m_Durability, (int)m_BaseDurability);
		//GCServerMsgStringSend(msg, aIndex, 1);	
		LogAddTD(msg);
#endif

		if (CheckDurabilityState())
		{	// ������ ���ҿ� ���� �г�Ƽ�� ����ȴ� 
			return 2;
		}
		return 1;
	}
	return 0;
}

// ��/â/����/�б� ���� ������ �������� ���� �����Ѵ�.
int CItem::NormalWeaponDurabilityDown(int defence, int aIndex)
{
	if (m_Durability <= 0.f)
	{
		return 0;
	}

	int damagemin = m_DamageMinOrigin;
	int plusdamage = 0;
	PlusSpecial(&plusdamage, AT_IMPROVE_DAMAGE);

	if (damagemin == 0)
	{
		LogAdd("damagemin�� 0�̴�");
		return 0;
	}

	int div = damagemin + (damagemin / 2) + plusdamage;

	if (div == 0)
		return 0;

	int DecreaseDur = (defence * 2) / div;

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	DecreaseDur -= (DecreaseDur * m_ImproveDurabilityRate) / 100;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

	m_DurabilitySmall += DecreaseDur;
#if defined(MASTER_LEVEL_SKILL_SYSTEM_20071122) && !defined(CHARACTERDB_SERVER)	// �нú� 1�� - ��������1 : �⺻ ���� ����
	int iBaseDurSmall = 564;
	if (gObj[aIndex].m_MPSkillOpt.iMpsDownDur1 > 0)
	{
		iBaseDurSmall += (iBaseDurSmall * gObj[aIndex].m_MPSkillOpt.iMpsDownDur1) / 100;
	}

	if (m_DurabilitySmall > iBaseDurSmall)
#else
	if (m_DurabilitySmall > 564)
#endif
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1;

		if (m_Durability < 0.f)
		{
			m_Durability = 0.f;
		}

		if (CheckDurabilityState())
		{	// ������ ���ҿ� ���� �г�Ƽ�� ����ȴ� 
			return 2;
		}
		return 1;
	}
	return 0;
}

// Ȱ ���� ������ �������� ���� �����Ѵ�.
int CItem::BowWeaponDurabilityDown(int defence, int aIndex)
{
	if (m_Durability <= 0.f)
	{
		return 0;
	}

	int damagemin = m_DamageMinOrigin;
	int plusdamage = 0;
	PlusSpecial(&plusdamage, AT_IMPROVE_DAMAGE);

	int div = damagemin + (damagemin / 2) + plusdamage;

	if (div == 0)
		return 0;

	int DecreaseDur = (defence * 2) / div;

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	DecreaseDur -= (DecreaseDur * m_ImproveDurabilityRate) / 100;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

	m_DurabilitySmall += DecreaseDur;
#if defined(MASTER_LEVEL_SKILL_SYSTEM_20071122) && !defined(CHARACTERDB_SERVER)	// �нú� 1�� - ��������1 : Ȱ ����
	int iBaseDurSmall = 780;
	if (gObj[aIndex].m_MPSkillOpt.iMpsDownDur1 > 0)
	{
		iBaseDurSmall += (iBaseDurSmall * gObj[aIndex].m_MPSkillOpt.iMpsDownDur1) / 100;
	}

	if (m_DurabilitySmall > iBaseDurSmall)
#else
	if (m_DurabilitySmall > 780)
#endif
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1;

		if (m_Durability < 0.f)
		{
			m_Durability = 0.f;
		}
		if (CheckDurabilityState())
		{	// ������ ���ҿ� ���� �г�Ƽ�� ����ȴ� 
			return 2;
		}
		return 1;
	}
	return 0;
}

// ������ ���� ������ �������� ���� �����Ѵ�.
int CItem::StaffWeaponDurabilityDown(int defence, int aIndex)
{
	if (m_Durability <= 0.f)
	{
		return 0;
	}

	int magic = (m_Magic / 2) + (m_Level * 2);
	int plusmagic = 0;

	PlusSpecial(&plusmagic, AT_IMPROVE_MAGIC);

	int div = magic + magic / 3 + plusmagic;
	if (div == 0)
		return 0;

	int DecreaseDur = defence / div;

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	DecreaseDur -= (DecreaseDur * m_ImproveDurabilityRate) / 100;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

	m_DurabilitySmall += DecreaseDur;
#if defined(MASTER_LEVEL_SKILL_SYSTEM_20071122) && !defined(CHARACTERDB_SERVER)	// �нú� 1�� - ��������1 : ������ ����
	int iBaseDurSmall = 1050;
	if (gObj[aIndex].m_MPSkillOpt.iMpsDownDur1 > 0)
	{
		iBaseDurSmall += (iBaseDurSmall * gObj[aIndex].m_MPSkillOpt.iMpsDownDur1) / 100;
	}

	if (m_DurabilitySmall > iBaseDurSmall)
#else
	if (m_DurabilitySmall > 1050)
#endif
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1;

		if (m_Durability < 0.f)
		{
			m_Durability = 0.f;
		}

		if (CheckDurabilityState())
		{	// ������ ���ҿ� ���� �г�Ƽ�� ����ȴ� 
			return 2;
		}
		return 1;
	}
	return 0;
}

// �� ������ �������� ���� �����Ѵ�.
int CItem::ArmorDurabilityDown(int damagemin, int aIndex)
{
	if (m_Durability <= 0.f)
	{
		return 0;
	}

	if (m_Type == -1)
	{
		return FALSE;
	}

	int def = m_DefenseOrigin;
	int plusdef = 0;

	if (def == 0)
	{
		LogAdd("def�� 0�̴�");
		return 0;
	}

	int DecreaseDur = 0;
	if ((m_Type >= ITEM_SHIELD && m_Type < ITEM_HELM))
	{	// ���з�
		PlusSpecial(&plusdef, AT_IMPROVE_BLOCKING);
		DecreaseDur = damagemin / (def * 5 + plusdef);
	}
	else if (m_RequireClass[CLASS_WIZARD])
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin / (def * 3 + plusdef);
	}
	else if (m_RequireClass[CLASS_KNIGHT])
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin / (def * 3 + plusdef);
	}
	else if (m_RequireClass[CLASS_ELF])
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin / (def * 2 + plusdef);
	}
	else if (m_RequireClass[CLASS_MAGUMSA])
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin / (def * 7 + plusdef);
	}
#ifdef DARKLORD_WORK
	else if (m_RequireClass[CLASS_DARKLORD])
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin / (def * 6 + plusdef);
	}
#endif
#ifdef MODIFY_SUMMONER_ITEM_DURABILITY_DOWN_BUGFIX_20071213
	else if (m_RequireClass[CLASS_SUMMONER])
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin / (def * 3 + plusdef);
	}
#endif	

	m_DurabilitySmall += DecreaseDur;

#if defined(MASTER_LEVEL_SKILL_SYSTEM_20071122) && !defined(CHARACTERDB_SERVER)	// �нú� 1�� - ��������1 : �� ����
	int iBaseDurSmall = 69;
	if (gObj[aIndex].m_MPSkillOpt.iMpsDownDur1 > 0)
	{
		iBaseDurSmall += (iBaseDurSmall * gObj[aIndex].m_MPSkillOpt.iMpsDownDur1) / 100;
	}

	if (m_DurabilitySmall > iBaseDurSmall)
#else
	if (m_DurabilitySmall > 69)
#endif
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1;

		if (m_Durability < 0.f)
		{
			m_Durability = 0.f;
		}

		if (CheckDurabilityState())
		{	// ������ ���ҿ� ���� �г�Ƽ�� ����ȴ� 
			return 2;
		}
		return 1;
	}
	return 0;
}

int	CItem::CheckDurabilityState()
{
	if (m_Durability == 0.f)
	{
		if (m_CurrentDurabilityState != 1.f)
		{
			m_CurrentDurabilityState = 1.f;

#ifdef ADD_380ITEM_NEWOPTION_20060711
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
			return TRUE;
		}
	}
	else if (m_Durability < m_DurabilityState[2])
	{
		if (m_CurrentDurabilityState != 0.5f)
		{
			m_CurrentDurabilityState = 0.5f;
#ifdef ADD_380ITEM_NEWOPTION_20060711
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
			return TRUE;
		}
	}
	else if (m_Durability < m_DurabilityState[1])
	{
		if (m_CurrentDurabilityState != 0.3f)
		{
			m_CurrentDurabilityState = 0.3f;
#ifdef ADD_380ITEM_NEWOPTION_20060711
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
			return TRUE;
		}
	}
	else if (m_Durability < m_DurabilityState[0])
	{
		if (m_CurrentDurabilityState != 0.2f)
		{
			m_CurrentDurabilityState = 0.2f;
#ifdef ADD_380ITEM_NEWOPTION_20060711
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
			return TRUE;
		}
	}
	else
	{
		if (m_CurrentDurabilityState != 0)
		{
			m_CurrentDurabilityState = 0.f;
#ifdef ADD_380ITEM_NEWOPTION_20060711
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption, m_ItemOptionEx);
#else
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
#endif
			return TRUE;
		}
	}
	return FALSE;
}


#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
// ���� ������ �����ִ� ���������� Ȯ��
bool CItem::IsEnableSetSocketItem()
{
	for (int i = 0; i < MAX_SOCKET_SLOT; i++)
	{
		if (m_SocketOption[i] == SOCKETSLOT_EMPTY)
		{
			return true;
		}
	}

	return false;
}

// �õ彺�Ǿ ���Ͽ� ����
bool CItem::SetSeedSphere(BYTE SlotIndex, BYTE SeedIndex, BYTE SphereLevel)
{
	BYTE SeedSphereIndex = 0;

	if (SlotIndex < 0 || SlotIndex >= MAX_SOCKET_SLOT)
	{
		return false;
	}

	// �õ彺�Ǿ��� �ε��� = �õ��ε��� + ( ���Ǿ�� * 50 )
	SeedSphereIndex = (SphereLevel * 50) + SeedIndex;

	if (m_SocketOption[SlotIndex] != SOCKETSLOT_NONE && m_SocketOption[SlotIndex] == SOCKETSLOT_EMPTY)
	{
		m_SocketOption[SlotIndex] = SeedSphereIndex;
		return true;
	}

	return false;
}

// �õ彺�Ǿ ���Ͽ��� ����
bool CItem::RemoveSeedSphere(BYTE SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= MAX_SOCKET_SLOT)
	{
		return false;
	}

	if (m_SocketOption[SlotIndex] != SOCKETSLOT_NONE && m_SocketOption[SlotIndex] != SOCKETSLOT_EMPTY)
	{
		m_SocketOption[SlotIndex] = SOCKETSLOT_EMPTY;
		return true;
	}

	return false;
}

// ���� ������ ����
void CItem::MakeSocketSlot(BYTE SlotCount)
{
#ifndef CHARACTERDB_SERVER
	if (g_SocketOptionSystem.IsEnableSocketItem(this) == false)
	{
		return;
	}

	for (int i = 0; i < SlotCount; i++)
	{
		m_SocketOption[i] = SOCKETSLOT_EMPTY;
	}

#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	m_BonusSocketOption = SOCKETSLOT_NONE;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
#endif
}

#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415


//-----------------------------------------------------------------------------

void BufferItemtoConvert3(LPBYTE buf, int & type, BYTE& level, BYTE & op1, BYTE & op2, BYTE & op3, BYTE & dur)
{
#ifdef ITEM_INDEX_EXTEND_20050706
	type = buf[0] + ((buf[3] & 0x80) << 1) + ((buf[5] & 0xF0) << 5);
#else
	type = buf[0] + (((buf[3] & 0x80) >> 7) * 256);
#endif	
	level = (buf[1] >> 3) & 0xF;
	op1 = (buf[1] >> 7) & 0x01;
	op2 = (buf[1] >> 2) & 0x01;
	op3 = (buf[1] & 0x03);
	dur = buf[2];

	if (type == MAKE_ITEMNUM(13, 3))
	{
		op3 |= (buf[3] & 0x40) >> 4;
	}
	else
	{
		if ((buf[3] & 0x40) == 0x40)
		{
			op3 += 4;
		}
	}
}

//----------------------------------------------------------------------------
// ������ ������ 7����Ʈ ������ �ٲ۴�.
//----------------------------------------------------------------------------
void ItemByteConvert7(LPBYTE buf, CItem item[], int maxitem)
{
	int n = 0;
	WORD hiWord, loWord;


	// type / index, Level, Dur, Special, Number
	//   4  ,   4  ,   5,   3,    8        4Byte
	for (int index = 0; index < maxitem; index++)
	{
		buf[n] = (BYTE)item[index].m_Type;				// Type       : 4bit
		n++;

		buf[n] = 0;
		buf[n] |= item[index].m_Level << 3;		// Level	: 5bit
		buf[n] |= item[index].m_Option1 << 7;		// Option 1 : 1bit
		buf[n] |= item[index].m_Option2 << 2;		// Option 2 : 1bit
		buf[n] |= item[index].m_Option3;			// Option 3 : 2bit

		n++;
		buf[n] = (BYTE)item[index].m_Durability;			// Durability : 8bit
		n++;

		hiWord = HIWORD(item[index].m_Number);
		loWord = LOWORD(item[index].m_Number);

		buf[n] = HIBYTE(hiWord);
		n++;
		buf[n] = LOBYTE(hiWord);
		n++;
		buf[n] = HIBYTE(loWord);
		n++;
		buf[n] = LOBYTE(loWord);
		n++;
	}
}



//----------------------------------------------------------------------------
// ������ ������ 10 ����Ʈ ������ �ٲ۴�.
//----------------------------------------------------------------------------
void ItemByteConvert10(LPBYTE buf, CItem item[], int maxitem)
{
	int n = 0;
	WORD hiWord, loWord;


	// type / index, Level, Dur, Special, Number, extension
	//   4  ,   4  ,   5,   3,    8        4Byte,   3Byte
	for (int index = 0; index < maxitem; index++)
	{
#ifdef FOR_BLOODCASTLE
		if (item[index].m_Type == MAKE_ITEMNUM(13, 19))
		{		// DB�� �����ϱ� ���� �׻� ��õ�� �ø�� Ȯ���ϰ� �����Ѵٸ� �����󿡼� �����.
			if (CHECK_LIMIT(item[index].m_Level, 3))
			{
				buf[n] = 255;
				buf[n + 1] = 255;
				buf[n + 2] = 255;
				buf[n + 3] = 255;
				buf[n + 4] = 255;
				buf[n + 5] = 255;
				buf[n + 6] = 255;
				buf[n + 7] = 255;
				buf[n + 8] = 255;
				buf[n + 9] = 255;
				n += MAX_ITEMDBBYTE;
				continue;
			}
		}
#endif

		if (item[index].m_Type < 0)
		{
			buf[n] = 255;
			buf[n + 1] = 255;
			buf[n + 2] = 255;
			buf[n + 3] = 255;
			buf[n + 4] = 255;
			buf[n + 5] = 255;
			buf[n + 6] = 255;
			buf[n + 7] = 255;
			buf[n + 8] = 255;
			buf[n + 9] = 255;
			n += MAX_ITEMDBBYTE;
		}
		else
		{
			buf[n] = (BYTE)item[index].m_Type % 256;				// Type       : 4bit

			n++;

			buf[n] = 0;
			buf[n] |= item[index].m_Level << 3;		// Level	: 5bit
			buf[n] |= item[index].m_Option1 << 7;		// Option 1 : 1bit
			buf[n] |= item[index].m_Option2 << 2;		// Option 2 : 1bit
			buf[n] |= (item[index].m_Option3 & 0x03);	// Option 3 : 2bit

			n++;
			buf[n] = (BYTE)item[index].m_Durability;			// Durability : 8bit
			n++;

			hiWord = HIWORD(item[index].m_Number);
			loWord = LOWORD(item[index].m_Number);

			buf[n] = HIBYTE(hiWord);
			n++;
			buf[n] = LOBYTE(hiWord);
			n++;
			buf[n] = HIBYTE(loWord);
			n++;
			buf[n] = LOBYTE(loWord);
			n++;

			buf[n] = 0;
			if (item[index].m_Type > 255) buf[n] = 128;
			if (item[index].m_Option3 > 3) buf[n] |= 64;
			buf[n] |= item[index].m_NewOption;
			n++;
#if GAME_VERSION >= G_V_99B
			buf[n] = item[index].m_SetOption;
#endif
			n++;
			n++;
		}
	}
}


//----------------------------------------------------------------------------
// ������ ������ N����Ʈ ������ �ٲ۴�. -> Ŭ���̾�Ʈ�� ������ ������ ���� 
// (DB���� 0x03:7����Ʈ - 20050721)
//----------------------------------------------------------------------------
void ItemByteConvert(LPBYTE buf, CItem item)
{
	int n = 0;

#ifdef ITEM_INDEX_EXTEND_20050706
	memset(buf, 0, ITEM_BUFFER_SIZE);
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
	buf[n] = (BYTE)(item.m_Type & 0x00FF);		// [0]	������ ��ȣ : 1 ~ 8��° bit����
#else
	buf[n] = (BYTE)item.m_Type % 256;
#endif
	n++;

	buf[n] = 0;
	buf[n] |= item.m_Level << 3;					// [1]	Level		: 5bit
	buf[n] |= item.m_Option1 << 7;				// [1]	Option 1	: 1bit
	buf[n] |= item.m_Option2 << 2;				// [1]	Option 2	: 1bit
	buf[n] |= item.m_Option3 & 0x03;				// [1]	Option 3	: 2bit

	n++;
	buf[n] = (BYTE)item.m_Durability;			// [2]	Durability	: 8bit
	n++;
	buf[n] = 0;
#ifdef ITEM_INDEX_EXTEND_20050706
	buf[n] |= (BYTE)((item.m_Type & 0x0100) >> 1);	// [3]	������ ��ȣ : 9��° bit
#else
	if (item.m_Type > 255)
		buf[n] = 0x80;
#endif
	if (item.m_Option3 > 3)
		buf[n] |= 0x40;							// [3]	Option 3 16�� ó��
	buf[n] |= item.m_NewOption;					// [3]	�׼���Ʈ �ɼ�

	n++;
#if GAME_VERSION >= G_V_99B
	buf[n] = item.m_SetOption;					// [4]	��Ʈ �ɼ� 
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
	n++;
	buf[n] |= (BYTE)((item.m_Type & 0x1E00) >> 5);	// [5]	������ ��ȣ : 10 ~ 13��° bit����

#ifdef ADD_380ITEM_NEWOPTION_20060711
	BYTE btItemEffeftFor380 = 0;
	// 380 ������ �߰� �ɼ� : m_ItemOptionEx�� ���� 1 bit ��
	// [5] ������ ���� 5 ��° ���� ����
	btItemEffeftFor380 = (item.m_ItemOptionEx & 0x80) >> 4;   // ���� 8�̵�.
	buf[n] |= btItemEffeftFor380;
#endif // ADD_380ITEM_NEWOPTION_20060711

	n++;

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	buf[n] = item.m_JewelOfHarmonyOption;		// [6] ��ȭ�Ǻ��� ��ȭ �ɼ� : 1����Ʈ
												// ���� ��� ���
#else
	// ���� ����Ʈ								// [6] ���� ����Ʈ 
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#ifndef CHARACTERDB_SERVER
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	if (g_SocketOptionSystem.IsSocketItem(&item) == true)
#else
	if (g_SocketOptionSystem.IsEnableSocketItem(&item) == true)
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	{
		buf[n] = item.m_BonusSocketOption;		// [6] ���ʽ� ���� �ɼ� : 1����Ʈ
												// ���� ������ �� ��� ��ȭ�� ���� �ɼ��� ������� �ʴ´�.
	}
	// ���� ������ �����͸� �Է��Ѵ�.			// [7] ~ [11] : ���� ������ ����
	n++;
	buf[n] = item.m_SocketOption[0];
	n++;
	buf[n] = item.m_SocketOption[1];
	n++;
	buf[n] = item.m_SocketOption[2];
	n++;
	buf[n] = item.m_SocketOption[3];
	n++;
	buf[n] = item.m_SocketOption[4];
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#endif
#endif	
}


//----------------------------------------------------------------------------
// ������ ������ N����Ʈ ������ �ٲ۴�. -> Ŭ���̾�Ʈ�� ������ ������ ���� 
// (DB���� 0x03:7����Ʈ - 20050721)
//----------------------------------------------------------------------------
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE JewelOfHarmonyOption, BYTE ItemEffectEx, BYTE SocketOption[], BYTE SocketBonusOption)
#else

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#ifdef ADD_380ITEM_NEWOPTION_20060711
void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE JewelOfHarmonyOption, BYTE ItemEffectEx)
#else
void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE JewelOfHarmonyOption)
#endif // ADD_380ITEM_NEWOPTION_20060711
#else
#ifdef ADD_380ITEM_NEWOPTION_20060711
void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE ItemEffectEx)
#else
void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption)
#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
{

	// 20051114 �ʱ�ȭ�ڵ� ����.
#ifdef ITEM_INDEX_EXTEND_20050706
	memset(buf, 0, ITEM_BUFFER_SIZE);
#endif

	int n = 0;

#ifdef ITEM_INDEX_EXTEND_20050706
	buf[n] = (BYTE)(type & 0x00FF);			// [0]	������ ��ȣ : 1 ~ 8��° bit����
#else
	buf[n] = (BYTE)type % 256;				// Type       : 4bit
#endif
	n++;
	buf[n] = 0;
	buf[n] |= level << 3;						// [1]	Level		: 5bit
	buf[n] |= Option1 << 7;					// [1]	Option 1	: 1bit
	buf[n] |= Option2 << 2;					// [1]	Option 2	: 1bit
	buf[n] |= Option3 & 0x03;					// [1]	Option 3	: 2bit
	n++;
	buf[n] = (BYTE)dur;						// [2]	Durability	: 8bit
	n++;
	buf[n] = 0;

#ifdef ITEM_INDEX_EXTEND_20050706
	buf[n] |= (BYTE)((type & 0x0100) >> 1);		// [3]	������ ��ȣ : 9��° bit
#else
	if (type > 255)
		buf[n] = 0x80;
#endif

	if (Option3 > 3)
		buf[n] |= 64;						// [3]	Option 3 16�� ó��
	buf[n] |= Noption;						// [3]	�׼���Ʈ �ɼ�

	n++;
#if GAME_VERSION >= G_V_99B
	buf[n] = SetOption;						// [4]	��Ʈ �ɼ� 
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
	n++;
	buf[n] |= (BYTE)((type & 0x1E00) >> 5);		// [5]	������ ��ȣ : 10 ~ 13��° bit����

#ifdef ADD_380ITEM_NEWOPTION_20060711
	BYTE btItemEffeftFor380 = 0;
	// 380 ������ �߰� �ɼ� : m_ItemOptionEx�� ���� 1 bit ��
	// [5] ������ ���� 5 ��° ���� ����
	btItemEffeftFor380 = (ItemEffectEx & 0x80) >> 4;   // ���� 8�̵�.
	buf[n] |= btItemEffeftFor380;
#endif // ADD_380ITEM_NEWOPTION_20060711

	n++;

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	buf[n] = JewelOfHarmonyOption;				// [6] ��ȭ�Ǻ��� ��ȭ �ɼ� : 1����Ʈ
												// ���� ��� ���
#else
	// ���� ����Ʈ								// [6] ���� ����Ʈ 
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530	
#endif	

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	if (SocketOption != NULL)
	{
		buf[n] = SocketBonusOption;				// [6] ���ʽ� ���� �ɼ� : 1����Ʈ
												// ���� �������� ��� ��ȭ�� �ɼ� ��� ���ʽ� ���� �ɼ��� ����.
		n++;
		buf[n] = SocketOption[0];				// [7] ���� �ɼ� : 1����Ʈ
		n++;
		buf[n] = SocketOption[1];				// [8] ���� �ɼ� : 1����Ʈ
		n++;
		buf[n] = SocketOption[2];				// [9] ���� �ɼ� : 1����Ʈ
		n++;
		buf[n] = SocketOption[3];				// [10] ���� �ɼ� : 1����Ʈ
		n++;
		buf[n] = SocketOption[4];				// [11] ���� �ɼ� : 1����Ʈ
	}
	else
	{
		n++;
		buf[n] = SOCKETSLOT_NONE;
		n++;
		buf[n] = SOCKETSLOT_NONE;
		n++;
		buf[n] = SOCKETSLOT_NONE;
		n++;
		buf[n] = SOCKETSLOT_NONE;
		n++;
		buf[n] = SOCKETSLOT_NONE;
	}
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
}


#ifdef ITEM_DBSIZE_EXTEND_20050706

//----------------------------------------------------------------------------
// ������ ������ 16 ����Ʈ ������ �ٲ۴�.
//----------------------------------------------------------------------------
void ItemByteConvert16(LPBYTE buf, CItem item[], int maxitem)
{
	int n = 0;
	WORD hiWord, loWord;

	// type / index, Level, Dur, Special, Number, extension
	//   4  ,   4  ,   5,   3,    8        4Byte,   3Byte
	for (int index = 0; index < maxitem; index++)
	{
#ifdef FOR_BLOODCASTLE
		if (item[index].m_Type == MAKE_ITEMNUM(13, 19))
		{
			// DB�� �����ϱ� ���� �׻� ��õ�� �ø�� Ȯ���ϰ� �����Ѵٸ� �����󿡼� �����.
			if (CHECK_LIMIT(item[index].m_Level, 3))
			{
				buf[n] = 255;
				buf[n + 1] = 255;
				buf[n + 2] = 255;
				buf[n + 3] = 255;
				buf[n + 4] = 255;
				buf[n + 5] = 255;
				buf[n + 6] = 255;
				buf[n + 7] = 255;
				buf[n + 8] = 255;
				buf[n + 9] = 255;
				buf[n + 10] = 255;
				buf[n + 11] = 255;
				buf[n + 12] = 255;
				buf[n + 13] = 255;
				buf[n + 14] = 255;
				buf[n + 15] = 255;
				n += MAX_ITEMDBBYTE;
				continue;
			}
		}
#endif

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724	// DB�� �����ϱ� ���� ������ ������ �����󿡼� ���
		if (item[index].m_Type == MAKE_ITEMNUM(14, 64))
		{
			buf[n] = 255;
			buf[n + 1] = 255;
			buf[n + 2] = 255;
			buf[n + 3] = 255;
			buf[n + 4] = 255;
			buf[n + 5] = 255;
			buf[n + 6] = 255;
			buf[n + 7] = 255;
			buf[n + 8] = 255;
			buf[n + 9] = 255;
			buf[n + 10] = 255;
			buf[n + 11] = 255;
			buf[n + 12] = 255;
			buf[n + 13] = 255;
			buf[n + 14] = 255;
			buf[n + 15] = 255;
			n += MAX_ITEMDBBYTE;
			continue;
		}
#endif

		if (item[index].m_Type < 0)
		{
			buf[n] = 255;
			buf[n + 1] = 255;
			buf[n + 2] = 255;
			buf[n + 3] = 255;
			buf[n + 4] = 255;
			buf[n + 5] = 255;
			buf[n + 6] = 255;
			buf[n + 7] = 255;
			buf[n + 8] = 255;
			buf[n + 9] = 255;
			buf[n + 10] = 255;
			buf[n + 11] = 255;
			buf[n + 12] = 255;
			buf[n + 13] = 255;
			buf[n + 14] = 255;
			buf[n + 15] = 255;
			n += MAX_ITEMDBBYTE;
		}
		else
		{
#ifdef ITEM_INDEX_EXTEND_20050706
			memset(&buf[n], 0, MAX_ITEMDBBYTE);
#endif

			buf[n] = (BYTE)item[index].m_Type % 256;	// [0]			
													// Type       : 4bit

			n++;

			buf[n] = 0;								// [1]
			buf[n] |= item[index].m_Level << 3;		// Level	: 5bit
			buf[n] |= item[index].m_Option1 << 7;		// Option 1 : 1bit
			buf[n] |= item[index].m_Option2 << 2;		// Option 2 : 1bit
			buf[n] |= (item[index].m_Option3 & 0x03);	// Option 3 : 2bit

			n++;
			buf[n] = (BYTE)item[index].m_Durability;// [2] ������		
													// Durability : 8bit
			n++;

			hiWord = HIWORD(item[index].m_Number);	// [3] ������ �ø��� 
			loWord = LOWORD(item[index].m_Number);

			buf[n] = HIBYTE(hiWord);
			n++;
			buf[n] = LOBYTE(hiWord);				// [4] ������ �ø���
			n++;
			buf[n] = HIBYTE(loWord);				// [5] ������ �ø���
			n++;
			buf[n] = LOBYTE(loWord);				// [6] ������ �ø���
			n++;

			buf[n] = 0;								// [7] ������ ��ȣ : 9��° bit
													// [7] ������ 16�ɼ� ó��
													// [7] ������Ʈ ������ �ɼ�

#ifdef ITEM_INDEX_EXTEND_20050706
			BYTE btItemIndexEx = 0;
			btItemIndexEx |= (BYTE)((item[index].m_Type & 0x1E00) >> 5);	// �ű� Ȯ�� 4��Ʈ
			buf[n] |= (BYTE)((item[index].m_Type & 0x0100) >> 1);			// ���� Ȯ�� 1��Ʈ
#else
			if (item[index].m_Type > 255) buf[n] = 128;
#endif
			if (item[index].m_Option3 > 3) buf[n] |= 64;
			buf[n] |= item[index].m_NewOption;
			n++;

			buf[n] = item[index].m_SetOption;		// [8] ��Ʈ������ �ɼ�

			n++;

#ifdef ITEM_INDEX_EXTEND_20050706
			buf[n] = 0;								// [9] ������ ��ȣ : 10 ~ 13��° bit����
			buf[n] |= btItemIndexEx;
#endif			

#ifdef ADD_380ITEM_NEWOPTION_20060711
			BYTE btItemEffeftFor380 = 0;
			// 380 ������ �߰� �ɼ� : m_ItemOptionEx�� ���� 1 bit ��
			// [9] ������ ���� 5 ��° ���� ����
			btItemEffeftFor380 = (item[index].m_ItemOptionEx & 0x80) >> 4;   // ���� 8�̵�.
			buf[n] |= btItemEffeftFor380;
#endif

			n++;

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#ifdef MODIFY_HARMONY_OPTION_INIT_VALUE_FIX_20061019
			if (item[index].m_JewelOfHarmonyOption == 0xFF)
			{
				// ��ȭ�� ���� �ɼ��� 0xFF(�߸��� �ʱⰪ)�� ��� 0x00���� ������ �����Ѵ�.
				item[index].m_JewelOfHarmonyOption = 0x00;
			}
#endif // MODIFY_HARMONY_OPTION_INIT_VALUE_FIX_20061019
			buf[n] = item[index].m_JewelOfHarmonyOption;	// [10] ��ȭ���� �ɼ� : 
															// 4 Bit : 0 ~ 15. ��ȭ �ɼ� Ÿ��
															// 4 Bit : 0 ~ 15.  ��ȭ �ɼ� ����
#ifndef CHARACTERDB_SERVER
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		// ���� �������� ��� ��ȭ�ɼ� ��� ���ʽ� ���� �ɼ��� ����Ѵ�.
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
			if (g_SocketOptionSystem.IsSocketItem(&item[index]) == true)
#else
			if (g_SocketOptionSystem.IsEnableSocketItem(&item[index]) == true)
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
			{
				buf[n] = item[index].m_BonusSocketOption;	// [10] ���ʽ� ���� �ɼ� : ���� �������� ��츸
			}
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#endif
			n++;

#ifndef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			n += 5;											// ���� ����Ʈ�� �ʿ��Ҷ� ���� �ϳ��� ����ϸ� �ȴ�.
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#else
			n += 6;											// ���� ����Ʈ�� �ʿ��Ҷ� ���� �ϳ��� ����ϸ� �ȴ�.
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			// ���� ����Ʈ ���θ� ���
			// ���� ������ �����͸� �Է��Ѵ�.
			buf[n] = item[index].m_SocketOption[0];			// [11] : ���� �ɼ� 1��° ����
			n++;
			buf[n] = item[index].m_SocketOption[1];			// [12] : ���� �ɼ� 2��° ����
			n++;
			buf[n] = item[index].m_SocketOption[2];			// [13] : ���� �ɼ� 3��° ����
			n++;
			buf[n] = item[index].m_SocketOption[3];			// [14] : ���� �ɼ� 4��° ����
			n++;
			buf[n] = item[index].m_SocketOption[4];			// [15] : ���� �ɼ� 5��° ����
			n++;
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		}
	}
}

#endif


//----------------------------------------------------------------------------
// type,�� index�� ������ ������ ���Ѵ�.
// ���������� ���� �ε����� ����� ����...
// �������� �����ϴ����� �˼��ִ�.
int ItemGetNumberMake(int type, int index)
{
	int make;

	make = (type*MAX_ITEM_INDEX) + index;
	if (ItemAttribute[make].Width < 1 || ItemAttribute[make].Height < 1)
	{
		return -1;
	}
	return make;
}

//----------------------------------------------------------------------------
// �ε����� �־��ָ� �������� ���ο� ���� ũ�⸦ �������ִ�.
void ItemGetSize(int index, int & width, int & height)
{
	width = ItemAttribute[index].Width;
	height = ItemAttribute[index].Height;
}


BOOL HasItemDurability(int index)
{
	if (ItemAttribute[index].Durability == 0 && ItemAttribute[index].MagicDurability == 0)
	{
		return FALSE;
	}
	return TRUE;
}

int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem)
{

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214	
	if (index == MAKE_ITEMNUM(14, 21) && itemLevel == 3)
	{
		// ������ ǥ�� (���� +3) �� �������� �ּ� 1 �� -> ���� ����
		itemLevel = 0;
	}
#endif

#ifdef HIDDEN_KALIMA_20050706	// ����� ǥ���� �⺻ �������� 1 �̴�.
	if (index == MAKE_ITEMNUM(14, 29))
	{
		return 1;
	}
#endif

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� �⺻ �������� 1
	if (index == MAKE_ITEMNUM(14, 100))
	{
		return 1;
	}
#endif // UPDATE_LUCKY_COIN_EVENT_20081029

	int dur = 0;

	// ������Ʈ ������ �̸�	
	if (itemLevel < 5)
	{
		dur = ItemAttribute[index].Durability + itemLevel;
	}
	else if (itemLevel >= 5)
	{
		if (itemLevel == 10)
		{
			dur = ItemAttribute[index].Durability + (itemLevel * 2) - 3;
		}
		else if (itemLevel == 11)
		{
			dur = ItemAttribute[index].Durability + (itemLevel * 2) - 1;
		}
#ifdef ITEM_12_13_20040401
		else if (itemLevel == 12)
		{
			dur = ItemAttribute[index].Durability + (itemLevel * 2) - 4 + 1 + 2 + 3;
		}
		else if (itemLevel == 13)
		{
			dur = ItemAttribute[index].Durability + (itemLevel * 2) - 4 + 1 + 2 + 3 + 4;
		}
#endif
		else
		{
			dur = ItemAttribute[index].Durability + (itemLevel * 2) - 4;
		}
	}

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	//���� �η縶���� ������ 1
	if (index == MAKE_ITEMNUM(13, 51))	//���� �η縶���� ������ 1
	{
		dur = 1;
	}
#endif

	if ((index<ITEM_WING + 3 || index>ITEM_WING + 6) &&
		index != MAKE_ITEMNUM(0, 19) &&
		index != MAKE_ITEMNUM(4, 18) &&
		index != MAKE_ITEMNUM(5, 10)
#ifdef ADD_WEAPON_OF_ARCHANGEL_SCEPTER_20050706
		&& index != MAKE_ITEMNUM(2, 13)
#endif
#ifdef MODIFY_UPGRADE_DARKLORDMANTLE_20060515
		&& index != MAKE_ITEMNUM(13, 30)	// ������ ���� : ������Ʈ ó�� ����
#endif		
#ifdef ADD_THIRD_WING_20070525	// 3�������� ������Ʈ ó�� ����
		&& index != MAKE_ITEMNUM(12, 36)
		&& index != MAKE_ITEMNUM(12, 37)
		&& index != MAKE_ITEMNUM(12, 38)
		&& index != MAKE_ITEMNUM(12, 39)
		&& index != MAKE_ITEMNUM(12, 40)
#endif
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// �ű�ĳ���� ����� ����, ������ ����, ������ ������ ������Ʈ ó�� ����
		&& index != MAKE_ITEMNUM(12, 41)
		&& index != MAKE_ITEMNUM(12, 42)
		&& index != MAKE_ITEMNUM(12, 43)
#endif
		) // �߰� ���� �� ���� �������� ������Ʈ ó�� ����
	{

		if (SetItem)
		{
			dur += 20;
		}
		else
			if (ExcellentItem)
			{
				dur += 15;
			}
	}

#ifdef MODIFY_EXCELLENT_DROP_DURATION_BUG_20060524
	// Dur���� BYTE������ 255�� �Ѿ �� ����. OverFlow ����.
	if (dur > 255)
		dur = 255;
#endif // MODIFY_EXCELLENT_DROP_DURATION_BUG_20060524

	return dur;
}

int ItemGetAttackDurability(int index)
{
	return ItemAttribute[index].AttackDur;
}

int ItemGetDefenseDurability(int index)
{
	return ItemAttribute[index].DefenceDur;
}

inline float	GetRepairItemRate(int index)
{
	return ItemAttribute[index].RepaireMoneyRate;
}

inline float	GetAllRepairItemRate(int index)
{
	return ItemAttribute[index].AllRepaireMoneyRate;
}

void CalRepairRate(int type, int index, ITEM_ATTRIBUTE *p)
{
	// �ʱ� �����ÿ��� �����ۺ��� �ٸ� ������� ������ ������ �־����� �ϰ������� ����ǰ� ���ߴ�
	// ���߿� �� ��� ���� �𸣱� ������ �ּ��� �޾� �״�
	p->RepaireMoneyRate = 0.1f;
#ifdef MODIFY_CALREPAIRRATE_20090528
	p->AllRepaireMoneyRate = 1.4f;
#else
	p->AllRepaireMoneyRate = 0.4f;
#endif
	/*
	if(type>=0 && type<=6)
	{
		if( type == 4 )
		{	// ����
			p->RepaireMoneyRate = 0.8f;
			p->AllRepaireMoneyRate = 0.4f;
		}
		else if( type == 5 )
		{	// �渶����
			p->RepaireMoneyRate = 0.8f;
			p->AllRepaireMoneyRate = 0.5f;
		}
		else
		{	// ������
			p->RepaireMoneyRate = 0.7f;
			p->AllRepaireMoneyRate = 0.3f;
		}
		return;
	}

	if(type>=7 && type<=11)
	{
		switch(index)
		{
			case 0:
			case 1:
			case 5:
			case 6:
			case 8:
			case 9:
			case 16:
				p->RepaireMoneyRate = 0.7f;
				p->AllRepaireMoneyRate = 0.4f;
			break;

			case 2:
			case 3:
			case 4:
			case 7:
				p->RepaireMoneyRate = 0.7f;
				p->AllRepaireMoneyRate = 0.6f;
				break;
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
				p->RepaireMoneyRate = 0.8f;
				p->AllRepaireMoneyRate = 0.4f;
				break;
			case 15:
				p->RepaireMoneyRate = 0.9f;
				p->AllRepaireMoneyRate = 0.4f;
				break;
		}
		return;
	}
	*/
}

BOOL CItem::CheckExcOption(int iMinExcOption)
{
	int iCount = 0;

	for (int i = 0; i < 6; i++)
	{
		if (this->m_NewOption & (1 << i))
			iCount++;
	}

	return (iCount >= iMinExcOption);
}

//----------------------------------------------------------------------------
// ������ ��ũ��Ʈ �����͸� �д´�.
//----------------------------------------------------------------------------
BOOL OpenItemScript(char *FileName)
{
	int loop_count;
	if ((SMDFile = fopen(FileName, "r")) == NULL)
		return FALSE;

	memset(&ItemAttribute, 0, sizeof(ItemAttribute));
	for (int n = 0; n < MAX_ITEM; n++)
	{
		ItemAttribute[n].Level = 0xFF;
		ItemAttribute[n].RepaireMoneyRate = 0;
		ItemAttribute[n].AllRepaireMoneyRate = 0;
	}

	SMDToken Token;
	while (true)
	{
		Token = (*GetToken)();
		if (Token == END) break;
		if (Token == NUMBER)
		{
			int Type = (int)TokenNumber;
			loop_count = 0;
			while (true)
			{
				Token = (*GetToken)();
				int Index = (int)TokenNumber;

#ifdef ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808
				if (Type >= 0 && Type < MAX_ITEM_TYPE)
				{
					// Ÿ���� �ִ� �ε����� ����Ѵ�.
					if (g_MaxItemIndexOfEachItemType[Type] <= Index)
						g_MaxItemIndexOfEachItemType[Type] = Index;
				}
#endif // ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808

				if (Token == NAME && strcmp("end", TokenString) == NULL) break;
				ITEM_ATTRIBUTE *p = &ItemAttribute[Type*MAX_ITEM_INDEX + Index];

				Token = (*GetToken)(); p->ItemSlot = (int)TokenNumber;
				Token = (*GetToken)(); p->SkillType = (int)TokenNumber;

				Token = (*GetToken)(); p->Width = (int)TokenNumber;
				Token = (*GetToken)(); p->Height = (int)TokenNumber;
				Token = (*GetToken)(); p->Serial = (int)TokenNumber;
				Token = (*GetToken)(); p->OptionFlag = (int)TokenNumber;
				Token = (*GetToken)(); p->MondownFlag = (int)TokenNumber;
				Token = (*GetToken)(); strcpy(p->Name, TokenString);

#if TESTSERVER == 1
				char sztext[255];
				wsprintf(sztext, "%d\t%s\n", MAKE_ITEMNUM(Type, Index), p->Name);
				OutputDebugString(sztext);
#endif

				// �� ����, �����̷� 
				if (Type >= 0 && Type <= 5)
				{

					// 1. > �⺻ ����
					Token = (*GetToken)(); p->Level = (int)TokenNumber;
					Token = (*GetToken)(); p->DamageMin = (int)TokenNumber;
					Token = (*GetToken)(); p->DamageMax = (int)TokenNumber;
					Token = (*GetToken)(); p->AttackSpeed = (int)TokenNumber;
					Token = (*GetToken)(); p->Durability = (int)TokenNumber;


					// 2. > ���� ���� 
					Token = (*GetToken)(); p->MagicDurability = (int)TokenNumber;
					Token = (*GetToken)(); p->MagicPW = (int)TokenNumber;

					// 3. > �䱸 ����, ���� ����
#ifdef EXPAND_ITEMSCRIPT_01_20050808 
					Token = (*GetToken)(); p->RequireLevel = (int)TokenNumber; // �䱸 ����
#else
					Token = (*GetToken)();
#endif
					Token = (*GetToken)(); p->RequireStrength = (int)TokenNumber;	// �䱸 ��
					Token = (*GetToken)(); p->RequireDexterity = (int)TokenNumber;	// �䱸 ��ø
					Token = (*GetToken)(); p->RequireEnergy = (int)TokenNumber;	// �䱸 ������

#ifdef EXPAND_ITEMSCRIPT_01_20050808 
					Token = (*GetToken)(); p->RequireVitality = (int)TokenNumber;	// �䱸 ����
					Token = (*GetToken)(); p->RequireLeadership = (int)TokenNumber; // �䱸 ���
#else
					Token = (*GetToken)();
					Token = (*GetToken)();
#endif

					// 4. > �ΰ� ���� ����
					if (p->Width >= 2) p->TwoHand = true;

					// ���� �������� ���� �������� ���� ���ȴ�
					p->Durability += p->MagicDurability;

				}

				// �� ����, ����
				if (Type >= 6 && Type <= 11)
				{
					// 1. > �⺻ ����
					Token = (*GetToken)(); p->Level = (int)TokenNumber;

					if (Type == 6)
					{
						Token = (*GetToken)(); p->Defense = (int)TokenNumber;			// ���
						Token = (*GetToken)(); p->SuccessfulBlocking = (int)TokenNumber;		// ��� ������			
					}

					else if (Type >= 7 && Type <= 9)
					{
						Token = (*GetToken)(); p->Defense = (int)TokenNumber;
						Token = (*GetToken)(); p->MagicDefense = (int)TokenNumber;
					}
					else if (Type == 10)
					{
						Token = (*GetToken)(); p->Defense = (int)TokenNumber;
						Token = (*GetToken)(); p->AttackSpeed = (int)TokenNumber;
					}
					else if (Type == 11)
					{
						Token = (*GetToken)(); p->Defense = (int)TokenNumber;
						Token = (*GetToken)(); p->WalkSpeed = (int)TokenNumber;
					}
					Token = (*GetToken)(); p->Durability = (int)TokenNumber;


					// 2. > �䱸 ����, ���� ����

#ifdef EXPAND_ITEMSCRIPT_01_20050808 
					Token = (*GetToken)(); p->RequireLevel = (int)TokenNumber; // �䱸 ����
#else
					Token = (*GetToken)();
#endif

					Token = (*GetToken)(); p->RequireStrength = (int)TokenNumber;	// �䱸 ��
					Token = (*GetToken)(); p->RequireDexterity = (int)TokenNumber; // �䱸 ��ø

#ifdef EXPAND_ITEMSCRIPT_01_20050808
					Token = (*GetToken)(); p->RequireEnergy = (int)TokenNumber;	// �䱸 ������
					Token = (*GetToken)(); p->RequireVitality = (int)TokenNumber;	// �䱸 ����
					Token = (*GetToken)(); p->RequireLeadership = (int)TokenNumber; // �䱸 ���
#else
					if (Type == 6)
					{
						Token = (*GetToken)(); p->RequireEnergy = (int)TokenNumber;
						Token = (*GetToken)();
						Token = (*GetToken)();
					}
					else
					{
						Token = (*GetToken)();
						Token = (*GetToken)();
						Token = (*GetToken)();
					}
#endif

				}

				// �� ���۷�, �����, ����, �Է�, ����, �ֹ��� ��
				if (Type == 13)
				{
					Token = (*GetToken)(); p->Level = (int)TokenNumber;
					Token = (*GetToken)(); p->Durability = (int)TokenNumber;
					Token = (*GetToken)(); p->Resistance[0] = (int)TokenNumber;
					Token = (*GetToken)(); p->Resistance[1] = (int)TokenNumber;
					Token = (*GetToken)(); p->Resistance[2] = (int)TokenNumber;
					Token = (*GetToken)(); p->Resistance[3] = (int)TokenNumber;

					Token = (*GetToken)(); p->Resistance[4] = (int)TokenNumber;
					Token = (*GetToken)(); p->Resistance[5] = (int)TokenNumber;
					Token = (*GetToken)(); p->Resistance[6] = (int)TokenNumber;

					p->ResistanceType = 0xFF;
					for (int ResiIndex = 0; ResiIndex < 6; ResiIndex++)
					{
						if (p->Resistance[ResiIndex])
							p->ResistanceType = ResiIndex;
					}

					// �����, ������ ��� �䱸������ �����۷��� ��ü�� �Ҵ� �س���..�̷�..
					p->RequireLevel = p->Level;
				}

				// �� ����, ����, ¡ǥ, ��, ����Ʈ ������ ��
				if (Type == 14)
				{
					Token = (*GetToken)(); p->Value = (int)TokenNumber;
					Token = (*GetToken)(); p->Level = (int)TokenNumber;
					//if( Index == 17 || Index == 18 )
					//{	// �����ۿ� �������� ������ �־��ش�(TESTSERVER1���� ���� �������� ������ ���ֱ� ���ؼ�)
						// �Ǹ��� ��, ����
					p->Durability = 1;
					//}
				}

				// �� ����, ȥ���Ǻ���, ��ũ��, ��/���� ����
				if (Type == 12)
				{
					Token = (*GetToken)(); p->Level = (int)TokenNumber;
					Token = (*GetToken)(); p->Defense = (int)TokenNumber;
					Token = (*GetToken)(); p->Durability = (int)TokenNumber;
					Token = (*GetToken)(); p->RequireLevel = (int)TokenNumber;
					Token = (*GetToken)(); p->RequireEnergy = (int)TokenNumber;

					Token = (*GetToken)(); p->RequireStrength = (int)TokenNumber;
#ifdef NEW_FORSKYLAND2	// ���� �䱸 ��ø �߰�(20030731)					
					Token = (*GetToken)(); p->RequireDexterity = (int)TokenNumber;
#else
					Token = (*GetToken)();
#endif

#ifdef DARKLORD_WORK // ��ũ ���Ǹ����� ���� �߰� 					
					Token = (*GetToken)(); p->RequireLeadership = (int)TokenNumber;
#else
					Token = (*GetToken)();
#endif

					Token = (*GetToken)(); p->BuyMoney = (int)TokenNumber;
				}

				// �� ����
				if (Type == 15)
				{
					Token = (*GetToken)(); p->Level = (int)TokenNumber;

#ifdef NEW_FORSKYLAND2	// ���� �䱸 ���� �߰�(20030725)(foguy)
					Token = (*GetToken)(); p->RequireLevel = (int)TokenNumber;
#else
					Token = (*GetToken)();
#endif

					Token = (*GetToken)(); p->RequireEnergy = (int)TokenNumber;

					Token = (*GetToken)(); p->BuyMoney = (int)TokenNumber;
					p->DamageMin = p->Level;
					p->DamageMax = p->Level + p->Level / 2;

				}


				// ��Ʈ �Ӽ� �߰�
				if (Type <= 11 || Type == 13)
					Token = (*GetToken)(); p->SetAttr = (int)TokenNumber;

				//��� Ŭ����
				if (Type <= 13 || Type == 15)
				{
					Token = (*GetToken)(); p->RequireClass[0] = (int)TokenNumber;
					Token = (*GetToken)(); p->RequireClass[1] = (int)TokenNumber;
					Token = (*GetToken)(); p->RequireClass[2] = (int)TokenNumber;
					Token = (*GetToken)(); p->RequireClass[3] = (int)TokenNumber;

#ifdef DARKLORD_WORK	
					// ��ũ�ε�
					Token = (*GetToken)(); p->RequireClass[4] = (int)TokenNumber;
#else
					Token = (*GetToken)();
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// item ��� Ŭ���� �߰�
					Token = (*GetToken)(); p->RequireClass[5] = (int)TokenNumber;
#else
					Token = (*GetToken)();
#endif

					Token = (*GetToken)(); //Rage Fighter Bypass
				}

				// ������ ������� ������ �����Ѵ�
				CalRepairRate(Type, Index, p);
				p->HaveItemInfo = TRUE;

				loop_count++;
				if (loop_count > MAX_ITEM_INDEX)
				{
					MsgBox("Error : Item Data fail. (LoopCount:%d) Item Type:%d Index %d", loop_count, Type, Index);
					break;
				}
			}
			if (loop_count > MAX_ITEM_INDEX)
			{
				break;
			}
		}
	}
	fclose(SMDFile);
	return TRUE;
}


//----------------------------------------------------------------------------
// �ؿ����� ���ؼ� ������ �ε��� ������ ������ ������ �̸��� �ٲ۴�
//----------------------------------------------------------------------------
BOOL OpenItemNameScript(char *FileName)
{
	if ((SMDFile = fopen(FileName, "r")) == NULL)
		return FALSE;

	SMDToken Token;
	while (true)
	{
		Token = (*GetToken)();
		if (Token == END) break;
		if (Token == NUMBER)
		{
			int Type = (int)TokenNumber;
			while (true)
			{
				Token = (*GetToken)();
				int Index = (int)TokenNumber;
				if (Token == NAME && strcmp("end", TokenString) == NULL) break;
				ITEM_ATTRIBUTE *p = &ItemAttribute[Type*MAX_ITEM_INDEX + Index];
				Token = (*GetToken)(); strcpy(p->Name, TokenString);
			}
		}
	}
	fclose(SMDFile);
	return TRUE;
}

#ifdef AUTH_GAMESERVER
BOOL OpenItemScript(char* Buffer, int iSize)
{
	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);


	memset(&ItemAttribute, 0, sizeof(ItemAttribute));
	for (int n = 0; n < MAX_ITEM; n++)
	{
		ItemAttribute[n].Level = 0xFF;
		ItemAttribute[n].RepaireMoneyRate = 0;
		ItemAttribute[n].AllRepaireMoneyRate = 0;
	}

	int loop_count;

	SMDToken Token;
	while (true)
	{
		Token = WzMemScript.GetToken();
		if (Token == END) break;
		if (Token == NUMBER)
		{
			int Type = WzMemScript.GetNumber();
			loop_count = 0;
			while (true)
			{
				Token = WzMemScript.GetToken();
				int Index = WzMemScript.GetNumber();

#ifdef ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808
				if (Type >= 0 && Type < MAX_ITEM_TYPE)
				{
					// Ÿ���� �ִ� �ε����� ����Ѵ�.
					if (g_MaxItemIndexOfEachItemType[Type] <= Index)
						g_MaxItemIndexOfEachItemType[Type] = Index;
				}
#endif // ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808

				if (Token == NAME && strcmp("end", WzMemScript.GetString()) == NULL) break;
				ITEM_ATTRIBUTE *p = &ItemAttribute[Type*MAX_ITEM_INDEX + Index];

#ifdef DARKLORD_WORK	// ������ ���� ��ġ				
				Token = WzMemScript.GetToken(); p->ItemSlot = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); p->SkillType = WzMemScript.GetNumber();
#endif
				Token = WzMemScript.GetToken(); p->Width = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); p->Height = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); p->Serial = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); p->OptionFlag = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); p->MondownFlag = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); strcpy(p->Name, WzMemScript.GetString());

#if TESTSERVER == 1
				char sztext[255];
				wsprintf(sztext, "%d\t%s\n", MAKE_ITEMNUM(Type, Index), p->Name);
				OutputDebugString(sztext);
#endif

				if (Type >= 0 && Type <= 5)
				{
					Token = WzMemScript.GetToken(); p->Level = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->DamageMin = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->DamageMax = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->AttackSpeed = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Durability = WzMemScript.GetNumber();

					// �߰�(20030305)(foguy)
					Token = WzMemScript.GetToken(); p->MagicDurability = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->MagicPW = WzMemScript.GetNumber();

#ifdef EXPAND_ITEMSCRIPT_01_20050808 
					Token = WzMemScript.GetToken(); p->RequireLevel = WzMemScript.GetNumber();	// �䱸 ����
#endif

					Token = WzMemScript.GetToken(); p->RequireStrength = WzMemScript.GetNumber();	// �䱸 ��
					Token = WzMemScript.GetToken(); p->RequireDexterity = WzMemScript.GetNumber();	// �䱸 ��ø
					Token = WzMemScript.GetToken(); p->RequireEnergy = WzMemScript.GetNumber();		// �䱸 ������

#ifdef EXPAND_ITEMSCRIPT_01_20050808 
					Token = WzMemScript.GetToken(); p->RequireVitality = WzMemScript.GetNumber();	// �䱸 ����
					Token = WzMemScript.GetToken(); p->RequireLeadership = WzMemScript.GetNumber();	// �䱸 ���
#endif

					if (p->Width >= 2) p->TwoHand = true;

					// ���� �������� ���� �������� ���� ���ȴ�
					p->Durability += p->MagicDurability;


				}
				if (Type >= 6 && Type <= 11)
				{
					Token = WzMemScript.GetToken(); p->Level = WzMemScript.GetNumber();

					if (Type == 6)
					{
						Token = WzMemScript.GetToken(); p->Defense = WzMemScript.GetNumber();
						Token = WzMemScript.GetToken(); p->SuccessfulBlocking = WzMemScript.GetNumber();
					}

					else if (Type >= 7 && Type <= 9)
					{
						Token = WzMemScript.GetToken(); p->Defense = WzMemScript.GetNumber();
						Token = WzMemScript.GetToken(); p->MagicDefense = WzMemScript.GetNumber();
					}
					else if (Type == 10)
					{
						Token = WzMemScript.GetToken(); p->Defense = WzMemScript.GetNumber();
						Token = WzMemScript.GetToken(); p->AttackSpeed = WzMemScript.GetNumber();
					}
					else if (Type == 11)
					{
						Token = WzMemScript.GetToken(); p->Defense = WzMemScript.GetNumber();
						Token = WzMemScript.GetToken(); p->WalkSpeed = WzMemScript.GetNumber();
					}
					Token = WzMemScript.GetToken(); p->Durability = WzMemScript.GetNumber();

#ifdef EXPAND_ITEMSCRIPT_01_20050808 
					Token = WzMemScript.GetToken(); p->RequireLevel = WzMemScript.GetNumber();	// �䱸 ����
#endif
					Token = WzMemScript.GetToken(); p->RequireStrength = WzMemScript.GetNumber();	// �䱸 ��
					Token = WzMemScript.GetToken(); p->RequireDexterity = WzMemScript.GetNumber();	// �䱸 ��ø

#ifdef EXPAND_ITEMSCRIPT_01_20050808
					Token = WzMemScript.GetToken(); p->RequireEnergy = WzMemScript.GetNumber();	// �䱸 ������
					Token = WzMemScript.GetToken(); p->RequireVitality = WzMemScript.GetNumber();	// �䱸 ����
					Token = WzMemScript.GetToken(); p->RequireLeadership = WzMemScript.GetNumber();  // �䱸 ���
#else
					if (Type == 6)
					{
						Token = WzMemScript.GetToken(); p->RequireEnergy = WzMemScript.GetNumber();
					}
#endif
				}
				if (Type == 13)
				{
					Token = WzMemScript.GetToken(); p->Level = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Durability = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Resistance[0] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Resistance[1] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Resistance[2] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Resistance[3] = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken(); p->Resistance[4] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Resistance[5] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Resistance[6] = WzMemScript.GetNumber();

					p->ResistanceType = 0xFF;
					for (int ResiIndex = 0; ResiIndex < 6; ResiIndex++)
					{
						if (p->Resistance[ResiIndex])
							p->ResistanceType = ResiIndex;
					}

					p->RequireLevel = p->Level;
				}
				if (Type == 14)
				{
					Token = WzMemScript.GetToken(); p->Value = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Level = WzMemScript.GetNumber();
					//if( Index == 17 || Index == 18 )
					//{	// �����ۿ� �������� ������ �־��ش�(TESTSERVER1���� ���� �������� ������ ���ֱ� ���ؼ�)
						// �Ǹ��� ��, ����
					p->Durability = 1;
					//}
				}
				if (Type == 12)
				{
					Token = WzMemScript.GetToken(); p->Level = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Defense = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->Durability = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->RequireLevel = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->RequireEnergy = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken(); p->RequireStrength = WzMemScript.GetNumber();
#ifdef NEW_FORSKYLAND2	// ���� �䱸 ��ø �߰�(20030731)					
					Token = WzMemScript.GetToken(); p->RequireDexterity = WzMemScript.GetNumber();
#endif

#ifdef DARKLORD_WORK					
					Token = WzMemScript.GetToken(); p->RequireLeadership = WzMemScript.GetNumber();
#endif

					Token = WzMemScript.GetToken(); p->BuyMoney = WzMemScript.GetNumber();
				}
				if (Type == 15)
				{
					Token = WzMemScript.GetToken(); p->Level = WzMemScript.GetNumber();

#ifdef NEW_FORSKYLAND2	// ���� �䱸 ���� �߰�(20030725)(foguy)
					Token = WzMemScript.GetToken(); p->RequireLevel = WzMemScript.GetNumber();
#endif
					Token = WzMemScript.GetToken(); p->RequireEnergy = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken(); p->BuyMoney = WzMemScript.GetNumber();
					p->DamageMin = p->Level;
					p->DamageMax = p->Level + p->Level / 2;

				}

				if (Type <= 11 || Type == 13)
					Token = WzMemScript.GetToken(); p->SetAttr = WzMemScript.GetNumber();

				//��� Ŭ����
				if (Type <= 13 || Type == 15)
				{
					Token = WzMemScript.GetToken(); p->RequireClass[0] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->RequireClass[1] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->RequireClass[2] = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); p->RequireClass[3] = WzMemScript.GetNumber();
#ifdef DARKLORD_WORK	// ��ũ�ε�
					Token = WzMemScript.GetToken(); p->RequireClass[4] = WzMemScript.GetNumber();
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// item ��� Ŭ���� �߰�
					Token = WzMemScript.GetToken(); p->RequireClass[5] = WzMemScript.GetNumber();
#endif
				}

				// ������ ������� ������ �����Ѵ�
				CalRepairRate(Type, Index, p);
				p->HaveItemInfo = TRUE;

				loop_count++;
				if (loop_count > MAX_ITEM_INDEX)
				{
					MsgBox("Error : Item Data fail. (LoopCount:%d) Item Type:%d Index %d", loop_count, Type, Index);
					break;
				}
			}
		}
	}
	return TRUE;
}

//----------------------------------------------------------------------------
// �ؿ����� ���ؼ� ������ �ε��� ������ ������ ������ �̸��� �ٲ۴�
//----------------------------------------------------------------------------
BOOL OpenItemNameScript(char *Buffer, int iSize)
{
	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);

	SMDToken Token;
	while (true)
	{
		Token = WzMemScript.GetToken();
		if (Token == END) break;
		if (Token == NUMBER)
		{
			int Type = WzMemScript.GetNumber();
			while (true)
			{
				Token = WzMemScript.GetToken();
				int Index = WzMemScript.GetNumber();
				if (Token == NAME && strcmp("end", WzMemScript.GetString()) == NULL) break;
				ITEM_ATTRIBUTE *p = &ItemAttribute[Type*MAX_ITEM_INDEX + Index];
				Token = WzMemScript.GetToken(); strcpy(p->Name, WzMemScript.GetString());
			}
		}
	}
	return TRUE;
}
#endif



#ifdef SCRIPT_DECODE_WORK
//----------------------------------------------------------------------------
// ������ ��ũ��Ʈ �����͸� �д´�.
//----------------------------------------------------------------------------
BOOL OpenItemScriptDecode(char *FileName)
{
	CWZScriptEncode		Encoder;
	if (!Encoder.Open(FileName))
	{
		return FALSE;
	}

	WZSMDToken Token;

	memset(&ItemAttribute, 0, sizeof(ItemAttribute));
	for (int n = 0; n < MAX_ITEM; n++)
	{
		ItemAttribute[n].Level = 0xFF;
		ItemAttribute[n].RepaireMoneyRate = 0;
		ItemAttribute[n].AllRepaireMoneyRate = 0;
	}


	while (true)
	{
		Token = Encoder.GetToken();
		if (Token == END) break;
		if (Token == T_NUMBER)
		{
			int Type = Encoder.GetNumber();
			while (true)
			{
				Token = Encoder.GetToken();
				int Index = Encoder.GetNumber();

#ifdef ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808
				if (Type >= 0 && Type < MAX_ITEM_TYPE)
				{
					// Ÿ���� �ִ� �ε����� ����Ѵ�.
					if (g_MaxItemIndexOfEachItemType[Type] <= Index)
						g_MaxItemIndexOfEachItemType[Type] = Index;
				}
#endif // ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808

				if (Token == T_NAME && strcmp("end", Encoder.GetString()) == NULL) break;
				ITEM_ATTRIBUTE *p = &ItemAttribute[Type*MAX_ITEM_INDEX + Index];

				Token = Encoder.GetToken(); p->Width = Encoder.GetNumber();
				Token = Encoder.GetToken(); p->Height = Encoder.GetNumber();
				Token = Encoder.GetToken(); p->Serial = Encoder.GetNumber();
				Token = Encoder.GetToken(); p->OptionFlag = Encoder.GetNumber();
				Token = Encoder.GetToken(); p->MondownFlag = Encoder.GetNumber();
				Token = Encoder.GetToken(); strcpy(p->Name, Encoder.GetString());

				if (Type >= 0 && Type <= 5)
				{
					Token = Encoder.GetToken(); p->Level = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->DamageMin = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->DamageMax = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->AttackSpeed = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->Durability = Encoder.GetNumber();

					// �߰�(20030305)(foguy)
					Token = Encoder.GetToken(); p->MagicDurability = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->MagicPW = Encoder.GetNumber();

#ifdef EXPAND_ITEMSCRIPT_01_20050808 
					Token = Encoder.GetToken(); p->RequireLevel = Encoder.GetNumber();	// �䱸 ����
#endif

					Token = Encoder.GetToken(); p->RequireStrength = Encoder.GetNumber();	// �䱸 ��
					Token = Encoder.GetToken(); p->RequireDexterity = Encoder.GetNumber();	// �䱸 ��ø
					Token = Encoder.GetToken(); p->RequireEnergy = Encoder.GetNumber();		// �䱸 ������

#ifdef EXPAND_ITEMSCRIPT_01_20050808 
					Token = Encoder.GetToken(); p->RequireVitality = Encoder.GetNumber();	// �䱸 ����
					Token = Encoder.GetToken(); p->RequireLeadership = Encoder.GetNumber();	// �䱸 ���
#endif

					if (p->Width >= 2) p->TwoHand = true;

					// ���� �������� ���� �������� ���� ���ȴ�
					p->Durability += p->MagicDurability;


				}
				if (Type >= 6 && Type <= 11)
				{
					Token = Encoder.GetToken(); p->Level = Encoder.GetNumber();

					if (Type == 6)
					{
						Token = Encoder.GetToken(); p->Defense = Encoder.GetNumber();
						Token = Encoder.GetToken(); p->SuccessfulBlocking = Encoder.GetNumber();
					}

					else if (Type >= 7 && Type <= 9)
					{
						Token = Encoder.GetToken(); p->Defense = Encoder.GetNumber();
						Token = Encoder.GetToken(); p->MagicDefense = Encoder.GetNumber();
					}
					else if (Type == 10)
					{
						Token = Encoder.GetToken(); p->Defense = Encoder.GetNumber();
						Token = Encoder.GetToken(); p->AttackSpeed = Encoder.GetNumber();
					}
					else if (Type == 11)
					{
						Token = Encoder.GetToken(); p->Defense = Encoder.GetNumber();
						Token = Encoder.GetToken(); p->WalkSpeed = Encoder.GetNumber();
					}
					Token = Encoder.GetToken(); p->Durability = Encoder.GetNumber();

#ifdef EXPAND_ITEMSCRIPT_01_20050808 
					Token = Encoder.GetToken(); p->RequireLevel = Encoder.GetNumber();	// �䱸 ����
#endif

					Token = Encoder.GetToken(); p->RequireStrength = Encoder.GetNumber();	// �䱸 ��
					Token = Encoder.GetToken(); p->RequireDexterity = Encoder.GetNumber();	// �䱸 ��ø

#ifdef EXPAND_ITEMSCRIPT_01_20050808
					Token = Encoder.GetToken(); p->RequireEnergy = Encoder.GetNumber();	// �䱸 ������
					Token = Encoder.GetToken(); p->RequireVitality = Encoder.GetNumber();	// �䱸 ����
					Token = Encoder.GetToken(); p->RequireLeadership = Encoder.GetNumber();	// �䱸 ���
#else
					if (Type == 6)
					{
						Token = Encoder.GetToken(); p->RequireEnergy = Encoder.GetNumber();
					}
#endif
				}
				if (Type == 13)
				{
					Token = Encoder.GetToken(); p->Level = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->Durability = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->Resistance[0] = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->Resistance[1] = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->Resistance[2] = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->Resistance[3] = Encoder.GetNumber();
					p->RequireLevel = p->Level;
				}
				if (Type == 14)
				{
					Token = Encoder.GetToken(); p->Value = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->Level = Encoder.GetNumber();
					//if( Index == 17 || Index == 18 )
					//{	// �����ۿ� �������� ������ �־��ش�(TESTSERVER1���� ���� �������� ������ ���ֱ� ���ؼ�)
						// �Ǹ��� ��, ����
					p->Durability = 1;
					//}
				}
				if (Type == 12)
				{
					Token = Encoder.GetToken(); p->Level = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->Defense = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->Durability = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->RequireLevel = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->RequireEnergy = Encoder.GetNumber();

					Token = Encoder.GetToken(); p->RequireStrength = Encoder.GetNumber();
#ifdef NEW_FORSKYLAND2	// ���� �䱸 ��ø �߰�(20030731)					
					Token = Encoder.GetToken(); p->RequireDexterity = Encoder.GetNumber();
#endif
					Token = Encoder.GetToken(); p->BuyMoney = Encoder.GetNumber();
				}
				if (Type == 15)
				{
					Token = Encoder.GetToken(); p->Level = Encoder.GetNumber();

#ifdef NEW_FORSKYLAND2	// ���� �䱸 ���� �߰�(20030725)(foguy)
					Token = Encoder.GetToken(); p->RequireLevel = Encoder.GetNumber();
#endif
					Token = Encoder.GetToken(); p->RequireEnergy = Encoder.GetNumber();

					Token = Encoder.GetToken(); p->BuyMoney = Encoder.GetNumber();
					p->DamageMin = p->Level;
					p->DamageMax = p->Level + p->Level / 2;

				}
				//��� Ŭ����
				if (Type <= 13 || Type == 15)
				{
					Token = Encoder.GetToken(); p->RequireClass[0] = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->RequireClass[1] = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->RequireClass[2] = Encoder.GetNumber();
					Token = Encoder.GetToken(); p->RequireClass[3] = Encoder.GetNumber();
				}

				// ������ ������� ������ �����Ѵ�
				CalRepairRate(Type, Index, p);
				p->HaveItemInfo = TRUE;
			}
		}
	}
	return TRUE;
}


//----------------------------------------------------------------------------
// �ؿ����� ���ؼ� ������ �ε��� ������ ������ ������ �̸��� �ٲ۴�
//----------------------------------------------------------------------------
BOOL OpenItemNameScriptDecode(char *FileName)
{
	CWZScriptEncode		Encoder;
	if (!Encoder.Open(FileName))
		return FALSE;

	WZSMDToken Token;
	while (true)
	{
		Token = Encoder.GetToken();
		if (Token == END) break;
		if (Token == T_NUMBER)
		{
			int Type = Encoder.GetNumber();
			while (true)
			{
				Token = Encoder.GetToken();
				int Index = Encoder.GetNumber();
				if (Token == T_NAME && strcmp("end", Encoder.GetString()) == NULL) break;
				ITEM_ATTRIBUTE *p = &ItemAttribute[Type*MAX_ITEM_INDEX + Index];
				Token = Encoder.GetToken();	strcpy(p->Name, Encoder.GetString());
			}
		}
	}
	return TRUE;
}
#endif

// �������� ������ ��´�.
BOOL zzzItemLevel(int type, int index, int level)
{
	int item_num;
	item_num = (type*MAX_ITEM_INDEX) + index;

	if (level < 0) level = 0;

	if (ItemAttribute[item_num].Level == 0xFF) return FALSE;	// �������������� �ʴٸ�.
	if (ItemAttribute[item_num].Level == 0) return FALSE;
	if (ItemAttribute[item_num].Level < level) return TRUE;
	return FALSE;
}

// �� �ش� �����ۿ� ���� ���� ���� �� +������ ���Ѵ�.
int GetLevelItem(int type, int index, int level)
{
	int item_num;
	int itemlevel;

	if (level < 0) level = 0;
	//	level = 65;
		//item_num = (type*MAX_ITEM_INDEX)+index;
	item_num = MAKE_ITEMNUM(type, index);

	if (ItemAttribute[item_num].MondownFlag == 0)
		return -1;

	if (ItemAttribute[item_num].Level == 0xFF) return -1;	// �������������� �ʴٸ�.
	if (ItemAttribute[item_num].Level == 0) return -1;	// �������������� �ʴٸ�.

	if (type == 14)	// ����� ����ø���� 
	{
		itemlevel = ItemAttribute[item_num].Level;

		if (index == 15) return -1;	// ���̸� ����
		if (itemlevel >= (level - 8) && itemlevel <= level)
		{
			return 0;
		}
		return -1;
	}
	// ���� �����϶���..
	if (type == 13 && index == 10)
	{
		if ((rand() % 10) == 0)
		{
			int callbead_level = 0;
			if (level < 0) level = 0;
			callbead_level = level / 10;
			if (callbead_level > 0) callbead_level -= 1;
			if (callbead_level > 5) callbead_level = 5;
			return callbead_level;
		}
		return -1;
	}
	if (type == 12 && index == 11)
	{
		if ((rand() % 10) == 0)
		{
			int callbead_level = 0;
			if (level < 0) level = 0;
			callbead_level = level / 10;
			if (callbead_level > 0) callbead_level -= 1;
			if (callbead_level > 6) callbead_level = 6;
			return callbead_level;
		}
		// �̹� ������ ��ȯ ���� �ȳ�����..
		return -1;
	}

	itemlevel = ItemAttribute[item_num].Level;

	// ������ ��� ������ �ִ� ������ ������ MAX_MONSTER_ITEM_DROP_RANGE ���� ������ ��� �Ѵ�.
	// MAX_MONSTER_ITEM_DROP_RANGE : ���� ���� ������ ���� ������ ����� �ּ� ���� �������� ����
#ifdef MODIFY_DROP_MAX_ITEM_LEVEL_6_OF_MONSTER_20050624
#define MAX_MONSTER_ITEM_DROP_RANGE   18  
	if (((itemlevel) >= (level - MAX_MONSTER_ITEM_DROP_RANGE)) && (itemlevel <= level))
	{
#else
#define MAX_MONSTER_ITEM_DROP_RANGE   15
	if (((itemlevel) >= (level - MAX_MONSTER_ITEM_DROP_RANGE)) && (itemlevel <= level))
	{
#endif
		if (type == 15) return 0;	// �������̸� + ������ ����..

#ifndef MODIFY_DROP_PREVENT_OF_RING_N_NECKLACE_LV_5_OVER_20050623
		return (level - itemlevel) / 3;
#endif

#ifdef MODIFY_DROP_PREVENT_OF_RING_N_NECKLACE_LV_5_OVER_20050623
		itemlevel = (level - itemlevel) / 3;

		// ��� ������ ����̴� ���� 4���ϸ� ����
		if (type == 13) {
			if (index == 8 || // �����ǹ���
				index == 9 || // ���ǹ���
				index == 12 || // �����Ǹ����
				index == 13 || // ���Ǹ����
				index == 20 || // �������ǹ���
				index == 21 || // ���ǹ���
				index == 22 || // ���ǹ���
				index == 23 || // �ٶ��ǹ���
				index == 24 || // �����ǹ���
				index == 25 || // �����Ǹ����
				index == 26 || // �ٶ��Ǹ���� 
				index == 27 || // ���Ǹ����
				index == 28    // ����Ǹ����
				) {
				if (itemlevel > 4) {
					itemlevel = 4;
				}
			}
		}

		return itemlevel;
#endif

	}
	return -1;
	}

int GetSerialItem(int type)
{
	int item_num;

	item_num = type;

	if (ItemAttribute[item_num].Level == 0xFF) return -1;	// �������������� �ʴٸ�.

	if (ItemAttribute[item_num].Serial == 0) return FALSE;
	return TRUE;
}

BOOL IsItem(int item_num)
{
	if (item_num < 0 || item_num >= MAX_ITEM)
		return FALSE;

	return ItemAttribute[item_num].HaveItemInfo;
}


ITEM_ATTRIBUTE *GetItemAttr(int item_num)
{
	if (item_num < 0 || item_num >= MAX_ITEM)
		return FALSE;

	return &ItemAttribute[item_num];
}

