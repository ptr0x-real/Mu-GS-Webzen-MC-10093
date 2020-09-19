// ItemSocketOptionSystem.h: interface for the CItemSocketOptionSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSOCKETOPTIONSYSTEM_H__735DC443_344C_4259_9AF3_898D34B7802B__INCLUDED_)
#define AFX_ITEMSOCKETOPTIONSYSTEM_H__735DC443_344C_4259_9AF3_898D34B7802B__INCLUDED_

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common\ItemSocketOptionDefine.h"
#include "ItemSeedSphere.h"

#include "TRandomPoolMgr.h"

typedef struct
{
	BYTE	OptionIndex;
	BYTE	EffectType;
	BYTE	ItemTypeBegin;
	BYTE	ItemTypeEnd;
	BYTE	OptionValueType;
	BYTE	OptionValue;
	int		ExtractRate;
	BYTE	SocketSlotType[MAX_SOCKET_SLOT];
	BYTE	SocketSlotCount;

	void Clear()
	{
		OptionIndex = 0;
		EffectType = 0;
		ItemTypeBegin = 0;
		ItemTypeEnd = 0;
		OptionValueType = 0;
		OptionValue = 0;
		ExtractRate = 0;
		for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
		{
			SocketSlotType[i] = 0;
		}
		SocketSlotCount = 0;
	};
}BONUS_SOCKET_OPTION, *LPBONUS_SOCKET_OPTION;

typedef struct
{
	BYTE	OptionIndex;
	BYTE	OptionValueType;
	BYTE	OptionValue;
	BYTE	EffectType;
	int		ExtractRate;
	BYTE	SeedTypeTable[6];

	void Clear()
	{
		OptionIndex = 0;
		OptionValueType = 0;
		OptionValue = 0;
		EffectType = 0;
		ExtractRate = 0;
		for( int i = 0 ; i < 6 ; i++ )
		{
			SeedTypeTable[i] = 0;
		}
	}
}SET_SOCKET_OPTION, *LPSET_SOCKET_OPTION;

class CItemSocketOptionSystem  
{
public:
	//////////////////////////////////////////////////////////////////////////
	// 0. ���� ������ ��ũ��Ʈ�� ���� �Լ���.
	// 0-1. ������ �ʱ�ȭ
	void Initialize();
	// 0-2. ���Ͼ����� ��ũ��Ʈ �ε�
	bool LoadOptionScript(char* pchFileName);
	// 0-3. Commonserver.cfg ���� �ε�
	void LoadCommonServerScript();

	//////////////////////////////////////////////////////////////////////////
	// 1. ���� ������ Ȯ���� ���� �Լ���.
	// 1-1. ���� ������ ������ ���������� Ȯ��
	bool	IsEnableSocketItem(CItem* lpItemData);
	// 1-1. ���� ������ ������ ���������� Ȯ�� (������ �ڵ�� Ȯ��)
	bool	IsEnableSocketItem(int ItemCode);
	// 1-2. �ش� �������� ���� ���� ������ Ȯ��
	BYTE	GetEmptySlotCount(CItem* lpItemData);
	// 1-3. �ش� �õ� ���Ǿ��� ȿ���� Ȯ��
	bool	GetSeedSphereEffect(BYTE btSeedSphereIndex, BYTE& pbSeedType, BYTE& pbEffectValueType, BYTE& pbEffectValue);
	// 1-4. �ش� �������� ���� ������ ����Ʈ �迭�� ��ȯ
	void	GetSocketOption(CItem* lpItemData, BYTE* pbSocketOption, BYTE& pbSocketBonusOption);
	// 1-5. ���� ���������� Ȯ��
	bool	IsSocketItem(CItem* lpItemData);
#ifndef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630				// !! NOT
	// 1-6. ���� ������ �ִ� ���������� Ȯ�� (������ �ڵ�� Ȯ��)
	bool	IsSocketItem(int ItemCode);
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	// 1-7. �õ� ���������� Ȯ��
	bool	IsSeedItem(int ItemCode);
	// 1-8. ���Ǿ� ���������� Ȯ��
	bool	IsSphereItem(int ItemCode);
	// 1-9. �õ彺�Ǿ� ���������� Ȯ��
	bool	IsSeedSphereItem(int ItemCode);
	// 1-10. �ش� �������� �õ� �ε����� Ȯ��
	BYTE	GetSeedIndex(int ItemCode, BYTE ItemLevel);
	// 1-11. �ش� �������� ���Ǿ� ������ Ȯ��
	BYTE	GetSphereLevel(int ItemCode);
	// 1-12. �ش� �����ۿ� ���� Ÿ���� �õ彺�Ǿ �ִ��� Ȯ��
	bool	CheckItemForSameSeedType(CItem* lpItemData, WORD SeedSphereItemCode, BYTE ItemLevel);
	// 1-13. �õ彺�Ǿ� �������ڵ�� ������ �õ彺�Ǿ��� �ε����� Ȯ��
	BYTE	GetSeedSphereIndexForItemCode(WORD ItemCode, BYTE ItemLevel);
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	// 1-14. �õ彺�Ǿ� �ε����� ������ ������ Ȯ��
	DWORD	GetSeedSphereValue(BYTE btSeedSphereIndex);
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	// 1-15. �ش� �õ彺�Ǿ �ش� �����ۿ� ������ �� �ִ��� Ȯ��
	bool	CheckSetSeedSphereItemType(CItem* lpItemData, BYTE btSeedSphereIndex);
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
#ifdef MODIFY_LIMIT_SOCKET_ITEM_DROP_MAP_RAKLION_20080825
	// 1-16. ���� �������� ��� ������ ������ Ȯ��
	bool	IsEnableDropSocketItemMap(int iMapIndex);
#endif // MODIFY_LIMIT_SOCKET_ITEM_DROP_MAP_RAKLION_20080825
#ifdef MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
	// 1-17. ���Ǿ� ��� Ȯ�� Ȯ���� ���� �Լ�
	int		GetSphereDropInfo(int iMonsterLevel);
#endif // MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903

	//////////////////////////////////////////////////////////////////////////
	// 2. �õ�� ���Ǿ�� �õ彺�Ǿ� ������ ���� �Լ���.
	// 2-1. �õ�� ���Ǿ�� �õ彺�Ǿ� �ε����� ����
	BYTE	GetSeedSphereIndex(BYTE btSeedIndex, BYTE btSphereLevel);
	// 2-2. �õ�� ���Ǿ�� �õ彺�Ǿ� �����͸� ����
	bool	MakeSeedSphere(CSeedSphereItem* lpSeedSphere, BYTE btSeedIndex, BYTE btSphereLevel);
	// 2-3. �õ嵥���͸� �˻�
	CSeedItem* GetSeedData(BYTE btSeedIndex);
	// 2-4. ���Ǿ���͸� �˻�
	CSphereItem* GetSphereData(BYTE btSphereLevel);
	// 2-5. �õ� ���� Ȯ���� ���� �õ� ������ ����
	BYTE	GetExtractSeed();

	//////////////////////////////////////////////////////////////////////////
	// 3. ���ʽ� ���� �ɼ��� �����ϱ� ���� �Լ���.
	// 3-1. �õ� ���Ǿ� �ε����� �ش� �õ��� Ÿ���� Ȯ��
	BYTE GetSocketType(BYTE btSeedSphereIndex);
	// 3-2. �ش� ���ʽ� ���� �ε����� ������ Ȯ��
	LPBONUS_SOCKET_OPTION GetBonusSocketOption(BYTE btBonusSocketOptionIndex);


	//////////////////////////////////////////////////////////////////////////
	// 4. �õ彺�Ǿ��� ȿ���� �����Ű�� ���� �Լ���.
	// 4-1. �õ彺�Ǿ��� ȿ���� ����
	int ApplySeedSphereEffect(LPOBJECTSTRUCT lpObj);
	// 4-2. ���ʽ� ���� �ɼ��� ȿ���� ����
	void ApplyBonusOptionEffect(LPOBJECTSTRUCT lpObj);
	// 4-3. ��Ʈ ���� �ɼ��� ȿ���� ����
	void ApplySetOptionEffect(LPOBJECTSTRUCT lpObj);
	// 4-4. ��� ���� �������� �ɼ��� ����
	bool SetApplySocketEffect(LPOBJECTSTRUCT lpObj);
	// 4-5. ������� ���� ������ ȿ���� ���
	void SetClearSocketEffect(LPOBJECTSTRUCT lpObj);
	// 4-6. ������ ȿ���� ���� �����ϱ� ���� �Լ�
	void SetEffect(LPOBJECTSTRUCT lpObj, BYTE btOptionType, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	// 4-7. �� �Ӽ��� ȿ���� ���
	void ClearEffect(LPOBJECTSTRUCT lpObj, LPSOCKET_OPTION_LIST lpOptionData);
	// 4-8. �� �Ӽ��� ȿ���� ����
	void _SetFireEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetWaterEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetIceEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetWindEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetThunderEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetEarthEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetUniqueEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetSetEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	// 4-9. ȿ�� ������ ���� ��ġ�� ����ϱ� ���� �Լ�
	int CalcEffectValue(int iInputValue, WORD wEffectValue, BYTE btEffectValueType );
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	// 4-10. �䱸��ġ ���� �ɼ��� �����ϱ� ���� �Լ�
	void SetRequireStatOption(CItem* lpItemdata);
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704

	//////////////////////////////////////////////////////////////////////////
	// 5. ���� �����ۿ� ���� ������ �����ϱ� ���� �Լ���.
	// 5-1. �ش� �����ۿ� ���� ������ ����
	void MakeSocketSlot(CItem* lpItemData, BYTE btSocketSlotCount);
	// 5-2. ���� ���� ���� Ȯ���� ���� ���� ������ ����
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	BYTE GetMakeSocketSlotCount(int iItemCode);
#else
	BYTE GetMakeSocketSlotCount();
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715

	//////////////////////////////////////////////////////////////////////////
	// 6. ���� ����� ������ Ȯ���� ���� �Լ���.
	// 6-1. ����� ���� ������ �ʱ�ȭ
	void ClearUserData(LPOBJECTSTRUCT lpObj);

	//////////////////////////////////////////////////////////////////////////
	// 7. �õ彺�Ǿ ���� �����ۿ� ����/�ı� �ϱ� ���� �Լ���.
	// 7-1. �õ彺�Ǿ ���Ͽ� ���� (ī���� ������ ���� ���� �����ۿ� �õ彺�Ǿ �����Ѵ�.)
	bool SetSocketSlot(CItem* lpTargetItem, CItem* lpSeedSphere, BYTE btSocketSlotIndex);
	bool SetSocketSlot(CItem* lpTargetItem, BYTE btSeedIndex, BYTE btSphereLevel, BYTE btSocketSlotIndex);
	bool SetSocketSlot(CItem* lpTargetItem, BYTE btSeedSphereIndex, BYTE btSocketSlotIndex);
	// 7-2. �õ彺�Ǿ� ���� �Ŀ� ���ʽ� ���� �ɼ��� ���� �Ǵ��� Ȯ��
	bool SetBonusSocketOption(CItem* lpItemData);
	// 7-3. �õ彺�Ǿ ���Ͽ��� ����
	bool ClearSocketSlot(CItem* lpTargetItem, BYTE btSocketSlotIndex);

	CItemSocketOptionSystem();
	virtual ~CItemSocketOptionSystem();
private:
	CSeedItem			m_SeedData[MAX_SEED_COUNT];
	CSphereItem			m_SphereData[MAX_SPHERE_LEVEL];

	BONUS_SOCKET_OPTION	m_BonusSocketOptionData[MAX_BONUS_OPTION];
	SET_SOCKET_OPTION	m_SetSocketOptionData[MAX_SET_OPTION];

	// �õ� ���� Ȯ��
	TRandomPoolMgr	m_SeedExtractPool;

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	// ���� ���� ���� Ȯ��
	TRandomPoolMgr	m_MakeSocketSlotCountPool;
	TRandomPoolMgr	m_TwoHandWeaponMakeSocketSlotCountPool;

	int		m_MakeSocketSlotRate[MAX_SOCKET_SLOT];
	int		m_TwoHandWeaponMakeSocketSlotRate[MAX_SOCKET_SLOT];
#else
	// ���� ���� ���� Ȯ��
	TRandomPoolMgr	m_MakeSocketSlotCountPool;

	int		m_MakeSocketSlotRate[MAX_SOCKET_SLOT];
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715

#ifdef MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
	int m_SphereDropOn;
	int m_SphereDropRate[MAX_SPHERE_LEVEL];
	int m_SphereDropLevel[MAX_SPHERE_LEVEL];
#endif // MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
};

extern CItemSocketOptionSystem g_SocketOptionSystem;
extern int g_SphereDropRate;
extern int g_RequireMoneyForSeedExtract;
extern int g_RequireMoneyForSeedSphereComposite;
extern int g_RequireMoneyForSeedSphereSet;
extern int g_RequireMoneyForSeedSphereRemove;

#ifdef MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808
extern int g_ShopBuySocketItemSlotCount;
#endif // MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808

#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#endif // !defined(AFX_ITEMSOCKETOPTIONSYSTEM_H__735DC443_344C_4259_9AF3_898D34B7802B__INCLUDED_)
