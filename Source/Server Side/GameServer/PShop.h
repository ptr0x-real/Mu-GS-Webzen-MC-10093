#pragma once

#if ENABLE_MC_SQL == 1
struct PBMSG_HEADIT	   
{
	BYTE c;
	BYTE size;
	BYTE headcode;
};



struct PBMSG_HEAD8	// Packet - Byte Type
{

public:

	void set(LPBYTE lpBuf, BYTE head, BYTE sub, BYTE size)
	{
		lpBuf[0] = 0xC1;
		lpBuf[1] = size;
		lpBuf[2] = head;
		lpBuf[3] = sub;
	};

	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
};
/*
struct PMSG_REQ_PSHOP_OPEN
{
	PBMSG_HEAD8 h;
	char szPShopText[36];	// 4
};
*/
/*

struct PMSG_REQ_BUYITEM_FROM_PSHOP
{
	PBMSG_HEAD8 h;
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE btName[10];	// 6
	BYTE btItemPos;	// 10

};

*/
class CPShop
{
public:
	//offstore creditos
	static void CGPShopReqBuyItemTest(PMSG_REQ_BUYITEM_FROM_PSHOP * lpMsg, int aSourceIndex);
	static void CGPShopReqOpenEx(PMSG_REQ_PSHOP_OPEN *lpMsg, int aIndex);
	//static void  CGPShopReqBuyItemEx(PMSG_REQ_BUYITEM_FROM_PSHOP *lpMsg, int aSourceIndex, int aIndex);
	static void CustomLogcash(char * Text);
	void loadpshop();

}; 

extern CPShop PShop;

//void CGPShopAnsCloseHook(int aIndex, BYTE btResult); 
//void ResponErrorCloseClientHook(int aIndex); 
//void CloseClient2Hook(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result); 
//void CloseClientHook(int aIndex); 
short gObjDel_Ex(int aIndex);
#endif // ENABLE_MC_SQL == 1