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

// 캐쉬샵 서버 메시지
#define WM_ASS_SHOPSERVERMSG	( WM_USER+ 30 )

#define SHOPSERVER_DEFAULT_PORT	55990

#define MAX_BUY_ITEM_AT_ONCE			1

// 캐쉬삽에 들어갈 수 있는 최대 아이템 리스트 개수
#define MAX_CASHITEM_LIST_CATEGORY		4		// 캐쉬샵 아이템 리스트 최대 카테고리 수
#define MAX_CASHITEM_LIST_PAGE			15		// 캐쉬샵 아이템 리스트 최대 페이지 개수
#define MAX_CASHITEM_LIST_PER_PAGE		9		// 캐쉬샵 아이템 페이지당 아이템 개수
#define MAX_CASHITEM_LIST				( MAX_CASHITEM_LIST_CATEGORY * MAX_CASHITEM_LIST_PAGE * MAX_CASHITEM_LIST_PER_PAGE )		// 캐쉬샵 아이템 최대 개수
#define MAX_CASHITEM_LIST_IN_CATEGORY	( MAX_CASHITEM_LIST_PAGE * MAX_CASHITEM_LIST_PER_PAGE )		// 캐쉬 아이템 카테고리당 최대 아이템 개수
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
// 아이템 구매 결과 코드
//////////////////////////////////////////////////////////////////////////

enum
{
	RESULT_SUCCESS						= 0,			// 구매 성공
	RESULT_FAIL_NOT_ENOUGH_CASH			= 1,			// 구매 실패 (캐쉬 잔액 부족)
	RESULT_FAIL_NOT_EMPTY_SPACE			= 2,			// 구매 실패 (인벤토리 공간 없음)
	RESULT_FAIL_NOT_FOUND_ITEM			= 3,			// 구매 실패 (일치하는 아이템 정보가 없음)
	RESULT_FAIL_ALREADY_HAVE_ITEM		= 4,			// 구매 실패 (중복 구매 불가능한 아이템을 소지)
	RESULT_FAIL_DB_ERROR				= 5,			// 구매 실패 (DB 오류)
	RESULT_FAIL_SHOPSERVER_DISCONNECT	= 6,			// 구매 실패 (샵 서버와의 연결이 끊어졌음)
	RESULT_FAIL_SHOP_ERROR				= 7,			// 구매 실패 (샵 서버로부터 이상한 에러 확인)
	RESULT_FAIL_OPEN_CASHSHOP			= 8,			// 아이템 샵 오픈 요청 실패
	RESULT_FAIL_ETC						= 9,			// 구매 실패 (기타)
#ifdef UPDATE_BUY_CASHITEM_ERROR_CODE_20090410	
	RESULT_FAIL_NOT_ENOUGH_MILEAGE		= 10,
	RESULT_FAIL_DONT_BUY_ITEM			= 11,
#endif	// UPDATE_BUY_CASHITEM_ERROR_CODE_20090410
};

// 캐쉬 상점 아이템 리스트 프로토콜을 만들기 위해서 1바이트로 팩킹한다.
#pragma pack(1)

// 사용 안함
typedef struct
{
	DWORD		dwPriceGuid;
	WORD		wItemSellPrice;
	BYTE		btItemSaleRate;
	BYTE		btItemSaleCount;
	DWORD		dwItemUseTime;
	// 12바이트
}CASHSHOP_ITEMPRICE, *LPCASHSHOP_ITEMPRICE;

// 스크립트로 부터 로딩된 캐쉬 아이템 리스트 (아이템 속성)
typedef struct
{
	BYTE		btItemType;						// 코드
	WORD		wItemIndex;						// 인덱스
	// 속성
	BYTE		btItemLevel;					// 레벨
	BYTE		btDurability;					// 내구도
	BYTE		btSkillOption;					// 스킬 옵션
	BYTE		btLuckOption;					// 행운 옵션
	BYTE		btAddOption;					// 추가 옵션
	BYTE		btExOption;						// 엑셀 옵션
	BYTE		btItemInfo[ITEM_BUFFER_SIZE];	// 아이템 정보를 클라이언트에 보내 줄 7 바이트 포멧으로 변환
}CASHSHOP_ITEM_STATUS, *LPCASHSHOP_ITEM_STATUS;

// 아이템 리스트 (아이템 속성 + 가격)
typedef struct
{
	DWORD				dwItemGuid;						// 판매 고유번호
	DWORD				dwPriceGuid;					// 가격 고유번호
	BYTE				btCategoryCode;					// 탭 코드
	BYTE				btItemType;						// 타입
	WORD				wItemIndex;						// 인덱스
	BYTE				btItemLevel;					// 레벨
	BYTE				btItemSkillOpion;				// 스킬 옵션
	BYTE				btItemLuckOption;				// 행운 옵션
	BYTE				btItemAddOption;				// 추가 옵션
	BYTE				btItemExOption;					// 엑셀런트 옵션
	BYTE				btItemSaleRatio;				// 할인률
	WORD				wItemPrice;						// 가격
	BYTE				btItemDuration;					// 내구도 (개수)
	BYTE				btSpecialOption;				// 특수 옵션 (신규, 히트 아이템)
	BYTE				btItemUsedType;					// 사용 타입 (1: 정액제 2: 정량제 3: 소모성 4:무제한)
	BYTE				btItemInfo[ITEM_BUFFER_SIZE];	// 아이템 정보를 클라이언트에 보내 줄 7 바이트 포멧으로 변환
	DWORD				dwItemUsePeriod;				// 아이템 사용 기간
	CItem				ItemInfo;						// 아이템 정보를 얻어오기 위한 CItem 클래스
	// 18바이트
}CASHSHOP_ITEMLIST, *LPCASHSHOP_ITEMLIST;

typedef struct
{
	DWORD				dwItemGuid;						// 판매 고유번호
	BYTE				btCategoryCode;					// 탭 코드
	BYTE				btItemDuration;					// 내구도 (개수)
	BYTE				btItemSaleRatio;				// 할인률
	WORD				wItemPrice;						// 가격
	BYTE				btSpecialOption;				// 특수 옵션 (신규, 히트 아이템)
	BYTE				btItemInfo[ITEM_BUFFER_SIZE];	// 아이템 정보를 클라이언트에 보내 줄 7 바이트 포멧으로 변환
	DWORD				dwItemUsePeriod;				// 아이템 사용 시간 (0일 경우 
	// 17바이트
}CASHSHOP_ITEMLIST_TO_PROTOCOL, *LPCASHSHOP_ITEMLIST_TO_PROTOCOL;

typedef struct
{
	DWORD				dwItemGuid;						// 판매 고유번호
	int					iBranchType;					// 뷰 타입
}CASHSHOP_BRANCH_ITEMLIST, *LPCASHSHOP_BRANCH_ITEMLST;

//////////////////////////////////////////////////////////////////////////
// [캐쉬샵 오픈 요청] 0xF5, 0x01
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;

	BYTE				btShopOpenType;			// 캐쉬 샵 인터페이스 상태 ( 0:닫기, 1:열기)
	BYTE				btShopOpenAlready;		// 캐쉬 샵을 연 적이 있는 가 (0;없음, 1:있음)
}PMSG_REQ_CASHSHOPOPEN, *LPPMSG_REQ_CASHSHOPOPEN;

//////////////////////////////////////////////////////////////////////////
// [캐쉬샵 오픈 요청 결과] 0xF5, 0x02
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;

	BYTE				btResult;				// 캐쉬 샵 오픈 요청 결과 ( 0:성공, 1:다른 인터페이스 사용 중, 2:캐쉬샵에 연결할 수 없음, 3:준비되지 않음, 4:네트워크 오류 )
}PMSG_ANS_CASHSHOPOPEN, *LPPMSG_ANS_CASHSHOPOPEN;

//////////////////////////////////////////////////////////////////////////
// [캐쉬 포인트 잔량 요청] 0xF5, 0x03
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;
}PMSG_REQ_CASHPOINT, *LPPMSG_REQ_CASHPOINT;

//////////////////////////////////////////////////////////////////////////
// [캐쉬 포인트 잔량] 0xF5, 0x04
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;

	int					iCashPoint;				// 캐쉬 잔량
}PMSG_ANS_CASHPOINT, *LPPMSG_ANS_CASHPOINT;

//////////////////////////////////////////////////////////////////////////
// [캐쉬 아이템 리스트 요청] 0xF5, 0x05
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2			head;

	BYTE				btCategoryIndex;		// 아이템 리스트 카테고리
	BYTE				btPageIndex;			// 페이지 인덱스
	BYTE				btPageLoaded;			// 해당 페이지의 리스트를 받은적이 있는가 (0:있음, 1:없음)
}PMSG_REQ_CASHITEMLIST, *LPPMSG_REQ_CASHITEMLIST;

//////////////////////////////////////////////////////////////////////////
// [캐쉬 아이템 리스트] 0xF5, 0x06
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2						head;

	BYTE							btCategoryIndex;		// 아이템 리스트 카테고리
	BYTE							btPageIndex;			// 페이지 인덱스
	BYTE							btItemCount;			// 해당 페이지의 아이템 카운트
	CASHSHOP_ITEMLIST_TO_PROTOCOL	ItemInfo[MAX_CASHITEM_LIST_PER_PAGE];	// 아이템 리스트 (한 페이지)
	// 151바이트
}PMSG_ANS_CASHITEMLIST, *LPPMSG_ANS_CASHITEMLIST;

//////////////////////////////////////////////////////////////////////////
// [캐쉬 아이템 구매 요청] 0xF5, 0x07
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2						head;

	DWORD							dwItemPriceGuid;		// 구매 아이템의 가격 Guid
	BYTE							btCategoryIndex;		// 구매 아이템의 카테고리
}PMSG_REQ_CASHITEM_BUY, *LPPMSG_REQ_CASHITEM_BUY;

//////////////////////////////////////////////////////////////////////////
// [캐쉬 아이템 구매 결과] 0xF5, 0x08
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	PBMSG_HEAD2						head;

	BYTE							btResult;				// 아이템 구매 결과
	// 1:성공, 0:캐쉬 잔량 부족, 2:기타 실패, 
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
	// [업데이트] 5차 부분유료화 캐쉬샵 소켓아이템 (grooving)
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

	// 샵 서버의 네트웍 메시지를 처리하기 위한 서브 프로세스
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
