//----------------------------------------------------------------------------------
// FileName				: RaklionBattleUserMng.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ������ ������ü ���� Ŭ����
//----------------------------------------------------------------------------------
// RaklionBattleUserMng.cpp: implementation of the CRaklionBattleUserMng class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_RAKLION_20080408

#include "RaklionBattleUserMng.h"

#include "RaklionDefine.h"
#include "Raklion.h"
#include "RaklionUtil.h"

CRaklionBattleUserMng	g_RaklionBattleUserMng;
static CRaklionUtil		g_RaklionUtil;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionBattleUserMng::CRaklionBattleUserMng()
{
	ResetAllData();
}

CRaklionBattleUserMng::~CRaklionBattleUserMng()
{
	ResetAllData();
}

void CRaklionBattleUserMng::ResetAllData()
{
	// ���� ���͸� �ʱ�ȭ �Ѵ�
	while( !m_BattleUser.empty() )
	{
		m_BattleUser.pop_back();
	}
	m_BattleUser.clear();	
}

BOOL CRaklionBattleUserMng::AddUserData( int iIndex )
{
	if( gObjIsConnected( iIndex ) == FALSE )
	{
		LogAddTD( "[ RAKLION ][ BattleUser ] Add User Fail - Disconnect User [%s][%s]",
			gObj[iIndex].AccountID, gObj[iIndex].Name );
		return FALSE;
	}
	
	// ������ ���Ϳ� �߰�
	m_BattleUser.push_back( iIndex );

	// �߰��� �������� ���� �������� ���¸� �˷��ش�.
	g_RaklionUtil.NotifyRaklionCurrentState( iIndex, g_Raklion.GetRaklionState()
		, g_Raklion.GetRaklionStateDetail() );

	LogAddTD( "[ RAKLION ][ Battle User ] Add User [%s][%s] Current Battle User:%d",
		gObj[iIndex].AccountID, gObj[iIndex].Name, m_BattleUser.size() );
	
	return TRUE;
}

// ��Ŭ���� ������ ������ �������� ������ 
// 1. �׾��� 2. �ٸ� ������ �̵� 3. ������ �������� �� ���� �����͸� �����Ѵ�.
BOOL CRaklionBattleUserMng::DeleteUserData( int iIndex )
{
	if( iIndex < 0 || iIndex > MAX_OBJECT - 1 )
	{
		LogAddC( LOGC_RED, "[ RAKLION ][ BattleUser ] Delete User Fail - Unvalid Index:%d", iIndex );
		return FALSE;
	}
	
	for( int iCount=0; iCount < m_BattleUser.size(); ++iCount )
	{
		if( m_BattleUser[iCount] == iIndex )
		{
			m_BattleUser.erase( m_BattleUser.begin() + iCount );
			
			LogAddTD( "[ RAKLION ][ BattleUser ] Delete User - [%s][%s] Current Battle User:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, m_BattleUser.size() );	
			
			return TRUE;
		}
	}
	
	return FALSE;
}

void CRaklionBattleUserMng::CheckUserState()
{
	for( int iCount = 0; iCount < m_BattleUser.size(); iCount++ )
	{
		// 1. �������� Ȯ��
		if( !gObjIsConnected( m_BattleUser[iCount] ) )
		{
			DeleteUserData( m_BattleUser[iCount] );
			LogAddTD( "[ RAKLION ][ BattleUser ] Delete User - Disconnect [%s][%s]",
				gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name );	
		}
		
		// 2. ���� �� ��ȣ�� ������ Ȯ��
		if( gObj[m_BattleUser[iCount]].MapNumber != MAP_INDEX_RAKLION_BOSS 
			&& gObj[m_BattleUser[iCount]].m_State == OBJST_PLAYING 
			&& gObj[m_BattleUser[iCount]].Live == 1 )	// ������ �����ΰ�?
		{
#ifdef MODIFY_RECALLSKIL_BUGFIX_20081107
			// ��ȯ�� �ڵ��� �ϴ� ������.. �� function ȣ�� ���Ŀ� mapNumber�� �־��ִ� �����̶�..   by natinda
			if(gObj[m_BattleUser[iCount]].SkillRecallParty_MapNumber != MAP_INDEX_RAKLION_BOSS) {
				DeleteUserData( m_BattleUser[iCount] );
				LogAddTD( "[ RAKLION ][ BattleUser ] Delete User - Map Move [%s][%s]",
					gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name );	
			}	
#else
			DeleteUserData( m_BattleUser[iCount] );
			LogAddTD( "[ RAKLION ][ BattleUser ] Delete User - Map Move [%s][%s]",
				gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name );	
#endif //MODIFY_RECALLSKIL_BUGFIX_20081107
		}
	}
}

BOOL CRaklionBattleUserMng::MoveAllUser( int iGateNumber )
{
	BOOL bMoveGateSuccess = FALSE;
	
	for( int iCount = 0; iCount < m_BattleUser.size(); iCount++ )
	{
		bMoveGateSuccess = gObjMoveGate( m_BattleUser[iCount], iGateNumber );
		
		if( bMoveGateSuccess == TRUE )
		{
			LogAddTD( "[ RAKLION ][ BattleUser ] [%s][%s] MoveGate(%d)",
				gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name, iGateNumber );
		}
		else
		{
			// ���� ���������� �̵����� ���� ������ ���� �����Ϳ��� ������ �� ĭ���� �� ������ �i�Ƴ���.

			DeleteUserData( m_BattleUser[iCount] );
			LogAddTD( "[ RAKLION ][ BattleUser ] [%s][%s] MoveGate Fail (%d)",
				gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name, iGateNumber );
			
			gObjMoveGate( m_BattleUser[iCount], RAKLION_DEFAULT_REGEN_GATE );
		}
	}
	
	return TRUE;
}

INT CRaklionBattleUserMng::GetUserCount()
{
	return m_BattleUser.size();
}

BOOL CRaklionBattleUserMng::IsEmpty()
{
	// ���� ���Ͱ� ������� TRUE, �׷��� ������ FALSE�� return
	return m_BattleUser.empty();
}

void CRaklionBattleUserMng::LogBattleWinnerUserInfo( BYTE btFlag, int iElapsedTime )
{
}

//-------------------------------------------------------------------------------------------------
INT CRaklionBattleUserMng::GetUserObjIndex( INT iBattleUserNumber )
{
	INT iObjIndex = m_BattleUser[iBattleUserNumber];

	return iObjIndex;
}

#ifdef MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_20080818
//-------------------------------------------------------------------------------------------------
INT CRaklionBattleUserMng::IsBattleUser( int iIndex )
{
	for( INT iCount=0; iCount<GetUserCount(); ++iCount )
	{
		if( iIndex == m_BattleUser[iCount] )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}
#endif // MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_20080818

#endif