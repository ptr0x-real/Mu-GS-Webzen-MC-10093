#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714

#include "TMonsterSkillElementInfo.h"

#include "User.h"

// 캐릭터나 몬스터에게 적용되고 있는 스킬엘리먼트를 체크하고 처리한다.
void TMonsterSkillElementInfo::CheckSkillElementInfoProc( LPOBJECTSTRUCT lpObj )
{
	// 지속 값들 해제
	if( lpObj->m_MonsterSkillElementInfo.CheckAttackTime() )
	{
		if( lpObj->m_MonsterSkillElementInfo.DecAttackTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAttack();
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-공증&감 해제");	
#endif
		}
	}

	if( lpObj->m_MonsterSkillElementInfo.CheckDefenseTime() )
	{
		if( lpObj->m_MonsterSkillElementInfo.DecDefenseTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetDefense();

#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-방증&감 해제");	
#endif
		}
	}
	
	if( lpObj->m_MonsterSkillElementInfo.CheckAutoAGTime() )
	{
		if( lpObj->m_MonsterSkillElementInfo.DecAutoAGTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAutoAG();
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-AG증&감 해제");	
#endif
		}
	}

	if( lpObj->m_MonsterSkillElementInfo.CheckAutoHPTime() )
	{

		if( lpObj->m_MonsterSkillElementInfo.DecAutoHPTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAutoHP();
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-HP증&감 해제");	
#endif
		}
		else
		{
			if( lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle == MSE_INCDEC_TYPE_CYCLE_CONSTANT )
			{
				// 상수값 적용
				lpObj->Life += lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP;
			}
			else
			{
				// 퍼센트값 적용
				lpObj->Life += (lpObj->Life) * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP / 100;
			}
			
			// 주기적 HP 감소
			if( lpObj->Life < 0 )
			{
				lpObj->Life = 0;
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
				GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0);
#endif
			}
			else if( lpObj->Life > lpObj->MaxLife + lpObj->AddLife )
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
				GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0);
#endif
			}
			else
			{
#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
				GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0);
#endif
			}
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "주기적 (%d)HP증&감소", lpObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP );	
#endif
		}

	}

	if( lpObj->m_MonsterSkillElementInfo.CheckAutoMPTime() )
	{
		if( lpObj->m_MonsterSkillElementInfo.DecAutoMPTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAutoMP();
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-MP증&감 해제");	
#endif
		}
	}

	if( lpObj->m_MonsterSkillElementInfo.CheckAttackTime() )
	{
		if( lpObj->m_MonsterSkillElementInfo.DecAttackTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetAttack();
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-공증&감 해제");	
#endif
		}
	}

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	if( lpObj->m_MonsterSkillElementInfo.CheckImmuneTime() )
	{
		if( lpObj->m_MonsterSkillElementInfo.DecImmuneTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetImmune();

			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_IMMUNE_TO_SKILL );
			GCStateInfoSend(lpObj, 0, STATE_IMMUNE_TO_SKILL);
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-Immune 해제");	
#endif
		}
	}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	if( lpObj->m_MonsterSkillElementInfo.CheckResistTime() )
	{
		if( lpObj->m_MonsterSkillElementInfo.DecResistTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetResist();
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-Resist 해제");	
#endif
		}
	}

	if( lpObj->m_MonsterSkillElementInfo.CheckModifyStatTime() )
	{
		if( lpObj->m_MonsterSkillElementInfo.DecModifyStatTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetModifyStat();
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-ModifyStat 해제");	
#endif
		}
	}

#ifdef ADD_RAKLION_20080408
	if( lpObj->m_MonsterSkillElementInfo.CheckImmuneAllTime() )
	{
		if( lpObj->m_MonsterSkillElementInfo.DecImmuneAllTime() )
		{
			lpObj->m_MonsterSkillElementInfo.ResetImmuneAll();
			
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004	
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_IMMUNE_TO_SKILL );
			GCStateInfoSend(lpObj, 0, STATE_IMMUNE_TO_SKILL);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			
#if TESTSERVER == 1
			MsgOutput(lpObj->m_Index, "스.엘-ImmuneAll 해제");	
#endif
		}
	}
#endif // ADD_RAKLION_20080408
}

void TMonsterSkillElementInfo::Reset()
{
	ResetDefense();
	ResetAttack();
	ResetAutoHP();
	ResetAutoMP();
	ResetAutoAG();
	ResetImmune();
	ResetResist();
	ResetModifyStat();

#ifdef ADD_RAKLION_20080408
	ResetImmuneAll();
#endif // ADD_RAKLION_20080408
	
}

// R E S E T
inline void TMonsterSkillElementInfo::ResetDefense()
{
	m_iSkillElementDefense			= 0;
	m_iSkillElementDefenseTime		= 0;
}
inline void TMonsterSkillElementInfo::ResetAttack()
{
	m_iSkillElementAttack			= 0;
	m_iSkillElementAttackTime		= 0;
}
inline void TMonsterSkillElementInfo::ResetAutoHP()
{
	m_iSkillElementAutoHP			= 0;
	m_iSkillElementAutoHPCycle		= 0;
	m_iSkillElementAutoHPTime		= 0;
}
inline void TMonsterSkillElementInfo::ResetAutoMP()
{
	m_iSkillElementAutoMP			= 0;
	m_iSkillElementAutoMPCycle		= 0;
	m_iSkillElementAutoMPTime		= 0;
}
inline void TMonsterSkillElementInfo::ResetAutoAG()
{
	m_iSkillElementAutoAG			= 0;
	m_iSkillElementAutoAGCycle		= 0;
	m_iSkillElementAutoAGTime		= 0;
}
inline void TMonsterSkillElementInfo::ResetImmune()
{
	m_iSkillElementImmuneNumber		= 0;
	m_iSkillElementImmuneTime		= 0;
}
inline void TMonsterSkillElementInfo::ResetResist()
{
	m_iSkillElementResistNumber		= 0;
	m_iSkillElementResistTime		= 0;
}
inline void TMonsterSkillElementInfo::ResetModifyStat()
{
	m_iSkillElementModifyStat		= 0;
	m_iSkillElementModifyStatType	= 0;
	m_iSkillElementModifyStatTime	= 0;
}

#ifdef ADD_RAKLION_20080408
inline void TMonsterSkillElementInfo::ResetImmuneAll()
{
	m_iSkillElementImmuneAllTime	= 0;
}
#endif // ADD_RAKLION_20080408

// C H E C K
inline BOOL TMonsterSkillElementInfo::CheckDefenseTime()
{
	return m_iSkillElementDefense>0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::CheckAttackTime()
{
	return m_iSkillElementAttack>0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::CheckAutoHPTime()
{
	return m_iSkillElementAutoHPTime>0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::CheckAutoMPTime()
{
	return m_iSkillElementAutoMPTime>0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::CheckAutoAGTime()
{
	return m_iSkillElementAutoAGTime>0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::CheckImmuneTime()
{
	return m_iSkillElementImmuneTime>0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::CheckResistTime()
{
	return m_iSkillElementResistTime>0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::CheckModifyStatTime()
{
	return m_iSkillElementModifyStatTime>0 ? TRUE: FALSE;
}

#ifdef ADD_RAKLION_20080408
inline BOOL TMonsterSkillElementInfo::CheckImmuneAllTime()
{
	return m_iSkillElementImmuneAllTime>0 ? TRUE: FALSE;
}
#endif // ADD_RAKLION_20080408

// D E C R E A S E
inline BOOL TMonsterSkillElementInfo::DecDefenseTime()
{
	return --m_iSkillElementDefense<=0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::DecAttackTime()
{
	return --m_iSkillElementAttack<=0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::DecAutoHPTime()
{
	return --m_iSkillElementAutoHPTime<=0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::DecAutoMPTime()
{
	return --m_iSkillElementAutoMPTime<=0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::DecAutoAGTime()
{
	return --m_iSkillElementAutoAGTime<=0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::DecImmuneTime()
{
	return --m_iSkillElementImmuneTime<=0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::DecResistTime()
{
	return --m_iSkillElementResistTime<=0 ? TRUE: FALSE;
}

inline BOOL TMonsterSkillElementInfo::DecModifyStatTime()
{
	return --m_iSkillElementModifyStatTime<=0 ? TRUE: FALSE;
}

#ifdef ADD_RAKLION_20080408
inline BOOL TMonsterSkillElementInfo::DecImmuneAllTime()
{
	return --m_iSkillElementImmuneAllTime<=0 ? TRUE: FALSE;
}
#endif // ADD_RAKLION_20080408
#endif