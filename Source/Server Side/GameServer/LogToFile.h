
#ifndef _LOGTOFILE_H_
#define _LOGTOFILE_H_

#include <windows.h>
#include <stdio.h>
//#include "stdafx.h"     // for MFC Application

#define DEF_LOG_FILE_NAME		"LOGDATA"
#define DEF_LOG_DIRECTORY_NAME	"LOG"
#define LINE_BUF				(1024 * 64)


class CLogToFile
{
public:
	void Output( char *fmt, ... );

	CLogToFile( char LogFileName [MAX_PATH] = DEF_LOG_FILE_NAME, char LogDirectoryName [MAX_PATH] = DEF_LOG_DIRECTORY_NAME, BOOL bWithDate = TRUE );
	~CLogToFile();

private:
	FILE * m_fLogFile;
	char m_szLogFileName [MAX_PATH];
	char m_szLogDirectoryName [MAX_PATH];
	char m_cBUFFER [LINE_BUF];

	BOOL m_bWithDate;
	CRITICAL_SECTION m_critLogToFile;
};


#endif
