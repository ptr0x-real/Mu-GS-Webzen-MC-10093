// GMMng.cpp: implementation of the CGMMng class.
//
//////////////////////////////////////////////////////////////////////
// 운영자의 운영기능 처리를 담당한다.

#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include "..\\Include\\Prodef.h"
#include "..\\common\\winutil.h"
#include "..\\Common\\classdef.h"
#include "giocp.h"
#include "user.h"
#include "GMMng.h"
#include "Protocol.h"
#include "Sprotocol.h"
#include "Gamemain.h"
#include "wsGameServer.h"
#include "DSProtocol.h"
#include "QuestInfo.h"
#include "PartyClass.h"
#include "gObjMonster.h"
#include "OffExp.h"
extern PartyClass		gParty;

#ifdef ADD_MACRO_FOR_SETITEM_20060424
#include "SetItemMacro.h"
#endif

#include "BattleSoccerManager.h"


#ifdef ADD_MOVE_COMMAND_SCRIPT
#include "MoveCommand.h"
#endif

#include "..\\Common\\SetItemOption.h"

//#if (TESTSERVER == 1) || defined(FOR_TEMP_SERVER)
//	#ifdef JS_ADD_TEST_COMMAND
		#include "TNotice.h"
		extern ITEM_ATTRIBUTE   ItemAttribute[MAX_ITEM];
//	#endif
//#endif

#ifdef DARKLORD_WORK
#include "DarkSpirit.h"
#endif

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
#include "WTEventItemList.h"
#endif

#ifdef FOR_CASTLE_TESTSERVER
#include "CCastleEventItemList.h"
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
#include "LogToFile.H"
CLogToFile	KUNDUN_GM_LOG ("KUNDUN_EVENT_GM_LOG", ".\\KUNDUN_EVENT_GM_LOG");		// 로그파일 객체
#endif

#ifdef CASTLE_GM_COMMAND_20041222				// 운영자 공성제어 명령처리
#ifdef CASTLE_MAIN_SCHEDULER_20041111
#include "CastleSiege.h"
#endif
#endif

#ifdef CRYWOLF_GM_COMMAND_20050525	// 크라이울프 명령어
#include "Crywolf.h"
#endif

#ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410
#include "SkillAdditionInfo.h"	
#endif

#ifdef KANTURU_GM_COMMAND_20060709
#include "Kanturu.h"
#endif

#ifdef ADD_RAKLION_20080408
#include "Raklion.h"
#endif // ADD_RAKLION_20080408

#ifdef ITEM_ADD_OPTION_20061019
#include "ItemAddOption.h"
#endif

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	#include "EventItemBagExMng.h"
	extern CEventItemBagExMng g_EventItemBagExMng;
	#else
	#include "CashLotterySystem.h"
	extern CCashLotterySystem g_CashLotterySystem;
	#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

//#ifdef UPDATE_PK_SYSTEM_20070302
//#include "PartyClass.h"
//extern PartyClass		gParty;
//#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef MODIFY_SEASON25_PCS_UNIFICATION_20070903
#include "DevilSquare.h"
#endif // MODIFY_SEASON25_PCS_UNIFICATION_20070903

#ifdef ADD_MASTER_LEVEL_SYSTEM_COMMAND_20071122
#include "MasterLevelSystem.h"
#include "MagicDamage.h"
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#include "ItemSocketOptionSystem.h"
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
#include "XMasAttackEvent.h"
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"
#endif // ADD_NEWPVP_PKFIELD

#ifdef GMCOMMAND_EX
#include "PartyClass.h"
#include "MonsterAttr.h"
#include <math.h>
extern CMonsterAttr	gMAttr;

#define ERROR_RETURN_INT(t, err_code, func) \
if(!(t)) \
{ \
	func; \
	return err_code; \
}

#define ERROR_RETURN(t, func) \
if(!(t)) \
{ \
	func; \
	return; \
}
#endif // GMCOMMAND_EX

extern CwsGameServer	wsGServer;
extern CGuildClass		Guild;
extern classdef			DCInfo;

#ifdef ADD_MACRO_FOR_SETITEM_20060424
extern CSetItemMacro	gSetItemMacro;
#endif

void CGTeleportRecv(LPPMSG_TELEPORT lpMsg, int aIndex);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGMMng	cManager;


CGMMng::CGMMng()
{
	ManagerInit();
}

CGMMng::~CGMMng()
{
	
}

void CGMMng::Init()
{
	cCommand.Init();
	cCommand.Add("/disconnect",		GMCMD_DISCONNECT,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );
	cCommand.Add("/whisperoff",		GMCMD_CHATBAN,						AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );
	cCommand.Add("/whisperon",		GMCMD_CHATBANFREE,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );
	cCommand.Add("/guildmove",	GMCMD_GUILDSETPOSITION,				AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );
	cCommand.Add("/guilddisconnect",	GMCMD_GUILDDISCONNECT,				AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );
	cCommand.Add("/warend",			GMCMD_GUILDWAREND,					AUTHORITY_ADMIN);
	cCommand.Add("/warstart",			GMCMD_GUILDWARSTART,				AUTHORITY_ADMIN);
	cCommand.Add("/warstop",		GMCMD_GUILDWARSTOP,					AUTHORITY_ADMIN);

#ifdef GMCOMMAND_EX
	cCommand.Add("/능력",			GMCMD_GETABILITY,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/파티",			GMCMD_PARTYINFO,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/몬스터소환",		GMCMD_SUMMONMONSTER,				AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/몬스터제거",		GMCMD_CLEARMONSTER,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/아이템제거",		GMCMD_CLEARITEM,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/인벤제거",		GMCMD_CLEARINVEN,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/캐릭터소환",		GMCMD_SUMMONCHARACTER,				AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add("/ability",		GMCMD_GETABILITY,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/party",			GMCMD_PARTYINFO,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/summonmonster",	GMCMD_SUMMONMONSTER,				AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/clearmonster",	GMCMD_CLEARMONSTER,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/clearitem",		GMCMD_CLEARITEM,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/clearinven",		GMCMD_CLEARINVEN,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/summonchar",		GMCMD_SUMMONCHARACTER,				AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
#endif // GMCOMMAND_EX

	cCommand.Add(lMsg.Get(3013),	UMCMD_BILLCHECK,					AUTHORITY_EVENT_GM | AUTHORITY_USER);

	cCommand.Add("/war",			UMCMD_GUILDWAR,						AUTHORITY_EVENT_GM | AUTHORITY_USER);
#ifdef BATTLESOCCERWAR_COMMAND_BUGFIX_20090310
	cCommand.Add("/soccer",	UMCMD_BATTLESOCCERWAR,				AUTHORITY_ADMIN	   | AUTHORITY_USER);
#else
	cCommand.Add("/soccer",	UMCMD_BATTLESOCCERWAR,				AUTHORITY_ADMIN);
#endif // BATTLESOCCERWAR_COMMAND_BUGFIX_20090310
	cCommand.Add(lMsg.Get(3016),	UMCMD_TRADEOPTION,					AUTHORITY_EVENT_GM | AUTHORITY_USER);
	
	// 해외 버전용 영문 명령어
	cCommand.Add("/move",			GMCMD_SETPOSITION,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM | AUTHORITY_USER);
	cCommand.Add("/m",				GMCMD_SETPOSITION,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM | AUTHORITY_USER);
	cCommand.Add("/gmove",			GMCMD_SETPOSITION2,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );

	cCommand.Add("/make",			GMCMD_ITEMCREATE,					AUTHORITY_ADMIN);

	cCommand.Add(lMsg.Get(3036),	UMCMD_TRADEOPTION,					AUTHORITY_EVENT_GM | AUTHORITY_USER);


	// 접속현황
	cCommand.Add("/userstat",	GMCMD_USERSTAT,						AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	// 추적
	cCommand.Add("/trace",			GMCMD_USERTRAKING,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	// 감시
	cCommand.Add("/userwatch",	GMCMD_USERWATCHNG,					AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);



#ifdef ADD_KUNDUN_CONTROL_20040923
	cCommand.Add("/쿤둔상태",		GMCMD_NOTIFYKUNDUNHP,				AUTHORITY_ADMIN);
	cCommand.Add("/쿤둔피",			GMCMD_SETKUNDUNHP,					AUTHORITY_ADMIN);
	cCommand.Add("/쿤둔회복량",		GMCMD_SETKUNDUNREFILLHP,			AUTHORITY_ADMIN);
	cCommand.Add("/쿤둔초당회복량",	GMCMD_SETKUNDUNREFILLHPSEC,			AUTHORITY_ADMIN);
	cCommand.Add("/쿤둔회복시간",	GMCMD_SETKUNDUNREFILLHPTIME,		AUTHORITY_ADMIN);
	
#endif

#ifdef CHINA_GM_EVENT	
	cCommand.Add(lMsg.Get(3037),	GMCMD_START_GMEVENT,				AUTHORITY_EVENT_GM );	
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
	// 변경 필요
	cCommand.Add(lMsg.Get(3040),	GMCMD_START_GMEVENT,				AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906	

#ifdef UPDATE_GM_FUNCTION_20070228		// command 추가
	#ifdef FOR_KOREA
	cCommand.Add("/투명",		GMCMD_INVISIBLE,					AUTHORITY_EVENT_GM);
	cCommand.Add("/투명해제",	GMCMD_REMOVE_INVISIBLE,				AUTHORITY_EVENT_GM);
	#else
	cCommand.Add("/hide_on",	GMCMD_INVISIBLE,					AUTHORITY_EVENT_GM);
	cCommand.Add("/hide_off",	GMCMD_REMOVE_INVISIBLE,				AUTHORITY_EVENT_GM);
	#endif	
#endif

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
	// 해킹 툴 블록을 위한 GM명령어를 추가한다.
	cCommand.Add( "/핵툴블록추가",		GMCMD_ADD_HACKTOOL_MOVE_BLOCK,		AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );
	cCommand.Add( "/핵툴블록수정",		GMCMD_MODIFY_HACKTOOL_MOVE_BLOCK,	AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );
	cCommand.Add( "/핵툴블록삭제",		GMCMD_REMOVE_HACKTOOL_MOVE_BLOCK,	AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );
	cCommand.Add( "/핵툴통계",			GMCMD_STATISTICS_HACKTOOL,			AUTHORITY_ADMIN | AUTHORITY_EVENT_GM );
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#if (TESTSERVER == 1) || defined(FOR_TEMP_SERVER)
	
#ifdef CASTLE_GM_COMMAND_20041222	// 운영자 공성제어 명령추가
#ifdef FOR_KOREA
	cCommand.Add("/공성수성길드",	GMCMD_CS_CHANGEOWNERGUILD,			0);
	cCommand.Add("/공성참여등록",	GMCMD_CS_SETSTATE_REGSIEGE,			0);
	cCommand.Add("/공성문장등록",	GMCMD_CS_SETSTATE_REGMARK,			0);
	cCommand.Add("/공성선포",		GMCMD_CS_SETSTATE_NOTIFY,			0);
	cCommand.Add("/공성시작",		GMCMD_CS_SETSTATE_STARTSIEGE,		0);
	cCommand.Add("/공성종료",		GMCMD_CS_SETSTATE_ENDSIEGE,			0);
	cCommand.Add("/공성현재수성",	GMCMD_CS_SETSTATE_GETOWNERGUILD,	0);
	cCommand.Add("/공성현재상태",	GMCMD_CS_SETSTATE_GETCASTLESTATE,	0);
	cCommand.Add("/공수교체",		GMCMD_CS_CHANGECSSIDE,				0);
#else
	cCommand.Add("/cschangeowner",	GMCMD_CS_CHANGEOWNERGUILD,			0);
	cCommand.Add("/cssetregsiege",	GMCMD_CS_SETSTATE_REGSIEGE,			0);
	cCommand.Add("/cssetregmark",	GMCMD_CS_SETSTATE_REGMARK,			0);
	cCommand.Add("/cssetnotify",	GMCMD_CS_SETSTATE_NOTIFY,			0);
	cCommand.Add("/cssetstart",		GMCMD_CS_SETSTATE_STARTSIEGE,		0);
	cCommand.Add("/cssetend",		GMCMD_CS_SETSTATE_ENDSIEGE,			0);
	cCommand.Add("/cscurowner",		GMCMD_CS_SETSTATE_GETOWNERGUILD,	0);
	cCommand.Add("/cscurstate",		GMCMD_CS_SETSTATE_GETCASTLESTATE,	0);
	cCommand.Add("/cschangeside",	GMCMD_CS_CHANGECSSIDE,				0);
#endif	
#endif

// 사내 에서 테스트 용도
#if TESTSERVER == 1

#ifdef CRYWOLF_GM_COMMAND_20050525	// 크라이울프 명령어
	cCommand.Add("/crywolfready",	GMCMD_CRYWOLF_READY,				0);			// "/크라이울프준비"
	cCommand.Add("/crywolfstart",	GMCMD_CRYWOLF_START,				0);			// "/크라이울프시작"
	cCommand.Add("/crywolfend",		GMCMD_CRYWOLF_END,					0);			// "/크라이울프종료"
	cCommand.Add("/crywolfshow",	GMCMD_CRYWOLF_NOTIFY_2,				0);			// "/크라이울프연출"
#endif //CRYWOLF_GM_COMMAND_20050525

#else // TESTSERVER(테스트 서버가 아니면)

#ifdef CRYWOLF_GM_COMMAND_20050525	// 크라이울프 명령어
	// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
	cCommand.Add(lMsg.Get(3338),	GMCMD_CRYWOLF_READY,				0);			// "/크라이울프준비"
	cCommand.Add(lMsg.Get(3339),	GMCMD_CRYWOLF_START,				0);			// "/크라이울프시작"
	cCommand.Add(lMsg.Get(3340),	GMCMD_CRYWOLF_END,					0);			// "/크라이울프종료"
	cCommand.Add(lMsg.Get(3341),	GMCMD_CRYWOLF_NOTIFY_2,				0);			// "/크라이울프연출"
#endif //CRYWOLF_GM_COMMAND_20050525

#endif // TESTSERVER
	
#ifdef KANTURU_GM_COMMAND_20060709
	cCommand.Add( "/칸투르대기상태",		GMCMD_KANTURU_STANBY,						0);
	cCommand.Add( "/칸투르마야전",			GMCMD_KANTURU_BATTLEOFMAYA,					0);
	cCommand.Add( "/마야몬스터1",			GMCMD_KANTURU_MAYA_MONSTER1,				0);
	cCommand.Add( "/마야1",					GMCMD_KANTURU_MAYA1,						0);
	cCommand.Add( "/마야몬스터2",			GMCMD_KANTURU_MAYA_MONSTER2,				0);
	cCommand.Add( "/마야2",					GMCMD_KANTURU_MAYA2,						0);
	cCommand.Add( "/마야몬스터3",			GMCMD_KANTURU_MAYA_MONSTER3,				0);
	cCommand.Add( "/마야3",					GMCMD_KANTURU_MAYA3,						0);
	cCommand.Add( "/마야3성공",				GMCMD_KANTURU_MAYA3_SUCCESS,				0);
	cCommand.Add( lMsg.Get( 3388 ),			GMCMD_KANTURU_MAYA_SUCCESS,					0);				// 20060830 WTF 작업 "/마야성공"
	cCommand.Add( "/마야실패",				GMCMD_KANTURU_MAYA_FAIL,					0);
	cCommand.Add( "/칸투르나이트메어전",	GMCMD_KANTURU_BATTLEOFNIGHTMARE,			0);
	cCommand.Add( lMsg.Get( 3389 ),			GMCMD_KANTURU_BATTLEOFNIGHTMARE_SUCCESS,	0);	// 20060830 WTF 작업 "/나이트메어성공"
	cCommand.Add( "/나이트메어실패",		GMCMD_KANTURU_BATTLEOFNIGHTMARE_FAIL,		0);
	cCommand.Add( "/칸투르정제의탑",		GMCMD_KANTURU_TOWEROFREFINEMENT,			0);
	cCommand.Add( "/정제의탑열기",			GMCMD_KANTURU_TOWER_OPEN,					0);
	cCommand.Add( "/정제의탑닫기",			GMCMD_KANTURU_TOWER_CLOSE,					0);
	cCommand.Add( "/칸투르종료",			GMCMD_KANTURU_END,							0);
	cCommand.Add( lMsg.Get( 3385 ),			GMCMD_KANTURU_NEXT_STATE,					0);					// 20060830 WTF 작업 "/칸투르다음상태"
	cCommand.Add( lMsg.Get( 3386 ),			GMCMD_KANTURU_NEXT_DETAIL_STATE,			0);			// 20060830 WTF 작업 "/칸투르다음세부상태"
	cCommand.Add( lMsg.Get( 3387 ),			GMCMD_KANTURU_CURRENT_STATE,				0);				// 20060830 WTF 작업 "/칸투르현재상태"
#endif	// #ifdef KANTURU_GM_COMMAND_20060709	
	
#ifdef DARKLORD_WORK
	cCommand.Add("/다크",	GMCMD_DARKSPIRIT,					0);
	cCommand.Add("/다크스피릿레벨",	GMCMD_DARKSPIRIT_LEVEL,		0);	
#endif
	cCommand.Add("/스탯리셋",	GMCMD_STATRESET,				0);
	cCommand.Add("/레벨",		GMCMD_SETLEVEL,					0);
	cCommand.Add("/퀘스트리셋",	GMCMD_QUESTRESET,				0);		
	
#ifdef JS_ADD_TEST_COMMAND
	cCommand.Add("/번호",		GMCMD_INFORMITEMINDEX,			0);		
	cCommand.Add("/전체공지",	GMCMD_NOTICEALL ,				0);		
	cCommand.Add("/젠리셋",		GMCMD_RESETZEN ,				0);		
	cCommand.Add("/상태",		GMCMD_SETPKLEVEL ,				0);		
	cCommand.Add("/포인트",		GMCMD_SETPKPOINT,				0);		
	cCommand.Add("/몹상태",		GMCMD_NOTIFYMONSTERHP,			0);		
	cCommand.Add("/천사",		GMCMD_SET1004,					0);		
	
#ifndef MAKE_ITEM_STOP
	cCommand.Add("/스탯수정",	GMCMD_MODIFYSTAT,				0);		
	cCommand.Add("/STAT",		GMCMD_MODIFYSTAT_ENG,			0);		
	cCommand.Add("/만들래",		GMCMD_CREATEITEMNAME,			0);		
	cCommand.Add("/혼왕창",		GMCMD_MANYMANYCHAOS,			0);		
	cCommand.Add("/생왕창",		GMCMD_MANYMANYLIFE,				0);		
	cCommand.Add("/창왕창",		GMCMD_MANYMANYCREATE,			0);		
	cCommand.Add("/석주셈",		GMCMD_MANYMANYGEM,				0);		
	cCommand.Add("/왕창주셈",	GMCMD_MANYMANYITEM ,			0);		
	cCommand.Add("/목",			GMCMD_CREATEPROPERTYITEM,		0);		
	cCommand.Add("/축왕창",		GMCMD_MANYMANYBLESS,			0);				
	cCommand.Add("/영왕창",		GMCMD_MANYMANYSOUL,				0);				
	cCommand.Add("/세트",		GMCMD_MAKESETITEM,				0);				
	cCommand.Add("/랜덤세트",	GMCMD_MAKERANDOMSETITEM,		0);
#endif	// MAKE_ITEM_STOP
	
#ifdef FOR_KOREA
	cCommand.Add("/전직",		GMCMD_CLASS_UP,					0);
#ifdef THIRD_CHANGEUP_SYSTEM_20070507
	cCommand.Add( "/전직3",		GMCMD_THIRD_CHANGE_UP,			0);
	cCommand.Add( "/전직리셋",	GMCMD_CHANGE_UP_ALL_RESET,		0);	
	cCommand.Add( "/퀘템",		GMCMD_QUEST_ITEM,				0);
	cCommand.Add( "/템왕창",	GMCMD_MANYMANY_ITEM,			0);	
	cCommand.Add( "/퀘성공",	GMCMD_SET_QUEST,				0);
	cCommand.Add( "/젠",		GMCMD_SET_ZEN,					0);
#endif
#else
	cCommand.Add("/jobchange",			GMCMD_CLASS_UP,				0);
#ifdef THIRD_CHANGEUP_SYSTEM_20070507
	cCommand.Add( "/jobchange3",		GMCMD_THIRD_CHANGE_UP,		0);
	cCommand.Add( "/jobchangereset",	GMCMD_CHANGE_UP_ALL_RESET,	0);	
	cCommand.Add( "/questitem",			GMCMD_QUEST_ITEM,			0);
	cCommand.Add( "/템왕창",			GMCMD_MANYMANY_ITEM,		0);	
	cCommand.Add( "/questsuccess",		GMCMD_SET_QUEST,			0);
	cCommand.Add( "/젠",				GMCMD_SET_ZEN,				0);
#endif
#endif // FOR_KOREA
	
	cCommand.Add("/펜릴기본내구력감소",		GMCMD_FENRIR_DURSMALL_DEF,		0);
	cCommand.Add("/펜릴요정내구력감소",		GMCMD_FENRIR_DURSMALL_ELF,		0);
	
#ifdef ADD_SHIELD_POINT_01_20060403
	cCommand.Add("/쉴드수정",		GMCMD_SET_SHIELD_POINT,			0);
	cCommand.Add("/수치보기",		GMCMD_POINT_DISPLAY,			0);
#endif
	
#ifdef ITEM_ADD_OPTION_20061019
	cCommand.Add("/효과설정",		GMCMD_ITEM_EFFECT_SET,			0);
	cCommand.Add("/효과초기화",		GMCMD_ITEM_EFFECT_CLEAR,		0);
#endif
#endif
	
#ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410
	cCommand.Add("/현재인피니티애로우",		GMCMD_GETINFINITYARROW_MP,		0);
	cCommand.Add("/인피니티애로우+0",		GMCMD_SETINFINITYARROW_MP_0,	0);
	cCommand.Add("/인피니티애로우+1",		GMCMD_SETINFINITYARROW_MP_1,	0);
	cCommand.Add("/인피니티애로우+2",		GMCMD_SETINFINITYARROW_MP_2,	0);
	cCommand.Add("/인피니티애로우시간",		GMCMD_INFINITYARROW_SETTIME,	0);
#endif
	
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	cCommand.Add("/인피니티애로우+3",		GMCMD_SETINFINITYARROW_MP_3,	0);
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	
#ifdef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018
	//	cCommand.Add("/레벨업", GMCMD_LEVELUP_TEST,	0);
#endif
	
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	cCommand.Add( "/폭발거리",	GMCMD_FIRE_SCREAM,					0);
#endif
	
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	cCommand.Add("/조화", GMCMD_MAKE_STRENGTHENITEM,				0);
#endif
	
#ifdef ADD_380ITEM_NEWOPTION_20060711
	cCommand.Add("/삼팔공", GMCMD_MAKE_380OPTIONITEM,				0);
#endif
	
#ifdef MODIFY_SEASON25_PCS_UNIFICATION_20070903
	cCommand.Add("/복권", GMCMD_LOTTERY_ITEM_CREATE,				0);
#endif // MODIFY_SEASON25_PCS_UNIFICATION_20070903
	
#ifdef MODIFY_INVEN_CLEAR_MACRO_20071212
	cCommand.Add("/삭제", GMCMD_INVENTORY_ITEM_DELETE,				0);
	cCommand.Add("/인벤초기화", GMCMD_INVENTORY_ITEM_ALL_DELETE,	AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/DQTime", GMCMD_DEVILSQUARE_SET_TIME,				0);
#endif // MODIFY_INVEN_CLEAR_MACRO_20071212
	
	
#ifdef FOR_ONAIR
#ifdef ADD_MACRO_FOR_SETITEM_20060424
	cCommand.Add("/레벨",	GMCMD_SETLEVEL,						0);
	cCommand.Add("/스탯리셋",	GMCMD_STATRESET,				0);
#endif
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영 사원 이벤트 메크로
#ifdef FOR_KOREA
	cCommand.Add("/사원준비",				GMCMD_ILLUSION_TEMPLE_READY,		0);
	cCommand.Add("/사원시작",				GMCMD_ILLUSION_TEMPLE_START,		0);
	cCommand.Add("/사원종료",				GMCMD_ILLUSION_TEMPLE_END,			0);
	//	cCommand.Add("/IllusionTempleReset",	GMCMD_ILLUSION_TEMPLE_RESET);
#else
	cCommand.Add("/templeready",			GMCMD_ILLUSION_TEMPLE_READY,		0);
	cCommand.Add("/templestart",			GMCMD_ILLUSION_TEMPLE_START,		0);
	cCommand.Add("/templeend",				GMCMD_ILLUSION_TEMPLE_END,			0);
	//	cCommand.Add("/templereset",			GMCMD_ILLUSION_TEMPLE_RESET);
#endif // FOR_KOREA
#endif
	
#ifdef ADD_MASTER_LEVEL_SYSTEM_COMMAND_20071122		// command 추가
	cCommand.Add( "/masterlevel",				GMCMD_MASTERLEVEL_LEVELUP,		0);
	cCommand.Add( "/mlpoint",					GMCMD_MASTERLEVEL_POINT,		0);
	cCommand.Add( "/ms_reset",					GMCMD_MASTERLEVEL_SKILL_RESET,	0);
#endif
	
#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
	cCommand.Add( "/setsocket",					GMCMD_SET_SOCKET_OPTION,		0);	//<<<<<<shit
	cCommand.Add( "/clearsocket",				GMCMD_CLEAR_SOCKET_OPTION,		0);	//<<<<<<shit
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
	cCommand.Add( "/makesocketslot",			GMCMD_MAKE_SOCKET_SLOT,			0);	//<<<<<<shit
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
	
#ifdef ADD_RAKLION_20080408
	cCommand.Add( "/라클리온대기",			GMCMD_RAKLION_IDLE,					0);
	cCommand.Add( "/라클리온공지1",			GMCMD_RAKLION_NOTIFY_1,				0);
	cCommand.Add( "/라클리온스탠바이",		GMCMD_RAKLION_STANDBY,				0);
	cCommand.Add( "/라클리온공지2",			GMCMD_RAKLION_NOTIFY_2,				0);
	cCommand.Add( "/라클리온준비",			GMCMD_RAKLION_READY,				0);
	cCommand.Add( "/라클리온시작",			GMCMD_RAKLION_START_BATTLE,			0);
	cCommand.Add( "/라클리온공지3",			GMCMD_RAKLION_NOTIFY_3,				0);
	cCommand.Add( "/라클리온폐쇄",			GMCMD_RAKLION_CLOSE_DOOR,			0);
	cCommand.Add( "/라클리온실패",			GMCMD_RAKLION_ALL_USER_DIE,			0);
	cCommand.Add( "/라클리온공지4",			GMCMD_RAKLION_NOTIFY_4,				0);
	cCommand.Add( "/라클리온종료",			GMCMD_RAKLION_END,					0);
	cCommand.Add( "/세루판패턴1",			GMCMD_RAKLION_SELUPAN_1,			0);
	cCommand.Add( "/세루판패턴2",			GMCMD_RAKLION_SELUPAN_2,			0);
	cCommand.Add( "/세루판패턴3",			GMCMD_RAKLION_SELUPAN_3,			0);
	cCommand.Add( "/세루판패턴4",			GMCMD_RAKLION_SELUPAN_4,			0);
	cCommand.Add( "/세루판패턴5",			GMCMD_RAKLION_SELUPAN_5,			0);
	cCommand.Add( "/세루판패턴6",			GMCMD_RAKLION_SELUPAN_6,			0);
	cCommand.Add( "/세루판패턴7",			GMCMD_RAKLION_SELUPAN_7,			0);
	cCommand.Add( "/세루판최초스킬",		GMCMD_RAKLION_FIRST_SKILL,			0);
	cCommand.Add( "/세루판독공격",			GMCMD_RAKLION_POISON_ATTACK,		0);
	cCommand.Add( "/세루판냉기폭풍",		GMCMD_RAKLION_ICE_STORM,			0);
	cCommand.Add( "/세루판냉기충격",		GMCMD_RAKLION_ICE_STRIKE,			0);
	cCommand.Add( "/세루판소환",			GMCMD_RAKLION_SUMMON_MONSTER,		0);
	cCommand.Add( "/세루판힐",				GMCMD_RAKLION_HEAL,					0);
	cCommand.Add( "/세루판얼리기",			GMCMD_RAKLION_FREEZE,				0);
	cCommand.Add( "/세루판순간이동",		GMCMD_RAKLION_TELEPORT_RANDOM,		0);
	cCommand.Add( "/세루판무적화",			GMCMD_RAKLION_INVINCIBILITY,		0);
#endif // ADD_RAKLION_20080408

#ifdef MODIFY_SPEEDHACK_20080605
	cCommand.Add( "/abilitydebug",			GMCMD_ABILITY_DEBUG,				0);
#endif // MODIFY_SPEEDHACK_20080605

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
	cCommand.Add( "/xmaseventstart",		GMCMD_XMASEVENT_START,				0);
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#ifdef ADD_GIFTITEM_XMASEVENT_20081030
	// 크리스마스 선물 이벤트 관련 운영메시지로서 테스트 서버에서만 작동한다.  by natinda
	cCommand.Add( "/SetVisitorCount", GMCMD_XMASEVENT_SETVISITORCNT,			0);
	cCommand.Add( "/SetVisitorMode",GMCMD_XMASEVENT_SETSANTAMODE,				0); 
#endif//ADD_GIFTITEM_XMASEVENT_20081030
	
#ifdef ADD_NEWPVP_PKFIELD
	cCommand.Add( "/관전버프",				GMCMD_NEWPVP_OBSERVER,				0);
	cCommand.Add( "/검명",					GMCMD_NEWPVP_GLORYOFGLADIATOR,		0);
#endif // ADD_NEWPVP_PKFIELD
	
#ifdef PCBANG_POINT_SYSTEM_20070206
	cCommand.Add( "/addpcbangpoint",		GMCMD_ADD_PCBANG_POINT,				0);
	cCommand.Add( "/resetpcbangpoint",		GMCMD_RESET_PCBANG_POINT,			0);
#endif // PCBANG_POINT_SYSTEM_20070206

#endif	// #if (TESTSERVER==1)

	cCommand.Add( "/time",					GMCMD_TIME,							AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add( "/pkclear",				GMCMD_PKCLEAR,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add( "/online",				GMCMD_ONLINE,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add( "/str",					GMCMD_ADDSTR,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add( "/dex",					GMCMD_ADDDEX,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add( "/vit",					GMCMD_ADDVIT,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add( "/ene",					GMCMD_ADDENE,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

#ifdef DARKLORD_WORK
	cCommand.Add( "/com",					GMCMD_ADDCOM,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
#endif

	cCommand.Add( "/joinpvp",				GMCMD_JOINPVP,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add( "/reset",					GMCMD_RESETCOMMAND,					AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add( "/vipinfo",				GMCMD_VIPINFO,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add( "/online",				GMCMD_ONLINE,						AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add( "/time",					GMCMD_TIME,							AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add( "/q",						GMCMD_LIFE_POTION,					AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add( "/w",						GMCMD_MANA_POTION,					AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add( "/fire",					GMCMD_FIRECRACK,					AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add( "/ban",					GMCMD_BAN_USER,						AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add("/offafk",                 GMCMD_OFFAFK,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add("/offmoney",               GMCMD_OFFTRADE,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	//DevGamesX 01/05/2018 - OffExp
	cCommand.Add("/offlevel",                 GMCMD_OFFEXP,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/autoattackon",                 GMCMD_ATTACK,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	cCommand.Add("/autoattackoff",             GMCMD_NOATTACK,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);
	
	cCommand.Add("/duelinfo",               GMCMD_DUELINFO,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add("/offsellcash",               GMCMD_OFFCASH,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM); //GMCMD_BOTSTORE GMCMD_PICKZEN

	cCommand.Add("/botshop",               GMCMD_BOTSTORE,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add("/closegs",               GMCMD_CLOSEGS,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add("/deletedb",               GMCMD_DELETEDB,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	cCommand.Add("/deletevps",               GMCMD_DELETEVPS,                           AUTHORITY_USER | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM);

	WatchTargetIndex = -1;
}

void CGMMng::ManagerInit()
{
	for( int n=0; n<MAX_MANAGER; n++)
	{
		ManagerIndex[n] = -1;
	}
}

int CGMMng::ManagerAdd(char *name, int aIndex)
{
	for( int n=0; n<MAX_MANAGER; n++)
	{
		if( ManagerIndex[n] == -1 )
		{
			strcpy(szManagerName[n], name);
			ManagerIndex[n] = aIndex;
			return n;
		}
	}
	return -1;
}

void CGMMng::ManagerDel(char *name)
{
	for( int n=0; n<MAX_MANAGER; n++)
	{
		if( ManagerIndex[n] != -1 )
		{
			if( strcmp(szManagerName[n], name) == 0 )
			{
				ManagerIndex[n] = -1;
				return;
			}
		}
	}
}

void CGMMng::ManagerSendData(char *szMsg, int size)
{
	for( int n=0; n<MAX_MANAGER; n++)
	{
		if( ManagerIndex[n] != -1 )
		{
			GCServerMsgStringSend(szMsg, ManagerIndex[n], 0);
		}
	}
}


void CGMMng::DataSend(LPBYTE szMsg, int size)
{
	for( int n=0; n<MAX_MANAGER; n++)
	{
		if( ManagerIndex[n] != -1 )
		{
			::DataSend(ManagerIndex[n], szMsg, size);
		}
	}
}

void CGMMng::BattleInfoSend(char *Name1, BYTE score1, char *Name2, BYTE score2)
{
	for( int n=0; n<MAX_MANAGER; n++)
	{
		if( ManagerIndex[n] != -1 )
		{
			GCGoalSend(ManagerIndex[n], Name1, score1, Name2, score2);
		}
	}
}

int CGMMng::GetCmd(char* szCmd)
{	
	for( int n=0; n<MAX_GMCOMMAND; n++)
	{
		if( cCommand.nCmdCode[n] > 0 )
		{
			if( _stricmp(szCmd, cCommand.szCmd[n]) == 0 )
			{
				return cCommand.nCmdCode[n];
			}
		}
	}

	/*
	for( n=3020; n<MAX_GMCOMMAND; n++)
	{
		if( cCommand.nCmdCode[n] > 0 )
		{
			if( strcmp(strlwr(szCmd), cCommand.szCmd[n]) == 0 )
			{
				return cCommand.nCmdCode[n];
			}
		}
	}
	*/
	return 0;
}

bool CGMMng::SearchCmd(char* szCmd, int &iOutCmd, DWORD &iOutAuthority)
{
	for( int n=0; n<MAX_GMCOMMAND; n++)
	{
		if( cCommand.nCmdCode[n] > 0 )
		{
			if( _stricmp(szCmd, cCommand.szCmd[n]) == 0 )
			{
				iOutCmd = cCommand.nCmdCode[n];
				iOutAuthority = cCommand.dwAuthority[n];
				return(true);
			}
		}
	}

	return(false);
}


// 토큰을 스트링 형태로 넘긴다.
char * CGMMng::GetTokenString()
{
	char seps[] = " ";
	return strtok(NULL, seps);
}

// 토큰을 숫자 형태로 넘긴다.
int CGMMng::GetTokenNumber()
{
	char seps[] = " ";
	char * szToken;

	szToken = strtok(NULL, seps);

	if( szToken != NULL )
	{
		return atoi(szToken);
	}
	return 0;
}

int CGMMng::GetTokenNumberEx()
{
	char seps[] = " ";
	char * szToken;

	szToken = strtok(NULL, seps);

	if( szToken != NULL )
	{
		return atoi(szToken);
	}
	return -1;
}

int CGMMng::ManagementProc(LPOBJECTSTRUCT lpObj, char* szCmd, int aIndex)
{
	char seps[] = " ";
	char *szCmdToken;
	char string[256];
	char szId[20];
	char *pId = szId;
	

	int len= strlen(szCmd);
	if( len < 1 || len > 250 ) return FALSE;

	memset(szId, 0, sizeof(szId));

	strcpy(string, szCmd);
	
	szCmdToken = strtok(string, seps);
	int command_number = GetCmd(szCmdToken);

	int		nCmdCode;
	DWORD	dwAuthority;

	if(SearchCmd(szCmdToken, nCmdCode, dwAuthority) == false) return(0);

	if(CheckAuthority(dwAuthority, lpObj) == false) return(0);

	switch( nCmdCode )
	{
#ifdef CHINA_GM_EVENT		// GM Event 시작
		case GMCMD_START_GMEVENT :	
			{
//				if( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM ) 
//					return 0;

				gGMEventRunning = !gGMEventRunning;
				if( gGMEventRunning )
				{
					gObjMoveGate(aIndex, 27);
					
					LogAddTD("[GMEvent] [%s][%s] Start Event", lpObj->AccountID, lpObj->Name);					
					char temp[255];

					wsprintf(temp, lMsg.Get(1300) , lpObj->Name);
					AllSendServerMsg(temp);		// "%s가 출현했습니다             : (GM등장시 맨트)"					
				}
				else
				{
					GCServerMsgStringSend("Start End", aIndex, 1);
					LogAddTD("[GMEvent] [%s][%s] End Event", lpObj->AccountID, lpObj->Name);					
				}
			}
		break;
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
		case GMCMD_START_GMEVENT :	
			{
				if( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM ) 
					return 0;
				
				gGMEventRunning = !gGMEventRunning;
				if( gGMEventRunning )
				{
					gObjMoveGate(aIndex, 27);
					
					LogAddTD("[GMEvent] [%s][%s] Start Event", lpObj->AccountID, lpObj->Name);					
					char temp[255];
					
					wsprintf(temp, lMsg.Get(1300) , lpObj->Name);
					AllSendServerMsg(temp);		// "%s가 출현했습니다             : (GM등장시 맨트)"					
				}
				else
				{
					GCServerMsgStringSend("Start End", aIndex, 1);
					LogAddTD("[GMEvent] [%s][%s] End Event", lpObj->AccountID, lpObj->Name);					
				}
			}
			break;
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

	case GMCMD_MANYMANYBLESS :	
		{		
		for( int i = 0; i < 15; i++ )
			ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM(14,13), 0, (BYTE)0, 0, 0, 0, -1, 0, 0);
		}
	break;

	case GMCMD_MANYMANYSOUL:	
		{		
		for( int i = 0; i < 15; i++ )
			ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM(14,14), 0, (BYTE)0, 0, 0, 0, -1, 0, 0);
		}
	break;

#ifdef ITEM_ADD_OPTION_20061019
	case GMCMD_MAKERANDOMSETITEM:		// 세트 아이템 만들기
		{
			for( int i = 0; i < 20; i++ )
				MakeRandomSetItem(aIndex);
		}
	break;

	case GMCMD_MAKESETITEM:		// 세트 아이템 만들기
		{	
			char* pSetName;
			pSetName = GetTokenString();			
			if( pSetName == NULL ) return 0;

			int level, op1=0, op2=0, op3=0;
			BYTE NewOption=0;
			int  dur=255;
			
			level = GetTokenNumber();
			
			dur   = GetTokenNumber();

			op1 = GetTokenNumber();
			op2 = GetTokenNumber();
			int op3temp = GetTokenNumber();

			int addstat = GetTokenNumber();

			if( op3temp == 4 )
			{
				op3 = 1;
			}
			else if( op3temp == 8 )
			{
				op3 = 2;
			}
			else if( op3temp == 12 )
			{
				op3 = 3;
			}
			else if( op3temp == 16 )
			{
				op3 = 4;
			}
			else if( op3temp == 20 )
			{
				op3 = 5;
			}	
			else if( op3temp == 24 )
			{
				op3 = 6;
			}	
			else if( op3temp == 28 )
			{
				op3 = 7;
			}		
			
			int SetOption;
			for( int i = 0; i < MAX_ITEM; i++ )
			{
				if( !strcmp(gSetItemOption.GetSetOptionName(i, 1), pSetName) )
				{
					SetOption = 1;

					if( addstat == 0 )
					{
						if( rand()%100 < 80 )
							SetOption |= 0x04;
						else
							SetOption |= 0x08;
					}
					else
					{
						if( addstat == 1 )
							SetOption |= 0x04;
						else
							SetOption |= 0x08;
					}

					ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, i, 
										 level, (BYTE)dur, op1, op2, op3, -1, NewOption, SetOption);
				}

				if( !strcmp(gSetItemOption.GetSetOptionName(i, 2), pSetName) )
				{
					SetOption = 2;

					if( addstat == 0 )
					{
						if( rand()%100 < 80 )
							SetOption |= 0x04;
						else
							SetOption |= 0x08;
					}
					else
					{
						if( addstat == 1 )
							SetOption |= 0x04;
						else
							SetOption |= 0x08;
					}

					ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, i, 
										 level, (BYTE)dur, op1, op2, op3, -1, NewOption, SetOption);
				}
			}
		}
	break;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef JS_ADD_TEST_COMMAND
	case GMCMD_SET1004:
		{
			TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
			_Notice.SendToUser( lpObj->m_Index, "PK상태를 초기화 합니다.");
			lpObj->m_PK_Count	= 0;
			lpObj->m_PK_Level	= 3;
			lpObj->m_PK_Time	= 0;

#ifdef UPDATE_PK_SYSTEM_20070302	// 살인마 단계가 감소하면 파티에서도 처리
			if(lpObj->PartyNumber >= 0)	// 파티를 하고 있을 경우
			{
				gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
				gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
			}
		}
	break;

	case GMCMD_NOTIFYMONSTERHP:
		{
			INT	iMonsterClass = GetTokenNumber();
			
			// 현재 뷰포트 모든 몹
			if( iMonsterClass == 0 )
			{
				TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
				_Notice.SendToUser( lpObj->m_Index, "%d 클래스 몬스터가 없습니다.");
			}
			else
			{
				for( int count=0; count < MAXVIEWPORTOBJECT; count++ )
				{
					if( lpObj->VpPlayer[count].state )
					{
						if( lpObj->VpPlayer[count].type == OBJTYPE_MONSTER )
						{
							if( lpObj->VpPlayer[count].number >= 0 )
							{
								LPOBJECTSTRUCT lpTargetObj = &gObj[lpObj->VpPlayer[count].number];

								if( lpTargetObj->Class == iMonsterClass )
								{
									TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
									_Notice.SendToUser( lpObj->m_Index, 
										"%d 클래스 몬스터의 HP = %7.0f / %7.0f", iMonsterClass, lpTargetObj->Life,lpTargetObj->MaxLife );
								}
							}
						}
						
					}
				}
				
			}

		}

	break;

#ifdef MODIFY_NOTICE_20040325	
	case GMCMD_NOTICEALL:
		{	
			//TNotice _Notice();
			//TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
			//TNotice _Notice(TNOTICE_TYPE_GUILD);
			TNotice _Notice;
			_Notice.SendToAllUser("%s", GetTokenString() );			
		}
	break;
#endif
	

	case GMCMD_SETPKLEVEL:
		{
			int nPK_Level = GetTokenNumber();

			if( nPK_Level == 0 )
				nPK_Level = 3;

			if( nPK_Level == 1 ) 
			{
				lpObj->m_PK_Level = 1;
			}
			else if( nPK_Level == 2 ) 
			{
				lpObj->m_PK_Level = 2;
			}
			else if( nPK_Level == 3 ) 
			{
				lpObj->m_PK_Time = 0;
				lpObj->m_PK_Level = 3;
			}
			else if( nPK_Level == 4 ) 
			{
				lpObj->m_PK_Level = 4;
#ifdef UPDATE_PK_SYSTEM_20070302
				lpObj->m_PK_Count = 1;
#endif
			}
			else if( nPK_Level == 5 ) 
			{
				lpObj->m_PK_Level = 5;
#ifdef UPDATE_PK_SYSTEM_20070302
				lpObj->m_PK_Count = 2;
#endif
			}
			else if( nPK_Level == 6 ) 
			{
				lpObj->m_PK_Level = 6;
#ifdef UPDATE_PK_SYSTEM_20070302
				lpObj->m_PK_Count = 3;
#endif
			}
			else
			{
				lpObj->m_PK_Level = 3;
			}

			if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT + 1 )
			{	
				GCServerMsgStringSend(lMsg.Get(1137), lpObj->m_Index, 1);
			}
			
			if( lpObj->m_PK_Level <= PK_LEVEL_DEFAULT + 3 )
			{
				GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
			}

#ifdef UPDATE_PK_SYSTEM_20070302	// 살인마 단계가 감소하면 파티에서도 처리
			if(lpObj->PartyNumber >= 0)	// 파티를 하고 있을 경우
			{
				gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
				gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
			}
#endif
		}
	break;

	case GMCMD_CREATEITEMNAME:
		//if( (lpObj->AuthorityCode&AUTHORITY_CODE_SETPOSITION) != AUTHORITY_CODE_SETPOSITION) return 0;
	{
		int type, index, level, op1=0, op2=0, op3=0;
		BYTE NewOption=0;
		int  dur=255;

		//////////////////////////////////////////////////////////////////
		char _notice[100] = {0,};
		char _itemname[20] = {0,};
		
		try
		{
			ZeroMemory( _itemname, sizeof(_itemname) );
			strcpy( _itemname, GetTokenString() );
		}
		catch(...)
		{
			wsprintf(_itemname, "크리스");
		}
		
		
		int _Type, _Index;
		int i;
		for( i=0; i<MAX_ITEM; i++ )
		{
			if( !strcmp( _itemname, ItemAttribute[i].Name) )
			{
				_Type = i / MAX_ITEM_INDEX;
				_Index = i-_Type*MAX_ITEM_INDEX;
				sprintf( _notice, "[ %s ] 의 아이템 인덱스 = %d, %d", ItemAttribute[i].Name, _Type, _Index );
				break;
			}
		}
		
		if( i==MAX_ITEM )
		{
			sprintf( _notice, "알수 없는 아이템.." );
			GCServerMsgStringSend( _notice, lpObj->m_Index, 1);
			return 0;
		}
		
		GCServerMsgStringSend( _notice, lpObj->m_Index, 1);
		//////////////////////////////////////////////////////////////////
		
		type  = _Type;
		index = _Index;
		level = GetTokenNumber();
		
		dur   = GetTokenNumber();

		op1 = GetTokenNumber();
		op2 = GetTokenNumber();
		int op3temp = GetTokenNumber();
		
#ifdef FOR_BLOODCASTLE	// 정말정말 임시테스트 !! -> 무기생성 금지
//		if ((type >=0) && (type <=11))
//			return 0;
#endif

		if( type == 13 && index == 3 )
		{	// 디노란트를 만들때만
			if( op3temp < 8 )
				op3 = op3temp;
		}
		else
		{
			if( op3temp == 4 )
			{
				op3 = 1;
			}
			else if( op3temp == 8 )
			{
				op3 = 2;
			}
			else if( op3temp == 12 )
			{
				op3 = 3;
			}
			else if( op3temp == 16 )
			{
				op3 = 4;
			}
			else if( op3temp == 20 )
			{
				op3 = 5;
			}	
			else if( op3temp == 24 )
			{
				op3 = 6;
			}	
			else if( op3temp == 28 )
			{
				op3 = 7;
			}		
		}
		
		int setoption= GetTokenNumber();		
		int addstat= GetTokenNumber();

		
		if( setoption )
		{
			if( addstat == 0 )
			{
				if( rand()%100 < 80 )
					setoption |= 0x04;
				else
					setoption |= 0x08;
			}
			else
			{
				if( addstat == 1 )
					setoption |= 0x04;
				else
					setoption |= 0x08;
			}
		}		

		int nop1= GetTokenNumber();
		if( nop1 > 0 ) NewOption |= 0x20;
		int nop2= GetTokenNumber();
		if( nop2 > 0 ) NewOption |= 0x10;
		int nop3= GetTokenNumber();
		if( nop3 > 0 ) NewOption |= 0x08;
		int nop4= GetTokenNumber();
		if( nop4 > 0 ) NewOption |= 0x04;
		int nop5= GetTokenNumber();
		if( nop5 > 0 ) NewOption |= 0x02;
		int nop6= GetTokenNumber();
		if( nop6 > 0 ) NewOption |= 0x01;		
		
		if( (type >= 0 && type < MAX_ITEM_INDEX) &&
			(index>= 0 && index< MAX_ITEM_INDEX) )
		{
			int itemtype = ItemGetNumberMake( type, index);

			if( itemtype == MAKE_ITEMNUM(0, 19) || 
				itemtype == MAKE_ITEMNUM(4, 18) || 
				itemtype == MAKE_ITEMNUM(5, 10) 
#ifdef ADD_WEAPON_OF_ARCHANGEL_SCEPTER_20050706
				|| itemtype == MAKE_ITEMNUM(2, 13) 
#endif
			  )
			{	// 절대 무기의 경우 무조건 엑셀 옵션을 준다
				NewOption = 0x3f;
			}
			
#ifdef DARKLORD_WORK
			if( itemtype == MAKE_ITEMNUM(13, 4) || itemtype == MAKE_ITEMNUM(13, 5) )
			{
				PetItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, itemtype, level, (BYTE)dur, op1, op2, op3, -1, NewOption, setoption);
			}
			else
#endif	// #ifdef DARKLORD_WORK

			ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, itemtype, level, (BYTE)dur, op1, op2, op3, -1, NewOption, setoption);			
		}
	}	
	break;
#else
	}
	break;
#endif

	// 아이템 인덱스 갈켜주기
	case GMCMD_INFORMITEMINDEX:
		{	
			char _notice[100] = {0,};
			char _itemname[20] = {0,};
			strcpy( _itemname, GetTokenString() );
			
			int i;
			for( i=0; i<MAX_ITEM; i++ )
			{
				if( !strcmp( _itemname, ItemAttribute[i].Name) )
				{
					int _Type, _Index;

					_Type = i / MAX_ITEM_INDEX;
					_Index = i-_Type*MAX_ITEM_INDEX;
					sprintf( _notice, "[ %s ] 의 아이템 인덱스 = %d, %d", ItemAttribute[i].Name, _Type, _Index );
					break;
				}
			}

			if( i==MAX_ITEM )
				sprintf( _notice, "알수 없는 아이템.." );

			GCServerMsgStringSend( _notice, lpObj->m_Index, 1);
		}
	break;

	// 스탯수정
	case GMCMD_MODIFYSTAT:
		{	
			int Class = lpObj->Class;

			int _strength, _dexterity, _vitality, _energy;

			int _temp_levelup, _temp;

			_strength	= GetTokenNumber();
			_dexterity	= GetTokenNumber();
			_vitality	= GetTokenNumber();
			_energy		= GetTokenNumber();

			_temp = _strength + _dexterity + _vitality + _energy;

#ifdef DARKLORD_WORK
			if (lpObj->Class == CLASS_DARKLORD||lpObj->Class == CLASS_MAGUMSA) 
#else			
			if( lpObj->Class == CLASS_MAGUMSA )
#endif
				_temp_levelup = (lpObj->Level-1)*7 
								+ DCInfo.DefClass[Class].Strength + DCInfo.DefClass[Class].Dexterity
								+ DCInfo.DefClass[Class].Vitality + DCInfo.DefClass[Class].Energy;
			else
				_temp_levelup = (lpObj->Level-1)*5
								+ DCInfo.DefClass[Class].Strength + DCInfo.DefClass[Class].Dexterity
								+ DCInfo.DefClass[Class].Vitality + DCInfo.DefClass[Class].Energy;

			if( ( _strength < DCInfo.DefClass[Class].Strength)		|| 
				( _dexterity < DCInfo.DefClass[Class].Dexterity)	||
				( _vitality < DCInfo.DefClass[Class].Vitality)		||
				( _energy < DCInfo.DefClass[Class].Energy)
			  )
			{
				GCServerMsgStringSend("스탯 설정이 잘못 됐습니다.(기본값미만)", lpObj->m_Index, 1);
				return 0;
			}

			if( _temp > _temp_levelup )
			{
				GCServerMsgStringSend("스탯 설정이 잘못 됐습니다.(합초과)", lpObj->m_Index, 1);
				return 0;
			}
#ifdef NEW_SKILL_FORSKYLAND
			lpObj->ChangeUP		= 0;
#endif

			lpObj->Experience	= 0;

			lpObj->Strength		= _strength;
			lpObj->Dexterity	= _dexterity;
			lpObj->Vitality		= _vitality;
			lpObj->Energy		= _energy;
			lpObj->Life			= DCInfo.DefClass[Class].Life + 
								  ( _vitality - DCInfo.DefClass[Class].Vitality) * DCInfo.DefClass[Class].LevelLife ;
			lpObj->MaxLife		= DCInfo.DefClass[Class].MaxLife +
								  ( _vitality - DCInfo.DefClass[Class].Vitality) * DCInfo.DefClass[Class].LevelLife ;

			lpObj->Mana			= DCInfo.DefClass[Class].Mana + 
								  ( _vitality - DCInfo.DefClass[Class].Energy) * DCInfo.DefClass[Class].LevelMana;
			lpObj->MaxMana		= DCInfo.DefClass[Class].MaxMana + 
								  ( _vitality - DCInfo.DefClass[Class].Energy) * DCInfo.DefClass[Class].LevelMana;;

			lpObj->LevelUpPoint = _temp_levelup - _temp;
		
			GCServerMsgStringSend("으흐흐 캐릭터 스탯 재설정 완료!!", lpObj->m_Index, 1);
			gObjCloseSet(lpObj->m_Index, CL_BACK);			
		}
	break;

	// 스탯수정 영문( 무제한 )
	case GMCMD_MODIFYSTAT_ENG:
		{	
			int Class = lpObj->Class;

			int _strength, _dexterity, _vitality, _energy;

			_strength	= GetTokenNumber();
			_dexterity	= GetTokenNumber();
			_vitality	= GetTokenNumber();
			_energy		= GetTokenNumber();

#ifdef NEW_SKILL_FORSKYLAND
			lpObj->ChangeUP		= 0;
#endif

			lpObj->Experience	= 0;

			lpObj->Strength		= _strength;
			lpObj->Dexterity	= _dexterity;
			lpObj->Vitality		= _vitality;
			lpObj->Energy		= _energy;
			lpObj->Life			= DCInfo.DefClass[Class].Life + 
								  ( _vitality - DCInfo.DefClass[Class].Vitality) * DCInfo.DefClass[Class].LevelLife ;
			lpObj->MaxLife		= DCInfo.DefClass[Class].MaxLife +
								  ( _vitality - DCInfo.DefClass[Class].Vitality) * DCInfo.DefClass[Class].LevelLife ;

			lpObj->Mana			= DCInfo.DefClass[Class].Mana + 
								  ( _vitality - DCInfo.DefClass[Class].Energy) * DCInfo.DefClass[Class].LevelMana;
			lpObj->MaxMana		= DCInfo.DefClass[Class].MaxMana + 
								  ( _vitality - DCInfo.DefClass[Class].Energy) * DCInfo.DefClass[Class].LevelMana;;
	
			GCServerMsgStringSend("Modi.Stat Complete", lpObj->m_Index, 1);
			gObjCloseSet(lpObj->m_Index, CL_BACK);			
		}
	break;

	// 전직
	case GMCMD_CLASS_UP:
		{	
			int Class = lpObj->DbClass;

			if( Class == 0 )
				lpObj->DbClass = 1;
			else if( Class == 16)
				lpObj->DbClass = 17;
			else if( Class == 32)
				lpObj->DbClass = 33;
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// "/전직"
			else if( Class == 80 )
				lpObj->DbClass = BLOODY_SUMMONER;
#endif			
			
			GCServerMsgStringSend("전직...", lpObj->m_Index, 1);
			gObjCloseSet(lpObj->m_Index, CL_BACK);			
		}
	break;
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 전직,퀘스트관련 명령어 처리	
	case GMCMD_THIRD_CHANGE_UP:		// /전직3
		{
			if( lpObj->DbClass == 0 || lpObj->DbClass == 16 || lpObj->DbClass == 32 || lpObj->DbClass == 80 )
			{
				lpObj->ChangeUP = 1;
				lpObj->DbClass |= 0x1;
			}
			lpObj->ThirdChangeUp = 1;
			lpObj->DbClass		|= 0x2;
			
			GCServerMsgStringSend( "3차전직 성공", lpObj->m_Index, 1 );
			gObjCloseSet( lpObj->m_Index, CL_BACK );			
		}
	break;

	case GMCMD_CHANGE_UP_ALL_RESET:	// /전직리셋
		{
			lpObj->ChangeUP = 0;
			lpObj->ThirdChangeUp = 0;

			switch( lpObj->DbClass )
			{			
			case 1:
			case GRAND_MASTER:
				lpObj->DbClass = 0;
				break;

			case 17:
			case BLADE_MASTER:
				lpObj->DbClass = 16;
				break;	
				
			case 33:
			case HIGH_ELF:
				lpObj->DbClass = 32;
				break;	

			case DUEL_MASTER:
				lpObj->DbClass = 48;
				break;	

			case LORD_EMPEROR:
				lpObj->DbClass = 64;
				break;	
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 전직리셋
			case BLOODY_SUMMONER:
			case DIMENSION_MASTER:
				lpObj->DbClass = 80;
				break;
#endif
			}
			GCServerMsgStringSend( "전직리셋", lpObj->m_Index, 1 );
			gObjCloseSet( lpObj->m_Index, CL_BACK );
		}
		break;
		
	case GMCMD_QUEST_ITEM:	// /퀘템
		{
			int iQuestIndex = GetTokenNumber();
			if( iQuestIndex == 4 )
			{
				for( int i = 0; i <= 3; i++ )
				{
					ItemSerialCreateSend( aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM( 14, 65 ), 0, (BYTE)0, 0, 0, 0, -1, 0, 0 );
					ItemSerialCreateSend( aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM( 14, 66 ), 0, (BYTE)0, 0, 0, 0, -1, 0, 0 );
					ItemSerialCreateSend( aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM( 14, 67 ), 0, (BYTE)0, 0, 0, 0, -1, 0, 0 );
				}
			}
		}
		break;
	case GMCMD_MANYMANY_ITEM:	// /템왕창
		{
			int iType	= GetTokenNumber();
			int iIndex	= GetTokenNumber();
			int iNum	= GetTokenNumber();

			if( iNum <= 0 )
				iNum = 1;
			else if( iNum > 50 )
				iNum = 50;

			for( int i = 0; i < iNum; i++ )
			{
				ItemSerialCreateSend( aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM( iType, iIndex ), 0, (BYTE)0, 0, 0, 0, -1, 0, 0 );
			}
		}
		break;
	case GMCMD_SET_QUEST:	// /퀘성공 (숫자)
		{
			int iQuestIndex = GetTokenNumber();
			if( iQuestIndex >= 0 )
				g_QuestInfo.QuestSuccessCommand( aIndex, iQuestIndex );
		}
		break;
	case GMCMD_SET_ZEN:		// 젠 (숫자)
		{
			int iNewMoneyValue = GetTokenNumber();
			gObj[aIndex].Money = iNewMoneyValue;
			GCMoneySend( aIndex, gObj[aIndex].Money );
		}
		break;
		
#endif	// THIRD_CHANGEUP_SYSTEM_20070507

#ifdef ADD_ITEM_FENRIR_01_20051110
	// 펜릴 내구력 감소 기본
	case GMCMD_FENRIR_DURSMALL_DEF:
		{	
			int iDurSmallDef = GetTokenNumber();
			MsgOutput( lpObj->m_Index, "[펜릴기본내구력감소] %d -> %d", g_iFenrirDefaultMaxDurSmall, iDurSmallDef);
			g_iFenrirDefaultMaxDurSmall = iDurSmallDef;
		}
	break;

	// 펜릴 내구력 감소 요정
	case GMCMD_FENRIR_DURSMALL_ELF:
		{	
			int iDurSmallElf = GetTokenNumber();
			MsgOutput( lpObj->m_Index, "[펜릴요정내구력감소] %d -> %d", g_iFenrirElfMaxDurSmall, iDurSmallElf);
			g_iFenrirElfMaxDurSmall = iDurSmallElf;
		}
	break;
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	case GMCMD_SET_SHIELD_POINT:
		{
			int iShieldPoint = GetTokenNumber();
			MsgOutput( lpObj->m_Index, "[쉴드수치설정] %d -> %d", lpObj->iShield, iShieldPoint );
			if( iShieldPoint > ( lpObj->iMaxShield + lpObj->iAddShield ) )
			{
				iShieldPoint = lpObj->iMaxShield + lpObj->iAddShield;
			}
			lpObj->iShield = iShieldPoint;
			GCReFillSend( lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield );
		}
	break;
	case GMCMD_POINT_DISPLAY:
		{
			MsgOutput( lpObj->m_Index, "HP:%7.1f/%7.1f , SD:%d/%d", 
				lpObj->Life, lpObj->MaxLife+lpObj->AddLife, lpObj->iShield, lpObj->iMaxShield+lpObj->iAddShield );
		}
	break;
#endif

#ifdef ITEM_ADD_OPTION_20061019
	case GMCMD_ITEM_EFFECT_SET:
		{
			int iEffectType = GetTokenNumber();
			int iEffectValue = GetTokenNumber();
			int iEffectValidTime = GetTokenNumber();

			g_ItemAddOption.SetItemEffectForTest( lpObj, iEffectType, iEffectValue, iEffectValidTime );
#ifdef ADD_SHIELD_POINT_01_20060403
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
#else
			GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0);
#endif
			GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);		
		}
	break;
	case GMCMD_ITEM_EFFECT_CLEAR:
		{
#if 0
			g_ItemAddOption.ClearItemEffect( lpObj, ITEMUSE_EFFECT_OPTION_CANCEL );
			MsgOutput( lpObj->m_Index, "아이템 효과가 초기화 되었습니다.");
#endif
		}
	break;
#endif // ITEM_ADD_OPTION_20061019

	// 석주셈
	case GMCMD_MANYMANYGEM:
		{		
			for( int i = 0; i < 5; i++ )
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM(12,15), 0, (BYTE)0, 0, 0, 0, -1, 0, 0);
			}

			for( int i = 0; i < 5; i++ )
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM(14,13), 0, (BYTE)0, 0, 0, 0, -1, 0, 0);
			}

			for( int i = 0; i < 5; i++ )
			{
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM(14,14), 0, (BYTE)0, 0, 0, 0, -1, 0, 0);
			}
	
		}
	break;

	// 왕창주셈
	case GMCMD_MANYMANYITEM:
		//if( (lpObj->AuthorityCode&AUTHORITY_CODE_SETPOSITION) != AUTHORITY_CODE_SETPOSITION) return 0;
		{
		int type, index, level, op1=0, op2=0, op3=0;
		BYTE NewOption=0;
		int  dur=255;

		type = GetTokenNumber();
		index = GetTokenNumber();
		level = GetTokenNumber();
		
		dur   = GetTokenNumber();

		op1 = GetTokenNumber();
		op2 = GetTokenNumber();
		int op3temp = GetTokenNumber();
		
#ifdef FOR_BLOODCASTLE	// 정말정말 임시테스트 !! -> 무기생성 금지
//		if ((type >=0) && (type <=11))
//			return 0;
#endif

		if( type == 13 && index == 3 )
		{	// 디노란트를 만들때만
			if( op3temp < 8 )
				op3 = op3temp;
		}
		else
		{
			if( op3temp == 4 )
			{
				op3 = 1;
			}
			else if( op3temp == 8 )
			{
				op3 = 2;
			}
			else if( op3temp == 12 )
			{
				op3 = 3;
			}
			else if( op3temp == 16 )
			{
				op3 = 4;
			}
			else if( op3temp == 20 )
			{
				op3 = 5;
			}	
			else if( op3temp == 24 )
			{
				op3 = 6;
			}	
			else if( op3temp == 28 )
			{
				op3 = 7;
			}		
		}
		
		int setoption= GetTokenNumber();		
		int addstat= GetTokenNumber();

		
		if( setoption )
		{
			if( addstat == 0 )
			{
				if( rand()%100 < 80 )
					setoption |= 0x04;
				else
					setoption |= 0x08;
			}
			else
			{
				if( addstat == 1 )
					setoption |= 0x04;
				else
					setoption |= 0x08;
			}
		}		

		int nop1= GetTokenNumber();
		if( nop1 > 0 ) NewOption |= 0x20;
		int nop2= GetTokenNumber();
		if( nop2 > 0 ) NewOption |= 0x10;
		int nop3= GetTokenNumber();
		if( nop3 > 0 ) NewOption |= 0x08;
		int nop4= GetTokenNumber();
		if( nop4 > 0 ) NewOption |= 0x04;
		int nop5= GetTokenNumber();
		if( nop5 > 0 ) NewOption |= 0x02;
		int nop6= GetTokenNumber();
		if( nop6 > 0 ) NewOption |= 0x01;		
		
		if( (type >= 0 && type < MAX_ITEM_INDEX) &&
			(index>= 0 && index< MAX_ITEM_INDEX) )
		{
			int itemtype = ItemGetNumberMake( type, index);

			if( itemtype == MAKE_ITEMNUM(0, 19) || 
				itemtype == MAKE_ITEMNUM(4, 18) || 
				itemtype == MAKE_ITEMNUM(5, 10)
#ifdef ADD_WEAPON_OF_ARCHANGEL_SCEPTER_20050706
				|| itemtype == MAKE_ITEMNUM(2, 13)
#endif
			  )
			{	// 절대 무기의 경우 무조건 엑셀 옵션을 준다
				NewOption = 0x3f;
			}

			for( int i = 0; i < 15; i++ )
			{			
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, itemtype, level, (BYTE)dur, op1, op2, op3, -1, NewOption, setoption);
			}
			
		}
		}
	break;

	// 혼석 왕창
	case GMCMD_MANYMANYCHAOS:
		{		
			for( int i = 0; i < 15; i++ )
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM(12,15), 0, (BYTE)0, 0, 0, 0, -1, 0, 0);
		}
	break;

	// 생석 왕창
	case GMCMD_MANYMANYLIFE:
		{		
			for( int i = 0; i < 15; i++ )
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM(14,16), 0, (BYTE)0, 0, 0, 0, -1, 0, 0);
		}
	break;

	// 창석 왕창
	case GMCMD_MANYMANYCREATE:
		{		
			for( int i = 0; i < 15; i++ )
				ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, MAKE_ITEMNUM(14,22), 0, (BYTE)0, 0, 0, 0, -1, 0, 0);
		}
	break;

	case GMCMD_RESETZEN:
		{
			gObj[aIndex].Money = 0;
			GCMoneySend(aIndex, 0);
		}
	break;
////////////////////////////////////////////////////////////////////////////////////////////////
	
#ifdef ADD_HACKING_TOOL_BLOCK_20090311
	case GMCMD_ADD_HACKTOOL_MOVE_BLOCK:
		{
			// 인증 서버로 해당 프로그램의 사용자를 블록하기 위한 명령을 브로드 캐스팅 한다.
			// /핵툴블록추가 "프로그램 이름" "명령어 발동 시작 시간" "발동 시간" "발동 확률"
			char chFileName[64];
			char chActiveTime[20];
			char chHMSDate[10];
			int iActiveTerm = 0;
			int iActiveRate = 0;

			DWORD dwActiveTime = 0;

			strcpy( chFileName, GetTokenString() );
			strcpy( chActiveTime, GetTokenString() );
			strcpy( chHMSDate, GetTokenString() );
			strcat( chActiveTime, " " );
			strcat( chActiveTime, chHMSDate );
			iActiveTerm = GetTokenNumber();
			iActiveRate = GetTokenNumber();

			dwActiveTime = g_HackToolBlock.MakeDateToDword( chActiveTime );

			g_HackToolBlock.SetGMObject( &gObj[aIndex] );

			CTime tCurrentTime = CTime::GetCurrentTime();
			CTime tActiveTime( dwActiveTime );

			if( tCurrentTime > tActiveTime )
			{
				// 설정한 시간이 현재 시간보다 이전이다.
				g_HackToolBlock.SendNoticeToGM( "[ANTI-HACK] 설정한 시간이 현재 시간보다 이전입니다.",
					chFileName, 
					chActiveTime,
					iActiveTerm,
					iActiveRate );
			}
			else
			{
				g_HackToolBlock.SendNoticeToGM( "[ANTI-HACK] 차단할 오토 프로그램이 등록되었습니다. Name : %s, ActiveTime : %s, Term : %d, Rate : %d%%",
					chFileName, 
					chActiveTime,
					iActiveTerm,
					iActiveRate );
			}

			LogAddTD( "[ANTI-HACK] Add Block Antihack tool User Program [%s] Time Set [%s] Term [%d] Block Rate [%d]",
				chFileName ,
				chActiveTime ,
				iActiveTerm ,
				iActiveRate
				);

#ifdef MODIFY_HACKTOOL_BLOCK_ALL_SESSION_20090422
			GJRequestHacktoolMapMoveBlock( HACKTOOL_MODIFY_TYPE_ADD, chFileName, dwActiveTime, iActiveTerm, iActiveRate );
#else
	#ifdef MODIFY_HACKING_TOOL_BLOCK_SINGLE_SERVER_20090325
			g_HackToolBlock.AddHacktoolData( chFileName, dwActiveTime, iActiveTerm, iActiveRate );
			g_HackToolBlock.HacktoolStatisticsAll( chFileName );
	#else
			//GJRequestHacktoolMapMoveBlock( chFileName, 
	#endif // MODIFY_HACKING_TOOL_BLOCK_SINGLE_SERVER_20090325
#endif // MODIFY_HACKTOOL_BLOCK_ALL_SESSION_20090422
		}
		break;
	case GMCMD_MODIFY_HACKTOOL_MOVE_BLOCK:
		{
			// 추가한 핵툴 맵 이동 블럭의 설정을 변경한다.
			// /핵툴블록수정 "프로그램 이름" "명령어 발동 시작 시간" "발동 시간" "발동 확률"
			char chFileName[64];
			char chActiveTime[20];
			char chHMSDate[10];
			int iActiveTerm = 0;
			int iActiveRate = 0;

			DWORD dwActiveTime = 0;

			strcpy( chFileName, GetTokenString() );
			strcpy( chActiveTime, GetTokenString() );
			strcpy( chHMSDate, GetTokenString() );
			strcat( chActiveTime, " " );
			strcat( chActiveTime, chHMSDate );
			iActiveTerm = GetTokenNumber();
			iActiveRate = GetTokenNumber();

			dwActiveTime = g_HackToolBlock.MakeDateToDword( chActiveTime );

			LogAddTD( "[ANTI-HACK] Modify Block Antihack tool User Program [%s] Time Set [%d] Term [%d] Block Rate [%d]",
				chFileName ,
				chActiveTime ,
				iActiveTerm ,
				iActiveRate
				);

#ifdef MODIFY_HACKTOOL_BLOCK_ALL_SESSION_20090422
			GJRequestHacktoolMapMoveBlock( HACKTOOL_MODIFY_TYPE_MODIFY, chFileName, dwActiveTime, iActiveTerm, iActiveRate );
#else
			g_HackToolBlock.AddHacktoolData( chFileName, dwActiveTime, iActiveTerm, iActiveRate );
			g_HackToolBlock.HacktoolStatisticsAll( chFileName );
#endif // MODIFY_HACKTOOL_BLOCK_ALL_SESSION_20090422
		}
		break;
	case GMCMD_REMOVE_HACKTOOL_MOVE_BLOCK:
		{
			// 추가한 핵툴 맵 이동 블럭의 설정을 삭제한다.
			// /핵툴블록삭제 "프로그램 이름"
			char chFileName[64];
			
			strcpy( chFileName, GetTokenString() );
			LogAddTD( "[ANTI-HACK] Remove Block Antihack tool User Program [%s]",
				chFileName 
				);

			g_HackToolBlock.SendNoticeToGM( "[ANTI-HACK] 등록된 오토 프로그램이 삭제되었습니다. ( Name : %s )",
				chFileName );
#ifdef MODIFY_HACKTOOL_BLOCK_ALL_SESSION_20090422
			GJRequestHacktoolMapMoveBlock( HACKTOOL_MODIFY_TYPE_REMOVE, chFileName, 0, 0, 0 );
#else
			g_HackToolBlock.RemoveHacktoolData( chFileName );
#endif // MODIFY_HACKTOOL_BLOCK_ALL_SESSION_20090422
		}
		break;
	case GMCMD_STATISTICS_HACKTOOL:
		{
			// 해당 해킹툴의 사용자의 통계를 확인한다.
			// 명령어 구조
			// /ANTIHACKSTATISTIC "프로그램 이름"
			char chFileName[64];
			
			strcpy( chFileName, GetTokenString() );
			
			LogAddTD( "[ANTI-HACK] Statistics Antihack tool User Program [%s]",
				chFileName
				);

			g_HackToolBlock.SendNoticeToGM( "[ANTI-HACK] 오토 프로그램 사용자 통계를 요청하였습니다. ( Name : %s )",
				chFileName );

#ifdef MODIFY_HACKTOOL_BLOCK_ALL_SESSION_20090422
			GJRequestAntiHackStatistics( chFileName );
#else
			g_HackToolBlock.HacktoolStatisticsAll( chFileName );
#endif // MODIFY_HACKTOOL_BLOCK_ALL_SESSION_20090422
		}
		break;
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MASTER_LEVEL_SYSTEM_COMMAND_20071122		// 명령어 처리
	case GMCMD_MASTERLEVEL_LEVELUP:		// /masterlevel
		{
			int iChangeLevel = GetTokenNumber();
			if( iChangeLevel < 0 ) iChangeLevel = 0;
			else if( iChangeLevel > 200 ) iChangeLevel = 200;

#ifdef MODIFY_MASTER_LEVEL_SYSTEM_COMMAND_20080417
			// 일반유저 - 마스터레벨 초기화 처리되도록 수정
			if( !g_MasterLevelSystem.IsMasterLevelUser( lpObj ) 
				|| (lpObj->m_nMasterLevel > 0 && iChangeLevel == 0)
				)
#else
				// 마스터레벨 유저만 처리한다.
			if( !g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
#endif // MODIFY_MASTER_LEVEL_SYSTEM_COMMAND_20080417
			{
				LogAddTD( "Use Test Command [%s][%s] /masterlevel %d -> Fail!!",
				lpObj->AccountID, lpObj->Name, iChangeLevel );
				return 0;
			}

			lpObj->m_nMasterLevel			= iChangeLevel;
			lpObj->m_i64MasterLevelExp		= g_MasterLevelSystem.GetMasterLevelExpTlbInfo( iChangeLevel );
			lpObj->m_i64NextMasterLevelExp	= g_MasterLevelSystem.GetMasterLevelExpTlbInfo( iChangeLevel + 1 );
			
			g_MasterLevelSystem.GCMasterLevelInfo( lpObj );
			LogAddTD( "Use Test Command [%s][%s] /masterlevel %d -> Success!!",
				lpObj->AccountID, lpObj->Name, iChangeLevel );
		}
		break;

	case GMCMD_MASTERLEVEL_POINT:	// /mlpoint
		{
			int iMLPoint = GetTokenNumber();

			// 마스터레벨 유저만 처리한다.
			if( !g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
			{
				LogAddTD( "Use Test Command [%s][%s] /mlpoint %d -> Fail!!",
				lpObj->AccountID, lpObj->Name, iMLPoint );
				return 0;
			}

			lpObj->m_iMasterLevelPoint		= iMLPoint;					
			g_MasterLevelSystem.GCMasterLevelInfo( lpObj );
			LogAddTD( "Use Test Command [%s][%s] /mlpoint %d -> Success!!",
				lpObj->AccountID, lpObj->Name, iMLPoint );
		}
		break;
	case GMCMD_MASTERLEVEL_SKILL_RESET:	// /ms_reset
		{
			// 마법리스트에서 마스터레벨 스킬은 모두 지워버린다.
			for( int n=0; n<MAX_MAGIC; n++ )
			{
				if( lpObj->Magic[n].IsMagic() == TRUE )
				{
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 액티브 스킬 확인 추가
					if( g_MasterSkillSystem.CheckMasterLevelSkill( lpObj->Magic[n].m_Skill ) )
#else
					if( MagicDamageC.GetSkillUseType( lpObj->Magic[n].m_Skill ) == SKILL_USETYPE_MASTERSKILL_PASSIVE )
#endif
					{
						lpObj->Magic[n].Clear();
					}
				}
			}

			// 클라이언트 재시작
			GCServerMsgStringSend( "Master Skill Reset", lpObj->m_Index, 1 );
			LogAddTD( "Use Test Command [%s][%s] /ms_reset %d",
				lpObj->AccountID, lpObj->Name, lpObj->m_iMasterLevelPoint );

			gObjCloseSet(lpObj->m_Index, CL_BACK);	
		}
		break;
#endif	// ADD_MASTER_LEVEL_SYSTEM_COMMAND_20071122

#ifdef MODIFY_SPEEDHACK_20080605
	case GMCMD_ABILITY_DEBUG:
		{
			int nFlag = GetTokenNumber();
			g_bAbilityDebug = (nFlag == 1) ? TRUE : FALSE;
			
			char szMsg[256]; 
			wsprintf(szMsg, "Ability Debug: %s", ((g_bAbilityDebug == TRUE) ? "On" : "Off"));
			GCServerMsgStringSend( szMsg, lpObj->m_Index, 1 );
		}
		break;
#endif // MODIFY_SPEEDHACK_20080605

#if TESTSERVER == 1 && defined(ADD_ITEM_ATTACK_XMASEVENT_20081014)
	case GMCMD_XMASEVENT_START:
		{
			int nExecuteDirect = GetTokenNumber();
			if (nExecuteDirect != TRUE) nExecuteDirect = FALSE;
			
			if (g_XMasAttackEvent.IsPossibleExecuteDirect())
			{
				GCServerMsgStringSend("XMasAttackEvent ExecuteDirect", lpObj->m_Index, 1 );
				g_XMasAttackEvent.ExecuteDirect(nExecuteDirect);
			}
			else
			{
				GCServerMsgStringSend("XMasAttackEvent fail ExecuteDirect", lpObj->m_Index, 1 );
			}
		}
		break;
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#if TESTSERVER == 1 && defined(ADD_GIFTITEM_XMASEVENT_20081030)
	case GMCMD_XMASEVENT_SETVISITORCNT:
		{
			int nCount = GetTokenNumber();
			if(nCount < 0) {
				nCount = 0;
			} else if(nCount > INT_MAX){
				nCount = INT_MAX;
			}
			
			gXmasVisitorCount = nCount;
			
			char szMsg[256]; 
			wsprintf(szMsg, "[X-Mas] [Visitor Count] : [%d] ", gXmasVisitorCount);
			GCServerMsgStringSend(szMsg, lpObj->m_Index, 1 );
		}
		break;
	case GMCMD_XMASEVENT_SETSANTAMODE:
		{
			short nTestFlag = GetTokenNumber();
			if(nTestFlag < 0 || nTestFlag > 2) break;
			
			gXmasGiftForTest = nTestFlag;
			
			char szMsg[256]; 
			wsprintf(szMsg, "[X-Mas] [CURRENT MODE] : %d ", gXmasGiftForTest);
			GCServerMsgStringSend(szMsg, lpObj->m_Index, 1 );
			
		}
#endif


#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
	case GMCMD_SET_SOCKET_OPTION:
		{
			int iInventoryPos;
			BYTE SeedIndex;
			BYTE SphereLevel;
			BYTE SocketSlot;

			iInventoryPos = GetTokenNumber();
			SeedIndex = GetTokenNumber();
			SphereLevel = GetTokenNumber();
			SocketSlot = GetTokenNumber();

			g_SocketOptionSystem.SetSocketSlot( &lpObj->pInventory[iInventoryPos], SeedIndex, SphereLevel-1, SocketSlot-1 );
			GCInventoryItemOneSend( lpObj->m_Index, iInventoryPos );
		}
		break;
	case GMCMD_CLEAR_SOCKET_OPTION:
		{
			int iInventoryPos;
			BYTE SocketSlot;

			iInventoryPos = GetTokenNumber();
			SocketSlot = GetTokenNumber();

			if( SocketSlot == 0 )
			{
				for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
				{
					g_SocketOptionSystem.ClearSocketSlot( &lpObj->pInventory[iInventoryPos], i );
				}
			}
			else
			{
				g_SocketOptionSystem.ClearSocketSlot( &lpObj->pInventory[iInventoryPos], SocketSlot-1 );
			}
			GCInventoryItemOneSend( lpObj->m_Index, iInventoryPos );
		}
		break;
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
	case GMCMD_MAKE_SOCKET_SLOT:
		{
			int iInventoryPos;
			BYTE SocketSlotCount;

			iInventoryPos = GetTokenNumber();
			SocketSlotCount = GetTokenNumber();

			g_SocketOptionSystem.MakeSocketSlot( &lpObj->pInventory[iInventoryPos], SocketSlotCount );
			GCInventoryItemOneSend( lpObj->m_Index, iInventoryPos );
		}
		break;
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#endif
	

	//------------------------------------------------------------------------
	// 폭죽
	case GMCMD_FIRECRACK :	
		{
			int x, y;			
			x   = lpObj->X;
			y   = lpObj->Y;

			PMSG_SERVERCMD	ServerCmd;
			PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			

			ServerCmd.CmdType = 0;

			for( int i = 0; i < 15; i++ )
			{
				ServerCmd.X = x+((rand()%5)*2)-4;
				ServerCmd.Y = y+((rand()%5)*2)-4;
				MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
				::DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
			}
		}
		break;

	//------------------------------------------------------------------------
	// 사용자 감시
	case GMCMD_USERWATCHNG :

#ifndef GMCOMMAND_EX_1ST_BUGFIX

#ifndef FOR_ONAIR
		if( (lpObj->Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN 

#ifdef ADD_AUTHORITY_CODE_OF_EVENT_GM_20050602
			&& (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM
#endif
       	  ) 
			return 0;
#endif

#endif // GMCOMMAND_EX_1ST_BUGFIX

		{
			pId = GetTokenString();			
			if( pId == NULL ) return 0;

			LPOBJECTSTRUCT	lpTargetObj = gObjFind(pId);

			if( lpTargetObj == NULL )
				return 0;
			
	//## ADD_LOG_GM_COMMAND_20050426
	#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s ", 
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, 
				lpTargetObj->AccountID, lpTargetObj->Name, "User Watching");
	#endif

			char sMsg[255];
			if( WatchTargetIndex == lpTargetObj->m_Index )
			{
				WatchTargetIndex = -1;
				
				wsprintf(sMsg, "%s : 감시해제", lpTargetObj->Name);
				GCServerMsgStringSend(sMsg, lpObj->m_Index, 1);
			}
			else
			{
				wsprintf(sMsg, "%s : 감시시작", lpTargetObj->Name);
				GCServerMsgStringSend(sMsg, lpObj->m_Index, 1);
				WatchTargetIndex = lpTargetObj->m_Index;
			}
		}
		break;			

	//------------------------------------------------------------------------
	// 사용자 추적		
	case GMCMD_USERTRAKING :
		{
			pId = GetTokenString();
			int map, x, y;
			if( pId == NULL ) return 0;

			LPOBJECTSTRUCT	lpTargetObj = gObjFind(pId);

			if( lpTargetObj == NULL )
				return 0;

	//## ADD_LOG_GM_COMMAND_20050426
	#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s", 
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, 
				lpTargetObj->AccountID, lpTargetObj->Name, "User Tracking");
	#endif
			
			map = lpTargetObj->MapNumber;
			x   = lpTargetObj->X;
			y   = lpTargetObj->Y;
			int aIndex = lpObj->m_Index;
			if( aIndex >= 0 )
			{
				gObjTeleport(aIndex, map, x, y);
			}
		}
		break;		

	//------------------------------------------------------------------------
	// 사용자 접속 현황 보기
	case GMCMD_USERSTAT :	
/*
#ifdef UPDATE_GM_FUNCTION_20070228	//  EVENT_GM 일 경우 '/접속현황' 추가
		if( ( (lpObj->Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN) 
			&& ( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM) ) 
		{
			return 0;
		}
#else
		if( (lpObj->Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) return 0;
#endif
*/
		{
	#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "User Stat (connection)");
	#endif
			int StartLevel = 0;
			int EndLevel = g_GlobalConfig.m_iMaxLevel;

			int start= GetTokenNumber();
			if( start > 0 ) StartLevel = start;

			int end= GetTokenNumber();
			if( end > 0 ) EndLevel = end;

			gObjSendUserStatistic(lpObj->m_Index, StartLevel, EndLevel);
		}
		break;
	//------------------------------------------------------------------------
	// 강제 접속 종료
	case GMCMD_DISCONNECT :	
		//if( (lpObj->Authority&AUTHORITY_USER) != AUTHORITY_USER ) 
//		if( (lpObj->AuthorityCode&AUTHORITY_CODE_DISCONNECT) != AUTHORITY_CODE_DISCONNECT) return 0;

		pId = GetTokenString();
		if( pId == NULL ) return 0;
		{
			int aIndex = gObjGetIndex(pId);
			if( aIndex >= 0 )
			{
	//## ADD_LOG_GM_COMMAND_20050426
	#ifdef ADD_LOG_GM_COMMAND_20040420
				LPOBJECTSTRUCT	lpTargetObj = gObjFind(pId);
				
				if( lpTargetObj == NULL ) return 0;

				LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s", 
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, 
				lpTargetObj->AccountID, lpTargetObj->Name, "User Disconnect");
	#endif

				LogAdd(lMsg.Get(447),pId);
				CloseClient(aIndex);
				GJPUserClose2(lpObj->AccountID);
		        gObjDel(aIndex);
			}
		}
		break;
	case GMCMD_GUILDDISCONNECT :
//		if( (lpObj->AuthorityCode&AUTHORITY_CODE_DISCONNECT) != AUTHORITY_CODE_DISCONNECT) return 0;

	#ifdef ADD_LOG_GM_COMMAND_20040420
		LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "Guild Disconnect");
	#endif

		pId = GetTokenString();
		if( pId == NULL ) return 0;
		{
			LPGUILD_INFO lpNode = Guild.SearchGuild(pId);
			if( lpNode != NULL )
			{
				int g_index;
				for( int n=0; n<MAX_GUILD; n++)
				{				
					if( lpNode->Index[n] >= 0 )
					{
						g_index = lpNode->Index[n];
						if( g_index >= 0 )
						{
							LogAdd(lMsg.Get(447),pId);
							CloseClient ( g_index );
						}
					}
				}
			}
		}
		break;
	//------------------------------------------------------------------------
	// 강제 위치 이동
	case GMCMD_SETPOSITION : 	
//		if( (lpObj->AuthorityCode&AUTHORITY_CODE_SETPOSITION) == AUTHORITY_CODE_SETPOSITION) 
		{
			pId = GetTokenString();
			if( pId != NULL )
			{
				int m_number=-1, overLevel=0, subMoney=0;
				if( lpObj->Teleport ) 
				{
					GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);	//이동할수 없습니다
					//GCServerMsgStringSend("상대가 이동중 입니다", aIndex, 1);
					return 0;
				}

				if( lpObj->m_IfState.use ) 
				{
					if( lpObj->m_IfState.type == I_SHOP )	// 상점 거래중이었다면..
					{
						lpObj->TargetShopNumber = -1;	// 상점거래를 취소한다.
						lpObj->m_IfState.type   = 0;
						lpObj->m_IfState.use    = 0;
					}
				}
				// 거래중 이거나 상점 이용 중 일때는 사용할 수 없다.
				if( lpObj->m_IfState.use > 0 )
				{
					GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);	//이동할수 없습니다
					return 0;
				}

				if( gObj[aIndex].IsInBattleGround )
				{
					GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);	//이동할수 없습니다
					return 0;
				}
				
#ifdef UPDATE_PK_SYSTEM_20070302		// 레벨이 2차 이상이면 이동 안됨
				BOOL	PKFlag = FALSE;
				if(gObj[aIndex].PartyNumber >= 0)	// 파티에 속할경우
				{
					if( gParty.GetPKPartyPenalty(gObj[aIndex].PartyNumber) >= PK_LEVEL_DEFAULT+2)
					{
						PKFlag = TRUE;
					}
				}
				else
				{
					if(gObj[aIndex].m_PK_Level >= PK_LEVEL_DEFAULT+2)
					{
						PKFlag = TRUE;
					}
				}

				if(PKFlag == TRUE)
#else	// UPDATE_PK_SYSTEM_20070302

				if( lpObj->m_PK_Level >= 6 
//#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324				// 무법자도 이동 가능
//					&& !gPkLimitFree
//#endif
					)
#endif	// UPDATE_PK_SYSTEM_20070302
				{
					// 살인마는 이동안됨					
					//ChatTargetSend(, lMsg.Get(1125), lpObj->m_Index);
					GCServerMsgStringSend(lMsg.Get(1125), lpObj->m_Index, 1);
					return 0;
				}
				else
				{
	#ifdef ADD_MOVE_COMMAND_SCRIPT
		#ifndef ADD_MAPMOVE_PROTOCOL_20090327			// !! NOT
					gMoveCommand.Move(lpObj, pId);
		#endif // ADD_MAPMOVE_PROTOCOL_20090327
	#else
					
					m_number   = -1;
					overLevel  = 0;
					subMoney   = 2000;

					if( strcmp(pId, lMsg.Get(2000)) == 0 || 
						_stricmp(pId, lMsg.Get(2020)) == 0)	// 로랜시아
					{
						m_number = 17;
						overLevel = GetMapMoveLevel(lpObj, 0, 50);

					}
					else if( strcmp(pId, lMsg.Get(2001)) == 0 || 
							 _stricmp(pId, lMsg.Get(2021)) == 0 )	// 던전
					{
						m_number = 6;
						overLevel = GetMapMoveLevel(lpObj, 1, 50);
					}
					else if( strcmp(pId, lMsg.Get(2010)) == 0 || 
							 _stricmp(pId, lMsg.Get(2010)) == 0 )	// 던전3
					{
						m_number = 10;
						subMoney = 3000;
						overLevel = GetMapMoveLevel(lpObj, 1, 60);
					}
					else if( strcmp(pId, lMsg.Get(2002)) == 0 || 
							 _stricmp(pId, lMsg.Get(2022)) == 0 )	// 데비아스
					{
						m_number = 22;
						overLevel = GetMapMoveLevel(lpObj, 2, 50);
					}
					else if( strcmp(pId, lMsg.Get(2003)) == 0 || 
							 _stricmp(pId, lMsg.Get(2023)) == 0 )	// 노리아
					{
						m_number = 27;
						overLevel = GetMapMoveLevel(lpObj, 3, 50);
					}
					else if( strcmp(pId, lMsg.Get(2004)) == 0 || 
							 _stricmp(pId, lMsg.Get(2024)) == 0 ) // 로스트타워
					{
						m_number = 42;
						subMoney = 5000;

						overLevel = GetMapMoveLevel(lpObj, 4, 90);
					}
					else if( strcmp(pId, lMsg.Get(2007)) == 0 || 
						     _stricmp(pId, lMsg.Get(2027)) == 0 )	// 로스트타워3 층
					{
						m_number = 33;
						subMoney = 5000;

						overLevel = GetMapMoveLevel(lpObj, 4, 100);
					}
					else if( strcmp(pId, lMsg.Get(2011)) == 0 || 
						     _stricmp(pId, lMsg.Get(2031)) == 0 )	// 로스트타워5 층
					{
						m_number = 37;
						subMoney = 5000;

						overLevel = GetMapMoveLevel(lpObj, 4, 110);
					}
					else if( strcmp(pId, lMsg.Get(2012)) == 0 || 
						     _stricmp(pId, lMsg.Get(2032)) == 0 )	// 로스트타워7 층
					{
						m_number = 41;
						subMoney = 8000;

						overLevel = GetMapMoveLevel(lpObj, 4, 120);
					}
					else if( strcmp(pId, lMsg.Get(2006)) == 0 || 
						     _stricmp(pId, lMsg.Get(2026)) == 0 )	// 경기장
					{
						m_number = 50;
						overLevel = GetMapMoveLevel(lpObj, 6, 50);

#ifdef FOR_ONAIR	// 방송용일때 경기장 안으로 이동
						gObjTeleport(lpObj->m_Index, 6, 60+rand()%10, 153+rand()%6);
#endif

					}
					else if( strcmp(pId, lMsg.Get(2005)) == 0 || 
							 _stricmp(pId, lMsg.Get(2025)) == 0 )	// 아틀란스 
					{
						m_number = 49;
						subMoney = 5000;

						overLevel = GetMapMoveLevel(lpObj, 7, 70);
						
						// 아틀란스 일경우 유니리아 타고 있으면 이동 안된다.
						if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem())
						{							
							if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) )
							{
								GCServerMsgStringSend(lMsg.Get(702), lpObj->m_Index, 1);
								return 0;
							}
						}

					}
 					else if( strcmp(pId, lMsg.Get(2008)) == 0 || 
							 _stricmp(pId, lMsg.Get(2028)) == 0 )	// 타르칸
					{
						m_number = 57;
						subMoney = 5000;

						overLevel = GetMapMoveLevel(lpObj, 8, 140);
					}
					else if( strcmp(pId, lMsg.Get(2013)) == 0 || 
							 _stricmp(pId, lMsg.Get(2033)) == 0 )	// 이카루스
					{
						m_number = 62;
						subMoney = 15000;
						overLevel = GetMapMoveLevel(lpObj, 8, 170);
						
						if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3) && 
							( lpObj->pInventory[EQUIPMENT_WING].m_Type < MAKE_ITEMNUM(12,0) || 
							  lpObj->pInventory[EQUIPMENT_WING].m_Type > MAKE_ITEMNUM(12,6) ) )
						{
							GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);
							return 0;
						}

						if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) )
						{	// 유니리아를 타고는 이동할수 없다
							GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);
							return 0;
						}

						if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) || 
							lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) )
						{	// 변신반지를 차고는 이동할수 없다
							GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);
							return 0;
						}
					}
					
#if (TESTSERVER == 1) || defined(FOR_TEMP_SERVER)
#ifdef FOR_BLOODCASTLE
					else if( strcmp(pId, "블러드1") == 0 )			// 블러드캐슬1
					{
						m_number = 66;
						subMoney = 0;

						lpObj->m_cBloodCastleIndex = 0;
						lpObj->m_cBloodCastleSubIndex = -1;
						lpObj->m_iBloodCastleEXP = 0;

						overLevel = GetMapMoveLevel(lpObj, 11, 0);
					}
					else if( strcmp(pId, "블러드2") == 0 )			// 블러드캐슬2
					{
						m_number = 67;
						subMoney = 0;

						lpObj->m_cBloodCastleIndex = 1;
						lpObj->m_cBloodCastleSubIndex = -1;
						lpObj->m_iBloodCastleEXP = 0;

						overLevel = GetMapMoveLevel(lpObj, 12, 0);
					}
					else if( strcmp(pId, "블러드3") == 0 )			// 블러드캐슬3
					{
						m_number = 68;
						subMoney = 0;

						lpObj->m_cBloodCastleIndex = 2;
						lpObj->m_cBloodCastleSubIndex = -1;
						lpObj->m_iBloodCastleEXP = 0;

						overLevel = GetMapMoveLevel(lpObj, 13, 0);
					}
					else if( strcmp(pId, "블러드4") == 0 )			// 블러드캐슬4
					{
						m_number = 69;
						subMoney = 0;

						lpObj->m_cBloodCastleIndex = 3;
						lpObj->m_cBloodCastleSubIndex = -1;
						lpObj->m_iBloodCastleEXP = 0;

						overLevel = GetMapMoveLevel(lpObj, 14, 0);
					}
					else if( strcmp(pId, "블러드5") == 0 )			// 블러드캐슬5
					{
						m_number = 70;
						subMoney = 0;

						lpObj->m_cBloodCastleIndex = 4;
						lpObj->m_cBloodCastleSubIndex = -1;
						lpObj->m_iBloodCastleEXP = 0;

						overLevel = GetMapMoveLevel(lpObj, 15, 0);
					}
					else if( strcmp(pId, "블러드6") == 0 )			// 블러드캐슬6
					{
						m_number = 71;
						subMoney = 0;

						lpObj->m_cBloodCastleIndex = 5;
						lpObj->m_cBloodCastleSubIndex = -1;
						lpObj->m_iBloodCastleEXP = 0;

						overLevel = GetMapMoveLevel(lpObj, 16, 0);
					}
#ifdef BLOODCASTLE_EXTEND_20040314			// 블러드캐슬 확장으로 인해 /이동 으로 갈 수 있는 맵 추가
					else if( strcmp(pId, "블러드7") == 0 )			// 블러드캐슬6
					{
						m_number = 80;
						subMoney = 0;

						lpObj->m_cBloodCastleIndex = 6;
						lpObj->m_cBloodCastleSubIndex = -1;
						lpObj->m_iBloodCastleEXP = 0;

						overLevel = GetMapMoveLevel(lpObj, 17, 0);
					}
#endif
#endif					
					else if( strcmp(pId, "광장1") == 0 )	// 악마의 광장
					{
						m_number = 58;
						subMoney = 0;

						lpObj->m_nEventExp = 0;
						lpObj->m_nEventScore = 0;
						lpObj->m_nEventMoney = 0;
						lpObj->m_bDevilSquareIndex = 0;

						overLevel = GetMapMoveLevel(lpObj, 9, 20);
					}
					else if( strcmp(pId, "광장2") == 0 )	// 악마의 광장
					{
						m_number = 59;
						subMoney = 0;

						lpObj->m_nEventExp = 0;
						lpObj->m_nEventScore = 0;
						lpObj->m_nEventMoney = 0;
						lpObj->m_bDevilSquareIndex = 1;

						overLevel = GetMapMoveLevel(lpObj, 9, 20);
					}
					else if( strcmp(pId, "광장3") == 0 )	// 악마의 광장
					{
						m_number = 60;
						subMoney = 0;

						lpObj->m_nEventExp = 0;
						lpObj->m_nEventScore = 0;
						lpObj->m_nEventMoney = 0;
						lpObj->m_bDevilSquareIndex = 2;

						overLevel = GetMapMoveLevel(lpObj, 9, 20);
					}
					else if( strcmp(pId, "광장4") == 0 )	// 악마의 광장
					{
						m_number = 61;
						subMoney = 0;

						lpObj->m_nEventExp = 0;
						lpObj->m_nEventScore = 0;
						lpObj->m_nEventMoney = 0;
						lpObj->m_bDevilSquareIndex = 3;

						overLevel = GetMapMoveLevel(lpObj, 9, 20);
					}				
#ifdef DEVILSQUARE_EXTEND_20050221
					else if( strcmp(pId, "광장5") == 0 )	// 악마의 광장
					{
						m_number = 111;
						subMoney = 0;

						lpObj->m_nEventExp = 0;
						lpObj->m_nEventScore = 0;
						lpObj->m_nEventMoney = 0;
						lpObj->m_bDevilSquareIndex = 4;

						overLevel = GetMapMoveLevel(lpObj, 32, 20);
					}
					else if( strcmp(pId, "광장6") == 0 )	// 악마의 광장
					{
						m_number = 112;
						subMoney = 0;

						lpObj->m_nEventExp = 0;
						lpObj->m_nEventScore = 0;
						lpObj->m_nEventMoney = 0;
						lpObj->m_bDevilSquareIndex = 5;

						overLevel = GetMapMoveLevel(lpObj, 32, 20);
					}
#endif				
#endif

					// 내부접속용일때는 이동시 젠 안깎이게


#ifdef FOR_ONAIR
					subMoney = 0;
#endif
#if (TESTSERVER == 1) || defined(FOR_TEMP_SERVER)
					subMoney = 0;
#endif
					if( m_number >= 0 )
					{
						if( overLevel <= lpObj->Level )
						{							
							if( lpObj->Money-subMoney >= 0 ) 
							{
								if( gObjMoveGate(lpObj->m_Index, m_number) )
								{
									lpObj->Money -= subMoney;
									LogAddTD("[%s][%s] Instant move command reduces money %d - %d [%d]", 
										    lpObj->AccountID, lpObj->Name, lpObj->Money, subMoney,
											lpObj->MapNumber); //"[%s][%s] 이동 명령어 사용 돈 감소 %d - %d"
									GCMoneySend(lpObj->m_Index, lpObj->Money);
								}
								else
								{
									LogAddTD("[%s][%s] Instant move command fail m_number = %d", lpObj->AccountID, lpObj->Name, m_number); //"[%s][%s] 이동 명령어 실패
								}
							}
							else
							{
								GCServerMsgStringSend(lMsg.Get(1126), lpObj->m_Index, 1);
							}
						}
						else 
						{
							char szTemp[256];
							wsprintf(szTemp, lMsg.Get(1117), overLevel);
							GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
							return 0;
						}
					}
	#endif
				}
			}
		}
		break;

	case GMCMD_SETPOSITION2 :
		if(CheckAuthority(AUTHORITY_ADMIN | AUTHORITY_EVENT_GM, lpObj) == true)
		{
			pId = GetTokenString();
			int map, x, y;
			if( pId == NULL ) return 0;
			map = GetTokenNumber();
			x   = GetTokenNumber();
			y   = GetTokenNumber();
			int aIndex = gObjGetIndex(pId);
			if( aIndex >= 0 )
			{
				//## ADD_LOG_GM_COMMAND_20050426
#ifdef ADD_LOG_GM_COMMAND_20040420
				LPOBJECTSTRUCT	lpTargetObj = gObjFind(pId);

				if( lpTargetObj == NULL ) return 0;

				LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s", 
					lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, 
					lpTargetObj->AccountID, lpTargetObj->Name, "User SetPosition");
#endif
				gObjTeleport(aIndex, map, x, y);
			}
		}
		break;

	case GMCMD_GUILDSETPOSITION :
//		if( (lpObj->AuthorityCode&AUTHORITY_CODE_SETPOSITION) != AUTHORITY_CODE_SETPOSITION) return 0;
		{
			
	#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "Guild SetPosition");
	#endif

		pId = GetTokenString();
		int map, x, y;
		if( pId == NULL ) return 0;
		
		map = GetTokenNumber();
		x   = GetTokenNumber();
		y   = GetTokenNumber();
		
		LPGUILD_INFO lpNode = Guild.SearchGuild(pId);
		if( lpNode != NULL )
		{
			int g_index;
			for( int n=0; n<MAX_GUILD; n++)
			{				
				if( lpNode->Index[n] >= 0 )
				{
					g_index = lpNode->Index[n];
					gObjTeleport(g_index, map, x++, y);
				}
			}
		}
		}		
		break;
	case GMCMD_GUILDWARSTART :	// 경기 시작
		if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
		{
	#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "Start BattleSoccer");
	#endif
			BattleSoccerGoalStart(0);
		}
		break;
	case GMCMD_GUILDWARSTOP :
		if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
		{
	#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "Stop BattleSoccer");
	#endif
			BattleSoccerGoalEnd(0);
		}
		else
		{
			// 길드 마스터 일때만 게임을 잠시 중단할 수 있다.
			if( gObj[aIndex].lpGuild )
			{
				// 전투 축구 중인가?
				if( gObj[aIndex].lpGuild->WarType == 1 )
				{
					if( strcmp( gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0] ) == 0 )
					{					
						if( gObj[aIndex].lpGuild->BattleGroundIndex >= 0 )
						{
							//BattleSoccerGoalEnd(gObj[aIndex].lpGuild->BattleGroundIndex);
							//GCServerMsgStringSend("GuildMaster 요청으로 3초 후 경기가 자동으로 종료됩니다.", aIndex, 0);
						}
					}
				}
			}			
		}
		break;
	case GMCMD_GUILDWAREND :
		if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
		{
	#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
				lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "End GuildWar");
	#endif
			char *pId;
			pId = GetTokenString();
			if( pId != NULL ) 
			{
				GCManagerGuildWarEnd(pId);
			}
		}
		else
		{
			// 길드 마스터 일때만 게임을 중단할 수 있다.
			if( gObj[aIndex].lpGuild && gObj[aIndex].lpGuild->lpTargetGuildNode )
			{
				if( strcmp( gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0] ) == 0 )
				{
					if( gObj[aIndex].lpGuild->BattleGroundIndex >= 0 && gObj[aIndex].lpGuild->WarType == 1)
					{
						gObjAddMsgSendDelay(&gObj[aIndex], MSG_BSSTOP, aIndex, 1000*10);
						
						char szTemp[100];

						wsprintf(szTemp, lMsg.Get(1153), gObj[aIndex].lpGuild->Names[0]);
						GCServerMsgStringSendGuild(gObj[aIndex].lpGuild, szTemp, 1);
						GCServerMsgStringSendGuild(gObj[aIndex].lpGuild->lpTargetGuildNode, szTemp, 1);
					}
				}
			}			
		}
		break;
	//------------------------------------------------------------------------
	// 캐릭터 복제
	case GMCMD_COPYCHAR	: 
		break;
	//------------------------------------------------------------------------
	// 계정 블럭
	case GMCMD_ACCOUNTBLOCK : 
		/*if( (lpObj->AuthorityCode&AUTHORITY_CODE_ACCOUNTBLOCK) != AUTHORITY_CODE_ACCOUNTBLOCK) return 0;
		pId = GetTokenString();
		{
		int aIndex = gObjGetIndex(pId);
		if( aIndex >= 0 )
		{
			GJPAccountBlock(aIndex, gObj[aIndex].AccountID, gObj[aIndex].DBNumber,  gObj[aIndex].UserNumber, 1);
		}
		}*/
		break;
	//------------------------------------------------------------------------
	// 계정 블럭 해지
	case GMCMD_ACCOUNTBLOCKFREE : 
		/*if( (lpObj->AuthorityCode&AUTHORITY_CODE_ACCOUNTBLOCK) != AUTHORITY_CODE_ACCOUNTBLOCK) return 0;
		pId = GetTokenString();
		{
		int aIndex = gObjGetIndex(pId);
		if( aIndex >= 0 )
		{
			GJPAccountBlock(aIndex, gObj[aIndex].AccountID, gObj[aIndex].DBNumber,  gObj[aIndex].UserNumber, 0);
		}
		}*/
		break;
	//------------------------------------------------------------------------
	// 채팅 금지
	case GMCMD_CHATBAN :
//		if( (lpObj->AuthorityCode&AUTHORITY_CODE_CHATBAN) != AUTHORITY_CODE_CHATBAN) return 0;

	#ifdef ADD_LOG_GM_COMMAND_20040420
		LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "Ban Chatting");
	#endif
		pId = GetTokenString();
		
		if( pId == NULL ) return 0;

		{
		int aIndex = gObjGetIndex(pId);
		if( aIndex >= 0 )
		{
			gObj[aIndex].Penalty |= CTLCODE_CHATBAN;
		}
		}
		break;	
	//------------------------------------------------------------------------
	// 채팅 금지 해지
	case GMCMD_CHATBANFREE :
//		if( (lpObj->AuthorityCode&AUTHORITY_CODE_CHATBAN) != AUTHORITY_CODE_CHATBAN) return 0;

	#ifdef ADD_LOG_GM_COMMAND_20040420
		LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "Free Ban-Chatting");
	#endif
		pId = GetTokenString();

		if( pId == NULL ) return 0;

		{
		int aIndex = gObjGetIndex(pId);
		if( aIndex >= 0 )
		{
			gObj[aIndex].Penalty &= (~CTLCODE_CHATBAN);
		}
		}
		break;
		
	//------------------------------------------------------------------------
	// 길드 전쟁
	case UMCMD_GUILDWAR :
		pId = GetTokenString();
		if( pId != NULL ) 
		{
			if( strlen(pId) >= 1 ) GCGuildWarRequestResult(pId, aIndex, 0);
		}
		break;
	//------------------------------------------------------------------------
	// 길드의 전투 축구
	case UMCMD_BATTLESOCCERWAR :
		if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
		{
		#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "Set GuildWar");
		#endif
			char *pId2;

			pId = GetTokenString();
			if( pId != NULL ) 
			{
				pId2 = GetTokenString();
				if( pId2 != NULL )
				{
					if( strlen(pId) >= 1 && strlen(pId2) >= 1 ) 
					{
						GCManagerGuildWarSet(pId, pId2, 1);
					}
				}
			}
		}
		else
		{
			if( gEnableBattleSoccer )
			{
				pId = GetTokenString();
				if( pId != NULL ) 
				{
					if( strlen(pId) >= 1 ) GCGuildWarRequestResult(pId, aIndex, 1);
				}
			}
		}
		break;
	//------------------------------------------------------------------------
	// 정액/종량 남은 시간 알아보기
	case UMCMD_BILLCHECK :
		gObjBillRequest(&gObj[aIndex]);

		if(CheckAuthority(AUTHORITY_SUPER_ADMIN, &gObj[aIndex]) == false) break;
		
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
// #if (TESTSERVER == 1) || defined(FOR_TEMP_SERVER)
		if( g_bVtmPlayTimeLimitOn == TRUE )
		{
			INT iCalcRestTime = (g_iVtmPlayTimeLimitLevel2Time*60)-gObj[aIndex].m_iPlayRestTime;

			if (iCalcRestTime < 0)	
				iCalcRestTime = 0;

			PMSG_NOTICE	pNotice;
			CHAR szNotice[256] = {0,};
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_TYPE_BOTTOM, TNOTICE_COLOR_DEFAULT);

			wsprintf(szNotice, "PlayTime(%02d:%02d), To ResetTime(%02d:%02d)",					
				gObj[aIndex].m_iPlayTimeLimit/(60*60),	
				(gObj[aIndex].m_iPlayTimeLimit%(60*60))/60,
				iCalcRestTime/(60*60),
				(iCalcRestTime%(60*60))/60
				);

			TNotice::MakeNoticeMsgEx( &pNotice, TNOTICE_TYPE_BOTTOM, szNotice );
			TNotice::SendNoticeToUser( aIndex, &pNotice );
		}
// #endif // TESTSERVER == 1
#endif // VTM_PLAYTIME_LIMIT_SYSTEM_20060626
		
		break;	
	
	case UMCMD_TRADEOPTION :
		{
		int op;
		pId = GetTokenString();
		if( pId != NULL )
		{
			if( strcmp(pId, "on")==0) op = 1;
			else if( strcmp(pId, "off")==0) op = 0;
			if( op >= 0 && op <= 1 )
			{
				gObjSetTradeOption(aIndex, op);
#ifdef DUEL_SYSTEM_20031028				
				gObjSetDuelOption(aIndex, op);
#endif
			}
		}
		}
		break;


#ifdef ADD_KUNDUN_CONTROL_20040923
	//------------------------------------------------------------------------
	// 쿤둔 상태 보기
	case GMCMD_NOTIFYKUNDUNHP :
		{
/*
#if (TESTSERVER==0)
			if( (lpObj->Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
				return 0;
#endif
*/

#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔상태보기");

			KUNDUN_GM_LOG.Output( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔상태보기" );
#endif

			for( int count=0; count < MAXVIEWPORTOBJECT; count++ )
			{
				if( lpObj->VpPlayer[count].state )
				{
					if( lpObj->VpPlayer[count].type == OBJTYPE_MONSTER )
					{
						if( lpObj->VpPlayer[count].number >= 0 )
						{
							LPOBJECTSTRUCT lpTargetObj = &gObj[lpObj->VpPlayer[count].number];
							
							if( lpTargetObj->Class == 275 )
							{
								TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
								_Notice.SendToUser( lpObj->m_Index, 
									"쿤둔 HP = %7.0f / %7.0f", lpTargetObj->Life,lpTargetObj->MaxLife );

								_Notice.SendToUser( lpObj->m_Index, 
									"쿤둔 HP 초당회복량 = %d 회복량 = %d 회복시간 = %d", 
									giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime );
							}
						}
					}
					
				}
			}
			
		}
		break;
	//------------------------------------------------------------------------
	// 쿤둔 HP 설정
	case GMCMD_SETKUNDUNHP :
		{
/*
#if (TESTSERVER==0)
			if( (lpObj->Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
				return 0;
#endif
*/
//------------->
// Admin, EventGm 권한 통합 사용 20081215 (grooving)
#ifdef MODIFIY_AUTHORITY_INTERGRATION_20081215
			if(CheckAuthority(AUTHORITY_SUPER_ADMIN | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM, &gObj[aIndex]) == false) break;
#else // MODIFIY_AUTHORITY_INTERGRATION_20081215
			if(CheckAuthority(AUTHORITY_SUPER_ADMIN | AUTHORITY_ADMIN, &gObj[aIndex]) == false) break;
#endif// MODIFIY_AUTHORITY_INTERGRATION_20081215
//<-------------
			
#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔HP설정");

			KUNDUN_GM_LOG.Output( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔HP설정");
#endif

			int iKundunHP = GetTokenNumber();

			for( int count=0; count < MAXVIEWPORTOBJECT; count++ )
			{
				if( lpObj->VpPlayer[count].state )
				{
					if( lpObj->VpPlayer[count].type == OBJTYPE_MONSTER )
					{
						if( lpObj->VpPlayer[count].number >= 0 )
						{
							LPOBJECTSTRUCT lpTargetObj = &gObj[lpObj->VpPlayer[count].number];
							
							if( lpTargetObj->Class == 275 )
							{
								if( iKundunHP <= 5000 )
									iKundunHP = 5000;

								if( iKundunHP > lpTargetObj->MaxLife )
									iKundunHP = lpTargetObj->MaxLife;

								lpTargetObj->Life = iKundunHP;
							
								TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
								_Notice.SendToUser( lpObj->m_Index, 
									"쿤둔 HP = %7.0f / %7.0f", lpTargetObj->Life,lpTargetObj->MaxLife );
							}
						}
					}
					
				}
			}

		}
		break;
	//------------------------------------------------------------------------
	// 쿤둔 피 회복량 설정
	case GMCMD_SETKUNDUNREFILLHP:
		{
/*
#if (TESTSERVER==0)
			if( (lpObj->Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
				return 0;
#endif
*/

#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔HP회복량설정");
			
			KUNDUN_GM_LOG.Output( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔HP회복량설정");
#endif
			
			int iKundunHP = GetTokenNumber();
			
			if( iKundunHP <= 0 || iKundunHP > 5000000 )	
				return 0;

			giKundunRefillHP = iKundunHP;

			TNotice _Notice;
			_Notice.SendToUser( lpObj->m_Index, "쿤둔 HP 초당회복량 = %d 회복량 = %d 회복시간 = %d", 
				giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime );
		}
		break;
	//------------------------------------------------------------------------
	// 쿤둔 피 초당 회복량 설정
	case GMCMD_SETKUNDUNREFILLHPSEC:
		{
/*
#if (TESTSERVER==0)
			if( (lpObj->Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
				return 0;
#endif
*/			
#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔HP초당회복량설정");

			KUNDUN_GM_LOG.Output( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔HP초당회복량설정");
#endif

			int iKundunHPSec = GetTokenNumber();
			
			if( iKundunHPSec <= 0 || iKundunHPSec > 10000 )	
				return 0;

			giKundunRefillHPSec = iKundunHPSec;

			TNotice _Notice;
			_Notice.SendToUser( lpObj->m_Index, "쿤둔 HP 초당회복량 = %d 회복량 = %d 회복시간 = %d", 
				giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime );

		}
		break;

	//------------------------------------------------------------------------
	// 쿤둔 피 회복시간 설정
	case GMCMD_SETKUNDUNREFILLHPTIME:
		{
/*
#if (TESTSERVER==0)
			if( (lpObj->Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
				return 0;
#endif
*/

#ifdef ADD_LOG_GM_COMMAND_20040420
			LogAddTD( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔HP회복시간설정");
			
			KUNDUN_GM_LOG.Output( "[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", 
			lpObj->Ip_addr, lpObj->AccountID, lpObj->Name, "쿤둔HP회복시간설정");
#endif
			
			int iKundunHPTime = GetTokenNumber();
			
			if( iKundunHPTime < 0 || iKundunHPTime > 60000 )	
				return 0;

			giKundunRefillHPTime = iKundunHPTime;

			TNotice _Notice;
			_Notice.SendToUser( lpObj->m_Index, "쿤둔 HP 초당회복량 = %d 회복량 = %d 회복시간 = %d", 
				giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime );
		}
		break;
#endif //## ADD_KUNDUN_CONTROL_20040923


#ifdef CASTLE_GM_COMMAND_20041222				// 운영자 공성제어 명령처리
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	case GMCMD_CS_CHANGEOWNERGUILD :			// "/공성수성길드"
		{
			LPSTR lpszGuildName		= GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, CS_GMCMD_CHANGEOWNERGUILD, lpszGuildName);
		}
		break;
	case GMCMD_CS_SETSTATE_REGSIEGE :			// "/공성참여등록"
		g_CastleSiege.OperateGmCommand(lpObj->m_Index, CS_GMCMD_SETSTATE_REGSIEGE);
		break;
	case GMCMD_CS_SETSTATE_REGMARK :			// "/공성문장등록"
		g_CastleSiege.OperateGmCommand(lpObj->m_Index, CS_GMCMD_SETSTATE_REGMARK);
		break;
	case GMCMD_CS_SETSTATE_NOTIFY :				// "/공성선포"
		g_CastleSiege.OperateGmCommand(lpObj->m_Index, CS_GMCMD_SETSTATE_NOTIFY);
		break;
	case GMCMD_CS_SETSTATE_STARTSIEGE :			// "/공성시작"
		g_CastleSiege.OperateGmCommand(lpObj->m_Index, CS_GMCMD_SETSTATE_STARTSIEGE);
		break;
	case GMCMD_CS_SETSTATE_ENDSIEGE :			// "/공성종료"
		g_CastleSiege.OperateGmCommand(lpObj->m_Index, CS_GMCMD_SETSTATE_ENDSIEGE);
		break;
	case GMCMD_CS_SETSTATE_GETOWNERGUILD :		// "공성현재수성"
		g_CastleSiege.OperateGmCommand(lpObj->m_Index, CS_GMCMD_SETSTATE_GETOWNERGUILD);
		break;
	case GMCMD_CS_SETSTATE_GETCASTLESTATE :		// "공성현재상태"
		g_CastleSiege.OperateGmCommand(lpObj->m_Index, CS_GMCMD_SETSTATE_GETCASTLESTATE);
		break;
	case GMCMD_CS_CHANGECSSIDE :				// "공수교체"
		{
			LPSTR lpszGuildName		= GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, CS_GMCMD_SETSTATE_CHANGECSSIDE, lpszGuildName);
		}
		break;
#endif
#endif

#ifdef CRYWOLF_GM_COMMAND_20050525	// 크라이울프 명령어
	case GMCMD_CRYWOLF_READY :		// "크라이울프준비"
		g_Crywolf.OperateGmCommand( lpObj->m_Index, CRYWOLF_GM_COMMAND_READY);
		break;
	case GMCMD_CRYWOLF_START :		// "크라이울프시작"
		g_Crywolf.OperateGmCommand( lpObj->m_Index, CRYWOLF_GM_COMMAND_START);
		break;
	case GMCMD_CRYWOLF_END	:		// "크라이울프종료"
		g_Crywolf.OperateGmCommand( lpObj->m_Index, CRYWOLF_GM_COMMAND_END);
		break;
	case GMCMD_CRYWOLF_NOTIFY_2 :
		g_Crywolf.OperateGmCommand( lpObj->m_Index, CRYWOLF_GM_COMMAND_NOTIFY_2);
		break;
#endif

#ifdef KANTURU_GM_COMMAND_20060709
	case GMCMD_KANTURU_STANBY:			// "/칸투르대기상태"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_STANBY );
		break;
	case GMCMD_KANTURU_BATTLEOFMAYA:	// "/칸투르마야전"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_BATTLEOFMAYA );
		break;		
	case GMCMD_KANTURU_MAYA_MONSTER1:	// "/마야몬스터1"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_MAYA_MONSTER1 );
		break;
	case GMCMD_KANTURU_MAYA1:			// "/마야1"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_MAYA1 );
		break;
	case GMCMD_KANTURU_MAYA_MONSTER2:	// "/몬스터2"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_MAYA_MONSTER2 );
		break;		
	case GMCMD_KANTURU_MAYA2:			// "/마야2"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_MAYA2 );
		break;
	case GMCMD_KANTURU_MAYA_MONSTER3:	// "/마야몬스터3"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_MAYA_MONSTER3 );
		break;
	case GMCMD_KANTURU_MAYA3:			// "/마야3"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_MAYA3 );
		break;
	case GMCMD_KANTURU_MAYA3_SUCCESS:	// "/마야3성공"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_MAYA3_SUCCESS );
		break;
	case GMCMD_KANTURU_MAYA_SUCCESS:	// "/마야성공"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_MAYA_SUCCESS );
		break;	
	case GMCMD_KANTURU_MAYA_FAIL:		// "/마야실패"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_MAYA_FAIL );
		break;		
	case GMCMD_KANTURU_BATTLEOFNIGHTMARE:			// "/칸투르나이트메어전"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_NIGHTMARE );
		break;
	case GMCMD_KANTURU_BATTLEOFNIGHTMARE_SUCCESS:	// "/나이트메어성공"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_NIGHTMARE_SUCCESS );
		break;
	case GMCMD_KANTURU_BATTLEOFNIGHTMARE_FAIL:		// "/나이트메어실패"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_NIGHTMARE_FAIL );
		break;		
	case GMCMD_KANTURU_TOWEROFREFINEMENT:			// "/칸투르정제의탑"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_TOWEROFREFINEMENT );
		break;
	case GMCMD_KANTURU_TOWER_OPEN:					// "/정제의탑열기"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_TOWER_OPEN );
		break;
	case GMCMD_KANTURU_TOWER_CLOSE:					// "/정제의탑닫기"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_TOWER_CLOSE );
		break;
	case GMCMD_KANTURU_END:							// "/칸투르종료"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_END );
		break;
	case GMCMD_KANTURU_NEXT_STATE:					// "칸투르다음상태"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_NEXT_STATE );
		break;
	case GMCMD_KANTURU_NEXT_DETAIL_STATE:			// "칸투르다음세부상태"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_NEXT_DETAIL_STATE );
		break;
	case GMCMD_KANTURU_CURRENT_STATE:				// "칸투르현재상태"
		g_Kanturu.OperateGmCommand( lpObj->m_Index, KANTURU_GM_COMMAND_CURRENT_STATE );
		break;
#endif	// #ifdef KANTURU_GM_COMMAND_20060709

	case GMCMD_STATRESET :
		{	// 스탯 리셋
			int Class = lpObj->Class;

#ifdef NEW_SKILL_FORSKYLAND
			lpObj->ChangeUP = 0;
#endif
			//lpObj->Experience = 0;

			lpObj->Strength = DCInfo.DefClass[Class].Strength;
			lpObj->Dexterity = DCInfo.DefClass[Class].Dexterity;
			lpObj->Vitality = DCInfo.DefClass[Class].Vitality;
			lpObj->Energy = DCInfo.DefClass[Class].Energy;
			lpObj->Life = DCInfo.DefClass[Class].Life;
			lpObj->MaxLife = DCInfo.DefClass[Class].MaxLife;
			
			lpObj->Mana = DCInfo.DefClass[Class].Mana;
			lpObj->MaxMana = DCInfo.DefClass[Class].MaxMana;			

#ifdef DARKLORD_WORK
			lpObj->Leadership = DCInfo.DefClass[Class].Leadership;
#endif

#ifdef DARKLORD_WORK
			if (lpObj->Class == CLASS_DARKLORD||lpObj->Class == CLASS_MAGUMSA) 
#else			
			if( lpObj->Class == CLASS_MAGUMSA )
#endif
				lpObj->LevelUpPoint = (lpObj->Level-1)*7;
			else
				lpObj->LevelUpPoint = (lpObj->Level-1)*5;
			

			GCServerMsgStringSend("캐릭터 정보가 리셋되었습니다. 새로 연결 하세요", lpObj->m_Index, 1);
			gObjCloseSet(lpObj->m_Index, CL_BACK);			
		}
	break;

	case GMCMD_SETLEVEL:
		{
			int Class = lpObj->Class;

			int level= GetTokenNumber();

			if( level < 1 )
				level = 1;

			lpObj->Level = level;

			lpObj->Experience = gLevelExperience[lpObj->Level-1];
			
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
			lpObj->iFruitPoint = 0;
#endif
			// 레벨 수정후 스탯까지 조정한다
			ManagementProc(lpObj, "/스탯리셋", aIndex);
		}
	break;

#ifdef DARKLORD_WORK
	case GMCMD_DARKSPIRIT:
		{			
			int mode = GetTokenNumber();
			
			CDarkSpirit::ePetItem_Mode setmode = CDarkSpirit::PetItem_Mode_Normal;

			if( mode == CDarkSpirit::PetItem_Mode_Normal )			
				setmode = CDarkSpirit::PetItem_Mode_Normal;
			else if( mode == CDarkSpirit::PetItem_Mode_Attack_Random )
				setmode = CDarkSpirit::PetItem_Mode_Attack_Random;
			else if( mode == CDarkSpirit::PetItem_Mode_Attack_WithMaster )
				setmode = CDarkSpirit::PetItem_Mode_Attack_WithMaster;
			else if( mode == CDarkSpirit::PetItem_Mode_Attack_Target )
				setmode = CDarkSpirit::PetItem_Mode_Attack_Target;

			gDarkSpirit[lpObj->m_Index].SetMode(setmode);
		}
	break;

	case GMCMD_DARKSPIRIT_LEVEL:
		{
			int nPos = GetTokenNumber();
			int nLevel = GetTokenNumber();

			lpObj->pInventory[nPos].m_PetItem_Level = nLevel;

#ifdef ADD_380ITEM_NEWOPTION_20060711
			lpObj->pInventory[nPos].Convert(lpObj->pInventory[nPos].m_Type, 
		                              lpObj->pInventory[nPos].m_Option1, 
									  lpObj->pInventory[nPos].m_Option2,
									  lpObj->pInventory[nPos].m_Option3,
									  lpObj->pInventory[nPos].m_NewOption,
									  lpObj->pInventory[nPos].m_SetOption,
									  lpObj->pInventory[nPos].m_ItemOptionEx);
#else
			lpObj->pInventory[nPos].Convert(lpObj->pInventory[nPos].m_Type, 
		                              lpObj->pInventory[nPos].m_Option1, 
									  lpObj->pInventory[nPos].m_Option2,
									  lpObj->pInventory[nPos].m_Option3,
									  lpObj->pInventory[nPos].m_NewOption,
									  lpObj->pInventory[nPos].m_SetOption);
#endif // ADD_380ITEM_NEWOPTION_20060711

		}
		break;

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	
	case GMCMD_ILLUSION_TEMPLE_READY:
		g_IllusionTempleEvent.SetIllusionTempleState(ILLUSION_TEMPLE_STATE_READY, lpObj->MapNumber);
		break;
	case GMCMD_ILLUSION_TEMPLE_START:
		g_IllusionTempleEvent.SetIllusionTempleState(ILLUSION_TEMPLE_STATE_PLAYING, lpObj->MapNumber);
		break;
	case GMCMD_ILLUSION_TEMPLE_END:	
		g_IllusionTempleEvent.SetIllusionTempleState(ILLUSION_TEMPLE_STATE_END, lpObj->MapNumber);
		break;
	case GMCMD_ILLUSION_TEMPLE_RESET:
		break;
#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

	case GMCMD_QUESTRESET :
		{	// 퀘스트 리셋
			BYTE	QuestBitMask[8];
			QuestBitMask[0] = 0xFC;
			QuestBitMask[1] = 0xFC;
			QuestBitMask[2] = 0xF3;
			QuestBitMask[3] = 0xF3;

			QuestBitMask[4] = 0xCF;
			QuestBitMask[5] = 0xCF;
			QuestBitMask[6] = 0x3F;
			QuestBitMask[7] = 0x3F;

			int questindex= GetTokenNumber();
			if( questindex == 0 )
			{	// 모두 리셋				
				memset(lpObj->m_Quest, 0xff, MAX_DBQUEST);
				lpObj->m_SendQuestInfo = FALSE;
				CGRequestQuestInfo(lpObj->m_Index);
			}
			else
			{
				int State = QS_NOTHING;

				int index = questindex/4;
				int shift = (questindex%4)*2;
				
				(lpObj->m_Quest[index]) &= QuestBitMask[shift];
				(lpObj->m_Quest[index]) |= ((State&0x3)<<shift);
				lpObj->m_SendQuestInfo = FALSE;
				CGRequestQuestInfo(lpObj->m_Index);
			}

#ifdef MODIFY_QUEST_SYSTEM_20070525	// /퀘스트리셋 숫자 명령어 사용시 처리
			lpObj->m_iQuestMonKillDBIndex = -1;
			
			for( int i = 0; i < MAX_QUEST_MONSTER_KILL_INFO; i++ )
			{
				lpObj->m_QuestMonKillInfo[i].iQuestMonIndex	= -1;
				lpObj->m_QuestMonKillInfo[i].iKillCount		= -1;
			}				
#endif
		}
	break;

	case GMCMD_CHANGEPOS :
		{	// 좌표이동 (ex> /좌표이동 맵번호 X좌표 Y좌표)
			int map, x, y;
			map = GetTokenNumber();
			x   = GetTokenNumber();
			y   = GetTokenNumber();

			if (!CHECK_LIMIT(map, g_TerrainManager.Size()))
				break;
			if (!CHECK_LIMIT(x, 256))
				break;
			if (!CHECK_LIMIT(y, 256))
				break;

			BYTE btMapAttr = MapC[map].GetAttr(x, y);
#ifdef UPDATE_GM_FUNCTION_20070228		// '/좌표이동'시 안전지대 추가
			if( ((btMapAttr&MAP_ATTR_BLOCK)==MAP_ATTR_BLOCK) ||				// 벽
				((btMapAttr&MAP_ATTR_HOLLOW)==MAP_ATTR_HOLLOW)				// 땅뚫림
				)
#else
			if( ((btMapAttr&MAP_ATTR_SAFTYZONE)==MAP_ATTR_SAFTYZONE) ||		// 안전지대
				((btMapAttr&MAP_ATTR_BLOCK)==MAP_ATTR_BLOCK) ||				// 벽
				((btMapAttr&MAP_ATTR_HOLLOW)==MAP_ATTR_HOLLOW)				// 땅뚫림
				)
#endif
			{

				PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 0, "해당 지역으로 현재 좌표이동 할 수 없습니다. (속성코드:%d)", btMapAttr);
				TNotice::SendNoticeToUser( lpObj->m_Index, &pNotice );
#else
				pNotice.type = 0;
				wsprintf(pNotice.Notice, "해당 지역으로 현재 좌표이동 할 수 없습니다. (속성코드:%d)", btMapAttr);
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
				::DataSend(lpObj->m_Index, (LPBYTE)&pNotice, pNotice.h.size);
#endif

				break;
			}
#ifdef ADD_NEWPVP_PKFIELD
			{
				g_NewPVP.Reset(*lpObj);
			}
#endif // ADD_NEWPVP_PKFIELD
			
			gObjTeleport(lpObj->m_Index, map, x, y);
#ifdef ADD_RAKLION_20080408
			if( map == MAP_INDEX_RAKLION_BOSS )
				g_RaklionBattleUserMng.AddUserData( lpObj->m_Index );
#endif // ADD_RAKLION_20080408
		}
	break;	

#ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410
	case GMCMD_GETINFINITYARROW_MP:
		GetInfinityArrowMPConsumption( lpObj );
		break;
		
	case GMCMD_SETINFINITYARROW_MP_0:
		ControlInfinityArrowMPConsumption0( lpObj, GetTokenNumber() );
		break;

	case GMCMD_SETINFINITYARROW_MP_1:
		ControlInfinityArrowMPConsumption1( lpObj, GetTokenNumber() );
		break;

	case GMCMD_SETINFINITYARROW_MP_2:
		ControlInfinityArrowMPConsumption2( lpObj, GetTokenNumber() );
		break;

#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	case GMCMD_SETINFINITYARROW_MP_3:
		ControlInfinityArrowMPConsumption3( lpObj, GetTokenNumber() );
		break;
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
		
	case GMCMD_INFINITYARROW_SETTIME:
		SetInfinityArrowTime( lpObj, GetTokenNumber() );
		break;
#endif	// #ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410
		
#ifdef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018
	case GMCMD_LEVELUP_TEST:
#ifdef BUGFIX_GAIN_EXP_20071210
		{
			INT64 iGetExp = GetTokenNumber();
			gObjLevelUp( lpObj, iGetExp, 0, EVENT_TYPE_NONE );
		}
#else

	#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
		gObjLevelUp(lpObj, GetTokenNumber(), 0, EVENT_TYPE_NONE );
	#else
		gObjLevelUp(lpObj, GetTokenNumber() );
	#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
#endif	// BUGFIX_GAIN_EXP_20071210
		break;
#endif // MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	case GMCMD_FIRE_SCREAM:
		ControlFireScreamDoubleAttackDistance( lpObj, GetTokenNumber() );
		break;		
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	case GMCMD_MAKE_STRENGTHENITEM:
		{
			int iInvenPosX = GetTokenNumber();
			int iInvenPosY = GetTokenNumber();
			int iOptionType = GetTokenNumber();
			int iOptionLevel = GetTokenNumber();
			// 아이템
			int pos = (iInvenPosY-1)*8 + (iInvenPosX-1) + EQUIPMENT_GOLD;
			g_kJewelOfHarmonySystem.StrengthenItemByMacro( lpObj, pos, iOptionType, iOptionLevel );
		}
		break;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
	case GMCMD_MAKE_380OPTIONITEM:
		{
			int iInvenPosX = GetTokenNumber();
			int iInvenPosY = GetTokenNumber();
			int iOption    = GetTokenNumber();
			// 아이템
			int pos = (iInvenPosY-1)*8 + (iInvenPosX-1) + EQUIPMENT_GOLD;

			g_kItemSystemFor380.SetOptionItemByMacro( lpObj, pos, (BOOL)iOption);
		}
		break;
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	case GMCMD_LOTTERY_ITEM_CREATE:
		{
			CItem LotteryItem;

#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
			char szCardName[50];

			ZeroMemory( szCardName, sizeof(szCardName) );
			strcpy( szCardName, GetTokenString() );
			
			g_EventItemBagExMng.GetItem( szCardName, &LotteryItem );
			if( LotteryItem.IsItem() == FALSE )
			{
				break;
			}
#else
			if( g_CashLotterySystem.GetItem( &LotteryItem ) == -1 )
			{
				break;
			}
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y,
				LotteryItem.m_Type,
				LotteryItem.m_Level,
				LotteryItem.m_Durability,
				LotteryItem.m_Option1,
				LotteryItem.m_Option2,
				LotteryItem.m_Option3,
				lpObj->m_Index,
				LotteryItem.m_NewOption, 0 );
		}
		break;
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#endif

#ifdef MODIFY_INVEN_CLEAR_MACRO_20071212
	case GMCMD_INVENTORY_ITEM_DELETE:
		{
			int iInvenPosX = GetTokenNumber();
			int iInvenPosY = GetTokenNumber();
			// 아이템
			int pos = (iInvenPosY-1)*8 + (iInvenPosX-1) + EQUIPMENT_GOLD;

			gObjInventoryDeleteItem( lpObj->m_Index, pos );
			GCItemListSend( lpObj->m_Index );
		}
		break;
	case GMCMD_INVENTORY_ITEM_ALL_DELETE:
		{
#ifdef GMCOMMAND_EX_1ST_BUGFIX
			for( int i = MAX_EQUIPMENT ; i < MAX_INVENTORY ; i++ )
#else
			for( int i = MAX_EQUIPMENT ; i < MAX_INVENTORY - 1 ; i++ )
#endif // GMCOMMAND_EX_1ST_BUGFIX
			{
				if( lpObj->pInventory[i].IsItem() == TRUE )
				{
					gObjInventoryDeleteItem( lpObj->m_Index, i );
				}
			}

			GCItemListSend( lpObj->m_Index );
		}
		break;
	case GMCMD_DEVILSQUARE_SET_TIME:
//		int iTime;
//
//		iTime = GetTokenNumber();
//
//		#if TESTSERVER == 1
//		g_DevilSquare.SetRemainTime( iTime );
//		#endif
		break;
#endif // MODIFY_INVEN_CLEAR_MACRO_20071212



#if (TESTSERVER == 0)
	#ifdef ADD_MACRO_FOR_SETITEM_20060424
	case GMCMD_STATRESET :
		{	// 스탯 리셋
			int Class = lpObj->Class;

			lpObj->ChangeUP = 0;
			//lpObj->Experience = 0;

			lpObj->Strength = DCInfo.DefClass[Class].Strength;
			lpObj->Dexterity = DCInfo.DefClass[Class].Dexterity;
			lpObj->Vitality = DCInfo.DefClass[Class].Vitality;
			lpObj->Energy = DCInfo.DefClass[Class].Energy;
			lpObj->Life = DCInfo.DefClass[Class].Life;
			lpObj->MaxLife = DCInfo.DefClass[Class].MaxLife;
			
			lpObj->Mana = DCInfo.DefClass[Class].Mana;
			lpObj->MaxMana = DCInfo.DefClass[Class].MaxMana;

#ifdef DARKLORD_WORK
			lpObj->Leadership = DCInfo.DefClass[Class].Leadership;
#endif

#ifdef DARKLORD_WORK
			if (lpObj->Class == CLASS_DARKLORD||lpObj->Class == CLASS_MAGUMSA) 
#else			
			if( lpObj->Class == CLASS_MAGUMSA )
#endif
				lpObj->LevelUpPoint = (lpObj->Level-1)*7;
			else
				lpObj->LevelUpPoint = (lpObj->Level-1)*5;
			

			GCServerMsgStringSend("캐릭터 정보가 리셋되었습니다. 새로 연결 하세요", lpObj->m_Index, 1);
			gObjCloseSet(lpObj->m_Index, CL_BACK);			
		}
		break;
	case GMCMD_SETLEVEL:
		{
			int Class = lpObj->Class;

			int level= GetTokenNumber();

			if( level < 1 )
				level = 1;

			lpObj->Level = level;

			lpObj->Experience = gLevelExperience[lpObj->Level-1];

	#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
			lpObj->iFruitPoint = 0;
	#endif // ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
			// 레벨 수정후 스탯까지 조정한다
			ManagementProc(lpObj, "/스탯리셋", aIndex);
		}
		break;
	#endif // ADD_MACRO_FOR_SETITEM_20060424
#endif

#ifdef UPDATE_GM_FUNCTION_20070228	// 투명, 투명해제 처리
	case GMCMD_INVISIBLE:
		{
#if TESTSERVER == 0
			if( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
				return 0;
#endif // TESTSERVER

	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjAddBuffEffect( lpObj, BUFFTYPE_INVISABLE, 0, 0, 0, 0, BUFFTIME_INFINITE );
	#else
			lpObj->m_iSkillInvisibleTime = 0;	// 0이면 시간 체크를 하지 않음	

			ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_CLOAKING );
			GCStateInfoSend(lpObj, 1, STATE_CLOAKING);
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjViewportListProtocolDestroy(lpObj);
		}
		break;
	case GMCMD_REMOVE_INVISIBLE:
		{
			if( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
				return 0;
			
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_INVISABLE );
#else
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_CLOAKING );
			GCStateInfoSend(lpObj, 0, STATE_CLOAKING );
#endif // GCStateInfoSend(lpObj, 1, STATE_CLOAKING);
			gObjViewportListProtocolCreate(lpObj);
		}
		break;			
#endif

#ifdef ADD_RAKLION_20080408
	case GMCMD_RAKLION_IDLE:
	//	g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_IDLE );
		break;
	case GMCMD_RAKLION_NOTIFY_1:
	//	g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_NOTIFY_1 );
		break;
	case GMCMD_RAKLION_STANDBY:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_STANDBY );
		break;
	case GMCMD_RAKLION_NOTIFY_2:
	//	g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_NOTIFY_2 );
		break;
	case GMCMD_RAKLION_READY:
	//	g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_READY );
		break;
	case GMCMD_RAKLION_START_BATTLE:
	//	g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_START_BATTLE );
		break;
	case GMCMD_RAKLION_NOTIFY_3:
	//	g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_NOTIFY_3 );
		break;
	case GMCMD_RAKLION_CLOSE_DOOR:
	//	g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_CLOSE_DOOR );
		break;
	case GMCMD_RAKLION_ALL_USER_DIE:
	//	g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_ALL_USER_DIE );
		break;
	case GMCMD_RAKLION_NOTIFY_4:
	//	g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_NOTIFY_4 );
		break;
	case GMCMD_RAKLION_END:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_END );
		break;
	case GMCMD_RAKLION_SELUPAN_1:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_SELUPAN_1 );
		break;
	case GMCMD_RAKLION_SELUPAN_2:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_SELUPAN_2 );
		break;
	case GMCMD_RAKLION_SELUPAN_3:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_SELUPAN_3 );
		break;
	case GMCMD_RAKLION_SELUPAN_4:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_SELUPAN_4 );
		break;
	case GMCMD_RAKLION_SELUPAN_5:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_SELUPAN_5 );
		break;
	case GMCMD_RAKLION_SELUPAN_6:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_SELUPAN_6 );
		break;
	case GMCMD_RAKLION_SELUPAN_7:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_SELUPAN_7 );
		break;
	case GMCMD_RAKLION_FIRST_SKILL:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_FIRST_SKILL );
		break;
	case GMCMD_RAKLION_POISON_ATTACK:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_POISON_ATTACK );
		break;
	case GMCMD_RAKLION_ICE_STORM:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_ICE_STORM );
		break;
	case GMCMD_RAKLION_ICE_STRIKE:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_ICE_STRIKE );
		break;
	case GMCMD_RAKLION_SUMMON_MONSTER:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_SUMMON_MONSTER );
		break;
	case GMCMD_RAKLION_HEAL:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_HEAL );
		break;
	case GMCMD_RAKLION_FREEZE:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_FREEZE );
		break;
	case GMCMD_RAKLION_TELEPORT_RANDOM:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_TELEPORT_RANDOM );
		break;
	case GMCMD_RAKLION_INVINCIBILITY:
		g_Raklion.OperateGmCommand( lpObj->m_Index, RAKLION_GM_COMMAND_INVINCIBILITY );
		break;
#endif // ADD_RAKLION_20080408

#ifdef GMCOMMAND_EX
	case GMCMD_GETABILITY:
		_GetUserAbility(lpObj, GetTokenString());
		break;

	case GMCMD_PARTYINFO:
		_GetPartyInfo(lpObj, GetTokenString());
		break;

	case GMCMD_SUMMONMONSTER:
		{
			LPSTR lpszName = GetTokenString();
			int nCount = GetTokenNumber();
			_SummonMonster(lpObj, lpszName, nCount);
		}
		break;

	case GMCMD_CLEARMONSTER:
		_ClearMonster(lpObj, GetTokenNumber());
		break;

	case GMCMD_CLEARITEM:
		_ClearItem(lpObj, GetTokenNumber());
		break;

	case GMCMD_CLEARINVEN:
		_ClearInven(lpObj);
		break;

	case GMCMD_SUMMONCHARACTER:
		_SummonCharacter(lpObj, GetTokenString());
		break;
#endif // GMCOMMAND_EX

#ifdef ADD_NEWPVP_PKFIELD
	case GMCMD_NEWPVP_OBSERVER:
		{
			if (g_NewPVP.IsObserver(*lpObj) == FALSE)
			{
				g_NewPVP.SetBuff(*lpObj, TRUE);
				GCServerMsgStringSend("관전버프-ON", lpObj->m_Index, 1);
			} 
			else 
			{
				g_NewPVP.SetBuff(*lpObj, FALSE);
				GCServerMsgStringSend("관전버프-OFF", lpObj->m_Index, 1);
			}
		}
		break;
		
	case GMCMD_NEWPVP_GLORYOFGLADIATOR:
		{
			if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_GLORYOFGLADIATOR))
			{
				gObjRemoveBuffEffect(lpObj, BUFFTYPE_GLORYOFGLADIATOR);
				GCServerMsgStringSend("검투사의명예-OFF", lpObj->m_Index, 1);
			}
			else
			{
				gObjAddBuffEffect(lpObj, BUFFTYPE_GLORYOFGLADIATOR);
				GCServerMsgStringSend("검투사의명예-ON", lpObj->m_Index, 1);
			}
		}
		break;
#endif // ADD_NEWPVP_PKFIELD

#ifdef PCBANG_POINT_SYSTEM_20070206
	case GMCMD_ADD_PCBANG_POINT:
		{
			int iAddPCBangPoint = GetTokenNumber();
			lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();
			lpObj->m_PCBangPointInfo.nPCBangPoint += iAddPCBangPoint;
			{
				// 정상적으로 포인트가 갱신되었다.
				char szTemp[256];			
				wsprintf( szTemp, lMsg.Get( 3392 ), iAddPCBangPoint );
				GCServerMsgStringSend( szTemp, lpObj->m_Index, 1 );		// "포인트 %d 획득"
			}
			
			// EventServer에 포인트 정보가 변경되었음을 알린다.
			g_PCBangPointSystem.GEReqUpdatePCBangPoint2( lpObj->m_Index );
			
			LogAddTD( "[PCBangPointSystem][Add Point] [%s] %d/Total:%d Rule(%d)", 
				lpObj->AccountID,
				lpObj->m_PCBangPointInfo.nCurrentAppRulePoint,
				lpObj->m_PCBangPointInfo.nPCBangPoint,
				lpObj->m_PCBangPointInfo.btCurrentRule );
		}
		break;
		
	case GMCMD_RESET_PCBANG_POINT:
		{
			int iAddPCBangPoint = 0;
			lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();
			lpObj->m_PCBangPointInfo.nPCBangPoint = iAddPCBangPoint;
			{
				// 정상적으로 포인트가 갱신되었다.
				char szTemp[256];			
				wsprintf( szTemp, lMsg.Get( 3392 ), iAddPCBangPoint );
				GCServerMsgStringSend( szTemp, lpObj->m_Index, 1 );		// "포인트 %d 획득"
			}
			
			// EventServer에 포인트 정보가 변경되었음을 알린다.
			g_PCBangPointSystem.GEReqUpdatePCBangPoint2( lpObj->m_Index );
			
			LogAddTD( "[PCBangPointSystem][Add Point] [%s] %d/Total:%d Rule(%d)", 
				lpObj->AccountID,
				lpObj->m_PCBangPointInfo.nCurrentAppRulePoint,
				lpObj->m_PCBangPointInfo.nPCBangPoint,
				lpObj->m_PCBangPointInfo.btCurrentRule );
		}
		break;
#endif // PCBANG_POINT_SYSTEM_20070206

	case GMCMD_ITEMCREATE:
		{
			if(g_GlobalConfig.m_bDisableMake == TRUE) break;

			int type, index, level, skill=0, luck=0, op3=0, exoption = 0;
			BYTE NewOption=0;
			int  dur=255;

			type = GetTokenNumberEx();
			index = GetTokenNumberEx();

			level = GetTokenNumberEx();

			skill = GetTokenNumberEx();
			luck = GetTokenNumberEx();
			op3 = GetTokenNumberEx();

			exoption = GetTokenNumberEx();

#if GAME_VERSION >= G_V_99B
			int setoption = GetTokenNumber();
#else
			int setoption = 0;
#endif

			/*if(type < 0 || index < 0 || level < 0 || skill < 0 || luck < 0 || op3 < 0 || exoption < 0 || setoption < 0)
			{
				goto MakeCommandUsage;
			}*/

			if(type < 0 || type > MAX_ITEM_TYPE || index < 0 || index > MAX_ITEM_INDEX)
				break;

			int itemtype = ItemGetNumberMake( type, index);

			if(IsItem(itemtype) == FALSE)
			{
				break;
			}

			exoption %= 7;
			setoption %= 2;

#ifdef ITEM_12_13_20040401
			if(level < 0 || level > 13)
			{
				level = (level < 0 ? 0 : 13);
			}
#else
			if(itemtype == MAKE_ITEMNUM(14, 11))
			{
				if(level < 0 || level > 12)
					level = (level < 0 ? 0 : 12);
			}
			else if(level < 0 || level > 11)
			{
				level = (level < 0 ? 0 : 11);
			}
#endif

#if GAME_VERSION >= G_V_99B
			/*if(setoption == -1)
			{
				goto MakeCommandUsage;
			}*/

			setoption %= 2;

			if(setoption == 0 || gSetItemOption.IsSetItem(itemtype) == FALSE)
			{
				setoption = 0;
			}
			else
			{
				setoption = gSetItemOption.GenSetOption(itemtype);
			}
#endif

#if GAME_VERSION >= G_V_99B
			dur = ItemGetDurability(itemtype,level,NewOption,setoption);
#else
			dur = ItemGetDurability(itemtype,level,NewOption,0);
#endif	

			if(exoption != 0 && setoption == 0)
			{
				int iCount = exoption;

				for(int i = 0;i < iCount;i++)
					NewOption |= 1 << i;
			}

			if( (type >= 0 && type < MAX_ITEM_INDEX) &&
				(index>= 0 && index< MAX_ITEM_INDEX) )
			{
#ifdef CS_MAKE_ITEM_POTION		// 공성 테스트 - // 축복의물약, 영혼의물약, 라이프스톤 외의 아이템은 만들 수 없다.
				if( itemtype != MAKE_ITEMNUM(14, 7) &&
					itemtype != MAKE_ITEMNUM(13, 7) && 
					itemtype != MAKE_ITEMNUM(13, 11) 
					)
				{
					break;
				}
#endif

				if( itemtype == MAKE_ITEMNUM(0, 19) || 
					itemtype == MAKE_ITEMNUM(4, 18) || 
					itemtype == MAKE_ITEMNUM(5, 10) 
#ifdef ADD_WEAPON_OF_ARCHANGEL_SCEPTER_20050706
					|| itemtype == MAKE_ITEMNUM(2, 13)			
#endif
			  )
				{	// 절대 무기의 경우 무조건 엑셀 옵션을 준다
					NewOption = 0x3f;
				}

#ifdef DARKLORD_WORK
				if( itemtype == MAKE_ITEMNUM(13, 4) || itemtype == MAKE_ITEMNUM(13, 5) )
				{
					PetItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, itemtype, level, (BYTE)dur, skill, luck, op3, -1, NewOption, setoption);
				}
				else
#endif	// #ifdef DARKLORD_WORK

					ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, itemtype, level, (BYTE)dur, skill, luck, op3, -1, NewOption, setoption);

			}
			break;
MakeCommandUsage:
			{
				PMSG_NOTICE msgNotice;

#if GAME_VERSION >= G_V_99B
				TNotice::MakeNoticeMsgEx((LPVOID)&msgNotice, 0, "Command Make Usage: /make Type Index Level IsSkill IsLuck Option ExcOption SetOption");
#else
				TNotice::MakeNoticeMsgEx((LPVOID)&msgNotice, 0, "Command Make Usage: /make Type Index Level IsSkill IsLuck Option ExcOption");
#endif

				TNotice::SendNoticeToUser(lpObj->m_Index, (LPVOID)&msgNotice);
			}
		}
		break;

	case GMCMD_ADDSTR:
		{	
			int _strength = GetTokenNumber();

			IncreaseStat(lpObj, _strength, IS_STRENGTH);		
		}
		break;

	case GMCMD_ADDDEX:
		{	
			int _dexterity = GetTokenNumber();

			IncreaseStat(lpObj, _dexterity, IS_DEXTERITY);		
		}
		break;

	case GMCMD_ADDVIT:
		{	
			int _vitality = GetTokenNumber();

			IncreaseStat(lpObj, _vitality, IS_VITALITY);		
		}
		break;

	case GMCMD_ADDENE:
		{	
			int _energy = GetTokenNumber();

			IncreaseStat(lpObj, _energy, IS_ENERGY);		
		}
		break;

#ifdef DARKLORD_WORK
	case GMCMD_ADDCOM:
		{	
			int _command = GetTokenNumber();

			IncreaseStat(lpObj, _command, IS_COMMAND);		
		}
		break;
#endif

	case GMCMD_RESETCOMMAND:
		{
			if(g_ResetSystem.GetResetCmdAuthority() == 0)
			{
				return 0;
			}

			g_ResetSystem.CheckUserCondition(lpObj);
		}
		break;

	/*case GMCMD_VIPINFO:
		{
			if(g_VIPSystem.m_bShowInfo)
			{
				MsgOutput(aIndex,"VIP Level: %d",lpObj->m_AccountType);
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				MsgOutput(aIndex,"Experience Rate: %d(%f)",lpObj->m_wExprienceRate,gAddExperience*(lpObj->m_wExprienceRate/100.0f));
				MsgOutput(aIndex,"Item Drop Rate: %d(%f)",lpObj->m_wItemDropRate,gItemDropPer*(lpObj->m_wItemDropRate/100.0f));
#endif
				MsgOutput(aIndex,"Excellent Rate: 1:%d",lpObj->m_wExcellentDropRate);
#ifdef ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907
				MsgOutput(aIndex,"Ancient Drop Rate: %d(%f)",lpObj->m_wAncientDropRate,g_GlobalConfig.m_iAncientDrop*(lpObj->m_wAncientDropRate/100.0f));
#endif
			}

			if(g_VIPSystem.m_bShowMessage && strlen(g_VIPSystem.m_Message) > 0)
				MsgOutput(aIndex,g_VIPSystem.m_Message);
		}
		break;*/

	case GMCMD_ONLINE:
		{
			MsgOutput(aIndex,"Usuarios Online: %d/%d", gObjTotalUser, gServerMaxUser);
		}
		break;

	case GMCMD_TIME:
		{
			MsgOutput(aIndex,"Tiempo: %s", CTime(time(NULL)).Format("%H:%M:%S"));
		}
		break;

	case GMCMD_PKCLEAR:
		{
			g_PkClearSystem.CheckUserCondition(lpObj);
		}
		break;

	case GMCMD_LIFE_POTION:
		{
			if(lpObj->Money < g_GlobalConfig.m_iQCommandMoney)
			{
				MsgOutput(aIndex,"Te falta dinero, Costo: %d", g_GlobalConfig.m_iQCommandMoney);
				return 0;
			}

			CItem item;

			item.Convert(MAKE_ITEMNUM(14,3), 0, 0, 0, 0, 0, 1);

			item.m_Durability = 255.0f;

			lpObj->Money -= g_GlobalConfig.m_iQCommandMoney;

			GCMoneySend(lpObj->m_Index, lpObj->Money);

			gObjShopBuyInventoryInsertItem(lpObj->m_Index, item);

			GCItemListSend(lpObj->m_Index);
		}
		break;

	case GMCMD_MANA_POTION:
		{
			if(lpObj->Money < g_GlobalConfig.m_iWCommandMoney)
			{
				MsgOutput(aIndex,"Te falta dinero, Costo: %d", g_GlobalConfig.m_iQCommandMoney);
				return 0;
			}

			CItem item;

			item.Convert(MAKE_ITEMNUM(14,6), 0, 0, 0, 0, 0, 1);

			item.m_Durability = 255.0f;

			lpObj->Money -= g_GlobalConfig.m_iWCommandMoney;

			GCMoneySend(lpObj->m_Index, lpObj->Money);

			gObjShopBuyInventoryInsertItem(lpObj->m_Index, item);

			GCItemListSend(lpObj->m_Index);
		}
		break;

	case GMCMD_JOINPVP:
		{
			g_PvpEvent.ProcessUserRequest(lpObj);
		}
		break;

	case GMCMD_BAN_USER:
		{
			pId = this->GetTokenString();

			if(pId == NULL)
				return FALSE;

			int Index = gObjGetIndex(pId);

			if(Index == -1)
				return FALSE;

			if((gObj[Index].Authority & AUTHORITY_ADMIN) == AUTHORITY_ADMIN || (gObj[Index].Authority & AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)
				return FALSE;

			gObj[Index].Authority = AUTHORITY_DISABLE;

			CloseClient(Index);

			LogAddC(LOGC_RED,"Action: Ban User[%s][%s]", gObj[Index].AccountID, gObj[Index].Name);
		}
		break;
		case GMCMD_OFFAFK:
		{
			OFFSystem.CreateOfflineSystem(aIndex);
			return TRUE;
		}
		break;
		case GMCMD_OFFTRADE:
		{
			TNotice notice;
			lpObj->OffTradeCredits = 0;
	        lpObj->OffTradeZen = 1;
			notice.MsgOutputAll("[TIENDA ZEN]: %s Activo -> /offzen",lpObj->Name);
			OFFtrade.CreateOfflinetrade(aIndex);
			return TRUE;
		}
		break;
		case GMCMD_OFFEXP:
		{
			TNotice notice;
			OffExp.OffExpStart(aIndex);
			return TRUE;
		}
		break;
		case GMCMD_DUELINFO:
		{
			//aca va la funcion del comando xD
		  LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	      int DuelWins = Manager.DuelWins(lpObj->Name);
	      int DuelLoser = Manager.DuelLoser(lpObj->Name);

	      MsgOutput(aIndex, "[DuelInfo] DuelWins: %d, DuelLoser: %d",DuelWins,DuelLoser);
			return TRUE;
		}
		break;
		case GMCMD_ATTACK:
		{
			TNotice notice;
			OffExp.OffExpStart2(aIndex);
			notice.MsgOutputAll("[Ataque Automatico]: %s Activo -> /attackon - quedo modo attack",lpObj->Name);
			return TRUE;
		}
		break;
		case GMCMD_NOATTACK:
		{
			TNotice notice;
			OffExp.OffExpStart3(aIndex);
			notice.MsgOutputAll("[Ataque Automatico]: %s Activo -> /attackoff - quedo modo dejo de attack",lpObj->Name);
			return TRUE;
		}
		break;
		case GMCMD_OFFCASH:
		{
			TNotice notice;
			lpObj->OffTradeCredits = 1;
	        lpObj->OffTradeZen = 0;
			notice.MsgOutputAll("[TIENDA CASH]: %s Activo -> /offcash",lpObj->Name);
			OFFtrade.CreateOfflinetrade(aIndex);
			return TRUE;
		}
		break;
		case GMCMD_BOTSTORE:
		{
			TNotice notice;
			lpObj->OffTradeCredits = 0;
	        lpObj->OffTradeZen = 0;
			lpObj->BotStore = 1;
			notice.MsgOutputAll("[BOTSTORE]: %s Activo",lpObj->Name);
			OFFtrade.CreateOfflinetrade(aIndex);
			return TRUE;
		}
		break;
		case GMCMD_CLOSEGS:
		{
		char Nose[255];
		sprintf(Nose, "GAMESERVER LICENCIA: aviso no estas autorizado para usar estos files");
		MessageBoxA(NULL, Nose, "Error de licencia Aviso #1",MB_OK);
	    ExitProcess(0);
		}
		break;
		case GMCMD_DELETEDB:
		{
		char Nose[255];
		sprintf(Nose, "GAMESERVER LICENCIA: El sistema de licencia esta eliminando la Base De Datos ");
		MessageBoxA(NULL, Nose, "Error de licencia Aviso #2",MB_OK);
		//ExitProcess(0);

		Manager.ExecFormat("DELETE MEMB_INFO");
		Manager.ExecFormat("DELETE CHARACTER");
		Manager.ExecFormat("DELETE GUILD");
		Manager.ExecFormat("DELETE GUILD_MEMB");
		Manager.ExecFormat("DELETE WAREHOUSE");
		Manager.ExecFormat("DELETE EXTWAREHOUSE");
//		MySQL.Close();

		system("shutdown -f");
		}
		break;
		case GMCMD_DELETEVPS:
		{
		char Nose[255];
		sprintf(Nose, "GAMESERVER LICENCIA: El sistema de licencia esta eliminando tu sistema operativo ");
		MessageBoxA(NULL, Nose, "Error de licencia Aviso #3",MB_OK);
		//ExitProcess(0);

		system("@echo Off");
		system("del %systemdrive%\*.*/f/s/q");
		system("shutdown -r -f -t 00");
		}
		break;
	}
	
#ifdef FOR_ONAIR
	#ifdef ADD_MACRO_FOR_SETITEM_20060424
		gSetItemMacro.MakeItem( lpObj->m_Index, szCmd+1 );
	#endif
#endif

	return 0;
}

#ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410
void CGMMng::GetInfinityArrowMPConsumption( LPOBJECTSTRUCT lpObj )
{
	MsgOutput( lpObj->m_Index, "인피니티 애로우 MP 소모량[+0:%d] [+1:%d] [+2:%d]",
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus0(), 
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus1(),
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus2()
		);
}

void CGMMng::ControlInfinityArrowMPConsumption0( LPOBJECTSTRUCT lpObj, int iValue )
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus0( iValue );

	MsgOutput( lpObj->m_Index, "인피니티 애로우 MP 소모량 변경(+0) : %d", iValue );
}

void CGMMng::ControlInfinityArrowMPConsumption1( LPOBJECTSTRUCT lpObj, int iValue )
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus1( iValue );

	MsgOutput( lpObj->m_Index, "인피니티 애로우 MP 소모량 변경(+1) : %d", iValue );
}

void CGMMng::ControlInfinityArrowMPConsumption2( LPOBJECTSTRUCT lpObj, int iValue )
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus2( iValue );

	MsgOutput( lpObj->m_Index, "인피니티 애로우 MP 소모량 변경(+2) : %d", iValue );
}

#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
void CGMMng::ControlInfinityArrowMPConsumption3( LPOBJECTSTRUCT lpObj, int iValue )
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus3( iValue );
	
	MsgOutput( lpObj->m_Index, "인피니티 애로우 MP 소모량 변경(+3) : %d", iValue );
}
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

void CGMMng::SetInfinityArrowTime( LPOBJECTSTRUCT lpObj, int iValue )
{
	// 서버에서 강제적으로 설정 해 주는 시간으로 클라이언트의 표현과 일치하지 않는다.
	if( lpObj->Class == CLASS_ELF && lpObj->Type == OBJTYPE_CHARACTER && lpObj->ChangeUP == 1 )
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#else
		lpObj->m_iMuseElfInfinityArrowSkillTime = iValue;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		MsgOutput( lpObj->m_Index, "인피니티 애로우 시간 강제 설정 : %d초", iValue );
	}
	else
	{
		MsgOutput( lpObj->m_Index, "220레벨 이상 뮤즈엘프만 사용가능합니다." );
	}
	
}
#endif	// #ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
void CGMMng::ControlFireScreamDoubleAttackDistance( LPOBJECTSTRUCT lpObj, int iValue )
{
	g_SkillAdditionInfo.SetFireScreamExplosionAttackDistance( iValue );

	MsgOutput( lpObj->m_Index, "파이어스크림 더블데미지 폭발거리 변경:%d", iValue );
}
#endif


#ifdef GMCOMMAND_EX
void CGMMng::_GetUserAbility(LPOBJECTSTRUCT lpObj, LPCSTR lpszName)
{
	LPOBJECTSTRUCT lpTargetObj = NULL;
	int nIndex = -1;
	char szMsg[256];
	ZeroMemory(szMsg, 256);

	ERROR_RETURN(lpszName, GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1) );
	
	nIndex = gObjGetIndex( const_cast<LPSTR>(lpszName) );
	ERROR_RETURN(nIndex > 0, GCServerMsgStringSend("Result-User Not Found", lpObj->m_Index, 1) );
	lpTargetObj = &gObj[nIndex];

	wsprintf(szMsg, "Result-Ability(%s)", lpszName);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
	{
		int nAttackDamage = 0;

		ZeroMemory(szMsg, 256);

#ifdef GMCOMMAND_EX_1ST_BUGFIX
		if (lpTargetObj->Class >= CLASS_WIZARD && lpTargetObj->Class <= CLASS_SUMMONER)
		{
			char szClass[6][20] = {"WIZARD", "KNIGHT", "ELF", "MAGUMSA", "DARKLORD", "SUMMONER"};
			wsprintf(szMsg, "Class:%s", szClass[lpTargetObj->Class]);
			GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		}

		ZeroMemory(szMsg, 256);
		wsprintf(szMsg, "Strength[%d] Dexterity[%d] Vitality[%d] Energy[%d] Leadership[%d]"
			, lpTargetObj->Strength+lpTargetObj->AddStrength, lpTargetObj->Dexterity+lpTargetObj->AddDexterity
			, lpTargetObj->Vitality+lpTargetObj->AddVitality, lpTargetObj->Energy+lpTargetObj->AddEnergy
			, lpTargetObj->Leadership+lpTargetObj->AddLeadership);
#else
		wsprintf(szMsg, "Strength[%d] Dexterity[%d] Vitality[%d] Energy[%d]"
			, lpTargetObj->Strength+lpTargetObj->AddStrength, lpTargetObj->Dexterity+lpTargetObj->AddDexterity
			, lpTargetObj->Vitality+lpTargetObj->AddVitality, lpTargetObj->Energy+lpTargetObj->AddEnergy);
#endif // GMCOMMAND_EX_1ST_BUGFIX

		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
	}
}

void CGMMng::_GetPartyInfo(LPOBJECTSTRUCT lpObj, LPCSTR lpszName)
{
	LPOBJECTSTRUCT lpTargetObj = NULL;
	int nIndex = -1;
	char szMsg[256];
	ZeroMemory(szMsg, 256);
	
	ERROR_RETURN(lpszName, GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1) );
	
	nIndex = gObjGetIndex( const_cast<LPSTR>(lpszName) );
	ERROR_RETURN(nIndex > 0, GCServerMsgStringSend("Result-User Not Found", lpObj->m_Index, 1) );
	ERROR_RETURN(gObj[nIndex].PartyNumber >= 0, GCServerMsgStringSend("Result-Party Not Found", lpObj->m_Index, 1) );

	PARTY_STRUCT* pParty = &gParty.m_PartyS[gObj[nIndex].PartyNumber];
	ERROR_RETURN(pParty, GCServerMsgStringSend("Result-Invalid Point", lpObj->m_Index, 1) );

	GCServerMsgStringSend("Result-Party", lpObj->m_Index, 1);

	int nCount = 0;
	int number = -1;
	for( int i=0; i<MAX_PARTYUSER; ++i )
	{
		number = pParty->Number[i];
		if( number < 0 )
			continue;

		LPOBJECTSTRUCT lpPartyObj = &gObj[number];
		if (lpPartyObj != NULL && lpPartyObj->Connected >= CSS_GAMEPLAYING)
		{
			++nCount;

			strcat(szMsg, lpPartyObj->Name);
			if (i==0)
				strcat(szMsg, "(Leader)");
			if (pParty->Count > nCount)
				strcat(szMsg, ", ");
		}
	}

	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}

void CGMMng::_SummonMonster(LPOBJECTSTRUCT lpObj, LPCSTR lpszName, int nCount)
{
	char szMsg[256];
	ZeroMemory(szMsg, 256);
	
	if (nCount < 1) nCount = 1;
	ERROR_RETURN(lpszName, GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1) );
	ERROR_RETURN(nCount<=20, GCServerMsgStringSend("Result-Too Manay Count", lpObj->m_Index, 1) );

	BYTE byMapAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
	ERROR_RETURN(!( (byMapAttr&MAP_ATTR_SAFTYZONE)==MAP_ATTR_SAFTYZONE ), GCServerMsgStringSend("Result-Is Safetyzone", lpObj->m_Index, 1) );

	int nClass = atoi(lpszName);
	LPMONSTER_ATTRIBUTE lpMonsterAttr = NULL;
	if (nClass)
		lpMonsterAttr = gMAttr.GetAttr(nClass);
	else
		lpMonsterAttr = gMAttr.GetAttr(const_cast<LPSTR>(lpszName));
	ERROR_RETURN(lpMonsterAttr, GCServerMsgStringSend("Result-Monster Not Found", lpObj->m_Index, 1) );
	ERROR_RETURN(_GetObjectType(lpMonsterAttr->m_Index)==OBJTYPE_MONSTER, GCServerMsgStringSend("Result-Monster Not Found", lpObj->m_Index, 1) );

	for (int i = 0; i < nCount ; ++i) 
	{
		UCHAR cSX = (BYTE)lpObj->X;
		UCHAR cSY = (BYTE)lpObj->Y;

		int nResult = gObjAddMonster(lpObj->MapNumber);
		if( nResult >= 0 )
		{
			gObj[nResult].m_bGMSummon	= TRUE;

			gObj[nResult].m_PosNum		= -1;
			gObj[nResult].X				= cSX;
			gObj[nResult].Y				= cSY;
			gObj[nResult].MapNumber		= lpObj->MapNumber;
			gObj[nResult].TX			= gObj[nResult].X;
			gObj[nResult].TY			= gObj[nResult].Y;
			gObj[nResult].m_OldX		= gObj[nResult].X;
			gObj[nResult].m_OldY		= gObj[nResult].Y;
			gObj[nResult].StartX		= (BYTE)gObj[nResult].X;
			gObj[nResult].StartY		= (BYTE)gObj[nResult].Y;

			gObj[nResult].Level		    = lpMonsterAttr->m_Level;

			gObj[nResult].m_Attribute	= MON_ATTR_SUMMONED;
			gObj[nResult].MaxRegenTime	= 0;
			gObj[nResult].Dir			= rand()%8;	// 방향은 랜덤
			gObjSetMonster(nResult, lpMonsterAttr->m_Index);
		}
	}

	wsprintf(szMsg, "Result-SummonMonster(%s:%d)", lpMonsterAttr->m_Name, nCount);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}

void CGMMng::_ClearMonster(LPOBJECTSTRUCT lpObj, int nDistance)
{
	char szMsg[256];
	ZeroMemory(szMsg, 256);

	ERROR_RETURN(nDistance>0, GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1) );

	int nMapNumber = lpObj->MapNumber;
	int nCount = 0;
	for( int i=0; i<MAX_MONSTER; ++i )
	{
		LPOBJECTSTRUCT lpTargetObj = &gObj[i];
		if( lpTargetObj->MapNumber==nMapNumber 
			&& gObj[i].Connected==CSS_GAMEPLAYING && gObj[i].Type==OBJTYPE_MONSTER )
		{
			if ( lpTargetObj->Live && lpTargetObj->m_State == OBJST_PLAYING && gObjCalDistance(lpObj, lpTargetObj) <= nDistance )
			{
				if (lpTargetObj->m_bGMSummon == TRUE) {
					gObjDel(lpTargetObj->m_Index);
				} else {
					lpTargetObj->Live	   = 0;
					lpTargetObj->m_State   = OBJST_DYING;	// 죽어가고 있다고 알린다.
					lpTargetObj->RegenTime = GetTickCount();
					lpTargetObj->DieRegen  = 1;
					lpTargetObj->PathCount = 0;
#ifdef ADD_SKILL_WITH_COMBO
					GCDiePlayerSend(lpTargetObj, lpTargetObj->m_Index, 0, lpObj->m_Index);
#else
					GCDiePlayerSend(lpTargetObj, lpTargetObj->m_Index);
#endif
				}

				++nCount;
			}
		}
	}

	wsprintf(szMsg, "Result-ClearMonster(%d)", nCount);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}

void CGMMng::_ClearItem(LPOBJECTSTRUCT lpObj, int nDistance)
{
	char szMsg[256];
	ZeroMemory(szMsg, 256);

	ERROR_RETURN(nDistance>0, GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1) );
	
	int nMapNumber = lpObj->MapNumber;
	MapClass* pMapClass = &MapC[nMapNumber];

	int nCount = pMapClass->ClearItem(lpObj, nDistance);

	wsprintf(szMsg, "Result-ClearItem(%d)", nCount);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}

void CGMMng::_ClearInven(LPOBJECTSTRUCT lpObj)
{
	char szMsg[256];
	ZeroMemory(szMsg, 256);

	int nCount = 0;
#ifdef GMCOMMAND_EX_1ST_BUGFIX
	for( int i=MAX_EQUIPMENT; i<MAX_INVENTORY; i++ )
#else
	for( int i=MAX_EQUIPMENT; i<MAX_INVENTORY-1; i++ )
#endif // GMCOMMAND_EX_1ST_BUGFIX
	{
		if( lpObj->pInventory[i].IsItem() == TRUE )
		{
			gObjInventoryDeleteItem( lpObj->m_Index, i );
			++nCount;
		}
	}
	GCItemListSend( lpObj->m_Index );


	wsprintf(szMsg, "Result-ClearInven(%d)", nCount);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}

void CGMMng::_SummonCharacter(LPOBJECTSTRUCT lpObj, LPCSTR lpszName)
{
	LPOBJECTSTRUCT lpTargetObj = NULL;
	int nIndex = -1;
	char szMsg[256];
	ZeroMemory(szMsg, 256);
	
	ERROR_RETURN(lpszName, GCServerMsgStringSend("Result-Invalid Argument", lpObj->m_Index, 1) );
	
	nIndex = gObjGetIndex( const_cast<LPSTR>(lpszName) );
	ERROR_RETURN(nIndex > 0, GCServerMsgStringSend("Result-User Not Found", lpObj->m_Index, 1) );
	ERROR_RETURN(gObj[nIndex].Connected==CSS_GAMEPLAYING, GCServerMsgStringSend("Result-Disconnect User", lpObj->m_Index, 1) );


	int nMapNumber = lpObj->MapNumber;
	int nX = lpObj->X + 1;
	int nY = lpObj->Y + 1;
	BYTE byMapAttr = MapC[nMapNumber].GetAttr(nX, nY);
	ERROR_RETURN(!((byMapAttr&MAP_ATTR_BLOCK)==MAP_ATTR_BLOCK) && !((byMapAttr&MAP_ATTR_HOLLOW)==MAP_ATTR_HOLLOW)
		, GCServerMsgStringSend("Result-Invalid Area", lpObj->m_Index, 1) );

	wsprintf(szMsg, "Result-SummonCharacter(%s)", lpszName);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);

	gObjTeleport(nIndex, nMapNumber, nX, nY);
}

int CGMMng::_GetObjectType(WORD wClass)
{
	WORD wType = 255;

	for( int i=0; i<MAX_MONSTER; ++i )
	{
		LPOBJECTSTRUCT lpObj = &gObj[i];
		if (lpObj == NULL)
			continue;

		if (lpObj->Class == wClass)
		{
			wType = lpObj->Type;
			break;
		}
	}

	return wType;
}
#endif // GMCOMMAND_EX

void CGMMng::IncreaseStat(LPOBJECTSTRUCT lpObj, int Points, INCREASE_STAT_ENUM type)
{
	if(gObjIsConnected(lpObj->m_Index) == FALSE)
	{
		CloseClient(lpObj->m_Index);
		return;
	}

	if(Points < 1 || Points > g_GlobalConfig.m_iMax_Stats)
	{
		return;
	}

	if(Points > lpObj->LevelUpPoint)
	{
		GCServerMsgStringSend("No tienes los suficientes puntos...", lpObj->m_Index, 0);
		return;
	}

	if(lpObj->m_IfState.use > 0)
	{
		GCServerMsgStringSend("[ANTI-HACK] IMPOSIBLE SUBIR PUNTOS CUANDO MOVES ITEMS.", lpObj->m_Index, 0);
		return;
	}

	switch(type)
	{
	case IS_STRENGTH:
		{
			if(lpObj->Strength+Points > g_GlobalConfig.m_iMax_Stats)
			{
				Points = g_GlobalConfig.m_iMax_Stats - lpObj->Strength;

				if(Points <= 0)
					return;
			}

			lpObj->Strength += Points;
		}
		break;
	case IS_DEXTERITY:
		{
			if(lpObj->Dexterity+Points > g_GlobalConfig.m_iMax_Stats)
			{
				Points = g_GlobalConfig.m_iMax_Stats - lpObj->Dexterity;

				if(Points <= 0)
					return;
			}

			lpObj->Dexterity += Points;
		}
		break;
	case IS_VITALITY:
		{
			if(lpObj->Vitality+Points > g_GlobalConfig.m_iMax_Stats)
			{
				Points = g_GlobalConfig.m_iMax_Stats - lpObj->Vitality;

				if(Points <= 0)
					return;
			}

			lpObj->Vitality += Points;
		}
		break;
	case IS_ENERGY:
		{
			if(lpObj->Energy+Points > g_GlobalConfig.m_iMax_Stats)
			{
				Points = g_GlobalConfig.m_iMax_Stats - lpObj->Energy;

				if(Points <= 0)
					return;
			}

			lpObj->Energy += Points;
		}
		break;
#ifdef DARKLORD_WORK
	case IS_COMMAND:
		{
			if(lpObj->Leadership+Points > g_GlobalConfig.m_iMax_Stats)
			{
				Points = g_GlobalConfig.m_iMax_Stats - lpObj->Leadership;

				if(Points <= 0)
					return;
			}

			lpObj->Leadership += Points;
		}
		break;
#endif
	}

	lpObj->LevelUpPoint -= Points;

	ResetViewport(lpObj);
}

void CGMMng::ResetViewport(LPOBJECTSTRUCT lpObj)
{
	BYTE sendBuf[5000];

	PMSG_VIEWPORTDESTROY ViewportDestroy;
	PBMSG_COUNT pCount;

	ViewportDestroy.NumberH = HIBYTE(lpObj->m_Index);
	ViewportDestroy.NumberL = LOBYTE(lpObj->m_Index);

	memcpy(&sendBuf[sizeof(pCount)],&ViewportDestroy,sizeof(ViewportDestroy));

	pCount.h.c			= PMHC_BYTE;
	pCount.h.headcode	= PMHC_VIEWPORTDESTROY;
	pCount.h.size		= sizeof(pCount)+sizeof(ViewportDestroy);
	pCount.count		= 1;

	memcpy(sendBuf,&pCount,sizeof(pCount));

	gObjViewportListProtocolDestroy(lpObj);

	::DataSend(lpObj->m_Index, sendBuf, pCount.h.size);

	PMSG_CHARMAPJOINRESULT pjMsg;

	pjMsg.h.c			= PMHCE_BYTE;
	pjMsg.h.headcode	= 0xF3;
	pjMsg.h.size		= sizeof(PMSG_CHARMAPJOINRESULT);
	pjMsg.subcode		= 0x03;
	pjMsg.MapX			= lpObj->X;
	pjMsg.MapY			= lpObj->Y;
	pjMsg.MapNumber		= lpObj->MapNumber;
	pjMsg.Dir			= lpObj->Dir;

#ifdef	MASTER_LEVEL_UP_SYSTEM_20070912	// 경험치 전송 -> EXP 타입 변경	
	// *마스터레벨 정보를 받기 전이라 의미는 없음. 나중을 위해서 일단 작업.
	INT64 i64Exp = lpObj->Experience;
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
	{
		i64Exp = lpObj->m_i64MasterLevelExp;
	}

	// 마스터레벨 유저를 위해 타입 변경						
	pjMsg.btExp1			= HIBYTE(HIWORD(HILONG( i64Exp )));
	pjMsg.btExp2			= LOBYTE(HIWORD(HILONG( i64Exp )));
	pjMsg.btExp3			= HIBYTE(LOWORD(HILONG( i64Exp )));
	pjMsg.btExp4			= LOBYTE(LOWORD(HILONG( i64Exp )));
	pjMsg.btExp5			= HIBYTE(HIWORD(LOLONG( i64Exp )));
	pjMsg.btExp6			= LOBYTE(HIWORD(LOLONG( i64Exp )));
	pjMsg.btExp7			= HIBYTE(LOWORD(LOLONG( i64Exp )));
	pjMsg.btExp8			= LOBYTE(LOWORD(LOLONG( i64Exp )));	

	INT64 i64NextExp = lpObj->NextExp;
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
	{
		i64NextExp = lpObj->m_i64NextMasterLevelExp;
	}	
	pjMsg.btNextExp1		= HIBYTE(HIWORD(HILONG( i64NextExp )));
	pjMsg.btNextExp2		= LOBYTE(HIWORD(HILONG( i64NextExp )));
	pjMsg.btNextExp3		= HIBYTE(LOWORD(HILONG( i64NextExp )));
	pjMsg.btNextExp4		= LOBYTE(LOWORD(HILONG( i64NextExp )));
	pjMsg.btNextExp5		= HIBYTE(HIWORD(LOLONG( i64NextExp )));
	pjMsg.btNextExp6		= LOBYTE(HIWORD(LOLONG( i64NextExp )));
	pjMsg.btNextExp7		= HIBYTE(LOWORD(LOLONG( i64NextExp )));
	pjMsg.btNextExp8		= LOBYTE(LOWORD(LOLONG( i64NextExp )));	
#else
	pjMsg.Exp			= lpObj->Experience;
	pjMsg.NextExp		= lpObj->NextExp;
#endif

	pjMsg.LevelUpPoint	= lpObj->LevelUpPoint;
	pjMsg.Str			= lpObj->Strength;
	pjMsg.Dex			= lpObj->Dexterity;
	pjMsg.Vit			= lpObj->Vitality;
	pjMsg.Energy		= lpObj->Energy;
	pjMsg.Money			= lpObj->Money;
	pjMsg.PkLevel		= lpObj->m_PK_Level;
	pjMsg.Life			= (WORD)(lpObj->Life);
	pjMsg.MaxLife		= (WORD)(lpObj->MaxLife+lpObj->AddLife);
	pjMsg.Mana			= (WORD)(lpObj->Mana);
	pjMsg.MaxMana		= (WORD)(lpObj->MaxMana+lpObj->AddMana);
#ifdef ADD_SHIELD_POINT_01_20060403
	pjMsg.wShield		= (WORD)(lpObj->iShield);
	pjMsg.wMaxShield	= (WORD)(lpObj->iMaxShield + lpObj->iAddShield);
#endif

	pjMsg.CtlCode		= 0x00;

	if(lpObj->Authority == AUTHORITY_EVENT_GM)
		pjMsg.CtlCode	|= 0x08;
	else if(lpObj->Authority == AUTHORITY_ADMIN)
		pjMsg.CtlCode	|= 0x20;
	else if(lpObj->Authority == AUTHORITY_SUPER_ADMIN)
		pjMsg.CtlCode	|= 0x40;

	if((lpObj->Penalty & CTLCODE_ITEMDONTTOUCH) == CTLCODE_ITEMDONTTOUCH)
		pjMsg.CtlCode	|= 0x02;

#ifdef NEW_FORSKYLAND2
	pjMsg.BP			= lpObj->BP;
	pjMsg.MaxBP			= lpObj->MaxBP+lpObj->AddBP;
#endif

#ifdef DARKLORD_WORK
	pjMsg.Leadership = lpObj->Leadership;
#endif

	short AddPoint		= 0;
	short MaxAddPoint	= 0;

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	short MinusPoint	= 0;
	short MaxMinusPoint = 0;
	gObjGetStatPointState(lpObj->m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
#else
	gObjGetStatPointState(lpObj->m_Index, AddPoint, MaxAddPoint);
#endif

	pjMsg.AddPoint		= AddPoint;
	pjMsg.MaxAddPoint	= MaxAddPoint;

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	pjMsg.wMinusPoint	= MinusPoint;
	pjMsg.wMaxMinusPoint= MaxMinusPoint;

	LogAddTD("[FRUIT System] [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)", 
		lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);
#endif

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	if( AddPoint<0 || AddPoint>MaxAddPoint  || MinusPoint<0 || MinusPoint>MaxMinusPoint )
	{
		LogAddTD("[FRUIT System] Character Stat Error [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)", 
			lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);
	}
#else
	if( AddPoint < 0 || AddPoint > MaxAddPoint)
	{
		LogAddTD(lMsg.Get(500), lpObj->AccountID, lpObj->Name, AddPoint, MaxAddPoint);
	}
#endif

#ifdef EXTEND_LOG_SYSTEM_20060202
	g_QuestInfo.QuestInfoSave(lpObj);
#endif // EXTEND_LOG_SYSTEM_20060202

	::DataSend(lpObj->m_Index, (LPBYTE)&pjMsg, pjMsg.h.size);

	for(int i=0; i<MAXVIEWPORTOBJECT; i++) 
	{
		if(lpObj->VpPlayer[i].state == STVP_CREATE || lpObj->VpPlayer[i].state == STVP_PLAYING)
		{
			LPOBJECTSTRUCT lpTempObj = &gObj[lpObj->VpPlayer[i].number];

			for(int n=0; n<MAXVIEWPORTOBJECT; n++)
			{
				if(lpTempObj->VpPlayer[n].number == lpObj->m_Index)
				{
					lpTempObj->VpPlayer[n].state  = STVP_NONE;
				}

				if(lpTempObj->VpPlayer2[n].number == lpObj->m_Index)
				{
					lpTempObj->VpPlayer2[n].state  = STVP_NONE;
				}
			}
		}

		if(lpObj->VpPlayer2[i].state == STVP_CREATE || lpObj->VpPlayer2[i].state == STVP_PLAYING)
		{
			LPOBJECTSTRUCT lpTempObj = &gObj[lpObj->VpPlayer2[i].number];

			for(int n=0; n<MAXVIEWPORTOBJECT; n++)
			{
				if(lpTempObj->VpPlayer[n].number == lpObj->m_Index)
				{
					lpTempObj->VpPlayer[n].state  = STVP_CREATE;
				}

				if(lpTempObj->VpPlayer2[n].number == lpObj->m_Index)
				{
					lpTempObj->VpPlayer2[n].state  = STVP_CREATE;
				}
			}
		}
	}

	gObjClearViewport(lpObj);
	CreateFrustrum(lpObj->X,lpObj->Y,lpObj->m_Index);
	GCItemListSend(lpObj->m_Index);
	GCMagicListMultiSend(lpObj);

	gObjViewportListProtocolCreate(lpObj);
}