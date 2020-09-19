#include "StdAfx.h"
#include "user.h"

#define BALANCESYSTEM_DIR "..\\data\\BalanceSystem.ini"

class cBalanceSystem
{
public:
	cBalanceSystem();
	virtual ~cBalanceSystem();

	void Load(char * File);
	void Init();
	void ReadConfigs(char * File);

	void Main(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTarget, int & AttackDamage);
	void Main2(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTarget, int & AttackDamage);

private:
	bool Enable;

	int DKvsDK;
	int DKvsDW;
	int DKvsELF;
	int DKvsMG;
	int DKvsDL;

	int DWvsDW;
	int DWvsDK;
	int DWvsELF;
	int DWvsMG;
	int DWvsDL;

	int ELFvsELF;
	int ELFvsDK;
	int ELFvsDW;
	int ELFvsMG;
	int ELFvsDL;

	int MGvsMG;
	int MGvsDK;
	int MGvsDW;
	int MGvsELF;
	int MGvsDL;

	int DLvsDL;
	int DLvsDK;
	int DLvsDW;
	int DLvsELF;
	int DLvsMG;

};
extern cBalanceSystem gBalanceSystem;