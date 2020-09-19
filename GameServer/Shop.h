// Shop.h: interface for the CShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOP_H__2B05C5E5_AB37_4B92_AAB1_36CBA13505CD__INCLUDED_)
#define AFX_SHOP_H__2B05C5E5_AB37_4B92_AAB1_36CBA13505CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SHOPITEM			(8*15)

class CShop
{
public :
	BYTE	ShopInventoryMap[MAX_SHOPITEM];
	int		ItemCount;						// 아이템 개수
	
	CItem	m_item[MAX_SHOPITEM];	// 상점에서 아이템들..

	BYTE	SendItemData[MAX_SHOPITEM*(ITEM_BUFFER_SIZE+1)];	// 상점아이템들을 미리 패킷화해 만들어두는 버퍼
	int		SendItemDataLen;				// 버퍼크기
	
	
	
public:

	
	CShop();
	virtual ~CShop();

	void Init();
	int  LoadShopItem(char *filename);

#ifdef SCRIPT_DECODE_WORK	
	int LoadShopItemDecode(char *filename);
#endif

#ifdef AUTH_GAMESERVER		
	int LoadShopItem(int Shopnumber);
#endif

	BOOL InsertItem(int type, int index, int level, int dur, int op1, int op2, int op3);
	int  InentoryMapCheck(int sx, int sy, int width, int height);
};

extern BOOL ShopDataLoad();

#endif // !defined(AFX_SHOP_H__2B05C5E5_AB37_4B92_AAB1_36CBA13505CD__INCLUDED_)
