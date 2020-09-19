// MultiCheckSum.cpp: implementation of the CMultiCheckSum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "..\\Common\\winutil.h"
#include "..\\Include\\GMProtocol.h"
#include "..\\Include\\Public.h"
#include "..\\Include\\cb_protocol.h"
#include "User.h"

#include "MultiCheckSum.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMultiCheckSum	g_MultiChecksum;

CMultiCheckSum::CMultiCheckSum()
{	
	Init();
}

CMultiCheckSum::~CMultiCheckSum()
{

}

void CMultiCheckSum::Init()
{
	for( int i = 0; i <	MAX_CHECKSUM_FILE_COUNT; i++ )
	{
		m_dwLoaded[i] = FALSE;
	}	

	m_sFilePath[0] = NULL;
}

void CMultiCheckSum::SetFilePath(char* filepath)
{
	strcpy(m_sFilePath, filepath);
}


BOOL CMultiCheckSum::LoadFile()
{	
	char filenames[255];	

	for( int i = 0; i <	MAX_CHECKSUM_FILE_COUNT; i++ )
	{
		if( i == 0 )
		{
			wsprintf(filenames, "%s\\CheckSum.dat", m_sFilePath);
		}
		else
		{
			wsprintf(filenames, "%s\\CheckSum%d.dat", m_sFilePath, i);
		}		
		
		if( LoadFile(filenames, i) )
		{
			m_dwLoaded[i] = TRUE;
			LogAddTD("Load Checksum file : %s", filenames);
		}		
	}	
	return TRUE;
}

BOOL CMultiCheckSum::LoadFile(char* filename, int index)
{
	FILE * fp;

	fp = fopen(filename, "rb");
	if( fp == NULL )
	{		
		return FALSE;
	}
	fread( m_dwgCheckSum[index], 1024 * sizeof ( DWORD), 1, fp);
	fclose(fp);
	return TRUE;
}

BOOL CMultiCheckSum::CompareCheckSum(int aIndex, DWORD TableNum, DWORD Key)
{
	for( int i = 0; i <	MAX_CHECKSUM_FILE_COUNT; i++ )
	{
		if( m_dwLoaded[i] )
		{
			if( m_dwgCheckSum[i][TableNum] == Key )
			{
				LogAddTD("CheckSum%d-Exe Compare Success [%s]", i, gObj[aIndex].AccountID);
				return TRUE;
			}
			else
			{
				LogAddTD("CheckSum%d-Exe Compare Fail %d %d %d %d [%s]", 
					     i,
						 m_dwgCheckSum[i][TableNum], 
						 Key, 
						 TableNum, 
					     gObj[aIndex].AccountID);
			}
		}		
	}
	return FALSE;
}
