// ItemBag.h: interface for the CItemBag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMBAG_H__15EE26B2_9BF9_47CE_ADD7_9A0F6E06B825__INCLUDED_)
#define AFX_ITEMBAG_H__15EE26B2_9BF9_47CE_ADD7_9A0F6E06B825__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef DARK_LORD_HEART_EVENT_20040810
#define MAX_ITEMBAG		150
#else
#define MAX_ITEMBAG		100
#endif

class CItemBagAttr
{
public :
	BYTE m_type;
	BYTE m_index;
	BYTE m_level;
	BYTE m_op1;
	BYTE m_op2;
	BYTE m_op3;

	CItemBagAttr()
	{
		m_type  =0;
		m_index =0;
		m_level =0;
		m_op1   =0;
		m_op2   =0;
		m_op3   =0;
	}
};


class CItemBag  
{

	CItemBagAttr BagObject[MAX_ITEMBAG];
	int			 BagObjectCount;

	int			 BagNormalItemCount;
	int			 BagExItemCount;
public:

	
	CItemBag();
	virtual ~CItemBag();

	void Init(char * name = "eventitembag.txt");
	void LoadItem(char *script_file);
	
#ifdef SCRIPT_DECODE_WORK
	void LoadItemDecode(char *script_file);
#endif

	int GetBagCount()
	{
		return BagObjectCount;
	}
	BYTE GetType(int n);
	BYTE GetIndex(int n);
	BYTE GetLevel(int n);
	BYTE GetOp1(int n);
	BYTE GetOp2(int n);
	BYTE GetOp3(int n);

	int	GetNormalItemCount(){return BagNormalItemCount;}
	int	GetExItemCount(){return BagExItemCount;}
};

#endif // !defined(AFX_ITEMBAG_H__15EE26B2_9BF9_47CE_ADD7_9A0F6E06B825__INCLUDED_)
