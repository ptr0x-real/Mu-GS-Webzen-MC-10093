// KanturuBattleOfNightmare.cpp: implementation of the CKanturuBattleOfNightmare class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuBattleOfNightmare.h"

#include "KanturuUtil.h"
#include "KanturuBattleUserMng.h"
#include "KanturuMonsterMng.h"

#include "..\Include\ReadScript.h"

#include "TMonsterAIGroup.h"


static CKanturuUtil	KANTURU_UTIL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuBattleOfNightmare::CKanturuBattleOfNightmare()
{
	SetBattleOfNightmareState( BATTLE_OF_NIGHTMARE_STATE_NONE );

	ResetAllData();
	
	m_iNightmareAIGroupNumber	= 0;
	m_iNightmareObjIndex		= -1;	
	
#ifdef MODIFY_KANTURU_NIGHTMARE_INIT_BUGFIX_20061010
	SetSuccessValue( FALSE );
#endif
}

CKanturuBattleOfNightmare::~CKanturuBattleOfNightmare()
{

}

void CKanturuBattleOfNightmare::ResetAllData()
{
	m_StateInfoCount = 0;
	
	for( int iCount = 0; iCount < MAX_BATTLE_OF_NIGHTMARE_STATE; iCount++ )
	{
		m_StateInfo[iCount].ResetTimeInfo();
	}
}

BOOL CKanturuBattleOfNightmare::LoadData( LPSTR lpszFileName )
{
	m_bFileDataLoad = FALSE;

	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		MsgBox( "[ KANTURU ][ BattleOfNightmare ] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{		
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )	
		{
			DWORD dwError = GetLastError();
			MsgBox( "[ KANTURU ][ BattleOfNightmare ] - Can't Open %s ", lpszFileName );			
			return FALSE;
		}
		
		// 모든 데이터 초기화.
		ResetAllData();

		SMDToken Token;		
		int iType = -1;

		int			iState = 0;
		int			iCondition = KANRUTU_STATE_END_COMDITION_NONE;
		int			iValue = 0;	

		while( TRUE )
		{
			Token = GetToken();
			if( Token == END ) break;
			iType = ( int )TokenNumber;

			while( TRUE )
			{				
				// .> 칸투르 기본 상태 시간 데이터
				if( iType == KANTURU_DATAFILE_LOAD_NIGHTMARE_STATE )
				{
					// 1. >	데이터 초기화
					iState		= 0;
					iCondition	= KANRUTU_STATE_END_COMDITION_NONE;
					iValue		= 0;

					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iState = ( int )TokenNumber;
					Token = GetToken(); iCondition	= ( BOOL )TokenNumber;
					Token = GetToken(); iValue		= ( int )TokenNumber;							
						
					// 3. >	데이터 유효성 검사
					if( m_StateInfoCount < 0 || m_StateInfoCount >= MAX_BATTLE_OF_NIGHTMARE_STATE )
					{
						MsgBox( "[ KANTURU ][ BattleOfNightmare ] - Exceed Max State Time (%d)", m_StateInfoCount );
						break;
					}
					
					// 4. > 데이터 세팅
					m_StateInfo[m_StateInfoCount].SetStateInfo( iState );
					m_StateInfo[m_StateInfoCount].SetCondition( iCondition );
					if( iCondition == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
					{
						iValue *= 1000;
					}
					m_StateInfo[m_StateInfoCount].SetValue( iValue );
					
					m_StateInfoCount++;				
				}
				else if( iType == KANTURU_DATAFILE_LOAD_NIGHTMARE_BATTLEMONSTER )
				{
					// 1. >	데이터 초기화
					int iValue = 0;
					
					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;

					iValue = ( int )TokenNumber;
											
					// 3. >	데이터 유효성 검사
					if( iValue < 0 )
					{
						MsgBox( "[ KANTURU ][ BattleOfNightmare ] - Nightmare Hand AI Group Read Error(%d)", iValue );
						iValue =  0;
					}
					// 4. > 데이터 세팅
					m_iNightmareAIGroupNumber = iValue;				
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
		
		LogAddC( 2, "[ KANTURU ][ BattleOfNightmare ]  %s file is Loaded", lpszFileName );

		m_bFileDataLoad = TRUE;			
	}
	catch(...) 
	{
		MsgBox( "[ KANTURU ][ BattleOfNightmare ] - Loading Exception Error (%s) File. ", lpszFileName );
	}

	return m_bFileDataLoad;	
}

//----------------------------------------------------------------------------
void CKanturuBattleOfNightmare::Run()
{
	// 1. 상태에 따른 시간 제한을 체크한다.
	CheckStateTime();

	// 2. 전투중 모든 유저가 사망하면 실패처리한다.
	CheckNightmareBattleUser();

	// 3. 유저의 상태 처리
	g_KanturuBattleUserMng.CheckUserState();

#if TESTSERVER == 1
	LogAddC(LOGC_RED, "남은 유저수 %d", g_KanturuBattleUserMng.GetUserCount());

	int iAliveMonsterCount = g_KanturuMonsterMng.GetAliveMonsterCount();
	if ( iAliveMonsterCount > 0 )
	{
		LogAddC(LOGC_RED, "남은 몬스터 %d", iAliveMonsterCount);
		if ( m_iNightmareObjIndex != -1 )
		{
			int per = (gObj[m_iNightmareObjIndex].Life * 100) / (gObj[m_iNightmareObjIndex].MaxLife + gObj[m_iNightmareObjIndex].AddLife);
			LogAddC(LOGC_RED, "[나이트메어] HP:%0.f (%d%%) AIUnit:%d", gObj[m_iNightmareObjIndex].Life,per,
				gObj[m_iNightmareObjIndex].m_iCurrentAI );

			//DebugLog("[나이트메어] HP:%0.f (%d%%) AIUnit:%d\n", gObj[m_NightmareMonster.m_iNightmareObjIndex].Life,per,				gObj[m_NightmareMonster.m_iNightmareObjIndex].m_iCurrentAI );

		}
	}
#endif

	// 4. 처리
	switch( m_iBattleOfNightmareState )
	{
	case BATTLE_OF_NIGHTMARE_STATE_NONE:
		ProcState_NONE();
		break;

	case BATTLE_OF_NIGHTMARE_STATE_IDLE:
		ProcState_IDLE();
		break;

	case BATTLE_OF_NIGHTMARE_STATE_NOTIFY:
		ProcState_NOTIFY();
		break;

	case BATTLE_OF_NIGHTMARE_STATE_START:
		ProcState_START();
		break;

	case BATTLE_OF_NIGHTMARE_STATE_END:
		ProcState_END();
		break;

	case BATTLE_OF_NIGHTMARE_STATE_ENDCYCLE:
		ProcState_ENDCYCLE();
		break;

	default:
		break;
	}
}
	
//----------------------------------------------------------------------------
void CKanturuBattleOfNightmare::SetState( int iBattleOfNightmareState )
{
	
	// 1. 종료조건이 시간이면 상태가 바뀌었을 때 시간을 적용한다.
	if( m_StateInfo[iBattleOfNightmareState].GetCondition() > KANRUTU_STATE_END_COMDITION_NONE  )
	{
		m_StateInfo[iBattleOfNightmareState].SetAppliedTime();
	}

	// 2. 상태가 바뀔 때에는 리젠되어 있는 모든 몬스터 삭제한다.	
	g_KanturuMonsterMng.ResetRegenMonsterObjData();	
	m_iNightmareObjIndex = -1;

	LogAddTD( "[ KANTURU ][ BattleOfNightmare ] CKanturuBattleOfNightmare::SetState(...)  m_iNightmareObjIndex %d", m_iNightmareObjIndex );

	// 3. 상태를 변경한다.
	switch( iBattleOfNightmareState )
	{
	case BATTLE_OF_NIGHTMARE_STATE_NONE:
		SetState_NONE();
		break;

	case BATTLE_OF_NIGHTMARE_STATE_IDLE:
		SetState_IDLE();
		break;

	case BATTLE_OF_NIGHTMARE_STATE_NOTIFY:
		SetState_NOTIFY();
		break;

	case BATTLE_OF_NIGHTMARE_STATE_START:
		SetState_START();
		break;
		
	case BATTLE_OF_NIGHTMARE_STATE_END:
		SetState_END();
		break;

	case BATTLE_OF_NIGHTMARE_STATE_ENDCYCLE:
		SetState_ENDCYCLE();
		break;

	default:
		break;
	}
	
	// 상태가 변경됐음을 클라이언트에 알린다.
	KANTURU_UTIL.NotifyKanturuChangeState( KANTURU_STATE_BATTLE_OF_NIGHTMARE, m_iBattleOfNightmareState );
}

void CKanturuBattleOfNightmare::SetNextState( int iCurrentState )
{
	int iNextState = iCurrentState + 1;
	if( iNextState >= MAX_BATTLE_OF_NIGHTMARE_STATE )
	{
		iNextState = BATTLE_OF_NIGHTMARE_STATE_NONE;
	}

	SetState( iNextState );	
}

//----------------------------------------------------------------------------
void CKanturuBattleOfNightmare::SetState_NONE()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfNightmare ] State(%d) -> NONE", m_iBattleOfNightmareState );

	SetBattleOfNightmareState( BATTLE_OF_NIGHTMARE_STATE_NONE );

	// 정제의 탑으로 정상 진행 안되는 문제로 삭제 - 20061024 (RKFKA)
//#ifdef MODIFY_KANTURU_NIGHTMARE_INIT_BUGFIX_20061010
//	SetSuccessValue( FALSE );
//#endif

	// 일단 여기서 초기화 
	TMonsterAIGroup::DelGroupInstance( m_iNightmareAIGroupNumber );
	LogAddTD( "[ KANTURU ][ BattleOfNightmare ] DelGroupInstance SetState_NONE" );


}
void CKanturuBattleOfNightmare::SetState_IDLE()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfNightmare ] State(%d) -> IDLE", m_iBattleOfNightmareState );
	
#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	LogAddTD("[ KANTURU ][ BattleOfNightmare ] State(%d) -> IDLE", m_iBattleOfNightmareState );
#endif


	SetBattleOfNightmareState( BATTLE_OF_NIGHTMARE_STATE_IDLE );

	// 일단 여기서 초기화 
#ifdef MODIFY_KANTURU_NIGHTMARE_INIT_BUGFIX_20061010
	SetSuccessValue( FALSE );
#endif
	TMonsterAIGroup::DelGroupInstance( m_iNightmareAIGroupNumber );
	LogAddTD( "[ KANTURU ][ BattleOfNightmare ] DelGroupInstance SetState_IDLE" );

	// 마야전투를 성공한 유저를 나이트메어 전투맵으로 옮긴다.
	g_KanturuBattleUserMng.MoveAllUser( KANTURU_NIGHTMARE_MAP_GATE );
}

void CKanturuBattleOfNightmare::SetState_NOTIFY()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfNightmare ] State(%d) -> NOTIFY", m_iBattleOfNightmareState );

	SetBattleOfNightmareState( BATTLE_OF_NIGHTMARE_STATE_NOTIFY );
}
void CKanturuBattleOfNightmare::SetState_START()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfNightmare ] State(%d) -> START", m_iBattleOfNightmareState );

	SetBattleOfNightmareState( BATTLE_OF_NIGHTMARE_STATE_START );
	
	if( m_StateInfo[m_iBattleOfNightmareState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
	{
		// 전투시간을 보낸다.
		KANTURU_UTIL.NotifyKanturuBattleTime( m_StateInfo[m_iBattleOfNightmareState].GetValue() );
	}

	// 몬스터를 설정한다.
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_NIGHTMAREMONSTER );
	
	TMonsterAIGroup::Init( m_iNightmareAIGroupNumber );
	TMonsterAIGroup::ChangeAIOrder( m_iNightmareAIGroupNumber, MAG_AI_ORDER_DEFAULT );
	int iIndex = TMonsterAIGroup::FindGroupLeader( m_iNightmareAIGroupNumber );	
	m_iNightmareObjIndex = iIndex;		// 나이트메어 index

	LogAddTD( "[ KANTURU ][ BattleOfNightmare ] CKanturuNightmare Set m_iNightmareObjIndex %d", m_iNightmareObjIndex );
	
}

void CKanturuBattleOfNightmare::SetState_END()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfNightmare ] State(%d) -> END", m_iBattleOfNightmareState );

	SetBattleOfNightmareState( BATTLE_OF_NIGHTMARE_STATE_END );

	// 성공/ 실패에 대한 정보를 클라이언트에 알린다.
	KANTURU_UTIL.NotifyKanturuSuccessValue( GetSuccessValue() );

	// 실패한 경우 메세지 통보
	if( GetSuccessValue() == KANTURU_BATTLE_FAIL )
	{
		KANTURU_UTIL.SendMsgKanturuBattleUser( lMsg.Get( 3364 ) );	// "제한시간이 지나 실패하였습니다. 곧 칸투르 안전지역으로 강제소환 됩니다."
		LogAddTD( "[ KANTURU ][ BattleOfNightmare ] Fail!! TimeOut" );
	}
}

void CKanturuBattleOfNightmare::SetState_ENDCYCLE()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfNightmare ] State(%d) -> ENDCYCLE", m_iBattleOfNightmareState );

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	LogAddTD("[ KANTURU ][ BattleOfNightmare ] State(%d) -> ENDCYCLE", m_iBattleOfNightmareState );
#endif

	SetBattleOfNightmareState( BATTLE_OF_NIGHTMARE_STATE_ENDCYCLE );

	// 성공한 경우 메세지 통보
	if( GetSuccessValue() == KANTURU_BATTLE_SUCCESS )
	{
		KANTURU_UTIL.SendMsgKanturuBattleUser( lMsg.Get( 3363 ) );	// "나이트메어를 무찔렀습니다. 이제 정제의 탑으로 가는 길이 열렸습니다."
	}
	else
	{
		g_KanturuBattleUserMng.MoveAllUser( KANTURU_DEFAULT_REGEN_MAP_GATE );
		g_KanturuBattleUserMng.ResetAllData();
	}
}

//----------------------------------------------------------------------------
void CKanturuBattleOfNightmare::ProcState_NONE()
{
}
void CKanturuBattleOfNightmare::ProcState_IDLE()
{
}
void CKanturuBattleOfNightmare::ProcState_NOTIFY()
{
}
void CKanturuBattleOfNightmare::ProcState_START()
{
	KANTURU_UTIL.NotifyKanturuUserMonsterCount( g_KanturuMonsterMng.GetAliveMonsterCount(), g_KanturuBattleUserMng.GetUserCount() );
	
	// 나이트메어가 죽으면 성공
	int iIndex = m_iNightmareObjIndex;
	if( gObj[iIndex].Live == 0 )	// 나이트메어가 죽었는지에 대한 판단 확인해야함!
	{
		SetSuccessValue( TRUE );
		LogAddTD( "[ KANTURU ][ BattleOfNightmare ] Success!! Nightmare Die" );

		// 남아있는 유저에 대한 정보와 경과 시간을 로그로 남긴다.
		g_KanturuBattleUserMng.LogBattleWinnerUserInfo( KANTURU_BATTLE_SUCCESS_LOG_NIGHTMARE, 
								m_StateInfo[m_iBattleOfNightmareState].GetElapsedTime() );	

		SetNextState( m_iBattleOfNightmareState );
	}
}
void CKanturuBattleOfNightmare::ProcState_END()
{
}
void CKanturuBattleOfNightmare::ProcState_ENDCYCLE()
{
	// 일단 여기서 초기화 
	TMonsterAIGroup::DelGroupInstance( m_iNightmareAIGroupNumber );
	LogAddTD( "[ KANTURU ][ BattleOfNightmare ] DelGroupInstance ProcState_ENDCYCLE" );
}

//--------------------------------------------------------------------------------------
void CKanturuBattleOfNightmare::CheckStateTime()
{
	// 각 State 당 시간을 체크한다.
	int iCurrentState = GetBattleOfNightmareState();

	if( iCurrentState != BATTLE_OF_NIGHTMARE_STATE_NONE )
	{		
		if( m_StateInfo[iCurrentState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
		{			
			if( m_StateInfo[iCurrentState].IsTimeOut() == TRUE )
			{
				SetNextState( iCurrentState );
			}
		}
	}
}

int CKanturuBattleOfNightmare::GetRemainTime()
{
	int iCurrentState = GetBattleOfNightmareState();
	return m_StateInfo[iCurrentState].GetRemainTime();
}

void CKanturuBattleOfNightmare::CheckNightmareBattleUser()
{
	// 전투중 모든 전투유저가 사망하면 실패처리	
	if( GetBattleOfNightmareState() == BATTLE_OF_NIGHTMARE_STATE_NOTIFY 
		|| GetBattleOfNightmareState() == BATTLE_OF_NIGHTMARE_STATE_START )
	{	
		if( g_KanturuBattleUserMng.IsEmpty() == TRUE )
		{
			// 전투유저 데이터 초기화
			g_KanturuBattleUserMng.ResetAllData();

			// 실패 적용
			SetSuccessValue( FALSE );
			
			LogAddTD( "[ KANTURU ][ BattleOfNightmare ] Fail!! All Battle User Die" );
			SetState( BATTLE_OF_NIGHTMARE_STATE_END );
		}
	}
}

#endif