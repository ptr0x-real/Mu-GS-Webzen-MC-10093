// ProbabilityItemBag.h: interface for the CProbabilityItemBag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROBABILITYITEMBAG_H__C242733D_E3BF_4B72_96A8_79711B78FBE3__INCLUDED_)
#define AFX_PROBABILITYITEMBAG_H__C242733D_E3BF_4B72_96A8_79711B78FBE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemBagEx.h"
	
#define MAX_ITEMRATE_COUNT	20					// �����ۺ� �巴Ȯ�� �ִ� ���� ��

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
class CItemBagAttrEx2: public CItemBagAttrEx
{
public:
	BYTE	m_isSetItem;	// ��Ʈ������

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
	char		m_sEventName[255];					// �̺�Ʈ �̸�	
	int			m_iEventItemType;					// ������ �̺�Ʈ ������(�����۹�ȣ)
	int			m_iEventItemLevel;					// ������ �̺�Ʈ ������ ����

	int			m_iDropZen;							// �������� �������� ������ ������ ��
	int			m_iEventItemDropRate;				// �̺�Ʈ �������� ������ Ȯ��
	int			m_iItemDropRate;					// �̺�Ʈ �����ۿ��� �������� ������ Ȯ��
	int			m_iExItemDropRate;					// �������� �������� ������Ʈ �������� ������ Ȯ��	

	int			m_iBagObjectCount;					// ������ ����Ʈ ��

	CItemBagDropMapInfo	*DropMapInfo;		// �̺�Ʈ �������� ��ӵ� �� ����

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
	CItemBagAttrEx2		BagObject[MAX_ITEMBAGEX];	// ������ ����Ʈ
#else	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
	CItemBagAttrEx		BagObject[MAX_ITEMBAGEX];	// ������ ����Ʈ
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

	int			m_iEventItemDropRateEx[MAX_ITEMRATE_COUNT];		// �����ۺ� ��� Ȯ��
	int			m_iDropRatePerItemCount[MAX_ITEMRATE_COUNT];	// Ȯ���� ������ ��
	int			m_iRateKindCount;								// Ȯ�� ���� ����

private:
	int		GetBagCount(){return m_iBagObjectCount;}
	BOOL	IsEnableEventItemDrop(int aIndex);		// �����۵���� �����Ѱ�?
	
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

#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	// �������ָӴ� ������ �巴
	BOOL DropNewYearLuckyBagEventItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
#endif

#ifdef UPDATE_GM_FUNCTION_20070228	// GM �������� ������ ���
	BOOL DropGMPresentBoxEventItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
#endif
	
#ifdef PCBANG_POINT_SYSTEM_20070206						// ȥ�� ���ڿ��� ������ ��� �Լ� ����		 
	BOOL DropBoxOfGreenChaosItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
	BOOL DropBoxOfRedChaosItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
	BOOL DropBoxOfPurpleChaosItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	int DropIllusionTempleGhostItem(int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0);
#endif

#ifdef MODIFY_EVENTITEMBAG_BASIC_SET_20070823		// ������ ��� �⺻
	BOOL DropEventItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );	
#endif	

#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329		// ��Ʈ�������� ����
	void DropEventSetItem(int aIndex, int itemnum, BYTE btMapNumber, int X, int Y);
#endif	// UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	BOOL DropCherryBlossomEventItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY, int* pMoney = NULL);
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_RAKLION_20080408					// ������ ������ ���
	BOOL	DropRaklionSelupanItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif // ADD_RAKLION_20080408
	
#ifdef ADD_NEWPVP_PKFIELD
	// 1/10000 * 100 Ȯ�� �Լ�
	BOOL DropEventItemEx( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0 );
#endif // ADD_NEWPVP_PKFIELD
};

#endif // !defined(AFX_PROBABILITYITEMBAG_H__C242733D_E3BF_4B72_96A8_79711B78FBE3__INCLUDED_)
