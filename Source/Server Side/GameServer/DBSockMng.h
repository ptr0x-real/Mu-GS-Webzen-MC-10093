// DBSockMng.h: interface for the DBSockMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBSOCKMNG_H__761A317E_DBD5_4A5B_8203_69588E3991A1__INCLUDED_)
#define AFX_DBSOCKMNG_H__761A317E_DBD5_4A5B_8203_69588E3991A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wsJoinServerCli.h"

#ifdef GS_DS_LIVECHECK_20041008
#define DS_MAX_CHECK_TIME			(30*1000)	// 체크를 시작할 시간
#define DS_MAX_CHECK_COUNT			(30)		// 최대 체크횟수
#endif

class DBSockMng  
{
public:
	DBSockMng();
	virtual ~DBSockMng();

	wsJoinServerCli wsCDBS[2];
	
#ifdef GS_DS_LIVECHECK_20041008
	BOOL			iCDBS_CHECK_COUNT[2];		// 각 DB서버와의 연결이 살아있는지 확인하는 개수
	DWORD			uCDBS_CHECK_TIME[2];		// 각 DB서버와 마지막 LIVE 신호를 받은 시간
#endif

	int  m_ActiveServer;			// 현재 활성화된 서버는?

	void	Connect();
	void	ActiveChange();
	int		Send(char *buf, int len);
	int		Send(int server, char *buf, int len);
	int		MsgProc(int line, WPARAM wParam, LPARAM lParam);

#ifdef GS_DS_LIVECHECK_20041008
	void	CheckDBServerLivePerSec();			// 1초 간격으로 활성화된 DB서버가 살아있는지 체크
	void	ResetActiveServerLiveTime();		// 현재 활동 중인 DB서버의 활성 시간을 초기화 한다.
#endif
	int		GetActiveDS()		{	return m_ActiveServer;	}
};

extern DBSockMng	cDBSMng;

#endif // !defined(AFX_DBSOCKMNG_H__761A317E_DBD5_4A5B_8203_69588E3991A1__INCLUDED_)
