// KanturuObjInfo.h: interface for the CKanturuObjInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUOBJINFO_H__54BC160B_7358_470F_A5A5_46E5F6624922__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUOBJINFO_H__54BC160B_7358_470F_A5A5_46E5F6624922__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_KANTURU_OBJINFO		800

class CKanturuObjInfo  
{
public:
	int	m_iObjCount;

	int	m_iObjIndex[MAX_KANTURU_OBJINFO];

public:
	CKanturuObjInfo()
	{
		Reset();
	}
	virtual ~CKanturuObjInfo(){}
	
	void Reset()
	{
		for( int iCount = 0; iCount < MAX_KANTURU_OBJINFO; iCount++ )
		{
			m_iObjIndex[iCount] = _INVALID;
		}

		m_iObjCount = _ZERO;
	}

	BOOL AddObj( int iIndex )
	{
		if( m_iObjCount >= MAX_KANTURU_OBJINFO )
			return FALSE;

		m_iObjIndex[m_iObjCount] = iIndex;
		++m_iObjCount;
		return TRUE;
	}

	int GetCount()
	{
		return m_iObjCount;
	}

};

#endif // !defined(AFX_KANTURUOBJINFO_H__54BC160B_7358_470F_A5A5_46E5F6624922__INCLUDED_)
