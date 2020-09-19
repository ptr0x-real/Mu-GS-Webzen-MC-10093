// ChaosCastle.h: interface for the CChaosCastle class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4786)

#if !defined(AFX_CHAOSCASTLE_H__535E5C79_81B4_467D_94A0_4220F9A43D7F__INCLUDED_)
#define AFX_CHAOSCASTLE_H__535E5C79_81B4_467D_94A0_4220F9A43D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CHAOSCASTLE_SYSTEM_20040408

#include "..\INCLUDE\DEFINE.H"
#include "..\COMMON\WINUTIL.H"
#include "VECTOR"
#include "MAP"
#include "ALGORITHM"
#include <list>

using namespace std;


#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010			// 카캐 최대 개수 증가
#define MAX_CHAOSCASTLE_COUNT			7							// 카오스 캐슬의 최대개수
#else
#define MAX_CHAOSCASTLE_COUNT			6							// 카오스 캐슬의 최대개수
#endif	// ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010

#define MAX_CHAOSCASTLE_USER_COUNT		70							// 카오스 캐슬 각 소굴의 최대 유저수

#define MAX_CHAOSCASTLE_MONSTER_COUNT	100							// 카오스 캐슬 각 소굴의 최대 몬스터수

#define CC_MAX_START_WAIT_TERM			(1*60)						// 카오스 캐슬이 시작하기 전에 대기시간 (초)

#define MAX_SEARCH_SETPOINT				5							// 사용자들을 주변으로 날려버릴 때 실패 시 재시도하는  최대 횟수

#define MAX_BLOWOUT_RANGE				3							// 사용자를 밀쳐내는 거리의 최대 수 (0 ~ 4)

#define MAX_BLOWOUT_DISTANCE			(MAX_BLOWOUT_RANGE+1)		// 사용자를 밀쳐내는 거리의 최대 수 (0 ~ 4)

#define MAX_BLOWOUT_PERCENT				50							// 몬스터가 죽었을 때 사용자를 밀쳐내는 확률

#define MIN_PLAYABLE_USERCOUNT			2							// 플레이 가능한 최소 사용자

#define CHAOSCASTLE_KILL_USER_PT		1							// 카오스 캐슬에서 사용자를 죽였을 때 점수

#define CHAOSCASTLE_KILL_MONSTER_PT		2							// 카오스 캐슬에서 몬스터를 죽였을 때 점수

#define CHAOSCASTLE_OFFLINE_GIFT_RATE	2							// 카오스 캐슬 오프라인 경품에 당첨될 확률


#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 맵 번호 체크
#define CHECK_CHAOSCASTLE(C_VALUE)		(C_VALUE==MAP_INDEX_CHAOSCASTLE7?TRUE:C_VALUE<MAP_INDEX_CHAOSCASTLE1?FALSE:C_VALUE>MAP_INDEX_CHAOSCASTLE6?FALSE:TRUE)
#else
#define CHECK_CHAOSCASTLE(C_VALUE)		(C_VALUE<MAP_INDEX_CHAOSCASTLE1?FALSE:C_VALUE>MAP_INDEX_CHAOSCASTLE6?FALSE:TRUE)
#endif

enum {
	CHAOSCASTLE_STATE_NONE				=0,							// 이벤트 중단 상태
	CHAOSCASTLE_STATE_CLOSED			=1,							// 이벤트 진행 전 대기상태
	CHAOSCASTLE_STATE_PLAYING			=2,							// 이벤트 진행 중 상태
	CHAOSCASTLE_STATE_PLAYEND			=3							// 이벤트 종료 상태
};


enum {
	CHAOSCASTLE_PLAYCHECK_ING			=0,							// 경기 진행상황 - 진행 중
	CHAOSCASTLE_PLAYCHECK_SUCCESS		=1,							// 경기 진행상황 - 경기 성공
	CHAOSCASTLE_PLAYCHECK_FAILED		=2,							// 경기 진행상황 - 경기 실패
};


// 각 카오스 캐슬의 게이트 번호
#define GATE_CHAOSCASTLE_1				82
#define GATE_CHAOSCASTLE_2				83
#define GATE_CHAOSCASTLE_3				84
#define GATE_CHAOSCASTLE_4				85
#define GATE_CHAOSCASTLE_5				86
#define GATE_CHAOSCASTLE_6				87
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬7 게이트번호 수정해야함
#define GATE_CHAOSCASTLE_7				272
#endif

// 카오스 캐슬 시간 구조체
typedef struct {
	INT		m_iHour;					// 카오스 캐슬이 시작할 시간
	INT		m_iMinute;					// 카오스 캐슬이 시작할 분
	
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	BYTE	m_btEvent;					// 카오스 캐슬 이벤트 종류
#endif
} CHAOSCASTLE_START_TIME, * LPCHAOSCASTLE_START_TIME;


/*===================================================
//	<< 카오스 캐슬 레벨 제한 구조체 >>
// : 카오스 캐슬의 각 캐슬에 대한 레벨 제한에 대한 정보를 가지고 있다.
===================================================*/
typedef struct _CHAOSCASTLE_LEVEL {
	INT iLOWER_BOUND;					// 하한선
	INT iUPPER_BOUND;					// 상한선
	INT iLOWER_BOUND_MAGUMSA;			// 하한선 - 마검사
	INT iUPPER_BOUND_MAGUMSA;			// 상한선 - 마검사
} CHAOSCASTLE_LEVEL, * LPCHAOSCASTLE_LEVEL;


static CHAOSCASTLE_LEVEL g_sttCHAOSCASTLE_LEVEL[MAX_CHAOSCASTLE_COUNT] = {
	15,		49,			15,		29,
	50,		119,		30,		99,
	120,	179,		100,	159,
	180,	239,		160,	219,
	240,	299,		220,	279,
	300,	MAX_LEVEL,	280,	MAX_LEVEL,
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 카오스캐슬 마스터레벨 등급 입장 레벨
	MAX_LEVEL,	(MAX_LEVEL+MAX_MASTER_LEVEL),	MAX_LEVEL,	(MAX_LEVEL+MAX_MASTER_LEVEL),
#endif
	};


/*===================================================
//	<< 카오스 캐슬의 각종 상수 데이터 >>
// : 카오스 캐슬에 필요한 각종 상수 데이터를 지정
===================================================*/

enum {		// 사용자의 현재상태를 지정
	CC_USER_ALIVE				= 0,							// 그냥 생존자 (사용자 상태의 기본값)
	CC_USER_DEAD				= 1,							// 사망자
	CC_USER_WINNER				= 2,							// 우승자
};

enum {		// 사용자에게 30초 카운트 메시지를 보냄
	CC_MSG_BEFORE_ENTER			= 10,							// [0x92] 입장 30초전의 메시지 타입
	CC_MSG_BEFORE_PLAY			= 11,							// [0x92] 경기시작 30초전의 메시지 타입
	CC_MSG_BEFORE_END			= 12,							// [0x92] 경기종료 30초전의 메시지 타입
	CC_MSG_BEFORE_QUIT			= 13,							// [0x92] 퇴장 30초전의 메시지 타입
};

enum {		// 사용자에게 상태를 알려주는 열거형
	CC_STATE_START				= 5,							// 카오스 캐슬 - 경기 시작
	CC_STATE_PLAY				= 6,							// 카오스 캐슬 - 경기 진행 중
	CC_STATE_END				= 7,							// 카오스 캐슬 - 경기 종료
	CC_STATE_TRAP_1				= 8,							// 카오스 캐슬 - 트랩 1단계
	CC_STATE_TRAP_2				= 9,							// 카오스 캐슬 - 트랩 1단계
	CC_STATE_TRAP_3				= 10,							// 카오스 캐슬 - 트랩 1단계
};

static INT g_iChaosCastle_BlowOutDistance [MAX_BLOWOUT_DISTANCE][2]={	
			// 거리에 따라 사용자를 밀쳐버릴 수 있는 최소, 최대 크기
	3,		4,													// 거리 0의 최소, 최대
	3,		4,													// 거리 1의 최소, 최대
	2,		3,													// 거리 2의 최소, 최대
	0,		1,													// 거리 3의 최소, 최대
};

static INT g_iChaosCastle_BlowOutDamage [MAX_BLOWOUT_DISTANCE]={
			// 거리에 따라 사용자를 밀쳐버릴 수 있는 최소, 최대 크기
	15,															// 거리 0의 최소, 최대
	15,															// 거리 1의 최소, 최대
	10,															// 거리 2의 최소, 최대
	5,															// 거리 3의 최소, 최대
};

#ifdef FOR_CHINA		// 중국은 카오스 캐슬 입장료 조정
static INT g_iChaosCastle_EnterCost [MAX_CHAOSCASTLE_COUNT]={			// 카오스 캐슬 입장료
	25000,														// 1 캐슬 입장료
//	50000,														// 1 캐슬 입장료
	80000,														// 2 캐슬 입장료
	200000,														// 3 캐슬 입장료
	350000,														// 4 캐슬 입장료
	600000,														// 5 캐슬 입장료
	900000,														// 6 캐슬 입장료
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	1000000,													// 7 캐슬 입장료
#endif
};
#else
static INT g_iChaosCastle_EnterCost [MAX_CHAOSCASTLE_COUNT]={			// 카오스 캐슬 입장료
	25000,														// 1 캐슬 입장료
//	50000,														// 1 캐슬 입장료
	80000,														// 2 캐슬 입장료
	150000,														// 3 캐슬 입장료
	250000,														// 4 캐슬 입장료
	400000,														// 5 캐슬 입장료
	650000,														// 6 캐슬 입장료
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	1000000,													// 7 캐슬 입장료
#endif
};
#endif

static INT g_iChaosCastle_GuardSetValue [MAX_CHAOSCASTLE_COUNT]={		// 카오스 캐슬 근위병 세트 가격
	2500,														// +1 근위병 세트 가격
	2500,														// +2 근위병 세트 가격
	2500,														// +3 근위병 세트 가격
	2500,														// +4 근위병 세트 가격
	2500,														// +5 근위병 세트 가격
	2500,														// +6 근위병 세트 가격
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	2500,														// 7 근위병 세트 가격
#endif
};

const BYTE	g_btCastleSafetyZoneMapXY [MAX_CHAOSCASTLE_COUNT][4] = {	// 캐슬별 경기장 좌표 => (좌X, 상Y, 우X, 하Y)
	23,	75,	44,	108,											// 1번 캐슬 => (좌X, 상Y, 우X, 하Y)
	23,	75,	44,	108,											// 2번 캐슬 => (좌X, 상Y, 우X, 하Y)
	23,	75,	44,	108,											// 3번 캐슬 => (좌X, 상Y, 우X, 하Y)
	23,	75,	44,	108,											// 4번 캐슬 => (좌X, 상Y, 우X, 하Y)
	23,	75,	44,	108,											// 5번 캐슬 => (좌X, 상Y, 우X, 하Y)
	23,	75,	44,	108,											// 6번 캐슬 => (좌X, 상Y, 우X, 하Y)
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	23,	75,	44,	108,											// 7번 캐슬 => (좌X, 상Y, 우X, 하Y)
#endif
};

const BYTE g_btCastleCenterHollowZoneMapXY  [MAX_CHAOSCASTLE_COUNT][2][4] = {	// 캐슬별 경기장 가운데 뚫린 두 지검 좌표 => (좌X, 상Y, 우X, 하Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 1번 캐슬 => (좌X, 상Y, 우X, 하Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 2번 캐슬 => (좌X, 상Y, 우X, 하Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 3번 캐슬 => (좌X, 상Y, 우X, 하Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 4번 캐슬 => (좌X, 상Y, 우X, 하Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 5번 캐슬 => (좌X, 상Y, 우X, 하Y) x 2
	32,	84,	35,	87, 32,	96,	35,	99,								// 6번 캐슬 => (좌X, 상Y, 우X, 하Y) x 2
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	32,	84,	35,	87, 32,	96,	35,	99,								// 7번 캐슬 => (좌X, 상Y, 우X, 하Y) x 2
#endif
};

const INT	g_iChaosCastle_ExpTable [MAX_CHAOSCASTLE_COUNT][2] = {		// 캐슬별 죽인 상대방에 대한 경험치 값 비례 테이블
	500,	1000,												// 1번 캐슬 =>  (사람 KILL, 몬스터 KILL)
	1000,	1500,												// 2번 캐슬 =>  (사람 KILL, 몬스터 KILL)
	1500,	2000,												// 3번 캐슬 =>  (사람 KILL, 몬스터 KILL)
	2000,	2500,												// 4번 캐슬 =>  (사람 KILL, 몬스터 KILL)
	2500,	3000,												// 5번 캐슬 =>  (사람 KILL, 몬스터 KILL)
	3000,	3500,												// 6번 캐슬 =>  (사람 KILL, 몬스터 KILL)
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 캐슬별 죽인 상대방에 대한 경험치 값
	4000,	5000,												// 7번 캐슬 =>  (사람 KILL, 몬스터 KILL)
#endif
};

enum {		// 위에 경험치 테이블에 사용되는 경험치 구분 인덱스
	CC_EXP_USER_KILL			= 0,							// 인덱스 - 사용자를 죽였을 때 얻는 경험치
	CC_EXP_MONSTER_KILL			= 1,							// 인덱스 - 몬스터를 죽였을 때 얻는 경험치
};

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
enum {		// 카오스 캐슬 입장 이벤트별 정의
	CC_EVENT_GENERAL				= 0,							// 일반 입장
	CC_EVENT_PCBANG_FREE_ENTER		= 1,							// PC방 이벤트용 카오스 캐슬 무료입장
};
#endif


//--------------------------------------------------------------
// 몬스터에게 드롭을 위해 지정되는 아이템 관련

//#define MAX_CHAOSCASTLE_ITEM_DROP_KIND		4					// 카오스 캐슬 몬스터가 아이템을 드롭하는 최대 종류
//#define MAX_CHAOSCASTLE_ITEM_DROP_KIND		3					// 카오스 캐슬 몬스터가 아이템을 드롭하는 최대 종류
#define MAX_CHAOSCASTLE_ITEM_DROP_KIND		2					// 카오스 캐슬 몬스터가 아이템을 드롭하는 최대 종류
#define MAX_CHAOSCASTLE_ITEM_DROP_LOOP		200					// 카오스 캐슬 몬스터가 아이템을 드롭 실패 시 재시도 횟수

typedef struct _MONSTER_ITEM_DROP {								// 카오스 캐슬 몬스터가 떨어뜨리는 아이템
	INT		m_iIndex;											// 몬스터의 인덱스
	INT		m_iItemKind;										// 아이템의 종류
	_MONSTER_ITEM_DROP() {
		m_iIndex				= -1;
		m_iItemKind				= -1;
	}
} MONSTER_ITEM_DROP, * LPMONSTER_ITEM_DROP;


#ifndef MODIFY_CHAOSCASTLE_REWARD_20090608

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
#ifdef UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828	// 카케 보상 변경
static INT g_iChaosCastle_MonsterItems [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = 
{
	// 각 캐슬의 몬스터에게 부여되는 아이템의 종류와 개수
	MAKE_ITEMNUM(14, 13),		1,								// 1번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 1번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		1,								// 2번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 2번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		1,								// 3번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 3번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		2,								// 4번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 4번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		2,								// 5번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 5번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		2,								// 6번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 6번 캐슬 - 영석

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7카캐에서 드롭하는 보석 수
	MAKE_ITEMNUM(14, 13),		2,								// 7번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		4,								// 7번 캐슬 - 영석
#endif
};
#else // UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828
static INT g_iChaosCastle_MonsterItems [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = 
{
	// 각 캐슬의 몬스터에게 부여되는 아이템의 종류와 개수
	MAKE_ITEMNUM(14, 13),		2,								// 1번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 1번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		2,								// 2번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 2번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		2,								// 3번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 3번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		2,								// 4번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 4번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		2,								// 5번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 5번 캐슬 - 영석

	MAKE_ITEMNUM(14, 13),		2,								// 6번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 6번 캐슬 - 영석

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7카캐에서 드롭하는 보석 수
	MAKE_ITEMNUM(14, 13),		3,								// 7번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 7번 캐슬 - 영석
#endif
};
#endif	// UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828

#else	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307

static INT g_iChaosCastle_MonsterItems [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = {
	// 카오스 캐슬 몬스터 드랍 아이템 해외와 국내 동일 하게(grooving)
	// 각 캐슬의 몬스터에게 부여되는 아이템의 종류와 개수
	MAKE_ITEMNUM(14, 13),		1,								// 1번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 1번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		1,								// 2번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 2번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		1,								// 3번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 3번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		2,								// 4번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 4번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		2,								// 5번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 5번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		2,								// 6번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 6번 캐슬 - 영석
	
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7카캐에서 드롭하는 보석 수
	MAKE_ITEMNUM(14, 13),		2,								// 7번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		4,								// 7번 캐슬 - 영석
#endif
};
#endif	// #ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
#else
static INT g_iChaosCastle_MonsterItems [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = 
{
	// 각 캐슬의 몬스터에게 부여되는 아이템의 종류와 개수
	MAKE_ITEMNUM(14, 13),		2,								// 1번 캐슬 - 축석
		MAKE_ITEMNUM(14, 14),		2,								// 1번 캐슬 - 영석
		
		MAKE_ITEMNUM(14, 13),		2,								// 2번 캐슬 - 축석
		MAKE_ITEMNUM(14, 14),		2,								// 2번 캐슬 - 영석
		
		MAKE_ITEMNUM(14, 13),		2,								// 3번 캐슬 - 축석
		MAKE_ITEMNUM(14, 14),		2,								// 3번 캐슬 - 영석
		
		MAKE_ITEMNUM(14, 13),		2,								// 4번 캐슬 - 축석
		MAKE_ITEMNUM(14, 14),		2,								// 4번 캐슬 - 영석
		
		MAKE_ITEMNUM(14, 13),		2,								// 5번 캐슬 - 축석
		MAKE_ITEMNUM(14, 14),		3,								// 5번 캐슬 - 영석
		
		MAKE_ITEMNUM(14, 13),		2,								// 6번 캐슬 - 축석
		MAKE_ITEMNUM(14, 14),		3,								// 6번 캐슬 - 영석
		
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7카캐에서 드롭하는 보석 수
		MAKE_ITEMNUM(14, 13),		3,								// 7번 캐슬 - 축석
		MAKE_ITEMNUM(14, 14),		3,								// 7번 캐슬 - 영석
#endif
};
#endif // MODIFY_CHAOSCASTLE_REWARD_20090608



#ifdef ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113
// 신섭 카오스캐슬 몬스터 사냥 시 드롭되는 보상 내역 추가
static INT g_iChaosCastle_MonsterItemsNewServer [MAX_CHAOSCASTLE_COUNT][MAX_CHAOSCASTLE_ITEM_DROP_KIND][2] = {
	// 카오스 캐슬 몬스터 드랍 아이템 해외와 국내 동일 하게(grooving)
	// 각 캐슬의 몬스터에게 부여되는 아이템의 종류와 개수
	0,							0,								// 1번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 1번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		1,								// 2번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		1,								// 2번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		1,								// 3번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 3번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		1,								// 4번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 4번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		2,								// 5번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		1,								// 5번 캐슬 - 영석
	
	MAKE_ITEMNUM(14, 13),		2,								// 6번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		2,								// 6번 캐슬 - 영석
	
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// 7카캐에서 드롭하는 보석 수
	MAKE_ITEMNUM(14, 13),		2,								// 7번 캐슬 - 축석
	MAKE_ITEMNUM(14, 14),		3,								// 7번 캐슬 - 영석
#endif
};
#endif // ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113

//--------------------------------------------------------------
// 카오스 캐슬 트랩 관련

#define CC_MAX_DAMAGE_STEP		4								// 카오스 캐슬 트랩 데미지 단계

#define CC_TRAP_DAMAGE			10								// 카오스 캐슬 트랩 데미지

enum {
	CC_TRAP_0					=0,								// 카오스 캐슬 트랩 - 0단계 (작용않됨)
	CC_TRAP_1					=1,								// 카오스 캐슬 트랩 - 1단계 (외곽 1칸)
	CC_TRAP_2					=2,								// 카오스 캐슬 트랩 - 2단계 (외곽 2칸)
	CC_TRAP_3					=3,								// 카오스 캐슬 트랩 - 3단계 (외곽 3칸)
};

static INT	g_iChaosCastle_GroundAxis[4] = {					// 카오스 캐슬 경기장의 외곽좌표 - 좌X, 상Y, 우X, 하Y
	23,		75,		44,		108,
};

static INT	g_iChaosCastle_DamageAxis[CC_MAX_DAMAGE_STEP][4] = {	// 카오스 캐슬의 트랩이 적용되지 않는 내부의 범위 (단계 별)
	23,		75,		44,		108,								// 카오스 캐슬 - 트랩 0 단계
	25,		77,		42,		106,								// 카오스 캐슬 - 트랩 1 단계
	27,		79,		40,		104,								// 카오스 캐슬 - 트랩 2 단계
	29,		81,		38,		102,								// 카오스 캐슬 - 트랩 3 단계
};

static INT	g_iChaosCastle_MapHollowZone[CC_MAX_DAMAGE_STEP][4][4] = {		// 카오스 캐슬의 트랩에 따라 땅뚫린 속성으로 변하는 구역 (단계 별)
	// 트랩 0 단계 (무시)
	23,		75,		44,		76,
	43,		77,		44,		108,
	23,		107,	42,		108,
	23,		77,		24,		106,

	// 트랩 1 단계
	23,		75,		44,		76,
	43,		77,		44,		108,
	23,		107,	42,		108,
	23,		77,		24,		106,

	// 트랩 2 단계
	25,		77,		42,		78,
	41,		79,		42,		106,
	25,		105,	40,		106,
	25,		79,		26,		104,

	// 트랩 3 단계
	27,		79,		40,		80,
	39,		81,		40,		104,
	27,		103,	38,		104,
	27,		81,		28,		102,
};


static RECT g_rtPOINT_FRAME[4]	= {0,};							// 외곽범위의 각 사분면의 좌표를 지정
static RECT g_rtPOINT_TRAP[4]	= {0,};							// 트랩범위의 각 사분면의 좌표를 지정



/*===================================================
//	<< 카오스 캐슬 세션 구조체 >>
// : 카오스 캐슬의 총 6개의 소굴 각각 에 대한 데이터를 보관한다.
===================================================*/
typedef struct _CHAOSCASTLE_USER {
	INT		m_iIndex;											// 입장한 사용자의 구조체
	INT		m_iEXP;												// 사용자가 얻은 경험치
	INT		m_iScore;											// 사용자가 얻은 점수
	INT		m_iUserState;										// 사용자의 상태 (나중에 보상을 하기위함)
	bool	m_bSendQuitMsg;										// 사용자가 카오스 캐슬 밖으로 나갔을때 종료메시지를 이미 보내주었는가 아닌가? (초기값 : false)
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	BYTE	m_btEvent;											// 사용자가 속한 이벤트
#endif

	_CHAOSCASTLE_USER() {
		Clear();
	}

	VOID Clear() {
		m_iIndex		= -1;
		m_iEXP			= 0;
		m_iScore		= 0;
		m_iUserState	= CC_USER_ALIVE;
		m_bSendQuitMsg	= false;
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
		m_btEvent		= 0;
#endif		
	}
} CHAOSCASTLE_USER, * LPCHAOSCASTLE_USER;


typedef struct _CHAOSCASTLE_DATA {
	INT		m_iCC_STATE;										// 카오스 캐슬의 현재 상태를 나타냄

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	BYTE	m_btCC_EVENT;										// 현재 카오스 캐슬의 이벤트
#endif

	INT		m_iMapNumber;										// 자신의 맵번호
	INT		m_iChaosCastleIndex;								// 자신의 캐슬번호
	INT		m_iMISSION_SUCCESS;									// 이 캐슬의 미션이 성공했다. (아니면 -1 맞으면 0 ~ 15의 사용자 인덱스)
	
	INT		m_iCC_REMAIN_MSEC;									// 다음 상태로 가기까지의 시간 (1/1000 초)
	INT		m_iCC_TICK_COUNT;									// 카운트 작업들을 하기위한 시간 (1/1000초)
	INT		m_iCC_NOTIFY_COUNT;									// 공지를 보내기 위한 카운터
	
	bool	m_bCC_CAN_ENTER;									// 현재 입장 가능한가 ?
	bool	m_bCC_CAN_PARTY;									// 현재 파티 가능한가 ?
	bool	m_bCC_PLAY_START;									// 경기가 시작되었는가 ?

	bool	m_bCC_MSG_BEFORE_ENTER;								// 30초알림 - 입장전에 보냈는가?
	bool	m_bCC_MSG_BEFORE_PLAY;								// 30초알림 - 경기시작전에 보냈는가?
	bool	m_bCC_MSG_BEFORE_END;								// 30초알림 - 경기종료전에 보냈는가?
	bool	m_bCC_MSG_BEFORE_QUIT;								// 30초알림 - 퇴장전에 보냈는가?

	INT		m_iCC_ENTERED_USER;									// 해당 카오스캐슬에 입장한 사용자의 수
	INT		m_iCC_FIRST_USER_COUNT;								// 경기 시작 시의 사용자 수
	INT		m_iCC_FIRST_MONSTER_COUNT;							// 경기 시작 시의 몬스터 수
	LONG	m_lCC_CURRENT_MONSTER_COUNT;						// 경기 시작 시의 몬스터 수

	INT		m_iCC_TRAP_STATUS;									// 경기장 외곾에서 안으로 들어오는 트랩의 상태를 단계별로 나타냄
	INT		m_iCC_TRAP_OLD_STATUS;								// 경기장 외곾에서 안으로 들어오는 트랩의 상태를 단계별로 나타냄 (이전 것)

	CHAOSCASTLE_USER	m_UserData [MAX_CHAOSCASTLE_USER_COUNT];			// 사용자 구조체
	LONG				m_lCC_MONSTER_COUNT[MAX_CHAOSCASTLE_MONSTER_COUNT];	// 몬스터 구조체	
	LONG				m_lFallUser[MAX_CHAOSCASTLE_USER_COUNT];			// 각 카오스 캐슬에서 밀쳐지는 사용자를 관리

	map <INT, MONSTER_ITEM_DROP>	m_mapMonsterItemList;		// 각 카오스 캐슬에서 경기 시 몬스터가 반드시 떨어뜨려야 하는 목록 저장

	#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	FLOAT	m_fRewardExpRate;
	#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

	VOID Clear() {
		m_iMISSION_SUCCESS			= -1;
		m_iCC_NOTIFY_COUNT			= -1;
		m_bCC_MSG_BEFORE_ENTER		= false;
		m_bCC_MSG_BEFORE_PLAY		= false;
		m_bCC_MSG_BEFORE_END		= false;
		m_bCC_MSG_BEFORE_QUIT		= false;
		m_bCC_CAN_ENTER				= false;
		m_bCC_CAN_PARTY				= false;
		m_bCC_PLAY_START			= false;
		m_iCC_ENTERED_USER			= 0;
		m_iCC_FIRST_USER_COUNT		= 0;
		m_iCC_FIRST_MONSTER_COUNT	= 0;
		m_lCC_CURRENT_MONSTER_COUNT	= 0;
		m_iCC_TRAP_STATUS			= CC_TRAP_0;
		m_iCC_TRAP_OLD_STATUS		= CC_TRAP_0;
		for (INT iMON = 0 ; iMON < MAX_CHAOSCASTLE_MONSTER_COUNT ; iMON++)
		m_lCC_MONSTER_COUNT[iMON]	= -1;
		for (INT iFALL = 0 ; iFALL < MAX_CHAOSCASTLE_USER_COUNT ; iFALL++)
		m_lFallUser[iFALL]	= -1;
		m_mapMonsterItemList.clear();
	}
} CHAOSCASTLE_DATA, * LPCHAOSCASTLE_DATA;

typedef struct 
{
	unsigned short item_type;
	char item_level;
	bool is_skill;
	bool is_luck;
	char option;
	char exc_option;
	unsigned short max_drop_rate;
} CHAOSCASTLE_REWARD, *LPCHAOSCASTLE_REWARD;

typedef std::list<CHAOSCASTLE_REWARD> tdCCRewardList;

/*===================================================
//	<< 카오스 캐슬 관리 클래스 >>
// : 카오스 캐슬의 전체적인 경기 스케쥴링, 관리를 맡는다.
===================================================*/

class CChaosCastle  
{
private:
	INT		m_iMinUserCount;
	std::string m_sMinPlayerMessage;
	BOOL	m_bCC_EVENT_ENABLE;											// 이벤트가 활성화 되었는가? (실행가능한가?) 여부

	INT		m_iCC_TIME_MIN_OPEN;										// 개방시간 (분) -> 몇분 전에 개방할 것인가?
	INT		m_iCC_TIME_MIN_PLAY;										// 게임시간	(분)
	INT		m_iCC_TIME_MIN_REST;										// 게임 후 휴식시간	(분)

	INT		m_iCC_NORMAL_ITEM_DROP;										// 일반 아아템 드롭율
	INT		m_iCC_EXCEL_ITEM_DROP;										// 엑셀런트 아이템 드롭율
	INT		m_iCC_MONSTER_REGEN;										// 몬스터 리젠 시간

	vector<CHAOSCASTLE_START_TIME> m_vtChaosCastleOpenTime;				// 카오스 캐슬이 시작할 시간을 가지고 있음
	tdCCRewardList m_LST_CHAOSCASTLE_REWARD[MAX_CHAOSCASTLE_COUNT];
	INT		m_iDropCount[MAX_CHAOSCASTLE_COUNT];

public:			// 공용 멤버변수들
	

public:			// 공용 메서드들
	CChaosCastle();
	virtual ~CChaosCastle();

	/* 카오스 캐슬 스케쥴링 관련 함수들 */
	VOID	Run();
	VOID	Init (bool bEVENT_ENABLE = TRUE);
	VOID	Load (char* filename);
	VOID	SetState (INT iChaosCastleIndex, INT iCC_STATE);			// 특정 소굴의 스케쥴러를 특정 상태로 변경한다.


	/* 카오스 캐슬 사용자 괸라 함수들 */
	INT		EnterUserChaosCastle (										// iIndex의 사용자를 iChaosCastleIndex 번째 캐슬에 입방시킨다. (-1 리턴 시 방없음)	
				INT iChaosCastleIndex, 
				INT iUserIndex
				);
	INT		LeaveUserChaosCastle (										// iChaosCastleIndex 번째 캐슬의 iChaosCastleSubIndex 번째 사용자의 방을 뺀다. (iUserIndex 가 맞을때만)
				INT iChaosCastleIndex, 
				INT iUserIndex
				);
	VOID	AddFallUser (												// 폭발에 날아가서 떨어져 죽을 사용자들을 추가한다. (매번 사용자 리스트를 검색하기 힘들므로)
				INT iChaosCastleIndex,
				INT iUserIndex
				);
	VOID	ProcessFallUser (											// 떨어져 사망할 사용자의 목록에 있는 대로 사망처리를 한다.
				INT iChaosCastleIndex
				);
	INT		GetCurrentAliveUser (										// 떨어져 사망할 사용자의 목록에 있는 대로 사망처리를 한다.
				INT iChaosCastleIndex
				);
	void	SetUserState (												// 차후 보상을 위해 사용자 상태를 지정한다. 
				INT iUserIndex,
				INT iState
				);
	INT		GetCurrentWinUser (											// 현재 경기에서 이긴 사람을 찾아낸다. (-1) 이면 없음
				INT iChaosCastleIndex
				);
	INT		CalcSendRewardEXP	(										// 경기 종료 후 경험치를 계산하고 해당 사용자에게 보내준다.
				INT iUserIndex, 
				INT iEXP,
				INT iKILLCOUNT_USER,
				INT iKILLCOUNT_MONSTER
				);		
#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	INT		LevelUp (													// 카오스 캐슬 전용 레벨업 메서드
				INT iUserIndex, 
				INT iAddExp,
				INT iEventType
				);
#else
	INT		LevelUp (													// 카오스 캐슬 전용 레벨업 메서드
				INT iUserIndex, 
				INT iAddExp
				);
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	INT		GetUserLevelToEnter (										// 사용자가 어떤 카오스 캐슬에 들어갈 수 있는지 레벨을 반환한다.
				INT iUserIndex 
				);
	BOOL	CheckUserEnterMoney (										// 사용자의 충분한 입장료를 가지고 있는지 확인한다.
				INT iUserIndex, 
				INT iEnterLevel
				);
	BOOL	PayUserEnterMoney (											// 사용자의 입장료를 지불 시킨다 (FALSE면 에러가나 돈이 모자람)
				INT iUserIndex, 
				INT iEnterLevel
				);
	VOID	GiveUserDamage (											// 사용자에게 일정량의 데미지를 준다. (데미지 수치도 클라이언트로 보내주며, 죽음도 자체 처리한다)
				INT iUserIndex, 
				INT iDamage
				);


	/* 카오스 캐슬 몬스터 관리 함수들 */
	VOID	ClearMonster	(											// 해당 캐슬의 모든 몬스터 제거
				INT iChaosCastleIndex
				);
	INT		SetMonster		(INT iChaosCastleIndex);					// 카오스 캐슬에 나오는 몬스터의 특정 속성을 SET하고 배치한다. (맵번호가 18~24인 몬스터들 일괄배치)
	VOID	AddMonsterList	(											// 특정 카오스 캐슬의 몬스터 개수를 추가한다.
				INT iChaosCastleIndex,
				INT iMonsterIndex
				);
	VOID	DelMonsterList	(											// 특정 카오스 캐슬의 몬스터 개수를 제거한다.
				INT iChaosCastleIndex,
				INT iMonsterIndex
				);
	INT		GetMonsterListCount (										// 몬스터 리스트의 현재 개수를 얻는다.
				INT iChaosCastleIndex
				);
	VOID	SetItemsToMonster (											// 몬스터에게 지정된 아이템을 세팅한다. (m_vtMonsterCount 가 미리 세팅되어 있어야 함)
				INT iChaosCastleIndex
				);
	VOID	SearchNDropMonsterItem (									// 각 캐슬의 몬스터들이 죽었을 때 위에서 세팅한 몬스터 인덱스에 아이템이 맞으면 해당 아이템을 드롭한다.
				INT iChaosCastleIndex,
				INT iMonsterIndex,
				INT iMaxHitUserIndex
				);


	/* 카오스 캐슬 공지관련 함수들 */
	VOID	SendAllUserAnyMsg(											// 접속해 있는 모든 사용자들에게 메시지를 보낸다.
				LPBYTE lpMsg, 
				INT iSize
				);
	
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	VOID	SendPCBangUserAnyMsg(										// 접속해있는 모든 PC방 사용자들에게 메시지를 보낸다.
				LPBYTE lpMsg, 
				INT iSize
				);		
#endif
	
	VOID	SendChaosCastleAnyMsg(										// 특정 카오스 캐슬의 사용자들에게 메시지를 보낸다.
				LPBYTE lpMsg, 
				INT iSize, 
				INT iChaosCastleIndex
				);
	VOID	SendNoticeMessage (											// 특정 카오스 캐슬의 인원들에게 공지를 보낸다. (0x0D)
				INT iChaosCastleIndex,
				char * lpszMSG
				);
	VOID	SendNoticeState (											// 특정 카오스 캐슬의 인원들에게 현재 경기진행 상황(시간, 몬스터수)을 보낸다.
				INT iChaosCastleIndex,
				INT iPlayState
				);
	VOID	SendWinMessage (											// 특정 카오스 캐슬의 우승자에게 우승 메시지를 보내준다.
				INT iChaosCastleIndex,
				INT iWinnerIndex
				);
	VOID	SendFailMessage (											// 특정 카오스 캐슬의 실패자(사망자) 에게 실패 메시지를 보내준다.
				INT iChaosCastleIndex,
				INT iLoserIndex
				);
	VOID	SendAllLoserFailMessage (									// 특정 카오스 캐슬의 우승자를 제외한 나머지 인원들에게 실패 메시지를 보내준다.
				INT iChaosCastleIndex,
				INT iWinnerIndex
				);


	/* 카오스 캐슬 관련 각종 상태반환 함수들 */
	INT		GetCurrentState		(INT iChaosCastleIndex);
	INT		GetCurEnteredUser	(INT iChaosCastleIndex);				// 현재 입장해 있는 사용자의 수를 반환
	INT		GetCurPlayUser		(INT iChaosCastleIndex);				// 현재 실제로 입장해서 플레이 준비를 하고있는 사용자의 수를 반환
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307	// Add Function
	INT		GetCurEventUser	(											// 현재 입장한 사용자 중 이벤트별 사용자 수를 반환
				INT iChaosCastleIndex,
				BYTE btEvent 
				);		
	BYTE	GetCurrentEvent		(INT iChaosCastleIndex);
#endif

#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	INT		GetRemainTime	(											// 열리기 까지 남은 시간을 확인한다. (분단위)
				INT iUserIndex, 
				INT iChaosCastleIndex
				);
#else
	INT		GetRemainTime		(INT iChaosCastleIndex);				// 열리기 까지 남은 시간을 확인한다. (분단위)
#endif	

	INT		GetCurrentRemainSec	(INT iChaosCastleIndex);				// 단지 다음상태까지 가기위한 초를 반환
	INT		GetExcelItemDropRate() {									// 일반 아이템 드롭율
				return m_iCC_EXCEL_ITEM_DROP; }
	INT		GetNormalItemDropRate() {									// 일반 아이템 드롭율
				return m_iCC_NORMAL_ITEM_DROP; }


	/* 카오스 캐슬 관련 각종 상태체크 함수들 */
	INT		CheckEnterLevel		(INT iIndex, INT iLevel);				// 해당 사용자가 카오스 캐슬에 입장할 수 있는지 확인 (0:가능, -1:낮다, +1:높다, 2:다른에러)
	INT		CheckEnterItem		(INT iIndex);							// 해당 사용자가 카오스 캐슬에 입장할 수 있는 아이템 (근위병의 갑옷세트) 이 있는지 확인 (0 이면 불가 / 1 ~ n 까지 근위병의 갑옷세트 레벨)
	INT		CheckPlayEnded		(INT iChaosCastleIndex);				// 현재 카오스 캐슬 경기가 종료했는지 체크한다. (0:진행중, 1:성공, 2:실패)
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	bool	CheckCanEnter		(INT iUserIndex, INT iChaosCastleIndex);	// 현재 카오스 캐슬 입장이 가능한지 확인한다.
#else
	bool	CheckCanEnter		(INT iChaosCastleIndex);				// 현재 카오스 캐슬 입장이 가능한지 확인한다.
#endif	
	bool	CheckPlayStart		(INT iChaosCastleIndex);				// 현재 카오스 캐슬 경기가 시작되었는지 확인한다.
	bool	CheckCanStartPlay	(INT iChaosCastleIndex);				// 현재 카오스 캐슬 경기를 시작할 수 있는 상태인지 확인한다.


	/* 카오스 캐슬 관련 게임진행 함수들 */
	BOOL	ObjSetPosition		(INT iIndex, INT iX, INT iY);			// 특정 오브젝트 (사용자, 몬스터)의 위치를 특정 위치로 강제 지정
	VOID	SearchNBlowObjs		(										// 특정 포인트 주변 5x5 크기를 검색하여 주변의 오브젝트 (사용자, 몬스터) 들을 날려버림
				INT iMapNumber,
				INT iX, 
				INT iY
				);
	BOOL	BlowObjsFromPoint	(										// 특정 오브젝트 (사용자, 몬스터)들을 특정 위치를 중심으로 외곽으로 날려버림
				INT iIndex, 
				INT iMapNumber,
				INT & iX, 
				INT & iY
				);
	BOOL	BlowObjsToPoint	(											// 특정 오브젝트 (사용자, 몬스터)들을 특정 위치로 날려버림
				INT iIndex, 
				INT iMapNumber,
				INT iX, 
				INT iY
				);
	INT		CalDistance			(INT iX1, INT iY1, INT iX2, INT iY2);	// 두 좌표간의 거리를 계산
	VOID	SafetyCastleZone	(INT iChaosCastleIndex);				// 특정 캐슬의 영역을 안전지대 속성으로 채운다.
	VOID	UnSafetyCastleZone	(INT iChaosCastleIndex);				// 특정 캐슬의 영역의 안전지대 속성을 푼다.
	VOID	SendCastleZoneSafetyInfo (									// 특정 캐슬의 모든 사용자에게 특정 캐슬 입구의 현재 안전지대 정보를 보낸다.
				INT iChaosCastleIndex,
				bool bDoSet
				);
	VOID	CheckUserInDieTile	(										// 사용자가 땅뚫림 속성위에 서있는지 체크하여 있으면 죽음 처리
				INT iChaosCastleIndex
				);
	VOID	CheckMonsterInDieTile	(									// 몬스터가 땅뚫림 속성위에 서있는지 체크하여 있으면 떨어짐 처리
				INT iChaosCastleIndex
				);
	VOID	PlayFailedRollBack	(INT iChaosCastleIndex);				// 사용자 수 미달 등으로 경기시작이 실패 시 남은 사람들의 정보를 원상태로 돌린다.
	INT		RewardZettoMoney	(INT iChaosCastleIndex);				// 특정 캐슬에 제또 보상을 한다 (전체 입장수입의 20%를 제외한 금액)
	INT		ReCalcUserCount		(INT iChaosCastleIndex);				// 현재 입장한 사용자의 수를 다시 계산 (접속해서 캐슬안에 있는 사람 기준)
	VOID	RewardUserEXP		(										// 특정 캐슬에 보상을 실시한다.
				INT iChaosCastleIndex,
				INT iChaosCastleSubIndex,
				BOOL bWinner
				);
	VOID	ProcessTrapStatus	(INT iChaosCastleIndex);				// 특정 캐슬의 트랩 상태를 처리한다.
	VOID	CalUsersInTrap		(INT iChaosCastleIndex);				// 특정 캐슬의 사용자들이 트랩 범위 안에 있는지 파악하여 있으면 사용자에게 일정량의 데미지를 준다.
	BOOL	CheckWearingMOPH	(INT iUserIndex);						// 해당 사용자가 변신반지를 착용하고 있는지 확인한다.
	VOID	GiveWinnerItem		(										// 카오스 캐슬 우승자에게 아이템을 준다.
				INT iChaosCastleIndex,
				INT iWinnerIndex
				);
	VOID	PullObjInnerPlace	(										// 각 트랩 단계에 따라 트랩위의 사용자들을 트랩이 아닌 안으로 밀어낸다. (재수없으면 떨어짐)
																		// 그리고 맵 외곾의 아이템들을 지워준다.
				INT iChaosCastleIndex,									
				INT iTRAP_STEP
				);
	VOID	SetMapAttrHollow	(										// 각 트랩 단계에 따라 맵의 속성을 땅 뚫림으로 변경해 준다.
				INT iChaosCastleIndex,									
				INT iTRAP_STEP
				);
	VOID	SetMapAttrFill	(											// 각 트랩 단계에 따라 맵의 속성을 땅 뚫림 아님으로 변경해 준다.
				INT iChaosCastleIndex								
				);
#ifdef		CHAOSCASTLE_GUILD_DUEL_BUGFIX_20040611						// 사용자들을 체크하여 만약 길드전이나 결투 중인 사람이 있다면 강제 종료 시킨다.
	VOID	TerminateGuildWarDuel (
				INT iChaosCastleIndex								
				);
#endif
	
	/* 카오스 캐슬 오프라인 경품 지급 관련 함수들 */
	VOID	CheckRegisterOfflineGift (									// 카오스 캐슬의 오프라인 경품에 당첨 시킬 수 있는지 체크한다.
				INT iUserIndex
				);

	
protected:		// 보호 멤버변수들
	CHAOSCASTLE_DATA			m_stChaosCastleData [MAX_CHAOSCASTLE_COUNT];			// 각 카오스 캐슬의 정보를 가지고 있는 구조체의 배열

	
protected:		// 보호 메서드들
	VOID	CheckSync				(INT iChaosCastleIndex);			// 특정 소굴의 싱크를 맞춘다.
	VOID	ClearChaosCastleData	(INT iChaosCastleIndex);			// 특정 소굴 데이터를 초기화 한다.

	VOID	ProcState_None		(INT iChaosCastleIndex);				// 아무 상태도 아님
	VOID	ProcState_Closed	(INT iChaosCastleIndex);				// 입장 대기상태
	VOID	ProcState_Playing	(INT iChaosCastleIndex);				// 카오스 캐슬 경기 중 
	VOID	ProcState_PlayEnd	(INT iChaosCastleIndex);				// 카오스 캐슬 경기 종료 후 대기

	VOID	SetState_None		(INT iChaosCastleIndex);				// 카오스 캐슬 이벤트 정지 시 사용
	VOID	SetState_Closed		(INT iChaosCastleIndex);				// 입장대기 상태로 변경
	VOID	SetState_Playing	(INT iChaosCastleIndex);				// 경기 중 상태로 변경
	VOID	SetState_PlayEnd	(INT iChaosCastleIndex);				// 경기 종료 후 대기 상태로 변경

#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 다리번호, 맵번호 얻기
public:
	INT		GetMapNumByChaosCastle( INT iChaosCastleIndex );		// 카오스캐슬 번호로 맵 인덱스 구하기
	INT		GetChaosCastleByMapNum( INT iMapNum );				// 맵 인덱스로 카오스캐슬 번호 구하기

#endif
};


extern CChaosCastle			g_ChaosCastle;								// 카오스 캐슬 이벤트 객체
#endif


#endif // !defined(AFX_CHAOSCASTLE_H__535E5C79_81B4_467D_94A0_4220F9A43D7F__INCLUDED_)

