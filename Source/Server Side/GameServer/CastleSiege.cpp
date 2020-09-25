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
CLogToFile	g_LogCsPcRoomUserList ("CS_PCROOM_USER_LOG", ".\\CS_PCROOM_USER_LOG");		// �α����� ��ü
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

	m_iCS_GATE_LEVER_INDEX	= -1;		// ���� ������ �ε����� ���� �ѹ��� �ʱ�ȭ (���� NPC�� �ѹ��� �����Ǿ�� �ϹǷ�)
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


VOID _CS_NPC_DATA::SetBaseValue			()				// �⺻ �������� ������ ����
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
	// ������������ ����
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

#if TESTSERVER == 1						// �׽�Ʈ ��� �� ���� ����� ������ ������
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

	// *> . ��ü ���� ����ڵ��� ��/�� �Ӽ� ����
	ResetAllUserCsJoinSide ();
}


BOOL CCastleSiege::Ready					(INT iMapSvrGroup)
{
	// ������ �߿� �����Ͱ� �ʱ�ȭ�Ǹ� �ʵȴ�.


	// ������ �� �������� 0 ���� �۾Ƽ��� �ʵȴ�.
	if (iMapSvrGroup < 0) {
		return FALSE;
	}

	// ���� ������ �ʱ�ȭ
	Clear();
	ClearAllNPC();			// DB NPC�� �����͵� �� �ʱ�ȭ �ǹǷ� �̷��� �ص� �������.

	m_iMapSvrGroup			= iMapSvrGroup;
	m_iCastleDataLoadState	= CASTLESIEGE_DATALOAD_1;	// ���⼭�� ���� ������ �ε� ��Ȳ�� ������� �������� (�߰��� DB���� �����͸� ���޾Ƽ� ����� ��쵵 �����Ƿ�)

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

	// 1 . �����͵��� �а� ��ȿ�� �˻縦 �Ѵ�.
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
		// �����Ͽ��� ����� ���� ������ ���� ���� ��¥�� �� �ʴٸ� �߸��� ���̴�. -> ���� �ֱⰡ ���� ��¥���� �ʴ°��� �󸶵��� ���εȴ�.
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
		// ������ ���� Ư������ ���� ��¥ ���� ���� ��¥�� �� �ʴٸ� �߸��� ���̴�. -> ���� Ư������ ���۳�¥�� �ʴ°��� ���� ���Ǹ� �� ������ �ð��� ������ �� ���� ���°� �ȴ�.
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

		// �ӽ� !!! -> ���߿� ���Խ��Ѿ� �� <*>
		if (m_iFixCastleSpecificState == CASTLESIEGE_STATE_ENDSIEGE) {
			// �������°� �����Ⱓ�� �Ǹ� �� �� ���´� ������ ������ �ǹǷ� ���� �� ���´� ���� �� ����. !!!
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

	// 2 . ���� �����͵��� ���Ͽ� �����Ǹ� �ʵǹǷ� �ʱ�ȭ �Ѵ�.
	WritePrivateProfileString("GameServerInfo", "CastleCycleStartYear",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleCycleStartMonth",	"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleCycleStartDay",		"0", lpszFileName);

	WritePrivateProfileString("GameServerInfo", "CastleSpecificState",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartYear",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartMonth",	"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartDay",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartHour",		"0", lpszFileName);
	WritePrivateProfileString("GameServerInfo", "CastleStateStartMinute",	"0", lpszFileName);

	// ������ ���� �����͵��� �Ŀ� ���� �������� ������ �� ������ ��ġ�� �ȴ�.
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

	// ���ο� ���� ���� (������, NPC) �����͵��� �д´�.
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

	int							iCountDfUpgrade_Gate	= 0;	// ī��Ʈ - ���� ���׷��̵� ������ (����)
	int							iCountDfUpgrade_Statue	= 0;	// ī��Ʈ - ���� ���׷��̵� ������ (����)
	int							iCountHpUpgrade_Gate	= 0;	// ī��Ʈ - ü�� ���׷��̵� ������ (����)
	int							iCountHpUpgrade_Statue	= 0;	// ī��Ʈ - ü�� ���׷��̵� ������ (����)
	int							iCountRgUpgrade_Statue	= 0;	// ī��Ʈ - ȸ���� ���׷��̵� ������ (����)
	int							iCountDfValue_Gate		= 0;	// ī��Ʈ - ���� ���׷��̵� ���� �� ����Ǵ� ���� ���� (����)
	int							iCountDfValue_Statue	= 0;	// ī��Ʈ - ���� ���׷��̵� ���� �� ����Ǵ� ���� ���� (����)

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{
			if( type == 0 )				// ���� �ֱ� ����
			{	
				// ������ ���� �Ϲ� �����͸� �о���δ�.
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				INT iCastleSiegeCycle						= (int)TokenNumber;

				// ����üũ
				if (iCastleSiegeCycle <= 0) {
					LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::LoadData() - iCastleSiegeCycle <= 0 : %d",
						iCastleSiegeCycle
						);
					return FALSE;
				}

				m_iCastleSiegeCycle							= iCastleSiegeCycle;
			}
			else if( type == 1 )		// ���� �⺻ ������ ������
			{	
				// ������ ������ ������ �о���δ�.
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

				// ����üũ
				if (!vtScheduleData.empty()) {
					vector <CS_SCHEDULE_DATA>::iterator it	= vtScheduleData.begin();
					for (; it != vtScheduleData.end() ; it++) {
						CS_SCHEDULE_DATA pScheduleData	= (*it);
						if (pScheduleData.m_bIN_USE) {
							if (pScheduleData.m_iSTATE == iSTATE) {
								// 1> . ���� ���°� �����ϴ��� Ȯ��
								LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::LoadData() - Same State Exist : %d",
									iSTATE
									);
								return FALSE;
							}
							else if (pScheduleData.m_iSTATE > iSTATE) {
								// 2. ���°��� ũ�� ��¥�� �ڿ��� �Ѵ�.
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
								// 3 . ���°��� ������ ��¥�� ���̾�� �Ѵ�.
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
			else if( type == 2 )		// �� ��/���� NPC ������
			{	
				// NPC ������ �о���δ�.
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


				// ����üũ
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
			else if( type == 3 )		//  ���� ���׷��̵� ������ (����)
			{
				INT iLEVEL		= 0;	// ���׷��̵� ����
				INT	iGEMCOUNT	= 0;	// �ʿ� ��ȣ���� ����
				INT iZEN		= 0;	// �ʿ� ��

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
			else if( type == 4 )		// ���� ���׷��̵� ������ (����)
			{
				INT iLEVEL		= 0;	// ���׷��̵� ����
				INT	iGEMCOUNT	= 0;	// �ʿ� ��ȣ���� ����
				INT iZEN		= 0;	// �ʿ� ��

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
			else if( type == 5 )		// ü�� ���׷��̵� ������ (����)
			{
				INT iHP			= 0;	// ���׷��̵� ü��
				INT	iGEMCOUNT	= 0;	// �ʿ� ��ȣ���� ����
				INT iZEN		= 0;	// �ʿ� ��

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
			else if( type == 6 )		// ü�� ���׷��̵� ������ (����)
			{
				INT iHP			= 0;	// ���׷��̵� ü��
				INT	iGEMCOUNT	= 0;	// �ʿ� ��ȣ���� ����
				INT iZEN		= 0;	// �ʿ� ��

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
			else if( type == 7 )		// ȸ���� ���׷��̵� ������ (����)
			{
				INT iLEVEL		= 0;	// ���׷��̵� ����
				INT	iGEMCOUNT	= 0;	// �ʿ� ��ȣ���� ����
				INT iZEN		= 0;	// �ʿ� ��

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
			else if( type == 8 )		// ���� ���׷��̵� ���� �� ����Ǵ� ���� ���� (����)
			{
				INT iDFVALUE	= 0;	// ���� ��ġ
			
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iDFVALUE							= (int)TokenNumber;

				if (iCountDfValue_Gate >= MAX_UPGRADE_LEVEL+1)
					break;

				g_iNpcDefense_CGATE[iCountDfValue_Gate]				= iDFVALUE;

				iCountDfValue_Gate++;
			}
			else if( type == 9 )		// ���� ���׷��̵� ���� �� ����Ǵ� ���� ���� (����)
			{
				INT iDFVALUE	= 0;	// ���� ��ġ
			
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

	// ���� ������ �ʱ�ȭ
//	Clear();
				
	// ������ ���� -> 1 . ������
	EnterCriticalSection (&m_critScheduleData);
	if (!vtScheduleData.empty()) {
		vector <CS_SCHEDULE_DATA>::iterator it	= vtScheduleData.begin();
		
		for (; it != vtScheduleData.end() ; it++) {
			m_vtScheduleData.push_back((*it));
		}
	}
	// ������� �����Ű�� ���� ����
	sort(m_vtScheduleData.begin(), m_vtScheduleData.end(), ScheduleStateCompFunc);
	// ���� ���¿��� �ð� ������ �̸� ����Ѵ�. (SEC)
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
				// �ֱ��� �� ������ ���̴�.
				m_vtScheduleData[iIDX].m_iGAP_SEC		= 0;
			}
		}
	}
	else {
		// �����ֱ� ������ ������ �ϳ��� ����.
		// �ӽ� !!!
	}
	LeaveCriticalSection (&m_critScheduleData);
				
	// ������ ���� - 2 . NPC ����
	EnterCriticalSection (&m_critNpcData);
	if (!vtNpcData.empty()) {
		vector <CS_NPC_DATA>::iterator it	= vtNpcData.begin();
		for (; it != vtNpcData.end() ; it++) {
			m_vtNpcData.push_back((*it));
		}
	}
	LeaveCriticalSection (&m_critNpcData);

	//-----------<Ȯ�ο��׽�Ʈ>-----------
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
	//-----------<Ȯ�ο��׽�Ʈ>-----------

	m_bFileDataLoadOK					= TRUE;

	return TRUE;
}


BOOL CCastleSiege::DataRequest			()
{
	// ������ ���� �����Ͱ� �� �������� ��쿡�� �ʱ�ȭ �� �� ����.
	if (m_bFileDataLoadOK == FALSE) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::DataRequest() - m_bFileDataLoadOK == FALSE");
		return FALSE;
	}

	// ������ �� �������� 0 ���� �۾Ƽ��� �ʵȴ�.
	if (m_iMapSvrGroup < 0) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::DataRequest() - m_iMapSvrGroup < 0");
		return FALSE;
	}

	if (m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::DataRequest() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2 (%d)", m_iCastleDataLoadState);
		return FALSE;
	}

	// ������ �ʱ�ȭ �����͸� �䱸
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
	m_iCastleDataLoadState	= CASTLESIEGE_DATALOAD_NONE;	// ������ �ε� ���¸� �����·� ������.

	// ������ ������ ������ �����Ѵ�.
	CreateAllCsGateLever();

	// ������ �ð��� �����Ѵ�.
	if (CheckSync() == FALSE) {
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
		MsgBox("[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
		return FALSE;
	}

	// ���� ������ ������ ������ �����ص� �ȴ�.
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


VOID CCastleSiege::SetState					(INT iCastleSiegeState, BOOL bSetRemainMsec)				// ������ �����췯�� Ư�� ���·� �����Ѵ�.
{
	if ((iCastleSiegeState < CASTLESIEGE_STATE_NONE) || (iCastleSiegeState > CASTLESIEGE_STATE_ENDCYCLE))
		return;

	m_iCastleSiegeState						= iCastleSiegeState;

	switch(m_iCastleSiegeState) {
	case CASTLESIEGE_STATE_NONE :			// ���¾���
		SetState_NONE						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_IDLE_1 :			// ���ޱⰣ - 1
		SetState_IDLE_1						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_REGSIEGE :		// ��û�Ⱓ	(�����ֱ� ����)
		SetState_REGSIEGE					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_IDLE_2 :			// ���ޱⰣ - 2
		SetState_IDLE_2						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_REGMARK :		// ���� ��ϱⰣ
		SetState_REGMARK					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_IDLE_3 :			// ���ޱⰣ - 3
		SetState_IDLE_3						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_NOTIFY :			// ��ǥ�Ⱓ
		SetState_NOTIFY						(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_READYSIEGE :		// �غ�Ⱓ
		SetState_READYSIEGE					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_STARTSIEGE :		// ��������
		SetState_STARTSIEGE					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_ENDSIEGE :		// �����Ⱓ
		SetState_ENDSIEGE					(bSetRemainMsec);
		break;
	case CASTLESIEGE_STATE_ENDCYCLE :		// �����ֱ� ����
		SetState_ENDCYCLE					(bSetRemainMsec);
		break;
	}

	// *> . ���°� �ٲ� �� ���� �� �ϵ�
	if (m_bDbNpcCreated == FALSE) {
		// ���� DB NPC���� �������� �ʾҴٸ� ���⼭ ���ش�.
		m_bDbNpcCreated			= TRUE;
		CreateDbNPC();
	}

	// ���°� �ٲ� �ð��� ���� ���۽ð����� �����Ѵ�.
	GetLocalTime(&m_tmStateStartDate);

	m_dwEVENT_MSG_TICK_COUNT				= 0;
	m_dwCHECK_GATE_ALIVE_COUNT				= 0;
}


VOID CCastleSiege::Run						()
{
	if (m_bDoRun == FALSE)					// �����͸� ���� �� ���� ���� �ʴ´�.
		return;

	if (m_bCastleSiegeEnable == FALSE)		// �������� ��Ȱ��ȭ �� ��� ���� �ʴ´�.
		return;

	switch(m_iCastleSiegeState) {
	case CASTLESIEGE_STATE_NONE :			// ���¾���
		ProcState_NONE						();
		break;
	case CASTLESIEGE_STATE_IDLE_1 :			// ���ޱⰣ - 1
		ProcState_IDLE_1					();
		break;
	case CASTLESIEGE_STATE_REGSIEGE :		// ��û�Ⱓ	(�����ֱ� ����)
		ProcState_REGSIEGE					();
		break;
	case CASTLESIEGE_STATE_IDLE_2 :			// ���ޱⰣ - 2
		ProcState_IDLE_2					();
		break;
	case CASTLESIEGE_STATE_REGMARK :		// ���� ��ϱⰣ
		ProcState_REGMARK					();
		break;
	case CASTLESIEGE_STATE_IDLE_3 :			// ���ޱⰣ - 3
		ProcState_IDLE_3					();
		break;
	case CASTLESIEGE_STATE_NOTIFY :			// ��ǥ�Ⱓ
		ProcState_NOTIFY					();
		break;
	case CASTLESIEGE_STATE_READYSIEGE :		// �غ�Ⱓ
		ProcState_READYSIEGE				();
		break;
	case CASTLESIEGE_STATE_STARTSIEGE :		// ��������
		ProcState_STARTSIEGE				();
		break;
	case CASTLESIEGE_STATE_ENDSIEGE :		// �����Ⱓ
		ProcState_ENDSIEGE					();
		break;
	case CASTLESIEGE_STATE_ENDCYCLE :		// �����ֱ� ����
		ProcState_ENDCYCLE					();
		break;
	}

	// ����� �� ���� üũ�� �͵�

	// 1 . ���� �ð����� NPC ������ �����Ѵ�.
	if (GetTickCount() - m_dwNPC_DBSAVE_TICK_COUNT > MAX_NPC_DBSAVE_TICK) {
		if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE) {
			StoreDbNpc();
		}
		m_dwNPC_DBSAVE_TICK_COUNT		= GetTickCount();
	}

	// 2 . ���� �ð����� ������ ���� ���۽ð��� ����Ѵ�.
	if (GetTickCount() - m_dwCALC_LEFTSIEGE_TICK_COUNT > MAX_CALC_LEFTSIEGE_TICK) {
		m_dwCALC_LEFTSIEGE_TICK_COUNT	= GetTickCount();
		CalcCastleLeftSiegeDate();
	}

	// �ӽ� !!!
	// �׽�Ʈ�� - ���� ������ ���� ���¸� ���
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
		// ���⼭�� ������ ���� ���� ���۽ð������� ������ ����Ѵ�.
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
	
	// ���� �ֱⰡ ���� ������ �� �� �ϵ�

	// 1 . ������ �ʿ��� �����͵��� �ʱ�ȭ �Ѵ�.
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

	// 1. ��/���� ��������� �����Ѵ�.
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

	// 1 . ��/���� ��������� �����Ѵ�.
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

	// ������ ���� ������ �� �۾���

	// �ӽ� !!! -> ���߿� �����ؾ� �� <*>
	// ��/���� ��������� �����Ѵ�.
//	CheckBuildCsGuildInfo();

	// *> . ���� ���� ���� �ʱ�ȭ
	m_bCastleTowerAccessable			= FALSE;
	m_bRegCrownAvailable				= FALSE;
	m_iCastleCrownAccessUser			= -1;		// �հ� �� ������ �����
	m_btCastleCrownAccessUserX			= 0;
	m_btCastleCrownAccessUserY			= 0;
	m_iCastleSwitchAccessUser1			= -1;		// �հ� ����ġ 1 �� ������ �����
	m_iCastleSwitchAccessUser2			= -1;		// �հ� ����ġ 2 �� ������ �����
	m_dwCrownAccessTime					= 0;		// �հ� ���ٽð� �ʱ�ȭ
	m_dwCHECK_GATE_ALIVE_COUNT			= 0;
	m_dwCS_STARTTIME_TICK_COUNT			= 0;
	m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();		// �������� �� �ٷ� ���� �ð��� �������� �ʵ���
	m_dwCS_JOINSIDE_REFRESH_TICK_COUNT	= GetTickCount();		// �������� �� �ٷ� ��/�� �������¸� �������� �ʵ���
	m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT	= 0;
	m_dwCS_MINIMAP_SEND_TICK_COUNT		= GetTickCount();
	memset (m_szMiddleWinnerGuild, 0, sizeof(m_szMiddleWinnerGuild));

	// �ӽ� !!! -> ���߿� �߰��ؾ� �� <*>
	// *> . ������ ������ �����ϴ��� Ȯ���Ѵ�. (���� ���±��� ������� ������ ���� �Ϸ�) -> ������ �������� ����
	if (CheckAttackGuildExist() == FALSE) {
		CheckCastleSiegeResult();
		SetState (CASTLESIEGE_STATE_ENDSIEGE);
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::SetState_STARTSIEGE() - CheckAttackGuildExist() == FALSE");
		return;
	}

	// 1 . DB NPC�� (����, ����)�� ������ DB�� �����Ѵ�.
	StoreDbNpc ();

	// 2 . ������ ���� NPC�� (�߼���, ĳ��Ÿ��, �� ���) �� ����, ����
	ClearNonDbNPC();
	CheckReviveNonDbNPC ();

	// 3 . ���� ���� ��ȣ������ �ִٸ� ����
#ifdef REMOVE_CASTLESIEGE_AUTO_REVIVE_GUARDIANSTATUE_20051101
	if (CheckGuardianStatueExist() == FALSE) 
	{
		// ��� ������ ���ŵǾ���. -> ��� ���� �����ϰ� �������� ���� �ö� �� �ֵ��� �Ѵ�.
		ClearCastleTowerBarrier();
		SetCastleTowerAccessable(TRUE);
	}
#else
	CheckReviveGuardianStatue ();
#endif

	// 4 . ���� ��ü�� (����, ����) ���� �ڽ��� ����, ȸ���� ������ �����Ŵ
	AdjustDbNpcLevel ();

	// 5 . ��� �����ϴ� �������� ���� ���·� ����
	SetAllCastleGateState (CS_GATE_STATE_CLOSED);

	// 6 . ��ü ����ڵ��� ��/�� �����Ӽ��� ���� ã�Ƽ� ������ (��/�� ����)
	SetAllUserCsJoinSide ();

#ifdef CASTLE_SAVE_PCROOM_USERLIST_20050105
	// *> . ��/�� �Ӽ��� ������ ���Ŀ� PC�� ������ �����Ѵ�.
	SavePcRoomUserList ();
#endif

	// 7 . �������� �������� ������
	ReSpawnEnemyUser(TRUE);

	// 7 . ��/�� �׸��� �׿��� ������� ������ ��ҷ� ������
//	ReSpawnAllUser ();

	// 8 . ��ü ����� �鿡�� �������� ���۵Ǿ����� �˸�
	NotifyAllUserCsStartState (1);
	
	// 9 . ������ ������ �ð��� �����ش�.
//	GCAnsCsLeftTimeAlarm (
//		(m_iCS_REMAIN_MSEC/1000)/3600,			// �ð�
//		((m_iCS_REMAIN_MSEC/1000)%3600)/60		// ��
//		);
}


VOID CCastleSiege::SetState_ENDSIEGE		(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_ENDSIEGE()");

	if (bSetRemainMsec) {
#ifdef CASTLE_TEST_SCHEDULER_20041218			// �׽�Ʈ �����췯 ���� ��
		m_iCS_REMAIN_MSEC					= CS_TEST_GAPSEC * 1000;
#else
		// ���⼭�� �����ֱ� ���ᳯ¥������ ���� �ð��� �ٽ� ����Ͽ� �Է��Ѵ�.
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

	// ������ ���� ���� -> ���� �Ⱓ ������ �� �۾���

	// 1 . ������ ���� NPC���� �Ҹ� (DB NPC ���� - ����, ����, ��������)
	ClearNonDbNPC ();

	// 2 . ��ü ���� ����ڵ��� ��/�� �Ӽ� ����
	ResetAllUserCsJoinSide ();

	// 3 . ������ ���� �޽��� ���
	SendAllUserAnyMsg(lMsg.Get(1620));		// "�������� ����Ǿ����ϴ�."
}


VOID CCastleSiege::SetState_ENDCYCLE		(BOOL bSetRemainMsec)
{
	LogAddTD("SET SetState_ENDCYCLE()");

	// �����ֱ��� ���̹Ƿ� �ʿ��� ������ �۾��� ����ģ �� ���� ���·� �Ѿ�� �Ѵ�.

	// 1 . ���� ���۽ð�, ���ð��� �����ϰ� DB�� �����Ѵ�. -> ���� ��¥�� ���۽ð��� �ȴ�.
	SYSTEMTIME tmNowDate;
	GetLocalTime(&tmNowDate);
	tmNowDate.wHour			= 0;
	tmNowDate.wMinute		= 0;
	tmNowDate.wSecond		= 0;
	tmNowDate.wMilliseconds	= 0;

#ifdef MODIFY_SIEGE_END_DATE_BUG_FIX_20060116
	// ���� ����� ���� �������� ���������� ���� ������ ���۽ð����� �����Ѵ�.
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
		-1,							// �ε����� �ʿ����
		m_tmStartDate.wYear,		
		m_tmStartDate.wMonth,		
		m_tmStartDate.wDay, 
		m_tmEndDate.wYear, 
		m_tmEndDate.wMonth, 
		m_tmEndDate.wDay
		);


	// 1 . ������ ������û ��� ����Ʈ�� �ʱ�ȭ ��
	GS_GDReqResetRegSiegeInfo (m_iMapSvrGroup);

	// 2 . ���� ������� ����Ʈ�� �ʱ�ȭ ��
	GS_GDReqResetSiegeGuildInfo (m_iMapSvrGroup);
	
	// 3 . �����ֱⰡ ������ ���� ���� �����ֱ⸦ ����
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
			SendAllUserAnyMsg(lMsg.Get(1614));		// "������ ���� ��û �Ⱓ �Դϴ�."
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
			SendAllUserAnyMsg(lMsg.Get(1615));		// "������ ���� ��û ���� ������ ǥ���� ����� �ּ���."
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
			SendAllUserAnyMsg(lMsg.Get(1616));		// "�������� ������ ��带 �����ϴ� �Ⱓ�Դϴ�."
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
			SendAllUserAnyMsg(lMsg.Get(1617));		// "�������� ������ ������ �����Ǿ����ϴ�."
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
			SendAllUserAnyMsg(lMsg.Get(1618));			// "������ �غ� �Ⱓ�Դϴ�. �������� �����Ͻô� �����鲲���� �������� �غ����ֽñ� �ٶ��ϴ�."
			m_dwEVENT_MSG_TICK_COUNT	= GetTickCount();
		}

#ifdef MU_CASTLESIEGE_START_NOTIFY_20050418
		// ������ ���� �� ���� �ð��� N�� ������ ���� M�� ���� �����ð� ����
		if (m_iCS_REMAIN_MSEC <= CS_STARTNOTIFY_MIN_TICK) {
			if (GetTickCount() - m_dwCS_STARTTIME_TICK_COUNT > CS_STARTNOTIFY_PER_TICK) {
				CHAR szText[256] = {0,};
				wsprintf(szText, lMsg.Get(1724), m_iCS_REMAIN_MSEC/60000 + 1);		// "������ ���� %d�� �� �Դϴ�."
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
#ifdef MODIFY_CASTLE_SIEGE_CYCLE_RESET_20071009	// �������� �߰� �¸�üũ�� 1�ʿ� 1�� -> 1�ʿ� 10������ �ٲ۴�.
	// ���� �������� �߰� �¸��� ��� üũ�Ѵ�.
	CheckMiddleWinnerGuild ();
#endif
	
	INT iTICK_MSEC = (GetTickCount() - m_iCS_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_iCS_REMAIN_MSEC							-= iTICK_MSEC;
		m_iCS_TICK_COUNT							= GetTickCount();

#ifndef MODIFY_CASTLE_SIEGE_CYCLE_RESET_20071009
		// ���� �������� �߰� �¸��� ��� üũ�Ѵ�.
		CheckMiddleWinnerGuild ();
#endif		

		// ������ �׾��ִ��� üũ�ϰ� �Ӽ��� ������
		if (GetTickCount() - m_dwCHECK_GATE_ALIVE_COUNT > CS_CHECK_DBNPC_ALIVE_TICK) {
			CheckCsDbNpcAlive ();
			m_dwCHECK_GATE_ALIVE_COUNT				= GetTickCount();
		}

		// ������ ���� �ð��� ����ڵ鿡�� ������ �ִ� �ð�
		if (GetTickCount() - m_dwCS_LEFTTIME_TICK_COUNT > DEFAULT_CS_LEFT_ALARM_TICK) {
			if (m_iCS_REMAIN_MSEC > DEFAULT_CS_LEFT_ALARM_TICK) {
				// N�� �̻� ������ ���� ���� (N�� ���ϴ� 1�о� ���� ����)
				GCAnsCsLeftTimeAlarm (
					(m_iCS_REMAIN_MSEC/1000)/3600,			// �ð�
					((m_iCS_REMAIN_MSEC/1000)%3600)/60		// ��
					);
				m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();
			}
		}

		// ������ ���� �ð��� N�� ������ ���� M�� ���� �����ð� ����
		if (m_iCS_REMAIN_MSEC <= DEFAULT_CS_LEFT_ALARM_TICK) {
			if (GetTickCount() - m_dwCS_LEFTTIME_TICK_COUNT > DEFAULT_CS_LEFT_ALARM_TICK_U10) {
				GCAnsCsLeftTimeAlarm (
					(m_iCS_REMAIN_MSEC/1000)/3600,			// �ð�
					((m_iCS_REMAIN_MSEC/1000)%3600)/60		// ��
					);
				m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();
			}
		}

		// ������ �ʹݿ� ����ڵ��� �������� ���¸� �������ִ� �ð� (�ʹ� N�� ���� ��������)
		if (m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT < MAX_CS_JOINSIDE_REFRESH_COUNT) {
			if (GetTickCount() - m_dwCS_JOINSIDE_REFRESH_TICK_COUNT > CS_JOINSIDE_REFRESH_TICK) {
				SetAllUserCsJoinSide ();
				m_dwCS_JOINSIDE_REFRESH_TICK_COUNT	= GetTickCount();
				m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT++;
			}
		}

		// �̴ϸ� �����͸� ��� ���� �����ش�.
		if (GetTickCount() - m_dwCS_MINIMAP_SEND_TICK_COUNT > CS_MINIMAP_SEND_TICK) {
			OperateMiniMapWork ();
			m_dwCS_MINIMAP_SEND_TICK_COUNT			= GetTickCount();
		}

		if (GetTickCount() - m_dwEVENT_MSG_TICK_COUNT > CS_MSG_TICK_STARTSIEGE) {
			SendAllUserAnyMsg(lMsg.Get(1619));		// "�������� ���� ���Դϴ�."
			m_dwEVENT_MSG_TICK_COUNT				= GetTickCount();
		}

		LogAddC(LOGC_GREEN, "RUN ProcState_STARTSIEGE()	LEFT-MSEC:%d", m_iCS_REMAIN_MSEC);
	}
	
	if (m_iCS_REMAIN_MSEC <= 0) {
		// �������� �����Ͽ���.

		// 1 . ���� �¸���带 üũ�Ѵ�.
		if (CheckCastleSiegeResult() == TRUE) {
			// ���� ������ �ٲ���ٸ� ������ ���� ������ ��� �����Ѵ�.
			GS_GDReqResetCastleTaxInfo (m_iMapSvrGroup);
		}

		// 2 . �������� �ƴ� ����ڵ��� �� ������ �ű��.
		ReSpawnEnemyUser();

		// 3 . ���� ���� �������¸� DB�� �����Ѵ�.
		if (m_btIsCastleOccupied == TRUE) {
			GS_GDReqCastleOwnerChange (m_iMapSvrGroup, m_btIsCastleOccupied, m_szCastleOwnerGuild);
		}

		// 4 . �������� ����Ǿ����� DB�� �����Ѵ�.
		GS_GDReqSiegeEndedChange (m_iMapSvrGroup, TRUE);

		// 5 . ������ NPC���¸� DB�� �����Ѵ�.
		StoreDbNpc ();

		// 6 . ��ü ����� �鿡�� �������� �������� �˸�
		NotifyAllUserCsStartState (0);
		
		// 7 . ���� ������� ����Ʈ�� �ʱ�ȭ �� (���� ����� ���������� ȸ���ϱ� ����)
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
	// ��� �� �κ��� �������� ���� -> SetState_ENDCYCLE() ���� CASTLESIEGE_STATE_IDLE_1 ���·� �ٷ� �̵���Ŵ
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


// DB NPC���� ���ʷ� ���� -> DB�� ����
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
			(*it).m_iNPC_SIDE				= CS_SIEGESIDE_DEFEND;			// DB�� NPC�����ʹ� ������ ����

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
	// iSTATE�� 9�ϰ��(��������) ������κ��� ���� �Ǿ�� �� ��¥�� ����Ѵ�. - hnine
	// GetNextDay()�� �̿��� ���� �������� ��¥ ���
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
				// ���;ȿ� �����ϴ� NPC��� ����ִ� ���·� �ٲپ��ְ� �����͵� DB�� �°� �����Ѵ�.
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
				(*it).m_iNPC_SIDE			= CS_SIEGESIDE_DEFEND;			// DB�� NPC�����ʹ� ������ ����
				bExist						= TRUE;
				break;				
			}
			
			it++;
		}
		LeaveCriticalSection (&m_critNpcData);

		if (bExist == FALSE) {
			// ������� �Դٸ� NPC ���� ���� �ȿ��� ���°��̴�. -> ���� �߰��Ѵ�.
			CS_NPC_DATA pNpcData2;
			pNpcData2.m_bIN_USE				= TRUE;
			pNpcData2.m_iNPC_NUM			= lpMsg[iNPC_COUNT].iNpcNumber;
			pNpcData2.m_iNPC_INDEX			= lpMsg[iNPC_COUNT].iNpcIndex;
			pNpcData2.m_iNPC_LIVE			= NPC_LIVE_READY;
			pNpcData2.m_iNPC_SIDE			= CS_SIEGESIDE_DEFEND;			// DB�� NPC�����ʹ� ������ ����

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


BOOL CCastleSiege::CheckSync				()								// ���� �������� �ð��� ���� �ð��� �������� ���� �ð� ��ũ�� �����. (���� ��Ȳ ����)
{
	if (!m_bFileDataLoadOK) {
		// ��������� ��ũ ������ ���� �����͸� �����־ �����ϴ�.
		LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::CheckSync() - m_bFileDataLoadOK == FALSE");
		return FALSE;
	}

	m_iCS_TICK_COUNT						= GetTickCount();

	SYSTEMTIME tmToDay;
	GetLocalTime(&tmToDay);
	INT iTODAY_DATE_NUM						= MAKELONG(MAKEWORD(tmToDay.wDay, tmToDay.wMonth), tmToDay.wYear);

	INT iCastleInitState					= CASTLESIEGE_STATE_IDLE_1;

	// 1 . �����ֱ� ���� �ð��� ���� �Է� �Ǿ��°� ?
	if (m_bFixCastleCycleStartDate) {
		// �������� �ð��� �Է� �� ���� �׻� �� �ֱ� �����ȿ� ���� �ð��� �����ϰų� �ٰ��� �ֱ����� �����Ѵ�.
		m_tmStartDate.wYear					= m_tmFixCastleCycleStartDate.wYear;
		m_tmStartDate.wMonth				= m_tmFixCastleCycleStartDate.wMonth;
		m_tmStartDate.wDay					= m_tmFixCastleCycleStartDate.wDay;

		m_tmEndDate							= m_tmStartDate;
		GetNextDay(m_tmEndDate, m_iCastleSiegeCycle);
	}

	// 2 . ���� �ð��� �����ֱ� ���� �ð����� �ʴ��� Ȯ�� -> ������ �������� (�����ֱ� ���� �ð��� ������ �Էµ��� �ʰ� DB�� ���� ��� ���⼭ �ɷ���)
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

	// *> . ���� üũ���� �����ߴٸ� �����ֱⰡ ����Ǿ��� ��� �ٷ� DB�� �����Ѵ�.
	if (m_bFixCastleCycleStartDate) {
		GS_GDReqSiegeDateChange (
			m_iMapSvrGroup,				
			-1,							// �ε����� �ʿ����
			m_tmStartDate.wYear,		
			m_tmStartDate.wMonth,		
			m_tmStartDate.wDay, 
			m_tmEndDate.wYear, 
			m_tmEndDate.wMonth, 
			m_tmEndDate.wDay
			);
	}

	// 3 . ���� Ư������ ���� �ð��� ���� �Է� �Ǿ��°� ?
	if (m_bFixCastleStateStartDate) {
		// ���� Ư������ �ð��� �Է� �� ���� �׻� ���� �ð����� ������ ���� �����Ѵ�.
		// ���� Ư�����´� �Է� �� ���� �׻� ���� ���¹��� �ȿ� �ִ°��� �����Ѵ�.

		// 1> . Ư�����°� �����ϴ����� üũ
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

		// �ӽ� !!! -> ���� ����ð��� �����ֱ� ����ð����� ������ ���� ???

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

			// ����ð����� Ư������ ���� �������� ��� �ð��� �� �� �����̴�.
			m_iCastleSiegeState				= m_iFixCastleSpecificState	- 1;
			SetState(m_iCastleSiegeState, FALSE);
			return TRUE;
		}
		else {
			// ���°� ���ٸ� ������ ������ (����) �� ���µ��� ���Ի��¿� �ʸ°ų� �߸��Ǿ��� �� �ִ�.
			MsgBox("[CastleSiege] CCastleSiege::CheckSync() - FIXED STATE:%d NOT FOUND IN SCHEDULE (P.S.> Check File 'MuCastleData.dat', 'commonserver.cfg')",
				m_iFixCastleSpecificState
				);
//			return FALSE;
		}

		// ������� �Դٸ� ������ ���̴�. -> �׳� �������� üũ�� �Ѿ��.
		m_bFixCastleStateStartDate			= FALSE;
	}

	// 4 . �������� �̹� ġ������°� ? -> �����ٸ� ������ CASTLESIEGE_STATE_ENDSIEGE (��������) ���·� ����.
	if (m_btIsSiegeEnded) {
		SetState(CASTLESIEGE_STATE_ENDSIEGE);
		return TRUE;
	}

	// 5 . ���� �������� ����Ѵ�. -> ���� �ð��� ���� ������ ����κп� ���ϴ��� ã�Ƴ���.
	INT iEVENT_START_DATE_NUM				= MAKELONG(MAKEWORD(m_tmStartDate.wDay, m_tmStartDate.wMonth), m_tmStartDate.wYear);

	if (iEVENT_START_DATE_NUM > iTODAY_DATE_NUM) {
		// �����ֱ� ���� ���̴�.
		SetState(CASTLESIEGE_STATE_IDLE_1);		// �� ���´� ���� �� �˾Ƽ� ���� �ð��� ����Ѵ�. (������ ���Ի��µ��� ����� �������� ���־�� ��)
		return TRUE;
	}
	else {
		// �����ֱ� �߰��� �ִ�.
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
			// 2���� üũ�Ͽ��ٸ� MuCastleData.dat ������ �߸��Ǳ� ������ ������� �� ���� �Ƹ��� ���� ���̴�.
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

		// ���� �ð��� ����Ͽ� ���� �Է��ϰ� �������� �����Ѵ�.
		// 1> . ���� ������ ��Ȯ�� ��¥�� ���
		SYSTEMTIME tmSchduleDate			= m_tmStartDate;
		GetNextDay(tmSchduleDate, pScheData_RR.m_iADD_DAY);
		tmSchduleDate.wHour					= pScheData_RR.m_iADD_HOUR;
		tmSchduleDate.wMinute				= pScheData_RR.m_iADD_MIN;
//		GetNextDay(tmSchduleDate, pScheData_RR.m_iADD_DAY, pScheData_RR.m_iADD_HOUR, pScheData_RR.m_iADD_MIN);
		
		// 2> . ���� ��¥���� ���̸� ���ؼ� MSEC������ ����
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


		// �ӽ� !!! -> ���߿� ���Խ��Ѿ� ��  <*>
		// Ư���� ��Ȳ�� ���� ����ó���� �Ѵ�.
		if (m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE) {
			// ���� ���� ���°� �������� �̶�� ������ �����غ� 30�� ������ ���߾�� �Ѵ�.
			m_iCastleSiegeState		= CASTLESIEGE_STATE_READYSIEGE;
			m_iCS_REMAIN_MSEC		= CS_MIN_SIEGESTART * 60 * 1000;
		}
		else if (m_iCastleSiegeState == CASTLESIEGE_STATE_READYSIEGE) {
			if (m_iCS_REMAIN_MSEC < CS_MIN_SIEGESTART * 60 * 1000) {
				// ���� ���� ���°� �����غ� 30�� �̳���� ������ �����غ� 30�� ������ ���߾�� �Ѵ�.
				m_iCastleSiegeState		= CASTLESIEGE_STATE_READYSIEGE;
				m_iCS_REMAIN_MSEC		= CS_MIN_SIEGESTART * 60 * 1000;
			}
		}
		
		// ���������� ���¸� �����Ѵ�.
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

INT CCastleSiege::GetStateGapSec			(INT iCastleSiegeState)			// Ư�� ������ ���� ���±����� �ð����� (SEC) �� ���Ѵ�.
{
	INT iGAP_SEC								= -1;
#ifdef CASTLE_TEST_SCHEDULER_20041218			// �׽�Ʈ �����췯 ���� ��
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
		// �������� ���� �������� �ʴ´�.
		return CASTLESIEGE_LEFTTIME_NOTRUNNING;
	}

	if (m_bDoRun == FALSE) {
		// �������� ���� �������� �ʴ´�.
		return CASTLESIEGE_LEFTTIME_NOTRUNNING;
	}

	if (m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE) {
		// �������� ���� ���̴�.
		return CASTLESIEGE_LEFTTIME_ONSIGE;
	}

	if (m_btIsSiegeEnded == TRUE || m_iCastleSiegeState >= CASTLESIEGE_STATE_ENDSIEGE) {
		// �������� �̹� �����ߴ�.
		return CASTLESIEGE_LEFTTIME_ENDSIEGE;
	}

	// ���⼭ �������� ������������ �ð��� ����ϰ� �ȴ�.
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
		// ���������� �������� ���� ���� �������� ����Ѵ�.
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
		// �������� ���� �������� �ʴ´�.
		return FALSE;
	}

	if (m_bDoRun == FALSE) {
		// �������� ���� �������� �ʴ´�.
		return FALSE;
	}

	// ���� ���۽ð��� ������ ������ ���Ҵ� ���� ����Ѵ�.
	tmStateStartDate	= m_tmStateStartDate;		
	
	// ���� ����ð��� ���� �ð����� ���� �ð� (m_iCS_REMAIN_MSEC) �� ���ؼ� ����Ѵ�.
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
		if (gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE) {					// �����ʿ� �ִ� NPC�鿡 ����
			if (
				gObj[n].Class == 277 ||			// ����
				gObj[n].Class == 283 ||			// ��ȣ����
				gObj[n].Class == 219			// �������� (DB NPC�� �ƴ����� ������ ���� ������)
				)
			{
				if (gObj[n].Class == 277) {
					// ���� ���Žÿ��� �����ڸ��� �Ӽ��� �����ش�.
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
			if ((gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE) &&		// �����ʿ� �ִ� NPC�鿡 ����
				(gObj[n].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
				(gObj[n].m_btCsNpcType != CS_NPC_TYPE_DB_DFN)
				) 
			{
				
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
				if (gObj[n].Class == NPC_INDEX_LIFESTONE) 
				{	
					// ���������� �̶�� ��� ���踦 �����ش�.
					g_CsNPC_LifeStone.DeleteLifeStone(n);
				}
#endif

#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214
				if( gObj[n].Class == NPC_INDEX_SPEARMAN
					|| gObj[n].Class == NPC_INDEX_BOWMAN )
				{
					// �뺴 ���ڸ� ���� ���� Delete �Լ��� ���Ѵ�.
					g_CsNPC_Mercenary.DeleteMercenary(n);
				}
#endif
				
				if (gObj[n].Class == 219) {							// ���� ������ �����ϸ� �ʵȴ�.
					continue;
				}
				
				gObjDel(n);

				// �ӽ� !!!
/*
				if (
//					gObj[n].Class == 224 ||			// ���
//					gObj[n].Class == 283 ||			// �հ�
					gObj[n].Class == 104 ||			// Ʈ��
					gObj[n].Class == 288 ||			// ����Ÿ��
					gObj[n].Class == 221 ||			// ������ (������)
					gObj[n].Class == 222			// ������ (������)
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
			if ((gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE) &&		// �����ʿ� �ִ� NPC�鿡 ����
				(gObj[n].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
				(gObj[n].m_btCsNpcType != CS_NPC_TYPE_DB_DFN)
				) 
			{
				
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
				if (gObj[n].Class == NPC_INDEX_LIFESTONE) {			// ���������� �̶�� ��� ���踦 �����ش�.
					g_CsNPC_LifeStone.DeleteLifeStone(n);
				}
#endif
			
				if (gObj[n].Class == NPC_INDEX_LIFESTONE			// ����������
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
		if (gObj[n].m_btCsNpcType != CS_NPC_TYPE_NONE) {					// ���� ���Ͷ�� Ÿ���� ������ �͵鸸 ��
/*
			if (
//				gObj[n].Class == 224 ||			// ���
//				gObj[n].Class == 283 ||			// �հ�
//				gObj[n].Class == 104 ||			// Ʈ��
				gObj[n].Class == 277 ||			// ����
				gObj[n].Class == 283 ||			// ��ȣ����
				gObj[n].Class == 288 ||			// ����Ÿ��
				gObj[n].Class == 221 ||			// ������ (������)
				gObj[n].Class == 222			// ������ (������)
				)
			{
				gObjDel(n);
			}
*/
			// �ӽ� !!!
			gObjDel(n);
		}
	}
}


VOID CCastleSiege::CreateDbNPC				()								// DB�� �����ϴ� NPC���� ��� ����
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
			// ���;ȿ� �����ϴ� NPC��� ����ִ� ���·� �ٲپ��ְ� �����͵� DB�� �°� �����Ѵ�.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;				// ������ �����ǰų� ���� ������ ���� NPC���� �ƴϴ�.
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
				gObj[iNPC_INDEX].m_btCsNpcType		= CS_NPC_TYPE_DB_DFN;		// ������ DB ����

				INT iNPC_DF_LEVEL					= pNpcData.m_iNPC_DF_LEVEL;	// ���� ����
				if (iNPC_DF_LEVEL < 0)						
					iNPC_DF_LEVEL					= 0;
				if (iNPC_DF_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_DF_LEVEL					= MAX_UPGRADE_LEVEL;
				INT iNPC_RG_LEVEL					= pNpcData.m_iNPC_RG_LEVEL;	// ȸ���� ����
				if (iNPC_RG_LEVEL < 0)						
					iNPC_RG_LEVEL					= 0;
				if (iNPC_RG_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_RG_LEVEL					= MAX_UPGRADE_LEVEL;
				
				// ȸ����, ���� ���� ��ġ�� �ʿ��ϴٸ� ���⼭ �������ش�. -> �ܺο��� (AdjustDbNpcLevel() - ���� ������)
				switch(pNpcData.m_iNPC_NUM) {
				case 277 :	// ����
					gObj[iNPC_INDEX].m_Defense					= g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_MagicDefense				= g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel			= iNPC_DF_LEVEL;
					break;
				case 283 :	// ����
					gObj[iNPC_INDEX].m_Defense					= g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_MagicDefense				= g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel			= iNPC_DF_LEVEL;
					gObj[iNPC_INDEX].m_btCsNpcRgLevel			= iNPC_RG_LEVEL;
					break;
				}

				// ���߿� ���� Ȯ���� �ϱ����� ���⿡ �������� �ִ´�.
				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;

				if (pNpcData.m_iNPC_NUM == 277) {
					// �ӽ� !!! -> ���߿� �����ؾ� ��
					// ���� ���� ���Ŀ��� ������ ���� ������ �Ѵ�.
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_OPENED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[iNPC_INDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObj[iNPC_INDEX].m_btCsGateOpen	= CS_GATE_STATE_OPENED;
					// �ӽ� !!! -> ���߿� �߰��ؾ� ��
					// ���� ���� ���Ŀ��� ������ ���� ������ �Ѵ�.
//					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_CLOSED);
//					gObj[iNPC_INDEX].m_btCsGateOpen	= CS_GATE_STATE_CLOSED;

					// ������ ������ ���⼭ �����Ѵ�.
					BOOL bCreateLever		= FALSE;
					if (pNpcData.m_iCS_GATE_LEVER_INDEX == -1) {
						// ������� ������ ����.
						bCreateLever		= TRUE;
					}
					else {
						// ������� ������ �ִ�.
						if (CheckLeverAlive(pNpcData.m_iCS_GATE_LEVER_INDEX) == FALSE) {
							bCreateLever	= TRUE;
						}
					}

					if (bCreateLever) {
						// ������ ���� �����.
						INT iLeverIndex	= CreateCsGateLever(pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY + 4);
						if (CHECK_LIMIT(iLeverIndex, MAX_OBJECT)) {
							// ���� ���� ����
							(*it).m_iCS_GATE_LEVER_INDEX	= iLeverIndex;							
							// ������� ������ �ε����� �����Ѵ�.
							BOOL bRETVAL	= LinkCsGateLever(iLeverIndex, iNPC_INDEX);
							if (bRETVAL == FALSE) {
								LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
									pNpcData.m_iNPC_INDEX
									);
							}
						}
						else {
							// ���� ���� ����
							(*it).m_iCS_GATE_LEVER_INDEX	= -1;
							LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								pNpcData.m_iNPC_INDEX
								);
						}
					}
					else {
						// ������� ������ �ε����� �����Ѵ�.
						BOOL bRETVAL	= LinkCsGateLever(pNpcData.m_iCS_GATE_LEVER_INDEX, iNPC_INDEX);
						if (bRETVAL == FALSE) {
							LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								pNpcData.m_iNPC_INDEX
								);
						}
					}
				}

				// ���������� �� NPC�� �����Ǿ����� �˸���.
				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


VOID CCastleSiege::CreateNonDbNPC					(BOOL bDelFirst)		// DB�� �������� �ʴ� NPC���� ��� ���� (���� ���� ���Ƿ� �ѹ� ������ ����)
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
			// ���;ȿ� �����ϴ� NPC��� ����ִ� ���·� �ٲپ��ְ� �����͵� DB�� �°� �����Ѵ�.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;						// ������ �����ǰų� ���� ������ ���� NPC���� �ƴϴ�.
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

				// ���߿� ���� Ȯ���� �ϱ����� ���⿡ �������� �ִ´�.
				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;

				// ���������� �� NPC�� �����Ǿ����� �˸���.
				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


BOOL CCastleSiege::CheckAddDbNPC					(						// DB�� �����ϴ� NPC�� �ϳ��� �߰� �������� üũ -> ���� �� DB���� �� (������ ���� �ƴҶ� ��)
	INT iIndex,
	INT iNpcType, 
	INT iNpcIndex,
	BYTE &btResult
	)
{
	// ���� NPC ���� ��Ͽ��� �Ʒ��� ���µ��� �ϴ� Ȯ��
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
					// ���� ��������Ƿ� �ٽ� �߰��� �� ����.
					btResult	= 0;
					iRETVAL		= FALSE;
					break;
				}
			}

			// ���� �׾������Ƿ� DB������ �߰��� �� �ִ��� ���θ� �����Ѵ�.
			
			// ���Ժ���� ����Ѵ�.
			INT iBuyCost	= 0;
			switch(pNpcData.m_iNPC_NUM) {
			case 277 :	// ����
				iBuyCost	= pNpcData.m_iNPC_BASE_MAXHP * 5;
				break;
			case 283 :	// ��ȣ����
				iBuyCost	= pNpcData.m_iNPC_BASE_MAXHP * 3;
				break;
			}

			// ���Ժ���� �ִ��� Ȯ���Ѵ�.
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


BOOL CCastleSiege::AddDbNPC					(								// DB�� �����ϴ� NPC�� �ϳ��� �߰� -> ���� �߿��� ����� �ʵ�
	INT iNpcType, 
	INT iNpcIndex
	)
{
	// ���� NPC ���� ��Ͽ��� �Ʒ��� ���µ��� �ϴ� Ȯ��
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
					// ���� ��������Ƿ� �ٽ� �߰��� �� ����. -> ����
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

			// ���� ���ӻ� �߰��ϰ� �߰��ߴٴ� ǥ�ø� �Ѵ�.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;				// ������ �����ǰų� ���� ������ ���� NPC���� �ƴϴ�.
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
#ifdef MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406	// �ʱⰪ���� ����
				gObj[iNPC_INDEX].Life				= pNpcData.m_iNPC_BASE_HP;
				gObj[iNPC_INDEX].MaxLife			= pNpcData.m_iNPC_BASE_MAXHP;
#else
				gObj[iNPC_INDEX].Life				= pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife			= pNpcData.m_iNPC_MAXHP;
#endif
				gObj[iNPC_INDEX].m_btCsNpcType		= CS_NPC_TYPE_DB_DFN;					// ������ DB ����

				INT iNPC_DF_LEVEL					= pNpcData.m_iNPC_BASE_DF_LEVEL;		// ���� �ʱ��� ���� ����
				if (iNPC_DF_LEVEL < 0)						
					iNPC_DF_LEVEL					= 0;
				if (iNPC_DF_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_DF_LEVEL					= MAX_UPGRADE_LEVEL;
				INT iNPC_RG_LEVEL					= pNpcData.m_iNPC_BASE_RG_LEVEL;		// ���� �ʱ��� ȸ���� ����
				if (iNPC_RG_LEVEL < 0)						
					iNPC_RG_LEVEL					= 0;
				if (iNPC_RG_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_RG_LEVEL					= MAX_UPGRADE_LEVEL;

				// ȸ����, ���� ���� ��ġ�� �ʿ��ϴٸ� ���⼭ �������ش�.
				switch(pNpcData.m_iNPC_NUM) {
				case 277 :	// ����
					gObj[iNPC_INDEX].m_Defense		= g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel			= iNPC_DF_LEVEL;			
					break;
				case 283 :	// ����
					gObj[iNPC_INDEX].m_Defense		= g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel			= iNPC_DF_LEVEL;
					gObj[iNPC_INDEX].m_btCsNpcRgLevel			= iNPC_RG_LEVEL;
					break;
				}

				// ���߿� ���� Ȯ���� �ϱ����� ���⿡ �������� �ִ´�.
				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;

				if (pNpcData.m_iNPC_NUM == 277) {
					// �ӽ� !!! -> ���߿� �����ؾ� ��
					// ���� ���� ���Ŀ��� ������ ���� ������ �Ѵ�.
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_OPENED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[iNPC_INDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObj[iNPC_INDEX].m_btCsGateOpen	= CS_GATE_STATE_OPENED;
					// �ӽ� !!! -> ���߿� �߰��ؾ� ��
					// ���� ���� ���Ŀ��� ������ ���� ������ �Ѵ�.
//						SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_CLOSED);
//						gObj[iNPC_INDEX].m_btCsGateOpen	= CS_GATE_STATE_CLOSED;

					// ������ ������ ���⼭ �����Ѵ�.
					BOOL bCreateLever		= FALSE;
					if (pNpcData.m_iCS_GATE_LEVER_INDEX == -1) {
						// ������� ������ ����.
						bCreateLever		= TRUE;
					}
					else {
						// ������� ������ �ִ�.
						if (CheckLeverAlive(pNpcData.m_iCS_GATE_LEVER_INDEX) == FALSE) {
							bCreateLever	= TRUE;
						}
					}

					if (bCreateLever) {
						// ������ ���� �����.
						INT iLeverIndex	= CreateCsGateLever(pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY + 4);
						if (CHECK_LIMIT(iLeverIndex, MAX_OBJECT)) {
							// ���� ���� ����
							(*it).m_iCS_GATE_LEVER_INDEX	= iLeverIndex;
							// ������� ������ �ε����� �����Ѵ�.
							BOOL bRETVAL	= LinkCsGateLever(iLeverIndex, iNPC_INDEX);
							if (bRETVAL == FALSE) {
								LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
									pNpcData.m_iNPC_INDEX
									);
							}
						}
						else {
							// ���� ���� ����
							(*it).m_iCS_GATE_LEVER_INDEX	= -1;
							LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								pNpcData.m_iNPC_INDEX
								);
						}
					}
					else {
						// ������� ������ �ε����� �����Ѵ�.
						BOOL bRETVAL	= LinkCsGateLever(pNpcData.m_iCS_GATE_LEVER_INDEX, iNPC_INDEX);
						if (bRETVAL == FALSE) {
							LogAddC(LOGC_RED,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								pNpcData.m_iNPC_INDEX
								);
						}
					}
				}
				
				// ���������� �� NPC�� �����Ǿ����� �˸���.
				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
				(*it).m_bIN_USE						= TRUE;

				// Ŭ���̾�Ʈ�� ����� �˸���. -> �ܺο���
				iRETVAL								= TRUE;
				
#ifdef MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406
				// �����̳� ������ ���� �α׸� �����.
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


BOOL CCastleSiege::DelNPC					(								// DB�� �����ϴ� NPC�� �ϳ��� ���� -> ���� �� DB���� �� (������ ���� �ƴҶ� ��)
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
				// OBJECTSTRUCT ����ü�� iIndex �� ���� �ܺο��� �ʱ�ȭ ���־�� �Ѵ�.

#ifdef MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406
				// ������ ������ ��� �⺻������ �ʱ�ȭ �� �ش�.
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
					// ���� ���Žÿ��� �����ڸ��� �Ӽ��� �����ش�.
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, CS_GATE_STATE_OPENED);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[pNpcData.m_iNPC_OBJINDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128

#ifdef MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406
					// ���� ������ ���� �α׸� �����. �� �� ���� ���¿� ���ؼ��� ����Ѵ�.
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
					// �������� ���� ���� ���� ���� ���� �� -> ���� �Ⱓ ��
					if (m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE) {
						if (CheckGuardianStatueExist() == FALSE) {
							// ��� ������ ���ŵǾ���. -> ��� ���� �����ϰ� �������� ���� �ö� �� �ֵ��� �Ѵ�.
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
		// NPC�� ���������� ��������.
		if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE) {
			// ������ ���� �ƴ϶�� ��� �ش� NPC�� DB���� �����Ѵ�.
			GS_GDReqNpcRemove (m_iMapSvrGroup, pTempNpcData.m_iNPC_NUM, pTempNpcData.m_iNPC_INDEX);
		}
	}

	return TRUE;
}


BOOL CCastleSiege::RepairDbNPC				(								// DB�� �����ϴ� NPC�� �ϳ��� ����
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
				
				// OBJECTSTRUCT ����ü�� iIndex �� ���� ã�Ƽ� �����Ѵٸ� ü���� �����Ѵ�.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].Class == iNpcType) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// �̰ͱ��� Ȯ���ϸ� �´� NPC�̴�.
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
BOOL CCastleSiege::PayForUpgradeDbNpc		(					// DB�� �����ϴ� NPC�� �ϳ��� ���׷��̵��ϱ� ���� �ʿ��� ����� �����Ѵ�.
	INT iIndex,
	INT	iNpcType,
	INT	iNpcIndex,
	INT	iNpcUpType,
	INT	iNpcUpValue,
	INT	iNpcUpIndex
	)
{
	// DS�� NPC ���׷��̵带 ��û�ϱ� �� �̸� �ʿ��� ����� �����Ѵ�.

	INT iNEED_GEMOFDEFEND	= 0;		// �ʿ��� ��ȣ����
	INT iNEED_MONEY			= 0;		// �ʿ��� ��

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
	case 277 :		// ����
		{
			// ���׷��̵� �ϰ��� �ϴ� ������ �ش� �ܰ谡 �����ϴ��� �����Ѵ�.
			switch(iNpcUpType) 
			{
			case CS_UPGRADE_TYPE_DEFENSE :			// ���׷��̵� - ����
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
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// ���׷��̵� - �ִ�HP
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
					// �ش� Ÿ���� ���� - ����
					GCAnsNpcUpgrade( iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
					return FALSE;
				}
				break;
			}
		}
		break;
	case 283 :		// ��ȣ����
		{
			// ���׷��̵� �ϰ��� �ϴ� ������ �ش� �ܰ谡 �����ϴ��� �����Ѵ�.
			switch( iNpcUpType ) 
			{
			case CS_UPGRADE_TYPE_DEFENSE :			// ���׷��̵� - ����
				{
					// ���׷��̵忡 ���� ��ȿ���� �˻��Ѵ�.
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
			case CS_UPGRADE_TYPE_REGENARATION :		// ���׷��̵� - ȸ����
				{
					// ���׷��̵忡 ���� ��ȿ���� �˻��Ѵ�.
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
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// ���׷��̵� - �ִ�HP
				{
					// ������ HP�� ��ܰ����� Ȯ�� -> ������ HP���� ���Ǻ񱳷� Ư�� ������ ������Ŵ
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
					// �ش� Ÿ���� ���� - ����
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

BOOL CCastleSiege::UpgradeDbNPC				(					// DB�� �����ϴ� NPC�� �ϳ��� ���׷��̵� -> �� ���� �̹� DB���� ���׷��̵� ��
	INT iIndex,
	INT	iNpcType,
	INT	iNpcIndex,
	INT	iNpcUpType,
	INT	iNpcUpValue,
	INT	iNpcUpIndex
	)
{
	// ���⼭ ������ NPC ���׷��̵带 �ǽ��Ѵ�.
	
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
	case 277 :		// ����
		{
			// ���׷��̵� �ϰ��� �ϴ� ������ �ش� �ܰ谡 �����ϴ��� �����Ѵ�.
			switch( iNpcUpType ) 
			{
			case CS_UPGRADE_TYPE_DEFENSE :			// ���׷��̵� - ����
				{
					// ���⼭ ���� ������ ������ ���׷��̵� ��
					UpgradeDbNPC_DFLEVEL( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// ���׷��̵� - �ִ�HP
				{					
					// ���⼭ ���� ������ �ִ� HP�� ���׷��̵� ��
					UpgradeDbNPC_MAXHP( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			default :
				{
					// �ش� Ÿ���� ���� - ����
					GCAnsNpcUpgrade( iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue );
					return FALSE;
				}
				break;
			}
		}
		break;
	case 283 :		// ��ȣ����
		{
			// ���׷��̵� �ϰ��� �ϴ� ������ �ش� �ܰ谡 �����ϴ��� �����Ѵ�.
			switch(iNpcUpType) 
			{
			case CS_UPGRADE_TYPE_DEFENSE :			// ���׷��̵� - ����
				{
					// ���⼭ ������ �ִ� ������ ���׷��̵� ��
					UpgradeDbNPC_DFLEVEL( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			case CS_UPGRADE_TYPE_REGENARATION :		// ���׷��̵� - ȸ����
				{					
					// ���⼭ ���� ������ ȸ������ ���׷��̵� ��
					UpgradeDbNPC_RGLEVEL( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// ���׷��̵� - �ִ�HP
				{
					// ���⼭ ���� ������ �ִ� HP�� ���׷��̵� ��
					UpgradeDbNPC_MAXHP( iNpcType, iNpcIndex, iNpcUpValue );
				}
				break;
			default :
				{
					// �ش� Ÿ���� ���� - ����
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
BOOL CCastleSiege::UpgradeDbNPC				(								// DB�� �����ϴ� NPC�� �ϳ��� ���׷��̵� -> �� ���� �̹� DB���� ���׷��̵� ��
	INT iIndex,
	INT	iNpcType,
	INT	iNpcIndex,
	INT	iNpcUpType,
	INT	iNpcUpValue,
	INT	iNpcUpIndex
	)
{
	// ���⼭ ������ NPC ���׷��̵带 �ǽ��Ѵ�.

	INT iNEED_GEMOFDEFEND	= 0;		// �ʿ��� ��ȣ����
	INT iNEED_MONEY			= 0;		// �ʿ��� ��

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
	case 277 :		// ����
		{
			// ���׷��̵� �ϰ��� �ϴ� ������ �ش� �ܰ谡 �����ϴ��� �����Ѵ�.
			switch(iNpcUpType) {
			case CS_UPGRADE_TYPE_DEFENSE :			// ���׷��̵� - ����
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

					// ���⼭ ���� ������ ������ ���׷��̵� ��
					UpgradeDbNPC_DFLEVEL(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// ���׷��̵� - �ִ�HP
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
					
					// ���⼭ ���� ������ �ִ� HP�� ���׷��̵� ��
					UpgradeDbNPC_MAXHP(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			default :
				{
					// �ش� Ÿ���� ���� - ����
					GCAnsNpcUpgrade (iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
					return FALSE;
				}
				break;
			}
		}
		break;
	case 283 :		// ��ȣ����
		{
			// ���׷��̵� �ϰ��� �ϴ� ������ �ش� �ܰ谡 �����ϴ��� �����Ѵ�.
			switch(iNpcUpType) {
			case CS_UPGRADE_TYPE_DEFENSE :			// ���׷��̵� - ����
				{
					// ���׷��̵忡 ���� ��ȿ���� �˻��Ѵ�.
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

					// ���⼭ ������ �ִ� ������ ���׷��̵� ��
					UpgradeDbNPC_DFLEVEL(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			case CS_UPGRADE_TYPE_REGENARATION :		// ���׷��̵� - ȸ����
				{
					// ���׷��̵忡 ���� ��ȿ���� �˻��Ѵ�.
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

					// ���⼭ ���� ������ ȸ������ ���׷��̵� ��
					UpgradeDbNPC_RGLEVEL(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			case CS_UPGRADE_TYPE_MAXHITPOINT :		// ���׷��̵� - �ִ�HP
				{
					// ������ HP�� ��ܰ����� Ȯ�� -> ������ HP���� ���Ǻ񱳷� Ư�� ������ ������Ŵ
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

					// ���⼭ ���� ������ �ִ� HP�� ���׷��̵� ��
					UpgradeDbNPC_MAXHP(iNpcType, iNpcIndex, iNpcUpValue);
				}
				break;
			default :
				{
					// �ش� Ÿ���� ���� - ����
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

BOOL CCastleSiege::UpgradeDbNPC_DFLEVEL		(								// DB�� �����ϴ� Ư�� NPC�� ���� ���׷��̵�
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
				// OBJECTSTRUCT ����ü�� iIndex �� ���� ã�Ƽ� ���� �����ϴ��� Ȯ���Ѵ�.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// �̰ͱ��� Ȯ���ϸ� �´� NPC�̴�.
						(*it).m_iNPC_DF_LEVEL				= iNpcDfLevel;
						bEXIST								= TRUE;

						// ���⼭ OBJECTSTRUCT ��ü�� ���� ������ �÷��ش�.
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


BOOL CCastleSiege::UpgradeDbNPC_RGLEVEL		(								// DB�� �����ϴ� Ư�� NPC�� ȸ���� ���׷��̵�
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
				// OBJECTSTRUCT ����ü�� iIndex �� ���� ã�Ƽ� ���� �����ϴ��� Ȯ���Ѵ�.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// �̰ͱ��� Ȯ���ϸ� �´� NPC�̴�.
						(*it).m_iNPC_RG_LEVEL				= iNpcRgLevel;
						bEXIST								= TRUE;

						// ���⼭ OBJECTSTRUCT ��ü�� ���� ȸ������ �÷��ش�.
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


BOOL CCastleSiege::UpgradeDbNPC_MAXHP		(								// DB�� �����ϴ� Ư�� NPC�� �ִ�HP ���׷��̵�
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
				// OBJECTSTRUCT ����ü�� iIndex �� ���� ã�Ƽ� ���� �����ϴ��� Ȯ���Ѵ�.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// �̰ͱ��� Ȯ���ϸ� �´� NPC�̴�.
						(*it).m_iNPC_MAXHP			= iNpcMaxHP;
						(*it).m_iNPC_HP				= iNpcMaxHP;
						bEXIST						= TRUE;

						// ���⼭ OBJECTSTRUCT ��ü�� ���� �ִ�HP�� �÷��ش�.
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


VOID CCastleSiege::StoreDbNpc				()				// DB�� �����ϴ� NPC���� �ϰ������� DB�� ���� (���� ������ ����)
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


VOID CCastleSiege::SendNpcStateList			(								// Ư�� ������� Ư�� ������ ���� ���� ����Ʈ�� �����ش�.
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
			
			// OBJECTSTRUCT ����ü�� iIndex �� ���� ã�Ƽ� ���� �����ϴ��� Ȯ���Ѵ�.
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
					// �̰ͱ��� Ȯ���ϸ� �´� NPC�̴�.
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


BOOL CCastleSiege::GetNpcData				(							// ���� �����Ǵ� Ư�� NPC�� ���� ������ ���� (FALSE�� ������ ����)
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
				// OBJECTSTRUCT ����ü�� iIndex �� ���� ã�Ƽ� ���� �����ϴ��� Ȯ���Ѵ�.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// �̰ͱ��� Ȯ���ϸ� �´� NPC�̴�.
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


VOID CCastleSiege::AdjustDbNpcLevel			()							// DB NPC�� ���� �ڷ���� ������ ����� ��ü�� �����Ѵ�. -> ���� ���� ������ �̷��� ���־�� �Ѵ�.
{
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it			= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData		= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_bIN_USE	== TRUE) &&
			(pNpcData.m_iNPC_LIVE > NPC_LIVE_NONE)
			)
		{
			// OBJECTSTRUCT ����ü�� iIndex �� ���� ã�Ƽ� ���� �����ϴ��� Ȯ���Ѵ�.
			INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
			if (gObjIsConnected(iNPC_INDEX)) {
				if ((gObj[iNPC_INDEX].Class == pNpcData.m_iNPC_NUM) &&
					(gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
					(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
					(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
					)
				{
					// �̰ͱ��� Ȯ���ϸ� �´� NPC�̴�.
					switch(gObj[iNPC_INDEX].Class) {
					case 277 :	// ������ ���
						{
							// ���� ����
							INT iDF_LEVEL							= pNpcData.m_iNPC_DF_LEVEL;
							if (iDF_LEVEL < 0)						iDF_LEVEL			= 0;
							if (iDF_LEVEL > MAX_UPGRADE_LEVEL)		iDF_LEVEL			= MAX_UPGRADE_LEVEL;
							gObj[iNPC_INDEX].m_btCsNpcDfLevel		= iDF_LEVEL;
							gObj[iNPC_INDEX].m_Defense				= g_iNpcDefense_CGATE[iDF_LEVEL];
						}
						break;
					case 283 :	// ������ ���
						{
							// ���� ����
							INT iDF_LEVEL							= pNpcData.m_iNPC_DF_LEVEL;
							if (iDF_LEVEL < 0)						iDF_LEVEL			= 0;
							if (iDF_LEVEL > MAX_UPGRADE_LEVEL)		iDF_LEVEL			= MAX_UPGRADE_LEVEL;
							gObj[iNPC_INDEX].m_btCsNpcDfLevel		= iDF_LEVEL;
							gObj[iNPC_INDEX].m_Defense				= g_iNpcDefense_CSTATUE[iDF_LEVEL];

							// ȸ��� ����
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


VOID CCastleSiege::SendCsGateStateViewPort	(							// ���� �� ���¸� ���� ���� �ִ� ����鿡�� ���� ����
	INT iGateIndex,
	BYTE btOperate
	)
{
	if (!gObjIsConnected(iGateIndex))
		return;

	LPOBJECTSTRUCT lpObj	= &gObj[iGateIndex];

	// ����Ʈ �� �ƹ��� ���ٸ� ������.
	if( lpObj->VPCount2 < 1 ) return;

	// ����Ʈ �󿡼� ���� �����ִ� ������� ã�´�.
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


VOID CCastleSiege::CreateAllCsGateLever		()							// ���� ������ ������ ���������� ���� ���Ŀ� ������ �ϰ������� ������ �ش�.
{
	EnterCriticalSection (&m_critNpcData);
	if (!m_vtNpcData.empty()) {
		vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
		for (; it != m_vtNpcData.end() ; it++) {
			CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
			if (pNpcData.m_iNPC_NUM == 277) {
				if (pNpcData.m_iCS_GATE_LEVER_INDEX != -1) {
					if (CheckLeverAlive(pNpcData.m_iCS_GATE_LEVER_INDEX) == TRUE) {
						// ������ �̹� �����Ѵٸ� �Ѿ��.
						continue;
					}
				}

				// ������ ���� �����.
				INT iLeverIndex	= CreateCsGateLever(pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY + 4);
				if (CHECK_LIMIT(iLeverIndex, MAX_OBJECT)) {
					// ���� ���� ����
					(*it).m_iCS_GATE_LEVER_INDEX	= iLeverIndex;							
				}
				else {
					// ���� ���� ����
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


INT	CCastleSiege::CreateCsGateLever			(							// Ư�� ������ ������ ������ ����� �ε����� �����Ѵ�. (���д� -1)
	INT iX, 
	INT iY
	)
{
	if (!CHECK_LIMIT(iX, MAX_TERRAIN_SIZE) || !CHECK_LIMIT(iY, MAX_TERRAIN_SIZE)) {
		return -1;
	}

	// ���;ȿ� �����ϴ� NPC��� ����ִ� ���·� �ٲپ��ְ� �����͵� DB�� �°� �����Ѵ�.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
	INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
	INT iNPC_INDEX = gObjAdd();
#endif
	if( iNPC_INDEX >= 0 )
	{
		gObjSetMonster(iNPC_INDEX, 219);

		gObj[iNPC_INDEX].m_PosNum					= -1;						// ������ �����ǰų� ���� ������ ���� NPC���� �ƴϴ�.
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
		gObj[iNPC_INDEX].m_btCsNpcType				= CS_NPC_TYPE_INS_DFN;		// ������ NPC�� (DB�� ������� ����)

		// ������ ������ �ε����� ������´�.
		gObj[iNPC_INDEX].m_iCsGateLeverLinkIndex	= -1;

		return iNPC_INDEX;
	}
	
	return -1;
}


BOOL CCastleSiege::LinkCsGateLever			(							// �̹� ������ ������� �ִٸ� Ư�� ������ �����Ѵ�.
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

	// ������ ������ �ε����� ������.
	gObj[iLeverIndex].m_iCsGateLeverLinkIndex		= iGateIndex;

	// ������ ������ �ε����� ������.
	gObj[iGateIndex].m_iCsGateLeverLinkIndex		= iLeverIndex;
	
	return TRUE;
}

BOOL CCastleSiege::CheckLeverAlive			(							// Ư�� �ε����� ���������̰� ����ִ����� Ȯ��
	INT iLeverIndex
	)
{
	if (!gObjIsConnected(iLeverIndex) || gObj[iLeverIndex].Class != 219) {
		return FALSE;
	}

	return TRUE;
}


VOID CCastleSiege::CheckCsDbNpcAlive		()							// �������� ����ִ��� Ȯ���Ͽ� �׾��ٸ� ���Ӽ� ����
{
	if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE)
		return;

	EnterCriticalSection (&m_critNpcData);
	if (!m_vtNpcData.empty()) {
		vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
		for (; it != m_vtNpcData.end() ; it++) {
			CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
			if ((pNpcData.m_iNPC_NUM == 277 ||	// ����
				pNpcData.m_iNPC_NUM == 283		// ����
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
					// �ʱ�ȭ
					pNpcData.m_iNPC_HP					= pNpcData.m_iNPC_BASE_HP;
					pNpcData.m_iNPC_MAXHP				= pNpcData.m_iNPC_BASE_MAXHP;				
					pNpcData.m_iNPC_RG_LEVEL			= pNpcData.m_iNPC_BASE_RG_LEVEL;				
					pNpcData.m_iNPC_DF_LEVEL			= pNpcData.m_iNPC_BASE_DF_LEVEL;	
#endif

					switch(pNpcData.m_iNPC_NUM) {
					case 277 :					// ���� - ���Ӽ��� ������ �ش�.
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
					case 283 :					// ���� - �ٸ� ������� ���ŵǾ����� Ȯ���Ͽ� ���� ž �Ա��� �����Ѵ�.
						{
							if (CheckGuardianStatueExist() == FALSE) {
								// ��� ������ ���ŵǾ���. -> ��� ���� �����ϰ� �������� ���� �ö� �� �ֵ��� �Ѵ�.
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


BOOL CCastleSiege::CheckCsGateAlive			(							// Ư�� �ε����� �����̰� ����ִ����� Ȯ��
	INT iGateIndex
	)
{
	if (!gObjIsConnected(iGateIndex) || gObj[iGateIndex].Class != 277) {
		return FALSE;
	}

	return TRUE;
}


BOOL CCastleSiege::DelGemOfDefend			(							// Ư�� ����ڿ��Լ� Ư�� ������ ��ȣ������ �����Ѵ�.
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

	// 1 . �켱 ��ȣ�� ������ ������ŭ �ִ��� Ȯ���Ѵ�.
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
		// ������ ���ڶ�
		return FALSE;
	}

	// 2 . ���⼭ ������ �����Ѵ�.
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
				// �̰��� Ȯ���ϹǷ� �������� �����.
				
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


VOID CCastleSiege::SetTaxRate (													// ������ �����ϰ� �ٸ� ���Ӽ����� ����ȭ �Ѵ�.
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

	// ���⼭ �ش� ������ �� ���Ӽ����� ����ȭ �Ѵ�. -> �� N�ʸ��� �ڵ����� ����ȭ �ȴ�.
	LogAddTD("[CastleSiege] Tax Rate Setted - Type:%d, Rate:%d",
		iTaxType,
		iTaxRate
		);
}


BOOL CCastleSiege::CheckCastleHasMoney		(									// ���� ���� ���� ������ �ִ��� Ȯ���Ѵ�.
	INT64 i64CastleMoney
	)
{
	if (m_i64CastleMoney >= i64CastleMoney)
		return TRUE;

	return FALSE;
}


VOID CCastleSiege::SetCastleMoney			(									// ���� ���� ���� Ư�� �׼��� �����.
	INT64 i64CastleMoney
	)
{
	m_i64CastleMoney		= i64CastleMoney;
}

	
VOID CCastleSiege::ResetCastleTaxInfo		()									// ���� ���� ���������� �ʱ�ȭ �Ѵ�.
{
	m_i64CastleMoney		= 0;
	m_iTaxRateChaos			= 0;
	m_iTaxRateStore			= 0;
	m_iTaxHuntZone			= 0;
}


VOID CCastleSiege::SendCastleStateSync		()									// ���� ���� ���¸� �ʼ����� ��ü�� ����ȭ �Ѵ�.
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


VOID CCastleSiege::ResetCastleCycle			()									// ���� �ֱ⸦ �ٽ� �����ϸ鼭 �ʿ��� �����͵��� �����Ѵ�.
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


VOID CCastleSiege::SetGateBlockState		(									// ������ ��� �Ӽ��� �����Ѵ�.
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


VOID CCastleSiege::CheckBuildCsGuildInfo	()									// �������� ��ü��� ������ �����ϱ� ���� ��Ȳ1 �� 2 �� �����Ѵ�.
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
		// [0x88] ��/�� ����� ���������� �ҷ����� ��û
		GS_GDReqCsLoadTotalGuildInfo(m_iMapSvrGroup);
	}
	else {
		// [0x85] ������ ��弱�� �ĺ��ڷ� ��û
		GS_GDReqCalcRegGuildList(m_iMapSvrGroup);
	}
}


VOID CCastleSiege::SetCalcRegGuildList		(									// ������ �������� �ĺ� �����͸� �����Ѵ�. (���� ������ ������ �ʱ�ȭ)
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
		// ������ ��û ��尡 ����. -> �������� ������ ���� ���� ��� �����Ͱ� ���� ������� �ʴ´�.
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
		// ������ ��û ��尡 ���� -> ��� ������ üũ�ߴٸ� ���� ����δ� �ʿð��̴�.
		return;
	}

	// �ְ� ���������� ������� �ǵ��� �����Ѵ�.
	sort(vtRegGuildScore.begin(), vtRegGuildScore.end(), RegGuildScoreCompFunc);

	// ���� N���� �����Ͽ� �����Ѵ�.
	INT iGUILD_COUNT	= vtRegGuildScore.size();
	if (iGUILD_COUNT > MAX_CS_ATTACK_GUILD) {
		iGUILD_COUNT = MAX_CS_ATTACK_GUILD;
	}
	
	EnterCriticalSection(& m_critCsBasicGuildInfo);
	m_mapCsBasicGuildInfo.clear();
	for (iGCNT = 0 ; iGCNT < iGUILD_COUNT ; iGCNT++) {
		// �� �����ʹ� �ڿ� ������ ������ ��ü �����͸� �����ϴ� �⺻�� �ȴ�.
		CS_TOTAL_GUILD_DATA pTotalGuildData;
		pTotalGuildData.m_bIN_USE			= TRUE;
		pTotalGuildData.m_iCsGuildID		= CS_SIEGESIDE_ATTACK + iGCNT;
		pTotalGuildData.m_bGuildInvolved	= 1;
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
		// ��� ������ ǥ���Ѵ�.
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
		pTotalGuildData.m_iGuildScore		= 0;	// �������� ������ ǥ������ �ʴ´�.
#endif		
		m_mapCsBasicGuildInfo.insert(pair<string, CS_TOTAL_GUILD_DATA> (m_szCastleOwnerGuild, pTotalGuildData));
	}
	LeaveCriticalSection(& m_critCsBasicGuildInfo);

	// ������ ������ �⺻ �����͸� ��ü �����Ϳ� �켱 �־���� ��û ��Ŷ�� �Բ� �ۼ��Ѵ�.
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
			// ���� - �̻�����
			break;
		}
		
		m_mapCsTotalGuildInfo.insert(pair<string, CS_TOTAL_GUILD_DATA>(it->first, it->second));
		string strGuildName								= it->first;
		memcpy(lpMsgSendBody[lpMsgSend->iCount].szGuildName, &strGuildName[0], 8);
		lpMsgSendBody[lpMsgSend->iCount].iCsGuildID		= (it->second).m_iCsGuildID;
		lpMsgSend->iCount++;
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);

	// ��/�� ����� ���������� ��û�Ѵ�.
	m_bCsBasicGuildInfoLoadOK = TRUE;

	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x86, sizeof(CSP_REQ_CSGUILDUNIONINFO) + sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount);
	cDBSMng.Send((char*)lpMsgSend, sizeof(CSP_REQ_CSGUILDUNIONINFO) + sizeof(CSP_CSGUILDUNIONINFO) * lpMsgSend->iCount);
}


bool CCastleSiege::RegGuildScoreCompFunc	(CS_REG_GUILD_DATA & A, CS_REG_GUILD_DATA & B)
{
	// ������������ ����
	if (A.m_i64TotolScore < B.m_i64TotolScore)
		return false;
	return true;
}


VOID CCastleSiege::MakeCsTotalGuildInfo		(							// ��/���� ���ձ�� ������ �޾Ƽ� ���� ��������� �Բ� �Ϻ��� ��������� �����Ѵ�.
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
			// ���ʿ� �Էµ� ���� ����� ��� ������ �ٷ� ���� ����� �������� �ٽ� �������� �ʴ´�.
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

	// ���⼭ ������ ������� ������ DB�� �����Ѵ�.
	SaveCsTotalGuildInfo ();
	// ���⼭ ���������� ��ü ����ڵ��� ��/�� ���������� �ٽ� �����ؾ� �Ѵ�.
	SetAllUserCsJoinSide ();
}


VOID CCastleSiege::SaveCsTotalGuildInfo		()							// ��/���� ��ü��� ������ DB�� �����ϵ��� ��û�Ѵ�.
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
			// ���� - �̻�����
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

	// ��/�� ����� ��ü������ �����Ѵ�.
	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x87, sizeof(CSP_REQ_CSSAVETOTALGUILDINFO) + sizeof(CSP_CSSAVETOTALGUILDINFO) * lpMsgSend->iCount);
	cDBSMng.Send((char*)lpMsgSend, sizeof(CSP_REQ_CSSAVETOTALGUILDINFO) + sizeof(CSP_CSSAVETOTALGUILDINFO) * lpMsgSend->iCount);


	//-----------<Ȯ�ο��׽�Ʈ>-----------
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


VOID CCastleSiege::SetCsTotalGuildInfo		(							// ��/���� ���ձ�� ������ �޾Ƽ� ���� ��������� �Բ� �Ϻ��� ��������� �����Ѵ�.
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

	// ���⼭ ���������� ��ü ����ڵ��� ��/�� ���������� �ٽ� �����ؾ� �Ѵ�.
	SetAllUserCsJoinSide ();

	//-----------<Ȯ�ο��׽�Ʈ>-----------
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
		// �ڽ��� �������¸� �� �ʿ䰡 ���� ��
		return FALSE;
	}

	if (m_bCsTotalGuildInfoLoadOK == FALSE) {
		return FALSE;
	}

	BOOL bRETVAL		= FALSE;

	// �������� ���۵��� �ʾҴٸ� ��ȿ
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


VOID CCastleSiege::ResetAllUserCsJoinSide	()				// �������� ��� ����� ���� ������ ��/�� ���� ���ΰ��� �������ش�.
{
	EnterCriticalSection(& m_critCsTotalGuildInfo);
	for (INT iUSER = ALLOC_USEROBJECTSTART; iUSER<MAX_OBJECT; iUSER++) {
		if (!gObjIsConnected(iUSER))
			continue;
		gObj[iUSER].m_btCsJoinSide				= CS_SIEGESIDE_NONE;
		gObj[iUSER].m_bCsGuildInvolved			= 0;
		gObj[iUSER].m_btLifeStoneCount			= 0;		// ���������� ���Ƚ�� �ʱ�ȭ (apple)
		NotifySelfCsJoinSide (iUSER);
		
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0;
#endif
	}
	LeaveCriticalSection(& m_critCsTotalGuildInfo);
}


VOID CCastleSiege::NotifySelfCsJoinSide		(INT iIndex)	// Ư�� ������� ��/�� ���� ���ΰ��� �����ش�.
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
	// ���� ��� 1, 2, 3�� �����ϱ� ���� �Ʒ��� ���� ó���Ѵ�.
	if( btCsJoinSide == CS_SIEGESIDE_ATTACK )
		pMsg.btCsJoinSide	= gObj[iIndex].m_btCsJoinSide;
	else
#endif
	pMsg.btCsJoinSide	= btCsJoinSide;

	
	DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


BOOL CCastleSiege::OperateGate				(				// �ܺ� ����ڰ�ü �ε����� ������ �۵���Ų��.
	INT		iObjIndex,
	INT		iMonsterExistVal,
	BOOL	bOpenType										// (0:�ݱ�/1:����)
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
				// OBJECTSTRUCT ����ü�� iIndex �� ���� ã�Ƽ� ���� �����ϴ��� Ȯ���Ѵ�.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
//					(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) 
					(iMonsterExistVal == pNpcData.m_iCsNpcExistVal)
					)
				{
					// �̰ͱ��� Ȯ���ϸ� ������ �´�.

					// ������ �Ӽ�, ������ ���� �ٲپ��ش�.
					SetGateBlockState (pNpcData.m_iNPC_SX, pNpcData.m_iNPC_SY, bOpenType);
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( &gObj[iNPC_INDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObj[iNPC_INDEX].m_btCsGateOpen	= bOpenType;
					
					// ������ ���¸� �ֺ��� �˸��� �۾��� �ܺο��� �Ѵ�.
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


BOOL CCastleSiege::OperateGate				(				// ���� NPC���� �ε����� ������ �۵���Ų��.
	INT		iNpcIndex,
	BOOL	bOpenType										// (0:�ݱ�/1:����)
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
				// OBJECTSTRUCT ����ü�� iIndex �� ���� ã�Ƽ� ���� �����ϴ��� Ȯ���Ѵ�.
				INT iNPC_INDEX						= pNpcData.m_iNPC_OBJINDEX;
				if (gObjIsConnected(iNPC_INDEX)) {
					if ((gObj[iNPC_INDEX].m_btCsNpcType != CS_NPC_TYPE_NONE) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal) &&
						(gObj[iNPC_INDEX].m_iCsNpcExistVal != 0)
						)
					{
						// �̰ͱ��� Ȯ���ϸ� ������ �´�.

						// ������ �Ӽ�, ������ ���� �ٲپ��ش�.
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

						// ������ ���¸� �ֺ��� �˸��� �۾��� �ܺο��� �Ѵ�.
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


BOOL CCastleSiege::CheckAttackGuildExist		()							// ������ ��尡 �����ϴ��� Ȯ���Ѵ�.
{
	BOOL iRETVAL	= FALSE;

	EnterCriticalSection(& m_critCsTotalGuildInfo);
	if (!m_mapCsTotalGuildInfo.empty())
		iRETVAL		= TRUE;
	LeaveCriticalSection(& m_critCsTotalGuildInfo);

	return iRETVAL;
}


BOOL CCastleSiege::CheckCastleOwnerMember		(INT iIndex)				// �ش� ����ڰ� ���� ������ ����� �Ҽ����� Ȯ���Ѵ�.
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


BOOL CCastleSiege::CheckCastleOwnerUnionMember	(INT iIndex)				// Ư�� ����ڰ� �ڽ��� ���� ��忬���� �Ͽ����� Ȯ���Ѵ�.
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
			// ���� ������ �Ͽ��̴�.
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
#endif

	return FALSE;
}


BOOL CCastleSiege::CheckGuardianStatueExist		()							// ��ȣ������ �����ϴ��� Ȯ���Ѵ�.
{
	for (INT iIDX = 0 ; iIDX < MAX_MONSTER ; iIDX++) {
		if(gObj[iIDX].Class == 283) {
			if (gObj[iIDX].Connected > 2) {
				if ((gObj[iIDX].Live == 1) && (gObj[iIDX].MapNumber == MAP_INDEX_CASTLESIEGE)) {
					// ��ȣ������ �����ϸ� ��
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


VOID CCastleSiege::ChangeWinnerGuild			(INT iCsJoinSide)			// ������ ��带 �ٲ۴� -> �߰� �¸� �� ���
{
	CHAR szGuildTo [16]		= {0,};
	CHAR szGuildFrom [16]	= {0,};

	EnterCriticalSection(& m_critCsTotalGuildInfo);
	// 1 . ��/�� ����� �ҼӰ��� �ٲ۴�.
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

	// 2 . ��ü ����ڵ��� ������� �ٲ� ��/�� �ҼӰ��� �����Ѵ�.
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
//		NotifySelfCsJoinSide (iUSER);		-> �ܺ��� NotifyAllUserCsProgState() ���� �����


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


BOOL CCastleSiege::CheckMiddleWinnerGuild	()							// ����!!> . ������ �߰��¸� ��尡 ��Ÿ������ Ȯ���Ѵ�.
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
					// �� ��尡 ���� ����̴�.
					LogAddTD("[CastleSiege] Castle Winner Has been Changed (GUILD:%s)", gObj[m_iCastleCrownAccessUser].GuildName);

					// �ϴ� �޽����� �ѷ��ش�.
					
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
					// ���� ����� ���������� ������� �ϴµ� ������ �ٸ������ �õ���(3)���� �Ǿ� �־� ������.
					GCAnsCsAccessCrownState (m_iCastleCrownAccessUser, 1);
#else
					GCAnsCsAccessCrownState (m_iCastleCrownAccessUser, 3);
#endif
					NotifyCrownState(2);

					// ���⼭ ���� ó���� �Ѵ�.
					memset(m_szMiddleWinnerGuild, 0, sizeof(m_szMiddleWinnerGuild));
					memcpy(m_szMiddleWinnerGuild, gObj[m_iCastleCrownAccessUser].GuildName, 8);

					// �߰��� �����ؾ� �� NPC���� ������ �ش�. (������ ���� ��)
					ClearNonDbNPC_MidWin ();

					// ��/�� ������ ���¸� ��ȯ�Ѵ�.
					ChangeWinnerGuild (gObj[m_iCastleCrownAccessUser].m_btCsJoinSide);

					// ������ ���� ����鿡�� ���� ������ ��� �Ǿ����� �˸���. -> ��/�� ��ȯ��
					NotifyAllUserCsProgState (1, gObj[m_iCastleCrownAccessUser].GuildName);

					// NPC ���� ���µ��� �ʱ�ȭ �Ѵ�.
					ResetCrownAccessTickCount();
					ResetCrownUserIndex();
					SetCrownAccessUserX(0);
					SetCrownAccessUserY(0);
					ResetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH1);
					ResetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH2);
					m_bRegCrownAvailable	= FALSE;

					// �������� �������� ������
					ReSpawnEnemyUser();
					
					// ���� �����ð��� �ٽ� �����־�� �Ѵ� -> Ŭ���̾�Ʈ�� ����Ƿ�
					GCAnsCsLeftTimeAlarm (
						(m_iCS_REMAIN_MSEC/1000)/3600,			// �ð�
						((m_iCS_REMAIN_MSEC/1000)%3600)/60		// ��
						);
					m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}


BOOL CCastleSiege::CheckCastleSiegeResult		()							// ����!!> . ������ ��������� üũ�Ѵ�.
{
	CHAR szMsg [256]		= {0,};
	BOOL bRETVAL			= FALSE;

	if ((m_btIsCastleOccupied == 1) && strcmp(m_szCastleOwnerGuild, "")) 
	{
		// ������ ���� ���ɴ��� ���¿���.
		if (!strcmp(m_szMiddleWinnerGuild, "") ||
			!strcmp(m_szMiddleWinnerGuild, m_szCastleOwnerGuild))
		{
			// ������ �� ��尡 �ٽ� ���� �����̴�.
			m_btIsCastleOccupied	= TRUE;
			wsprintf(szMsg, lMsg.Get(1733),		// "%s ��尡 ���� ��Ű�� �� �����߽��ϴ�."
				m_szCastleOwnerGuild
				);

			bRETVAL					= FALSE;
		}
		else {
			// ���� ���α�尡 �ٲ����.
			m_btIsCastleOccupied	= TRUE;
			memset(m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
			memcpy(m_szCastleOwnerGuild, m_szMiddleWinnerGuild, 8);
			wsprintf(szMsg, lMsg.Get(1734),		// "%s ��尡 ���ο� ���� ������ �Ǿ����ϴ�."
				m_szCastleOwnerGuild
				);

			bRETVAL					= TRUE;
		}
	}
	else {
		// ������ ������ ������ ������.
		if (!strcmp(m_szMiddleWinnerGuild, ""))
		{
			// ������ ���� ���α�尡 ����.
			m_btIsCastleOccupied	= FALSE;
			wsprintf(szMsg, lMsg.Get(1735));	// "���� ���α�尡 �������� �ʾҽ��ϴ�."

			bRETVAL					= FALSE;
		}
		else {
			// ���� ���α�尡 �����.
			m_btIsCastleOccupied	= TRUE;
			memset(m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
			memcpy(m_szCastleOwnerGuild, m_szMiddleWinnerGuild, 8);
			wsprintf(szMsg, lMsg.Get(1734),		// "%s ��尡 ���ο� ���� ������ �Ǿ����ϴ�."
				m_szCastleOwnerGuild
				);

			bRETVAL					= TRUE;
		}
	}

	m_btIsSiegeEnded				= TRUE;

	LogAddTD("[CastleSiege] Castle Final Winner Guild : %s", !strcmp(m_szCastleOwnerGuild,"")?"NONE":m_szCastleOwnerGuild);

	// ��ü �� �������� �޽����� ������.
	SendMapServerGroupMsg(szMsg);

	return bRETVAL;
}


BOOL CCastleSiege::CheckUnionGuildMaster		(INT iIndex)				// Ư�� ����ڰ� �ڽ��� ���� ��忬���� ���� ���������� Ȯ���Ѵ�.
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
	case CS_NPC_CROWN_SWITCH1 :		// �հ� ����ġ 1
		m_iCastleSwitchAccessUser1	= iIndex;
		break;
	case CS_NPC_CROWN_SWITCH2 :		// �հ� ����ġ 2
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
	case CS_NPC_CROWN_SWITCH1 :		// �հ� ����ġ 1
		iUserIndex	= m_iCastleSwitchAccessUser1;
		break;
	case CS_NPC_CROWN_SWITCH2 :		// �հ� ����ġ 2
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
	case CS_NPC_CROWN_SWITCH1 :		// �հ� ����ġ 1
		m_iCastleSwitchAccessUser1	= -1;
		break;
	case CS_NPC_CROWN_SWITCH2 :		// �հ� ����ġ 2
		m_iCastleSwitchAccessUser2	= -1;
		break;
	}
}


VOID CCastleSiege::SetCrownAccessTickCount		()						// �հ� ���� �ð��� �����Ѵ�.
{
	m_dwCrownAccessTime		= GetTickCount();
}


VOID CCastleSiege::ResetCrownAccessTickCount	()						// �հ� ���� �ð��� �ʱ�ȭ�Ѵ�.
{
	m_dwCrownAccessTime		= 0;
}


INT	 CCastleSiege::CheckOverlapCsMarks			(INT iIndex)			// Ư�� ������� �κ��丮�� ������ ǥ���� ��ĥ �� �ִٸ� ���° ĭ���� �˷��ش�.
{
	for(int x=MAX_EQUIPMENT; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 21) &&
				gObj[iIndex].pInventory[x].m_Level == 3
				)
			{
				// ������ ǥ���� �����Ѵٸ�
				INT iITEM_DUR	= (INT) gObj[iIndex].pInventory[x].m_Durability;	// ������ ���� (������) �� ��´�.

				if (CHECK_LIMIT(iITEM_DUR, MAX_CS_MARKS_OVERLAP)) {
					// ���� M�� �̸��̶�� ���⿡ �� ���� �� �ִ�.
					return x;
				}
			}
		}
	}

	return -1;
}


INT	CCastleSiege::GetCsAttkGuildList			(						// ���� ���� ����� ����Ʈ�� �����´�. [0xB5] (��� - 0:����/1:����/2:���Ѿʵ�/3:�����;���)
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


BOOL CCastleSiege::CheckTeleportMagicAxisY		(						// �ڷ���Ʈ ���� ���� �� ������ �ɸ��� �ʴ��� üũ�Ѵ�.
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


VOID CCastleSiege::NotifyCsSelfLeftTime			(						// ������ �������� ���� �ð��� �����ش�.
	INT iIndex
	)
{
	GCAnsSelfCsLeftTimeAlarm(
		iIndex,
		(m_iCS_REMAIN_MSEC/1000)/3600,			// �ð�
		((m_iCS_REMAIN_MSEC/1000)%3600)/60		// ��
		);
}


VOID CCastleSiege::CheckReviveNonDbNPC			()						// DB�� �������� �ʴ� NPC�� �����Ѵ�.
{
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it				= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData		= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_bNPC_DBSAVE == FALSE))
		{
			// DB�� �������� �ʴ� NPC���̴�.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;				// ������ �����ǰų� ���� ������ ���� NPC���� �ƴϴ�.
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
				case 215 :	// �� (������)
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_DFN;		// ������ DB ����
					break;
				case 221 :	// �߼��� (������)
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_ATK;		// ������ DB ����
					break;
				case 222 :	// �߼��� (������)
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_DFN;		// ������ DB ����
					break;
				case 288 :	// ����Ÿ�� (������)
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_DFN;		// ������ DB ����
					break;
				default :	// �⺻ ?
					gObj[iNPC_INDEX].m_btCsNpcType	= CS_NPC_TYPE_INS_DFN;		// ������ DB ����
					break;
				}

				// ���߿� ���� Ȯ���� �ϱ����� ���⿡ �������� �ִ´�. -> DB NPC�� �ƴ� ��쿡�� ��ǻ� �ʿ�� ����.
//				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
//				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
//				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
//				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;
		
				// ���������� �� NPC�� �����Ǿ����� �˸���.
//				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
//				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
				(*it).m_bIN_USE						= TRUE;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


VOID CCastleSiege::CheckReviveGuardianStatue	()						// ��ȣ���� �� ���� �׾��ִ� ���� �ִٸ� DB���� ���� �ٷ� �߰���Ų��.
{
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it				= m_vtNpcData.begin();
	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData		= ((CS_NPC_DATA)(*it));
		if ((pNpcData.m_iNPC_NUM == 283) &&
			(pNpcData.m_iNPC_LIVE == NPC_LIVE_NONE)
			)
		{
			// ���� �׾��ִ� ��ȣ���� -> ���� �� �߰��Ѵ�.
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
			INT iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
#else
			INT iNPC_INDEX = gObjAdd();
#endif
			if( iNPC_INDEX >= 0 )
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum			= -1;				// ������ �����ǰų� ���� ������ ���� NPC���� �ƴϴ�.
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
				gObj[iNPC_INDEX].m_btCsNpcType		= CS_NPC_TYPE_DB_DFN;				// ������ DB ����

				INT iNPC_DF_LEVEL					= pNpcData.m_iNPC_BASE_DF_LEVEL;	// ���� �ʱ��� ���� ����
				if (iNPC_DF_LEVEL < 0)						
					iNPC_DF_LEVEL					= 0;
				if (iNPC_DF_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_DF_LEVEL					= MAX_UPGRADE_LEVEL;
				INT iNPC_RG_LEVEL					= pNpcData.m_iNPC_BASE_RG_LEVEL;	// ���� �ʱ��� ȸ���� ����
				if (iNPC_RG_LEVEL < 0)						
					iNPC_RG_LEVEL					= 0;
				if (iNPC_RG_LEVEL > MAX_UPGRADE_LEVEL)				
					iNPC_RG_LEVEL					= MAX_UPGRADE_LEVEL;

				// ȸ����, ���� ���� ��ġ�� �ʿ��ϴٸ� ���⼭ �������ش�. -> �ܺο��� (AdjustDbNpcLevel() - ���� ������)
				gObj[iNPC_INDEX].m_Defense			= g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
				gObj[iNPC_INDEX].m_btCsNpcDfLevel	= iNPC_DF_LEVEL;
				gObj[iNPC_INDEX].m_btCsNpcRgLevel	= iNPC_RG_LEVEL;

				// ���߿� ���� Ȯ���� �ϱ����� ���⿡ �������� �ִ´�.
				gObj[iNPC_INDEX].m_btCsNpcExistVal1	= (*it).m_btCsNpcExistVal1	= rand()%255 + 1;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2	= (*it).m_btCsNpcExistVal2	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3	= (*it).m_btCsNpcExistVal3	= rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4	= (*it).m_btCsNpcExistVal4	= rand()%256;
		
				// ���������� �� NPC�� �����Ǿ����� �˸���.
				(*it).m_iNPC_OBJINDEX				= iNPC_INDEX;
				(*it).m_iNPC_LIVE					= NPC_LIVE_OK;
				(*it).m_bIN_USE						= TRUE;
			}
		}
		
		it++;
	}
	LeaveCriticalSection (&m_critNpcData);
}


VOID CCastleSiege::ReSpawnAllUser				()						// ���� ������ �ȿ� �ִ� ��� ������� ������ ���ҿ� �°� ������ ��ġ�� ���� �̵� ��Ų��.
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
						// ���⼭ ����ڸ� �̵� ��Ų��.
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
						// ���⼭ ����ڸ� �̵� ��Ų��.
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
						// ���⼭ ����ڸ� �̵� ��Ų��.
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


VOID CCastleSiege::ReSpawnEnemyUser				(						// ���� ������ �ȿ� �ִ� �������� �ƴ� ��� ������� �� ������ ���� �̵� ��Ų��.
	BOOL bRefreshOwnerUser
	)
{
	INT		iTX, iTY;
	BYTE	btMapAttr;

	for (INT iUSER = ALLOC_USEROBJECTSTART; iUSER<MAX_OBJECT; iUSER++) {
		if (!gObjIsConnected(iUSER))
			continue;
		
#ifdef MODIFY_CASTLEHUNTZONE_USER_BUGFIX_20070223
		// �÷��� ���� �ִ� ������ �ű��.
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
					// ������ ��ġ�� �������� ���ش�. (���� ���� ���� ��)
					gObj[iUSER].RegenMapNumber		= gObj[iUSER].MapNumber;
					gObj[iUSER].RegenMapX			= (BYTE) gObj[iUSER].X;
					gObj[iUSER].RegenMapY			= (BYTE) gObj[iUSER].Y;		

#ifdef MODIFY_CASTLEHUNTZONE_USER_BUGFIX_20070223
					// �÷��� ���� �ִ� ������ ���
					if( gObj[iUSER].MapNumber == MAP_INDEX_CASTLEHUNTZONE )
					{
						for (INT iCNT = 0 ; iCNT < 100 ; iCNT++) 
						{
							iTX			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][2] - g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][0]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][0];
							iTY			= rand() % (g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][3] - g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][1]) + g_iCsUserReSpawnArea[CS_SIEGESIDE_DEFEND][1];
							btMapAttr	= MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[(iTY*MAX_TERRAIN_SIZE)+iTX];
							if( (btMapAttr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) 
							{
								// ���⼭ ����ڸ� �̵� ��Ų��.
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
						// ���⼭ ����ڸ� �̵� ��Ų��.
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
						// ���⼭ ����ڸ� �̵� ��Ų��.
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

		// �ٽ��ѹ� �ڽ��� ���¸� ������ �� �ֵ��� �����ش�.
		NotifySelfCsJoinSide (iUSER);
		
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0;
#endif

	}
}


VOID CCastleSiege::NotifyAllUserCsStartState	(BYTE btStartState)		// ���� �������� ���۵Ǿ����� ���θ� ��� ����ڵ鿡�� �˸���.
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


VOID CCastleSiege::NotifyAllUserCsProgState		(						// ���� �������� ���� ���¸� ��� ����ڵ鿡�� �˸���.
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


VOID CCastleSiege::ClearCastleTowerBarrier	()							// ���� �����ϴ� ��� �� NPC�� �����Ѵ�.
{
	for (INT iIDX = 0 ; iIDX < MAX_MONSTER ; iIDX++) {
		if (!gObjIsConnected(iIDX))
			continue;
		if (gObj[iIDX].Class == 215) {
			gObjDel(iIDX);
		}
	}
}


VOID CCastleSiege::SetAllCastleGateState	(BOOL bOpenType)			// ��� �����ϴ� ������ ���¸� �����Ѵ�.
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
				// ������ �´�.
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


VOID CCastleSiege::AddMiniMapDataReqUser	(INT iIndex)				// �̴ϸ� ������ ��û �ο��� ����Ѵ�.
{
	EnterCriticalSection(& m_critCsMiniMap);
	if (find(m_vtMiniMapReqUser.begin(), m_vtMiniMapReqUser.end(), iIndex) == m_vtMiniMapReqUser.end()) {
		m_vtMiniMapReqUser.push_back(iIndex);
	}
	LeaveCriticalSection(& m_critCsMiniMap);
}


VOID CCastleSiege::DelMiniMapDataReqUser	(INT iIndex)				// �̴ϸ� ������ ��û �ο��� �����Ѵ�.
{
	EnterCriticalSection(& m_critCsMiniMap);
	vector<INT>::iterator it = find(m_vtMiniMapReqUser.begin(), m_vtMiniMapReqUser.end(), iIndex);
	if (it != m_vtMiniMapReqUser.end()) {
		m_vtMiniMapReqUser.erase(it);
	}
	LeaveCriticalSection(& m_critCsMiniMap);
}


VOID CCastleSiege::OperateMiniMapWork		()							// �̴ϸ� ���� �۾��� �����Ѵ�. -> ������ ���� ����
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

	// 1 . �����ؾ� �� ��������� ������ �ľ��Ѵ�.
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

	// 2 . ������ ���� NPC ������ �����Ѵ�.
	EnterCriticalSection(& m_critNpcData);
	lpMsgSend2->iCount					= 0;
	if (!m_vtNpcData.empty()) {
		vector <CS_NPC_DATA>::iterator it	= m_vtNpcData.begin();
		for (; it != m_vtNpcData.end() ; it++) {
			if (lpMsgSend2->iCount >= MAX_CS_NPC_COUNT)
				break;

			CS_NPC_DATA & pNpcData			= ((CS_NPC_DATA)(*it));
			if ((pNpcData.m_iNPC_NUM == 277 ||	// ����
				pNpcData.m_iNPC_NUM == 283		// ����
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
					case 277 :					// ���� - ���Ӽ��� ������ �ش�.
						{
							btNpcType	= 0;
						}
						break;
					case 283 :					// ���� - �ٸ� ������� ���ŵǾ����� Ȯ���Ͽ� ���� ž �Ա��� �����Ѵ�.
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

	// 3 . ������ �ľǵ� ������� ������ �������� ���� ��ǥ �����͸� �����Ѵ�.
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

			// �� ����� ����� ���� ������ Ȯ���Ͽ� ������ ��帶���� �ε����� ����Ѵ�.
			if (gObj[iIDX].GuildStatus == G_MASTER) {
				if (strcmp(gObj[iIDX].GuildName, "") &&
					!strcmp(gObj[iIDX].GuildName, (it3->second).m_szGuildName)
					)
				{
					(it3->second).m_iGuildMasterIndex	= iIDX;
					continue;
				}
			}

			// Ư�� �������� ���ϴ� ����� �´�. -> �ش� �̴ϸʿ� ��ġ�� �߰��� �ش�.
			(it3->second).m_stMiniMapPoint[(it3->second).m_iMiniMapPointCount].btX	= gObj[iIDX].X;
			(it3->second).m_stMiniMapPoint[(it3->second).m_iMiniMapPointCount].btY	= gObj[iIDX].Y;
			(it3->second).m_iMiniMapPointCount++;
		}
	}

	// ���������� �� ����� �渶�鿡�� �̴ϸ� �����͸� ������.
	map <INT, CS_MINIMAP_DATA>::iterator it4			= m_mapMiniMapData.begin();
	for ( ; it4 != m_mapMiniMapData.end() ; it4++ ) {
		lpMsgSend1->iCount								= 0;
		
		if (!gObjIsConnected((it4->second).m_iGuildMasterIndex))
			continue;
		
		// ������ ��帶���� üũ�� �����Ƿ� ���⼭�� ������ üũ ���� ������ ������.
		lpMsgSend1->iCount								= (it4->second).m_iMiniMapPointCount;
		if (lpMsgSend1->iCount > MAX_MINIMAP_USER_COUNT)
			lpMsgSend1->iCount							= MAX_MINIMAP_USER_COUNT;
		memcpy(lpMsgSendBody1, (it4->second).m_stMiniMapPoint, sizeof(PMSG_SENDMINIMAPDATA) * lpMsgSend1->iCount);
		lpMsgSend1->h.set(cBUFFER1, 0xB6, sizeof(PMSG_ANS_SENDMINIMAPDATA) + sizeof(PMSG_SENDMINIMAPDATA) * lpMsgSend1->iCount);
		DataSend((it4->second).m_iGuildMasterIndex, cBUFFER1, sizeof(PMSG_ANS_SENDMINIMAPDATA) + sizeof(PMSG_SENDMINIMAPDATA) * lpMsgSend1->iCount);

		// ������ ������ ������ NPC ������ ���� ������.
		DataSend((it4->second).m_iGuildMasterIndex, cBUFFER2, sizeof(PMSG_ANS_SENDNPCMINIMAPDATA) + sizeof(PMSG_SENDNPCMINIMAPDATA) * lpMsgSend2->iCount);
	}

	// ������ �̴ϸ� �����͸� ��û�ߴ� ����ڵ鿡�� ������ ������ �����͸� ������.
/*
	vector <INT>::iterator it4							= m_vtMiniMapReqUser.begin();
	for ( ; it4 != m_vtMiniMapReqUser.end() ; it4++ ) {
		INT iIndex										= (*it4);

		lpMsgSend1->iCount								= 0;
		
		if (!gObjIsConnected(iIndex)) {
			// �������� �ʾҴ�.
			m_vtMiniMapReqUser.erase(it4);
			continue;
		}

		if (gObj[iIndex].m_btCsJoinSide <= 0) {
			// ���� ������� �Ҽ��� �ƴϴ�.
			m_vtMiniMapReqUser.erase(it4);
			continue;
		}

		map <INT, CS_MINIMAP_DATA>::iterator it5		= m_mapMiniMapData.find(gObj[iIndex].m_btCsJoinSide);
		if (it5 == m_mapMiniMapData.end()) {
			// ���� ������� �Ҽ��� �ƴϴ�.
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


VOID CCastleSiege::NotifyCrownState			(BYTE btState)				// ���� �հ��� ���¸� �ֺ��� �˸� (0:��ȣ������/1:��ȣ������/2:��ϼ���)
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

	// ����Ʈ �� �ƹ��� ���ٸ� ������.
	if( lpObj->VPCount2 < 1 ) return;

	PMSG_ANS_NOTIFYCROWNSTATE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x16, sizeof(PMSG_ANS_NOTIFYCROWNSTATE));
	pMsg.btCrownState		= btState;

	// ����Ʈ �󿡼� �ش�Ǵ� ������� ã�´�.
	INT	tObjNum	= -1;
	for( int i=0; i<MAXVIEWPORTOBJECT; i++)							// �հ� (NPC) �� ����Ʈ ������ ����� ����
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
	// �հ� ����ġ ������ ������.

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

	// ����Ʈ �� �ƹ��� ���ٸ� ������.
	if( lpObj->VPCount2 < 1 ) return;

	PMSG_ANS_NOTIFY_CROWNSWITCH_INFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x20, sizeof(PMSG_ANS_NOTIFY_CROWNSWITCH_INFO));
	pMsg.btIndex1 = HIBYTE((WORD)iCrownSwitchIndex);
	pMsg.btIndex2 = LOBYTE((WORD)iCrownSwitchIndex);
	pMsg.btSwitchState	= 0;
	pMsg.btJoinSide		= 0;
	ZeroMemory( pMsg.szGuildName, MAX_GUILDNAMESTRING );	
	ZeroMemory( pMsg.szUserName, MAX_IDSTRING );


	// ���� ����ġ�� ���� �� �ִ� ���� ���� ����
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
		
		// ���� ����ġ�� ���� �� �ִ� �������Դ� ������ �����ش�.
		DataSend(iCrownSwitchUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	

	// ����Ʈ �󿡼� �ش�Ǵ� ������� ã�´�.
	INT	tObjNum	= -1;
	for( int i=0; i<MAXVIEWPORTOBJECT; i++)							// �հ� (NPC) �� ����Ʈ ������ ����� ����
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


VOID CCastleSiege::SendMapServerGroupMsg	(						// �ʼ��� �׷� ��ü�� ����ڿ��� Ư�� �޽����� ������.
	LPSTR lpszMsg 
	)
{
	GS_GDReqMapSvrMsgMultiCast	(m_iMapSvrGroup, lpszMsg);
}


VOID CCastleSiege::SendAllUserAnyData		(						// ������ �ִ� ��� ����ڵ鿡�� Ư�� �����͸� ������.
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


VOID CCastleSiege::SendAllUserAnyMsg		(						// ������ �ִ� ��� ����ڵ鿡�� Ư�� �޽����� ������.
	LPSTR lpszMsg,
	INT iType
	)
{
	if (lpszMsg == NULL)
		return;

	PMSG_NOTICE	pNotice;

	switch(iType) {
	case 1 :			// �����̵� ����
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
	case 2:				// �߾� ����
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


VOID CCastleSiege::SendCsUserAnyData		(						// �������ʿ� ������ �ִ� ��� ����ڵ鿡�� Ư�� �����͸� ������.
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


VOID CCastleSiege::SendCsUserAnyMsg			(						// �������ʿ� ������ �ִ� ��� ����ڵ鿡�� Ư�� �޽����� ������.
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
#ifdef CASTLE_GM_COMMAND_20041222								// ��� �������� ���ó��
VOID CCastleSiege::OperateGmCommand		(
		INT iIndex,
		INT iGmCommand,
		LPVOID lpParam
		)
{
	if (!gObjIsConnected(iIndex))
		return;

#if TESTSERVER != 1
	// ��ڰ� �ƴϸ� ����� �����Ѵ�.
	if( (gObj[iIndex].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
		return;
#endif
	
	switch (iGmCommand) {
	case CS_GMCMD_CHANGEOWNERGUILD :							// "/�����������"
		{
			LPSTR lpszCastleOwnerGuild	= (LPSTR) lpParam;
			if (lpszCastleOwnerGuild == NULL) {
				MsgOutput(iIndex, lMsg.Get(3200) );		// "������ ��庯�� ����"
				return;
			}
			if (strcmp(lpszCastleOwnerGuild, "")) {
				memset(m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
				memcpy(m_szCastleOwnerGuild, lpszCastleOwnerGuild, 8);
				m_btIsCastleOccupied	= TRUE;

				MsgOutput(iIndex, lMsg.Get(3201), m_szCastleOwnerGuild);	// "������ ��庯�� : %s"
			}
			else {
				strcpy(m_szCastleOwnerGuild, "");
				m_btIsCastleOccupied	= FALSE;

				MsgOutput(iIndex, lMsg.Get(3200) );	// "������ ��庯�� ����"
			}
		}
		return;
	case CS_GMCMD_SETSTATE_REGSIEGE :							// "/�����������"
		{
			GetLocalTime(&m_tmStartDate);
			m_tmStartDate.wHour			= 0;
			m_tmStartDate.wMinute		= 0;
			m_tmStartDate.wSecond		= 0;
			m_tmEndDate					= m_tmStartDate;
			GetNextDay(m_tmEndDate, m_iCastleSiegeCycle);

			m_bFixCastleCycleStartDate	= FALSE;
			m_btIsSiegeEnded			= FALSE;
			
			// �������� DB NPC���� �ٽ� �����Ѵ�. (�Ϻ� �ı��Ǿ��� �� �����Ƿ�)

			// 20051101 ����������� �� �ӽ÷� �����Ǵ� NPC ������ ����. (apple)
			// ���׷��̵� �׽�Ʈ�� ����!
			// REMOVE_CASTLESIEGE_AUTO_REVIVE_GUARDIANSTATUE_20051101
//			CreateDbNPC_INS();
			
			// ������ ������û ��� ����Ʈ�� �ʱ�ȭ�Ѵ�.
			GS_GDReqResetRegSiegeInfo (m_iMapSvrGroup);

			// ������ �ð��� �����Ѵ�.
			if (CheckSync() == FALSE) {
				LogAddC(LOGC_RED, "[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
				MsgBox("[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
				MsgOutput(iIndex, lMsg.Get(3202));	// "���º��� ���� - �����������"
				return;
			}

			// ���� ������ ������ ������ �����ص� �ȴ�.
			m_bDoRun								= TRUE;
			m_dwNPC_DBSAVE_TICK_COUNT				= GetTickCount();
			m_dwCALC_LEFTSIEGE_TICK_COUNT			= 0;
			m_dwEVENT_MSG_TICK_COUNT				= 0;
			m_dwCHECK_GATE_ALIVE_COUNT				= 0;
			MsgOutput(iIndex, lMsg.Get(3203));		// "���º��� ���� - �����������"
		}
		return;
	case CS_GMCMD_SETSTATE_REGMARK :							// "/����������"
		{
			SetState (CASTLESIEGE_STATE_REGMARK);
			MsgOutput(iIndex, lMsg.Get(3204));		// "���º��� ���� - ����������"
		}
		return;
	case CS_GMCMD_SETSTATE_NOTIFY :								// "/��������"
		{
			m_btIsSiegeGuildList					= FALSE;
			SetState (CASTLESIEGE_STATE_NOTIFY);
			MsgOutput(iIndex, lMsg.Get(3205));			// "���º��� ���� - ��������"
		}
		return;
	case CS_GMCMD_SETSTATE_STARTSIEGE :							// "/��������"
		{
			SetState (CASTLESIEGE_STATE_STARTSIEGE);
			MsgOutput(iIndex, lMsg.Get(3206));			// "���º��� ���� - ��������"
		}
		return;
	case CS_GMCMD_SETSTATE_ENDSIEGE :							// "/��������"
		{
			if (m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE) {
				m_iCS_REMAIN_MSEC	= 0;
				MsgOutput(iIndex, lMsg.Get(3207));		// "���º��� ���� - ��������"
			}
			else {
				MsgOutput(iIndex, lMsg.Get(3208));		// "���º��� ���� - ��������"
			}
		}
		return;
	case CS_GMCMD_SETSTATE_GETOWNERGUILD :						// "/�����������"
		{
			if (!strcmp(m_szCastleOwnerGuild, "")) {
				MsgOutput(iIndex, lMsg.Get(3209));				// "���� ������ ����"
			}
			else {
				MsgOutput(iIndex, lMsg.Get(3210), m_szCastleOwnerGuild); // "���� ������ ���  - %s"
			}
		}
		return;
	case CS_GMCMD_SETSTATE_GETCASTLESTATE :						// "/�����������"
		{
			if (m_bDoRun == FALSE) {
				MsgOutput(iIndex, lMsg.Get(3211));	// "���� �������� - �� �� ����"
			}
			else {
				switch(m_iCastleSiegeState) {
				case CASTLESIEGE_STATE_NONE :			// ���¾���
					MsgOutput(iIndex, lMsg.Get(3211));	// "���� �������� - �� �� ����"
					break;
				case CASTLESIEGE_STATE_IDLE_1 :			// ���ޱⰣ - 1
					MsgOutput(iIndex, lMsg.Get(3212));	// "���� �������� - ���� �ֱ� ���� (���۾ʵ�)"
					break;
				case CASTLESIEGE_STATE_REGSIEGE :		// ��û�Ⱓ	(�����ֱ� ����)
					MsgOutput(iIndex, lMsg.Get(3213), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "���� �������� - ������ ��ϱⰣ : %d�ð�:%d��:%d�� ����"
					break;
				case CASTLESIEGE_STATE_IDLE_2 :			// ���ޱⰣ - 2
					MsgOutput(iIndex, lMsg.Get(3214), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "���� �������� - ������ ������� (�޽ıⰣ) : %d�ð�:%d��:%d�� ����
					break;
				case CASTLESIEGE_STATE_REGMARK :		// ���� ��ϱⰣ
					MsgOutput(iIndex, lMsg.Get(3215), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "���� �������� - ���� ��ϱⰣ : %d�ð�:%d��:%d�� ����"
					break;
				case CASTLESIEGE_STATE_IDLE_3 :			// ���ޱⰣ - 3
					MsgOutput(iIndex, lMsg.Get(3216), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "���� �������� - ���� �������  (�޽ıⰣ) : %d�ð�:%d��:%d�� ����"
					break;
				case CASTLESIEGE_STATE_NOTIFY :			// ��ǥ�Ⱓ
					MsgOutput(iIndex, lMsg.Get(3217), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "���� �������� - ������ ������� ��ǥ�Ⱓ : %d�ð�:%d��:%d�� ����"
					break;
				case CASTLESIEGE_STATE_READYSIEGE :		// �غ�Ⱓ
					MsgOutput(iIndex, lMsg.Get(3218), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "���� �������� - ������ �غ�Ⱓ : %d�ð�:%d��:%d�� ����"
					break;
				case CASTLESIEGE_STATE_STARTSIEGE :		// ��������
					MsgOutput(iIndex, lMsg.Get(3219), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "���� �������� - ������ ���� �� : %d�ð�:%d��:%d�� ����"
					break;
				case CASTLESIEGE_STATE_ENDSIEGE :		// �����Ⱓ
					MsgOutput(iIndex, lMsg.Get(3220), 
						(m_iCS_REMAIN_MSEC/1000)/3600,
						((m_iCS_REMAIN_MSEC/1000)%3600)/60,
						(((m_iCS_REMAIN_MSEC/1000)%3600)%60)
						);	// "���� �������� - ������ �������� (�޽ıⰣ) : %d�ð�:%d��:%d�� ����"
					break;
				case CASTLESIEGE_STATE_ENDCYCLE :		// �����ֱ� ����
					MsgOutput(iIndex, lMsg.Get(3221));
					break;	// "���� �������� - ������ �ֱ�����"
				default :
					MsgOutput(iIndex, lMsg.Get(3211));
					break;	// "���� �������� - �� �� ����"
				}
			}
		}
		return;
	case CS_GMCMD_SETSTATE_CHANGECSSIDE :						// "/������ü"
		{
			LPSTR lpszAttackGuild	= (LPSTR) lpParam;
			if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE) {
				MsgOutput(iIndex, lMsg.Get(3222));		// "������ü ����"
				return;
			}
			if (lpszAttackGuild == NULL) {
				MsgOutput(iIndex, lMsg.Get(3222));		// "������ü ����"
				return;
			}
			if (!strcmp(lpszAttackGuild, "")) {
				MsgOutput(iIndex, lMsg.Get(3222));		// "������ü ����"
				return;
			}

			BYTE btCsJoinSide		= CS_SIEGESIDE_NONE;
			bool bGuildInvolved		= FALSE;
			BOOL bRETVAL			= GetCsJoinSide(lpszAttackGuild, btCsJoinSide, bGuildInvolved);
			
			if (bRETVAL == TRUE) {
				if (btCsJoinSide > CS_SIEGESIDE_DEFEND && bGuildInvolved == true) {
					// �������̰� ��ü�� ����� ������ü
					LogAddTD("[CastleSiege] Castle Winner Has been Changed - GM (GUILD:%s)", lpszAttackGuild);

					// �ϴ� �޽����� �ѷ��ش�.
					NotifyCrownState(2);

					// ���⼭ ���� ó���� �Ѵ�.
					memset(m_szMiddleWinnerGuild, 0, sizeof(m_szMiddleWinnerGuild));
					memcpy(m_szMiddleWinnerGuild, lpszAttackGuild, 8);

					// �߰��� �����ؾ� �� NPC���� ������ �ش�. (������ ���� ��)
					ClearNonDbNPC_MidWin ();

					// ��/�� ������ ���¸� ��ȯ�Ѵ�.
					ChangeWinnerGuild (btCsJoinSide);

					// ������ ���� ����鿡�� ���� ������ ��� �Ǿ����� �˸���. -> ��/�� ��ȯ��
					NotifyAllUserCsProgState (1, lpszAttackGuild);

					// NPC ���� ���µ��� �ʱ�ȭ �Ѵ�.
					ResetCrownAccessTickCount();
					ResetCrownUserIndex();
					SetCrownAccessUserX(0);
					SetCrownAccessUserY(0);
					ResetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH1);
					ResetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH2);
					m_bRegCrownAvailable	= FALSE;

					// �������� �������� ������
					ReSpawnEnemyUser();

					// ���� �����ð��� �ٽ� �����־�� �Ѵ� -> Ŭ���̾�Ʈ�� ����Ƿ�
					GCAnsCsLeftTimeAlarm (
						(m_iCS_REMAIN_MSEC/1000)/3600,			// �ð�
						((m_iCS_REMAIN_MSEC/1000)%3600)/60		// ��
						);
					m_dwCS_LEFTTIME_TICK_COUNT			= GetTickCount();
				}

				MsgOutput(iIndex, lMsg.Get(3223));	//	"������ü ����"
			}
			else {
				MsgOutput(iIndex, lMsg.Get(3222));	//	"������ü ����"

				
			}
		}
		return;
	}
}

VOID CCastleSiege::CreateDbNPC_INS			(							// ����, ���� �� DB NPC�� DB ���� ���� �ٷ� �����.
	)
{
	// ��� DB NPC���� �����Ѵ�.
	ClearDbNPC ();

	// �ٽ� ��� �����Ѵ�.
	EnterCriticalSection (&m_critNpcData);
	vector <CS_NPC_DATA>::iterator it		= m_vtNpcData.begin();

	while (it != m_vtNpcData.end()) {
		CS_NPC_DATA & pNpcData				= ((CS_NPC_DATA)(*it));
		if (pNpcData.m_bNPC_DBSAVE			== TRUE) {
			(*it).SetBaseValue();
			(*it).m_iNPC_LIVE				= NPC_LIVE_READY;
			(*it).m_iNPC_SIDE				= CS_SIEGESIDE_DEFEND;	// DB�� NPC�����ʹ� ������ ����
			(*it).m_iNPC_OBJINDEX			= -1;
			(*it).m_iCS_GATE_LEVER_INDEX	= -1;					// ������ ������ �ʸ������ ������ �Ѵ�.
		}

		it++;
	}
	LeaveCriticalSection (&m_critNpcData);

	m_bDbNpcCreated	= FALSE;
}
#endif


#ifdef CASTLE_SAVE_PCROOM_USERLIST_20050105						// ������ �����ڵ��� PC�� ���� ����

VOID CCastleSiege::SavePcRoomUserList		()
{
	if (m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE)
		return;

#ifdef FOR_KOREA	// �ѱ��� �ش� ��
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

