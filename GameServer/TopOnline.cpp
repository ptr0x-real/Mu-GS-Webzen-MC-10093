#include "StdAfx.h"

int TopOnline__Time;
int TopOnline__BaseExp;
int TopOnline__LevelUpPoints;
int TopOnline__MaxLevel;
char Query12[100];

void YDTeamTopOnline()
{
	int TopOnlineEnabled = GetPrivateProfileIntA("TopOnline", "Switch", 0, TopOnline_Event);
	TopOnline__BaseExp = GetPrivateProfileIntA("TopOnline", "Experiencia", 8000, TopOnline_Event);
	int Secs = GetPrivateProfileIntA("TopOnline", "Tiempo", 10, TopOnline_Event);
	TopOnline__MaxLevel = GetPrivateProfileIntA("TopOnline", "MaxLevel", 401, TopOnline_Event);
	TopOnline__LevelUpPoints = GetPrivateProfileIntA("TopOnline", "Mensaje de Cantidad de golds", 5, TopOnline_Event); // si pero ponele tambien el mismo numero de la querry
	GetPrivateProfileString("TOPONLINE GOLDS","UPDATE GOLDS","UPDATE MuOnline.dbo.MEMB_INFO SET Credits = Credits + 5 Where memb___id = '%s'",Query12,100,TopOnline_Event);
	if(TopOnlineEnabled != 0)
	{
		TopOnline__Time	= (1000 * Secs);
		_beginthread( TopOnline__Trigger, 0, NULL  );
	}
}

int TopOnlineLevelUpPoints( DWORD PlayerID)
{
	//OBJECTSTRUCT *lpObj = (OBJECTSTRUCT*)OBJECT_POINTER(PlayerID);
	LPOBJECTSTRUCT lpObj = &gObj[PlayerID];

	int LevelUpPoint = 0;
	//if(gObj[PlayerID].Offtrade != 1)
	if ((lpObj->OffExp != 1 ) && ( lpObj->OffAfk != 1)&& ( lpObj->Offtrade != 1))
	{
	if (lpObj->Level < TopOnline__MaxLevel)         
	{
		switch(lpObj->Class)
		{
		case 3:
			{
				LevelUpPoint = 7;	
			}break;
		case 4:
			{
				LevelUpPoint = 7;  

			}break;

		}

	}
	}
	return LevelUpPoint;
}

void TopOnline__Trigger(void * lpParam)
{
	BYTE Packet[10] = {0xC3,0x09,0x16,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF};

	while(true)
	{		
		Sleep(TopOnline__Time);

		for(int PlayerID=OBJECT_MIN;PlayerID<MAX_OBJECT;PlayerID++)
		{
			int AddExp = 0;

			if(func.IsOnGame(PlayerID))
			{
				LPOBJECTSTRUCT lpObj = &gObj[PlayerID];

				int pLevel = lpObj->Level;

				if (pLevel <= TopOnline__MaxLevel && OfflineShop[PlayerID].IsOffTrade != 1 && ExUser[PlayerID].OffAttack != 1 ) // evitar q de creditos cuando estan offline system
				if (pLevel <= TopOnline__MaxLevel)  
				{

					func.OutWhiteMSG(lpObj->m_Index, mjecust.MSG168,TopOnline__LevelUpPoints);
					Manager.ExecFormat(Query12,lpObj->AccountID);
				
				}
			}
		}
	}
}