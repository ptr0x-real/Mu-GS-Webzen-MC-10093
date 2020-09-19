//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// # DARK PLUGIN - POWERED BY FIRE TEAM
// # GAME SERVER: 99.60T (C) WEBZEN.
// # VERSÃO: 1.0.0.0
// # Autor: Caron22
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "StdAfx.h"
#include "Manager.h"
#include "DSProtocol.h"
#include "..\include\Tokenizer.h"

SORTE_PREMIO aSORTEO[1000];
SORTEO_CUSTOM eSORTEO;
char Connectedsa[500];

void SORTEO_CUSTOM::Sorteo(void * lpParam)
{

	Tokenizer          token;
	TokenizerGroup     group;
	TokenizerSection   Section;

	token.ParseFile(std::string(CFG_SORTEO), group);
	
	if(group.GetSection(0, Section))
	{	
		eSORTEO.elSORTEO = Section.Rows[0].GetInt(0);
	    eSORTEO.Sorteo_Start_Time = Section.Rows[0].GetInt(1);
	}
	if(group.GetSection(1, Section))
	{	

		for(int i = 0; i < Section.RowCount; i++)
		{
		
	    aSORTEO[i].Type = Section.Rows[i].GetInt(0);
	    aSORTEO[i].Index = Section.Rows[i].GetInt(1);
		aSORTEO[i].ItemLevel = Section.Rows[i].GetInt(2);
	    aSORTEO[i].ItemDur = Section.Rows[i].GetInt(3);
	    aSORTEO[i].ItemSkill = Section.Rows[i].GetInt(4);
		aSORTEO[i].ItemLuck = Section.Rows[i].GetInt(5);
	    aSORTEO[i].ItemOpt = Section.Rows[i].GetInt(6);
		aSORTEO[i].ItemExc = Section.Rows[i].GetInt(7);
		}

	}

	while(true)
	{
		char Mensagem[58];

		if(eSORTEO.elSORTEO == 0)
		{
			_endthread();
		}

		Sleep(eSORTEO.Sorteo_Start_Time*30000);
		//Sleep(30000); para test
		int Sec = 0;
		for (int i = OBJECT_MIN; i < MAX_OBJECT; i++)
		{

			if (gObj[i].Connected >= 3)
			{
				eSORTEO.Received[Sec++] = i; // Lista todos los usuarios online			
			}
		}

		if (Sec > 0)
		{
			
			int Index = rand() % Sec; //Elige un ganador
			int Target = eSORTEO.Received[Index];
			int ctlCode = Manager.CtlCount(gObj[Target].AccountID);
			int NPCGOLDS = Manager.VipCount(gObj[Target].AccountID);
			if (NPCGOLDS != 1)
			if (ctlCode == 0)
			{
				LPOBJECTSTRUCT lpObj;
				TNotice notice;
					//func.AllServerAnnounce("[%s] Fue el ganador del sorteo, Felicitaciones!", gObj[Target].Name);
					notice.SendToAllUser("[%s] Fue el ganador del sorteo, Felicitaciones!",lpObj->Name);
					//func.FireWork(Target);
					if(group.GetSection(1, Section))
					{	
							int NumRand = rand() % Section.RowCount; //Elige una fila al azar			
							DWORD Item =  ITEMGET(aSORTEO[NumRand].Type, aSORTEO[NumRand].Index);
							ItemSerialCreateSend(gObj[Target].m_Index, gObj[Target].MapNumber, gObj[Target].X, gObj[Target].Y,Item, aSORTEO[NumRand].ItemLevel,aSORTEO[NumRand].ItemDur ,aSORTEO[NumRand].ItemSkill, aSORTEO[NumRand].ItemLuck, aSORTEO[NumRand].ItemOpt, gObj[Target].m_Index, aSORTEO[NumRand].ItemExc);      
							//LogAddFuncColor(3, "[SORTEO] GANADOR : %s", gObj[Target].Name);
					}	

				}
			else
			{
				//LogAddFuncColor(3, "[SORTEO] ESTE PERSONAJE: %s  NO PARTICIPA", gObj[Target].Name);
				}
			

		}

	}
	_endthread();
}