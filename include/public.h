//----------------------------------------------------------------------------
// ������ Ŭ���̾�Ʈ�� �����ϴ� ��� ����
//----------------------------------------------------------------------------
#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#ifndef GAME_VERSION
#include "define.h"
#endif

//----------------------------------------------------------------------------
// ĳ���� ��ȣ
enum ClassNumber
{
	CLASS_WIZARD,
	CLASS_KNIGHT,
	CLASS_ELF,
	CLASS_MAGUMSA,	
	CLASS_DARKLORD
//#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	,CLASS_SUMMONER
//#endif
};


//#define MAX_MAGIC					20	// ����ڴ� �ִ� ���� ����
//#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116		// ������ ��ġ �� �������� �ø�
#define MAX_MAGIC					60	// ����ڴ� �ִ� ���� ����
//#endif

#define MAX_TOTALMAGIC				100	// �����ִ� ����

#define DEF_DARKLORD_LEVEL			1	// ��ũ�ε� ĳ���� ������ �⺻ ����
//----------------------------------------------------------------------------
// ���׷� ��ȣ
// -1: ����, 0: ����, 1: ��, 2:����, 3:��, 4:��, 5:�ٶ�, 6:��
#define RESISTANCE_COLD				0
#define RESISTANCE_POISON			1
#define RESISTANCE_THUNDER			2
#define RESISTANCE_FIRE				3

#define RESISTANCE_EARTH			4
#define RESISTANCE_WIND				5
#define RESISTANCE_WATER			6

//#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// MAX_SKILL ����
#define MAX_SKILL					600
//#endif


// View ����
#define STATE_NONE					0			//  ����.
#define STATE_POISON				0x00000001  //  �� �ɸ�����.
#define STATE_FREEZE				0x00000002  //  ���� �ɸ�����.
#define STATE_ATTACK				0x00000004  //  ���ݷ� ����.
#define STATE_DEFENSE				0x00000008  //  ���� ����.
#define STATE_ADD_LIFE				0x00000010	//  ����� ����.
#define STATE_HARDEN				0x00000020	//  ��ȭ.
#define STATE_REDUCE_DEFENSE		0x00000040	//  ����(���˻�)	(���� ��ų�� ���� ���)
#define STATE_REDUCE_ATTACKDAMAGE	0x00000080	//  ���ݰ���(����)


#define STATE_REDUCE_MAGICDEFENSE	0x00000100	//  ��������(����)
#define STATE_REDUCE_MAGICPOWER		0x00000200	//  ���� �ɷ� ���� (���� ��ų)
#define STATE_ADD_AG				0x00000400	//  AG���� �ӵ� ���
#define STATE_ADD_CRITICAL_DAMAGE	0x00000800	//  ũ��Ƽ�� ������ ����

//#ifdef ADD_AUTHORITY_CODE_OF_EVENT_GM_20050602
#define STATE_ADD_GM_MARK	        0x01000000	//  GM ��ũ ��Ų �߰�
//#endif

#define STATE_IMMUNE_TO_MAGIC		0x00000080	// ������� ����(�����׸�.)	(STATE_REDUCE_ATTACKDAMAGE = ����, �ҿ�ٸ���)
#define STATE_IMMUNE_TO_HARM		0x00000008	// ������� ����(�����׸�.)	(STATE_DEFENSE = ����, �������)


//#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
#define STATE_BRAND_OF_SKILL		0x00000200	// ��ũ�ε尡 �����ϴ� ��� �귣�� ^^
#define STATE_STUNED				0x00001000	// ���� �ɸ� ����.
#define STATE_ADD_MANA				0x00002000	// ���� ����.
#define STATE_CLOAKING				0x00004000	// ���� ����.
#define STATE_BLESS_POTION			0x00008000	// �ູ�� ����.
#define STATE_SOUL_POTION			0x00010000	// ��ȥ�� ����.
#define STATE_REMOVAL_MAGIC			0x00100000	// ���� ����.
//#endif

//#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
#define STATE_IMMUNE_TO_SKILL		0x00200000	// Ư�� ��ų�� IMMUNE ����
//#endif

//#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
#define STATE_INFINITY_ARROW		0x04000000	// ���Ǵ�Ƽ �ַο�(�����) ��ų
//#endif

//#ifdef CASTLE_NPC_GATE_WORK_20041206
#define STATE_CASTLE_GATE			0x00020000	// ������ ������ ����
//#endif
//#ifdef CASTLE_NPC_CROWN_WORK_20041210
#define STATE_CASTLE_CROWN			0x00020000	// �հ��� ��� �� ����
//#endif

//#ifdef MU_CASTLESIEGE_JOINSIDE_VIEWPORT_20041209
#define STATE_JOIN_SIDE_DEFEND		0x00040000	// ������ �� ��/�� ����
#define STATE_JOIN_SIDE_ATTACK		0x00080000	// ������ �� ��/�� ���� - ���� 01
//#endif

//#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
#define STATE_JOIN_SIDE_ATTACK_2	0x00400000	// ������ �� ��/�� ���� - ���� 02
#define STATE_JOIN_SIDE_ATTACK_3	0x00800000	// ������ �� ��/�� ���� - ���� 03
//#endif


//#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
#define STATE_NPC_HELP              0x02000000  // ������ ���� ���ݷ� ��� & ���� ��� ��ų ����
//#endif

//#ifdef CRYWOLF_NPC_WORK_20050912
#define STATE_BESERKER					0x00800000	// BERSERKER ����

#define STATE_CRYWOLF_NPC_HIDE			0x10000000	// ũ���̿��� ONLY - NPC HIDE ����

#define STATE_CRYWOLF_STATUE_SHIELD1	0x00000001	// ������ �Ż� ONLY - 1�ܰ� �� ���� ����
#define STATE_CRYWOLF_STATUE_SHIELD2	0x00000002	// ������ �Ż� ONLY - 2�ܰ� �� ���� ����
#define STATE_CRYWOLF_STATUE_SHIELD3	0x00000004	// ������ �Ż� ONLY - 3�ܰ� �� ���� ����
#define STATE_CRYWOLF_STATUE_SHIELD4	0x00000008	// ������ �Ż� ONLY - 4�ܰ� �� ���� ����
#define STATE_CRYWOLF_STATUE_SHIELD5	0x00000100	// ������ �Ż� ONLY - 5�ܰ� �� ���� ����

#define STATE_CRYWOLF_ALTAR_ENABLE		0x00010000	// ������ ���� ONLY - ��� ���� ����
#define STATE_CRYWOLF_ALTAR_DISABLE		0x00020000	// ������ ���� ONLY - ��� �Ҵ� ����
#define STATE_CRYWOLF_ALTAR_CONTRACTED	0x00040000	// ������ ���� ONLY - ��� �Ǿ� �ִ� ����
#define STATE_CRYWOLF_ALTAR_ATTEMPT		0x00080000	// ������ ���� ONLY - ��� �õ� ����
#define STATE_CRYWOLF_ALTAR_OCCUFIED	0x00100000	// ������ ���� ONLY - ���� ���� ����
//#endif

//#ifdef PCBANG_POINT_SYSTEM_20070206		// ���� ����
#define STATE_SEAL_ITEM					0x08000000	// PC�� ����Ʈ ���� ������ 
//#endif

//#ifdef ADD_PCS_MARK_OF_EXP_20070205
#define STATE_MARK_USE					0x08000000
//#endif // ADD_PCS_MARK_OF_EXP_20070205

//#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��� ���� ��ų
#define STATE_ILLUSION_TEMPLE_PRODECTION		0x00040000	// ��ȣ�� �ֹ�
#define STATE_ILLUSION_TEMPLE_RESTRAINT			0x00080000	// �ӹ��� �ֹ�
//#endif

//#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
#define DEL_VIEWSKILLSTATE( x, y )		( x &= ~ y )
#define ADD_VIEWSKILLSTATE( x, y )		( x |= y   )
//#endif

//ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
enum CHARACTER_AUTO_RECUPERATION_TIME
{
	CART_10_SEC         = 1000 * 10,
	CART_15_SEC         = 1000 * 15,
	CART_25_SEC         = 1000 * 25,
};
enum HP_MP_AG_VALUE_PER_TIME
{
	VPT_HP_MP_AG_1_PER_1_SEC   = 1,
	VPT_HP_MP_AG_5_PER_1_SEC   = 5,
	VPT_HP_MP_AG_10_PER_1_SEC   = 10
};
//#endif



//#ifdef DARKLORD_WORK
enum eSkillType
{
	//������
	AT_SKILL_POISON		= 1,		// �̰����� 16���� ����å�� �����Ѵ�.(�����Ҽ� ����)
	AT_SKILL_METEO		= 2,
	AT_SKILL_THUNDER	= 3,
	AT_SKILL_FIREBALL	= 4,
	AT_SKILL_FLAME		= 5,
	AT_SKILL_TELEPORT   = 6,
	AT_SKILL_SLOW		= 7,
	AT_SKILL_STORM		= 8,
	AT_SKILL_EVIL		= 9,
	AT_SKILL_HELL		= 10,
	AT_SKILL_POWERWAVE  = 11,
	AT_SKILL_FLASH      = 12,
	AT_SKILL_BLAST      = 13,
	AT_SKILL_INFERNO    = 14,

	AT_SKILL_TARGET_TELEPORT		= 15,	// ��ȯ �̵�
	AT_SKILL_MAGICDEFENSE= 16,				// �ҿ�ٸ���

	AT_SKILL_ENERGYBALL = 17,	
	
	//���
	AT_SKILL_BLOCKING   = 18,//���
	AT_SKILL_SWORD1     = 19,//�������
	AT_SKILL_SWORD2     = 20,//���
	AT_SKILL_SWORD3     = 21,//�÷�ġ��
	AT_SKILL_SWORD4     = 22,//����ġ��
	AT_SKILL_SWORD5     = 23,//����

	//����
	AT_SKILL_CROSSBOW   = 24,//Ȱ��ų     (�ٹ�)

	AT_SKILL_BOW		= 25,//����(?)

	AT_SKILL_HEALING    = 26,//ġ��       (������/5+(�����۷���+1)*5)                   //�̰����ʹ� ���������� �����Ѵ�.
	AT_SKILL_DEFENSE    = 27,//�������� (��30�ʰ� ������/8+(�ƾ��۷���+1)*2��ŭ ����)
	AT_SKILL_ATTACK     = 28,//���ݷ����� (��30�ʰ� ������/7+(�ƾ��۷���+1)*3��ŭ ����)
	

	AT_SKILL_CALLMON1   = 30,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
	AT_SKILL_CALLMON2   = 31,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
	AT_SKILL_CALLMON3   = 32,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
	AT_SKILL_CALLMON4   = 33,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
	AT_SKILL_CALLMON5   = 34,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
	AT_SKILL_CALLMON6   = 35,//�߸�       (�ƾ��۷����� ���� ����ȯ)
	AT_SKILL_CALLMON7   = 36,//����       (�ƾ��۷����� ���� ����ȯ)

	AT_SKILL_WHEEL			= 41,//ȸ��������(�������)
	AT_SKILL_BLOWOFFURY		= 42,//�˿÷����(�г����ϰ�)(�������)
	AT_SKILL_STRIKE			= 43,//��ο�(���_���̵峪��Ʈ����)
	AT_SKILL_KNIGHTSPEAR	= 47,//â���(���ϸ� Ÿ�� �־�� �ȴ�)(�������)
	AT_SKILL_KNIGHTADDLIFE	= 48,//����������(�������)	
	AT_SKILL_KNIGHTDINORANT	= 49,//���̵� ��:����Ʈ ��ų(�������)

	AT_SKILL_ELFHARDEN		= 51,	//���̽��ַο�(����)
	AT_SKILL_PENETRATION	= 52,	//����(����)

	AT_SKILL_DEFENSEDOWN	= 55,	//�������(���˻�)

	AT_SKILL_SWORD6			= 56,	//�׽�Ʈ ��ų -> ������ε� 


	//AT_SKILL_4CROSSBOW		= 53,	//���� 4�ٹ�
	AT_SKILL_5CROSSBOW		= 54,	//���� 5�ٹ�

	AT_SKILL_EXPPOISON		= 38,	//Ȯ�嵶
	AT_SKILL_EXPICE			= 39,	//Ȯ����̽�
	
	AT_SKILL_EXPHELL		= 40,	//Ȯ�������̾� ����
	AT_SKILL_EXPHELL_START	= 58,	//Ȯ�������̾� ����
	
	AT_SKILL_IMPROVE_AG_REFILL	= 53,	//�ο��� ��ų (AG���� �ӵ� ���)

	//����
	AT_SKILL_DEVILFIRE		= 50,
	
	AT_SKILL_COMBO			= 59,	// �޺� ��ų

	AT_SKILL_SPEAR	= 60,			// ���Ǿ�
	AT_SKILL_FIREBURST,				// ���̾����Ʈ
	AT_SKILL_DARKHORSE_ATTACK,		// ��ũȣ�� ����
	AT_SKILL_RECALL_PARTY,			// ��Ƽ��ȯ
	AT_SKILL_ADD_CRITICALDAMAGE,	// ũ��Ƽ�� ������ ����
	AT_SKILL_ELECTRICSPARK,			// �Ϸ�Ʈ�� ����ũ

	AT_SKILL_LONGSPEAR,				// �ս��Ǿ�


//#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	AT_SKILL_RUSH				= 44,	// ���̵� ����Ʈ
	AT_SKILL_JAVALIN			= 45,	// �ҿ� ������
	AT_SKILL_DEEPIMPACT			= 46,	// ���� ����
	AT_SKILL_ONE_FLASH			= 57,	// ����� ���˻�
	AT_SKILL_DEATH_CANNON		= 73,	// ������ ���˻�
	AT_SKILL_SPACE_SPLIT		= 74,	// ��ũ�ε� ���� ��ų
	
	AT_SKILL_BRAND_OF_SKILL		= 75,	// ��ũ�ε� ���� ��ų
	AT_SKILL_STUN				= 67,	// ����
	AT_SKILL_REMOVAL_STUN		= 68,	// ���� ����
	AT_SKILL_ADD_MANA			= 69,	// ���� ����
	AT_SKILL_INVISIBLE			= 70,	// ����
	AT_SKILL_REMOVAL_INVISIBLE	= 71,	// ���� ����
	AT_SKILL_REMOVAL_MAGIC		= 72,	// ���� ����
//endif	

//#ifdef ADD_ITEM_FENRIR_01_20051110
	// �渱 ��ų
	AT_SKILL_FENRIR_ATTACK		= 76,	// �ö������

//#endif
	
//#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	AT_SKILL_INFINITY_ARROW	= 77,	// ���Ǵ�Ƽ �ַο� (�����)
//#endif

//#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���̾� ��ũ�� �ε��� ����
	AT_SKILL_FIRESCREAM			= 78,	// ���̾� ��ũ�� (��ũ�ε�)
	AT_SKILL_EXPLOSION			= 79,	// ����( ��ũ�ε� - ���̾ũ���� ���� ���� ������ ó���� ���δ�.)
//#endif

//#ifdef ADD_NEW_MAP_KALIMA_20040518	// Į������ �߰��� ���� ��ų��
	AT_SKILL_SUMMON				= 200,	// ���� ���� ��ȯ
	AT_SKILL_IMMUNE_TO_MAGIC	= 201,	// ������� ����
	AT_SKILL_IMMUNE_TO_HARM		= 202,	// ������� ����
//#endif

//#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	//  ȯ�� ��� ���뽺ų
	AT_SKILL_PRODECTION			= 210,	// ��ȣ�� �ֹ�
	AT_SKILL_RESTRAINT			= 211,	// �ӹ��� �ֹ�
	AT_SKILL_CALL				= 212,	// ��ȯ�� �ֹ�
	AT_SKILL_SHIELD_BURN		= 213,	// ���� ��
//#endif

//#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű� ĳ���� ��ų
	// ����
	AT_SKILL_DRAINLIFE			= 214,	// �巹�ζ�����
	AT_SKILL_CHAINLIGHTNING		= 215,	// ü�� ����Ʈ��
	AT_SKILL_LIGHTNINGORB		= 216,	// ����Ʈ�׿���

	AT_SKILL_THORNS				= 217,	// ����(�������ݻ�)
	
//#ifdef SEASON4_PATCH_GROOVING
	// --->
	// ���� 4.0 �߰� ��ų  grooving 2008/04/24	
	AT_SKILL_BERSERKER			= 218,	// ����Ŀ
	// <---
//#endif
	
	AT_SKILL_SLEEP				= 219,	// ����
	AT_SKILL_BLIND				= 220,	// ����ε�

//#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111	// �űԽ�ų ��ũ�Ͻ� �̳ʺ��̼� ����
	AT_SKILL_WEAKNESS			= 221,	// ��ũ�Ͻ�
	AT_SKILL_ENERVATION			= 222,	// �̳ʺ��̼�
//#endif

	// ȯ�� ��ų
	AT_SKILL_EXPLOSION_2		= 223,	// �ͽ��÷���
	AT_SKILL_REQUIEM			= 224,	// ������
//#endif

//#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425	// ȯ�� �űԽ�ų �޷��
	AT_SKILL_PULLUTION			= 225,	// �޷��
//#endif

//#ifdef SEASON4_PATCH_GROOVING
	// --->
	// ���� 4.0 �߰� ��ų  grooving 2008/04/24
	AT_SKILL_LIGHTING_SHOCK		= 230,	// ����Ʈ�� ��ũ 
	//<-----
//#endif

//#ifdef ADD_SEASON4_NEW_SKILL_20080522	// ����4 �ű� ��ų
	AT_SKILL_DESTRUCTION		= 232,	// �ı��� �ϰ�
	AT_SKILL_MAGICPOWRER		= 233,	// ��������
	AT_SKILL_RECOVERY			= 234,	// ȸ��
	AT_SKILL_MULTISHOT			= 235,	// ��Ƽ��
	AT_SKILL_FLAMESTRIKE		= 236,	// �÷��� ��Ʈ����ũ
	AT_SKILL_GIGANTICSTORM		= 237,	// �Ⱓƽ ����
	AT_SKILL_CHAOTICDESAIR		= 238,	// ī��ƽ ���̾�
//#endif

//#ifdef ADD_RAKLION_20080408		// ��Ŭ���� ���� ������ ��ų
AT_SKILL_SELUPAN_POISON_ATTACK	= 250,	// ��Ŭ���� ���� ������ ������
AT_SKILL_SELUPAN_ICE_STORM		= 251,	// ��Ŭ���� ���� ������ �ñ� ��ǳ
AT_SKILL_SELUPAN_ICE_STRIKE		= 252,	// ��Ŭ���� ���� ������ �ñ� ���
AT_SKILL_SELUPAN_FIRST_SKILL	= 253,	// ��Ŭ���� ���� ������ ����(���� ��ų)
//#endif // ADD_RAKLION_20080408

//#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122		// �����ͷ��� ��ų 1�� - �нú� ��ų ��ȣ ����
	AT_SKILL_ML_ATTACK_SUCCESS_RATE		= 300,	// ���ݼ����� ���
	AT_SKILL_ML_PVP_ATTACKDAMAGE_RATE	= 305,	// ���ΰ��ݷ� ���
	AT_SKILL_ML_PVP_BLOCKING_RATE		= 310,	// ���ι���� ����
	AT_SKILL_ML_DOWN_DUR1				= 315,	// ���� ����1
	AT_SKILL_ML_DOWN_DUR2				= 320,	// ���� ����2
	AT_SKILL_ML_RESISTANCE_POISON		= 325,	// �� ���׷� ���
	AT_SKILL_ML_RESISTANCE_THUNDER		= 330,	// ���� ���׷� ���
	AT_SKILL_ML_RESISTANCE_ICE			= 335,	// ���� ���׷� ���
	AT_SKILL_ML_AUTO_RECOVER_LIFE		= 340,	// ����� �ڵ�ȸ�� ��ȭ
	AT_SKILL_ML_PLUS_ZEN				= 345,	// �� ����
//#endif

//#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	// �����ͷ��� ��ų 2�� - ��ų �߰�
	AT_SKILL_ML_DEFENCE					= 350,	// ���� ����
	AT_SKILL_ML_MAX_HP					= 355,	// �ִ�HP ����
	AT_SKILL_ML_MAX_AG					= 360,	// �ִ�AG ����
	AT_SKILL_ML_MONSTER_KILL_MANA		= 365,	// ���� ų �� ����ȸ��
	AT_SKILL_ML_MONSTER_KILL_HP			= 370,	// ���� ų �� �����ȸ��
	AT_SKILL_ML_MONSTER_KILL_SD			= 375,	// ���� ų �� SDȸ��
	AT_SKILL_ML_PLUS_EXP				= 380,	// ����ġ ���
//#endif

//#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �����ͷ��� ��ų 4��ũ �нú꽺ų
	AT_SKILL_ML_MAX_SD					= 385,	// ĳ���� �ִ� SD����
	AT_SKILL_ML_SD_SPEED				= 390,	// SD ȸ�� �ӵ� ���
	AT_SKILL_ML_MAX_ATTACK_DAMAGE		= 395,	// �ִ� ���ݷ� ���( ����, ����, ��ũ�ε�)
	AT_SKILL_ML_MIN_ATTACK_DAMAGE		= 400,	// �ּ� ���ݷ� ���( ����, ����, ��ũ�ε�)
	AT_SKILL_ML_DECREASE_MANA			= 405,	// ���� ���� ��ȭ
	AT_SKILL_ML_MAX_MAGIC_DAMAGE		= 410,	// �ִ� ���� ���(�渶����)
	AT_SKILL_ML_MIN_MAGIC_DAMAGE		= 415,	// �ּ� ���� ���(�渶����)
	AT_SKILL_ML_PET_DUR_DOWN_SPEED		= 420,	// �� ���� �Ҹ� �ӵ� ����
	AT_SKILL_ML_MAX_ATTACK_MAGIC_DAMAGE	= 425,	// �ִ� ���ݷ�/���� ���(���˻�)
	AT_SKILL_ML_MIN_ATTACK_MAGIC_DAMAGE	= 430,	// �ּ� ���ݷ�/���� ���(���˻�)
//#endif

//#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��Ƽ�� ��ų ��ȣ �߰�
	AT_SKILL_ML_STRENGTHEN_MAGICDEFENSE			= 435,	// �ҿ�ٸ��� ��ȭ
	AT_SKILL_ML_STRENGTHEN_HELL					= 440,	// �����̾� ��ȭ
	AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT			= 455,	// ȸ�������� ��ȭ1 (���)
	AT_SKILL_ML_STRENGTHEN_HEALING				= 475,	// ġ�� ��ȭ
	AT_SKILL_ML_STRENGTHEN_CROSSBOW				= 490,	// �ٹ� ��ȭ
	AT_SKILL_ML_STRENGTHEN_WHEEL_MAGUMSA		= 495,	// ȸ�������� ��ȭ2 (���˻�)
	AT_SKILL_ML_STRENGTHEN_KNIGHTADDLIFE		= 470,	// ���������� ��ȭ	
	AT_SKILL_ML_STRENGTHEN_BLAST				= 510,	// ����Ʈ ��ȭ
	AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK		= 515,	// �����ũ ��ȭ
	AT_SKILL_ML_STRENGTHEN_FIREBURST			= 520,	// ���̾����Ʈ ��ȭ
//#endif

//#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �����ͷ��� ��ų 4��ũ ��Ƽ�꽺ų
	AT_SKILL_ML_STRENGTHEN_EVIL					= 445,	// �Ƿ� ��ȭ
	AT_SKILL_ML_STRENGTHEN_EXPICE				= 450,	// ���� ���̽� ��ȭ
	AT_SKILL_ML_STRENGTHEN_STRIKE				= 460,	// ��ο� ��ȭ(���)
	AT_SKILL_ML_STRENGTHEN_BLOWOFFURY			= 465,	// �г��� �ϰ� ��ȭ(���)
	AT_SKILL_ML_STRENGTHEN_DEFENSE				= 480,	// ���� ���
	AT_SKILL_ML_STRENGTHEN_ATTACK				= 485,	// ���ݷ� ���	
	AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN			= 500,	// ���� ���� ��ȭ
	AT_SKILL_ML_STRENGTHEN_SWORD6				= 505,	// �Ŀ� ������ ��ȭ
	AT_SKILL_ML_STRENGTHEN_FIRESCREAM			= 525,	// ���̾� ��ũ�� ��ȭ
// #endif

//#ifdef UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
	AT_SKILL_ML_STRENGTHEN_MAGUMSA_EVIL			= 530,	// �Ƿ� ��ȭ(���˻�)
//#endif	
};

enum eOptionType
{
	AT_IMPROVE_DAMAGE       = 80,//�߰� ���ݷ�           (Level*4)
	AT_IMPROVE_MAGIC        = 81,//�߰� ����             (Level*4)
	AT_IMPROVE_BLOCKING     = 82,//�߰� �����           (Level*5)
	AT_IMPROVE_DEFENSE      = 83,//�߰� ����           (Level*4)
	AT_LUCK                 = 84,//��ȥ�� ���� ���� Ȯ�� +25%
    AT_LIFE_REGENERATION    = 85,//���� �ڵ� ȸ��        (Level*1%)

	AT_IMPROVE_LIFE             = 86,//����� ����
	AT_IMPROVE_MANA             = 87,//���� ����
	AT_DECREASE_DAMAGE          = 88,//������ ����
	AT_REFLECTION_DAMAGE        = 89,//������ �ݻ�
	AT_IMPROVE_BLOCKING_PERCENT = 90,//������
	AT_IMPROVE_GAIN_GOLD        = 91,//ȹ�� �� ����
	AT_EXCELLENT_DAMAGE         = 92,//�׼���Ʈ ������ ����
	AT_IMPROVE_DAMAGE_LEVEL     = 93,//���ݷ� ����(����)
	AT_IMPROVE_DAMAGE_PERCENT   = 94,//���ݷ� ����(�ۼ�Ʈ)
	AT_IMPROVE_MAGIC_LEVEL      = 95,//���� ����(����)
	AT_IMPROVE_MAGIC_PERCENT    = 96,//���� ����(�ۼ�Ʈ)
	AT_IMPROVE_ATTACK_SPEED     = 97,//���� �ӵ�
	AT_IMPROVE_GAIN_LIFE        = 98,//ȹ�� �����
	AT_IMPROVE_GAIN_MANA        = 99,//ȹ�� ����


	//  ���� �߰� �ɼ�.        

	AT_PLUS_WING_MAXLIFE		= 100,		// �ִ� HP+50
	AT_PLUS_WING_MAXMANA		= 101,		// �ִ� MP+50
	AT_PLUS_ONE_PERCENT_DAMAGE  = 102,   //  1% Ȯ���� ���� ���� ���� ����.
	AT_IMPROVE_AG_MAX           = 103,   //  �ִ� AG +50 ����.
    AT_DAMAGE_ABSORB            = 104,   //  5% �߰� ������ ���.
	AT_PLUS_WING_ADD_LEADERSHIP	= 105,		// �߰� ���

//#ifdef ADD_ITEM_FENRIR_01_20051110
//	�渱 �߰� �ɼ�.
	AT_PLUS_FENRIR_INCREASE_LAST_DAMAGE = 106,
	AT_PLUS_FENRIR_DECREASE_LAST_DAMAGE = 107,
//#endif

//#ifdef ADD_THIRD_WING_20070525	// 3������ �߰� �ɼ�
	AT_5PER_IGNORE_DEFENSE		= 108,	// (������ ������ ���) %5 Ȯ���� �� ���� ����
	AT_5PER_ATTACK_TURNING		= 109,	// (���ݴ��� ���) 5% Ȯ���� �� ���ݷ� 50% ������
	AT_RECOVER_100PER_LIFE		= 110,	// (���ݴ��� ���) 5% Ȯ���� ���� ���� 100% ���� ȸ��
	AT_RECOVER_100PER_MASIC		= 111,	// (���ݴ��� ���) 5% Ȯ���� ���� ���� 100% ���� ȸ��
//#endif
//#ifdef ADD_ILLUSION_FENRIR_20070703	// ȯ�� �渱 �߰�
	AT_PLUS_ILLUSION_FENRIR_OPTION	= 112,	// HP ����, MP����, ���ݷ�����, ���� ����
//#endif
//#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ���� �߰� �ɼ� 
	AT_IMPROVE_CURSE	= 113,
//#endif
	AT_ATTACK1          = 120,
	AT_ATTACK2,

	AT_STAND1,
	AT_STAND2,
	AT_MOVE1,
	AT_MOVE2,

	AT_DAMAGE1,
	AT_DIE1,
	AT_SIT1,
	AT_POSE1,
	AT_HEALING1,
	AT_GREETING1,
	AT_GOODBYE1,
	AT_CLAP1,
	AT_GESTURE1,
	AT_DIRECTION1,
	AT_UNKNOWN1,
	AT_CRY1,
	AT_CHEER1,
	AT_AWKWARD1,
	AT_SEE1,
	AT_WIN1,
    AT_SMILE1,
	AT_SLEEP1,
	AT_COLD1,
	AT_AGAIN1,
	AT_RESPECT1,
	AT_SALUTE1,
	AT_RUSH1,	

	AT_SET_RING_OPTION_ADD_SKILLDAMAGE,					// ��Ʈ �߰� ������(����, ����̷�)

	AT_SET_OPTION_IMPROVE_STRENGTH = 160,				// �� ����
	AT_SET_OPTION_IMPROVE_DEXTERITY,					// ��ø�� ����
	AT_SET_OPTION_IMPROVE_ENERGY,						// ������ ����
	AT_SET_OPTION_IMPROVE_VITALITY,						// ü�� ����
	AT_SET_OPTION_IMPROVE_LEADERSHIP,						// ��� ����
	AT_SET_OPTION_IMPROVE_MINATTACKDAMAGE,				// "�ּ� ���ݷ� ����"
	AT_SET_OPTION_IMPROVE_MAXATTACKDAMAGE,				// "�ִ� ���ݷ� ����"
	AT_SET_OPTION_IMPROVE_MAGICDAMAGE,					// "���� ����"
	AT_SET_OPTION_IMPROVE_DAMAGE,						// "������ ����"
	AT_SET_OPTION_IMPROVE_ATTACKRATE,					// "������ ����"
	AT_SET_OPTION_IMPROVE_DEFENCE,						// "���� ����"	
	AT_SET_OPTION_IMPROVE_MAXLIFE,							// "���� ����"
	AT_SET_OPTION_IMPROVE_MAXMANA,						// "���� ����"
	AT_SET_OPTION_IMPROVE_MAXAG,						// �ִ� AG����
	AT_SET_OPTION_IMPROVE_AG_VALUE,						// AG������ ����
	AT_SET_OPTION_IMPROVE_CRITICALDAMAGE_SUCCESS,		// "ũ��Ƽ�� ������ Ȯ�� ����"
	AT_SET_OPTION_IMPROVE_CRITICALDAMAGE,				// "ũ��Ƽ�� ������ ����"
	AT_SET_OPTION_IMPROVE_EX_DAMAGE_SUCCESS,			// "������Ʈ ������ Ȯ�� ����"
	AT_SET_OPTION_IMPROVE_EX_DAMAGE,					// "������Ʈ ������ ����"
	AT_SET_OPTION_IMPROVE_SKILLDAMAGE,					// "��ų ���ݷ� ����"
	AT_SET_OPTION_DOUBLE_DAMAGE,						// "�������� Ȯ��"	
	AT_SET_OPTION_DEFENCE_IGNORE,						// "�� ���� ����"
	AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE,				//	���� ���� ���� 5% ����          (��)
	AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE,		//	��հ� ���� ������ 15% ����       ( ������ )	
	AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_STR,		// "���ݷ� ���� ��/x"
	AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_DEX,		// "���ݷ� ���� ��ø/x"
	AT_SET_OPTION_IMPROVE_DEFENCE_WITH_DEX,				// "���� ���� ��ø��/x"
	AT_SET_OPTION_IMPROVE_DEFENCE_WITH_VIT,				// "���� ���� ü��/x"	
	AT_SET_OPTION_IMPROVE_MAGICDAMAGE_WITH_ENERGY,		// "���� ���� ������/x"
	AT_SET_OPTION_ICE_MASTERY,							//	Ư�� ĳ���� �����Ӽ� ��ų �߰� ������
	AT_SET_OPTION_POSION_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ������
	AT_SET_OPTION_THUNDER_MASTERY,						//	Ư�� ĳ���� �����Ӽ� ��ų �߰� ������
	AT_SET_OPTION_FIRE_MASTERY,							//	Ư�� ĳ���� �ҼӼ� ��ų �߰� ������
	AT_SET_OPTION_EARTH_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ������
	AT_SET_OPTION_WIND_MASTERY,							//	Ư�� ĳ���� �ٶ��Ӽ� ��ų �߰� ������
	AT_SET_OPTION_WATER_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ������

	AT_SET_IMPROVE_STRENGTH,							// ��Ʈ ������ �ɼ� (������ : *3)	: �����
	AT_SET_IMPROVE_DEXTERITY,							// ��Ʈ ������ �ɼ� (��ø���� : *3)	: ����̷� 
	AT_SET_IMPROVE_ENERGY,								// ��Ʈ ������ �ɼ� (���������� : *3) : ������
	AT_SET_IMPROVE_VITALITY,							// ��Ʈ ������ �ɼ� (ü������ : *3) : ����
	
//
//
//	AT_SET_OPTION_DOUBLE_DAMAGE					= 140,  //	���� ������ Ȯ�� 2%                 ( ������ )
//	AT_SET_OPTION_DEFENCE_IGNORE,						//	�� ���� ���� 2%                   ( ������ )
//	AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE,		//	��հ� ���� ������ 15% ����       ( ������ )	
//	AT_SET_OPTION_IMPROVE_ATTACKING_PERCENT,			//	���� ������ ���� 5%                 (��)
//	AT_SET_OPTION_REFLECTION_DAMAGE,					//	100% ������ �ݻ� Ȯ�� 5% ����       ( ������ ó�� )
//	AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE,				//	���� ���� ���� 5% ����          (��)
//	AT_SET_OPTION_IMPROVE_STRENGTH_1,					//	�� ���� +10                         (��)	                
//	AT_SET_OPTION_IMPROVE_STRENGTH_2,					//	�� ���� +20                         (��)		                
//	AT_SET_OPTION_IMPROVE_DEXTERITY_1,					//	��ø�� ���� +10                     (��)		            
//	AT_SET_OPTION_IMPROVE_DEXTERITY_2,					//	��ø�� ���� +20                     (��)	                
//	AT_SET_OPTION_IMPROVE_ENERGY_1,						//	������ ���� +10	                    (��)	                
//	AT_SET_OPTION_IMPROVE_ENERGY_2,						//	������ ���� +20	                    (��)	                
//	AT_SET_OPTION_IMPROVE_VITALITY_1,					//	ü�� ���� +10	                    (��)	                
//	AT_SET_OPTION_IMPROVE_VITALITY_2,					//	ü�� ���� +20	                    (��)	                
//	AT_SET_OPTION_IMPROVE_MAX_LIFE_1,					//	�ִ� ����� ���� +40	            (�������� ������ �޴´�.)
//	AT_SET_OPTION_IMPROVE_MAX_LIFE_2,					//	�ִ� ����� ���� +80            	(�������� ������ �޴´�.)
//	AT_SET_OPTION_IMPROVE_MAX_MANA_1,					//	�ִ� ���� ���� +40	               	(�������� ������ �޴´�.)
//	AT_SET_OPTION_IMPROVE_MAX_MANA_2,					//	�ִ� ���� ���� +80          		(�������� ������ �޴´�.)
//	AT_SET_OPTION_IMPROVE_MAX_AG,						//	�ִ� AG 5% ����		            	(�������� ������ �޴´�.)
//	AT_SET_OPTION_DECREASE_SPELL_AG,					//	�Һ� AG 10% ����	                (��)
//	AT_SET_OPTION_IMPROVE_ITEM_DROP,					//	������ ��� Ȯ�� 5% ����		
//
//	AT_SET_OPTION_IMPROVE_ATTACK_1,						//	Ư�� ĳ���� ���ݷ� ���� ��ø��/60	(��)
//	AT_SET_OPTION_IMPROVE_ATTACK_2,						//	Ư�� ĳ���� ���ݷ� ���� ��/50   	(��)
//	AT_SET_OPTION_IMPROVE_MAGIC,						//	Ư�� ĳ���� ���� ���� ������/70	    (��)
//	AT_SET_OPTION_IMPROVE_DEFENCE_1,					//	Ư�� ĳ���� ���� ���� ����/20	    (��)
//	AT_SET_OPTION_IMPROVE_DEFENCE_2,					//	Ư�� ĳ���� ���� ���� 3%	        (��)
//	AT_SET_OPTION_IMPROVE_DEFENCE_3,					//	Ư�� ĳ���� ���� ���� ��ø��/50	(��)
//	AT_SET_OPTION_IMPROVE_DEFENCE_4,					//	Ư�� ĳ���� ���� ���� ü��/40	    (��)
//
//    AT_SET_OPTION_FIRE_MASTERY,							//	Ư�� ĳ���� �ҼӼ� ��ų �߰� ���ݷ� 10%		        
//	AT_SET_OPTION_ICE_MASTERY,							//	Ư�� ĳ���� �����Ӽ� ��ų �߰� ���ݷ� 10%			    
//	AT_SET_OPTION_THUNDER_MASTERY,						//	Ư�� ĳ���� �����Ӽ� ��ų �߰� ���ݷ� 10%			    
//	AT_SET_OPTION_POSION_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ���ݷ� 10%			        
//	AT_SET_OPTION_WATER_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ���ݷ� 10%			        
//	AT_SET_OPTION_WIND_MASTERY,							//	Ư�� ĳ���� �ٶ��Ӽ� ��ų �߰� ���ݷ� 10%			    
//	AT_SET_OPTION_EARTH_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ���ݷ� 10%
//	

//
//	AT_IMPROVE_OPTION_MAX_MANA,				// �ִ� ���� �ɼ�
//	AT_IMPROVE_OPTION_MAX_AG,				// �ִ� AG����
};

//#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
enum eJewelOfHarmonyItemOption
{
	// ����
	AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM				 = 0,	// ȿ�� ���� : ��ȭ���� ���� ������

	// ����� �ɼ�
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE	 = 1,	// �ּ� ���ݷ� ��� (��)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE	 = 2,	// �ִ� ���ݷ� ��� (��)
	AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR		 = 3,	// �ʿ� �� ���� (��)
	AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX		 = 4,	// �ʿ� ��ø ���� (��)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKDAMAGE_BOTH	 = 5,	// ���ݷ� ��� (�ּ�,�ִ�) (��)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_CRITICALDAMAGE		 = 6,	// ũ��Ƽ�� ������ ��� (��)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_SKILLDAMAGE		 = 7,	// ��ų ���ݷ� ��� (��)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKSUCCESRATE_PVP= 8,	// ���� ������ ��� (PVP)  (��)
	AT_JEWELOFHARMONY_WEAPON_DECREASE_SD_RATE			 = 9,	// SD ������ ��� (�ǵ� ������ ����� ����. ���� HP�� �ִ� �������� ����)  (��)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_IGNORE_SD_RATE		 = 10,  // ���ݽ� SD ���� Ȯ�� ��� : SD�� �����ϰ� ���� HP�� �������� ���Ѵ�.  (��)

	// �����̷�
	AT_JEWELOFHARMONY_STAFF_IMPROVE_MAGICDAMAGE			 = 1,	// ���� ���(��)
	AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR		 = 2,	// �ʿ� �� ���� (��)
	AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX		 = 3,	// �ʿ� ��ø ���� (��)
	AT_JEWELOFHARMONY_STAFF_IMPROVE_SKILLDAMAGE			 = 4,	// ��ų ���ݷ� ��� (��)
	AT_JEWELOFHARMONY_STAFF_IMPROVE_CRITICALDAMAGE		 = 5,	// ũ��Ƽ�� ������ ��� (��)
	AT_JEWELOFHARMONY_STAFF_DECREASE_SD_RATE			 = 6,	// SD ������ ��� (�ǵ� ������ ����� ����. ���� HP�� �ִ� �������� ����)  (��)//#endif
	AT_JEWELOFHARMONY_STAFF_IMPROVE_ATTACKSUCCESRATE_PVP = 7,	// ���� ������ ��� (PVP)  (��)
	AT_JEWELOFHARMONY_STAFF_IMPROVE_IGNORE_SD_RATE		 = 8,   // ���ݽ� SD ���� Ȯ�� ��� : SD�� �����ϰ� ���� HP�� �������� ���Ѵ�.  (��)

	// ����
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DEFENSE			  = 1,   // ���� ��� (��)
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_AG			  = 2,	// �ִ� AG ��� (��)
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_HP			  = 3,	// �ִ� HP ��� (��)
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_HP	  = 4,	// ����� �ڵ� ������ ���
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_MP	  = 5,	// ���� �ڵ� ������ ���
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_BLOCKSUCCESRATE_PVP = 6,	// ��� ������ ��� (PVP) ��
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DAMAGE_DECREASE	  = 7,	// ������ ���� % ���
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_SD_RATE			  = 8   // SD ���� ��� ( PVP�ÿ� SD ������ ������ )
};

//#else
//----------------------------------------------------------------------------
// ĳ������ �ൿ ��ȣ
//enum ActionType
//{
//	//������
//	AT_SKILL_POISON		= 1,		// �̰����� 16���� ����å�� �����Ѵ�.(�����Ҽ� ����)
//	AT_SKILL_METEO		= 2,
//	AT_SKILL_THUNDER	= 3,
//	AT_SKILL_FIREBALL	= 4,
//	AT_SKILL_FLAME		= 5,
//	AT_SKILL_TELEPORT   = 6,
//	AT_SKILL_SLOW		= 7,
//	AT_SKILL_STORM		= 8,
//	AT_SKILL_EVIL		= 9,
//	AT_SKILL_HELL		= 10,
//	AT_SKILL_POWERWAVE  = 11,
//	AT_SKILL_FLASH      = 12,
//	AT_SKILL_BLAST      = 13,
//	AT_SKILL_INFERNO    = 14,
//
//	AT_SKILL_TARGET_TELEPORT		= 15,	// ��ȯ �̵�
//	AT_SKILL_MAGICDEFENSE= 16,				// �ҿ�ٸ���
//
//	AT_SKILL_ENERGYBALL = 17,	
//	
//	//���
//	AT_SKILL_BLOCKING   = 18,//���
//	AT_SKILL_SWORD1     = 19,//�������
//	AT_SKILL_SWORD2     = 20,//���
//	AT_SKILL_SWORD3     = 21,//�÷�ġ��
//	AT_SKILL_SWORD4     = 22,//����ġ��
//	AT_SKILL_SWORD5     = 23,//����
//
//	//����
//	AT_SKILL_CROSSBOW   = 24,//Ȱ��ų     (�ٹ�)
//
//	AT_SKILL_BOW		= 25,//����(?)
//
//	AT_SKILL_HEALING    = 26,//ġ��       (������/5+(�����۷���+1)*5)                   //�̰����ʹ� ���������� �����Ѵ�.
//	AT_SKILL_DEFENSE    = 27,//�������� (��30�ʰ� ������/8+(�ƾ��۷���+1)*2��ŭ ����)
//	AT_SKILL_ATTACK     = 28,//���ݷ����� (��30�ʰ� ������/7+(�ƾ��۷���+1)*3��ŭ ����)
//	
//
//	AT_SKILL_CALLMON1   = 30,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
//	AT_SKILL_CALLMON2   = 31,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
//	AT_SKILL_CALLMON3   = 32,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
//	AT_SKILL_CALLMON4   = 33,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
//	AT_SKILL_CALLMON5   = 34,//��ȯ       (�ƾ��۷����� ���� ����ȯ)
//	AT_SKILL_CALLMON6   = 35,//�߸�       (�ƾ��۷����� ���� ����ȯ)
//	AT_SKILL_CALLMON7   = 36,//����       (�ƾ��۷����� ���� ����ȯ)
//
//	AT_SKILL_WHEEL			= 41,//ȸ��������(�������)
//	AT_SKILL_BLOWOFFURY		= 42,//�˿÷����(�г����ϰ�)(�������)
//	AT_SKILL_STRIKE			= 43,//��ο�(���_���̵峪��Ʈ����)
//	AT_SKILL_KNIGHTSPEAR	= 47,//â���(���ϸ� Ÿ�� �־�� �ȴ�)(�������)
//	AT_SKILL_KNIGHTADDLIFE	= 48,//����������(�������)	
//	AT_SKILL_KNIGHTDINORANT	= 49,//���̵� ��:����Ʈ ��ų(�������)
//
//	AT_SKILL_ELFHARDEN		= 51,	//���̽��ַο�(����)
//	AT_SKILL_PENETRATION	= 52,	//����(����)
//
//	AT_SKILL_DEFENSEDOWN	= 55,	//�������(���˻�)
//
//	AT_SKILL_SWORD6			= 56,	//�׽�Ʈ ��ų -> ������ε� 
//
//
//	//AT_SKILL_4CROSSBOW		= 53,	//���� 4�ٹ�
//	AT_SKILL_5CROSSBOW		= 54,	//���� 5�ٹ�
//
//	AT_SKILL_EXPPOISON		= 38,	//Ȯ�嵶
//	AT_SKILL_EXPICE			= 39,	//Ȯ����̽�
//	
//	AT_SKILL_EXPHELL		= 40,	//Ȯ�������̾� ����
//	AT_SKILL_EXPHELL_START	= 58,	//Ȯ�������̾� ����
//	
//	AT_SKILL_IMPROVE_AG_REFILL	= 53,	//�ο��� ��ų (AG���� �ӵ� ���)
//
//	//����
//	AT_SKILL_DEVILFIRE		= 50,
//	
//	AT_SKILL_COMBO			= 59,	// �޺� ��ų
//	
//
//	AT_IMPROVE_DAMAGE       = 60,//�߰� ���ݷ�           (Level*4)
//	AT_IMPROVE_MAGIC        = 61,//�߰� ����             (Level*4)
//	AT_IMPROVE_BLOCKING     = 62,//�߰� �����           (Level*5)
//	AT_IMPROVE_DEFENSE      = 63,//�߰� ����           (Level*4)
//	AT_LUCK                 = 64,//��ȥ�� ���� ���� Ȯ�� +25%
//    AT_LIFE_REGENERATION    = 65,//���� �ڵ� ȸ��        (Level*1%)
//
//	AT_IMPROVE_LIFE             = 66,//����� ����
//	AT_IMPROVE_MANA             = 67,//���� ����
//	AT_DECREASE_DAMAGE          = 68,//������ ����
//	AT_REFLECTION_DAMAGE        = 69,//������ �ݻ�
//	AT_IMPROVE_BLOCKING_PERCENT = 70,//������
//	AT_IMPROVE_GAIN_GOLD        = 71,//ȹ�� �� ����
//	AT_EXCELLENT_DAMAGE         = 72,//�׼���Ʈ ������ ����
//	AT_IMPROVE_DAMAGE_LEVEL     = 73,//���ݷ� ����(����)
//	AT_IMPROVE_DAMAGE_PERCENT   = 74,//���ݷ� ����(�ۼ�Ʈ)
//	AT_IMPROVE_MAGIC_LEVEL      = 75,//���� ����(����)
//	AT_IMPROVE_MAGIC_PERCENT    = 76,//���� ����(�ۼ�Ʈ)
//	AT_IMPROVE_ATTACK_SPEED     = 77,//���� �ӵ�
//	AT_IMPROVE_GAIN_LIFE        = 78,//ȹ�� �����
//	AT_IMPROVE_GAIN_MANA        = 79,//ȹ�� ����
//
//
//	//  ���� �߰� �ɼ�.        
//
//	AT_PLUS_WING_MAXLIFE		= 80,		// �ִ� HP+50
//	AT_PLUS_WING_MAXMANA		= 81,		// �ִ� MP+50
//	AT_PLUS_ONE_PERCENT_DAMAGE  = 82,   //  1% Ȯ���� ���� ���� ���� ����.
//	AT_IMPROVE_AG_MAX           = 83,   //  �ִ� AG +50 ����.
//    AT_DAMAGE_ABSORB            = 84,   //  5% �߰� ������ ���.
//	
//
//	AT_ATTACK1          = 100,
//	AT_ATTACK2,
//
//	AT_STAND1,
//	AT_STAND2,
//	AT_MOVE1,
//	AT_MOVE2,
//
//	AT_DAMAGE1,
//	AT_DIE1,
//	AT_SIT1,
//	AT_POSE1,
//	AT_HEALING1,
//	AT_GREETING1,
//	AT_GOODBYE1,
//	AT_CLAP1,
//	AT_GESTURE1,
//	AT_DIRECTION1,
//	AT_UNKNOWN1,
//	AT_CRY1,
//	AT_CHEER1,
//	AT_AWKWARD1,
//	AT_SEE1,
//	AT_WIN1,
//    AT_SMILE1,
//	AT_SLEEP1,
//	AT_COLD1,
//	AT_AGAIN1,
//	AT_RESPECT1,
//	AT_SALUTE1,
//	AT_RUSH1,	
//
//	AT_SET_RING_OPTION_ADD_SKILLDAMAGE,					// ��Ʈ �߰� ������(����, ����̷�)
//
//	AT_SET_OPTION_IMPROVE_STRENGTH = 140,				// �� ����
//	AT_SET_OPTION_IMPROVE_DEXTERITY,					// ��ø�� ����
//	AT_SET_OPTION_IMPROVE_ENERGY,						// ������ ����
//	AT_SET_OPTION_IMPROVE_VITALITY,						// ü�� ����
//	AT_SET_OPTION_IMPROVE_LEADERSHIP,						// ��� ����
//	AT_SET_OPTION_IMPROVE_MINATTACKDAMAGE,				// "�ּ� ���ݷ� ����"
//	AT_SET_OPTION_IMPROVE_MAXATTACKDAMAGE,				// "�ִ� ���ݷ� ����"
//	AT_SET_OPTION_IMPROVE_MAGICDAMAGE,					// "���� ����"
//	AT_SET_OPTION_IMPROVE_DAMAGE,						// "������ ����"
//	AT_SET_OPTION_IMPROVE_ATTACKRATE,					// "������ ����"
//	AT_SET_OPTION_IMPROVE_DEFENCE,						// "���� ����"	
//	AT_SET_OPTION_IMPROVE_MAXLIFE,							// "���� ����"
//	AT_SET_OPTION_IMPROVE_MAXMANA,						// "���� ����"
//	AT_SET_OPTION_IMPROVE_MAXAG,						// �ִ� AG����
//	AT_SET_OPTION_IMPROVE_AG_VALUE,						// AG������ ����
//	AT_SET_OPTION_IMPROVE_CRITICALDAMAGE_SUCCESS,		// "ũ��Ƽ�� ������ Ȯ�� ����"
//	AT_SET_OPTION_IMPROVE_CRITICALDAMAGE,				// "ũ��Ƽ�� ������ ����"
//	AT_SET_OPTION_IMPROVE_EX_DAMAGE_SUCCESS,			// "������Ʈ ������ Ȯ�� ����"
//	AT_SET_OPTION_IMPROVE_EX_DAMAGE,					// "������Ʈ ������ ����"
//	AT_SET_OPTION_IMPROVE_SKILLDAMAGE,					// "��ų ���ݷ� ����"
//	AT_SET_OPTION_DOUBLE_DAMAGE,						// "�������� Ȯ��"	
//	AT_SET_OPTION_DEFENCE_IGNORE,						// "�� ���� ����"
//	AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE,				//	���� ���� ���� 5% ����          (��)
//	AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE,		//	��հ� ���� ������ 15% ����       ( ������ )	
//	AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_STR,		// "���ݷ� ���� ��/x"
//	AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_DEX,		// "���ݷ� ���� ��ø/x"
//	AT_SET_OPTION_IMPROVE_DEFENCE_WITH_DEX,				// "���� ���� ��ø��/x"
//	AT_SET_OPTION_IMPROVE_DEFENCE_WITH_VIT,				// "���� ���� ü��/x"	
//	AT_SET_OPTION_IMPROVE_MAGICDAMAGE_WITH_ENERGY,		// "���� ���� ������/x"
//	AT_SET_OPTION_ICE_MASTERY,							//	Ư�� ĳ���� �����Ӽ� ��ų �߰� ������
//	AT_SET_OPTION_POSION_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ������
//	AT_SET_OPTION_THUNDER_MASTERY,						//	Ư�� ĳ���� �����Ӽ� ��ų �߰� ������
//	AT_SET_OPTION_FIRE_MASTERY,							//	Ư�� ĳ���� �ҼӼ� ��ų �߰� ������
//	AT_SET_OPTION_EARTH_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ������
//	AT_SET_OPTION_WIND_MASTERY,							//	Ư�� ĳ���� �ٶ��Ӽ� ��ų �߰� ������
//	AT_SET_OPTION_WATER_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ������
//
//	AT_SET_IMPROVE_STRENGTH,							// ��Ʈ ������ �ɼ� (������ : *3)	: �����
//	AT_SET_IMPROVE_DEXTERITY,							// ��Ʈ ������ �ɼ� (��ø���� : *3)	: ����̷� 
//	AT_SET_IMPROVE_ENERGY,								// ��Ʈ ������ �ɼ� (���������� : *3) : ������
//	AT_SET_IMPROVE_VITALITY,							// ��Ʈ ������ �ɼ� (ü������ : *3) : ����
//	
////
////
////	AT_SET_OPTION_DOUBLE_DAMAGE					= 140,  //	���� ������ Ȯ�� 2%                 ( ������ )
////	AT_SET_OPTION_DEFENCE_IGNORE,						//	�� ���� ���� 2%                   ( ������ )
////	AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE,		//	��հ� ���� ������ 15% ����       ( ������ )	
////	AT_SET_OPTION_IMPROVE_ATTACKING_PERCENT,			//	���� ������ ���� 5%                 (��)
////	AT_SET_OPTION_REFLECTION_DAMAGE,					//	100% ������ �ݻ� Ȯ�� 5% ����       ( ������ ó�� )
////	AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE,				//	���� ���� ���� 5% ����          (��)
////	AT_SET_OPTION_IMPROVE_STRENGTH_1,					//	�� ���� +10                         (��)	                
////	AT_SET_OPTION_IMPROVE_STRENGTH_2,					//	�� ���� +20                         (��)		                
////	AT_SET_OPTION_IMPROVE_DEXTERITY_1,					//	��ø�� ���� +10                     (��)		            
////	AT_SET_OPTION_IMPROVE_DEXTERITY_2,					//	��ø�� ���� +20                     (��)	                
////	AT_SET_OPTION_IMPROVE_ENERGY_1,						//	������ ���� +10	                    (��)	                
////	AT_SET_OPTION_IMPROVE_ENERGY_2,						//	������ ���� +20	                    (��)	                
////	AT_SET_OPTION_IMPROVE_VITALITY_1,					//	ü�� ���� +10	                    (��)	                
////	AT_SET_OPTION_IMPROVE_VITALITY_2,					//	ü�� ���� +20	                    (��)	                
////	AT_SET_OPTION_IMPROVE_MAX_LIFE_1,					//	�ִ� ����� ���� +40	            (�������� ������ �޴´�.)
////	AT_SET_OPTION_IMPROVE_MAX_LIFE_2,					//	�ִ� ����� ���� +80            	(�������� ������ �޴´�.)
////	AT_SET_OPTION_IMPROVE_MAX_MANA_1,					//	�ִ� ���� ���� +40	               	(�������� ������ �޴´�.)
////	AT_SET_OPTION_IMPROVE_MAX_MANA_2,					//	�ִ� ���� ���� +80          		(�������� ������ �޴´�.)
////	AT_SET_OPTION_IMPROVE_MAX_AG,						//	�ִ� AG 5% ����		            	(�������� ������ �޴´�.)
////	AT_SET_OPTION_DECREASE_SPELL_AG,					//	�Һ� AG 10% ����	                (��)
////	AT_SET_OPTION_IMPROVE_ITEM_DROP,					//	������ ��� Ȯ�� 5% ����		
////
////	AT_SET_OPTION_IMPROVE_ATTACK_1,						//	Ư�� ĳ���� ���ݷ� ���� ��ø��/60	(��)
////	AT_SET_OPTION_IMPROVE_ATTACK_2,						//	Ư�� ĳ���� ���ݷ� ���� ��/50   	(��)
////	AT_SET_OPTION_IMPROVE_MAGIC,						//	Ư�� ĳ���� ���� ���� ������/70	    (��)
////	AT_SET_OPTION_IMPROVE_DEFENCE_1,					//	Ư�� ĳ���� ���� ���� ����/20	    (��)
////	AT_SET_OPTION_IMPROVE_DEFENCE_2,					//	Ư�� ĳ���� ���� ���� 3%	        (��)
////	AT_SET_OPTION_IMPROVE_DEFENCE_3,					//	Ư�� ĳ���� ���� ���� ��ø��/50	(��)
////	AT_SET_OPTION_IMPROVE_DEFENCE_4,					//	Ư�� ĳ���� ���� ���� ü��/40	    (��)
////
////    AT_SET_OPTION_FIRE_MASTERY,							//	Ư�� ĳ���� �ҼӼ� ��ų �߰� ���ݷ� 10%		        
////	AT_SET_OPTION_ICE_MASTERY,							//	Ư�� ĳ���� �����Ӽ� ��ų �߰� ���ݷ� 10%			    
////	AT_SET_OPTION_THUNDER_MASTERY,						//	Ư�� ĳ���� �����Ӽ� ��ų �߰� ���ݷ� 10%			    
////	AT_SET_OPTION_POSION_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ���ݷ� 10%			        
////	AT_SET_OPTION_WATER_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ���ݷ� 10%			        
////	AT_SET_OPTION_WIND_MASTERY,							//	Ư�� ĳ���� �ٶ��Ӽ� ��ų �߰� ���ݷ� 10%			    
////	AT_SET_OPTION_EARTH_MASTERY,						//	Ư�� ĳ���� ���Ӽ� ��ų �߰� ���ݷ� 10%
////	
//
////
////	AT_IMPROVE_OPTION_MAX_MANA,				// �ִ� ���� �ɼ�
////	AT_IMPROVE_OPTION_MAX_AG,				// �ִ� AG����
//};
//----------------------------------------------------------------------------
//#endif


#endif

