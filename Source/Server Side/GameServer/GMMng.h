// GMMng.h: interface for the CGMMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMMNG_H__53E6F8F0_D7DE_48E7_A7AD_E75FB75257D4__INCLUDED_)
#define AFX_GMMNG_H__53E6F8F0_D7DE_48E7_A7AD_E75FB75257D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#ifdef GMCOMMAND_EX
#define MASK_GMCMDEX				0x01F4	//500
#endif // GMCOMMAND_EX

#ifdef MODIFY_MAX_GMCMD_EXTEND_20080630
	#if TESTSERVER == 1
		#define MAX_GMCOMMAND		500
	#else
		#define MAX_GMCOMMAND		450
	#endif // TESTSERVER == 1
#else
	#if TESTSERVER == 1 
		#ifdef JS_ADD_TEST_COMMAND
			// 20060719 100 -> 150 변경 (apple)
			#define MAX_GMCOMMAND		150
		#endif
	#else
		#ifdef ADD_MACRO_FOR_SETITEM_20060424
			// 20060719 60 -> 100 변경 (apple)
			#define MAX_GMCOMMAND		100
		#else
			// 20060719 50 -> 100 변경 (apple)
			#define MAX_GMCOMMAND		100
		#endif
	#endif
#endif // MODIFY_MAX_GMCMD_EXTEND_20080630

#define GMCMD_DISCONNECT			100		// 강제 접속 종료
#define GMCMD_SETPOSITION			101		// 강제 위치 이동  
#define GMCMD_COPYCHAR				102		// 캐릭터 변신&복제
#define GMCMD_ACCOUNTBLOCK			103		// 계정 블럭
#define GMCMD_CHATBAN				104		// 채팅 금지
#define GMCMD_ACCOUNTBLOCKFREE		105		// 계정 블럭 해지
#define GMCMD_CHATBANFREE			106		// 채팅 금지 해지
#define GMCMD_ITEMCREATE			107		// 아이템 생성
#define GMCMD_GUILDSETPOSITION		108		// 길드 강제 위치 이동  
#define GMCMD_GUILDWARSTART			109		// 전투 시작을 알린다.
#define GMCMD_GUILDWARSTOP			110		// 전투 잠시 중단을 알린다.
#define GMCMD_GUILDWAREND			111		// 전투 종료를 알린다.
#define GMCMD_GUILDDISCONNECT		112		// 길드 종료를 알린다.
#define GMCMD_SETPOSITION2			113

#define UMCMD_GUILDWAR				200		// 길드 전쟁
#define UMCMD_BILLCHECK				201		// 남은시간 확인
#define UMCMD_BATTLESOCCERWAR		202		// 전투축구 
#define UMCMD_TRADEOPTION			203		// 요청 on/off

#define GMCMD_USERSTAT				214		// 접속현황
#define GMCMD_USERTRAKING			215		// 추적
#define GMCMD_USERWATCHNG			216		// 감시

#define GMCMD_FIRECRACK				217		// 폭죽

#define GMCMD_STATRESET				218		// 스탯 리셋
#define GMCMD_QUESTRESET			219		// 스탯 리셋


#define GMCMD_MANYMANYBLESS			230		// 축왕창
#define GMCMD_MANYMANYSOUL			231		// 영왕창
#define GMCMD_MAKESETITEM			232		// 영왕창

#define GMCMD_MAKERANDOMSETITEM		233		// 랜덤세트만들기

#define GMCMD_CHANGEPOS				220		// 좌표 이동

#ifdef CHINA_GM_EVENT	
#define GMCMD_START_GMEVENT			221
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
#define GMCMD_START_GMEVENT			221
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#ifdef JS_ADD_TEST_COMMAND
#define GMCMD_MANYMANYCHAOS			301
#define GMCMD_MANYMANYLIFE			302
#define GMCMD_MANYMANYCREATE		303
#define GMCMD_MANYMANYGEM			304
#define GMCMD_MODIFYSTAT			305
#define GMCMD_INFORMITEMINDEX		308
#define GMCMD_NOTICEALL				309
#define GMCMD_RESETZEN				310
#define GMCMD_MANYMANYITEM			311

#define GMCMD_CREATEPROPERTYITEM	313
#define GMCMD_SETPKLEVEL			314
#define GMCMD_SETPKPOINT			315

#define GMCMD_CREATEITEMNAME		318
#define GMCMD_NOTIFYMONSTERHP		319
#define GMCMD_SET1004				330		// PK 정보 초기화
#define GMCMD_MODIFYSTAT_ENG		339		// 스탯설정.무제한.
#define GMCMD_CLASS_UP				341		// 전직
#define GMCMD_FENRIR_DURSMALL_DEF	342		// 펜릴 내구도 감소치 기본 DurSmall
#define GMCMD_FENRIR_DURSMALL_ELF	343		// 팬릴 내구도 감소치 요정 DurSmall
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
#define GMCMD_SET_SHIELD_POINT		349		// 쉴드 수치 설정
#define GMCMD_POINT_DISPLAY			350		// 쉴드 수치 표시
#endif

#ifdef ITEM_ADD_OPTION_20061019
#define GMCMD_ITEM_EFFECT_CLEAR		351		// 아이템 효과 초기화
#define GMCMD_ITEM_EFFECT_SET		352		// 아이템 효과 설정
#endif


#define GMCMD_SETLEVEL				312		// 레벨설정

#ifdef DARKLORD_WORK
#define GMCMD_DARKSPIRIT			316		// 다크스피릿 명령
#define GMCMD_DARKSPIRIT_LEVEL		317		// 다크스피릿 명령
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
#define GMCMD_NOTIFYKUNDUNHP		320		// 쿤둔 피보기
#define GMCMD_SETKUNDUNHP			321		// 쿤둔 피설정
#define GMCMD_SETKUNDUNREFILLHP		322		// 쿤둔 피회복량설정
#define GMCMD_SETKUNDUNREFILLHPSEC	323		// 쿤둔 초당피회복량설정
#define GMCMD_SETKUNDUNREFILLHPTIME	324		// 쿤둔 피회복 시간 설정
#endif

#ifdef CASTLE_GM_COMMAND_20041222	// 운영자 공성제어 명령추가
#define GMCMD_CS_CHANGEOWNERGUILD			331		// "/공성수성길드"
#define GMCMD_CS_SETSTATE_REGSIEGE			332		// "/공성참여등록"
#define GMCMD_CS_SETSTATE_REGMARK			333		// "/공성문장등록"
#define GMCMD_CS_SETSTATE_NOTIFY			334		// "/공성선포"
#define GMCMD_CS_SETSTATE_STARTSIEGE		335		// "/공성시작"
#define GMCMD_CS_SETSTATE_ENDSIEGE			336		// "/공성종료"
#define GMCMD_CS_SETSTATE_GETOWNERGUILD		337		// "/공성현재수성"
#define GMCMD_CS_SETSTATE_GETCASTLESTATE	338		// "/공성현재상태"
#define GMCMD_CS_CHANGECSSIDE				340		// "/공수교체"
#endif

#ifdef CRYWOLF_GM_COMMAND_20050525	// 크라이울프 명령어
#define GMCMD_CRYWOLF_READY		345	// "/크라이울프준비"
#define	GMCMD_CRYWOLF_START		346	// "/크라이울프시작"
#define GMCMD_CRYWOLF_END		347 // "/크라이울프종료"
#define GMCMD_CRYWOLF_NOTIFY_2	348 // "/크라이울프연출"
#endif

#ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410	// 인피니티 애로우 관련 명령어	
#define GMCMD_GETINFINITYARROW_MP		360	// "/현재인피니태애로우"
#define GMCMD_SETINFINITYARROW_MP_0		361	// "/인피니티애로우+0"
#define GMCMD_SETINFINITYARROW_MP_1		362	// "/인피니티애로우+1"
#define GMCMD_SETINFINITYARROW_MP_2		363	// "/인피니티애로우+2"
#define GMCMD_INFINITYARROW_SETTIME		364	// "/인피티니애로우시간"
#endif

#ifdef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018
#define GMCMD_LEVELUP_TEST	365
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// 파이어 스크림 관련 명령어
#define GMCMD_FIRE_SCREAM				366	// "/폭발거리"
#endif

	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530	// 조화보석 옵션 테스트
		#define GMCMD_MAKE_STRENGTHENITEM 367
	#endif

	#ifdef ADD_380ITEM_NEWOPTION_20060711	// 380 아이템 옵션 테스트
		#define GMCMD_MAKE_380OPTIONITEM 368
	#endif

#ifdef KANTURU_GM_COMMAND_20060709
#define GMCMD_KANTURU_STANBY						369	// "/칸투르대기상태"
#define GMCMD_KANTURU_BATTLEOFMAYA					370	// "/칸투르마야전"
#define GMCMD_KANTURU_MAYA_MONSTER1					371	// "/마야몬스터1"
#define GMCMD_KANTURU_MAYA1							372	// "/마야1"
#define GMCMD_KANTURU_MAYA_MONSTER2					373	// "/마야몬스터2"
#define GMCMD_KANTURU_MAYA2							374	// "/마야2"
#define GMCMD_KANTURU_MAYA_MONSTER3					375	// "/마야몬스터3"
#define GMCMD_KANTURU_MAYA3							376	// "/마야3"
#define GMCMD_KANTURU_MAYA3_SUCCESS					377	// "/마야3성공"
#define GMCMD_KANTURU_MAYA_SUCCESS					378	// "/마야성공"
#define GMCMD_KANTURU_MAYA_FAIL						379	// "/마야실패"
#define GMCMD_KANTURU_BATTLEOFNIGHTMARE				380	// "/칸투르나이트메어전"
#define GMCMD_KANTURU_BATTLEOFNIGHTMARE_SUCCESS		381	// "/나이트메어성공"
#define GMCMD_KANTURU_BATTLEOFNIGHTMARE_FAIL		382	// "/나이트메어성공"
#define GMCMD_KANTURU_TOWEROFREFINEMENT				383	// "/칸투르정제의탑"
#define GMCMD_KANTURU_TOWER_OPEN					384	// "/정제의탑열기"
#define GMCMD_KANTURU_TOWER_CLOSE					385	// "/정제의탑닫기"
#define GMCMD_KANTURU_END							386	// "/칸투르종료"
#define GMCMD_KANTURU_NEXT_STATE					387	// "/칸투르다음상태"
#define GMCMD_KANTURU_NEXT_DETAIL_STATE				388	// "/칸투르다음세부상태"
#define GMCMD_KANTURU_CURRENT_STATE					389	// "/칸투르현재상태"
#endif	// #ifdef KANTURU_GM_COMMAND_20060709

#ifdef UPDATE_GM_FUNCTION_20070228		// "/투명" "/투명해제" 인덱스 정의
#define GMCMD_INVISIBLE								390 // "/투명"
#define GMCMD_REMOVE_INVISIBLE						391 // "/투명해제"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영 사원 이벤트 메크로
#define GMCMD_ILLUSION_TEMPLE_READY					392	// 이벤트 준비
#define GMCMD_ILLUSION_TEMPLE_START					393	// 이벤트 시작
#define GMCMD_ILLUSION_TEMPLE_END					394	// 이벤트 종료
#define GMCMD_ILLUSION_TEMPLE_RESET					395 // 이벤트 정보 리셋
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// /전직3 명령어 추가
#define GMCMD_THIRD_CHANGE_UP						396	// "/전직3"
#define GMCMD_CHANGE_UP_ALL_RESET					397	// "/전직리셋"
#define GMCMD_QUEST_ITEM							398 // "/퀘템 (숫자)"
#define GMCMD_MANYMANY_ITEM							399	// "/템왕창 (타입) (인덱스) (개수)"
#define GMCMD_SET_QUEST								400	// "/퀘성공 (숫자)"
#define GMCMD_SET_ZEN								401	// "/젠 (숫자)"
#endif

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	#define GMCMD_LOTTERY_ITEM_CREATE				402
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef MODIFY_INVEN_CLEAR_MACRO_20071212
	#define GMCMD_INVENTORY_ITEM_DELETE				403
	#define GMCMD_INVENTORY_ITEM_ALL_DELETE			404
	#define GMCMD_DEVILSQUARE_SET_TIME				405
#endif // MODIFY_INVEN_CLEAR_MACRO_20071212

#ifdef ADD_MASTER_LEVEL_SYSTEM_COMMAND_20071122
#define GMCMD_MASTERLEVEL_LEVELUP					406	// "/masterlevel (숫자)"
#define GMCMD_MASTERLEVEL_POINT						407	// "/mlpoint (숫자)"
#define GMCMD_MASTERLEVEL_SKILL_RESET				408	// "/ms_reset"
#endif

#ifdef ADD_RAKLION_20080408
#define GMCMD_RAKLION_IDLE							409 // "/라클리온대기"
#define GMCMD_RAKLION_NOTIFY_1						410 // "/라클리온공지1"
#define GMCMD_RAKLION_STANDBY						411 // "/라클리온준비"
#define GMCMD_RAKLION_NOTIFY_2						412 // "/라클리온공지2"
#define GMCMD_RAKLION_READY							413 // "/라클리온공지2"
#define GMCMD_RAKLION_START_BATTLE					414 // "/라클리온시작"
#define GMCMD_RAKLION_NOTIFY_3						415 // "/라클리온공지3"
#define GMCMD_RAKLION_CLOSE_DOOR					416 // "/라클리온폐쇄"
#define GMCMD_RAKLION_ALL_USER_DIE					417 // "/라클리온실패"
#define GMCMD_RAKLION_NOTIFY_4						418 // "/라클리온공지4"
#define GMCMD_RAKLION_END							419 // "/라클리온종료"
#define GMCMD_RAKLION_SELUPAN_1						420 // "/라클리온종료"
#define GMCMD_RAKLION_SELUPAN_2						421 // "/라클리온종료"
#define GMCMD_RAKLION_SELUPAN_3						422 // "/라클리온종료"
#define GMCMD_RAKLION_SELUPAN_4						423 // "/라클리온종료"
#define GMCMD_RAKLION_SELUPAN_5						424 // "/라클리온종료"
#define GMCMD_RAKLION_SELUPAN_6						425 // "/라클리온종료"
#define GMCMD_RAKLION_SELUPAN_7						426 // "/라클리온종료"
#define GMCMD_RAKLION_FIRST_SKILL					427 // "/세루판최초스킬"
#define GMCMD_RAKLION_POISON_ATTACK					428 // "/세루판독공격"
#define GMCMD_RAKLION_ICE_STORM						429 // "/세루판냉기폭풍"
#define GMCMD_RAKLION_ICE_STRIKE					430	// "/세루판냉기충격"
#define GMCMD_RAKLION_SUMMON_MONSTER				431 // "/세루판소환"
#define GMCMD_RAKLION_HEAL							432 // "/세루판힐"
#define GMCMD_RAKLION_FREEZE						433 // "/세루판얼리기"
#define GMCMD_RAKLION_TELEPORT_RANDOM				434 // "/세루판순간이동"
#define GMCMD_RAKLION_INVINCIBILITY					435 // "/세루판무적화"
#endif // ADD_RAKLION_20080408

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#define GMCMD_SET_SOCKET_OPTION						436	// "/setsocket (아이템위치) 시드인덱스 스피어레벨 소켓위치
#define GMCMD_CLEAR_SOCKET_OPTION					437	// "/clearsocket (아이템위치) 소켓위치

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#define GMCMD_MAKE_SOCKET_SLOT						438	// "/makesocketslot (아이템위치) 소켓슬롯수
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710

#ifdef MODIFY_SPEEDHACK_20080605
#define GMCMD_ABILITY_DEBUG							439 // "/abilitydebug (1,0)"
#endif // MODIFY_SPEEDHACK_20080605
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#if TESTSERVER == 1 

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
#define GMCMD_XMASEVENT_START						440	// "/xmaseventstart (1,0)"
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#ifdef ADD_GIFTITEM_XMASEVENT_20081030
#define GMCMD_XMASEVENT_SETVISITORCNT				441
#define GMCMD_XMASEVENT_SETSANTAMODE				442
#endif //ADD_GIFTITEM_XMASEVENT_20081030

#endif //TESTSERVER

#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
#define GMCMD_SETINFINITYARROW_MP_3					443 // "/인피니티애로우+3"
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

#ifdef ADD_NEWPVP_PKFIELD
#define GMCMD_NEWPVP_OBSERVER						444	// "관전버프"
#define GMCMD_NEWPVP_GLORYOFGLADIATOR				445	// "/검명"
#endif // ADD_NEWPVP_PKFIELD

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
#define GMCMD_ADD_HACKTOOL_MOVE_BLOCK				446	// "/핵툴블록추가"
#define GMCMD_MODIFY_HACKTOOL_MOVE_BLOCK			447 // "/핵툴블록수정"
#define GMCMD_REMOVE_HACKTOOL_MOVE_BLOCK			448 // "/핵툴블록삭제"
#define GMCMD_STATISTICS_HACKTOOL					449	// "/핵툴통계"
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef PCBANG_POINT_SYSTEM_20070206
#define GMCMD_ADD_PCBANG_POINT						450 // "/addpcbangpoint 10"
#define GMCMD_RESET_PCBANG_POINT					451 // "/resetpcbangpoint"
#endif // PCBANG_POINT_SYSTEM_20070206

#define GMCMD_TIME									452
#define GMCMD_PKCLEAR								453
#define GMCMD_ONLINE								454
#define GMCMD_ADDSTR								455
#define GMCMD_ADDDEX								456
#define GMCMD_ADDVIT								457
#define GMCMD_ADDENE								458
#ifdef DARKLORD_WORK
#define GMCMD_ADDCOM								459
#endif
#define GMCMD_JOINPVP								460
#define GMCMD_RESETCOMMAND							461
#define GMCMD_VIPINFO								462
#define GMCMD_LIFE_POTION							463
#define GMCMD_MANA_POTION							464
#define GMCMD_BAN_USER								465
#define GMCMD_OFFAFK                                466
#define GMCMD_OFFTRADE                              467
#define GMCMD_OFFEXP                                468
#define GMCMD_DUELINFO                              469
#define GMCMD_ATTACK                                470
#define GMCMD_NOATTACK                              471
#define GMCMD_OFFCASH                               472
#define GMCMD_BOTSTORE                              473

#ifdef GMCOMMAND_EX
enum EnumCheatCode
{
	GMCMD_GETABILITY				= MASK_GMCMDEX | 1,		// "/상태 [캐릭터명]"
	GMCMD_PARTYINFO,										// "/파티 [캐릭터명]"
	GMCMD_SUMMONMONSTER,									// "/소환 [몬스터명]"
	GMCMD_CLEARMONSTER,										// "/몬스터제거"
	GMCMD_CLEARITEM,										// "/아이템제거"
	GMCMD_CLEARINVEN,										// "/인벤제거"
	GMCMD_SUMMONCHARACTER,									// "/캐릭터소환 [캐릭터명]"
};
#endif // GMCOMMAND_EX

#define MAX_MANAGER					5

class CGMCommand
{

public :

	char	szCmd[MAX_GMCOMMAND][20];
	int		nCmdCode[MAX_GMCOMMAND];
	DWORD	dwAuthority[MAX_GMCOMMAND];
	int  count;

	CGMCommand() 
	{ 
		Init();
	}

	void Init()
	{
		for( int n=0; n<MAX_GMCOMMAND; n++) nCmdCode[0] = -1; 
		count = 0;	
	}

	void Add(char * command_str, int command_code, DWORD dwInAuthority)
	{
		strcpy(szCmd[count], command_str);
		nCmdCode[count] = command_code;

		// Admin, EventGm 권한 통합 사용 20081215 (grooving)
#ifdef MODIFIY_AUTHORITY_INTERGRATION_20081215
		dwAuthority[count] = dwInAuthority | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM | AUTHORITY_SUPER_ADMIN;
#else // MODIFIY_AUTHORITY_INTERGRATION_20081215
		dwAuthority[count] = dwInAuthority | AUTHORITY_SUPER_ADMIN;
#endif // MODIFIY_AUTHORITY_INTERGRATION_20081215

		if(g_GlobalConfig.CheckCommand(command_code) == true)
		{
			dwAuthority[count] = g_GlobalConfig.GetCommandAuthority(command_code);
		}

		if( count < MAX_GMCOMMAND-1 ) count++;
	}
};

enum INCREASE_STAT_ENUM
{
	IS_STRENGTH,
	IS_DEXTERITY,
	IS_VITALITY,
	IS_ENERGY,
	IS_COMMAND
};

class CGMMng
{
public:
	
	CGMCommand	cCommand;
	

	CGMMng();
	virtual ~CGMMng();
	void Init();
	int GetCmd(char* szCmd);
	bool SearchCmd(char* szCmd, int &iOutCmd, DWORD &iOutAuthority);
	int ManagementProc(LPOBJECTSTRUCT lpObj, char* szCmd, int aIndex);
	char* GetTokenString();
	int   GetTokenNumber();
	int   GetTokenNumberEx();

	////////////////////////////////////////////////////////////////////////////
	
#ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410
	void GetInfinityArrowMPConsumption( LPOBJECTSTRUCT lpObj );
	void ControlInfinityArrowMPConsumption0( LPOBJECTSTRUCT lpObj, int iValue );
	void ControlInfinityArrowMPConsumption1( LPOBJECTSTRUCT lpObj, int iValue );
	void ControlInfinityArrowMPConsumption2( LPOBJECTSTRUCT lpObj, int iValue );
	void SetInfinityArrowTime( LPOBJECTSTRUCT lpObj, int iValue );
#endif

#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	void ControlInfinityArrowMPConsumption3( LPOBJECTSTRUCT lpObj, int iValue );
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	void ControlFireScreamDoubleAttackDistance( LPOBJECTSTRUCT lpObj, int iValue );
#endif

	////////////////////////////////////////////////////////////////////////////
	char szManagerName[MAX_MANAGER][MAX_IDSTRING+1];
	int  ManagerIndex[MAX_MANAGER];

	int	 WatchTargetIndex;

	void ManagerInit();
	int  ManagerAdd(char *name, int aIndex);
	void ManagerDel(char *name);
	void ManagerSendData(char *szMsg, int size);
	void BattleInfoSend(char *Name1, BYTE score1, char *Name2, BYTE score2);

	void	DataSend(LPBYTE szMsg, int size);
	////////////////////////////////////////////////////////////////////////////
#ifdef GMCOMMAND_EX
private:
	void	_GetUserAbility(LPOBJECTSTRUCT lpObj, LPCSTR lpszName);
	void	_GetPartyInfo(LPOBJECTSTRUCT lpObj, LPCSTR lpszName);
	void	_SummonMonster(LPOBJECTSTRUCT lpObj, LPCSTR lpszName, int nCount);
	void	_ClearMonster(LPOBJECTSTRUCT lpObj, int nDistance);
	void	_ClearItem(LPOBJECTSTRUCT lpObj, int nDistance);
	void	_ClearInven(LPOBJECTSTRUCT lpObj);
	void	_SummonCharacter(LPOBJECTSTRUCT lpObj, LPCSTR lpszName);

	int		_GetObjectType(WORD wClass);
#endif // GMCOMMAND_EX

	void	IncreaseStat(LPOBJECTSTRUCT lpObj, int Points, INCREASE_STAT_ENUM type);

public:
	void	ResetViewport(LPOBJECTSTRUCT lpObj);
};


extern CGMMng cManager;

#endif // !defined(AFX_GMMNG_H__53E6F8F0_D7DE_48E7_A7AD_E75FB75257D4__INCLUDED_)

