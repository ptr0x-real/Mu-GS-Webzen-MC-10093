// IpCache.cpp: implementation of the CIpCache class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IpCache.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIpCache	IpCache;

CIpCache::CIpCache()
{
	Init();
}

CIpCache::~CIpCache()
{

}

void CIpCache::Init()
{
	int n;
	
	for( n=0; n<MAX_IPINFO; n++)
	{
		m_IpInfo[n].IpUsed = 0;
	}
}


int CIpCache::AddIp(char *ip)
{
	int n;
	int blank=-1;
	
	for( n=0; n<MAX_IPINFO; n++)
	{
		if( m_IpInfo[n].IpUsed )
		{
			if( GetTickCount() > m_IpInfo[n].Time )	// 시간이 다되면 캐쉬에서 없앤다.
			{
				m_IpInfo[n].IpUsed = 0;
				if( blank < 0 ) blank = n;
			}
			else
			{
				if( m_IpInfo[n].IpAddress[0] == ip[0] )
				{
					if( m_IpInfo[n].IpAddress[4] == ip[4] )
					{
						if( strcmp(m_IpInfo[n].IpAddress, ip) == 0 )	// 같은 IP가 있다면.
						{
							return FALSE;
						}
					}
				}
			}
		}
		else
		{
			if( blank < 0 ) blank = n;
		}
	}
	if( blank >= 0 )
	{
		strcpy(m_IpInfo[blank].IpAddress, ip);
		m_IpInfo[blank].IpUsed = 1;
		m_IpInfo[blank].Time   = GetTickCount()+(30*1000);	// 1초후에 
		return TRUE;
	}

	return FALSE;
}

void CIpCache::DelIp(char *ip)
{
	int n;
	
	for( n=0; n<MAX_IPINFO; n++)
	{
		if( m_IpInfo[n].IpUsed )
		{
			if( m_IpInfo[n].IpAddress[0] == ip[0] )
			{
				if( m_IpInfo[n].IpAddress[4] == ip[4] )
				{
					if( strcmp(m_IpInfo[n].IpAddress, ip) == 0 )	// 같은 IP가 있다면.
					{
						m_IpInfo[n].IpUsed = 0;
					}
				}
			}
		}
	}
}
