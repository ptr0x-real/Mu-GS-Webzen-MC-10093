#ifndef	__TSYNC_HEADER__
#define	__TSYNC_HEADER__

#include <windows.h>
#include <assert.h>

class	TSync
{
public:
	TSync();
	~TSync();

public:
	VOID		Lock();	
	VOID		Unlock();

protected:
	CRITICAL_SECTION	m_cs;
	/*volatile*/	LONG	m_nLock;

};

#endif