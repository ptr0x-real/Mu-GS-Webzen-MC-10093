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

#define MAX_ILLUSION_TEMPLE_COUNT			6		// 환영 사원 수

#ifndef ADD_SEASON_3_NEW_MAP_20070910
#define MAP_INDEX_ILLUSION_TEMPLE1			45		// 제 1 환영 사원 맵번호
#endif

#define GATE_ILLUSION_TEMPLE1				142		// 제1 환영사원 대기실 이동
#define GATE_ILLUSION_TEMPLE_ALLIED1		148		// 제1 환영사원 연합군팀 위치로 이동
#define GATE_ILLUSION_TEMPLE_ILLUSION1		154		// 제1 환영사원 환영교단팀 위치로 이동

#define MAX_ILLUSION_TEMPLE_USER_COUNT		10		// 최대 유저 수

#if TESTSERVER == 1	// 테스트 서버일경우 한명도 게임 가능
#define MIN_ILLUSION_TEMPLE_USER_COUNT		2		// 최소 유저 수
#else
#define MIN_ILLUSION_TEMPLE_USER_COUNT		4		// 최소 유저 수
#endif

#define MAX_ILLUSION_TEMPLE_STATUS_COUNT	2		// 최대 석상 수
#define MAX_ILLUSION_TEMPLE_MONSTER_COUNT	40		// 최대 몬스터 수

#define MAX_ILLUSION_TEMPLE_SKILL_POINT		90		// 최대 스킬 포인트

#define MAX_ILLUSION_TEMPLE_KILL_COUNT		255		// 최대 킬카운트
#define MAX_ILLUSION_TEMPLE_USE_SKILL_COUNT	255		// 최대 스킬 사 용횟수
#define MAX_ILLUSION_TEMPLE_RELICE_MOVE_COUNT	255	// 최대 성물 운반 횟수 

#define DEFAULT_SKILL_POINT					10		// 초기 스킬포인트 값
#define INC_SKILL_POINT_CHARACTER			5		// 플레이어 사냥시 얻는 스킬 포인트
#define INC_SKILL_POINT_MONSTER				2		// 몬스터 사냥시 얻는 스킬 포인트

#define ILLUSION_TEMPLE_MIN_WIN_POINT		2		// 승리하기 위한 최소 점수
#define ILLUSION_TEMPLE_MAX_WIN_POINT		7		// 승리하기 위한 최대 점수

#define MIN_USE_SKILL_COUNT					5		// 경험치를 얻기위한 최소 스킬 사용수 

#ifndef UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710
#define MIN_USE_SKILL_COUNT					5		// 경험치를 얻기위한 최소 스킬 사용수 
#endif

// 이벤트 상태
#define ILLUSION_TEMPLE_STATE_NONE			0
#define ILLUSION_TEMPLE_STATE_READY			1
#define ILLUSION_TEMPLE_STATE_PLAYING		2
#define ILLUSION_TEMPLE_STATE_END			3

#define ILLUSION_TEMPLE_SKILL_PRODECTION_TIME			15		// 보호의 주문 유지 시간
#define ILLUSION_TEMPLE_SKILL_RESTRAINT_TIME			15		// 속박의 주문 유지 시간

// 이벤트 (ILLUSION_TEMPLE_STATE_PLAYING)진행 상태
enum __EVENT_PLAY_INFO
{
	ILLUSION_TEMPLE_PLAY_START		= 0,
	ILLUSION_TEMPLE_PLAY_PLAYING	= 1,
	ILLUSION_TEMPLE_PLAY_END		= 2,
};

// 성물 루팅시간
#define ILLUSION_TEMPLE_RELICS_LOOT_TIME			5

// 혼석 루팅 시간
#define ILLUSION_TEMPLE_CHAOSGEM_LOOT_TIME		5*60*1000

// 환영 사원에서 캐릭터 변신을 위해 사용
#define ALLIED_CHARACTER				404	
#define ILLUSION_CHARACTER				405	

// NPC
enum __ILLUSION_TEMPLE_NPC
{
	NPC_ILLUSION_HOLY_RELICS_STATUE		= 380,		// 석상
	NPC_LEADER_OF_ALLIED_FORCE			= 381,		// 뮤연합군대장
	NPC_ELDER_OF_ILLUSION				= 382,		// 환영교단장로
	NPC_HOLY_RELICS_BOX_ALLIED			= 383,		// 성물보관함(연합군)
	NPC_HOLY_RELICS_BOX_ILLUSION		= 384,		// 성물보관함(환영교)
	NPC_PLATFORM_MIRAGE					= 385,		// 미라쥬
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

// 팀
enum __ILLUSION_TEMPLE_TEAM
{
	TEAM_ALLIED		= 0,
	TEAM_ILLUSION	= 1,	
};

// 입장 가능 최소 레벨 
enum __ENABLE_ENTER_USER_LEVEL
{
	MIN_ENTER_LEVEL1	=	220,
	MIN_ENTER_LEVEL2	=	271,
	MIN_ENTER_LEVEL3	=	321,
	MIN_ENTER_LEVEL4	=	351,
	MIN_ENTER_LEVEL5	=	381,
	MIN_ENTER_LEVEL6	=	400,
};

// 상태 변화 전에 보내는 시간 알림 메시지
enum {
	TEMPLE_MSG_BEFORE_READY	= 14,					// [0x92] 준비 30초전의 메시지 타입
	TEMPLE_MSG_BEFORE_PLAY	= 15,					// [0x92] 경기시작 30초전의 메시지 타입
};

// 등급별 기본 경험치 보상(A)
const int g_IllusionTemple_Default_Exp [MAX_ILLUSION_TEMPLE_COUNT] =
{
	300000,
	340000,
	380000,
	420000,
	460000,
	500000,
};

// 성물 획득 점수에 따른 경험치(B)
const int g_IllusionTemple_Relics_Point_Exp[MAX_ILLUSION_TEMPLE_COUNT]=
{
	50000,
	60000,
	70000,
	80000,
	90000,
	100000,
};

// 플레이어를 죽인 수에 대한 가산점(C)
const int g_IllusionTemple_Add_Kill_Exp [MAX_ILLUSION_TEMPLE_COUNT] = 
{
	600, 
	1200, 
	1800, 
	2400, 
	3000, 
	3500,
};

// 성물 운반에 대한 가산점(D)
const int g_IllusionTemple_Move_Relics_Exp [MAX_ILLUSION_TEMPLE_COUNT] = 
{
	1200, 
	2400, 
	3600, 
	4800, 
	6000, 
	7200,
};

// 플레이 유저 수에 따른 보상
const int g_IllusionTemple_PartyUser_Point[MAX_PARTYUSER] = 
{
	10,
	20,
	30,
	40,
	50,
};

// 환영 사원에 입장한 사용자들의 정보
typedef struct __ILLUSION_TEMPLE_USER_DATA
{
	INT		m_nIndex;								// 입장한 사용자의 인덱스
	BYTE	m_btTeam;								// 사용자 팀

	BYTE	m_btSkillPoint;							// 스킬포인트
	
	// 스킬
	DWORD	m_dwSkillProdectionTime;				// 보호의 주문 지속 시간
	DWORD	m_dwSkillRestraintTime;					// 속박의 주문 지속 시간
	
	BYTE	m_btUserKillCount;						// PK수
	BYTE	m_btMonsterKillCount;					// 몬스터 킬수
	BYTE	m_btRelicsMoveCount;					// 성물 운반 횟수 
	BYTE	m_btUseSkillCount;						// 스킬 사용 횟수

#ifdef BUGFIX_GAIN_EXP_20071210
	INT64	m_nRewardExp;							// 보상 경험치
#else
	int		m_nRewardExp;							// 보상 경험치
#endif
	int		m_nRankingPoint;						// 랭킹 포인트
	BOOL	m_bReward;								// 보상을 받았는지 여부
#ifdef MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
	int		m_btItemPos;							// 티켓(피의 두루마리, 무료 입장권) 아이템 위치
#else // MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
	BYTE	m_btItemPos;							// 티켓(피의 두루마리, 무료 입장권) 아이템 위치
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

// 오브젝트 정보
typedef struct __ILLUSION_TAMPE_OBJECT_DATA
{
	int m_nIndex;		// 오브젝트 인덱스	
	short m_wPosNum;	// 오브젝트 위치 테이블 번호
} ILLUSION_TEMPLE_OBJECT;

class CIllusionTempleProcess  
{
public:
	CIllusionTempleProcess();
	virtual ~CIllusionTempleProcess();

private:
	CRITICAL_SECTION	m_critUserData;										// m_UserData[] 에 데이터 입출력을 보호하기 위함
	ILLUSION_TEMPLE_USER	m_UserData[MAX_ILLUSION_TEMPLE_USER_COUNT];		// 사용자 구조체
	DWORD				m_dwViewSkillState[MAX_ILLUSION_TEMPLE_USER_COUNT];	// 스킬 상태
	int		m_nUserCount;

	BYTE	m_nTempleNumber;					// 환영 사원 번호
	BYTE	m_btIllusionTempleState;			// 이벤트 상태
	
	ILLUSION_TEMPLE_OBJECT	m_StatusData[MAX_ILLUSION_TEMPLE_STATUS_COUNT];		// 성물 데이타
	ILLUSION_TEMPLE_OBJECT	m_RelicsBoxAlliedData;								// 보관함 데이타(연합군)
	ILLUSION_TEMPLE_OBJECT	m_RelicsBoxIllusionData;							// 보관함 데이타(환영교)
	ILLUSION_TEMPLE_OBJECT	m_MonsterData[MAX_ILLUSION_TEMPLE_MONSTER_COUNT];	// 몬스터 데이타

	BOOL	m_bEntrance;					// 입장 가능

	int		m_StatusRegenTime;				// 성물 리젠 시간

	int		m_nRemainTimeMin;				// 다음 Ready상태로 가기 위해 남은 분
	int		m_nRemainTimeMSec;				// 다음 상태로 가기 위해 남은 초/1000
	int		m_nTickCount;					// 카운팅을 위한 틱카운트
	int		m_nNextTimeMin;					// 다음 분(min) 계산

	int		m_nEnventEndRemainTime;			// 게임종료시 남은시간(초)

	BOOL	m_bGameEndPosMove;				// 종료후 시작 위치로 이동

	BOOL	m_bSendMsgBeforeReady;			// 준비 30초전에 메시지를 보냈는지 여부
	BOOL	m_bSendMsgBeforePlay;			// 시작 30초전에 메시지를 보냈는지 여부

	// 구조체로 변경할것
	int		m_nRelicsUserIndex;				// 성물을 가지고 있는 사용자 번호
	BYTE	m_btRelicsTeam;					// 성물을 가지고 있는 팀
	BYTE	m_btInventoryPos;				// 성물이 들어있는 인벤토리 위치

	int		m_nStatusCount;					// 석상 수
	int		m_nMonsterCount;				// 몬스터 수

	int		m_nAlliedUserCount;				// 연합군측 인원수
	int		m_nIllusionUserCount;			// 환영 교단측 인원수
	BYTE	m_btAlliedPoint;				// 연합군측 점수
	BYTE	m_btIllusionPoint;				// 환영 교단측 점수

	BYTE	m_btWinTeam;					// 승리 팀

	BYTE	m_btAlliedClassCnt[MAX_CLASSTYPE];		// 연합군 클래스별 인원수
	BYTE	m_btIllusionClassCnt[MAX_CLASSTYPE];	// 환영교단 클래스별 인원수

	void	ClearUserData(int nIndex);				// 사용자 정보를 초기화

	void	SendIllusionTemplePlayState(BYTE btTeam);						// 현재 게임의 (plyaing) 시간 정보 / 파티원 위치를 전송

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
	BOOL	m_bAllCheckSync;			// 게임 시작 10분전에 모든 사원의 시간을 다시맞춤.
#endif	

public:
	void	IllusionTempleProcessInit(int nTempleNum);
	void	CheckSync();									// 오픈시간 설정

	void	ProcIllusionTemple(int nCurTime);				// 이벤트 상태에 따른 처리

	void	ProcIllusionTempleState_None(int nCurTime);		// 이벤트 시간 아님
	void	ProcIllusionTempleState_Ready(int nCurTime);		// 이벤트 준비
	void	ProcIllusionTempleState_Playing(int nCurTime);	// 이벤트 진행
	void	ProcIllusionTempleState_End(int nCurTime);		// 이벤트 종료

	void	SetIllusionTempleState(BYTE btState);			// 이벤트 상태 설정
	BYTE	GetIllusionTempleState();						// 이벤트 상태 얻음

	void	SetIllusionTempleStateNone();					// 이벤트가 닫힘 상태로 변경
	void	SetIllusionTempleStateReady();					// 이벤트가 준비중 상태로 변경
	void	SetIllusionTempleStatePlaying();				// 이벤트가 진행중 상태로 변경
	void	SetIllusionTempleStateEnd();					// 이벤트가 종료중 상태로 변경

	void	SeStatusInfo(BYTE btMapNumber, int nStatusIndex, short wStatusPosNum);		// 석상의 기본정보 저장

	BOOL	EnterUserIllusionTemple( int aIndex, BYTE btTempleIndex
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
		, BYTE btItemPos
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
		);					// 이벤트맵 입장

	BOOL	LeaveUserIllusionTemple( int aIndex );										// 이벤트맵 나감
	void	CheckUsersOnConnect();

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	int		CheckCanEnterLevel(int aIndex);
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	
	BYTE	GetRemainTimeMin();								// 입장시 시작까지 남은 시간(분)
	BYTE	GetEnterRemainTimeMin();						// 입장까지 남은 시간(분)

	void	IllusionHolyRelicsStatusAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);	// 석상 행동
	void	HolyRelicsBoxAlliedAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);			// 성물 보관함행동(연합군)
	void	HolyRelicsBoxIllusionAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 성물 보관함행동(환영교)
	
	void	SetInventoryPos(int aIndex, BYTE btPos);	// 성물을 집었을때, 인벤토리에서 성물을 옮겼을 때 index와 인벤토리의 위치를 저장
	void	SendRelicsUser(LPOBJECTSTRUCT lpObj);		// 성물을 집은사람을 사용자들에게 전송
	int		SearchRelicsItem(int iIndex);				// 인벤토리에서 성물을 찾음
	void	DeleteRelicsItem(LPOBJECTSTRUCT lpObj);		// 인벤토리에서 성물을 지움
	void	DropRelicsItem(int iIndex);					// 인벤토리에서 성물을 드랍

	void	SetRemainTime();

	void	AddNpcPosNum(int nClass, int nPosNum);				// NPC 위치테이블 정보 추가			
	void	AddMonsterPosNum(int nClass, int nPosNum);			// Monster 위치 테이블 정보 추가
	void	ResetPosNum();										// NPC, 몬스터의 위치정보를 리셋

	void	AddNpcStatus();				// 석상 추가
	void	AddNpcRelicsBox();			// 성물 보관함 추가
	void	AddMonsterGhost();			// 몬스터 추가
	void	RemoveAllObj();				// 모든 오브젝트를 제거

	void	SetStatusRegenTime();		// 석상의 리젠시간을 설정
	void	RegenStatus();				// 석상중 하나를 리젠시킨다.
	void	RegenMonster();				// 몬스터를 리젠 시킨다.

	void	SendNoticeMessage(char* lpMsg);

	BYTE	GetUserTeam(int nIndex);
	BYTE	GetUserCount()	{ return m_nUserCount; }
	BOOL	GetEntrance()	{ return m_bEntrance; }

	BYTE	IncSkillPoint(int nIndex, BYTE btIncPoint);		// 스킬 포인트 증가
	BYTE	DecSkillPoint(int nIndex, BYTE btDecPoint);		// 스킬 포인트 감소
	BYTE	GetSkillPoint(int nIndex);						// 스킬 포인트 얻음
	BOOL	SetSkillPoint(int nIndex, BYTE btSkillPoint);	// 스킬 포인트 설정
	void	ResetSkillPoint();								// 스킬 포인트 리셋

	void	IncUserKillCount(int nIndex);					// 사용자 킬 카운트 증가
	void	IncMonsterKillCount(int nIndex);				// 몬스터 킬 카운트 증가
	void	ResetKillCount();								// 킬카운트 리셋

	void	IncRelicsMoveCount(int nIndex);					// 성물 운반수 증가
	void	ResetRelicsMoveCount();							// 성물 운반수 리셋
	void	IncUseSkillCount(int nIndex);					// 스킬 사용 횟수 증가
	void	ResetUseSkillCount();							// 스킬 사용 횟수 리셋

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// 스킬번호 BYTE -> WORD
	void	IllusionTempleUseSkill(int nIndex, WORD btSkillNumber, int nTargetIndex, BYTE btDis);	// 전용 스킬 사용
#else
	void	IllusionTempleUseSkill(int nIndex, BYTE btSkillNumber, int nTargetIndex, BYTE btDis);	// 전용 스킬 사용
#endif

	void	IllusionTempleSkillProc(LPOBJECTSTRUCT lpObj);					// 스킬 사용시 처리
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// 스킬번호 BYTE -> WORD
	void	SendIllusionTempleSkillEnd(LPOBJECTSTRUCT lpObj, WORD btState);	// 스킬 종료시 전송
#else
	void	SendIllusionTempleSkillEnd(LPOBJECTSTRUCT lpObj, BYTE btState);	// 스킬 종료시 전송
#endif
	
	BOOL	UseSkillProdection(LPOBJECTSTRUCT lpObj);						// 보호의 주문 사용
	BOOL	UseSkillRestraint(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);	// 속박의 주문 사용
	BOOL 	UseSkillTeleport(LPOBJECTSTRUCT lpObj);							// 소환의 주문 사용
	BOOL	UseSkillShieldBurn(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);	// 쉴드번 사용
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	void	SendUseSkillResult(int nIndex, int nTargetIndex, WORD wSkillNumber, BYTE btResult);		// 스킬 사용 결과
#else
	void	SendUseSkillResult(int nIndex, int nTargetIndex, BYTE btSkillNumber, BYTE btResult);		// 스킬 사용 결과
#endif
	
	void	IllusionTempleUserDie(LPOBJECTSTRUCT lpObj);						// 사용자가 죽었을 때의 처리

	void	CalCursetTempleResult();										// 게임 결과 계산
	void	DropChaosGem(int nIndex);										// 승리한팀에게 혼석을 드랍

	int		GetRelicsUserIndex(){ return m_nRelicsUserIndex; }				// 성물을 가지고 있는 사용자 인덱스
	
	int		SearchScollOfBloodItem(int iIndex, float ScrollDur);			// 인벤토리에서 피의 두루마리를 찾음
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
	int		SearchTicketItem(int iIndex);									// 인벤토리에서 피의 두루마리 및 입장권 찾음
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617

	void	MonsterDieItemDrop(LPOBJECTSTRUCT lpObj);						// 몬스터가 죽었을때 아이템 드랍

	void	SendAllUserAnyMsg(LPBYTE lpMsg, int iSize);						// 접속해있는 모든 사용자들에게 메시지를 보낸다.
	int		LevelUp (int iIndex, int iAddExp);								// 레벨업

	void	SendIllusionTempleState(BYTE btTempleStateint, int nIndex = -1);	// 현재 이벤트맵의 상태

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
	void	SetAllCheckSync()		{ m_bAllCheckSync = TRUE; }
	BOOL	GetAllChecksync()		{ return m_bAllCheckSync; }

	BYTE		GetRealRemainTimeMin();
#endif
	
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
	BOOL	CheckSkillProdection(int iIllusionTempleIndex);				// 보호의주문 사용중인지 검사
	BOOL	CheckSkillRestraint(int iIllusionTempleIndex);				// 속박의주문이 걸린상태인지 검사
#endif
	
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013
	void	SendIllusionTempleResult();									// 사원 점수를 보냄
	BOOL	m_bSendResult;												// 사원 결과 정송 여부
#endif
};

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#endif // !defined(AFX_ILLUSIONTEMPLEPROCESS_H__9C575967_0401_4599_940B_6D73FB6C9D2E__INCLUDED_)
