// GameServerAuth.cpp: implementation of the CGameServerAuth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef AUTH_GAMESERVER
#include "GameServerAuth.h"
#include "Gamemain.h"
#include "User.h"

CGameServerAuth gGameServerAuth;
extern int gMonsterHp;
void	GameServerAuthCallBackFunc(int Type, int pData)
{
	if( Type == 0 )
	{	// 옵션을 다시 읽는다
		int ReloadType = pData;
		switch(ReloadType)
		{
			case 1:
				{	// Option reLoad
					ReadCommonServerInfo();
				}
			break;

			case 2:
				{	// Monster reLoad
					GameMonsterAllCloseAndReLoad();
				}
			break;

			case 3:
				{	// Shop reLoad
					ShopDataLoad();
				}
			break;
		}
	}
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameServerAuth::CGameServerAuth()
{
	WZAGSerInfo = NULL;
	WZAGGetClientVersion = NULL;
	WZAGSetSubInfo = NULL;
	m_bHandle = NULL;
	m_iTimer = 0;

	m_bDllHandle = NULL;
}

CGameServerAuth::~CGameServerAuth()
{
	WzAGReleaseHandle(GetHandle());

	if( m_bHandle )
		FreeLibrary(m_bHandle);	
}


void CGameServerAuth::Init()
{	
	if( m_bHandle )
		FreeLibrary(m_bHandle);

	m_bHandle = LoadLibrary ("WzAG.dll");
	if (m_bHandle == NULL)
	{ 	// dll 로드 에러
		return;
	}	

	WZAGSerInfo = (IAGFuncSetInfo)GetProcAddress(m_bHandle, "AGSetInfo");
	WZAGGetClientVersion = (IAGGetClientVersion)GetProcAddress(m_bHandle, "AGGetClientVersion");
	WZAGSetSubInfo = (IAGSetSubInfo)GetProcAddress(m_bHandle , "AGSetSubInfo");
	WZAGGetKey = (IAGGetKey)GetProcAddress(m_bHandle , "AGGetKey");
	WzAGRequestData = (IAGRequestData)GetProcAddress(m_bHandle , "AGRequestData");
	WzAGGetDataBufferSize = (IAGGetDataBufferSize)GetProcAddress(m_bHandle , "AGGetDataBufferSize");
	WzAGGetDataBuffer = (IAGGetDataBuffer)GetProcAddress(m_bHandle , "AGGetDataBuffer");
	WZAGConnectSetFS = (IAGConnectSetFS)GetProcAddress(m_bHandle , "AGConnectSetFS");
	WzAGReleaseHandle = (IAGReleaseHandle)GetProcAddress(m_bHandle , "AGReleaseHandle");
}

void CGameServerAuth::SendInfo()
{
	m_iTimer++;
	if( m_iTimer > 120 )
	{	// 2분에 한번씩... 서버 정보 전송
		m_iTimer = 0;
		if( WZAGSetSubInfo )
		{			
			WZAGSetSubInfo(GetHandle(), gServerMaxUser, gObjTotalUser, gMonsterHp, gItemDropPer, gAddExperience, GetEventFlag());
		}
		
	}
}

void CGameServerAuth::GetKey(char* key, int startindex, int len)
{
	WZAGGetKey(GetHandle(), key, startindex, len);
}

void CGameServerAuth::RequestData(int type)
{
	WzAGRequestData(GetHandle(), type);
}

int CGameServerAuth::GetDataBufferSize()
{
	return WzAGGetDataBufferSize(GetHandle());
}

char* CGameServerAuth::GetDataBuffer()
{
	return WzAGGetDataBuffer(GetHandle());
}

AG_HANDLE CGameServerAuth::SetInfo(int iCountryCode, 
							  BYTE cPartition,
							  short iServerCode,
							  char* szServerVersion,
							  char* szServerName,
							  int ServerType,
							  void callbackfunc(int, int))
{
	m_bDllHandle = WZAGSerInfo(iCountryCode, cPartition, iServerCode, szServerVersion, szServerName, ServerType, callbackfunc);	
	return m_bDllHandle;
}

void CGameServerAuth::GetClientVersion(char* szClientVersion,char* szClientSerial)
{
	WZAGGetClientVersion(GetHandle(), szClientVersion, szClientSerial);
}

#endif