// WhisperCash.cpp: implementation of the CWhisperCash class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "User.h"
#include "WhisperCash.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWhisperCash::CWhisperCash()
{

}

CWhisperCash::~CWhisperCash()
{

}

int CWhisperCash::AddCash(char *id)
{
	int count=0;
	int maxcount=MAXWHISPERCASH;
	int result;
	
	// 캐시에 존재하는 사용자일땐 일단 캐시에서 찾아서 알려주고..
	result = SearchCash(id);
	if( result != -1 ) 
	{
		// 캐시의 사용자의 접속 여부를 체크한다.
		if( gObjUserIdConnectCheck(id, result) == FALSE )
		{
			// 접속 상대가 존재하지 않는다.
			return -1;
		}
		return result;
	}

	// 전체 사용자 중에서 찾는다.
	result = gObjGetIndex(id);
	if( result == -1 ) return -1;	// 사용자가 존재하지 않는다면
	
	// 사용자가 존재하면 일단 캐시에 넣는다.
	count = WChashCount;
	while(maxcount--)
	{
		count ++;
		if( count > MAXWHISPERCASH-1 ) count = 0;

		if( WChash[count].Live == 0 )
		{
			memcpy(WChash[count].szId, id, MAX_IDSTRING);
			WChash[count].Live		= TRUE;
			WChash[count].m_Index   = result;
			WChash[count].m_Time = GetTickCount();
			WChashCount++;
			if( WChashCount > MAXWHISPERCASH-1 ) WChashCount = 0;
			return result;
		}		
	}

	return result;
}

// 캐쉬에서 사용자를 삭제한다.
void CWhisperCash::DelCash(char *id)
{
	int n=0;

	for( n=0; n<MAXWHISPERCASH; n++)
	{
		if( WChash[n].Live == TRUE )
		{
			if( WChash[n].szId[0] == id[0] )	// 첫바이트가 같다면..
			{
				if( strcmp(WChash[n].szId, id) == 0 )	// 찾았다면..
				{
					WChash[n].Live = FALSE;
					return;
				}
			}
		}
	}
}

// 캐쉬에서 사용자가 있는지 체크한다.
int CWhisperCash::SearchCash(char *id)
{
	int n=0;

	for( n=0; n<MAXWHISPERCASH; n++)
	{
		if( WChash[n].Live == TRUE )
		{
			if( WChash[n].szId[0] == id[0] )	// 첫바이트가 같다면..
			{
				if( strcmp(WChash[n].szId, id) == 0 )	// 찾았다면..
				{
					WChash[n].m_Time = GetTickCount();
					return WChash[n].m_Index;
				}
			}
		}
	}
	return -1;
}

// 일정시간이 되면 많이 사용하지 않는 아이디는 삭제한다. 
void CWhisperCash::TimeCheckCashDelete()
{
	int n=0;
	int CurTime = GetTickCount();

	for( n=0; n<MAXWHISPERCASH; n++)
	{
		if( WChash[n].Live == TRUE )
		{
			if( (CurTime-WChash[n].m_Time) > (1000*60) )	// 1분이 지나면 삭제한다.
			{
				WChash[n].Live = FALSE;
			}
		}
	}
}