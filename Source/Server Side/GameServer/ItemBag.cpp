// ItemBag.cpp: implementation of the CItemBag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common\winutil.h"
#include "Include\ReadScript.h"
#include "Common\zzzitem.h"

#include "DirPath.h"
#include "ItemBag.h"

#ifdef SCRIPT_DECODE_WORK	
#include "Common\WZScriptEncode.h"
#endif

extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CDirPath		gDirPath;


CItemBag::CItemBag()
{
	//Init();
}

CItemBag::~CItemBag()
{

}

void CItemBag::Init(char * name)
{	
	BagObjectCount = 0;

	BagNormalItemCount = 0;
	BagExItemCount = 0;
	
#ifdef SCRIPT_DECODE_WORK
	LoadItemDecode(gDirPath.GetNewPath(name));	
#else
	LoadItem(gDirPath.GetNewPath(name));	
#endif
}

#ifdef SCRIPT_DECODE_WORK
void CItemBag::LoadItemDecode(char *script_file)
{
	CWZScriptEncode		Encoder;
	if( !Encoder.Open(script_file) )
	{
		LogAdd(lMsg.Get(453), script_file);
		return;
	}

	WZSMDToken Token;
	int n=0;
	
	while( true )
	{
		Token = Encoder.GetToken();
		if(Token == END) break;
		if(Token == T_NUMBER)
		{
			n = BagObjectCount;
			BagObject[n].m_type = Encoder.GetNumber();
			Token = Encoder.GetToken(); BagObject[n].m_index	= Encoder.GetNumber();
			Token = Encoder.GetToken(); BagObject[n].m_level	= Encoder.GetNumber();
			Token = Encoder.GetToken(); BagObject[n].m_op1	= Encoder.GetNumber();
			Token = Encoder.GetToken(); BagObject[n].m_op2	= Encoder.GetNumber();
			Token = Encoder.GetToken(); BagObject[n].m_op3	= Encoder.GetNumber();
			
			if( BagObject[n].m_op2 )
			{
				BagExItemCount++;
			}
			else
			{
				BagNormalItemCount++;
			}			
			
			BagObjectCount++;
			if( BagObjectCount > MAX_ITEMBAG-1 ) break;
		}
	}
	LogAdd(lMsg.Get(454), script_file);
}
#endif

void CItemBag::LoadItem(char *script_file)
{
	if((SMDFile=fopen(script_file,"r")) == NULL)	
	{
		LogAdd(lMsg.Get(453), script_file);
		return ;
	}	

	SMDToken Token;
	int n=0;
	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			n = BagObjectCount;
			BagObject[n].m_type = (int)TokenNumber;
			Token = (*GetToken)(); BagObject[n].m_index	= (int)TokenNumber;
			Token = (*GetToken)(); BagObject[n].m_level	= (int)TokenNumber;
			Token = (*GetToken)(); BagObject[n].m_op1	= (int)TokenNumber;
			Token = (*GetToken)(); BagObject[n].m_op2	= (int)TokenNumber;
			Token = (*GetToken)(); BagObject[n].m_op3	= (int)TokenNumber;
			
			if( BagObject[n].m_op2 )
			{
				BagExItemCount++;
			}
			else
			{
				BagNormalItemCount++;
			}
			//LogAdd("Item Ãß°¡ type:%d index:%d level:%d op1%d  op2:%d  op3:%d", BagObject[n].m_type, BagObject[n].m_index, BagObject[n].m_level, BagObject[n].m_op1, BagObject[n].m_op2,BagObject[n].m_op3);
			/*
			char msg[255];
			wsprintf(msg, "%d\t%d\t%d\t%d\t%d\t%d\n", BagObject[n].m_type, BagObject[n].m_index, BagObject[n].m_level, BagObject[n].m_op1, BagObject[n].m_op2,BagObject[n].m_op3);
			OutputDebugString(msg);			
			*/
			
			BagObjectCount++;
			if( BagObjectCount > MAX_ITEMBAG-1 ) break;
		}
	}
	fclose(SMDFile);
	
	LogAdd(lMsg.Get(454), script_file);
}

BYTE CItemBag::GetType(int n)
{
	if( n < 0 || n > MAX_ITEMBAG-1 ) return 0;
	return BagObject[n].m_type;
	
}

BYTE CItemBag::GetIndex(int n)
{
	if( n < 0 || n > MAX_ITEMBAG-1 ) return 0;
	return BagObject[n].m_index;
}

BYTE CItemBag::GetLevel(int n)
{
	if( n < 0 || n > MAX_ITEMBAG-1 ) return 0;
	return BagObject[n].m_level;
}

BYTE CItemBag::GetOp1(int n)
{
	if( n < 0 || n > MAX_ITEMBAG-1 ) return 0;
	return BagObject[n].m_op1;
}

BYTE CItemBag::GetOp2(int n)
{
	if( n < 0 || n > MAX_ITEMBAG-1 ) return 0;
	return BagObject[n].m_op2;
}

BYTE CItemBag::GetOp3(int n)
{
	if( n < 0 || n > MAX_ITEMBAG-1 ) return 0;
	return BagObject[n].m_op3;
}








