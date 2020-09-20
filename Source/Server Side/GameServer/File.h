#pragma once

#include <windows.h>

class CFileSys
{
public:
	CFileSys();
	~CFileSys();

	///
	///	Operators.
	///
	//operator HFILE() const { return static_cast<HFILE>(m_hFile);}
	//operator HANDLE() const{ return m_hFile;}

	///
	///	Implementation
	///
	bool Open(	PCHAR szFileName, 
				DWORD dwAccess = GENERIC_READ, 
				DWORD dwShareMode = FILE_SHARE_READ, 
				DWORD dwFlags = OPEN_EXISTING,
				DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL);
	inline bool Create(
				PCHAR szFileName,
				DWORD dwAccess = GENERIC_WRITE, 
				DWORD dwShareMode = 0 /*DENY ALL*/, 
				DWORD dwFlags = CREATE_ALWAYS,
				DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL)
	{
		return Open(szFileName, dwAccess, dwShareMode, dwFlags, dwAttributes);
	}
	void Close();
	inline bool isOpen() const { return m_hFile != INVALID_HANDLE_VALUE; }

	bool Write(LPVOID lpBuf, DWORD nCount);
private:
	HANDLE	m_hFile;
};
