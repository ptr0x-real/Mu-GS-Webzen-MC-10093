// WTEventItemList.h: interface for the CWTEventItemList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WTEVENTITEMLIST_H__838AAEB2_6E48_44E1_9878_00DC709CB7BB__INCLUDED_)
#define AFX_WTEVENTITEMLIST_H__838AAEB2_6E48_44E1_9878_00DC709CB7BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WORLD_TOURNAMENT_EVENT_SETTING

#define MAX_WT_MAKEITEMLIST_COUNT		200
#define MAX_WT_MAKESUBITEMLIST_COUNT	20

typedef struct 
{
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
}WT_EVENTITEMLIST, *LPWT_EVENTITEMLIST;

class CWTEventItemList  
{
	BOOL				bLoadItemName[MAX_WT_MAKEITEMLIST_COUNT];
	char				sMakeItemName[MAX_WT_MAKEITEMLIST_COUNT][255];
	WT_EVENTITEMLIST	SubItemList[MAX_WT_MAKEITEMLIST_COUNT][MAX_WT_MAKESUBITEMLIST_COUNT];
public:
	CWTEventItemList();
	virtual ~CWTEventItemList();

	BOOL		Load(char* filename);

	BOOL		CommandMakeItem(int aIndex, char* sCommand);

};

extern CWTEventItemList gWTEventItemList;
#endif

#endif // !defined(AFX_WTEVENTITEMLIST_H__838AAEB2_6E48_44E1_9878_00DC709CB7BB__INCLUDED_)
