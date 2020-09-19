// ProbabilityItemBag.h: interface for the CProbabilityItemBag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROBABILITYITEMBAG_H__C242733D_E3BF_4B72_96A8_79711B78FBE3__INCLUDED_)
#define AFX_PROBABILITYITEMBAG_H__C242733D_E3BF_4B72_96A8_79711B78FBE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemBagEx.h"
	
#define MAX_ITEMRATE_COUNT	20					// 아이템별 드럽확률 최대 저장 수

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
class CItemBagAttrEx2: public CItemBagAttrEx
{
public:
	BYTE	m_isSetItem;	// 셋트아이템

	CItemBagAttrEx2()
	{
		m_isSetItem = 0;
	}
};
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

class CProbabilityItemBag  
{
public:
	CProbabilityItemBag();
	virtual ~CProbabilityItemBag();

private:
	BOOL		m_bLoad;
	char		m_sEventName[255];					// 이벤트 이름	
	int			m_iEventItemType;					// 떨어질 이벤트 아이템(아이템번호)
	int			m_iEventItemLevel;					// 떨어질 이벤트 아이템 레벨

	int			m_iDropZen;							// 아이템이 떨어지지 않을때 떨어질 젠
	int			m_iEventItemDropRate;				// 이벤트 아이템이 떨어질 확률
	int			m_iItemDropRate;					// 이벤트 아이템에서 아이템이 떨어질 확률
	int			m_iExItemDropRate;					// 아이템이 떨어질때 엑설런트 아이템이 떨어질 확률	

	int			m_iBagObjectCount;					// 아이템 리스트 수

	CItemBagDropMapInfo	*DropMapInfo;		// 이벤트 아이템이 드롭될 맵 정보

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
	CItemBagAttrEx2		BagObject[MAX_ITEMBAGEX];	// 아이템 리스트
#else	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
	CItemBagAttrEx		BagObject[MAX_ITEMBAGEX];	// 아이템 리스트
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

	int			m_iEventItemDropRateEx[MAX_ITEMRATE_COUNT];		// 아이템별 드롭 확률
	int			m_iDropRatePerItemCount[MAX_ITEMRATE_COUNT];	// 확률당 아이템 수
	int			m_iRateKindCount;								// 확률 종류 개수

private:
	int		GetBagCount(){return m_iBagObjectCount;}
	BOOL	IsEnableEventItemDrop(int aIndex);		// 아이템드롭이 가능한가?
	
	BYTE	GetLevel(int n);
	BYTE	IsSkill(int n);
	BYTE	IsLuck(int n);
	BYTE	IsOption(int n);

public:
	void	ProbabilityItemBagInit(char * name);
	void	LoadItem(char *script_file);

	int		GetDropItemNumber(int nDropItemRate);
	int		GetDropItemNumberEx(int nDropItemRate);
	
#ifdef MODIFY_EVENTITEMBAG_BASIC_SET_20070823
	char*	GetEventName();
#endif

public:

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	// 설날복주머니 아이템 드럽
	BOOL DropNewYearLuckyBagEventItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
#endif

#ifdef UPDATE_GM_FUNCTION_20070228	// GM 선물상자 아이템 드랍
	BOOL DropGMPresentBoxEventItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
#endif
	
#ifdef PCBANG_POINT_SYSTEM_20070206						// 혼돈 상자에서 아이템 드랍 함수 선언		 
	BOOL DropBoxOfGreenChaosItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
	BOOL DropBoxOfRedChaosItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
	BOOL DropBoxOfPurpleChaosItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	int DropIllusionTempleGhostItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
#endif

#ifdef MODIFY_EVENTITEMBAG_BASIC_SET_20070823		// 아이템 드롭 기본
	BOOL DropEventItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );	
#endif	

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329		// 셋트아이템을 생성
	void DropEventSetItem(int aIndex, int itemnum, BYTE btMapNumber, int X, int Y);
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	BOOL DropCherryBlossomEventItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY, int* pMoney = NULL);
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_RAKLION_20080408					// 세루판 아이템 드롭
	BOOL	DropRaklionSelupanItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif // ADD_RAKLION_20080408
	
#ifdef ADD_NEWPVP_PKFIELD
	// 1/10000 * 100 확률 함수
	BOOL DropEventItemEx( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif // ADD_NEWPVP_PKFIELD
};

#endif // !defined(AFX_PROBABILITYITEMBAG_H__C242733D_E3BF_4B72_96A8_79711B78FBE3__INCLUDED_)
