//----------------------------------------------------------------------------------
// FileName				: RaklionDefine.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 관련 define을 모아 놓은 파일
//----------------------------------------------------------------------------------

#pragma once

#define RAKLION_DEFAULT_REGEN_GATE		286		// 보스맵에서 사망하거나 리젠되는 경우
#define RAKLION_COOLUTIN				457		// 쿨러틴 인덱스
#define RAKLION_IRON_KNIGHT				458		// 아이언 나이트 인덱스
#define RAKLION_SELUPAN					459		// 세루판 인덱스
#define RAKLION_SELUPAN_EGG_1			460		// 세루판 알1 인덱스
#define RAKLION_SELUPAN_EGG_2			461		// 세루판 알2 인덱스
#define RAKLION_SELUPAN_EGG_3			462		// 세루판 알3 인덱스

#ifndef MAP_INDEX_RAKLION_BOSS
#define MAP_INDEX_RAKLION_FIELD			57		// MAP_INDEX_RAKLION_BOSS 가 디파인 되있지 않다면 디파인한다.
#define MAP_INDEX_RAKLION_BOSS			58		// MAP_INDEX_RAKLION_BOSS 가 디파인 되있지 않다면 디파인한다.
#endif											// 단지 버그 나는것을 피하기 위한 것이고 다른 의미는 없다.


///임시//////
#define RAKLION_SELUPAN_EGG_HALF		1		// 연출을 위한 거미알 개수(임시)
#define RAKLION_SELUPAN_EGG_MAX			15		// 거미알 총 개수(임시)
#define RAKLION_BOSS_APPEARANCE_DELAY	5		// 거미알이 모두 파괴된 후 보스가 나오기까지 딜레이(연출시간)(sec)
#define RAKLION_BOSSZONE_CLOSE_DELAY	10		// 보스전이 시작된 뒤 보스존이 닫히는 시간(sec)
#define RAKLION_BOSS_END_DELAY			10		// 보스전이 종료된 뒤 초기화되는 시간(sec)
#define RAKLION_BOSS_SKILL_DELAY		3000	// 세루판의 스킬 사용 딜레이
/////////////

enum RAKLION_STATE								// 라클리온 전체 상태 스케쥴
{
	RAKLION_STATE_IDLE					= 0,	// 라클리온 보스전 대기 상태(항상 대기 상태이다)
	RAKLION_STATE_NOTIFY_1				= 1,	// 일정량의 거미알이 깨졌다.(클라이언트에 메시지 보냄)
	RAKLION_STATE_STANDBY				= 2,	// 보스 출현전 연출을 위한 딜레이
	RAKLION_STATE_NOTIFY_2				= 3,	// 서버 메시지 출력(세루판이 등장하였습니다.)
	RAKLION_STATE_READY					= 4,	// 보스전을 준비 - 보스 생성 및 최초 스킬 발동
	RAKLION_STATE_START_BATTLE			= 5,	// 보스전 진행 상태 - 보스전 시작 후 5분간은 열린 상태이다
	RAKLION_STATE_NOTIFY_3				= 6,	// 서버 메시지 출력(라클리온 보스존이 닫혔습니다.)
	RAKLION_STATE_CLOSE_DOOR			= 7,	// 보스전 진행 상태 - 보스존 닫힌 상태로 진행
	RAKLION_STATE_ALL_USER_DIE			= 8,	// 보스존의 유저가 모두 죽은 상태
	RAKLION_STATE_NOTIFY_4				= 9,	// 서버 메시지 출력(보스전 종료) - 5분간 대기
	RAKLION_STATE_END					= 10,	// 전투 종료 - 보스존 다시 열림
	RAKLION_STATE_DETAIL_STATE			= 11,	// 세루판과의 보스전 세부 상태를 보낼때 쓰는 값
	RAKLION_STATE_MAX					= 12,
};

enum RAKLION_BATTLE_OF_SELUPAN_PATTERN				// 세루판 상태 스케쥴
{
	BATTLE_OF_SELUPAN_NONE					= 0,	// 세루판전 상태가 아님(보스전 시작 안함)
	BATTLE_OF_SELUPAN_STANDBY				= 1,	// 세루판전 준비 상태
	BATTLE_OF_SELUPAN_PATTERN_1				= 2,	// 세루판 패턴 1단계
	BATTLE_OF_SELUPAN_PATTERN_2				= 3,	// 세루판 패턴 2단계
	BATTLE_OF_SELUPAN_PATTERN_3				= 4,	// 세루판 패턴 3단계
	BATTLE_OF_SELUPAN_PATTERN_4				= 5,	// 세루판 패턴 4단계
	BATTLE_OF_SELUPAN_PATTERN_5				= 6,	// 세루판 패턴 5단계
	BATTLE_OF_SELUPAN_PATTERN_6				= 7,	// 세루판 패턴 6단계
	BATTLE_OF_SELUPAN_PATTERN_7				= 8,	// 세루판 패턴 7단계
	BATTLE_OF_SELUPAN_DIE					= 9,	// 세루판이 죽었다.
	BATTLE_OF_SELUPAN_MAX					= 10,	// 세루판전 최대 상태 수
};

enum RAKLION_BATTLE_SUCCESS_VALUE
{
	RAKLION_BATTLE_FAIL							= 0,	// 실패
	RAKLION_BATTLE_SUCCESS						= 1		// 성공
};

enum RAKLION_SELUPAN_SKILL
{
	RAKLION_SELUPAN_SKILL_FIRST_SKILL			= 0,	// 최초등장시 밀어내기 스킬
	RAKLION_SELUPAN_SKILL_POISON_ATTACK			= 1,	// 기본 공격(독)
	RAKLION_SELUPAN_SKILL_ICE_STORM				= 2,	// 아이스 스톰
	RAKLION_SELUPAN_SKILL_ICE_STRIKE			= 3,	// 아이스 스트라이크
	RAKLION_SELUPAN_SKILL_SUMMON_MONSTER		= 4,	// 몬스터 소환
	RAKLION_SELUPAN_SKILL_HEAL					= 5,	// 자기 자신 치료
	RAKLION_SELUPAN_SKILL_FREEZE				= 6,	// 얼리기
	RAKLION_SELUPAN_SKILL_TELEPORT_RANDOM		= 7,	// 랜덤 순간 이동
	RAKLION_SELUPAN_SKILL_INVINCIBILITY			= 8,	// 순간 무적
	RAKLION_SELUPAN_SKILL_MAX					= 9,
};


//---------------------------------------------------------------------------------------------
//
// 운영자용 명령어
//----------------------------------------------------------------------------------------------
enum RAKLION_GM_COMMAND
{
	RAKLION_GM_COMMAND_IDLE					= 0,		// "/라클리온대기"
	RAKLION_GM_COMMAND_NOTIFY_1				= 1,		// "/라클리온공지1"
	RAKLION_GM_COMMAND_STANDBY				= 2,		// "/라클리온스탠바이"
	RAKLION_GM_COMMAND_NOTIFY_2				= 3,		// "/라클리온공지2"
	RAKLION_GM_COMMAND_READY				= 4,		// "/라클리온준비"
	RAKLION_GM_COMMAND_START_BATTLE			= 5,		// "/라클리온시작"
	RAKLION_GM_COMMAND_NOTIFY_3				= 6,		// "/라클리온공지3"
	RAKLION_GM_COMMAND_CLOSE_DOOR			= 7,		// "/라클리온폐쇄"
	RAKLION_GM_COMMAND_ALL_USER_DIE			= 8,		// "/라클리온실패"
	RAKLION_GM_COMMAND_NOTIFY_4				= 9,		// "/라클리온공지4"
	RAKLION_GM_COMMAND_END					= 10,		// "/라클리온종료"

	RAKLION_GM_COMMAND_SELUPAN_1			= 11,		// "/세루판패턴1"
	RAKLION_GM_COMMAND_SELUPAN_2			= 12,		// "/세루판패턴2"
	RAKLION_GM_COMMAND_SELUPAN_3			= 13,		// "/세루판패턴3"
	RAKLION_GM_COMMAND_SELUPAN_4			= 14,		// "/세루판패턴4"
	RAKLION_GM_COMMAND_SELUPAN_5			= 15,		// "/세루판패턴5"
	RAKLION_GM_COMMAND_SELUPAN_6			= 16,		// "/세루판패턴6"
	RAKLION_GM_COMMAND_SELUPAN_7			= 17,		// "/세루판패턴7"

	RAKLION_GM_COMMAND_FIRST_SKILL			= 18,		// "/세루판최초스킬"
	RAKLION_GM_COMMAND_POISON_ATTACK		= 19,		// "/세루판독공격"
	RAKLION_GM_COMMAND_ICE_STORM			= 20,		// "/세루판냉기폭풍"
	RAKLION_GM_COMMAND_ICE_STRIKE			= 21,		// "/세루판냉기충격"
	RAKLION_GM_COMMAND_SUMMON_MONSTER		= 22,		// "/세루판소환"
	RAKLION_GM_COMMAND_HEAL					= 23,		// "/세루판힐"
	RAKLION_GM_COMMAND_FREEZE				= 24,		// "/세루판얼리기"
	RAKLION_GM_COMMAND_TELEPORT_RANDOM		= 25,		// "/세루판순간이동"
	RAKLION_GM_COMMAND_INVINCIBILITY		= 26,		// "/세루판무적화"
};
