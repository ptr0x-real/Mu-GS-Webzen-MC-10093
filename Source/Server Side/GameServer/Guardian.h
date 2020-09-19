// Guardian.h: interface for the CGuardian class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUARDIAN_H__097EC7D7_F8C2_4541_B284_0656B213E450__INCLUDED_)
#define AFX_GUARDIAN_H__097EC7D7_F8C2_4541_B284_0656B213E450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116

#include "..\INCLUDE\DEFINE.H"

#define NPC_INDEX_GUARDIAN	285

class CGuardian  
{
public:
	CGuardian();
	virtual ~CGuardian();

	BOOL	CreateGuardian( INT iIndex );
	VOID	GuardianAct	(INT iIndex);
};

extern CGuardian	g_CsNPC_Guardian;

#endif

#endif // !defined(AFX_GUARDIAN_H__097EC7D7_F8C2_4541_B284_0656B213E450__INCLUDED_)
