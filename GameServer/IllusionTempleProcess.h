// IllusionTempleProcess.h: interface for the CIllusionTempleProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ILLUSIONTEMPLEPROCESS_H__9C575967_0401_4599_940B_6D73FB6C9D2E__INCLUDED_)
#define AFX_ILLUSIONTEMPLEPROCESS_H__9C575967_0401_4599_940B_6D73FB6C9D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#include <list>
#include "PartyClass.h"


#define CHECK_ILLUSION_TEMPLE(C_VALUE)		(C_VALUE<45?FALSE:C_VALUE>50?FALSE:TRUE)

#define MAX_ILLUSION_TEMPLE_COUNT			6		// ȯ�� ��� ��

#ifndef ADD_SEASON_3_NEW_MAP_20070910
#define MAP_INDEX_ILLUSION_TEMPLE1			45		// �� 1 ȯ�� ��� �ʹ�ȣ
#endif

#define GATE_ILLUSION_TEMPLE1				142		// ��1 ȯ����� ���� �̵�
#define GATE_ILLUSION_TEMPLE_ALLIED1		148		// ��1 ȯ����� ���ձ��� ��ġ�� �̵�
#define GATE_ILLUSION_TEMPLE_ILLUSION1		154		// ��1 ȯ����� ȯ�������� ��ġ�� �̵�

#define MAX_ILLUSION_TEMPLE_USER_COUNT		10		// �ִ� ���� ��

#if TESTSERVER == 1	// �׽�Ʈ �����ϰ�� �Ѹ� ���� ����
#define MIN_ILLUSION_TEMPLE_USER_COUNT		2		// �ּ� ���� ��
#else
#define MIN_ILLUSION_TEMPLE_USER_COUNT		4		// �ּ� ���� ��
#endif

#define MAX_ILLUSION_TEMPLE_STATUS_COUNT	2		// �ִ� ���� ��
#define MAX_ILLUSION_TEMPLE_MONSTER_COUNT	40		// �ִ� ���� ��

#define MAX_ILLUSION_TEMPLE_SKILL_POINT		90		// �ִ� ��ų ����Ʈ

#define MAX_ILLUSION_TEMPLE_KILL_COUNT		255		// �ִ� ųī��Ʈ
#define MAX_ILLUSION_TEMPLE_USE_SKILL_COUNT	255		// �ִ� ��ų �� ��Ƚ��
#define MAX_ILLUSION_TEMPLE_RELICE_MOVE_COUNT	255	// �ִ� ���� ��� Ƚ�� 

#define DEFAULT_SKILL_POINT					10		// �ʱ� ��ų����Ʈ ��
#define INC_SKILL_POINT_CHARACTER			5		// �÷��̾� ��ɽ� ��� ��ų ����Ʈ
#define INC_SKILL_POINT_MONSTER				2		// ���� ��ɽ� ��� ��ų ����Ʈ

#define ILLUSION_TEMPLE_MIN_WIN_POINT		2		// �¸��ϱ� ���� �ּ� ����
#define ILLUSION_TEMPLE_MAX_WIN_POINT		7		// �¸��ϱ� ���� �ִ� ����

#define MIN_USE_SKILL_COUNT					5		// ����ġ�� ������� �ּ� ��ų ���� 

#ifndef UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710
#define MIN_USE_SKILL_COUNT					5		// ����ġ�� ������� �ּ� ��ų ���� 
#endif

// �̺�Ʈ ����
#define ILLUSION_TEMPLE_STATE_NONE			0
#define ILLUSION_TEMPLE_STATE_READY			1
#define ILLUSION_TEMPLE_STATE_PLAYING		2
#define ILLUSION_TEMPLE_STATE_END			3

#define ILLUSION_TEMPLE_SKILL_PRODECTION_TIME			15		// ��ȣ�� �ֹ� ���� �ð�
#define ILLUSION_TEMPLE_SKILL_RESTRAINT_TIME			15		// �ӹ��� �ֹ� ���� �ð�

// �̺�Ʈ (ILLUSION_TEMPLE_STATE_PLAYING)���� ����
enum __EVENT_PLAY_INFO
{
	ILLUSION_TEMPLE_PLAY_START		= 0,
	ILLUSION_TEMPLE_PLAY_PLAYING	= 1,
	ILLUSION_TEMPLE_PLAY_END		= 2,
};

// ���� ���ýð�
#define ILLUSION_TEMPLE_RELICS_LOOT_TIME			5

// ȥ�� ���� �ð�
#define ILLUSION_TEMPLE_CHAOSGEM_LOOT_TIME		5*60*1000

// ȯ�� ������� ĳ���� ������ ���� ���
#define ALLIED_CHARACTER				404	
#define ILLUSION_CHARACTER				405	

// NPC
enum __ILLUSION_TEMPLE_NPC
{
	NPC_ILLUSION_HOLY_RELICS_STATUE		= 380,		// ����
	NPC_LEADER_OF_ALLIED_FORCE			= 381,		// �¿��ձ�����
	NPC_ELDER_OF_ILLUSION				= 382,		// ȯ���������
	NPC_HOLY_RELICS_BOX_ALLIED			= 383,		// ����������(���ձ�)
	NPC_HOLY_RELICS_BOX_ILLUSION		= 384,		// ����������(ȯ����)
	NPC_PLATFORM_MIRAGE					= 385,		// �̶���
};

// Monster
enum __ILLUSION_TEMPLE_MONSTER
{
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_A1	= 386,	
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_B1	= 387,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_C1	= 388,

	MON_GHOST_OF_ILLUSION_BROTHERHOOD_A2	= 389,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_B2	= 390,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_C2	= 391,

	MON_GHOST_OF_ILLUSION_BROTHERHOOD_A3	= 392,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_B3	= 393,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_C3	= 394,

	MON_GHOST_OF_ILLUSION_BROTHERHOOD_A4	= 395,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_B4	= 396,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_C4	= 397,

	MON_GHOST_OF_ILLUSION_BROTHERHOOD_A5	= 398,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_B5	= 399,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_C5	= 400,

	MON_GHOST_OF_ILLUSION_BROTHERHOOD_A6	= 401,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_B6	= 402,
	MON_GHOST_OF_ILLUSION_BROTHERHOOD_C6	= 403,
};

// ��
enum __ILLUSION_TEMPLE_TEAM
{
	TEAM_ALLIED		= 0,
	TEAM_ILLUSION	= 1,	
};

// ���� ���� �ּ� ���� 
enum __ENABLE_ENTER_USER_LEVEL
{
	MIN_ENTER_LEVEL1	=	220,
	MIN_ENTER_LEVEL2	=	271,
	MIN_ENTER_LEVEL3	=	321,
	MIN_ENTER_LEVEL4	=	351,
	MIN_ENTER_LEVEL5	=	381,
	MIN_ENTER_LEVEL6	=	400,
};

// ���� ��ȭ ���� ������ �ð� �˸� �޽���
enum {
	TEMPLE_MSG_BEFORE_READY	= 14,					// [0x92] �غ� 30������ �޽��� Ÿ��
	TEMPLE_MSG_BEFORE_PLAY	= 15,					// [0x92] ������ 30������ �޽��� Ÿ��
};

// ��޺� �⺻ ����ġ ����(A)
const int g_IllusionTemple_Default_Exp [MAX_ILLUSION_TEMPLE_COUNT] =
{
	300000,
	340000,
	380000,
	420000,
	460000,
	500000,
};

// ���� ȹ�� ������ ���� ����ġ(B)
const int g_IllusionTemple_Relics_Point_Exp[MAX_ILLUSION_TEMPLE_COUNT]=
{
	50000,
	60000,
	70000,
	80000,
	90000,
	100000,
};

// �÷��̾ ���� ���� ���� ������(C)
const int g_IllusionTemple_Add_Kill_Exp [MAX_ILLUSION_TEMPLE_COUNT] = 
{
	600, 
	1200, 
	1800, 
	2400, 
	3000, 
	3500,
};

// ���� ��ݿ� ���� ������(D)
const int g_IllusionTemple_Move_Relics_Exp [MAX_ILLUSION_TEMPLE_COUNT] = 
{
	1200, 
	2400, 
	3600, 
	4800, 
	6000, 
	7200,
};

// �÷��� ���� ���� ���� ����
const int g_IllusionTemple_PartyUser_Point[MAX_PARTYUSER] = 
{
	10,
	20,
	30,
	40,
	50,
};

// ȯ�� ����� ������ ����ڵ��� ����
typedef struct __ILLUSION_TEMPLE_USER_DATA
{
	INT		m_nIndex;								// ������ ������� �ε���
	BYTE	m_btTeam;								// ����� ��

	BYTE	m_btSkillPoint;							// ��ų����Ʈ
	
	// ��ų
	DWORD	m_dwSkillProdectionTime;				// ��ȣ�� �ֹ� ���� �ð�
	DWORD	m_dwSkillRestraintTime;					// �ӹ��� �ֹ� ���� �ð�
	
	BYTE	m_btUserKillCount;						// PK��
	BYTE	m_btMonsterKillCount;					// ���� ų��
	BYTE	m_btRelicsMoveCount;					// ���� ��� Ƚ�� 
	BYTE	m_btUseSkillCount;						// ��ų ��� Ƚ��

#ifdef BUGFIX_GAIN_EXP_20071210
	INT64	m_nRewardExp;							// ���� ����ġ
#else
	int		m_nRewardExp;							// ���� ����ġ
#endif
	int		m_nRankingPoint;						// ��ŷ ����Ʈ
	BOOL	m_bReward;								// ������ �޾Ҵ��� ����
#ifdef MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
	int		m_btItemPos;							// Ƽ��(���� �η縶��, ���� �����) ������ ��ġ
#else // MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
	BYTE	m_btItemPos;							// Ƽ��(���� �η縶��, ���� �����) ������ ��ġ
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617	
#endif // MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826

	__ILLUSION_TEMPLE_USER_DATA()
	{
		m_nIndex		= -1;
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
		m_btItemPos		= -1;
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
	}
} ILLUSION_TEMPLE_USER, *LPILLUSION_TEMPLE_USER;

// ������Ʈ ����
typedef struct __ILLUSION_TAMPE_OBJECT_DATA
{
	int m_nIndex;		// ������Ʈ �ε���	
	short m_wPosNum;	// ������Ʈ ��ġ ���̺� ��ȣ
} ILLUSION_TEMPLE_OBJECT;

class CIllusionTempleProcess  
{
public:
	CIllusionTempleProcess();
	virtual ~CIllusionTempleProcess();

private:
	CRITICAL_SECTION	m_critUserData;										// m_UserData[] �� ������ ������� ��ȣ�ϱ� ����
	ILLUSION_TEMPLE_USER	m_UserData[MAX_ILLUSION_TEMPLE_USER_COUNT];		// ����� ����ü
	DWORD				m_dwViewSkillState[MAX_ILLUSION_TEMPLE_USER_COUNT];	// ��ų ����
	int		m_nUserCount;

	BYTE	m_nTempleNumber;					// ȯ�� ��� ��ȣ
	BYTE	m_btIllusionTempleState;			// �̺�Ʈ ����
	
	ILLUSION_TEMPLE_OBJECT	m_StatusData[MAX_ILLUSION_TEMPLE_STATUS_COUNT];		// ���� ����Ÿ
	ILLUSION_TEMPLE_OBJECT	m_RelicsBoxAlliedData;								// ������ ����Ÿ(���ձ�)
	ILLUSION_TEMPLE_OBJECT	m_RelicsBoxIllusionData;							// ������ ����Ÿ(ȯ����)
	ILLUSION_TEMPLE_OBJECT	m_MonsterData[MAX_ILLUSION_TEMPLE_MONSTER_COUNT];	// ���� ����Ÿ

	BOOL	m_bEntrance;					// ���� ����

	int		m_StatusRegenTime;				// ���� ���� �ð�

	int		m_nRemainTimeMin;				// ���� Ready���·� ���� ���� ���� ��
	int		m_nRemainTimeMSec;				// ���� ���·� ���� ���� ���� ��/1000
	int		m_nTickCount;					// ī������ ���� ƽī��Ʈ
	int		m_nNextTimeMin;					// ���� ��(min) ���

	int		m_nEnventEndRemainTime;			// ��������� �����ð�(��)

	BOOL	m_bGameEndPosMove;				// ������ ���� ��ġ�� �̵�

	BOOL	m_bSendMsgBeforeReady;			// �غ� 30������ �޽����� ���´��� ����
	BOOL	m_bSendMsgBeforePlay;			// ���� 30������ �޽����� ���´��� ����

	// ����ü�� �����Ұ�
	int		m_nRelicsUserIndex;				// ������ ������ �ִ� ����� ��ȣ
	BYTE	m_btRelicsTeam;					// ������ ������ �ִ� ��
	BYTE	m_btInventoryPos;				// ������ ����ִ� �κ��丮 ��ġ

	int		m_nStatusCount;					// ���� ��
	int		m_nMonsterCount;				// ���� ��

	int		m_nAlliedUserCount;				// ���ձ��� �ο���
	int		m_nIllusionUserCount;			// ȯ�� ������ �ο���
	BYTE	m_btAlliedPoint;				// ���ձ��� ����
	BYTE	m_btIllusionPoint;				// ȯ�� ������ ����

	BYTE	m_btWinTeam;					// �¸� ��

	BYTE	m_btAlliedClassCnt[MAX_CLASSTYPE];		// ���ձ� Ŭ������ �ο���
	BYTE	m_btIllusionClassCnt[MAX_CLASSTYPE];	// ȯ������ Ŭ������ �ο���

	void	ClearUserData(int nIndex);				// ����� ������ �ʱ�ȭ

	void	SendIllusionTemplePlayState(BYTE btTeam);						// ���� ������ (plyaing) �ð� ���� / ��Ƽ�� ��ġ�� ����

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
	BOOL	m_bAllCheckSync;			// ���� ���� 10������ ��� ����� �ð��� �ٽø���.
#endif	

public:
	void	IllusionTempleProcessInit(int nTempleNum);
	void	CheckSync();									// ���½ð� ����

	void	ProcIllusionTemple(int nCurTime);				// �̺�Ʈ ���¿� ���� ó��

	void	ProcIllusionTempleState_None(int nCurTime);		// �̺�Ʈ �ð� �ƴ�
	void	ProcIllusionTempleState_Ready(int nCurTime);		// �̺�Ʈ �غ�
	void	ProcIllusionTempleState_Playing(int nCurTime);	// �̺�Ʈ ����
	void	ProcIllusionTempleState_End(int nCurTime);		// �̺�Ʈ ����

	void	SetIllusionTempleState(BYTE btState);			// �̺�Ʈ ���� ����
	BYTE	GetIllusionTempleState();						// �̺�Ʈ ���� ����

	void	SetIllusionTempleStateNone();					// �̺�Ʈ�� ���� ���·� ����
	void	SetIllusionTempleStateReady();					// �̺�Ʈ�� �غ��� ���·� ����
	void	SetIllusionTempleStatePlaying();				// �̺�Ʈ�� ������ ���·� ����
	void	SetIllusionTempleStateEnd();					// �̺�Ʈ�� ������ ���·� ����

	void	SeStatusInfo(BYTE btMapNumber, int nStatusIndex, short wStatusPosNum);		// ������ �⺻���� ����

	BOOL	EnterUserIllusionTemple( int aIndex, BYTE btTempleIndex
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
		, BYTE btItemPos
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
		);					// �̺�Ʈ�� ����

	BOOL	LeaveUserIllusionTemple( int aIndex );										// �̺�Ʈ�� ����
	void	CheckUsersOnConnect();

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	int		CheckCanEnterLevel(int aIndex);
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	
	BYTE	GetRemainTimeMin();								// ����� ���۱��� ���� �ð�(��)
	BYTE	GetEnterRemainTimeMin();						// ������� ���� �ð�(��)

	void	IllusionHolyRelicsStatusAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);	// ���� �ൿ
	void	HolyRelicsBoxAlliedAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);			// ���� �������ൿ(���ձ�)
	void	HolyRelicsBoxIllusionAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// ���� �������ൿ(ȯ����)
	
	void	SetInventoryPos(int aIndex, BYTE btPos);	// ������ ��������, �κ��丮���� ������ �Ű��� �� index�� �κ��丮�� ��ġ�� ����
	void	SendRelicsUser(LPOBJECTSTRUCT lpObj);		// ������ ��������� ����ڵ鿡�� ����
	int		SearchRelicsItem(int iIndex);				// �κ��丮���� ������ ã��
	void	DeleteRelicsItem(LPOBJECTSTRUCT lpObj);		// �κ��丮���� ������ ����
	void	DropRelicsItem(int iIndex);					// �κ��丮���� ������ ���

	void	SetRemainTime();

	void	AddNpcPosNum(int nClass, int nPosNum);				// NPC ��ġ���̺� ���� �߰�			
	void	AddMonsterPosNum(int nClass, int nPosNum);			// Monster ��ġ ���̺� ���� �߰�
	void	ResetPosNum();										// NPC, ������ ��ġ������ ����

	void	AddNpcStatus();				// ���� �߰�
	void	AddNpcRelicsBox();			// ���� ������ �߰�
	void	AddMonsterGhost();			// ���� �߰�
	void	RemoveAllObj();				// ��� ������Ʈ�� ����

	void	SetStatusRegenTime();		// ������ �����ð��� ����
	void	RegenStatus();				// ������ �ϳ��� ������Ų��.
	void	RegenMonster();				// ���͸� ���� ��Ų��.

	void	SendNoticeMessage(char* lpMsg);

	BYTE	GetUserTeam(int nIndex);
	BYTE	GetUserCount()	{ return m_nUserCount; }
	BOOL	GetEntrance()	{ return m_bEntrance; }

	BYTE	IncSkillPoint(int nIndex, BYTE btIncPoint);		// ��ų ����Ʈ ����
	BYTE	DecSkillPoint(int nIndex, BYTE btDecPoint);		// ��ų ����Ʈ ����
	BYTE	GetSkillPoint(int nIndex);						// ��ų ����Ʈ ����
	BOOL	SetSkillPoint(int nIndex, BYTE btSkillPoint);	// ��ų ����Ʈ ����
	void	ResetSkillPoint();								// ��ų ����Ʈ ����

	void	IncUserKillCount(int nIndex);					// ����� ų ī��Ʈ ����
	void	IncMonsterKillCount(int nIndex);				// ���� ų ī��Ʈ ����
	void	ResetKillCount();								// ųī��Ʈ ����

	void	IncRelicsMoveCount(int nIndex);					// ���� ��ݼ� ����
	void	ResetRelicsMoveCount();							// ���� ��ݼ� ����
	void	IncUseSkillCount(int nIndex);					// ��ų ��� Ƚ�� ����
	void	ResetUseSkillCount();							// ��ų ��� Ƚ�� ����

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// ��ų��ȣ BYTE -> WORD
	void	IllusionTempleUseSkill(int nIndex, WORD btSkillNumber, int nTargetIndex, BYTE btDis);	// ���� ��ų ���
#else
	void	IllusionTempleUseSkill(int nIndex, BYTE btSkillNumber, int nTargetIndex, BYTE btDis);	// ���� ��ų ���
#endif

	void	IllusionTempleSkillProc(LPOBJECTSTRUCT lpObj);					// ��ų ���� ó��
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// ��ų��ȣ BYTE -> WORD
	void	SendIllusionTempleSkillEnd(LPOBJECTSTRUCT lpObj, WORD btState);	// ��ų ����� ����
#else
	void	SendIllusionTempleSkillEnd(LPOBJECTSTRUCT lpObj, BYTE btState);	// ��ų ����� ����
#endif
	
	BOOL	UseSkillProdection(LPOBJECTSTRUCT lpObj);						// ��ȣ�� �ֹ� ���
	BOOL	UseSkillRestraint(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);	// �ӹ��� �ֹ� ���
	BOOL 	UseSkillTeleport(LPOBJECTSTRUCT lpObj);							// ��ȯ�� �ֹ� ���
	BOOL	UseSkillShieldBurn(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);	// ����� ���
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	void	SendUseSkillResult(int nIndex, int nTargetIndex, WORD wSkillNumber, BYTE btResult);		// ��ų ��� ���
#else
	void	SendUseSkillResult(int nIndex, int nTargetIndex, BYTE btSkillNumber, BYTE btResult);		// ��ų ��� ���
#endif
	
	void	IllusionTempleUserDie(LPOBJECTSTRUCT lpObj);						// ����ڰ� �׾��� ���� ó��

	void	CalCursetTempleResult();										// ���� ��� ���
	void	DropChaosGem(int nIndex);										// �¸��������� ȥ���� ���

	int		GetRelicsUserIndex(){ return m_nRelicsUserIndex; }				// ������ ������ �ִ� ����� �ε���
	
	int		SearchScollOfBloodItem(int iIndex, float ScrollDur);			// �κ��丮���� ���� �η縶���� ã��
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
	int		SearchTicketItem(int iIndex);									// �κ��丮���� ���� �η縶�� �� ����� ã��
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617

	void	MonsterDieItemDrop(LPOBJECTSTRUCT lpObj);						// ���Ͱ� �׾����� ������ ���

	void	SendAllUserAnyMsg(LPBYTE lpMsg, int iSize);						// �������ִ� ��� ����ڵ鿡�� �޽����� ������.
	int		LevelUp (int iIndex, int iAddExp);								// ������

	void	SendIllusionTempleState(BYTE btTempleStateint, int nIndex = -1);	// ���� �̺�Ʈ���� ����

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
	void	SetAllCheckSync()		{ m_bAllCheckSync = TRUE; }
	BOOL	GetAllChecksync()		{ return m_bAllCheckSync; }

	BYTE		GetRealRemainTimeMin();
#endif
	
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
	BOOL	CheckSkillProdection(int iIllusionTempleIndex);				// ��ȣ���ֹ� ��������� �˻�
	BOOL	CheckSkillRestraint(int iIllusionTempleIndex);				// �ӹ����ֹ��� �ɸ��������� �˻�
#endif
	
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013
	void	SendIllusionTempleResult();									// ��� ������ ����
	BOOL	m_bSendResult;												// ��� ��� ���� ����
#endif
};

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#endif // !defined(AFX_ILLUSIONTEMPLEPROCESS_H__9C575967_0401_4599_940B_6D73FB6C9D2E__INCLUDED_)
