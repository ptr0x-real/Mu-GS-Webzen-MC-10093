// KanturuBattleOfMaya.cpp: implementation of the CKanturuBattleOfMaya class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuBattleOfMaya.h"

#include "KanturuBattleUserMng.h"
#include "KanturuUtil.h"
#include "KanturuMonsterMng.h"

#include "Include\ReadScript.h"

#include "TMonsterAIGroup.h"

static CKanturuUtil	KANTURU_UTIL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuBattleOfMaya::CKanturuBattleOfMaya()
{
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_NONE );
	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_NONE );

	m_iMayaLeftHandObjIndex		= -1;
	m_iMayaRightHandObjIndex	= -1;

	for( int iCount = 0; iCount < 4; iCount++ )
	{
		m_iMayaHandAIChangeTime[iCount]	= -1;
	}
	
	m_iMayaHandAIAppliedTime		= 0;
	
	m_iMayaHandCurrentAI			= 0;

	for( int iCount = 0; iCount < MAX_MAYA_HAND_AI_GROUP_NUMBER; iCount++ )
	{
		m_iMayaHandGroupNumber[iCount] = -1;
	}	
	
	m_iMayaHandDieTimeCounter = 0;

	ResetAllData();
}

CKanturuBattleOfMaya::~CKanturuBattleOfMaya()
{

}

void CKanturuBattleOfMaya::ResetAllData()
{		
	m_bFileDataLoad				= FALSE;

	for( int iCount = 0; iCount < MAX_BATTLE_STATE_BATTLE_OF_MAYA; iCount++ )
	{
		m_BattleTimeInfo[iCount].ResetTimeInfo();
	}

	for( int iCount = 0; iCount < MAX_BATTLE_OF_MAYA_STATE; iCount++ )
	{
		m_StateInfo[iCount].ResetTimeInfo();
	}

	m_StateInfoCount			= 0;
	m_BattleTimeInfoCount		= 0;
	
	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );
	SetEntrancePermit( FALSE );

	m_iMayaHandAIChangeTimeCount	= 0;
	m_iMayaHAndGroupNumberCount		= 0;

	m_iMayaIceStormUsingRate		= 0;
}

BOOL CKanturuBattleOfMaya::LoadData( LPSTR lpszFileName )
{
	m_bFileDataLoad = FALSE;

	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		MsgBox( "[ KANTURU ][ BattleOfMaya ] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{		
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )	
		{
			MsgBox( "[ KANTURU ][ BattleOfMaya ] - Can't Open %s ", lpszFileName );
			return FALSE;
		}
		
		// ��� ������ �ʱ�ȭ.
		ResetAllData();

		SMDToken	Token;	
		int			iType = -1;

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
				// .> ������ ���⺰ ���ѽð� ������
				if( iType == KANTURU_DATAFILE_LOAD_MAYA_SCENE_STATE )
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
					if( m_BattleTimeInfoCount < 0 || m_BattleTimeInfoCount >= MAX_BATTLE_STATE_BATTLE_OF_MAYA )
					{
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Exceed Max Scene (%d)", m_BattleTimeInfoCount );
						break;
					}
					
					// 4. > ������ ����
					m_BattleTimeInfo[m_BattleTimeInfoCount].SetStateInfo( iState );
					m_BattleTimeInfo[m_BattleTimeInfoCount].SetCondition( iCondition );
					if( iCondition == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
					{
						iValue *= 1000;
					}
					m_BattleTimeInfo[m_BattleTimeInfoCount].SetValue( iValue );
					
					m_BattleTimeInfoCount++;
				}				
				// .> ������ �� ���� �⺻ �ð� ������
				else if( iType == KANTURU_DATAFILE_LOAD_MAYA_STATE )
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
					if( m_StateInfoCount < 0 || m_StateInfoCount >= MAX_BATTLE_OF_MAYA_STATE )
					{
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Exceed Max State Time (%d)", m_StateInfoCount );
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
				// .> ������ ���� �� AI Change �ð�
				else if( iType == KANTURU_DATAFILE_LOAD_MAYA_HAND_AICHANGE_TIME )
				{
					// 1. >	������ �ʱ�ȭ					
					int iValue	= 0;	
					
					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;

					iValue = ( int )TokenNumber;
											
					// 3. >	������ ��ȿ�� �˻�
					if( m_iMayaHandAIChangeTimeCount < 0 || m_iMayaHandAIChangeTimeCount >= MAX_AI_STATE )
					{
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Exceed Maya Hand AI Change Time(%d)", m_iMayaHandAIChangeTime );												
						break;
					}

					// 4. > ������ ����
					m_iMayaHandAIChangeTime[m_iMayaHandAIChangeTimeCount] = iValue * 1000;

					++m_iMayaHandAIChangeTimeCount;
				}

				// .> ������ ���� �� AI Group Number
				else if( iType == KANTURU_DATAFILE_LOAD_MAYA_HAND_GROUP_NUMBER )
				{
					// 1. >	������ �ʱ�ȭ
					int iValue = 0;
					
					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;

					iValue = ( int )TokenNumber;
											
					// 3. >	������ ��ȿ�� �˻�
					if( m_iMayaHAndGroupNumberCount < 0 || m_iMayaHAndGroupNumberCount >= MAX_MAYA_HAND_AI_GROUP_NUMBER )
					{
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Maya Hand AI Group Read Error(%d)", m_iMayaHAndGroupNumberCount );
						break;
					}
					// 4. > ������ ����
					m_iMayaHandGroupNumber[m_iMayaHAndGroupNumberCount] = iValue;					

					m_iMayaHAndGroupNumberCount++;
				}
				else if( iType == KANTURU_DATAFILE_LOAD_MAYA_ICESTORM_USING_RATE )
				{
					// 1. >	������ �ʱ�ȭ
					int iValue = 0;
					
					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;

					iValue = ( int )TokenNumber;
											
					// 3. >	������ ��ȿ�� �˻�
					if( iValue < 0 )
					{
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Maya IceStorm Using Rate Load Error(%d)", iValue );
						iValue = 60;				// ������ 60���� �����Ѵ�.			
					}

					// 4. > ������ ����
					m_iMayaIceStormUsingRate = iValue;
					LogAddTD( "[ KANTURU ][ BattleOfMaya ] IceStorm Using Rate Load: %d", m_iMayaIceStormUsingRate );
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
		
		LogAddC( 2, "[ KANTURU ][ BattleOfMaya ] %s file is Loaded", lpszFileName );

		m_bFileDataLoad = TRUE;
	}
	catch(...) 
	{
		MsgBox( "[ KANTURU ][ BattleOfMaya ] - Loading Exception Error (%s) File. ", lpszFileName );
	}

	return m_bFileDataLoad;	
}

//------------------------------------------------------------
void CKanturuBattleOfMaya::Run()
{
	// 1. ���¿� ���� �ð� ������ üũ�Ѵ�.
	CheckStateTime();

	// 2. ������ ��� ����ϸ� ����
	CheckMayaBattleUser();

	// 3. ������ ���� ó��
	g_KanturuBattleUserMng.CheckUserState();

	// 4. ó��
	switch( m_iBattleOfMayaState )
	{
	case BATTLE_OF_MAYA_STATE_NONE:
		ProcState_NONE();
		break;

	case BATTLE_OF_MAYA_STATE_STANBY1:
		ProcState_STANBY1();
		break;

	case BATTLE_OF_MAYA_STATE_NOTIFY1:
		ProcState_NOTIFY1();
		break;

	case BATTLE_OF_MAYA_STATE_START1:
		ProcState_START1();
		break;

	case BATTLE_OF_MAYA_STATE_MAYA1:
		ProcState_MAYA1();
		break;

	case BATTLE_OF_MAYA_STATE_END_MAYA1:
		ProcState_END_MAYA1();
		break;

	case BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA1:
		ProcState_ENDCYCLE_MAYA1();
		break;		
			
	case BATTLE_OF_MAYA_STATE_STANBY2:
		ProcState_STANBY2();
		break;

	case BATTLE_OF_MAYA_STATE_START2:
		ProcState_START2();
		break;

	case BATTLE_OF_MAYA_STATE_MAYA2:
		ProcState_MAYA2();
		break;

	case BATTLE_OF_MAYA_STATE_END_MAYA2:
		ProcState_END_MAYA2();
		break;
		
	case BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA2:
		ProcState_ENDCYCLE_MAYA2();
		break;
			
	case BATTLE_OF_MAYA_STATE_STANBY3:
		ProcState_STANBY3();
		break;

	case BATTLE_OF_MAYA_STATE_START3:
		ProcState_START3();
		break;

	case BATTLE_OF_MAYA_STATE_MAYA3:
		ProcState_MAYA3();
		break;

	case BATTLE_OF_MAYA_STATE_END_MAYA3:
		ProcState_END_MAYA3();
		break;

	case BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA3:
		ProcState_ENDCYCLE_MAYA3();
		break;			
			
	case BATTLE_OF_MAYA_STATE_END:
		ProcState_END();
		break;

	case BATTLE_OF_MAYA_STATE_ENDCYCLE:
		ProcState_ENDCYCLE();  
		break;
			
	default:
		break;
	}

}

//------------------------------------------------------------
void CKanturuBattleOfMaya::SetState( int iBattleOfMayaState )
{	
	// 1. ���������� �ð��̸� ���°� �ٲ���� �� �ð��� �����Ѵ�.
	if( m_StateInfo[iBattleOfMayaState].GetCondition() > KANRUTU_STATE_END_COMDITION_NONE )
	{
		m_StateInfo[iBattleOfMayaState].SetAppliedTime();
	}

	// 2. ���°� �ٲ� ������ �����Ǿ� �ִ� ��� ���� �����Ѵ�.
	g_KanturuMonsterMng.ResetRegenMonsterObjData();
	m_KanturuMaya.Init();	

	// 4. AI�� ����ϴ� ������ ó��
	ResetAIMonster();
	
	// 3. ���¸� �����Ѵ�.
	switch( iBattleOfMayaState )
	{
	case BATTLE_OF_MAYA_STATE_NONE:
		SetState_NONE();
		break;

	case BATTLE_OF_MAYA_STATE_STANBY1:
		SetState_STANBY1();
		break;
		
	case BATTLE_OF_MAYA_STATE_NOTIFY1:
		SetState_NOTIFY1();
		break;

	case BATTLE_OF_MAYA_STATE_START1:
		SetState_START1();
		break;

	case BATTLE_OF_MAYA_STATE_MAYA1:
		SetState_MAYA1();
		break;

	case BATTLE_OF_MAYA_STATE_END_MAYA1:
		SetState_END_MAYA1();
		break;

	case BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA1:
		SetState_ENDCYCLE_MAYA1();
		break;
		
	case BATTLE_OF_MAYA_STATE_STANBY2:
		SetState_STANBY2();
		break;

	case BATTLE_OF_MAYA_STATE_START2:
		SetState_START2();
		break;

	case BATTLE_OF_MAYA_STATE_MAYA2:
		SetState_MAYA2();
		break;

	case BATTLE_OF_MAYA_STATE_END_MAYA2:
		SetState_END_MAYA2();
		break;

	case BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA2:
		SetState_ENDCYCLE_MAYA2();
		break;
			
	case BATTLE_OF_MAYA_STATE_STANBY3:
		SetState_STANBY3();
		break;

	case BATTLE_OF_MAYA_STATE_START3:
		SetState_START3();
		break;

	case BATTLE_OF_MAYA_STATE_MAYA3:
		SetState_MAYA3();
		break;

	case BATTLE_OF_MAYA_STATE_END_MAYA3:
		SetState_END_MAYA3();
		break;

	case BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA3:
		SetState_ENDCYCLE_MAYA3();
		break;

	case BATTLE_OF_MAYA_STATE_END:
		SetState_END();
		break;

		
	case BATTLE_OF_MAYA_STATE_ENDCYCLE:
		SetState_ENDCYCLE();  
		break;
			
	default:
		break;
	}

	// ���°� ��������� Ŭ���̾�Ʈ�� �˸���.
	KANTURU_UTIL.NotifyKanturuChangeState( KANTURU_STATE_BATTLE_OF_MAYA, m_iBattleOfMayaState );
}

void CKanturuBattleOfMaya::SetNextState( int iCurrentState )
{
	int iNextState = iCurrentState + 1;
	if( iNextState >= MAX_BATTLE_OF_MAYA_STATE )
	{
		iNextState = BATTLE_OF_MAYA_STATE_NONE;
	}
	SetState( iNextState );	
}

//------------------------------------------------------------
void CKanturuBattleOfMaya::SetState_NONE()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> NONE", m_iBattleOfMayaState);
	
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_NONE );

	// ������ �� ����
	SetEntrancePermit( FALSE );
}

void CKanturuBattleOfMaya::SetState_STANBY1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY1", m_iBattleOfMayaState );

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	LogAddTD("[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY1", m_iBattleOfMayaState );
#endif

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_STANBY1 );

	// �������� ������ �ʱ�ȭ
	g_KanturuBattleUserMng.ResetAllData();

	// ���ð����� ������ ������ ���� �� �ִ�.
	SetEntrancePermit( TRUE );
}

void CKanturuBattleOfMaya::SetState_NOTIFY1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> NOTIFY1", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_NOTIFY1 );

	// �������� ���¿��� ������ ������ ���� �� �ִ�.
	SetEntrancePermit( FALSE );	
}


void CKanturuBattleOfMaya::SetState_START1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> START1", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_START1 );
	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_FIRST );

	// �����߿��� ������ ������ �� ����.
	SetEntrancePermit( FALSE );	

	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	m_BattleTimeInfo[m_iSceneState].SetAppliedTime();		// ���� �ð� ����
	if( m_BattleTimeInfo[m_iSceneState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
	{
		// ���� ���� �ð��� ������.
		KANTURU_UTIL.NotifyKanturuBattleTime( m_BattleTimeInfo[m_iSceneState].GetValue() );
	}

	// ���͸� �����Ѵ�.
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYAMONSTER1 );	
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );
}

void CKanturuBattleOfMaya::SetState_MAYA1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> MAYA1", m_iBattleOfMayaState );
	
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_MAYA1 );

	// �����߿��� ������ ������ �� ����.
	SetEntrancePermit( FALSE );	

	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	// ���͸� �����Ѵ�.
	// ����
	m_iMayaHandDieTimeCounter = 0;		// ���� ���� �׾��� �� ���� �ð�üũ�� ���� ����
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYA_LEFTHAND );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );

	// ���� �޼� ����
	SetAIMonsterGroup( m_iMayaHandGroupNumber[MAYA_LEFT_HAND_GROUP_NUMBER] );
	m_iMayaLeftHandObjIndex		= TMonsterAIGroup::FindGroupLeader( m_iMayaHandGroupNumber[MAYA_LEFT_HAND_GROUP_NUMBER] );
}

void CKanturuBattleOfMaya::SetState_END_MAYA1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END_MAYA1", m_iBattleOfMayaState );

	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_NONE );	
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_END_MAYA1 );

	// ������ ������ �� ����.
	SetEntrancePermit( FALSE );	

	// ����, ���� ���θ� Ŭ���̾�Ʈ�� �˸���.
	KANTURU_UTIL.NotifyKanturuSuccessValue( GetSuccessValue() );
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE_MAYA1", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA1 );

	// ������ ������ �� ����.
	SetEntrancePermit( FALSE );
	
	KANTURU_UTIL.SendMsgKauturuMapUser( lMsg.Get( 3361 ) );	// "1�� �� �ٽ� �� �� ������ ���۵˴ϴ�."
}

void CKanturuBattleOfMaya::SetState_STANBY2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY2", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_STANBY2 );

	// ���ð����� ������ ������ ���� �� �ִ�.
	SetEntrancePermit( TRUE );	
}

void CKanturuBattleOfMaya::SetState_START2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> START2", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_START2 );

	// �����߿��� ������ ������ �� ����.
	SetEntrancePermit( FALSE );
	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_SECOND );

	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	m_BattleTimeInfo[m_iSceneState].SetAppliedTime();		// ���� �ð� ����	
	if( m_BattleTimeInfo[m_iSceneState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
	{
		// ���� ���� �ð��� ������.
		KANTURU_UTIL.NotifyKanturuBattleTime( m_BattleTimeInfo[m_iSceneState].GetValue() );
	}

	// ���͸� �����Ѵ�.
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYAMONSTER1 );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );
}

void CKanturuBattleOfMaya::SetState_MAYA2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> MAYA2", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_MAYA2 );

	// �����߿��� ������ ������ �� ����.
	SetEntrancePermit( FALSE );	
	
	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	// ���͸� �����Ѵ�.
	m_iMayaHandDieTimeCounter = 0;		// ���� ���� �׾��� �� ���� �ð�üũ�� ���� ����
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYA_RIGHTHAND );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );

	// ���� ������ ����
	SetAIMonsterGroup( m_iMayaHandGroupNumber[MAYA_RIGHT_HAND_GROUP_NUMBER] );
	m_iMayaRightHandObjIndex	= TMonsterAIGroup::FindGroupLeader( m_iMayaHandGroupNumber[MAYA_RIGHT_HAND_GROUP_NUMBER] );
}

void CKanturuBattleOfMaya::SetState_END_MAYA2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END_MAYA2", m_iBattleOfMayaState );

	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_NONE );
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_END_MAYA2 );

	// ������ ������ �� ����.
	SetEntrancePermit( FALSE );

	// ����, ���� ���θ� Ŭ���̾�Ʈ�� �˸���.
	KANTURU_UTIL.NotifyKanturuSuccessValue( GetSuccessValue() );
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE_MAYA2", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA2 );

	// ������ ������ �� ����.
	SetEntrancePermit( FALSE );	

	KANTURU_UTIL.SendMsgKauturuMapUser( lMsg.Get( 3361 ) );	// "1�� �� �ٽ� �� �� ������ ���۵˴ϴ�."
}

void CKanturuBattleOfMaya::SetState_STANBY3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY3", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_STANBY3 );

	// ���ð����� ������ ������ ���� �� �ִ�.
	SetEntrancePermit( TRUE );
}

void CKanturuBattleOfMaya::SetState_START3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> START3", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_START3 );
	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_THIRD );

	// �����߿��� ������ ������ �� ����.
	SetEntrancePermit( FALSE );	
	
	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	m_BattleTimeInfo[m_iSceneState].SetAppliedTime();		// ���� �ð� ����	
	if( m_BattleTimeInfo[m_iSceneState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
	{
		// ���� ���� �ð��� ������.	
		KANTURU_UTIL.NotifyKanturuBattleTime( m_BattleTimeInfo[m_iSceneState].GetValue() );
	}

	// ���͸� �����Ѵ�.
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYAMONSTER3 );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );
}

void CKanturuBattleOfMaya::SetState_MAYA3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> MAYA3", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_MAYA3 );

	// �����߿��� ������ ������ �� ����.
	SetEntrancePermit( FALSE );	

	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );
	
	// ���͸� �����Ѵ�.
	m_iMayaHandDieTimeCounter = 0;		// ���� ���� �׾��� �� ���� �ð�üũ�� ���� ����	
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYA_HANDS );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );

	// ���� ������ ����
	SetAIMonsterGroup( m_iMayaHandGroupNumber[MAYA_HANDS_GROUP_NUMBER] );
	m_iMayaLeftHandObjIndex		= TMonsterAIGroup::FindGroupLeader( m_iMayaHandGroupNumber[MAYA_HANDS_GROUP_NUMBER] );
	m_iMayaRightHandObjIndex	= TMonsterAIGroup::FindGroupMemberObjectIndex( m_iMayaHandGroupNumber[MAYA_HANDS_GROUP_NUMBER], 1 );
}

void CKanturuBattleOfMaya::SetState_END_MAYA3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END_MAYA3", m_iBattleOfMayaState );

	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_NONE );
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_END_MAYA3 );

	// ������ ������ �� ����.
	SetEntrancePermit( FALSE );

	// ����, ���� ���θ� Ŭ���̾�Ʈ�� �˸���.
	KANTURU_UTIL.NotifyKanturuSuccessValue( GetSuccessValue() );
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE_MAYA3", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA3 );

	// ������ ������ �� ����.
	SetEntrancePermit( FALSE );	
}

void CKanturuBattleOfMaya::SetState_END()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END", m_iBattleOfMayaState );
	
	// �������¸� �ʱ�ȭ�Ѵ�.
	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_NONE );	
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_END );

	// ������ ������ �� ����.
	SetEntrancePermit( FALSE );
	
	if( GetSuccessValue() == KANTURU_BATTLE_SUCCESS )
	{
		KANTURU_UTIL.SendMsgKanturuBattleUser( lMsg.Get( 3362 ) );	// "�� ����Ʈ�޾���� ������ ���۵� ���Դϴ�."
	}
	else
	{
		// ���������� Ŭ���̾�Ʈ�� �ϸ���.
		KANTURU_UTIL.NotifyKanturuSuccessValue( GetSuccessValue() );
		KANTURU_UTIL.SendMsgKanturuBattleUser( lMsg.Get( 3364 ) );	// "���ѽð��� ���� �����Ͽ����ϴ�. �� ĭ���� ������������ ������ȯ �˴ϴ�."	
	}

	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] Success Value[%d]", GetSuccessValue() );
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE", m_iBattleOfMayaState );

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	// ĭ���� ������ ������¸� ����� ���� �α� ����
	LogAddTD("[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE", m_iBattleOfMayaState );
#endif
	
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_ENDCYCLE );

	// ������ ������ �� ����.
	SetEntrancePermit( FALSE );	

	// ������ ��� ��� ���� ������ ĭ���� 2���� ��������� �ű�� ������ �ʱ�ȭ
	if( GetSuccessValue() == KANTURU_BATTLE_FAIL )
	{
		g_KanturuBattleUserMng.MoveAllUser( KANTURU_DEFAULT_REGEN_MAP_GATE );
		g_KanturuBattleUserMng.ResetAllData();
	}
}

//------------------------------------------------------------
void CKanturuBattleOfMaya::ProcState_NONE()
{
}

void CKanturuBattleOfMaya::ProcState_STANBY1()
{
}

void CKanturuBattleOfMaya::ProcState_NOTIFY1()
{
}

void CKanturuBattleOfMaya::ProcState_START1()
{
	if( g_KanturuMonsterMng.IsExistAliveMonster() == FALSE )
	{
		// ����! ���� ���·� �̵�
		SetSuccessValue( TRUE );
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

		SetNextState( m_iBattleOfMayaState );
		return;
	}

	// ���Ϳ��� ���������� ���� �ൿ
	m_KanturuMaya.KanturuMayaAct_IceStorm( m_iMayaIceStormUsingRate );

	KANTURU_UTIL.NotifyKanturuUserMonsterCount( g_KanturuMonsterMng.GetAliveMonsterCount(), g_KanturuBattleUserMng.GetUserCount() );
}

void CKanturuBattleOfMaya::ProcState_MAYA1()
{
#ifdef MEM_KUZIMO
	if(m_iMayaLeftHandObjIndex == -1) return;
#endif

	// ������ ���� �׾����� Ȯ���Ѵ�.
	if( !gObj[m_iMayaLeftHandObjIndex].Live )
	{
		if( m_iMayaHandDieTimeCounter == 0 )
		{
			// ����! 
			SetSuccessValue( TRUE );
			SetSceneSuccessValue( TRUE );
			LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

			// �����ִ� ������ ���� ������ ��� �ð��� �α׷� �����.
			g_KanturuBattleUserMng.LogBattleWinnerUserInfo( KANTURU_BATTLE_SUCCESS_LOG_MAYA1, 
									m_BattleTimeInfo[m_iSceneState].GetElapsedTime() );			

			// ���� ���� �׾����� �˸���.
			KANTURU_UTIL.NotifyKanturuUserMonsterCount( 0, g_KanturuBattleUserMng.GetUserCount() );

			m_iMayaHandDieTimeCounter = GetTickCount();
		}
		
		if( GetTickCount() - m_iMayaHandDieTimeCounter >= 3000 )	// ���� �� �ִϸ��̼� ó���� ���� 3���� �����̸� �ش�.
		{
			// ���� ���·� �̵�
			SetNextState( m_iBattleOfMayaState );
		}
		
		return;
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuUserMonsterCount( 1, g_KanturuBattleUserMng.GetUserCount() );
	}
	
	// AI ���� ����
	ChangeAI( m_iMayaHandGroupNumber[MAYA_LEFT_HAND_GROUP_NUMBER] );


	// ���� �հ��� �������� ���� �ൿ
	m_KanturuMaya.KanturuMayaAct_Hands();
}

void CKanturuBattleOfMaya::ProcState_END_MAYA1()
{
}

void CKanturuBattleOfMaya::ProcState_ENDCYCLE_MAYA1()
{
}

void CKanturuBattleOfMaya::ProcState_STANBY2()
{
}

void CKanturuBattleOfMaya::ProcState_START2()
{
	if( g_KanturuMonsterMng.IsExistAliveMonster() == FALSE )
	{
		// ����! ���� ���·� �̵�
		SetSuccessValue( TRUE );		
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

		SetNextState( m_iBattleOfMayaState );
		return;
	}
	
	// ���Ϳ��� ���������� ���� �ൿ
	m_KanturuMaya.KanturuMayaAct_IceStorm( m_iMayaIceStormUsingRate );	

	KANTURU_UTIL.NotifyKanturuUserMonsterCount( g_KanturuMonsterMng.GetAliveMonsterCount(), g_KanturuBattleUserMng.GetUserCount() );
}

void CKanturuBattleOfMaya::ProcState_MAYA2()
{
#ifdef MEM_KUZIMO
	if(m_iMayaRightHandObjIndex == -1) return;
#endif

	// ������ �������� �׾����� Ȯ���Ѵ�.
	if( !gObj[m_iMayaRightHandObjIndex].Live )
	{		
		if( m_iMayaHandDieTimeCounter == 0 )
		{
			// ����! 
			SetSuccessValue( TRUE );
			SetSceneSuccessValue( TRUE );
			LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

			// �����ִ� ������ ���� ������ ��� �ð��� �α׷� �����.
			g_KanturuBattleUserMng.LogBattleWinnerUserInfo( KANTURU_BATTLE_SUCCESS_LOG_MAYA2, 
									m_BattleTimeInfo[m_iSceneState].GetElapsedTime() );	

			// ���� ���� �׾����� �˸���.
			KANTURU_UTIL.NotifyKanturuUserMonsterCount( 0, g_KanturuBattleUserMng.GetUserCount() );
			m_iMayaHandDieTimeCounter = GetTickCount();
		}
				
		if( GetTickCount() - m_iMayaHandDieTimeCounter >= 3000 )	// ���� �� �ִϸ��̼� ó���� ���� 3���� �����̸� �ش�.
		{
			// ���� ���·� �̵�
			SetNextState( m_iBattleOfMayaState );
		}		

		return;
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuUserMonsterCount( 1, g_KanturuBattleUserMng.GetUserCount() );
	}
	
	// AI ���� ����
	ChangeAI( m_iMayaHandGroupNumber[MAYA_RIGHT_HAND_GROUP_NUMBER] );

	// ���� �հ��� �������� ���� �ൿ
	m_KanturuMaya.KanturuMayaAct_Hands();
}

void CKanturuBattleOfMaya::ProcState_END_MAYA2()
{
}

void CKanturuBattleOfMaya::ProcState_ENDCYCLE_MAYA2()
{
}

void CKanturuBattleOfMaya::ProcState_STANBY3()
{
}

void CKanturuBattleOfMaya::ProcState_START3()
{
	
	if( g_KanturuMonsterMng.IsExistAliveMonster() == FALSE )
	{
		// ����! ���� ���·� �̵�
		SetSuccessValue( TRUE );
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );
		
		SetNextState( m_iBattleOfMayaState );
		return;
	}

	// ���Ϳ��� ���������� ���� �ൿ
	m_KanturuMaya.KanturuMayaAct_IceStorm( m_iMayaIceStormUsingRate );

	KANTURU_UTIL.NotifyKanturuUserMonsterCount( g_KanturuMonsterMng.GetAliveMonsterCount(), g_KanturuBattleUserMng.GetUserCount() );
}

void CKanturuBattleOfMaya::ProcState_MAYA3()
{
	// ������ �޼�, �������� ��� �׾����� Ȯ���Ѵ�.
	int iAliveCount = 0;
#ifdef MEM_KUZIMO
	if( m_iMayaLeftHandObjIndex != -1 )
	{
		if( gObj[m_iMayaLeftHandObjIndex].Live )
			++iAliveCount;
	}
#else
	if( gObj[m_iMayaLeftHandObjIndex].Live )
		++iAliveCount;
#endif

#ifdef MEM_KUZIMO
	if( m_iMayaRightHandObjIndex != -1 )
	{
		if( gObj[m_iMayaRightHandObjIndex].Live )
			++iAliveCount;
	}
#else
	if( gObj[m_iMayaRightHandObjIndex].Live )
		++iAliveCount;
#endif
		
	if( iAliveCount == 0 )
	{
		if( m_iMayaHandDieTimeCounter == 0 )
		{
			// ����! 
			SetSuccessValue( TRUE );
			SetSceneSuccessValue( TRUE );
			LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

			// �����ִ� ������ ���� ������ ��� �ð��� �α׷� �����.
			g_KanturuBattleUserMng.LogBattleWinnerUserInfo( KANTURU_BATTLE_SUCCESS_LOG_MAYA3, 
									m_BattleTimeInfo[m_iSceneState].GetElapsedTime() );	

			m_iMayaHandDieTimeCounter = GetTickCount();
		}
		
		if( GetTickCount() - m_iMayaHandDieTimeCounter >= 3000 )	// ���� �� �ִϸ��̼� ó���� ���� 3���� �����̸� �ش�.
		{
			// ���� ���·� �̵�
			SetNextState( m_iBattleOfMayaState );
			return;
		}		
	}

	KANTURU_UTIL.NotifyKanturuUserMonsterCount( iAliveCount, g_KanturuBattleUserMng.GetUserCount() );
	
	// AI ���� ����
	ChangeAI( m_iMayaHandGroupNumber[MAYA_HANDS_GROUP_NUMBER] );

	// ���� �հ��� �������� ���� �ൿ
	m_KanturuMaya.KanturuMayaAct_Hands();
}

void CKanturuBattleOfMaya::ProcState_END_MAYA3()
{
}

void CKanturuBattleOfMaya::ProcState_ENDCYCLE_MAYA3()
{
}

void CKanturuBattleOfMaya::ProcState_END()
{
}

void CKanturuBattleOfMaya::ProcState_ENDCYCLE()
{
}

//------------------------------------------------------------
// �� State�� ���������� üũ�Ѵ�.
BOOL CKanturuBattleOfMaya::CheckStateTime()
{
	// �� State �� �ð��� üũ�Ѵ�.
	if( GetBattleOfMayaState() != BATTLE_OF_MAYA_STATE_NONE )
	{		
		if( m_StateInfo[m_iBattleOfMayaState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
		{			
			if( m_StateInfo[m_iBattleOfMayaState].IsTimeOut() == TRUE )
			{
				SetNextState( m_iBattleOfMayaState );
			}
		}
	}
	
	// �� ���� �� �ð��� üũ�Ѵ�.
	if( GetBattleSceneState() != BATTLE_OF_MAYA_BATTLE_NONE )
	{		
		if( m_BattleTimeInfo[m_iSceneState].GetCondition() != KANRUTU_STATE_END_COMDITION_TIMELIMIT )
		{
			return TRUE;
		}
		
		// �����ߴٸ� ���ѽð� üũ�� �׸��Ѵ�.
		if( GetSceneSuccessValue() == TRUE )
		{
			return TRUE;
		}

		if( m_BattleTimeInfo[m_iSceneState].IsTimeOut() == TRUE )
		{
			// ���� �ð��� ������.
			if( GetBattleOfMayaState() != BATTLE_OF_MAYA_STATE_END )
			{
				// 1�� ���� ������ ����
				SetSceneSuccessValue( FALSE );
				SetSuccessValue( FALSE );
				LogAddTD( "[ KANTURU ][ BattleOfMaya ] Fail!! TimeOut - DetailState:%d", m_iBattleOfMayaState );

				SetState( BATTLE_OF_MAYA_STATE_END );
			}
		}
	}
	
	return TRUE;
}

int CKanturuBattleOfMaya::GetRemainTime()
{
	int iCurrentState = GetBattleOfMayaState();
	return m_StateInfo[iCurrentState].GetRemainTime();
}

void CKanturuBattleOfMaya::CheckMayaBattleUser()
{
	// ������ ��� ���������� ����ϸ� ����ó��	
	if( GetBattleOfMayaState() == BATTLE_OF_MAYA_STATE_NOTIFY1
		|| GetBattleOfMayaState() == BATTLE_OF_MAYA_STATE_START1 
		|| GetBattleOfMayaState() == BATTLE_OF_MAYA_STATE_MAYA1 
		|| GetBattleOfMayaState() == BATTLE_OF_MAYA_STATE_START2 
		|| GetBattleOfMayaState() == BATTLE_OF_MAYA_STATE_MAYA2 
		|| GetBattleOfMayaState() == BATTLE_OF_MAYA_STATE_START3 
		|| GetBattleOfMayaState() == BATTLE_OF_MAYA_STATE_MAYA3 )
	{	
		if( g_KanturuBattleUserMng.IsEmpty() == TRUE )
		{
			SetSuccessValue( FALSE );
			SetSceneSuccessValue( FALSE );
			LogAddTD( "[ KANTURU ][ BattleOfMaya ] Fail!! All Battle User Die - DetailState:%d", m_iBattleOfMayaState );

			SetState( BATTLE_OF_MAYA_STATE_END );
		}
	}
}

//------------------------------------------------------------
void CKanturuBattleOfMaya::SetAIMonsterGroup( int iGroupNumber )
{
	// ���� ��
	TMonsterAIGroup::Init( iGroupNumber );
	TMonsterAIGroup::ChangeAIOrder( iGroupNumber, MAG_AI_ORDER_DEFAULT );
	m_iMayaHandAIAppliedTime	= GetTickCount();
}

// ���°� �ٲ� ������ AI�� ����ϴ� ������ ��ϵ� AI ������ �����Ѵ�.
void CKanturuBattleOfMaya::ResetAIMonster()
{	
	// �ϴ� ���δ�.
#ifdef MEM_KUZIMO
	if(m_iMayaLeftHandObjIndex != -1)	gObj[m_iMayaLeftHandObjIndex].Life = 0;
	if(m_iMayaRightHandObjIndex != -1)	gObj[m_iMayaRightHandObjIndex].Life = 0;
#else
	gObj[m_iMayaLeftHandObjIndex].Life = 0;
	gObj[m_iMayaRightHandObjIndex].Life = 0;
#endif


	// ��ϵ� ���� ���� AI ����
	for( int iCount = 0; iCount < MAX_AI_STATE; iCount++ )
	{
		TMonsterAIGroup::DelGroupInstance( m_iMayaHandGroupNumber[iCount] );
	}

	m_iMayaLeftHandObjIndex		= -1;
	m_iMayaRightHandObjIndex	= -1;

	m_iMayaHandAIAppliedTime	= 0;
	m_iMayaHandCurrentAI		= MAG_AI_ORDER_DEFAULT;
	
}

void CKanturuBattleOfMaya::ChangeAI( int iGroupNumber )
{
	// AI ���¸� �����Ѵ�.
	if( GetTickCount() - m_iMayaHandAIAppliedTime >= m_iMayaHandAIChangeTime[m_iMayaHandCurrentAI] )
	{
		m_iMayaHandAIAppliedTime = GetTickCount();

		// ������ AI ���´� ��� �����ȴ�.
		if( m_iMayaHandCurrentAI != MAG_AI_ORDER_THIRD )
		{
			// ���� �ð� ���·� �ѱ��.
			m_iMayaHandCurrentAI += 1;
			TMonsterAIGroup::ChangeAIOrder( iGroupNumber, m_iMayaHandCurrentAI );			
		}
	}
}



#endif