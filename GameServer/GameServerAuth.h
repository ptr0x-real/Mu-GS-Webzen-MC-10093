// GameServerAuth.h: interface for the CGameServerAuth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESERVERAUTH_H__E0355714_5A90_45BF_B651_8A5AE81C4377__INCLUDED_)
#define AFX_GAMESERVERAUTH_H__E0355714_5A90_45BF_B651_8A5AE81C4377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef AUTH_GAMESERVER
#include "..\\include\\WzSvMng_Interface.h"


//WZSVMNG_DLLINTERFACE BOOL WZSvMngFuncSetInfo(int iCountryCode, 
//											 char* szServerVersion,
//											 char* szServerName,											 
//											 int ServerType, 
//											 void callbackfunc(int, void*));
//WZSVMNG_DLLINTERFACE void WZSvMngGetClientVersion(char* szClientVersion,char* szClientSerial);
//WZSVMNG_DLLINTERFACE void WZSvMngSetSubInfo(int MaxUser, int CurUser, int MonsterHP, int ItemDropRate, float AddExp, int EventFlag);

class CGameServerAuth  
{
	HMODULE						m_bHandle;
	int							m_iTimer;	

	AG_HANDLE					m_bDllHandle;
	IAGFuncSetInfo				WZAGSerInfo;
	IAGGetClientVersion			WZAGGetClientVersion;
	IAGSetSubInfo				WZAGSetSubInfo;
	IAGGetKey					WZAGGetKey;
	IAGRequestData				WzAGRequestData;
	IAGGetDataBufferSize		WzAGGetDataBufferSize;
	IAGGetDataBuffer			WzAGGetDataBuffer;
	IAGConnectSetFS				WZAGConnectSetFS;
	IAGReleaseHandle			WzAGReleaseHandle;
public:
	CGameServerAuth();
	virtual ~CGameServerAuth();

	// dll �ʱ�ȭ
	void		Init();	

	void		SetHandle(AG_HANDLE pHandle){m_bDllHandle=pHandle;}
	AG_HANDLE	GetHandle(){return m_bDllHandle;}
	// ���Ӽ��� ������ �����ش�
	void		SendInfo();
	// Ű�� �д´�
	void		GetKey(char* key, int startindex, int len);

	// ������ �����͸� ��û�Ѵ�
	void		RequestData(int type);

	// �������� ���� �������� ũ�⸦ ��û�Ѵ�
	int			GetDataBufferSize();

	// �������� ���� ������ ���۸� ��û�Ѵ�
	char*		GetDataBuffer();
	
	AG_HANDLE	SetInfo(int, BYTE, short, char*, char*, int, void callbackfunc(int, int));	
	void		GetClientVersion(char* szClientVersion,char* szClientSerial);
};

extern CGameServerAuth gGameServerAuth;
extern void	GameServerAuthCallBackFunc(int Type, int pData);
#endif

#endif // !defined(AFX_GAMESERVERAUTH_H__E0355714_5A90_45BF_B651_8A5AE81C4377__INCLUDED_)
