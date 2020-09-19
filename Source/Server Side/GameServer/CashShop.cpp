// CashShop.cpp: implementation of the CCashShop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CashShop.h"

#ifdef PREMIUMITEM_20090106
#include "..\\common\winutil.h"
#endif

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#include "Include\Readscript.h"
#include "Common\zzzitem.h"
#include "..\\common\winutil.h"
#include "DirPath.h"
#include "DSProtocol.h"

#include "Gamemain.h"
#include "GameServer.h"

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#include "CashItemPeriodSystem.h"
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef MODIFY_RECONNECTSHOPSERVER_20080807
#include "iprtrmib.h"
#include "iphlpapi.h"
#pragma comment(lib, "Iphlpapi.lib")
#endif // MODIFY_RECONNECTSHOPSERVER_20080807

extern CDirPath gDirPath;
extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];

CCashShop g_CashShop;

BOOL g_bUseCashShop = FALSE;
BOOL g_bConnectShopServer = FALSE;
BOOL g_bShopServerConnectState = FALSE;
BOOL g_bUseLotteryEvent = FALSE;
char g_ShopServerIP[16] = "";
int	g_ShopserverPort = SHOPSERVER_DEFAULT_PORT;
BOOL g_bUseMoveMapBound = FALSE;

wsShopServerCli g_ShopServerClient;

WNDPROC CCashShop::m_lpOldProc = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void SGAnsCashPoint(MSG_STOG_USER_CASH_ANS* aRecv);
void SGAnsCashItemList(MSG_STOG_ITEM_LIST_ANS* aRecv);
void SGAnsPackageItemList(MSG_STOG_PACKAGE_LIST_ANS* aRecv);
void SGAnsBuyCashItem(MSG_STOG_BUY_ITEM_ANS* aRecv);

#ifdef MODIFY_RECONNECTSHOPSERVER_20080807
string GetMIBTcpState(DWORD dwState);
void GetTCPTableInfo();
#endif // MODIFY_RECONNECTSHOPSERVER_20080807

void ShopServerProtocolCore(DWORD protoNum, BYTE *aRecv, int aLen)
{
	switch( protoNum )
	{
	case STOG_USER_CASH_ANS:
		SGAnsCashPoint( (MSG_STOG_USER_CASH_ANS *)aRecv );
		break;
	case STOG_ITEM_LIST_ANS:
		SGAnsCashItemList( (MSG_STOG_ITEM_LIST_ANS *)aRecv );
		break;
	case STOG_PACKAGE_LIST_ANS:
		SGAnsPackageItemList( (MSG_STOG_PACKAGE_LIST_ANS *)aRecv );
		break;
	case STOG_BRANCH_ITEM_LIST_ANS:
		SGAnsBranchItemList( (MSG_STOG_BRANCH_ITEM_LIST_ANS *)aRecv );
		break;
	case STOG_RELOAD_SHOP_DATA_NTF:
		g_CashShop.CashShopOptioNReload();
		break;
	case STOG_BUY_ITEM_ANS:
		SGAnsBuyCashItem( (MSG_STOG_BUY_ITEM_ANS *)aRecv );
		break;
	}
}

CCashShop::CCashShop()
{

}

CCashShop::~CCashShop()
{

}

void CCashShop::Initialize()
{
	int i = 0;
	int j = 0;
	int k = 0;

	iCashItemCount = 0;
	iCashItemPageNumber = 0;
	bCashItemListReload = FALSE;

	iBranchItemCount = 0;

	MapUserObject.clear();
	MapCashItemList.clear();
	MapCashItemStatus.clear();

	for( i = 0 ; i < MAX_CASHITEM_LIST_CATEGORY ; i++ )
	{
		iCashItemCountInCategory[i] = 0;
	}

	for( i = 0 ; i < MAX_CASHITEM_LIST_CATEGORY ; i++ )
	{
		for( j = 0 ; j < MAX_CASHITEM_LIST_PAGE ; j++ )
		{
			CashItemProtocol[i][j].btItemCount		= 0;
			CashItemProtocol[i][j].btCategoryIndex	= 0;
			CashItemProtocol[i][j].btPageIndex		= 0;
			for( k = 0 ; k < MAX_CASHITEM_LIST_PER_PAGE ; k++ )
			{
				// 캐쉬 아이템 리스트 프로토콜 초기화
				CashItemProtocol[i][j].ItemInfo[k].dwItemGuid		= 0;
				CashItemProtocol[i][j].ItemInfo[k].btCategoryCode	= 0;
				CashItemProtocol[i][j].ItemInfo[k].btItemSaleRatio	= 0;
				CashItemProtocol[i][j].ItemInfo[k].wItemPrice		= 0;
				memset( CashItemProtocol[i][j].ItemInfo[k].btItemInfo, 0, ITEM_BUFFER_SIZE );
			}
		}
	}

	for( i = 0 ; i < MAX_CASHITEM_LIST_CATEGORY ; i++ )
	{
		for( j = 0 ; j < MAX_CASHITEM_LIST_IN_CATEGORY ; j++ )
		{
			// 캐쉬 아이템 리스트 초기화
			CashItemList[i][j].dwItemGuid		= 0;
			CashItemList[i][j].dwPriceGuid		= 0;
			CashItemList[i][j].btCategoryCode	= 0;
			CashItemList[i][j].btItemType		= 0;
			CashItemList[i][j].wItemIndex		= 0;
			CashItemList[i][j].btItemLevel		= 0;
			CashItemList[i][j].btItemDuration	= 0;
			CashItemList[i][j].btItemLuckOption	= 0;
			CashItemList[i][j].btItemSkillOpion	= 0;
			CashItemList[i][j].btItemAddOption	= 0;
			CashItemList[i][j].btItemSaleRatio	= 0;
			CashItemList[i][j].wItemPrice		= 0;
			memset( CashItemList[i][j].btItemInfo, 0, ITEM_BUFFER_SIZE );
		}
	}

	for( i = 0 ; i < MAX_CASHITEM_LIST_BRANCH ; i++ )
	{
		BranchItemList[i].dwItemGuid	= 0;
		BranchItemList[i].iBranchType	= -1;
	}
}

void CCashShop::CashShopOptioNReload()
{
	bCashItemListReload = TRUE;
	
	LogAddTD("[CashShop][OptionLoad] - CashShop Option Load Begin");

	LoadShopOption( gDirPath.GetNewPath("CashShopOption.dat") );
	Load( gDirPath.GetNewPath("CashShopList.txt") );
	// cashshop 연결 끊어지는 문제 by natinda
	//ReConnectShopServer();
	GSReqBranchItemList();
	GSReqCashItemList();
	GSReqPackageItemList();

	LogAddTD("[CashShop][OptionLoad] - CashShop Option Load finish");

	bCashItemListReload = FALSE;
}

void CCashShop::Load(char* pchFilename)
{
	if((SMDFile=fopen(pchFilename,"r")) == NULL)	
	{
		MsgBox("[CashItemList] Cash Item List load failed. [%s]", pchFilename );
		return;
	}

	// 리스트 초기화
	Initialize();

	SMDToken Token;

	int iType = 0;

	int	iItemCode			= 0;
	BYTE btItemType			= 0;
	WORD wItemIndex			= 0;
	BYTE btItemLevel		= 0;
	BYTE btItemSkillOpion	= 0;
	BYTE btItemLuckOption	= 0;
	BYTE btItemAddOption	= 0;
	BYTE btItemExOption		= 0;

	BYTE btItemX			= 0;
	BYTE btItemY			= 0;
	BYTE btItemScale		= 0;
	CASHSHOP_ITEM_STATUS ItemStatus;

	BOOL bResult			= FALSE;

	while( true )
	{
		// 0. 캐쉬 샵 정보를 로드한다.
		// 0-1. 캐쉬샵 On/Off 정보
		// 1. 캐쉬 아이템 리스트를 로드한다.
		Token = (*GetToken)();
		if(Token == END) break;

		iType = (int)TokenNumber;
		while( true )
		{
			Token = (*GetToken)();
			if(Token == END) break;

			if( iType == CASHSHOP_SCRIPT_SERVEROPTION )
			{
			}

			// 1-1. 캐쉬 아이템 리스트
			if( iType == CASHSHOP_SCRIPT_ITEMLIST )
			{
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				ItemStatus.btItemType		= (int)TokenNumber;
				Token = (*GetToken)();
				ItemStatus.wItemIndex		= (int)TokenNumber;
				Token = (*GetToken)();
				ItemStatus.btItemLevel		= (int)TokenNumber;
				Token = (*GetToken)();
				ItemStatus.btSkillOption	= (int)TokenNumber;
				Token = (*GetToken)();
				ItemStatus.btLuckOption		= (int)TokenNumber;
				Token = (*GetToken)();
				ItemStatus.btAddOption		= (int)TokenNumber;

				BYTE btExOptionValue	= 0;
				BYTE btExOption			= 0;
				Token = (*GetToken)();
				btItemExOption				= (int)TokenNumber;
				if( btItemExOption > 0 )
				{
					btExOptionValue			= 1 << (int)(btItemExOption - 1);
					btExOption				|= btExOptionValue;
					ItemStatus.btExOption	= btExOption;
				}
				else
				{
					ItemStatus.btExOption	= 0;
				}

				Token = (*GetToken)();
				btItemX						= (int)TokenNumber;		// X크기
				Token = (*GetToken)();
				btItemY						= (int)TokenNumber;		// Y크기
				Token = (*GetToken)();
				btItemScale					= (int)TokenNumber;		// 크기

				iItemCode = MAKE_ITEMNUM( ItemStatus.btItemType, ItemStatus.wItemIndex );

				ItemStatus.btDurability		= ItemAttribute[iItemCode].Durability;

				ItemByteConvert( ItemStatus.btItemInfo, iItemCode, ItemStatus.btSkillOption, ItemStatus.btLuckOption, ItemStatus.btAddOption, ItemStatus.btItemLevel, 1, ItemStatus.btExOption );

				LogAddTD("[CashShop][Load Cash Item List] - Add List - (%d/%d) Level:%d, Dur:%d, Skill:%d, Luck:%d, Add:%d, Ex:%d, X:%d, Y:%d, Scale:%d",
					ItemStatus.btItemType,
					ItemStatus.wItemIndex,
					ItemStatus.btItemLevel,
					ItemStatus.btDurability,
					ItemStatus.btSkillOption,
					ItemStatus.btLuckOption,
					ItemStatus.btAddOption,
					ItemStatus.btExOption,
					btItemX,
					btItemY,
					btItemScale);

				InsertItemStatus( &ItemStatus );
			}
		}
	}

	fclose(SMDFile);

	LogAddTD("[CashShop][Load Cash Item List] - Complete! - Total:%d",
		MapCashItemStatus.size()
		);

	// 3. 사용자에게 보낼 아이템 리스트 프로토콜을 미리 만들어 놓는다.

	// 4. 샵 서버와의 연결을 시도한다.
	if( g_bConnectShopServer == TRUE && g_bShopServerConnectState == FALSE )
	{
		g_bShopServerConnectState = ConnectShopServer( g_ShopServerIP, g_ShopserverPort );
		if( g_bShopServerConnectState == FALSE )
		{
			MsgBox("[CashShop] Shop Server Connect Failed!!");
		}
	}
}

void CCashShop::LoadShopOption(char* pchFilename)
{
	// 1. 샵 서버의 옵션을 로드한다. (CashShopOption.dat)
	LogAddTD("[CashShop][ShopOption] - Shop Option Reload '%s'", pchFilename );
	g_bUseCashShop			= GetPrivateProfileInt("CashShopOption", "UseCashShop",			0, pchFilename );
	g_bConnectShopServer	= GetPrivateProfileInt("CashShopOption", "ConnectShopServer",	0, pchFilename );
	g_bUseLotteryEvent		= GetPrivateProfileInt("CashShopOption", "UseLotteryEvent",		0, pchFilename );
	GetPrivateProfileString("CashShopOption", "ShopServerIP", "0", g_ShopServerIP, 16, pchFilename );
	g_ShopserverPort		= GetPrivateProfileInt("CashShopOption", "ShopServerPort",		0, pchFilename );
	g_bUseMoveMapBound		= GetPrivateProfileInt("CashShopOption", "MapMoveBoundCheck",	0, pchFilename );
}

void CCashShop::LoadTestScript(char* pchFilename)
{
	if((SMDFile=fopen(pchFilename,"r")) == NULL)	
	{
		MsgBox("[CashItemList] Cash Item List load failed. [%s]", pchFilename );
		return;
	}

	SMDToken Token;
	int iType = 0;

	int iGuid = 0;
	int iCategory = 0;
	BYTE btItemType = 0;
	WORD wItemIndex = 0;
	BYTE btSaleRatio = 0;
	BYTE btDuration = 0;
	WORD wPrice = 0;

	int iItemInfoCount = 1;

	MSG_STOG_ITEM_LIST_ANS pMsg;

	while( true )
	{
		// 0. 캐쉬 샵 정보를 로드한다.
		// 0-1. 캐쉬샵 On/Off 정보
		Token = (*GetToken)();
		if(Token == END) break;

		iType = (int)TokenNumber;
		while( true )
		{
			// 1-1. 캐쉬 아이템 리스트
			Token = (*GetToken)();
			if(Token == END) break;

			if( iType == 1 )
			{
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iGuid		= (int)TokenNumber;
				iGuid		= iItemInfoCount;
				Token = (*GetToken)();
				iCategory	= (int)TokenNumber;
				Token = (*GetToken)();
				btItemType	= (int)TokenNumber;
				Token = (*GetToken)();
				wItemIndex	= (int)TokenNumber;
				Token = (*GetToken)();
				btSaleRatio	= (int)TokenNumber;
				Token = (*GetToken)();
				btDuration	= (int)TokenNumber;
				Token = (*GetToken)();
				wPrice		= (int)TokenNumber;

				pMsg.dwItemCount = 1;
				pMsg.sellItems[0].dwItemGuid		= iItemInfoCount;
				pMsg.sellItems[0].dwCategoryID		= iCategory;
				pMsg.sellItems[0].dwItemCODE		= MAKE_ITEMNUM( btItemType, wItemIndex );
				pMsg.sellItems[0].dwCoolTime		= 0;
				pMsg.sellItems[0].dwBuyType			= 1;
				pMsg.sellItems[0].dwLimitSellCount	= 100;
				pMsg.sellItems[0].dwPriceCount		= 1;
				pMsg.sellItems[0].dwState			= 1;
				pMsg.sellItems[0].dwUseType			= 2;
				pMsg.sellItems[0].itemPrice[0].dwItemGuid	= iItemInfoCount;
				pMsg.sellItems[0].itemPrice[0].dwPrice		= wPrice;
				pMsg.sellItems[0].itemPrice[0].dwPriceGuid	= iGuid;
				pMsg.sellItems[0].itemPrice[0].dwSellRate	= btSaleRatio;
				pMsg.sellItems[0].itemPrice[0].dwUseTime	= 1;
				pMsg.sellItems[0].itemPrice[0].dwAmount		= btDuration;

				SetItemInfoFromShop( &pMsg );

				MakeItemListProtocol();

				iItemInfoCount++;
			}
		}
	}

	fclose(SMDFile);
}

BOOL CCashShop::ConnectShopServer(char* pchIpAddress, int iPortNumber)
{
	if( g_ShopServerClient.CreateSocket( ghWnd ) == FALSE )
	{
		return FALSE;
	}

	if( g_ShopServerClient.SetProtocolCore( ShopServerProtocolCore ) == FALSE )
	{
		return FALSE;
	}

	CCashShop::m_lpOldProc = (WNDPROC) SetWindowLong (ghWnd, GWL_WNDPROC, (LONG) CCashShop::ParentWndProc);

	if( g_ShopServerClient.Connect( pchIpAddress, iPortNumber, WM_ASS_SHOPSERVERMSG ) == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCashShop::ReConnectShopServer()
{
	g_ShopServerClient.Close();

	LogAddC(LOGC_RED, "[CashShop][ShopServer] - ReConnectShopServer");

	if( g_ShopServerClient.CreateSocket( ghWnd ) == FALSE )
	{
		return FALSE;
	}

	if( g_ShopServerClient.SetProtocolCore( ShopServerProtocolCore ) == FALSE )
	{
		return FALSE;
	}

	if( g_ShopServerClient.Connect( g_ShopServerIP, g_ShopserverPort, WM_ASS_SHOPSERVERMSG ) == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}

LRESULT CCashShop::ParentWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch( iMessage )
	{
	case WM_ASS_SHOPSERVERMSG:
		switch (LOWORD (lParam))
		{
		case FD_WRITE:
			break;
		case FD_READ:
			g_ShopServerClient.DataRecv();
			return 0;
		case FD_CONNECT:
			LogAddTD("[ShopServer] - Connect To Shop Server!!");
			g_bShopServerConnectState = TRUE;
			return 0;
		case FD_CLOSE:
#ifdef MODIFY_RECONNECTSHOPSERVER_20080807	
			GetTCPTableInfo();
			g_ShopServerClient.Close(g_ShopServerClient.GetSocket());
#else
			g_ShopServerClient.Close();
#endif // MODIFY_RECONNECTSHOPSERVER_20080807
			LogAddC( LOGC_RED, "[ShopServer] - Disconnect From Shop Server!!");
			g_bShopServerConnectState = FALSE;
			return 0;
		}
		return 0;
	}

	return CallWindowProc(CCashShop::m_lpOldProc, hWnd, iMessage, wParam, lParam);
}

void CCashShop::CheckShopServerConnectState()
{
	if( g_bConnectShopServer == FALSE )
	{
		return;
	}

	if( GetTickCount() - dwCheckShopServerConnectStatePeriod > ( 10 * 1000 ) )
	{
		dwCheckShopServerConnectStatePeriod = GetTickCount();
		if( g_bShopServerConnectState == FALSE )
		{
			g_bShopServerConnectState = ReConnectShopServer();
			if( g_bShopServerConnectState == FALSE )
			{
				LogAddC(LOGC_RED, "[CashShop][ShopServer] - Disconnect From Shop Server!!");
			}
		}
	}
}

BOOL CCashShop::InsertItemStatus(LPCASHSHOP_ITEM_STATUS lpItemStatus)
{
	int iItemCode = 0;
	LPCASHSHOP_ITEM_STATUS lpNewItemStatus = new CASHSHOP_ITEM_STATUS;

	memcpy( lpNewItemStatus, lpItemStatus, sizeof( CASHSHOP_ITEM_STATUS ) );

	iItemCode = MAKE_ITEMNUM( lpItemStatus->btItemType, lpItemStatus->wItemIndex );

	MapCashItemStatus.insert( pair<int, LPCASHSHOP_ITEM_STATUS> (iItemCode, lpNewItemStatus ) );

	return TRUE;
}

BOOL CCashShop::SetItemInfoFromShop(MSG_STOG_ITEM_LIST_ANS* lpMsg)
{
	int iItemCount = 0;

	// 1. 샵 아이템의 아이템 리스트를 받고, 아이템 개수를 확인한다.
	iItemCount = lpMsg->dwItemCount;

	// 1-1. 아이템 개수가 0이면 잘못된 데이터
	if( iItemCount == 0 )
	{
		return FALSE;
	}

	int iItemCode = 0;
	int iItemPriceCount = 0;
	LPCASHSHOP_ITEM_STATUS lpItemStatus = NULL;

	// 2. 샵으로 부터 받은 아이템 정보로 판매 아이템의 정보를 설정한다.
	for( int i = 0 ; i < iItemCount ; i++ )
	{
		// 2-1. CheckValidItemInfo로 받은 아이템의 정보가 정확한지 검사한다.
		if( CheckValidItemInfo( &lpMsg->sellItems[i] ) == FALSE )
		{
			LogAddTD("[CashShop] Invalid Item Info from ShopServer (GUID:%d,Category:%d,ItemCode:%d,Price:%d)",
				lpMsg->sellItems[i].dwItemGuid, lpMsg->sellItems[i].dwCategoryID, lpMsg->sellItems[i].dwItemCODE, lpMsg->sellItems[i].itemPrice[0].dwPrice );
			continue;
		}

		// 2-2. 해당 아이템의 속성을 가져온다.
		iItemCode = lpMsg->sellItems[i].dwItemCODE;
		lpItemStatus = GetCashItemStatus( iItemCode );
		if( lpItemStatus == NULL )
		{
			LogAddTD("[CashShop] Can not Find Item Status (Code : %d)", iItemCode );
			continue;
		}

		// 2-3. 아이템의 가격 개수만큼 아이템 리스트를 생성한다.
		// 2-3-1. 아이템 리스트에 넣을 아이템 리스트를 생성한다.
		MakeItemList( lpItemStatus, &lpMsg->sellItems[i] );
		// 2-3-2. 아이템 리스트에 아이템을 추가한다. (속성+가격)
	}

	return TRUE;
}

BOOL CCashShop::SetPackageItemInfoFromShop(MSG_STOG_PACKAGE_LIST_ANS* lpMsg)
{
	return TRUE;
}

BOOL CCashShop::CheckValidItemInfo(tblSellItem* lpItemInfo)
{
	if( lpItemInfo->dwItemGuid < 0 )
	{
		return FALSE;
	}

	if( lpItemInfo->dwPriceCount <= 0 || lpItemInfo->dwPriceCount > MAX_ITEM_PRICE_SIZE )
	{
		return FALSE;
	}

	if( lpItemInfo->dwCategoryID < 0 || lpItemInfo->dwCategoryID > MAX_CASHITEM_LIST_CATEGORY )
	{
		lpItemInfo->dwCategoryID = 0;
		return FALSE;
	}

	return TRUE;
}

LPCASHSHOP_ITEM_STATUS CCashShop::GetCashItemStatus(int iItemCode)
{
	LPCASHSHOP_ITEM_STATUS lpReturn = NULL;
	ITER_CASHITEM_STATUS Iter;

	Iter = MapCashItemStatus.find( iItemCode );
	if( Iter == MapCashItemStatus.end() )
	{
		return NULL;
	}

	lpReturn = (LPCASHSHOP_ITEM_STATUS)Iter->second;

	return lpReturn;
}

//------>
// [업데이트] 5차 부분유료화 캐쉬샵 소켓아이템 (grooving)
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_05_20080930
BOOL CCashShop::IsSocketItem(int iItemCode)
{
	switch( iItemCode )
	{
	case MAKE_ITEMNUM2( 12, 60 ):	//시드(불)
	case MAKE_ITEMNUM2( 12, 61 ):	//시드(물)
	case MAKE_ITEMNUM2( 12, 62 ):	//시드(얼음)
	case MAKE_ITEMNUM2( 12, 63 ):	//시드(바람)
	case MAKE_ITEMNUM2( 12, 64 ):	//시드(번개)
	case MAKE_ITEMNUM2( 12, 65 ):	//시드(땅)
		return TRUE;
	}
	
	return FALSE;
}
#endif
//<------

BOOL CCashShop::IsGetAmountFromShopItem(int iItemCode)
{
	switch( iItemCode )
	{
	case MAKE_ITEMNUM2( 13, 46 ):
	case MAKE_ITEMNUM2( 13, 47 ):
	case MAKE_ITEMNUM2( 13, 48 ):
	case MAKE_ITEMNUM2( 14, 53 ):
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
	case MAKE_ITEMNUM2( 13, 53 ):		// 면죄부
	case MAKE_ITEMNUM2( 13, 54 ):		// 힘리셋열매
	case MAKE_ITEMNUM2( 13, 55 ):		// 민첩리셋열매
	case MAKE_ITEMNUM2( 13, 56 ):		// 체력리셋열매
	case MAKE_ITEMNUM2( 13, 57 ):		// 에너지리셋열매
	case MAKE_ITEMNUM2( 13, 58 ):		// 통솔리셋열매
	case MAKE_ITEMNUM2( 14, 70 ):		// 엘리트치료물약
	case MAKE_ITEMNUM2( 14, 71 ):		// 엘리트마나물약
	case MAKE_ITEMNUM2( 14, 78 ):		// 힘의비약
	case MAKE_ITEMNUM2( 14, 79 ):		// 민첩의비약
	case MAKE_ITEMNUM2( 14, 80 ):		// 체력의비약
	case MAKE_ITEMNUM2( 14, 81 ):		// 에너지의비약
	case MAKE_ITEMNUM2( 14, 82 ):		// 통솔의비약
#endif // ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
	case MAKE_ITEMNUM2( 14, 94 ):		// 중간 엘리트치료물약
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
#ifdef ADD_ELITE_SD_POTION_20090918
	case MAKE_ITEMNUM2( 14, 133 ):		// 엘리트 SD 회복 물약
#endif // ADD_ELITE_SD_POTION_20090918
		return TRUE;
	}

	return FALSE;
}

void CCashShop::MakeItemList(LPCASHSHOP_ITEM_STATUS lpItemStatus, tblSellItem* lpItemSellInfo)
{
	//////////////////////////////////////////////////////////////////////////
	// 캐쉬 아이템 리스트 작성 ( 속성 + 가격 )
	//////////////////////////////////////////////////////////////////////////
	// 1. 아이템이 들어갈 페이지 번호와 해당 페이지 에서의 인덱스를 얻어온다.
	int iIndex = 0;
	int iItemCount = lpItemSellInfo->dwPriceCount;
	int iCategory = lpItemSellInfo->dwCategoryID;
	int iItemIndex = iCashItemCountInCategory[iCategory];
	int iBranchType = 0;
	int iItemPrice = 0;
	int iItemSaleRate = 0;

	// 1-1. 새로 추가된 아이템의 개수를 저장한다.
//	for( iIndex = 0 ; iIndex < MAX_CASHITEM_LIST_CATEGORY ; iIndex++ )
//	{
//		iAddItemCountInCategory[iIndex] = 0;
//	}

	// 1-2. 아이템을 해당 카테고리에 추가한다.
	for( iIndex = 0 ; iIndex < iItemCount ; iIndex++ )
	{
		iBranchType = GetBranchType( lpItemSellInfo->dwItemGuid );

		CashItemList[iCategory][iItemIndex+iIndex].dwItemGuid			= lpItemSellInfo->dwItemGuid;
		CashItemList[iCategory][iItemIndex+iIndex].dwPriceGuid			= lpItemSellInfo->itemPrice[iIndex].dwPriceGuid;
		CashItemList[iCategory][iItemIndex+iIndex].btCategoryCode		= iCategory;
		CashItemList[iCategory][iItemIndex+iIndex].btItemType			= lpItemStatus->btItemType;
		CashItemList[iCategory][iItemIndex+iIndex].wItemIndex			= lpItemStatus->wItemIndex;
		CashItemList[iCategory][iItemIndex+iIndex].btItemLevel			= lpItemStatus->btItemLevel;
		CashItemList[iCategory][iItemIndex+iIndex].btItemAddOption		= lpItemStatus->btAddOption;
		CashItemList[iCategory][iItemIndex+iIndex].btItemLuckOption		= lpItemStatus->btLuckOption;
		CashItemList[iCategory][iItemIndex+iIndex].btItemSkillOpion		= lpItemStatus->btSkillOption;
		CashItemList[iCategory][iItemIndex+iIndex].btItemExOption		= lpItemStatus->btExOption;
		CashItemList[iCategory][iItemIndex+iIndex].btItemUsedType		= lpItemSellInfo->dwUseType;
		CashItemList[iCategory][iItemIndex+iIndex].btSpecialOption		= iBranchType;
		if( IsGetAmountFromShopItem( MAKE_ITEMNUM( lpItemStatus->btItemType, lpItemStatus->wItemIndex ) ) == TRUE )
		{
			CashItemList[iCategory][iItemIndex+iIndex].btItemDuration		= lpItemSellInfo->itemPrice[iIndex].dwAmount;			// 개수 (내구도)
		}
		else
		{
			CashItemList[iCategory][iItemIndex+iIndex].btItemDuration		= lpItemStatus->btDurability;			// 개수 (내구도)
		}
		//------>
		// [업데이트] 5차 부분유료화 캐쉬샵 소켓아이템 - 소켓아이템 레벨 (grooving)
		#ifdef ADD_PARTIALY_CHARGE_SYSTEM_05_20080930
		if( IsSocketItem( MAKE_ITEMNUM( lpItemStatus->btItemType, lpItemStatus->wItemIndex ) ) == TRUE )
		{
			CashItemList[iCategory][iItemIndex+iIndex].btItemLevel		= lpItemSellInfo->itemPrice[iIndex].dwAmount - 1;			// 개수	
		}
		#endif
		//<------

		// 가격 설정시 활인률 적용 -- !!삭제
		//iItemSaleRate	= lpItemSellInfo->itemPrice[iIndex].dwSellRate;
		//iItemPrice		= lpItemSellInfo->itemPrice[iIndex].dwPrice;
		//if( iItemSaleRate > 0 )
		//{
		//	iItemPrice = iItemPrice - (int)(( iItemPrice * iItemSaleRate ) / 100);
		//	//iItemPrice = ( ( iItemPrice * ( 100 - iItemSaleRate ) ) / 100 );
		//}

		CashItemList[iCategory][iItemIndex+iIndex].btItemSaleRatio		= lpItemSellInfo->itemPrice[iIndex].dwSellRate;
		CashItemList[iCategory][iItemIndex+iIndex].wItemPrice			= lpItemSellInfo->itemPrice[iIndex].dwPrice;
		CashItemList[iCategory][iItemIndex+iIndex].dwItemUsePeriod		= lpItemSellInfo->itemPrice[iIndex].dwUseTime;
		memcpy( CashItemList[iCategory][iItemIndex+iIndex].btItemInfo, lpItemStatus->btItemInfo, ITEM_BUFFER_SIZE );
		CashItemList[iCategory][iItemIndex+iIndex].ItemInfo.Convert(
			MAKE_ITEMNUM( lpItemStatus->btItemType, lpItemStatus->wItemIndex ),
			lpItemStatus->btSkillOption,
			lpItemStatus->btLuckOption,
			lpItemStatus->btAddOption,
			lpItemStatus->btExOption);

		MapCashItemList.insert( pair<int, LPCASHSHOP_ITEMLIST> (CashItemList[iCategory][iItemIndex+iIndex].dwPriceGuid, &CashItemList[iCategory][iItemIndex+iIndex] ) );

		CashItemListCompress[iCategory][iItemIndex+iIndex].dwItemGuid		= lpItemSellInfo->itemPrice[iIndex].dwPriceGuid;
		CashItemListCompress[iCategory][iItemIndex+iIndex].btCategoryCode	= iCategory;
		CashItemListCompress[iCategory][iItemIndex+iIndex].btItemDuration	= CashItemList[iCategory][iItemIndex+iIndex].btItemDuration;
		CashItemListCompress[iCategory][iItemIndex+iIndex].btItemSaleRatio	= lpItemSellInfo->itemPrice[iIndex].dwSellRate;
		CashItemListCompress[iCategory][iItemIndex+iIndex].wItemPrice		= lpItemSellInfo->itemPrice[iIndex].dwPrice;
		CashItemListCompress[iCategory][iItemIndex+iIndex].btSpecialOption	= iBranchType;
		CashItemListCompress[iCategory][iItemIndex+iIndex].dwItemUsePeriod	= lpItemSellInfo->itemPrice[iIndex].dwUseTime;
		memcpy( CashItemListCompress[iCategory][iItemIndex+iIndex].btItemInfo, lpItemStatus->btItemInfo, ITEM_BUFFER_SIZE );
		
		//------>
		// [업데이트] 5차 부분유료화 캐쉬샵 소켓아이템 - 클라이언트에 소켓 아이템 레벨 주기 위해서 (grooving)
		#ifdef ADD_PARTIALY_CHARGE_SYSTEM_05_20080930
		if( IsSocketItem( MAKE_ITEMNUM( lpItemStatus->btItemType, lpItemStatus->wItemIndex ) ) == TRUE )
			CashItemListCompress[iCategory][iItemIndex+iIndex].btItemInfo[1] |= (lpItemSellInfo->itemPrice[iIndex].dwAmount - 1) << 3;
		#endif
		//<------

		iAddItemCountInCategory[iCategory]++;
		iCashItemCountInCategory[iCategory]++;

		LogAddTD("[CashShop][ShopList] (Category:%d, Guid:%d) ItemInfo(Type : %d, Index : %d, Name : %s, Price : %d(%d) Amount : %d, UseTime : %d, SPOP:%d ",
			iCategory,
			CashItemListCompress[iCategory][iItemIndex+iIndex].dwItemGuid,
			CashItemList[iCategory][iItemIndex+iIndex].btItemType,
			CashItemList[iCategory][iItemIndex+iIndex].wItemIndex,
			ItemAttribute[MAKE_ITEMNUM( CashItemList[iCategory][iItemIndex+iIndex].btItemType, CashItemList[iCategory][iItemIndex+iIndex].wItemIndex)].Name,
			CashItemListCompress[iCategory][iItemIndex+iIndex].wItemPrice,
			CashItemListCompress[iCategory][iItemIndex+iIndex].btItemSaleRatio,
			CashItemListCompress[iCategory][iItemIndex+iIndex].btItemDuration,
			CashItemList[iCategory][iItemIndex+iIndex].dwItemUsePeriod,
			iBranchType );

#if TESTSERVER == 1
		AddLog( "[CashShop][ShopList] (Category:%d, Guid:%d) ItemInfo(Type : %d, Index : %d, Name : %s, Price : %d(%d) Amount : %d, UseTime : %d, SPOP:%d ", 
			iCategory,
			CashItemListCompress[iCategory][iItemIndex+iIndex].dwItemGuid,
			CashItemList[iCategory][iItemIndex+iIndex].btItemType,
			CashItemList[iCategory][iItemIndex+iIndex].wItemIndex,
			ItemAttribute[MAKE_ITEMNUM( CashItemList[iCategory][iItemIndex+iIndex].btItemType, CashItemList[iCategory][iItemIndex+iIndex].wItemIndex)].Name,
			CashItemListCompress[iCategory][iItemIndex+iIndex].wItemPrice,
			CashItemListCompress[iCategory][iItemIndex+iIndex].btItemSaleRatio,
			CashItemListCompress[iCategory][iItemIndex+iIndex].btItemDuration,
			CashItemList[iCategory][iItemIndex+iIndex].dwItemUsePeriod,
			iBranchType );
#endif

		if( ( iItemIndex + iIndex - 1 ) > MAX_CASHITEM_LIST_IN_CATEGORY )
		{
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
}

void CCashShop::MakeItemListProtocol()
{
	int iCategory = 0;
	int iPageIndex = 0;
	int iItemIndex = 0;

	int iItemCount = 0;
	int iAddItemCount = 0;		// 추가 된 아이템 카운트
	int iAddItemIndex = 0;		// 프로토콜 작성 지점
	int iAddItemListIndex = 0;	// 아이템 리스트 정보 인덱스

	LPPMSG_ANS_CASHITEMLIST lpMsg = NULL;

	//////////////////////////////////////////////////////////////////////////
	// 아이템 리스트 프로토콜 작성
	//////////////////////////////////////////////////////////////////////////
	// 1. 각 카테고리별, 페이지별 클라이언트에게 보낼 아이템 리스트 프로토콜을 작성한다.
	for( iCategory = 0 ; iCategory < MAX_CASHITEM_LIST_CATEGORY ; iCategory++ )
	{
		// 1-1. 각 카테고리별로 아이템 리스트가 추가될 부분을 얻어온다.
		iItemCount = iCashItemCountInCategory[iCategory];
		iAddItemCount = iAddItemCountInCategory[iCategory];
		iAddItemIndex = iItemCount - iAddItemCount;
		iAddItemListIndex = iAddItemIndex;

		// 1-2. 각 카테고리별 아이템 카운트로 추가될 페이지와 인덱스를 계산한다.
		iPageIndex = iAddItemIndex / MAX_CASHITEM_LIST_PER_PAGE;

		iItemIndex = CashItemProtocol[iCategory][iPageIndex].btItemCount;

		for( int i = 0 ; i < iAddItemCount ; i++ )
		{
			CashItemProtocol[iCategory][iPageIndex].btCategoryIndex = iCategory;
			CashItemProtocol[iCategory][iPageIndex].btPageIndex		= iPageIndex;
			CashItemProtocol[iCategory][iPageIndex].ItemInfo[iItemIndex+i].dwItemGuid			= CashItemListCompress[iCategory][iAddItemListIndex].dwItemGuid;
			CashItemProtocol[iCategory][iPageIndex].ItemInfo[iItemIndex+i].btCategoryCode		= CashItemListCompress[iCategory][iAddItemListIndex].btCategoryCode;
			CashItemProtocol[iCategory][iPageIndex].ItemInfo[iItemIndex+i].btItemDuration		= CashItemListCompress[iCategory][iAddItemListIndex].btItemDuration;
			CashItemProtocol[iCategory][iPageIndex].ItemInfo[iItemIndex+i].btItemSaleRatio		= CashItemListCompress[iCategory][iAddItemListIndex].btItemSaleRatio;
			CashItemProtocol[iCategory][iPageIndex].ItemInfo[iItemIndex+i].wItemPrice			= CashItemListCompress[iCategory][iAddItemListIndex].wItemPrice;
			CashItemProtocol[iCategory][iPageIndex].ItemInfo[iItemIndex+i].btSpecialOption		= CashItemListCompress[iCategory][iAddItemListIndex].btSpecialOption;
			CashItemProtocol[iCategory][iPageIndex].ItemInfo[iItemIndex+i].dwItemUsePeriod		= CashItemListCompress[iCategory][iAddItemListIndex].dwItemUsePeriod;
			memcpy( CashItemProtocol[iCategory][iPageIndex].ItemInfo[iItemIndex+i].btItemInfo, CashItemListCompress[iCategory][iAddItemListIndex].btItemInfo, ITEM_BUFFER_SIZE );

			CashItemProtocol[iCategory][iPageIndex].btItemCount++;

			iAddItemCountInCategory[iCategory]--;
			iAddItemListIndex++;

			if( CashItemProtocol[iCategory][iPageIndex].btItemCount >= 9 )
			{
				iAddItemCount -= i + 1;
				i = -1;
				iPageIndex++;
			}

			PHeadSubSetB( (LPBYTE)&CashItemProtocol[iCategory][iPageIndex], 0xF5, 0x06, sizeof( PMSG_ANS_CASHITEMLIST ) );
		}
	}
}

LPBYTE CCashShop::GetItemList(int iCategory, int iPageCount)
{
	if( iPageCount < 0 || iPageCount >= MAX_CASHITEM_LIST_PAGE )
	{
		return NULL;
	}

	return (LPBYTE)&CashItemProtocol[iCategory][iPageCount];
}


// Map과 gObj[]를 둘다 사용하도록 수정!! Define으로 선택 가능하도록 설정
BOOL CCashShop::AddUser(LPOBJECTSTRUCT lpObj)
{
	if( SearchUser( lpObj->DBNumber ) != NULL )
	{
		return FALSE;
	}

	lpObj->m_wCashPoint = 10000;

	MapUserObject.insert( pair<int, LPOBJECTSTRUCT> ( lpObj->DBNumber, lpObj ) );
	return TRUE;
}

BOOL CCashShop::DeleteUser(LPOBJECTSTRUCT lpObj)
{
	ITER_OBJECTSTRUCT Iter;

	Iter = MapUserObject.find( lpObj->DBNumber );
	if( Iter == MapUserObject.end() )
	{
		// 해당 사용자가 없다.
		return FALSE;
	}

	MapUserObject.erase( Iter );
	return TRUE;
}

LPOBJECTSTRUCT CCashShop::SearchUser(int iUserGuid)
{
	LPOBJECTSTRUCT lpReturnObj = NULL;
	ITER_OBJECTSTRUCT Iter;

	Iter = MapUserObject.find( iUserGuid );
	if( Iter == MapUserObject.end() )
	{
		return NULL;
	}

	lpReturnObj = (LPOBJECTSTRUCT)Iter->second;

	return lpReturnObj;
}

BOOL CCashShop::CGCashShopOpen(LPOBJECTSTRUCT lpObj, LPPMSG_REQ_CASHSHOPOPEN lpMsg)
{
	BYTE btResult = RESULT_SUCCESS;
	PMSG_ANS_CASHSHOPOPEN pMsg;

	if( bCashItemListReload == TRUE )
	{
		btResult = RESULT_FAIL_SHOPSERVER_DISCONNECT;
	}

	if( g_bConnectShopServer == TRUE && g_bShopServerConnectState == FALSE )
	{
		btResult = RESULT_FAIL_SHOPSERVER_DISCONNECT;
	}

	// 1. 사용자가 정상적인 사용자 인지를 확인한다.
	if( gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(431), lpObj->AccountID, lpObj->m_Index);
		return FALSE;
	}

	// 1-1. 사용자가 접속종료 중이면 샵 인터페이스를 띄우지 못하도록 한다.
	if( lpObj->Connected <= CSS_LOGINOK || lpObj->CloseCount != -1 )
	{
		return FALSE;
	}

	// 2. 사용자의 샵 인터페이스 요청 내용을 확인한다.
	// 2-1. 샵 인터페이스 오픈
	if( lpMsg->btShopOpenType == CASHSHOP_INTERFACE_OPEN )
	{
		if( lpObj->m_IfState.use > 0 )
		{
			btResult = RESULT_FAIL_OPEN_CASHSHOP;
		}

		if( btResult == RESULT_SUCCESS )
		{
			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = I_CASHSHOP_INTERFACE;
			lpObj->m_IfState.state = 1;
			// 2-1-1. 샵 인터페이스 오픈 시 사용자의 캐쉬 잔량을 요청한다.
			CGCashPoint( lpObj );
		}
	}
	// 2-2. 샵 인터페이스 클로즈
	else if( lpMsg->btShopOpenType == CASHSHOP_INTERFACE_CLOSE )
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.type = 0;
		lpObj->m_IfState.state = 0;
	}

	pMsg.btResult = btResult;

	PHeadSubSetB( (LPBYTE)&pMsg, 0xF5, 0x02, sizeof( PMSG_ANS_CASHSHOPOPEN ) );

	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size );

	return TRUE;
}

BOOL CCashShop::CGCashPoint(LPOBJECTSTRUCT lpObj)
{
	DWORD dwUserGuid = 0;

	if( g_bConnectShopServer == TRUE && g_bShopServerConnectState == FALSE )
	{
		return FALSE;
	}

	if( gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(431), lpObj->AccountID, lpObj->m_Index);
		return FALSE;
	}

	if( lpObj->Connected != 3 && lpObj->Type != OBJTYPE_CHARACTER )
	{
		return FALSE;
	}

	dwUserGuid = lpObj->DBNumber;

	GSReqCashPoint( dwUserGuid );

	return TRUE;
}

void CCashShop::GCCashPoint(LPOBJECTSTRUCT lpObj, DWORD dwCashPoint)
{

	if( gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(431), lpObj->AccountID, lpObj->m_Index);
		return;
	}

	PMSG_ANS_CASHPOINT	pMsg;

	PHeadSubSetB( (LPBYTE)&pMsg, 0xF5, 0x04, sizeof( PMSG_ANS_CASHPOINT ) );
	lpObj->m_wCashPoint = dwCashPoint;
	pMsg.iCashPoint = dwCashPoint;

	if( dwCashPoint < 0 )
	{
		return;
	}

	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size );
}

BOOL CCashShop::GCCashItemListSend(LPOBJECTSTRUCT lpObj, LPPMSG_REQ_CASHITEMLIST lpMsg)
{
	BOOL bResult = FALSE;
	LPBYTE lpSendMsg = NULL;

	int iCategory = 0;
	int iPageIndex = 0;

	if( bCashItemListReload == TRUE )
	{
		return FALSE;
	}

	if( g_bConnectShopServer == TRUE && g_bShopServerConnectState == FALSE )
	{
		return FALSE;
	}

	iCategory = lpMsg->btCategoryIndex;
	iPageIndex = lpMsg->btPageIndex;

	if( lpMsg->btPageLoaded == 0 )
	{
		return FALSE;
	}

	lpSendMsg = GetItemList( iCategory, iPageIndex );
	if( lpSendMsg == NULL )
	{
		return FALSE;
	}

#ifdef  MODIFY_BUGFIX_CASHSHOP_OPEN_INTERFACE
	int msgSize = lpSendMsg[1];
	
	if(msgSize < 1) {
		LogAddTD("[CashShop][GCCashItemListSend] size is zero !!");
		
		return FALSE;
	}
#endif

	DataSend( lpObj->m_Index, lpSendMsg, sizeof( PMSG_ANS_CASHITEMLIST) );

	return TRUE;
}

void CCashShop::CGCashItemBuy(LPOBJECTSTRUCT lpObj, LPPMSG_REQ_CASHITEM_BUY lpMsg)
{
	int iCategoryIndex = 0;
	BYTE btResult = RESULT_SUCCESS;
	BYTE btPosition = 0x00;
	BOOL bItemEmptySpace = FALSE;

	int iItemCode = 0;
	LPCASHSHOP_ITEMLIST	lpCashItemInfo = NULL;

	if( bCashItemListReload == TRUE )
	{
		btResult = RESULT_FAIL_SHOP_ERROR;

		goto label_end;
	}

	if( g_bConnectShopServer == TRUE && g_bShopServerConnectState == FALSE )
	{
		btResult = RESULT_FAIL_SHOPSERVER_DISCONNECT;

		goto label_end;
	}

	if( gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(431), lpObj->AccountID, lpObj->m_Index);
		btResult = RESULT_FAIL_ETC;

		goto label_end;
	}

	if( lpObj->Connected != 3 && lpObj->Type != OBJTYPE_CHARACTER )
	{
		btResult = RESULT_FAIL_ETC;

		goto label_end;
	}

	iCategoryIndex = lpMsg->btCategoryIndex;

	if( iCategoryIndex < 0 || iCategoryIndex > MAX_CASHITEM_LIST_CATEGORY )
	{
		btResult = RESULT_FAIL_NOT_FOUND_ITEM;

		goto label_end;
	}

	// 구매할 아이템의 정보를 검색한다.
	lpCashItemInfo = SearchItemList( lpMsg->dwItemPriceGuid );

	if( lpCashItemInfo == NULL )
	{
		btResult = RESULT_FAIL_NOT_FOUND_ITEM;

		//lpCashItemInfo NULL 인경우 아래처리를 무시해야 함(riverstyx)
		LogAddTD("[CashShop][Buy Request] User(ID:%s, Name:%s) Item(Guid:%d,Category:%d) Result:RESULT_FAIL_NOT_FOUND_ITEM",
			lpObj->AccountID,
			lpObj->Name,
			lpMsg->dwItemPriceGuid,
			iCategoryIndex);
		
		goto label_end;
	}
	
	iItemCode = MAKE_ITEMNUM( lpCashItemInfo->btItemType, lpCashItemInfo->wItemIndex );

#ifdef ADD_CHARCARD_SUMMONER_20080313
	//소환술사캐릭터카드인 경우 이전에 구매하였는지 검사한다.
	if (iItemCode == MAKE_ITEMNUM( 14, 91 ) && lpObj->m_bCharCardSummoner)
	{
		btResult = RESULT_FAIL_NOT_FOUND_ITEM;
		
		goto label_end;
	}
#endif // ADD_CHARCARD_SUMMONER_20080313

	if( CheckPeriodItem( iItemCode ) == TRUE )
	{
		// 패키지 아이템의 경우 이미 사용중인 아이템이 있는지 확인한다.
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		g_CashItemPeriodSystem.CheckItemUsedInfo( lpObj, iItemCode );
#else
		if( CheckPeriodItemUsed( lpObj, iItemCode ) == TRUE )
		{
			btResult = RESULT_FAIL_ALREADY_HAVE_ITEM;
		}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

	}
	else
	{
		if( CheckInventoryEmptySpace( lpObj, lpCashItemInfo ) == FALSE )
		{
			btResult = RESULT_FAIL_NOT_EMPTY_SPACE;
		}
	}

label_end:
	if( btResult == RESULT_SUCCESS )
	{
		LogAddTD("[CashShop][Buy Request] User(ID:%s, Name:%s) Item(Name:%s,Guid:%d,Category:%d,Price:%d,SaleRate:%d) Result:%d",
			lpObj->AccountID,
			lpObj->Name,
			ItemAttribute[iItemCode].Name,
			lpMsg->dwItemPriceGuid,
			iCategoryIndex,
			lpCashItemInfo->wItemPrice,
			lpCashItemInfo->btItemSaleRatio,
			btResult );

		GSReqBuyCashItem( gGameServerCode, lpObj->DBNumber, lpObj->m_Index, lpObj->Name, MAX_BUY_ITEM_AT_ONCE, lpMsg->dwItemPriceGuid );
	}
	else
	{
		LogAddTD("[CashShop][Buy Request] User(ID:%s, Name:%s) Item(Guid:%d,Category:%d) Result:%d",
			lpObj->AccountID,
			lpObj->Name,
			lpMsg->dwItemPriceGuid,
			iCategoryIndex, 
			btResult);

		GCCashItemBuyResult( lpObj, btResult );
	}
}

void CCashShop::GCCashItemBuyResult(LPOBJECTSTRUCT lpObj, BYTE btResult)
{
	PMSG_ANS_CASHITEM_BUY pMsg;

	pMsg.btResult = btResult;
	PHeadSubSetB( (LPBYTE)&pMsg, 0xF5, 0x08, sizeof( PMSG_ANS_CASHITEM_BUY ) );

	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size );
}

BOOL CCashShop::CheckPeriodItemUsed(LPOBJECTSTRUCT lpObj, int iItemCode)
{
	if( lpObj->m_iPeriodItemEffectIndex != -1 )
	{
		if( iItemCode == MAKE_ITEMNUM( 13, 43 )
			|| iItemCode == MAKE_ITEMNUM( 13, 44 )
			|| iItemCode == MAKE_ITEMNUM( 13, 45 )
#ifdef ADD_CASHITEM_4RD_MARK_20080723
			|| iItemCode == MAKE_ITEMNUM( 13, 62 )		// 치유의 인장
			|| iItemCode == MAKE_ITEMNUM( 13, 63 )		// 신성의 인장
#endif // ADD_CASHITEM_4RD_MARK_20080723
#ifdef ADD_ELITE_SCROLL_20070521
			|| iItemCode == MAKE_ITEMNUM( 14, 72 )		// 신속의 스크롤
			|| iItemCode == MAKE_ITEMNUM( 14, 73 )		// 방어의 스크롤
			|| iItemCode == MAKE_ITEMNUM( 14, 74 )		// 분노의 스크롤
			|| iItemCode == MAKE_ITEMNUM( 14, 75 )		// 마력의 스크롤
			|| iItemCode == MAKE_ITEMNUM( 14, 76 )		// 체력의 스크롤
			|| iItemCode == MAKE_ITEMNUM( 14, 77 )		// 마나의 스크롤
#endif // ADD_ELITE_SCROLL_20070521
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
			|| iItemCode == MAKE_ITEMNUM( 14, 97 )		// 전투의 스크롤
			|| iItemCode == MAKE_ITEMNUM( 14, 98 )		// 강화의 스크롤
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			|| iItemCode == MAKE_ITEMNUM( 13, 93 )      // 상승의 인장 마스터 
			|| iItemCode == MAKE_ITEMNUM( 13, 94 )      // 풍요의 인장 마스터 
#endif
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
			|| iItemCode == MAKE_ITEMNUM( 13, 103 )      // 파티 경험치 상승 주문서
			|| iItemCode == MAKE_ITEMNUM( 13, 104 )      // 최대 AG 상승 오라
			|| iItemCode == MAKE_ITEMNUM( 13, 105 )      // 최대 SD 상승 오라
#endif // ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
		  )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCashShop::CheckPeriodItem(int iItemCode)
{
	if( iItemCode == MAKE_ITEMNUM( 13, 43 )			// 상승의 인장
		|| iItemCode == MAKE_ITEMNUM( 13, 44 )		// 풍요의 인장
		|| iItemCode == MAKE_ITEMNUM( 13, 45 )		// 유지의 인장
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
	#ifdef MODIFY_PCS_OVERLAP_ITEM_BUFF_INDEX_20071011
		|| iItemCode == MAKE_ITEMNUM( 13, 59 )		// 이동의 인장
	#else
		|| iItemCode == MAKE_ITEMNUM( 13, 52 )		// 이동의 인장
	#endif // MODIFY_PCS_OVERLAP_ITEM_BUFF_INDEX_20071011
		|| iItemCode == MAKE_ITEMNUM( 14, 72 )		// 신속의 스크롤
		|| iItemCode == MAKE_ITEMNUM( 14, 73 )		// 방어의 스크롤
		|| iItemCode == MAKE_ITEMNUM( 14, 74 )		// 분노의 스크롤
		|| iItemCode == MAKE_ITEMNUM( 14, 75 )		// 마력의 스크롤
		|| iItemCode == MAKE_ITEMNUM( 14, 76 )		// 체력의 스크롤
		|| iItemCode == MAKE_ITEMNUM( 14, 77 )		// 마나의 스크롤
#endif // ADD_PARTIALY_CHARGE_SYSTEM_02_20070423

#ifdef ADD_CASHITEM_4RD_MARK_20080723
		|| iItemCode == MAKE_ITEMNUM( 13, 62 )		// 치유의 인장
		|| iItemCode == MAKE_ITEMNUM( 13, 63 )		// 신성의 인장
#endif // ADD_CASHITEM_4RD_MARK_20080723
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
		|| iItemCode == MAKE_ITEMNUM( 14, 97 )		// 전투의 스크롤
		|| iItemCode == MAKE_ITEMNUM( 14, 98 )		// 강화의 스크롤
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		//		|| iItemCode == MAKE_ITEMNUM( 13, 81 )
		//		|| iItemCode == MAKE_ITEMNUM( 13, 82 )
		|| iItemCode == MAKE_ITEMNUM( 13, 93 )      // 상승의 인장 마스터 
		|| iItemCode == MAKE_ITEMNUM( 13, 94 )      // 풍요의 인장 마스터 
#endif 
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
		|| iItemCode == MAKE_ITEMNUM( 13, 103 )      // 파티 경험치 상승 주문서
		|| iItemCode == MAKE_ITEMNUM( 13, 104 )      // 최대 AG 상승 오라
		|| iItemCode == MAKE_ITEMNUM( 13, 105 )      // 최대 SD 상승 오라
#endif // ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
	  )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CCashShop::CheckInventoryEmptySpace(LPOBJECTSTRUCT lpObj, LPCASHSHOP_ITEMLIST lpItemInfo)
{
	int h = 0;
	int w = 0;
	BYTE blank = 0;

	int iItemHeight = 0;
	int iItemWidth = 0;

	lpItemInfo->ItemInfo.GetSize( iItemWidth, iItemHeight );

	for( h=0; h<8; h++)
	{		
		for( w=0; w<8; w++)	
		{
			// 비어있는지 체크
			if( *(lpObj->pInventoryMap+(h*8)+w) == 0xFF )
			{
				blank = gObjOnlyInventoryRectCheck( lpObj->m_Index, w, h, iItemWidth, iItemHeight);
				// 
				if( blank == 0xFE )
				{
					return FALSE;
				}

				if( blank != 0xFF )	// 인벤토리에 자리가 있다면
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

LPCASHSHOP_ITEMLIST CCashShop::SearchItemList(int iItemGuid)
{
	LPCASHSHOP_ITEMLIST lpItemInfo = NULL;
	ITER_CASHITEM_LIST Iter;

	Iter = MapCashItemList.find( iItemGuid );
	if( Iter == MapCashItemList.end() )
	{
		return NULL;
	}

	lpItemInfo = (LPCASHSHOP_ITEMLIST)Iter->second;

	return lpItemInfo;
}

BOOL CCashShop::GiveBuyItemToInventory(LPOBJECTSTRUCT lpObj, int iItemGuid )
{
	BYTE btPosition = 0;
	BYTE btResult = 0;
	BOOL bEmptySpace = FALSE;
	LPCASHSHOP_ITEMLIST lpItemInfo = NULL;
	CItem pItem;
	int iItemCode = 0;
	BYTE ExOption[6];

	lpItemInfo = SearchItemList( iItemGuid );

	iItemCode = MAKE_ITEMNUM( lpItemInfo->btItemType, lpItemInfo->wItemIndex );

	bEmptySpace = CheckInventoryEmptySpace( lpObj, lpItemInfo );
	if( bEmptySpace == FALSE )
	{
		return FALSE;
	}

	pItem.Convert( iItemCode, lpItemInfo->btItemSkillOpion, lpItemInfo->btItemLuckOption, lpItemInfo->btItemAddOption, lpItemInfo->btItemExOption, 0 );
	pItem.m_Level = lpItemInfo->btItemLevel;
	pItem.m_Durability = lpItemInfo->btItemDuration;

	ItemIsBufExOption( ExOption, &pItem );

	// 시리얼 아이템 발급
	ItemSerialCreateSend( lpObj->m_Index,
		ITEMMAKE_INVENTORY_CASHITEM,
		lpObj->X, lpObj->Y,
		pItem.m_Type,
		pItem.m_Level,
		pItem.m_Durability,
		pItem.m_Option1,
		pItem.m_Option2,
		pItem.m_Option3,
		lpObj->m_Index,
		pItem.m_NewOption,
		0 );

	LogAddTD("[CashShop][Buy Item Create in Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,Skill:%d,Luck:%d,Add:%d,Ex(%d:%d:%d:%d:%d:%d))",
		lpObj->AccountID,
		lpObj->Name,
		ItemAttribute[iItemCode].Name,
		iItemCode,
		pItem.m_Option1,
		pItem.m_Option2,
		pItem.m_Option3,
		ExOption[0],
		ExOption[1],
		ExOption[2],
		ExOption[3],
		ExOption[4],
		ExOption[5] );

	return TRUE;
}

void CCashShop::GSReqCashPoint(DWORD dwUserGuid)
{
	MSG_GTOS_USER_CASH_REQ pMsg;

	pMsg.dwUserGuid = dwUserGuid;

	g_ShopServerClient.DataSend( (char*)&pMsg, sizeof( MSG_GTOS_USER_CASH_REQ ) );
}

void CCashShop::GSReqCashItemList()
{
	MSG_GTOS_ITEM_LIST_REQ pMsg;

	g_ShopServerClient.DataSend( (char*)&pMsg, sizeof( MSG_GTOS_ITEM_LIST_REQ ) );

	LogAddTD("[CashShop] Request Cash Item List to ShopServer.");
}

void CCashShop::GSReqPackageItemList()
{
	MSG_GTOS_PACKAGE_LIST_REQ pMsg;

	g_ShopServerClient.DataSend( (char*)&pMsg, sizeof( MSG_GTOS_PACKAGE_LIST_REQ ) );

	LogAddTD("[CashShop] Request Cash Package Item List to ShopServer.");
}

void CCashShop::SetBranchItem(DWORD dwItemGuid, int iBranchType)
{
	if( iBranchItemCount >= MAX_CASHITEM_LIST_BRANCH )
	{
		return;
	}

	BranchItemList[iBranchItemCount].dwItemGuid		= dwItemGuid;
	BranchItemList[iBranchItemCount].iBranchType	= iBranchType;
	iBranchItemCount++;
}

int CCashShop::GetBranchType(DWORD dwItemGuid)
{
	for( int i = 0 ; i < MAX_CASHITEM_LIST_BRANCH ; i++ )
	{
		if( BranchItemList[i].dwItemGuid == dwItemGuid )
		{
			return BranchItemList[i].iBranchType;
		}
	}

	return 0;
}

void CCashShop::GSReqBuyCashItem(DWORD dwServerGuid, DWORD dwUserGuid, DWORD dwCharacterGuid, char* szCharacterName, DWORD dwCount, DWORD dwPriceGuid)
{
	MSG_GTOS_BUY_ITEM_REQ pMsg;

	pMsg.dwServerGuid		= dwServerGuid;
	pMsg.dwUserGuid			= dwUserGuid;
	pMsg.dwCharGuid			= dwCharacterGuid;
	pMsg.dwCount			= 1;
	pMsg.dwPriceGuids[0]	= dwPriceGuid;

	memset( pMsg.szCharName, 0, MAX_CHARACTER_NAME_LENGTH );
	strcpy( pMsg.szCharName, szCharacterName );

	g_ShopServerClient.DataSend( (char*)&pMsg, sizeof( MSG_GTOS_BUY_ITEM_REQ ) );
}

void CCashShop::GSNtfBuyCashItem(MSG_GTOS_BUY_ITEM_NTF* pMsg)
{
	pMsg->resize();

	g_ShopServerClient.DataSend( (char*)pMsg, pMsg->size );
}

void SGAnsCashPoint(MSG_STOG_USER_CASH_ANS* aRecv)
{
	LPOBJECTSTRUCT lpObj = NULL;

	lpObj = g_CashShop.SearchUser( aRecv->dwUserGuid );
	if( lpObj == NULL )
	{
		return;
	}

	g_CashShop.GCCashPoint( lpObj, aRecv->dwUserCash );
}

void SGAnsCashItemList(MSG_STOG_ITEM_LIST_ANS* aRecv)
{
	LogAddTD("[CashShop] Receive Cash Item List from ShopServer. (%d)",
		aRecv->dwItemCount );
	
	if( g_CashShop.SetItemInfoFromShop( aRecv ) == FALSE )
	{
		//LogAddTD("[CashShop]
		return;
	}

	g_CashShop.MakeItemListProtocol();
}

void SGAnsPackageItemList(MSG_STOG_PACKAGE_LIST_ANS* aRecv)
{
	LogAddTD("[CashShop] Receive Cash Package Item List from ShopServer. (%d)",
		aRecv->dwPackageCount );

	if( g_CashShop.SetPackageItemInfoFromShop( aRecv ) == FALSE )
	{
		//LogAddTD
		return;
	}

	g_CashShop.MakeItemListProtocol();
}

void CCashShop::GSReqBranchItemList()
{
	LogAddTD("[CashShop] Request Cash Branch Item List to ShopServer.");

	MSG_GTOS_BRANCH_ITEM_LIST_REQ pMsg;

	g_ShopServerClient.DataSend( (char*)&pMsg, sizeof( MSG_GTOS_BRANCH_ITEM_LIST_REQ ) );
}

void SGAnsBranchItemList(MSG_STOG_BRANCH_ITEM_LIST_ANS* lpMsg)
{
	int iBranchItemCount = 0;
	int iItemGuid = 0;
	LPCASHSHOP_ITEMLIST lpItemInfo = NULL;
	
	iBranchItemCount = lpMsg->dwItemCount;

	for( int i = 0 ; i < iBranchItemCount ; i++ )
	{
		if( lpMsg->branchItems[i].dwItemType == 1 )		// 아이템 일 경우
		{
			g_CashShop.SetBranchItem( lpMsg->branchItems[i].dwGuid, lpMsg->branchItems[i].dwBranchID );
		}
	}
}

void SGAnsBuyCashItem(MSG_STOG_BUY_ITEM_ANS* aRecv)
{
	int iItemBuyTransactionCount = 0;
	LPOBJECTSTRUCT lpObj = NULL;
	LPCASHSHOP_ITEMLIST lpItemList = NULL;
	int iItemCode = 0;
	int iItemUsePeriod = 0;
	MSG_GTOS_BUY_ITEM_NTF pMsg;
	int iResult = eRET_SUCCESS;

	lpObj = g_CashShop.SearchUser( aRecv->dwUserGuid );
	if( lpObj == NULL )
	{
		return;
	}

	// 사용자의 잔여 캐쉬를 업데이트
	lpObj->m_wCashPoint = aRecv->dwUserCash;

	iItemBuyTransactionCount = aRecv->dwCount;

	pMsg.dwServerGuid	= gGameServerCode;
	pMsg.dwUserGuid		= lpObj->DBNumber;
	pMsg.dwCharGuid		= lpObj->m_Index;
	pMsg.dwCount		= iItemBuyTransactionCount;

	for( int i = 0 ; i < iItemBuyTransactionCount ; i++ )
	{
		if( aRecv->transactions[i].dwResult == eRET_SUCCESS )
		{
			lpItemList = g_CashShop.SearchItemList( aRecv->transactions[i].dwPriceGuid );
			iItemCode = MAKE_ITEMNUM( lpItemList->btItemType, lpItemList->wItemIndex );
			if( g_CashShop.CheckPeriodItem( iItemCode ) == TRUE )
			{
				g_CashItemPeriodSystem.GDReqPeriodItemInsert( lpObj, iItemCode, lpItemList->dwItemUsePeriod );
			}
/*
			if( lpItemList->btItemUsedType == 1 )		// 아이템이 정액제인 경우
			{
				g_CashItemPeriodSystem.GDReqPeriodItemInsert( lpObj, iItemCode, aRecv->dwCount );
			}
*/
			else
			{
#ifdef MODIFY_OVERLAP_ITEM_DURATION_20070730
				// 아이템이 비약인 경우 기존에 아이템을 검사하여 갯수에 더해주거나 새로 만들어준다.
				if( iItemCode == MAKE_ITEMNUM( 14, 78 )			// 힘의비약
					|| iItemCode == MAKE_ITEMNUM( 14, 79 )		// 민첩의비약
					|| iItemCode == MAKE_ITEMNUM( 14, 80 )		// 체력의비약
					|| iItemCode == MAKE_ITEMNUM( 14, 81 )		// 에너지의비약
					|| iItemCode == MAKE_ITEMNUM( 14, 82 )		// 통솔의비약
				  )
				{
					if( gObjOverlapItemDuration( lpObj, iItemCode, 0, lpItemList->btItemDuration, MAX_OVERLAP_IMPROVE_POTION ) == FALSE )
					{
						if( g_CashShop.GiveBuyItemToInventory( lpObj, aRecv->transactions[i].dwPriceGuid ) == FALSE )
						{
							iResult = eRET_FAIL;
						}
					}
				}
				else if( iItemCode == MAKE_ITEMNUM( 14, 70 )
					|| iItemCode == MAKE_ITEMNUM( 14, 71 )
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
					|| iItemCode == MAKE_ITEMNUM( 14, 94 )
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
#ifdef ADD_ELITE_SD_POTION_20090918
					|| iItemCode == MAKE_ITEMNUM( 14, 133 )		// 엘리트 SD 회복 물약
#endif // ADD_ELITE_SD_POTION_20090918
					   )
				{
					if( gObjOverlapItemDuration( lpObj, iItemCode, 0, lpItemList->btItemDuration, MAX_OVERLAP_ELITE_POTION ) == FALSE )
					{
						if( g_CashShop.GiveBuyItemToInventory( lpObj, aRecv->transactions[i].dwPriceGuid ) == FALSE )
						{
							iResult = eRET_FAIL;
						}
					}
				}
#ifdef ADD_CHARCARD_SUMMONER_20080313
				else if (iItemCode == MAKE_ITEMNUM( 14, 91 ))	//소환술사캐릭터카드
				{
					DataServerSetAccountInfo(lpObj, lpObj->m_Index);
				}
#endif // ADD_CHARCARD_SUMMONER_20080313
				else
#endif // MODIFY_OVERLAP_ITEM_DURATION_20070730
				if( g_CashShop.GiveBuyItemToInventory( lpObj, aRecv->transactions[i].dwPriceGuid ) == FALSE )
				{
					iResult = eRET_FAIL;
				}
			}

			pMsg.transactions[i].dwPriceGuid		= aRecv->transactions[i].dwPriceGuid;
			pMsg.transactions[i].dwResult			= iResult;
			pMsg.transactions[i].dwTransactionGuid	= aRecv->transactions[i].dwTransactionGuid;
		}
	}

	g_CashShop.GCCashPoint( lpObj, aRecv->dwUserCash );
	g_CashShop.GSNtfBuyCashItem( &pMsg );

	switch( aRecv->dwResult )
	{
	case eRET_SUCCESS:
		iResult = RESULT_SUCCESS;
		break;
	case eRET_FAIL:
		iResult = RESULT_FAIL_SHOP_ERROR;
		break;
	case eRET_NOT_ENOUGH_CASH:
		iResult = RESULT_FAIL_NOT_ENOUGH_CASH;
		break;
	case eRET_DB_ERROR:
		iResult = RESULT_FAIL_DB_ERROR;
		break;
	case eRET_NOT_FOUND_ITEM:
		iResult = RESULT_FAIL_NOT_FOUND_ITEM;
		break;
	case eRET_LIMIT_BUY_COUNT_ERROR:
		iResult = RESULT_FAIL_SHOP_ERROR;
		break;
	case eRET_BUY_COOL_TIME_ERROR:
		iResult = RESULT_FAIL_SHOP_ERROR;
		break;
	case eRET_INVALID_VALUE:
		iResult = RESULT_FAIL_SHOP_ERROR;
		break;
	case eRET_INVALID_ID_VALUE:
		iResult = RESULT_FAIL_SHOP_ERROR;
		break;
#ifdef UPDATE_BUY_CASHITEM_ERROR_CODE_20090410	
	case eRET_NOT_ENOUGH_MILEAGE:
		iResult = RESULT_FAIL_NOT_ENOUGH_MILEAGE;
		break;
	case eRET_DONT_BUY_ITEM:
		iResult = RESULT_FAIL_DONT_BUY_ITEM;
		break;
#endif	// UPDATE_BUY_CASHITEM_ERROR_CODE_20090410
	}

	LogAddTD("[CashShop][Buy Answer] User(ID:%s, Name:%s) Result:%d",
		lpObj->AccountID,
		lpObj->Name,
		iResult );
	g_CashShop.GCCashItemBuyResult( lpObj, iResult );
}

#if TESTSERVER == 1
void AddLog(char* pchLog, ...)
{
	char szBuffer[512]="";
	va_list		pArguments;
	static FILE *	logfp=NULL;

	logfp = fopen("ShopList.txt", "a+t");
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

BOOL IsCashItem(int iItemCode)
{
	switch( iItemCode )
	{
	case MAKE_ITEMNUM2( 13, 43 ):		// 상승의 인장
		return TRUE;
	case MAKE_ITEMNUM2( 13, 44 ):		// 풍요의 인장
		return TRUE;
	case MAKE_ITEMNUM2( 13, 45 ):		// 유지의 인장
		return TRUE;
	case MAKE_ITEMNUM2( 13, 46 ):		// 데빌스퀘어 자유 입장권
		return TRUE;
	case MAKE_ITEMNUM2( 13, 47 ):		// 블러드캐슬 자유 입장권
		return TRUE;
	case MAKE_ITEMNUM2( 13, 48 ):		// 칼리마 자유 입장권
		return TRUE;
	case MAKE_ITEMNUM2( 14, 53 ):		// 행운의 부적
		return TRUE;
	case MAKE_ITEMNUM2( 14, 54 ):		// 카오스 카드
		return TRUE;
	case MAKE_ITEMNUM2( 14, 58 ):		// 희귀 아이템 티켓 1
		return TRUE;
	case MAKE_ITEMNUM2( 14, 59 ):		// 희귀 아이템 티켓 2
		return TRUE;
	case MAKE_ITEMNUM2( 14, 60 ):		// 희귀 아이템 티켓 3
		return TRUE;
	case MAKE_ITEMNUM2( 14, 61 ):		// 희귀 아이템 티켓 4
		return TRUE;
	case MAKE_ITEMNUM2( 14, 62 ):		// 희귀 아이템 티켓 5
		return TRUE;
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
	#ifdef MODIFY_PCS_OVERLAP_ITEM_BUFF_INDEX_20071011
	case MAKE_ITEMNUM2( 13, 59 ):		// 이동의 인장
	case MAKE_ITEMNUM2( 13, 60 ):		// 면죄부
	#else
	case MAKE_ITEMNUM2( 13, 52 ):		// 이동의 인장
	case MAKE_ITEMNUM2( 13, 53 ):		// 면죄부
	#endif // MODIFY_PCS_OVERLAP_ITEM_BUFF_INDEX_20071011
	case MAKE_ITEMNUM2( 13, 54 ):		// 확장 마이너스 열매 - 힘
	case MAKE_ITEMNUM2( 13, 55 ):		// 확장 마이너스 열매 - 민첩
	case MAKE_ITEMNUM2( 13, 56 ):		// 확장 마이너스 열매 - 체력
	case MAKE_ITEMNUM2( 13, 57 ):		// 확장 마이너스 열매 - 에너지
	case MAKE_ITEMNUM2( 13, 58 ):		// 확장 마이너스 열매 - 통솔
	case MAKE_ITEMNUM2( 14, 70 ):		// 엘리트 치료 물약
	case MAKE_ITEMNUM2( 14, 71 ):		// 엘리트 마나 물약
	case MAKE_ITEMNUM2( 14, 72 ):		// 신속의 스크롤
	case MAKE_ITEMNUM2( 14, 73 ):		// 방어의 스크롤
	case MAKE_ITEMNUM2( 14, 74 ):		// 분노의 스크롤
	case MAKE_ITEMNUM2( 14, 75 ):		// 마력의 스크롤
	case MAKE_ITEMNUM2( 14, 76 ):		// 체력의 스크롤
	case MAKE_ITEMNUM2( 14, 77 ):		// 마나의 스크롤
	case MAKE_ITEMNUM2( 14, 78 ):		// 힘의 비약
	case MAKE_ITEMNUM2( 14, 79 ):		// 민첩의 비약
	case MAKE_ITEMNUM2( 14, 80 ):		// 체력의 비약
	case MAKE_ITEMNUM2( 14, 81 ):		// 에너지의 비약
	case MAKE_ITEMNUM2( 14, 82 ):		// 통솔의 비약
#ifdef MODIFY_ADD_CASHITEM_MOVE_CHECK_ITEM_20070821
	case MAKE_ITEMNUM2( 14, 83 ):		// 희귀 아이템 티켓 6
#endif // MODIFY_ADD_CASHITEM_MOVE_CHECK_ITEM_20070821 
#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	case MAKE_ITEMNUM2( 13, 61 ):		// 환영사원 자유 입장권
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
	case MAKE_ITEMNUM2( 14, 94 ):		// 중간 엘리트 치료 물약
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
		return TRUE;
#endif // ADD_PARTIALY_CHARGE_SYSTEM_02_20070423

//#ifdef ADD_PARTIALY_CHARGE_SYSTEM_04_20080723
#ifdef ADD_SAFEGUARD_ITEM_20080702
	case MAKE_ITEMNUM2( 13, 64 ):		// 데몬
	case MAKE_ITEMNUM2( 13, 65 ):		// 수호정령
#endif	//ADD_SAFEGUARD_ITEM_20080702
#ifdef ADD_CHAOSMIXCHARM_ITEM_20080702
	case MAKE_ITEMNUM2( 14, 96 ):		// 카오스조합부적
#endif //ADD_CHAOSMIXCHARM_ITEM_20080702
#ifdef ADD_CASHITEM_4RD_MARK_20080723
	case MAKE_ITEMNUM2( 13, 62 ):		// 치유의 인장
	case MAKE_ITEMNUM2( 13, 63 ):		// 신성의 인장
#endif // ADD_CASHITEM_4RD_MARK_20080723
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
	case MAKE_ITEMNUM2( 14, 97 ):		// 전투의 스크롤
	case MAKE_ITEMNUM2( 14, 98 ):		// 강화의 스크롤
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		//6차 과금 아이템 추가
    case MAKE_ITEMNUM2(13, 81):   // 수호의 부적
	case MAKE_ITEMNUM2(13, 82):  // 아이템 보호 부적
		
	case MAKE_ITEMNUM2(13, 83):  // 사탄날개 부적
	case MAKE_ITEMNUM2(13, 84):  // 천공날개 부적
	case MAKE_ITEMNUM2(13, 85):  // 요정날개 부적
	case MAKE_ITEMNUM2(13, 86):  // 재앙날개 부적
	case MAKE_ITEMNUM2(13, 87):  // 망토조합 부적
	case MAKE_ITEMNUM2(13, 88):  // 드라곤날개 부적
	case MAKE_ITEMNUM2(13, 89):  // 영혼날개 부적
	case MAKE_ITEMNUM2(13, 90):  // 정령날개 부적
    case MAKE_ITEMNUM2(13, 91):  // 절망날개 부적
	case MAKE_ITEMNUM2(13, 92):  // 암흑조합 부적 
		
	case MAKE_ITEMNUM2(13, 93):  // 상승의인장 마스터 
	case MAKE_ITEMNUM2(13, 94):  // 풍요의인장 마스터 		
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
		// 7차 과금 아이템 추가
	case MAKE_ITEMNUM2(14, 133):		// 엘리트 SD 회복 물약
	case MAKE_ITEMNUM2(13, 103):		// 파티 경험치 증가 주문서
	case MAKE_ITEMNUM2(13, 104):		// 최대 AG증가 오라
	case MAKE_ITEMNUM2(13, 105):		// 최대 SD증가 오라
	case MAKE_ITEMNUM2(13, 106):		// 유니콘 펫
	case MAKE_ITEMNUM2(13, 107):		// 치명 마법사 반지
#endif // ADD_PARTIALY_CHARGE_SYSTEM_07_20090129
		return TRUE;
//#endif // ADD_PARTIALY_CHARGE_SYSTEM_04_20080723
	}

	return FALSE;
}

#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PREMIUMITEM_20090106
BOOL	IsPremiumItem(int iItemCode)
{
	switch( iItemCode )
	{
		case MAKE_ITEMNUM2( 13, 65 ):		// 수호 정령
		case MAKE_ITEMNUM2( 13, 64 ):		// 데몬
		case MAKE_ITEMNUM2( 14, 96 ):		// 카오스조합부적
		case MAKE_ITEMNUM2( 14, 54 ):		// 카오스카드
#ifndef FOR_KOREA
//#ifdef ADD_PREMIUMITEM_PANDA
//		case MAKE_ITEMNUM2( 13, 76):			// 팬더변신반지
//		case MAKE_ITEMNUM2( 13, 80):			// 팬더펫
//#endif // ADD_PREMIUMITEM_PANDA
#else // NOT_FOR_KOREA
#ifdef ADD_PREMIUMITEM_PANDA			
		case MAKE_ITEMNUM2( 13, 76):			// 팬더변신반지
		case MAKE_ITEMNUM2( 13, 80):			// 팬더펫
#endif // ADD_PREMIUMITEM_PANDA
#endif // NOT_FOR_KOREA
			return TRUE;
	}

	return FALSE;
}
#endif //PREMIUMITEM_20090106

#ifdef MODIFY_RECONNECTSHOPSERVER_20080807
string GetMIBTcpState(DWORD dwState)
{
	string strState;
	
	switch (dwState)
	{
	case MIB_TCP_STATE_CLOSED:		strState = "CLOSED";		break;
	case MIB_TCP_STATE_LISTEN:		strState = "LISTEN";		break;
	case MIB_TCP_STATE_SYN_SENT:	strState = "SYN_SENT";		break;
	case MIB_TCP_STATE_SYN_RCVD:	strState = "SYN_RCVD";		break;
	case MIB_TCP_STATE_ESTAB:		strState = "ESTAB";			break;
	case MIB_TCP_STATE_FIN_WAIT1:	strState = "FIN_WAIT1";		break;
	case MIB_TCP_STATE_FIN_WAIT2:	strState = "FIN_WAIT2";		break;
	case MIB_TCP_STATE_CLOSE_WAIT:	strState = "CLOSE_WAIT";	break;
	case MIB_TCP_STATE_CLOSING:		strState = "CLOSING";		break;
	case MIB_TCP_STATE_LAST_ACK:	strState = "LAST_ACK";		break;
	case MIB_TCP_STATE_TIME_WAIT:	strState = "TIME_WAIT";		break;
	case MIB_TCP_STATE_DELETE_TCB:	strState = "DELETE_TCB";	break;
	default:
		strState = "";
	}
	
	return strState;
}

void GetTCPTableInfo() {
    MIB_TCPTABLE* pTcpTable = 0;
    DWORD dwSize = 0;
	
    GetTcpTable(pTcpTable, &dwSize, TRUE);
    pTcpTable = (MIB_TCPTABLE*) malloc(dwSize);
    GetTcpTable(pTcpTable, &dwSize, TRUE);
	
	{
		//9(Tab) 제거
		int nLen = strlen(g_ShopServerIP);
		if (nLen > 0 && g_ShopServerIP[nLen -1] == 9) {
			g_ShopServerIP[nLen -1] = 0;
		}
	}
	
    for (unsigned int i = 0; i < pTcpTable->dwNumEntries; i++) 
	{
		char szRemoteAddr[16];
		char szLocalAddr[16];
		{
			unsigned char* localAddr = (unsigned char*)&pTcpTable->table[i].dwLocalAddr;
			unsigned char* remoteAddr = (unsigned char*)&pTcpTable->table[i].dwRemoteAddr;
			wsprintf(szLocalAddr, "%d.%d.%d.%d", *(localAddr+0), *(localAddr+1), *(localAddr+2), *(localAddr+3));
			wsprintf(szRemoteAddr, "%d.%d.%d.%d", *(remoteAddr+0), *(remoteAddr+1), *(remoteAddr+2), *(remoteAddr+3));
		}
		
		if (strcmp(szRemoteAddr, g_ShopServerIP) == 0)
		{
			string strState = GetMIBTcpState(pTcpTable->table[i].dwState);
			LogAddTD("[TCPTable] Local addr: %s:%d, Remote addr: %s:%d, State: %s(%d)"
				, szLocalAddr, pTcpTable->table[i].dwLocalPort, szRemoteAddr, pTcpTable->table[i].dwRemotePort
				, strState.data(), pTcpTable->table[i].dwState);
		}
    } 
	
    free(pTcpTable);
}
#endif // MODIFY_RECONNECTSHOPSERVER_20080807