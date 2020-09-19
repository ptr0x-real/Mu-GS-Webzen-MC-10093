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

// 칸투르 메인 스케줄 및 데이터 관리를 담당한다.
class CKanturu  
{
private:
	// KANTURU INFORMATION	
	BOOL						m_bKanturuEnable;					// 칸투르 보스전을 실행하는가?
	int							m_iKanturuState;					// 칸투르 현재 상태
	BOOL						m_bIsSucccess;						// 성공, 실패
	
	// KANTURU DATA FILE
	BOOL						m_bFileDataLoad;					// 데이터 파일 로드 유/무( Kanturu.dat )
	CKanturuStateInfo			m_StateInfo[MAX_KANTURU_STATE];		// 칸투르 각 상태의 종료조건에 대한 정보
	int							m_StateInfoCount;					// 파일에 저장된 칸투르 각 상태의 수

	// DETAIL STATE
	CKanturuBattleStanby		m_BattleStanby;						// 칸투르 보스전 대기상태
	CKanturuBattleOfMaya		m_BattleOfMaya;						// 칸투르 보스전 마야전
	CKanturuBattleOfNightmare	m_BattleOfNightmare;				// 칸투르 보스전 나이트메어전
	CKanturuTowerOfRefinement	m_TowerOfRefinement;				// 칸투르 보스전 정제의 탑
	
	// MAP
	MapClass					m_KanturuMap[MAX_KANTURU_TOWEROFREFINEMENT_MAP];	// 정제의 탑 열림/ 닫힘에 대한 맵 정보

	// MOONSTONE CHECK
	BOOL						m_bEnableCheckMoonStone;			// 입장시 문스톤펜던트 체크 여부->운영자로
	
#ifdef KANTURU_TIMEATTACK_EVENT_20060822	// 칸투르 보스전 카운터 변수 선언
	// EVENT
	int							m_iKanturuBattleCounter;	// 하나의 보스전이 시작되면 +1. 하루 단위로 초기화한다.
	int							m_iKanturuBattleDate;		// 하루단위로 초기화하기 위해 날짜 정보를 저장 해 둔다.
#endif
	
public:
	CKanturu();
	virtual ~CKanturu();

	void	Init();
	void	ResetAllData();

	BOOL	LoadData( LPSTR lpszFileName );								// 칸투르 보스전에 관한 데이터를 로드한다.
	void	LoadKanturuMapAttr( LPSTR lpszFileName, BYTE btLevel );		// 맵 데이터를 로드한다.
	void	SetKanturuMapAttr( BYTE btLevel );							// 정제의 탑 열림/닫힘에 따라 맵 속성 정보를 바꾼다.
	
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
	void	CheckUserOnKanturuBossMap();			// 비정상적인 방법으로 맵에 있는 유저는 쫒아낸다.
	int		CheckEnterKanturu( int iUserIndex );	// 칸투르에 입장 가능한가?
	BOOL	CheckCanEnterKanturuBattle();			// 칸투르 보스전 전투에 입장할 수 있는 상태인가?

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
