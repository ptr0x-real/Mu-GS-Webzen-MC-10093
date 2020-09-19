// JewelOfHarmonySystem.h: interface for the CJewelOfHarmonySystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JEWELOFHARMONYSYSTEM_H__6F9B9B4D_CC7C_4173_884D_530D53DE5A58__INCLUDED_)
#define AFX_JEWELOFHARMONYSYSTEM_H__6F9B9B4D_CC7C_4173_884D_530D53DE5A58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

// include : 값 추첨 클래스
#include "TRandomPoolMgr.h"	

// const define
const BYTE MAX_JEWELOFHARMONY_ITEM_TYPE		= 3;	// 1 : 무기류, 2 : 지팡이류, 3 : 방어구류
const BYTE MAX_JEWELOFHARMONY_EFFECT_OPTION	= 15;	// 아이템 효과 종류 1 ~ 15 가지 
const BYTE MAX_JEWELOFHARMONY_EFFECT_VALUE	= 14;	// 아이템 효과 레벨 lv0 ~ lv13 단계

const BYTE JEWELOFHARMONY_BITSHIFT_OPTION		= 4;	// 1바이트 중 상위 4bit
const BYTE JEWELOFHARMONY_BITMASK_OPTION		= 0xF0;	// 상위 4 bit
const BYTE JEWELOFHARMONY_BITMASK_OPTIONLEVEL	= 0x0F;	// 하위 4 bit

// 조화의보석 NPC. 위치를 이곳이 아닌 조화보석 클래스로 이동 고려.
const int JEWELOFHARMONY_NPC_PURITY		= 368;
const int JEWELOFHARMONY_NPC_SMELT		= 369;
const int JEWELOFHARMONY_NPC_RESTORE	= 370;

// 아이템 분류
enum eJewelOfHarmonySystem_ItemType
{
	JEWELOFHARMONY_ITEM_TYPE_NULL	= 0,	// 0 : 해당없음
	JEWELOFHARMONY_ITEM_TYPE_WEAPON = 1,	// 1 : 무기류
	JEWELOFHARMONY_ITEM_TYPE_STAFF  = 2,	// 2 : 지팡이류
	JEWELOFHARMONY_ITEM_TYPE_DEFENSE= 3,	// 3 : 방어구류
};

// 조화의 보석 강화 옵션 테이블
typedef struct
{
	BOOL	bValid;				// 활성상태
	BYTE	iRandomWeight;		// 선택을 위한 확률 가중치
	BYTE	iRequireLevel;		// 최소 아이템 레벨
	BYTE	iItemEffectValue[MAX_JEWELOFHARMONY_EFFECT_VALUE];		// 효과 값
	DWORD	iZenForRestore[MAX_JEWELOFHARMONY_EFFECT_VALUE];		// 환원에 필요한 젠
	char    szOptionName[256];										// 옵션 이름
} JEWELOFHARMONY_ITEM_OPTION, *LPJEWELOFHARMONY_ITEM_OPTION;

// 조화의 보석 강화 옵션 변수
typedef struct
{
	short   HJOpAddMinAttackDamage;		// 최소 공격력 증가 v
	short	HJOpAddMaxAttackDamage;		// 최대 공격력 증가 v

	short	HJOpRequireStr;				// 필요 힘 감소.  아이템에 적용.
	short	HJOpRequireDex;				// 필요 민첩 감소. 아이템에 적용.
	
	short   HJOpAddAttackDamage;		// 공격력 증가 ( 최소. 최대)
	short	HJOpAddCriticalDamage;		// 크리티컬 데미지 증가 v
	short	HJOpAddSkillAttack;			// 스킬 공격력 증가 v

	short	HJOpAddAttackSuccessRatePVP;	// 공격 성공률 상승 PvP
	short	HJOpDecreaseSDRate;				// 상대 실드 흡수율 감소
	short	HJOpAddIgnoreSDRate;			// 정해진 확률 만큼 상대의 SD를 무시하고 직접 HP데미지.

	short	HJOpAddMagicPower;				// 추가 마력 v

	short	HJOpAddDefense;					// 추가 방어력 v
	short	HJOpAddMaxAG;					// 최대 AG 상승
	short	HJOpAddMaxHP;					// 최대 HP 상승
	short	HJOpAddRefillHP;				// 생명력 자동 증가량 상승 : ??? 아이템이 있다면 추가?
	short	HJOpAddRefillMP;				// 마나 자동 증가량 상승
	short	HJOpAddDefenseSuccessRatePvP;	// 방어 성공률 상승 PvP
	short	HJOpAddDamageDecrease;			// 데미지 감소량 % 상승
	short	HJOpAddSDRate;					// 방어시 흡수하는 SD 비율 상승
} JEWELOFHARMONY_ITEM_EFFECT, *LPJEWELOFHARMONY_ITEM_EFFECT;

class CJewelOfHarmonySystem  
{
public:
	CJewelOfHarmonySystem();
	virtual ~CJewelOfHarmonySystem();

private:
	// 기본 설정 값
	BOOL	m_bEnable;								// 정제 시스템 사용 가능 여부. TRUE 인 경우 사용 가능.
	int		JEWEL_OF_HARMONY_ITEMINDEX;				// 조화의보석 원석 
	int		JEWEL_OF_HARMONY_PURITY_ITEMINDEX;		// 조화의보석 정제석
	int		JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX;	// 하급 제련석
	int		JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX;	// 상급 제련석

	// 강화 옵션 데이터
	JEWELOFHARMONY_ITEM_OPTION m_itemOption[MAX_JEWELOFHARMONY_ITEM_TYPE+1][MAX_JEWELOFHARMONY_EFFECT_OPTION+1];

	// 환경 변수 : 시스템 On/Off
	BOOL	m_bSystemPrutiyJewel;		// 정제 시스템 On/Off
	BOOL	m_bSystemMixSmeltingStone;	// 제련석 조합 시스템 On/Off
	BOOL	m_bSystemRestoreStrengthen; // 강화 아이템 환원 시스템 On/Off
	BOOL	m_bSystemStrengthenItem;	// 아이템 강화 시스템 On/Off
	BOOL	m_bSystemSmeltingItem;		// 제련석에 의한 강화아이템 업그레이드 시스템 On/Off

	// 환경 변수 : 확률 및 필요 젠관련
	int		m_iRatePuritySuccess;		// 정제 확률 ( 1 / 100 )
	int		m_iZenForPurity;			// 정제 필요 젠 (확장 고려)
	int		m_iRateMixSmeltingStoneNor;	// 제련석 조합 성공율. 일반 아이템 ( 1 / 100 )
	int		m_iRateMixSmeltingStoneExt;	// 제련석 조합 성공율. 엑설 아이템 ( 1 / 100 )
	int		m_iZenForMixSmeltingStone;	// 제련석 조합 필요젠 (확장 고려)
	int		m_iRateStrengthenSuccess;	// 강화 성공 확률 ( 1 / 100 )
	int		m_iRateSmeltingSuccessNor;	// 하급제련석에 의한 강화아이템 업그레이드 성공 확률 ( 1/ 100)
	int		m_iRateSmeltingSuccessExt;	// 하급제련석에 의한 강화아이템 업그레이드 성공 확률 ( 1/ 100)

	// 제련석 조합 아이템 : 레벨 제한 목록
	map<int, int> m_mapEnableMixList;

	// 강화옵션 선택 클래스
	TRandomPoolMgr m_kRandomPool;

private:
	void	_InitOption();		// 값 초기화

public:
	BOOL	LoadScript( LPSTR lpszFileName );				// 옵션 데이타 로드
	BOOL	LoadScriptOfSmelt( LPSTR lpszFileName );		// 제련석 조합 대상 아이템 데이타 로드

	BOOL	IsJewelOfHarmonyOriginal(short type);			// 조화의보석 원석 확인
	BOOL	IsJewelOfHarmonyPurity(short type);				// 조화의보석 정제석 확인
	BOOL	IsJewelOfHarmonySmeltingItems(short type);		// 제련석 확인 (통합)
private:
	BOOL	_IsJewelOfHarmonySmeltingItemNor(short type);	// 하급 제련석 확인
	BOOL	_IsJewelOfHarmonySmeltingItemExt(short type);	// 상급 제련석 확인

public:
	void	SetEnableToUsePuritySystem(BOOL bEnable);		// 정제 시스템 사용 가능 여부 설정
	BOOL	IsEnableToUsePuritySystem();					// 정제 시스템 사용 가능 한지 여부
	void	PurityJewelOfHarmony(LPOBJECTSTRUCT lpObj);		// 조화의 보석 정제
	
#ifdef MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729		// public으로 옮김
	int		_GetItemType(CItem *pItem);								// 아이템의 종류 : 무기류/지팡이류/방어구류
#else
	//int		_GetItemType(CItem *pItem);								// 아이템의 종류 : 무기류/지팡이류/방어구류
#endif // MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729

private:
	BYTE	_GetItemOptionLevel(CItem *pItem);						// 강화 아이템 옵션 레벨
	BYTE	_GetItemOptionRequireLevel(CItem *pItem);				// 강화 아이템의 최소 요구 레벨 : 제련 실패 시 사용

#ifdef MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729		// public으로 옮김
	//int		_GetItemType(CItem *pItem);								// 아이템의 종류 : 무기류/지팡이류/방어구류
#else
	int		_GetItemType(CItem *pItem);								// 아이템의 종류 : 무기류/지팡이류/방어구류
#endif // MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729

	int		_GetSelectRandomOption(CItem *pItem, int iItemType);	// 강화 옵션 결정

public:
	BYTE	GetItemStrengthenOption(CItem *pItem);					// 강화 아이템 옵션 정보
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	BYTE	GetItemOptionLevel(CItem *pItem);						// 강화 아이템 옵션 레벨
#endif
	BOOL	IsActive(CItem *pItem);									// 강화 옵션 활성화 여부

public:
	BOOL	IsStrengthenByJewelOfHarmony(CItem *pItem);				// 강화된 아이템인지 확인
	BOOL	StrengthenItemByJewelOfHarmony(LPOBJECTSTRUCT lpObj, int source, int target);									// 조화의보석을 이용한 강화
	void	StrengthenItemByMacro(LPOBJECTSTRUCT lpObj, BYTE invenrotyTargetPos, BYTE btOptionType, BYTE btOptionLevel);	// 매크로를 이용한 강화
private:
	BOOL	_MakeOption(CItem *pItem, BYTE btOptionType, BYTE btOptionLevel);	// 옵션과 옵션레벨 설정

	// 아이템 옵션 적용
public:
	void	SetApplyStrengthenItem( LPOBJECTSTRUCT lpObj );					// 강화 아이템 적용
	void	InitEffectValue( LPJEWELOFHARMONY_ITEM_EFFECT pItemEffect );	// 강화 아이템 효과 값 초기화
	int		GetItemEffectValue(CItem *pItem, int iOptionType);				// 특정 옵션의 값 가져오기
private:
	BOOL	_CalcItemEffectValue(CItem *pItem, LPJEWELOFHARMONY_ITEM_EFFECT pItemEffect);	// 강화 옵션 값 계산

	// 제련석 시스템
public:
	BOOL	IsEnableToMakeSmeltingStoneItem( CItem *pItem );						// 제련석 조합 가능 아이템 여부 확인
	BOOL	MakeSmeltingStoneItem(LPOBJECTSTRUCT lpObj);							// 제련석 조합
	BOOL	SmeltItemBySmeltingStone(LPOBJECTSTRUCT lpObj, int source, int target);	// 제련석에 의한 강화아이템 업그레이드

	// 환원 시스템
private:
	int		_GetZenForRestoreItem(CItem *pItem);			// 환원에 필요한 젠
public:
	BOOL	RestoreStrengthenItem(LPOBJECTSTRUCT lpObj);	// 아이템 환원


	// Preview 데이타 : 뷰포트 데이타 생성
	BYTE	MakeCharSetData( LPOBJECTSTRUCT lpObj );

	// NPC 대화 처리
	BOOL	NpcJewelOfHarmony(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

	// 강화 옵션 값 : 디버깅.
	BYTE	ShowStrengthenOption(CItem *pItem);

	// 거래 시 확인
public:
	BOOL	IsEnableToTrade(LPOBJECTSTRUCT lpObj);
	
};

extern CJewelOfHarmonySystem	g_kJewelOfHarmonySystem;

#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#endif // !defined(AFX_JEWELOFHARMONYSYSTEM_H__6F9B9B4D_CC7C_4173_884D_530D53DE5A58__INCLUDED_)
