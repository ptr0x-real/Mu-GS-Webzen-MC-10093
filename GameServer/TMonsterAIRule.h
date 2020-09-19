#if !defined(__TMONSTERAIRULE_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAIRULE_DEF__




#include "TMonsterAIRuleInfo.h"


class TMonsterAIRule
{

public:
	TMonsterAIRule();
	virtual ~TMonsterAIRule();


public:
	static	int						LoadData( LPSTR lpszFileName );
	static	int						DelAllAIRule();

	static	int						GetCurrentAIUnit( int iMonsterClass );

	static	void					MonsterAIRuleProc();

public:
	static	BOOL					s_bDataLoad; 
	static	int						s_iMonsterCurrentAIUnitTable[MAX_MONSTER_CLASS];
	static	TMonsterAIRuleInfo		s_MonsterAIRuleInfoArray[MAX_MONSTER_AI_RULEINFO];
	static	int						s_iMonsterAIRuleInfoCount;
	
};

#endif