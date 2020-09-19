#pragma once
#pragma pack(1)
// ��� ����
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
	���� ������ ����
 */
enum eGIFT_STATE
{
	eGS_WAIT	= 0,	///> ���
	eGS_ACCEPT,			///> ���� ����
	eGS_REJECT,			///> ���� �ź�
	eGS_DELETE			///> ���� ����
};

/*
	������ ����
 */
enum eDEF_ITEM_TYPE 
{
	eTYPE_ITEM		= 1,
	eTYPE_PACKAGE
};

/*
	������ ����
 */
enum eBUY_TYPE
{
	eBT_CASH		= 1,
	eBT_NON_CASH	= 2
};
 
/**
	ERROR ����
 */
enum eRET_ERROR_CODE
{	
	eRET_FAIL,					///>  ����        
	eRET_SUCCESS,				///>  ����        
	eRET_NOT_ENOUGH_CASH = 8,   ///>  �ܾ� ����   
	eRET_DB_ERROR,				///>  Shop DB ����
	eRET_NOT_FOUND_ITEM,		///>  ���� ���� �ʴ� �Ǹ� ������
	eRET_LIMIT_BUY_COUNT_ERROR,	///>  �Ǹ� ���Ѽ� ����
	eRET_BUY_COOL_TIME_ERROR,	///>  ���� ��Ÿ�� ����
	eRET_INVALID_VALUE,			///>  �߸��� ���� ���
	eRET_INVALID_ID_VALUE		///>  �߸��� ID�� GUID���� ���
#ifdef UPDATE_BUY_CASHITEM_ERROR_CODE_20090410
	,eRET_NOT_ENOUGH_MILEAGE,	///>  ���ϸ��� ����
	eRET_DONT_BUY_ITEM,			///>  ��ǰ ���� �Ұ���(�۷ι� �������� Ư�� ���� ���� ������ ������ ���� ����)
#endif	// UPDATE_BUY_CASHITEM_ERROR_CODE_20090410	
};

/**
	�Ǹ� ������ ���� ���� record
 */
typedef struct sellItemPrice
{
	DWORD	dwPriceGuid;						///> �Ǹ� ���� ���� ��ȣ
	DWORD	dwItemGuid;							///> �Ǹ� ���� ��ȣ
	DWORD	dwUseTime;							///> (������:��¥ ������:�ð�)
	DWORD	dwAmount;							///> (�Ҹ�,������:����)
	DWORD	dwPrice;							///> �ǸŰ���
	DWORD	dwSellRate;							///> ������ ���� ������	
} tblSellItemPrice;

/**
	�Ǹ� ������ record
 */
typedef struct sellItem
{
	DWORD				dwItemGuid;							///> �Ǹ� ���� ��ȣ
	DWORD				dwItemCODE;							///> ������ ���� ��ȣ
	DWORD				dwCategoryID;						///> ī�װ� ���̵�
	DWORD				dwUseType;							///> ��� �Ӽ�(1: ������ 2: ������ 3: �Ҹ� 4:������)
	DWORD				dwBuyType; 							///> ���� ����(1: ĳ�� 2: ����)
	DWORD				dwCoolTime;							///> ���� ��Ÿ��
	double				dbVarEndDate;						///> ���� ����
	DWORD				dwLimitSellCount;					///> ������ �� �Ǹ� ����
	DWORD				dwState;							///> ������ ����(1:�Ǹ��� 2:�ǸŴ����)
	char 				szItemName[MAX_ITEM_NAME_LENGTH];	///> ������ �̸�
	char 				szItemDesc[MAX_ITEM_DESC_LENGTH];	///> ������ ����
	DWORD				dwPriceCount;
	tblSellItemPrice	itemPrice[MAX_ITEM_PRICE_SIZE];		///> ���� ����
} tblSellItem;

/**
	�Ǹ� ��Ű�� record
 */
typedef struct	sellPackage
{	
	DWORD				dwPackageGuid;							///> ��Ű�� ������ ���� ��ȣ
	tblSellItemPrice	itemPrice;								///> ��Ű�� ���� ����
	DWORD				dwImageCode;							///> ��Ű�� �̹��� �ڵ�
	DWORD				dwCategory;								///> ī�װ� ���̵�
	DWORD				dwBuyType;								///> ���ż���(1: ĳ�� 2: ����)
	DWORD				dwLimitSellCount;						///> ������ �� �Ǹ� ����
	DWORD				dwInItemCount;							///> ���Ե� ������ ����
	double				dbVarEndDate;							///> ���� ����
	DWORD				dwState;								///> ������ ����(1:�Ǹ��� 2:�ǸŴ����)
	tblSellItemPrice	inItemPrices[MAX_PACKAGE_ITEM_SIZE];	///> ���Ե� ������ ���� ���� ����Ʈ
	char 				szPackageName[MAX_PACKAGE_NAME_LENGTH];	///> ��Ű�� �̸�
	char 				szPackageDesc[MAX_PACKAGE_DESC_LENGTH];	///> ��Ű�� ����
} tblSellPackage;

/**
	������ ���� �̷� record
 */
typedef struct buyItem
{
	DWORD	dwServerGuid;							///> ���� ������ ���� ���� Ű
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];	///> ĳ���� �̸�
	char	szItemName[MAX_ITEM_NAME_LENGTH];		///> ������ �̸�
	DWORD	dwPriceGuid;							///> ���Ź�ȣ GUID
	DWORD	dwBuyType;								///> ���� ����
	DWORD	dwPrice;								///> ���� ����
	double	dbVarBuyDate;							///> ������
} tblBuyItem;

/**
	������ �з� ��� record
 */
typedef struct itemBranch
{
	DWORD	dwBranchID;
	DWORD	dwGuid;		///> ������ �� ���� ������ Guid / ��Ű���� ���� ��Ű�� Guid
	DWORD	dwItemType;	///> ������ Ÿ��(1:������ 2:��Ű��)
} tblItemBranch;

/**
	���� ��û Transzction record
 */
typedef struct buyItemTransaction
{
	DWORD	dwTransactionGuid;		///> ���� ��û Transaction Guid
	DWORD	dwPriceGuid;			///> �Ǹ� ���� ���� ��ȣ	
	DWORD	dwResult;				///> ���
						/*
							0 : ���� 
							1 : ����
							8 : �ܾ� ����
							9 : Shop DB ����
						*/
} tblBuyItemTransaction;

/**
	���� ���� ��� record
 */
typedef struct recvGiftItem
{
	DWORD 	dwGiftItemID;								///> GiftItemID
	DWORD	dwBuyItemID;								///> BuyItemID
	DWORD	dwPriceGuid;								///> PriceGuid
	char	szSendCharName[MAX_CHARACTER_NAME_LENGTH];	///> ���� ����� ĳ���� �̸�
	char	szSendMessage[MAX_GIFT_MESSAGE_LENGTH];		///> ���� �޼���
	DWORD	dwState;									///> ����
	char	szItemName[MAX_ITEM_NAME_LENGTH];			///> ������ �̸�
	double	dbSendDate;									///> ���� ��¥
} tblRecvGiftItem;

/**
	���� ���� ��� record
 */
typedef struct sendGiftItem
{
	DWORD 	dwGiftItemID;								///> GiftItemID
	DWORD	dwBuyItemID;								///> BuyItemID
	DWORD	dwPriceGuid;								///> PriceGuid
	char	szSendMessage[MAX_GIFT_MESSAGE_LENGTH];		///> ���� �޼���
	char	szRecvCharName[MAX_CHARACTER_NAME_LENGTH];	///> ���� ����� ĳ���� �̸�
	char	szRecvMessage[MAX_GIFT_MESSAGE_LENGTH];		///> ���� �޼���
	DWORD	dwState;									///> ����
	char	szItemName[MAX_ITEM_NAME_LENGTH];			///> ������ �̸�
	double	dbSendDate;									///> ���� ��¥
} tblSendGiftItem;
#pragma pack()
