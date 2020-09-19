// CannonTower.h: interface for the CCannonTower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANNONTOWER_H__84C8122C_24F0_4076_B94A_6ED09692413F__INCLUDED_)
#define AFX_CANNONTOWER_H__84C8122C_24F0_4076_B94A_6ED09692413F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CASTLE_NPC_CANNON_TOWER_20041223

#include "..\INCLUDE\DEFINE.H"


class CCannonTower  
{
public:
	CCannonTower();
	virtual ~CCannonTower();

	VOID CannonTowerAct		(				// 캐논타워의 행동방식
		INT iIndex
		);
};


extern CCannonTower	g_CsNPC_CannonTower;

#endif

#endif // !defined(AFX_CANNONTOWER_H__84C8122C_24F0_4076_B94A_6ED09692413F__INCLUDED_)
