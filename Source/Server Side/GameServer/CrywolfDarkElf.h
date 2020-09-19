#if !defined(__CRYWOLFDARKELF_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLFDARKELF_DEF__

#include "CrywolfDefine.h"

class CCrywolfDarkElf
{

public:
	CCrywolfDarkElf();
	virtual ~CCrywolfDarkElf();

public:
	void CrywolfDarkElfAct( int iIndex );

};

extern CCrywolfDarkElf	g_CrywolfMON_DarkElf;

#endif