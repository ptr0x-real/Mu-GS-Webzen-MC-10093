// CastleSiege.cpp: implementation of the CCastleSiege class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CastleSiege.h"

#ifdef CASTLE_MAIN_SCHEDULER_20041111

CCastleSiege	g_CastleSiege;


#include "Common\LogProc.h"
#include "Include\ReadScript.h"
#include "DSProtocol.h"

#include "wsJoinServerCli.h"
#include "DBSockMng.h"
#include <TIME.H>

#include "MapClass.h"
extern MapClass		MapC[];

#include "LifeStone.h"
#include "Mercenary.h"

#ifdef ADD_NEW_UNION_01_20041006
#include "TUnion.h"
#include "TUnionInfo.h"
#endif


#include "LOGTOFILE.H"

#ifdef CASTLE_SAVE_PCROOM_USERLIST_20050105
CLogToFile	g_LogCsPcRoomUserList ("CS_PCROOM_USER_LOG", ".\\CS_PCROOM_USER_LOG");		// 로그파일 객체
#endif


//===================================================================


_CS_SCHEDULE_DATA::_CS_SCHEDULE_DATA	()
{
	Clear();
}


_CS_SCHEDULE_DATA::~_CS_SCHEDULE_DATA	()
{
	
}


VOID _CS_SCHEDULE_DATA::Clear			()
{
	m_bIN_USE				= FALSE;
	m_iSTATE				= -1;
	m_iADD_DAY				= -1;
	m_iADD_HOUR				= -1;
	m_iADD_MIN				= -1;
	m_iGAP_SEC				= 0;
}


//===================================================================


_CS_REG_GUILD_DATA::_CS_REG_GUILD_DATA	()
{
	Clear();
}


_CS_REG_GUILD_DATA::~_CS_REG_GUILD_DATA	()
{

}


VOID _CS_REG_GUILD_DATA::Clear			()
{
	m_bIN_USE				= FALSE;
	m_strGuildName			= "";
	m_iRegMarkCount			= 0;
	m_iGuildMemberCount		= 0;
	m_iGuildMasterLevel		= 0;
	m_iSeqNum				= 0;
	m_i64TotolScore			= 0;
}


//===================================================================


_CS_TOTAL_GUILD_DATA::_CS_TOTAL_GUILD_DATA	()
{
	Clear();
}


_CS_TOTAL_GUILD_DATA::~_CS_TOTAL_GUILD_DATA	()
{

}


VOID _CS_TOTAL_GUILD_DATA::Clear			()
{
	m_bIN_USE				= FALSE;
	m_iCsGuildID			= CS_SIEGESIDE_NONE;
	m_bGuildInvolved		= FALSE;
	
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
	m_iGuildScore			= 0;
#endif
}


//===================================================================


_CS_MINIMAP_DATA::_CS_MINIMAP_DATA		()
{
	Clear();
}


_CS_MINIMAP_DATA::~_CS_MINIMAP_DATA		()
{
	
}


VOID _CS_MINIMAP_DATA::Clear			()
{
	m_iGuildMasterIndex		= -1;
	memset (m_szGuildName, 0, sizeof(m_szGuildName));
	memset (m_stMiniMapPoint, 0, sizeof(m_stMiniMapPoint));
	m_iMiniMapPointCount	= 0;
}


//===================================================================


_CS_NPC_DATA::_CS_NPC_DATA				()
{
	Clear();

	m_iCS_GATE_LEVER_INDEX	= -1;		// 성문 레버의 인덱스는 최초 한번만 초기화 (레버 NPC가 한번만 생성되어야 하므로)
}


_CS_NPC_DATA::~_CS_NPC_DATA				()
{
	
}			


VOID _CS_NPC_DATA::Clear				()
{
	m_bIN_USE				= FALSE;
	m_iNPC_NUM				= -1;
	m_iNPC_INDEX			= -1;
	m_iNPC_LIVE				= NPC_LIVE_NONE;
	m_iNPC_AUTH				= -1;
	m_bNPC_DBSAVE			= 0;
	m_iNPC_SIDE				= 0;
	m_iNPC_OBJINDEX			= -1;

	m_iNPC_DF_LEVEL			= 0;
	m_iNPC_RG_LEVEL			= 0;
	m_iNPC_MAXHP			= 0;
	m_iNPC_HP				= 0;
	m_iNPC_SX				= -1;
	m_iNPC_SY				= -1;
	m_iNPC_DX				= -1;
	m_iNPC_DY				= -1;
	m_iNPC_DIR				= -1;

	m_iNPC_BASE_DF_LEVEL	= 0;
	m_iNPC_BASE_RG_LEVEL	= 0;
	m_iNPC_BASE_MAXHP		= 0;
	m_iNPC_BASE_HP			= 0;
	m_iNPC_BASE_SX			= -1;
	m_iNPC_BASE_SY			= -1;
	m_iNPC_BASE_DX			= -1;
	m_iNPC_BASE_DY			= -1;
	m_iNPC_BASE_DIR			= -1;

	m_iCsNpcExistVal		= 0;
}


VOID _CS_NPC_DATA::SetBaseValue			()				// 기본 데이터의 값으로 세팅
{
	m_iNPC_DF_LEVEL			= m_iNPC_BASE_DF_LEVEL;
	m_iNPC_RG_LEVEL			= m_iNPC_BASE_RG_LEVEL;
	m_iNPC_MAXHP			= m_iNPC_BASE_MAXHP;
	m_iNPC_HP				= m_iNPC_BASE_HP;
	m_iNPC_SX				= m_iNPC_BASE_SX;
	m_iNPC_SY				= m_iNPC_BASE_SY;
	m_iNPC_DX				= m_iNPC_BASE_DX;
	m_iNPC_DY				= m_iNPC_BASE_DY;
	m_iNPC_DIR				= m_iNPC_BASE_DIR;
}


//===================================================================


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCastleSiege::CCastleSiege					()
{
	InitializeCriticalSection (&m_critScheduleData);
	InitializeCriticalSection (&m_critNpcData);
	InitializeCriticalSection (&m_critCsTotalGuildInfo);
	InitializeCriticalSection (&m_critCsBasicGuildInfo);
	InitializeCriticalSection (&m_critCsMiniMap);
		
	Clear();
}

CCastleSiege::~CCastleSiege					()
{
	DeleteCriticalSection (&m_critScheduleData);
	DeleteCriticalSection (&m_critNpcData);
	DeleteCriticalSection (&m_critCsTotalGuildInfo);
	DeleteCriticalSection (&m_critCsBasicGuildInfo);
	DeleteCriticalSection (&m_critCsMiniMap);
}


//////////////////////////////////////////////////////////////////////
// Method Implementation
//////////////////////////////////////////////////////////////////////

bool CCastleSiege::ScheduleStateCompFunc	(CS_SCHEDULE_DATA & A, CS_SCHEDULE_DATA & B)
{
	// 오름차순으로 정렬
	if (A.m_iSTATE > B.m_iSTATE)
		return false;
	return true;
}


VOID CCastleSiege::Clear					()
{
	m_bCastleSiegeEnable				= TRUE;
	m_bFileDataLoadOK					= FALSE;
	m_bDbDataLoadOK						= FALSE;
	m_bDoRun							= FALSE;
	m_iMapSvrGroup						= -1;
	m_iCastleSiegeCycle					= DEFAULT_CASTLE_SIEGE_CYCLE;
	m_iCastleSiegeState					= CASTLESIEGE_STATE_NONE;
	m_bFixCastleCycleStartDate			= FALSE;
	m_i64CastleMoney					= 0;
	memset (&m_tmFixCastleCycleStartDate, 0, sizeof(m_tmFixCastleCycleStartDate));
	m_iFixCastleSpecificState			= 0;
	m_bFixCastleStateStartDate			= FALSE;
	memset (&m_tmFixCastleStateStartDate, 0, sizeof(m_tmFixCastleStateStartDate));
	m_iCastleDataLoadState				= CASTLESIEGE_DATALOAD_NONE;
	m_dwDbDataLoadTickCount				= 0;
	m_iCS_REMAIN_MSEC					= -1;
	memset (&m_tmStateStartDate, 0, sizeof(m_tmStateStartDate));
	memset (&m_tmLeftCastleSiegeDate, 0, sizeof(m_tmLeftCastleSiegeDate));
	m_bCsBasicGuildInfoLoadOK			= FALSE;
	m_bCsTotalGuildInfoLoadOK			= FALSE;

	m_btIsSiegeGuildList				= FALSE;
	m_btIsCastleOccupied				= FALSE;
	memset (m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
	
//	m_iTaxRateChaos						= 0;
//	m_iTaxRateStore						= 0;
//	m_iTaxHuntZone						= 0;

#if TESTSERVER == 1						// 테스트 모드 일 때는 사냥터 입장이 가능함
	m_bEnterHuntZone					= TRUE;
#else
	m_bEnterHuntZone					= FALSE;
#endif	

	m_dwNPC_DBSAVE_TICK_COUNT			= GetTickCount();
	m_dwCALC_LEFTSIEGE_TICK_COUNT		= 0;
	m_dwEVENT_MSG_TICK_COUNT			= 0;
	m_dwCHECK_GATE_ALIVE_COUNT			= 0;
	m_dwCS_LEFTTIME_TICK_COUNT			= 0;
	m_dwCS_STARTTIME_TICK_COUNT			= 0;
	m_dwCS_JOINSIDE_REFRESH_TICK_COUNT	= 0;
	m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT	= 0;
	m_dwCS_MINIMAP_SEND_TICK_COUNT		= 0;

	m_bCastleTowerAccessable			= FALSE;
	m_bRegCrownAvailable				= FALSE;
	m_iCastleCrownAccessUser			= -1;
	m_btCastleCrownAccessUserX			= 0;
	m_btCastleCrownAccessUserY			= 0;
	m_iCastleSwitchAccessUser1			= -1;
	m_iCastleSwitchAccessUser2			= -1;
	m_dwCrownAccessTime					= 0;
	memset (m_szMiddleWinnerGuild, 0, sizeof(m_szMiddleWinnerGuild));

	EnterCriticalSection (&m_critScheduleData);
	m_vtScheduleData.clear();
	LeaveCriticalSection (&m_critScheduleData);

	EnterCriticalSection (&m_critNpcData);
	m_vtNpcData.clear();
	LeaveCriticalSection (&m_critNpcData);
	m_bDbNpcCreated				= FALSE;

	// *> . 전체 서버 사용자들의 공/수 속성 해제
	ResetAllUserCsJoinSide ();
}


BOOL CCastleSiege::Ready					(INT iMapSvrGroup)
{
	// 공성전 중에 데이터가 초기화되면 않된다.


	// 지정된 맵 서버군이 0 보다 작아서는 않된다.
	if (iMapSvrGroup < 0) {
		return FALSE;
	}

	// 기존 데이터 초기화
	Clear();
	ClearAllNPC();			// DB NPC의 데이터도 다 초기화 되므로 이렇게 해도 상관없다.

	m_iMapSvrGroup			= iMapSvrGroup;
	m_iCastleDataLoadState	= CASTLESIEGE_DATALOAD_1;	// 여기서는 현재 데이터 로드 상황에 상관없이 강제지정 (중간에 DB에서 데이터를 못받아서 끊기는 경우도 있으므로)

	return TRUE;
}


BOOL CCastleSiege::LoadPreFixData			(LPSTR lpszFileName)
{
	if (lpszFileName == NULL)
		return FALSE;

	SYSTEMTIME tmToDay;
	GetLocalTime(&tmToDay);
	INT iTODAY_DATE_NUM						= MAKELONG(MAKEWORD(tmToDay.wDay, tmToDay.wMonth), tmToDay.wYear);
	INT64 iTODAY_DATE_NUM64					= ((INT64) ((INT64)MAKELONG(tmToDay.wMinute, tmToDay.wHour) | (INT64)((INT64)MAKELONG(MAKEWORD(tmToDay.wDay, tmToDay.wMonth), tmToDay.wYear) << 32)));

	// 1 . 데이터들을 읽고 유효성 검사를 한다.
	m_bCastleSiegeEnable					= GetPrivateProfileInt("GameServerInfo", "CastleSiegeRun", 1, lpszFileName);

	m_tmFixCastleCycleStartDate.wYear		= GetPrivateProfileInt("GameServerInfo", "CastleCycleStartYear", 0, lpszFileName);
	m_tmFixCastleCycleStartDate.wMonth		= GetPrivateProfileInt("GameServerInfo", "CastleCycleStartMonth", 0, lpszFileName);
	m_tmFixCastleCycleStartDate.wDay		= GetPrivateProfileInt("GameServerInfo", "CastleCycleStartDay", 0, lpszFileName);

	m_iFixCastleSpecificState				= GetPrivateProfileInt("GameServerInfo", "CastleSpecificState", 0, lpszFileName);
	m_tmFixCastleStateStartDate.wYear		= GetPrivateProfileInt("GameServerInfo", "CastleStateStartYear", 0, lpszFileName);
	m_tmFixCastleStateStartDate.wMonth		= GetPrivateProfileInt("GameServerInfo", "CastleStateStartMonth", 0, lpszFileName);
	m_tmFixCastleStateStartDate.wDay		= GetPrivateProfileInt("GameServerInfo", "CastleStateStartDay", 0, lpszFileName);
	m_tmFixCastleStateStartDate.wHour		= GetPrivateProfileInt("GameServerInfo", "CastleStateStartHour", 0, lpszFileName);
	m_tmFixCastleStateStartDate.wMinute		= GetPrivateProfileInt("GameServerInfo", "CastleStateStartMinute", 0, lpszFileName);


	if (!m_tmFixCastleCycleStartDate.wYear	||
		!m_tmFixCastleCycleStartDate.wMonth	||
		!m_tmFixCastleCycleStartDate.wDay
		)
	{
		m_bFixCastleCycleStartDate			= FALSE;
	}
	else {
		// 시작일에서 계산한 예상 종료일 보다 오늘 날짜가 더 늦다면 잘못된 것이다. -> 공성 주기가 오늘 날짜보다 늦는것은 얼마든지 용인된다.
		SYSTEMTIME tmEndDate				= m_tmFixCastleCycleStartDate;
		GetNextDay(tmEndDate, m_iCastleSiegeCycle);
		INT iEVENT_END_DATE_NUM				= MAKELONG(MAKEWORD(tmEndDate.wDay, tmEndDate.wMonth), tmEndDate.wYear);
		if (iEVENT_END_DATE_NUM <= iTODAY_DATE_NUM) {
			m_bFixCastleCycleStartDate		= FALSE;
			MsgBox("[CastleSiege] CCastleSiege::LoadPreFixData() - iEVENT_END_DATE_NUM (%04d-%02d-%02d) <= iTODAY_DATE_NUM (%04d-%02d-%02d)",
				tmEndDate.wYear,
				tmEndDate.wMonth,
				tmEndDate.wDay,
				tmToDay.wYear,
				tmToDay.wMonth,
				tmToDay.wDay
				);
		}
		else {
			m_bFixCastleCycleStartDate		= TRUE;
		}

		if (!CHECK_LIMIT(m_tmFixCastleCycleStartDate.wMonth, 12+1)	||
			!CHECK_LIMIT(m_tmFixCastleCycleStartDate.wDay-1, 31)
			)
		{
			m_bFixCastleCycleStartDate		= FALSE;
		}
	}

	if (!m_tmFixCastleStateStartDate.wYear	||
		!m_tmFixCastleStateStartDate.wMonth	||
		!m_tmFixCastleStateStartDate.wDay
		)
	{
		m_bFixCastleStateStartDate			= FALSE;
	}
	else if (m_iFixCastleSpecificState < CASTLESIEGE_STATE_REGSIEGE || m_iFixCastleSpecificState > CASTLESIEGE_STATE_ENDCYCLE) {
		m_bFixCastleStateStartDate			= FALSE;
	}
	else {
		// 무조건 공성 특정상태 시작 날짜 보다 오늘 날짜가 더 늦다면 잘못된 것이다. -> 공성 특정상태 시작날짜가 늦는것은 물론 허용되며 그 사이의 시간은 무조건 그 전의 상태가 된다.
		INT64 iSIEGE_DATE_NUM64				= ((INT64) ((INT64)MAKELONG(m_tmFixCastleStateStartDate.wMinute, m_tmFixCastleStateStartDate.wHour) | (INT64)((INT64)MAKELONG(MAKEWORD(m_tmFixCastleStateStartDate.wDay, m_tmFixCastleStateStartDate.wMonth), m_tmFixCastleStateStartDate.wYear) << 32)));
		if (iSIEGE_DATE_NUM64 <= iTODAY_DATE_NUM64) {
			m_bFixCastleStateStartDate		= FALSE;
			MsgBox("[CastleSiege] CCastleSiege::LoadPreFixData() - iSIEGE_DATE_NUM64 (STATE:%d, %04d-%02d-%02d %02d:%02d:%02d) <= iTODAY_DATE_NUM64 (%04d-%02d-%02d %02d:%02d:%02d)",
				m_iFixCastleSpecificState,
				m_tmFixCastleStateStartDate.wYear,
				m_tmFixCastleStateStartDate.wMonth,
				m_tmFixCastleStateStartDate.wDay,
				m_tmFixCastleStateStartDate.wHour,
				m_tmFixCastleStateStartDate.wMinute,
				m_tmFixCastleStateStartDate.wSecond,
				tmToDay.wYear,
				tmToDay.wMonth,
				tmToDay.wDay,
				tmToDay.wHour,
				tmToDay.wMinute,
				tmToDay.wSecond
				);
		}
		else {
			m_bFixCastleStateStartDate		= TRUE;
		}

		// 임시 !!! -> 나중에 포함시켜야 함 <*>
		if (m_iFixCastleSpecificState == CASTLESIEGE_STATE_ENDSIEGE) {
			// 공성상태가 휴전기간이 되면 그 전 상태는 공성전 시작이 되므로 절대 이 상태는 받을 수 없다. !!!
			MsgBox("[CastleSiege] CCastleSiege::LoadPreFixData() - FIX STATE 'CASTLESIEGE_STATE_ENDSIEGE' CAN'T BE USED");
			m_bFixCastleStateStartDate		= FALSE;
		}

		if (!CHECK_LIMIT(m_tmFixCastleStateStartDate.wMonth, 12+1)	||
			!CHECK_LIMIT(m_tmFixCastleStateStartDate.wDay-1, 31)	||
			!CHECK_LIMIT(m_tmFixCastleStateStartDate.wHour, 24)	||
			!CHECK_LIMIT(m_tmFixCastleStateStartDate.wMinute, 60)
			)
		{
			m_bFixCastleStateStartDate		= FALSE;
		}
	}

	// 2 . 위의 데이터들은 파일에 유지되면 않되므로 초기화 한다.
	WritePrivateProfileString("GameServerInfo", "CastleCycleStartYear",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleCycleStartMonth",	"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleCycleStartDay",		"0", lpszFileName);

	WritePrivateProfileString("GameServerInfo", "CastleSpecificState",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartYear",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartMonth",	"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartDay",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartHour",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartMinute",	"0", lpszFileName);

	// 위에서 읽은 데이터들은 후에 공성 스케쥴을 설정할 때 영향을 미치게 된다.
	return (m_bFixCastleCycleStartDate & m_bFixCastleStateStartDate);
}


BOOL CCastleSiege::LoadData					(LPSTR lpszFileName)
{
	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) {
		MsgBox("[CastleSiege] file load error - File Name Error");
		return FALSE;
	}

	if (m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_1) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::LoadData() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_1 (%d)", m_iCastleDataLoadState);
		return FALSE;
	}

	// 새로운 공성 관련 (스케쥴, NPC) 데이터들을 읽는다.
	if((SMDFile=fopen(lpszFileName,"r")) == NULL)	
	{
		MsgBox("[CastleSiege] file load error - fopen() : %s", lpszFileName);
		return FALSE;
	}

	int type = -1;
	SMDToken Token;
	
	vector <CS_SCHEDULE_DATA>	vtScheduleData;
	vector <CS_NPC_DATA>		vtNpcData;
	map <unsigned int, int>		mpNpcDataCheck;

	int							iCountDfUpgrade_Gate	= 0;	// 카운트 - 방어력 업그레이드 데이터 (성문)
	int							iCountDfUpgrade_Statue	= 0;	// 카운트 - 방어력 업그레이드 데이터 (석상)
	int							iCountHpUpgrade_Gate	= 0;	// 카운트 - 체력 업그레이드 데이터 (성문)
	int							iCountHpUpgrade_Statue	= 0;	// 카운트 - 체력 업그레이드 데이터 (석상)
	int							iCountRgUpgrade_Statue	= 0;	// 카운트 - 회복력 업그레이드 데이터 (석상)
	int							iCountDfValue_Gate		= 0;	// 카운트 - 방어력 업그레이드 레벨 별 적용되는 실제 방어력 (성문)
	int							iCountDfValue_Statue	= 0;	// 카운트 - 방어력 업그레이드 레벨 별 적용되는 실제 방어력 (석상)

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{
			if( type == 0 )				// 공성 주기 정보
			{	
				// 공성전 관련 일반 데이터를 읽어들인다.
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				INT iCastleSiegeCycle						= (int)TokenNumber;

				// 에러체크
				if (iCastleSiegeCycle <= 0) {
					LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::LoadData() - iCastleSiegeCycle <= 0 : %d",
						iCastleSiegeCycle
						);
					return FALSE;
				}

				m_iCastleSiegeCycle							= iCastleSiegeCycle;
			}
			else if( type == 1 )		// 공성 기본 스케쥴 데이터
			{	
				// 공성전 스케쥴 정보를 읽어들인다.
				INT	iSTATE									= -1;
				INT	iSTART_DAY								= -1;
				INT	iSTART_HOUR								= -1;
				INT	iSTART_MIN								= -1;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iSTATE										= (int)TokenNumber;
				Token = (*GetToken)(); iSTART_DAY			= (int)TokenNumber;
				Token = (*GetToken)(); iSTART_HOUR			= (int)TokenNumber;
				Token = (*GetToken)(); iSTART_MIN			= (int)TokenNumber;

				// 에러체크
				if (!vtScheduleData.empty()) {
					vector <CS_SCHEDULE_DATA>::iterator it	= vtScheduleData.begin();
					for (; it != vtScheduleData.end() ; it++) {
						CS_SCHEDULE_DATA pScheduleData	= (*it);
						if (pScheduleData.m_bIN_USE) {
							if (pScheduleData.m_iSTATE == iSTATE) {
								// 1> . 같은 상태가 존재하는지 확인
								LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::LoadData() - Same State Exist : %d",
									iSTATE
									);
								return FALSE;
							}
							else if (pScheduleData.m_iSTATE > iSTATE) {
								// 2. 상태값이 크면 날짜도 뒤여야 한다.
								INT iSTATE1_TIME	= pScheduleData.m_iADD_DAY * 24 * 60 + pScheduleData.m_iADD_HOUR * 60 + pScheduleData.m_iADD_MIN;
								INT iSTATE2_TIME	= iSTART_DAY * 24 * 60 + iSTART_HOUR * 60 + iSTART_MIN;

								if (iSTATE1_TIME <= iSTATE2_TIME) {
									LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::LoadData() - Date Order is wrong : %d",
										iSTATE2_TIME
										);
									return FALSE;
								}
							}
							else {
								// 3 . 상태값이 작으면 날짜도 앞이어야 한다.
								INT iSTATE1_TIME	= pScheduleData.m_iADD_DAY * 24 * 60 + pScheduleData.m_iADD_HOUR * 60 + pScheduleData.m_iADD_MIN;
								INT iSTATE2_TIME	= iSTART_DAY * 24 * 60 + iSTART_HOUR * 60 + iSTART_MIN;

								if (iSTATE1_TIME >= iSTATE2_TIME) {
									LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::LoadData() - Date Order is wrong : %d",
										iSTATE2_TIME
										);
									return FALSE;
								}
							}
						}
					}
				}
					
				CS_SCHEDULE_DATA pScheduleDataInsert;
				pScheduleDataInsert.m_bIN_USE		= TRUE;
				pScheduleDataInsert.m_iSTATE		= iSTATE;
				pScheduleDataInsert.m_iADD_DAY		= iSTART_DAY;
				pScheduleDataInsert.m_iADD_HOUR		= iSTART_HOUR;
				pScheduleDataInsert.m_iADD_MIN		= iSTART_MIN;
				vtScheduleData.push_back(pScheduleDataInsert);
			}
			else if( type == 2 )		// 뮤 공/수성 NPC 데이터
			{	
				// NPC 정보를 읽어들인다.
				INT	iNPC_NUM		= -1;
				INT	iNPC_INDEX		= -1;
				INT	iNPC_SIDE		= 0;
				INT	iNPC_DBSAVE		= 0;
				INT	iNPC_DF_LEVEL	= 0;
				INT	iNPC_RG_LEVEL	= 0;
				INT	iNPC_MAXHP		= 0;
				INT	iNPC_SX			= -1;
				INT	iNPC_SY			= -1;
				INT	iNPC_DX			= -1;
				INT	iNPC_DY			= -1;
				INT	iNPC_DIR		= -1;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iNPC_NUM								= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_INDEX		= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_DBSAVE		= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_SIDE		= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_DF_LEVEL	= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_RG_LEVEL	= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_MAXHP		= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_SX			= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_SY			= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_DX			= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_DY			= (int)TokenNumber;
				Token = (*GetToken)(); iNPC_DIR			= (int)TokenNumber;


				// 에러체크
				if (iNPC_NUM < 0) {
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_NUM < 0)");
					return FALSE;
				}
				if (iNPC_INDEX < 0) {
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_INDEX < 0)");
					return FALSE;
				}
				if (iNPC_SIDE < 0) {
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_SIDE < 0)");
					return FALSE;
				}
				if (iNPC_DF_LEVEL < 0) {
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_DF_LEVEL < 0)");
					return FALSE;
				}
				if (iNPC_RG_LEVEL < 0) {
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_RG_LEVEL < 0)");
					return FALSE;
				}
				if (iNPC_MAXHP < 0) {
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_MAXHP < 0)");
					return FALSE;
				}
				if (iNPC_SX < 0) {
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_SX < 0)");
					return FALSE;
				}
				if (iNPC_SY < 0) {
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_SY < 0)");
					return FALSE;
				}

				unsigned int uNpcKey	= MAKELONG(iNPC_INDEX, iNPC_NUM);
				map<unsigned int, int>::iterator it	= mpNpcDataCheck.find(uNpcKey);
				if (it != mpNpcDataCheck.end()) {
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Same Npc Key Exist : NUM:%d-IDX:%d",
						iNPC_NUM,
						iNPC_INDEX
						);
					return FALSE;
				}
				else {
					mpNpcDataCheck.insert(pair<unsigned int, int>(MAKELONG(iNPC_INDEX, iNPC_NUM), 0));
				}

				CS_NPC_DATA pNpcDataInsert;
				pNpcDataInsert.m_bIN_USE		= TRUE;
				pNpcDataInsert.m_iNPC_NUM		= iNPC_NUM;
				pNpcDataInsert.m_iNPC_INDEX		= iNPC_INDEX;
				pNpcDataInsert.m_iNPC_SIDE		= iNPC_SIDE;
				pNpcDataInsert.m_bNPC_DBSAVE	= iNPC_DBSAVE;

				pNpcDataInsert.m_iNPC_DF_LEVEL	= pNpcDataInsert.m_iNPC_BASE_DF_LEVEL	= iNPC_DF_LEVEL;
				pNpcDataInsert.m_iNPC_RG_LEVEL	= pNpcDataInsert.m_iNPC_BASE_RG_LEVEL	= iNPC_RG_LEVEL;
				pNpcDataInsert.m_iNPC_MAXHP		= pNpcDataInsert.m_iNPC_BASE_MAXHP		= iNPC_MAXHP;
				pNpcDataInsert.m_iNPC_HP		= pNpcDataInsert.m_iNPC_BASE_HP			= iNPC_MAXHP;
				pNpcDataInsert.m_iNPC_SX		= pNpcDataInsert.m_iNPC_BASE_SX			= iNPC_SX;
				pNpcDataInsert.m_iNPC_SY		= pNpcDataInsert.m_iNPC_BASE_SY			= iNPC_SY;
				pNpcDataInsert.m_iNPC_DX		= pNpcDataInsert.m_iNPC_BASE_DX			= iNPC_DX;
				pNpcDataInsert.m_iNPC_DY		= pNpcDataInsert.m_iNPC_BASE_DY			= iNPC_DY;
				pNpcDataInsert.m_iNPC_DIR		= pNpcDataInsert.m_iNPC_BASE_DIR		= iNPC_DIR;
				vtNpcData.push_back(pNpcDataInsert);
			}
			else if( type == 3 )		//  방어력 업그레이드 데이터 (성문)
			{
				INT iLEVEL		= 0;	// 업그레이드 레벨
				INT	iGEMCOUNT	= 0;	// 필요 수호보석 개수
				INT iZEN		= 0;	// 필요 젠

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iLEVEL								= (int)TokenNumber;
				Token = (*GetToken)(); iGEMCOUNT	= (int)TokenNumber;
				Token = (*GetToken)(); iZEN			= (int)TokenNumber;

				if (iCountDfUpgrade_Gate >= MAX_UPGRADE_LEVEL)
					break;

				g_iNpcUpDfLevel_CGATE[iCountDfUpgrade_Gate][0]	= iLEVEL;
				g_iNpcUpDfLevel_CGATE[iCountDfUpgrade_Gate][1]	= iGEMCOUNT;
				g_iNpcUpDfLevel_CGATE[iCountDfUpgrade_Gate][2]	= iZEN;

				iCountDfUpgrade_Gate++;
			}
			else if( type == 4 )		// 방어력 업그레이드 데이터 (석상)
			{
				INT iLEVEL		= 0;	// 업그레이드 레벨
				INT	iGEMCOUNT	= 0;	// 필요 수호보석 개수
				INT iZEN		= 0;	// 필요 젠

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iLEVEL								= (int)TokenNumber;
				Token = (*GetToken)(); iGEMCOUNT	= (int)TokenNumber;
				Token = (*GetToken)(); iZEN			= (int)TokenNumber;

				if (iCountDfUpgrade_Statue >= MAX_UPGRADE_LEVEL)
					break;

				g_iNpcUpDfLevel_CSTATUE[iCountDfUpgrade_Statue][0]	= iLEVEL;
				g_iNpcUpDfLevel_CSTATUE[iCountDfUpgrade_Statue][1]	= iGEMCOUNT;
				g_iNpcUpDfLevel_CSTATUE[iCountDfUpgrade_Statue][2]	= iZEN;

				iCountDfUpgrade_Statue++;
			}
			else if( type == 5 )		// 체력 업그레이드 데이터 (성문)
			{
				INT iHP			= 0;	// 업그레이드 체력
				INT	iGEMCOUNT	= 0;	// 필요 수호보석 개수
				INT iZEN		= 0;	// 필요 젠

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iHP									= (int)TokenNumber;
				Token = (*GetToken)(); iGEMCOUNT	= (int)TokenNumber;
				Token = (*GetToken)(); iZEN			= (int)TokenNumber;

				if (iCountHpUpgrade_Gate >= MAX_UPGRADE_LEVEL)
					break;

				g_iNpcUpMaxHP_CGATE[iCountHpUpgrade_Gate][0]	= iHP;
				g_iNpcUpMaxHP_CGATE[iCountHpUpgrade_Gate][1]	= iGEMCOUNT;
				g_iNpcUpMaxHP_CGATE[iCountHpUpgrade_Gate][2]	= iZEN;

				iCountHpUpgrade_Gate++;
			}
			else if( type == 6 )		// 체력 업그레이드 데이터 (석상)
			{
				INT iHP			= 0;	// 업그레이드 체력
				INT	iGEMCOUNT	= 0;	// 필요 수호보석 개수
				INT iZEN		= 0;	// 필요 젠

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iHP									= (int)TokenNumber;
				Token = (*GetToken)(); iGEMCOUNT	= (int)TokenNumber;
				Token = (*GetToken)(); iZEN			= (int)TokenNumber;

				if (iCountHpUpgrade_Statue >= MAX_UPGRADE_LEVEL)
					break;

				g_iNpcUpMaxHP_CSTATUE[iCountHpUpgrade_Statue][0]	= iHP;
				g_iNpcUpMaxHP_CSTATUE[iCountHpUpgrade_Statue][1]	= iGEMCOUNT;
				g_iNpcUpMaxHP_CSTATUE[iCountHpUpgrade_Statue][2]	= iZEN;

				iCountHpUpgrade_Statue++;
			}
			else if( type == 7 )		// 회복력 업그레이드 데이터 (석상)
			{
				INT iLEVEL		= 0;	// 업그레이드 레벨
				INT	iGEMCOUNT	= 0;	// 필요 수호보석 개수
				INT iZEN		= 0;	// 필요 젠

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iLEVEL								= (int)TokenNumber;
				Token = (*GetToken)(); iGEMCOUNT	= (int)TokenNumber;
				Token = (*GetToken)(); iZEN			= (int)TokenNumber;

				if (iCountRgUpgrade_Statue >= MAX_UPGRADE_LEVEL)
					break;

				g_iNpcUpRgLevel_CSTATUE[iCountRgUpgrade_Statue][0]	= iLEVEL;
				g_iNpcUpRgLevel_CSTATUE[iCountRgUpgrade_Statue][1]	= iGEMCOUNT;
				g_iNpcUpRgLevel_CSTATUE[iCountRgUpgrade_Statue][2]	= iZEN;

				iCountRgUpgrade_Statue++;
			}
			else if( type == 8 )		// 방어력 업그레이드 레벨 별 적용되는 실제 방어력 (성문)
			{
				INT iDFVALUE	= 0;	// 방어력 수치
			
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iDFVALUE							= (int)TokenNumber;

				if (iCountDfValue_Gate >= MAX_UPGRADE_LEVEL+1)
					break;

				g_iNpcDefense_CGATE[iCountDfValue_Gate]				= iDFVALUE;

				iCountDfValue_Gate++;
			}
			else if( type == 9 )		// 방어력 업그레이드 레벨 별 적용되는 실제 방어력 (석상)
			{
				INT iDFVALUE	= 0;	// 방어력 수치
			
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iDFVALUE							= (int)TokenNumber;

				if (iCountDfValue_Statue >= MAX_UPGRADE_LEVEL+1)
					break;

				g_iNpcDefense_CSTATUE[iCountDfValue_Statue]			= iDFVALUE;

				iCountDfValue_Statue++;
			}
		}
	}
	
	fclose(SMDFile);

	// 기존 데이터 초기화
//	Clear();
				
	// 데이터 세팅 -> 1 . 스케쥴
	EnterCriticalSection (&m_critScheduleData);
	if (!vtScheduleData.empty()) {
		vector <CS_SCHEDULE_DATA>::iterator it	= vtScheduleData.begin();
		
		for (; it != vtScheduleData.end() ; it++) {
			m_vtScheduleData.push_back((*it));
		}
	}
	// 순서대로 진행시키기 위해 정렬
	sort(m_vtScheduleData.begin(), m_vtScheduleData.end(), ScheduleStateCompFunc);
	// 다음 상태와의 시간 간격을 미리 계산한다. (SEC)
	if(!m_vtScheduleData.empty()) {
		for (INT iIDX = 0 ; iIDX < m_vtScheduleData.size() ; iIDX++) {
			if (iIDX + 1 < m_vtScheduleData.size()) {
				INT iIDX_SEC1		= m_vtScheduleData[iIDX].m_iADD_DAY*24*60*60 + m_vtScheduleData[iIDX].m_iADD_HOUR*60*60 + m_vtScheduleData[iIDX].m_iADD_MIN*60;
				INT iIDX_SEC2		= m_vtScheduleData[iIDX+1].m_iADD_DAY*24*60*60 + m_vtScheduleData[iIDX+1].m_iADD_HOUR*60*60 + m_vtScheduleData[iIDX+1].m_iADD_MIN*60;
				INT iIDX_RESULT		= iIDX_SEC2 - iIDX_SEC1;

				if (iIDX_RESULT < 0) {
					LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::LoadData() - Date Order is wrong (sort fail) : %d-%d",
						m_vtScheduleData[iIDX].m_iSTATE,
						m_vtScheduleData[iIDX+1].m_iSTATE
						);
				}
				else {
					m_vtScheduleData[iIDX].m_iGAP_SEC	= iIDX_RESULT;
				}
			}
			else {
				// 주기의 맨 마지막 값이다.
				m_vtScheduleData[iIDX].m_iGAP_SEC		= 0;
			}
		}
	}
	else {
		// 공성주기 스케쥴 정보가 하나도 없다.
		// 임시 !!!
	}
	LeaveCriticalSection (&m_critScheduleData);
				
	// 데이터 세팅 - 2 . NPC 정보
	EnterCriticalSection (&m_critNpcData);
	if (!vtNpcData.empty()) {
		vector <CS_NPC_DATA>::iterator it	= vtNpcData.begin();
		for (; it != vtNpcData.end() ; it++) {
			m_vtNpcData.push_back((*it));
		}
	}
	LeaveCriticalSection (&m_critNpcData);

	//-----------<확인용테스트>-----------
	vector <CS_SCHEDULE_DATA>::iterator it1	= m_vtScheduleData.begin();
	while (it1 != m_vtScheduleData.end()) {
		CS_SCHEDULE_DATA stTEMP	= (*it1);
		CHAR szTEMP[256]	= {0,};
		wsprintf(szTEMP, "STT:%d, DAY:%d, HOUR:%d, MIN:%d, GAP:%d\n", 
			stTEMP.m_iSTATE,
			stTEMP.m_iADD_DAY,
			stTEMP.m_iADD_HOUR,
			stTEMP.m_iADD_MIN,
			stTEMP.m_iGAP_SEC
			);
		OutputDebugString(szTEMP);
		it1++;
	}

	vector <CS_NPC_DATA>::iterator it2	= m_vtNpcData.begin();
	while (it2 != m_vtNpcData.end()) {
		CS_NPC_DATA stTEMP		= (*it2);
		CHAR szTEMP[256]	= {0,};
		wsprintf(szTEMP, "NUM:%d, IDX:%d, DBSAVE:%s, SIDE:%d, DFLEVEL:%d, RGLEVEL:%d, MXHP:%d, (%d,%d)-(%d,%d), DIR:%d\n", 
			stTEMP.m_iNPC_NUM,
			stTEMP.m_iNPC_INDEX,
			(stTEMP.m_bNPC_DBSAVE)?"YES":"NO",
			stTEMP.m_iNPC_SIDE,
			stTEMP.m_iNPC_DF_LEVEL,
			stTEMP.m_iNPC_RG_LEVEL,
			stTEMP.m_iNPC_MAXHP,
			stTEMP.m_iNPC_SX,
			stTEMP.m_iNPC_SY,
			stTEMP.m_iNPC_DX,
			stTEMP.m_iNPC_DY,
			stTEMP.m_iNPC_DIR
			);
		OutputDebugString(szTEMP);
		it2++;
	}
	//-----------<확인용테스트>-----------

	m_bFileDataLoadOK					= TRUE;

	return TRUE;
}


BOOL CCastleSiege::DataRequest			()
{
	// 공성전 관련 데이터가 덜 읽혀졌을 경우에는 초기화 될 수 없다.
	if (m_bFileDataLoadOK == FALSE) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::DataRequest() - m_bFileDataLoadOK == FALSE");
		return FALSE;
	}

	// 지정된 맵 서버군이 0 보다 작아서는 않된다.
	if (m_iMapSvrGroup < 0) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::DataRequest() - m_iMapSvrGroup < 0");
		return FALSE;
	}

	if (m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::DataRequest() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2 (%d)", m_iCastleDataLoadState);
		return FALSE;
	}

	// 공성전 초기화 데이터를 요구
	BOOL bDbRequestData					= FALSE;
	if (!m_dwDbDataLoadTickCount) {
		bDbRequestData					= TRUE;
		m_dwDbDataLoadTickCount			= GetTickCount();
		LogAddC(LOGC_GREEN, "[CastleSiege] CCastleSiege::DataRequest() - REQUEST DATA FIRST");
	}
	else {
		if (GetTickCount() - m_dwDbDataLoadTickCount > MAX_DBDATA_WAIT_TICK) {
			bDbRequestData				= TRUE;
			m_dwDbDataLoadTickCount		= GetTickCount();
			LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::DataRequest() - REQUEST DATA AGAIN");
		}
	}

	if (bDbRequestData) {
		GS_GDReqCastleInitData	(m_iMapSvrGroup, m_iCastleSiegeCycle);
	}
	
	return TRUE;
}


BOOL CCastleSiege::Init						()
{
	if (m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::Init() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4 (%d)", m_iCastleDataLoadState);
		MsgBox("[CastleSiege] CCastleSiege::Init() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4 (%d)", m_iCastleDataLoadState);
		return FALSE;
	}

	if (!m_bFileDataLoadOK || !m_bDbDataLoadOK) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::Init() - Data Load Fail (FILE:%d, DB:%d)", m_bFileDataLoadOK, m_bDbDataLoadOK);
		MsgBox("[CastleSiege] CCastleSiege::Init() - Data Load Fail (FILE:%d, DB:%d)", m_bFileDataLoadOK, m_bDbDataLoadOK);
		return FALSE;
	}
		
	LogAddTD("[CastleSiege] CCastleSiege::Init() - START", m_bFileDataLoadOK, m_bDbDataLoadOK);
	m_iCastleDataLoadState	= CASTLESIEGE_DATALOAD_NONE;	// 데이터 로드 상태를 원상태로 돌린다.

	// 공성전 성문의 레버를 생성한다.
	CreateAllCsGateLever();

	// 공성전 시간을 세팅한다.
	if (CheckSync() == FALSE) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
		MsgBox("[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
		return FALSE;
	}

	// 이제 공성전 스케쥴 루프를 진행해도 된다.
	m_bDoRun								= TRUE;
	m_dwNPC_DBSAVE_TICK_COUNT				= GetTickCount();
	m_dwCALC_LEFTSIEGE_TICK_COUNT			= 0;
	m_dwEVENT_MSG_TICK_COUNT				= 0;
	m_dwCHECK_GATE_ALIVE_COUNT				= 0;
	m_dwCS_STARTTIME_TICK_COUNT				= 0;
	m_dwCS_LEFTTIME_TICK_COUNT				= 0;
	m_dwCS_JOINSIDE_REFRESH_TICK_COUNT		= 0;
	m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT		= 0;
	m_dwCS_MINIMAP_SEND_TICK_COUNT			= 0;
	return TRUE;
}


VOID CCastleSiege::SetState					(INT iCastleSiegeState, BOOL bSetRemainMsec)				// 공성전 스케쥴러를 특정 상태로 변경한다.
{
	if ((iCastleSiegeState < CASTLESIEGE_STATE_NONE) || (iCastleSiegeState > CASTLESIEGE_STATE_ENDCYCLE))
		return;

	m_iCastleSiegeState						= iCastleSiegeState;

	switch(m_iCastleSiegeState) {
	case CASTLESIEGE_STATE_NONE :			// 상태없음
		SetState_NONE						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_IDLE_1 :			// 유휴기간 - 1
		SetState_IDLE_1						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_REGSIEGE :		// 신청기간	(공성주기 시작)
		SetState_REGSIEGE					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_IDLE_2 :			// 유휴기간 - 2
		SetState_IDLE_2						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_REGMARK :		// 문장 등록기간
		SetState_REGMARK					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_IDLE_3 :			// 유휴기간 - 3
		SetState_IDLE_3						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_NOTIFY :			// 발표기간
		SetState_NOTIFY						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_READYSIEGE :		// 준비기간
		SetState_READYSIEGE					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_STARTSIEGE :		// 공성시작
		SetState_STARTSIEGE					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_ENDSIEGE :		// 휴전기간
		SetState_ENDSIEGE					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_ENDCYCLE :		// 공성주기 종료
		SetState_ENDCYCLE					(bSetRemainMsec);
		break;
	}

	// *> . 상태가 바뀔 때 마다 할 일들
	if (m_bDbNpcCreated == FALSE) {
		// 아직 DB NPC들이 생성되지 않았다면 여기서 해준다.
		m_bDbNpcCreated			= TRUE;
		CreateDbNPC();
	}

	// 상태가 바뀐 시간을 상태 시작시간으로 저장한다.
	GetLocalTime(&m_tmStateStartDate);

	m_dwEVENT_MSG_TICK_COUNT				= 0;
	m_dwCHECK_GATE_ALIVE_COUNT				= 0;
}


VOID CCastleSiege::Run						()
{
	if (m_bDoRun == FALSE)					// 데이터를 읽을 때 등은 돌지 않는다.
		return;

	if (m_bCastleSiegeEnable == FALSE)		// 공성전이 비활성화 된 경우 돌지 않는다.
		return;

	switch(m_iCastleSiegeState) {
	case CASTLESIEGE_STATE_NONE :			// 상태없음
		ProcState_NONE						();
		break;
	case CASTLESIEGE_STATE_IDLE_1 :			// 유휴기간 - 1
		ProcState_IDLE_1					();
		break;
	case CASTLESIEGE_STATE_REGSIEGE :		// 신청기간	(공성주기 시작)
		ProcState_REGSIEGE					();
		break;
	case CASTLESIEGE_STATE_IDLE_2 :			// 유휴기간 - 2
		ProcState_IDLE_2					();
		break;
	case CASTLESIEGE_STATE_REGMARK :		// 문장 등록기간
		ProcState_REGMARK					();
		break;
	case CASTLESIEGE_STATE_IDLE_3 :			// 유휴기간 - 3
		ProcState_IDLE_3					();
		break;
	case CASTLESIEGE_STATE_NOTIFY :			// 발표기간
		ProcState_NOTIFY					();
		break;
	case CASTLESIEGE_STATE_READYSIEGE :		// 준비기간
		ProcState_READYSIEGE				();
		break;
	case CASTLESIEGE_STATE_STARTSIEGE :		// 공성시작
		ProcState_STARTSIEGE				();
		break;
	case CASTLESIEGE_STATE_ENDSIEGE :		// 휴전기간
		ProcState_ENDSIEGE					();
		break;
	case CASTLESIEGE_STATE_ENDCYCLE :		// 공성주기 종료
		ProcState_ENDCYCLE					();
		break;
	}

	// 실행될 때 마다 체크할 것들

	// 1 . 일정 시간마다 NPC 정보를 저장한다.
	if (GetTickCount() - m_dwNPC_DBSAVE_TICK_COUNT > MAX_NPC_DBSAVE_TICK) {
		if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE) {
			StoreDbNpc();
		}
		m_dwNPC_DBSAVE_TICK_COUNT		= GetTickCount();
	}

	// 2 . 일정 시간마다 공성전 예상 시작시간을 계산한다.
	if (GetTickCount() - m_dwCALC_LEFTSIEGE_TICK_COUNT > MAX_CALC_LEFTSIEGE_TICK) {
		m_dwCALC_LEFTSIEGE_TICK_COUNT	= GetTickCount();
		CalcCastleLeftSiegeDate();
	}

	// 임시 !!!
	// 테스트용 - 현재 스케쥴 진행 상태를 출력
	static INT	sttNowTick	= 0;
	if (GetTickCount() - sttNowTick > 3000) {
		sttNowTick			= GetTickCount();
		INT iRET_SEC		= CalcCastleLeftSiegeDate ();
		if (iRET_SEC >= 0) {
			SYSTEMTIME tmRetDate	= GetCastleLeftSiegeDate();
			LogAddC(LOGC_BLUE, "LEFT : SEC(%d), SIEGE-DAY(%04d-%02d-%02d %02d:%02d:%02d)",
				iRET_SEC,
				tmRetDate.wYear,
				tmRetDate.wMonth,
				tmRetDate.wDay,
				tmRetDate.wHour,
				tmRetDate.wMinute,
				tmRetDate.wSecond
				);
		}
		else {
			switch(iRET_SEC) {
			case CASTLESIEGE_LEFTTIME_ONSIGE :
				LogAddC(LOGC_BLUE, "[CastleSiege] LEFT-TIME : CASTLESIEGE_LEFTTIME_ONSIGE");
				break;
			case CASTLESIEGE_LEFTTIME_ENDSIEGE :
				LogAddC(LOGC_BLUE, "[CastleSiege] LEFT-TIME : CASTLESIEGE_LEFTTIME_ENDSIEGE");
				break;
			case CASTLESIEGE_LEFTTIME_NOTRUNNING :
				LogAddC(LOGC_BLUE, "[CastleSiege] LEFT-TIME : CASTLESIEGE_LEFTTIME_NOTRUNNING");
				break;
			}
		}
	}
}


VOID CCastleSiege::SetState_NONE			(BOOL bSetRemainMsec)
{
	
}


VOID CCastleSiege::SetState_IDLE_1			(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_IDLE_1()");

	if (bSetRemainMsec) {
		// 여기서는 무조건 최초 공성 시작시간까지의 간격을 계산한다.
		time_t	ttSTime;
		time_t	ttETime;
		tm		tmETime;

		memset (&tmETime, 0, sizeof(tmETime));
		tmETime.tm_year						= m_tmStartDate.wYear	- 1900;
		tmETime.tm_mon						= m_tmStartDate.wMonth	- 1;
		tmETime.tm_mday						= m_tmStartDate.wDay;
		ttETime								= mktime(&tmETime);

		time(&ttSTime);
	
		double	dResultSecond				= difftime(ttETime, ttSTime);
		m_iCS_REMAIN_MSEC					= dResultSecond * 1000;
	}
}


VOID CCastleSiege::SetState_REGSIEGE		(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_REGSIEGE()");

	if (bSetRemainMsec) {
		INT iGAP_SEC		= GetStateGapSec(CASTLESIEGE_STATE_REGSIEGE);
		if (iGAP_SEC < 0) {
			m_bDoRun		= FALSE;
			MsgBox("[CastleSiege] CCastleSiege::SetState_REGSIEGE() - iGAP_SEC < 0");
		}
		m_iCS_REMAIN_MSEC	= iGAP_SEC * 1000;
	}
	
	// 공성 주기가 최초 시작할 때 할 일들

	// 1 . 공성의 필요한 데이터들을 초기화 한다.
	GS_GDReqRestartCastleState (m_iMapSvrGroup);
}


VOID CCastleSiege::SetState_IDLE_2			(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_IDLE_2()");

	if (bSetRemainMsec) {
		INT iGAP_SEC		= GetStateGapSec(CASTLESIEGE_STATE_IDLE_2);
		if (iGAP_SEC < 0) {
			m_bDoRun		= FALSE;
			MsgBox("[CastleSiege] CCastleSiege::SetState_IDLE_2() - iGAP_SEC < 0");
		}
		m_iCS_REMAIN_MSEC	= iGAP_SEC * 1000;
	}
}


VOID CCastleSiege::SetState_REGMARK			(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_REGMARK()");

	if (bSetRemainMsec) {
		INT iGAP_SEC		= GetStateGapSec(CASTLESIEGE_STATE_REGMARK);
		if (iGAP_SEC < 0) {
			m_bDoRun		= FALSE;
			MsgBox("[CastleSiege] CCastleSiege::SetState_REGMARK() - iGAP_SEC < 0");
		}
		m_iCS_REMAIN_MSEC	= iGAP_SEC * 1000;
	}
}


VOID CCastleSiege::SetState_IDLE_3			(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_IDLE_3()");

	if (bSetRemainMsec) {
		INT iGAP_SEC		= GetStateGapSec(CASTLESIEGE_STATE_IDLE_3);
		if (iGAP_SEC < 0) {
			m_bDoRun		= FALSE;
			MsgBox("[CastleSiege] CCastleSiege::SetState_IDLE_3() - iGAP_SEC < 0");
		}
		m_iCS_REMAIN_MSEC	= iGAP_SEC * 1000;
	}
}


VOID CCastleSiege::SetState_NOTIFY			(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_NOTIFY()");

	if (bSetRemainMsec) {
		INT iGAP_SEC		= GetStateGapSec(CASTLESIEGE_STATE_NOTIFY);
		if (iGAP_SEC < 0) {
			m_bDoRun		= FALSE;
			MsgBox("[CastleSiege] CCastleSiege::SetState_NOTIFY() - iGAP_SEC < 0");
		}
		m_iCS_REMAIN_MSEC	= iGAP_SEC * 1000;
	}

	// 1. 공/수성 길드정보를 구축한다.
	CheckBuildCsGuildInfo();
}


VOID CCastleSiege::SetState_READYSIEGE		(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_READYSIEGE()");

	if (bSetRemainMsec) {
		INT iGAP_SEC		= GetStateGapSec(CASTLESIEGE_STATE_READYSIEGE);
		if (iGAP_SEC < 0) {
			m_bDoRun		= FALSE;
			MsgBox("[CastleSiege] CCastleSiege::SetState_READYSIEGE() - iGAP_SEC < 0");
		}
		m_iCS_REMAIN_MSEC	= iGAP_SEC * 1000;
	}

	// 1 . 공/수성 길드정보를 구축한다.
	CheckBuildCsGuildInfo();
}


VOID CCastleSiege::SetState_STARTSIEGE		(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_STARTSIEGE()");

	if (bSetRemainMsec) {
		INT iGAP_SEC		= GetStateGapSec(CASTLESIEGE_STATE_STARTSIEGE);
		if (iGAP_SEC < 0) {
			m_bDoRun		= FALSE;
			MsgBox("[CastleSiege] CCastleSiege::SetState_STARTSIEGE() - iGAP_SEC < 0");
		}
		m_iCS_REMAIN_MSEC	= iGAP_SEC * 1000;
	}

	// 공성전 시작 직전에 할 작업들

	// 임시 !!! -> 나중에 제거해야 함 <*>
	// 공/수성 길드정보를 구축한다.
//	CheckBuildCsGuildInfo();

	// *> . 공성 관련 변수 초기화
	m_bCastleTowerAccessable			= FALSE;
	m_bRegCrownAvailable				= FALSE;
	m_iCastleCrownAccessUser			= -1;		// 왕관 에 접근한 사용자
	m_btCastleCrownAccessUserX			= 0;
	m_btCastleCrownAccessUserY			= 0;
	m_iCastleSwitchAccessUser1			= -1;		// 왕관 스위치 1 에 접근한 사용자
	m_iCastleSwitchAccessUser2			= -1;		// 왕관 스위치 2 에 접근한 사용자
	m_dwCrownAccessTime					= 0;		// 왕관 접근시간 초기화
	m_dwCHECK_GATE_ALIVE_COUNT			= 0;
	m_dwCS_STARTTIME_TICK_COUNT			= 0;
	m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();		// 공성시작 후 바로 남은 시간을 전송하지 않도록
	m_dwCS_JOINSIDE_REFRESH_TICK_COUNT	= GetTickCount();		// 공성시작 후 바로 공/수 참여상태를 갱신하지 않도록
	m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT	= 0;
	m_dwCS_MINIMAP_SEND_TICK_COUNT		= GetTickCount();
	memset (m_szMiddleWinnerGuild, 0, sizeof(m_szMiddleWinnerGuild));

	// 임시 !!! -> 나중에 추가해야 함 <*>
	// *> . 공성측 길드들이 존재하는지 확인한다. (이전 상태까지 공성길드 데이터 구축 완료) -> 없으면 공성상태 종료
	if (CheckAttackGuildExist() == FALSE) {
		CheckCastleSiegeResult();
		SetState (CASTLESIEGE_STATE_ENDSIEGE);
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::SetState_STARTSIEGE() - CheckAttackGuildExist() == FALSE");
		return;
	}

	// 1 . DB NPC들 (성문, 석상)의 정보를 DB에 저장한다.
	StoreDbNpc ();

	// 2 . 공성전 관련 NPC들 (발석차, 캐논타워, 방어막 등등) 을 제거, 생성
	ClearNonDbNPC();
	CheckReviveNonDbNPC ();

	// 3 . 만약 없는 수호석상이 있다면 생성
#ifdef REMOVE_CASTLESIEGE_AUTO_REVIVE_GUARDIANSTATUE_20051101
	if (CheckGuardianStatueExist() == FALSE) 
	{
		// 모든 석상이 제거되었다. -> 모든 방어막을 제거하고 공성측도 성에 올라갈 수 있도록 한다.
		ClearCastleTowerBarrier();
		SetCastleTowerAccessable(TRUE);
	}
#else
	CheckReviveGuardianStatue ();
#endif

	// 4 . 몬스터 객체들 (성문, 석상) 에게 자신의 방어력, 회복력 레벨을 적용시킴
	AdjustDbNpcLevel ();

	// 5 . 모든 존재하는 성문들을 닫은 상태로 변경
	SetAllCastleGateState (CS_GATE_STATE_CLOSED);

	// 6 . 전체 사용자들의 공/수 참여속성을 전부 찾아서 갱신함 (공/수 구분)
	SetAllUserCsJoinSide ();

#ifdef CASTLE_SAVE_PCROOM_USERLIST_20050105
	// *> . 공/수 속성이 정해진 직후에 PC방 정보를 저장한다.
	SavePcRoomUserList ();
#endif

	// 7 . 공성측만 성밖으로 리스폰
	ReSpawnEnemyUser(TRUE);

	// 7 . 공/수 그리고 그외의 사람들을 적당한 장소로 리스폰
//	ReSpawnAllUser ();

	// 8 . 전체 사용자 들에게 공성전이 시작되었음을 알림
	NotifyAllUserCsStartState (1);
	
	// 9 . 최초의 공성전 시간을 보내준다.
//	GCAnsCsLeftTimeAlarm (
//		(m_iCS_REMAIN_MSEC/1000)/3600,			// 시간
//		((m_iCS_REMAIN_MSEC/1000)%3600)/60		// 분
//		);
}


VOID CCastleSiege::SetState_ENDSIEGE		(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_ENDSIEGE()");

	if (bSetRemainMsec) {
#ifdef CASTLE_TEST_SCHEDULER_20041218			// 테스트 스케쥴러 적용 시
		m_iCS_REMAIN_MSEC					= CS_TEST_GAPSEC * 1000;
#else
		// 여기서는 공성주기 종료날짜까지의 남은 시간을 다시 계산하여 입력한다.
		time_t	ttSTime;
		time_t	ttETime;
		tm		tmETime;

		memset (&tmETime, 0, sizeof(tmETime));
		tmETime.tm_year						= m_tmEndDate.wYear		- 1900;
		tmETime.tm_mon						= m_tmEndDate.wMonth	- 1;
		tmETime.tm_mday						= m_tmEndDate.wDay;

		time(&ttSTime);
		ttETime								= mktime(&tmETime);
	
		double	dResultSecond				= difftime(ttETime, ttSTime);
		m_iCS_REMAIN_MSEC					= dResultSecond * 1000;
#endif
	}

	// 공성전 종료 직후 -> 휴전 기간 직전에 할 작업들

	// 1 . 공성전 관련 NPC들을 소멸 (DB NPC 제외 - 성문, 석상, 성문레버)
	ClearNonDbNPC ();

	// 2 . 전체 서버 사용자들의 공/수 속성 해제
	ResetAllUserCsJoinSide ();

	// 3 . 공성전 종료 메시지 출력
	SendAllUserAnyMsg(lMsg.Get(1620));		// "공성전이 종료되었습니다."
}


VOID CCastleSiege::SetState_ENDCYCLE		(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_ENDCYCLE()");

	// 공성주기의 끝이므로 필요한 마무리 작업을 끝마친 후 최초 상태로 넘어가야 한다.

	// 1 . 다음 시작시간, 끝시간을 설정하고 DB에 저장한다. -> 오늘 날짜가 시작시간이 된다.
	SYSTEMTIME tmNowDate;
	GetLocalTime(&tmNowDate);
	tmNowDate.wHour			= 0;
	tmNowDate.wMinute		= 0;
	tmNowDate.wSecond		= 0;
	tmNowDate.wMilliseconds	= 0;

#ifdef MODIFY_SIEGE_END_DATE_BUG_FIX_20060116
	// 공성 종료시 예상 스케쥴의 종료일정을 다음 공성의 시작시간으로 설정한다.
	tmNowDate				= m_tmSiegeEndSchedule;
	m_tmStartDate			= m_tmSiegeEndSchedule;
	GetNextDay( tmNowDate, m_iCastleSiegeCycle );
	GetNextDay( m_tmSiegeEndSchedule, m_iCastleSiegeCycle );
	m_tmEndDate				= tmNowDate;
	LogAddTD( "[CastleSiege] State - End Cycle Schedule : (%d-%d-%d(%d:%d:%d)) (%d-%d-%d(%d:%d:%d))",
		m_tmStartDate.wYear, m_tmStartDate.wMonth, m_tmStartDate.wDay, m_tmStartDate.wHour, m_tmStartDate.wMinute, m_tmStartDate.wSecond,
		m_tmEndDate.wYear, m_tmEndDate.wMonth, m_tmEndDate.wDay, m_tmEndDate.wHour, m_tmEndDate.wMinute, m_tmEndDate.wSecond );
#else
	m_tmStartDate			= tmNowDate;
	GetNextDay(tmNowDate, m_iCastleSiegeCycle);
	m_tmEndDate				= tmNowDate;
#endif

#ifdef	ADD_CASTLESIEGE_DATE_UPDATING_LOG_20051103
	LogAddTD( "[CastleSiege] State - End Cycle : Date-Changing Info (%d-%d-%d) (%d-%d-%d)",
		m_tmStartDate.wYear, m_tmStartDate.wMonth, m_tmStartDate.wDay, 
		m_tmEndDate.wYear, m_tmEndDate.wMonth, m_tmEndDate.wDay 
		);
#endif

	GS_GDReqSiegeDateChange (
		m_iMapSvrGroup,				
		-1,							// 인덱스는 필요없음
		m_tmStartDate.wYear,		
		m_tmStartDate.wMonth,		
		m_tmStartDate.wDay, 
		m_tmEndDate.wYear, 
		m_tmEndDate.wMonth, 
		m_tmEndDate.wDay
		);


	// 1 . 공성전 참여신청 길드 리스트를 초기화 함
	GS_GDReqResetRegSiegeInfo (m_iMapSvrGroup);

	// 2 . 공성 참여길드 리스트를 초기화 함
	GS_GDReqResetSiegeGuildInfo (m_iMapSvrGroup);
	
	// 3 . 공성주기가 끝나자 마자 다음 공성주기를 시작
	SetState (CASTLESIEGE_STATE_REGSIEGE);
}


VOID CCastleSiege::ProcState_NONE			()
{
	
}


VOID CCastleSiege::ProcState_IDLE_1			()
{
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC		-= iTICK_MSEC;
		m_iCS_TICK_COUNT		= GetTickCount();


		LogAddC(LOGC_GREEN, "RUN ProcState_IDLE_1()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		SetState (CASTLESIEGE_STATE_REGSIEGE);
	}
}


VOID CCastleSiege::ProcState_REGSIEGE		()
{
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC		-= iTICK_MSEC;
		m_iCS_TICK_COUNT		= GetTickCount();


		if (GetTickCount() - m_dwEVENT_MSG_TICK_COUNT > CS_MSG_TICK_REGSIEGE) {
			SendAllUserAnyMsg(lMsg.Get(1614));		// "공성전 참여 신청 기간 입니다."
			m_dwEVENT_MSG_TICK_COUNT	= GetTickCount();
		}

		LogAddC(LOGC_GREEN, "RUN ProcState_REGSIEGE()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		SetState (CASTLESIEGE_STATE_IDLE_2);
	}
}


VOID CCastleSiege::ProcState_IDLE_2			()
{
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC		-= iTICK_MSEC;
		m_iCS_TICK_COUNT		= GetTickCount();


		LogAddC(LOGC_GREEN, "RUN ProcState_IDLE_2()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		SetState (CASTLESIEGE_STATE_REGMARK);
	}
}


VOID CCastleSiege::ProcState_REGMARK		()
{
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC		-= iTICK_MSEC;
		m_iCS_TICK_COUNT		= GetTickCount();

		if (GetTickCount() - m_dwEVENT_MSG_TICK_COUNT > CS_MSG_TICK_REGMARK) {
			SendAllUserAnyMsg(lMsg.Get(1615));		// "공성전 참여 신청 길드는 성주의 표식을 등록해 주세요."
			m_dwEVENT_MSG_TICK_COUNT	= GetTickCount();
		}

		LogAddC(LOGC_GREEN, "RUN ProcState_REGMARK()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		SetState (CASTLESIEGE_STATE_IDLE_3);
	}
}


VOID CCastleSiege::ProcState_IDLE_3			()
{
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC		-= iTICK_MSEC;
		m_iCS_TICK_COUNT		= GetTickCount();

		if (GetTickCount() - m_dwEVENT_MSG_TICK_COUNT > CS_MSG_TICK_IDLE_3) {
			SendAllUserAnyMsg(lMsg.Get(1616));		// "공성전에 참여할 길드를 선발하는 기간입니다."
			m_dwEVENT_MSG_TICK_COUNT	= GetTickCount();
		}

		LogAddC(LOGC_GREEN, "RUN ProcState_IDLE_3()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		SetState (CASTLESIEGE_STATE_NOTIFY);
	}
}


VOID CCastleSiege::ProcState_NOTIFY			()
{
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC		-= iTICK_MSEC;
		m_iCS_TICK_COUNT		= GetTickCount();

		if (GetTickCount() - m_dwEVENT_MSG_TICK_COUNT > CS_MSG_TICK_NOTIFY) {
			SendAllUserAnyMsg(lMsg.Get(1617));		// "공성전에 참여할 길드들이 선정되었습니다."
			m_dwEVENT_MSG_TICK_COUNT	= GetTickCount();
		}

		LogAddC(LOGC_GREEN, "RUN ProcState_NOTIFY()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		SetState (CASTLESIEGE_STATE_READYSIEGE);
	}
}


VOID CCastleSiege::ProcState_READYSIEGE		()
{
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC		-= iTICK_MSEC;
		m_iCS_TICK_COUNT		= GetTickCount();

		if (GetTickCount() - m_dwEVENT_MSG_TICK_COUNT > CS_MSG_TICK_READYSIEGE) {
			SendAllUserAnyMsg(lMsg.Get(1618));			// "공성전 준비 기간입니다. 공성전에 참여하시는 길드원들께서는 공성전을 준비해주시기 바랍니다."
			m_dwEVENT_MSG_TICK_COUNT	= GetTickCount();
		}

#ifdef MU_CASTLESIEGE_START_NOTIFY_20050418
		// 공성전 시작 전 남은 시간이 N분 이하일 때는 M분 마다 남은시간 전송
		if (m_iCS_REMAIN_MSEC <= CS_STARTNOTIFY_MIN_TICK) {
			if (GetTickCount() - m_dwCS_STARTTIME_TICK_COUNT > CS_STARTNOTIFY_PER_TICK) {
				CHAR szText[256] = {0,};
				wsprintf(szText, lMsg.Get(1724), m_iCS_REMAIN_MSEC/60000 + 1);		// "공성전 시작 %d분 전 입니다."
				SendAllUserAnyMsg(szText, 2);		
				m_dwCS_STARTTIME_TICK_COUNT			= GetTickCount();
			}
		}
#endif		

		LogAddC(LOGC_GREEN, "RUN ProcState_READYSIEGE()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		SetState (CASTLESIEGE_STATE_STARTSIEGE);
	}
}


VOID CCastleSiege::ProcState_STARTSIEGE		()
{
#ifdef MODIFY_CASTLE_SIEGE_CYCLE_RESET_20071009	// 공성전의 중간 승리체크를 1초에 1번 -> 1초에 10번으로 바꾼다.
	// 현재 공성전의 중간 승리를 계속 체크한다.
	CheckMiddleWinnerGuild ();
#endif
	
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC							-= iTICK_MSEC;
		m_iCS_TICK_COUNT							= GetTickCount();

#ifndef MODIFY_CASTLE_SIEGE_CYCLE_RESET_20071009
		// 현재 공성전의 중간 승리를 계속 체크한다.
		CheckMiddleWinnerGuild ();
#endif		

		// 성문이 죽어있는지 체크하고 속성을 없애줌
		if (GetTickCount() - m_dwCHECK_GATE_ALIVE_COUNT > CS_CHECK_DBNPC_ALIVE_TICK) {
			CheckCsDbNpcAlive ();
			m_dwCHECK_GATE_ALIVE_COUNT				= GetTickCount();
		}

		// 공성전 남은 시간을 사용자들에게 전송해 주는 시간
		if (GetTickCount() - m_dwCS_LEFTTIME_TICK_COUNT > DEFAULT_CS_LEFT_ALARM_TICK) {
			if (m_iCS_REMAIN_MSEC > DEFAULT_CS_LEFT_ALARM_TICK) {
				// N분 이상 남았을 때만 전송 (N분 이하는 1분씩 따로 보냄)
				GCAnsCsLeftTimeAlarm (
					(m_iCS_REMAIN_MSEC/1000)/3600,			// 시간
					((m_iCS_REMAIN_MSEC/1000)%3600)/60		// 분
					);
				m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();
			}
		}

		// 공성전 남은 시간이 N분 이하일 때는 M분 마다 남은시간 전송
		if (m_iCS_REMAIN_MSEC <= DEFAULT_CS_LEFT_ALARM_TICK) {
			if (GetTickCount() - m_dwCS_LEFTTIME_TICK_COUNT > DEFAULT_CS_LEFT_ALARM_TICK_U10) {
				GCAnsCsLeftTimeAlarm (
					(m_iCS_REMAIN_MSEC/1000)/3600,			// 시간
					((m_iCS_REMAIN_MSEC/1000)%3600)/60		// 분
					);
				m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();
			}
		}

		// 공성전 초반에 사용자들의 공성참여 상태를 갱신해주는 시간 (초반 N번 정도 갱신해줌)
		if (m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT < MAX_CS_JOINSIDE_REFRESH_COUNT) {
			if (GetTickCount() - m_dwCS_JOINSIDE_REFRESH_TICK_COUNT > CS_JOINSIDE_REFRESH_TICK) {
				SetAllUserCsJoinSide ();
				m_dwCS_JOINSIDE_REFRESH_TICK_COUNT	= GetTickCount();
				m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT++;
			}
		}

		// 미니맵 데이터를 계속 만들어서 보내준다.
		if (GetTickCount() - m_dwCS_MINIMAP_SEND_TICK_COUNT > CS_MINIMAP_SEND_TICK) {
			OperateMiniMapWork ();
			m_dwCS_MINIMAP_SEND_TICK_COUNT			= GetTickCount();
		}

		if (GetTickCount() - m_dwEVENT_MSG_TICK_COUNT > CS_MSG_TICK_STARTSIEGE) {
			SendAllUserAnyMsg(lMsg.Get(1619));		// "공성전이 진행 중입니다."
			m_dwEVENT_MSG_TICK_COUNT				= GetTickCount();
		}

		LogAddC(LOGC_GREEN, "RUN ProcState_STARTSIEGE()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		// 공성전이 종료하였다.

		// 1 . 성의 승리길드를 체크한다.
		if (CheckCastleSiegeResult() == TRUE) {
			// 성의 주인이 바뀌었다면 기존의 세금 정보는 모두 리셋한다.
			GS_GDReqResetCastleTaxInfo (m_iMapSvrGroup);
		}

		// 2 . 수성측이 아닌 사용자들을 성 밖으로 옮긴다.
		ReSpawnEnemyUser();

		// 3 . 현재 성의 소유상태를 DB에 저장한다.
		if (m_btIsCastleOccupied == TRUE) {
			GS_GDReqCastleOwnerChange (m_iMapSvrGroup, m_btIsCastleOccupied, m_szCastleOwnerGuild);
		}

		// 4 . 공성전이 종료되었음을 DB에 저장한다.
		GS_GDReqSiegeEndedChange (m_iMapSvrGroup, TRUE);

		// 5 . 현재의 NPC상태를 DB에 저장한다.
		StoreDbNpc ();

		// 6 . 전체 사용자 들에게 공성전이 끝났음을 알림
		NotifyAllUserCsStartState (0);
		
		// 7 . 공성 참여길드 리스트를 초기화 함 (연합 기능을 정상적으로 회복하기 위함)
		GS_GDReqResetSiegeGuildInfo (m_iMapSvrGroup);

		SetState (CASTLESIEGE_STATE_ENDSIEGE);
	}
}


VOID CCastleSiege::ProcState_ENDSIEGE		()
{
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC		-= iTICK_MSEC;
		m_iCS_TICK_COUNT		= GetTickCount();
	
		LogAddC(LOGC_GREEN, "RUN ProcState_ENDSIEGE()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		SetState (CASTLESIEGE_STATE_ENDCYCLE);
	}
}


VOID CCastleSiege::ProcState_ENDCYCLE		()
{
	// 사실 이 부분은 존재하지 않음 -> SetState_ENDCYCLE() 에서 CASTLESIEGE_STATE_IDLE_1 상태로 바로 이동시킴
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC		-= iTICK_MSEC;
		m_iCS_TICK_COUNT		= GetTickCount();
		
		LogAddC(LOGC_GREEN, "RUN ProcState_ENDCYCLE()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		SetState (CASTLESIEGE_STATE_IDLE_1);
	}
}


// DB NPC들을 최초로 세팅 -> DB에 저장
BOOL CCastleSiege::FirstCreateDbNPC			()
{
	if (m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::FirstCreateDbNPC() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4");
		return FALSE;
	}

	CHAR cBUFFER[sizeof(CSP_REQ_NPCSAVEDATA) + sizeof(CSP_NPCSAVEDATA) * (MAX_CS_NPC_COUNT+1)];
	LPCSP_REQ_NPCSAVEDATA lpMsg	= (LPCSP_REQ_NPCSAVEDATA)cBUFFER;
	LPCSP_NPCSAVEDATA lpMsgBody	= (LPCSP_NPCSAVEDATA)(cBUFFER + sizeof(CSP_REQ_NPCSAVEDATA));

	INT iCOUNT	= 0;

	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it		= m_vtNpcData.begin();
	BOOL bExist	= FALSE;

	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData				= ((CS_NPC_DATA)(*it));
		if (pNpcData.m_bNPC_DBSAVE			== TRUE) {
			(*it).SetBaseValue();
			(*it).m_iNPC_LIVE				= NPC_LIVE_READY;
			(*it).m_iNPC_SIDE				= CS_SIEGESIDE_DEFEND;			// DB의 NPC데이터는 무조건 수성

			lpMsgBody[iCOUNT].iNpcNumber	= pNpcData.m_iNPC_NUM;
			lpMsgBody[iCOUNT].iNpcIndex		= pNpcData.m_iNPC_INDEX;
			lpMsgBody[iCOUNT].iNpcDfLevel	= pNpcData.m_iNPC_BASE_DF_LEVEL;
			lpMsgBody[iCOUNT].iNpcRgLevel	= pNpcData.m_iNPC_BASE_RG_LEVEL;
			lpMsgBody[iCOUNT].iNpcMaxHp		= pNpcData.m_iNPC_BASE_MAXHP;
			lpMsgBody[iCOUNT].iNpcHp		= pNpcData.m_iNPC_BASE_HP;
			lpMsgBody[iCOUNT].btNpcX		= pNpcData.m_iNPC_BASE_SX;
			lpMsgBody[iCOUNT].btNpcY		= pNpcData.m_iNPC_BASE_SY;
			lpMsgBody[iCOUNT].btNpcDIR		= pNpcData.m_iNPC_BASE_DIR;

			iCOUNT++;
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	lpMsg->wMapSvrNum						= m_iMapSvrGroup;
	lpMsg->iCount							= iCOUNT;
	lpMsg->h.set((LPBYTE)lpMsg, 0x84, sizeof(CSP_REQ_NPCSAVEDATA) + sizeof(CSP_NPCSAVEDATA) * iCOUNT);
	cDBSMng.Send((char*)lpMsg, sizeof(CSP_REQ_NPCSAVEDATA) + sizeof(CSP_NPCSAVEDATA) * iCOUNT);
	
	return TRUE;
}


BOOL CCastleSiege::SetCastleInitData		(LPCSP_ANS_CSINITDATA lpMsg)
{
	if (m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::SetCastleInitData() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3");
		return FALSE;
	}
	
	if (lpMsg == NULL)
		return FALSE;

	memset(&m_tmStartDate, 0, sizeof(m_tmStartDate));
	memset(&m_tmEndDate, 0, sizeof(m_tmEndDate));
	m_tmStartDate.wYear				= lpMsg->wStartYear;
	m_tmStartDate.wMonth			= lpMsg->btStartMonth;
	m_tmStartDate.wDay				= lpMsg->btStartDay;
	m_tmEndDate.wYear				= lpMsg->wEndYear;
	m_tmEndDate.wMonth				= lpMsg->btEndMonth;
	m_tmEndDate.wDay				= lpMsg->btEndDay;
	m_btIsSiegeGuildList			= lpMsg->btIsSiegeGuildList;
	m_btIsSiegeEnded				= lpMsg->btIsSiegeEnded;
	m_btIsCastleOccupied			= lpMsg->btIsCastleOccupied;
	memset(m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
	memcpy(m_szCastleOwnerGuild, lpMsg->szCastleOwnGuild, 8);
	m_i64CastleMoney				= lpMsg->i64CastleMoney;
	m_iTaxRateChaos					= lpMsg->iTaxRateChaos;
	m_iTaxRateStore					= lpMsg->iTaxRateStore;
	m_iTaxHuntZone					= lpMsg->iTaxHuntZone;

	INT iSTART_DATE_NUM				= MAKELONG(MAKEWORD(m_tmStartDate.wDay, m_tmStartDate.wMonth), m_tmStartDate.wYear);
	INT iEND_DATE_NUM				= MAKELONG(MAKEWORD(m_tmEndDate.wDay, m_tmEndDate.wMonth), m_tmEndDate.wYear);

#ifdef MODIFY_SIEGE_END_DATE_BUG_FIX_20060116
	// iSTATE가 9일경우(공성종료) 스케쥴로부터 종료 되어야 할 날짜를 계산한다. - hnine
	// GetNextDay()를 이용해 종료 스케쥴의 날짜 계산
	SYSTEMTIME EndTime;
	m_tmSiegeEndSchedule = m_tmStartDate;

	GetStateDate( CASTLESIEGE_STATE_ENDCYCLE, &EndTime );
	LogAddTD( "[CastleSiege] CCastleSiege::LoadData() - Siege Schedule Start Date (%d-%d-%d)", m_tmStartDate.wYear, m_tmStartDate.wMonth, m_tmStartDate.wDay );
	LogAddTD( "[CastleSiege] CCastleSiege::LoadData() - Siege Schedule Date (%d-%d-%d)", EndTime.wDay, EndTime.wHour, EndTime.wMinute );
	GetNextDay( m_tmSiegeEndSchedule, EndTime.wDay, EndTime.wHour, EndTime.wMinute, 0 );
	LogAddTD( "[CastleSiege] CCastleSiege::LoadData() - Siege Schedule End Date (%d-%d-%d(%d:%d:%d)", m_tmSiegeEndSchedule.wYear, m_tmSiegeEndSchedule.wMonth, m_tmSiegeEndSchedule.wDay, m_tmSiegeEndSchedule.wHour, m_tmSiegeEndSchedule.wMinute, m_tmSiegeEndSchedule.wSecond );
#endif

	if (iSTART_DATE_NUM > iEND_DATE_NUM) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::SetCastleInitData() - iSTART_DATE_NUM > iEND_DATE_NUM");
		return FALSE;
	}

	return TRUE;
}


BOOL CCastleSiege::SetCastleNpcData			(LPCSP_CSINITDATA lpMsg, INT iCOUNT)
{
	if (m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::SetCastleNpcData() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3");
		return FALSE;
	}

	if (lpMsg == NULL)
		return FALSE;
	
	if (iCOUNT < 0)
		return FALSE;

	vector <CS_NPC_DATA>::iterator it;

	for (INT iNPC_COUNT = 0 ; iNPC_COUNT < iCOUNT ; iNPC_COUNT++) {
		EnterCriticalSection (&m_critNpcData);
		it	= m_vtNpcData.begin();
		BOOL bExist	= FALSE;

		while (it != m_vtNpcData.end()) {
			CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
			if ((pNpcData.m_iNPC_NUM == lpMsg[iNPC_COUNT].iNpcNumber) &&
				(pNpcData.m_iNPC_INDEX == lpMsg[iNPC_COUNT].iNpcIndex)
				)
			{
				// 벡터안에 존재하는 NPC라면 살아있는 상태로 바꾸어주고 데이터도 DB에 맞게 수정한다.
				(*it).m_iNPC_LIVE			= NPC_LIVE_READY;
				(*it).m_iNPC_DF_LEVEL		= lpMsg[iNPC_COUNT].iNpcDfLevel;
				(*it).m_iNPC_RG_LEVEL		= lpMsg[iNPC_COUNT].iNpcRgLevel;
				(*it).m_iNPC_MAXHP			= lpMsg[iNPC_COUNT].iNpcMaxHp;
				(*it).m_iNPC_HP				= lpMsg[iNPC_COUNT].iNpcHp;
				(*it).m_iNPC_SX				= lpMsg[iNPC_COUNT].btNpcX;
				(*it).m_iNPC_SY				= lpMsg[iNPC_COUNT].btNpcY;
				(*it).m_iNPC_DX				= -1;
				(*it).m_iNPC_DY				= -1;
				(*it).m_iNPC_DIR			= lpMsg[iNPC_COUNT].btNpcDIR;
				(*it).m_iNPC_SIDE			= CS_SIEGESIDE_DEFEND;			// DB의 NPC데이터는 무조건 수성
				bExist						= TRUE;
				break;				
			}
			
			it++;
		}
		LeaveCriticalSection (&m_critNpcData);

		if (bExist == FALSE) {
			// 여기까지 왔다면 NPC 관리 벡터 안에는 없는것이다. -> 새로 추가한다.
			CS_NPC_DATA pNpcData2;
			pNpcData2.m_bIN_USE				= TRUE;
			pNpcData2.m_iNPC_NUM			= lpMsg[iNPC_COUNT].iNpcNumber;
			pNpcData2.m_iNPC_INDEX			= lpMsg[iNPC_COUNT].iNpcIndex;
			pNpcData2.m_iNPC_LIVE			= NPC_LIVE_READY;
			pNpcData2.m_iNPC_SIDE			= CS_SIEGESIDE_DEFEND;			// DB의 NPC데이터는 무조건 수성

			pNpcData2.m_iNPC_DF_LEVEL		= pNpcData2.m_iNPC_BASE_DF_LEVEL	= lpMsg[iNPC_COUNT].iNpcDfLevel;
			pNpcData2.m_iNPC_RG_LEVEL		= pNpcData2.m_iNPC_BASE_RG_LEVEL	= lpMsg[iNPC_COUNT].iNpcRgLevel;
			pNpcData2.m_iNPC_MAXHP			= pNpcData2.m_iNPC_BASE_MAXHP		= lpMsg[iNPC_COUNT].iNpcMaxHp;
			pNpcData2.m_iNPC_HP				= pNpcData2.m_iNPC_BASE_HP			= lpMsg[iNPC_COUNT].iNpcHp;
			pNpcData2.m_iNPC_SX				= pNpcData2.m_iNPC_BASE_SX			= lpMsg[iNPC_COUNT].btNpcX;
			pNpcData2.m_iNPC_SY				= pNpcData2.m_iNPC_BASE_SY			= lpMsg[iNPC_COUNT].btNpcY;
			pNpcData2.m_iNPC_DX				= pNpcData2.m_iNPC_BASE_DX			= -1;
			pNpcData2.m_iNPC_DY				= pNpcData2.m_iNPC_BASE_DY			= -1;
			pNpcData2.m_iNPC_DIR			= pNpcData2.m_iNPC_BASE_DIR			= lpMsg[iNPC_COUNT].btNpcDIR;
			EnterCriticalSection (&m_critNpcData);
			m_vtNpcData.push_back(pNpcData2);
			LeaveCriticalSection (&m_critNpcData);
		}
	}
	
	return TRUE;
}


BOOL CCastleSiege::CheckSync				()								// 파일 데이터의 시간과 현재 시간을 바탕으로 성의 시간 싱크를 맞춘다. (예외 상황 포함)
{
	if (!m_bFileDataLoadOK) {
		// 여기까지의 싱크 점검은 파일 데이터만 읽혀있어도 가능하다.
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::CheckSync() - m_bFileDataLoadOK == FALSE");
		return FALSE;
	}

	m_iCS_TICK_COUNT						= GetTickCount();

	SYSTEMTIME tmToDay;
	GetLocalTime(&tmToDay);
	INT iTODAY_DATE_NUM						= MAKELONG(MAKEWORD(tmToDay.wDay, tmToDay.wMonth), tmToDay.wYear);

	INT iCastleInitState					= CASTLESIEGE_STATE_IDLE_1;

	// 1 . 공성주기 시작 시간이 강제 입력 되었는가 ?
	if (m_bFixCastleCycleStartDate) {
		// 공성시작 시간은 입력 때 부터 항상 그 주기 범위안에 현재 시간을 포함하거나 다가올 주기임을 보장한다.
		m_tmStartDate.wYear					= m_tmFixCastleCycleStartDate.wYear;
		m_tmStartDate.wMonth				= m_tmFixCastleCycleStartDate.wMonth;
		m_tmStartDate.wDay					= m_tmFixCastleCycleStartDate.wDay;

		m_tmEndDate							= m_tmStartDate;
		GetNextDay(m_tmEndDate, m_iCastleSiegeCycle);
	}

	// 2 . 현재 시간이 공성주기 종료 시간보다 늦는지 확인 -> 늦으면 문제있음 (공성주기 시작 시간이 강제로 입력되지 않고 DB서 받은 경우 여기서 걸러짐)
	INT iEVENT_END_DATE_NUM					= MAKELONG(MAKEWORD(m_tmEndDate.wDay, m_tmEndDate.wMonth), m_tmEndDate.wYear);
	if (iEVENT_END_DATE_NUM <= iTODAY_DATE_NUM) {
		m_bFixCastleCycleStartDate			= FALSE;
		MsgBox("[CastleSiege] CCastleSiege::CheckSync() - iEVENT_END_DATE_NUM (%04d-%02d-%02d) <= iTODAY_DATE_NUM (%04d-%02d-%02d)",
			m_tmEndDate.wYear,
			m_tmEndDate.wMonth,
			m_tmEndDate.wDay,
			tmToDay.wYear,
			tmToDay.wMonth,
			tmToDay.wDay
			);
		return FALSE;
	}

	// *> . 위의 체크에서 무사했다면 공성주기가 변경되었을 경우 바로 DB에 저장한다.
	if (m_bFixCastleCycleStartDate) {
		GS_GDReqSiegeDateChange (
			m_iMapSvrGroup,				
			-1,							// 인덱스는 필요없음
			m_tmStartDate.wYear,		
			m_tmStartDate.wMonth,		
			m_tmStartDate.wDay, 
			m_tmEndDate.wYear, 
			m_tmEndDate.wMonth, 
			m_tmEndDate.wDay
			);
	}

	// 3 . 공성 특정상태 시작 시간이 강제 입력 되었는가 ?
	if (m_bFixCastleStateStartDate) {
		// 공성 특정상태 시간은 입력 때 부터 항상 현재 시간보다 나중인 것을 보장한다.
		// 공성 특정상태는 입력 때 부터 항상 정규 상태범위 안에 있는것을 보장한다.

		// 1> . 특정상태가 존재하는지를 체크
		BOOL bSpecificStateExist				= FALSE;
		EnterCriticalSection (&m_critScheduleData);
		vector <CS_SCHEDULE_DATA>::iterator it	= m_vtScheduleData.begin();
		while (it != m_vtScheduleData.end()) {
			CS_SCHEDULE_DATA & pScheduleData	= (*it);

			if (pScheduleData.m_iSTATE == m_iFixCastleSpecificState) {
				bSpecificStateExist				= TRUE;
				break;
			}

			it++;
		}
		LeaveCriticalSection (&m_critScheduleData);

		// 임시 !!! -> 공성 종료시간이 공성주기 종료시간보다 늦으면 에러 ???

		if (bSpecificStateExist) {
			time_t	ttSTime;
			time_t	ttETime;
			tm		tmETime;

			memset (&tmETime, 0, sizeof(tmETime));
			tmETime.tm_year					= m_tmFixCastleStateStartDate.wYear		- 1900;
			tmETime.tm_mon					= m_tmFixCastleStateStartDate.wMonth	- 1;
			tmETime.tm_mday					= m_tmFixCastleStateStartDate.wDay;
			tmETime.tm_hour					= m_tmFixCastleStateStartDate.wHour;
			tmETime.tm_min					= m_tmFixCastleStateStartDate.wMinute;

			time(&ttSTime);
			ttETime							= mktime(&tmETime);
		
			double	dResultSecond			= difftime(ttETime, ttSTime);
			m_iCS_REMAIN_MSEC				= dResultSecond * 1000;

			// 현재시간부터 특정상태 시작 전까지의 모든 시간은 그 전 상태이다.
			m_iCastleSiegeState				= m_iFixCastleSpecificState	- 1;
			SetState(m_iCastleSiegeState, FALSE);
			return TRUE;
		}
		else {
			// 상태가 없다면 스케쥴 데이터 (파일) 의 상태들이 정규상태와 않맞거나 잘못되었을 수 있다.
			MsgBox("[CastleSiege] CCastleSiege::CheckSync() - FIXED STATE:%d NOT FOUND IN SCHEDULE (P.S.> Check File 'MuCastleData.dat', 'commonserver.cfg')",
				m_iFixCastleSpecificState
				);
//			return FALSE;
		}

		// 여기까지 왔다면 실패한 것이다. -> 그냥 다음상태 체크로 넘어간다.
		m_bFixCastleStateStartDate			= FALSE;
	}

	// 4 . 공성전이 이미 치루어졌는가 ? -> 끝났다면 무조건 CASTLESIEGE_STATE_ENDSIEGE (공성종료) 상태로 간다.
	if (m_btIsSiegeEnded) {
		SetState(CASTLESIEGE_STATE_ENDSIEGE);
		return TRUE;
	}

	// 5 . 공성 스케쥴을 계산한다. -> 현재 시간이 공성 상태의 어느부분에 속하는지 찾아낸다.
	INT iEVENT_START_DATE_NUM				= MAKELONG(MAKEWORD(m_tmStartDate.wDay, m_tmStartDate.wMonth), m_tmStartDate.wYear);

	if (iEVENT_START_DATE_NUM > iTODAY_DATE_NUM) {
		// 공성주기 시작 전이다.
		SetState(CASTLESIEGE_STATE_IDLE_1);		// 이 상태는 시작 시 알아서 남은 시간을 계산한다. (나머지 정규상태들은 계산후 강제적용 해주어야 함)
		return TRUE;
	}
	else {
		// 공성주기 중간에 있다.
		BOOL bStateSetted						= FALSE;
		CS_SCHEDULE_DATA pScheData_FR;
		CS_SCHEDULE_DATA pScheData_RR;
		EnterCriticalSection (&m_critScheduleData);
		vector <CS_SCHEDULE_DATA>::iterator it	= m_vtScheduleData.begin();
		while (it != m_vtScheduleData.end()) {
			CS_SCHEDULE_DATA & pScheduleData	= (*it);

			SYSTEMTIME tmSchduleDate			= m_tmStartDate;
			GetNextDay(tmSchduleDate, pScheduleData.m_iADD_DAY);
			tmSchduleDate.wHour					= pScheduleData.m_iADD_HOUR;
			tmSchduleDate.wMinute				= pScheduleData.m_iADD_MIN;

			INT64 i64ScheduleDateNum			= ((INT64) ((INT64)MAKELONG(tmSchduleDate.wMinute, tmSchduleDate.wHour) | (INT64)((INT64)MAKELONG(MAKEWORD(tmSchduleDate.wDay, tmSchduleDate.wMonth), tmSchduleDate.wYear) << 32)));
			INT64 i64ToDayDateNum				= ((INT64) ((INT64)MAKELONG(tmToDay.wMinute, tmToDay.wHour) | (INT64)((INT64)MAKELONG(MAKEWORD(tmToDay.wDay, tmToDay.wMonth), tmToDay.wYear) << 32)));

			if (i64ScheduleDateNum > i64ToDayDateNum) {
				bStateSetted					= TRUE;
				pScheData_RR					= pScheduleData;
				m_iCastleSiegeState				= iCastleInitState;
				break;
			}
			iCastleInitState					= pScheduleData.m_iSTATE;
			pScheData_FR						= pScheduleData;

			it++;
		}
		LeaveCriticalSection (&m_critScheduleData);

		if (bStateSetted == FALSE) {
			// 2번을 체크하였다면 MuCastleData.dat 파일이 잘못되기 전에는 여기까지 올 일은 아마도 없을 것이다.
			MsgBox("[CastleSiege] CCastleSiege::CheckSync() - bStateSetted == FALSE  START_DATE (%04d-%02d-%02d), END_DATE (%04d-%02d-%02d)",
				m_tmStartDate.wYear,
				m_tmStartDate.wMonth,
				m_tmStartDate.wDay,
				m_tmEndDate.wYear,
				m_tmEndDate.wMonth,
				m_tmEndDate.wDay
				);
			return FALSE;
		}

		// 남은 시간을 계산하여 강제 입력하고 스케쥴을 시작한다.
		// 1> . 다음 상태의 정확한 날짜를 계산
		SYSTEMTIME tmSchduleDate			= m_tmStartDate;
		GetNextDay(tmSchduleDate, pScheData_RR.m_iADD_DAY);
		tmSchduleDate.wHour					= pScheData_RR.m_iADD_HOUR;
		tmSchduleDate.wMinute				= pScheData_RR.m_iADD_MIN;
//		GetNextDay(tmSchduleDate, pScheData_RR.m_iADD_DAY, pScheData_RR.m_iADD_HOUR, pScheData_RR.m_iADD_MIN);
		
		// 2> . 오늘 날짜와의 차이를 구해서 MSEC값으로 만듦
		time_t	ttSTime;
		time_t	ttETime;
		tm		tmETime;

		memset (&tmETime, 0, sizeof(tmETime));
		tmETime.tm_year						= tmSchduleDate.wYear	- 1900;
		tmETime.tm_mon						= tmSchduleDate.wMonth	- 1;
		tmETime.tm_mday						= tmSchduleDate.wDay;
		tmETime.tm_hour						= tmSchduleDate.wHour;
		tmETime.tm_min						= tmSchduleDate.wMinute;

		time(&ttSTime);
		ttETime								= mktime(&tmETime);
	
		double	dResultSecond				= difftime(ttETime, ttSTime);
		m_iCS_REMAIN_MSEC					= dResultSecond * 1000;


		// 임시 !!! -> 나중에 포함시켜야 함  <*>
		// 특수한 상황에 대한 예외처리를 한다.
		if (m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE) {
			// 만약 현재 상태가 공성시작 이라면 강제로 공성준비 30분 전으로 맞추어야 한다.
			m_iCastleSiegeState		= CASTLESIEGE_STATE_READYSIEGE;
			m_iCS_REMAIN_MSEC		= CS_MIN_SIEGESTART * 60 * 1000;
		}
		else if (m_iCastleSiegeState == CASTLESIEGE_STATE_READYSIEGE) {
			if (m_iCS_REMAIN_MSEC < CS_MIN_SIEGESTART * 60 * 1000) {
				// 만약 현재 상태가 공성준비 30분 이내라면 강제로 공성준비 30분 전으로 맞추어야 한다.
				m_iCastleSiegeState		= CASTLESIEGE_STATE_READYSIEGE;
				m_iCS_REMAIN_MSEC		= CS_MIN_SIEGESTART * 60 * 1000;
			}
		}
		
		// 최종적으로 상태를 세팅한다.
#ifdef CASTLE_TEST_SCHEDULER_20041218
		SetState(m_iCastleSiegeState);
#else
		SetState(m_iCastleSiegeState, FALSE);
#endif
	}

	return TRUE;
}


VOID CCastleSiege::GetNextDay				(SYSTEMTIME &st, INT iAfterDay, INT iAfterHour, INT iAfterMin, INT iAfterSec)
{
	INT64 i64;
	FILETIME fst;

//	st.wHour = st.wMinute = st.wSecond = st.wMilliseconds = st.wDayOfWeek = 0;
	st.wSecond = st.wMilliseconds = st.wDayOfWeek = 0;

	SystemTimeToFileTime(&st,&fst);
	i64=(((INT64)fst.dwHighDateTime) << 32) + fst.dwLowDateTime;
	i64 = i64 + (INT64)864000000000*(INT64)iAfterDay + (INT64)36000000000*(INT64)iAfterHour + (INT64)600000000*(INT64)iAfterMin + (INT64)10000000*(INT64)iAfterSec;
	fst.dwHighDateTime = (DWORD)(i64 >> 32);
	fst.dwLowDateTime = (DWORD)(i64 & 0xffffffff);
	FileTimeToSystemTime(&fst, &st);
	return;
}

#ifdef MODIFY_SIEGE_END_DATE_BUG_FIX_20060116
VOID CCastleSiege::GetStateDate				(INT iCastleSiegeState, SYSTEMTIME* st)
{
	EnterCriticalSection (&m_critScheduleData);
	vector <CS_SCHEDULE_DATA>::iterator it		= m_vtScheduleData.begin();
	for (; it != m_vtScheduleData.end() ; it++) {
		const CS_SCHEDULE_DATA & pScheduleData	= (*it);

		if (pScheduleData.m_iSTATE == iCastleSiegeState) {
			st->wDay							= pScheduleData.m_iADD_DAY;
			st->wHour							= pScheduleData.m_iADD_HOUR;
			st->wMinute							= pScheduleData.m_iADD_MIN;
			st->wSecond							= 0;
			break;
		}
	}
	LeaveCriticalSection (&m_critScheduleData);
}
#endif

INT CCastleSiege::GetStateGapSec			(INT iCastleSiegeState)			// 특정 상태의 다음 상태까지의 시간간격 (SEC) 을 구한다.
{
	INT iGAP_SEC								= -1;
#ifdef CASTLE_TEST_SCHEDULER_20041218			// 테스트 스케쥴러 적용 시
	iGAP_SEC									= CS_TEST_GAPSEC;
#else
	EnterCriticalSection (&m_critScheduleData);
	vector <CS_SCHEDULE_DATA>::iterator it		= m_vtScheduleData.begin();
	for (; it != m_vtScheduleData.end() ; it++) {
		const CS_SCHEDULE_DATA & pScheduleData	= (*it);

		if (pScheduleData.m_iSTATE == iCastleSiegeState) {
			iGAP_SEC							= pScheduleData.m_iGAP_SEC;
			break;
		}
	}
	LeaveCriticalSection (&m_critScheduleData);
#endif
	return iGAP_SEC;
}


INT CCastleSiege::CalcCastleLeftSiegeDate	()
{
	memset (&m_tmLeftCastleSiegeDate, 0, sizeof(m_tmLeftCastleSiegeDate));

	if ((m_iCastleSiegeState < CASTLESIEGE_STATE_IDLE_1) || (m_iCastleSiegeState > CASTLESIEGE_STATE_ENDCYCLE)) {
		// 공성전이 현재 동작하지 않는다.
		return CASTLESIEGE_LEFTTIME_NOTRUNNING;
	}

	if (m_bDoRun == FALSE) {
		// 공성전이 현재 동작하지 않는다.
		return CASTLESIEGE_LEFTTIME_NOTRUNNING;
	}

	if (m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE) {
		// 공성전이 진행 중이다.
		return CASTLESIEGE_LEFTTIME_ONSIGE;
	}

	if (m_btIsSiegeEnded == TRUE || m_iCastleSiegeState >= CASTLESIEGE_STATE_ENDSIEGE) {
		// 공성전이 이미 종료했다.
		return CASTLESIEGE_LEFTTIME_ENDSIEGE;
	}

	// 여기서 실제적인 공성전까지의 시간을 계산하게 된다.
	INT iGAP_SEC								= -1;
	BOOL bAddStart								= FALSE;
	EnterCriticalSection (&m_critScheduleData);
	vector <CS_SCHEDULE_DATA>::iterator it		= m_vtScheduleData.begin();
	for (; it != m_vtScheduleData.end() ; it++) {
		const CS_SCHEDULE_DATA & pScheduleData	= (*it);
		if (pScheduleData.m_iSTATE == CASTLESIEGE_STATE_STARTSIEGE)
			break;

		if (bAddStart) {
			iGAP_SEC							+= pScheduleData.m_iGAP_SEC;
		}

		if (pScheduleData.m_iSTATE == m_iCastleSiegeState) {
			iGAP_SEC							= m_iCS_REMAIN_MSEC / 1000;
			bAddStart							= TRUE;
		}
	}
	LeaveCriticalSection (&m_critScheduleData);

	if (iGAP_SEC >= 0) {
		// 마지막으로 공성전이 언제 오픈 예정인지 계산한다.
		SYSTEMTIME tmNowDate;
		GetLocalTime(&tmNowDate);
		GetNextDay(tmNowDate, 0, 0, 0, iGAP_SEC);
		m_tmLeftCastleSiegeDate					= tmNowDate;
		if (!iGAP_SEC)
			LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CalcCastleLeftSiegeDate() - iGAP_SEC == 0");
		return iGAP_SEC;
	}
	
	return CASTLESIEGE_LEFTTIME_NOTRUNNING;
}


BOOL CCastleSiege::GetCastleStateTerm		(SYSTEMTIME & tmStateStartDate, SYSTEMTIME & tmStateEndDate)
{
	if ((m_iCastleSiegeState < CASTLESIEGE_STATE_IDLE_1) || (m_iCastleSiegeState > CASTLESIEGE_STATE_ENDCYCLE)) {
		// 공성전이 현재 동작하지 않는다.
		return FALSE;
	}

	if (m_bDoRun == FALSE) {
		// 공성전이 현재 동작하지 않는다.
		return FALSE;
	}

	// 상태 시작시간은 이전에 저장해 놓았던 값을 사용한다.
	tmStateStartDate	= m_tmStateStartDate;		
	
	// 상태 종료시간은 현재 시간에서 남은 시간 (m_iCS_REMAIN_MSEC) 을 더해서 계산한다.
	GetLocalTime (&tmStateEndDate);
	GetNextDay (tmStateEndDate, 0, 0, 0, m_iCS_REMAIN_MSEC / 1000);
	
	return TRUE;
}


INT CCastleSiege::GetCurRemainSec			()
{	
	if (m_iCS_REMAIN_MSEC < 0)
		return -1;

	return m_iCS_REMAIN_MSEC / 1000;	
}


VOID CCastleSiege::ClearDbNPC()
{
	for( int n=0; n<MAX_MONSTER; n++)
	{
		if (gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE) {					// 공성맵에 있는 NPC들에 한정
			if (
				gObj[n].Class == 277 ||			// 성문
				gObj[n].Class == 283 ||			// 수호석상
				gObj[n].Class == 219			// 성문레버 (DB NPC는 아니지만 성문과 같이 없애줌)
				)
			{
				if (gObj[n].Class == 277) {
					// 성문 제거시에는 성문자리의 속성을 없애준다.
					SetGateBlockState (gObj[n].X, gObj[n].Y, CS_GATE_STATE_OPENED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[n], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0 ,0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
				}

				gObjDel(n);
			}
		}
	}
}


VOID CCastleSiege::ClearNonDbNPC()
{
	for( int n=0; n<MAX_MONSTER; n++)
	{
		if (gObjIsConnected(n))
		{
			if ((gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE) &&		// 공성맵에 있는 NPC들에 한정
				(gObj[n].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
				(gObj[n].m_btCsNpcType != CS_NPC_TYPE_DB_DFN)
				) 
			{
				
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
				if (gObj[n].Class == NPC_INDEX_LIFESTONE) 
				{	
					// 라이프스톤 이라면 길드 관계를 없애준다.
					g_CsNPC_LifeStone.DeleteLifeStone(n);
				}
#endif

#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214
				if( gObj[n].Class == NPC_INDEX_SPEARMAN
					|| gObj[n].Class == NPC_INDEX_BOWMAN )
				{
					// 용병 숫자를 세기 위해 Delete 함수를 콜한다.
					g_CsNPC_Mercenary.DeleteMercenary(n);
				}
#endif
				
				if (gObj[n].Class == 219) {							// 성문 레버는 제거하면 않된다.
					continue;
				}
				
				gObjDel(n);

				// 임시 !!!
/*
				if (
//					gObj[n].Class == 224 ||			// 경비병
//					gObj[n].Class == 283 ||			// 왕관
					gObj[n].Class == 104 ||			// 트랩
					gObj[n].Class == 288 ||			// 가드타워
					gObj[n].Class == 221 ||			// 투석기 (공성측)
					gObj[n].Class == 222			// 투석기 (수성측)
					)
				{
					gObjDel(n);
				}
*/
			}
		}
	}
}


VOID CCastleSiege::ClearNonDbNPC_MidWin()
{
	for( int n=0; n<MAX_MONSTER; n++)
	{
		if (gObjIsConnected(n))
		{
			if ((gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE) &&		// 공성맵에 있는 NPC들에 한정
				(gObj[n].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
				(gObj[n].m_btCsNpcType != CS_NPC_TYPE_DB_DFN)
				) 
			{
				
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
				if (gObj[n].Class == NPC_INDEX_LIFESTONE) {			// 라이프스톤 이라면 길드 관계를 없애준다.
					g_CsNPC_LifeStone.DeleteLifeStone(n);
				}
#endif
			
				if (gObj[n].Class == NPC_INDEX_LIFESTONE			// 라이프스톤
					)
				{
					gObjDel(n);
				}
			}
		}
	}
}


VOID CCastleSiege::ClearAllNPC()
{
	for( int n=0; n<MAX_MONSTER; n++)
	{
		if (gObj[n].m_btCsNpcType != CS_NPC_TYPE_NONE) {					// 공성 몬스터라는 타입이 지정된 것들만 비교
/*
			if (
//				gObj[n].Class == 224 ||			// 경비병
//				gObj[n].Class == 283 ||			// 왕관
//				gObj[n].Class == 104 ||			// 트랩
				gObj[n].Class == 277 ||			// 성문
				gObj[n].Class == 283 ||			// 수호석상
				gObj[n].Class == 288 ||			// 가드타워
				gObj[n].Class == 221 ||			// 투석기 (공성측)
				gObj[n].Class == 222			// 투석기 (수성측)
				)
			{
				gObjDel(n);
			}
*/
			// 임시 !!!
			gObjDel(n);
		}
	}
}


VOID CCastleSiege::CreateDbNPC				()								// DB에 저장하는 NPC들을 모두 생성
{
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	BOOL bExist	= FALSE;

	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_bIN_USE	== TRUE) &&
			(pNpcData.m_iNPC_LIVE == NPC_LIVE_READY)
			)
		{
			// 벡터안에 존재하는 NPC라면 살아있는 상태로 바꾸어주고 데이터도 DB에 맞게 수정한다.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;				// 어차피 리젠되거나 고정 정보를 가진 NPC들이 아니다.
				gObj[iNPC_INDEX].X					= pNpcData.m_iNPC_SX;
				gObj[iNPC_INDEX].Y					= pNpcData.m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber			= MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX					= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY					= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX				= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY				= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir				= pNpcData.m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX			    = (BYTE)gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY				= (BYTE)gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen			= 0;
				gObj[iNPC_INDEX].MaxRegenTime		= 0;
				gObj[iNPC_INDEX].Life				= pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife			= pNpcData.m_iNPC_MAXHP;
				gObj[iNPC_INDEX].m_btCsNpcType		= CS_NPC_TYPE_DB_DFN;		// 수성측 DB 몬스터

				INT iNPC_DF_LEVEL					= pNpcData.m_iNPC_DF_LEVEL;	// 방어력 레벨
				if (iNPC_DF_LEVEL < 0)						
					iNPC_DF_LEVEL					= 0;
				if (iNPC_DF_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_DF_LEVEL					= MAX_UPGRADE_LEVEL;
				INT iNPC_RG_LEVEL					= pNpcData.m_iNPC_RG_LEVEL;	// 회복력 레벨
				if (iNPC_RG_LEVEL < 0)						
					iNPC_RG_LEVEL					= 0;
				if (iNPC_RG_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_RG_LEVEL					= MAX_UPGRADE_LEVEL;
				
				// 회복력, 방어력 등의 수치가 필요하다면 여기서 세팅해준다. -> 외부에서 (AdjustDbNpcLevel() - 공성 시작전)
				switch(pNpcData.m_iNPC_NUM) {
				case 277 :	// 성문
					gObj[iNPC_INDEX].m_Defense					= g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_MagicDefense				= g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel			= iNPC_DF_LEVEL;
					break;
				case 283 :	// 석상
					gObj[iNPC_INDEX].m_Defense					= g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_MagicDefense				= g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel			= iNPC_DF_LEVEL;
					gObj[iNPC_INDEX].m_btCsNpcRgLevel			= iNPC_RG_LEVEL;
					break;
				}

				// 나중에 관리 확인을 하기위해 여기에 인증값을 넣는다.
				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;

				if (pNpcData.m_iNPC_NUM == 277) {
					// 임시 !!! -> 나중에 제거해야 함
					// 성문 생성 직후에는 성문을 열은 것으로 한다.
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_OPENED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[iNPC_INDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObj[iNPC_INDEX].m_btCsGateOpen	= CS_GATE_STATE_OPENED;
					// 임시 !!! -> 나중에 추가해야 함
					// 성문 생성 직후에는 성문을 닫은 것으로 한다.
//					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_CLOSED);
//					gObj[iNPC_INDEX].m_btCsGateOpen	= CS_GATE_STATE_CLOSED;

					// 성문의 레버를 여기서 연결한다.
					BOOL bCreateLever		= FALSE;
					if (pNpcData.m_iCS_GATE_LEVER_INDEX == -1) {
						// 만들어진 레버가 없다.
						bCreateLever		= TRUE;
					}
					else {
						// 만들어진 레버가 있다.
						if (CheckLeverAlive(pNpcData.m_iCS_GATE_LEVER_INDEX) == FALSE) {
							bCreateLever	= TRUE;
						}
					}

					if (bCreateLever) {
						// 레버를 새로 만든다.
						INT iLeverIndex	= CreateCsGateLever(pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY + 4);
						if (CHECK_LIMIT(iLeverIndex, MAX_OBJECT)) {
							// 레버 생성 성공
							(*it).m_iCS_GATE_LEVER_INDEX	= iLeverIndex;							
							// 만들어진 레버에 인덱스를 연결한다.
							BOOL bRETVAL	= LinkCsGateLever(iLeverIndex, iNPC_INDEX);
							if (bRETVAL == FALSE) {
								LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
									pNpcData.m_iNPC_INDEX
									);
							}
						}
						else {
							// 레버 생성 실패
							(*it).m_iCS_GATE_LEVER_INDEX	= -1;
							LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								pNpcData.m_iNPC_INDEX
								);
						}
					}
					else {
						// 만들어진 레버에 인덱스를 연결한다.
						BOOL bRETVAL	= LinkCsGateLever(pNpcData.m_iCS_GATE_LEVER_INDEX, iNPC_INDEX);
						if (bRETVAL == FALSE) {
							LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								pNpcData.m_iNPC_INDEX
								);
						}
					}
				}

				// 최종적으로 이 NPC는 생성되었음을 알린다.
				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


VOID CCastleSiege::CreateNonDbNPC					(BOOL bDelFirst)		// DB에 저장하지 않는 NPC들을 모두 생성 (생성 전에 임의로 한번 지울지 결정)
{
	if (bDelFirst) {
		ClearNonDbNPC();
	}
	
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_bIN_USE	== TRUE) &&
			(pNpcData.m_bNPC_DBSAVE == FALSE)			
			)
		{
			// 벡터안에 존재하는 NPC라면 살아있는 상태로 바꾸어주고 데이터도 DB에 맞게 수정한다.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;						// 어차피 리젠되거나 고정 정보를 가진 NPC들이 아니다.
				gObj[iNPC_INDEX].X					= pNpcData.m_iNPC_SX;
				gObj[iNPC_INDEX].Y					= pNpcData.m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber			= MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX					= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY					= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX				= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY				= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir				= pNpcData.m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX			    = (BYTE)gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY				= (BYTE)gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen			= 0;
				gObj[iNPC_INDEX].MaxRegenTime		= 0;
				gObj[iNPC_INDEX].Life				= pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife			= pNpcData.m_iNPC_MAXHP;
				switch(pNpcData.m_iNPC_SIDE) {
				case CS_SIEGESIDE_DEFEND :
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_DFN;
					break;
				case CS_SIEGESIDE_ATTACK :
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_ATK;
					break;
				default :
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_ATK;
					break;
				}	

				// 나중에 관리 확인을 하기위해 여기에 인증값을 넣는다.
				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;

				// 최종적으로 이 NPC는 생성되었음을 알린다.
				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


BOOL CCastleSiege::CheckAddDbNPC					(						// DB에 저장하는 NPC중 하나를 추가 가능한지 체크 -> 생성 후 DB저장 함 (공성전 중이 아닐때 만)
	INT iIndex,
	INT iNpcType, 
	INT iNpcIndex,
	BYTE &btResult
	)
{
	// 현재 NPC 관리 목록에서 아래의 상태들을 일단 확인
	INT iRETVAL		= FALSE;

	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == iNpcType) &&
			(pNpcData.m_iNPC_INDEX == iNpcIndex)
			)
		{
			if ((pNpcData.m_bIN_USE	== TRUE) &&
				(pNpcData.m_iNPC_LIVE > NPC_LIVE_NONE) &&
				(gObjIsConnected(pNpcData.m_iNPC_OBJINDEX))
				)
			{
				if ((gObj[pNpcData.m_iNPC_OBJINDEX].Class == iNpcType) &&
					(gObj[pNpcData.m_iNPC_OBJINDEX].Live == 1) &&
					(gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
					(gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal != 0)
					)
				{
					// 현재 살아있으므로 다시 추가할 수 없다.
					btResult	= 0;
					iRETVAL		= FALSE;
					break;
				}
			}

			// 현재 죽어있으므로 DB서버에 추가할 수 있는지 여부를 질의한다.
			
			// 구입비용을 계산한다.
			INT iBuyCost	= 0;
			switch(pNpcData.m_iNPC_NUM) {
			case 277 :	// 성문
				iBuyCost	= pNpcData.m_iNPC_BASE_MAXHP * 5;
				break;
			case 283 :	// 수호석상
				iBuyCost	= pNpcData.m_iNPC_BASE_MAXHP * 3;
				break;
			}

			// 구입비용이 있는지 확인한다.
			if (gObj[iIndex].Money < iBuyCost) {
				btResult	= 3;
				iRETVAL		= FALSE;
				break;
			}

			GS_GDReqCastleNpcBuy (m_iMapSvrGroup, iIndex, iNpcType, iNpcIndex, pNpcData.m_iNPC_DF_LEVEL, pNpcData.m_iNPC_RG_LEVEL, pNpcData.m_iNPC_MAXHP, pNpcData.m_iNPC_HP, pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, pNpcData.m_iNPC_DIR, iBuyCost);
			btResult		= 1;
			iRETVAL			= TRUE;
			break;
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
	
	return iRETVAL;
}


BOOL CCastleSiege::AddDbNPC					(								// DB에 저장하는 NPC중 하나를 추가 -> 공성 중에는 생기면 않됨
	INT iNpcType, 
	INT iNpcIndex
	)
{
	// 현재 NPC 관리 목록에서 아래의 상태들을 일단 확인
	INT iRETVAL		= FALSE;

	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == iNpcType) &&
			(pNpcData.m_iNPC_INDEX == iNpcIndex)
			)
		{
			if (gObjIsConnected(pNpcData.m_iNPC_OBJINDEX)) {
				if ((gObj[pNpcData.m_iNPC_OBJINDEX].Class == iNpcType) &&
					(gObj[pNpcData.m_iNPC_OBJINDEX].Live == 1) &&
					(gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
					(gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal != 0)
					)
				{
					// 현재 살아있으므로 다시 추가할 수 없다. -> 오류
					LogAddTD("[CastleSiege] CCastleSiege::AddDbNPC() ERROR - DB NPC EXIST (CLS:%d, IDX:%d(%d), MAXHP:%d, HP:%d, DF:%d, RG:%d)",
						pNpcData.m_iNPC_NUM,
						pNpcData.m_iNPC_INDEX,
						pNpcData.m_iNPC_OBJINDEX,
						gObj[pNpcData.m_iNPC_OBJINDEX].MaxLife,
						gObj[pNpcData.m_iNPC_OBJINDEX].Life,
						gObj[pNpcData.m_iNPC_OBJINDEX].m_btCsNpcDfLevel,
						gObj[pNpcData.m_iNPC_OBJINDEX].m_btCsNpcRgLevel
						);
					break;
				}
			}

			// 실제 게임상에 추가하고 추가했다는 표시를 한다.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;				// 어차피 리젠되거나 고정 정보를 가진 NPC들이 아니다.
				gObj[iNPC_INDEX].X					= pNpcData.m_iNPC_SX;
				gObj[iNPC_INDEX].Y					= pNpcData.m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber			= MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX					= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY					= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX				= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY				= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir				= pNpcData.m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX			    = (BYTE)gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY				= (BYTE)gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen			= 0;
				gObj[iNPC_INDEX].MaxRegenTime		= 0;
#ifdef MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406	// 초기값으로 설정
				gObj[iNPC_INDEX].Life				= pNpcData.m_iNPC_BASE_HP;
				gObj[iNPC_INDEX].MaxLife			= pNpcData.m_iNPC_BASE_MAXHP;
#else
				gObj[iNPC_INDEX].Life				= pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife			= pNpcData.m_iNPC_MAXHP;
#endif
				gObj[iNPC_INDEX].m_btCsNpcType		= CS_NPC_TYPE_DB_DFN;					// 수성측 DB 몬스터

				INT iNPC_DF_LEVEL					= pNpcData.m_iNPC_BASE_DF_LEVEL;		// 생성 초기의 방어력 레벨
				if (iNPC_DF_LEVEL < 0)						
					iNPC_DF_LEVEL					= 0;
				if (iNPC_DF_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_DF_LEVEL					= MAX_UPGRADE_LEVEL;
				INT iNPC_RG_LEVEL					= pNpcData.m_iNPC_BASE_RG_LEVEL;		// 생성 초기의 회복력 레벨
				if (iNPC_RG_LEVEL < 0)						
					iNPC_RG_LEVEL					= 0;
				if (iNPC_RG_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_RG_LEVEL					= MAX_UPGRADE_LEVEL;

				// 회복력, 방어력 등의 수치가 필요하다면 여기서 세팅해준다.
				switch(pNpcData.m_iNPC_NUM) {
				case 277 :	// 성문
					gObj[iNPC_INDEX].m_Defense		= g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel			= iNPC_DF_LEVEL;			
					break;
				case 283 :	// 석상
					gObj[iNPC_INDEX].m_Defense		= g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel			= iNPC_DF_LEVEL;
					gObj[iNPC_INDEX].m_btCsNpcRgLevel			= iNPC_RG_LEVEL;
					break;
				}

				// 나중에 관리 확인을 하기위해 여기에 인증값을 넣는다.
				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;

				if (pNpcData.m_iNPC_NUM == 277) {
					// 임시 !!! -> 나중에 제거해야 함
					// 성문 생성 직후에는 성문을 열은 것으로 한다.
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_OPENED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[iNPC_INDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObj[iNPC_INDEX].m_btCsGateOpen	= CS_GATE_STATE_OPENED;
					// 임시 !!! -> 나중에 추가해야 함
					// 성문 생성 직후에는 성문을 닫은 것으로 한다.
//						SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_CLOSED);
//						gObj[iNPC_INDEX].m_btCsGateOpen	= CS_GATE_STATE_CLOSED;

					// 성문의 레버를 여기서 연결한다.
					BOOL bCreateLever		= FALSE;
					if (pNpcData.m_iCS_GATE_LEVER_INDEX == -1) {
						// 만들어진 레버가 없다.
						bCreateLever		= TRUE;
					}
					else {
						// 만들어진 레버가 있다.
						if (CheckLeverAlive(pNpcData.m_iCS_GATE_LEVER_INDEX) == FALSE) {
							bCreateLever	= TRUE;
						}
					}

					if (bCreateLever) {
						// 레버를 새로 만든다.
						INT iLeverIndex	= CreateCsGateLever(pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY + 4);
						if (CHECK_LIMIT(iLeverIndex, MAX_OBJECT)) {
							// 레버 생성 성공
							(*it).m_iCS_GATE_LEVER_INDEX	= iLeverIndex;
							// 만들어진 레버에 인덱스를 연결한다.
							BOOL bRETVAL	= LinkCsGateLever(iLeverIndex, iNPC_INDEX);
							if (bRETVAL == FALSE) {
								LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
									pNpcData.m_iNPC_INDEX
									);
							}
						}
						else {
							// 레버 생성 실패
							(*it).m_iCS_GATE_LEVER_INDEX	= -1;
							LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								pNpcData.m_iNPC_INDEX
								);
						}
					}
					else {
						// 만들어진 레버에 인덱스를 연결한다.
						BOOL bRETVAL	= LinkCsGateLever(pNpcData.m_iCS_GATE_LEVER_INDEX, iNPC_INDEX);
						if (bRETVAL == FALSE) {
							LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								pNpcData.m_iNPC_INDEX
								);
						}
					}
				}
				
				// 최종적으로 이 NPC는 생성되었음을 알린다.
				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
				(*it).m_bIN_USE						= TRUE;

				// 클라이언트에 결과를 알린다. -> 외부에서
				iRETVAL								= TRUE;
				
#ifdef MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406
				// 석상이나 성문에 대해 로그를 남긴다.
				if( pNpcData.m_iNPC_NUM == 277 || pNpcData.m_iNPC_NUM == 283 )
				{
					LogAddTD( "[CastleSiege][CreateDBNPC] (Index:%d, Num:%d, DF:%d, RG:%d, MaxHP:%d, HP:%d )",
								pNpcData.m_iNPC_INDEX,
								pNpcData.m_iNPC_NUM,
								pNpcData.m_iNPC_DF_LEVEL,
								pNpcData.m_iNPC_RG_LEVEL,
								pNpcData.m_iNPC_MAXHP,
								pNpcData.m_iNPC_HP
								);
				}				
#endif
				
				break;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
	
	return iRETVAL;
}


BOOL CCastleSiege::DelNPC					(								// DB에 저장하는 NPC중 하나를 삭제 -> 삭제 후 DB제거 함 (공성전 중이 아닐때 만)
	INT iIndex,
	INT iNpcType, 
	INT iMonsterExistVal,
	BOOL bDbSave
	)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return FALSE;

	BOOL bEXIST			= FALSE;
	BOOL bNPC_DB		= FALSE;
	CS_NPC_DATA pTempNpcData;

	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == iNpcType) &&
			(pNpcData.m_iNPC_OBJINDEX == iIndex) &&
			(pNpcData.m_iCsNpcExistVal == iMonsterExistVal)
			)
		{
			if ((pNpcData.m_bIN_USE	== TRUE) &&
				(pNpcData.m_iNPC_LIVE > NPC_LIVE_NONE)
				)
			{
				(*it).SetBaseValue();
				(*it).m_iNPC_OBJINDEX					= -1;
				(*it).m_iNPC_LIVE						= NPC_LIVE_NONE;
				// OBJECTSTRUCT 구조체의 iIndex 의 값은 외부에서 초기화 해주어야 한다.

#ifdef MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406
				// 성문과 석상의 경우 기본값으로 초기화 해 준다.
				if( pNpcData.m_iNPC_NUM == 277 || pNpcData.m_iNPC_NUM == 283 )
				{				
					pNpcData.m_iNPC_HP					= pNpcData.m_iNPC_BASE_HP;
					pNpcData.m_iNPC_MAXHP				= pNpcData.m_iNPC_BASE_MAXHP;				
					pNpcData.m_iNPC_RG_LEVEL			= pNpcData.m_iNPC_BASE_RG_LEVEL;				
					pNpcData.m_iNPC_DF_LEVEL			= pNpcData.m_iNPC_BASE_DF_LEVEL;
				}
#endif
				
				bEXIST									= TRUE;
				pTempNpcData							= pNpcData;

				if (pNpcData.m_iNPC_NUM == 277) {
					// 성문 제거시에는 성문자리의 속성을 없애준다.
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_OPENED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[pNpcData.m_iNPC_OBJINDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128

#ifdef MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406
					// 성문 삭제에 대한 로그를 남긴다. 이 때 공성 상태에 대해서도 기록한다.
					LogAddTD( "[CastleSiege][ State:%d ] Delete Castle Gate ( CLS:%d, IDX:%d, X:%d. Y:%d )",
							m_iCastleSiegeState,
							pNpcData.m_iNPC_NUM,
							pNpcData.m_iNPC_INDEX,
							pNpcData.m_iNPC_SX,
							pNpcData.m_iNPC_SY
							);
#endif
				}

				if (pNpcData.m_iNPC_NUM == 283) {
					// 정상적인 게임 진행 동안 석상 제거 시 -> 공성 기간 만
					if (m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE) {
						if (CheckGuardianStatueExist() == FALSE) {
							// 모든 석상이 제거되었다. -> 모든 방어막을 제거하고 공성측도 성에 올라갈 수 있도록 한다.
							ClearCastleTowerBarrier();
							SetCastleTowerAccessable(TRUE);
						}

						// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
						LogAddTD("[CastleSiege] Defend Statue is Broken (CLS:%d, IDX:%d, X:%d, Y:%d)",
							pNpcData.m_iNPC_NUM,
							pNpcData.m_iNPC_INDEX,
							pNpcData.m_iNPC_SX,
							pNpcData.m_iNPC_SY
							);
					}
				}
				break;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
	
//	if (bEXIST == TRUE && pTempNpcData.m_bNPC_DBSAVE == TRUE) {
	if (bEXIST == TRUE && bDbSave == TRUE) {
		// NPC가 존재했으며 지워졌다.
		if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE) {
			// 공성전 중이 아니라면 즉시 해당 NPC를 DB에서 제거한다.
			GS_GDReqNpcRemove (m_iMapSvrGroup, pTempNpcData.m_iNPC_NUM, pTempNpcData.m_iNPC_INDEX);
		}
	}

	return TRUE;
}


BOOL CCastleSiege::RepairDbNPC				(								// DB에 저장하는 NPC중 하나를 수리
	INT iNpcType, 
	INT iNpcIndex,
	INT iNpcHP,
	INT iNpcMaxHP
	)
{
	BOOL bEXIST			= FALSE;
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == iNpcType) &&
			(pNpcData.m_iNPC_INDEX == iNpcIndex)
			)
		{
			if ((pNpcData.m_bIN_USE	== TRUE) &&
				(pNpcData.m_iNPC_LIVE > NPC_LIVE_NONE)
				)
			{
				(*it).m_iNPC_HP						= iNpcHP;
				(*it).m_iNPC_MAXHP					= iNpcMaxHP;
				
				// OBJECTSTRUCT 구조체의 iIndex 의 값을 찾아서 존재한다면 체력을 수리한다.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].Class == iNpcType) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// 이것까지 확인하면 맞는 NPC이다.
						gObj[iNPC_INDEX].Life		= gObj[iNPC_INDEX].MaxLife;
						bEXIST						= TRUE;
					}
				}

				break;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	return bEXIST;
}

#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
BOOL CCastleSiege::PayForUpgradeDbNpc		(					// DB에 저장하는 NPC중 하나를 업그레이드하기 위해 필요한 비용을 지불한다.
	INT iIndex,
	INT	iNpcType,
	INT	iNpcIndex,
	INT	iNpcUpType,
	INT	iNpcUpValue,
	INT	iNpcUpIndex
	)
{
	// DS에 NPC 업그레이드를 요청하기 전 미리 필요한 비용을 지불한다.

	INT iNEED_GEMOFDEFEND	= 0;		// 필요한 수호보석
	INT iNEED_MONEY			= 0;		// 필요한 돈

	if( !CHECK_LIMIT( iNpcUpIndex, MAX_UPGRADE_LEVEL ) ) 
	{
		LogAddC( LOGC_RED,"[CastleSiege] CCastleSiege::PayForUpgradeDbNpc() ERROR - lpMsg->iNpcUpIndex is Out of Bound : %d",
			iNpcUpIndex
			);
		return FALSE;
	}
	
	CS_NPC_DATA pNpcData;
	BOOL bIsLive	= GetNpcData( iNpcType, iNpcIndex, pNpcData );
	if( !bIsLive ) 
	{
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::PayForUpgradeDbNpc() ERROR - bIsLive == FALSE", iNpcUpIndex );
		return FALSE;
	}

	switch(iNpcType) 
	{
	case 277 :		// 성문
		{
			// 업그레이드 하고자 하는 종류와 해당 단계가 존재하는지 검증한다.
			switch(iNpcUpType) 
			{
			case CS_UPGRADE_TYPE_DEFENSE :			// 업그레이드 - 방어력
				{
					iNEED_GEMOFDEFEND	= g_iNpcUpDfLevel_CGATE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpDfLevel_CGATE[iNpcUpIndex][2];

					if( gObj[iIndex].Money < iNEED_MONEY ) 
					{
						GCAnsNpcUpgrade( iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					
					BOOL bDelGemOfDefend	= DelGemOfDefend( iIndex, iNEED_GEMOFDEFEND );
					if (bDelGemOfDefend == FALSE) 
					{
						GCAnsNpcUpgrade( iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;				
					
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// 업그레이드 - 최대HP
				{
					iNEED_GEMOFDEFEND	= g_iNpcUpMaxHP_CGATE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpMaxHP_CGATE[iNpcUpIndex][2];

					if( gObj[iIndex].Money < iNEED_MONEY )  
					{
						GCAnsNpcUpgrade( iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					
					BOOL bDelGemOfDefend	= DelGemOfDefend( iIndex, iNEED_GEMOFDEFEND );
					

					if( bDelGemOfDefend == FALSE ) 
					{
						GCAnsNpcUpgrade( iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;					
					
				}
				break;
			default :
				{
					// 해당 타입은 없다 - 에러
					GCAnsNpcUpgrade( iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
					return FALSE;
				}
				break;
			}
		}
		break;
	case 283 :		// 수호석상
		{
			// 업그레이드 하고자 하는 종류와 해당 단계가 존재하는지 검증한다.
			switch( iNpcUpType ) 
			{
			case CS_UPGRADE_TYPE_DEFENSE :			// 업그레이드 - 방어력
				{
					// 업그레이드에 대한 유효성을 검사한다.
					iNEED_GEMOFDEFEND	= g_iNpcUpDfLevel_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpDfLevel_CSTATUE[iNpcUpIndex][2];

					if( gObj[iIndex].Money < iNEED_MONEY ) 
					{
						GCAnsNpcUpgrade( iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					BOOL bDelGemOfDefend	= DelGemOfDefend( iIndex, iNEED_GEMOFDEFEND );
					if( bDelGemOfDefend == FALSE ) 
					{
						GCAnsNpcUpgrade( iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;					
				}
				break;
			case CS_UPGRADE_TYPE_REGENARATION :		// 업그레이드 - 회복력
				{
					// 업그레이드에 대한 유효성을 검사한다.
					iNEED_GEMOFDEFEND	= g_iNpcUpRgLevel_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpRgLevel_CSTATUE[iNpcUpIndex][2];

					if( gObj[iIndex].Money < iNEED_MONEY ) 
					{
						GCAnsNpcUpgrade( iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					BOOL bDelGemOfDefend	= DelGemOfDefend( iIndex, iNEED_GEMOFDEFEND );
					if( bDelGemOfDefend == FALSE ) 
					{
						GCAnsNpcUpgrade( iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// 업그레이드 - 최대HP
				{
					// 현재의 HP는 몇단계인지 확인 -> 임의의 HP값도 조건비교로 특정 레벨로 고정시킴
					iNEED_GEMOFDEFEND	= g_iNpcUpMaxHP_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpMaxHP_CSTATUE[iNpcUpIndex][2];

					if( gObj[iIndex].Money < iNEED_MONEY ) 
					{
						GCAnsNpcUpgrade( iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					BOOL bDelGemOfDefend		= DelGemOfDefend( iIndex, iNEED_GEMOFDEFEND );
					if( bDelGemOfDefend == FALSE ) 
					{
						GCAnsNpcUpgrade( iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;
				}
				break;
			default :
				{
					// 해당 타입은 없다 - 에러
					GCAnsNpcUpgrade( iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
					return FALSE;
				}
				break;
			}
		}
		break;
	}

	GCMoneySend( iIndex, gObj[iIndex].Money );
	return TRUE;	
}

BOOL CCastleSiege::UpgradeDbNPC				(					// DB에 저장하는 NPC중 하나를 업그레이드 -> 이 전에 이미 DB에서 업그레이드 됨
	INT iIndex,
	INT	iNpcType,
	INT	iNpcIndex,
	INT	iNpcUpType,
	INT	iNpcUpValue,
	INT	iNpcUpIndex
	)
{
	// 여기서 실제로 NPC 업그레이드를 실시한다.
	
	if( !CHECK_LIMIT(iNpcUpIndex, MAX_UPGRADE_LEVEL ) ) 
	{
		LogAddC( LOGC_RED,"[CastleSiege] CCastleSiege::UpgradeDbNPC() ERROR - lpMsg->iNpcUpIndex is Out of Bound : %d", iNpcUpIndex );
		return FALSE;
	}
	
	CS_NPC_DATA pNpcData;
	BOOL bIsLive	= GetNpcData( iNpcType, iNpcIndex, pNpcData );
	if( !bIsLive ) 
	{
		LogAddC( LOGC_RED,"[CastleSiege] CCastleSiege::UpgradeDbNPC() ERROR - bIsLive == FALSE", iNpcUpIndex );
		return FALSE;
	}

	switch(iNpcType) 
	{
	case 277 :		// 성문
		{
			// 업그레이드 하고자 하는 종류와 해당 단계가 존재하는지 검증한다.
			switch( iNpcUpType ) 
			{
			case CS_UPGRADE_TYPE_DEFENSE :			// 업그레이드 - 방어력
				{
					// 여기서 실제 성문의 방어력을 업그레이드 함
					UpgradeDbNPC_DFLEVEL( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// 업그레이드 - 최대HP
				{					
					// 여기서 실제 성문의 최대 HP를 업그레이드 함
					UpgradeDbNPC_MAXHP( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			default :
				{
					// 해당 타입은 없다 - 에러
					GCAnsNpcUpgrade( iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
					return FALSE;
				}
				break;
			}
		}
		break;
	case 283 :		// 수호석상
		{
			// 업그레이드 하고자 하는 종류와 해당 단계가 존재하는지 검증한다.
			switch(iNpcUpType) 
			{
			case CS_UPGRADE_TYPE_DEFENSE :			// 업그레이드 - 방어력
				{
					// 여기서 석상의 최대 방어력을 업그레이드 함
					UpgradeDbNPC_DFLEVEL( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			case CS_UPGRADE_TYPE_REGENARATION :		// 업그레이드 - 회복력
				{					
					// 여기서 실제 석상의 회복력을 업그레이드 함
					UpgradeDbNPC_RGLEVEL( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// 업그레이드 - 최대HP
				{
					// 여기서 실제 석상의 최대 HP를 업그레이드 함
					UpgradeDbNPC_MAXHP( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			default :
				{
					// 해당 타입은 없다 - 에러
					GCAnsNpcUpgrade( iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
					return FALSE;
				}
				break;
			}
		}
		break;
	}	
	return TRUE;
}

#else	// #ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
BOOL CCastleSiege::UpgradeDbNPC				(								// DB에 저장하는 NPC중 하나를 업그레이드 -> 이 전에 이미 DB에서 업그레이드 됨
	INT iIndex,
	INT	iNpcType,
	INT	iNpcIndex,
	INT	iNpcUpType,
	INT	iNpcUpValue,
	INT	iNpcUpIndex
	)
{
	// 여기서 실제로 NPC 업그레이드를 실시한다.

	INT iNEED_GEMOFDEFEND	= 0;		// 필요한 수호보석
	INT iNEED_MONEY			= 0;		// 필요한 돈

	if (!CHECK_LIMIT(iNpcUpIndex, MAX_UPGRADE_LEVEL)) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::UpgradeDbNPC() ERROR - lpMsg->iNpcUpIndex is Out of Bound : %d",
			iNpcUpIndex
			);
		return FALSE;
	}
	
	CS_NPC_DATA pNpcData;
	BOOL bIsLive	= GetNpcData(iNpcType, iNpcIndex, pNpcData);
	if (!bIsLive) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::UpgradeDbNPC() ERROR - bIsLive == FALSE",
			iNpcUpIndex
			);
		return FALSE;
	}

	switch(iNpcType) {
	case 277 :		// 성문
		{
			// 업그레이드 하고자 하는 종류와 해당 단계가 존재하는지 검증한다.
			switch(iNpcUpType) {
			case CS_UPGRADE_TYPE_DEFENSE :			// 업그레이드 - 방어력
				{
					iNEED_GEMOFDEFEND	= g_iNpcUpDfLevel_CGATE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpDfLevel_CGATE[iNpcUpIndex][2];

					if (gObj[iIndex].Money < iNEED_MONEY) {
						GCAnsNpcUpgrade (iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					BOOL bDelGemOfDefend	= DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);
					if (bDelGemOfDefend == FALSE) {
						GCAnsNpcUpgrade (iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;

					// 여기서 실제 성문의 방어력을 업그레이드 함
					UpgradeDbNPC_DFLEVEL(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// 업그레이드 - 최대HP
				{
					iNEED_GEMOFDEFEND	= g_iNpcUpMaxHP_CGATE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpMaxHP_CGATE[iNpcUpIndex][2];

					if (gObj[iIndex].Money < iNEED_MONEY) {
						GCAnsNpcUpgrade (iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					BOOL bDelGemOfDefend	= DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);
					if (bDelGemOfDefend == FALSE) {
						GCAnsNpcUpgrade (iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;
					
					// 여기서 실제 성문의 최대 HP를 업그레이드 함
					UpgradeDbNPC_MAXHP(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			default :
				{
					// 해당 타입은 없다 - 에러
					GCAnsNpcUpgrade (iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
					return FALSE;
				}
				break;
			}
		}
		break;
	case 283 :		// 수호석상
		{
			// 업그레이드 하고자 하는 종류와 해당 단계가 존재하는지 검증한다.
			switch(iNpcUpType) {
			case CS_UPGRADE_TYPE_DEFENSE :			// 업그레이드 - 방어력
				{
					// 업그레이드에 대한 유효성을 검사한다.
					iNEED_GEMOFDEFEND	= g_iNpcUpDfLevel_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpDfLevel_CSTATUE[iNpcUpIndex][2];

					if (gObj[iIndex].Money < iNEED_MONEY) {
						GCAnsNpcUpgrade (iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					BOOL bDelGemOfDefend	= DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);
					if (bDelGemOfDefend == FALSE) {
						GCAnsNpcUpgrade (iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;

					// 여기서 석상의 최대 방어력을 업그레이드 함
					UpgradeDbNPC_DFLEVEL(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			case CS_UPGRADE_TYPE_REGENARATION :		// 업그레이드 - 회복력
				{
					// 업그레이드에 대한 유효성을 검사한다.
					iNEED_GEMOFDEFEND	= g_iNpcUpRgLevel_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpRgLevel_CSTATUE[iNpcUpIndex][2];

					if (gObj[iIndex].Money < iNEED_MONEY) {
						GCAnsNpcUpgrade (iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					BOOL bDelGemOfDefend	= DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);
					if (bDelGemOfDefend == FALSE) {
						GCAnsNpcUpgrade (iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;

					// 여기서 실제 석상의 회복력을 업그레이드 함
					UpgradeDbNPC_RGLEVEL(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// 업그레이드 - 최대HP
				{
					// 현재의 HP는 몇단계인지 확인 -> 임의의 HP값도 조건비교로 특정 레벨로 고정시킴
					iNEED_GEMOFDEFEND	= g_iNpcUpMaxHP_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY			= g_iNpcUpMaxHP_CSTATUE[iNpcUpIndex][2];

					if (gObj[iIndex].Money < iNEED_MONEY) {
						GCAnsNpcUpgrade (iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					BOOL bDelGemOfDefend		= DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);
					if (bDelGemOfDefend == FALSE) {
						GCAnsNpcUpgrade (iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}
					gObj[iIndex].Money			-= iNEED_MONEY;

					// 여기서 실제 석상의 최대 HP를 업그레이드 함
					UpgradeDbNPC_MAXHP(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			default :
				{
					// 해당 타입은 없다 - 에러
					GCAnsNpcUpgrade (iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
					return FALSE;
				}
				break;
			}
		}
		break;
	}

	GCMoneySend(iIndex, gObj[iIndex].Money);
	return TRUE;
}
#endif //	#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619

BOOL CCastleSiege::UpgradeDbNPC_DFLEVEL		(								// DB에 저장하는 특정 NPC의 방어력 업그레이드
	INT	iNpcType,
	INT	iNpcIndex,
	INT	iNpcDfLevel
	)
{
	BOOL bEXIST			= FALSE;
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == iNpcType) &&
			(pNpcData.m_iNPC_INDEX == iNpcIndex)
			)
		{
			if ((pNpcData.m_bIN_USE	== TRUE) &&
				(pNpcData.m_iNPC_LIVE > NPC_LIVE_NONE)
				)
			{
				// OBJECTSTRUCT 구조체의 iIndex 의 값을 찾아서 실제 존재하는지 확인한다.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// 이것까지 확인하면 맞는 NPC이다.
						(*it).m_iNPC_DF_LEVEL				= iNpcDfLevel;
						bEXIST								= TRUE;

						// 여기서 OBJECTSTRUCT 객체의 실제 방어력을 올려준다.
						gObj[iNPC_INDEX].m_Defense			= g_iNpcDefense_CGATE[iNpcDfLevel];
						gObj[iNPC_INDEX].m_MagicDefense		= g_iNpcDefense_CGATE[iNpcDfLevel];
						gObj[iNPC_INDEX].m_btCsNpcDfLevel	= iNpcDfLevel;
					}
				}

				break;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	return bEXIST;
}


BOOL CCastleSiege::UpgradeDbNPC_RGLEVEL		(								// DB에 저장하는 특정 NPC의 회복력 업그레이드
	INT	iNpcType,
	INT	iNpcIndex,
	INT	iNpcRgLevel
	)
{
	BOOL bEXIST			= FALSE;
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == iNpcType) &&
			(pNpcData.m_iNPC_INDEX == iNpcIndex)
			)
		{
			if ((pNpcData.m_bIN_USE	== TRUE) &&
				(pNpcData.m_iNPC_LIVE > NPC_LIVE_NONE)
				)
			{
				// OBJECTSTRUCT 구조체의 iIndex 의 값을 찾아서 실제 존재하는지 확인한다.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// 이것까지 확인하면 맞는 NPC이다.
						(*it).m_iNPC_RG_LEVEL				= iNpcRgLevel;
						bEXIST								= TRUE;

						// 여기서 OBJECTSTRUCT 객체의 실제 회복력을 올려준다.
						gObj[iNPC_INDEX].m_btCsNpcRgLevel	= iNpcRgLevel;
					}
				}

				break;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	return bEXIST;
}


BOOL CCastleSiege::UpgradeDbNPC_MAXHP		(								// DB에 저장하는 특정 NPC의 최대HP 업그레이드
	INT	iNpcType,
	INT	iNpcIndex,
	INT	iNpcMaxHP
	)
{
	BOOL bEXIST			= FALSE;
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == iNpcType) &&
			(pNpcData.m_iNPC_INDEX == iNpcIndex)
			)
		{
			if ((pNpcData.m_bIN_USE	== TRUE) &&
				(pNpcData.m_iNPC_LIVE > NPC_LIVE_NONE)
				)
			{
				// OBJECTSTRUCT 구조체의 iIndex 의 값을 찾아서 실제 존재하는지 확인한다.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// 이것까지 확인하면 맞는 NPC이다.
						(*it).m_iNPC_MAXHP			= iNpcMaxHP;
						(*it).m_iNPC_HP				= iNpcMaxHP;
						bEXIST						= TRUE;

						// 여기서 OBJECTSTRUCT 객체의 실제 최대HP를 올려준다.
						gObj[iNPC_INDEX].MaxLife	= iNpcMaxHP;	
						gObj[iNPC_INDEX].Life		= iNpcMaxHP;	
					}
				}

				break;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	return bEXIST;
}


VOID CCastleSiege::StoreDbNpc				()				// DB에 저장하는 NPC들을 일괄적으로 DB에 저장 (이전 데이터 삭제)
{
	LogAddTD("[CastleSiege] CCastleSiege::StoreDbNpc() - << START >>");

	CHAR cBUFFER[sizeof(CSP_REQ_NPCUPDATEDATA) + sizeof(CSP_NPCUPDATEDATA) * (MAX_CS_NPC_COUNT+1)];
	LPCSP_REQ_NPCUPDATEDATA lpMsg	= (LPCSP_REQ_NPCUPDATEDATA)cBUFFER;
	LPCSP_NPCUPDATEDATA lpMsgBody	= (LPCSP_NPCUPDATEDATA)(cBUFFER + sizeof(CSP_REQ_NPCUPDATEDATA));

	INT iCOUNT						= 0;

	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it				= m_vtNpcData.begin();
	BOOL bExist	= FALSE;

	while (it != m_vtNpcData.end()) {
		if (iCOUNT >= MAX_CS_NPC_COUNT)
			break;

		CS_NPC_DATA & pNpcData						= ((CS_NPC_DATA)(*it));
		if (pNpcData.m_bNPC_DBSAVE == TRUE) {
			if (gObjIsConnected(pNpcData.m_iNPC_OBJINDEX)) {
				if (gObj[pNpcData.m_iNPC_OBJINDEX].Class == pNpcData.m_iNPC_NUM &&
					gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal &&
					gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal != 0 &&
					gObj[pNpcData.m_iNPC_OBJINDEX].Live == 1 &&
					gObj[pNpcData.m_iNPC_OBJINDEX].Life > 0
					) 
				{
					lpMsgBody[iCOUNT].iNpcNumber	= pNpcData.m_iNPC_NUM;
					lpMsgBody[iCOUNT].iNpcIndex		= pNpcData.m_iNPC_INDEX;
					lpMsgBody[iCOUNT].iNpcDfLevel	= pNpcData.m_iNPC_DF_LEVEL;
					lpMsgBody[iCOUNT].iNpcRgLevel	= pNpcData.m_iNPC_RG_LEVEL;
					lpMsgBody[iCOUNT].iNpcMaxHp		= gObj[pNpcData.m_iNPC_OBJINDEX].MaxLife;
					lpMsgBody[iCOUNT].iNpcHp		= gObj[pNpcData.m_iNPC_OBJINDEX].Life;
					lpMsgBody[iCOUNT].btNpcX		= pNpcData.m_iNPC_SX;
					lpMsgBody[iCOUNT].btNpcY		= pNpcData.m_iNPC_SY;
					lpMsgBody[iCOUNT].btNpcDIR		= pNpcData.m_iNPC_DIR;

					LogAddTD("[CastleSiege] CCastleSiege::StoreDbNpc() NPC INFO (CLS:%d, IDX:%d(%d), MAXHP:%d, HP:%d, DF:%d, RG:%d)",
						pNpcData.m_iNPC_NUM,
						pNpcData.m_iNPC_INDEX,
						pNpcData.m_iNPC_OBJINDEX,
						lpMsgBody[iCOUNT].iNpcMaxHp,
						lpMsgBody[iCOUNT].iNpcHp,
						lpMsgBody[iCOUNT].iNpcDfLevel,
						lpMsgBody[iCOUNT].iNpcRgLevel
						);

					iCOUNT++;
				}
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	if (iCOUNT < 0) {
		iCOUNT						= 0;
	}
	
	lpMsg->wMapSvrNum				= m_iMapSvrGroup;
	lpMsg->iCount					= iCOUNT;
	lpMsg->h.set((LPBYTE)lpMsg, 0x89, sizeof(CSP_REQ_NPCUPDATEDATA) + sizeof(CSP_NPCUPDATEDATA) * iCOUNT);
	cDBSMng.Send((char*)lpMsg, sizeof(CSP_REQ_NPCUPDATEDATA) + sizeof(CSP_NPCUPDATEDATA) * iCOUNT);
	
	LogAddTD("[CastleSiege] CCastleSiege::StoreDbNpc() - << END >>");
}


VOID CCastleSiege::SendNpcStateList			(								// 특정 사람에게 특정 몬스터의 현재 상태 리스트를 보내준다.
	INT iIndex, 
	INT iNpcType
	)
{
	if (!gObjIsConnected(iIndex)) {
		return;
	}

	CHAR cNPC_LIST [sizeof(PMSG_ANS_NPCDBLIST) + sizeof(PMSG_NPCDBLIST) * MAX_NPC_SEND_COUNT] = {0,};
	INT iNPC_COUNT	= 0;

	LPPMSG_ANS_NPCDBLIST lpMsgSend		= (LPPMSG_ANS_NPCDBLIST)cNPC_LIST;
	LPPMSG_NPCDBLIST lpMsgSendBody		= (LPPMSG_NPCDBLIST)(cNPC_LIST + sizeof(PMSG_ANS_NPCDBLIST));
	
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		if (iNPC_COUNT >= MAX_NPC_SEND_COUNT)
			break;
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if (pNpcData.m_iNPC_NUM	== iNpcType && pNpcData.m_bIN_USE == TRUE)
		{
			INT iHP		= 0;
			INT iMAX_HP	= 0;
			BYTE btLIVE	= 0;
			
			// OBJECTSTRUCT 구조체의 iIndex 의 값을 찾아서 실제 존재하는지 확인한다.
			INT iNPC_INDEX							= pNpcData.m_iNPC_OBJINDEX;
			if (gObjIsConnected(iNPC_INDEX)) {
				if ((gObj[iNPC_INDEX].Class == iNpcType) &&
					(gObj[iNPC_INDEX].Live == 1) &&
					(gObj[iNPC_INDEX].Life > 0) &&
					(gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
					(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
					(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
					)
				{
					// 이것까지 확인하면 맞는 NPC이다.
					iHP								= gObj[iNPC_INDEX].Life;
					iMAX_HP							= gObj[iNPC_INDEX].MaxLife;
					btLIVE							= 1;
				}
				else {
					iHP								= 0;
					iMAX_HP							= pNpcData.m_iNPC_BASE_MAXHP;
					btLIVE							= 0;
				}
			}
			else {
				iHP									= 0;
				iMAX_HP								= pNpcData.m_iNPC_BASE_MAXHP;
				btLIVE								= 0;
			}

			lpMsgSendBody[iNPC_COUNT].iNpcNumber	= pNpcData.m_iNPC_NUM;
			lpMsgSendBody[iNPC_COUNT].iNpcIndex		= pNpcData.m_iNPC_INDEX;
			lpMsgSendBody[iNPC_COUNT].iNpcDfLevel	= pNpcData.m_iNPC_DF_LEVEL;
			lpMsgSendBody[iNPC_COUNT].iNpcRgLevel	= pNpcData.m_iNPC_RG_LEVEL;
			lpMsgSendBody[iNPC_COUNT].iNpcHp		= iHP;
			lpMsgSendBody[iNPC_COUNT].iNpcMaxHp		= iMAX_HP;
			lpMsgSendBody[iNPC_COUNT].btNpcX		= pNpcData.m_iNPC_SX;
			lpMsgSendBody[iNPC_COUNT].btNpcY		= pNpcData.m_iNPC_SY;

			lpMsgSendBody[iNPC_COUNT].btNpcLive		= btLIVE;
		
			iNPC_COUNT++;
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
	
	lpMsgSend->btResult	= 1;
	lpMsgSend->h.set((LPBYTE)&cNPC_LIST, 0xB3, sizeof(PMSG_ANS_NPCDBLIST) + sizeof(PMSG_NPCDBLIST) * iNPC_COUNT);
	lpMsgSend->iCount	= iNPC_COUNT;
	DataSend(iIndex, (LPBYTE)&cNPC_LIST, sizeof(PMSG_ANS_NPCDBLIST) + sizeof(PMSG_NPCDBLIST) * iNPC_COUNT);
}


BOOL CCastleSiege::GetNpcData				(							// 현재 관리되는 특정 NPC에 대한 정보를 보냄 (FALSE는 데이터 없음)
	INT iNpcType, 
	INT iNpcIndex,
	CS_NPC_DATA & pRetNpcData
	)
{
	pRetNpcData.Clear();

	BOOL bEXIST			= FALSE;
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == iNpcType) &&
			(pNpcData.m_iNPC_INDEX == iNpcIndex)
			)
		{
			if ((pNpcData.m_bIN_USE	== TRUE) &&
				(pNpcData.m_iNPC_LIVE > NPC_LIVE_NONE)
				)
			{
				// OBJECTSTRUCT 구조체의 iIndex 의 값을 찾아서 실제 존재하는지 확인한다.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// 이것까지 확인하면 맞는 NPC이다.
						pRetNpcData					= pNpcData;
						bEXIST						= TRUE;
					}
				}

				break;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	return bEXIST;
}


VOID CCastleSiege::AdjustDbNpcLevel			()							// DB NPC의 현재 자료상의 레벨을 사용자 객체에 적용한다. -> 공성 시작 전에는 이렇게 해주어야 한다.
{
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it			= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData		= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_bIN_USE	== TRUE) &&
			(pNpcData.m_iNPC_LIVE > NPC_LIVE_NONE)
			)
		{
			// OBJECTSTRUCT 구조체의 iIndex 의 값을 찾아서 실제 존재하는지 확인한다.
			INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
			if (gObjIsConnected(iNPC_INDEX)) {
				if ((gObj[iNPC_INDEX].Class == pNpcData.m_iNPC_NUM) &&
					(gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
					(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
					(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
					)
				{
					// 이것까지 확인하면 맞는 NPC이다.
					switch(gObj[iNPC_INDEX].Class) {
					case 277 :	// 성문의 경우
						{
							// 방어력 적용
							INT iDF_LEVEL							= pNpcData.m_iNPC_DF_LEVEL;
							if (iDF_LEVEL < 0)						iDF_LEVEL			= 0;
							if (iDF_LEVEL > MAX_UPGRADE_LEVEL)		iDF_LEVEL			= MAX_UPGRADE_LEVEL;
							gObj[iNPC_INDEX].m_btCsNpcDfLevel		= iDF_LEVEL;
							gObj[iNPC_INDEX].m_Defense				= g_iNpcDefense_CGATE[iDF_LEVEL];
						}
						break;
					case 283 :	// 석상의 경우
						{
							// 방어력 적용
							INT iDF_LEVEL							= pNpcData.m_iNPC_DF_LEVEL;
							if (iDF_LEVEL < 0)						iDF_LEVEL			= 0;
							if (iDF_LEVEL > MAX_UPGRADE_LEVEL)		iDF_LEVEL			= MAX_UPGRADE_LEVEL;
							gObj[iNPC_INDEX].m_btCsNpcDfLevel		= iDF_LEVEL;
							gObj[iNPC_INDEX].m_Defense				= g_iNpcDefense_CSTATUE[iDF_LEVEL];

							// 회목력 적용
							gObj[iNPC_INDEX].m_btCsNpcRgLevel		= pNpcData.m_iNPC_RG_LEVEL;
						}
						break;
					}
				}
			}
			break;
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


VOID CCastleSiege::SendCsGateStateViewPort	(							// 성의 문 상태를 나를 보고 있는 사람들에게 보두 보냄
	INT iGateIndex,
	BYTE btOperate
	)
{
	if (!gObjIsConnected(iGateIndex))
		return;

	LPOBJECTSTRUCT lpObj	= &gObj[iGateIndex];

	// 뷰포트 상에 아무도 없다면 끝난다.
	if( lpObj->VPCount2 < 1 ) return;

	// 뷰포트 상에서 나를 보고있는 사람들을 찾는다.
	INT	tObjNum	= -1;
	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
	{
		tObjNum = lpObj->VpPlayer2[i].number;
		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObjIsConnected(tObjNum) )
			{
				GCAnsCsGateCurState (tObjNum, iGateIndex, btOperate);
			}
		}
	}
}


VOID CCastleSiege::CreateAllCsGateLever		()							// 공성 데이터 세팅이 성공적으로 끝난 직후에 레버를 일괄적으로 생성해 준다.
{
	EnterCriticalSection (&m_critNpcData);
	if (!m_vtNpcData.empty()) {
		vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
		for (; it != m_vtNpcData.end() ; it++) {
			CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
			if (pNpcData.m_iNPC_NUM == 277) {
				if (pNpcData.m_iCS_GATE_LEVER_INDEX != -1) {
					if (CheckLeverAlive(pNpcData.m_iCS_GATE_LEVER_INDEX) == TRUE) {
						// 레버가 이미 존재한다면 넘어간다.
						continue;
					}
				}

				// 레버를 새로 만든다.
				INT iLeverIndex	= CreateCsGateLever(pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY + 4);
				if (CHECK_LIMIT(iLeverIndex, MAX_OBJECT)) {
					// 레버 생성 성공
					(*it).m_iCS_GATE_LEVER_INDEX	= iLeverIndex;							
				}
				else {
					// 레버 생성 실패
					(*it).m_iCS_GATE_LEVER_INDEX	= -1;
					LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateAllCsGateLever() ERROR - Failed to Create Gate Lever : GateIndex:%d",
						pNpcData.m_iNPC_INDEX
						);
				}
			}
		}
	}
	LeaveCriticalSection (&m_critNpcData);
}


INT	CCastleSiege::CreateCsGateLever			(							// 특정 지역에 성문의 레버를 만들고 인덱스를 리턴한다. (실패는 -1)
	INT iX, 
	INT iY
	)
{
	if (!CHECK_LIMIT(iX, MAX_TERRAIN_SIZE) || !CHECK_LIMIT(iY, MAX_TERRAIN_SIZE)) {
		return -1;
	}

	// 벡터안에 존재하는 NPC라면 살아있는 상태로 바꾸어주고 데이터도 DB에 맞게 수정한다.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
	INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
	INT iNPC_INDEX = gObjAdd();
#endif
	if( iNPC_INDEX >= 0 )
	{
		gObjSetMonster(iNPC_INDEX, 219);

		gObj[iNPC_INDEX].m_PosNum					= -1;						// 어차피 리젠되거나 고정 정보를 가진 NPC들이 아니다.
		gObj[iNPC_INDEX].X							= iX;
		gObj[iNPC_INDEX].Y							= iY;
		gObj[iNPC_INDEX].MapNumber					= MAP_INDEX_CASTLESIEGE;
		gObj[iNPC_INDEX].TX							= gObj[iNPC_INDEX].X;
		gObj[iNPC_INDEX].TY							= gObj[iNPC_INDEX].Y;
		gObj[iNPC_INDEX].m_OldX						= gObj[iNPC_INDEX].X;
		gObj[iNPC_INDEX].m_OldY						= gObj[iNPC_INDEX].Y;
		gObj[iNPC_INDEX].Dir						= 1;
		gObj[iNPC_INDEX].StartX					    = (BYTE)gObj[iNPC_INDEX].X;
		gObj[iNPC_INDEX].StartY						= (BYTE)gObj[iNPC_INDEX].Y;
		gObj[iNPC_INDEX].DieRegen					= 0;
		gObj[iNPC_INDEX].MaxRegenTime				= 0;
		gObj[iNPC_INDEX].Life						= 10000;
		gObj[iNPC_INDEX].MaxLife					= 10000;
		gObj[iNPC_INDEX].m_btCsNpcType				= CS_NPC_TYPE_INS_DFN;		// 수성측 NPC임 (DB에 저장되지 않음)

		// 레버는 성문의 인덱스를 비워놓는다.
		gObj[iNPC_INDEX].m_iCsGateLeverLinkIndex	= -1;

		return iNPC_INDEX;
	}
	
	return -1;
}


BOOL CCastleSiege::LinkCsGateLever			(							// 이미 성문이 만들어져 있다면 특정 성문과 연결한다.
	INT iLeverIndex,
	INT iGateIndex
	)
{
	if (!gObjIsConnected(iLeverIndex) || gObj[iLeverIndex].Class != 219) {
		return FALSE;
	}
	if (!gObjIsConnected(iGateIndex) || gObj[iGateIndex].Class != 277) {
		return FALSE;
	}

	// 레버는 성문의 인덱스를 가진다.
	gObj[iLeverIndex].m_iCsGateLeverLinkIndex		= iGateIndex;

	// 성문도 레버의 인덱스를 가진다.
	gObj[iGateIndex].m_iCsGateLeverLinkIndex		= iLeverIndex;
	
	return TRUE;
}

BOOL CCastleSiege::CheckLeverAlive			(							// 특정 인덱스가 성문레버이고 살아있느지를 확인
	INT iLeverIndex
	)
{
	if (!gObjIsConnected(iLeverIndex) || gObj[iLeverIndex].Class != 219) {
		return FALSE;
	}

	return TRUE;
}


VOID CCastleSiege::CheckCsDbNpcAlive		()							// 성문들이 살아있는지 확인하여 죽었다면 블럭속성 해제
{
	if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE)
		return;

	EnterCriticalSection (&m_critNpcData);
	if (!m_vtNpcData.empty()) {
		vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
		for (; it != m_vtNpcData.end() ; it++) {
			CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
			if ((pNpcData.m_iNPC_NUM == 277 ||	// 성문
				pNpcData.m_iNPC_NUM == 283		// 석상
				) &&
				pNpcData.m_iNPC_OBJINDEX != -1
				) 
			{
				if (!gObjIsConnected(pNpcData.m_iNPC_OBJINDEX) ||
					(gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal != pNpcData.m_iCsNpcExistVal) ||
					(gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal == 0) ||
					(gObj[pNpcData.m_iNPC_OBJINDEX].Class != 277 && gObj[pNpcData.m_iNPC_OBJINDEX].Class != 283) ||
					(gObj[pNpcData.m_iNPC_OBJINDEX].Live == 0)
					) 
				{
#ifdef MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406
					// 초기화
					pNpcData.m_iNPC_HP					= pNpcData.m_iNPC_BASE_HP;
					pNpcData.m_iNPC_MAXHP				= pNpcData.m_iNPC_BASE_MAXHP;				
					pNpcData.m_iNPC_RG_LEVEL			= pNpcData.m_iNPC_BASE_RG_LEVEL;				
					pNpcData.m_iNPC_DF_LEVEL			= pNpcData.m_iNPC_BASE_DF_LEVEL;	
#endif

					switch(pNpcData.m_iNPC_NUM) {
					case 277 :					// 성문 - 블럭속성을 해제해 준다.
						{
							SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_OPENED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
							gObjAddBuffEffect( &gObj[pNpcData.m_iNPC_OBJINDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128

							// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
							LogAddTD("[CastleSiege] Castle Gate is Broken (CLS:%d, IDX:%d, X:%d, Y:%d)",
								pNpcData.m_iNPC_NUM,
								pNpcData.m_iNPC_INDEX,
								pNpcData.m_iNPC_SX,
								pNpcData.m_iNPC_SY
								);
						}
						break;
					case 283 :					// 석상 - 다른 석상들이 제거되었는지 확인하여 용의 탑 입구를 개방한다.
						{
							if (CheckGuardianStatueExist() == FALSE) {
								// 모든 석상이 제거되었다. -> 모든 방어막을 제거하고 공성측도 성에 올라갈 수 있도록 한다.
								ClearCastleTowerBarrier();
								SetCastleTowerAccessable(TRUE);
							}

							// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
							LogAddTD("[CastleSiege] Defend Statue is Broken (CLS:%d, IDX:%d, X:%d, Y:%d)",
								pNpcData.m_iNPC_NUM,
								pNpcData.m_iNPC_INDEX,
								pNpcData.m_iNPC_SX,
								pNpcData.m_iNPC_SY
								);
						}
						break;
					}
					
					(*it).m_iNPC_OBJINDEX	= -1;
				}
			}
		}
	}
	LeaveCriticalSection (&m_critNpcData);
}


BOOL CCastleSiege::CheckCsGateAlive			(							// 특정 인덱스가 성문이고 살아있는지를 확인
	INT iGateIndex
	)
{
	if (!gObjIsConnected(iGateIndex) || gObj[iGateIndex].Class != 277) {
		return FALSE;
	}

	return TRUE;
}


BOOL CCastleSiege::DelGemOfDefend			(							// 특정 사용자에게서 특정 개수의 수호보석을 제거한다.
	INT iIndex,
	INT iNeedGemOfDefend
	)
{
	if (!gObjIsConnected(iIndex))
		return FALSE;
	if (iNeedGemOfDefend <= 0)
		return FALSE;

	INT iGemOfDefendCount	= 0;

	int x;

	// 1 . 우선 수호의 보석이 개수만큼 있는지 확인한다.
#ifdef PERSONAL_SHOP_20040113
	for(x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
	for(x=0; x<MAX_INVENTORY; x++)
#endif
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 31) )
			{
				iGemOfDefendCount++;
			}
		}
	}
	if(iGemOfDefendCount < iNeedGemOfDefend) {
		// 개수가 모자람
		return FALSE;
	}

	// 2 . 여기서 실제로 제거한다.
	INT iGemOfDefendDeletCount	= 0;
#ifdef PERSONAL_SHOP_20040113
	for(x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
	for(x=0; x<MAX_INVENTORY; x++)
#endif
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if(gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 31 ))
			{
				// 이것이 확실하므로 아이템을 지운다.
				
				// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[CastleSiege] [%s][%s] Deleting Gem of Defend (GUILD:%s) - Serial:%u",
#else
				LogAddTD("[CastleSiege] [%s][%s] Deleting Gem of Defend (GUILD:%s) - Serial:%d",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					gObj[iIndex].AccountID,
					gObj[iIndex].Name,
					gObj[iIndex].GuildName,
					gObj[iIndex].pInventory[x].m_Number
					);

				gObjInventoryItemSet(iIndex, x, 0xFF);
				gObjInventoryDeleteItem(iIndex, x);
				GCInventoryItemDeleteSend(iIndex, x, 1);

				iGemOfDefendDeletCount++;
				if (iGemOfDefendDeletCount >= iNeedGemOfDefend)
					break;
			}
		}
	}

	return TRUE;
}


VOID CCastleSiege::SetTaxRate (													// 세율을 조정하고 다른 게임서버와 동기화 한다.
	INT iTaxType,
	INT iTaxRate
	)
{
	INT iMaxTaxRate		= 0;
	switch (iTaxType) {
	case CS_TAXRATE_TYPE_CHAOS :
		{
			iMaxTaxRate = MAX_CS_TAX_RATE_CHAOS;
		}
		break;
	case CS_TAXRATE_TYPE_STORE :
		{
			iMaxTaxRate = MAX_CS_TAX_RATE_STORE;
		}
		break;
	case CS_TAXRATE_TYPE_HUNTZONE :
		{
			iMaxTaxRate = MAX_CS_TAX_HUNT_ZONE;
		}
		break;
	}

	if (iTaxRate < 0 || iTaxRate > iMaxTaxRate)
		return;

	switch(iTaxType) {
	case CS_TAXRATE_TYPE_CHAOS :
		{
			m_iTaxRateChaos = iTaxRate;
		}
		break;
	case CS_TAXRATE_TYPE_STORE :
		{
			m_iTaxRateStore = iTaxRate;
		}
		break;
	case CS_TAXRATE_TYPE_HUNTZONE :
		{
			m_iTaxHuntZone	= iTaxRate;
		}
		break;
	}

	// 여기서 해당 세율을 각 게임서버와 동기화 한다. -> 매 N초마다 자동으로 동기화 된다.
	LogAddTD("[CastleSiege] Tax Rate Setted - Type:%d, Rate:%d",
		iTaxType,
		iTaxRate
		);
}


BOOL CCastleSiege::CheckCastleHasMoney		(									// 현재 성에 돈이 그정도 있는지 확인한다.
	INT64 i64CastleMoney
	)
{
	if (m_i64CastleMoney >= i64CastleMoney)
		return TRUE;

	return FALSE;
}


VOID CCastleSiege::SetCastleMoney			(									// 현재 성의 돈을 특정 액수로 맞춘다.
	INT64 i64CastleMoney
	)
{
	m_i64CastleMoney		= i64CastleMoney;
}

	
VOID CCastleSiege::ResetCastleTaxInfo		()									// 성의 돈과 세율정보를 초기화 한다.
{
	m_i64CastleMoney		= 0;
	m_iTaxRateChaos			= 0;
	m_iTaxRateStore			= 0;
	m_iTaxHuntZone			= 0;
}


VOID CCastleSiege::SendCastleStateSync		()									// 현재 성의 상태를 맵서버군 전체에 동기화 한다.
{
	GS_GDReqCastleStateSync (m_iMapSvrGroup, m_iCastleSiegeState, m_iTaxRateChaos, m_iTaxRateStore, m_iTaxHuntZone, m_szCastleOwnerGuild);
}


BOOL CCastleSiege::CheckGuildOwnCastle		(LPSTR lpszGuildName)
{
	if (lpszGuildName == NULL)
		return FALSE;

	if (!strcmp(m_szCastleOwnerGuild, ""))
		return FALSE;

	if (!strcmp(m_szCastleOwnerGuild, lpszGuildName))
		return TRUE;

	return FALSE;
}


VOID CCastleSiege::ResetCastleCycle			()									// 공성 주기를 다시 시작하면서 필요한 데이터들을 리셋한다.
{
	m_bCsBasicGuildInfoLoadOK = FALSE;
	m_bCsTotalGuildInfoLoadOK = FALSE;
	EnterCriticalSection(& m_critCsBasicGuildInfo);
	m_mapCsBasicGuildInfo.clear();
	LeaveCriticalSection(& m_critCsBasicGuildInfo);
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	m_mapCsTotalGuildInfo.clear();
	LeaveCriticalSection(& m_critCsTotalGuildInfo);
	
	m_bCastleTowerAccessable	= FALSE;
	m_bRegCrownAvailable		= FALSE;
	m_btIsSiegeGuildList		= FALSE;
	m_btIsSiegeEnded			= FALSE;
}


VOID CCastleSiege::SetGateBlockState		(									// 성문의 블록 속성을 변경한다.
		INT iX, 
		INT iY, 
		INT iGateState
		)
{
	if (!CHECK_LIMIT(iX, MAX_TERRAIN_SIZE) ||
		!CHECK_LIMIT(iY, MAX_TERRAIN_SIZE)
		)
	{
		return;
	}

	switch(iGateState) {
	case CS_GATE_STATE_CLOSED :
		{
			for (INT i = iX-2 ; i <= iX+3 ; i++) {
				for (INT j = iY ; j <= iY+2 ; j++) {
					MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= ((BYTE) MAP_ATTR_WATER);
				}
			}
		}
		break;
	case CS_GATE_STATE_OPENED :
		{
			for (INT i = iX-2 ; i <= iX+3 ; i++) {
				for (INT j = iY ; j <= iY+2 ; j++) {
					MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_WATER);
				}
			}
		}
		break;
	}
}


VOID CCastleSiege::CheckBuildCsGuildInfo	()									// 공성관련 전체길드 정보를 구축하기 위해 상황1 과 2 중 선택한다.
{
	m_bCsBasicGuildInfoLoadOK = FALSE;
	m_bCsTotalGuildInfoLoadOK = FALSE;
	EnterCriticalSection(& m_critCsBasicGuildInfo);
	m_mapCsBasicGuildInfo.clear();
	LeaveCriticalSection(& m_critCsBasicGuildInfo);
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	m_mapCsTotalGuildInfo.clear();
	LeaveCriticalSection(& m_critCsTotalGuildInfo);

	if (m_btIsSiegeGuildList == TRUE) {
		// [0x88] 공/수 길드의 최종정보를 불러오기 요청
		GS_GDReqCsLoadTotalGuildInfo(m_iMapSvrGroup);
	}
	else {
		// [0x85] 공성측 길드선정 후보자료 요청
		GS_GDReqCalcRegGuildList(m_iMapSvrGroup);
	}
}


VOID CCastleSiege::SetCalcRegGuildList		(									// 공성측 최종선정 후보 데이터를 세팅한다. (이전 데이터 무조건 초기화)
		LPCSP_CALCREGGUILDLIST lpMsg,
		INT iCOUNT
		)
{
	if (lpMsg == NULL)
		return;

	if (iCOUNT > MAX_CS_REG_GUILD) {
		iCOUNT = MAX_CS_REG_GUILD;
	}

	if (iCOUNT <= 0) {
		// 공성측 요청 길드가 없다. -> 공성전이 열리지 않을 경우로 길드 데이터가 전혀 구축되지 않는다.
		return;
	}

	m_bCsBasicGuildInfoLoadOK				= FALSE;
	m_bCsTotalGuildInfoLoadOK				= FALSE;

	vector <CS_REG_GUILD_DATA>	vtRegGuildScore;

	INT iGCNT;

	for (iGCNT = 0 ; iGCNT < iCOUNT ; iGCNT++) {
		CS_REG_GUILD_DATA	pRegGuildData;

		CHAR szGuildName[16] = {0,};
		memcpy (szGuildName, lpMsg[iGCNT].szGuildName, 8);

		pRegGuildData.m_bIN_USE				= TRUE;
		pRegGuildData.m_strGuildName		= szGuildName;
		pRegGuildData.m_iRegMarkCount		= lpMsg[iGCNT].iRegMarkCount;
		pRegGuildData.m_iGuildMasterLevel	= lpMsg[iGCNT].iGuildMasterLevel;
		pRegGuildData.m_iGuildMemberCount	= lpMsg[iGCNT].iGuildMemberCount;
		pRegGuildData.m_iSeqNum				= CS_SIEGEGUILD_SEQ_PRIORITY - lpMsg[iGCNT].iSeqNum;
		INT iTotalScore						= ((lpMsg[iGCNT].iRegMarkCount * 5) + lpMsg[iGCNT].iGuildMemberCount) + lpMsg[iGCNT].iGuildMasterLevel/4;
		pRegGuildData.m_i64TotolScore		= MAKELONGLONG(lpMsg[iGCNT].iSeqNum, iTotalScore);
		vtRegGuildScore.push_back(pRegGuildData);
		
#ifdef EXTEND_LOG_SYSTEM_20060202
		LogAdd("[CastleSiege][REG Mark Count] - [%s][%d]",
			szGuildName, lpMsg[iGCNT].iRegMarkCount );
#endif
	}

	if (vtRegGuildScore.empty()) {
		// 공성측 요청 길드가 없다 -> 사실 위에서 체크했다면 거의 여기로는 않올것이다.
		return;
	}

	// 최고 득점길드부터 순서대로 되도록 정렬한다.
	sort(vtRegGuildScore.begin(), vtRegGuildScore.end(), RegGuildScoreCompFunc);

	// 상위 N개만 추출하여 저장한다.
	INT iGUILD_COUNT	= vtRegGuildScore.size();
	if (iGUILD_COUNT > MAX_CS_ATTACK_GUILD) {
		iGUILD_COUNT = MAX_CS_ATTACK_GUILD;
	}
	
	EnterCriticalSection(& m_critCsBasicGuildInfo);
	m_mapCsBasicGuildInfo.clear();
	for (iGCNT = 0 ; iGCNT < iGUILD_COUNT ; iGCNT++) {
		// 이 데이터는 뒤에 연합을 포함한 전체 데이터를 구축하는 기본이 된다.
		CS_TOTAL_GUILD_DATA pTotalGuildData;
		pTotalGuildData.m_bIN_USE			= TRUE;
		pTotalGuildData.m_iCsGuildID		= CS_SIEGESIDE_ATTACK + iGCNT;
		pTotalGuildData.m_bGuildInvolved	= 1;
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
		// 길드 점수를 표시한다.
		pTotalGuildData.m_iGuildScore		= ((vtRegGuildScore[iGCNT].m_iRegMarkCount * 5) + vtRegGuildScore[iGCNT].m_iGuildMemberCount) + vtRegGuildScore[iGCNT].m_iGuildMasterLevel/4;
#endif
		m_mapCsBasicGuildInfo.insert(pair<string, CS_TOTAL_GUILD_DATA> (vtRegGuildScore[iGCNT].m_strGuildName, pTotalGuildData));
	}
	if (m_btIsCastleOccupied == TRUE && strcmp(m_szCastleOwnerGuild,"")) {
		CS_TOTAL_GUILD_DATA pTotalGuildData;
		pTotalGuildData.m_bIN_USE			= TRUE;
		pTotalGuildData.m_iCsGuildID		= CS_SIEGESIDE_DEFEND;
		pTotalGuildData.m_bGuildInvolved	= 1;
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
		pTotalGuildData.m_iGuildScore		= 0;	// 수성측은 점수를 표시하지 않는다.
#endif		
		m_mapCsBasicGuildInfo.insert(pair<string, CS_TOTAL_GUILD_DATA> (m_szCastleOwnerGuild, pTotalGuildData));
	}
	LeaveCriticalSection(& m_critCsBasicGuildInfo);

	// 위에서 구축한 기본 데이터를 전체 데이터에 우선 넣어놓고 요청 패킷을 함께 작성한다.
	CHAR cBUFFER[sizeof(CSP_REQ_CSGUILDUNIONINFO) + sizeof(CSP_CSGUILDUNIONINFO) * (MAX_CS_TOTAL_GUILD + 1) + 256];
	LPCSP_REQ_CSGUILDUNIONINFO lpMsgSend	= (LPCSP_REQ_CSGUILDUNIONINFO) cBUFFER;
	LPCSP_CSGUILDUNIONINFO lpMsgSendBody	= (LPCSP_CSGUILDUNIONINFO) (cBUFFER + sizeof(CSP_REQ_CSGUILDUNIONINFO));

	lpMsgSend->wMapSvrNum					= m_iMapSvrGroup;
	lpMsgSend->iCount						= 0;

	EnterCriticalSection(& m_critCsTotalGuildInfo);
	m_mapCsTotalGuildInfo.clear();
	map <string, CS_TOTAL_GUILD_DATA>::iterator it		= m_mapCsBasicGuildInfo.begin();
	for(;it != m_mapCsBasicGuildInfo.end() ; it++) {
		if (lpMsgSend->iCount > MAX_CS_TOTAL_GUILD) {
			// 에러 - 이상현상
			break;
		}
		
		m_mapCsTotalGuildInfo.insert(pair<string, CS_TOTAL_GUILD_DATA>(it->first, it->second));
		string strGuildName								= it->first;
		memcpy(lpMsgSendBody[lpMsgSend->iCount].szGuildName, &strGuildName[0], 8);
		lpMsgSendBody[lpMsgSend->iCount].iCsGuildID		= (it->second).m_iCsGuildID;
		lpMsgSend->iCount++;
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);

	// 공/수 길드의 연합정보를 요청한다.
	m_bCsBasicGuildInfoLoadOK = TRUE;

	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x86, sizeof(CSP_REQ_CSGUILDUNIONINFO) + sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount);
	cDBSMng.Send((char*)lpMsgSend, sizeof(CSP_REQ_CSGUILDUNIONINFO) + sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount);
}


bool CCastleSiege::RegGuildScoreCompFunc	(CS_REG_GUILD_DATA & A, CS_REG_GUILD_DATA & B)
{
	// 내림차순으로 정렬
	if (A.m_i64TotolScore < B.m_i64TotolScore)
		return false;
	return true;
}


VOID CCastleSiege::MakeCsTotalGuildInfo		(							// 공/수의 연합길드 정보를 받아서 기존 길드정보와 함께 완벽한 길드정보를 구축한다.
	LPCSP_CSGUILDUNIONINFO lpMsg,
	INT iCOUNT
	)
{
	if (lpMsg == NULL)
		return;

	if (m_bCsBasicGuildInfoLoadOK == FALSE) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::MakeCsTotalGuildInfo() ERROR - m_bCsBasicGuildInfoLoadOK == FALSE");
		return;
	}

	if (iCOUNT <= 0) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::MakeCsTotalGuildInfo() ERROR - iCOUNT <= 0 : %d",
			iCOUNT
			);
		return;
	}
	
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	for(INT iGCNT = 0 ; iGCNT < iCOUNT ; iGCNT++) {
		CHAR szGuildName[16] = {0,};
		memcpy (szGuildName, lpMsg[iGCNT].szGuildName, 8);

		map<string, CS_TOTAL_GUILD_DATA>::iterator it = m_mapCsTotalGuildInfo.find(szGuildName);
		if (it != m_mapCsTotalGuildInfo.end()) {
			// 최초에 입력된 공성 당사자 길드 정보나 바로 전에 저장된 정보들은 다시 저장하지 않는다.
			continue;
		}

		CS_TOTAL_GUILD_DATA	pTotalGuildData;
		pTotalGuildData.m_bIN_USE			= TRUE;
		pTotalGuildData.m_iCsGuildID		= lpMsg[iGCNT].iCsGuildID;
		pTotalGuildData.m_bGuildInvolved	= 0;

		m_mapCsTotalGuildInfo.insert(pair<string, CS_TOTAL_GUILD_DATA>(szGuildName, pTotalGuildData));
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);
	
	m_bCsTotalGuildInfoLoadOK = TRUE;

	// 여기서 현재의 최종길드 정보를 DB에 저장한다.
	SaveCsTotalGuildInfo ();
	// 여기서 공성서버의 전체 사용자들의 공/수 구분정보를 다시 세팅해야 한다.
	SetAllUserCsJoinSide ();
}


VOID CCastleSiege::SaveCsTotalGuildInfo		()							// 공/수의 전체길드 정보를 DB에 저장하도록 요청한다.
{
	if (m_bCsTotalGuildInfoLoadOK == FALSE) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::SaveCsTotalGuildInfo() ERROR - m_bCsTotalGuildInfoLoadOK == FALSE");
		return;
	}
	
	CHAR cBUFFER[sizeof(CSP_REQ_CSSAVETOTALGUILDINFO) + sizeof(CSP_CSSAVETOTALGUILDINFO) * (MAX_CS_TOTAL_GUILD + 1) + 256];
	LPCSP_REQ_CSSAVETOTALGUILDINFO lpMsgSend	= (LPCSP_REQ_CSSAVETOTALGUILDINFO) cBUFFER;
	LPCSP_CSSAVETOTALGUILDINFO lpMsgSendBody	= (LPCSP_CSSAVETOTALGUILDINFO) (cBUFFER + sizeof(CSP_REQ_CSSAVETOTALGUILDINFO));

	lpMsgSend->wMapSvrNum						= m_iMapSvrGroup;
	lpMsgSend->iCount							= 0;

	EnterCriticalSection(& m_critCsTotalGuildInfo);
	map <string, CS_TOTAL_GUILD_DATA>::iterator it		= m_mapCsTotalGuildInfo.begin();
	for(;it != m_mapCsTotalGuildInfo.end() ; it++) {
		if (lpMsgSend->iCount > MAX_CS_TOTAL_GUILD) {
			// 에러 - 이상현상
			break;
		}

		string strGuildName								= it->first;
		memcpy(lpMsgSendBody[lpMsgSend->iCount].szGuildName, &strGuildName[0], 8);
		lpMsgSendBody[lpMsgSend->iCount].iCsGuildID		= (it->second).m_iCsGuildID;
		lpMsgSendBody[lpMsgSend->iCount].iGuildInvolved	= (it->second).m_bGuildInvolved;
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
		lpMsgSendBody[lpMsgSend->iCount].iGuildScore	= (it->second).m_iGuildScore;
#endif
		lpMsgSend->iCount++;
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);

	// 공/수 길드의 전체정보를 저장한다.
	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x87, sizeof(CSP_REQ_CSSAVETOTALGUILDINFO) + sizeof(CSP_CSSAVETOTALGUILDINFO) * lpMsgSend->iCount);
	cDBSMng.Send((char*)lpMsgSend, sizeof(CSP_REQ_CSSAVETOTALGUILDINFO) + sizeof(CSP_CSSAVETOTALGUILDINFO) * lpMsgSend->iCount);


	//-----------<확인용테스트>-----------
/*
	OutputDebugString("------<CS GUILD LIST SAVE>------\n");
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	map <string, CS_TOTAL_GUILD_DATA>::iterator it2		= m_mapCsTotalGuildInfo.begin();
	INT iSize = m_mapCsTotalGuildInfo.size();
	for(;it2 != m_mapCsTotalGuildInfo.end() ; it2++) {
		string strGuildName			= it2->first;
		CS_TOTAL_GUILD_DATA stTEMP	= it2->second;

		CHAR szTEMP[256]	= {0,};
		wsprintf(szTEMP, "GNAME:%s, GGUID:%d, INVOLVED:%d\n", 
			&strGuildName[0],
			stTEMP.m_iCsGuildID,
			stTEMP.m_bGuildInvolved
			);
		OutputDebugString(szTEMP);
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);
*/
	
	// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	map <string, CS_TOTAL_GUILD_DATA>::iterator it2		= m_mapCsTotalGuildInfo.begin();
	INT iSize = m_mapCsTotalGuildInfo.size();
	for(;it2 != m_mapCsTotalGuildInfo.end() ; it2++) {
		string strGuildName			= it2->first;
		CS_TOTAL_GUILD_DATA stTEMP	= it2->second;

#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
		LogAddTD("[CastleSiege] Siege Guild List Save - GNAME:%s, CSGUID:%d, INVOLVED:%d, SCORE:%d",
			&strGuildName[0],
			stTEMP.m_iCsGuildID,
			stTEMP.m_bGuildInvolved,
			stTEMP.m_iGuildScore
			);
#else		
		LogAddTD("[CastleSiege] Siege Guild List Save - GNAME:%s, CSGUID:%d, INVOLVED:%d",
			&strGuildName[0],
			stTEMP.m_iCsGuildID,
			stTEMP.m_bGuildInvolved
			);
#endif
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);
}


VOID CCastleSiege::SetCsTotalGuildInfo		(							// 공/수의 연합길드 정보를 받아서 기존 길드정보와 함께 완벽한 길드정보를 구축한다.
	LPCSP_CSLOADTOTALGUILDINFO lpMsg,
	INT iCOUNT
	)
{
	if (lpMsg == NULL)
		return;

	if (iCOUNT <= 0) {
		LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::SetCsTotalGuildInfo() ERROR - iCOUNT <= 0 : %d",
			iCOUNT
			);
		return;
	}

	for(INT iGCNT = 0 ; iGCNT < iCOUNT ; iGCNT++) {
		CHAR szGuildName[16] = {0,};
		memcpy (szGuildName, lpMsg[iGCNT].szGuildName, 8);
		CS_TOTAL_GUILD_DATA	pTotalGuildData;
		pTotalGuildData.m_bIN_USE			= TRUE;
		pTotalGuildData.m_iCsGuildID		= lpMsg[iGCNT].iCsGuildID;
		pTotalGuildData.m_bGuildInvolved	= lpMsg[iGCNT].iGuildInvolved;
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
		pTotalGuildData.m_iGuildScore		= lpMsg[iGCNT].iGuildScore;
#endif

		EnterCriticalSection(& m_critCsTotalGuildInfo);
		m_mapCsTotalGuildInfo.insert(pair<string, CS_TOTAL_GUILD_DATA>(szGuildName, pTotalGuildData));
		LeaveCriticalSection(& m_critCsTotalGuildInfo);

		if (lpMsg[iGCNT].iGuildInvolved == 1) {
			EnterCriticalSection(& m_critCsBasicGuildInfo);
			m_mapCsBasicGuildInfo.insert(pair<string, CS_TOTAL_GUILD_DATA>(szGuildName, pTotalGuildData));
			LeaveCriticalSection(& m_critCsBasicGuildInfo);
		}
	}
	
	m_bCsBasicGuildInfoLoadOK = TRUE;
	m_bCsTotalGuildInfoLoadOK = TRUE;

	// 여기서 공성서버의 전체 사용자들의 공/수 구분정보를 다시 세팅해야 한다.
	SetAllUserCsJoinSide ();

	//-----------<확인용테스트>-----------
/*
	OutputDebugString("------<CS GUILD LIST LOAD>------\n");
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	map <string, CS_TOTAL_GUILD_DATA>::iterator it2		= m_mapCsTotalGuildInfo.begin();
	INT iSize = m_mapCsTotalGuildInfo.size();
	for(;it2 != m_mapCsTotalGuildInfo.end() ; it2++) {
		string strGuildName			= it2->first;
		CS_TOTAL_GUILD_DATA stTEMP	= it2->second;

		CHAR szTEMP[256]	= {0,};
		wsprintf(szTEMP, "GNAME:%s, GGUID:%d, INVOLVED:%d\n", 
			&strGuildName[0],
			stTEMP.m_iCsGuildID,
			stTEMP.m_bGuildInvolved
			);
		OutputDebugString(szTEMP);
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);
*/

	// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	map <string, CS_TOTAL_GUILD_DATA>::iterator it2		= m_mapCsTotalGuildInfo.begin();
	INT iSize = m_mapCsTotalGuildInfo.size();
	for(;it2 != m_mapCsTotalGuildInfo.end() ; it2++) {
		string strGuildName			= it2->first;
		CS_TOTAL_GUILD_DATA stTEMP	= it2->second;

#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
		LogAddTD("[CastleSiege] Siege Guild List Load - GNAME:%s, CSGUID:%d, INVOLVED:%d, SCORE:%d",
			&strGuildName[0],
			stTEMP.m_iCsGuildID,
			stTEMP.m_bGuildInvolved,
			stTEMP.m_iGuildScore
			);
#else
		LogAddTD("[CastleSiege] Siege Guild List Load - GNAME:%s, CSGUID:%d, INVOLVED:%d",
			&strGuildName[0],
			stTEMP.m_iCsGuildID,
			stTEMP.m_bGuildInvolved
			);
#endif
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);
}


BOOL CCastleSiege::GetCsJoinSide				(
	LPSTR lpszGuildName,
	BYTE &btCsJoinSide,
	bool &bCsGuildInvolved
	)
{
	btCsJoinSide		= CS_SIEGESIDE_NONE;
	bCsGuildInvolved	= 0;
	
	if (lpszGuildName == NULL) {
		return FALSE;
	}

	if (m_bDoRun == FALSE) {
		return FALSE;
	}
	
	if (m_iCastleSiegeState < CASTLESIEGE_STATE_NOTIFY || m_iCastleSiegeState > CASTLESIEGE_STATE_STARTSIEGE) {
		// 자신의 공성상태를 알 필요가 없을 때
		return FALSE;
	}

	if (m_bCsTotalGuildInfoLoadOK == FALSE) {
		return FALSE;
	}

	BOOL bRETVAL		= FALSE;

	// 공성전이 시작되지 않았다면 무효
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	map<string, CS_TOTAL_GUILD_DATA>::iterator it	= m_mapCsTotalGuildInfo.find(lpszGuildName);
	if (it != m_mapCsTotalGuildInfo.end()) {
		btCsJoinSide		= (it->second).m_iCsGuildID;
		bCsGuildInvolved	= (it->second).m_bGuildInvolved;
		bRETVAL			= TRUE;
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);

	return bRETVAL;
}


VOID CCastleSiege::SetAllUserCsJoinSide		()
{
	if (m_bCsTotalGuildInfoLoadOK == FALSE)
		return;
	
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	for (INT iUSER = ALLOC_USEROBJECTSTART; iUSER<MAX_OBJECT; iUSER++) {
		if (!gObjIsConnected(iUSER))
			continue;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
//		gObjClearBuffEffect( &gObj[iUSER], CLEAR_TYPE_CASTLESIEGE );
#else
		gObj[iUSER].m_ViewSkillState			&= ~(STATE_JOIN_SIDE_DEFEND | STATE_JOIN_SIDE_ATTACK);
		
	#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
		DEL_VIEWSKILLSTATE( gObj[iUSER].m_ViewSkillState, STATE_JOIN_SIDE_ATTACK_2);
		DEL_VIEWSKILLSTATE( gObj[iUSER].m_ViewSkillState, STATE_JOIN_SIDE_ATTACK_3);
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

		CHAR szGuildName[16]					= {0,};
		memcpy(szGuildName, gObj[iUSER].GuildName, 8);
		map<string, CS_TOTAL_GUILD_DATA>::iterator it	= m_mapCsTotalGuildInfo.find(szGuildName);
		if (it != m_mapCsTotalGuildInfo.end()) {
			gObj[iUSER].m_btCsJoinSide			= (it->second).m_iCsGuildID;
			gObj[iUSER].m_bCsGuildInvolved		= (it->second).m_bGuildInvolved;
		}
		NotifySelfCsJoinSide (iUSER);

#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0;
#endif

	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);
}


VOID CCastleSiege::ResetAllUserCsJoinSide	()				// 접속중인 모든 사용자 들의 현재의 공/수 참여 여부값을 해제해준다.
{
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	for (INT iUSER = ALLOC_USEROBJECTSTART; iUSER<MAX_OBJECT; iUSER++) {
		if (!gObjIsConnected(iUSER))
			continue;
		gObj[iUSER].m_btCsJoinSide				= CS_SIEGESIDE_NONE;
		gObj[iUSER].m_bCsGuildInvolved			= 0;
		gObj[iUSER].m_btLifeStoneCount			= 0;		// 라이프스톤 사용횟수 초기화 (apple)
		NotifySelfCsJoinSide (iUSER);
		
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0;
#endif
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);
}


VOID CCastleSiege::NotifySelfCsJoinSide		(INT iIndex)	// 특정 사용자의 공/수 참여 여부값을 보내준다.
{
	if (!gObjIsConnected(iIndex))
		return;

	INT iState									= 0;
	INT iViewState								= 0;
	BYTE btCsJoinSide							= gObj[iIndex].m_btCsJoinSide;
	if (btCsJoinSide < CS_SIEGESIDE_NONE)		btCsJoinSide	= CS_SIEGESIDE_NONE;
#ifdef MODIFY_CASTLESIEGE_MARK_DISPLAY_20071216
	if( btCsJoinSide > CS_SIEGESIDE_ATTACK_3 )
	{
		btCsJoinSide = CS_SIEGESIDE_NONE;
	}
#else
	if (btCsJoinSide > CS_SIEGESIDE_ATTACK)		btCsJoinSide	= CS_SIEGESIDE_ATTACK;
#endif // MODIFY_CASTLESIEGE_MARK_DISPLAY_20071216

	switch(btCsJoinSide)
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	case CS_SIEGESIDE_DEFEND:
		{
			iState		= 1;
			iViewState	= STATE_JOIN_SIDE_DEFEND;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
			if( m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE || m_iCastleSiegeState == CASTLESIEGE_STATE_ENDSIEGE )
			{
				gObjAddBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_DEFENSE_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
			}
			else
			{
				gObjRemoveBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_DEFENSE_MARK );
			}
#else
			gObjAddBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_DEFENSE_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		}
		break;
	case CS_SIEGESIDE_ATTACK:
		{
			iState		= 1;
			iViewState	= STATE_JOIN_SIDE_ATTACK;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
			if( m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE || m_iCastleSiegeState == CASTLESIEGE_STATE_ENDSIEGE )
			{
				gObjAddBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_ATTACK_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
			}
			else
			{
				gObjRemoveBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_ATTACK_MARK );
			}
#else
			gObjAddBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_ATTACK_MARK, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		}
		break;
	case CS_SIEGESIDE_ATTACK_2:
		{
			iState		= 1;
			iViewState	= STATE_JOIN_SIDE_ATTACK_2;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
			if( m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE || m_iCastleSiegeState == CASTLESIEGE_STATE_ENDSIEGE )
			{
				gObjAddBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_ATTACK_MARK_2, 0, 0, 0, 0, BUFFTIME_INFINITE );
			}
			else
			{
				gObjRemoveBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_ATTACK_MARK_2 );
			}
#else
			gObjAddBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_ATTACK_MARK_2, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		}
		break;
	case CS_SIEGESIDE_ATTACK_3:
		{
			iState		= 1;
			iViewState	= STATE_JOIN_SIDE_ATTACK_3;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
			if( m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE || m_iCastleSiegeState == CASTLESIEGE_STATE_ENDSIEGE )
			{
				gObjAddBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_ATTACK_MARK_3, 0, 0, 0, 0, BUFFTIME_INFINITE );
			}
			else
			{
				gObjRemoveBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_ATTACK_MARK_3 );
			}
#else
			gObjAddBuffEffect( &gObj[iIndex], BUFFTYPE_CASTLE_ATTACK_MARK_3, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		}
		break;
	case CS_SIEGESIDE_NONE:
		{
			iState		= 0;
			iViewState	= STATE_JOIN_SIDE_DEFEND | STATE_JOIN_SIDE_ATTACK;
			gObjClearBuffEffect( &gObj[iIndex], CLEAR_TYPE_CASTLESIEGE );
		}
		break;
#else
	case CS_SIEGESIDE_DEFEND :
		{
			iState		= 1;
			iViewState	= STATE_JOIN_SIDE_DEFEND;
		}
		break;
	case CS_SIEGESIDE_ATTACK :
		{
			iState		= 1;
			iViewState	= STATE_JOIN_SIDE_ATTACK;
		}
		break;
	case CS_SIEGESIDE_NONE :
	default :
		{
			iState		= 0;
			iViewState	= STATE_JOIN_SIDE_DEFEND | STATE_JOIN_SIDE_ATTACK;
		}
		break;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}

	PMSG_ANS_NOTIFYCSJOINSIDE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x19, sizeof(PMSG_ANS_NOTIFYCSJOINSIDE));

#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
	// 공성 길드 1, 2, 3을 구분하기 위해 아래와 같이 처리한다.
	if( btCsJoinSide == CS_SIEGESIDE_ATTACK )
		pMsg.btCsJoinSide	= gObj[iIndex].m_btCsJoinSide;
	else
#endif
	pMsg.btCsJoinSide	= btCsJoinSide;

	
	DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


BOOL CCastleSiege::OperateGate				(				// 외부 사용자객체 인덱스로 성문을 작동시킨다.
	INT		iObjIndex,
	INT		iMonsterExistVal,
	BOOL	bOpenType										// (0:닫기/1:열기)
	)
{
	if (!gObjIsConnected(iObjIndex)) {
		return FALSE;
	}

	BOOL bRETVAL	= FALSE;

	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == 277) &&
			(pNpcData.m_iNPC_OBJINDEX == iObjIndex)
			)
		{
			if ((pNpcData.m_bIN_USE	== TRUE) &&
				(pNpcData.m_iNPC_LIVE == NPC_LIVE_OK)
				)
			{
				// OBJECTSTRUCT 구조체의 iIndex 의 값을 찾아서 실제 존재하는지 확인한다.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
//					(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) 
					(iMonsterExistVal == pNpcData.m_iCsNpcExistVal)
					)
				{
					// 이것까지 확인하면 성문이 맞다.

					// 성문의 속성, 블럭상태 등을 바꾸어준다.
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, bOpenType);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[iNPC_INDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObj[iNPC_INDEX].m_btCsGateOpen	= bOpenType;
					
					// 성문의 상태를 주변에 알리는 작업은 외부에서 한다.
					bRETVAL	= TRUE;
				}
				break;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	return bRETVAL;
}


BOOL CCastleSiege::OperateGate				(				// 내부 NPC관리 인덱스로 성문을 작동시킨다.
	INT		iNpcIndex,
	BOOL	bOpenType										// (0:닫기/1:열기)
	)
{
	BOOL bRETVAL	= FALSE;

	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM	== 277) &&
			(pNpcData.m_iNPC_INDEX	== iNpcIndex)
			)
		{
			if ((pNpcData.m_bIN_USE	== TRUE) &&
				(pNpcData.m_iNPC_LIVE == NPC_LIVE_OK)
				)
			{
				// OBJECTSTRUCT 구조체의 iIndex 의 값을 찾아서 실제 존재하는지 확인한다.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// 이것까지 확인하면 성문이 맞다.

						// 성문의 속성, 블럭상태 등을 바꾸어준다.
						SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, bOpenType);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
						if( bOpenType == 0 )
						{
							gObjRemoveBuffEffect( &gObj[iNPC_INDEX], BUFFTYPE_CASTLE_DOOR_STATE );
						}
						else
						{
							gObjAddBuffEffect( &gObj[iNPC_INDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
						}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
						gObj[iNPC_INDEX].m_btCsGateOpen	= bOpenType;

						// 성문의 상태를 주변에 알리는 작업은 외부에서 한다.
						bRETVAL	= TRUE;
					}
				}
				break;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	return bRETVAL;
}


BOOL CCastleSiege::CheckAttackGuildExist		()							// 공성측 길드가 존재하는지 확인한다.
{
	BOOL iRETVAL	= FALSE;

	EnterCriticalSection(& m_critCsTotalGuildInfo);
	if (!m_mapCsTotalGuildInfo.empty())
		iRETVAL		= TRUE;
	LeaveCriticalSection(& m_critCsTotalGuildInfo);

	return iRETVAL;
}


BOOL CCastleSiege::CheckCastleOwnerMember		(INT iIndex)				// 해당 사용자가 성을 차지한 길드의 소속인지 확인한다.
{
	if (!gObjIsConnected(iIndex))
		return FALSE;

	if (!strcmp(m_szCastleOwnerGuild, ""))
		return FALSE;
	
	if (!strcmp(gObj[iIndex].GuildName, ""))
		return FALSE;

	if (strcmp(gObj[iIndex].GuildName, m_szCastleOwnerGuild))
		return FALSE;

	return TRUE;
}


BOOL CCastleSiege::CheckCastleOwnerUnionMember	(INT iIndex)				// 특정 사용자가 자신이 속한 길드연합의 일원인지 확인한다.
{
	if (!gObjIsConnected(iIndex))
		return FALSE;

	if (!strcmp(m_szCastleOwnerGuild, ""))
		return FALSE;

	if (!strcmp(gObj[iIndex].GuildName, ""))
		return FALSE;

	LPGUILD_INFO lpGuildInfo		= gObj[iIndex].lpGuild;
	if (lpGuildInfo == NULL)
		return FALSE;

#ifdef ADD_NEW_UNION_01_20041006
	TUnionInfo* pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);
	if( pUnionInfo == NULL) {
		return FALSE;
	}
	else {
		if (!strcmp (pUnionInfo->m_szMasterGuild, m_szCastleOwnerGuild)) {
			// 내가 연합의 일원이다.
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
#endif

	return FALSE;
}


BOOL CCastleSiege::CheckGuardianStatueExist		()							// 수호석상이 존재하는지 확인한다.
{
	for (INT iIDX = 0 ; iIDX < MAX_MONSTER ; iIDX++) {
		if(gObj[iIDX].Class == 283) {
			if (gObj[iIDX].Connected > 2) {
				if ((gObj[iIDX].Live == 1) && (gObj[iIDX].MapNumber == MAP_INDEX_CASTLESIEGE)) {
					// 수호석상이 존재하면 참
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


VOID CCastleSiege::ChangeWinnerGuild			(INT iCsJoinSide)			// 수성측 길드를 바꾼다 -> 중간 승리 시 사용
{
	CHAR szGuildTo [16]		= {0,};
	CHAR szGuildFrom [16]	= {0,};

	EnterCriticalSection(& m_critCsTotalGuildInfo);
	// 1 . 공/수 길드의 소속값을 바꾼다.
	map <string, CS_TOTAL_GUILD_DATA>::iterator it		= m_mapCsTotalGuildInfo.begin();
	for(;it != m_mapCsTotalGuildInfo.end() ; it++) 
	{
		if ((it->second).m_iCsGuildID == iCsJoinSide) 
		{
			(it->second).m_iCsGuildID = CS_SIEGESIDE_DEFEND;
			if ((it->second).m_bGuildInvolved == TRUE) 
			{
				memcpy(szGuildTo, &(it->first)[0], MAX_GUILDNAMESTRING);
			}
		}
		else if ((it->second).m_iCsGuildID == CS_SIEGESIDE_DEFEND) 
		{
			(it->second).m_iCsGuildID = iCsJoinSide;
			if ((it->second).m_bGuildInvolved == TRUE) 
			{
				memcpy(szGuildFrom, &(it->first)[0], MAX_GUILDNAMESTRING);
			}
		}
	}

	// 2 . 전체 사용자들을 대상으로 바뀐 공/수 소속값을 적용한다.
	for (INT iUSER = ALLOC_USEROBJECTSTART; iUSER<MAX_OBJECT; iUSER++) 
	{
		if (!gObjIsConnected(iUSER))
			continue;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
//		gObjClearBuffEffect( &gObj[iUSER], CLEAR_TYPE_CASTLESIEGE );
#else
		gObj[iUSER].m_ViewSkillState		&= ~(STATE_JOIN_SIDE_DEFEND | STATE_JOIN_SIDE_ATTACK);

	#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
		DEL_VIEWSKILLSTATE( gObj[iUSER].m_ViewSkillState, STATE_JOIN_SIDE_ATTACK_2);
		DEL_VIEWSKILLSTATE( gObj[iUSER].m_ViewSkillState, STATE_JOIN_SIDE_ATTACK_3);
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

		CHAR szGuildName[16]				= {0,};
		memcpy(szGuildName, gObj[iUSER].GuildName, 8);
		map<string, CS_TOTAL_GUILD_DATA>::iterator it	= m_mapCsTotalGuildInfo.find(szGuildName);
		if (it != m_mapCsTotalGuildInfo.end()) 
		{
			gObj[iUSER].m_btCsJoinSide		= (it->second).m_iCsGuildID;
			gObj[iUSER].m_bCsGuildInvolved	= (it->second).m_bGuildInvolved;
		}
//		NotifySelfCsJoinSide (iUSER);		-> 외부의 NotifyAllUserCsProgState() 에서 변경됨


#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0;
#endif
		
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);

	LogAddTD("[CastleSiege] Castle Winner Guild Change From '%s' To '%s'",
		szGuildFrom,
		szGuildTo
		);
}


BOOL CCastleSiege::CheckMiddleWinnerGuild	()							// 판정!!> . 공성전 중간승리 길드가 나타났는지 확인한다.
{
	if (m_dwCrownAccessTime == 0)
		return FALSE;

	if (
		!gObjIsConnected(m_iCastleCrownAccessUser) ||
		!gObjIsConnected(m_iCastleSwitchAccessUser1) ||
		!gObjIsConnected(m_iCastleSwitchAccessUser2)
		)
	{
		return FALSE;
	}

	if (
		!strcmp(gObj[m_iCastleCrownAccessUser].GuildName, "") ||
		!strcmp(gObj[m_iCastleSwitchAccessUser1].GuildName, "") ||
		!strcmp(gObj[m_iCastleSwitchAccessUser2].GuildName, "")
		)
	{
		return FALSE;
	}

	if (
		gObj[m_iCastleCrownAccessUser].m_btCsJoinSide >= CS_SIEGESIDE_ATTACK &&
		gObj[m_iCastleSwitchAccessUser1].m_btCsJoinSide >= CS_SIEGESIDE_ATTACK &&
		gObj[m_iCastleSwitchAccessUser2].m_btCsJoinSide >= CS_SIEGESIDE_ATTACK
		)
	{
		if (
			gObj[m_iCastleCrownAccessUser].m_bCsGuildInvolved == true
//			&& gObj[m_iCastleSwitchAccessUser1].m_bCsGuildInvolved == true
//			&& gObj[m_iCastleSwitchAccessUser2].m_bCsGuildInvolved == true
			)
		{
			if (
				gObj[m_iCastleCrownAccessUser].m_btCsJoinSide == gObj[m_iCastleSwitchAccessUser1].m_btCsJoinSide &&
				gObj[m_iCastleCrownAccessUser].m_btCsJoinSide == gObj[m_iCastleSwitchAccessUser2].m_btCsJoinSide
				)
			{


#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
				if (GetTickCount() - m_dwCrownAccessTime 
					+ gObj[m_iCastleCrownAccessUser].m_iAccumulatedCrownAccessTime
					>= CS_CASTLE_CROWN_ACCESS_TIME									) 
#else
				if (GetTickCount() - m_dwCrownAccessTime >= CS_CASTLE_CROWN_ACCESS_TIME) 
#endif
				{
					// 이 길드가 성공 길드이다.
					LogAddTD("[CastleSiege] Castle Winner Has been Changed (GUILD:%s)", gObj[m_iCastleCrownAccessUser].GuildName);

					// 일단 메시지를 뿌려준다.
					
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
					// 직인 등록이 성공했음을 보내줘야 하는데 기존에 다른사람이 시도중(3)으로 되어 있어 수정함.
					GCAnsCsAccessCrownState (m_iCastleCrownAccessUser, 1);
#else
					GCAnsCsAccessCrownState (m_iCastleCrownAccessUser, 3);
#endif
					NotifyCrownState(2);

					// 여기서 성공 처리를 한다.
					memset(m_szMiddleWinnerGuild, 0, sizeof(m_szMiddleWinnerGuild));
					memcpy(m_szMiddleWinnerGuild, gObj[m_iCastleCrownAccessUser].GuildName, 8);

					// 중간에 삭제해야 할 NPC들을 정리해 준다. (라이프 스톤 등)
					ClearNonDbNPC_MidWin ();

					// 공/수 양측의 상태를 전환한다.
					ChangeWinnerGuild (gObj[m_iCastleCrownAccessUser].m_btCsJoinSide);

					// 공성맵 안의 사람들에게 왕의 직인이 등록 되었음을 알린다. -> 공/수 전환됨
					NotifyAllUserCsProgState (1, gObj[m_iCastleCrownAccessUser].GuildName);

					// NPC 접근 상태들을 초기화 한다.
					ResetCrownAccessTickCount();
					ResetCrownUserIndex();
					SetCrownAccessUserX(0);
					SetCrownAccessUserY(0);
					ResetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH1);
					ResetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH2);
					m_bRegCrownAvailable	= FALSE;

					// 공성측만 성밖으로 리스폰
					ReSpawnEnemyUser();
					
					// 현재 공성시간을 다시 보내주어야 한다 -> 클라이언트가 지우므로
					GCAnsCsLeftTimeAlarm (
						(m_iCS_REMAIN_MSEC/1000)/3600,			// 시간
						((m_iCS_REMAIN_MSEC/1000)%3600)/60		// 분
						);
					m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}


BOOL CCastleSiege::CheckCastleSiegeResult		()							// 판정!!> . 공성전 최종결과를 체크한다.
{
	CHAR szMsg [256]		= {0,};
	BOOL bRETVAL			= FALSE;

	if ((m_btIsCastleOccupied == 1) && strcmp(m_szCastleOwnerGuild, "")) 
	{
		// 기존의 성이 점령당한 상태였다.
		if (!strcmp(m_szMiddleWinnerGuild, "") ||
			!strcmp(m_szMiddleWinnerGuild, m_szCastleOwnerGuild))
		{
			// 기존의 성 길드가 다시 성의 주인이다.
			m_btIsCastleOccupied	= TRUE;
			wsprintf(szMsg, lMsg.Get(1733),		// "%s 길드가 성을 지키는 데 성공했습니다."
				m_szCastleOwnerGuild
				);

			bRETVAL					= FALSE;
		}
		else {
			// 성의 주인길드가 바뀌었다.
			m_btIsCastleOccupied	= TRUE;
			memset(m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
			memcpy(m_szCastleOwnerGuild, m_szMiddleWinnerGuild, 8);
			wsprintf(szMsg, lMsg.Get(1734),		// "%s 길드가 새로운 성의 주인이 되었습니다."
				m_szCastleOwnerGuild
				);

			bRETVAL					= TRUE;
		}
	}
	else {
		// 기존의 성에는 주인이 없었다.
		if (!strcmp(m_szMiddleWinnerGuild, ""))
		{
			// 여전히 성의 주인길드가 없다.
			m_btIsCastleOccupied	= FALSE;
			wsprintf(szMsg, lMsg.Get(1735));	// "성의 주인길드가 정해지지 않았습니다."

			bRETVAL					= FALSE;
		}
		else {
			// 성의 주인길드가 생겼다.
			m_btIsCastleOccupied	= TRUE;
			memset(m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
			memcpy(m_szCastleOwnerGuild, m_szMiddleWinnerGuild, 8);
			wsprintf(szMsg, lMsg.Get(1734),		// "%s 길드가 새로운 성의 주인이 되었습니다."
				m_szCastleOwnerGuild
				);

			bRETVAL					= TRUE;
		}
	}

	m_btIsSiegeEnded				= TRUE;

	LogAddTD("[CastleSiege] Castle Final Winner Guild : %s", !strcmp(m_szCastleOwnerGuild,"")?"NONE":m_szCastleOwnerGuild);

	// 전체 맵 서버군에 메시지를 보낸다.
	SendMapServerGroupMsg(szMsg);

	return bRETVAL;
}


BOOL CCastleSiege::CheckUnionGuildMaster		(INT iIndex)				// 특정 사용자가 자신이 속한 길드연합의 연합 마스터인지 확인한다.
{
	if (!gObjIsConnected(iIndex))
		return FALSE;

	LPGUILD_INFO lpGuildInfo		= gObj[iIndex].lpGuild;
	if (lpGuildInfo == NULL)
		return FALSE;

#ifdef ADD_NEW_GUILD_01_20040913
	if ((gObj[iIndex].GuildStatus == G_MASTER) &&
		((lpGuildInfo->Number == lpGuildInfo->iGuildUnion) ||
		 (lpGuildInfo->iGuildUnion == G_RELATIONSHIP_NONE))
		)
	{
		return TRUE;
	}
#endif

	return FALSE;
}


VOID CCastleSiege::SetCrownSwitchUserIndex		(
	INT iMonsterClass, 
	INT iIndex
	)
{
	switch(iMonsterClass) {
	case CS_NPC_CROWN_SWITCH1 :		// 왕관 스위치 1
		m_iCastleSwitchAccessUser1	= iIndex;
		break;
	case CS_NPC_CROWN_SWITCH2 :		// 왕관 스위치 2
		m_iCastleSwitchAccessUser2	= iIndex;
		break;
	}
}


INT	 CCastleSiege::GetCrownSwitchUserIndex		(
	INT iMonsterClass
	)
{
	INT iUserIndex	= -1;

	switch(iMonsterClass) {
	case CS_NPC_CROWN_SWITCH1 :		// 왕관 스위치 1
		iUserIndex	= m_iCastleSwitchAccessUser1;
		break;
	case CS_NPC_CROWN_SWITCH2 :		// 왕관 스위치 2
		iUserIndex	= m_iCastleSwitchAccessUser2;
		break;
	}
	
	return	iUserIndex;
}


VOID CCastleSiege::ResetCrownSwitchUserIndex	(
	INT iMonsterClass
	)
{
	switch(iMonsterClass) {
	case CS_NPC_CROWN_SWITCH1 :		// 왕관 스위치 1
		m_iCastleSwitchAccessUser1	= -1;
		break;
	case CS_NPC_CROWN_SWITCH2 :		// 왕관 스위치 2
		m_iCastleSwitchAccessUser2	= -1;
		break;
	}
}


VOID CCastleSiege::SetCrownAccessTickCount		()						// 왕관 접근 시간을 설정한다.
{
	m_dwCrownAccessTime		= GetTickCount();
}


VOID CCastleSiege::ResetCrownAccessTickCount	()						// 왕관 접근 시간을 초기화한다.
{
	m_dwCrownAccessTime		= 0;
}


INT	 CCastleSiege::CheckOverlapCsMarks			(INT iIndex)			// 특정 사용자의 인벤토리에 성주의 표식을 겹칠 수 있다면 몇번째 칸인지 알려준다.
{
	for(int x=MAX_EQUIPMENT; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 21) &&
				gObj[iIndex].pInventory[x].m_Level == 3
				)
			{
				// 성주의 표식이 존재한다면
				INT iITEM_DUR	= (INT) gObj[iIndex].pInventory[x].m_Durability;	// 아이템 개수 (내구도) 를 얻는다.

				if (CHECK_LIMIT(iITEM_DUR, MAX_CS_MARKS_OVERLAP)) {
					// 만약 M개 미만이라면 여기에 더 넣을 수 있다.
					return x;
				}
			}
		}
	}

	return -1;
}


INT	CCastleSiege::GetCsAttkGuildList			(						// 공성 참여 길드의 리스트를 가져온다. [0xB5] (결과 - 0:실패/1:성공/2:기한않됨/3:데이터없음)
	LPPMSG_CSATTKGUILDLIST lpMsgBody,
	INT & iCount
	)
{
	iCount					= 0;

	if (lpMsgBody == NULL) {
		return 0;
	}

	if (m_iCastleSiegeState != CASTLESIEGE_STATE_NOTIFY && 
		m_iCastleSiegeState != CASTLESIEGE_STATE_READYSIEGE 
		)
	{
		return 2;
	}

	if (m_bCsBasicGuildInfoLoadOK == FALSE ||
		m_bCsTotalGuildInfoLoadOK == FALSE
		)
	{
		return 3;
	}
	
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	map <string, CS_TOTAL_GUILD_DATA>::iterator it	= m_mapCsTotalGuildInfo.begin();
	for (;it != m_mapCsTotalGuildInfo.end() ; it++) {
		lpMsgBody[iCount].btCsJoinSide				= (it->second).m_iCsGuildID;
		lpMsgBody[iCount].btGuildInvolved			= (it->second).m_bGuildInvolved;
		memset (lpMsgBody[iCount].szGuildName, 0, sizeof(lpMsgBody[iCount].szGuildName));
		memcpy (lpMsgBody[iCount].szGuildName, &(it->first)[0], 8);
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
		lpMsgBody[iCount].iGuildScore				= (it->second).m_iGuildScore;
#endif		
		iCount++;
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);

	return 1;
}


BOOL CCastleSiege::CheckTeleportMagicAxisY		(						// 텔레포트 마법 시전 시 성벽에 걸리지 않는지 체크한다.
	INT iStartY,
	INT iTargetX,
	INT iTargetY
	)
{
	for (INT iSTEP = 0 ; iSTEP < MAX_CASTLEWALL_STEP ; iSTEP++) {
		if (g_iCastleWallStepAxisY[iSTEP] >= iStartY &&
			g_iCastleWallStepAxisY[iSTEP] <= iTargetY &&
			iTargetX < 144
			)
		{
			return FALSE;
		}
	}

	return TRUE;
}


VOID CCastleSiege::NotifyCsSelfLeftTime			(						// 공성전 진행중의 남은 시간을 보내준다.
	INT iIndex
	)
{
	GCAnsSelfCsLeftTimeAlarm(
		iIndex,
		(m_iCS_REMAIN_MSEC/1000)/3600,			// 시간
		((m_iCS_REMAIN_MSEC/1000)%3600)/60		// 분
		);
}


VOID CCastleSiege::CheckReviveNonDbNPC			()						// DB에 저장하지 않는 NPC를 생성한다.
{
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it				= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData		= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_bNPC_DBSAVE == FALSE))
		{
			// DB에 저장하지 않는 NPC들이다.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;				// 어차피 리젠되거나 고정 정보를 가진 NPC들이 아니다.
				gObj[iNPC_INDEX].X					= pNpcData.m_iNPC_SX;
				gObj[iNPC_INDEX].Y					= pNpcData.m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber			= MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX					= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY					= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX				= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY				= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir				= pNpcData.m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX			    = (BYTE)gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY				= (BYTE)gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen			= 0;
				gObj[iNPC_INDEX].MaxRegenTime		= 0;
				gObj[iNPC_INDEX].Life				= pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife			= pNpcData.m_iNPC_MAXHP;

				switch(pNpcData.m_iNPC_NUM) {
				case 215 :	// 방어막 (수성측)
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_DFN;		// 수성측 DB 몬스터
					break;
				case 221 :	// 발석차 (공성측)
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_ATK;		// 공성측 DB 몬스터
					break;
				case 222 :	// 발석차 (수성측)
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_DFN;		// 수성측 DB 몬스터
					break;
				case 288 :	// 가드타워 (수성측)
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_DFN;		// 수성측 DB 몬스터
					break;
				default :	// 기본 ?
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_DFN;		// 공성측 DB 몬스터
					break;
				}

				// 나중에 관리 확인을 하기위해 여기에 인증값을 넣는다. -> DB NPC가 아닌 경우에는 사실상 필요는 없다.
//				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
//				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
//				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
//				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;
		
				// 최종적으로 이 NPC는 생성되었음을 알린다.
//				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
//				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
				(*it).m_bIN_USE						= TRUE;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


VOID CCastleSiege::CheckReviveGuardianStatue	()						// 수호석상 중 현재 죽어있는 것이 있다면 DB저장 없이 바로 추가시킨다.
{
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it				= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData		= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == 283) &&
			(pNpcData.m_iNPC_LIVE == NPC_LIVE_NONE)
			)
		{
			// 현재 죽어있는 수호석상 -> 게임 상에 추가한다.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;				// 어차피 리젠되거나 고정 정보를 가진 NPC들이 아니다.
				gObj[iNPC_INDEX].X					= pNpcData.m_iNPC_SX;
				gObj[iNPC_INDEX].Y					= pNpcData.m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber			= MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX					= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY					= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX				= gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY				= gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir				= pNpcData.m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX			    = (BYTE)gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY				= (BYTE)gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen			= 0;
				gObj[iNPC_INDEX].MaxRegenTime		= 0;
				gObj[iNPC_INDEX].Life				= pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife			= pNpcData.m_iNPC_MAXHP;
				gObj[iNPC_INDEX].m_btCsNpcType		= CS_NPC_TYPE_DB_DFN;				// 수성측 DB 몬스터

				INT iNPC_DF_LEVEL					= pNpcData.m_iNPC_BASE_DF_LEVEL;	// 생성 초기의 방어력 레벨
				if (iNPC_DF_LEVEL < 0)						
					iNPC_DF_LEVEL					= 0;
				if (iNPC_DF_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_DF_LEVEL					= MAX_UPGRADE_LEVEL;
				INT iNPC_RG_LEVEL					= pNpcData.m_iNPC_BASE_RG_LEVEL;	// 생성 초기의 회복력 레벨
				if (iNPC_RG_LEVEL < 0)						
					iNPC_RG_LEVEL					= 0;
				if (iNPC_RG_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_RG_LEVEL					= MAX_UPGRADE_LEVEL;

				// 회복력, 방어력 등의 수치가 필요하다면 여기서 세팅해준다. -> 외부에서 (AdjustDbNpcLevel() - 공성 시작전)
				gObj[iNPC_INDEX].m_Defense			= g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
				gObj[iNPC_INDEX].m_btCsNpcDfLevel	= iNPC_DF_LEVEL;
				gObj[iNPC_INDEX].m_btCsNpcRgLevel	= iNPC_RG_LEVEL;

				// 나중에 관리 확인을 하기위해 여기에 인증값을 넣는다.
				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;
		
				// 최종적으로 이 NPC는 생성되었음을 알린다.
				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
				(*it).m_bIN_USE						= TRUE;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


VOID CCastleSiege::ReSpawnAllUser				()						// 현재 공성맵 안에 있는 모든 사람들을 각각의 역할에 맞게 지정된 위치로 강제 이동 시킨다.
{
	INT		iTX, iTY;
	BYTE	btMapAttr;

	for (INT iUSER = ALLOC_USEROBJECTSTART; iUSER<MAX_OBJECT; iUSER++) {
		if (!gObjIsConnected(iUSER))
			continue;
		if (gObj[iUSER].MapNumber != MAP_INDEX_CASTLESIEGE)
			continue;

		BYTE btCsJoinSide							= gObj[iUSER].m_btCsJoinSide;
		if (btCsJoinSide < CS_SIEGESIDE_NONE)		btCsJoinSide	= CS_SIEGESIDE_NONE;
		if (btCsJoinSide > CS_SIEGESIDE_ATTACK)		btCsJoinSide	= CS_SIEGESIDE_ATTACK;

		BOOL bMoveOK								= FALSE;

		switch(btCsJoinSide) {
		case CS_SIEGESIDE_DEFEND :
			{
				for (INT iCNT = 0 ; iCNT < 100 ; iCNT++) {
					iTX			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][2] - g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][0]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][0];
					iTY			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][3] - g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][1]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][1];
					btMapAttr	= MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[(iTY*MAX_TERRAIN_SIZE)+iTX];
					if( (btMapAttr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) {
						// 여기서 사용자를 이동 시킨다.
						bMoveOK	= TRUE;
						break;
					}
				}
			}
			break;
		case CS_SIEGESIDE_ATTACK :
			{
				for (INT iCNT = 0 ; iCNT < 100 ; iCNT++) {
					iTX			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][2] - g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][0]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][0];
					iTY			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][3] - g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][1]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][1];
					btMapAttr	= MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[(iTY*MAX_TERRAIN_SIZE)+iTX];
					if( (btMapAttr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) {
						// 여기서 사용자를 이동 시킨다.
						bMoveOK	= TRUE;
						break;
					}
				}
			}
			break;
		case CS_SIEGESIDE_NONE :
		default :
			{
				for (INT iCNT = 0 ; iCNT < 100 ; iCNT++) {
					iTX			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][2] - g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][0]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][0];
					iTY			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][3] - g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][1]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][1];
					btMapAttr	= MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[(iTY*MAX_TERRAIN_SIZE)+iTX];
					if( (btMapAttr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) {
						// 여기서 사용자를 이동 시킨다.
						bMoveOK	= TRUE;
						break;
					}
				}
			}
			break;
		}

		if (bMoveOK) {
			gObj[iUSER].m_State			= OBJST_DELCMD;
			gObj[iUSER].X				= iTX;
			gObj[iUSER].Y				= iTY;
			gObj[iUSER].TX				= iTX;
			gObj[iUSER].TX				= iTY;
			gObj[iUSER].MapNumber		= MAP_INDEX_CASTLESIEGE;
//			gObj[iUSER].Dir				= dir; 
			gObj[iUSER].PathCount		= 0;
			gObj[iUSER].Teleport		= 0;

			gObjClearViewport(&gObj[iUSER]);
			GCTeleportSend(&gObj[iUSER], -1, MAP_INDEX_CASTLESIEGE, (BYTE)gObj[iUSER].X, (BYTE)gObj[iUSER].Y, gObj[iUSER].Dir);

			if( gObj[iUSER].m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[iUSER]);
			}

			gObj[iUSER].RegenMapNumber	= MAP_INDEX_CASTLESIEGE;
			gObj[iUSER].RegenMapX		= (BYTE)iTX;
			gObj[iUSER].RegenMapY		= (BYTE)iTY;		
			gObj[iUSER].RegenOk			= 1;
		}
	}
}


VOID CCastleSiege::ReSpawnEnemyUser				(						// 현재 공성맵 안에 있는 수성측이 아닌 모든 사람들을 성 밖으로 강제 이동 시킨다.
	BOOL bRefreshOwnerUser
	)
{
	INT		iTX, iTY;
	BYTE	btMapAttr;

	for (INT iUSER = ALLOC_USEROBJECTSTART; iUSER<MAX_OBJECT; iUSER++) {
		if (!gObjIsConnected(iUSER))
			continue;
		
#ifdef MODIFY_CASTLEHUNTZONE_USER_BUGFIX_20070223
		// 시련의 땅에 있는 유저도 옮긴다.
		if (gObj[iUSER].MapNumber != MAP_INDEX_CASTLESIEGE && gObj[iUSER].MapNumber != MAP_INDEX_CASTLEHUNTZONE)
			continue;
#else
		if (gObj[iUSER].MapNumber != MAP_INDEX_CASTLESIEGE)
			continue;		
#endif

		BYTE btCsJoinSide							= gObj[iUSER].m_btCsJoinSide;
		if (btCsJoinSide < CS_SIEGESIDE_NONE)		btCsJoinSide	= CS_SIEGESIDE_NONE;
		if (btCsJoinSide > CS_SIEGESIDE_ATTACK)		btCsJoinSide	= CS_SIEGESIDE_ATTACK;

		BOOL bMoveOK								= FALSE;

		switch(btCsJoinSide) {
		case CS_SIEGESIDE_DEFEND :
			{
				if (bRefreshOwnerUser) {
					// 수성측 위치를 리프레쉬 해준다. (최초 공성 시작 시)
					gObj[iUSER].RegenMapNumber		= gObj[iUSER].MapNumber;
					gObj[iUSER].RegenMapX			= (BYTE) gObj[iUSER].X;
					gObj[iUSER].RegenMapY			= (BYTE) gObj[iUSER].Y;		

#ifdef MODIFY_CASTLEHUNTZONE_USER_BUGFIX_20070223
					// 시련의 땅에 있는 유저일 경우
					if( gObj[iUSER].MapNumber == MAP_INDEX_CASTLEHUNTZONE )
					{
						for (INT iCNT = 0 ; iCNT < 100 ; iCNT++) 
						{
							iTX			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][2] - g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][0]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][0];
							iTY			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][3] - g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][1]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][1];
							btMapAttr	= MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[(iTY*MAX_TERRAIN_SIZE)+iTX];
							if( (btMapAttr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) 
							{
								// 여기서 사용자를 이동 시킨다.
								bMoveOK	= TRUE;
								break;
							}	
						}
						break;
					}

#endif
					gObjClearViewport(&gObj[iUSER]);
					GCTeleportSend(&gObj[iUSER], -1, gObj[iUSER].MapNumber, (BYTE)gObj[iUSER].X, (BYTE)gObj[iUSER].Y, gObj[iUSER].Dir);

					if( gObj[iUSER].m_Change >= 0 )
					{
						gObjViewportListProtocolCreate(&gObj[iUSER]);
					}

					gObj[iUSER].RegenOk				 = 1;				
				}
				bMoveOK								= FALSE;
			}
			break;
		case CS_SIEGESIDE_ATTACK :
			{
				for (INT iCNT = 0 ; iCNT < 100 ; iCNT++) {
					iTX			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][2] - g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][0]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][0];
					iTY			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][3] - g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][1]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_ATTACK][1];
					btMapAttr	= MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[(iTY*MAX_TERRAIN_SIZE)+iTX];
					if( (btMapAttr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) {
						// 여기서 사용자를 이동 시킨다.
						bMoveOK	= TRUE;
						break;
					}
				}
			}
			break;
		case CS_SIEGESIDE_NONE :
			{
				for (INT iCNT = 0 ; iCNT < 100 ; iCNT++) {
					iTX			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][2] - g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][0]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][0];
					iTY			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][3] - g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][1]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_NONE][1];
					btMapAttr	= MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[(iTY*MAX_TERRAIN_SIZE)+iTX];
					if( (btMapAttr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) {
						// 여기서 사용자를 이동 시킨다.
						bMoveOK	= TRUE;
						break;
					}
				}
			}
			break;
		}

		if (bMoveOK) {
			gObj[iUSER].m_State						= OBJST_DELCMD;
			gObj[iUSER].X							= iTX;
			gObj[iUSER].Y							= iTY;
			gObj[iUSER].TX							= iTX;
			gObj[iUSER].TX							= iTY;
			gObj[iUSER].MapNumber					= MAP_INDEX_CASTLESIEGE;
			gObj[iUSER].PathCount					= 0;
			gObj[iUSER].Teleport					= 0;

			gObjClearViewport(&gObj[iUSER]);
			GCTeleportSend(&gObj[iUSER], -1, MAP_INDEX_CASTLESIEGE, (BYTE)gObj[iUSER].X, (BYTE)gObj[iUSER].Y, gObj[iUSER].Dir);

			if( gObj[iUSER].m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[iUSER]);
			}

			gObj[iUSER].RegenMapNumber				= MAP_INDEX_CASTLESIEGE;
			gObj[iUSER].RegenMapX					= (BYTE)iTX;
			gObj[iUSER].RegenMapY					= (BYTE)iTY;		
			gObj[iUSER].RegenOk						= 1;
		}

		// 다시한번 자신의 상태를 갱신할 수 있도록 보내준다.
		NotifySelfCsJoinSide (iUSER);
		
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0;
#endif

	}
}


VOID CCastleSiege::NotifyAllUserCsStartState	(BYTE btStartState)		// 현재 공성전이 시작되었는지 여부를 모든 사용자들에게 알린다.
{
	PMSG_ANS_NOTIFYCSSTART pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x17, sizeof(PMSG_ANS_NOTIFYCSSTART));
	pMsg.btStartState		= btStartState;

	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}


VOID CCastleSiege::NotifyAllUserCsProgState		(						// 현재 공성전의 진행 상태를 모든 사용자들에게 알린다.
		BYTE btProgState,
		LPSTR lpszGuildName
		)
{
	PMSG_ANS_NOTIFYCSPROGRESS pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x18, sizeof(PMSG_ANS_NOTIFYCSPROGRESS));
	pMsg.btCastleSiegeState		= btProgState;
	memset(pMsg.szGuildName, 0, sizeof(pMsg.szGuildName));
	memcpy(pMsg.szGuildName, lpszGuildName, 8);

	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			if (gObj[i].MapNumber == MAP_INDEX_CASTLESIEGE) {
				DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}


VOID CCastleSiege::ClearCastleTowerBarrier	()							// 현재 존재하는 모든 방어막 NPC를 제거한다.
{
	for (INT iIDX = 0 ; iIDX < MAX_MONSTER ; iIDX++) {
		if (!gObjIsConnected(iIDX))
			continue;
		if (gObj[iIDX].Class == 215) {
			gObjDel(iIDX);
		}
	}
}


VOID CCastleSiege::SetAllCastleGateState	(BOOL bOpenType)			// 모든 존재하는 성문의 상태를 변경한다.
{
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
	BOOL bExist	= FALSE;

	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));

		if (pNpcData.m_iNPC_NUM == 277 &&
			gObjIsConnected(pNpcData.m_iNPC_OBJINDEX)
			)
		{
			if (gObj[pNpcData.m_iNPC_OBJINDEX].Class == 277) {
				// 성문이 맞다.
				switch(bOpenType) {
				case CS_GATE_STATE_CLOSED :
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_CLOSED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjRemoveBuffEffect( &gObj[pNpcData.m_iNPC_OBJINDEX], BUFFTYPE_CASTLE_DOOR_STATE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObj[pNpcData.m_iNPC_OBJINDEX].m_btCsGateOpen	= CS_GATE_STATE_CLOSED;
					break;
				case CS_GATE_STATE_OPENED :
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_OPENED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[pNpcData.m_iNPC_OBJINDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObj[pNpcData.m_iNPC_OBJINDEX].m_btCsGateOpen	= CS_GATE_STATE_OPENED;
					break;
				}
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


VOID CCastleSiege::AddMiniMapDataReqUser	(INT iIndex)				// 미니맵 데이터 요청 인원을 등록한다.
{
	EnterCriticalSection(& m_critCsMiniMap);
	if (find(m_vtMiniMapReqUser.begin(), m_vtMiniMapReqUser.end(), iIndex) == m_vtMiniMapReqUser.end()) {
		m_vtMiniMapReqUser.push_back(iIndex);
	}
	LeaveCriticalSection(& m_critCsMiniMap);
}


VOID CCastleSiege::DelMiniMapDataReqUser	(INT iIndex)				// 미니맵 데이터 요청 인원을 제거한다.
{
	EnterCriticalSection(& m_critCsMiniMap);
	vector<INT>::iterator it = find(m_vtMiniMapReqUser.begin(), m_vtMiniMapReqUser.end(), iIndex);
	if (it != m_vtMiniMapReqUser.end()) {
		m_vtMiniMapReqUser.erase(it);
	}
	LeaveCriticalSection(& m_critCsMiniMap);
}


VOID CCastleSiege::OperateMiniMapWork		()							// 미니맵 관련 작업을 수행한다. -> 데이터 구축 전송
{
	if (m_bCsBasicGuildInfoLoadOK == FALSE)
		return;

	map <INT, CS_MINIMAP_DATA>	mapMiniMapData;

	BYTE cBUFFER1[sizeof(PMSG_ANS_SENDMINIMAPDATA) + sizeof(PMSG_SENDMINIMAPDATA)*MAX_MINIMAP_USER_COUNT + 256]	= {0,};
	LPPMSG_ANS_SENDMINIMAPDATA lpMsgSend1 = (LPPMSG_ANS_SENDMINIMAPDATA) cBUFFER1;
	LPPMSG_SENDMINIMAPDATA lpMsgSendBody1 = (LPPMSG_SENDMINIMAPDATA) (cBUFFER1 + sizeof(PMSG_ANS_SENDMINIMAPDATA));

	BYTE cBUFFER2[sizeof(PMSG_ANS_SENDNPCMINIMAPDATA) + sizeof(PMSG_SENDNPCMINIMAPDATA)*MAX_CS_NPC_COUNT + 256]	= {0,};
	LPPMSG_ANS_SENDNPCMINIMAPDATA lpMsgSend2 = (LPPMSG_ANS_SENDNPCMINIMAPDATA) cBUFFER2;
	LPPMSG_SENDNPCMINIMAPDATA lpMsgSendBody2 = (LPPMSG_SENDNPCMINIMAPDATA) (cBUFFER2 + sizeof(PMSG_ANS_SENDNPCMINIMAPDATA));

	// 1 . 수집해야 할 참여길드의 종류를 파악한다.
	EnterCriticalSection(& m_critCsBasicGuildInfo);
	map<string, CS_TOTAL_GUILD_DATA>::iterator it		= m_mapCsBasicGuildInfo.begin();
	for ( ; it != m_mapCsBasicGuildInfo.end() ; it++ ) {
		CS_TOTAL_GUILD_DATA & stTotalGuildData			= (it->second);
		CS_MINIMAP_DATA stMiniMapData;
		memcpy (stMiniMapData.m_szGuildName, &(it->first)[0], 8);
		mapMiniMapData.insert(pair<INT, CS_MINIMAP_DATA>(stTotalGuildData.m_iCsGuildID, stMiniMapData));
	}
	LeaveCriticalSection(& m_critCsBasicGuildInfo);

	if (mapMiniMapData.empty())
		return;

	// 2 . 공성전 참여 NPC 정보를 수집한다.
	EnterCriticalSection(& m_critNpcData);
	lpMsgSend2->iCount					= 0;
	if (!m_vtNpcData.empty()) {
		vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
		for (; it != m_vtNpcData.end() ; it++) {
			if (lpMsgSend2->iCount >= MAX_CS_NPC_COUNT)
				break;

			CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
			if ((pNpcData.m_iNPC_NUM == 277 ||	// 성문
				pNpcData.m_iNPC_NUM == 283		// 석상
				) &&
				pNpcData.m_iNPC_OBJINDEX != -1
				) 
			{
				if (!gObjIsConnected(pNpcData.m_iNPC_OBJINDEX) ||
					(gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal != pNpcData.m_iCsNpcExistVal) ||
					(gObj[pNpcData.m_iNPC_OBJINDEX].Class != 277 && gObj[pNpcData.m_iNPC_OBJINDEX].Class != 283)
					) 
				{
					BYTE btNpcType		= 0;
					switch(pNpcData.m_iNPC_NUM) {
					case 277 :					// 성문 - 블럭속성을 해제해 준다.
						{
							btNpcType	= 0;
						}
						break;
					case 283 :					// 석상 - 다른 석상들이 제거되었는지 확인하여 용의 탑 입구를 개방한다.
						{
							btNpcType	= 1;
						}
						break;
					default :
						{
							continue;
						}
						break;
					}

					lpMsgSendBody2[lpMsgSend2->iCount].btNpcType	= btNpcType;
					lpMsgSendBody2[lpMsgSend2->iCount].btX			= gObj[pNpcData.m_iNPC_OBJINDEX].X;
					lpMsgSendBody2[lpMsgSend2->iCount].btY			= gObj[pNpcData.m_iNPC_OBJINDEX].Y;
					lpMsgSend2->iCount++;
				}
			}
		}
	}
	lpMsgSend2->h.set(cBUFFER2, 0xBB, sizeof(PMSG_ANS_SENDNPCMINIMAPDATA) + sizeof(PMSG_SENDNPCMINIMAPDATA) * lpMsgSend2->iCount);
	LeaveCriticalSection(& m_critNpcData);

	// 3 . 위에서 파악된 참여길드 정보를 바탕으로 길드원 좌표 데이터를 구축한다.
	EnterCriticalSection(& m_critCsMiniMap);
	m_mapMiniMapData.clear();
	map <INT, CS_MINIMAP_DATA>::iterator it2			= mapMiniMapData.begin();
	for ( ; it2 != mapMiniMapData.end() ; it2++ ) {
		m_mapMiniMapData.insert(pair<INT, CS_MINIMAP_DATA>(it2->first, it2->second));
	}

	for (INT iIDX = ALLOC_USEROBJECTSTART ; iIDX < MAX_OBJECT ; iIDX++) {
		if (!gObjIsConnected(iIDX))
			continue;

		if (gObj[iIDX].MapNumber != MAP_INDEX_CASTLESIEGE)
			continue;
		
		if (gObj[iIDX].m_btCsJoinSide == CS_SIEGESIDE_NONE)
			continue;
		
		map <INT, CS_MINIMAP_DATA>::iterator it3		= m_mapMiniMapData.find(gObj[iIDX].m_btCsJoinSide);
		if (it3 != m_mapMiniMapData.end()) {
			if ((it3->second).m_iMiniMapPointCount >= MAX_MINIMAP_USER_COUNT)
				continue;

			// 이 사람이 길드의 연합 장인지 확인하여 맞으면 길드마스터 인덱스에 기록한다.
			if (gObj[iIDX].GuildStatus == G_MASTER) {
				if (strcmp(gObj[iIDX].GuildName, "") &&
					!strcmp(gObj[iIDX].GuildName, (it3->second).m_szGuildName)
					)
				{
					(it3->second).m_iGuildMasterIndex	= iIDX;
					continue;
				}
			}

			// 특정 공성측에 속하는 사람이 맞다. -> 해당 미니맵에 위치를 추가해 준다.
			(it3->second).m_stMiniMapPoint[(it3->second).m_iMiniMapPointCount].btX	= gObj[iIDX].X;
			(it3->second).m_stMiniMapPoint[(it3->second).m_iMiniMapPointCount].btY	= gObj[iIDX].Y;
			(it3->second).m_iMiniMapPointCount++;
		}
	}

	// 최종적으로 각 길드의 길마들에게 미니맵 데이터를 보낸다.
	map <INT, CS_MINIMAP_DATA>::iterator it4			= m_mapMiniMapData.begin();
	for ( ; it4 != m_mapMiniMapData.end() ; it4++ ) {
		lpMsgSend1->iCount								= 0;
		
		if (!gObjIsConnected((it4->second).m_iGuildMasterIndex))
			continue;
		
		// 위에서 길드마스터 체크를 했으므로 여기서는 별도의 체크 없이 무조건 보낸다.
		lpMsgSend1->iCount								= (it4->second).m_iMiniMapPointCount;
		if (lpMsgSend1->iCount > MAX_MINIMAP_USER_COUNT)
			lpMsgSend1->iCount							= MAX_MINIMAP_USER_COUNT;
		memcpy(lpMsgSendBody1, (it4->second).m_stMiniMapPoint, sizeof(PMSG_SENDMINIMAPDATA) * lpMsgSend1->iCount);
		lpMsgSend1->h.set(cBUFFER1, 0xB6, sizeof(PMSG_ANS_SENDMINIMAPDATA) + sizeof(PMSG_SENDMINIMAPDATA) * lpMsgSend1->iCount);
		DataSend((it4->second).m_iGuildMasterIndex, cBUFFER1, sizeof(PMSG_ANS_SENDMINIMAPDATA) + sizeof(PMSG_SENDMINIMAPDATA) * lpMsgSend1->iCount);

		// 위에서 수집한 공성전 NPC 정보를 같이 보낸다.
		DataSend((it4->second).m_iGuildMasterIndex, cBUFFER2, sizeof(PMSG_ANS_SENDNPCMINIMAPDATA) + sizeof(PMSG_SENDNPCMINIMAPDATA) * lpMsgSend2->iCount);
	}

	// 이전에 미니맵 데이터를 요청했던 사용자들에게 위에서 정리한 데이터를 보낸다.
/*
	vector <INT>::iterator it4							= m_vtMiniMapReqUser.begin();
	for ( ; it4 != m_vtMiniMapReqUser.end() ; it4++ ) {
		INT iIndex										= (*it4);

		lpMsgSend1->iCount								= 0;
		
		if (!gObjIsConnected(iIndex)) {
			// 접속하지 않았다.
			m_vtMiniMapReqUser.erase(it4);
			continue;
		}

		if (gObj[iIndex].m_btCsJoinSide <= 0) {
			// 공성 참여길드 소속이 아니다.
			m_vtMiniMapReqUser.erase(it4);
			continue;
		}

		map <INT, CS_MINIMAP_DATA>::iterator it5		= m_mapMiniMapData.find(gObj[iIndex].m_btCsJoinSide);
		if (it5 == m_mapMiniMapData.end()) {
			// 공성 참여길드 소속이 아니다.
			m_vtMiniMapReqUser.erase(it4);
			continue;
		}

		CS_MINIMAP_DATA & stMiniMapData					= (it5->second);
		
		lpMsgSend1->iCount								= stMiniMapData.m_iMiniMapPointCount;
		if (lpMsgSend1->iCount > MAX_MINIMAP_USER_COUNT)
			lpMsgSend1->iCount							= MAX_MINIMAP_USER_COUNT;
		memcpy(lpMsgSendBody1, stMiniMapData.m_stMiniMapPoint, sizeof(PMSG_SENDMINIMAPDATA) * lpMsgSend1->iCount);
		lpMsgSend1->h.set(cBUFFER1, 0xB6, sizeof(PMSG_ANS_SENDMINIMAPDATA) + sizeof(PMSG_SENDMINIMAPDATA) * lpMsgSend1->iCount);
		DataSend(iIndex, cBUFFER1, sizeof(PMSG_ANS_SENDMINIMAPDATA) + sizeof(PMSG_SENDMINIMAPDATA) * lpMsgSend1->iCount);
	}
*/

	LeaveCriticalSection(& m_critCsMiniMap);
}


VOID CCastleSiege::NotifyCrownState			(BYTE btState)				// 현재 왕관의 상태를 주변에 알림 (0:보호막해제/1:보호막생성/2:등록성공)
{
	if (!gObjIsConnected(m_iCrownIndex)) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::NotifyCrownState() - m_iCrownIndex isn't Valid");
		return;
	}
	if (gObj[m_iCrownIndex].Class != 216) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::NotifyCrownState() - m_iCrownIndex isn't Valid");
		return;
	}

	LPOBJECTSTRUCT lpObj	= &gObj[m_iCrownIndex];

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
	if( btState == 0 )
	{
		gObjAddBuffEffect( lpObj, BUFFTYPE_CASTLE_CROWN_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
	}
	else
	{
		gObjRemoveBuffEffect( lpObj, BUFFTYPE_CASTLE_CROWN_STATE );
	}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128

	// 뷰포트 상에 아무도 없다면 끝난다.
	if( lpObj->VPCount2 < 1 ) return;

	PMSG_ANS_NOTIFYCROWNSTATE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x16, sizeof(PMSG_ANS_NOTIFYCROWNSTATE));
	pMsg.btCrownState		= btState;

	// 뷰포트 상에서 해당되는 사람들을 찾는다.
	INT	tObjNum	= -1;
	for( int i=0; i<MAXVIEWPORTOBJECT; i++)							// 왕관 (NPC) 의 뷰포트 개수는 사람과 같음
	{
		tObjNum = lpObj->VpPlayer2[i].number;
		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
			{
				DataSend(tObjNum, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
	
	LogAddTD("[CastleSiege] Crown State Changed (%d)", btState);
}

#ifdef MODIFY_CASTLESIEGE_NOTIFY_CROWNSWITCH_USER_20051017
VOID CCastleSiege::NotifyCrownSwitchInfo( INT iCrownSwitchIndex )
{
	// 왕관 스위치 정보를 보낸다.

	if (!gObjIsConnected(iCrownSwitchIndex)) 
	{
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::NotifyCrownSwitchInfo() - iCrownSwitchIndex is Invalid");
		return;
	}
	if (gObj[iCrownSwitchIndex].Class != CS_NPC_CROWN_SWITCH1 && gObj[iCrownSwitchIndex].Class != CS_NPC_CROWN_SWITCH2)
	{
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::NotifyCrownSwitchInfo() - iCrownSwitchIndex is Invalid");
		return;
	}

	LPOBJECTSTRUCT lpObj	= &gObj[iCrownSwitchIndex];

	// 뷰포트 상에 아무도 없다면 끝난다.
	if( lpObj->VPCount2 < 1 ) return;

	PMSG_ANS_NOTIFY_CROWNSWITCH_INFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x20, sizeof(PMSG_ANS_NOTIFY_CROWNSWITCH_INFO));
	pMsg.btIndex1 = HIBYTE((WORD)iCrownSwitchIndex);
	pMsg.btIndex2 = LOBYTE((WORD)iCrownSwitchIndex);
	pMsg.btSwitchState	= 0;
	pMsg.btJoinSide		= 0;
	ZeroMemory( pMsg.szGuildName, MAX_GUILDNAMESTRING );	
	ZeroMemory( pMsg.szUserName, MAX_IDSTRING );


	// 공성 스위치에 접속 해 있는 유저 정보 세팅
	INT iCrownSwitchUserIndex = g_CastleSiege.GetCrownSwitchUserIndex(lpObj->Class);

	if( gObjIsConnected(iCrownSwitchUserIndex) )
	{
		LPOBJECTSTRUCT lpCrownSwitchUserObj = 	&gObj[iCrownSwitchUserIndex];
		
		pMsg.btSwitchState	= 1;
		pMsg.btJoinSide		= lpCrownSwitchUserObj->m_btCsJoinSide;
		CopyMemory( pMsg.szUserName, lpCrownSwitchUserObj->Name, MAX_IDSTRING );
		CopyMemory( pMsg.szGuildName, lpCrownSwitchUserObj->GuildName, MAX_GUILDNAMESTRING );

		char szUnionName[MAX_GUILDNAMESTRING];
		ZeroMemory( szUnionName, MAX_GUILDNAMESTRING );
		
		if( lpCrownSwitchUserObj->lpGuild )
		{
			if( UnionManager.GetUnionName(lpCrownSwitchUserObj->lpGuild->iGuildUnion, szUnionName) != -1 )
			{
				CopyMemory( pMsg.szGuildName, szUnionName, MAX_GUILDNAMESTRING );
			}
		}
		
		// 공성 스위치에 접속 해 있는 유저에게는 무조건 보내준다.
		DataSend(iCrownSwitchUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	

	// 뷰포트 상에서 해당되는 사람들을 찾는다.
	INT	tObjNum	= -1;
	for( int i=0; i<MAXVIEWPORTOBJECT; i++)							// 왕관 (NPC) 의 뷰포트 개수는 사람과 같음
	{
		tObjNum = lpObj->VpPlayer2[i].number;
		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
			{
				DataSend(tObjNum, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}


}
#endif


VOID CCastleSiege::SendMapServerGroupMsg	(						// 맵서버 그룹 전체의 사용자에게 특정 메시지를 보낸다.
	LPSTR lpszMsg 
	)
{
	GS_GDReqMapSvrMsgMultiCast	(m_iMapSvrGroup, lpszMsg);
}


VOID CCastleSiege::SendAllUserAnyData		(						// 접속해 있는 모든 사용자들에게 특정 데이터를 보낸다.
	LPBYTE lpMsg, 
	INT iSize
	)
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}


VOID CCastleSiege::SendAllUserAnyMsg		(						// 접속해 있는 모든 사용자들에게 특정 메시지를 보낸다.
	LPSTR lpszMsg,
	INT iType
	)
{
	if (lpszMsg == NULL)
		return;

	PMSG_NOTICE	pNotice;

	switch(iType) {
	case 1 :			// 슬라이딩 공지
		{
#ifdef MODIFY_NOTICE_20040325
			TNotice::MakeNoticeMsg( &pNotice, 0, lpszMsg );
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_DEFAULT);
			TNotice::SendNoticeToAllUser( &pNotice );
#else
			AllSendServerMsg(lpszMsg);
#endif
		}
		break;
	case 2:				// 중앙 공지
		{
#ifdef MODIFY_NOTICE_20040325
			TNotice::MakeNoticeMsg( &pNotice, 0, lpszMsg );
#else
			pNotice.type = 0;
			strcpy(pNotice.Notice, lpszMsg);
			PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

			for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
			{
				if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
				{
					DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
				}
			}
		}
		break;
	}
}


VOID CCastleSiege::SendCsUserAnyData		(						// 공성전맵에 접속해 있는 모든 사용자들에게 특정 데이터를 보낸다.
	LPBYTE lpMsg, 
	INT iSize
	)
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER) && (gObj[i].MapNumber == MAP_INDEX_CASTLESIEGE))
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}


VOID CCastleSiege::SendCsUserAnyMsg			(						// 공성전맵에 접속해 있는 모든 사용자들에게 특정 메시지를 보낸다.
	LPSTR lpszMsg 
	)
{
	if (lpszMsg == NULL)
		return;

	PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsg( &pNotice, 0, lpszMsg );
#else
	pNotice.type = 0;
	strcpy(pNotice.Notice, lpszMsg);
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER) && (gObj[i].MapNumber == MAP_INDEX_CASTLESIEGE))
		{
			DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
		}
	}
}


// ## MODIFY_CASTLESIEGE_GM_MESSAGE_20050722
#ifdef CASTLE_GM_COMMAND_20041222								// 운영자 공성제어 명령처리
VOID CCastleSiege::OperateGmCommand		(
		INT iIndex,
		INT iGmCommand,
		LPVOID lpParam
		)
{
	if (!gObjIsConnected(iIndex))
		return;

#if TESTSERVER != 1
	// 운영자가 아니면 명령을 무시한다.
	if( (gObj[iIndex].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
		return;
#endif
	
	switch (iGmCommand) {
	case CS_GMCMD_CHANGEOWNERGUILD :							// "/공성수성길드"
		{
			LPSTR lpszCastleOwnerGuild	= (LPSTR) lpParam;
			if (lpszCastleOwnerGuild == NULL) {
				MsgOutput(iIndex, lMsg.Get(3200) );		// "수성측 길드변경 실패"
				return;
			}
			if (strcmp(lpszCastleOwnerGuild, "")) {
				memset(m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
				memcpy(m_szCastleOwnerGuild, lpszCastleOwnerGuild, 8);
				m_btIsCastleOccupied	= TRUE;

				MsgOutput(iIndex, lMsg.Get(3201), m_szCastleOwnerGuild);	// "수성측 길드변경 : %s"
			}
			else {
				strcpy(m_szCastleOwnerGuild, "");
				m_btIsCastleOccupied	= FALSE;

				MsgOutput(iIndex, lMsg.Get(3200) );	// "수성측 길드변경 실패"
			}
		}
		return;
	case CS_GMCMD_SETSTATE_REGSIEGE :							// "/공성참여등록"
		{
			GetLocalTime(&m_tmStartDate);
			m_tmStartDate.wHour			= 0;
			m_tmStartDate.wMinute		= 0;
			m_tmStartDate.wSecond		= 0;
			m_tmEndDate					= m_tmStartDate;
			GetNextDay(m_tmEndDate, m_iCastleSiegeCycle);

			m_bFixCastleCycleStartDate	= FALSE;
			m_btIsSiegeEnded			= FALSE;
			
			// 공성전의 DB NPC들을 다시 생성한다. (일부 파괴되었을 수 있으므로)

			// 20051101 공성참여등록 시 임시로 생성되는 NPC 생성을 막음. (apple)
			// 업그레이드 테스트를 위해!
			// REMOVE_CASTLESIEGE_AUTO_REVIVE_GUARDIANSTATUE_20051101
//			CreateDbNPC_INS();
			
			// 공성전 참여신청 길드 리스트를 초기화한다.
			GS_GDReqResetRegSiegeInfo (m_iMapSvrGroup);

			// 공성전 시간을 세팅한다.
			if (CheckSync() == FALSE) {
				LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
				MsgBox("[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
				MsgOutput(iIndex, lMsg.Get(3202));	// "상태변경 실패 - 공성참여등록"
				return;
			}

			// 이제 공성전 스케쥴 루프를 진행해도 된다.
			m_bDoRun								= TRUE;
			m_dwNPC_DBSAVE_TICK_COUNT				= GetTickCount();
			m_dwCALC_LEFTSIEGE_TICK_COUNT			= 0;
			m_dwEVENT_MSG_TICK_COUNT				= 0;
			m_dwCHECK_GATE_ALIVE_COUNT				= 0;
			MsgOutput(iIndex, lMsg.Get(3203));		// "상태변경 성공 - 공성참여등록"
		}
		return;
	case CS_GMCMD_SETSTATE_REGMARK :							// "/공성문장등록"
		{
			SetState (CASTLESIEGE_STATE_REGMARK);
			MsgOutput(iIndex, lMsg.Get(3204));		// "상태변경 성공 - 공성문장등록"
		}
		return;
	case CS_GMCMD_SETSTATE_NOTIFY :								// "/공성선포"
		{
			m_btIsSiegeGuildList					= FALSE;
			SetState (CASTLESIEGE_STATE_NOTIFY);
			MsgOutput(iIndex, lMsg.Get(3205));			// "상태변경 성공 - 공성선포"
		}
		return;
	case CS_GMCMD_SETSTATE_STARTSIEGE :							// "/공성시작"
		{
			SetState (CASTLESIEGE_STATE_STARTSIEGE);
			MsgOutput(iIndex, lMsg.Get(3206));			// "상태변경 성공 - 공성시작"
		}
		return;
	case CS_GMCMD_SETSTATE_ENDSIEGE :							// "/공성종료"
		{
			if (m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE) {
				m_iCS_REMAIN_MSEC	= 0;
				MsgOutput(iIndex, lMsg.Get(3207));		// "상태변경 성공 - 공성종료"
			}
			else {
				MsgOutput(iIndex, lMsg.Get(3208));		// "상태변경 실패 - 공성종료"
			}
		}
		return;
	case CS_GMCMD_SETSTATE_GETOWNERGUILD :						// "/공성현재수성"
		{
			if (!strcmp(m_szCastleOwnerGuild, "")) {
				MsgOutput(iIndex, lMsg.Get(3209));				// "현재 수성측 없음"
			}
			else {
				MsgOutput(iIndex, lMsg.Get(3210), m_szCastleOwnerGuild); // "현재 수성측 길드  - %s"
			}
		}
		return;
	case CS_GMCMD_SETSTATE_GETCASTLESTATE :						// "/공성현재상태"
		{
			if (m_bDoRun == FALSE) {
				MsgOutput(iIndex, lMsg.Get(3211));	// "현재 공성상태 - 알 수 없음"
			}
			else {
				switch(m_iCastleSiegeState) {
				case CASTLESIEGE_STATE_NONE :			// 상태없음
					MsgOutput(iIndex, lMsg.Get(3211));	// "현재 공성상태 - 알 수 없음"
					break;
				case CASTLESIEGE_STATE_IDLE_1 :			// 유휴기간 - 1
					MsgOutput(iIndex, lMsg.Get(3212));	// "현재 공성상태 - 공성 주기 직전 (시작않됨)"
					break;
				case CASTLESIEGE_STATE_REGSIEGE :		// 신청기간	(공성주기 시작)
					MsgOutput(iIndex, lMsg.Get(3213), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "현재 공성상태 - 공성전 등록기간 : %d시간:%d분:%d초 남음"
					break;
				case CASTLESIEGE_STATE_IDLE_2 :			// 유휴기간 - 2
					MsgOutput(iIndex, lMsg.Get(3214), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "현재 공성상태 - 공성전 등록직후 (휴식기간) : %d시간:%d분:%d초 남음
					break;
				case CASTLESIEGE_STATE_REGMARK :		// 문장 등록기간
					MsgOutput(iIndex, lMsg.Get(3215), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "현재 공성상태 - 문장 등록기간 : %d시간:%d분:%d초 남음"
					break;
				case CASTLESIEGE_STATE_IDLE_3 :			// 유휴기간 - 3
					MsgOutput(iIndex, lMsg.Get(3216), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "현재 공성상태 - 문장 등록직후  (휴식기간) : %d시간:%d분:%d초 남음"
					break;
				case CASTLESIEGE_STATE_NOTIFY :			// 발표기간
					MsgOutput(iIndex, lMsg.Get(3217), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "현재 공성상태 - 공성전 참여길드 발표기간 : %d시간:%d분:%d초 남음"
					break;
				case CASTLESIEGE_STATE_READYSIEGE :		// 준비기간
					MsgOutput(iIndex, lMsg.Get(3218), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "현재 공성상태 - 공성전 준비기간 : %d시간:%d분:%d초 남음"
					break;
				case CASTLESIEGE_STATE_STARTSIEGE :		// 공성시작
					MsgOutput(iIndex, lMsg.Get(3219), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "현재 공성상태 - 공성전 전쟁 중 : %d시간:%d분:%d초 남음"
					break;
				case CASTLESIEGE_STATE_ENDSIEGE :		// 휴전기간
					MsgOutput(iIndex, lMsg.Get(3220), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "현재 공성상태 - 공성전 전쟁종료 (휴식기간) : %d시간:%d분:%d초 남음"
					break;
				case CASTLESIEGE_STATE_ENDCYCLE :		// 공성주기 종료
					MsgOutput(iIndex, lMsg.Get(3221));
					break;	// "현재 공성상태 - 공성전 주기종료"
				default :
					MsgOutput(iIndex, lMsg.Get(3211));
					break;	// "현재 공성상태 - 알 수 없음"
				}
			}
		}
		return;
	case CS_GMCMD_SETSTATE_CHANGECSSIDE :						// "/공수교체"
		{
			LPSTR lpszAttackGuild	= (LPSTR) lpParam;
			if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE) {
				MsgOutput(iIndex, lMsg.Get(3222));		// "공수교체 실패"
				return;
			}
			if (lpszAttackGuild == NULL) {
				MsgOutput(iIndex, lMsg.Get(3222));		// "공수교체 실패"
				return;
			}
			if (!strcmp(lpszAttackGuild, "")) {
				MsgOutput(iIndex, lMsg.Get(3222));		// "공수교체 실패"
				return;
			}

			BYTE btCsJoinSide		= CS_SIEGESIDE_NONE;
			bool bGuildInvolved		= FALSE;
			BOOL bRETVAL			= GetCsJoinSide(lpszAttackGuild, btCsJoinSide, bGuildInvolved);
			
			if (bRETVAL == TRUE) {
				if (btCsJoinSide > CS_SIEGESIDE_DEFEND && bGuildInvolved == true) {
					// 공격측이고 주체인 길드라면 공수교체
					LogAddTD("[CastleSiege] Castle Winner Has been Changed - GM (GUILD:%s)", lpszAttackGuild);

					// 일단 메시지를 뿌려준다.
					NotifyCrownState(2);

					// 여기서 성공 처리를 한다.
					memset(m_szMiddleWinnerGuild, 0, sizeof(m_szMiddleWinnerGuild));
					memcpy(m_szMiddleWinnerGuild, lpszAttackGuild, 8);

					// 중간에 삭제해야 할 NPC들을 정리해 준다. (라이프 스톤 등)
					ClearNonDbNPC_MidWin ();

					// 공/수 양측의 상태를 전환한다.
					ChangeWinnerGuild (btCsJoinSide);

					// 공성맵 안의 사람들에게 왕의 직인이 등록 되었음을 알린다. -> 공/수 전환됨
					NotifyAllUserCsProgState (1, lpszAttackGuild);

					// NPC 접근 상태들을 초기화 한다.
					ResetCrownAccessTickCount();
					ResetCrownUserIndex();
					SetCrownAccessUserX(0);
					SetCrownAccessUserY(0);
					ResetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH1);
					ResetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH2);
					m_bRegCrownAvailable	= FALSE;

					// 공성측만 성밖으로 리스폰
					ReSpawnEnemyUser();

					// 현재 공성시간을 다시 보내주어야 한다 -> 클라이언트가 지우므로
					GCAnsCsLeftTimeAlarm (
						(m_iCS_REMAIN_MSEC/1000)/3600,			// 시간
						((m_iCS_REMAIN_MSEC/1000)%3600)/60		// 분
						);
					m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();
				}

				MsgOutput(iIndex, lMsg.Get(3223));	//	"공수교체 성공"
			}
			else {
				MsgOutput(iIndex, lMsg.Get(3222));	//	"공수교체 실패"

				
			}
		}
		return;
	}
}

VOID CCastleSiege::CreateDbNPC_INS			(							// 성문, 석상 등 DB NPC를 DB 저장 없이 바로 만든다.
	)
{
	// 모든 DB NPC들을 제거한다.
	ClearDbNPC ();

	// 다시 모두 생성한다.
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it		= m_vtNpcData.begin();

	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData				= ((CS_NPC_DATA)(*it));
		if (pNpcData.m_bNPC_DBSAVE			== TRUE) {
			(*it).SetBaseValue();
			(*it).m_iNPC_LIVE				= NPC_LIVE_READY;
			(*it).m_iNPC_SIDE				= CS_SIEGESIDE_DEFEND;	// DB의 NPC데이터는 무조건 수성
			(*it).m_iNPC_OBJINDEX			= -1;
			(*it).m_iCS_GATE_LEVER_INDEX	= -1;					// 레버는 무조건 않만들어진 것으로 한다.
		}

		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	m_bDbNpcCreated	= FALSE;
}
#endif


#ifdef CASTLE_SAVE_PCROOM_USERLIST_20050105						// 공성전 참여자들의 PC방 정보 저장

VOID CCastleSiege::SavePcRoomUserList		()
{
	if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE)
		return;

#ifdef FOR_KOREA	// 한국만 해당 됨
	for (INT iUSER = ALLOC_USEROBJECTSTART; iUSER<MAX_OBJECT; iUSER++) {
		if (gObjIsConnected(iUSER)) {
			if (gObj[iUSER].m_btCsJoinSide > 0) {
#ifdef ADD_PROTOCOL_PCBANG_GUID
				g_LogCsPcRoomUserList.Output("%s\t%s\t%d",
					gObj[iUSER].AccountID,
					gObj[iUSER].Name,
					gObj[iUSER].PcBangGuid
					);
#else
				g_LogCsPcRoomUserList.Output("%s\t%s",
					gObj[iUSER].AccountID,
					gObj[iUSER].Name
					);
#endif
			}
		}
	}
#endif	
}

#endif


#endif

