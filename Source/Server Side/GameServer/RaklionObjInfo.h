//----------------------------------------------------------------------------------
// FileName				: RaklionObjInfo.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온에서 객체의 정보를 담는 클래스
//----------------------------------------------------------------------------------
// RaklionObjInfo.h: interface for the CRaklionObjInfo class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONOBJINFO_H__D08CDD2B_CF86_4E7C_B7F9_C26FC652BEAE__INCLUDED_) && defined(ADD_RAKLION_20080408)
#define AFX_RAKLIONOBJINFO_H__D08CDD2B_CF86_4E7C_B7F9_C26FC652BEAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_RAKLION_OBJINFO		800

class CRaklionObjInfo  
{
public:
	int	m_iObjCount;

	int	m_iObjIndex[MAX_RAKLION_OBJINFO];

public:
	CRaklionObjInfo()
	{
		Reset();
	}
	virtual ~CRaklionObjInfo(){}
	
	void Reset()
	{
		for( int iCount = 0; iCount < MAX_RAKLION_OBJINFO; iCount++ )
		{
			m_iObjIndex[iCount] = _INVALID;
		}

		m_iObjCount = _ZERO;
	}

	BOOL AddObj( int iIndex )
	{
		if( m_iObjCount >= MAX_RAKLION_OBJINFO )
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

#endif // !defined(AFX_RAKLIONOBJINFO_H__D08CDD2B_CF86_4E7C_B7F9_C26FC652BEAE__INCLUDED_)
