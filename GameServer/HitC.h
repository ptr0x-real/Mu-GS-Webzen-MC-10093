// HitC.h: interface for the CHitC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HITC_H__65E5F5EB_0F4A_4A7D_B8D3_791CECAACC3C__INCLUDED_)
#define AFX_HITC_H__65E5F5EB_0F4A_4A7D_B8D3_791CECAACC3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//----------------------------------------------------------------------------
// 타격을 가한 유저
#define MAX_HITUSER		10

class CHitC  
{
	int		m_UserNumber[MAX_HITUSER];
	int		m_DBNumber[MAX_HITUSER];
	BYTE	m_HitCount[MAX_HITUSER];
	int		m_Count;
public:
	CHitC();
	virtual ~CHitC();

	BOOL Add(int aIndex, int DbNumber);
	void Init();
};

#endif // !defined(AFX_HITC_H__65E5F5EB_0F4A_4A7D_B8D3_791CECAACC3C__INCLUDED_)
