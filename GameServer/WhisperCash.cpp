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
	
	// ĳ�ÿ� �����ϴ� ������϶� �ϴ� ĳ�ÿ��� ã�Ƽ� �˷��ְ�..
	result = SearchCash(id);
	if( result != -1 ) 
	{
		// ĳ���� ������� ���� ���θ� üũ�Ѵ�.
		if( gObjUserIdConnectCheck(id, result) == FALSE )
		{
			// ���� ��밡 �������� �ʴ´�.
			return -1;
		}
		return result;
	}

	// ��ü ����� �߿��� ã�´�.
	result = gObjGetIndex(id);
	if( result == -1 ) return -1;	// ����ڰ� �������� �ʴ´ٸ�
	
	// ����ڰ� �����ϸ� �ϴ� ĳ�ÿ� �ִ´�.
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

// ĳ������ ����ڸ� �����Ѵ�.
void CWhisperCash::DelCash(char *id)
{
	int n=0;

	for( n=0; n<MAXWHISPERCASH; n++)
	{
		if( WChash[n].Live == TRUE )
		{
			if( WChash[n].szId[0] == id[0] )	// ù����Ʈ�� ���ٸ�..
			{
				if( strcmp(WChash[n].szId, id) == 0 )	// ã�Ҵٸ�..
				{
					WChash[n].Live = FALSE;
					return;
				}
			}
		}
	}
}

// ĳ������ ����ڰ� �ִ��� üũ�Ѵ�.
int CWhisperCash::SearchCash(char *id)
{
	int n=0;

	for( n=0; n<MAXWHISPERCASH; n++)
	{
		if( WChash[n].Live == TRUE )
		{
			if( WChash[n].szId[0] == id[0] )	// ù����Ʈ�� ���ٸ�..
			{
				if( strcmp(WChash[n].szId, id) == 0 )	// ã�Ҵٸ�..
				{
					WChash[n].m_Time = GetTickCount();
					return WChash[n].m_Index;
				}
			}
		}
	}
	return -1;
}

// �����ð��� �Ǹ� ���� ������� �ʴ� ���̵�� �����Ѵ�. 
void CWhisperCash::TimeCheckCashDelete()
{
	int n=0;
	int CurTime = GetTickCount();

	for( n=0; n<MAXWHISPERCASH; n++)
	{
		if( WChash[n].Live == TRUE )
		{
			if( (CurTime-WChash[n].m_Time) > (1000*60) )	// 1���� ������ �����Ѵ�.
			{
				WChash[n].Live = FALSE;
			}
		}
	}
}