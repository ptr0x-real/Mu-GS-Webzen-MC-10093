//----------------------------------------------------------------------------------
// FileName				: RaklionDefine.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ���� define�� ��� ���� ����
//----------------------------------------------------------------------------------

#pragma once

#define RAKLION_DEFAULT_REGEN_GATE		286		// �����ʿ��� ����ϰų� �����Ǵ� ���
#define RAKLION_COOLUTIN				457		// ��ƾ �ε���
#define RAKLION_IRON_KNIGHT				458		// ���̾� ����Ʈ �ε���
#define RAKLION_SELUPAN					459		// ������ �ε���
#define RAKLION_SELUPAN_EGG_1			460		// ������ ��1 �ε���
#define RAKLION_SELUPAN_EGG_2			461		// ������ ��2 �ε���
#define RAKLION_SELUPAN_EGG_3			462		// ������ ��3 �ε���

#ifndef MAP_INDEX_RAKLION_BOSS
#define MAP_INDEX_RAKLION_FIELD			57		// MAP_INDEX_RAKLION_BOSS �� ������ ������ �ʴٸ� �������Ѵ�.
#define MAP_INDEX_RAKLION_BOSS			58		// MAP_INDEX_RAKLION_BOSS �� ������ ������ �ʴٸ� �������Ѵ�.
#endif											// ���� ���� ���°��� ���ϱ� ���� ���̰� �ٸ� �ǹ̴� ����.


///�ӽ�//////
#define RAKLION_SELUPAN_EGG_HALF		1		// ������ ���� �Ź̾� ����(�ӽ�)
#define RAKLION_SELUPAN_EGG_MAX			15		// �Ź̾� �� ����(�ӽ�)
#define RAKLION_BOSS_APPEARANCE_DELAY	5		// �Ź̾��� ��� �ı��� �� ������ ��������� ������(����ð�)(sec)
#define RAKLION_BOSSZONE_CLOSE_DELAY	10		// �������� ���۵� �� �������� ������ �ð�(sec)
#define RAKLION_BOSS_END_DELAY			10		// �������� ����� �� �ʱ�ȭ�Ǵ� �ð�(sec)
#define RAKLION_BOSS_SKILL_DELAY		3000	// �������� ��ų ��� ������
/////////////

enum RAKLION_STATE								// ��Ŭ���� ��ü ���� ������
{
	RAKLION_STATE_IDLE					= 0,	// ��Ŭ���� ������ ��� ����(�׻� ��� �����̴�)
	RAKLION_STATE_NOTIFY_1				= 1,	// �������� �Ź̾��� ������.(Ŭ���̾�Ʈ�� �޽��� ����)
	RAKLION_STATE_STANDBY				= 2,	// ���� ������ ������ ���� ������
	RAKLION_STATE_NOTIFY_2				= 3,	// ���� �޽��� ���(�������� �����Ͽ����ϴ�.)
	RAKLION_STATE_READY					= 4,	// �������� �غ� - ���� ���� �� ���� ��ų �ߵ�
	RAKLION_STATE_START_BATTLE			= 5,	// ������ ���� ���� - ������ ���� �� 5�а��� ���� �����̴�
	RAKLION_STATE_NOTIFY_3				= 6,	// ���� �޽��� ���(��Ŭ���� �������� �������ϴ�.)
	RAKLION_STATE_CLOSE_DOOR			= 7,	// ������ ���� ���� - ������ ���� ���·� ����
	RAKLION_STATE_ALL_USER_DIE			= 8,	// �������� ������ ��� ���� ����
	RAKLION_STATE_NOTIFY_4				= 9,	// ���� �޽��� ���(������ ����) - 5�а� ���
	RAKLION_STATE_END					= 10,	// ���� ���� - ������ �ٽ� ����
	RAKLION_STATE_DETAIL_STATE			= 11,	// �����ǰ��� ������ ���� ���¸� ������ ���� ��
	RAKLION_STATE_MAX					= 12,
};

enum RAKLION_BATTLE_OF_SELUPAN_PATTERN				// ������ ���� ������
{
	BATTLE_OF_SELUPAN_NONE					= 0,	// �������� ���°� �ƴ�(������ ���� ����)
	BATTLE_OF_SELUPAN_STANDBY				= 1,	// �������� �غ� ����
	BATTLE_OF_SELUPAN_PATTERN_1				= 2,	// ������ ���� 1�ܰ�
	BATTLE_OF_SELUPAN_PATTERN_2				= 3,	// ������ ���� 2�ܰ�
	BATTLE_OF_SELUPAN_PATTERN_3				= 4,	// ������ ���� 3�ܰ�
	BATTLE_OF_SELUPAN_PATTERN_4				= 5,	// ������ ���� 4�ܰ�
	BATTLE_OF_SELUPAN_PATTERN_5				= 6,	// ������ ���� 5�ܰ�
	BATTLE_OF_SELUPAN_PATTERN_6				= 7,	// ������ ���� 6�ܰ�
	BATTLE_OF_SELUPAN_PATTERN_7				= 8,	// ������ ���� 7�ܰ�
	BATTLE_OF_SELUPAN_DIE					= 9,	// �������� �׾���.
	BATTLE_OF_SELUPAN_MAX					= 10,	// �������� �ִ� ���� ��
};

enum RAKLION_BATTLE_SUCCESS_VALUE
{
	RAKLION_BATTLE_FAIL							= 0,	// ����
	RAKLION_BATTLE_SUCCESS						= 1		// ����
};

enum RAKLION_SELUPAN_SKILL
{
	RAKLION_SELUPAN_SKILL_FIRST_SKILL			= 0,	// ���ʵ���� �о�� ��ų
	RAKLION_SELUPAN_SKILL_POISON_ATTACK			= 1,	// �⺻ ����(��)
	RAKLION_SELUPAN_SKILL_ICE_STORM				= 2,	// ���̽� ����
	RAKLION_SELUPAN_SKILL_ICE_STRIKE			= 3,	// ���̽� ��Ʈ����ũ
	RAKLION_SELUPAN_SKILL_SUMMON_MONSTER		= 4,	// ���� ��ȯ
	RAKLION_SELUPAN_SKILL_HEAL					= 5,	// �ڱ� �ڽ� ġ��
	RAKLION_SELUPAN_SKILL_FREEZE				= 6,	// �󸮱�
	RAKLION_SELUPAN_SKILL_TELEPORT_RANDOM		= 7,	// ���� ���� �̵�
	RAKLION_SELUPAN_SKILL_INVINCIBILITY			= 8,	// ���� ����
	RAKLION_SELUPAN_SKILL_MAX					= 9,
};


//---------------------------------------------------------------------------------------------
//
// ��ڿ� ��ɾ�
//----------------------------------------------------------------------------------------------
enum RAKLION_GM_COMMAND
{
	RAKLION_GM_COMMAND_IDLE					= 0,		// "/��Ŭ���´��"
	RAKLION_GM_COMMAND_NOTIFY_1				= 1,		// "/��Ŭ���°���1"
	RAKLION_GM_COMMAND_STANDBY				= 2,		// "/��Ŭ���½��Ĺ���"
	RAKLION_GM_COMMAND_NOTIFY_2				= 3,		// "/��Ŭ���°���2"
	RAKLION_GM_COMMAND_READY				= 4,		// "/��Ŭ�����غ�"
	RAKLION_GM_COMMAND_START_BATTLE			= 5,		// "/��Ŭ���½���"
	RAKLION_GM_COMMAND_NOTIFY_3				= 6,		// "/��Ŭ���°���3"
	RAKLION_GM_COMMAND_CLOSE_DOOR			= 7,		// "/��Ŭ�������"
	RAKLION_GM_COMMAND_ALL_USER_DIE			= 8,		// "/��Ŭ���½���"
	RAKLION_GM_COMMAND_NOTIFY_4				= 9,		// "/��Ŭ���°���4"
	RAKLION_GM_COMMAND_END					= 10,		// "/��Ŭ��������"

	RAKLION_GM_COMMAND_SELUPAN_1			= 11,		// "/����������1"
	RAKLION_GM_COMMAND_SELUPAN_2			= 12,		// "/����������2"
	RAKLION_GM_COMMAND_SELUPAN_3			= 13,		// "/����������3"
	RAKLION_GM_COMMAND_SELUPAN_4			= 14,		// "/����������4"
	RAKLION_GM_COMMAND_SELUPAN_5			= 15,		// "/����������5"
	RAKLION_GM_COMMAND_SELUPAN_6			= 16,		// "/����������6"
	RAKLION_GM_COMMAND_SELUPAN_7			= 17,		// "/����������7"

	RAKLION_GM_COMMAND_FIRST_SKILL			= 18,		// "/���������ʽ�ų"
	RAKLION_GM_COMMAND_POISON_ATTACK		= 19,		// "/�����ǵ�����"
	RAKLION_GM_COMMAND_ICE_STORM			= 20,		// "/�����ǳñ���ǳ"
	RAKLION_GM_COMMAND_ICE_STRIKE			= 21,		// "/�����ǳñ����"
	RAKLION_GM_COMMAND_SUMMON_MONSTER		= 22,		// "/�����Ǽ�ȯ"
	RAKLION_GM_COMMAND_HEAL					= 23,		// "/��������"
	RAKLION_GM_COMMAND_FREEZE				= 24,		// "/�����Ǿ󸮱�"
	RAKLION_GM_COMMAND_TELEPORT_RANDOM		= 25,		// "/�����Ǽ����̵�"
	RAKLION_GM_COMMAND_INVINCIBILITY		= 26,		// "/�����ǹ���ȭ"
};
