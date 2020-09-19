#include "StdAfx.h"
#include "PickUpSystem.h"
#include "protocol.h"
#include "logproc.h"
#include "Include\ReadScript.h"

#include "QuestInfo.h"
#include "KalimaGate.h"
#include "ChaosCastle.h"
#include "Include\define.h"
//========================================================================================================================
#ifdef PICKUP_SYSTEM
CPickUpSystem s_PickUp;


//========================================================================================================================
void CPickUpSystem::Init()
{
	for (int i = 0; i < 32; i++)
	{

		this->m_PICK[i].Type = 0;
		this->m_PICK[i].ID = 0;

	}
	iCount = 0;
}
//========================================================================================================================
void CPickUpSystem::LoadFile(char *filename)
	{
		this->Init();

		if ((SMDFile = fopen(filename, "r")) == NULL)
		{
			MessageBoxA(0, "CPickUpSystem::LoadFile() error", "CRITICAL ERROR", 0);
			ExitProcess(1);
			return;
		}

		SMDToken Token;
		int iIndex;

		int sIndex;
		char szItemText[20] = { 0 };
		int isItemType;
		int isItemIndex;
		
		while (true)
		{
			Token = GetToken();

			if (Token == END)
				break;

			if (Token == NUMBER)
			{
				iIndex = TokenNumber;

				while (true)
				{
					if (iIndex == 0)
					{
						Token = GetToken();

						if (strcmp("end", TokenString) == 0)
							break;

						sIndex = TokenNumber;

						Token = GetToken();
						memcpy(szItemText, TokenString, sizeof(szItemText));


						Token = GetToken();
						isItemType = TokenNumber;

						Token = GetToken();
						isItemIndex = TokenNumber;


						this->Insert(sIndex, szItemText, isItemType, isItemIndex);

						this->iCount++;
					}
					


				

				}
			}
		}

		fclose(SMDFile);
		return;
	}
	//========================================================================================================================
void CPickUpSystem::Insert(int gIndex, char *ItemText, int ItemType, int ItemIndex)
	{
		this->m_PICK[gIndex].ID = ItemIndex;
		this->m_PICK[gIndex].Type = ItemType;
		strcpy(this->m_PICK[gIndex].TextItem, ItemText);
	}
//========================================================================================================================
int CPickUpSystem::CheckIsPickItem(char *szItemDrop)
{
	for (int i = 0; i <= this->iCount; i++)
	{
		if (strcmp(szItemDrop, this->m_PICK[i].TextItem) == 0)
		{
			return i;
		}
	}
	return -1;
}
//========================================================================================================================
void CPickUpSystem::IsPickAddCommand(LPOBJECTSTRUCT lpObj, PMSG_CHATDATA *lpChat)
{
    char ItemText[MAX_CHAT_LEN];
	int PickCode = -1;
    sscanf(&lpChat->chatmsg[strlen("/pickup")+1],"%s",&ItemText);
	LogAdd("Use Pick Command -> [ %s ]\t[ %s ]\t[ %s ] Item: %s ", lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, ItemText);

	PickCode = CheckIsPickItem(ItemText);

	if (PickCode == -1)
	{
		//MsgOutput(lpObj->m_Index, "[PickUp System] : [%s] Not Found", ItemText);
		MsgOutput(lpObj->m_Index, "[PickUp System] : El item no existe en el PickUp", ItemText);
		return;
	}
	int ItemType = m_PICK[PickCode].Type;
	int ItemID = m_PICK[PickCode].ID;

	DWORD Item = ItemType * 32 + ItemID;

	if (!IfItemExistInList(lpObj, Item))
	{

		for (int x = 0; x < MAX_PICKUP_ITEMS; x++)
		{
			if (lpObj->m_pickup[x].Item == -1)
			{
				//if (lpObj->m_pickup[x].Item != Item)
				//{

				lpObj->m_pickup[x].Type = ItemType;
				lpObj->m_pickup[x].Index = ItemID;
				lpObj->m_pickup[x].Item = Item;
				break;
				//  }

			}
		}
		MsgOutput(lpObj->m_Index, "[PickUp System] : Usted agrego %s a su listado", ItemAttribute[Item].Name); 
	}
	else
	{
		MsgOutput(lpObj->m_Index, "[PickUp System] : Usted ya tiene %s en su listado ", ItemAttribute[Item].Name);
	}
}

BOOL CPickUpSystem::IfItemExistInList(LPOBJECTSTRUCT lpObj,int Item)
{
	for (int x = 0; x < MAX_PICKUP_ITEMS; x++)
	{

		if (lpObj->m_pickup[x].Item == Item)
		{
			return true;
			
			//break;
		}

	}
	return false;

}
void CPickUpSystem::IsPickList(LPOBJECTSTRUCT lpObj)
{
	for (int x = 0; x < MAX_PICKUP_ITEMS; x++)
	{
		if (lpObj->m_pickup[x].Item != -1)
		{
			MsgOutput(lpObj->m_Index, "[PickUp System] Item #%d: %s (%d %d)", x+1, ItemAttribute[lpObj->m_pickup[x].Item].Name, lpObj->m_pickup[x].Type, lpObj->m_pickup[x].Index);
			//break;
		}
	}

}
/*
struct PMSG_ITEMGETRESULT
{
	PBMSG_HEAD h;	// C1:22
	BYTE result;	// [0xFE:Money] 3
	BYTE Data[MAX_ITEM_INFO];	// 4
};
*/
BOOL CPickUpSystem::PickUpActive(int aIndex, int Type)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	
	for (int x = 0; x < MAX_PICKUP_ITEMS; x++)
	{
		if (lpObj->m_pickup[x].Item == Type)
		{	
			return true;
		}
	}
	return false;
}

//void CCustomPick::ItemGet(LPOBJ lpObj)
void CPickUpSystem::PickUpGetItem2(LPOBJECTSTRUCT lpObj, int aIndex)
{
	
}
#endif