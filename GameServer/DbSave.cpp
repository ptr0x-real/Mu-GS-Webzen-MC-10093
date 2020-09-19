// DbSave.cpp: implementation of the CDbSave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wzqueue.h"
#include "DbSave.h"
#include "WsJoinServerCli.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern wsJoinServerCli  wsDataCli;

static DWORD WINAPI cSaveThreadProc( CDbSave *pThis );

CDbSave::CDbSave()
{
	WzQSave = NULL;
	m_bIsRunning = FALSE;
	Initialize();
}

CDbSave::~CDbSave()
{
	End();
	Feee();
}

BOOL CDbSave::Initialize()
{
	WzQSave = new WZQueue;
	if( WzQSave == NULL )
	{
		return FALSE;
	}
	
	InitializeCriticalSection(&criti);
	return TRUE;
}

BOOL CDbSave::Feee()
{
	if( WzQSave != NULL )
	{
		delete WzQSave;
		WzQSave = NULL;
	}
	DeleteCriticalSection(&criti);
	return TRUE;
}

BOOL CDbSave::Add(LPBYTE pObject, int nSize, BYTE headcode, int index )
{
	int bRet;

	EnterCriticalSection(&criti);
	bRet = WzQSave->AddToQueue(pObject, nSize, headcode, index);
	LeaveCriticalSection(&criti);
	return bRet;
}

static DWORD WINAPI cSaveThreadProc( CDbSave *pThis )
{
  return pThis->ThreadProc();
}

BOOL CDbSave::Begin()
{
	if( m_ThreadHandle )
	{
		End();
	}
	m_bIsRunning = 1;
	m_ThreadHandle = CreateThread( NULL,
                                 0,
                                 (LPTHREAD_START_ROUTINE)cSaveThreadProc,
                                 this,
                                 0,
                                 (LPDWORD)&m_dwThreadID );

	if( m_ThreadHandle == NULL )
	{
		MsgBox(lMsg.Get(113), __FILE__, __LINE__);
		return FALSE;
	}
	return TRUE;
}

void CDbSave::End()
{
	if( m_ThreadHandle != NULL )
	{
		m_bIsRunning = false;
		WaitForSingleObject( m_ThreadHandle, INFINITE );
		CloseHandle( m_ThreadHandle );
		m_ThreadHandle = NULL;
	}
}

DWORD CDbSave::ThreadProc()
{
	int count;
	BYTE RecvData[5000];
	unsigned int	 nSize;
	BYTE headcode;
	int  uindex;

	while(1)
	{	
		EnterCriticalSection(&criti);
		count = WzQSave->GetCount();
		if( count )
		{
			if( WzQSave->GetFromQueue((LPBYTE)RecvData, &nSize, &headcode, &uindex) == TRUE )
			{
				if( wsDataCli.DataSend((char*)RecvData, nSize) == FALSE )
				{
					LogAdd(lMsg.Get(445),count, uindex);
				}
				else LogAdd(lMsg.Get(446),count, uindex);
			}
		}		
		if( m_bIsRunning == false && count == 0 )
		{
			LeaveCriticalSection(&criti);
			break;
		}
		LeaveCriticalSection(&criti);
		WaitForSingleObject( m_ThreadHandle, 300 );
	}
	return 0;
}
