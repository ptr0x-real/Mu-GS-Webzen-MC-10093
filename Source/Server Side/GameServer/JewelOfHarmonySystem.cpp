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
	// 정제 시스템 초기화 : 사용할 수 있는 상태.
	m_bEnable = TRUE;

	// 값 초기화
	_InitOption();
}

CJewelOfHarmonySystem::~CJewelOfHarmonySystem()
{

}

void CJewelOfHarmonySystem::_InitOption()
{
	// 조화의 보석 원석/정제석, 제련석 아이템 인덱스 설정
	JEWEL_OF_HARMONY_ITEMINDEX			= MAKE_ITEMNUM(14,41); 
	JEWEL_OF_HARMONY_PURITY_ITEMINDEX	= MAKE_ITEMNUM(14,42);
	JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX = MAKE_ITEMNUM(14,43);	// 하급제련석
	JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX = MAKE_ITEMNUM(14,44);	// 하급제련석

	// 옵션 테이블 초기화
	memset(m_itemOption, 0, sizeof(m_itemOption) );

	// 환경 변수 : 시스템 On/Off
	m_bSystemPrutiyJewel	   = GetPrivateProfileInt("GameServerInfo",	"PurityHarmonyJewel",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_bSystemMixSmeltingStone  = GetPrivateProfileInt("GameServerInfo",	"MixSmeltingStoneItem",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_bSystemRestoreStrengthen = GetPrivateProfileInt("GameServerInfo",	"RestoreStrengthenItem",	0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_bSystemStrengthenItem	   = GetPrivateProfileInt("GameServerInfo",	"StrengthenItem",			0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_bSystemSmeltingItem	   = GetPrivateProfileInt("GameServerInfo",	"SmeltItemBySmeltingStone",	0,	gDirPath.GetNewPath("commonserver.cfg"));

	// 환경 변수 : 확률 및 필요 젠관련
	m_iRatePuritySuccess	   = GetPrivateProfileInt("GameServerInfo",	"PuritySuccessRate",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iZenForPurity			   = GetPrivateProfileInt("GameServerInfo",	"PurityNeedZen",					0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateMixSmeltingStoneNor = GetPrivateProfileInt("GameServerInfo",	"SmeltingStoneSuccesRateNormal",	0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateMixSmeltingStoneExt = GetPrivateProfileInt("GameServerInfo",	"SmeltingStoneSuccesRateExcellent",	0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iZenForMixSmeltingStone  = GetPrivateProfileInt("GameServerInfo",	"SmeltingNeedZen",					0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateStrengthenSuccess   = GetPrivateProfileInt("GameServerInfo",	"StrengthenSuccessRate",			0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateSmeltingSuccessNor  = GetPrivateProfileInt("GameServerInfo",	"SmeltingItemSuccessRateNor",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateSmeltingSuccessExt  = GetPrivateProfileInt("GameServerInfo",	"SmeltingItemSuccessRateExt",		0,	gDirPath.GetNewPath("commonserver.cfg"));

	// 테스트
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

	// 기본 값 초기화
	_InitOption();

	// 스크립트 처리
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

				// 값 설정
               	JEWELOFHARMONY_ITEM_OPTION *p = &m_itemOption[Type][Index];
				Token = (*GetToken)(); strcpy(p->szOptionName, TokenString);	// 이름 : 넘어감
				Token = (*GetToken)();
				p->iRandomWeight = (int)TokenNumber;	// 확률 가중치
				Token = (*GetToken)();
				p->iRequireLevel  = (int)TokenNumber;	// 요구 레벨
				for (int i = 0; i < MAX_JEWELOFHARMONY_EFFECT_VALUE; ++i )
				{
					Token = (*GetToken)();
					p->iItemEffectValue[i]  = (int)TokenNumber;		// 옵션 값
					Token = (*GetToken)();
					p->iZenForRestore[i]  = (DWORD)TokenNumber;		// 환원필요젠
				}
				p->bValid = TRUE;
			}
		}
	}

	fclose(SMDFile);

	return TRUE;
}


// 제련석 조합 대상 아이템 데이타 로드
BOOL	CJewelOfHarmonySystem::LoadScriptOfSmelt( LPSTR lpszFileName )
{
	if((SMDFile=fopen(lpszFileName,"r")) == NULL)	
		return FALSE;

	// 목록 초기화
	m_mapEnableMixList.clear();

	// 스크립트 처리
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
				// 레벨 제한 아이템
				Token = (*GetToken)();					// 아이템 이름
				Token = (*GetToken)();					// 필요레벨
				int reqLev = (int)TokenNumber;

				// 목록 추가
				int idx = MAKE_ITEMNUM( Type, index);
				m_mapEnableMixList[idx] = reqLev;
			}
		}
	}

	fclose(SMDFile);

	return TRUE;
}
//--------------------------------------------------------------------
// 조화의 보석 원석 체크
BOOL CJewelOfHarmonySystem::IsJewelOfHarmonyOriginal(short type)
{
	BOOL bRet = FALSE;

	if ( type == JEWEL_OF_HARMONY_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

//--------------------------------------------------------------------
// 조화의 보석 정제석 체크
BOOL CJewelOfHarmonySystem::IsJewelOfHarmonyPurity(short type)
{
	BOOL bRet = FALSE;

	if ( type == JEWEL_OF_HARMONY_PURITY_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

//--------------------------------------------------------------------
// 제련석 체크
BOOL CJewelOfHarmonySystem::IsJewelOfHarmonySmeltingItems(short type)
{
	BOOL bRet = FALSE;

	if ( _IsJewelOfHarmonySmeltingItemNor( type ) == TRUE
		|| _IsJewelOfHarmonySmeltingItemExt( type ) == TRUE )
		bRet = TRUE;

	return bRet;
}


// 하급 제련석 체크
BOOL CJewelOfHarmonySystem::_IsJewelOfHarmonySmeltingItemNor(short type)
{
	BOOL bRet = FALSE;

	if ( type == JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

// 상급 제련석 체크
BOOL CJewelOfHarmonySystem::_IsJewelOfHarmonySmeltingItemExt(short type)
{
	BOOL bRet = FALSE;

	if ( type == JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

//--------------------------------------------------------------------
// 정제 가능 여부 설정 : 외부에서 설정
//--------------------------------------------------------------------
void CJewelOfHarmonySystem::SetEnableToUsePuritySystem(BOOL bEnable)
{
	m_bEnable = bEnable;
	LogAddTD("[JewelOfHarmony][PuritySystem] Enable %d", bEnable);
}

//--------------------------------------------------------------------
// 정제 가능 한지 여부
//--------------------------------------------------------------------
BOOL CJewelOfHarmonySystem::IsEnableToUsePuritySystem()
{
#if TESTSERVER == 1
	// 사내 서버는 언제나 가능. 추후 테스트할 때 수정해야함.
	return TRUE;
#endif
	return m_bEnable;
}


//--------------------------------------------------------------------
// 조화의 보석 정제
//--------------------------------------------------------------------
void CJewelOfHarmonySystem::PurityJewelOfHarmony(LPOBJECTSTRUCT lpObj)
{
	// 0. 시스템 사용 가능 여부
	if ( m_bSystemPrutiyJewel != TRUE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3378), lpObj->m_Index, 1); // 정제시스템 이용 불가
#endif
		return ;
	}

	// 1. 정제 시스템 사용 가능 여부
	if ( IsEnableToUsePuritySystem() == FALSE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3378), lpObj->m_Index, 1); // 정제시스템 이용 불가
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

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == JEWEL_OF_HARMONY_ITEMINDEX )
			{	
				// 조화의보석(원석) 개수
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
	
	// 2. 필요 젠 조사
	iChaosMixPrice = m_iZenForPurity;
#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// 카오스 시스템 세율적용
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

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );
#endif

	GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	g_MixSystem.LogChaosItem( lpObj, "JewelOfHarmony][Purity");
	LogAddTD("[JewelOfHarmony][Purity] - Mix Start");
#endif


	// 3. 정제 확률 결정 및 정제 시도
	int iRate = rand()%100;
	if ( iRate < m_iRatePuritySuccess )	// 확률 적용
	{
		int iItemType = JEWEL_OF_HARMONY_PURITY_ITEMINDEX;	// 조화의보석(정제)

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
		// 정제 실패	
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
// m_JewelOfHarmonyOption  비트 사용 (상위기준점)
// 0~3 Bit :  0 ~ 15 옵션 종류. 0 인경우 강화안된 아이템
// 4~7 Bit :  0 ~ 15  옵션 레벨
//---------------------------------------------------------------------
BYTE CJewelOfHarmonySystem::GetItemStrengthenOption(CItem *pItem)
{
	// 1번째 부터 4번째까지 4bit : 옵션 종류	 11110000
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
	// 5번째 부터 8번째까지 4bit : 옵션 레벨	 00001111
	return pItem->m_JewelOfHarmonyOption&JEWELOFHARMONY_BITMASK_OPTIONLEVEL;
}

// 강화 되었는지 여부
BOOL	CJewelOfHarmonySystem::IsStrengthenByJewelOfHarmony(CItem *pItem)
{
	// 강화 옵션값이 0보다 큰 경우를 의미.
	if ( GetItemStrengthenOption( pItem ) > 0 )
		return TRUE;

	return FALSE;
}

// 유효 한지 체크
BOOL	CJewelOfHarmonySystem::IsActive(CItem *pItem)
{
	// 강화된 아이템이 아니면 비활성
	if ( IsStrengthenByJewelOfHarmony( pItem ) == FALSE )
		return FALSE;
	
	// 아이템의 레벨이 옵션레벨 보다 작은 경우 비활성
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

// 강화 옵션 분류 찾기 
int		CJewelOfHarmonySystem::_GetItemType(CItem *pItem)
{
	int iItemType = JEWELOFHARMONY_ITEM_TYPE_NULL;

	// 1 : 무기류
	if ( ( pItem->m_Type >= ITEM_SWORD  && pItem->m_Type < ITEM_BOW )					// 검(0), 도끼(1), 둔기(2), 창(3)
		|| ( pItem->m_Type >= ITEM_BOW	  && pItem->m_Type < ITEM_BOW+7)				// 활(4)
		|| ( pItem->m_Type >= ITEM_BOW+8  && pItem->m_Type < ITEM_BOW+15  )				// 석궁(4)
		|| ( pItem->m_Type >= ITEM_BOW+16 && pItem->m_Type < ITEM_BOW+MAX_ITEM_INDEX)	// 석궁(4)
		)
	{
		iItemType = JEWELOFHARMONY_ITEM_TYPE_WEAPON;
	}

	// 2 : 지팡이류
	if (  pItem->m_Type >= ITEM_STAFF  && pItem->m_Type < ITEM_SHIELD )				// 지팡이(5)
	{
		iItemType = JEWELOFHARMONY_ITEM_TYPE_STAFF;
	}

	// 3 : 방어구류
	if( pItem->m_Type >= ITEM_SHIELD && pItem->m_Type < ITEM_BOOT+MAX_ITEM_INDEX ) 	// 방어구 : 방패(6), 투구(7), 갑옷(8), 바지(9), 장값(10), 부츠(11)
	{
		iItemType = JEWELOFHARMONY_ITEM_TYPE_DEFENSE;
	}

	return iItemType;
}


// 랜덤하게 옵션 정하기
int		CJewelOfHarmonySystem::_GetSelectRandomOption(CItem *pItem, int iItemType)
{
	// 이외의 아이템은 무시 처리.
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return iItemType;

	// 값 초기화
	int iItemOption = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;
	m_kRandomPool.InitPool();

	// 유효한 옵션 찾기
	for (int idx = 1; idx < MAX_JEWELOFHARMONY_EFFECT_OPTION + 1; ++idx )
	{
		if ( m_itemOption[iItemType][idx].bValid == FALSE )
		{
			continue;
		}

		// 요구레벨
		if ( pItem->m_Level < m_itemOption[iItemType][idx].iRequireLevel )
		{
			continue;
		}

		// 필요 힘 감소 옵션이나, 힘 스탯이 필요하지 않는 아이템 인 경우.
		if ( iItemOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR
			|| iItemOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR )
		{
			if ( pItem->m_RequireStrength == 0 )
			{
				continue;
			}
		}

		// 필요 민첩 감소 옵션이나, 민첩 스탯이 필요하지 않는 아이템 인 경우.
		if ( iItemOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX
			|| iItemOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX )
		{
			if ( pItem->m_RequireDexterity == 0 )
			{
				continue;
			}
		}

		// 유효한 옵션 Pool에 넣기
		m_kRandomPool.AddValue( idx, m_itemOption[iItemType][idx].iRandomWeight);
	}

	// 추첨 방식에 따라 옵션 값 선택
	iItemOption = m_kRandomPool.GetRandomValue( eRANDOMPOOL_BY_WEIGHT );
	return iItemOption;
}


// 조화의보석 강화 시스템
BOOL	CJewelOfHarmonySystem::StrengthenItemByJewelOfHarmony(LPOBJECTSTRUCT lpObj, int source, int target)
{
	// 0. 기본 적인 체크
	if ( m_bSystemStrengthenItem == FALSE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3380), lpObj->m_Index, 1); // 조화의보석 강화 시스템 사용불가
#endif
		return FALSE;
	}

	if( source < 0 || source > MAX_INVENTORY-1 ) 
		return FALSE;
	if( target < 0 || target > MAX_INVENTORY-1 ) 
		return FALSE;
	
	// 아이템이 있는지 체크
	if( lpObj->pInventory[source].IsItem() == FALSE ) 
		return FALSE;
	if( lpObj->pInventory[target].IsItem() == FALSE ) 
		return FALSE;

	CItem *pSource = &lpObj->pInventory[source];
	CItem *pTarget = &lpObj->pInventory[target];

	// 1. 타겟 아이템이 이미 강화된 것인지 체크
	if ( IsStrengthenByJewelOfHarmony(pTarget) == TRUE )
	{
		LogAddTD("[JewelOfHarmony][Strengthen Item] Already Strengtened [%s][%s]",
			   lpObj->AccountID, 
			   lpObj->Name);
		return FALSE;
	}

	// 1.5 세트 아이템은 강화할 수 없다.
#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
	if ( pTarget->IsSetItem() )
#else
	if ( pTarget->IsSetItem() == TRUE )
#endif
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3372), lpObj->m_Index, 1); // 세트 아이템은 강화시킬 수 없습니다.
#endif
		LogAddTD("[JewelOfHarmony][Strengthen Item] SetItem not enable to Strengthened [%s][%s]",
			   lpObj->AccountID, 
			   lpObj->Name);
		return FALSE;
	}

	// 2. 타겟 아이템이 강화 가능한 아이템 인지 체크
	int iItemType = _GetItemType( pTarget );		// 무기 / 지팡이 /방어구
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		// 강화 가능한 아이템이 아님. 강화 실패 : 이런 경우는 나오면 안된다.
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

	// 3. 아이템 종류에 따른 옵션 부여
	int iItemOption			= _GetSelectRandomOption( pTarget, iItemType );			// 타입별 옵션 
	if ( iItemOption == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		// 옵션 찾기 실패. 강화 실패 : 이런 경우는 나오면 안된다.
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
	int iItemOptionLevel	= m_itemOption[iItemType][iItemOption].iRequireLevel;	// 옵션 값 설정 : 최소 레벨.

	// 4. 성공 확률 결정 및 적용
	int iSuccessRate = rand()%100;
	if ( iSuccessRate >= m_iRateStrengthenSuccess )
	{
		// 강화 실패
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
		GCServerMsgStringSend(lMsg.Get(3373), lpObj->m_Index, 1); // 조화의보석 강화 실패
#endif
		return TRUE;
	}


	// 5. 최종 강화 처리
	_MakeOption( pTarget, iItemOption, iItemOptionLevel );

	// 강화 성공
	// 6. 조화의 보석 강화 로그
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
	GCServerMsgStringSend(lMsg.Get(3374), lpObj->m_Index, 1); // 조화의보석 강화 성공
#endif

	// 테스트
#if TESTSERVER == 1
	ShowStrengthenOption( pTarget );
#endif
	
	// 뷰포트 업데이트
	gObjMakePreviewCharSet(lpObj->m_Index);

	// 내구도 재설정 : 변화 없음
	float levelitemdur =  (float)ItemGetDurability(lpObj->pInventory[target].m_Type, 
												   lpObj->pInventory[target].m_Level, 
												   lpObj->pInventory[target].IsExtItem(),
												   lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur*lpObj->pInventory[target].m_Durability/lpObj->pInventory[target].m_BaseDurability;

	// 6. 아이템 가격을 재설정한다	: 변화 없음.
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
	// 강화 옵션 처리
	pItem->m_JewelOfHarmonyOption = 0;
	pItem->m_JewelOfHarmonyOption |= btOptionType<<JEWELOFHARMONY_BITSHIFT_OPTION;		// 4 Bit : 옵션 
	pItem->m_JewelOfHarmonyOption |= btOptionLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL;	// 4 Bit : 옵션 레벨 

	// 테스트
#if TESTSERVER == 1
	ShowStrengthenOption( pItem );
#endif

	return TRUE;
}

void	CJewelOfHarmonySystem::StrengthenItemByMacro(LPOBJECTSTRUCT lpObj, BYTE invenrotyTargetPos, BYTE btOptionType, BYTE btOptionLevel)
{
	// 사내 테스트 서버인 경우에만 작동
#if TESTSERVER == 0
	return;
#endif

	CItem *pItem = &lpObj->pInventory[invenrotyTargetPos];
	// 1. 강화 체크
	if ( IsStrengthenByJewelOfHarmony( pItem ) == TRUE )
	{
		//return;
	}

	// 2. 옵션체크
	int iType = _GetItemType( pItem );
	if ( iType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		char szMsg[256];
		sprintf(szMsg, "강화할 수 있는 아이템이 아닙니다.");
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}

	// 3. 체크
	// 랜덤 테스트
	if ( btOptionType == 0xFF )
	{
		btOptionType = _GetSelectRandomOption( pItem, iType );
	}
	if ( btOptionType > MAX_JEWELOFHARMONY_EFFECT_OPTION )
	{
		char szMsg[256];
		sprintf(szMsg, "옵션 최대 값은 %d 입니다.", MAX_JEWELOFHARMONY_EFFECT_OPTION);
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}
    if ( btOptionLevel > MAX_JEWELOFHARMONY_EFFECT_VALUE - 1 )
	{
		char szMsg[256];
		sprintf(szMsg, "옵션 레벨의 범위는 0~%d 입니다.", MAX_JEWELOFHARMONY_EFFECT_VALUE - 1);
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}


	// 3.5 무기류 최소 공격증가인 경우 최대치를 넘지 않게 하기 위함.
	if ( iType == JEWELOFHARMONY_ITEM_TYPE_WEAPON 
		&& btOptionType == AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE )
	{
		// 적용 레벨값
		BYTE iValue = m_itemOption[JEWELOFHARMONY_ITEM_TYPE_WEAPON][AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE].iItemEffectValue[btOptionLevel];
		{
			if ( pItem->m_DamageMin + iValue > pItem->m_DamageMax - 1)
			{
				//서버메시지
				GCServerMsgStringSend("업션 적용 불가 (최소공격력이 최대공격력보다 커짐)", lpObj->m_Index, 1);
				return;
			}
		}
	}

	// 4. 옵션이 유효한지 체크
	if ( btOptionType != 0 )
	{
		// 옵션 타입 값이 0 인경우 환원인 경우임. 예외 처리.
		if ( m_itemOption[iType][btOptionType].bValid == FALSE )
		{
			char szMsg[256];
			sprintf(szMsg, "없는 옵션번호입니다.");
			GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
			return;
		}
	}
	else
	{
		// 환원 처리
		_MakeOption( pItem, 0,0);
		LogAddTD("[JewelOfHarmony][Restore Item] Macro Restore Success [%s][%s] Pos:%d",
		   lpObj->AccountID, lpObj->Name, 
		   invenrotyTargetPos );	

		char szMsg[256];
		sprintf(szMsg, "환원처리되었습니다.");
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		GCInventoryItemOneSend(lpObj->m_Index, invenrotyTargetPos);
		return;
	}

	// 5. 옵션 레벨 조정
	if ( pItem->m_Level < m_itemOption[iType][btOptionType].iRequireLevel )
	{
		char szMsg[256];
		sprintf(szMsg, "[%s] 강화 옵션 요구레벨은 아이템 레벨 %d 입니다.",m_itemOption[iType][btOptionType].szOptionName,
			m_itemOption[iType][btOptionType].iRequireLevel );
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}

//	if ( btOptionLevel > pItem->m_Level )
//		btOptionLevel = pItem->m_Level;

	if ( btOptionLevel < m_itemOption[iType][btOptionType].iRequireLevel )
	{
		char szMsg[256];
		sprintf(szMsg, "[%s] 강화 옵션의 최소 옵션레벨은 %d 입니다.", m_itemOption[iType][btOptionType].szOptionName,
			m_itemOption[iType][btOptionType].iRequireLevel );
		GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
		return;
	}

	// 6. 강화 처리
	_MakeOption( pItem, btOptionType, btOptionLevel );

	LogAddTD("[JewelOfHarmony][Strengthen Item] Macro Strengthen Success [%s][%s]  Option %d OptionLevel %d Pos:%d",
		   lpObj->AccountID, lpObj->Name, 
		   btOptionType,btOptionLevel, invenrotyTargetPos );	

	char szMsg[256];
	sprintf(szMsg, "[%s] 옵션레벨 %d  값 %d 으로 강화", m_itemOption[iType][btOptionType].szOptionName,
		btOptionLevel, m_itemOption[iType][btOptionType].iItemEffectValue[btOptionLevel]);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);

	GCInventoryItemOneSend(lpObj->m_Index, invenrotyTargetPos);
}


//--------------------------------------------------------------------
BYTE	CJewelOfHarmonySystem::ShowStrengthenOption(CItem *pItem)
{
	BYTE bResult = -1;

	// 강화 여부 조사
	if ( IsStrengthenByJewelOfHarmony( pItem ) == FALSE )
		return bResult;

	int type = _GetItemType( pItem );	// 아이템 분류
	bResult  = GetItemStrengthenOption( pItem );	// 1번째 부터 4번째까지 4bit : 옵션 종류
	int iItemOptionLevel = _GetItemOptionLevel( pItem );	// 옵션 레벨


	char buf[256];
	sprintf (buf, "효과 : %20s    type:%d, option:%d optionLevel %d value %d \n", m_itemOption[type][bResult].szOptionName, type, bResult,iItemOptionLevel
		, m_itemOption[type][bResult].iItemEffectValue[iItemOptionLevel]);
	OutputDebugString(buf);

	return bResult;
}

//--------------------------------------------------------------------
BYTE	CJewelOfHarmonySystem::MakeCharSetData( LPOBJECTSTRUCT lpObj )
{
	// 현재 사용하지 않음 (2006.8.8)
	
	BYTE btResult = 0;

	// 2 bit : 오른손 강화 무기. 이펙트 0 ~ 3단계. 활성화 되어 있어야함.
	if ( IsActive ( &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT] ) == TRUE )
	{
		int iOptionLevel = _GetItemOptionLevel( &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT] );
		if ( iOptionLevel > 5 && iOptionLevel < 9 )
			btResult |= 0x40;		// 6~8 레벨  : 0100 0000
		else if ( iOptionLevel > 8 && iOptionLevel < 13 )
			btResult |= 0x80;		// 9~12 레벨 : 1000 0000
		else if ( iOptionLevel > 12 )
			btResult |= 0xC0;		// 13 레벨   : 1100 0000
	}
	// 2 bit : 왼손 강화 무기. 이펙트 0 ~ 3단계. 활성화 되어 있어야함.
	if ( IsActive( &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT] ) == TRUE )
	{
		int iOptionLevel = _GetItemOptionLevel( &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT] );
		if ( iOptionLevel > 5 && iOptionLevel < 9 )
			btResult |= 0x10;		// 6~8 레벨  : 0001 0000
		else if ( iOptionLevel > 8 && iOptionLevel < 13 )
			btResult |= 0x20;		// 9~12 레벨 : 0010 0000
		else if ( iOptionLevel > 12 )
			btResult |= 0x30;		// 13 레벨   : 0011 0000
	}

	return btResult;
}

//--------------------------------------------------------------------
// 아이템 옵션 적용
// pItem : 대상 아이템
// pItemEffect : 아이템 효과가 적용된 값 
// Return Value 
//    - TRUE  : 적용 성공
//    - FALSE : 적용 실패
//    - pItemEffect : 적용된 아이템 효과 값
//--------------------------------------------------------------------
void	CJewelOfHarmonySystem::SetApplyStrengthenItem( LPOBJECTSTRUCT lpObj )
{
	// 1. 값 초기화
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
				// 로그
				OutputDebugString("착용 강화 아이템 적용----\n");
				ShowStrengthenOption( &lpObj->pInventory[iItemIndex] );
				OutputDebugString("착용 강화 아이템 적용끝----\n");
#endif
			}
		}
	}

	// 2. 1차 수치계산에 적용되는 값 적용
 // AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE		 = 1,	// 최소 공격력 상승 (값)
 // AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE		 = 2,	// 최대 공격력 상승 (값)
 // AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKDAMAGE_BOTH	 = 5,	// 공격력 상승 (최소,최대) (값)
 // AT_JEWELOFHARMONY_STAFF_IMPROVE_MAGICDAMAGE			 = 1,	// 마력 상승(값)
 // AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DEFENSE			 	 = 1,   // 방어력 상승 (값)
 // AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_AG				 = 2,	// 최대 AG 상승 (값)
 // AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_HP				 = 3,	// 최대 HP 상승 (값)
 // AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DAMAGE_DECREASE		 = 7,	// 데미지 감소 % 증가

	// 1 : 최소 공격력 상승, 5 : 공격력 상승
	lpObj->m_AttackDamageMinLeft	+= pItemEffect->HJOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinLeft	+= pItemEffect->HJOpAddAttackDamage;
	lpObj->m_AttackDamageMinRight	+= pItemEffect->HJOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinRight	+= pItemEffect->HJOpAddAttackDamage;

	// 2 : 최대 공격력 상승, 5 : 공격력 상승
	lpObj->m_AttackDamageMaxLeft 	+= pItemEffect->HJOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxLeft	+= pItemEffect->HJOpAddAttackDamage;
	lpObj->m_AttackDamageMaxRight	+= pItemEffect->HJOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxRight	+= pItemEffect->HJOpAddAttackDamage;

	// 로그 테스트 : 최소 공격력이 최대 공겨력보다 커지는 경우
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

	// 1 : 마력 상승
	lpObj->m_MagicDamageMin += pItemEffect->HJOpAddMagicPower;
	lpObj->m_MagicDamageMax += pItemEffect->HJOpAddMagicPower;

	// 1 : 방어력 상승 (값) - 세트아이템처럼 50%만 서버에는 적용.
	lpObj->m_Defense	+= ( pItemEffect->HJOpAddDefense*10/ 20);

	// 2 : 최대 AG 상승
	lpObj->AddBP		+= pItemEffect->HJOpAddMaxAG;

	// 3 : 최대 HP 상승
	lpObj->AddLife		+= pItemEffect->HJOpAddMaxHP;

	// 7 : 데미지 감소 % 증가
	lpObj->DamageMinus += pItemEffect->HJOpAddDamageDecrease;
}


int	CJewelOfHarmonySystem::GetItemEffectValue(CItem *pItem, int iOptionType)
{
	// 1. 아이템 분류
	int iItemType = _GetItemType(pItem);
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		return 0;
	}

	// 2. 아이템 효과 확인 : 같은 옵션인 경우만 검사
	int iItemOptionType = 0;
	iItemOptionType = GetItemStrengthenOption( pItem );
	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM 
		|| iItemOptionType != iOptionType )
	{
		return 0;
	}

	// 3. 옵션레벨에 따른 아이템 효과 값
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
	// 1. 아이템 분류
	int iItemType = _GetItemType(pItem);
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		return FALSE;
	}

	// 2. 아이템 효과 확인
	int iItemOptionType = 0;
	iItemOptionType = GetItemStrengthenOption( pItem );
	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		return FALSE;
	}
	// 3. 옵션레벨에 따른 아이템 효과 값
	int		iItemOptionLevel = _GetItemOptionLevel( pItem );
	if ( iItemOptionLevel >= MAX_JEWELOFHARMONY_EFFECT_VALUE )
	{
		return FALSE;
	}

	// 3.5 아이템의 레벨이 옵션 레벨보다 작은 경우 비활성 처리
	if ( IsActive( pItem ) == FALSE )
	{
		return FALSE;
	}
	
	int		iItemEffectValue = m_itemOption[iItemType][iItemOptionType].iItemEffectValue[iItemOptionLevel];

	// 4. 효과 적용
	BOOL bResult = TRUE;
	switch( iItemType )
	{
		// 무기류 아이템
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
					// 아이템 착용에 들어가는 옵션 : 따로 처리.
					break;
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX:
					// 아이템 착용에 들어가는 옵션 : 따로 처리.
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
		// 지팡이류
		case JEWELOFHARMONY_ITEM_TYPE_STAFF:
			{
				switch( iItemOptionType )
				{
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_MAGICDAMAGE:
					pItemEffect->HJOpAddMagicPower += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR:
					// 아이템 착용에 들어가는 옵션 : 따로 처리.
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX:
					// 아이템 착용에 들어가는 옵션 : 따로 처리.
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
		// 방어구류
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


// 효과값 초기화 : 캐릭터 수치 재 계산시에 반드시 호출
void	CJewelOfHarmonySystem::InitEffectValue( LPJEWELOFHARMONY_ITEM_EFFECT pItemEffect )
{
	pItemEffect->HJOpAddMinAttackDamage			= 0;	// 최소 공격력 증가 v
	pItemEffect->HJOpAddMaxAttackDamage			= 0;	// 최대 공격력 증가 v

	pItemEffect->HJOpRequireStr					= 0;	// 필요 힘 감소.  아이템에 적용.
	pItemEffect->HJOpRequireDex					= 0;	// 필요 민첩 감소. 아이템에 적용.
	
	pItemEffect->HJOpAddAttackDamage			= 0;	// 공격력 증가 ( 최소. 최대)
	pItemEffect->HJOpAddCriticalDamage			= 0;	// 크리티컬 데미지 증가 v
	pItemEffect->HJOpAddSkillAttack				= 0;	// 스킬 공격력 증가 v

	pItemEffect->HJOpAddAttackSuccessRatePVP	= 0;	// 공격 성공률 상승 PvP
	pItemEffect->HJOpDecreaseSDRate				= 0;	// 상대 실드 흡수율 감소
	pItemEffect->HJOpAddIgnoreSDRate			= 0;	// 정해진 확률 만큼 상대의 SD를 무시하고 직접 HP데미지.

	pItemEffect->HJOpAddMagicPower				= 0;	// 추가 마력 v

	pItemEffect->HJOpAddDefense					= 0;	// 추가 방어력 v
	pItemEffect->HJOpAddMaxAG					= 0;	// 최대 AG 상승
	pItemEffect->HJOpAddMaxHP					= 0;	// 최대 HP 상승
	pItemEffect->HJOpAddRefillHP				= 0;	// 생명력 자동 증가량 상승 : ??? 아이템이 있다면 추가?
	pItemEffect->HJOpAddRefillMP				= 0;	// 마나 자동 증가량 상승
	pItemEffect->HJOpAddDefenseSuccessRatePvP	= 0;	// 방어 성공률 상승 PvP
	pItemEffect->HJOpAddDamageDecrease			= 0;	// 데미지 감소량 상승..??
	pItemEffect->HJOpAddSDRate					= 0;	// 방어시 흡수하는 SD 비율 상승
}
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// 제련석 조합 가능 아이템 여부 확인
BOOL	CJewelOfHarmonySystem::IsEnableToMakeSmeltingStoneItem( CItem *pItem )
{
	// 1차 조건 : 무기류 / 지팡이류 / 방어구류만 해당하도록 설정 : 변경될 수 있음
	if ( _GetItemType( pItem ) == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		return FALSE;
	}

	// 2차 조건 : 강화된 아이템은 제련할 수 없다.
	if ( IsStrengthenByJewelOfHarmony( pItem ) == TRUE )
	{
		return FALSE;
	}

	// 2.5차 조건 : 세트 아이템은 제련할 수 없다.
#ifdef UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329
	if ( pItem->IsSetItem() )
#else
	if ( pItem->IsSetItem() == TRUE )
#endif
	{
		return FALSE;
	}

	// 3차 조건 : 요구레벨이 있는 경우
	map<int, int>::iterator iter = m_mapEnableMixList.find( pItem->m_Type );
	if ( iter != m_mapEnableMixList.end() )
	{
		int reqLevel = iter->second;
		if ( pItem->m_Level < reqLevel )
		{
#if TESTSERVER == 1
			LogAddTD("제련석 조합 불가 필요레벨 %d 아이템레벨 %d", reqLevel, pItem->m_Level);
#endif
			return FALSE;
		}

	}
		

	return TRUE;
}


// 제련석 조합
BOOL	CJewelOfHarmonySystem::MakeSmeltingStoneItem(LPOBJECTSTRUCT lpObj)
{
	// 시스템 사용 가능 여부
	if ( m_bSystemMixSmeltingStone != TRUE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3381), lpObj->m_Index, 1); // 제련조합시스템 사용 불가
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

	// 1. 카오스조합창 아이템 검사
	int iItemPos = -1;
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			// 제련가능한 아이템 확인
			if( IsEnableToMakeSmeltingStoneItem( &lpObj->pChaosBox[n] ) == TRUE )
			{	
				// 유효 아이템 갯수
				++iValidItemCount;
				iItemPos = n;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	// 반드시 1개의 아이템만 존재 해야함 : 유효한 아이템 1개, 유효하지 않은 아이템 0개, 아이템위치 0 이상
	if(  iValidItemCount != 1 || iInvalidItemCount != 0 || iItemPos == -1 )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	// 2. 일반 / 엑설런트 분류에 따른 확률 결정 : 일반 10%, 엑설런트 60%
	// 조합 조건 변수 설정
	int		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE;				// 제련석 조합 성공 확률
	BOOL	bIsItemNormal = TRUE;
	if ( lpObj->pChaosBox[iItemPos].IsExtItem() == TRUE )
	{
		bIsItemNormal = FALSE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE		= m_iRateMixSmeltingStoneExt;		// 제련석 조합 성공 확률. 엑설런트.
	}
	else
	{
		bIsItemNormal = TRUE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE		= m_iRateMixSmeltingStoneNor;		// 제련석 조합 성공 확률. 일반
	}

	
	// 3. 필요 젠 처리
	iMakeSmeltingStoneMixPrice = m_iZenForMixSmeltingStone;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// 카오스 시스템 세율적용
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

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	g_MixSystem.LogChaosItem( lpObj, "JewelOfHarmony][Smelt Item Mix" );
	LogAddTD("[JewelOfHarmony][Smelt Item Mix] - Mix Start");
#else
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
	ItemIsBufExOption(ExOption, &lpObj->pChaosBox[iItemPos]);
	// 제련에 사용된 아이템 로그 출력
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


	// 4. 제련 확률에 따른 처리
	int iRate = rand()%100;
	if ( iRate < JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE )
	{
		// 제련석 생성
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
	{	// 조합실패	
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

// 제련석에 의한 강화
BOOL	CJewelOfHarmonySystem::SmeltItemBySmeltingStone(LPOBJECTSTRUCT lpObj, int source, int target)
{
	// 0. 기본 적인 체크
	if ( m_bSystemSmeltingItem == FALSE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3379), lpObj->m_Index, 1); // 제련석 강화 시스템 사용 불가
#endif
		return FALSE;
	}
	if( source < 0 || source > MAX_INVENTORY-1 ) 
		return FALSE;
	if( target < 0 || target > MAX_INVENTORY-1 ) 
		return FALSE;
	
	// 아이템이 있는지 체크
	if( lpObj->pInventory[source].IsItem() == FALSE ) 
		return FALSE;
	if( lpObj->pInventory[target].IsItem() == FALSE ) 
		return FALSE;

	CItem *pSource = &lpObj->pInventory[source];
	CItem *pTarget = &lpObj->pInventory[target];

	// 1. 타겟 아이템이 강화 된 것인지 체크
	if ( IsStrengthenByJewelOfHarmony(pTarget) == FALSE )
	{
		LogAddTD("[JewelOfHarmony][Smelt Item] Not Strengthen Item [%s][%s]",
			   lpObj->AccountID, 
			   lpObj->Name);
		return FALSE;
	}

	// 2. 강화 레벨 체크
	int iItemOptionLevel = _GetItemOptionLevel( pTarget ); // 현재 강화 옵션 레벨
	if ( iItemOptionLevel >= pTarget->m_Level )	
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		if ( iItemOptionLevel == 13 ) // 최고 옵션 레벨
		{
			GCServerMsgStringSend(lMsg.Get(3369), lpObj->m_Index, 1); // 강화 가능한 최대 수치가 적용 중입니다. 더 이상 강화할 수 없습니다.
		}
		else
		{
			GCServerMsgStringSend(lMsg.Get(3368), lpObj->m_Index, 1); // 아이템 레벨이 낮아 더 이상 강화할 수 없습니다.
		}
#endif 
		LogAddTD("[JewelOfHarmony][Smelt Item] Already Have Max OptionLevel [%s][%s] OptionLevel [%d] ItemLevel [%d]",
			   lpObj->AccountID, 
			   lpObj->Name, 
			   iItemOptionLevel,
			   pTarget->m_Level);	
		return FALSE;
	}

	// 2.5 무기류 최소 공격력 상승의 경우 최대 공격력-1 값을 넘을 수 없다.
	if ( _GetItemType( pTarget ) == JEWELOFHARMONY_ITEM_TYPE_WEAPON 
		&& GetItemStrengthenOption( pTarget ) == AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE )
	{
		// 다음 레벨값
		int iNextLevel = _GetItemOptionLevel( pTarget ) +1;
		BYTE iValue = m_itemOption[JEWELOFHARMONY_ITEM_TYPE_WEAPON][AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE].iItemEffectValue[iNextLevel];
		{
			if ( pTarget->m_DamageMin + iValue > pTarget->m_DamageMax - 1)
			{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
				GCServerMsgStringSend(lMsg.Get(3369), lpObj->m_Index, 1); // 강화 가능한 최대 수치가 적용 중입니다. 더이상 강화할 수 없습니다.
#endif
				return FALSE;
			}
		}
	}

	// 3. 성공 확률 : 상급/하급 제련석에 따라 확률 변경
	int iSuccessRate = rand()%100;
	int iRateSmeltingSuccess = 0;
	BOOL bIsNormalSmeltingStone = _IsJewelOfHarmonySmeltingItemNor( pSource->m_Type );
	if ( bIsNormalSmeltingStone == TRUE )
		iRateSmeltingSuccess = m_iRateSmeltingSuccessNor;
	else
		iRateSmeltingSuccess = m_iRateSmeltingSuccessExt;

	if ( iSuccessRate >= iRateSmeltingSuccess )
	{
		// 현재 강화 옵션 레벨 변경
		int iItemOptionNewLevel = _GetItemOptionRequireLevel( pTarget );	// 옵션 레벨 설정 : 최소 레벨
		pTarget->m_JewelOfHarmonyOption  = pTarget->m_JewelOfHarmonyOption&JEWELOFHARMONY_BITMASK_OPTION;		// 옵션 4Bit 값 0 변경
		pTarget->m_JewelOfHarmonyOption |= (iItemOptionNewLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL);			// 4 Bit : 옵션 레벨 

		// 강화업그레이드 실패
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
		GCServerMsgStringSend(lMsg.Get(3375), lpObj->m_Index, 1); // 제련석 강화 실패
#endif
	}
	else
	{
		// 현재 강화 옵션 레벨 변경
		int iItemOptionNewLevel = iItemOptionLevel + 1;
		pTarget->m_JewelOfHarmonyOption  = pTarget->m_JewelOfHarmonyOption&JEWELOFHARMONY_BITMASK_OPTION;		// 옵션 4Bit 값 0 변경
		pTarget->m_JewelOfHarmonyOption |= (iItemOptionNewLevel&JEWELOFHARMONY_BITMASK_OPTIONLEVEL);					// 4 Bit : 옵션 레벨 

		// 강화업그레이드 성공
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
		GCServerMsgStringSend(lMsg.Get(3376), lpObj->m_Index, 1); // 제련석 강화 성공
#endif
	}

	return TRUE;
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
// 환원 시스템
int		CJewelOfHarmonySystem::_GetZenForRestoreItem(CItem *pItem)
{
	// 1. 아이템 분류
	int iItemType = _GetItemType(pItem);
	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		return -1;
	}

	// 2. 아이템 효과 확인 : 같은 옵션인 경우만 검사
	int iItemOptionType = 0;
	iItemOptionType = GetItemStrengthenOption( pItem );
	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		return -1;
	}

	// 3. 옵션레벨에 따른 아이템 효과 값
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
	// 시스템 사용 가능 여부
	if ( m_bSystemRestoreStrengthen != TRUE )
	{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3382), lpObj->m_Index, 1); // 환원시스템 사용불가
#endif
		return TRUE;

	}
	
	// ChaosBox Lock
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	// 1. 카오스조합창 아이템 검사 : 강화된 아이템을 찾는다.
	// 조건도 넣어야함 : 이외에 아이템 조사도 넣어야함. 테스트를 위해
	// 넣지 않는다.
	
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

	// 2. 젠 체크
	int iItemOption		 = GetItemStrengthenOption( pItem );
	int iItemOptionLevel = _GetItemOptionLevel( pItem );

	int JEWEL_OF_HARMONY_RETORE_NEEDZEN;
	JEWEL_OF_HARMONY_RETORE_NEEDZEN = _GetZenForRestoreItem( pItem );	// 환원에 필요한 젠.
	if ( JEWEL_OF_HARMONY_RETORE_NEEDZEN < 0 )
	{
		// 환원에 필요한 젠 설정 이상.
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// 카오스 시스템 세율적용
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
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	g_MixSystem.LogChaosItem( lpObj, "JewelOfHarmony][Restore Item" );
	LogAddTD("[JewelOfHarmony][Restore Item] - Restore Start");
#endif

	// 3. 환원 처리
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
	// 환원
	pItem->m_JewelOfHarmonyOption = 0;

	// 카오스박스 내용 전송
	GCUserChaosBoxSend(lpObj);

	ShowStrengthenOption( pItem );

	lpObj->ChaosLock = FALSE;
	
	return TRUE;
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
// NPC 처리
extern BOOL bCanChaosBox;

BOOL CJewelOfHarmonySystem::NpcJewelOfHarmony(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
#ifdef BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	if( lpObj->m_IfState.use > 0 )
	{	// 창을 사용하고 있을때는.. 사용할수 없다
		return TRUE;
	}
#endif

	PMSG_TALKRESULT			pResult;
	pResult.h.c				= PMHCE_BYTE;
	pResult.h.headcode		= 0x30;
	pResult.h.size			= sizeof( pResult );

	if ( lpNpc->Class == JEWELOFHARMONY_NPC_PURITY )
	{
		pResult.result			= 0x11;						// 정제 NPC
		pResult.level1			= m_iRatePuritySuccess;		// 정제 성공 확률

		// 시스템 사용 가능 여부
		if ( m_bSystemPrutiyJewel != TRUE )
		{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
			GCServerMsgStringSend(lMsg.Get(3378), lpObj->m_Index, 1);		// 정제시스템 이용 불가
#endif
			return TRUE;

		}
		// 현재 가능한지 여부 파악
		if ( IsEnableToUsePuritySystem() == FALSE )
		{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
			GCServerMsgStringSend(lMsg.Get(3378), lpObj->m_Index, 1);		// 정제시스템 이용 불가
#endif
			LogAddTD("[JewelOfHarmony][%s][%s] Not Purtiy Time ", lpObj->AccountID, lpObj->Name);
			return TRUE;
		}
	}
	else if ( lpNpc->Class == JEWELOFHARMONY_NPC_SMELT )
	{
		pResult.result			= 0x12;		// 제련 NPC
		pResult.level1			= m_iRateMixSmeltingStoneNor;	// 일반아이템 조합 성공 확률
		pResult.level2			= m_iRateMixSmeltingStoneExt;	// 엑설런트아이템 조합 성공 확률

		// 시스템 사용 가능 여부
		if ( m_bSystemMixSmeltingStone != TRUE )
		{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
			ChatTargetSend(lpNpc, lMsg.Get(3381), lpObj->m_Index);   // 제련조합시스템 사용 불가
#endif
			return TRUE;

		}
	}
	else if ( lpNpc->Class == JEWELOFHARMONY_NPC_RESTORE )
	{
		pResult.result			= 0x13;		// 환원 NPC
		// 시스템 사용 가능 여부
		if ( m_bSystemRestoreStrengthen != TRUE )
		{
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
			ChatTargetSend(lpNpc, lMsg.Get(3382), lpObj->m_Index);   // 아이템 환원 시스템 사용 불가
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
#ifdef PERSONAL_SHOP_20040113		// 자신이나 상대방이 개인상점을 개설중이면 카오스 조합을 할 수 없다.
			if (lpObj->m_bPShopOpen == true) 
			{
				LogAdd("[JewelOfHarmony][%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
				GCServerMsgStringSend(lMsg.Get(1218), lpObj->m_Index, 1);		// "개인상점 개설 중에서는 카오스 조합을 사용하실 수 없습니다."
				return TRUE;
			}
#endif
			
		lpObj->m_IfState.type   = I_CHAOSBOX;
		lpObj->m_IfState.state	= 0;
		lpObj->m_IfState.use    = 1;

#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
		lpObj->bIsChaosMixCompleted = false;			// 카오스 조합 전에 '아직 조합이 완료되지 않았다' 로 초기값을 세팅
#endif

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		gObjInventoryTrans(lpObj->m_Index);
		LogAddTD("[JewelOfHarmony][%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219		// 현재 적용된 세율 (카오스조합) 을 보내준다.
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
			GCAnsCsMapSvrTaxInfo (lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
#endif
#endif

	}
		
	return TRUE;	
}


// 거래 시 강화아이템을 포함하고 있으면 거래 불가
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