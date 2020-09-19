#include "Stdafx.h"
#include "TServerAlertManager.h"
#include "wsJoinServerCli.h"
#include "DBSockMng.h"

#ifdef ADD_SERVER_ALERT_USING_WHATSUP_01_20050628

extern wsJoinServerCli  wsJServerCli;

// WhatsUp Disble 경고 레벨

// 


TServerAlertManager g_ServerAlertManager;

TServerAlertManager::TServerAlertManager()
{
	m_bStopWhatsUp		 = FALSE;

	m_bValidJSConnection = FALSE;
	m_bValidDSConnection = FALSE;
}

TServerAlertManager::~TServerAlertManager()
{


}

void TServerAlertManager::Run( HWND hWnd )
{
	CheckDSConnection();
	CheckJSConnection();
	CheckLogFileSize();

	PaintAllAlert( hWnd );
}

void TServerAlertManager::CheckLogFileSize()
{

}

void TServerAlertManager::CheckJSConnection()
{
	if( wsJServerCli.GetSocket() == INVALID_SOCKET )
	{
		m_bStopWhatsUp = TRUE;
		m_bValidJSConnection = FALSE;
	}
	else
	{
		m_bValidJSConnection = TRUE;
	}
}

void TServerAlertManager::CheckDSConnection()
{
	if( cDBSMng.wsCDBS[0].GetSocket() == INVALID_SOCKET )
	{
		m_bStopWhatsUp = TRUE;
		m_bValidDSConnection = FALSE;
	}
	else
	{
		m_bValidDSConnection = TRUE;
	}
}

void TServerAlertManager::ClearAllAlert()
{

	m_bStopWhatsUp = FALSE;

}

static char *ErrorMessge[] = 
{
	"--- [Alert Manager] Stopped WhatsUp Func.! ---",
	"--- JoinServer - Disconnected",
	"--- DataServer - Disconnected"
};

enum SAM_MESSAGE_INDEX
{
	SAM_MESSAGE_INDEX_STOPPEDWHATSUP = 0,
	SAM_MESSAGE_INDEX_DISCONNECTEDJS = 1,
	SAM_MESSAGE_INDEX_DISCONNECTEDDS = 2,
};

void TServerAlertManager::PaintAllAlert( HWND hWnd, int iTopLeftX, int iTopLeftY )
{
	HDC	hDC;
	hDC = GetDC(hWnd);

	int iLineCount = 0;

	if( m_bStopWhatsUp == TRUE )
	{
		SetTextColor(hDC, RGB(255,0,0));

		TextOut( hDC, iTopLeftX, iTopLeftY + iLineCount*15 , 
				 ErrorMessge[SAM_MESSAGE_INDEX_STOPPEDWHATSUP], 
				 strlen(ErrorMessge[SAM_MESSAGE_INDEX_STOPPEDWHATSUP]) );
		iLineCount++;
	}

	if( m_bValidJSConnection == FALSE )
	{
		SetTextColor(hDC, RGB( 0,0,255));

		TextOut( hDC, iTopLeftX, iTopLeftY + iLineCount*15 , 
				 ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDJS],
				 strlen(ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDJS]) );

		iLineCount++;
	}

	if( m_bValidDSConnection == FALSE )
	{
		SetTextColor(hDC, RGB( 0,0,255));

		TextOut( hDC, iTopLeftX, iTopLeftY + iLineCount*15 , 
				 ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDDS],
				 strlen(ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDDS]) );

		iLineCount++;
	}
		
	ReleaseDC(hWnd, hDC);
}

#endif
