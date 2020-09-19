#ifndef _CLASS_SETITEMMACRO_H_
#define _CLASS_SETITEMMACRO_H_

#ifdef ITEM_ADD_OPTION_20061019

typedef struct 
{
	int		SetIndex;
	int		ItemIndex;
	int		Level;
	int		Skill;
	int		Luck;
	int		Option;
	int		Ex1;
	int		Ex2;
	int		Ex3;
	int		Ex4;
	int		Ex5;
	int		Ex6;
}SETITEMLIST, *LPSETITEMLIST;

#define MAX_SETITEMLIST_COUNT		50
#define MAX_SETSUBITEMLIST_COUNT	15

class CSetItemMacro
{
public:
	CSetItemMacro();
	~CSetItemMacro();

	BOOL				Load(char* szFileName);
	BOOL				MakeItem(int aIndex, char* szSetName);
private:
	SETITEMLIST			SubItemList[MAX_SETITEMLIST_COUNT][MAX_SETSUBITEMLIST_COUNT];
	BOOL				bLoadItemName[MAX_SETITEMLIST_COUNT];
	char				sMakeItemName[MAX_SETITEMLIST_COUNT][255];
};
#endif
#endif