// ------------------------------
// ResetSystem.cpp for GameServer 97D. (1275 lines)
// Author: HermeX (Special Thanks to Marin, Prody, SirMaster)
// Last Modified Date: 2011-01-13
// Comment: Reset System adapted to 97d and Reset NPC Improved
// ------------------------------
#include "stdafx.h"
#include "ResetSystem.h"
#include "LogProc.h"
#include "LogToFile.h"
#include "..\\include\\readscript.h"
#include "user.h"
#include "DSProtocol.h"
#include "ObjUseSkill.h"
#include "ObjCalCharacter.h"
#include "GameMain.h"
#include "..\\common\\winutil.h"
#include "..\\common\\classdef.h"
#include "DBSockMng.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "GMMng.h"

#include "DevilSquare.h"

#include "MonsterAttr.h"

#include "..\\common\\zzzitem.h"

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

extern classdef			DCInfo;
extern CMonsterAttr		gMAttr;
extern ITEM_ATTRIBUTE	ItemAttribute [MAX_ITEM];

CLogToFile RESET_SYSTEM_LOG( "RESET_SYSTEM_LOG", ".\\RESET_BUGTRACER_LOG", 1);

CResetSystem g_ResetSystem;

CResetSystem::CResetSystem()
{
	this->m_bHasData = FALSE;
	this->m_bUseResetSystem = FALSE;
	this->m_bResetSystemType = 0;
	this->Clear();

	//InitializeCriticalSection(&this->m_cs);
}

CResetSystem::~CResetSystem()
{
	//DeleteCriticalSection(&this->m_cs);
}

void CResetSystem::Clear()
{
	this->m_vtNonTableReset.clear();
	this->m_vtTableReset.clear();
	this->m_vtReqSpecialItem.clear();
	this->m_vtBornPlace.clear();
	this->ClearResetNPCData();

	for(int i= 0; i < MAX_MESSAGE; i++) //Clear Messages
	{
		this->lpMessage[i] = NULL;
	}
}

void CResetSystem::Init()
{
	BOOL bError = FALSE;

	LogAddTD("[Reset System] Selected Setting: USE_RESET_SYSTEM -> %d || RESET_SYSTEM_TYPE -> %d || USE_RESET_CMD -> %d", this->m_bUseResetSystem, this->m_bResetSystemType, this->m_bUseResetCommand);

	if(this->m_bResetSystemType == 1)
	{
		if(this->m_vtTableReset.empty() != false)
		{
			LogAddC(2,"[Reset System] CResetSystem::Init() - Table Reset Setting Is Empty (m_vtTableReset is Zero)");
			this->m_bHasData = FALSE;
			return;
		}

		for ( std::vector<TABLE_RESET>::iterator it = this->m_vtTableReset.begin() ; it != this->m_vtTableReset.end() ; it++ )
		{
			if((*(it)).m_bReqSpecialItem == TRUE && this->m_vtReqSpecialItem.size() <= 0)
			{
				LogAddC(2,"[Reset System] CResetSystem::Init() - Require Special Item Is Empty (m_vtReqSpecialItem is Zero)");
				bError = TRUE;
			}

			if((*(it)).m_bBornPlace == TRUE && this->m_vtBornPlace.size() <= 0)
			{
				LogAddC(2,"[Reset System] CResetSystem::Init() - Born Place Is Empty (m_vtBornPlace is Zero)");
				bError = TRUE;
			}

			LogAddTD("[Reset System] R_IDX: %d, C_TYPE: %d, MIN_LVL: %d, MAX_LVL: %d, CLR_STAT: %d, LVL_AFTER: %d, POINT: %d, RQ_ITEM: %d, B_PLACE: %d, RST_NUM: %d,CLR_BUFF: %d, VIP: %d, CHK_INVENT: %d, CLR_MAGIC_LIST: %d",
				(*(it)).m_iRuleIndex, (*(it)).m_iCharClassType, (*(it)).m_iMinLevel, (*(it)).m_iMaxLevel, (*(it)).m_bClearStat, (*(it)).m_iLevelAfterReset, (*(it)).m_iLevelUpPoint, (*(it)).m_bReqSpecialItem, (*(it)).m_bBornPlace, (*(it)).m_iResetNumber, (*(it)).m_bClearBuffState, (*(it)).m_iVipCode, (*(it)).m_iCheckInventory, (*(it)).m_bClearMagicList);
		}

		if(bError == TRUE)
		{
			this->m_bHasData = FALSE;
			return;
		}
	}
	else
	{
		if(this->m_vtNonTableReset.empty() != false)
		{
			LogAddC(2,"[Reset System] CResetSystem::Init() - Non Table Reset Setting Is Empty (m_vtNonTableReset is Zero)");
			this->m_bHasData = FALSE;
			return;
		}

		for ( std::vector<NON_TABLE_RESET>::iterator it = this->m_vtNonTableReset.begin() ; it != this->m_vtNonTableReset.end() ; it++ )
		{
			if((*(it)).m_bReqSpecialItem == TRUE && this->m_vtReqSpecialItem.size() <= 0)
			{
				LogAddC(2,"[Reset System] CResetSystem::Init() - Require Special Item Is Empty (m_vtReqSpecialItem is Zero)");
				bError = TRUE;
			}

			if((*(it)).m_bBornPlace == TRUE && this->m_vtBornPlace.size() <= 0)
			{
				LogAddC(2,"[Reset System] CResetSystem::Init() - Born Place Is Empty (m_vtBornPlace is Zero)");
				bError = TRUE;
			}

			LogAddTD("[Reset System] R_IDX: %d, C_TYPE: %d, MIN_LVL: %d, MAX_LVL: %d, CLR_STAT: %d, LVL_AFTER: %d, POINT: %d, RQ_ITEM: %d, B_PLACE: %d, CLR_BUFF: %d, VIP: %d, CHK_INVENT: %d, CLR_MAGIC_LIST: %d",
				(*(it)).m_iRuleIndex, (*(it)).m_iCharClassType, (*(it)).m_iMinLevel, (*(it)).m_iMaxLevel, (*(it)).m_bClearStat, (*(it)).m_iLevelAfterReset, (*(it)).m_iLevelUpPoint, (*(it)).m_bReqSpecialItem, (*(it)).m_bBornPlace, (*(it)).m_bClearBuffState, (*(it)).m_iVipCode, (*(it)).m_iCheckInventory, (*(it)).m_bClearMagicList);
		}

		if(bError == TRUE)
		{
			this->m_bHasData = FALSE;
			return;
		}
	}

	if(this->m_bUseResetCommand == 0 && this->m_mNpcData.size() == 0)
	{
		LogAddC(2,"[Reset System] CResetSystem::Init() - Reset Command is Disable and NPC Setting is empty (m_mNpcData is Zero)");
		this->m_bHasData = FALSE;
		return;
	}

	this->m_bHasData = TRUE;
}

BOOL CResetSystem::Load(LPSTR lpszFileName)
{
	this->Clear();

	if ( !lpszFileName || !strcmp(lpszFileName , "") )
	{
		MsgBox("[Reset System] - File load error : File Name Error");
		return FALSE;
	}

	TABLE_RESET TableReset;

	memset(&TableReset, 0, sizeof(TableReset));

	try
	{
		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[Reset System] Info file Load Fail [%s]", lpszFileName);

			return false;
		}

		int Token;
		int type = -1;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
			{
				break;
			}

			type = TokenNumber;

			while ( true )
			{
				if ( type == 0 ) //Reset Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					this->m_bUseResetSystem = TokenNumber;
					
					Token = GetToken();
					this->m_bResetSystemType = TokenNumber;

					Token = GetToken();
					this->m_bUseResetCommand = TokenNumber;

					Token = GetToken();
					this->m_bZenMultiply = TokenNumber > 0;

					Token = GetToken();
					this->m_iZenPrice = TokenNumber;

					Token = GetToken();
					this->m_bLeadershipReset = TokenNumber > 0;
				}

				else if ( type == 1 ) //Simple Reset Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					NON_TABLE_RESET NonTableReset;

					NonTableReset.m_iRuleIndex = TokenNumber;

					Token = GetToken();
					NonTableReset.m_iCharClassType = TokenNumber;

					Token = GetToken();
					NonTableReset.m_iMinLevel = TokenNumber;

					Token = GetToken();
					NonTableReset.m_iMaxLevel = TokenNumber;

					Token = GetToken();
					NonTableReset.m_bClearStat = TokenNumber;

					Token = GetToken();
					NonTableReset.m_iLevelAfterReset = TokenNumber;

					Token = GetToken();
					NonTableReset.m_iLevelUpPoint = TokenNumber;

					Token = GetToken();
					NonTableReset.m_bReqSpecialItem = TokenNumber;

					Token = GetToken();
					NonTableReset.m_bBornPlace = TokenNumber;

					Token = GetToken();
					NonTableReset.m_bClearBuffState = TokenNumber;

					Token = GetToken();
					NonTableReset.m_iVipCode = TokenNumber;

					Token = GetToken();
					NonTableReset.m_iCheckInventory = TokenNumber;

					Token = GetToken();
					NonTableReset.m_bClearMagicList = TokenNumber;

					this->m_vtNonTableReset.push_back(NonTableReset);
				}

				else if ( type == 2 ) //Table Reset Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					TableReset.m_iRuleIndex = TokenNumber;

					Token = GetToken();
					TableReset.m_iCharClassType = TokenNumber;

					Token = GetToken();
					TableReset.m_iMinLevel = TokenNumber;

					Token = GetToken();
					TableReset.m_iMaxLevel = TokenNumber;

					Token = GetToken();
					TableReset.m_bClearStat = TokenNumber;

					Token = GetToken();
					TableReset.m_iLevelAfterReset = TokenNumber;

					Token = GetToken();
					TableReset.m_iLevelUpPoint = TokenNumber;

					Token = GetToken();
					TableReset.m_bReqSpecialItem = TokenNumber;

					Token = GetToken();
					TableReset.m_bBornPlace = TokenNumber;

					TableReset.m_iMinResetNumber = TableReset.m_iResetNumber;

					Token = GetToken();
					TableReset.m_iResetNumber = TokenNumber;

					Token = GetToken();
					TableReset.m_bClearBuffState = TokenNumber;

					Token = GetToken();
					TableReset.m_iVipCode = TokenNumber;

					Token = GetToken();
					TableReset.m_iCheckInventory = TokenNumber;

					Token = GetToken();
					TableReset.m_bClearMagicList = TokenNumber;

					this->m_vtTableReset.push_back(TableReset);
				}

				else if ( type == 3 ) //Special Item Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					REQUIRE_SPECIAL_ITEM ReqSpecialItem;

					ReqSpecialItem.m_iResetRuleIndex = TokenNumber;

					Token = GetToken();
					ReqSpecialItem.m_iItemCategory = TokenNumber;

					Token = GetToken();
					ReqSpecialItem.m_iItemCode = TokenNumber;

					Token = GetToken();
					ReqSpecialItem.m_iItemLevel = TokenNumber;

					Token = GetToken();
					ReqSpecialItem.m_btOption1 = TokenNumber;

					Token = GetToken();
					ReqSpecialItem.m_btOption2 = TokenNumber;

					Token = GetToken();
					ReqSpecialItem.m_btOption3 = TokenNumber;

					Token = GetToken();
					ReqSpecialItem.m_btNewOption = TokenNumber;

					Token = GetToken();
					ReqSpecialItem.m_iItemNumber = TokenNumber;

					Token = GetToken();
					ReqSpecialItem.m_iItemPosition = TokenNumber;

					this->m_vtReqSpecialItem.push_back(ReqSpecialItem);
				}

				else if ( type == 4 ) //Born Place Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					BORN_PLACE BornPlace;

					BornPlace.m_iResetRuleIndex = TokenNumber;

					Token = GetToken();
					BornPlace.m_iCharClassType = TokenNumber;

					Token = GetToken();
					BornPlace.m_iGateNumber = TokenNumber;

					this->m_vtBornPlace.push_back(BornPlace);
				}

				else if ( type == 5 ) //Message Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					int Index = TokenNumber;

					Token = GetToken();
					
					if(Index < MAX_MESSAGE && Index >= 0)
					{
						this->lpMessage[Index] = new char[strlen(TokenString)+1];
						strcpy(this->lpMessage[Index], TokenString);
					}
				}

				else if ( type == 6 ) //NPC Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					NPC_DATA NpcData;

					NpcData.m_iNpcIndex = TokenNumber;

					Token = GetToken();
					NpcData.m_iMapIndex = TokenNumber;

					Token = GetToken();
					NpcData.m_icX = TokenNumber;

					Token = GetToken();
					NpcData.m_icY = TokenNumber;

					Token = GetToken();
					NpcData.m_iDir = TokenNumber;

					this->SetResetNPCData(NpcData);
				}
			}
		}

		fclose(SMDFile);
		LogAdd("[Reset System] %s file load!", lpszFileName);

		this->Init(); //check settings from the file
	}	// __try
	catch ( DWORD )
	{
		MsgBox("[Reset System] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return true;
}

void CResetSystem::DBGetResetInfo(LPOBJECTSTRUCT lpObj) //0xD1 GS->MNG_Server
{
	if(lpObj->m_bResetState == 1)
	{
		return;
	}

	PMSG_CHARACTER_RESET pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xA8, sizeof(pMsg));
	pMsg.iUserIndex = lpObj->m_Index;
	memcpy(pMsg.chAccountID, lpObj->AccountID, MAX_IDSTRING);
	pMsg.chAccountID[MAX_IDSTRING] = '\0';
	memcpy(pMsg.chCharacterName, lpObj->Name, MAX_IDSTRING);
	pMsg.chCharacterName[MAX_IDSTRING] = '\0';
	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}

void CResetSystem::DBLoadReset(LPBYTE aRecv) //0xD1 MNG_Server->GS
{
	PMSG_ANS_CHARACTER_RESET * pRecv = (PMSG_ANS_CHARACTER_RESET *)aRecv;

	if(!gObjIsConnectedGP(pRecv->iUserIndex))
	{
		return;
	}

	int aIndex = pRecv->iUserIndex;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if(lpObj->m_bResetState == 0)
	{
		lpObj->m_iResetNumber = pRecv->ResetNumber;
		lpObj->m_bResetState = 1;

		char szTemp[256];
		wsprintf(szTemp,"[RESET SYSTEM]: Tienes %d Reset(s).", lpObj->m_iResetNumber); //[Reset System] You currently have %d Reset(s).
		GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);


		LogAddTD("[Reset System] Set User ResetInfo [%s][%s](%d)", lpObj->AccountID, lpObj->Name, lpObj->m_iResetNumber);
	}
}

void CResetSystem::DBReqUpdateReset(LPOBJECTSTRUCT lpObj, int iRuleIndex, BOOL iClearStatPoint, int iLevelAfter, int iLevelUpPoint, BOOL iReqSpecialItem, BOOL iBornPlace, int iResetNumber, BOOL iClearAllBuffState, BOOL iClearMagicList) //0xD2 GS->MNG_Server
{
	PMSG_REQ_SAVE_CHARACTER_RESET pMsg;

	if(lpObj->m_bReqUpdateReset == true)
	{
		return;
	}

	PHeadSetB( (LPBYTE)&pMsg, 0xA9, sizeof(pMsg));
	memcpy(pMsg.chAccountID, lpObj->AccountID, 10);
	memcpy(pMsg.chCharacterName, lpObj->Name, 10);
	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.m_iRuleIndex = iRuleIndex;
	pMsg.m_bClearStat = iClearStatPoint;
	pMsg.m_iLevelAfterReset = iLevelAfter;
	pMsg.m_iLevelUpPoint = iLevelUpPoint;
	pMsg.m_bReqSpecialItem = iReqSpecialItem;
	pMsg.m_bBornPlace = iBornPlace;
	pMsg.m_iResetNumber = iResetNumber;
	pMsg.m_bClearBuffState = iClearAllBuffState;
	pMsg.m_bClearMagicList = iClearMagicList;

	LogAddTD("[Reset System] Req Update User ResetInfo [%s][%s](%d)", lpObj->AccountID, lpObj->Name, lpObj->m_iResetNumber);

	lpObj->m_bReqUpdateReset = true;

	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}

void CResetSystem::DBAnsUpdateReset(LPBYTE aRecv) //0xD4 MNG_Server->GS
{
	//LeaveCriticalSection(&this->m_cs);

	PMSG_ANS_SAVE_CHARACTER_RESET * pRecv = (PMSG_ANS_SAVE_CHARACTER_RESET *)aRecv;
	
	char szTemp[256];

	if(!gObjIsConnectedGP(pRecv->iUserIndex))
	{
		wsprintf(szTemp,"[Reset System] DBAnsUpdateReset ERROR (%d)", pRecv->iUserIndex);
		RESET_SYSTEM_LOG.Output(szTemp);
		return;
	}

	int aIndex = pRecv->iUserIndex;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if(pRecv->m_iResetNumber == lpObj->m_iResetNumber+1)
	{
		LogAddTD("[Reset System] Ans Update User ResetInfo - SUCCESS [%s][%s](%d)", lpObj->AccountID, lpObj->Name, pRecv->m_iResetNumber);
		this->ExecResetApply(lpObj, pRecv->m_iRuleIndex, pRecv->m_bClearStat, pRecv->m_iLevelAfterReset, pRecv->m_iLevelUpPoint, pRecv->m_bReqSpecialItem, pRecv->m_bBornPlace, pRecv->m_iResetNumber, pRecv->m_bClearBuffState, pRecv->m_bClearMagicList);
	}
	else
	{
		LogAddTD("[Reset System] Ans Update User ResetInfo - FAIL [%s][%s](%d)", lpObj->AccountID, lpObj->Name, pRecv->m_iResetNumber);
		lpObj->m_bReqUpdateReset = false;
		
		wsprintf(szTemp,"[Reset System] Ans Update User ResetInfo - FAIL [%s][%s](%d)", lpObj->AccountID, lpObj->Name, pRecv->m_iResetNumber);
		RESET_SYSTEM_LOG.Output(szTemp);
	}
}

void CResetSystem::ClearAllBuffStates(LPOBJECTSTRUCT lpObj)
{
	lpObj->m_bAttackerKilled = 0;
	lpObj->m_ViewState = 0;
	lpObj->Teleport = 0;

	int i; for(i = 0; i < MAXSELFDEFENSE; i++)
	{
		lpObj->SelfDefenseTime[i] = 0;
	}

	gObjTimeCheckSelfDefense(lpObj);

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	gObjUseSkill.RemoveAllCharacterInvalidMagicAndSkillState(lpObj);
#endif

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
	lpObj->m_iAutoRecuperationTime = GetTickCount();
#endif
}

void CResetSystem::CalculateEquipment(LPOBJECTSTRUCT lpObj)
{
	gObjCalCharacter(lpObj->m_Index);
#ifdef ADD_SHIELD_POINT_01_20060403
	GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);
#else
	GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0);
#endif
	gObjSetBP(lpObj->m_Index);
	GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP);
	gObjNextExpCal(lpObj);
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	GCLevelUpMsgSend(lpObj->m_Index);
#else
	short AddPoint = 0, MaxAddPoint = 0;

	gObjGetStatPointState(lpObj->m_Index, AddPoint, MaxAddPoint);

	GCLevelUpMsgSend(lpObj->m_Index, lpObj->Level, lpObj->LevelUpPoint, lpObj->MaxLife, lpObj->MaxMana, lpObj->MaxBP, AddPoint, MaxAddPoint);
#endif
}

void CResetSystem::FixUserExpInterface(LPOBJECTSTRUCT lpObj)
{
	PMSG_UPDATE_EXP pMsg;
	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0xF3;
	pMsg.h.size = 0x18; //size defined o.O
	pMsg.subcode = 4; //unk
	pMsg.Dir = lpObj->Dir;
	pMsg.MapX = lpObj->X;
	pMsg.MapY = lpObj->Y;
	pMsg.Life = lpObj->Life;
	pMsg.Mana = lpObj->Mana;
	pMsg.Exp = lpObj->Experience;
	pMsg.MapNumber = lpObj->MapNumber;
	pMsg.Money = lpObj->Money;
	pMsg.BP = lpObj->BP;
	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
}

void CResetSystem::SetUserDefaultLifePlusMana(LPOBJECTSTRUCT lpObj)
{
	lpObj->MaxLife = DCInfo.DefClass[lpObj->Class].Life + (lpObj->Level - 1) * DCInfo.DefClass[lpObj->Class].LevelLife  + ((lpObj->Vitality - DCInfo.DefClass[lpObj->Class].Vitality ) * DCInfo.DefClass[lpObj->Class].VitalityToLife);
	lpObj->MaxMana = DCInfo.DefClass[lpObj->Class].Mana + (lpObj->Level - 1) * DCInfo.DefClass[lpObj->Class].LevelMana  + ((lpObj->Energy - DCInfo.DefClass[lpObj->Class].Energy ) * DCInfo.DefClass[lpObj->Class].EnergyToMana);

	lpObj->VitalityToLife = DCInfo.DefClass[lpObj->Class].VitalityToLife;
	lpObj->EnergyToMana = DCInfo.DefClass[lpObj->Class].EnergyToMana;

	::gObjSetBP(lpObj->m_Index);
	lpObj->BP = lpObj->MaxBP / 2;
	::gObjCalcMaxLifePower(lpObj->m_Index);
}

void CResetSystem::SetUserBornPlace(LPOBJECTSTRUCT lpObj, int iRuleIndex, int Class)
{
	for(std::vector<BORN_PLACE>::iterator it = this->m_vtBornPlace.begin(); it != this->m_vtBornPlace.end(); it++)
	{
		if ((*(it)).m_iResetRuleIndex == iRuleIndex)
		{
			if((*(it)).m_iCharClassType == Class)
			{
				if(::gObjMoveGate(lpObj->m_Index, (*(it)).m_iGateNumber) == TRUE)
				{
					break;
				}
			}
		}
	}
}

void CResetSystem::UpdateUserStatusPoint(LPOBJECTSTRUCT lpObj) // -> SirMaster Specialty + GameServer 1.00.19 Specialty
{
	/*PMSG_STAT_UPDATE pMsg;
	::PHeadSetB((LPBYTE)&pMsg,0x2C,sizeof(PMSG_STAT_UPDATE));

	pMsg.result = 3;

	int iCharacterClass = lpObj->Class;
	int iReduceStr = 0;
	int iReduceDtx = 0;
	int iReduceVit = 0;
	int iReduceEne = 0;
	int iReduceCmd = 0;

	if(lpObj->Strength > DCInfo.DefClass[iCharacterClass].Strength){iReduceStr = lpObj->Strength - DCInfo.DefClass[iCharacterClass].Strength;}
	if(lpObj->Dexterity > DCInfo.DefClass[iCharacterClass].Dexterity){iReduceDtx = lpObj->Dexterity - DCInfo.DefClass[iCharacterClass].Dexterity;}
	if(lpObj->Vitality > DCInfo.DefClass[iCharacterClass].Vitality){iReduceVit = lpObj->Vitality - DCInfo.DefClass[iCharacterClass].Vitality;}
	if(lpObj->Energy > DCInfo.DefClass[iCharacterClass].Energy){iReduceEne = lpObj->Energy - DCInfo.DefClass[iCharacterClass].Energy;}
	if(lpObj->Leadership > DCInfo.DefClass[iCharacterClass].Leadership){iReduceCmd = lpObj->Leadership - DCInfo.DefClass[iCharacterClass].Leadership;}

	int i; for(i = 0; i < 5; i++) // -> GameServer 1.00.19 Specialty
	{
		switch(i)
		{
			case 4:{pMsg.btFruitType = 4;	pMsg.btStatValue = iReduceCmd;	lpObj->Leadership	-= iReduceCmd;}		break;
			case 3:{pMsg.btFruitType = 3;	pMsg.btStatValue = iReduceStr;	lpObj->Strength		-= iReduceStr;}		break;
			case 2:{pMsg.btFruitType = 2;	pMsg.btStatValue = iReduceDtx;	lpObj->Dexterity	-= iReduceDtx;}		break;
			case 1:{pMsg.btFruitType = 1;	pMsg.btStatValue = iReduceVit;	lpObj->Vitality		-= iReduceVit;}		break;
			case 0:{pMsg.btFruitType = 0;	pMsg.btStatValue = iReduceEne;	lpObj->Energy		-= iReduceEne;}		break;
		}
		DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
	}*/

	int iCharacterClass = lpObj->Class;
	if(lpObj->Strength > DCInfo.DefClass[iCharacterClass].Strength){lpObj->Strength = DCInfo.DefClass[iCharacterClass].Strength;}
	if(lpObj->Dexterity > DCInfo.DefClass[iCharacterClass].Dexterity){lpObj->Dexterity = DCInfo.DefClass[iCharacterClass].Dexterity;}
	if(lpObj->Vitality > DCInfo.DefClass[iCharacterClass].Vitality){lpObj->Vitality = DCInfo.DefClass[iCharacterClass].Vitality;}
	if(lpObj->Energy > DCInfo.DefClass[iCharacterClass].Energy){lpObj->Energy = DCInfo.DefClass[iCharacterClass].Energy;}
	if(m_bLeadershipReset == false && lpObj->Leadership > DCInfo.DefClass[iCharacterClass].Leadership){lpObj->Leadership = DCInfo.DefClass[iCharacterClass].Leadership;}

	cManager.ResetViewport(lpObj);
}

bool CResetSystem::SearchResetRequiredItem(LPOBJECTSTRUCT lpObj, int iRuleIndex) //Fixed
{
	int SearchCount = 0;
	int itemcount = 0;

	for ( std::vector<REQUIRE_SPECIAL_ITEM>::iterator it = this->m_vtReqSpecialItem.begin() ; it != this->m_vtReqSpecialItem.end() ; it++ )
	{
		if((*(it)).m_iResetRuleIndex != iRuleIndex)
		{
			continue;
		}

		itemcount++;
		SearchCount = 0;

		//EnterCriticalSection(&this->m_cs);

#ifdef PERSONAL_SHOP_20040113
		for( int n=0; n<MAX_INVENTORY_EXTEND; n++)
#else
		for( int n=0; n<MAX_INVENTORY; n++)
#endif
		{
			if ( lpObj->pInventory[n].IsItem() == TRUE )
			{
				if ( lpObj->pInventory[n].m_Type == MakeItemNumber((*(it)).m_iItemCategory,(*(it)).m_iItemCode) )
				{
					if ( (*(it)).m_iItemLevel != -1 )
					{
						if (  lpObj->pInventory[n].m_Level != (*(it)).m_iItemLevel )
						{
							continue;
						}
					}

					if ( (*(it)).m_btOption1 != -1 )
					{
						if (  lpObj->pInventory[n].m_Option1 != (*(it)).m_btOption1 )
						{
							continue;
						}
					}

					if ( (*(it)).m_btOption2 != -1 )
					{
						if (  lpObj->pInventory[n].m_Option2 != (*(it)).m_btOption2 )
						{
							continue;
						}
					}

					if ( (*(it)).m_btOption3 != -1 )
					{
						if (  lpObj->pInventory[n].m_Option3 != (*(it)).m_btOption3 )
						{
							continue;
						}
					}

					if ( (*(it)).m_btNewOption != -1 )
					{
						if (  lpObj->pInventory[n].m_NewOption != (*(it)).m_btNewOption )
						{
							continue;
						}
					}

					if ( (*(it)).m_iItemPosition != -1 )
					{
						if (n != (*(it)).m_iItemPosition )
						{
							continue;
						}
					}

					SearchCount++;
				}
			}
		}

		if ( SearchCount < (*(it)).m_iItemNumber)
		{
			ITEM_ATTRIBUTE *p = &ItemAttribute[MAKE_ITEMNUM((*(it)).m_iItemCategory, (*(it)).m_iItemCode) ];
			char szTemp[256];
			wsprintf(szTemp,"Fallo al resetear. Tienes (%d of %d -> %s) de los objetos necesarios para resetear.", SearchCount, (*(it)).m_iItemNumber, p->Name); //[Reset System] Failed to Reset. You have (%d of %d -> %s) Required Item.
			GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
			continue;
		}

		itemcount--;
		
		ITEM_ATTRIBUTE *p = &ItemAttribute[MAKE_ITEMNUM((*(it)).m_iItemCategory, (*(it)).m_iItemCode) ];
		char szTemp[256];
		wsprintf(szTemp,"Se encontraron (%d de %d -> %s) de los objetos necesarios para resetear.", SearchCount, (*(it)).m_iItemNumber, p->Name); //[Reset System] Found Valid (%d of %d -> %s) Required Item.
		GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
	}

	if(itemcount == 0) //Perfect
	{
		return true;
	}

	return false;
}

void CResetSystem::DeleteResetRequiredItem(LPOBJECTSTRUCT lpObj, int iRuleIndex) //Good
{
	for ( std::vector<REQUIRE_SPECIAL_ITEM>::iterator it = this->m_vtReqSpecialItem.begin() ; it != this->m_vtReqSpecialItem.end() ; it++ ) //it will check all rules
	{
		if((*(it)).m_iResetRuleIndex != iRuleIndex)
		{
			continue;
		}

		int ItemCount = 0; //

#ifdef PERSONAL_SHOP_20040113
		for( int pos=0; pos<MAX_INVENTORY_EXTEND; pos++)
#else
		for( int pos=0; pos<MAX_INVENTORY; pos++)
#endif
		{
			if ( lpObj->pInventory[pos].IsItem() == TRUE )
			{
				if ( lpObj->pInventory[pos].m_Type == MAKE_ITEMNUM((*(it)).m_iItemCategory,(*(it)).m_iItemCode) )
				{
					int iLEVEL = lpObj->pInventory[pos].m_Level;
					BYTE iOPTION1 = lpObj->pInventory[pos].m_Option1;
					BYTE iOPTION2 = lpObj->pInventory[pos].m_Option2;
					BYTE iOPTION3 = lpObj->pInventory[pos].m_Option3;
					BYTE iNEWOPTION = lpObj->pInventory[pos].m_NewOption;

					if ( iLEVEL != -1 && iLEVEL != (*(it)).m_iItemLevel)
					{
						continue;
					}

					if ( iOPTION1 != -1 && iOPTION1 != (*(it)).m_btOption1)
					{
						continue;
					}

					if ( iOPTION2 != -1 && iOPTION2 != (*(it)).m_btOption2)
					{
						continue;
					}

					if ( iOPTION3 != -1 && iOPTION3 != (*(it)).m_btOption3)
					{
						continue;
					}

					if ( iNEWOPTION != -1 && iNEWOPTION != (*(it)).m_btNewOption)
					{
						continue;
					}

					if ( (*(it)).m_iItemPosition != -1 && pos != (*(it)).m_iItemPosition)
					{
						continue;
					}

					LogAddTD("[Reset System] Force (Account:%s, Name:%s) to Delete Required Item in Pos:(%d) [Serial:%d]", lpObj->AccountID, lpObj->Name, pos, lpObj->pInventory[pos].m_Number);
							
					::gObjInventoryItemSet(lpObj->m_Index, pos, -1);
					::gObjInventoryDeleteItem(lpObj->m_Index, pos);
					::GCInventoryItemDeleteSend(lpObj->m_Index, pos, true);

					ItemCount++;

					if(ItemCount >= (*(it)).m_iItemNumber) //Prevent Multi Delete
					{
						break; //break the "for" of inventory not the vector "for"
					}
				}
			}
		}
	}
}

void CResetSystem::CheckUserCondition(LPOBJECTSTRUCT lpObj)
{
	if (lpObj->CloseCount > 0 || lpObj->CloseType >= 0)
	{
		return;
	}

	if(lpObj->m_bResetState == 0)
	{
		this->DBGetResetInfo(lpObj);
		return;
	}

	if(lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 6)
	{
		CGWarehouseUseEnd(lpObj->m_Index); //Lock the Vault
	}

	if(lpObj->m_ReqWarehouseOpen != false)
	{
		return;
	}

	if(lpObj->m_iResetNumber == -1)
	{
		return;
	}

	if(lpObj->m_bReqUpdateReset == true)
	{
		return;
	}

	if(lpObj->m_stateEngagePVP == PVP_USER_PLAYING)
		return;

	if(CHECK_BLOODCASTLE(lpObj->MapNumber)
#ifdef DEVILSQUARE_EXTEND_20050221
		|| CHECK_DEVILSQUARE(lpObj->MapNumber)
#else
		|| lpObj->MapNumber == 9
#endif
#ifdef CHAOSCASTLE_SYSTEM_20040408
		|| CHECK_CHAOSCASTLE(lpObj->MapNumber)
#endif
		)
	{
		return;
	}

	if((lpObj->Money < this->m_iZenPrice) || (this->m_bZenMultiply && lpObj->Money < this->m_iZenPrice*(lpObj->m_iResetNumber+1)))
	{
		char szMsg[256] = {0};

		sprintf(szMsg, "No tienen suficiente zen(%d)...", (this->m_bZenMultiply ? this->m_iZenPrice*(lpObj->m_iResetNumber+1) : this->m_iZenPrice));

		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}

	BOOL bError = TRUE;

	if(this->m_bResetSystemType == 1)
	{
		for ( std::vector<TABLE_RESET>::iterator it = this->m_vtTableReset.begin() ; it != this->m_vtTableReset.end() ; it++ )
		{
			if((*(it)).m_iVipCode != -1)
			{
				if(lpObj->m_AccountType != (*(it)).m_iVipCode)
				{
					continue;
				}
			}

			if(lpObj->m_iResetNumber < (*it).m_iMinResetNumber || lpObj->m_iResetNumber+1 > (*(it)).m_iResetNumber) //Dangerous
			{
				continue;
			}
			
			if((*(it)).m_iCharClassType != -1)
			{
				if(lpObj->Class != (*(it)).m_iCharClassType)
				{
					continue;
				}
			}

			if(lpObj->Level < (*(it)).m_iMinLevel)
			{
				continue;
			}

			if(lpObj->Level > (*(it)).m_iMaxLevel)
			{
				continue;
			}

			if((*(it)).m_bReqSpecialItem != FALSE)
			{
				bool FindItem = this->SearchResetRequiredItem(lpObj,(*(it)).m_iRuleIndex);

				if( FindItem != true)
				{
					bError = 2;
					continue;
				}
			}

			if((*(it)).m_iCheckInventory != FALSE)
			{
				BOOL Result = this->CheckUserEquipment(lpObj);

				if( Result != TRUE)
				{
					continue;
				}
			}

			//EnterCriticalSection(&this->m_cs);
			this->DBReqUpdateReset(lpObj, (*(it)).m_iRuleIndex, (*(it)).m_bClearStat, (*(it)).m_iLevelAfterReset, (*(it)).m_iLevelUpPoint, (*(it)).m_bReqSpecialItem, (*(it)).m_bBornPlace, lpObj->m_iResetNumber+1, (*(it)).m_bClearBuffState, (*(it)).m_bClearMagicList);
			bError = FALSE;
			break;
		}
	}
	else
	{
		for ( std::vector<NON_TABLE_RESET>::iterator it = this->m_vtNonTableReset.begin() ; it != this->m_vtNonTableReset.end() ; it++ )
		{
			if((*(it)).m_iVipCode != -1)
			{
				if(lpObj->m_AccountType != (*(it)).m_iVipCode)
				{
					continue;
				}
			}

			if((*(it)).m_iCharClassType != -1)
			{
				if(lpObj->Class != (*(it)).m_iCharClassType)
				{
					continue;
				}
			}

			if(lpObj->Level < (*(it)).m_iMinLevel)
			{
				continue;
			}

			if(lpObj->Level > (*(it)).m_iMaxLevel)
			{
				continue;
			}

			if((*(it)).m_bReqSpecialItem != FALSE)
			{
				bool FindItem = this->SearchResetRequiredItem(lpObj,(*(it)).m_iRuleIndex);

				if( FindItem != true)
				{
					bError = 2;
					continue;
				}
			}
			
			if((*(it)).m_iCheckInventory != FALSE)
			{
				BOOL Result = this->CheckUserEquipment(lpObj);

				if( Result != TRUE)
				{
					continue;
				}
			}

			//EnterCriticalSection(&this->m_cs);
			this->DBReqUpdateReset(lpObj, (*(it)).m_iRuleIndex, (*(it)).m_bClearStat, (*(it)).m_iLevelAfterReset, (*(it)).m_iLevelUpPoint, (*(it)).m_bReqSpecialItem, (*(it)).m_bBornPlace, lpObj->m_iResetNumber+1, (*(it)).m_bClearBuffState, (*(it)).m_bClearMagicList);
			bError = FALSE;
			break;
		}
	}

	if(bError == TRUE)
	{
		GCServerMsgStringSend("No tienes el nivel necesario para resetear!", lpObj->m_Index, 1); //[Reset System] Your current status doesn't meet the Requeriments. Try again later!
	}
}

void CResetSystem::ExecResetApply(LPOBJECTSTRUCT lpObj, int iRuleIndex, BOOL iClearStatPoint, int iLevelAfter, int iLevelUpPoint, BOOL iReqSpecialItem, BOOL iBornPlace, int iResetNumber, BOOL iClearAllBuffState, BOOL iClearMagicList)
{
	//EnterCriticalSection(&this->m_cs);

	lpObj->Level = iLevelAfter;
	lpObj->Experience = 0;

	if(iLevelUpPoint != -1)
	{
		lpObj->LevelUpPoint = iLevelUpPoint * iResetNumber;
	}

	lpObj->m_iResetNumber = iResetNumber;

	if(iClearStatPoint == TRUE)
	{
		this->UpdateUserStatusPoint(lpObj);
	}

	if(iBornPlace == TRUE)
	{
		this->SetUserBornPlace(lpObj,iRuleIndex,lpObj->Class);
	}

	if(iReqSpecialItem == TRUE)
	{
		this->DeleteResetRequiredItem(lpObj,iRuleIndex); //only one RuleIndex
	}

	if(iClearAllBuffState == TRUE)
	{
		this->ClearAllBuffStates(lpObj);
	}

	if(iClearMagicList == TRUE)
	{
		this->ClearMagicList(lpObj);
	}

	this->SetUserDefaultLifePlusMana(lpObj);
	this->CalculateEquipment(lpObj);
	this->FixUserExpInterface(lpObj);

	lpObj->m_bReqUpdateReset = false; //clear

	if(m_bZenMultiply)
		lpObj->Money -= this->m_iZenPrice*lpObj->m_iResetNumber;
	else
		lpObj->Money -= this->m_iZenPrice;

	GCMoneySend(lpObj->m_Index, lpObj->Money);

	GJSetCharacterInfo(lpObj, lpObj->m_Index);

	char szTemp[256];
	wsprintf(szTemp,"[RESET SYSTEM]: Tienes %d Reset(s).", lpObj->m_iResetNumber); //[Reset System] You currently have %d Reset(s).
	GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);


	//LeaveCriticalSection(&this->m_cs);
}

void CResetSystem::SetResetNPCData(NPC_DATA &npc)
{
	int iMonsterIndex = gObjAddMonster(npc.m_iMapIndex);

	if(iMonsterIndex >= 0)
	{
		MONSTER_ATTRIBUTE * MAttr = gMAttr.GetAttr(npc.m_iNpcIndex);
	
		if ( MAttr == NULL )
		{
			gObjDel(iMonsterIndex);
			return;
		}

		gObjSetMonster(iMonsterIndex, npc.m_iNpcIndex);
		gObj[iMonsterIndex].Live = TRUE;
		gObj[iMonsterIndex].Life = MAttr->m_Hp;
		gObj[iMonsterIndex].MaxLife = MAttr->m_Hp;
		gObj[iMonsterIndex].m_PosNum = -1;
		gObj[iMonsterIndex].X = npc.m_icX;
		gObj[iMonsterIndex].Y = npc.m_icY;
		gObj[iMonsterIndex].MTX = npc.m_icX;
		gObj[iMonsterIndex].MTY = npc.m_icY;
		gObj[iMonsterIndex].TX = npc.m_icX;
		gObj[iMonsterIndex].TY = npc.m_icY;
		gObj[iMonsterIndex].m_OldX = npc.m_icX;
		gObj[iMonsterIndex].m_OldY = npc.m_icY;
		gObj[iMonsterIndex].StartX = npc.m_icX;
		gObj[iMonsterIndex].StartY = npc.m_icY;
		gObj[iMonsterIndex].MapNumber = npc.m_iMapIndex;
		gObj[iMonsterIndex].m_MoveRange = 0;
		gObj[iMonsterIndex].Level = MAttr->m_Level;
		gObj[iMonsterIndex].Type = OBJTYPE_NPC;
		gObj[iMonsterIndex].MaxRegenTime = 1000;
		gObj[iMonsterIndex].Dir = npc.m_iDir;
		gObj[iMonsterIndex].RegenTime = GetTickCount();
		gObj[iMonsterIndex].m_Attribute = 0;
		gObj[iMonsterIndex].DieRegen = 0;

		LogAddTD("[Reset System] Npc Addition : Index(%d) NpcID(%d) MapID(%d) PosX(%d) PosY(%d)", iMonsterIndex, npc.m_iNpcIndex, npc.m_iMapIndex, npc.m_icX, npc.m_icY);

		this->m_mNpcData.insert(std::make_pair(iMonsterIndex,npc));
	}
}

void CResetSystem::ClearResetNPCData()
{
	for(NPC_DATA_MAP::iterator it = m_mNpcData.begin();it != m_mNpcData.end();)
	{
		LogAddTD("[Reset System] To Delete Npc : Index(%d)", it->first);
		gObjDel(it->first);
		it = m_mNpcData.erase(it);
	}
}

BOOL CResetSystem::GetNPCResetData(int iIndex)
{
	if(m_mNpcData.find(iIndex) != m_mNpcData.end())
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CResetSystem::CheckUserEquipment(LPOBJECTSTRUCT lpObj)
{
	//EnterCriticalSection(&this->m_cs);

	BOOL bFind = FALSE;

	for ( int pos = 0; pos < 11; pos++)
	{
		if(lpObj->pInventory[pos].IsItem() == TRUE)
		{
			bFind = TRUE;
		}
	}

	//LeaveCriticalSection(&this->m_cs);

	if(bFind == TRUE)
	{
		return FALSE;
	}

	return TRUE;
}

void CResetSystem::ClearMagicList(LPOBJECTSTRUCT lpObj) //Perfect
{
	int i;
	int n;

	for(i = 0; i < MAX_MAGIC; i++) //Clear all the magic first
	{
		if(lpObj->Magic[i].IsMagic() == TRUE )
		{
			::GCMagicListOneDelSend(lpObj->m_Index, i, lpObj->Magic[i].m_Skill, lpObj->Magic[i].m_Level, 0);
			lpObj->Magic[i].Clear();
		}
	}

	if ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_ELF || lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA ) //Re-create all the default Skills and Non_Scroll(Quest) Skill
	{
		gObjMagicAddEnergyCheckOnOff(FALSE);

		if ( lpObj->Class == CLASS_WIZARD )
		{
			gObjMagicAdd(lpObj, 17, 0);
		}

		::gObjMagicAddEnergyCheckOnOff(TRUE);
	}

	::GCMagicListMultiSend(lpObj); //Send the Info to MuClient

	for(n = 0; n < 11; n++) //Re-create the Equiped Weapon Skill
	{
		if(lpObj->pInventory[n].IsItem() == TRUE)
		{
			CItem * lpItem = &lpObj->pInventory[n];

			if(lpItem->m_Option1)
			{
				int s_pos = gObjWeaponMagicAdd(lpObj,lpItem->m_Special[0],lpItem->m_Level);

				if(s_pos >= 0)
				{
					::GCMagicListOneSend(lpObj->m_Index,s_pos,lpItem->m_Special[0],lpItem->m_Level,0);
				}
			}
		}
	}
}