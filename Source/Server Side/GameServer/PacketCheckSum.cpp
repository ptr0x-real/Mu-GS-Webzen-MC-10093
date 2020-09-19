// PacketCheckSum.cpp: implementation of the CPacketCheckSum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketCheckSum.h"

#include "Include\Public.h"
#include "Include\ProDef.h"
#include "Include\SproDef.h"
#include "Include\GMProtocol.h"
#include "Common\Winutil.h"
#include "Common\zzzItem.h"
#include "Common\Classdef.h"
#include "Include\ReadScript.h"

#include "user.h"
#include "protocol.h"
#include "Gamemain.h"

#include "SendHackLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPacketCheckSum gPacketCheckSum;

CPacketCheckSum::CPacketCheckSum()
{
	Init();
}

CPacketCheckSum::~CPacketCheckSum()
{

}

void CPacketCheckSum::Init()
{	
	m_ChecksumTableAllClearState = 0;
	memset(m_ChecksumTableClearState, 0, sizeof(m_ChecksumTableClearState));
	memset(m_ChecksumTableAVGCount, 0, sizeof(m_ChecksumTableAVGCount));
	memset(m_ChecksumTableAVG, 0, sizeof(m_ChecksumTableAVG));

	memset(PaketChecksum, 0, sizeof(PaketChecksum));
}

void CPacketCheckSum::Check(int aIndex)
{
	if( gObj[aIndex].m_InWebzen )
	{
		return;
	}

	for( int i = 0; i < MAX_PACKETCHECKSUMFUNC_COUNT; i++ )
	{
		if( PaketChecksum[aIndex].Check[i] == 0 )
		{	// 패킷이 안들어 왔다
#if TESTSERVER == 1
			GCServerMsgStringSend("패킷 CRC에러:3분에 한번씩하는거", aIndex, 1);
#endif			
			LogAddTD("PacketCheckSum Error [%d][%s][%s] %d", aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name, i);
#ifdef HACK_LOG	
			char msg[255];
			wsprintf(msg, "PacketCheckSum Error %d", i);
			gSendHackLog.Send(aIndex, 1, msg);
#endif
			ClearCheckSum(aIndex);
			
			if( gDisconnectHackUser )
			{
				gObjCloseSet(aIndex, CL_EXIT);	
			}
			return;
		}
	}
	LogAddTD("Check PacketCheckSum [%d][%s][%s]", aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name);
	ClearCheckSum(aIndex);
}

void CPacketCheckSum::ClearCheckSum(int aIndex)
{
	memset(&PaketChecksum[aIndex], 0, sizeof(PAKETCHECKSUM));
}

BOOL CPacketCheckSum::Add(int aIndex, int funcindex, DWORD checksum)
{
	if( gObj[aIndex].m_InWebzen )
	{
		return TRUE;
	}

	if( !m_ChecksumTableAllClearState )
	{
		PaketChecksum[aIndex].Check[funcindex] = checksum;
		return TRUE;
	}

	if( m_ChecksumTable[funcindex] == checksum )
	{
#if TESTSERVER == 1
		LogAddTD("체크섬 들어 왔다 [%d][%s][%s] %d,%d", 
				 aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name,funcindex, checksum);
#endif
		PaketChecksum[aIndex].Check[funcindex] = checksum;
	}
	else
	{
#if TESTSERVER == 1
		GCServerMsgStringSend("패킷 CRC에러:주기적", aIndex, 1);	
#endif
		LogAddTD("PacketCheckSum : Compare Fail : [%d][%s][%s] (%d,%u)", 
				 aIndex, gObj[aIndex].AccountID, gObj[aIndex].Name,
				 funcindex, checksum);

#ifdef HACK_LOG	
		char msg[255];
		wsprintf(msg, "PacketCheckSum : Compare Fail : (%d,%u)", funcindex, checksum);
		gSendHackLog.Send(aIndex, 1, msg);

		if( gDisconnectHackUser )
		{
			gObjCloseSet(aIndex, CL_EXIT);	
		}
#endif
	}
	return TRUE;
}

void CPacketCheckSum::AddCheckSum(int aIndex, int funcindex, DWORD checksum)
{
	if( funcindex < 0 || funcindex >= MAX_PACKETCHECKSUMFUNC_COUNT)
	{
		LogAddTD("PacketCheckSum : Invalid FuncIndex : %d, %d", aIndex, funcindex);
		return;
	}

	Add(aIndex, funcindex, checksum);

	if( m_ChecksumTableAllClearState )
	{	// 모든 체크섬 정보가 다 모였다
		
	}
	else if( m_ChecksumTableClearState[funcindex] )
	{	// 체크섬 정보가 다 모였다
	}
	else
	{
		m_ChecksumTableAVG[funcindex][m_ChecksumTableAVGCount[funcindex]%100] = checksum;
		m_ChecksumTableAVGCount[funcindex]++;

		LogAddTD("Insert PacketCheckSum %d, %d,%d", funcindex, m_ChecksumTableAVGCount[funcindex], checksum);

		if( m_ChecksumTableAVGCount[funcindex] >= MAX_PACKETCHECKSUM )
		{			
			SetClearChecksumFunc(funcindex);
		}
	}
}

void CPacketCheckSum::SetClearChecksumFunc(int funcindex)
{
	int iCount = 0;

	DWORD Checksum = m_ChecksumTableAVG[funcindex][0];
	for( int i = 0; i < MAX_PACKETCHECKSUM; i++ )
	{
		if( m_ChecksumTableAVG[funcindex][i] == Checksum )
		{
			iCount++;
		}	
	}	

	if( iCount >= MAX_PACKETCHECKSUM-1 )
	{
		m_ChecksumTableClearState[funcindex] = TRUE;
		m_ChecksumTableAllClearState = TRUE;
		int i;
		for( i = 0; i < MAX_PACKETCHECKSUMFUNC_COUNT; i++)
		{
			if( !m_ChecksumTableClearState[i] )
			{
				m_ChecksumTableAllClearState = FALSE;				
			}
			m_ChecksumTable[i] = m_ChecksumTableAVG[i][0];
		}

		if( m_ChecksumTableAllClearState )
		{
			LogAddTD("Complete Init Checksum");

			for( i = 0; i < MAX_PACKETCHECKSUMFUNC_COUNT; i++)
			{
				m_ChecksumTable[i] = m_ChecksumTableAVG[i][0];

				LogAddTD("%d, %d", i, m_ChecksumTable[i]);
			}
		}
	}
}