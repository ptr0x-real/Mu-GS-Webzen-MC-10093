#ifndef __GIOCP_H__
#define __GIOCP_H__

//#define DATA_BUFSIZE	8192
#include <winsock2.h>
#include "Wzqueue.h"

// ## EXPAND_MAX_SENDBUFFER_20050413 
// 3000 -> 6000
#define MAX_BUFF_SIZE		8192*2
#define MAX_WORKER_THREAD	16

typedef struct _PER_IO_CONTEXT
{
	OVERLAPPED					Overlapped;
	WSABUF						wsabuf;
	CHAR						Buffer[MAX_BUFF_SIZE*2];
	CHAR						BufferSecond[MAX_BUFF_SIZE*2];
	int							nSecondOfs;
	int							nTotalBytes;
	int							nSentBytes;
	int 						IOOperation;
	int  						nWaitIO;			// IO 작업을 진행했다면.. 기다리게 알린다.
} PER_IO_CONTEXT, * LPPER_IO_CONTEXT;

typedef struct _PER_SOCKET_CONTEXT
{
	SOCKET			m_socket;
	int				nIndex;
	PER_IO_CONTEXT	IOContext[2];			// 0 : recv, 1 : send
	int 			dwIOCount;

} PER_SOCKET_CONTEXT, *LPPER_SOCKET_CONTEXT;

extern void GiocpInit();
extern void GiocpDelete();

int		SocketInit();
int		CreateGIocp(int server_port);
//int  IoSend(int aIndex, LPBYTE lpMsg, DWORD size);
BOOL	UpdateCompletionPort( SOCKET sd, int ClientIndex, BOOL bAddToList ) ;
void	CloseClient ( int index );
void CloseClientEx(int index);
int		DataSend(int aIndex, LPBYTE lpMsg, DWORD dwSize);
void	ResponErrorCloseClient ( int index );

extern void	DestroyGIocp();
#endif
