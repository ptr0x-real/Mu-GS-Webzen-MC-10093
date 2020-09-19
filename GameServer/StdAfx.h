// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x500

#pragma warning(disable:4067)
#pragma warning(disable:4101)
#pragma warning(disable:4102)
#pragma warning(disable:4786)
#pragma warning(disable:4800)

// Windows Header Files:

#define	USING_MFC_LIB_20050107							// 게임서버 MFC 라이브러리 추가 (b4nfter)

// *> . MFC 라이브러리 추가 시 해야할 일
// 1 . Project -> Settings -> General -> Microsoft Foundation Classes -> Use MFC in a Static Library 로 변경 (기본 Not Using MFC)
// 2 . Project -> Settings -> Link -> Input -> Ignore libraries -> libc.lib 추가 (기본 없음)

#ifdef	USING_MFC_LIB_20050107
#include <AFX.H>
#else
#include <windows.h>
#endif

#include "..\\Include\\Define.h"
#include "Logproc.h"
#include "..\\Common\\Msg.h"

#ifdef HACKTOOL_KUZIMO
	#include "AntiHackDef.h"
#endif


// C RunTime Header Files
#include <Windows.h>
#include <winbase.h>
#include <winsock2.h>
#include <CommCtrl.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <process.h>
#include <direct.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <fstream>
#include <string>
#include <ios>
#include <algorithm>
#include <vector>
#include <set>
#include <sql.h>
#include <sqlext.h>
#include <rpc.h>
#include <rpcdce.h>
#include <Psapi.h>

// Local Header Files
#ifdef MODIFY_NOTICE_20040325
#include "TNotice.h"
#endif

#include "Database.h"
#include "Manager.h"
#include "Global.h"
#include "Post.h"
#include "PKClear.h"
#include "ResetSystem.h"
#include "VIPSystem.h"
#include "DropTable.h"
#include "BuffHelper.h"
#include "TerrainManager.h"
#include "EngagePVP.h"
#include "ArrowSystem.h"
#include "BalanceSystem.h"
#include "DynamicExperience.h"
#include "ExpSettings.h"
#include "IniFile.h"
#include "NSIniFile.h"
#include "LogShop.h"
#include "OffAfk.h"
#include "Offtrade.h"
#include "PShop.h"
#include "ConnectBonus.h"
#include "Sobrevivencia.h"
#include "Functions.h"
#include "Command.h"

using std::vector;
using std::string;
using namespace std;

#pragma comment( lib , "wsock32.lib" )
#pragma comment( lib , "ws2_32.lib" )
#pragma comment( lib , "COMCTL32.lib" )
#pragma comment( lib , "rpcrt4.lib")
#pragma comment( lib , "Psapi.lib")

#define _GAMESERVER_
// TODO: reference additional headers your program requires here

extern CMsg			lMsg;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
