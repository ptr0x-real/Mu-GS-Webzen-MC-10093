// ComboAttack.h: interface for the CComboAttack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBOATTACK_H__0FE39B3D_9810_4EE4_8AFD_1D9777F7F0F8__INCLUDED_)
#define AFX_COMBOATTACK_H__0FE39B3D_9810_4EE4_8AFD_1D9777F7F0F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{	
	DWORD	dwTime;
#ifdef MODIFY_COMBOSKILLDATA_SKILLNUMBER_20080801
	short	Skill[3];
#else	// MODIFY_COMBOSKILLDATA_SKILLNUMBER_20080801
	BYTE	Skill[3];
#endif	// MODIFY_COMBOSKILLDATA_SKILLNUMBER_20080801
	int		ProgressIndex;

	void Init()
	{
		dwTime = 0;
#ifdef MODIFY_COMBOSKILLDATA_SKILLNUMBER_20080801
		Skill[0] = -1;	Skill[1] = -1;	Skill[2] = -1;
#else	// MODIFY_COMBOSKILLDATA_SKILLNUMBER_20080801
		Skill[0] = 0xff;	Skill[1] = 0xff;	Skill[2] = 0xff;
#endif	 // MODIFY_COMBOSKILLDATA_SKILLNUMBER_20080801
		ProgressIndex = -1;
	}
}ComboSkillData;

#ifdef ADD_SKILL_WITH_COMBO
class CComboAttack  
{
	int		GetSkillPos(int skillnum);
public:
	CComboAttack();
	virtual ~CComboAttack();

	BOOL	CheckCombo(int aIndex, int skillnum);
};

extern CComboAttack	gComboAttack;
#endif

#endif // !defined(AFX_COMBOATTACK_H__0FE39B3D_9810_4EE4_8AFD_1D9777F7F0F8__INCLUDED_)
