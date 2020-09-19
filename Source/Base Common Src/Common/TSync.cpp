#include "Stdafx.h"
#include "TSync.h"

TSync::TSync() : m_nLock(0)
{
	InitializeCriticalSection( &m_cs );
}

TSync::~TSync()
{
	DeleteCriticalSection( &m_cs );

} 

VOID TSync::Lock()
{
	EnterCriticalSection( &m_cs );
	InterlockedIncrement( &m_nLock );
} 

VOID TSync::Unlock()
{
	LONG nResult = InterlockedDecrement( &m_nLock );
	assert( nResult >=0 && "m_nLock Count Err" );
	LeaveCriticalSection( &m_cs );
} 