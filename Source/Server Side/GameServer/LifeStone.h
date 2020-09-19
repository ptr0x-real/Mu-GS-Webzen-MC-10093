// LifeStone.h: interface for the CLifeStone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIFESTONE_H__5A6238A8_A092_472B_87A9_B68859727544__INCLUDED_)
#define AFX_LIFESTONE_H__5A6238A8_A092_472B_87A9_B68859727544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208

#include "Include\DEFINE.H"

#define NPC_INDEX_LIFESTONE				278
#define MAX_LIFESTONE_CREATION_STATE	5
#define MAX_LIFESTONE_CREATION_TIME		60

#define MAX_LIFESTONE_CREATION_LIMIT01	1	// 일반 길드 마스터
#define MAX_LIFESTONE_CREATION_LIMIT02	3	// 다크로드 길드마스터

class CLifeStone  
{
public:
	CLifeStone				();
	virtual ~CLifeStone		();
	
	BOOL	CreateLifeStone( INT iIndex );
	BOOL	DeleteLifeStone( INT iIndex );


	BOOL	SetReSpawnUserXY( INT iUserIndex );
	VOID	LifeStoneAct	(INT iIndex );
};

extern CLifeStone	g_CsNPC_LifeStone;

#endif

#endif // !defined(AFX_LIFESTONE_H__5A6238A8_A092_472B_87A9_B68859727544__INCLUDED_)
