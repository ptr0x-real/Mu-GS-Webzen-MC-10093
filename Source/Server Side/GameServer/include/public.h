//----------------------------------------------------------------------------
// 서버와 클라이언트가 공유하는 헤더 정의
//----------------------------------------------------------------------------
#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#ifndef GAME_VERSION
#include "define.h"
#endif

//----------------------------------------------------------------------------
// 캐릭터 번호
enum ClassNumber
{
	CLASS_WIZARD,
	CLASS_KNIGHT,
	CLASS_ELF,
	CLASS_MAGUMSA,	
	CLASS_DARKLORD
//#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	,CLASS_SUMMONER
//#endif
};


//#define MAX_MAGIC					20	// 사용자당 최대 마법 개수
//#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116		// 공성전 패치 때 마법개수 늘림
#define MAX_MAGIC					60	// 사용자당 최대 마법 개수
//#endif

#define MAX_TOTALMAGIC				100	// 마법최대 개수

#define DEF_DARKLORD_LEVEL			1	// 다크로드 캐릭터 생성시 기본 레벨
//----------------------------------------------------------------------------
// 저항력 번호
// -1: 없음, 0: 얼음, 1: 독, 2:번개, 3:불, 4:땅, 5:바람, 6:물
#define RESISTANCE_COLD				0
#define RESISTANCE_POISON			1
#define RESISTANCE_THUNDER			2
#define RESISTANCE_FIRE				3

#define RESISTANCE_EARTH			4
#define RESISTANCE_WIND				5
#define RESISTANCE_WATER			6

//#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// MAX_SKILL 수정
#define MAX_SKILL					600
//#endif


// View 상태
#define STATE_NONE					0			//  보통.
#define STATE_POISON				0x00000001  //  독 걸린상태.
#define STATE_FREEZE				0x00000002  //  얼음 걸린상태.
#define STATE_ATTACK				0x00000004  //  공격력 증가.
#define STATE_DEFENSE				0x00000008  //  방어력 증가.
#define STATE_ADD_LIFE				0x00000010	//  생명력 증가.
#define STATE_HARDEN				0x00000020	//  석화.
#define STATE_REDUCE_DEFENSE		0x00000040	//  방어감소(마검사)	(몬스터 스킬도 같이 사용)
#define STATE_REDUCE_ATTACKDAMAGE	0x00000080	//  공격감소(법사)


#define STATE_REDUCE_MAGICDEFENSE	0x00000100	//  방어력증가(법사)
#define STATE_REDUCE_MAGICPOWER		0x00000200	//  마법 능력 감소 (몬스터 스킬)
#define STATE_ADD_AG				0x00000400	//  AG증가 속도 향상
#define STATE_ADD_CRITICAL_DAMAGE	0x00000800	//  크리티컬 데미지 증가

//#ifdef ADD_AUTHORITY_CODE_OF_EVENT_GM_20050602
#define STATE_ADD_GM_MARK	        0x01000000	//  GM 마크 스킨 추가
//#endif

#define STATE_IMMUNE_TO_MAGIC		0x00000080	// 마법방어 무적(몹한테만.)	(STATE_REDUCE_ATTACKDAMAGE = 법사, 소울바리어)
#define STATE_IMMUNE_TO_HARM		0x00000008	// 물리방어 무적(몹한테만.)	(STATE_DEFENSE = 요정, 방어력향상)


//#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
#define STATE_BRAND_OF_SKILL		0x00000200	// 다크로드가 보장하는 기술 브랜드 ^^
#define STATE_STUNED				0x00001000	// 스턴 걸린 상태.
#define STATE_ADD_MANA				0x00002000	// 마나 증가.
#define STATE_CLOAKING				0x00004000	// 투명 상태.
#define STATE_BLESS_POTION			0x00008000	// 축복의 물약.
#define STATE_SOUL_POTION			0x00010000	// 영혼의 물약.
#define STATE_REMOVAL_MAGIC			0x00100000	// 마법 해제.
//#endif

//#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
#define STATE_IMMUNE_TO_SKILL		0x00200000	// 특정 스킬에 IMMUNE 상태
//#endif

//#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
#define STATE_INFINITY_ARROW		0x04000000	// 인피니티 애로우(뮤즈엘프) 스킬
//#endif

//#ifdef CASTLE_NPC_GATE_WORK_20041206
#define STATE_CASTLE_GATE			0x00020000	// 성문의 여닫힌 상태
//#endif
//#ifdef CASTLE_NPC_CROWN_WORK_20041210
#define STATE_CASTLE_CROWN			0x00020000	// 왕관의 등록 중 상태
//#endif

//#ifdef MU_CASTLESIEGE_JOINSIDE_VIEWPORT_20041209
#define STATE_JOIN_SIDE_DEFEND		0x00040000	// 공성전 시 공/수 구분
#define STATE_JOIN_SIDE_ATTACK		0x00080000	// 공성전 시 공/수 구분 - 공성 01
//#endif

//#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
#define STATE_JOIN_SIDE_ATTACK_2	0x00400000	// 공성전 시 공/수 구분 - 공성 02
#define STATE_JOIN_SIDE_ATTACK_3	0x00800000	// 공성전 시 공/수 구분 - 공성 03
//#endif


//#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
#define STATE_NPC_HELP              0x02000000  // 쉐도우 팬텀 공격력 향상 & 방어력 향상 스킬 상태
//#endif

//#ifdef CRYWOLF_NPC_WORK_20050912
#define STATE_BESERKER					0x00800000	// BERSERKER 상태

#define STATE_CRYWOLF_NPC_HIDE			0x10000000	// 크라이울프 ONLY - NPC HIDE 상태

#define STATE_CRYWOLF_STATUE_SHIELD1	0x00000001	// 늑대의 신상 ONLY - 1단계 방어막 생성 상태
#define STATE_CRYWOLF_STATUE_SHIELD2	0x00000002	// 늑대의 신상 ONLY - 2단계 방어막 생성 상태
#define STATE_CRYWOLF_STATUE_SHIELD3	0x00000004	// 늑대의 신상 ONLY - 3단계 방어막 생성 상태
#define STATE_CRYWOLF_STATUE_SHIELD4	0x00000008	// 늑대의 신상 ONLY - 4단계 방어막 생성 상태
#define STATE_CRYWOLF_STATUE_SHIELD5	0x00000100	// 늑대의 신상 ONLY - 5단계 방어막 생성 상태

#define STATE_CRYWOLF_ALTAR_ENABLE		0x00010000	// 늑대의 제단 ONLY - 계약 가능 상태
#define STATE_CRYWOLF_ALTAR_DISABLE		0x00020000	// 늑대의 제단 ONLY - 계약 불능 상태
#define STATE_CRYWOLF_ALTAR_CONTRACTED	0x00040000	// 늑대의 제단 ONLY - 계약 되어 있는 상태
#define STATE_CRYWOLF_ALTAR_ATTEMPT		0x00080000	// 늑대의 제단 ONLY - 계약 시도 상태
#define STATE_CRYWOLF_ALTAR_OCCUFIED	0x00100000	// 늑대의 제단 ONLY - 점령 당한 상태
//#endif

//#ifdef PCBANG_POINT_SYSTEM_20070206		// 인장 버퍼
#define STATE_SEAL_ITEM					0x08000000	// PC방 포인트 인장 아이템 
//#endif

//#ifdef ADD_PCS_MARK_OF_EXP_20070205
#define STATE_MARK_USE					0x08000000
//#endif // ADD_PCS_MARK_OF_EXP_20070205

//#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영 사원 전용 스킬
#define STATE_ILLUSION_TEMPLE_PRODECTION		0x00040000	// 보호의 주문
#define STATE_ILLUSION_TEMPLE_RESTRAINT			0x00080000	// 속박의 주문
//#endif

//#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
#define DEL_VIEWSKILLSTATE( x, y )		( x &= ~ y )
#define ADD_VIEWSKILLSTATE( x, y )		( x |= y   )
//#endif

//ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
enum CHARACTER_AUTO_RECUPERATION_TIME
{
	CART_10_SEC         = 1000 * 10,
	CART_15_SEC         = 1000 * 15,
	CART_25_SEC         = 1000 * 25,
};
enum HP_MP_AG_VALUE_PER_TIME
{
	VPT_HP_MP_AG_1_PER_1_SEC   = 1,
	VPT_HP_MP_AG_5_PER_1_SEC   = 5,
	VPT_HP_MP_AG_10_PER_1_SEC   = 10
};
//#endif



//#ifdef DARKLORD_WORK
enum eSkillType
{
	//마법사
	AT_SKILL_POISON		= 1,		// 이곳부터 16개는 마법책과 대응한다.(수정할수 없다)
	AT_SKILL_METEO		= 2,
	AT_SKILL_THUNDER	= 3,
	AT_SKILL_FIREBALL	= 4,
	AT_SKILL_FLAME		= 5,
	AT_SKILL_TELEPORT   = 6,
	AT_SKILL_SLOW		= 7,
	AT_SKILL_STORM		= 8,
	AT_SKILL_EVIL		= 9,
	AT_SKILL_HELL		= 10,
	AT_SKILL_POWERWAVE  = 11,
	AT_SKILL_FLASH      = 12,
	AT_SKILL_BLAST      = 13,
	AT_SKILL_INFERNO    = 14,

	AT_SKILL_TARGET_TELEPORT		= 15,	// 소환 이동
	AT_SKILL_MAGICDEFENSE= 16,				// 소울바리어

	AT_SKILL_ENERGYBALL = 17,	
	
	//기사
	AT_SKILL_BLOCKING   = 18,//방어
	AT_SKILL_SWORD1     = 19,//내려찍기
	AT_SKILL_SWORD2     = 20,//찌르기
	AT_SKILL_SWORD3     = 21,//올려치기
	AT_SKILL_SWORD4     = 22,//돌려치기
	AT_SKILL_SWORD5     = 23,//베기

	//요정
	AT_SKILL_CROSSBOW   = 24,//활스킬     (다발)

	AT_SKILL_BOW		= 25,//관통(?)

	AT_SKILL_HEALING    = 26,//치료       (에너지/5+(아이템레벨+1)*5)                   //이곳부터는 마법구술과 대응한다.
	AT_SKILL_DEFENSE    = 27,//방어력증가 (약30초간 에너지/8+(아아템레벨+1)*2만큼 증가)
	AT_SKILL_ATTACK     = 28,//공격력증가 (약30초간 에너지/7+(아아템레벨+1)*3만큼 증가)
	

	AT_SKILL_CALLMON1   = 30,//소환       (아아템레벨에 따라 몹소환)
	AT_SKILL_CALLMON2   = 31,//소환       (아아템레벨에 따라 몹소환)
	AT_SKILL_CALLMON3   = 32,//소환       (아아템레벨에 따라 몹소환)
	AT_SKILL_CALLMON4   = 33,//소환       (아아템레벨에 따라 몹소환)
	AT_SKILL_CALLMON5   = 34,//소환       (아아템레벨에 따라 몹소환)
	AT_SKILL_CALLMON6   = 35,//발리       (아아템레벨에 따라 몹소환)
	AT_SKILL_CALLMON7   = 36,//솔져       (아아템레벨에 따라 몹소환)

	AT_SKILL_WHEEL			= 41,//회오리베기(기사전용)
	AT_SKILL_BLOWOFFURY		= 42,//검올려찍기(분노의일격)(기사전용)
	AT_SKILL_STRIKE			= 43,//블로우(기사_블레이드나이트전용)
	AT_SKILL_KNIGHTSPEAR	= 47,//창찌르기(유니를 타고 있어야 된다)(기사전용)
	AT_SKILL_KNIGHTADDLIFE	= 48,//스웰라이프(기사전용)	
	AT_SKILL_KNIGHTDINORANT	= 49,//레이드 슛:디노란트 스킬(기사전용)

	AT_SKILL_ELFHARDEN		= 51,	//아이스애로우(요정)
	AT_SKILL_PENETRATION	= 52,	//관통(요정)

	AT_SKILL_DEFENSEDOWN	= 55,	//블러드어택(마검사)

	AT_SKILL_SWORD6			= 56,	//테스트 스킬 -> 데스브로드 


	//AT_SKILL_4CROSSBOW		= 53,	//요정 4다발
	AT_SKILL_5CROSSBOW		= 54,	//요정 5다발

	AT_SKILL_EXPPOISON		= 38,	//확장독
	AT_SKILL_EXPICE			= 39,	//확장아이스
	
	AT_SKILL_EXPHELL		= 40,	//확장헬파이어 적용
	AT_SKILL_EXPHELL_START	= 58,	//확장헬파이어 시작
	
	AT_SKILL_IMPROVE_AG_REFILL	= 53,	//민요정 스킬 (AG증가 속도 향상)

	//몬스터
	AT_SKILL_DEVILFIRE		= 50,
	
	AT_SKILL_COMBO			= 59,	// 콤보 스킬

	AT_SKILL_SPEAR	= 60,			// 스피어
	AT_SKILL_FIREBURST,				// 파이어버스트
	AT_SKILL_DARKHORSE_ATTACK,		// 다크호스 어택
	AT_SKILL_RECALL_PARTY,			// 파티소환
	AT_SKILL_ADD_CRITICALDAMAGE,	// 크리티컬 데미지 증가
	AT_SKILL_ELECTRICSPARK,			// 일렉트릭 스파크

	AT_SKILL_LONGSPEAR,				// 롱스피어


//#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	AT_SKILL_RUSH				= 44,	// 블레이드 나이트
	AT_SKILL_JAVALIN			= 45,	// 소울 마스터
	AT_SKILL_DEEPIMPACT			= 46,	// 뮤즈 엘프
	AT_SKILL_ONE_FLASH			= 57,	// 기사형 마검사
	AT_SKILL_DEATH_CANNON		= 73,	// 법사형 마검사
	AT_SKILL_SPACE_SPLIT		= 74,	// 다크로드 공격 스킬
	
	AT_SKILL_BRAND_OF_SKILL		= 75,	// 다크로드 보조 스킬
	AT_SKILL_STUN				= 67,	// 스턴
	AT_SKILL_REMOVAL_STUN		= 68,	// 스턴 해제
	AT_SKILL_ADD_MANA			= 69,	// 스웰 마나
	AT_SKILL_INVISIBLE			= 70,	// 투명
	AT_SKILL_REMOVAL_INVISIBLE	= 71,	// 투명 해제
	AT_SKILL_REMOVAL_MAGIC		= 72,	// 마법 해제
//endif	

//#ifdef ADD_ITEM_FENRIR_01_20051110
	// 펜릴 스킬
	AT_SKILL_FENRIR_ATTACK		= 76,	// 플라즈마스톰

//#endif
	
//#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	AT_SKILL_INFINITY_ARROW	= 77,	// 인피니티 애로우 (뮤즈엘프)
//#endif

//#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// 파이어 스크림 인덱스 선언
	AT_SKILL_FIRESCREAM			= 78,	// 파이어 스크림 (다크로드)
	AT_SKILL_EXPLOSION			= 79,	// 폭발( 다크로드 - 파이어스크림에 대한 폭발 데미지 처리에 쓰인다.)
//#endif

//#ifdef ADD_NEW_MAP_KALIMA_20040518	// 칼리마에 추가된 몬스터 스킬들
	AT_SKILL_SUMMON				= 200,	// 몬스터 부하 소환
	AT_SKILL_IMMUNE_TO_MAGIC	= 201,	// 마법방어 무적
	AT_SKILL_IMMUNE_TO_HARM		= 202,	// 물리방어 무적
//#endif

//#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	//  환영 사원 전용스킬
	AT_SKILL_PRODECTION			= 210,	// 보호의 주문
	AT_SKILL_RESTRAINT			= 211,	// 속박의 주문
	AT_SKILL_CALL				= 212,	// 소환의 주문
	AT_SKILL_SHIELD_BURN		= 213,	// 쉴드 번
//#endif

//#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 신규 캐릭터 스킬
	// 저주
	AT_SKILL_DRAINLIFE			= 214,	// 드레인라이프
	AT_SKILL_CHAINLIGHTNING		= 215,	// 체인 라이트닝
	AT_SKILL_LIGHTNINGORB		= 216,	// 라이트닝오브

	AT_SKILL_THORNS				= 217,	// 쏜즈(데미지반사)
	
//#ifdef SEASON4_PATCH_GROOVING
	// --->
	// 시즌 4.0 추가 스킬  grooving 2008/04/24	
	AT_SKILL_BERSERKER			= 218,	// 버서커
	// <---
//#endif
	
	AT_SKILL_SLEEP				= 219,	// 슬립
	AT_SKILL_BLIND				= 220,	// 블라인드

//#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111	// 신규스킬 위크니스 이너베이션 정의
	AT_SKILL_WEAKNESS			= 221,	// 위크니스
	AT_SKILL_ENERVATION			= 222,	// 이너베이션
//#endif

	// 환수 스킬
	AT_SKILL_EXPLOSION_2		= 223,	// 익스플로젼
	AT_SKILL_REQUIEM			= 224,	// 레퀴엠
//#endif

//#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425	// 환수 신규스킬 펄루션
	AT_SKILL_PULLUTION			= 225,	// 펄루션
//#endif

//#ifdef SEASON4_PATCH_GROOVING
	// --->
	// 시즌 4.0 추가 스킬  grooving 2008/04/24
	AT_SKILL_LIGHTING_SHOCK		= 230,	// 라이트닝 쇼크 
	//<-----
//#endif

//#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 시즌4 신규 스킬
	AT_SKILL_DESTRUCTION		= 232,	// 파괴의 일격
	AT_SKILL_MAGICPOWRER		= 233,	// 마력증대
	AT_SKILL_RECOVERY			= 234,	// 회복
	AT_SKILL_MULTISHOT			= 235,	// 멀티샷
	AT_SKILL_FLAMESTRIKE		= 236,	// 플레임 스트라이크
	AT_SKILL_GIGANTICSTORM		= 237,	// 기간틱 스톰
	AT_SKILL_CHAOTICDESAIR		= 238,	// 카오틱 디세이어
//#endif

//#ifdef ADD_RAKLION_20080408		// 라클리온 보스 세루판 스킬
AT_SKILL_SELUPAN_POISON_ATTACK	= 250,	// 라클리온 보스 세루판 독공격
AT_SKILL_SELUPAN_ICE_STORM		= 251,	// 라클리온 보스 세루판 냉기 폭풍
AT_SKILL_SELUPAN_ICE_STRIKE		= 252,	// 라클리온 보스 세루판 냉기 충격
AT_SKILL_SELUPAN_FIRST_SKILL	= 253,	// 라클리온 보스 세루판 낙하(최초 스킬)
//#endif // ADD_RAKLION_20080408

//#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122		// 마스터레벨 스킬 1차 - 패시브 스킬 번호 선언
	AT_SKILL_ML_ATTACK_SUCCESS_RATE		= 300,	// 공격성공률 상승
	AT_SKILL_ML_PVP_ATTACKDAMAGE_RATE	= 305,	// 대인공격률 상승
	AT_SKILL_ML_PVP_BLOCKING_RATE		= 310,	// 대인방어율 증가
	AT_SKILL_ML_DOWN_DUR1				= 315,	// 내구 감소1
	AT_SKILL_ML_DOWN_DUR2				= 320,	// 내구 감소2
	AT_SKILL_ML_RESISTANCE_POISON		= 325,	// 독 저항력 상승
	AT_SKILL_ML_RESISTANCE_THUNDER		= 330,	// 번개 저항력 상승
	AT_SKILL_ML_RESISTANCE_ICE			= 335,	// 얼음 저항력 상승
	AT_SKILL_ML_AUTO_RECOVER_LIFE		= 340,	// 생명력 자동회복 강화
	AT_SKILL_ML_PLUS_ZEN				= 345,	// 젠 증가
//#endif

//#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	// 마스터레벨 스킬 2차 - 스킬 추가
	AT_SKILL_ML_DEFENCE					= 350,	// 방어력 증가
	AT_SKILL_ML_MAX_HP					= 355,	// 최대HP 증가
	AT_SKILL_ML_MAX_AG					= 360,	// 최대AG 증가
	AT_SKILL_ML_MONSTER_KILL_MANA		= 365,	// 몬스터 킬 시 마나회복
	AT_SKILL_ML_MONSTER_KILL_HP			= 370,	// 몬스터 킬 시 생명력회복
	AT_SKILL_ML_MONSTER_KILL_SD			= 375,	// 몬스터 킬 시 SD회복
	AT_SKILL_ML_PLUS_EXP				= 380,	// 경험치 상승
//#endif

//#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 마스터레벨 스킬 4랭크 패시브스킬
	AT_SKILL_ML_MAX_SD					= 385,	// 캐릭터 최대 SD증가
	AT_SKILL_ML_SD_SPEED				= 390,	// SD 회복 속도 상승
	AT_SKILL_ML_MAX_ATTACK_DAMAGE		= 395,	// 최대 공격력 상승( 흑기사, 요정, 다크로드)
	AT_SKILL_ML_MIN_ATTACK_DAMAGE		= 400,	// 최소 공격력 상승( 흑기사, 요정, 다크로드)
	AT_SKILL_ML_DECREASE_MANA			= 405,	// 마나 감소 강화
	AT_SKILL_ML_MAX_MAGIC_DAMAGE		= 410,	// 최대 마력 상승(흑마법사)
	AT_SKILL_ML_MIN_MAGIC_DAMAGE		= 415,	// 최소 마력 상승(흑마법사)
	AT_SKILL_ML_PET_DUR_DOWN_SPEED		= 420,	// 펫 내구 소모 속도 감소
	AT_SKILL_ML_MAX_ATTACK_MAGIC_DAMAGE	= 425,	// 최대 공격력/마력 상승(마검사)
	AT_SKILL_ML_MIN_ATTACK_MAGIC_DAMAGE	= 430,	// 최소 공격력/마력 상승(마검사)
//#endif

//#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 액티브 스킬 번호 추가
	AT_SKILL_ML_STRENGTHEN_MAGICDEFENSE			= 435,	// 소울바리어 강화
	AT_SKILL_ML_STRENGTHEN_HELL					= 440,	// 헬파이어 강화
	AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT			= 455,	// 회오리베기 강화1 (기사)
	AT_SKILL_ML_STRENGTHEN_HEALING				= 475,	// 치료 강화
	AT_SKILL_ML_STRENGTHEN_CROSSBOW				= 490,	// 다발 강화
	AT_SKILL_ML_STRENGTHEN_WHEEL_MAGUMSA		= 495,	// 회오리베기 강화2 (마검사)
	AT_SKILL_ML_STRENGTHEN_KNIGHTADDLIFE		= 470,	// 스웰라이프 강화	
	AT_SKILL_ML_STRENGTHEN_BLAST				= 510,	// 블래스트 강화
	AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK		= 515,	// 어스쉐이크 강화
	AT_SKILL_ML_STRENGTHEN_FIREBURST			= 520,	// 파이어버스트 강화
//#endif

//#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 마스터레벨 스킬 4랭크 액티브스킬
	AT_SKILL_ML_STRENGTHEN_EVIL					= 445,	// 악령 강화
	AT_SKILL_ML_STRENGTHEN_EXPICE				= 450,	// 서든 아이스 강화
	AT_SKILL_ML_STRENGTHEN_STRIKE				= 460,	// 블로우 강화(기사)
	AT_SKILL_ML_STRENGTHEN_BLOWOFFURY			= 465,	// 분노의 일격 강화(기사)
	AT_SKILL_ML_STRENGTHEN_DEFENSE				= 480,	// 방어력 향상
	AT_SKILL_ML_STRENGTHEN_ATTACK				= 485,	// 공격력 향상	
	AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN			= 500,	// 블러드 어택 강화
	AT_SKILL_ML_STRENGTHEN_SWORD6				= 505,	// 파워 슬래쉬 강화
	AT_SKILL_ML_STRENGTHEN_FIRESCREAM			= 525,	// 파이어 스크림 강화
// #endif

//#ifdef UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
	AT_SKILL_ML_STRENGTHEN_MAGUMSA_EVIL			= 530,	// 악령 강화(마검사)
//#endif	
};

enum eOptionType
{
	AT_IMPROVE_DAMAGE       = 80,//추가 공격력           (Level*4)
	AT_IMPROVE_MAGIC        = 81,//추가 마력             (Level*4)
	AT_IMPROVE_BLOCKING     = 82,//추가 방어율           (Level*5)
	AT_IMPROVE_DEFENSE      = 83,//추가 방어력           (Level*4)
	AT_LUCK                 = 84,//영혼의 보석 성공 확율 +25%
    AT_LIFE_REGENERATION    = 85,//생명 자동 회복        (Level*1%)

	AT_IMPROVE_LIFE             = 86,//생명력 증가
	AT_IMPROVE_MANA             = 87,//마나 증가
	AT_DECREASE_DAMAGE          = 88,//데미지 감소
	AT_REFLECTION_DAMAGE        = 89,//데미지 반사
	AT_IMPROVE_BLOCKING_PERCENT = 90,//방어성공율
	AT_IMPROVE_GAIN_GOLD        = 91,//획득 젠 증가
	AT_EXCELLENT_DAMAGE         = 92,//액설런트 데미지 증가
	AT_IMPROVE_DAMAGE_LEVEL     = 93,//공격력 증가(레벨)
	AT_IMPROVE_DAMAGE_PERCENT   = 94,//공격력 증가(퍼센트)
	AT_IMPROVE_MAGIC_LEVEL      = 95,//마력 증가(레벨)
	AT_IMPROVE_MAGIC_PERCENT    = 96,//마력 증가(퍼센트)
	AT_IMPROVE_ATTACK_SPEED     = 97,//공격 속도
	AT_IMPROVE_GAIN_LIFE        = 98,//획득 생명력
	AT_IMPROVE_GAIN_MANA        = 99,//획득 마나


	//  날개 추가 옵션.        

	AT_PLUS_WING_MAXLIFE		= 100,		// 최대 HP+50
	AT_PLUS_WING_MAXMANA		= 101,		// 최대 MP+50
	AT_PLUS_ONE_PERCENT_DAMAGE  = 102,   //  1% 확률로 적에 방어력 무시 공격.
	AT_IMPROVE_AG_MAX           = 103,   //  최대 AG +50 증가.
    AT_DAMAGE_ABSORB            = 104,   //  5% 추가 데미지 흡수.
	AT_PLUS_WING_ADD_LEADERSHIP	= 105,		// 추가 통솔

//#ifdef ADD_ITEM_FENRIR_01_20051110
//	펜릴 추가 옵션.
	AT_PLUS_FENRIR_INCREASE_LAST_DAMAGE = 106,
	AT_PLUS_FENRIR_DECREASE_LAST_DAMAGE = 107,
//#endif

//#ifdef ADD_THIRD_WING_20070525	// 3차날개 추가 옵션
	AT_5PER_IGNORE_DEFENSE		= 108,	// (공격이 명중한 경우) %5 확률로 적 방어력 무시
	AT_5PER_ATTACK_TURNING		= 109,	// (공격당한 경우) 5% 확률로 적 공격력 50% 돌려줌
	AT_RECOVER_100PER_LIFE		= 110,	// (공격당한 경우) 5% 확률로 유저 생명 100% 순간 회복
	AT_RECOVER_100PER_MASIC		= 111,	// (공격당한 경우) 5% 확률로 유저 마력 100% 순간 회복
//#endif
//#ifdef ADD_ILLUSION_FENRIR_20070703	// 환영 펜릴 추가
	AT_PLUS_ILLUSION_FENRIR_OPTION	= 112,	// HP 증가, MP증가, 공격력증가, 마력 증가
//#endif
//#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// 절말의 날개 추가 옵션 
	AT_IMPROVE_CURSE	= 113,
//#endif
	AT_ATTACK1          = 120,
	AT_ATTACK2,

	AT_STAND1,
	AT_STAND2,
	AT_MOVE1,
	AT_MOVE2,

	AT_DAMAGE1,
	AT_DIE1,
	AT_SIT1,
	AT_POSE1,
	AT_HEALING1,
	AT_GREETING1,
	AT_GOODBYE1,
	AT_CLAP1,
	AT_GESTURE1,
	AT_DIRECTION1,
	AT_UNKNOWN1,
	AT_CRY1,
	AT_CHEER1,
	AT_AWKWARD1,
	AT_SEE1,
	AT_WIN1,
    AT_SMILE1,
	AT_SLEEP1,
	AT_COLD1,
	AT_AGAIN1,
	AT_RESPECT1,
	AT_SALUTE1,
	AT_RUSH1,	

	AT_SET_RING_OPTION_ADD_SKILLDAMAGE,					// 세트 추가 데미지(반지, 목걸이류)

	AT_SET_OPTION_IMPROVE_STRENGTH = 160,				// 힘 증가
	AT_SET_OPTION_IMPROVE_DEXTERITY,					// 민첩성 증가
	AT_SET_OPTION_IMPROVE_ENERGY,						// 에너지 증가
	AT_SET_OPTION_IMPROVE_VITALITY,						// 체력 증가
	AT_SET_OPTION_IMPROVE_LEADERSHIP,						// 통솔 증가
	AT_SET_OPTION_IMPROVE_MINATTACKDAMAGE,				// "최소 공격력 증가"
	AT_SET_OPTION_IMPROVE_MAXATTACKDAMAGE,				// "최대 공격력 증가"
	AT_SET_OPTION_IMPROVE_MAGICDAMAGE,					// "마력 증가"
	AT_SET_OPTION_IMPROVE_DAMAGE,						// "데미지 증가"
	AT_SET_OPTION_IMPROVE_ATTACKRATE,					// "공격율 증가"
	AT_SET_OPTION_IMPROVE_DEFENCE,						// "방어력 증가"	
	AT_SET_OPTION_IMPROVE_MAXLIFE,							// "생명 증가"
	AT_SET_OPTION_IMPROVE_MAXMANA,						// "마나 증가"
	AT_SET_OPTION_IMPROVE_MAXAG,						// 최대 AG증가
	AT_SET_OPTION_IMPROVE_AG_VALUE,						// AG증가량 증가
	AT_SET_OPTION_IMPROVE_CRITICALDAMAGE_SUCCESS,		// "크리티컬 데미지 확률 증가"
	AT_SET_OPTION_IMPROVE_CRITICALDAMAGE,				// "크리티컬 데미지 증가"
	AT_SET_OPTION_IMPROVE_EX_DAMAGE_SUCCESS,			// "엑설런트 데미지 확률 증가"
	AT_SET_OPTION_IMPROVE_EX_DAMAGE,					// "엑설런트 데미지 증가"
	AT_SET_OPTION_IMPROVE_SKILLDAMAGE,					// "스킬 공격력 증가"
	AT_SET_OPTION_DOUBLE_DAMAGE,						// "더블데미지 확률"	
	AT_SET_OPTION_DEFENCE_IGNORE,						// "적 방어력 무시"
	AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE,				//	방패 장비시 방어력 5% 증가          (●)
	AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE,		//	양손검 장비시 데미지 15% 증가       ( 서버만 )	
	AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_STR,		// "공격력 증가 힘/x"
	AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_DEX,		// "공격력 증가 민첩/x"
	AT_SET_OPTION_IMPROVE_DEFENCE_WITH_DEX,				// "방어력 증가 민첩성/x"
	AT_SET_OPTION_IMPROVE_DEFENCE_WITH_VIT,				// "방어력 증가 체력/x"	
	AT_SET_OPTION_IMPROVE_MAGICDAMAGE_WITH_ENERGY,		// "마력 증가 에너지/x"
	AT_SET_OPTION_ICE_MASTERY,							//	특정 캐릭터 얼음속성 스킬 추가 데미지
	AT_SET_OPTION_POSION_MASTERY,						//	특정 캐릭터 독속성 스킬 추가 데미지
	AT_SET_OPTION_THUNDER_MASTERY,						//	특정 캐릭터 번개속성 스킬 추가 데미지
	AT_SET_OPTION_FIRE_MASTERY,							//	특정 캐릭터 불속성 스킬 추가 데미지
	AT_SET_OPTION_EARTH_MASTERY,						//	특정 캐릭터 땅속성 스킬 추가 데미지
	AT_SET_OPTION_WIND_MASTERY,							//	특정 캐릭터 바람속성 스킬 추가 데미지
	AT_SET_OPTION_WATER_MASTERY,						//	특정 캐릭터 물속성 스킬 추가 데미지

	AT_SET_IMPROVE_STRENGTH,							// 세트 아이템 옵션 (힘증가 : *3)	: 무기류
	AT_SET_IMPROVE_DEXTERITY,							// 세트 아이템 옵션 (민첩증가 : *3)	: 목걸이류 
	AT_SET_IMPROVE_ENERGY,								// 세트 아이템 옵션 (에너지증가 : *3) : 반지류
	AT_SET_IMPROVE_VITALITY,							// 세트 아이템 옵션 (체력증가 : *3) : 방어구류
	
//
//
//	AT_SET_OPTION_DOUBLE_DAMAGE					= 140,  //	더블 데미지 확률 2%                 ( 서버만 )
//	AT_SET_OPTION_DEFENCE_IGNORE,						//	적 방어력 무시 2%                   ( 서버만 )
//	AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE,		//	양손검 장비시 데미지 15% 증가       ( 서버만 )	
//	AT_SET_OPTION_IMPROVE_ATTACKING_PERCENT,			//	공격 성공률 증가 5%                 (●)
//	AT_SET_OPTION_REFLECTION_DAMAGE,					//	100% 데미지 반사 확률 5% 증가       ( 서버만 처리 )
//	AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE,				//	방패 장비시 방어력 5% 증가          (●)
//	AT_SET_OPTION_IMPROVE_STRENGTH_1,					//	힘 증가 +10                         (●)	                
//	AT_SET_OPTION_IMPROVE_STRENGTH_2,					//	힘 증가 +20                         (●)		                
//	AT_SET_OPTION_IMPROVE_DEXTERITY_1,					//	민첩성 증가 +10                     (●)		            
//	AT_SET_OPTION_IMPROVE_DEXTERITY_2,					//	민첩성 증가 +20                     (●)	                
//	AT_SET_OPTION_IMPROVE_ENERGY_1,						//	에너지 증가 +10	                    (●)	                
//	AT_SET_OPTION_IMPROVE_ENERGY_2,						//	에너지 증가 +20	                    (●)	                
//	AT_SET_OPTION_IMPROVE_VITALITY_1,					//	체력 증가 +10	                    (●)	                
//	AT_SET_OPTION_IMPROVE_VITALITY_2,					//	체력 증가 +20	                    (●)	                
//	AT_SET_OPTION_IMPROVE_MAX_LIFE_1,					//	최대 생명력 증가 +40	            (서버에서 정보를 받는다.)
//	AT_SET_OPTION_IMPROVE_MAX_LIFE_2,					//	최대 생명력 증가 +80            	(서버에서 정보를 받는다.)
//	AT_SET_OPTION_IMPROVE_MAX_MANA_1,					//	최대 마나 증가 +40	               	(서버에서 정보를 받는다.)
//	AT_SET_OPTION_IMPROVE_MAX_MANA_2,					//	최대 마나 증가 +80          		(서버에서 정보를 받는다.)
//	AT_SET_OPTION_IMPROVE_MAX_AG,						//	최대 AG 5% 증가		            	(서버에서 정보를 받는다.)
//	AT_SET_OPTION_DECREASE_SPELL_AG,					//	소비 AG 10% 감소	                (●)
//	AT_SET_OPTION_IMPROVE_ITEM_DROP,					//	아이템 드랍 확률 5% 증가		
//
//	AT_SET_OPTION_IMPROVE_ATTACK_1,						//	특정 캐릭터 공격력 증가 민첩성/60	(●)
//	AT_SET_OPTION_IMPROVE_ATTACK_2,						//	특정 캐릭터 공격력 증가 힘/50   	(●)
//	AT_SET_OPTION_IMPROVE_MAGIC,						//	특정 캐릭터 마력 증가 에너지/70	    (●)
//	AT_SET_OPTION_IMPROVE_DEFENCE_1,					//	특정 캐릭터 방어력 증가 레벨/20	    (●)
//	AT_SET_OPTION_IMPROVE_DEFENCE_2,					//	특정 캐릭터 방어력 증가 3%	        (●)
//	AT_SET_OPTION_IMPROVE_DEFENCE_3,					//	특정 캐릭터 방어력 증가 민첩성/50	(●)
//	AT_SET_OPTION_IMPROVE_DEFENCE_4,					//	특정 캐릭터 방어력 증가 체력/40	    (●)
//
//    AT_SET_OPTION_FIRE_MASTERY,							//	특정 캐릭터 불속성 스킬 추가 공격력 10%		        
//	AT_SET_OPTION_ICE_MASTERY,							//	특정 캐릭터 얼음속성 스킬 추가 공격력 10%			    
//	AT_SET_OPTION_THUNDER_MASTERY,						//	특정 캐릭터 번개속성 스킬 추가 공격력 10%			    
//	AT_SET_OPTION_POSION_MASTERY,						//	특정 캐릭터 독속성 스킬 추가 공격력 10%			        
//	AT_SET_OPTION_WATER_MASTERY,						//	특정 캐릭터 물속성 스킬 추가 공격력 10%			        
//	AT_SET_OPTION_WIND_MASTERY,							//	특정 캐릭터 바람속성 스킬 추가 공격력 10%			    
//	AT_SET_OPTION_EARTH_MASTERY,						//	특정 캐릭터 땅속성 스킬 추가 공격력 10%
//	

//
//	AT_IMPROVE_OPTION_MAX_MANA,				// 최대 마나 옵션
//	AT_IMPROVE_OPTION_MAX_AG,				// 최대 AG증가
};

//#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
enum eJewelOfHarmonyItemOption
{
	// 공통
	AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM				 = 0,	// 효과 없음 : 강화되지 않은 아이템

	// 무기류 옵션
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE	 = 1,	// 최소 공격력 상승 (값)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE	 = 2,	// 최대 공격력 상승 (값)
	AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR		 = 3,	// 필요 힘 감소 (값)
	AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX		 = 4,	// 필요 민첩 감소 (값)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKDAMAGE_BOTH	 = 5,	// 공격력 상승 (최소,최대) (값)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_CRITICALDAMAGE		 = 6,	// 크리티컬 데미지 상승 (값)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_SKILLDAMAGE		 = 7,	// 스킬 공격력 상승 (값)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKSUCCESRATE_PVP= 8,	// 공격 성공율 상승 (PVP)  (값)
	AT_JEWELOFHARMONY_WEAPON_DECREASE_SD_RATE			 = 9,	// SD 감소율 상승 (실드 데미지 흡수율 감소. 직접 HP에 주는 데미지가 증가)  (값)
	AT_JEWELOFHARMONY_WEAPON_IMPROVE_IGNORE_SD_RATE		 = 10,  // 공격시 SD 무시 확률 상승 : SD를 무시하고 직접 HP에 데미지를 가한다.  (값)

	// 지팡이류
	AT_JEWELOFHARMONY_STAFF_IMPROVE_MAGICDAMAGE			 = 1,	// 마력 상승(값)
	AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR		 = 2,	// 필요 힘 감소 (값)
	AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX		 = 3,	// 필요 민첩 감소 (값)
	AT_JEWELOFHARMONY_STAFF_IMPROVE_SKILLDAMAGE			 = 4,	// 스킬 공격력 상승 (값)
	AT_JEWELOFHARMONY_STAFF_IMPROVE_CRITICALDAMAGE		 = 5,	// 크리티컬 데미지 상승 (값)
	AT_JEWELOFHARMONY_STAFF_DECREASE_SD_RATE			 = 6,	// SD 감소율 상승 (실드 데미지 흡수율 감소. 직접 HP에 주는 데미지가 증가)  (값)//#endif
	AT_JEWELOFHARMONY_STAFF_IMPROVE_ATTACKSUCCESRATE_PVP = 7,	// 공격 성공율 상승 (PVP)  (값)
	AT_JEWELOFHARMONY_STAFF_IMPROVE_IGNORE_SD_RATE		 = 8,   // 공격시 SD 무시 확률 상승 : SD를 무시하고 직접 HP에 데미지를 가한다.  (값)

	// 방어구류
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DEFENSE			  = 1,   // 방어력 상승 (값)
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_AG			  = 2,	// 최대 AG 상승 (값)
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_HP			  = 3,	// 최대 HP 상승 (값)
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_HP	  = 4,	// 생명력 자동 증가량 상승
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_MP	  = 5,	// 마나 자동 증가량 상승
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_BLOCKSUCCESRATE_PVP = 6,	// 방어 성공률 상승 (PVP) 값
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DAMAGE_DECREASE	  = 7,	// 데미지 감소 % 상승
	AT_JEWELOFHARMONY_DEFENSE_IMPROVE_SD_RATE			  = 8   // SD 비율 상승 ( PVP시에 SD 비율이 높아짐 )
};

//#else
//----------------------------------------------------------------------------
// 캐릭터의 행동 번호
//enum ActionType
//{
//	//마법사
//	AT_SKILL_POISON		= 1,		// 이곳부터 16개는 마법책과 대응한다.(수정할수 없다)
//	AT_SKILL_METEO		= 2,
//	AT_SKILL_THUNDER	= 3,
//	AT_SKILL_FIREBALL	= 4,
//	AT_SKILL_FLAME		= 5,
//	AT_SKILL_TELEPORT   = 6,
//	AT_SKILL_SLOW		= 7,
//	AT_SKILL_STORM		= 8,
//	AT_SKILL_EVIL		= 9,
//	AT_SKILL_HELL		= 10,
//	AT_SKILL_POWERWAVE  = 11,
//	AT_SKILL_FLASH      = 12,
//	AT_SKILL_BLAST      = 13,
//	AT_SKILL_INFERNO    = 14,
//
//	AT_SKILL_TARGET_TELEPORT		= 15,	// 소환 이동
//	AT_SKILL_MAGICDEFENSE= 16,				// 소울바리어
//
//	AT_SKILL_ENERGYBALL = 17,	
//	
//	//기사
//	AT_SKILL_BLOCKING   = 18,//방어
//	AT_SKILL_SWORD1     = 19,//내려찍기
//	AT_SKILL_SWORD2     = 20,//찌르기
//	AT_SKILL_SWORD3     = 21,//올려치기
//	AT_SKILL_SWORD4     = 22,//돌려치기
//	AT_SKILL_SWORD5     = 23,//베기
//
//	//요정
//	AT_SKILL_CROSSBOW   = 24,//활스킬     (다발)
//
//	AT_SKILL_BOW		= 25,//관통(?)
//
//	AT_SKILL_HEALING    = 26,//치료       (에너지/5+(아이템레벨+1)*5)                   //이곳부터는 마법구술과 대응한다.
//	AT_SKILL_DEFENSE    = 27,//방어력증가 (약30초간 에너지/8+(아아템레벨+1)*2만큼 증가)
//	AT_SKILL_ATTACK     = 28,//공격력증가 (약30초간 에너지/7+(아아템레벨+1)*3만큼 증가)
//	
//
//	AT_SKILL_CALLMON1   = 30,//소환       (아아템레벨에 따라 몹소환)
//	AT_SKILL_CALLMON2   = 31,//소환       (아아템레벨에 따라 몹소환)
//	AT_SKILL_CALLMON3   = 32,//소환       (아아템레벨에 따라 몹소환)
//	AT_SKILL_CALLMON4   = 33,//소환       (아아템레벨에 따라 몹소환)
//	AT_SKILL_CALLMON5   = 34,//소환       (아아템레벨에 따라 몹소환)
//	AT_SKILL_CALLMON6   = 35,//발리       (아아템레벨에 따라 몹소환)
//	AT_SKILL_CALLMON7   = 36,//솔져       (아아템레벨에 따라 몹소환)
//
//	AT_SKILL_WHEEL			= 41,//회오리베기(기사전용)
//	AT_SKILL_BLOWOFFURY		= 42,//검올려찍기(분노의일격)(기사전용)
//	AT_SKILL_STRIKE			= 43,//블로우(기사_블레이드나이트전용)
//	AT_SKILL_KNIGHTSPEAR	= 47,//창찌르기(유니를 타고 있어야 된다)(기사전용)
//	AT_SKILL_KNIGHTADDLIFE	= 48,//스웰라이프(기사전용)	
//	AT_SKILL_KNIGHTDINORANT	= 49,//레이드 슛:디노란트 스킬(기사전용)
//
//	AT_SKILL_ELFHARDEN		= 51,	//아이스애로우(요정)
//	AT_SKILL_PENETRATION	= 52,	//관통(요정)
//
//	AT_SKILL_DEFENSEDOWN	= 55,	//블러드어택(마검사)
//
//	AT_SKILL_SWORD6			= 56,	//테스트 스킬 -> 데스브로드 
//
//
//	//AT_SKILL_4CROSSBOW		= 53,	//요정 4다발
//	AT_SKILL_5CROSSBOW		= 54,	//요정 5다발
//
//	AT_SKILL_EXPPOISON		= 38,	//확장독
//	AT_SKILL_EXPICE			= 39,	//확장아이스
//	
//	AT_SKILL_EXPHELL		= 40,	//확장헬파이어 적용
//	AT_SKILL_EXPHELL_START	= 58,	//확장헬파이어 시작
//	
//	AT_SKILL_IMPROVE_AG_REFILL	= 53,	//민요정 스킬 (AG증가 속도 향상)
//
//	//몬스터
//	AT_SKILL_DEVILFIRE		= 50,
//	
//	AT_SKILL_COMBO			= 59,	// 콤보 스킬
//	
//
//	AT_IMPROVE_DAMAGE       = 60,//추가 공격력           (Level*4)
//	AT_IMPROVE_MAGIC        = 61,//추가 마력             (Level*4)
//	AT_IMPROVE_BLOCKING     = 62,//추가 방어율           (Level*5)
//	AT_IMPROVE_DEFENSE      = 63,//추가 방어력           (Level*4)
//	AT_LUCK                 = 64,//영혼의 보석 성공 확율 +25%
//    AT_LIFE_REGENERATION    = 65,//생명 자동 회복        (Level*1%)
//
//	AT_IMPROVE_LIFE             = 66,//생명력 증가
//	AT_IMPROVE_MANA             = 67,//마나 증가
//	AT_DECREASE_DAMAGE          = 68,//데미지 감소
//	AT_REFLECTION_DAMAGE        = 69,//데미지 반사
//	AT_IMPROVE_BLOCKING_PERCENT = 70,//방어성공율
//	AT_IMPROVE_GAIN_GOLD        = 71,//획득 젠 증가
//	AT_EXCELLENT_DAMAGE         = 72,//액설런트 데미지 증가
//	AT_IMPROVE_DAMAGE_LEVEL     = 73,//공격력 증가(레벨)
//	AT_IMPROVE_DAMAGE_PERCENT   = 74,//공격력 증가(퍼센트)
//	AT_IMPROVE_MAGIC_LEVEL      = 75,//마력 증가(레벨)
//	AT_IMPROVE_MAGIC_PERCENT    = 76,//마력 증가(퍼센트)
//	AT_IMPROVE_ATTACK_SPEED     = 77,//공격 속도
//	AT_IMPROVE_GAIN_LIFE        = 78,//획득 생명력
//	AT_IMPROVE_GAIN_MANA        = 79,//획득 마나
//
//
//	//  날개 추가 옵션.        
//
//	AT_PLUS_WING_MAXLIFE		= 80,		// 최대 HP+50
//	AT_PLUS_WING_MAXMANA		= 81,		// 최대 MP+50
//	AT_PLUS_ONE_PERCENT_DAMAGE  = 82,   //  1% 확률로 적에 방어력 무시 공격.
//	AT_IMPROVE_AG_MAX           = 83,   //  최대 AG +50 증가.
//    AT_DAMAGE_ABSORB            = 84,   //  5% 추가 데미지 흡수.
//	
//
//	AT_ATTACK1          = 100,
//	AT_ATTACK2,
//
//	AT_STAND1,
//	AT_STAND2,
//	AT_MOVE1,
//	AT_MOVE2,
//
//	AT_DAMAGE1,
//	AT_DIE1,
//	AT_SIT1,
//	AT_POSE1,
//	AT_HEALING1,
//	AT_GREETING1,
//	AT_GOODBYE1,
//	AT_CLAP1,
//	AT_GESTURE1,
//	AT_DIRECTION1,
//	AT_UNKNOWN1,
//	AT_CRY1,
//	AT_CHEER1,
//	AT_AWKWARD1,
//	AT_SEE1,
//	AT_WIN1,
//    AT_SMILE1,
//	AT_SLEEP1,
//	AT_COLD1,
//	AT_AGAIN1,
//	AT_RESPECT1,
//	AT_SALUTE1,
//	AT_RUSH1,	
//
//	AT_SET_RING_OPTION_ADD_SKILLDAMAGE,					// 세트 추가 데미지(반지, 목걸이류)
//
//	AT_SET_OPTION_IMPROVE_STRENGTH = 140,				// 힘 증가
//	AT_SET_OPTION_IMPROVE_DEXTERITY,					// 민첩성 증가
//	AT_SET_OPTION_IMPROVE_ENERGY,						// 에너지 증가
//	AT_SET_OPTION_IMPROVE_VITALITY,						// 체력 증가
//	AT_SET_OPTION_IMPROVE_LEADERSHIP,						// 통솔 증가
//	AT_SET_OPTION_IMPROVE_MINATTACKDAMAGE,				// "최소 공격력 증가"
//	AT_SET_OPTION_IMPROVE_MAXATTACKDAMAGE,				// "최대 공격력 증가"
//	AT_SET_OPTION_IMPROVE_MAGICDAMAGE,					// "마력 증가"
//	AT_SET_OPTION_IMPROVE_DAMAGE,						// "데미지 증가"
//	AT_SET_OPTION_IMPROVE_ATTACKRATE,					// "공격율 증가"
//	AT_SET_OPTION_IMPROVE_DEFENCE,						// "방어력 증가"	
//	AT_SET_OPTION_IMPROVE_MAXLIFE,							// "생명 증가"
//	AT_SET_OPTION_IMPROVE_MAXMANA,						// "마나 증가"
//	AT_SET_OPTION_IMPROVE_MAXAG,						// 최대 AG증가
//	AT_SET_OPTION_IMPROVE_AG_VALUE,						// AG증가량 증가
//	AT_SET_OPTION_IMPROVE_CRITICALDAMAGE_SUCCESS,		// "크리티컬 데미지 확률 증가"
//	AT_SET_OPTION_IMPROVE_CRITICALDAMAGE,				// "크리티컬 데미지 증가"
//	AT_SET_OPTION_IMPROVE_EX_DAMAGE_SUCCESS,			// "엑설런트 데미지 확률 증가"
//	AT_SET_OPTION_IMPROVE_EX_DAMAGE,					// "엑설런트 데미지 증가"
//	AT_SET_OPTION_IMPROVE_SKILLDAMAGE,					// "스킬 공격력 증가"
//	AT_SET_OPTION_DOUBLE_DAMAGE,						// "더블데미지 확률"	
//	AT_SET_OPTION_DEFENCE_IGNORE,						// "적 방어력 무시"
//	AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE,				//	방패 장비시 방어력 5% 증가          (●)
//	AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE,		//	양손검 장비시 데미지 15% 증가       ( 서버만 )	
//	AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_STR,		// "공격력 증가 힘/x"
//	AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_DEX,		// "공격력 증가 민첩/x"
//	AT_SET_OPTION_IMPROVE_DEFENCE_WITH_DEX,				// "방어력 증가 민첩성/x"
//	AT_SET_OPTION_IMPROVE_DEFENCE_WITH_VIT,				// "방어력 증가 체력/x"	
//	AT_SET_OPTION_IMPROVE_MAGICDAMAGE_WITH_ENERGY,		// "마력 증가 에너지/x"
//	AT_SET_OPTION_ICE_MASTERY,							//	특정 캐릭터 얼음속성 스킬 추가 데미지
//	AT_SET_OPTION_POSION_MASTERY,						//	특정 캐릭터 독속성 스킬 추가 데미지
//	AT_SET_OPTION_THUNDER_MASTERY,						//	특정 캐릭터 번개속성 스킬 추가 데미지
//	AT_SET_OPTION_FIRE_MASTERY,							//	특정 캐릭터 불속성 스킬 추가 데미지
//	AT_SET_OPTION_EARTH_MASTERY,						//	특정 캐릭터 땅속성 스킬 추가 데미지
//	AT_SET_OPTION_WIND_MASTERY,							//	특정 캐릭터 바람속성 스킬 추가 데미지
//	AT_SET_OPTION_WATER_MASTERY,						//	특정 캐릭터 물속성 스킬 추가 데미지
//
//	AT_SET_IMPROVE_STRENGTH,							// 세트 아이템 옵션 (힘증가 : *3)	: 무기류
//	AT_SET_IMPROVE_DEXTERITY,							// 세트 아이템 옵션 (민첩증가 : *3)	: 목걸이류 
//	AT_SET_IMPROVE_ENERGY,								// 세트 아이템 옵션 (에너지증가 : *3) : 반지류
//	AT_SET_IMPROVE_VITALITY,							// 세트 아이템 옵션 (체력증가 : *3) : 방어구류
//	
////
////
////	AT_SET_OPTION_DOUBLE_DAMAGE					= 140,  //	더블 데미지 확률 2%                 ( 서버만 )
////	AT_SET_OPTION_DEFENCE_IGNORE,						//	적 방어력 무시 2%                   ( 서버만 )
////	AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE,		//	양손검 장비시 데미지 15% 증가       ( 서버만 )	
////	AT_SET_OPTION_IMPROVE_ATTACKING_PERCENT,			//	공격 성공률 증가 5%                 (●)
////	AT_SET_OPTION_REFLECTION_DAMAGE,					//	100% 데미지 반사 확률 5% 증가       ( 서버만 처리 )
////	AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE,				//	방패 장비시 방어력 5% 증가          (●)
////	AT_SET_OPTION_IMPROVE_STRENGTH_1,					//	힘 증가 +10                         (●)	                
////	AT_SET_OPTION_IMPROVE_STRENGTH_2,					//	힘 증가 +20                         (●)		                
////	AT_SET_OPTION_IMPROVE_DEXTERITY_1,					//	민첩성 증가 +10                     (●)		            
////	AT_SET_OPTION_IMPROVE_DEXTERITY_2,					//	민첩성 증가 +20                     (●)	                
////	AT_SET_OPTION_IMPROVE_ENERGY_1,						//	에너지 증가 +10	                    (●)	                
////	AT_SET_OPTION_IMPROVE_ENERGY_2,						//	에너지 증가 +20	                    (●)	                
////	AT_SET_OPTION_IMPROVE_VITALITY_1,					//	체력 증가 +10	                    (●)	                
////	AT_SET_OPTION_IMPROVE_VITALITY_2,					//	체력 증가 +20	                    (●)	                
////	AT_SET_OPTION_IMPROVE_MAX_LIFE_1,					//	최대 생명력 증가 +40	            (서버에서 정보를 받는다.)
////	AT_SET_OPTION_IMPROVE_MAX_LIFE_2,					//	최대 생명력 증가 +80            	(서버에서 정보를 받는다.)
////	AT_SET_OPTION_IMPROVE_MAX_MANA_1,					//	최대 마나 증가 +40	               	(서버에서 정보를 받는다.)
////	AT_SET_OPTION_IMPROVE_MAX_MANA_2,					//	최대 마나 증가 +80          		(서버에서 정보를 받는다.)
////	AT_SET_OPTION_IMPROVE_MAX_AG,						//	최대 AG 5% 증가		            	(서버에서 정보를 받는다.)
////	AT_SET_OPTION_DECREASE_SPELL_AG,					//	소비 AG 10% 감소	                (●)
////	AT_SET_OPTION_IMPROVE_ITEM_DROP,					//	아이템 드랍 확률 5% 증가		
////
////	AT_SET_OPTION_IMPROVE_ATTACK_1,						//	특정 캐릭터 공격력 증가 민첩성/60	(●)
////	AT_SET_OPTION_IMPROVE_ATTACK_2,						//	특정 캐릭터 공격력 증가 힘/50   	(●)
////	AT_SET_OPTION_IMPROVE_MAGIC,						//	특정 캐릭터 마력 증가 에너지/70	    (●)
////	AT_SET_OPTION_IMPROVE_DEFENCE_1,					//	특정 캐릭터 방어력 증가 레벨/20	    (●)
////	AT_SET_OPTION_IMPROVE_DEFENCE_2,					//	특정 캐릭터 방어력 증가 3%	        (●)
////	AT_SET_OPTION_IMPROVE_DEFENCE_3,					//	특정 캐릭터 방어력 증가 민첩성/50	(●)
////	AT_SET_OPTION_IMPROVE_DEFENCE_4,					//	특정 캐릭터 방어력 증가 체력/40	    (●)
////
////    AT_SET_OPTION_FIRE_MASTERY,							//	특정 캐릭터 불속성 스킬 추가 공격력 10%		        
////	AT_SET_OPTION_ICE_MASTERY,							//	특정 캐릭터 얼음속성 스킬 추가 공격력 10%			    
////	AT_SET_OPTION_THUNDER_MASTERY,						//	특정 캐릭터 번개속성 스킬 추가 공격력 10%			    
////	AT_SET_OPTION_POSION_MASTERY,						//	특정 캐릭터 독속성 스킬 추가 공격력 10%			        
////	AT_SET_OPTION_WATER_MASTERY,						//	특정 캐릭터 물속성 스킬 추가 공격력 10%			        
////	AT_SET_OPTION_WIND_MASTERY,							//	특정 캐릭터 바람속성 스킬 추가 공격력 10%			    
////	AT_SET_OPTION_EARTH_MASTERY,						//	특정 캐릭터 땅속성 스킬 추가 공격력 10%
////	
//
////
////	AT_IMPROVE_OPTION_MAX_MANA,				// 최대 마나 옵션
////	AT_IMPROVE_OPTION_MAX_AG,				// 최대 AG증가
//};
//----------------------------------------------------------------------------
//#endif


#endif

