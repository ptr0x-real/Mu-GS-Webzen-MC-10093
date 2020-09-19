/*
	<< Wenzen MultiCast Socket Class >>

	by H.J.I.
	First Build Date	: 2004-03-11
	Last Build Date		: 2004-03-11
	Version				: 00.01.00
	
	- 본 DLL은 WIN32 환경에서 멀티캐스트 그룹을 통하여 WZ 프로토콜을 주고 받기 위해 만들어진 소켓 모듈이다.
	- 이 모듈은 wsock32.lib 라이브러리를 사용한다. WSA~ 윈속함수와 함께 쓸때는 라이브러리 링크에 wsock32.lib ws2_32.lib 순서로 넣어준다.
*/


#ifndef	_MULTICAST_SOCKET_H_
#define	_MULTICAST_SOCKET_H_

#include <winsock.h>

#define DEFAULT_MULTICAST_IP		"227.100.100.100"	// 멀티캐스트 그룹 주소 (이 주소가 같은 호스트들은 아래 포트로 데이터를 공유)
#define	DEFAULT_MULTICAST_PORT		37769				// 멀티캐스트 포트

#define MAX_IPINFO					32					// 쿼리가능한 최대 IP개수
#define MAX_IPADDR_LEN				16					// IP주소 길이

#define DEFAULT_RECVBUF_SIZE		(8196)				// 기본 RECV 버퍼
#define DEFAULT_SENDBUF_SIZE		(1024)				// 기본 SEND 버퍼


class CWzMultiCastSock;									// 전방참조 선언

// RECV하여 완성된 패킷이 왔을 때 콜백하는 함수의 TYPE - (인수 : 멀티캐스트객체 포인터, 보낸주소, 버퍼시작주소, 버퍼크기)
typedef VOID (* LPCALLBACKPROC_RECVNOTIFY)	(CWzMultiCastSock * lpTHIS, INT iIPADDR, INT iHEADCODE, INT iSUBCODE, CHAR * lpcBUF, INT iBUF_SIZE);		
// ERROR 발생 시 콜백하는 함수의 TYPE - (인수 : 멀티캐스트객체 포인터, 에러번호, 에러메시지)
typedef VOID (* LPCALLBACKPROC_ERRORNOTIFY)	(CWzMultiCastSock * lpTHIS, INT iERRNUM, LPSTR szERRMSG);				


typedef struct {
	BOOL	m_bInUsed;
	CHAR	m_szIPADDR[MAX_IPADDR_LEN];
} IPINFO_NODE, * LPIPINFO_NODE;


class CWzMultiCastSock {
private :
	SOCKET			m_skSocket;
	CHAR			m_szIPADDR_JOIN[MAX_IPADDR_LEN];						// 내가 멀티캐스트 그룹에 가입한 IP

	IPINFO_NODE		m_stIPINFO_LIST[MAX_IPINFO];
	INT				m_iIPINFO_COUNT;

	UINT			m_uLastErrorNum;
	CHAR			m_szLastErrorMessage[MAX_PATH];

	HANDLE			m_hWorkerThread;										// recv() 작업스레드 핸들
	unsigned int	m_idWorkerThread;										// recv() 작업스레드 ID

	LPCALLBACKPROC_RECVNOTIFY	m_lpfnCALLBACK_RECVNOTIFY;					// RECV 콜백함수
	LPCALLBACKPROC_ERRORNOTIFY	m_lpfnCALLBACK_ERRORNOTIFY;					// ERROR 콜백함수
	
public :
	CWzMultiCastSock();
	~CWzMultiCastSock();

	INT		DoEnumIpAddress		();											// 로컬IP주소들을 쿼리 (멀티캐스팅에 사용)
	BOOL	GetEnummedIpAddress	(INT iIPINDEX, LPSTR lpszOutputIpAddress);	// DoEnumIpAddress() 이 성공한 후 특정 인덱스의 주소값을 가져온다.

	BOOL 	StartMultiCast		(LPSTR lpszIPADDR=NULL);					// 멀티캐스팅을 시작함 (lpszIPADDR가 NULL이면 주소를 쿼리하여 첫번째 주소를 자동으로 사용)
	BOOL 	ReStartMultiCast	();											// 멀티캐스팅 도중 이 함수를 호출하여 이전의 설정으로 그룹 재가입을 시도
	VOID	EndMultiCast		();											// 멀티캐스팅을 종료함

	BOOL	SendData			(LPSTR lpBUFFER, INT iBUFSIZE);				// 밀티캐스트 그룹에 데이터를 보냄

	VOID	SetErrorMsgFlag		(BOOL bErrorMsg);							// 에러 메시지 박스를 DLL에서 보일지 말지를 결정
	UINT	GetLastErrorNum		() { return m_uLastErrorNum; }				// 마지막 에러 번호를 반환
	CHAR *	GetLastErrorMsg		() { return m_szLastErrorMessage; }			// 마지막 에러 메시지를 반환

	VOID	SetRecvNotifyFunc	(											// RECV 통지를 받을 콜백함수 지정		
			LPCALLBACKPROC_RECVNOTIFY lpfnCALLBACK_RECVNOTIFY
		) {	m_lpfnCALLBACK_RECVNOTIFY = lpfnCALLBACK_RECVNOTIFY;	}
	VOID	SetErrorNotifyFunc	(											// ERROR 통지를 받을 콜백함수 지정		
			LPCALLBACKPROC_ERRORNOTIFY lpfnCALLBACK_ERRORNOTIFY
		) {	m_lpfnCALLBACK_ERRORNOTIFY = lpfnCALLBACK_ERRORNOTIFY;	}

	static unsigned int __stdcall RecvThreadProc (LPVOID lpParam=NULL);		// 부모 윈도우가 존재하지 않을 때 - recv() 작업스레드

protected :
	VOID	SaveLastError (INT iErrorNo = 0);								// 마지막 에러번호와 메시지를 저장
	VOID	SavePrivateErrorMessage (LPSTR lpszErrorMessage);				// WSAGetLaseError() 로 얻을 수 없는 에러메시지를 수동으로 저장
};


#endif


