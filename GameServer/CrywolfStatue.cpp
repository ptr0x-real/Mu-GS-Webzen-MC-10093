#include "Stdafx.h"

#ifdef CRYWOLF_COMMON_PATCH_20050413

#include "CrywolfStatue.h"
#include "CrywolfAltar.h"
#include "CrywolfSync.h"
#include "CrywolfUtil.h"

static	CCrywolfUtil	UTIL;

CCrywolfStatue g_CrywolfNPC_Statue;

CCrywolfStatue::CCrywolfStatue()
{

}

CCrywolfStatue::~CCrywolfStatue()
{

}
int	 CCrywolfStatue::GetStatueViewState( int iPriestNumber )
{
	int iViewState = 0;

	switch( iPriestNumber )
	{
	case 0	:
		iViewState = 0;
		break;
	case 1	:
		iViewState = STATE_CRYWOLF_STATUE_SHIELD1;
		break;
	case 2	:
		iViewState = STATE_CRYWOLF_STATUE_SHIELD2;
		break;
	case 3	:
		iViewState = STATE_CRYWOLF_STATUE_SHIELD3;
		break;
	case 4	:
		iViewState = STATE_CRYWOLF_STATUE_SHIELD4;
		break;
	case 5	:
		iViewState = STATE_CRYWOLF_STATUE_SHIELD5;
		break;
	}

	return iViewState;
}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
void CCrywolfStatue::SetStatueViewState( LPOBJECTSTRUCT lpObj, int iPriestNumber )
{
	switch( iPriestNumber )
	{
	case 1:
		gObjAddBuffEffect( lpObj, BUFFTYPE_CRYWOLF_STATUE_LV1, 0, 0, 0, 0, BUFFTIME_INFINITE );
		break;
	case 2:
		gObjAddBuffEffect( lpObj, BUFFTYPE_CRYWOLF_STATUE_LV2, 0, 0, 0, 0, BUFFTIME_INFINITE );
		break;
	case 3:
		gObjAddBuffEffect( lpObj, BUFFTYPE_CRYWOLF_STATUE_LV3, 0, 0, 0, 0, BUFFTIME_INFINITE );
		break;
	case 4:
		gObjAddBuffEffect( lpObj, BUFFTYPE_CRYWOLF_STATUE_LV4, 0, 0, 0, 0, BUFFTIME_INFINITE );
		break;
	case 5:
		gObjAddBuffEffect( lpObj, BUFFTYPE_CRYWOLF_STATUE_LV5, 0, 0, 0, 0, BUFFTIME_INFINITE );
		break;
	}
}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

void CCrywolfStatue::CrywolfStatueAct( int iIndex )
{
	if (!gObjIsConnected(iIndex))
		return;
	
	if (gObj[iIndex].Type != OBJTYPE_NPC || !CHECK_CRYWOLF_NPC_STATUE(gObj[iIndex].Class)) 
	{
		return;
	}

	LPOBJECTSTRUCT lpObj		= &gObj[ iIndex ];

	// 1. 사제와 계약 되어 있는 제단의 정보를 얻는다.

	int iContractedAlterCount	= g_CrywolfNPC_Altar.GetContractedAltarCount();
	int iPriestHPSum			= g_CrywolfNPC_Altar.GetPriestHPSum();
	int iPriestMaxHPSum			= g_CrywolfNPC_Altar.GetPriestMaxHPSum();

	if( iContractedAlterCount == _ZERO || m_Shield.m_iShieldHP == _ZERO )
	{
		if( m_Shield.m_iShieldState == CRYWOLF_SHILED_STATE_CREATED )
		{
			// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
			UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3333) );			// "늑대 신상의 방어막이 사라졌습니다."

			m_Shield.m_iShieldState		= CRYWOLF_SHILED_STATE_NONE;
			m_Shield.m_iShieldHP		= 0;
			m_Shield.m_iShieldMaxHP		= 0;
			m_Shield.m_iPriestNumber	= iContractedAlterCount;
		
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			SetStatueViewState( lpObj, iContractedAlterCount );
#else
			ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, GetStatueViewState(iContractedAlterCount) );
			GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		}
		else
		{
			if( m_Shield.m_iPriestNumber != iContractedAlterCount )
			{
				m_Shield.m_iShieldState		= CRYWOLF_SHILED_STATE_CREATED;
				m_Shield.m_iShieldHP		= iPriestHPSum;
				m_Shield.m_iShieldMaxHP		= iPriestMaxHPSum;
				m_Shield.m_iPriestNumber	= iContractedAlterCount;

				// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
				UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3334) , iContractedAlterCount, iPriestHPSum );			// "늑대 신상 방어막의 사제수는  %d 명, HP는 %d 입니다."
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				SetStatueViewState( lpObj, iContractedAlterCount );
#else
				ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, GetStatueViewState(iContractedAlterCount) );
				GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			}
			
		}

	}
	else
	{
		if( m_Shield.m_iShieldState == CRYWOLF_SHILED_STATE_NONE )
		{
			// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
			UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3335) );			// "늑대 신상의 방어막이 생성 됐습니다."
			
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			SetStatueViewState( lpObj, iContractedAlterCount );
#else
			ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, GetStatueViewState(iContractedAlterCount) );
			GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		}
		else
		{
			if( m_Shield.m_iPriestNumber != iContractedAlterCount )
			{
				// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
				UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3334) , iContractedAlterCount, iPriestHPSum );			// "늑대 신상 방어막의 사제수는  %d 명, HP는 %d 입니다."
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				SetStatueViewState( lpObj, iContractedAlterCount );
#else
				ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, GetStatueViewState(iContractedAlterCount) );
				GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );	
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			}
			
		}

		m_Shield.m_iShieldState		= CRYWOLF_SHILED_STATE_CREATED;
		m_Shield.m_iShieldHP		= iPriestHPSum;
		m_Shield.m_iShieldMaxHP		= iPriestMaxHPSum;
		m_Shield.m_iPriestNumber	= iContractedAlterCount;
	}	
}

#endif