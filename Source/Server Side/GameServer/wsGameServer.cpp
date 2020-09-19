// wsGameServer.cpp: implementation of the CwsGameServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Include\\Prodef.h"
#include "User.h"
#include "Protocol.h"
#include <Winsock2.h>
#include "..\\Common\\Winutil.h"
#include "wsGameServer.h"
#include "IpCache.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CwsGameServer::CwsGameServer()
{
	m_SendSec = 0;
	m_SendSecTmp = 0;
}

CwsGameServer::~CwsGameServer()
{

}

BOOL CwsGameServer::SetSocketIndex(SOCKET socket, int aIndex)
{	
	return TRUE;
}

/*int CwsGameServer::GetSocketIndex(SOCKET socket)
{
	if( socket < 0 || socket > MAX_SOCKETINDEX-1 ) 
	{		
		return -1;
	}
	return m_SockIndex[(int)socket];
	
}
*/


BOOL CwsGameServer::CreateServer(char *ip_addr, WORD port, DWORD WinServerMsg, DWORD WinClientMsg)
{
	sockaddr_in	addr;
	int			nResult;

	if( m_hWnd == NULL ) { MsgBox("windows handle error"); return FALSE; }

	memset(&addr, 0, sizeof(sockaddr_in));
	
	addr.sin_family			= PF_INET;
    addr.sin_addr.s_addr	= htonl( INADDR_ANY );
	addr.sin_port			= htons( port );
 
	nResult = bind(m_socket, (LPSOCKADDR)&addr, sizeof(addr) );
    if( nResult == SOCKET_ERROR ) 
	{
		LogAdd("Server bind error %d", WSAGetLastError());
		closesocket(m_socket);
		return FALSE;
    }

	nResult = listen( m_socket, 4 );
    if( nResult == SOCKET_ERROR) {
		closesocket(m_socket);
		LogAdd("Server listen error %d", WSAGetLastError());
		return FALSE;
    }

	nResult = WSAAsyncSelect( m_socket, m_hWnd, WinServerMsg, FD_ACCEPT);
    if( nResult == SOCKET_ERROR) {
		closesocket(m_socket);
		LogAdd("Server WSAAsyncSelect error %d", WSAGetLastError());
        return FALSE;
    }
	//LogAdd("recvbuffer size  %d %d", GetSendBuffSize(), GetRecvBuffSize());
	/*
	LINGER ling;
	ling.l_onoff = 1;
	ling.l_linger = 5;
	setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (LPSTR)&ling, sizeof(ling));
*/
	m_WinClientMsg = WinClientMsg;
	return TRUE;
}

BOOL CwsGameServer::AcceptSocket(SOCKET & clientSocket, IN_ADDR	& cInAddr)
{
	SOCKET		cSocket;
	SOCKADDR_IN cAddr;	
	int			Len;
	int			nResult;
	
	Len = sizeof(SOCKADDR_IN);
	cSocket = accept( m_socket, (LPSOCKADDR)&cAddr, &Len);
	if( cSocket != INVALID_SOCKET )
	{
		memcpy( &cInAddr, &cAddr.sin_addr.s_addr, 4 );
		if( IpCache.AddIp(inet_ntoa(cInAddr)) == FALSE )
		{
			closesocket(cSocket);
			return FALSE;
		}

		//LogAdd("사용자가 접속요청(Accept) Address:%s, port %d socket number:%d", inet_ntoa(clientIn), ntohs(clientAddr.sin_port) , clientSocket);
		nResult = WSAAsyncSelect( cSocket, m_hWnd, m_WinClientMsg, FD_READ|FD_WRITE|FD_CLOSE );
		if( nResult == SOCKET_ERROR) {
			closesocket(cSocket);
			//LogAdd("Server WSAAsyncSelect error %d", WSAGetLastError());
			return FALSE;
		}
	}
	clientSocket = cSocket;
	
	LINGER ling;
	ling.l_onoff = 0;
	ling.l_linger = 0;
	setsockopt(clientSocket, SOL_SOCKET, SO_DONTLINGER, (LPSTR)&ling, sizeof(ling));
	
	return TRUE;
}

/*
int CwsGameServer::CloseSocketBuffer(int uindex)
{
	
	if( !CHECK_LIMIT(uindex, MAX_OBJECT) )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}
	SOCKET socket = gObj[uindex].sb.m_socket;
	Close(socket);
	return 1;
}
*/

//extern int IoSend(int , LPBYTE, DWORD);

// uindex 는 유저가 할당되어 있는 index이다.
BOOL CwsGameServer::DataSendW(int uindex, char *buf, int len)
{	
	
	//IoSend(uindex, (LPBYTE)buf, len);
	return TRUE;
	
	/*
	int nResult;
	int nDx=0;
	int nLeft;
	LPBYTE sendbuf;
	
	
	if( len < 1 ) return FALSE;

	if( !CHECK_LIMIT(uindex, MAX_OBJECT) )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}
		
	SOCKET socket = gObj[uindex].m_socket;

	if( socket == INVALID_SOCKET ) { 
#ifdef _DEBUG
		LogAdd("INVALID_SOCKET %s %d %d", __FILE__, __LINE__, uindex);
#endif
		return FALSE; 
	}

	nLeft = len;
	sendbuf		        = gObj[uindex].sb.m_SendBuf;
	int & sendbuflen	= gObj[uindex].sb.m_SendBufLen;

	if( sendbuflen+len > MAX_SENDBUFSIZE )
	{
		LogAdd("복사할 양이 버퍼보다 더 크다.");
		sendbuflen = 0;
		Close(socket);
		return FALSE;
	}

	memcpy(sendbuf+sendbuflen, buf, len);
	sendbuflen += len;

	while( nLeft > 0 )
	{
		nResult = send(socket, (char*)buf+nDx, sendbuflen-nDx, 0);

		if( nResult == SOCKET_ERROR )
		{
			if( WSAGetLastError() != WSAEWOULDBLOCK )
			{
				LogAddC(LOGC_RED, "%d Send Error FILE:%s LINE:%d Err:%d size:%d", uindex, __FILE__, __LINE__, WSAGetLastError(), len-nDx);
				//LogAdd("Close Message %s %d", __FILE__, __LINE__);				
				sendbuflen = 0;
				Close(socket);
				gObjDel(uindex);
				return FALSE;
			}
			else
			{
				LogAddC(LOGC_RED,"%d) 사용자 버퍼에 복사 cursize:%d, len:%d", uindex, sendbuflen, nLeft);
				if( ( sendbuflen+nLeft) > MAX_SENDBUFSIZE ) 
				{
					LogAdd("(%d) 버퍼가 꽉차서 더이상 저장할수 없다.. 사용자 접속 종료 시킨다. %d, %d", uindex, (sendbuflen+nLeft) , MAX_SENDBUFSIZE);
					sendbuflen = 0;
					Close(socket);
					gObjDel(uindex);					
					return FALSE;
				}
				memcpy(sendbuf, sendbuf+nDx, sendbuflen-nDx);
				break;
			}
		} // SOCKET_ERROR
		else {
			if( nResult == 0 ) break;
#ifdef _DEBUG_PROTOCOL
		//	LogAddHeadHex(2, buf, len);
#endif
		}
		nDx += nResult;
		nLeft -= nResult;
		m_SendSecTmp += nResult;
		if( nLeft <= 0 ) break;
	}	
	sendbuflen -= nDx;
	return TRUE;*/
}

// uindex 는 유저가 할당되어 있는 index이다.
BOOL CwsGameServer::DataSocketSend(SOCKET socket, char *buf, int len)
{	
	int nResult;
	int nDx=0;
	int nLeft;
	
	if( socket == INVALID_SOCKET ) { 
		LogAdd("INVALID_SOCKET %s %d", __FILE__, __LINE__);
		return FALSE;
	}
	
	nLeft = len;
	while( nLeft > 0 )
	{
		nResult = send(socket, (char*)buf+nDx, len-nDx, 0);

		if( nResult == SOCKET_ERROR )
		{
			if( WSAGetLastError() != WSAEWOULDBLOCK )
			{
				return FALSE;
			}
		} // SOCKET_ERROR
		else
		{
			nDx += nResult;
			nLeft -= nResult;
			if( nLeft <= 0 ) break;
		}
	}	
	return TRUE;
}


BOOL CwsGameServer::FDWRITE_MsgDataSend(SOCKET socket)
{
	//int nResult;
	//int nDx;
	return FALSE;

	/*
	int     uindex = gObjGetSocket(socket);
	if( uindex < 0 ) return FALSE;

	LPBYTE	sendbuf		= gObj[uindex].sb.m_SendBuf;
	int *	sendbuflen  = &gObj[uindex].sb.m_SendBufLen;
	int		nLeft		= (*sendbuflen);

	nDx = 0;
	
	while( (*sendbuflen) > 0 ) 
	{
		nResult = send(socket, (char*)sendbuf+nDx, (*sendbuflen)-nDx, 0);
		//cLogProc.Add("fd_write %d byte 메시지 보냄 ", nResult);
		LogAdd("fdwrite : %d 데이터를 보냄", nResult);
		if( nResult == SOCKET_ERROR )
		{
			if( WSAGetLastError() != WSAEWOULDBLOCK )
			{
				LogAdd("FD_WRITE send() 에러 %d", WSAGetLastError());
				Close(socket);
				gObjDel(uindex);				
				return FALSE;
			}
			else 
			{
				//cLogProc.Add("FD_WRITE send() WSAEWOULDBLOCK : %d", WSAGetLastError());
				break;
			}
		}
		else {
			if( nResult <= 0 ) {
				LogAdd("send() result is zero %d", WSAGetLastError());			
				Close(socket);
				gObjDel(uindex);
				return FALSE;
			}			
#ifdef _DEBUG_PROTOCOL
			//LogAddHeadHex(3, (char*)sendbuf, (*sendbuflen)-nDx);
#endif
		}
		nDx += nResult;
		(*sendbuflen) -= nResult;		
	}
	return TRUE;
	*/
}

int CwsGameServer::DataRecv(SOCKET socket)
{	
	/*int		nResult;
	int		uindex;
	LPBYTE	recvbuf;
	uindex = gObjGetSocket(socket);

	if( uindex < 0 ) return FALSE;
	
	recvbuf = gObj[uindex].sb.m_RecvBuf;
	int & recvbuflen = gObj[uindex].sb.m_RecvBufLen;

	if( MAX_RECVBUFSIZE <= recvbuflen ) 
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}

	nResult = recv( socket, (char*)recvbuf+recvbuflen, MAX_RECVBUFSIZE-recvbuflen, 0);
	if( nResult == 0 ) //접속이 끊겼씀.
	{
		LogAdd("접속 종료됨 UserIndex:%d Err:%d", uindex, WSAGetLastError());		
		Close(socket);
		gObjDel(uindex);
		return 1;
	}
	if( nResult == SOCKET_ERROR )
	{
		if( WSAGetLastError() == WSAEWOULDBLOCK )
		{
			return 1;
		}
		else {
			LogAdd("recv() 받기 에러 %d", WSAGetLastError());
			switch( WSAGetLastError())
			{
			case 10053 :
				LogAdd("error : 10053 에러로 종료됨 UserIndex:%d", uindex);
				Close(socket);
				gObjDel(uindex);				
				break;
			}
		}
		return 1;
	}	
	
#ifdef _DEBUG_PROTOCOL
	//LogAddHeadHex(4, (char*)recvbuf+recvbuflen, nResult);
#endif
	
	recvbuflen += nResult;

	if( recvbuflen < 3 ) {
		return 3;	
	}
	
	m_RecvSecTmp += nResult;
	
	int lOfs=0;
	int size=0;
	BYTE headcode;

	while(1)
	{
		if( recvbuf[lOfs] == 0xC1 ) //
		{
			LPPBMSG_HEAD lphead=(LPPBMSG_HEAD)(recvbuf+lOfs);
			size = lphead->size;
			headcode = lphead->headcode;
		}
		else if( recvbuf[lOfs] == 0xC2 ) //
		{
			LPPWMSG_HEAD lphead=(LPPWMSG_HEAD)(recvbuf+lOfs);
			size = ((WORD)(lphead->sizeH)<<8);
			size |= (WORD)(lphead->sizeL);
			headcode = lphead->headcode;
		}
		else  // 헤더가 맞지 않다면..
		{
			LogAdd("헤더가 올바르지 않다.(%s %d)lOfs:%d, size:%d", __FILE__, __LINE__, lOfs, recvbuflen);
			recvbuflen = 0;
			Close(socket);
			gObjDel(uindex);
			return FALSE;
		}
		if( size <= 0 )
		{
			LogAdd("size가 %d이다.", size);
			Close(socket);
			gObjDel(uindex);
			return FALSE;
		}
		if( size <= recvbuflen )	// 데이터가 크기만큼 존재하면..
		{
			ProtocolCore(headcode, (recvbuf+lOfs), size, uindex);
			lOfs += size;
			recvbuflen -= size;
			if( recvbuflen <= 0 ) break;
		}			
		else {	// recv 데이터가 아직 완성되지 않았다면..
			if( lOfs > 0 )  // 하나이상 데이터를 보낸 후라면..
			{
				if( recvbuflen < 1 ) 
				{
					LogAdd("recvbuflen 이 1보다 작다..");
					break;
				}
				else {
				//	if( (*recvbuflen) > lOfs )
					//{
					memcpy(recvbuf, (recvbuf+lOfs), recvbuflen); // 남은만큼 복사한다.
					//cLogProc.Add("%d, 남은만큼 복사했다. ofs:%d  len:%d", lphead->size, lOfs, (*recvbuflen));
					//}
					//else {
						
					//}
				}
			}
			break;
		}
	}
	*/
	return 0;
}

int CwsGameServer::SetSocketBuffer(int index, SOCKET socket, char *ip)
{
	/*
	if( !CHECK_LIMIT(index, MAX_SOCKETBUFFER) )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return FALSE;
	}
	sb[index].Clear();
	sb[index].live = 1;	
	sb[index].m_socket = socket;
	strcpy(sb[index].Ip_addr, ip);*/
	return TRUE;
}

