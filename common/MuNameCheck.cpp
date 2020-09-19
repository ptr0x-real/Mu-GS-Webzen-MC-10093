//////////////////////////////////////////////////////////////////////
// MuNameCheck.cpp: 
// �¿��� ���Ǵ� �̸��� üũ�ؼ� DB���� ĳ���� �̸��̳� ������
// � ������ �ʵ��� �Ѵ�.
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "MuNameCheck.h"
#include "..\\Include\\ReadScript.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMuNameCheck::CMuNameCheck()
{
	m_nMuNameCount = 0;
}

CMuNameCheck::~CMuNameCheck()
{

}

BOOL CMuNameCheck::NameAdd(char * sz_name)
{
	if( strlen(sz_name) < 1 ) return FALSE;

	memcpy(m_szName[m_nMuNameCount], sz_name, MAX_MUNAME_STRING);
	m_szName[m_nMuNameCount][MAX_MUNAME_STRING-1] = '\0';
	
	if( m_nMuNameCount < MAX_MUNAME-1 ) m_nMuNameCount++;
	else 
	{
		MsgBox("Error : System Name Overflow %d", m_nMuNameCount);
		return FALSE;
	}
	return TRUE;
}

BOOL CMuNameCheck::CmpString( char *cmp_name )
{
	if( strlen( cmp_name ) < 1 )
		return FALSE;

	for( int n=0; n<m_nMuNameCount; n++)
	{
		if( m_szName[n][0] == cmp_name[0] && m_szName[n][1] == cmp_name[1] )
		{
			if( strcmp(m_szName[n], cmp_name) == 0 ) 
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

// Item.txt ���� �̸��� �о�´�.
BOOL CMuNameCheck::NameLoad(char * filename)
{
	if((SMDFile=fopen(filename,"r")) == NULL)	
		return FALSE;

	SMDToken Token;
	while(true)
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			int Type = (int)TokenNumber;
			while(true)
			{
				Token = (*GetToken)();
				int Index = (int)TokenNumber;
				if(Token==NAME && strcmp("end",TokenString)==NULL) break;
				else if ( Token == NAME )
				{
					NameAdd(TokenString);					
				}
			}
		}		
	}
	fclose(SMDFile);
	return TRUE;
}
