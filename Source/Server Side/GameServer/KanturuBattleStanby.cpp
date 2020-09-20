// KanturuBattleStanby.cpp: implementation of the CKanturuBattleStanby class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuBattleStanby.h"
#include "KanturuMonsterMng.h"

#include "KanturuUtil.h"

#include "Include\ReadScript.h"

static CKanturuUtil	KANTURU_UTIL;

#include "KanturuBattleUserMng.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuBattleStanby::CKanturuBattleStanby()
#ifdef MEM_KUZIMO
:m_iBattleStanbyState(0),m_bIsSucccess(FALSE),m_bFileDataLoad(FALSE),m_StateInfoCount(0)
{
	ResetAllData();
#else
{
#endif
	SetBattleStanbyState( KANTURU_BATTLE_STANBY_STATE_NONE );
}

CKanturuBattleStanby::~CKanturuBattleStanby()
{

}

void CKanturuBattleStanby::Init()
{
}

void CKanturuBattleStanby::ResetAllData()
{

	for( int iCount = 0; iCount < MAX_KANTURU_BATTLE_STANBY_STATE; iCount++ )
	{
		m_StateInfo[iCount].ResetTimeInfo();
	}
	
	m_StateInfoCount = 0;
}

BOOL CKanturuBattleStanby::LoadData( LPSTR lpszFileName )
{
	m_bFileDataLoad = FALSE;

	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		MsgBox( "[ KANTURU ][ BattleStanby ] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{		
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )	
		{
			DWORD dwError = GetLastError();
			MsgBox( "[ KANTURU ][ BattleStanby ] - Can't Open %s ", lpszFileName );			
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
				// .> 칸투르 대기상태 각 상태 정보
				if( iType == KANTURU_DATAFILE_LOAD_STANBY_STATE )
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
					if( m_StateInfoCount < 0 || m_StateInfoCount >= MAX_KANTURU_BATTLE_STANBY_STATE )
					{
						MsgBox( "[ KANTURU ][ BattleStanby ] - Exceed Max State Time (%d)", m_StateInfoCount );
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
				else
				{
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) 
						break;
				}				
			}
		}
		fclose( SMDFile );
		
		LogAddC( 2, "[ KANTURU ][ BattleStanby ] %s file is Loaded", lpszFileName );

		m_bFileDataLoad = TRUE;			
	}
	catch(...) 
	{
		MsgBox( "[ KANTURU ][ BattleStanby ] - Loading Exception Error (%s) File. ", lpszFileName );
	}

	return m_bFileDataLoad;	
}

//----------------------------------------------------------------------------
void CKanturuBattleStanby::Run()
{
	// 1. 상태에 따른 시간 제한을 체크한다.
	CheckStateTime();
	
	switch( m_iBattleStanbyState )
	{
	case KANTURU_BATTLE_STANBY_STATE_NONE:
		ProcState_NONE();
		break;

	case KANTURU_BATTLE_STANBY_STATE_START:
		ProcState_START();
		break;

	case KANTURU_BATTLE_STANBY_STATE_NOTIFY:
		ProcState_NOTIFY();
		break;

	case KANTURU_BATTLE_STANBY_STATE_END:
		ProcState_END();
		break;

	default:
		break;
	}
}


//----------------------------------------------------------------------------
void CKanturuBattleStanby::SetState( int iBattleStanbyState )
{
	// 1. 종료조건이 시간이면 상태가 바뀌었을 때 시간을 적용한다.
	if( m_StateInfo[iBattleStanbyState].GetCondition() > KANRUTU_STATE_END_COMDITION_NONE  )
	{
		m_StateInfo[iBattleStanbyState].SetAppliedTime();
	}
			
	// 2. 상태가 바뀔 때에는 리젠되어 있는 모든 몬스터와 칸투르 관련 유저 데이터를 삭제한다.
	g_KanturuMonsterMng.ResetRegenMonsterObjData();
#ifdef ADD_NEW_NPC_KANTURU_20060612	
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		gObj[iCount].m_bKanturuEntranceByNPC = 0;
	}
#endif
	g_KanturuBattleUserMng.ResetAllData();

	// 3. 상태를 변경한다.
	switch( iBattleStanbyState ) 
	{
	case KANTURU_BATTLE_STANBY_STATE_NONE:
		SetState_NONE();
		break;

	case KANTURU_BATTLE_STANBY_STATE_START:
		SetState_START();
		break;

	case KANTURU_BATTLE_STANBY_STATE_NOTIFY:
		SetState_NOTIFY();
		break;
		
	case KANTURU_BATTLE_STANBY_STATE_END:
		SetState_END();
		break;

	default:
		break;
	}

	// 상태가 변경됐음을 클라이언트에 알린다.
	KANTURU_UTIL.NotifyKanturuChangeState( KANTURU_STATE_BATTLE_STANDBY, m_iBattleStanbyState );

	// 전투유저 데이터 초기화 - 대기상태에서는 전투유저 데이터가 필요 없다(매크로로 설정될 경우를 위한 관리)
	g_KanturuBattleUserMng.ResetAllData();
}

void CKanturuBattleStanby::SetNextState( int iCurrentState )
{
	int iNextState = iCurrentState + 1;
	if( iNextState >= MAX_KANTURU_BATTLE_STANBY_STATE )
	{
		iNextState = KANTURU_BATTLE_STANBY_STATE_NONE;
	}

	SetState( iNextState );	
}


//----------------------------------------------------------------------------
void CKanturuBattleStanby::SetState_NONE()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleStanby ] State(%d) -> NONE", m_iBattleStanbyState );

	SetBattleStanbyState( KANTURU_BATTLE_STANBY_STATE_NONE );
}

void CKanturuBattleStanby::SetState_START()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleStanby ] State(%d) -> START", m_iBattleStanbyState );

	SetBattleStanbyState( KANTURU_BATTLE_STANBY_STATE_START );
}	

void CKanturuBattleStanby::SetState_NOTIFY()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleStanby ] State(%d) -> NOTIFY", m_iBattleStanbyState );

	SetBattleStanbyState( KANTURU_BATTLE_STANBY_STATE_NOTIFY );
	
	KANTURU_UTIL.SendMsgKauturuMapUser( lMsg.Get( 3359 ) );	// "1분 후 거대한 힘에게 도전할 수 있습니다."
}

void CKanturuBattleStanby::SetState_END()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleStanby ] State(%d) -> END", m_iBattleStanbyState );

	SetBattleStanbyState( KANTURU_BATTLE_STANBY_STATE_END );
}

void CKanturuBattleStanby::ProcState_NONE()
{
}	

void CKanturuBattleStanby::ProcState_START()
{
}

void CKanturuBattleStanby::ProcState_NOTIFY()
{
}

void CKanturuBattleStanby::ProcState_END()
{
}


//--------------------------------------------------------------------------------------
void CKanturuBattleStanby::CheckStateTime()
{
	// 각 State 당 시간을 체크한다.
	int iCurrentState = GetBattleStanbyState();

	if( iCurrentState != KANTURU_BATTLE_STANBY_STATE_NONE )
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

int CKanturuBattleStanby::GetRemainTime()
{
	int iCurrentState = GetBattleStanbyState();
	return m_StateInfo[iCurrentState].GetRemainTime();
}

#endif