// MultiCheckSum.h: interface for the CMultiCheckSum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTICHECKSUM_H__0991BF87_A17D_4BE1_8BDB_E399B08A9ECB__INCLUDED_)
#define AFX_MULTICHECKSUM_H__0991BF87_A17D_4BE1_8BDB_E399B08A9ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_CHECKSUM_FILE_COUNT		10

class CMultiCheckSum  
{
	BOOL	m_dwLoaded[MAX_CHECKSUM_FILE_COUNT];
	DWORD	m_dwgCheckSum[MAX_CHECKSUM_FILE_COUNT][1024];

	char	m_sFilePath[255];

private:
	BOOL	LoadFile(char* filename, int index);
public:
	CMultiCheckSum();
	virtual ~CMultiCheckSum();

	void	Init();

	void	SetFilePath(char* filepath);
	BOOL	LoadFile();	

	BOOL	CompareCheckSum(int aIndex, DWORD TableNum, DWORD Key);
};

extern CMultiCheckSum	g_MultiChecksum;

#endif // !defined(AFX_MULTICHECKSUM_H__0991BF87_A17D_4BE1_8BDB_E399B08A9ECB__INCLUDED_)
