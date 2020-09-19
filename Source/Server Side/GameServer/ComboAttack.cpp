// ComboAttack.cpp: implementation of the CComboAttack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Include\Public.h"
#include "Include\ProDef.h"
#include "Include\SproDef.h"
#include "Include\GMProtocol.h"
#include "Common\Winutil.h"
#include "Common\zzzItem.h"
#include "Common\Classdef.h"
#include "Include\ReadScript.h"

#include "User.h"
#include "protocol.h"

#include "ComboAttack.h"

#ifdef ADD_SKILL_WITH_COMBO
CComboAttack	gComboAttack;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComboAttack::CComboAttack()
{

}

CComboAttack::~CComboAttack()
{

}

int	CComboAttack::GetSkillPos(int skillnum)
{
	switch(skillnum)
	{
		case AT_SKILL_SWORD1:		//내려찍기
		case AT_SKILL_SWORD2:		//찌르기
		case AT_SKILL_SWORD3:		//올려치기
		case AT_SKILL_SWORD4:		//돌려치기
		case AT_SKILL_SWORD5:		//베기
			return 0;
		case AT_SKILL_WHEEL:		// 회오리베기
		case AT_SKILL_BLOWOFFURY:	// 분노의 일격 
		case AT_SKILL_STRIKE:		// 블로우
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// 파괴의 일겨 콤보 2단계에 추가
		case AT_SKILL_DESTRUCTION:	// 파괴의 일격
#endif	// ADD_SEASON4_NEW_SKILL_20080522			
			return 1;
	}	

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 회오리베기강화(기사) : 콤보 등록
	if( g_MasterSkillSystem.GetBaseMasterLevelSkill( skillnum ) == AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT )
		return 1;
#endif

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 콤보등록(분노의 일격, 블로우)
	if( g_MasterSkillSystem.GetBaseMasterLevelSkill( skillnum ) == AT_SKILL_ML_STRENGTHEN_STRIKE	// 블로우 강화(기사)
		|| g_MasterSkillSystem.GetBaseMasterLevelSkill( skillnum ) == AT_SKILL_ML_STRENGTHEN_BLOWOFFURY	// 분노의 일격 강화(기사)
		)
		return 1;	
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
	
	return -1;
}

BOOL CComboAttack::CheckCombo(int aIndex, int skillnum)
{
#ifdef SECOND_QUEST
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

#if TESTSERVER == 0
	if( lpObj->ComboSkillquestClear )
#endif
	{	// 콤보 스킬이 사용가능하면..
		int pos = GetSkillPos(skillnum);
		if( pos == 0 )
		{	// 첫번째 콤보 등록
			lpObj->comboSkill.ProgressIndex = 0;
			lpObj->comboSkill.dwTime = GetTickCount()+3*1000;
			lpObj->comboSkill.Skill[0] = skillnum;
		}		
		else if( pos == 1 )
		{
			if( lpObj->comboSkill.Skill[0] == 0xff )
			{	// 콤보가 되지 않는다
				lpObj->comboSkill.Init();							
				return FALSE;
			}
			else
			{
				int tick = GetTickCount();
				if( lpObj->comboSkill.dwTime < GetTickCount() )
				{	// 3초보다 크면 콤보 캔슬
					lpObj->comboSkill.Init();					
					return FALSE;
				}
				else
				{
					if( lpObj->comboSkill.ProgressIndex == 0 )
					{	// 두번째 콤보 등록
						lpObj->comboSkill.ProgressIndex = 1;
						lpObj->comboSkill.dwTime = GetTickCount()+3*1000;
						lpObj->comboSkill.Skill[1] = skillnum;						
					}
					else
					{	
						if( lpObj->comboSkill.Skill[1] != skillnum )
						{	// 콤보 완성
							lpObj->comboSkill.Init();
							return TRUE;
						}	

						lpObj->comboSkill.Init();						
					}
				}
			}
		}
		else
		{	// 사용가능한 콤보 스킬 목록에 없다
			lpObj->comboSkill.ProgressIndex = -1;
			lpObj->comboSkill.dwTime = 0;
			lpObj->comboSkill.Skill[0] = -1;			
		}
	}	
#endif
	return FALSE;
}
#endif