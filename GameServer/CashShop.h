// CashShop.h: interface for the CCashShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASHSHOP_H__273C22CB_4E7F_40FB_BE1C_F41149A9BBD2__INCLUDED_)
#define AFX_CASHSHOP_H__273C22CB_4E7F_40FB_BE1C_F41149A9BBD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "PCSdefines.h"
//#include "PCSprotocol.h"
#include "wsShopServerCli.h"

#pragma warning (disable : 4786)

#include <map>

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

// ĳ���� ���� �޽���
#define WM_ASS_SHOPSERVERMSG	( WM_USER+ 30 )

#define SHOPSERVER_DEFAULT_PORT	55990

#define MAX_BUY_ITEM_AT_ONCE			1

// ĳ���� �� �� �ִ� �ִ� ������ ����Ʈ ����
#define MAX_CASHITEM_LIST_CATEGORY		4		// ĳ���� ������ ����Ʈ �ִ� ī�װ� ��
#define MAX_CASHITEM_LIST_PAGE			15		// ĳ���� ������ ����Ʈ �ִ� ������ ����
#define MAX_CASHITEM_LIST_PER_PAGE		9		// ĳ���� ������ �������� ������ ����
#define MAX_CASHITEM_LIST				( MAX_CASHITEM_LIST_CATEGORY * MAX_CASHITEM_LIST_PAGE * MAX_CASHITEM_LIST_PER_PAGE )		// ĳ���� ������ �ִ� ����
#define MAX_CASHITEM_LIST_IN_CATEGORY	( MAX_CASHITEM_LIST_PAGE * MAX_CASHITEM_LIST_PER_PAGE )		// ĳ�� ������ ī�װ��� �ִ� ������ ����
#define MAX_CASHITEM_LIST_BRANCH		100

enum
{
	CASHSHOP_SCRIPT_SERVEROPTION	= 0,
	CASHSHOP_SCRIPT_ITEMLIST		= 1
};

enum
{
	CASHSHOP_INTERFACE_CLOSE		= 0,
	CASHSHOP_INTERFACE_OPEN			= 1
};

//////////////////////////////////////////////////////////////////////////
// ������ ���� ��� �ڵ�
//////////////////////////////////////////////////////////////////////////

enum
{
	RESULT_SUCCESS						= 0,			// ���� ����
	RESULT_FAIL_NOT_ENOUGH_CASH			= 1,			// ���� ���� (ĳ�� �ܾ� ����)
	RESULT_FAIL_NOT_EMPTY_SPACE			= 2,			// ���� ���� (�κ��丮 ���� ����)
	RESULT_FAIL_NOT_FOUND_ITEM			= 3,			// ���� ���� (��ġ�ϴ� ������ ������ ����)
	RESULT_FAIL_ALREADY_HAVE_ITEM		= 4,			// ���� ���� (�ߺ� ���� �Ұ����� �������� ����)
	RESULT_FAIL_DB_ERROR				= 5,			// ���� ���� (DB ����)
	RESULT_FAIL_SHOPSERVER_DISCONNECT	= 6,			// ���� ���� (�� �������� ������ ��������)
	RESULT_FAIL_SHOP_ERROR				= 7,			// ���� ���� (�� �����κ��� �̻��� ���� Ȯ��)
	RESULT_FAIL_OPEN_CASHSHOP			= 8,			// ������ �� ���� ��û ����
	RESULT_FAIL_ETC						= 9,			// ���� ���� (��Ÿ)
#ifdef UPDATE_BUY_CASHITEM_ERROR_CODE_20090410	
	RESULT_FAIL_NOT_ENOUGH_MILEAGE		= 10,
	RESULT_FAIL_DONT_BUY_ITEM			= 11,
#endif	// UPDATE_BUY_CASHITEM_ERROR_CODE_20090410
};

// ĳ�� ���� ������ ����Ʈ ���������� ����� ���ؼ� 1����Ʈ�� ��ŷ�Ѵ�.
#pragma pack(1)

// ��� ����
typedef struct
{
	DWORD		dwPriceGuid;
	WORD		wItemSellPrice;
	BYTE		btItemSaleRate;
	BYTE		btItemSaleCount;
	DWORD		dwItemUseTime;
	// 12����Ʈ
}CASHSHOP_ITEMPRICE, *LPCASHSHOP_ITEMPRICE;

// ��ũ��Ʈ�� ���� �ε��� ĳ�� ������ ����Ʈ (������ �Ӽ�)
typedef struct
{
	BYTE		btItemType;						// �ڵ�
	WORD		wItemIndex;						// �ε���
	// �Ӽ�
	BYTE		btItemLevel;					// ����
	BYTE		btDurability;					// ������
	BYTE		btSkillOption;					// ��ų �ɼ�
	BYTE		btLuckOption;					// ��� �ɼ�
	BYTE		btAddOption;					// �߰� �ɼ�
	BYTE		btExOption;						// ���� �ɼ�
	BYTE		btItemInfo[ITEM_BUFFER_SIZE];	// ������ ������ Ŭ���̾�Ʈ�� ���� �� 7 ����Ʈ �������� ��ȯ
}CASHSHOP_ITEM_STATUS, *LPCASHSHOP_ITEM_STATUS;

// ������ ����Ʈ (������ �Ӽ� + ����)
typedef struct
{
	DWORD				dwItemGuid;						// �Ǹ� ������ȣ
	DWORD				dwPriceGuid;					// ���� ������ȣ
	BYTE				btCategoryCode;					// �� �ڵ�
	BYTE				btItemType;						// Ÿ��
	WORD				wItemIndex;						// �ε���
	BYTE				btItemLevel;					// ����
	BYTE				btItemSkillOpion;				// ��ų �ɼ�
	BYTE				btItemLuckOption;				// ��� �ɼ�
	BYTE				btItemAddOption;				// �߰� �ɼ�
	BYTE				btItemExOption;					// ������Ʈ �ɼ�
	BYTE				btItemSaleRatio;				// ���η�
	WORD				wItemPrice;						// ����
	BYTE				btItemDuration;					// ������ (����)
	BYTE				btSpecialOption;				// Ư�� �ɼ� (�ű�, ��Ʈ ������)
	BYTE				btItemUsedType;					// ��� Ÿ�� (1: ������ 2: ������ 3: �Ҹ� 4:������)
	BYTE				btItemInfo[ITEM_BUFFER_SIZE];	// ������ ������ Ŭ���̾�Ʈ�� ���� �� 7 ����Ʈ �������� ��ȯ
	DWORD				dwItemUsePeriod;				// ������ ��� �Ⱓ
	CItem				ItemInfo;						// ������ ������ ������ ���� CItem Ŭ����
	// 18����Ʈ
}CASHSHOP_ITEMLIST, *LPCASHSHOP_ITEMLIST;

typedef struct
{
	DWORD				dwItemGuid;						// �Ǹ� ������ȣ
	BYTE				btCategoryCode;					// �� �ڵ�
	BYTE				btItemDuration;					// ������ (����)
	BYTE				btItemSaleRatio;				// ���η�
	WORD				wItemPrice;						// ����
	BYTE				btSpecialOption;				// Ư�� �ɼ� (�ű�, ��Ʈ ������)
	BYTE				btItemInfo[ITEM_BUFFER_SIZE];	// ������ ������ Ŭ���̾�Ʈ�� ���� �� 7 ����Ʈ �������� ��ȯ
	DWORD				dwItemUsePeriod;				// ������ ��� �ð� (0�� ��� 
	// 17����Ʈ
}CASHSHOP_ITEMLIST_TO_PROTOCOL, *LPCASHSHOP_ITEMLIST_TO_PROTOCOL;

typedef struct
{
	DWORD				dwItemGuid;						// �Ǹ� ������ȣ
	int					iBranchType;					// �� Ÿ��
}CASHSHOP_BRANCH_ITEMLIST, *LPCASHSHOP_BRANCH_ITEMLST;

//////////////////////////////////////////////////////////////////////////
// [ĳ���� ���� ��û] 0xF5, 0x01
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;

	BYTE				btShopOpenType;			// ĳ�� �� �������̽� ���� ( 0:�ݱ�, 1:����)
	BYTE				btShopOpenAlready;		// ĳ�� ���� �� ���� �ִ� �� (0;����, 1:����)
}PMSG_REQ_CASHSHOPOPEN, *LPPMSG_REQ_CASHSHOPOPEN;

//////////////////////////////////////////////////////////////////////////
// [ĳ���� ���� ��û ���] 0xF5, 0x02
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;

	BYTE				btResult;				// ĳ�� �� ���� ��û ��� ( 0:����, 1:�ٸ� �������̽� ��� ��, 2:ĳ������ ������ �� ����, 3:�غ���� ����, 4:��Ʈ��ũ ���� )
}PMSG_ANS_CASHSHOPOPEN, *LPPMSG_ANS_CASHSHOPOPEN;

//////////////////////////////////////////////////////////////////////////
// [ĳ�� ����Ʈ �ܷ� ��û] 0xF5, 0x03
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;
}PMSG_REQ_CASHPOINT, *LPPMSG_REQ_CASHPOINT;

//////////////////////////////////////////////////////////////////////////
// [ĳ�� ����Ʈ �ܷ�] 0xF5, 0x04
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;

	int					iCashPoint;				// ĳ�� �ܷ�
}PMSG_ANS_CASHPOINT, *LPPMSG_ANS_CASHPOINT;

//////////////////////////////////////////////////////////////////////////
// [ĳ�� ������ ����Ʈ ��û] 0xF5, 0x05
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;

	BYTE				btCategoryIndex;		// ������ ����Ʈ ī�װ�
	BYTE				btPageIndex;			// ������ �ε���
	BYTE				btPageLoaded;			// �ش� �������� ����Ʈ�� �������� �ִ°� (0:����, 1:����)
}PMSG_REQ_CASHITEMLIST, *LPPMSG_REQ_CASHITEMLIST;

//////////////////////////////////////////////////////////////////////////
// [ĳ�� ������ ����Ʈ] 0xF5, 0x06
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2						head;

	BYTE							btCategoryIndex;		// ������ ����Ʈ ī�װ�
	BYTE							btPageIndex;			// ������ �ε���
	BYTE							btItemCount;			// �ش� �������� ������ ī��Ʈ
	CASHSHOP_ITEMLIST_TO_PROTOCOL	ItemInfo[MAX_CASHITEM_LIST_PER_PAGE];	// ������ ����Ʈ (�� ������)
	// 151����Ʈ
}PMSG_ANS_CASHITEMLIST, *LPPMSG_ANS_CASHITEMLIST;

//////////////////////////////////////////////////////////////////////////
// [ĳ�� ������ ���� ��û] 0xF5, 0x07
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2						head;

	DWORD							dwItemPriceGuid;		// ���� �������� ���� Guid
	BYTE							btCategoryIndex;		// ���� �������� ī�װ�
}PMSG_REQ_CASHITEM_BUY, *LPPMSG_REQ_CASHITEM_BUY;

//////////////////////////////////////////////////////////////////////////
// [ĳ�� ������ ���� ���] 0xF5, 0x08
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2						head;

	BYTE							btResult;				// ������ ���� ���
	// 1:����, 0:ĳ�� �ܷ� ����, 2:��Ÿ ����, 
}PMSG_ANS_CASHITEM_BUY, *LPPMSG_ANS_CASHITEM_BUY;

typedef map<int, LPCASHSHOP_ITEMLIST> MAP_CASHITEM_LIST;
typedef map<int, LPCASHSHOP_ITEMLIST>::iterator ITER_CASHITEM_LIST;

typedef map<int, LPCASHSHOP_ITEM_STATUS> MAP_CASHITEM_STATUS;
typedef map<int, LPCASHSHOP_ITEM_STATUS>::iterator ITER_CASHITEM_STATUS;

typedef map<int, LPOBJECTSTRUCT> MAP_OBJECTSTRUCT;
typedef map<int, LPOBJECTSTRUCT>::iterator ITER_OBJECTSTRUCT;

const int iCashItemListSize = sizeof( CASHSHOP_ITEMLIST );

class CCashShop  
{
public:
	CCashShop();
	virtual ~CCashShop();

	void Initialize();
	void Load(char* pchFilename);
	void LoadShopOption(char* pchFilename);
	void LoadTestScript(char* pchFilename);
	BOOL ConnectShopServer(char* pchIpAddress, int iPortNumber);
	BOOL ReConnectShopServer();

	void CheckShopServerConnectState();

	void CashShopOptioNReload();

	BOOL InsertItemStatus(LPCASHSHOP_ITEM_STATUS lpItemStatus);
	BOOL SetItemInfoFromShop(MSG_STOG_ITEM_LIST_ANS* lpMsg);
	BOOL SetPackageItemInfoFromShop(MSG_STOG_PACKAGE_LIST_ANS* lpMsg);
	void MakeItemList(LPCASHSHOP_ITEM_STATUS lpItemStatus, tblSellItem* lpItemSellInfo);
	BOOL CheckValidItemInfo(tblSellItem* lpItemInfo);
	LPCASHSHOP_ITEM_STATUS GetCashItemStatus(int iItemCode);
	BOOL IsGetAmountFromShopItem(int iItemCode);
	//------->
	// [������Ʈ] 5�� �κ�����ȭ ĳ���� ���Ͼ����� (grooving)
	#ifdef ADD_PARTIALY_CHARGE_SYSTEM_05_20080930
	BOOL IsSocketItem(int iItemCode);
	#endif
	//<-------
	// for cash item list
	LPCASHSHOP_ITEMLIST SearchItemList(int iItemGuid);

	// for User
	BOOL AddUser(LPOBJECTSTRUCT lpObj);
	BOOL DeleteUser(LPOBJECTSTRUCT lpObj);
	LPOBJECTSTRUCT SearchUser(int iUserGuid);

	// for Client
	BOOL CGCashShopOpen(LPOBJECTSTRUCT lpObj, LPPMSG_REQ_CASHSHOPOPEN lpMsg);
	BOOL CGCashPoint(LPOBJECTSTRUCT lpObj);
	BOOL GCCashItemListSend(LPOBJECTSTRUCT lpObj, LPPMSG_REQ_CASHITEMLIST lpMsg);
	void CGCashItemBuy(LPOBJECTSTRUCT lpObj, LPPMSG_REQ_CASHITEM_BUY lpMsg);
	void GCCashPoint(LPOBJECTSTRUCT lpObj, DWORD dwCashPoint);

	void GCCashItemBuyResult(LPOBJECTSTRUCT lpObj, BYTE btResult);

	BOOL CheckInventoryEmptySpace(LPOBJECTSTRUCT lpObj, LPCASHSHOP_ITEMLIST lpItemInfo);
	BOOL GiveBuyItemToInventory(LPOBJECTSTRUCT lpObj, int iItemGuid );

	BOOL CheckPeriodItemUsed(LPOBJECTSTRUCT lpObj, int iItemCode);
	BOOL CheckPeriodItem(int iItemCode);

	// for Shop Server
	void GSReqCashPoint(DWORD dwUserGuid);
	void GSReqCashItemList();
	void GSReqPackageItemList();
	void GSReqBuyCashItem(DWORD dwServerGuid, DWORD dwUserGuid, DWORD dwCharacterGuid, char* szCharacterName, DWORD dwCount, DWORD dwPriceGuid);
	void GSNtfBuyCashItem(MSG_GTOS_BUY_ITEM_NTF* pMsg);

	// for Branch Item List
	void GSReqBranchItemList();
	void SetBranchItem(DWORD dwItemGuid, int iBranchType);
	int GetBranchType(DWORD dwItemGuid);

	// for item list protocol
	void MakeItemListProtocol();
	LPBYTE GetItemList(int iCategory, int iPageCount);

	// �� ������ ��Ʈ�� �޽����� ó���ϱ� ���� ���� ���μ���
	static WNDPROC m_lpOldProc;
	static LRESULT ParentWndProc (HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);	
private:
	int		iCashItemCount;
	int		iCashItemPageNumber;

	BOOL	bCashItemListReload;

	MAP_OBJECTSTRUCT				MapUserObject;
	MAP_CASHITEM_LIST				MapCashItemList;
	MAP_CASHITEM_STATUS				MapCashItemStatus;
	CASHSHOP_ITEMLIST				CashItemList[MAX_CASHITEM_LIST_CATEGORY][MAX_CASHITEM_LIST_IN_CATEGORY];
	CASHSHOP_ITEMLIST_TO_PROTOCOL	CashItemListCompress[MAX_CASHITEM_LIST_CATEGORY][MAX_CASHITEM_LIST_IN_CATEGORY];
	PMSG_ANS_CASHITEMLIST			CashItemProtocol[MAX_CASHITEM_LIST_CATEGORY][MAX_CASHITEM_LIST_PAGE];
	int								iAddItemCountInCategory[MAX_CASHITEM_LIST_CATEGORY];
	int								iCashItemCountInCategory[MAX_CASHITEM_LIST_CATEGORY];

	CASHSHOP_BRANCH_ITEMLIST		BranchItemList[MAX_CASHITEM_LIST_BRANCH];
	int								iBranchItemCount;

	DWORD							dwCheckShopServerConnectStatePeriod;
};

extern wsShopServerCli g_ShopServerClient;
extern CCashShop g_CashShop;
extern BOOL g_bUseCashShop;
extern BOOL g_bConnectShopServer;
extern BOOL	g_bShopServerConnectState;
extern BOOL g_bUseLotteryEvent;
extern char g_ShopServerIP[16];
extern int	g_ShopserverPort;
extern BOOL g_bUseMoveMapBound;

extern void SGAnsCashPoint(MSG_STOG_USER_CASH_ANS* aRecv);
extern void SGAnsCashItemList(MSG_STOG_ITEM_LIST_ANS* aRecv);
extern void SGAnsPackageItemList(MSG_STOG_PACKAGE_LIST_ANS* aRecv);
extern void SGAnsBuyCashItem(MSG_STOG_BUY_ITEM_ANS* aRecv);
extern void SGAnsBranchItemList(MSG_STOG_BRANCH_ITEM_LIST_ANS* lpMsg);

extern BOOL IsCashItem(int iItemCode);

#if TESTSERVER == 1
extern void AddLog(char* pchLog, ...);
#endif

#ifdef MODIFY_RECONNECTSHOPSERVER_20080807
extern void GetTCPTableInfo();
#endif // MODIFY_RECONNECTSHOPSERVER_20080807

#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PREMIUMITEM_20090106
extern BOOL	IsPremiumItem(int iItemCode);
#endif //PREMIUMITEM_20090106

#endif // !defined(AFX_CASHSHOP_H__273C22CB_4E7F_40FB_BE1C_F41149A9BBD2__INCLUDED_)
