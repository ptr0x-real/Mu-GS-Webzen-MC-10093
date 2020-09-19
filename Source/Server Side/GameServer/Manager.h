#pragma once

class CManager : public CDataBase
{
public:
	CManager(void);
	~CManager(void);

public:
	bool Initialize();
	bool InitDatabase();
	bool Load();
	int VipCount(char* Account);
	int CountResets(char * Character);
	int CountMasters(char * Character);
	int KCredits( char* Account );
	int Pshopnpc( char* Account );
	int DuelWins(char * Character);
	int DuelLoser(char * Character);
	int CtlCount( char* Account );


private:
	char Host[50];
	char Database[50];
	char User[50];
	char Senha[50];
};

extern CManager Manager;