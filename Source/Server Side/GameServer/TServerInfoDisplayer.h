#ifndef __TSERVERINFODISPLAYER_DEF__
#define __TSERVERINFODISPLAYER_DEF__

class TServerInfoDisplayer
{

public:
	TServerInfoDisplayer();
	virtual ~TServerInfoDisplayer();

public:	
	void	Run( HWND hWnd );

	void	CheckLogFileSize();
	void	CheckPerformance();
	
	void	CheckJSConnection();
	void	CheckFSConnection();
	void	CheckDSConnection();
#ifdef _NEW_EXDB_
	void	CheckEXDSConnection();
#endif
	void	CheckEVDSConnection();
	void	CheckRKDSConnection();
	
	void	InitGDIObject();
	void	DelGDIObject();

	void	PaintAllInfo( HWND hWnd, int iTopLeftX = 0, int iTopLeftY = 20 );

private:
	BOOL	m_bValidJSConnection;
	BOOL	m_bValidFSConnection;
	BOOL	m_bValidDSConnection;
	BOOL	m_bValidEXDSConnection;
	BOOL	m_bValidEVDSConnection;
	BOOL	m_bValidRKDSConnection;
	
	HFONT	m_hFont;
	HBRUSH	m_hNormalBrush;
	HBRUSH	m_hAbnormalBrush;

};

extern TServerInfoDisplayer g_ServerInfoDisplayer;


#endif