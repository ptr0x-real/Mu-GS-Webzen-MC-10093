// CastleCrownSwitch.h: interface for the CCastleCrownSwitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLECROWNSWITCH_H__360A9A72_A668_47B9_86BD_00CCA159033D__INCLUDED_)
#define AFX_CASTLECROWNSWITCH_H__360A9A72_A668_47B9_86BD_00CCA159033D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CASTLE_NPC_CROWN_SWITCH_WORK_20041210

#include "Include\DEFINE.H"


class CCastleCrownSwitch  
{
public:
	CCastleCrownSwitch			();
	virtual ~CCastleCrownSwitch	();

	VOID CastleCrownSwitchAct	(			// 왕관 스위치의 행동방식
		INT iIndex
		);
};


extern CCastleCrownSwitch	g_CsNPC_CastleCrownSwitch;

#endif // !defined(AFX_CASTLECROWNSWITCH_H__360A9A72_A668_47B9_86BD_00CCA159033D__INCLUDED_)

#endif