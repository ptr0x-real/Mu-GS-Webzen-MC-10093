// CashLotterySystem.h: interface for the CCashLotterySystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASHLOTTERYSYSTEM_H__85C33899_B1CE_421E_93EC_800E35D5FDE5__INCLUDED_)
#define AFX_CASHLOTTERYSYSTEM_H__85C33899_B1CE_421E_93EC_800E35D5FDE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117

#include "TRandomPoolMgr.h"

//#define 0	확률값 입력부분
//#define 1	추가옵션 비중치 입력부분
//#define 2	엑설런트옵션 비중치 입력부분
//#define 3	무기류 1 (ex)
//#define 4	무기류 2
//#define 5	방패류 1 (ex)
//#define 6	방패류 2
//#define 7	방어구류 1(ex)
//#define 8	방어구류 2
//#define 9	액세서리류
//#define 10	보석류 1
//#define 11	보석류 2
//#define 12	보셕류 세트
//#define 13	기타 1
//#define 14	기타 2
//#define 15	희귀아이템티켓 1
//#define 16	희귀아이템티켓 2
//#define 17	희귀아이템티켓 3
//#define 18	희귀아이템티켓 4
//#define 19	희귀아이템티켓 5

enum
{
	LOTTERY_SCRIPT_CATEGORY_DROPRATE	= 0,
	LOTTERY_SCRIPT_CATEGORY_ADDOPTION	= 1,
	LOTTERY_SCRIPT_CATEGORY_EXOPTION	= 2,
	LOTTERY_SCRIPT_CATEGORY_WEAPON1		= 3,
	LOTTERY_SCRIPT_CATEGORY_WEAPON2		= 4,
	LOTTERY_SCRIPT_CATEGORY_SHIELD1		= 5,
	LOTTERY_SCRIPT_CATEGORY_SHIELD2		= 6,
	LOTTERY_SCRIPT_CATEGORY_DEFENSE1	= 7,
	LOTTERY_SCRIPT_CATEGORY_DEFENSE2	= 8,
	LOTTERY_SCRIPT_CATEGORY_ACCESSORIES	= 9,
	LOTTERY_SCRIPT_CATEGORY_JEWEL1		= 10,
	LOTTERY_SCRIPT_CATEGORY_JEWEL2		= 11,
	LOTTERY_SCRIPT_CATEGORY_JEWELSET	= 12,
	LOTTERY_SCRIPT_CATEGORY_ETC1		= 13,
	LOTTERY_SCRIPT_CATEGORY_ETC2		= 14,
	LOTTERY_SCRIPT_CATEGORY_TICKET1		= 15,
	LOTTERY_SCRIPT_CATEGORY_TICKET2		= 16,
	LOTTERY_SCRIPT_CATEGORY_TICKET3		= 17,
	LOTTERY_SCRIPT_CATEGORY_TICKET4		= 18,
	LOTTERY_SCRIPT_CATEGORY_TICKET5		= 19,
#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	LOTTERY_SCRIPT_CATEGORY_TICKET6		= 20,
	LOTTERY_SCRIPT_CATEGORY_WEAPON3		= 21,
	LOTTERY_SCRIPT_CATEGORY_WEAPON4		= 22,
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326	
	LOTTERY_SCRIPT_CATEGORY_MAX
};

enum
{
	LOTTERY_ITEM_CATEGORY_DROPRATE		= 0,
	LOTTERY_ITEM_CATEGORY_ADDOPTION		= 1,
	LOTTERY_ITEM_CATEGORY_EXOPTION		= 2,
	LOTTERY_ITEM_CATEGORY_WEAPON1		= 3,
	LOTTERY_ITEM_CATEGORY_WEAPON2		= 4,
	LOTTERY_ITEM_CATEGORY_SHIELD1		= 5,
	LOTTERY_ITEM_CATEGORY_SHIELD2		= 6,
	LOTTERY_ITEM_CATEGORY_DEFENSE1		= 7,
	LOTTERY_ITEM_CATEGORY_DEFENSE2		= 8,
	LOTTERY_ITEM_CATEGORY_ACCESSORIES	= 9,
	LOTTERY_ITEM_CATEGORY_JEWEL1		= 10,
	LOTTERY_ITEM_CATEGORY_JEWEL2		= 11,
	LOTTERY_ITEM_CATEGORY_JEWELSET		= 12,
	LOTTERY_ITEM_CATEGORY_ETC1			= 13,
	LOTTERY_ITEM_CATEGORY_ETC2			= 14,
	LOTTERY_ITEM_CATEGORY_TICKET1		= 15,
	LOTTERY_ITEM_CATEGORY_TICKET2		= 16,
	LOTTERY_ITEM_CATEGORY_TICKET3		= 17,
	LOTTERY_ITEM_CATEGORY_TICKET4		= 18,
	LOTTERY_ITEM_CATEGORY_TICKET5		= 19,
#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	LOTTERY_ITEM_CATEGORY_TICKET6		= 20,
	LOTTERY_ITEM_CATEGORY_WEAPON3		= 21,
	LOTTERY_ITEM_CATEGORY_WEAPON4		= 22,
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	LOTTERY_ITEM_CATEGORY_MAX
};

#define MAX_LOTTERY_ITEMLIST	130

#define MAX_LOTTERY_ITEM_HEIGHT	4
#define MAX_LOTTERY_ITEM_WIDTH	2

typedef struct
{
	WORD	wItemIndex;
	BYTE	btItemType;
	BYTE	btItemLevelMin;
	BYTE	btItemLevelMax;
	BYTE	btSkillOption;
	BYTE	btLuckOption;
	BYTE	btAddOption;
	BYTE	btExOption;
}LOTTERY_ITEMLIST, *LPLOTTERY_ITEMLIST;

typedef struct
{
	int		iSkillOptionRate;
	int		iLuckOptionRate;
	int		iAddOptionRate;
	int		iExOptionRate;
}LOTTERY_ITEM_OPTION_RATE, *LPLOTTERY_ITEM_OPTION_RATE;

class CCashLotterySystem  
{
public:
	CCashLotterySystem();
	virtual ~CCashLotterySystem();

	void Initialize();

	void Load(char* pchFilename);

	void SetitemDropRate(int iCategory, int iDropRate, int iSkillOptionRate, int iLuckOptionRate, int iAddOptionRate, int iExOptionRate);
	BOOL InsertItem(int iItemCategory, int iItemType, int iItemIndex, int iItemLevelMin, int iItemLevelMax, int iSkillOption, int iLuckOption, int iAddOption, int iExOption);

	int GetItem(CItem* lpItem);

	// 아이템 최대 크기 확인
	void GetRequireInvenSize(int* piHeight, int* piWidth);

#if TESTSERVER == 1
	void SimulationLotterySystem();
	void SaveStatistics(char* pchLog, ... );
#endif
private:
	TRandomPoolMgr CategoryRandomPool;
	TRandomPoolMgr AddOptionRandomPool;
	TRandomPoolMgr ExOptionRandomPool;

	int iItemMaxHeight;
	int iItemMaxWidth;

	int LotteryItemListCount[LOTTERY_ITEM_CATEGORY_MAX];
	LOTTERY_ITEM_OPTION_RATE LotteryItemOptionRate[LOTTERY_ITEM_CATEGORY_MAX];
	LOTTERY_ITEMLIST LotteryItemList[LOTTERY_ITEM_CATEGORY_MAX][MAX_LOTTERY_ITEMLIST];
};

#ifndef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326				// !! NOT
extern CCashLotterySystem g_CashLotterySystem;
extern BOOL g_bUseLotterySystem;
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#endif // !defined(AFX_CASHLOTTERYSYSTEM_H__85C33899_B1CE_421E_93EC_800E35D5FDE5__INCLUDED_)