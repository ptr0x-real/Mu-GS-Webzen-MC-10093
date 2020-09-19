// MapItem.cpp: implementation of the CMapItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapItem.h"
#include "User.h"
#include "Gamemain.h"
#include "Common\winutil.h"

#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapItem::CMapItem()
{
	Init();
}

CMapItem::~CMapItem()
{

}

//----------------------------------------------------------------------------
// 아이템을 초기화 한다.
void CMapItem::Init()
{
	VPCount = 0;
	m_State = OBJST_NONE;
	for( int n=0; n<MAXVIEWPORTOBJECT; n++)
	{
		memset((char*)&VpPlayer[n], 0, sizeof(VIEWPORT_PLAYER_STRUCT));
	}
}

//----------------------------------------------------------------------------
// 아이템을 생성한다.  dur 포함
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
void CMapItem::CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption,DWORD number, BYTE ItemEffectEx, BYTE SocketOption[], BYTE SocketBonusOption)
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
	void CMapItem::CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption,DWORD number, BYTE ItemEffectEx)
	#else
	void CMapItem::CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption,DWORD number)
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
{
	Init();
	m_Level = level;
	m_Durability = dur;

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	Convert(type, Option1,Option2,Option3, NOption, SOption, ItemEffectEx, SocketOption, SocketBonusOption);
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
	Convert(type, Option1,Option2,Option3, NOption, SOption, ItemEffectEx);
	#else
	Convert(type, Option1,Option2,Option3, NOption, SOption);
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	
	px			= x;
	py			= y;
	live		= 1;
	Give		= 0;
	m_State		= OBJST_CREATE;

#ifdef NEW_SKILL_FORSKYLAND
	if( m_QuestItem )
	{
		m_Time		= GetTickCount()+((1000*60)*1);
		m_LootTime	= GetTickCount()+((1000*60)*10);
		m_Number = number;	
	}
	else
	{
#ifdef FOR_ONAIR
		if( type == MAKE_ITEMNUM(14,11) )
		{
			m_Time  = GetTickCount()+((1000*60*60*4));
		}
		else
		{
			m_Time		= GetTickCount()+((1000*60)*2);
		}		
#else
		m_Time		= GetTickCount()+((1000*60)*2);
#endif
		m_LootTime	= GetTickCount()+(1000*gLootingTime);
		m_Number = number;	
	}
#else
	m_Time		= GetTickCount()+((1000*60)*2);
	m_LootTime	= GetTickCount()+(1000*gLootingTime);
	m_Number = number;	
#endif
}


//----------------------------------------------------------------------------
// 아이템을 생성한다.  dur 포함
#ifdef DARKLORD_WORK
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		void CMapItem::DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int PetLevel, int PetExp, BYTE ItemEffectEx, BYTE SocketOption[], BYTE SocketBonusOpion)
	#else
 		#ifdef ADD_380ITEM_NEWOPTION_20060711
			void CMapItem::DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int PetLevel, int PetExp, BYTE ItemEffectEx)
		#else
			void CMapItem::DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption,DWORD number, int PetLevel, int PetExp)
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#else
	void CMapItem::DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption,DWORD number)
#endif
{
	Init();
	m_Level = level;
	m_Durability = dur;

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	Convert(type, Option1,Option2,Option3, NOption, SOption, ItemEffectEx, SocketOption, SocketBonusOpion );
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		Convert(type, Option1,Option2,Option3, NOption, SOption, ItemEffectEx);
	#else
		Convert(type, Option1,Option2,Option3, NOption, SOption);
	#endif //ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef DARKLORD_WORK
	SetPetItemInfo(PetLevel, PetExp);
#endif

	px      = x;
	py      = y;
	live    = 1;
	Give	= 0;
	m_State	= OBJST_CREATE;

#ifdef NEW_SKILL_FORSKYLAND
	if( m_QuestItem )
	{
		if( type == MAKE_ITEMNUM(14,11) )
		{
#ifdef FOR_ONAIR
			m_Time  = GetTickCount()+((1000*60*60*2));
#else
			m_Time  = GetTickCount()+((1000*1));
#endif
		}
		else m_Time  = GetTickCount()+((1000*60)*1);
		m_LootTime	= GetTickCount()+((1000*60)*10);
		m_Number = number;
	}
	else
	{
		if( type == MAKE_ITEMNUM(14,11) )
		{
#ifdef FOR_ONAIR
			m_Time  = GetTickCount()+((1000*60*60*2));
#else
			m_Time  = GetTickCount()+((1000*1));
#endif
		}
		else m_Time  = GetTickCount()+((1000*60)*2);
		m_LootTime	= GetTickCount()+(1000*gLootingTime);
		m_Number = number;
	}
#else
	if( type == MAKE_ITEMNUM(14,11) )
	{
#ifdef FOR_ONAIR
			m_Time  = GetTickCount()+((1000*60*60*2));
#else
			m_Time  = GetTickCount()+((1000*1));
#endif
	}
	else m_Time  = GetTickCount()+((1000*60)*2);
	m_LootTime	= GetTickCount()+(1000*gLootingTime);
	m_Number = number;
#endif

#ifdef FOR_BLOODCASTLE
	if (type == MAKE_ITEMNUM(13, 19)) {
		if (CHECK_LIMIT(level, 3)) {
			// 대천사 시리즈라면 없어지는 시간만 늘리고 루팅은 바로되게 한다.
			m_Time		= GetTickCount() + BC_REWARD_CHAOSEGEM_LOOT_TIME * 3;
			m_LootTime	= GetTickCount();
		}
	}
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 성물일 경우 없어지는 시간설정
	if(type == MAKE_ITEMNUM(14, 64))
	{	
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013	// 성물 사라지는 시간 10초에서 20초로 변경
		m_Time	= GetTickCount() + 20 * 1000;
#else		
		m_Time	= GetTickCount() + 10 * 1000;
#endif
	}
#endif
}

