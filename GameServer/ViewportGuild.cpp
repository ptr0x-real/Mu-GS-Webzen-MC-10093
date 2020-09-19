
// 뷰포트 길드 관리 처리
// ViewportGuild.cpp: implementation of the CViewportGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Include\\prodef.h"
#include "user.h"
#include "Protocol.h"
#include "ViewportGuild.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CViewportGuild::CViewportGuild()
{

}

CViewportGuild::~CViewportGuild()
{

}

void CViewportGuild::Init()
{		
	memset(m_GuildNumber, 0, sizeof(m_GuildNumber));
	m_GuildCount = 0;
}

BOOL CViewportGuild::Add(int number, int aIndex)
{
	int blank=-1;
	if( number < 1 ) return FALSE;
	//if( gObj[aIndex].lpGuild == NULL ) return FALSE;

	for( int n=0; n<MAXVIEWPORTOBJECT; n++)
	{
		if( m_GuildNumber[n] > 0 )
		{
			if( m_GuildNumber[n] == number )
			{
				return FALSE;	// 같은 길드 이름이 존재
			}
		}
		else 
		{
			if( blank < 0 ) 
			{
				blank = n;
			}
		}
	}
	if( blank < 0 ) return FALSE;
	
	if( m_GuildNumber[blank] == 0 )
	{
		m_GuildNumber[blank] = number;
		m_GuildCount++;
		return TRUE;
	}
	return FALSE;
}



