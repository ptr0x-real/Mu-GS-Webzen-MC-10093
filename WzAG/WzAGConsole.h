// WzAGConsole.h: interface for the CWzAGConsole class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZAGCONSOLE_H__7097852A_A949_4E27_8EE6_F46AD12000E4__INCLUDED_)
#define AFX_WZAGCONSOLE_H__7097852A_A949_4E27_8EE6_F46AD12000E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void WINAPI ConsoleRoutine(PVOID pVoid);

class CWzAGConsole  
{
public:
	CWzAGConsole() : m_bEnable(FALSE)
	{
		m_hThread = NULL;
		m_dwHandle = 0;
		m_hWnd = NULL;
		
		InitializeCriticalSection(&m_critiLog);
	}

	virtual ~CWzAGConsole()
	{
		DeleteCriticalSection(&m_critiLog);
		Destroy();
	}

	void Create()
	{
		m_bEnable = TRUE;

		if( (m_hThread = CreateThread(0 ,0 ,(LPTHREAD_START_ROUTINE)ConsoleRoutine, 0, 0, &m_dwHandle)) == FALSE )
		{
			char szMsg[256] = {0};
			wsprintf(szMsg, "CreateThread() failed with error %d", GetLastError());
			MessageBox(NULL, szMsg, "CWzAGConsole::Create() -> Error!!!", 0);
			return;
		}
		
		Sleep(100);		

		SetConsoleWindowAttribute();

		ConsoleOutput("CWzAGConsole::Create");
		ConsoleOutput(GetLog());
	}

	void Destroy()
	{
		if( !m_bEnable )	return;
		
		m_bEnable = FALSE;
		FreeConsole();

		TerminateThread(m_hThread, 0);

		m_hThread = NULL;
		m_dwHandle = 0;
		m_hWnd = NULL;
	}

	void ConsoleOutput(char* Format, ...)
	{
		if( !m_bEnable )	return;

		SYSTEMTIME t;
		GetLocalTime(&t);
		
		char Message[1024];
		
		DWORD dwBytesWritten;
		HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);

		va_list pArguments;
		va_start(pArguments, Format);
		vsprintf(Message, Format, pArguments);
		va_end(pArguments);
		
		char currdate[11] = {0};
		sprintf(currdate, "(%02d:%02d:%02d)", t.wHour, t.wMinute, t.wSecond);
		
		char outputmsg[2048];
		sprintf(outputmsg,"%s %s\n", currdate, Message);
		
		WriteFile(Handle, outputmsg, strlen(outputmsg), &dwBytesWritten, NULL);
	}

	void SetConsoleWindowAttribute()
	{
		RECT WinRect;
		
		GetWindowRect(m_hWnd, &WinRect);
		
		WinRect.top = 0;
		WinRect.bottom = GetSystemMetrics(SM_CYFULLSCREEN)/2;
		WinRect.left = 0;
		WinRect.right = GetSystemMetrics(SM_CXFULLSCREEN)/2;

		EnableMenuItem(GetSystemMenu(m_hWnd, FALSE), SC_CLOSE , MF_GRAYED);
		EnableMenuItem(GetSystemMenu(m_hWnd, FALSE), SC_MAXIMIZE , MF_GRAYED);

		DrawMenuBar(m_hWnd);

		int iPosition[4]; //Dialog Position

		//cX : cY
		iPosition[0] = (GetSystemMetrics(SM_CXFULLSCREEN)/2)-(GetSystemMetrics(SM_CXFULLSCREEN)/4);
		iPosition[1] = (GetSystemMetrics(SM_CYFULLSCREEN)/2)-(GetSystemMetrics(SM_CYFULLSCREEN)/4);

		//Width : Height
		iPosition[2] = int(	WinRect.right-WinRect.left	);
		iPosition[3] = int(	WinRect.bottom-WinRect.top	);
		
		MoveWindow(m_hWnd, iPosition[0], iPosition[1], iPosition[2], iPosition[3], TRUE);
	}

	void SaveLog(char *pLog)
	{
		EnterCriticalSection(&m_critiLog);
		strcpy(m_chLog, pLog);
		LeaveCriticalSection(&m_critiLog);
	}

	char* GetLog()
	{
		EnterCriticalSection(&m_critiLog);
		char* szLog = m_chLog;
		LeaveCriticalSection(&m_critiLog);
		return szLog;
	}

	BOOL	m_bEnable;	//Control function execution
	HANDLE	m_hThread; //Thread control (Start:Stop)
	DWORD	m_dwHandle; //Thread ID saving
	HWND	m_hWnd;	//Window Event

	CRITICAL_SECTION	m_critiLog;	//Log Critical Section
	char	m_chLog[512]; //Console System Log
};

extern CWzAGConsole	g_WzAGConsole;

#endif // !defined(AFX_WZAGCONSOLE_H__7097852A_A949_4E27_8EE6_F46AD12000E4__INCLUDED_)
