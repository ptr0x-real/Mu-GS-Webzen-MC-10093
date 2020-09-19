//----------------------------------------------------------------------------------
// FileName				: Raklion.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 보스전 관련 스케줄링
//----------------------------------------------------------------------------------
// Raklion.cpp: implementation of the CRaklion class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_RAKLION_20080408

#include "Raklion.h"

#include "DirPath.h"
#include "..\Include\ReadScript.h"
#include "MapServerManager.h"
#include "protocol.h"
#include "user.h"
#include "gamemain.h"
#include "MonsterSetBase.h"
#include "gObjMonster.h"

#include "RaklionDefine.h"
#include "RaklionUtil.h"

extern CDirPath			gDirPath;
extern CMonsterSetBase	gMSetBase;
static CRaklionUtil		g_RaklionUtil;

CRaklion g_Raklion;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklion::CRaklion()
{
#ifdef UPDATE_SEASON_4_EVENT_LOG_200800715		// 이벤트용 로그 추가
	time_t t;
	tm *pCurrentTime;
	time( &t );
	pCurrentTime = localtime( &t );
	
	m_iTodayYear = pCurrentTime->tm_year;
	m_iTodayMon = pCurrentTime->tm_mon;
	m_iTodayDay = pCurrentTime->tm_mday;
	
	m_bEvent	= FALSE;	
#endif // UPDATE_SEASON_4_EVENT_LOG_200800715

	ResetAllData();
}

CRaklion::~CRaklion()
{
}

void CRaklion::Init()
{
}

// 라클리온의 모든 데이터를 초기화한다.
void CRaklion::ResetAllData()
{
	m_bIsBossKillSuccess		= FALSE;
	m_bFileDataLoad				= FALSE;

	m_iBossAppearanceStartTime	= 0;
	m_iBosszoneCloseStartTime	= 0;
	m_iBosszoneOpenStartTime	= 0;

	m_bIsNotify1				= FALSE;

	m_iBossEggDieCount			= 0;
}

// 라클리온 보스전에 관한 데이터를 로드한다.
BOOL CRaklion::LoadData( LPSTR lpszFileName )
{
	m_bFileDataLoad = FALSE;

	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		MsgBox( "[ RAKLION ] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{		
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )	
		{
			DWORD dwError = GetLastError();
			MsgBox( "[ RAKLION ] - Can't Open %s ", lpszFileName );			
			return FALSE;
		}
		
		SMDToken Token;
		INT iType = -1;

		while( TRUE )
		{
			Token = GetToken();
			if( Token == END ) break;
			iType = ( int )TokenNumber;

			while( TRUE )
			{
				// 라클리온 스케쥴링에 사용되는 딜레이 값 읽어오기(sec)
				if( iType == 0 )
				{
					// 1. >	데이터 초기화
					INT iAppearanceDelay = 0;
					INT iBosszoneCloseDelay = 0;
					INT iBosszoneOpenDelay = 0;
				
					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iAppearanceDelay = ( int )TokenNumber;		Token = GetToken();
					iBosszoneCloseDelay = ( int )TokenNumber;	Token = GetToken();
					iBosszoneOpenDelay = ( int )TokenNumber;
						
					// 3. >	데이터 유효성 검사
					// 등장시 연출 딜레이가 10초를 넘는다면 에러처리(바뀔 수 있음)
					if( iAppearanceDelay < 0 || iAppearanceDelay > 10 )
					{
						MsgBox( "[ RAKLION ] - Exceed AppearanceDelay : (%d)", iAppearanceDelay );
						break;
					}
					// 보스 출현 후 보스존이 닫히기까지의 딜레이가 10분을 넘는다면 에러처리(바뀔 수 있음)
					if( iBosszoneCloseDelay < 0 || iBosszoneCloseDelay >= 500 )
					{
						MsgBox( "[ RAKLION ] - Exceed BosszoneCloseDelay : (%d)", iBosszoneCloseDelay );
						break;
					}
					// 보스전 종료 후 보스존이 열리기까지의 딜레이가 10분을 넘는다면 에러처리(바뀔 수 있음)
					if( iBosszoneOpenDelay < 0 || iBosszoneOpenDelay >= 500 )
					{
						MsgBox( "[ RAKLION ] - Exceed BosszoneOpenDelay : (%d)", iBosszoneOpenDelay );
						break;
					}
					
					// 4. > 데이터 세팅
					m_iBossAppearanceDelay = iAppearanceDelay;
					m_iBosszoneCloseDelay = iBosszoneCloseDelay;
					m_iBosszoneOpenDelay = iBosszoneOpenDelay;
				}
				// 세루판의 알 갯수 읽어오기
				else if( iType == 1 )
				{
					// 1. >	데이터 초기화
					INT iBossEggHalf = 0;
					INT iBossEggMax = 0;

					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iBossEggHalf = ( int )TokenNumber;		Token = GetToken();
					iBossEggMax = ( int )TokenNumber;
						
					// 3. >	데이터 유효성 검사
					// 알파괴 중 연출을 위한 알 갯수가 음수이면 에러 처리
					if( iBossEggHalf < 0 )
					{
						MsgBox( "[ RAKLION ] - BossEggHalf count error : (%d)", iBossEggHalf );
						break;
					}
					// 알 최대 갯수가 음수이면 에러 처리
					if( iBossEggMax < 0 )
					{
						MsgBox( "[ RAKLION ] - BossEggMax count error : (%d)", iBossEggMax );
						break;
					}
					
					// 4. > 데이터 세팅
					m_iBossEggHalf = iBossEggHalf;
					m_iBossEggMax = iBossEggMax;
				}
				// 보스의 스킬 딜레이 값 읽어오기(millisec)
				else if( iType == 2 )
				{
					// 1. >	데이터 초기화
					INT iBossSkillDelay = 0;
					
					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iBossSkillDelay = ( int )TokenNumber;
					
					// 3. >	데이터 유효성 검사
					// 스킬 딜레이가 음수이면 에러 처리
					if( iBossSkillDelay < 0 )
					{
						MsgBox( "[ RAKLION ] - BossSkillDelay count error : (%d)", iBossSkillDelay );
						break;
					}
									
					// 4. > 데이터 세팅
					m_BattleOfSelupan.SetSelupanSkillDelay( iBossSkillDelay );
				}
				// 보스의 각 패턴 변이 조건 읽어오기
				else if( iType == 3 )
				{
					// 1. >	데이터 초기화
					INT iBossPatternCondition[6] = {0, };
					
					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iBossPatternCondition[0] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[1] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[2] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[3] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[4] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[5] = ( int )TokenNumber;
					
					// 3. >	데이터 유효성 검사
					// 4. > 데이터 세팅					
					for( INT iCount=0; iCount<6; ++iCount )
					{						
						if( iBossPatternCondition[iCount] < 0 )
						{
							//HP %값이 음수이면 에러 처리
							MsgBox( "[ RAKLION ] - BossPatternCondition[%d] error : (%d)",
								iCount, iBossPatternCondition[iCount] );
							break;
						}
						else
						{
							// 정상적이면 값을 세팅
							m_BattleOfSelupan.SetPatternCondition( iCount, iBossPatternCondition[iCount] );
						}
					}					
				}
				else
				{
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) 
						break;
				}
			}
		}
		fclose( SMDFile );
		
		LogAddC( 2, "[ RAKLION ] - %s file is Loaded", lpszFileName );

		m_bFileDataLoad = TRUE;	
	}
	catch(...) 
	{
		MsgBox( "[ RAKLION ] - Loading Exception Error (%s) File. ", lpszFileName );
	}

	return m_bFileDataLoad;	
}

// 맵 데이터를 로드한다. Terrain58.att Terrain59.att
void CRaklion::LoadRaklionMapAttr( LPSTR lpszFileName, BYTE btLevel )
{
}

//--------------------------------------------------------------------------------------------------------
void CRaklion::Run()
{
	
#ifdef UPDATE_SEASON_4_EVENT_LOG_200800715		// 이벤트용 로그 추가
	INT iRaklionUserCount = 0;
	
	time_t t;
	tm *pCurrentTime;
	time( &t );
	pCurrentTime = localtime( &t );
	
	if( m_iTodayDay!=pCurrentTime->tm_mday		// 일이 틀리거나
		|| m_iTodayMon!=pCurrentTime->tm_mon	// 월이 틀리거나
		|| m_iTodayYear!=pCurrentTime->tm_year)	// 연도가 틀리면 로그를 남길수 있다.( 다음날이 되었다.)
	{
		m_iTodayDay = pCurrentTime->tm_mday;
		m_iTodayMon = pCurrentTime->tm_mon;
		m_iTodayYear = pCurrentTime->tm_year;
		
		m_bEvent = FALSE;
	}
	
	// 로그를 남기지 않은 상태이고 0시 0분 0초이면 라클리온에 있는 유저들을 로그로 남긴다.
	if( m_bEvent==FALSE && pCurrentTime->tm_hour==0
		&& pCurrentTime->tm_min==0 && pCurrentTime->tm_sec==0 )
	{
		for( INT iCount=ALLOC_USEROBJECTSTART; iCount<MAX_OBJECT; ++iCount )
		{
			if( gObj[iCount].MapNumber==MAP_INDEX_RAKLION_FIELD || gObj[iCount].MapNumber==MAP_INDEX_RAKLION_BOSS )
			{
				if( gObj[iCount].Connected==CSS_GAMEPLAYING && gObj[iCount].Type==OBJTYPE_CHARACTER )
				{
					++iRaklionUserCount;
					LogAdd( "[RAKLION][IsInRaklionEvent] %d/%d/%d %dth User : %s (%s)",
						pCurrentTime->tm_year+1900, pCurrentTime->tm_mon+1, pCurrentTime->tm_mday,	// 날짜
						iRaklionUserCount,															// 유저 카운트
						gObj[iCount].Name,															// 캐릭터명
						gObj[iCount].AccountID );													// 유저 아이디
				}
			}			
		}
		
		m_bEvent = TRUE;
	}
	
#endif // UPDATE_SEASON_4_EVENT_LOG_200800715

	// 1. 라클리온 보스전을 진행하는가?
	if( !IsRaklionBossEnable() )
	{
		// 라클리온 이벤트를 진행하지 않도록 설정되어 있음
		return;
	}
	
	// 2. 라클리온 맵에 있는 유저에 대한 처리
	CheckUserOnRaklionBossMap();
	
#ifdef MODIFY_RAKLION_BATTLE_USER_DELETE_BUGFIX_20080916
	// 유저의 상태 처리
	g_RaklionBattleUserMng.CheckUserState();
#endif // MODIFY_RAKLION_BATTLE_USER_DELETE_BUGFIX_20080916

	// 3. 라클리온 상태 처리
	switch( m_iRaklionState )
	{
	case RAKLION_STATE_IDLE:
		ProcState_IDLE();
		break;

	case RAKLION_STATE_NOTIFY_1:
		ProcState_NOTIFY_1();
		break;

	case RAKLION_STATE_STANDBY:
		ProcState_STANDBY();
		break;

	case RAKLION_STATE_NOTIFY_2:
		ProcState_NOTIFY_2();
		break;

	case RAKLION_STATE_READY:
		ProcState_READY();
		break;
	
	case RAKLION_STATE_START_BATTLE:
		ProcState_START_BATTLE();
		break;
	
	case RAKLION_STATE_NOTIFY_3:
		ProcState_NOTIFY_3();
		break;

	case RAKLION_STATE_CLOSE_DOOR:
		ProcState_CLOSE_DOOR();
		break;

	case RAKLION_STATE_ALL_USER_DIE:
		ProcState_ALL_USER_DIE();
		break;

	case RAKLION_STATE_NOTIFY_4:
		ProcState_NOTIFY_4();
		break;
				
	case RAKLION_STATE_END:
		ProcState_END();
		break;
	}	
}

//--------------------------------------------------------------------------------------------------------
void CRaklion::SetState( int iRaklionState )
{
	switch( iRaklionState )
	{
	case RAKLION_STATE_IDLE:
		SetState_IDLE();
		break;
	case RAKLION_STATE_NOTIFY_1:
		SetState_NOTIFY_1();
		break;
	case RAKLION_STATE_STANDBY:
		SetState_STANDBY();
		break;
	case RAKLION_STATE_NOTIFY_2:
		SetState_NOTIFY_2();
		break;
	case RAKLION_STATE_READY:
		SetState_READY();
		break;
	case RAKLION_STATE_START_BATTLE:
		SetState_START_BATTLE();
		break;
	case RAKLION_STATE_NOTIFY_3:
		SetState_NOTIFY_3();
		break;
	case RAKLION_STATE_CLOSE_DOOR:
		SetState_CLOSE_DOOR();
		break;
	case RAKLION_STATE_ALL_USER_DIE:
		SetState_ALL_USER_DIE();
		break;
	case RAKLION_STATE_NOTIFY_4:
		SetState_NOTIFY_4();
		break;
	case RAKLION_STATE_END:
		SetState_END();
		break;		
	}

	g_RaklionUtil.NotifyRaklionChangeState( GetRaklionState(), m_BattleOfSelupan.GetBattleOfSelupanState() );
}

// 다음 상태로 상태를 변환한다.
void CRaklion::SetNextState( RAKLION_STATE iCurrentState )
{
	int iNextState = iCurrentState + 1;
	if( iNextState >= RAKLION_STATE_MAX )
	{
		iNextState = RAKLION_STATE_IDLE;
	}
	
	SetState( iNextState );
}

//--------------------------------------------------------------------------------------------------------
// 상태를 변환할 때 해 주어야 할 일들을 명시한다. 
void CRaklion::ChangeState( int iState, int DetailState )
{
}

//--------------------------------------------------------------------------------------------------------
void CRaklion::SetState_IDLE()
{
	//////////////////////////////////////////////////////////////////////////
	// IDLE 상태로 세팅 해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> IDLE", m_iRaklionState );

	SetRaklionState( RAKLION_STATE_IDLE );
}

void CRaklion::SetState_NOTIFY_1()
{
	//////////////////////////////////////////////////////////////////////////
	// 일정수의 거미알이 깨졌다.
	// 클라이언트 연출을 위해 메시지를 전송한다.
	// NOTIFY_1 상태로 세팅 해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> NOTIFY_1", m_iRaklionState );

	SetRaklionState( RAKLION_STATE_NOTIFY_1 );
}

void CRaklion::SetState_STANDBY()
{
	//////////////////////////////////////////////////////////////////////////
	// 거미알이 모두 깨졌다.
	// 클라이언트 연출을 위한 시간만큼 딜레이를 위한 현재 틱카운트를 저장한다.
	// STANDBY 상태로 세팅 해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> STANDBY", m_iRaklionState );

	m_iBossAppearanceStartTime = GetTickCount();

	SetRaklionState( RAKLION_STATE_STANDBY );
}

void CRaklion::SetState_NOTIFY_2()
{
	//////////////////////////////////////////////////////////////////////////
	// 거미알이 모두 깨졌다.
	// 클라이언트에 보스 출현 메시지를 보낸다.
	// NOTIFY_2 상태로 세팅 해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> NOTIFY_2", m_iRaklionState );
	
	INT iDelayMinute = m_iBosszoneCloseDelay / 60;
	if( iDelayMinute == 0 )
		iDelayMinute = 1;

	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3410), iDelayMinute );
	
	SetRaklionState( RAKLION_STATE_NOTIFY_2 );
}

void CRaklion::SetState_READY()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스의 상태를 STANDBY 상태로 해준다.
	// 보스의 상태가 STANDBY 상태가 되면 세루판을 생성하고 초기화 하게 된다.
	// READY 상태로 세팅 해준다.
	//////////////////////////////////////////////////////////////////////////
	
	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> READY", m_iRaklionState );

	// 보스 상태 STANDBY로 세팅
	m_BattleOfSelupan.SetState( BATTLE_OF_SELUPAN_STANDBY );
	
	SetRaklionState( RAKLION_STATE_READY );
}

void CRaklion::SetState_START_BATTLE()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스존 폐쇄를 위해 딜레이 변수에 현재 틱카운를 저장한다.
	// START_BATTLE 상태로 세팅 해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> START_BATTLE", m_iRaklionState );

	m_iBosszoneCloseStartTime = GetTickCount();
	
	SetRaklionState( RAKLION_STATE_START_BATTLE );
}

void CRaklion::SetState_NOTIFY_3()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스존의 입장 불가 메시지를 클라이언트에 보낸다.
	// NOTIFY_3 상태로 세팅 해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> NOTIFY_3", m_iRaklionState );

	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3411) );
	
	SetRaklionState( RAKLION_STATE_NOTIFY_3 );
}

void CRaklion::SetState_CLOSE_DOOR()
{
	//////////////////////////////////////////////////////////////////////////
	// CLOSE_DOOR 상태로 세팅 해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> CLOSE_DOOR", m_iRaklionState );
	
	SetRaklionState( RAKLION_STATE_CLOSE_DOOR );
}

void CRaklion::SetState_ALL_USER_DIE()
{
	//////////////////////////////////////////////////////////////////////////
	// ALL_USER_DIE 상태로 세팅 해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> ALL_USER_DIE", m_iRaklionState );

#ifdef MODIFY_BATTLE_OF_SELUPAN_FAIL_DELETE_IMMEDIATELY_20080917
	// 세루판 초기화
	m_BattleOfSelupan.HideSelupan();
	
	// 소환 몬스터 삭제
	m_BattleOfSelupan.DeleteSummonMonster();
#endif // MODIFY_BATTLE_OF_SELUPAN_FAIL_DELETE_IMMEDIATELY_20080917
	
	SetRaklionState( RAKLION_STATE_ALL_USER_DIE );
}

void CRaklion::SetState_NOTIFY_4()
{
	//////////////////////////////////////////////////////////////////////////
	// NOTIFY_4 상태로 세팅 해준다.
	// 보스전 종료까지 5분간 대기하기 위해 시간을 저장한다.
	// 보스전이 성공 실패인지 클라이언트에 전송해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> NOTIFY_4", m_iRaklionState );

	m_iBosszoneOpenStartTime = GetTickCount();

	BOOL bBattleSuccess = m_BattleOfSelupan.GetSuccessValue();

	// 개방되기 까지의 시간을 분으로 환산하여 메시지 전송
	INT iDelayMinute = m_iBosszoneOpenDelay / 60;
	if( iDelayMinute == 0 )
		iDelayMinute = 1;

	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3413), iDelayMinute );

	// 결과값 전송
	g_RaklionUtil.NotifyRaklionSuccessValue( bBattleSuccess );
	
	SetRaklionState( RAKLION_STATE_NOTIFY_4 );
}

void CRaklion::SetState_END()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스전 초기화
	// END 상태로 세팅해준다.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> END", m_iRaklionState );

	// 세루판 초기화
	m_BattleOfSelupan.HideSelupan();

	// 소환 몬스터 삭제
	m_BattleOfSelupan.DeleteSummonMonster();

	// 보스전 참여 유저 리스트 초기화
//	g_RaklionBattleUserMng.ResetAllData();

	// 보스전 세부 스케쥴링 초기화
	m_BattleOfSelupan.ResetAllData();
	
	// 보스전 전체 스케쥴링 데이터 초기화
	ResetAllData();

	// 보스존 입구 개방 메시지 전송
	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3414) );

	SetRaklionState( RAKLION_STATE_END );

	// 거미알 리젠
	RegenBossEgg();
}

//------------------------------------------------------------------------------------------------
void CRaklion::ProcState_IDLE()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스전이 진행되고 있지 않다.
	// 아무일 없이 대기 한다.
	//////////////////////////////////////////////////////////////////////////

	// 보스전 시작 조건을 체크
	if( GetBossEggDieCount() == 0 )
	{
		SetState( RAKLION_STATE_STANDBY );
	}
	else
	{
		// 아직 NOTIFY_1이 실행되지 않았다면 거미알이 설정치만큼 파괴되었는지 체크
		if( m_bIsNotify1 == FALSE )
		{
			if( GetBossEggDieCount() == m_iBossEggHalf )
			{
				// 거미알이 설정치만큼 파괴되었다면 NOTIFY_1 실행
				SetState( RAKLION_STATE_NOTIFY_1 );
				m_bIsNotify1 = TRUE;
			}
		}
	}
}

void CRaklion::ProcState_NOTIFY_1()
{
	//////////////////////////////////////////////////////////////////////////
	// SetState( RAKLION_STATE_IDLE );
	//////////////////////////////////////////////////////////////////////////

	SetState( RAKLION_STATE_IDLE );
}

void CRaklion::ProcState_STANDBY()
{
	//////////////////////////////////////////////////////////////////////////
	// 정해진 연출 시간만큼 대기 후 NOTIFY_2 상태로 변경한다.
	// SetState( RAKLION_STATE_NOTIFY_2 );
	//////////////////////////////////////////////////////////////////////////

	if( 1000*m_iBossAppearanceDelay <= GetTickCount()-m_iBossAppearanceStartTime )
	{
		SetState( RAKLION_STATE_NOTIFY_2 );
	}
}

void CRaklion::ProcState_NOTIFY_2()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스 등장을 클리어언트에 알렸으므로 상태를 READY상태로 바꿔준다.
	//////////////////////////////////////////////////////////////////////////

	SetState( RAKLION_STATE_READY );
}

void CRaklion::ProcState_READY()
{
	//////////////////////////////////////////////////////////////////////////
	// 세루판전 시작을 준비한다.
	//////////////////////////////////////////////////////////////////////////
	
#ifdef MODIFY_RAKLION_SELUPAN_NO_REGEN_BUGFIX_20080730
//	INT iSelupanIndex = m_BattleOfSelupan.GetSelupanObjIndex();

	// 세루판의 리젠이 완료되지 않았다면 return
//	if( gObj[iSelupanIndex].DieRegen > 0 )
//		return;
#else
	INT iSelupanIndex = m_BattleOfSelupan.GetSelupanObjIndex();
	
	// 세루판의 리젠이 완료되지 않았다면 return
	if( gObj[iSelupanIndex].DieRegen > 0 )
		return;
#endif // MODIFY_RAKLION_SELUPAN_NO_REGEN_BUGFIX_20080730

	SetState( RAKLION_STATE_START_BATTLE );
}

void CRaklion::ProcState_START_BATTLE()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스존이 개방된 상태로 보스전이 진행중이다.
	// 세팅한 시간만큼 현 상태를 유지하다가 NOTIFY_3으로 변경한다.
	// m_BattleOfSelupan.Run() 을 해준다.
	//////////////////////////////////////////////////////////////////////////

	if( 1000*m_iBosszoneCloseDelay < GetTickCount()-m_iBosszoneCloseStartTime )
	{
		SetState( RAKLION_STATE_NOTIFY_3 );
	}

	// 보스전 진행
	m_BattleOfSelupan.SetBattleOfSelupanStateAuto();
	m_BattleOfSelupan.Run();
}

void CRaklion::ProcState_NOTIFY_3()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스전이 시작된 후 세팅된 시간이 지나 보스존이 닫혔다는 메시지를 보냈다.
	// 보스존을 폐쇄하기 위해 CLOSE_DOOR 상태로 변경한다..
	// SetState( RAKLION_STATE_CLOSE_DOOR );
	//////////////////////////////////////////////////////////////////////////

	// 보스전 진행
	m_BattleOfSelupan.SetBattleOfSelupanStateAuto();
	m_BattleOfSelupan.Run();

	SetState( RAKLION_STATE_CLOSE_DOOR );
}

void CRaklion::ProcState_CLOSE_DOOR()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스존이 폐쇄된 상태로 진행중이다.
	// m_BattleOfSelupan.Run() 을 해준다.
	//////////////////////////////////////////////////////////////////////////

	if( g_RaklionBattleUserMng.IsEmpty() == TRUE )
	{	// 보스존의 모든 유저가 사망했다면 결과를 FALSE로 해준다.
		m_BattleOfSelupan.SetSuccessValue( FALSE );
		SetState( RAKLION_STATE_ALL_USER_DIE );
	}
	else
	{
		if( m_BattleOfSelupan.GetBattleOfSelupanState() == BATTLE_OF_SELUPAN_DIE )
		{	// 보스가 죽은 상태라면 결과를 TRUE해준다.
			m_BattleOfSelupan.SetSuccessValue( TRUE );
			SetState( RAKLION_STATE_NOTIFY_4 );
		}
		else
		{	// 종료 상황이 아닐 경우 계속해서 진행한다.
			m_BattleOfSelupan.SetBattleOfSelupanStateAuto();
			m_BattleOfSelupan.Run();
		}
	}
}

void CRaklion::ProcState_ALL_USER_DIE()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스존 폐쇄 후 모든 유저가 죽었다.
	// 보스전의 종료를 알리기 위해 NOTIFY_4 상태로 변경한다.
	//////////////////////////////////////////////////////////////////////////

	SetState( RAKLION_STATE_NOTIFY_4 );
}

void CRaklion::ProcState_NOTIFY_4()
{
	//////////////////////////////////////////////////////////////////////////
	// 보스전이 종료를 클라이언트에 알렸다.
	// 세팅한 시간만큼 대기 후 END 상태로 변경한다.
	//////////////////////////////////////////////////////////////////////////

	if( 1000*m_iBosszoneOpenDelay < GetTickCount()-m_iBosszoneOpenStartTime )
	{
		SetState( RAKLION_STATE_END );
	}
}

void CRaklion::ProcState_END()
{
	//////////////////////////////////////////////////////////////////////////
	// 라클리온 보스전이 완전히 종료되었고 초기화 되었다.
	// IDLE상태로 변경한다.
	// SetState( RAKLION_STATE_IDLE );
	//////////////////////////////////////////////////////////////////////////

	SetState( RAKLION_STATE_IDLE );
}

//---------------------------------------------------------------------------------------
// 라클리온 보스전 가능 여부를 세팅한다.,
void CRaklion::SetRaklionBossEnable( BOOL bEnable )
{
	m_bRaklionBossEnable = bEnable;
}

// 라클리온 보스전이 가능한가?
int CRaklion::IsRaklionBossEnable()
{
#ifdef MODIFY_CRYWOLF_MOVE_RAKLION_BUGFIX_20080910
	if( g_bRaklionEvent && m_bRaklionBossEnable )
		return TRUE;
	
	return FALSE;
#else
	return m_bRaklionBossEnable;
#endif // MODIFY_CRYWOLF_MOVE_RAKLION_BUGFIX_20080910
}

// 라클리온 상태를 세팅한다.
void CRaklion::SetRaklionState( RAKLION_STATE iRaklionState )
{
	m_iRaklionState = iRaklionState;
}

// 현재 라클리온의 상태를 얻어온다.
int CRaklion::GetRaklionState()
{
	return m_iRaklionState;
}

// 보스의 행동 상태를 세팅한다.
void CRaklion::SetRaklionStateDetail( INT iSelupanState )
{
	m_BattleOfSelupan.SetBattleOfSelupanState(iSelupanState);
}

// 보스의 행동 상태를 얻어온다.
int CRaklion::GetRaklionStateDetail()
{
	return m_BattleOfSelupan.GetBattleOfSelupanState();
}

//---------------------------------------------------------------------------------------
// 파괴된 거미알의 수를 증가시킨다.
void CRaklion::BossEggDieIncrease()
{
	if( m_iBossEggDieCount >= m_iBossEggMax )
	{
		// 로그 찍고 에러 발생
		LogAddC( LOGC_RED, "[RAKLION] BossEggDieCount Error : (%d)", m_iBossEggDieCount );
		return;
	}
	else
		++m_iBossEggDieCount;
}

// 파괴된 거미알의 수를 감소시킨다.
void CRaklion::BossEggDieDecrease()
{
	if( m_iBossEggDieCount <= 0 )
	{
		// 로그 찍고 에러 발생
		LogAddC( LOGC_RED, "[RAKLION] BossEggDieCount Error : (%d)", m_iBossEggDieCount );
		return;
	}
	else
		--m_iBossEggDieCount;
}

// 현재 파괴된 거미알의 수를 얻어온다.
DWORD CRaklion::GetBossEggDieCount()
{
	return m_iBossEggDieCount;
}

// 거미알을 모두 리젠시킨다.
void CRaklion::RegenBossEgg()
{
	for( INT iCount=0; iCount<MAX_MONSTER; ++iCount )
	{
		if( gObj[iCount].Class == 460 || gObj[iCount].Class == 461 || gObj[iCount].Class == 462 )
		{
			gObjSetMonster( gObj[iCount].m_Index, gObj[iCount].Class );
			if( gObjMonsterRegen( &gObj[iCount] ) )
			{
				gObj[iCount].Live = 1;
				gObj[iCount].DieRegen = 0;
				
				BossEggDieIncrease();
			}
		}
	}

	LogAddC( LOGC_LIGHTRED, "[ RAKLION ] [ BossEggRegen ] : BossEgg(%d ea)", GetBossEggDieCount() );
}

// 거미알을 모두 없앤다.
void CRaklion::DeleteBossEgg()
{
	for( INT iCount=0; iCount<MAX_MONSTER; ++iCount )
	{
		if( gObj[iCount].Class == 460 || gObj[iCount].Class == 461 || gObj[iCount].Class == 462 )
		{
			gObjDel ( iCount );
			
			gObj[iCount].Live = 0;
			gObj[iCount].m_State = OBJST_NONE;
			
			// 거미알을 뷰포트에서 지운다.
			gObjViewportListProtocolDestroy( &gObj[iCount] );
			gObjViewportClose( &gObj[iCount] );
				
			BossEggDieDecrease();
		}
	}

	LogAddC( LOGC_LIGHTRED, "[ RAKLION ] [ BossEggDelete ] : BossEgg(%d ea)", GetBossEggDieCount() );
}

//--------------------------------------------------------------------------------------------
// 비정상적인 방법으로 맵에 있는 유저는 쫒아낸다.
void CRaklion::CheckUserOnRaklionBossMap()
{
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{		
		if( ( gObj[iCount].Connected == CSS_GAMEPLAYING ) && ( gObj[iCount].Type == OBJTYPE_CHARACTER ) 
			&& gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS 
			)			
		{
#ifdef MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_2ND_20080917
			//if( GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR )		// 보스존이 폐쇄된 상태
#else
			if( GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR )		// 보스존이 폐쇄된 상태
#endif // MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_2ND_20080917
			{
				// 운영자는 괜찮다.
				if( ( gObj[iCount].Authority & AUTHORITY_ADMIN ) != AUTHORITY_ADMIN )
				{
#ifdef MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_20080818
					if( g_RaklionBattleUserMng.IsBattleUser(iCount) )	// 보스전 중인 유저면 넘어가고
						continue;
					else												// 아니라면 추방한다.
					{
						// 보스맵으로 완전히 이동한 후에 옮긴다.
						if( gObj[iCount].RegenOk == 0  && gObj[iCount].m_State == OBJST_PLAYING 
							&& gObj[iCount].Live == 1 )
						{
							// 라클리온 안전지대로 쫓아버린다. ( 접속을 끊는걸로 변경해도 괜찮을 듯. )
							gObjMoveGate( iCount, 286 );
							LogAddC( LOGC_RED, "[ RAKLION ][ Invalid User ] Invalid Raklion Boss Map User[%s][%s]",
								gObj[iCount].AccountID, gObj[iCount].Name );
						}
					}
#else
					// 보스맵으로 완전히 이동한 후에 옮긴다.
					if( gObj[iCount].RegenOk == 0  && gObj[iCount].m_State == OBJST_PLAYING 
						&& gObj[iCount].Live == 1 )
					{
						// 라클리온 안전지대로 쫓아버린다. ( 접속을 끊는걸로 변경해도 괜찮을 듯. )
						gObjMoveGate( iCount, 286 );
						LogAddC( LOGC_RED, "[ RAKLION ][ Invalid User ] Invalid Raklion Boss Map User[%s][%s]",
							gObj[iCount].AccountID, gObj[iCount].Name );
					}
#endif // MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_20080818
				}
			}
		}
	}
}

// 라클리온 보스존에 입장 가능한가?
int CRaklion::CheckEnterRaklion( int iUserIndex )
{
	if( !gObjIsConnected( iUserIndex ) )
		return -1;	

	if( GetRaklionState() == RAKLION_STATE_CLOSE_DOOR )
	{
		// 입장할 수 없음
		LogAddTD( "[ RAKLION ][ Entrance Fail ] [%s][%s] State(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetRaklionState() );
		return 3;		// 보스존이 닫혀서 입장 불가
	}

	// 맵 번호가 다르면 입장할 수 없다.
	if( gObj[iUserIndex].MapNumber != MAP_INDEX_RAKLION_BOSS )
	{
		// 입장할 수 없음
		LogAddTD( "[ RAKLION ][ Entrance Fail ] Invalid Map Number(%d) [%s][%s] State(%d)",
			gObj[iUserIndex].MapNumber,
			gObj[iUserIndex].AccountID,
			gObj[iUserIndex].Name,
			GetRaklionState() );
		return 4;		// 맵번호가 틀리므로 입장 불가
	}

	// 위의 입장 불가 조건들을 통과하면 입장 가능 - return 값이 0이면 입장 가능
	return 0;
}

// 운영자 명령어
void CRaklion::OperateGmCommand( int iUserIndex, int iCommand )
{
	#if ( TESTSERVER != 1 )
	// 테스트중에만 사용 가능하다.
	return;	
#endif

	if( !gObjIsConnected( iUserIndex ) )
		return;

	LogAddTD( "[ RAKLION ][ Use GM Command ] [%s][%s] Command : %d", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, iCommand );
	
	switch( iCommand )
	{
	case RAKLION_GM_COMMAND_IDLE:
		SetState( RAKLION_STATE_END );
		break;
	case RAKLION_GM_COMMAND_NOTIFY_1:
		SetState( RAKLION_STATE_NOTIFY_1 );
		break;
	case RAKLION_GM_COMMAND_STANDBY:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_STANDBY );
		break;
	case RAKLION_GM_COMMAND_NOTIFY_2:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_NOTIFY_2 );
		break;
	case RAKLION_GM_COMMAND_READY:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_READY );
		break;
	case RAKLION_GM_COMMAND_START_BATTLE:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_START_BATTLE );
		break;
	case RAKLION_GM_COMMAND_NOTIFY_3:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_NOTIFY_3 );
		break;
	case RAKLION_GM_COMMAND_CLOSE_DOOR:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_CLOSE_DOOR );
		break;
	case RAKLION_GM_COMMAND_ALL_USER_DIE:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_ALL_USER_DIE );
		break;
	case RAKLION_GM_COMMAND_NOTIFY_4:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_GM_COMMAND_NOTIFY_4 );
		break;
	case RAKLION_GM_COMMAND_END:
		SetState( RAKLION_STATE_END );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_1:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_1 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_2:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_2 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_3:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_3 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_4:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_4 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_5:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_5 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_6:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_6 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_7:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_7 );
		break;
	case RAKLION_GM_COMMAND_FIRST_SKILL:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_FIRST_SKILL );
		break;
	case RAKLION_GM_COMMAND_POISON_ATTACK:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_POISON_ATTACK );
		break;
	case RAKLION_GM_COMMAND_ICE_STORM:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_ICE_STORM );
		break;
	case RAKLION_GM_COMMAND_ICE_STRIKE:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_ICE_STRIKE );
		break;
	case RAKLION_GM_COMMAND_SUMMON_MONSTER:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_SUMMON_MONSTER );
		break;
	case RAKLION_GM_COMMAND_HEAL:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_HEAL );
		break;
	case RAKLION_GM_COMMAND_FREEZE:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_FREEZE );
		break;
	case RAKLION_GM_COMMAND_TELEPORT_RANDOM:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_TELEPORT_RANDOM );
		break;
	case RAKLION_GM_COMMAND_INVINCIBILITY:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_INVINCIBILITY );
		break;
	}	
}

#include "LogToFile.h"
CLogToFile	RAKLION_TEST_LOG( "RAKLION_TEST_LOG", ".\\RAKLION_TEST_LOG");		// 로그파일 객체
void CRaklion::UserMonsterCountCheck()
{	
#if ( TESTSERVER != 1 )
	return;
#endif
	
	// 전투중일 때만 검사
	if( GetRaklionState() != RAKLION_STATE_START_BATTLE && GetRaklionState() != RAKLION_STATE_CLOSE_DOOR )
	{
		return;
	}
	
	// 1. 현재 유저 수 검사 - Manager		
	RAKLION_TEST_LOG.Output( "[ RAKLION ][ Debug ] 1. UserMng Count:%d", g_RaklionBattleUserMng.GetUserCount() );
	
	// 2. 현재 유저 수 검사 - 전체
	int iUserCount_Live = 0;
	int iUserCount_Die = 0;
	for( int iAllUserCount = ALLOC_USEROBJECTSTART; iAllUserCount < MAX_OBJECT; iAllUserCount++ )
	{
		if( gObj[iAllUserCount].MapNumber == MAP_INDEX_RAKLION_BOSS 
			&& gObj[iAllUserCount].Type == OBJTYPE_CHARACTER
			&& ( gObj[iAllUserCount].Authority & AUTHORITY_ADMIN ) != AUTHORITY_ADMIN )
		{
			if( gObj[iAllUserCount].Live == 1 )
			{
				++iUserCount_Live;
			}
			else
			{
				++iUserCount_Die;
			}
		}
	}
	RAKLION_TEST_LOG.Output( "[ RAKLION ][ Debug ] 2. All User Check:%d/(Die:%d)", iUserCount_Live, iUserCount_Die );
	
	// 3. 현재 몬스터 수 검사 - Manager
//	RAKLION_TEST_LOG.Output( "[ RAKLION ][ Debug ] 3. MonsterMng Count:%d", g_RaklionMonsterMng.GetAliveMonsterCount() );
	
	// 4. 현재 몬스터 수 검사 - 전체
	int iMonsterCount = 0;
	for( int iAllMonsterCount = 0; iAllMonsterCount < ALLOC_USEROBJECTSTART; iAllMonsterCount++ )
	{
		if( gObj[iAllMonsterCount].MapNumber == MAP_INDEX_RAKLION_BOSS 
			&& gObj[iAllMonsterCount].Connected == 3
			&& gObj[iAllMonsterCount].Type	== OBJTYPE_MONSTER )
		{
			
			if( gObj[iAllMonsterCount].Class != RAKLION_SELUPAN )		// 세루판 제외
			{
				++iMonsterCount;
				RAKLION_TEST_LOG.Output( "[ RAKLION ][ Debug ] 4. Monster Info %s(%d/ObjIndex:%d) [%d]%d-%d HP:%0.1f IsAlive:%d Connected:%d State:%d ActState:Attack(%d)/Move(%d) DieRegen:%d", 
					gObj[iAllMonsterCount].Name, gObj[iAllMonsterCount].Class, gObj[iAllMonsterCount].m_Index, 
					gObj[iAllMonsterCount].MapNumber, gObj[iAllMonsterCount].X, gObj[iAllMonsterCount].Y,
					gObj[iAllMonsterCount].Life, 
					gObj[iAllMonsterCount].Live, 
					gObj[iAllMonsterCount].Connected, 
					gObj[iAllMonsterCount].m_State, 
					gObj[iAllMonsterCount].m_ActState.Attack,
					gObj[iAllMonsterCount].m_ActState.Move,
					gObj[iAllMonsterCount].DieRegen
					);
			}
		}
	}
}



#endif