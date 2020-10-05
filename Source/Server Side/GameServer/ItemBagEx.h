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
	BYTE	m_bIsDrop;				// ��� �Ǵ°�?
	BYTE	m_MinMonsterLevel;		// �ּ� ���� ��� ����
	BYTE	m_MaxMonsterLevel;		// �ִ� ���� ��� ����

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
	char		m_sEventName[255];					// �̺�Ʈ �̸�	
	int			m_iEventItemType;					// ������ �̺�Ʈ ������(�����۹�ȣ)
	int			m_iEventItemLevel;					// ������ �̺�Ʈ ������ ����

	int			m_iDropZen;							// �������� �������� ������ ������ ��
	int			m_iEventItemDropRate;				// �̺�Ʈ �������� ������ Ȯ��
	int			m_iItemDropRate;					// �̺�Ʈ �����ۿ��� �������� ������ Ȯ��
	int			m_iExItemDropRate;					// �������� �������� ������Ʈ �������� ������ Ȯ��	

	int			m_iBagObjectCount;					// ������ ����Ʈ ��

	CItemBagDropMapInfo	DropMapInfo[MAX_MAP];		// �̺�Ʈ �������� ��ӵ� �� ����
	CItemBagAttrEx	BagObject[MAX_ITEMBAGEX];		// ������ ����Ʈ
	
public:
	CItemBagEx();
	virtual ~CItemBagEx();

	void	Init(char * name);
	void	LoadItem(char *script_file);
#ifdef SCRIPT_DECODE_WORK
	void	LoadItemDecode(char *script_file);
#endif

	BOOL	DropEventItem(int aIndex);				// �̺�Ʈ ������ ���
	BOOL	DropItem(int aIndex);					// ������ ���
	
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
	BOOL	DropKundunEventItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);		// ��� �̺�Ʈ ������ ���
#endif
	
#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303
// MODIFY_ITEM_DROP_MAP_OF_EROHIM_KUNDUN_20050610
	BOOL	DropCastleHuntZoneBossReward(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0); // ������� ������ ������ ���
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404	// ��ũ���� �����۹� ��� ó��
	BOOL	DropCrywolfDarkElfItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404		// �߰��� �����۹� ��� ó��
	BOOL	DropCrywolfBossMonsterItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif
	
#ifdef KANTURU_MONSTER_ITEMDROP_20060807
	BOOL	DropKanturuMayaHandItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );	// ���� �� ������ ���
	BOOL	DropKanturuNightmareItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );	// ����Ʈ�޾� ������ ���
#endif
	
#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
	BOOL	DropHallowinEventItem(LPOBJECTSTRUCT lpOBj);
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
	
#ifdef PCBANG_COUPON_EVENT_20060124
	BOOL	DropPCBangRevitalizationEventItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif
	
#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	// ���� ������ ��� �Լ� ����
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
	BOOL	IsEnableEventItemDrop(int aIndex);		// �����۵���� �����Ѱ�?
	
	BYTE	GetLevel(int n);
	BYTE	IsSkill(int n);
	BYTE	IsLuck(int n);
	BYTE	IsOption(int n);	
};

#endif // !defined(AFX_ITEMBAGEX_H__D3BB88D8_6ABA_4A8B_B1BF_97716E97A2E4__INCLUDED_)
