// PacketCheckSum.h: interface for the CPacketCheckSum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETCHECKSUM_H__CEAC2C2A_90BF_43E3_8388_9431ED9ED686__INCLUDED_)
#define AFX_PACKETCHECKSUM_H__CEAC2C2A_90BF_43E3_8388_9431ED9ED686__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\include\\define.h"

#define MAX_PACKETCHECKSUMFUNC_COUNT	22

#if TESTSERVER == 1
#define MAX_PACKETCHECKSUM				1
#else
#define MAX_PACKETCHECKSUM				100
#endif

typedef struct 
{
	DWORD	Check[MAX_PACKETCHECKSUMFUNC_COUNT];	
}PAKETCHECKSUM, * LPPAKETCHECKSUM;

class CPacketCheckSum  
{
	PAKETCHECKSUM	PaketChecksum[MAX_OBJECT];

	int		m_ChecksumTableAllClearState;

	
	int		m_ChecksumTableClearState[MAX_PACKETCHECKSUMFUNC_COUNT];
	int		m_ChecksumTableAVGCount[MAX_PACKETCHECKSUMFUNC_COUNT];
	DWORD	m_ChecksumTableAVG[MAX_PACKETCHECKSUMFUNC_COUNT][100];
	DWORD	m_ChecksumTable[MAX_PACKETCHECKSUMFUNC_COUNT];
public:
	CPacketCheckSum();
	virtual ~CPacketCheckSum();

	void	Init();

	void	Check(int aIndex);

	void	ClearCheckSum(int aIndex);

	BOOL	Add(int aIndex, int funcindex, DWORD checksum);

	void	AddCheckSum(int aIndex, int funcindex, DWORD checksum);
	void	SetClearChecksumFunc(int funcindex);
};

extern CPacketCheckSum gPacketCheckSum;
#endif // !defined(AFX_PACKETCHECKSUM_H__CEAC2C2A_90BF_43E3_8388_9431ED9ED686__INCLUDED_)
