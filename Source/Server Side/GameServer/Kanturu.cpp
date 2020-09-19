// Kanturu.cpp: implementation of the CKanturu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "Kanturu.h"

#include "DirPath.h"
#include "Include\ReadScript.h"
#include "MapServerManager.h"
#include "protocol.h"
#include "user.h"
#include "gamemain.h"

#include "KanturuUtil.h"
#include "KanturuMonsterMng.h"

extern CDirPath			gDirPath;
static CKanturuUtil		KANTURU_UTIL;

CKanturu g_Kanturu;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturu::CKanturu()
{
	m_bKanturuEnable		= 0;
	m_iKanturuState			= KANTURU_STATE_NONE;

	m_StateInfoCount		= 0;
	
	m_bFileDataLoad			= FALSE;

	m_bEnableCheckMoonStone = FALSE;
	
#ifdef KANTURU_TIMEATTACK_EVENT_20060822	// 칸투르 보스전 카운터 변수 초기화
	m_iKanturuBattleCounter	= 0;
	m_iKanturuBattleDate	= 0;
#endif
}

CKanturu::~CKanturu()
{
	
}

// 칸투르의 모든 데이터를 초기화한다.
void CKanturu::ResetAllData()
{
	m_StateInfoCount = 0;
	
	for( int iCount = 0; iCount < MAX_KANTURU_STATE; iCount++ )
	{
		m_StateInfo[iCount].ResetTimeInfo();
	}
}

// Kanguru.dat파일을 로드한다.
BOOL CKanturu::LoadData( LPSTR lpszFileName )
{
	m_bFileDataLoad = FALSE;

	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		MsgBox( "[ KANTURU ] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{		
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )	
		{
			DWORD dwError = GetLastError();
			MsgBox( "[ KANTURU ] - Can't Open %s ", lpszFileName );			
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
				if( iType == KANTURU_DATAFILE_LOAD_STATE )
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
					if( m_StateInfoCount < 0 || m_StateInfoCount >= MAX_KANTURU_STATE )
					{
						MsgBox( "[ KANTURU ] - Exceed Max State Time (%d)", m_StateInfoCount );
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
				//	.> 칸투르 전투맵 입장시 문스톤펜던트 착용검사 여부
				else if( iType == KANTURU_DATAFILE_LOAD_CHECK_MOONSTONE)
				{
					// 1. >	데이터 초기화
					BOOL	bEnableCheckMoonStone = FALSE;

					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					bEnableCheckMoonStone = ( BOOL )TokenNumber;							
						
					// 3. >	데이터 유효성 검사

					
					// 4. > 데이터 세팅
					SetEnableCheckMoonStone( bEnableCheckMoonStone );
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
		
		LogAddC( 2, "[ KANTURU ] - %s file is Loaded", lpszFileName );

		m_bFileDataLoad = TRUE;	

		m_BattleStanby.LoadData( gDirPath.GetNewPath( "Kanturu.dat" ) );
		m_BattleOfMaya.LoadData( gDirPath.GetNewPath( "Kanturu.dat" ) );
		m_BattleOfNightmare.LoadData( gDirPath.GetNewPath( "Kanturu.dat" ) );
		m_TowerOfRefinement.LoadData( gDirPath.GetNewPath( "Kanturu.dat" ) );
	}
	catch(...) 
	{
		MsgBox( "[ KANTURU ] - Loading Exception Error (%s) File. ", lpszFileName );
	}

	return m_bFileDataLoad;	
}

// 칸투르 보스전 맵 속성을 읽어들여 저장한다. Terrain40_CLOSE.att Terratin40_OPEN.att
void CKanturu::LoadKanturuMapAttr( LPSTR lpszFileName, BYTE btLevel )
{
	if( !lpszFileName || btLevel > MAX_KANTURU_TOWEROFREFINEMENT_MAP )
		return;

	switch( btLevel )
	{
	case KANTURU_MAP_TOWEROFREFINEMENT_CLOSE:
		m_KanturuMap[btLevel].LoadMapAttr( lpszFileName, MAP_INDEX_KANTURU_BOSS );
		break;

	case KANTURU_MAP_TOWEROFREFINEMENT_OPEN:
		m_KanturuMap[btLevel].LoadMapAttr( lpszFileName, MAP_INDEX_KANTURU_BOSS );
		break;

	default:
		break;
	}
}

// 정제의 탑에 입장 가능/ 불가능에 따른  맵 속성 변경
void CKanturu::SetKanturuMapAttr( BYTE btLevel )
{
	MapClass& KanturuMap = m_KanturuMap[btLevel];
	
	CopyMemory( MapC[MAP_INDEX_KANTURU_BOSS].m_attrbuf, KanturuMap.m_attrbuf, ( MAX_TERRAIN_SIZE * MAX_TERRAIN_SIZE ) );
	LogAddC( LOGC_RED, "[ KANTURU ][ Map Attr Change ] Map(%d) State(%d) DetailState(%d)",
						btLevel, GetKanturuState(), GetKanturuDetailState() );
}

//--------------------------------------------------------------------------------------
void CKanturu::Run()
{	
	// 1. 칸투르 보스전을 진행하는가?
	if( !IsKanturuEnable() )
	{
		return;
	}

	// 2. 시간 체크
	CheckStateTime();

	// 3. 칸투르 맵에 있는 유저에 대한 처리
	CheckUserOnKanturuBossMap();

	// 4. 칸투르 상태 처리
	switch( m_iKanturuState )
	{
	case KANTURU_STATE_NONE:
		ProcState_NONE();
		break;
		
	case KANTURU_STATE_BATTLE_STANDBY:
		ProcState_BATTLE_STANDBY();
		break;

	case KANTURU_STATE_BATTLE_OF_MAYA:
		ProcState_BATTLE_OF_MAYA();
		break;
		
	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		ProcState_BATTLE_OF_NIGHTMARE();
		break;

	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		ProcState_TOWER_OF_REFINEMENT();
		break;

	case KANTURU_STATE_END:
		ProcState_END();
		break;

	default:
		break;
	}

	// KANTURU MONSTER INFO LOG
	//UserMonsterCountCheck();
}

//--------------------------------------------------------------------------------------
void CKanturu::SetState( int iKanturuState )
{	
	// 1. 종료조건이 시간이면 상태가 바뀌었을 때 시간을 적용한다.
	if( m_StateInfo[iKanturuState].GetCondition() > KANRUTU_STATE_END_COMDITION_NONE )
	{
		m_StateInfo[iKanturuState].SetAppliedTime();
	}

	// 2. 데이터 초기화
	g_KanturuMonsterMng.ResetRegenMonsterObjData();
	
	// 3. 정제의 탑은 기본으로 항상 닫아둔다. ( 정제의 탑 상태안에서 열림속성을 준다. )
	SetKanturuMapAttr( KANTURU_MAP_TOWEROFREFINEMENT_CLOSE );

	// 4. 상태를 변경한다.
	switch( iKanturuState )
	{
	case KANTURU_STATE_NONE:
		SetState_NONE();
		break;
		
	case KANTURU_STATE_BATTLE_STANDBY:
		SetState_BATTLE_STANDBY();
		break;

	case KANTURU_STATE_BATTLE_OF_MAYA:
		SetState_BATTLE_OF_MAYA();
		break;
		
	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		SetState_BATTLE_OF_NIGHTMARE();
		break;

	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		SetState_TOWER_OF_REFINEMENT();
		break;

	case KANTURU_STATE_END:
		SetState_END();
		break;

	default:
		break;
	}	
}

// 다음 상태로 상태를 변환한다.
void CKanturu::SetNextState( int iCurrentState )
{
	int iNextState = iCurrentState + 1;
	if( iNextState >= MAX_KANTURU_STATE )
	{
		iNextState = KANTURU_STATE_NONE;
	}
	
	SetState( iNextState );
}


//--------------------------------------------------------------------------------------
// 상태를 변환할 때 해 주어야 할 일들을 명시한다. 
void CKanturu::ChangeState( int iState, int DetailState )
{
	switch( iState )
	{
	case KANTURU_STATE_NONE:
		break;
	case KANTURU_STATE_BATTLE_STANDBY:
		break;
	case KANTURU_STATE_BATTLE_OF_MAYA:
		break;
	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		break;
	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		break;
	case KANTURU_STATE_END:
		break;
	}
}

//--------------------------------------------------------------------------------------
void CKanturu::SetState_NONE()
{
	LogAddC( LOGC_PURPLE, "[ KANTURU ] State(%d) -> NONE", m_iKanturuState );
	
	SetKanturuState( KANTURU_STATE_NONE );	
}

void CKanturu::SetState_BATTLE_STANDBY()
{
	LogAddC( LOGC_PURPLE, "[ KANTURU ] State(%d) -> STANDBY", m_iKanturuState );

	// 칸투르 전투 유저 데이터를 모두 초기화.
	g_KanturuBattleUserMng.ResetAllData();

	SetKanturuState( KANTURU_STATE_BATTLE_STANDBY );	
	m_BattleStanby.SetState( KANTURU_BATTLE_STANBY_STATE_START );

#ifdef KANTURU_TIMEATTACK_EVENT_20060822
	SetKanturuTimeAttackEventInfo();
#endif
}

void CKanturu::SetState_BATTLE_OF_MAYA()
{
	LogAddC( LOGC_PURPLE, "[ KANTURU ] State(%d) -> BATTLE_OF_MAYA", m_iKanturuState );

	SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
	m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_STANBY1 );
}

void CKanturu::SetState_BATTLE_OF_NIGHTMARE()
{
	LogAddC( LOGC_PURPLE, "[ KANTURU ] State(%d) -> BATTLE_OF_NIGHTMARE", m_iKanturuState );

	SetKanturuState( KANTURU_STATE_BATTLE_OF_NIGHTMARE );
	m_BattleOfNightmare.SetState( BATTLE_OF_NIGHTMARE_STATE_IDLE );
}

void CKanturu::SetState_TOWER_OF_REFINEMENT()
{
	LogAddC( LOGC_PURPLE, "[ KANTURU ] State(%d) -> TOWER_OF_REFINEMENT", m_iKanturuState );

	SetKanturuState( KANTURU_STATE_TOWER_OF_REFINEMENT );
	m_TowerOfRefinement.SetState( TOWER_OF_REFINEMENT_STATE_REVITALIZATION );

	// 정제의 탑 상태일때는 열어준다.
	SetKanturuMapAttr( KANTURU_MAP_TOWEROFREFINEMENT_OPEN );
}

void CKanturu::SetState_END()
{
	LogAddC( LOGC_PURPLE, "[ KANTURU ] State(%d) -> END", m_iKanturuState );
	
	SetKanturuState( KANTURU_STATE_END );

	// 칸투르 전투 유저 데이터를 모두 초기화.
	g_KanturuBattleUserMng.ResetAllData();
}

//--------------------------------------------------------------------------------------
void CKanturu::ProcState_NONE()
{
	// 칸투르 보스전은 특정한 시작 시간 없이 바로 시작한다.
	SetState( KANTURU_STATE_BATTLE_STANDBY );
}

void CKanturu::ProcState_BATTLE_STANDBY()
{
	// 대기상태가 종료되었다면 상태를 변경한다.
	if( m_BattleStanby.GetBattleStanbyState() == KANTURU_BATTLE_STANBY_STATE_END )
	{
		m_BattleStanby.SetState_NONE();
		
		SetState( KANTURU_STATE_BATTLE_OF_MAYA );
		return;
	}

	m_BattleStanby.Run();
}

void CKanturu::ProcState_BATTLE_OF_MAYA()
{
	// 1차 보스 마야 전투가 종료되었다면 상태를 변경한다.
	if( m_BattleOfMaya.GetBattleOfMayaState() == BATTLE_OF_MAYA_STATE_ENDCYCLE )
	{
		m_BattleOfMaya.SetState_NONE();
		
		if( m_BattleOfMaya.GetSuccessValue() == TRUE )
		{
			SetState( KANTURU_STATE_BATTLE_OF_NIGHTMARE );
		}
		else
		{			
			SetState( KANTURU_STATE_END );
		}
		return;
	}

	m_BattleOfMaya.Run();
	
}

void CKanturu::ProcState_BATTLE_OF_NIGHTMARE()
{
	// 2차 보스 나이트메어 전투가 종료되었다면 상태를 변경한다.
	if( m_BattleOfNightmare.GetBattleOfNightmareState() == BATTLE_OF_NIGHTMARE_STATE_ENDCYCLE )
	{
		m_BattleOfNightmare.SetState( BATTLE_OF_NIGHTMARE_STATE_NONE );
		
		if( m_BattleOfNightmare.GetSuccessValue() == TRUE )
		{
			SetState( KANTURU_STATE_TOWER_OF_REFINEMENT );
		}
		else
		{
			SetState( KANTURU_STATE_END );
		}

		return;
	}
	
	m_BattleOfNightmare.Run();	
}

void CKanturu::ProcState_TOWER_OF_REFINEMENT()
{
	if( m_TowerOfRefinement.GetTowerOfRefinementState() == TOWER_OF_REFINEMENT_STATE_END )
	{
		m_TowerOfRefinement.SetState( TOWER_OF_REFINEMENT_STATE_NONE );
		SetState( KANTURU_STATE_END );
		
		return;
	}
	
	m_TowerOfRefinement.Run();
}

void CKanturu::ProcState_END()
{
	// 칸투르 종료	
	SetState( KANTURU_STATE_NONE );
}

//--------------------------------------------------------------------------------------
int	CKanturu::GetKanturuDetailState()
{
	// 칸투르 보스전 상태에 대한 세부 상태를 검사한다.
	int iCurrentDetailState = 0;
	switch( GetKanturuState() )
	{
	case KANTURU_STATE_BATTLE_STANDBY:
		iCurrentDetailState = m_BattleStanby.GetBattleStanbyState();
		break;
	case KANTURU_STATE_BATTLE_OF_MAYA:
		iCurrentDetailState = m_BattleOfMaya.GetBattleOfMayaState();
		break;
	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		iCurrentDetailState = m_BattleOfNightmare.GetBattleOfNightmareState();
		break;
	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		iCurrentDetailState = m_TowerOfRefinement.GetTowerOfRefinementState();
		break;
	}

	return iCurrentDetailState;
}

//--------------------------------------------------------------------------------------
void CKanturu::CheckStateTime()
{
	// 각 State 당 종료조건 - 제한시간을 체크한다.
	if( GetKanturuState() != KANTURU_STATE_NONE )
	{
		int iCurrentState = GetKanturuState();
		if( m_StateInfo[iCurrentState].GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
		{
			if( m_StateInfo[iCurrentState].IsTimeOut() == TRUE )
			{
				SetNextState( iCurrentState );
			}
		}
	}
}

int CKanturu::GetRemainTime()
{
	// 각 세부상태 종료조건이 제한시간인 경우 남은 시간을 돌려준다.
	int iRemainTime = 0;
	switch( GetKanturuState() )
	{
	case KANTURU_STATE_BATTLE_STANDBY:
		iRemainTime = m_BattleStanby.GetRemainTime();
		break;

	case KANTURU_STATE_BATTLE_OF_MAYA:
		iRemainTime = m_BattleOfMaya.GetRemainTime();
		break;

	case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		iRemainTime = m_BattleOfNightmare.GetRemainTime();
		break;

	case KANTURU_STATE_TOWER_OF_REFINEMENT:
		iRemainTime = m_TowerOfRefinement.GetRemainTime();
		break;
	}

	return iRemainTime;
}

// 칸투르 보스전 몬스터가 죽었을 때의 처리
void CKanturu::KanturuMonsterDieProc( int iMonIndex, int iKillerIndex )
{
	// 일반 몬스터가 죽었을 때의 처리
	g_KanturuMonsterMng.MonsterDie( iMonIndex );
}

//--------------------------------------------------------------------------------------
// 비정상적인 방법으로 맵에 있는 유저는 쫒아낸다.
void CKanturu::CheckUserOnKanturuBossMap()			
{
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{		
		if( ( gObj[iCount].Connected == CSS_GAMEPLAYING ) && ( gObj[iCount].Type == OBJTYPE_CHARACTER ) 
			&& gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS 
			)			
		{			
#ifdef ADD_NEW_NPC_KANTURU_20060612
			if( gObj[iCount].m_bKanturuEntranceByNPC == FALSE )
#endif
			{
				// 운영자는 괜찮다.
				if( ( gObj[iCount].Authority & AUTHORITY_ADMIN ) != AUTHORITY_ADMIN )
				{
					// 보스맵으로 완전히 이동한 후에 옮긴다.
					if( gObj[iCount].RegenOk == 0  && gObj[iCount].m_State == OBJST_PLAYING 
						&& gObj[iCount].Live == 1 )
					{
						// 로랜시아로 쫓아버린다. ( 접속을 끊는걸로 변경해도 괜찮을 듯. )
						gObjMoveGate( iCount, 17 );
						LogAddC( LOGC_RED, "[ KANTURU ][ Invalid User ] Invalid Kanturu Boss Map User[%s][%s]",
							gObj[iCount].AccountID, gObj[iCount].Name );
					}
				}
			}
		}
	}
}

int CKanturu::CheckEnterKanturu( int iUserIndex )
{
	if( !gObjIsConnected( iUserIndex ) )
		return -1;	

	// 맵 번호가 다르면 입장할 수 없다.
	if( gObj[iUserIndex].MapNumber != MAP_INDEX_KANTURU2 )
	{
		// 입장할 수 없음
		LogAddTD( "[ KANTURU ][ Entrance Fail ] Invalid Map Number(%d) [%s][%s] State(%d)",
					gObj[iUserIndex].MapNumber,
					gObj[iUserIndex].AccountID,
					gObj[iUserIndex].Name,
					GetKanturuState() );
		return 4;	// 알 수 없는 원인으로 입장할 수 없음
	}

	// 마야전 대기시간 중에는 입장할 수 있다.
	if( GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA && m_BattleOfMaya.GetEntrancePermit() == TRUE )
	{
		if( g_KanturuBattleUserMng.IsOverMaxUser() == TRUE )
		{
			LogAddTD( "[ KANTURU ][ Entrance Fail ] Over Max User [%s][%s] State(%d)-(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );				
			return 1;		// 입장 가능한 인원 수 초과
		}
		// 문스톤펜던트 착용 여부
		if( CheckEqipmentMoonStone( iUserIndex ) == FALSE )
		{
			LogAddTD( "[ KANTURU ][ Entrance Fail ] Moon Stone is not exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 2;		// 입장에 필요한 아이템이 없음
		}
#ifdef ADD_KANTURU_CONDITION_20060725
		// 1. 날개,디노란트,펜릴 아이템 착용 안함
		if( ( gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type < MAKE_ITEMNUM(12,0) ||			// 날개
			gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type > MAKE_ITEMNUM(12,6) )			// 날개
#ifdef DARKLORD_WORK
			&& gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(13,30)		// 망토
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 신규캐릭터 1차날개(재앙의 날개) 칸투루입장 조건
			&& gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12,41)		// 신규캐릭터 1차날개(재앙의 날개)
#endif			
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// 절망의 날개, 차원의 날개 칸투루 입장 조건
			&& gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12,42)	// 절망의 날개
			&& gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12,43)	// 차원의 날개
#endif			
			&& gObj[iUserIndex].pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3)		// 디노란트
			&& gObj[iUserIndex].pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,37)		// 펜릴
#ifdef MODIFY_KANTURU_ENTERANCE_DARKHORSE_BUGFIX_20080627
			&& gObj[iUserIndex].pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,4)		// 다크호스
#endif // MODIFY_KANTURU_ENTERANCE_DARKHORSE_BUGFIX_20080627
#ifdef ADD_THIRD_WING_20070525	// 칸투르 보스맵 입장조건에 3차 날개 추가
			&& ( gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type < MAKE_ITEMNUM( 12, 36 ) ||			// 3차 ㅡ날개
			gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type > MAKE_ITEMNUM( 12, 40 ) )
#endif			
		  )
		{
			LogAddTD( "[ KANTURU ][ Entrance Fail ] Wing Item is not exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 7;
		}

		// 2. 유니리아 착용
		if( gObj[iUserIndex].pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) )		// 유니리아
		{
			LogAddTD( "[ KANTURU ][ Entrance Fail ] Uniria Item is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 5;
		}
		// 3. 변신반지 착용
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) ||	// 변신반지
			gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) )	// 변신반지
		{
			LogAddTD( "[ KANTURU ][ Entrance Fail ] Trasportation Ring is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 6;
		}
#endif // ADD_KANTURU_CONDITION_20060725
		
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// 칸투르 보스전 입장 금지
		// 3-1. 엘리트해골전사변신반지 착용
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) ||	
			gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 ) )	
		{
			LogAddTD( "[ Kanturu ][ Entrance Fail ] EliteSkelletoneChangeRing is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 6;
		}
#endif
		
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 ) ||
			gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			LogAddTD( "[ Kanturu ][ Entrance Fail ] Jack'O Lantern Polymorph Ring is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 6;
		}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017	
		
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 ) ||
			gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			LogAddTD( "[ Kanturu ][ Entrance Fail ] Santa Polymorph Ring is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 6;
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

//------------->
//20081013 해외 크리스마스 이벤트 - 변신반지(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 ) ||
			gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			LogAddTD( "[ Kanturu ][ Entrance Fail ] X-MASEvent Polymorph Ring is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 6;
		}
#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------
#ifdef ADD_PREMIUMITEM_PANDA
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 ) ||
			gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
		{
			LogAddTD( "[ Kanturu ][ Entrance Fail ] Panda Polymorph Ring is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 6;
		}
#endif // ADD_PREMIUMITEM_PANDA
		
		// 입장 가능
		LogAddTD( "[ KANTURU ][ Entrance Success ] [%s][%s] State(%d)-(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
		return 0;
	}
	else if( GetKanturuState() == KANTURU_STATE_TOWER_OF_REFINEMENT && m_TowerOfRefinement.GetEntrancePermit() == TRUE )
	{
		// 정제의 탑 입장시간에는 입장할 수 있다.
		LogAddTD( "[ KANTURU ][ Entrance Success ] [%s][%s] State(%d)-(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_TowerOfRefinement.GetTowerOfRefinementState() );
		return 0;		
	}
	else
	{
		// 입장할 수 없음
		LogAddTD( "[ KANTURU ][ Entrance Fail ] [%s][%s] State(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState() );
		return 3;	// 입장가능한 상태가 아님
	}

	// 입장할 수 없음
	LogAddTD( "[ KANTURU ][ Entrance Error ] [%s][%s] State(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState() );
	return 4;	// 알 수 없는 원인으로 입장할 수 없음
}

BOOL CKanturu::CheckCanEnterKanturuBattle()
{
	// 마야전 대기시간 중에는 입장할 수 있다.
	if( GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA 
		&& m_BattleOfMaya.GetEntrancePermit() == TRUE )	
	{
		return TRUE;
	}

	// 정제의 탑이 열려있고 입장 가능한 상태이면 입장
	if( GetKanturuState() == KANTURU_STATE_TOWER_OF_REFINEMENT 
		&& m_TowerOfRefinement.GetEntrancePermit() == TRUE 
		&& m_TowerOfRefinement.IsUseTowerOfRefinement() == TRUE )
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL CKanturu::CheckEqipmentMoonStone( int iUserIndex )
{
	// 문스톤펜던트 체크 여부
	if( GetEnableCheckMoonStone() == FALSE )
		return TRUE;

	if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].IsItem() || 
		gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].IsItem() )
	{
		// 오른쪽
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 38 ) )
		{
			// 내구도가 0이 아닐 때만 적용된다.
			if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Durability != 0 )
				return TRUE;
		}
		// 왼쪽
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 38 ) )
		{
			// 내구도가 0이 아닐 때만 적용된다.
			if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Durability != 0 )
				return TRUE;
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------
#ifdef KANTURU_GM_COMMAND_20060709
void CKanturu::OperateGmCommand( int iUserIndex, int iCommand )
{
#if ( TESTSERVER != 1 )
	// 테스트중에만 사용 가능하다.
	return;	
#endif

	if( !gObjIsConnected( iUserIndex ) )
		return;

	LogAddTD( "[ KANTURU ][ Use GM Command ] [%s][%s] Command : %d", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, iCommand );
	
	switch( iCommand )
	{
	case KANTURU_GM_COMMAND_STANBY:					// "/칸투르대기상태"		
		SetKanturuState( KANTURU_STATE_BATTLE_STANDBY );
		m_BattleStanby.SetState( KANTURU_BATTLE_STANBY_STATE_START );		
		break;
	case KANTURU_GM_COMMAND_BATTLEOFMAYA:			// "/칸투르마야전"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_STANBY1 );
		break;
	case KANTURU_GM_COMMAND_MAYA_MONSTER1:			// "/마야몬스터1"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_START1 );
		break;
	case KANTURU_GM_COMMAND_MAYA1:					// "/마야1"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_MAYA1 );
		break;
	case KANTURU_GM_COMMAND_MAYA_MONSTER2:			// "/몬스터2"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_START2 );
		break;
	case KANTURU_GM_COMMAND_MAYA2:					// "/마야2"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_MAYA2 );
		break;
	case KANTURU_GM_COMMAND_MAYA_MONSTER3:			// "/마야몬스터3"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_START3 );
		break;
	case KANTURU_GM_COMMAND_MAYA3:					// "/마야3"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_MAYA3 );
		break;
	case KANTURU_GM_COMMAND_MAYA3_SUCCESS:			// "/마야3성공"	
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetSuccessValue( TRUE );
		m_BattleOfMaya.SetSceneSuccessValue( TRUE );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA3 );
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success - Macro[%s][%s]:/마야3성공",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );
		break;
	case KANTURU_GM_COMMAND_MAYA_SUCCESS:			// "/마야성공"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetSuccessValue( TRUE );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_END_MAYA3 );
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success - Macro[%s][%s]:/마야성공",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );
		break;
	case KANTURU_GM_COMMAND_MAYA_FAIL:				// "/마야실패"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetSuccessValue( FALSE );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_END );	
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Fail - Macro[%s][%s]:/마야실패",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );		
		break;
	case KANTURU_GM_COMMAND_NIGHTMARE:				// "/칸투르나이트메어전"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_NIGHTMARE );
		m_BattleOfNightmare.SetState( BATTLE_OF_NIGHTMARE_STATE_IDLE );
		break;
	case KANTURU_GM_COMMAND_NIGHTMARE_SUCCESS:		// "/나이트메어성공"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_NIGHTMARE );
		m_BattleOfNightmare.SetSuccessValue( TRUE );
		m_BattleOfNightmare.SetState( BATTLE_OF_NIGHTMARE_STATE_END );
		LogAddTD( "[ KANTURU ][ BattleOfNightmare ] Success - Macro[%s][%s]:/나이트메어성공",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );			
		break;
	case KANTURU_GM_COMMAND_NIGHTMARE_FAIL:			// "/나이트메어실패"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_NIGHTMARE );
		m_BattleOfNightmare.SetSuccessValue( FALSE );
		m_BattleOfNightmare.SetState( BATTLE_OF_NIGHTMARE_STATE_END );
		LogAddTD( "[ KANTURU ][ BattleOfNightmare ] Fail - Macro[%s][%s]:/나이트메어실패",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );			
		break;
	case KANTURU_GM_COMMAND_TOWEROFREFINEMENT:		// "/칸투르정제의탑"
		SetKanturuState( KANTURU_STATE_TOWER_OF_REFINEMENT );
		m_TowerOfRefinement.SetState( TOWER_OF_REFINEMENT_STATE_REVITALIZATION );
		break;
	case KANTURU_GM_COMMAND_TOWER_OPEN:				// "/정제의탑열기"
		m_TowerOfRefinement.SetEntrancePermit( TRUE );
		break;
	case KANTURU_GM_COMMAND_TOWER_CLOSE:			// "/정제의탑닫기"
		m_TowerOfRefinement.SetEntrancePermit( FALSE );
		break;
	case KANTURU_GM_COMMAND_END:					// "/칸투르종료"
		SetState( KANTURU_STATE_END );		
		break;
	case KANTURU_GM_COMMAND_NEXT_STATE:				// "칸투르다음상태"	
		SetNextState( GetKanturuState() );		
		break;
	case KANTURU_GM_COMMAND_NEXT_DETAIL_STATE:		// "칸투르다음세부상태"
		{
			int iCurrentState = GetKanturuState();
			switch( iCurrentState )
			{
			case KANTURU_STATE_BATTLE_STANDBY:
				m_BattleStanby.SetNextState( m_BattleStanby.GetBattleStanbyState() );
				break;
			case KANTURU_STATE_BATTLE_OF_MAYA:
				m_BattleOfMaya.SetNextState( m_BattleOfMaya.GetBattleOfMayaState() );
				break;
			case KANTURU_STATE_BATTLE_OF_NIGHTMARE:	
				m_BattleOfNightmare.SetNextState( m_BattleOfNightmare.GetBattleOfNightmareState() );
				break;
			case KANTURU_STATE_TOWER_OF_REFINEMENT:	
				m_TowerOfRefinement.SetNextState( m_TowerOfRefinement.GetTowerOfRefinementState() );
				break;
			}
		}
		break;
	case KANTURU_GM_COMMAND_CURRENT_STATE:			// "칸투르현재상태"
		{
			KANTURU_UTIL.SendMsgToUser( iUserIndex, "KANTURU CURRENT STATE %d:%d"
				, GetKanturuState(), GetKanturuDetailState() );
		}
		break;
	}	
}
#endif	//#ifdef KANTURU_GM_COMMAND_20060709

#include "LogToFile.h"
CLogToFile	KANTURU_TEST_LOG( "KANTURU_TEST_LOG", ".\\KANTURU_TEST_LOG");		// 로그파일 객체
void CKanturu::UserMonsterCountCheck()
{	
#if ( TESTSERVER != 1 )
	return;
#endif

	// 전투중일 때만 검사
	if( GetKanturuState() != KANTURU_STATE_BATTLE_OF_MAYA && GetKanturuState() != KANTURU_STATE_BATTLE_OF_NIGHTMARE )
	{
		return;
	}

	// 1. 현재 유저 수 검사 - Manager		
	KANTURU_TEST_LOG.Output( "[ KANTURU ][ Debug ] 1. UserMng Count:%d", g_KanturuBattleUserMng.GetUserCount() );

	// 2. 현재 유저 수 검사 - 전체
	int iUserCount_Live = 0;
	int iUserCount_Die = 0;
	for( int iAllUserCount = ALLOC_USEROBJECTSTART; iAllUserCount < MAX_OBJECT; iAllUserCount++ )
	{
		if( gObj[iAllUserCount].MapNumber == MAP_INDEX_KANTURU_BOSS 
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
	KANTURU_TEST_LOG.Output( "[ KANTURU ][ Debug ] 2. All User Check:%d/(Die:%d)", iUserCount_Live, iUserCount_Die );

	// 3. 현재 몬스터 수 검사 - Manager
	KANTURU_TEST_LOG.Output( "[ KANTURU ][ Debug ] 3. MonsterMng Count:%d", g_KanturuMonsterMng.GetAliveMonsterCount() );

	// 4. 현재 몬스터 수 검사 - 전체
	int iMonsterCount = 0;
	for( int iAllMonsterCount = 0; iAllMonsterCount < ALLOC_USEROBJECTSTART; iAllMonsterCount++ )
	{
		if( gObj[iAllMonsterCount].MapNumber == MAP_INDEX_KANTURU_BOSS 
			&& gObj[iAllMonsterCount].Connected == 3
			&& gObj[iAllMonsterCount].Type	== OBJTYPE_MONSTER )
		{
			
			if( gObj[iAllMonsterCount].Class != KANTURU_MONSTER_CANNONTRAP 
				&& gObj[iAllMonsterCount].Class != KANTURU_MONSTER_LASERTRAP		// 트랩은 제외
				&& gObj[iAllMonsterCount].Class != KANTURU_MONSTER_MAYA				// 마야 제외
				)
			{
				++iMonsterCount;
				KANTURU_TEST_LOG.Output( "[ KANTURU ][ Debug ] 4. Monster Info %s(%d/ObjIndex:%d) [%d]%d-%d HP:%0.1f IsAlive:%d Connected:%d State:%d ActState:Attack(%d)/Move(%d) DieRegen:%d", 
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
	//KANTURU_TEST_LOG.Output( "[ KANTURU ][ Debug ] 5. All Monster Check:%d KanturuEventCount:%d", iMonsterCount, m_dwKanturuEventCount );

}

#ifdef KANTURU_TIMEATTACK_EVENT_20060822
void CKanturu::SetKanturuTimeAttackEventInfo()
{
	struct tm	*today;
	time_t		ltime;
	
	time( &ltime );
	today = localtime( &ltime );
	
	today->tm_year += 1900;
	int iYear = today->tm_year * 10000;
	int iMonth = ( today->tm_mon + 1 ) * 100;
	int iDay = today->tm_mday;

	int iDateInfo = iYear + iMonth + iDay;
	
	// 날짜가 변경되었다.
	if( m_iKanturuBattleDate < iDateInfo )
	{
		m_iKanturuBattleDate		= iDateInfo;	// 날짜 정보 변경	
		m_iKanturuBattleCounter		= 0;			// 초기화
	}

	// 카운터 증가
	++m_iKanturuBattleCounter;	

	LogAddTD( "[ KANTURU ][ TimeAttackEvent ] Date:%d, Counter:%d", m_iKanturuBattleDate, m_iKanturuBattleCounter );
}
#endif	// KANTURU_TIMEATTACK_EVENT_20060822



#endif