#ifndef __TDURMAGICKEYCHECKER_DEF__
#define __TDURMAGICKEYCHECKER_DEF__

#define DURATION_M_MAX_COUNT				60
#define DURATION_M_LIMIT_TIME				15000
#define DURATION_M_DEFAULT_MAGICKEY			0
#define DURATION_M_DEFAULT_VALIDCOUNT		7

#define CHECK_VALID_MAGICKEY( x )		( (x<=DURATION_M_MAX_COUNT-1) && (x>0) )

class TDurMagicKeyChecker
{
public:
	BOOL IsValidDurationTime( BYTE btMagicKey )
	{
		if( CHECK_VALID_MAGICKEY(btMagicKey) )
		{
			if( ( GetTickCount() - m_dwDurationTime[btMagicKey] ) < DURATION_M_LIMIT_TIME )
			{
				return TRUE;
			}
		}
		
		return FALSE;
	}

	BOOL IsValidCount( BYTE btMagicKey )
	{
		if( CHECK_VALID_MAGICKEY(btMagicKey) )
		{
			if( m_btValidCount[btMagicKey] >=0 )
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	void SetDurationTime( BYTE btMagicKey, DWORD dwCurrentTickCount )
	{
		if( CHECK_VALID_MAGICKEY(btMagicKey) )
		{
			m_dwDurationTime[btMagicKey] = dwCurrentTickCount;	
			m_btValidCount[btMagicKey] = DURATION_M_DEFAULT_VALIDCOUNT;
		}
	}

	BYTE DecValidCount( BYTE btMagicKey )
	{
		if( CHECK_VALID_MAGICKEY(btMagicKey) )
		{
			if( m_btValidCount[btMagicKey] <=0 )
				return 0;

			return --m_btValidCount[btMagicKey];
		}

		return 0;
	}

	INT	GetValidDurationTime( BYTE btMagicKey )
	{
		if( CHECK_VALID_MAGICKEY(btMagicKey) )
		{
			return 	m_dwDurationTime[btMagicKey];		
		}

		return -77;
	}
	
	INT	GetValidCount( BYTE btMagicKey )
	{
		if( CHECK_VALID_MAGICKEY(btMagicKey) )
		{
			return 	m_btValidCount[btMagicKey];		
		}

		return -77;
	}
	
private:
	DWORD m_dwDurationTime[DURATION_M_MAX_COUNT];
	BYTE  m_btValidCount[DURATION_M_MAX_COUNT];
};

#endif