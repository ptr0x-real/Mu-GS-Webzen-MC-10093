// ChaosCastleSetItemDrop.cpp: implementation of the CChaosCastleSetItemDrop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common\winutil.h"
#include "Include\Readscript.h"
#include "ChaosCastleSetItemDrop.h"

#include "User.h"
#include "DsProtocol.h"

#include "Common\SetItemOption.h"

#ifdef CHANGE_CHAOSCASTLE_SET_ITEM_DROP

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CChaosCastleSetItemDrop	gChaosCastleSetItemDrop;

CChaosCastleSetItemDrop::CChaosCastleSetItemDrop()
{
	Init();
}

CChaosCastleSetItemDrop::~CChaosCastleSetItemDrop()
{

}

void CChaosCastleSetItemDrop::Init()
{
	memset(DropExItemIndex, 0, sizeof(DropExItemIndex));
}

BOOL CChaosCastleSetItemDrop::Load(char* filename)
{
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("load error %s", filename);
		return FALSE;
	}

	Init();
	SMDToken Token;
	
	int n=0;

	int	ItemType, ItemIndex, DropLevel;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			ItemType = (int)TokenNumber;

			if( ItemType == 255 )
			{
				for( int i = 0; i < MAX_CC_SETITEM_DROP_LEVEL; i++ )
				{
					Token = (*GetToken)();
					DropExItemIndex[i].DropRate = (int)TokenNumber;
				}
			}
			else
			{				
				while(1)
				{
					Token = (*GetToken)();
					ItemIndex = (int)TokenNumber;
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;

					Token = (*GetToken)();
					DropLevel = (int)TokenNumber;
					DropExItemIndex[DropLevel].DropSetItemIndex[DropExItemIndex[DropLevel].IndexCount++] = MAKE_ITEMNUM(ItemType, ItemIndex);
				}
			}
		}
	}
	fclose(SMDFile);
	return TRUE;
}

int CChaosCastleSetItemDrop::GetItemIndex()
{
	int randv = rand()%100;
	int	DropRate = DropExItemIndex[0].DropRate;

	for(int i = 0; i < MAX_CC_SETITEM_DROP_LEVEL; i++ )
	{
		if( randv < DropRate )
		{
			if( DropExItemIndex[i].IndexCount )
				return DropExItemIndex[i].DropSetItemIndex[rand()%(DropExItemIndex[i].IndexCount)];
		}
		DropRate += DropExItemIndex[i+1].DropRate;
	}

	return DropExItemIndex[0].DropSetItemIndex[0];
}

void CChaosCastleSetItemDrop::MakeRandomSetItem(int aIndex)
{
	int itemnum = GetItemIndex();
	int	SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand, option2rand, option3rand, optionc;	
	int	Option1 = 0, Option2 = 0, Option3 = 0;

	
	option1rand = 6;				// 일반적인 옵션1 (스킬) 붙을 확률 6/100
	option2rand = 4;				// 일반적인 옵션2 (행운) 붙을 확률 4/100
	option3rand = (rand()%100);		// 일반적인 옵션3 (추가포인트) 붙을 확률 랜덤/100
	optionc		= (rand()%3);

	// 스킬아이템 확률을 대폭 낮춘다.	
	if((rand()%100) < option2rand ) Option2 = 1;

	switch( optionc )
	{
	case 0 :
		if( option3rand < 4 ) Option3 = 3;	// 옵션
		break;
	case 1 :
		if( option3rand < 8 ) Option3 = 2;	// 옵션
		break;
	case 2 :
		if( option3rand < 12 ) Option3 = 1;	// 옵션
		break;
	}

	Option1 = 1;
	ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, itemnum, 
						 0, (BYTE)0, Option1, Option2, Option3, aIndex, 0, SetOption);

#ifdef UPDATE_SET_ITEM_DROP_LOG_20080420	// 카오스캐슬 셋트아이템 로그
	int tmpSetOption = 0;

	if( SetOption & 0x01)
	{
		tmpSetOption = 1;
	}
	else if( SetOption & 0x02 )
	{
		tmpSetOption = 2;
	}

	LogAddTD("[Chaos Castle] [%s][%s] Winner Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",			 
			 gObj[aIndex].AccountID, 
			 gObj[aIndex].Name,
			 itemnum, Option1, Option2, Option3,SetOption,
			 gSetItemOption.GetSetOptionName(itemnum, tmpSetOption));
#else	// UPDATE_SET_ITEM_DROP_LOG_20080420
	LogAddTD("[Chaos Castle] [%s][%s] Winner Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",			 
			 gObj[aIndex].AccountID, 
			 gObj[aIndex].Name,
			 itemnum, Option1, Option2, Option3,SetOption);
#endif	// UPDATE_SET_ITEM_DROP_LOG_20080420
}	

#endif