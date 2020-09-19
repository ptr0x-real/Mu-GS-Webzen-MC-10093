// CastleCrown.h: interface for the CCastleCrown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLECROWN_H__FAC9119F_D0F2_4363_9242_541182F64216__INCLUDED_)
#define AFX_CASTLECROWN_H__FAC9119F_D0F2_4363_9242_541182F64216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CASTLE_NPC_CROWN_WORK_20041210

#include "..\INCLUDE\DEFINE.H"


class CCastleCrown  
{
public:
	CCastleCrown			();
	virtual ~CCastleCrown	();

	VOID CastleCrownAct		(				// 왕관의 행동방식
		INT iIndex
		);
};


extern CCastleCrown	g_CsNPC_CastleCrown;

#endif

#endif // !defined(AFX_CASTLECROWN_H__FAC9119F_D0F2_4363_9242_541182F64216__INCLUDED_)
