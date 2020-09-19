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
		
		// ��� ������ �ʱ�ȭ.
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
				// .> ĭ���� ������ �� ���� ����
				if( iType == KANTURU_DATAFILE_LOAD_STANBY_STATE )
				{
					// 1. >	������ �ʱ�ȭ
					iState		= 0;
					iCondition	= KANRUTU_STATE_END_COMDITION_NONE;
					iValue		= 0;

					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iState = ( int )TokenNumber;
					Token = GetToken(); iCondition	= ( BOOL )TokenNumber;
					Token = GetToken(); iValue		= ( int )TokenNumber;							
						
					// 3. >	������ ��ȿ�� �˻�
					if( m_StateInfoCount < 0 || m_StateInfoCount >= MAX_KANTURU_BATTLE_STANBY_STATE )
					{
						MsgBox( "[ KANTURU ][ BattleStanby ] - Exceed Max State Time (%d)", m_StateInfoCount );
						break;
					}
					
					// 4. > ������ ����
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
	// 1. ���¿� ���� �ð� ������ üũ�Ѵ�.
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
	// 1. ���������� �ð��̸� ���°� �ٲ���� �� �ð��� �����Ѵ�.
	if( m_StateInfo[iBattleStanbyState].GetCondition() > KANRUTU_STATE_END_COMDITION_NONE  )
	{
		m_StateInfo[iBattleStanbyState].SetAppliedTime();
	}
			
	// 2. ���°� �ٲ� ������ �����Ǿ� �ִ� ��� ���Ϳ� ĭ���� ���� ���� �����͸� �����Ѵ�.
	g_KanturuMonsterMng.ResetRegenMonsterObjData();
#ifdef ADD_NEW_NPC_KANTURU_20060612	
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		gObj[iCount].m_bKanturuEntranceByNPC = 0;
	}
#endif
	g_KanturuBattleUserMng.ResetAllData();

	// 3. ���¸� �����Ѵ�.
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

	// ���°� ��������� Ŭ���̾�Ʈ�� �˸���.
	KANTURU_UTIL.NotifyKanturuChangeState( KANTURU_STATE_BATTLE_STANDBY, m_iBattleStanbyState );

	// �������� ������ �ʱ�ȭ - �����¿����� �������� �����Ͱ� �ʿ� ����(��ũ�η� ������ ��츦 ���� ����)
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
	
	KANTURU_UTIL.SendMsgKauturuMapUser( lMsg.Get( 3359 ) );	// "1�� �� �Ŵ��� ������ ������ �� �ֽ��ϴ�."
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
	// �� State �� �ð��� üũ�Ѵ�.
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