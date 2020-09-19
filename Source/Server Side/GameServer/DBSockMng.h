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
#define DS_MAX_CHECK_TIME			(30*1000)	// üũ�� ������ �ð�
#define DS_MAX_CHECK_COUNT			(30)		// �ִ� üũȽ��
#endif

class DBSockMng  
{
public:
	DBSockMng();
	virtual ~DBSockMng();

	wsJoinServerCli wsCDBS[2];
	
#ifdef GS_DS_LIVECHECK_20041008
	BOOL			iCDBS_CHECK_COUNT[2];		// �� DB�������� ������ ����ִ��� Ȯ���ϴ� ����
	DWORD			uCDBS_CHECK_TIME[2];		// �� DB������ ������ LIVE ��ȣ�� ���� �ð�
#endif

	int  m_ActiveServer;			// ���� Ȱ��ȭ�� ������?

	void	Connect();
	void	ActiveChange();
	int		Send(char *buf, int len);
	int		Send(int server, char *buf, int len);
	int		MsgProc(int line, WPARAM wParam, LPARAM lParam);

#ifdef GS_DS_LIVECHECK_20041008
	void	CheckDBServerLivePerSec();			// 1�� �������� Ȱ��ȭ�� DB������ ����ִ��� üũ
	void	ResetActiveServerLiveTime();		// ���� Ȱ�� ���� DB������ Ȱ�� �ð��� �ʱ�ȭ �Ѵ�.
#endif
	int		GetActiveDS()		{	return m_ActiveServer;	}
};

extern DBSockMng	cDBSMng;

#endif // !defined(AFX_DBSOCKMNG_H__761A317E_DBD5_4A5B_8203_69588E3991A1__INCLUDED_)
