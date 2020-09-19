// LogProc.cpp: implementation of the LogProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <TIME.H>
#include "LogProc.h"
#include "GameMain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static char m_szLogFileName[256];
static FILE *	logfp=NULL;

#ifdef MONSTER_ITEMDROP_LOG_20040309
static char m_szLogFileName_MonsterItemDrop[256];
static FILE *	logfp_MonsterItemDrop=NULL;
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
static char m_szTempLog[256];
static FILE *	logfpTempLog=NULL;
#endif

#define MAXLOGTEXT		30
#define MAXLOGTEXTC		80
char  LogText[MAXLOGTEXT][MAXLOGTEXTC+1];
short LogTextLength[MAXLOGTEXT];
BYTE  LogTextViewType[MAXLOGTEXT];

int   gLogOutType=1;	// 출력형식은 0이면 화면에 출력, 1이면 파일에 출력

void LogAddFuncVoid(char *szlog, ...);
void LogAddHeadHexFuncVoid(char *, char *data, int len);
void LogTextPaintProcVoid(HWND hWnd);
void LogAddLocalFuncVoid(char *szlog, ...);
#ifdef MONSTER_ITEMDROP_LOG_20040309
void LogAddFunc_MonsterItemDrop(char *szlog, ...);
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
void LogAddFuncTD_LogTemp(char *szlog, ...);
#endif

BOOL LogDateChange();
void LogDataSet();

void LogAddTimeDateFunc(char *szlog, ...);
void LogAddTimeDateVoid(char *szlog, ...);

void LogAddFunc(char *szlog, ...);
void LogAddFuncColor(BYTE Color, char *szlog, ...);
void LogAddHeadHexFunc(int , char *data, int len);
void LogAddLocalFunc(char *szlog, ...);

void (*LogAdd)(char *szlog, ...);
void (*LogAddC)(BYTE Color, char *szlog, ...);
void (*LogAddTD)(char *szlog, ...);
void (*LogAddHeadHex)(char *,char *data, int);
void (*LogAddL)(char *szlog, ...);
#ifdef MONSTER_ITEMDROP_LOG_20040309
void (*LogAdd_MonsterItemDrop)(char *szlog, ...);
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
void (*LogAddTD_TempLog)(char *szlog, ...);
#endif

void LogAddStrHexFunc(char *str, char *data, int len);

void (*LogTextPaint)(HWND hWnd);
void LogTextPaintProc(HWND hWnd);

CRITICAL_SECTION	LogCritical;
#ifdef MONSTER_ITEMDROP_LOG_20040309
CRITICAL_SECTION	LogCritical_MonsterItemDrop;
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
CRITICAL_SECTION	LogCritical_TempLog;
#endif

void TRACE_LOG(char *szFormat, ...);

void LogInit(int logprint)
{
	int n;

	InitializeCriticalSection(&LogCritical);
#ifdef MONSTER_ITEMDROP_LOG_20040309
	InitializeCriticalSection(&LogCritical_MonsterItemDrop);
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	InitializeCriticalSection(&LogCritical_TempLog);
#endif

	if( logprint )		// 로그를 출력할 것인가 아닌가 ?
	{

		LogAdd			= LogAddFunc;
		LogAddC			= LogAddFuncColor;
		LogAddTD		= LogAddTimeDateFunc;
		LogAddHeadHex	= LogAddStrHexFunc;
		LogTextPaint	= LogTextPaintProc;
		LogAddL			= LogAddLocalFunc;

		for( n=0; n<MAXLOGTEXT; n++)
		{
			memset(LogText[n], 0, MAXLOGTEXTC);
			LogTextLength[n] = 0;
			LogTextViewType[n] = 0;
		}
		if( gLogOutType == 0 ) return; // 파일까지 출력할땐	


		LogDataSet();
	}
	else
	{
		LogAdd			= LogAddFuncVoid;
		LogAddHeadHex	= LogAddHeadHexFuncVoid;
		LogTextPaint	= LogTextPaintProcVoid;
		LogAddTD		= LogAddTimeDateVoid;
		LogAddL			= LogAddLocalFuncVoid;
		
	}

#ifdef MONSTER_ITEMDROP_LOG_20040309
	LogAdd_MonsterItemDrop	= LogAddFunc_MonsterItemDrop;
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	LogAddTD_TempLog = LogAddFuncTD_LogTemp;
#endif
	
}

int LogMYear, LogMonth, LogMDay;

void LogDataSet()
{
	char szTemp[250];
	struct tm *today;
	time_t ltime;
	
	time( &ltime );
	today = localtime( &ltime );
	
	today->tm_year += 1900;

	LogMYear = today->tm_year;
	LogMonth = today->tm_mon+1;
	LogMDay  = today->tm_mday;

	wsprintf(szTemp, "log\\%02d%02d%02d_%s.log",LogMYear, LogMonth,LogMDay, szServerName);
	
	if( logfp!=NULL ) {
		fclose(logfp);
		logfp = NULL;
	}
	
	logfp = fopen(szTemp, "a+t");
	if( logfp == NULL )
	{
		LogAdd("Log file create error");
	}
	strcpy(m_szLogFileName, szTemp);


#ifdef MONSTER_ITEMDROP_LOG_20040309
	wsprintf(szTemp, "log\\%02d%02d%02d_%s_MITD.log",LogMYear, LogMonth,LogMDay, szServerName);
	
	if( logfp_MonsterItemDrop!=NULL ) {
		fclose(logfp_MonsterItemDrop);
		logfp_MonsterItemDrop = NULL;
	}
	
	logfp_MonsterItemDrop = fopen(szTemp, "a+t");
	if( logfp_MonsterItemDrop == NULL )
	{
		LogAdd("Log file create error");
	}
	strcpy(m_szLogFileName_MonsterItemDrop, szTemp);
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	
#if TEMPLOG_PRINT == 0
	return;
#endif
	
	memset(szTemp,0,sizeof(szTemp));
	wsprintf(szTemp, "log\\%02d%02d%02d_%s_TempLog.log",LogMYear, LogMonth,LogMDay, szServerName);
	
	if( logfpTempLog != NULL ) {
		fclose(logfpTempLog);
		logfpTempLog = NULL;
	}
	
	logfpTempLog = fopen(szTemp, "a+t");
	if( logfpTempLog == NULL )
	{
		LogAdd("TempLog file create error");
	}
	strcpy(m_szTempLog, szTemp);
	
#endif

}

BOOL LogDateChange()
{
	char szTemp[250];
	struct tm *today;
	time_t ltime;
	
	time( &ltime );
	today = localtime( &ltime );
	
	today->tm_year += 1900;
	today->tm_mon  += 1;

	if( (today->tm_year <= LogMYear) && 
		(today->tm_mon  <= LogMonth) &&
		(today->tm_mday <= LogMDay) )
	{
		return FALSE;
	}
	
	LogMYear = today->tm_year;
	LogMonth = today->tm_mon;
	LogMDay  = today->tm_mday;

	wsprintf(szTemp, "log\\%02d%02d%02d_%s.log",LogMYear, LogMonth,LogMDay,szServerName);

	EnterCriticalSection(&LogCritical);
	if( logfp!=NULL ) {
		fclose(logfp);
		logfp = NULL;
	}
	
	logfp = fopen(szTemp, "a+t");
	if( logfp == NULL )
	{
		//MsgBox("로그 파일 생성 에러");
	}
	strcpy(m_szLogFileName, szTemp);

	LeaveCriticalSection(&LogCritical);

#ifdef MONSTER_ITEMDROP_LOG_20040309
	EnterCriticalSection(&LogCritical_MonsterItemDrop);

	wsprintf(szTemp, "log\\%02d%02d%02d_%s_MITD.log",LogMYear, LogMonth,LogMDay,szServerName);

	if( logfp_MonsterItemDrop!=NULL ) {
		fclose(logfp_MonsterItemDrop);
		logfp_MonsterItemDrop = NULL;
	}
	
	logfp_MonsterItemDrop = fopen(szTemp, "a+t");
	if( logfp_MonsterItemDrop == NULL )
	{
		//MsgBox("로그 파일 생성 에러");
	}
	strcpy(m_szLogFileName_MonsterItemDrop, szTemp);

	LeaveCriticalSection(&LogCritical_MonsterItemDrop);
#endif

	return TRUE;
}

int m_cline=0;

void LogTextAdd(BYTE type, char *msg, int len)
{
	if( len > MAXLOGTEXTC-1 ) len = MAXLOGTEXTC-1;

	m_cline++;
	if( m_cline > MAXLOGTEXT-1) m_cline = 0;

	LogText[m_cline][0]			= '\0';
	memcpy(LogText[m_cline], msg, len);
	LogText[m_cline][len]		= LogText[m_cline][len+1] = '\0';
	LogTextLength[m_cline  ]	= len;
	LogTextViewType[m_cline]	= type;

}

void LogAddFuncVoid(char *szlog, ...)
{
	return;
}

void LogAddTimeDateVoid(char *szlog, ...)
{
	return;
}


void LogAddFunc(char *szlog, ...)
{	
	char szBuffer[512]="";
	va_list		pArguments;

	//if( strlen(m_szLogFileName) < 1 ) return;

	va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);
	LogTextAdd(0, szBuffer, strlen(szBuffer));

	if( gLogOutType == 0 ) return;

#ifdef _DEBUG
	if( logfp == NULL ) {
		//MessageBox(NULL,"파일을 생성할수 없습니다.", "error", MB_OK);
		return;	
	}
#endif
	EnterCriticalSection(&LogCritical);
	fprintf(logfp, "%s\n",szBuffer);
	LeaveCriticalSection(&LogCritical);
}

void LogAddFuncColor(BYTE Color, char *szlog, ...)
{	
	char szBuffer[1024]="";
	va_list		pArguments;

	va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);
	LogTextAdd(Color, szBuffer, strlen(szBuffer));

	if( gLogOutType == 0 ) return;

#ifdef _DEBUG
	if( logfp == NULL ) {
		//MessageBox(NULL,"파일을 생성할수 없습니다.", "error", MB_OK);
		return;	
	}
#endif
	EnterCriticalSection(&LogCritical);
	fprintf(logfp, "%s\n",szBuffer);
	LeaveCriticalSection(&LogCritical);
}

void LogAddLocalFunc(char *szlog, ...)
{	
#if (LOCALCONNECT==0)
	return;
#endif

	char szBuffer[512]="";
	va_list		pArguments;

	//if( strlen(m_szLogFileName) < 1 ) return;

	va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);
	LogTextAdd(0, szBuffer, strlen(szBuffer));

	if( gLogOutType == 0 ) return;
	EnterCriticalSection(&LogCritical);
	fprintf(logfp, "%s\n",szBuffer);
	LeaveCriticalSection(&LogCritical);
}

void LogAddLocalFuncVoid(char *szlog, ...)
{
	return;
}


void LogAddTimeDateFunc(char *szlog, ...)
{	
	char szBuffer[1024]="";
	va_list		pArguments;
	struct	tm *today;
	time_t		ltime;
	char		tmpbuf[512];
	
	time( &ltime );
	today = localtime( &ltime );
  
	wsprintf(tmpbuf, "%.8s ",asctime( today )+11);
	
	va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);
 
	strcat(tmpbuf, szBuffer);

	LogTextAdd(0, tmpbuf, strlen(tmpbuf));

	if( gLogOutType == 0 ) return;
	if( logfp == NULL ) 
	{
		LogAdd("error-L2 : file create error %s %d",__FILE__, __LINE__);
		return;	
	}
	
	EnterCriticalSection(&LogCritical);
	fprintf(logfp, "%s\n",tmpbuf);
	LeaveCriticalSection(&LogCritical);
}

void LogAddHeadHexFuncVoid(char *, char *data, int len)
{
	return;
}

void LogAddHeadHexFunc(int type, char *data, int len)
{
	//LogTextAdd(type, data, len);

	if( gLogOutType == 0 ) return;
	
	if( logfp == NULL ) {
		//(NULL,"파일을 생성할수 없습니다.", "error", MB_OK);
		return;	
	}

	EnterCriticalSection(&LogCritical);
	switch(type)
	{
	case 2  : fprintf(logfp, "Send:[%d] : (", len);	break;
	case 3  : fprintf(logfp, "FDWrite:[%d] : (", len);	break;
	case 4  : fprintf(logfp, "Recv:[%d] : (", len);	break;
	default : fprintf(logfp, "%d[%d] : (", type, len);	break;
	}
	
	for( int n=0; n<len; n++)
	{
		fprintf(logfp, "0x%02x ",(BYTE)data[n]);
	}
	fprintf(logfp, ")\n");
	LeaveCriticalSection(&LogCritical);
}

void LogAddStrHexFunc(char *str, char *data, int len)
{
	//LogTextAdd(type, data, len);
	struct	tm *today;
	time_t		ltime;
	//char		tmpbuf[512];
	
	time( &ltime );
	today = localtime( &ltime );
  

	if( gLogOutType == 0 ) return;
	
	if( logfp == NULL ) {
		//(NULL,"파일을 생성할수 없습니다.", "error", MB_OK);
		return;	
	}

	EnterCriticalSection(&LogCritical);
	fprintf(logfp, "%.8s",asctime( today )+11);
	fprintf(logfp, "[%s][%d] : (", str, len);
	
	for( int n=0; n<len; n++)
	{
		fprintf(logfp, "0x%02x ",(BYTE)data[n]);
	}
	fprintf(logfp, ")\n");
	LeaveCriticalSection(&LogCritical);
}


void LogTextClear()
{
	int n;
	for( n=0; n<MAXLOGTEXT; n++)
	{
		memset(LogText[n], 0, MAXLOGTEXTC);
		LogTextLength[n] = 0;
		LogTextViewType[n] = 0;
	}	
}

void LogTextPaintProcVoid(HWND hWnd)
{
	return;
}


void LogTextPaintProc(HWND hWnd)
{
	HDC	hdc;
	hdc = GetDC(hWnd);
	int total = MAXLOGTEXT;
	int n     = m_cline;
	while( total-- )
	{
		switch( LogTextViewType[n] )
		{
		case 2  : SetTextColor(hdc, RGB(255,0,0)); break;
		case 3  : SetTextColor(hdc, RGB(0,100,0)); break;
		case 4  : SetTextColor(hdc, RGB(0,0,255)); break;
		case 5  : SetTextColor(hdc, RGB(155,0,0)); break;
		case 6  : SetTextColor(hdc, RGB(0,0,100)); break;
		case 7  : SetTextColor(hdc, RGB(210,30,150)); break;
		default : SetTextColor(hdc, RGB(0,0,0)); break;
		}
		
		if( strlen(LogText[n]) > 1 )

#ifdef ADD_SERVER_INFO_DISPLAY_EXTEND_01_20060313
			TextOut(hdc, 0, 100+(total*15), LogText[n], strlen(LogText[n]));
#else
			TextOut(hdc, 0, 0+(total*15), LogText[n], strlen(LogText[n]));
#endif

		n--;
		if( n < 0 ) n = MAXLOGTEXT-1;
	}
	
	ReleaseDC(hWnd, hdc);
}

char * LogGetFileName()
{
#ifdef _DEBUG
	return m_szLogFileName;
#endif
	return NULL;
}


void LogClose()
{
	if( logfp != NULL )
		fclose(logfp);

	DeleteCriticalSection(&LogCritical);
#ifdef MONSTER_ITEMDROP_LOG_20040309
	DeleteCriticalSection(&LogCritical_MonsterItemDrop);
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	if(logfpTempLog != NULL)
		fclose(logfpTempLog);
	
	DeleteCriticalSection(&LogCritical_TempLog);
#endif
	
}

void MsgBox(char *szlog, ...)
{	
	char szBuffer[512]="";
	va_list		pArguments;

	va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);
	
	MessageBox(NULL, szBuffer, "error", MB_OK);
}


#ifdef MONSTER_ITEMDROP_LOG_20040309

void LogAddFunc_MonsterItemDrop(char *szlog, ...)
{
	char szBuffer[512]="";
	va_list		pArguments;
	
	va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);

	EnterCriticalSection(&LogCritical_MonsterItemDrop);
	fprintf(logfp_MonsterItemDrop, "%s\n",szBuffer);
	LeaveCriticalSection(&LogCritical_MonsterItemDrop);
}

#endif

#if TESTSERVER == 1
void TRACE_LOG(char *szFormat, ...)
{
	char szBuffer[1024];
    char *pArguments;
	
	pArguments=(char*)&szFormat+sizeof(szFormat);
	vsprintf(szBuffer, szFormat, pArguments);
	
	OutputDebugString( szBuffer );
}
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409

void LogAddFuncTD_LogTemp(char *szlog, ...)
{
	
#if TEMPLOG_PRINT == 0
	return;
#endif
	
	char szBuffer[1024]="";
	va_list		pArguments;
	struct	tm *today;
	time_t		ltime;
	char		tmpbuf[512];
	
	time( &ltime );
	today = localtime( &ltime );
	
	wsprintf(tmpbuf, "%.8s ",asctime( today )+11);
	
	va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);
	
	strcat(tmpbuf, szBuffer);
	
	//LogTextAdd(0, tmpbuf, strlen(tmpbuf));
	
	if( gLogOutType == 0 ) return;
	if( logfpTempLog == NULL ) 
	{
		LogAdd("error-L2 : file create error %s %d",__FILE__, __LINE__);
		return;	
	}
	
	EnterCriticalSection(&LogCritical_TempLog);
	fprintf(logfpTempLog, "%s\n",tmpbuf);
	LeaveCriticalSection(&LogCritical_TempLog);
}

#endif
