// GamblingSystem.h: interface for the CGamblingItemBag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMBLINGSYSTEM_H__32FE5097_1ED4_421E_AD70_E598D3CD6EEE__INCLUDED_)
#define AFX_GAMBLINGSYSTEM_H__32FE5097_1ED4_421E_AD70_E598D3CD6EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemBagEx.h"

// �߰� �ɼ� �迭 �ε����� ����ϱ� ���� enum ����
enum {
	ADD_OPTION_PLUS_4 = 0,
	ADD_OPTION_PLUS_8 = 1,
	ADD_OPTION_PLUS_12 = 2,
	ADD_OPTION_PLUS_16 = 3,

	ADD_OPTION_COUNT,
};

// �׺����� �����ϴ� �������� �׷� ��ȣ enum ����
enum {
	GAMBILNG_ITEM_TYPE_SWORD	= 0,
	GAMBILNG_ITEM_TYPE_STAFF	= 1,
	GAMBILNG_ITEM_TYPE_BOW		= 2,
	GAMBILNG_ITEM_TYPE_SCEPTER	= 3,
	GAMBILNG_ITEM_TYPE_STICK	= 4,

	GAMBILNG_ITEM_TYPE_COUNT,
};

// �׺� �����ۿ� �ɼ��� ������ ���� Ȯ��
enum {
	ATTACH_1_OPTION_RATE = 0,		// �ɼ��� 1�� ���� Ȯ�� �ε���
	ATTACH_2_OPTION_RATE = 1,		// �ɼ��� 2�� ���� Ȯ�� �ε���
	ATTACH_3_OPTION_RATE = 2,		// �ɼ��� 3�� ���� Ȯ�� �ε���
	ATTACH_4_OPTION_RATE = 3,		// �ɼ��� 4�� ���� Ȯ�� �ε���

	ATTACH_OPTION_COUNT,
};

// �� ������Ʈ �ɼ��� ���� Ȯ�� �ε���
enum {
	EX_1_OPTION_RATE = 0,		// ù��° ������Ʈ �ɼ��� ���� Ȯ��
	EX_2_OPTION_RATE = 1,		// �ι�° ������Ʈ �ɼ��� ���� Ȯ��
	EX_3_OPTION_RATE = 2,		// ����° ������Ʈ �ɼ��� ���� Ȯ��
	EX_4_OPTION_RATE = 3,		// �׹�° ������Ʈ �ɼ��� ���� Ȯ��
	EX_5_OPTION_RATE = 4,		// �ٹ�° ������Ʈ �ɼ��� ���� Ȯ��
	EX_6_OPTION_RATE = 5,		// ������° ������Ʈ �ɼ��� ���� Ȯ��
		
	EX_OPTION_COUNT,
};

// ��ũ��Ʈ�� ������ �׺� �������� �׷� ��ȣ
enum {
	// �Ϲ� ���� �׷�
	ITEM_GROUP_NORMAL_SWORD_1	= 0,
	ITEM_GROUP_NORMAL_SWORD_2	= 1,
	ITEM_GROUP_NORMAL_SWORD_3	= 2,
	ITEM_GROUP_NORMAL_SWORD_4	= 3,
	ITEM_GROUP_NORMAL_SWORD_5	= 4,
	ITEM_GROUP_NORMAL_STAFF_1	= 5,
	ITEM_GROUP_NORMAL_STAFF_2	= 6,
	ITEM_GROUP_NORMAL_STAFF_3	= 7,
	ITEM_GROUP_NORMAL_STAFF_4	= 8,
	ITEM_GROUP_NORMAL_STAFF_5	= 9,
	ITEM_GROUP_NORMAL_BOW_1		= 10,
	ITEM_GROUP_NORMAL_BOW_2		= 11,
	ITEM_GROUP_NORMAL_BOW_3		= 12,
	ITEM_GROUP_NORMAL_BOW_4		= 13,
	ITEM_GROUP_NORMAL_BOW_5		= 14,
	ITEM_GROUP_NORMAL_SCEPTER_1	= 15,
	ITEM_GROUP_NORMAL_SCEPTER_2	= 16,
	ITEM_GROUP_NORMAL_SCEPTER_3	= 17,
	ITEM_GROUP_NORMAL_SCEPTER_4	= 18,
	ITEM_GROUP_NORMAL_SCEPTER_5	= 19,
	ITEM_GROUP_NORMAL_STICK_1	= 20,
	ITEM_GROUP_NORMAL_STICK_2	= 21,
	ITEM_GROUP_NORMAL_STICK_3	= 22,
	ITEM_GROUP_NORMAL_STICK_4	= 23,
	ITEM_GROUP_NORMAL_STICK_5	= 24,

	ITEM_GROUP_MAX,
};

typedef struct _GAMBLING_RATE
{
	int		iSkillRate;		// �� ������ �׷캰 ��ų ���� Ȯ��
	int		iLuckRate;		// �� ������ �׷캰 ��� ���� Ȯ��
	int		iAddOptionRate;	// �� ������ �׷캰 �߰� �ɼ� ���� Ȯ��
	int		iExcellentRate;	// �� ������ �׷캰 ������Ʈ �������� Ȯ��
	int		iSelectedRate;	// �� ������ �׷캰 ���õǾ��� Ȯ��

	_GAMBLING_RATE()
	{
		iSkillRate = 0;
		iLuckRate = 0;
		iAddOptionRate = 0;
		iExcellentRate = 0;
		iSelectedRate = 0;
	}
} GAMBLING_RATE, *LPGAMBLING_RATE;

class CGamblingItemBag
{
public:
	CGamblingItemBag();
	virtual ~CGamblingItemBag();

private:
	BOOL	m_bLoad;
	
	GAMBLING_RATE m_stGamblingRate[ITEM_GROUP_MAX];			// Ȯ�� ���� ����ü ����
	int		m_iAddOption[ADD_OPTION_COUNT];					// �߰� �ɼ��� ������ �� �߰� �ɼǺ� ���� Ȯ��
	int		m_iGamblingItemGetRate[GAMBILNG_ITEM_TYPE_COUNT];	// �� Ÿ�Ժ� �׺� �������� ���� Ȯ��
	int		m_iGamblingOptionCountRate[ATTACH_OPTION_COUNT];	// �׺� �����ۿ� �ɼ��� ������ ���� Ȯ��
	int		m_iGamblingExOptionRate[EX_OPTION_COUNT];		// �׺� �����ۿ� �� ������Ʈ �ɼ��� ���� Ȯ��
	int		m_iNormalExOptionRate[EX_OPTION_COUNT];			// ���� ������Ʈ �����ۿ� �� ������Ʈ �ɼ��� ���� Ȯ��

	int		m_iBagObjectCount;							// ������ ����Ʈ ��
	int		m_iDropRatePerItemCount[ITEM_GROUP_MAX];	// Ȯ���� ������ ��
	int		m_iRateKindCount;							// Ȯ�� ���� ����

	CItemBagAttrEx		BagObject[MAX_ITEMBAGEX];		// ������ ����Ʈ

private:
	int		GetBagCount(){return m_iBagObjectCount;}
	
	BYTE	GetLevel(int n);
	BYTE	IsSkill(int n);
	BYTE	IsLuck(int n);
	BYTE	IsOption(int n);

public:
	void	GamblingItemBagInit(char * name);
	void	LoadItem(char *script_file);

	BYTE	GetGamblingSystemItem( int aIndex, BYTE btMapNumber, UCHAR cX = 0, UCHAR cY = 0, BYTE btItemType = 0 );

	BYTE	GamblingOptionRand( int iOptionCount );
	BYTE	ExcellentOptionRand();
};

#endif // !defined(AFX_GAMBLINGSYSTEM_H__32FE5097_1ED4_421E_AD70_E598D3CD6EEE__INCLUDED_)
