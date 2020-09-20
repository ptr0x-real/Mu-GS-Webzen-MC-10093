// EventItemBagExMng.cpp: implementation of the CEventItemBagExMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventItemBagExMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

CEventItemBagExMng g_EventItemBagExMng;
bool g_bUseLotterySystem = FALSE;

CEventItemBagExMng::CEventItemBagExMng()
{
	Initialize();
}

CEventItemBagExMng::~CEventItemBagExMng()
{

}

void CEventItemBagExMng::Initialize()
{
	for( int i = 0 ; i < MAX_ITEMBAGEX_SIZE ; i++ )
	{
		ItemBagExList[i].bIsUsed = false;
		ItemBagExList[i].ItembagName[0] = '\0';
		ItemBagExList[i].lpItembag = NULL;
	}
}

bool CEventItemBagExMng::AddEventItemBagEx(char* pszItemBagName, char* pszItemBagFileName)
{
	CCashLotterySystem* lpItemBag = NULL;

	for( int i = 0 ; i < MAX_ITEMBAGEX_SIZE ; i++ )
	{
		if( ItemBagExList[i].bIsUsed == false )
		{
			ItemBagExList[i].bIsUsed = true;
			strcpy( ItemBagExList[i].ItembagName, pszItemBagName );
			lpItemBag = new CCashLotterySystem();

			if( lpItemBag == NULL )
			{
				// ItemBag 생성 실패
				return false;
			}

			lpItemBag->Load( pszItemBagFileName );
			ItemBagExList[i].lpItembag = lpItemBag;

			return true;
		}
	}

	return false;
}

CCashLotterySystem* CEventItemBagExMng::GetItemBag(char* pszItemBagName)
{
	for( int i = 0 ; i < MAX_ITEMBAGEX_SIZE ; i++ )
	{
		if( ItemBagExList[i].bIsUsed == false )
		{
			continue;
		}

		if( strcmp( ItemBagExList[i].ItembagName, pszItemBagName ) == 0 )
		{
			return ItemBagExList[i].lpItembag;
		}
	}

	return NULL;
}

int CEventItemBagExMng::GetItem(char* pszItemBagName, CItem* lpItemInfo)
{
	CCashLotterySystem* lpItemList = NULL;

	lpItemList = GetItemBag( pszItemBagName );
	if( lpItemList == NULL )
	{
		return false;
	}

	lpItemList->GetItem( lpItemInfo );

	if( lpItemInfo == NULL )
	{
		return false;
	}

	return true;
}

#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326