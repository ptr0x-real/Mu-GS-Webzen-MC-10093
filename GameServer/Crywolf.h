#if !defined(__CRYWOLF_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLF_DEF__

#include "CrywolfDefine.h"
#include "CrywolfSync.h"
#include "CrywolfStateTimeInfo.h"

#include "CrywolfObjInfo.h"

#include "CrywolfStatue.h"
#include "CrywolfAltar.h"

#include "CrywolfDarkElf.h"
#include "CrywolfTanker.h"

#include "MapClass.h"

class CCrywolf
{

public:
	CCrywolf();
	virtual ~CCrywolf();
	
public:
	BOOL	LoadData( LPSTR lpszFileName );

	void	Init();
	void	Run();
	void	DelAllData();

	void	LoadCrywolfMapAttr( LPSTR lpszFileName, int iOccupationState );
	
public:
	//-----------------------------------------------------------
	
	void	SetState( int iCrywolfState );
	void	SetNextState( int iCurrentState );

	int		CheckStateTimeSync();

	void	SetState_NONE();
	void	SetState_NOTIFY_1();
	void	SetState_NOTIFY_2();
	void	SetState_READY();
	void	SetState_START();
	void	SetState_END();
	void	SetState_ENDCYCLE();

	void	ProcState_NONE();
	void	ProcState_NOTIFY_1();
	void	ProcState_NOTIFY_2();
	void	ProcState_READY();
	void	ProcState_START();
	void	ProcState_END();
	void	ProcState_ENDCYCLE();

	//-----------------------------------------------------------

	void	CrywolfNpcAct( int iIndex );
	void	CrywolfMonsterAct( int iIndex );
	void	CrywolfSecondAct();

public:
	int		GetCrywolfState(){ return m_iCrywolfState; }
	int		GetOccupationState(){ return m_iOccupationState; }
	int		GetCrywolfBossIndex(){ return m_iBossIndex; }
	
	void	SetDBDataLoad( BOOL bIsLoaded ) { m_bDBDataLoadOK = bIsLoaded; }
	void	SetCrywolfState( int iCrywolfState ){ m_iCrywolfState = iCrywolfState; }
	void	SetCrywolfStateAppliedTime( int iCrywolfState ) { m_StateTimeInfo[iCrywolfState].SetAppliedTime(); }
	void	SetOccupationState( int iOccupationState ) { m_iOccupationState = iOccupationState; }
	void	SetCrywolfBossIndex( int iBossIndex ){ m_iBossIndex = iBossIndex; }

	

public:
	// ũ���̿��� ���� ���� & �� ���� ���¸� �˸���.
	void	NotifyCrywolfCurrentState();

	// ũ���̿����� ��� �����鿡�� �ֱ������� ũ���̿��� ������ ������.
	void	NotifyCrywolfStatueAndAltarInfo();
	void	NotifyCrywolfBossMonsterInfo();

	// ũ���̿����� ���� ���¿� ���� �ð��� �˸���.
	void	NotifyCrywolfStateLeftTime();
	
	// ũ���̿����� ���� ON / OFF �� �˸���.
	void	NotifyCrywolfStageEffectOnOff( BYTE btOnOff );

	// ũ���̿��� ���� ��ũ,ȹ�����ġ�� �˸���.
	void	NotifyCrywolfPersonalRank();
	// ũ���̿��� ����� ����Ʈ�� �����ش�.
	void	NotifyCrywolfHeroList();
	
	// ���� AI ���¸� �ٲ۴�.
	void	ChangeAI(int iAIOrder);

	// �߰��� ����
	void	TurnUpBoss();

	// ũ���̿��� �Ϲ� NPC ����
	void	SetCrywolfCommonNPC( int iOccupationState );
	// ũ���̿��� �� Attr ����
	void	SetCrywolfMapAttr( int iOccupationState );
	// ũ���̿��� �Ϲ� ���� ���� ���� - iMode 1 / ���� , iMode 0 / ����
	void	SetCrywolfAllCommonMonsterState( int iMonsterState, int iMode = 1 );
	// ũ���̿��� �Ϲ� ���͸� ���� �Ѵ�.
	void	RemoveCrywolfCommonMonster();
	// ũ���̿��� �Ϲ� ���͸� ���� �Ѵ�.
	void	CreateCrywolfCommonMonster();
	// ũ���̿����� ���Ͱ� �׾��� �� Ư���� ó���� �Ѵ�.
	void	CrywolfMonsterDieProc( int iMonIndex, int iKillerIndex );
	
#ifdef ADD_CRYWOLF_SUCCESS_REWARD_20060404
	// ũ���̿��� ������ ���ܿ� ����ϰ� �ִ� �������� ������ ����
	void	MakeRewardForAltarElf( int iAltarUserIndex );
#endif

#ifdef ADD_CRYWOLF_SUCCESS_REWARD_20060404
	// ũ���̿��� ������ HeroList�� ǥ�õǴ� 1~5�� ������Ŀ���� ������ ����
	void	MakeRewardForHeroListTop5( int iUserIndex );
#endif
	
	
public:
	// ũ���̿��� ��� ���
	void	OperateGmCommand( int iUserIndex, int iCommand );

public:
	// ũ���̿��� ���� �� ������ ����ȭ �Ѵ�.
	void	CrywolfServerGroupSync();
	
	void	CrywolfInfoDBSave();
	void	CrywolfInfoDBLoad();

	void	ApplyCrywolfDBInfo( int iCrywolfState, int iOccupationState );
	
public:
	// ũ���̿��� ����, �г�Ƽ, ��ũ 
	int		CalcGettingScore( int iUserIndex, int iMonIndex, int iScoreType );
	int		CalcGettingRank( int iUserIndex );
	int		CalcGettingRewardExp( int iUserIndex, int iMVPRank );
	void	GiveRewardExp( int iUserIndex, int iRewardExp );
	void	ResetAllUserMVPScore();

public:
	BOOL					m_bFileDataLoad;
	BOOL					m_bDBDataLoadOK;
	BOOL					m_bDBDataLoading;
	int						m_iScheduleMode;

	MapClass				m_CrywolfMapAttr[MAX_CRYWOLF_MAP_STATE];

public:
	int						m_iCrywolfState;
	int						m_iOccupationState;

	
public:
	// ũ���̿��� OBJECT ���� 
	CCrywolfObjInfo			m_ObjCommonNPC;
	CCrywolfObjInfo			m_ObjSpecialNPC;
	CCrywolfObjInfo			m_ObjCommonMonster;
	CCrywolfObjInfo			m_ObjSpecialMonster;

private:
	// ���� ������ ����
	CCrywolfStateTimeInfo	m_StartTimeInfo[MAX_CRYWOLF_STARTTIME];
	int						m_StartTimeInfoCount;

	// ���� ���� ������ ����
	CCrywolfStateTimeInfo	m_StateTimeInfo[MAX_CRYWOLF_STATE_TYPE];

private:
	DWORD					m_dwCrywolfNotifyMsgStartTick;
	DWORD					m_dwCrywolfStartProcTick;
	BOOL					m_bTurnUpBoss;
	BOOL					m_bChangeAI;

private:
	// ���� �׷� ����
	int						m_iMonsterGroupNumberArray[MAX_CRYWOLF_MVP_MONSTER_GROUP];
	int						m_iMonsterGroupNumberCount;
	int						m_iMonsterGroupChangeAITime;
	
private:
	// �߰��� ����
	int						m_iBossIndex;
	int						m_iBossGroupNumber;
	int						m_iBossTurnUpTime;
	
private:
	// MVP Score, Rank ������
	int						m_iMVPScoreTable[MAX_CRYWOLF_SCORE_TABLE];
	int						m_iMVPRankScoreTable[MAX_CRYWOLF_RANK];
	int						m_iMVPRankExpTable[MAX_CRYWOLF_RANK];

};

extern CCrywolf	g_Crywolf;

#endif