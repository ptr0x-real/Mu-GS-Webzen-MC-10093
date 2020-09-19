/*
	<< Wenzen MultiCast Socket Class >>

	by H.J.I.
	First Build Date	: 2004-03-11
	Last Build Date		: 2004-03-11
	Version				: 00.01.00
	
	- �� DLL�� WIN32 ȯ�濡�� ��Ƽĳ��Ʈ �׷��� ���Ͽ� WZ ���������� �ְ� �ޱ� ���� ������� ���� ����̴�.
	- �� ����� wsock32.lib ���̺귯���� ����Ѵ�. WSA~ �����Լ��� �Բ� ������ ���̺귯�� ��ũ�� wsock32.lib ws2_32.lib ������ �־��ش�.
*/


#ifndef	_MULTICAST_SOCKET_H_
#define	_MULTICAST_SOCKET_H_

#include <winsock.h>

#define DEFAULT_MULTICAST_IP		"227.100.100.100"	// ��Ƽĳ��Ʈ �׷� �ּ� (�� �ּҰ� ���� ȣ��Ʈ���� �Ʒ� ��Ʈ�� �����͸� ����)
#define	DEFAULT_MULTICAST_PORT		37769				// ��Ƽĳ��Ʈ ��Ʈ

#define MAX_IPINFO					32					// ���������� �ִ� IP����
#define MAX_IPADDR_LEN				16					// IP�ּ� ����

#define DEFAULT_RECVBUF_SIZE		(8196)				// �⺻ RECV ����
#define DEFAULT_SENDBUF_SIZE		(1024)				// �⺻ SEND ����


class CWzMultiCastSock;									// �������� ����

// RECV�Ͽ� �ϼ��� ��Ŷ�� ���� �� �ݹ��ϴ� �Լ��� TYPE - (�μ� : ��Ƽĳ��Ʈ��ü ������, �����ּ�, ���۽����ּ�, ����ũ��)
typedef VOID (* LPCALLBACKPROC_RECVNOTIFY)	(CWzMultiCastSock * lpTHIS, INT iIPADDR, INT iHEADCODE, INT iSUBCODE, CHAR * lpcBUF, INT iBUF_SIZE);		
// ERROR �߻� �� �ݹ��ϴ� �Լ��� TYPE - (�μ� : ��Ƽĳ��Ʈ��ü ������, ������ȣ, �����޽���)
typedef VOID (* LPCALLBACKPROC_ERRORNOTIFY)	(CWzMultiCastSock * lpTHIS, INT iERRNUM, LPSTR szERRMSG);				


typedef struct {
	BOOL	m_bInUsed;
	CHAR	m_szIPADDR[MAX_IPADDR_LEN];
} IPINFO_NODE, * LPIPINFO_NODE;


class CWzMultiCastSock {
private :
	SOCKET			m_skSocket;
	CHAR			m_szIPADDR_JOIN[MAX_IPADDR_LEN];						// ���� ��Ƽĳ��Ʈ �׷쿡 ������ IP

	IPINFO_NODE		m_stIPINFO_LIST[MAX_IPINFO];
	INT				m_iIPINFO_COUNT;

	UINT			m_uLastErrorNum;
	CHAR			m_szLastErrorMessage[MAX_PATH];

	HANDLE			m_hWorkerThread;										// recv() �۾������� �ڵ�
	unsigned int	m_idWorkerThread;										// recv() �۾������� ID

	LPCALLBACKPROC_RECVNOTIFY	m_lpfnCALLBACK_RECVNOTIFY;					// RECV �ݹ��Լ�
	LPCALLBACKPROC_ERRORNOTIFY	m_lpfnCALLBACK_ERRORNOTIFY;					// ERROR �ݹ��Լ�
	
public :
	CWzMultiCastSock();
	~CWzMultiCastSock();

	INT		DoEnumIpAddress		();											// ����IP�ּҵ��� ���� (��Ƽĳ���ÿ� ���)
	BOOL	GetEnummedIpAddress	(INT iIPINDEX, LPSTR lpszOutputIpAddress);	// DoEnumIpAddress() �� ������ �� Ư�� �ε����� �ּҰ��� �����´�.

	BOOL 	StartMultiCast		(LPSTR lpszIPADDR=NULL);					// ��Ƽĳ������ ������ (lpszIPADDR�� NULL�̸� �ּҸ� �����Ͽ� ù��° �ּҸ� �ڵ����� ���)
	BOOL 	ReStartMultiCast	();											// ��Ƽĳ���� ���� �� �Լ��� ȣ���Ͽ� ������ �������� �׷� �簡���� �õ�
	VOID	EndMultiCast		();											// ��Ƽĳ������ ������

	BOOL	SendData			(LPSTR lpBUFFER, INT iBUFSIZE);				// ��Ƽĳ��Ʈ �׷쿡 �����͸� ����

	VOID	SetErrorMsgFlag		(BOOL bErrorMsg);							// ���� �޽��� �ڽ��� DLL���� ������ ������ ����
	UINT	GetLastErrorNum		() { return m_uLastErrorNum; }				// ������ ���� ��ȣ�� ��ȯ
	CHAR *	GetLastErrorMsg		() { return m_szLastErrorMessage; }			// ������ ���� �޽����� ��ȯ

	VOID	SetRecvNotifyFunc	(											// RECV ������ ���� �ݹ��Լ� ����		
			LPCALLBACKPROC_RECVNOTIFY lpfnCALLBACK_RECVNOTIFY
		) {	m_lpfnCALLBACK_RECVNOTIFY = lpfnCALLBACK_RECVNOTIFY;	}
	VOID	SetErrorNotifyFunc	(											// ERROR ������ ���� �ݹ��Լ� ����		
			LPCALLBACKPROC_ERRORNOTIFY lpfnCALLBACK_ERRORNOTIFY
		) {	m_lpfnCALLBACK_ERRORNOTIFY = lpfnCALLBACK_ERRORNOTIFY;	}

	static unsigned int __stdcall RecvThreadProc (LPVOID lpParam=NULL);		// �θ� �����찡 �������� ���� �� - recv() �۾�������

protected :
	VOID	SaveLastError (INT iErrorNo = 0);								// ������ ������ȣ�� �޽����� ����
	VOID	SavePrivateErrorMessage (LPSTR lpszErrorMessage);				// WSAGetLaseError() �� ���� �� ���� �����޽����� �������� ����
};


#endif


