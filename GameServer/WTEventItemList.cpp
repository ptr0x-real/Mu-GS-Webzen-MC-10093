// WTEventItemList.cpp: implementation of the CWTEventItemList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WTEventItemList.h"

#ifdef WORLD_TOURNAMENT_EVENT_SETTING

#include "..\\common\\winutil.h"
#include "..\\Include\\ReadScript.h"
#include "..\\common\\zzzitem.h"
#include "..\\Include\\ItemDef.h"

#include "..\\Include\\Prodef.h"
#include "..\\common\\winutil.h"
#include "..\\Common\\classdef.h"
#include "giocp.h"
#include "user.h"
#include "GMMng.h"
#include "Protocol.h"
#include "Sprotocol.h"
#include "Gamemain.h"
#include "wsGameServer.h"
#include "DSProtocol.h"
#include "QuestInfo.h"

CWTEventItemList gWTEventItemList;

extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWTEventItemList::CWTEventItemList()
{

}

CWTEventItemList::~CWTEventItemList()
{

}


BOOL CWTEventItemList::Load(char* filename)
{
	memset(bLoadItemName, 0, sizeof(bLoadItemName));
	memset(SubItemList, 0xFF, sizeof(SubItemList));

	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		LogAdd("World Tournament EventItemList Load Fail!!");
		return FALSE;
	}	

	SMDToken Token;
	int n=0;

	char name[255];
	char	sLog[255];
	int type, index, level, skill, luck, option, ex1, ex2, ex3, ex4, ex5, ex6;
	
	Token = (*GetToken)();
	int i = 0;
	int	iName = 0;
	while( true )
	{		
		if(Token == END) break;		
		if(Token == NAME)
		{
			strcpy(name, TokenString);
			sprintf(sLog, "%d - %s\n", iName, name);

			bLoadItemName[iName] = TRUE;
			strcpy(sMakeItemName[iName], name);

			OutputDebugString(sLog);
			Token = (*GetToken)();				

			i = 0;

			if(Token == END) break;		
			do
			{					
				type = (int)TokenNumber;
				Token = (*GetToken)();	index = (int)TokenNumber;
				Token = (*GetToken)();	level = (int)TokenNumber;
				Token = (*GetToken)();	skill = (int)TokenNumber;
				Token = (*GetToken)();	luck = (int)TokenNumber;
				Token = (*GetToken)();	option = (int)TokenNumber;
				Token = (*GetToken)();	ex1 = (int)TokenNumber;
				Token = (*GetToken)();	ex2 = (int)TokenNumber;
				Token = (*GetToken)();	ex3 = (int)TokenNumber;
				Token = (*GetToken)();	ex4 = (int)TokenNumber;
				Token = (*GetToken)();	ex5 = (int)TokenNumber;
				Token = (*GetToken)();	ex6 = (int)TokenNumber;

				SubItemList[iName][i].ItemIndex = MAKE_ITEMNUM(type, index);
				SubItemList[iName][i].Level = level;
				SubItemList[iName][i].Skill = skill;
				SubItemList[iName][i].Luck = luck;
				SubItemList[iName][i].Option = option;
				SubItemList[iName][i].Ex1 = ex1;
				SubItemList[iName][i].Ex2 = ex2;
				SubItemList[iName][i].Ex3 = ex3;
				SubItemList[iName][i].Ex4 = ex4;
				SubItemList[iName][i].Ex5 = ex5;
				SubItemList[iName][i].Ex6 = ex6;
				
				wsprintf(sLog, "[%d] %s level:%d skill:%d luck:%d option:%d [%d,%d,%d,%d,%d,%d]\n",
						 i, ItemAttribute[MAKE_ITEMNUM(type, index)].Name,
						 level, skill, luck, option, ex1, ex2, ex3, ex4,ex5, ex6);
				OutputDebugString(sLog);

				i++;

				Token = (*GetToken)();
			} while( Token == NUMBER);

			iName++;
		}
		else
		{
			LogAdd("World Tournament EventItemList Load Fail!!");
			return FALSE;
		}
	}

	fclose(SMDFile);		
	return TRUE;
}

BOOL CWTEventItemList::CommandMakeItem(int aIndex, char* sCommand)
{
	BOOL	bFindItem = FALSE;
	for( int i = 0; i < MAX_WT_MAKEITEMLIST_COUNT; i++ )
	{
		if( bLoadItemName[i] )
		{
			if( !strcmp(sMakeItemName[i], sCommand) )
			{
				bFindItem = TRUE;
				for( int j = 0; j < MAX_WT_MAKESUBITEMLIST_COUNT; j++ )
				{
					if( SubItemList[i][j].ItemIndex != -1 )
					{
						int level, skill=0, luck=0, option=0;
						BYTE NewOption=0;
						int  dur=0;

						skill = SubItemList[i][j].Skill;
						luck = SubItemList[i][j].Luck;
						option = SubItemList[i][j].Option;
						level = SubItemList[i][j].Level;

						NewOption = SubItemList[i][j].Ex1<<5;
						NewOption |= SubItemList[i][j].Ex2<<4;
						NewOption |= SubItemList[i][j].Ex3<<3;
						NewOption |= SubItemList[i][j].Ex4<<2;
						NewOption |= SubItemList[i][j].Ex5<<1;
						NewOption |= SubItemList[i][j].Ex6;

						ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, 
											 gObj[aIndex].X, gObj[aIndex].Y, 
											 SubItemList[i][j].ItemIndex, 
											 level, 
											 (BYTE)dur, 
											 skill, 
											 luck, 
											 option, 
											 -1, 
											 NewOption, 
											 0);
					}
					
				}
			}
		}
	}
	return bFindItem;
}

#endif