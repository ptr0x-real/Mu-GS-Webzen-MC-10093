#include "StdAfx.h"
#include "user.h"
#include "OffExp.h"
#include "GameMain.h"
#include "ObjUseSkill.h"
#include "MagicInf.h"
#include "SProtocol.h"
#include "PartyClass.h"
#include "ObjAttack.h"
#include "GameServer.h"
#include "protocol.h"

#include "LogProc.h"
extern PartyClass		gParty;

cOffExp OffExp;

void cOffExp::LoadConfig()
{
	this->Enable = GetPrivateProfileInt("Common", "Enable", 1, OFF_EXP_DIR);
	this->KickFallen = GetPrivateProfileInt("Common", "KickFallen", 1, OFF_EXP_DIR);
	this->EnableTime = GetPrivateProfileInt("Common", "EnableTime", 0, OFF_EXP_DIR);
	this->TimeEnd = GetPrivateProfileInt("Common", "Time", 9999999, OFF_EXP_DIR);


}


void cOffExp::OffExpStart(int aIndex)
{
	if (!this->Enable) return;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if (this->CheckTerrain(lpObj))
	{
		CloseClientEx(aIndex);
		lpObj->OffExp = true;
		if (this->EnableTime) 
		{
			lpObj->OffExpTick = 0;
		}
	}
	else
	{
		GCServerMsgStringSend("[Ataque Automatico] No puedes utilizarlo en la ciudad. ", aIndex, 1);
	}
}

void cOffExp::TickTimes(int aIndex)
{
	if (!this->Enable) return;

	if (this->EnableTime)
	{
		LPOBJECTSTRUCT lpObj = &gObj[aIndex];
		lpObj->OffExpTick++;
		if (lpObj->OffExpTick >= this->TimeEnd)
		{
			LogAddTD("[OffExp] [%s][%s] PJ Descoenctado Tiempo agotado: [%d]", lpObj->AccountID, lpObj->Name, lpObj->m_Index);
			lpObj->OffExp = false;
			GJPUserClose2(lpObj->AccountID);
			gObjDel(aIndex);
			return;
		}
	}
}

void cOffExp::OffExpStart2(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if (this->CheckTerrain(lpObj))
	{
		lpObj->OffExp = true;
	}
	else
	{
		GCServerMsgStringSend("[Ataque Automatico] No puedes utilizarlo en la ciudad. ", aIndex, 1);
	}
}

void cOffExp::OffExpStart3(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if (this->CheckTerrain(lpObj))
	{
		lpObj->OffExp = false;
	}
	else
	{
		GCServerMsgStringSend("[Ataque Automático] No puedes utilizarlo en la ciudad. ", aIndex, 1);
	}
}


void cOffExp::MainFunction()
{
	if (!this->Enable) return;

	for (int n = 0; n < MAX_OBJECT; n++)
	{
		LPOBJECTSTRUCT lpObj = &gObj[n];

		if (lpObj->Connected > PLAYER_LOGGED &&	lpObj->Type == OBJTYPE_CHARACTER && lpObj->OffExp == true )
		{
			bool Attack = false;

			if (lpObj->Dexterity > 0 && lpObj->Dexterity <= 3000)
			{
				lpObj->OffExpSpeed++;
				if (lpObj->OffExpSpeed == 10)
				{
					Attack = true;
					lpObj->OffExpSpeed = 0;
				}
			}
			else if (lpObj->Dexterity > 3000 && lpObj->Dexterity <= 6000)
			{
				lpObj->OffExpSpeed++;
				if (lpObj->OffExpSpeed == 9)
				{
					Attack = true;
					lpObj->OffExpSpeed = 0;
				}
			}
			else if (lpObj->Dexterity > 6000 && lpObj->Dexterity <= 9000)
			{
				lpObj->OffExpSpeed++;
				if (lpObj->OffExpSpeed == 8)
				{
					Attack = true;
					lpObj->OffExpSpeed = 0;
				}
			}
			else if (lpObj->Dexterity > 9000 && lpObj->Dexterity <= 12000)
			{
				lpObj->OffExpSpeed++;
				if (lpObj->OffExpSpeed == 7)
				{
					Attack = true;
					lpObj->OffExpSpeed = 0;
				}
			}
			else if (lpObj->Dexterity > 12000 && lpObj->Dexterity <= 15000)
			{
				lpObj->OffExpSpeed++;
				if (lpObj->OffExpSpeed == 5)
				{
					Attack = true;
					lpObj->OffExpSpeed = 0;
				}
			}
			else if (lpObj->Dexterity > 15000 && lpObj->Dexterity <= 18000)
			{
				lpObj->OffExpSpeed++;
				if (lpObj->OffExpSpeed == 4)
				{
					Attack = true;
					lpObj->OffExpSpeed = 0;
				}
			}
			else if (lpObj->Dexterity > 18000 && lpObj->Dexterity <= 21000)
			{
				lpObj->OffExpSpeed++;
				if (lpObj->OffExpSpeed == 3)
				{
					Attack = true;
					lpObj->OffExpSpeed = 0;
				}
			}
			else if (lpObj->Dexterity > 21000 && lpObj->Dexterity <= 24000)
			{
				lpObj->OffExpSpeed++;
				if (lpObj->OffExpSpeed == 2)
				{
					Attack = true;
					lpObj->OffExpSpeed = 0;
				}
			}
			else if (lpObj->Dexterity > 24000)
			{
				Attack = true;
			}
			else
			{
				continue;
			}

			if (Attack == false)
			{
				continue;
			}

			if (!this->CheckTerrain(lpObj))
			{
				if (this->KickFallen) lpObj->OffExp = 0;
				LogAddTD("[OffExp] [%s][%s] Cierre porque mato: [%d]", lpObj->AccountID, lpObj->Name, lpObj->m_Index);
				GJPUserClose2(lpObj->AccountID);
				gObjDel(lpObj->m_Index);
				return;
			}

			switch (lpObj->Class)
			{
			case 0:	this->DarkWizardOff(lpObj);		break;
			case 1:	this->DarkKnightOff(lpObj);		break;
			case 2:	this->FairyElfOff(lpObj);		break;
			case 3:	this->MagicGladiatorOff(lpObj);	break;
			case 4:	this->DarkLordOff(lpObj);		break;
			}
		}
	}
}

//=====================================
//	Dark Wizard Offline Experience
//=====================================
void cOffExp::DarkWizardOff(LPOBJECTSTRUCT lpObj)
{
	int SearchSkill = 0;
	CMagicInf * lpMagic;

	if (gObjGetMagicSearch(lpObj, 9) != NULL) SearchSkill = 9;
	else if (gObjGetMagicSearch(lpObj, 445) != NULL) SearchSkill = 445;
	else if (gObjGetMagicSearch(lpObj, 446) != NULL) SearchSkill = 446;
	else if (gObjGetMagicSearch(lpObj, 447) != NULL) SearchSkill = 447;
	else if (gObjGetMagicSearch(lpObj, 448) != NULL) SearchSkill = 448;
	else if (gObjGetMagicSearch(lpObj, 449) != NULL) SearchSkill = 449;
	else return;

	lpMagic = gObjGetMagicSearch(lpObj, SearchSkill);
	if (lpMagic == NULL) return;

	gObjUseSkill.UseSkill(lpObj->m_Index, lpMagic, lpObj->X, lpObj->Y, 0, 0, -1);

	int dis;
	int tObjNum;
	BYTE attr;
	for (int n = 0; n<MAX_VIEWPORT_MONSTER; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].number;
		if (lpObj->VpPlayer2[n].state == 0) continue;
		if (tObjNum >= 0)
		{
			if (gObj[tObjNum].Type == OBJTYPE_MONSTER)
			{
				attr = MapC[gObj[tObjNum].MapNumber].GetAttr(gObj[tObjNum].X, gObj[tObjNum].Y);
				if ((attr & 1) != 1)
				{
					dis = gObjCalDistance(lpObj, &gObj[tObjNum]);
					if (6 >= dis)
					{
						gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, 0);
					}
				}
			}
		}
	}

	this->UseMana(lpObj->m_Index);
}
//=====================================
//	Dark Knight Offline Experience
//=====================================
void cOffExp::DarkKnightOff(LPOBJECTSTRUCT lpObj)
{
	int SearchSkill = 0;
	CMagicInf * lpMagic;

	if (gObjGetMagicSearch(lpObj, 41) != NULL) SearchSkill = 41;
	else if (gObjGetMagicSearch(lpObj, 455) != NULL) SearchSkill = 455;
	else if (gObjGetMagicSearch(lpObj, 456) != NULL) SearchSkill = 456;
	else if (gObjGetMagicSearch(lpObj, 457) != NULL) SearchSkill = 457;
	else if (gObjGetMagicSearch(lpObj, 458) != NULL) SearchSkill = 458;
	else if (gObjGetMagicSearch(lpObj, 459) != NULL) SearchSkill = 459;
	else return;

	lpMagic = gObjGetMagicSearch(lpObj, SearchSkill);
	if (lpMagic == NULL) return;

	gObjUseSkill.UseSkill(lpObj->m_Index, lpMagic, lpObj->X, lpObj->Y, 0, 0, -1);

	int dis;
	int tObjNum;
	BYTE attr;
	for (int n = 0; n<MAX_VIEWPORT_MONSTER; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].number;
		if (lpObj->VpPlayer2[n].state == 0) continue;
		if (tObjNum >= 0)
		{
			if (gObj[tObjNum].Type == OBJTYPE_MONSTER)
			{
				attr = MapC[gObj[tObjNum].MapNumber].GetAttr(gObj[tObjNum].X, gObj[tObjNum].Y);
				if ((attr & 1) != 1)
				{
					dis = gObjCalDistance(lpObj, &gObj[tObjNum]);
					if (2 >= dis)
					{
						gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, 0);
					}
				}
			}
		}
	}

	this->UseMana(lpObj->m_Index);
}

bool SetBuffPartyUser(int UserIndex, CMagicInf *lpMagic)
{
	if( gParty.GetPartyCount(gObj[UserIndex].PartyNumber) < 0 ) 
	{
		return false;
	}
	// ----
	for(int i = 0; i < MAX_PARTYUSER; i++)
	{
		int PartyNumIndex = gParty.m_PartyS[gObj[i].PartyNumber].Number[i];
		// ----
		if( PartyNumIndex >= 0 )
		{
			gObjUseSkill.RunningSkill(PartyNumIndex, PartyNumIndex, lpMagic, 0);		
		}
	}
	// ----
	return true;
}

//=====================================
//	Fairy Elf Offline Experience
//=====================================
void cOffExp::FairyElfOff(LPOBJECTSTRUCT lpObj)
{
	int SearchSkill = 0;
	CMagicInf * lpMagic;

	lpMagic = gObjGetMagicSearch( lpObj, 24);	//Multi-Shot 235
	if(lpMagic==NULL) gObjUseSkill.UseSkill(lpObj->m_Index, lpObj->m_Index, lpMagic);

	if(gObjGetMagicSearch( lpObj, 24) != NULL) SearchSkill = 24;
	else if(gObjGetMagicSearch( lpObj, 414) != NULL) SearchSkill = 414;
	else if(gObjGetMagicSearch( lpObj, 418) != NULL) SearchSkill = 418;
	else return;

	lpMagic = gObjGetMagicSearch( lpObj, SearchSkill);
	if(lpMagic==NULL) gObjUseSkill.UseSkill(lpObj->m_Index, lpObj->m_Index, lpMagic);

	int NumDir[6] = {0, 50, 100, 150, 200, 250 };
	int StartDir = NumDir[rand()%6];
	gObjUseSkill.UseSkill(lpObj->m_Index, lpMagic,lpObj->X, lpObj->Y,StartDir,0,-1);
	

	int dis;
	int tObjNum;
	BYTE attr;
	for (int n = 0; n<MAX_VIEWPORT_MONSTER; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].number;
		if (lpObj->VpPlayer2[n].state == 0) continue;
		if (tObjNum >= 0)
		{
			if (gObj[tObjNum].Type == OBJTYPE_MONSTER)
			{
				attr = MapC[gObj[tObjNum].MapNumber].GetAttr(gObj[tObjNum].X, gObj[tObjNum].Y);
				if ((attr & 1) != 1)
				{
					dis = gObjCalDistance(lpObj, &gObj[tObjNum]);
					if (5 >= dis)
					{
						gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, 0);
					}
				}
			}
		}
	}
	
	this->UseMana(lpObj->m_Index);
}

//=====================================
//	Magic Gladiator Offline Experience
//=====================================
void cOffExp::MagicGladiatorOff(LPOBJECTSTRUCT lpObj)
{
	int skill;
	int NormDis;
	CMagicInf * lpMagic;
	int SearchSkill = 0;

	if (lpObj->pInventory[0].m_Type >= ITEMGET(5, 0) && lpObj->pInventory[0].m_Type < ITEMGET(6, 0))
	{
		if (gObjGetMagicSearch(lpObj, 9) != NULL) SearchSkill = 9;
		else if (gObjGetMagicSearch(lpObj, 530) != NULL) SearchSkill = 530;
		else if (gObjGetMagicSearch(lpObj, 531) != NULL) SearchSkill = 531;
		else if (gObjGetMagicSearch(lpObj, 532) != NULL) SearchSkill = 532;
		else if (gObjGetMagicSearch(lpObj, 533) != NULL) SearchSkill = 533;
		else if (gObjGetMagicSearch(lpObj, 534) != NULL) SearchSkill = 534;
		else return;
		skill = SearchSkill;
		NormDis = 6;
	}
	else
	{
		if (gObjGetMagicSearch(lpObj, 41) != NULL) SearchSkill = 41;
		else if (gObjGetMagicSearch(lpObj, 495) != NULL) SearchSkill = 495;
		else if (gObjGetMagicSearch(lpObj, 496) != NULL) SearchSkill = 496;
		else if (gObjGetMagicSearch(lpObj, 497) != NULL) SearchSkill = 497;
		else if (gObjGetMagicSearch(lpObj, 498) != NULL) SearchSkill = 498;
		else if (gObjGetMagicSearch(lpObj, 499) != NULL) SearchSkill = 499;
		else return;
		skill = SearchSkill;
		NormDis = 2;
	}

	lpMagic = gObjGetMagicSearch(lpObj, skill);	//Twisting Slash 41
	if (lpMagic == NULL) return;

	gObjUseSkill.UseSkill(lpObj->m_Index, lpMagic, lpObj->X, lpObj->Y, 0, 0, -1);

	int dis;
	int tObjNum;
	BYTE attr;
	for (int n = 0; n<MAX_VIEWPORT_MONSTER; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].number;
		if (lpObj->VpPlayer2[n].state == 0) continue;
		if (tObjNum >= 0)
		{
			if (gObj[tObjNum].Type == OBJTYPE_MONSTER)
			{
				attr = MapC[gObj[tObjNum].MapNumber].GetAttr(gObj[tObjNum].X, gObj[tObjNum].Y);
				if ((attr & 1) != 1)
				{
					dis = gObjCalDistance(lpObj, &gObj[tObjNum]);
					if (NormDis >= dis)
					{
						gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, 0);
					}
				}
			}
		}
	}

	this->UseMana(lpObj->m_Index);
}
//=====================================
//	Dark Lord Offline Experience
//=====================================
void cOffExp::DarkLordOff(LPOBJECTSTRUCT lpObj)	//Work
{
	int Target = this->SearchTarget(lpObj);

	if (Target == -1) return;

	int SearchSkill = 0;
	CMagicInf * lpMagic;

	if (gObjGetMagicSearch(lpObj, 61) != NULL) SearchSkill = 61;
	else if (gObjGetMagicSearch(lpObj, 520) != NULL) SearchSkill = 520;
	else if (gObjGetMagicSearch(lpObj, 521) != NULL) SearchSkill = 521;
	else if (gObjGetMagicSearch(lpObj, 522) != NULL) SearchSkill = 522;
	else if (gObjGetMagicSearch(lpObj, 523) != NULL) SearchSkill = 523;
	else if (gObjGetMagicSearch(lpObj, 524) != NULL) SearchSkill = 524;
	else return;

	lpMagic = gObjGetMagicSearch(lpObj, SearchSkill);
	if (lpMagic == NULL) return;

	gObjUseSkill.UseSkill(lpObj->m_Index, Target, lpMagic); //

	this->UseMana(lpObj->m_Index);
}


int cOffExp::SearchTarget(LPOBJECTSTRUCT lpObj)
{
	int dis;
	int searchtarget = -1;
	int tObjNum;
	BYTE attr;
	int TargetIndex[MAX_VIEWPORT_MONSTER];
	int TargetCount = 0;
	int IndexNumber;
	for (int n = 0; n<MAX_VIEWPORT_MONSTER; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].number;
		if (lpObj->VpPlayer2[n].state == 0) continue;
		if (tObjNum >= 0)
		{
			if (gObj[tObjNum].Type == OBJTYPE_MONSTER)
			{
				attr = MapC[gObj[tObjNum].MapNumber].GetAttr(gObj[tObjNum].X, gObj[tObjNum].Y);
				if ((attr & 1) != 1)
				{
					dis = gObjCalDistance(lpObj, &gObj[tObjNum]);
					if (6 >= dis)
					{
						TargetIndex[TargetCount] = tObjNum;
						TargetCount++;
					}
				}
			}
		}
	}

	if (TargetCount == 0)	return -1;

	IndexNumber = rand() % TargetCount;

	searchtarget = TargetIndex[IndexNumber];

	return searchtarget;
}

void cOffExp::UseMana(int aIndex)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if (lpObj->Mana > 100) return;

	int pos;

	pos = gObjGetManaItemPos(aIndex);

	if (pos == -1) return;

	if (lpObj->pInventory[pos].m_Type == ITEMGET(14, 4) ||
		lpObj->pInventory[pos].m_Type == ITEMGET(14, 5) ||
		lpObj->pInventory[pos].m_Type == ITEMGET(14, 6))
	{
		int tMana = (lpObj->pInventory[pos].m_Value * 10) - (gObj[aIndex].Level);

		if (tMana < 0)
		{
			tMana = 0;
		}

		switch (lpObj->pInventory[pos].m_Type)
		{
		case ITEMGET(14, 4):
			tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana)) * 20 / 100;
			break;
		case ITEMGET(14, 5):
			tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana)) * 30 / 100;
			break;
		case ITEMGET(14, 6):
			tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana)) * 40 / 100;
			break;
		}

		gObj[aIndex].Mana += tMana;

		if (gObj[aIndex].Mana > (gObj[aIndex].MaxMana + gObj[aIndex].AddMana - 1.0f))
		{
			gObj[aIndex].Mana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
		}

		GCManaSend(aIndex, (int)gObj[aIndex].Mana, 0xFF, 0, (int)gObj[aIndex].BP);

		if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
		{
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
		}
	}
}

bool cOffExp::CheckTerrain(LPOBJECTSTRUCT lpObj)
{
	BYTE attr;
	attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

	if ((attr & 1) != 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void cOffExp::Close(char * AccountID)
{
	for (int i = OBJ_STARTUSERINDEX; i < MAX_OBJECT; i++)
	{
		LPOBJECTSTRUCT lpUser = &gObj[i];
		// ----
		if (lpUser->Connected != PLAYER_PLAYING || !lpUser->OffExp || strcmp(AccountID, lpUser->AccountID))
		{
			continue;
		}
		// ----
		GJPUserClose2(lpUser->AccountID);
		gObjDel(i);
		lpUser->OffExp = false;
	}
}

bool cOffExp::Update(int UserIndex, BYTE Type)
{
	LPOBJECTSTRUCT lpUser = &gObj[UserIndex];
	// ----
	if (!lpUser->OffExp)
	{
		return false;
	}
	// ----
	if (Type == 0)
	{
		lpUser->CheckTick = GetTickCount();
	}
	else if (Type == 1)
	{
		lpUser->ConnectCheckTime = GetTickCount();
	}
	// ----
	return true;
}



