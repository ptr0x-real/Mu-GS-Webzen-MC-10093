// CastleSiege.h: interface for the CCastleSiege class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLESIEGE_H__6C5A97C8_FFFB_42E4_8F56_ABE80C670758__INCLUDED_)
#define AFX_CASTLESIEGE_H__6C5A97C8_FFFB_42E4_8F56_ABE80C670758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)

#ifdef CASTLE_MAIN_SCHEDULER_20041111

#include <VECTOR>
#include <MAP>
#include <STRING>
#include <ALGORITHM>

using namespace std;

#include "Include\DEFINE.H"
#include "Include\SPRODEF.H"
#include "Common\WINUTIL.H"

#ifdef MODIFY_CASTLE_PK_01_20050309
#define CASTLE_PK_BORDER					113					// �� ���� ��尡 ���� �غ�Ⱓ�� PK �Ҽ� �ִ� ��輱 (apple)
#else
#define CASTLE_PK_BORDER					112					// �� ���� ��尡 ���� �غ�Ⱓ�� PK �Ҽ� �ִ� ��輱 (apple)
#endif
#define MIN_CS_REGSIEGE_GUILD_MEMBERCOUNT	20					// ���� ���� ��û ����� ���� �ּ��ο�(apple)

#define	MAX_CS_SCHEDULE_COUNT				50					// �ִ� ���� ������ �׸��� ����
#define	MAX_CS_NPC_COUNT					150					// �ִ� ���� NPC ����
#define MAX_CS_GUILD_COUNT					100					// �ִ� �������� ��� ����
#define	DEFAULT_CASTLE_SIEGE_CYCLE			14					// �⺻ �����ֱ� (��)
#define	DEFAULT_CS_LEFT_ALARM_TICK			(3*60*1000)			// ������ �����ð� �˸� �ֱ� (MSEC)
#define	DEFAULT_CS_LEFT_ALARM_TICK_U10		(1*60*1000)			// ������ �����ð� �˸� �ֱ� (MSEC) - 10�� ������ ��

#define	MAX_DBDATA_WAIT_TICK				(3*1000)			// DB �����Ͱ� �ö����� ��ٸ��� �ִ� �ð����� (�Ѿ�� �� ��û)
#define MAX_NPC_DBSAVE_TICK					(5*60*1000)			// NPC�� DB�� �����ϴ� �ð�����
#define MAX_CALC_LEFTSIEGE_TICK				(10*1000)			// ������ ���۱��� ���� �ð��� ����ϴ� �ð�����

#define CS_JOINSIDE_REFRESH_TICK			(10*1000)			// ������ �ʹݿ� ��/�� ������ �������¸� ������ �ִ� �ð�����
#define MAX_CS_JOINSIDE_REFRESH_COUNT		3					// ������ �ʹݿ� ��/�� ������ �������¸� ������ �ִ� �ִ�Ƚ��

#define CS_MINIMAP_SEND_TICK				(3*1000)			// ������ �� �̴ϸ� �����͸� ������ �ִ� �ð�����

#ifdef MU_CASTLESIEGE_START_NOTIFY_20050418
#define CS_STARTNOTIFY_MIN_TICK				(5*60*1000)			// ������ ������ ���� ���۽ð�
#define CS_STARTNOTIFY_PER_TICK				(1*60*1000)			// ������ ������ ���� ����ֱ�
#endif

#define CS_MSG_TICK_REGSIEGE				(30*60*1000)		// �̺�Ʈ �޽��� ��� �ð� - ������ ��ϱⰣ
#define CS_MSG_TICK_REGMARK					(30*60*1000)		// �̺�Ʈ �޽��� ��� �ð� - ������ǥ�� ��ϱⰣ
#define CS_MSG_TICK_IDLE_3					(120*60*1000)		// �̺�Ʈ �޽��� ��� �ð� - ������ ������� �����Ⱓ
#define CS_MSG_TICK_NOTIFY					(30*60*1000)		// �̺�Ʈ �޽��� ��� �ð� - ������ ������� ��ǥ�Ⱓ
#define CS_MSG_TICK_READYSIEGE				(30*60*1000)		// �̺�Ʈ �޽��� ��� �ð� - ������ �غ�Ⱓ
#define CS_MSG_TICK_STARTSIEGE				(30*60*1000)		// �̺�Ʈ �޽��� ��� �ð�

#define	CS_MIN_SIEGESTART					(15)				// ������ ���� �� �ּ� ���۽ð�
#define CS_CHECK_DBNPC_ALIVE_TICK			(5*1000)			// ����/������ ����ִ��� üũ�ϴ� �ð�

#define	CS_MOVENUM_LORENDEEP				23					// "/�̵� �η�����" �� MoveReq.txt �� �ε���

#define MAX_UPGRADE_LEVEL					3					// ���׷��̵� ������ �ִ뷹��
#define UPTYPE_DEFENCE						1					// ���׷��̵� Ÿ�� - ���� ����
#define UPTYPE_REGEN						2					// ���׷��̵� Ÿ�� - ȸ���� ����
#define UPTYPE_MAXHP						3					// ���׷��̵� Ÿ�� - �ִ�HP ����

#define	MAX_NPC_SEND_COUNT					50					// NPC����Ʈ�� ����ڿ��� �ѹ��� ���� �� �ִ� �ִ� NPC ����
#define	MAX_CS_MARKS_OVERLAP				255					// ������ ǥ���� ��ĥ �� �ִ� �ִ� ����
	
#define MAX_CS_TAX_RATE_CHAOS				3					// ���� �ִ� ���� (ī�������� -> %)
#define MAX_CS_TAX_RATE_STORE				3					// ���� �ִ� ���� (���� -> %)
#define MAX_CS_TAX_HUNT_ZONE				300000				// ���� �ִ� ���� (����� -> ��)
#define UNIT_CS_TAX_HUNT_ZONE				10000				// ���� ����� �������� (apple)

#define	MAX_CS_ATTACK_GUILD					3					// �ִ� ������ ����
#define	MAX_CS_REG_GUILD					100					// �ִ� ���� ��� ����
#define	MAX_CS_TOTAL_GUILD					150					// �ִ� ������ ����
#define MAX_MINIMAP_USER_COUNT				1000				// �� ���� 1000����� �̴ϸʿ� �ִ� �������� �� �� �ִ�.

#define	CS_MARK_GIVEUP_MONEY				3000				// ������ ������ ��帶���Ϳ��� ȯ���� �ִ� ��ũ�� ��
#define	CS_MARK_EXCHANGE_MONEY				2500				// ��ũ�� ȯ�ҹ������� ��帶���Ϳ��� ȯ���� �ִ� ��ũ�� ��

#ifdef MODIFY_CASTLE_CROWN_ACCESS_TIME_20070828		// ���� �����ð� 30�� -> 60�� ����
	#define CS_CASTLE_CROWN_ACCESS_TIME			(60*1000)			// �հ��� ���ο� �ɸ��� �ð�
#else
	#define CS_CASTLE_CROWN_ACCESS_TIME			(30*1000)			// �հ��� ���ο� �ɸ��� �ð�
#endif

#define CS_NPC_CROWN						216					// �հ�
#define CS_NPC_CROWN_SWITCH1				217					// �հ� ����ġ 1
#define CS_NPC_CROWN_SWITCH2				218					// �հ� ����ġ 2

#define CS_SIEGEGUILD_SEQ_PRIORITY			(1024*1024)			// ������ ��� ���� �� ������ �켱������ �ֱ� ���� �⺻��

#define CHECK_NPC_CROWN_SWITCH(C_VALUE)		(C_VALUE<CS_NPC_CROWN_SWITCH1?FALSE:C_VALUE>CS_NPC_CROWN_SWITCH2?FALSE:TRUE)


#ifdef CASTLE_TEST_SCHEDULER_20041218
//#define	CS_TEST_GAPSEC						(60*5)				// �׽�Ʈ �����췯 ���� �� �ð�����
#define	CS_TEST_GAPSEC						(60*1)				// �׽�Ʈ �����췯 ���� �� �ð�����
#endif


// ���� ���׷��̵� ���̺�
// 1 . ����
static INT g_iNpcUpDfLevel_CGATE[MAX_UPGRADE_LEVEL][3] = {		// �÷����� (0:����,1:�ʿ��ȣ����,2:�ʿ���,3:�������)
	1,			2,	3000000,
	2,			3,	3000000,
	3,			4,	3000000,
};
// 2 . ����
static INT g_iNpcUpDfLevel_CSTATUE[MAX_UPGRADE_LEVEL][3] = {	// �÷����� (0:����,1:�ʿ��ȣ����,2:�ʿ���,3:�������)
	1,			3,	3000000,
	2,			5,	3000000,
	3,			7,	3000000,
};
// �ִ� HP ���׷��̵� ���̺�
#ifdef MODIFY_GATE_AND_STATUE_INCREASE_MAX_HP_20080718
// 1 . ����
static INT g_iNpcUpMaxHP_CGATE[MAX_UPGRADE_LEVEL][3] = {		// �÷����� (0:�����ִ�HP,1:�ʿ��ȣ����,2:�ʿ���)
	2500000,	2,	1000000,
	3500000,	3,	1000000,
	5200000,	4,	1000000,
};
// 2 . ����
static INT g_iNpcUpMaxHP_CSTATUE[MAX_UPGRADE_LEVEL][3] = {		// �÷����� (0:�����ִ�HP,1:�ʿ��ȣ����,2:�ʿ���)
	2200000,	3,	1000000,
	3400000,	5,	1000000,
	5000000,	7,	1000000,
};
#else
// 1 . ����
static INT g_iNpcUpMaxHP_CGATE[MAX_UPGRADE_LEVEL][3] = {		// �÷����� (0:�����ִ�HP,1:�ʿ��ȣ����,2:�ʿ���)
	800000,		2,	1000000,
	1500000,	3,	1000000,
	3000000,	4,	1000000,
};
// 2 . ����
static INT g_iNpcUpMaxHP_CSTATUE[MAX_UPGRADE_LEVEL][3] = {		// �÷����� (0:�����ִ�HP,1:�ʿ��ȣ����,2:�ʿ���)
	750000,		3,	1000000,
	1300000,	5,	1000000,
	2500000,	7,	1000000,
};
#endif // MODIFY_GATE_AND_STATUE_INCREASE_MAX_HP_20080718

// ȸ���� ���׷��̵� ���̺� (���� ����)
static INT g_iNpcUpRgLevel_CSTATUE[MAX_UPGRADE_LEVEL][3] = {	// �÷����� (0:����,1:�ʿ��ȣ����,2:�ʿ���)
	1,			3,	5000000,
	2,			5,	5000000,
	3,			7,	5000000,
};
// �������� ������ ���� ���е� �⺻ ������ ��ġ
static INT g_iCsUserReSpawnArea[3][4] = {						// �� ���������� ���� SX, SY, DX, DY
	35,		17,		144,	48,									// CS_SIEGESIDE_NONE
	74,		144,	115,	154,								// CS_SIEGESIDE_DEFEND
	35,		17,		144,	48,									// CS_SIEGESIDE_ATTACK
};

// ���� ����
// 1 . ����
static INT g_iNpcDefense_CGATE[MAX_UPGRADE_LEVEL+1] = {			// ������ �� ���� ���� �� ���� ����� ����
	100,	// ���� - 0 �ܰ�
	180,	// ���� - 1 �ܰ�
	300,	// ���� - 2 �ܰ�
	520,	// ���� - 3 �ܰ�
};
// 2 . ����
static INT g_iNpcDefense_CSTATUE[MAX_UPGRADE_LEVEL+1] = {		// ������ �� ���� ���� �� ���� ����� ����
	80,		// ���� - 0 �ܰ�
	180,	// ���� - 1 �ܰ�
	340,	// ���� - 2 �ܰ�
	550,	// ���� - 3 �ܰ�
};

// ������ Y�� �ܰ�
#define		MAX_CASTLEWALL_STEP				3
const INT g_iCastleWallStepAxisY[MAX_CASTLEWALL_STEP] = {
	113,	// ���� - 1�ܰ�
	160,	// ���� - 2�ܰ�
	203,	// ���� - 3�ܰ�
};

// ������ �ε� �ܰ�
enum {
	CASTLESIEGE_DATALOAD_NONE				= 0,				// ���¾���
	CASTLESIEGE_DATALOAD_1					= 1,				// ���� ���� �����͸� �ε��ؾ� �Ѵ�.
	CASTLESIEGE_DATALOAD_2					= 2,				// ���� ������ �ε� �� DB ������ �ε� �ʿ���
	CASTLESIEGE_DATALOAD_3					= 3,				// DB ������ �ε� ��û �� ������ ��ٸ��� ����
	CASTLESIEGE_DATALOAD_4					= 4,				// db ������ �ε� �� ���� ��ٸ��� ����
};

// ������ ����
enum {
	CASTLESIEGE_STATE_NONE					= -1,				// ���¾���
	CASTLESIEGE_STATE_IDLE_1				= 0,				// ���ޱⰣ - 1
	CASTLESIEGE_STATE_REGSIEGE				= 1,				// ��û�Ⱓ (�����ֱ� ����)
	CASTLESIEGE_STATE_IDLE_2				= 2,				// ���ޱⰣ - 1
	CASTLESIEGE_STATE_REGMARK				= 3,				// ���� ��ϱⰣ
	CASTLESIEGE_STATE_IDLE_3				= 4,				// ���ޱⰣ - 2
	CASTLESIEGE_STATE_NOTIFY				= 5,				// ��ǥ�Ⱓ
	CASTLESIEGE_STATE_READYSIEGE			= 6,				// �غ�Ⱓ
	CASTLESIEGE_STATE_STARTSIEGE			= 7,				// ��������
	CASTLESIEGE_STATE_ENDSIEGE				= 8,				// �����Ⱓ
	CASTLESIEGE_STATE_ENDCYCLE				= 9,				// �����ֱ� ����
};

// ������ ������� ���� (OBJECTSTRUCT�� m_btCsJoinSide)
enum {
	CS_SIEGESIDE_NONE						= 0,				// ��/�� ���� - ���о���
	CS_SIEGESIDE_DEFEND						= 1,				// ��/�� ���� - ������
	CS_SIEGESIDE_ATTACK						= 2,				// ��/�� ���� - ������ (�� ���� �⺻���� �ؼ� ��帶�� +1 �� �����Ѵ�.)

#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
	CS_SIEGESIDE_ATTACK_2					= 3,
	CS_SIEGESIDE_ATTACK_3					= 4,
#endif
	
};

// ������ �ð��� ���� ���� (0 �̻��̸� ��)
enum {
	CASTLESIEGE_LEFTTIME_ONSIGE				= -1,				// ������ ���� ��
	CASTLESIEGE_LEFTTIME_ENDSIEGE			= -2,				// ������ ����
	CASTLESIEGE_LEFTTIME_NOTRUNNING			= -3,				// ������ ���� �ƴ�
};

// NPC ���翡 ���� ����
enum {
	NPC_LIVE_NONE							= 0,				// NPC �� �������� ����
	NPC_LIVE_READY							= 1,				// NPC �� ��Ƴ� �غ� (DB���� NPC �����͸� ���� ����)
	NPC_LIVE_OK								= 2,				// NPC �� �����ϰ� ����
};

// ������ NPC�� ����
enum {
	CS_NPC_TYPE_NONE						= 0,				// ������ NPC�� �ƴ�
	CS_NPC_TYPE_DB_DFN						= 1,				// ������ NPC - ������ DB
	CS_NPC_TYPE_INS_DFN						= 2,				// ������ NPC - ������
	CS_NPC_TYPE_INS_ATK						= 3,				// ������ NPC - ������
};

// ���� NPC ���׷��̵� ����
enum {
	CS_UPGRADE_TYPE_DEFENSE					= 1,				// ���׷��̵� - ����
	CS_UPGRADE_TYPE_REGENARATION			= 2,				// ���׷��̵� - ȸ����
	CS_UPGRADE_TYPE_MAXHITPOINT				= 3,				// ���׷��̵� - �ִ�HP
};

// ���� ����
enum {
	CS_TAXRATE_TYPE_CHAOS					= 1,				// ���� ���� - ī���� ����
	CS_TAXRATE_TYPE_STORE					= 2,				// ���� ���� - ����
	CS_TAXRATE_TYPE_HUNTZONE				= 3,				// ���� ���� - �����
};

// ������ ���� ����
enum {
	CS_GATE_STATE_CLOSED					= 0,				// ���� ������ ���� - ����
	CS_GATE_STATE_OPENED					= 1,				// ���� ������ ���� - ����
};


#ifdef CASTLE_GM_COMMAND_20041222								// ��� �������� ��ɼ�
enum {
	CS_GMCMD_CHANGEOWNERGUILD				= 0,				// "/�����������"
	CS_GMCMD_SETSTATE_REGSIEGE				= 1,				// "/�����������"
	CS_GMCMD_SETSTATE_REGMARK				= 2,				// "/����������"
	CS_GMCMD_SETSTATE_NOTIFY				= 3,				// "/��������"
	CS_GMCMD_SETSTATE_STARTSIEGE			= 4,				// "/��������"
	CS_GMCMD_SETSTATE_ENDSIEGE				= 5,				// "/��������"
	CS_GMCMD_SETSTATE_GETOWNERGUILD			= 6,				// "/�����������"
	CS_GMCMD_SETSTATE_GETCASTLESTATE		= 7,				// "/�����������"
	CS_GMCMD_SETSTATE_CHANGECSSIDE			= 8,				// "/������ü
};
#endif


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_SCHEDULE_DATA
//*---------------------------------------------------------------
// DESC : ������ ������ ������ ����ü
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_SCHEDULE_DATA {
	BOOL	m_bIN_USE;						// ���� �� ��ü�� ��������� ����
	INT		m_iSTATE;						// ������ ���¹�ȣ
	INT		m_iADD_DAY;						// ���ð� - ��
	INT		m_iADD_HOUR;					// ���ð� - ��
	INT		m_iADD_MIN;						// ���ð� - ��
	INT		m_iGAP_SEC;						// �ٷ� ���� �ֱ���� ���ð� (��)

	_CS_SCHEDULE_DATA			();				
	virtual ~_CS_SCHEDULE_DATA	();				

	VOID	Clear				();			// ������ �ʱ�ȭ
} CS_SCHEDULE_DATA, * LPCS_SCHEDULE_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_REG_GUILD_DATA
//*---------------------------------------------------------------
// DESC : ������ ������ ��� ������ ������ ����ü
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_REG_GUILD_DATA {
	BOOL	m_bIN_USE;						// ���� �� ��ü�� ��������� ����
	string	m_strGuildName;
	INT		m_iRegMarkCount;				// ��ϵ� �����
	INT		m_iGuildMemberCount;			// ����� ��
	INT		m_iGuildMasterLevel;			// ��帶���� ����
	INT		m_iSeqNum;						// ��� ����
	INT64	m_i64TotolScore;				// ���� �׸���� �����ؼ� ���� ����

	_CS_REG_GUILD_DATA			();
	virtual ~_CS_REG_GUILD_DATA	();

	VOID	Clear				();			// ������ �ʱ�ȭ
} CS_REG_GUILD_DATA, * LPCS_REG_GUILD_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_TOTAL_GUILD_DATA
//*---------------------------------------------------------------
// DESC : ������ ���� ��ü����� ������ ����ü
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_TOTAL_GUILD_DATA {
	BOOL	m_bIN_USE;						// ���� �� ��ü�� ��������� ����
	INT		m_iCsGuildID;					// ��/�� �� ����� ���� ID (1:���� / 2�̻�:����)
	BOOL	m_bGuildInvolved;				// �� ��尡 ���� ����� ����ΰ� ?
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
	INT		m_iGuildScore;					// �������� ������ �� ����� ����
#endif	

	_CS_TOTAL_GUILD_DATA			();
	virtual ~_CS_TOTAL_GUILD_DATA	();

	VOID	Clear					();		// ������ �ʱ�ȭ
} CS_TOTAL_GUILD_DATA, * LPCS_TOTAL_GUILD_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_MINIMAP_DATA
//*---------------------------------------------------------------
// DESC : ������ ���� �̴ϸ� ������ ����ü
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_MINIMAP_DATA {
	INT		m_iGuildMasterIndex;
	CHAR	m_szGuildName[16];
	INT		m_iMiniMapPointCount;
	PMSG_SENDMINIMAPDATA	m_stMiniMapPoint[MAX_MINIMAP_USER_COUNT];

	_CS_MINIMAP_DATA				();
	virtual ~_CS_MINIMAP_DATA		();

	VOID	Clear					();		// ������ �ʱ�ȭ
} CS_MINIMAP_DATA, * LPCS_MINIMAP_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_NPC_DATA
//*---------------------------------------------------------------
// DESC : ������ NPC ������ ������ ����ü
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_NPC_DATA {	
	// ���� NPC ���� ������
	BOOL	m_bIN_USE;						// ���� �� ��ü�� ��������� ����
	INT		m_iNPC_NUM;						// NPC ������ȣ
	INT		m_iNPC_INDEX;					// NPC ������ ���� �ε��� (�� �������� �����ϰ� ����)
	BOOL	m_bNPC_DBSAVE;					// DB������ �ؾ��ϴ��� �ƴ���
	INT		m_iNPC_SIDE;					// ��� �� ������ ���� (1 : ���� / 2 : ����)
	INT		m_iNPC_OBJINDEX;				// OBJECTSTURCT ����ü �迭������ �ε���

	// ��ȭ ������ ����
	INT		m_iNPC_DF_LEVEL;				// ���� ����
	INT		m_iNPC_RG_LEVEL;				// ȸ���� ����
	INT		m_iNPC_MAXHP;					// �ִ�ü��
	INT		m_iNPC_HP;						// ü��
	INT		m_iNPC_SX;						// ��ǥ - ���� X
	INT		m_iNPC_SY;						// ��ǥ - ���� Y
	INT		m_iNPC_DX;						// ��ǥ - ���� X
	INT		m_iNPC_DY;						// ��ǥ - ���� Y
	INT		m_iNPC_DIR;						// NPC ���� (-1�� ����)

	// �ʱ� ����
	INT		m_iNPC_BASE_DF_LEVEL;			// ���� ����
	INT		m_iNPC_BASE_RG_LEVEL;			// ȸ���� ����
	INT		m_iNPC_BASE_MAXHP;				// �ִ�ü��
	INT		m_iNPC_BASE_HP;					// ü��
	INT		m_iNPC_BASE_SX;					// ��ǥ - ���� X
	INT		m_iNPC_BASE_SY;					// ��ǥ - ���� Y
	INT		m_iNPC_BASE_DX;					// ��ǥ - ���� X
	INT		m_iNPC_BASE_DY;					// ��ǥ - ���� Y
	INT		m_iNPC_BASE_DIR;				// NPC ���� (-1�� ����)

	// Ư�� NPC���� ���� ����
	INT		m_iCS_GATE_LEVER_INDEX;			// �������� - ������ �ε����� (��ü ���� �� ���� �ѹ��� -1)

	BOOL	m_iNPC_LIVE;					// �����ϴ��� ����
	INT		m_iNPC_AUTH;					// NPC ���ٱ��� (������ ���)

	union {
		struct {
			BYTE	m_btCsNpcExistVal1;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� - 1 (1 ~ 255)
			BYTE	m_btCsNpcExistVal2;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� - 2 (0 ~ 255)
			BYTE	m_btCsNpcExistVal3;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� - 3 (0 ~ 255)
			BYTE	m_btCsNpcExistVal4;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� - 4 (0 ~ 255)
		};
		INT			m_iCsNpcExistVal;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� (0�̸� ��ȿ)
	};

	_CS_NPC_DATA			();				
	virtual ~_CS_NPC_DATA	();				

	VOID	Clear			();				// ������ �ʱ�ȭ
	VOID	SetBaseValue	();				// �⺻ �������� ������ ����
} CS_NPC_DATA, * LPCS_NPC_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : CLASS
// NAME : CCastleSiege
//*---------------------------------------------------------------
// DESC : ������ ������ ���� Ŭ����
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
class CCastleSiege  
{
public:
	CCastleSiege					();
	virtual ~CCastleSiege			();
	
	VOID	Clear					();
	BOOL	Ready					(INT iMapSvrGroup);
	BOOL	LoadPreFixData			(LPSTR lpszFileName);		// �������̰� �������� �����͸� ���� (commonserver.cfg)
	BOOL	LoadData				(LPSTR lpszFileName);		// �������� �����͸� ���� (MuCastleData.dat)
	BOOL	DataRequest				();	
	BOOL	Init					();
	VOID	Run						();
	
	static bool						ScheduleStateCompFunc		(CS_SCHEDULE_DATA & A, CS_SCHEDULE_DATA & B);

	// ���� �����͸� �а��ִ� ���� ����
	BOOL	GetDataLoadState		()							{	return m_iCastleDataLoadState;				}
	VOID	SetDataLoadState		(INT iDataLoadState)		{	m_iCastleDataLoadState	= iDataLoadState;	}

	// ���� �����͸� �о����� ����
	BOOL	GetFileDataLoadOK		()							{	return m_bFileDataLoadOK;					}
	VOID	SetFileDataLoadOK		(BOOL bFileDataLoadOK)		{	m_bFileDataLoadOK	= bFileDataLoadOK;		}

	// DB �����͸� �о����� ����
	BOOL	GetDbDataLoadOK			()							{	return m_bDbDataLoadOK;						}
	VOID	SetDbDataLoadOK			(BOOL bDbDataLoadOK)		{	m_bDbDataLoadOK	= bDbDataLoadOK;			}

	// DB NPC���� ���ʷ� ���� -> DB�� ����
	BOOL	FirstCreateDbNPC		();

	// DB �� ���� ������ ������ �޾Ƽ� �ʱ�ȭ -> [0x81] ��Ŷ
	// 1 . ��ü ������ ����
	BOOL	SetCastleInitData		(
			LPCSP_ANS_CSINITDATA lpMsg
			);
	// 2 . ���� NPC ������ ����
	BOOL	SetCastleNpcData		(
			LPCSP_CSINITDATA lpMsg, 
			INT iCOUNT
			);

	BOOL 	CheckSync				();							// ���� �������� �ð��� ���� �ð��� �������� ���� �ð� ��ũ�� �����. (���� ��Ȳ ����)
	VOID	GetNextDay				(							// ���� ��¥�� ���ϴ� �Լ�
				SYSTEMTIME &st, 
				INT iAfterDay,
				INT iAfterHour		= 0, 
				INT iAfterMin		= 0, 
				INT iAfterSec		= 0
				);	
#ifdef MODIFY_SIEGE_END_DATE_BUG_FIX_20060116
	VOID		GetStateDate			(INT iCastleSiegeState, SYSTEMTIME* st);	// Ư�� ������ ���� ���±����� �ð�(Day, Hour, Min)
#endif
	INT		GetStateGapSec			(INT iCastleSiegeState);	// Ư�� ������ ���� ���±����� �ð����� (SEC) �� ���Ѵ�. -> �̹� ���� ��ġ


	//===============================================
	// �������� ������ ���� ���� �޼����

	// 1> . ������ ������ ���� �޼����
	// 1 . Ư������ ����
	VOID	SetState				(INT iCastleSiegeState, BOOL bSetRemainMsec =TRUE);
	VOID	SetState_NONE			(BOOL bSetRemainMsec);
	VOID	SetState_IDLE_1			(BOOL bSetRemainMsec);
	VOID	SetState_REGSIEGE		(BOOL bSetRemainMsec);
	VOID	SetState_IDLE_2			(BOOL bSetRemainMsec);
	VOID	SetState_REGMARK		(BOOL bSetRemainMsec);
	VOID	SetState_IDLE_3			(BOOL bSetRemainMsec);
	VOID	SetState_NOTIFY			(BOOL bSetRemainMsec);
	VOID	SetState_READYSIEGE		(BOOL bSetRemainMsec);
	VOID	SetState_STARTSIEGE		(BOOL bSetRemainMsec);
	VOID	SetState_ENDSIEGE		(BOOL bSetRemainMsec);
	VOID	SetState_ENDCYCLE		(BOOL bSetRemainMsec);

	// 2 . Ư������ ����
	VOID	ProcState_NONE			();
	VOID	ProcState_IDLE_1		();
	VOID	ProcState_REGSIEGE		();
	VOID	ProcState_IDLE_2		();
	VOID	ProcState_REGMARK		();
	VOID	ProcState_IDLE_3		();
	VOID	ProcState_NOTIFY		();
	VOID	ProcState_READYSIEGE	();
	VOID	ProcState_STARTSIEGE	();
	VOID	ProcState_ENDSIEGE		();
	VOID	ProcState_ENDCYCLE		();

	// 2> . ������ ������� �޼����
	// 1 . ������ ���� ���� ���¹�ȯ
	INT			GetCastleState			()						{	return m_iCastleSiegeState;		}
	SYSTEMTIME	GetCastleLeftSiegeDate	()						{	return m_tmLeftCastleSiegeDate; }
	INT			CalcCastleLeftSiegeDate	();
	BOOL		GetCastleStateTerm		(						// ���� ������ ������ ����, �� �Ⱓ�� ���Ѵ�.
				SYSTEMTIME & tmStateStartDate, 
				SYSTEMTIME & tmStateEndDate
				);
	INT			GetCurRemainSec			();
	
	// 2 . NPC ����
	// NPC - ���� ����
	VOID	ClearDbNPC				();							// DB�� �����ϴ� NPC�� �� ���� -> ��������
	VOID	ClearNonDbNPC			();							// DB�� �������� �ʴ� NPC�� �� ���� -> ��������
	VOID	ClearNonDbNPC_MidWin	();							// DB�� �������� �ʴ� NPC�� �� �Ϻθ� �߰� �¸� �� ����
	VOID	ClearAllNPC				();							// ClearDbNPC() + ClearNonDbNPC()

	VOID	CreateDbNPC				();							// DB�� �����ϴ� NPC���� ��� ���� (NPC_LIVE_READY ������ ��)
	VOID	CreateNonDbNPC			(BOOL bDelFirst = TRUE);	// DB�� �������� �ʴ� NPC���� ��� ���� (���� ���� ���Ƿ� �ѹ� ������ ����)
	BOOL	CheckAddDbNPC			(							// DB�� �����ϴ� NPC�� �ϳ��� �߰� �������� üũ -> ���� �� DB���� �� (������ ���� �ƴҶ� ��)
			INT iIndex,
			INT iNpcType, 
			INT iNpcIndex,
			BYTE &btResult
			);
	BOOL	AddDbNPC				(							// DB�� �����ϴ� NPC�� �ϳ��� �߰� -> ������ ���� ��û �� �̹� �����
			INT iNpcType, 
			INT iNpcIndex
			);
	BOOL	RepairDbNPC				(							// DB�� �����ϴ� NPC�� �ϳ��� ���� -> �� ���� �̹� DB���� ������
			INT iNpcType, 
			INT iNpcIndex,
			INT iNpcHP,
			INT iNpcMaxHP
			);
#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
	BOOL	PayForUpgradeDbNpc		(							// DB�� �����ϴ� NPC�� �ϳ��� ���׷��̵��ϱ� ���� �ʿ��� ����� �����Ѵ�.
			INT iIndex,
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcUpType,
			INT	iNpcUpValue,
			INT	iNpcUpIndex
			);
#endif	
	BOOL	UpgradeDbNPC			(							// DB�� �����ϴ� NPC�� �ϳ��� ���׷��̵� -> �� ���� �̹� DB���� ���׷��̵� ��
			INT iIndex,
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcUpType,
			INT	iNpcUpValue,
			INT	iNpcUpIndex
			);
	BOOL	UpgradeDbNPC_DFLEVEL	(							// DB�� �����ϴ� Ư�� NPC�� ���� ���׷��̵�
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcDfLevel
			);
	BOOL	UpgradeDbNPC_RGLEVEL	(							// DB�� �����ϴ� Ư�� NPC�� ȸ���� ���׷��̵�
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcRgLevel
			);
	BOOL	UpgradeDbNPC_MAXHP		(							// DB�� �����ϴ� Ư�� NPC�� �ִ�HP ���׷��̵�
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcMaxHP
			);
	VOID	StoreDbNpc				();							// DB�� �����ϴ� NPC���� �ϰ������� DB�� ���� (���� ������ ����)
	BOOL	DelNPC					(							// ������ NPC�� ���� ���·� ���� -> DB NPC��� ���� �� DB���� �� (������ ���� �ƴҶ� ��)
			INT iIndex,
			INT iNpcType, 
			INT iMonsterExistVal,
			BOOL bDbSave = FALSE
			);
	VOID	SendNpcStateList		(							// Ư�� ������� Ư�� ������ ���� ���� ����Ʈ�� �����ش�.
			INT iIndex, 
			INT iNpcType
			);
	BOOL	GetNpcData				(							// ���� �����Ǵ� Ư�� NPC�� ���� ������ ���� (FALSE�� ������ ����)
			INT iNpcType, 
			INT iNpcIndex,
			CS_NPC_DATA & pRetNpcData							// ����� ��ȯ�Ǵ� NPC ������
			);
	VOID	AdjustDbNpcLevel		();							// DB NPC�� ���� �ڷ���� ����/ȸ���� ������ ����� ��ü�� �����Ѵ�. -> ���� ���� ������ �̷��� ���־�� �Ѵ�.
	
	// NPC - ����, ���� ����
	VOID	SendCsGateStateViewPort	(							// ���� �� ���¸� ���� ���� �ִ� ����鿡�� ���� ����
			INT iGateIndex,
			BYTE btOperate
			);
	VOID	CreateAllCsGateLever	();							// ���� ������ ������ ���������� ���� ���Ŀ� ������ �ϰ������� ������ �ش�.
	INT		CreateCsGateLever		(							// Ư�� ������ ������ ������ ����� �ε����� �����Ѵ�. (���д� -1)
			INT iX, 
			INT iY
			);
	BOOL	LinkCsGateLever			(							// �̹� ������ ������� �ִٸ� Ư�� ������ �����Ѵ�.
			INT iLeverIndex,
			INT iGateIndex
			);
	BOOL	CheckLeverAlive			(							// Ư�� �ε����� ���������̰� ����ִ����� Ȯ��
			INT iLeverIndex
			);
	BOOL	CheckCsGateAlive		(
			INT iGateIndex
			);
	VOID	CheckCsDbNpcAlive		();							// �������� ����ִ��� Ȯ���Ͽ� �׾��ٸ� ���Ӽ� ����
	
	// NPC - �հ� ����
	VOID	SetCrownIndex			(INT iIndex)				{	m_iCrownIndex	= iIndex;					}	// �հ� NPC �ε����� �����Ѵ�. -> �հ� ��ȣ�� ���� ������ ���� �ʿ�
	VOID	NotifyCrownState		(BYTE btState);				// ���� �հ��� ���¸� �ֺ��� �˸� (0:��ȣ������/1:��ȣ������/2:��ϼ���)
	BOOL	GetRegCrownAvailable	()							{	return m_bRegCrownAvailable;				}
	VOID	SetRegCrownAvailable	(BOOL bRegCrownAvailable)	{	m_bRegCrownAvailable = bRegCrownAvailable;	}
	BOOL	GetCastleTowerAccessable()							{	return m_bCastleTowerAccessable;			}
	VOID	SetCastleTowerAccessable(BOOL bTowerAccessable)		{	m_bCastleTowerAccessable =bTowerAccessable;	}
	
#ifdef MODIFY_CASTLESIEGE_NOTIFY_CROWNSWITCH_USER_20051017
	// NPC - �հ� ����ġ ����
	VOID	NotifyCrownSwitchInfo	(INT iCrownSwitchIndex);
#endif


	// 3 . ���ӿ����
	BOOL	DelGemOfDefend			(							// Ư�� ����ڿ��Լ� Ư�� ������ ��ȣ������ �����Ѵ�.
			INT iIndex,
			INT iNeedGemOfDefend
			);
	VOID	SetTaxRate (										// ������ �����ϰ� �ٸ� ���Ӽ����� ����ȭ �Ѵ�.
			INT iTaxType,
			INT iTaxRate
			);
	BOOL	CheckCastleHasMoney		(							// ���� ���� ���� ������ �ִ��� Ȯ���Ѵ�.
			INT64 i64CastleMoney
			);
	INT64	GetCastleMoney			()							{	return m_i64CastleMoney;	}
	VOID	SetCastleMoney			(							// ���� ���� ���� Ư�� �׼��� �����.
			INT64 i64CastleMoney
			);
	VOID	ResetCastleTaxInfo		();							// ���� ���� ���������� �ʱ�ȭ �Ѵ�.
	VOID	SetIsSiegeGuildList		(BOOL bIsSiegGuildList)		{	m_btIsSiegeGuildList = bIsSiegGuildList;	}
	VOID	SendCastleStateSync		();							// ���� ���� ���¸� �ʼ����� ��ü�� ����ȭ �Ѵ�.
	BOOL	GetDbNpcCreated			()							{	return m_bDbNpcCreated;						}
	VOID	SetDbNpcCreated			(BOOL bDbNpcCreated)		{	m_bDbNpcCreated	= bDbNpcCreated;			}
	BOOL	CheckGuildOwnCastle		(LPSTR lpszGuildName);		// �ش� ��尡 ���� ������ ��尡 �´��� Ȯ���Ѵ�.
	VOID	ResetCastleCycle		();							// ���� �ֱ⸦ �ٽ� �����ϸ鼭 �ʿ��� �����͵��� �����Ѵ�.
	VOID	SetGateBlockState		(							// ������ ��� �Ӽ��� �����Ѵ�.
			INT iX, 
			INT iY, 
			INT iGateState
			);
	LPSTR	GetCastleOwnerGuild		()							{	return m_szCastleOwnerGuild;				}
	BOOL	GetHuntZoneEnter		()							{	return m_bEnterHuntZone;					}
	VOID	SetHuntZoneEnter		(BOOL bEnterHuntZone)		{	m_bEnterHuntZone	= bEnterHuntZone;		}
	
	INT		GetTaxRateChaos			()							{	return m_iTaxRateChaos;						}
	INT		GetTaxRateStore			()							{	return m_iTaxRateStore;						}
	INT		GetTaxHuntZone			()							{	return m_iTaxHuntZone;						}

	// *> . ������ ����
	// ������� ���� üũ
	VOID	CheckBuildCsGuildInfo	();							// �������� ��ü��� ������ �����ϱ� ���� ��Ȳ1 �� 2 �� �����Ѵ�.
	// ������� ���� 1 - ���� ���¿��� ���� ����
	VOID	SetCalcRegGuildList		(							// ������ �������� �ĺ� �����͸� �����Ѵ�. (���� �����Ͱ� �־��ٸ� �ʱ�ȭ �Ѵ�.)
			LPCSP_CALCREGGUILDLIST lpMsg,
			INT iCOUNT
			);
	VOID	MakeCsTotalGuildInfo	(							// ��/���� ���ձ�� ������ �޾Ƽ� ���� ��������� �Բ� �Ϻ��� ��������� �����Ѵ�.
			LPCSP_CSGUILDUNIONINFO lpMsg,
			INT iCOUNT
			);
	VOID	SaveCsTotalGuildInfo	();							// ��/���� ��ü��� ������ DB�� �����ϵ��� ��û�Ѵ�.

	// ������� ���� 2 - �ִ� ���¿��� DB���� �޾Ƽ� �ٷ� ����
	VOID	SetCsTotalGuildInfo		(							// ��/���� ���ձ�� ������ �޾Ƽ� ���� ��������� �Բ� �Ϻ��� ��������� �����Ѵ�.
			LPCSP_CSLOADTOTALGUILDINFO lpMsg,
			INT iCOUNT
			);
	// ��� ��/�� ������ ���� ����
	BOOL	GetCsJoinSide			(							// Ư�� ����� ���� ��/�� ���� Ÿ���� ��ȯ�ϰ� ������Ŷ�� �����ش�.
			LPSTR lpszGuildName,
			BYTE &btCsJoinSide,
			bool &bCsGuildInvolved
			);
	VOID	SetAllUserCsJoinSide	();							// �������� ��� ����� ���� ������ ��/�� ���� ���ΰ��� �������ش�.
	VOID	ResetAllUserCsJoinSide	();							// �������� ��� ����� ���� ������ ��/�� ���� ���ΰ��� �������ش�.
	VOID	NotifySelfCsJoinSide	(INT iIndex);				// Ư�� ������� ��/�� ���� ���ΰ��� �����ش�.

	// *> . ��������
	BOOL	OperateGate				(							// �ܺ� ����ڰ�ü �ε����� ������ �۵���Ų��.
			INT		iObjIndex,
			INT		iMonsterExistVal,
			BOOL	bOpenType									// (0:�ݱ�/1:����)
			);
	BOOL	OperateGate				(							// ���� NPC���� �ε����� ������ �۵���Ų��.
			INT		iNpcIndex,
			BOOL	bOpenType									// (0:�ݱ�/1:����)
			);

	// *> . �������� ����
	BOOL	CheckAttackGuildExist	();							// ������ ��尡 �����ϴ��� Ȯ���Ѵ�.
	BOOL	CheckCastleOwnerMember	(INT iIndex);				// �ش� ����ڰ� ���� ������ ����� �Ҽ����� Ȯ���Ѵ�.
	BOOL	CheckCastleOwnerUnionMember		(					// Ư�� ����ڰ� �ڽ��� ���� ��忬���� �Ͽ����� Ȯ���Ѵ�.
			INT iIndex
			);
	BOOL	CheckGuardianStatueExist();							// ��ȣ������ �����ϴ��� Ȯ���Ѵ�.
	VOID	ChangeWinnerGuild		(INT iCsJoinSide);			// ������ ��带 �ٲ۴� -> �߰� �¸� �� ���
	BOOL	CheckMiddleWinnerGuild	();							// ����!!> . ������ �߰��¸� ��尡 ��Ÿ������ Ȯ���Ѵ�.
	BOOL	CheckCastleSiegeResult	();							// ����!!> . ������ ��������� üũ�Ѵ�.

	BOOL	CheckUnionGuildMaster	(INT iIndex);				// Ư�� ����ڰ� �ڽ��� ���� ��忬���� ���� ���������� Ȯ���Ѵ�.

	VOID	SetCrownUserIndex		(INT iIndex)				{	m_iCastleCrownAccessUser = iIndex;		}
	INT		GetCrownUserIndex		()							{	return m_iCastleCrownAccessUser;		}
	VOID	ResetCrownUserIndex		()							{	m_iCastleCrownAccessUser	= -1;		}

	INT		GetCrownAccessUserX		()							{	return m_btCastleCrownAccessUserX;		}
	INT		GetCrownAccessUserY		()							{	return m_btCastleCrownAccessUserY;		}
	VOID	SetCrownAccessUserX		(BYTE iX)					{	m_btCastleCrownAccessUserX	= iX;		}
	VOID	SetCrownAccessUserY		(BYTE iY)					{	m_btCastleCrownAccessUserY	= iY;		}

	VOID	SetCrownSwitchUserIndex		(
			INT iMonsterClass, 
			INT iIndex
			);
	INT		GetCrownSwitchUserIndex		(
			INT iMonsterClass
			);
	VOID	ResetCrownSwitchUserIndex	(
			INT iMonsterClass
			);

#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
	DWORD	GetCrownAccessTickCount()							{	return m_dwCrownAccessTime;				}
#endif

	VOID	SetCrownAccessTickCount		();						// �հ� ���� �ð��� �����Ѵ�.
	VOID	ResetCrownAccessTickCount	();						// �հ� ���� �ð��� �ʱ�ȭ�Ѵ�.

	// *> . ���� ������ �κ��� � ����
	INT		CheckOverlapCsMarks			(INT iIndex);			// Ư�� ������� �κ��丮�� ������ ǥ���� ��ĥ �� �ִٸ� ���° ĭ���� �˷��ش�.
	INT		GetCsAttkGuildList			(						// ���� ���� ����� ����Ʈ�� �����´�. [0xB5] (��� - 0:����/1:����/2:���Ѿʵ�/3:�����;���)
			LPPMSG_CSATTKGUILDLIST lpMsgBody,
			INT & iCount
			);
	BOOL	CheckTeleportMagicAxisY		(						// �ڷ���Ʈ ���� ���� �� ������ �ɸ��� �ʴ��� üũ�Ѵ�.
			INT iStartY,
			INT iTargetX,
			INT iTargetY
			);
	VOID	NotifyCsSelfLeftTime		(						// ������ �������� ���� �ð��� �����ش�.
			INT iIndex
			);

	// *> . ���� �� ����, � ����
	VOID	CheckReviveNonDbNPC			();						// DB�� �������� �ʴ� NPC�� �����Ѵ�.
	VOID	CheckReviveGuardianStatue	();						// ��ȣ���� �� ���� �׾��ִ� ���� �ִٸ� DB���� ���� �ٷ� �߰���Ų��.
	VOID	ReSpawnAllUser				();						// ���� ������ �ȿ� �ִ� ��� ������� ������ ���ҿ� �°� ������ ��ġ�� ���� �̵� ��Ų��.
	VOID	ReSpawnEnemyUser			(						// ���� ������ �ȿ� �ִ� �������� �ƴ� ��� ������� �� ������ ���� �̵� ��Ų��.
			BOOL bRefreshOwnerUser = FALSE
			);
	VOID	NotifyAllUserCsStartState	(BYTE btStartState);	// ���� �������� ���۵Ǿ����� ���θ� ��� ����ڵ鿡�� �˸���.
	VOID	NotifyAllUserCsProgState	(						// ���� �������� ���� ���¸� ��� ����ڵ鿡�� �˸���.
			BYTE btProgState,
			LPSTR lpszGuildName
			);
	VOID	ClearCastleTowerBarrier		();						// ���� �����ϴ� ��� �� NPC�� �����Ѵ�.
	VOID	SetAllCastleGateState		(BOOL bOpenType);		// ��� �����ϴ� ������ ���¸� �����Ѵ�.
	// ���� � - �̴ϸ� ����
	VOID	AddMiniMapDataReqUser		(INT iIndex);			// �̴ϸ� ������ ��û �ο��� ����Ѵ�.
	VOID	DelMiniMapDataReqUser		(INT iIndex);			// �̴ϸ� ������ ��û �ο��� �����Ѵ�.
	VOID	OperateMiniMapWork			();						// �̴ϸ� ���� �۾��� �����Ѵ�. -> ������ ���� ����


	// *> . ����, ������ ���� ����
	VOID	SendMapServerGroupMsg	(							// �ʼ��� �׷� ��ü�� ����ڿ��� Ư�� �޽����� ������.
			LPSTR lpszMsg 
			);
	VOID	SendAllUserAnyData		(							// ������ ������ �ִ� ��� ����ڵ鿡�� Ư�� �����͸� ������.
			LPBYTE lpMsg, 
			INT iSize
			);
	VOID	SendAllUserAnyMsg		(							// ������ ������ �ִ� ��� ����ڵ鿡�� Ư�� �޽����� ������.
			LPSTR lpszMsg, 
			INT iType	= 1										// �޽��� Ÿ�� (1-�����̵�/2-�߾Ӱ���)			
			);
	VOID	SendCsUserAnyData		(							// �������ʿ� ������ �ִ� ��� ����ڵ鿡�� Ư�� �����͸� ������.
			LPBYTE lpMsg, 
			INT iSize
			);
	VOID	SendCsUserAnyMsg		(							// �������ʿ� ������ �ִ� ��� ����ڵ鿡�� Ư�� �޽����� ������.
			LPSTR lpszMsg
			);

	static bool						RegGuildScoreCompFunc		(CS_REG_GUILD_DATA & A, CS_REG_GUILD_DATA & B);

#ifdef CASTLE_GM_COMMAND_20041222								// ��� �������� ���ó��
	VOID	OperateGmCommand		(
			INT iIndex,
			INT iGmCommand,
			LPVOID lpParam	= NULL
			);
	VOID	CreateDbNPC_INS			(							// ����, ���� �� DB NPC�� DB ���� ���� �ٷ� �����.
			);
#endif
	
#ifdef CASTLE_SAVE_PCROOM_USERLIST_20050105						// ������ �����ڵ��� PC�� ���� ����
	VOID	SavePcRoomUserList		();
#endif

private:
	BOOL						m_bCastleSiegeEnable;			// �������� ��Ȱ��ȭ �Ǵ°� ?
	
	BOOL						m_bFileDataLoadOK;				// ���� �����͸� ����� �о����� ���� ?
	BOOL						m_bDbDataLoadOK;				// DB �����͸� ����� �о����� ���� ?

	BOOL						m_bDoRun;						// ���� �������� ������ �� �ִ��� ����
	BOOL						m_iMapSvrGroup;					// �� ���� �����ִ� �ʼ����� �ڵ��ȣ
	
	INT							m_iCastleDataLoadState;			// �������� �������� ���� �ε� ���� ����
	DWORD						m_dwDbDataLoadTickCount;		// DB �����͸� �ε��ϱ� ���� ƽī��Ʈ

	INT							m_iCastleSiegeCycle;			// �����ֱ�
	INT							m_iCastleSiegeState;			// ��������

#ifdef MODIFY_SIEGE_END_DATE_BUG_FIX_20060116
	SYSTEMTIME					m_tmSiegeEndSchedule;			// ���� ���� ���� �ð�
#endif

	BOOL						m_bFixCastleCycleStartDate;		// �����ֱ� �ð� �������� ���� (commonserver.cfg)
	SYSTEMTIME					m_tmFixCastleCycleStartDate;	// �����ֱ� �ð� ��������

	INT							m_iFixCastleSpecificState;		// ���� ���� ����� Ư������
	BOOL						m_bFixCastleStateStartDate;		// ���� Ư������ �ð� �������� ���� (commonserver.cfg)
	SYSTEMTIME					m_tmFixCastleStateStartDate;	// ���� Ư������ �ð� ��������

	INT							m_iCS_REMAIN_MSEC;				// ���� �ֱ���� ���� �ð� (MSEC)
	INT							m_iCS_TICK_COUNT;				// ī��Ʈ �۾����� �ϱ����� �ð� (1/1000��)

	SYSTEMTIME					m_tmStateStartDate;				// Ư�� ���°� ������ �ð�
	SYSTEMTIME					m_tmLeftCastleSiegeDate;		// ���� ���� ���� ���۽ð�
	
	DWORD						m_dwNPC_DBSAVE_TICK_COUNT;				// NPC�� DB�� ����� �ð��� üũ�ϴµ� ��� (N�� ���� �ѹ���)
	DWORD						m_dwCALC_LEFTSIEGE_TICK_COUNT;			// ������ ���۱��� ���� �ð��� ����ϴµ� ��� (N�� ���� �ѹ���)
	DWORD						m_dwEVENT_MSG_TICK_COUNT;				// �̺�Ʈ �޽����� ��µ� �ð��� üũ�ϴµ� ���
	DWORD						m_dwCHECK_GATE_ALIVE_COUNT;				// ����Ʈ�� �����ϴ��� üũ�ϴµ� ���
	DWORD						m_dwCS_JOINSIDE_REFRESH_TICK_COUNT;		// ���� ���� ���� ��/�� �������¸� ������ �ִµ� ���
	DWORD						m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT;	// ���� ���� ���� ��/�� �������¸� ������ �ִµ� ���
	DWORD						m_dwCS_MINIMAP_SEND_TICK_COUNT;			// ������ �� �̴ϸ� �����͸� ������ �ִµ� ���
	DWORD						m_dwCS_LEFTTIME_TICK_COUNT;				// ������ �� ���� �ð��� ��� ����ڵ鿡�� �˸��µ� ���
	DWORD						m_dwCS_STARTTIME_TICK_COUNT;			// ������ ������ ���� �ð��� ��� ����ڵ鿡�� �˸��µ� ���


	// ������ ���� �����͵�
	// 1 . �⺻ ������
	SYSTEMTIME					m_tmStartDate;					// �����ֱ� ������
	SYSTEMTIME					m_tmEndDate;					// �����ֱ� ������
	BYTE						m_btIsSiegeGuildList;			// �������� ��������� �̹� ����Ǿ��°� ?
	BYTE						m_btIsSiegeEnded;				// ������ �̹� ġ��� ���°� ?
	BYTE						m_btIsCastleOccupied;			// ���� �̹� ����ڵ鿡�� ������ �����ΰ� ?
	CHAR						m_szCastleOwnerGuild[16];		// ĳ���� ������ ���� (������ "")
	INT64						m_i64CastleMoney;				// ���� ���� ���� ��
	INT							m_iTaxRateChaos;				// ī���� ���� ����
	INT							m_iTaxRateStore;				// ���� ����
	INT							m_iTaxHuntZone;					// ����� �����
	BOOL						m_bEnterHuntZone;				// ����� ���� ���ɿ���

	// 2 . ������ ������
	vector <CS_SCHEDULE_DATA>	m_vtScheduleData;				// ������ ������ ���� ����
	CRITICAL_SECTION			m_critScheduleData;

	// 3 . NPC ������
	vector <CS_NPC_DATA>		m_vtNpcData;					// ������ NPC������ ����
	CRITICAL_SECTION			m_critNpcData;
	BOOL						m_bDbNpcCreated;				// DB���� �о�� NPC���� �����Ͽ��°� ? (SetState() �� ���� �ѹ��� ���ش�.)

	// 4 . ���� ����� ��� ������
	map <string, CS_TOTAL_GUILD_DATA>	m_mapCsBasicGuildInfo;	// ������ �� �� ������� (�����) �� ��/�� ���� ������ ����
	CRITICAL_SECTION			m_critCsBasicGuildInfo;
	map <string, CS_TOTAL_GUILD_DATA>	m_mapCsTotalGuildInfo;	// ������ �� ��� ������� (��������) �� ��/�� ���� ������ ����
	CRITICAL_SECTION			m_critCsTotalGuildInfo;
	BOOL						m_bCsBasicGuildInfoLoadOK;		// ���� ������ ����� ����� �����Ͱ� ����Ǿ����� ����
	BOOL						m_bCsTotalGuildInfoLoadOK;		// �������� ����� �������� ��� ������� �����Ͱ� ����Ǿ����� ����

	// 5 . �̴ϸ� ������
	map <INT, CS_MINIMAP_DATA>	m_mapMiniMapData;				// �̴ϸ� �����͸� ��Ƴ��� ��
	CRITICAL_SECTION			m_critCsMiniMap;				// �̴ϸ� ������ ��ȣ (��/���� �Բ� ���ÿ� ��ȣ)
	vector <INT>				m_vtMiniMapReqUser;				// �̴ϸ� �����͸� ��û�ϴ� ����ڵ�

	// 6 . ������ �� �ʿ��� ������
	BOOL						m_bCastleTowerAccessable;		// �������� ���� 2������ �ö� �� �ִ°� ? (��ȣ���� ���� ��)
	BOOL						m_bRegCrownAvailable;			// �հ� ���ε���� �����Ѱ� ? (��ȣ���� �����Ǿ��� ?)
	INT							m_iCrownIndex;					// �հ��� �ε���
	INT							m_iCastleCrownAccessUser;		// �հ��� ������ �����
	BYTE						m_btCastleCrownAccessUserX;		// �հ��� ������ ������� X��ǥ
	BYTE						m_btCastleCrownAccessUserY;		// �հ��� ������ ������� Y��ǥ
	INT							m_iCastleSwitchAccessUser1;		// �հ� ����ġ 1 �� ������ �����
	INT							m_iCastleSwitchAccessUser2;		// �հ� ����ġ 2 �� ������ �����
	DWORD						m_dwCrownAccessTime;			// �հ� ���� �ð� (0 �� �⺻)
	CHAR						m_szMiddleWinnerGuild[16];		// ������ �߰� �¸� ����� �̸��� �����Ѵ�. -> ������ ������ �¸���尡 �ȴ�.
};


extern CCastleSiege	g_CastleSiege;


#endif


#endif // !defined(AFX_CASTLESIEGE_H__6C5A97C8_FFFB_42E4_8F56_ABE80C670758__INCLUDED_)
