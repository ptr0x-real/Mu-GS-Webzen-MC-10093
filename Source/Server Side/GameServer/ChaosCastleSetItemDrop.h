// ChaosCastleSetItemDrop.h: interface for the CChaosCastleSetItemDrop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAOSCASTLESETITEMDROP_H__297BDBE7_C2A9_4A74_B434_81151B137CF4__INCLUDED_)
#define AFX_CHAOSCASTLESETITEMDROP_H__297BDBE7_C2A9_4A74_B434_81151B137CF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CHANGE_CHAOSCASTLE_SET_ITEM_DROP

// 최대 드랍 레벨
#define		MAX_CC_SETITEM_DROP_LEVEL			5		
// 레벨당 최대 세트 아이템 인덱스 수
#define		MAX_CC_SETITEM_INDEX_COUNT_PER_LEVEL	30

typedef struct 
{
	int	DropSetItemIndex[MAX_CC_SETITEM_INDEX_COUNT_PER_LEVEL];
	int	IndexCount;
	int	DropRate;
}CCSetItemDropInfo;

// 카오스 캐슬 세트 아이템 드롭 설정
class CChaosCastleSetItemDrop  
{
	CCSetItemDropInfo	DropExItemIndex[MAX_CC_SETITEM_DROP_LEVEL];
public:
	CChaosCastleSetItemDrop();
	virtual ~CChaosCastleSetItemDrop();


	void	Init();
	BOOL	Load(char* filename);

	int		GetItemIndex();
	void	MakeRandomSetItem(int aIndex);
};

extern CChaosCastleSetItemDrop	gChaosCastleSetItemDrop;
#endif

#endif // !defined(AFX_CHAOSCASTLESETITEMDROP_H__297BDBE7_C2A9_4A74_B434_81151B137CF4__INCLUDED_)
