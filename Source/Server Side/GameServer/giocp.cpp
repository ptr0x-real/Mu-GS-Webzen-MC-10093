#include "stdafx.h"
#include <stdio.h>
#include "Include\Prodef.h"
#include "Common\Winutil.h"
#include <winsock2.h>

#include "IpCache.h"
#include "User.h"
#include "Protocol.h"
#include "Gameserver.h"

#include "WzQueue.h"
#include "giocp.h"
#include "SimpleModulus.h"

#include "spe.h"
#include "OffExp.h"

#include "NSerialCheck.h"

#include "Include\define.h"

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
#include "AntiHack.h"
#endif


#ifdef MUTE_EVENT_MESSAGE	// 공지 다 버린다
extern bool		g_bMuteServerMessage;
#endif

enum {
	RECV_IO,
	SEND_IO
} SOCKET_FLAG;


CRITICAL_SECTION criti;

extern int		gLanguage;

DWORD           g_dwThreadCount = 0;        //worker thread count 
HANDLE          g_ThreadHandles[MAX_WORKER_THREAD]; 
SOCKET			g_Listen = INVALID_SOCKET;
int				g_ServerPort;
HANDLE			g_CompletionPort=NULL;
HANDLE			g_IocpThreadHandle;
LPBYTE			ExSendBuf;


extern NSerialCheck	gNSerialCheck[];

int		IoSend(int aIndex, LPBYTE lpMsg, DWORD dwSize);
BOOL	RecvDataParse(LPPER_IO_CONTEXT	lpIOContext, int uIndex);
void	CloseClient ( LPPER_SOCKET_CONTEXT lpPerSocketContext, BOOL bGraceful ) ;
int		IoSendSecond(LPPER_SOCKET_CONTEXT lpPerSocketContext);
int		IoMoreSend(LPPER_SOCKET_CONTEXT lpPerSocketContext);

void	ResponErrorCloseClient ( int index );

DWORD	WINAPI IocpServerWorker(LPVOID p);
DWORD   WINAPI ServerWorkerThread(LPVOID CompletionPortID);

void GiocpInit()
{
	ExSendBuf = new BYTE [MAX_BUFF_SIZE*2];
}

void GiocpDelete()
{
	delete ExSendBuf;
}

#ifdef PACKET_KUZIMO
extern void WriteLog(const char *pFilename, char *pszLog, ...);
#endif

#ifdef HACKTOOL_KUZIMO
BOOL IsHackTool(BYTE headcode, int size, int uIndex, BYTE xcode)
{
	if(CHECK_LIMIT( uIndex, MAX_OBJECT ))
	{
		int				iOutPacketCount;
		int				iOutEncryptPacketCount;
		unsigned int	iOutPacketTotalSize;
		
		gObj[uIndex].m_kRecvPacketStatistics.GetStatistics(iOutPacketCount, iOutEncryptPacketCount, iOutPacketTotalSize);
		
		if(gObj[uIndex].m_kRecvPacketStatistics.Process() == PacketStatistics::PS_RESET) // 시간되면 통계치가 리셋된다.
		{
			// 초당 데이터가 잡힌다.
//				char temp[1000];
//				wsprintf(temp, "PacketCount = %d, iOutEncryptPacketCount = %d, PacketTotalSize = %d\n", iOutPacketCount, iOutEncryptPacketCount, iOutPacketTotalSize);
//				OutputDebugString(temp);
		}
		
		if(iOutPacketCount > PACKET_NUMBER_PER_SECOND)
		{

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
			LogAddTD_TempLog(", [HACKTOOL] : Packets Per Second = %d,"
				"account [%s], ip [%s], name:%s, HEAD:%x (%s,%d), State:%d", 
				iOutPacketCount,
				gObj[uIndex].AccountID, 
				gObj[uIndex].Ip_addr,
				gObj[uIndex].Name, 
				headcode, 
				__FILE__, __LINE__, 
				gObj[uIndex].Connected);
#endif
			LogAddC(LOGC_RED,"[HACKTOOL] : Packets Per Second = %d"
				"ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
				iOutPacketCount,
				gObj[uIndex].Ip_addr,
				gObj[uIndex].AccountID, 
				gObj[uIndex].Name, 
				headcode, 
				__FILE__, __LINE__, 
				gObj[uIndex].Connected);
			return TRUE;
		}
		
		// 0xC3: 친구 삭제, 0xC8: 메모 삭제
		if (headcode == 0xC3 || headcode == 0xC8)
		{
			return FALSE;
		}
		
		gObj[uIndex].m_kRecvPacketStatistics.AddPacketInfo(((xcode == 0xC3) || (xcode == 0xC4))? true : false, size);
	}
	
	return FALSE;
}

#endif // HACKTOOL_KUZIMO

int CreateGIocp(int server_port)
{
	DWORD	ThreadID;

#ifdef PACKET_KUZIMO
	WriteLog("PacketLog.txt", "\n\n\n**************************** Server Start\n");
#endif
	
	g_ServerPort = server_port;
	if ((g_IocpThreadHandle = CreateThread(NULL, 0, IocpServerWorker, (LPVOID)NULL,
			0, &ThreadID)) == NULL)
	{
		LogAdd("CreateThread() failed with error %d", GetLastError());
		return FALSE;
	}
	return TRUE;
}

void DestroyGIocp()
{
	LogAddTD("Error-L10");
	closesocket(g_Listen);

	for(DWORD dwCPU = 0; dwCPU < g_dwThreadCount; dwCPU++)
	{
		TerminateThread(g_ThreadHandles[dwCPU], 0);
	}
	TerminateThread(g_IocpThreadHandle, 0);

	if( g_CompletionPort )
	{
		CloseHandle(g_CompletionPort);
		g_CompletionPort = NULL;
	}
}

BOOL CreateListenSocket(void) 
{	
	SOCKADDR_IN InternetAddr;
	int			nRet;

	g_Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (g_Listen == INVALID_SOCKET)
	{
		LogAdd("WSASocket() failed with error %d", WSAGetLastError());
		return FALSE;
	} 
	
	InternetAddr.sin_family			= AF_INET;
	InternetAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	InternetAddr.sin_port			= htons(g_ServerPort);
	
	nRet = bind(g_Listen, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr));
	if( nRet == SOCKET_ERROR )
	{
		MsgBox(lMsg.Get(567));
		SendMessage(ghWnd, WM_CLOSE, 0,0);
		return FALSE;
	}

	nRet = listen(g_Listen, 5);

	if (nRet == SOCKET_ERROR)
	{
		LogAdd("listen() failed with error %d", WSAGetLastError());
		return FALSE;
	} 
	return TRUE;
}


DWORD WINAPI IocpServerWorker(LPVOID p)
{
	SYSTEM_INFO SystemInfo;
	DWORD		ThreadID;
	SOCKET		Accept;
	int			nRet;
	int			ClientIndex;
	SOCKADDR_IN cAddr;
	IN_ADDR		cInAddr;
	int			cAddrlen = sizeof( cAddr );

	LPPER_SOCKET_CONTEXT lpPerSocketContext = NULL; 

	DWORD RecvBytes;
	DWORD Flags=0;
		
	InitializeCriticalSection(&criti);

	GetSystemInfo(&SystemInfo);
	g_dwThreadCount = SystemInfo.dwNumberOfProcessors * 2;

	if ( 2 * SystemInfo.dwNumberOfProcessors > MAX_WORKER_THREAD )
	{
		g_dwThreadCount = MAX_WORKER_THREAD;
		LogAdd(" **** Worker Thread Max Count Fix 16ea ****");
	}
	
    __try  
    { 
		g_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if ( g_CompletionPort == NULL)
		{
			LogAdd("CreateIoCompletionPort failed with error: %d", GetLastError());
			__leave;
		}
		
		for(DWORD dwCPU = 0; dwCPU < g_dwThreadCount; dwCPU++)
		{
			HANDLE ThreadHandle;
			// Create a server worker thread and pass the completion port to the thread.
			
			ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, g_CompletionPort, 0, &ThreadID);
			if ( ThreadHandle == NULL)
			{
				LogAdd("CreateThread() failed with error %d", GetLastError());
				__leave;
			}
			g_ThreadHandles[dwCPU] = ThreadHandle;
			CloseHandle(ThreadHandle);
		}

		if (!CreateListenSocket() )
			__leave;

		while(TRUE)		
		{
			Accept = WSAAccept(g_Listen, (LPSOCKADDR)&cAddr, &cAddrlen, NULL, 0);
			if (Accept==SOCKET_ERROR)
			{
				EnterCriticalSection(&criti);
				LogAdd("WSAAccept() failed with error %d", WSAGetLastError());
				LeaveCriticalSection(&criti);
				continue;
			}
			EnterCriticalSection(&criti);

			memcpy( &cInAddr, &cAddr.sin_addr.s_addr, 4 );

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
			
			IP_BLOCK pIpBlock;
			pIpBlock.dwBlockTime = GetTickCount();
			memcpy(pIpBlock.Ip_addr, inet_ntoa(cInAddr), 15);
			
			if( ChkBlockIp(&pIpBlock) == true )
			{
				//LogAddTD_TempLog(", [TempLog - BlockIP], IP [%s], Fail", gObj[ClientIndex].Ip_addr);
				closesocket(Accept);
				LeaveCriticalSection(&criti);
				continue;
			}
			
#endif	//MODIFY_CONNNECT_HACK_ATTACK_20090409

			ClientIndex = gObjAddSearch(Accept, inet_ntoa(cInAddr));
			if( ClientIndex == -1 ) 
			{
				LogAddL("error-L2 : ClientIndex = -1");
				closesocket(Accept);
				LeaveCriticalSection(&criti);
				continue;
			}
			if( UpdateCompletionPort(Accept, ClientIndex, TRUE) == FALSE )
			{ 
				LogAddL("error-L1 : %d %d CreateIoCompletionPort failed with error %d", Accept, ClientIndex, GetLastError());
				closesocket(Accept);
				LeaveCriticalSection(&criti);
				continue;
			}
			if( gObjAdd(Accept, inet_ntoa(cInAddr), ClientIndex) == -1 )
			{
				LogAddL("error-L1 : %d %d gObjAdd() failed with error %d", Accept, ClientIndex, GetLastError());
				LeaveCriticalSection(&criti);
				closesocket(Accept);
				continue;
			}
			
			memset(&gObj[ClientIndex].PerSocketContext->IOContext[0].Overlapped, 0, sizeof(OVERLAPPED));
			memset(&gObj[ClientIndex].PerSocketContext->IOContext[1].Overlapped, 0, sizeof(OVERLAPPED));
			gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf.buf = gObj[ClientIndex].PerSocketContext->IOContext[0].Buffer;
			gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf.len = MAX_BUFF_SIZE;
			gObj[ClientIndex].PerSocketContext->IOContext[0].nTotalBytes = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[0].nSentBytes = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO    = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[0].nSecondOfs = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[0].IOOperation = RECV_IO;
			
			gObj[ClientIndex].PerSocketContext->IOContext[1].wsabuf.buf = gObj[ClientIndex].PerSocketContext->IOContext[0].Buffer;
			gObj[ClientIndex].PerSocketContext->IOContext[1].wsabuf.len = MAX_BUFF_SIZE;
			gObj[ClientIndex].PerSocketContext->IOContext[1].nTotalBytes= 0;
			gObj[ClientIndex].PerSocketContext->IOContext[1].nSentBytes = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[1].nWaitIO    = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[1].nSecondOfs = 0;
			gObj[ClientIndex].PerSocketContext->IOContext[1].IOOperation = SEND_IO;
			gObj[ClientIndex].PerSocketContext->m_socket = Accept;
			gObj[ClientIndex].PerSocketContext->nIndex   = ClientIndex;

			nRet = WSARecv(Accept, &(gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf), 1, &RecvBytes, &Flags,
				&(gObj[ClientIndex].PerSocketContext->IOContext[0].Overlapped), NULL);
			
			if( nRet == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING )
			{
				LogAddL("error-L1 : WSARecv() failed with error %d", WSAGetLastError());
				gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 4;
				CloseClient(gObj[ClientIndex].PerSocketContext, FALSE);
				LeaveCriticalSection(&criti);
				continue;
			}
			else
			{
				gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 1;
				gObj[ClientIndex].PerSocketContext->dwIOCount++;
			}
			LeaveCriticalSection(&criti);
			SCPJoinResultSend( ClientIndex, 0x01);

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
			LogAddTD_TempLog(", [Connect], Account [%s], IP [%s], Success",gObj[ClientIndex].Name, gObj[ClientIndex].Ip_addr);
#endif

		}	// while
	}	// __try
	__finally  
	{
		if( g_CompletionPort )
		{
			for (DWORD i = 0; i < g_dwThreadCount; i++) 
					PostQueuedCompletionStatus(g_CompletionPort, 0, 0, NULL); 
		}
		if( g_CompletionPort )
		{
			CloseHandle(g_CompletionPort);
			g_CompletionPort = NULL;
		}
		if (g_Listen != INVALID_SOCKET)  
		{ 
            closesocket(g_Listen);  
            g_Listen = INVALID_SOCKET; 
		} 
	}
	return TRUE;
}


DWORD WINAPI ServerWorkerThread(LPVOID CompletionPortID)
{
	HANDLE	CompletionPort = (HANDLE) CompletionPortID;
	DWORD	dwIoSize;
	DWORD	RecvBytes;
	DWORD	Flags;
	DWORD	dwSendNumBytes = 0;
	BOOL	bSuccess = FALSE; 
	int		nRet;
	int     ClientIndex;

	srand(time(NULL));

	LPPER_SOCKET_CONTEXT	lpPerSocketContext = NULL;
	LPOVERLAPPED			lpOverlapped = NULL; 
	LPPER_IO_CONTEXT		lpIOContext = NULL;
	

	while(TRUE)
	{
		bSuccess = GetQueuedCompletionStatus(
			CompletionPort,
			&dwIoSize,
			(LPDWORD)&ClientIndex,
			&lpOverlapped,
			INFINITE
		);

		if( !bSuccess )
		{
			if( lpOverlapped != NULL ) 
			{
				DWORD errorcode = GetLastError();

				if( ERROR_NETNAME_DELETED != errorcode && 
					ERROR_CONNECTION_ABORTED != errorcode && 
					ERROR_OPERATION_ABORTED != errorcode &&
					ERROR_SEM_TIMEOUT != errorcode)
				{
					//EnterCriticalSection(&criti);
					LogAdd( "Error Thread : GetQueueCompletionStatus( %d )", errorcode );
					//LeaveCriticalSection(&criti);
				}

				//if(ClientIndex >= ALLOC_USEROBJECTSTART && ClientIndex < MAX_OBJECT)
				//	CloseClient(ClientIndex);

				//continue;
			}
		}

		EnterCriticalSection(&criti);
		lpPerSocketContext = gObj[ClientIndex].PerSocketContext;

		if(!bSuccess)
		{
			LogAdd( "Connection Closed, bSuccess == FALSE (Index:%d)", lpPerSocketContext->nIndex);
			CloseClient(lpPerSocketContext, FALSE);
			LeaveCriticalSection(&criti);
			continue;
		}
		
		lpPerSocketContext->dwIOCount--;
		if( dwIoSize == 0 )
		{
			LogAdd( "Connection Closed, dwIoSize == 0 (Index:%d)", lpPerSocketContext->nIndex);
			CloseClient(lpPerSocketContext, FALSE);
			LeaveCriticalSection(&criti);
			continue;
		}

		lpIOContext = (LPPER_IO_CONTEXT)lpOverlapped;

		if( lpIOContext == NULL )
		{
			LogAdd("lpIoContext is NULL");
			continue;
		}

		if( lpIOContext->IOOperation == SEND_IO )
		{
			lpIOContext->nSentBytes += dwIoSize;

			if( lpIOContext->nSentBytes >= lpIOContext->nTotalBytes )
			{
				lpIOContext->nWaitIO = 0;
				if( lpIOContext->nSecondOfs > 0 )
				{
					IoSendSecond(lpPerSocketContext);
				}
			}
			else
			{
				IoMoreSend(lpPerSocketContext);
			}
		}
		else if( lpIOContext->IOOperation == RECV_IO )
		{
			RecvBytes = 0;

			lpIOContext->nSentBytes += dwIoSize;
			
			if( RecvDataParse(lpIOContext, lpPerSocketContext->nIndex) == FALSE )
			{
				LogAdd("error-L1 : Socket Header error %d, %d", WSAGetLastError(), lpPerSocketContext->nIndex);
				CloseClient( lpPerSocketContext, FALSE);
				LeaveCriticalSection(&criti);
				continue;
			}
			else
			{
				lpIOContext->nWaitIO = 0;
				Flags = 0;
				ZeroMemory(&(lpIOContext->Overlapped), sizeof(OVERLAPPED));
				
				lpIOContext->wsabuf.len		= MAX_BUFF_SIZE-lpIOContext->nSentBytes;
				lpIOContext->wsabuf.buf		= lpIOContext->Buffer+lpIOContext->nSentBytes;
				lpIOContext->IOOperation	= RECV_IO;
				
				nRet = WSARecv(lpPerSocketContext->m_socket, &(lpIOContext->wsabuf), 1, &RecvBytes, &Flags,
					&(lpIOContext->Overlapped), NULL);
				if( nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING) )
				{
					//if ((OfflineShop[i].IsOffTrade == 1) && (ExUser[i].OffAttack != 1) && (Exafk[i].OffAfk != 1))
				if(gObj[lpPerSocketContext->nIndex].OffExp == FALSE && gObj[lpPerSocketContext->nIndex].OffAfk == FALSE && gObj[lpPerSocketContext->nIndex].Offtrade == FALSE)
				{
					LogAdd("WSARecv() failed with error %d", WSAGetLastError());
					//lpIOContext->nWaitIO = 2;
					CloseClient(lpPerSocketContext,FALSE);
				}

					//LogAdd("WSARecv() failed with error %d", WSAGetLastError());
					//lpIOContext->nWaitIO = 2;
					//CloseClient( lpPerSocketContext, FALSE);
					LeaveCriticalSection(&criti);
					continue;
				}
				else
				{	
					lpPerSocketContext->dwIOCount++;
					lpIOContext->nWaitIO = 1;
				}
			}
		}
		LeaveCriticalSection(&criti);
	}
	return TRUE;
}

extern CSimpleModulus g_SimpleModulusCS;
extern CSimpleModulus g_SimpleModulusSC;


BOOL RecvDataParse(LPPER_IO_CONTEXT	lpIOContext, int uIndex)
{
	if( lpIOContext->nSentBytes < 3 ) return TRUE;
	
	LPBYTE	recvbuf;
	int		lOfs=0;
	int		size=0;
	BYTE	headcode;
	BYTE	xcode=0;
	BYTE	*byDec = new BYTE[MAX_BUFF_SIZE*2];

	recvbuf = (LPBYTE)lpIOContext->Buffer;

	while(1)
	{
		if( recvbuf[lOfs] == 0xC1 || recvbuf[lOfs] == 0xC3 ) 
		{
			LPPBMSG_HEAD lphead = (LPPBMSG_HEAD)(recvbuf+lOfs);
			size				= lphead->size;
			headcode			= lphead->headcode;
			xcode				= recvbuf[lOfs];
		}
		else if( recvbuf[lOfs] == 0xC2 || recvbuf[lOfs] == 0xC4 ) //
		{
			LPPWMSG_HEAD lphead	= (LPPWMSG_HEAD)(recvbuf+lOfs);
			size				= ((WORD)(lphead->sizeH)<<8);
			size			   |= (WORD)(lphead->sizeL);
			headcode			= lphead->headcode;
			xcode				= recvbuf[lOfs];
		}
		else  // 헤더가 맞지 않다면..
		{
			LogAdd("error-L1 : Header error (%s %d)lOfs:%d, size:%d", __FILE__, __LINE__, lOfs, lpIOContext->nSentBytes);
			lpIOContext->nSentBytes = 0;
			delete[] byDec;
			return FALSE;
		}
/*
#ifdef HACKTOOL_KUZIMO
		if(CHECK_LIMIT( uIndex, MAX_OBJECT ))
		{
			int				iOutPacketCount;
			int				iOutEncryptPacketCount;
			unsigned int	iOutPacketTotalSize;

			gObj[uIndex].m_kRecvPacketStatistics.GetStatistics(iOutPacketCount, iOutEncryptPacketCount, iOutPacketTotalSize);

			if(gObj[uIndex].m_kRecvPacketStatistics.Process() == PacketStatistics::PS_RESET) // 시간되면 통계치가 리셋된다.
			{
				// 초당 데이터가 잡힌다.
//				char temp[1000];
//				wsprintf(temp, "PacketCount = %d, iOutEncryptPacketCount = %d, PacketTotalSize = %d\n", iOutPacketCount, iOutEncryptPacketCount, iOutPacketTotalSize);
//				OutputDebugString(temp);
			}

			if(iOutPacketCount > PACKET_NUMBER_PER_SECOND)
			{
				LogAddC(LOGC_RED,"[HACKTOOL] : Packets Per Second = %d"
								 "ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
						iOutPacketCount,
						gObj[uIndex].Ip_addr,
						gObj[uIndex].AccountID, 
						gObj[uIndex].Name, 
						headcode, 
						__FILE__, __LINE__, 
						gObj[uIndex].Connected);
				return FALSE;

			}

			gObj[uIndex].m_kRecvPacketStatistics.AddPacketInfo(((xcode == 0xC3) || (xcode == 0xC4))? true : false, size);
		}
#endif
*/
		if( size <= 0 )
		{
			LogAdd("error-L1 : size %d", size);
			delete[] byDec;
			return FALSE;
		}
		if( size <= lpIOContext->nSentBytes )	// 데이터가 크기만큼 존재하면..
		{
			if( xcode == 0xC3 )
			{
				int iSize = g_SimpleModulusCS.Decrypt( byDec+2, (recvbuf+lOfs+2), size-2);
				if ( iSize < 0)					// 깨지거나 조작된 패킷	
				{
				}
				else 
				{
					LPPBMSG_HEAD lphead;				
					/*
					if( gLanguage == 3 )
					{	// 중국의 경우 패킷 암호화 한번 더 있음										
					*/
						lphead = (LPPBMSG_HEAD)(byDec+1);
						headcode		= lphead->headcode;

						BYTE bSerial = byDec[2];

						byDec[1] = 0XC1;
						byDec[2] = (BYTE)iSize+2;

						CStreamPacketEngine_Server spe;
						spe.Clear();

						if( !spe.AddData(byDec+1, iSize+2) )
						{
							LogAddC(LOGC_RED,"error-L1 : CStreamPacketEngine Adding Error : "
										     "ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
								    gObj[uIndex].Ip_addr,
									gObj[uIndex].AccountID, 
									gObj[uIndex].Name, 
									headcode, 
									__FILE__, __LINE__, 
									gObj[uIndex].Connected);
							delete[] byDec;
							return FALSE;
						}

						if( spe.ExtractPacket(byDec) )
						{
							LogAddC(LOGC_RED,"error-L1 : CStreamPacketEngine ExtractPacket Error : "
                                             "ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
									gObj[uIndex].Ip_addr,
									gObj[uIndex].AccountID, 
									gObj[uIndex].Name, 
									headcode, 
									__FILE__, __LINE__, 
									gObj[uIndex].Connected);
							delete[] byDec;
							return FALSE;
						}
#ifdef TESTSERVER
		if( xcode == 0xC3 )
		{
			if( headcode == 0xC5)
			{
				LogAdd("암호패킷 : [0x%x]", headcode);
			}
		}
#endif

#ifdef HACKTOOL_KUZIMO
		if (IsHackTool(headcode, size, uIndex, xcode) == TRUE)
		{
			delete[] byDec;
			return FALSE;
		}
#endif // HACKTOOL_KUZIMO

						// Serial Check
						ProtocolCore(headcode, byDec, iSize, uIndex, 1, bSerial);
					/*
					}
					else
					{
						lphead = (LPPBMSG_HEAD)(byDec);
						headcode		= lphead->headcode;

						// Serial Check
						//ProtocolCore(headcode, byDec+1, iSize, uIndex, 1, byDec[0]);
						ProtocolCore(headcode, byDec, iSize, uIndex, 1, byDec[0]);
					}
					*/
				}
			}
			else if( xcode == 0xC4 )
			{
				int iSize = g_SimpleModulusCS.Decrypt( byDec+3, (recvbuf+lOfs+3), size-3);
				if ( iSize < 0)
				{
				}
				else 
				{
					LPPBMSG_HEAD lphead;			
					
					/*
					if( gLanguage == 3 )
					{	// 중국의 경우 패킷 암호화 한번 더 있음							
					*/
						lphead = (LPPBMSG_HEAD)(byDec+1);

						BYTE bSerial = byDec[3];
						/*
						

						byDec[1] = 0XC1;
						byDec[2] = (BYTE)iSize+2;

						 */
						/*byDec[0] = 0XC2;
						WORD wSize = ( WORD)iSize+3;
						byDec[1] = LOBYTE( wSize);
						byDec[2] = HIBYTE( wSize);
						*/
						byDec[1] = 0XC2;
						WORD wSize = ( WORD)iSize+3;
						byDec[2] = HIBYTE( wSize);
						byDec[3] = LOBYTE( wSize);

						
						CStreamPacketEngine_Server spe;
						spe.Clear();

						if( !spe.AddData(byDec+1, iSize+3) )
						{
							LogAddC(LOGC_RED,"error-L1 : CStreamPacketEngine Adding Error : "
											 "ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
									gObj[uIndex].Ip_addr,
									gObj[uIndex].AccountID, 
									gObj[uIndex].Name, 
									headcode, 
									__FILE__, __LINE__, 
									gObj[uIndex].Connected);
							delete[] byDec;
							return FALSE;
						}

						if( spe.ExtractPacket(byDec) )
						{
							LogAddC(LOGC_RED,"error-L1 : CStreamPacketEngine ExtractPacket Error : "
											 "ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
									gObj[uIndex].Ip_addr,
									gObj[uIndex].AccountID, 
									gObj[uIndex].Name, 
									headcode, 
									__FILE__, __LINE__, 
									gObj[uIndex].Connected);
							delete[] byDec;
							return FALSE;
						}

						headcode		= lphead->headcode;

#ifdef TESTSERVER
		if( xcode == 0xC4 )
		{
			if( headcode == 0xC5)
			{
				LogAdd("암호패킷 : [0x%x]", headcode);
			}
		}
#endif

#ifdef HACKTOOL_KUZIMO
		if (IsHackTool(headcode, size, uIndex, xcode) == TRUE)
		{
			delete[] byDec;
			return FALSE;
		}
#endif // HACKTOOL_KUZIMO

						// Serial Check
						ProtocolCore(headcode, byDec, iSize, uIndex, 1, bSerial);						
					/*
					}
					else
					{
						lphead = (LPPBMSG_HEAD)(byDec);
						headcode		= lphead->headcode;

						// Serial Check
						//ProtocolCore(headcode, byDec+1, iSize, uIndex, 1, byDec[0]);
						ProtocolCore(headcode, byDec, iSize, uIndex, 1, byDec[0]);
					}
					*/
				}
			}
			else 
			{	
				/*
				if( gLanguage == 3 )
				{	// 중국의 경우 패킷 암호화 한번 더 있음				
				*/
				if(headcode != 0x5E)
				{
					CStreamPacketEngine_Server spe;
					spe.Clear();
					if( !spe.AddData(recvbuf+lOfs, size) )
					{
						LogAddC(LOGC_RED,"error-L1 : CStreamPacketEngine Adding Error : "
										 "ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
								gObj[uIndex].Ip_addr,
								gObj[uIndex].AccountID, 
								gObj[uIndex].Name, 
								headcode, 
								__FILE__, __LINE__, 
								gObj[uIndex].Connected);
						delete[] byDec;
						return FALSE;
					}

					if( spe.ExtractPacket(byDec) )
					{
						LogAddC(LOGC_RED,"error-L1 : CStreamPacketEngine ExtractPacket Error : "
                                         "ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d", 
								gObj[uIndex].Ip_addr,
								gObj[uIndex].AccountID, 
								gObj[uIndex].Name, 
								headcode, 
								__FILE__, __LINE__, 
								gObj[uIndex].Connected);
						delete[] byDec;
						return FALSE;
					}
				}
				else
				{
					memcpy(byDec,recvbuf+lOfs,size);
				}
					//spe.AddData(recvbuf+lOfs, size);
					//spe.ExtractPacket(byDec);

#ifdef HACKTOOL_KUZIMO
					if (IsHackTool(headcode, size, uIndex, xcode) == TRUE)
					{
						delete[] byDec;
						return FALSE;
					}
#endif // HACKTOOL_KUZIMO
					ProtocolCore(headcode, byDec, size, uIndex);
				/*
				}
				else
				{
					ProtocolCore(headcode, (recvbuf+lOfs), size, uIndex);
				}
				*/
			}
			lOfs += size;
			lpIOContext->nSentBytes -= size;			
			if( lpIOContext->nSentBytes <= 0 ) break;
		}			
		else 
		{	// recv 데이터가 아직 완성되지 않았다면..
			if( lOfs > 0 )  // 하나이상 데이터를 보낸 후라면..
			{
				if( lpIOContext->nSentBytes < 1 ) 
				{
					LogAdd("error-L1 : recvbuflen 1 %s %d", __FILE__, __LINE__);
					break;
				}
				else 
				{
					if( lpIOContext->nSentBytes < MAX_BUFF_SIZE )
					{
						memcpy(recvbuf, (recvbuf+lOfs), lpIOContext->nSentBytes); // 남은만큼 복사한다.
						LogAdd("Message copy %d",lpIOContext->nSentBytes);
					}
					break;
				}
			}
			break;
		}
	}
	delete[] byDec;
	return TRUE;
}


int DataSend(int aIndex, LPBYTE lpMsg, DWORD dwSize)
{	
#ifdef PACKET_KUZIMO
	char tempBuff[1024 * 10];

	int msgCode;
	int msgSize;

	if((PMHC_BYTE == lpMsg[0]) || (PMHCE_BYTE == lpMsg[0]))
	{
		msgCode = lpMsg[2];
		msgSize = lpMsg[1];
	}
	else
	{
		msgCode = lpMsg[3];
		msgSize = (lpMsg[1] << 8) | lpMsg[2];
	}

	wsprintf(tempBuff, "%08u	S	%-12s	UserIndex(%05d)			Msg(0x%02X)	Size(0x%05X)	", GetTickCount(), gObj[aIndex].Name, aIndex, msgCode, msgSize);

	for(int i = 0; i < dwSize; i++)
	{
		wsprintf(&tempBuff[strlen(tempBuff)], ",%02X", lpMsg[i]);
	}

	wsprintf(&tempBuff[strlen(tempBuff)], "\n");
	WriteLog("PacketLog.txt", tempBuff);
#endif

	// 음냐음냐
//	CHAR szTEMP[256];
//	LPPMSG_DEFAULT2 lpMsg3=(LPPMSG_DEFAULT2)lpMsg;
//	wsprintf(szTEMP, "(S->C)[%x-%x]\n", lpMsg3->h.headcode, lpMsg3->subcode);
//	OutputDebugString(szTEMP);

#ifdef MUTE_EVENT_MESSAGE	// 공지 다 버린다
	if (g_bMuteServerMessage == 1) {
		LPPBMSG_HEAD	h = (LPPBMSG_HEAD)lpMsg;
		if( h->c == 0xC1 && h->headcode == 0x0D )
			return TRUE;
	}
#endif
	
#ifdef UPGRADE_GAMESERVER_01_20041006
	if( aIndex < ALLOC_USEROBJECTSTART )
		return TRUE;
#endif

	if( gObj[aIndex].Offtrade == TRUE && gObj[aIndex].OffExp == TRUE && gObj[aIndex].OffAfk == TRUE)
	{
		return true;
	}

	EnterCriticalSection(&criti);
	
	if( CHECK_LIMIT(aIndex, MAX_OBJECT) == FALSE )
	{
		LogAdd("error-L2 : Index(%d) %x %x %x ", dwSize, lpMsg[0],lpMsg[1],lpMsg[2]);
		LeaveCriticalSection(&criti);
		return FALSE;
	}

	DWORD SendBytes;
	LPPER_SOCKET_CONTEXT lpPerSocketContext;
	LPBYTE SendBuf;

	if( lpMsg[0] == 0xC3 || lpMsg[0] == 0xC4 )
	{
		int iSize;
		BYTE btemp;

		if( lpMsg[0] == 0xC3 )
		{
			btemp = lpMsg[1];
			lpMsg[1] = gNSerialCheck[aIndex].GetSendSerial();

			iSize			= g_SimpleModulusSC.Encrypt( ExSendBuf+2, lpMsg+1, dwSize-1);
			ExSendBuf[0]	= 0xC3;
			ExSendBuf[1]	= iSize+2;
			SendBuf			= ExSendBuf;
			dwSize			= iSize+2;

			lpMsg[1]  = btemp;
		}
		else
		{
			btemp = lpMsg[2];
			lpMsg[2] = gNSerialCheck[aIndex].GetSendSerial();

			iSize			= g_SimpleModulusSC.Encrypt( ExSendBuf+3, lpMsg+2, dwSize-2);
			ExSendBuf[0]	= 0xC4;
			ExSendBuf[1]	= HIBYTE(iSize+3);
			ExSendBuf[2]	= LOBYTE(iSize+3);
			SendBuf			= ExSendBuf;
			dwSize			= iSize+3;
			lpMsg[2] = btemp;
		}
	}
	else SendBuf = lpMsg;

	if( gObj[aIndex].Connected < 1
#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
		&& gObj[aIndex].Connected != CSS_LOGINWAIT
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
		)
	{
		LeaveCriticalSection(&criti);
		return FALSE;
	}
		
	lpPerSocketContext = gObj[aIndex].PerSocketContext;
	
	if( dwSize > MAX_BUFF_SIZE )
	{
		LogAdd("Error : Max msg(%d) %s %d", dwSize, __FILE__, __LINE__);
		CloseClient( aIndex );
		LeaveCriticalSection(&criti);
		return FALSE;
	}

	LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];

	if( lpIoCtxt->nWaitIO > 0 )	// 쓰기 작업이 아직 완료되지 않았다면.. 세컨트로 복사
	{	
		if( (lpIoCtxt->nSecondOfs+dwSize) > MAX_BUFF_SIZE-1)	// 크기를 넘었다.
		{
			LogAdd("(%d)error-L2 MAX BUFFER OVER %d %d %d [%s][%s]", aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize, gObj[aIndex].AccountID, gObj[aIndex].Name);
			lpIoCtxt->nWaitIO = 0;
			CloseClient( aIndex );
			LeaveCriticalSection(&criti);
			return TRUE;
		}
		else
		{
			// 세컨드를 일단 복사..
			memcpy(lpIoCtxt->BufferSecond+lpIoCtxt->nSecondOfs, SendBuf, dwSize);
			lpIoCtxt->nSecondOfs+=dwSize;
			//LogAdd("세컨드 복사.. %d+=%d (%s)", lpIoCtxt->nSecondOfs, dwSize, gObj[aIndex].AccountID);
			LeaveCriticalSection(&criti);
			return TRUE;
		}
	}
	else
	{
		lpIoCtxt->nTotalBytes = 0;
		if( lpIoCtxt->nSecondOfs > 0 )
		{
			memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
			lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
			//LogAdd("프론트로 복사.. %d", lpIoCtxt->nTotalBytes);
			lpIoCtxt->nSecondOfs = 0;
		}
		if( (lpIoCtxt->nTotalBytes+dwSize) > MAX_BUFF_SIZE-1)	// 크기를 넘었다.
		{
			LogAdd("(%d)error-L2 MAX BUFFER OVER %d %d [%s][%s]", aIndex, lpIoCtxt->nTotalBytes, dwSize, gObj[aIndex].AccountID, gObj[aIndex].Name);
			lpIoCtxt->nWaitIO = 0;
			CloseClient( aIndex );
			LeaveCriticalSection(&criti);
			return FALSE;
		}
		else
		{
			memcpy(lpIoCtxt->Buffer+lpIoCtxt->nTotalBytes, SendBuf, dwSize);
			lpIoCtxt->nTotalBytes += dwSize;
			//LogAdd("error-L4 : 메시지 복사.. %d", lpIoCtxt->nTotalBytes);
		}
	}
	// 쓰기작업이 완료되었다면..

	lpIoCtxt->wsabuf.buf	= (char*)lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len	= lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes	= 0;
	lpIoCtxt->IOOperation	= SEND_IO;

	if (WSASend(gObj[aIndex].m_socket, &(lpIoCtxt->wsabuf), 1, &SendBytes, 0,
		&(lpIoCtxt->Overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			lpIoCtxt->nWaitIO = 0;

			if( lpIoCtxt->wsabuf.buf[0] == 0xC1 )
			{
				LogAdd("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0], (BYTE)lpIoCtxt->wsabuf.buf[2], WSAGetLastError(), gObj[aIndex].Ip_addr);
			}
			else if( lpIoCtxt->wsabuf.buf[0] == 0xC2 )
			{
				LogAdd("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0], (BYTE)lpIoCtxt->wsabuf.buf[3], WSAGetLastError(), gObj[aIndex].Ip_addr);
			}
			CloseClient( aIndex );
			LeaveCriticalSection(&criti);
			return FALSE;
		}
	}
	else 
	{
		lpPerSocketContext->dwIOCount++;
	}
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	return TRUE;
}

int IoSendSecond(LPPER_SOCKET_CONTEXT lpPerSocketContext)
{	
	EnterCriticalSection(&criti);
	DWORD SendBytes;
	int aIndex = lpPerSocketContext->nIndex;
	
	LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];

	if( lpIoCtxt->nWaitIO > 0 )	
	{
		LeaveCriticalSection(&criti);
		return FALSE;
	}
	
	lpIoCtxt->nTotalBytes = 0;
	if( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
		//LogAdd("세컨드 프론트로 복사.. %d", lpIoCtxt->nTotalBytes);
		lpIoCtxt->nSecondOfs = 0;
	}
	else 
	{
		LeaveCriticalSection(&criti);
		return FALSE;
	}
	// 쓰기작업이 완료되었다면..

	lpIoCtxt->wsabuf.buf	= (char*)lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len	= lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes	= 0;
	lpIoCtxt->IOOperation	= SEND_IO;

	if (WSASend(gObj[aIndex].m_socket, &(lpIoCtxt->wsabuf), 1, &SendBytes, 0,
		&(lpIoCtxt->Overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			LogAdd("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), gObj[aIndex].Ip_addr);
			//lpIoCtxt->nWaitIO = 3;
			CloseClient( aIndex );
			LeaveCriticalSection(&criti);
			return FALSE;
		}
	}
	else 
	{
		lpPerSocketContext->dwIOCount++;
	}
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	return TRUE;
}

int IoMoreSend(LPPER_SOCKET_CONTEXT lpPerSocketContext)
{	
	EnterCriticalSection(&criti);
	DWORD SendBytes;
	int aIndex = lpPerSocketContext->nIndex;

	LPPER_IO_CONTEXT	lpIoCtxt = (LPPER_IO_CONTEXT)&lpPerSocketContext->IOContext[1];
	
	//LogAdd("Send IO More");

	if( (lpIoCtxt->nTotalBytes-lpIoCtxt->nSentBytes) < 0 ) 
	{
		LeaveCriticalSection(&criti);
		return FALSE;
	}
	
	//LogAddC(LOGC_RED, "%d 남은 만큼 다시 보냄", lpIoCtxt->nTotalBytes-lpIoCtxt->nSentBytes);
	// 보낸 다음부터 
	lpIoCtxt->wsabuf.buf	= (char*)lpIoCtxt->Buffer+lpIoCtxt->nSentBytes;
	lpIoCtxt->wsabuf.len	= lpIoCtxt->nTotalBytes-lpIoCtxt->nSentBytes;
	lpIoCtxt->IOOperation	= SEND_IO;

	if (WSASend(gObj[aIndex].m_socket, &(lpIoCtxt->wsabuf), 1, &SendBytes, 0,
		&(lpIoCtxt->Overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			LogAdd("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), gObj[aIndex].Ip_addr);
			CloseClient( aIndex );
			LeaveCriticalSection(&criti);
			return FALSE;
		}
	}
	else 
	{
		lpPerSocketContext->dwIOCount++;
	}
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	return TRUE;
}


BOOL UpdateCompletionPort( SOCKET sd, int ClientIndex, BOOL bAddToList ) 
{
    LPPER_SOCKET_CONTEXT lpPerSocketContext = NULL;
	HANDLE			cp;
    cp = CreateIoCompletionPort((HANDLE)sd, g_CompletionPort, (DWORD)ClientIndex, 0); 
    
	if (cp == NULL)
	{
        LogAdd("CreateIoCompletionPort: %d", GetLastError());
        return FALSE;
	}
	gObj[ClientIndex].PerSocketContext->dwIOCount = 0;
    return TRUE; 
} 

void CloseClient ( LPPER_SOCKET_CONTEXT lpPerSocketContext, BOOL bGraceful ) 
{
	int index=-1;

	/*if( lpPerSocketContext->dwIOCount > 0 ) 
	{
		if( lpPerSocketContext->dwIOCount > 1 )	// 작업 들어가 있는 상태
		{
			if( lpPerSocketContext->IOContext[1].nWaitIO > 0 )	// 쓰기 작업중이면..
			{
				LogAdd("#1 Count Zero %d ior:%d iow:%d", lpPerSocketContext->dwIOCount,lpPerSocketContext->IOContext[0].nWaitIO, lpPerSocketContext->IOContext[1].nWaitIO);
				return;
			}
		}
		else 
		{	// 작업이 들어가 있지만.. 쓰기 완료를 기다리는 중 이다.
			if( lpPerSocketContext->IOContext[1].nWaitIO > 0 )	// 
			{
				LogAdd("#2 Count Zero %d ior:%d iow:%d", lpPerSocketContext->dwIOCount,lpPerSocketContext->IOContext[0].nWaitIO, lpPerSocketContext->IOContext[1].nWaitIO);
			}			
		}
	}
	*/
	index = lpPerSocketContext->nIndex;

#ifdef PACKET_KUZIMO
	WriteLog("PacketLog.txt", "%08u	S	%-12s	UserIndex(%05d)	CloseClient(Socket)\n", GetTickCount(), gObj[index].Name, index);
#endif

	if( index >= ALLOC_USEROBJECTSTART && index < MAX_OBJECT ) 
	{		
		//DevGamesX 01/05/2018 - OffExp
		if( OffExp.Update(index, 0) )
		{
			return;
		}

		if( OFFSystem.Updateoffafk(index, 0) )
		{
			return;
		}

		if( OFFtrade.Updateofftrade(index, 0) )
	{
		return;
	}

		if( INVALID_SOCKET != gObj[index].m_socket )
		{
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
			gObj[index].NPggCSAuth.Close();
#endif
			if( closesocket(gObj[index].m_socket) == SOCKET_ERROR )
			{
				if(WSAGetLastError() != 10038)
				{	
					return;
				}
			}

			gObj[index].m_socket = INVALID_SOCKET;
		}
		gObjDel(index);
	}
	return;     
}

void CloseClient ( int index )
{
#ifdef PACKET_KUZIMO
	WriteLog("PacketLog.txt", "%08u	S	%-12s	UserIndex(%05d)	CloseClient\n", GetTickCount(), gObj[index].Name, index);
#endif

	if( index < 0 || index > MAX_OBJECT-1 ) 
	{
		LogAdd("error-L1 : CloseClient index error");
		return;
	}
	if( gObj[index].Connected == 0 ) 
	{
		LogAdd("error-L1 : CloseClient connect error");
		return;
	}

	//DevGamesX 01/05/2018 - OffExp
	if( OffExp.Update(index, 0) )
	{
		return;
	}

	if( OFFSystem.Updateoffafk(index, 0) )
	{
		return;
	}

	if( OFFtrade.Updateofftrade(index, 0) )
	{
		return;
	}

	EnterCriticalSection(&criti);	

	if( gObj[index].m_socket != INVALID_SOCKET )
	{
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
		gObj[index].NPggCSAuth.Close();
#endif
		closesocket(gObj[index].m_socket);
		gObj[index].m_socket = INVALID_SOCKET;		
	}
	else
	{
		LogAdd("error-L1 : CloseClient INVALID_SOCKET");
	}

	LeaveCriticalSection(&criti);
    return;
}

void CloseClientEx(int index)
{
	if ( index < 0 || index > MAX_OBJECT-1 )
	{
		LogAdd("error-L1 : CloseClient index error");
		return;
	}

	if ( gObj[index].Connected == 0 )
	{
		LogAdd("error-L1 : CloseClient connect error");
		return;
	}
	
	EnterCriticalSection(&criti);

	if ( gObj[index].m_socket != INVALID_SOCKET )
	{
		closesocket(gObj[index].m_socket );
		gObj[index].m_socket = INVALID_SOCKET;
	}
	else
	{
		//LogAdd("error-L1 : CloseClient INVALID_SOCKET");
	}

	LeaveCriticalSection(&criti);
}

void ResponErrorCloseClient ( int index )
{ 	
	if( index < 0 || index > MAX_OBJECT-1 ) 
	{
		LogAdd("error-L1 : CloseClient index error");
		return;
	}
	if( gObj[index].Connected == 0 ) 
	{
		LogAdd("error-L1 : CloseClient connect error");
		return;
	}

	//DevGamesX 01/05/2018 - OffExp
	if( OffExp.Update(index, 0) )
	{
		return;
	}

	if( OFFSystem.Updateoffafk(index, 0) )
	{
		return;
	}

	if( OFFtrade.Updateofftrade(index, 0) )
	{
		return;
	}

	EnterCriticalSection(&criti);
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	gObj[index].NPggCSAuth.Close();
#endif
	closesocket(gObj[index].m_socket);
	gObj[index].m_socket = INVALID_SOCKET;

	if( gObj[index].m_socket == INVALID_SOCKET )
	{
		LogAdd("error-L1 : CloseClient INVALID_SOCKET");
	}

	gObjDel(index);
	LeaveCriticalSection(&criti);
    return;
}
