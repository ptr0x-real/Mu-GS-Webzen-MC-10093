// ChaosCastle.h: interface for the CChaosCastle class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4786)

#if !defined(AFX_CHAOSCASTLE_H__535E5C79_81B4_467D_94A0_4220F9A43D7F__INCLUDED_)
#define AFX_CHAOSCASTLE_H__535E5C79_81B4_467D_94A0_4220F9A43D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CHAOSCASTLE_SYSTEM_20040408

#include "..\INCLUDE\DEFINE.H"
#include "..\COMMON\WINUTIL.H"
#include "VECTOR"
#include "MAP"
#include "ALGORITHM"
#include <list>

using namespace std;


#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010			// īĳ �ִ� ���� ����
#define MAX_CHAOSCASTLE_COUNT			7							// ī���� ĳ���� �ִ밳��
#else
#define MAX_CHAOSCASTLE_COUNT			6							// ī���� ĳ���� �ִ밳��
#endif	// ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010

#define MAX_CHAOSCASTLE_USER_COUNT		70							// ī���� ĳ�� �� �ұ��� �ִ� ������

#define MAX_CHAOSCASTLE_MONSTER_COUNT	100							// ī���� ĳ�� �� �ұ��� �ִ� ���ͼ�

#define CC_MAX_START_WAIT_TERM			(1*60)						// ī���� ĳ���� �����ϱ� ���� ���ð� (��)

#define MAX_SEARCH_SETPOINT				5							// ����ڵ��� �ֺ����� �������� �� ���� �� ��õ��ϴ�  �ִ� Ƚ��

#define MAX_BLOWOUT_RANGE				3							// ����ڸ� ���ĳ��� �Ÿ��� �ִ� �� (0 ~ 4)

#define MAX_BLOWOUT_DISTANCE			(MAX_BLOWOUT_RANGE+1)		// ����ڸ� ���ĳ��� �Ÿ��� �ִ� �� (0 ~ 4)

#define MAX_BLOWOUT_PERCENT				50							// ���Ͱ� �׾��� �� ����ڸ� ���ĳ��� Ȯ��

#define MIN_PLAYABLE_USERCOUNT			2							// �÷��� ������ �ּ� �����

#define CHAOSCASTLE_KILL_USER_PT		1							// ī���� ĳ������ ����ڸ� �׿��� �� ����

#define CHAOSCASTLE_KILL_MONSTER_PT		2							// ī���� ĳ������ ���͸� �׿��� �� ����

#define CHAOSCASTLE_OFFLINE_GIFT_RATE	2							// ī���� ĳ�� �������� ��ǰ�� ��÷�� Ȯ��


#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ�� �� ��ȣ üũ
#define CHECK_CHAOSCASTLE(C_VALUE)		(C_VALUE==MAP_INDEX_CHAOSCASTLE7?TRUE:C_VALUE<MAP_INDEX_CHAOSCASTLE1?FALSE:C_VALUE>MAP_INDEX_CHAOSCASTLE6?FALSE:TRUE)
#else
#define CHECK_CHAOSCASTLE(C_VALUE)		(C_VALUE<MAP_INDEX_CHAOSCASTLE1?FALSE:C_VALUE>MAP_INDEX_CHAOSCASTLE6?FALSE:TRUE)
#endif

enum {
	CHAOSCASTLE_STATE_NONE				=0,							// �̺�Ʈ �ߴ� ����
	CHAOSCASTLE_STATE_CLOSED			=1,							// �̺�Ʈ ���� �� ������
	CHAOSCASTLE_STATE_PLAYING			=2,							// �̺�Ʈ ���� �� ����
	CHAOSCASTLE_STATE_PLAYEND			=3							// �̺�Ʈ ���� ����
};


enum {
	CHAOSCASTLE_PLAYCHECK_ING			=0,							// ��� �����Ȳ - ���� ��
	CHAOSCASTLE_PLAYCHECK_SUCCESS		=1,							// ��� �����Ȳ - ��� ����
	CHAOSCASTLE_PLAYCHECK_FAILED		=2,							// ��� �����Ȳ - ��� ����
};


// �� ī���� ĳ���� ����Ʈ ��ȣ
#define GATE_CHAOSCASTLE_1				82
#define GATE_CHAOSCASTLE_2				83
#define GATE_CHAOSCASTLE_3				84
#define GATE_CHAOSCASTLE_4				85
#define GATE_CHAOSCASTLE_5				86
#define GATE_CHAOSCASTLE_6				87
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ��7 ����Ʈ��ȣ �����ؾ���
#define GATE_CHAOSCASTLE_7				272
#endif

// ī���� ĳ�� �ð� ����ü
typedef struct {
	INT		m_iHour;					// ī���� ĳ���� ������ �ð�
	INT		m_iMinute;					// ī���� ĳ���� ������ ��
	
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	BYTE	m_btEvent;					// ī���� ĳ�� �̺�Ʈ ����
#endif
} CHAOSCASTLE_START_TIME, * LPCHAOSCASTLE_START_TIME;


/*===================================================
//	<< ī���� ĳ�� ���� ���� ����ü >>
// : ī���� ĳ���� �� ĳ���� ���� ���� ���ѿ� ���� ������ ������ �ִ�.
===================================================*/
typedef struct _CHAOSCASTLE_LEVEL {
	INT iLOWER_BOUND;					// ���Ѽ�
	INT iUPPER_BOUND;					// ���Ѽ�
	INT iLOWER_BOUND_MAGUMSA;			// ���Ѽ� - ���˻�
	INT iUPPER_BOUND_MAGUMSA;			// ���Ѽ� - ���˻�
} CHAOSCASTLE_LEVEL, * LPCHAOSCASTLE_LEVEL;


static CHAOSCASTLE_LEVEL g_sttCHAOSCASTLE_LEVEL[MAX_CHAOSCASTLE_COUNT] = {
	15,		49,			15,		29,
	50,		119,		30,		99,
	120,	179,		100,	159,
	180,	239,		160,	219,
	240,	299,		220,	279,
	300,	MAX_LEVEL,	280,	MAX_LEVEL,
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// ī����ĳ�� �����ͷ��� ��� ���� ����
	MAX_LEVEL,	(MAX_LEVEL+MAX_MASTER_LEVEL),	MAX_LEVEL,	(MAX_LEVEL+MAX_MASTER_LEVEL),
#endif
	};


/*===================================================
//	<< ī���� ĳ���� ���� ��� ������ >>
// : ī���� ĳ���� �ʿ��� ���� ��� �����͸� ����
===================================================*/

enum {		// ������� ������¸� ����
	CC_USER_ALIVE				= 0,							// �׳� ������ (����� ������ �⺻��)
	CC_USER_DEAD				= 1,							// �����
	CC_USER_WINNER				= 2,							// �����
};

enum {		// ����ڿ��� 30�� ī��Ʈ �޽����� ����
	CC_MSG_BEFORE_ENTER			= 10,							// [0x92] ���� 30������ �޽��� Ÿ��
	CC_MSG_BEFORE_PLAY			= 11,							// [0x92] ������ 30������ �޽��� Ÿ��
	CC_MSG_BEFORE_END			= 12,							// [0x92] ������� 30������ �޽��� Ÿ��
	CC_MSG_BEFORE_QUIT			= 13,							// [0x92] ���� 30������ �޽��� Ÿ��
};

enum {		// ����ڿ��� ���¸� �˷��ִ� ������
	CC_STATE_START				= 5,							// ī���� ĳ�� - ��� ����
	CC_STATE_PLAY				= 6,							// ī���� ĳ�� - ��� ���� ��
	CC_STATE_END				= 7,							// ī���� ĳ�� - ��� ����
	CC_STATE_TRAP_1				= 8,							// ī���� ĳ�� - Ʈ�� 1�ܰ�
	CC_STATE_TRAP_2				= 9,							// ī���� ĳ�� - Ʈ�� 1�ܰ�
	CC_STATE_TRAP_3				= 10,							// ī���� ĳ�� - Ʈ�� 1�ܰ�
};

static INT g_iChaosCastle_BlowOutDistance [MAX_BLOWOUT_DISTANCE][2]={	
			// �Ÿ��� ���� ����ڸ� ���Ĺ��� �� �ִ� �ּ�, �ִ� ũ��
	3,		4,													// �Ÿ� 0�� �ּ�, �ִ�
	3,		4,													// �Ÿ� 1�� �ּ�, �ִ�
	2,		3,													// �Ÿ� 2�� �ּ�, �ִ�
	0,		1,													// �Ÿ� 3�� �ּ�, �ִ�
};

static INT g_iChaosCastle_BlowOutDamage [MAX_BLOWOUT_DISTANCE]={
			// �Ÿ��� ���� ����ڸ� ���Ĺ��� �� �ִ� �ּ�, �ִ� ũ��
	15,															// �Ÿ� 0�� �ּ�, �ִ�
	15,															// �Ÿ� 1�� �ּ�, �ִ�
	10,															// �Ÿ� 2�� �ּ�, �ִ�
	5,															// �Ÿ� 3�� �ּ�, �ִ�
};

#ifdef FOR_CHINA		// �߱��� ī���� ĳ�� ����� ����
static INT g_iChaosCastle_EnterCost [MAX_CHAOSCASTLE_COUNT]={			// ī���� ĳ�� �����
	25000,														// 1 ĳ�� �����
//	50000,														// 1 ĳ�� �����
	80000,														// 2 ĳ�� �����
	200000,														// 3 ĳ�� �����
	350000,														// 4 ĳ�� �����
	600000,														// 5 ĳ�� �����
	900000,														// 6 ĳ�� �����
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	1000000,													// 7 ĳ�� �����
#endif
};
#else
static INT g_iChaosCastle_EnterCost [MAX_CHAOSCASTLE_COUNT]={			// ī���� ĳ�� �����
	25000,														// 1 ĳ�� �����
//	50000,														// 1 ĳ�� �����
	80000,														// 2 ĳ�� �����
	150000,														// 3 ĳ�� �����
	250000,														// 4 ĳ�� �����
	400000,														// 5 ĳ�� �����
	650000,														// 6 ĳ�� �����
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	1000000,													// 7 ĳ�� �����
#endif
};
#endif

static INT g_iChaosCastle_GuardSetValue [MAX_CHAOSCASTLE_COUNT]={		// ī���� ĳ�� ������ ��Ʈ ����
	2500,														// +1 ������ ��Ʈ ����
	2500,														// +2 ������ ��Ʈ ����
	2500,														// +3 ������ ��Ʈ ����
	2500,														// +4 ������ ��Ʈ ����
	2500,														// +5 ������ ��Ʈ ����
	2500,														// +6 ������ ��Ʈ ����
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	2500,														// 7 ������ ��Ʈ ����
#endif
};

const BYTE	g_btCastleSafetyZoneMapXY [MAX_CHAOSCASTLE_COUNT][4] = {	// ĳ���� ����� ��ǥ => (��X, ��Y, ��X, ��Y)
	23,	75,	44,	108,											// 1�� ĳ�� => (��X, ��Y, ��X, ��Y)
	23,	75,	44,	108,											// 2�� ĳ�� => (��X, ��Y, ��X, ��Y)
	23,	75,	44,	108,											// 3�� ĳ�� => (��X, ��Y, ��X, ��Y)
	23,	75,	44,	108,											// 4�� ĳ�� => (��X, ��Y, ��X, ��Y)
	23,	75,	44,	108,											// 5�� ĳ�� => (��X, ��Y, ��X, ��Y)
	23,	75,	44,	108,											// 6�� ĳ�� => (��X, ��Y, ��X, ��Y)
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	23,	75,	44,	108,											// 7�� ĳ�� => (��X, ��Y, ��X, ��Y)
#endif
};

const BYTE g_btCastleCenterHollowZoneMapXY  [MAX_CHAOSCASTLE_COUNT][2][4] = {	// ĳ���� ����� ��� �ո� �� ���� ��ǥ => (��X, ��Y, ��X, ��Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 1�� ĳ�� => (��X, ��Y, ��X, ��Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 2�� ĳ�� => (��X, ��Y, ��X, ��Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 3�� ĳ�� => (��X, ��Y, ��X, ��Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 4�� ĳ�� => (��X, ��Y, ��X, ��Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 5�� ĳ�� => (��X, ��Y, ��X, ��Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 6�� ĳ�� => (��X, ��Y, ��X, ��Y) x 2
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	32,	84,	35,	87, 32,	96,	35,	99,								// 7�� ĳ�� => (��X, ��Y, ��X, ��Y) x 2
#endif
};

const INT	g_iChaosCastle_ExpTable [MAX_CHAOSCASTLE_COUNT][2] = {		// ĳ���� ���� ���濡 ���� ����ġ �� ��� ���̺�
	500,	1000,												// 1�� ĳ�� =>  (��� KILL, ���� KILL)
	1000,	1500,												// 2�� ĳ�� =>  (��� KILL, ���� KILL)
	1500,	2000,												// 3�� ĳ�� =>  (��� KILL, ���� KILL)
	2000,	2500,												// 4�� ĳ�� =>  (��� KILL, ���� KILL)
	2500,	3000,												// 5�� ĳ�� =>  (��� KILL, ���� KILL)
	3000,	3500,												// 6�� ĳ�� =>  (��� KILL, ���� KILL)
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ĳ���� ���� ���濡 ���� ����ġ ��
	4000,	5000,												// 7�� ĳ�� =>  (��� KILL, ���� KILL)
#endif
};

enum {		// ���� ����ġ ���̺� ���Ǵ� ����ġ ���� �ε���
	CC_EXP_USER_KILL			= 0,							// �ε��� - ����ڸ� �׿��� �� ��� ����ġ
	CC_EXP_MONSTER_KILL			= 1,							// �ε��� - ���͸� �׿��� �� ��� ����ġ
};

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
enum {		// ī���� ĳ�� ���� �̺�Ʈ�� ����
	CC_EVENT_GENERAL				= 0,							// �Ϲ� ����
	CC_EVENT_PCBANG_FREE_ENTER		= 1,							// PC�� �̺�Ʈ�� ī���� ĳ�� ��������
};
#endif


//--------------------------------------------------------------
// ���Ϳ��� ����� ���� �����Ǵ� ������ ����

//#define MAX_CHAOSCASTLE_ITEM_DROP_KIND		4					// ī���� ĳ�� ���Ͱ� �������� ����ϴ� �ִ� ����
//#define MAX_CHAOSCASTLE_ITEM_DROP_KIND		3					// ī���� ĳ�� ���Ͱ� �������� ����ϴ� �ִ� ����
#define MAX_CHAOSCASTLE_ITEM_DROP_KIND		2					// ī���� ĳ�� ���Ͱ� �������� ����ϴ� �ִ� ����
#define MAX_CHAOSCASTLE_ITEM_DROP_LOOP		200					// ī���� ĳ�� ���Ͱ� �������� ��� ���� �� ��õ� Ƚ��

typedef struct _MONSTER_ITEM_DROP {								// ī���� ĳ�� ���Ͱ� ����߸��� ������
	INT		m_iIndex;											// ������ �ε���
	INT		m_iItemKind;										// �������� ����
	_MONSTER_ITEM_DROP() {
		m_iIndex				= -1;
		m_iItemKind				= -1;
	}
} MONSTER_ITEM_DROP, * LPMONSTER_ITEM_DROP;


#ifndef MODIFY_CHAOSCASTLE_REWARD_20090608

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
#ifdef UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828	// ī�� ���� ����
static INT g_iChaosCastle_MonsterItems [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = 
{
	// �� ĳ���� ���Ϳ��� �ο��Ǵ� �������� ������ ����
	MAKE_ITEMNUM(14, 13),		1,								// 1�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 1�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		1,								// 2�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 2�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		1,								// 3�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 3�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		2,								// 4�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 4�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		2,								// 5�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 5�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		2,								// 6�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 6�� ĳ�� - ����

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7īĳ���� ����ϴ� ���� ��
	MAKE_ITEMNUM(14, 13),		2,								// 7�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		4,								// 7�� ĳ�� - ����
#endif
};
#else // UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828
static INT g_iChaosCastle_MonsterItems [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = 
{
	// �� ĳ���� ���Ϳ��� �ο��Ǵ� �������� ������ ����
	MAKE_ITEMNUM(14, 13),		2,								// 1�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 1�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		2,								// 2�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 2�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		2,								// 3�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 3�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		2,								// 4�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 4�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		2,								// 5�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 5�� ĳ�� - ����

	MAKE_ITEMNUM(14, 13),		2,								// 6�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 6�� ĳ�� - ����

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7īĳ���� ����ϴ� ���� ��
	MAKE_ITEMNUM(14, 13),		3,								// 7�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 7�� ĳ�� - ����
#endif
};
#endif	// UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828

#else	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307

static INT g_iChaosCastle_MonsterItems [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = {
	// ī���� ĳ�� ���� ��� ������ �ؿܿ� ���� ���� �ϰ�(grooving)
	// �� ĳ���� ���Ϳ��� �ο��Ǵ� �������� ������ ����
	MAKE_ITEMNUM(14, 13),		1,								// 1�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 1�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		1,								// 2�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 2�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		1,								// 3�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 3�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		2,								// 4�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 4�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		2,								// 5�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 5�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		2,								// 6�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 6�� ĳ�� - ����
	
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7īĳ���� ����ϴ� ���� ��
	MAKE_ITEMNUM(14, 13),		2,								// 7�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		4,								// 7�� ĳ�� - ����
#endif
};
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
#else
static INT g_iChaosCastle_MonsterItems [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = 
{
	// �� ĳ���� ���Ϳ��� �ο��Ǵ� �������� ������ ����
	MAKE_ITEMNUM(14, 13),		2,								// 1�� ĳ�� - �༮
		MAKE_ITEMNUM(14, 14),		2,								// 1�� ĳ�� - ����
		
		MAKE_ITEMNUM(14, 13),		2,								// 2�� ĳ�� - �༮
		MAKE_ITEMNUM(14, 14),		2,								// 2�� ĳ�� - ����
		
		MAKE_ITEMNUM(14, 13),		2,								// 3�� ĳ�� - �༮
		MAKE_ITEMNUM(14, 14),		2,								// 3�� ĳ�� - ����
		
		MAKE_ITEMNUM(14, 13),		2,								// 4�� ĳ�� - �༮
		MAKE_ITEMNUM(14, 14),		2,								// 4�� ĳ�� - ����
		
		MAKE_ITEMNUM(14, 13),		2,								// 5�� ĳ�� - �༮
		MAKE_ITEMNUM(14, 14),		3,								// 5�� ĳ�� - ����
		
		MAKE_ITEMNUM(14, 13),		2,								// 6�� ĳ�� - �༮
		MAKE_ITEMNUM(14, 14),		3,								// 6�� ĳ�� - ����
		
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7īĳ���� ����ϴ� ���� ��
		MAKE_ITEMNUM(14, 13),		3,								// 7�� ĳ�� - �༮
		MAKE_ITEMNUM(14, 14),		3,								// 7�� ĳ�� - ����
#endif
};
#endif // MODIFY_CHAOSCASTLE_REWARD_20090608



#ifdef ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113
// �ż� ī����ĳ�� ���� ��� �� ��ӵǴ� ���� ���� �߰�
static INT g_iChaosCastle_MonsterItemsNewServer [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = {
	// ī���� ĳ�� ���� ��� ������ �ؿܿ� ���� ���� �ϰ�(grooving)
	// �� ĳ���� ���Ϳ��� �ο��Ǵ� �������� ������ ����
	0,							0,								// 1�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 1�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		1,								// 2�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		1,								// 2�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		1,								// 3�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 3�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		1,								// 4�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 4�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		2,								// 5�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		1,								// 5�� ĳ�� - ����
	
	MAKE_ITEMNUM(14, 13),		2,								// 6�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		2,								// 6�� ĳ�� - ����
	
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7īĳ���� ����ϴ� ���� ��
	MAKE_ITEMNUM(14, 13),		2,								// 7�� ĳ�� - �༮
	MAKE_ITEMNUM(14, 14),		3,								// 7�� ĳ�� - ����
#endif
};
#endif // ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113

//--------------------------------------------------------------
// ī���� ĳ�� Ʈ�� ����

#define CC_MAX_DAMAGE_STEP		4								// ī���� ĳ�� Ʈ�� ������ �ܰ�

#define CC_TRAP_DAMAGE			10								// ī���� ĳ�� Ʈ�� ������

enum {
	CC_TRAP_0					=0,								// ī���� ĳ�� Ʈ�� - 0�ܰ� (�ۿ�ʵ�)
	CC_TRAP_1					=1,								// ī���� ĳ�� Ʈ�� - 1�ܰ� (�ܰ� 1ĭ)
	CC_TRAP_2					=2,								// ī���� ĳ�� Ʈ�� - 2�ܰ� (�ܰ� 2ĭ)
	CC_TRAP_3					=3,								// ī���� ĳ�� Ʈ�� - 3�ܰ� (�ܰ� 3ĭ)
};

static INT	g_iChaosCastle_GroundAxis[4] = {					// ī���� ĳ�� ������� �ܰ���ǥ - ��X, ��Y, ��X, ��Y
	23,		75,		44,		108,
};

static INT	g_iChaosCastle_DamageAxis[CC_MAX_DAMAGE_STEP][4] = {	// ī���� ĳ���� Ʈ���� ������� �ʴ� ������ ���� (�ܰ� ��)
	23,		75,		44,		108,								// ī���� ĳ�� - Ʈ�� 0 �ܰ�
	25,		77,		42,		106,								// ī���� ĳ�� - Ʈ�� 1 �ܰ�
	27,		79,		40,		104,								// ī���� ĳ�� - Ʈ�� 2 �ܰ�
	29,		81,		38,		102,								// ī���� ĳ�� - Ʈ�� 3 �ܰ�
};

static INT	g_iChaosCastle_MapHollowZone[CC_MAX_DAMAGE_STEP][4][4] = {		// ī���� ĳ���� Ʈ���� ���� ���ո� �Ӽ����� ���ϴ� ���� (�ܰ� ��)
	// Ʈ�� 0 �ܰ� (����)
	23,		75,		44,		76,
	43,		77,		44,		108,
	23,		107,	42,		108,
	23,		77,		24,		106,

	// Ʈ�� 1 �ܰ�
	23,		75,		44,		76,
	43,		77,		44,		108,
	23,		107,	42,		108,
	23,		77,		24,		106,

	// Ʈ�� 2 �ܰ�
	25,		77,		42,		78,
	41,		79,		42,		106,
	25,		105,	40,		106,
	25,		79,		26,		104,

	// Ʈ�� 3 �ܰ�
	27,		79,		40,		80,
	39,		81,		40,		104,
	27,		103,	38,		104,
	27,		81,		28,		102,
};


static RECT g_rtPOINT_FRAME[4]	= {0,};							// �ܰ������� �� ��и��� ��ǥ�� ����
static RECT g_rtPOINT_TRAP[4]	= {0,};							// Ʈ�������� �� ��и��� ��ǥ�� ����



/*===================================================
//	<< ī���� ĳ�� ���� ����ü >>
// : ī���� ĳ���� �� 6���� �ұ� ���� �� ���� �����͸� �����Ѵ�.
===================================================*/
typedef struct _CHAOSCASTLE_USER {
	INT		m_iIndex;											// ������ ������� ����ü
	INT		m_iEXP;												// ����ڰ� ���� ����ġ
	INT		m_iScore;											// ����ڰ� ���� ����
	INT		m_iUserState;										// ������� ���� (���߿� ������ �ϱ�����)
	bool	m_bSendQuitMsg;										// ����ڰ� ī���� ĳ�� ������ �������� ����޽����� �̹� �����־��°� �ƴѰ�? (�ʱⰪ : false)
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	BYTE	m_btEvent;											// ����ڰ� ���� �̺�Ʈ
#endif

	_CHAOSCASTLE_USER() {
		Clear();
	}

	VOID Clear() {
		m_iIndex		= -1;
		m_iEXP			= 0;
		m_iScore		= 0;
		m_iUserState	= CC_USER_ALIVE;
		m_bSendQuitMsg	= false;
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
		m_btEvent		= 0;
#endif		
	}
} CHAOSCASTLE_USER, * LPCHAOSCASTLE_USER;


typedef struct _CHAOSCASTLE_DATA {
	INT		m_iCC_STATE;										// ī���� ĳ���� ���� ���¸� ��Ÿ��

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	BYTE	m_btCC_EVENT;										// ���� ī���� ĳ���� �̺�Ʈ
#endif

	INT		m_iMapNumber;										// �ڽ��� �ʹ�ȣ
	INT		m_iChaosCastleIndex;								// �ڽ��� ĳ����ȣ
	INT		m_iMISSION_SUCCESS;									// �� ĳ���� �̼��� �����ߴ�. (�ƴϸ� -1 ������ 0 ~ 15�� ����� �ε���)
	
	INT		m_iCC_REMAIN_MSEC;									// ���� ���·� ��������� �ð� (1/1000 ��)
	INT		m_iCC_TICK_COUNT;									// ī��Ʈ �۾����� �ϱ����� �ð� (1/1000��)
	INT		m_iCC_NOTIFY_COUNT;									// ������ ������ ���� ī����
	
	bool	m_bCC_CAN_ENTER;									// ���� ���� �����Ѱ� ?
	bool	m_bCC_CAN_PARTY;									// ���� ��Ƽ �����Ѱ� ?
	bool	m_bCC_PLAY_START;									// ��Ⱑ ���۵Ǿ��°� ?

	bool	m_bCC_MSG_BEFORE_ENTER;								// 30�ʾ˸� - �������� ���´°�?
	bool	m_bCC_MSG_BEFORE_PLAY;								// 30�ʾ˸� - ���������� ���´°�?
	bool	m_bCC_MSG_BEFORE_END;								// 30�ʾ˸� - ����������� ���´°�?
	bool	m_bCC_MSG_BEFORE_QUIT;								// 30�ʾ˸� - �������� ���´°�?

	INT		m_iCC_ENTERED_USER;									// �ش� ī����ĳ���� ������ ������� ��
	INT		m_iCC_FIRST_USER_COUNT;								// ��� ���� ���� ����� ��
	INT		m_iCC_FIRST_MONSTER_COUNT;							// ��� ���� ���� ���� ��
	LONG	m_lCC_CURRENT_MONSTER_COUNT;						// ��� ���� ���� ���� ��

	INT		m_iCC_TRAP_STATUS;									// ����� �܁𿡼� ������ ������ Ʈ���� ���¸� �ܰ躰�� ��Ÿ��
	INT		m_iCC_TRAP_OLD_STATUS;								// ����� �܁𿡼� ������ ������ Ʈ���� ���¸� �ܰ躰�� ��Ÿ�� (���� ��)

	CHAOSCASTLE_USER	m_UserData [MAX_CHAOSCASTLE_USER_COUNT];			// ����� ����ü
	LONG				m_lCC_MONSTER_COUNT[MAX_CHAOSCASTLE_MONSTER_COUNT];	// ���� ����ü	
	LONG				m_lFallUser[MAX_CHAOSCASTLE_USER_COUNT];			// �� ī���� ĳ������ �������� ����ڸ� ����

	map <INT, MONSTER_ITEM_DROP>	m_mapMonsterItemList;		// �� ī���� ĳ������ ��� �� ���Ͱ� �ݵ�� ����߷��� �ϴ� ��� ����

	#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	FLOAT	m_fRewardExpRate;
	#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

	VOID Clear() {
		m_iMISSION_SUCCESS			= -1;
		m_iCC_NOTIFY_COUNT			= -1;
		m_bCC_MSG_BEFORE_ENTER		= false;
		m_bCC_MSG_BEFORE_PLAY		= false;
		m_bCC_MSG_BEFORE_END		= false;
		m_bCC_MSG_BEFORE_QUIT		= false;
		m_bCC_CAN_ENTER				= false;
		m_bCC_CAN_PARTY				= false;
		m_bCC_PLAY_START			= false;
		m_iCC_ENTERED_USER			= 0;
		m_iCC_FIRST_USER_COUNT		= 0;
		m_iCC_FIRST_MONSTER_COUNT	= 0;
		m_lCC_CURRENT_MONSTER_COUNT	= 0;
		m_iCC_TRAP_STATUS			= CC_TRAP_0;
		m_iCC_TRAP_OLD_STATUS		= CC_TRAP_0;
		for (INT iMON = 0 ; iMON < MAX_CHAOSCASTLE_MONSTER_COUNT ; iMON++)
		m_lCC_MONSTER_COUNT[iMON]	= -1;
		for (INT iFALL = 0 ; iFALL < MAX_CHAOSCASTLE_USER_COUNT ; iFALL++)
		m_lFallUser[iFALL]	= -1;
		m_mapMonsterItemList.clear();
	}
} CHAOSCASTLE_DATA, * LPCHAOSCASTLE_DATA;

typedef struct 
{
	unsigned short item_type;
	char item_level;
	bool is_skill;
	bool is_luck;
	char option;
	char exc_option;
	unsigned short max_drop_rate;
} CHAOSCASTLE_REWARD, *LPCHAOSCASTLE_REWARD;

typedef std::list<CHAOSCASTLE_REWARD> tdCCRewardList;

/*===================================================
//	<< ī���� ĳ�� ���� Ŭ���� >>
// : ī���� ĳ���� ��ü���� ��� �����층, ������ �ô´�.
===================================================*/

class CChaosCastle  
{
private:
	INT		m_iMinUserCount;
	std::string m_sMinPlayerMessage;
	BOOL	m_bCC_EVENT_ENABLE;											// �̺�Ʈ�� Ȱ��ȭ �Ǿ��°�? (���డ���Ѱ�?) ����

	INT		m_iCC_TIME_MIN_OPEN;										// ����ð� (��) -> ��� ���� ������ ���ΰ�?
	INT		m_iCC_TIME_MIN_PLAY;										// ���ӽð�	(��)
	INT		m_iCC_TIME_MIN_REST;										// ���� �� �޽Ľð�	(��)

	INT		m_iCC_NORMAL_ITEM_DROP;										// �Ϲ� �ƾ��� �����
	INT		m_iCC_EXCEL_ITEM_DROP;										// ������Ʈ ������ �����
	INT		m_iCC_MONSTER_REGEN;										// ���� ���� �ð�

	vector<CHAOSCASTLE_START_TIME> m_vtChaosCastleOpenTime;				// ī���� ĳ���� ������ �ð��� ������ ����
	tdCCRewardList m_LST_CHAOSCASTLE_REWARD[MAX_CHAOSCASTLE_COUNT];
	INT		m_iDropCount[MAX_CHAOSCASTLE_COUNT];

public:			// ���� ���������
	

public:			// ���� �޼����
	CChaosCastle();
	virtual ~CChaosCastle();

	/* ī���� ĳ�� �����층 ���� �Լ��� */
	VOID	Run();
	VOID	Init (bool bEVENT_ENABLE = TRUE);
	VOID	Load (char* filename);
	VOID	SetState (INT iChaosCastleIndex, INT iCC_STATE);			// Ư�� �ұ��� �����췯�� Ư�� ���·� �����Ѵ�.


	/* ī���� ĳ�� ����� ���� �Լ��� */
	INT		EnterUserChaosCastle (										// iIndex�� ����ڸ� iChaosCastleIndex ��° ĳ���� �Թ��Ų��. (-1 ���� �� �����)	
				INT iChaosCastleIndex, 
				INT iUserIndex
				);
	INT		LeaveUserChaosCastle (										// iChaosCastleIndex ��° ĳ���� iChaosCastleSubIndex ��° ������� ���� ����. (iUserIndex �� ��������)
				INT iChaosCastleIndex, 
				INT iUserIndex
				);
	VOID	AddFallUser (												// ���߿� ���ư��� ������ ���� ����ڵ��� �߰��Ѵ�. (�Ź� ����� ����Ʈ�� �˻��ϱ� ����Ƿ�)
				INT iChaosCastleIndex,
				INT iUserIndex
				);
	VOID	ProcessFallUser (											// ������ ����� ������� ��Ͽ� �ִ� ��� ���ó���� �Ѵ�.
				INT iChaosCastleIndex
				);
	INT		GetCurrentAliveUser (										// ������ ����� ������� ��Ͽ� �ִ� ��� ���ó���� �Ѵ�.
				INT iChaosCastleIndex
				);
	void	SetUserState (												// ���� ������ ���� ����� ���¸� �����Ѵ�. 
				INT iUserIndex,
				INT iState
				);
	INT		GetCurrentWinUser (											// ���� ��⿡�� �̱� ����� ã�Ƴ���. (-1) �̸� ����
				INT iChaosCastleIndex
				);
	INT		CalcSendRewardEXP	(										// ��� ���� �� ����ġ�� ����ϰ� �ش� ����ڿ��� �����ش�.
				INT iUserIndex, 
				INT iEXP,
				INT iKILLCOUNT_USER,
				INT iKILLCOUNT_MONSTER
				);		
#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	INT		LevelUp (													// ī���� ĳ�� ���� ������ �޼���
				INT iUserIndex, 
				INT iAddExp,
				INT iEventType
				);
#else
	INT		LevelUp (													// ī���� ĳ�� ���� ������ �޼���
				INT iUserIndex, 
				INT iAddExp
				);
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	INT		GetUserLevelToEnter (										// ����ڰ� � ī���� ĳ���� �� �� �ִ��� ������ ��ȯ�Ѵ�.
				INT iUserIndex 
				);
	BOOL	CheckUserEnterMoney (										// ������� ����� ����Ḧ ������ �ִ��� Ȯ���Ѵ�.
				INT iUserIndex, 
				INT iEnterLevel
				);
	BOOL	PayUserEnterMoney (											// ������� ����Ḧ ���� ��Ų�� (FALSE�� �������� ���� ���ڶ�)
				INT iUserIndex, 
				INT iEnterLevel
				);
	VOID	GiveUserDamage (											// ����ڿ��� �������� �������� �ش�. (������ ��ġ�� Ŭ���̾�Ʈ�� �����ָ�, ������ ��ü ó���Ѵ�)
				INT iUserIndex, 
				INT iDamage
				);


	/* ī���� ĳ�� ���� ���� �Լ��� */
	VOID	ClearMonster	(											// �ش� ĳ���� ��� ���� ����
				INT iChaosCastleIndex
				);
	INT		SetMonster		(INT iChaosCastleIndex);					// ī���� ĳ���� ������ ������ Ư�� �Ӽ��� SET�ϰ� ��ġ�Ѵ�. (�ʹ�ȣ�� 18~24�� ���͵� �ϰ���ġ)
	VOID	AddMonsterList	(											// Ư�� ī���� ĳ���� ���� ������ �߰��Ѵ�.
				INT iChaosCastleIndex,
				INT iMonsterIndex
				);
	VOID	DelMonsterList	(											// Ư�� ī���� ĳ���� ���� ������ �����Ѵ�.
				INT iChaosCastleIndex,
				INT iMonsterIndex
				);
	INT		GetMonsterListCount (										// ���� ����Ʈ�� ���� ������ ��´�.
				INT iChaosCastleIndex
				);
	VOID	SetItemsToMonster (											// ���Ϳ��� ������ �������� �����Ѵ�. (m_vtMonsterCount �� �̸� ���õǾ� �־�� ��)
				INT iChaosCastleIndex
				);
	VOID	SearchNDropMonsterItem (									// �� ĳ���� ���͵��� �׾��� �� ������ ������ ���� �ε����� �������� ������ �ش� �������� ����Ѵ�.
				INT iChaosCastleIndex,
				INT iMonsterIndex,
				INT iMaxHitUserIndex
				);


	/* ī���� ĳ�� �������� �Լ��� */
	VOID	SendAllUserAnyMsg(											// ������ �ִ� ��� ����ڵ鿡�� �޽����� ������.
				LPBYTE lpMsg, 
				INT iSize
				);
	
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	VOID	SendPCBangUserAnyMsg(										// �������ִ� ��� PC�� ����ڵ鿡�� �޽����� ������.
				LPBYTE lpMsg, 
				INT iSize
				);		
#endif
	
	VOID	SendChaosCastleAnyMsg(										// Ư�� ī���� ĳ���� ����ڵ鿡�� �޽����� ������.
				LPBYTE lpMsg, 
				INT iSize, 
				INT iChaosCastleIndex
				);
	VOID	SendNoticeMessage (											// Ư�� ī���� ĳ���� �ο��鿡�� ������ ������. (0x0D)
				INT iChaosCastleIndex,
				char * lpszMSG
				);
	VOID	SendNoticeState (											// Ư�� ī���� ĳ���� �ο��鿡�� ���� ������� ��Ȳ(�ð�, ���ͼ�)�� ������.
				INT iChaosCastleIndex,
				INT iPlayState
				);
	VOID	SendWinMessage (											// Ư�� ī���� ĳ���� ����ڿ��� ��� �޽����� �����ش�.
				INT iChaosCastleIndex,
				INT iWinnerIndex
				);
	VOID	SendFailMessage (											// Ư�� ī���� ĳ���� ������(�����) ���� ���� �޽����� �����ش�.
				INT iChaosCastleIndex,
				INT iLoserIndex
				);
	VOID	SendAllLoserFailMessage (									// Ư�� ī���� ĳ���� ����ڸ� ������ ������ �ο��鿡�� ���� �޽����� �����ش�.
				INT iChaosCastleIndex,
				INT iWinnerIndex
				);


	/* ī���� ĳ�� ���� ���� ���¹�ȯ �Լ��� */
	INT		GetCurrentState		(INT iChaosCastleIndex);
	INT		GetCurEnteredUser	(INT iChaosCastleIndex);				// ���� ������ �ִ� ������� ���� ��ȯ
	INT		GetCurPlayUser		(INT iChaosCastleIndex);				// ���� ������ �����ؼ� �÷��� �غ� �ϰ��ִ� ������� ���� ��ȯ
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307	// Add Function
	INT		GetCurEventUser	(											// ���� ������ ����� �� �̺�Ʈ�� ����� ���� ��ȯ
				INT iChaosCastleIndex,
				BYTE btEvent 
				);		
	BYTE	GetCurrentEvent		(INT iChaosCastleIndex);
#endif

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	INT		GetRemainTime	(											// ������ ���� ���� �ð��� Ȯ���Ѵ�. (�д���)
				INT iUserIndex, 
				INT iChaosCastleIndex
				);
#else
	INT		GetRemainTime		(INT iChaosCastleIndex);				// ������ ���� ���� �ð��� Ȯ���Ѵ�. (�д���)
#endif	

	INT		GetCurrentRemainSec	(INT iChaosCastleIndex);				// ���� �������±��� �������� �ʸ� ��ȯ
	INT		GetExcelItemDropRate() {									// �Ϲ� ������ �����
				return m_iCC_EXCEL_ITEM_DROP; }
	INT		GetNormalItemDropRate() {									// �Ϲ� ������ �����
				return m_iCC_NORMAL_ITEM_DROP; }


	/* ī���� ĳ�� ���� ���� ����üũ �Լ��� */
	INT		CheckEnterLevel		(INT iIndex, INT iLevel);				// �ش� ����ڰ� ī���� ĳ���� ������ �� �ִ��� Ȯ�� (0:����, -1:����, +1:����, 2:�ٸ�����)
	INT		CheckEnterItem		(INT iIndex);							// �ش� ����ڰ� ī���� ĳ���� ������ �� �ִ� ������ (�������� ���ʼ�Ʈ) �� �ִ��� Ȯ�� (0 �̸� �Ұ� / 1 ~ n ���� �������� ���ʼ�Ʈ ����)
	INT		CheckPlayEnded		(INT iChaosCastleIndex);				// ���� ī���� ĳ�� ��Ⱑ �����ߴ��� üũ�Ѵ�. (0:������, 1:����, 2:����)
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	bool	CheckCanEnter		(INT iUserIndex, INT iChaosCastleIndex);	// ���� ī���� ĳ�� ������ �������� Ȯ���Ѵ�.
#else
	bool	CheckCanEnter		(INT iChaosCastleIndex);				// ���� ī���� ĳ�� ������ �������� Ȯ���Ѵ�.
#endif	
	bool	CheckPlayStart		(INT iChaosCastleIndex);				// ���� ī���� ĳ�� ��Ⱑ ���۵Ǿ����� Ȯ���Ѵ�.
	bool	CheckCanStartPlay	(INT iChaosCastleIndex);				// ���� ī���� ĳ�� ��⸦ ������ �� �ִ� �������� Ȯ���Ѵ�.


	/* ī���� ĳ�� ���� �������� �Լ��� */
	BOOL	ObjSetPosition		(INT iIndex, INT iX, INT iY);			// Ư�� ������Ʈ (�����, ����)�� ��ġ�� Ư�� ��ġ�� ���� ����
	VOID	SearchNBlowObjs		(										// Ư�� ����Ʈ �ֺ� 5x5 ũ�⸦ �˻��Ͽ� �ֺ��� ������Ʈ (�����, ����) ���� ��������
				INT iMapNumber,
				INT iX, 
				INT iY
				);
	BOOL	BlowObjsFromPoint	(										// Ư�� ������Ʈ (�����, ����)���� Ư�� ��ġ�� �߽����� �ܰ����� ��������
				INT iIndex, 
				INT iMapNumber,
				INT & iX, 
				INT & iY
				);
	BOOL	BlowObjsToPoint	(											// Ư�� ������Ʈ (�����, ����)���� Ư�� ��ġ�� ��������
				INT iIndex, 
				INT iMapNumber,
				INT iX, 
				INT iY
				);
	INT		CalDistance			(INT iX1, INT iY1, INT iX2, INT iY2);	// �� ��ǥ���� �Ÿ��� ���
	VOID	SafetyCastleZone	(INT iChaosCastleIndex);				// Ư�� ĳ���� ������ �������� �Ӽ����� ä���.
	VOID	UnSafetyCastleZone	(INT iChaosCastleIndex);				// Ư�� ĳ���� ������ �������� �Ӽ��� Ǭ��.
	VOID	SendCastleZoneSafetyInfo (									// Ư�� ĳ���� ��� ����ڿ��� Ư�� ĳ�� �Ա��� ���� �������� ������ ������.
				INT iChaosCastleIndex,
				bool bDoSet
				);
	VOID	CheckUserInDieTile	(										// ����ڰ� ���ո� �Ӽ����� ���ִ��� üũ�Ͽ� ������ ���� ó��
				INT iChaosCastleIndex
				);
	VOID	CheckMonsterInDieTile	(									// ���Ͱ� ���ո� �Ӽ����� ���ִ��� üũ�Ͽ� ������ ������ ó��
				INT iChaosCastleIndex
				);
	VOID	PlayFailedRollBack	(INT iChaosCastleIndex);				// ����� �� �̴� ������ �������� ���� �� ���� ������� ������ �����·� ������.
	INT		RewardZettoMoney	(INT iChaosCastleIndex);				// Ư�� ĳ���� ���� ������ �Ѵ� (��ü ��������� 20%�� ������ �ݾ�)
	INT		ReCalcUserCount		(INT iChaosCastleIndex);				// ���� ������ ������� ���� �ٽ� ��� (�����ؼ� ĳ���ȿ� �ִ� ��� ����)
	VOID	RewardUserEXP		(										// Ư�� ĳ���� ������ �ǽ��Ѵ�.
				INT iChaosCastleIndex,
				INT iChaosCastleSubIndex,
				BOOL bWinner
				);
	VOID	ProcessTrapStatus	(INT iChaosCastleIndex);				// Ư�� ĳ���� Ʈ�� ���¸� ó���Ѵ�.
	VOID	CalUsersInTrap		(INT iChaosCastleIndex);				// Ư�� ĳ���� ����ڵ��� Ʈ�� ���� �ȿ� �ִ��� �ľ��Ͽ� ������ ����ڿ��� �������� �������� �ش�.
	BOOL	CheckWearingMOPH	(INT iUserIndex);						// �ش� ����ڰ� ���Ź����� �����ϰ� �ִ��� Ȯ���Ѵ�.
	VOID	GiveWinnerItem		(										// ī���� ĳ�� ����ڿ��� �������� �ش�.
				INT iChaosCastleIndex,
				INT iWinnerIndex
				);
	VOID	PullObjInnerPlace	(										// �� Ʈ�� �ܰ迡 ���� Ʈ������ ����ڵ��� Ʈ���� �ƴ� ������ �о��. (��������� ������)
																		// �׸��� �� �܁��� �����۵��� �����ش�.
				INT iChaosCastleIndex,									
				INT iTRAP_STEP
				);
	VOID	SetMapAttrHollow	(										// �� Ʈ�� �ܰ迡 ���� ���� �Ӽ��� �� �ո����� ������ �ش�.
				INT iChaosCastleIndex,									
				INT iTRAP_STEP
				);
	VOID	SetMapAttrFill	(											// �� Ʈ�� �ܰ迡 ���� ���� �Ӽ��� �� �ո� �ƴ����� ������ �ش�.
				INT iChaosCastleIndex								
				);
#ifdef		CHAOSCASTLE_GUILD_DUEL_BUGFIX_20040611						// ����ڵ��� üũ�Ͽ� ���� ������̳� ���� ���� ����� �ִٸ� ���� ���� ��Ų��.
	VOID	TerminateGuildWarDuel (
				INT iChaosCastleIndex								
				);
#endif
	
	/* ī���� ĳ�� �������� ��ǰ ���� ���� �Լ��� */
	VOID	CheckRegisterOfflineGift (									// ī���� ĳ���� �������� ��ǰ�� ��÷ ��ų �� �ִ��� üũ�Ѵ�.
				INT iUserIndex
				);

	
protected:		// ��ȣ ���������
	CHAOSCASTLE_DATA			m_stChaosCastleData [MAX_CHAOSCASTLE_COUNT];			// �� ī���� ĳ���� ������ ������ �ִ� ����ü�� �迭

	
protected:		// ��ȣ �޼����
	VOID	CheckSync				(INT iChaosCastleIndex);			// Ư�� �ұ��� ��ũ�� �����.
	VOID	ClearChaosCastleData	(INT iChaosCastleIndex);			// Ư�� �ұ� �����͸� �ʱ�ȭ �Ѵ�.

	VOID	ProcState_None		(INT iChaosCastleIndex);				// �ƹ� ���µ� �ƴ�
	VOID	ProcState_Closed	(INT iChaosCastleIndex);				// ���� ������
	VOID	ProcState_Playing	(INT iChaosCastleIndex);				// ī���� ĳ�� ��� �� 
	VOID	ProcState_PlayEnd	(INT iChaosCastleIndex);				// ī���� ĳ�� ��� ���� �� ���

	VOID	SetState_None		(INT iChaosCastleIndex);				// ī���� ĳ�� �̺�Ʈ ���� �� ���
	VOID	SetState_Closed		(INT iChaosCastleIndex);				// ������ ���·� ����
	VOID	SetState_Playing	(INT iChaosCastleIndex);				// ��� �� ���·� ����
	VOID	SetState_PlayEnd	(INT iChaosCastleIndex);				// ��� ���� �� ��� ���·� ����

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ�� �ٸ���ȣ, �ʹ�ȣ ���
public:
	INT		GetMapNumByChaosCastle( INT iChaosCastleIndex );		// ī����ĳ�� ��ȣ�� �� �ε��� ���ϱ�
	INT		GetChaosCastleByMapNum( INT iMapNum );				// �� �ε����� ī����ĳ�� ��ȣ ���ϱ�

#endif
};


extern CChaosCastle			g_ChaosCastle;								// ī���� ĳ�� �̺�Ʈ ��ü
#endif


#endif // !defined(AFX_CHAOSCASTLE_H__535E5C79_81B4_467D_94A0_4220F9A43D7F__INCLUDED_)

