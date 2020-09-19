// BloodCastle.h: interface for the CBloodCastle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOODCASTLE_H__B6D64999_D5F0_453A_BBC9_A8567758D31D__INCLUDED_)
#define AFX_BLOODCASTLE_H__B6D64999_D5F0_453A_BBC9_A8567758D31D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"
#include <time.h>
#include <list>

#ifdef BLOODCASTLE_EVENT_5TH_20050531			// �����ο��� ���� �߰� ����Ʈ
#include "PartyClass.h"
#endif

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ�� �ٸ� Ȯ��
#define CHECK_BLOODCASTLE(C_VALUE)		(C_VALUE==MAP_INDEX_BLOODCASTLE8?TRUE:C_VALUE<11?FALSE:C_VALUE>17?FALSE:TRUE)
#define	MAX_BLOODCASTLE_BRIDGE_COUNT	8			// ���� ĳ�� �ٸ��� �� ���� (8)
#else
#ifdef BLOODCASTLE_EXTEND_20040314
#define CHECK_BLOODCASTLE(C_VALUE)		(C_VALUE<11?FALSE:C_VALUE>17?FALSE:TRUE)
#define	MAX_BLOODCASTLE_BRIDGE_COUNT	7			// ���� ĳ�� �ٸ��� �� ���� (7)
#else
#define CHECK_BLOODCASTLE(C_VALUE)		(C_VALUE<11?FALSE:C_VALUE>16?FALSE:TRUE)
#define	MAX_BLOODCASTLE_BRIDGE_COUNT	6			// ���� ĳ�� �ٸ��� �� ���� (6)
#endif
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010

#ifdef FOR_BLOODCASTLE3
	#define MAX_BLOODCASTLE_USER_COUNT	10			// ���� ĳ�� �� �ٸ��� �� ������
#else
	#define MAX_BLOODCASTLE_USER_COUNT	15			// ���� ĳ�� �� �ٸ��� �� ������
#endif


#define MAX_BLOODCASTLE_BOSS_COUNT		20				// �� �ٸ��� ������ �� �ִ� �ִ� ���������� ��

#define BC_REWARD_CHAOSEGEM_LOOT_TIME	(5*60*1000)		// ȥ���� �ٸ�������� ���õ��� �ʴ� �ð�

#define BC_ANGEL_WEAPON_LOOT_TIME		(10*1000)		// ��õ���� ������� �ٸ�������� ���õ��� �ʴ� �ð�

#define BC_PERUSER_MONSTER_KILL			40				// ����� �Ѹ�� �Ҵ�� ���� ���ż�
#define BC_PERUSER_BOSS_MONSTER_KILL	2				// ����� �Ѹ�� �Ҵ�� ���� ���� ���ż�

#define	BC_MONSTER_KILL_EXP_PERCENT		50				// ���� ĳ������ ���͸� ������� �� ����ġ�� �ݿ����� (ex> 50/100)

#define BC_MAX_MONSTER_SUCCESS_MSG		1

#define BC_MAX_TICK_DOOR_OPEN			(3*1000)		// ����ĳ���� �� �ٸ��� �Ҵ�� ���͸� �� ���� �� ������ ������ �������� ������

#define BC_MAX_START_WAIT_TERM			(1*60)			// ����ĳ���� �����ϱ� ���� ���ð� (��)

#define BC_SAINT_STATUE_1				132				// ������ ����1
#define BC_SAINT_STATUE_2				133				// ������ ����2
#define BC_SAINT_STATUE_3				134				// ������ ����3


enum {
	BLOODCASTLE_STATE_NONE				=0,
	BLOODCASTLE_STATE_CLOSED			=1,
	BLOODCASTLE_STATE_PLAYING			=2,
	BLOODCASTLE_STATE_PLAYEND			=3
};

// �� ����ĳ���� ����Ʈ ��ȣ
#define GATE_BLOODCASTLE_1				66
#define GATE_BLOODCASTLE_2				67
#define GATE_BLOODCASTLE_3				68
#define GATE_BLOODCASTLE_4				69
#define GATE_BLOODCASTLE_5				70
#define GATE_BLOODCASTLE_6				71
#ifdef BLOODCASTLE_EXTEND_20040314
#define GATE_BLOODCASTLE_7				80
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ��8 ����Ʈ ��ȣ
#define GATE_BLOODCASTLE_8				271
#endif

#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
#include <list>

typedef struct __BLOODCASTLE_STARTTIME
{
	int nHour;
	int nMin;
} BLOODCASTLE_STARTTIME;
#endif	//UPDATE_BLOODECASTLE_SCADULE_20080624

/*===================================================
//	<< ���� ĳ�� ���� ���� ����ü >>
// : ���� ĳ���� �� �ٸ��� ���� ���� ���ѿ� ���� ������ ������ �ִ�.
===================================================*/
typedef struct _BLOOODCASTLE_LEVEL {
	INT iLOWER_BOUND;									// ���Ѽ�
	INT iUPPER_BOUND;									// ���Ѽ�
	INT iLOWER_BOUND_MAGUMSA;							// ���Ѽ� - ���˻�
	INT iUPPER_BOUND_MAGUMSA;							// ���Ѽ� - ���˻�
} BLOODCASTLE_LEVEL, * LPBLOODCASTLE_LEVEL;


static BLOODCASTLE_LEVEL g_sttBLOODCASTLE_LEVEL[MAX_BLOODCASTLE_BRIDGE_COUNT] = {
	15,		80,			10,		60,
	81,		130,		61,		110,
	131,	180,		111,	160,
	181,	230,		161,	210,
	231,	280,		211,	260,
#ifdef BLOODCASTLE_EXTEND_20040314
	281,	330,		261,	310,
	331,	MAX_LEVEL,	311,	MAX_LEVEL,
#else
	281,	MAX_LEVEL,	261,	MAX_LEVEL,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ��8 ���尡�� ����
	MAX_LEVEL,	(MAX_LEVEL + MAX_MASTER_LEVEL),	MAX_LEVEL,	(MAX_LEVEL + MAX_MASTER_LEVEL),	
#endif
	};


/*===================================================
//	<< ���� ĳ���� ���� ��� ������ >>
// : ���� ĳ���� �ʿ��� ���� ��� �����͸� ����
===================================================*/

const BYTE	g_btCastleEntranceMapXY [MAX_BLOODCASTLE_BRIDGE_COUNT][4] = {	// �ٸ��� ����ؾ��� �ӱ���ǥ => (��X, ��Y, ��X, ��Y)
	13,	15,	15,	23,										// 1�� �ٸ��� �Ա� => (��X, ��Y, ��X, ��Y)
	13,	15,	15,	23,										// 2�� �ٸ��� �Ա� => (��X, ��Y, ��X, ��Y)
	13,	15,	15,	23,										// 3�� �ٸ��� �Ա� => (��X, ��Y, ��X, ��Y)
	13,	15,	15,	23,										// 4�� �ٸ��� �Ա� => (��X, ��Y, ��X, ��Y)
	13,	15,	15,	23,										// 5�� �ٸ��� �Ա� => (��X, ��Y, ��X, ��Y)
	13,	15,	15,	23,										// 6�� �ٸ��� �Ա� => (��X, ��Y, ��X, ��Y)
#ifdef BLOODCASTLE_EXTEND_20040314
	13,	15,	15,	23,										// 7�� �ٸ��� �Ա� => (��X, ��Y, ��X, ��Y)
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
	13,	15,	15,	23,										// 8�� �ٸ��� �Ա� => (��X, ��Y, ��X, ��Y)
#endif	
	};


const BYTE	g_btCastleBridgeMapXY [MAX_BLOODCASTLE_BRIDGE_COUNT][4] = {	// �ٸ��� �����ǥ => (��X, ��Y, ��X, ��Y)
	13,	70,	15,	75,										// 1�� �ٸ� => (��X, ��Y, ��X, ��Y)
	13,	70,	15,	75,										// 2�� �ٸ� => (��X, ��Y, ��X, ��Y)
	13,	70,	15,	75,										// 3�� �ٸ� => (��X, ��Y, ��X, ��Y)
	13,	70,	15,	75,										// 4�� �ٸ� => (��X, ��Y, ��X, ��Y)
	13,	70,	15,	75,										// 5�� �ٸ� => (��X, ��Y, ��X, ��Y)
	13,	70,	15,	75,										// 6�� �ٸ� => (��X, ��Y, ��X, ��Y)
#ifdef BLOODCASTLE_EXTEND_20040314
	13,	70,	15,	75,										// 7�� �ٸ� => (��X, ��Y, ��X, ��Y)
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
	13,	70,	15,	75,										// 8�� �ٸ� => (��X, ��Y, ��X, ��Y)
#endif
	};


const BYTE	g_btCastleDoorMapXY [MAX_BLOODCASTLE_BRIDGE_COUNT][3][4] = {	// �ٸ��� ����ؾ��� ������ǥ => (��X, ��Y, ��X, ��Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 1�� �ٸ��� ������ ������ => (��X, ��Y, ��X, ��Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 2�� �ٸ��� ������ ������ => (��X, ��Y, ��X, ��Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 3�� �ٸ��� ������ ������ => (��X, ��Y, ��X, ��Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 4�� �ٸ��� ������ ������ => (��X, ��Y, ��X, ��Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 5�� �ٸ��� ������ ������ => (��X, ��Y, ��X, ��Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 6�� �ٸ��� ������ ������ => (��X, ��Y, ��X, ��Y) x 3
#ifdef BLOODCASTLE_EXTEND_20040314
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 7�� �ٸ��� ������ ������ => (��X, ��Y, ��X, ��Y) x 3
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 8�� �ٸ��� ������ ������ => (��X, ��Y, ��X, ��Y) x 3
#endif
	};


const int g_iQuestWinExpendEXP[MAX_BLOODCASTLE_BRIDGE_COUNT][3] = {		// ����Ʈ ���� �� ���� ����ġ ���� (n/100) - 0 : ���� (���� ��Ƽ��) / 1 : �������
	18,			14,			16,
	14,			10,			12,
	11,			 7,			9,
	 9,			 5,			7,	
	 7,			 3,			5,
	 5,			 1,			3,
#ifdef BLOODCASTLE_EXTEND_20040314
	 5,			 1,			3,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����Ʈ������ ���� ����ġ ����
	 5,			 1,			3,
#endif	 
};


const int g_iQuestWinExpendZEN[MAX_BLOODCASTLE_BRIDGE_COUNT][2] = {		// ����Ʈ ���� �� ���� �� - 0 : ���� (���� ��Ƽ��) / 1 : �������
	20000,		10000,
	50000,		25000,
	100000,		50000,
	150000,		80000,
	200000,		100000,
	250000,		120000,
#ifdef BLOODCASTLE_EXTEND_20040314
	250000,		120000,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����Ʈ ���� �� ���� ��
	250000,		120000,
#endif
};


const int g_iBC_ChoasMixSuccessRate [MAX_BLOODCASTLE_BRIDGE_COUNT] = {	// ���� ĳ�� ����� (����) ī���� ���� ������
	80,
	80,
	80,
	80,
	80,
	80,
#ifdef BLOODCASTLE_EXTEND_20040314
	80,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ������� +8 ���� ������
	80,
#endif
};

const int g_iBC_ChoasMixMoney [MAX_BLOODCASTLE_BRIDGE_COUNT] = {		// ���� ĳ�� ����� (����) ī���� ���տ� �ʿ��� ��
	50000,
	80000,
	150000,
	250000,
	400000,
	600000,
#ifdef BLOODCASTLE_EXTEND_20040314
	850000,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ������� +8 ���պ��
	1050000,
#endif	
};

const int g_iBC_EventScore [MAX_BLOODCASTLE_BRIDGE_COUNT][5] = {		
	//0:���� / 1:������� / 2:����Ʈ�Ϸ��� / 3:����Ʈ�Ϸ����� ��Ƽ�� (������) / 4:����Ʈ�Ϸ����� ��Ƽ�� (�����)
	600,		300,		1000,		800,		400,
	600,		300,		1000,		800,		400,
	600,		300,		1005,		800,		400,
	600,		300,		1005,		800,		400,
	600,		300,		1005,		800,		400,
	600,		300,		1005,		800,		400,
#ifdef BLOODCASTLE_EXTEND_20040314
	600,		300,		1005,		800,		400,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// �̺�Ʈ �߰� ����
	600,		300,		1005,		800,		400,
#endif
};


const int g_iBC_EventScore_Fail [MAX_BLOODCASTLE_BRIDGE_COUNT] = {		// ���¿� ������� ������ ����.
	-300,
	-300,
	-300,
	-300,
	-300,
	-300,
#ifdef BLOODCASTLE_EXTEND_20040314
	-300,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ���н� �̺�Ʈ���� �谨
	-300,
#endif
};


/*
const int g_iBC_EventScore_Fail [MAX_BLOODCASTLE_BRIDGE_COUNT] = {		// ���¿� ������� ������ ����.
	0,
	0,
	0,
	0,
	0,
	0,
};
*/


#ifdef FOR_BLOODCASTLE2
enum {
	ADD_EXP_KILL_DOOR		= 0,
	ADD_EXP_KILL_STATUE		= 1,
	ADD_EXP_WIN_QUEST		= 2,
	ADD_EXP_PER_SEC			= 3,
};

#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
const int g_iBC_Add_Exp [MAX_BLOODCASTLE_BRIDGE_COUNT][4] = {		
	// 
	20000,		20000,		5000,		192, 
	50000,		50000,		10000,		216, 
	80000,		80000,		15000,		240, 
	90000,		90000,		20000,		264, 
	100000,		100000,		25000,		288, 
	110000,		110000,		30000,		312,	
#ifdef BLOODCASTLE_EXTEND_20040314
	120000,		120000,		35000,		336,	
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// �߰� ����ġ
	130000,		130000,		40000,		300,	
#endif
};
#else	// UPDATE_BLOODECASTLE_SCADULE_20080624
const int g_iBC_Add_Exp [MAX_BLOODCASTLE_BRIDGE_COUNT][4] = {		
	// 
	20000,		20000,		5000,		160, 
	50000,		50000,		10000,		180, 
	80000,		80000,		15000,		200, 
	90000,		90000,		20000,		220, 
	100000,		100000,		25000,		240, 
	110000,		110000,		30000,		260,	
#ifdef BLOODCASTLE_EXTEND_20040314
	120000,		120000,		35000,		280,	
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// �߰� ����ġ
	130000,		130000,		40000,		300,	
#endif
};
#endif
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624


#ifdef BLOODCASTLE_EVENT_5TH_20050531			// �����ο��� ���� �߰� ����Ʈ
const int g_iBC_Party_EventPoint [MAX_PARTYUSER] = {
	5,			// 1��
	10,			// 2��
	15,			// 3��			
	20,			// 4��
	30,			// 5��
};

const int g_iBC_MONSTER_CHANGE_STATE[2][3] = {	// ����ĳ������ �ð��� ���� ���� ���̳��� ������ �ɷ�ġ
	// �ּ� ���ݷ�, �ִ� ���ݷ�, ����
	10,		20,		-20,						// 5�� ���
	20,		40,		-50,						// 10�� ���
};
#endif


enum {
	BC_STATE_START		=0,
	BC_STATE_PLAY		=1,
	BC_STATE_END		=2,
	BC_STATE_MONSTEREND	=3,						// ���� �Ҵ緮 - 1 �϶� Ŭ���̾�Ʈ�� �ٸ� ������� ����
	BC_STATE_PLAY_BOSS	=4,						
};


enum {
	BC_USER_ALIVE				=0,				// �׳� ������ (����� ������ �⺻��)
	BC_USER_DEAD				=1,				// �����
	BC_USER_WINNER				=2,				// �����
	BC_USER_WINNER_PARTY_ALIVE	=3,				// ������� ��Ƽ�� (������)
	BC_USER_WINNER_PARTY_DEAD	=4,				// ������� ��Ƽ�� (�����)
};


enum {
	BC_REWARD_WINNER,							// �����
	BC_REWARD_LOSER,							// �й���
	BC_REWARD_ALIVE,							// ��Ƴ�����
};


enum {
	BC_MSG_BEFORE_ENTER	= 3,					// [0x92] ���� 30������ �޽��� Ÿ��
	BC_MSG_BEFORE_PLAY	= 4,					// [0x92] ������ 30������ �޽��� Ÿ��
	BC_MSG_BEFORE_END	= 5,					// [0x92] ������� 30������ �޽��� Ÿ��
	BC_MSG_BEFORE_QUIT	= 6,					// [0x92] ���� 30������ �޽��� Ÿ��
};


enum {
	BC_MAPNUMBER_CHAOSGEM1		 = 238,			// ȥ���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (1�ٸ�)
	BC_MAPNUMBER_CHAOSGEM2		 = 239,			// ȥ���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (2�ٸ�)
	BC_MAPNUMBER_CHAOSGEM3		 = 240,			// ȥ���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (3�ٸ�)
	BC_MAPNUMBER_CHAOSGEM4		 = 241,			// ȥ���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (4�ٸ�)
	BC_MAPNUMBER_CHAOSGEM5		 = 242,			// ȥ���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (5�ٸ�)
	BC_MAPNUMBER_CHAOSGEM6		 = 243,			// ȥ���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (6�ٸ�)
#ifdef BLOODCASTLE_EXTEND_20040314
	BC_MAPNUMBER_CHAOSGEM7		 = 244,			// ȥ���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (7�ٸ�)
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ȥ���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (8�ٸ�) �����ʿ���
	BC_MAPNUMBER_CHAOSGEM8		 = 245,			// ȥ���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (8�ٸ�)
#endif

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ 
	BC_MAPNUMBER_ULTIMATEWEAPON1 = 246,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (1�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON2 = 247,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (2�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON3 = 248,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (3�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON4 = 249,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (4�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON5 = 250,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (5�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON6 = 251,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (6�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON7 = 252,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (7�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON8 = 253,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (8�ٸ�) *254,255�� �����.

#else
	BC_MAPNUMBER_ULTIMATEWEAPON1 = 247,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (1�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON2 = 248,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (2�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON3 = 249,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (3�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON4 = 250,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (4�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON5 = 251,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (5�ٸ�)
	BC_MAPNUMBER_ULTIMATEWEAPON6 = 252,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (6�ٸ�)
#ifdef BLOODCASTLE_EXTEND_20040314
	BC_MAPNUMBER_ULTIMATEWEAPON7 = 253,			// ���빫���� ������ �� Ư�� �Ӽ��� �ֱ����� �ʹ�ȣ (7�ٸ�)
#endif
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
};

#define BC_MAPNUMBER_BASE_CHAOSGEM				(BC_MAPNUMBER_CHAOSGEM1 - MAP_INDEX_BLOODCASTLE1)
#define BC_MAPNUMBER_BASE_ULTIMATEWEAPON		(BC_MAPNUMBER_ULTIMATEWEAPON1 - MAP_INDEX_BLOODCASTLE1)


/*===================================================
//	<< ���� ĳ�� ���� ���� ����ü >>
// : ���� ĳ���� �� �ٸ��� ���������� ������ ������ �ִ�.	(-> �� �������� ��ũ��Ʈ �����͸� �ε��ϱ� ������ �ʱ�ȸ�Ǹ� �ȴ�.)
===================================================*/
typedef struct _BLOODCASTLE_BOSS_MONSTER {
	INT iMonsterClass;
	INT	iMapNumber;
	INT iBridgeNum;
	INT	iSTART_X;
	INT	iSTART_Y;
	INT iEND_X;
	INT iEND_Y;
	INT iHEALTH;
} BLOODCASTLE_BOSS_MONSTER, * LPBLOODCASTLE_BOSS_MONSTER;


/*===================================================
//	<< ���� ĳ�� ���� ����ü >>
// : ���� ĳ���� �� 8���� �ٸ� ���� �� ���� �����͸� �����Ѵ�.
===================================================*/
typedef struct _BLOODCASTLE_USER {
	INT		m_iIndex;								// ������ ������� ����ü
	INT		m_iEXP;									// ����ڰ� ���� ����ġ
	INT		m_iScore;								// ����ڰ� ���� ����
	INT		m_iUserState;							// ������� ���� (���߿� ������ �ϱ�����)
	bool	m_bSendQuitMsg;							// ����ڰ� ����ĳ�� ������ �������� ����޽����� �̹� �����־��°� �ƴѰ�? (�ʱⰪ : false)
	bool	m_bLiveWhenDoorBreak;					// ������ �ı��Ǿ��� �� ����־��°�?
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
	char	m_szAccountID	[MAX_IDSTRING+1];		// ������
	char	m_szCharName	[MAX_IDSTRING+1];		// �ɸ���
	INT		m_iCharClass;							// Ŭ����
	INT		m_iBloodCastleIndex;					// ����ĳ�� �ε���
	INT		m_iBloodCastleSubIndex;					// ����ĳ�� �� �ε���
#endif	
} BLOODCASTLE_USER, * LPBLOODCASTLE_USER;

typedef struct _BLOODCASTLE_BRIDGE {
	CRITICAL_SECTION	m_critUserData;										// m_UserData[] �� ������ ������� ��ȣ�ϱ� ����
	BLOODCASTLE_USER	m_UserData [MAX_BLOODCASTLE_USER_COUNT];			// ����� ����ü
	INT					m_nBossMonsterPosNum[MAX_BLOODCASTLE_BOSS_COUNT];	// ���������� MonsterSetBase������ ��ġ

	INT					m_nSaintStatuePosNum;								// ������ ������ MonsterSetBase ������ ��ġ�� �����Ѵ�.
	INT					m_nCastleDoorPosNum;								// ������ MonsterSetBase ������ ��ġ�� �����Ѵ�.

	INT					m_iTOTAL_EXP;										// ����ڵ��� ���� ��ü ����ġ ?
	INT					m_iMapNumber;										// �ڽ��� �ʹ�ȣ
	INT					m_iBridgeIndex;										// �ڽ��� �ٸ���ȣ
	INT					m_iMISSION_SUCCESS;									// �� �ٸ��� �̼��� �����ߴ�. (�ƴϸ� -1 ������ 0 ~ 15�� ����� �ε���)
	BOOL				m_bCASTLE_DOOR_LIVE;								// �ٸ��� ���� ���� ����ִ��� �׾����� ǥ��

	INT					m_iBC_STATE;										// ���� ���¸� ��Ÿ����.
	INT					m_iBC_REMAIN_MSEC;									// ���� ���·� ��������� �ð� (1/1000 ��)
	INT					m_iBC_TICK_COUNT;									// ī��Ʈ �۾����� �ϱ����� �ð� (1/1000��)
	INT					m_iBC_NOTIFY_COUNT;									// ������ ������ ���� ī����
	bool				m_bBC_MONSTER_KILL_COMPLETE;						// ���� �����Ҵ���� �޼��Ͽ��°� ?
	bool				m_bBC_BOSS_MONSTER_KILL_COMPLETE;					// ���� ���� �����Ҵ���� �޼��Ͽ��°� ?

	bool				m_bBC_DOOR_TERMINATE_COMPLETE;						// ������ �ν����°�?
	

	bool				m_bBC_CAN_ENTER;									// ���� ���� �����Ѱ� ?
	bool				m_bBC_CAN_PARTY;									// ���� ��Ƽ �����Ѱ� ?
	bool				m_bBC_PLAY_START;									// ��Ⱑ ���۵Ǿ��°� ?
	bool				m_bBC_REWARDED;										// ������ ������ �޾Ҵ°�?
	bool				m_bBC_MSG_BEFORE_ENTER;								// 30�ʾ˸� - �������� ���´°�?
	bool				m_bBC_MSG_BEFORE_PLAY;								// 30�ʾ˸� - ���������� ���´°�?
	bool				m_bBC_MSG_BEFORE_END;								// 30�ʾ˸� - ����������� ���´°�?
	bool				m_bBC_MSG_BEFORE_QUIT;								// 30�ʾ˸� - �������� ���´°�?

	float				m_iCastleStatueHealth;								// �ٸ��� ������ ������ ü��
	float				m_iCastleDoorHealth;								// �ٸ��� ������ ü��
	
	INT					m_iBC_MONSTER_MAX_COUNT;							// �ٸ��� ��ƾ� �ϴ� �ִ� ���� ��
	INT					m_iBC_MONSTER_KILL_COUNT;							// �ٸ��� ������� ���� ���� ��
	INT					m_iBC_MONSTER_SUCCESS_MSG_COUNT;					// �ٸ��� �Ҵ緮 ���͸� ä������ �������� ������ ���� Ƚ��
	DWORD				m_dwBC_TICK_DOOR_OPEN;								// �ٸ��� ���͸� �� ���� �� BC_MAX_TICK_DOOR_OPEN ���� �����̸� �ְ� �����Ӽ��� �����ϱ� ����

	INT					m_iBC_BOSS_MONSTER_MAX_COUNT;						// �ٸ��� ��ƾ� �ϴ� �ִ� �������� ��
	INT					m_iBC_BOSS_MONSTER_KILL_COUNT;						// �ٸ��� ������� ���� ���� ���� ��
	INT					m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT;				// �ٸ��� �Ҵ緮 ���� ���͸� ä������ �������� ������ ���� Ƚ��

	DWORD				m_nBC_QUESTITEM_SERIAL;								// ��õ���� ���빫�� �ø����� �ø����� �����Ѵ�.
	INT					m_iBC_QUEST_ITEM_USER_INDEX;						// ��õ���� ���빫�⸦ ���� ����� �ε����� �����Ѵ�.
	BYTE				m_btBC_QUEST_ITEM_NUMBER;							// ���� �ٸ��� ��õ���� ���빫���� ������ȣ (0:������ / 1:�� / 2:����)

	INT					m_iAngelKingPosNum;									// ��õ���� ���� ������ �ε����� ������ �ִ´�.

	INT					m_iExtraEXP_Kill_Door_Party;						// ����Ʈ ���� �� ������ �ı��� ��Ƽ��ȣ
	INT					m_iExtraEXP_Kill_Door_Index;						// ����Ʈ ���� �� ������ �ı��� ����ڹ�ȣ
	CHAR				m_szKill_Door_AccountID[MAX_IDSTRING+1];			// ����Ʈ ���� �� ������ �ı��� ����ڰ�����
	CHAR				m_szKill_Door_CharName[MAX_IDSTRING+1];				// ����Ʈ ���� �� ������ �ı��� ������ɸ���
	INT					m_iExtraEXP_Kill_Statue_Party;						// ����Ʈ ���� �� ������ �ı��� ��Ƽ��ȣ
	INT					m_iExtraEXP_Kill_Statue_Index;						// ����Ʈ ���� �� ������ �ı��� ����ڹ�ȣ
	CHAR				m_szKill_Status_CharName[MAX_IDSTRING+1];			// ����Ʈ ���� �� ������ �ı��� ������ɸ���
	CHAR				m_szKill_Status_AccountID[MAX_IDSTRING+1];			// ����Ʈ ���� �� ������ �ı��� ����ڰ�����
	INT					m_iExtraEXP_Win_Quest_Party;						// ���Ŀ� ����Ʈ�� ������ ��Ƽ��ȣ
	INT					m_iExtraEXP_Win_Quest_Index;						// ���Ŀ� ����Ʈ�� ������ ����ڹ�ȣ
	CHAR				m_szWin_Quest_CharName[MAX_IDSTRING+1];				// ���Ŀ� ����Ʈ�� ������ ������ɸ���
	CHAR				m_szWin_Quest_AccountID[MAX_IDSTRING+1];			// ����Ʈ ���� �� ������ �ı��� ����ڰ�����

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	INT					m_iBC_COMPLETE_USER_INDEX;							// �Ϸ��� ������� �ε��� -> ��Ƽ���� �����ϱ� ����
#endif

#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
	INT					m_iDoorIndex;										// ������ �����ɶ��� gObj�� �ε���
#endif
	
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	FLOAT				m_fRewardExpRate;
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

	INT				m_iDropItemCount;
} BLOODCASTLE_BRIDGE, * LPBLOODCASTLE_BRIDGE;

typedef struct 
{
	unsigned short item_type;
	char item_level;
	bool is_skill;
	bool is_luck;
	char option;
	char exc_option;
	unsigned short max_drop_rate;
} BLOODCASTLE_REWARD, *LPBLOODCASTLE_REWARD;

typedef std::list<BLOODCASTLE_REWARD> tdBCRewardList;


/*===================================================
//	<< ���� ĳ�� ����ü >>
// : ���� ĳ���� ��ü���� ����, �����층, ����Ȯ����� ��� �������� ���� �ϴ� �̺�Ʈ ������ Ŭ����
===================================================*/
class CBloodCastle  
{
public:
	/* ī���� ���� ���� �Լ��� */
	INT		CheckChoasMixItem	(INT iIndex);				// �ش� ����ڰ� ������並 ���� ��� (��õ���Ǽ�, ���庻) �� ������ �ִ��� Ȯ�� (0 �̸� ���� / 1 ~ 7 �� �� ������ ���) / 8 �� �ٸ������� �߰� / 9�� ���� �ʸ��� / 10�� ȥ������ / 11�� �������� �ϳ� ���� / 12�� �̺�Ʈ�������� �ʹ� ���� / 14 �� �� ������ ����.
	bool	BloodCastleChaosMix (INT iIndex, INT iLEVEL);	// ����ĳ�� ����� (����) ������ �����Ѵ�.

	/* ����� ����ġ �� ������� �Լ��� */
	bool	AddExperience		(INT iIndex, INT iEXP);		// ������� �ٸ���ȣ�� Ȯ���ϰ� ����ġ�� �߰��Ѵ�.
	INT		CalcSendRewardEXP	(INT iIndex, INT iEXP);		// ����ġ�� ����ϰ� �ش� ����ڿ��� �����ش�.
	INT		CalcSendRewardZEN	(INT iIndex, INT iZEN);		// ���� ����ϰ� �ش� ����ڿ��� �����ش�.
	void	DropChaosGem		(INT iIndex, INT iBridgeNum);				// Ư�� ����ڰ� �ִ� �ڸ��� ȥ���� ������ ������.

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	void	SendRewardScore		(							// ��ŷ������ ���������� ������.
								INT iIndex, 
								INT iSCORE,
								INT iLeftTime,
								INT iAlivePartyCount
								);
#else	
#ifdef BLOODCASTLE_EVENT_3RD_20040401
	void	SendRewardScore		(							// ��ŷ������ ���������� ������.
								INT iIndex, 
								INT iSCORE,
								INT iLeftTime
								);
#else
	void	SendRewardScore		(INT iIndex, INT iSCORE);	// ��ŷ������ ���������� ������.
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
	void	SendRewardScore		(							// ��ŷ������ ���������� ������.
								LPBLOODCASTLE_USER lpBC_USER, 
								INT iSCORE,
								INT iLeftTime
								);
#endif
	
	void	GiveReward_Win		(							// �ٸ����� ������ ����(����) �Ǿ��� �� ��Ȳ�� ���� ������ ������ �Ѵ�.
				INT iIndex,
				INT iBridgeIndex
				);
	void	GiveReward_Fail		(INT iBridgeIndex);			// �ٸ����� ������ ����(����) �Ǿ��� �� �⺻ ������ �ϰ������� ������ �Ѵ�.

	/* ����ĳ�� ���Ӱ��� �Լ��� */
	void	SearchUserDeleteQuestItem (INT iIndex);			// ����ڸ� �˻��Ͽ� ����Ʈ �������� ��ӽ�Ų��.
	void	SearchUserDropQuestItem (INT iIndex);			// ����ڸ� �˻��Ͽ� ����Ʈ �������� �����.
	void	SetUserState (									// ���� ������ ���� ����� ���¸� �����Ѵ�. 
				INT iIndex,
				INT iState
				);
	void	SendCastleEntranceBlockInfo (					// Ư�� �ٸ��� ��� ����ڿ��� Ư���ٸ� �Ա��� ���� ��������� ������.
				INT iBridgeIndex,
				bool bLive
				);
	void	SendCastleBridgeBlockInfo (						// Ư�� �ٸ��� ��� ����ڿ��� Ư���ٸ��� ���� ��������� ������.
				INT iBridgeIndex,
				bool bLive
				);
	void	SendCastleDoorBlockInfo (						// Ư�� �ٸ��� ��� ����ڿ��� Ư������ ���� ���� ��������� ������.
				INT iBridgeIndex,
				bool bLive
				);
	void	SendAllUserAnyMsg(								// ������ �ִ� ��� ����ڵ鿡�� �޽����� ������.
				LPBYTE lpMsg, 
				INT iSize
				);
	void	SendBridgeAnyMsg(								// Ư�� �ٸ��� ����ڵ鿡�� �޽����� ������.
				LPBYTE lpMsg, 
				INT iSize, 
				INT iBridgeIndex
				);
	void	SendNoticeMessage (								// Ư�� �ٸ��� �ο��鿡�� ������ ������. (0x0D)
				INT iBridgeIndex,
				char * lpszMSG
				);
	void	SendNoticeScore (								// Ư�� �ٸ��� �ο��鿡�� �ڽ��� ������ ������. (0x0D)
				INT iBridgeIndex
				);
	void	SendNoticeState (								// Ư�� �ٸ��� �ο��鿡�� ���� ������� ��Ȳ(�ð�, ���ͼ�)�� ������.
				INT iBridgeIndex,
				INT iPlayState
				);
	bool	CheckUserBridgeMember(							// Ư�� ����ڰ� Ư�� �ٸ��� ��������� üũ�Ѵ�.
				INT iBridgeIndex,
				INT iIndex
				);
	void	BlockCastleEntrance		(INT iBridgeIndex);		// Ư�� �ٸ��� �Ա��� �� �Ӽ����� ���´�.
	void	ReleaseCastleEntrance	(INT iBridgeIndex);		// Ư�� �ٸ��� �Ա��� �� �Ӽ��� Ǭ��.
	void	BlockCastleBridge		(INT iBridgeIndex);		// Ư�� �ٸ��� �ո� �Ӽ����� ���´�.
	void	ReleaseCastleBridge		(INT iBridgeIndex);		// Ư�� �ٸ��� �ո� �Ӽ��� Ǭ��.
	void	BlockCastleDoor			(INT iBridgeIndex);		// Ư�� �ٸ��� ������ �� �Ӽ��� ���´�.
	void	ReleaseCastleDoor		(INT iBridgeIndex);		// Ư�� �ٸ��� ������ �� �Ӽ��� Ǭ��.
	void	SetMonsterKillCount		(INT iBridgeIndex);		// Ư�� �ٸ��� ���� ���� �Ҵ���� �����Ѵ�.
	bool	CheckMonsterKillCount	(INT iBridgeIndex);		// Ư�� �ٸ��� ���� ���� �Ҵ���� ä�������� Ȯ���Ѵ�. (���� ����Ȯ��)
	bool	CheckMonsterKillSuccess	(INT iBridgeIndex);		// Ư�� �ٸ��� ���� ���� �Ҵ���� ä�������� Ȯ���Ѵ�. (������ Ȯ��)
	bool	CheckBossKillCount		(INT iBridgeIndex);		// Ư�� �ٸ��� ���� ���� ���� �Ҵ���� ä�������� Ȯ���Ѵ�. (���� ����Ȯ��)
	bool	CheckBossKillSuccess	(INT iBridgeIndex);		// Ư�� �ٸ��� ���� ���� ���� �Ҵ���� ä�������� Ȯ���Ѵ�. (������ Ȯ��)
	bool	CheckEveryUserDie		(INT iBridgeIndex);		// Ư�� �ٸ��� ��� ����ڰ� �װų� ���ŵǾ����� Ȯ���Ѵ�. (true : ���׾���)		
	bool	CheckAngelKingExist		(INT iBridgeIndex);		// Ư�� �ٸ��� ��õ�簡 �ִ��� ������ üũ (false : �ƿ� �����ǹ�ȣ�� ����)
	INT		GetWhoGotUltimateWeapon	(INT iBridgeIndex);		// Ư�� �ٸ��� ��õ���� ���빫�Ⱑ �ִ� �ɸ��� ����� �ε����� ���� (������ -1)
	INT		GetCurrentLiveUserCount	(INT iBridgeIndex);		// Ư�� �ٸ��� �������� ���� ��ȯ
#ifdef BLOODCASTLE_ITEMDROP_DIRECTLY_20040728
	BOOL	DropItemDirectly		(						// �������� ���ǰ˻� ���� �ٷ� ����Ѵ�. (��õ���� ���빫�� ��ӿ��� ���)
				INT iBridgeIndex,
				INT iIndex, 
				INT iItemType,
				INT iItemPos
				);
#endif

	/* ����ĳ�� ���� ���� �Լ��� */
	void	ClearMonster	(								// �ش� �ٸ��� ��� ���� ����
				INT iBridgeIndex,
				bool bClearCastleDoor
				);
	void	SetMonster		(INT iBridgeIndex);				// ���� ĳ���� ������ ������ Ư�� �Ӽ��� SET�ϰ� ��ġ�Ѵ�. (�ʹ�ȣ�� 11~17�� ���͵� �ϰ���ġ)
	void	SetBossMonster	(INT iBridgeIndex);				// ������ �о��� ���� ������ �Ӽ��� ���ʷ� ���������� �Ӽ��� SET�ϰ� ��ġ�Ѵ�.
	void	SetSaintStatue	(INT iBridgeIndex);				// ������ �о��� ������ ������ ��ġ�Ѵ�.
	void	LoadBossMonster (								// ���� ������ �Ӽ��� ��ũ��Ʈ ���Ͽ��� �о ������ �ִ´�.
				INT iMonsterClass,	
				INT iMapNumber, 
				INT iBridgeNum,
				INT iSTART_X,
				INT iSTART_Y,
				INT iEND_X,
				INT iEND_Y,
				INT iHEALTH
				);
	
	/* ����ĳ�� ����� ���� �Լ��� */
	INT		LeaveUserBridge (								// iBridgeNum ��° �ٸ��� iBridgeIndex ��° ������� ���� ����. (iUserIndex �� ��������)
				INT iBridgeIndex, 
				INT iBridgeSubIndex,
				INT iUserIndex
				);	
	INT		EnterUserBridge (								// iIndex�� ����ڸ� iBridgeNum ��° �ٸ��� �Թ��Ų��. (-1 ���� �� �����)	
				INT iBridgeIndex, 
				INT iUserIndex
				);
#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	INT		LevelUp (INT iIndex, INT iAddExp, INT iEventType);				// ����ĳ�� ���� ������ �޼���
#else
	INT		LevelUp (INT iIndex, INT iAddExp);				// ����ĳ�� ���� ������ �޼���
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	VOID	CheckUsersOnConnect (							// ī���� ĳ���� ������ ����ڰ� ���� ������ üũ
				INT iBridgeIndex
				);
	
	/* ���� ���¹�ȯ �Լ��� */
	INT		GetCurrentState(INT iBridgeIndex);
	INT		GetRemainTime(INT iBridgeIndex);				// ������ ���� ���� �ð��� Ȯ���Ѵ�. (�д���)
	INT		GetCurrentRemainSec(INT iBridgeIndex);			// ���� �������±��� �������� �ʸ� ��ȯ
	INT		GetExcelItemDropRate() {						// �Ϲ� ������ �����
				return m_iBC_EXCEL_ITEM_DROP; }
	INT		GetNormalItemDropRate() {						// �Ϲ� ������ �����
				return m_iBC_NORMAL_ITEM_DROP; }

	/* ���� ����üũ �Լ��� */
	INT		CheckEnterLevel		(INT iIndex, INT iLevel);	// �ش� ����ڰ� ���� ĳ���� ������ �� �ִ��� Ȯ�� (0:����, -1:����, +1:����, 2:�ٸ�����)
#ifdef MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
	bool	CheckEnterFreeTicket(INT iIndex);	// ���� ������� �����ϰ� �ִ��� Ȯ��
#endif // MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
	INT		CheckEnterItem		(INT iIndex);				// �ش� ����ڰ� ���� ĳ���� ������ �� �ִ� ������ (�������) �� �ִ��� Ȯ�� (0 �̸� �Ұ� / 1 ~ n ���� ���� ����)
	INT		CheckQuestItem		(INT iIndex);				// �ش� ����ڰ� ���� ĳ���� ����Ʈ ������ (��õ���� ������, ��, ���� �� �ϳ�) �� �ִ��� Ȯ�� (0 �̸� �Ұ� / 1 ~ 3 ���� ���� ������, ��, ����)
	bool	CheckWalk			(							// ���ð� �� �������� ������ �������� �ϴ��� �˻� (�����ٸ� true)
				INT iIndex,
				INT iMoveX,
				INT iMoveY						
				);
	bool	CheckCanEnter		(INT iBridgeIndex);			// ���� ����ĳ�� ������ �������� Ȯ���Ѵ�.
	bool	CheckCanParty		(INT iBridgeIndex);			// ���� ����ĳ�� ��Ƽ�� �������� Ȯ���Ѵ�.
	bool	CheckPlayStart		(INT iBridgeIndex);			// ���� ����ĳ�� ������ �������� Ȯ���Ѵ�.
	bool	CheckQuestItemSerial(							// ����Ʈ �������� �ø����� Ư�� �����۰� �´��� Ȯ��
				INT iBridgeIndex,
				CMapItem * lpItem
				);
	bool	CheckUserHaveUlimateWeapon (INT iIndex);		// �ش� ����ڰ� ��õ���� ���빫�⸦ ������ �ִ��� Ȯ��

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	bool	CheckWinnerExist (								// ����� ����ڰ� �ִ��� üũ
				INT iBridgeIndex
				);
	bool	CheckWinnerValid (								// ����� ����ڰ� ������ ���ų� ���� ��Ż���� �ʾҴ��� üũ
				INT iBridgeIndex
				);
	bool	CheckUserWinnerParty (							// ����ڰ� ������� ��Ƽ�� �ִ��� Ȯ�� (������ ����ĳ�� ���� �����ؾ� ��)
				INT iBridgeIndex,
				INT iIndex
				);
	bool	CheckPartyExist		(							// Ư�� ������� ��Ƽ���� ����ĳ�� �ʳ��� �����ϴ��� Ȯ���Ѵ�.
				INT iIndex
				);
	bool	CheckWinnerPartyComplete (						// ������� ��Ƽ�� �¸��ߴ��� ����
				INT iBridgeIndex
				);
	bool	SetBridgeWinner		(							// Ư�� �ٸ��� ����ڸ� ���Ѵ�.
				INT iBridgeIndex,
				INT iIndex
				);
	int		GetWinnerPartyCompleteCount (					// Ư�� �ٸ��� ������� ��Ƽ�� ��ȭ�� ������ ����� ������� Ȯ���Ѵ�.
				INT iBridgeIndex
				);
	int		GetWinnerPartyCompletePoint (					// Ư�� �ٸ��� ������� ��Ƽ�����ڿ� ���� ������ ���Ѵ�.
				INT iBridgeIndex
				);
	void	ChangeMonsterState (							// ����ĳ���� �����ϴ� ���ʹ� ������ �� �ɷ�ġ ���� ���θ� �����Ͽ� �����Ѵ�.
				INT iBridgeIndex,
				INT iIndex
				);
	void	FixUsersPlayStateWin (							// ����ĳ���� ���� �� �������� ������ ����ڵ��� ���¸� �ٽ��ѹ� üũ ������ ��
				INT iBridgeIndex
				);
	void	FixUsersPlayStateFail (							// ����ĳ���� ���� �� �������� ������ ����ڵ��� ���¸� �ٽ��ѹ� üũ ������ ��
				INT iBridgeIndex
				);
#endif
	

	/* ����ĳ�� �����층 ���� �Լ��� */
	void	Run();
	void	Init (bool bEVENT_ENABLE = TRUE);
	void	Load (char* filename);
	void	LoadItemDropRate ();
	void	SetState (INT iBridgeIndex, INT iBC_STATE);		// Ư�� �ٸ��� �����췯�� Ư�� ���·� �����Ѵ�.
	CBloodCastle();
	virtual ~CBloodCastle();


public:
	BLOODCASTLE_BRIDGE m_BridgeData [MAX_BLOODCASTLE_BRIDGE_COUNT];		// ����ĳ�� �ٸ� ����ü


private:
	bool	m_bBC_EVENT_ENABLE;								// �̺�Ʈ�� �Ұ��ΰ� �ƴѰ� ?

	INT		m_iBC_TIME_MIN_OPEN;							// ����ð� (��) -> ��� ���� ������ ���ΰ�?
	INT		m_iBC_TIME_MIN_PLAY;							// ���ӽð�	(��)
	INT		m_iBC_TIME_MIN_REST;							// ���� �� �޽Ľð�	(��)

	INT		m_iBC_NORMAL_ITEM_DROP;							// ���͵��� �Ϲݾ����� �����
	INT		m_iBC_EXCEL_ITEM_DROP;							// ���͵��� ���������� �����
	INT		m_iBC_MONSTER_REGEN;							// ���� �����ð� (ms)

	tdBCRewardList	m_LST_BLOODCASTLE_REWARD[MAX_BLOODCASTLE_BRIDGE_COUNT];

	
protected:
	void	CheckSync			(INT iBridgeIndex);			// Ư�� �ٸ��� ��ũ�� �����.
	void	ClearBridgeData		(INT iBridgeIndex);			// Ư�� �ٸ� �����͸� �ʱ�ȭ �Ѵ�.

	void	ProcState_None		(INT iBridgeIndex);
	void	ProcState_Closed	(INT iBridgeIndex);
	void	ProcState_Playing	(INT iBridgeIndex);
	void	ProcState_PlayEnd	(INT iBridgeIndex);

	void	SetState_None		(INT iBridgeIndex);			// ����ĳ�� �̺�Ʈ ���� �� ���
	void	SetState_Closed		(INT iBridgeIndex);
	void	SetState_Playing	(INT iBridgeIndex);
	void	SetState_PlayEnd	(INT iBridgeIndex);

	void	BlockSector(									// ���� Ư�����Ϳ� �� �Ӽ��� ���Ƿ� �ִ´�.
				INT iMAP_NUM,								// �ʹ�ȣ
				INT iSTART_X,								// ���� X
				INT iSTART_Y,								// ��� Y
				INT iEND_X,									// ���� X
				INT iEND_Y									// �ϴ� Y
				);		
	void	ReleaseSector(									// �ʿ� �����ϴ� �� �Ӽ��� �����Ѵ�.
				INT iMAP_NUM,
				INT iSTART_X,								// ���� X
				INT iSTART_Y,								// ��� Y
				INT iEND_X,									// ���� X
				INT iEND_Y									// �ϴ� Y
				);


	/* ����� ����ġ �� ������� �Լ��� */
	INT		GetAliveUserTotalEXP(							// ����ִ� ����ڵ��� �� ����ġ�� ���Ѵ�.
				INT iBridgeIndex
				);		
	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ���ȣ�� �ʹ�ȣ ���
public:
	INT		GetMapNumByBCBridge( INT iBridgeIndex );	// �ٸ� �ε����� �� ��ȣ ���ϱ�
	INT		GetBridgeIndexByMapNum( INT iMapNum );		// �� ��ȣ�� �ٸ� �ε��� ���ϱ�

	INT		GetMapNumByBC_CHAOSGEM( INT iChaosGem );			// DS�� ���(ȥ�� ������ ����)�� ���� �� ��ȣ�� ���Ѵ�.
	INT		GetMapNumByBC_ULTIMATEWEAPON( INT iUltimateWeapon );	// DS�� ���(���빫�� ������ ����)�� ���� �� ��ȣ�� ���Ѵ�.
#endif

	
#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
	list <BLOODCASTLE_STARTTIME>	m_LST_BLOODCASTLE_START_TIME;
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624

};


extern CBloodCastle g_BloodCastle;

#endif // !defined(AFX_BLOODCASTLE_H__B6D64999_D5F0_453A_BBC9_A8567758D31D__INCLUDED_)
