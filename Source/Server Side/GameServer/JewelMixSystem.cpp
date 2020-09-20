// JewelMixSystem.cpp: implementation of the CJewelMixSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef JEWEL_MIX_SYSTEM_20050309

#include "JewelMixSystem.h"
#include "user.h"
#include "DSProtocol.h"
#include "SProtocol.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJewelMixSystem::CJewelMixSystem()
{
	
}

CJewelMixSystem::~CJewelMixSystem()
{
	
}

INT	CJewelMixSystem::GetJewelCount	(
	INT iIndex,
	INT iJewelType
	)
{
	if (!gObjIsConnected(iIndex))
		return -1;

	INT iItemType;

	switch(iJewelType) {
	case JEWEL_TYPE_BLESS	:		// 축복의 보석
		{
			iItemType	= MAKE_ITEMNUM(14,13);
		}
		break;
	case JEWEL_TYPE_SOUL	:		// 영혼의 보석
		{
			iItemType	= MAKE_ITEMNUM(14,14);
		}
		break;
	default :
		return -1;					// 조건이 없으면 처리하지 않음
		break;
	}
	
	INT iItemCount		= gObjGetItemCountInIventory(iIndex, iItemType);
	
	return iItemCount;
}


INT CJewelMixSystem::GetJewelCountPerLevel (
	INT iJewelType,
	INT iJewelLevel
	)
{
	if (!CHECK_LIMIT(iJewelLevel, 3))
		return -1;

	INT iJewelCount		= 0;
	switch(iJewelType) {
	case JEWEL_TYPE_BLESS	:		// 축복의 보석
		{
			iJewelCount	= g_JewelMixInfo[JEWEL_TYPE_BLESS][iJewelLevel].m_iJewelCount;
		}
		break;
	case JEWEL_TYPE_SOUL	:		// 영혼의 보석
		{
			iJewelCount	= g_JewelMixInfo[JEWEL_TYPE_SOUL][iJewelLevel].m_iJewelCount;
		}
		break;
	default :
		return -1;					// 조건이 없으면 처리하지 않음
		break;
	}
	
	return iJewelCount;
}


BOOL CJewelMixSystem::MixJewel (
	INT iIndex,
	INT iJewelType,
	INT iMixType
	)
{
	if (!gObjIsConnected(iIndex)) {
		return FALSE;
	}
	
#ifdef MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020
	LPINTERFACE_STATUS lpIfState = &gObj[iIndex].m_IfState;
	if( !IS_MIXJEWEL(lpIfState->use, lpIfState->type) )
	{
		// 핵유저 - 비정상 적인 보석 조합요청(보석조합 패킷만 받은 경우)
		GCAnsJewelMix(iIndex, 6);
		LogAddTD("[ANTI-HACK][protocol] [Mix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return FALSE;
	}
#endif // MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202
	// 인벤 아이템 시리얼을 조사해서 0인 것이 있으면, 
	// "사용 할 수 없습니다. GM 에게 문의하세요"
	if( gObjCheckInventorySerial0Item(&gObj[iIndex]) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( iIndex, lMsg.Get(3354) );			// "사용할 수 없습니다. 고객센터에 문의하세요."
		GCAnsJewelMix(iIndex, 4);
		LogAddTD("[ANTI-HACK][Serial 0 Item] [Mix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return FALSE;
	}
#endif // ANTIHACKING_SERIAL_0_ITEM_20051202

	if (gObj[iIndex].ChaosLock == TRUE) {
		LogAddTD("[JewelMix] [%s][%s] Chaos Mix is already working", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name
			);
		GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	gObj[iIndex].ChaosLock		= TRUE;

	if (!CHECK_LIMIT(iJewelType, MAX_JM_JEWEL_TYPE)) {
		// 보석 타입이 범위를 벗어남
		LogAddTD("[JewelMix] [%s][%s] Mix iJewelType is out of bound : %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iJewelType
			);
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelMix(iIndex, 2);
		return FALSE;
	}
	
	if (!CHECK_LIMIT(iMixType, MAX_JM_MIX_TYPE)) {
		// 조합 타입이 범위를 벗어남
		LogAddTD("[JewelMix] [%s][%s] iMixType is out of bound : %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iMixType
			);
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelMix(iIndex, 3);
		return FALSE;
	}

	// *> . 보석조합이 가능한지 확인
	INT iItemType	= g_JewelMixInfo[iJewelType][iMixType].m_iSourceType;
	INT iJewelCount	= g_JewelMixInfo[iJewelType][iMixType].m_iJewelCount;
	INT iMixMoney	= g_JewelMixInfo[iJewelType][iMixType].m_iMixMoney;
	INT iChangeType	= g_JewelMixInfo[iJewelType][iMixType].m_iChangeType;

	if (iJewelCount <= 0) {
		// 데이터 오류 ?
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	int iUserJewelCount	= gObjGetItemCountInIventory(iIndex, iItemType);
	if (iJewelCount > iUserJewelCount) {
		// 소지하고 있는 보석이 모자람
		LogAddTD("[JewelMix] [%s][%s] lack of jewel to mix : %d / %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iUserJewelCount,
			iJewelCount
			);
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelMix(iIndex, 4);
		return FALSE;
	}

	if (iMixMoney > gObj[iIndex].Money) {
		// 소지하고 있는 조합금이 모자람
		LogAddTD("[JewelMix] [%s][%s] lack of money to mix : %d / %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			gObj[iIndex].Money,
			iMixMoney
			);
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelMix(iIndex, 5);
		return FALSE;
	}

	// *> . 보석조합을 시작
	// 1 . 우선 해당 보석을 필요한 개수만큼 제거한다.
	INT iDelJewelCount	= 0;
	BOOL bItemDelOK		= FALSE;
	for(int x=MAX_EQUIPMENT; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == iItemType )
			{
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[JewelMix] [%s][%s] Mix - Delete Jewel, Type:%d, Level:%d, Serial:%u",
#else
				LogAddTD("[JewelMix] [%s][%s] Mix - Delete Jewel, Type:%d, Level:%d, Serial:%d",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					gObj[iIndex].AccountID, 
					gObj[iIndex].Name,
					gObj[iIndex].pInventory[x].m_Type,
					gObj[iIndex].pInventory[x].m_Level,
					gObj[iIndex].pInventory[x].m_Number
					);
				gObjInventoryItemSet(iIndex, x, 0xFF);
				gObj[iIndex].pInventory[x].Clear();
				
				if (iJewelCount <= ++iDelJewelCount) {
					bItemDelOK	= TRUE;
					break;
				}
			}
		}
	}

	// 2 . 여기서 아예 인벤토리의 목록을 다시 보낸다.
	GCItemListSend (iIndex);

	if (bItemDelOK == FALSE) {
		LogAddTD("[JewelMix] [%s][%s] lack of jewel to mix (in deleting) : %d / %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iDelJewelCount,
			iJewelCount
			);
		// 실제 보석의 개수가 모자랐다?
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	// *> . 아이템 생성 요청을 한다.
	ItemSerialCreateSend(
		iIndex, 
		ITEMMAKE_INVENTORY_1CELL, 
		(BYTE)gObj[iIndex].X, 
		(BYTE)gObj[iIndex].Y, 
		iChangeType,	// 아이템 타입
		iMixType		// 아이템 레벨 별로 0=10, 1=20, 2=30 개가 됨
		, (BYTE)0, 0, 0, 0, iIndex);
	
	gObj[iIndex].ChaosLock		= FALSE;

	gObj[iIndex].Money -= iMixMoney;
	GCMoneySend(iIndex, gObj[iIndex].Money);		

	GCAnsJewelMix(iIndex, 1);

	LogAddTD("[JewelMix] [%s][%s] jewel mix succeed : ItemType:%d, JewelCount:%d", 
		gObj[iIndex].AccountID, 
		gObj[iIndex].Name,
		iItemType,
		iJewelCount
		);

	return TRUE;
}


BOOL CJewelMixSystem::UnMixJewel (
	INT iIndex,
	INT iJewelType,
	INT iJewelLevel,
	INT iInventoryPos
	)
{
	if (!gObjIsConnected(iIndex))
		return FALSE;

#ifdef MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020
	LPINTERFACE_STATUS lpIfState = &gObj[iIndex].m_IfState;
	if( !IS_MIXJEWEL(lpIfState->use, lpIfState->type) )
	{
		// 핵유저 - 비정상 적인 보석 조합요청(보석조합 패킷만 받은 경우)
		GCAnsJewelUnMix(iIndex, 9);
		LogAddTD("[ANTI-HACK][protocol] [UnMix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return FALSE;
	}
#endif // MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202
	// 인벤 아이템 시리얼을 조사해서 0인 것이 있으면, 
	// "사용 할 수 없습니다. GM 에게 문의하세요"
	if( gObjCheckInventorySerial0Item(&gObj[iIndex]) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( iIndex, lMsg.Get(3354) );			// "사용할 수 없습니다. 고객센터에 문의하세요."
		GCAnsJewelUnMix(iIndex, 0);
		LogAddTD("[ANTI-HACK][Serial 0 Item] [UnMix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return FALSE;
	}
#endif // ANTIHACKING_SERIAL_0_ITEM_20051202

	if (gObj[iIndex].ChaosLock == TRUE) {
		LogAddTD("[JewelMix] [%s][%s] Chaos Mix is already working", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name
			);
		GCAnsJewelUnMix(iIndex, 0);
		return FALSE;
	}

	gObj[iIndex].ChaosLock		= TRUE;

	if (!CHECK_LIMIT(iJewelType, MAX_JM_JEWEL_TYPE)) {
		// 보석 타입이 범위를 벗어남
		LogAddTD("[JewelMix] [%s][%s] UnMix iJewelType is out of bound : %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iJewelType
			);
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelUnMix(iIndex, 2);
		return FALSE;
	}

	if (!CHECK_LIMIT(iInventoryPos, MAX_INVENTORY)) {
		// 인벤토리 위치가 잘못됨
		LogAddTD("[JewelMix] [%s][%s] iInventoryPos is out of bound : %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iInventoryPos
			);
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelUnMix(iIndex, 5);
		return FALSE;
	}

	// *> . 아이템 존재여부 확인
	INT iInventoryItemType; 
	INT iInventoryItemLevel;
	if (gObj[iIndex].pInventory[iInventoryPos].IsItem() == TRUE) {
		if (gObj[iIndex].pInventory[iInventoryPos].m_Level == iJewelLevel) {
			iInventoryItemType		= gObj[iIndex].pInventory[iInventoryPos].m_Type;
			iInventoryItemLevel		= gObj[iIndex].pInventory[iInventoryPos].m_Level;
		}
		else {
			// 아이템 레벨이 다르다.
			LogAddTD("[JewelMix] [%s][%s] iJewelLevel is different from request : %d / %d", 
				gObj[iIndex].AccountID, 
				gObj[iIndex].Name,
				gObj[iIndex].pInventory[iInventoryPos].m_Level,
				iJewelLevel
				);
			gObj[iIndex].ChaosLock	= FALSE;
			GCAnsJewelUnMix(iIndex, 3);
			return FALSE;
		}
	}
	else {
		// 아이템이 존재하지 않는다.
		LogAddTD("[JewelMix] [%s][%s] Item to unmix is not exist", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name
			);
		gObj[iIndex].ChaosLock		= FALSE;
		GCAnsJewelUnMix(iIndex, 4);
		return FALSE;
	}

	if (!CHECK_LIMIT(iInventoryItemLevel, MAX_JM_MIX_TYPE)) {
		// 아이템 레벨이 조합 타입의 범위를 벗어남
		LogAddTD("[JewelMix] [%s][%s] iInventoryItemLevel is out of bound : %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iInventoryItemLevel
			);
		gObj[iIndex].ChaosLock		= FALSE;
		GCAnsJewelUnMix(iIndex, 3);
		return FALSE;
	}

	INT iItemType		= g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iChangeType;;
	INT iJewelCount		= g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iJewelCount;
	INT iChangeType		= g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iSourceType;
	INT iMixMoney		= JEWEL_UNMIX_MONEY;

	if (iInventoryItemType != iItemType) {
		// 조합 해체하려는 아이템이 잘못됨
		LogAddTD("[JewelMix] [%s][%s] iItemType is different from request : %d / %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iInventoryItemType,
			iItemType
			);
		gObj[iIndex].ChaosLock		= FALSE;
		GCAnsJewelUnMix(iIndex, 6);
		return FALSE;
	}

	if (iMixMoney > gObj[iIndex].Money) {
		// 소지하고 있는 조합금이 모자람
		LogAddTD("[JewelMix] [%s][%s] lack of money to unmix : %d / %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			gObj[iIndex].Money,
			iMixMoney
			);
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelUnMix(iIndex, 8);
		return FALSE;
	}

	
	// *> . 인벤토리에 보석이 들어갈만한 빈자리가 존재하는지 확인
	INT iEmptyCount		= 0;
	for(int x=0; x<MAX_INVENTORYMAP; x++)
	{
		if( gObj[iIndex].pInventoryMap[x] == 0xFF )
		{
			++iEmptyCount;
		}
	}

	if (iEmptyCount < iJewelCount) {
		// 인벤토리에 충분한 빈자리가 없다.
		LogAddTD("[JewelMix] [%s][%s] lack of empty slot to unmix : %d / %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iEmptyCount,
			iJewelCount
			);
		gObj[iIndex].ChaosLock		= FALSE;
		GCAnsJewelUnMix(iIndex, 7);
		return FALSE;
	}

	// *> . 보석조합 해체를 시작한다.
	// 1 . 보석조합 아이템을 먼저 제거한다.
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[JewelMix] [%s][%s] UnMix - Delete Jewel, Type:%d, Level:%d, Serial:%u",
#else
	LogAddTD("[JewelMix] [%s][%s] UnMix - Delete Jewel, Type:%d, Level:%d, Serial:%d",
#endif / MODIFY_ITEM_SERIAL_EXPAND_20080227
		gObj[iIndex].AccountID, 
		gObj[iIndex].Name,
		gObj[iIndex].pInventory[iInventoryPos].m_Type,
		gObj[iIndex].pInventory[iInventoryPos].m_Level,
		gObj[iIndex].pInventory[iInventoryPos].m_Number
		);
	gObjInventoryItemSet(iIndex, iInventoryPos, 0xFF);
	gObjInventoryDeleteItem(iIndex, iInventoryPos);
	GCInventoryItemDeleteSend(iIndex, iInventoryPos, 1);

	// 2 . 해당 보석을 개수만큼 생성한다.
	INT iCrtJewelCount	= 0;
	BOOL bItemCrtOK		= FALSE;
	for( int x=0; x<MAX_INVENTORYMAP; x++)
	{
		if( gObj[iIndex].pInventoryMap[x] == 0xFF )
		{
			ItemSerialCreateSend(
				iIndex, 
				ITEMMAKE_INVENTORY_1CELL, 
				(BYTE)gObj[iIndex].X, 
				(BYTE)gObj[iIndex].Y, 
				iChangeType,	// 아이템 타입
				0
				, (BYTE)0, 0, 0, 0, iIndex);

			if (iJewelCount <= ++iCrtJewelCount) {
				bItemCrtOK	= TRUE;
				break;
			}
		}
	}

	gObj[iIndex].ChaosLock			= FALSE;

	if (bItemCrtOK == TRUE) {
		// 보석조합 해체 성공
		LogAddTD("[JewelMix] [%s][%s] jewel unmix succeed : ItemType:%d, JewelCount:%d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iItemType,
			iJewelCount
			);
		gObj[iIndex].Money -= iMixMoney;
		GCMoneySend(iIndex, gObj[iIndex].Money);		

		GCAnsJewelUnMix(iIndex, 1);
	}
	else {
		// 보석조합 해체 실패
		LogAddTD("[JewelMix] [%s][%s] jewel unmix failed : ItemType:%d, JewelCount:%d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iItemType,
			iJewelCount
			);
		GCAnsJewelUnMix(iIndex, 0);
	}

	return TRUE;
}


#endif
