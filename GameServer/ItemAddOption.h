// ItemAddOption.h: interface for the CItemAddOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMADDOPTION_H__35EBD2EC_40D5_4FA9_A9CF_4FD2DECC5B90__INCLUDED_)
#define AFX_ITEMADDOPTION_H__35EBD2EC_40D5_4FA9_A9CF_4FD2DECC5B90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ITEM_ADD_OPTION_20061019

#define MAX_ITEM_ADD_OPTION_ROW		100

enum
{
	ITEMADDOPTION_NONE				= 0,
	ITEMADDOPTION_ATTACKSPEED		= 1,
	ITEMADDOPTION_DAMAGE			= 2,
	ITEMADDOPTION_DEFENSE			= 3,
	ITEMADDOPTION_HP				= 4,
	ITEMADDOPTION_MANA				= 5,
#ifdef ADD_PCS_MARK_OF_EXP_20070205
	ITEMADDOPTION_EXPERIENCE		= 6,
	ITEMADDOPTION_ITEMDROPRATE		= 7,
	ITEMADDOPTION_EXPERIENCE_ZERO	= 8,
#endif // ADD_PCS_MARK_OF_EXP_20070205
	ITEMADDOPTION_STRENGTH			= 9,
	ITEMADDOPTION_DEXTERITY			= 10,
	ITEMADDOPTION_VITALITY			= 11,
	ITEMADDOPTION_ENERGY			= 12,
	ITEMADDOPTION_LEADERSHIP		= 13,
#ifdef ADD_ELITE_SCROLL_20070521
	ITEMADDOPTION_MELEEDAMAGE		= 14,
	ITEMADDOPTION_MAGICDAMAGE		= 15,
#endif // ADD_ELITE_SCROLL_20070521
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
	ITEMADDOPTION_MAPMOVEFREE		= 16,		// Only japan
#endif // ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
};

enum
{
	ITEMUSE_EFFECT_OPTION_USE		= 0,
	ITEMUSE_EFFECT_OPTION_END		= 1,
	ITEMUSE_EFFECT_OPTION_CANCEL	= 2,
};

typedef struct
{
	WORD	wOptionType;

	WORD	wEffectType1;
	WORD	wEffectType2;

	int		iItemNumber;

	int		iItemType;
	int		iItemIndex;

	int		iEffectValue1;
	int		iEffectValue2;

	int		iEffectValidTime;
}ITEMEFFECT, *LPITEMEFFECT;

static char ItemEffectName[MAX_ITEM_ADD_OPTION_ROW][20] = {"공격속도", "공격력", "방어력", "최대 HP", "최대 MP"};

class CItemAddOption  
{
public:
	CItemAddOption();
	virtual ~CItemAddOption();

	void Load(char* chFileName);
	void Initialize();

	LPITEMEFFECT GetCurrentEffectType(LPOBJECTSTRUCT lpObj);
	
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
	bool PrevSetItemLastEffectForHallowin(LPOBJECTSTRUCT lpObj);
	bool NextSetItemLastEffectForHallowin(LPOBJECTSTRUCT lpObj);
	void SendItemUse(LPOBJECTSTRUCT lpObj, int iItemNumber, int iOptionType, int iEffectType, int iEffectValue, int iValidTime );
#endif

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	bool SetItemEffect(LPOBJECTSTRUCT lpObj, int iItemNumber, int iItemUseTime = 0);
#else
	bool SetItemEffect(LPOBJECTSTRUCT lpObj, int iItemNumber);
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	bool ClearItemEffect(LPOBJECTSTRUCT lpObj, int iEffectOption);

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	bool ClearItemEffect(LPOBJECTSTRUCT lpObj, int iItemCode, int iEffectOption);
	bool PrevSetItemEffect(LPOBJECTSTRUCT lpObj);
	bool NextSetItemEffect(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	bool SearchItemEffectType(int iItemNumber, int* iEffectType1, int* iEffectType2, int* iEffectUseTime);
	#else
	bool SearchItemEffectType(int iItemNumber, int* iEffectType1, int* iEffectType2);
	#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	int GetItemEffectOption(int iItemCode);
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	bool IsValidEffect(int iItemNumber);
	LPITEMEFFECT GetItemEffectData(int iItemNumber);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	bool SetItemEffectForTest(LPOBJECTSTRUCT lpObj, int iEffectType, int iEffectValue, int iEffectValidTime);
private:
	LPITEMEFFECT _SearchItemEffect(int iItemNumber);
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
	bool _SetItemEffect(LPOBJECTSTRUCT lpObj, int iEffectType, int iEffectValue);
#else
	bool _SetItemEffect(LPOBJECTSTRUCT lpObj, int iOptionType, int iEffectType, int iEffectValue, int iItemNumber, int iEffectValidTime);
#endif // MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
	bool _ClearItemEffect(LPOBJECTSTRUCT lpObj, int iEffectOption, int iOptionType, int iEffectType, int iEffectValue);

	ITEMEFFECT m_ItemAddOption[MAX_ITEM_ADD_OPTION_ROW];
};

extern CItemAddOption g_ItemAddOption;

#endif // ITEM_ADD_OPTION_20061019

#endif // !defined(AFX_ITEMADDOPTION_H__35EBD2EC_40D5_4FA9_A9CF_4FD2DECC5B90__INCLUDED_)
