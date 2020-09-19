// HitC.cpp: implementation of the CHitC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HitC.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHitC::CHitC()
{
	
}

CHitC::~CHitC()
{

}

void CHitC::Init()
{
	for( int n=0; n<MAX_HITUSER; n++)
	{
		m_UserNumber[n] = -1;
		m_DBNumber[n]   = 0;
		m_HitCount[n]   = 0;
	}
	m_Count = 0;
}

BOOL CHitC::Add(int aIndex, int DbNumber)
{
	for( int n=0; n<MAX_HITUSER; n++)
	{
		if( m_UserNumber[n] < 0 )
		{
			m_UserNumber[n] = aIndex;
			m_DBNumber[n]   = DbNumber;
			m_HitCount[n]++;
			m_Count++;
			return TRUE;
		}
	}
	return FALSE;
}

