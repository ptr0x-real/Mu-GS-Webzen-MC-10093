// MapClass.cpp: implementation of the MapClass class.
//
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include "Include\prodef.h"
#include "Common\winutil.h"
#include "User.h"
#include "Protocol.h"
#include "wsGameServer.h"
#include "MapClass.h"
#include "Event.h"
#include "GameMain.h"
#include "Include\define.h"

#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif

#ifdef MONSTER_HERD_SYSTEM_20031120
#include "MonsterHerd.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif


//----------------------------------------------------------------------------
// E X T E R N
//----------------------------------------------------------------------------
extern CwsGameServer	wsGServer;


#define TERRAIN_SCALE     100.f
#define TERRAIN_SIZE      256
#define TERRAIN_SIZE_MASK 255


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



MapClass::MapClass()
{
	if(g_TerrainManager.Size() < 1)
		g_TerrainManager.Load();

	gRegenRect = new RECT[g_TerrainManager.Size()];

	m_Weather = 0;
	m_WeatherTimer = GetTickCount();
	m_NextWeatherTimer = (10*1000)+(rand()%(10*1000));
	init();

	SetRect(&gRegenRect[0], 130,116,151,137);	// 로렌시아
	//SetRect(&gRegenRect[1], 130,116,151,137);	// 던전 
	SetRect(&gRegenRect[1], 106,236,112,243);	// 던전 
	SetRect(&gRegenRect[2], 197, 35,218, 50);	// 데비아스
	SetRect(&gRegenRect[3], 174,101,187,125);   // 요정숲
	SetRect(&gRegenRect[4], 201, 70,213, 81);   // LOST TOWER
	SetRect(&gRegenRect[5],  89,135, 90,136);   // 쿤둔맵
	SetRect(&gRegenRect[6],  89,135, 90,136);   // 
	SetRect(&gRegenRect[7],  14, 11, 27, 23);   // 아틀란스
	SetRect(&gRegenRect[8], 187, 54,203, 69);   // 사막

#ifdef ADD_NEW_MAP_AIDA_20050617
	SetRect(&gRegenRect[MAP_INDEX_AIDA], 82,	8,	87,	14);   // 아이다
#endif
	
#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
	SetRect(&gRegenRect[MAP_INDEX_CRYWOLF_FIRSTZONE], 133,	41,	140, 44);   // 크라이울프
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910
	SetRect(&gRegenRect[MAP_INDEX_ELBELAND], 40,	214,	43,	224);	   // 엘베란드
#endif
	m_ItemCount = 0;
}

MapClass::~MapClass()
{
	delete[] gRegenRect;
	free();
}

// 맵 번호를 체크한다.
BOOL MapNumberCheck(int map)
{
	if( map < 0 || map > g_TerrainManager.Size()-1 ) return FALSE;
	return TRUE;
}

// 레벨별 태어나는 위치를 얻는다.
void MapClass::GetLevelPos(short level, short & ox, short & oy )
{
	int x, y, levelpos=0;
	int loopcount=50;
	BYTE attr;
	int w, h;
	
	/*
	if( level > 0 && level <= 5 ) levelpos = 0;
	else if( level > 5 && level <= 10 ) levelpos = 1;
	else if( level > 10 && level <= 15 ) levelpos = 2;
	else levelpos = 3;
	*/
	while( loopcount-- )
	{
		w = gRegenRect[levelpos].right-gRegenRect[levelpos].left;
		h = gRegenRect[levelpos].bottom-gRegenRect[levelpos].top;
		
		__try
		{
			x = (short)(gRegenRect[levelpos].left+(rand()%w));
			y = (short)(gRegenRect[levelpos].top +(rand()%h));
		}
		__except( w=1, h=1, EXCEPTION_EXECUTE_HANDLER )
		{
		}
		
		attr = GetAttr(x, y);
		if( ((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) && 
			//((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) &&
			((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) 
		{
			ox = x;
			oy = y;
			return;
		}
	}
	LogAdd(lMsg.Get(460),__FILE__, __LINE__);
	x = gRegenRect[levelpos].left;
	y = gRegenRect[levelpos].top;
}


// 지정된 위치에서 지정된 거리 내로 아무 위치나 리턴한다.
void MapClass::GetRandomLengthPos(SHORT & x, SHORT & y, int length)
{
	int lc=10;
	int px, py;
	BYTE attr;
	int maxlen;
	
	if( length < 1 ) length = 1;
	maxlen = length*2;

	while(lc--)
	{
		px = (x-length)+(rand()%maxlen);
		py = (y-length)+(rand()%maxlen);
		
		attr = GetAttr(px, py);
		if( ((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) && 
			((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) 
		{
			x = px;
			y = py;
			return;
		}
	}
}

// 레벨별 태어나는 위치를 얻는다.
void MapClass::GetMapPos(short Map, short & ox, short & oy )
{
	if( Map != 8 )
	{
#ifdef ADD_NEW_MAP_AIDA_20050617
		if( Map == MAP_INDEX_AIDA )
		{
			Map = MAP_INDEX_AIDA;
		}
		else
#endif

#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
		if( Map == MAP_INDEX_CRYWOLF_FIRSTZONE )
		{
			Map = MAP_INDEX_CRYWOLF_FIRSTZONE;
		}
		else
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910
		if( Map == MAP_INDEX_ELBELAND )
		{
			Map = MAP_INDEX_ELBELAND;
		}
		else
#endif
		if( Map > 4 && Map != 7 ) 
			Map = 0;

	}

	int x, y, levelpos=Map;
	int loopcount=50;
	BYTE attr;
	int w, h;

	while( loopcount-- )
	{
		w = gRegenRect[levelpos].right-gRegenRect[levelpos].left;
		h = gRegenRect[levelpos].bottom-gRegenRect[levelpos].top;
		__try
		{
			x = (short)(gRegenRect[levelpos].left+(rand()%w));
			y = (short)(gRegenRect[levelpos].top +(rand()%h));
		}
		__except(w=1, h=1, EXCEPTION_EXECUTE_HANDLER )
		{
		}	

		attr = GetAttr(x, y);
		if( ((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) && 
			//((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) &&
			((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) 
		{
			ox = x;
			oy = y;
			return;
		}
	}
	LogAdd(lMsg.Get(460),__FILE__, __LINE__);
	x = gRegenRect[levelpos].left;
	y = gRegenRect[levelpos].top;
}

void MapClass::LoadMapAttr(char *filename, int MapNumber)
{
	if( AttrLoad(filename) == FALSE ) 
	{
		MsgBox(lMsg.Get(564));
	}
	thisMapNumber = MapNumber;
}

// 현재 맵의 날씨를 얻는다
BYTE MapClass::GetWeather()
{
	BYTE	weather;

	weather  = m_Weather<<4;
	weather |= m_WeatherVariation;
	

	return weather;
}

// 현재 맵의 날씨를 세팅한다.
void MapClass::SetWeather(BYTE a_weather, BYTE a_variation)
{
	LPOBJECTSTRUCT	lpObj;
	BYTE	weather;

	m_Weather = a_weather;
	m_WeatherVariation = a_variation;

	weather  = m_Weather<<4;
	weather |= m_WeatherVariation;

	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		lpObj = (LPOBJECTSTRUCT)&gObj[n];		
		if( lpObj->Connected > 1 && lpObj->Live && (lpObj->MapNumber == thisMapNumber ) )
		{
			CGWeatherSend(n, weather);
		}		
	}
}


void MapClass::WeatherVariationProcess()
{
	LPOBJECTSTRUCT	lpObj;
	BYTE	weather;

	if( (GetTickCount()-m_WeatherTimer) > m_NextWeatherTimer )
	{
		m_WeatherTimer		= GetTickCount();
		m_NextWeatherTimer  = (10*1000)+(rand()%(10*1000));
		m_WeatherVariation  = rand()%10;
		m_Weather		    = rand()%3;
		
		weather  = m_Weather<<4;
		weather |= m_WeatherVariation;
		
		for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
		{
			lpObj = (LPOBJECTSTRUCT)&gObj[n];			
			if( lpObj->Connected > 1 && lpObj->Live && (lpObj->MapNumber == thisMapNumber) )
			{
				CGWeatherSend(n, weather);
			}
		}
	}
}

//--------------------------------------------------------------------
// 게임에 사용되는 아이테 정보를 파일로 저장해 준다.
// *** 주의 !!!! 아이템이 생성 되므로 사용후 다시 컴파일 해야한다.
void MapClass::SaveItemInfo()
{
	/*
	FILE *fp;
	int item_type, count=0;
	
	fp = fopen("iteminfo.txt", "w");
	if( fp == NULL ) 
	{
		MsgBox("File create error %s %d", __FILE__, __LINE__);
		return;
	}
	//int i=0;
	for( int i=0; i<15; i++)
	{
		if( i== 0)
		{
			fprintf(fp, "               이름 : 최소데미지,최대데미지,내구력,요구힘,요구방어력\n");
		}
		else if( i == 6 )
		{
			fprintf(fp, "\n               이름 : 방어율,내구력,요구힘,요구방어력\n");
		}
		else if( i == 7 )
		{
			fprintf(fp, "\n               이름 : 방어력,마법방어력,내구력,요구힘,요구방어력\n");
		}
		
		for( int n=0; n<15; n++, count++)
		{
 		    item_type = ItemGetNumberMake( i, n);
			if( item_type >= 0 ) 
			{			
				m_cItem[count].CreateItem(item_type, 0, 180+n, 80+i, 0, 0, 0, 0);
				if( i < 6 )
				{
					fprintf(fp, "%18s : %4d %4d %4d %4d %4d  \n", \
						m_cItem[count].GetName(), 						
						m_cItem[count].m_DamageMin, 
						m_cItem[count].m_DamageMax, 
						m_cItem[count].m_Durability, 
						m_cItem[count].m_RequireStrength, 
						m_cItem[count].m_RequireDexterity);
				}
				else if( i == 6 )
				{
					fprintf(fp, "%18s : %4d %4d %4d %4d  \n", \
						m_cItem[count].GetName(), 						
						m_cItem[count].m_SuccessfulBlocking,
						m_cItem[count].m_Durability, 
						m_cItem[count].m_RequireStrength, 
						m_cItem[count].m_RequireDexterity);
				}
				else if( i >= 7 && i <= 11)
				{
					fprintf(fp, "%18s : %4d %4d %4d %4d %4d  \n", \
						m_cItem[count].GetName(), 						
						m_cItem[count].m_Defense,
						m_cItem[count].m_MagicDefense,
						m_cItem[count].m_Durability, 
						m_cItem[count].m_RequireStrength, 
						m_cItem[count].m_RequireDexterity);
				}
			}
		}
	}

	fclose(fp);
	*/
}

//--------------------------------------------------------------------
// 아이템 정보를 초기화 한다.
void MapClass::ItemInit()
{

}

//--------------------------------------------------------------------
// 몬스터가 맵에 아이템을 떨군다.
#ifdef FOR_BLOODCASTLE
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		INT MapClass::MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, DWORD number, BYTE ItemEffectEx, BYTE SocketOption[], BYTE SocketBonusOption)
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			INT MapClass::MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, DWORD number, BYTE ItemEffectEx)
		#else
			INT MapClass::MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, DWORD number)
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
{
	int  count, counttot=0;
	BYTE	attr;

	//못가는 곳이라면 아이템도 못 버리게..
	attr = GetAttr(x, y);
	if( ((attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) ||
		((attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW) ) 
	{		
		return -1;
	}
	
	count = m_ItemCount;
	while( TRUE )
	{
		// 비어있는 공간을 찾아서..
		if( m_cItem[count].IsItem() == FALSE )
		{
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			m_cItem[count].CreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number, ItemEffectEx, SocketOption, SocketBonusOption );
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
			m_cItem[count].CreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number, ItemEffectEx);
	#else
			m_cItem[count].CreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number);
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			m_cItem[count].m_UserIndex = aIndex;
			m_ItemCount++;
			if( m_ItemCount > MAX_MAPITEM-1 ) m_ItemCount = 0;
			return count;
		}
		count++;
		if( count > MAX_MAPITEM-1 ) count=0;
		counttot++;
		if( counttot > MAX_MAPITEM-1 ) break;
	}
	LogAdd(lMsg.Get(461), __FILE__, __LINE__);
	return -1;
}
#else
BOOL MapClass::MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, DWORD number)
{
	int  count, counttot=0;
	BYTE	attr;

	//못가는 곳이라면 아이템도 못 버리게..
	attr = GetAttr(x, y);
	if( ((attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) ||
		((attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW) ) 
	{		
		return FALSE;
	}
	
	count = m_ItemCount;
	while( TRUE )
	{
		// 비어있는 공간을 찾아서..
		if( m_cItem[count].IsItem() == FALSE )
		{
			m_cItem[count].CreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number);
			m_cItem[count].m_UserIndex = aIndex;
			m_ItemCount++;
			if( m_ItemCount > MAX_MAPITEM-1 ) m_ItemCount = 0;
			return TRUE;
		}
		count++;
		if( count > MAX_MAPITEM-1 ) count=0;
		counttot++;
		if( counttot > MAX_MAPITEM-1 ) break;
	}
	LogAdd(lMsg.Get(461), __FILE__, __LINE__);
	return FALSE;
}
#endif


//--------------------------------------------------------------------
// aIndex 유저가 맵에 아이템을 버린다.
#ifdef DARKLORD_WORK
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		BOOL MapClass::ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex, int PetLevel, int PetExp, BYTE ItemEffectEx, BYTE SocketOption[], BYTE SocketBonusOption)
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			BOOL MapClass::ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex, int PetLevel, int PetExp, BYTE ItemEffectEx)
		#else
			BOOL MapClass::ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex, int PetLevel, int PetExp)
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#else	// #ifdef DARKLORD_WORK
		BOOL MapClass::ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex)
#endif	// #ifdef DARKLORD_WORK
{
	int  count, counttot=0;
	BYTE	attr;

	//못가는 곳이라면 아이템도 못 버리게..
	attr = GetAttr(x, y);
	if( ((attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) ||
		((attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW) ) return FALSE;
	
	count = m_ItemCount;
	while( TRUE )
	{
		// 비어있는 공간을 찾아서..
		if( m_cItem[count].IsItem() == FALSE )
		{
#ifdef DARKLORD_WORK
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			m_cItem[count].DropCreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number, PetLevel, PetExp, ItemEffectEx, SocketOption, SocketBonusOption);
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
				m_cItem[count].DropCreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number, PetLevel, PetExp, ItemEffectEx);
		#else
				m_cItem[count].DropCreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number, PetLevel, PetExp);
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#else
				m_cItem[count].DropCreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number);
#endif
			m_cItem[count].m_UserIndex = aIndex;
			m_ItemCount++;
			if( m_ItemCount > MAX_MAPITEM-1 ) m_ItemCount = 0;
			
#ifdef FOR_BLOODCASTLE
			if (CHECK_BLOODCASTLE(thisMapNumber)) {
				// 사실 여기가 진짜
				if (type == MAKE_ITEMNUM(13, 19)) {
					if (CHECK_LIMIT(level, 3)) {
						// 블러드캐슬의 대천사 시리즈라면 현재 해당 블러드캐슬의 대천사 시리얼과 비교해서 맞으면 가지고 있는 사람정보 초기화
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경
						int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( thisMapNumber );
						if (g_BloodCastle.m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL != -1) {
							if (number == g_BloodCastle.m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL) {
								if (g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX != -1) {
									LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Dropped Angel King's Weapon (%d)",
										iBridgeIndex + 1,
										gObj[g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX].AccountID,
										gObj[g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX].Name,
										g_BloodCastle.m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER
										);

									g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = -1;
									g_BloodCastle.m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER = 0;
								}
							}
						}
#else						
						if (g_BloodCastle.m_BridgeData[thisMapNumber - MAP_INDEX_BLOODCASTLE1].m_nBC_QUESTITEM_SERIAL != -1) {
							if (number == g_BloodCastle.m_BridgeData[thisMapNumber - MAP_INDEX_BLOODCASTLE1].m_nBC_QUESTITEM_SERIAL) {
								if (g_BloodCastle.m_BridgeData[thisMapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_QUEST_ITEM_USER_INDEX != -1) {
									LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Dropped Angel King's Weapon (%d)",
										thisMapNumber - MAP_INDEX_BLOODCASTLE1 + 1,
										gObj[g_BloodCastle.m_BridgeData[thisMapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_QUEST_ITEM_USER_INDEX].AccountID,
										gObj[g_BloodCastle.m_BridgeData[thisMapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_QUEST_ITEM_USER_INDEX].Name,
										g_BloodCastle.m_BridgeData[thisMapNumber - MAP_INDEX_BLOODCASTLE1].m_btBC_QUEST_ITEM_NUMBER
										);

									g_BloodCastle.m_BridgeData[thisMapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_QUEST_ITEM_USER_INDEX = -1;
									g_BloodCastle.m_BridgeData[thisMapNumber - MAP_INDEX_BLOODCASTLE1].m_btBC_QUEST_ITEM_NUMBER = 0;
								}
							}
						}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
					}
				}
			}
#endif
			return TRUE;
		}
		count++;
		if( count > MAX_MAPITEM-1 ) count=0;
		counttot++;
		if( counttot > MAX_MAPITEM-1 ) return FALSE;
	}
	LogAdd(lMsg.Get(461), __FILE__, __LINE__);
	return FALSE;
}


//--------------------------------------------------------------------
// 돈을 맵에 떨어뜨린다.
BOOL MapClass::MoneyItemDrop(int money, int x, int y)
{
	int  count, counttot=0;
	
	BYTE	attr;

	//못가는 곳이라면 아이템도 못 버리게..
	attr = GetAttr(x, y);
	if( ((attr&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) ||
		((attr&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW) ) 
	{		
		return FALSE;
	}

	count = m_ItemCount;
	while( TRUE )
	{

		// 비어있는 공간을 찾아서..
		if( m_cItem[count].IsItem() == FALSE )
		//if( m_cItem[count].live == 0 && m_cItem[count].m_State == OBJST_NONE )
		{
			//m_cItem[count].Clear();
			m_cItem[count].m_Type		= GOLD_TYPE_INDEX;
			m_cItem[count].m_SellMoney  = money;
			m_cItem[count].m_BuyMoney	= money;
			m_cItem[count].px			= x;
			m_cItem[count].py			= y;
			m_cItem[count].live			= 1;
			m_cItem[count].Give         = 0;
			m_cItem[count].m_State		= OBJST_CREATE;
			m_cItem[count].m_Time       = GetTickCount()+((1000*gZenDurationTime));
			m_cItem[count].m_LootTime	= 0;//GetTickCount()+(1000*gLootingTime);
			m_ItemCount++;
			if( m_ItemCount > MAX_MAPITEM-1 ) m_ItemCount = 0;
			return TRUE;
		}
		count++;
		if( count > MAX_MAPITEM-1 ) count=0;
		counttot++;
		if( counttot > MAX_MAPITEM-1 ) return FALSE;
	}
	return FALSE;
}


//--------------------------------------------------------------------
// 맵에서 아이템을 삭제한다.
#ifdef ADD_NEW_MAP_KALIMA_20040518
BOOL MapClass::ItemGive(int aIndex, int item_num, bool bFailNotSend)
#else
BOOL MapClass::ItemGive(int aIndex, int item_num)
#endif
{	
	if( !CHECK_LIMIT( item_num, MAX_MAPITEM ) )
	{
		LogAdd(lMsg.Get(462), __FILE__, __LINE__);
		return FALSE;
	}

	if( gObj[aIndex].MapNumber != thisMapNumber )
	{
		LogAdd(lMsg.Get(463),__FILE__, __LINE__,  gObj[aIndex].MapNumber, gObj[aIndex].Name);
		return FALSE;
	}
	if( m_cItem[item_num].IsItem() == FALSE ) {
		LogAdd(lMsg.Get(464), __FILE__, __LINE__, gObj[aIndex].Name);
		return FALSE;
	}
	if( m_cItem[item_num].Give == 1 ) {
		return FALSE;
	}
	if( m_cItem[item_num].live == 0 ) 
	{
		return FALSE;
	}		

	int disx= m_cItem[item_num].px-gObj[aIndex].X;
	int disy= m_cItem[item_num].py-gObj[aIndex].Y;

	// 멀리 있는 아이템을 주울수 없다..
	if( disx > 2 || disx < -2 ) return FALSE;
	if( disy > 2 || disy < -2 ) return FALSE;

	int lootresult=1;

	if( gLootingTime > 0 )
	{
		if( m_cItem[item_num].m_UserIndex != -1 )
		{
			if( GetTickCount() < m_cItem[item_num].m_LootTime )
			{
				//LogAddL("루팅 체크 %d %d 아이템 체크", aIndex, m_cItem[item_num].m_UserIndex);
				
				// 자기 아이템이 아니면..
				if( aIndex != m_cItem[item_num].m_UserIndex )
				{
					lootresult = 0;
#ifndef THIRD_CHANGEUP_SYSTEM_20070507	// 퀘스트 아이템도 파티체크를한다.
#ifdef NEW_SKILL_FORSKYLAND
					if( !m_cItem[item_num].m_QuestItem )
					{	// 퀘스트 아이템이 아닐때는 파티 체크를 한다
#endif
#endif	// THIRD_CHANGEUP_SYSTEM_20070507
						// 파티가 있고..
						if( gObj[aIndex].PartyNumber >= 0 )
						{
							// 나의 파티원 들 이라면..
							if( gObj[aIndex].PartyNumber == gObj[m_cItem[item_num].m_UserIndex].PartyNumber )
							{
#ifdef FOR_BLOODCASTLE
								if (CHECK_BLOODCASTLE(gObj[aIndex].MapNumber)) {
									// 같은 파티원 
									if ((m_cItem[item_num].m_Type == MAKE_ITEMNUM(12, 15)) ||
										((m_cItem[item_num].m_Type == MAKE_ITEMNUM(13, 19)) && (CHECK_LIMIT(m_cItem[item_num].m_Level, 3))))
									{
										lootresult = 0;
									}
									else
										lootresult = 1;
								}
								else
									lootresult = 1;

#else
								lootresult = 1;
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 사원이벤트중 혼석은 자신만 루팅가능
								if(CHECK_ILLUSION_TEMPLE(gObj[aIndex].MapNumber))
								{
									if (m_cItem[item_num].m_Type == MAKE_ITEMNUM(12, 15))
									{
										lootresult = 0;
									}
								}
#endif

								//LogAddL("파티중 아이템 줍기 : %s %s", gObj[aIndex].Name, gObj[m_cItem[item_num].m_UserIndex].Name);
							}
						}
#ifndef THIRD_CHANGEUP_SYSTEM_20070507	// 퀘스트 아이템도 파티체크를한다. 
#ifdef NEW_SKILL_FORSKYLAND
					}
#endif
#endif
				}
			}
		}
	}

	if( lootresult == 0 )
	{
#ifdef ADD_NEW_MAP_KALIMA_20040518
		if (!bFailNotSend) {
			char szTemp[256];
			//LogAddL("error-L3 : 자신의 아이템이 아니다.%s %d/%d", gObj[aIndex].Name, aIndex, m_cItem[item_num].m_UserIndex);
			wsprintf(szTemp,lMsg.Get(1152), gObj[aIndex].Name);
			GCServerMsgStringSend(szTemp, aIndex, 1);
		}
#endif
		return FALSE;
	}

	m_cItem[item_num].m_State	= OBJST_DIECMD;
	m_cItem[item_num].Give		= 1;
	m_cItem[item_num].live		= 0;



	//LogAdd("OBJST_DIECMD1");
	return TRUE;
}

#ifdef GMCOMMAND_EX
int MapClass::ClearItem(LPOBJECTSTRUCT lpObj, int nDistance)
{
	int nCount = 0;

	for (int i=0; i<m_ItemCount; ++i)
	{
		CMapItem* pMapItem = &m_cItem[i];
		if( pMapItem->IsItem() && !pMapItem->Give && pMapItem->live && gLootingTime > 0)
		{
#ifndef GMCOMMAND_EX_1ST_BUGFIX
			if (pMapItem->m_UserIndex < 0)
				continue;
#endif // GMCOMMAND_EX_1ST_BUGFIX

			int nDisX = lpObj->X-pMapItem->px;
			int nDisY = lpObj->Y-pMapItem->py;
			int nDis = (int)( sqrt((double)(nDisX*nDisX)+(nDisY*nDisY)) );
			//if ( GetTickCount() < pMapItem->m_LootTime && nDis <= nDistance )
			if ( nDis <= nDistance )
			{
				pMapItem->m_State	= OBJST_DIECMD;
				pMapItem->Give		= 1;
				pMapItem->live		= 0;

				++nCount;
			}
		}
	}

	return nCount;
}
#endif // GMCOMMAND_EX

//--------------------------------------------------------------------
// 한번 루프를 돌았을때 상태를 전환해준다.
void MapClass::StateSetDestroy()
{
	DWORD CurTime=GetTickCount();
	for( int n=0; n<MAX_MAPITEM; n++)
	{
		// 비어있는 공간을 찾아서..		
		// 시간이 지난 아이템은 삭제한다.
		if( m_cItem[n].IsItem() )
		{
			if( m_cItem[n].m_State == OBJST_CREATE )
			{
				m_cItem[n].m_State = OBJST_PLAYING;
			}
			else if( m_cItem[n].m_State == OBJST_DIECMD )
			{
				m_cItem[n].Clear();
				m_cItem[n].live = 0;
				m_cItem[n].Give = 1;
			}
		}
		if( m_cItem[n].IsItem() )
		{
			if( m_cItem[n].m_State != OBJST_DIECMD )
			{
#ifndef FOR_ONAIR				
				if( CurTime > m_cItem[n].m_Time )
				{
					LogAddTD(lMsg.Get(1100), m_cItem[n].GetName(), m_cItem[n].m_Type, m_cItem[n].m_Level, m_cItem[n].m_Special[0], m_cItem[n].m_Number);
					//LogAddTD(lMsg.Get(1100), m_cItem[n].GetName(), m_cItem[n].m_Type, m_cItem[n].m_Level, m_cItem[n].m_Special[0]);
					m_cItem[n].m_State	= OBJST_DIECMD;

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 성물이 없어졌으므로 석상 생성시간 설정
					if(m_cItem[n].m_Type == MAKE_ITEMNUM(14, 64))
					{
						g_IllusionTempleEvent.SetStatusRegenTime(thisMapNumber);
					}
#endif
				}
#endif
			}
		}
	}
}

//----------------------------------------------------------------------------
// 맵 속성을 읽는다.
int MapClass::AttrLoad(char *filename)
{
	FILE *fp;
	BYTE  head;
	
	fp = fopen(filename, "rb");
	
	if( fp == NULL ) 
	{ 
		return FALSE; 
	}

#ifdef ENC_TILE_ATTR
	int iSize = 65539;
	BYTE byBuffer[65539];
	fread( byBuffer, iSize, 1, fp);
	BuxConvert( byBuffer, iSize);
	head = byBuffer[0];
	m_width = byBuffer[1];
	m_height = byBuffer[2];

	if( (m_width > MAX_TERRAIN_SIZE_MASK ) || (m_height > MAX_TERRAIN_SIZE_MASK) ) {
		MsgBox(lMsg.Get(563));
		fclose(fp);
		return FALSE;
	}
	if( m_attrbuf != NULL ) 
	{
		GlobalFree(m_attrbuf);
		m_attrbuf = NULL;
	}

	m_attrbuf = (LPBYTE)GlobalAlloc(GPTR, (MAX_TERRAIN_SIZE*MAX_TERRAIN_SIZE));
	memcpy( m_attrbuf, &byBuffer[3], TERRAIN_SIZE*TERRAIN_SIZE);

	fclose(fp);

	path->SetMapDimensions(MAX_TERRAIN_SIZE,MAX_TERRAIN_SIZE,m_attrbuf);
#else
	fread(&head,     1, 1, fp);
	fread(&m_width,  1, 1, fp);
	fread(&m_height, 1, 1, fp);

	if( (m_width > MAX_TERRAIN_SIZE_MASK ) || (m_height > MAX_TERRAIN_SIZE_MASK) ) {
		MsgBox(lMsg.Get(563));
		fclose(fp);
		return FALSE;
	}
	if( m_attrbuf != NULL ) 
	{
		GlobalFree(m_attrbuf);
		m_attrbuf = NULL;
	}
	m_attrbuf = (LPBYTE)GlobalAlloc(GPTR, (MAX_TERRAIN_SIZE*MAX_TERRAIN_SIZE));
	if(m_attrbuf != NULL)
	{
		fread(m_attrbuf, MAX_TERRAIN_SIZE, MAX_TERRAIN_SIZE, fp);
		path->SetMapDimensions(MAX_TERRAIN_SIZE,MAX_TERRAIN_SIZE,m_attrbuf);
	}
	
	fclose(fp);
#endif
	return TRUE;
}

BYTE MapClass::GetAttr(int x, int y)
{
	if( x < 0 ) return MAP_ATTR_BLOCK;
	if( y < 0 ) return MAP_ATTR_BLOCK;
	if( x > m_width-1 ) return MAP_ATTR_BLOCK;
	if( y > m_height-1 ) return MAP_ATTR_BLOCK;
	
	return (BYTE)(m_attrbuf[(y*MAX_TERRAIN_SIZE)+x]);
}

// 특정위치의 
BOOL MapClass::GetStandAttr(int x, int y)
{
	if( x > m_width-1 ) return FALSE;
	if( y > m_height-1 ) return FALSE;
	BYTE attr;

	attr = m_attrbuf[(y*MAX_TERRAIN_SIZE)+x];
	
	if( (attr&MAP_ATTR_STAND)	== MAP_ATTR_STAND ) return FALSE;
	if( (attr&MAP_ATTR_BLOCK)	== MAP_ATTR_BLOCK ) return FALSE;
	if( (attr&MAP_ATTR_HOLLOW)	== MAP_ATTR_HOLLOW) return FALSE;
	
	return TRUE;
}

// 서있을 수 있는 자리를 찾아본다.
void MapClass::SearchStandAttr(short & x, short & y)
{
	int scount=10;
	int tx, ty, tindex;
	int subx=1;

	tx = x;
	ty = y;
	tindex = y*MAX_TERRAIN_SIZE;

	if( (tx-10) < 0 ) subx = 1;
	if( (tx+10) > MAX_TERRAIN_SIZE_MASK ) subx = -1;

	while(scount--)
	{
		if( ((m_attrbuf[tindex+tx]&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) && 
			((m_attrbuf[tindex+tx]&MAP_ATTR_STAND) != MAP_ATTR_STAND) &&
			((m_attrbuf[tindex+tx]&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) // 비어있다면..
		{
			x = tx;
			y = ty;
			return;
		}
		tx += subx;
	}
}

// 캐릭터가 서있는 맵에 비트를 추가한다.
void MapClass::SetStandAttr(int x, int y)
{
	if( x > m_width-1 ) return;
	if( y > m_height-1 ) return;
	m_attrbuf[(y*MAX_TERRAIN_SIZE)+x] |= MAP_ATTR_STAND;
}

// 캐릭터가 지나가서 맵에 비트를 삭제한다.
void MapClass::ClearStandAttr(int x, int y)
{
#ifdef CLEAR_STANDATTR_OBJDEL_20050325
	if (!CHECK_LIMIT(x, m_width))	return;
	if (!CHECK_LIMIT(y, m_height))	return;
#else
	if( x > m_width-1 ) return;
	if( y > m_height-1 ) return;
#endif
	if( (m_attrbuf[(y*MAX_TERRAIN_SIZE)+x]&MAP_ATTR_STAND) == MAP_ATTR_STAND ) m_attrbuf[(y*MAX_TERRAIN_SIZE)+x] &= 0xFD;
}

//int test;
bool MapClass::PathFinding2(int sx,int sy,int tx,int ty,PATH_t *a)
{
	bool Success = path->FindPath(sx,sy,tx,ty,true);
	if(!Success)
	{
		Success = path->FindPath(sx,sy,tx,ty,false);
	}
	if(Success)
	{
		int PathNum = path->GetPath();

		if( PathNum > 1 )
		{
			a->PathNum = PathNum;

			unsigned char *x = path->GetPathX();
			unsigned char *y = path->GetPathY();

			for(int i=0;i<a->PathNum;i++)
			{
				a->PathX[i] = x[i];
				a->PathY[i] = y[i];
			}
			a->CurrentPath = 0;
						
			return TRUE;
		}
	}
	return FALSE;
}

inline int TERRAIN_INDEX_REPEAT(int x,int y)
{
	return (y&TERRAIN_SIZE_MASK)*TERRAIN_SIZE+(x&TERRAIN_SIZE_MASK);
}

int MapClass::CheckWall(int sx1,int sy1,int sx2,int sy2)
{
	int Index = TERRAIN_INDEX_REPEAT(sx1,sy1);//sy1*256+sy1

    int nx1,ny1,d1,d2,len1,len2;
    int px1 = sx2-sx1;
    int py1 = sy2-sy1;
    if(px1 < 0  ) {px1 = -px1;nx1 =-1           ;} else nx1 = 1;
    if(py1 < 0  ) {py1 = -py1;ny1 =-TERRAIN_SIZE;} else ny1 = TERRAIN_SIZE;
    if(px1 > py1) {len1 = px1;len2 = py1;d1 = ny1;d2 = nx1;}
    else          {len1 = py1;len2 = px1;d1 = nx1;d2 = ny1;}

	int   error = 0,count = 0;
	float Shadow = 0.f;
    do
	{
		if((m_attrbuf[Index]&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) return 0;//벽
		//if((m_attrbuf[Index]&MAP_ATTR_HOLLOW) == MAP_ATTR_HOLLOW) return 0;//땅뚤림
		error += len2;
		if(error > len1/2) 
		{
			Index += d1;
			error -= len1;
		}
		Index += d2;
	} while(++count <= len1);
	
	return 1;
}


#ifdef MONSTER_HERD_SYSTEM_20031120

bool MapClass::PathFinding3(int sx,int sy,int tx,int ty,PATH_t *a)
{
	bool Success = path->FindPath2(sx,sy,tx,ty,true);
	if(!Success)
	{
		Success = path->FindPath2(sx,sy,tx,ty,false);
	}
	if(Success)
	{
		int PathNum = path->GetPath();

		if( PathNum > 1 )
		{
			a->PathNum = PathNum;

			unsigned char *x = path->GetPathX();
			unsigned char *y = path->GetPathY();

			for(int i=0;i<a->PathNum;i++)
			{
				a->PathX[i] = x[i];
				a->PathY[i] = y[i];
			}
			a->CurrentPath = 0;
						
			return TRUE;
		}
	}
	return FALSE;
}

#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

bool MapClass::PathFinding4(int sx,int sy,int tx,int ty,PATH_t *a)
{
	// . > 시작 위치와 목표 위치의 거리가 일정 거리 이상일 경우 
	//		목표 위치와 근접 한 위치를 예측하여 이동 시킨다.

//	int iDist   = sqrt( (sx-tx) * (sx-tx) + (sy-ty) * (sx-ty) );
//	
//	if( iDist > 6 )
//	{
//		int iDirectionX[8] = { -6, -4, 0, 4, 6, 4, 0, -4 };
//		int iDirectionY[8] = { 0, -4, -6, -4, 0, 4, 6, 4 };
//
//		// . > 중간 지점 예측
//
//		int iMidMinX = -1;
//		int iMidMinY = -1;
//		int iMinCost = 10000000;
//		
//		// . > 8방향 거리가 6인 점들을 구한다.
//		for ( int i=0; i<8; i++ )
//		{
//			int iMidX = sx + iDirectionX[i];	
//			int iMidY = sy + iDirectionY[i];
//			
//			BYTE btExistWall = CheckWall2(sx, sy, iMidX, iMidY );
//			
//			if( btExistWall == 0x01 )
//				continue;
//			
//			int iMidCost = sqrt( (iMidX-tx) * (iMidX-tx) + (iMidY-ty) * (iMidY-ty) );
//			
//			if( iMidCost < iMinCost )
//			{
//				iMidMinX = iMidX;
//				iMidMinY = iMidY;
//				iMinCost = iMidCost;
//			}
//		}
//		
//		if( iMinCost != 10000000 )
//		{
//			tx = iMidMinX;
//			ty = iMidMinY;
//		}


//		int iMinCost = 10000000;
//		int iMidMinX = -1;
//		int iMidMinY = -1;
//		
//		for( int iChkDist = 6; iChkDist>2; iChkDist-- )
//		{
//			float f2SqrtDist = (float)iDist / sqrt(2);
//			
//			int iDirectionX[8] = { iChkDist*-1, f2SqrtDist*-1, 0, f2SqrtDist, iChkDist, f2SqrtDist, 0, f2SqrtDist*-1 };
//			int iDirectionY[8] = { 0, f2SqrtDist*-1, iChkDist*-1, f2SqrtDist*-1, 0, f2SqrtDist, iChkDist, f2SqrtDist };
//			
//			// . > 중간 지점 예측
//			
//			// . > 8방향 거리가 6인 점들을 구한다.
//			for ( int i=0; i<8; i++ )
//			{
//				int iMidX = sx + iDirectionX[i];	
//				int iMidY = sy + iDirectionY[i];
//				
//				BYTE btExistWall = CheckWall2(sx, sy, iMidX, iMidY );
//				
//				if( btExistWall == 0x01 )
//					continue;
//				
//				int iMidCost = sqrt( (iMidX-tx) * (iMidX-tx) + (iMidY-ty) * (iMidY-ty) );
//				
//				if( iMidCost < iMinCost )
//				{
//					iMidMinX = iMidX;
//					iMidMinY = iMidY;
//					iMinCost = iMidCost;
//				}
//			}
//
//			if( iMinCost != 10000000 )
//				break;
//		}
//		
//		if( iMinCost != 10000000 )
//		{
//			tx = iMidMinX;
//			ty = iMidMinY;
//		}
			
//	}


	bool Success = path->FindPath3(sx,sy,tx,ty,true);
	if(!Success)
	{
		Success = path->FindPath3(sx,sy,tx,ty,false);
	}
	if(Success)
	{
		int PathNum = path->GetPath();

		if( PathNum > 1 )
		{
			a->PathNum = PathNum;

			unsigned char *x = path->GetPathX();
			unsigned char *y = path->GetPathY();

			for(int i=0;i<a->PathNum;i++)
			{
				a->PathX[i] = x[i];
				a->PathY[i] = y[i];
			}
			a->CurrentPath = 0;
						
			return TRUE;
		}
	}
	return FALSE;
}

#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725


// 가고자 하는 곳까지 막혀있지는 않은지..
BYTE MapClass::CheckWall2(int sx1,int sy1,int sx2,int sy2)
{
	int Index = TERRAIN_INDEX_REPEAT(sx1,sy1);//sy1*256+sy1

    int nx1,ny1,d1,d2,len1,len2;
    int px1 = sx2-sx1;
    int py1 = sy2-sy1;
    if(px1 < 0  ) {px1 = -px1;nx1 =-1           ;} else nx1 = 1;
    if(py1 < 0  ) {py1 = -py1;ny1 =-TERRAIN_SIZE;} else ny1 = TERRAIN_SIZE;
    if(px1 > py1) {len1 = px1;len2 = py1;d1 = ny1;d2 = nx1;}
    else          {len1 = py1;len2 = px1;d1 = nx1;d2 = ny1;}

	int   start=0;
	int   error = 0,count = 0;
	float Shadow = 0.f;
    do
	{
		if( start == 0 ) 
		{
			if((m_attrbuf[Index]&MAP_ATTR_STAND) == MAP_ATTR_STAND) return MAP_ATTR_STAND;//케릭터
		}
		else start = 1;

		if((m_attrbuf[Index]&MAP_ATTR_BLOCK) == MAP_ATTR_BLOCK) return MAP_ATTR_BLOCK;//벽
		error += len2;
		if(error > len1/2) 
		{
			Index += d1;
			error -= len1;
		}
		Index += d2;
	} while(++count <= len1);
	
	return 0x01;
}
