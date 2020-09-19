// KanturuMonsterMng.h: interface for the CKanturuMonsterMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUMONSTERMNG_H__1D7D21EA_3EC4_4BDA_A5E6_B2A3861302A6__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUMONSTERMNG_H__1D7D21EA_3EC4_4BDA_A5E6_B2A3861302A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuObjInfo.h"

#define MAX_KANTURU_MONSTER_SETBASE_INFO	200

typedef struct
{
	BYTE	btGroup;			// 그룹 정보
	WORD    wType;
	BYTE	btMapNumber;
	BYTE    btDistance;			// 움직일수 있는 거리
	BYTE	btX;
	BYTE	btY;
	BYTE	btDir;
} KANTURU_MONSTER_SETBASE_INFO, *LPKANTURU_MONSTER_SETBASE_INFO;

class CKanturuMonsterMng  
{
private:
	// MONSTER DATA
	int								m_iMaxMonsterCount;
	KANTURU_MONSTER_SETBASE_INFO	m_SetBaseInfo[MAX_KANTURU_MONSTER_SETBASE_INFO];

	int								m_iMayaObjIndex;
#ifdef KANTURU_TEST_200607119	
	int								m_iNightmareObjIndex;
#endif

	// MOSNTER DATA LOAD
	BOOL							m_bFileDataLoad;

	// KANTURU MONSTER OBJECT INFORMATION
	CKanturuObjInfo					m_KanturuMonster;
	int								m_iMaxRegenMonsterCount;	// 리젠된 전체 몬스터 수
	int								m_iAliveMonsterCount;		// 현재 살아있는 몬스터의 수

public:
	CKanturuMonsterMng();
	virtual ~CKanturuMonsterMng();

	void	ResetLoadData();
	void	ResetRegenMonsterObjData();
	void	MonsterDie( int iIndex );

	// DATA FILE LOAD
	BOOL	LoadData( LPSTR lpszFileName );
	void	SetMonsterSetBaseInfo( int iIndex, BYTE	btGroup, WORD wType, BYTE btMapNumber, BYTE btDistance, BYTE btX, BYTE btY, BYTE btDir );
	void	SetMaxMonsterCount( int iCount )	{ m_iMaxMonsterCount = iCount; }
	int		GetMaxMonsterCount()				{ return m_iMaxMonsterCount; }	

	// MONSTER SET POSITION
	int		SetKanturuMonster( int iGroupNumber );		// 그룹별로 몬스터를 맵에 뿌려준다.
	BOOL	GetPosition( int TableNum, short MapNumber, short &x, short &y );

	int		GetAliveMonsterCount()				{ return m_iAliveMonsterCount; }
	int		GetMaxRegenMonsterCount()			{ return m_iMaxRegenMonsterCount; }
	int		IsExistAliveMonster();

	// MAYA DATA
	int		GetMayaObjIndex()					{ return m_iMayaObjIndex; }
	
#ifdef KANTURU_TEST_200607119
	// NIGHTMARE DATA
	int		GetNightmareObjIndex()				{ return m_iNightmareObjIndex; }
#endif

};

extern CKanturuMonsterMng	g_KanturuMonsterMng;
#endif // !defined(AFX_KANTURUMONSTERMNG_H__1D7D21EA_3EC4_4BDA_A5E6_B2A3861302A6__INCLUDED_)
