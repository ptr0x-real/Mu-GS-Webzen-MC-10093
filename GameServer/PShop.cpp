#include "StdAfx.h"
#include "protocol.h"
/*
void  CPShop::CGPShopReqBuyItemEx(PMSG_REQ_BUYITEM_FROM_PSHOP *lpMsg, int aSourceIndex, int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	if (lpObj->BotStore == 1)
	{
	LPOBJECTSTRUCT lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];
	int Golds = Manager.KCredits(gObj[aSourceIndex].AccountID);
	int pide = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;
	int NCPSHOP = Manager.Pshopnpc(lpObj->AccountID);
	GetPrivateProfileString("UPDATE OFFTRADE", "OFFTRADERESTA", "UPDATE MuOnline.dbo.MEMB_INFO SET credits = credits - %d WHERE memb___id = '%s'", g_GlobalConfig.OFFTRADERESTA, 200, CFG_QUERYSHOP);//SACA GOLDS
	GetPrivateProfileString("UPDATE OFFTRADE", "OFFTRADESUMA", "UPDATE MuOnline.dbo.MEMB_INFO SET credits = credits + %d WHERE memb___id = '%s'", g_GlobalConfig.OFFTRADESUMA, 200, CFG_QUERYSHOP);//SACA GOLDS
	
	if (NCPSHOP == 1)//parametro del comando offtrade
	{


		if (Golds < pide)
		{
			MsgOutput(aSourceIndex,"[PShop] Necesitas %d Golds", lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue);
			MsgOutput(aSourceIndex,"[PShop] Actualmente tienes  %d Golds", Golds);
			CGPShopAnsOpen(aIndex, 0);
			return;
		}

		BYTE btNewItemPos = 0;
	    btNewItemPos = gObjOnlyInventoryInsertItem(aSourceIndex, lpObj->Inventory1[lpMsg->btItemPos]);//inventario lleno o no

		if (btNewItemPos == 0xFF)
	    {
		//Log.Inform("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] No Room to Buy Item", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
		CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);

		return;
	    }

		Manager.ExecFormat(g_GlobalConfig.OFFTRADERESTA, pide, gObj[aSourceIndex].AccountID); // gObj lpObj
		Manager.ExecFormat(g_GlobalConfig.OFFTRADESUMA, pide, lpObj->AccountID);
		MsgOutput(aSourceIndex,"[PShop] Te quedan %d Golds", Golds - pide);
		CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, btNewItemPos, 1);
		CGPShopReqBuyItemTest(lpMsg, aSourceIndex);

		return;
	}
	CGPShopReqBuyItem(lpMsg, aSourceIndex);
}
}

*/
void CPShop::CGPShopReqBuyItemTest(PMSG_REQ_BUYITEM_FROM_PSHOP * lpMsg, int aSourceIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];

	int NPCGOLDS = Manager.Pshopnpc(lpObj->AccountID);
	int Golds = Manager.KCredits(gObj[aSourceIndex].AccountID);
	//GetPrivateProfileString("UPDATE OFFTRADE", "OFFTRADEOFF", "UPDATE MuOnline.dbo.MEMB_INFO SET OfftradeOn = +1 WHERE memb___id = '%s'", g_GlobalConfig.OfftradeOff, 200, CFG_QUERYSHOP);//SACA GOLDS
	
	if (gObjIsConnected(MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)) == FALSE)
	{
		//Log.Inform("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller did Not CONNECTED (%d)", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if (lpObj->Type != OBJTYPE_CHARACTER)
	{
		//Log.Inform("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Not CHARACTER (%d)", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if (lpObj->CloseCount >= 0)
	{
		//Log.Inform("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Closing (%d)", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if (gObj[aSourceIndex].CloseCount >= 0)
	{
		//Log.Inform("[PShop] [%s][%s] PShop Item Buy Request Failed : Buyer is Closing (%d)", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if (lpObj->m_bPShopOpen == false)
	{
		//Log.Inform("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did not Open PShop", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
		CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}
	
	char szName[MAX_ACCOUNT_LEN + 1] = { 0 };
	memcpy(szName, lpMsg->btName, sizeof(lpMsg->btName));
	szName[MAX_ACCOUNT_LEN] = 0;
	int iITEM_LOG_TYPE;
	int iITEM_LOG_LEVEL;
	int iITEM_LOG_DUR;
	int iITEM_LOG_SERIAL;

	if (NPCGOLDS == 1)
	{
		iITEM_LOG_TYPE = gObj[aSourceIndex].Inventory1[lpMsg->btItemPos].m_Type;
		iITEM_LOG_LEVEL = gObj[aSourceIndex].Inventory1[lpMsg->btItemPos].m_Level;
		iITEM_LOG_DUR = gObj[aSourceIndex].Inventory1[lpMsg->btItemPos].m_Durability;
		iITEM_LOG_SERIAL = gObj[aSourceIndex].Inventory1[lpMsg->btItemPos].m_Number;
		int precio = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;
		SQLCHAR CashLog[255] = { 0 };
		wsprintf((char *)CashLog, "[COMPRA] Cuenta:[%s] Usuario:[%s] Compro por [%d] Cash", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, precio);//GUARDA LOGS DEL CAMBIO DE GOLDS
		CPShop::CustomLogcash((char*)CashLog);//GUARDA LOGS DEL CAMBIO DE GOLDS
	   // Log.Inform("[COMPRA] Cuenta:[%s] Usuario : [%s] Compro por [%d] Cash", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, precio);//GUARDA LOGS DEL CAMBIO DE GOLDS	
	}

	if (PShop_CheckInventoryEmpty(lpObj->m_Index) == true) // era ==
	{

		lpObj->m_bPShopOpen = false;
		memset(lpObj->m_szPShopText, 0, sizeof(lpObj->m_szPShopText));
		CGPShopAnsClose(lpObj->m_Index, 1);

	}
	else
	{
		lpObj->m_bPShopItemChange = true;
	}


	lpObj->m_bPShopTransaction = false;

	}

//AGREGAR LOGS PARA CASH
void CPShop::CustomLogcash(char * Text)
{
	SYSTEMTIME now;
	GetLocalTime(&now);

	TCHAR LogTexto[512];
	FILE *stream;

	TCHAR logString[512];

	wsprintf(logString, "BOTSTORE\\%02d_%02d_%02d_cash_Log.txt", now.wYear, now.wMonth, now.wDay);
	stream = fopen(logString, "a+");
	fprintf(stream, "%02d:%02d:%02d - %s\n", now.wHour, now.wMinute, now.wSecond, Text);
	fclose(stream);
}