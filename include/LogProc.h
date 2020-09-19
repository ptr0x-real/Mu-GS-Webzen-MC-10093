
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LOGPROC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LOGPROC_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LOGPROC_EXPORTS
#define LOGPROC_API __declspec(dllexport)
#else
#define LOGPROC_API __declspec(dllimport)
#endif

#define MAXLOGTEXT		30		// �ִ� �α� �ؽ�Ʈ ����
#define MAXLOGTEXTC		80		// �ִ� �α� �ؽ�Ʈ �÷�


#define LOGC_RED			2
#define LOGC_GREEN			3
#define LOGC_BLUE			4
#define LOGC_LIGHTRED		5



// This class is exported from the LogProc.dll
class LOGPROC_API CLogProc 
{
	CRITICAL_SECTION	m_cs;

	int   m_OutType;
	int	  m_DateChange;	// ���ڹٲ� ��� 

	
	//int   gLogOutType;	// ��������� 0�̸� ȭ�鿡 ���, 1�̸� ���Ͽ� ���
	//int	  gLogDateChange;	// ���ڹٲ� ��� 

	int   m_Year, m_Month, m_Day;
	int   m_cline;

	char  m_szLogFileName[256];
	FILE  *	logfp;

	char  LogText[MAXLOGTEXT][MAXLOGTEXTC];
	short LogTextLength[MAXLOGTEXT];
	BYTE  LogTextViewType[MAXLOGTEXT];

public:
	CLogProc(void);
	~CLogProc();

	void Create(int log_print);
	void Clear();
	void TextAdd(BYTE type, char *msg, int len);
	void Add(char *szlog, ...);
	void AddText(char *szlog);
	void AddC(BYTE Color, char *szlog, ...);
	void AddTD(char *szlog, ...);
	void Print(HWND hWnd, int x, int y );
	void Printf();
	void LogClose();
	char * LogGetFileName();
	void DateSet();
	BOOL DateChange();
	void Run();
};

LOGPROC_API void MsgBox(char *szlog, ...);

extern LOGPROC_API int nLogProc;

LOGPROC_API int fnLogProc(void);
