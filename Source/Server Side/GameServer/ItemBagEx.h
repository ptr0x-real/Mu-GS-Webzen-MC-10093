// ItemBagEx.h: interface for the CItemBagEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMBAGEX_H__D3BB88D8_6ABA_4A8B_B1BF_97716E97A2E4__INCLUDED_)
#define AFX_ITEMBAGEX_H__D3BB88D8_6ABA_4A8B_B1BF_97716E97A2E4__INCLUDED_

#include "Common/zzzitem.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_ITEMBAGEX	150

class CItemBagAttrEx
{
public :
	BYTE m_type;
	BYTE m_index;
	BYTE m_minLevel;
	BYTE m_maxLevel;
	BYTE m_isskill;
	BYTE m_isluck;
	BYTE m_isoption;
	BYTE m_isexitem;

	CItemBagAttrEx()
	{
		m_type  =0;
		m_index  =0;
		m_minLevel  =0;
		m_maxLevel  =0;
		m_isskill  =0;
		m_isluck  =0;
		m_isoption  =0;
		m_isexitem = 0;
	}
};

class CItemBagDropMapInfo
{
public :
	BYTE	m_bIsDrop;				// 드롭 되는가?
	BYTE	m_MinMonsterLevel;		// 최소 몬스터 드롭 레벨
	BYTE	m_MaxMonsterLevel;		// 최대 몬스터 드롭 레벨

	CItemBagDropMapInfo()
	{		
		Init();
	}

	void	Init()
	{
		m_bIsDrop = 0;
		m_MinMonsterLevel = 0;
		m_MaxMonsterLevel = 0;
	}
};

class CItemBagEx  
{
	BOOL		m_bLoad;
	char		m_sEventName[255];					// 이벤트 이름	
	int			m_iEventItemType;					// 떨어질 이벤트 아이템(아이템번호)
	int			m_iEventItemLevel;					// 떨어질 이벤트 아이템 레벨

	int			m_iDropZen;							// 아이템이 떨어지지 않을때 떨어질 젠
	int			m_iEventItemDropRate;				// 이벤트 아이템이 떨어질 확률
	int			m_iItemDropRate;					// 이벤트 아이템에서 아이템이 떨어질 확률
	int			m_iExItemDropRate;					// 아이템이 떨어질때 엑설런트 아이템이 떨어질 확률	

	int			m_iBagObjectCount;					// 아이템 리스트 수

	CItemBagDropMapInfo	DropMapInfo[MAX_MAP];		// 이벤트 아이템이 드롭될 맵 정보
	CItemBagAttrEx	BagObject[MAX_ITEMBAGEX];		// 아이템 리스트
	
public:
	CItemBagEx();
	virtual ~CItemBagEx();

	void	Init(char * name);
	void	LoadItem(char *script_file);
#ifdef SCRIPT_DECODE_WORK
	void	LoadItemDecode(char *script_file);
#endif

	BOOL	DropEventItem(int aIndex);				// 이벤트 아이템 드롭
	BOOL	DropItem(int aIndex);					// 아이템 드롭
	
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
	BOOL	DropRedRibbonBoxEventItem( int aIndex );
	BOOL	DropGreenRibbonBoxEventItem( int aIndex );
	BOOL	DropBlueRibbonBoxEventItem( int aIndex );	
#endif

#ifdef VALENTINES_DAY_BOX_EVENT_20060124
	BOOL	DropPinkChocolateBoxEventItem( int aIndex );
	BOOL	DropRedChocolateBoxEventItem( int aIndex );
	BOOL	DropBlueChocolateBoxEventItem( int aIndex );	
#endif

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
	BOOL	DropLightPurpleCandyBoxEventItem( int aIndex );
	BOOL	DropVermilionCandyBoxEventItem( int aIndex );
	BOOL	DropDeepBlueCandyBoxEventItem( int aIndex );	
#endif

#ifdef MODIFY_KUNDUN_REWARD_01_20040915
// MODIFY_ITEM_DROP_MAP_OF_EROHIM_KUNDUN_20050610
	BOOL	DropKundunEventItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);		// 쿤둔 이벤트 아이템 드롭
#endif
	
#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303
// MODIFY_ITEM_DROP_MAP_OF_EROHIM_KUNDUN_20050610
	BOOL	DropCastleHuntZoneBossReward(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0); // 성사냥터 보스몹 아이템 드롭
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404	// 다크엘프 아이템백 드롭 처리
	BOOL	DropCrywolfDarkElfItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404		// 발가스 아이템백 드롭 처리
	BOOL	DropCrywolfBossMonsterItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif
	
#ifdef KANTURU_MONSTER_ITEMDROP_20060807
	BOOL	DropKanturuMayaHandItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );	// 마야 손 아이템 드롭
	BOOL	DropKanturuNightmareItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );	// 나이트메어 아이템 드롭
#endif
	
#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
	BOOL	DropHallowinEventItem(LPOBJECTSTRUCT lpOBj);
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
	
#ifdef PCBANG_COUPON_EVENT_20060124
	BOOL	DropPCBangRevitalizationEventItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif
	
#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// 레오 아이템 드랍 함수 선언
	char*	GetEventName();
	BOOL	DropItemGiveEventItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif

//#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
	BYTE	GetMinLevel(int n);
	BYTE	GetMaxLevel(int n);
	BOOL	PickItem (CItem & objItem, int & nItemIndex);
//#endif
	
private:

	int		GetBagCount(){return m_iBagObjectCount;}
	BOOL	IsEnableEventItemDrop(int aIndex);		// 아이템드롭이 가능한가?
	
	BYTE	GetLevel(int n);
	BYTE	IsSkill(int n);
	BYTE	IsLuck(int n);
	BYTE	IsOption(int n);	
};

#endif // !defined(AFX_ITEMBAGEX_H__D3BB88D8_6ABA_4A8B_B1BF_97716E97A2E4__INCLUDED_)
