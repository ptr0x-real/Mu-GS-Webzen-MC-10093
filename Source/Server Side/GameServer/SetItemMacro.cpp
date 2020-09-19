#include "stdafx.h"
#include "SetItemMacro.h"

#include "Common\winutil.h"
#include "Include\ReadScript.h"
#include "Common\zzzitem.h"
#include "Include\ItemDef.h"

#include "Include\Prodef.h"
#include "Common\winutil.h"
#include "Common\classdef.h"
#include "giocp.h"
#include "user.h"
#include "GMMng.h"
#include "Protocol.h"
#include "Sprotocol.h"
#include "Gamemain.h"
#include "wsGameServer.h"
#include "DSProtocol.h"
#include "QuestInfo.h"

#include "Common\SetItemOption.h"

#ifdef ITEM_ADD_OPTION_20061019

extern ITEM_ATTRIBUTE		ItemAttribute [MAX_ITEM];
extern CSetItemOption		gSetItemOption;

CSetItemMacro::CSetItemMacro()
{
}

CSetItemMacro::~CSetItemMacro()
{
}

BOOL CSetItemMacro::Load(char* szFileName)
{
	if((SMDFile=fopen(szFileName,"r")) == NULL)
	{
		LogAdd("¡Ú For BroadCasting Server Set Item Macro Load!!");
		return FALSE;
	}

	SMDToken Token;
	int n=0;

	char	szName[255];
	char	szLog[255];
	int		iSetIndex;
	int		iType;
	int		iIndex;
	int		iLevel;
	int		iSkill;
	int		iLuck;
	int		iOption;
	int		iExOption1;
	int		iExOption2;
	int		iExOption3;
	int		iExOption4;
	int		iExOption5;
	int		iExOption6;
	int		i = 0;
	int		iName = 0;

	Token = (*GetToken)();

	while( true )
	{
		if(Token == END)
		{
			break;
		}

		if(Token == NAME)
		{
			strcpy(szName, TokenString);
			sprintf(szLog, "%d - %s\n", iName, szName);

			bLoadItemName[iName] = TRUE;
			strcpy(sMakeItemName[iName], szName);

			OutputDebugString(szLog);
			Token = (*GetToken)();

			i = 0;

			if(Token == END)
			{
				break;
			}

			do
			{
				iSetIndex	= (int)TokenNumber;
				Token		= (*GetToken)();
				iType		= (int)TokenNumber;
				Token		= (*GetToken)();
				iIndex		= (int)TokenNumber;
				Token		= (*GetToken)();
				iLevel		= (int)TokenNumber;
				Token		= (*GetToken)();
				iSkill		= (int)TokenNumber;
				Token		= (*GetToken)();
				iLuck		= (int)TokenNumber;
				Token		= (*GetToken)();
				iOption		= (int)TokenNumber;
				Token		= (*GetToken)();
				iExOption1	= (int)TokenNumber;
				Token		= (*GetToken)();
				iExOption2	= (int)TokenNumber;
				Token		= (*GetToken)();
				iExOption3	= (int)TokenNumber;
				Token		= (*GetToken)();
				iExOption4	= (int)TokenNumber;
				Token		= (*GetToken)();
				iExOption5	= (int)TokenNumber;
				Token		= (*GetToken)();
				iExOption6	= (int)TokenNumber;

				SubItemList[iName][i].SetIndex		= iSetIndex;
				SubItemList[iName][i].ItemIndex		= MAKE_ITEMNUM(iType, iIndex);
				SubItemList[iName][i].Level			= iLevel;
				SubItemList[iName][i].Skill			= iSkill;
				SubItemList[iName][i].Luck			= iLuck;
				SubItemList[iName][i].Option		= iOption;
				SubItemList[iName][i].Ex1			= iExOption1;
				SubItemList[iName][i].Ex2			= iExOption2;
				SubItemList[iName][i].Ex3			= iExOption3;
				SubItemList[iName][i].Ex4			= iExOption4;
				SubItemList[iName][i].Ex5			= iExOption5;
				SubItemList[iName][i].Ex6			= iExOption6;

				wsprintf(szLog, "[%d] %s level:%d skill:%d luck:%d option:%d [%d,%d,%d,%d,%d,%d]\n",
						 i, ItemAttribute[MAKE_ITEMNUM(iType, iIndex)].Name,
						 iLevel, iSkill, iLuck, iOption, iExOption1, iExOption2, iExOption3, iExOption4, iExOption5, iExOption6);
				OutputDebugString(szLog);

				i++;

				Token = (*GetToken)();

			} while( Token == NUMBER);

			iName++;
		}
		else
		{
			LogAdd("¡Ú For BroadCasting Server Set Item Macro Load!!");
			return FALSE;
		}
	}

	fclose(SMDFile);		
	return TRUE;
}

BOOL CSetItemMacro::MakeItem(int aIndex, char* szSetName)
{
	BOOL	bFindItem = FALSE;
	int i;

	for( i = 0; i < MAX_SETITEMLIST_COUNT; i++ )
	{
		if( bLoadItemName[i] )
		{
			if( !strcmp(sMakeItemName[i], szSetName) )
			{
				bFindItem = TRUE;
				for( int j = 0; j < MAX_SETSUBITEMLIST_COUNT; j++ )
				{
					if( SubItemList[i][j].ItemIndex != 0 )
					{
						int		iLevel		= 0;
						int		iSkill		= 0;
						int		iLuck		= 0;
						int		iOption		= 0;
						BYTE	btNewOption	= 0;
						int		iDur		= 0;
						int		iAddStat	= 0;
						int		iSetOption	= 0;

						if( SubItemList[i][j].SetIndex != -1 )
						{
							iSkill	= SubItemList[i][j].Skill;
							iLuck	= SubItemList[i][j].Luck;
							iOption = SubItemList[i][j].Option;
							iLevel	= SubItemList[i][j].Level;

							btNewOption = SubItemList[i][j].Ex1<<5;
							btNewOption |= SubItemList[i][j].Ex2<<4;
							btNewOption |= SubItemList[i][j].Ex3<<3;
							btNewOption |= SubItemList[i][j].Ex4<<2;
							btNewOption |= SubItemList[i][j].Ex5<<1;
							btNewOption |= SubItemList[i][j].Ex6;

							gSetItemOption.GetSetOptionName(SubItemList[i][j].SetIndex);
							iSetOption = 1;

							if( rand() %100 < 80 )
							{
								iSetOption |= 0x04;
							}
							else
							{
								iSetOption |= 0x08;
							}

							ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, SubItemList[i][j].ItemIndex, 
														 iLevel, (BYTE)iDur, iSkill, iLuck, iOption, -1, btNewOption, iSetOption);
						}
						else
						{
							iSkill	= SubItemList[i][j].Skill;
							iLuck	= SubItemList[i][j].Luck;
							iOption = SubItemList[i][j].Option;
							iLevel	= SubItemList[i][j].Level;

							btNewOption = SubItemList[i][j].Ex1<<5;
							btNewOption |= SubItemList[i][j].Ex2<<4;
							btNewOption |= SubItemList[i][j].Ex3<<3;
							btNewOption |= SubItemList[i][j].Ex4<<2;
							btNewOption |= SubItemList[i][j].Ex5<<1;
							btNewOption |= SubItemList[i][j].Ex6;

							ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, SubItemList[i][j].ItemIndex, 
												 iLevel, (BYTE)iDur, iSkill, iLuck, iOption, -1, btNewOption, iSetOption);
						}
					}
					
				}
			}
		}
	}

	return bFindItem;
}
#endif