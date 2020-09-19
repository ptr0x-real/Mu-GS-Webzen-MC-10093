//*---------------------------------------------------------------
// << CCallStackTrace CLASS >>
// BUILT BY H.J.I.
//
// FILE : CallStackTrace.H
// DESC : 호출스택 출력 클래스 (실행파일에 반드시 .pdb (심볼정보) 첨부 필요. 없으면 데이터가 남지않음)
// INIT BUILT DATE : 2004. 09. 16
// LAST BUILT DATE : 2004. 09. 16
//
//*---------------------------------------------------------------

#pragma warning (disable : 4786)

#ifndef _CALLSTACKTRACE_H_
#define _CALLSTACKTRACE_H_

#include <WINDOWS.H>			// MFC 일때는 stdafx.h
#include <STRING>

#define DEF_LOG_FILE_NAME		"LOGDATA"
#define DEF_LOG_DIRECTORY_NAME	"LOG"

using namespace std;


///////////////////////////////////////////////////////////////////
//	DECLARATION
///////////////////////////////////////////////////////////////////


//*---------------------------------------------------------------
// TYPE : CLASS
// NAME : CCallStackTrace
//*---------------------------------------------------------------
// DESC : 호출스택 출력 클래스
// PROGRAMMER : H.J.I.
//*---------------------------------------------------------------
class CCallStackTrace
{
public:
	CCallStackTrace							(
		char szLogID [MAX_PATH],			// 본 객체를 식별할 사용자 지정 ID (반드시 필요)
		char szLogFileName [MAX_PATH] = DEF_LOG_FILE_NAME,				// 로그파일 이름 
		char szLogDirectoryName [MAX_PATH] = DEF_LOG_DIRECTORY_NAME		// 로그파일 디렉토리 이름
		);
	virtual ~CCallStackTrace();

	int		SaveCallStack					(BOOL bWriteInFile = TRUE);
	char *	GetCallStackLogBuffer			()	{	return &m_strCallStackLog[0]; }		// 만일 파일기록을 않했을 경우 로그버퍼 포인터를 바로 가져옴

private:
	VOID	Output							();
	int		TraceStack						(BOOL bWriteInFile = TRUE);
	DWORD	ConvertAddress					(HANDLE hProcess, DWORD address, char* output_buffer);
	static DWORD __stdcall GetModuleBase	(HANDLE hProcess, DWORD address);

private:
	FILE *				m_fLogFile;
	char				m_szLogID				[MAX_PATH];
	char				m_szLogFileName			[MAX_PATH];
	char				m_szLogDirectoryName	[MAX_PATH];

	char				m_szCallFuncName		[MAX_PATH];		// TraceStack() 을 호출한 함수명

	CRITICAL_SECTION	m_critLogToFile;

	string				m_strCallStackLog;
};


#endif

