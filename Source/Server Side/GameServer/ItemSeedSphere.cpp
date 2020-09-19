// ItemSeedSphere.cpp: implementation of the CItemSeedSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemSeedSphere.h"
#include "Common\ItemSocketOptionDefine.h"

#include "Common\winutil.h"

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeedItem::CSeedItem()
{
	Clear();
}

CSeedItem::~CSeedItem()
{
}

void CSeedItem::Clear()
{
	m_SeedIndex				= 0;
	m_SeedType				= 0;
	m_ItemLevel				= 0;
	m_SeedOptionValueType	= 0;
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	for( int i = 0 ; i < MAX_SPHERE_LEVEL ; i++ )
	{
		m_SeedOptionValue[i]	= 0;
	}
#else
	m_SeedOptionValue		= 0;
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
	m_SeedExtractRate		= 0;
	m_ItemCode				= 0;
}

bool CSeedItem::IsMatched(CItem* lpItem)
{
	if( lpItem == NULL )
	{
		return false;
	}

	if( lpItem->IsItem() == FALSE )
	{
		return false;
	}

	if( m_ItemCode == lpItem->m_Type && m_ItemLevel == lpItem->m_Level )
	{
		return true;
	}

	return false;
}

#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
void CSeedItem::SetOption(BYTE SeedIndex, BYTE SeedType, BYTE ItemLevel, BYTE SeedOptionValueType, BYTE SeeedOptionValue[], int ExtractRate)
#else
void CSeedItem::SetOption(BYTE SeedIndex, BYTE SeedType, BYTE ItemLevel, BYTE SeedOptionValueType, BYTE SeeedOptionValue, int ExtractRate)
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
{
	m_SeedIndex = SeedIndex;
	m_SeedType = SeedType;
	m_ItemLevel = ItemLevel;
	m_SeedOptionValueType = SeedOptionValueType;
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	for( int i = 0 ; i < MAX_SPHERE_LEVEL ; i++ )
	{
		m_SeedOptionValue[i] = SeeedOptionValue[i];
	}
#else
	m_SeedOptionValue = SeeedOptionValue;
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
	m_SeedExtractRate = ExtractRate;

	switch( SeedType )
	{
	case SEED_TYPE_FIRE:
		m_ItemCode	= MAKE_ITEMNUM( 12, 60 );
		break;
	case SEED_TYPE_WATER:
		m_ItemCode	= MAKE_ITEMNUM( 12, 61 );
		break;
	case SEED_TYPE_ICE:
		m_ItemCode	= MAKE_ITEMNUM( 12, 62 );
		break;
	case SEED_TYPE_WIND:
		m_ItemCode	= MAKE_ITEMNUM( 12, 63 );
		break;
	case SEED_TYPE_LIGHTNING:
		m_ItemCode	= MAKE_ITEMNUM( 12, 64 );
		break;
	case SEED_TYPE_EARTH:
		m_ItemCode	= MAKE_ITEMNUM( 12, 65 );
		break;
	case SEED_TYPE_UNIQUE:
		m_ItemCode	= MAKE_ITEMNUM( 12, 66 );
		break;
	}
}

CSphereItem::CSphereItem()
{
}

CSphereItem::~CSphereItem()
{
}

void CSphereItem::Clear()
{
	m_SphereLevel		= 0;
	m_ValueIncreaseRate	= 0;
	m_ItemCode			= 0;
}

void CSphereItem::SetOption(BYTE SphereLevel, int ValueIncreaseRate)
{
	m_SphereLevel		= SphereLevel;
	m_ValueIncreaseRate	= ValueIncreaseRate;

	switch( SphereLevel )
	{
	case 1:
		m_ItemCode	= MAKE_ITEMNUM( 12, 70 );
		break;
	case 2:
		m_ItemCode	= MAKE_ITEMNUM( 12, 71 );
		break;
	case 3:
		m_ItemCode	= MAKE_ITEMNUM( 12, 72 );
		break;
	case 4:
		m_ItemCode	= MAKE_ITEMNUM( 12, 73 );
		break;
	case 5:
		m_ItemCode	= MAKE_ITEMNUM( 12, 74 );
		break;
	}
}

CSeedSphereItem::CSeedSphereItem()
{
	Clear();
}

CSeedSphereItem::~CSeedSphereItem()
{
}

void CSeedSphereItem::Clear()
{
	m_SeedSphereIndex		= 0;
	m_SeedIndex				= 0;
	m_SeedType				= 0;
	m_ItemLevel				= 0;
	m_SeedOptionValueType	= 0;
	m_SeedOptionValue		= 0;
	m_ItemCode				= 0;
}

#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
bool CSeedSphereItem::SetOption(CSeedItem* SeedItem, BYTE SphereLevel)
#else
bool CSeedSphereItem::SetOption(CSeedItem* SeedItem, CSphereItem* SphereItem)
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725
{
#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	if( SeedItem == NULL || SphereLevel == 0 )
	{
		return false;
	}

	// DB에 시드 스피어 옵션 부분이 기존 아이템은 0으로 초기화 되어 있기 때문에 서버에서만 시드 스피어 인덱스를 1부터 시작한다.
	// 클라이언트 전송시엔 1을 빼고 0부터 시작하도록 설정해야 한다.

	m_SeedSphereIndex		= ( SphereLevel * 50 ) + SeedItem->m_SeedIndex;
	m_SeedIndex				= SeedItem->m_SeedIndex;
	m_SeedType				= SeedItem->m_SeedType;
	m_ItemLevel				= SeedItem->m_ItemLevel;
	m_SeedOptionValueType	= SeedItem->m_SeedOptionValueType;
	m_SphereLevel			= SphereLevel;
	m_SeedOptionValue		= SeedItem->m_SeedOptionValue[SphereLevel-1];
#else
	if( SeedItem == NULL || SphereItem == NULL )
	{
		return false;
	}

	m_SeedSphereIndex		= ( SphereItem->m_SphereLevel * 50 ) + SeedItem->m_SeedIndex;
	m_SeedIndex				= SeedItem->m_SeedIndex;
	m_SeedType				= SeedItem->m_SeedType;
	m_ItemLevel				= SeedItem->m_ItemLevel;
	m_SeedOptionValueType	= SeedItem->m_SeedOptionValueType;
	m_SphereLevel			= SphereItem->m_SphereLevel;
	m_SeedOptionValue		= BYTE(( SeedItem->m_SeedOptionValue * SphereItem->m_ValueIncreaseRate ) / 10000);
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

	switch( SeedItem->m_SeedType )
	{
	case SEED_TYPE_FIRE:
		m_ItemCode = MAKE_ITEMNUM( 12, 100 );
		break;
	case SEED_TYPE_WATER:
		m_ItemCode = MAKE_ITEMNUM( 12, 101 );
		break;
	case SEED_TYPE_ICE:
		m_ItemCode = MAKE_ITEMNUM( 12, 102 );
		break;
	case SEED_TYPE_WIND:
		m_ItemCode = MAKE_ITEMNUM( 12, 103 );
		break;
	case SEED_TYPE_LIGHTNING:
		m_ItemCode = MAKE_ITEMNUM( 12, 104 );
		break;
	case SEED_TYPE_EARTH:
		m_ItemCode = MAKE_ITEMNUM( 12, 105 );
		break;
	case SEED_TYPE_UNIQUE:
		break;
	default:
		return false;
	}

	m_ItemCode += ( ( m_SphereLevel - 1 ) * 6 );

	return true;
}

void CSeedSphereItem::AddOptionValue(BYTE btOptionValue)
{
	if( m_SeedOptionValue + btOptionValue > 65534 )
	{
		return;
	}

	m_SeedOptionValue += btOptionValue;
}

#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415