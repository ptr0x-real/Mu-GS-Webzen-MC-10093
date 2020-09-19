#include "stdafx.h"
#include <stdio.h>
#include "Log.h"

CLogSystem::CLogSystem()
{
	m_Crit.Create();
}
CLogSystem::~CLogSystem()
{
	m_Crit.Delete();

	if( m_LogFile.isOpen() ) {
		m_LogFile.Close();
	}
}

void CLogSystem::Init(const char * szName)
{
	CreateDirectory("Log", NULL);

	SYSTEMTIME strSystime;

	GetLocalTime(&strSystime);

	wsprintf(m_szLogFileName, "Log\\%s_%04u-%02u-%02u.txt",
		szName,	
		strSystime.wYear, 
		strSystime.wMonth,
		strSystime.wDay);

	m_LogFile.Open(m_szLogFileName, FILE_APPEND_DATA, FILE_SHARE_READ, OPEN_ALWAYS);
}

void CLogSystem::Output(char * fmt, ...)
{
	SYSTEMTIME	strSystime;
	va_list		argptr;
	INT			iChrWritten	= 0;
	CHAR		pTmpBuffer[8096];
	CHAR		*pBuffer = new CHAR[8096 + 1024];

	va_start(argptr, fmt);
	iChrWritten = vsnprintf(pTmpBuffer, 8096, fmt, argptr);
	va_end(argptr);

	GetLocalTime(&strSystime);

	INT iCount = wsprintf(pBuffer, "[%u:%u:%u]  %s\r\n", strSystime.wHour, strSystime.wMinute, strSystime.wSecond, pTmpBuffer);

	m_Crit.Enter();
		m_LogFile.Write(pBuffer, iCount);
	m_Crit.Leave();

	delete[] pBuffer;
}