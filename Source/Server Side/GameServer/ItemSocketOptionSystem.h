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
	// 0. 소켓 아이템 스크립트를 위한 함수들.
	// 0-1. 데이터 초기화
	void Initialize();
	// 0-2. 소켓아이템 스크립트 로드
	bool LoadOptionScript(char* pchFileName);
	// 0-3. Commonserver.cfg 파일 로드
	void LoadCommonServerScript();

	//////////////////////////////////////////////////////////////////////////
	// 1. 소켓 아이템 확인을 위한 함수들.
	// 1-1. 소켓 장착이 가능한 아이템인지 확인
	bool	IsEnableSocketItem(CItem* lpItemData);
	// 1-1. 소켓 장착이 가능한 아이템인지 확인 (아이템 코드로 확인)
	bool	IsEnableSocketItem(int ItemCode);
	// 1-2. 해당 아이템의 남은 소켓 슬롯을 확인
	BYTE	GetEmptySlotCount(CItem* lpItemData);
	// 1-3. 해당 시드 스피어의 효과를 확인
	bool	GetSeedSphereEffect(BYTE btSeedSphereIndex, BYTE& pbSeedType, BYTE& pbEffectValueType, BYTE& pbEffectValue);
	// 1-4. 해당 아이템의 소켓 정보를 바이트 배열로 변환
	void	GetSocketOption(CItem* lpItemData, BYTE* pbSocketOption, BYTE& pbSocketBonusOption);
	// 1-5. 소켓 아이템인지 확인
	bool	IsSocketItem(CItem* lpItemData);
#ifndef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630				// !! NOT
	// 1-6. 소켓 슬롯이 있는 아이템인지 확인 (아이템 코드로 확인)
	bool	IsSocketItem(int ItemCode);
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	// 1-7. 시드 아이템인지 확인
	bool	IsSeedItem(int ItemCode);
	// 1-8. 스피어 아이템인지 확인
	bool	IsSphereItem(int ItemCode);
	// 1-9. 시드스피어 아이템인지 확인
	bool	IsSeedSphereItem(int ItemCode);
	// 1-10. 해당 아이템의 시드 인덱스를 확인
	BYTE	GetSeedIndex(int ItemCode, BYTE ItemLevel);
	// 1-11. 해당 아이템의 스피어 레벨을 확인
	BYTE	GetSphereLevel(int ItemCode);
	// 1-12. 해당 아이템에 같은 타입의 시드스피어가 있는지 확인
	bool	CheckItemForSameSeedType(CItem* lpItemData, WORD SeedSphereItemCode, BYTE ItemLevel);
	// 1-13. 시드스피어 아이템코드와 레벨로 시드스피어의 인덱스를 확인
	BYTE	GetSeedSphereIndexForItemCode(WORD ItemCode, BYTE ItemLevel);
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	// 1-14. 시드스피어 인덱스로 아이템 가격을 확인
	DWORD	GetSeedSphereValue(BYTE btSeedSphereIndex);
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	// 1-15. 해당 시드스피어를 해당 아이템에 적용할 수 있는지 확인
	bool	CheckSetSeedSphereItemType(CItem* lpItemData, BYTE btSeedSphereIndex);
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
#ifdef MODIFY_LIMIT_SOCKET_ITEM_DROP_MAP_RAKLION_20080825
	// 1-16. 소켓 아이템이 드롭 가능한 멥인지 확인
	bool	IsEnableDropSocketItemMap(int iMapIndex);
#endif // MODIFY_LIMIT_SOCKET_ITEM_DROP_MAP_RAKLION_20080825
#ifdef MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
	// 1-17. 스피어 드롭 확률 확인을 위한 함수
	int		GetSphereDropInfo(int iMonsterLevel);
#endif // MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903

	//////////////////////////////////////////////////////////////////////////
	// 2. 시드와 스피어로 시드스피어 조합을 위한 함수들.
	// 2-1. 시드와 스피어로 시드스피어 인덱스를 생성
	BYTE	GetSeedSphereIndex(BYTE btSeedIndex, BYTE btSphereLevel);
	// 2-2. 시드와 스피어로 시드스피어 데이터를 생성
	bool	MakeSeedSphere(CSeedSphereItem* lpSeedSphere, BYTE btSeedIndex, BYTE btSphereLevel);
	// 2-3. 시드데이터를 검색
	CSeedItem* GetSeedData(BYTE btSeedIndex);
	// 2-4. 스피어데이터를 검색
	CSphereItem* GetSphereData(BYTE btSphereLevel);
	// 2-5. 시드 추출 확률에 따른 시드 아이템 생성
	BYTE	GetExtractSeed();

	//////////////////////////////////////////////////////////////////////////
	// 3. 보너스 소켓 옵션을 생성하기 위한 함수들.
	// 3-1. 시드 스피어 인덱스로 해당 시드의 타입을 확인
	BYTE GetSocketType(BYTE btSeedSphereIndex);
	// 3-2. 해당 보너스 소켓 인덱스의 정보를 확인
	LPBONUS_SOCKET_OPTION GetBonusSocketOption(BYTE btBonusSocketOptionIndex);


	//////////////////////////////////////////////////////////////////////////
	// 4. 시드스피어의 효과를 적용시키기 위한 함수들.
	// 4-1. 시드스피어의 효과를 적용
	int ApplySeedSphereEffect(LPOBJECTSTRUCT lpObj);
	// 4-2. 보너스 소켓 옵션의 효과를 적용
	void ApplyBonusOptionEffect(LPOBJECTSTRUCT lpObj);
	// 4-3. 세트 소켓 옵션의 효과를 적용
	void ApplySetOptionEffect(LPOBJECTSTRUCT lpObj);
	// 4-4. 모든 소켓 아이템의 옵션을 적용
	bool SetApplySocketEffect(LPOBJECTSTRUCT lpObj);
	// 4-5. 사용자의 소켓 아이템 효과를 취소
	void SetClearSocketEffect(LPOBJECTSTRUCT lpObj);
	// 4-6. 소켓의 효과를 실제 적용하기 위한 함수
	void SetEffect(LPOBJECTSTRUCT lpObj, BYTE btOptionType, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	// 4-7. 각 속성별 효과를 취소
	void ClearEffect(LPOBJECTSTRUCT lpObj, LPSOCKET_OPTION_LIST lpOptionData);
	// 4-8. 각 속성별 효과를 적용
	void _SetFireEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetWaterEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetIceEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetWindEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetThunderEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetEarthEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetUniqueEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void _SetSetEffect(LPOBJECTSTRUCT lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	// 4-9. 효과 적용을 위한 수치를 계산하기 위한 함수
	int CalcEffectValue(int iInputValue, WORD wEffectValue, BYTE btEffectValueType );
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	// 4-10. 요구수치 감소 옵션을 적용하기 위한 함수
	void SetRequireStatOption(CItem* lpItemdata);
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704

	//////////////////////////////////////////////////////////////////////////
	// 5. 소켓 아이템에 소켓 슬롯을 생성하기 위한 함수들.
	// 5-1. 해당 아이템에 소켓 슬롯을 생성
	void MakeSocketSlot(CItem* lpItemData, BYTE btSocketSlotCount);
	// 5-2. 소켓 슬롯 생성 확률에 따른 소켓 개수를 설정
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	BYTE GetMakeSocketSlotCount(int iItemCode);
#else
	BYTE GetMakeSocketSlotCount();
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715

	//////////////////////////////////////////////////////////////////////////
	// 6. 소켓 사용자 데이터 확인을 위한 함수들.
	// 6-1. 사용자 소켓 데이터 초기화
	void ClearUserData(LPOBJECTSTRUCT lpObj);

	//////////////////////////////////////////////////////////////////////////
	// 7. 시드스피어를 소켓 아이템에 장착/파괴 하기 위한 함수들.
	// 7-1. 시드스피어를 소켓에 장착 (카오스 조합을 통해 소켓 아이템에 시드스피어를 장착한다.)
	bool SetSocketSlot(CItem* lpTargetItem, CItem* lpSeedSphere, BYTE btSocketSlotIndex);
	bool SetSocketSlot(CItem* lpTargetItem, BYTE btSeedIndex, BYTE btSphereLevel, BYTE btSocketSlotIndex);
	bool SetSocketSlot(CItem* lpTargetItem, BYTE btSeedSphereIndex, BYTE btSocketSlotIndex);
	// 7-2. 시드스피어 장착 후에 보너스 소켓 옵션이 적용 되는지 확인
	bool SetBonusSocketOption(CItem* lpItemData);
	// 7-3. 시드스피어를 소켓에서 삭제
	bool ClearSocketSlot(CItem* lpTargetItem, BYTE btSocketSlotIndex);

	CItemSocketOptionSystem();
	virtual ~CItemSocketOptionSystem();
private:
	CSeedItem			m_SeedData[MAX_SEED_COUNT];
	CSphereItem			m_SphereData[MAX_SPHERE_LEVEL];

	BONUS_SOCKET_OPTION	m_BonusSocketOptionData[MAX_BONUS_OPTION];
	SET_SOCKET_OPTION	m_SetSocketOptionData[MAX_SET_OPTION];

	// 시드 추출 확률
	TRandomPoolMgr	m_SeedExtractPool;

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
	// 소켓 슬롯 생성 확률
	TRandomPoolMgr	m_MakeSocketSlotCountPool;
	TRandomPoolMgr	m_TwoHandWeaponMakeSocketSlotCountPool;

	int		m_MakeSocketSlotRate[MAX_SOCKET_SLOT];
	int		m_TwoHandWeaponMakeSocketSlotRate[MAX_SOCKET_SLOT];
#else
	// 소켓 슬롯 생성 확률
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
