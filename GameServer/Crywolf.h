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
	// 크라이울프 진행 상태 & 맵 정령 상태를 알린다.
	void	NotifyCrywolfCurrentState();

	// 크라이울프의 모든 유저들에게 주기적으로 크라이울프 정보를 보낸다.
	void	NotifyCrywolfStatueAndAltarInfo();
	void	NotifyCrywolfBossMonsterInfo();

	// 크라이울프의 현재 상태와 남은 시간을 알린다.
	void	NotifyCrywolfStateLeftTime();
	
	// 크라이울프의 연출 ON / OFF 를 알린다.
	void	NotifyCrywolfStageEffectOnOff( BYTE btOnOff );

	// 크라이울프 개인 랭크,획득경험치를 알린다.
	void	NotifyCrywolfPersonalRank();
	// 크라이울프 히어로 리스트를 보여준다.
	void	NotifyCrywolfHeroList();
	
	// 몬스터 AI 상태를 바꾼다.
	void	ChangeAI(int iAIOrder);

	// 발가스 출현
	void	TurnUpBoss();

	// 크라이울프 일반 NPC 설정
	void	SetCrywolfCommonNPC( int iOccupationState );
	// 크라이울프 맵 Attr 설정
	void	SetCrywolfMapAttr( int iOccupationState );
	// 크라이울프 일반 몬스터 상태 설정 - iMode 1 / 적용 , iMode 0 / 제거
	void	SetCrywolfAllCommonMonsterState( int iMonsterState, int iMode = 1 );
	// 크라이울프 일반 몬스터를 제거 한다.
	void	RemoveCrywolfCommonMonster();
	// 크라이울프 일반 몬스터를 생성 한다.
	void	CreateCrywolfCommonMonster();
	// 크라이울프의 몬스터가 죽었을 때 특별한 처리를 한다.
	void	CrywolfMonsterDieProc( int iMonIndex, int iKillerIndex );
	
#ifdef ADD_CRYWOLF_SUCCESS_REWARD_20060404
	// 크라이울프 성공시 제단에 계약하고 있는 요정에게 아이템 보상
	void	MakeRewardForAltarElf( int iAltarUserIndex );
#endif

#ifdef ADD_CRYWOLF_SUCCESS_REWARD_20060404
	// 크라이울프 성공시 HeroList에 표시되는 1~5위 상위랭커에게 아이템 보상
	void	MakeRewardForHeroListTop5( int iUserIndex );
#endif
	
	
public:
	// 크라이울프 운영자 명령
	void	OperateGmCommand( int iUserIndex, int iCommand );

public:
	// 크라이울프 상태 및 정보를 동기화 한다.
	void	CrywolfServerGroupSync();
	
	void	CrywolfInfoDBSave();
	void	CrywolfInfoDBLoad();

	void	ApplyCrywolfDBInfo( int iCrywolfState, int iOccupationState );
	
public:
	// 크라이울프 혜택, 패널티, 랭크 
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
	// 크라이울프 OBJECT 관리 
	CCrywolfObjInfo			m_ObjCommonNPC;
	CCrywolfObjInfo			m_ObjSpecialNPC;
	CCrywolfObjInfo			m_ObjCommonMonster;
	CCrywolfObjInfo			m_ObjSpecialMonster;

private:
	// 시작 스케쥴 정보
	CCrywolfStateTimeInfo	m_StartTimeInfo[MAX_CRYWOLF_STARTTIME];
	int						m_StartTimeInfoCount;

	// 진행 상태 스케쥴 정보
	CCrywolfStateTimeInfo	m_StateTimeInfo[MAX_CRYWOLF_STATE_TYPE];

private:
	DWORD					m_dwCrywolfNotifyMsgStartTick;
	DWORD					m_dwCrywolfStartProcTick;
	BOOL					m_bTurnUpBoss;
	BOOL					m_bChangeAI;

private:
	// 몬스터 그룹 관리
	int						m_iMonsterGroupNumberArray[MAX_CRYWOLF_MVP_MONSTER_GROUP];
	int						m_iMonsterGroupNumberCount;
	int						m_iMonsterGroupChangeAITime;
	
private:
	// 발가스 관리
	int						m_iBossIndex;
	int						m_iBossGroupNumber;
	int						m_iBossTurnUpTime;
	
private:
	// MVP Score, Rank 설정값
	int						m_iMVPScoreTable[MAX_CRYWOLF_SCORE_TABLE];
	int						m_iMVPRankScoreTable[MAX_CRYWOLF_RANK];
	int						m_iMVPRankExpTable[MAX_CRYWOLF_RANK];

};

extern CCrywolf	g_Crywolf;

#endif