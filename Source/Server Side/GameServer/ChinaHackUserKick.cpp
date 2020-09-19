// ChinaHackUserKick.cpp: implementation of the CChinaHackUserKick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Common\\Winutil.h"
#include "ChinaHackUserKick.h"
#include "user.h"
#include "DSProtocol.h"
#include "Gamemain.h"

#ifdef CHINA_HACKUSER_KICK_SYSTEM
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChiLogToFile	ChinaHackLogFile("HackLogSystem", ".\\HackLogSystem");
CChinaHackUserKick ChinaHackUserKick;

CChiLogToFile::CChiLogToFile( char LogFileName [MAX_PATH], char LogDirectoryName [MAX_PATH], BOOL bWithDate )
{
	if ( ! strcmp (LogFileName, "") )
		strcpy (m_szLogFileName, DEF_LOG_FILE_NAME);
	else
		strcpy (m_szLogFileName, LogFileName);

	if ( ! strcmp (LogDirectoryName, "") )
		strcpy (m_szLogDirectoryName, DEF_LOG_DIRECTORY_NAME);
	else
		strcpy (m_szLogDirectoryName, LogDirectoryName);

	m_bWithDate = bWithDate;

	CreateDirectory (LogDirectoryName, NULL());

	InitializeCriticalSection (& m_critLogToFile);
}


CChiLogToFile::~CChiLogToFile()
{
	DeleteCriticalSection (& m_critLogToFile);
}


void CChiLogToFile::Output( char *fmt, ... )
{
	va_list argptr;
	int iChrWritten;

	EnterCriticalSection (& m_critLogToFile);
	va_start( argptr, fmt );
	iChrWritten = vsprintf( m_cBUFFER, fmt, argptr );
	va_end( argptr );

	char szLogFileName [MAX_PATH];
	SYSTEMTIME strSystime;

	GetLocalTime (& strSystime);
	wsprintf (szLogFileName, "%s\\%s %04d-%02d-%02d.txt",
		m_szLogDirectoryName,
		m_szLogFileName,
		strSystime.wYear,
		strSystime.wMonth,
		strSystime.wDay
		);

	if ( ! ( m_fLogFile = fopen (szLogFileName, "a+") ) ) {
		LeaveCriticalSection (& m_critLogToFile);
		return;
	}

	if ( ! m_bWithDate ) {
		fprintf (m_fLogFile, "%s\n", m_cBUFFER);
	}
	else {
		fprintf (m_fLogFile, "%d:%d:%d  %s\n", 
			strSystime.wHour,
			strSystime.wMinute,
			strSystime.wSecond,
			m_cBUFFER
			);
	}

	fclose (m_fLogFile);
	LeaveCriticalSection (& m_critLogToFile);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CChinaHackUserKick::CChinaHackUserKick(HKEY hKeyRoot)
{
	m_hKey = hKeyRoot;		
}

CChinaHackUserKick::~CChinaHackUserKick()
{
	Close();
}

void CChinaHackUserKick::Init()
{
	ZeroMemory(m_iUserSpeedN, sizeof(m_iUserSpeedN));
	ZeroMemory(m_iUserKickN, sizeof(m_iUserKickN));

	ZeroMemory(m_iUserCataLogCount, sizeof(m_iUserCataLogCount));
	ZeroMemory(m_iUserCataAccLogCount, sizeof(m_iUserCataAccLogCount));

	m_iSpeedN = 15;
	m_iKickN = 4;
	//m_iCataCount = 0;

	ChinaHackLogFile.Output("===========================================");

	ChinaHackLogFile.Output("Load HackLog Registry Infomation");
	int	CataCount;

	//if( Open("SoftWare\\Webzen\\HackLogSystem") )
	{
		m_iSpeedN = GetPrivateProfileInt("GameServerInfo", "ATTACK_SPEED_N", 0, gDirPath.GetNewPath("commonserver.cfg"));			
		//Read("SPEED_N", m_iSpeedN);
		ChinaHackLogFile.Output("[HLS] ATTACK_SPEED_N = %d", m_iSpeedN);

		m_iKickN = GetPrivateProfileInt("GameServerInfo", "KICK_N", 0, gDirPath.GetNewPath("commonserver.cfg"));			
		//Read("KICK_N", m_iKickN);
		ChinaHackLogFile.Output("[HLS] KICK_N = %d", m_iKickN);

		CataCount = GetPrivateProfileInt("GameServerInfo", "ATTACKAREA_CATA_COUNT", 0, gDirPath.GetNewPath("commonserver.cfg"));			
		//Read("CATA_COUNT", CataCount);
		ChinaHackLogFile.Output("[HLS] CATA_COUNT = %d", CataCount);

		ChinaHackLogFile.Output("[HLS] Parsing Class RefTable");
		char	sKey[255], strValue[255];		
		for( int i = 0; i < CataCount; i++ )
		{
			wsprintf(sKey, "ATTACKAREA_CATA%d", i+1);

			GetPrivateProfileString("GameServerInfo", sKey, "0,0,0,0,0", strValue, 255, gDirPath.GetNewPath("commonserver.cfg"));			
			//Read(sKey, strValue, 255);
			//ChinaHackLogFile.Output("%s = %s", sKey, strValue);
			AddCataInfo(strValue);
		}
	}	
	ChinaHackLogFile.Output("===========================================");
}

void CChinaHackUserKick::InitUserInfo(int aIndex)
{
	aIndex = aIndex - ALLOC_USEROBJECTSTART;
	if( !CHECK_LIMIT(aIndex, MAX_USER) ) return;

	m_iUserSpeedN[aIndex] = 0;

	m_iUserKickN[aIndex] = 0;	

	m_iUserCataLogCount[aIndex] = 0;
	m_iUserCataAccLogCount[aIndex] = 0;
}

void CChinaHackUserKick::ResetNCount(int aIndex)
{
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];	
	aIndex = aIndex - ALLOC_USEROBJECTSTART;
	if( !CHECK_LIMIT(aIndex, MAX_USER) ) return;

	if( (GetTickCount()-m_iUserTickCount[aIndex]) > (1000*60) )
	{
		ChinaHackLogFile.Output("[HLS] [%s][%s] %s ResetNCount SpeedN : [%d] AttackArea : [%d]", 
								lpObj->AccountID, lpObj->Name,
								lMsg.Get(1900+lpObj->Class),
								m_iUserSpeedN[aIndex], m_iUserCataLogCount[aIndex]);
		InitUserInfo(lpObj->m_Index);
	}
}

void CChinaHackUserKick::InsertSpeedNCount(int aIndex)
{
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];	
	aIndex = aIndex - ALLOC_USEROBJECTSTART;
	if( !CHECK_LIMIT(aIndex, MAX_USER) ) return;

	m_iUserSpeedN[aIndex]++;

	if( m_iUserSpeedN[aIndex] > m_iSpeedN )
	{
		ChinaHackLogFile.Output("[HLS] [%s][%s] %s Detected attack speed Hack", lpObj->AccountID, lpObj->Name,
								lMsg.Get(1900+lpObj->Class));

		if( !lpObj->bWaitBlockState )
		{
			lpObj->bWaitBlockState = TRUE;
			GDRequestBlockHackUser(lpObj->m_Index, lpObj->AccountID, lpObj->Name);
		}
	}
}

//void CChinaHackUserKick::InsertKickNCount(int aIndex)
//{
//	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];	
//	aIndex = aIndex - ALLOC_USEROBJECTSTART;
//	if( !CHECK_LIMIT(aIndex, MAX_USER) ) return;
//
//	m_iUserKickN[aIndex]++;
//
//	if( m_iUserKickN[aIndex] > m_iKickN )
//	{
//		ChinaHackLogFile.Output("[%s][%s] Detected Hack User Kick", lpObj->AccountID, lpObj->Name);
//
//		if( !lpObj->bWaitBlockState )
//		{
//			lpObj->bWaitBlockState = TRUE;
//			GDRequestBlockHackUser(lpObj->m_Index, lpObj->AccountID, lpObj->Name);
//		}
//	}
//}

void CChinaHackUserKick::InsertCataNCount(int aIndex)
{
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];	

	aIndex = aIndex - ALLOC_USEROBJECTSTART;
	if( !CHECK_LIMIT(aIndex, MAX_USER) ) return;

	int Class = lpObj->Class;
	int	Level = lpObj->Level;

	m_iUserCataLogCount[aIndex]++;

	if( m_iUserCataLogCount[aIndex] > m_Cata[Class][Level].LogCountN )
	{		
		lpObj->m_NotAttackAreaCount = 0;
		m_iUserCataLogCount[aIndex] = 0;
		//m_iUserCataAccLogCount[aIndex]++;
		//if( m_iUserCataAccLogCount[aIndex] > m_Cata[Class][Level].LogCountN )
		{
			ChinaHackLogFile.Output("[HLS] [%s][%s] %s Detected not Attackarea Count HackTool", lpObj->AccountID, lpObj->Name,lMsg.Get(1900+lpObj->Class));

			if( !lpObj->bWaitBlockState )
			{
				lpObj->bWaitBlockState = TRUE;
				GDRequestBlockHackUser(lpObj->m_Index, lpObj->AccountID, lpObj->Name);
			}
		}
	}
}

void CChinaHackUserKick::AddCataInfo(char* strCata)
{
	char seps[] = ", ";
	char *szCmdToken;

	int		Class, StartLevel, EndLevel, CountN, AccuCountN;

	szCmdToken = strtok(strCata, seps);
	if( !szCmdToken ){ChinaHackLogFile.Output("Cata Info Load Fail");return;}
	Class = atoi(szCmdToken);
	Class = Class/16;

	szCmdToken = strtok(NULL, seps);
	if( !szCmdToken ){ChinaHackLogFile.Output("[HLS] Cata Info Load Fail");return;}
	StartLevel = atoi(szCmdToken);

	szCmdToken = strtok(NULL, seps);
	if( !szCmdToken ){ChinaHackLogFile.Output("[HLS] Cata Info Load Fail");return;}
	EndLevel = atoi(szCmdToken);		

	szCmdToken = strtok(NULL, seps);
	if( !szCmdToken ){ChinaHackLogFile.Output("[HLS] Cata Info Load Fail");return;}
	CountN = atoi(szCmdToken);

	//szCmdToken = strtok(NULL, seps);
	//if( !szCmdToken ){ChinaHackLogFile.Output("[HLS] Cata Info Load Fail");return;}
	//AccuCountN = atoi(szCmdToken);

	for( int i = StartLevel; i <= EndLevel; i++ )
	{
		m_Cata[Class][i].LogCountN = CountN;
		m_Cata[Class][i].AccuLogCountN = AccuCountN;
		ChinaHackLogFile.Output("[HLS] Class = %d Level = %d CountN = %d AccCountN = %d", 
								Class,i,CountN,AccuCountN);
	}
	
}

BOOL CChinaHackUserKick::Open (LPCTSTR pszPath)
{
	//m_sPath = pszPath;

	LONG ReturnValue = RegOpenKeyEx (HKEY_LOCAL_MACHINE, pszPath, 0L,
		KEY_READ | KEY_QUERY_VALUE, &m_hKey);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = 0L;
	m_Info.dwType = 0L;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

void CChinaHackUserKick::Close()
{
	if (m_hKey)
	{
		RegCloseKey (m_hKey);
		m_hKey = NULL;
	}
}

BOOL CChinaHackUserKick::Write (LPCTSTR pszKey, int iVal)
{
	DWORD dwValue;
	
	dwValue = (DWORD)iVal;
	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwValue, sizeof(DWORD));

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = sizeof(DWORD);
	m_Info.dwType = REG_DWORD;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CChinaHackUserKick::Write (LPCTSTR pszKey, DWORD dwVal)
{
	return RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwVal, sizeof(DWORD));
}

BOOL CChinaHackUserKick::Write (LPCTSTR pszKey, LPCTSTR pszData)
{
	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_SZ,
		(CONST BYTE*) pszData, strlen(pszData) + 1);

	m_Info.lMessage = ReturnValue;
	m_Info.dwSize = strlen(pszData) + 1;
	m_Info.dwType = REG_SZ;

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}


BOOL CChinaHackUserKick::Read(LPCTSTR pszKey, int& iVal)
{
	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		iVal = (int)dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CChinaHackUserKick::Read (LPCTSTR pszKey, LPSTR lpszBuffer, int nBufferSize )
{
	DWORD dwType;
	DWORD dwSize = nBufferSize;
	char  szString[255];

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) szString, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;
	
	if(lReturn == ERROR_SUCCESS)
	{
		memcpy( lpszBuffer, szString, (int) dwSize );
		nBufferSize = (int) dwSize;
		return TRUE;
	}

	return FALSE;
}

BOOL CChinaHackUserKick::Read (LPCTSTR pszKey, DWORD& dwVal)
{
	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL, 
		&dwType, (BYTE *) &dwDest, &dwSize);

	m_Info.lMessage = lReturn;
	m_Info.dwType = dwType;
	m_Info.dwSize = dwSize;

	if(lReturn == ERROR_SUCCESS)
	{
		dwVal = dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CChinaHackUserKick::DeleteValue (LPCTSTR pszValue)
{	
	LONG lReturn = RegDeleteValue(m_hKey, pszValue);


	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CChinaHackUserKick::DeleteValueKey (LPCTSTR pszPath)
{
	LONG lReturn = RegDeleteKey(HKEY_LOCAL_MACHINE, pszPath);

	m_Info.lMessage = lReturn;
	m_Info.dwType = 0L;
	m_Info.dwSize = 0L;

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}
#endif	// #ifdef CHINA_HACKUSER_KICK_SYSTEM