#ifdef PICKUP_SYSTEM
#define MAX_PICKUP_ITEMS 20

struct PICKUP_ITEM
{
	char TextItem[20];
	int Type;
	int ID;	
};

//========================================================================================================================
class CPickUpSystem
{
public:
	void Init();
	void LoadFile(char *filename);
	void IsPickAddCommand(LPOBJECTSTRUCT lpObj, PMSG_CHATDATA *lpChat);
	void Insert(int gIndex, char *ItemText, int ItemType, int ItemIndex);
	int CheckIsPickItem(char *szItemDrop);
	void IsPickList(LPOBJECTSTRUCT lpObj);
	int PickUpActive(int IndexItem, int Type);
	void PickUpGetItem2(LPOBJECTSTRUCT lpObj, int aIndex);
	void PickUpGetItem(int aIndex, int ItemNum);
	int IfItemExistInList(LPOBJECTSTRUCT lpObj, int Item);
	PICKUP_ITEM m_PICK[32];
	int iCount;
}; extern CPickUpSystem s_PickUp;
#endif
//=========================================================================================================================