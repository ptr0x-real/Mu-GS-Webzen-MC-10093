//----------------------------------------------------------------------------------
//
//	KanturuDefine.h
//
//----------------------------------------------------------------------------------

#define MAX_KANTURU_BATTLE_USER			15

//---------------------------------------------------------------------------------------------
//
// ĭ���� ��
//----------------------------------------------------------------------------------------------
enum KANTURU_MAP_TYPE			
{
	KANTURU_MAP_TOWEROFREFINEMENT_CLOSE		= 0,	// ������ ž ����
	KANTURU_MAP_TOWEROFREFINEMENT_OPEN		= 1,	// ������ ž ����
	MAX_KANTURU_TOWEROFREFINEMENT_MAP		= 2
};

// ������ ����Ʈ ��ȣ
#define KANTURU_MAYA_MAP_GATE			133		// NPC�� ���� ���尡���� �������� �� ����Ʈ ����
#define KANTURU_NIGHTMARE_MAP_GATE		134		// NPC�� ���� ���尡���� ����Ʈ�޾����� �� ����Ʈ ����
#define KANTURU_TOWER_OPEN_MAP_GATE		135		// NPC�� ���� ������ �� ������ ž�� ���� ������ ������ ���
#define KANTURU_KICK_OUT_FROM_TOWER		136		// ������ ž�� ���� ���¿��� ĭ���� ��������� ������ �̵�
#define KANTURU_DEFAULT_REGEN_MAP_GATE	137		// �����ʿ��� ����ϰų� �����Ǵ� ���

#ifdef ADD_KANTURU_CONDITION_20060725
// ĭ���� �Ϲݸʿ��� ����� �̵� ����Ʈ ��ȣ
#define KANTURU_DIEREGEN_GATE_MAP_1		138
#define KANTURU_DIEREGEN_GATE_MAP_2		139
#endif

//---------------------------------------------------------------------------------------------
//
// ������ ���� �ε�
//---------------------------------------------------------------------------------------------
enum KANTURU_DATAFILE_LOAD
{
	KANTURU_DATAFILE_LOAD_STATE						= 0,	// ĭ���� ������ �⺻ ���� ����
	KANTURU_DATAFILE_LOAD_STANBY_STATE				= 1,	// ĭ���� ������ ���λ��� ����
	KANTURU_DATAFILE_LOAD_MAYA_SCENE_STATE			= 2,	// ĭ���� ������ ���⺰ ���� ����
	KANTURU_DATAFILE_LOAD_MAYA_STATE				= 3,	// ĭ���� ������ ���λ��� ����
	KANTURU_DATAFILE_LOAD_MAYA_HAND_AICHANGE_TIME	= 4,	// ĭ���� ������ ���� �� AI ���� �ð�
	KANTURU_DATAFILE_LOAD_MAYA_HAND_GROUP_NUMBER	= 5,	// ĭ���� ������ ���߼� �׷��ȣ
	KANTURU_DATAFILE_LOAD_MAYA_ICESTORM_USING_RATE	= 6,	// ĭ���� ������ ���� ���̽����� ��ų ��� Ȯ��
	KANTURU_DATAFILE_LOAD_NIGHTMARE_STATE			= 7,	// ĭ���� ����Ʈ�޾��� ���λ��� ����
	KANTURU_DATAFILE_LOAD_NIGHTMARE_BATTLEMONSTER	= 8,	// ĭ���� ����Ʈ�޾��� ���� ���� ����
	KANTURU_DATAFILE_LOAD_TOWEROFREFINEMENT_STATE	= 9,	// ĭ���� ������ž ���λ��� ����
	KANTURU_DATAFILE_LOAD_CHECK_MOONSTONE			= 10	// ĭ���� ������ ����� ���������Ʈ ����˻� ����
};

enum KANTURU_SET_MONSTER
{
	KANTURU_SET_MONSTER_MAYAMONSTER1				= 0,	// ĭ���� ������ ���Ϳ��� ���� ù ��°, �� ��° ���� ����
	KANTURU_SET_MONSTER_MAYAMONSTER3				= 1,	// ĭ���� ������ ���Ϳ��� ���� �� ��°
	KANTURU_SET_MONSTER_NIGHTMAREMONSTER			= 2,	// ĭ���� ����Ʈ�޾��� ����
	KANTURU_SET_MONSTER_MAYA_LEFTHAND				= 3,	// ĭ���� ���� �޼�
	KANTURU_SET_MONSTER_MAYA_RIGHTHAND				= 4,	// ĭ���� ���� ������
	KANTURU_SET_MONSTER_MAYA_HANDS					= 5,	// ĭ���� ���� ���	
	KANTURU_SET_MONSTER_REGENMONSTER				= 6,	// ĭ���� ����Ʈ�޾��� �����Ǵ� ����
};


//---------------------------------------------------------------------------------------------
//
// ������ ����
//----------------------------------------------------------------------------------------------
#define KANTURU_MONSTER_CANNONTRAP		105
#define KANTURU_MONSTER_LASERTRAP		106

#define KANTURU_MONSTER_BLADEHUNTER		354
#define KANTURU_MONSTER_GENOCIDER		357
#define KANTURU_MONSTER_PERSONA			358
#define KANTURU_MONSTER_TWINTAILE		359
#define KANTURU_MONSTER_DREADPEAR		360

#define KANTURU_MONSTER_MAYA_LEFTHAND	362
#define KANTURU_MONSTER_MAYA_RIGHTHAND	363
#define KANTURU_MONSTER_MAYA			364

#define KANTURU_MONSTER_NIGHTMARE		361

//---------------------------------------------------------------------------------------------
//
// ���� ����
//----------------------------------------------------------------------------------------------
enum KANTURU_BATTLE_OF_MAYA_BATTLE_STATE			// ������ �� ���� ����
{
	BATTLE_OF_MAYA_BATTLE_NONE			= -1,	// ������ �������� �ƴ�
	BATTLE_OF_MAYA_BATTLE_FIRST			= 0,	// 1�� ����
	BATTLE_OF_MAYA_BATTLE_SECOND		= 1,	// 2�� ����
	BATTLE_OF_MAYA_BATTLE_THIRD			= 2,	// 3�� ����
	MAX_BATTLE_STATE_BATTLE_OF_MAYA		= 3		// ������ �ִ� ���� ����
};

enum KANTURU_BATTLE_SUCCESS_LOG						// ĭ���� ���� ������ ���� �α׸� ����� ���� �ε���
{
	KANTURU_BATTLE_SUCCESS_LOG_MAYA1		= 0,	// ���� �޼հ��� ���� ����
	KANTURU_BATTLE_SUCCESS_LOG_MAYA2		= 1,	// ���� �����հ��� ���� ����
	KANTURU_BATTLE_SUCCESS_LOG_MAYA3		= 2,	// ���� ��հ��� ���� ����
	KANTURU_BATTLE_SUCCESS_LOG_NIGHTMARE	= 3		// ����Ʈ���ӿ��� ���� ����
};


//---------------------------------------------------------------------------------------------
//
// �����췯 ����
//----------------------------------------------------------------------------------------------
enum KANTURU_STATE								// ĭ���� ��ü ���� ������
{
	KANTURU_STATE_NONE						= 0,	// ĭ���� ���� ������ ����	
	KANTURU_STATE_BATTLE_STANDBY			= 1,	// ���� ��� ����	
	KANTURU_STATE_BATTLE_OF_MAYA			= 2,	// 1�� ������(����)	
	KANTURU_STATE_BATTLE_OF_NIGHTMARE		= 3,	// 2�� ������(����Ʈ�޾�)	
	KANTURU_STATE_TOWER_OF_REFINEMENT		= 4,	// ������ ž	
	KANTURU_STATE_END						= 5,	// ĭ���� ������ ����
	MAX_KANTURU_STATE						= 6
};

enum KANTURU_BATTLE_STANBY_STATE				// ĭ���� ���� ������ ������
{
	KANTURU_BATTLE_STANBY_STATE_NONE		= 0,	// ���� ������ �ƴ�
	KANTURU_BATTLE_STANBY_STATE_START		= 1,	// ���� ������
	KANTURU_BATTLE_STANBY_STATE_NOTIFY		= 2,	// ���� �޼��� ���
	KANTURU_BATTLE_STANBY_STATE_END			= 3,	// ����
	MAX_KANTURU_BATTLE_STANBY_STATE			= 4		// ���� ������ �ִ� ��
};

enum KANTURU_BATTLE_OF_MAYA_STATE				// 1�� ������(����) ���� ������
{
	BATTLE_OF_MAYA_STATE_NONE				= 0,	// 1�� ������ ���°� �ƴ�
	BATTLE_OF_MAYA_STATE_STANBY1			= 1,	// ������	
	BATTLE_OF_MAYA_STATE_NOTIFY1			= 2,	// �������� 1
	BATTLE_OF_MAYA_STATE_START1				= 3,	// ù ��° ���Ϳ��� ����
	BATTLE_OF_MAYA_STATE_MAYA1				= 4,	// ù ��° ���� �հ��� ���� 
	BATTLE_OF_MAYA_STATE_END_MAYA1			= 5,	// ù ��° ���� ���� ����
	BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA1		= 6,	// ù ��° ���� ���� ���ῡ ���� ó��
	BATTLE_OF_MAYA_STATE_STANBY2			= 7,	// ������

	BATTLE_OF_MAYA_STATE_START2				= 8,	// �� ��° ���Ϳ��� ����
	BATTLE_OF_MAYA_STATE_MAYA2				= 9,	// �� ��° ���� �հ��� ����
	BATTLE_OF_MAYA_STATE_END_MAYA2			= 10,	// �� ��° ���� ���� ����
	BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA2		= 11,	// �� ��° ���� ���� ���ῡ ���� ó��
	BATTLE_OF_MAYA_STATE_STANBY3			= 12,	// ������

	BATTLE_OF_MAYA_STATE_START3				= 13,	// �� ��° ���Ϳ��� ����
	BATTLE_OF_MAYA_STATE_MAYA3				= 14,	// �� ��° ���� �հ��� ����
	BATTLE_OF_MAYA_STATE_END_MAYA3			= 15,	// �� ��° ���� ���� ����
	BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA3		= 16,	// �� ��° ���� ���� ���ῡ ���� ó��
	BATTLE_OF_MAYA_STATE_END				= 17,	// 1�� ������ ����
	BATTLE_OF_MAYA_STATE_ENDCYCLE			= 18,	// 1�� ������ ���ῡ ���� ó��
	MAX_BATTLE_OF_MAYA_STATE						// 1�� ������ �ִ� ���� ��
};

enum KANTURU_BATTLE_OF_NIGHTMARE_STATE			// 2�� ������(����Ʈ�޾�) ���� ������
{
	BATTLE_OF_NIGHTMARE_STATE_NONE			= 0,	// 2�� ������ ���� �ƴ�
	BATTLE_OF_NIGHTMARE_STATE_IDLE			= 1,	// Ŭ���̾�Ʈ ������		
	BATTLE_OF_NIGHTMARE_STATE_NOTIFY		= 2,	// ����Ʈ�޾� ���� ���� ����
	BATTLE_OF_NIGHTMARE_STATE_START			= 3,	// ���� ����	
	BATTLE_OF_NIGHTMARE_STATE_END			= 4,	// 2�� ������ ����	
	BATTLE_OF_NIGHTMARE_STATE_ENDCYCLE		= 5,	// ���� ó��
	MAX_BATTLE_OF_NIGHTMARE_STATE					// 2�� ������ ����Ʈ�޾� ���� �ִ� ��
};

enum KANTURU_TOWER_OF_REFINEMENT_STATE			// ������ ž ���� ������
{
	TOWER_OF_REFINEMENT_STATE_NONE				= 0,	// ������ ž Ȱ��ȭ ���°� �ƴ�
	TOWER_OF_REFINEMENT_STATE_REVITALIZATION	= 1,	// ������ ž Ȱ��ȭ
	TOWER_OF_REFINEMENT_STATE_NOTIFY1			= 2,	// ���� �޼��� ���
	TOWER_OF_REFINEMENT_STATE_CLOSE				= 3,	// ������ ž ���� �Ұ�
	TOWER_OF_REFINEMENT_STATE_NOTIFY2			= 4,	// ���� �޼��� ���
	TOWER_OF_REFINEMENT_STATE_END				= 5,	// ������ ž ��� ����
	MAX_TOWER_OF_REFINEMENT_STATE						// ������ ž ���� �ִ� ��
};



//---------------------------------------------------------------------------------------------
//
// ��������
//----------------------------------------------------------------------------------------------
enum KANRUTU_STATE_END_COMDITION
{
	KANRUTU_STATE_END_COMDITION_NONE			= 0,	// �ƹ��� �������� ����
	KANRUTU_STATE_END_COMDITION_TIMELIMIT		= 1,		// �ð�����
};



//---------------------------------------------------------------------------------------------
//
// ����/ ����
//----------------------------------------------------------------------------------------------
enum KANTURU_BATTLE_SUCCESS_VALUE
{
	KANTURU_BATTLE_FAIL							= 0,	// ����
	KANTURU_BATTLE_SUCCESS						= 1		// ����
};

enum KANTURU_MAYA_SKILL
{
	KANTURU_MAYA_SKILL_ICE_STORM				= 0,	// ���̽� ����
	KANTURU_MAYA_SKILL_BROKEN_SHOWER			= 1		// ���ū ����
};


#ifdef KANTURU_GM_COMMAND_20060709
//---------------------------------------------------------------------------------------------
//
// ��ڿ� ��ɾ�
//----------------------------------------------------------------------------------------------
enum KANTURU_GM_COMMAND
{
	KANTURU_GM_COMMAND_STANBY				= 0,	// "/ĭ����������"
	KANTURU_GM_COMMAND_BATTLEOFMAYA			= 1,		// "/ĭ����������"
	
	KANTURU_GM_COMMAND_MAYA_MONSTER1		= 2,		// "/���߸���1"
	KANTURU_GM_COMMAND_MAYA1				= 3,		// "/����1"
	KANTURU_GM_COMMAND_MAYA_MONSTER2		= 4,		// "/����2"
	KANTURU_GM_COMMAND_MAYA2				= 5,		// "/����2"
	KANTURU_GM_COMMAND_MAYA_MONSTER3		= 6,		// "/���߸���3"
	KANTURU_GM_COMMAND_MAYA3				= 7,		// "/����3"
	KANTURU_GM_COMMAND_MAYA3_SUCCESS		= 8,		// "/����3����"	

	KANTURU_GM_COMMAND_MAYA_SUCCESS			= 9,		// "/���߼���"
	KANTURU_GM_COMMAND_MAYA_FAIL			= 10,		// "/���߽���"

	KANTURU_GM_COMMAND_NIGHTMARE			= 11,		// "/ĭ��������Ʈ�޾���"
	KANTURU_GM_COMMAND_NIGHTMARE_SUCCESS	= 12,		// "/����Ʈ�޾��"
	KANTURU_GM_COMMAND_NIGHTMARE_FAIL		= 13,		// "/����Ʈ�޾����"

	KANTURU_GM_COMMAND_TOWEROFREFINEMENT	= 14,		// "/ĭ����������ž"
	KANTURU_GM_COMMAND_TOWER_OPEN			= 15,		// "/������ž����"
	KANTURU_GM_COMMAND_TOWER_CLOSE			= 16,		// "/������ž�ݱ�"
	KANTURU_GM_COMMAND_END					= 17,		// "/ĭ��������"

	KANTURU_GM_COMMAND_NEXT_STATE			= 18,		// "ĭ������������"
	KANTURU_GM_COMMAND_NEXT_DETAIL_STATE	= 19,		// "ĭ�����������λ���"
	KANTURU_GM_COMMAND_CURRENT_STATE		= 20,		// "ĭ�����������"	
};

#endif