
#include "stdafx.h"

// 안준석 추가...준일이형 전 이거 넣어야 컴파일 되네여;;
// _WINSOCKAPI_ 이 정의가 winsock2.h에 먼저 되어 있어서 그런것 같아여..
#include "Ws2tcpip.h"

#include "WzMultiCastSock.h"
#include "process.h"				// _beginthreadex()

#include "..\\include\\Prodef.h"

extern HWND g_hMainList;
extern HWND g_hNumStatic;


CWzMultiCastSock::CWzMultiCastSock()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(1,1), &wsadata);

	m_skSocket					= INVALID_SOCKET;
	m_iIPINFO_COUNT				= 0;
	m_lpfnCALLBACK_RECVNOTIFY	= NULL;
	m_lpfnCALLBACK_ERRORNOTIFY	= NULL;
	memset(m_stIPINFO_LIST, 0, sizeof(m_stIPINFO_LIST));
}


CWzMultiCastSock::~CWzMultiCastSock()
{
  shutdown (m_skSocket, 0x00);
  closesocket (m_skSocket);
  WSACleanup ();
}


INT	CWzMultiCastSock::DoEnumIpAddress			()
{
	m_iIPINFO_COUNT = 0;
	memset(m_stIPINFO_LIST, 0, sizeof(m_stIPINFO_LIST));

	char buf [128];
	gethostname (buf, sizeof (buf));

	LPHOSTENT lpHostent = gethostbyname (buf);

	in_addr * lpaddr;

	switch (lpHostent->h_addrtype) {

	case AF_INET :

		while ( (lpaddr = ((in_addr *) * lpHostent->h_addr_list ++ )) != NULL) {
			m_stIPINFO_LIST[m_iIPINFO_COUNT].m_bInUsed = TRUE;
			strcpy (m_stIPINFO_LIST[m_iIPINFO_COUNT++].m_szIPADDR, inet_ntoa (*lpaddr));
			if (m_iIPINFO_COUNT>=MAX_IPINFO)	break;
		}

		break;
	}
	
	return m_iIPINFO_COUNT;
}


BOOL CWzMultiCastSock::GetEnummedIpAddress		(INT iIPINDEX, LPSTR lpszOutputIpAddress)
{
	if (iIPINDEX < 0 || iIPINDEX >= MAX_IPINFO)
		return FALSE;
	
	if (m_stIPINFO_LIST[iIPINDEX].m_bInUsed ==TRUE) {
		if (lpszOutputIpAddress) {
			memcpy (lpszOutputIpAddress, m_stIPINFO_LIST[iIPINDEX].m_szIPADDR, MAX_IPADDR_LEN);
			return TRUE;
		}
	}
	
	return FALSE;
}


BOOL CWzMultiCastSock::StartMultiCast			(LPSTR lpszIPADDR)		// 밀티캐스팅을 시작함
{

	if (m_skSocket != INVALID_SOCKET) {
		// 이전에 소켓이 생성되어 있었다면 닫는다.
		closesocket(m_skSocket);
		m_skSocket = INVALID_SOCKET;
	}

	if (!lpszIPADDR) {
		// 만약 지정된 IP가 없다면 IP를 받는다.
		INT iCOUNT = DoEnumIpAddress();
		if (iCOUNT > 0) {
			char szText [MAX_IPADDR_LEN];
			BOOL bRES = FALSE;
			bRES = GetEnummedIpAddress(0, szText);
			if (bRES) {
				memcpy (m_szIPADDR_JOIN, szText, MAX_IPADDR_LEN);
				m_szIPADDR_JOIN[MAX_IPADDR_LEN-1] = 0;
			}
			else {
				return FALSE;
			}
		}
		else {
			return FALSE;
		}
	}
	else {
		memcpy (m_szIPADDR_JOIN, lpszIPADDR, MAX_IPADDR_LEN);
		m_szIPADDR_JOIN[MAX_IPADDR_LEN-1] = 0;
	}

	m_skSocket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_skSocket == INVALID_SOCKET) {
		m_skSocket = INVALID_SOCKET;
		return FALSE;
	}

	SOCKADDR_IN sockaddr;
	sockaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons (DEFAULT_MULTICAST_PORT);

	INT iERR;

	iERR = bind (m_skSocket, (LPSOCKADDR) & sockaddr, sizeof (SOCKADDR_IN));
	if (iERR == SOCKET_ERROR) {
		// 멀티캐스팅 시 기본포트로 대기할 수 있도록 소켓에 전체주소(0) 를 바인딩한다.
		SaveLastError();
		closesocket(m_skSocket);
		m_skSocket = INVALID_SOCKET;
		return FALSE;
	}

	ip_mreq mreq;
	memset((char*)&mreq, 0, sizeof(ip_mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(DEFAULT_MULTICAST_IP);
	mreq.imr_interface.s_addr = inet_addr(m_szIPADDR_JOIN);
	
	iERR = setsockopt(m_skSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) & mreq, sizeof (mreq));
	if (iERR == SOCKET_ERROR) {
		SaveLastError();
		closesocket(m_skSocket);
		m_skSocket = INVALID_SOCKET;
		return FALSE;
	}

	unsigned long addr = inet_addr(m_szIPADDR_JOIN);
	iERR = setsockopt(m_skSocket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr));
	if (iERR == SOCKET_ERROR) {
		SaveLastError();
		closesocket(m_skSocket);
		m_skSocket = INVALID_SOCKET;
		return FALSE;
	}

	// 부모윈도우가 지정되지 않았다면 읽기 스레드를 생성한다.
	m_hWorkerThread = (HANDLE) _beginthreadex (NULL, NULL, RecvThreadProc, (LPVOID) this, NULL, & m_idWorkerThread);

	return TRUE;
}

BOOL CWzMultiCastSock::ReStartMultiCast			()			// 멀티캐스팅 도중 이 함수를 호출하여 이전의 설정으로 그룹 재가입을 시도
{
	EndMultiCast();

	return StartMultiCast(m_szIPADDR_JOIN);
}


VOID CWzMultiCastSock::EndMultiCast				()			// 밀티캐스팅을 종료함
{
	if (m_skSocket != INVALID_SOCKET) {
		// 이전에 소켓이 생성되어 있었다면 닫는다.
		closesocket(m_skSocket);
		m_skSocket = INVALID_SOCKET;
	}

	if (m_hWorkerThread != INVALID_HANDLE_VALUE) {
		TerminateThread(m_hWorkerThread, 0);
		CloseHandle(m_hWorkerThread);
		m_hWorkerThread = INVALID_HANDLE_VALUE;
	}
}


BOOL CWzMultiCastSock::SendData					(LPSTR lpBUFFER, INT iBUFSIZE)		// 밀티캐스트 그룹에 데이터를 보냄
{
	if (!lpBUFFER)	
		return FALSE;

	SOCKADDR_IN sockaddr;
	sockaddr.sin_family			= AF_INET;
	sockaddr.sin_addr.s_addr	= inet_addr (DEFAULT_MULTICAST_IP);
	sockaddr.sin_port			= htons (DEFAULT_MULTICAST_PORT);

	int iSENDBYTES = sendto (m_skSocket, lpBUFFER, iBUFSIZE, 0, (LPSOCKADDR) & sockaddr, sizeof(sockaddr));

	if (iSENDBYTES == SOCKET_ERROR) {
		SaveLastError();
		closesocket(m_skSocket);
		m_skSocket = INVALID_SOCKET;
		return FALSE;
	}
	else if (iSENDBYTES < -1) {
		SaveLastError();
		closesocket(m_skSocket);
		m_skSocket = INVALID_SOCKET;
		return FALSE;
	}

	return TRUE;
}


VOID CWzMultiCastSock::SaveLastError (INT iErrorNo)
{
	if (!iErrorNo)
		m_uLastErrorNum = WSAGetLastError();
	else
		m_uLastErrorNum = iErrorNo;

	memset (m_szLastErrorMessage, 0, sizeof(m_szLastErrorMessage));

	FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_uLastErrorNum, 0, m_szLastErrorMessage, sizeof (m_szLastErrorMessage), NULL);

	if (m_lpfnCALLBACK_ERRORNOTIFY)
		(* m_lpfnCALLBACK_ERRORNOTIFY) (this, m_uLastErrorNum, m_szLastErrorMessage);
}


VOID CWzMultiCastSock::SavePrivateErrorMessage (LPSTR lpszErrorMessage)
{
	m_uLastErrorNum = 0;
	strcpy (m_szLastErrorMessage, lpszErrorMessage);

	if (m_lpfnCALLBACK_ERRORNOTIFY)
		(* m_lpfnCALLBACK_ERRORNOTIFY) (this, m_uLastErrorNum, m_szLastErrorMessage);
}


unsigned int _stdcall CWzMultiCastSock::RecvThreadProc (LPVOID lpParam)
{
	char cBUFFER [1024];
	SOCKADDR_IN sockaddr;
	int isockaddr_len = sizeof(sockaddr);
	
	CWzMultiCastSock * lpCMSock = (CWzMultiCastSock *) lpParam;

	lpCMSock->m_skSocket;

	while (1) {
		int iRecvLen = recvfrom (lpCMSock->m_skSocket, cBUFFER, sizeof (cBUFFER), 0, (LPSOCKADDR) & sockaddr, & isockaddr_len);

		if (iRecvLen ==	SOCKET_ERROR) {
			lpCMSock->SaveLastError();
			closesocket(lpCMSock->m_skSocket);
			lpCMSock->m_skSocket = INVALID_SOCKET;
			break;
		}
		else if (iRecvLen < 0 ) {
			lpCMSock->SavePrivateErrorMessage("RecvThreadProc() ERROR : iRecvLen < 0");
			closesocket(lpCMSock->m_skSocket);
			lpCMSock->m_skSocket = INVALID_SOCKET;
			break;
		}

		// 실제로 받은 패킷을 파싱
		INT iSize = 0;
		BYTE btHeadCode = 0, btSubCode = 0;				// sub code can be no use. (most of time it'll be a trash)

		switch ( (unsigned char) cBUFFER [0] ) {

		case 0xC1 :	// packet size is byte range ( 1 ~ 255 )
			iSize = ((LPPBMSG_HEAD)(cBUFFER))->size;
			btHeadCode = ((LPPBMSG_HEAD) (cBUFFER))->headcode;
			btSubCode = ((LPPBMSG_HEAD2) (cBUFFER))->subcode;
			if (lpCMSock->m_lpfnCALLBACK_RECVNOTIFY)
				(* lpCMSock->m_lpfnCALLBACK_RECVNOTIFY) (lpCMSock, sockaddr.sin_addr.s_addr, btHeadCode, btSubCode, cBUFFER, iRecvLen);
			break;

		case 0xC2 :	// packet size is word range ( 1 ~ 65535 )
			iSize = ((INT)  (((LPPWMSG_HEAD)(cBUFFER))->sizeH << 8)) + ((LPPWMSG_HEAD)(cBUFFER))->sizeL;
			btHeadCode = ((LPPWMSG_HEAD) (cBUFFER))->headcode;
			btSubCode = ((LPPWMSG_HEAD2) (cBUFFER))->subcode;
			if (lpCMSock->m_lpfnCALLBACK_RECVNOTIFY)
				(* lpCMSock->m_lpfnCALLBACK_RECVNOTIFY) (lpCMSock, sockaddr.sin_addr.s_addr, btHeadCode, btSubCode, cBUFFER, iRecvLen);
			break;

		default :	// must be error
			break;
		}
	}
	
	return 0;
}








