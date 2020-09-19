//----------------------------------------------------------------------------------
// FileName				: Raklion.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ������ ���� �����ٸ�
//----------------------------------------------------------------------------------
// Raklion.cpp: implementation of the CRaklion class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_RAKLION_20080408

#include "Raklion.h"

#include "DirPath.h"
#include "..\Include\ReadScript.h"
#include "MapServerManager.h"
#include "protocol.h"
#include "user.h"
#include "gamemain.h"
#include "MonsterSetBase.h"
#include "gObjMonster.h"

#include "RaklionDefine.h"
#include "RaklionUtil.h"

extern CDirPath			gDirPath;
extern CMonsterSetBase	gMSetBase;
static CRaklionUtil		g_RaklionUtil;

CRaklion g_Raklion;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklion::CRaklion()
{
#ifdef UPDATE_SEASON_4_EVENT_LOG_200800715		// �̺�Ʈ�� �α� �߰�
	time_t t;
	tm *pCurrentTime;
	time( &t );
	pCurrentTime = localtime( &t );
	
	m_iTodayYear = pCurrentTime->tm_year;
	m_iTodayMon = pCurrentTime->tm_mon;
	m_iTodayDay = pCurrentTime->tm_mday;
	
	m_bEvent	= FALSE;	
#endif // UPDATE_SEASON_4_EVENT_LOG_200800715

	ResetAllData();
}

CRaklion::~CRaklion()
{
}

void CRaklion::Init()
{
}

// ��Ŭ������ ��� �����͸� �ʱ�ȭ�Ѵ�.
void CRaklion::ResetAllData()
{
	m_bIsBossKillSuccess		= FALSE;
	m_bFileDataLoad				= FALSE;

	m_iBossAppearanceStartTime	= 0;
	m_iBosszoneCloseStartTime	= 0;
	m_iBosszoneOpenStartTime	= 0;

	m_bIsNotify1				= FALSE;

	m_iBossEggDieCount			= 0;
}

// ��Ŭ���� �������� ���� �����͸� �ε��Ѵ�.
BOOL CRaklion::LoadData( LPSTR lpszFileName )
{
	m_bFileDataLoad = FALSE;

	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		MsgBox( "[ RAKLION ] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{		
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )	
		{
			DWORD dwError = GetLastError();
			MsgBox( "[ RAKLION ] - Can't Open %s ", lpszFileName );			
			return FALSE;
		}
		
		SMDToken Token;
		INT iType = -1;

		while( TRUE )
		{
			Token = GetToken();
			if( Token == END ) break;
			iType = ( int )TokenNumber;

			while( TRUE )
			{
				// ��Ŭ���� �����층�� ���Ǵ� ������ �� �о����(sec)
				if( iType == 0 )
				{
					// 1. >	������ �ʱ�ȭ
					INT iAppearanceDelay = 0;
					INT iBosszoneCloseDelay = 0;
					INT iBosszoneOpenDelay = 0;
				
					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iAppearanceDelay = ( int )TokenNumber;		Token = GetToken();
					iBosszoneCloseDelay = ( int )TokenNumber;	Token = GetToken();
					iBosszoneOpenDelay = ( int )TokenNumber;
						
					// 3. >	������ ��ȿ�� �˻�
					// ����� ���� �����̰� 10�ʸ� �Ѵ´ٸ� ����ó��(�ٲ� �� ����)
					if( iAppearanceDelay < 0 || iAppearanceDelay > 10 )
					{
						MsgBox( "[ RAKLION ] - Exceed AppearanceDelay : (%d)", iAppearanceDelay );
						break;
					}
					// ���� ���� �� �������� ����������� �����̰� 10���� �Ѵ´ٸ� ����ó��(�ٲ� �� ����)
					if( iBosszoneCloseDelay < 0 || iBosszoneCloseDelay >= 500 )
					{
						MsgBox( "[ RAKLION ] - Exceed BosszoneCloseDelay : (%d)", iBosszoneCloseDelay );
						break;
					}
					// ������ ���� �� �������� ����������� �����̰� 10���� �Ѵ´ٸ� ����ó��(�ٲ� �� ����)
					if( iBosszoneOpenDelay < 0 || iBosszoneOpenDelay >= 500 )
					{
						MsgBox( "[ RAKLION ] - Exceed BosszoneOpenDelay : (%d)", iBosszoneOpenDelay );
						break;
					}
					
					// 4. > ������ ����
					m_iBossAppearanceDelay = iAppearanceDelay;
					m_iBosszoneCloseDelay = iBosszoneCloseDelay;
					m_iBosszoneOpenDelay = iBosszoneOpenDelay;
				}
				// �������� �� ���� �о����
				else if( iType == 1 )
				{
					// 1. >	������ �ʱ�ȭ
					INT iBossEggHalf = 0;
					INT iBossEggMax = 0;

					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iBossEggHalf = ( int )TokenNumber;		Token = GetToken();
					iBossEggMax = ( int )TokenNumber;
						
					// 3. >	������ ��ȿ�� �˻�
					// ���ı� �� ������ ���� �� ������ �����̸� ���� ó��
					if( iBossEggHalf < 0 )
					{
						MsgBox( "[ RAKLION ] - BossEggHalf count error : (%d)", iBossEggHalf );
						break;
					}
					// �� �ִ� ������ �����̸� ���� ó��
					if( iBossEggMax < 0 )
					{
						MsgBox( "[ RAKLION ] - BossEggMax count error : (%d)", iBossEggMax );
						break;
					}
					
					// 4. > ������ ����
					m_iBossEggHalf = iBossEggHalf;
					m_iBossEggMax = iBossEggMax;
				}
				// ������ ��ų ������ �� �о����(millisec)
				else if( iType == 2 )
				{
					// 1. >	������ �ʱ�ȭ
					INT iBossSkillDelay = 0;
					
					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iBossSkillDelay = ( int )TokenNumber;
					
					// 3. >	������ ��ȿ�� �˻�
					// ��ų �����̰� �����̸� ���� ó��
					if( iBossSkillDelay < 0 )
					{
						MsgBox( "[ RAKLION ] - BossSkillDelay count error : (%d)", iBossSkillDelay );
						break;
					}
									
					// 4. > ������ ����
					m_BattleOfSelupan.SetSelupanSkillDelay( iBossSkillDelay );
				}
				// ������ �� ���� ���� ���� �о����
				else if( iType == 3 )
				{
					// 1. >	������ �ʱ�ȭ
					INT iBossPatternCondition[6] = {0, };
					
					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					iBossPatternCondition[0] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[1] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[2] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[3] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[4] = ( int )TokenNumber;	Token = GetToken();
					iBossPatternCondition[5] = ( int )TokenNumber;
					
					// 3. >	������ ��ȿ�� �˻�
					// 4. > ������ ����					
					for( INT iCount=0; iCount<6; ++iCount )
					{						
						if( iBossPatternCondition[iCount] < 0 )
						{
							//HP %���� �����̸� ���� ó��
							MsgBox( "[ RAKLION ] - BossPatternCondition[%d] error : (%d)",
								iCount, iBossPatternCondition[iCount] );
							break;
						}
						else
						{
							// �������̸� ���� ����
							m_BattleOfSelupan.SetPatternCondition( iCount, iBossPatternCondition[iCount] );
						}
					}					
				}
				else
				{
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) 
						break;
				}
			}
		}
		fclose( SMDFile );
		
		LogAddC( 2, "[ RAKLION ] - %s file is Loaded", lpszFileName );

		m_bFileDataLoad = TRUE;	
	}
	catch(...) 
	{
		MsgBox( "[ RAKLION ] - Loading Exception Error (%s) File. ", lpszFileName );
	}

	return m_bFileDataLoad;	
}

// �� �����͸� �ε��Ѵ�. Terrain58.att Terrain59.att
void CRaklion::LoadRaklionMapAttr( LPSTR lpszFileName, BYTE btLevel )
{
}

//--------------------------------------------------------------------------------------------------------
void CRaklion::Run()
{
	
#ifdef UPDATE_SEASON_4_EVENT_LOG_200800715		// �̺�Ʈ�� �α� �߰�
	INT iRaklionUserCount = 0;
	
	time_t t;
	tm *pCurrentTime;
	time( &t );
	pCurrentTime = localtime( &t );
	
	if( m_iTodayDay!=pCurrentTime->tm_mday		// ���� Ʋ���ų�
		|| m_iTodayMon!=pCurrentTime->tm_mon	// ���� Ʋ���ų�
		|| m_iTodayYear!=pCurrentTime->tm_year)	// ������ Ʋ���� �α׸� ����� �ִ�.( �������� �Ǿ���.)
	{
		m_iTodayDay = pCurrentTime->tm_mday;
		m_iTodayMon = pCurrentTime->tm_mon;
		m_iTodayYear = pCurrentTime->tm_year;
		
		m_bEvent = FALSE;
	}
	
	// �α׸� ������ ���� �����̰� 0�� 0�� 0���̸� ��Ŭ���¿� �ִ� �������� �α׷� �����.
	if( m_bEvent==FALSE && pCurrentTime->tm_hour==0
		&& pCurrentTime->tm_min==0 && pCurrentTime->tm_sec==0 )
	{
		for( INT iCount=ALLOC_USEROBJECTSTART; iCount<MAX_OBJECT; ++iCount )
		{
			if( gObj[iCount].MapNumber==MAP_INDEX_RAKLION_FIELD || gObj[iCount].MapNumber==MAP_INDEX_RAKLION_BOSS )
			{
				if( gObj[iCount].Connected==CSS_GAMEPLAYING && gObj[iCount].Type==OBJTYPE_CHARACTER )
				{
					++iRaklionUserCount;
					LogAdd( "[RAKLION][IsInRaklionEvent] %d/%d/%d %dth User : %s (%s)",
						pCurrentTime->tm_year+1900, pCurrentTime->tm_mon+1, pCurrentTime->tm_mday,	// ��¥
						iRaklionUserCount,															// ���� ī��Ʈ
						gObj[iCount].Name,															// ĳ���͸�
						gObj[iCount].AccountID );													// ���� ���̵�
				}
			}			
		}
		
		m_bEvent = TRUE;
	}
	
#endif // UPDATE_SEASON_4_EVENT_LOG_200800715

	// 1. ��Ŭ���� �������� �����ϴ°�?
	if( !IsRaklionBossEnable() )
	{
		// ��Ŭ���� �̺�Ʈ�� �������� �ʵ��� �����Ǿ� ����
		return;
	}
	
	// 2. ��Ŭ���� �ʿ� �ִ� ������ ���� ó��
	CheckUserOnRaklionBossMap();
	
#ifdef MODIFY_RAKLION_BATTLE_USER_DELETE_BUGFIX_20080916
	// ������ ���� ó��
	g_RaklionBattleUserMng.CheckUserState();
#endif // MODIFY_RAKLION_BATTLE_USER_DELETE_BUGFIX_20080916

	// 3. ��Ŭ���� ���� ó��
	switch( m_iRaklionState )
	{
	case RAKLION_STATE_IDLE:
		ProcState_IDLE();
		break;

	case RAKLION_STATE_NOTIFY_1:
		ProcState_NOTIFY_1();
		break;

	case RAKLION_STATE_STANDBY:
		ProcState_STANDBY();
		break;

	case RAKLION_STATE_NOTIFY_2:
		ProcState_NOTIFY_2();
		break;

	case RAKLION_STATE_READY:
		ProcState_READY();
		break;
	
	case RAKLION_STATE_START_BATTLE:
		ProcState_START_BATTLE();
		break;
	
	case RAKLION_STATE_NOTIFY_3:
		ProcState_NOTIFY_3();
		break;

	case RAKLION_STATE_CLOSE_DOOR:
		ProcState_CLOSE_DOOR();
		break;

	case RAKLION_STATE_ALL_USER_DIE:
		ProcState_ALL_USER_DIE();
		break;

	case RAKLION_STATE_NOTIFY_4:
		ProcState_NOTIFY_4();
		break;
				
	case RAKLION_STATE_END:
		ProcState_END();
		break;
	}	
}

//--------------------------------------------------------------------------------------------------------
void CRaklion::SetState( int iRaklionState )
{
	switch( iRaklionState )
	{
	case RAKLION_STATE_IDLE:
		SetState_IDLE();
		break;
	case RAKLION_STATE_NOTIFY_1:
		SetState_NOTIFY_1();
		break;
	case RAKLION_STATE_STANDBY:
		SetState_STANDBY();
		break;
	case RAKLION_STATE_NOTIFY_2:
		SetState_NOTIFY_2();
		break;
	case RAKLION_STATE_READY:
		SetState_READY();
		break;
	case RAKLION_STATE_START_BATTLE:
		SetState_START_BATTLE();
		break;
	case RAKLION_STATE_NOTIFY_3:
		SetState_NOTIFY_3();
		break;
	case RAKLION_STATE_CLOSE_DOOR:
		SetState_CLOSE_DOOR();
		break;
	case RAKLION_STATE_ALL_USER_DIE:
		SetState_ALL_USER_DIE();
		break;
	case RAKLION_STATE_NOTIFY_4:
		SetState_NOTIFY_4();
		break;
	case RAKLION_STATE_END:
		SetState_END();
		break;		
	}

	g_RaklionUtil.NotifyRaklionChangeState( GetRaklionState(), m_BattleOfSelupan.GetBattleOfSelupanState() );
}

// ���� ���·� ���¸� ��ȯ�Ѵ�.
void CRaklion::SetNextState( RAKLION_STATE iCurrentState )
{
	int iNextState = iCurrentState + 1;
	if( iNextState >= RAKLION_STATE_MAX )
	{
		iNextState = RAKLION_STATE_IDLE;
	}
	
	SetState( iNextState );
}

//--------------------------------------------------------------------------------------------------------
// ���¸� ��ȯ�� �� �� �־�� �� �ϵ��� ����Ѵ�. 
void CRaklion::ChangeState( int iState, int DetailState )
{
}

//--------------------------------------------------------------------------------------------------------
void CRaklion::SetState_IDLE()
{
	//////////////////////////////////////////////////////////////////////////
	// IDLE ���·� ���� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> IDLE", m_iRaklionState );

	SetRaklionState( RAKLION_STATE_IDLE );
}

void CRaklion::SetState_NOTIFY_1()
{
	//////////////////////////////////////////////////////////////////////////
	// �������� �Ź̾��� ������.
	// Ŭ���̾�Ʈ ������ ���� �޽����� �����Ѵ�.
	// NOTIFY_1 ���·� ���� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> NOTIFY_1", m_iRaklionState );

	SetRaklionState( RAKLION_STATE_NOTIFY_1 );
}

void CRaklion::SetState_STANDBY()
{
	//////////////////////////////////////////////////////////////////////////
	// �Ź̾��� ��� ������.
	// Ŭ���̾�Ʈ ������ ���� �ð���ŭ �����̸� ���� ���� ƽī��Ʈ�� �����Ѵ�.
	// STANDBY ���·� ���� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> STANDBY", m_iRaklionState );

	m_iBossAppearanceStartTime = GetTickCount();

	SetRaklionState( RAKLION_STATE_STANDBY );
}

void CRaklion::SetState_NOTIFY_2()
{
	//////////////////////////////////////////////////////////////////////////
	// �Ź̾��� ��� ������.
	// Ŭ���̾�Ʈ�� ���� ���� �޽����� ������.
	// NOTIFY_2 ���·� ���� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> NOTIFY_2", m_iRaklionState );
	
	INT iDelayMinute = m_iBosszoneCloseDelay / 60;
	if( iDelayMinute == 0 )
		iDelayMinute = 1;

	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3410), iDelayMinute );
	
	SetRaklionState( RAKLION_STATE_NOTIFY_2 );
}

void CRaklion::SetState_READY()
{
	//////////////////////////////////////////////////////////////////////////
	// ������ ���¸� STANDBY ���·� ���ش�.
	// ������ ���°� STANDBY ���°� �Ǹ� �������� �����ϰ� �ʱ�ȭ �ϰ� �ȴ�.
	// READY ���·� ���� ���ش�.
	//////////////////////////////////////////////////////////////////////////
	
	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> READY", m_iRaklionState );

	// ���� ���� STANDBY�� ����
	m_BattleOfSelupan.SetState( BATTLE_OF_SELUPAN_STANDBY );
	
	SetRaklionState( RAKLION_STATE_READY );
}

void CRaklion::SetState_START_BATTLE()
{
	//////////////////////////////////////////////////////////////////////////
	// ������ ��⸦ ���� ������ ������ ���� ƽī� �����Ѵ�.
	// START_BATTLE ���·� ���� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> START_BATTLE", m_iRaklionState );

	m_iBosszoneCloseStartTime = GetTickCount();
	
	SetRaklionState( RAKLION_STATE_START_BATTLE );
}

void CRaklion::SetState_NOTIFY_3()
{
	//////////////////////////////////////////////////////////////////////////
	// �������� ���� �Ұ� �޽����� Ŭ���̾�Ʈ�� ������.
	// NOTIFY_3 ���·� ���� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> NOTIFY_3", m_iRaklionState );

	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3411) );
	
	SetRaklionState( RAKLION_STATE_NOTIFY_3 );
}

void CRaklion::SetState_CLOSE_DOOR()
{
	//////////////////////////////////////////////////////////////////////////
	// CLOSE_DOOR ���·� ���� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> CLOSE_DOOR", m_iRaklionState );
	
	SetRaklionState( RAKLION_STATE_CLOSE_DOOR );
}

void CRaklion::SetState_ALL_USER_DIE()
{
	//////////////////////////////////////////////////////////////////////////
	// ALL_USER_DIE ���·� ���� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> ALL_USER_DIE", m_iRaklionState );

#ifdef MODIFY_BATTLE_OF_SELUPAN_FAIL_DELETE_IMMEDIATELY_20080917
	// ������ �ʱ�ȭ
	m_BattleOfSelupan.HideSelupan();
	
	// ��ȯ ���� ����
	m_BattleOfSelupan.DeleteSummonMonster();
#endif // MODIFY_BATTLE_OF_SELUPAN_FAIL_DELETE_IMMEDIATELY_20080917
	
	SetRaklionState( RAKLION_STATE_ALL_USER_DIE );
}

void CRaklion::SetState_NOTIFY_4()
{
	//////////////////////////////////////////////////////////////////////////
	// NOTIFY_4 ���·� ���� ���ش�.
	// ������ ������� 5�а� ����ϱ� ���� �ð��� �����Ѵ�.
	// �������� ���� �������� Ŭ���̾�Ʈ�� �������ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> NOTIFY_4", m_iRaklionState );

	m_iBosszoneOpenStartTime = GetTickCount();

	BOOL bBattleSuccess = m_BattleOfSelupan.GetSuccessValue();

	// ����Ǳ� ������ �ð��� ������ ȯ���Ͽ� �޽��� ����
	INT iDelayMinute = m_iBosszoneOpenDelay / 60;
	if( iDelayMinute == 0 )
		iDelayMinute = 1;

	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3413), iDelayMinute );

	// ����� ����
	g_RaklionUtil.NotifyRaklionSuccessValue( bBattleSuccess );
	
	SetRaklionState( RAKLION_STATE_NOTIFY_4 );
}

void CRaklion::SetState_END()
{
	//////////////////////////////////////////////////////////////////////////
	// ������ �ʱ�ȭ
	// END ���·� �������ش�.
	//////////////////////////////////////////////////////////////////////////

	LogAddC( LOGC_PURPLE, "[ RAKLION ] State(%d) -> END", m_iRaklionState );

	// ������ �ʱ�ȭ
	m_BattleOfSelupan.HideSelupan();

	// ��ȯ ���� ����
	m_BattleOfSelupan.DeleteSummonMonster();

	// ������ ���� ���� ����Ʈ �ʱ�ȭ
//	g_RaklionBattleUserMng.ResetAllData();

	// ������ ���� �����층 �ʱ�ȭ
	m_BattleOfSelupan.ResetAllData();
	
	// ������ ��ü �����층 ������ �ʱ�ȭ
	ResetAllData();

	// ������ �Ա� ���� �޽��� ����
	g_RaklionUtil.SendMsgAllUser( lMsg.Get(3414) );

	SetRaklionState( RAKLION_STATE_END );

	// �Ź̾� ����
	RegenBossEgg();
}

//------------------------------------------------------------------------------------------------
void CRaklion::ProcState_IDLE()
{
	//////////////////////////////////////////////////////////////////////////
	// �������� ����ǰ� ���� �ʴ�.
	// �ƹ��� ���� ��� �Ѵ�.
	//////////////////////////////////////////////////////////////////////////

	// ������ ���� ������ üũ
	if( GetBossEggDieCount() == 0 )
	{
		SetState( RAKLION_STATE_STANDBY );
	}
	else
	{
		// ���� NOTIFY_1�� ������� �ʾҴٸ� �Ź̾��� ����ġ��ŭ �ı��Ǿ����� üũ
		if( m_bIsNotify1 == FALSE )
		{
			if( GetBossEggDieCount() == m_iBossEggHalf )
			{
				// �Ź̾��� ����ġ��ŭ �ı��Ǿ��ٸ� NOTIFY_1 ����
				SetState( RAKLION_STATE_NOTIFY_1 );
				m_bIsNotify1 = TRUE;
			}
		}
	}
}

void CRaklion::ProcState_NOTIFY_1()
{
	//////////////////////////////////////////////////////////////////////////
	// SetState( RAKLION_STATE_IDLE );
	//////////////////////////////////////////////////////////////////////////

	SetState( RAKLION_STATE_IDLE );
}

void CRaklion::ProcState_STANDBY()
{
	//////////////////////////////////////////////////////////////////////////
	// ������ ���� �ð���ŭ ��� �� NOTIFY_2 ���·� �����Ѵ�.
	// SetState( RAKLION_STATE_NOTIFY_2 );
	//////////////////////////////////////////////////////////////////////////

	if( 1000*m_iBossAppearanceDelay <= GetTickCount()-m_iBossAppearanceStartTime )
	{
		SetState( RAKLION_STATE_NOTIFY_2 );
	}
}

void CRaklion::ProcState_NOTIFY_2()
{
	//////////////////////////////////////////////////////////////////////////
	// ���� ������ Ŭ�����Ʈ�� �˷����Ƿ� ���¸� READY���·� �ٲ��ش�.
	//////////////////////////////////////////////////////////////////////////

	SetState( RAKLION_STATE_READY );
}

void CRaklion::ProcState_READY()
{
	//////////////////////////////////////////////////////////////////////////
	// �������� ������ �غ��Ѵ�.
	//////////////////////////////////////////////////////////////////////////
	
#ifdef MODIFY_RAKLION_SELUPAN_NO_REGEN_BUGFIX_20080730
//	INT iSelupanIndex = m_BattleOfSelupan.GetSelupanObjIndex();

	// �������� ������ �Ϸ���� �ʾҴٸ� return
//	if( gObj[iSelupanIndex].DieRegen > 0 )
//		return;
#else
	INT iSelupanIndex = m_BattleOfSelupan.GetSelupanObjIndex();
	
	// �������� ������ �Ϸ���� �ʾҴٸ� return
	if( gObj[iSelupanIndex].DieRegen > 0 )
		return;
#endif // MODIFY_RAKLION_SELUPAN_NO_REGEN_BUGFIX_20080730

	SetState( RAKLION_STATE_START_BATTLE );
}

void CRaklion::ProcState_START_BATTLE()
{
	//////////////////////////////////////////////////////////////////////////
	// �������� ����� ���·� �������� �������̴�.
	// ������ �ð���ŭ �� ���¸� �����ϴٰ� NOTIFY_3���� �����Ѵ�.
	// m_BattleOfSelupan.Run() �� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	if( 1000*m_iBosszoneCloseDelay < GetTickCount()-m_iBosszoneCloseStartTime )
	{
		SetState( RAKLION_STATE_NOTIFY_3 );
	}

	// ������ ����
	m_BattleOfSelupan.SetBattleOfSelupanStateAuto();
	m_BattleOfSelupan.Run();
}

void CRaklion::ProcState_NOTIFY_3()
{
	//////////////////////////////////////////////////////////////////////////
	// �������� ���۵� �� ���õ� �ð��� ���� �������� �����ٴ� �޽����� ���´�.
	// �������� ����ϱ� ���� CLOSE_DOOR ���·� �����Ѵ�..
	// SetState( RAKLION_STATE_CLOSE_DOOR );
	//////////////////////////////////////////////////////////////////////////

	// ������ ����
	m_BattleOfSelupan.SetBattleOfSelupanStateAuto();
	m_BattleOfSelupan.Run();

	SetState( RAKLION_STATE_CLOSE_DOOR );
}

void CRaklion::ProcState_CLOSE_DOOR()
{
	//////////////////////////////////////////////////////////////////////////
	// �������� ���� ���·� �������̴�.
	// m_BattleOfSelupan.Run() �� ���ش�.
	//////////////////////////////////////////////////////////////////////////

	if( g_RaklionBattleUserMng.IsEmpty() == TRUE )
	{	// �������� ��� ������ ����ߴٸ� ����� FALSE�� ���ش�.
		m_BattleOfSelupan.SetSuccessValue( FALSE );
		SetState( RAKLION_STATE_ALL_USER_DIE );
	}
	else
	{
		if( m_BattleOfSelupan.GetBattleOfSelupanState() == BATTLE_OF_SELUPAN_DIE )
		{	// ������ ���� ���¶�� ����� TRUE���ش�.
			m_BattleOfSelupan.SetSuccessValue( TRUE );
			SetState( RAKLION_STATE_NOTIFY_4 );
		}
		else
		{	// ���� ��Ȳ�� �ƴ� ��� ����ؼ� �����Ѵ�.
			m_BattleOfSelupan.SetBattleOfSelupanStateAuto();
			m_BattleOfSelupan.Run();
		}
	}
}

void CRaklion::ProcState_ALL_USER_DIE()
{
	//////////////////////////////////////////////////////////////////////////
	// ������ ��� �� ��� ������ �׾���.
	// �������� ���Ḧ �˸��� ���� NOTIFY_4 ���·� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////

	SetState( RAKLION_STATE_NOTIFY_4 );
}

void CRaklion::ProcState_NOTIFY_4()
{
	//////////////////////////////////////////////////////////////////////////
	// �������� ���Ḧ Ŭ���̾�Ʈ�� �˷ȴ�.
	// ������ �ð���ŭ ��� �� END ���·� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////

	if( 1000*m_iBosszoneOpenDelay < GetTickCount()-m_iBosszoneOpenStartTime )
	{
		SetState( RAKLION_STATE_END );
	}
}

void CRaklion::ProcState_END()
{
	//////////////////////////////////////////////////////////////////////////
	// ��Ŭ���� �������� ������ ����Ǿ��� �ʱ�ȭ �Ǿ���.
	// IDLE���·� �����Ѵ�.
	// SetState( RAKLION_STATE_IDLE );
	//////////////////////////////////////////////////////////////////////////

	SetState( RAKLION_STATE_IDLE );
}

//---------------------------------------------------------------------------------------
// ��Ŭ���� ������ ���� ���θ� �����Ѵ�.,
void CRaklion::SetRaklionBossEnable( BOOL bEnable )
{
	m_bRaklionBossEnable = bEnable;
}

// ��Ŭ���� �������� �����Ѱ�?
int CRaklion::IsRaklionBossEnable()
{
#ifdef MODIFY_CRYWOLF_MOVE_RAKLION_BUGFIX_20080910
	if( g_bRaklionEvent && m_bRaklionBossEnable )
		return TRUE;
	
	return FALSE;
#else
	return m_bRaklionBossEnable;
#endif // MODIFY_CRYWOLF_MOVE_RAKLION_BUGFIX_20080910
}

// ��Ŭ���� ���¸� �����Ѵ�.
void CRaklion::SetRaklionState( RAKLION_STATE iRaklionState )
{
	m_iRaklionState = iRaklionState;
}

// ���� ��Ŭ������ ���¸� ���´�.
int CRaklion::GetRaklionState()
{
	return m_iRaklionState;
}

// ������ �ൿ ���¸� �����Ѵ�.
void CRaklion::SetRaklionStateDetail( INT iSelupanState )
{
	m_BattleOfSelupan.SetBattleOfSelupanState(iSelupanState);
}

// ������ �ൿ ���¸� ���´�.
int CRaklion::GetRaklionStateDetail()
{
	return m_BattleOfSelupan.GetBattleOfSelupanState();
}

//---------------------------------------------------------------------------------------
// �ı��� �Ź̾��� ���� ������Ų��.
void CRaklion::BossEggDieIncrease()
{
	if( m_iBossEggDieCount >= m_iBossEggMax )
	{
		// �α� ��� ���� �߻�
		LogAddC( LOGC_RED, "[RAKLION] BossEggDieCount Error : (%d)", m_iBossEggDieCount );
		return;
	}
	else
		++m_iBossEggDieCount;
}

// �ı��� �Ź̾��� ���� ���ҽ�Ų��.
void CRaklion::BossEggDieDecrease()
{
	if( m_iBossEggDieCount <= 0 )
	{
		// �α� ��� ���� �߻�
		LogAddC( LOGC_RED, "[RAKLION] BossEggDieCount Error : (%d)", m_iBossEggDieCount );
		return;
	}
	else
		--m_iBossEggDieCount;
}

// ���� �ı��� �Ź̾��� ���� ���´�.
DWORD CRaklion::GetBossEggDieCount()
{
	return m_iBossEggDieCount;
}

// �Ź̾��� ��� ������Ų��.
void CRaklion::RegenBossEgg()
{
	for( INT iCount=0; iCount<MAX_MONSTER; ++iCount )
	{
		if( gObj[iCount].Class == 460 || gObj[iCount].Class == 461 || gObj[iCount].Class == 462 )
		{
			gObjSetMonster( gObj[iCount].m_Index, gObj[iCount].Class );
			if( gObjMonsterRegen( &gObj[iCount] ) )
			{
				gObj[iCount].Live = 1;
				gObj[iCount].DieRegen = 0;
				
				BossEggDieIncrease();
			}
		}
	}

	LogAddC( LOGC_LIGHTRED, "[ RAKLION ] [ BossEggRegen ] : BossEgg(%d ea)", GetBossEggDieCount() );
}

// �Ź̾��� ��� ���ش�.
void CRaklion::DeleteBossEgg()
{
	for( INT iCount=0; iCount<MAX_MONSTER; ++iCount )
	{
		if( gObj[iCount].Class == 460 || gObj[iCount].Class == 461 || gObj[iCount].Class == 462 )
		{
			gObjDel ( iCount );
			
			gObj[iCount].Live = 0;
			gObj[iCount].m_State = OBJST_NONE;
			
			// �Ź̾��� ����Ʈ���� �����.
			gObjViewportListProtocolDestroy( &gObj[iCount] );
			gObjViewportClose( &gObj[iCount] );
				
			BossEggDieDecrease();
		}
	}

	LogAddC( LOGC_LIGHTRED, "[ RAKLION ] [ BossEggDelete ] : BossEgg(%d ea)", GetBossEggDieCount() );
}

//--------------------------------------------------------------------------------------------
// ���������� ������� �ʿ� �ִ� ������ �i�Ƴ���.
void CRaklion::CheckUserOnRaklionBossMap()
{
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{		
		if( ( gObj[iCount].Connected == CSS_GAMEPLAYING ) && ( gObj[iCount].Type == OBJTYPE_CHARACTER ) 
			&& gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS 
			)			
		{
#ifdef MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_2ND_20080917
			//if( GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR )		// �������� ���� ����
#else
			if( GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR )		// �������� ���� ����
#endif // MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_2ND_20080917
			{
				// ��ڴ� ������.
				if( ( gObj[iCount].Authority & AUTHORITY_ADMIN ) != AUTHORITY_ADMIN )
				{
#ifdef MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_20080818
					if( g_RaklionBattleUserMng.IsBattleUser(iCount) )	// ������ ���� ������ �Ѿ��
						continue;
					else												// �ƴ϶�� �߹��Ѵ�.
					{
						// ���������� ������ �̵��� �Ŀ� �ű��.
						if( gObj[iCount].RegenOk == 0  && gObj[iCount].m_State == OBJST_PLAYING 
							&& gObj[iCount].Live == 1 )
						{
							// ��Ŭ���� ��������� �Ѿƹ�����. ( ������ ���°ɷ� �����ص� ������ ��. )
							gObjMoveGate( iCount, 286 );
							LogAddC( LOGC_RED, "[ RAKLION ][ Invalid User ] Invalid Raklion Boss Map User[%s][%s]",
								gObj[iCount].AccountID, gObj[iCount].Name );
						}
					}
#else
					// ���������� ������ �̵��� �Ŀ� �ű��.
					if( gObj[iCount].RegenOk == 0  && gObj[iCount].m_State == OBJST_PLAYING 
						&& gObj[iCount].Live == 1 )
					{
						// ��Ŭ���� ��������� �Ѿƹ�����. ( ������ ���°ɷ� �����ص� ������ ��. )
						gObjMoveGate( iCount, 286 );
						LogAddC( LOGC_RED, "[ RAKLION ][ Invalid User ] Invalid Raklion Boss Map User[%s][%s]",
							gObj[iCount].AccountID, gObj[iCount].Name );
					}
#endif // MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_20080818
				}
			}
		}
	}
}

// ��Ŭ���� �������� ���� �����Ѱ�?
int CRaklion::CheckEnterRaklion( int iUserIndex )
{
	if( !gObjIsConnected( iUserIndex ) )
		return -1;	

	if( GetRaklionState() == RAKLION_STATE_CLOSE_DOOR )
	{
		// ������ �� ����
		LogAddTD( "[ RAKLION ][ Entrance Fail ] [%s][%s] State(%d)", 
				gObj[iUserIndex].AccountID,
				gObj[iUserIndex].Name,
				GetRaklionState() );
		return 3;		// �������� ������ ���� �Ұ�
	}

	// �� ��ȣ�� �ٸ��� ������ �� ����.
	if( gObj[iUserIndex].MapNumber != MAP_INDEX_RAKLION_BOSS )
	{
		// ������ �� ����
		LogAddTD( "[ RAKLION ][ Entrance Fail ] Invalid Map Number(%d) [%s][%s] State(%d)",
			gObj[iUserIndex].MapNumber,
			gObj[iUserIndex].AccountID,
			gObj[iUserIndex].Name,
			GetRaklionState() );
		return 4;		// �ʹ�ȣ�� Ʋ���Ƿ� ���� �Ұ�
	}

	// ���� ���� �Ұ� ���ǵ��� ����ϸ� ���� ���� - return ���� 0�̸� ���� ����
	return 0;
}

// ��� ��ɾ�
void CRaklion::OperateGmCommand( int iUserIndex, int iCommand )
{
	#if ( TESTSERVER != 1 )
	// �׽�Ʈ�߿��� ��� �����ϴ�.
	return;	
#endif

	if( !gObjIsConnected( iUserIndex ) )
		return;

	LogAddTD( "[ RAKLION ][ Use GM Command ] [%s][%s] Command : %d", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, iCommand );
	
	switch( iCommand )
	{
	case RAKLION_GM_COMMAND_IDLE:
		SetState( RAKLION_STATE_END );
		break;
	case RAKLION_GM_COMMAND_NOTIFY_1:
		SetState( RAKLION_STATE_NOTIFY_1 );
		break;
	case RAKLION_GM_COMMAND_STANDBY:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_STANDBY );
		break;
	case RAKLION_GM_COMMAND_NOTIFY_2:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_NOTIFY_2 );
		break;
	case RAKLION_GM_COMMAND_READY:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_READY );
		break;
	case RAKLION_GM_COMMAND_START_BATTLE:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_START_BATTLE );
		break;
	case RAKLION_GM_COMMAND_NOTIFY_3:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_NOTIFY_3 );
		break;
	case RAKLION_GM_COMMAND_CLOSE_DOOR:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_CLOSE_DOOR );
		break;
	case RAKLION_GM_COMMAND_ALL_USER_DIE:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_STATE_ALL_USER_DIE );
		break;
	case RAKLION_GM_COMMAND_NOTIFY_4:
		SetState( RAKLION_STATE_END );
		DeleteBossEgg();
		SetState( RAKLION_GM_COMMAND_NOTIFY_4 );
		break;
	case RAKLION_GM_COMMAND_END:
		SetState( RAKLION_STATE_END );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_1:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_1 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_2:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_2 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_3:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_3 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_4:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_4 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_5:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_5 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_6:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_6 );
		break;
	case RAKLION_GM_COMMAND_SELUPAN_7:
		m_BattleOfSelupan.GmCommandSelupanPattern( BATTLE_OF_SELUPAN_PATTERN_7 );
		break;
	case RAKLION_GM_COMMAND_FIRST_SKILL:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_FIRST_SKILL );
		break;
	case RAKLION_GM_COMMAND_POISON_ATTACK:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_POISON_ATTACK );
		break;
	case RAKLION_GM_COMMAND_ICE_STORM:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_ICE_STORM );
		break;
	case RAKLION_GM_COMMAND_ICE_STRIKE:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_ICE_STRIKE );
		break;
	case RAKLION_GM_COMMAND_SUMMON_MONSTER:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_SUMMON_MONSTER );
		break;
	case RAKLION_GM_COMMAND_HEAL:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_HEAL );
		break;
	case RAKLION_GM_COMMAND_FREEZE:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_FREEZE );
		break;
	case RAKLION_GM_COMMAND_TELEPORT_RANDOM:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_TELEPORT_RANDOM );
		break;
	case RAKLION_GM_COMMAND_INVINCIBILITY:
		m_BattleOfSelupan.ActionOfSelupan( RAKLION_SELUPAN_SKILL_INVINCIBILITY );
		break;
	}	
}

#include "LogToFile.h"
CLogToFile	RAKLION_TEST_LOG( "RAKLION_TEST_LOG", ".\\RAKLION_TEST_LOG");		// �α����� ��ü
void CRaklion::UserMonsterCountCheck()
{	
#if ( TESTSERVER != 1 )
	return;
#endif
	
	// �������� ���� �˻�
	if( GetRaklionState() != RAKLION_STATE_START_BATTLE && GetRaklionState() != RAKLION_STATE_CLOSE_DOOR )
	{
		return;
	}
	
	// 1. ���� ���� �� �˻� - Manager		
	RAKLION_TEST_LOG.Output( "[ RAKLION ][ Debug ] 1. UserMng Count:%d", g_RaklionBattleUserMng.GetUserCount() );
	
	// 2. ���� ���� �� �˻� - ��ü
	int iUserCount_Live = 0;
	int iUserCount_Die = 0;
	for( int iAllUserCount = ALLOC_USEROBJECTSTART; iAllUserCount < MAX_OBJECT; iAllUserCount++ )
	{
		if( gObj[iAllUserCount].MapNumber == MAP_INDEX_RAKLION_BOSS 
			&& gObj[iAllUserCount].Type == OBJTYPE_CHARACTER
			&& ( gObj[iAllUserCount].Authority & AUTHORITY_ADMIN ) != AUTHORITY_ADMIN )
		{
			if( gObj[iAllUserCount].Live == 1 )
			{
				++iUserCount_Live;
			}
			else
			{
				++iUserCount_Die;
			}
		}
	}
	RAKLION_TEST_LOG.Output( "[ RAKLION ][ Debug ] 2. All User Check:%d/(Die:%d)", iUserCount_Live, iUserCount_Die );
	
	// 3. ���� ���� �� �˻� - Manager
//	RAKLION_TEST_LOG.Output( "[ RAKLION ][ Debug ] 3. MonsterMng Count:%d", g_RaklionMonsterMng.GetAliveMonsterCount() );
	
	// 4. ���� ���� �� �˻� - ��ü
	int iMonsterCount = 0;
	for( int iAllMonsterCount = 0; iAllMonsterCount < ALLOC_USEROBJECTSTART; iAllMonsterCount++ )
	{
		if( gObj[iAllMonsterCount].MapNumber == MAP_INDEX_RAKLION_BOSS 
			&& gObj[iAllMonsterCount].Connected == 3
			&& gObj[iAllMonsterCount].Type	== OBJTYPE_MONSTER )
		{
			
			if( gObj[iAllMonsterCount].Class != RAKLION_SELUPAN )		// ������ ����
			{
				++iMonsterCount;
				RAKLION_TEST_LOG.Output( "[ RAKLION ][ Debug ] 4. Monster Info %s(%d/ObjIndex:%d) [%d]%d-%d HP:%0.1f IsAlive:%d Connected:%d State:%d ActState:Attack(%d)/Move(%d) DieRegen:%d", 
					gObj[iAllMonsterCount].Name, gObj[iAllMonsterCount].Class, gObj[iAllMonsterCount].m_Index, 
					gObj[iAllMonsterCount].MapNumber, gObj[iAllMonsterCount].X, gObj[iAllMonsterCount].Y,
					gObj[iAllMonsterCount].Life, 
					gObj[iAllMonsterCount].Live, 
					gObj[iAllMonsterCount].Connected, 
					gObj[iAllMonsterCount].m_State, 
					gObj[iAllMonsterCount].m_ActState.Attack,
					gObj[iAllMonsterCount].m_ActState.Move,
					gObj[iAllMonsterCount].DieRegen
					);
			}
		}
	}
}



#endif