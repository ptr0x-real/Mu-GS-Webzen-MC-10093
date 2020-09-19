// KanturuBattleUserMng.cpp: implementation of the CKanturuBattleUserMng class.
////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuBattleUserMng.h"

#include "KanturuDefine.h"

CKanturuBattleUserMng	g_KanturuBattleUserMng;

#include "KanturuUtil.h"
static CKanturuUtil	KANTURU_UTIL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuBattleUserMng::CKanturuBattleUserMng()
{
	ResetAllData();	
}

CKanturuBattleUserMng::~CKanturuBattleUserMng()
{
	
}

void CKanturuBattleUserMng::ResetAllData()
{
	m_iBattleUserCount = 0;
	SetMaxUser( MAX_KANTURU_BATTLE_USER );

	for( int iCount = 0; iCount < MAX_KANTURU_BATTLE_USER; iCount++ )
	{
		m_BattleUser[iCount].ResetData();
	}		
}

//----------------------------------------------------------------------------
BOOL CKanturuBattleUserMng::AddUserData( int iIndex )
{
	if( gObjIsConnected( iIndex ) == FALSE )
	{
		LogAddTD( "[ KANTURU ][ BattleUser ] Add User Fail - Disconnect User [%s][%s]",
			gObj[iIndex].AccountID, gObj[iIndex].Name );
		return FALSE;
	}

	if( IsOverMaxUser() )
	{
		LogAddTD( "[ KANTURU ][ BattleUser ] Add User Fail - Over Max User [%s][%s]",
			gObj[iIndex].AccountID, gObj[iIndex].Name );
		return FALSE;
	}

	for( int iCount = 0; iCount < MAX_KANTURU_BATTLE_USER; iCount++ )
	{
		if( m_BattleUser[iCount].IsUseData() == FALSE )
		{
			m_BattleUser[iCount].SetIndex( iIndex );
			
			++m_iBattleUserCount;

#ifndef EXTEND_LOG_SYSTEM_04_20060823
			LogAddTD( "[ KANTURU ][ Battle User ] Add User [%s][%s] Current Battle User:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, m_iBattleUserCount );
#endif // EXTEND_LOG_SYSTEM_04_20060823 NOT		
			return TRUE;
		}
	}

	// error!!
	return FALSE;
}

// ĭ���� ������ ������ �������� ������ 
// 1. �׾��� 2. �ٸ� ������ �̵� 3. ������ �������� �� ���� �����͸� �����Ѵ�.
BOOL CKanturuBattleUserMng::DeleteUserData( int iIndex )
{
	if( iIndex < 0 || iIndex > MAX_OBJECT - 1 )
	{
		LogAddTD( "[ KANTURU ][ BattleUser ] Delete User Fail - Unvalid Index:%d", iIndex );
		return FALSE;
	}

	for( int iCount = 0; iCount < MAX_KANTURU_BATTLE_USER; iCount++ )
	{
		if( m_BattleUser[iCount].IsUseData() == TRUE )
		{
			if( m_BattleUser[iCount].GetIndex() == iIndex )
			{
				m_BattleUser[iCount].ResetData();
				--m_iBattleUserCount;

#ifdef ADD_NEW_NPC_KANTURU_20060612
				// ĭ���� ������ �ʿ� ������ �����Ѵ�.
				gObj[iIndex].m_bKanturuEntranceByNPC	= FALSE;
#endif

				LogAddTD( "[ KANTURU ][ BattleUser ] Delete User - [%s][%s] Current Battle User:%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, m_iBattleUserCount );	

				return TRUE;
			}
		}
	}

	return FALSE;	
}

// ĭ���� ������ ������ �������� ������ ���¸� �˻��Ѵ�.
void CKanturuBattleUserMng::CheckUserState()
{
	int iIndex;
	for( int iCount = 0; iCount < MAX_KANTURU_BATTLE_USER; iCount++ )
	{
		if( m_BattleUser[iCount].IsUseData() == TRUE )
		{
			iIndex = m_BattleUser[iCount].GetIndex();

			// 1. �������� Ȯ��
			if( !gObjIsConnected( iIndex ) )
			{
				DeleteUserData( iIndex );
				LogAddTD( "[ KANTURU ][ BattleUser ] Delete User - Disconnect [%s][%s]",
					gObj[iIndex].AccountID, gObj[iIndex].Name );	
			}

			// 2. ���� �� ��ȣ�� ������ Ȯ��
			if( gObj[iIndex].MapNumber != MAP_INDEX_KANTURU_BOSS 
				&& gObj[iIndex].m_State == OBJST_PLAYING 
				&& gObj[iIndex].Live == 1 )	// ������ �����ΰ�?
			{
				DeleteUserData( iIndex );
				LogAddTD( "[ KANTURU ][ BattleUser ] Delete User - Map Move [%s][%s]",
					gObj[iIndex].AccountID, gObj[iIndex].Name );	
			}
		}
	}	
}

//----------------------------------------------------------------------------
// ĭ���� ������ ������ �������� ������ ��� Ư�� ��ġ�� �ű��.
BOOL CKanturuBattleUserMng::MoveAllUser( int iGateNumber )
{
	int iIndex;
	BOOL bMoveGateSuccess = FALSE;

	for( int iCount = 0; iCount < MAX_KANTURU_BATTLE_USER; iCount++ )
	{
		iIndex = m_BattleUser[iCount].GetIndex();
		if( m_BattleUser[iCount].IsUseData() )
		{		
			bMoveGateSuccess = gObjMoveGate( iIndex, iGateNumber );

			if( bMoveGateSuccess == TRUE )
			{
				LogAddTD( "[ KANTURU ][ BattleUser ] [%s][%s] MoveGate(%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iGateNumber );
			}
			else
			{
				// ���� ���������� �̵����� ���� ������ ���� �����Ϳ��� ������ �� ĭ���� �� ������ �i�Ƴ���.
#ifdef KANTURU_TEST_200607119
				KANTURU_UTIL.SendMsgToUser( iIndex, "����Ʈ�޾� ������ �����̵��� �� �����ϴ�. �������� �ٷ� �����ּ���" );
#endif
				DeleteUserData( iIndex );
				LogAddTD( "[ KANTURU ][ BattleUser ] [%s][%s] MoveGate Fail (%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iGateNumber );

				gObjMoveGate( iIndex, KANTURU_DEFAULT_REGEN_MAP_GATE );
			}
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// �������� �����ϰ�
void CKanturuBattleUserMng::SetMaxUser( int iMaxUser )
{
	m_iBattleMaxUser = iMaxUser;

	if( iMaxUser > MAX_KANTURU_BATTLE_USER )
	{
		m_iBattleMaxUser = MAX_KANTURU_BATTLE_USER;
		LogAddTD( "[ KANTURU ][ BattleUser ] Set Max User:%d", iMaxUser );
	}
}

int CKanturuBattleUserMng::GetMaxUser()
{
	return m_iBattleMaxUser;
}

int CKanturuBattleUserMng::GetUserCount()
{
	return m_iBattleUserCount;
}

// ĭ���� �������� �������� ������ ������ TRUE, ������ FALSE
BOOL CKanturuBattleUserMng::IsEmpty()
{
	if( m_iBattleUserCount <= 0 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CKanturuBattleUserMng::IsOverMaxUser()
{
	if( m_iBattleUserCount >= MAX_KANTURU_BATTLE_USER )
	{
		return TRUE;
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
#ifdef KANTURU_TIMEATTACK_EVENT_20060822
#include "Include\cb_protocol.h"
#include "Event.h"
#include "GameMain.h"
#endif

// ���� ĭ���� �����ʿ� �ִ� ������ ���� ������ �α׷� �����.
void CKanturuBattleUserMng::LogBattleWinnerUserInfo( BYTE btFlag, int iElapsedTime )
{
	int iIndex = -1;
	float fTime = ( float )iElapsedTime / 1000;
	for( int iCount =0; iCount < MAX_KANTURU_BATTLE_USER; iCount++ )
	{
		if( m_BattleUser[iCount].IsUseData() == TRUE )
		{
			iIndex = m_BattleUser[iCount].GetIndex();

			LogAddTD( "[ KANTURU ][ BATTLE WINNER ] [%d/ElapsedTime:%0.3f] [%s][%s] MapNumber[%d]-(%d/%d)",
					btFlag, fTime,
					gObj[iIndex].AccountID, gObj[iIndex].Name, 
					gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y );

#ifdef KANTURU_TIMEATTACK_EVENT_20060822
			KANTURU_UTIL.SendDataKanturuTimeAttackEvent( iIndex, btFlag, ( int )fTime );
#endif
		}
	}	
}

#endif