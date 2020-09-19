#include "Stdafx.h"
#include "wsJoinServerCli.h"
#include "DBSockMng.h"
#include "LogToFile.H"

#include "TServerInfoDisplayer.h"

#ifdef ADD_SERVER_INFO_DISPLAY_EXTEND_01_20060313
//--------------------------------------------------------------------------------------------

extern wsJoinServerCli  wsJServerCli;
#ifdef _NEW_EXDB_
extern wsJoinServerCli  wsExDbCli;
#endif
extern wsJoinServerCli	wsRServerCli;
extern wsJoinServerCli	wsEvenChipServerCli;

extern int IsDevilSquareEventConnected;
extern int IsEventChipServerConnected;

//--------------------------------------------------------------------------------------------

TServerInfoDisplayer g_ServerInfoDisplayer;

CLogToFile	SERVER_CONN_STATE_LOG ("SERVER_CONN_STATE_LOG", ".\\SERVER_CONN_STATE_LOG");		// 로그파일 객체

//--------------------------------------------------------------------------------------------

static char *ServerTypeText = 
{
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
	
	#ifdef FOR_NEW_TESTSERVER
		"]+[  Test Castle  ]+["
	#else
		"]+[  Castle  ]+["
	#endif 

#else

	#ifdef FOR_NEW_TESTSERVER
		"[-  Test Common  -]"
	#else
		"[-  MC 99B+ V9 -]"
	#endif 

#endif 
};

static char *ErrorMessge[] = 
{
	"0) JoinServer - Disconnected",
	"1) FsGateServer - Disconnected",
	"2) DataServer - Disconnected",
	"3) ExDBServer - Disconnected",
	"4) EventServer - Disconnected",
	"5) RankingServer - Disconnected",
};

enum SAM_MESSAGE_INDEX
{
	SAM_MESSAGE_INDEX_DISCONNECTEDJS	= 0,
	SAM_MESSAGE_INDEX_DISCONNECTEDFS	= 1,
	SAM_MESSAGE_INDEX_DISCONNECTEDDS	= 2,
	SAM_MESSAGE_INDEX_DISCONNECTEDEXDS	= 3,
	SAM_MESSAGE_INDEX_DISCONNECTEDEVDS	= 4,
	SAM_MESSAGE_INDEX_DISCONNECTEDRKDS	= 5,
};

//--------------------------------------------------------------------------------------------

TServerInfoDisplayer::TServerInfoDisplayer()
{
	m_bValidJSConnection	= FALSE;
	m_bValidFSConnection	= FALSE;
	m_bValidDSConnection	= FALSE;
	m_bValidEXDSConnection	= FALSE;
	m_bValidEVDSConnection	= FALSE;
	m_bValidRKDSConnection	= FALSE;

	InitGDIObject();
}

TServerInfoDisplayer::~TServerInfoDisplayer()
{
	DelGDIObject();
}

void TServerInfoDisplayer::InitGDIObject()
{
	m_hFont				= CreateFont( 80, 0, 0, 0, 100, 0, 1, 0, 0, 0, 0, 0, 0, "Times");

	m_hNormalBrush		= CreateSolidBrush( RGB(110,240,120) );
	m_hAbnormalBrush	= CreateSolidBrush( RGB(250,110,110) );
}

void TServerInfoDisplayer::DelGDIObject()
{
	DeleteObject( m_hFont			);
	DeleteObject( m_hNormalBrush	);
	DeleteObject( m_hAbnormalBrush	);
}


void TServerInfoDisplayer::Run( HWND hWnd )
{
	CheckDSConnection();
	CheckJSConnection();
	CheckFSConnection();
#ifdef _NEW_EXDB_
	CheckEXDSConnection();
#endif
	CheckEVDSConnection();
	CheckRKDSConnection();
	
	CheckLogFileSize();

	PaintAllInfo( hWnd );
}

//--------------------------------------------------------------------------------------------

inline void TServerInfoDisplayer::CheckLogFileSize()
{

}

inline void	TServerInfoDisplayer::CheckPerformance()
{

}

inline void TServerInfoDisplayer::CheckJSConnection()
{
	if( wsJServerCli.GetSocket() == INVALID_SOCKET )
	{
		m_bValidJSConnection = FALSE;
	}
	else
	{
		m_bValidJSConnection = TRUE;
	}
}

inline void TServerInfoDisplayer::CheckDSConnection()
{
	if( cDBSMng.wsCDBS[0].GetSocket() == INVALID_SOCKET )
	{
		m_bValidDSConnection = FALSE;
	}
	else
	{
		m_bValidDSConnection = TRUE;
	}
}

inline void TServerInfoDisplayer::CheckFSConnection()
{
	m_bValidFSConnection = TRUE;
}

#ifdef _NEW_EXDB_
inline 	void TServerInfoDisplayer::CheckEXDSConnection()
{
	if( wsExDbCli.GetSocket() == INVALID_SOCKET ) 
	{
		m_bValidEXDSConnection = FALSE;
	}
	else
	{
		m_bValidEXDSConnection = TRUE;
	}
}
#endif

inline 	void TServerInfoDisplayer::CheckEVDSConnection()
{
	if( IsEventChipServerConnected == 0 ) 
	{
		m_bValidEVDSConnection = FALSE;
	}
	else
	{
		m_bValidEVDSConnection = TRUE;
	}
}

inline 	void TServerInfoDisplayer::CheckRKDSConnection()
{
	if( IsDevilSquareEventConnected == 0 ) 
	{
		m_bValidRKDSConnection = FALSE;
	}
	else
	{
		m_bValidRKDSConnection = TRUE;
	}
}

//--------------------------------------------------------------------------------------------

void TServerInfoDisplayer::PaintAllInfo( HWND hWnd, int iTopLeftX, int iTopLeftY )
{
	HDC	hDC;
	hDC = GetDC(hWnd);

	RECT rect;
	GetClientRect( hWnd, &rect );

	rect.top		= 20;
	rect.bottom		= 100;

	int iLineCount	= 0;

	int iOldBkMode	= SetBkMode(hDC, TRANSPARENT); 
	

	// 1. > 중요한 연결 끊김에 대한 표시
	if( m_bValidJSConnection && m_bValidDSConnection && m_bValidFSConnection )
	{
		FillRect(hDC, &rect, m_hNormalBrush );
	}
	else
	{
		FillRect(hDC, &rect, m_hAbnormalBrush );
	}
	

	// 2. > 각 서버들과의 연결 상태 표시
	if( m_bValidJSConnection == FALSE )
	{
		SetTextColor(hDC, RGB( 0,0,255));

		TextOut( hDC, iTopLeftX, iTopLeftY + iLineCount*15 , 
				 ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDJS],
				 strlen(ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDJS]) );

		iLineCount++;
	}

	if( m_bValidFSConnection == FALSE )
	{
		SetTextColor(hDC, RGB( 0,0,255));

		TextOut( hDC, iTopLeftX, iTopLeftY + iLineCount*15 , 
				 ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDFS],
				 strlen(ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDFS]) );

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

	if( m_bValidEXDSConnection == FALSE )
	{
		SetTextColor(hDC, RGB( 0,0,255));

		TextOut( hDC, iTopLeftX, iTopLeftY + iLineCount*15 , 
				 ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDEXDS],
				 strlen(ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDEXDS]) );

		iLineCount++;
	}

	if( m_bValidEVDSConnection == FALSE )
	{
		SetTextColor(hDC, RGB( 0,0,255));

		TextOut( hDC, iTopLeftX, iTopLeftY + iLineCount*15 , 
				 ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDEVDS],
				 strlen(ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDEVDS]) );

		iLineCount++;
	}

	if( m_bValidRKDSConnection == FALSE )
	{
		SetTextColor(hDC, RGB( 0,0,255));

		TextOut( hDC, iTopLeftX, iTopLeftY + iLineCount*15 , 
				 ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDRKDS],
				 strlen(ErrorMessge[SAM_MESSAGE_INDEX_DISCONNECTEDRKDS]) );

		iLineCount++;
	}

	// 3. > 파일 로그 남김
	if(		
			m_bValidJSConnection	== FALSE
		||	m_bValidDSConnection	== FALSE 
		||	m_bValidFSConnection	== FALSE
		||	m_bValidEXDSConnection	== FALSE 
		||	m_bValidEVDSConnection	== FALSE 
		||	m_bValidRKDSConnection	== FALSE
	  )
	{
		SERVER_CONN_STATE_LOG.Output( "Server Interconnection Check - [FS/%d] [DS/%d] [EX/%d] [ES/%d] [RS/%d]",   
			m_bValidFSConnection,
			m_bValidDSConnection,
			m_bValidEXDSConnection,
			m_bValidEVDSConnection,
			m_bValidRKDSConnection
		);
	}

	
	// 4. > 서버 종류 표시
	HFONT pOldFont = (HFONT)SelectObject( hDC, m_hFont );
	SetTextColor( hDC, RGB(250,250,250) );
	
	TextOut( hDC, 250, 20,	ServerTypeText, strlen(ServerTypeText) );	
	
	SelectObject( hDC, pOldFont );

	
	SetBkMode( hDC, iOldBkMode );
	ReleaseDC(hWnd, hDC);
}

#endif
