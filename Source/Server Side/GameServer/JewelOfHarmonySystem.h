// JewelOfHarmonySystem.h: interface for the CJewelOfHarmonySystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JEWELOFHARMONYSYSTEM_H__6F9B9B4D_CC7C_4173_884D_530D53DE5A58__INCLUDED_)
#define AFX_JEWELOFHARMONYSYSTEM_H__6F9B9B4D_CC7C_4173_884D_530D53DE5A58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

// include : �� ��÷ Ŭ����
#include "TRandomPoolMgr.h"	

// const define
const BYTE MAX_JEWELOFHARMONY_ITEM_TYPE		= 3;	// 1 : �����, 2 : �����̷�, 3 : ����
const BYTE MAX_JEWELOFHARMONY_EFFECT_OPTION	= 15;	// ������ ȿ�� ���� 1 ~ 15 ���� 
const BYTE MAX_JEWELOFHARMONY_EFFECT_VALUE	= 14;	// ������ ȿ�� ���� lv0 ~ lv13 �ܰ�

const BYTE JEWELOFHARMONY_BITSHIFT_OPTION		= 4;	// 1����Ʈ �� ���� 4bit
const BYTE JEWELOFHARMONY_BITMASK_OPTION		= 0xF0;	// ���� 4 bit
const BYTE JEWELOFHARMONY_BITMASK_OPTIONLEVEL	= 0x0F;	// ���� 4 bit

// ��ȭ�Ǻ��� NPC. ��ġ�� �̰��� �ƴ� ��ȭ���� Ŭ������ �̵� ���.
const int JEWELOFHARMONY_NPC_PURITY		= 368;
const int JEWELOFHARMONY_NPC_SMELT		= 369;
const int JEWELOFHARMONY_NPC_RESTORE	= 370;

// ������ �з�
enum eJewelOfHarmonySystem_ItemType
{
	JEWELOFHARMONY_ITEM_TYPE_NULL	= 0,	// 0 : �ش����
	JEWELOFHARMONY_ITEM_TYPE_WEAPON = 1,	// 1 : �����
	JEWELOFHARMONY_ITEM_TYPE_STAFF  = 2,	// 2 : �����̷�
	JEWELOFHARMONY_ITEM_TYPE_DEFENSE= 3,	// 3 : ����
};

// ��ȭ�� ���� ��ȭ �ɼ� ���̺�
typedef struct
{
	BOOL	bValid;				// Ȱ������
	BYTE	iRandomWeight;		// ������ ���� Ȯ�� ����ġ
	BYTE	iRequireLevel;		// �ּ� ������ ����
	BYTE	iItemEffectValue[MAX_JEWELOFHARMONY_EFFECT_VALUE];		// ȿ�� ��
	DWORD	iZenForRestore[MAX_JEWELOFHARMONY_EFFECT_VALUE];		// ȯ���� �ʿ��� ��
	char    szOptionName[256];										// �ɼ� �̸�
} JEWELOFHARMONY_ITEM_OPTION, *LPJEWELOFHARMONY_ITEM_OPTION;

// ��ȭ�� ���� ��ȭ �ɼ� ����
typedef struct
{
	short   HJOpAddMinAttackDamage;		// �ּ� ���ݷ� ���� v
	short	HJOpAddMaxAttackDamage;		// �ִ� ���ݷ� ���� v

	short	HJOpRequireStr;				// �ʿ� �� ����.  �����ۿ� ����.
	short	HJOpRequireDex;				// �ʿ� ��ø ����. �����ۿ� ����.
	
	short   HJOpAddAttackDamage;		// ���ݷ� ���� ( �ּ�. �ִ�)
	short	HJOpAddCriticalDamage;		// ũ��Ƽ�� ������ ���� v
	short	HJOpAddSkillAttack;			// ��ų ���ݷ� ���� v

	short	HJOpAddAttackSuccessRatePVP;	// ���� ������ ��� PvP
	short	HJOpDecreaseSDRate;				// ��� �ǵ� ����� ����
	short	HJOpAddIgnoreSDRate;			// ������ Ȯ�� ��ŭ ����� SD�� �����ϰ� ���� HP������.

	short	HJOpAddMagicPower;				// �߰� ���� v

	short	HJOpAddDefense;					// �߰� ���� v
	short	HJOpAddMaxAG;					// �ִ� AG ���
	short	HJOpAddMaxHP;					// �ִ� HP ���
	short	HJOpAddRefillHP;				// ����� �ڵ� ������ ��� : ??? �������� �ִٸ� �߰�?
	short	HJOpAddRefillMP;				// ���� �ڵ� ������ ���
	short	HJOpAddDefenseSuccessRatePvP;	// ��� ������ ��� PvP
	short	HJOpAddDamageDecrease;			// ������ ���ҷ� % ���
	short	HJOpAddSDRate;					// ���� ����ϴ� SD ���� ���
} JEWELOFHARMONY_ITEM_EFFECT, *LPJEWELOFHARMONY_ITEM_EFFECT;

class CJewelOfHarmonySystem  
{
public:
	CJewelOfHarmonySystem();
	virtual ~CJewelOfHarmonySystem();

private:
	// �⺻ ���� ��
	BOOL	m_bEnable;								// ���� �ý��� ��� ���� ����. TRUE �� ��� ��� ����.
	int		JEWEL_OF_HARMONY_ITEMINDEX;				// ��ȭ�Ǻ��� ���� 
	int		JEWEL_OF_HARMONY_PURITY_ITEMINDEX;		// ��ȭ�Ǻ��� ������
	int		JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX;	// �ϱ� ���ü�
	int		JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX;	// ��� ���ü�

	// ��ȭ �ɼ� ������
	JEWELOFHARMONY_ITEM_OPTION m_itemOption[MAX_JEWELOFHARMONY_ITEM_TYPE+1][MAX_JEWELOFHARMONY_EFFECT_OPTION+1];

	// ȯ�� ���� : �ý��� On/Off
	BOOL	m_bSystemPrutiyJewel;		// ���� �ý��� On/Off
	BOOL	m_bSystemMixSmeltingStone;	// ���ü� ���� �ý��� On/Off
	BOOL	m_bSystemRestoreStrengthen; // ��ȭ ������ ȯ�� �ý��� On/Off
	BOOL	m_bSystemStrengthenItem;	// ������ ��ȭ �ý��� On/Off
	BOOL	m_bSystemSmeltingItem;		// ���ü��� ���� ��ȭ������ ���׷��̵� �ý��� On/Off

	// ȯ�� ���� : Ȯ�� �� �ʿ� ������
	int		m_iRatePuritySuccess;		// ���� Ȯ�� ( 1 / 100 )
	int		m_iZenForPurity;			// ���� �ʿ� �� (Ȯ�� ���)
	int		m_iRateMixSmeltingStoneNor;	// ���ü� ���� ������. �Ϲ� ������ ( 1 / 100 )
	int		m_iRateMixSmeltingStoneExt;	// ���ü� ���� ������. ���� ������ ( 1 / 100 )
	int		m_iZenForMixSmeltingStone;	// ���ü� ���� �ʿ��� (Ȯ�� ���)
	int		m_iRateStrengthenSuccess;	// ��ȭ ���� Ȯ�� ( 1 / 100 )
	int		m_iRateSmeltingSuccessNor;	// �ϱ����ü��� ���� ��ȭ������ ���׷��̵� ���� Ȯ�� ( 1/ 100)
	int		m_iRateSmeltingSuccessExt;	// �ϱ����ü��� ���� ��ȭ������ ���׷��̵� ���� Ȯ�� ( 1/ 100)

	// ���ü� ���� ������ : ���� ���� ���
	map<int, int> m_mapEnableMixList;

	// ��ȭ�ɼ� ���� Ŭ����
	TRandomPoolMgr m_kRandomPool;

private:
	void	_InitOption();		// �� �ʱ�ȭ

public:
	BOOL	LoadScript( LPSTR lpszFileName );				// �ɼ� ����Ÿ �ε�
	BOOL	LoadScriptOfSmelt( LPSTR lpszFileName );		// ���ü� ���� ��� ������ ����Ÿ �ε�

	BOOL	IsJewelOfHarmonyOriginal(short type);			// ��ȭ�Ǻ��� ���� Ȯ��
	BOOL	IsJewelOfHarmonyPurity(short type);				// ��ȭ�Ǻ��� ������ Ȯ��
	BOOL	IsJewelOfHarmonySmeltingItems(short type);		// ���ü� Ȯ�� (����)
private:
	BOOL	_IsJewelOfHarmonySmeltingItemNor(short type);	// �ϱ� ���ü� Ȯ��
	BOOL	_IsJewelOfHarmonySmeltingItemExt(short type);	// ��� ���ü� Ȯ��

public:
	void	SetEnableToUsePuritySystem(BOOL bEnable);		// ���� �ý��� ��� ���� ���� ����
	BOOL	IsEnableToUsePuritySystem();					// ���� �ý��� ��� ���� ���� ����
	void	PurityJewelOfHarmony(LPOBJECTSTRUCT lpObj);		// ��ȭ�� ���� ����
	
#ifdef MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729		// public���� �ű�
	int		_GetItemType(CItem *pItem);								// �������� ���� : �����/�����̷�/����
#else
	//int		_GetItemType(CItem *pItem);								// �������� ���� : �����/�����̷�/����
#endif // MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729

private:
	BYTE	_GetItemOptionLevel(CItem *pItem);						// ��ȭ ������ �ɼ� ����
	BYTE	_GetItemOptionRequireLevel(CItem *pItem);				// ��ȭ �������� �ּ� �䱸 ���� : ���� ���� �� ���

#ifdef MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729		// public���� �ű�
	//int		_GetItemType(CItem *pItem);								// �������� ���� : �����/�����̷�/����
#else
	int		_GetItemType(CItem *pItem);								// �������� ���� : �����/�����̷�/����
#endif // MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729

	int		_GetSelectRandomOption(CItem *pItem, int iItemType);	// ��ȭ �ɼ� ����

public:
	BYTE	GetItemStrengthenOption(CItem *pItem);					// ��ȭ ������ �ɼ� ����
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	BYTE	GetItemOptionLevel(CItem *pItem);						// ��ȭ ������ �ɼ� ����
#endif
	BOOL	IsActive(CItem *pItem);									// ��ȭ �ɼ� Ȱ��ȭ ����

public:
	BOOL	IsStrengthenByJewelOfHarmony(CItem *pItem);				// ��ȭ�� ���������� Ȯ��
	BOOL	StrengthenItemByJewelOfHarmony(LPOBJECTSTRUCT lpObj, int source, int target);									// ��ȭ�Ǻ����� �̿��� ��ȭ
	void	StrengthenItemByMacro(LPOBJECTSTRUCT lpObj, BYTE invenrotyTargetPos, BYTE btOptionType, BYTE btOptionLevel);	// ��ũ�θ� �̿��� ��ȭ
private:
	BOOL	_MakeOption(CItem *pItem, BYTE btOptionType, BYTE btOptionLevel);	// �ɼǰ� �ɼǷ��� ����

	// ������ �ɼ� ����
public:
	void	SetApplyStrengthenItem( LPOBJECTSTRUCT lpObj );					// ��ȭ ������ ����
	void	InitEffectValue( LPJEWELOFHARMONY_ITEM_EFFECT pItemEffect );	// ��ȭ ������ ȿ�� �� �ʱ�ȭ
	int		GetItemEffectValue(CItem *pItem, int iOptionType);				// Ư�� �ɼ��� �� ��������
private:
	BOOL	_CalcItemEffectValue(CItem *pItem, LPJEWELOFHARMONY_ITEM_EFFECT pItemEffect);	// ��ȭ �ɼ� �� ���

	// ���ü� �ý���
public:
	BOOL	IsEnableToMakeSmeltingStoneItem( CItem *pItem );						// ���ü� ���� ���� ������ ���� Ȯ��
	BOOL	MakeSmeltingStoneItem(LPOBJECTSTRUCT lpObj);							// ���ü� ����
	BOOL	SmeltItemBySmeltingStone(LPOBJECTSTRUCT lpObj, int source, int target);	// ���ü��� ���� ��ȭ������ ���׷��̵�

	// ȯ�� �ý���
private:
	int		_GetZenForRestoreItem(CItem *pItem);			// ȯ���� �ʿ��� ��
public:
	BOOL	RestoreStrengthenItem(LPOBJECTSTRUCT lpObj);	// ������ ȯ��


	// Preview ����Ÿ : ����Ʈ ����Ÿ ����
	BYTE	MakeCharSetData( LPOBJECTSTRUCT lpObj );

	// NPC ��ȭ ó��
	BOOL	NpcJewelOfHarmony(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

	// ��ȭ �ɼ� �� : �����.
	BYTE	ShowStrengthenOption(CItem *pItem);

	// �ŷ� �� Ȯ��
public:
	BOOL	IsEnableToTrade(LPOBJECTSTRUCT lpObj);
	
};

extern CJewelOfHarmonySystem	g_kJewelOfHarmonySystem;

#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#endif // !defined(AFX_JEWELOFHARMONYSYSTEM_H__6F9B9B4D_CC7C_4173_884D_530D53DE5A58__INCLUDED_)
