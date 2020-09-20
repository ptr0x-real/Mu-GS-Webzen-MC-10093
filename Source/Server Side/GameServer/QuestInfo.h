// QuestInfo.h: interface for the CQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTINFO_H__03AD8F22_587B_40B0_9861_69BA091B280F__INCLUDED_)
#define AFX_QUESTINFO_H__03AD8F22_587B_40B0_9861_69BA091B280F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 퀘스트 상태 정의
enum QuestStat
{
	QS_NONE		= 0,		// 없음
	QS_ACCEPT	= 1,		// 진행중
	QS_CLEAR	= 2,		// 클리어,
	QS_NOTHING	= 3			// 없음
};


enum QuestRewardType
{
	QRT_STATUP		= 200,		// 스탯업	
	QRT_CHANGEUP	= 201,		// 전직
	QRT_PLUSSTAT	= 202,		// 추가 스탯 적용
	QRT_USINGCOMBOSKILL	= 203,	// 콤보 스킬 사용
#ifdef THIRD_CHANGEUP_SYSTEM_20070507
	QRT_THIRD_CHANGEUP	= 204,	// 3차 전직
#endif
};

#ifdef THIRD_CHANGEUP_SYSTEM_20070507		
#define GATE_BARRACKS_OF_BALGAS			256
#define GATE_RESTINGPLACE_OF_BALGAS		257
#define GATE_REGENPLACE_OF_BALGAS		258
#endif

// 퀘스트 요구 조건
typedef struct 
{
	int		Index;					// 조건 인덱스( -1일때 모두 해당)
	int		NeedQuestIndex;			// 이 퀘스트를 수행하기 위해 필요한 퀘스트(없으면 -1)
	int		MinLevel;				// 최소 레벨 
	int		MaxLevel;				// 최대 레벨

#ifdef SECOND_QUEST
	int		ReqStr;					// 요구 힘
#endif
	int		NeedZen;				// 필요 젠
	int		StartContext;			// 조건이 안될때 표시될 텍스트 번호
}QUEST_CONDITION,* LPQUEST_CONDITION;

//퀘스트 상세 정보
typedef struct 
{	
	int		QuestType;				// 퀘스트 타입( 1 : 아이템 2 : 몬스터 등)
	int		NeedType;				// 필요 조건 Type( 퀘스트타입 : 1 == ItemType 2 == MonsterNumber)
	int		NeedSubType;			// 필요 조건 SubType( 퀘스트서브타입 : 2 = ItemIndex 2 = 사용안함)

#ifdef SECOND_QUEST
	int		ItemLevel;				// 아이템 레벨 (퀘스트 타입이 1일 경우에 사용됨)
#endif
	
	int		NeedNumber;				// 필요수 

	int		NeedTargetMinLevel;		// 필요한 최소 레벨
	int		NeedTargetMaxLevel;		// 필요한 최대 레벨

	int		NeedDropRate;			// 드롭 확률
	int		RewardType;				// 보상 타입( 1 : 전직 2 : 아이템 3 : 크레스타 등)
	int		RewardSubType;			// 보상 서브타입
	int		RewardCount;			// 보상수

	int		LinkConditionIndex;		// 요구조건 인덱스 

	int		ContextBeforeReg;		// 등록전 텍스트 번호
	int		ContextAfterReg;		// 등록후 텍스트 번호
	int		ContextCompleteQuest;	// 조건 완비후 텍스트 번호
	int		ContextQuestClear;		// 수행완료 후 텍스트 번호

	BYTE	RequireClass[MAX_CLASSTYPE];
}QUEST_SUB_INFO,* LPQUEST_SUB_INFO;


//  퀘스트 메인 정보
typedef struct 
{
	char	Name[50];
	int		QuestIndex;				// 퀘스트 번호
	int		QuestStartType;			// 퀘스트 시작타입(1:레벨업시, 2: NPC클릭시, 3:항상 등등)
	int		QuestStartSubType;		// 퀘스트 시작 서브타입 (퀘스트 시작타입 1 == 제한레벨  2 == NPC번호)

	int		QuestSubInfoCount;		// 퀘스트 상세정보 수
	QUEST_SUB_INFO	QuestSubInfo[MAX_QUEST_SUBINFO];

	int		QuestConditionCount;	// 퀘스트 조건 수
	QUEST_CONDITION	QuestCondition[MAX_QUEST_CONDITION];
}QUEST_INFO, * LPQUEST_INFO;

#ifdef MODIFY_QUEST_SYSTEM_20070525	// 몬스터 킬 카운트 구조체

#define QUEST_TYPE_ITEM					1	// 퀘스트타입 1 : 아이템
#define QUEST_TYPE_MONSTER_KILLCOUNT	2	// 퀘스트타입 2 : 몬스터 킬 카운스

#define MAX_QUEST_MONSTER_KILL_INFO		5
#define MAX_QUEST_MONSTER_KILL_INFO_CL	( MAX_QUEST_MONSTER_KILL_INFO * 2 )
#define MAX_QUEST_MONSTER_KILL_COUNT	50	// 50마리 이상은 취급하지 않는다.
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

	// 지정된 퀘스트의 상태를 얻어 온다
	int		GetQuestState(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// 지정된 퀘스트의 상태를 BYTE로 얻어 온다
	BYTE	GetQuestStateBYTE(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// 지정된 퀘스트의 상태를 설정한다
	BYTE	SetQuestState(LPOBJECTSTRUCT lpObj, int QuestIndex, int State);
	
	BYTE	ReSetQuestState(LPOBJECTSTRUCT lpObj, int QuestIndex);

	// 퀘스트를 클리어 하기 위한 조건을 검사한다
	BYTE	QuestClearConditionCheck(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// 퀘스트를 실행하기 위한 조건을 검사한다 : 0이면 성공 0xff 퀘스트 없음	
	BYTE	QuestRunConditionCheck(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// 조건을 비교한다
	BOOL	CompareCondition(LPOBJECTSTRUCT lpObj, LPQUEST_CONDITION lpCondition);

	// 퀘스트에 관련된 NPC를 검사한다
	BOOL	NpcTalk(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);

	// 퀘스트 아이테 드롭
	BOOL	MonsterItemDrop(LPOBJECTSTRUCT lpObj);
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 퀘스트 아이템 드롭 파티플레이 - 함수선언
	BOOL	MonsterItemDropForParty( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpMaxHitObj );
#endif	

	// 퀘스트를 진행한다
	BOOL	QuestAccept(LPOBJECTSTRUCT lpObj, int QuestIndex);
	// 퀘스트를 클리어 했다. 보상해 준다
	BOOL	QuestClear(LPOBJECTSTRUCT lpObj, int QuestIndex);

	void	QuestInfoSave(LPOBJECTSTRUCT lpObj);

#ifdef MODIFY_QUEST_SYSTEM_20070525	// CheckQuestMonsterKill 선언
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
