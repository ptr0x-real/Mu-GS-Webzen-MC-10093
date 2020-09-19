// StreamPacketEngine.h: interface for the CStreamPacketEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMPACKETENGINE_H__C0F4554E_77C8_4E5A_A27E_10A6E10A1364__INCLUDED_)
#define AFX_STREAMPACKETENGINE_H__C0F4554E_77C8_4E5A_A27E_10A6E10A1364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <assert.h>


#define MAX_SPE_BUFFERSIZE	( 2048 )
#define SIZE_SPE_XORFILTER	( 32)


class CStreamPacketEngine_Server
{
public:
	CStreamPacketEngine_Server()
	{
		Clear();
	}
	virtual ~CStreamPacketEngine_Server()	{}
	void Clear( void)
	{
		m_wSize = 0;
	}

protected:
	WORD m_wSize;
	BYTE m_byBuffer[MAX_SPE_BUFFERSIZE];

protected:
	void XorData( int iStart, int iEnd, int iDir = 1)
	{
		if( iStart < iEnd )
		{
			LogAddC(LOGC_RED,"CStreamPacketEngine XorData Error %d,%d", iStart, iEnd); 
			return;
		}

		BYTE byXorFilter[SIZE_SPE_XORFILTER] =
		{
			0xE7, 0x6D, 0x3A, 0x89, 0xBC, 0xB2, 0x9F, 0x73,
			0x23, 0xA8, 0xFE, 0xB6, 0x49, 0x5D, 0x39, 0x5D,
			0x8A, 0xCB, 0x63, 0x8D, 0xEA, 0x7D, 0x2B, 0x5F,
			0xC3, 0xB1, 0xE9, 0x83, 0x29, 0x51, 0xE8, 0x56
		};
		for ( int i = iStart; i != iEnd; i += iDir)
		{
			m_byBuffer[i] ^= ( m_byBuffer[i - 1] ^ byXorFilter[i%SIZE_SPE_XORFILTER]);
		}
	}
public:
	BOOL AddData( void *pSrc, WORD wSize)
	{
		//assert( m_wSize + wSize <= MAX_SPE_BUFFERSIZE);
		if( ((m_wSize + wSize) >= MAX_SPE_BUFFERSIZE) || wSize == 0 )
		{
			LogAddC(LOGC_RED,"CStreamPacketEngine Adding Buffer Size Error %d", m_wSize + wSize); 

			int isize = MAX_SPE_BUFFERSIZE;
			return FALSE;
		}
		memcpy( &m_byBuffer[m_wSize], pSrc, wSize);
		m_wSize += wSize;
		return TRUE;
	}
	int ExtractPacket( void *pTar)
	{
		WORD wSize;
		switch ( m_byBuffer[0])
		{
		case 0xC1:
			wSize = ( WORD)m_byBuffer[1];
			break;
		case 0xC2:
			wSize = ( ( WORD)m_byBuffer[1] << 8) + m_byBuffer[2];
			break;
		default:
			return ( 1);	// invalid packet
		}
		if ( m_wSize < wSize)
		{
			return ( 2);	// not complete packet
		}

		// extract
		BYTE byXorFilter[SIZE_SPE_XORFILTER] =
		{
			0xE7, 0x6D, 0x3A, 0x89, 0xBC, 0xB2, 0x9F, 0x73,
			0x23, 0xA8, 0xFE, 0xB6, 0x49, 0x5D, 0x39, 0x5D,
			0x8A, 0xCB, 0x63, 0x8D, 0xEA, 0x7D, 0x2B, 0x5F,
			0xC3, 0xB1, 0xE9, 0x83, 0x29, 0x51, 0xE8, 0x56
		};
		XorData( ( int)wSize - 1, ( int)( ( m_byBuffer[0] == 0xC1) ? 3 : 4) - 1, -1);
		memcpy( pTar, m_byBuffer, wSize);
		// pull
		BYTE byTemp[MAX_SPE_BUFFERSIZE];
		m_wSize -= wSize;
		memcpy( byTemp, &m_byBuffer[wSize], m_wSize);
		memcpy( m_byBuffer, byTemp, m_wSize);
		return ( 0);	// success
	}
};


#endif // !defined(AFX_STREAMPACKETENGINE_H__C0F4554E_77C8_4E5A_A27E_10A6E10A1364__INCLUDED_)
