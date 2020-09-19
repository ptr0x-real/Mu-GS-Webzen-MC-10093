// KanturuBattleUserMng.h: interface for the CKanturuBattleUserMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUBATTLEUSERMNG_H__08BE7DD0_2105_4D8D_B6CE_1DCCBDBF5C0D__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUBATTLEUSERMNG_H__08BE7DD0_2105_4D8D_B6CE_1DCCBDBF5C0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuBattleUser.h"

#define MAX_KANTURU_BATTLE_USER 15
class CKanturuBattleUserMng
{
public:
	CKanturuBattleUser		m_BattleUser[MAX_KANTURU_BATTLE_USER];

private:	
	int						m_iBattleUserCount;
	int						m_iBattleMaxUser;
	
public:
	CKanturuBattleUserMng();
	virtual ~CKanturuBattleUserMng();

	// RESET DATA
	void					ResetAllData();
	
	// USER MANAGEMENT
	BOOL					AddUserData( int iIndex );
	BOOL					DeleteUserData( int iIndex );	
	void					CheckUserState();
	
	BOOL					MoveAllUser( int iGateNumber );

	// GET INFORMATION		
	int						GetUserCount();
	BOOL					IsEmpty();
	BOOL					IsOverMaxUser();
	int						IsBattleUser( int iIndex );

	// SET DATA
	void					SetMaxUser( int iMaxUser );
	int						GetMaxUser();

	// 전투에서 승리한 유저에 대한 로그를 남긴다.
	void					LogBattleWinnerUserInfo( BYTE btFlag, int iElpasedTime );	
	
};

extern CKanturuBattleUserMng	g_KanturuBattleUserMng;
#endif // !defined(AFX_KANTURUBATTLEUSERMNG_H__08BE7DD0_2105_4D8D_B6CE_1DCCBDBF5C0D__INCLUDED_)
