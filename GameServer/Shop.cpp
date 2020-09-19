// Shop.cpp: implementation of the CShop class.
// 상점 아이템 제어 클래스
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Common\\zzzitem.h"
#include "..\\Common\\winUtil.h"
#include "..\\Include\\ReadScript.h"
#include "Dirpath.h"
#include "Shop.h"

#ifdef SCRIPT_DECODE_WORK	
#include "..\\common\\WZScriptEncode.h"
#endif

#ifdef AUTH_GAMESERVER
#include "..\\common\\WzMemScript.h"
#include "GameServerAuth.h"
#endif

#ifdef MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808
#include "ItemSocketOptionSystem.h"
#endif // MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808

extern CDirPath		gDirPath;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CShop			ShopC[MAX_SHOP];

CShop::CShop()
{
}

CShop::~CShop()
{

}

//----------------------------------------------------------------------------
// 상점 정보를 초기화 한다.
//----------------------------------------------------------------------------
void CShop::Init()
{
	SendItemDataLen  = 0;
	ItemCount = 0;
	ZeroMemory(ShopInventoryMap, MAX_SHOPITEM);
}

//----------------------------------------------------------------------------
// 아이템을 상점 인벤토리에 추가한다.
//----------------------------------------------------------------------------
BOOL CShop::InsertItem(int type, int index, int level, int dur, int op1, int op2, int op3)
{
	int itemp;
	int width, height;
	int x, y;
	int blank=-1;

	itemp = MAKE_ITEMNUM( type, index );
	if( itemp < 0 ) 
	{	
		MsgBox(lMsg.Get(565), __FILE__, __LINE__, type, index);
		return FALSE;
	}

	ItemGetSize( itemp, width, height);

	if( width < 0 || height < 0 ) 
	{
		MsgBox(lMsg.Get(566), __FILE__, __LINE__);
		return FALSE;
	}

	for( y=0; y<15; y++)
	{
		for( x=0; x<8; x++)
		{
			// 비어있는지 체크
			if( ShopInventoryMap[(y*8)+x] == 0 )
			{
				blank = InentoryMapCheck( x, y, width, height);
				if( blank >= 0 ) goto SearchOk;
			}
		}
	}

	if( blank < 0 ) 
	{
		MsgBox("error-L2 : %s %d",__FILE__, __LINE__);
		return FALSE;
	}

SearchOk :	// GO TO

	m_item[blank].m_Level = level;

	if( dur == 0 )
	{
		dur = ItemGetDurability(MAKE_ITEMNUM(type, index), level, 0, 0);
	}

	m_item[blank].m_Durability = (float)dur;
	m_item[blank].Convert(itemp, op1, op2, op3);	
	m_item[blank].Value();

#ifdef MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808
	if( g_SocketOptionSystem.IsEnableSocketItem( itemp ) == true )
	{
		// 소켓 아이템인 경우
		g_SocketOptionSystem.MakeSocketSlot( &m_item[blank], g_ShopBuySocketItemSlotCount );
	}
#endif // MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808

	// 미리패킷 데이터를 만든다.
	SendItemData[SendItemDataLen] = blank;
	SendItemDataLen += 1;
	ItemByteConvert( (SendItemData+SendItemDataLen), m_item[blank] );

	SendItemDataLen += ITEM_BUFFER_SIZE;

	ItemCount++;
	return TRUE;
}

//----------------------------------------------------------------------------
// 아이템을 넣을 자리가 있는지 체크
//----------------------------------------------------------------------------
int CShop::InentoryMapCheck(int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;

	if( (sx+width) > 8 ) // 가로 크기를 넘었으면
	{
		return -1;
	}
	if( (sy+height) > 15 ) {
		return -1;
	}
	
	for( y=0; y<height; y++)
	{		
		for(x=0; x<width; x++)
		{
			if( ShopInventoryMap[((sy+y)*8)+(sx+x)] ) 
			{
				blank++;
				break;
			}
		}
	}
	if( blank == 0 ) 
	{
		for( y=0; y<height; y++)
		{
			for(x=0; x<width; x++)
			{
				ShopInventoryMap[((sy+y)*8)+(sx+x)] = 1;
			}
		}
		return ((sy*8)+sx);
	}
	return -1;
}

//----------------------------------------------------------------------------
// 사점에서 사용하게될 아이템들을 파일에서 읽어온다.
//----------------------------------------------------------------------------

#ifdef SCRIPT_DECODE_WORK
int CShop::LoadShopItemDecode(char *filename)
{
	Init();

	CWZScriptEncode		Encoder;
	if( !Encoder.Open(filename) )
	{
		MsgBox("Shop data load error %s", filename);
		return FALSE;
	}

	WZSMDToken Token;
	
	int type, index, level, dur;
	int op1, op2, op3;
	while(true)
	{
		Token = Encoder.GetToken();
		if(Token == END) break;
		if(Token == T_NUMBER)
		{
			type  = Encoder.GetNumber(); 		
			Token = Encoder.GetToken();	index = Encoder.GetNumber();		
			Token = Encoder.GetToken();	level = Encoder.GetNumber();
			Token = Encoder.GetToken();	dur   = Encoder.GetNumber();
			Token = Encoder.GetToken();	op1   = Encoder.GetNumber();
			Token = Encoder.GetToken();	op2   = Encoder.GetNumber();
			Token = Encoder.GetToken();	op3   = Encoder.GetNumber();

			if( InsertItem(type, index, level, dur, op1, op2, op3) == FALSE )
			{
				MsgBox("error-L3 : %s %s %d", filename, __FILE__, __LINE__);
			}
		}
	}
	
	return TRUE;
}
#endif

int CShop::LoadShopItem(char *filename)
{
	Init();

	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("Shop data load error %s", filename);
		return FALSE;
	}

	SMDToken Token;

	int type, index, level, dur;
	int op1, op2, op3;
	while(true)
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			type  = (int)TokenNumber; 		
			Token = (*GetToken)();	index = (int)TokenNumber;		
			Token = (*GetToken)();	level = (int)TokenNumber;
			Token = (*GetToken)();	dur   = (int)TokenNumber;
			Token = (*GetToken)();	op1   = (int)TokenNumber;
			Token = (*GetToken)();	op2   = (int)TokenNumber;
			Token = (*GetToken)();	op3   = (int)TokenNumber;

			if( InsertItem(type, index, level, dur, op1, op2, op3) == FALSE )
			{
				MsgBox("error-L3 : %s %s %d", filename, __FILE__, __LINE__);
			}
		}
	}
	fclose(SMDFile);
	
	return TRUE;
}


#ifdef AUTH_GAMESERVER	
int CShop::LoadShopItem(int Shopnumber)
{
	int DataBufferSize;
	char* DataBuffer;
	
	gGameServerAuth.RequestData(Shopnumber);
	DataBufferSize = gGameServerAuth.GetDataBufferSize();
	DataBuffer = gGameServerAuth.GetDataBuffer();

	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(DataBuffer, DataBufferSize);

	Init();
	
	SMDToken Token;

	int type, index, level, dur;
	int op1, op2, op3;
	while(true)
	{	
		Token = WzMemScript.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			type  = WzMemScript.GetNumber(); 					
			Token = WzMemScript.GetToken();	index = WzMemScript.GetNumber();		
			Token = WzMemScript.GetToken();	level = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();	dur   = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();	op1   = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();	op2   = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();	op3   = WzMemScript.GetNumber();

			if( InsertItem(type, index, level, dur, op1, op2, op3) == FALSE )
			{
				MsgBox("error-L3 : Shop %d", Shopnumber);
			}
		}
	}
	return TRUE;
}
#endif

// 상점데이터를 로딩한다.
BOOL ShopDataLoad()
{

#ifdef AUTH_GAMESERVER	
	ShopC[0].LoadShopItem(FILE_SHOP0);
	ShopC[1].LoadShopItem(FILE_SHOP1);
	ShopC[2].LoadShopItem(FILE_SHOP2);
	ShopC[3].LoadShopItem(FILE_SHOP3);
	ShopC[4].LoadShopItem(FILE_SHOP4);
	ShopC[5].LoadShopItem(FILE_SHOP5);
	ShopC[6].LoadShopItem(FILE_SHOP6);
	ShopC[7].LoadShopItem(FILE_SHOP7);
	ShopC[8].LoadShopItem(FILE_SHOP8);
	ShopC[9].LoadShopItem(FILE_SHOP9);
	ShopC[10].LoadShopItem(FILE_SHOP10);
	#ifdef ADD_SHOP_11_12	
		ShopC[11].LoadShopItem(FILE_SHOP11);
		ShopC[12].LoadShopItem(FILE_SHOP12);
	#endif
#ifdef ADD_FIRECRACKER_NPC_20070306
		ShopC[13].LoadShopItem(FILE_SHOP13);
#endif
#ifdef ADD_SHOP_14_20070308
			ShopC[14].LoadShopItem(FILE_SHOP14);
#endif		
#ifdef ADD_SEASON_3_NEW_MAP_20070910
		ShopC[15].LoadShopItem(FILE_SHOP15);
		ShopC[16].LoadShopItem(FILE_SHOP16);
		ShopC[17].LoadShopItem(FILE_SHOP17);
#endif
#ifdef ADD_GAMBLING_20090120
		ShopC[18].LoadShopItem(FILE_SHOP18);
#endif // ADD_GAMBLING_20090120
	
#else
	#ifdef SCRIPT_DECODE_WORK
			ShopC[0].LoadShopItemDecode(gDirPath.GetNewPath("shop0.cwz"));
			ShopC[1].LoadShopItemDecode(gDirPath.GetNewPath("shop1.cwz"));
			ShopC[2].LoadShopItemDecode(gDirPath.GetNewPath("shop2.cwz"));
			ShopC[3].LoadShopItemDecode(gDirPath.GetNewPath("shop3.cwz"));
			ShopC[4].LoadShopItemDecode(gDirPath.GetNewPath("shop4.cwz"));	
			ShopC[5].LoadShopItemDecode(gDirPath.GetNewPath("shop5.cwz"));
			ShopC[6].LoadShopItemDecode(gDirPath.GetNewPath("shop6.cwz"));
			ShopC[7].LoadShopItemDecode(gDirPath.GetNewPath("shop7.cwz"));
			ShopC[8].LoadShopItemDecode(gDirPath.GetNewPath("shop8.cwz"));
			ShopC[9].LoadShopItemDecode(gDirPath.GetNewPath("shop9.cwz"));
			ShopC[10].LoadShopItemDecode(gDirPath.GetNewPath("shop10.cwz"));
		#ifdef ADD_SHOP_11_12	
			ShopC[11].LoadShopItemDecode(gDirPath.GetNewPath("shop11.cwz"));
			ShopC[12].LoadShopItemDecode(gDirPath.GetNewPath("shop12.cwz"));
		#endif
		#ifdef ADD_SHOP_14_20070308			
			ShopC[14].LoadShopItem(gDirPath.GetNewPath("shop14.cwz"));
		#endif	
	#else
			ShopC[0].LoadShopItem(gDirPath.GetNewPath("shop0.txt"));
			ShopC[1].LoadShopItem(gDirPath.GetNewPath("shop1.txt"));
			ShopC[2].LoadShopItem(gDirPath.GetNewPath("shop2.txt"));
			ShopC[3].LoadShopItem(gDirPath.GetNewPath("shop3.txt"));
			ShopC[4].LoadShopItem(gDirPath.GetNewPath("shop4.txt"));	
			ShopC[5].LoadShopItem(gDirPath.GetNewPath("shop5.txt"));
			ShopC[6].LoadShopItem(gDirPath.GetNewPath("shop6.txt"));
			ShopC[7].LoadShopItem(gDirPath.GetNewPath("shop7.txt"));
			ShopC[8].LoadShopItem(gDirPath.GetNewPath("shop8.txt"));
			ShopC[9].LoadShopItem(gDirPath.GetNewPath("shop9.txt"));
			ShopC[10].LoadShopItem(gDirPath.GetNewPath("shop10.txt"));
		#ifdef ADD_SHOP_11_12	
			ShopC[11].LoadShopItem(gDirPath.GetNewPath("shop11.txt"));
			ShopC[12].LoadShopItem(gDirPath.GetNewPath("shop12.txt"));
		#endif
		#ifdef ADD_FIRECRACKER_NPC_20070306
			ShopC[13].LoadShopItem(gDirPath.GetNewPath("shop13.txt"));
		#endif
		#ifdef ADD_SHOP_14_20070308			
			ShopC[14].LoadShopItem(gDirPath.GetNewPath("shop14.txt"));
		#endif
		#ifdef ADD_SEASON_3_NEW_MAP_20070910
			ShopC[15].LoadShopItem(gDirPath.GetNewPath("shop15.txt"));
			ShopC[16].LoadShopItem(gDirPath.GetNewPath("shop16.txt"));
			ShopC[17].LoadShopItem(gDirPath.GetNewPath("shop17.txt"));
		#endif
		#ifdef ADD_GAMBLING_20090120
			ShopC[18].LoadShopItem(gDirPath.GetNewPath("shop18.txt"));
		#endif // ADD_GAMBLING_20090120
	#endif
#endif

	
	LogAdd(lMsg.Get(465));
	return TRUE;
}
