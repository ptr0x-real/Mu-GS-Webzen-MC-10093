//----------------------------------------------------------------------------------
// FileName				: RaklionBattleUserMng.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ������ ������ü ���� Ŭ����
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
	// �������� �����ϴ� ���� ����
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
		
	// �������� �¸��� ������ ���� �α׸� �����.
	void					LogBattleWinnerUserInfo( BYTE btFlag, int iElpasedTime );
};

extern CRaklionBattleUserMng	g_RaklionBattleUserMng;

#endif // !defined(AFX_RAKLIONBATTLEUSERMNG_H__1DE3F610_128A_4FF7_9EDB_0E2972DCB784__INCLUDED_)
