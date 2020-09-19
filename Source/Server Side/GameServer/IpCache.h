// IpCache.h: interface for the CIpCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPCACHE_H__4A1A4EAE_8421_4FC0_8E70_2535B15E9EBA__INCLUDED_)
#define AFX_IPCACHE_H__4A1A4EAE_8421_4FC0_8E70_2535B15E9EBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef struct
{
	char  IpAddress[16];
	BOOL  IpUsed;
	DWORD Time;
} IPINFO;

#define MAX_IPINFO		2000

class CIpCache  
{
	IPINFO	m_IpInfo[MAX_IPINFO];
	
public:
	void DelIp(char *ip);
	int AddIp(char *ip);
	void Init();
	CIpCache();
	virtual ~CIpCache();

};

extern CIpCache	IpCache;

#endif // !defined(AFX_IPCACHE_H__4A1A4EAE_8421_4FC0_8E70_2535B15E9EBA__INCLUDED_)
