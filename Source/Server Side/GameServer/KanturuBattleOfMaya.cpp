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
		
		// 모든 데이터 초기화.
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
				// .> 마야전 연출별 제한시간 데이터
				if( iType == KANTURU_DATAFILE_LOAD_MAYA_SCENE_STATE )
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
					if( m_BattleTimeInfoCount < 0 || m_BattleTimeInfoCount >= MAX_BATTLE_STATE_BATTLE_OF_MAYA )
					{
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Exceed Max Scene (%d)", m_BattleTimeInfoCount );
						break;
					}
					
					// 4. > 데이터 세팅
					m_BattleTimeInfo[m_BattleTimeInfoCount].SetStateInfo( iState );
					m_BattleTimeInfo[m_BattleTimeInfoCount].SetCondition( iCondition );
					if( iCondition == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
					{
						iValue *= 1000;
					}
					m_BattleTimeInfo[m_BattleTimeInfoCount].SetValue( iValue );
					
					m_BattleTimeInfoCount++;
				}				
				// .> 마야전 각 상태 기본 시간 데이터
				else if( iType == KANTURU_DATAFILE_LOAD_MAYA_STATE )
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
					if( m_StateInfoCount < 0 || m_StateInfoCount >= MAX_BATTLE_OF_MAYA_STATE )
					{
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Exceed Max State Time (%d)", m_StateInfoCount );
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
				// .> 마야전 마야 손 AI Change 시간
				else if( iType == KANTURU_DATAFILE_LOAD_MAYA_HAND_AICHANGE_TIME )
				{
					// 1. >	데이터 초기화					
					int iValue	= 0;	
					
					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;

					iValue = ( int )TokenNumber;
											
					// 3. >	데이터 유효성 검사
					if( m_iMayaHandAIChangeTimeCount < 0 || m_iMayaHandAIChangeTimeCount >= MAX_AI_STATE )
					{
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Exceed Maya Hand AI Change Time(%d)", m_iMayaHandAIChangeTime );												
						break;
					}

					// 4. > 데이터 세팅
					m_iMayaHandAIChangeTime[m_iMayaHandAIChangeTimeCount] = iValue * 1000;

					++m_iMayaHandAIChangeTimeCount;
				}

				// .> 마야전 마야 손 AI Group Number
				else if( iType == KANTURU_DATAFILE_LOAD_MAYA_HAND_GROUP_NUMBER )
				{
					// 1. >	데이터 초기화
					int iValue = 0;
					
					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;

					iValue = ( int )TokenNumber;
											
					// 3. >	데이터 유효성 검사
					if( m_iMayaHAndGroupNumberCount < 0 || m_iMayaHAndGroupNumberCount >= MAX_MAYA_HAND_AI_GROUP_NUMBER )
					{
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Maya Hand AI Group Read Error(%d)", m_iMayaHAndGroupNumberCount );
						break;
					}
					// 4. > 데이터 세팅
					m_iMayaHandGroupNumber[m_iMayaHAndGroupNumberCount] = iValue;					

					m_iMayaHAndGroupNumberCount++;
				}
				else if( iType == KANTURU_DATAFILE_LOAD_MAYA_ICESTORM_USING_RATE )
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
						MsgBox( "[ KANTURU ][ BattleOfMaya ] - Maya IceStorm Using Rate Load Error(%d)", iValue );
						iValue = 60;				// 강제로 60으로 설정한다.			
					}

					// 4. > 데이터 세팅
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
	// 1. 상태에 따른 시간 제한을 체크한다.
	CheckStateTime();

	// 2. 유저가 모두 사망하면 실패
	CheckMayaBattleUser();

	// 3. 유저의 상태 처리
	g_KanturuBattleUserMng.CheckUserState();

	// 4. 처리
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
	// 1. 종료조건이 시간이면 상태가 바뀌었을 때 시간을 적용한다.
	if( m_StateInfo[iBattleOfMayaState].GetCondition() > KANRUTU_STATE_END_COMDITION_NONE )
	{
		m_StateInfo[iBattleOfMayaState].SetAppliedTime();
	}

	// 2. 상태가 바뀔 때에는 리젠되어 있는 모든 몬스터 삭제한다.
	g_KanturuMonsterMng.ResetRegenMonsterObjData();
	m_KanturuMaya.Init();	

	// 4. AI를 사용하는 몬스터의 처리
	ResetAIMonster();
	
	// 3. 상태를 변경한다.
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

	// 상태가 변경됐음을 클라이언트에 알린다.
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

	// 입장할 수 없다
	SetEntrancePermit( FALSE );
}

void CKanturuBattleOfMaya::SetState_STANBY1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY1", m_iBattleOfMayaState );

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	LogAddTD("[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY1", m_iBattleOfMayaState );
#endif

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_STANBY1 );

	// 전투유저 데이터 초기화
	g_KanturuBattleUserMng.ResetAllData();

	// 대기시간에는 유저의 입장을 받을 수 있다.
	SetEntrancePermit( TRUE );
}

void CKanturuBattleOfMaya::SetState_NOTIFY1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> NOTIFY1", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_NOTIFY1 );

	// 강제연출 상태에는 유저의 입장을 받을 수 있다.
	SetEntrancePermit( FALSE );	
}


void CKanturuBattleOfMaya::SetState_START1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> START1", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_START1 );
	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_FIRST );

	// 전투중에는 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );	

	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	m_BattleTimeInfo[m_iSceneState].SetAppliedTime();		// 전투 시간 설정
	if( m_BattleTimeInfo[m_iSceneState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
	{
		// 전투 연출 시간을 보낸다.
		KANTURU_UTIL.NotifyKanturuBattleTime( m_BattleTimeInfo[m_iSceneState].GetValue() );
	}

	// 몬스터를 생성한다.
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYAMONSTER1 );	
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );
}

void CKanturuBattleOfMaya::SetState_MAYA1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> MAYA1", m_iBattleOfMayaState );
	
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_MAYA1 );

	// 전투중에는 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );	

	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	// 몬스터를 생성한다.
	// 마야
	m_iMayaHandDieTimeCounter = 0;		// 마야 손이 죽었을 때 부터 시간체크를 위한 설정
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYA_LEFTHAND );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );

	// 마야 왼손 등장
	SetAIMonsterGroup( m_iMayaHandGroupNumber[MAYA_LEFT_HAND_GROUP_NUMBER] );
	m_iMayaLeftHandObjIndex		= TMonsterAIGroup::FindGroupLeader( m_iMayaHandGroupNumber[MAYA_LEFT_HAND_GROUP_NUMBER] );
}

void CKanturuBattleOfMaya::SetState_END_MAYA1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END_MAYA1", m_iBattleOfMayaState );

	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_NONE );	
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_END_MAYA1 );

	// 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );	

	// 성공, 실패 여부를 클라이언트에 알린다.
	KANTURU_UTIL.NotifyKanturuSuccessValue( GetSuccessValue() );
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA1()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE_MAYA1", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA1 );

	// 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );
	
	KANTURU_UTIL.SendMsgKauturuMapUser( lMsg.Get( 3361 ) );	// "1분 후 다시 한 번 전투가 시작됩니다."
}

void CKanturuBattleOfMaya::SetState_STANBY2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY2", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_STANBY2 );

	// 대기시간에는 유저의 입장을 받을 수 있다.
	SetEntrancePermit( TRUE );	
}

void CKanturuBattleOfMaya::SetState_START2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> START2", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_START2 );

	// 전투중에는 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );
	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_SECOND );

	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	m_BattleTimeInfo[m_iSceneState].SetAppliedTime();		// 전투 시간 설정	
	if( m_BattleTimeInfo[m_iSceneState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
	{
		// 전투 연출 시간을 보낸다.
		KANTURU_UTIL.NotifyKanturuBattleTime( m_BattleTimeInfo[m_iSceneState].GetValue() );
	}

	// 몬스터를 생성한다.
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYAMONSTER1 );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );
}

void CKanturuBattleOfMaya::SetState_MAYA2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> MAYA2", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_MAYA2 );

	// 전투중에는 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );	
	
	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	// 몬스터를 생성한다.
	m_iMayaHandDieTimeCounter = 0;		// 마야 손이 죽었을 때 부터 시간체크를 위한 설정
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYA_RIGHTHAND );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );

	// 마야 오른손 등장
	SetAIMonsterGroup( m_iMayaHandGroupNumber[MAYA_RIGHT_HAND_GROUP_NUMBER] );
	m_iMayaRightHandObjIndex	= TMonsterAIGroup::FindGroupLeader( m_iMayaHandGroupNumber[MAYA_RIGHT_HAND_GROUP_NUMBER] );
}

void CKanturuBattleOfMaya::SetState_END_MAYA2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END_MAYA2", m_iBattleOfMayaState );

	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_NONE );
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_END_MAYA2 );

	// 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );

	// 성공, 실패 여부를 클라이언트에 알린다.
	KANTURU_UTIL.NotifyKanturuSuccessValue( GetSuccessValue() );
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA2()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE_MAYA2", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA2 );

	// 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );	

	KANTURU_UTIL.SendMsgKauturuMapUser( lMsg.Get( 3361 ) );	// "1분 후 다시 한 번 전투가 시작됩니다."
}

void CKanturuBattleOfMaya::SetState_STANBY3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> STANBY3", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_STANBY3 );

	// 대기시간에는 유저의 입장을 받을 수 있다.
	SetEntrancePermit( TRUE );
}

void CKanturuBattleOfMaya::SetState_START3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> START3", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_START3 );
	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_THIRD );

	// 전투중에는 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );	
	
	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );

	m_BattleTimeInfo[m_iSceneState].SetAppliedTime();		// 전투 시간 설정	
	if( m_BattleTimeInfo[m_iSceneState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
	{
		// 전투 연출 시간을 보낸다.	
		KANTURU_UTIL.NotifyKanturuBattleTime( m_BattleTimeInfo[m_iSceneState].GetValue() );
	}

	// 몬스터를 생성한다.
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYAMONSTER3 );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );
}

void CKanturuBattleOfMaya::SetState_MAYA3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> MAYA3", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_MAYA3 );

	// 전투중에는 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );	

	SetSuccessValue( FALSE );
	SetSceneSuccessValue( FALSE );
	
	// 몬스터를 생성한다.
	m_iMayaHandDieTimeCounter = 0;		// 마야 손이 죽었을 때 부터 시간체크를 위한 설정	
	g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_MAYA_HANDS );
	m_KanturuMaya.SetMayaObjIndex( g_KanturuMonsterMng.GetMayaObjIndex() );

	// 마야 오른손 등장
	SetAIMonsterGroup( m_iMayaHandGroupNumber[MAYA_HANDS_GROUP_NUMBER] );
	m_iMayaLeftHandObjIndex		= TMonsterAIGroup::FindGroupLeader( m_iMayaHandGroupNumber[MAYA_HANDS_GROUP_NUMBER] );
	m_iMayaRightHandObjIndex	= TMonsterAIGroup::FindGroupMemberObjectIndex( m_iMayaHandGroupNumber[MAYA_HANDS_GROUP_NUMBER], 1 );
}

void CKanturuBattleOfMaya::SetState_END_MAYA3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END_MAYA3", m_iBattleOfMayaState );

	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_NONE );
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_END_MAYA3 );

	// 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );

	// 성공, 실패 여부를 클라이언트에 알린다.
	KANTURU_UTIL.NotifyKanturuSuccessValue( GetSuccessValue() );
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA3()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE_MAYA3", m_iBattleOfMayaState );

	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA3 );

	// 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );	
}

void CKanturuBattleOfMaya::SetState_END()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> END", m_iBattleOfMayaState );
	
	// 전투상태를 초기화한다.
	SetBattleSceneState( BATTLE_OF_MAYA_BATTLE_NONE );	
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_END );

	// 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );
	
	if( GetSuccessValue() == KANTURU_BATTLE_SUCCESS )
	{
		KANTURU_UTIL.SendMsgKanturuBattleUser( lMsg.Get( 3362 ) );	// "곧 나이트메어와의 전투가 시작될 것입니다."
	}
	else
	{
		// 실패했음을 클라이언트에 일린다.
		KANTURU_UTIL.NotifyKanturuSuccessValue( GetSuccessValue() );
		KANTURU_UTIL.SendMsgKanturuBattleUser( lMsg.Get( 3364 ) );	// "제한시간이 지나 실패하였습니다. 곧 칸투르 안전지역으로 강제소환 됩니다."	
	}

	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] Success Value[%d]", GetSuccessValue() );
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE()
{
	LogAddC( LOGC_LIGHTRED, "[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE", m_iBattleOfMayaState );

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	// 칸투르 보스전 진행상태를 남기기 위한 로그 수정
	LogAddTD("[ KANTURU ][ BattleOfMaya ] State(%d) -> ENDCYCLE", m_iBattleOfMayaState );
#endif
	
	SetBattleOfMayaState( BATTLE_OF_MAYA_STATE_ENDCYCLE );

	// 유저가 입장할 수 없다.
	SetEntrancePermit( FALSE );	

	// 실패한 경우 모든 전투 유저를 칸투르 2차맵 안전지대로 옮기고 데이터 초기화
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
		// 성공! 다음 상태로 이동
		SetSuccessValue( TRUE );
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

		SetNextState( m_iBattleOfMayaState );
		return;
	}

	// 몬스터와의 전투에서의 마야 행동
	m_KanturuMaya.KanturuMayaAct_IceStorm( m_iMayaIceStormUsingRate );

	KANTURU_UTIL.NotifyKanturuUserMonsterCount( g_KanturuMonsterMng.GetAliveMonsterCount(), g_KanturuBattleUserMng.GetUserCount() );
}

void CKanturuBattleOfMaya::ProcState_MAYA1()
{
#ifdef MEM_KUZIMO
	if(m_iMayaLeftHandObjIndex == -1) return;
#endif

	// 마야의 손이 죽었는지 확인한다.
	if( !gObj[m_iMayaLeftHandObjIndex].Live )
	{
		if( m_iMayaHandDieTimeCounter == 0 )
		{
			// 성공! 
			SetSuccessValue( TRUE );
			SetSceneSuccessValue( TRUE );
			LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

			// 남아있는 유저에 대한 정보와 경과 시간을 로그로 남긴다.
			g_KanturuBattleUserMng.LogBattleWinnerUserInfo( KANTURU_BATTLE_SUCCESS_LOG_MAYA1, 
									m_BattleTimeInfo[m_iSceneState].GetElapsedTime() );			

			// 마야 손이 죽었음을 알린다.
			KANTURU_UTIL.NotifyKanturuUserMonsterCount( 0, g_KanturuBattleUserMng.GetUserCount() );

			m_iMayaHandDieTimeCounter = GetTickCount();
		}
		
		if( GetTickCount() - m_iMayaHandDieTimeCounter >= 3000 )	// 마야 손 애니메이션 처리를 위해 3초의 딜레이를 준다.
		{
			// 다음 상태로 이동
			SetNextState( m_iBattleOfMayaState );
		}
		
		return;
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuUserMonsterCount( 1, g_KanturuBattleUserMng.GetUserCount() );
	}
	
	// AI 상태 변경
	ChangeAI( m_iMayaHandGroupNumber[MAYA_LEFT_HAND_GROUP_NUMBER] );


	// 마야 손과의 전투에서 마야 행동
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
		// 성공! 다음 상태로 이동
		SetSuccessValue( TRUE );		
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

		SetNextState( m_iBattleOfMayaState );
		return;
	}
	
	// 몬스터와의 전투에서의 마야 행동
	m_KanturuMaya.KanturuMayaAct_IceStorm( m_iMayaIceStormUsingRate );	

	KANTURU_UTIL.NotifyKanturuUserMonsterCount( g_KanturuMonsterMng.GetAliveMonsterCount(), g_KanturuBattleUserMng.GetUserCount() );
}

void CKanturuBattleOfMaya::ProcState_MAYA2()
{
#ifdef MEM_KUZIMO
	if(m_iMayaRightHandObjIndex == -1) return;
#endif

	// 마야의 오른손이 죽었는지 확인한다.
	if( !gObj[m_iMayaRightHandObjIndex].Live )
	{		
		if( m_iMayaHandDieTimeCounter == 0 )
		{
			// 성공! 
			SetSuccessValue( TRUE );
			SetSceneSuccessValue( TRUE );
			LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

			// 남아있는 유저에 대한 정보와 경과 시간을 로그로 남긴다.
			g_KanturuBattleUserMng.LogBattleWinnerUserInfo( KANTURU_BATTLE_SUCCESS_LOG_MAYA2, 
									m_BattleTimeInfo[m_iSceneState].GetElapsedTime() );	

			// 마야 손이 죽었음을 알린다.
			KANTURU_UTIL.NotifyKanturuUserMonsterCount( 0, g_KanturuBattleUserMng.GetUserCount() );
			m_iMayaHandDieTimeCounter = GetTickCount();
		}
				
		if( GetTickCount() - m_iMayaHandDieTimeCounter >= 3000 )	// 마야 손 애니메이션 처리를 위해 3초의 딜레이를 준다.
		{
			// 다음 상태로 이동
			SetNextState( m_iBattleOfMayaState );
		}		

		return;
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuUserMonsterCount( 1, g_KanturuBattleUserMng.GetUserCount() );
	}
	
	// AI 상태 변경
	ChangeAI( m_iMayaHandGroupNumber[MAYA_RIGHT_HAND_GROUP_NUMBER] );

	// 마야 손과의 전투에서 마야 행동
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
		// 성공! 다음 상태로 이동
		SetSuccessValue( TRUE );
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );
		
		SetNextState( m_iBattleOfMayaState );
		return;
	}

	// 몬스터와의 전투에서의 마야 행동
	m_KanturuMaya.KanturuMayaAct_IceStorm( m_iMayaIceStormUsingRate );

	KANTURU_UTIL.NotifyKanturuUserMonsterCount( g_KanturuMonsterMng.GetAliveMonsterCount(), g_KanturuBattleUserMng.GetUserCount() );
}

void CKanturuBattleOfMaya::ProcState_MAYA3()
{
	// 마야의 왼손, 오른손이 모두 죽었는지 확인한다.
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
			// 성공! 
			SetSuccessValue( TRUE );
			SetSceneSuccessValue( TRUE );
			LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success!! - DetailState:%d", m_iBattleOfMayaState );

			// 남아있는 유저에 대한 정보와 경과 시간을 로그로 남긴다.
			g_KanturuBattleUserMng.LogBattleWinnerUserInfo( KANTURU_BATTLE_SUCCESS_LOG_MAYA3, 
									m_BattleTimeInfo[m_iSceneState].GetElapsedTime() );	

			m_iMayaHandDieTimeCounter = GetTickCount();
		}
		
		if( GetTickCount() - m_iMayaHandDieTimeCounter >= 3000 )	// 마야 손 애니메이션 처리를 위해 3초의 딜레이를 준다.
		{
			// 다음 상태로 이동
			SetNextState( m_iBattleOfMayaState );
			return;
		}		
	}

	KANTURU_UTIL.NotifyKanturuUserMonsterCount( iAliveCount, g_KanturuBattleUserMng.GetUserCount() );
	
	// AI 상태 변경
	ChangeAI( m_iMayaHandGroupNumber[MAYA_HANDS_GROUP_NUMBER] );

	// 마야 손과의 전투에서 마야 행동
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
// 각 State당 종료조건을 체크한다.
BOOL CKanturuBattleOfMaya::CheckStateTime()
{
	// 각 State 당 시간을 체크한다.
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
	
	// 각 전투 당 시간을 체크한다.
	if( GetBattleSceneState() != BATTLE_OF_MAYA_BATTLE_NONE )
	{		
		if( m_BattleTimeInfo[m_iSceneState].GetCondition() != KANRUTU_STATE_END_COMDITION_TIMELIMIT )
		{
			return TRUE;
		}
		
		// 성공했다면 제한시간 체크는 그만한다.
		if( GetSceneSuccessValue() == TRUE )
		{
			return TRUE;
		}

		if( m_BattleTimeInfo[m_iSceneState].IsTimeOut() == TRUE )
		{
			// 제한 시간이 지났다.
			if( GetBattleOfMayaState() != BATTLE_OF_MAYA_STATE_END )
			{
				// 1차 보스 마야전 실패
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
	// 전투중 모든 전투유저가 사망하면 실패처리	
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
	// 마야 손
	TMonsterAIGroup::Init( iGroupNumber );
	TMonsterAIGroup::ChangeAIOrder( iGroupNumber, MAG_AI_ORDER_DEFAULT );
	m_iMayaHandAIAppliedTime	= GetTickCount();
}

// 상태가 바뀔 때마다 AI를 사용하는 몬스터의 등록된 AI 정보를 삭제한다.
void CKanturuBattleOfMaya::ResetAIMonster()
{	
	// 일단 죽인다.
#ifdef MEM_KUZIMO
	if(m_iMayaLeftHandObjIndex != -1)	gObj[m_iMayaLeftHandObjIndex].Life = 0;
	if(m_iMayaRightHandObjIndex != -1)	gObj[m_iMayaRightHandObjIndex].Life = 0;
#else
	gObj[m_iMayaLeftHandObjIndex].Life = 0;
	gObj[m_iMayaRightHandObjIndex].Life = 0;
#endif


	// 등록된 마야 손의 AI 삭제
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
	// AI 상태를 변경한다.
	if( GetTickCount() - m_iMayaHandAIAppliedTime >= m_iMayaHandAIChangeTime[m_iMayaHandCurrentAI] )
	{
		m_iMayaHandAIAppliedTime = GetTickCount();

		// 마지막 AI 상태는 계속 유지된다.
		if( m_iMayaHandCurrentAI != MAG_AI_ORDER_THIRD )
		{
			// 다음 시간 상태로 넘긴다.
			m_iMayaHandCurrentAI += 1;
			TMonsterAIGroup::ChangeAIOrder( iGroupNumber, m_iMayaHandCurrentAI );			
		}
	}
}



#endif