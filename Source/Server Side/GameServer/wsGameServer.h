// wsGameServer.h: interface for the CwsGameServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSGAMESERVER_H__BA6A3F00_3334_402A_B5F4_A399FC76D404__INCLUDED_)
#define AFX_WSGAMESERVER_H__BA6A3F00_3334_402A_B5F4_A399FC76D404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common\MyWinsockBase.h"


//#define MAX_SOCKETINDEX		65536		// 소켓 인덱스 최대 개수
//#define MAX_SOCKETBUFFER	MAX_OBJECT	// 최대 처리할 소켓 버퍼수

#define MAX_SENDBUFSIZE	 1		// 보낼수 있는 버퍼의 최대 크기
#define MAX_RECVBUFSIZE	 1		// 받을수 있는 버퍼의 최대 크기

class CSocketBuffer
{
public:
	int		live;		// 
	char	Ip_addr[16];		// ip주소
	
	SOCKET	m_socket;	// 소켓 번호

	BYTE	m_SendBuf[MAX_SENDBUFSIZE];	//
	int		m_SendBufLen;			// 버퍼크기

	BYTE	m_RecvBuf[MAX_RECVBUFSIZE];	// 보낼 버퍼
	int		m_RecvBufLen;			// 보낼 버퍼 크기 

public:

	CSocketBuffer() 
	{
		Clear();
	};
	void Clear()
	{
		live = 0;
		memset(Ip_addr, 0, 16);
		//memset(m_SendBuf, 0, MAX_SENDBUFSIZE);
		memset(m_RecvBuf, 0, MAX_RECVBUFSIZE);
		m_socket = INVALID_SOCKET;
		m_SendBufLen = 0;
		m_RecvBufLen = 0;
	};
};

class CwsGameServer : public MyWinsockBase
{


	//int		m_SockIndex[MAX_SOCKETINDEX];	// 소켓당 유저 배열 인덱스

public:	
	//CSocketBuffer	sb[MAX_SOCKETBUFFER];

	int				m_SendSec;	// 초동 보낸 바이트수
	int				m_SendSecTmp;

	int				m_RecvSec;	// 초동 받은 바이트수
	int				m_RecvSecTmp;

	DWORD	m_WinClientMsg;

public:
	//int  GetSocketIndex(SOCKET socket);
	BOOL SetSocketIndex(SOCKET socket, int aIndex);
	int SetSocketBuffer(int index, SOCKET socket, char * ip);
	BOOL AcceptSocket(SOCKET & clientSocket, IN_ADDR & cInAddr);
	int  DataRecv(SOCKET socket);
	BOOL FDWRITE_MsgDataSend(SOCKET socket);
	BOOL DataSendW(int uindex, char *buf, int len);
	BOOL DataSocketSend(SOCKET socket, char *buf, int len);
	BOOL CreateServer(char *ip_addr, WORD port, DWORD WinServerMsg, DWORD WinClientMsg);
	//char *GetIPAddress(int index);


	CwsGameServer();
	virtual ~CwsGameServer();
	
};

#endif // !defined(AFX_WSGAMESERVER_H__BA6A3F00_3334_402A_B5F4_A399FC76D404__INCLUDED_)
