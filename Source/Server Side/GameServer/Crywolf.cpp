#include "Stdafx.h"

#ifdef CRYWOLF_COMMON_PATCH_20050413

#include "Crywolf.h"
#include "CrywolfUtil.h"

#include "TMonsterAIGroup.h"

#include "..\Include\ReadScript.h"
#include "..\common\Winutil.h"
#include "protocol.h"
#include "user.h"
#include "gamemain.h"

#include "MapServerManager.h"
#include "DSProtocol.h"

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif


#include "MonsterSetBase.h"
extern	CMonsterSetBase	gMSetBase;
static	CCrywolfUtil	UTIL;

BOOL			g_bDBCrywolfOccupied = 0;
CCrywolf		g_Crywolf;

#include <set>
#include <algorithm>


CCrywolf::CCrywolf()
{
	m_bFileDataLoad					= FALSE;
	m_bDBDataLoadOK					= FALSE;
	m_bDBDataLoading				= FALSE;
	m_iCrywolfState					= CRYWOLF_STATE_NONE;

	m_iScheduleMode					= CRYWOLF_SCHEDULE_MODE_COMMON;

	m_dwCrywolfNotifyMsgStartTick	= _ZERO;
	m_dwCrywolfStartProcTick		= _ZERO;
	m_bTurnUpBoss					= FALSE;
	m_bChangeAI						= FALSE;
	m_iBossIndex					= _INVALID;

	m_iBossGroupNumber				= _ZERO;

	ZeroMemory( &m_iMonsterGroupNumberArray, MAX_CRYWOLF_MVP_MONSTER_GROUP );
}

CCrywolf::~CCrywolf()
{

}

BOOL CCrywolf::LoadData( LPSTR lpszFileName )
{
	m_bFileDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Crywolf Main Job] - File load error : File Name Error");
		return FALSE;
	}

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Crywolf Main Job] - Can't Open %s ", lpszFileName);
			return FALSE;
		}
		
		// ��� ������ �ʱ�ȭ.
		DelAllData();

		SMDToken Token;
		
		int iType = -1;

		//----------------------------------------------------------------------------
		//
		//
		//	[0] ũ���̿��� ���� �� ������ ��� ����
		//	[1] ũ���̿��� ���� �ܰ躰 Ÿ�� ���̺� 
		//	[2] ũ���̿��� ���� Group ��ȣ
		//	[3] ũ���̿��� ���� Balgars Group ��ȣ, �����ð�(sec)
		//	[4] ũ���̿��� ���� AI Change �ð�(sec) 
		//	
		//----------------------------------------------------------------------------
		while( TRUE )
		{
			Token = GetToken();
			if(Token == END) break;
			iType = (int)TokenNumber;

			while( TRUE )
			{				
				// [0] ũ���̿��� ���� �� ������ ��� ����
				if( iType == 0 )
				{	
					// 1. >	������ �ʱ�ȭ
					int	iScheduleMode		= CRYWOLF_SCHEDULE_MODE_COMMON;
					int	iStateNumber		= _NONE;
					int	iMonth				= _NONE;
					int iDay				= _NONE;
					int iDayOfWeek			= _NONE;
					int iHour				= _NONE;
					int iMinute				= _NONE;
					int iContinuanceTime	= _ZERO;
					
					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iScheduleMode = (int)TokenNumber;
					Token = GetToken(); iStateNumber		= (int)TokenNumber;
					Token = GetToken(); iMonth				= (int)TokenNumber;
					Token = GetToken(); iDay				= (int)TokenNumber;
					Token = GetToken(); iDayOfWeek			= (int)TokenNumber;
					Token = GetToken(); iHour				= (int)TokenNumber;
					Token = GetToken(); iMinute				= (int)TokenNumber;
					Token = GetToken(); iContinuanceTime	= (int)TokenNumber;
					
					// 3. >	������ ��ȿ�� �˻�
					if( m_StartTimeInfoCount <= _INVALID || m_StartTimeInfoCount >= MAX_CRYWOLF_STARTTIME )
					{
						MsgBox("[Crywolf Main Job] - Excced MAX StartTime (%d)", m_StartTimeInfoCount);
					}

					// 4. > ������ ����
					m_iScheduleMode												= iScheduleMode;
					m_StartTimeInfo[m_StartTimeInfoCount].m_iStateNumber		= iStateNumber;
					m_StartTimeInfo[m_StartTimeInfoCount].m_iMonth				= iMonth;
					m_StartTimeInfo[m_StartTimeInfoCount].m_iDay				= iDay;
					m_StartTimeInfo[m_StartTimeInfoCount].m_iDayOfWeek			= iDayOfWeek;
					m_StartTimeInfo[m_StartTimeInfoCount].m_iHour				= iHour;
					m_StartTimeInfo[m_StartTimeInfoCount].m_iMinute				= iMinute;
					m_StartTimeInfo[m_StartTimeInfoCount].m_iContinuanceTime	= iContinuanceTime;
					m_StartTimeInfo[m_StartTimeInfoCount].m_bUsed				= TRUE;

					m_StartTimeInfoCount++;
										
				}
				
				// [1] ũ���̿��� ���� �ܰ躰 Ÿ�� ���̺� 
				if( iType == 1 )
				{
					// 1. >	������ �ʱ�ȭ
					int	iStateNumber		= _NONE;
					int iHour				= _ZERO;
					int iMinute				= _ZERO;
					int	iSecond				= _ZERO;
					
					
					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iStateNumber = (int)TokenNumber;
					Token = GetToken(); iHour						= (int)TokenNumber;
					Token = GetToken(); iMinute						= (int)TokenNumber;
					Token = GetToken(); iSecond						= (int)TokenNumber;
		
					// 3. >	������ ��ȿ�� �˻�

				
					// 4. > ������ ����
					m_StateTimeInfo[iStateNumber].m_iStateNumber	= iStateNumber;
					m_StateTimeInfo[iStateNumber].m_iHour			= iHour;
					m_StateTimeInfo[iStateNumber].m_iMinute			= iMinute;
					m_StateTimeInfo[iStateNumber].m_iSecond			= iSecond;
					
					m_StateTimeInfo[iStateNumber].m_iContinuanceTime= iHour*60*60*1000 + iMinute*60*1000 + iSecond*1000;

					m_StateTimeInfo[iStateNumber].m_bUsed			= TRUE;
				}

				// [2] ũ���̿��� ���� Group ��ȣ
				if( iType == 2 )
				{
					// 1. >	������ �ʱ�ȭ
					int iGroupNumber = _INVALID;

					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iGroupNumber = (int)TokenNumber;

					// 3. >	������ ��ȿ�� �˻�
					if( iGroupNumber<=_INVALID && iGroupNumber>=MAX_CRYWOLF_MVP_MONSTER_GROUP )
					{
						MsgBox("[Crywolf Main Job] - Invalid Monster Group Number (%d)", iGroupNumber);
					}

					// 4. > ������ ����
					m_iMonsterGroupNumberArray[m_iMonsterGroupNumberCount] = iGroupNumber;
					m_iMonsterGroupNumberCount++;
				}

				// [3] ũ���̿��� ���� Balgars Group ��ȣ
				if( iType == 3 )
				{
					// 1. >	������ �ʱ�ȭ
					int iGroupNumber		= _INVALID;
					int	iTurnUpTime			= _ZERO;

					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iGroupNumber = (int)TokenNumber;
					Token = GetToken(); iTurnUpTime = (int)TokenNumber;

					// 3. >	������ ��ȿ�� �˻�
					if( iGroupNumber<=_INVALID && iGroupNumber>=MAX_CRYWOLF_MVP_MONSTER_GROUP )
					{
						MsgBox("[Crywolf Main Job] - Invalid Monster Boss Group Number (%d)", iGroupNumber);
					}

					// 4. > ������ ����
					m_iBossGroupNumber	= iGroupNumber;
					m_iBossTurnUpTime	= iTurnUpTime;
				}
				
				// [4] ũ���̿��� �Ϲ� ���� AI Change �ð�
				if( iType == 4 )
				{
					int iChangeAITime		= _ZERO;

					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iChangeAITime = (int)TokenNumber;
					
					// 3. >	������ ��ȿ�� �˻�
				
					// 4. > ������ ����
					m_iMonsterGroupChangeAITime = iChangeAITime;
				}

				// [5] MVP Score ��갪 
				if( iType == 5 )
				{
					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_BALGARS] = (int)TokenNumber;
					Token = GetToken(); m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_DARKELF]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_BALRAM]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_SORAM]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_DEATHSPRIT]	= (int)TokenNumber;
					Token = GetToken(); m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_TANKER]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPScoreTable[CRYWOLF_SCORE_LAST_CONTRACTOR]		= (int)TokenNumber;
					
					// 3. >	������ ��ȿ�� �˻�
				
					// 4. > ������ ����
				}
				
				// [6] MVP Score�� �̿��� ��ũ ��갪
				if( iType == 6 )
				{
					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					m_iMVPRankScoreTable[CRYWOLF_RANK_D] = (int)TokenNumber;
					Token = GetToken(); m_iMVPRankScoreTable[CRYWOLF_RANK_C]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPRankScoreTable[CRYWOLF_RANK_B]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPRankScoreTable[CRYWOLF_RANK_A]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPRankScoreTable[CRYWOLF_RANK_S]		= (int)TokenNumber;
					
					// 3. >	������ ��ȿ�� �˻�
				
					// 4. > ������ ����
					
				}
			
				// [7] MVP Score�� �̿��� ����ġ ��갪
				if( iType == 7 )
				{
					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					m_iMVPRankExpTable[CRYWOLF_RANK_D] = (int)TokenNumber;
					Token = GetToken(); m_iMVPRankExpTable[CRYWOLF_RANK_C]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPRankExpTable[CRYWOLF_RANK_B]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPRankExpTable[CRYWOLF_RANK_A]		= (int)TokenNumber;
					Token = GetToken(); m_iMVPRankExpTable[CRYWOLF_RANK_S]		= (int)TokenNumber;
					
					// 3. >	������ ��ȿ�� �˻�
				
					// 4. > ������ ����
				}
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Crywolf Main Job] - %s file is Loaded", lpszFileName);

		m_bFileDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Crywolf Main Job] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return m_bFileDataLoad;
}

void CCrywolf::DelAllData()
{
	for( int i=0; i<MAX_CRYWOLF_STATE_TYPE; i++ )
	{
		m_StateTimeInfo[i].Reset();
	}

	for( int j=0; j<MAX_CRYWOLF_STARTTIME; j++ )
	{
		m_StartTimeInfo[j].Reset();	
	}

	m_StartTimeInfoCount = 0;

	for( int k=0; k<MAX_CRYWOLF_MVP_MONSTER_GROUP; k++ )
	{
		m_iMonsterGroupNumberArray[k]	= _INVALID;
	}

	m_iMonsterGroupNumberCount	= _ZERO;
	m_iBossGroupNumber			= _ZERO;
	m_iBossTurnUpTime			= _ZERO;
	m_iMonsterGroupChangeAITime = _ZERO;

	m_bDBDataLoadOK				= FALSE;
	m_bDBDataLoading			= FALSE;

	ZeroMemory( &m_iMVPScoreTable,		MAX_CRYWOLF_SCORE_TABLE );
	ZeroMemory( &m_iMVPRankScoreTable,	MAX_CRYWOLF_RANK );
	ZeroMemory( &m_iMVPRankExpTable,	MAX_CRYWOLF_RANK );

	SetState( CRYWOLF_STATE_NONE );
}

void CCrywolf::Init()
{

}


// ũ���̿��� �� �ε� 
void CCrywolf::LoadCrywolfMapAttr( LPSTR lpszFileName, int iOccupationState )
{
	if( !lpszFileName )
		return;

	if( iOccupationState < 0 || iOccupationState >= MAX_CRYWOLF_MAP_STATE )
		return;

	m_CrywolfMapAttr[iOccupationState].LoadMapAttr( lpszFileName, MAP_INDEX_CRYWOLF_FIRSTZONE );
}


// ũ���̿��� �� ����
void CCrywolf::SetCrywolfMapAttr( int iOccupationState )
{
	if( iOccupationState < 0 || iOccupationState >= MAX_CRYWOLF_MAP_STATE )
		return;

	MapClass& CrywolfMap = m_CrywolfMapAttr[iOccupationState];
	
	if( !CHECK_CRYWOLF_FIRSTZONE(CrywolfMap.thisMapNumber) )
	{
		LogAddC( LOGC_RED, "[ Crywolf ] Fail - Chang Map Attribute (State:%d) ", iOccupationState );
		return;
	}
	
	CopyMemory( MapC[MAP_INDEX_CRYWOLF_FIRSTZONE].m_attrbuf, CrywolfMap.m_attrbuf, (MAX_TERRAIN_SIZE*MAX_TERRAIN_SIZE) );
}


// ũ���̿��� NPC ���� ���� ���¿� ���� ���� ���ش�.
void CCrywolf::SetCrywolfCommonNPC( int iOccupationState )
{
	
	for( int i=0; i<m_ObjCommonNPC.m_iObjCount; i++ )
	{
		if( !gObjIsConnected(m_ObjCommonNPC.m_iObjIndex[i]) )
			return;

		LPOBJECTSTRUCT lpObj = &gObj[ m_ObjCommonNPC.m_iObjIndex[i] ];

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( iOccupationState == CRYWOLF_OCCUPATION_STATE_WAR || iOccupationState == CRYWOLF_OCCUPATION_STATE_OCCUPIED )
		{
			gObjAddBuffEffect( lpObj, BUFFTYPE_CRYWOLF_NPC_INVISABLE, 0, 0, 0, 0, BUFFTIME_INFINITE );
		}
		else if( iOccupationState == CRYWOLF_OCCUPATION_STATE_PEACE )
		{
			gObjRemoveBuffEffect( lpObj, BUFFTYPE_CRYWOLF_NPC_INVISABLE );
		}
#else
		if( iOccupationState == CRYWOLF_OCCUPATION_STATE_WAR )
		{
			// ���·� ����� ���� ǥ���ϵ�������.
			ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_CRYWOLF_NPC_HIDE );
			GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );
		}
		else if( iOccupationState == CRYWOLF_OCCUPATION_STATE_PEACE )
		{
			// ��Ÿ��
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_CRYWOLF_NPC_HIDE );
			GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );
		}
		else if( iOccupationState == CRYWOLF_OCCUPATION_STATE_OCCUPIED )
		{
			// �����
			ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_CRYWOLF_NPC_HIDE );
			GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );
		}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		
	}
	
}


// ���� ���¸� �ٲ��ش�. - ����Ŀ ���
void CCrywolf::SetCrywolfAllCommonMonsterState( int iMonsterState, int iMode )
{

	for( int i=0; i<m_ObjCommonMonster.m_iObjCount; i++ )
	{
		if( !gObjIsConnected(m_ObjCommonMonster.m_iObjIndex[i]) )
			return;

		LPOBJECTSTRUCT lpObj = &gObj[ m_ObjCommonMonster.m_iObjIndex[i] ];

		// 0 : ���� ����
		if( iMode == 0 )
		{
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, iMonsterState );
			GCStateInfoSend( lpObj, 0, lpObj->m_ViewSkillState );
#endif / MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		}
		// 1 : ���� ����
		else if( iMode == 1)
		{
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
			ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, iMonsterState );
			GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		}
		
	}
	
}


// ũ���̿��� �Ϲ� ���͸� ���� �Ѵ�.
void CCrywolf::RemoveCrywolfCommonMonster()
{
	for( int i=0; i<m_ObjCommonMonster.m_iObjCount; i++ )
	{
		gObjDel(m_ObjCommonMonster.m_iObjIndex[i]);
	}

	m_ObjCommonMonster.Reset();
}


// ũ���̿��� �Ϲ� ���͸� ���� �Ѵ�.
void CCrywolf::CreateCrywolfCommonMonster()
{
	for( int n=0; n<gMSetBase.m_Count; n++)
	{
		if( CHECK_CRYWOLF_FIRSTZONE(gMSetBase.m_Mp[n].m_MapNumber) )
		{
			// NPC ����
			if( gMSetBase.m_Mp[n].m_Type >= 204 && gMSetBase.m_Mp[n].m_Type <= 257) 
				continue;

			int result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);
			
			if( result >= 0 )
			{
				if( !gObjSetPosMonster(result, n) )
				{
					gObjDel(result);
					continue;
				}

				if( !gObjSetMonster(result, gMSetBase.m_Mp[n].m_Type) )
				{
					gObjDel(result);
					continue;
				}
				
				// ũ���̿��� �Ϲ� MONSTER�� ����Ѵ�.
				if( gObj[result].Type == OBJTYPE_MONSTER )
					g_Crywolf.m_ObjCommonMonster.AddObj( result );
			}
		}
	}

}


// ���� �׷��� AI�� �����Ѵ�.
void CCrywolf::ChangeAI( int iAIOrder )
{
	for( int n=0; n<m_iMonsterGroupNumberCount; n++ )
	{
		TMonsterAIGroup::ChangeAIOrder(m_iMonsterGroupNumberArray[n], iAIOrder);
	}
}


// �߰��� ������Ű��, AI�� �����Ѵ�.
void CCrywolf::TurnUpBoss()
{
	
	TMonsterAIGroup::Init( m_iBossGroupNumber );
	TMonsterAIGroup::ChangeAIOrder( m_iBossGroupNumber, MAG_AI_ORDER_FIRST );

	m_iBossIndex = TMonsterAIGroup::FindGroupLeader(m_iBossGroupNumber);
}

//-----------------------------------------------------------------------------------------------------

int	CCrywolf::CheckStateTimeSync()
{
	// 1. > ũ���̿��� ���� �� - ���� �ܰ躰 �ð� üũ 
	if( m_iCrywolfState != CRYWOLF_STATE_NONE )
	{
		if( !m_StateTimeInfo[m_iCrywolfState].m_bUsed )
			return _INVALID;

		DWORD dwCurrentTickCount = GetTickCount();
		
		// ���� ���� ���� ���� Ȯ��!
		if( !m_StateTimeInfo[m_iCrywolfState].CheckContinuanceTime() )
		{
			m_StateTimeInfo[m_iCrywolfState].ResetAppliedTime();
			
			if( m_iCrywolfState == CRYWOLF_STATE_ENDCYCLE )
			{
				// * �ݺ� ����� ��� - ũ���̿��� �ֱ��� ó������ ����.
				if( m_iScheduleMode == CRYWOLF_SCHEDULE_MODE_ITERATION )
				{
					SetState( CRYWOLF_STATE_NOTIFY_1 );
				}
				// * �Ϲ� ����� ��� - ũ���̿��� �ֱ⸦ ������.
				else
				{
					SetState( CRYWOLF_STATE_NONE );
				}
			}
			else
			{
				SetNextState( m_iCrywolfState );
			}

		}

		return _SUCCESS;
	}
	
	// 2. > ũ���̿��� ���� �� - ���� �ð� üũ
	for( int i=0; i<m_StartTimeInfoCount; i++ )
	{
		if( m_StartTimeInfo[i].CheckScheduleTime() )
		{
			LogAddTD("[ Crywolf ] Start Crywolf");
			SetState( m_StartTimeInfo[i].m_iStateNumber );
		}
	}
	
	return 0;
}

//-----------------------------------------------------------------------------------------------------

void CCrywolf::Run()
{
	// 1. > ũ���̿��� MVP Ȱ�� ���� üũ
	if ( !g_CrywolfSync.CheckEnableCrywolf() )		 
		return;

	// 2. > ũ���̿��� ���� �ð� �ε� ���� üũ
	if( !m_StartTimeInfo[0].m_bUsed )
		return;

	// 3. > ũ���̿��� ���� ���º� �ð� ����ȭ üũ
	CheckStateTimeSync();	

	// 4. > ũ���̿��� ���� ó��
	switch( m_iCrywolfState )
	{
	case CRYWOLF_STATE_NONE		:
		ProcState_NONE();
		break;

	case CRYWOLF_STATE_NOTIFY_1	:
		ProcState_NOTIFY_1();
		break;

	case CRYWOLF_STATE_NOTIFY_2	:
		ProcState_NOTIFY_2();
		break;

	case CRYWOLF_STATE_READY	:
		ProcState_READY();
		break;

	case CRYWOLF_STATE_START	:
		ProcState_START();
		break;

	case CRYWOLF_STATE_END		:
		ProcState_END();
		break;

	case CRYWOLF_STATE_ENDCYCLE :
		ProcState_ENDCYCLE();
		break;
	};

}


//-----------------------------------------------------------------------------------------------------

void CCrywolf::SetState( int iCrywolfState )
{
	switch( iCrywolfState )
	{
	case CRYWOLF_STATE_NONE		:
		SetState_NONE();
		break;

	case CRYWOLF_STATE_NOTIFY_1	:
		SetState_NOTIFY_1();
		break;

	case CRYWOLF_STATE_NOTIFY_2	:
		SetState_NOTIFY_2();
		break;

	case CRYWOLF_STATE_READY	:
		SetState_READY();
		break;

	case CRYWOLF_STATE_START	:
		SetState_START();
		break;

	case CRYWOLF_STATE_END		:
		SetState_END();
		break;

	case CRYWOLF_STATE_ENDCYCLE :
		SetState_ENDCYCLE();
		break;
	}
	
	// ũ���̿��� ����/���� ���� ���¸� �����鿡�� �˸���.
	NotifyCrywolfCurrentState();

	// ũ���̿��� ����/���� DB�� �����Ѵ�.
	CrywolfInfoDBSave();
}


void CCrywolf::SetNextState( int iCurrentState )
{
	int iNextState = iCurrentState + 1;
	SetState(iNextState);
}


void CCrywolf::SetState_NONE()
{
	LogAddC( LOGC_BLUE, "[ Crywolf ] State (%d) -> NONE", m_iCrywolfState);

	SetCrywolfState( CRYWOLF_STATE_NONE );
	SetCrywolfStateAppliedTime( CRYWOLF_STATE_NONE );
	
	// DB�� ���� ������ �����Ѵ�.
}


void CCrywolf::SetState_NOTIFY_1()
{
	// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
	UTIL.SendMapServerGroupMsg( lMsg.Get(3300) );	// "����� ���� ���� �߰����� ũ���̿��� ����� ħ���ϱ� ���� ������ �غ��ϰ� �ֽ��ϴ�."
	
	LogAddC( LOGC_BLUE, "[ Crywolf ] State (%d) -> NOTIFY_1", m_iCrywolfState );

	SetCrywolfState( CRYWOLF_STATE_NOTIFY_1 );
	SetCrywolfStateAppliedTime( CRYWOLF_STATE_NOTIFY_1 );

	m_dwCrywolfNotifyMsgStartTick = GetTickCount();

	// DB�� ���� ������ �����Ѵ�.

}


void CCrywolf::SetState_NOTIFY_2()
{
	// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
	UTIL.SendMapServerGroupMsg( lMsg.Get(3301) );		// "�߰����� ������ ũ���̿��� ����� ���� ����߽��ϴ�."

	LogAddC( LOGC_BLUE, "[ Crywolf ] State (%d) -> NOTIFY_2", m_iCrywolfState );

	SetCrywolfState( CRYWOLF_STATE_NOTIFY_2 );
	SetCrywolfStateAppliedTime( CRYWOLF_STATE_NOTIFY_2 );


	// 0. > ���� ���� ����
	SetOccupationState( CRYWOLF_OCCUPATION_STATE_WAR );

	// 1. > ũ���̿����� ��� NPC�� ö���Ѵ�. 
	//		- GameAllMonsterAdd() ��� ũ���̿��� ���� & NPC �� ����� ���´�.
	SetCrywolfCommonNPC( CRYWOLF_OCCUPATION_STATE_WAR );

	// 2. > ũ���̿��� �� ���¸� ���� ���·� �ٲ۴�.
	SetCrywolfMapAttr( CRYWOLF_OCCUPATION_STATE_WAR );

	// 3. > ũ���̿����� ��� ���͵��� ���� �Ѵ�.
	RemoveCrywolfCommonMonster();

	m_dwCrywolfNotifyMsgStartTick = GetTickCount();
	// DB�� ���� ������ �����Ѵ�.

}


void CCrywolf::SetState_READY()
{
	// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
	UTIL.SendMapServerGroupMsg( lMsg.Get(3302) );			// "�߰����� ������ ũ���̿��� ����� �����ϱ� ���� �غ� �ϰ� �ֽ��ϴ�."

	LogAddC( LOGC_BLUE, "[ Crywolf ] State (%d) -> READY", m_iCrywolfState );

	SetCrywolfState( CRYWOLF_STATE_READY );
	SetCrywolfStateAppliedTime( CRYWOLF_STATE_READY );

	// 0. > ���� ���� ���� 
	SetOccupationState( CRYWOLF_OCCUPATION_STATE_WAR );

	// 1. > ũ���̿����� ��� NPC�� ö���Ѵ�. 
	//		- GameAllMonsterAdd() ��� ũ���̿��� ���� & NPC �� ����� ���´�.
	SetCrywolfCommonNPC( CRYWOLF_OCCUPATION_STATE_WAR );

	// 2. > ũ���̿��� �� ���¸� ���� ���·� �ٲ۴�.
	SetCrywolfMapAttr( CRYWOLF_OCCUPATION_STATE_WAR );

	// 3. > ũ���̿����� ��� ���͵��� ���� �Ѵ�.
	RemoveCrywolfCommonMonster();

	// 4. > ������ ������ ObjIndex�� �����Ѵ�.
	g_CrywolfNPC_Altar.SetAllAltarObjectIndex();

	// 5. > ������ ���� ViewState ����
	g_CrywolfNPC_Altar.SetAllAltarViewState(STATE_CRYWOLF_ALTAR_ENABLE);


	// 6. > Dummy AI����� ���͸� ���� ���� ���´�.
	for( int n=0; n<m_iMonsterGroupNumberCount; n++ )
	{
		TMonsterAIGroup::Init(m_iMonsterGroupNumberArray[n]);
	}
	
	m_dwCrywolfNotifyMsgStartTick = GetTickCount();
	
	// 6. > ���� MVP Score�� �ʱ�ȭ�Ѵ�
	ResetAllUserMVPScore();
	
	// DB�� ���� ������ �����Ѵ�.
	

}


void CCrywolf::SetState_START()
{
	// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
	UTIL.SendMapServerGroupMsg( lMsg.Get(3303) );			// "ũ���̿��� ����� ���� �߰����� ������ ���� �ƽ��ϴ�."

	LogAddC( LOGC_BLUE, "[ Crywolf ] State (%d) -> START", m_iCrywolfState );

	SetCrywolfState( CRYWOLF_STATE_START );
	SetCrywolfStateAppliedTime( CRYWOLF_STATE_START );

	// 0 . > ���� ��� ���¸� �˻��Ѵ�.
	if( g_CrywolfNPC_Altar.GetContractedAltarCount() == _ZERO )
	{
		// ũ���̿��� ����..! ���� ������.
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3304) );		// "���� �Ż��� ��ȣ�ϴ� ������ ���� �Ż��� ������ϴ�. "
		SetOccupationState( CRYWOLF_OCCUPATION_STATE_OCCUPIED );
		SetState( CRYWOLF_STATE_END );
		return;
	}

	// 1 . > ũ���̿��� ���� �� ��ũ ������ ���� 6�ݰ� ĳ���͸� �ڷ� �δ�!
	for( int n=0; n<m_iMonsterGroupNumberCount; n++ )
	{
		int iLeaderIndex = TMonsterAIGroup::FindGroupLeader(m_iMonsterGroupNumberArray[n]);

		if( !CHECK_LIMIT(iLeaderIndex,MAX_MONSTER) )
			continue;

		if( gObj[iLeaderIndex].Class != CRYWOLF_MON_DARKELF )
			continue;

		LPOBJECTSTRUCT lpObj = &gObj[iLeaderIndex];

		for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++ )
		{
			if( lpObj->VpPlayer2[i].state )
			{
				int iTargetNumber = lpObj->VpPlayer2[i].number;
				if( CHECK_LIMIT(iTargetNumber, MAX_OBJECT) 
					&& gObj[iTargetNumber].Type == OBJTYPE_CHARACTER )
				{
					gObjBackSpring2(&gObj[iTargetNumber], lpObj, 3);
				}
			}
		}

	}

	// AI�� ���� �Ѵ�.

	ChangeAI( MAG_AI_ORDER_FIRST );

	m_dwCrywolfStartProcTick	= GetTickCount();
	m_bTurnUpBoss				= FALSE;
	m_bChangeAI					= FALSE;

	// DB�� ���� ������ �����Ѵ�.
	
}


void CCrywolf::SetState_END()
{
	m_dwCrywolfStartProcTick	= _ZERO;
	m_bChangeAI					= FALSE;
	
	LogAddC( LOGC_BLUE, "[ Crywolf ] State (%d) -> END", m_iCrywolfState );

	// 1. > ���� / ���� ��ó��
	//  - �߰����� ��� ������ ����!
	if( m_bTurnUpBoss && CHECK_LIMIT(m_iBossIndex, MAX_MONSTER) )
	{
		if( gObj[m_iBossIndex].Live )
		{
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3305) );			// "�߰����� �����ð� ������� ��Ƴ��ҽ��ϴ�."
			SetOccupationState( CRYWOLF_OCCUPATION_STATE_OCCUPIED );
		}
	}
	
	m_bTurnUpBoss	= FALSE;
	SetCrywolfBossIndex(_INVALID);
	
	if( GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		UTIL.SendMapServerGroupMsg( lMsg.Get(3306) );			// "MVP �й�! �߰������� ������ �Ż��� ������ϴ�."
		SetOccupationState( CRYWOLF_OCCUPATION_STATE_OCCUPIED );
		LogAddTD( "[ Crywolf ][MVP] FAIL.......!!" );
	}
	else
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		UTIL.SendMapServerGroupMsg( lMsg.Get(3307) );			// "MVP �¸�! �߰����� ���� ������ �Ż��� ���׽��ϴ�."
		SetOccupationState( CRYWOLF_OCCUPATION_STATE_PEACE );
		LogAddTD( "[ Crywolf ][MVP] SUCCESS.......!!" );
	}

	// 2. > ���� ����
	SetCrywolfState( CRYWOLF_STATE_END );
	SetCrywolfStateAppliedTime( CRYWOLF_STATE_END );
	NotifyCrywolfCurrentState();
	
	// 4. > ���͸� �����Ѵ�.
	ChangeAI(MAG_AI_ORDER_DEFAULT);
	
	// 3. > ���� ó�� : ���� ����ڿ� ���ؼ� Score�� ����Ѵ�.
	for( int iAltar=CRYWOLF_NPC_ALTAR1; iAltar<=CRYWOLF_NPC_ALTAR5; iAltar++ )
	{
		int iAltarUserIndex = g_CrywolfNPC_Altar.GetAltarUserIndex( iAltar );
		
#ifdef ADD_CRYWOLF_SUCCESS_REWARD_20060404	// ���� ��� ������ ���� ������ ���� - Ư��
		if( iAltarUserIndex != _INVALID )
		{
			CalcGettingScore( iAltarUserIndex, _INVALID, CRYWOLF_SCORE_LAST_CONTRACTOR );

			if( GetOccupationState() != CRYWOLF_OCCUPATION_STATE_OCCUPIED )
			{			
				MakeRewardForAltarElf( iAltarUserIndex );
			}	
		}
#else
		if( iAltarUserIndex != _INVALID )
			CalcGettingScore( iAltarUserIndex, _INVALID, CRYWOLF_SCORE_LAST_CONTRACTOR );		
#endif

	}

	// 5. > ���� ���� �ʱ�ȭ
	g_CrywolfNPC_Altar.ResetAllAltar();

	// 3. > ���� ó��
	// - ������ Rank + Score �� ������
	NotifyCrywolfPersonalRank();
	

	// DB�� ���� ������ �����Ѵ�.
	
}


void CCrywolf::SetState_ENDCYCLE()
{
	LogAddC( LOGC_BLUE, "[ Crywolf ] State (%d) -> ENDCYCLE", m_iCrywolfState );
	
	SetCrywolfState( CRYWOLF_STATE_ENDCYCLE );
	SetCrywolfStateAppliedTime( CRYWOLF_STATE_ENDCYCLE );

	// 1. > ������ ���� NPC ���� �� �������� ����
	// �Ϲ� ���Ͱ� ���� ���뿡 ���� ���� �� �����Ƿ� ��ü ����
	SetCrywolfMapAttr( GetOccupationState() );
	SetCrywolfCommonNPC( GetOccupationState() );
	
	// 3. > MVP ���� ���� 
	for( int n=0; n<m_iMonsterGroupNumberCount; n++ )
	{
		TMonsterAIGroup::DelGroupInstance(m_iMonsterGroupNumberArray[n]);
	}

	TMonsterAIGroup::DelGroupInstance(m_iBossGroupNumber);


	// 4. > ũ���̿��� �Ϲ� ���͸� ���� ��Ų��.
	CreateCrywolfCommonMonster();

	// 5. > ���� ��ó��
	// - 1~10�� ��ŷ�� ������.
	NotifyCrywolfHeroList();

	// 6. > ���� MVP Score�� �ʱ�ȭ�Ѵ�
	ResetAllUserMVPScore();

	// 2. > DB�� ���� ������ �����Ѵ�.
	
}


//-----------------------------------------------------------------------------------------------------


void CCrywolf::ProcState_NONE()
{
//	LogAddC( LOGC_BLUE, "[ Crywolf ] Proc - NONE" );
	
	// DB���� �ֱ������� ũ���̿��� ���� ������ �о�´�.

}


void CCrywolf::ProcState_NOTIFY_1()
{
//	LogAddC( LOGC_BLUE, "[ Crywolf ] Proc - NOTIFY_1" );
	if( GetTickCount() - m_dwCrywolfNotifyMsgStartTick > 70*1000 )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		UTIL.SendMapServerGroupMsg( lMsg.Get(3300) );			// "����� ���� ���� �߰����� ũ���̿��� ����� ħ���ϱ� ���� ������ �غ��ϰ� �ֽ��ϴ�."
		m_dwCrywolfNotifyMsgStartTick = GetTickCount();
	}

}


void CCrywolf::ProcState_NOTIFY_2()
{
//	LogAddC( LOGC_BLUE, "[ Crywolf ] Proc - NOTIFY_2" );
	if( GetTickCount() - m_dwCrywolfNotifyMsgStartTick > 70*1000 )
	{
		int iTime = (m_StateTimeInfo[GetCrywolfState()].GetLeftTime() / 1000) / 60 ;

		if( iTime == _ZERO )
		{
			iTime = (m_StateTimeInfo[GetCrywolfState()].GetLeftTime() / 1000) ;
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			UTIL.SendMapServerGroupMsg( lMsg.Get(3308) , iTime );			// "%d �� �� ������ ���ܰ� ��� �� �� �ֽ��ϴ�."
		}
		else
		{
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			UTIL.SendMapServerGroupMsg( lMsg.Get(3309) , iTime );			// "%d �� �� ������ ���ܰ� ��� �� �� �ֽ��ϴ�."
		}
		
		m_dwCrywolfNotifyMsgStartTick = GetTickCount();
	}
}


void CCrywolf::ProcState_READY()
{
//	LogAddC( LOGC_BLUE, "[ Crywolf ] Proc - READY" );

	// 2 sec Cycling..
	NotifyCrywolfStatueAndAltarInfo();

	if( GetTickCount() - m_dwCrywolfNotifyMsgStartTick > 70*1000 )
	{
		int iTime = (m_StateTimeInfo[GetCrywolfState()].GetLeftTime() / 1000) / 60 ;

		if( iTime == _ZERO )
		{
			iTime = (m_StateTimeInfo[GetCrywolfState()].GetLeftTime() / 1000) ;
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			UTIL.SendMapServerGroupMsg( lMsg.Get(3310), iTime );			// "%d �� �� �߰����� ������ ħ�� �մϴ�."
		}
		else
		{
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			UTIL.SendMapServerGroupMsg( lMsg.Get(3311), iTime );			// "%d �� �� �߰����� ������ ħ�� �մϴ�."
		}
		
		m_dwCrywolfNotifyMsgStartTick = GetTickCount();
	}
}


void CCrywolf::ProcState_START()
{
//	LogAddC( LOGC_BLUE, "[ Crywolf ] Proc - START" );


	// 0. > ���̵� ���� ���� üũ : ���� ��� ���¸� üũ�Ѵ�.
	if( g_CrywolfNPC_Altar.GetContractedAltarCount() == _ZERO )
	{
		// ũ���̿��� ����..! ���� ������.
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3312) );			// "������ �Ż��� ��ȣ�ϴ� ������ ���� �Ż��� ������ϴ�. "
		SetOccupationState( CRYWOLF_OCCUPATION_STATE_OCCUPIED );
		SetState( CRYWOLF_STATE_END );
		return;
	}

	
	// 1. > ũ���̿��� ���� ���� ������ �˸���.

	// 1. 1 > �Ż�, ���� ���� ������ �˸���.
	// 2 sec Cycling..
	NotifyCrywolfStatueAndAltarInfo();

	// 1. 2 > �߰����� ��ũ������ ������ �˸���.
	// 5 sec Cycling..
	NotifyCrywolfBossMonsterInfo();

	// 1. 3 > ������ ���� �ð��� �˸���.	
	// 10 sec Cycling..
	NotifyCrywolfStateLeftTime();


	// 2. > ���͸� ��Ʈ�� �Ѵ�.
	
	// 2. 1 > �߰��� ����
	if( ( (GetTickCount()-m_dwCrywolfStartProcTick) > m_iBossTurnUpTime*1000) && m_bTurnUpBoss == FALSE )
	{
		m_bTurnUpBoss = TRUE;
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(3313) );			// "������ �߰����� �����߽��ϴ�. �Ż��� �����մϴ�."
		TurnUpBoss();

		LogAddTD("[ Crywolf ][MVP] Barlgars TurnUp !!!");
	}

	// 2. 2 > ���� �׷� AI ����
	if( ( (GetTickCount()-m_dwCrywolfStartProcTick) > m_iMonsterGroupChangeAITime*1000) && m_bChangeAI == FALSE )
	{
		m_bChangeAI = TRUE;
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(3314) );			// "�߰����� ������ ������ �Ż����� ������ �����մϴ�!"
		ChangeAI( MAG_AI_ORDER_SECOND );

		LogAddTD("[ Crywolf ][MVP] Start Monster Rush !!!");
	}

	
	// 3. > ���̵� ���� ���� üũ : �߰����� �׾����� üũ
	if( m_bTurnUpBoss && CHECK_LIMIT(m_iBossIndex, MAX_MONSTER) )
	{
		if( !gObj[m_iBossIndex].Live )
		{
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3315) );			// "������ �������� �߰����� �����߽��ϴ�."
			SetOccupationState( CRYWOLF_OCCUPATION_STATE_PEACE );
			SetState( CRYWOLF_STATE_END );
			return;
		}
	}

}


void CCrywolf::ProcState_END()
{
//	LogAddC( LOGC_BLUE, "[ Crywolf ] Proc - END" );

}


void CCrywolf::ProcState_ENDCYCLE()
{
//	LogAddC( LOGC_BLUE, "[ Crywolf ] Proc - ENDCYCLE" );

}


//-----------------------------------------------------------------------------------------------------


void CCrywolf::NotifyCrywolfCurrentState()
{
	// ���� ũ���̿��� ���¸� �˸���.
	PMSG_ANS_CRYWOLF_INFO	pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x00, sizeof( pMsg) );

	pMsg.btOccupationState	= GetOccupationState();
	pMsg.btCrywolfState		= GetCrywolfState();

	UTIL.SendCrywolfUserAnyData( (LPBYTE)&pMsg, sizeof(pMsg) );

	LogAddC( LOGC_GREEN, "[ Crywolf ] SetState - StateInfo : Occupation/%d, State/%d", pMsg.btOccupationState, pMsg.btCrywolfState );
}


void CCrywolf::NotifyCrywolfStateLeftTime()
{
	// ũ���̿����� ������ ���� �ð��� �˸���.	
	static DWORD dwNotificationTimeTick = 0;

	// Cycle Time :	20 sec
	if( GetTickCount() - dwNotificationTimeTick < 20000 )
		return;

	dwNotificationTimeTick = GetTickCount();
	
	PMSG_ANS_CRYWOLF_LEFTTIME	pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x04, sizeof( pMsg) );

	pMsg.btHour				= (m_StateTimeInfo[GetCrywolfState()].GetLeftTime() / 1000) / 60 / 24;
	pMsg.btMinute			= (m_StateTimeInfo[GetCrywolfState()].GetLeftTime() / 1000) / 60 ;
	

	UTIL.SendCrywolfUserAnyData( (LPBYTE)&pMsg, sizeof(pMsg) );

	LogAddC( LOGC_GREEN, "[ Crywolf ] War LeftTime - (%02d:%02d)", pMsg.btHour, pMsg.btMinute );		
}


void CCrywolf::NotifyCrywolfStatueAndAltarInfo()
{
	static DWORD dwNotificationTimeTick = 0;

	// Cycle Time :	2 sec
	if( GetTickCount() - dwNotificationTimeTick < 2000 )
		return;

	dwNotificationTimeTick = GetTickCount();
	
	// �Ż� ��, ������ ������ �ֱ������� Notification �Ѵ�.
	PMSG_ANS_CRYWOLF_STATUE_ALTAR_INFO pMsg;
	PHeadSubSetB( (LPBYTE)&pMsg, 0xBD, 0x02, sizeof(pMsg) );
	
	pMsg.iCrywolfStatueHP	= g_CrywolfNPC_Statue.m_Shield.GetHPPercentage();

	// ���� 4bit : ���� ��� ���� Ƚ��
	pMsg.btAltarState1		= g_CrywolfNPC_Altar.GetAltarRemainContractCount( CRYWOLF_NPC_ALTAR1 );
	pMsg.btAltarState2		= g_CrywolfNPC_Altar.GetAltarRemainContractCount( CRYWOLF_NPC_ALTAR2 );
	pMsg.btAltarState3		= g_CrywolfNPC_Altar.GetAltarRemainContractCount( CRYWOLF_NPC_ALTAR3 );
	pMsg.btAltarState4		= g_CrywolfNPC_Altar.GetAltarRemainContractCount( CRYWOLF_NPC_ALTAR4 );
	pMsg.btAltarState5		= g_CrywolfNPC_Altar.GetAltarRemainContractCount( CRYWOLF_NPC_ALTAR5 );

	// ���� 4bit : ���� ����
	pMsg.btAltarState1		|= g_CrywolfNPC_Altar.GetAltarState( CRYWOLF_NPC_ALTAR1 ) << 4;
	pMsg.btAltarState2		|= g_CrywolfNPC_Altar.GetAltarState( CRYWOLF_NPC_ALTAR2 ) << 4;
	pMsg.btAltarState3		|= g_CrywolfNPC_Altar.GetAltarState( CRYWOLF_NPC_ALTAR3 ) << 4;
	pMsg.btAltarState4		|= g_CrywolfNPC_Altar.GetAltarState( CRYWOLF_NPC_ALTAR4 ) << 4;
	pMsg.btAltarState5		|= g_CrywolfNPC_Altar.GetAltarState( CRYWOLF_NPC_ALTAR5 ) << 4;

	
	UTIL.SendCrywolfUserAnyData( (LPBYTE)&pMsg, sizeof(pMsg) );

	LogAddTD( "[ Crywolf ][Statue Info] Statue HP : %d", pMsg.iCrywolfStatueHP );

	for( int iAltar=CRYWOLF_NPC_ALTAR1; iAltar<=CRYWOLF_NPC_ALTAR5; iAltar++ )
	{
		int iAltarUserIndex = g_CrywolfNPC_Altar.GetAltarUserIndex( iAltar );

		if( iAltarUserIndex != _INVALID )
		{
			LogAddTD( "[ Crywolf ][Altar Info] Altar(%d) Contractor: [%s][%s] ", 
				iAltar, gObj[iAltarUserIndex].AccountID, gObj[iAltarUserIndex].Name );
		}
	}
}


void CCrywolf::NotifyCrywolfBossMonsterInfo()
{
	static DWORD dwNotificationTimeTick = 0;

	// Cycle Time :	5 sec
	if( GetTickCount() - dwNotificationTimeTick < 5000 )
		return;

	dwNotificationTimeTick = GetTickCount();

	PMSG_ANS_CRYWOLF_BOSSMONSTER_INFO pMsg;
	PHeadSubSetB( (LPBYTE)&pMsg, 0xBD, 0x05, sizeof(pMsg) );

	pMsg.iMonster1HP	= _INVALID;	// �߰��� HP
	pMsg.btMonster2		= _ZERO;	// ��ũ ������ ��
	
	for( int i=0; i<MAX_MONSTER; i++ )
	{
		if( !gObjIsConnected(i) )
			continue;

		if( gObj[i].MapNumber != MAP_INDEX_CRYWOLF_FIRSTZONE )
			continue;

		if( !gObj[i].Live )
			continue;

		switch( gObj[i].Class )
		{
		case CRYWOLF_MON_DARKELF : 
			pMsg.btMonster2++ ;
			break;
		case CRYWOLF_MON_BALGARS :
			pMsg.iMonster1HP = gObj[i].Life * 100 / (gObj[i].MaxLife+1);
			break;
		}
	}
	
	UTIL.SendCrywolfUserAnyData( (LPBYTE)&pMsg, sizeof(pMsg) );

	LogAddTD( "[ Crywolf ][Boss Monster Info] Balgars HP:%d, DarkElf:%d", pMsg.iMonster1HP, pMsg.btMonster2 );
}

void CCrywolf::NotifyCrywolfStageEffectOnOff( BYTE btOnOff )
{
	PMSG_ANS_CRYWOLF_STAGE_EFFECT_ONOFF	pMsg = {0,};
	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x06, sizeof( pMsg) );

	pMsg.btStageEffectOnOff	= btOnOff;

	UTIL.SendCrywolfUserAnyData( (LPBYTE)&pMsg, sizeof(pMsg) );
}

void CCrywolf::NotifyCrywolfPersonalRank()
{	
	PMSG_ANS_CRYWOLF_PERSONAL_RANK pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x07, sizeof( pMsg) );

	pMsg.btRank = 0;

	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == CSS_GAMEPLAYING ) 
			&& (gObj[i].Type == OBJTYPE_CHARACTER) 
			&& (gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
		  )
		{
			// ���� ��ũ & ���� ����ġ ���
			pMsg.btRank			= CalcGettingRank(i );
			pMsg.iGettingExp	= CalcGettingRewardExp( i, pMsg.btRank );

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	#ifdef MODIFY_MARKITEM_EFFECT_USE_CRYWOLF_REWARD_20070502
			if( gObj[i].m_wExprienceRate == 0 )
			{
				pMsg.iGettingExp = 0;
			}
	#endif // MODIFY_MARKITEM_EFFECT_USE_CRYWOLF_REWARD_20070502
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			
			// ����ġ�� �ش�.
			GiveRewardExp( i, pMsg.iGettingExp );

			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			MsgOutput( i, lMsg.Get(3316) ,		pMsg.btRank );				// "* MVP ��ũ [%d]"
			MsgOutput( i, lMsg.Get(3317) ,	pMsg.iGettingExp );				// "* MVP ��ũ ����ġ [%d]"

			LogAddTD( "[ Crywolf ][Personal Rank & Exp.] [%s][%s] Score(%d) Rank(%d) Exp(%d)",
				gObj[i].AccountID, gObj[i].Name, gObj[i].m_iCrywolfMVPScore, pMsg.btRank, pMsg.iGettingExp );

			DataSend(i, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
}

void CCrywolf::NotifyCrywolfHeroList()
{
	CHAR cBUFFER [MAX_BUFF_SIZE];
	INT	iHeroCount = 0;
	
	ZeroMemory( cBUFFER, MAX_BUFF_SIZE );
	
	LPPMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT	lpMsg		= (LPPMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT) cBUFFER;
	LPPMSG_ANS_CRYWOLF_HERO_LIST_INFO		lpMsgBody	= (LPPMSG_ANS_CRYWOLF_HERO_LIST_INFO) ( cBUFFER + sizeof(PMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT) );
	
	// 1. > Hero Set �� �����.
	set< LPOBJECTSTRUCT, CCrywolfScoreSort > HeroSet;

	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == CSS_GAMEPLAYING ) 
			&& (gObj[i].Type == OBJTYPE_CHARACTER) 
			&& (gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
			)
		{
			HeroSet.insert(&gObj[i]);
		}
	}

	// 2. > Hero �� �����Ѵ�.
	set< LPOBJECTSTRUCT, CCrywolfScoreSort >::iterator	_Itor = HeroSet.begin();

	for( int j=0; j<MAX_CRYWOLF_MVP_HERO && _Itor!=HeroSet.end() ; j++, _Itor++ )
	{
		LPOBJECTSTRUCT lpHeroObj = (LPOBJECTSTRUCT)*_Itor;
		lpMsgBody[j].iRank			= iHeroCount;
		lpMsgBody[j].btHeroClass	= lpHeroObj->Class;
		lpMsgBody[j].iHeroScore		= lpHeroObj->m_iCrywolfMVPScore;
		CopyMemory( lpMsgBody[j].szHeroName, lpHeroObj->Name, MAX_IDSTRING );
		iHeroCount++;

#ifdef MODIFY_CRYWOLF_HEROLIST_LOG_BUGFIX_20060828
		LogAddTD( "[ Crywolf ][Hero List] [%s][%s] Score(%d) Ranking(%d)",
				lpHeroObj->AccountID, lpHeroObj->Name, lpHeroObj->m_iCrywolfMVPScore, iHeroCount );
#else
		LogAddTD( "[ Crywolf ][Hero List] [%s][%s] Score(%d) Ranking(%d)",
				lpHeroObj->AccountID, lpHeroObj->Name, gObj[i].m_iCrywolfMVPScore, iHeroCount );
#endif // MODIFY_CRYWOLF_HEROLIST_LOG_BUGFIX_20060828

#ifdef ADD_CRYWOLF_SUCCESS_REWARD_20060404
		// �������� ������ ��� 1 ~ 5������ Ư�� ������ ����
		if( GetOccupationState() != CRYWOLF_OCCUPATION_STATE_OCCUPIED )
		{			
			if( lpMsgBody[j].iRank <= 5 )
			{
				MakeRewardForHeroListTop5( lpHeroObj->m_Index );
			}
		}
#endif

	}

	lpMsg->btCount = iHeroCount;
	PHeadSubSetB((LPBYTE)lpMsg, 0xBD, 0x08, sizeof(PMSG_ANS_CRYWOLF_HERO_LIST_INFO_COUNT) + (sizeof(PMSG_ANS_CRYWOLF_HERO_LIST_INFO) * iHeroCount) );
	
	UTIL.SendCrywolfUserAnyData( (LPBYTE)lpMsg, lpMsg->h.size );
}

//-----------------------------------------------------------------------------------------------------

void CCrywolf::OperateGmCommand( int iUserIndex, int iCommand )
{
	if (!gObjIsConnected(iUserIndex))
		return;

#if TESTSERVER != 1
	// ��ڰ� �ƴϸ� ����� �����Ѵ�.
	if( (gObj[iUserIndex].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
		return;

	LogAddTD( "[ Crywolf ][Use GM Command] [%s][%s] Command : %d", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, iCommand );
#endif
	
	switch( iCommand )
	{
	case CRYWOLF_GM_COMMAND_NOTIFY_2 :
		SetState( CRYWOLF_STATE_NOTIFY_2 );
		break;
	case CRYWOLF_GM_COMMAND_READY :
		SetState( CRYWOLF_STATE_READY );
		break;
	case CRYWOLF_GM_COMMAND_START :
		if( GetCrywolfState() == CRYWOLF_STATE_READY )
			SetState( CRYWOLF_STATE_START );
		else
		{
			// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
			MsgOutput( iUserIndex, lMsg.Get(3318) );			// "/ũ���̿����غ� �� �̿��� �غ� ���¸� ���弼��!"
		}
		break;
	case CRYWOLF_GM_COMMAND_END :
		SetState( CRYWOLF_STATE_END );
		break;
	}
	
}


//-----------------------------------------------------------------------------------------------------

void CCrywolf::CrywolfServerGroupSync()
{
	GDReqCrywolfSync( g_MapServerManager.GetMapSvrGroup(), g_Crywolf.GetCrywolfState(), g_Crywolf.GetOccupationState() );
}

void CCrywolf::CrywolfInfoDBSave()
{
	// DB �����Ͱ� Load �Ǿ� �־�߸� Save �Ҽ� �ִ�.
	if( m_bDBDataLoadOK )
		GDReqCrywolfInfoSave( g_MapServerManager.GetMapSvrGroup(), GetCrywolfState(), GetOccupationState() );
}

void CCrywolf::CrywolfInfoDBLoad()
{
	m_bDBDataLoading = TRUE;
	GDReqCrywolfInfoLoad( g_MapServerManager.GetMapSvrGroup() );
}

void CCrywolf::ApplyCrywolfDBInfo( int iCrywolfState, int iOccupationState )
{
	// ��ü ���͸� Reload �ؼ� ������ ���� �� �� �ֵ��� �Ѵ�.
	SetDBDataLoad( TRUE );
	SetOccupationState( iOccupationState );

//	GameMonsterAllCloseAndReLoad();
	
	SetCrywolfCommonNPC( GetOccupationState() );
	SetCrywolfMapAttr( GetOccupationState() );

	LogAddTD( "[ Crywolf ][ ApplyDBInfo] OccupationState : %d", iOccupationState );

}

void CCrywolf::CrywolfMonsterDieProc( int iMonIndex, int iKillerIndex )
{
	LPOBJECTSTRUCT lpMonObj		= &gObj[iMonIndex];
	LPOBJECTSTRUCT lpKillerObj	= &gObj[iKillerIndex];
	
	BOOL bExistKiller = gObjIsConnected(iKillerIndex);
	
	if( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_START 
		&& lpMonObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
	{
		if( lpMonObj->m_iCurrentAI )
		{
			if( lpMonObj->Class == CRYWOLF_MON_BALGARS )
			{
				if( bExistKiller )
				{
					// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
					UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(3319) , lpKillerObj->Name);			// "�߰����� [%s] ���� �׿����ϴ�."
					LogAddTD( "[ Crywolf ][MVP] [Balgars Dead] by [%s][%s]", lpKillerObj->AccountID, lpKillerObj->Name );
				}
				else
				{
					// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
					UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(3320) );			// "�߰����� �׾����ϴ�."
					LogAddTD( "[ Crywolf ][MVP] [Balgars Dead] by Unknown User" );
				}
			}
			if( lpMonObj->Class == CRYWOLF_MON_DARKELF )
			{
				if( bExistKiller )
				{
					// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
					UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(3321) , lpMonObj->m_iGroupNumber, lpKillerObj->Name );			// "�߰��������� [%d] �д��� ��ũ������ [%s] ���� �׿����ϴ�."
					LogAddTD( "[ Crywolf ][MVP] [DarkElf Dead] by [%s][%s]", lpKillerObj->AccountID, lpKillerObj->Name );
				}
			}


			// MVP ���ھ ����Ѵ�.
			int iMVPScore = CalcGettingScore( iKillerIndex, iMonIndex, CRYWOLF_SCORE_MONSTERKILL );

			if( iMVPScore != _INVALID )
			{
				// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
				MsgOutput( iKillerIndex, lMsg.Get(3322) , iMVPScore);			// "���� MVP ������ %d �Դϴ�."
			}

		}
	}
}

#ifdef ADD_CRYWOLF_SUCCESS_REWARD_20060404
void CCrywolf::MakeRewardForAltarElf( int iAltarUserIndex )
{	 
	int iItemNumber = MAKE_ITEMNUM( 14, 13 );	// �ູ�� ����	

	ItemSerialCreateSend( gObj[iAltarUserIndex].m_Index, gObj[iAltarUserIndex].MapNumber, ( BYTE )gObj[iAltarUserIndex].X, ( BYTE )gObj[iAltarUserIndex].Y, iItemNumber, 0, 0, 0, 0, 0, iAltarUserIndex, 0 );	

	LogAddTD( "[ Crywolf ][Reward] [Altar Elf] [%s][%s] Success Reward - Gem Of Bless", 
												gObj[iAltarUserIndex].AccountID, gObj[iAltarUserIndex].Name );

}
#endif	// #ifdef ADD_CRYWOLF_SUCCESS_REWARD_20060404

#ifdef ADD_CRYWOLF_SUCCESS_REWARD_20060404
void CCrywolf::MakeRewardForHeroListTop5( int iUserIndex )
{
	int iItemNumber = MAKE_ITEMNUM( 14, 13 );	// �ູ�� ����

	ItemSerialCreateSend( gObj[iUserIndex].m_Index, gObj[iUserIndex].MapNumber, 
		( BYTE )gObj[iUserIndex].X, ( BYTE )gObj[iUserIndex].Y, iItemNumber, 0, 0, 0, 0, 0, iUserIndex, 0 );				

	LogAddTD( "[ Crywolf ][Reward] [Hero List Top 5] [%s][%s] Success Reward - Gem Of Bless", 
												gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );
}
#endif

int CCrywolf::CalcGettingScore( int iUserIndex, int iMonIndex, int iScoreType )
{
	if( !gObjIsConnected(iUserIndex) )
		return _INVALID;

	int iAddMVPScore = _ZERO;

	if( CHECK_LIMIT(iMonIndex, MAX_MONSTER) )
	{
		switch( gObj[iMonIndex].Class )
		{
		case CRYWOLF_MON_BALGARS	:
			iAddMVPScore = m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_BALGARS];
			break;
		case CRYWOLF_MON_DARKELF	:		
			iAddMVPScore = m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_DARKELF];
			break;
		case CRYWOLF_MON_BALRAM		:		
			iAddMVPScore = m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_BALRAM];
			break;
		case CRYWOLF_MON_SORAM 		:
			iAddMVPScore = m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_SORAM];
			break;
		case CRYWOLF_MON_DEATHSPIRIT:	
			iAddMVPScore = m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_DEATHSPRIT];
			break;
		case CRYWOLF_MON_TANKER		:		
			iAddMVPScore = m_iMVPScoreTable[CRYWOLF_SCORE_MONKILL_TANKER];
			break;
		}
	}

	if( iScoreType == CRYWOLF_SCORE_LAST_CONTRACTOR )
	{
		iAddMVPScore = m_iMVPScoreTable[CRYWOLF_SCORE_LAST_CONTRACTOR];
	}
	
	gObj[iUserIndex].m_iCrywolfMVPScore += iAddMVPScore;

	LogAddTD("[ Crywolf ][MVP] Plus MVP Score : %d, Accumulated : %d [%s][%s]", 
		iAddMVPScore, gObj[iUserIndex].m_iCrywolfMVPScore, gObj[iUserIndex].AccountID, gObj[iUserIndex].Name);

	return gObj[iUserIndex].m_iCrywolfMVPScore;
}

int CCrywolf::CalcGettingRewardExp( int iUserIndex, int iMVPRank )
{
	if( !gObjIsConnected(iUserIndex) )
		return _ZERO;

	if( !CHECK_LIMIT(iMVPRank, MAX_CRYWOLF_RANK) )
		return _ZERO;

#ifdef BUGFIX_GAIN_EXP_20071210
	INT64	iRewardExp = m_iMVPRankExpTable[iMVPRank];
#else
	int	iRewardExp = m_iMVPRankExpTable[iMVPRank];
#endif

	// ���� ������ ��� 10%�� ���ȴ�.
	if( GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED )
	{
		iRewardExp = iRewardExp * 10 / 100;
	}

#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
	CheckItemOptForGetExpEx( &gObj[iUserIndex], iRewardExp, TRUE);
#else
//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC ���� �� �Ʒ��ڵ� ������ ����	
#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
	g_PCBangPointSystem.CheckItemOptForGetExp( &gObj[iUserIndex], iRewardExp );
#else	
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004


	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
	CheckItemOptForGetExp(&gObj[iUserIndex], iRewardExp, TRUE);
	#else
	//20080416 - �Ʒ� �ڵ�� �����ص� �ɰ� ����
	int iExpRate = 0;
	iExpRate = gObjGetTotalValueOfEffect( &gObj[iUserIndex], EFFECTTYPE_EXPERIENCE );
	if( gObjCheckUsedBuffEffect( &gObj[iUserIndex], BUFFTYPE_PCBANG_POINT_MARK3 ) == false
		&& iExpRate == 0
		)
	{
		iExpRate = 100;
	}

	iRewardExp = ( iRewardExp * iExpRate ) / 100;
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318


#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
//<--
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#ifdef BUGFIX_GAIN_EXP_20071210
	return (int)iRewardExp;
#else
	return iRewardExp;
#endif
}

int	CCrywolf::CalcGettingRank( int iUserIndex )
{
	int iRank = CRYWOLF_RANK_D; 

	for( int i=CRYWOLF_RANK_D; i<=CRYWOLF_RANK_S; i++ )
	{
		if( gObj[iUserIndex].m_iCrywolfMVPScore < m_iMVPRankScoreTable[i] )
			break;
		
		iRank = i;
	}
	
	return iRank;
}

void CCrywolf::GiveRewardExp( int iUserIndex, int iRewardExp )
{
	if( !gObjIsConnected(iUserIndex) )
		return;
	
	if( iRewardExp <= _INVALID )
		return;

	INT iRET_EXP		= 0;
	INT iCAL_EXP		= iRewardExp;
	INT iMAX_LEVCOUNT	= 0;

	if (!gObjIsConnected(iUserIndex))
		return;

	iRET_EXP = iCAL_EXP;

	if( gObj[iUserIndex].Type == OBJTYPE_CHARACTER ) 
	{
		while (iCAL_EXP > 0) 
		{
			if (iCAL_EXP > 0) 
			{
				iCAL_EXP = UTIL.CrywolfMVPLevelUp(iUserIndex, iCAL_EXP);
			}
			iMAX_LEVCOUNT++;

			if (iMAX_LEVCOUNT > 5)
			{
				LogAddTD("[ Crywolf ][GiveRewardExp] LevelUp Error [%s][%s]", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name );
				break;
			}
		}

#ifdef BUGFIX_GAIN_EXP_20071210
		// *�����ͷ��� ������ ����ġ�� �̹� ��������.
		if( !g_MasterLevelSystem.IsMasterLevelUser( &gObj[iUserIndex] ) )
		{
			GCSendExp_INT64( iUserIndex, 0xFFFF, iRewardExp, 0, 0 );
		}
#else
		PMSG_KILLPLAYER_EXT	pkillMsg;

		// ���� 0x16 �̾����� INT�� Ȯ���ϱ� ���� 0x9C�� ����.
		PHeadSetBE((LPBYTE)&pkillMsg, 0x9C, sizeof( pkillMsg));		

		pkillMsg.NumberH	= HIBYTE( 0xFFFF );
		pkillMsg.NumberL	= LOBYTE( 0xFFFF );
		pkillMsg.ExpH		= HIWORD( iRewardExp ) ;
		pkillMsg.ExpL		= LOWORD( iRewardExp ) ;
		pkillMsg.DamageH    = HIBYTE( 0 );
		pkillMsg.DamageL    = LOBYTE( 0 );

		// ����ڿ��� �������ġ�� ������.
		DataSend(iUserIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);
#endif
	}
}

void CCrywolf::ResetAllUserMVPScore()
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == CSS_GAMEPLAYING ) 
			&& (gObj[i].Type == OBJTYPE_CHARACTER) 
			&& (gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
		  )
		{
			gObj[i].m_iCrywolfMVPScore = _ZERO;
		}
	}
}

//-----------------------------------------------------------------------------------------------------

void CCrywolf::CrywolfNpcAct( int iIndex )
{
	if( GetCrywolfState() == CRYWOLF_STATE_READY || GetCrywolfState() == CRYWOLF_STATE_START )
	{
		g_CrywolfNPC_Statue.CrywolfStatueAct( iIndex );
		g_CrywolfNPC_Altar.CrywolfAltarAct( iIndex );
	}
}

void CCrywolf::CrywolfMonsterAct( int iIndex )
{

	
}

void CCrywolf::CrywolfSecondAct()
{
	// 1. > �����鰣�� ũ���̿��� ������ ����ȭ�Ѵ�.
	CrywolfServerGroupSync();

	// 2. > DB���� ũ���̿��� ������ �о�´�.
	if( m_bDBDataLoadOK == FALSE && m_bDBDataLoading == FALSE )
		CrywolfInfoDBLoad();

}

//-----------------------------------------------------------------------------------------------------

#endif