// LogToFile.h: interface for the CLogToFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTOFILE_H__58CA8D10_C1DC_4C1F_80A2_37C33CC12B5D__INCLUDED_)
#define AFX_LOGTOFILE_H__58CA8D10_C1DC_4C1F_80A2_37C33CC12B5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BUFFER	(5*65536)
class CLogToFile  
{
	FILE* m_fLogFile;
	char m_szLogFileName[260];
	char m_szLogDirectoryName[260];
	char m_cBUFFER[MAX_BUFFER];
	int m_bWithDate;
	CRITICAL_SECTION m_critLogToFile;
public:
	CLogToFile(char *LogFileName, char *LogDirectoryName, BOOL bWithDate);
	~CLogToFile();

	void Output(char *fmt, ...);
};

#endif // !defined(AFX_LOGTOFILE_H__58CA8D10_C1DC_4C1F_80A2_37C33CC12B5D__INCLUDED_)
