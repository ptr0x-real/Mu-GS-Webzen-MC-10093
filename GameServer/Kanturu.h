// Kanturu.h: interface for the CKanturu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURU_H__191641E6_BDB4_41AA_8079_5319C61CA981__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURU_H__191641E6_BDB4_41AA_8079_5319C61CA981__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapClass.h"

#include "KanturuBattleUserMng.h"
#include "KanturuStateInfo.h"
#include "KanturuObjInfo.h"

#include "KanturuBattleStanby.h"
#include "KanturuBattleOfMaya.h"
#include "KanturuBattleOfNightmare.h"
#include "KanturuTowerOfRefinement.h"

#include "KanturuEntranceNPC.h"

// ĭ���� ���� ������ �� ������ ������ ����Ѵ�.
class CKanturu  
{
private:
	// KANTURU INFORMATION	
	BOOL						m_bKanturuEnable;					// ĭ���� �������� �����ϴ°�?
	int							m_iKanturuState;					// ĭ���� ���� ����
	BOOL						m_bIsSucccess;						// ����, ����
	
	// KANTURU DATA FILE
	BOOL						m_bFileDataLoad;					// ������ ���� �ε� ��/��( Kanturu.dat )
	CKanturuStateInfo			m_StateInfo[MAX_KANTURU_STATE];		// ĭ���� �� ������ �������ǿ� ���� ����
	int							m_StateInfoCount;					// ���Ͽ� ����� ĭ���� �� ������ ��

	// DETAIL STATE
	CKanturuBattleStanby		m_BattleStanby;						// ĭ���� ������ ������
	CKanturuBattleOfMaya		m_BattleOfMaya;						// ĭ���� ������ ������
	CKanturuBattleOfNightmare	m_BattleOfNightmare;				// ĭ���� ������ ����Ʈ�޾���
	CKanturuTowerOfRefinement	m_TowerOfRefinement;				// ĭ���� ������ ������ ž
	
	// MAP
	MapClass					m_KanturuMap[MAX_KANTURU_TOWEROFREFINEMENT_MAP];	// ������ ž ����/ ������ ���� �� ����

	// MOONSTONE CHECK
	BOOL						m_bEnableCheckMoonStone;			// ����� ���������Ʈ üũ ����->��ڷ�
	
#ifdef KANTURU_TIMEATTACK_EVENT_20060822	// ĭ���� ������ ī���� ���� ����
	// EVENT
	int							m_iKanturuBattleCounter;	// �ϳ��� �������� ���۵Ǹ� +1. �Ϸ� ������ �ʱ�ȭ�Ѵ�.
	int							m_iKanturuBattleDate;		// �Ϸ������ �ʱ�ȭ�ϱ� ���� ��¥ ������ ���� �� �д�.
#endif
	
public:
	CKanturu();
	virtual ~CKanturu();

	void	Init();
	void	ResetAllData();

	BOOL	LoadData( LPSTR lpszFileName );								// ĭ���� �������� ���� �����͸� �ε��Ѵ�.
	void	LoadKanturuMapAttr( LPSTR lpszFileName, BYTE btLevel );		// �� �����͸� �ε��Ѵ�.
	void	SetKanturuMapAttr( BYTE btLevel );							// ������ ž ����/������ ���� �� �Ӽ� ������ �ٲ۴�.
	
	void	Run();
public:
	// STATE
	void	SetState( int iKanturuState );	
	void	SetNextState( int iCurrentState );
	
	void	ChangeState( int iState, int DetailState );

	void	SetState_NONE();
	void	SetState_BATTLE_STANDBY();	
	void	SetState_BATTLE_OF_MAYA();
	void	SetState_BATTLE_OF_NIGHTMARE();
	void	SetState_TOWER_OF_REFINEMENT();	
	void	SetState_END();
	void	SetState_ENDCYCLE();

	void	ProcState_NONE();
	void	ProcState_BATTLE_STANDBY();
	void	ProcState_BATTLE_OF_MAYA();
	void	ProcState_BATTLE_OF_NIGHTMARE();
	void	ProcState_TOWER_OF_REFINEMENT();	
	void	ProcState_END();
	void	ProcState_ENDCYCLE();

	// KANTURU INFORMATION	
	void	SetKanturuEnable( BOOL bEnable )		{ m_bKanturuEnable = bEnable; }
	int		IsKanturuEnable()						{ return m_bKanturuEnable; }
	void	SetKanturuState( int iKanturuState )	{ m_iKanturuState = iKanturuState; }
	int		GetKanturuState()						{ return m_iKanturuState; }	
	int		GetKanturuDetailState();
	
	void	SetSuccessValue( BOOL bIsSuccess )		{ m_bIsSucccess	= bIsSuccess; }
	BOOL	GetSuccessValue()						{ return m_bIsSucccess; }

	// SCHEDULE
	void	CheckStateTime();
	int		GetRemainTime();
	
	// BATTLE USER MANAGEMENT
	void	CheckUserOnKanturuBossMap();			// ���������� ������� �ʿ� �ִ� ������ �i�Ƴ���.
	int		CheckEnterKanturu( int iUserIndex );	// ĭ������ ���� �����Ѱ�?
	BOOL	CheckCanEnterKanturuBattle();			// ĭ���� ������ ������ ������ �� �ִ� �����ΰ�?

	// MOONSTONE
	void	SetEnableCheckMoonStone( BOOL bEnable )		{ m_bEnableCheckMoonStone = bEnable; }
	BOOL	GetEnableCheckMoonStone()					{ return m_bEnableCheckMoonStone; }
	BOOL	CheckEqipmentMoonStone( int iUserIndex );

	// MONSTER MANAGEMENT
	void	KanturuMonsterDieProc( int iMonIndex, int iKillerIndex );

#ifdef KANTURU_GM_COMMAND_20060709
	// GM COMMAND
	void	OperateGmCommand( int iUserIndex, int iCommand );
#endif

	// KANTURU MONSTER INFO LOG
	void	UserMonsterCountCheck();

#ifdef KANTURU_TIMEATTACK_EVENT_20060822
	void	SetKanturuTimeAttackEventInfo();

	int		GetKanturuBattleCounter()	{ return m_iKanturuBattleCounter; }
	int		GetKanturuBattleDate()		{ return m_iKanturuBattleDate; }
#endif
};

extern CKanturu g_Kanturu;

#endif // !defined(AFX_KANTURU_H__191641E6_BDB4_41AA_8079_5319C61CA981__INCLUDED_)
