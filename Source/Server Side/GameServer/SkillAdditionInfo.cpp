// SkillAdditionInfo.cpp: implementation of the CSkillAdditionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillAdditionInfo.h"

#include "DirPath.h"
extern CDirPath		gDirPath;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSkillAdditionInfo	g_SkillAdditionInfo;

CSkillAdditionInfo::CSkillAdditionInfo()
{
	Init();
}

CSkillAdditionInfo::~CSkillAdditionInfo()
{

}

void CSkillAdditionInfo::Init()
{
	m_iInfinityArrowSkillTime			= 0;
	m_iInfinityArrowUseLevel			= 0;	
	m_iInfinityArrowMPConsumptionPlus0	= 0;		
	m_iInfinityArrowMPConsumptionPlus1	= 0;
	m_iInfinityArrowMPConsumptionPlus2	= 0;
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	m_iInfinityArrowMPConsumptionPlus3	= 0;
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	for( int iCount = 0; iCount <= 5; iCount++ )
	{
		m_nSoulBarrierManaRate_Level[iCount]	= 0;
	}
#endif

}

BOOL CSkillAdditionInfo::Load( LPSTR lpszFileName )
{
	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		LogAddTD("[Skill Addition Info] - File load error : File Name Error");
		return FALSE;
	}

	m_iInfinityArrowSkillTime			= GetPrivateProfileInt( "SkillInfo", "InfinityArrowSkillTime", 600, gDirPath.GetNewPath( lpszFileName ) );	;
	m_iInfinityArrowUseLevel			= GetPrivateProfileInt( "SkillInfo", "InfinityArrowUseLevel", 220, gDirPath.GetNewPath( lpszFileName ) );	;
	m_iInfinityArrowMPConsumptionPlus0	= GetPrivateProfileInt( "SkillInfo", "InfinityArraowMPConsumptionPlus0", 0, gDirPath.GetNewPath( lpszFileName ) );	
	m_iInfinityArrowMPConsumptionPlus1	= GetPrivateProfileInt( "SkillInfo", "InfinityArraowMPConsumptionPlus1", 0, gDirPath.GetNewPath( lpszFileName ) );		
#ifdef MODIFY_INFINITY_ARROW_USE_MANA_BUGFIX_20090416
	m_iInfinityArrowMPConsumptionPlus2	= GetPrivateProfileInt( "SkillInfo", "InfinityArraowMPConsumptionPlus2", 0, gDirPath.GetNewPath( lpszFileName ) );		
#endif // MODIFY_INFINITY_ARROW_USE_MANA_BUGFIX_20090416
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	m_iInfinityArrowMPConsumptionPlus3	= GetPrivateProfileInt( "SkillInfo", "InfinityArraowMPConsumptionPlus3", 0, gDirPath.GetNewPath( lpszFileName ) );
	LogAddTD("[Skill Addition Info] [Infinity Arrow] Skill Time[%d], UseLevel[%d], Arrow MP Consumption +0[%d] +1[%d] +2[%d] +3[%d]", 
											m_iInfinityArrowSkillTime,
											m_iInfinityArrowUseLevel,
											m_iInfinityArrowMPConsumptionPlus0,
											m_iInfinityArrowMPConsumptionPlus1,
											m_iInfinityArrowMPConsumptionPlus2,
											m_iInfinityArrowMPConsumptionPlus3
											);
#else // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	LogAddTD("[Skill Addition Info] [Infinity Arrow] Skill Time[%d], UseLevel[%d], Arrow MP Consumption +0[%d] +1[%d] +2[%d]", 
											m_iInfinityArrowSkillTime,
											m_iInfinityArrowUseLevel,
											m_iInfinityArrowMPConsumptionPlus0,
											m_iInfinityArrowMPConsumptionPlus1,
											m_iInfinityArrowMPConsumptionPlus2 );
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	m_bFireScreamSkill						= GetPrivateProfileInt( "SkillInfo", "FireScreamSkill", 1, gDirPath.GetNewPath( lpszFileName ) );
	m_iFireScreamExplosionAttackDistance	= GetPrivateProfileInt( "SkillInfo", "FireScreamExplosionAttackDistance", 1, gDirPath.GetNewPath( lpszFileName ) );
	m_iFireScreamExplosionRate				= GetPrivateProfileInt( "SkillInfo", "FireScreamExplosionRate", 300, gDirPath.GetNewPath( lpszFileName ) );
	m_iFireScreamMaxAttackCountSameSerial	= GetPrivateProfileInt( "SkillInfo", "FireScreamMaxAttackCountSameSerial", 3, gDirPath.GetNewPath( lpszFileName ) );
	m_iFireScreamSerialInitCount			= GetPrivateProfileInt( "SkillInfo", "FireScreamSerialInitCount", 100, gDirPath.GetNewPath( lpszFileName ) );
	m_bCheckFireScreamSerial				= GetPrivateProfileInt( "SkillInfo", "CheckFireScreamSerial", 1, gDirPath.GetNewPath( lpszFileName ) );
	LogAddTD("[Skill Addition Info] [Fire Scream] Use(%d) : Explosion Distance(%d) - Rate(%d), MaxAttack_SameSerial(%d), SerialInitCount(%d), Check Serial(%d)",
											m_bFireScreamSkill
											, m_iFireScreamExplosionAttackDistance
											, m_iFireScreamExplosionRate
											, m_iFireScreamMaxAttackCountSameSerial
											, m_iFireScreamSerialInitCount
											, m_bCheckFireScreamSerial );
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	m_nSoulBarrierManaRate_Level[0]			= GetPrivateProfileInt( "SkillInfo", "SoulBarrierManaRate_Level0", 25, gDirPath.GetNewPath( lpszFileName ) );							
	m_nSoulBarrierManaRate_Level[1]			= GetPrivateProfileInt( "SkillInfo", "SoulBarrierManaRate_Level1", 25, gDirPath.GetNewPath( lpszFileName ) );							
	m_nSoulBarrierManaRate_Level[2]			= GetPrivateProfileInt( "SkillInfo", "SoulBarrierManaRate_Level2", 30, gDirPath.GetNewPath( lpszFileName ) );							
	m_nSoulBarrierManaRate_Level[3]			= GetPrivateProfileInt( "SkillInfo", "SoulBarrierManaRate_Level3", 35, gDirPath.GetNewPath( lpszFileName ) );							
	m_nSoulBarrierManaRate_Level[4]			= GetPrivateProfileInt( "SkillInfo", "SoulBarrierManaRate_Level4", 40, gDirPath.GetNewPath( lpszFileName ) );							
	m_nSoulBarrierManaRate_Level[5]			= GetPrivateProfileInt( "SkillInfo", "SoulBarrierManaRate_Level5", 45, gDirPath.GetNewPath( lpszFileName ) );
	LogAddTD("[Skill Addition Info] [SoulBarrier Strengrhen] BaseSkill(Level0):%d, Level1:%d, 2:%d, 3:%d, 4:%d, 5:%d",
											m_nSoulBarrierManaRate_Level[0]
											, m_nSoulBarrierManaRate_Level[1]
											, m_nSoulBarrierManaRate_Level[2]
											, m_nSoulBarrierManaRate_Level[3]
											, m_nSoulBarrierManaRate_Level[4] 
											, m_nSoulBarrierManaRate_Level[5] );
		
#endif
	return TRUE;
}

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 소울바리어 강화 관련
short CSkillAdditionInfo::GetSoulBarrierManaRate_Level( int iLevel )			
{ 
	// 소울바리어 일반은 레벨0, 강화 스킬레벨은 1 ~ 5
	if( iLevel < 0 || iLevel > 5 )
	{
		return 20;	// 에러상황이지만 일단 기본값을 넘겨준다.
	}

	return m_nSoulBarrierManaRate_Level[iLevel];
}
#endif