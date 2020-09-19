// BloodCastle.h: interface for the CBloodCastle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOODCASTLE_H__B6D64999_D5F0_453A_BBC9_A8567758D31D__INCLUDED_)
#define AFX_BLOODCASTLE_H__B6D64999_D5F0_453A_BBC9_A8567758D31D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"
#include <time.h>
#include <list>

#ifdef BLOODCASTLE_EVENT_5TH_20050531			// 생존인원에 따른 추가 포인트
#include "PartyClass.h"
#endif

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// 블러드캐슬 다리 확장
#define CHECK_BLOODCASTLE(C_VALUE)		(C_VALUE==MAP_INDEX_BLOODCASTLE8?TRUE:C_VALUE<11?FALSE:C_VALUE>17?FALSE:TRUE)
#define	MAX_BLOODCASTLE_BRIDGE_COUNT	8			// 블러드 캐슬 다리의 총 개수 (8)
#else
#ifdef BLOODCASTLE_EXTEND_20040314
#define CHECK_BLOODCASTLE(C_VALUE)		(C_VALUE<11?FALSE:C_VALUE>17?FALSE:TRUE)
#define	MAX_BLOODCASTLE_BRIDGE_COUNT	7			// 블러드 캐슬 다리의 총 개수 (7)
#else
#define CHECK_BLOODCASTLE(C_VALUE)		(C_VALUE<11?FALSE:C_VALUE>16?FALSE:TRUE)
#define	MAX_BLOODCASTLE_BRIDGE_COUNT	6			// 블러드 캐슬 다리의 총 개수 (6)
#endif
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010

#ifdef FOR_BLOODCASTLE3
	#define MAX_BLOODCASTLE_USER_COUNT	10			// 블러드 캐슬 각 다리의 총 유저수
#else
	#define MAX_BLOODCASTLE_USER_COUNT	15			// 블러드 캐슬 각 다리의 총 유저수
#endif


#define MAX_BLOODCASTLE_BOSS_COUNT		20				// 각 다리에 출현할 수 있는 최대 보스몬스터의 수

#define BC_REWARD_CHAOSEGEM_LOOT_TIME	(5*60*1000)		// 혼석이 다른사람에게 루팅되지 않는 시간

#define BC_ANGEL_WEAPON_LOOT_TIME		(10*1000)		// 대천사의 무기들이 다른사람에게 루팅되지 않는 시간

#define BC_PERUSER_MONSTER_KILL			40				// 사용자 한명당 할당된 몬스터 제거수
#define BC_PERUSER_BOSS_MONSTER_KILL	2				// 사용자 한명당 할당된 보스 몬스터 제거수

#define	BC_MONSTER_KILL_EXP_PERCENT		50				// 블러드 캐슬에서 몬스터를 사냥했을 때 경험치의 반영비율 (ex> 50/100)

#define BC_MAX_MONSTER_SUCCESS_MSG		1

#define BC_MAX_TICK_DOOR_OPEN			(3*1000)		// 블러드캐슬의 각 다리에 할당된 몬스터를 다 잡은 후 성문이 열리기 전까지의 딜레이

#define BC_MAX_START_WAIT_TERM			(1*60)			// 블러드캐슬이 시작하기 전에 대기시간 (초)

#define BC_SAINT_STATUE_1				132				// 성자의 석상1
#define BC_SAINT_STATUE_2				133				// 성자의 석상2
#define BC_SAINT_STATUE_3				134				// 성자의 석상3


enum {
	BLOODCASTLE_STATE_NONE				=0,
	BLOODCASTLE_STATE_CLOSED			=1,
	BLOODCASTLE_STATE_PLAYING			=2,
	BLOODCASTLE_STATE_PLAYEND			=3
};

// 각 블러드캐슬의 게이트 번호
#define GATE_BLOODCASTLE_1				66
#define GATE_BLOODCASTLE_2				67
#define GATE_BLOODCASTLE_3				68
#define GATE_BLOODCASTLE_4				69
#define GATE_BLOODCASTLE_5				70
#define GATE_BLOODCASTLE_6				71
#ifdef BLOODCASTLE_EXTEND_20040314
#define GATE_BLOODCASTLE_7				80
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬8 게이트 번호
#define GATE_BLOODCASTLE_8				271
#endif

#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
#include <list>

typedef struct __BLOODCASTLE_STARTTIME
{
	int nHour;
	int nMin;
} BLOODCASTLE_STARTTIME;
#endif	//UPDATE_BLOODECASTLE_SCADULE_20080624

/*===================================================
//	<< 블러드 캐슬 레벨 제한 구조체 >>
// : 블러드 캐슬의 각 다리에 대한 레벨 제한에 대한 정보를 가지고 있다.
===================================================*/
typedef struct _BLOOODCASTLE_LEVEL {
	INT iLOWER_BOUND;									// 하한선
	INT iUPPER_BOUND;									// 상한선
	INT iLOWER_BOUND_MAGUMSA;							// 하한선 - 마검사
	INT iUPPER_BOUND_MAGUMSA;							// 상한선 - 마검사
} BLOODCASTLE_LEVEL, * LPBLOODCASTLE_LEVEL;


static BLOODCASTLE_LEVEL g_sttBLOODCASTLE_LEVEL[MAX_BLOODCASTLE_BRIDGE_COUNT] = {
	15,		80,			10,		60,
	81,		130,		61,		110,
	131,	180,		111,	160,
	181,	230,		161,	210,
	231,	280,		211,	260,
#ifdef BLOODCASTLE_EXTEND_20040314
	281,	330,		261,	310,
	331,	MAX_LEVEL,	311,	MAX_LEVEL,
#else
	281,	MAX_LEVEL,	261,	MAX_LEVEL,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// 블러드캐슬8 입장가능 레벨
	MAX_LEVEL,	(MAX_LEVEL + MAX_MASTER_LEVEL),	MAX_LEVEL,	(MAX_LEVEL + MAX_MASTER_LEVEL),	
#endif
	};


/*===================================================
//	<< 블러드 캐슬의 각종 상수 데이터 >>
// : 블러드 캐슬에 필요한 각종 상수 데이터를 지정
===================================================*/

const BYTE	g_btCastleEntranceMapXY [MAX_BLOODCASTLE_BRIDGE_COUNT][4] = {	// 다리별 블록해야할 임구좌표 => (좌X, 상Y, 우X, 하Y)
	13,	15,	15,	23,										// 1번 다리의 입구 => (좌X, 상Y, 우X, 하Y)
	13,	15,	15,	23,										// 2번 다리의 입구 => (좌X, 상Y, 우X, 하Y)
	13,	15,	15,	23,										// 3번 다리의 입구 => (좌X, 상Y, 우X, 하Y)
	13,	15,	15,	23,										// 4번 다리의 입구 => (좌X, 상Y, 우X, 하Y)
	13,	15,	15,	23,										// 5번 다리의 입구 => (좌X, 상Y, 우X, 하Y)
	13,	15,	15,	23,										// 6번 다리의 입구 => (좌X, 상Y, 우X, 하Y)
#ifdef BLOODCASTLE_EXTEND_20040314
	13,	15,	15,	23,										// 7번 다리의 입구 => (좌X, 상Y, 우X, 하Y)
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
	13,	15,	15,	23,										// 8번 다리의 입구 => (좌X, 상Y, 우X, 하Y)
#endif	
	};


const BYTE	g_btCastleBridgeMapXY [MAX_BLOODCASTLE_BRIDGE_COUNT][4] = {	// 다리별 블록좌표 => (좌X, 상Y, 우X, 하Y)
	13,	70,	15,	75,										// 1번 다리 => (좌X, 상Y, 우X, 하Y)
	13,	70,	15,	75,										// 2번 다리 => (좌X, 상Y, 우X, 하Y)
	13,	70,	15,	75,										// 3번 다리 => (좌X, 상Y, 우X, 하Y)
	13,	70,	15,	75,										// 4번 다리 => (좌X, 상Y, 우X, 하Y)
	13,	70,	15,	75,										// 5번 다리 => (좌X, 상Y, 우X, 하Y)
	13,	70,	15,	75,										// 6번 다리 => (좌X, 상Y, 우X, 하Y)
#ifdef BLOODCASTLE_EXTEND_20040314
	13,	70,	15,	75,										// 7번 다리 => (좌X, 상Y, 우X, 하Y)
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
	13,	70,	15,	75,										// 8번 다리 => (좌X, 상Y, 우X, 하Y)
#endif
	};


const BYTE	g_btCastleDoorMapXY [MAX_BLOODCASTLE_BRIDGE_COUNT][3][4] = {	// 다리별 블록해야할 성문좌표 => (좌X, 상Y, 우X, 하Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 1번 다리의 성문과 성안쪽 => (좌X, 상Y, 우X, 하Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 2번 다리의 성문과 성안쪽 => (좌X, 상Y, 우X, 하Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 3번 다리의 성문과 성안쪽 => (좌X, 상Y, 우X, 하Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 4번 다리의 성문과 성안쪽 => (좌X, 상Y, 우X, 하Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 5번 다리의 성문과 성안쪽 => (좌X, 상Y, 우X, 하Y) x 3
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 6번 다리의 성문과 성안쪽 => (좌X, 상Y, 우X, 하Y) x 3
#ifdef BLOODCASTLE_EXTEND_20040314
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 7번 다리의 성문과 성안쪽 => (좌X, 상Y, 우X, 하Y) x 3
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
	13,	76,	15,	79, 11, 80, 25, 89, 8, 80, 10, 83,		// 8번 다리의 성문과 성안쪽 => (좌X, 상Y, 우X, 하Y) x 3
#endif
	};


const int g_iQuestWinExpendEXP[MAX_BLOODCASTLE_BRIDGE_COUNT][3] = {		// 퀘스트 성공 시 가산 경험치 비율 (n/100) - 0 : 산사람 (죽은 파티원) / 1 : 죽은사람
	18,			14,			16,
	14,			10,			12,
	11,			 7,			9,
	 9,			 5,			7,	
	 7,			 3,			5,
	 5,			 1,			3,
#ifdef BLOODCASTLE_EXTEND_20040314
	 5,			 1,			3,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 퀘스트성공시 가산 경험치 비율
	 5,			 1,			3,
#endif	 
};


const int g_iQuestWinExpendZEN[MAX_BLOODCASTLE_BRIDGE_COUNT][2] = {		// 퀘스트 성공 시 가산 젠 - 0 : 산사람 (죽은 파티원) / 1 : 죽은사람
	20000,		10000,
	50000,		25000,
	100000,		50000,
	150000,		80000,
	200000,		100000,
	250000,		120000,
#ifdef BLOODCASTLE_EXTEND_20040314
	250000,		120000,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 퀘스트 성공 시 가산 젠
	250000,		120000,
#endif
};


const int g_iBC_ChoasMixSuccessRate [MAX_BLOODCASTLE_BRIDGE_COUNT] = {	// 블러드 캐슬 입장권 (투망) 카오스 조합 성공률
	80,
	80,
	80,
	80,
	80,
	80,
#ifdef BLOODCASTLE_EXTEND_20040314
	80,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 투명망토 +8 조합 성공률
	80,
#endif
};

const int g_iBC_ChoasMixMoney [MAX_BLOODCASTLE_BRIDGE_COUNT] = {		// 블러드 캐슬 입장권 (투망) 카오스 조합에 필요한 돈
	50000,
	80000,
	150000,
	250000,
	400000,
	600000,
#ifdef BLOODCASTLE_EXTEND_20040314
	850000,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 투명망토 +8 조합비용
	1050000,
#endif	
};

const int g_iBC_EventScore [MAX_BLOODCASTLE_BRIDGE_COUNT][5] = {		
	//0:산사람 / 1:죽은사람 / 2:퀘스트완료자 / 3:퀘스트완료자의 파티원 (생존자) / 4:퀘스트완료자의 파티원 (사망자)
	600,		300,		1000,		800,		400,
	600,		300,		1000,		800,		400,
	600,		300,		1005,		800,		400,
	600,		300,		1005,		800,		400,
	600,		300,		1005,		800,		400,
	600,		300,		1005,		800,		400,
#ifdef BLOODCASTLE_EXTEND_20040314
	600,		300,		1005,		800,		400,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// 이벤트 추가 점수
	600,		300,		1005,		800,		400,
#endif
};


const int g_iBC_EventScore_Fail [MAX_BLOODCASTLE_BRIDGE_COUNT] = {		// 상태에 상관없이 무조건 같다.
	-300,
	-300,
	-300,
	-300,
	-300,
	-300,
#ifdef BLOODCASTLE_EXTEND_20040314
	-300,
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 실패시 이벤트점수 삭감
	-300,
#endif
};


/*
const int g_iBC_EventScore_Fail [MAX_BLOODCASTLE_BRIDGE_COUNT] = {		// 상태에 상관없이 무조건 같다.
	0,
	0,
	0,
	0,
	0,
	0,
};
*/


#ifdef FOR_BLOODCASTLE2
enum {
	ADD_EXP_KILL_DOOR		= 0,
	ADD_EXP_KILL_STATUE		= 1,
	ADD_EXP_WIN_QUEST		= 2,
	ADD_EXP_PER_SEC			= 3,
};

#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
const int g_iBC_Add_Exp [MAX_BLOODCASTLE_BRIDGE_COUNT][4] = {		
	// 
	20000,		20000,		5000,		192, 
	50000,		50000,		10000,		216, 
	80000,		80000,		15000,		240, 
	90000,		90000,		20000,		264, 
	100000,		100000,		25000,		288, 
	110000,		110000,		30000,		312,	
#ifdef BLOODCASTLE_EXTEND_20040314
	120000,		120000,		35000,		336,	
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 추가 경험치
	130000,		130000,		40000,		300,	
#endif
};
#else	// UPDATE_BLOODECASTLE_SCADULE_20080624
const int g_iBC_Add_Exp [MAX_BLOODCASTLE_BRIDGE_COUNT][4] = {		
	// 
	20000,		20000,		5000,		160, 
	50000,		50000,		10000,		180, 
	80000,		80000,		15000,		200, 
	90000,		90000,		20000,		220, 
	100000,		100000,		25000,		240, 
	110000,		110000,		30000,		260,	
#ifdef BLOODCASTLE_EXTEND_20040314
	120000,		120000,		35000,		280,	
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 추가 경험치
	130000,		130000,		40000,		300,	
#endif
};
#endif
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624


#ifdef BLOODCASTLE_EVENT_5TH_20050531			// 생존인원에 따른 추가 포인트
const int g_iBC_Party_EventPoint [MAX_PARTYUSER] = {
	5,			// 1명
	10,			// 2명
	15,			// 3명			
	20,			// 4명
	30,			// 5명
};

const int g_iBC_MONSTER_CHANGE_STATE[2][3] = {	// 블러드캐슬에서 시간이 지날 수록 차이나는 몬스터의 능력치
	// 최소 공격력, 최대 공격력, 방어력
	10,		20,		-20,						// 5분 경과
	20,		40,		-50,						// 10분 경과
};
#endif


enum {
	BC_STATE_START		=0,
	BC_STATE_PLAY		=1,
	BC_STATE_END		=2,
	BC_STATE_MONSTEREND	=3,						// 몬스터 할당량 - 1 일때 클라이언트에 다리 내리라고 보냄
	BC_STATE_PLAY_BOSS	=4,						
};


enum {
	BC_USER_ALIVE				=0,				// 그냥 생존자 (사용자 상태의 기본값)
	BC_USER_DEAD				=1,				// 사망자
	BC_USER_WINNER				=2,				// 우승자
	BC_USER_WINNER_PARTY_ALIVE	=3,				// 우승자의 파티원 (생존자)
	BC_USER_WINNER_PARTY_DEAD	=4,				// 우승자의 파티원 (사망자)
};


enum {
	BC_REWARD_WINNER,							// 우승자
	BC_REWARD_LOSER,							// 패배자
	BC_REWARD_ALIVE,							// 살아남은자
};


enum {
	BC_MSG_BEFORE_ENTER	= 3,					// [0x92] 입장 30초전의 메시지 타입
	BC_MSG_BEFORE_PLAY	= 4,					// [0x92] 경기시작 30초전의 메시지 타입
	BC_MSG_BEFORE_END	= 5,					// [0x92] 경기종료 30초전의 메시지 타입
	BC_MSG_BEFORE_QUIT	= 6,					// [0x92] 퇴장 30초전의 메시지 타입
};


enum {
	BC_MAPNUMBER_CHAOSGEM1		 = 238,			// 혼석을 생성할 때 특정 속성을 주기위한 맵번호 (1다리)
	BC_MAPNUMBER_CHAOSGEM2		 = 239,			// 혼석을 생성할 때 특정 속성을 주기위한 맵번호 (2다리)
	BC_MAPNUMBER_CHAOSGEM3		 = 240,			// 혼석을 생성할 때 특정 속성을 주기위한 맵번호 (3다리)
	BC_MAPNUMBER_CHAOSGEM4		 = 241,			// 혼석을 생성할 때 특정 속성을 주기위한 맵번호 (4다리)
	BC_MAPNUMBER_CHAOSGEM5		 = 242,			// 혼석을 생성할 때 특정 속성을 주기위한 맵번호 (5다리)
	BC_MAPNUMBER_CHAOSGEM6		 = 243,			// 혼석을 생성할 때 특정 속성을 주기위한 맵번호 (6다리)
#ifdef BLOODCASTLE_EXTEND_20040314
	BC_MAPNUMBER_CHAOSGEM7		 = 244,			// 혼석을 생성할 때 특정 속성을 주기위한 맵번호 (7다리)
#endif
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 혼석을 생성할 때 특정 속성을 주기위한 맵번호 (8다리) 수정필요함
	BC_MAPNUMBER_CHAOSGEM8		 = 245,			// 혼석을 생성할 때 특정 속성을 주기위한 맵번호 (8다리)
#endif

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 
	BC_MAPNUMBER_ULTIMATEWEAPON1 = 246,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (1다리)
	BC_MAPNUMBER_ULTIMATEWEAPON2 = 247,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (2다리)
	BC_MAPNUMBER_ULTIMATEWEAPON3 = 248,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (3다리)
	BC_MAPNUMBER_ULTIMATEWEAPON4 = 249,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (4다리)
	BC_MAPNUMBER_ULTIMATEWEAPON5 = 250,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (5다리)
	BC_MAPNUMBER_ULTIMATEWEAPON6 = 251,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (6다리)
	BC_MAPNUMBER_ULTIMATEWEAPON7 = 252,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (7다리)
	BC_MAPNUMBER_ULTIMATEWEAPON8 = 253,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (8다리) *254,255는 사용중.

#else
	BC_MAPNUMBER_ULTIMATEWEAPON1 = 247,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (1다리)
	BC_MAPNUMBER_ULTIMATEWEAPON2 = 248,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (2다리)
	BC_MAPNUMBER_ULTIMATEWEAPON3 = 249,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (3다리)
	BC_MAPNUMBER_ULTIMATEWEAPON4 = 250,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (4다리)
	BC_MAPNUMBER_ULTIMATEWEAPON5 = 251,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (5다리)
	BC_MAPNUMBER_ULTIMATEWEAPON6 = 252,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (6다리)
#ifdef BLOODCASTLE_EXTEND_20040314
	BC_MAPNUMBER_ULTIMATEWEAPON7 = 253,			// 절대무기을 생성할 때 특정 속성을 주기위한 맵번호 (7다리)
#endif
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
};

#define BC_MAPNUMBER_BASE_CHAOSGEM				(BC_MAPNUMBER_CHAOSGEM1 - MAP_INDEX_BLOODCASTLE1)
#define BC_MAPNUMBER_BASE_ULTIMATEWEAPON		(BC_MAPNUMBER_ULTIMATEWEAPON1 - MAP_INDEX_BLOODCASTLE1)


/*===================================================
//	<< 블러드 캐슬 보스 몬스터 구조체 >>
// : 블러드 캐슬의 각 다리의 보스몬스터의 정보를 가지고 있다.	(-> 이 정보들은 스크립트 데이터를 로드하기 전에만 초기회되면 된다.)
===================================================*/
typedef struct _BLOODCASTLE_BOSS_MONSTER {
	INT iMonsterClass;
	INT	iMapNumber;
	INT iBridgeNum;
	INT	iSTART_X;
	INT	iSTART_Y;
	INT iEND_X;
	INT iEND_Y;
	INT iHEALTH;
} BLOODCASTLE_BOSS_MONSTER, * LPBLOODCASTLE_BOSS_MONSTER;


/*===================================================
//	<< 블러드 캐슬 세션 구조체 >>
// : 블러드 캐슬의 총 8개의 다리 각각 에 대한 데이터를 보관한다.
===================================================*/
typedef struct _BLOODCASTLE_USER {
	INT		m_iIndex;								// 입장한 사용자의 구조체
	INT		m_iEXP;									// 사용자가 얻은 경험치
	INT		m_iScore;								// 사용자가 얻은 점수
	INT		m_iUserState;							// 사용자의 상태 (나중에 보상을 하기위함)
	bool	m_bSendQuitMsg;							// 사용자가 블러드캐슬 밖으로 나갔을때 종료메시지를 이미 보내주었는가 아닌가? (초기값 : false)
	bool	m_bLiveWhenDoorBreak;					// 성문이 파괴되었을 때 살아있었는가?
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
	char	m_szAccountID	[MAX_IDSTRING+1];		// 계정명
	char	m_szCharName	[MAX_IDSTRING+1];		// 케릭명
	INT		m_iCharClass;							// 클래스
	INT		m_iBloodCastleIndex;					// 블러드캐슬 인덱스
	INT		m_iBloodCastleSubIndex;					// 블러드캐슬 부 인덱스
#endif	
} BLOODCASTLE_USER, * LPBLOODCASTLE_USER;

typedef struct _BLOODCASTLE_BRIDGE {
	CRITICAL_SECTION	m_critUserData;										// m_UserData[] 에 데이터 입출력을 보호하기 위함
	BLOODCASTLE_USER	m_UserData [MAX_BLOODCASTLE_USER_COUNT];			// 사용자 구조체
	INT					m_nBossMonsterPosNum[MAX_BLOODCASTLE_BOSS_COUNT];	// 보스몬스터의 MonsterSetBase에서의 위치

	INT					m_nSaintStatuePosNum;								// 성자의 석상의 MonsterSetBase 에서의 위치를 저장한다.
	INT					m_nCastleDoorPosNum;								// 성문의 MonsterSetBase 에서의 위치를 저장한다.

	INT					m_iTOTAL_EXP;										// 사용자들이 얻은 전체 경험치 ?
	INT					m_iMapNumber;										// 자신의 맵번호
	INT					m_iBridgeIndex;										// 자신의 다리번호
	INT					m_iMISSION_SUCCESS;									// 이 다리의 미션이 성공했다. (아니면 -1 맞으면 0 ~ 15의 사용자 인덱스)
	BOOL				m_bCASTLE_DOOR_LIVE;								// 다리의 문이 현재 살아있는지 죽었는지 표시

	INT					m_iBC_STATE;										// 현재 상태를 나타낸다.
	INT					m_iBC_REMAIN_MSEC;									// 다음 상태로 가기까지의 시간 (1/1000 초)
	INT					m_iBC_TICK_COUNT;									// 카운트 작업들을 하기위한 시간 (1/1000초)
	INT					m_iBC_NOTIFY_COUNT;									// 공지를 보내기 위한 카운터
	bool				m_bBC_MONSTER_KILL_COMPLETE;						// 몬스터 제거할당수를 달성하였는가 ?
	bool				m_bBC_BOSS_MONSTER_KILL_COMPLETE;					// 보스 몬스터 제거할당수를 달성하였는가 ?

	bool				m_bBC_DOOR_TERMINATE_COMPLETE;						// 성문이 부숴졌는가?
	

	bool				m_bBC_CAN_ENTER;									// 현재 입장 가능한가 ?
	bool				m_bBC_CAN_PARTY;									// 현재 파티 가능한가 ?
	bool				m_bBC_PLAY_START;									// 경기가 시작되었는가 ?
	bool				m_bBC_REWARDED;										// 이전에 보상을 받았는가?
	bool				m_bBC_MSG_BEFORE_ENTER;								// 30초알림 - 입장전에 보냈는가?
	bool				m_bBC_MSG_BEFORE_PLAY;								// 30초알림 - 경기시작전에 보냈는가?
	bool				m_bBC_MSG_BEFORE_END;								// 30초알림 - 경기종료전에 보냈는가?
	bool				m_bBC_MSG_BEFORE_QUIT;								// 30초알림 - 퇴장전에 보냈는가?

	float				m_iCastleStatueHealth;								// 다리별 성자의 석상의 체력
	float				m_iCastleDoorHealth;								// 다리별 성문의 체력
	
	INT					m_iBC_MONSTER_MAX_COUNT;							// 다리별 잡아야 하는 최대 몬스터 수
	INT					m_iBC_MONSTER_KILL_COUNT;							// 다리별 현재까지 잡은 몬스터 수
	INT					m_iBC_MONSTER_SUCCESS_MSG_COUNT;					// 다리별 할당량 몬스터를 채웠을때 성문공격 공지를 보낸 횟수
	DWORD				m_dwBC_TICK_DOOR_OPEN;								// 다리별 몬스터를 다 잡은 후 BC_MAX_TICK_DOOR_OPEN 초의 딜레이를 주고 성문속성을 변경하기 위함

	INT					m_iBC_BOSS_MONSTER_MAX_COUNT;						// 다리별 잡아야 하는 최대 보스몬스터 수
	INT					m_iBC_BOSS_MONSTER_KILL_COUNT;						// 다리별 현재까지 잡은 보스 몬스터 수
	INT					m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT;				// 다리별 할당량 보스 몬스터를 채웠을때 성문공격 공지를 보낸 횟수

	DWORD				m_nBC_QUESTITEM_SERIAL;								// 대천사의 절대무기 시리즈의 시리얼을 저장한다.
	INT					m_iBC_QUEST_ITEM_USER_INDEX;						// 대천사의 절대무기를 가신 사람의 인덱스를 저장한다.
	BYTE				m_btBC_QUEST_ITEM_NUMBER;							// 현재 다리의 대천사의 절대무기의 종류번호 (0:지팡이 / 1:검 / 2:석궁)

	INT					m_iAngelKingPosNum;									// 대천사의 몬스터 포지션 인덱스를 가지고 있는다.

	INT					m_iExtraEXP_Kill_Door_Party;						// 퀘스트 수행 중 성문을 파괴한 파티번호
	INT					m_iExtraEXP_Kill_Door_Index;						// 퀘스트 수행 중 성문을 파괴한 사용자번호
	CHAR				m_szKill_Door_AccountID[MAX_IDSTRING+1];			// 퀘스트 수행 중 성문을 파괴한 사용자계정명
	CHAR				m_szKill_Door_CharName[MAX_IDSTRING+1];				// 퀘스트 수행 중 성문을 파괴한 사용자케릭명
	INT					m_iExtraEXP_Kill_Statue_Party;						// 퀘스트 수행 중 석상을 파괴한 파티번호
	INT					m_iExtraEXP_Kill_Statue_Index;						// 퀘스트 수행 중 석상을 파괴한 사용자번호
	CHAR				m_szKill_Status_CharName[MAX_IDSTRING+1];			// 퀘스트 수행 중 석상을 파괴한 사용자케릭명
	CHAR				m_szKill_Status_AccountID[MAX_IDSTRING+1];			// 퀘스트 수행 중 성문을 파괴한 사용자계정명
	INT					m_iExtraEXP_Win_Quest_Party;						// 최후에 퀘스트를 성공한 파티번호
	INT					m_iExtraEXP_Win_Quest_Index;						// 최후에 퀘스트를 성공한 사용자번호
	CHAR				m_szWin_Quest_CharName[MAX_IDSTRING+1];				// 최후에 퀘스트를 성공한 사용자케릭명
	CHAR				m_szWin_Quest_AccountID[MAX_IDSTRING+1];			// 퀘스트 수행 중 성문을 파괴한 사용자계정명

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	INT					m_iBC_COMPLETE_USER_INDEX;							// 완료한 사용자의 인덱스 -> 파티원을 추적하기 위함
#endif

#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
	INT					m_iDoorIndex;										// 성문이 생성될때의 gObj의 인덱스
#endif
	
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	FLOAT				m_fRewardExpRate;
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

	INT				m_iDropItemCount;
} BLOODCASTLE_BRIDGE, * LPBLOODCASTLE_BRIDGE;

typedef struct 
{
	unsigned short item_type;
	char item_level;
	bool is_skill;
	bool is_luck;
	char option;
	char exc_option;
	unsigned short max_drop_rate;
} BLOODCASTLE_REWARD, *LPBLOODCASTLE_REWARD;

typedef std::list<BLOODCASTLE_REWARD> tdBCRewardList;


/*===================================================
//	<< 블러드 캐슬 구조체 >>
// : 블러드 캐슬의 전체적인 관리, 스케쥴링, 관련확률계산 등등 전반적인 일을 하는 이벤트 관리용 클래스
===================================================*/
class CBloodCastle  
{
public:
	/* 카오스 조합 관련 함수들 */
	INT		CheckChoasMixItem	(INT iIndex);				// 해당 사용자가 투명망토를 만들 재료 (대천사의서, 블러드본) 를 가지고 있는지 확인 (0 이면 없음 / 1 ~ 7 은 각 레벨의 재료) / 8 은 다른아이템 발견 / 9는 레벨 않맞음 / 10은 혼석없음 / 11은 아이템중 하나 없음 / 12는 이벤트아이템이 너무 많음 / 14 은 내 레벨이 낮다.
	bool	BloodCastleChaosMix (INT iIndex, INT iLEVEL);	// 블러드캐슬 입장권 (투망) 조합을 시작한다.

	/* 사용자 경험치 및 보상관련 함수들 */
	bool	AddExperience		(INT iIndex, INT iEXP);		// 사용자의 다리번호를 확인하고 경험치를 추가한다.
	INT		CalcSendRewardEXP	(INT iIndex, INT iEXP);		// 경험치를 계산하고 해당 사용자에게 보내준다.
	INT		CalcSendRewardZEN	(INT iIndex, INT iZEN);		// 젠을 계산하고 해당 사용자에게 보내준다.
	void	DropChaosGem		(INT iIndex, INT iBridgeNum);				// 특정 사용자가 있는 자리에 혼돈의 보석을 떨군다.

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	void	SendRewardScore		(							// 랭킹서버에 보상점수를 보낸다.
								INT iIndex, 
								INT iSCORE,
								INT iLeftTime,
								INT iAlivePartyCount
								);
#else	
#ifdef BLOODCASTLE_EVENT_3RD_20040401
	void	SendRewardScore		(							// 랭킹서버에 보상점수를 보낸다.
								INT iIndex, 
								INT iSCORE,
								INT iLeftTime
								);
#else
	void	SendRewardScore		(INT iIndex, INT iSCORE);	// 랭킹서버에 보상점수를 보낸다.
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
	void	SendRewardScore		(							// 랭킹서버에 보상점수를 보낸다.
								LPBLOODCASTLE_USER lpBC_USER, 
								INT iSCORE,
								INT iLeftTime
								);
#endif
	
	void	GiveReward_Win		(							// 다리별로 게임이 종료(성공) 되었을 때 상황에 따라 적절한 보상을 한다.
				INT iIndex,
				INT iBridgeIndex
				);
	void	GiveReward_Fail		(INT iBridgeIndex);			// 다리별로 게임이 종료(실패) 되었을 때 기본 점수만 일괄적으로 보상을 한다.

	/* 블러드캐슬 게임관리 함수들 */
	void	SearchUserDeleteQuestItem (INT iIndex);			// 사용자를 검색하여 퀘스트 아이템을 드롭시킨다.
	void	SearchUserDropQuestItem (INT iIndex);			// 사용자를 검색하여 퀘스트 아이템을 지운다.
	void	SetUserState (									// 차후 보상을 위해 사용자 상태를 지정한다. 
				INT iIndex,
				INT iState
				);
	void	SendCastleEntranceBlockInfo (					// 특정 다리의 모든 사용자에게 특정다리 입구의 현재 블록정보를 보낸다.
				INT iBridgeIndex,
				bool bLive
				);
	void	SendCastleBridgeBlockInfo (						// 특정 다리의 모든 사용자에게 특정다리의 현재 블록정보를 보낸다.
				INT iBridgeIndex,
				bool bLive
				);
	void	SendCastleDoorBlockInfo (						// 특정 다리의 모든 사용자에게 특정성문 안의 현재 블록정보를 보낸다.
				INT iBridgeIndex,
				bool bLive
				);
	void	SendAllUserAnyMsg(								// 접속해 있는 모든 사용자들에게 메시지를 보낸다.
				LPBYTE lpMsg, 
				INT iSize
				);
	void	SendBridgeAnyMsg(								// 특정 다리의 사용자들에게 메시지를 보낸다.
				LPBYTE lpMsg, 
				INT iSize, 
				INT iBridgeIndex
				);
	void	SendNoticeMessage (								// 특정 다리의 인원들에게 공지를 보낸다. (0x0D)
				INT iBridgeIndex,
				char * lpszMSG
				);
	void	SendNoticeScore (								// 특정 다리의 인원들에게 자신의 점수를 보낸다. (0x0D)
				INT iBridgeIndex
				);
	void	SendNoticeState (								// 특정 다리의 인원들에게 현재 경기진행 상황(시간, 몬스터수)을 보낸다.
				INT iBridgeIndex,
				INT iPlayState
				);
	bool	CheckUserBridgeMember(							// 특정 사용자가 특정 다리의 멤버인지를 체크한다.
				INT iBridgeIndex,
				INT iIndex
				);
	void	BlockCastleEntrance		(INT iBridgeIndex);		// 특정 다리의 입구를 벽 속성으로 막는다.
	void	ReleaseCastleEntrance	(INT iBridgeIndex);		// 특정 다리의 입구의 벽 속성을 푼다.
	void	BlockCastleBridge		(INT iBridgeIndex);		// 특정 다리를 뚫림 속성으로 막는다.
	void	ReleaseCastleBridge		(INT iBridgeIndex);		// 특정 다리의 뚫림 속성을 푼다.
	void	BlockCastleDoor			(INT iBridgeIndex);		// 특정 다리의 성문의 벽 속성을 막는다.
	void	ReleaseCastleDoor		(INT iBridgeIndex);		// 특정 다리의 성문의 벽 속성을 푼다.
	void	SetMonsterKillCount		(INT iBridgeIndex);		// 특정 다리의 몬스터 제거 할당수를 지정한다.
	bool	CheckMonsterKillCount	(INT iBridgeIndex);		// 특정 다리의 몬스터 제거 할당수가 채워졌는지 확인한다. (개수 직접확인)
	bool	CheckMonsterKillSuccess	(INT iBridgeIndex);		// 특정 다리의 몬스터 제거 할당수가 채워졌는지 확인한다. (변수만 확인)
	bool	CheckBossKillCount		(INT iBridgeIndex);		// 특정 다리의 보스 몬스터 제거 할당수가 채워졌는지 확인한다. (개수 직접확인)
	bool	CheckBossKillSuccess	(INT iBridgeIndex);		// 특정 다리의 보스 몬스터 제거 할당수가 채워졌는지 확인한다. (변수만 확인)
	bool	CheckEveryUserDie		(INT iBridgeIndex);		// 특정 다리의 모든 사용자가 죽거나 제거되었는지 확인한다. (true : 다죽었음)		
	bool	CheckAngelKingExist		(INT iBridgeIndex);		// 특정 다리의 대천사가 있는지 없는지 체크 (false : 아예 포지션번호도 없음)
	INT		GetWhoGotUltimateWeapon	(INT iBridgeIndex);		// 특정 다리의 대천사의 절대무기가 있는 케릭의 사용자 인덱스를 리턴 (없으면 -1)
	INT		GetCurrentLiveUserCount	(INT iBridgeIndex);		// 특정 다리의 생존자의 수를 반환
#ifdef BLOODCASTLE_ITEMDROP_DIRECTLY_20040728
	BOOL	DropItemDirectly		(						// 아이템을 조건검사 없이 바로 드롭한다. (대천사의 절대무기 드롭에만 사용)
				INT iBridgeIndex,
				INT iIndex, 
				INT iItemType,
				INT iItemPos
				);
#endif

	/* 블러드캐슬 몬스터 관리 함수들 */
	void	ClearMonster	(								// 해당 다리의 모든 몬스터 제거
				INT iBridgeIndex,
				bool bClearCastleDoor
				);
	void	SetMonster		(INT iBridgeIndex);				// 블러드 캐슬에 나오는 몬스터의 특정 속성을 SET하고 배치한다. (맵번호가 11~17인 몬스터들 일괄배치)
	void	SetBossMonster	(INT iBridgeIndex);				// 이전에 읽었던 보스 몬스터의 속성을 기초로 보스몬스터의 속성을 SET하고 배치한다.
	void	SetSaintStatue	(INT iBridgeIndex);				// 이전에 읽었던 성자의 석상을 배치한다.
	void	LoadBossMonster (								// 보스 몬스터의 속성을 스크립트 파일에서 읽어서 가지고 있는다.
				INT iMonsterClass,	
				INT iMapNumber, 
				INT iBridgeNum,
				INT iSTART_X,
				INT iSTART_Y,
				INT iEND_X,
				INT iEND_Y,
				INT iHEALTH
				);
	
	/* 블러드캐슬 사용자 괸라 함수들 */
	INT		LeaveUserBridge (								// iBridgeNum 번째 다리의 iBridgeIndex 번째 사용자의 방을 뺀다. (iUserIndex 가 맞을때만)
				INT iBridgeIndex, 
				INT iBridgeSubIndex,
				INT iUserIndex
				);	
	INT		EnterUserBridge (								// iIndex의 사용자를 iBridgeNum 번째 다리에 입방시킨다. (-1 리턴 시 방없음)	
				INT iBridgeIndex, 
				INT iUserIndex
				);
#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	INT		LevelUp (INT iIndex, INT iAddExp, INT iEventType);				// 블러드캐슬 전용 레벨업 메서드
#else
	INT		LevelUp (INT iIndex, INT iAddExp);				// 블러드캐슬 전용 레벨업 메서드
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	VOID	CheckUsersOnConnect (							// 카오스 캐슬에 입장한 사용자가 접속 중인지 체크
				INT iBridgeIndex
				);
	
	/* 각종 상태반환 함수들 */
	INT		GetCurrentState(INT iBridgeIndex);
	INT		GetRemainTime(INT iBridgeIndex);				// 열리기 까지 남은 시간을 확인한다. (분단위)
	INT		GetCurrentRemainSec(INT iBridgeIndex);			// 단지 다음상태까지 가기위한 초를 반환
	INT		GetExcelItemDropRate() {						// 일반 아이템 드롭율
				return m_iBC_EXCEL_ITEM_DROP; }
	INT		GetNormalItemDropRate() {						// 일반 아이템 드롭율
				return m_iBC_NORMAL_ITEM_DROP; }

	/* 각종 상태체크 함수들 */
	INT		CheckEnterLevel		(INT iIndex, INT iLevel);	// 해당 사용자가 블러드 캐슬에 입장할 수 있는지 확인 (0:가능, -1:낮다, +1:높다, 2:다른에러)
#ifdef MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
	bool	CheckEnterFreeTicket(INT iIndex);	// 자유 입장권을 소지하고 있는지 확인
#endif // MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
	INT		CheckEnterItem		(INT iIndex);				// 해당 사용자가 블러드 캐슬에 입장할 수 있는 아이템 (투명망토) 이 있는지 확인 (0 이면 불가 / 1 ~ n 까지 투망 레벨)
	INT		CheckQuestItem		(INT iIndex);				// 해당 사용자가 블러드 캐슬의 퀘스트 아이템 (대천사의 지팡이, 검, 석궁 중 하나) 이 있는지 확인 (0 이면 불가 / 1 ~ 3 까지 각각 지팡이, 검, 석궁)
	bool	CheckWalk			(							// 대기시간 중 안전지대 밖으로 나가려고 하는지 검사 (나갔다면 true)
				INT iIndex,
				INT iMoveX,
				INT iMoveY						
				);
	bool	CheckCanEnter		(INT iBridgeIndex);			// 현재 블러드캐슬 입장이 가능한지 확인한다.
	bool	CheckCanParty		(INT iBridgeIndex);			// 현재 블러드캐슬 파티가 가능한지 확인한다.
	bool	CheckPlayStart		(INT iBridgeIndex);			// 현재 블러드캐슬 입장이 가능한지 확인한다.
	bool	CheckQuestItemSerial(							// 퀘스트 아이템의 시리얼이 특정 아이템과 맞는지 확인
				INT iBridgeIndex,
				CMapItem * lpItem
				);
	bool	CheckUserHaveUlimateWeapon (INT iIndex);		// 해당 사용자가 대천사의 절대무기를 가지고 있는지 확인

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	bool	CheckWinnerExist (								// 등록한 사용자가 있는지 체크
				INT iBridgeIndex
				);
	bool	CheckWinnerValid (								// 등록한 사용자가 접속을 끊거나 맵을 이탈하지 않았는지 체크
				INT iBridgeIndex
				);
	bool	CheckUserWinnerParty (							// 사용자가 우승자의 파티에 있는지 확인 (본인이 블러드캐슬 내에 존재해야 함)
				INT iBridgeIndex,
				INT iIndex
				);
	bool	CheckPartyExist		(							// 특정 사용자의 파티원이 블러드캐슬 맵내에 존재하는지 확인한다.
				INT iIndex
				);
	bool	CheckWinnerPartyComplete (						// 우승자의 파티가 승리했는지 여부
				INT iBridgeIndex
				);
	bool	SetBridgeWinner		(							// 특정 다리의 우승자를 정한다.
				INT iBridgeIndex,
				INT iIndex
				);
	int		GetWinnerPartyCompleteCount (					// 특정 다리의 우승자의 파티에 대화에 성공한 사람이 몇명인지 확인한다.
				INT iBridgeIndex
				);
	int		GetWinnerPartyCompletePoint (					// 특정 다리의 우승자의 파티생존자에 따른 점수를 구한다.
				INT iBridgeIndex
				);
	void	ChangeMonsterState (							// 블러드캐슬에 등장하는 몬스터는 리젠될 때 능력치 수정 여부를 결정하여 수정한다.
				INT iBridgeIndex,
				INT iIndex
				);
	void	FixUsersPlayStateWin (							// 블러드캐슬에 성공 시 점수적용 직전에 사용자들의 상태를 다시한번 체크 수정해 줌
				INT iBridgeIndex
				);
	void	FixUsersPlayStateFail (							// 블러드캐슬에 실패 시 점수적용 직전에 사용자들의 상태를 다시한번 체크 수정해 줌
				INT iBridgeIndex
				);
#endif
	

	/* 블러드캐슬 스케쥴링 관련 함수들 */
	void	Run();
	void	Init (bool bEVENT_ENABLE = TRUE);
	void	Load (char* filename);
	void	LoadItemDropRate ();
	void	SetState (INT iBridgeIndex, INT iBC_STATE);		// 특정 다리의 스케쥴러를 특정 상태로 변경한다.
	CBloodCastle();
	virtual ~CBloodCastle();


public:
	BLOODCASTLE_BRIDGE m_BridgeData [MAX_BLOODCASTLE_BRIDGE_COUNT];		// 블러드캐슬 다리 구조체


private:
	bool	m_bBC_EVENT_ENABLE;								// 이벤트를 할것인가 아닌가 ?

	INT		m_iBC_TIME_MIN_OPEN;							// 개방시간 (분) -> 몇분 전에 개방할 것인가?
	INT		m_iBC_TIME_MIN_PLAY;							// 게임시간	(분)
	INT		m_iBC_TIME_MIN_REST;							// 게임 후 휴식시간	(분)

	INT		m_iBC_NORMAL_ITEM_DROP;							// 몬스터들의 일반아이템 드롭율
	INT		m_iBC_EXCEL_ITEM_DROP;							// 몬스터들의 엑셀아이템 드롭율
	INT		m_iBC_MONSTER_REGEN;							// 몬스터 리젠시간 (ms)

	tdBCRewardList	m_LST_BLOODCASTLE_REWARD[MAX_BLOODCASTLE_BRIDGE_COUNT];

	
protected:
	void	CheckSync			(INT iBridgeIndex);			// 특정 다리의 싱크를 맞춘다.
	void	ClearBridgeData		(INT iBridgeIndex);			// 특정 다리 데이터를 초기화 한다.

	void	ProcState_None		(INT iBridgeIndex);
	void	ProcState_Closed	(INT iBridgeIndex);
	void	ProcState_Playing	(INT iBridgeIndex);
	void	ProcState_PlayEnd	(INT iBridgeIndex);

	void	SetState_None		(INT iBridgeIndex);			// 블러드캐슬 이벤트 정지 시 사용
	void	SetState_Closed		(INT iBridgeIndex);
	void	SetState_Playing	(INT iBridgeIndex);
	void	SetState_PlayEnd	(INT iBridgeIndex);

	void	BlockSector(									// 맵의 특정섹터에 벽 속성을 임의로 넣는다.
				INT iMAP_NUM,								// 맵번호
				INT iSTART_X,								// 좌측 X
				INT iSTART_Y,								// 상단 Y
				INT iEND_X,									// 우측 X
				INT iEND_Y									// 하단 Y
				);		
	void	ReleaseSector(									// 맵에 존재하는 벽 속성을 제거한다.
				INT iMAP_NUM,
				INT iSTART_X,								// 좌측 X
				INT iSTART_Y,								// 상단 Y
				INT iEND_X,									// 우측 X
				INT iEND_Y									// 하단 Y
				);


	/* 사용자 경험치 및 보상관련 함수들 */
	INT		GetAliveUserTotalEXP(							// 살아있는 사용자들의 총 경험치를 구한다.
				INT iBridgeIndex
				);		
	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리번호로 맵번호 얻기
public:
	INT		GetMapNumByBCBridge( INT iBridgeIndex );	// 다리 인덱스로 맵 번호 구하기
	INT		GetBridgeIndexByMapNum( INT iMapNum );		// 맵 번호로 다리 인덱스 구하기

	INT		GetMapNumByBC_CHAOSGEM( INT iChaosGem );			// DS와 통신(혼석 생성을 위해)을 위해 맵 번호를 구한다.
	INT		GetMapNumByBC_ULTIMATEWEAPON( INT iUltimateWeapon );	// DS와 통신(절대무기 생성을 위해)을 위해 맵 번호를 구한다.
#endif

	
#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
	list <BLOODCASTLE_STARTTIME>	m_LST_BLOODCASTLE_START_TIME;
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624

};


extern CBloodCastle g_BloodCastle;

#endif // !defined(AFX_BLOODCASTLE_H__B6D64999_D5F0_453A_BBC9_A8567758D31D__INCLUDED_)
