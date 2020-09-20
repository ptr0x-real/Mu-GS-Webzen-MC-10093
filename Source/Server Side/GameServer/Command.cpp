//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # DARK PLUGIN - POWERED BY FIRE TEAM
// # GAME SERVER: 99.60T (C) WEBZEN.
// # VERSÃO: 1.0.0.0
// # Autor: Maykon
// # Skype: Maykon.ale
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # O Senhor é meu pastor e nada me faltará!
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "StdAfx.h"
#include "DSProtocol.h"

CHAT_COMMANDS gs_cmd;

bool CHAT_COMMANDS::Settings()
{

	GetPrivateProfileString("IrCommand","Sintax do Comando </ir>","/ir",gs_cmd.IrSintax,50,CFG_COMMAND);

	//open baul
	OpenWareAtivo	= GetPrivateProfileInt("OpenWareCommand","Activar Comando",1,CFG_COMMAND);
	OpenWareLevel	= GetPrivateProfileInt("OpenWareCommand","Level para usar el comando",1,CFG_COMMAND);
	OpenWareVip		= GetPrivateProfileInt("OpenWareCommand","Activar Solamente para Vips",0,CFG_COMMAND);
	OpenWareZen		= GetPrivateProfileInt("OpenWareCommand","Zen por usar el comando",100000,CFG_COMMAND);
	GetPrivateProfileString("OpenWareCommand","Sintax de Comando","/open",OpenWareSintax,50,CFG_COMMAND);

	GetPrivateProfileString("ComproCommand","Sintax de Comando </compro>","/compro",gs_cmd.ComproSintax,50,CFG_COMMAND);
	GetPrivateProfileString("ComproCommand","Formato para exhibir el comando </compro>","[COMPRO]:%s",gs_cmd.ComproFormato,50,CFG_COMMAND);
	GetPrivateProfileString("VendoCommand","Sintax de Comando </vendo>","/vendo",gs_cmd.VendoSintax,50,CFG_COMMAND);
	GetPrivateProfileString("VendoCommand","Formato para exhibir el comando </vendo>","[VENDO]:%s",gs_cmd.VendoFormato,50,CFG_COMMAND);


	//GetPrivateProfileString("Server","Mensaje comando /premiar","[%s] Foi premiado(a) com [%d] Golds!",gs_cmd.PremiarNotice,160,"..\\data\\ConfigSurvivor.txt");
	GetPrivateProfileString("PREMIAR UPDATE","UPDATE PREMIAR","UPDATE MuOnline.dbo.MEMB_INFO SET Gold = Gold + %d Where memb___id = '%s'",gs_cmd.PremiarQuery,100,CFG_QUERYSHOP);

	GetPrivateProfileString("Server","Mensaje Sobrevivencia 1","digite [/ir] para participar.",gs_cmd.SobreNotice,160,"..\\data\\ConfigSurvivor.txt");

	return true;
}

void CHAT_COMMANDS::myinfo(int aIndex)
	{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

#if ENABLE_MC_SQL == 1
	int Golds = Manager.KCredits(lpObj->AccountID);
	int Resets = Manager.CountResets(lpObj->Name);

	func.OutWhiteMSG(aIndex, "[Info] Level: %d,Resets: %d", lpObj->Level, Resets);
	func.OutWhiteMSG(aIndex, "[Info] Cash: %d ", Golds);
#else
	func.OutWhiteMSG(aIndex, "[Info] Level: %d", lpObj->Level);
#endif // ENABLE_MC_SQL == 1

	func.OutWhiteMSG(aIndex, "[Info] Character: %s || Guild: %s", lpObj->Name, lpObj->GuildName);
	func.OutWhiteMSG(aIndex, "WwW.MuCastillo.Net");	
}


/*void ChangeWare(int aIndex)
{
	while(true)
	{

		if(gObj[aIndex].WarehouseSave == 1)
		{
			GCServerMsgStringSend("[COMANDO BAU]: Cierre el baul para usar el comando.",aIndex,1);
			return;
		}

		for(int i = 5; i >= 1; i--)
		{

			if(i == 5)
			{
				CGWarehouseUseEnd(aIndex);
				gObj[aIndex].m_IfState.state = 0;
				gObj[aIndex].m_IfState.type  = 0;
				gObj[aIndex].m_IfState.use   = 0;
			}

			gObj[aIndex].m_ReqWarehouseOpen = true;
			gObj[aIndex].LoadWareHouseInfo  = false;
			gObj[aIndex].WarehouseSave      = false;

			MsgOutput(aIndex,"[COMANDO BAU]: Cambiando el baul en [%d]",i);

			if(i == 3)
			{
				Manager.ExecFormat("EXEC MuOnline.dbo.MUDAR_BAU '%s', %d",gObj[aIndex].AccountID,gs_cmd.WareCod);
			}

			if(i == 1)
			{
				MsgOutput(aIndex,"[COMANDO BAU]: Cambio de baul completado con sucesso, baú [%d].",gs_cmd.WareCod);
//				GCLevelUpMsgSend(aIndex,gObj[aIndex].Level,gObj[aIndex].LevelUpPoint,(WORD)(gObj[aIndex].MaxLife+gObj[aIndex].AddLife),(WORD)(gObj[aIndex].MaxMana+gObj[aIndex].AddMana),(WORD)(gObj[aIndex].MaxBP+gObj[aIndex].AddBP));
				gObj[aIndex].m_ReqWarehouseOpen = false;
			}

			Sleep(1000);
		}
		break;
	}

	_endthread();
}*/
/*
void CHAT_COMMANDS::WareCommand(int aIndex, LPCSTR IpBuffer)
{  
	int _ativa = GetPrivateProfileInt("BauCommand","Ativar Comando </bau>",0,CFG_COMMAND);
	int Level = GetPrivateProfileInt("BauCommand","Level minimo para usar o comando </bau>",1,CFG_COMMAND);
	unsigned int Zen = GetPrivateProfileInt("BauCommand","Zen Minimo Para Usar o Comando",10000,CFG_COMMAND);
	int WareFree = GetPrivateProfileInt("BauCommand","Limite de baús Free",3,CFG_COMMAND);
	int WareVip1 = GetPrivateProfileInt("BauCommand","Limite de baús Vip1",5,CFG_COMMAND);
	int WareVip2 = GetPrivateProfileInt("BauCommand","Limite de baús Vip2",7,CFG_COMMAND);
	int WareVip3 = GetPrivateProfileInt("BauCommand","Limite de baús Vip3",9,CFG_COMMAND);

	int VipIndex = Manager.VipCount(gObj[aIndex].AccountID);

	gs_cmd.WareCod = atoi(IpBuffer);

	if(_ativa == 0)
	{
		GCServerMsgStringSend("Comando desabilitado.",aIndex,1);
		return;  
	}
	else if(gObj[aIndex].Level < Level )
	{
		GCServerMsgStringSend("[COMANDO BAU]: Usted no tiene el level necessário.",aIndex,1);
		return;
	}
	else if(gObj[aIndex].Money < Zen )
	{
		GCServerMsgStringSend("[COMANDO BAU]: Usted no tiene el zen suficiente.",aIndex,1);
		return;
	}

	switch(VipIndex)
	{
	case 0:
		{
			if(gs_cmd.WareCod > WareFree)
			{
				MsgOutput(aIndex,"[COMANDO BAU]: Su limite de bauls es de [%d].",WareFree);
				return;
			}
		}
		break; 
	case 1:
		{
			if(gs_cmd.WareCod > WareVip1)
			{
				MsgOutput(aIndex,"[COMANDO BAU]: Su limite de bauls es de [%d].",WareVip1);
				return;
			}
		}
		break; 
	case 2:
		{
			if(gs_cmd.WareCod > WareVip2)
			{
				MsgOutput(aIndex,"[COMANDO BAU]: Su limite de bauls es de [%d].",WareVip2);
				return;
			}
		}
		break; 
	case 3:
		{
			if(gs_cmd.WareCod > WareVip3)
			{
				MsgOutput(aIndex,"[COMANDO BAU]: Su limite de bauls es de [%d].",WareVip3);
				return;
			}
		}
		break; 
	}

	GCMoneySend(aIndex,gObj[aIndex].Money -= Zen);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ChangeWare,(void*)gObj[aIndex].m_Index,0,0);

}
*/
/*
void CHAT_COMMANDS::Premiar(int aIndex, char* msg)
{
	TNotice notice;
	int _ativa = GetPrivateProfileInt("GameMasterCommand","Ativar comando </premiar>",0,CFG_COMMAND);

	if(_ativa == 0)
	{
		GCServerMsgStringSend("Comando desabilitado.",aIndex,1);
		return;  
	}
	else if(func.VerificarGM(aIndex) == false)
	{
		GCServerMsgStringSend("Exclusivo apenas para gamemaster.",aIndex,1);
		return;
	}
	else if (strlen(msg) < 1)
	{
		return;
	}

	int num = { -1 };
	char tName[11];
	sscanf(msg, "%s %d", tName, &num);
	int tIndex = func.Get_PlayerIndex(tName);

	if(gObj[tIndex].Connected < 3)
	{
		GCServerMsgStringSend("El personaje no se encuentra.",aIndex,1);
		return;
	}
	else
	{

		Manager.ExecFormat(gs_cmd.PremiarQuery,num,gObj[tIndex].AccountID);

		notice.SendToAllUser(gs_cmd.PremiarNotice,tName,num);

	SQLCHAR CashLoggm[255] = { 0 };
	wsprintf((char *)CashLoggm, "[GM: %s] Entrega a Cuenta:[%s] Usuario:[%s] la cantidad de [%d] Creditos",gObj[aIndex].Name, gObj[tIndex].AccountID, gObj[tIndex].Name, num);//GUARDA LOGS DEL CAMBIO DE GOLDS
	LogSystem::CustomLogcashgm((char*)CashLoggm);//GUARDA LOGS DEL CAMBIO DE GOLDS
	}
}
*/

void CHAT_COMMANDS::OpenWareCommand(int aIndex)
{
#if ENABLE_MC_SQL == 1
	int VipIndex = Manager.VipCount(gObj[aIndex].AccountID);
#endif // ENABLE_MC_SQL == 1
	
	/*if(OpenWareAtivo == 0)
	{
		MsgOutput(aIndex,"[COMANDO OPENWARE]: [%s] Este Comando fue Desabilitado Por Administrador",gObj[aIndex].Name);
		
		return;
	}*/
	/*else if(OpenWareVip != 0)
	{	
		if( VipIndex <= 0 )
		{
			MsgOutput(aIndex,"[COMANDO OPENWARE]: [%s] Comando Exclusivo para vip's ",gObj[aIndex].Name);
			return;
		}
	}*/
	if(gObj[aIndex].Level < OpenWareLevel)
	{
		MsgOutput(aIndex,"[COMANDO OPENWARE]: [%s] Falta %d Level Para usar el comando",gObj[aIndex].Name,gObj[aIndex].Level - OpenWareLevel);
		return;
	}
	else if(gObj[aIndex].Money < OpenWareZen)
	{
		MsgOutput(aIndex,"[COMANDO OPENWARE]: [%s] Falta %d Zen Para usar el comando",gObj[aIndex].Name,gObj[aIndex].Money - OpenWareZen );
		return;
	}
	else 
	{
		if(gObj[aIndex].pTransaction == 1)
		{
			MsgOutput(aIndex,"[COMANDO OPENWARE]: %s Cerrar el Trade Para Usar el Comando!",gObj[aIndex].Name);		
			return;
		}

		if(gObj[aIndex].m_IfState.use == 1 && gObj[aIndex].m_IfState.type == 7 )
		{
			MsgOutput(aIndex,"[COMANDO OPENWARE]: %s Cerrar el ChaosBox Para Usar el Comando!",gObj[aIndex].Name);	
			return;
		}

		if(gObj[aIndex].m_ReqWarehouseOpen != 0  || gObj[aIndex].WarehouseSave != 0 )
		{
			MsgOutput(aIndex,"[COMANDO OPENWARE]: %s Cerrar el Baul Para Usar el Comando!",gObj[aIndex].Name);	
			return;
		}
			gObj[aIndex].m_ReqWarehouseOpen = 1;
			gObj[aIndex].m_IfState.type = 6;
			gObj[aIndex].m_IfState.use = 1;
			gObj[aIndex].m_IfState.state = 0;
			gObj[aIndex].WarehouseSave = TRUE;
			gObj[aIndex].WarehouseCount = 0;

			GCMoneySend(aIndex,gObj[aIndex].Money -= OpenWareZen);
			GDGetWarehouseList(aIndex,gObj[aIndex].AccountID);
			MsgOutput(aIndex,"[COMANDO OPENWARE]: %s Baul abierto con exito ",gObj[aIndex].Name);	
		}
}

void CHAT_COMMANDS::Compro(int aIndex, char*msg)
{
	//int banned = Manager.BancharPost(&gObj[aIndex].Name[0]);
	int _ativa = GetPrivateProfileInt("ComproCommand","Activar Comando </compro>",1,CFG_COMMAND);
	int Level = GetPrivateProfileInt("ComproCommand","Level minimo para usar el comando </compro>",1,CFG_COMMAND);
	unsigned int Zen = GetPrivateProfileInt("ComproCommand","Zen minimo para usar el comando </compro>",1,CFG_COMMAND);
	//int ComproCor = GetPrivateProfileInt("ComproCommand","Color de comando </compro>",1,CFG_COMMAND);

	if(gObj[aIndex].Level < Level)
	{
		GCServerMsgStringSend("Usted necesita mas level",aIndex,1);
		return;
	}
	else if(gObj[aIndex].Money < Zen)
	{
		GCServerMsgStringSend("usted necesita mas zen",aIndex,1);
		return;
	}

	//CHAR Message[61];
	char Buffer[60];
	sprintf(Buffer, gs_cmd.VendoFormato, msg);
	//sprintf(Message,gs_cmd.ComproFormato,msg);

	func.GreenChatSend(gObj[aIndex].Name, Buffer, aIndex);

	GCMoneySend(aIndex,gObj[aIndex].Money -= Zen);

}

void CHAT_COMMANDS::Vendo(int aIndex, char*msg)
{
	int _ativa = GetPrivateProfileInt("VendoCommand","Activar Comando </vendo>",1,CFG_COMMAND);
	int Level = GetPrivateProfileInt("VendoCommand","Level minimo para usar el comando </vendo>",1,CFG_COMMAND);
	unsigned int Zen = GetPrivateProfileInt("VendoCommand","Zen minimo para usar el comando </vendo>",1,CFG_COMMAND);

	if(gObj[aIndex].Level < Level)
	{
		GCServerMsgStringSend("Usted necesita mas level",aIndex,1);
		return;
	}
	else if(gObj[aIndex].Money < Zen)
	{
		GCServerMsgStringSend("usted necesita mas zen",aIndex,1);
		return;
	}

	CHAR Message[61];
	char Buffer[60];

	//sprintf(Message,gs_cmd.VendoFormato,msg);
	sprintf(Buffer, gs_cmd.VendoFormato, msg);

	func.GreenChatSend(gObj[aIndex].Name, Buffer, aIndex);

	GCMoneySend(aIndex,gObj[aIndex].Money -= Zen);
}

bool CHAT_COMMANDS::ChatRecv(PMSG_CHATDATA * lpMsg, int aIndex)
{
	
	//open baul
	//if(!_stricmp((const char*)(lpMsg->chatmsg),OpenWareSintax))
    if(!_memicmp(lpMsg->chatmsg,"/abrirbaul",strlen("/abrirbaul")))
	{
		gs_cmd.OpenWareCommand(aIndex);
		return true;
	}

	if(!_memicmp(lpMsg->chatmsg,"/info",strlen("/info")))
	{
		gs_cmd.myinfo(aIndex);
		return true;
	}
	
	// comando compro
	if(!_memicmp(lpMsg->chatmsg,gs_cmd.ComproSintax,strlen(gs_cmd.ComproSintax)))
	{
		gs_cmd.Compro(aIndex,(char*)(lpMsg->chatmsg+strlen(gs_cmd.ComproSintax)));
		return true;
	}
	// comando vendo
	if(!_memicmp(lpMsg->chatmsg,gs_cmd.VendoSintax,strlen(gs_cmd.VendoSintax)))
	{
		gs_cmd.Vendo(aIndex,(char*)(lpMsg->chatmsg+strlen(gs_cmd.VendoSintax)));
		return true;
	}

	/*if(!_memicmp(lpMsg->chatmsg,gs_cmd.WareSintax,strlen(gs_cmd.WareSintax)))
	{
		gs_cmd.WareCommand(aIndex,(char*)(lpMsg->chatmsg+strlen(gs_cmd.WareSintax)));
		return true;
	}
	if(!_memicmp(lpMsg->chatmsg,gs_cmd.PremiarSintax,strlen(gs_cmd.PremiarSintax)))
	{
		gs_cmd.Premiar(aIndex,(char*)(lpMsg->chatmsg+strlen(gs_cmd.PremiarSintax)));
		return true;
	}
	*/
	/*if(!_memicmp(lpMsg->chatmsg,SobreSintax,strlen(SobreSintax)))
	{
		Sobre.Init(aIndex,(char*)(lpMsg->chatmsg+strlen(SobreSintax)));
		return true;
	}*/

	/*if(!_memicmp(lpMsg->chatmsg,IrSintax,strlen(IrSintax)))
	{
		Sobre.Ir(aIndex);
		return true;
	}*/

	return false;
}