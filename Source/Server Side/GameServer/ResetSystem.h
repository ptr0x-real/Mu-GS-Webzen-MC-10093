#ifndef RESETSYSTEM_H
#define RESETSYSTEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#include "Include\ProDef.h"

#include <vector>

struct PMSG_REQ_VIP_INFO
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccount[MAX_IDSTRING+1];
};

struct PMSG_ANS_VIP_INFO
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccount[MAX_IDSTRING+1];
	int iVIPLevel;
};

struct NON_TABLE_RESET
{
	int m_iRuleIndex;	// 0
	int m_iCharClassType;	// 4
	int m_iMinLevel;	// 8
	int m_iMaxLevel;	// C
	BOOL m_bClearStat;	// 10
	int m_iLevelAfterReset;	// 14
	int m_iLevelUpPoint; //18
	BOOL m_bReqSpecialItem; //1C
	BOOL m_bBornPlace; //20
	BOOL m_bClearBuffState; //24
	int m_iVipCode; //28
	int m_iCheckInventory; //2C
	BOOL m_bClearMagicList; //30
};

struct TABLE_RESET
{
	int m_iRuleIndex;	// 0
	int m_iCharClassType;	// 4
	int m_iMinLevel;	// 8
	int m_iMaxLevel;	// C
	BOOL m_bClearStat;	// 10
	int m_iLevelAfterReset;	// 14
	int m_iLevelUpPoint; //18
	BOOL m_bReqSpecialItem; //1C
	BOOL m_bBornPlace; //20
	int m_iMinResetNumber;
	int m_iResetNumber; //24
	BOOL m_bClearBuffState; //28
	int m_iVipCode; //2C
	int m_iCheckInventory; //30
	BOOL m_bClearMagicList; //34
};

struct REQUIRE_SPECIAL_ITEM
{
	int m_iResetRuleIndex;	// 0
	int m_iItemCategory;	// 4
	int m_iItemCode;	// 8
	int m_iItemLevel;	// C
	BYTE m_btOption1;	// D
	BYTE m_btOption2;	// E
	BYTE m_btOption3;	// F
	BYTE m_btNewOption;	// 10
	int m_iItemNumber;	// 11
	int m_iItemPosition; // 15
};

struct BORN_PLACE
{
	int m_iResetRuleIndex;	// 0
	int m_iCharClassType;	// 4
	int m_iGateNumber;	// 8
};

struct PMSG_UPDATE_EXP
{
	PBMSG_HEAD h;	// 0
	BYTE subcode;	// 3
	BYTE MapX;	// 4
	BYTE MapY;	// 5
	BYTE MapNumber;	// 6
	BYTE Dir;	// 7
	WORD Life;	// 8
	WORD Mana;	// A
	WORD wShield;	// C
	WORD BP;	// E
	DWORD Exp;	// 10
	DWORD Money;	// 14
};

struct PMSG_STAT_UPDATE
{
	 PBMSG_HEAD h;	// 0
	BYTE result;	// 3
	WORD btStatValue; // 4
	BYTE btFruitType; // 6
};

struct NPC_DATA
{
	int m_iNpcIndex;	// 0
	int m_iMapIndex;	// 4
	int m_icX;	// 8
	int m_icY;	// C
	int m_iDir; // 10
};

#define MAX_MESSAGE 5

typedef std::map<int,NPC_DATA> NPC_DATA_MAP;

class CResetSystem
{
public:

	CResetSystem();
	virtual ~CResetSystem();

	void Clear();
	void Init();
	BOOL Load(LPSTR lpszFileName);
	void DBGetResetInfo(LPOBJECTSTRUCT lpObj);
	void DBLoadReset(LPBYTE aRecv);
	void DBReqUpdateReset(LPOBJECTSTRUCT lpObj, int iRuleIndex, BOOL iClearStatPoint, int iLevelAfter, int iLevelUpPoint, BOOL iReqSpecialItem, BOOL iBornPlace, int iResetNumber, BOOL iClearAllBuffState, BOOL iClearMagicList);
	void DBAnsUpdateReset(LPBYTE aRecv);
	void ClearAllBuffStates(LPOBJECTSTRUCT lpObj);
	void CalculateEquipment(LPOBJECTSTRUCT lpObj);
	void FixUserExpInterface(LPOBJECTSTRUCT lpObj);
	void SetUserDefaultLifePlusMana(LPOBJECTSTRUCT lpObj);
	void SetUserBornPlace(LPOBJECTSTRUCT lpObj, int iRuleIndex, int iClass);
	void UpdateUserStatusPoint(LPOBJECTSTRUCT lpObj);
	bool SearchResetRequiredItem(LPOBJECTSTRUCT lpObj, int iRuleIndex);
	void DeleteResetRequiredItem(LPOBJECTSTRUCT lpObj, int iRuleIndex);
	void CheckUserCondition(LPOBJECTSTRUCT lpObj);
	void ExecResetApply(LPOBJECTSTRUCT lpObj, int iRuleIndex, BOOL iClearStatPoint, int iLevelAfter, int iLevelUpPoint, BOOL iReqSpecialItem, BOOL iBornPlace, int iResetNumber, BOOL iClearAllBuffState, BOOL iClearMagicList);
	void SetResetNPCData(NPC_DATA &npc);
	void ClearResetNPCData();
	BOOL GetNPCResetData(int iIndex);
	BOOL CheckUserEquipment(LPOBJECTSTRUCT lpObj);
	void ClearMagicList(LPOBJECTSTRUCT lpObj);
	BOOL GetResetCmdAuthority(){return this->m_bUseResetCommand;}

private:
	CRITICAL_SECTION	m_cs; // 4
	BOOL m_bHasData;	// 8
	BOOL m_bUseResetSystem; // C
	BOOL m_bResetSystemType; // 10
	BOOL m_bUseResetCommand; // 14
	BOOL m_bZenMultiply;
	INT  m_iZenPrice;
	bool m_bLeadershipReset;
	std::vector<NON_TABLE_RESET> m_vtNonTableReset;	// 
	std::vector<TABLE_RESET> m_vtTableReset;	// 
	std::vector<REQUIRE_SPECIAL_ITEM> m_vtReqSpecialItem;	// 
	std::vector<BORN_PLACE> m_vtBornPlace;	//
	NPC_DATA_MAP m_mNpcData;	//
	char * lpMessage[MAX_MESSAGE]; //
};

extern CResetSystem g_ResetSystem;

#endif