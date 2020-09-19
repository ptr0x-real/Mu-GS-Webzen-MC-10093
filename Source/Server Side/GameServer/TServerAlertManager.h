#ifndef __TSERVERALERTMANAGER_DEF__
#define __TSERVERALERTMANAGER_DEF__

// 인증서버와 연결 끊어졌을때
// 디비서버와 연결 끊어졌을때

// 로그 사이즈가 커졌을때.

class TServerAlertManager
{

public:
	TServerAlertManager();
	virtual ~TServerAlertManager();

public:	
	void Run( HWND hWnd );
	void ClearAllAlert();

	void CheckLogFileSize();
	void CheckJSConnection();
	void CheckDSConnection();
	
	void PaintAllAlert( HWND hWnd, int iTopLeftX = 0, int iTopLeftY = 600 );

	BOOL StopWhatsUp() { return m_bStopWhatsUp; };

private:
	BOOL m_bStopWhatsUp;

		
	BOOL m_bValidJSConnection;
	BOOL m_bValidDSConnection;

};

extern TServerAlertManager g_ServerAlertManager;


#endif