// CastleEventItemList.h: interface for the CCastleEventItemList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CastleEventItemList_H__838AAEB2_6E48_44E1_9878_00DC709CB7BB__INCLUDED_)
#define AFX_CastleEventItemList_H__838AAEB2_6E48_44E1_9878_00DC709CB7BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef FOR_CASTLE_TESTSERVER

#define MAX_CS_MAKEITEMLIST_COUNT		200
#define MAX_CS_MAKESUBITEMLIST_COUNT	20

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

class CCastleEventItemList  
{
	BOOL				bLoadItemName[MAX_CS_MAKEITEMLIST_COUNT];
	char				sMakeItemName[MAX_CS_MAKEITEMLIST_COUNT][255];
	WT_EVENTITEMLIST	SubItemList[MAX_CS_MAKEITEMLIST_COUNT][MAX_CS_MAKESUBITEMLIST_COUNT];
public:
	CCastleEventItemList();
	virtual ~CCastleEventItemList();

	BOOL		Load(char* filename);

	BOOL		CommandMakeItem(int aIndex, char* sCommand);

};

extern CCastleEventItemList g_CastleEventItemList;
#endif

#endif // !defined(AFX_CastleEventItemList_H__838AAEB2_6E48_44E1_9878_00DC709CB7BB__INCLUDED_)
