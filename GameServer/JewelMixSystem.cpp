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
	case JEWEL_TYPE_BLESS	:		// �ູ�� ����
		{
			iItemType	= MAKE_ITEMNUM(14,13);
		}
		break;
	case JEWEL_TYPE_SOUL	:		// ��ȥ�� ����
		{
			iItemType	= MAKE_ITEMNUM(14,14);
		}
		break;
	default :
		return -1;					// ������ ������ ó������ ����
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
	case JEWEL_TYPE_BLESS	:		// �ູ�� ����
		{
			iJewelCount	= g_JewelMixInfo[JEWEL_TYPE_BLESS][iJewelLevel].m_iJewelCount;
		}
		break;
	case JEWEL_TYPE_SOUL	:		// ��ȥ�� ����
		{
			iJewelCount	= g_JewelMixInfo[JEWEL_TYPE_SOUL][iJewelLevel].m_iJewelCount;
		}
		break;
	default :
		return -1;					// ������ ������ ó������ ����
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
		// ������ - ������ ���� ���� ���տ�û(�������� ��Ŷ�� ���� ���)
		GCAnsJewelMix(iIndex, 6);
		LogAddTD("[ANTI-HACK][protocol] [Mix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return FALSE;
	}
#endif // MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202
	// �κ� ������ �ø����� �����ؼ� 0�� ���� ������, 
	// "��� �� �� �����ϴ�. GM ���� �����ϼ���"
	if( gObjCheckInventorySerial0Item(&gObj[iIndex]) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( iIndex, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
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
		// ���� Ÿ���� ������ ���
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
		// ���� Ÿ���� ������ ���
		LogAddTD("[JewelMix] [%s][%s] iMixType is out of bound : %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iMixType
			);
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelMix(iIndex, 3);
		return FALSE;
	}

	// *> . ���������� �������� Ȯ��
	INT iItemType	= g_JewelMixInfo[iJewelType][iMixType].m_iSourceType;
	INT iJewelCount	= g_JewelMixInfo[iJewelType][iMixType].m_iJewelCount;
	INT iMixMoney	= g_JewelMixInfo[iJewelType][iMixType].m_iMixMoney;
	INT iChangeType	= g_JewelMixInfo[iJewelType][iMixType].m_iChangeType;

	if (iJewelCount <= 0) {
		// ������ ���� ?
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	int iUserJewelCount	= gObjGetItemCountInIventory(iIndex, iItemType);
	if (iJewelCount > iUserJewelCount) {
		// �����ϰ� �ִ� ������ ���ڶ�
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
		// �����ϰ� �ִ� ���ձ��� ���ڶ�
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

	// *> . ���������� ����
	// 1 . �켱 �ش� ������ �ʿ��� ������ŭ �����Ѵ�.
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

	// 2 . ���⼭ �ƿ� �κ��丮�� ����� �ٽ� ������.
	GCItemListSend (iIndex);

	if (bItemDelOK == FALSE) {
		LogAddTD("[JewelMix] [%s][%s] lack of jewel to mix (in deleting) : %d / %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iDelJewelCount,
			iJewelCount
			);
		// ���� ������ ������ ���ڶ���?
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	// *> . ������ ���� ��û�� �Ѵ�.
	ItemSerialCreateSend(
		iIndex, 
		ITEMMAKE_INVENTORY_1CELL, 
		(BYTE)gObj[iIndex].X, 
		(BYTE)gObj[iIndex].Y, 
		iChangeType,	// ������ Ÿ��
		iMixType		// ������ ���� ���� 0=10, 1=20, 2=30 ���� ��
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
		// ������ - ������ ���� ���� ���տ�û(�������� ��Ŷ�� ���� ���)
		GCAnsJewelUnMix(iIndex, 9);
		LogAddTD("[ANTI-HACK][protocol] [UnMix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return FALSE;
	}
#endif // MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202
	// �κ� ������ �ø����� �����ؼ� 0�� ���� ������, 
	// "��� �� �� �����ϴ�. GM ���� �����ϼ���"
	if( gObjCheckInventorySerial0Item(&gObj[iIndex]) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( iIndex, lMsg.Get(3354) );			// "����� �� �����ϴ�. �����Ϳ� �����ϼ���."
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
		// ���� Ÿ���� ������ ���
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
		// �κ��丮 ��ġ�� �߸���
		LogAddTD("[JewelMix] [%s][%s] iInventoryPos is out of bound : %d", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			iInventoryPos
			);
		gObj[iIndex].ChaosLock	= FALSE;
		GCAnsJewelUnMix(iIndex, 5);
		return FALSE;
	}

	// *> . ������ ���翩�� Ȯ��
	INT iInventoryItemType; 
	INT iInventoryItemLevel;
	if (gObj[iIndex].pInventory[iInventoryPos].IsItem() == TRUE) {
		if (gObj[iIndex].pInventory[iInventoryPos].m_Level == iJewelLevel) {
			iInventoryItemType		= gObj[iIndex].pInventory[iInventoryPos].m_Type;
			iInventoryItemLevel		= gObj[iIndex].pInventory[iInventoryPos].m_Level;
		}
		else {
			// ������ ������ �ٸ���.
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
		// �������� �������� �ʴ´�.
		LogAddTD("[JewelMix] [%s][%s] Item to unmix is not exist", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name
			);
		gObj[iIndex].ChaosLock		= FALSE;
		GCAnsJewelUnMix(iIndex, 4);
		return FALSE;
	}

	if (!CHECK_LIMIT(iInventoryItemLevel, MAX_JM_MIX_TYPE)) {
		// ������ ������ ���� Ÿ���� ������ ���
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
		// ���� ��ü�Ϸ��� �������� �߸���
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
		// �����ϰ� �ִ� ���ձ��� ���ڶ�
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

	
	// *> . �κ��丮�� ������ ������ ���ڸ��� �����ϴ��� Ȯ��
	INT iEmptyCount		= 0;
	for(int x=0; x<MAX_INVENTORYMAP; x++)
	{
		if( gObj[iIndex].pInventoryMap[x] == 0xFF )
		{
			++iEmptyCount;
		}
	}

	if (iEmptyCount < iJewelCount) {
		// �κ��丮�� ����� ���ڸ��� ����.
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

	// *> . �������� ��ü�� �����Ѵ�.
	// 1 . �������� �������� ���� �����Ѵ�.
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

	// 2 . �ش� ������ ������ŭ �����Ѵ�.
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
				iChangeType,	// ������ Ÿ��
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
		// �������� ��ü ����
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
		// �������� ��ü ����
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
