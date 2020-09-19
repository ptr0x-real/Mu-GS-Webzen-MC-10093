#include "StdAfx.h"
#include "ConnectBonus.h"
#include "Gamemain.h"
char Query12[100];
int TopOnline__LevelUpPoints;

void YDTeamTopOnline()
{
	TopOnline__LevelUpPoints = GetPrivateProfileIntA("Bonus", "Mensaje de Cantidad de cash", 5, "..//data//OnlineBonus.ini"); // si pero ponele tambien el mismo numero de la querry
	GetPrivateProfileString("SQLBonus","UPDATE GOLDS","UPDATE MuOnline.dbo.MEMB_INFO SET Credits = Credits + 5 Where memb___id = '%s'",Query12,100,"..//data//OnlineBonus.ini");
}
void AddTimeBonus(int UserIndex)
{
	// ----
	if( gObj[UserIndex].m_ConnectBonusTime != gConnectBonusTime)
	{
		return;
	}
	// ----
	gObj[UserIndex].m_ConnectBonusTime = 0;

	if ((gObj[UserIndex].OffExp == 1) || (gObj[UserIndex].Offtrade == 1)|| (gObj[UserIndex].OffAfk == 1))
	{
		return;
	}

	if (gConnectBonusCashPoints > 0)
	{
		Manager.ExecFormat(Query12,gObj[UserIndex].AccountID);
		MsgOutput(gObj[UserIndex].m_Index, "[TimeBonus]  Ganaste %d Cash(s)", TopOnline__LevelUpPoints);
		//LogAdd("[TimeBonus] Ganaste %d Cash(s) por tu Hora", TopOnline__LevelUpPoints);

	}
}
