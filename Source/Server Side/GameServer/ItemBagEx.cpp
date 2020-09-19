// ItemBagEx.cpp: implementation of the CItemBagEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemBagEx.h"

#include "Common\winutil.h"
#include "Include\ReadScript.h"
#include "Common\zzzitem.h"

#include "Include\public.h"
#include "Include\ProDef.h"
#include "Include\SproDef.h"
#include "Include\GMProtocol.h"
#include "Common\ZzzPath.h"
#include "SProtocol.h"
#include "User.h"
#include "gObjMonster.h"
#include "DSProtocol.h"
#include "Gamemain.h"

#include "DirPath.h"
#include "ItemBag.h"

#ifdef SCRIPT_DECODE_WORK	
#include "Common\WZScriptEncode.h"
#endif

extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];
extern CDirPath		gDirPath;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemBagEx::CItemBagEx()
{
	this->DropMapInfo = NULL;
}

CItemBagEx::~CItemBagEx()
{
	delete[] DropMapInfo;
}


void CItemBagEx::Init(char * name)
{	
	m_bLoad = FALSE;
	m_sEventName[0] = NULL;
	m_iEventItemType = -1;
	m_iEventItemLevel = 0;
	m_iDropZen = 0;
	m_iEventItemDropRate = 0;
	m_iItemDropRate = 0;
	m_iExItemDropRate = 0;
	
	m_iBagObjectCount = 0;	

	if(g_TerrainManager.Size() < 1)
		g_TerrainManager.Load();

	if(DropMapInfo == NULL)
		DropMapInfo = new CItemBagDropMapInfo[g_TerrainManager.Size()];
	
#ifdef SCRIPT_DECODE_WORK
	LoadItemDecode(gDirPath.GetNewPath(name));	
#else
	LoadItem(gDirPath.GetNewPath(name));	
#endif
}

#ifdef SCRIPT_DECODE_WORK
void CItemBagEx::LoadItemDecode(char *script_file)
{
	m_bLoad = FALSE;
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
			int Type = Encoder.GetNumber();
			if( Type == 0 )
			{	// 드롭될 아이템 조건 정보(맵, 몬스터 레벨)
				while(true)
				{
					Token = Encoder.GetToken();
					if(Token==T_NAME && strcmp("end",Encoder.GetString())==NULL) break;

					int	MapIndex = Encoder.GetNumber();
					if( !CHECK_LIMIT(MapIndex, g_TerrainManager.Size()) )
					{
						MsgBox("ExEvent ItemBag LoadFail [%s]", script_file);
						return;
					}

					Token = Encoder.GetToken(); DropMapInfo[MapIndex].m_bIsDrop = Encoder.GetNumber();
					Token = Encoder.GetToken(); DropMapInfo[MapIndex].m_MinMonsterLevel = Encoder.GetNumber();
					Token = Encoder.GetToken(); DropMapInfo[MapIndex].m_MaxMonsterLevel = Encoder.GetNumber();
				}
			}
			else if( Type == 1 )
			{	// 이벤트 아이템 정보
				while(true)
				{
					int type, index;
					Token = Encoder.GetToken();
					if(Token==T_NAME && strcmp("end",Encoder.GetString())==NULL) break;

					strcpy(m_sEventName, Encoder.GetString());
					Token = Encoder.GetToken(); m_iDropZen = Encoder.GetNumber();

					Token = Encoder.GetToken(); type = Encoder.GetNumber();
					Token = Encoder.GetToken(); index = Encoder.GetNumber();
					m_iEventItemType = MAKE_ITEMNUM(type, index);

					Token = Encoder.GetToken(); m_iEventItemLevel = Encoder.GetNumber();

					Token = Encoder.GetToken(); m_iEventItemDropRate = Encoder.GetNumber();
					Token = Encoder.GetToken(); m_iItemDropRate = Encoder.GetNumber();
					Token = Encoder.GetToken(); m_iExItemDropRate = Encoder.GetNumber();

					char msg[255];
					wsprintf(msg, "[%s] Eventitemnum = %d,EventItemLevel = %d, EventItemDropRate = %d, ItemDropRate = %d, ExItemDropRate = %d",
						     m_sEventName, m_iEventItemType, m_iEventItemLevel, m_iEventItemDropRate, 
							 m_iItemDropRate, m_iExItemDropRate);
				}
			}
			else if( Type == 2 )
			{
				while(true)
				{				
					Token = Encoder.GetToken();
					if(Token==T_NAME && strcmp("end",Encoder.GetString())==NULL) break;

					BagObject[n].m_type = m_iDropZen = Encoder.GetNumber();					

					Token = Encoder.GetToken(); BagObject[n].m_index = Encoder.GetNumber();

					Token = Encoder.GetToken(); BagObject[n].m_minLevel = Encoder.GetNumber();
					Token = Encoder.GetToken(); BagObject[n].m_maxLevel = Encoder.GetNumber();
					Token = Encoder.GetToken(); BagObject[n].m_isskill = Encoder.GetNumber();
					Token = Encoder.GetToken(); BagObject[n].m_isluck = Encoder.GetNumber();
					Token = Encoder.GetToken(); BagObject[n].m_isoption = Encoder.GetNumber();
					Token = Encoder.GetToken(); BagObject[n].m_isexitem = Encoder.GetNumber();

					n++;
					m_iBagObjectCount++;
					if( m_iBagObjectCount > MAX_ITEMBAGEX-1 ) break;
				}
			}
		}
	}
	LogAdd(lMsg.Get(454), script_file);
	m_bLoad = TRUE;
}
#endif

void CItemBagEx::LoadItem(char *script_file)
{
	m_bLoad = FALSE;
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
			int Type = (int)TokenNumber;
			if( Type == 0 )
			{	// 드롭될 아이템 조건 정보(맵, 몬스터 레벨)
				while(true)
				{
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;

					int	MapIndex = (int)TokenNumber;
					if( !CHECK_LIMIT(MapIndex, g_TerrainManager.Size()) )
					{
						MsgBox("ExEvent ItemBag LoadFail [%s]", script_file);
						return;
					}

					Token = (*GetToken)(); DropMapInfo[MapIndex].m_bIsDrop = (int)TokenNumber;
					Token = (*GetToken)(); DropMapInfo[MapIndex].m_MinMonsterLevel = (int)TokenNumber;
					Token = (*GetToken)(); DropMapInfo[MapIndex].m_MaxMonsterLevel = (int)TokenNumber;
				}
				
			}
			else if( Type == 1 )
			{	// 이벤트 아이템 정보
				while(true)
				{
					int type, index;
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;

					strcpy(m_sEventName, TokenString);
					Token = (*GetToken)(); m_iDropZen = (int)TokenNumber;

					Token = (*GetToken)(); type = (int)TokenNumber;
					Token = (*GetToken)(); index = (int)TokenNumber;
					m_iEventItemType = MAKE_ITEMNUM(type, index);

					Token = (*GetToken)(); m_iEventItemLevel = (int)TokenNumber;

					Token = (*GetToken)(); m_iEventItemDropRate = (int)TokenNumber;
					Token = (*GetToken)(); m_iItemDropRate = (int)TokenNumber;
					Token = (*GetToken)(); m_iExItemDropRate = (int)TokenNumber;

					char msg[255];
					wsprintf(msg, "[%s] Eventitemnum = %d,EventItemLevel = %d, EventItemDropRate = %d, ItemDropRate = %d, ExItemDropRate = %d",
						     m_sEventName, m_iEventItemType, m_iEventItemLevel, m_iEventItemDropRate, 
							 m_iItemDropRate, m_iExItemDropRate);
					LogAddTD(msg);
				}
			}
			else if( Type == 2 )
			{
				while(true)
				{				
					Token = (*GetToken)();
					if(Token==NAME && strcmp("end",TokenString)==NULL) break;

					BagObject[n].m_type = (int)TokenNumber;					

					Token = (*GetToken)(); BagObject[n].m_index = (int)TokenNumber;

					Token = (*GetToken)(); BagObject[n].m_minLevel = (int)TokenNumber;
					Token = (*GetToken)(); BagObject[n].m_maxLevel = (int)TokenNumber;
					Token = (*GetToken)(); BagObject[n].m_isskill = (int)TokenNumber;
					Token = (*GetToken)(); BagObject[n].m_isluck = (int)TokenNumber;
					Token = (*GetToken)(); BagObject[n].m_isoption = (int)TokenNumber;
					Token = (*GetToken)(); BagObject[n].m_isexitem = (int)TokenNumber;

					/*
					char msg[255];
					wsprintf(msg, "[%d] [%s] type=%d,index=%d,minLv=%d,MaxLv=%d,skill=%d,luck=%d,op=%d,Ex=%d\n",							
							n, 
							ItemAttribute[MAKE_ITEMNUM(BagObject[n].m_type, BagObject[n].m_index)].Name,
							BagObject[n].m_index,
							BagObject[n].m_minLevel,
							BagObject[n].m_maxLevel,
							BagObject[n].m_isskill,
							BagObject[n].m_isluck,
							BagObject[n].m_isoption,
							BagObject[n].m_isexitem);

					wsprintf(msg, "%s\n",
									ItemAttribute[MAKE_ITEMNUM(BagObject[n].m_type, BagObject[n].m_index)].Name);									

					OutputDebugString(msg);
					*/
					n++;
					m_iBagObjectCount++;
					if( m_iBagObjectCount > MAX_ITEMBAGEX-1 ) break;
				}
			}				
		}
	}

	fclose(SMDFile);	
	LogAdd(lMsg.Get(454), script_file);

	m_bLoad = TRUE;
}


BYTE CItemBagEx::GetLevel(int n)
{
	if( n < 0 || n > MAX_ITEMBAGEX-1 ) return 0;

	if( BagObject[n].m_minLevel == BagObject[n].m_maxLevel )
	{
		return BagObject[n].m_minLevel;
	}
	
	int sub = BagObject[n].m_maxLevel - BagObject[n].m_minLevel + 1;
	int level = BagObject[n].m_minLevel+rand()%sub;

	return level;
}

BOOL CItemBagEx::IsEnableEventItemDrop(int aIndex)
{
	int iMapNumber = gObj[aIndex].MapNumber;
	
	if( !DropMapInfo[iMapNumber].m_bIsDrop )
	{	// 드롭수 없는 맵이다
		return FALSE;
	}

	int iLevel = gObj[aIndex].Level;

	if( iLevel < DropMapInfo[iMapNumber].m_MinMonsterLevel || 
		iLevel > DropMapInfo[iMapNumber].m_MaxMonsterLevel )
	{	// 드롭 몬스터 레벨이 맞지 않다
		return FALSE;
	}
	return TRUE;	
}

BOOL CItemBagEx::DropEventItem(int aIndex)
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;

	if( !IsEnableEventItemDrop(aIndex) )	
		return FALSE;				// 드롭조건이 맞지 않다

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( (rand()%1000) < m_iEventItemDropRate )
	{
		int type, level, x, y;
		float dur=0;

		dur  = 255;
		x    = lpObj->X;
		y    = lpObj->Y;
		level= m_iEventItemLevel;
		type = m_iEventItemType;

		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, 0, 0, 0, MaxHitUser);
		LogAddTD("[%s][%s] [%s] MonsterEventItemDrop (%d)(%d/%d)",
				 lpObj->AccountID, lpObj->Name, m_sEventName, lpObj->MapNumber, x, y);
		return TRUE;
	}
	return FALSE;
}


BOOL CItemBagEx::DropItem(int aIndex)
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{
		// m_iItemDropRate/100 의 확률로 이벤트용 아이템 증정
		if( (rand()%100) < m_iItemDropRate )
		{
			DropItemNum = (rand()%GetBagCount());
			dur  = 0;
			x    = lpObj->X;
			y    = lpObj->Y;

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = rand()%2;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	// 행운이 붙을수 있으면
				Option2 = rand()%2;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

#ifdef NEW_SKILL_FORSKYLAND
			if( BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(0);
				Option1 = 1;
			}
#endif
			
			// 혼석, 축석, 영석은 레벨이 없게
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// 반지류는 레벨 0
				level   = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index, ExOption);
			LogAddTD("[%s][%s] [%s] Event ItemDrop : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );
			return TRUE;
		}
	}
	x    = lpObj->X;
	y    = lpObj->Y;	
	MapC[lpObj->MapNumber].MoneyItemDrop(m_iDropZen, x ,y);	
	return TRUE;
}

BYTE CItemBagEx::GetMinLevel(int n)
{
	if( n < 0 || n > MAX_ITEMBAGEX-1 ) return 0;
	
	return BagObject[n].m_minLevel;
}

BYTE CItemBagEx::GetMaxLevel(int n)
{
	if( n < 0 || n > MAX_ITEMBAGEX-1 ) return 0;
	
	return BagObject[n].m_maxLevel;
}

BOOL CItemBagEx::PickItem (CItem & objItem, int & nItemIndex)
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	int DropItemNum;
//	int ExOption = 0;

	if( GetBagCount() > 0 )
	{
		DropItemNum = (rand()%GetBagCount());
		nItemIndex	= DropItemNum;

		// 타입
		objItem.m_Type	= ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);
		// 레벨
		objItem.m_Level	= GetLevel(DropItemNum);

		if( objItem.m_Type == -1 )
			return FALSE;

		if( BagObject[DropItemNum].m_isskill )
		{	// 스킬이 붙을 수 있으면
			objItem.m_Option1		= 1;
		}

		if( BagObject[DropItemNum].m_isluck )
		{	
			// 행운이 붙을 수 있으면
			objItem.m_Option2		= 1;
		}

		if( BagObject[DropItemNum].m_isoption )
		{
			// 추가옵션이 붙을 수 있으면
			objItem.m_Option3		= 1;
		}

		if( BagObject[DropItemNum].m_isexitem )
		{
			// 액설런트 아이템은 행운이 붙지 않는다.
			objItem.m_NewOption	= 1;
			objItem.m_Option1	= 0;
			objItem.m_Option2	= 0;
		}
		
		// 혼석, 축석, 영석은 레벨이 없게
		if( (objItem.m_Type == MAKE_ITEMNUM(12, 15)) ||
			(objItem.m_Type == MAKE_ITEMNUM(14, 13)) ||
			(objItem.m_Type == MAKE_ITEMNUM(14, 14)) )
		{	
			objItem.m_Option1	= 0; 
			objItem.m_Option2	= 0;
			objItem.m_Option3	= 0;
			objItem.m_Level		= 0;
		}

		if( (objItem.m_Type == MAKE_ITEMNUM(13, 0)) ||
			(objItem.m_Type == MAKE_ITEMNUM(13, 1)) ||				
			(objItem.m_Type == MAKE_ITEMNUM(13, 2)) ||
			(objItem.m_Type == MAKE_ITEMNUM(13, 8)) ||
			(objItem.m_Type == MAKE_ITEMNUM(13, 9)) ||
			(objItem.m_Type == MAKE_ITEMNUM(13, 12)) ||
			(objItem.m_Type == MAKE_ITEMNUM(13, 13)) )
		{	// 반지류는 레벨 0
			objItem.m_Level		= 0;
		}

		objItem.m_Durability	= 0;

		return TRUE;
	}

	return FALSE;
}

#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
BOOL CItemBagEx::DropRedRibbonBoxEventItem( int aIndex )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int	DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{	
		// 100% 의 확률로 이벤트용 아이템 증정
		DropItemRate = rand() % 10000;
		if( DropItemRate < g_iRedRibbonBoxDropZenRate )
		{		
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iRedRibbonBoxDropZen, lpObj->X, lpObj->Y );

			LogAddTD("[EVENT CHRISTMAS RIBBONBOX] RIBBONBOX Event ItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iRedRibbonBoxDropZen );
		}
		else 
		{
			DropItemNum = ( rand() % GetBagCount() );


			dur  = 0;

			x   = lpObj->X;
			y   = lpObj->Y;

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// 빨간리본상자는 엑셀 아이템을 드롭하지 않는다.
			ExOption = 0;
			
			// 혼석, 축석, 영석은 레벨이 없게
			if( ( type == MAKE_ITEMNUM( 12, 15 ) ) ||
				( type == MAKE_ITEMNUM( 14, 13 ) ) ||
				( type == MAKE_ITEMNUM( 14, 14 ) ) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			// 데빌스퀘어초대권, 잃어버린지도, 투명망토는 스킬, 행운, 옵션이 붙지 않는다.
			if( ( type == MAKE_ITEMNUM( 14, 19 ) ) ||
				( type == MAKE_ITEMNUM( 14, 28 ) ) ||
				( type == MAKE_ITEMNUM( 13, 18 ) ) )
			{
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;				
			}

			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, ( BYTE )x, ( BYTE )y, type, level, ( BYTE )dur, Option1, Option2, Option3, lpObj->m_Index, ExOption );
#ifdef ADD_LOG_EVENT_DROP_EXITEM_20070308	// 빨간리본상자
			LogAddTD("[EVENT CHRISTMAS RIBBONBOX ] REDRIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]", 
				lpObj->AccountID, lpObj->Name, m_sEventName, 
				lpObj->MapNumber, x, y,
				ItemAttribute[type].Name, type, level, Option1, Option2, Option3,
				(ExOption & 0x20) >> 5,
				(ExOption & 0x10) >> 4,
				(ExOption & 0x08) >> 3,
				(ExOption & 0x04) >> 2,
				(ExOption & 0x02) >> 1,
				(ExOption & 0x01) >> 0,
				0x00);
#else
			LogAddTD("[EVENT CHRISTMAS RIBBONBOX ] REDRIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );	
#endif
		}
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));		
	}
	
	return TRUE;
}

BOOL CItemBagEx::DropGreenRibbonBoxEventItem( int aIndex )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int	DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 100% 의 확률로 이벤트용 아이템 증정		
		DropItemRate = rand() % 10000;
		if( DropItemRate < g_iGreenRibbonBoxDropZenRate )
		{	
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iGreenRibbonBoxDropZen, lpObj->X, lpObj->Y );
			
			LogAddTD("[EVENT CHRISTMAS RIBBONBOX] GREENRIBBONBOX Event ItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iGreenRibbonBoxDropZen );			
		}
		else 
		{
			DropItemNum = ( rand() % GetBagCount() );


			dur  = 0;

			x = lpObj->X;
			y = lpObj->Y;

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// 엑셀 아이템이면 무조건 엑셀옵션
			if( BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				// 액설런트 아이템은 행운이 붙지 않는다.
				Option2 = 0;
				Option1 = 1;
				level	= 0;
			}

			// 혼석, 축석, 영석, 생석은 레벨이 없게
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) ||
				(type == MAKE_ITEMNUM(14, 16)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			// 데빌스퀘어초대권, 잃어버린지도, 투명망토는 스킬, 행운, 옵션이 붙지 않는다.
			if( ( type == MAKE_ITEMNUM( 14, 19 ) ) ||
				( type == MAKE_ITEMNUM( 14, 28 ) ) ||
				( type == MAKE_ITEMNUM( 13, 18 ) ) )
			{
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;				
			}			

			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, ( BYTE )x, ( BYTE )y, type, level, ( BYTE )dur, Option1, Option2, Option3, lpObj->m_Index, ExOption );
#ifdef ADD_LOG_EVENT_DROP_EXITEM_20070308	// 초록리본상자
			LogAddTD("[EVENT CHRISTMAS RIBBONBOX ] GREENRIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]", 
				lpObj->AccountID, lpObj->Name, m_sEventName, 
				lpObj->MapNumber, x, y,
				ItemAttribute[type].Name, type, level, Option1, Option2, Option3,
				(ExOption & 0x20) >> 5,
				(ExOption & 0x10) >> 4,
				(ExOption & 0x08) >> 3,
				(ExOption & 0x04) >> 2,
				(ExOption & 0x02) >> 1,
				(ExOption & 0x01) >> 0,
				0x00);
#else
			LogAddTD("[EVENT CHRISTMAS RIBBONBOX ] GREENRIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );
#endif
		}
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}
	
	return TRUE;
}

BOOL CItemBagEx::DropBlueRibbonBoxEventItem( int aIndex )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int	DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 100% 의 확률로 이벤트용 아이템 증정		
		DropItemRate = rand() % 10000;
		if( DropItemRate < g_iBlueRibbonBoxDropZenRate )
		{		
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iBlueRibbonBoxDropZen, lpObj->X, lpObj->Y );
			
			LogAddTD("[EVENT CHRISTMAS RIBBONBOX] BLUERIBBONBOX Event ItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iBlueRibbonBoxDropZen );
		}
		else 
		{
			DropItemNum = ( rand() % GetBagCount() );
			dur  = 0;

			x = lpObj->X;
			y = lpObj->Y;

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// 엑셀 아이템이면 무조건 엑셀옵션
			if( BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				// 액설런트 아이템은 행운이 붙지 않는다.
				Option2 = 0;
				Option1 = 1;
				level	= 0;
			}

			// 혼석, 축석, 영석은 레벨이 없게
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 21)) )
			{	// 반지류는 레벨 0
				level   = 0;
			}

			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, ( BYTE )x, ( BYTE )y, type, level, ( BYTE )dur, Option1, Option2, Option3, lpObj->m_Index, ExOption );
#ifdef ADD_LOG_EVENT_DROP_EXITEM_20070308	// 파란리본상자
			LogAddTD("[EVENT CHRISTMAS RIBBONBOX ] BLUERIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]", 
				lpObj->AccountID, lpObj->Name, m_sEventName, 
				lpObj->MapNumber, x, y,
				ItemAttribute[type].Name, type, level, Option1, Option2, Option3,
				(ExOption & 0x20) >> 5,
				(ExOption & 0x10) >> 4,
				(ExOption & 0x08) >> 3,
				(ExOption & 0x04) >> 2,
				(ExOption & 0x02) >> 1,
				(ExOption & 0x01) >> 0,
				0x00);
#else
			LogAddTD("[EVENT CHRISTMAS RIBBONBOX ] BLUERIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );
#endif
		}
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}
	
	return TRUE;
}
#endif	// #ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212

#ifdef VALENTINES_DAY_BOX_EVENT_20060124
BOOL CItemBagEx::DropPinkChocolateBoxEventItem( int aIndex )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int	DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{	
		// 100% 의 확률로 이벤트용 아이템 증정
		DropItemRate = rand() % 10000;
		if( DropItemRate < g_iPinkChocolateBoxDropZenRate )
		{			
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iPinkChocolateBoxDropZen, lpObj->X, lpObj->Y );

			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX] PINKCHOCOLATE BOX Event ItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iPinkChocolateBoxDropZen );
		}
		else 
		{
			DropItemNum = ( rand() % GetBagCount() );


			dur  = 0;

			x   = lpObj->X;
			y   = lpObj->Y;

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// 빨간리본상자는 엑셀 아이템을 드롭하지 않는다.
			ExOption = 0;
			
			// 혼석, 축석, 영석은 레벨이 없게
			if( ( type == MAKE_ITEMNUM( 12, 15 ) ) ||
				( type == MAKE_ITEMNUM( 14, 13 ) ) ||
				( type == MAKE_ITEMNUM( 14, 14 ) ) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			// 데빌스퀘어초대권, 잃어버린지도, 투명망토는 스킬, 행운, 옵션이 붙지 않는다.
			if( ( type == MAKE_ITEMNUM( 14, 19 ) ) ||
				( type == MAKE_ITEMNUM( 14, 28 ) ) ||
				( type == MAKE_ITEMNUM( 13, 18 ) ) )
			{
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;				
			}

			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, ( BYTE )x, ( BYTE )y, type, level, ( BYTE )dur, Option1, Option2, Option3, lpObj->m_Index, ExOption );
#ifdef ADD_LOG_VALENTINES_DAY_DROP_EXITEM_20070212
			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX ] PINKCHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3,
					(ExOption & 0x20) >> 5,
					(ExOption & 0x10) >> 4,
					(ExOption & 0x08) >> 3,
					(ExOption & 0x04) >> 2,
					(ExOption & 0x02) >> 1,
					(ExOption & 0x01) >> 0,
					0x00);
#else
			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX ] PINKCHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );		
#endif	
		}
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));		
	}
	
	return TRUE;
}

BOOL CItemBagEx::DropRedChocolateBoxEventItem( int aIndex )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int	DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 100% 의 확률로 이벤트용 아이템 증정		
		DropItemRate = rand() % 10000;
		if( DropItemRate < g_iRedChocolateBoxDropZenRate )
		{			
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iRedChocolateBoxDropZen, lpObj->X, lpObj->Y );	
			
			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX] REDCHOCOLATEBOX Event ItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iRedChocolateBoxDropZen );			
		}
		else 
		{
			DropItemNum = ( rand() % GetBagCount() );


			dur  = 0;

			x = lpObj->X;
			y = lpObj->Y;

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// 엑셀 아이템이면 무조건 엑셀옵션
			if( BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				// 액설런트 아이템은 행운이 붙지 않는다.
				Option2 = 0;
				Option1 = 1;
				level	= 0;
			}

			// 혼석, 축석, 영석, 생석은 레벨이 없게
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) ||
				(type == MAKE_ITEMNUM(14, 16)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			// 데빌스퀘어초대권, 잃어버린지도, 투명망토는 스킬, 행운, 옵션이 붙지 않는다.
			if( ( type == MAKE_ITEMNUM( 14, 19 ) ) ||
				( type == MAKE_ITEMNUM( 14, 28 ) ) ||
				( type == MAKE_ITEMNUM( 13, 18 ) ) )
			{
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;				
			}			

			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, ( BYTE )x, ( BYTE )y, type, level, ( BYTE )dur, Option1, Option2, Option3, lpObj->m_Index, ExOption );
#ifdef ADD_LOG_VALENTINES_DAY_DROP_EXITEM_20070212
			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX ] REDCHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3,
					(ExOption & 0x20) >> 5,
					(ExOption & 0x10) >> 4,
					(ExOption & 0x08) >> 3,
					(ExOption & 0x04) >> 2,
					(ExOption & 0x02) >> 1,
					(ExOption & 0x01) >> 0,
					0x00);
#else
			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX ] REDCHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );	
#endif	
		}
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}
	
	return TRUE;
}

BOOL CItemBagEx::DropBlueChocolateBoxEventItem( int aIndex )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int	DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 100% 의 확률로 이벤트용 아이템 증정		
		DropItemRate = rand() % 10000;
		if( DropItemRate < g_iBlueChocolateBoxDropZenRate )
		{			
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iBlueChocolateBoxDropZen, lpObj->X, lpObj->Y );	

			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX] BLUECHOCOLATEBOX Event ItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iBlueChocolateBoxDropZen );		
		}
		else 
		{
			DropItemNum = ( rand() % GetBagCount() );
			dur  = 0;

			x = lpObj->X;
			y = lpObj->Y;

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// 엑셀 아이템이면 무조건 엑셀옵션
			if( BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				// 액설런트 아이템은 행운이 붙지 않는다.
				Option2 = 0;
				Option1 = 1;
				level	= 0;
			}

			// 혼석, 축석, 영석은 레벨이 없게
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 21)) )
			{	// 반지류는 레벨 0
				level   = 0;
			}

			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, ( BYTE )x, ( BYTE )y, type, level, ( BYTE )dur, Option1, Option2, Option3, lpObj->m_Index, ExOption );
#ifdef ADD_LOG_VALENTINES_DAY_DROP_EXITEM_20070212
			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX ] BLUECHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d  Ex:[%d,%d,%d,%d,%d,%d,%d]", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3,
					(ExOption & 0x20) >> 5,
					(ExOption & 0x10) >> 4,
					(ExOption & 0x08) >> 3,
					(ExOption & 0x04) >> 2,
					(ExOption & 0x02) >> 1,
					(ExOption & 0x01) >> 0,
					0x00);

#else
			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX ] BLUECHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );
#endif
		}
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}
	
	return TRUE;
}
#endif	// #ifdef VALENTINES_DAY_BOX_EVENT_20060124

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
BOOL CItemBagEx::DropLightPurpleCandyBoxEventItem( int aIndex )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int	DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{	
		// 100% 의 확률로 이벤트용 아이템 증정
		DropItemRate = rand() % 10000;
		if( DropItemRate < g_iLightPurpleCandyBoxDropZenRate )
		{			
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iLightPurpleCandyBoxDropZen, lpObj->X, lpObj->Y );

			LogAddTD("[EVENT WHITE DAY CANDY BOX] LIGHT PURPLE BOX Event ItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iLightPurpleCandyBoxDropZen );
		}
		else 
		{
			DropItemNum = ( rand() % GetBagCount() );


			dur  = 0;

			x   = lpObj->X;
			y   = lpObj->Y;

			// 레벨
			level= GetLevel( DropItemNum );
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// 빨간리본상자는 엑셀 아이템을 드롭하지 않는다.
			ExOption = 0;
			
			// 혼석, 축석, 영석은 레벨이 없게
			if( ( type == MAKE_ITEMNUM( 12, 15 ) ) ||
				( type == MAKE_ITEMNUM( 14, 13 ) ) ||
				( type == MAKE_ITEMNUM( 14, 14 ) ) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			// 데빌스퀘어초대권, 잃어버린지도, 투명망토는 스킬, 행운, 옵션이 붙지 않는다.
			if( ( type == MAKE_ITEMNUM( 14, 19 ) ) ||
				( type == MAKE_ITEMNUM( 14, 28 ) ) ||
				( type == MAKE_ITEMNUM( 13, 18 ) ) )
			{
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;				
			}

			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, ( BYTE )x, ( BYTE )y, type, level, ( BYTE )dur, Option1, Option2, Option3, lpObj->m_Index, ExOption );
#ifdef ADD_LOG_EVENT_DROP_EXITEM_20070308	// 연보라사탕상자
			LogAddTD("[EVENT WHITE DAY CANDY BOX] LIGHT PURPLE BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3,
					(ExOption & 0x20) >> 5,
					(ExOption & 0x10) >> 4,
					(ExOption & 0x08) >> 3,
					(ExOption & 0x04) >> 2,
					(ExOption & 0x02) >> 1,
					(ExOption & 0x01) >> 0,
					0x00);	
#else
			LogAddTD("[EVENT WHITE DAY CANDY BOX] LIGHT PURPLE BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );	
#endif
		}
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));		
	}
	
	return TRUE;
}

BOOL CItemBagEx::DropVermilionCandyBoxEventItem( int aIndex )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int	DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 100% 의 확률로 이벤트용 아이템 증정		
		DropItemRate = rand() % 10000;
		if( DropItemRate < g_iVermilionCandyBoxDropZenRate )
		{			
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iVermilionCandyBoxDropZen, lpObj->X, lpObj->Y );

			LogAddTD("[EVENT WHITE DAY CANDY BOX] VERMILION BOX Event ItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iVermilionCandyBoxDropZen );			
		}
		else 
		{
			DropItemNum = ( rand() % GetBagCount() );


			dur  = 0;

			x = lpObj->X;
			y = lpObj->Y;

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// 엑셀 아이템이면 무조건 엑셀옵션
			if( BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				// 액설런트 아이템은 행운이 붙지 않는다.
				Option2 = 0;
				Option1 = 1;
				level	= 0;
			}

			// 혼석, 축석, 영석, 생석은 레벨이 없게
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) ||
				(type == MAKE_ITEMNUM(14, 16)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			// 데빌스퀘어초대권, 잃어버린지도, 투명망토는 스킬, 행운, 옵션이 붙지 않는다.
			if( ( type == MAKE_ITEMNUM( 14, 19 ) ) ||
				( type == MAKE_ITEMNUM( 14, 28 ) ) ||
				( type == MAKE_ITEMNUM( 13, 18 ) ) )
			{
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;				
			}			

			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, ( BYTE )x, ( BYTE )y, type, level, ( BYTE )dur, Option1, Option2, Option3, lpObj->m_Index, ExOption );
#ifdef ADD_LOG_EVENT_DROP_EXITEM_20070308	// 주홍초콜릿상자
			LogAddTD("[EVENT WHITE DAY CANDY BOX] VERMILION BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3,
					(ExOption & 0x20) >> 5,
					(ExOption & 0x10) >> 4,
					(ExOption & 0x08) >> 3,
					(ExOption & 0x04) >> 2,
					(ExOption & 0x02) >> 1,
					(ExOption & 0x01) >> 0,
					0x00);	
#else
			LogAddTD("[EVENT WHITE DAY CANDY BOX] VERMILION BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );
#endif
		}
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}
	
	return TRUE;
}

BOOL CItemBagEx::DropDeepBlueCandyBoxEventItem( int aIndex )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int	DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 100% 의 확률로 이벤트용 아이템 증정		
		DropItemRate = rand() % 10000;
		if( DropItemRate < g_iDeepBlueCandyBoxDropZenRate )
		{			
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iDeepBlueCandyBoxDropZen, lpObj->X, lpObj->Y );	
			
			LogAddTD("[EVENT WHITE DAY CANDY BOX] DEEP BLUE BOX Event ItemDrop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iDeepBlueCandyBoxDropZen );		
		}
		else 
		{
			DropItemNum = ( rand() % GetBagCount() );
			dur  = 0;

			x = lpObj->X;
			y = lpObj->Y;

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			// 엑셀 아이템이면 무조건 엑셀옵션
			if( BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				// 액설런트 아이템은 행운이 붙지 않는다.
				Option2 = 0;
				Option1 = 1;
				level	= 0;
			}

			// 혼석, 축석, 영석은 레벨이 없게
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 21)) )
			{	// 반지류는 레벨 0
				level   = 0;
			}

			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, ( BYTE )x, ( BYTE )y, type, level, ( BYTE )dur, Option1, Option2, Option3, lpObj->m_Index, ExOption );
#ifdef ADD_LOG_EVENT_DROP_EXITEM_20070308	// 감청초콜릿상자
			LogAddTD("[EVENT WHITE DAY CANDY BOX] DEEP BLUE BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3,
					(ExOption & 0x20) >> 5,
					(ExOption & 0x10) >> 4,
					(ExOption & 0x08) >> 3,
					(ExOption & 0x04) >> 2,
					(ExOption & 0x02) >> 1,
					(ExOption & 0x01) >> 0,
					0x00);	
#else
			LogAddTD("[EVENT WHITE DAY CANDY BOX] DEEP BLUE BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					lpObj->MapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );
#endif
		}
		PMSG_SERVERCMD	ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;
		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}
	
	return TRUE;
}
#endif	// #ifdef WHITEDAY_CANDY_BOX_EVENT_20060222

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
BOOL CItemBagEx::DropHallowinEventItem(LPOBJECTSTRUCT lpObj)
{
	if( !m_bLoad )
	{
		return FALSE;
	}

	int iType;
	int iLevel;
	int X;
	int Y;
	int iDropItemNum = 0;
	int iDropItemRate;
	int iDuration = 0;
	int iTotalDropRate = 0;

	int iItemBagCount = GetBagCount();

	if( iItemBagCount > 0 )
	{
//		iDropItemNum = (rand()%iItemBagCount);
	 	iTotalDropRate = g_iHallowinDayEventJOLBlessDropRate + g_iHallowinDayEventJOLAngerDropRaTe + g_iHallowinDayEventJOLScreamDropRate
			+ g_iHallowinDayEventJOLFoodDropRate + g_iHallowinDayEventJOLDrinkDropRate + g_iHallowinDayEventJOLPolymorphRingDropRate;
		iDropItemRate = rand() % iTotalDropRate;

		if( iDropItemRate >=
			( iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate ) )
		{
			iDropItemNum = 0;
		}
		else if( iDropItemRate >=
			( iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe ) )
		{
			iDropItemNum = 1;
		}
		else if( iDropItemRate >=
			( iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe - g_iHallowinDayEventJOLScreamDropRate ) )
		{
			iDropItemNum = 2;
		}
		else if( iDropItemRate >=
			( iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe - g_iHallowinDayEventJOLScreamDropRate - g_iHallowinDayEventJOLFoodDropRate ) )
		{
			iDropItemNum = 3;
		}
		else if( iDropItemRate >= 
			( iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe - g_iHallowinDayEventJOLScreamDropRate - g_iHallowinDayEventJOLFoodDropRate - g_iHallowinDayEventJOLDrinkDropRate ) )
		{
			iDropItemNum = 4;
		}
		else if( iDropItemRate >= 
			( iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe - g_iHallowinDayEventJOLScreamDropRate - g_iHallowinDayEventJOLFoodDropRate - g_iHallowinDayEventJOLDrinkDropRate - g_iHallowinDayEventJOLPolymorphRingDropRate ) )
		{
			iDropItemNum = 5;
		}

		X = lpObj->X;
		Y = lpObj->Y;

		iLevel = GetLevel( iDropItemNum );
		iType = ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

		if( iType == MAKE_ITEMNUM( 13, 40 ) )
		{
			iLevel = 0;
			iDuration = 100;
		}

		ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, X, Y, iType, iLevel, iDuration, 0, 0, 0, lpObj->m_Index, 0, 0 );
		LogAddTD("[HallowinDay Event][Pumpkin of Luck] Event Item Drop [%s][%s] : (%d)(%d/%d) Item Attribute : %s(%d) Level : %d Dur : %d"
			, lpObj->AccountID
			, lpObj->Name
			, lpObj->MapNumber
			, X
			, Y
			, ItemAttribute[iType].Name
			, iType
			, iLevel
			, iDuration
			);
	}

	return TRUE;
}
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

#ifdef MODIFY_KUNDUN_REWARD_01_20040915
BOOL CItemBagEx::DropKundunEventItem(int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY)		// 쿤둔 이벤트 아이템 드롭
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

#ifdef EXTEND_LOG_SYSTEM_20060202
	// 쿤둔 사망시 쿤둔 이벤트 아이템 백의 리스트를 로그로 저장한다.
	int iItemBagCount = GetBagCount();

	for( int i = 0 ; i < iItemBagCount ; i++ )
	{
		type = ItemGetNumberMake(BagObject[i].m_type, BagObject[i].m_index);
		LogAdd("[○KUNDUN EVENT][KUNDUN ITEM BAG LIST] Item:(%s)%d Level:%d-%d op1:%d op2:%d op3:%d expo:%d",
				ItemAttribute[type].Name, type, BagObject[i].m_minLevel, BagObject[i].m_maxLevel,
				BagObject[i].m_isskill, BagObject[i].m_isluck, BagObject[i].m_isoption, BagObject[i].m_isexitem );
	}
#endif

	if( GetBagCount() > 0 )
	{
		// 100% 확률로 이벤트용 아이템 증정
		if( 1 )
		{
			DropItemNum = (rand()%GetBagCount());
			dur  = 0;

			if( cX==0 && cY==0 )
			{
				x   = lpObj->X;
				y   = lpObj->Y;
			}
			else
			{
				x	= cX;
				y	= cY;
			}
			

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

#ifdef NEW_SKILL_FORSKYLAND
			if( BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(0);
				// 액설런트 아이템은 행운이 붙지 않는다.
				Option2 = 0;
				Option1 = 1;
			}
#endif
			
			// 혼석, 축석, 영석은 레벨이 없게
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) ||
				(type == MAKE_ITEMNUM(14, 16)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// 반지류는 레벨 0
				level   = 0;
			}

			// MODIFY_ITEM_DROP_MAP_OF_EROHIM_KUNDUN_20050610
			ItemSerialCreateSend(lpObj->m_Index, btMapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index, ExOption);
			LogAddTD("[○Kundun EVENT] ★KUNDUN Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					btMapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );
			return TRUE;
		}
	}

	return TRUE;

}
#endif


#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303

BOOL CItemBagEx::DropCastleHuntZoneBossReward(int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY)		// 성사냥터 보스몹 아이템 드롭
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
		return FALSE;
	
	float dur;
	int type, level, x, y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{
		// 100% 확률로 이벤트용 아이템 증정
		if( 1 )
		{
			DropItemNum = (rand()%GetBagCount());
			dur  = 0;

			if( cX==0 && cY==0 )
			{
				x   = lpObj->X;
				y   = lpObj->Y;
			}
			else
			{
				x	= cX;
				y	= cY;
			}
			

			// 레벨
			level= GetLevel(DropItemNum);
			type = ItemGetNumberMake(BagObject[DropItemNum].m_type, BagObject[DropItemNum].m_index);

			if( type == -1 )
				return FALSE;

			if( BagObject[DropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				Option1 = 1;
			}

			if( BagObject[DropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				Option2 = 0;
				if( rand()%2 == 0 )
					Option2 = 1;
			}

			if( BagObject[DropItemNum].m_isoption )
			{
				if( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else Option3 = (rand()%3);
			}

			if( BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(0);
				// 액설런트 아이템은 행운이 붙지 않는다.
				Option2 = 0;
				Option1 = 1;
			}
			
			// 혼석, 축석, 영석은 레벨이 없게
			if( (type == MAKE_ITEMNUM(12, 15)) ||
				(type == MAKE_ITEMNUM(14, 13)) ||
				(type == MAKE_ITEMNUM(14, 14)) )
			{	
				Option1 = 0; 
				Option2 = 0;
				Option3 = 0;
				level   = 0;
			}

			if( (type == MAKE_ITEMNUM(13, 0)) ||
				(type == MAKE_ITEMNUM(13, 1)) ||				
				(type == MAKE_ITEMNUM(13, 2)) ||
				(type == MAKE_ITEMNUM(13, 8)) ||
				(type == MAKE_ITEMNUM(13, 9)) ||
				(type == MAKE_ITEMNUM(13, 12)) ||
				(type == MAKE_ITEMNUM(13, 13)) )
			{	// 반지류는 레벨 0
				level   = 0;
			}

			// MODIFY_ITEM_DROP_MAP_OF_EROHIM_KUNDUN_20050610
			ItemSerialCreateSend(lpObj->m_Index, btMapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, lpObj->m_Index, ExOption);
			LogAddTD("[Castle HuntZone] ★Castle HuntZone Boss ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d", 
					lpObj->AccountID, lpObj->Name, m_sEventName, 
					btMapNumber, x, y,
					ItemAttribute[type].Name, type, level, Option1, Option2, Option3 );
			return TRUE;
		}
	}

	return TRUE;

}
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404	// 크라이울프 다크엘프 아이템 드랍
BOOL CItemBagEx::DropCrywolfDarkElfItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
	{
		return FALSE;
	}
	
	float fDur		= 0;
	int iType		= 0;
	int iLevel		= 0;
	int X			= 0;
	int Y			= 0;
	int iOption1	= 0;
	int iOption2	= 0;
	int iOption3	= 0;
	int iDropItemNum	= 0;
	int	iDropItemRate	= 0;
	int iExOption	= 0;
	

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 100% 의 확률로 아이템 증정		
		iDropItemRate = rand() % 10000;
		if( iDropItemRate < g_iCrywolfMonsterDarkElfDropZenRate )
		{			
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iCrywolfMonsterDarkElfDropZen, cX, cY );	
			
			LogAddTD("[ Crywolf ][Reward] Dark Elf Item Drop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iCrywolfMonsterDarkElfDropZen );		
		}
		else
		{
			iDropItemNum = ( rand() % GetBagCount() );
			fDur  = 0;

			if( cX == 0 && cY == 0 )
			{
				X   = lpObj->X;
				Y   = lpObj->Y;
			}
			else
			{
				X	= cX;
				Y	= cY;
			}			

			// 레벨
			iLevel	= GetLevel( iDropItemNum );
			iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

			if( iType == -1 )
			{
				return FALSE;
			}

			if( BagObject[iDropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				iOption1 = 1;
			}

			if( BagObject[iDropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				iOption2 = 0;
				if( ( rand() % 2 ) == 0 )
				{
					iOption2 = 1;
				}
			}

			if( BagObject[iDropItemNum].m_isoption )
			{
				if( ( rand() % 5 ) < 1 )
				{
					iOption3 = 3;
				}
				else 
				{
					iOption3 = ( rand() % 3 );
				}
			}

			if( BagObject[iDropItemNum].m_isexitem )
			{
				iExOption = NewOptionRand(0);
				// 액설런트 아이템은 행운이 붙지 않는다.
				iOption2 = 0;
				iOption1 = 1;

				// 레벨은 0이다.
				iLevel	= 0;
			}

			ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
			LogAddTD( "[ Crywolf ][Reward] Dark Elf Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
					lpObj->AccountID, lpObj->Name,  
					btMapNumber, X, Y,
					ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );
			return TRUE;
		}
	}

	return TRUE;		
}
#endif	// #ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404	// 크라이울프 발가스 아이템 드랍
BOOL CItemBagEx::DropCrywolfBossMonsterItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
	{
		return FALSE;
	}
	
	float fDur		= 0;
	int iType		= 0;
	int iLevel		= 0;
	int X			= 0;
	int Y			= 0;
	int iOption1	= 0;
	int iOption2	= 0;
	int iOption3	= 0;
	int iDropItemNum	= 0;
	int	iDropItemRate	= 0;
	int iExOption	= 0;
	

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 100% 의 확률로 아이템 증정		
		iDropItemRate = rand() % 10000;
		if( iDropItemRate < g_iCrywolfBossMonsterDropZenRate )
		{			
			// 젠 드롭
			MapC[lpObj->MapNumber].MoneyItemDrop( g_iCrywolfBossMonsterDropZen, cX, cY );	
			
			LogAddTD("[ Crywolf ][Reward] Boss Monster Item Drop [%s][%s] [%d Zen]", 
													lpObj->AccountID, lpObj->Name, g_iCrywolfBossMonsterDropZen );		
		}
		else
		{
			iDropItemNum = ( rand() % GetBagCount() );
			fDur  = 0;

			if( cX == 0 && cY == 0 )
			{
				X   = lpObj->X;
				Y   = lpObj->Y;
			}
			else
			{
				X	= cX;
				Y	= cY;
			}			

			// 레벨
			iLevel	= GetLevel( iDropItemNum );
			iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

			if( iType == -1 )
			{
				return FALSE;
			}

			if( BagObject[iDropItemNum].m_isskill )
			{	// 스킬이 붙을수 있으면
				iOption1 = 1;
			}

			if( BagObject[iDropItemNum].m_isluck )
			{	
				// 행운이 붙을수 있으면
				iOption2 = 0;
				if( ( rand() % 2 ) == 0 )
				{
					iOption2 = 1;
				}
			}

			if( BagObject[iDropItemNum].m_isoption )
			{
				if( ( rand() % 5 ) < 1 )
				{
					iOption3 = 3;
				}
				else 
				{
					iOption3 = ( rand() % 3 );
				}
			}

			if( BagObject[iDropItemNum].m_isexitem )
			{
				iExOption = NewOptionRand(0);
				// 액설런트 아이템은 행운이 붙지 않는다.
				iOption2 = 0;
				iOption1 = 1;

				// 레벨은 0이다.
				iLevel	= 0;
			}

			ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
			LogAddTD( "[ Crywolf ][Reward] Boss Monster Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
					lpObj->AccountID, lpObj->Name,  
					btMapNumber, X, Y,
					ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );
			return TRUE;
		}
	}

	return TRUE;	
}
#endif	//ADD_CRYWOLF_MONSTER_ITEMDROP_20060404

#ifdef KANTURU_MONSTER_ITEMDROP_20060807
BOOL CItemBagEx::DropKanturuMayaHandItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
	{
		return FALSE;
	}
	
	float fDur		= 0;
	int iType		= 0;
	int iLevel		= 0;
	int X			= 0;
	int Y			= 0;
	int iOption1	= 0;
	int iOption2	= 0;
	int iOption3	= 0;
	int iDropItemNum	= 0;
	int	iDropItemRate	= 0;
	int iExOption	= 0;
	

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 20060828 (RKFKA) 마야 손 젠 드롭 삭제
		iDropItemNum = ( rand() % GetBagCount() );
		fDur  = 0;

		if( cX == 0 && cY == 0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}			

		// 레벨
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

		if( BagObject[iDropItemNum].m_isskill )
		{	// 스킬이 붙을수 있으면
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// 행운이 붙을수 있으면
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand(0);
			// 액설런트 아이템은 행운이 붙지 않는다.
			iOption2 = 0;
			iOption1 = 1;

			// 레벨은 0이다.
			iLevel	= 0;
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[ KANTURU ][ Reward ] MayaHand Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name,  
				btMapNumber, X, Y,
				ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );
		return TRUE;
	}

	return TRUE;		
}

BOOL CItemBagEx::DropKanturuNightmareItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
	{
		return FALSE;
	}
	
	float fDur		= 0;
	int iType		= 0;
	int iLevel		= 0;
	int X			= 0;
	int Y			= 0;
	int iOption1	= 0;
	int iOption2	= 0;
	int iOption3	= 0;
	int iDropItemNum	= 0;
	int	iDropItemRate	= 0;
	int iExOption	= 0;
	

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if( GetBagCount() > 0 )
	{		
		// 20060828 (RKFKA) 나이트메어 아이템드롭 변경
		iDropItemNum = ( rand() % GetBagCount() );
		fDur  = 0;

		if( cX == 0 && cY == 0 )
		{
			X   = lpObj->X;
			Y   = lpObj->Y;
		}
		else
		{
			X	= cX;
			Y	= cY;
		}			

		// 레벨
		iLevel	= GetLevel( iDropItemNum );
		iType	= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );

		if( iType == -1 )
		{
			return FALSE;
		}

		if( BagObject[iDropItemNum].m_isskill )
		{	// 스킬이 붙을수 있으면
			iOption1 = 1;
		}

		if( BagObject[iDropItemNum].m_isluck )
		{	
			// 행운이 붙을수 있으면
			iOption2 = 0;
			if( ( rand() % 2 ) == 0 )
			{
				iOption2 = 1;
			}
		}

		if( BagObject[iDropItemNum].m_isoption )
		{
			if( ( rand() % 5 ) < 1 )
			{
				iOption3 = 3;
			}
			else 
			{
				iOption3 = ( rand() % 3 );
			}
		}

		if( BagObject[iDropItemNum].m_isexitem )
		{
			iExOption = NewOptionRand(0);
			// 액설런트 아이템은 행운이 붙지 않는다.
			iOption2 = 0;
			iOption1 = 1;

			// 레벨은 0이다.
			iLevel	= 0;
		}

		ItemSerialCreateSend( lpObj->m_Index, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption );
		LogAddTD( "[ KANTURU ][ Reward ] Nightmare Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name,  
				btMapNumber, X, Y,
				ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption );
		return TRUE;
	}
	return TRUE;		
}
#endif //#ifdef KANTURU_MONSTER_ITEMDROP_20060807

#ifdef PCBANG_COUPON_EVENT_20060124
BOOL CItemBagEx::DropPCBangRevitalizationEventItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
	{
		return FALSE;
	}
	
	float fDur			= 0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int iDropItemNum	= 0;
	int	iDropItemRate	= 0;
	int iExOption		= 0;
	
	if( GetBagCount() > 0 )
	{		
		iDropItemNum = ( rand() % GetBagCount() );
		fDur		= 0;
		X			= cX;
		Y			= cY;
		iLevel		= GetLevel( iDropItemNum );
		iType		= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );
		if( iType == -1 )
			return FALSE;
			
		// 혼석, 축석, 영석은 레벨이 없게
		if( ( iType == MAKE_ITEMNUM( 12, 15 ) ) ||
			( iType == MAKE_ITEMNUM( 14, 13 ) ) ||
			( iType == MAKE_ITEMNUM( 14, 14 ) ) )
		{	
			iOption1 = 0; 
			iOption2 = 0;
			iOption3 = 0;
			iLevel   = 0;
		}

		if( ( iType == MAKE_ITEMNUM( 13, 0 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 1 ) ) ||				
			( iType == MAKE_ITEMNUM( 13, 2 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 8 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 9 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 12) ) ||
			( iType == MAKE_ITEMNUM( 13, 13 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 38 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 39 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 40 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 41 ) ) )
		{	// 반지류는 레벨 0
			iLevel   = 0;

			// 내구도는 255
			fDur	= 255;
		}

		ItemSerialCreateSend( aIndex, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, aIndex, iExOption );
		LogAddTD( "[ PCBangRevitalizationEvent ] [%s][%s][%s] ItemDrop : Item:%d Dur:%d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr, iType, fDur );
		return TRUE;
	}
	
	return FALSE;
}
#endif	// PCBANG_COUPON_EVENT_20060124

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822	//레오 아이템 드랍 함수 처리
char* CItemBagEx::GetEventName()
{
	return m_sEventName;
}

BOOL CItemBagEx::DropItemGiveEventItem( int aIndex, BYTE btMapNumber, UCHAR cX, UCHAR cY )
{
	if( !m_bLoad )					// 아이템 스크립트가 로드되지 않았다
	{
		return FALSE;
	}
	
	float fDur			= 0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int iDropItemNum	= 0;
	int	iDropItemRate	= 0;
	int iExOption		= 0;
	
	if( GetBagCount() > 0 )
	{		
		iDropItemNum = ( rand() % GetBagCount() );
		fDur		= 0;
		X			= cX;
		Y			= cY;
		iLevel		= GetLevel( iDropItemNum );
		iType		= ItemGetNumberMake( BagObject[iDropItemNum].m_type, BagObject[iDropItemNum].m_index );
		if( iType == -1 )
			return FALSE;
			
		if( ( iType == MAKE_ITEMNUM( 13, 0 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 1 ) ) ||				
			( iType == MAKE_ITEMNUM( 13, 2 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 8 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 9 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 12) ) ||
			( iType == MAKE_ITEMNUM( 13, 13 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 38 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 39 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 40 ) ) ||
			( iType == MAKE_ITEMNUM( 13, 41 ) ) )
		{	// 반지류는 레벨 0
			iLevel   = 0;

			// 내구도는 255
			fDur	= 255;
		}

		ItemSerialCreateSend( aIndex, btMapNumber, (BYTE)X, (BYTE)Y, iType, iLevel, (BYTE)fDur, iOption1, iOption2, iOption3, aIndex, iExOption );
		LogAddTD( "[ %s ] [%s][%s][%s] ItemDrop : Item:%d Dur:%d", m_sEventName, gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr, iType, fDur );
		return TRUE;
	}
	
	return FALSE;
}
#endif	// WHITEANGEL_GET_ITEM_EVENT_20060822