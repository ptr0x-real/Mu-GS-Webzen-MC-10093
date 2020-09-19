#ifndef _GG_AUTH25_SERVER_H_
#define _GG_AUTH25_SERVER_H_

#ifndef WIN32
	typedef unsigned short	WORD;
	typedef unsigned long	DWORD;
	typedef void*			LPVOID;
	typedef int            BOOL;
#endif
								
#ifdef _EXPORT_DLL
	#define GGAUTHS_API    extern "C" __declspec(dllexport)
	#define GGAUTHS_EXPORT __declspec(dllexport)
	#define __CDECL        __cdecl
#else
  	#define GGAUTHS_API extern "C"
	//#define GGAUTHS_API  <== ggsrv_cpp_c.h ���� Ȱ��ȭ. extern "C" �� c �����Ϸ��� �νĸ���.
	#define GGAUTHS_EXPORT 
	#define __CDECL 
#endif

#define	NPGG_INFO_SUCCESS					0	//Get info success.
#define	NPGG_INFO_ERROR_NOTENOUGHFMEMORY	1 //Need more memory

#define NPGG_CHECKUPDATED_VERIFIED			0 //csa.CheckUpdated() �Լ� ���ϰ�. ���� ���� �����
#define NPGG_CHECKUPDATED_NOTREADY			1 //csa.CheckUpdated() �Լ� ���ϰ�. �����˻縦 �ϱ� ��. ������ ���� ������ ����.
#define NPGG_CHECKUPDATED_HIGH				2 //csa.CheckUpdated() �Լ� ���ϰ�. ���� �������� ���� �����
#define NPGG_CHECKUPDATED_LOW				3 //csa.CheckUpdated() �Լ� ���ϰ�. ���� �������� ���� �����

#define	NPLOG_DEBUG	0x00000001	// ����� �޼���
#define	NPLOG_ERROR	0x00000002  // ���� �޼���

#define NPLOG_ENABLE_DEBUG 0x00000001 // ����� �޼��� ������
#define NPLOG_ENABLE_ERROR 0x00000002 // ���� �޼��� ������

extern void (*LogAdd)(char *szlog, ...);
extern void (*LogAddC)(BYTE, char *szlog, ...);
//extern void (*LogAddHeadHex)(int type,char *data, int);
extern void (*LogTextPaint)(HWND hWnd);
extern void (*LogAddTD)(char *szlog, ...);

// gameguard auth data
typedef struct _GG_AUTH_DATA25
{
	DWORD dwIndex;
	DWORD dwValue1;
	DWORD dwValue2;
	DWORD dwValue3;
} GG_AUTH_DATA25, *PGG_AUTH_DATA25;

// gameguard version data
typedef struct _GG_VERSION25
{
	DWORD	dwGGVer;
	WORD	wYear;
	WORD	wMonth;
	WORD	wDay;
	WORD	wNum;
} GG_VERSION25, *PGG_VERSION25;

typedef struct _GG_AUTH_PROTOCOL25 *PGG_AUTH_PROTOCOL25;

// ggauth.dll�� Path
GGAUTHS_API DWORD __CDECL InitGameguardAuth25(char* sGGPath, DWORD dwNumActive, BOOL useTimer, int useLog);
GGAUTHS_API void  __CDECL CleanupGameguardAuth25();
GGAUTHS_API DWORD __CDECL GGAuthUpdateTimer25(); //useTimer �� true �� ��� ȣ��.

// protocol dll name
GGAUTHS_API DWORD __CDECL AddAuthProtocol25(char* sDllName);

//Need for modify by gameserver engineer !!!!!!!!!!!!!!!!
GGAUTHS_API void NpLog25(int mode, char* msg)
{
	if(mode & (NPLOG_DEBUG | NPLOG_ERROR)) //Select log mode. 
		LogAddTD("[GameGuard][Debug|Error Log] %s", msg );
};

typedef struct _GG_UPREPORT25
{
	DWORD	dwBefore;   // Before version
	DWORD	dwNext;		// Now version
	int		nType;		// Update type 1 : gameguard version, 2 : protocol num
} GG_UPREPORT25, *PGG_UPREPORT25;

//Need for modify by gameserver engineer !!!!!!!!!!!!!!!!
GGAUTHS_API void GGAuthUpdateCallback25(PGG_UPREPORT25 report)
{
	LogAddTD("GGAuth version update [%s] : [%ld] -> [%ld] \n", 
			report->nType==1?"GameGuard Ver":"Protocol Num", 
			report->dwBefore,
			report->dwNext); 
};

// protocol information
GGAUTHS_API int ModuleInfo25(char* dest, int length);

// Class CSAuth2
class GGAUTHS_EXPORT CCSAuth25
{
public:
	// Constructor
	CCSAuth25();

	// Destructor
	~CCSAuth25();

protected:
	PGG_AUTH_PROTOCOL25 m_pProtocol;
	DWORD m_bPrtcRef;
	DWORD m_dwUserFlag;
	GG_VERSION25 m_GGVer;	//���Ӱ��� ����
	GG_AUTH_DATA25 m_AuthQueryTmp;	//m_AuthQuery ���� ����صд�.

	BOOL m_bNewProtocol; // ���ο� �������ݷ� �����ϸ� true. ���ο� ���������� �Ҽ�ȭ�Ǹ� false, ����Ʈ false;

	BOOL m_bActive; //���� ��ü�� ����ڿ��� �Ҵ�Ǿ�����. Init ȣ���� true, Close ȣ���� false

public:	
	GG_AUTH_DATA25 m_AuthQuery;
	GG_AUTH_DATA25 m_AuthAnswer;

	void  Init();
	DWORD GetAuthQuery();
	DWORD CheckAuthAnswer();	
	void  Close();
	int	  Info(char* dest, int length); // protocol information
	int	  CheckUpdated(); //���� ������� ������Ʈ ���θ� Ȯ���Ѵ�.
};

// C type CSAuth2
typedef LPVOID          LPGGAUTH25;

GGAUTHS_API LPGGAUTH25 __CDECL GGAuthCreateUser25();                                              // CCSAuth2()
GGAUTHS_API DWORD     __CDECL GGAuthDeleteUser25(LPGGAUTH25 pGGAuth);                             // ~CCSAuth2()
GGAUTHS_API DWORD	 __CDECL GGAuthInitUser25(LPGGAUTH25 pGGAuth);                              // Init()
GGAUTHS_API DWORD     __CDECL GGAuthCloseUser25(LPGGAUTH25 pGGAuth);                             // Close()
GGAUTHS_API DWORD    __CDECL GGAuthGetQuery25(LPGGAUTH25 pGGAuth, PGG_AUTH_DATA25 pAuthData);     // GetAuthQuery()
GGAUTHS_API DWORD    __CDECL GGAuthCheckAnswer25(LPGGAUTH25 pGGAuth, PGG_AUTH_DATA25 pAuthData);  // CheckAuthAnswer()
GGAUTHS_API int      __CDECL GGAuthCheckUpdated25(LPGGAUTH25 pGGAuth);  // CheckAuthAnswer()
GGAUTHS_API int      __CDECL GGAuthUserInfo25(LPGGAUTH25 pGGAuth, char* dest, int length);  // CheckAuthAnswer()
#endif

