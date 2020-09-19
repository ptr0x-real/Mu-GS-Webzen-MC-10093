//----------------------------------------------------------------------------------
// FileName				: Raklion.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ������ ���� �����ٸ�
//----------------------------------------------------------------------------------
// Raklion.h: interface for the CRaklion class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLION_H__4DB886D1_A742_451C_827C_9E3C98C66B99__INCLUDED_) && defined(ADD_RAKLION_20080408)
#define AFX_RAKLION_H__4DB886D1_A742_451C_827C_9E3C98C66B99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapClass.h"

#include "RaklionDefine.h"
#include "RaklionBattleUserMng.h"
#include "RaklionObjInfo.h"
#include "RaklionBattleOfSelupan.h"

class CRaklion  
{
private:
	// RAKLION INFORMATION	
	BOOL						m_bRaklionBossEnable;				// ��Ŭ���� �������� �����ϴ°�?
	RAKLION_STATE				m_iRaklionState;					// ��Ŭ���� ���� ����(enum��)
	INT							m_iRaklionStateDetail;				// �������� ���� ����
	BOOL						m_bIsBossKillSuccess;				// ����, ����

	DWORD						m_iBossAppearanceStartTime;			// �Ź̾��� ��� ���ŵ� �� ������ ����������� ������(����ð�)
	DWORD						m_iBosszoneCloseStartTime;			// �������� ���۵ȵ� 5���� �������� ����ϱ� ���� �ð��� �����Ѵ�.
	DWORD						m_iBosszoneOpenStartTime;			// �������� ������ 5�а� ��⸦ ���� �ð��� �����Ѵ�.
	
	// RAKLION DATA FILE
	BOOL						m_bFileDataLoad;					// ������ ���� �ε� ��/��( Raklion.dat )
	INT							m_iBossAppearanceDelay;				// ���� ����� ������ ���� ������
	INT							m_iBosszoneCloseDelay;				// ���� ���� �� ������ ������ ������
	INT							m_iBosszoneOpenDelay;				// ������ ���� �� ������ ������� ������
	INT							m_iBossEggHalf;						// �� �ı� �� ������ ���� ����
	INT							m_iBossEggMax;						// ������ �� �ִ� ����

	// DETAIL STATE
	CRaklionBattleOfSelupan		m_BattleOfSelupan;					// ��Ŭ���� ������ ��������
	BOOL						m_bIsNotify1;						// NOTIFY_1�� ���� �ߴ°�?
	INT							m_iBossEggDieCount;					// �ı��� ������ ���� ����
	
#ifdef UPDATE_SEASON_4_EVENT_LOG_200800715
	// EVENT
	BOOL						m_bEvent;							// 0�ÿ� ���� �α׸� ����°�?
	
	INT							m_iTodayYear;						// ���� ��¥�� ����صδ� ������
	INT							m_iTodayMon;
	INT							m_iTodayDay;
#endif // UPDATE_SEASON_4_EVENT_LOG_200800715
	
public:
	CRaklion();
	virtual ~CRaklion();
	
	void	Init();
	void	ResetAllData();

	BOOL	LoadData( LPSTR lpszFileName );								// ��Ŭ���� �������� ���� �����͸� �ε��Ѵ�.
	void	LoadRaklionMapAttr( LPSTR lpszFileName, BYTE btLevel );		// �� �����͸� �ε��Ѵ�.
	
	void	Run();
public:
	// STATE
	void	SetState( int iRaklionState );	
	void	SetNextState( RAKLION_STATE iCurrentState );
	
	void	ChangeState( int iState, int DetailState );

	void	SetState_IDLE();
	void	SetState_NOTIFY_1();
	void	SetState_STANDBY();
	void	SetState_NOTIFY_2();
	void	SetState_READY();
	void	SetState_START_BATTLE();
	void	SetState_NOTIFY_3();
	void	SetState_CLOSE_DOOR();
	void	SetState_ALL_USER_DIE();
	void	SetState_NOTIFY_4();
	void	SetState_END();

	void	ProcState_IDLE();
	void	ProcState_NOTIFY_1();
	void	ProcState_STANDBY();
	void	ProcState_NOTIFY_2();
	void	ProcState_READY();
	void	ProcState_START_BATTLE();
	void	ProcState_NOTIFY_3();
	void	ProcState_CLOSE_DOOR();
	void	ProcState_ALL_USER_DIE();
	void	ProcState_NOTIFY_4();
	void	ProcState_END();

	// RAKLION INFORMATION	
	void	SetRaklionBossEnable( BOOL bEnable );
	int		IsRaklionBossEnable();
	void	SetRaklionState( RAKLION_STATE iRaklionState );
	int		GetRaklionState();
	void	SetRaklionStateDetail( INT iSelupanState );
	int		GetRaklionStateDetail();
	
	void	SetBossKillValue( BOOL bIsBossKill )	{ m_bIsBossKillSuccess	= bIsBossKill; }
	BOOL	GetBossKillValue()						{ return m_bIsBossKillSuccess; }

	// BATTLE USER MANAGEMENT
	void	CheckUserOnRaklionBossMap();			// ���������� ������� �ʿ� �ִ� ������ �i�Ƴ���.
	int		CheckEnterRaklion( int iUserIndex );	// ĭ������ ���� �����Ѱ�?
	BOOL	CheckCanEnterRaklionBattle();			// ĭ���� ������ ������ ������ �� �ִ� �����ΰ�?

	// MONSTER MANAGEMENT
	void	BossEggDieIncrease();
	void	BossEggDieDecrease();
	DWORD	GetBossEggDieCount();
	void	RegenBossEgg();
	void	DeleteBossEgg();
	
	// GM COMMAND
	void	OperateGmCommand( int iUserIndex, int iCommand );

	// RAKLION MONSTER INFO LOG
	void	UserMonsterCountCheck();
};

extern CRaklion g_Raklion;

#endif // !defined(AFX_RAKLION_H__4DB886D1_A742_451C_827C_9E3C98C66B99__INCLUDED_)
