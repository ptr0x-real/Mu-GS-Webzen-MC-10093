// KanturuTowerOfRefinement.cpp: implementation of the CKanturuTowerOfRefinement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuTowerOfRefinement.h"

#include "KanturuUtil.h"

#include "Include\ReadScript.h"

static CKanturuUtil	KANTURU_UTIL;

#include "KanturuBattleUserMng.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuTowerOfRefinement::CKanturuTowerOfRefinement()
{
	SetTowerOfRefinementState( TOWER_OF_REFINEMENT_STATE_NONE );
	SetEntrancePermit( FALSE );
	SetUseTowerOfRefinement( FALSE );

	m_StateInfoCount = 0;
}

CKanturuTowerOfRefinement::~CKanturuTowerOfRefinement()
{

}

void CKanturuTowerOfRefinement::Init()
{
}

void CKanturuTowerOfRefinement::ResetAllData()
{
	m_StateInfoCount = 0;
	
	for( int iCount = 0; iCount < MAX_TOWER_OF_REFINEMENT_STATE; iCount++ )
	{
		m_StateInfo[iCount].ResetTimeInfo();
	}
}

BOOL CKanturuTowerOfRefinement::LoadData( LPSTR lpszFileName )
{
	m_bFileDataLoad = FALSE;

	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		MsgBox( "[ KANTURU ][ TowerOfRefinement ] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{		
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )	
		{
			DWORD dwError = GetLastError();
			MsgBox( "[ KANTURU ][ TowerOfRefinement ] - Can't Open %s ", lpszFileName );			
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
				// .> 정제의 탑 상태 시간 데이터
				if( iType == KANTURU_DATAFILE_LOAD_TOWEROFREFINEMENT_STATE )
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
					if( m_StateInfoCount < 0 || m_StateInfoCount >= MAX_TOWER_OF_REFINEMENT_STATE )
					{
						MsgBox( "[ KANTURU ][ TowerOfRefinement ] - Exceed Max State Time (%d)", m_StateInfoCount );
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
		
		LogAddC( 2, "[ KANTURU ][ TowerOfRefinement ] %s file is Loaded", lpszFileName );

		m_bFileDataLoad = TRUE;			
	}
	catch(...) 
	{
		MsgBox( "[ KANTURU ][ TowerOfRefinement ] - Loading Exception Error (%s) File. ", lpszFileName );
	}

	return m_bFileDataLoad;	
}

//---------------------------------------------------------------------
void CKanturuTowerOfRefinement::Run()
{
	// 1. 상태에 따른 시간 제한을 체크한다.
	CheckStateTime();

	switch( m_iTowerOfRefinementState )
	{
	case TOWER_OF_REFINEMENT_STATE_NONE:
		ProcState_NONE();
		break;

	case TOWER_OF_REFINEMENT_STATE_REVITALIZATION:
		ProcState_REVITALIZATION();
		break;
	
	case TOWER_OF_REFINEMENT_STATE_NOTIFY1:
		ProcState_NOTIFY1();
		break;
	
	case TOWER_OF_REFINEMENT_STATE_CLOSE:
		ProcState_CLOSE();
		break;

	case TOWER_OF_REFINEMENT_STATE_NOTIFY2:
		ProcState_NOTIFY2();
		break;		

	case TOWER_OF_REFINEMENT_STATE_END:
		ProcState_END();
		break;
	}
}
	
//---------------------------------------------------------------------
void CKanturuTowerOfRefinement::SetState( int iTowerOfRefinement )
{
	if( m_StateInfo[iTowerOfRefinement].GetCondition() > KANRUTU_STATE_END_COMDITION_NONE  )
	{
		m_StateInfo[iTowerOfRefinement].SetAppliedTime();
	}
	
	switch( iTowerOfRefinement )
	{
	case TOWER_OF_REFINEMENT_STATE_NONE:
		SetState_NONE();
		break;

	case TOWER_OF_REFINEMENT_STATE_REVITALIZATION:
		SetState_REVITALIZATION();
		break;

	case TOWER_OF_REFINEMENT_STATE_NOTIFY1:
		SetState_NOTIFY1();
		break;
	
	case TOWER_OF_REFINEMENT_STATE_CLOSE:
		SetState_CLOSE();
		break;

	case TOWER_OF_REFINEMENT_STATE_NOTIFY2:
		SetState_NOTIFY2();
		break;

	case TOWER_OF_REFINEMENT_STATE_END:
		SetState_END();
		break;
	}	

	// 상태가 변경됐음을 클라이언트에 알린다.
	KANTURU_UTIL.NotifyKanturuChangeState( KANTURU_STATE_TOWER_OF_REFINEMENT, m_iTowerOfRefinementState );

	// 전투유저 데이터 초기화 - 정제의 탑에서는 전투유저 데이터가 필요 없다(매크로로 설정될 경우를 위한 관리)
	g_KanturuBattleUserMng.ResetAllData();
}

void CKanturuTowerOfRefinement::SetNextState( int iCurrentState )
{
	int iNextState = iCurrentState + 1;
	if( iNextState >= MAX_TOWER_OF_REFINEMENT_STATE )
	{
		iNextState = TOWER_OF_REFINEMENT_STATE_NONE;
	}
	
	SetState( iNextState );
}

//---------------------------------------------------------------------
void CKanturuTowerOfRefinement::SetState_NONE()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ TowerOfRefinement ] State(%d) -> NONE", m_iTowerOfRefinementState );

	SetTowerOfRefinementState( TOWER_OF_REFINEMENT_STATE_NONE );

	// 입장 불가능하다
	SetEntrancePermit( FALSE );
	// 정제의탑 이용 불가능
	SetUseTowerOfRefinement( FALSE );	

	// 전투유저 데이터 초기화
	g_KanturuBattleUserMng.ResetAllData();
}
void CKanturuTowerOfRefinement::SetState_REVITALIZATION()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ TowerOfRefinement ] State(%d) -> REVITALIZATION", m_iTowerOfRefinementState );

	SetTowerOfRefinementState( TOWER_OF_REFINEMENT_STATE_REVITALIZATION );
	
	// 입장 가능하다
	SetEntrancePermit( TRUE );
	// 정제의탑 이용 가능
	SetUseTowerOfRefinement( TRUE );
}
void CKanturuTowerOfRefinement::SetState_NOTIFY1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ TowerOfRefinement ] State(%d) -> NOTIFY1", m_iTowerOfRefinementState );

	SetTowerOfRefinementState( TOWER_OF_REFINEMENT_STATE_NOTIFY1 );

	// 입장 가능하다
	SetEntrancePermit( TRUE );
	// 정제의탑 이용 가능
	SetUseTowerOfRefinement( TRUE );

	KANTURU_UTIL.SendMsgKauturuMapUser( lMsg.Get( 3357 ) );	// "정제의 탑으로 향하는 길이 약 한시간 후에 다시 봉인됩니다."
}
void CKanturuTowerOfRefinement::SetState_CLOSE()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ TowerOfRefinement ] State(%d) -> CLOSE", m_iTowerOfRefinementState );

	SetTowerOfRefinementState( TOWER_OF_REFINEMENT_STATE_CLOSE );
	
	// 입장 할 수 없다.
	SetEntrancePermit( FALSE );
	// 정제의탑 이용 가능
	SetUseTowerOfRefinement( TRUE );
	
	KANTURU_UTIL.SendMsgKauturuMapUser( lMsg.Get( 3358 ) );
}
void CKanturuTowerOfRefinement::SetState_NOTIFY2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ TowerOfRefinement ] State(%d) -> NOTIFY2", m_iTowerOfRefinementState );

	SetTowerOfRefinementState( TOWER_OF_REFINEMENT_STATE_NOTIFY2 );

	// 입장 할 수 없다.
	SetEntrancePermit( FALSE );
	// 정제의탑 이용 가능
	SetUseTowerOfRefinement( TRUE );	
}
void CKanturuTowerOfRefinement::SetState_END()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ TowerOfRefinement ] State(%d) -> END", m_iTowerOfRefinementState );

	SetTowerOfRefinementState( TOWER_OF_REFINEMENT_STATE_END );

	// 입장 할 수 없다.
	SetEntrancePermit( FALSE );
	// 정제의탑 이용 할 수 없다.
	SetUseTowerOfRefinement( FALSE );

	// 칸투르 보스전 맵 안에 있는 모든 유저를 쫓아낸다.
	for( int iCount = 0; iCount < MAX_OBJECT; iCount++ )
	{
		if( gObj[iCount].Type == OBJTYPE_CHARACTER
			&& gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS 
			)
		{
			gObjMoveGate( iCount, KANTURU_KICK_OUT_FROM_TOWER );
			LogAddTD( "[ KANTURU ][ TowerOfRefinement ] State End:Kick Out [%s][%s]"
				, gObj[iCount].AccountID, gObj[iCount].Name );
		}
	}	
}

//---------------------------------------------------------------------
void CKanturuTowerOfRefinement::ProcState_NONE()
{
}
void CKanturuTowerOfRefinement::ProcState_REVITALIZATION()
{
}
void CKanturuTowerOfRefinement::ProcState_NOTIFY1()
{
}
void CKanturuTowerOfRefinement::ProcState_CLOSE()
{
}
void CKanturuTowerOfRefinement::ProcState_NOTIFY2()
{
}
void CKanturuTowerOfRefinement::ProcState_END()
{
}

//--------------------------------------------------------------------------------------
void CKanturuTowerOfRefinement::CheckStateTime()
{
	// 각 State 당 시간을 체크한다.
	int iCurrentState = GetTowerOfRefinementState();

	if( iCurrentState != TOWER_OF_REFINEMENT_STATE_NONE )
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

int CKanturuTowerOfRefinement::GetRemainTime()
{
	int iCurrentState = GetTowerOfRefinementState();
	return m_StateInfo[iCurrentState].GetRemainTime();
}

#endif