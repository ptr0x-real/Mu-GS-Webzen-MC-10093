//----------------------------------------------------------------------------------
// FileName				: RaklionBattleUserMng.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 보스전 유저객체 관리 클래스
//----------------------------------------------------------------------------------
// RaklionBattleUserMng.h: interface for the CRaklionBattleUserMng class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONBATTLEUSERMNG_H__1DE3F610_128A_4FF7_9EDB_0E2972DCB784__INCLUDED_) && defined(ADD_RAKLION_20080408)
#define AFX_RAKLIONBATTLEUSERMNG_H__1DE3F610_128A_4FF7_9EDB_0E2972DCB784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

#define MAX_RAKLION_BATTLE_USER 100

class CRaklionBattleUserMng  
{
public:
	// 보스전에 참가하는 유저 벡터
	vector<INT>				m_BattleUser;
	
public:
	CRaklionBattleUserMng();
	virtual ~CRaklionBattleUserMng();
	
	// RESET DATA
	void					ResetAllData();
	
	// USER MANAGEMENT
	BOOL					AddUserData( int iIndex );
	BOOL					DeleteUserData( int iIndex );	
	void					CheckUserState();
	
	BOOL					MoveAllUser( int iGateNumber );
	
	// GET INFORMATION		
	INT						GetUserCount();
	BOOL					IsEmpty();
	INT						IsBattleUser( int iIndex );
	INT						GetUserObjIndex( INT iBattleUserNumber );
		
	// 전투에서 승리한 유저에 대한 로그를 남긴다.
	void					LogBattleWinnerUserInfo( BYTE btFlag, int iElpasedTime );
};

extern CRaklionBattleUserMng	g_RaklionBattleUserMng;

#endif // !defined(AFX_RAKLIONBATTLEUSERMNG_H__1DE3F610_128A_4FF7_9EDB_0E2972DCB784__INCLUDED_)
