#pragma once
#pragma pack(1)
// 상수 정의
#define MAX_RECORD_SIZE						1024
#define MAX_ITEM_NAME_LENGTH				64
#define MAX_ITEM_DESC_LENGTH        		128
#define MAX_PACKAGE_NAME_LENGTH     		64
#define MAX_PACKAGE_DESC_LENGTH     		128
#define MAX_ITEM_SIZE               		128
#define MAX_PACKAGE_SIZE            		96
#define MAX_BRANCH_ITEM_SIZE	       		512
#define MAX_BUY_ITEM_SIZE					256
#define MAX_PACKAGE_ITEM_SIZE       		16
#define MAX_ITEM_PRICE_SIZE					8
#define MAX_CHARACTER_NAME_LENGTH			96
#define MAX_BUY_REQ_SIZE					16
#define MAX_GIFT_MESSAGE_LENGTH				96
#define MAX_GIFT_ITEM_SIZE					64

#define E_EXCEPTION		(DWORD)(~0)

/*
	선물 아이템 상태
 */
enum eGIFT_STATE
{
	eGS_WAIT	= 0,	///> 대기
	eGS_ACCEPT,			///> 선물 승인
	eGS_REJECT,			///> 선물 거부
	eGS_DELETE			///> 선물 삭제
};

/*
	아이템 구분
 */
enum eDEF_ITEM_TYPE 
{
	eTYPE_ITEM		= 1,
	eTYPE_PACKAGE
};

/*
	아이템 구분
 */
enum eBUY_TYPE
{
	eBT_CASH		= 1,
	eBT_NON_CASH	= 2
};
 
/**
	ERROR 정의
 */
enum eRET_ERROR_CODE
{	
	eRET_FAIL,					///>  실패        
	eRET_SUCCESS,				///>  성공        
	eRET_NOT_ENOUGH_CASH = 8,   ///>  잔액 부족   
	eRET_DB_ERROR,				///>  Shop DB 오류
	eRET_NOT_FOUND_ITEM,		///>  존재 하지 않는 판매 아이템
	eRET_LIMIT_BUY_COUNT_ERROR,	///>  판매 제한수 제한
	eRET_BUY_COOL_TIME_ERROR,	///>  구매 쿨타임 제한
	eRET_INVALID_VALUE,			///>  잘못된 값인 경우
	eRET_INVALID_ID_VALUE		///>  잘못된 ID및 GUID값인 경우
#ifdef UPDATE_BUY_CASHITEM_ERROR_CODE_20090410
	,eRET_NOT_ENOUGH_MILEAGE,	///>  마일리지 부족
	eRET_DONT_BUY_ITEM,			///>  상품 구매 불가능(글로벌 버전에서 특정 연령 이하 유저의 아이템 구매 제한)
#endif	// UPDATE_BUY_CASHITEM_ERROR_CODE_20090410	
};

/**
	판매 아이템 가격 정보 record
 */
typedef struct sellItemPrice
{
	DWORD	dwPriceGuid;						///> 판매 가격 고유 번호
	DWORD	dwItemGuid;							///> 판매 고유 번호
	DWORD	dwUseTime;							///> (정액제:날짜 정량제:시간)
	DWORD	dwAmount;							///> (소모성,무제한:수량)
	DWORD	dwPrice;							///> 판매가격
	DWORD	dwSellRate;							///> 아이템 세일 비율값	
} tblSellItemPrice;

/**
	판매 아이템 record
 */
typedef struct sellItem
{
	DWORD				dwItemGuid;							///> 판매 고유 번호
	DWORD				dwItemCODE;							///> 아이템 고유 번호
	DWORD				dwCategoryID;						///> 카테고리 아이디
	DWORD				dwUseType;							///> 사용 속성(1: 정액제 2: 정량제 3: 소모성 4:무제한)
	DWORD				dwBuyType; 							///> 구매 수단(1: 캐쉬 2: 하임)
	DWORD				dwCoolTime;							///> 구매 쿨타임
	double				dbVarEndDate;						///> 만료 일자
	DWORD				dwLimitSellCount;					///> 아이템 총 판매 수량
	DWORD				dwState;							///> 아이템 상태(1:판매중 2:판매대기중)
	char 				szItemName[MAX_ITEM_NAME_LENGTH];	///> 아이템 이름
	char 				szItemDesc[MAX_ITEM_DESC_LENGTH];	///> 아이템 설명
	DWORD				dwPriceCount;
	tblSellItemPrice	itemPrice[MAX_ITEM_PRICE_SIZE];		///> 가격 정보
} tblSellItem;

/**
	판매 패키지 record
 */
typedef struct	sellPackage
{	
	DWORD				dwPackageGuid;							///> 패키지 아이템 고유 번호
	tblSellItemPrice	itemPrice;								///> 패키지 가격 정보
	DWORD				dwImageCode;							///> 패키지 이미지 코드
	DWORD				dwCategory;								///> 카테고리 아이디
	DWORD				dwBuyType;								///> 구매수단(1: 캐쉬 2: 하임)
	DWORD				dwLimitSellCount;						///> 아이템 총 판매 수량
	DWORD				dwInItemCount;							///> 포함된 아이템 갯수
	double				dbVarEndDate;							///> 만료 일자
	DWORD				dwState;								///> 아이템 상태(1:판매중 2:판매대기중)
	tblSellItemPrice	inItemPrices[MAX_PACKAGE_ITEM_SIZE];	///> 포함된 아이템 가격 정보 리스트
	char 				szPackageName[MAX_PACKAGE_NAME_LENGTH];	///> 패키지 이름
	char 				szPackageDesc[MAX_PACKAGE_DESC_LENGTH];	///> 패키지 설명
} tblSellPackage;

/**
	아이템 구매 이력 record
 */
typedef struct buyItem
{
	DWORD	dwServerGuid;							///> 월드 구별을 위한 서버 키
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];	///> 캐릭터 이름
	char	szItemName[MAX_ITEM_NAME_LENGTH];		///> 아이템 이름
	DWORD	dwPriceGuid;							///> 구매번호 GUID
	DWORD	dwBuyType;								///> 구매 수단
	DWORD	dwPrice;								///> 구매 가격
	double	dbVarBuyDate;							///> 구매일
} tblBuyItem;

/**
	아이템 분류 목록 record
 */
typedef struct itemBranch
{
	DWORD	dwBranchID;
	DWORD	dwGuid;		///> 아이템 인 경우는 아이템 Guid / 패키지인 경우는 패키지 Guid
	DWORD	dwItemType;	///> 아이템 타입(1:아이템 2:패키지)
} tblItemBranch;

/**
	구매 요청 Transzction record
 */
typedef struct buyItemTransaction
{
	DWORD	dwTransactionGuid;		///> 구매 요청 Transaction Guid
	DWORD	dwPriceGuid;			///> 판매 가격 고유 번호	
	DWORD	dwResult;				///> 결과
						/*
							0 : 실패 
							1 : 성공
							8 : 잔액 부족
							9 : Shop DB 오류
						*/
} tblBuyItemTransaction;

/**
	받은 선물 목록 record
 */
typedef struct recvGiftItem
{
	DWORD 	dwGiftItemID;								///> GiftItemID
	DWORD	dwBuyItemID;								///> BuyItemID
	DWORD	dwPriceGuid;								///> PriceGuid
	char	szSendCharName[MAX_CHARACTER_NAME_LENGTH];	///> 보낸 사용자 캐릭터 이름
	char	szSendMessage[MAX_GIFT_MESSAGE_LENGTH];		///> 보낸 메세지
	DWORD	dwState;									///> 상태
	char	szItemName[MAX_ITEM_NAME_LENGTH];			///> 아이템 이름
	double	dbSendDate;									///> 보낸 날짜
} tblRecvGiftItem;

/**
	보낸 선물 목록 record
 */
typedef struct sendGiftItem
{
	DWORD 	dwGiftItemID;								///> GiftItemID
	DWORD	dwBuyItemID;								///> BuyItemID
	DWORD	dwPriceGuid;								///> PriceGuid
	char	szSendMessage[MAX_GIFT_MESSAGE_LENGTH];		///> 보낸 메세지
	char	szRecvCharName[MAX_CHARACTER_NAME_LENGTH];	///> 받은 사용자 캐릭터 이름
	char	szRecvMessage[MAX_GIFT_MESSAGE_LENGTH];		///> 받은 메세지
	DWORD	dwState;									///> 상태
	char	szItemName[MAX_ITEM_NAME_LENGTH];			///> 아이템 이름
	double	dbSendDate;									///> 보낸 날짜
} tblSendGiftItem;
#pragma pack()
