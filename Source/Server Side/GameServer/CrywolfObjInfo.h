#if !defined(__CRYWOLFOBJINFO_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLFOBJINFO_DEF__

#define MAX_CRYWOLF_OBJINFO	300

class CCrywolfObjInfo
{

public:
	CCrywolfObjInfo()
	{
		Reset();
	}
	
	void Reset()
	{
		for( int i=0; i<MAX_CRYWOLF_OBJINFO; i++ )
		{
			m_iObjIndex[i] = _INVALID;
		}

		m_iObjCount = _ZERO;
	}

	BOOL AddObj( int iIndex )
	{
		if( m_iObjCount >= MAX_CRYWOLF_OBJINFO )
			return FALSE;

		m_iObjIndex[m_iObjCount] = iIndex;
		m_iObjCount++;
		return TRUE;
	}

	
public:
	int	m_iObjCount;

	int	m_iObjIndex[MAX_CRYWOLF_OBJINFO];

};

#endif