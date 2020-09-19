//----------------------------------------------------------------------------------
// FileName				: Raklion.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 보스전 관련 스케줄링
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
	BOOL						m_bRaklionBossEnable;				// 라클리온 보스전을 실행하는가?
	RAKLION_STATE				m_iRaklionState;					// 라클리온 현재 상태(enum값)
	INT							m_iRaklionStateDetail;				// 세루판의 세부 상태
	BOOL						m_bIsBossKillSuccess;				// 성공, 실패

	DWORD						m_iBossAppearanceStartTime;			// 거미알이 모두 제거된 후 보스가 나오기까지의 딜레이(연출시간)
	DWORD						m_iBosszoneCloseStartTime;			// 보스전이 시작된뒤 5분후 보스존을 폐쇄하기 위한 시간을 저장한다.
	DWORD						m_iBosszoneOpenStartTime;			// 보스전을 종료전 5분간 대기를 위한 시간을 저장한다.
	
	// RAKLION DATA FILE
	BOOL						m_bFileDataLoad;					// 데이터 파일 로드 유/무( Raklion.dat )
	INT							m_iBossAppearanceDelay;				// 보스 등장시 연출을 위한 딜레이
	INT							m_iBosszoneCloseDelay;				// 보스 등장 후 보스존 폐쇄까지 딜레이
	INT							m_iBosszoneOpenDelay;				// 보스전 종료 후 보스존 개방까지 딜레이
	INT							m_iBossEggHalf;						// 알 파괴 중 연출을 위한 갯수
	INT							m_iBossEggMax;						// 세루판 알 최대 갯수

	// DETAIL STATE
	CRaklionBattleOfSelupan		m_BattleOfSelupan;					// 라클리온 보스전 세루판전
	BOOL						m_bIsNotify1;						// NOTIFY_1을 실행 했는가?
	INT							m_iBossEggDieCount;					// 파괴된 세루판 알의 개수
	
#ifdef UPDATE_SEASON_4_EVENT_LOG_200800715
	// EVENT
	BOOL						m_bEvent;							// 0시에 유저 로그를 남겼는가?
	
	INT							m_iTodayYear;						// 오늘 날짜를 백업해두는 변수들
	INT							m_iTodayMon;
	INT							m_iTodayDay;
#endif // UPDATE_SEASON_4_EVENT_LOG_200800715
	
public:
	CRaklion();
	virtual ~CRaklion();
	
	void	Init();
	void	ResetAllData();

	BOOL	LoadData( LPSTR lpszFileName );								// 라클리온 보스전에 관한 데이터를 로드한다.
	void	LoadRaklionMapAttr( LPSTR lpszFileName, BYTE btLevel );		// 맵 데이터를 로드한다.
	
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
	void	CheckUserOnRaklionBossMap();			// 비정상적인 방법으로 맵에 있는 유저는 쫒아낸다.
	int		CheckEnterRaklion( int iUserIndex );	// 칸투르에 입장 가능한가?
	BOOL	CheckCanEnterRaklionBattle();			// 칸투르 보스전 전투에 입장할 수 있는 상태인가?

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
