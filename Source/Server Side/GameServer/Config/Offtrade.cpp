#include "StdAfx.h"
#include "user.h"
#include "SProtocol.h"
#include "OffAfk.h"

//==================================================================================================
// Offline System System
//==================================================================================================
COFFSystem OFFSystem;

void COFFSystem::CreateOfflineSystem(int aIndex)
{

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	/*
	if(gObj[aIndex].MapNumber != 0)
	{
		MsgOutput(aIndex,"[OFFSystem][%s]Usted Necesita estar en Lorencia",gObj->Name);
		return;
	}
	*/
	lpObj->OffAfk = true;
	//CloseClient(aIndex);
	CloseClientEx(aIndex);
	return;
}

void COFFSystem::Closeoffafk(char * AccountID)
{
	for (int i = OBJ_STARTUSERINDEX; i < MAX_OBJECT; i++)
	{
		LPOBJECTSTRUCT lpUser = &gObj[i];
		// ----
		if (lpUser->Connected != PLAYER_PLAYING || !lpUser->OffAfk || strcmp(AccountID, lpUser->AccountID))
		{
			continue;
		}
		// ----
		GJPUserClose2(lpUser->AccountID);
		gObjDel(i);
		lpUser->OffAfk = false;
	}
}

bool COFFSystem::Updateoffafk(int UserIndex, BYTE Type)
{
	LPOBJECTSTRUCT lpUser = &gObj[UserIndex];
	// ----
	if (!lpUser->OffAfk)
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