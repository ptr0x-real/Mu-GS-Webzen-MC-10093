// JewelOfHarmonySystem.cpp: implementation of the CJewelOfHarmonySystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common\winutil.h"
#include "JewelOfHarmonySystem.h"

#ifdef MODIFY_MIX_SYSTEM_20070518
//#include "ChaosBox.h"
#include "MixSystem.h"
#endif

#include "DSProtocol.h"
#include "Include\ReadScript.h"

#include "DirPath.h"
extern CDirPath		gDirPath;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219
	#include "CastleSiegeSync.h"
#endif


#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

CJewelOfHarmonySystem	g_kJewelOfHarmonySystem;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJewelOfHarmonySystem::CJewelOfHarmonySystem()
{
	// ���� �ý��� �ʱ�ȭ : ����� �� �ִ� ����.
	m_bEnable = TRUE;

	// �� �ʱ�ȭ
	_InitOption();
}

CJewelOfHarmonySystem::~CJewelOfHarmonySystem()
{

}

void CJewelOfHarmonySystem::_InitOption()
{
	// ��ȭ�� ���� ����/������, ���ü� ������ �ε��� ����
	JEWEL_OF_HARMONY_ITEMINDEX			= MAKE_ITEMNUM(14,41); 
	JEWEL_OF_HARMONY_PURITY_ITEMINDEX	= MAKE_ITEMNUM(14,42);
	JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX = MAKE_ITEMNUM(14,43);	// �ϱ����ü�
	JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX = MAKE_ITEMNUM(14,44);	// �ϱ����ü�

	// �ɼ� ���̺� �ʱ�ȭ
	memset(m_itemOption, 0, sizeof(m_itemOption) );

	// ȯ�� ���� : �ý��� On/Off
	m_bSystemPrutiyJewel	   = GetPrivateProfileInt("GameServerInfo",	"PurityHarmonyJewel",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_bSystemMixSmeltingStone  = GetPrivateProfileInt("GameServerInfo",	"MixSmeltingStoneItem",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_bSystemRestoreStrengthen = GetPrivateProfileInt("GameServerInfo",	"RestoreStrengthenItem",	0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_bSystemStrengthenItem	   = GetPrivateProfileInt("GameServerInfo",	"StrengthenItem",			0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_bSystemSmeltingItem	   = GetPrivateProfileInt("GameServerInfo",	"SmeltItemBySmeltingStone",	0,	gDirPath.GetNewPath("commonserver.cfg"));

	// ȯ�� ���� : Ȯ�� �� �ʿ� ������
	m_iRatePuritySuccess	   = GetPrivateProfileInt("GameServerInfo",	"PuritySuccessRate",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iZenForPurity			   = GetPrivateProfileInt("GameServerInfo",	"PurityNeedZen",					0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateMixSmeltingStoneNor = GetPrivateProfileInt("GameServerInfo",	"SmeltingStoneSuccesRateNormal",	0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateMixSmeltingStoneExt = GetPrivateProfileInt("GameServerInfo",	"SmeltingStoneSuccesRateExcellent",	0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iZenForMixSmeltingStone  = GetPrivateProfileInt("GameServerInfo",	"SmeltingNeedZen",					0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateStrengthenSuccess   = GetPrivateProfileInt("GameServerInfo",	"StrengthenSuccessRate",			0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateSmeltingSuccessNor  = GetPrivateProfileInt("GameServerInfo",	"SmeltingItemSuccessRateNor",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateSmeltingSuccessExt  = GetPrivateProfileInt("GameServerInfo",	"SmeltingItemSuccessRateExt",		0,	gDirPath.GetNewPath("commonserver.cfg"));

	// �׽�Ʈ
#if TESTSERVER == 1 
	char buf[256];
	sprintf(buf, "PurityHarmonyJewel  %d \n", m_bSystemPrutiyJewel); OutputDebugString(buf);
	sprintf(buf, "MixSmeltingStoneItem  %d \n", m_bSystemMixSmeltingStone); OutputDebugString(buf);
	sprintf(buf, "RestoreStrengthenItem  %d \n", m_bSystemRestoreStrengthen); OutputDebugString(buf);
	sprintf(buf, "StrengthenItem  %d \n", m_bSystemStrengthenItem); OutputDebugString(buf);
	sprintf(buf, "SmeltItemBySmeltingStone  %d \n", m_bSystemSmeltingItem); OutputDebugString(buf);

	sprintf(buf, "PuritySuccessRate  %d \n", m_iRatePuritySuccess); OutputDebugString(buf);
	sprintf(buf, "PurityNeedZen  %d \n", m_iZenForPurity); OutputDebugString(buf);
	sprintf(buf, "SmeltingStoneSuccesRateNormal  %d \n", m_iRateMixSmeltingStoneNor); OutputDebugString(buf);
	sprintf(buf, "SmeltingStoneSuccesRateExcellent  %d \n", m_iRateMixSmeltingStoneExt); OutputDebugString(buf);
	sprintf(buf, "SmeltingNeedZen  %d \n", m_iZenForMixSmeltingStone); OutputDebugString(buf);
	sprintf(buf, "StrengthenSuccessRate  %d \n", m_iRateStrengthenSuccess); OutputDebugString(buf);
	sprintf(buf, "SmeltingItemSuccessRateNor  %d \n", m_iRateSmeltingSuccessNor); OutputDebugString(buf);
	sprintf(buf, "SmeltingItemSuccessRateExt  %d \n", m_iRateSmeltingSuccessExt); OutputDebugString(buf);
#endif
}

//--------------------------------------------------------------------
BOOL CJewelOfHarmonySystem::LoadScript( LPSTR lpszFileName )
{
	if((SMDFile=fopen(lpszFileName,"r")) == NULL)	
		return FALSE;

	// �⺻ �� �ʱ�ȭ
	_InitOption();

	// ��ũ��Ʈ ó��
	SMDToken Token;
	while(true)
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			int Type = (int)TokenNumber;
			while(true)
			{
				Token = (*GetToken)();
				int Index = (int)TokenNumber;
				if(Token==NAME && strcmp("end",TokenString)==NULL) 
					break;

				// �� ����
               	JEWELOFHARMONY_ITEM_OPTION *p = &m_itemOption[Type][Index];
				Token = (*GetToken)(); strcpy(p->szOptionName, TokenString);	// �̸� : �Ѿ
				Token = (*GetToken)();
				p->iRandomWeight = (int)TokenNumber;	// Ȯ�� ����ġ
				Token = (*GetToken)();
				p->iRequireLevel  = (int)TokenNumber;	// �䱸 ����
				for (int i = 0; i < MAX_JEWELOFHARMONY_EFFECT_VALUE; ++i )
				{
					Token = (*GetToken)();
					p->iItemEffectValue[i]  = (int)TokenNumber;		// �ɼ� ��
					Token = (*GetToken)();
					p->iZenForRestore[i]  = (DWORD)TokenNumber;		// ȯ���ʿ���
				}
				p->bValid = TRUE;
			}
		}
	}

	fclose(SMDFile);

	return TRUE;
}


// ���ü� ���� ��� ������ ����Ÿ �ε�
BOOL	CJewelOfHarmonySystem::LoadScriptOfSmelt( LPSTR lpszFileName )
{
	if((SMDFile=fopen(lpszFileName,"r")) == NULL)	
		return FALSE;

	// ��� �ʱ�ȭ
	m_mapEnableMixList.clear();

	// ��ũ��Ʈ ó��
	SMDToken Token;
	while(true)
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			int Type = (int)TokenNumber;
			while(true)
			{
				Token = (*GetToken)();
				int index = (int)TokenNumber;
				if(Token==NAME && strcmp("end",TokenString)==NULL) 
					break;
				// ���� ���� ������
				Token = (*GetToken)();					// ������ �̸�
				Token = (*GetToken)();					// �ʿ䷹��
				int reqLev = (int)TokenNumber;

				// ��� �߰�
				int idx = MAKE_ITEMNUM( Type, index);
				m_mapEnableMixList[idx] = reqLev;
			}
		}
	}

	fclose(SMDFile);

	return TRUE;
}
//--------------------------------------------------------------------
// ��ȭ�� ���� ���� üũ
BOOL CJewelOfHarmonySystem::IsJewelOfHarmonyOriginal(short type)
{
	BOOL bRet = FALSE;

	if ( type == JEWEL_OF_HARMONY_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

//--------------------------------------------------------------------
// ��ȭ�� ���� ������ üũ
BOOL CJewelOfHarmonySystem::IsJewelOfHarmonyPurity(short type)
{
	BOOL bRet = FALSE;

	if ( type == JEWEL_OF_HARMONY_PURITY_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

//--------------------------------------------------------------------
// ���ü� üũ
BOOL CJewelOfHarmonySystem::IsJewelOfHarmonySmeltingItems(short type)
{
	BOOL bRet = FALSE;

	if ( _IsJewelOfHarmonySmeltingItemNor( type ) == TRUE
		|| _IsJewelOfHarmonySmeltingItemExt( type ) == TRUE )
		bRet = TRUE;

	return bRet;
}


// �ϱ� ���ü� üũ
BOOL CJewelOfHarmonySystem::_IsJewelOfHarmonySmeltingItemNor(short type)
{
	BOOL bRet = FALSE;

	if ( type == JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

// ��� ���ü� üũ
BOOL CJewelOfHarmonySystem::_IsJewelOfHarmonySmeltingItemExt(short type)
{
	BOOL bRet = FALSE;

	if ( type == JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

//--------------------------------------------------------------------
// ���� ���� ���� ���� : �ܺο��� ����
//--------------------------------------------------------------------
void CJewelOfHarmonySystem::SetEnableToUsePuritySystem(BOOL bEnable)
{
	m_bEnable = bEnable;
	LogAddTD("[JewelOfHarmony][PuritySystem] Enable %d", bEnable);
}

//--------------------------------------------------------------------
// ���� ���� ���� ����
//--------------------------------------------------------------------
BOOL CJewelOfHarmonySystem::IsEnableToUsePuritySystem()
{
#if TESTSERVER == 1
	// �系 ������ ������ ����. ���� �׽�Ʈ�� �� �����ؾ���.
	return TRUE;
#endif
	return m_bEnable;
}


//--------------------------------------------------------------------
// ��ȭ�� ���� ����
//--------------------------------------------------------------------
void CJewelOfHarmonySystem::PurityJewelOfHarmony(LPOBJECTSTRUCT lpObj)
{
	// 0. �ý��� ��� ���� ����
	if ( m_bSystemPrutiyJewel != TRUE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3378), lpObj->m_Index, 1); // �����ý��� �̿� �Ұ�
#endif
		return ;
	}

	// 1. ���� �ý��� ��� ���� ����
	if ( IsEnableToUsePuritySystem() == FALSE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3378), lpObj->m_Index, 1); // �����ý��� �̿� �Ұ�
#endif
		LogAddTD("[JewelOfHarmony][%s][%s] Not Purtiy Time ", lpObj->AccountID, lpObj->Name);
		return;
	}

	// ChaosBox Lock
	lpObj->ChaosLock = TRUE;

	int iJewelOfHarmonyItemCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	
#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef EXTEND_LOG_SYSTEM_04_20060823
	BYTE ExOption[8];
	#endif
#endif // MODIFY_LOG_SYSTEM_2_3_20060828

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	// 1. ī��������â ������ �˻�
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == JEWEL_OF_HARMONY_ITEMINDEX )
			{	
				// ��ȭ�Ǻ���(����) ����
				++iJewelOfHarmonyItemCount;
			}
			else
			{
				iInvalidItemCount++;
			}

#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef EXTEND_LOG_SYSTEM_04_20060823
			ItemIsBufExOption( ExOption, &lpObj->pChaosBox[n]);
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[JewelOfHarmony][Purity] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d] Serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO[%d,%d]",
		#else
			LogAddTD("[JewelOfHarmony][Purity] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d] Serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO[%d,%d]",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				lpObj->AccountID, lpObj->Name, n,
				lpObj->pChaosBox[n].GetName(),
				lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_Option1,
				lpObj->pChaosBox[n].m_Option2,
				lpObj->pChaosBox[n].m_Option3,
				lpObj->pChaosBox[n].m_Number,
				(int)lpObj->pChaosBox[n].m_Durability,
				ExOption[0],
				ExOption[1],
				ExOption[2],
				ExOption[3],
				ExOption[4],
				ExOption[5],
				ExOption[6],
				lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pChaosBox[n] ),
				g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pChaosBox[n] )
				);
	#endif // EXTEND_LOG_SYSTEM_04_20060823
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
		}
	}

	if( ( iInvalidItemCount > 0 ) || ( iJewelOfHarmonyItemCount != 1 ) )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	// 2. �ʿ� �� ����
	iChaosMixPrice = m_iZenForPurity;
#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// ī���� �ý��� ��������
	INT iChaosTaxMoney = (INT)(iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100);
	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}
	iChaosMixPrice = iChaosMixPrice + iChaosTaxMoney;
	if (iChaosMixPrice < 0)
	{
		iChaosMixPrice = 0;
	}
#endif
	if( lpObj->Money < iChaosMixPrice )
	{
		pMsg.Result = 0x02;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}
	lpObj->Money = lpObj->Money - iChaosMixPrice;

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// ī���� ���� ������ ����
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );
#endif

	GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	g_MixSystem.LogChaosItem( lpObj, "JewelOfHarmony][Purity");
	LogAddTD("[JewelOfHarmony][Purity] - Mix Start");
#endif


	// 3. ���� Ȯ�� ���� �� ���� �õ�
	int iRate = rand()%100;
	if ( iRate < m_iRatePuritySuccess )	// Ȯ�� ����
	{
		int iItemType = JEWEL_OF_HARMONY_PURITY_ITEMINDEX;	// ��ȭ�Ǻ���(����)

		ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0 );
		gObjInventoryCommit( lpObj->m_Index );

#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
		g_MixSystem.LogChaosItem( lpObj, "[JewelOfHarmony][Purity]");
#endif
		LogAddTD("[JewelOfHarmony][Purity] Purity Success [%s][%s] Rate %d/%d",
			lpObj->AccountID, lpObj->Name, iRate, m_iRatePuritySuccess);
		
#ifdef MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
		return;
#endif	// MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
	}
	else
	{	
		// ���� ����	
		g_MixSystem.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[JewelOfHarmony][Purity] Purity Fail [%s][%s] Rate %d/%d",
			   lpObj->AccountID, lpObj->Name, iRate, m_iRatePuritySuccess);	
	}

	lpObj->ChaosLock = FALSE;
}

//--------------------------------------------------------------------


//---------------------------------------------------------------------
// m_JewelOfHarmonyOption  ��Ʈ ��� (����������)
// 0~3 Bit :  0 ~ 15 �ɼ� ����. 0 �ΰ�� ��ȭ�ȵ� ������
// 4~7 Bit :  0 ~ 15  �ɼ� ����
//---------------------------------------------------------------------
BYTE CJewelOfHarmonySystem::GetItemStrengthenOption(CItem *pItem)
{
	// 1��° ���� 4��°���� 4bit : �ɼ� ����	 11110000
	return (pItem->m_JewelOfHarmonyOption&JEWELOFHARMONY_BITMASK_OPTION) >> 4;
}

#ifdef EXTEND_LOG_SYSTEM_03_20060816
BYTE CJewelOfHarmonySystem::GetItemOptionLevel(CItem *pItem)
{
	return _GetItemOptionLevel( pItem );
}
#endif 

BYTE CJewelOfHarmonySystem::_GetItemOptionLevel(CItem *pItem)
{
	// 5��° ���� 8��°���� 4bit : �ɼ� ����	 00001111
	return pItem->m_JewelOfHarmonyOption&JEWELOFHARMONY_BITMASK_OPTIONLEVEL;
}

// ��ȭ �Ǿ����� ����
BOOL	CJewelOfHarmonySystem::IsStrengthenByJewelOfHarmony(CItem *pItem)
{
	// ��ȭ �ɼǰ��� 0���� ū ��츦 �ǹ�.
	if ( GetItemStrengthenOption( pItem ) > 0 )
		return TRUE;

	return FALSE;
}

// ��ȿ ���� üũ
BOOL	CJewelOfHarmonySystem::IsActive(CItem *pItem)
{
	// ��ȭ�� �������� �ƴϸ� ��Ȱ��
	if ( IsStrengthenByJewelOfHarmony( pItem ) == FALSE )
		return FALSE;
	
	// �������� ������ �ɼǷ��� ���� ���� ��� ��Ȱ��
	int iOptionLevel = _GetItemOptionLevel( pItem );
	if ( pItem->m_Level < iOptionLevel )
		return FALSE;

	return TRUE;
}

BYTE CJewelOfHarmonySystem::_GetItemOptionRequireLevel(CItem *pItem)
{
	int iItemType	 = _GetItemType( pItem );
	BYTE iItemOption = GetItemStrengthenOption( pItem );

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL
		|| iItemOption > MAX_JEWELOFHARMONY_EFFECT_OPTION 
		|| iItemOption == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		return 0;
	}

	return m_itemOption[iItemType][iItemOption].iRequireLevel;
}

// ��ȭ �ɼ� �з� ã�� 
int		CJewelOfHarmonySystem::_GetItemType(CItem *pItem)
{
	int iItemType = JEWELOFHARMONY_ITEM_TYPE_NULL;

	// 1 : �����
	if ( ( pItem->m_Type >= ITEM_SWORD  && pItem->m_Type < ITEM_BOW )					// ��(0), ����(1), �б�(2), â(3)
		|| ( pItem->m_Type >= ITEM_BOW	  && pItem->m_Type < ITEM_BOW+7)				// Ȱ(4)
		|| ( pItem->m_Type >= ITEM_BOW+8  && pItem->m_Type < ITEM_BOW+15  )				// ����(4)
		|| ( pItem->m_Type >= ITEM_BOW+16 && pItem->m_Type < ITEM_BOW+MAX_ITEM_INDEX)	// ����(4)
		)
	{
		iItemType = JEWELOFHARMONY_ITEM_TYPE_WEAPON;
	}

	// 2 : �����̷�
	if (  pItem->m_Type >= ITEM_STAFF  && pItem->m_Type < ITEM_SHIELD )				// ������(5)
	{
		iItemType = JEWELOFHARMONY_ITEM_TYPE_STAFF;
	}

	// 3 : ����
	if( pItem->m_Type >= ITEM_SHIELD && pItem->m_Type < ITEM_BOOT+MAX_ITEM_INDEX ) 	// �� : ����(6), ����(7), ����(8), ����(9), �尪(10), ����(11)
	{
		iItemType = JEWELOFHARMONY_ITEM_TYPE_DEFENSE;
	}

	return iItemType;
}


// �����ϰ� �ɼ� ���ϱ�
int		CJewelOfHarmonySystem::_GetSelectRandomOption(CItem *pItem, int iItemType)
{
	// �̿��� �������� ���� ó��.
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return iItemType;

	// �� �ʱ�ȭ
	int iItemOption = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;
	m_kRandomPool.InitPool();

	// ��ȿ�� �ɼ� ã��
	for (int idx = 1; idx < MAX_JEWELOFHARMONY_EFFECT_OPTION + 1; ++idx )
	{
		if ( m_itemOption[iItemType][idx].bValid == FALSE )
		{
			continue;
		}

		// �䱸����
		if ( pItem->m_Level < m_itemOption[iItemType][idx].iRequireLevel )
		{
			continue;
		}

		// �ʿ� �� ���� �ɼ��̳�, �� ������ �ʿ����� �ʴ� ������ �� ���.
		if ( iItemOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR
			|| iItemOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR )
		{
			if ( pItem->m_RequireStrength == 0 )
			{
				continue;
			}
		}

		// �ʿ� ��ø ���� �ɼ��̳�, ��ø ������ �ʿ����� �ʴ� ������ �� ���.
		if ( iItemOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX
			|| iItemOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX )
		{
			if ( pItem->m_RequireDexterity == 0 )
			{
				continue;
			}
		}

		// ��ȿ�� �ɼ� Pool�� �ֱ�
		m_kRandomPool.AddValue( idx, m_itemOption[iItemType][idx].iRandomWeight);
	}

	// ��÷ ��Ŀ� ���� �ɼ� �� ����
	iItemOption = m_kRandomPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
	return iItemOption;
}


// ��ȭ�Ǻ��� ��ȭ �ý���
BOOL	CJewelOfHarmonySystem::StrengthenItemByJewelOfHarmony(LPOBJECTSTRUCT lpObj, int source, int target)
{
	// 0. �⺻ ���� üũ
	if ( m_bSystemStrengthenItem == FALSE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3380), lpObj->m_Index, 1); // ��ȭ�Ǻ��� ��ȭ �ý��� ���Ұ�
#endif
		return FALSE;
	}

	if( source < 0 || source > MAX_INVENTORY-1 ) 
		return FALSE;
	if( target < 0 || target > MAX_INVENTORY-1 ) 
		return FALSE;
	
	// �������� �ִ��� üũ
	if( lpObj->pInventory[source].IsItem() == FALSE ) 
		return FALSE;
	if( lpObj->pInventory[target].IsItem() == FALSE ) 
		return FALSE;

	CItem *pSource = &lpObj->pInventory[source];
	CItem *pTarget = &lpObj->pInventory[target];

	// 1. Ÿ�� �������� �̹� ��ȭ�� ������ üũ
	if ( IsStrengthenByJewelOfHarmony(pTarget) == TRUE )
	{
		LogAddTD("[JewelOfHarmony][Strengthen Item] Already Strengtened [%s][%s]",
			   lpObj->AccountID, 
			   lpObj->Name);
		return FALSE;
	}

	// 1.5 ��Ʈ �������� ��ȭ�� �� ����.
#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
	if ( pTarget->IsSetItem() )
#else
	if ( pTarget->IsSetItem() == TRUE )
#endif
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3372), lpObj->m_Index, 1); // ��Ʈ �������� ��ȭ��ų �� �����ϴ�.
#endif
		LogAddTD("[JewelOfHarmony][Strengthen Item] SetItem not enable to Strengthened [%s][%s]",
			   lpObj->AccountID, 
			   lpObj->Name);
		return FALSE;
	}

	// 2. Ÿ�� �������� ��ȭ ������ ������ ���� üũ
	int iItemType = _GetItemType( pTarget );		// ���� / ������ /��
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		// ��ȭ ������ �������� �ƴ�. ��ȭ ���� : �̷� ���� ������ �ȵȴ�.
#ifdef UPDATE_ADD_LOG_20080117
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) Invalid ItemType[%d]",
	#else
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%d] JewelSerial(%d) Invalid ItemType[%d]",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->GetName(),
			   pTarget->m_Type,
			   pTarget->m_Number,
			   pSource->m_Number,
			   iItemType);	
#else	// UPDATE_ADD_LOG_20080117		
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%u] Invalid ItemType[%d]",
	#else
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%d] Invalid ItemType[%d]",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->GetName(),
			   pTarget->m_Type,
			   pTarget->m_Number,
			   iItemType);	
#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Type[%d] Serial[%u] Invalid ItemType[%d]",
	#else
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Type[%d] Serial[%d] Invalid ItemType[%d]",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->m_Type,
			   pTarget->m_Number,
			   iItemType);	
#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif	// UPDATE_ADD_LOG_20080117
		return FALSE;
	}

	// 3. ������ ������ ���� �ɼ� �ο�
	int iItemOption			= _GetSelectRandomOption( pTarget, iItemType );			// Ÿ�Ժ� �ɼ� 
	if ( iItemOption == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		// �ɼ� ã�� ����. ��ȭ ���� : �̷� ���� ������ �ȵȴ�.
#ifdef UPDATE_ADD_LOG_20080117
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) ItemType[%d]",
	#else
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Name[%s] Type[%d] Serial[%d] JewelSerial(%d) ItemType[%d]",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				lpObj->AccountID, lpObj->Name, 
				pTarget->GetName(),
				pTarget->m_Type,
				pTarget->m_Number,
				pSource->m_Number,
				iItemType);	
#else	// UPDATE_ADD_LOG_20080117
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Name[%s] Type[%d] Serial[%u] ItemType[%d]",
	#else
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Name[%s] Type[%d] Serial[%d] ItemType[%d]",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->GetName(),
			   pTarget->m_Type,
			   pTarget->m_Number,
				iItemType);	
#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Type[%d] Serial[%u] ItemType[%d]",
	#else
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Type[%d] Serial[%d] ItemType[%d]",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->m_Type,
			   pTarget->m_Number,
				iItemType);	
#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif	// UPDATE_ADD_LOG_20080117
		return FALSE;
	}
	int iItemOptionLevel	= m_itemOption[iItemType][iItemOption].iRequireLevel;	// �ɼ� �� ���� : �ּ� ����.

	// 4. ���� Ȯ�� ���� �� ����
	int iSuccessRate = rand()%100;
	if ( iSuccessRate >= m_iRateStrengthenSuccess )
	{
		// ��ȭ ����
#ifdef UPDATE_ADD_LOG_20080117
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) Rate (%d/%d)",
	#else
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%d] JewelSerial(%d) Rate (%d/%d)",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   lpObj->AccountID, lpObj->Name,
			   pTarget->GetName(),
			   pTarget->m_Type,
			   pTarget->m_Number,
			   pSource->m_Number,
			   iSuccessRate,
			   m_iRateStrengthenSuccess);
#else	// UPDATE_ADD_LOG_20080117
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%u]  Rate (%d/%d)",
	#else
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%d]  Rate (%d/%d)",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   lpObj->AccountID, lpObj->Name,
			   pTarget->GetName(),
			   pTarget->m_Type,
			   pTarget->m_Number,
			   iSuccessRate,
			   m_iRateStrengthenSuccess);
#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Type[%d] Serial[%u]  Rate (%d/%d)",
	#else
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Type[%d] Serial[%d]  Rate (%d/%d)",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   lpObj->AccountID, lpObj->Name,
			   pTarget->m_Type,
			   pTarget->m_Number,
			   iSuccessRate,
			   m_iRateStrengthenSuccess);
#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif	// UPDATE_ADD_LOG_20080117

#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3373), lpObj->m_Index, 1); // ��ȭ�Ǻ��� ��ȭ ����
#endif
		return TRUE;
	}


	// 5. ���� ��ȭ ó��
	_MakeOption( pTarget, iItemOption, iItemOptionLevel );

	// ��ȭ ����
	// 6. ��ȭ�� ���� ��ȭ �α�
#ifdef EXTEND_LOG_SYSTEM_07_20070320
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) Rate (%d/%d) Option %d OptionLevel %d",
	#else
	LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Name[%s] Type[%d] Serial[%d] JewelSerial(%d) Rate (%d/%d) Option %d OptionLevel %d",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		   lpObj->AccountID, lpObj->Name, 
		   pTarget->GetName(),
		   pTarget->m_Type,
		   pTarget->m_Number,
		   pSource->m_Number,
		   iSuccessRate,
		   m_iRateStrengthenSuccess,
		   iItemOption,iItemOptionLevel);
	#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Type[%d] Serial[%u] JewelSerial(%u) Rate (%d/%d) Option %d OptionLevel %d Rate (%d/%d)",
	#else
	LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Type[%d] Serial[%d] JewelSerial(%d) Rate (%d/%d) Option %d OptionLevel %d Rate (%d/%d)",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		   lpObj->AccountID, lpObj->Name, 
		   pTarget->m_Type,
		   pTarget->m_Number,
		   pSource->m_Number,
		   iSuccessRate,
		   m_iRateStrengthenSuccess,
		   iItemOption,iItemOptionLevel,
		   iSuccessRate,
		   m_iRateStrengthenSuccess);
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#else
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Name[%s] Type[%d] Serial[%u] Rate (%d/%d) Option %d OptionLevel %d",
	#else
	LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Name[%s] Type[%d] Serial[%d] Rate (%d/%d) Option %d OptionLevel %d",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		   lpObj->AccountID, lpObj->Name, 
		   pTarget->GetName(),
		   pTarget->m_Type,
		   pTarget->m_Number,
		   iSuccessRate,
		   m_iRateStrengthenSuccess,
		   iItemOption,iItemOptionLevel);
	#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Type[%d] Serial[%u] Rate (%d/%d) Option %d OptionLevel %d Rate (%d/%d)",
	#else
	LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Type[%d] Serial[%d] Rate (%d/%d) Option %d OptionLevel %d Rate (%d/%d)",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		   lpObj->AccountID, lpObj->Name, 
		   pTarget->m_Type,
		   pTarget->m_Number,
		   iSuccessRate,
		   m_iRateStrengthenSuccess,
		   iItemOption,iItemOptionLevel,
		   iSuccessRate,
		   m_iRateStrengthenSuccess);
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // EXTEND_LOG_SYSTEM_07_20070320

#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
	GCServerMsgStringSend(lMsg.Get(3374), lpObj->m_Index, 1); // ��ȭ�Ǻ��� ��ȭ ����
#endif

	// �׽�Ʈ
#if TESTSERVER == 1
	ShowStrengthenOption( pTarget );
#endif
	
	// ����Ʈ ������Ʈ
	gObjMakePreviewCharSet(lpObj->m_Index);

	// ������ �缳�� : ��ȭ ����
	float levelitemdur =  (float)ItemGetDurability(lpObj->pInventory[target].m_Type, 
												   lpObj->pInventory[target].m_Level, 
												   lpObj->pInventory[target].IsExtItem(),
												   lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur*lpObj->pInventory[target].m_Durability/lpObj->pInventory[target].m_BaseDurability;

	// 6. ������ ������ �缳���Ѵ�	: ��ȭ ����.
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type, 
										  lpObj->pInventory[target].m_Option1, 
										  lpObj->pInventory[target].m_Option2,
										  lpObj->pInventory[target].m_Option3,
										  lpObj->pInventory[target].m_NewOption,
										  lpObj->pInventory[target].m_SetOption,
										  lpObj->pInventory[target].m_ItemOptionEx);
	#else
		lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type, 
										  lpObj->pInventory[target].m_Option1, 
										  lpObj->pInventory[target].m_Option2,
										  lpObj->pInventory[target].m_Option3,
										  lpObj->pInventory[target].m_NewOption,
										  lpObj->pInventory[target].m_SetOption);
	#endif // ADD_380ITEM_NEWOPTION_20060711

	return TRUE;	
}

BOOL	CJewelOfHarmonySystem::_MakeOption(CItem *pItem, BYTE btOptionType, BYTE btOptionLevel)
{
	// ��ȭ �ɼ� ó��
	pItem->m_JewelOfHarmonyOption = 0;
	pItem->m_JewelOfHarmonyOption |= btOptionType<<JEWELOFHARMONY_BITSHIFT_OPTION;		// 4 Bit : �ɼ� 
	pItem->m_JewelOfHarmonyOption |= btOptionLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL;	// 4 Bit : �ɼ� ���� 

	// �׽�Ʈ
#if TESTSERVER == 1
	ShowStrengthenOption( pItem );
#endif

	return TRUE;
}

void	CJewelOfHarmonySystem::StrengthenItemByMacro(LPOBJECTSTRUCT lpObj, BYTE invenrotyTargetPos, BYTE btOptionType, BYTE btOptionLevel)
{
	// �系 �׽�Ʈ ������ ��쿡�� �۵�
#if TESTSERVER == 0
	return;
#endif

	CItem *pItem = &lpObj->pInventory[invenrotyTargetPos];
	// 1. ��ȭ üũ
	if ( IsStrengthenByJewelOfHarmony( pItem ) == TRUE )
	{
		//return;
	}

	// 2. �ɼ�üũ
	int iType = _GetItemType( pItem );
	if ( iType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		char szMsg[256];
		sprintf(szMsg, "��ȭ�� �� �ִ� �������� �ƴմϴ�.");
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}

	// 3. üũ
	// ���� �׽�Ʈ
	if ( btOptionType == 0xFF )
	{
		btOptionType = _GetSelectRandomOption( pItem, iType );
	}
	if ( btOptionType > MAX_JEWELOFHARMONY_EFFECT_OPTION )
	{
		char szMsg[256];
		sprintf(szMsg, "�ɼ� �ִ� ���� %d �Դϴ�.", MAX_JEWELOFHARMONY_EFFECT_OPTION);
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}
    if ( btOptionLevel > MAX_JEWELOFHARMONY_EFFECT_VALUE - 1 )
	{
		char szMsg[256];
		sprintf(szMsg, "�ɼ� ������ ������ 0~%d �Դϴ�.", MAX_JEWELOFHARMONY_EFFECT_VALUE - 1);
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}


	// 3.5 ����� �ּ� ���������� ��� �ִ�ġ�� ���� �ʰ� �ϱ� ����.
	if ( iType == JEWELOFHARMONY_ITEM_TYPE_WEAPON 
		&& btOptionType == AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE )
	{
		// ���� ������
		BYTE iValue = m_itemOption[JEWELOFHARMONY_ITEM_TYPE_WEAPON][AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE].iItemEffectValue[btOptionLevel];
		{
			if ( pItem->m_DamageMin + iValue > pItem->m_DamageMax - 1)
			{
				//�����޽���
				GCServerMsgStringSend("���� ���� �Ұ� (�ּҰ��ݷ��� �ִ���ݷº��� Ŀ��)", lpObj->m_Index, 1);
				return;
			}
		}
	}

	// 4. �ɼ��� ��ȿ���� üũ
	if ( btOptionType != 0 )
	{
		// �ɼ� Ÿ�� ���� 0 �ΰ�� ȯ���� �����. ���� ó��.
		if ( m_itemOption[iType][btOptionType].bValid == FALSE )
		{
			char szMsg[256];
			sprintf(szMsg, "���� �ɼǹ�ȣ�Դϴ�.");
			GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
			return;
		}
	}
	else
	{
		// ȯ�� ó��
		_MakeOption( pItem, 0,0);
		LogAddTD("[JewelOfHarmony][Restore Item] Macro Restore Success [%s][%s] Pos:%d",
		   lpObj->AccountID, lpObj->Name, 
		   invenrotyTargetPos );	

		char szMsg[256];
		sprintf(szMsg, "ȯ��ó���Ǿ����ϴ�.");
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		GCInventoryItemOneSend(lpObj->m_Index, invenrotyTargetPos);
		return;
	}

	// 5. �ɼ� ���� ����
	if ( pItem->m_Level < m_itemOption[iType][btOptionType].iRequireLevel )
	{
		char szMsg[256];
		sprintf(szMsg, "[%s] ��ȭ �ɼ� �䱸������ ������ ���� %d �Դϴ�.",m_itemOption[iType][btOptionType].szOptionName,
			m_itemOption[iType][btOptionType].iRequireLevel );
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}

//	if ( btOptionLevel > pItem->m_Level )
//		btOptionLevel = pItem->m_Level;

	if ( btOptionLevel < m_itemOption[iType][btOptionType].iRequireLevel )
	{
		char szMsg[256];
		sprintf(szMsg, "[%s] ��ȭ �ɼ��� �ּ� �ɼǷ����� %d �Դϴ�.", m_itemOption[iType][btOptionType].szOptionName,
			m_itemOption[iType][btOptionType].iRequireLevel );
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}

	// 6. ��ȭ ó��
	_MakeOption( pItem, btOptionType, btOptionLevel );

	LogAddTD("[JewelOfHarmony][Strengthen Item] Macro Strengthen Success [%s][%s]  Option %d OptionLevel %d Pos:%d",
		   lpObj->AccountID, lpObj->Name, 
		   btOptionType,btOptionLevel, invenrotyTargetPos );	

	char szMsg[256];
	sprintf(szMsg, "[%s] �ɼǷ��� %d  �� %d ���� ��ȭ", m_itemOption[iType][btOptionType].szOptionName,
		btOptionLevel, m_itemOption[iType][btOptionType].iItemEffectValue[btOptionLevel]);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);

	GCInventoryItemOneSend(lpObj->m_Index, invenrotyTargetPos);
}


//--------------------------------------------------------------------
BYTE	CJewelOfHarmonySystem::ShowStrengthenOption(CItem *pItem)
{
	BYTE bResult = -1;

	// ��ȭ ���� ����
	if ( IsStrengthenByJewelOfHarmony( pItem ) == FALSE )
		return bResult;

	int type = _GetItemType( pItem );	// ������ �з�
	bResult  = GetItemStrengthenOption( pItem );	// 1��° ���� 4��°���� 4bit : �ɼ� ����
	int iItemOptionLevel = _GetItemOptionLevel( pItem );	// �ɼ� ����


	char buf[256];
	sprintf (buf, "ȿ�� : %20s    type:%d, option:%d optionLevel %d value %d \n", m_itemOption[type][bResult].szOptionName, type, bResult,iItemOptionLevel
		, m_itemOption[type][bResult].iItemEffectValue[iItemOptionLevel]);
	OutputDebugString(buf);

	return bResult;
}

//--------------------------------------------------------------------
BYTE	CJewelOfHarmonySystem::MakeCharSetData( LPOBJECTSTRUCT lpObj )
{
	// ���� ������� ���� (2006.8.8)
	
	BYTE btResult = 0;

	// 2 bit : ������ ��ȭ ����. ����Ʈ 0 ~ 3�ܰ�. Ȱ��ȭ �Ǿ� �־����.
	if ( IsActive ( &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT] ) == TRUE )
	{
		int iOptionLevel = _GetItemOptionLevel( &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT] );
		if ( iOptionLevel > 5 && iOptionLevel < 9 )
			btResult |= 0x40;		// 6~8 ����  : 0100 0000
		else if ( iOptionLevel > 8 && iOptionLevel < 13 )
			btResult |= 0x80;		// 9~12 ���� : 1000 0000
		else if ( iOptionLevel > 12 )
			btResult |= 0xC0;		// 13 ����   : 1100 0000
	}
	// 2 bit : �޼� ��ȭ ����. ����Ʈ 0 ~ 3�ܰ�. Ȱ��ȭ �Ǿ� �־����.
	if ( IsActive( &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT] ) == TRUE )
	{
		int iOptionLevel = _GetItemOptionLevel( &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT] );
		if ( iOptionLevel > 5 && iOptionLevel < 9 )
			btResult |= 0x10;		// 6~8 ����  : 0001 0000
		else if ( iOptionLevel > 8 && iOptionLevel < 13 )
			btResult |= 0x20;		// 9~12 ���� : 0010 0000
		else if ( iOptionLevel > 12 )
			btResult |= 0x30;		// 13 ����   : 0011 0000
	}

	return btResult;
}

//--------------------------------------------------------------------
// ������ �ɼ� ����
// pItem : ��� ������
// pItemEffect : ������ ȿ���� ����� �� 
// Return Value 
//    - TRUE  : ���� ����
//    - FALSE : ���� ����
//    - pItemEffect : ����� ������ ȿ�� ��
//--------------------------------------------------------------------
void	CJewelOfHarmonySystem::SetApplyStrengthenItem( LPOBJECTSTRUCT lpObj )
{
	// 1. �� �ʱ�ȭ
	LPJEWELOFHARMONY_ITEM_EFFECT pItemEffect = &lpObj->m_JewelOfHarmonyEffect;
	InitEffectValue( pItemEffect );

	int iItemIndex = 0;
	for( iItemIndex = 0; iItemIndex < MAX_EQUIPMENT; iItemIndex++ )
	{
		if( lpObj->pInventory[iItemIndex].IsItem() && lpObj->pInventory[iItemIndex].m_IsValidItem )
		{
			BOOL bResult = _CalcItemEffectValue( &lpObj->pInventory[iItemIndex], pItemEffect );

			if ( bResult == TRUE ) 
			{
#if TESTSERVER == 1
				// �α�
				OutputDebugString("���� ��ȭ ������ ����----\n");
				ShowStrengthenOption( &lpObj->pInventory[iItemIndex] );
				OutputDebugString("���� ��ȭ ������ ���볡----\n");
#endif
			}
		}
	}

	// 2. 1�� ��ġ��꿡 ����Ǵ� �� ����
 // AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE		 = 1,	// �ּ� ���ݷ� ��� (��)
 // AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE		 = 2,	// �ִ� ���ݷ� ��� (��)
 // AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKDAMAGE_BOTH	 = 5,	// ���ݷ� ��� (�ּ�,�ִ�) (��)
 // AT_JEWELOFHARMONY_STAFF_IMPROVE_MAGICDAMAGE			 = 1,	// ���� ���(��)
 // AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DEFENSE			 	 = 1,   // ���� ��� (��)
 // AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_AG				 = 2,	// �ִ� AG ��� (��)
 // AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_HP				 = 3,	// �ִ� HP ��� (��)
 // AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DAMAGE_DECREASE		 = 7,	// ������ ���� % ����

	// 1 : �ּ� ���ݷ� ���, 5 : ���ݷ� ���
	lpObj->m_AttackDamageMinLeft	+= pItemEffect->HJOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinLeft	+= pItemEffect->HJOpAddAttackDamage;
	lpObj->m_AttackDamageMinRight	+= pItemEffect->HJOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinRight	+= pItemEffect->HJOpAddAttackDamage;

	// 2 : �ִ� ���ݷ� ���, 5 : ���ݷ� ���
	lpObj->m_AttackDamageMaxLeft 	+= pItemEffect->HJOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxLeft	+= pItemEffect->HJOpAddAttackDamage;
	lpObj->m_AttackDamageMaxRight	+= pItemEffect->HJOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxRight	+= pItemEffect->HJOpAddAttackDamage;

	// �α� �׽�Ʈ : �ּ� ���ݷ��� �ִ� ���ܷº��� Ŀ���� ���
#if TESTSERVER == 1
	if ( lpObj->m_AttackDamageMinLeft >= lpObj->m_AttackDamageMaxLeft)
	{
		LogAddTD("[JewelOfHarmony][AttackDamageError] [%s][%s] LeftMin:%d LeftMax:%d AddMinAttackDamage:%d AddAttackDamage:%d", 
			lpObj->AccountID, lpObj->Name, 
			lpObj->m_AttackDamageMinLeft,
			lpObj->m_AttackDamageMaxLeft,
			pItemEffect->HJOpAddMinAttackDamage,
			pItemEffect->HJOpAddAttackDamage
			);
	}
	if ( lpObj->m_AttackDamageMinRight >= lpObj->m_AttackDamageMaxRight)
	{
		LogAddTD("[JewelOfHarmony][AttackDamageError] [%s][%s] RightMin:%d RightMax:%d AddMinAttackDamage:%d AddAttackDamage:%d", 
			lpObj->AccountID, lpObj->Name, 
			lpObj->m_AttackDamageMinRight,
			lpObj->m_AttackDamageMaxRight,
			pItemEffect->HJOpAddMinAttackDamage,
			pItemEffect->HJOpAddAttackDamage
			);
	}
#endif

	// 1 : ���� ���
	lpObj->m_MagicDamageMin += pItemEffect->HJOpAddMagicPower;
	lpObj->m_MagicDamageMax += pItemEffect->HJOpAddMagicPower;

	// 1 : ���� ��� (��) - ��Ʈ������ó�� 50%�� �������� ����.
	lpObj->m_Defense	+= ( pItemEffect->HJOpAddDefense*10/ 20);

	// 2 : �ִ� AG ���
	lpObj->AddBP		+= pItemEffect->HJOpAddMaxAG;

	// 3 : �ִ� HP ���
	lpObj->AddLife		+= pItemEffect->HJOpAddMaxHP;

	// 7 : ������ ���� % ����
	lpObj->DamageMinus += pItemEffect->HJOpAddDamageDecrease;
}


int	CJewelOfHarmonySystem::GetItemEffectValue(CItem *pItem, int iOptionType)
{
	// 1. ������ �з�
	int iItemType = _GetItemType(pItem);
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		return 0;
	}

	// 2. ������ ȿ�� Ȯ�� : ���� �ɼ��� ��츸 �˻�
	int iItemOptionType = 0;
	iItemOptionType = GetItemStrengthenOption( pItem );
	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM 
		|| iItemOptionType != iOptionType )
	{
		return 0;
	}

	// 3. �ɼǷ����� ���� ������ ȿ�� ��
	int		iItemOptionLevel = _GetItemOptionLevel( pItem );
	if ( iItemOptionLevel >= MAX_JEWELOFHARMONY_EFFECT_VALUE )
	{
		return 0;
	}

	int		iItemEffectValue = m_itemOption[iItemType][iItemOptionType].iItemEffectValue[iItemOptionLevel];

	return iItemEffectValue;
}


BOOL	CJewelOfHarmonySystem::_CalcItemEffectValue(CItem *pItem, LPJEWELOFHARMONY_ITEM_EFFECT pItemEffect)
{
	// 1. ������ �з�
	int iItemType = _GetItemType(pItem);
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		return FALSE;
	}

	// 2. ������ ȿ�� Ȯ��
	int iItemOptionType = 0;
	iItemOptionType = GetItemStrengthenOption( pItem );
	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		return FALSE;
	}
	// 3. �ɼǷ����� ���� ������ ȿ�� ��
	int		iItemOptionLevel = _GetItemOptionLevel( pItem );
	if ( iItemOptionLevel >= MAX_JEWELOFHARMONY_EFFECT_VALUE )
	{
		return FALSE;
	}

	// 3.5 �������� ������ �ɼ� �������� ���� ��� ��Ȱ�� ó��
	if ( IsActive( pItem ) == FALSE )
	{
		return FALSE;
	}
	
	int		iItemEffectValue = m_itemOption[iItemType][iItemOptionType].iItemEffectValue[iItemOptionLevel];

	// 4. ȿ�� ����
	BOOL bResult = TRUE;
	switch( iItemType )
	{
		// ����� ������
		case JEWELOFHARMONY_ITEM_TYPE_WEAPON:
			{
				switch( iItemOptionType )
				{
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE:
					pItemEffect->HJOpAddMinAttackDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE:
					pItemEffect->HJOpAddMaxAttackDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR:
					// ������ ���뿡 ���� �ɼ� : ���� ó��.
					break;
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX:
					// ������ ���뿡 ���� �ɼ� : ���� ó��.
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKDAMAGE_BOTH:
					pItemEffect->HJOpAddMinAttackDamage += iItemEffectValue;
					pItemEffect->HJOpAddMaxAttackDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_CRITICALDAMAGE:
					pItemEffect->HJOpAddCriticalDamage  += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_SKILLDAMAGE:
					pItemEffect->HJOpAddSkillAttack 	+= iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKSUCCESRATE_PVP:
					pItemEffect->HJOpAddAttackSuccessRatePVP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_SD_RATE:
					pItemEffect->HJOpDecreaseSDRate			 += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_IGNORE_SD_RATE:
					pItemEffect->HJOpAddIgnoreSDRate		 += iItemEffectValue;
					break;
				default:
					bResult = FALSE;
					break;
				}
			}
			break;
		// �����̷�
		case JEWELOFHARMONY_ITEM_TYPE_STAFF:
			{
				switch( iItemOptionType )
				{
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_MAGICDAMAGE:
					pItemEffect->HJOpAddMagicPower += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR:
					// ������ ���뿡 ���� �ɼ� : ���� ó��.
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX:
					// ������ ���뿡 ���� �ɼ� : ���� ó��.
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_SKILLDAMAGE:
					pItemEffect->HJOpAddSkillAttack 	+= iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_CRITICALDAMAGE:
					pItemEffect->HJOpAddCriticalDamage  += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_SD_RATE:
					pItemEffect->HJOpDecreaseSDRate			 += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_ATTACKSUCCESRATE_PVP:
					pItemEffect->HJOpAddAttackSuccessRatePVP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_IGNORE_SD_RATE:
					pItemEffect->HJOpAddIgnoreSDRate		 += iItemEffectValue;
					break;
				default:
					bResult = FALSE;
					break;
				}
			}
			break;
		// ����
		case JEWELOFHARMONY_ITEM_TYPE_DEFENSE:
			{
				switch( iItemOptionType )
				{
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DEFENSE:
					pItemEffect->HJOpAddDefense += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_AG:
					pItemEffect->HJOpAddMaxAG += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_HP:
					pItemEffect->HJOpAddMaxHP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_HP:
					pItemEffect->HJOpAddRefillHP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_MP:
					pItemEffect->HJOpAddRefillMP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_BLOCKSUCCESRATE_PVP:
					pItemEffect->HJOpAddDefenseSuccessRatePvP  += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DAMAGE_DECREASE:
					pItemEffect->HJOpAddDamageDecrease 	+= iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_SD_RATE:
					pItemEffect->HJOpAddSDRate += iItemEffectValue;
					break;
				default:
					bResult = FALSE;
					break;
				}
			}
			break;
		default:
			bResult = FALSE;
			break;

	}

	return bResult;
}


// ȿ���� �ʱ�ȭ : ĳ���� ��ġ �� ���ÿ� �ݵ�� ȣ��
void	CJewelOfHarmonySystem::InitEffectValue( LPJEWELOFHARMONY_ITEM_EFFECT pItemEffect )
{
	pItemEffect->HJOpAddMinAttackDamage			= 0;	// �ּ� ���ݷ� ���� v
	pItemEffect->HJOpAddMaxAttackDamage			= 0;	// �ִ� ���ݷ� ���� v

	pItemEffect->HJOpRequireStr					= 0;	// �ʿ� �� ����.  �����ۿ� ����.
	pItemEffect->HJOpRequireDex					= 0;	// �ʿ� ��ø ����. �����ۿ� ����.
	
	pItemEffect->HJOpAddAttackDamage			= 0;	// ���ݷ� ���� ( �ּ�. �ִ�)
	pItemEffect->HJOpAddCriticalDamage			= 0;	// ũ��Ƽ�� ������ ���� v
	pItemEffect->HJOpAddSkillAttack				= 0;	// ��ų ���ݷ� ���� v

	pItemEffect->HJOpAddAttackSuccessRatePVP	= 0;	// ���� ������ ��� PvP
	pItemEffect->HJOpDecreaseSDRate				= 0;	// ��� �ǵ� ����� ����
	pItemEffect->HJOpAddIgnoreSDRate			= 0;	// ������ Ȯ�� ��ŭ ����� SD�� �����ϰ� ���� HP������.

	pItemEffect->HJOpAddMagicPower				= 0;	// �߰� ���� v

	pItemEffect->HJOpAddDefense					= 0;	// �߰� ���� v
	pItemEffect->HJOpAddMaxAG					= 0;	// �ִ� AG ���
	pItemEffect->HJOpAddMaxHP					= 0;	// �ִ� HP ���
	pItemEffect->HJOpAddRefillHP				= 0;	// ����� �ڵ� ������ ��� : ??? �������� �ִٸ� �߰�?
	pItemEffect->HJOpAddRefillMP				= 0;	// ���� �ڵ� ������ ���
	pItemEffect->HJOpAddDefenseSuccessRatePvP	= 0;	// ��� ������ ��� PvP
	pItemEffect->HJOpAddDamageDecrease			= 0;	// ������ ���ҷ� ���..??
	pItemEffect->HJOpAddSDRate					= 0;	// ���� ����ϴ� SD ���� ���
}
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// ���ü� ���� ���� ������ ���� Ȯ��
BOOL	CJewelOfHarmonySystem::IsEnableToMakeSmeltingStoneItem( CItem *pItem )
{
	// 1�� ���� : ����� / �����̷� / ������ �ش��ϵ��� ���� : ����� �� ����
	if ( _GetItemType( pItem ) == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		return FALSE;
	}

	// 2�� ���� : ��ȭ�� �������� ������ �� ����.
	if ( IsStrengthenByJewelOfHarmony( pItem ) == TRUE )
	{
		return FALSE;
	}

	// 2.5�� ���� : ��Ʈ �������� ������ �� ����.
#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
	if ( pItem->IsSetItem() )
#else
	if ( pItem->IsSetItem() == TRUE )
#endif
	{
		return FALSE;
	}

	// 3�� ���� : �䱸������ �ִ� ���
	map<int, int>::iterator iter = m_mapEnableMixList.find( pItem->m_Type );
	if ( iter != m_mapEnableMixList.end() )
	{
		int reqLevel = iter->second;
		if ( pItem->m_Level < reqLevel )
		{
#if TESTSERVER == 1
			LogAddTD("���ü� ���� �Ұ� �ʿ䷹�� %d �����۷��� %d", reqLevel, pItem->m_Level);
#endif
			return FALSE;
		}

	}
		

	return TRUE;
}


// ���ü� ����
BOOL	CJewelOfHarmonySystem::MakeSmeltingStoneItem(LPOBJECTSTRUCT lpObj)
{
	// �ý��� ��� ���� ����
	if ( m_bSystemMixSmeltingStone != TRUE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3381), lpObj->m_Index, 1); // �������սý��� ��� �Ұ�
#endif
		return TRUE;

	}

	// ChaosBox Lock
	lpObj->ChaosLock = TRUE;

	int iValidItemCount = 0;
	int iInvalidItemCount = 0;
	int iMakeSmeltingStoneMixPrice = 0;

#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
	BYTE ExOption[8];
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // MODIFY_LOG_SYSTEM_2_3_20060828

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	// 1. ī��������â ������ �˻�
	int iItemPos = -1;
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			// ���ð����� ������ Ȯ��
			if( IsEnableToMakeSmeltingStoneItem( &lpObj->pChaosBox[n] ) == TRUE )
			{	
				// ��ȿ ������ ����
				++iValidItemCount;
				iItemPos = n;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	// �ݵ�� 1���� �����۸� ���� �ؾ��� : ��ȿ�� ������ 1��, ��ȿ���� ���� ������ 0��, ��������ġ 0 �̻�
	if(  iValidItemCount != 1 || iInvalidItemCount != 0 || iItemPos == -1 )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	// 2. �Ϲ� / ������Ʈ �з��� ���� Ȯ�� ���� : �Ϲ� 10%, ������Ʈ 60%
	// ���� ���� ���� ����
	int		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE;				// ���ü� ���� ���� Ȯ��
	BOOL	bIsItemNormal = TRUE;
	if ( lpObj->pChaosBox[iItemPos].IsExtItem() == TRUE )
	{
		bIsItemNormal = FALSE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE		= m_iRateMixSmeltingStoneExt;		// ���ü� ���� ���� Ȯ��. ������Ʈ.
	}
	else
	{
		bIsItemNormal = TRUE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE		= m_iRateMixSmeltingStoneNor;		// ���ü� ���� ���� Ȯ��. �Ϲ�
	}

	
	// 3. �ʿ� �� ó��
	iMakeSmeltingStoneMixPrice = m_iZenForMixSmeltingStone;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// ī���� �ý��� ��������
	INT iChaosTaxMoney = (INT)(iMakeSmeltingStoneMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100);
	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	iMakeSmeltingStoneMixPrice = iMakeSmeltingStoneMixPrice + iChaosTaxMoney;

	if (iMakeSmeltingStoneMixPrice < 0)
	{
		iMakeSmeltingStoneMixPrice = 0;
	}
#endif

	if( lpObj->Money < iMakeSmeltingStoneMixPrice )
	{
		pMsg.Result = 0x02;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	lpObj->Money = lpObj->Money - iMakeSmeltingStoneMixPrice;

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// ī���� ���� ������ ����
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	g_MixSystem.LogChaosItem( lpObj, "JewelOfHarmony][Smelt Item Mix" );
	LogAddTD("[JewelOfHarmony][Smelt Item Mix] - Mix Start");
#else
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
	ItemIsBufExOption(ExOption, &lpObj->pChaosBox[iItemPos]);
	// ���ÿ� ���� ������ �α� ���
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[JewelOfHarmony][Smelt Item Mix] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d] Serial:[%u] [%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
	#else
	LogAddTD("[JewelOfHarmony][Smelt Item Mix] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d] Serial:[%d] [%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		lpObj->AccountID, lpObj->Name, iItemPos,
		lpObj->pChaosBox[iItemPos].GetName(),
		lpObj->pChaosBox[iItemPos].m_Level,
		lpObj->pChaosBox[iItemPos].m_Option1,
		lpObj->pChaosBox[iItemPos].m_Option2,
		lpObj->pChaosBox[iItemPos].m_Option3,
		lpObj->pChaosBox[iItemPos].m_Number,
		(int)lpObj->pChaosBox[iItemPos].m_Durability,
		ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6],
		lpObj->pChaosBox[iItemPos].m_SetOption,
		lpObj->pChaosBox[iItemPos].m_ItemOptionEx >> 7,
		g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pChaosBox[iItemPos] ),
		g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pChaosBox[iItemPos] )
		);
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // MODIFY_LOG_SYSTEM_2_3_20060828


	// 4. ���� Ȯ���� ���� ó��
	int iRate = rand()%100;
	if ( iRate < JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE )
	{
		// ���ü� ����
		int iItemType;
		if ( bIsItemNormal == TRUE )
			iItemType = JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX;
		else
			iItemType = JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX;

		ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0 );
		gObjInventoryCommit( lpObj->m_Index );

#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
		g_MixSystem.LogChaosItem( lpObj, "[JewelOfHarmony][Smelt Item Mix]");
#endif
		LogAddTD("[JewelOfHarmony][Smelt Item Mix] Smelting Stone Normal[%d] Mix Success [%s][%s], Money(%d-%d) Rate(%d/%d)",
			bIsItemNormal,
			lpObj->AccountID, lpObj->Name, lpObj->Money, iMakeSmeltingStoneMixPrice, iRate, JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE);

#ifdef MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
		return TRUE;
#endif	// MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
	}
	else
	{	// ���ս���	
		g_MixSystem.ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[JewelOfHarmony][Smelt Item Mix] Smelting Stone Normal[%d] Mix Fail [%s][%s], Money : %d-%d Rate(%d/%d)",
			bIsItemNormal,
			lpObj->AccountID, lpObj->Name, 
		    lpObj->Money, iMakeSmeltingStoneMixPrice,
			iRate,
		    JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE);
	}

	lpObj->ChaosLock = FALSE;

	return TRUE;
}

// ���ü��� ���� ��ȭ
BOOL	CJewelOfHarmonySystem::SmeltItemBySmeltingStone(LPOBJECTSTRUCT lpObj, int source, int target)
{
	// 0. �⺻ ���� üũ
	if ( m_bSystemSmeltingItem == FALSE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3379), lpObj->m_Index, 1); // ���ü� ��ȭ �ý��� ��� �Ұ�
#endif
		return FALSE;
	}
	if( source < 0 || source > MAX_INVENTORY-1 ) 
		return FALSE;
	if( target < 0 || target > MAX_INVENTORY-1 ) 
		return FALSE;
	
	// �������� �ִ��� üũ
	if( lpObj->pInventory[source].IsItem() == FALSE ) 
		return FALSE;
	if( lpObj->pInventory[target].IsItem() == FALSE ) 
		return FALSE;

	CItem *pSource = &lpObj->pInventory[source];
	CItem *pTarget = &lpObj->pInventory[target];

	// 1. Ÿ�� �������� ��ȭ �� ������ üũ
	if ( IsStrengthenByJewelOfHarmony(pTarget) == FALSE )
	{
		LogAddTD("[JewelOfHarmony][Smelt Item] Not Strengthen Item [%s][%s]",
			   lpObj->AccountID, 
			   lpObj->Name);
		return FALSE;
	}

	// 2. ��ȭ ���� üũ
	int iItemOptionLevel = _GetItemOptionLevel( pTarget ); // ���� ��ȭ �ɼ� ����
	if ( iItemOptionLevel >= pTarget->m_Level )	
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		if ( iItemOptionLevel == 13 ) // �ְ� �ɼ� ����
		{
			GCServerMsgStringSend(lMsg.Get(3369), lpObj->m_Index, 1); // ��ȭ ������ �ִ� ��ġ�� ���� ���Դϴ�. �� �̻� ��ȭ�� �� �����ϴ�.
		}
		else
		{
			GCServerMsgStringSend(lMsg.Get(3368), lpObj->m_Index, 1); // ������ ������ ���� �� �̻� ��ȭ�� �� �����ϴ�.
		}
#endif 
		LogAddTD("[JewelOfHarmony][Smelt Item] Already Have Max OptionLevel [%s][%s] OptionLevel [%d] ItemLevel [%d]",
			   lpObj->AccountID, 
			   lpObj->Name, 
			   iItemOptionLevel,
			   pTarget->m_Level);	
		return FALSE;
	}

	// 2.5 ����� �ּ� ���ݷ� ����� ��� �ִ� ���ݷ�-1 ���� ���� �� ����.
	if ( _GetItemType( pTarget ) == JEWELOFHARMONY_ITEM_TYPE_WEAPON 
		&& GetItemStrengthenOption( pTarget ) == AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE )
	{
		// ���� ������
		int iNextLevel = _GetItemOptionLevel( pTarget ) +1;
		BYTE iValue = m_itemOption[JEWELOFHARMONY_ITEM_TYPE_WEAPON][AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE].iItemEffectValue[iNextLevel];
		{
			if ( pTarget->m_DamageMin + iValue > pTarget->m_DamageMax - 1)
			{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
				GCServerMsgStringSend(lMsg.Get(3369), lpObj->m_Index, 1); // ��ȭ ������ �ִ� ��ġ�� ���� ���Դϴ�. ���̻� ��ȭ�� �� �����ϴ�.
#endif
				return FALSE;
			}
		}
	}

	// 3. ���� Ȯ�� : ���/�ϱ� ���ü��� ���� Ȯ�� ����
	int iSuccessRate = rand()%100;
	int iRateSmeltingSuccess = 0;
	BOOL bIsNormalSmeltingStone = _IsJewelOfHarmonySmeltingItemNor( pSource->m_Type );
	if ( bIsNormalSmeltingStone == TRUE )
		iRateSmeltingSuccess = m_iRateSmeltingSuccessNor;
	else
		iRateSmeltingSuccess = m_iRateSmeltingSuccessExt;

	if ( iSuccessRate >= iRateSmeltingSuccess )
	{
		// ���� ��ȭ �ɼ� ���� ����
		int iItemOptionNewLevel = _GetItemOptionRequireLevel( pTarget );	// �ɼ� ���� ���� : �ּ� ����
		pTarget->m_JewelOfHarmonyOption  = pTarget->m_JewelOfHarmonyOption&JEWELOFHARMONY_BITMASK_OPTION;		// �ɼ� 4Bit �� 0 ����
		pTarget->m_JewelOfHarmonyOption |= (iItemOptionNewLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL);			// 4 Bit : �ɼ� ���� 

		// ��ȭ���׷��̵� ����
		ShowStrengthenOption( pTarget );

#ifdef UPDATE_ADD_LOG_20080117
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Fail by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%u] JewelSerial(%u) Rate(%d/%d) Level(%d->%d)",
	#else
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Fail by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%d] JewelSerial(%d) Rate(%d/%d) Level(%d->%d)",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   bIsNormalSmeltingStone,
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->GetName(),
			   pTarget->m_Type,
			   pTarget->m_Number,
			   pSource->m_Number,
			   iSuccessRate,
			   iRateSmeltingSuccess,
			   iItemOptionLevel,
			   iItemOptionNewLevel);
#else	// UPDATE_ADD_LOG_20080117
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Fail by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%u] Rate(%d/%d) Level(%d->%d)",
	#else
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Fail by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%d] Rate(%d/%d) Level(%d->%d)",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   bIsNormalSmeltingStone,
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->GetName(),
			   pTarget->m_Type,
			   pTarget->m_Number,
			   iSuccessRate,
			   iRateSmeltingSuccess,
			   iItemOptionLevel,
			   iItemOptionNewLevel);
#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Fail by Normal[%d] [%s][%s] Type [%d] Serial [%u] Rate(%d/%d) Level(%d->%d)",
	#else
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Fail by Normal[%d] [%s][%s] Type [%d] Serial [%d] Rate(%d/%d) Level(%d->%d)",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
			   bIsNormalSmeltingStone,
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->m_Type,
			   pTarget->m_Number,
			   iSuccessRate,
			   iRateSmeltingSuccess,
			   iItemOptionLevel,
			   iItemOptionNewLevel);	
#endif // EXTEND_LOG_SYSTEM_03_20060816	
#endif	// UPDATE_ADD_LOG_20080117

#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3375), lpObj->m_Index, 1); // ���ü� ��ȭ ����
#endif
	}
	else
	{
		// ���� ��ȭ �ɼ� ���� ����
		int iItemOptionNewLevel = iItemOptionLevel + 1;
		pTarget->m_JewelOfHarmonyOption  = pTarget->m_JewelOfHarmonyOption&JEWELOFHARMONY_BITMASK_OPTION;		// �ɼ� 4Bit �� 0 ����
		pTarget->m_JewelOfHarmonyOption |= (iItemOptionNewLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL);					// 4 Bit : �ɼ� ���� 

		// ��ȭ���׷��̵� ����
		ShowStrengthenOption( pTarget );
#ifdef EXTEND_LOG_SYSTEM_07_20070320
		int iItemOptionType = GetItemStrengthenOption( pTarget );
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%u] JewelSerial(%u) Rate(%d/%d) Option(%d) Level(%d->%d)",
		#else
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%d] JewelSerial(%d) Rate(%d/%d) Option(%d) Level(%d->%d)",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				bIsNormalSmeltingStone,
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->GetName(),
			   pTarget->m_Type,
			   pTarget->m_Number,
			   pSource->m_Number,
			   iSuccessRate,
			   iRateSmeltingSuccess,
			   iItemOptionType,
			   iItemOptionLevel,
			   (iItemOptionNewLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL));
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Type [%d] Serial [%u] JewelSerial(%u) Rate(%d/%d) Option(%d) Level(%d->%d)",
		#else
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Type [%d] Serial [%d] JewelSerial(%d) Rate(%d/%d) Option(%d) Level(%d->%d)",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				bIsNormalSmeltingStone,
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->m_Type,
			   pTarget->m_Number,
			   pSource->m_Number,
			   iSuccessRate,
			   iRateSmeltingSuccess,
			   iItemOptionType,
			   iItemOptionLevel,
			   (iItemOptionNewLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL));	
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#else
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%u] Rate(%d/%d) Level(%d->%d)",
		#else
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%d] Rate(%d/%d) Level(%d->%d)",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				bIsNormalSmeltingStone,
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->GetName(),
			   pTarget->m_Type,
			   pTarget->m_Number,
			   iSuccessRate,
			   iRateSmeltingSuccess,
			   iItemOptionLevel,
			   (iItemOptionNewLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL));
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Type [%d] Serial [%u] Rate(%d/%d) Level(%d->%d)",
		#else
		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Type [%d] Serial [%d] Rate(%d/%d) Level(%d->%d)",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				bIsNormalSmeltingStone,
			   lpObj->AccountID, lpObj->Name, 
			   pTarget->m_Type,
			   pTarget->m_Number,
			   iSuccessRate,
			   iRateSmeltingSuccess,
			   iItemOptionLevel,
			   (iItemOptionNewLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL));	
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // EXTEND_LOG_SYSTEM_07_20070320

#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3376), lpObj->m_Index, 1); // ���ü� ��ȭ ����
#endif
	}

	return TRUE;
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
// ȯ�� �ý���
int		CJewelOfHarmonySystem::_GetZenForRestoreItem(CItem *pItem)
{
	// 1. ������ �з�
	int iItemType = _GetItemType(pItem);
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		return -1;
	}

	// 2. ������ ȿ�� Ȯ�� : ���� �ɼ��� ��츸 �˻�
	int iItemOptionType = 0;
	iItemOptionType = GetItemStrengthenOption( pItem );
	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		return -1;
	}

	// 3. �ɼǷ����� ���� ������ ȿ�� ��
	int		iItemOptionLevel = _GetItemOptionLevel( pItem );
	if ( iItemOptionLevel >= MAX_JEWELOFHARMONY_EFFECT_VALUE )
	{
		return -1;
	}

	int		iZenForRestore = m_itemOption[iItemType][iItemOptionType].iZenForRestore[iItemOptionLevel];

	return iZenForRestore;
}

BOOL	CJewelOfHarmonySystem::RestoreStrengthenItem(LPOBJECTSTRUCT lpObj)
{
	// �ý��� ��� ���� ����
	if ( m_bSystemRestoreStrengthen != TRUE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3382), lpObj->m_Index, 1); // ȯ���ý��� ���Ұ�
#endif
		return TRUE;

	}
	
	// ChaosBox Lock
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	// 1. ī��������â ������ �˻� : ��ȭ�� �������� ã�´�.
	// ���ǵ� �־���� : �̿ܿ� ������ ���絵 �־����. �׽�Ʈ�� ����
	// ���� �ʴ´�.
	
	int iStrengtenItemCount = 0;
	int iInvalidItemCount = 0;
	CItem *pItem = NULL;
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( IsStrengthenByJewelOfHarmony( &lpObj->pChaosBox[n] ) == TRUE )
			{
				++iStrengtenItemCount;
				pItem = &lpObj->pChaosBox[n];
			}
		}
	}
	if( ( iStrengtenItemCount != 1 ) )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	// 2. �� üũ
	int iItemOption		 = GetItemStrengthenOption( pItem );
	int iItemOptionLevel = _GetItemOptionLevel( pItem );

	int JEWEL_OF_HARMONY_RETORE_NEEDZEN;
	JEWEL_OF_HARMONY_RETORE_NEEDZEN = _GetZenForRestoreItem( pItem );	// ȯ���� �ʿ��� ��.
	if ( JEWEL_OF_HARMONY_RETORE_NEEDZEN < 0 )
	{
		// ȯ���� �ʿ��� �� ���� �̻�.
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// ī���� �ý��� ��������
	INT iChaosTaxMoney = (INT)(JEWEL_OF_HARMONY_RETORE_NEEDZEN * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100);
	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}
	JEWEL_OF_HARMONY_RETORE_NEEDZEN = JEWEL_OF_HARMONY_RETORE_NEEDZEN + iChaosTaxMoney;
	if (JEWEL_OF_HARMONY_RETORE_NEEDZEN < 0)
	{
		JEWEL_OF_HARMONY_RETORE_NEEDZEN = 0;
	}
#endif
	if( lpObj->Money < JEWEL_OF_HARMONY_RETORE_NEEDZEN )
	{
		pMsg.Result = 0x02;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
	lpObj->Money = lpObj->Money - JEWEL_OF_HARMONY_RETORE_NEEDZEN;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// ī���� ���� ������ ����
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	g_MixSystem.LogChaosItem( lpObj, "JewelOfHarmony][Restore Item" );
	LogAddTD("[JewelOfHarmony][Restore Item] - Restore Start");
#endif

	// 3. ȯ�� ó��
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[JewelOfHarmony][Restore Item] Restore Strengthened Item [%s][%s] Name[%s] ItemType[%d] Serial[%u] OptionType[%d] OptionLevel [%d] Money %d-%d",
	#else
	LogAddTD("[JewelOfHarmony][Restore Item] Restore Strengthened Item [%s][%s] Name[%s] ItemType[%d] Serial[%d] OptionType[%d] OptionLevel [%d] Money %d-%d",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		   lpObj->AccountID, 
		   lpObj->Name, 
		   pItem->GetName(),
		   pItem->m_Type,
		   pItem->m_Number,
		   iItemOption,
		   iItemOptionLevel,
		   lpObj->Money, JEWEL_OF_HARMONY_RETORE_NEEDZEN);
#else
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
	LogAddTD("[JewelOfHarmony][Restore Item] Restore Strengthened Item [%s][%s] ItemType[%d] Serial[%u] OptionType[%d] OptionLevel [%d] Money %d-%d",
	#else
	LogAddTD("[JewelOfHarmony][Restore Item] Restore Strengthened Item [%s][%s] ItemType[%d] Serial[%d] OptionType[%d] OptionLevel [%d] Money %d-%d",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
		   lpObj->AccountID, 
		   lpObj->Name, 
		   pItem->m_Type,
		   pItem->m_Number,
		   iItemOption,
		   iItemOptionLevel,
		   lpObj->Money, JEWEL_OF_HARMONY_RETORE_NEEDZEN);	
#endif // EXTEND_LOG_SYSTEM_03_20060816
	// ȯ��
	pItem->m_JewelOfHarmonyOption = 0;

	// ī�����ڽ� ���� ����
	GCUserChaosBoxSend(lpObj);

	ShowStrengthenOption( pItem );

	lpObj->ChaosLock = FALSE;
	
	return TRUE;
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
// NPC ó��
extern BOOL bCanChaosBox;

BOOL CJewelOfHarmonySystem::NpcJewelOfHarmony(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// â�� ����ϰ� ��������.. ����Ҽ� ����
		return TRUE;
	}
#endif

	PMSG_TALKRESULT			pResult;
	pResult.h.c				= PMHCE_BYTE;
	pResult.h.headcode		= 0x30;
	pResult.h.size			= sizeof( pResult );

	if ( lpNpc->Class == JEWELOFHARMONY_NPC_PURITY )
	{
		pResult.result			= 0x11;						// ���� NPC
		pResult.level1			= m_iRatePuritySuccess;		// ���� ���� Ȯ��

		// �ý��� ��� ���� ����
		if ( m_bSystemPrutiyJewel != TRUE )
		{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
			GCServerMsgStringSend(lMsg.Get(3378), lpObj->m_Index, 1);		// �����ý��� �̿� �Ұ�
#endif
			return TRUE;

		}
		// ���� �������� ���� �ľ�
		if ( IsEnableToUsePuritySystem() == FALSE )
		{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
			GCServerMsgStringSend(lMsg.Get(3378), lpObj->m_Index, 1);		// �����ý��� �̿� �Ұ�
#endif
			LogAddTD("[JewelOfHarmony][%s][%s] Not Purtiy Time ", lpObj->AccountID, lpObj->Name);
			return TRUE;
		}
	}
	else if ( lpNpc->Class == JEWELOFHARMONY_NPC_SMELT )
	{
		pResult.result			= 0x12;		// ���� NPC
		pResult.level1			= m_iRateMixSmeltingStoneNor;	// �Ϲݾ����� ���� ���� Ȯ��
		pResult.level2			= m_iRateMixSmeltingStoneExt;	// ������Ʈ������ ���� ���� Ȯ��

		// �ý��� ��� ���� ����
		if ( m_bSystemMixSmeltingStone != TRUE )
		{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
			ChatTargetSend(lpNpc, lMsg.Get(3381), lpObj->m_Index);   // �������սý��� ��� �Ұ�
#endif
			return TRUE;

		}
	}
	else if ( lpNpc->Class == JEWELOFHARMONY_NPC_RESTORE )
	{
		pResult.result			= 0x13;		// ȯ�� NPC
		// �ý��� ��� ���� ����
		if ( m_bSystemRestoreStrengthen != TRUE )
		{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
			ChatTargetSend(lpNpc, lMsg.Get(3382), lpObj->m_Index);   // ������ ȯ�� �ý��� ��� �Ұ�
#endif
			return TRUE;

		}
	}
	else
	{
		LogAddTD("[JewelOfHarmony][%s][%s] Open Chaos Box Failed NpcType [%d]", lpObj->AccountID, lpObj->Name, lpNpc->Class);
		return TRUE;
	}

	if( bCanChaosBox == TRUE )
	{
#ifdef PERSONAL_SHOP_20040113		// �ڽ��̳� ������ ���λ����� �������̸� ī���� ������ �� �� ����.
			if (lpObj->m_bPShopOpen == true) 
			{
				LogAdd("[JewelOfHarmony][%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
				GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "���λ��� ���� �߿����� ī���� ������ ����Ͻ� �� �����ϴ�."
				return TRUE;
			}
#endif
			
		lpObj->m_IfState.type   = I_CHAOSBOX;
		lpObj->m_IfState.state	= 0;
		lpObj->m_IfState.use    = 1;

#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
		lpObj->bIsChaosMixCompleted = false;			// ī���� ���� ���� '���� ������ �Ϸ���� �ʾҴ�' �� �ʱⰪ�� ����
#endif

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		gObjInventoryTrans(lpObj->m_Index);
		LogAddTD("[JewelOfHarmony][%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219		// ���� ����� ���� (ī��������) �� �����ش�.
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
			GCAnsCsMapSvrTaxInfo (lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
#endif
#endif

	}
		
	return TRUE;	
}


// �ŷ� �� ��ȭ�������� �����ϰ� ������ �ŷ� �Ұ�
BOOL	CJewelOfHarmonySystem::IsEnableToTrade(LPOBJECTSTRUCT lpObj)
{
	BOOL bRet = TRUE;
	
	for( int n=0; n<MAX_TRADEITEM; n++)
	{
		if( lpObj->Trade[n].IsItem() == TRUE &&
			IsStrengthenByJewelOfHarmony( &lpObj->Trade[n] ) == TRUE )
		{
			bRet = FALSE;
		}
	}

	return bRet;

}
//////////////////////////////////////////////////////////////////////
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530