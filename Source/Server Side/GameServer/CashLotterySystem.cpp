// CashLotterySystem.cpp: implementation of the CCashLotterySystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CashLotterySystem.h"

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117

#include "..\\Include\\Readscript.h"
#include "..\\common\\winutil.h"
#include "..\\common\\zzzitem.h"
#include "LargeRand.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326				// !! NOT
CCashLotterySystem g_CashLotterySystem;
BOOL g_bUseLotterySystem = TRUE;
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

extern ITEM_ATTRIBUTE ItemAttribute [MAX_ITEM];

CCashLotterySystem::CCashLotterySystem()
{
}

CCashLotterySystem::~CCashLotterySystem()
{

}

void CCashLotterySystem::Initialize()
{
	for( int i = 0 ; i < LOTTERY_ITEM_CATEGORY_MAX ; i++ )
	{
		LotteryItemListCount[i]						= 0;
		LotteryItemOptionRate[i].iSkillOptionRate	= 0;
		LotteryItemOptionRate[i].iLuckOptionRate	= 0;
		LotteryItemOptionRate[i].iAddOptionRate		= 0;
		LotteryItemOptionRate[i].iExOptionRate		= 0;
	}

	for( int i = 0 ; i < LOTTERY_ITEM_CATEGORY_MAX ; i++ )
	{
		for( int j = 0 ; j < MAX_LOTTERY_ITEMLIST ; j++ )
		{
			LotteryItemList[i][j].btItemType		= -1;
			LotteryItemList[i][j].wItemIndex		= -1;
			LotteryItemList[i][j].btItemLevelMin	= -1;
			LotteryItemList[i][j].btItemLevelMax	= -1;
			LotteryItemList[i][j].btSkillOption		= -1;
			LotteryItemList[i][j].btLuckOption		= -1;
			LotteryItemList[i][j].btAddOption		= -1;
			LotteryItemList[i][j].btExOption		= -1;
		}
	}

	CategoryRandomPool.InitPool();
	AddOptionRandomPool.InitPool();
	ExOptionRandomPool.InitPool();

	iItemMaxHeight = 0;
	iItemMaxWidth = 0;
}

void CCashLotterySystem::Load(char* pchFilename)
{
	// 0. 기존에 로드 되었던 데이터 초기화
	Initialize();

	if((SMDFile=fopen(pchFilename,"r")) == NULL)	
	{
		MsgBox("[CashLoterrySystem] Lottery Item List load failed. [%s]", pchFilename );
		return;
	}

	SMDToken Token;
	int iType = 0;

	int iCategory				= 0;
	int iDropRate				= 0;
	int iSkillOptionRate		= 0;
	int iLuckOptionRate			= 0;
	int iAddOptionRate			= 0;
	int iExOptionRate			= 0;

	int iItemAddOption		= 0;
	int iItemAddOptionRate	= 0;
	int iItemExOption		= 0;
	int iItemExOptionRate	= 0;

	int iItemType		= 0;
	int iItemIndex		= 0;
	int iItemLevelMin	= 0;
	int iItemLevelMax	= 0;
	int iSkillOption	= 0;
	int iLuckOption		= 0;
	int iAddOption		= 0;
	int iExOption		= 0;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		iType = (int)TokenNumber;
		while( true )
		{
			Token = (*GetToken)();
			if(Token == END) break;
			// 1. 복권 아이템 드롭 확률을 로드한다.
			// #define 0:무기류, 1:방패/방어구류, 2:액세서리류, 3:보석류, 4:기타
			if( iType == LOTTERY_SCRIPT_CATEGORY_DROPRATE )
			{
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iCategory				= (int)TokenNumber;
				Token = (*GetToken)();
				iDropRate				= (int)TokenNumber;
				Token = (*GetToken)();
				iSkillOptionRate		= (int)TokenNumber;
				Token = (*GetToken)();
				iLuckOptionRate			= (int)TokenNumber;
				Token = (*GetToken)();
				iAddOptionRate			= (int)TokenNumber;
				Token = (*GetToken)();
				iExOptionRate			= (int)TokenNumber;

				SetitemDropRate( iCategory - 3, iDropRate, iSkillOptionRate, iLuckOptionRate, iAddOptionRate, iExOptionRate );
			}

			// 2. AddOption 확률을 로드한다.
			if( iType == LOTTERY_SCRIPT_CATEGORY_ADDOPTION )
			{
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iItemAddOption		= (int)TokenNumber;
				Token = (*GetToken)();
				iItemAddOptionRate	= (int)TokenNumber;

				AddOptionRandomPool.AddValue( iItemAddOption, iItemAddOptionRate );
			}

			// 3. 엑셀런트 옵션 확률을 로드한다.
			if( iType == LOTTERY_SCRIPT_CATEGORY_EXOPTION )
			{
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iItemExOption		= (int)TokenNumber;
				Token = (*GetToken)();
				iItemExOptionRate	= (int)TokenNumber;

				ExOptionRandomPool.AddValue( iItemExOption, iItemExOptionRate );
			}

			// 3. 복권 아이템 리스트를 로드한다. (무기류)
			else if( iType >= LOTTERY_SCRIPT_CATEGORY_WEAPON1 && iType < LOTTERY_SCRIPT_CATEGORY_MAX )
			{
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				// 타입, 인덱스, 최소레벨, 최대레벨, 스킬, 행운, 에드옵션, 엑셀
				iItemType		= (int)TokenNumber;
				Token = (*GetToken)();
				iItemIndex		= (int)TokenNumber;
				Token = (*GetToken)();
				iItemLevelMin	= (int)TokenNumber;
				Token = (*GetToken)();
				iItemLevelMax	= (int)TokenNumber;
				Token = (*GetToken)();
				iSkillOption	= (int)TokenNumber;
				Token = (*GetToken)();
				iLuckOption		= (int)TokenNumber;
				Token = (*GetToken)();
				iAddOption		= (int)TokenNumber;
				Token = (*GetToken)();
				iExOption		= (int)TokenNumber;

				InsertItem( iType - 3, iItemType, iItemIndex, iItemLevelMin, iItemLevelMax, iSkillOption, iLuckOption, iAddOption, iExOption );
			}
		}
	}
	
	fclose(SMDFile);
}

void CCashLotterySystem::SetitemDropRate(int iCategory, int iDropRate, int iSkillOptionRate, int iLuckOptionRate, int iAddOptionRate, int iExOptionRate)
{
	CategoryRandomPool.AddValue( iCategory, iDropRate );

	LotteryItemOptionRate[iCategory].iSkillOptionRate	= iSkillOptionRate;
	LotteryItemOptionRate[iCategory].iLuckOptionRate	= iLuckOptionRate;
	LotteryItemOptionRate[iCategory].iAddOptionRate		= iAddOptionRate;
	LotteryItemOptionRate[iCategory].iExOptionRate		= iExOptionRate;
}

BOOL CCashLotterySystem::InsertItem(int iItemCategory, int iItemType, int iItemIndex, int iItemLevelMin, int iItemLevelMax, int iSkillOption, int iLuckOption, int iAddOption, int iExOption)
{
	if( iItemCategory < 0 || iItemCategory > LOTTERY_ITEM_CATEGORY_MAX )
	{
		return FALSE;
	}

	int iItemCount = LotteryItemListCount[iItemCategory];

	if( iItemCount < 0 || iItemCount > MAX_LOTTERY_ITEMLIST )
	{
		return FALSE;
	}

	LotteryItemList[iItemCategory][iItemCount].btItemType		= iItemType;
	LotteryItemList[iItemCategory][iItemCount].wItemIndex		= iItemIndex;
	LotteryItemList[iItemCategory][iItemCount].btItemLevelMin	= iItemLevelMin;
	LotteryItemList[iItemCategory][iItemCount].btItemLevelMax	= iItemLevelMax;
	LotteryItemList[iItemCategory][iItemCount].btSkillOption	= iSkillOption;
	LotteryItemList[iItemCategory][iItemCount].btLuckOption		= iLuckOption;
	LotteryItemList[iItemCategory][iItemCount].btAddOption		= iAddOption;
	LotteryItemList[iItemCategory][iItemCount].btExOption		= iExOption;

	int iItemCode = 0;
	int iItemHeight = 0;
	int iItemWidth = 0;

	iItemCode = MAKE_ITEMNUM( iItemType, iItemIndex );

	iItemHeight = ItemAttribute[iItemCode].Height;
	iItemWidth	= ItemAttribute[iItemCode].Width;

	if( iItemWidth > iItemMaxWidth )
	{
		iItemMaxWidth = iItemWidth;
	}

	if( iItemHeight > iItemMaxHeight )
	{
		iItemMaxHeight = iItemHeight;
	}

	LotteryItemListCount[iItemCategory]++;
	return TRUE;
}

int CCashLotterySystem::GetItem(CItem* lpItem)
{
	int iItemCategory = 0;
	int iMaxItemCountInCategory = 0;
	int iItemSelectNumber = 0;

	CItem ReturnItem;
	int iItemNumber		= 0;
	int iItemType		= 0;
	int iItemIndex		= 0;
	int iItemLevel		= 0;
	int iItemLevelMin	= 0;
	int iItemLevelMax	= 0;
	int iSkillOption	= 0;
	int iLuckOption		= 0;
	int iAddOption		= 0;
	int iExOption		= 0;

	int iSkillOptionRate	= 0;
	int iLuckOptionRate		= 0;
	int iAddOptionRate		= 0;
	int iExOptionRate		= 0;

	BYTE btSkillOption = 0;
	BYTE btLuckOption = 0;
	BYTE btAddOption = 0;
	BYTE btExOption = 0;
	BYTE btExOptionKey = 0;
	BYTE btExOptionValue1 = 0;
	BYTE btExOptionValue2 = 0;

	int iTemp = 0;

	int iRandomKey	= 0;

	if( lpItem == NULL )
	{
		return -1;
	}

	// 1. 가중치에 따른 아이템 카테고리를 얻어온다.
	iItemCategory = CategoryRandomPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );

	if( iItemCategory < 0 || iItemCategory > LOTTERY_ITEM_CATEGORY_MAX )
	{
		return -1;
	}

	iMaxItemCountInCategory = LotteryItemListCount[iItemCategory];

	if( iMaxItemCountInCategory <= 0 || iMaxItemCountInCategory > MAX_LOTTERY_ITEMLIST )
	{
		return -1;
	}

	// 2. 해당 아이템 카테고리 내에서 아이템을 렌덤으로 선택한다.
	iItemSelectNumber = rand() % iMaxItemCountInCategory;

	if( iItemSelectNumber < 0 || iItemSelectNumber > MAX_LOTTERY_ITEMLIST )
	{
		return -1;
	}

	// 3. 아이템을 지정한다.
	iItemType		= LotteryItemList[iItemCategory][iItemSelectNumber].btItemType;
	iItemIndex		= LotteryItemList[iItemCategory][iItemSelectNumber].wItemIndex;
	iItemLevelMin	= LotteryItemList[iItemCategory][iItemSelectNumber].btItemLevelMin;
	iItemLevelMax	= LotteryItemList[iItemCategory][iItemSelectNumber].btItemLevelMax;
	iSkillOption	= LotteryItemList[iItemCategory][iItemSelectNumber].btSkillOption;
	iLuckOption		= LotteryItemList[iItemCategory][iItemSelectNumber].btLuckOption;
	iAddOption		= LotteryItemList[iItemCategory][iItemSelectNumber].btAddOption;
	iExOption		= LotteryItemList[iItemCategory][iItemSelectNumber].btExOption;

#if TESTSERVER == 1
	LogAddTD("[DEBUG] Log - Type:%d, Index:%d, MinLevel:%d, MaxLevel:%d, Skill:%d, Luck:%d, Add:%d, Ex:%d",
		iItemType,
		iItemIndex,
		iItemLevelMin,
		iItemLevelMax,
		iSkillOption,
		iLuckOption,
		iAddOption,
		iExOption );
#endif

	if( iItemType == -1 || iItemIndex == -1 || iItemLevelMin == -1 || iItemLevelMax == -1 || iItemLevelMin == -1 || iItemLevelMax == -1 || iSkillOption == -1 || iLuckOption == -1 || iAddOption == -1 || iExOption == -1 )
	{
		return -1;
	}

	// 4. 아이템 옵션 확률에 따라 옵션을 정한다.
	// 4-1. 각 옵션의 지정 확률을 결정한다.
	iSkillOptionRate	= LotteryItemOptionRate[iItemCategory].iSkillOptionRate;
	iLuckOptionRate		= LotteryItemOptionRate[iItemCategory].iLuckOptionRate;
	iAddOptionRate		= LotteryItemOptionRate[iItemCategory].iAddOptionRate;
	iExOptionRate		= LotteryItemOptionRate[iItemCategory].iExOptionRate;
	// 4-1. 레벨 지정
	iRandomKey = rand() % ( ( iItemLevelMax - iItemLevelMin ) + 1 );
	iItemLevel = iItemLevelMin + iRandomKey;
	if( ( iItemLevel > iItemLevelMax ) || ( iItemLevel < iItemLevelMin ) )
	{
		iItemLevel = iItemLevelMin;
	}
	
	// 4-2. 스킬 옵션 지정
	if( iSkillOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;
		if( iRandomKey < iSkillOptionRate )
		{
			btSkillOption = 1;
		}
	}

	// 4-3. 행운 옵션 지정
	if( iLuckOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;
		if( iRandomKey < iLuckOptionRate )
		{
			btLuckOption = 1;
		}
	}

	// 4-4. 부가 옵션 지정
	if( iAddOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;
		if( iRandomKey < iAddOptionRate )
		{
			btAddOption = AddOptionRandomPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
			iTemp = btAddOption;
		}
	}

	// 4-5. 엑셀런트 옵션 지정
	// 4-5-1. 엑셀런트 옵션 값이 1이면 확률에 의해 옵션이 결정 되고, 실패시 레벨이 붙는다.
	if( iExOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;
		if( iRandomKey < iExOptionRate )
		{
			btExOptionValue1 = ExOptionRandomPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
			btExOptionKey = 1 << btExOptionValue1;
			btExOption |= btExOptionKey;

			// 레벨을 무조건 0으로 설정한다.
			iItemLevel = 0;

			if( rand() % 4 == 0 )
			{
				btExOptionValue2 = ExOptionRandomPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
				if( btExOptionValue1 != btExOptionValue2 )
				{
					btExOptionKey = 1 << btExOptionValue2;
					btExOption |= btExOptionKey;
				}
			}
		}
	}
	// 4-5-2. 엑셀런트 옵션 값이 2이면 100% 엑셀런트 옵션이 붙는다.
	else if( iExOption == 2 )
	{
		btExOptionValue1 = ExOptionRandomPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
		btExOptionKey = 1 << btExOptionValue1;
		btExOption |= btExOptionKey;

		// 레벨을 무조건 0으로 설정한다.
		iItemLevel = 0;

		if( rand() % 4 == 0 )
		{
			btExOptionValue2 = ExOptionRandomPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
			if( btExOptionValue1 != btExOptionValue2 )
			{
				btExOptionKey = 1 << btExOptionValue2;
				btExOption |= btExOptionKey;
			}
		}
	}

	if( btAddOption != iTemp )
	{
		btAddOption = 0;
	}

	iItemNumber = ItemGetNumberMake( iItemType, iItemIndex);
	lpItem->Convert( iItemNumber, btSkillOption, btLuckOption, btAddOption, btExOption, 0, 0 );
	lpItem->m_Level = iItemLevel;

	if( lpItem->IsItem() == FALSE )
	{
		return -1;
	}

	BYTE ExOption[8];
	ItemIsBufExOption( ExOption, lpItem );

	LogAddTD("[CashItem][LotteryItem] Get Item Category:%d,Index:%d (Name:%s,Type:%d,Index:%d,Level:%d) Skill:%d,Luck:%d,AddOption:%d,ExOption(%d,%d,%d,%d,%d,%d)",
		iItemCategory,
		iItemSelectNumber,
		ItemAttribute[iItemNumber].Name,
		iItemType,
		iItemIndex,
		iItemLevel,
		btSkillOption,
		btLuckOption,
		btAddOption,
		ExOption[0],
		ExOption[1],
		ExOption[2],
		ExOption[3],
		ExOption[4],
		ExOption[5]
		);

	return iItemCategory;
}

void CCashLotterySystem::GetRequireInvenSize(int* piHeight, int* piWidth)
{
	*piHeight	= iItemMaxHeight;
	*piWidth	= iItemMaxWidth;
}

#if TESTSERVER == 1
void CCashLotterySystem::SimulationLotterySystem()
{

	return;

	CItem pItem;
	int iTotalSimulationCount = 100000;
	int iFailed = 0;
	int iSuccess = 0;
	int iLevel[8] = {0};
	int iRetCategory = 0;
	int iCategory[LOTTERY_ITEM_CATEGORY_MAX] = {0};
	int iCategoryLuck[LOTTERY_ITEM_CATEGORY_MAX] = {0};
	int iCategorySkill[LOTTERY_ITEM_CATEGORY_MAX] = {0};
	int iCategoryAddOption[LOTTERY_ITEM_CATEGORY_MAX] = {0};
	int iCategoryEx[LOTTERY_ITEM_CATEGORY_MAX] = {0};

	for( int i = 0 ; i < iTotalSimulationCount ; i++ )
	{
		iRetCategory = GetItem( &pItem );
		if( iRetCategory == -1 )
		{
			iFailed++;
			continue;
		}
		else
		{
			iSuccess++;
		}

		iLevel[pItem.m_Level]++;
		iCategory[iRetCategory]++;
		if( pItem.m_Option1 != 0 )
		{
			iCategorySkill[iRetCategory]++;
		}

		if( pItem.m_Option2 != 0 )
		{
			iCategoryLuck[iRetCategory]++;
		}

		if( pItem.m_Option3 != 0 )
		{
			iCategoryAddOption[iRetCategory]++;
		}

		if( pItem.m_NewOption != 0 )
		{
			iCategoryEx[iRetCategory]++;
		}
	}

	SaveStatistics( "[ChaosCardMix][Statistics] Begin - Total SimulationCount : %d, Success:%d/Failed:%d",
		iTotalSimulationCount,
		iSuccess,
		iFailed );

	SaveStatistics( "[ChoasCardMix][Statistics] Level Distribute (0:%d) / (1:%d) / (2:%d) / (3:%d) / (4:%d) / (5:%d) / (6:%d) / (7:%d)",
		iLevel[0],
		iLevel[1],
		iLevel[2],
		iLevel[3],
		iLevel[4],
		iLevel[5],
		iLevel[6],
		iLevel[7] );

	SaveStatistics( "[ChoasCardMix][Statistics] Category Distribute (3:%d)/(4:%d)/(5:%d)/(6:%d)/(7:%d)/(8:%d)/(9:%d)/(10:%d)/(11:%d)/(12:%d)/(13:%d)/(14:%d)/(15:%d)/(16:%d)/(17:%d)/(18:%d)",
		iCategory[0],
		iCategory[1],
		iCategory[2],
		iCategory[3],
		iCategory[4],
		iCategory[5],
		iCategory[6],
		iCategory[7],
		iCategory[8],
		iCategory[9],
		iCategory[10],
		iCategory[11],
		iCategory[12],
		iCategory[13],
		iCategory[14],
		iCategory[15] );

	SaveStatistics( "[ChoasCardMix][Statistics] Skill Distribute (3:%d)/(4:%d)/(5:%d)/(6:%d)/(7:%d)/(8:%d)/(9:%d)/(10:%d)/(11:%d)/(12:%d)/(13:%d)/(14:%d)/(15:%d)/(16:%d)/(17:%d)/(18:%d)",
		iCategorySkill[0],
		iCategorySkill[1],
		iCategorySkill[2],
		iCategorySkill[3],
		iCategorySkill[4],
		iCategorySkill[5],
		iCategorySkill[6],
		iCategorySkill[7],
		iCategorySkill[8],
		iCategorySkill[9],
		iCategorySkill[10],
		iCategorySkill[11],
		iCategorySkill[12],
		iCategorySkill[13],
		iCategorySkill[14],
		iCategorySkill[15] );

	SaveStatistics( "[ChoasCardMix][Statistics] Luck Distribute (3:%d)/(4:%d)/(5:%d)/(6:%d)/(7:%d)/(8:%d)/(9:%d)/(10:%d)/(11:%d)/(12:%d)/(13:%d)/(14:%d)/(15:%d)/(16:%d)/(17:%d)/(18:%d)",
		iCategoryLuck[0],
		iCategoryLuck[1],
		iCategoryLuck[2],
		iCategoryLuck[3],
		iCategoryLuck[4],
		iCategoryLuck[5],
		iCategoryLuck[6],
		iCategoryLuck[7],
		iCategoryLuck[8],
		iCategoryLuck[9],
		iCategoryLuck[10],
		iCategoryLuck[11],
		iCategoryLuck[12],
		iCategoryLuck[13],
		iCategoryLuck[14] );

	SaveStatistics( "[ChoasCardMix][Statistics] AddOption Distribute (3:%d)/(4:%d)/(5:%d)/(6:%d)/(7:%d)/(8:%d)/(9:%d)/(10:%d)/(11:%d)/(12:%d)/(13:%d)/(14:%d)/(15:%d)/(16:%d)/(17:%d)/(18:%d)",
		iCategoryAddOption[0],
		iCategoryAddOption[1],
		iCategoryAddOption[2],
		iCategoryAddOption[3],
		iCategoryAddOption[4],
		iCategoryAddOption[5],
		iCategoryAddOption[6],
		iCategoryAddOption[7],
		iCategoryAddOption[8],
		iCategoryAddOption[9],
		iCategoryAddOption[10],
		iCategoryAddOption[11],
		iCategoryAddOption[12],
		iCategoryAddOption[13],
		iCategoryAddOption[14] );

	SaveStatistics( "[ChoasCardMix][Statistics] ExOption Distribute (3:%d)/(4:%d)/(5:%d)/(6:%d)/(7:%d)/(8:%d)/(9:%d)/(10:%d)/(11:%d)/(12:%d)/(13:%d)/(14:%d)/(15:%d)/(16:%d)/(17:%d)/(18:%d)",
		iCategoryEx[0],
		iCategoryEx[1],
		iCategoryEx[2],
		iCategoryEx[3],
		iCategoryEx[4],
		iCategoryEx[5],
		iCategoryEx[6],
		iCategoryEx[7],
		iCategoryEx[8],
		iCategoryEx[9],
		iCategoryEx[10],
		iCategoryEx[11],
		iCategoryEx[12],
		iCategoryEx[13],
		iCategoryEx[14] );

	SaveStatistics( "[ChoasCardMix][Statistics] End" );

}

void CCashLotterySystem::SaveStatistics(char* pchLog, ... )
{
	char szBuffer[512]="";
	va_list		pArguments;
	static FILE *	logfp=NULL;

	logfp = fopen("ChaosCardMixStatistics.txt", "a+t");
	if( logfp == NULL )
	{
		return;
	}

	va_start(pArguments, pchLog);
    vsprintf(szBuffer, pchLog, pArguments);
    va_end(pArguments);

	fprintf(logfp, "%s\n",szBuffer);

	fclose( logfp );
}

#endif

#endif // ADD_PCS_LOTTERY_SYSTEM_20070117