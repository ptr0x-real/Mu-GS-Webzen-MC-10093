// JewelMixSystem.h: interface for the CJewelMixSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JEWELMIXSYSTEM_H__82324472_99D3_42A7_B4F7_2B469891AF85__INCLUDED_)
#define AFX_JEWELMIXSYSTEM_H__82324472_99D3_42A7_B4F7_2B469891AF85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\Common\\winutil.h"


#ifdef	JEWEL_MIX_SYSTEM_20050309

#define	MAX_JM_JEWEL_TYPE	2				// 최대 보석 종류
#define	MAX_JM_MIX_TYPE		3				// 최대 조합 종류

enum	{									// 보석 타입
	JEWEL_TYPE_BLESS		= 0,			// 축복의 보석
	JEWEL_TYPE_SOUL			= 1,			// 영혼의 보석
};

enum	{									// 조합 타입
	JEWEL_COUNT_10			= 0,			// 보석 조합 개수 - 10개
	JEWEL_COUNT_20			= 1,			// 보석 조합 개수 - 20개
	JEWEL_COUNT_30			= 2,			// 보석 조합 개수 - 30개
};

typedef struct {
	INT		m_iJewelCount;
	INT		m_iMixMoney;
	INT		m_iSourceType;
	INT		m_iChangeType;
} JEWELMIX_INFO, * LPJEWELMIX_INFO;

const JEWELMIX_INFO	g_JewelMixInfo[MAX_JM_JEWEL_TYPE][3]	= {		// 두번째 인자 - 각 인덱스마다 10, 20, 30 개 일때
	// 축복의 보석
	10,		500000,		MAKE_ITEMNUM(14,13),	MAKE_ITEMNUM(12,30),
	20,		1000000,	MAKE_ITEMNUM(14,13),	MAKE_ITEMNUM(12,30),
	30,		1500000,	MAKE_ITEMNUM(14,13),	MAKE_ITEMNUM(12,30),

	// 영혼의 보석
	10,		500000,		MAKE_ITEMNUM(14,14),	MAKE_ITEMNUM(12,31),
	20,		1000000,	MAKE_ITEMNUM(14,14),	MAKE_ITEMNUM(12,31),
	30,		1500000,	MAKE_ITEMNUM(14,14),	MAKE_ITEMNUM(12,31),
};

#define	JEWEL_UNMIX_MONEY				1000000						// 보석조합 해체 시 필요한 젠


class CJewelMixSystem  
{

public:
	CJewelMixSystem						();
	virtual ~CJewelMixSystem			();

	static INT	GetJewelCount			(
		INT iIndex,
		INT iJewelType
		);
	static INT	GetJewelCountPerLevel	(
		INT iJewelType,
		INT iJewelLevel
		);
	static BOOL	MixJewel				(
		INT iIndex,
		INT iJewelType,
		INT iMixType
		);
	static BOOL	UnMixJewel				(
		INT iIndex,
		INT iJewelType,
		INT iJewelLevel,
		INT iInventoryPos
		);
};


#endif

#endif // !defined(AFX_JEWELMIXSYSTEM_H__82324472_99D3_42A7_B4F7_2B469891AF85__INCLUDED_)
