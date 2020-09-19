#ifndef PKCLEARSYSTEM_H
#define PKCLEARSYSTEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#include <map>

#define MAX_MESSAGE 5

struct NPC_DATA_PKCLEAR
{
	int m_iNpcIndex;	// 0
	int m_iMapIndex;	// 4
	int m_icX;	// 8
	int m_icY;	// C
	int m_iDir; // 10
};

typedef std::map<int,NPC_DATA_PKCLEAR> NPC_DATA_MAP_PKCLEAR;

class CPkClearSystem
{
public:

	CPkClearSystem();
	virtual ~CPkClearSystem();

	void Clear();
	void Init();
	BOOL Load(LPSTR lpszFileName);
	void CheckUserCondition(LPOBJECTSTRUCT lpObj);
	void SetPkClearNPCData(NPC_DATA_PKCLEAR &npc);
	void ClearPkClearNPCData();
	BOOL GetNPCPkClearData(int iIndex);

private:
	BOOL m_bHasData;	// 4
	BOOL m_bUsePkClearSystem;
	INT m_iZenPrice;
	BOOL m_bPriceType;
	NPC_DATA_MAP_PKCLEAR m_mNpcData;	//
	char * lpMessage[MAX_MESSAGE]; //
};

extern CPkClearSystem	g_PkClearSystem;

#endif