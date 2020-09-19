#if !defined(__TMONSTERSKILLINFO_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_01_20050714)
#define __TMONSTERSKILLINFO_DEF__

#include "TSkillDefine.h"
#include "TMonsterSkillUnit.h"

class TMonsterSkillInfo
{

public:
	TMonsterSkillInfo()
	{
		Reset();
	}

	void Reset()
	{
		m_iMonsterIndex		= _NONE;
		m_iSkillUnitCount	= _ZERO;
		
		for( int i=0; i<MAX_MONSTER_SKILLUNIT_SLOT; i++ )
		{
			m_iSkillUnitTypeArray[i]	= _NONE;
			m_lpSkillUnitArray[i]		= NULL;
		}
	}
	
	BOOL IsValid()
	{
		if( m_iMonsterIndex==_NONE || m_iSkillUnitCount==_ZERO )	
			return FALSE;

		return TRUE;
	}

public:
	int	m_iMonsterIndex;
	int	m_iSkillUnitCount;
	int	m_iSkillUnitTypeArray[MAX_MONSTER_SKILLUNIT_SLOT];
	TMonsterSkillUnit*	m_lpSkillUnitArray[MAX_MONSTER_SKILLUNIT_SLOT];
};

#endif // __TMONSTERSKILLINFO_DEF__