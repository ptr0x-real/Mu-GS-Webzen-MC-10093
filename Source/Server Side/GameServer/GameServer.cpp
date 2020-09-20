// GameServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <ctime>
#ifndef ADD_MINIDUMP_20081222

#ifdef HAPPY_CAT_USE_EXCEPTIONHANDLER
#include "ExceptionHandler.h"
#endif //HAPPY_CAT_USE_EXCEPTIONHANDLER

#endif //ADD_MINIDUMP_20081222


#include "resource.h"
#include <CommCtrl.h>
#include <stdio.h>
#include "Include\Prodef.H"
#include "Include\SProdef.h"
#include "wsGameServer.h"
#include "wsJoinServerCli.h"
#include "User.h"
#include "Gamemain.h"
#include "Shop.h"
#include "GameServer.h"
#include "Closeplayer.h"
#include "WhisperCash.h"
#include "MagicDamage.h"
#include "Acceptip.h"
#include "Giocp.h"
#include "DragonEvent.h"
#include "CalCharacter.h"
#include "OffExp.h"
#include "PickUpSystem.h"

#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif


#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#ifdef MAP_SERVER_WORK_20041030
#include "MapServerManager.h"
#endif

#ifdef 	CASTLE_MAIN_SCHEDULER_20041111
#include "CastleSiege.h"
#endif

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
#include "CastleSiegeSync.h"
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#include "Weapon.h"
#endif

#ifdef CASTLEDEEP_EVENT_20050413
#include "CastleDeepEvent.h"
#endif

#include "CWhatsUpDummyServer.h"

#include "Dirpath.h"
#include "DBSockMng.h"
#include "AttackEvent.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"

#include "News.h"

#include "HappyHour.h"

#ifdef MONSTER_HERD_SYSTEM_20031120
#include "RingAttackEvent.h"

CRingMonsterHerd		g_MonsterHerdTest;
CRingAttackEvent		g_RingAttackEvent;

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
#include "XMasAttackEvent.h"
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014
#endif // MONSTER_HERD_SYSTEM_20031120

#include "EledoradoEvent.h"

extern wsJoinServerCli  wsDataCli;

#ifdef EVENT_MANAGEMENT_20040711
#include "EventManagement.h"
#endif


#ifdef ADD_SERVER_ALERT_USING_WHATSUP_01_20050628
#include "TServerAlertManager.h"
#endif

#ifdef ADD_SERVER_INFO_DISPLAY_EXTEND_01_20060313
#include "TServerInfoDisplayer.h"
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
#include "TMonsterSkillElement.h"
#include "TMonsterSkillUnit.h"
#include "TMonsterSkillManager.h"

#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
#include "TMonsterAI.h"
#include "TMonsterAIUnit.h"
#include "TMonsterAIAutomata.h"
#endif

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
#include "Crywolf.h"
#endif

#ifdef MU_CRYWOLF_SERVER_SYNC_20050505
#include "CrywolfSync.h"
#endif

#ifdef ADD_KANTURU_20060627
#include "Kanturu.h"
#endif

#ifdef ADD_RAKLION_20080408
#include "Raklion.h"
#endif // ADD_RAKLION_20080408

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#include "CashShop.h"
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#include "CashItemPeriodSystem.h"
#endif //ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#include "BuffEffectSlot.h"
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef ADD_MINIDUMP_20081222
#include "MiniDump.h"
#endif

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"
#endif // ADD_NEWPVP_PKFIELD

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
#include "HackToolBlock.h"
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
#include "KeyGenerater.h"
#endif // ADD_MAPMOVE_PROTOCOL_20090327

//----------------------------------------------------------------------------
// E X T E R N
//----------------------------------------------------------------------------
extern CWhisperCash	WhisperCash;
extern CDirPath		gDirPath;

#ifndef ADD_SERVER_ALERT_USING_WHATSUP_01_20050628
// Ŭ���� ������ ����.
CWhatsUpDummyServer gWhatsUpDummyServer;
#endif


#ifdef GAMESERVER_INFO_TCP_20031203
extern char connectserverip[20];
extern int  connectserverport;
extern int  connectserverport_tcp;

extern int	g_ConServerInfoSendType;		// ���Ӽ����� ������ ���� Ÿ���� ���� (0:UDP / 1:TCP)
#endif


#define MAX_LOADSTRING 100

#define MTIME_HOUR			1001
#define MTIME_SECOND		1002	// 1�ʿ� �ѹ��� ȣ��Ǵ� Ÿ�̸�
#define MTIME_DAY			1003

#ifdef FOR_BLOODCASTLE
#define MTIME_100			1004	// 1�ʿ� 10���� ȣ��Ǵ� Ÿ�̸�
#define MTIME_300			1005	// 1�ʿ� 10���� ȣ��Ǵ� Ÿ�̸�
#define MTIME_500			1006	// 1�ʿ� 2���� ȣ��Ǵ� Ÿ�̸�
#define MTIME_MIN			1007

#define MTIME_DBSGUILDLISTREQUEST		1008
#else
#define MTIME_100			1004	// 1�ʿ� 10���� ȣ��Ǵ� Ÿ�̸�
#define MTIME_500			1005	// 1�ʿ� 2���� ȣ��Ǵ� Ÿ�̸�
#define MTIME_MIN			1006

#define MTIME_DBSGUILDLISTREQUEST		1007
#endif


#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
#define TIMER_MONSTER_MOVE_CYCLE	100		// Ÿ�̸� ����Ŭ ms
#define TIMER_MONSTER_MOVE			1010	// 1�ʿ� 10���� ȣ��Ǵ� Ÿ�̸� ID
#define TIMER_MONSTER_AI			500		// Ÿ�̸� ����Ŭ ms
#define TIMER_MONSTER_AI_CYCLE		1011	// 1�ʿ� 2���� ȣ��Ǵ� Ÿ�̸�
#endif

#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
extern INT	g_iCastleItemMixLimit;	// �������� Ư�� ������ ���� �Ϸ� ���� Ƚ��
#endif


// Global Variables:
HINSTANCE hInst;								// current instance
HWND  ghWnd;
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#ifdef MU_CASTLESIEGE_LOG_20050107
HWND g_hCsLogDlgProc;
BOOL CALLBACK		CsLogDlgProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
#endif

int GameServerStart();
int AllServerStart();

extern CwsGameServer	wsGServer;
extern wsJoinServerCli	wsJServerCli;
extern wsJoinServerCli	wsJDataCli;
extern CDragonEvent*	DragonEvent;
extern CAttackEvent*	AttackEvent;

#ifdef GAMESERVER_INFO_TCP_20031203
extern wsJoinServerCli	wsConnectServerCli;
#endif

int		gCurPaintMapNumber=0;
int		gCurPaintType=0;
int		gCurPaintPlayer=0;

int		gCloseMsgTime=0;
int		gCloseMsg=0;

int		gServerReady=0;	// ���������� �����ͼ��� ��� ������ �Ϸ�Ǿ��°�?
char	JoinServerIp[256]="";
char	DataServerIp[256]="";

#ifdef ADD_GAMESERVER_PARAMETER_20050728
char	DataServerIp2[256]="";	// �����ͼ��� 2 IP
char	ExDbIp[256]="";			// EXDB IP
#endif

#ifdef MODIFY_AUTHSERVER_CLOSE_20041207
BOOL	gbAuthServerClosed	= FALSE;
#endif


#ifdef EVENT_OPTION_RELOAD_20040305				// ���� �̺�Ʈ �ɼǸ� �ٽ� �б� ����
extern void ReadEventInfo	(MU_EVENT_TYPE eEventType);
#endif

#ifdef ETC_OPTION_RELOAD_20040308				// ���� �ɼ� �� �̺�Ʈ ���� �͵鸸 ���ε�
extern void ReadGameEtcInfo	(MU_ETC_TYPE eGameEtcType);
#endif


#ifdef PRINT_MAGICBOOK_ITEMINSERT_LOG

struct MONITEM {
	INT iMagicBook;
	INT iETC;
	INT iHolyGem1;			// �༮ 
	INT iHolyGem2;			// ���� 
	INT iHolyGem3;			// ȥ�� 
	INT iHolyGem4;			// ���� 
	INT iHolyGem5;			// â�� 
	MONITEM() {
		iMagicBook = 0;
		iHolyGem1 = 0;		// �༮ 
		iHolyGem2 = 0;		// ���� 
		iHolyGem3 = 0;		// ȥ�� 
		iHolyGem4 = 0;		// ����
		iHolyGem5 = 0;		// â�� 
		iETC = 0;
	}
};

extern MONITEM g_MONITEM[200];

#endif

#include <crtdbg.h>


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

#ifdef ADD_MINIDUMP_20081222
	CMiniDump::Begin();
#endif

#ifdef	fCHECK_CRT_DBG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif

	MSG msg;
	HACCEL hAccelTable;

#ifndef MUTEX_REMOVE

#if TESTSERVER == 0
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING
		HANDLE hMutex = CreateMutex(NULL, FALSE, "WZ_MU_GS_MUTEX");
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(hMutex);
			MessageBox (NULL, "Webzen Mu Game Server is already Running.", "Error", MB_OK | MB_ICONERROR);
			return 0;
		}
	#endif
#endif

#endif
	
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMESERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	InitCommonControls();

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	ShowWindow(ghWnd, SW_NORMAL);
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GAMESERVER);
	GiocpInit();			// ���� ��ȣȭ ��Ŷ�� ���� ����
	GameMainInit(ghWnd);	// ��κ��� ���� �ʱ�ȭ & �ε�

	int serverport;

#ifdef ADD_GAMESERVER_PARAMETER_20050728
	memset(DataServerIp2, 0, sizeof(DataServerIp2));
	memset(ExDbIp, 0, sizeof(ExDbIp));
	DataServerPort2	= 0;
	ExDbPort		= 0;
	
	if( strlen(lpCmdLine) > 1 ) 
		sscanf(lpCmdLine, "%s %d %s %d %d %s %d %s %d", 
		JoinServerIp, 
		&JoinServerPort, 
		DataServerIp, 
		&DataServerPort, 
		&serverport,
		DataServerIp2, 
		&DataServerPort2, 
		ExDbIp, 
		&ExDbPort
		);
	else {
		serverport		= DEFMAPSERVERPORT;
		JoinServerPort	= DEFJOINSERVERPORT;
		DataServerPort	= DEFDATASERVERPORT;
		DataServerPort2	= DEFDATASERVERPORT+2;
		ExDbPort		= DEFEXDBSERVERPORT;
		MsgBox(lMsg.Get(114));
	}

	if (DataServerIp2[0] == 0) {
		memcpy (DataServerIp2, DataServerIp, sizeof(DataServerIp));
	}
	if (DataServerPort2 == 0) {
		DataServerPort2	= DEFDATASERVERPORT+2;
	}
	if (ExDbIp[0] == 0) {
		memcpy (ExDbIp, DataServerIp, sizeof(DataServerIp));
	}
	if (ExDbPort == 0) {
		ExDbPort		= DEFEXDBSERVERPORT;
	}

#else
	if( strlen(lpCmdLine) > 1 ) sscanf(lpCmdLine, "%s %d %s %d %d", JoinServerIp, &JoinServerPort, DataServerIp, &DataServerPort, &serverport);
	else {
		serverport		= DEFMAPSERVERPORT;
		JoinServerPort	= DEFJOINSERVERPORT;
		DataServerPort	= DEFDATASERVERPORT;
		MsgBox(lMsg.Get(114));
	}
#endif
	GameServerPort = serverport;

#ifdef WHATSUP_DUMMY_SERVER
	gWhatsUpDummyServer.Start(ghWnd, GameServerPort+1);
#endif
		
	AllServerStart();	// ����, DB, ��ŷ, �̺�Ʈ ���� ����

#ifdef PRINT_MAGICBOOK_ITEMINSERT_LOG
	char szTemp[255];
	wsprintf(szTemp, "���� ����\t����\t�༮\t����\tȥ��\t����\tâ��\t�׿�\n");
	LogAdd("���� ����\t����\t�༮\t����\tȥ��\t����\tâ��\t�׿�");
	OutputDebugString(szTemp);

	for (int i = 67 ; i < 109 ; i++) {

		wsprintf(szTemp, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				 i,
				 g_MONITEM[i].iMagicBook,
				 g_MONITEM[i].iHolyGem1,
				 g_MONITEM[i].iHolyGem2,
				 g_MONITEM[i].iHolyGem3,
				 g_MONITEM[i].iHolyGem4,
				 g_MONITEM[i].iHolyGem5,
				 g_MONITEM[i].iETC);
		OutputDebugString(szTemp);

		LogAdd("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
			i,
			g_MONITEM[i].iMagicBook,
			g_MONITEM[i].iHolyGem1,
			g_MONITEM[i].iHolyGem2,
			g_MONITEM[i].iHolyGem3,
			g_MONITEM[i].iHolyGem4,
			g_MONITEM[i].iHolyGem5,
			g_MONITEM[i].iETC
			);
	}
#endif
	
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

#ifdef ADD_MINIDUMP_20081222
	CMiniDump::End();
#endif

	return msg.wParam;
}

// ��� ������ �����Ѵ�.
int AllServerStart()
{
	if( !JoinServerConnected )
	{
		if( GMJoinServerConnect(JoinServerIp, WM_ASS_JOINCLIMSG) == FALSE )
		{
			MsgBox(lMsg.Get(400));
			return FALSE;
		}
	}

	if( !DevilSquareEventConnect && !IsDevilSquareEventConnected )
	{	// ��ŷ���� ����
		if( GMRankingServerConnect(gDevilSquareEventServerIp, WM_ASS_RANKINGCLIMSG) == FALSE )
		{
			MsgBox("Ranking Server�� ���� �ȵ�");
			return FALSE;
		}		
		IsDevilSquareEventConnected = 1;
	}
	
	if( EventChipServerConnect && !IsEventChipServerConnected )
	{	// ���� ���� �̺�Ʈ���� ����
		if( GMEventChipServerConnect(gEventChipServerIp, WM_ASS_EVENTCHIPCLIMSG) == FALSE )
		{
			MsgBox("Event Chip Server�� ���� �ȵ�");
			return FALSE;
		}		
		IsEventChipServerConnected = 1;
	}

	/*if( !DataServerConnected )
	{
		if( GMDataServerConnect(DataServerIp, WM_ASS_DATACLIMSG)== FALSE )
		{
			MsgBox(lMsg.Get(399));
			return FALSE;
		}

		
	}
	*/
	cDBSMng.Connect();

#ifdef GAMESERVER_INFO_TCP_20031203
/*	if( !IsConnectServerConnected )
	{	// ���Ӽ����� ����
		if( ConnectServerConnect(connectserverip, WM_ASS_CSERVERMSG) == FALSE )
		{
			MsgBox("Connect Server�� ���� �ȵ�");
			return FALSE;
		}		
		IsConnectServerConnected = 1;
	}
*/
#endif

	return TRUE;
}

// ���Ӽ����� �����Ѵ�.
int GameServerStart()
{
	if( GameMainServerCreate(WM_ASS_SERVERMSG, WM_ASS_CLIENTMSG) == FALSE )
	{
		MsgBox(lMsg.Get(398));
		return FALSE;
	}

	CreateGIocp(GameServerPort);

	SetTimer(ghWnd, 100, 2000, NULL);
	SetTimer(ghWnd, 101, 1000, NULL);
	//SetTimer(ghWnd, 100, 500, NULL);
	SetTimer(ghWnd, MTIME_HOUR, 1000*60, NULL);
	SetTimer(ghWnd, MTIME_DAY, (1000*60), NULL);
	SetTimer(ghWnd, MTIME_MIN, 1000*10, NULL);
	//SetTimer(ghWnd, MTIME_DAY, 1000, NULL);
	//SetTimer(ghWnd, 1000, 1000, NULL);
	SetTimer(ghWnd, 1000, 1000, NULL);
	SetTimer(ghWnd, MTIME_SECOND, 1000, NULL);
	SetTimer(ghWnd, MTIME_500, 500 , NULL);

#ifdef FOR_BLOODCASTLE
	SetTimer(ghWnd, MTIME_300, 300 , NULL);
	SetTimer(ghWnd, MTIME_100, 100 , NULL);
#else	
	SetTimer(ghWnd, MTIME_100, 300 , NULL);
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	SetTimer(ghWnd, TIMER_MONSTER_MOVE, TIMER_MONSTER_MOVE_CYCLE,	NULL);
	SetTimer(ghWnd, TIMER_MONSTER_AI,	TIMER_MONSTER_AI_CYCLE,		NULL);
#endif

#if (DBGUILDLOAD==1)
	SetTimer(ghWnd, MTIME_DBSGUILDLISTREQUEST, 1000, NULL);
#else
	MsgBox("Guild List not load");
	GameServerInfoSendStart();
#endif
	return TRUE;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GAMESERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_GAMESERVER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   ghWnd = hWnd;
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
#include "Include\Prodef.h"
#include "Protocol.h"

#ifdef MAP_SERVER_WORK_20041030
#include "Sprotocol.h"
#endif


extern void CSPJoinIdPassRequest(LPPMSG_IDPASS lpMsg, short aIndex);
extern void GJPGetCharListRequest(short aIndex);
extern void CSPJoinIdPassRequest(LPPMSG_IDPASS lpMsg, int aIndex);

extern void GraphPaint(HWND hWnd);

extern void TestSend();

extern int gServerMaxUser;
extern void CSPJoinIdPassRequestTEST(LPPMSG_IDPASS lpMsg, int aIndex);

void CGPCharacterMapJoinRequest(LPPMSG_CHARMAPJOIN lpMsg, int aIndex);


void TestDSSend();

extern int gCurConnectUser;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;
	//TCHAR szHello[MAX_LOADSTRING];
	//LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_CREATE :
#ifdef MU_CASTLESIEGE_LOG_20050107
			g_hCsLogDlgProc = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CS_LOG_DLG), hWnd, CsLogDlgProc);
#endif			
			return 0;
		
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{

#ifdef ADD_SERVER_ALERT_USING_WHATSUP_01_20050628
				case IDM_ALERT_ALL_CLEAR:
					{
						g_ServerAlertManager.ClearAllAlert();
					}
					break;
#endif

				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:				   
				   SendMessage(hWnd, WM_CLOSE, 0,0);
				   break;
				case ID_MENU_DISPLAYER :
					{
					Ccloseplayer	cp;
					cp.Load("closeplayer.txt");
					}
					break;
				case ID_MENU_DISCONNECT :
					gObjAllDisconnect();
					break;
				case ID_MENU_USERLOGOUT :
					gObjAllLogOut();
					break;				
				case ID_LOAD_MONSTERPOS :
					GameMonsterAllCloseAndReLoad();
					break;
				case ID_LOAD_SHOP :
					ShopDataLoad();
					break;
#ifdef MAP_SERVER_WORK_20041030
				case ID_LOAD_MAPSVRINFO :
					g_MapServerManager.LoadData(gDirPath.GetNewPath("MapServerInfo.dat"));
					break;
#ifdef CASTLE_MAIN_SCHEDULER_20041111				// �������� �� ���������� ����
				case ID_LOAD_CASTLESIEGE :
					if (g_CastleSiege.Ready(g_MapServerManager.GetMapSvrGroup()) == TRUE) {
						if (g_CastleSiege.LoadData(gDirPath.GetNewPath("MuCastleData.dat"))) {
							g_CastleSiege.LoadPreFixData(gDirPath.GetNewPath("commonserver.cfg"));
							g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);		// DB �����͸� ���� �غ� �Ѵ�.
						}
					}
					break;
#endif
#endif
				case ID_LOAD_ACCEPTIPLOAD :
					acceptIP.Load(gDirPath.GetNewPath("Iplist.dat"));
					break;
				case ID_MENUGAMESERVEROPTIONLOAD :	// ���Ӽ��� ���� �ɼ� �б�
					ReadCommonServerInfo();
					break;
#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
				case ID_MENUSHOPLISTRELOAD:
					g_CashShop.CashShopOptioNReload();
					break;
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
				case ID_VIEW_LOG :
					gCurPaintType = 0;
					break;
#ifdef MU_CASTLESIEGE_LOG_20050107
				case ID_VIEW_LOG_CS :
					SendMessage(GetDlgItem(g_hCsLogDlgProc, IDC_MAIN_LIST), LB_ADDSTRING, 0, rand()%2?(LPARAM)"HELLO WORLD":(LPARAM)"�ȳ�");
					ShowWindow(g_hCsLogDlgProc, TRUE);
					break;
#endif					
				case ID_MAP_1 :
					gCurPaintMapNumber = 0; 
					gCurPaintType = 1;
					break;
				case ID_MAP_2 :
					gCurPaintType = 1;
					gCurPaintMapNumber = 1; 
					break;
				case ID_MAP_3 : 
					gCurPaintType = 1;
					gCurPaintMapNumber = 2; 
					break;
				case ID_MAP_4 :
					gCurPaintType = 1;
					gCurPaintMapNumber = 3; 
					break;
#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
				case ID_MAP_CRYWOLF :
					gCurPaintType = 1;
					gCurPaintMapNumber = MAP_INDEX_CRYWOLF_FIRSTZONE; 
					break;
#endif
				case ID_MENUITEM_EVENT :
					if( DragonEvent->GetState() )
					{
						DragonEvent->End();
					}
					else DragonEvent->Start();
					break;

				case ID_MENUITEM_STAROFXMAS_EVENT :
					if( gXMasEvent )
					{
						gXMasEvent = 0;
					}
					else gXMasEvent = 1;
					break;				
				
					// ���� �̺�Ʈ Ȱ��ȭ�޴�
				case ID_MENUITEM_FIRECRACKER_EVENT :
					if( gFireCrackerEvent )
					{
						gFireCrackerEvent = 0;
					}
					else gFireCrackerEvent = 1;
					break;		

				case ID_LOAD_CUSTOM:
					
					gBalanceSystem.Load(gDirPath.GetNewPath(BALANCESYSTEM_DIR));
					gSettings = new CSettings; gSettings->Loading(); _dynamic_experience._loading(); 
					Balancer.Load();
					cexDrop.LoadConfig("..\\data\\DropTable.cfg");
		           // s_PickUp.LoadFile(gDirPath.GetNewPath("..\\data\\PickupSystem.txt"));
					//_beginthread(SORTEO_CUSTOM::Sorteo,0,NULL);// Sorteo custom
					break;

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
				case ID_GAMEGUARD_MODULE_UPDATE:
					char chGGModuleInfo[256];
					ReadGameGuardUpdateInfo();
					if( strlen( gGGModuleName ) )
					{
						AddAuthProtocol25( gGGModuleName );
						ModuleInfo25( chGGModuleInfo, 256);
						LogAddTD("[GameGuard][Add New Auth Protocol] - %s", gGGModuleName);
						LogAddTD("[GameGuard][Current Module Info] - %s", chGGModuleInfo);
					}
					break;
#endif

				case ID_MENU_FIVECLOSE :
					if( gCloseMsg == 0 )
					{
						gCloseMsgTime=300;
						gCloseMsg=1;
						AllSendServerMsg(lMsg.Get(1128));
					}
					break;
				case ID_MENU_THREECLOSE :
					if( gCloseMsg == 0 )
					{
						gCloseMsgTime=180;
						gCloseMsg=2;
						AllSendServerMsg(lMsg.Get(1129));	// "3���� ������ �ڵ� ���� �˴ϴ�."
					}
					break;
				case ID_MENU_ONECLOSE :
					if( gCloseMsg == 0 )
					{
						gCloseMsgTime=60;
						gCloseMsg=3;
						AllSendServerMsg(lMsg.Get(1130));//1���� ������ �ڵ� ���� �˴ϴ�
					}
					break;
#ifdef EVENT_OPTION_RELOAD_20040305						// �̺�Ʈ ���� �ɼǸ� ���ε� �Ѵ�.
				case ID_EVENTLOAD_ALL :					// ��� �ɼ� ���ε�
					ReadEventInfo (MU_EVENT_ALL);
					break;
				case ID_EVENTLOAD_DEVILSQUARE :			// ����������
					ReadEventInfo (MU_EVENT_DEVILSQUARE);
					break;
				case ID_EVENTLOAD_BLOODCASTLE :			// ����ĳ��
					ReadEventInfo (MU_EVENT_BLOODCASTLE);
					break;
				case ID_EVENTLOAD_UNDERTROOPATTACK :	// ���ϱ���
					ReadEventInfo (MU_EVENT_ATTACKEVENTTROOP);
					break;
				case ID_EVENTLOAD_GOLDENTROOPATTACK :	// Ȳ�ݱ���
					ReadEventInfo (MU_EVENT_GOLDENTROOP);
					break;
				case ID_EVENTLOAD_WHITEMAGEATTACK :		// ��������籺��
					ReadEventInfo (MU_EVENT_WHITEMAGETROOP);
					break;
				case ID_EVENTLOAD_LOVEPANGPANG :		// ���귯������
					ReadEventInfo (MU_EVENT_LOVEPANGPANG);
					break;
				case ID_EVENTLOAD_FIRECRACKER :			// ����
					ReadEventInfo (MU_EVENT_FIRECRACKER);
					break;
				case ID_EVENTLOAD_XMASSTAR :			// ��ź�Ǻ�
					ReadEventInfo (MU_EVENT_XMASSTAR);
					break;
				case ID_EVENTLOAD_HEARTOFLOVE :			// �������Ʈ
					ReadEventInfo (MU_EVENT_HEARTOFLOVE);
					break;
				case ID_EVENTLOAD_SAY_HAPPYNEWYEAR :	// NPC �����λ�
					ReadEventInfo (MU_EVENT_SAY_HAPPYNEWYEAR);
					break;
				case ID_EVENTLOAD_SAY_MERRYXMAS :		// NPC ũ���������λ�
					ReadEventInfo (MU_EVENT_SAY_MERRYXMAS);
					break;
					
#ifdef CHAOSCASTLE_SYSTEM_20040408 
				case ID_EVENTLOAD_CHAOSCASTLE :			// ī���� ĳ��
					ReadEventInfo (MU_EVENT_CHAOSCASTLE);
					break;
#endif

#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
				case ID_EVENTLOAD_RIBBONBOX:		// ũ�������� �������� �̺�Ʈ
					ReadEventInfo( MU_EVENT_CHRISTMAS_RIBBONBOX );
					break;
#endif // #ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212

#ifdef VALENTINES_DAY_BOX_EVENT_20060124
				case ID_EVENTLOAD_CHOCOLATEBOX:		// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ
					ReadEventInfo( MU_EVENT_VALENTINESDAY_CHOCOLATEBOX );
					break;
#endif // #ifdef VALENTINES_DAY_BOX_EVENT_20060124

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
				case ID_EVENTLOAD_CANDYBOX:			// ȭ��Ʈ���� �������� �̺�Ʈ
					ReadEventInfo( MU_EVENT_WHITEDAY_CANDYBOX );
					break;
#endif	// #ifdef WHITEDAY_CANDY_BOX_EVENT_20060222	

#endif
#ifdef ETC_OPTION_RELOAD_20040308				// ���� �ɼ� �� �̺�Ʈ ���� �͵鸸 ���ε�
				case ID_ETCLOAD_ALL :
					ReadGameEtcInfo (MU_ETC_ALL);
					break;
				case ID_ETCLOAD_CREATECHAR :
					ReadGameEtcInfo (MU_ETC_CREATECHARACTER);
					break;
				case ID_ETCLOAD_GUILD :
					ReadGameEtcInfo (MU_ETC_GUILD);
					break;
				case ID_ETCLOAD_TRADE :
					ReadGameEtcInfo (MU_ETC_TRADE);
					break;
				case ID_ETCLOAD_CHAOSBOX :
					ReadGameEtcInfo (MU_ETC_USECHAOSBOX);
					break;
				case ID_ETCLOAD_PSHOP :
					ReadGameEtcInfo (MU_ETC_PERSONALSHOP);
					break;
				case ID_ETCLOAD_PKITEMDROP :
					ReadGameEtcInfo (MU_ETC_PKITEMDROP);
					break;
				case ID_ETCLOAD_ITEMDROPRATE :
					ReadGameEtcInfo (MU_ETC_ITEMDROPRATE);
					break;
				case ID_ETCLOAD_SPEEDHACK :
					ReadGameEtcInfo (MU_ETC_SPEEDHACK);
					break;
				case ID_ETCLOAD_GAMEGUARD :
					ReadGameEtcInfo (MU_ETC_GAMEGUARD);
					break;
#endif
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		/*case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;*/
		case WM_ASS_SERVERMSG :
			GMServerMsgProc(wParam, lParam);
			break;
		case WM_ASS_CLIENTMSG :
			{
			int wp=wParam;
			int lp=lParam;
			GMClientMsgProc(wParam, lParam);
			//GMClientMsgProc(wp, lp);
			}
			break;
		case WM_ASS_JOINCLIMSG :
			GMJoinClientMsgProc(wParam, lParam);
			break;

		case WM_ASS_RANKINGCLIMSG :
			GMRankingClientMsgProc(wParam, lParam);
			break;

		case WM_ASS_EVENTCHIPCLIMSG :
			GMEventChipClientMsgProc(wParam, lParam);
			break;
			
#ifdef GAMESERVER_INFO_TCP_20031203
		case WM_ASS_CSERVERMSG :
			GMConnectServerClientMsgProc(wParam, lParam);
			break;
#endif

		case WM_ASS_DATACLIMSG :
			//GMDataClientMsgProc(wParam, lParam);
			cDBSMng.MsgProc(0, wParam, lParam);
			break;
		case WM_ASS_DATACLIMSG+1 :
			cDBSMng.MsgProc(1, wParam, lParam);
			//GMDataClientMsgProc(wParam, lParam);
			break;
#ifdef _NEW_EXDB_
//##EXDB##
		case WM_ASS_EXDATACLIMSG :
			ExDataClientMsgProc(wParam, lParam);
			break;
//##EXDB##
#endif

		case WM_ASS_CREATEGAMESERVER :	// ���Ӽ����� ���� �Ǵ°�?
			if( gServerReady == 2 )
			{
				GameServerStart();
			}
			break;
		case WM_TIMER :
			switch( wParam )
			{
			case MTIME_DBSGUILDLISTREQUEST :
#ifdef _NEW_EXDB_
				DGGuildListRequest();
#endif
				KillTimer(ghWnd, MTIME_DBSGUILDLISTREQUEST);	// ���� �ѹ��� ��û�ϰ� Ÿ�̸� ����
				break;
			case 100 :
				{
				RECT rect;
				HDC hdc = GetDC(hWnd);

				GetClientRect(hWnd, &rect);
				FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));	
				ReleaseDC(hWnd, hdc);
	
				if( gCurPaintType == 0 ) {
					if( LogTextPaint != NULL ) LogTextPaint(hWnd);
				}
				gObjViewportPaint(hWnd, gCurPaintPlayer);

#ifdef ADD_SERVER_ALERT_USING_WHATSUP_01_20050628
				g_ServerAlertManager.Run( hWnd );
#endif
				
#ifdef ADD_SERVER_INFO_DISPLAY_EXTEND_01_20060313
				g_ServerInfoDisplayer.Run( hWnd );
#endif
				}
				//GraphPaint(hWnd);
#ifdef MAP_SERVER_WORK_20041030
				GJNotifyMaxUserCount();
#endif
				break;
			case MTIME_500 :
				MonsterAndMsgProc();
#ifdef ADD_NEWPVP_PKFIELD
				g_NewPVP.Run();
#endif // ADD_NEWPVP_PKFIELD
				break;
#ifdef FOR_BLOODCASTLE
			case MTIME_300 :
				MoveMonsterProc();
#ifdef ADD_KANTURU_20060627
				// 20060818 RKFKA - For Test
				g_Kanturu.UserMonsterCountCheck();
#endif
				break;
			case MTIME_100 :
				g_BloodCastle.Run();
	#ifdef MONSTER_HERD_SYSTEM_20031120
				g_RingAttackEvent.Run();
	#endif	// #ifdef MONSTER_HERD_SYSTEM_20031120

	#ifdef CHAOSCASTLE_SYSTEM_20040408
				g_ChaosCastle.Run();
	#endif	// #ifdef CHAOSCASTLE_SYSTEM_20040408

	#ifdef CASTLE_MAIN_SCHEDULER_20041111
				g_CastleSiege.Run();
	#endif	// #ifdef CASTLE_MAIN_SCHEDULER_20041111

	#ifdef CASTLEDEEP_EVENT_20050413
				g_CastleDeepEvent.Run();
	#endif	// #ifdef CASTLEDEEP_EVENT_20050413
				
	#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
				g_CsNPC_Weapon.WeaponAttackProc();
	#endif  // ADD_NEW_WEAPON_FOR_CASTLE_01_20041116


	#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
				g_Crywolf.Run();
	#endif	// CRYWOLF_MAIN_SCHEDULER_20050505

	#ifdef ADD_KANTURU_20060627
				g_Kanturu.Run();
	#endif // ADD_KANTURU_20060627

	#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
				g_CashShop.CheckShopServerConnectState();
	#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

	#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// Ÿ�̸�
				g_IllusionTempleEvent.IllusionTempleRun();
	#endif
	
	#ifdef ADD_RAKLION_20080408
				g_Raklion.Run();
	#endif // ADD_RAKLION_20080408

	#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
				g_XMasAttackEvent.Run();
	#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

				g_PvpEvent.Run();
				//DevGamesX 01/05/2018 - OffExp
				OffExp.MainFunction();

				break;
#else	// #ifdef FOR_BLOODCASTLE
			case MTIME_100 :
				MoveMonsterProc();
				break;
#endif	// #ifdef FOR_BLOODCASTLE

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
			case TIMER_MONSTER_MOVE :
				TMonsterAI::MonsterMoveProc();
				break;
			case TIMER_MONSTER_AI :
				TMonsterAI::MonsterAIProc();
				break;
#endif

			case 101 :
				{
				int n;
				for( n=0; n<MAX_OBJECT; n++)
					gObjStateSetCreate(n);
				
				for( n=0; n<MAX_OBJECT; n++)
					gObjViewportListDestroy(n);
								
				for( n=0; n<MAX_OBJECT; n++)
				{
					gObjViewportListCreate(n);
					
				}
				
				for( n=0; n<MAX_OBJECT; n++)
					gObjViewportListProtocol(n);

#ifdef PERSONAL_SHOP_20040113	// ���λ��� ����Ʈ �籸�� �� �߰��� ���� ����Ʈ ���� (������ ������ �ϹǷ� ���⼭�� ���ص� ��)
				if (gDoPShopOpen) {
					for( n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++) {
						PShop_ViewportListRegenarate(n);
					}
					for( n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++) {
						if (gObjIsConnected(n)) {
							gObj[n].m_bPShopItemChange = false;
						}
					}
				}
#endif

#ifdef ADD_NEW_UNION_01_20041006 // ���� ���� TimeStamp Check �� ó��!
				for( n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++) 
				{
					gObjUnionUpdateProc(n);
				}	
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// �߱�����> . �ð����� ����ڿ��� ���� �ð��� ���� ������ �Ѵ�.
	#ifdef MODIFY_PLAYTIME_LIMIT_MESSAGE_ONOFF_20060427
				if( g_bChnPlayTimeLimitOn == TRUE )
	#endif
				{
					INT iTickGap;
					INT iCheckTick;
					for( n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++) 
					{
						if (gObjIsConnected(n))
						{
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
								// ���� ������ ���� ����ڴ� �ٸ� ������ �ʿ� ����.
								if( gObj[n].m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
								{
									PMSG_NOTICE	pNotice;
									CHAR szNotice[256] = {0,};

		#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_02_20080218
									gObj[n].m_iPlayTimeLimit++;
									gObj[n].m_dwPlayTick = GetTickCount();

									if( gObj[n].m_iPlayTimeLimit == ( 60 * 60 ) )			// �ѽð� ����
									{
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										wsprintf(szNotice, lMsg.Get(1260)		// "���� ���� �ð��� �̹� 1�ð��� �Ǿ����ϴ�"
											);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
										TNotice::SendNoticeToUser( n, &pNotice );
									}
									else if( gObj[n].m_iPlayTimeLimit == ( 3 * 60 * 60 ) )	// 3�ð� ����
									{
										// 3�ð� ���
										gObj[n].m_iPlayNoticeTick = GetTickCount();

										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										wsprintf(szNotice, lMsg.Get(1261)		// "���� ���� �ð��� �̹� 3�ð��� �Ǿ����ϴ�....(����)"
											);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
										TNotice::SendNoticeToUser( n, &pNotice );
									}
									else if( gObj[n].m_iPlayTimeLimit == ( 5 * 60 * 60 ) )	// 5�ð� ����
									{
										gObj[n].m_iPlayNoticeTick = GetTickCount();

										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										wsprintf(szNotice, lMsg.Get(1262)		// "����� ���� �ð��� �̹� �������� ���� ������ �Ǿ����ϴ�....(����)"
											);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
										TNotice::SendNoticeToUser( n, &pNotice );
									}

									if( GetTickCount() - gObj[n].m_iPlayNoticeTick >= ( 15 * 60 * 1000 ) )
									{
										gObj[n].m_iPlayNoticeTick = GetTickCount();

										PMSG_NOTICE	pNotice;
										CHAR szNotice[256] = {0,};

										if( gObj[n].m_iPlayTimeLimit >= 5*60*60)
										{
											// 5�ð� ���
											TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
											wsprintf(szNotice, lMsg.Get(1262)		// "����� ���� �ð��� �̹� �������� ���� ������ �Ǿ����ϴ�....(����)"
												);
											TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
											TNotice::SendNoticeToUser( n, &pNotice );
										}
										else if( gObj[n].m_iPlayTimeLimit >= 3*60*60)
										{
											// 3�ð� ���
											TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
											wsprintf(szNotice, lMsg.Get(1261)		// "���� ���� �ð��� �̹� 3�ð��� �Ǿ����ϴ�....(����)"
												);
											TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
											TNotice::SendNoticeToUser( n, &pNotice );
										}
									}
		#else
									gObj[n].m_iPlayTimeLimit += iTickGap;
									gObj[n].m_dwPlayTick = GetTickCount();

									if( gObj[n].m_iPlayTimeLimit >= 5*60*60)		// 5�ð� ����
									{
										iCheckTick = 15*60*1000;					// 15�� ���� �޽��� ����
									}
									else if( gObj[n].m_iPlayTimeLimit >= 3*60*60)	// 3�ð� ����
									{
										iCheckTick = 30*60*1000;					// 30�� ���� �޽��� ����
									}

									if( gObj[n].m_iPlayTimeLimit == ( 60 * 60 ) )			// �ѽð� ����
									{
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										wsprintf(szNotice, lMsg.Get(1260)		// "���� ���� �ð��� �̹� 1�ð��� �Ǿ����ϴ�"
											);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
										TNotice::SendNoticeToUser( n, &pNotice );
									}
									else if( gObj[n].m_iPlayTimeLimit == ( 3 * 60 * 60 ) )	// 3�ð� ����
									{
										// 3�ð� ���
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										wsprintf(szNotice, lMsg.Get(1261)		// "���� ���� �ð��� �̹� 3�ð��� �Ǿ����ϴ�....(����)"
											);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
										TNotice::SendNoticeToUser( n, &pNotice );
									}
									else if( gObj[n].m_iPlayTimeLimit == ( 5 * 60 * 60 ) )	// 5�ð� ����
									{
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										wsprintf(szNotice, lMsg.Get(1262)		// "����� ���� �ð��� �̹� �������� ���� ������ �Ǿ����ϴ�....(����)"
											);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
										TNotice::SendNoticeToUser( n, &pNotice );
									}

									// �߱��߰�> . �ð����� ����ڿ��� ���� �ð��� ���� ������ �Ѵ�.
									if (GetTickCount() - gObj[n].m_iPlayNoticeTick >= iCheckTick)
									{
										gObj[n].m_iPlayNoticeTick = GetTickCount();

										PMSG_NOTICE	pNotice;
										CHAR szNotice[256] = {0,};
										if( gObj[n].m_iPlayTimeLimit >= 5*60*60)
										{
											// 5�ð� ���
											TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
											wsprintf(szNotice, lMsg.Get(1262)		// "����� ���� �ð��� �̹� �������� ���� ������ �Ǿ����ϴ�....(����)"
												);
											TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
											TNotice::SendNoticeToUser( n, &pNotice );
										}
										else if( gObj[n].m_iPlayTimeLimit >= 3*60*60)
										{
											// 3�ð� ���
											TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
											wsprintf(szNotice, lMsg.Get(1261)		// "���� ���� �ð��� �̹� 3�ð��� �Ǿ����ϴ�....(����)"
												);
											TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
											TNotice::SendNoticeToUser( n, &pNotice );
										}
										/*else if( gObj[n].m_iPlayTimeLimit >= 60*60 )
										{
											// 1�ð� ���
											TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
											wsprintf(szNotice, lMsg.Get(1260)		// "���� ���� �ð��� �̹� 1�ð��� �Ǿ����ϴ�"
												);
											TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
											TNotice::SendNoticeToUser( n, &pNotice );
										}*/
									}
		#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_02_20080218
								}
	#else
								gObj[n].m_iPlayTimeLimit += iTickGap;
								gObj[n].m_dwPlayTick = GetTickCount();

								if (gObj[n].m_iPlayTimeLimit >= 5*60*60) {
									iCheckTick = 15*60*1000;
								}
								else if (gObj[n].m_iPlayTimeLimit >= 3*60*60) {
									iCheckTick = 30*60*1000;
								}
								else {
									iCheckTick = 60*60*1000;
								}

								// �߱��߰�> . �ð����� ����ڿ��� ���� �ð��� ���� ������ �Ѵ�.
								if (GetTickCount() - gObj[n].m_iPlayNoticeTick >= iCheckTick)
								{
									gObj[n].m_iPlayNoticeTick = GetTickCount();

									PMSG_NOTICE	pNotice;
									CHAR szNotice[256] = {0,};
									if (gObj[n].m_iPlayTimeLimit >= 5*60*60) {
										// ���ؽð�
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										wsprintf(szNotice, lMsg.Get(1258),		// "���ؽð��Դϴ�. ���ӳ� ���� 0%%. �ܿ� �÷��̽ð�:%02d��%02d��"
											0,
											0
	//										((5*60*60)-gObj[n].m_iPlayTimeLimit)/(60*60),
	//										(((5*60*60)-gObj[n].m_iPlayTimeLimit)%(60*60))/60
											);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
										TNotice::SendNoticeToUser( n, &pNotice );
									}
									else if (gObj[n].m_iPlayTimeLimit >= 3*60*60) {
										// �Ƿνð�
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										wsprintf(szNotice, lMsg.Get(1257),		// "�Ƿνð��Դϴ�. ���ӳ� ���� 50%%. �ܿ� �÷��̽ð�:%02d��%02d��"
											((5*60*60)-gObj[n].m_iPlayTimeLimit)/(60*60),
											(((5*60*60)-gObj[n].m_iPlayTimeLimit)%(60*60))/60
											);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
										TNotice::SendNoticeToUser( n, &pNotice );
									}
									else {
										// ����ð�
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										wsprintf(szNotice, lMsg.Get(1256),		// "�÷��̽ð�:%02d��%02d��, �ܿ� �÷��̽ð�:%02d��%02d��"
											gObj[n].m_iPlayTimeLimit/(60*60),
											(gObj[n].m_iPlayTimeLimit%(60*60))/60,
											((5*60*60)-gObj[n].m_iPlayTimeLimit)/(60*60),
											(((5*60*60)-gObj[n].m_iPlayTimeLimit)%(60*60))/60
											);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNotice);
										TNotice::SendNoticeToUser( n, &pNotice );
									}
								}
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
#if 0 //�߱� ���������� ���� �ȵ� "}" �ϳ��� �߰��Ȱ� ���Ƽ� ������
							}
#endif
						}
					}
				}
#endif
				
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626 // ��Ʈ�� : �Ƿε� ����
				
				if( g_bVtmPlayTimeLimitOn == TRUE )
				{
					INT iTickGap;	// (sec)
					INT iCheckTick;	// (ms)
					for( n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++) 
					{
						if (gObjIsConnected(n)) 
						{
							iTickGap = (GetTickCount() - gObj[n].m_dwPlayTick)/1000;
							
							if (iTickGap >= 1) 
							{
								gObj[n].m_iPlayTimeLimit += iTickGap;
								gObj[n].m_dwPlayTick = GetTickCount();

								// 5 �ð� �̻� (sec)
								if (gObj[n].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time*60) 
								{
									// 15�� ���� ���� (ms)
									iCheckTick = g_iVtmPlayTimeLimitLevel2NoticeTime*60*1000;	
								}
								// 3 �ð� �̻� (sec)
								else if (gObj[n].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time*60) 
								{
									// 30�� ���� ���� (ms)
									iCheckTick = g_iVtmPlayTimeLimitLevel1NoticeTime*60*1000;
								}
								else 
								{
									// 60�� ���� ���� (ms)
									iCheckTick = g_iVtmPlayTimeLimitNoticeTime*60*1000;
								}

								// (ms) �ð����� ����ڿ��� ���� �ð��� ���� ������ �Ѵ�.
								if (GetTickCount() - gObj[n].m_iPlayNoticeTick >= iCheckTick) 
								{
									gObj[n].m_iPlayNoticeTick = GetTickCount();

									PMSG_NOTICE	pNotice;
	
									// (sec)
									if (gObj[n].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time*60 ) 
									{
										// ���ؽð� ���� "�������ʹ� ����ġ �� �������� ������ �� �����ϴ�."
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1273));
										TNotice::SendNoticeToUser( n, &pNotice );
									}
									else if (gObj[n].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time*60) 
									{
										// �Ƿνð� ���� "�������ʹ� ����ġ �� ������ ����� 50%�� ����˴ϴ�."
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1271));
										TNotice::SendNoticeToUser( n, &pNotice );
									}
									else 
									{
										// ����ð� ���� 
										INT iCalcRestTime = (g_iVtmPlayTimeLimitLevel2Time*60)-gObj[n].m_iPlayRestTime;
										TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
										// "���� �÷��� �ð��� %d�� %d���̸�, �ʱ�ȭ���� %d�� %d���� �޽��� �ʿ��մϴ�."
										TNotice::MakeNoticeMsgEx( &pNotice, TNOTICE_PRIORITY_LV1, lMsg.Get(1270), 
											gObj[n].m_iPlayTimeLimit/(60*60), (gObj[n].m_iPlayTimeLimit%(60*60))/60,
											iCalcRestTime/(60*60), (iCalcRestTime%(60*60))/60 );
										TNotice::SendNoticeToUser( n, &pNotice );
									}
								}

							}
						}
					}
				}
#endif // VTM_PLAYTIME_LIMIT_SYSTEM_20060626
				
				gObjSetState();
				
				GameServerInfoSend();
				//LogAdd("Server info send");

#ifdef GAMESERVER_INFO_EXTENDED_20040625	
				GameServerExtInfoSend();	// ���Ӽ����� ���Ӽ��� �߰����� ���� ����
#endif
								
				}
				break;
			case 1000 :

				{
				for( int n=0; n<g_TerrainManager.Size(); n++)
				{
					MapC[n].WeatherVariationProcess();
				}
				wsGServer.m_SendSec = wsGServer.m_SendSecTmp;
				wsGServer.m_RecvSec = wsGServer.m_RecvSecTmp;
				wsGServer.m_SendSecTmp = 0;
				wsGServer.m_RecvSecTmp = 0;	
				DragonEvent->Run();
				AttackEvent->Run();
#ifdef ELEGORADO_EVENT
				gEledoradoEvent.Run();
#endif

#ifdef EVENT_MANAGEMENT_20040711
				g_EventManager.Run();
#endif

				//g_VIPSystem.ProcessVIP();

#ifdef __HAPPYHOUR_H__
				g_HappyHour.Process();
#endif
				}
				break;
			case MTIME_SECOND :
				{				
					srand(time(NULL));
				if( gCloseMsg )
				{					
					gCloseMsgTime--;
					if( gCloseMsgTime <= 1 )
					{						
						if( gCloseMsgTime == 1 )
						{
							AllSendServerMsg(lMsg.Get(449));
							LogAddC(LOGC_RED, lMsg.Get(449));
						}
					}
					else
					{
						if((gCloseMsgTime%10) == 0 )
						{
							char szTemp[256];
							wsprintf(szTemp,lMsg.Get(450), gCloseMsgTime);
							AllSendServerMsg(szTemp);
							LogAddC(LOGC_RED, szTemp);
						}
					}
					if( gCloseMsgTime < 0 )
					{
						KillTimer(ghWnd, MTIME_SECOND);
						gObjAllLogOut();
						gCloseMsgTime = 0;
						gCloseMsg = 0;
#ifdef MODIFY_AUTHSERVER_CLOSE_20041207
						gbAuthServerClosed = FALSE;
#endif
					}
				}
				}
				gObjSecondProc();
#ifdef CASTLE_MAIN_SCHEDULER_20041111
				if (cDBSMng.GetActiveDS() > -1 && g_CastleSiege.GetDataLoadState() == CASTLESIEGE_DATALOAD_2) {
					if (g_CastleSiege.DataRequest()) {
//						g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_3);
					}
				}
				// �ʴ� �ѹ��� ���� ���¸� ����ȭ �Ѵ�.
				g_CastleSiege.SendCastleStateSync();
#endif
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
				g_CastleSiegeSync.AdjustTributeMoney();
#endif
				
#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
				g_Crywolf.CrywolfSecondAct();
#endif				
				break;
			case MTIME_HOUR :
				gSetDate();							// �ѽð��� �ѹ��� ��¥ �����͸� ���� (2003.01.02)
				WhisperCash.TimeCheckCashDelete();	// ��ǻ� �� ���� ����.
				break;
			case MTIME_MIN :						// 10�ʿ� �ѹ��� ���Ӽ������� ���� ����
				cDBSMng.Connect();
#ifndef ADD_NEWPVP_PKFIELD
#ifdef DUEL_SYSTEM_FIX_20031224
				gObjCheckAllUserDuelStop();
#endif
#endif // ADD_NEWPVP_PKFIELD
				break;
			case MTIME_DAY :
				if( LogDateChange() == TRUE )
				{
#ifdef CASTLE_SPECIAL_ITEMMIX_20050425				// �Ϸ簡 ������ ���� ������ Ƚ���� ���󺹱��ȴ�.
					g_iCastleItemMixLimit = CASTLE_ITEM_MIX_MAXCOUNT;
#endif
				}
				break;
			}
			break;
		case WM_CLOSE :
				if( gLanguage != 3 )
				{
					if( MessageBox(ghWnd, "GameServer close?", "Close", MB_YESNO) == IDYES ) 
					{
#ifdef MODIFY_AUTHSERVER_CLOSE_20041207
						if (gbAuthServerClosed == TRUE) {
							SendMessageA(ghWnd, WM_COMMAND, MAKELONG(ID_MENU_ONECLOSE, 0), 0);
						} 
						else {
#endif
						DestroyWindow(hWnd);
#ifdef MODIFY_AUTHSERVER_CLOSE_20041207
						}
#endif
					}
				}
				else
				{
					DestroyWindow(hWnd);
				}
			break;
		case WM_KEYDOWN :
			switch( wParam )
			{
			case VK_F1 :
				gCurPaintPlayer--;
				if( gCurPaintPlayer < 0 ) gCurPaintPlayer = 0;
				break;
			case VK_F2 :
				gCurPaintPlayer++;
				if( gCurPaintPlayer > MAX_OBJECT-1 ) gCurPaintPlayer = MAX_OBJECT-1;
				break;
			case VK_F4 :
				//GameMainServerCreate(WM_ASS_SERVERMSG, WM_ASS_CLIENTMSG);
				//TestDSSend();
				{
				/*PMSG_IDPASS Msg;
				strcpy(Msg.Id,"saemsong");
				strcpy(Msg.Pass,"ssman");	
				Msg.CliVersion[0] = szClientVersion[0];
				Msg.CliVersion[1] = szClientVersion[1];
				Msg.CliVersion[2] = szClientVersion[2];
				CSPJoinIdPassRequest(&Msg, 0);
				*/
				}
				break;
			case VK_F5 : 
				gServerMaxUser-=10;
				if( gServerMaxUser < 10 ) gServerMaxUser = 10;
				break;
			case VK_F6 :
				gServerMaxUser+=10;
				if( gServerMaxUser > 1500 ) gServerMaxUser = 1500;
				break;
			case VK_F7 :
				{
					PMSG_CHARMAPJOIN Msg;
					strcpy(Msg.Name,"�׽�Ʈ");				
					CGPCharacterMapJoinRequest(&Msg, 0);
				}
				break;
			case VK_F9 :				
				#if( TESTSERVER == 1 )
				//{
					TestDSSend();
				/*PMSG_IDPASS pMsg;
				for( int n=0; n<100; n++)
				{
					CSPJoinIdPassRequestTEST(&pMsg, 0);					
				}
				}*/
				#endif
				break;
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
			case VK_F11 :
				DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				break;
#endif				
			}
			break;
		case WM_DESTROY:
			GameMainFree();
			GiocpDelete();
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


#ifdef MU_CASTLESIEGE_LOG_20050107

BOOL CALLBACK CsLogDlgProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch(uMessage) {
	case WM_INITDIALOG :
		SendMessage(hWnd, WM_SIZE, 0, 0);
//		ShowWindow(hWnd, TRUE);
		break;
	case WM_CLOSE :
		ShowWindow(hWnd, FALSE);
		return TRUE;
	case WM_SIZE :
		{
			RECT stRT;
			GetClientRect(hWnd, &stRT);
			MoveWindow(GetDlgItem(hWnd, IDC_MAIN_LIST), stRT.left, stRT.top, stRT.right, stRT.bottom, TRUE);
		}
		return TRUE;
	}

	return FALSE;
}

#endif
