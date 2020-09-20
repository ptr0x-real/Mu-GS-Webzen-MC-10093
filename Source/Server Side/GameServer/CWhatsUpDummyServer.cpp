#include "stdafx.h"
#include "CWhatsUpDummyServer.h"

#ifdef ADD_SERVER_ALERT_USING_WHATSUP_01_20050628
#include "TServerAlertManager.h"
CWhatsUpDummyServer gWhatsUpDummyServer;
#endif

CWhatsUpDummyServer::CWhatsUpDummyServer()
{
	m_sckDUMMY = INVALID_SOCKET;
	m_hParentWnd = NULL;
	m_lpOldProc = NULL;

	WSADATA wsaVer;
	WSAStartup(MAKEWORD(2,2), &wsaVer);
}


CWhatsUpDummyServer::~CWhatsUpDummyServer()
{
	if (m_sckDUMMY != INVALID_SOCKET)
		closesocket(m_sckDUMMY);

	//WSACleanup();
}


WNDPROC CWhatsUpDummyServer::m_lpOldProc = NULL;


BOOL CWhatsUpDummyServer::Start (HWND hWnd, WORD wPort)
{
	if (hWnd == NULL)
		return FALSE;

	if (!IsWindow (hWnd))
		return FALSE;

	m_hParentWnd = hWnd;

	m_sckDUMMY = socket (AF_INET, SOCK_STREAM, 0);
	if (m_sckDUMMY == INVALID_SOCKET)
		return FALSE;

	SOCKADDR_IN	sockinfo;
	sockinfo.sin_family = AF_INET;
	sockinfo.sin_addr.s_addr = INADDR_ANY;
	sockinfo.sin_port = htons(wPort);

	INT iRetVal = 0;
	iRetVal = bind (m_sckDUMMY, (LPSOCKADDR)&sockinfo, sizeof(sockinfo));
	if (iRetVal == SOCKET_ERROR) {
		closesocket(m_sckDUMMY);
		m_sckDUMMY = INVALID_SOCKET;
		return FALSE;
	}

	iRetVal = listen (m_sckDUMMY, SOMAXCONN);
	if (iRetVal == SOCKET_ERROR) {
		closesocket(m_sckDUMMY);
		m_sckDUMMY = INVALID_SOCKET;
		return FALSE;
	}

	iRetVal = WSAAsyncSelect(m_sckDUMMY, hWnd, WM_ASYNCDUMMYSOCK, FD_ACCEPT);
	if (iRetVal == SOCKET_ERROR) {
		closesocket(m_sckDUMMY);
		m_sckDUMMY = INVALID_SOCKET;
		return FALSE;
	}

	CWhatsUpDummyServer::m_lpOldProc = (WNDPROC) SetWindowLong (hWnd, GWL_WNDPROC, (LONG) CWhatsUpDummyServer::ParentWndProc);

	return TRUE;
}


LRESULT CWhatsUpDummyServer::ParentWndProc (HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage) {
	case WM_ASYNCDUMMYSOCK :
		switch (LOWORD (lParam)) {
		case FD_ACCEPT :
			{
				SOCKADDR_IN sockinfo;
				INT isockinfolen = sizeof (SOCKADDR_IN);

				SOCKET sckTemp = accept (LOWORD(wParam), (LPSOCKADDR) &sockinfo, &isockinfolen);
				if (sckTemp == INVALID_SOCKET)
					return 0;

#ifdef ADD_SERVER_ALERT_USING_WHATSUP_01_20050628
				if( g_ServerAlertManager.StopWhatsUp() )
				{
					LogAddC( 2, "[WhatsUp Func.] Stopped WhatsUp Function by Alert Manager!!");
					closesocket(sckTemp);
					return 0;
				}
#endif

				INT iRetVal = WSAAsyncSelect(sckTemp, hWnd, WM_ASYNCDUMMYSOCK, FD_READ | FD_CLOSE);
				if (iRetVal == SOCKET_ERROR) {
					closesocket(sckTemp);
				}
			}
			return 0;
		case FD_READ :
			{
				CHAR cBUFFER[256];
				INT iRecvBytes = recv (LOWORD(wParam), cBUFFER, sizeof(cBUFFER), 0);
				if (iRecvBytes <= 0) {
					closesocket(LOWORD(wParam));
				}
			}
			return 0;
		case FD_CLOSE :
			closesocket(LOWORD(wParam));
			return 0;
		}
		return 0;
	}

	return CallWindowProc(CWhatsUpDummyServer::m_lpOldProc, hWnd, iMessage, wParam, lParam);
}


