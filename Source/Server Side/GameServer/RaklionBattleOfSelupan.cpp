//----------------------------------------------------------------------------------
// FileName				: RaklionBattleOfSelupan.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ������ ���� ó��
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

// ����� �����͸� �ʱ�ȭ �Ѵ�.
void CRaklionBattleOfSelupan::ResetAllData()
{
	m_bFileDataLoad				= FALSE;
	m_iSelupanSkillDelay		= 0;
	m_iSelupanSkillStartTime	= 0;
	m_bIsRunFirstSkill			= FALSE;

	ZeroMemory( m_SelupanPattern, sizeof(RAKLION_SELUPAN_SKILL)*RAKLION_SELUPAN_PATTERN_MAX );

	for( INT iCount=0; iCount<RAKLION_SELUPAN_PATTERN_MAX; ++iCount )
	{
		// �Ʒ� 4���� ��ų�� �׻� ���Ͽ� ���Եȴ�.
		m_SelupanPattern[iCount][RAKLION_SELUPAN_SKILL_POISON_ATTACK] = TRUE;
		m_SelupanPattern[iCount][RAKLION_SELUPAN_SKILL_ICE_STORM] = TRUE;
		m_SelupanPattern[iCount][RAKLION_SELUPAN_SKILL_ICE_STRIKE] = TRUE;
		m_SelupanPattern[iCount][RAKLION_SELUPAN_SKILL_TELEPORT_RANDOM] = TRUE;
	}

	// PATTERN_1
	// �⺻���� 3���� �����̵��� ����Ѵ�. ���� ���� ������ �ʿ� ����.

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
	// ������ ���� ó��
	//g_RaklionBattleUserMng.CheckUserState();
#else
	// ������ ���� ó��
	g_RaklionBattleUserMng.CheckUserState();
#endif // MODIFY_RAKLION_BATTLE_USER_DELETE_BUGFIX_20080916

	// �������� HP�� 0�̸� DIE ���·� �ٲ��ְ� return �Ѵ�.
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

	// ��ų ���� ������ ������ �ٷ� return �Ѵ�.
	if ( m_iSelupanSkillDelay > GetTickCount() - m_iSelupanSkillStartTime )
		return;
	
	// ó��
	switch( m_iBattleOfSelupanState )
	{
	case BATTLE_OF_SELUPAN_NONE:				// �������� ���°� �ƴ�(������ ���� ����)
		ProcState_NONE();
		break;
	case BATTLE_OF_SELUPAN_STANDBY:				// �������� �ʱ�ȭ �ܰ�
		ProcState_STANDBY();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_1:			// ������ ���� 1�ܰ�
		ProcState_PATTERN_1();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_2:			// ������ ���� 2�ܰ�
		ProcState_PATTERN_2();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_3:			// ������ ���� 3�ܰ�
		ProcState_PATTERN_3();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_4:			// ������ ���� 4�ܰ�
		ProcState_PATTERN_4();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_5:			// ������ ���� 5�ܰ�
		ProcState_PATTERN_5();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_6:			// ������ ���� 6�ܰ�
		ProcState_PATTERN_6();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_7:			// ������ ���� 7�ܰ�
		ProcState_PATTERN_7();
		break;
	case BATTLE_OF_SELUPAN_DIE:					// �������� �׾���
		ProcState_DIE();
		break;
	}
}

//---------------------------------------------------------------------------------------------
void CRaklionBattleOfSelupan::SetState( int iBattleOfSelupanState )
{
	// ��ų ���� ������ ������ �ٷ� return �Ѵ�.
	if ( m_iSelupanSkillDelay > GetTickCount() - m_iSelupanSkillStartTime )
		return;

	// ���¸� �����Ѵ�.
	switch( iBattleOfSelupanState )
	{
	case BATTLE_OF_SELUPAN_NONE:				// �������� ���°� �ƴ�(������ ���� ����)
		SetState_NONE();
		break;
	case BATTLE_OF_SELUPAN_STANDBY:				// �������� �ʱ�ȭ �ܰ�
		SetState_STANDBY();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_1:			// ������ ���� 1�ܰ�
		SetState_PATTERN_1();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_2:			// ������ ���� 2�ܰ�
		SetState_PATTERN_2();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_3:			// ������ ���� 3�ܰ�
		SetState_PATTERN_3();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_4:			// ������ ���� 4�ܰ�
		SetState_PATTERN_4();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_5:			// ������ ���� 5�ܰ�
		SetState_PATTERN_5();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_6:			// ������ ���� 6�ܰ�
		SetState_PATTERN_6();
		break;
	case BATTLE_OF_SELUPAN_PATTERN_7:			// ������ ���� 7�ܰ�
		SetState_PATTERN_7();
		break;
	case BATTLE_OF_SELUPAN_DIE:					// �������� �׾���
		SetState_DIE();
		break;
	}

	// �ٲ� ������ ���¸� ������.
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
		
	// ������ ����ȭ ����
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();

	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_NONE );
}

void CRaklionBattleOfSelupan::SetState_STANDBY()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> STANDBY", m_iBattleOfSelupanState );
	
	// �������� �ε����� �����Ѵ�.
	m_RaklionSelupan.InitSelupanIndex();

	// �������� �����Ѵ�.
	m_RaklionSelupan.RegenSelupan();

	// ������ ����ȭ ����
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();

	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_STANDBY );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_1()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_1 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// ������ ����ȭ ����
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();
	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_1 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_2()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_2 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// ����ȭ 1�ܰ� ���
	m_RaklionSelupan.RaklionSelupanAct_Berserk1();
	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_2 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_3()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_3 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );
	
	// ����ȭ 2�ܰ� ���
	m_RaklionSelupan.RaklionSelupanAct_Berserk2();

	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_3 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_4()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_4 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// ����ȭ 2�ܰ谡 �ƴϸ� 2�ܰ�� ����
	if( m_RaklionSelupan.GetBerserkLevel() != 2 )
		m_RaklionSelupan.RaklionSelupanAct_Berserk2();

	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_4 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_5()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_5 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// ����ȭ 3�ܰ� ���
	m_RaklionSelupan.RaklionSelupanAct_Berserk3();

	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_5 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_6()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_6 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// ����ȭ 4�ܰ� ���
	m_RaklionSelupan.RaklionSelupanAct_Berserk4();
	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_6 );
}

void CRaklionBattleOfSelupan::SetState_PATTERN_7()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> PATTERN_7 // Life(%d/4800000)",
		m_iBattleOfSelupanState, m_RaklionSelupan.GetSelupanHP() );

	// ����ȭ�� 4�ܰ谡 �ƴϸ� 4�ܰ�� ����
	if( m_RaklionSelupan.GetBerserkLevel() != 4 )
		m_RaklionSelupan.RaklionSelupanAct_Berserk4();
	
	SetBattleOfSelupanState( BATTLE_OF_SELUPAN_PATTERN_7 );
}

void CRaklionBattleOfSelupan::SetState_DIE()
{
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ][ BattleOfSelupan ] State(%d) -> DIE", m_iBattleOfSelupanState );

	// ������ ����ȭ ����
	m_RaklionSelupan.RaklionSelupanAct_Berserk_Cancel();

	// �������� �׾����Ƿ� ��ȯ���͸� �����ش�.
	DeleteSummonMonster();

	INT iWinner = m_RaklionSelupan.GetTargetOfSelupan();
	LPOBJECTSTRUCT lpObj = &gObj[iWinner];

	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3412), lpObj->Name );
	
#ifdef UPDATE_SEASON_4_EVENT_LOG_200800715		// �̺�Ʈ�� �α� �߰�
	time_t t;
	tm *pCurrentTime;
	time( &t );
	pCurrentTime = localtime( &t );
	
	for( INT iCount=0; iCount<g_RaklionBattleUserMng.GetUserCount(); ++iCount )
	{
		LogAdd( "[Raklion][SelupanDie] %d/%d/%d %d:%d:%d %dth Winner : %s (%s)",
			pCurrentTime->tm_year+1900, pCurrentTime->tm_mon+1, pCurrentTime->tm_mday,	// ��¥
			pCurrentTime->tm_hour, pCurrentTime->tm_min, pCurrentTime->tm_sec,			// �ð�
			iCount+1,																	// ����ī��Ʈ
			gObj[g_RaklionBattleUserMng.GetUserObjIndex(iCount)].Name,					// ĳ���͸�
			gObj[g_RaklionBattleUserMng.GetUserObjIndex(iCount)].AccountID );			// ���� ���̵�
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
	
	// �ڷ���Ʈ ���̰ų� �̵����̸� ��ų�� ������� �ʴ´�.
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
		// �߸��� �׼� ��ȣ�� ���Դ�. ���� ó��!
		return;
	}

	switch( iAction )
	{
	case RAKLION_SELUPAN_SKILL_FIRST_SKILL:		// ���� ��ų
		m_RaklionSelupan.RaklionSelupanAct_FirstSkill();
		break;
	case RAKLION_SELUPAN_SKILL_POISON_ATTACK:	// �⺻ ����(��)
		m_RaklionSelupan.RaklionSelupanAct_PoisonAttack();
		break;
	case RAKLION_SELUPAN_SKILL_ICE_STORM:		// ���̽� ����
		m_RaklionSelupan.RaklionSelupanAct_IceStorm();
		break;
	case RAKLION_SELUPAN_SKILL_ICE_STRIKE:		// ���̽� ��Ʈ����ũ
		m_RaklionSelupan.RaklionSelupanAct_IceStrike();
		break;
	case RAKLION_SELUPAN_SKILL_SUMMON_MONSTER:	// ���� ��ȯ
		m_RaklionSelupan.RaklionSelupanAct_SummonMonster();
		break;
	case RAKLION_SELUPAN_SKILL_HEAL:			// �ڱ� �ڽ� ġ��
		m_RaklionSelupan.RaklionSelupanAct_Heal();
		break;
	case RAKLION_SELUPAN_SKILL_FREEZE:			// �󸮱�
		m_RaklionSelupan.RaklionSelupanAct_Freeze();
		break;
	case RAKLION_SELUPAN_SKILL_TELEPORT_RANDOM:	// ���� ���� �̵�
		m_RaklionSelupan.RaklionSelupanAct_Teleport();
		break;
	case RAKLION_SELUPAN_SKILL_INVINCIBILITY:	// ���� ����
		m_RaklionSelupan.RaklionSelupanAct_Invincibility();
		break;
	}

	// �������� ��ų ������ ����
	m_iSelupanSkillStartTime = GetTickCount();
	m_iSelupanSkillDelay = m_RaklionSelupan.GetSelupanSkillDelay();
}

//-----------------------------------------------------------------------------------
// �������� ���� ���� ������ �����Ѵ�.
void CRaklionBattleOfSelupan::SetPatternCondition( INT iPatternNum, INT iCondition )
{
	if( iPatternNum<0 || iCondition<0 )
	{
		LogAddC( LOGC_RED, "[RAKLION][SetPatternCondition] error : PatternNum(%d), Condition(%d)",
			iPatternNum, iCondition );
	}

	m_iSelupanPatternCondition[iPatternNum] = iCondition;
}

// �������� ���� HP�� ���� ���°� �����ȴ�.
void CRaklionBattleOfSelupan::SetBattleOfSelupanStateAuto()
{
	INT iSelupanObjIndex = 0;
	INT iSelupanMAXLife = 0;
	INT iSelupanLifePercent = 0;
	LPOBJECTSTRUCT lpObj = NULL;

	iSelupanObjIndex = m_RaklionSelupan.GetSelupanObjIndex();
	lpObj = &gObj[iSelupanObjIndex];

	// ������ �Ϸ���� �ʾҴ�.
	if( lpObj->DieRegen )
		return;

	iSelupanMAXLife = lpObj->MaxLife + lpObj->AddLife;

	if( iSelupanMAXLife <= 0 )
	{
		// �������� HP �ִ밪�� �߸��Ǿ���.
		LogAddC( LOGC_RED, "[RAKLION] SelupanMAXLife error : Index(%d), MaxLife(%d)",
			lpObj->m_Index, lpObj->MaxLife );
		return;
	}
	
	// ���� ������ HP�� %���� �����Ѵ�.
	iSelupanLifePercent = (DWORD)(m_RaklionSelupan.GetSelupanHP() * 100 / iSelupanMAXLife );
	
	if( iSelupanLifePercent < 0 )
	{
		// �������� HP %���� �߸��Ǿ���
		LogAddC( LOGC_RED, "[RAKLION] SelupanLifePercent error : Index(%d), MaxLife(%d)",
			lpObj->m_Index, iSelupanLifePercent );
		return;
	}

	// ù��° ��ų�� ������� �ʾҴٸ� ù��° ��ų ����� return
	if( m_bIsRunFirstSkill == FALSE )
	{
		ActionOfSelupan( RAKLION_SELUPAN_SKILL_FIRST_SKILL );
		m_bIsRunFirstSkill = TRUE;
		return;
	}

	// �������� ���� ������� %�� ���� ������ �����Ѵ�.
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
// �������� �������� �������� ���´�.
INT CRaklionBattleOfSelupan::GetSelupanBattleUserCount()
{
	return g_RaklionBattleUserMng.GetUserCount();
}

// �������� �����Ѵ�.
void CRaklionBattleOfSelupan::RegenSelupan()
{
	m_RaklionSelupan.RegenSelupan();
}

// �������� ���ش�.
void CRaklionBattleOfSelupan::HideSelupan()
{
	m_RaklionSelupan.HideSelupan();
}

// �������� ObjIndex�� ���´�.
INT CRaklionBattleOfSelupan::GetSelupanObjIndex()
{
	INT iSelupanIndex = m_RaklionSelupan.GetSelupanObjIndex();

	return iSelupanIndex;
}

// ��ȯ�� ���͸� ���ش�.
void CRaklionBattleOfSelupan::DeleteSummonMonster()
{
	m_RaklionSelupan.DeleteSummonMonster();
}

// �������� ��ų �����̸� �����Ѵ�.
void CRaklionBattleOfSelupan::SetSelupanSkillDelay( INT iDelay )
{
	m_RaklionSelupan.SetSelupanSkillDelay( iDelay );
}

//-----------------------------------------------------------------------------------
// �������� ���� ���� GM ��ɾ ó���Ѵ�.
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