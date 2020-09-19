#if !defined(__CRYWOLFTANKER_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLFTANKER_DEF__

#include "CrywolfDefine.h"

class CCrywolfTanker
{

public:
	CCrywolfTanker();
	virtual ~CCrywolfTanker();

public:
	void CrywolfTankerAct( int iIndex );

};

extern CCrywolfTanker	g_CrywolfMON_Tanker;

#endif