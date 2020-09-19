// AcceptIp.cpp: implementation of the CAcceptIp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcceptIp.h"
#include "..\\Include\\Readscript.h"

CAcceptIp	acceptIP;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcceptIp::CAcceptIp()
{
	Init();
}

CAcceptIp::~CAcceptIp()
{

}

void CAcceptIp::Init()
{
	for( int n=0; n<MAXACCEPTIP; n++)
	{
		memset( &szIp[n], 0, 16);
		UseIp[n] = 0;
	}
}

BOOL CAcceptIp::IsIp(char * ip)
{
	for( int n=0; n<MAXACCEPTIP; n++)
	{
		if( UseIp[n] )
		{
			if( strcmp(szIp[n], ip) == 0 )
			{
				return TRUE;	// 존재하면 
			}
		}
	}
	return FALSE;
}

void CAcceptIp::Load(char* filename)
{
	int count=0;

	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox(lMsg.Get(112), filename);
		return ;
	}
	SMDToken Token;
	
	int n=0;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NAME )
		{
			strcpy(szIp[count], TokenString);			
			UseIp[count] = 1;
			count++;
		}
	}
	fclose(SMDFile);
	LogAdd(lMsg.Get(420), count);
}
