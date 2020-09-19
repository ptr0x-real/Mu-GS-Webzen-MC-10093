
#if !defined(AFX_GAMESERVER_H__BED99C0E_ECAD_416E_9836_277FAE878298__INCLUDED_)
#define AFX_GAMESERVER_H__BED99C0E_ECAD_416E_9836_277FAE878298__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#define WM_ASS_SERVERMSG		WM_USER+1
#define WM_ASS_CLIENTMSG		WM_USER+2
#define WM_ASS_JOINCLIMSG		WM_USER+3	// 인증서버 접속용 클라이언트 메시지
#define WM_ASS_CREATEGAMESERVER	WM_USER+5	// 서버를 시작하라고.. 알림

#ifdef GAMESERVER_INFO_TCP_20031203
#define WM_ASS_CSERVERMSG		WM_USER+26	// 접속서버용 클라이언트
#endif


#define WM_ASS_RANKINGCLIMSG	WM_USER+12	// 랭킹서버 접속용 클라이언트 메시지
#define WM_ASS_EVENTCHIPCLIMSG	WM_USER+13	// 이벤트칩서버 접속용 클라이언트 메시지

#define WM_ASS_DATACLIMSG		WM_USER+10

#ifdef _NEW_EXDB_
//##EXDB##
#define WM_ASS_EXDATACLIMSG		WM_USER+14
//##EXDB##
#endif


extern HWND		ghWnd;
extern int		gCurPaintMapNumber;
extern int		gCurPaintType;
extern int		gServerReady;

#ifdef MODIFY_AUTHSERVER_CLOSE_20041207
extern BOOL		gbAuthServerClosed;
#endif


extern void		gSetDate();
#endif // !defined(AFX_GAMESERVER_H__BED99C0E_ECAD_416E_9836_277FAE878298__INCLUDED_)
