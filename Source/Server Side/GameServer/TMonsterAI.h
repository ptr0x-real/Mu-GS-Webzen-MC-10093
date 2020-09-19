#if !defined(__TMONSTERAI_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAI_DEF__

#include "TMonsterAIRule.h"
#include "TMonsterAIUnit.h"

class TMonsterAI
{

public:
	TMonsterAI();
	virtual ~TMonsterAI();

public:
	static	void					MonsterMove			( int iIndex );
	static	void					MonsterMoveProc		( );
	
	static	void					MonsterAIProc		( );
	static	int						RunAI				( int iIndex, int iMonsterClass );

	static	int						UpdateCurrentAIUnit	( int iIndex );
	
	static	void					MonsterStateMsgProc	( int iIndex );
	
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	static  void					ProcessStateMsg		( LPOBJECTSTRUCT lpObj, int iMsgCode, int iIndex, int aMsgSubCode =0 );
#else
	static  void					ProcessStateMsg		( LPOBJECTSTRUCT lpObj, int iMsgCode, int iIndex );
#endif

};

#endif // __TMONSTERAI_DEF__