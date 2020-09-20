#include "StdAfx.h"

#if ENABLE_MC_SQL == 1
CManager Manager = CManager();

CManager::CManager(void)
{
}

CManager::~CManager(void)
{
}

bool CManager::Initialize()
{
	if (!this->InitDatabase())
	{
		return false;
	}

	return true;
}

bool CManager::InitDatabase()
{
	bool Return = false;

	Manager.Load();

	if (this->Connect(this->Host,this->Database,this->User,this->Senha) == true)
	{	
		Return = true;
	}
	else
	{
		MessageBox(NULL,"Conexion con SQL Server Fallo!\PorFavor Verificar el archivo 'SQLConnect.txt","Erroe en conexion con SQL Server!",MB_OK | MB_ICONSTOP);
		ExitProcess(0);
		
		this->DiagnosticConn();
	}

	return Return;
}

bool CManager::Load()
{
	GetPrivateProfileString("SQL", "User", "sa", this->User, sizeof(this->User), "..\\Data\\SQLConnect.txt");
	GetPrivateProfileString("SQL", "Password", "fireteam", this->Senha, sizeof(this->Senha), "..\\Data\\SQLConnect.txt");
	GetPrivateProfileString("SQL", "DataBase", "MuOnline", this->Database, sizeof(this->Database), "..\\Data\\SQLConnect.txt");
	GetPrivateProfileString("SQL", "Host", "127.0.0.1", this->Host, sizeof(this->Host), "..\\Data\\SQLConnect.txt");

	return true;
}

int CManager::DuelWins(char * Character)
{
	int DuelWins;

	this->ExecFormat("SELECT DuelWins from Character WHERE Name = '%s'",Character);

	this->Fetch( );

	DuelWins = this->GetInt("DuelWins");

	this->Clear( );

	return DuelWins;
}

int CManager::DuelLoser(char * Character)
{
	int DuelLoser;

	this->ExecFormat("SELECT DuelLoser from Character WHERE Name = '%s'",Character);

	this->Fetch( );

	DuelLoser = this->GetInt("DuelLoser");

	this->Clear( );

	return DuelLoser;
}

int CManager::VipCount(char* Account)
{
	int Vip;

	this->ExecFormat("SELECT vip FROM MEMB_INFO WHERE memb___id = '%s'" , Account );

	this->Fetch( );

	Vip = this->GetInt("vip");

	this->Clear( );

	return Vip;
}

int CManager::CountResets(char* Character)
{
	int Resets;

	this->ExecFormat("SELECT Resets from Character WHERE Name = '%s'",Character);

	this->Fetch( );

	Resets = this->GetInt("Resets");

	this->Clear( );

	return Resets;
}

int CManager::CountMasters(char* Character)
{
	int MResets;

	this->ExecFormat("SELECT MResets from Character WHERE Name = '%s'",Character);

	this->Fetch( );

	MResets = this->GetInt("MResets");

	this->Clear( );

	return MResets;
}

int CManager::KCredits( char* Account )
{
	int Cash;

	this->ExecFormat("SELECT Cash FROM MEMB_INFO WHERE memb___id = '%s'" , Account );

	this->Fetch( );

	Cash = this->GetInt("Cash");

	this->Clear( );

	return Cash;
}

int CManager::Pshopnpc( char* Account )
{
	int Pshopnpc;

	this->ExecFormat("SELECT pshop FROM MEMB_INFO WHERE memb___id = '%s'" , Account );

	this->Fetch( );

	Pshopnpc = this->GetInt("pshop");

	this->Clear( );

	return Pshopnpc;
}

int CManager::CtlCount( char* Account )
{
	int CtlCode;

	this->ExecFormat("SELECT CtlCode FROM Character WHERE AccountID = '%s'" , Account );

	this->Fetch( );

	CtlCode = this->GetInt("CtlCode");

	this->Clear( );

	return CtlCode;
}
#endif // ENABLE_MC_SQL == 1