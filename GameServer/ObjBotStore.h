#include "user.h"
#include "stdafx.h"

#define MAX_BOTSTORE	200

struct BotStoreBodyItems
{
	int num;
	int level;
	int opt;
	bool Enabled;
};

struct BotStoreItems
{
	int Value;
	WORD num;
	BYTE Level;
	BYTE Opt;
	BYTE Luck;
	BYTE Skill;
	BYTE Dur;
	BYTE Exc;
	BYTE Anc;
};

struct botStoreStruct
{
	int index;
	int Class;
	int OnlyVip;
	int UseVipMoney;
	char Name[11];
	char StoreName[20];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	bool Enabled;
	BotStoreBodyItems body[9];
	BotStoreItems storeItem[MAIN_INVENTORY_SIZE+PSHOP_SIZE];
	BYTE ItemCount;
};

class ObjBotStore
{
public:
	bool Enabled;
	void Read(char * FilePath);
	void MakeBot();
	void AddItem(int bIndex,int botNum);
	BYTE CheckSpace(LPOBJECTSTRUCT lpObj, int type, BYTE * TempMap);
	botStoreStruct bot[MAX_BOTSTORE];
};
extern ObjBotStore BotStore;