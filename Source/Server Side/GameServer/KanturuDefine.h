//----------------------------------------------------------------------------------
//
//	KanturuDefine.h
//
//----------------------------------------------------------------------------------

#define MAX_KANTURU_BATTLE_USER			15

//---------------------------------------------------------------------------------------------
//
// 칸투르 맵
//----------------------------------------------------------------------------------------------
enum KANTURU_MAP_TYPE			
{
	KANTURU_MAP_TOWEROFREFINEMENT_CLOSE		= 0,	// 정제의 탑 닫힘
	KANTURU_MAP_TOWEROFREFINEMENT_OPEN		= 1,	// 정제의 탑 열림
	MAX_KANTURU_TOWEROFREFINEMENT_MAP		= 2
};

// 입장할 게이트 번호
#define KANTURU_MAYA_MAP_GATE			133		// NPC를 통해 입장가능한 마야전투 맵 게이트 정보
#define KANTURU_NIGHTMARE_MAP_GATE		134		// NPC를 통해 입장가능한 나이트메어전투 맵 게이트 정보
#define KANTURU_TOWER_OPEN_MAP_GATE		135		// NPC를 통해 입장할 때 정제의 탑에 입장 가능한 상태일 경우
#define KANTURU_KICK_OUT_FROM_TOWER		136		// 정제의 탑이 열린 상태에서 칸투르 유적지대로 강제로 이동
#define KANTURU_DEFAULT_REGEN_MAP_GATE	137		// 보스맵에서 사망하거나 리젠되는 경우

#ifdef ADD_KANTURU_CONDITION_20060725
// 칸투르 일반맵에서 사망시 이동 케이트 번호
#define KANTURU_DIEREGEN_GATE_MAP_1		138
#define KANTURU_DIEREGEN_GATE_MAP_2		139
#endif

//---------------------------------------------------------------------------------------------
//
// 데이터 파일 로드
//---------------------------------------------------------------------------------------------
enum KANTURU_DATAFILE_LOAD
{
	KANTURU_DATAFILE_LOAD_STATE						= 0,	// 칸투르 보스전 기본 진행 상태
	KANTURU_DATAFILE_LOAD_STANBY_STATE				= 1,	// 칸투르 대기상태 세부상태 정보
	KANTURU_DATAFILE_LOAD_MAYA_SCENE_STATE			= 2,	// 칸투르 마야전 연출별 상태 정보
	KANTURU_DATAFILE_LOAD_MAYA_STATE				= 3,	// 칸투르 마야전 세부상태 정보
	KANTURU_DATAFILE_LOAD_MAYA_HAND_AICHANGE_TIME	= 4,	// 칸투르 마야전 마야 손 AI 변경 시간
	KANTURU_DATAFILE_LOAD_MAYA_HAND_GROUP_NUMBER	= 5,	// 칸투르 마야전 마야손 그룹번호
	KANTURU_DATAFILE_LOAD_MAYA_ICESTORM_USING_RATE	= 6,	// 칸투르 마야전 마야 아이스스톰 스킬 사용 확률
	KANTURU_DATAFILE_LOAD_NIGHTMARE_STATE			= 7,	// 칸투르 나이트메어전 세부상태 정보
	KANTURU_DATAFILE_LOAD_NIGHTMARE_BATTLEMONSTER	= 8,	// 칸투르 나이트메어전 몬스터 전투 정보
	KANTURU_DATAFILE_LOAD_TOWEROFREFINEMENT_STATE	= 9,	// 칸투르 정제의탑 세부상태 정보
	KANTURU_DATAFILE_LOAD_CHECK_MOONSTONE			= 10	// 칸투르 전투맵 입장시 문스톤펜던트 착용검사 여부
};

enum KANTURU_SET_MONSTER
{
	KANTURU_SET_MONSTER_MAYAMONSTER1				= 0,	// 칸투르 마야전 몬스터와의 전투 첫 번째, 두 번째 몬스터 리젠
	KANTURU_SET_MONSTER_MAYAMONSTER3				= 1,	// 칸투르 마야전 몬스터와의 전투 세 번째
	KANTURU_SET_MONSTER_NIGHTMAREMONSTER			= 2,	// 칸투르 나이트메어전 몬스터
	KANTURU_SET_MONSTER_MAYA_LEFTHAND				= 3,	// 칸투르 마야 왼손
	KANTURU_SET_MONSTER_MAYA_RIGHTHAND				= 4,	// 칸투르 마야 오른손
	KANTURU_SET_MONSTER_MAYA_HANDS					= 5,	// 칸투르 마야 양손	
	KANTURU_SET_MONSTER_REGENMONSTER				= 6,	// 칸투르 나이트메어전 리젠되는 몬스터
};


//---------------------------------------------------------------------------------------------
//
// 보스전 몬스터
//----------------------------------------------------------------------------------------------
#define KANTURU_MONSTER_CANNONTRAP		105
#define KANTURU_MONSTER_LASERTRAP		106

#define KANTURU_MONSTER_BLADEHUNTER		354
#define KANTURU_MONSTER_GENOCIDER		357
#define KANTURU_MONSTER_PERSONA			358
#define KANTURU_MONSTER_TWINTAILE		359
#define KANTURU_MONSTER_DREADPEAR		360

#define KANTURU_MONSTER_MAYA_LEFTHAND	362
#define KANTURU_MONSTER_MAYA_RIGHTHAND	363
#define KANTURU_MONSTER_MAYA			364

#define KANTURU_MONSTER_NIGHTMARE		361

//---------------------------------------------------------------------------------------------
//
// 상태 관련
//----------------------------------------------------------------------------------------------
enum KANTURU_BATTLE_OF_MAYA_BATTLE_STATE			// 마야전 각 전투 상태
{
	BATTLE_OF_MAYA_BATTLE_NONE			= -1,	// 마야전 전투상태 아님
	BATTLE_OF_MAYA_BATTLE_FIRST			= 0,	// 1차 전투
	BATTLE_OF_MAYA_BATTLE_SECOND		= 1,	// 2차 전투
	BATTLE_OF_MAYA_BATTLE_THIRD			= 2,	// 3차 전투
	MAX_BATTLE_STATE_BATTLE_OF_MAYA		= 3		// 마야전 최대 전투 상태
};

enum KANTURU_BATTLE_SUCCESS_LOG						// 칸투르 전투 성공에 대한 로그를 남기기 위한 인덱스
{
	KANTURU_BATTLE_SUCCESS_LOG_MAYA1		= 0,	// 마야 왼손과의 전투 성공
	KANTURU_BATTLE_SUCCESS_LOG_MAYA2		= 1,	// 마야 오른손과의 전투 성공
	KANTURU_BATTLE_SUCCESS_LOG_MAYA3		= 2,	// 마야 양손과의 전투 성공
	KANTURU_BATTLE_SUCCESS_LOG_NIGHTMARE	= 3		// 나이트에머와의 전투 성공
};


//---------------------------------------------------------------------------------------------
//
// 스케쥴러 관련
//----------------------------------------------------------------------------------------------
enum KANTURU_STATE								// 칸투르 전체 상태 스케쥴
{
	KANTURU_STATE_NONE						= 0,	// 칸투르 진행 스케쥴 없음	
	KANTURU_STATE_BATTLE_STANDBY			= 1,	// 전투 대기 상태	
	KANTURU_STATE_BATTLE_OF_MAYA			= 2,	// 1차 보스전(마야)	
	KANTURU_STATE_BATTLE_OF_NIGHTMARE		= 3,	// 2차 보스전(나이트메어)	
	KANTURU_STATE_TOWER_OF_REFINEMENT		= 4,	// 정제의 탑	
	KANTURU_STATE_END						= 5,	// 칸투르 스케쥴 종료
	MAX_KANTURU_STATE						= 6
};

enum KANTURU_BATTLE_STANBY_STATE				// 칸투르 전투 대기상태 스케쥴
{
	KANTURU_BATTLE_STANBY_STATE_NONE		= 0,	// 전투 대기상태 아님
	KANTURU_BATTLE_STANBY_STATE_START		= 1,	// 전투 대기상태
	KANTURU_BATTLE_STANBY_STATE_NOTIFY		= 2,	// 서버 메세지 출력
	KANTURU_BATTLE_STANBY_STATE_END			= 3,	// 종료
	MAX_KANTURU_BATTLE_STANBY_STATE			= 4		// 전투 대기상태 최대 수
};

enum KANTURU_BATTLE_OF_MAYA_STATE				// 1차 보스전(마야) 상태 스케쥴
{
	BATTLE_OF_MAYA_STATE_NONE				= 0,	// 1차 보스전 상태가 아님
	BATTLE_OF_MAYA_STATE_STANBY1			= 1,	// 대기상태	
	BATTLE_OF_MAYA_STATE_NOTIFY1			= 2,	// 강제연출 1
	BATTLE_OF_MAYA_STATE_START1				= 3,	// 첫 번째 몬스터와의 전투
	BATTLE_OF_MAYA_STATE_MAYA1				= 4,	// 첫 번째 마야 손과의 전투 
	BATTLE_OF_MAYA_STATE_END_MAYA1			= 5,	// 첫 번째 전투 상태 종료
	BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA1		= 6,	// 첫 번째 전투 상태 종료에 대한 처리
	BATTLE_OF_MAYA_STATE_STANBY2			= 7,	// 대기상태

	BATTLE_OF_MAYA_STATE_START2				= 8,	// 두 번째 몬스터와의 전투
	BATTLE_OF_MAYA_STATE_MAYA2				= 9,	// 두 번째 마야 손과의 전투
	BATTLE_OF_MAYA_STATE_END_MAYA2			= 10,	// 두 번째 전투 상태 종료
	BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA2		= 11,	// 두 번째 전투 상태 종료에 대한 처리
	BATTLE_OF_MAYA_STATE_STANBY3			= 12,	// 대기상태

	BATTLE_OF_MAYA_STATE_START3				= 13,	// 세 번째 몬스터와의 전투
	BATTLE_OF_MAYA_STATE_MAYA3				= 14,	// 세 번째 마야 손과의 전투
	BATTLE_OF_MAYA_STATE_END_MAYA3			= 15,	// 세 번째 전투 상태 종료
	BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA3		= 16,	// 세 번째 전투 상태 종료에 대한 처리
	BATTLE_OF_MAYA_STATE_END				= 17,	// 1차 보스전 종료
	BATTLE_OF_MAYA_STATE_ENDCYCLE			= 18,	// 1차 보스전 종료에 대한 처리
	MAX_BATTLE_OF_MAYA_STATE						// 1차 보스전 최대 상태 수
};

enum KANTURU_BATTLE_OF_NIGHTMARE_STATE			// 2차 보스전(나이트메어) 상태 스케쥴
{
	BATTLE_OF_NIGHTMARE_STATE_NONE			= 0,	// 2차 보스전 상태 아님
	BATTLE_OF_NIGHTMARE_STATE_IDLE			= 1,	// 클라이언트 대기상태		
	BATTLE_OF_NIGHTMARE_STATE_NOTIFY		= 2,	// 나이트메어 등장 강제 연출
	BATTLE_OF_NIGHTMARE_STATE_START			= 3,	// 전투 시작	
	BATTLE_OF_NIGHTMARE_STATE_END			= 4,	// 2차 보스전 종료	
	BATTLE_OF_NIGHTMARE_STATE_ENDCYCLE		= 5,	// 종료 처리
	MAX_BATTLE_OF_NIGHTMARE_STATE					// 2차 보스전 나이트메어 상태 최대 수
};

enum KANTURU_TOWER_OF_REFINEMENT_STATE			// 정제의 탑 상태 스케쥴
{
	TOWER_OF_REFINEMENT_STATE_NONE				= 0,	// 정제의 탑 활성화 상태가 아님
	TOWER_OF_REFINEMENT_STATE_REVITALIZATION	= 1,	// 정제의 탑 활성화
	TOWER_OF_REFINEMENT_STATE_NOTIFY1			= 2,	// 서버 메세지 출력
	TOWER_OF_REFINEMENT_STATE_CLOSE				= 3,	// 정제의 탑 입장 불가
	TOWER_OF_REFINEMENT_STATE_NOTIFY2			= 4,	// 서버 메세지 출력
	TOWER_OF_REFINEMENT_STATE_END				= 5,	// 정제의 탑 사용 종료
	MAX_TOWER_OF_REFINEMENT_STATE						// 정제의 탑 상태 최대 수
};



//---------------------------------------------------------------------------------------------
//
// 종료조건
//----------------------------------------------------------------------------------------------
enum KANRUTU_STATE_END_COMDITION
{
	KANRUTU_STATE_END_COMDITION_NONE			= 0,	// 아무런 종료조건 없음
	KANRUTU_STATE_END_COMDITION_TIMELIMIT		= 1,		// 시간제한
};



//---------------------------------------------------------------------------------------------
//
// 성공/ 실패
//----------------------------------------------------------------------------------------------
enum KANTURU_BATTLE_SUCCESS_VALUE
{
	KANTURU_BATTLE_FAIL							= 0,	// 실패
	KANTURU_BATTLE_SUCCESS						= 1		// 성공
};

enum KANTURU_MAYA_SKILL
{
	KANTURU_MAYA_SKILL_ICE_STORM				= 0,	// 아이스 스톰
	KANTURU_MAYA_SKILL_BROKEN_SHOWER			= 1		// 브로큰 샤워
};


#ifdef KANTURU_GM_COMMAND_20060709
//---------------------------------------------------------------------------------------------
//
// 운영자용 명령어
//----------------------------------------------------------------------------------------------
enum KANTURU_GM_COMMAND
{
	KANTURU_GM_COMMAND_STANBY				= 0,	// "/칸투르대기상태"
	KANTURU_GM_COMMAND_BATTLEOFMAYA			= 1,		// "/칸투르마야전"
	
	KANTURU_GM_COMMAND_MAYA_MONSTER1		= 2,		// "/마야몬스터1"
	KANTURU_GM_COMMAND_MAYA1				= 3,		// "/마야1"
	KANTURU_GM_COMMAND_MAYA_MONSTER2		= 4,		// "/몬스터2"
	KANTURU_GM_COMMAND_MAYA2				= 5,		// "/마야2"
	KANTURU_GM_COMMAND_MAYA_MONSTER3		= 6,		// "/마야몬스터3"
	KANTURU_GM_COMMAND_MAYA3				= 7,		// "/마야3"
	KANTURU_GM_COMMAND_MAYA3_SUCCESS		= 8,		// "/마야3성공"	

	KANTURU_GM_COMMAND_MAYA_SUCCESS			= 9,		// "/마야성공"
	KANTURU_GM_COMMAND_MAYA_FAIL			= 10,		// "/마야실패"

	KANTURU_GM_COMMAND_NIGHTMARE			= 11,		// "/칸투르나이트메어전"
	KANTURU_GM_COMMAND_NIGHTMARE_SUCCESS	= 12,		// "/나이트메어성공"
	KANTURU_GM_COMMAND_NIGHTMARE_FAIL		= 13,		// "/나이트메어실패"

	KANTURU_GM_COMMAND_TOWEROFREFINEMENT	= 14,		// "/칸투르정제의탑"
	KANTURU_GM_COMMAND_TOWER_OPEN			= 15,		// "/정제의탑열기"
	KANTURU_GM_COMMAND_TOWER_CLOSE			= 16,		// "/정제의탑닫기"
	KANTURU_GM_COMMAND_END					= 17,		// "/칸투르종료"

	KANTURU_GM_COMMAND_NEXT_STATE			= 18,		// "칸투르다음상태"
	KANTURU_GM_COMMAND_NEXT_DETAIL_STATE	= 19,		// "칸투르다음세부상태"
	KANTURU_GM_COMMAND_CURRENT_STATE		= 20,		// "칸투르현재상태"	
};

#endif