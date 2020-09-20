#ifndef __USER_H__
#define __USER_H__

#include <winsock2.h>

#include "Common\zzzItem.h"
#include "Magicinf.h"
#include "HitC.h"
#include "wsGameServer.h"
#include "MapItem.h"
#include "giocp.h"
#include "GuildClass.h"
#include "CLoginCount.h"

#include "EngagePVP.h"

#ifdef PCBANG_KUZIMO
#include "MDate.h"
#endif

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
	#include "CSAuth.h"

#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
//	#include "Include\ggsrv.h"		
	#include "ggsrv.h"						// 기존 루트와 맞춤 (b4nfter)
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	#include "ggsrv25.h"						// 기존 루트와 맞춤 (hnine)
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

#ifdef ADD_SKILL_WITH_COMBO
#include "ComboAttack.h"
#endif

//#ifdef EVENT_LOTTO
#define ITEMMAKE_INVENTORY_1CELL	235		// DB서버에서 시리얼오면 1칸짜리 아이템을 인벤에 추가		
//#endif // EVENT_LOTTO

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#define ITEMMAKE_INVENTORY_CASHITEM	236		// 캐쉬 아이템 생성시 시리얼 발급을 위한 인덱스
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117f

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// 행운의 동전으로 아이템을 생성하여 인벤에 추가
#define ITEMMAKE_INVENTORY_COINITEM 237		
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_GAMBLING_20090120
#define ITEMMAKE_INVENTORY_GAMBLINGITEM 234
#endif // ADD_GAMBLING_20090120

#ifdef MONSTER_HERD_SYSTEM_20031120
class MonsterHerd;							// OBJECTSTRUCT 구조체에 MonsterHerd * 멤버를 쓰기 위한 전방참조 선언
#endif

#ifdef DARKLORD_WORK
#include "SkillDelay.h"
#endif


#ifdef APPLY_DURATION_MAGIC_HACK_CLEANER_20040504
#include "TDurMagicKeyChecker.h"
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
#include "TMonsterSkillElementInfo.h"
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
#include "TMonsterAI.h"
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#include "JewelOfHarmonySystem.h"
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
	#include "ItemSystemFor380.h"
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef PCBANG_POINT_SYSTEM_20070206		// 파일 선언
#include "PCBangPointSystem.h"
#endif

#ifdef MODIFY_QUEST_SYSTEM_20070525	// "QuestInfo.h"
#include "QuestInfo.h"
#endif

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#include "BuffEffectSlot.h"
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
#include "MasterSkillSystem.h"
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#include "ItemSocketOptionSystem.h"
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415


#ifdef HACKTOOL_KUZIMO
#include "PacketStatistics.h"
#endif

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
#include "HackToolBlock.h"
#endif // ADD_HACKING_TOOL_BLOCK_20090311

// 투명캐릭터 설정시 CtlCode 를 0x06으로 설정하면 방송촬영을 위한 투명캐릭터가 된다 (apple)
// 투명캐릭터 설정시 CtlCode 를 0x0E으로 설정하면 방송촬영+무적 투명캐릭터가 된다 (apple)

// 권한
// gObj->Authority
#define AUTHORITY_USER					0x00000001		// 일반 유저 
#define AUTHORITY_ADMIN					0x00000002		// 관리자 
#define AUTHORITY_VIP_USER				0x00000004		// 게임서버 관리자
//#define AUTHORITY_GM					0x00000008		// 게임 마스터
// #define AUTHORITY_SUBGM					0x00000010		// 서브 게임 마스터
//#ifdef CHINA_GM_EVENT
#define AUTHORITY_EVENT_GM				0x00000020		// 서브 게임 마스터
#define AUTHORITY_SUPER_ADMIN			0x00000040		// 슈퍼 관리자.
#define AUTHORITY_DISABLE				0x00000080
//#endif

// 권한 코드
// 명령어를 받았을때 명령을 입력한 자에 대한 권한이 있는지 체크하기 위해
/*
#define AUTHORITY_CODE_NOTICE			0x00000001		// 공지 가능한가?
#define AUTHORITY_CODE_ACCOUNTBLOCK		0x00000002		// 계정 블럭 가능한가?
#define AUTHORITY_CODE_DISCONNECT		0x00000004		// 강제 접속 종료
#define AUTHORITY_CODE_SETPOSITION		0x00000008		// 위치 이동
#define AUTHORITY_CODE_COPYCHAR			0x00000010		// 캐릭터 변신&복제
#define AUTHORITY_CODE_CHATBAN			0x00000020		// 채팅 금지 시킨다

#define	AUTHORITY_CODE_FULLCONTROL		0xFFFFFFFF		// 모든 기능 가능
*/
// 처벌코드
// gObj->Penalty Chracter = CtlCode
// 캐릭터에 이러한 처벌 코드가 있다면 그 해당 기능에 제약이 따른다.
#define CTLCODE_ACCOUNTBLOCK			0x00000001		// 계정 블럭
#define CTLCODE_CHATBAN					0x00000002		// 채팅 금지
#define CTLCODE_ITEMDONTTOUCH			0x00000004		// 아이템에 손대지 못하게 한다.
#define CTLCODE_ACCOUNTITEMBLOCK		0x00000008		// 계정 아이템 블럭 (계정 안에서의 아이템 이동이 허용됨)

#ifdef CHINA_GM_EVENT
#define CTLCODE_EVENT_GM				0x00000010		// 서브 게임 마스터
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
#define CTLCODE_EVENT_GM				0x00000010		// 서브 게임 마스터
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#ifdef JAPAN_DESTROYKING_EVENT_20040625
#define CTLCODE_EVENT_GM				0x00000010		// 서브 게임 마스터
#endif
// 

#define CTLCODE_TRANSFER_SERVER			0x00000080		// 서버이전 캐릭터

#define OPTION_TRADE					0x00000001		// 거래 상태 1일때만 거래 할 수 있다.

#ifdef DUEL_SYSTEM_20031028
#define OPTION_DUEL						0x00000002		// 결투 상태 1일때만 결투 할 수 있다.
#endif

// 캐릭터 상태..
// gObj[].m_State
#define OBJST_NONE				0x0000		// 
#define OBJST_CREATE			0x0001		// 캐릭터 생성
#define OBJST_PLAYING			0x0002		// 게임을 진행중이다.
#define OBJST_DYING				0x0004		// 죽어가고 있다.
#define OBJST_DIECMD			0x0008		// 죽여라!!
#define OBJST_DIED				0x0010		// 죽었다.
#define OBJST_DELCMD			0x0020		// 순간이동을 위해 리스트에서 삭제


// Client Service State ( 클라이언트의 서비스 상태를 나타낸다.)
#define CSS_LOGIN				1		// 인증이 아직 안되었으며..  오브젝트만 할당됨
#define CSS_LOGINOK				2		// 인증된 상태다
#define CSS_CHARACTERVIEW		2		// 캐릭터 리스트 상태에 있다.^
#define CSS_GAMEPLAYING			3		// 게임 플레이 중 이다.

#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
#define CSS_LOGINWAIT			-1		// 인증 요청을 한 후 대기 상태
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801

// 게임 종료 세트에서 사용
#define CL_EXIT					0		// 게임완전 종료
#define CL_BACK					1		// 이전으로 돌아가기
#define CL_SERVERLIST			2		// 서버 선택으로 가기

// 메시지 처리 정의
#define MSG_BEATTACK			2		// 공격당했다.
#define MSG_FILLENERGY			3		// 에너지를 채운다.
#define MSG_GUILDWAREND			4		// 길드전쟁 끝남을 알린다.
#define MSG_BSSTART				5		// 배틀사커 게임 시작
#define MSG_BSPAUSE				6		// 배틀사커 게임 잠시 멈춤
#define MSG_BSSTOP				7		// 배틀사커 게임 중단
#define MSG_REFLECT				10		// 공격 반사

#define MSG_LIFECHECK			11		// 생명체크

// _MODIFY_ADD_THIRD_WING_20070623
#ifdef ADD_THIRD_WING_20070525
#define	MSG_ATTACK_TURNING		12		// 공격력 돌려줌
#define MSG_FULL_LIFE			13		// 유저생명 완전 회복
#define MSG_FULL_MANA			14		// 유저마나 완전 회복
#endif

#define MSG_BILLSEND			1000	// 과금 정보를 보낸다.

#define MSG_ATTACKPROC_ENTIREMAGIC		50		// 전체 공격

#ifdef DARKLORD_WORK
#define MSG_ATTACKPROC_DARKSPRITATTACK	51		// 다크스피릿 공격
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#define MSG_ATTACKPROC_WEAPONATTACK		52		// 공성 무기 공격
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
#define MSG_ATTACKPROC_FENRIR			53		// 펜릴 공격
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// MSG_ATTACKPROC_FIRESCREAM
#define MSG_ATTACKPROC_FIRESCREAM		54		// 파이어 스크림 더블데미지에 의한 폭발
#endif

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	// RKFKA 20060728 번호 변경
	#define MSG_ATTACK_NORMAL			55		// 딜레이있는 일반 공격 설정
	#define	MSG_MONSTER_SKILL_POISON	56		// 딜레이있는 스킬 공격
	#define	MSG_MONSTER_SKILL_SPRING	57		// 딜레이있는 밀기 공격
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// LighingOrb에 대한 Splash Damage 메시지
#define MSG_ATTACK_LIGHTNINGORB			58
#endif

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425	// pullution을 위한 메시지
#define MSG_SKILL_PULLUTION1			59
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425

// 전직 관련
#define	CHANGEUP_MASK			0x07	// Class에서 2,3,4bit를 사용한다

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3차 전직 캐릭터 번호 지정
#define GRAND_MASTER		3		// 그랜드마스터 (법사 3차전직)
#define BLADE_MASTER		19		// 블레이드마스터 (기사 3차전직)
#define HIGH_ELF			35		// 하이엘프	(요정 3차전직)
#define DUEL_MASTER			50		// 듀얼마스터 (마검사 3차전직)
#define LORD_EMPEROR		66		// 로드엠퍼러 (다크로드 3차전직)
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 신규캐릭터 2,3차전직 번호지정
#define BLOODY_SUMMONER		81		// 블러디 서머너
#define DIMENSION_MASTER	83		// 디멘션 마스터 (소환술사 3차전직)
#endif

typedef struct tagActionState
{
	DWORD Rest:1;		// 쉬다.
	DWORD Attack:1;		// 공격하다.
	DWORD Move:1;		// 움직이다.
	DWORD Escape:1;		// 도망치다.
	DWORD Emotion:4;	// 감정, 0:보통, 1:흥분, 2:빡돌음, 4:완전돌았다..
	DWORD EmotionCount:8;// 감정 카운터로서 1Tick에 한번씩 감소한다. 0에 가까워지면 보통이된다.
} ACTION_STATUS, *LPACTION_STATUS;


#ifdef EXPAND_INTERFACE_BYTE_TO_DWORD_20041209
typedef struct tagInterfaceState
{
	DWORD  use:2;		// 사용되어지는지..
	DWORD  state:4;		// 0: 요청, 1: 진행중
	DWORD  type:10;		// 1: 인벤토리, 2:파티, 3:상점
	
} INTERFACE_STATUS, *LPINTERFACE_STATUS;
#else
typedef struct tagInterfaceState
{
	BYTE  use:2;		// 사용되어지는지..
	BYTE  state:2;		// 0: 요청, 1: 진행중
	BYTE  type:4;		// 1: 인벤토리, 2:파티, 3:상점
	
} INTERFACE_STATUS, *LPINTERFACE_STATUS;
#endif

typedef struct
{
	int		MsgCode;	// 메시지 코드
	int		SendUser;	// 메시지를 보낸 유저
	DWORD	MsgTime;	// 메시지 보낸시간
	int		SubCode;	// 서브로 사용되는 코드
} MessageStateMachine;


typedef struct
{
	int		MsgCode;	// 메시지 코드
	int		SendUser;	// 메시지를 보낸 유저
	DWORD	MsgTime;	// 메시지 보낸시간
	int		SubCode;	// 서브로 사용되는 코드
	int		SubCode2;	// 서브코드 2
} ExMessageStateMachine;


extern CLoginCount	    gLCount[];

//----------------------------------------------------------------------------
// 과금 클래스 
//----------------------------------------------------------------------------
class BILL_CLASS
{

private:

#ifdef UPDATE_UNIFICATIONBILL_20090203
	BYTE btDeductType;
	DWORD dwRestPoint;
	DWORD dwRestTime;
#else	// UPDATE_UNIFICATIONBILL_20090203	
	char cCertifyType;
	BYTE PayCode;
	int  EndTime;
#endif	// UPDATE_UNIFICATIONBILL_20090203

	char EndsDays[13];

public:

	BILL_CLASS()
	{
		Init();
	};
	
	void Init()
	{
#ifdef UPDATE_UNIFICATIONBILL_20090203
		btDeductType	= BILL_TYPE_NONE;
		dwRestPoint		= 0;
		dwRestTime		= 0;
#else	// UPDATE_UNIFICATIONBILL_20090203
		cCertifyType	= -1;
		PayCode			= 0;
		EndTime			= 0;
#endif	// UPDATE_UNIFICATIONBILL_20090203

		EndsDays[0]		= '\0';
	};

	// 과금을 세팅한다.
#ifdef UPDATE_UNIFICATIONBILL_20090203
	BOOL SetBill(BYTE deductType, DWORD rest_point, DWORD rest_time, char *ends_days)
	{
		BOOL bRet=FALSE;
		
		if( btDeductType == BILL_TYPE_NONE ) bRet = TRUE;
		btDeductType		= deductType;
		
		dwRestPoint			= rest_point;
		dwRestTime			= rest_time;
		strcpy(EndsDays, ends_days);
		
		return bRet;
	};
#else	// UPDATE_UNIFICATIONBILL_20090203
	BOOL SetBill(BYTE certify_type, BYTE pay, DWORD end_time, char *ends_days)
	{
		BOOL bRet=FALSE;
		
		if( cCertifyType == -1 ) bRet = TRUE;
		cCertifyType	= certify_type;
		PayCode			= pay;
		EndTime			= end_time;
		strcpy(EndsDays, ends_days);
		return bRet;
	};
#endif	// UPDATE_UNIFICATIONBILL_20090203
	
	// 시간이 종료되었나?
	BOOL IsEndTime()
	{
#ifdef UPDATE_UNIFICATIONBILL_20090203
		if( btDeductType == BILL_TYPE_NONE             )	return FALSE;
		if( btDeductType == BILL_TYPE_PCBANG_QUANTITY )		return FALSE;
		if( btDeductType == BILL_TYPE_PERSONAL_QUANTITY )   return FALSE;
		
		// 시간이 다되었다면.
		if( dwRestTime < 0 )								return TRUE;
#else	// UPDATE_UNIFICATIONBILL_20090203
		if( cCertifyType == -1             )    return FALSE;
		if( cCertifyType == CERTIFYTYPE_IP )    return FALSE;
		if( PayCode      == BILLTYPE_JUNGYANG ) return FALSE;
		
		// 시간이 다되었다면.
		if( EndTime < 0 )						return TRUE;
#endif	// UPDATE_UNIFICATIONBILL_20090203	
		
		return FALSE;
	};

#ifndef UPDATE_UNIFICATIONBILL_20090203
	BYTE  GetPayCode()  { return PayCode; };
#endif	// UPDATE_UNIFICATIONBILL_20090203

	char* GetEndsDays() { return EndsDays; };

#ifdef UPDATE_UNIFICATIONBILL_20090203
	DWORD	GetRestTime()	{ return dwRestTime; }
	DWORD	GetRestPoint()	{ return dwRestPoint; }
	BYTE	GetDeductType()	{ return btDeductType; }
#else	// UPDATE_UNIFICATIONBILL_20090203
	int   GetEndTime()  { return EndTime; };
	int   GetCertify()  { return cCertifyType; };
#endif	// UPDATE_UNIFICATIONBILL_20090203
} ;
//----------------------------------------------------------------------------


#define MAXMESSAGE				10		// 일반 메시지 
#define MAXATTACKMESSAGE		50		// 공격처리 메시지

extern MessageStateMachine	gSMMsg[][MAXMESSAGE];	// 메시지

#ifdef ADD_SKILL_WITH_COMBO
extern ExMessageStateMachine	gSMAttackProcMsg[MAX_USER][MAXATTACKMESSAGE];	// 메시지
#elif defined(NEW_ATTACK_PROC)
extern ExMessageStateMachine	gSMAttackProcMsg[MAX_USER][MAXATTACKMESSAGE];	// 메시지
#endif

extern BILL_CLASS	m_ObjBill[];
//PICKUP
struct PICK_UP_ITEMS
{
	int Type;
	int Index;
	int Item;

};

#ifdef MEM2_KUZIMO
	class ZeroMemBase
	{
	public:	
		void	*operator new[] (size_t);
		void	operator delete[] (void*);
	};

	class OBJECTSTRUCT : public ZeroMemBase
	{
	public:
#else
	typedef struct OBJECTSTRUCT
	{
#endif
	int		m_Index;					// 자신의 배열번호

	//-----------------------------------------------------------------------
	// 접속 관련 및 관리	
	int		Connected;					// 접속되어 있나?	1이면 접속되었고.. 2이면 인증도 받았고 3이면 게임 진행중..

	char	LoginMsgSnd;			// 접속메시지 보낸 후라면 1, 인증메시지를 받기 
	char	LoginMsgCount;			// 인증을 몇번 물어봤나? 실패 3번이면 접속을 종료시킨다.
	
	char	CloseCount;				// 접속 종료시 카운터..  1이면 종료 시킴 (시간 카운터는 CloseCount-1)
	char	CloseType;				// 종료 타입
	
#ifdef BLOCK_INVALID_CHARACTER_DELETE_01_20060327
	BOOL	bEnableDelCharacter; 	// 캐릭터 삭제가 가능한가?
#endif

	//-----------------------------------------------------------------------
	// SOCKET && NETWORK
	PER_SOCKET_CONTEXT *PerSocketContext;		
	SOCKET				m_socket;
	char				Ip_addr[16];				// ip주소

	int  UserNumber;				// 인증서버에서의 유저 번호
	int  DBNumber;					// DB에 저장되어 있는 유저의 절대 번호(Guid)	

#ifdef ADD_PROTOCOL_PCBANG_GUID	
	int		PcBangGuid;				// PC방 Guid (0이면 피씨방 사용자 아님)
#endif	
	BYTE Magumsa;					// 마검사를 만들수 있는 계정인가?

	//-----------------------------------------------------------------------
	// Tick 및 시간관련
	DWORD AutoSaveTime;				// 자동 저장 시간
	DWORD ConnectCheckTime;			// 연결되었는지 체크하는 타임
	DWORD CheckTick;				// Tick 동기화에 사용되기 위해 접속시에 클라이언트 Tick을 접수
	BYTE  CheckSpeedHack;
	DWORD CheckTick2;
	BYTE  CheckTickCount;			// Tick 동기화에 사용되는 카운트수

#if defined ( MODIFY_HACKLOG_OF_ATTACK_20050518 ) || defined ( MU_CHN_HACKLOG3_MODIFY_20041117 )
	DWORD	dwPlayTime;
#endif
	
#ifdef ITEMSAVE_MODIFY_FORSTATICS
	DWORD SaveTimeForStatics;		// 캐릭터 통계를 위해서 현재 시간에 접속해 있다고 알리기 위한 시간(1시간간격)
#endif

	int		iPingTime;
	BYTE	m_TimeCount;			// 내구도 감소를 위한 타이머
	DWORD	m_dwPKTimer;			// PK레벨 조정을 위한 타이머
	
	//-----------------------------------------------------------------------
	// 체크섬
	short CheckSumTableNum;
	DWORD CheckSumTime;

	//------------------------------------------------------------------------
	// 
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD	Type;						// 타입: 0이면 유저, 1이면 몬스터
#else
	BYTE	Type;						// 타입: 0이면 유저, 1이면 몬스터
#endif
	BYTE	Live;
	
	//-----------------------------------------------------------------------
	// 캐릭터 정보
	char	CharacterPos;				// 게임 진행중인 캐릭터 번호(5개중)

	char	AccountID[MAX_IDSTRING+1];	// 계정 아이디
	char	Name[MAX_IDSTRING+1];		// 캐릭터 이름 
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
	char	LastJoominNumber[MAX_JOOMINNUMBER_LENGTH+1];		// 주민번호
#else
	char	LastJoominNumber[14];		// 주민번호
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424

#ifdef SECOND_QUEST
	BYTE	PlusStatQuestClear;			// 퀘스트가 클리어 되면, 레벨업시 +1 스탯추가
	BYTE	ComboSkillquestClear;		// 콤보 스킬 퀘스트 클리어
#endif

#ifdef ADD_SKILL_WITH_COMBO
	ComboSkillData	comboSkill;
#endif

	//------------------------------------------------------------------------
	// 기본 정보
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD	Class;				// 직업 - 몬스터는 종족으로 사용됨
#else
	BYTE	Class;				// 직업 - 몬스터는 종족으로 사용됨
#endif
	BYTE    DbClass;			// 원래직업 정보

#ifdef NEW_SKILL_FORSKYLAND
	BYTE	ChangeUP;			// 전직상태
#else
	BYTE    Skin;				// 스킨 (캐릭터가 입고있는 기본 옷 종류)
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507
	BYTE	ThirdChangeUp;		// 3차 전직상태
#endif	

	short   Level;				// 레벨
	int     LevelUpPoint;		// 레벨업 포인트

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	int		iFruitPoint;		// 열매 포인트
#endif

	DWORD	Experience;			// 경험치
	DWORD	NextExp;			// 다음 경험치
	
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 캐릭터 기본 변수 선언
	short	m_nMasterLevel;					// 마스터레벨	

	INT64	m_i64MasterLevelExp;			// 마스터레벨 경험치
	INT64	m_i64NextMasterLevelExp;		// 다음 마스터레벨 경험치

	int		m_iMasterLevelPoint;			// 마스터레벨 포인트

	BOOL	m_bMasterLevelDBLoad;			// 마스터레벨 정보를 정상적으로 불러왔는지 여부
#endif

	int		Money;				// 돈
	WORD	Strength;			// 힘
	WORD	Dexterity;			// 민첩성 
	WORD	Vitality;			// 건강
	WORD	Energy;				// 에너지
	float	Life;				// 현재 생명
	float	MaxLife;			// 현재 최대 생명값
	
#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
	int		m_iScriptMaxLife;	// Monster.txt 에 스크립팅 된 최대 생명값
#endif
	
	float   FillLife;			// 채워야 하는 생명
	float   FillLifeMax;		// 채워야 하는 생명
	float	Mana;				// 현재 마나
	float	MaxMana;			// 현재 최대 마나값
	
#ifdef DARKLORD_WORK	
	WORD	Leadership;			// 통솔
	WORD	AddLeadership;		// 추가 통솔
	WORD	ChatLitmitTime;		// 채팅 제한 시간
	BYTE	ChatLimitTimeSec;	// 채팅 제한 시간(초)
#endif

#ifdef ADD_HP_POTION_LEVEL1_20040401
	BYTE	FillLifeCount;		// 채워야 하는 생명 <- 몇번만에 채울것이냐?
#endif

	WORD	AddStrength;			// 추가힘
	WORD	AddDexterity;			// 추가민첩성 
	WORD	AddVitality;			// 추가체력
	WORD	AddEnergy;				// 추가에너지

#ifdef NEW_FORSKYLAND2
	int		BP;					// AG 파워
	int		MaxBP;				// 최대AG파워
	int		AddBP;				// 추가 AG파워
#endif

	float   VitalityToLife;
	float   EnergyToMana;
	
	char	m_PK_Count;
	char	m_PK_Level;
	int     m_PK_Time;

	short	X;
	short	Y;

	BYTE	Dir;
	BYTE	MapNumber;

	int		AddLife;			// 추가 생명값
	int		AddMana;			// 추가 마나값
	
#ifdef ADD_SHIELD_POINT_01_20060403
	int		iShield;
	int		iMaxShield;
	int		iAddShield;

	int		iFillShieldMax;
	int		iFillShield;
	int		iFillShieldCount;

	DWORD	dwShieldAutoRefillTimer;
#endif

	//------------------------------------------------------------------------
	// 엑설런트 옵션
	BYTE	DamageMinus;
	BYTE	DamageReflect;

	short 	MonsterDieGetMoney;
	BYTE	MonsterDieGetLife;
	BYTE	MonsterDieGetMana;
	

	//------------------------------------------------------------------------
	// 이동 정보
	BYTE	StartX, StartY;	// 처음 시작 좌표X,Y

	short	m_OldX;	// 이전 X 좌표
	short	m_OldY;	// 이전 Y 좌표
	
	short	TX;	// 대상 X
	short	TY;	// 대상 Y
	short	MTX;	// 이동하려는 곳의 목적지 X
	short	MTY;	// 이동하려는 곳의 목적지 Y

	int		PathCount;
	int		PathCur;
	char	PathStartEnd;			// 0이면 이동 완료, 1이면 이동 시작
	short	PathOri[MAX_ROADPATH];// 패스원본
	short	PathX[MAX_ROADPATH];
	short	PathY[MAX_ROADPATH];
	char	PathDir[MAX_ROADPATH];
	DWORD	PathTime;

	char				m_MoveGateNumber;	// 이동하려는 게이트 번호
	
	//------------->
	//20080930 해외 5차 부분유료화 부활, 이동 부적 (grooving)
	#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
	short	m_siDieRegenX;			// 부활 부적 X
	short	m_siDieRegenY;			// 부활 부적 Y
	short	m_siDieRegenMapIndex;	// 부활 부적 Map
	
	short	m_siMoveX;				// 이동 부적 X
	short	m_siMoveY;				// 이동 부적 Y
	short	m_siMoveMapIndex;		// 이동 부적 Map
	#endif // ADD_TELEPORT_CHARM_ITEM_20080930
	//<-------------
	
	//------------------------------------------------------------------------
	// 사용자 권한
	DWORD	Authority;	// 권한
// by kuzimo
//	DWORD	AuthorityCode;
	DWORD	Penalty;		// 처벌
	BYTE	m_cAccountItemBlock;	// 계정 아이템블럭인가?	

	//------------------------------------------------------------------------
	// 상태 코드
	ACTION_STATUS		m_ActState;			// 액션 상태 
	BYTE				m_ActionNumber;		// 현재 행동 번호 
	
	DWORD				m_State;			// 캐릭터의 현재 상태 변수
	char				m_StateSub;			// 캐릭터의 현재 서브 상태 변수

	BYTE				m_Rest;				// 현재 휴식 중이라면..

	char				m_ViewState;		// 캐릭터의 보여지는 상태 변수

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE				m_BuffEffectCount;
	EFFECTLIST			m_BuffEffectList[MAX_BUFF_SLOT_INDEX];
#else
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	INT					m_ViewSkillState;
	#else
		#ifdef CHANGE_VIEWSTATE	
		WORD				m_ViewSkillState;
		#else
		BYTE				m_ViewSkillState;
		#endif
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	
	DWORD				m_LastMoveTime;		// 맵이동이나 타운포탈 이용시 마지막 이용한시간

	DWORD				m_LastAttackTime;			// 공격시 마지막 시간.

#ifdef __FRIEND_WORK__
	BYTE				m_FriendServerOnline;	// 친구 서버 온/오프라인 0이면 친구서버에 로그온을 요청한다.
#endif
	//------------------------------------------------------------------------
	// 핵 체크
	int		m_DetectSpeedHackTime;	// 스피드핵 체크 시간
	DWORD	m_SumLastAttackTime;	// 공격속도가 이상한 시간의 합
	DWORD	m_DetectCount;			// 공격속도가 이상하게 들어온 수	
	int		m_DetectedHackKickCount;			// 공격속도가 이상하게 들어온 수	
	int		m_SpeedHackPenalty;		// 공격속도가 이상하게 들어온 수

	BYTE	m_AttackSpeedHackDetectedCount;
	
	DWORD	m_PacketCheckTime;		// 패킷의 무시 시간
	BYTE	m_ShopTime;				// 상점에서 물건을 살수있는 최대 시간

	DWORD	m_TotalAttackTime;		// 총 공격 시간
	int		m_TotalAttackCount;		// 총 공격 카운트

	DWORD	TeleportTime;
	char	Teleport;
	
	char	KillerType;				// 나를 죽인것은?? 몬스터 인지.. 사람인지.. 길드사람인지..

	//------------------------------------------------------------------------
	// 리젠
	char    DieRegen;				// 죽었다가 다시 살아난다.

	char	RegenOk;
	BYTE	RegenMapNumber;
	BYTE	RegenMapX;
	BYTE	RegenMapY;	
	DWORD	RegenTime;
	DWORD   MaxRegenTime;			// 

	//------------------------------------------------------------------------	
	short	m_PosNum;				// 위치테이블 번호

	
	DWORD	LifeRefillTimer;		// 생명력 다시 채워줄 시간..
	DWORD	CurActionTime;			// 현재 행동 시간
	DWORD	NextActionTime;			// 다음 액션 시간.
	DWORD	DelayActionTime;		// 액션지연 시간 
	char	DelayLevel;				// 지연 레벨 

#ifdef ADD_SKILL_WITH_COMBO
	char 	m_PoisonType;			// 중독일때 중독 타입(스킬번호)
	char 	m_IceType;				// 얼었을때 중독 타입(스킬번호)
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518
	char	m_ImmuneToMagicCount;		// 마법공격 무적 시간
	char	m_ImmuneToHarmCount;		// 물리공격 무적 시간
	char	m_iMonsterBattleDelay;		// 몬스터가 전투 중 잠시 머뭇거림 등 딜레이되는 시간 (공격되지도 받지도 않음)
	char	m_cKalimaGateExist;			// 내가 소유한 마석이 존재하는지 확인한다. (소유주)
	int		m_iKalimaGateIndex;			// 내가 소유한 마석의 인덱스
	char	m_cKalimaGateEnterCount;
#endif

	OBJECTSTRUCT *lpAttackObj;		// 나를 공격한 유저 오브젝트

	short	m_SkillNumber;			// 스킬번호	(-1 이면 스킬이 존재하지 않는것!)
	DWORD	m_SkillTime;			// 스킬적용시 시간

#ifdef USER_DIE_MAKE_OPPOSIT_PK_20040329
	bool	m_bAttackerKilled;		// 내가 죽었을 때 공격을 하다 죽었는지 여부
#endif

	char    m_ManaFillCount;		// 마법사 마나를 채울 시간
	char    m_LifeFillCount;		// 마법사 마나를 채울 시간
	

#ifdef MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221	// 정당방위 최대 인원 수정
#define MAXSELFDEFENSE	7		// 최대 7명에 대한 정당방위를 실시할수 있다. 
#else	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221
#define MAXSELFDEFENSE	5		// 최대 5명에 대한 정당방위를 실시할수 있다. 
#endif	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221

	//------------------------------------------------------------------------	
	//정당방위
	int		SelfDefense[MAXSELFDEFENSE];		// 정당방위 대상
	DWORD   SelfDefenseTime[MAXSELFDEFENSE];	// 정당방위 시간
	DWORD   MySelfDefenseTime;					// 나의 마지막 정당방위 시간

	

	//-----------------------------------------------------------------------
	// 술, 공향/방향 등의 능력치 증가
	char	m_Drink;			// 술을 마셨을때 능력

	int		m_SkillAttack2;		// 공격력 증가 스킬2 (사랑의 묘약)
	char	m_SkillAttackTime2;	// 	

	//-----------------------------------------------------------------------
	// PARTY
	int     PartyNumber;		// 사용자가 속해있는 파티 번호 : Default : -1
	int		PartyTargetUser;	// 파티를 맺고자 하는 유저의 번호 (확인용)  : Default : -1

	//-----------------------------------------------------------------------
	// GUILD
	int		GuildNumber;		// 사용자가 속해있는 길드 번호
	LPGUILD_INFO lpGuild;		// 사용자가 속해있는 길드의 포인터
	char	GuildName[MAX_IDSTRING+1];

#ifdef ADD_NEW_GUILD_01_20040913
	INT		GuildStatus;		// 길드 직책 및 상태 
#endif

#ifdef ADD_NEW_UNION_01_20041006
	INT		iGuildUnionTimeStamp;	// Union 및 Rival 정보 갱신을 위한 TimeStamp.
#endif
	
	//-----------------------------------------------------------------------
	// 소환
	int		m_RecallMon;		// 소환수
	
	int		m_Change;			// 변신

	//-----------------------------------------------------------------------		
	short	TargetNumber;		// 어떤 경우 대상이 되는 번호( 상점번호)
	short	TargetShopNumber;
	
	short	ShopNumber;			// NPC는 상점 번호로 쓰인다.
	
#ifdef FOR_BLOODCASTLE
	short	LastAttackerID;		// 나를 마지막 공격한 사람의 인덱스 (몬스터일 경우만)
#endif


	//-----------------------------------------------------------------------		
	// 공격력/방어력/민첩등의 스탯정보
	int		m_AttackDamageMin;	// 최소 공격력(몬스터의 경우 최초에 결정
	int		m_AttackDamageMax;	// 최대 공격력(사용자의 경우, 실시간으로 계산됨)

	int		m_MagicDamageMin;	// 최소 마법 공격력(실시간)
	int		m_MagicDamageMax;	// 최대 마법 공격력

	int		m_AttackDamageLeft;		// 왼손 공격력(실시간으로 계산됨)
	int		m_AttackDamageRight;	// 오른손 공격력 	

	int		m_AttackDamageMaxLeft;	// 최소 왼손 공격력(장비 착용시 계산)
	int		m_AttackDamageMinLeft;	// 최대 왼손 공격력

	int		m_AttackDamageMaxRight;	// 최소 오른손 공격력(장비 착용시 계산)
	int		m_AttackDamageMinRight;	// 최대 오른손 공격력

	int		m_AttackRating;			// 공격률(?)

	int		m_AttackSpeed;			// 공격속도
	int		m_MagicSpeed;			// 마법속도
	
	int		m_Defense;				// 방어력
	int		m_MagicDefense;			// 마법방어력
	
	int		m_SuccessfulBlocking;	// 방어성공률

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 저주력 추가
	int		m_Curse;				// 저주력
	int		m_CurseDamgeMin;		// 최소 저주력
	int		m_CurseDamgeMax;		// 최대 저주력
#endif

	short	m_MoveSpeed;		// 이동 속도
	short   m_MoveRange;		// 이동 범위
	short   m_AttackRange;		// 공격 범위
	short   m_AttackType;		// 공격 형태

	short   m_ViewRange;
	short   m_Attribute;		// 속성

	short	m_ItemRate;			// 아이템 드랍 확률(몬스터)
	short   m_MoneyRate;		// 젠 드랍 확률(몬스터)

	int		m_CriticalDamage;	// 크리티컬 데미지 확률(아이템의 행운이 적용)
	int		m_ExcelentDamage;

	//-----------------------------------------------------------------------		
	// 마법/스킬 정보
	CMagicInf*	m_lpMagicBack;
	CMagicInf*	Magic;

	char		MagicCount;
	BYTE		UseMagicNumber;	// 현재 공격한 마법 번호
	DWORD		UseMagicTime;	// 마지막 공격한 마법의 시간
	char		UseMagicCount;	// 한번 마법에 타격 개수
	
	short       OSAttackSerial;
	BYTE		SASCount;
	DWORD		SkillAttackTime;

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// MASTERLEVEL_PASSIVE_SKILL_OPTION m_MPSkillOpt
	MASTERLEVEL_PASSIVE_SKILL_OPTION	m_MPSkillOpt;
#endif
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 소울바리어 소모 마나 수치 저장 변수
	int			m_nSoulBarrierDefence;
	short		m_nSoulBarrierManaRate;
#endif

	//------------------------------------------------------------------------
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE		CharSet[MAX_PREVIEWCHARSET+9];		// 자신의 장비 정보를 미리 만들어 두어서 뷰포트 안의 다른 사용자에게 보낸다.
#else
#ifdef DARKLORD_WORK
	BYTE		CharSet[MAX_PREVIEWCHARSET+4];		// 자신의 장비 정보를 미리 만들어 두어서 뷰포트 안의 다른 사용자에게 보낸다.
#else
		BYTE	CharSet[MAX_PREVIEWCHARSET+3];		// 자신의 장비 정보를 미리 만들어 두어서 뷰포트 안의 다른 사용자에게 보낸다.
#endif
#endif

	BYTE	m_Resistance[MAX_RESISTANCE];		// 각각의 저항력 
	
	BYTE	m_AddResistance[MAX_RESISTANCE];		

	//------------------------------------------------------------------------
	// ViewPort
	int FrustrumX[4];
	int FrustrumY[4];
	
	VIEWPORT_STRUCT			VpPlayer[MAXVIEWPORTOBJECT];
	VIEWPORT_PLAYER_STRUCT	VpPlayer2[MAXVIEWPORTOBJECT];	// 나를 보고 있는 유저는?
	int  VPCount;		// 뷰포트에 들어와 있는 사용자 개수
	int  VPCount2;


	//------------------------------------------------------------------------
	// 경험치 분배 및 아이템 루팅 우선권을 위한 자신을 공격한 Obj에 대한 정보
	HITDAMAGE_STRUCT		sHD[MAX_HITDAMAGE];
	short	sHDCount;
	
	//------------------------------------------------------------------------
	// 인터페이스 관련 (인벤, 창고, 카오스박스 등)
	INTERFACE_STATUS	m_IfState;	
	DWORD				m_InterfaceTime;		// 인터페이스 요청 시간

	// 인벤토리
	CItem * pInventory;				// 아이템 목록
	BYTE  * pInventoryMap;
	char  * pInventoryCount;
	char    pTransaction;			// 인벤토리 처리 상태 1=사용, 0=사용안함

	CItem*  Inventory1;				// 아이템 목록
	BYTE*	InventoryMap1;			// 인벤토리 착용 체크용 버퍼
	char    InventoryCount1;

	CItem*  Inventory2;				// 아이템 목록
	BYTE*	InventoryMap2;			// 인벤토리 착용 체크용 버퍼
	char    InventoryCount2;

	// 거래창
	CItem*	Trade;					// 교환창 아이템 목록
	BYTE *	TradeMap;				// 교환창 체크용 버퍼
	int 	TradeMoney;				// 교환할 돈의 액수
	BYTE	TradeOk;				// 교환 OK 버튼 상태

	// 창고
	CItem*  pWarehouse;				// 사용자의 창고
	BYTE *  pWarehouseMap;			// 
	char    WarehouseCount;			// 몬스터: 창고에 있는 아이템의 개수
									// 사용자: 창고의 아이템 사용을 했는지의 여부... (ㅡ,ㅡ)
	short	WarehousePW;			// 창고 비밀번호 0 이면 잠금 해제, 그 이상이면 잠금상태
	BYTE	WarehouseLock;			// 창고의 잠금 상태 0이면 사용할수 있고 1이면 사용할수 없다.
	BYTE    WarehouseUnfailLock;	// 창고 잠금을 해제 할때 비밀번호 실패한 카운트
	int		WarehouseMoney;			// 창고에 있는 금액
	BOOL	WarehouseSave;			// 창고를 사용 했었는지..
	
	// 카오스창
	CItem*  pChaosBox;				// 카오스박스
	BYTE *  pChaosBoxMap;			// 
	int		ChaosMoney;				// 믹스시 필요한 금액
	int		ChaosSuccessRate;		// 성공 확률 
	int		ChaosLock;				// ChaosLock 가 1이면 어떠한 이동도 할수없다.
	
	//------------------------------------------------------------------------
	DWORD	m_Option;				// 게임 옵션

	//------------------------------------------------------------------------
	// 이벤트용 
	// 악마의 광장
	int		m_nEventScore;			// 이벤트용 점수
	int		m_nEventExp;			// 이벤트용 경험치
	int		m_nEventMoney;			// 이벤트용 돈	
	BYTE	m_bDevilSquareIndex;	// 광장 인덱스
	BYTE	m_bDevilSquareAuth;		// 악마의 광장 인증(?)	

#ifdef FOR_BLOODCASTLE
	// 블러드캐슬
	char	m_cBloodCastleIndex;	// 블러드 캐슬의 다리번호 (0 ~ 6)
	char	m_cBloodCastleSubIndex;	// 블러드 캐슬의 다리에 몇번째 방에 내가 있는가 (사람일 경우만)
	int		m_iBloodCastleEXP;		// 블러드 캐슬에서 얻은 경험치 (사람일 경우만)
#endif
#ifdef BLOODCASTLE_EVENT_5TH_20050531
	bool	m_bBloodCastleComplete;	// 해당 사용자가 블러드캐슬을 완료 (우승자 이거나 우승자와 같은 파이일 때 대천사와 대화) 했는지 확인
#endif
	
#ifdef CHAOSCASTLE_SYSTEM_20040408
	// 카오스 캐슬
	char	m_cChaosCastleIndex;	// 카오스 캐슬의 다리번호 (0 ~ 7)
	char	m_cChaosCastleSubIndex;	// 카오스 캐슬의 몇번째 방에 내가 있는가 (사람일 경우만)
	int		m_iChaosCastleBlowTime;	// 카오스 캐슬에서 마지막으로 폭발로 밀린 시간 (클라이언트쪽에서 돌려치기로 미는것 방지)
	char	m_cKillUserCount;		// 카오스 캐슬에서 PK를 한 횟수
	char	m_cKillMonsterCount;	// 카오스 캐슬에서 몬스터를 죽인 횟수
#endif

#ifdef DUEL_SYSTEM_20031028
	//------------------------------------------------------------------------
	// 결투
	int		m_iDuelUserReserved;	// 나와 대전할 상대방 (내가 신청)
	int		m_iDuelUserRequested;	// 나에게 대전을 신청한 상대방
	int		m_iDuelUser;			// 나와 대전하고 있는 상대방
	BYTE	m_btDuelScore;			// 결투 중의 내 점수

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	DWORD	m_iDuelTimer;			// 결투 시간	
#endif	
#endif
#ifdef DUEL_SYSTEM_FIX_20031224
	int		m_iDuelTickCount;		// 결투 후 공격했을 때의 시간을 체크	
#endif


#ifdef PERSONAL_SHOP_20040113	
	//------------------------------------------------------------------------
	// 개인상점
	bool	m_bPShopOpen;							// 개인상점을 오픈했는지의 여부
	bool	m_bPShopTransaction;					// 개인상점의 상품 교환 중의 트랜잭션 (true인 동안은 거래 불가)
	bool	m_bPShopItemChange;						// 개인상점의 상품이 팔렸을 경우등에 아이템이 변경된 경우 (파는 사람 입장에서)
	bool	m_bPShopRedrawAbs;						// 개인상점의 뷰포트를 무조건 다시 그려라
	CHAR	m_szPShopText[MAX_PSHOP_TEXT];			// 개인상점의 이름

	bool	m_bPShopWantDeal;						// 다른 사람의 개인상점 창을 보고 있는가 여부
	int		m_iPShopDealerIndex;					// 내가 보고 있는 상점 주인의 인덱스
	CHAR	m_szPShopDealerName[MAX_IDSTRING];		// 내가 보고 있는 상점 주인의 케릭명

	CRITICAL_SECTION	m_critPShopTrade;			// 개인상점의 물건이동이 진행될 때의 동기화를 위해 사용되는 CRITICAL_SECTION

	INT		m_iVpPShopPlayer[MAXVIEWPORTOBJECT];	// 개인상점의 뷰포트
	WORD	m_wVpPShopPlayerCount;					// 개인상점의 뷰포트안에 실제 가지고 있는 데이터의 수
#endif
	

	//------------------------------------------------------------------------
	// 기타
	BYTE	IsInBattleGround;		// 전투축구 중인가?
	BYTE	HaveWeaponInHand;		// 손에 무기를 들고 있는가?

	// 레나이벤트(20030509)
	short	EventChipCount;				// 이벤트 칩의 수
	int		MutoNumber;					// 뮤토번호
	int		UseEventServer;

	BYTE	LoadWareHouseInfo;		// 창고정보를 읽었는가?

#ifdef FOR_STONE_EXCHANGE
	INT		iStoneCount;			// 스톤의 개수
#endif

	// 퀘스트
#ifdef MODIFY_QUEST_SYSTEM_20070525	// 퀘스트 완료요건중 몬스터 킬수 처리 추가(DB저장)	
	BOOL	m_bQuestMonKillDBLoad;		// DB에 해당 캐릭터의 정보가 있을 때만 불러온다.
	int		m_iQuestMonKillDBIndex;		// DB에 저장된 현재 퀘스트 인덱스 - 몬스터킬수가 완료요건일 경우	
	QUEST_INFO_MOSNTER_KILLCOUNT m_QuestMonKillInfo[MAX_QUEST_MONSTER_KILL_INFO];
#endif
	
#ifdef NEW_SKILL_FORSKYLAND
	BYTE	m_Quest[MAX_DBQUEST];
	BYTE	m_SendQuestInfo;

	int		m_SkyBossMonSheildLinkIndex;	// 보스몹 방어막 인덱스
	int		m_SkyBossMonSheild;				// 방어막을 사용하는 시간
	int		m_SkyBossMonSheildTime;			// 방어막이 펼쳐지는 시간
#endif

#ifdef NEW_FORSKYLAND2
	int		m_MaxLifePower;					// 최대 생명력

	int		m_PacketChecksumTime;
	int		m_CheckLifeTime;
#endif


#ifdef GAMESERVER_DIVISION
	BYTE	m_MoveOtherServer;				// 서버를 이전시키고 있는가?	
#endif

#ifdef LOGINSERVER_MULOG
	char	BackName[MAX_IDSTRING+1];		// 캐릭터 이름 백업
#endif

	char	m_BossGoldDerconMapNumber;
	BYTE	m_InWebzen;

	char	m_LastTeleportTime;				// 이동을 한후, 일정 시간 전기등으로 인해 뒤로 안밀리게 설정하기 위한 값

	BYTE	m_ClientHackLogCount;

#ifdef FOR_THAILAND
	BYTE	m_AuthAdult;
	BYTE	m_DisplayTeenLogOutMsg;
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	int		m_iVerifyType;					// 중국 피로도 성인 인증 타입 (0 : 성인, 1 : 미성년자, 2 : 오류)
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	INT		m_iPlayTimeLimit;				// 중국 피로드 - 게임시간 (5시간 지나면 게임혜택 0%)
	DWORD	m_dwPlayTick;					// 게임시간을 계산하기 위한 틱 카운트
	INT		m_iPlayNoticeTick;				// 게임 중간에 남은 게임시간을 공지해주기 위한 틱카운트
#endif
	
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	INT		m_iPlayTimeLimit;				// (Sec) 베트남 피로도- 누적게임시간 (5시간 지나면 게임혜택 0%)
	INT		m_iPlayRestTime;				// (Sec) 베트남 피로도- 누적휴식시간
	DWORD	m_dwPlayTick;					// (ms)	 게임시간을 계산하기 위한 틱 카운트
	INT		m_iPlayNoticeTick;				// (ms)  게임 중간에 남은 게임시간을 공지해주기 위한 틱카운트
#endif

#ifdef MONSTER_HERD_SYSTEM_20031120
	BOOL	m_bIsInMonsterHerd;				// 특정 몬스터 무리에 속하는지 ?
	BOOL	m_bIsMonsterAttackFirst;		// 선공 몹인지 아닌지 ?
	MonsterHerd * m_lpMonsterHerd;			// 내가 속한 몬스터 무리 클래스의 포인터, 무리의 GUID로도 활용 (반드시 다른 변수에 받아놓고 쓴다.)
#endif

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
	CCSAuth	NPggCSAuth;	
	DWORD	m_NPggCheckSum;
#endif
	
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	CCSAuth2 NPggCSAuth;
	bool m_bSentGGAuth;
#endif 

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	CCSAuth25 NPggCSAuth;
	bool m_bSentGGAuth;
	bool m_bSentGGAuthAgain;
	#ifdef MODIFY_GAMEGUARD_NOT_AUTH_MAPSERVER_MOVE_20060513
		bool m_bCheckGGAuth;
#ifdef MODIFY_GAMEGARD_AUTH_MAPSERVER_MOVE_20080723	// 맵서버 이동시 재전송 시간
		DWORD m_dwNPGGMapSvrMoveTime;
#endif	// MODIFY_GAMEGARD_AUTH_MAPSERVER_MOVE_20080723
	#endif
#endif

	DWORD	m_NPggCheckSumSendTime;
	
	int		fSkillFrustrumX[4];
	int		fSkillFrustrumY[4];

#ifdef ADD_SKILL_WITH_COMBO
	BYTE	SkillHellFire2State;
	BYTE	SkillHellFire2Count;
	DWORD	SkillHellFire2Time;
#endif
	
#ifdef WAREHOUSE_ITEMCOPY_BUG_FIX
	BYTE	m_ReqWarehouseOpen;	
#endif

//#ifdef CHECK_ATTACK_AREA                  // define 설정을 해제해야 국가별 핵로그 처리에 영향이 없음 (u4us)
	int		m_NotAttackAreaCount;			// 공격 안되는 지역에서 공격 시도한 횟수
//#endif

#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
	int     m_NotAttackAreaCountFor10Min;   // 최근 10 분동안 누적되는 제한 구역 공격 횟수
#endif

//#if defined ( MODIFY_HACKLOG_OF_ATTACK_20050518 ) || defined ( MU_CHN_HACKLOG3_MODIFY_20041117 )
//	DWORD	dwPlayTime;
//#endif

	short	SetOpAddSkillAttack;				// 스킬 공격력 증가 v

	short	SetOpAddExDamage;					// 엑설런트 데미지 증가 v
	short	SetOpAddExDamageSuccessRate;		// 엑설런트 데미지 증가	v
	short	SetOpAddCriticalDamage;				// 크리티컬 데미지 증가 v
	short	SetOpAddCriticalDamageSuccessRate;	// 크리티컬 데미지 확률 증가 v

	short	SetOpIncAGValue;					// AG증가량 증가 v
	short	SetOpAddDamage;						// 데미지증가 v
	short	SetOpAddMinAttackDamage;			// 최소 공격력 증가 v
	short	SetOpAddMaxAttackDamage;			// 최대 공격력 증가 v

	short	SetOpAddAttackDamage;				// 추가 공격력 v
	short	SetOpAddDefence;					// 추가 방어력 v
	short	SetOpAddDefenceRate;				// 추가 방어력(%) v
	short	SetOpAddMagicPower;					// 추가 마력 v

	BYTE	SetOpIgnoreDefense;					// 적방어 무시 확률 v
	BYTE	SetOpDoubleDamage;					// 더블 데미지 v
	BYTE	SetOpTwoHandSwordImproveDamage;		// 양손검 장비시 데미지 15% 증가 v
	BYTE	SetOpImproveSuccessAttackRate;		// 공격 성공률 증가 v
	BYTE	SetOpReflectionDamage;				// 100% 데미지 반사 확률 5% 증가 v
	BYTE	SetOpImproveSheldDefence;			// 방패 장착시 방어력 5%증가 v
	BYTE	SetOpDecreaseAG;					// 소비 AG 10% 감소 v
	BYTE	SetOpImproveItemDropRate;			// 아이템 드랍 확률 5% 증가	 v

	BYTE	IsFullSetItem;



#ifdef TEST_SETITEM_CLEAR
	BYTE	ClearInfoForSetItem;
#endif

#ifdef APPLY_DURATION_MAGIC_HACK_CLEANER_20040504
	TDurMagicKeyChecker DurMagicKeyChecker;
#endif

#ifdef DARKLORD_WORK	
	WORD	SkillRecallParty_Time;
	BYTE	SkillRecallParty_MapNumber;
	BYTE	SkillRecallParty_X;
	BYTE	SkillRecallParty_Y;
#endif
	
#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
	bool	bIsChaosMixCompleted;			// 카오스 조합 중에 조합이 완료되었는지 (1) / 미완료 인지 (0) 의 상태값
#endif
	
#ifdef DARKLORD_WORK
	BYTE	SkillLongSpearChange;
	CSkillDelay	SkillDelay;
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
	INT		iObjectSecTimer;
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
	BYTE	bWaitBlockState;
#endif
	
#ifdef MAP_SERVER_WORK_20041030				// 맵서버 이동에 필요한 사용자 데이터
	bool	m_bMapSvrMoveQuit;				// 맵서버를 이동하기 위해 나갈 예정이었는가 ? 
	bool	m_bMapSvrMoveReq;				// 맵서버 이동 요청을 했었는가 ?
	DWORD	m_dwMapSvrQuitTick;				// 맵서버 이동 시의 틱 카운트
	SHORT	m_sPrevMapSvrCode;				// 이전 맵서버 코드
	SHORT	m_sDestMapNumber;				// 이동할 맵번호	(이동된 서버쪽에서 시작 시 변경)
	BYTE	m_btDestX;						// 이동할 X좌표		(이동된 서버쪽에서 시작 시 변경)
	BYTE	m_btDestY;						// 이동할 Y좌표		(이동된 서버쪽에서 시작 시 변경)
#endif
#ifdef	CASTLE_MAIN_SCHEDULER_20041111
	// 공성전 NPC들에 필요한 데이터
	union {
		struct {
			BYTE	m_btCsNpcExistVal1;		// NPC가 유효한지 확인하기 위한 값 - 1 (1 ~ 255)
			BYTE	m_btCsNpcExistVal2;		// NPC가 유효한지 확인하기 위한 값 - 2 (0 ~ 255)
			BYTE	m_btCsNpcExistVal3;		// NPC가 유효한지 확인하기 위한 값 - 3 (0 ~ 255)
			BYTE	m_btCsNpcExistVal4;		// NPC가 유효한지 확인하기 위한 값 - 4 (0 ~ 255)
		};
		INT			m_iCsNpcExistVal;		// NPC가 유효한지 확인하기 위한 값 (0이면 무효)
	};
	BYTE	m_btCsNpcType;
	BYTE	m_btCsGateOpen;					// 성문만 사용 -> 현재 게이트가 열린상태인지 확인 (초기값 1:닫힘)
	INT		m_iCsGateLeverLinkIndex;		// 성문,레버-서로의인덱스
	// NPC 업그레이드
	BYTE	m_btCsNpcDfLevel;				// 성문,석상-방어력
	BYTE	m_btCsNpcRgLevel;				// 석상-회복력
	// 공성전 시 현 사용자의 공성 소속 여부를 확인하기 위한 값
	BYTE	m_btCsJoinSide;					// 공성전 길드 소속값 (0:관련아님/1:수성측/2이상:공성측) / 라이프스톤 - 소환자의 길드 소속값
	bool	m_bCsGuildInvolved;				// 공성에 실제 관련된 당사자 길드인지 확인 (0:당사자아님/1:당사자)
#endif
	
#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
	bool	m_bIsCastleNPCUpgradeCompleted;	// 공성 관련 NPC를 업그레이드 했는가? ( 0:안했다 1:했다 )
											// 클라이언트 강제종료시 해당 유저의 변경 된 아이템 정보가 rollback 되는것을 막는다.
#endif	

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
	// 공성전 무기 상태
	BYTE	m_btWeaponState;
	INT		m_iWeaponUser;
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// 스킬을 사용하기 위해 필요한 조건 카운터
	BYTE	m_btKillCount;
#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	INT		m_iAccumulatedDamage;			// 석상, 성문 가격시 데미지 누적
#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
	BYTE	m_btLifeStoneCount;				// 길드의 라이프스톤 개수
	BYTE	m_btCreationState;				// 생성 상태.
	INT		m_iCreatedActivationTime;		// 생성 후 시간.
	
#endif
	
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
	INT		m_iAccumulatedCrownAccessTime;	// 직인 등록 누적 시간

#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
	// 몬스터 스킬엘리먼트의 효과 지속 및 조건
	TMonsterSkillElementInfo	m_MonsterSkillElementInfo;
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	// 몬스터 AI
	int		m_iBasicAI;
	int		m_iCurrentAI;
	
	// 몬스터 AI 상태 -> 구조체로 바꿀것인가?
	int		m_iCurrentAIState;

	// 몬스터 AI 적용 시간
	int		m_iLastAIRunTime;
	
	// 몬스터 Group 번호
	int		m_iGroupNumber;
	int		m_iSubGroupNumber;

	int		m_iGroupMemberGuid;
	int		m_iRegenType;

//	int		m_iLastAIPathSpot;

	TMonsterAIAgro	m_Agro;
	#ifdef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
		int		m_iLastAutomataRuntime;	// 몬스터 Automata 적용 시간
		int		m_iLastAutomataDelay;	// 몬스터 현재 Automata의 Delay 값
	#endif // MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725


#ifdef CRYWOLF_MVP_REWARD_RANK_20051215
	int		m_iCrywolfMVPScore;
#endif

#ifdef PCBANG_ADVANTAGE
	BOOL	m_bPCBangUser;				// happycat@20050201 (PC방 혜택)		
#endif

#ifdef PCBANG_FREE_KALIMA_EVENT_20060124
	BOOL	m_bPCBangCouponUser;
#endif
	
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	BOOL	m_bPCBangFreeChaosCastleUser;	// PC방 카오스캐슬 무료 입장 이벤트(RKFKA)
#endif

#ifdef HAPPY_POUCH // happycat@20050201 (복주머니) - 국내 설 이벤트	
	LONG	m_lFreeKalimaTime;			//Second
	LONG	m_lLifeIncTime;
	BYTE	m_btLifeInc;
#endif
//#ifdef HAPPY_CAT
	DWORD	m_dwLastCheckTick;			// 몬스터는 별도의 용도로 사용 (로랜협곡이벤트 - b4nfter)
//#endif
	
#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
	int m_iAutoRecuperationTime;
#endif	

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	#ifdef NEW_SKILL_FORSKYLAND
	int		m_SkillAddLife;			// 생명력 증가 스킬
	int		m_SkillAddLifeTime;		// 생명력 증가 스킬지속 시간

	int		m_SkillHarden;			// 석화 스킬
	int		m_SkillHardenTime;		// 석화 스킬 지속 시간	

	int		m_SkillMagumReduceDefense;		// 마검 방어력 감소
	int		m_SkillMagumReduceDefenseTime;	// 마검 방어력 감소 지속시간
	#endif

	#ifdef NEW_FORSKYLAND2
	int		m_WizardSkillDefense;			// 법사 방어력 증가
	int		m_WizardSkillDefenseTime;
	#endif // NEW_FORSKYLAND2

	#ifdef DARKLORD_WORK
	WORD	SkillAddCriticalDamageTime;
	WORD	SkillAddCriticalDamage;
	#endif // DARKLORD_WORK

	#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// STUN
	INT		m_iSkillStunTime;				// 스턴 지속 시간
	
	// BRAND OF SKILL
	INT		m_iSkillBrandOfSkillTime;		// 브랜드오브스킬 지속시간

	// Invisible
	INT		m_iSkillInvisibleTime;			// 인비져블 지속시간

	// Mana Swell
	INT		m_iSkillManaSwellTime;			// 마나스웰 지속시간
	INT		m_iSkillManaSwell;				// 마나스웰량
	#endif

	#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	// Bless Potion
	INT		m_iPotionBlessTime;				// 축복의 물약 지속 시간

	// Spirit Potion
	INT		m_iPotionSoulTime;				// 영혼의 물약 지속 시간 
	#endif // ADD_NEW_ITEM_FOR_CASTLE_01_20041116

	char 	m_PoisonBeattackCount;	// 0 이면 중독상태가 아니고 그 이상이면 중독 
	char    m_ColdBeattackCount;	// 0이면  얼음공격 당한 상태가 아니고 그 이상이면 얼음 공격을 당한 상태이다.

	int 	m_SkillDefense;		// 방어력 증가 스킬(요정방향)
	DWORD	m_SkillDefenseTime;	

	int		m_SkillAttack;		// 공격력 증가 스킬(요정공향)
	DWORD	m_SkillAttackTime;	// 

	#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
	int m_iSkillNPCHelpTime;
	int m_iSkillNPCDefense;
	int m_iSkillNPCAttack;
	#endif

	#ifdef ADD_PCS_MARK_OF_EXP_20070205
	WORD	m_wExprienceRate;
	WORD	m_wItemDropRate;
	BYTE	m_btMoveMapBound;
	#endif // ADD_PCS_MARK_OF_EXP_20070205

	#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	// 뮤즈엘프 인피니티 애로우 스킬 지속시간
	int m_iMuseElfInfinityArrowSkillTime;
	#endif

	#ifdef PCBANG_POINT_SYSTEM_20070206
	// PC방 포인트 아이템 사용 정보
	int		m_iPCBangPointItemTime;				// 아이템 사용 남은시간(Sec)
					
	// PC방 포인트 아이템을 적용했을 때 나타나는 효과
	int		m_iPCBangPointItemAddExpPer;			// 추가 획득 경험치 확률
	BYTE	m_bPCBangPointNoMoreExp;				// TRUE : 경험치가 증가하지 않는다 / FALSE
	int		m_iPCBangPointItemGiveItemPer;		// 아이템 획득 추가 확률
	#endif // PCBANG_POINT_SYSTEM_20070206
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	WORD m_wExcellentDropRate;
#ifdef ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907
	WORD m_wAncientDropRate;
#endif
	
#ifdef ADD_ANTI_HACK_01_20051027
	// PACKET_ATTACKET 패킷에 대한 시간 체크
	DWORD	m_dwLastPacketAttackTime;
#endif
	
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
	int m_iSkillDistanceErrorCount;	// 스킬 거리 체크 이상한 경우 
#endif	//MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417

#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421
	DWORD m_dwSkillDistanceErrorTick;	// 스킬 거리 핵 사용자 체크 위한 타임틱
#endif
	
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// 조화의 보석에 의한 강화 아이템 옵션 정보
	JEWELOFHARMONY_ITEM_EFFECT	m_JewelOfHarmonyEffect;
#endif	// ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
	ITEMOPTION_FOR380ITEM_EFFECT m_ItemOptionExFor380;	// 380 추가 옵션 정보
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_NEW_NPC_KANTURU_20060612
	BOOL	m_bKanturuEntranceByNPC;	// 칸투르 입장 NPC를 통해 칸투르 보스전 맵에 입장한 유저이다.
#endif
	
#ifdef ITEM_ADD_OPTION_20061019
	WORD	m_wItemEffectType;
	int		m_iItemEffectValidTime;
#endif // ITEM_ADD_OPTION_20061019

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	WORD	m_wCashPoint;
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PCBANG_POINT_SYSTEM_20070206			// PCBANG_POINT_INFO 선언
	BOOL				m_bPCBangPointUser;		// PC방 포인트 시스템 대상 유저인가? 0:아니다/1:그렇다	
	PCBANG_POINT_INFO	m_PCBangPointInfo;		// PC방 포인트 정보
#ifdef PCBANG_KUZIMO
	MWeeklyTimer		m_kWeeklyTimer;
#endif
#endif	// PCBANG_POINT_SYSTEM_20070206

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영 사원의 유저 인덱스 추가
	int		m_iIllusionTempleIndex;
#endif

#ifdef MODIFY_0708_BUFFIX_20070820	// 스킬 키값을 받았는지 여부
	BOOL	m_bSkillKeyRecv;		
#endif

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	int		m_iPeriodItemEffectIndex;
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	
#ifdef ADD_CHARCARD_SUMMONER_20080313
	BYTE	m_bCharCardSummoner;
#endif // ADD_CHARCARD_SUMMONER_20080313

#ifdef MODIFY_DENY_USER_DISCONNNECT_HACK_20080323
	int		m_UserKillRecvCount;
#endif // MODIFY_DENY_USER_DISCONNNECT_HACK_20080323

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
	SOCKET_OPTION_LIST	m_SeedOptionList[MAX_SEEDSPHERE_OPTION_DATA];
	SOCKET_OPTION_LIST	m_BonusOptionList[MAX_BONUS_OPTION_DATA];
	SOCKET_OPTION_LIST	m_SetOptionList[MAX_SET_OPTION_DATA];
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
	
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	WORD	m_btRefillHPSocketOption;
	WORD	m_btRefillMPSocketOption;
#else
	BYTE	m_btRefillHPSocketOption;
	BYTE	m_btRefillMPSocketOption;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
	
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	WORD	m_wSocketOptionMonsterDieGetHp;
	WORD	m_wSocketOptionMonsterDieGetMana;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	BYTE	m_btAGReduceRate;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

#ifdef HACKTOOL_KUZIMO
	PacketStatistics	m_kRecvPacketStatistics;
#endif
	
#ifdef GMCOMMAND_EX
	BYTE	m_bGMSummon;
#endif // GMCOMMAND_EX

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
	DWORD	m_dwUseHacktoolList[MAX_HACKTOOL_LIST];
#endif // ADD_HACKING_TOOL_BLOCK_20090311
	
#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	DWORD	m_dwMapMoveKeyValue;
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#ifdef UPDATE_RESEND_GETCHARlIST_DISABLE_20090605
	BOOL	m_bRequestPaket;		// 패킷을 보냈는지 여부
	int		m_nRequestPacketCnt;	// 같은 패킷이 몇번 왓는지 여부
#endif	// UPDATE_RESEND_GETCHARlIST_DISABLE_20090605

	BYTE	m_AccountType;
	INT64	m_EndTime;
	time_t	m_JoinTime;
	DWORD	m_dwRequestVip;
	bool	m_bRequestVip;
	bool	m_bVIPReload;

	bool	m_bResetState;
	INT		m_iResetNumber;
	bool	m_bReqUpdateReset;

	bool	m_bShell;

	PVP_USER_STATE	m_stateEngagePVP;

	bool	m_bBuffHelper;

	//offafk
	bool OffAfk;
	bool Offtrade;
	//DevGamesX 01/05/2018 - OffExp
	int DbOffExp;
	bool OffExp;
	int OffExpTick;
	int OffExpSpeed;

	int OffTradeZen;
	int OffTradeCredits;
	int BotStore;
	int m_ConnectBonusTime;

	//PICKUP
	PICK_UP_ITEMS m_pickup[20];
	int PickUpZen;
	
#ifdef MEM2_KUZIMO
	};

	typedef OBJECTSTRUCT* LPOBJECTSTRUCT;
#else
	} OBJECTSTRUCT, *LPOBJECTSTRUCT;
#endif

// OBJECTSTRUCT, *LPOBJECTSTRUCT;
/*
inline BOOL CheckAuthority(DWORD dwMyAuthority, DWORD dwCmdAuthority)
{
	return(((dwMyAuthority & dwCmdAuthority) == dwMyAuthority)? true : false);
}
*/
inline bool CheckAuthority(DWORD dwComposeAuthority, OBJECTSTRUCT *pMyObject)	// dwComposeAuthority는 조합속성이다. pMyObject->Authority는 조합하지 말자.
{
	return ((pMyObject->Authority & AUTHORITY_DISABLE) ? false : ((pMyObject->Authority & dwComposeAuthority) ? true : false));
	//return(((pMyObject->Authority & dwComposeAuthority) == pMyObject->Authority)? true : false);
}


#ifdef MEM_KUZIMO
extern OBJECTSTRUCT *gObj;
#else
extern OBJECTSTRUCT	gObj[];
#endif

extern short RoadPathTable[];

extern int gServerMaxUser;
extern int gObjTotalUser;

#ifdef ADD_SKILL_WITH_COMBO
extern void gObjAddAttackProcMsgSendDelay(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2 = 0);
extern void gObjStateAttackProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
#elif defined(NEW_ATTACK_PROC)
extern void gObjAddAttackProcMsgSendDelay(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2 = 0);
extern void gObjStateAttackProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
#endif

extern void  gObjInit();
extern void  gObjEnd();
extern short gObjMemFree(int index);
extern short gObjSometimeClose(int index);

extern void gObjDestroy(SOCKET aSocket, int client);
extern short gObjAdd(SOCKET aSocket,char *ip, int aIndex);
extern short gObjAddCallMon();
extern short gObjAddSearch(SOCKET aSocket,char *ip);

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
extern short gObjAddMonster(int iMapNumber);
#else
extern short gObjAdd();
#endif

extern short gObjDel(int socket);
extern int gObjGameClose(int aIndex);
extern int gObjGetSocket(SOCKET socket);
extern void gObjBillRequest(LPOBJECTSTRUCT lpObj);
extern void gObjUserKill(int aIndex);
extern void gObjViewportClose(LPOBJECTSTRUCT lpObj);
extern BOOL gObjJoominCheck(int aIndex, char *szInJN);

extern BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int iw, int ih);		// 

#ifdef PERSONAL_SHOP_20040113
extern BYTE gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
#endif

//#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
// ADD_PARTIALY_CHARGE_CASH_SHOP_20070117 정의 이후 버전에서 사용 가능하도록 
// ADD_PARTIALY_CHARGE_CASH_SHOP_20070117 제외함
extern BOOL CheckInventoryEmptySpace(LPOBJECTSTRUCT lpObj, int iItemHeight, int iItemWidth);
//#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
extern BOOL CheckInventoryEmptySpace_(LPOBJECTSTRUCT lpObj, int iItemHeight, int iItemWidth);
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

extern char *gObjGetAccountId(int aIndex);
extern int   gObjGetIndex(char *szId);
extern BOOL  gObjUserIdConnectCheck(char *szId, int index);
extern BOOL gObjIsConnectedGP(int aIndex);
extern BOOL gObjIsConnected(LPOBJECTSTRUCT lpObj);
extern BOOL gObjIsConnected(LPOBJECTSTRUCT lpObj, int dbnumber);
extern BOOL gObjIsConnected(int aIndex);
#ifdef ADD_NEWPVP_PKFIELD
extern BOOL gObjIsConnectedEx(int aIndex);
#endif // ADD_NEWPVP_PKFIELD
extern BOOL gObjIsAccontConnect(int aIndex, char *accountid);
extern BOOL gObjIsGamePlaing(LPOBJECTSTRUCT lpObj);

#ifdef ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131
#define DBGNAME_Default					0
#define DBGNAME_PMoveProc				1
#define DBGNAME_RecvPositionSetProc		2
#define DBGNAME_gObjSecondProc			3
extern BOOL gObjCheckXYMapTile(LPOBJECTSTRUCT lpObj, int iDbgName = DBGNAME_Default );
#else
extern BOOL gObjCheckXYMapTile(LPOBJECTSTRUCT lpObj);
#endif

extern void gObjTimeCheckSelfDefense(LPOBJECTSTRUCT lpObj);


#ifdef ADD_PROTOCOL_PCBANG_GUID	

	#ifdef PCBANG_ADVANTAGE //happycat@20050201 (PC방 해택)
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, int aPCBangGuid, char *JoominNumber, BYTE	bPCBangUser);
	#else
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, int aPCBangGuid, char *JoominNumber);
	#endif//PCBANG_ADVANTAGE

#else
	#if defined(PCBANG_ADVANTAGE) && defined(FOR_TAIWAN)
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, int aPCBangGuid, char *JoominNumber, BYTE	bPCBangUser);
	#else
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, char *JoominNumber);
	#endif
#endif


extern BOOL  gObjSetCharacter(BYTE *lpdata, int aIndex);
extern BOOL  gObjSetMonster(int aIndex, int MonsterClass);
extern BOOL  gObjSetPosMonster(int aIndex, int PosTableNum);

extern void gObjViewportListCreate(short aIndex);
extern void gObjViewportListDestroy(short aIndex);
extern void gObjViewportListProtocol(short aIndex);
extern void gObjSetState();
extern void gObjSecondProc();


#ifdef PERSONAL_SHOP_20040113	// 개인상점 개설 여부를 체크하고 개인상점용 뷰포트에 추가한다.
// 개인상점의 뷰포트 리스트를 재구축 한다. -> 추가된 개인상점에 대한 패킷도 보낸다.
extern void		PShop_ViewportListRegenarate(short aIndex);
// 해당 인덱스의 개인상점이 비었는지 확인한다. (true - 비었다 / false - 물건이 있다.)
extern bool		PShop_CheckInventoryEmpty(short aIndex);
#endif

#ifdef ADD_NEW_UNION_01_20041006
extern BOOL gObjCheckRival( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj );
extern INT	gObjGetRelationShip( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj );
extern INT	gObjGetGuildUnionNumber( LPOBJECTSTRUCT lpObj );
extern void gObjGetGuildUnionName( LPOBJECTSTRUCT lpObj, CHAR* szUnionName, INT iUnionNameLen = MAX_GUILDNAMESTRING);
extern void gObjNotifyUpdateUnionV1( LPOBJECTSTRUCT lpObj );
extern void gObjNotifyUpdateUnionV2( LPOBJECTSTRUCT lpObj );
extern void gObjUnionUpdateProc( INT aIndex );
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
// Kill Count 설정
extern void gObjSetKillCount( INT aIndex, INT iOption = KILLCOUNT_INC);

#endif

#ifdef ADD_NEW_VIEWPORT_SYSTEM_01_20041201
extern void gObjNotifyUseWeaponV1( LPOBJECTSTRUCT lpOwnerObj, LPOBJECTSTRUCT lpWeaponObj, INT iTargetX, INT iTargetY  );
extern void gObjNotifyUseWeaponDamage( LPOBJECTSTRUCT lpWeaponObj, INT iTargetX, INT iTargetY );

#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
extern void gObjUseBlessAndSoulPotion( INT aIndex, INT iItemLevel );

#endif


extern void gObjStateSetCreate(int aIndex);

extern void gObjViewportPaint(HWND hWnd, short aIndex);

extern void ObjectMapJoinPositionSend(short aIndex);

extern void gObjViewportListProtocol(short aIndex);


extern int  gObjGetUserNumber(int aIndex);
extern int  gObjGetNumber(int aIndex, int & UN, int & DBN);


#ifdef BUGFIX_GAIN_EXP_20071210
extern bool gObjLevelUp(LPOBJECTSTRUCT lpObj, INT64 &addexp, int iMonsterType, int iEventType);
#else
	#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	extern bool gObjLevelUp(LPOBJECTSTRUCT lpObj, int addexp, int iMonsterType, int iEventType);
	#else
		#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
		extern bool gObjLevelUp(LPOBJECTSTRUCT lpObj, int addexp);
		#else
		extern void gObjLevelUp(LPOBJECTSTRUCT lpObj, int addexp);
		#endif
	#endif
#endif
extern int  gObjLevelUpPointAdd(BYTE type, LPOBJECTSTRUCT lpObj);

extern BYTE LevelSmallConvert(int aIndex, int inventoryindex);
extern BYTE LevelSmallConvert(int level);
extern BYTE gObjInventoryInsertItem(int aIndex, int item_num);
extern BYTE gObjInventoryInsertItem(int aIndex, CItem item);
extern BYTE gObjInventoryInsertItem(int aIndex, CMapItem * item);
#ifdef PERSONAL_SHOP_20040113		
extern BYTE gObjOnlyInventoryInsertItem(int aIndex, CItem item);	// 오직 실제 인벤영역 (개인상점 영역 제외) 에 아이템을 넣으려고 할 때 사용
#endif
extern BYTE gObjInventoryInsertItemTemp(LPOBJECTSTRUCT lpObj, CMapItem *Item);
extern BYTE gObjShopBuyInventoryInsertItem(int aIndex, CItem item);
extern BYTE gObjShopBuyInventoryInsertItemSerial(int aIndex, CItem item);

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
extern BYTE gObjShopBuyInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level, int iSerial, int iDur);
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_HP_POTION_LEVEL1_FIX_20040420
	extern BOOL gObjSearchItem(LPOBJECTSTRUCT lpObj, int item_type, int add_dur, int nLevel);	
#else
	extern BOOL gObjSearchItem(LPOBJECTSTRUCT lpObj, int item_type, int add_dur);
#endif

#ifdef MODIFY_OVERLAP_ITEM_DURATION_20070730
extern BOOL gObjOverlapItemDuration(LPOBJECTSTRUCT lpObj, int iItemCode, int iLevel, int iAddDuration, int iMaxDuration);
#endif // MODIFY_OVERLAP_ITEM_DURATION_20070730

extern BOOL gObjSearchItemMinus(LPOBJECTSTRUCT lpObj, int pos, int m_dur);

//#ifdef EVENT_LOTTO
extern BYTE gObjInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level, int iSerial, int iDur);
//#endif

extern BYTE gObjInventoryDeleteItem(int aIndex, int itempos);
extern void gObjInventoryItemSet(int aIndex, int itempos, BYTE set_byte);
extern void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
extern void gObjMakePreviewCharSet(int aIndex);

#ifdef PERSONAL_SHOP_20040113		
extern void gObjInventoryItemSet_PShop(int aIndex, int itempos, BYTE set_byte);
extern void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
#endif

extern void gObjAddMsgSend(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode=0);
extern void gObjAddMsgSendDelay(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode=0);

#ifdef ADD_QPROCESSING_STATE_OF_DEAD_20050221
// 강제로 메시지 큐의 엘리먼트에 추가한다.
extern void gObjAddMsgSendDelayInSpecificQPos( LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition = 0 );
#endif

extern void InitFrustrum();
extern void CreateFrustrum(int x,int y, int aIndex);

extern void gObjAllDel();

extern BOOL gObjAttack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int magicsend, BYTE MSBFlag, int AttackDamage=0, BOOL bCombo = FALSE);

extern void gObjMsgProc(LPOBJECTSTRUCT lpObj);
extern BOOL gObjBackSpring(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
extern BYTE GetPathPacketDirPos(int px, int py);
extern int  gObjCalDistance(LPOBJECTSTRUCT lpObj1, LPOBJECTSTRUCT lpObj2);


extern void gObjTeleportMagicUse(int aIndex, BYTE x, BYTE y);
extern int  gObjMagicManaUse(LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic);

#ifdef NEW_FORSKYLAND2
extern int  gObjMagicBPUse(LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic);
#endif

extern void GetMoveArea(short &x, short &y, BYTE MapNumber, BYTE x1, BYTE y1, BYTE x2, BYTE y2, BYTE & dir);

extern BOOL gObjPosMagicAttack(LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic, BYTE x, BYTE y);
extern BOOL gObjMovePlayer(int aIndex, BYTE mapnumber, BYTE x1, BYTE y1, BYTE x2, BYTE y2);
extern BOOL gObjMoveGate(int aIndex, int gatenumber);
extern void gObjMoveDataLoadingOK(int aIndex);
extern void gObjTeleport(int aIndex, int map, int x, int y);

extern int gObjMagicAdd(LPOBJECTSTRUCT lpObj, BYTE Type, BYTE Index, BYTE Level, BYTE & SkillNumber);

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// gObjMagicAdd - Skill Type 변경
extern int gObjMagicAdd(LPOBJECTSTRUCT lpObj, INT aSkill, BYTE Level);
#else
extern int gObjMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
#endif

extern CMagicInf *gObjGetMagic(LPOBJECTSTRUCT lpObj, int mIndex);
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
extern CMagicInf *gObjGetMagicSearch(LPOBJECTSTRUCT lpObj, int skillnumber);
#else
extern CMagicInf *gObjGetMagicSearch(LPOBJECTSTRUCT lpObj, BYTE skillnumber);
#endif


extern void MonsterAndMsgProc();
extern void MoveMonsterProc();


extern void gObjTradeCancel(int aIndex);
extern void gObjTradeOkButton(int aIndex);

extern BYTE gObjInventoryMoveItem(int aIndex, BYTE source, BYTE target, int & durSsend, int & durTsend, BYTE sFlag, BYTE tFlag, LPBYTE siteminfo );
extern void gObjInventory2MoveItem(int aIndex, BYTE source, BYTE target);
extern BYTE gObjTradeInventoryMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target);
extern BYTE gObjTradeTradeMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target);
extern BYTE gObjInventoryTradeMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target);

extern int gObjInventoryTrans(int aIndex);
extern int gObjInventoryCommit(int aIndex);

extern BOOL gObjItemLevelUp(LPOBJECTSTRUCT lpObj, int source, int target);
extern BOOL gObjItemRandomLevelUp(LPOBJECTSTRUCT lpObj, int source, int target);
extern BOOL gObjItemRandomOption3Up(LPOBJECTSTRUCT lpObj, int source, int target);


extern void gObjUseDrink(LPOBJECTSTRUCT lpObj, int level = 0);
extern void gObjAbilityReSet(LPOBJECTSTRUCT lpObj);

extern DWORD gGetItemNumber();
extern void gPlusItemNumber();


extern void gObjAllDisconnect();
extern void gObjAllLogOut();
extern void gObjCloseSet(int aIndex, int Flag);

extern BOOL gObjCheckViewport(int aIndex, int x, int y);
extern BOOL gObjPositionCheck(LPOBJECTSTRUCT lpObj);
extern BOOL gObjCheckTileArea(int aIndex, int x, int y, int dis);

extern int  gObjGuildMasterCapacityTest(LPOBJECTSTRUCT lpObj);

#ifdef _NEW_EXDB_
//------------------------------------------------------------------------------------
// 길드..
extern void gObjGuildWarEndSend(LPOBJECTSTRUCT lpObj, BYTE Result1, BYTE Result2);
extern void gObjGuildWarEndSend(LPGUILD_INFO lpGuild1, LPGUILD_INFO lpGuild2, BYTE Result1, BYTE Result2);
extern int  gObjGuildWarProc(LPGUILD_INFO lpGuild1, LPGUILD_INFO lpGuild2, int score);
extern void gObjGuildWarEnd(LPGUILD_INFO lpGuild, LPGUILD_INFO lpTargetGuild);
//------------------------------------------------------------------------------------
#endif

extern BOOL gObjGetPkTime(LPOBJECTSTRUCT lpObj, int & hour, int & min);
extern BOOL gObjInventorySearchSerialNumber(LPOBJECTSTRUCT lpObj, DWORD serial);
extern void gObjSetInventory1Pointer(LPOBJECTSTRUCT lpObj);

extern void gObjStatTextSave(LPOBJECTSTRUCT lpObj);
extern void gObjMagicTextSave(LPOBJECTSTRUCT lpObj);
extern void gObjItemTextSave(LPOBJECTSTRUCT lpObj);
extern void gObjWarehouseTextSave(LPOBJECTSTRUCT lpObj);

extern BYTE gObjWarehouseInsertItemPos(int aIndex, CItem item, int pos, int source);
extern void gObjWarehouseItemSet(int aIndex, int itempos, BYTE set_byte);

extern BOOL gObjMonsterCall(int aIndex, int MonsterType, int x, int y);
extern void gObjMonsterCallKill(int aIndex);
extern BOOL gObjBackSpring2(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int count);


// 옵션 설정 부분
extern void gObjSetTradeOption(int aIndex, int option);
#ifdef DUEL_SYSTEM_20031028
extern void gObjSetDuelOption(int aIndex, int option);
extern bool IsDuelEnable(int aIndex);
extern bool IsOnDuel (int aIndex1, int aIndex2);
#endif

//#HAPPY_CAT
extern void	gObjClearViewportOfMine(LPOBJECTSTRUCT lpObj);
//HAPPY_CAT#

extern void gObjClearViewport(LPOBJECTSTRUCT lpObj);
extern void gObjViewportListProtocolDestroy(LPOBJECTSTRUCT lpObj);
extern void gObjViewportListProtocolCreate(LPOBJECTSTRUCT lpObj);

extern int GetMapMoveLevel(LPOBJECTSTRUCT lpObj, int mapnumber, int max_over);

extern BYTE gObjChaosBoxInsertItemPos(int aIndex, CItem item, int pos, int source);
extern void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte);

extern BOOL gObjCanItemTouch(LPOBJECTSTRUCT lpObj, int type);
extern void ItemIsBufExOption(LPBYTE buf, CItem * lpItem);

extern BOOL	gObjCheckTeleportArea(int aIndex, BYTE x, BYTE y);
extern BOOL	gObjCheckAttackArea(int aIndex, int TarObjIndex);

extern bool gObjIsSelfDefense (LPOBJECTSTRUCT lpObj, int aTargetIndex);


#ifdef DUEL_SYSTEM_20031028
// 특정 사용자가 결투중인지 체크
extern BOOL gObjDuelCheck(LPOBJECTSTRUCT lpObj);
// 서로 결투중인지 체크
extern BOOL gObjDuelCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
// 결투상태 초기화
extern VOID gObjResetDuel(LPOBJECTSTRUCT lpObj);
// 결투가 종료되었다는 패킷을 보내줌
extern VOID gObjSendDuelEnd(LPOBJECTSTRUCT lpObj);
#endif

#ifdef DUEL_SYSTEM_FIX_20031224
extern VOID gObjCheckAllUserDuelStop ();		// 10초에 한번씩 결투중인데 1분넘게 싸우지 않은 사람이 있는지 확인 -> 있으면 결투해제
#endif

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
extern VOID	gObjSendTimeView(int aIndex);
#endif

//------------->
//20080930 해외 5차 부분유료화 부활, 이동 부적 (grooving)
#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
extern	BOOL	gIsMoveCharmEnable(int iMapNum);
#endif //ADD_TELEPORT_CHARM_ITEM_20080930
//<-------------
// 악마의 광장 초대궈이 있는지 확인
extern BOOL	gUserFindDevilSquareInvitation(int aIndex);
// 악마의 열쇠, 눈이 있는지 확인
extern BOOL	gUserFindDevilSquareKeyEyes(int aIndex);

// 특정 맵의 인원들을 대상맵으로 이동 
extern void	gObjBatchMoveGate(BYTE mapnumber, BYTE targetmap);

extern void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel);

extern LPOBJECTSTRUCT	 gObjFind(char* targetcharname);

// 열개의 이벤트칩을 찾는다
extern int		gObjFind10EventChip(int aIndex);
extern int		gObjDelete10EventChip(int aIndex);

// 위치를 강제로 고정시킨다
void gObjSetPosition(int aIndex, int x, int y);

// 인벤토리안의 특정 아이템 수를 얻는다

#ifdef SECOND_QUEST
extern int		gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel = -1);
#else
extern int		gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex);
#endif
extern int		gObjGetItemCountInIventory(int aIndex, int itemnum);

extern int		gObjGetManaItemPos(int aIndex);

// 인벤토리안의 특정 아이템를 수만큼 삭제 한다
extern void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count);

// 스탯포인트를 올릴수 있는지
extern int	gObjCheckStatPoint(int aIndex);

// 열매를 사용한다
extern void	gObjUseCircle(int aIndex, int pos);

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
//  - 0) 스탯생성
extern void gObjUsePlusStatFruit(int aIndex, int pos);
//  - 1) 스탯 감소
extern void gObjUseMinusStatFruit(int aIndex, int pos);
#endif

#ifdef ADD_PCS_FRUITS_POINT_EXTENTION_20070430
extern void gObjUseExMinusFruit(LPOBJECTSTRUCT lpObj, int iPos);
#endif // ADD_PCS_FRUITS_POINT_EXTENTION_20070430

#ifdef ADD_PCS_IMPROVE_POTION_20070718
extern void gObjUseImprovePotion(LPOBJECTSTRUCT lpObj, int iPos);
#endif // ADD_PCS_IMPROVE_POTION_20070718

#ifdef ADD_PCS_INDULGENCE_20070504
extern void gObjUseIndulgence(LPOBJECTSTRUCT lpObj, int iPos);
#endif // ADD_PCS_INDULGENCE_20070504

#ifdef ADD_FULL_RECOVERY_POTION_20070719
extern void gObjUseFullRecoveryPotion(LPOBJECTSTRUCT lpObj, int iPos);
#endif // ADD_FULL_RECOVERY_POTION_20070719

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
void gObjUseRecoveryPotion(LPOBJECTSTRUCT lpObj, int iPos, double dRate = 1);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

#ifdef NEW_FORSKYLAND2
extern void	gObjSetBP(int aIndex);
#endif

extern void gObjCalcMaxLifePower(int aIndex);

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
extern void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint);
#else
extern void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint);
#endif

extern void gObjPenaltyDurDown(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
extern void gObjWeaponDurDown(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int type);
extern void gObjArmorRandomDurDown(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpAttackObj);
#ifdef _NEW_EXDB_
extern BOOL gObjTargetGuildWarCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
#endif
extern void gObjCheckSelfDefense(LPOBJECTSTRUCT lpObj, int aTargetIndex);

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
extern void gObjWeaponDurDownInCastle( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, INT iDecValue );

#endif



extern BOOL gObjAttackQ(LPOBJECTSTRUCT lpObj);

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage=0);
#else

#ifdef ADD_SHIELD_POINT_01_20060403
	#ifdef ADD_SKILL_WITH_COMBO
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill, int iShieldDamage=0);
	#else
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, int iShieldDamage=0);
	#endif
#else
	#ifdef ADD_SKILL_WITH_COMBO
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill);
	#else
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage);
	#endif
#endif // ADD_SHIELD_POINT_01_20060403
#endif // ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107


extern BOOL retResistance(LPOBJECTSTRUCT lpObj, int Resistance_Type);

// 스킬 저항력
extern BOOL retCalcSkillResistance(LPOBJECTSTRUCT lpTargetObj, BOOL isDouble = FALSE);

extern void gObjSpriteDamage(LPOBJECTSTRUCT lpObj, int damage);
extern BOOL gObjSatanSprite(LPOBJECTSTRUCT lpObj);
extern BOOL gObjAngelSprite(LPOBJECTSTRUCT lpObj);
extern BOOL gObjWingSprite(LPOBJECTSTRUCT lpObj);
extern BOOL gObjUniriaSprite(LPOBJECTSTRUCT lpObj);

#ifdef	ADD_SAFEGUARD_ITEM_20080702
extern BOOL gObjDaemonSprite(LPOBJECTSTRUCT lpObj);
extern BOOL gObjSafeGuardSprite(LPOBJECTSTRUCT lpObj);
#endif // ADD_SAFEGUARD_ITEM_20080702

extern void gObjNextExpCal(LPOBJECTSTRUCT lpObj);

#ifdef NEW_FORSKYLAND2
extern void	gObjDelayLifeCheck(int aIndex);
#endif


void SkillFrustrum(BYTE bangle, int aIndex);
BOOL SkillTestFrustrum(int x,int y, int aIndex);

#ifdef RING_EVENT_ITEMBOX_20031124		// 반지이벤트 - 장착한 장비중에서 해당 아이템의 개수를 파악
int	 gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel);
#endif

#ifdef MODIFY_ZEN_MAX_20040414
// MAX_ZEN을 넘었는가?
extern BOOL gObjCheckMaxZen( int aIndex, int nAddZen );
#endif

#ifdef CHINA_GM_EVENT
extern BOOL	gGMEventRunning;
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
extern BOOL	gGMEventRunning;
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

extern BOOL gObjIsItemPut(LPOBJECTSTRUCT lpObj, CItem *lpItem, int pos);

#if GAME_VERSION >= G_V_99B
extern void MakeRandomSetItem(int aIndex);
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
extern BOOL	gObjMagicEnergyCheck(LPOBJECTSTRUCT lpObj, WORD aSkill);	// 마법을 사용할때 에너지 체크를 한다
#else
extern BOOL	gObjMagicEnergyCheck(LPOBJECTSTRUCT lpObj, BYTE aSkill);	// 마법을 사용할때 에너지 체크를 한다
#endif


#ifdef DARKLORD_WORK	
extern void	gObjRecall(int aIndex, int mapnumber, int x, int y);
extern void	gObjSetExpPetItem(int aIndex, int exp);
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// 아이템 복사 방지용 함수들
extern bool gObjFixInventoryPointer(int aIndex);
#endif

#ifdef ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907
// Reward Type = 1 : 쿤둔에 대한 보상
#ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
extern void MakeRewardSetItem(int aIndex, UCHAR cDropX=0, UCHAR cDropY=0, int iRewardType=0, int iMapnumber = 0);
#else
extern void MakeRewardSetItem(int aIndex, UCHAR cDropX=0, UCHAR cDropY=0, int iRewardType=0);
#endif	// #ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
#endif

#if TESTSERVER == 1
void	DebugOutput(int aIndex, char* msg, ...);
#endif
void	MsgOutput(int aIndex, char* msg, ...);

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
extern void gProcessAutoRecuperation(LPOBJECTSTRUCT lpObj);
#endif

#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518
// 특정맵의 cX, cY 근처 지역 중에 빈자리를 하나 찾아낸다.
extern BOOL	gObjGetRandomFreeLocation (	INT	iMapNumber,	UCHAR & cX,	UCHAR & cY, INT iRangeX, INT iRangeY, INT iLoopCount);
// 해당 케릭 (사용자)의 공격이 마법공격인지 밝혀낸다.
extern INT	gObjCheckAttackTypeMagic ( INT iClass, INT iSkill);
#endif

#ifdef CASTLEDEEP_EVENT_20050413
extern BOOL gObjGetRandomFreeArea (INT	iMapNumber,	UCHAR & cX,	UCHAR & cY, INT iSX, INT iSY, INT iDX, INT iDY, INT iLoopCount);
#endif

#ifdef ADD_MONSTER_DIE_REWARD_ITEMS_20040915
extern BOOL gObjGetRandomItemDropLocation ( INT	iMapNumber,	UCHAR & cX,	UCHAR & cY, INT iRangeX, INT iRangeY, INT iLoopCount);
#endif

#ifdef	HAPPY_CAT
extern void	gObjCheckTimeOutValue(LPOBJECTSTRUCT lpObj, DWORD & rNowTick);	// happycat@20050201
extern void	gObjReady4Relife(LPOBJECTSTRUCT lpObj);							// happycat@20050201
#endif //HAPPY_CAT

#ifdef MODIFY_SEASON_01_20060317
	#ifndef HAPPY_CAT
	extern void	gObjCheckTimeOutValue(LPOBJECTSTRUCT lpObj, DWORD & rNowTick);	// happycat@20050201
	extern void	gObjReady4Relife(LPOBJECTSTRUCT lpObj);							// happycat@20050201
	#endif // HAPPY_CAT
#endif // MODIFY_SEASON_01_20060317

#ifdef PCBANG_ADVANTAGE
extern BOOL	IsOKPCBangBenefit(LPOBJECTSTRUCT lpObj);						// happycat@20050201
#endif

// PC방 혜택 여부를 맵 등 제약 조건에 상관없이 판단한다.
#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
extern BOOL	IsOKPCBangBenefitAll(LPOBJECTSTRUCT lpObj);
#endif

#ifdef FOR_TAIWAN
// 대만은 주민 번호 체크가 틀리다.
extern BOOL gObjTaiwanJoominCheck(int aIndex, char *szInJN);
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
extern int gObjCheckOverlapItemUsingDur( int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel );
extern int gObjOverlapItemUsingDur( CItem *lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel );
#endif

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202
// 특정 아이템 시리얼 0 이 있을 경우 FALSE 를 반환한다.
extern BOOL gObjCheckInventorySerial0Item(LPOBJECTSTRUCT lpObj);

extern BOOL gObjCheckSerial0ItemList( CItem *lpItem );

#endif // ANTIHACKING_SERIAL_0_ITEM_20051202

// 스킬 공격 시, 가능한 공격 위치인지 확인
#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413

extern BOOL gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum);

#endif //MODIFY_SKILL_CHECK_DISTANCE_20060413

#ifdef MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328
extern void gObjSaveChaosBoxItemList(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
extern void gStatReset( int iIndex );
#endif //EXPERIENCE_SERVER_NPC_STAT_RESET

#ifdef ADD_GAMBLING_20090120
extern BOOL gObjGamblingInventoryCheck( int aIndex, int iWidth, int iHeight );
#endif // ADD_GAMBLING_20090120

extern void gObjMagicAddEnergyCheckOnOff(int flag);

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
extern int gObjWeaponMagicAdd(LPOBJECTSTRUCT lpObj, WORD aSkill, BYTE Level);
#else
extern int gObjWeaponMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
#endif