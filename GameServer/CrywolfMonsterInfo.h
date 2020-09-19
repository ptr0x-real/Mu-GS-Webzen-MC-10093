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
			m_iObjIndex = _INVALID;
		}
	}
	
public:
	int	m_iObjCount;

	int	m_iObjIndex[MAX_CRYWOLF_OBJINFO];



};

#endif