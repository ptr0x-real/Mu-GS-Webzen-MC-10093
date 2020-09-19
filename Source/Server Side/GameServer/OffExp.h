#include "StdAfx.h"
#include "user.h"

#define OFF_EXP_DIR "..\\data\\OfflineAttack.txt"


class cOffExp
{
public:
	int Enable;
	int KickFallen;
	int EnableTime;
	int TimeEnd;
	int Speed1;
	int Speed2;
	int Speed3;
	int Speed4;
	int Speed5;
	int Speed6;
	int Speed7;
	int Speed8;

	void LoadConfig();

	void OffExpStart(int aIndex);
	void OffExpStart2(int aIndex);
	void OffExpStart3(int aIndex);

	void MainFunction();

	void DarkKnightOff(LPOBJECTSTRUCT lpObj);
	void DarkWizardOff(LPOBJECTSTRUCT lpObj);
	void FairyElfOff(LPOBJECTSTRUCT lpObj);
	void MagicGladiatorOff(LPOBJECTSTRUCT lpObj);
	void DarkLordOff(LPOBJECTSTRUCT lpObj);
	void SummonerOff(LPOBJECTSTRUCT lpObj);
	void RageFighterOff(LPOBJECTSTRUCT lpObj);

	void UseMana(int aIndex);

	int SearchTarget(LPOBJECTSTRUCT lpObj);

	bool CheckTerrain(LPOBJECTSTRUCT lpObj);

	void Close(char * AccountID);
	bool Update(int UserIndex, BYTE Type);
	void CheckAndPickUpItem(int aIndex, CMapItem* cMapItem,short tObjNum);

	void TickTimes(int aIndex);
};
extern cOffExp OffExp;