// GuardianStatue.h: interface for the CGuardianStatue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUARDIANSTATUE_H__AF06D052_2802_4123_BA4C_F588DA105F0D__INCLUDED_)
#define AFX_GUARDIANSTATUE_H__AF06D052_2802_4123_BA4C_F588DA105F0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CASTLE_NPC_GUARDIANSTATUE_WORK_20041207

#include "Include\DEFINE.H"


class CGuardianStatue  
{
public:
	CGuardianStatue				();
	virtual ~CGuardianStatue	();

	VOID GuardianStatueAct		(				// 수호석상의 행동방식
		INT iIndex
		);
};

extern CGuardianStatue	g_CsNPC_GuardianStatue;

#endif

#endif // !defined(AFX_GUARDIANSTATUE_H__AF06D052_2802_4123_BA4C_F588DA105F0D__INCLUDED_)
