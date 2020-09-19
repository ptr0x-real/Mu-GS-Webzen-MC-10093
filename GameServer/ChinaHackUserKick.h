// ChinaHackUserKick.h: interface for the CChinaHackUserKick class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHINAHACKUSERKICK_H__BF9B25A7_D6D3_431B_8636_6943B4E0BBA5__INCLUDED_)
#define AFX_CHINAHACKUSERKICK_H__BF9B25A7_D6D3_431B_8636_6943B4E0BBA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winreg.h>
#include "LogToFile.H"

#ifdef CHINA_HACKUSER_KICK_SYSTEM
typedef struct 
{
	int		LogCountN;			// 적용 기준 N Count
	int		AccuLogCountN;		// 적용 기준 누적 N Count(LogCountN의 값을 넘은 수)
}CHI_HACK_SYSTEM_CATAINFO, *LPCHI_HACK_SYSTEM_CATAINFO;

class CChinaHackUserKick  
{
	int		m_iSpeedN;
	int		m_iUserSpeedN[MAX_USER];

	int		m_iKickN;	
	int		m_iUserKickN[MAX_USER];	

	int		m_iUserCataLogCount[MAX_USER];
	int		m_iUserCataAccLogCount[MAX_USER];

	DWORD	m_iUserTickCount[MAX_USER];
	
	char	m_strCata[100][100];

	//int							m_iCataCount[MAX_CLASSTYPE];
	CHI_HACK_SYSTEM_CATAINFO	m_Cata[MAX_CLASSTYPE][MAX_LEVEL+1];

	HKEY 	m_hKey;

	struct REGINFO
	{
		LONG lMessage;
		DWORD dwType;
		DWORD dwSize;
	} m_Info;	
public:
	CChinaHackUserKick(HKEY hKeyRoot = HKEY_LOCAL_MACHINE);
	virtual ~CChinaHackUserKick();

	void Init();	

	void	InitUserInfo(int aIndex);	
	void	ResetNCount(int aIndex);
	void	InsertSpeedNCount(int aIndex);
	void	InsertKickNCount(int aIndex);
	void	InsertCataNCount(int aIndex);
private:
	// Cata Info 읽기
	void	AddCataInfo(char* strCata);
	
	// 레지스트리 읽기
	BOOL Open(LPCTSTR pszPath);
	void Close();
	BOOL Write(LPCTSTR pszKey, int iVal);
	BOOL Write(LPCTSTR pszKey, DWORD dwVal);
	BOOL Write (LPCTSTR pszKey, LPCTSTR pszData);
	BOOL Read(LPCTSTR pszKey, int& iVal);
	BOOL Read(LPCTSTR pszKey, LPSTR lpszBuffer, int nBufferSize);
	BOOL Read(LPCTSTR pszKey, DWORD& dwVal);
	BOOL DeleteValue(LPCTSTR pszValue);
	BOOL DeleteValueKey(LPCTSTR pszPath);
};

class CChiLogToFile
{
public:
	void Output( char *fmt, ... );

	CChiLogToFile( char LogFileName [MAX_PATH] = DEF_LOG_FILE_NAME, char LogDirectoryName [MAX_PATH] = DEF_LOG_DIRECTORY_NAME, BOOL bWithDate = TRUE );
	~CChiLogToFile();

private:
	FILE * m_fLogFile;
	char m_szLogFileName [MAX_PATH];
	char m_szLogDirectoryName [MAX_PATH];
	char m_cBUFFER [LINE_BUF];

	BOOL m_bWithDate;
	CRITICAL_SECTION m_critLogToFile;
};


extern CChiLogToFile	ChinaHackLogFile;
extern CChinaHackUserKick ChinaHackUserKick;
#endif

#endif // !defined(AFX_CHINAHACKUSERKICK_H__BF9B25A7_D6D3_431B_8636_6943B4E0BBA5__INCLUDED_)
