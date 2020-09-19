// KalimaGate.h: interface for the CKalimaGate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KALIMAGATE_H__4E0C037D_DA6B_4CED_8C8B_DEE36ED82342__INCLUDED_)
#define AFX_KALIMAGATE_H__4E0C037D_DA6B_4CED_8C8B_DEE36ED82342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_NEW_MAP_KALIMA_20040518

#include "user.h"
#include "gObjMonster.h"

#define KALIMAGATE_SUMMON_FIELDSIZE		5				// ����� �ֺ��� ������ ��ȯ�ϴ� ��ǥ���� (N x N)

#ifdef HIDDEN_KALIMA_20050706
#define MAX_KALIMAGATE_COUNT			7				// ���� ���� ����
#else
#define MAX_KALIMAGATE_COUNT			6				// ���� ���� ����
#endif

//#define MAX_KALIMAGATE_STONE_OVERLAP	20				// ������ �ִ� �������� �� 
// ### 20040906 (apple) 
//#define MAX_KALIMAGATE_STONE_OVERLAP	10				// ������ �ִ� �������� �� (10���� ���� - apple)
#define MAX_KALIMAGATE_STONE_OVERLAP	5				// ������ �ִ� �������� �� (5���� ���� - b4nfter)
//#define MAX_KALIMAGATE_LIFE_TICKTERM	300000			// ������ ��� �ִ� �ð�
// ### 20040906 (apple) 
#define MAX_KALIMAGATE_LIFE_TICKTERM	180000			// ������ ��� �ִ� �ð�

#ifdef HIDDEN_KALIMA_20050706
#define MAX_KALIMAGATE_LIFE_TICKTERM2	60000			// ������ ��� �ִ� �ð� (Ư�� ����)
#endif

#define MAX_KALIMAGATE_ENTER_COUNT		5				// ������ ���� ������ �� �ִ� �ִ� Ƚ��


#define GATE_INDEX_KALIMA_1				88				// Į���� 1
#define GATE_INDEX_KALIMA_2				89				// Į���� 2
#define GATE_INDEX_KALIMA_3				90				// Į���� 3
#define GATE_INDEX_KALIMA_4				91				// Į���� 4
#define GATE_INDEX_KALIMA_5				92				// Į���� 5
#define GATE_INDEX_KALIMA_6				93				// Į���� 6
#ifdef HIDDEN_KALIMA_20050706
#define GATE_INDEX_KALIMA_7				116				// Į���� 7 (���� Į����)
#endif

#define MONSTER_INDEX_KALIMA_1			152				// ���� �ε��� 1
#define MONSTER_INDEX_KALIMA_2			153				// ���� �ε��� 2
#define MONSTER_INDEX_KALIMA_3			154				// ���� �ε��� 3
#define MONSTER_INDEX_KALIMA_4			155				// ���� �ε��� 4
#define MONSTER_INDEX_KALIMA_5			156				// ���� �ε��� 5
#define MONSTER_INDEX_KALIMA_6			157				// ���� �ε��� 6
#ifdef HIDDEN_KALIMA_20050706
#define MONSTER_INDEX_KALIMA_7			158				// ���� �ε��� 7
#endif

typedef struct _KALIMAGATE_LEVEL {
	INT iLOWER_BOUND;									// ���Ѽ�
	INT iUPPER_BOUND;									// ���Ѽ�
	INT iLOWER_BOUND_MAGUMSA;							// ���Ѽ� - ���˻�
	INT iUPPER_BOUND_MAGUMSA;							// ���Ѽ� - ���˻�
} KALIMAGATE_LEVEL, * LPKALIMAGATE_LEVEL;

/*
static KALIMAGATE_LEVEL	g_sttKALIMAGATE_LEVEL[MAX_KALIMAGATE_COUNT] = {
	50,		119,		30,		99,
	120,	179,		100,	159,
	180,	239,		160,	219,
	240,	299,		220,	279,
	300,	349,		280,	319,
	350,	MAX_LEVEL,	320,	MAX_LEVEL,
	};
*/

// ### 20040906 (apple) 

#ifdef HIDDEN_KALIMA_20050706
static int g_iKalimaGateGateIndex[MAX_KALIMAGATE_COUNT] = {
	88,													// Į���� 1
	89,													// Į���� 2
	90,													// Į���� 3
	91,													// Į���� 4
	92,													// Į���� 5
	93,													// Į���� 6
	116,												// Į���� 7
};

static KALIMAGATE_LEVEL	g_sttKALIMAGATE_LEVEL[MAX_KALIMAGATE_COUNT] = {		// "GameMain.cpp" �� MapMinUserLevel[] �� �����͵� �Բ� �����Ǿ�� ��
	40,		130,		20,		110,					// Į���� 1
	131,	180,		111,	160,					// Į���� 2
	181,	230,		161,	210,					// Į���� 3
	231,	280,		211,	260,					// Į���� 4
	281,	330,		261,	310,					// Į���� 5
#ifdef UPDATE_LOWLEVEL_SUPPORT_1_20051215
	331,	349,		311,	349,					// Į���� 6
	350,	MAX_LEVEL,	350,	MAX_LEVEL,				// Į���� 7
#else
	331,	379,		311,	379,					// Į���� 6
	380,	MAX_LEVEL,	380,	MAX_LEVEL,				// Į���� 7
#endif
	};
#else
static KALIMAGATE_LEVEL	g_sttKALIMAGATE_LEVEL[MAX_KALIMAGATE_COUNT] = {		// "GameMain.cpp" �� MapMinUserLevel[] �� �����͵� �Բ� �����Ǿ�� ��
	15,		49,			15,		29,
	50,		119,		30,		99,
	120,	179,		100,	159,
	180,	239,		160,	219,
	240,	299,		220,	279,
	300,	MAX_LEVEL,	280,	MAX_LEVEL,
	};
#endif


class CKalimaGate  
{
	CRITICAL_SECTION	m_critKalimaGate;

public:
					CKalimaGate				();
	virtual			~CKalimaGate			();
#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
	BOOL	CreateKalimaGate		(			// Ư�� ������� ������ ��ȯ. ���� ���� üũ (apple)
		INT		iIndex,
		BYTE	btLevel, 
		UCHAR	cTX, 
		UCHAR	cTY 
		);
#else
	BOOL	CreateKalimaGate		(			// Ư�� ������� ������ ��ȯ
		INT		iIndex,
		UCHAR	cTX, 
		UCHAR	cTY 
		);
#endif

	VOID	KalimaGateAct			(			// ������ �ൿ���
		INT		iIndex
		);

#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
	INT		CheckOverlapKundunMark	(			// ����� ǥ���� �κ��丮�� ��ĥ �� �ִ��� Ȯ���Ѵ�. 
		INT		iIndex,
		BYTE	btLevel
		);
#else
	INT		CheckOverlapKundunMark	(			// ����� ǥ���� �κ��丮�� ��ĥ �� �ִ��� Ȯ���Ѵ�. 
		INT		iIndex
		);
#endif

	BOOL	DeleteKalimaGate		(			// Į���� ����Ʈ�� �����Ѵ�.
		INT		iKalimaGateIndex,
		INT		iCallOwnerIndex
		);
	BOOL	DeleteKalimaGate		(			// Į���� ����Ʈ�� �����Ѵ�.
		INT		iCallOwnerIndex
		);

#ifdef HIDDEN_KALIMA_20050706					// ���� ������ ������ Į���� ����Ʈ�� ����, ���� �Լ�
	BOOL	CreateKalimaGate2		(			// ���� ������ ������ Į���� ����Ʈ�� ����
		INT		iIndex,
		INT		iMonMapNumber,
		UCHAR	cTX, 
		UCHAR	cTY 
		);

	VOID	KalimaGateAct2			(			// ���� ������ ������ Į���� ����Ʈ�� ����
		INT		iIndex
		);
#endif

#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
	INT		GetKalimaGateLevel2		(
		INT		iIndex
		);
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

private:

	BOOL	GetRandomLocation		(			// ����� �ݰ泻�� ������ ��ǥ�� �� �����Ѵ�.
		INT		iMapNumber,
		UCHAR &	cX,
		UCHAR & cY
		);
	INT		GetKalimaGateLevel		(			// ����ڰ� ��� ������ ������ ������ ��´�.
		INT		iIndex
		);
};


extern CKalimaGate	g_KalimaGate;


#endif


#endif // !defined(AFX_KALIMAGATE_H__4E0C037D_DA6B_4CED_8C8B_DEE36ED82342__INCLUDED_)
