// AcceptIp.h: interface for the CAcceptIp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCEPTIP_H__56D3A82A_11C6_462D_BBED_C483233D1A60__INCLUDED_)
#define AFX_ACCEPTIP_H__56D3A82A_11C6_462D_BBED_C483233D1A60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXACCEPTIP		100



class CAcceptIp  
{
public:

	BYTE UseIp[MAXACCEPTIP];
	char szIp[MAXACCEPTIP][16];
	
	CAcceptIp();
	virtual ~CAcceptIp();


	void Init();
	void Load(char* filename);
	BOOL IsIp(char * ip);
};

extern CAcceptIp	acceptIP;

#endif // !defined(AFX_ACCEPTIP_H__56D3A82A_11C6_462D_BBED_C483233D1A60__INCLUDED_)
