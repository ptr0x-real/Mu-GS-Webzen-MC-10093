// Mercenary.h: interface for the Mercenary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MERCENARY_H__6B0C8315_3A59_4C30_A73B_F1D75A002D6F__INCLUDED_)
#define AFX_MERCENARY_H__6B0C8315_3A59_4C30_A73B_F1D75A002D6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214

#include "..\INCLUDE\DEFINE.H"

#define NPC_INDEX_SPEARMAN		287
#define NPC_INDEX_BOWMAN		286

#define MAX_MERCENARY_NUMBER	100

class CMercenary  
{
public:
	CMercenary();
	virtual ~CMercenary();
	
	// 용병 스크롤을 사용한다. - 공성 상태가 아닐 경우 아이템으로 대기.
	BOOL	UseLifeStoneScroll( INT iIndex );

	// 용병을 생성한다.
	BOOL	CreateMercenary( INT iIndex, INT iMercenaryTypeIndex, UCHAR cTX, UCHAR cTY );

	BOOL	DeleteMercenary( INT iIndex );
	
	// 적을 찾는다.
	BOOL	SearchEnemy( LPOBJECTSTRUCT lpObj );

	VOID	MercenaryAct	(INT iIndex);
	
public:
	// 용병의 수를 계산한다.
	INT		m_iMercenaryCount;	
};

extern CMercenary	g_CsNPC_Mercenary;

#endif

#endif // !defined(AFX_MERCENARY_H__6B0C8315_3A59_4C30_A73B_F1D75A002D6F__INCLUDED_)
