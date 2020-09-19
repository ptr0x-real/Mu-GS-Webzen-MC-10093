// WzAGConsole.cpp: implementation of the CWzAGConsole class.
// Date & Author: [1/4/2012 HermeX]
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WzAGConsole.h"

//#pragma comment( linker, "/subsystem:\"windows\" /entry:\""mainCRTStartup\"" )

CWzAGConsole	g_WzAGConsole;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*CWzAGConsole::CWzAGConsole()
{

}

CWzAGConsole::~CWzAGConsole()
{

}
*/

void WINAPI ConsoleRoutine(PVOID pVoid)
{
	int nCount;

	__try
	{
		AllocConsole();		
		SetConsoleTitleA("WzAG");
	}
	__finally
	{
		if( g_WzAGConsole.m_hWnd == NULL )
		{
			nCount = 0;
			while( g_WzAGConsole.m_hWnd == NULL )
			{
				g_WzAGConsole.m_hWnd = FindWindow(NULL, "WzAG");
				nCount++;
				Sleep(1);
			}
			
		}
	}

	char szMsg[64] = {0};
	wsprintf(szMsg, "g_WzAGConsole.m_hWnd = FindWindow -> Attempt %d", nCount);
	g_WzAGConsole.SaveLog(szMsg);
	
	while(true)
	{
		Sleep(1);
	}
}