// ItemSystemFor380.h: interface for the CItemSystemFor380 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSYSTEMFOR380_H__C69D017E_B1AA_4120_81D8_971216D676D6__INCLUDED_)
#define AFX_ITEMSYSTEMFOR380_H__C69D017E_B1AA_4120_81D8_971216D676D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_380ITEM_NEWOPTION_20060711

// 추가 아이템 효과
const BYTE ITEMOPTION_FOR380ITEM_NULL					  = 0;	// 옵션 없음
const BYTE ITEMOPTION_FOR380ITEM_ATTACKSUCCESRATE_PVP	  = 1;	// 대인 공격 성공률 상승
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_DAMAGE_PVP		  = 2;	// 대인 데미지증가 +%d
const BYTE ITEMOPTION_FOR380ITEM_BLOCKSUCCESRATE_PVP	  = 3;	// 대인 방어 성공률 상승
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_DEFENSE_PVP	  = 4;  // 대인 방어력 +%d
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_MAXHP			  = 5;	// 최대 HP 상승
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_MAXSD			  = 6;	// 최대 SD 상승
const BYTE ITEMOPTION_FOR380ITEM_RECOVER_SD				  = 7;	// SD 자동회복 : 안전지대가 아니더라도.
const BYTE ITEMOPTION_FOR380ITEM_IMPROVE_REFILLSD		  = 8;	// SD 자동 회복량 증가

// 380 추가 옵션 테이블 정보
#ifdef MODIFY_380ITEM_SCRIPTTYPE_EXTEND_20060811
	typedef struct 
	{
		BOOL IsLoad;		// 활성 상태
		BYTE m_Type;		// 아이템 타입
		BYTE m_Index;		// 아이템 인덱스
		BYTE m_Option1;		// 아이템 옵션1
		WORD m_Value1;		// 아이템 옵션1 값
		BYTE m_Option2;		// 아이템 옵션2
		WORD m_Value2;		// 아이템 옵션2 값
	} ITEMOPTION_FOR380ITEM;
#else
	typedef struct 
	{
		BOOL IsLoad;		// 활성 상태
		BYTE m_Type;		// 아이템 타입
		BYTE m_Index;		// 아이템 인덱스
		BYTE m_Option1;		// 아이템 옵션1
		BYTE m_Value1;		// 아이템 옵션1 값
		BYTE m_Option2;		// 아이템 옵션2
		BYTE m_Value2;		// 아이템 옵션2 값
	} ITEMOPTION_FOR380ITEM;
#endif // MODIFY_380ITEM_SCRIPTTYPE_EXTEND_20060811
	
// 380 추가 옵션 적용 변수
typedef struct  
{
	short	OpAddAttackSuccessRatePVP;	// 대인 공격 성공률 상승
	short   OpAddDamage;			    // 대인 데미지증가 +%d
	short	OpAddDefenseSuccessRatePvP;	// 대인 방어 성공률 상승
	short	OpAddDefense;				// 대인 방어력 +%d
	short	OpAddMaxHP;					// 최대 HP 상승
	short	OpAddMaxSD;					// 최대 SD 상승
	short	OpRefillOn;					// SD자동 회복 On : 안전지대가 아니더라도 회복.
	short	OpAddRefillSD;				// SD자동 증가량 상승
} ITEMOPTION_FOR380ITEM_EFFECT, *LPITEMOPTION_FOR380ITEM_EFFECT;

class CItemSystemFor380  
{
public:
	CItemSystemFor380();
	virtual ~CItemSystemFor380();

private:
	// 380 추가 옵션 데이터
	ITEMOPTION_FOR380ITEM m_itemOption[MAX_ITEM];

	// 환경변수
	BOOL	m_bSystemFor380ItemOption;						// 시스템 On/Off
	int		m_iNeedJewelOfHarmonyCount;						// 조화의보석 수
	int		m_iNeedJewelOfSuhoCount;						// 수호의보석 수
	int		m_iNeedZenFor380Option;							// 필요젠
	int		m_iRateSuccessRateForMix1;						// 380 옵션 아이템 조합 확률. 1단계
	int		m_iRateSuccessRateForMix2;						// 380 옵션 아이템 조합 확률. 2단계
	int		m_iRateSuccessRateForMix3;						// 380 옵션 아이템 조합 확률. 3단계
	void	_InitOption();

public:
	BOOL	Load380ItemOptionInfo(char* filename);			// script load

	BOOL	Is380Item( const CItem* pItem );				// 380 아이템 확인
	BOOL	Is380OptionItem( const CItem* pItem );			// 380 옵션 붙은 아이템 확인

	void	InitEffectValue( LPITEMOPTION_FOR380ITEM_EFFECT pItemEffect );
	BOOL	ApplyFor380Option(LPOBJECTSTRUCT lpObj);		// 380 옵션 적용

private:
	BOOL	_CalcItemEffectValue(int iItemOptionType, int iItemEffectValue, LPITEMOPTION_FOR380ITEM_EFFECT pItemEffect);
	BOOL	_SetOption(CItem *pItem, BOOL bOption);			// 해당 아이템에 380 옵션 추가

public:
	void	SetOptionItemByMacro(LPOBJECTSTRUCT lpObj, BYTE invenrotyTargetPos, BOOL bOption);	// 매크로를 이용한 옵션추가

public:
	BOOL	ChaosMix380ItemOption(LPOBJECTSTRUCT lpObj);	// 380 아이템 옵션 조합
};


extern CItemSystemFor380	g_kItemSystemFor380;


inline void DebugLog(char *szlog, ...)
{
	char szBuffer[512]="";
	va_list		pArguments;

	va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);

	OutputDebugString( szBuffer );
}

#endif // ADD_380ITEM_NEWOPTION_20060711

#endif // !defined(AFX_ITEMSYSTEMFOR380_H__C69D017E_B1AA_4120_81D8_971216D676D6__INCLUDED_)
