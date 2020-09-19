#ifndef __MYWSBASE_H__
#define __MYWSBASE_H__

#include <winsock2.h>

class MyWinsockBase
{
private:
	BOOL    Startup();

protected:
	int		m_iMaxSockets;				// ���� �ִ� ����
	HWND	m_hWnd;					// ������ �޽����� ó���� ������ �ڵ�
	SOCKET	m_socket;				// ���� ����
	int     m_sendbufsize;			// ������ ������ ���� ũ�� 	
	int     m_recvbufsize;			// ������ �ޱ� ���� ũ��
	BOOL	m_Connect;				// ���� �Ǿ��°�?
public :
	BOOL CreateSocket(HWND hWnd);
	BOOL Close();
	BOOL Close(SOCKET socket);
	BOOL Close2(SOCKET socket);
	
	SOCKET GetSocket();
	int GetSendBuffSize();
	int GetRecvBuffSize();

	BOOL GetConnect();
	void SetConnect(BOOL connected);

	MyWinsockBase();
	~MyWinsockBase();
};

#endif


