// WzUdp.cpp: implementation of the WzUdp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <winsock2.h>
#include "WzQueue.h"
#include "WzUdp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD WINAPI WzUdpRecvThread(WzUdp*		lpWzUdp);
DWORD WINAPI WzUDPSendThread(LPVOID p);

WzUdp::WzUdp()
{
	Init();
}

WzUdp::~WzUdp()
{
	Close();
}

BOOL WzUdp::Init()
{
	WSADATA			wsd;

	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {        
        return FALSE;
    }
	m_Socket	= INVALID_SOCKET;
	m_dwLength  = DEFAULT_BUFFER_LENGTH;
	m_dwRecvOfs = 0;
	m_ThreadHandle = NULL;
	ProtocolCore   = NULL;
	return TRUE;
}

BOOL WzUdp::Close()
{
	TerminateThread(m_ThreadHandle, 0);
	if( m_ThreadHandle != NULL )
	{
		WaitForSingleObject( m_ThreadHandle, INFINITE );
		CloseHandle(m_ThreadHandle);
		m_ThreadHandle = NULL;
	}
	HeapFree(GetProcessHeap(), 0, m_Recvbuf);
	return TRUE;
}	

BOOL WzUdp::CreateSocket()
{
	if (m_Socket != INVALID_SOCKET) {
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	m_Socket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, NULL);
    
	if( m_Socket == INVALID_SOCKET )
    {        
        return FALSE;
    }
	return TRUE;
}

BOOL WzUdp::SendSet(char *ip, int port)
{
	m_Port					= port;
    m_SockAddr.sin_family	= AF_INET;
    m_SockAddr.sin_port		= htons(port);
	if( (m_SockAddr.sin_addr.s_addr	= inet_addr(ip)) == INADDR_NONE )
	{
		struct hostent *host=NULL;
		host = gethostbyname(ip);
		if( host )
		{
			CopyMemory(&m_SockAddr.sin_addr, host->h_addr_list[0], host->h_length);
		}
		else
		{		
			return FALSE;
		}
	}
	return TRUE;
}

BOOL WzUdp::SetProtocolCore(void (*pc)(BYTE, BYTE*, int))
{
	ProtocolCore = pc;
	return TRUE;
}


BOOL WzUdp::RecvSet(int port)
{
	m_Port						= port;
    m_SockAddr.sin_port			= htons(port);
	m_SockAddr.sin_family		= AF_INET;
    m_SockAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	
	if( bind(m_Socket, (SOCKADDR *)&m_SockAddr, sizeof(m_SockAddr)) == SOCKET_ERROR )
    {		
		return FALSE;
    }
	
	m_Recvbuf = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, m_dwLength);
    if (!m_Recvbuf)
    {        
        return FALSE;
    }
	return TRUE;
}



BOOL WzUdp::SendData(LPBYTE SendData, DWORD nSendDataLen)
{
	DWORD Ret;

	ZeroMemory(&(m_PerIoSendData.Overlapped), sizeof(OVERLAPPED));
	
	memcpy(m_PerIoSendData.Buffer, SendData, nSendDataLen);
	
	m_PerIoSendData.lOfs		= nSendDataLen;
	m_PerIoSendData.DataBuf.buf = m_PerIoSendData.Buffer;
	m_PerIoSendData.DataBuf.len = m_PerIoSendData.lOfs;
		
	Ret = WSASendTo(m_Socket, &(m_PerIoSendData.DataBuf), 1, &nSendDataLen, 
		0, (SOCKADDR*)&m_SockAddr, sizeof(m_SockAddr), &(m_PerIoSendData.Overlapped), NULL);
	if( Ret == SOCKET_ERROR )
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			//printf("WSASend() failed with error %d\n", WSAGetLastError());
			return -1;
		}
	}
	return TRUE;
}

BOOL WzUdp::MuProtocolParse(LPBYTE RecvData, int & nRecvDataLen)
{	
	int		lOfs = 0;
	int		size = 0;
	BYTE	headcode;

	if( ProtocolCore == NULL ) return FALSE;

	while( TRUE )
	{
		if( RecvData[lOfs] == 0xC1 )
		{
			size		= *(RecvData+lOfs+1);
			headcode	= *(RecvData+lOfs+2);
		}
		else if( RecvData[lOfs] == 0xC2 )
		{
			size		 = (WORD)(*(RecvData+lOfs+1));
			size	   <<= 8;
			size		|= (WORD)(*(RecvData+lOfs+2));
			headcode	 = *(RecvData+lOfs+3);
		}
		else  // 헤더가 맞지 않다면..
		{
			m_dwRecvOfs = 0;
			return FALSE;
		}
		if( size <= 0 )	// size 가 0일때는 에러..
		{
			return FALSE;
		}		
		else if( size <= nRecvDataLen )	// 하나의 패킷이 완전하다면..
		{
			(*ProtocolCore)(headcode, RecvData+lOfs, size);
			lOfs		 += size;
			m_dwRecvOfs  -= size;
			if( m_dwRecvOfs <= 0 ) break;
		}		
		else												// 데이터가 더 남았다면
		{	
			if( lOfs > 0 )									// 하나이상 데이터를 처리한 후 라면..
			{
				if( m_dwRecvOfs < 1 )
				{
					return FALSE;
				}
				else 
				{
					memcpy(RecvData, (RecvData+lOfs), m_dwRecvOfs); // 남은만큼 복사한다.
					return TRUE;
				}
			}
			break;
		}
	}
	return TRUE;
}


BOOL WzUdp::Run()
{
	if( (m_ThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WzUdpRecvThread, (void*)this, 0, &m_ThreadID) ) == NULL)
	{		
		return FALSE;
	}
	return TRUE;
}

BOOL WzUdp::RecvThread()
{
	int			ret;
    DWORD		dwSenderSize;
    SOCKADDR_IN	sender;	
	
	dwSenderSize = sizeof(sender);
	while(1)
	{		
		ret = recvfrom(m_Socket, (char*)m_Recvbuf+m_dwRecvOfs,
			DEFAULT_BUFFER_LENGTH-m_dwRecvOfs, 0, (SOCKADDR *)&sender, (int*)&dwSenderSize);

		if (ret == SOCKET_ERROR)
		{
			//printf("recvfrom() failed; %d\n", WSAGetLastError());
		}
		else if (ret == 0)
		{
		}
		else
		{
			m_dwLength  = ret;
			m_dwRecvOfs += ret;
			MuProtocolParse(m_Recvbuf, m_dwLength);
		}
	}
}

DWORD WINAPI WzUdpRecvThread(WzUdp*	lpWzUdp)
{
    lpWzUdp->RecvThread();
	return TRUE;
}

