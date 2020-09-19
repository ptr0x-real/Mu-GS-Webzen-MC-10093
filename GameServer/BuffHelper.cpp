#include "stdafx.h"
#include "BuffHelper.h"
#include "LogProc.h"
#include "LogToFile.h"
#include "..\include\readscript.h"
#include "user.h"
#include "GameMain.h"
#include "..\common\winutil.h"
#include "BloodCastle.h"
#include "DevilSquare.h"

#include "MonsterAttr.h"

#include "ObjUseSkill.h"

#include "TNotice.h"

#include "ChaosCastle.h"

extern CMonsterAttr	gMAttr;

CBuffHelper g_BuffHelper;

CBuffHelper::CBuffHelper()
{
	this->m_bUseBuffHelper = false;
	this->Clear();
}

void CBuffHelper::Clear()
{
	this->ClearNPCData();
}

void CBuffHelper::Init()
{
	if(this->m_bUseBuffHelper == false || this->m_mNpcData.size() == 0)
	{
		LogAddC(2,"[Buff Helper System] CBuffHelper::Init() - Buff Helper is Disable or NPC Setting is empty (m_mNpcData is Zero)");
		this->m_bHasData = FALSE;
		return;
	}

	this->m_bHasData = TRUE;
}

BOOL CBuffHelper::Load(LPSTR lpszFileName)
{
	this->Clear();

	if ( !lpszFileName || !strcmp(lpszFileName , "") )
	{
		MsgBox("[Buff Helper System] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[Buff Helper System] Info file Load Fail [%s]", lpszFileName);

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

					this->m_bUseBuffHelper = TokenNumber;

					Token = GetToken();
					this->m_iMaxLevel = TokenNumber;

					Token = GetToken();
					this->m_iNpcEnergy = TokenNumber;

					Token = GetToken();
					this->m_iAuraTime = TokenNumber;
				}

				else if ( type == 1 ) //NPC Settings
				{
					Token = GetToken();

					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					NPC_DATA_HELPER NpcData;

					NpcData.m_iNpcIndex = TokenNumber;

					Token = GetToken();
					NpcData.m_iMapIndex = TokenNumber;

					Token = GetToken();
					NpcData.m_icX = TokenNumber;

					Token = GetToken();
					NpcData.m_icY = TokenNumber;

					Token = GetToken();
					NpcData.m_iDir = TokenNumber;

					this->SetNPCData(NpcData);
				}
			}
		}

		fclose(SMDFile);
		LogAdd("[Buff Helper System] %s file load!", lpszFileName);

		this->Init(); //check settings from the file
	}	// __try
	catch ( DWORD )
	{
		MsgBox("[Buff Helper System] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return true;
}

void CBuffHelper::CheckUserCondition(LPOBJECTSTRUCT lpObj, int aNpcIndex)
{
	if (lpObj->CloseCount > 0 || lpObj->CloseType >= 0)
	{
		return;
	}

	if(this->m_bHasData == FALSE)
	{
		GCServerMsgStringSend("Buff Helper Disabled", lpObj->m_Index, 1); //The PKClear System is currently disabled
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

	if(lpObj->Level > this->m_iMaxLevel)
	{
		PMSG_NOTICE msgNotice;

		TNotice::MakeNoticeMsgEx((LPVOID)&msgNotice, 0, "Buff Nivel Maximo: %d", this->m_iMaxLevel);

		TNotice::SendNoticeToUser(lpObj->m_Index, (LPVOID)&msgNotice);
		return;
	}

	gObjUseSkill.SkillAttack(aNpcIndex, lpObj->m_Index, 7);
	gObjUseSkill.SkillDefense(aNpcIndex, lpObj->m_Index, 7);
}

void CBuffHelper::SetNPCData(NPC_DATA_HELPER &npc)
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
		gObj[iMonsterIndex].m_bBuffHelper = true;

		gObj[iMonsterIndex].Energy = this->m_iNpcEnergy;

		LogAddTD("[Buff Helper System] Npc Addition : Index(%d) NpcID(%d) MapID(%d) PosX(%d) PosY(%d)", iMonsterIndex, npc.m_iNpcIndex, npc.m_iMapIndex, npc.m_icX, npc.m_icY);

		this->m_mNpcData.insert(std::make_pair(iMonsterIndex,npc));
	}
}

void CBuffHelper::ClearNPCData()
{
	for(NPC_DATA_MAP_HELPER::iterator it = m_mNpcData.begin();it != m_mNpcData.end();)
	{
		LogAddTD("[Buff Helper System] To Delete Npc : Index(%d)", it->first);
		gObjDel(it->first);
		it = m_mNpcData.erase(it);
	}
}

BOOL CBuffHelper::GetNPCData(int iIndex)
{
	if(m_mNpcData.find(iIndex) != m_mNpcData.end())
	{
		return TRUE;
	}

	return FALSE;
}