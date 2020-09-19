// 
// Database server socket manager
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "User.h"
#include "Gamemain.h"
#include "Gameserver.h"
#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "..\\Common\\winutil.h"
#include "DSprotocol.h"
#include "Protocol.h"
#include "wsJoinServerCli.h"
#include "DBSockMng.h"


extern char	DataServerIp[];
#ifdef ADD_GAMESERVER_PARAMETER_20050728
extern char	DataServerIp2[];
extern char	ExDbIp[];
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DBSockMng	cDBSMng;


#ifdef _NEW_EXDB_
//##EXDB##
extern wsJoinServerCli wsExDbCli;
//##EXDB##
#endif


DBSockMng::DBSockMng()
{
	m_ActiveServer = -1;	
}

DBSockMng::~DBSockMng()
{

}

void DBSockMng::Connect()
{

	if( wsCDBS[0].GetSocket() == INVALID_SOCKET )
	{
#ifdef GS_DS_LIVECHECK_20041008
		iCDBS_CHECK_COUNT[0]	= 0;
#endif
		BOOL result;
		
		wsCDBS[0].CreateSocket(ghWnd);
		wsCDBS[0].SetProtocolCore(DataServerProtocolCore);
		result = wsCDBS[0].Connect(DataServerIp, DataServerPort, WM_ASS_DATACLIMSG);
		if( result == FALSE ) 
		{
			LogAdd(lMsg.Get(403), DataServerIp, DataServerPort);
			wsCDBS[0].Close();
		}
		else
		{
			LogAdd(lMsg.Get(402), DataServerIp, DataServerPort);
			m_ActiveServer = 0;
			DataServerLogin(0);

#ifdef GS_DS_LIVECHECK_20041008
			uCDBS_CHECK_TIME[0]	= GetTickCount();
#endif
		}
	}

	if( wsCDBS[1].GetSocket() == INVALID_SOCKET )
	{
#ifdef GS_DS_LIVECHECK_20041008
		iCDBS_CHECK_COUNT[1]	= FALSE;
#endif
		BOOL result;

		wsCDBS[1].CreateSocket(ghWnd);
		wsCDBS[1].SetProtocolCore(DataServerProtocolCore);
		
#ifdef ADD_GAMESERVER_PARAMETER_20050728
		result = wsCDBS[1].Connect(DataServerIp2, DataServerPort2, WM_ASS_DATACLIMSG+1);
		if( result == FALSE ) 
		{
			LogAdd(lMsg.Get(403), DataServerIp2, DataServerPort2);
			wsCDBS[1].Close();
		}
		else
		{			
			LogAdd(lMsg.Get(402), DataServerIp2, DataServerPort2);
			
			// 활성된 서버가 없었다면..
			if( m_ActiveServer == -1 ) 
			{
				m_ActiveServer = 1;
			}
			DataServerLogin(1);

#ifdef GS_DS_LIVECHECK_20041008
			uCDBS_CHECK_TIME[1]	= GetTickCount();
#endif
		}
#else
		result = wsCDBS[1].Connect(DataServerIp, DataServerPort+2, WM_ASS_DATACLIMSG+1);
		if( result == FALSE ) 
		{
			LogAdd(lMsg.Get(403), DataServerIp, DataServerPort+2);
			wsCDBS[1].Close();
		}
		else
		{			
			LogAdd(lMsg.Get(402), DataServerIp, DataServerPort+2);
			
			// 활성된 서버가 없었다면..
			if( m_ActiveServer == -1 ) 
			{
				m_ActiveServer = 1;
			}
			DataServerLogin(1);

#ifdef GS_DS_LIVECHECK_20041008
			uCDBS_CHECK_TIME[1]	= GetTickCount();
#endif
		}
#endif
	}	
	
#ifdef _NEW_EXDB_
	//##EXDB##
	if( wsExDbCli.GetSocket() == INVALID_SOCKET )
	{
		LogAddC(LOGC_RED,"ExDB Server Check..");
		wsExDbCli.CreateSocket(ghWnd);
#ifdef ADD_GAMESERVER_PARAMETER_20050728
		ExDataServerConnect(ExDbIp, WM_ASS_EXDATACLIMSG);
#else
		ExDataServerConnect(DataServerIp, WM_ASS_EXDATACLIMSG);
#endif
	}
#endif
#ifdef __FRIEND_WORK__
	else 
	{
		if( wsExDbCli.GetConnect() == FALSE )
		{
			LogAddC(LOGC_RED,"ExDB Server Check..");
#ifdef ADD_GAMESERVER_PARAMETER_20050728
			ExDataServerConnect(ExDbIp, WM_ASS_EXDATACLIMSG);
#else
			ExDataServerConnect(DataServerIp, WM_ASS_EXDATACLIMSG);			
#endif
		}
	}
#endif


}

void DBSockMng::ActiveChange()
{
	m_ActiveServer == 0 ? m_ActiveServer = 1 : m_ActiveServer = 0;
	LogAdd("Active DB Change");
}

int DBSockMng::Send(char *buf, int len)
{
	if( m_ActiveServer >= 2 ) return FALSE;
	if( m_ActiveServer < 0 ) return FALSE;

	//LogAdd("Buffer [%x] ", buf[1]);
	if( wsCDBS[m_ActiveServer].DataSend(buf, len) == FALSE )
	{
		ActiveChange();
		wsCDBS[m_ActiveServer].DataSend(buf, len);
	}	
	return TRUE;
}

int DBSockMng::Send(int server, char *buf, int len)
{
	if( server >= 2 ) return FALSE;
	if( server < 0 ) return FALSE;

	if( wsCDBS[server].DataSend(buf, len) == FALSE )
	{
		ActiveChange();
	}
	return TRUE;
}

/*
void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_READ :
		wsDataCli.DataRecv();
		break;
	case FD_WRITE :
		wsDataCli.FDWRITE_MsgDataSend();
		break;
	case FD_CLOSE :
		LogAddC(LOGC_RED,lMsg.Get(406));
		SendMessage(ghWnd, WM_CLOSE, 0,0);
		break;
	}
}
*/

int DBSockMng::MsgProc(int line, WPARAM wParam, LPARAM lParam)
{
	switch( WSAGETSELECTEVENT( lParam ) )
	{
	case FD_READ :
		wsCDBS[line].DataRecv();
		break;
	case FD_WRITE :
		wsCDBS[line].FDWRITE_MsgDataSend();
		break;
	case FD_CLOSE :	
		wsCDBS[line].Close();
		//LogAddC(LOGC_RED,lMsg.Get(406));
		//SendMessage(ghWnd, WM_CLOSE, 0,0);
		break;
	}
	return TRUE;
}


#ifdef GS_DS_LIVECHECK_20041008

void DBSockMng::CheckDBServerLivePerSec()
{
	if( m_ActiveServer >= 2 )	return;
	if( m_ActiveServer < 0 )	return;

	if (GetTickCount() - uCDBS_CHECK_TIME[m_ActiveServer] >= DS_MAX_CHECK_TIME) {
		iCDBS_CHECK_COUNT[m_ActiveServer]++;

		if (iCDBS_CHECK_COUNT[m_ActiveServer] > DS_MAX_CHECK_COUNT) {
			LogAddC(LOGC_RED, "[DS-LiveCheck] DS:%d Live Check Failed",
				m_ActiveServer
				);
//			wsCDBS[m_ActiveServer].Close();
			// m_ActiveServer 를 임의로 바꾸어주면 ActiveChange() 가 제대로 호출이 않된다.
		}
	}

	return;
}


void DBSockMng::ResetActiveServerLiveTime() {			// 현재 활동 중인 DB서버의 활성 시간을 초기화 한다.
	if( m_ActiveServer >= 2 )	return;
	if( m_ActiveServer < 0 )	return;

	uCDBS_CHECK_TIME[m_ActiveServer]	= GetTickCount();
	iCDBS_CHECK_COUNT[m_ActiveServer]	= 0;
}


#endif


