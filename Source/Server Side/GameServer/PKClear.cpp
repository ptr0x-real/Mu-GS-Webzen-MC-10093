// ------------------------------
// PKClearSystem.cpp for GameServer 97D. ( lines)
// Author: HermeX
// Last Modified Date: 2011-01-05
// Comment: 
// ------------------------------
#include "stdafx.h"
#include "PKClear.h"
#include "LogProc.h"
#include "LogToFile.h"
#include "Include\readscript.h"
#include "user.h"
#include "GameMain.h"
#include "Common\winutil.h"
#include "BloodCastle.h"
#include "DevilSquare.h"

#include "MonsterAttr.h"

#include "ChaosCastle.h"

extern CMonsterAttr	gMAttr;

CPkClearSystem g_PkClearSystem;

CPkClearSystem::CPkClearSystem()
{
	this->m_bUsePkClearSystem = FALSE;
	this->Clear();
}

CPkClearSystem::~CPkClearSystem()
{
}

void CPkClearSystem::Clear()
{
	this->ClearPkClearNPCData();
	for(int i= 0; i < MAX_MESSAGE; i++) //Clear Messages
	{
		this->lpMessage[i] = NULL;
	}
}

void CPkClearSystem::Init()
{
	if(this->m_bUsePkClearSystem == FALSE || this->m_mNpcData.size() == 0)
	{
		LogAddC(2,"[PkClear System] CPkClearSystem::Init() - PkClear is Disable or NPC Setting is empty (m_mNpcData is Zero)");
		this->m_bHasData = FALSE;
		return;
	}

	this->m_bHasData = TRUE;
}

BOOL CPkClearSystem::Load(LPSTR lpszFileName)
{
	this->Clear();

	if ( !lpszFileName || !strcmp(lpszFileName , "") )
	{
		MsgBox("[PkClear System] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[PkClear System] Info file Load Fail [%s]", lpszFileName);

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
				if ( type == 0 ) //PkClear Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					this->m_bUsePkClearSystem = TokenNumber > 0;
					Token = GetToken();
					this->m_iZenPrice	= TokenNumber;
					Token = GetToken();
					this->m_bPriceType	= TokenNumber > 0;
				}

				else if ( type == 1 ) //Message Settings
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

				else if ( type == 2 ) //NPC Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					NPC_DATA_PKCLEAR NpcData;

					NpcData.m_iNpcIndex = TokenNumber;

					Token = GetToken();
					NpcData.m_iMapIndex = TokenNumber;

					Token = GetToken();
					NpcData.m_icX = TokenNumber;

					Token = GetToken();
					NpcData.m_icY = TokenNumber;

					Token = GetToken();
					NpcData.m_iDir = TokenNumber;

					this->SetPkClearNPCData(NpcData);
				}
			}
		}

		fclose(SMDFile);
		LogAdd("[PkClear System] %s file load!", lpszFileName);

		this->Init(); //check settings from the file
	}	// __try
	catch ( DWORD )
	{
		MsgBox("[PkClear System] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return true;
}

void CPkClearSystem::CheckUserCondition(LPOBJECTSTRUCT lpObj)
{
	if (lpObj->CloseCount > 0 || lpObj->CloseType >= 0)
	{
		return;
	}

	if(this->m_bHasData == FALSE)
	{
		GCServerMsgStringSend(this->lpMessage[0], lpObj->m_Index, 1); //The PKClear System is currently disabled
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

	if(lpObj->m_stateEngagePVP == PVP_USER_PLAYING)
		return;

	if(CHECK_BLOODCASTLE(lpObj->MapNumber) || 
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		CHECK_DEVILSQUARE(lpObj->MapNumber)
#else
		lpObj->MapNumber == 9 
#endif
#ifdef CHAOSCASTLE_SYSTEM_20040408
		|| CHECK_CHAOSCASTLE(lpObj->MapNumber)	// 카오스캐슬에 접속하지 않은 사람들만 골라서 보낸다.
#endif
		)
	{
		return;
	}

	if((this->m_bPriceType && lpObj->Money < this->m_iZenPrice*lpObj->m_PK_Count) || lpObj->Money < this->m_iZenPrice)
	{
		GCServerMsgStringSend(this->lpMessage[3], lpObj->m_Index, 1);
		return;
	}

	if(lpObj->m_PK_Level <= 3)
	{
		GCServerMsgStringSend(this->lpMessage[1], lpObj->m_Index, 1); //You are not a killer
		return;
	}

	lpObj->m_PK_Level = 3;
	lpObj->m_PK_Time = 0;

	if(this->m_bPriceType)
		lpObj->Money -= this->m_iZenPrice*lpObj->m_PK_Count;
	else
		lpObj->Money -= this->m_iZenPrice;

	lpObj->m_PK_Count = 0;

	GCMoneySend(lpObj->m_Index, lpObj->Money);
	GCPkLevelSend(lpObj->m_Index,lpObj->m_PK_Level);
	GCServerMsgStringSend(this->lpMessage[2], lpObj->m_Index, 1); //You killer status has been removed
}

void CPkClearSystem::SetPkClearNPCData(NPC_DATA_PKCLEAR &npc)
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

		LogAddTD("[PkClear System] Npc Addition : Index(%d) NpcID(%d) MapID(%d) PosX(%d) PosY(%d)", iMonsterIndex, npc.m_iNpcIndex, npc.m_iMapIndex, npc.m_icX, npc.m_icY);

		this->m_mNpcData.insert(std::make_pair(iMonsterIndex,npc));
	}
}

void CPkClearSystem::ClearPkClearNPCData()
{
	for(NPC_DATA_MAP_PKCLEAR::iterator it = m_mNpcData.begin();it != m_mNpcData.end();)
	{
		LogAddTD("[PkClear System] To Delete Npc : Index(%d)", it->first);
		gObjDel(it->first);
		it = m_mNpcData.erase(it);
	}
}

BOOL CPkClearSystem::GetNPCPkClearData(int iIndex)
{
	if(m_mNpcData.find(iIndex) != m_mNpcData.end())
	{
		return TRUE;
	}

	return FALSE;
}