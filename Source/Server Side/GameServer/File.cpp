#include "stdafx.h"
#include <crtdbg.h>
#include "File.h"

CFileSys::CFileSys() : m_hFile(INVALID_HANDLE_VALUE)
{
}

CFileSys::~CFileSys()
{
}

bool CFileSys::Open(PCHAR szFileName, DWORD dwAccess, DWORD dwShareMode, DWORD dwFlags, DWORD dwAttributes)
{
	_ASSERTE(!::IsBadStringPtr(szFileName, -1));
	
	Close();

	//
	// Attempt file creation
	//
	HANDLE hFile = ::CreateFile(szFileName, 
								dwAccess, 
								dwShareMode, 
								NULL,
								dwFlags, 
								dwAttributes, 
								NULL);

	if( hFile == INVALID_HANDLE_VALUE ) {
		return false;
	}
	m_hFile = hFile;
	
	return true;
}

void CFileSys::Close()
{
	if( m_hFile == INVALID_HANDLE_VALUE ) {
		return;
	}
	CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
}
bool CFileSys::Write(LPVOID lpBuf, DWORD nCount)
{
	_ASSERTE(m_hFile != INVALID_HANDLE_VALUE);
	_ASSERTE(lpBuf != NULL);
	_ASSERTE(!::IsBadReadPtr(lpBuf, nCount));  

	if( nCount == 0 ) {
		return true;
	}
	DWORD dwBytesWritten = 0;

	if( !::WriteFile(m_hFile, lpBuf, nCount, &dwBytesWritten, NULL) ) {
		return false;
	}
	return true;
}