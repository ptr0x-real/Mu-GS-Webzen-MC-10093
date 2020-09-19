// CastleDeepEvent.h: interface for the CCastleDeepEvent class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_CASTLEDEEPEVENT_H__C1287797_9DCE_4E63_B1A6_D426FED40097__INCLUDED_)
#define AFX_CASTLEDEEPEVENT_H__C1287797_9DCE_4E63_B1A6_D426FED40097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push)

#pragma warning (disable : 4786)

#include <map>
#include <vector>

using namespace std;

#ifdef CASTLEDEEP_EVENT_20050413


#define	CASTLEDEEP_NOTIFYTIME			5
#define	CASTLEDEEP_MAXSPAWNCHECKCOUNT	100				// 로랜협곡 습격 이벤트 시작 시 출현할 팀을 고를 최대 횟수
#define	CASTLEDEEP_ENDTICKCOUNT			(1000*60*10)
#define	CASTLEDEEP_DELTICKCOUNT			(1000*60*10)

enum {
	CASTLEDEEPEVENT_STATE_NONE			= 0,
	CASTLEDEEPEVENT_STATE_CLOSED		= 1,
	CASTLEDEEPEVENT_STATE_PLAYING		= 2,
};

// 로랜협곡 습격 이벤트에 등장하는 몬스터 정보 (타입, 개수)
typedef struct {
	INT		m_iMonsterGroup;			// 몬스터 그룹
	INT		m_iMonsterType;				// 몬스터 타입
	INT		m_iMonsterNumber;			// 몬스터 마릿수
	INT		m_iSX;
	INT		m_iSY;
	INT		m_iDX;
	INT		m_iDY;
} CASTLEDEEP_MONSTERINFO, * LPCASTLEDEEP_MONSTERINFO;

// 로랜협곡 습격 이벤트가 진행되기 위한 스케쥴을 저장
typedef struct {
	INT		m_iHour;					// 이벤트가 시작할 시간
	INT		m_iMinute;					// 이벤트가 시작할 분
} CASTLEDEEP_EVENTTIME, * LPCASTLEDEEP_EVENTTIME;

// 로랜협곡 습격 이벤트의 타입에 따라 각 그룹이 등장할 시간을 저장
typedef struct {
	INT		m_iMonsterGroup;			// 몬스터 그룹
	INT		m_iMonsterSpawnMinute;		// 몬스터가 등장하는 분
} CASTLEDEEP_SPAWNTIME, * LPCASTLEDEEP_SPAWNTIME;

typedef map<int, vector<CASTLEDEEP_MONSTERINFO> >				TP_CASTLEDEEP_MONSTERINFO;
typedef map<int, vector<CASTLEDEEP_SPAWNTIME> >					TP_CASTLEDEEP_SPAWNTIME;
typedef vector<CASTLEDEEP_EVENTTIME>							TP_CASTLEDEEP_EVENTTIME;
typedef vector<int>												TP_CASTLEDEEP_ASSULTTYPE;
typedef map<int,int>											TP_CASTLEDEEP_RUNCHECK;

typedef map<int, vector<CASTLEDEEP_MONSTERINFO> >::iterator		TP_CASTLEDEEP_MONSTERINFO_ITR;
typedef map<int, vector<CASTLEDEEP_SPAWNTIME> >::iterator		TP_CASTLEDEEP_SPAWNTIME_ITR;
typedef vector<CASTLEDEEP_EVENTTIME>::iterator					TP_CASTLEDEEP_EVENTTIME_ITR;
typedef vector<int>::iterator									TP_CASTLEDEEP_ASSULTTYPE_ITR;
typedef map<int,int>::iterator									TP_CASTLEDEEP_RUNCHECK_ITR;


class CCastleDeepEvent  
{
	BOOL	m_bDoEvent;									// 이벤트를 할지 않할지 여부
	BOOL	m_bHasData;									// 현재 데이터가 로드되었는가?

	BOOL	m_bCheckEnd;								// 이벤트 종료를 할 수 있는지 체크 (마지막 몬스터 출현 후 N분 후 메시지가 나오도록)
	DWORD	m_uCheckEndTick;							// 종료 확인을 위한 틱카운트

	INT		m_iTIME_MSEC_REMAIN;						// 다음 상태로 가지 전까지 남는 초
	INT		m_iTIME_MIN_PLAY;							// 이벤트를 진행할 시간
	INT		m_iTIME_TICK_COUNT;							// 현재의 틱카운트 저장
	INT		m_iTIME_NOTIFY_COUNT;						// 공지 카운트
	
	INT		m_iEVENT_STATE;								// 현재의 이벤트 상태

	INT		m_iCUR_ASSULT_TYPE;							// 현재의 습격타입
	INT		m_iCUR_SPAWN_STEP;							// 현재의 그룹별 몬스터 출현 상태

	TP_CASTLEDEEP_MONSTERINFO		m_mapMonsterInfo;	// 각 습격타입, 그룹에 대한 몬스터정보 리스트
	TP_CASTLEDEEP_SPAWNTIME			m_mapSpawnTime;		// 각 습격타입, 그룹에 대한 몬스터 출현시간 리스트
	TP_CASTLEDEEP_EVENTTIME			m_vtEventTime;		// 몬스터 이벤트가 시작할 시간 리스트
	TP_CASTLEDEEP_ASSULTTYPE		m_vtAssultType;		// 습격타입
	TP_CASTLEDEEP_ASSULTTYPE		m_vtAssultTypeRate;	// 습격타입의 출현확률 (1/100)

	TP_CASTLEDEEP_RUNCHECK			m_mapRunCheck;		// 이벤트 진행 중에 체크하게 되는 부분 (그룹/시간)
	
	CRITICAL_SECTION				m_critEventData;	// 이벤트 데이터 접근을 보호하는 임계구역

public:
	CCastleDeepEvent			();
	virtual ~CCastleDeepEvent	();

	VOID	StartEvent			();						// 이벤트를 즉시 시작함
	VOID	StopEvent			();						// 이벤트를 즉시 중지함
	VOID	EnableEvent			(BOOL bDoEvent);		// 이벤트를 활성화 할지 여부
	BOOL	Load				(LPSTR lpszFileName);	// 반지이벤트의 속성정보를 로드

	// 스케쥴 관리 관련 함수
	VOID	Run					();
	VOID	CheckSync			();						// 특정 다리의 싱크를 맞춘다.
	VOID	SetState			(INT iSTATE);			// 특정 다리의 스케쥴러를 특정 상태로 변경한다.

	VOID	ProcState_None		();
	VOID	ProcState_Closed	();
	VOID	ProcState_Playing	();

	VOID	SetState_None		();
	VOID	SetState_Closed		();
	VOID	SetState_Playing	();

	// 이벤트 진행 관련 함수
	VOID	ClearMonster		();						// 이벤트 몬스터들을 제거
	VOID	AddMonster			(						// 특정 습격타입, 그룹의 몬스터들을 추가한다.
		INT iAssultType, 
		INT iGroup
		);
};


extern CCastleDeepEvent	g_CastleDeepEvent;

#pragma warning(pop)

#endif

#endif // !defined(AFX_CASTLEDEEPEVENT_H__C1287797_9DCE_4E63_B1A6_D426FED40097__INCLUDED_)
