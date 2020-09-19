// ConMember.cpp: implementation of the CConMember class.
// 立加 啊瓷茄 蜡历 府胶飘 包府.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConMember.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "..\\Include\\Readscript.h"
CConMember	ConMember;

CConMember::CConMember()
{	
}

CConMember::~CConMember()
{
}

void CConMember::Init()
{
#ifdef CONNECT_MEMBER_COUNT_INFINITE_20040823
	m_szAccount.clear();
#else
	for( int n=0; n<MAX_CONNECTMEMBER; n++)
	{
		szAccount[n][0] = '\0';
	}
#endif	
}

BOOL CConMember::IsMember(char *AccountID)
{
#ifdef CONNECT_MEMBER_COUNT_INFINITE_20040823
	if (AccountID == NULL || !strcmp(AccountID, ""))
		return FALSE;

	map <string, int>::iterator it = m_szAccount.find(string(AccountID));
	if (it != m_szAccount.end())
		return TRUE;	
#else
	for( int n=0; n<MAX_CONNECTMEMBER; n++)
	{
		if( szAccount[n][0] != '\0' )
		{
			if( strcmp(szAccount[n], AccountID) == 0 )
			{
				return TRUE;
			}
		}
	}
#endif
	return FALSE;
}

void CConMember::Load(char *filename)
{
	int count=0;

	Init();

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
#ifdef CONNECT_MEMBER_COUNT_INFINITE_20040823
			m_szAccount.insert(pair<string, int> (string(TokenString), 0));
#else

			strcpy(szAccount[count], TokenString);
			count++;
			if( count == MAX_CONNECTMEMBER )
			{
				MsgBox("Connect member overflow %s (Max: %d)", filename, MAX_CONNECTMEMBER);
				break;
			}
#endif
		}
	}
	fclose(SMDFile);
}
