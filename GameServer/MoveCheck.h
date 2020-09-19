// MoveCheck.h: interface for the CMoveCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVECHECK_H__C41613F4_F032_4868_9047_ED830CB4374F__INCLUDED_)
#define AFX_MOVECHECK_H__C41613F4_F032_4868_9047_ED830CB4374F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MOVECHECK		5

class CMoveCheck  
{
public:
	CMoveCheck();
	virtual ~CMoveCheck();

	DWORD	m_Time;
	int     m_PosX[MAX_MOVECHECK];
	int     m_PosY[MAX_MOVECHECK];
	int     m_PosCount;

	int		m_TmpPosX;
	int		m_TmpPosY;

	void Init();
	int  Insert(int x, int y);
	int  Check();
};

#endif // !defined(AFX_MOVECHECK_H__C41613F4_F032_4868_9047_ED830CB4374F__INCLUDED_)
