//----------------------------------------------------------------------------------
// FileName				: RaklionBattleUserMng.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 보스전 유저객체 관리 클래스
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
	// 유저 벡터를 초기화 한다
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
	
	// 유저를 벡터에 추가
	m_BattleUser.push_back( iIndex );

	// 추가된 유저에게 현재 보스전의 상태를 알려준다.
	g_RaklionUtil.NotifyRaklionCurrentState( iIndex, g_Raklion.GetRaklionState()
		, g_Raklion.GetRaklionStateDetail() );

	LogAddTD( "[ RAKLION ][ Battle User ] Add User [%s][%s] Current Battle User:%d",
		gObj[iIndex].AccountID, gObj[iIndex].Name, m_BattleUser.size() );
	
	return TRUE;
}

// 라클리온 보스전 전투에 참여중인 유저가 
// 1. 죽었다 2. 다른 맵으로 이동 3. 접속을 종료했을 때 관련 데이터를 삭제한다.
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
		// 1. 접속종료 확인
		if( !gObjIsConnected( m_BattleUser[iCount] ) )
		{
			DeleteUserData( m_BattleUser[iCount] );
			LogAddTD( "[ RAKLION ][ BattleUser ] Delete User - Disconnect [%s][%s]",
				gObj[m_BattleUser[iCount]].AccountID, gObj[m_BattleUser[iCount]].Name );	
		}
		
		// 2. 현재 맵 번호가 같은지 확인
		if( gObj[m_BattleUser[iCount]].MapNumber != MAP_INDEX_RAKLION_BOSS 
			&& gObj[m_BattleUser[iCount]].m_State == OBJST_PLAYING 
			&& gObj[m_BattleUser[iCount]].Live == 1 )	// 멀쩡한 상태인가?
		{
#ifdef MODIFY_RECALLSKIL_BUGFIX_20081107
			// 소환된 자들은 일단 빼주자.. 이 function 호출 이후에 mapNumber에 넣어주는 로직이라..   by natinda
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
			// 맵을 정상적으로 이동하지 못한 유저는 관리 데이터에서 삭제한 뒤 칸투르 맵 밖으로 쫒아낸다.

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
	// 유저 벡터가 비었으면 TRUE, 그렇지 않으면 FALSE를 return
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