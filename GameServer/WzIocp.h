// WzIocp.h: interface for the CWzIocp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZIOCP_H__719DF5BB_F4BF_4810_BEC6_0D0E056012B2__INCLUDED_)
#define AFX_WZIOCP_H__719DF5BB_F4BF_4810_BEC6_0D0E056012B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Wzqueue.h"

#define MAX_DATA_SIZE		8192



typedef struct //_PER_IO_CONTEXT
{
	OVERLAPPED					Overlapped;
	WSABUF						wsabuf;
	LPBYTE						pBuffer;
	int							nTotalBytes;
	int							nSentBytes;
	int							IOOperation;
	ListNode*					pQList;
	//struct _PER_IO_CONTEXT		*pIOContextForward;

} PER_IO_CONTEXT_BUFFER, * LPPER_IO_CONTEXT_BUFFER;

typedef struct //_PER_SOCKET_CONTEXT
{
	SOCKET			m_socket;
	int				nIndex;
	PER_IO_CONTEXT_BUFFER	IOContext[2];			// 0 : recv, 1 : send
	WZQueue*        WzSendQ;
	DWORD			dwIOCount;	

} PER_SOCKET_IOCONTEXT, *LPPER_SOCKET_IOCONTEXT;

class CWzIocp  
{
public:
	int			m_ServerPort;
	SOCKET		m_Listen;
	SOCKET		m_socket;
	sockaddr_in	m_Addr;
	HANDLE		m_CompletionPort;

		
	LPPER_SOCKET_IOCONTEXT	m_lpIOContext;

public:
	CWzIocp();
	virtual ~CWzIocp();

	BOOL	Initialize(char *ipaddr, int port);
	BOOL	Run(int ServerPort);
	BOOL	CreateSocketThread();
	BOOL	CreateSocket();
	BOOL	ConnectSocket();
	BOOL	ContextInit();
	BOOL	RecvInitContext();
	BOOL	Run();	
	BOOL	CreateIocp();
	BOOL    SendData(char *buffer, DWORD dwSize);

private :
	CRITICAL_SECTION	criti;
	

};

#endif // !defined(AFX_WZIOCP_H__719DF5BB_F4BF_4810_BEC6_0D0E056012B2__INCLUDED_)
