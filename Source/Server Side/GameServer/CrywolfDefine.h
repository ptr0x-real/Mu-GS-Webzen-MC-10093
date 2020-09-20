#ifndef __CRYWOLFDEFINE_DEF__
#define __CRYWOLFDEFINE_DEF__

//---------------------------------------------------------------------------------------------
//
// �����췯 ����
//----------------------------------------------------------------------------------------------

#define MAX_CRYWOLF_STATE_TYPE	7
#define MAX_CRYWOLF_STARTTIME	20


enum CRYWOLF_STATE_TYPE
{
	CRYWOLF_STATE_NONE					= 0,
	CRYWOLF_STATE_NOTIFY_1				= 1,
	CRYWOLF_STATE_NOTIFY_2				= 2,
	CRYWOLF_STATE_READY					= 3,
	CRYWOLF_STATE_START					= 4,
	CRYWOLF_STATE_END					= 5,
	CRYWOLF_STATE_ENDCYCLE				= 6,
};

enum CRYWOLF_SCHEDULE_MODE
{
	CRYWOLF_SCHEDULE_MODE_COMMON		= 0,	// �Ϲ� ������
	CRYWOLF_SCHEDULE_MODE_ITERATION		= 1,	// �ݺ� ������
};

#define MAX_CRYWOLF_MAP_STATE	3

enum CRYWOLF_OCCUPATION_STATE_TYPE
{
	CRYWOLF_OCCUPATION_STATE_PEACE		= 0,	// ��ȭ ����
	CRYWOLF_OCCUPATION_STATE_OCCUPIED	= 1,	// ���� ����
	CRYWOLF_OCCUPATION_STATE_WAR		= 2,	// ���� ����
};

enum CRYWOLF_OBJECT_TYPE
{
	CRYWOLF_OBJ_NONE					= 0,	// ũ���̿��� OBJ�� �ƴ�
	CRYWOLF_OBJ_COMMON_NPC				= 1,	// ũ���̿��� NPC
	CRYWOLF_OBJ_SPECIAL_NPC				= 2,	// ũ���̿��� ����, ���� Ư�� NPC
	CRYWOLF_OBJ_COMMON_MONSTER			= 3,	// ũ���̿��� �Ϲ� MONSTER
	CRYWOLF_OBJ_SPECAIL_MONSTER			= 4,	// ũ���̿��� Ư�� ���� MONSTER 
	CRYWOLF_OBJ_BOSS1					= 5,	// ũ���̿��� ���� 1
	CRYWOLF_OBJ_BOSS2					= 6,	// ũ���̿��� ���� 2
};

//---------------------------------------------------------------------------------------------
//
// NPC ����
//----------------------------------------------------------------------------------------------

#define CRYWOLF_NPC_STATUE				204		// ������ �Ż� 

#define CRYWOLF_NPC_ALTAR1				205		// ������ ���� 1
#define CRYWOLF_NPC_ALTAR2				206		// ������ ���� 2
#define CRYWOLF_NPC_ALTAR3				207		// ������ ���� 3
#define CRYWOLF_NPC_ALTAR4				208		// ������ ���� 4
#define CRYWOLF_NPC_ALTAR5				209		// ������ ���� 5


#define CHECK_CRYWOLF_NPC_STATUE(C_VALUE)		(C_VALUE!=CRYWOLF_NPC_STATUE?FALSE:TRUE)
#define CHECK_CRYWOLF_NPC_ALTAR(C_VALUE)		(C_VALUE<CRYWOLF_NPC_ALTAR1?FALSE:C_VALUE>CRYWOLF_NPC_ALTAR5?FALSE:TRUE)

#define CHECK_CRYWOLF_SPECIAL_NPC(C_VALUE)		(C_VALUE<CRYWOLF_NPC_STATUE?FALSE:C_VALUE>CRYWOLF_NPC_ALTAR5?FALSE:TRUE)

#define CHECK_CRYWOLF_FIRSTZONE(C_VALUE)		(C_VALUE!=MAP_INDEX_CRYWOLF_FIRSTZONE?FALSE:TRUE)


//---------------------------------------------------------------------------------------------
//
// MONSTER ����
//----------------------------------------------------------------------------------------------

#define MAX_CRYWOLF_MVP_MONSTER_GROUP	20		// ũ���̿��� ��Ʈ�ͱ׷� �ְ� ����

enum CRYWOLF_MVP_MONSTER
{
	CRYWOLF_MON_BALGARS				= 349,
	CRYWOLF_MON_DARKELF				= 340,
	CRYWOLF_MON_BALRAM				= 344,
	CRYWOLF_MON_SORAM				= 341,
	CRYWOLF_MON_DEATHSPIRIT			= 345,
	CRYWOLF_MON_TANKER				= 348,
};

//---------------------------------------------------------------------------------------------
//
// ��� ��ɾ�
//----------------------------------------------------------------------------------------------
enum CRYWOLF_GM_COMMAND
{
	CRYWOLF_GM_COMMAND_NOTIFY_2		= 0,
	CRYWOLF_GM_COMMAND_READY		= 1,
	CRYWOLF_GM_COMMAND_START		= 2,	
	CRYWOLF_GM_COMMAND_END			= 3,
};


//---------------------------------------------------------------------------------------------
//
// ����, �г�Ƽ, ����, ��ũ ����
//----------------------------------------------------------------------------------------------

#define MAX_CRYWOLF_MVP_HERO		5

#define MAX_CRYWOLF_SCORE_TABLE		7

#define MAX_CRYWOLF_RANK			5

enum CRYWOLF_SCORE_TYPE
{ 	
	CRYWOLF_SCORE_MONKILL_BALGARS	= 0,
	CRYWOLF_SCORE_MONKILL_DARKELF	= 1,
	CRYWOLF_SCORE_MONKILL_BALRAM	= 2,
	CRYWOLF_SCORE_MONKILL_SORAM		= 3,
	CRYWOLF_SCORE_MONKILL_DEATHSPRIT= 4,
	CRYWOLF_SCORE_MONKILL_TANKER	= 5,
	
	CRYWOLF_SCORE_LAST_CONTRACTOR	= 6,
	CRYWOLF_SCORE_MONSTERKILL		= 100,
};

enum CRYWOLF_SCORE_RANK
{
	CRYWOLF_RANK_D					= 0,
	CRYWOLF_RANK_C					= 1,
	CRYWOLF_RANK_B					= 2,
	CRYWOLF_RANK_A					= 3,
	CRYWOLF_RANK_S					= 4,
};



#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
enum CRYWOLF_BENEFIT_KUNDUNHP_REFILL_STATE
{
	CRYWOLF_BENEFIT_KUNDUN_HP_REFILL_ON		= 0,
	CRYWOLF_BENEFIT_KUNDUN_HP_REFILL_OFF	= 1,
};
#endif 



#endif // __CRYWOLFDEFINE_DEF__