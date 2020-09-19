
#include "stdafx.h"

#ifndef _LOGTOFILE_CPP_
#define _LOGTOFILE_CPP_

#include "LogToFile.h"

#ifdef MODIFY_KUNDUN_EVENT_LOG_FILENAME_20060220
extern char			szServerName[];
#endif

CLogToFile::CLogToFile( char LogFileName [MAX_PATH], char LogDirectoryName [MAX_PATH], BOOL bWithDate )
{
//#ifdef FOR_KOREA	
	if ( ! strcmp (LogFileName, "") )
		strcpy (m_szLogFileName, DEF_LOG_FILE_NAME);
	else
		strcpy (m_szLogFileName, LogFileName);

	if ( ! strcmp (LogDirectoryName, "") )
		strcpy (m_szLogDirectoryName, DEF_LOG_DIRECTORY_NAME);
	else
		strcpy (m_szLogDirectoryName, LogDirectoryName);

	m_bWithDate = bWithDate;

	CreateDirectory (LogDirectoryName, NULL());

	InitializeCriticalSection (& m_critLogToFile);
//#endif
}


CLogToFile::~CLogToFile()
{
//#ifdef FOR_KOREA
	DeleteCriticalSection (& m_critLogToFile);
//#endif
}


void CLogToFile::Output( char *fmt, ... )
{
//#ifdef FOR_KOREA
	va_list argptr;
	int iChrWritten;

	EnterCriticalSection (& m_critLogToFile);
	va_start( argptr, fmt );
	iChrWritten = vsprintf( m_cBUFFER, fmt, argptr );
	va_end( argptr );

	char szLogFileName [MAX_PATH];
	SYSTEMTIME strSystime;

	GetLocalTime (& strSystime);
#ifdef MODIFY_KUNDUN_EVENT_LOG_FILENAME_20060220
	wsprintf (szLogFileName, "%s\\%s %04u-%02u-%02u_%s.txt",
		m_szLogDirectoryName,
		m_szLogFileName,
		strSystime.wYear,
		strSystime.wMonth,
		strSystime.wDay,
		szServerName
		);
#else
	wsprintf (szLogFileName, "%s\\%s %04u-%02u-%02u.txt",
		m_szLogDirectoryName,
		m_szLogFileName,
		strSystime.wYear,
		strSystime.wMonth,
		strSystime.wDay
		);
#endif

	if ( ! ( m_fLogFile = fopen (szLogFileName, "a+") ) ) {
		LeaveCriticalSection (& m_critLogToFile);
		return;
	}

	if ( ! m_bWithDate ) {
		fprintf (m_fLogFile, "%s\n", m_cBUFFER);
	}
	else {
		fprintf (m_fLogFile, "%u:%u:%u  %s\n", 
			strSystime.wHour,
			strSystime.wMinute,
			strSystime.wSecond,
			m_cBUFFER
			);
	}

	fclose (m_fLogFile);
	LeaveCriticalSection (& m_critLogToFile);
//#endif
}


#endif
