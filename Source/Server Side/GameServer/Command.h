//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # DARK PLUGIN - POWERED BY FIRE TEAM
// # GAME SERVER: 99.60T (C) WEBZEN.
// # VERSÃO: 1.0.0.0
// # Autor: Maykon
// # Skype: Maykon.ale
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # O Senhor é meu pastor e nada me faltará!
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

class CHAT_COMMANDS
{
public:
	bool Settings();

	void Valley(int aIndex);
	void GameServerInfo(DWORD aIndex);
	void Post(int aIndex, char*msg);
	void Zen(DWORD aIndex);
	void Add(int aIndex, LPCSTR lpBuffer, int Status);
	void Pk(DWORD aIndex);
	void Readd(int aIndex,char * msg);
	void MoveAll(short aIndex, char *bIndex);
	void Drop(int aIndex,char * msg);
	void Fire(DWORD aIndex);
	void WareCommand(int aIndex, LPCSTR IpBuffer);
	void Online(DWORD aIndex);
	void Premiar(int aIndex, char* msg);
	void Skin(int aIndex, char* msg);
	void Move(int aIndex, char* msg);
	void myinfo(int aindex);
	void Compro(int aIndex, char*msg);
	void Vendo(int aIndex, char*msg);
	void OpenWareCommand(int aIndex);
	void ThreadReload(int aIndex,char* Message);
	void StormCommand(int aIndex);

	bool ChatRecv(PMSG_CHATDATA * lpMsg, int aIndex);

	char StormSintax[50];
	char ReloadSintax[50];
	char ComproSintax[50];
	char ComproFormato[50];
	char VendoSintax[50];
	char VendoFormato[50];
	char PostSintax[50];
	char PostFormato[50];
	char ZenSintax[50];
	char ForSintax[50];
	char AgiSintax[50];
	char VitSintax[50];
	char EneSintax[50];
	char CmdSintax[50];
	char PkSintax[50];
	char ReaddSintax[50];
	char MoveSintax[50];
	char DropSintax[50];
	char FireSintax[50];
	char WareSintax[50];
	char OnlineSintax[50];
	char PremiarSintax[50];
	char SkinSintax[50];
	char GmoveSintax[50];
	char SobreSintax[50];
	char IrSintax[50];

		//open baul
	unsigned int OpenWareAtivo;
	unsigned int OpenWareLevel;
	unsigned int OpenWareVip;
	unsigned int OpenWareZen;
	char OpenWareSintax[50];

	char PremiarQuery[100];
	char PremiarNotice[160];

	char MataMataQuery1[100];

	char SobreNotice[160];

	int WareCod;
	int Contplayer;

	bool Sobrevivencia;
	bool Sobreblock;

	//offsystem afk y trade
	char OffafkSintax[50];
	char OffTradeSintax[50];

};

extern CHAT_COMMANDS gs_cmd;

void ChangeWare(int aIndex);

