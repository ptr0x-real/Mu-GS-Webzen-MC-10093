// LogProc.h: interface for the LogProc class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __LOGPROC_H__
#define __LOGPROC_H__
#include <windows.h>


#define LOGC_RED			2
#define LOGC_GREEN			3
#define LOGC_BLUE			4
#define LOGC_LIGHTRED		5
#define LOGC_PURPLE			7


extern void (*LogAdd)(char *szlog, ...);
extern void (*LogAddC)(BYTE, char *szlog, ...);
extern void (*LogAddL)(char *szlog, ...);
extern void (*LogAddHeadHex)(char *,char *data, int);
extern void (*LogTextPaint)(HWND hWnd);
extern void (*LogAddTD)(char *szlog, ...);
#ifdef MONSTER_ITEMDROP_LOG_20040309
extern void (*LogAdd_MonsterItemDrop)(char *szlog, ...);
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
extern void (*LogAddTD_TempLog)(char *szlog, ...);
#endif

extern void LogInit(int logprint);
extern char * LogGetFileName();
extern void LogClose();
extern void LogTextClear();
extern BOOL LogDateChange();

extern void MsgBox(char *szlog, ...);

#if TESTSERVER == 1
extern void TRACE_LOG(char *szFormat, ...);
#endif

#endif
