#ifndef BUFFHELPER_H
#define BUFFHELPER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#include <map>

struct NPC_DATA_HELPER
{
	int m_iNpcIndex;	// 0
	int m_iMapIndex;	// 4
	int m_icX;	// 8
	int m_icY;	// C
	int m_iDir; // 10
};

typedef std::map<int,NPC_DATA_HELPER> NPC_DATA_MAP_HELPER;

class CBuffHelper
{
public:
	CBuffHelper();

	void Clear();
	void Init();
	BOOL Load(LPSTR lpszFileName);
	void CheckUserCondition(LPOBJECTSTRUCT lpObj, int aNpcIndex);
	void SetNPCData(NPC_DATA_HELPER &npc);
	void ClearNPCData();
	BOOL GetNPCData(int iIndex);

private:
	bool	m_bHasData;	// 4
	bool	m_bUseBuffHelper;
	INT		m_iMaxLevel;
	INT		m_iNpcEnergy;

public:
	INT		m_iAuraTime;

private:
	NPC_DATA_MAP_HELPER m_mNpcData;	//
};

extern CBuffHelper	g_BuffHelper;

#endif