// JewelMixSystem.h: interface for the CJewelMixSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JEWELMIXSYSTEM_H__82324472_99D3_42A7_B4F7_2B469891AF85__INCLUDED_)
#define AFX_JEWELMIXSYSTEM_H__82324472_99D3_42A7_B4F7_2B469891AF85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\Common\\winutil.h"


#ifdef	JEWEL_MIX_SYSTEM_20050309

#define	MAX_JM_JEWEL_TYPE	2				// �ִ� ���� ����
#define	MAX_JM_MIX_TYPE		3				// �ִ� ���� ����

enum	{									// ���� Ÿ��
	JEWEL_TYPE_BLESS		= 0,			// �ູ�� ����
	JEWEL_TYPE_SOUL			= 1,			// ��ȥ�� ����
};

enum	{									// ���� Ÿ��
	JEWEL_COUNT_10			= 0,			// ���� ���� ���� - 10��
	JEWEL_COUNT_20			= 1,			// ���� ���� ���� - 20��
	JEWEL_COUNT_30			= 2,			// ���� ���� ���� - 30��
};

typedef struct {
	INT		m_iJewelCount;
	INT		m_iMixMoney;
	INT		m_iSourceType;
	INT		m_iChangeType;
} JEWELMIX_INFO, * LPJEWELMIX_INFO;

const JEWELMIX_INFO	g_JewelMixInfo[MAX_JM_JEWEL_TYPE][3]	= {		// �ι�° ���� - �� �ε������� 10, 20, 30 �� �϶�
	// �ູ�� ����
	10,		500000,		MAKE_ITEMNUM(14,13),	MAKE_ITEMNUM(12,30),
	20,		1000000,	MAKE_ITEMNUM(14,13),	MAKE_ITEMNUM(12,30),
	30,		1500000,	MAKE_ITEMNUM(14,13),	MAKE_ITEMNUM(12,30),

	// ��ȥ�� ����
	10,		500000,		MAKE_ITEMNUM(14,14),	MAKE_ITEMNUM(12,31),
	20,		1000000,	MAKE_ITEMNUM(14,14),	MAKE_ITEMNUM(12,31),
	30,		1500000,	MAKE_ITEMNUM(14,14),	MAKE_ITEMNUM(12,31),
};

#define	JEWEL_UNMIX_MONEY				1000000						// �������� ��ü �� �ʿ��� ��


class CJewelMixSystem  
{

public:
	CJewelMixSystem						();
	virtual ~CJewelMixSystem			();

	static INT	GetJewelCount			(
		INT iIndex,
		INT iJewelType
		);
	static INT	GetJewelCountPerLevel	(
		INT iJewelType,
		INT iJewelLevel
		);
	static BOOL	MixJewel				(
		INT iIndex,
		INT iJewelType,
		INT iMixType
		);
	static BOOL	UnMixJewel				(
		INT iIndex,
		INT iJewelType,
		INT iJewelLevel,
		INT iInventoryPos
		);
};


#endif

#endif // !defined(AFX_JEWELMIXSYSTEM_H__82324472_99D3_42A7_B4F7_2B469891AF85__INCLUDED_)
