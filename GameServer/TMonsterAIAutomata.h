#if !defined(__TMONSTERAIAUTOMATA_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAIAUTOMATA_DEF__

#include "TMonsterAIAgro.h"
#include "TMonsterAIState.h"
#include "TMonsterAIElement.h" 

#define MAX_MONSTERAI_AUTOMATA					300

class TMonsterAIAutomata
{

public:
	TMonsterAIAutomata();
	virtual ~TMonsterAIAutomata();

public:
	static	int						LoadData( LPSTR lpszFileName );
	static	int						DelAllAutomata();
	static	TMonsterAIAutomata*		FindAutomata( int iAutomataNumber );

public:
	TMonsterAIState*				RunAutomata( int iIndex );
	void							Reset();

public:
	static	BOOL					s_bDataLoad; 
	static	TMonsterAIAutomata		s_MonsterAIAutomataArray[MAX_MONSTERAI_AUTOMATA];

private:
	int								m_iAutomataNumber;

	
	TMonsterAIState					m_AIState[MAX_MONSTER_AI_ELEMENT_STATE][MAX_MONSTERAI_STATE_TRANSITION_INFO];
	int								m_AIStateTransCount[MAX_MONSTER_AI_ELEMENT_STATE];

#ifndef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907	// not define
	#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
		DWORD							m_dwTickAutomata;
		DWORD							m_dwTickNextAutomataDelay;
	#endif
#endif

};


#endif // __TMONSTERAIAUTOMATA_DEF__