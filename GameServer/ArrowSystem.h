#ifndef __ARROW_SYSTEM_H__
#define __ARROW_SYSTEM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#include <map>

typedef std::map<INT, bool>	ITEM_LIST;

class CArrowSystem
{
public:
	CArrowSystem();

	~CArrowSystem();

	void Load(const char *filename = "..\\data\\ArrowSystem.cfg");

	bool CheckArrow(int itemId);

private:
	ITEM_LIST			m_itemList;

	CRITICAL_SECTION	m_criticalSection;

	bool				m_bFreeArrow;
};

extern CArrowSystem g_ArrowSystem;

#endif