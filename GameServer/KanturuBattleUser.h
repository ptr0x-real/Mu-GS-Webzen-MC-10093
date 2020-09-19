// KanturuBattleUser.h: interface for the CKanturuBattleUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUBATTLEUSER_H__B51818AE_B9E1_4727_95DC_D456F50B4806__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUBATTLEUSER_H__B51818AE_B9E1_4727_95DC_D456F50B4806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKanturuBattleUser	
{
private:
	int		m_iIndex;
	BOOL	m_bIsUse;

	


public:
	CKanturuBattleUser()
	{
		ResetData();
	}
	virtual ~CKanturuBattleUser() {};

	void	ResetData()
	{
		m_iIndex	= -1;
		m_bIsUse	= FALSE;
	}	

	int		GetIndex()
	{
		return m_iIndex;
	}
	
	void	SetIndex( int iIndex )
	{
		m_iIndex = iIndex;
		m_bIsUse = TRUE;
	}

	BOOL	IsUseData()
	{
		return m_bIsUse;
	}
	
};

#endif // !defined(AFX_KANTURUBATTLEUSER_H__B51818AE_B9E1_4727_95DC_D456F50B4806__INCLUDED_)
