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
	
#ifdef KANTURU_TIMEATTACK_EVENT_20060822	// ĭ���� ������ ī���� ���� �ʱ�ȭ
	m_iKanturuBattleCounter	= 0;
	m_iKanturuBattleDate	= 0;
#endif
}

CKanturu::~CKanturu()
{
	
}

// ĭ������ ��� �����͸� �ʱ�ȭ�Ѵ�.
void CKanturu::ResetAllData()
{
	m_StateInfoCount = 0;
	
	for( int iCount = 0; iCount < MAX_KANTURU_STATE; iCount++ )
	{
		m_StateInfo[iCount].ResetTimeInfo();
	}
}

// Kanguru.dat������ �ε��Ѵ�.
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
				// .> ĭ���� �⺻ ���� �ð� ������
				if( iType == KANTURU_DATAFILE_LOAD_STATE )
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
					if( m_StateInfoCount < 0 || m_StateInfoCount >= MAX_KANTURU_STATE )
					{
						MsgBox( "[ KANTURU ] - Exceed Max State Time (%d)", m_StateInfoCount );
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
				//	.> ĭ���� ������ ����� ���������Ʈ ����˻� ����
				else if( iType == KANTURU_DATAFILE_LOAD_CHECK_MOONSTONE)
				{
					// 1. >	������ �ʱ�ȭ
					BOOL	bEnableCheckMoonStone = FALSE;

					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					bEnableCheckMoonStone = ( BOOL )TokenNumber;							
						
					// 3. >	������ ��ȿ�� �˻�

					
					// 4. > ������ ����
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

// ĭ���� ������ �� �Ӽ��� �о�鿩 �����Ѵ�. Terrain40_CLOSE.att Terratin40_OPEN.att
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

// ������ ž�� ���� ����/ �Ұ��ɿ� ����  �� �Ӽ� ����
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
	// 1. ĭ���� �������� �����ϴ°�?
	if( !IsKanturuEnable() )
	{
		return;
	}

	// 2. �ð� üũ
	CheckStateTime();

	// 3. ĭ���� �ʿ� �ִ� ������ ���� ó��
	CheckUserOnKanturuBossMap();

	// 4. ĭ���� ���� ó��
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
	// 1. ���������� �ð��̸� ���°� �ٲ���� �� �ð��� �����Ѵ�.
	if( m_StateInfo[iKanturuState].GetCondition() > KANRUTU_STATE_END_COMDITION_NONE )
	{
		m_StateInfo[iKanturuState].SetAppliedTime();
	}

	// 2. ������ �ʱ�ȭ
	g_KanturuMonsterMng.ResetRegenMonsterObjData();
	
	// 3. ������ ž�� �⺻���� �׻� �ݾƵд�. ( ������ ž ���¾ȿ��� �����Ӽ��� �ش�. )
	SetKanturuMapAttr( KANTURU_MAP_TOWEROFREFINEMENT_CLOSE );

	// 4. ���¸� �����Ѵ�.
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

// ���� ���·� ���¸� ��ȯ�Ѵ�.
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
// ���¸� ��ȯ�� �� �� �־�� �� �ϵ��� ����Ѵ�. 
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

	// ĭ���� ���� ���� �����͸� ��� �ʱ�ȭ.
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

	// ������ ž �����϶��� �����ش�.
	SetKanturuMapAttr( KANTURU_MAP_TOWEROFREFINEMENT_OPEN );
}

void CKanturu::SetState_END()
{
	LogAddC( LOGC_PURPLE, "[ KANTURU ] State(%d) -> END", m_iKanturuState );
	
	SetKanturuState( KANTURU_STATE_END );

	// ĭ���� ���� ���� �����͸� ��� �ʱ�ȭ.
	g_KanturuBattleUserMng.ResetAllData();
}

//--------------------------------------------------------------------------------------
void CKanturu::ProcState_NONE()
{
	// ĭ���� �������� Ư���� ���� �ð� ���� �ٷ� �����Ѵ�.
	SetState( KANTURU_STATE_BATTLE_STANDBY );
}

void CKanturu::ProcState_BATTLE_STANDBY()
{
	// �����°� ����Ǿ��ٸ� ���¸� �����Ѵ�.
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
	// 1�� ���� ���� ������ ����Ǿ��ٸ� ���¸� �����Ѵ�.
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
	// 2�� ���� ����Ʈ�޾� ������ ����Ǿ��ٸ� ���¸� �����Ѵ�.
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
	// ĭ���� ����	
	SetState( KANTURU_STATE_NONE );
}

//--------------------------------------------------------------------------------------
int	CKanturu::GetKanturuDetailState()
{
	// ĭ���� ������ ���¿� ���� ���� ���¸� �˻��Ѵ�.
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
	// �� State �� �������� - ���ѽð��� üũ�Ѵ�.
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
	// �� ���λ��� ���������� ���ѽð��� ��� ���� �ð��� �����ش�.
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

// ĭ���� ������ ���Ͱ� �׾��� ���� ó��
void CKanturu::KanturuMonsterDieProc( int iMonIndex, int iKillerIndex )
{
	// �Ϲ� ���Ͱ� �׾��� ���� ó��
	g_KanturuMonsterMng.MonsterDie( iMonIndex );
}

//--------------------------------------------------------------------------------------
// ���������� ������� �ʿ� �ִ� ������ �i�Ƴ���.
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
				// ��ڴ� ������.
				if( ( gObj[iCount].Authority & AUTHORITY_ADMIN ) != AUTHORITY_ADMIN )
				{
					// ���������� ������ �̵��� �Ŀ� �ű��.
					if( gObj[iCount].RegenOk == 0  && gObj[iCount].m_State == OBJST_PLAYING 
						&& gObj[iCount].Live == 1 )
					{
						// �η��þƷ� �Ѿƹ�����. ( ������ ���°ɷ� �����ص� ������ ��. )
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

	// �� ��ȣ�� �ٸ��� ������ �� ����.
	if( gObj[iUserIndex].MapNumber != MAP_INDEX_KANTURU2 )
	{
		// ������ �� ����
		LogAddTD( "[ KANTURU ][ Entrance Fail ] Invalid Map Number(%d) [%s][%s] State(%d)",
					gObj[iUserIndex].MapNumber,
					gObj[iUserIndex].AccountID,
					gObj[iUserIndex].Name,
					GetKanturuState() );
		return 4;	// �� �� ���� �������� ������ �� ����
	}

	// ������ ���ð� �߿��� ������ �� �ִ�.
	if( GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA && m_BattleOfMaya.GetEntrancePermit() == TRUE )
	{
		if( g_KanturuBattleUserMng.IsOverMaxUser() == TRUE )
		{
			LogAddTD( "[ KANTURU ][ Entrance Fail ] Over Max User [%s][%s] State(%d)-(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );				
			return 1;		// ���� ������ �ο� �� �ʰ�
		}
		// ���������Ʈ ���� ����
		if( CheckEqipmentMoonStone( iUserIndex ) == FALSE )
		{
			LogAddTD( "[ KANTURU ][ Entrance Fail ] Moon Stone is not exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 2;		// ���忡 �ʿ��� �������� ����
		}
#ifdef ADD_KANTURU_CONDITION_20060725
		// 1. ����,����Ʈ,�渱 ������ ���� ����
		if( ( gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type < MAKE_ITEMNUM(12,0) ||			// ����
			gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type > MAKE_ITEMNUM(12,6) )			// ����
#ifdef DARKLORD_WORK
			&& gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(13,30)		// ����
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1������(����� ����) ĭ�������� ����
			&& gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12,41)		// �ű�ĳ���� 1������(����� ����)
#endif			
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ����, ������ ���� ĭ���� ���� ����
			&& gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12,42)	// ������ ����
			&& gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12,43)	// ������ ����
#endif			
			&& gObj[iUserIndex].pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3)		// ����Ʈ
			&& gObj[iUserIndex].pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,37)		// �渱
#ifdef MODIFY_KANTURU_ENTERANCE_DARKHORSE_BUGFIX_20080627
			&& gObj[iUserIndex].pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,4)		// ��ũȣ��
#endif // MODIFY_KANTURU_ENTERANCE_DARKHORSE_BUGFIX_20080627
#ifdef ADD_THIRD_WING_20070525	// ĭ���� ������ �������ǿ� 3�� ���� �߰�
			&& ( gObj[iUserIndex].pInventory[EQUIPMENT_WING].m_Type < MAKE_ITEMNUM( 12, 36 ) ||			// 3�� �ѳ���
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

		// 2. ���ϸ��� ����
		if( gObj[iUserIndex].pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) )		// ���ϸ���
		{
			LogAddTD( "[ KANTURU ][ Entrance Fail ] Uniria Item is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 5;
		}
		// 3. ���Ź��� ����
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) ||	// ���Ź���
			gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) )	// ���Ź���
		{
			LogAddTD( "[ KANTURU ][ Entrance Fail ] Trasportation Ring is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
			return 6;
		}
#endif // ADD_KANTURU_CONDITION_20060725
		
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ĭ���� ������ ���� ����
		// 3-1. ����Ʈ�ذ����纯�Ź��� ����
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
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
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
		
		// ���� ����
		LogAddTD( "[ KANTURU ][ Entrance Success ] [%s][%s] State(%d)-(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_BattleOfMaya.GetBattleOfMayaState() );
		return 0;
	}
	else if( GetKanturuState() == KANTURU_STATE_TOWER_OF_REFINEMENT && m_TowerOfRefinement.GetEntrancePermit() == TRUE )
	{
		// ������ ž ����ð����� ������ �� �ִ�.
		LogAddTD( "[ KANTURU ][ Entrance Success ] [%s][%s] State(%d)-(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState(),
				m_TowerOfRefinement.GetTowerOfRefinementState() );
		return 0;		
	}
	else
	{
		// ������ �� ����
		LogAddTD( "[ KANTURU ][ Entrance Fail ] [%s][%s] State(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState() );
		return 3;	// ���尡���� ���°� �ƴ�
	}

	// ������ �� ����
	LogAddTD( "[ KANTURU ][ Entrance Error ] [%s][%s] State(%d)",
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetKanturuState() );
	return 4;	// �� �� ���� �������� ������ �� ����
}

BOOL CKanturu::CheckCanEnterKanturuBattle()
{
	// ������ ���ð� �߿��� ������ �� �ִ�.
	if( GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA 
		&& m_BattleOfMaya.GetEntrancePermit() == TRUE )	
	{
		return TRUE;
	}

	// ������ ž�� �����ְ� ���� ������ �����̸� ����
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
	// ���������Ʈ üũ ����
	if( GetEnableCheckMoonStone() == FALSE )
		return TRUE;

	if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].IsItem() || 
		gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].IsItem() )
	{
		// ������
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 38 ) )
		{
			// �������� 0�� �ƴ� ���� ����ȴ�.
			if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Durability != 0 )
				return TRUE;
		}
		// ����
		if( gObj[iUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 38 ) )
		{
			// �������� 0�� �ƴ� ���� ����ȴ�.
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
	// �׽�Ʈ�߿��� ��� �����ϴ�.
	return;	
#endif

	if( !gObjIsConnected( iUserIndex ) )
		return;

	LogAddTD( "[ KANTURU ][ Use GM Command ] [%s][%s] Command : %d", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, iCommand );
	
	switch( iCommand )
	{
	case KANTURU_GM_COMMAND_STANBY:					// "/ĭ����������"		
		SetKanturuState( KANTURU_STATE_BATTLE_STANDBY );
		m_BattleStanby.SetState( KANTURU_BATTLE_STANBY_STATE_START );		
		break;
	case KANTURU_GM_COMMAND_BATTLEOFMAYA:			// "/ĭ����������"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_STANBY1 );
		break;
	case KANTURU_GM_COMMAND_MAYA_MONSTER1:			// "/���߸���1"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_START1 );
		break;
	case KANTURU_GM_COMMAND_MAYA1:					// "/����1"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_MAYA1 );
		break;
	case KANTURU_GM_COMMAND_MAYA_MONSTER2:			// "/����2"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_START2 );
		break;
	case KANTURU_GM_COMMAND_MAYA2:					// "/����2"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_MAYA2 );
		break;
	case KANTURU_GM_COMMAND_MAYA_MONSTER3:			// "/���߸���3"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_START3 );
		break;
	case KANTURU_GM_COMMAND_MAYA3:					// "/����3"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_MAYA3 );
		break;
	case KANTURU_GM_COMMAND_MAYA3_SUCCESS:			// "/����3����"	
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetSuccessValue( TRUE );
		m_BattleOfMaya.SetSceneSuccessValue( TRUE );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_ENDCYCLE_MAYA3 );
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success - Macro[%s][%s]:/����3����",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );
		break;
	case KANTURU_GM_COMMAND_MAYA_SUCCESS:			// "/���߼���"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetSuccessValue( TRUE );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_END_MAYA3 );
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Success - Macro[%s][%s]:/���߼���",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );
		break;
	case KANTURU_GM_COMMAND_MAYA_FAIL:				// "/���߽���"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_MAYA );
		m_BattleOfMaya.SetSuccessValue( FALSE );
		m_BattleOfMaya.SetState( BATTLE_OF_MAYA_STATE_END );	
		LogAddTD( "[ KANTURU ][ BattleOfMaya ] Fail - Macro[%s][%s]:/���߽���",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );		
		break;
	case KANTURU_GM_COMMAND_NIGHTMARE:				// "/ĭ��������Ʈ�޾���"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_NIGHTMARE );
		m_BattleOfNightmare.SetState( BATTLE_OF_NIGHTMARE_STATE_IDLE );
		break;
	case KANTURU_GM_COMMAND_NIGHTMARE_SUCCESS:		// "/����Ʈ�޾��"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_NIGHTMARE );
		m_BattleOfNightmare.SetSuccessValue( TRUE );
		m_BattleOfNightmare.SetState( BATTLE_OF_NIGHTMARE_STATE_END );
		LogAddTD( "[ KANTURU ][ BattleOfNightmare ] Success - Macro[%s][%s]:/����Ʈ�޾��",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );			
		break;
	case KANTURU_GM_COMMAND_NIGHTMARE_FAIL:			// "/����Ʈ�޾����"
		SetKanturuState( KANTURU_STATE_BATTLE_OF_NIGHTMARE );
		m_BattleOfNightmare.SetSuccessValue( FALSE );
		m_BattleOfNightmare.SetState( BATTLE_OF_NIGHTMARE_STATE_END );
		LogAddTD( "[ KANTURU ][ BattleOfNightmare ] Fail - Macro[%s][%s]:/����Ʈ�޾����",
			 gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );			
		break;
	case KANTURU_GM_COMMAND_TOWEROFREFINEMENT:		// "/ĭ����������ž"
		SetKanturuState( KANTURU_STATE_TOWER_OF_REFINEMENT );
		m_TowerOfRefinement.SetState( TOWER_OF_REFINEMENT_STATE_REVITALIZATION );
		break;
	case KANTURU_GM_COMMAND_TOWER_OPEN:				// "/������ž����"
		m_TowerOfRefinement.SetEntrancePermit( TRUE );
		break;
	case KANTURU_GM_COMMAND_TOWER_CLOSE:			// "/������ž�ݱ�"
		m_TowerOfRefinement.SetEntrancePermit( FALSE );
		break;
	case KANTURU_GM_COMMAND_END:					// "/ĭ��������"
		SetState( KANTURU_STATE_END );		
		break;
	case KANTURU_GM_COMMAND_NEXT_STATE:				// "ĭ������������"	
		SetNextState( GetKanturuState() );		
		break;
	case KANTURU_GM_COMMAND_NEXT_DETAIL_STATE:		// "ĭ�����������λ���"
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
	case KANTURU_GM_COMMAND_CURRENT_STATE:			// "ĭ�����������"
		{
			KANTURU_UTIL.SendMsgToUser( iUserIndex, "KANTURU CURRENT STATE %d:%d"
				, GetKanturuState(), GetKanturuDetailState() );
		}
		break;
	}	
}
#endif	//#ifdef KANTURU_GM_COMMAND_20060709

#include "LogToFile.h"
CLogToFile	KANTURU_TEST_LOG( "KANTURU_TEST_LOG", ".\\KANTURU_TEST_LOG");		// �α����� ��ü
void CKanturu::UserMonsterCountCheck()
{	
#if ( TESTSERVER != 1 )
	return;
#endif

	// �������� ���� �˻�
	if( GetKanturuState() != KANTURU_STATE_BATTLE_OF_MAYA && GetKanturuState() != KANTURU_STATE_BATTLE_OF_NIGHTMARE )
	{
		return;
	}

	// 1. ���� ���� �� �˻� - Manager		
	KANTURU_TEST_LOG.Output( "[ KANTURU ][ Debug ] 1. UserMng Count:%d", g_KanturuBattleUserMng.GetUserCount() );

	// 2. ���� ���� �� �˻� - ��ü
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

	// 3. ���� ���� �� �˻� - Manager
	KANTURU_TEST_LOG.Output( "[ KANTURU ][ Debug ] 3. MonsterMng Count:%d", g_KanturuMonsterMng.GetAliveMonsterCount() );

	// 4. ���� ���� �� �˻� - ��ü
	int iMonsterCount = 0;
	for( int iAllMonsterCount = 0; iAllMonsterCount < ALLOC_USEROBJECTSTART; iAllMonsterCount++ )
	{
		if( gObj[iAllMonsterCount].MapNumber == MAP_INDEX_KANTURU_BOSS 
			&& gObj[iAllMonsterCount].Connected == 3
			&& gObj[iAllMonsterCount].Type	== OBJTYPE_MONSTER )
		{
			
			if( gObj[iAllMonsterCount].Class != KANTURU_MONSTER_CANNONTRAP 
				&& gObj[iAllMonsterCount].Class != KANTURU_MONSTER_LASERTRAP		// Ʈ���� ����
				&& gObj[iAllMonsterCount].Class != KANTURU_MONSTER_MAYA				// ���� ����
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
	
	// ��¥�� ����Ǿ���.
	if( m_iKanturuBattleDate < iDateInfo )
	{
		m_iKanturuBattleDate		= iDateInfo;	// ��¥ ���� ����	
		m_iKanturuBattleCounter		= 0;			// �ʱ�ȭ
	}

	// ī���� ����
	++m_iKanturuBattleCounter;	

	LogAddTD( "[ KANTURU ][ TimeAttackEvent ] Date:%d, Counter:%d", m_iKanturuBattleDate, m_iKanturuBattleCounter );
}
#endif	// KANTURU_TIMEATTACK_EVENT_20060822



#endif