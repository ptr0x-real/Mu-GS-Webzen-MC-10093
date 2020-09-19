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
#define CASTLE_PK_BORDER					113					// 성 주인 길드가 공성 준비기간에 PK 할수 있는 경계선 (apple)
#else
#define CASTLE_PK_BORDER					112					// 성 주인 길드가 공성 준비기간에 PK 할수 있는 경계선 (apple)
#endif
#define MIN_CS_REGSIEGE_GUILD_MEMBERCOUNT	20					// 공성 참여 요청 길드의 길드원 최소인원(apple)

#define	MAX_CS_SCHEDULE_COUNT				50					// 최대 공성 스케쥴 항목의 개수
#define	MAX_CS_NPC_COUNT					150					// 최대 공성 NPC 개수
#define MAX_CS_GUILD_COUNT					100					// 최대 공성관련 길드 개수
#define	DEFAULT_CASTLE_SIEGE_CYCLE			14					// 기본 공성주기 (일)
#define	DEFAULT_CS_LEFT_ALARM_TICK			(3*60*1000)			// 공성전 남은시간 알림 주기 (MSEC)
#define	DEFAULT_CS_LEFT_ALARM_TICK_U10		(1*60*1000)			// 공성전 남은시간 알림 주기 (MSEC) - 10분 이하일 때

#define	MAX_DBDATA_WAIT_TICK				(3*1000)			// DB 데이터가 올때까지 기다리는 최대 시간간격 (넘어가면 재 요청)
#define MAX_NPC_DBSAVE_TICK					(5*60*1000)			// NPC를 DB에 저장하는 시간간격
#define MAX_CALC_LEFTSIEGE_TICK				(10*1000)			// 공성전 시작까지 남은 시간을 계산하는 시간간격

#define CS_JOINSIDE_REFRESH_TICK			(10*1000)			// 공성전 초반에 공/수 양측의 참여상태를 갱신해 주는 시간간격
#define MAX_CS_JOINSIDE_REFRESH_COUNT		3					// 공성전 초반에 공/수 양측의 참여상태를 갱신해 주는 최대횟수

#define CS_MINIMAP_SEND_TICK				(3*1000)			// 공성전 중 미니맵 데이터를 전송해 주는 시간간격

#ifdef MU_CASTLESIEGE_START_NOTIFY_20050418
#define CS_STARTNOTIFY_MIN_TICK				(5*60*1000)			// 공성전 시작전 공지 시작시간
#define CS_STARTNOTIFY_PER_TICK				(1*60*1000)			// 공성전 시작전 공지 출력주기
#endif

#define CS_MSG_TICK_REGSIEGE				(30*60*1000)		// 이벤트 메시지 출력 시간 - 공성전 등록기간
#define CS_MSG_TICK_REGMARK					(30*60*1000)		// 이벤트 메시지 출력 시간 - 성주의표식 등록기간
#define CS_MSG_TICK_IDLE_3					(120*60*1000)		// 이벤트 메시지 출력 시간 - 공성전 참여길드 선정기간
#define CS_MSG_TICK_NOTIFY					(30*60*1000)		// 이벤트 메시지 출력 시간 - 공성전 참여길드 발표기간
#define CS_MSG_TICK_READYSIEGE				(30*60*1000)		// 이벤트 메시지 출력 시간 - 공성전 준비기간
#define CS_MSG_TICK_STARTSIEGE				(30*60*1000)		// 이벤트 메시지 출력 시간

#define	CS_MIN_SIEGESTART					(15)				// 공성전 시작 전 최소 시작시간
#define CS_CHECK_DBNPC_ALIVE_TICK			(5*1000)			// 성문/석상이 살아있는지 체크하는 시간

#define	CS_MOVENUM_LORENDEEP				23					// "/이동 로랜협곡" 의 MoveReq.txt 의 인덱스

#define MAX_UPGRADE_LEVEL					3					// 업그레이드 가능한 최대레벨
#define UPTYPE_DEFENCE						1					// 업그레이드 타입 - 방어력 레벨
#define UPTYPE_REGEN						2					// 업그레이드 타입 - 회복력 레벨
#define UPTYPE_MAXHP						3					// 업그레이드 타입 - 최대HP 레벨

#define	MAX_NPC_SEND_COUNT					50					// NPC리스트의 사용자에게 한번에 보낼 수 있는 최대 NPC 개수
#define	MAX_CS_MARKS_OVERLAP				255					// 성주의 표식을 겹칠 수 있는 최대 개수
	
#define MAX_CS_TAX_RATE_CHAOS				3					// 성의 최대 세율 (카오스조합 -> %)
#define MAX_CS_TAX_RATE_STORE				3					// 성의 최대 세율 (상점 -> %)
#define MAX_CS_TAX_HUNT_ZONE				300000				// 성의 최대 세율 (사냥터 -> 젠)
#define UNIT_CS_TAX_HUNT_ZONE				10000				// 성의 사냥터 세율단위 (apple)

#define	MAX_CS_ATTACK_GUILD					3					// 최대 공격측 길드수
#define	MAX_CS_REG_GUILD					100					// 최대 공성 등록 길드수
#define	MAX_CS_TOTAL_GUILD					150					// 최대 공격측 길드수
#define MAX_MINIMAP_USER_COUNT				1000				// 한 길드당 1000명까지 미니맵에 있는 길드원들을 볼 수 있다.

#define	CS_MARK_GIVEUP_MONEY				3000				// 공성을 포기한 길드마스터에게 환산해 주는 마크당 젠
#define	CS_MARK_EXCHANGE_MONEY				2500				// 마크를 환불받으려는 길드마스터에게 환산해 주는 마크당 젠

#ifdef MODIFY_CASTLE_CROWN_ACCESS_TIME_20070828		// 직인 누적시간 30초 -> 60초 변경
	#define CS_CASTLE_CROWN_ACCESS_TIME			(60*1000)			// 왕관의 직인에 걸리는 시간
#else
	#define CS_CASTLE_CROWN_ACCESS_TIME			(30*1000)			// 왕관의 직인에 걸리는 시간
#endif

#define CS_NPC_CROWN						216					// 왕관
#define CS_NPC_CROWN_SWITCH1				217					// 왕관 스위치 1
#define CS_NPC_CROWN_SWITCH2				218					// 왕관 스위치 2

#define CS_SIEGEGUILD_SEQ_PRIORITY			(1024*1024)			// 공성전 길드 선정 시 순서에 우선순위를 주기 위한 기본값

#define CHECK_NPC_CROWN_SWITCH(C_VALUE)		(C_VALUE<CS_NPC_CROWN_SWITCH1?FALSE:C_VALUE>CS_NPC_CROWN_SWITCH2?FALSE:TRUE)


#ifdef CASTLE_TEST_SCHEDULER_20041218
//#define	CS_TEST_GAPSEC						(60*5)				// 테스트 스케쥴러 적용 시 시간간격
#define	CS_TEST_GAPSEC						(60*1)				// 테스트 스케쥴러 적용 시 시간간격
#endif


// 방어력 업그레이드 테이블
// 1 . 성문
static INT g_iNpcUpDfLevel_CGATE[MAX_UPGRADE_LEVEL][3] = {		// 컬럼설명 (0:레벨,1:필요수호보석,2:필요젠,3:적용방어력)
	1,			2,	3000000,
	2,			3,	3000000,
	3,			4,	3000000,
};
// 2 . 석상
static INT g_iNpcUpDfLevel_CSTATUE[MAX_UPGRADE_LEVEL][3] = {	// 컬럼설명 (0:레벨,1:필요수호보석,2:필요젠,3:적용방어력)
	1,			3,	3000000,
	2,			5,	3000000,
	3,			7,	3000000,
};
// 최대 HP 업그레이드 테이블
#ifdef MODIFY_GATE_AND_STATUE_INCREASE_MAX_HP_20080718
// 1 . 성문
static INT g_iNpcUpMaxHP_CGATE[MAX_UPGRADE_LEVEL][3] = {		// 컬럼설명 (0:적용최대HP,1:필요수호보석,2:필요젠)
	2500000,	2,	1000000,
	3500000,	3,	1000000,
	5200000,	4,	1000000,
};
// 2 . 석상
static INT g_iNpcUpMaxHP_CSTATUE[MAX_UPGRADE_LEVEL][3] = {		// 컬럼설명 (0:적용최대HP,1:필요수호보석,2:필요젠)
	2200000,	3,	1000000,
	3400000,	5,	1000000,
	5000000,	7,	1000000,
};
#else
// 1 . 성문
static INT g_iNpcUpMaxHP_CGATE[MAX_UPGRADE_LEVEL][3] = {		// 컬럼설명 (0:적용최대HP,1:필요수호보석,2:필요젠)
	800000,		2,	1000000,
	1500000,	3,	1000000,
	3000000,	4,	1000000,
};
// 2 . 석상
static INT g_iNpcUpMaxHP_CSTATUE[MAX_UPGRADE_LEVEL][3] = {		// 컬럼설명 (0:적용최대HP,1:필요수호보석,2:필요젠)
	750000,		3,	1000000,
	1300000,	5,	1000000,
	2500000,	7,	1000000,
};
#endif // MODIFY_GATE_AND_STATUE_INCREASE_MAX_HP_20080718

// 회복력 업그레이드 테이블 (석상만 적용)
static INT g_iNpcUpRgLevel_CSTATUE[MAX_UPGRADE_LEVEL][3] = {	// 컬럼설명 (0:레벨,1:필요수호보석,2:필요젠)
	1,			3,	5000000,
	2,			5,	5000000,
	3,			7,	5000000,
};
// 공성참여 종류에 따라 구분된 기본 리스폰 위치
static INT g_iCsUserReSpawnArea[3][4] = {						// 각 참여종류에 따른 SX, SY, DX, DY
	35,		17,		144,	48,									// CS_SIEGESIDE_NONE
	74,		144,	115,	154,								// CS_SIEGESIDE_DEFEND
	35,		17,		144,	48,									// CS_SIEGESIDE_ATTACK
};

// 방어력 적용
// 1 . 성문
static INT g_iNpcDefense_CGATE[MAX_UPGRADE_LEVEL+1] = {			// 성문의 각 방어력 레벨 당 실제 적용될 방어력
	100,	// 방어력 - 0 단계
	180,	// 방어력 - 1 단계
	300,	// 방어력 - 2 단계
	520,	// 방어력 - 3 단계
};
// 2 . 석상
static INT g_iNpcDefense_CSTATUE[MAX_UPGRADE_LEVEL+1] = {		// 석상의 각 방어력 레벨 당 실제 적용될 방어력
	80,		// 방어력 - 0 단계
	180,	// 방어력 - 1 단계
	340,	// 방어력 - 2 단계
	550,	// 방어력 - 3 단계
};

// 성벽의 Y축 단계
#define		MAX_CASTLEWALL_STEP				3
const INT g_iCastleWallStepAxisY[MAX_CASTLEWALL_STEP] = {
	113,	// 성벽 - 1단계
	160,	// 성벽 - 2단계
	203,	// 성벽 - 3단계
};

// 데이터 로드 단계
enum {
	CASTLESIEGE_DATALOAD_NONE				= 0,				// 상태없음
	CASTLESIEGE_DATALOAD_1					= 1,				// 최초 파일 데이터를 로드해야 한다.
	CASTLESIEGE_DATALOAD_2					= 2,				// 파일 데이터 로드 후 DB 데이터 로드 필요함
	CASTLESIEGE_DATALOAD_3					= 3,				// DB 데이터 로드 요청 후 데이터 기다리고 있음
	CASTLESIEGE_DATALOAD_4					= 4,				// db 데이터 로드 후 시작 기다리고 있음
};

// 공성전 상태
enum {
	CASTLESIEGE_STATE_NONE					= -1,				// 상태없음
	CASTLESIEGE_STATE_IDLE_1				= 0,				// 유휴기간 - 1
	CASTLESIEGE_STATE_REGSIEGE				= 1,				// 신청기간 (공성주기 시작)
	CASTLESIEGE_STATE_IDLE_2				= 2,				// 유휴기간 - 1
	CASTLESIEGE_STATE_REGMARK				= 3,				// 문장 등록기간
	CASTLESIEGE_STATE_IDLE_3				= 4,				// 유휴기간 - 2
	CASTLESIEGE_STATE_NOTIFY				= 5,				// 발표기간
	CASTLESIEGE_STATE_READYSIEGE			= 6,				// 준비기간
	CASTLESIEGE_STATE_STARTSIEGE			= 7,				// 공성시작
	CASTLESIEGE_STATE_ENDSIEGE				= 8,				// 휴전기간
	CASTLESIEGE_STATE_ENDCYCLE				= 9,				// 공성주기 종료
};

// 공성전 참여길드 종류 (OBJECTSTRUCT의 m_btCsJoinSide)
enum {
	CS_SIEGESIDE_NONE						= 0,				// 공/수 구분 - 구분없음
	CS_SIEGESIDE_DEFEND						= 1,				// 공/수 구분 - 수성측
	CS_SIEGESIDE_ATTACK						= 2,				// 공/수 구분 - 공성측 (이 값을 기본으로 해서 길드마다 +1 씩 증가한다.)

#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
	CS_SIEGESIDE_ATTACK_2					= 3,
	CS_SIEGESIDE_ATTACK_3					= 4,
#endif
	
};

// 공성전 시간에 대한 상태 (0 이상이면 초)
enum {
	CASTLESIEGE_LEFTTIME_ONSIGE				= -1,				// 공성전 진행 중
	CASTLESIEGE_LEFTTIME_ENDSIEGE			= -2,				// 공성전 종료
	CASTLESIEGE_LEFTTIME_NOTRUNNING			= -3,				// 공성전 진행 아님
};

// NPC 존재에 대한 상태
enum {
	NPC_LIVE_NONE							= 0,				// NPC 가 존재하지 않음
	NPC_LIVE_READY							= 1,				// NPC 가 살아날 준비 (DB에서 NPC 데이터를 읽은 직후)
	NPC_LIVE_OK								= 2,				// NPC 가 존재하고 있음
};

// 공성전 NPC의 종류
enum {
	CS_NPC_TYPE_NONE						= 0,				// 공성전 NPC가 아님
	CS_NPC_TYPE_DB_DFN						= 1,				// 공성전 NPC - 수성측 DB
	CS_NPC_TYPE_INS_DFN						= 2,				// 공성전 NPC - 수성측
	CS_NPC_TYPE_INS_ATK						= 3,				// 공성전 NPC - 공성측
};

// 공성 NPC 업그레이드 종류
enum {
	CS_UPGRADE_TYPE_DEFENSE					= 1,				// 업그레이드 - 방어력
	CS_UPGRADE_TYPE_REGENARATION			= 2,				// 업그레이드 - 회복력
	CS_UPGRADE_TYPE_MAXHITPOINT				= 3,				// 업그레이드 - 최대HP
};

// 세율 종류
enum {
	CS_TAXRATE_TYPE_CHAOS					= 1,				// 세율 종류 - 카오스 조합
	CS_TAXRATE_TYPE_STORE					= 2,				// 세율 종류 - 상점
	CS_TAXRATE_TYPE_HUNTZONE				= 3,				// 세율 종류 - 사냥터
};

// 성문의 상태 종류
enum {
	CS_GATE_STATE_CLOSED					= 0,				// 현재 성문의 상태 - 닫힘
	CS_GATE_STATE_OPENED					= 1,				// 현재 성문의 상태 - 열림
};


#ifdef CASTLE_GM_COMMAND_20041222								// 운영자 공성제어 명령셋
enum {
	CS_GMCMD_CHANGEOWNERGUILD				= 0,				// "/공성수성길드"
	CS_GMCMD_SETSTATE_REGSIEGE				= 1,				// "/공성참여등록"
	CS_GMCMD_SETSTATE_REGMARK				= 2,				// "/공성문장등록"
	CS_GMCMD_SETSTATE_NOTIFY				= 3,				// "/공성선포"
	CS_GMCMD_SETSTATE_STARTSIEGE			= 4,				// "/공성시작"
	CS_GMCMD_SETSTATE_ENDSIEGE				= 5,				// "/공성종료"
	CS_GMCMD_SETSTATE_GETOWNERGUILD			= 6,				// "/공성현재수성"
	CS_GMCMD_SETSTATE_GETCASTLESTATE		= 7,				// "/공성현재상태"
	CS_GMCMD_SETSTATE_CHANGECSSIDE			= 8,				// "/공수교체
};
#endif


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_SCHEDULE_DATA
//*---------------------------------------------------------------
// DESC : 공성전 스케쥴 관리용 구조체
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_SCHEDULE_DATA {
	BOOL	m_bIN_USE;						// 현재 이 객체를 사용중인지 여부
	INT		m_iSTATE;						// 스케쥴 상태번호
	INT		m_iADD_DAY;						// 상대시간 - 일
	INT		m_iADD_HOUR;					// 상대시간 - 시
	INT		m_iADD_MIN;						// 상대시간 - 분
	INT		m_iGAP_SEC;						// 바로 다음 주기와의 상대시간 (초)

	_CS_SCHEDULE_DATA			();				
	virtual ~_CS_SCHEDULE_DATA	();				

	VOID	Clear				();			// 데이터 초기화
} CS_SCHEDULE_DATA, * LPCS_SCHEDULE_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_REG_GUILD_DATA
//*---------------------------------------------------------------
// DESC : 공성전 공격측 길드 선정용 데이터 구조체
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_REG_GUILD_DATA {
	BOOL	m_bIN_USE;						// 현재 이 객체를 사용중인지 여부
	string	m_strGuildName;
	INT		m_iRegMarkCount;				// 등록된 문장수
	INT		m_iGuildMemberCount;			// 길드멤버 수
	INT		m_iGuildMasterLevel;			// 길드마스터 레벨
	INT		m_iSeqNum;						// 등록 순서
	INT64	m_i64TotolScore;				// 위의 항목들을 집계해서 나온 점수

	_CS_REG_GUILD_DATA			();
	virtual ~_CS_REG_GUILD_DATA	();

	VOID	Clear				();			// 데이터 초기화
} CS_REG_GUILD_DATA, * LPCS_REG_GUILD_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_TOTAL_GUILD_DATA
//*---------------------------------------------------------------
// DESC : 공성전 관련 전체길드의 데이터 구조체
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_TOTAL_GUILD_DATA {
	BOOL	m_bIN_USE;						// 현재 이 객체를 사용중인지 여부
	INT		m_iCsGuildID;					// 공/수 각 길드의 고유 ID (1:수성 / 2이상:공성)
	BOOL	m_bGuildInvolved;				// 본 길드가 공성 당사자 길드인가 ?
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
	INT		m_iGuildScore;					// 참여길드로 선정될 때 길드의 점수
#endif	

	_CS_TOTAL_GUILD_DATA			();
	virtual ~_CS_TOTAL_GUILD_DATA	();

	VOID	Clear					();		// 데이터 초기화
} CS_TOTAL_GUILD_DATA, * LPCS_TOTAL_GUILD_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_MINIMAP_DATA
//*---------------------------------------------------------------
// DESC : 공성전 관련 미니맵 데이터 구조체
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_MINIMAP_DATA {
	INT		m_iGuildMasterIndex;
	CHAR	m_szGuildName[16];
	INT		m_iMiniMapPointCount;
	PMSG_SENDMINIMAPDATA	m_stMiniMapPoint[MAX_MINIMAP_USER_COUNT];

	_CS_MINIMAP_DATA				();
	virtual ~_CS_MINIMAP_DATA		();

	VOID	Clear					();		// 데이터 초기화
} CS_MINIMAP_DATA, * LPCS_MINIMAP_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : STRUCT
// NAME : CS_NPC_DATA
//*---------------------------------------------------------------
// DESC : 공성전 NPC 데이터 관리용 구조체
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
typedef struct _CS_NPC_DATA {	
	// 공성 NPC 관련 데이터
	BOOL	m_bIN_USE;						// 현재 이 객체를 사용중인지 여부
	INT		m_iNPC_NUM;						// NPC 종류번호
	INT		m_iNPC_INDEX;					// NPC 데이터 내의 인덱스 (각 종류별로 고유하게 유지)
	BOOL	m_bNPC_DBSAVE;					// DB저장을 해야하는지 아닌지
	INT		m_iNPC_SIDE;					// 어느 쪽 편인지 여부 (1 : 수성 / 2 : 공성)
	INT		m_iNPC_OBJINDEX;				// OBJECTSTURCT 구조체 배열에서의 인덱스

	// 변화 가능한 값들
	INT		m_iNPC_DF_LEVEL;				// 방어력 레벨
	INT		m_iNPC_RG_LEVEL;				// 회복력 레벨
	INT		m_iNPC_MAXHP;					// 최대체력
	INT		m_iNPC_HP;						// 체력
	INT		m_iNPC_SX;						// 좌표 - 시작 X
	INT		m_iNPC_SY;						// 좌표 - 시작 Y
	INT		m_iNPC_DX;						// 좌표 - 종료 X
	INT		m_iNPC_DY;						// 좌표 - 종료 Y
	INT		m_iNPC_DIR;						// NPC 방향 (-1은 랜덤)

	// 초기 값들
	INT		m_iNPC_BASE_DF_LEVEL;			// 방어력 레벨
	INT		m_iNPC_BASE_RG_LEVEL;			// 회복력 레벨
	INT		m_iNPC_BASE_MAXHP;				// 최대체력
	INT		m_iNPC_BASE_HP;					// 체력
	INT		m_iNPC_BASE_SX;					// 좌표 - 시작 X
	INT		m_iNPC_BASE_SY;					// 좌표 - 시작 Y
	INT		m_iNPC_BASE_DX;					// 좌표 - 종료 X
	INT		m_iNPC_BASE_DY;					// 좌표 - 종료 Y
	INT		m_iNPC_BASE_DIR;				// NPC 방향 (-1은 랜덤)

	// 특정 NPC들을 위한 값들
	INT		m_iCS_GATE_LEVER_INDEX;			// 성문전용 - 레버의 인덱스값 (객체 생성 시 최초 한번만 -1)

	BOOL	m_iNPC_LIVE;					// 존재하는지 여부
	INT		m_iNPC_AUTH;					// NPC 접근권한 (성문만 사용)

	union {
		struct {
			BYTE	m_btCsNpcExistVal1;		// NPC가 유효한지 확인하기 위한 값 - 1 (1 ~ 255)
			BYTE	m_btCsNpcExistVal2;		// NPC가 유효한지 확인하기 위한 값 - 2 (0 ~ 255)
			BYTE	m_btCsNpcExistVal3;		// NPC가 유효한지 확인하기 위한 값 - 3 (0 ~ 255)
			BYTE	m_btCsNpcExistVal4;		// NPC가 유효한지 확인하기 위한 값 - 4 (0 ~ 255)
		};
		INT			m_iCsNpcExistVal;		// NPC가 유효한지 확인하기 위한 값 (0이면 무효)
	};

	_CS_NPC_DATA			();				
	virtual ~_CS_NPC_DATA	();				

	VOID	Clear			();				// 데이터 초기화
	VOID	SetBaseValue	();				// 기본 데이터의 값으로 세팅
} CS_NPC_DATA, * LPCS_NPC_DATA;


//================================================================


//*---------------------------------------------------------------
// TYPE : CLASS
// NAME : CCastleSiege
//*---------------------------------------------------------------
// DESC : 공성전 스케쥴 관리 클래스
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
class CCastleSiege  
{
public:
	CCastleSiege					();
	virtual ~CCastleSiege			();
	
	VOID	Clear					();
	BOOL	Ready					(INT iMapSvrGroup);
	BOOL	LoadPreFixData			(LPSTR lpszFileName);		// 유동적이고 한정적인 데이터를 읽음 (commonserver.cfg)
	BOOL	LoadData				(LPSTR lpszFileName);		// 고정적인 데이터를 읽음 (MuCastleData.dat)
	BOOL	DataRequest				();	
	BOOL	Init					();
	VOID	Run						();
	
	static bool						ScheduleStateCompFunc		(CS_SCHEDULE_DATA & A, CS_SCHEDULE_DATA & B);

	// 현재 데이터를 읽고있는 상태 정보
	BOOL	GetDataLoadState		()							{	return m_iCastleDataLoadState;				}
	VOID	SetDataLoadState		(INT iDataLoadState)		{	m_iCastleDataLoadState	= iDataLoadState;	}

	// 파일 데이터를 읽었는지 여부
	BOOL	GetFileDataLoadOK		()							{	return m_bFileDataLoadOK;					}
	VOID	SetFileDataLoadOK		(BOOL bFileDataLoadOK)		{	m_bFileDataLoadOK	= bFileDataLoadOK;		}

	// DB 데이터를 읽었는지 여부
	BOOL	GetDbDataLoadOK			()							{	return m_bDbDataLoadOK;						}
	VOID	SetDbDataLoadOK			(BOOL bDbDataLoadOK)		{	m_bDbDataLoadOK	= bDbDataLoadOK;			}

	// DB NPC들을 최초로 세팅 -> DB에 저장
	BOOL	FirstCreateDbNPC		();

	// DB 로 부터 공성전 데이터 받아서 초기화 -> [0x81] 패킷
	// 1 . 전체 데이터 세팅
	BOOL	SetCastleInitData		(
			LPCSP_ANS_CSINITDATA lpMsg
			);
	// 2 . 수성 NPC 데이터 세팅
	BOOL	SetCastleNpcData		(
			LPCSP_CSINITDATA lpMsg, 
			INT iCOUNT
			);

	BOOL 	CheckSync				();							// 파일 데이터의 시간과 현재 시간을 바탕으로 성의 시간 싱크를 맞춘다. (예외 상황 포함)
	VOID	GetNextDay				(							// 다음 날짜를 구하는 함수
				SYSTEMTIME &st, 
				INT iAfterDay,
				INT iAfterHour		= 0, 
				INT iAfterMin		= 0, 
				INT iAfterSec		= 0
				);	
#ifdef MODIFY_SIEGE_END_DATE_BUG_FIX_20060116
	VOID		GetStateDate			(INT iCastleSiegeState, SYSTEMTIME* st);	// 특정 상태의 다음 상태까지의 시간(Day, Hour, Min)
#endif
	INT		GetStateGapSec			(INT iCastleSiegeState);	// 특정 상태의 다음 상태까지의 시간간격 (SEC) 을 구한다. -> 이미 계산된 수치


	//===============================================
	// 실제적인 공성전 진행 관련 메서드들

	// 1> . 공성전 스케쥴 관련 메서드들
	// 1 . 특정상태 시작
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

	// 2 . 특정상태 동작
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

	// 2> . 공성전 진행관련 메서드들
	// 1 . 공성전 관련 각종 상태반환
	INT			GetCastleState			()						{	return m_iCastleSiegeState;		}
	SYSTEMTIME	GetCastleLeftSiegeDate	()						{	return m_tmLeftCastleSiegeDate; }
	INT			CalcCastleLeftSiegeDate	();
	BOOL		GetCastleStateTerm		(						// 현재 공성전 상태의 시작, 끝 기간을 구한다.
				SYSTEMTIME & tmStateStartDate, 
				SYSTEMTIME & tmStateEndDate
				);
	INT			GetCurRemainSec			();
	
	// 2 . NPC 관련
	// NPC - 관리 관련
	VOID	ClearDbNPC				();							// DB에 저장하는 NPC들 만 삭제 -> 수동변경
	VOID	ClearNonDbNPC			();							// DB에 저장하지 않는 NPC들 만 삭제 -> 수동변경
	VOID	ClearNonDbNPC_MidWin	();							// DB에 저장하지 않는 NPC들 중 일부를 중간 승리 시 삭제
	VOID	ClearAllNPC				();							// ClearDbNPC() + ClearNonDbNPC()

	VOID	CreateDbNPC				();							// DB에 저장하는 NPC들을 모두 생성 (NPC_LIVE_READY 상태인 것)
	VOID	CreateNonDbNPC			(BOOL bDelFirst = TRUE);	// DB에 저장하지 않는 NPC들을 모두 생성 (생성 전에 임의로 한번 지울지 결정)
	BOOL	CheckAddDbNPC			(							// DB에 저장하는 NPC중 하나를 추가 가능한지 체크 -> 생성 후 DB저장 함 (공성전 중이 아닐때 만)
			INT iIndex,
			INT iNpcType, 
			INT iNpcIndex,
			BYTE &btResult
			);
	BOOL	AddDbNPC				(							// DB에 저장하는 NPC중 하나를 추가 -> 위에서 저장 요청 시 이미 저장됨
			INT iNpcType, 
			INT iNpcIndex
			);
	BOOL	RepairDbNPC				(							// DB에 저장하는 NPC중 하나를 수리 -> 이 전에 이미 DB에서 수리됨
			INT iNpcType, 
			INT iNpcIndex,
			INT iNpcHP,
			INT iNpcMaxHP
			);
#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
	BOOL	PayForUpgradeDbNpc		(							// DB에 저장하는 NPC중 하나를 업그레이드하기 위해 필요한 비용을 지불한다.
			INT iIndex,
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcUpType,
			INT	iNpcUpValue,
			INT	iNpcUpIndex
			);
#endif	
	BOOL	UpgradeDbNPC			(							// DB에 저장하는 NPC중 하나를 업그레이드 -> 이 전에 이미 DB에서 업그레이드 됨
			INT iIndex,
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcUpType,
			INT	iNpcUpValue,
			INT	iNpcUpIndex
			);
	BOOL	UpgradeDbNPC_DFLEVEL	(							// DB에 저장하는 특정 NPC의 방어력 업그레이드
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcDfLevel
			);
	BOOL	UpgradeDbNPC_RGLEVEL	(							// DB에 저장하는 특정 NPC의 회복력 업그레이드
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcRgLevel
			);
	BOOL	UpgradeDbNPC_MAXHP		(							// DB에 저장하는 특정 NPC의 최대HP 업그레이드
			INT	iNpcType,
			INT	iNpcIndex,
			INT	iNpcMaxHP
			);
	VOID	StoreDbNpc				();							// DB에 저장하는 NPC들을 일괄적으로 DB에 저장 (이전 데이터 삭제)
	BOOL	DelNPC					(							// 공성전 NPC를 죽은 상태로 변경 -> DB NPC라면 죽인 후 DB제거 함 (공성전 중이 아닐때 만)
			INT iIndex,
			INT iNpcType, 
			INT iMonsterExistVal,
			BOOL bDbSave = FALSE
			);
	VOID	SendNpcStateList		(							// 특정 사람에게 특정 몬스터의 현재 상태 리스트를 보내준다.
			INT iIndex, 
			INT iNpcType
			);
	BOOL	GetNpcData				(							// 현재 관리되는 특정 NPC에 대한 정보를 보냄 (FALSE는 데이터 없음)
			INT iNpcType, 
			INT iNpcIndex,
			CS_NPC_DATA & pRetNpcData							// 결과로 반환되는 NPC 데이터
			);
	VOID	AdjustDbNpcLevel		();							// DB NPC의 현재 자료상의 방어력/회복력 레벨을 사용자 객체에 적용한다. -> 공성 시작 전에는 이렇게 해주어야 한다.
	
	// NPC - 성문, 레버 관련
	VOID	SendCsGateStateViewPort	(							// 성의 문 상태를 나를 보고 있는 사람들에게 보두 보냄
			INT iGateIndex,
			BYTE btOperate
			);
	VOID	CreateAllCsGateLever	();							// 공성 데이터 세팅이 성공적으로 끝난 직후에 레버를 일괄적으로 생성해 준다.
	INT		CreateCsGateLever		(							// 특정 지역에 성문의 레버를 만들고 인덱스를 리턴한다. (실패는 -1)
			INT iX, 
			INT iY
			);
	BOOL	LinkCsGateLever			(							// 이미 성문이 만들어져 있다면 특정 성문과 연결한다.
			INT iLeverIndex,
			INT iGateIndex
			);
	BOOL	CheckLeverAlive			(							// 특정 인덱스가 성문레버이고 살아있는지를 확인
			INT iLeverIndex
			);
	BOOL	CheckCsGateAlive		(
			INT iGateIndex
			);
	VOID	CheckCsDbNpcAlive		();							// 성문들이 살아있는지 확인하여 죽었다면 블럭속성 해제
	
	// NPC - 왕관 관련
	VOID	SetCrownIndex			(INT iIndex)				{	m_iCrownIndex	= iIndex;					}	// 왕관 NPC 인덱스를 세팅한다. -> 왕관 보호막 상태 해제를 위해 필요
	VOID	NotifyCrownState		(BYTE btState);				// 현재 왕관의 상태를 주변에 알림 (0:보호막해제/1:보호막생성/2:등록성공)
	BOOL	GetRegCrownAvailable	()							{	return m_bRegCrownAvailable;				}
	VOID	SetRegCrownAvailable	(BOOL bRegCrownAvailable)	{	m_bRegCrownAvailable = bRegCrownAvailable;	}
	BOOL	GetCastleTowerAccessable()							{	return m_bCastleTowerAccessable;			}
	VOID	SetCastleTowerAccessable(BOOL bTowerAccessable)		{	m_bCastleTowerAccessable =bTowerAccessable;	}
	
#ifdef MODIFY_CASTLESIEGE_NOTIFY_CROWNSWITCH_USER_20051017
	// NPC - 왕관 스위치 관련
	VOID	NotifyCrownSwitchInfo	(INT iCrownSwitchIndex);
#endif


	// 3 . 게임운영관련
	BOOL	DelGemOfDefend			(							// 특정 사용자에게서 특정 개수의 수호보석을 제거한다.
			INT iIndex,
			INT iNeedGemOfDefend
			);
	VOID	SetTaxRate (										// 세율을 조정하고 다른 게임서버와 동기화 한다.
			INT iTaxType,
			INT iTaxRate
			);
	BOOL	CheckCastleHasMoney		(							// 현재 성에 돈이 그정도 있는지 확인한다.
			INT64 i64CastleMoney
			);
	INT64	GetCastleMoney			()							{	return m_i64CastleMoney;	}
	VOID	SetCastleMoney			(							// 현재 성의 돈을 특정 액수로 맞춘다.
			INT64 i64CastleMoney
			);
	VOID	ResetCastleTaxInfo		();							// 성의 돈과 세율정보를 초기화 한다.
	VOID	SetIsSiegeGuildList		(BOOL bIsSiegGuildList)		{	m_btIsSiegeGuildList = bIsSiegGuildList;	}
	VOID	SendCastleStateSync		();							// 현재 성의 상태를 맵서버군 전체에 동기화 한다.
	BOOL	GetDbNpcCreated			()							{	return m_bDbNpcCreated;						}
	VOID	SetDbNpcCreated			(BOOL bDbNpcCreated)		{	m_bDbNpcCreated	= bDbNpcCreated;			}
	BOOL	CheckGuildOwnCastle		(LPSTR lpszGuildName);		// 해당 길드가 성을 소유한 길드가 맞는지 확인한다.
	VOID	ResetCastleCycle		();							// 공성 주기를 다시 시작하면서 필요한 데이터들을 리셋한다.
	VOID	SetGateBlockState		(							// 성문의 블록 속성을 변경한다.
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

	// *> . 길드관리 관련
	// 길드정보 구축 체크
	VOID	CheckBuildCsGuildInfo	();							// 공성관련 전체길드 정보를 구축하기 위해 상황1 과 2 중 선택한다.
	// 길드정보 구축 1 - 없는 상태에서 새로 구축
	VOID	SetCalcRegGuildList		(							// 공성측 최종선정 후보 데이터를 세팅한다. (이전 데이터가 있었다면 초기화 한다.)
			LPCSP_CALCREGGUILDLIST lpMsg,
			INT iCOUNT
			);
	VOID	MakeCsTotalGuildInfo	(							// 공/수의 연합길드 정보를 받아서 기존 길드정보와 함께 완벽한 길드정보를 구축한다.
			LPCSP_CSGUILDUNIONINFO lpMsg,
			INT iCOUNT
			);
	VOID	SaveCsTotalGuildInfo	();							// 공/수의 전체길드 정보를 DB에 저장하도록 요청한다.

	// 길드정보 구축 2 - 있는 상태에서 DB에서 받아서 바로 구축
	VOID	SetCsTotalGuildInfo		(							// 공/수의 연합길드 정보를 받아서 기존 길드정보와 함께 완벽한 길드정보를 구축한다.
			LPCSP_CSLOADTOTALGUILDINFO lpMsg,
			INT iCOUNT
			);
	// 길드 공/수 참가측 관리 관련
	BOOL	GetCsJoinSide			(							// 특정 길드의 현재 공/수 참여 타입을 반환하고 상태패킷을 보내준다.
			LPSTR lpszGuildName,
			BYTE &btCsJoinSide,
			bool &bCsGuildInvolved
			);
	VOID	SetAllUserCsJoinSide	();							// 접속중인 모든 사용자 들의 현재의 공/수 참여 여부값을 세팅해준다.
	VOID	ResetAllUserCsJoinSide	();							// 접속중인 모든 사용자 들의 현재의 공/수 참여 여부값을 해제해준다.
	VOID	NotifySelfCsJoinSide	(INT iIndex);				// 특정 사용자의 공/수 참여 여부값을 보내준다.

	// *> . 성문관련
	BOOL	OperateGate				(							// 외부 사용자객체 인덱스로 성문을 작동시킨다.
			INT		iObjIndex,
			INT		iMonsterExistVal,
			BOOL	bOpenType									// (0:닫기/1:열기)
			);
	BOOL	OperateGate				(							// 내부 NPC관리 인덱스로 성문을 작동시킨다.
			INT		iNpcIndex,
			BOOL	bOpenType									// (0:닫기/1:열기)
			);

	// *> . 공성판정 관련
	BOOL	CheckAttackGuildExist	();							// 공성측 길드가 존재하는지 확인한다.
	BOOL	CheckCastleOwnerMember	(INT iIndex);				// 해당 사용자가 성을 차지한 길드의 소속인지 확인한다.
	BOOL	CheckCastleOwnerUnionMember		(					// 특정 사용자가 자신이 속한 길드연합의 일원인지 확인한다.
			INT iIndex
			);
	BOOL	CheckGuardianStatueExist();							// 수호석상이 존재하는지 확인한다.
	VOID	ChangeWinnerGuild		(INT iCsJoinSide);			// 수성측 길드를 바꾼다 -> 중간 승리 시 사용
	BOOL	CheckMiddleWinnerGuild	();							// 판정!!> . 공성전 중간승리 길드가 나타났는지 확인한다.
	BOOL	CheckCastleSiegeResult	();							// 판정!!> . 공성전 최종결과를 체크한다.

	BOOL	CheckUnionGuildMaster	(INT iIndex);				// 특정 사용자가 자신이 속한 길드연합의 연합 마스터인지 확인한다.

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

	VOID	SetCrownAccessTickCount		();						// 왕관 접근 시간을 설정한다.
	VOID	ResetCrownAccessTickCount	();						// 왕관 접근 시간을 초기화한다.

	// *> . 공성 외적인 부분의 운영 관련
	INT		CheckOverlapCsMarks			(INT iIndex);			// 특정 사용자의 인벤토리에 성주의 표식을 겹칠 수 있다면 몇번째 칸인지 알려준다.
	INT		GetCsAttkGuildList			(						// 공성 참여 길드의 리스트를 가져온다. [0xB5] (결과 - 0:실패/1:성공/2:기한않됨/3:데이터없음)
			LPPMSG_CSATTKGUILDLIST lpMsgBody,
			INT & iCount
			);
	BOOL	CheckTeleportMagicAxisY		(						// 텔레포트 마법 시전 시 성벽에 걸리지 않는지 체크한다.
			INT iStartY,
			INT iTargetX,
			INT iTargetY
			);
	VOID	NotifyCsSelfLeftTime		(						// 공성전 진행중의 남은 시간을 보내준다.
			INT iIndex
			);

	// *> . 공성 시 전투, 운영 관련
	VOID	CheckReviveNonDbNPC			();						// DB에 저장하지 않는 NPC를 생성한다.
	VOID	CheckReviveGuardianStatue	();						// 수호석상 중 현재 죽어있는 것이 있다면 DB저장 없이 바로 추가시킨다.
	VOID	ReSpawnAllUser				();						// 현재 공성맵 안에 있는 모든 사람들을 각각의 역할에 맞게 지정된 위치로 강제 이동 시킨다.
	VOID	ReSpawnEnemyUser			(						// 현재 공성맵 안에 있는 수성측이 아닌 모든 사람들을 성 밖으로 강제 이동 시킨다.
			BOOL bRefreshOwnerUser = FALSE
			);
	VOID	NotifyAllUserCsStartState	(BYTE btStartState);	// 현재 공성전이 시작되었는지 여부를 모든 사용자들에게 알린다.
	VOID	NotifyAllUserCsProgState	(						// 현재 공성전의 진행 상태를 모든 사용자들에게 알린다.
			BYTE btProgState,
			LPSTR lpszGuildName
			);
	VOID	ClearCastleTowerBarrier		();						// 현재 존재하는 모든 방어막 NPC를 제거한다.
	VOID	SetAllCastleGateState		(BOOL bOpenType);		// 모든 존재하는 성문의 상태를 변경한다.
	// 공성 운영 - 미니맵 관련
	VOID	AddMiniMapDataReqUser		(INT iIndex);			// 미니맵 데이터 요청 인원을 등록한다.
	VOID	DelMiniMapDataReqUser		(INT iIndex);			// 미니맵 데이터 요청 인원을 제거한다.
	VOID	OperateMiniMapWork			();						// 미니맵 관련 작업을 수행한다. -> 데이터 구축 전송


	// *> . 공지, 데이터 전송 관련
	VOID	SendMapServerGroupMsg	(							// 맵서버 그룹 전체의 사용자에게 특정 메시지를 보낸다.
			LPSTR lpszMsg 
			);
	VOID	SendAllUserAnyData		(							// 서버에 접속해 있는 모든 사용자들에게 특정 데이터를 보낸다.
			LPBYTE lpMsg, 
			INT iSize
			);
	VOID	SendAllUserAnyMsg		(							// 서버에 접속해 있는 모든 사용자들에게 특정 메시지를 보낸다.
			LPSTR lpszMsg, 
			INT iType	= 1										// 메시지 타입 (1-슬라이딩/2-중앙공지)			
			);
	VOID	SendCsUserAnyData		(							// 공성전맵에 접속해 있는 모든 사용자들에게 특정 데이터를 보낸다.
			LPBYTE lpMsg, 
			INT iSize
			);
	VOID	SendCsUserAnyMsg		(							// 공성전맵에 접속해 있는 모든 사용자들에게 특정 메시지를 보낸다.
			LPSTR lpszMsg
			);

	static bool						RegGuildScoreCompFunc		(CS_REG_GUILD_DATA & A, CS_REG_GUILD_DATA & B);

#ifdef CASTLE_GM_COMMAND_20041222								// 운영자 공성제어 명령처리
	VOID	OperateGmCommand		(
			INT iIndex,
			INT iGmCommand,
			LPVOID lpParam	= NULL
			);
	VOID	CreateDbNPC_INS			(							// 성문, 석상 등 DB NPC를 DB 저장 없이 바로 만든다.
			);
#endif
	
#ifdef CASTLE_SAVE_PCROOM_USERLIST_20050105						// 공성전 참여자들의 PC방 정보 저장
	VOID	SavePcRoomUserList		();
#endif

private:
	BOOL						m_bCastleSiegeEnable;			// 공성전이 비활성화 되는가 ?
	
	BOOL						m_bFileDataLoadOK;				// 파일 데이터를 제대로 읽었는지 여부 ?
	BOOL						m_bDbDataLoadOK;				// DB 데이터를 제대로 읽었는지 여부 ?

	BOOL						m_bDoRun;						// 공성 스케쥴을 진행할 수 있는지 여부
	BOOL						m_iMapSvrGroup;					// 본 성이 속해있는 맵서버군 코드번호
	
	INT							m_iCastleDataLoadState;			// 공성관련 데이터의 현재 로드 중인 상태
	DWORD						m_dwDbDataLoadTickCount;		// DB 데이터를 로드하기 위한 틱카운트

	INT							m_iCastleSiegeCycle;			// 공성주기
	INT							m_iCastleSiegeState;			// 공성상태

#ifdef MODIFY_SIEGE_END_DATE_BUG_FIX_20060116
	SYSTEMTIME					m_tmSiegeEndSchedule;			// 공성 예상 종료 시간
#endif

	BOOL						m_bFixCastleCycleStartDate;		// 공성주기 시간 강제적용 여부 (commonserver.cfg)
	SYSTEMTIME					m_tmFixCastleCycleStartDate;	// 공성주기 시간 강제적용

	INT							m_iFixCastleSpecificState;		// 공성 강제 적용된 특정상태
	BOOL						m_bFixCastleStateStartDate;		// 공성 특정상태 시간 강제적용 여부 (commonserver.cfg)
	SYSTEMTIME					m_tmFixCastleStateStartDate;	// 공성 특정상태 시간 강제적용

	INT							m_iCS_REMAIN_MSEC;				// 다음 주기까지 남은 시간 (MSEC)
	INT							m_iCS_TICK_COUNT;				// 카운트 작업들을 하기위한 시간 (1/1000초)

	SYSTEMTIME					m_tmStateStartDate;				// 특정 상태가 시작한 시간
	SYSTEMTIME					m_tmLeftCastleSiegeDate;		// 남은 예상 공성 시작시간
	
	DWORD						m_dwNPC_DBSAVE_TICK_COUNT;				// NPC가 DB에 저장될 시간을 체크하는데 사용 (N분 마다 한번씩)
	DWORD						m_dwCALC_LEFTSIEGE_TICK_COUNT;			// 공성전 시작까지 남은 시간을 계산하는데 사용 (N초 마다 한번씩)
	DWORD						m_dwEVENT_MSG_TICK_COUNT;				// 이벤트 메시지가 출력될 시간을 체크하는데 사용
	DWORD						m_dwCHECK_GATE_ALIVE_COUNT;				// 게이트가 존재하는지 체크하는데 사용
	DWORD						m_dwCS_JOINSIDE_REFRESH_TICK_COUNT;		// 공성 시작 직후 공/수 참여상태를 갱신해 주는데 사용
	DWORD						m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT;	// 공성 시작 직후 공/수 참여상태를 갱신해 주는데 사용
	DWORD						m_dwCS_MINIMAP_SEND_TICK_COUNT;			// 공성전 중 미니맵 데이터를 전송해 주는데 사용
	DWORD						m_dwCS_LEFTTIME_TICK_COUNT;				// 공성전 중 남은 시간을 모든 사용자들에게 알리는데 사용
	DWORD						m_dwCS_STARTTIME_TICK_COUNT;			// 공성전 시작전 남은 시간을 모든 사용자들에게 알리는데 사용


	// 공성전 관련 데이터들
	// 1 . 기본 데이터
	SYSTEMTIME					m_tmStartDate;					// 공성주기 시작일
	SYSTEMTIME					m_tmEndDate;					// 공성주기 종료일
	BYTE						m_btIsSiegeGuildList;			// 공성관련 길드정보가 이미 구축되었는가 ?
	BYTE						m_btIsSiegeEnded;				// 공성이 이미 치루어 졌는가 ?
	BYTE						m_btIsCastleOccupied;			// 성이 이미 사용자들에게 차지된 상태인가 ?
	CHAR						m_szCastleOwnerGuild[16];		// 캐슬을 차지한 길드명 (없으면 "")
	INT64						m_i64CastleMoney;				// 현재 성의 보유 젠
	INT							m_iTaxRateChaos;				// 카오스 조합 세율
	INT							m_iTaxRateStore;				// 상점 세율
	INT							m_iTaxHuntZone;					// 사냥터 입장료
	BOOL						m_bEnterHuntZone;				// 사냥터 입장 가능여부

	// 2 . 스케쥴 데이터
	vector <CS_SCHEDULE_DATA>	m_vtScheduleData;				// 공성전 스케쥴 상태 관리
	CRITICAL_SECTION			m_critScheduleData;

	// 3 . NPC 데이터
	vector <CS_NPC_DATA>		m_vtNpcData;					// 공성전 NPC데이터 관리
	CRITICAL_SECTION			m_critNpcData;
	BOOL						m_bDbNpcCreated;				// DB에서 읽어온 NPC들을 생성하였는가 ? (SetState() 중 최초 한번만 해준다.)

	// 4 . 공성 당사자 길드 데이터
	map <string, CS_TOTAL_GUILD_DATA>	m_mapCsBasicGuildInfo;	// 공성전 시 각 참여길드 (당사자) 의 공/수 구분 정보를 가짐
	CRITICAL_SECTION			m_critCsBasicGuildInfo;
	map <string, CS_TOTAL_GUILD_DATA>	m_mapCsTotalGuildInfo;	// 공성전 시 모든 참여길드 (연합포함) 의 공/수 구분 정보를 가짐
	CRITICAL_SECTION			m_critCsTotalGuildInfo;
	BOOL						m_bCsBasicGuildInfoLoadOK;		// 최종 공격측 당사자 길드의 데이터가 구축되었는지 여부
	BOOL						m_bCsTotalGuildInfoLoadOK;		// 공성전에 관계된 연합포함 모든 길드정보 데이터가 구축되었는지 여부

	// 5 . 미니맵 데이터
	map <INT, CS_MINIMAP_DATA>	m_mapMiniMapData;				// 미니맵 데이터를 모아놓는 맵
	CRITICAL_SECTION			m_critCsMiniMap;				// 미니맵 데이터 보호 (공/수를 함께 동시에 보호)
	vector <INT>				m_vtMiniMapReqUser;				// 미니맵 데이터를 요청하는 사용자들

	// 6 . 공성전 중 필요한 데이터
	BOOL						m_bCastleTowerAccessable;		// 공성측이 성의 2층으로 올라갈 수 있는가 ? (수호석상 완파 시)
	BOOL						m_bRegCrownAvailable;			// 왕관 직인등록이 가능한가 ? (보호막이 해제되었나 ?)
	INT							m_iCrownIndex;					// 왕관의 인덱스
	INT							m_iCastleCrownAccessUser;		// 왕관에 접근한 사용자
	BYTE						m_btCastleCrownAccessUserX;		// 왕관에 접근한 사용자의 X좌표
	BYTE						m_btCastleCrownAccessUserY;		// 왕관에 접근한 사용자의 Y좌표
	INT							m_iCastleSwitchAccessUser1;		// 왕관 스위치 1 에 접근한 사용자
	INT							m_iCastleSwitchAccessUser2;		// 왕관 스위치 2 에 접근한 사용자
	DWORD						m_dwCrownAccessTime;			// 왕관 접근 시간 (0 이 기본)
	CHAR						m_szMiddleWinnerGuild[16];		// 공성전 중간 승리 길드의 이름을 저장한다. -> 끝까지 남으면 승리길드가 된다.
};


extern CCastleSiege	g_CastleSiege;


#endif


#endif // !defined(AFX_CASTLESIEGE_H__6C5A97C8_FFFB_42E4_8F56_ABE80C670758__INCLUDED_)
