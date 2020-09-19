#if !defined(__TMONSTERAIUNIT_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAIUNIT_DEF__

#include "TMonsterAIElement.h"
#include "TMonsterAIAutomata.h"

#define MAX_AIUNITNAME					50
#define MAX_MONSTER_AI_UNIT				100

class TMonsterAIUnit
{

public:
	TMonsterAIUnit();
	virtual ~TMonsterAIUnit();

public:
	static	int						LoadData( LPSTR lpszFileName );
	static	int						DelAllAIUnit();
	static	TMonsterAIUnit*			FindAIUnit( int iUnitNumber );

public:
	int								RunAIUnit( int iIndex );
	void							Reset();

public:
	static	BOOL					s_bDataLoad; 
	static	TMonsterAIUnit			s_MonsterAIUnitArray[MAX_MONSTER_AI_UNIT];

public:
	char							m_szUnitName[MAX_AIUNITNAME];
	int								m_iUnitNumber;
	int								m_iDelayTime;
	
private:
	TMonsterAIAutomata*				m_lpAutomata;

	TMonsterAIElement*				m_lpAIClassNormal;
	TMonsterAIElement*				m_lpAIClassMove;
	TMonsterAIElement*				m_lpAIClassAttack;
	TMonsterAIElement*				m_lpAIClassHeal;
	TMonsterAIElement*				m_lpAIClassAvoid;
	TMonsterAIElement*				m_lpAIClassHelp;
	TMonsterAIElement*				m_lpAIClassSpecial;
	TMonsterAIElement*				m_lpAIClassEvent;

	TMonsterAIElement**				m_lppAIClassMap[MAX_MONSTER_AI_ELEMENT_STATE];
	
};


#endif	// __TMONSTERAIUNIT_DEF__