// MonsterSetBase.h: interface for the CMonsterSetBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTERSETBASE_H__80078AF6_D127_4DA9_98CE_1A6C89DF5A98__INCLUDED_)
#define AFX_MONSTERSETBASE_H__80078AF6_D127_4DA9_98CE_1A6C89DF5A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\include\\define.h"


//#define MAX_MONSTER			3200
//#define MAX_CONNECTMON		30

typedef struct
{
	BYTE    m_ArrangeType;	// 몬스터 배치 타입
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD    m_Type;
#else
	BYTE    m_Type;
#endif 
	BYTE	m_MapNumber;
	BYTE    m_Dis;		// 움직일수 있는 거리
	BYTE	m_X;
	BYTE	m_Y;
	BYTE	m_Dir;
	BYTE	m_W;	//
	BYTE	m_H;
} MONSTER_POSITION, *LPMONSTER_POSITION;



class CMonsterSetBase  
{
public:
	MONSTER_POSITION	m_Mp[MAX_MONSTER];
	int	m_Count;
public:
	void LoadSetBase(char *filename);

#ifdef SCRIPT_DECODE_WORK	
	void LoadSetBaseDecode(char *filename);
#endif

#ifdef AUTH_GAMESERVER
	void LoadSetBase(char *Buffer, int iSize);
#endif

	CMonsterSetBase();
	virtual ~CMonsterSetBase();

	BOOL GetPosition(int TableNum, short MapNumber, short & x, short & y);
	BOOL GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short & mx, short& my);
	void SetBoxPosition(int TableNum, int mapnumber, int ax, int ay, int aw, int ah);

};

#endif // !defined(AFX_MONSTERSETBASE_H__80078AF6_D127_4DA9_98CE_1A6C89DF5A98__INCLUDED_)
