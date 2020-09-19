// MapClass.h: interface for the MapClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPCLASS_H__4C9BD96B_B053_451C_AD21_838336A62833__INCLUDED_)
#define AFX_MAPCLASS_H__4C9BD96B_B053_451C_AD21_838336A62833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapItem.h"
#include "..\\Common\\zzzPath.h"
#include "user.h"


#define MAX_TERRAIN_SIZE			256
#define MAX_TERRAIN_SIZE_MASK		255


#define MAP_ATTR_NONE				0x00		// ¸Ê¼Ó¼º - ¾øÀ½
#define MAP_ATTR_SAFTYZONE			0x01		// ¸Ê¼Ó¼º - ¾ÈÀüÁö´ë
#define MAP_ATTR_STAND				0x02		// ¸Ê¼Ó¼º - ÄÉ¸¯ÀÖÀ½
#define MAP_ATTR_BLOCK				0x04		// ¸Ê¼Ó¼º - º®
#define MAP_ATTR_HOLLOW				0x08		// ¸Ê¼Ó¼º - ¶¥¶Õ¸²
#define MAP_ATTR_WATER				0x10		// ¸Ê¼Ó¼º - ¹°


typedef struct
{
	int PathNum;
	int PathX[MAX_PATH_FIND];
	int PathY[MAX_PATH_FIND];
	int CurrentPath;
} PATH_t;


class MapClass
{
	BYTE	m_Weather;			// 0 ÀÌ¸é º¸Åë ³¯¾¾, 1 ºñ, 2 ´«,
	BYTE	m_WeatherVariation;	// ³¯¾¾ º¯È­ Ä«¿îÆ®
	DWORD	m_WeatherTimer;		// ³¯¾¾ º¯È­ Å¸ÀÌ¸Ó
	DWORD	m_NextWeatherTimer;	// ´ÙÀ½ ³¯¾¾ º¯È­±îÁö ½Ã°£

public :
	LPBYTE m_attrbuf;
	int    m_width;
	int    m_height;

	PATH     *path;
	// DATA
	//CMapBase	CMap;
	CMapItem	m_cItem[MAX_MAPITEM];
	int			m_ItemCount;
	
	RECT	*gRegenRect;
	int		thisMapNumber;

public:

	void IndexAutoPlus();
	void SaveItemInfo();

	void GetLevelPos(short level, short & ox, short & oy );
	void GetMapPos(short Map, short & ox, short & oy );
	void GetRandomLengthPos(SHORT & x, SHORT & y, int length);
	void SearchStandAttr(short & x, short & y);

	int  AttrLoad(char *filename);
	void LoadMapAttr(char *filename, int MapNumber);
	BOOL CheckWall(int sx1,int sy1,int sx2,int sy2);
	BYTE CheckWall2(int sx1,int sy1,int sx2,int sy2);

	void ItemInit();
	//BOOL ItemCreate(int type, int level);
#ifdef ADD_NEW_MAP_KALIMA_20040518
	BOOL ItemGive(int aIndex, int item_num, bool bFailNotSend = FALSE);
#else
	BOOL ItemGive(int aIndex, int item_num);
#endif

#ifdef GMCOMMAND_EX
	int  ClearItem(LPOBJECTSTRUCT lpObj, int nDistance);
#endif // GMCOMMAND_EX
	
#ifdef DARKLORD_WORK
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	BOOL ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex, int PetLevel, int PetExp, BYTE ItemEffectEx, BYTE SocketOption[], BYTE SocketBonusOption);
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			BOOL ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex, int PetLevel, int PetExp, BYTE ItemEffectEx);
		#else
			BOOL ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex, int PetLevel, int PetExp);
			//BOOL ItemDrop(int type, int level, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex=-1, int PetLevel = 0, int PetExp = 0);
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#else	// #ifdef DARKLORD_WORK
	BOOL ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex=-1);
	//BOOL ItemDrop(int type, int level, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex=-1);
#endif	// #ifdef DARKLORD_WORK

#ifdef FOR_BLOODCASTLE
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	INT	 MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, DWORD number=0, BYTE ItemEffectEx = 0, BYTE SocketOption[] = NULL, BYTE SocketBonusOption = SOCKETSLOT_NONE);
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			INT	 MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, DWORD number=0, BYTE ItemEffectEx = 0);
		#else
			INT	 MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, DWORD number=0);
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#else	// #ifdef FOR_BLOODCASTLE
	BOOL MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, DWORD number=0);
#endif	// #ifdef FOR_BLOODCASTLE

	BOOL MoneyItemDrop(int money, int x, int y);
	void WeatherVariationProcess();
	BYTE GetWeather();
	void SetWeather(BYTE weather, BYTE variation);
	void StateSetDestroy();
	// FUNCTION

	bool PathFinding2(int sx,int sy,int tx,int ty,PATH_t *a);
#ifdef MONSTER_HERD_SYSTEM_20031120
	bool PathFinding3(int sx,int sy,int tx,int ty,PATH_t *a);
#endif	
	
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	bool PathFinding4(int sx,int sy,int tx,int ty,PATH_t *a);
#endif

	void SetStandAttr(int x, int y);
	void ClearStandAttr(int x, int y);
	BOOL GetStandAttr(int x, int y);
	BYTE GetAttr(int x, int y);

	void init() 
	{
		m_attrbuf = NULL;
		path      = new PATH;
		m_width   = 0;
		m_height  = 0;
	};
	void free() 
	{
		if( m_attrbuf != NULL ) 
		{	
			GlobalFree( m_attrbuf );
			m_attrbuf = NULL;
		}
		if( path != NULL )
		{
			delete path;
			path = NULL;
		}
	};

	MapClass();
	virtual ~MapClass();

};

extern BOOL MapNumberCheck(int map);

#endif // !defined(AFX_MAPCLASS_H__4C9BD96B_B053_451C_AD21_838336A62833__INCLUDED_)
