// QuestInfo.h: interface for the CQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTINFO_H__03AD8F22_587B_40B0_9861_69BA091B280F__INCLUDED_)
#define AFX_QUESTINFO_H__03AD8F22_587B_40B0_9861_69BA091B280F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ����Ʈ ���� ����
enum QuestStat
{
	QS_NONE		= 0,		// ����
	QS_ACCEPT	= 1,		// ������
	QS_CLEAR	= 2,		// Ŭ����,
	QS_NOTHING	= 3			// ����
};


enum QuestRewardType
{
	QRT_STATUP		= 200,		// ���Ⱦ�	
	QRT_CHANGEUP	= 201,		// ����
	QRT_PLUSSTAT	= 202,		// �߰� ���� ����
	QRT_USINGCOMBOSKILL	= 203,	// �޺� ��ų ���
#ifdef THIRD_CHANGEUP_SYSTEM_20070507
	QRT_THIRD_CHANGEUP	= 204,	// 3�� ����
#endif
};

#ifdef THIRD_CHANGEUP_SYSTEM_20070507		
#define GATE_BARRACKS_OF_BALGAS			256
#define GATE_RESTINGPLACE_OF_BALGAS		257
#define GATE_REGENPLACE_OF_BALGAS		258
#endif

// ����Ʈ �䱸 ����
typedef struct 
{
	int		Index;					// ���� �ε���( -1�϶� ��� �ش�)
	int		NeedQuestIndex;			// �� ����Ʈ�� �����ϱ� ���� �ʿ��� ����Ʈ(������ -1)
	int		MinLevel;				// �ּ� ���� 
	int		MaxLevel;				// �ִ� ����

#ifdef SECOND_QUEST
	int		ReqStr;					// �䱸 ��
#endif
	int		NeedZen;				// �ʿ� ��
	int		StartContext;			// ������ �ȵɶ� ǥ�õ� �ؽ�Ʈ ��ȣ
}QUEST_CONDITION,* LPQUEST_CONDITION;

//����Ʈ �� ����
typedef struct 
{	
	int		QuestType;				// ����Ʈ Ÿ��( 1 : ������ 2 : ���� ��)
	int		NeedType;				// �ʿ� ���� Type( ����ƮŸ�� : 1 == ItemType 2 == MonsterNumber)
	int		NeedSubType;			// �ʿ� ���� SubType( ����Ʈ����Ÿ�� : 2 = ItemIndex 2 = ������)

#ifdef SECOND_QUEST
	int		ItemLevel;				// ������ ���� (����Ʈ Ÿ���� 1�� ��쿡 ����)
#endif
	
	int		NeedNumber;				// �ʿ�� 

	int		NeedTargetMinLevel;		// �ʿ��� �ּ� ����
	int		NeedTargetMaxLevel;		// �ʿ��� �ִ� ����

	int		NeedDropRate;			// ��� Ȯ��
	int		RewardType;				// ���� Ÿ��( 1 : ���� 2 : ������ 3 : ũ����Ÿ ��)
	int		RewardSubType;			// ���� ����Ÿ��
	int		RewardCount;			// �����

	int		LinkConditionIndex;		// �䱸���� �ε��� 

	int		ContextBeforeReg;		// ����� �ؽ�Ʈ ��ȣ
	int		ContextAfterReg;		// ����� �ؽ�Ʈ ��ȣ
	int		ContextCompleteQuest;	// ���� �Ϻ��� �ؽ�Ʈ ��ȣ
	int		ContextQuestClear;		// ����Ϸ� �� �ؽ�Ʈ ��ȣ

	BYTE	RequireClass[MAX_CLASSTYPE];
}QUEST_SUB_INFO,* LPQUEST_SUB_INFO;


//  ����Ʈ ���� ����
typedef struct 
{
	char	Name[50];
	int		QuestIndex;				// ����Ʈ ��ȣ
	int		QuestStartType;			// ����Ʈ ����Ÿ��(1:��������, 2: NPCŬ����, 3:�׻� ���)
	int		QuestStartSubType;		// ����Ʈ ���� ����Ÿ�� (����Ʈ ����Ÿ�� 1 == ���ѷ���  2 == NPC��ȣ)

	int		QuestSubInfoCount;		// ����Ʈ ������ ��
	QUEST_SUB_INFO	QuestSubInfo[MAX_QUEST_SUBINFO];

	int		QuestConditionCount;	// ����Ʈ ���� ��
	QUEST_CONDITION	QuestCondition[MAX_QUEST_CONDITION];
}QUEST_INFO, * LPQUEST_INFO;

#ifdef MODIFY_QUEST_SYSTEM_20070525	// ���� ų ī��Ʈ ����ü

#define QUEST_TYPE_ITEM					1	// ����ƮŸ�� 1 : ������
#define QUEST_TYPE_MONSTER_KILLCOUNT	2	// ����ƮŸ�� 2 : ���� ų ī�

#define MAX_QUEST_MONSTER_KILL_INFO		5
#define MAX_QUEST_MONSTER_KILL_INFO_CL	( MAX_QUEST_MONSTER_KILL_INFO * 2 )
#define MAX_QUEST_MONSTER_KILL_COUNT	50	// 50���� �̻��� ������� �ʴ´�.
typedef struct 
{	
	int		iQuestMonIndex;
	int		iKillCount;
} QUEST_INFO_MOSNTER_KILLCOUNT, *LPQUEST_INFO_MOSNTER_KILLCOUNT;
#endif	// MODIFY_QUEST_SYSTEM_20070525


#ifdef NEW_SKILL_FORSKYLAND
class CQuestInfo  
{
	int				m_QuestCount;	

	QUEST_INFO		QuestInfo[MAX_QUEST];

public:
	CQuestInfo();
	virtual ~CQuestInfo();

	void	Init();

	BOOL	IsQuest(int QuestIndex);

	int		GetQeustCount(){return m_QuestCount;}

	BOOL	LoadQuestInfo(char* filename);

#ifdef AUTH_GAMESERVER
	BOOL	LoadQuestInfo(char* Buffer, int iSize);
#endif
	
#ifdef SCRIPT_DECODE_WORK
	BOOL	LoadQuestInfoDecode(char* filename);
#endif

	void	InitQuestItem();

	LPQUEST_INFO		GetQuestInfo(int QuestIndex);
	LPQUEST_SUB_INFO	GetSubquestInfo(LPOBJECTSTRUCT lpObj, LPQUEST_INFO lpQuestInfo, int subquestindex);
	LPQUEST_CONDITION	GetQuestCondition(LPQUEST_INFO lpQuestInfo, int conditionindex);

	// ������ ����Ʈ�� ���¸� ��� �´�
	int		GetQuestState(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// ������ ����Ʈ�� ���¸� BYTE�� ��� �´�
	BYTE	GetQuestStateBYTE(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// ������ ����Ʈ�� ���¸� �����Ѵ�
	BYTE	SetQuestState(LPOBJECTSTRUCT lpObj, int QuestIndex, int State);
	
	BYTE	ReSetQuestState(LPOBJECTSTRUCT lpObj, int QuestIndex);

	// ����Ʈ�� Ŭ���� �ϱ� ���� ������ �˻��Ѵ�
	BYTE	QuestClearConditionCheck(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// ����Ʈ�� �����ϱ� ���� ������ �˻��Ѵ� : 0�̸� ���� 0xff ����Ʈ ����	
	BYTE	QuestRunConditionCheck(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// ������ ���Ѵ�
	BOOL	CompareCondition(LPOBJECTSTRUCT lpObj, LPQUEST_CONDITION lpCondition);

	// ����Ʈ�� ���õ� NPC�� �˻��Ѵ�
	BOOL	NpcTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

	// ����Ʈ ������ ���
	BOOL	MonsterItemDrop(LPOBJECTSTRUCT lpObj);
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// ����Ʈ ������ ��� ��Ƽ�÷��� - �Լ�����
	BOOL	MonsterItemDropForParty( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpMaxHitObj );
#endif	

	// ����Ʈ�� �����Ѵ�
	BOOL	QuestAccept(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// ����Ʈ�� Ŭ���� �ߴ�. ������ �ش�
	BOOL	QuestClear(LPOBJECTSTRUCT lpObj, int QuestIndex);

	void	QuestInfoSave(LPOBJECTSTRUCT lpObj);

#ifdef MODIFY_QUEST_SYSTEM_20070525	// CheckQuestMonsterKill ����
	BOOL	CheckQuestMonsterKill( LPOBJECTSTRUCT lpMonsterObj, LPOBJECTSTRUCT lpObj );
	BOOL	QuestMonsterKillInfoSend( int QuestIndex, int iIndex );
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507
	void	CheckQuestMapEnterOnWerwolf( int iIndex );
	void	CheckQuestMapEnterOnGateKeeper( int iIndex );

	void	QuestSuccessCommand( int iIndex, int iQuestIndex );
#endif

#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704
	BOOL	GetQuestItem( int iIndex, short nType, short nLevel );
#endif
};

extern CQuestInfo	g_QuestInfo;
#endif

#endif // !defined(AFX_QUESTINFO_H__03AD8F22_587B_40B0_9861_69BA091B280F__INCLUDED_)
