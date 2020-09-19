// KanturuStateInfo.h: interface for the CKanturuStateInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUSTATEINFO_H__A11B56C0_7BE2_43A9_A143_0F11E338604E__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUSTATEINFO_H__A11B56C0_7BE2_43A9_A143_0F11E338604E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuDefine.h"

class CKanturuStateInfo  
{
private:
	int		m_iState;				// ����
	int		m_iCondition;			// ���º� ���� ���� �ε���
	int		m_iValue;				// ���� ���ǿ� ���� ��

	DWORD	m_dwAppliedTickCount;	
	
public:
	CKanturuStateInfo();
	virtual ~CKanturuStateInfo();

	void	ResetTimeInfo()
	{
		SetStateInfo( 0 );
		SetCondition( KANRUTU_STATE_END_COMDITION_NONE );
		SetValue( 0 );
	}

	void	SetStateInfo( int iStateTimeInfo )		{ m_iState = iStateTimeInfo; }
	int		GetStateInfo()							{ return m_iState; }
	void	SetCondition( int iCondition )			{ m_iCondition = iCondition; }
	int		GetCondition()							{ return m_iCondition; }
	void	SetValue( int iValue )					{ m_iValue = iValue; }
	int		GetValue()								{ return m_iValue; }

	void	SetAppliedTime()						{ m_dwAppliedTickCount = GetTickCount(); }
	int		GetAppliedTime()						{ return m_dwAppliedTickCount; }
	void	ResetAppliedTime()						{ m_dwAppliedTickCount	= 0; }
	int		GetElapsedTime()
	{
		int iElapsedTime = GetTickCount() - GetAppliedTime();
		return iElapsedTime;
	}

	int		GetRemainTime()
	{
		int iRemainTime = 0;
		if( GetCondition() == KANRUTU_STATE_END_COMDITION_TIMELIMIT )
		{
			iRemainTime = m_iValue - GetElapsedTime();
			if( iRemainTime <= 0 )
				return 0;

			iRemainTime /= 1000;
		}
		
		return iRemainTime;
	}

	BOOL	IsTimeOut()
	{
		// ���������� �ð��� ��� �ð��� �� ������ TRUE, �ð��� ���������� FALSE
		if( GetRemainTime() <= 0 )
			return TRUE;

		return FALSE;
	}

};

#endif // !defined(AFX_KANTURUSTATEINFO_H__A11B56C0_7BE2_43A9_A143_0F11E338604E__INCLUDED_)
