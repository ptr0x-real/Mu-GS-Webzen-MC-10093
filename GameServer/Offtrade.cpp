#include "StdAfx.h"
#include "user.h"
#include "SProtocol.h"
#include "OffAfk.h"
#include "TNotice.h"

//==================================================================================================
// Offline System System
//==================================================================================================
COFFtrade OFFtrade;

void COFFtrade::CreateOfflinetrade(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if(lpObj->m_bPShopOpen == false)
	{
		MsgOutput(aIndex,"[OffTrade] Usted Tiene que abrir su tienda personal.");
		return;
	}
	
	if (lpObj->OffTradeZen == 1)
	{
		    char TextShop[36] = "<<ZEN SHOP>>";
			PMSG_ANS_PSHOP_TEXT_CHANGED pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x10, sizeof(pMsg));
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);
			memcpy(lpObj->m_szPShopText, TextShop, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btPShopText, TextShop, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btName, lpObj->Name, sizeof(pMsg.btName));
			MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
	}

	if (lpObj->OffTradeCredits == 1)
	{
		    char TextShop[36] = "<<CASH SHOP>>";
			PMSG_ANS_PSHOP_TEXT_CHANGED pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x10, sizeof(pMsg));
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);
			memcpy(lpObj->m_szPShopText, TextShop, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btPShopText, TextShop, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btName, lpObj->Name, sizeof(pMsg.btName));
			MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
	
	}

	if (lpObj->BotStore == 1)
	{
		    char TextShop[36] = "<<BOTSTORE>>";
			PMSG_ANS_PSHOP_TEXT_CHANGED pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x10, sizeof(pMsg));
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);
			memcpy(lpObj->m_szPShopText, TextShop, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btPShopText, TextShop, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btName, lpObj->Name, sizeof(pMsg.btName));
			MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
	
	}

	lpObj->Offtrade = true;
	//CloseClient(aIndex);
	CloseClientEx(aIndex);
	return;
}

void COFFtrade::Closeofftrade(char * AccountID)
{
	for (int i = OBJ_STARTUSERINDEX; i < MAX_OBJECT; i++)
	{
		LPOBJECTSTRUCT lpUser = &gObj[i];
		// ----
		if (lpUser->Connected != PLAYER_PLAYING || !lpUser->Offtrade || strcmp(AccountID, lpUser->AccountID))
		{
			continue;
		}
		// ----
		CloseClientEx(i);
		GJPUserClose2(lpUser->AccountID);
		gObjDel(i);
		lpUser->Offtrade = false;
	}
}

bool COFFtrade::Updateofftrade(int UserIndex, BYTE Type)
{
	LPOBJECTSTRUCT lpUser = &gObj[UserIndex];
	// ----
	if (!lpUser->Offtrade)
	{
		return false;
	}
	// ----
	if (Type == 0)
	{
		lpUser->CheckTick = GetTickCount();
	}
	else if (Type == 1)
	{
		lpUser->ConnectCheckTime = GetTickCount();
	}
	// ----
	return true;
}