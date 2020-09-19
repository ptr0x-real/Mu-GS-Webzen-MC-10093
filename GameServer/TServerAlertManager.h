#ifndef __TSERVERALERTMANAGER_DEF__
#define __TSERVERALERTMANAGER_DEF__

// ���������� ���� ����������
// ��񼭹��� ���� ����������

// �α� ����� Ŀ������.

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