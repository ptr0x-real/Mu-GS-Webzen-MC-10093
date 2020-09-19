// wsJoinServerCli.cpp: implementation of the wsJoinServerCli class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wsShopServerCli.h"
#include "..\\Include\\Prodef.h"
#include "User.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wsShopServerCli::wsShopServerCli()
{
}

wsShopServerCli::~wsShopServerCli()
{

}

BOOL wsShopServerCli::SetProtocolCore(void (*pc)(DWORD, BYTE*, int))
{
	wsProtocolCore = pc;
	return TRUE;
}


BOOL wsShopServerCli::Connect(char *ip_addr, unsigned short port, DWORD WinMsgNum)
{
	struct hostent* host = NULL;
	sockaddr_in		addr;
	int				nResult;

	if( m_hWnd == NULL ) { LogAdd(" Connect Error %s %d",__FILE__, __LINE__); return FALSE; }

    addr.sin_family		 = PF_INET;
    addr.sin_port		 = htons( port );
    addr.sin_addr.s_addr = inet_addr(ip_addr); 

	if( addr.sin_addr.s_addr == INADDR_NONE )
	{
		host = gethostbyname(ip_addr);
		if( host == NULL )
		{
			LogAdd("server name not found [%s]", ip_addr);
			return FALSE;
		}
		CopyMemory(&addr.sin_addr, host->h_addr_list[0], host->h_length);
	}
    
	nResult = connect( m_socket, (LPSOCKADDR)&addr, sizeof(addr) );
	
	if( nResult == SOCKET_ERROR) 
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK) 
		{
			//closesocket(m_socket);
			Close(m_socket);
			return FALSE;
		}
    }
	nResult = WSAAsyncSelect( m_socket, m_hWnd, WinMsgNum, FD_READ | FD_WRITE | FD_CLOSE);
    if( nResult == SOCKET_ERROR) {
		closesocket(m_socket);
		LogAdd("Client WSAAsyncSelect error %d", WSAGetLastError());
        return FALSE;
    }

	m_SendBufLen = 0;
	SetConnect(TRUE);
	return TRUE;
}

BOOL wsShopServerCli::DataSend(char *buf, int len)
{
	if( len <= 0 ) 
	{
		return FALSE;
	}
	if( m_socket == INVALID_SOCKET ) 
	{
		return FALSE;
	}
	int nResult;
	
	int nLeft;
	int nDx=0;

	LPBYTE sendbuf;
	int *  sendbuflen;

	sendbuf		= m_SendBuf;
	sendbuflen	= &len;

#ifdef BUGFIX_WSJOINCLIENT_SENDLENGTH_20041011
	int		iOldSendBufLen	= m_SendBufLen;
	if (iOldSendBufLen < 0)
		iOldSendBufLen		= 0;
#endif
	
	if( m_SendBufLen > 0 )
	{
		if( m_SendBufLen+len < MAX_JOINSENDBUFSIZE )
		{
			memcpy(m_SendBuf+m_SendBufLen, buf, len);
			m_SendBufLen+=len;
#ifdef BUGFIX_WSJOINCLIENT_SENDLENGTH_20041011
			(*sendbuflen) = m_SendBufLen;		//	#BUGFIX_WSJOINCLIENT_SENDLENGTH_BUG_20041011
#else
			return TRUE;						//	#BUGFIX_WSJOINCLIENT_SENDLENGTH_BUG_20041011
#endif			
		}
		else
		{
			Close();
			return FALSE;
		}
	}
	nLeft = (*sendbuflen);

	while( 1 ) 
	{
		nResult = send(m_socket, (char*)buf+nDx, (*sendbuflen)-nDx, 0);
		if( nResult == SOCKET_ERROR )
		{
			if( WSAGetLastError() != WSAEWOULDBLOCK )
			{	
				LogAdd("error-L3 : %d (%s %d)", WSAGetLastError(), __FILE__, __LINE__);
				(*sendbuflen) = 0;
				Close(m_socket);
				return FALSE;
			}
			else
			{	
				if( ((*sendbuflen)+nLeft) > MAX_JOINSENDBUFSIZE ) {
					LogAddC(LOGC_RED, "error-L3 : buffer error closed %d, %d", ((*sendbuflen)+nLeft) , MAX_JOINSENDBUFSIZE);
					(*sendbuflen) = 0;
					Close(m_socket);
					return FALSE;
				}		
				if( nDx >= 0 )
				{
#ifdef BUGFIX_WSJOINCLIENT_SENDLENGTH_20041011
					int iLeftBufLen		= nLeft-iOldSendBufLen;
					if (iLeftBufLen > 0) {
						memcpy(m_SendBuf+m_SendBufLen, buf+nDx, iLeftBufLen);
						m_SendBufLen += iLeftBufLen;
					}
#else
					memcpy(m_SendBuf+m_SendBufLen, buf+nDx, nLeft);
					m_SendBufLen += nLeft;
#endif
					LogAdd("error-L3 : Copy1 : nLeft:%d len:%d nDx:%d", nLeft, m_SendBufLen, nDx);
					return TRUE;
				}
			}
		}
		else {
			if( nResult == 0 ) {
				LogAdd("error-L3 : send()  result is zero", WSAGetLastError());
				break;
			}
		}
		if( nResult > 0 )
		{
			nDx += nResult;
			nLeft -= nResult;
			(*sendbuflen) -= nResult;
			
#ifdef BUGFIX_WSJOINCLIENT_SENDLENGTH_20041011
			//	#BUGFIX_WSJOINCLIENT_SENDLENGTH_BUG_20041011
			if( m_SendBufLen > 0 )
			{
				m_SendBufLen		-= nResult;
				if (m_SendBufLen < 0)
					m_SendBufLen	= 0;
				
			}
			//	#BUGFIX_WSJOINCLIENT_SENDLENGTH_BUG_20041011
#endif

		}
		if( nLeft <= 0 ) break;		
	}
	return TRUE;
}

int wsShopServerCli::FDWRITE_MsgDataSend()
{
	int nResult;
	int nDx	  = 0;
	
	LPBYTE		sendbuf		= m_SendBuf;
	int *		sendbuflen  = &m_SendBufLen;

	if( m_SendBufLen < 1 ) return TRUE;
	
	while( (*sendbuflen) > 0 ) 
	{
		nResult = send(m_socket, (char*)sendbuf+nDx, (*sendbuflen)-nDx, 0);
		if( nResult == SOCKET_ERROR )
		{
			if( WSAGetLastError() != WSAEWOULDBLOCK )
			{
				LogAdd("FD_WRITE send() 에러 %d %d", WSAGetLastError(), (*sendbuflen));
				Close();
				return FALSE;
			}
			else 
			{
				LogAdd("FD_WRITE send() WSAEWOULDBLOCK : %d", WSAGetLastError());
				break;
			}
		}
		else {
			if( nResult <= 0 ) {
				LogAdd("send() result is zero %d", WSAGetLastError());
				break;
			}	 		
		}
		LogAdd("error-L3 : nDx %d m_SendBufLen %d", nDx, m_SendBufLen);
		if( nResult > 0 )
		{
			nDx += nResult;
			(*sendbuflen) -= nResult;
		}
		LogAdd("error-L3 : nDx %d m_SendBufLen %d", nDx, m_SendBufLen);
	}
	return TRUE;
}

int wsShopServerCli::DataRecv()
{
	int		nResult;
	LPBYTE	recvbuf;
	int *	recvbuflen;

	recvbuf		= m_RecvBuf;
	recvbuflen	= &m_RecvBufLen;

	nResult = recv( m_socket, (char*)recvbuf+(*recvbuflen), MAX_JOINRECVBUFSIZE-(*recvbuflen), 0);

	if( nResult == 0 )
	{
		// 2006.07.24 클라이어트 소켓 에러 정보 로그추가 (apple)
		LogAdd("error-L3 : closed %d - buff:%d(%d) ", WSAGetLastError(), m_RecvBufLen, MAX_JOINRECVBUFSIZE-(*recvbuflen) );
		return 1;
	}
	if( nResult == SOCKET_ERROR )
	{
		if( WSAGetLastError() == WSAEWOULDBLOCK )
		{
			return 1;
		}
		else {
			LogAdd("error-L3 : recv error %d", WSAGetLastError());
		}
		return 1;
	}
	
	(*recvbuflen) += nResult;

	if( (*recvbuflen) < 3 ) return 3;	

	int lOfs=0;
	int size=0;

	while(1)
	{
		MSG_HEADER* lpHead = (MSG_HEADER*)(recvbuf+lOfs);
		DWORD dwMsgID = lpHead->id;
		size = lpHead->size;

		if( size <= 0 )
		{
			LogAdd("error-L2 : size %d", size);
			return FALSE;
		}
		if( size <= (*recvbuflen) )	// 데이터가 크기만큼 존재하면..
		{
			(*wsProtocolCore)(dwMsgID, (recvbuf+lOfs), size);
			lOfs += size;
			(*recvbuflen) -= size;
			if( (*recvbuflen) <= 0 ) break;
		}			
		else {	// recv 데이터가 아직 완성되지 않았다면..
			if( lOfs > 0 )  // 하나이상 데이터를 보낸 후라면..
			{
				if( (*recvbuflen) < 1 ) 
				{
					//LogAdd("(*recvbuflen) 이 1보다 작다..");
					break;
				}
				else {
					memcpy(recvbuf, (recvbuf+lOfs), (*recvbuflen)); // 남은만큼 복사한다.
					break;
				}
			}
			break;
		}
	}
	return 0;
}


