//----------------------------------------------------------------------------------
// FileName				: RaklionBattleOfSelupan.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 보스전 상태 처리
//----------------------------------------------------------------------------------
// RaklionBattleOfSelupan.cpp: implementation of the CRaklionBattleOfSelupan class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_RAKLION_20080408

#include "RaklionBattleOfSelupan.h"

#include "RaklionBattleUserMng.h"
#include "RaklionUtil.h"
#include "RaklionDefine.h"

//#include "Include\ReadScript.h"

static CRaklionUtil	g_RaklionUtil;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionBattleOfSelupan::CRaklionBattleOfSelupan()
{		
	ResetAllData();
}

CRaklionBattleOfSelupan::~CRaklionBattleOfSelupan()
{
}

// 사용할 데이터를 초기화 한다.
void CRaklionBattleOfSelupan::ResetAllData()
{
	m_bFileDataLoad				= FALSE;
	m_iSelupanSkillDelay		= 0;
	m_iSelupanSkillStartTime	= 0;
	m_bIsRunFirstSkill			= FALSE;

	ZeroMemory( m_SelupanPattern, sizeof(RAKLION_SELUPAN_SKILL)*RAKLION_SELUPAN_PATTERN_MAX );

	for( INT iCount=0; iCount<RAKLION_SELUPAN_PATTERN_MAX; ++iCount )
	{
		// 아래 4가지 스킬은 항상 패턴에 포함된다.
		m_SelupanPattern[iCount][RAKLION_SELUPAN_SKILL_POISON_ATTACK] = TRUE;
		m_SelupanPattern[iCount][RAKLION_SELUPAN_SKILL_ICE_STORM] = TRUE;
		m_SelupanPattern[iCount][RAKLION_SELUPAN_SKILL_ICE_STRIKE] = TRUE;
		m_SelupanPattern[iCount][RAKLION_SELUPAN_SKILL_TELEPORT_RANDOM] = TRUE;
	}

	// PATTERN_1
	// 기본공격 3종과 순간이동만 사용한다. 따라서 따로 세팅이 필요 없다.

	// PATTERN_2
	m_SelupanPattern[1][RAKLION_SELUPAN_SKILL_FREEZE] = TRUE;

	// PATTERN_3
	m_SelupanPattern[2][RAKLION_SELUPAN_SKILL_FREEZE] = TRUE;

	// PATTERN_4
	m_SelupanPattern[3][RAKLION_SELUPAN_SKILL_FREEZE] = TRUE;
	m_SelupanPattern[3][RAKLION_SELUPAN_SKILL_HEAL] = TRUE;

	// PATTERN_5
	m_SelupanPattern[4][RAKLION_SELUPAN_SKILL_SUMMON_MONSTER] = TRUE;
	m_SelupanPattern[4][RAKLION_SELUPAN_SKILL_FREEZE] = TRUE;
	m_SelupanPattern[4][RAKLION_SELUPAN_SKILL_HEAL] = TRUE;

	// PATTERN_6
	m_SelupanPattern[5][RAKLION_SELUPAN_SKILL_SUMMON_MONSTER] = TRUE;
	m_SelupanPattern[5][RAKLION_SELUPAN_SKILL_FREEZE] = TRUE;
	m_SelupanPattern[5][RAKLION_SELUPAN_SKILL_HEAL] = TRUE;

	// PATTERN_7
	m_SelupanPattern[6][RAKLION_SELUPAN_SKILL_SUMMON_MONSTER] = TRUE;
	m_SelupanPattern[6][RAKLION_SELUPAN_SKILL_FREEZE] = TRUE;
	m_SelupanPattern[6][RAKLION_SELUPAN_SKILL_HEAL] = TRUE;
	m_SelupanPattern[6][RAKLION_SELUPAN_SKILL_INVINCIBILITY] = TRUE;

	SetSuccessValue( FALSE );

	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_NONE );
}

BOOL CRaklionBattleOfSelupan::LoadData( LPSTR lpszFileName )
{
	return TRUE;
}

//---------------------------------------------------------------------------------------------
void CRaklionBattleOfSelupan::Run()
{
#ifdef MODIFY_RAKLION_BATTLE_USER_DELETE_BUGFIX_20080916
	// 유저의 상태 처리
	//g_RaklionBattleUserMng.CheckUserState();
#else
	// 유저의 상태 처리
	g_RaklionBattleUserMng.CheckUserState();
#endif // MODIFY_RAKLION_BATTLE_USER_DELETE_BUGFIX_20080916

	// 세루판의 HP가 0이면 DIE 상태로 바꿔주고 return 한다.
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	if( m_RaklionSelupan.GetSelupanHP() <= 0 && GetBattleOfSelupanState() != BATTLE_OF_SELUPAN_DIE )
	{
		SetState( BATTLE_OF_SELUPAN_DIE );
		return;
	}
#else
	if( m_RaklionSelupan.GetSelupanHP() <= 0 )
	{
		SetState( BATTLE_OF_SELUPAN_DIE );
		return;
	}
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

	// 스킬 시전 딜레이 동안은 바로 return 한다.
	if ( m_iSelupanSkillDelay > GetTickCount() - m_iSelupanSkillStartTime )
		return;
	
	// 처리
	switch( m_iBattleOfSelupanState )
	{
	case BATTLE_OF_SELUPAN_NONE:				// 세루판전 상태가 아님(보스전 시작 안함)
		ProcState_NONE();
		break;
	case BATTLE_OF_SELUPAN_STANDBY:				// 세루판전 초기화 단계
		ProcState_STANDBY();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_1:			// 세루판 패턴 1단계
		ProcState_PATTERN_1();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_2:			// 세루판 패턴 2단계
		ProcState_PATTERN_2();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_3:			// 세루판 패턴 3단계
		ProcState_PATTERN_3();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_4:			// 세루판 패턴 4단계
		ProcState_PATTERN_4();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_5:			// 세루판 패턴 5단계
		ProcState_PATTERN_5();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_6:			// 세루판 패턴 6단계
		ProcState_PATTERN_6();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_7:			// 세루판 패턴 7단계
		ProcState_PATTERN_7();
		break;
	case BATTLE_OF_SELUPAN_DIE:					// 세루판이 죽었다
		ProcState_DIE();
		break;
	}
}

//---------------------------------------------------------------------------------------------
void CRaklionBattleOfSelupan::SetState( int iBattleOfSelupanState )
{
	// 스킬 시전 딜레이 동안은 바로 return 한다.
	if ( m_iSelupanSkillDelay > GetTickCount() - m_iSelupanSkillStartTime )
		return;

	// 상태를 변경한다.
	switch( iBattleOfSelupanState )
	{
	case BATTLE_OF_SELUPAN_NONE:				// 세루판전 상태가 아님(보스전 시작 안함)
		SetState_NONE();
		break;
	case BATTLE_OF_SELUPAN_STANDBY:				// 세루판전 초기화 단계
		SetState_STANDBY();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_1:			// 세루판 패턴 1단계
		SetState_PATTERN_1();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_2:			// 세루판 패턴 2단계
		SetState_PATTERN_2();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_3:			// 세루판 패턴 3단계
		SetState_PATTERN_3();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_4:			// 세루판 패턴 4단계
		SetState_PATTERN_4();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_5:			// 세루판 패턴 5단계
		SetState_PATTERN_5();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_6:			// 세루판 패턴 6단계
		SetState_PATTERN_6();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_7:			// 세루판 패턴 7단계
		SetState_PATTERN_7();
		break;
	case BATTLE_OF_SELUPAN_DIE:					// 세루판이 죽었다
		SetState_DIE();
		break;
	}

	// 바뀐 보스전 상태를 보낸다.
	g_RaklionUtil.NotifyRaklionChangeState( RAKLION_STATE_DETAIL_STATE, GetBattleOfSelupanState() );
}

//---------------------------------------------------------------------------------------------
void CRaklionBattleOfSelupan::SetState_NONE()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> NONE", m_iBattleOfSelupanState);

#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
//	INT iSelupanindex = m_RaklionSelupan.GetSelupanObjIndex();
//	LPOBJECTSTRUCT lpObj = NULL;
//	
//	if( iSelupanindex == _INVALID )
//	{
//		LogAddC( LOGC_RED, "[RAKLION][SetState] error SelupanObjIndex(%d)", iSelupanindex );
//		return;
//	}
#else
	INT iSelupanindex = m_RaklionSelupan.GetSelupanObjIndex();
	LPOBJECTSTRUCT lpObj = NULL;
	
	if( iSelupanindex == _INVALID )
	{
		LogAddC( LOGC_RED, "[RAKLION][SetState] error SelupanObjIndex(%d)", iSelupanindex );
		return;
	}
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
		
	// 세루판 광폭화 해제
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();

	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_NONE );
}

void CRaklionBattleOfSelupan::SetState_STANDBY()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> STANDBY", m_iBattleOfSelupanState );
	
	// 세루판의 인덱스를 세팅한다.
	m_RaklionSelupan.InitSelupanIndex();

	// 세루판을 생성한다.
	m_RaklionSelupan.RegenSelupan();

	// 세루판 광폭화 해제
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();

	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_STANDBY );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_1()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_1 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// 세루판 광폭화 해제
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();
	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_1 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_2()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_2 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// 광폭화 1단계 사용
	m_RaklionSelupan.RaklionSelupanAct_Berserk1();
	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_2 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_3()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_3 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );
	
	// 광폭화 2단계 사용
	m_RaklionSelupan.RaklionSelupanAct_Berserk2();

	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_3 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_4()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_4 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// 광폭화 2단계가 아니면 2단계로 세팅
	if( m_RaklionSelupan.GetBerserkLevel() != 2 )
		m_RaklionSelupan.RaklionSelupanAct_Berserk2();

	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_4 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_5()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_5 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// 광폭화 3단계 사용
	m_RaklionSelupan.RaklionSelupanAct_Berserk3();

	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_5 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_6()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_6 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// 광폭화 4단계 사용
	m_RaklionSelupan.RaklionSelupanAct_Berserk4();
	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_6 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_7()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_7 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// 광폭화가 4단계가 아니면 4단계로 적용
	if( m_RaklionSelupan.GetBerserkLevel() != 4 )
		m_RaklionSelupan.RaklionSelupanAct_Berserk4();
	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_7 );
}

void CRaklionBattleOfSelupan::SetState_DIE()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> DIE", m_iBattleOfSelupanState );

	// 세루판 광폭화 해제
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();

	// 세루판이 죽었으므로 소환몬스터를 지워준다.
	DeleteSummonMonster();

	INT iWinner = m_RaklionSelupan.GetTargetOfSelupan();
	LPOBJECTSTRUCT lpObj = &gObj[iWinner];

	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3412), lpObj->Name );
	
#ifdef UPDATE_SEASON_4_EVENT_LOG_200800715		// 이벤트용 로그 추가
	time_t t;
	tm *pCurrentTime;
	time( &t );
	pCurrentTime = localtime( &t );
	
	for( INT iCount=0; iCount<g_RaklionBattleUserMng.GetUserCount(); ++iCount )
	{
		LogAdd( "[Raklion][SelupanDie] %d/%d/%d %d:%d:%d %dth Winner : %s (%s)",
			pCurrentTime->tm_year+1900, pCurrentTime->tm_mon+1, pCurrentTime->tm_mday,	// 날짜
			pCurrentTime->tm_hour, pCurrentTime->tm_min, pCurrentTime->tm_sec,			// 시간
			iCount+1,																	// 유저카운트
			gObj[g_RaklionBattleUserMng.GetUserObjIndex(iCount)].Name,					// 캐릭터명
			gObj[g_RaklionBattleUserMng.GetUserObjIndex(iCount)].AccountID );			// 유저 아이디
	}
#endif // UPDATE_SEASON_4_EVENT_LOG_200800715
	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_DIE );
}

//---------------------------------------------------------------------------------------------
void CRaklionBattleOfSelupan::ProcState_NONE()
{

}

void CRaklionBattleOfSelupan::ProcState_STANDBY()
{
	
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_1()
{
	INT iAction = 0;
	while( m_SelupanPattern[0][iAction] == FALSE )
		iAction = rand()%RAKLION_SELUPAN_SKILL_MAX;

	ActionOfSelupan( iAction );
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_2()
{
	INT iAction = 0;
	while( m_SelupanPattern[1][iAction] == FALSE )
		iAction = rand()%RAKLION_SELUPAN_SKILL_MAX;
	
	ActionOfSelupan( iAction );
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_3()
{
	INT iAction = 0;
	while( m_SelupanPattern[2][iAction] == FALSE )
		iAction = rand()%RAKLION_SELUPAN_SKILL_MAX;
	
	ActionOfSelupan( iAction );
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_4()
{
	INT iAction = 0;
	while( m_SelupanPattern[3][iAction] == FALSE )
		iAction = rand()%RAKLION_SELUPAN_SKILL_MAX;
	
	ActionOfSelupan( iAction );
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_5()
{
	INT iAction = 0;
	while( m_SelupanPattern[4][iAction] == FALSE )
		iAction = rand()%RAKLION_SELUPAN_SKILL_MAX;
	
	ActionOfSelupan( iAction );
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_6()
{
	INT iAction = 0;
	while( m_SelupanPattern[5][iAction] == FALSE )
		iAction = rand()%RAKLION_SELUPAN_SKILL_MAX;
	
	ActionOfSelupan( iAction );
}

void CRaklionBattleOfSelupan::ProcState_PATTERN_7()
{
	INT iAction = 0;
	while( m_SelupanPattern[6][iAction] == FALSE )
		iAction = rand()%RAKLION_SELUPAN_SKILL_MAX;
	
	ActionOfSelupan( iAction );
}

void CRaklionBattleOfSelupan::ProcState_DIE()
{
	
}

//-----------------------------------------------------------------------------------
void CRaklionBattleOfSelupan::ActionOfSelupan( INT iAction )
{
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	INT iSelupanIndex = m_RaklionSelupan.GetSelupanObjIndex();
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
	// 텔레포트 중이거나 이동중이면 스킬을 사용하지 않는다.
#ifdef MODIFY_RAKLION_SELUPAN_MOVING_USE_SKILL_BUGFIX_20080813
	if( lpObj->Teleport || lpObj->m_ActState.Move )
		return;	
#else
	if( lpObj->Teleport )
		return;
#endif // MODIFY_RAKLION_SELUPAN_MOVING_USE_SKILL_BUGFIX_20080813
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

	if( iAction<0 || iAction>=RAKLION_SELUPAN_SKILL_MAX )
	{
		// 잘못된 액션 번호가 들어왔다. 에러 처리!
		return;
	}

	switch( iAction )
	{
	case RAKLION_SELUPAN_SKILL_FIRST_SKILL:		// 최초 스킬
		m_RaklionSelupan.RaklionSelupanAct_FirstSkill();
		break;
	case RAKLION_SELUPAN_SKILL_POISON_ATTACK:	// 기본 공격(독)
		m_RaklionSelupan.RaklionSelupanAct_PoisonAttack();
		break;
	case RAKLION_SELUPAN_SKILL_ICE_STORM:		// 아이스 스톰
		m_RaklionSelupan.RaklionSelupanAct_IceStorm();
		break;
	case RAKLION_SELUPAN_SKILL_ICE_STRIKE:		// 아이스 스트라이크
		m_RaklionSelupan.RaklionSelupanAct_IceStrike();
		break;
	case RAKLION_SELUPAN_SKILL_SUMMON_MONSTER:	// 몬스터 소환
		m_RaklionSelupan.RaklionSelupanAct_SummonMonster();
		break;
	case RAKLION_SELUPAN_SKILL_HEAL:			// 자기 자신 치료
		m_RaklionSelupan.RaklionSelupanAct_Heal();
		break;
	case RAKLION_SELUPAN_SKILL_FREEZE:			// 얼리기
		m_RaklionSelupan.RaklionSelupanAct_Freeze();
		break;
	case RAKLION_SELUPAN_SKILL_TELEPORT_RANDOM:	// 랜덤 순간 이동
		m_RaklionSelupan.RaklionSelupanAct_Teleport();
		break;
	case RAKLION_SELUPAN_SKILL_INVINCIBILITY:	// 순간 무적
		m_RaklionSelupan.RaklionSelupanAct_Invincibility();
		break;
	}

	// 세루판의 스킬 딜레이 세팅
	m_iSelupanSkillStartTime = GetTickCount();
	m_iSelupanSkillDelay = m_RaklionSelupan.GetSelupanSkillDelay();
}

//-----------------------------------------------------------------------------------
// 세루판의 패턴 변이 조건을 설정한다.
void CRaklionBattleOfSelupan::SetPatternCondition( INT iPatternNum, INT iCondition )
{
	if( iPatternNum<0 || iCondition<0 )
	{
		LogAddC( LOGC_RED, "[RAKLION][SetPatternCondition] error : PatternNum(%d), Condition(%d)",
			iPatternNum, iCondition );
	}

	m_iSelupanPatternCondition[iPatternNum] = iCondition;
}

// 세루판의 남의 HP에 따라 상태가 설정된다.
void CRaklionBattleOfSelupan::SetBattleOfSelupanStateAuto()
{
	INT iSelupanObjIndex = 0;
	INT iSelupanMAXLife = 0;
	INT iSelupanLifePercent = 0;
	LPOBJECTSTRUCT lpObj = NULL;

	iSelupanObjIndex = m_RaklionSelupan.GetSelupanObjIndex();
	lpObj = &gObj[iSelupanObjIndex];

	// 리젠이 완료되지 않았다.
	if( lpObj->DieRegen )
		return;

	iSelupanMAXLife = lpObj->MaxLife + lpObj->AddLife;

	if( iSelupanMAXLife <= 0 )
	{
		// 세루판의 HP 최대값이 잘못되었다.
		LogAddC( LOGC_RED, "[RAKLION] SelupanMAXLife error : Index(%d), MaxLife(%d)",
			lpObj->m_Index, lpObj->MaxLife );
		return;
	}
	
	// 현재 세루판 HP의 %값을 산출한다.
	iSelupanLifePercent = (DWORD)(m_RaklionSelupan.GetSelupanHP() * 100 / iSelupanMAXLife );
	
	if( iSelupanLifePercent < 0 )
	{
		// 세루판의 HP %값이 잘못되었다
		LogAddC( LOGC_RED, "[RAKLION] SelupanLifePercent error : Index(%d), MaxLife(%d)",
			lpObj->m_Index, iSelupanLifePercent );
		return;
	}

	// 첫번째 스킬을 사용하지 않았다면 첫번째 스킬 사용후 return
	if( m_bIsRunFirstSkill == FALSE )
	{
		ActionOfSelupan( RAKLION_SELUPAN_SKILL_FIRST_SKILL );
		m_bIsRunFirstSkill = TRUE;
		return;
	}

	// 세루판의 현재 생명력의 %에 따라 패턴을 설정한다.
	if( iSelupanLifePercent > m_iSelupanPatternCondition[0] )
	{
		if( GetBattleOfSelupanState() != BATTLE_OF_SELUPAN_PATTERN_1 )
			SetState( BATTLE_OF_SELUPAN_PATTERN_1 );
	}
	else if( iSelupanLifePercent > m_iSelupanPatternCondition[1] )
	{
		if( GetBattleOfSelupanState() != BATTLE_OF_SELUPAN_PATTERN_2 )
			SetState( BATTLE_OF_SELUPAN_PATTERN_2 );
	}
	else if( iSelupanLifePercent > m_iSelupanPatternCondition[2] )
	{
		if( GetBattleOfSelupanState() != BATTLE_OF_SELUPAN_PATTERN_3 )
			SetState( BATTLE_OF_SELUPAN_PATTERN_3 );
	}
	else if( iSelupanLifePercent > m_iSelupanPatternCondition[3] )
	{
		if( GetBattleOfSelupanState() != BATTLE_OF_SELUPAN_PATTERN_4 )
			SetState( BATTLE_OF_SELUPAN_PATTERN_4 );
	}
	else if( iSelupanLifePercent > m_iSelupanPatternCondition[4] )
	{
		if( GetBattleOfSelupanState() != BATTLE_OF_SELUPAN_PATTERN_5 )
			SetState( BATTLE_OF_SELUPAN_PATTERN_5 );
	}
	else if( iSelupanLifePercent > m_iSelupanPatternCondition[5] )
	{
		if( GetBattleOfSelupanState() != BATTLE_OF_SELUPAN_PATTERN_6 )
			SetState( BATTLE_OF_SELUPAN_PATTERN_6 );
	}
	else if( iSelupanLifePercent <= m_iSelupanPatternCondition[5] )
	{
		if( GetBattleOfSelupanState() != BATTLE_OF_SELUPAN_PATTERN_7 )
			SetState( BATTLE_OF_SELUPAN_PATTERN_7 );
	}
}

//-----------------------------------------------------------------------------------
// 보스전에 참가중인 유저수를 얻어온다.
INT CRaklionBattleOfSelupan::GetSelupanBattleUserCount()
{
	return g_RaklionBattleUserMng.GetUserCount();
}

// 세루판을 리젠한다.
void CRaklionBattleOfSelupan::RegenSelupan()
{
	m_RaklionSelupan.RegenSelupan();
}

// 세루판을 없앤다.
void CRaklionBattleOfSelupan::HideSelupan()
{
	m_RaklionSelupan.HideSelupan();
}

// 세루판의 ObjIndex를 얻어온다.
INT CRaklionBattleOfSelupan::GetSelupanObjIndex()
{
	INT iSelupanIndex = m_RaklionSelupan.GetSelupanObjIndex();

	return iSelupanIndex;
}

// 소환된 몬스터를 없앤다.
void CRaklionBattleOfSelupan::DeleteSummonMonster()
{
	m_RaklionSelupan.DeleteSummonMonster();
}

// 세루판의 스킬 딜레이를 설정한다.
void CRaklionBattleOfSelupan::SetSelupanSkillDelay( INT iDelay )
{
	m_RaklionSelupan.SetSelupanSkillDelay( iDelay );
}

//-----------------------------------------------------------------------------------
// 세루판의 패턴 변경 GM 명령어를 처리한다.
void CRaklionBattleOfSelupan::GmCommandSelupanPattern( INT iPatternNumber )
{
	if( iPatternNumber < BATTLE_OF_SELUPAN_PATTERN_1 || iPatternNumber > BATTLE_OF_SELUPAN_PATTERN_7 )
	{
		LogAddC( LOGC_RED, "[RAKLION] [GmCommandSelupanPattern] Invalid pattern number : %d", iPatternNumber );
		return;
	}

	switch( iPatternNumber )
	{
	case BATTLE_OF_SELUPAN_PATTERN_1:
		m_RaklionSelupan.SetSelupanHP( 100 );
		break;
	case BATTLE_OF_SELUPAN_PATTERN_2:
		m_RaklionSelupan.SetSelupanHP( 80 );
		break;
	case BATTLE_OF_SELUPAN_PATTERN_3:
		m_RaklionSelupan.SetSelupanHP( 60 );
		break;
	case BATTLE_OF_SELUPAN_PATTERN_4:
		m_RaklionSelupan.SetSelupanHP( 50 );
		break;
	case BATTLE_OF_SELUPAN_PATTERN_5:
		m_RaklionSelupan.SetSelupanHP( 40 );
		break;
	case BATTLE_OF_SELUPAN_PATTERN_6:
		m_RaklionSelupan.SetSelupanHP( 20 );
		break;
	case BATTLE_OF_SELUPAN_PATTERN_7:
		m_RaklionSelupan.SetSelupanHP( 10 );
		break;
	}
}







#endif