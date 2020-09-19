#include "StdAfx.h"
#include "Include\Tokenizer.h"
#include "TNotice.h"

SOBRE SobreDrop[1000];

Sobrevivencia Sobre;

SobrePlayer SbPlayer[MAX_OBJECT];

bool Sobrevivencia::LoadSettings()
{
	Tokenizer          token;
	TokenizerGroup     group;
	TokenizerSection   Section;

	token.ParseFile(std::string("./Data/Sobrevivencia.txt"), group);

	if(group.GetSection(0, Section))
	{
		Sobre.SectionCount = Section.RowCount;

		for(int i = 0; i < Section.RowCount; i++)
		{
			SobreDrop[i].ItemType = Section.Rows[i].GetInt(0);
			SobreDrop[i].ItemNr	= Section.Rows[i].GetInt(1);
			SobreDrop[i].ItemLevel = Section.Rows[i].GetInt(2);
			SobreDrop[i].ItemLuck = Section.Rows[i].GetInt(3);
			SobreDrop[i].ItemSkill = Section.Rows[i].GetInt(4);
			SobreDrop[i].ItemDur	= Section.Rows[i].GetInt(5);
			SobreDrop[i].ItemOpt = Section.Rows[i].GetInt(6);
			SobreDrop[i].ItemExc = Section.Rows[i].GetInt(7);
		}
	}

	  return true;
}

void Sobrevivencia::ResetEvent()
{
	InitTime = 0;
	InitSeconds = 0;
	gs_cmd.Contplayer = 0;
	gs_cmd.Sobrevivencia = 0;
	gs_cmd.Sobreblock = 0;

	Activated = false;

	for (int x = OBJECT_MIN; x < MAX_OBJECT; x++)
	{
		SbPlayer[x].pWaiting = false;
		SbPlayer[x].inSobreEvent = 0;
	}
}

void Sobrevivencia::Init(int aIndex, char* msg) 
{
	int TempTime = atoi(msg);
	TNotice notice;

	if(TempTime < 1)
	{
		MsgOutput(aIndex, "Error en la configuracion de evento!");
		return;
	}
	
	if(func.VerificarGM(aIndex) == false)
	{
		MsgOutput(aIndex, "Exclusivo apenas para gamemaster.");
		return;
	}
	
	InitTime = TempTime * 60;

	if(Activated == true)
	{
		MsgOutput(aIndex, "Sobrevivencia ya esta activado.");
		return;
	}
	else if(SbPlayer[aIndex].inSobreEvent != 0)
	{
		MsgOutput(aIndex, "Usted ya esta ocupado.");
		return;
	}
	else
	{
		Activated = true;
		SbPlayer[aIndex].inSobreEvent = 2;
		notice.SendToAllUser("%s inicio el Sobrevivencia", gObj[aIndex].Name);
		notice.SendToAllUser(gs_cmd.SobreNotice);
	}
}

void Sobrevivencia::SobreStart()
{

	TNotice notice;

	if(Activated = true && InitTime > 0)
	{
		InitSeconds++;
	}

	int TempRest;

	TempRest = InitSeconds == (InitTime - 59) ? 1 : InitSeconds == (InitTime - 119) ? 2 : InitSeconds == (InitTime - 179) ? 3 : 4;
	
	if (InitSeconds == (InitTime - 59) || InitSeconds == (InitTime - 119) || InitSeconds == (InitTime - 179) || InitSeconds == (InitTime - 239))
	{
		notice.SendToAllUser("Sobrevivencia iniciara en %d %s",TempRest,(TempRest == 1 ? "Minuto" : "Minutos"));
	}

	if(InitSeconds == InitTime && Activated == true)
	{
		if(gs_cmd.Contplayer <= 1)
		{
			notice.SendToAllUser("El evento no puede ser iniciado con %s Jugadores!",(gs_cmd.Contplayer == 1 ? "1" : "0"));
			ResetEvent();
			return;
		}
		else
		{
			Activated = false;
			InitSeconds = 0;
			InitTime = 0;

			notice.SendToAllUser("Sobrevivencia Ha Iniciado!");
			gs_cmd.Sobreblock = 0;
			gs_cmd.Sobrevivencia = 1;
		}
	}
	
}

void Sobrevivencia::Ir(int aIndex)
{
	if(Activated == false)
	{
		MsgOutput(aIndex, "Sobrevivencia no esta activado.");
		return;
	}
	else if(gObj[aIndex].Level < 50) 
    { 
		MsgOutput(aIndex, "Usted notiene el level necessário.");
        return; 
    } 
	else if(SbPlayer[aIndex].inSobreEvent != 0)
	{
		MsgOutput(aIndex, "Usted ya esta %s", (SbPlayer[aIndex].inSobreEvent == 1) ? "no evento." : "ocupado.");
		return;
	}
	else if(SbPlayer[aIndex].pWaiting == true)
	{
		MsgOutput(aIndex, "Usted ya esta esperando...");
		return;
	}
	else
	{
		SbPlayer[aIndex].pWaiting = true;
		SbPlayer[aIndex].inSobreEvent = 1;
		gs_cmd.Sobreblock = 1;
		gs_cmd.Contplayer++;
		gObj[aIndex].m_PK_Level = 6;
		gObjTeleport(aIndex, 1, 153, 191);
		GCPkLevelSend(aIndex,gObj[aIndex].m_PK_Level);

		MsgOutput(aIndex,"Usted entro en Sobrevivencia");
		MsgOutput(aIndex,"Este atento que va a comenzar!");
	}
}

bool Sobrevivencia::Dead(OBJECTSTRUCT* lpObj, OBJECTSTRUCT* tObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill, int iShieldDamage)
{
	int aIndex = lpObj->m_Index;
	int tIndex = tObj->m_Index;
	TNotice notice;

	if(lpObj->MapNumber == 1 && lpObj->Type == 1 && tObj->Type == 1 && gs_cmd.Sobrevivencia == 1)
	{
		if(lpObj->Life <= 0 && lpObj->Live != 0)
		{
			gs_cmd.Contplayer--;
			notice.SendToAllUser("[%s] Eliminado(a) de Sobrevivencia!",lpObj->Name);
			SbPlayer[aIndex].inSobreEvent = 0;
			SbPlayer[aIndex].pWaiting = false;
			lpObj->m_PK_Level = 3;
			gObjTeleport(lpObj->m_Index, 0, 125, 125);
			GCPkLevelSend(aIndex,gObj[aIndex].m_PK_Level);

			lpObj->DieRegen = 1;

			if(gs_cmd.Contplayer == 1)
			{
				notice.SendToAllUser("[%s] Gano el Sobrevivencia!",tObj->Name);

				//int Sorteado = rand() % Sobre.SectionCount;

				//DWORD Item = ITEMGET(SobreDrop[Sorteado].ItemType,SobreDrop[Sorteado].ItemNr);
	            //ItemSerialCreateSend(tIndex,gObj[tIndex].MapNumber,gObj[tIndex].X,gObj[tIndex].Y,Item,SobreDrop[Sorteado].ItemLevel,SobreDrop[Sorteado].ItemDur,SobreDrop[Sorteado].ItemSkill,SobreDrop[Sorteado].ItemLuck,SobreDrop[Sorteado].ItemOpt,tIndex,SobreDrop[Sorteado].ItemExc);
			    
				SbPlayer[tIndex].inSobreEvent = 0;
			    SbPlayer[tIndex].pWaiting = false;
				gs_cmd.Contplayer = 0;
			    gs_cmd.Sobrevivencia = 0;
				tObj->m_PK_Level = 3;
				GCPkLevelSend(tIndex,gObj[tIndex].m_PK_Level);
			}

			GCDiePlayerSend(lpObj, lpObj->m_Index, 0xFF, lpObj->m_Index);
		    return true;

		}
	}

	return false;
}
