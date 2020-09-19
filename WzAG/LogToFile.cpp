// LogToFile.cpp: implementation of the CLogToFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogToFile.h"
#include "WzAGConsole.h"

extern char	sztmpServerName[];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogToFile::CLogToFile(char *LogFileName, char *LogDirectoryName, BOOL bWithDate)
{
	if(strcmp(LogFileName, "") == 0) {
		strcpy(m_szLogFileName, "LOGDATA");
	}
	else strcpy(m_szLogFileName, LogFileName);
	
	if(strcmp(LogDirectoryName, "") == 0) {
		strcpy(m_szLogDirectoryName, "LOG");
	}
	else strcpy(m_szLogDirectoryName, LogDirectoryName);
	
	m_bWithDate  = bWithDate;	

	CreateDirectory(LogDirectoryName, NULL);

	InitializeCriticalSection(&m_critLogToFile);
}

CLogToFile::~CLogToFile()
{
	DeleteCriticalSection(&m_critLogToFile );
}

void CLogToFile::Output(char *fmt, ...)
{
	va_list argptr;
	
	int iChrWritten;
	char szLogFileName[260];

	SYSTEMTIME strSystime;

	EnterCriticalSection(&m_critLogToFile);

	va_start(argptr, fmt);
	iChrWritten=vsprintf(m_cBUFFER, fmt, argptr);
	va_end(argptr);

	GetLocalTime(&strSystime);

	wsprintf(szLogFileName, "%s\\%s %04d-%02d-%02d_%s.txt", m_szLogDirectoryName, m_szLogFileName, strSystime.wYear, strSystime.wMonth, strSystime.wDay, sztmpServerName);

	if((m_fLogFile = fopen(szLogFileName, "a+"))==0)
	{
		LeaveCriticalSection(&m_critLogToFile);
		return;
	}

	if (!m_bWithDate) {
		fprintf(m_fLogFile , "%s\n", m_cBUFFER);
	}
	else fprintf(m_fLogFile , "%d:%d:%d  %s\n", strSystime.wHour, strSystime.wMinute, strSystime.wSecond, m_cBUFFER);

	g_WzAGConsole.ConsoleOutput(m_cBUFFER);

	fclose(m_fLogFile);
	LeaveCriticalSection(&m_critLogToFile);
}