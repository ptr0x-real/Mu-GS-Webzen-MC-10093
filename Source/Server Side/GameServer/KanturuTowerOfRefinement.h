// KanturuTowerOfRefinement.h: interface for the CKanturuTowerOfRefinement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUTOWEROFREFINEMENT_H__911B6239_E25B_4052_8528_1B48DB8A40FF__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUTOWEROFREFINEMENT_H__911B6239_E25B_4052_8528_1B48DB8A40FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuStateInfo.h"

class CKanturuTowerOfRefinement  
{
private:
	int							m_iTowerOfRefinementState;
	BOOL						m_bEntrancePermit;					// 정제의 탑 입장 가능 여부
	BOOL						m_bIsUseTowerOfRefinement;			// 정제의 탑 사용 가능 여부

	BOOL						m_bFileDataLoad;					// 데이터 파일 로드 유/무
	
	CKanturuStateInfo			m_StateInfo[MAX_TOWER_OF_REFINEMENT_STATE];
	int							m_StateInfoCount;

public:
	CKanturuTowerOfRefinement();
	virtual ~CKanturuTowerOfRefinement();

	void	Init();
	BOOL	LoadData( LPSTR lpszFileName );
	void	ResetAllData();
	void	Run();

	void	SetState( int iTowerOfRefinement );
	void	SetNextState( int iCurrentState );

	void	SetState_NONE();	
	void	SetState_REVITALIZATION();
	void	SetState_NOTIFY1();
	void	SetState_CLOSE();
	void	SetState_NOTIFY2();
	void	SetState_END();

	void	ProcState_NONE();	
	void	ProcState_REVITALIZATION();
	void	ProcState_NOTIFY1();
	void	ProcState_CLOSE();
	void	ProcState_NOTIFY2();
	void	ProcState_END();

	// BATTLE OF NIGHTMARE INFORMATION	
	void	SetTowerOfRefinementState( int iTowerOfRefinement )	{ m_iTowerOfRefinementState = iTowerOfRefinement; }
	int		GetTowerOfRefinementState()							{ return m_iTowerOfRefinementState; }

	void	SetEntrancePermit( BOOL bEntrancePermit )		{ m_bEntrancePermit = bEntrancePermit; }
	BOOL	GetEntrancePermit()							{ return m_bEntrancePermit; }

	void	SetUseTowerOfRefinement( BOOL bUse )		{ m_bIsUseTowerOfRefinement = bUse; }
	BOOL	IsUseTowerOfRefinement()					{ return m_bIsUseTowerOfRefinement; }

	// SCHEDULE
	void	CheckStateTime();
	int		GetRemainTime();
	
};

#endif // !defined(AFX_KANTURUTOWEROFREFINEMENT_H__911B6239_E25B_4052_8528_1B48DB8A40FF__INCLUDED_)
