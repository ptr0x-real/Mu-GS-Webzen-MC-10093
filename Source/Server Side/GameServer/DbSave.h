// DbSave.h: interface for the CDbSave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBSAVE_H__52556339_3556_401C_BEE4_181E183BA1AF__INCLUDED_)
#define AFX_DBSAVE_H__52556339_3556_401C_BEE4_181E183BA1AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDbSave  
{
public:
	CDbSave();
	virtual ~CDbSave();

	BOOL	Initialize();
	BOOL	Feee();
	BOOL	Add(LPBYTE pObject, int nSize, BYTE headcode, int index );
	DWORD	ThreadProc();
	
	BOOL	Begin();
	void	End();
	
	WZQueue*	WzQSave;
	HANDLE		m_ThreadHandle;
	DWORD		m_dwThreadID;
	BOOL		m_bIsRunning;

	CRITICAL_SECTION criti;
	
};

#endif // !defined(AFX_DBSAVE_H__52556339_3556_401C_BEE4_181E183BA1AF__INCLUDED_)
