

#ifndef _WHATSUP_DUMMY_SERVER_H_
#define _WHATSUP_DUMMY_SERVER_H_

#include <WINSOCK2.H>

#define DEFAULT_DUMMYPORT		55902
#define WM_ASYNCDUMMYSOCK		(WM_USER+15)


class CWhatsUpDummyServer {
	SOCKET	m_sckDUMMY;
	WORD	m_wPORT;
	HWND	m_hParentWnd;

public :

	CWhatsUpDummyServer();
	~CWhatsUpDummyServer();

	BOOL Start (HWND hWnd, WORD wPort);
	
	static WNDPROC	m_lpOldProc;
	static LRESULT ParentWndProc (HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);	
};

#ifdef ADD_SERVER_ALERT_USING_WHATSUP_01_20050628
extern CWhatsUpDummyServer gWhatsUpDummyServer;
#endif


#endif