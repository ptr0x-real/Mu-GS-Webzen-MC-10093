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
		case AT_SKILL_SWORD1:		//�������
		case AT_SKILL_SWORD2:		//���
		case AT_SKILL_SWORD3:		//�÷�ġ��
		case AT_SKILL_SWORD4:		//����ġ��
		case AT_SKILL_SWORD5:		//����
			return 0;
		case AT_SKILL_WHEEL:		// ȸ��������
		case AT_SKILL_BLOWOFFURY:	// �г��� �ϰ� 
		case AT_SKILL_STRIKE:		// ��ο�
#ifdef ADD_SEASON4_NEW_SKILL_20080522	// �ı��� �ϰ� �޺� 2�ܰ迡 �߰�
		case AT_SKILL_DESTRUCTION:	// �ı��� �ϰ�
#endif	// ADD_SEASON4_NEW_SKILL_20080522			
			return 1;
	}	

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ȸ�������Ⱝȭ(���) : �޺� ���
	if( g_MasterSkillSystem.GetBaseMasterLevelSkill( skillnum ) == AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT )
		return 1;
#endif

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �޺����(�г��� �ϰ�, ��ο�)
	if( g_MasterSkillSystem.GetBaseMasterLevelSkill( skillnum ) == AT_SKILL_ML_STRENGTHEN_STRIKE	// ��ο� ��ȭ(���)
		|| g_MasterSkillSystem.GetBaseMasterLevelSkill( skillnum ) == AT_SKILL_ML_STRENGTHEN_BLOWOFFURY	// �г��� �ϰ� ��ȭ(���)
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
	{	// �޺� ��ų�� ��밡���ϸ�..
		int pos = GetSkillPos(skillnum);
		if( pos == 0 )
		{	// ù��° �޺� ���
			lpObj->comboSkill.ProgressIndex = 0;
			lpObj->comboSkill.dwTime = GetTickCount()+3*1000;
			lpObj->comboSkill.Skill[0] = skillnum;
		}		
		else if( pos == 1 )
		{
			if( lpObj->comboSkill.Skill[0] == 0xff )
			{	// �޺��� ���� �ʴ´�
				lpObj->comboSkill.Init();							
				return FALSE;
			}
			else
			{
				int tick = GetTickCount();
				if( lpObj->comboSkill.dwTime < GetTickCount() )
				{	// 3�ʺ��� ũ�� �޺� ĵ��
					lpObj->comboSkill.Init();					
					return FALSE;
				}
				else
				{
					if( lpObj->comboSkill.ProgressIndex == 0 )
					{	// �ι�° �޺� ���
						lpObj->comboSkill.ProgressIndex = 1;
						lpObj->comboSkill.dwTime = GetTickCount()+3*1000;
						lpObj->comboSkill.Skill[1] = skillnum;						
					}
					else
					{	
						if( lpObj->comboSkill.Skill[1] != skillnum )
						{	// �޺� �ϼ�
							lpObj->comboSkill.Init();
							return TRUE;
						}	

						lpObj->comboSkill.Init();						
					}
				}
			}
		}
		else
		{	// ��밡���� �޺� ��ų ��Ͽ� ����
			lpObj->comboSkill.ProgressIndex = -1;
			lpObj->comboSkill.dwTime = 0;
			lpObj->comboSkill.Skill[0] = -1;			
		}
	}	
#endif
	return FALSE;
}
#endif