// ItemSystemFor380.cpp: implementation of the CItemSystemFor380 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Common\\winutil.h"
#include "ItemSystemFor380.h"

#ifdef MODIFY_MIX_SYSTEM_20070518
//#include "ChaosBox.h"
#include "MixSystem.h"
#endif

#include "DSProtocol.h"
#include "..\\Include\\ReadScript.h"

#include "DirPath.h"
extern CDirPath		gDirPath;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219
	#include "CastleSiegeSync.h"
#endif


#ifdef ADD_380ITEM_NEWOPTION_20060711

CItemSystemFor380	g_kItemSystemFor380;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemSystemFor380::CItemSystemFor380()
{
	memset(&m_itemOption, 0, sizeof(m_itemOption));
}

CItemSystemFor380::~CItemSystemFor380()
{

}

void CItemSystemFor380::_InitOption()
{
	memset(&m_itemOption, 0, sizeof(m_itemOption));
	m_iNeedJewelOfHarmonyCount = 1;						// 조화의보석 수
	m_iNeedJewelOfSuhoCount	   = 1;						// 수호의보석 수
	m_iNeedZenFor380Option     = 10000000;				// 필요젠
	m_iRateSuccessRateForMix1   = 50;					// 380 옵션 아이템 조합 확률
	m_iRateSuccessRateForMix2   = 60;					// 380 옵션 아이템 조합 확률
	m_iRateSuccessRateForMix3   = 100;					// 380 옵션 아이템 조합 확률

	// 환경 변수 : 확률 및 필요 젠관련
	m_bSystemFor380ItemOption  = GetPrivateProfileInt("GameServerInfo",	"System380ItemSystem",				0,	gDirPath.GetNewPath("commonserver.cfg")); 
	m_iNeedJewelOfHarmonyCount = GetPrivateProfileInt("GameServerInfo",	"CountOfJewelOfHarmonyForMix",		1,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iNeedJewelOfSuhoCount	   = GetPrivateProfileInt("GameServerInfo",	"CountOfSuhoOfHarmonyForMix",		1,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iNeedZenFor380Option	   = GetPrivateProfileInt("GameServerInfo",	"NeedZenForMix",			 10000000,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateSuccessRateForMix1  = GetPrivateProfileInt("GameServerInfo",	"MixRateForGrade1",					50,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateSuccessRateForMix2  = GetPrivateProfileInt("GameServerInfo",	"MixRateForGrade2",					60,	gDirPath.GetNewPath("commonserver.cfg"));
	m_iRateSuccessRateForMix3  = GetPrivateProfileInt("GameServerInfo",	"MixRateForGrade3",					60,	gDirPath.GetNewPath("commonserver.cfg"));

	// 테스트
#if TESTSERVER == 1
	char buf[256];
	sprintf(buf, "System380ItemSystem  %d \n", m_bSystemFor380ItemOption); OutputDebugString(buf);
	sprintf(buf, "CountOfJewelOfHarmonyForMix  %d \n", m_iNeedJewelOfHarmonyCount); OutputDebugString(buf);
	sprintf(buf, "CountOfSuhoOfHarmonyForMix  %d \n", m_iNeedJewelOfSuhoCount); OutputDebugString(buf);
	sprintf(buf, "NeedZenForMix  %d \n", m_iNeedZenFor380Option); OutputDebugString(buf);
	sprintf(buf, "MixRateForGrade1  %d \n", m_iRateSuccessRateForMix1); OutputDebugString(buf);
	sprintf(buf, "MixRateForGrade2  %d \n", m_iRateSuccessRateForMix2); OutputDebugString(buf);
	sprintf(buf, "MixRateForGrade3  %d \n", m_iRateSuccessRateForMix3); OutputDebugString(buf);
#endif
}

BOOL CItemSystemFor380::Load380ItemOptionInfo(char* filename)
{
	if((SMDFile=fopen(filename,"r")) == NULL)	
		return FALSE;

	_InitOption();
	
	SMDToken Token;
	while(true)
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			int Index = (int)TokenNumber;
			// 380 아이템 옵션
			while ( true )
			{
				if ( Index == 0 )
				{
					Token = (*GetToken)();
					int _type  = (int)TokenNumber;		// Type
					if(Token==NAME && strcmp("end",TokenString)==NULL) 
#ifdef ITEM_ADD_OPTION_20061019
						return TRUE;
#else
						break;
#endif // ITEM_ADD_OPTION_20061019
					Token = (*GetToken)();
					int _index = (int)TokenNumber;		// Index

					ITEMOPTION_FOR380ITEM *p = &m_itemOption[_type*MAX_ITEM_INDEX+_index];
					p->IsLoad = TRUE;
					p->m_Type	= _type;
					p->m_Index	= _index;
					Token = (*GetToken)();
					p->m_Option1 = (int)TokenNumber;	// Option1
					Token = (*GetToken)();
					p->m_Value1 = (int)TokenNumber;		// Value1
					Token = (*GetToken)();
					p->m_Option2 = (int)TokenNumber;	// Option2
					Token = (*GetToken)();
					p->m_Value2 = (int)TokenNumber;		// Value2

	#ifdef ITEM_ADD_OPTION_20061019
					int iTime;
					Token = (*GetToken)();				// Time 컬럼이 추가되었다. 380옵션에서는 무시
					iTime = (int)TokenNumber;
	#endif // ITEM_ADD_OPTION_20061019
			#if TESTSERVER == 1	
					char msg[512];
					wsprintf(msg, "%d\t%d\t%d\t%d\t%d\t%d\n",
							 p->m_Type,
							 p->m_Index,
							 p->m_Option1,
							 p->m_Value1,
							 p->m_Option2,
							 p->m_Value2);
					OutputDebugString(msg);
			#endif
				} // end if
			} // while
		} // end if
	}

	fclose(SMDFile);

	return TRUE;
}

// 380 아이템 확인
BOOL CItemSystemFor380::Is380Item( const CItem* pItem)
{
	if( pItem->m_Type < 0 || pItem->m_Type > MAX_ITEM-1 ) 
	{
		LogAdd("[380Item] OptionItem Check error: (iItemNum:%d)", pItem->m_Type);
		return FALSE;
	}
	
	// 1. 380 옵션 테이블에 해당 아이템이 있는지 확인.
	if ( m_itemOption[pItem->m_Type].IsLoad == FALSE )
		return FALSE;

	return TRUE;
}

// 380 옵션 잇는 아이템인지 확인
BOOL CItemSystemFor380::Is380OptionItem( const CItem* pItem)
{
	// 1. 380 아이템 확인
	if ( Is380Item( pItem ) == FALSE )
		return FALSE;

	// 2. 380 아이템이지만, 옵션이 붙었는지 확인.
	BYTE bResult = (pItem->m_ItemOptionEx&0x80) >> 7;
	if ( bResult != 1 )
	{
		return FALSE;
	}
	
	return TRUE;
}

void CItemSystemFor380::InitEffectValue( LPITEMOPTION_FOR380ITEM_EFFECT pItemEffect )
{
	pItemEffect->OpAddAttackSuccessRatePVP	= 0;	// 대인 공격 성공률 상승
	pItemEffect->OpAddDamage			    = 0;	// 대인 데미지증가 +%d

	pItemEffect->OpAddDefenseSuccessRatePvP	= 0;	// 대인 방어 성공률 상승
	pItemEffect->OpAddDefense				= 0;	// 대인 방어력 +%d
	
	pItemEffect->OpAddMaxHP				    = 0;	// 최대 AG 상승
	pItemEffect->OpAddMaxSD					= 0;	// 최대 SD 상승
	pItemEffect->OpRefillOn					= 0;	// SD자동 회복 On : 안전지대가 아니더라도 회복.
	pItemEffect->OpAddRefillSD				= 0;	// SD자동 증가량 상승
}

BOOL CItemSystemFor380::ApplyFor380Option(LPOBJECTSTRUCT lpObj)
{
	// 1. 값 초기화
	LPITEMOPTION_FOR380ITEM_EFFECT pItemEffect = &lpObj->m_ItemOptionExFor380;
	InitEffectValue( pItemEffect );

	// 2. 옵션 적용 값 계산
	int iItemIndex = 0;
	for( iItemIndex = 0; iItemIndex < MAX_EQUIPMENT; iItemIndex++ )
	{
		CItem *pItem = &lpObj->pInventory[iItemIndex];

		if( pItem->IsItem() && pItem->m_IsValidItem && Is380OptionItem( pItem ) == TRUE )
		{
#ifdef MODIFY_380ITEM_SCRIPTTYPE_EXTEND_20060811
			BYTE iOption1, iOption2;
			WORD iValue1,  iValue2;
			iOption1 = m_itemOption[pItem->m_Type].m_Option1;
			iValue1  = m_itemOption[pItem->m_Type].m_Value1;
			iOption2 = m_itemOption[pItem->m_Type].m_Option2;
			iValue2  = m_itemOption[pItem->m_Type].m_Value2;
#else
			BYTE iOption1, iOption2, iValue1, iValue2;
			iOption1 = m_itemOption[pItem->m_Type].m_Option1;
			iValue1  = m_itemOption[pItem->m_Type].m_Value1;
			iOption2 = m_itemOption[pItem->m_Type].m_Option2;
			iValue2  = m_itemOption[pItem->m_Type].m_Value2;
#endif 
			BOOL bResult = FALSE;
			bResult =  _CalcItemEffectValue( iOption1, iValue1, pItemEffect );
			bResult |= _CalcItemEffectValue( iOption2, iValue2, pItemEffect );
		}
	}

	// 즉시 적용 옵션
	lpObj->AddLife		+= pItemEffect->OpAddMaxHP;		// 최대 HP 상승
	lpObj->iAddShield	+= pItemEffect->OpAddMaxSD;		// 최대 SD 상승

	return TRUE;
}

BOOL CItemSystemFor380::_CalcItemEffectValue(int iItemOptionType, int iItemEffectValue, LPITEMOPTION_FOR380ITEM_EFFECT pItemEffect)
{
	if ( iItemOptionType == ITEMOPTION_FOR380ITEM_NULL )
	{
		return FALSE;
	}

	BOOL bResult = TRUE;
	switch( iItemOptionType )
	{
	case ITEMOPTION_FOR380ITEM_ATTACKSUCCESRATE_PVP:
		pItemEffect->OpAddAttackSuccessRatePVP += iItemEffectValue;
		DebugLog("380효과 : 공격성공율상승 %d\n", iItemEffectValue);
		break;
	case ITEMOPTION_FOR380ITEM_IMPROVE_DAMAGE_PVP:
		pItemEffect->OpAddDamage += iItemEffectValue;
		DebugLog("380효과 : 데미지 상승 %d\n", iItemEffectValue);
		break;
	case ITEMOPTION_FOR380ITEM_BLOCKSUCCESRATE_PVP:
		pItemEffect->OpAddDefenseSuccessRatePvP += iItemEffectValue;
		DebugLog("380효과 : 방어성공율상승 %d\n", iItemEffectValue);
		break;
	case ITEMOPTION_FOR380ITEM_IMPROVE_DEFENSE_PVP:
		pItemEffect->OpAddDefense += iItemEffectValue;
		DebugLog("380효과 : 방어력 상승 %d\n", iItemEffectValue);
		break;
	case ITEMOPTION_FOR380ITEM_IMPROVE_MAXHP:
		pItemEffect->OpAddMaxHP += iItemEffectValue;
		DebugLog("380효과 : 최대 HP 상승 %d\n", iItemEffectValue);
		break;
	case ITEMOPTION_FOR380ITEM_IMPROVE_MAXSD:
		pItemEffect->OpAddMaxSD  += iItemEffectValue;
		DebugLog("380효과 : 최대 SD 상승 %d\n", iItemEffectValue);
		break;
	case ITEMOPTION_FOR380ITEM_RECOVER_SD:
		pItemEffect->OpRefillOn  += 1; //iItemEffectValue;
		DebugLog("380효과 : SD 자동 회복 %d\n", iItemEffectValue);
		break;
	case ITEMOPTION_FOR380ITEM_IMPROVE_REFILLSD:
		pItemEffect->OpAddRefillSD += iItemEffectValue;
		DebugLog("380효과 : SD자동회복량 증가 %d\n", iItemEffectValue);
		break;
	default:
		bResult = FALSE;
		break;
	}

	return bResult;	
}


// 해당 아이템에 380 옵션 추가
BOOL CItemSystemFor380::_SetOption(CItem *pItem, BOOL bOption)
{
	// 1. 380 아이템 확인
	if ( Is380Item( pItem ) == FALSE )
	{
		return FALSE;
	}

	// 2. 값 설정
	if ( bOption == TRUE )
	{
		pItem->m_ItemOptionEx |= 0x80;		// m_ItemOptionEx의 최상위 1 bit 값 1로 설정
	}
	else
	{
		pItem->m_ItemOptionEx &= 0x7F;		// m_ItemOptionEx의 최상위 1 bit 값 0로 설정
	}

	return TRUE;
}

// 매크로를 이용한 옵션추가
void CItemSystemFor380::SetOptionItemByMacro(LPOBJECTSTRUCT lpObj, BYTE invenrotyTargetPos, BOOL bOption)
{
	// 사내 테스트인 경우만 처리
#if TESTSERVER == 0
	return;
#endif

	CItem *pItem = &lpObj->pInventory[invenrotyTargetPos];
	// 1. 380 아이템 체크
	if ( Is380Item( pItem ) == FALSE )
	{
		return;
	}

	// 2. 옵션적용
	_SetOption( pItem, bOption );

	LogAddTD("[380Item][Macro] 380Item Set Option [%d]  [%s][%s]",
		bOption,
		lpObj->AccountID, lpObj->Name);	

	DebugLog("[380Item][Macro] 380Item Set Option [%d]  [%s][%s]\n",
		bOption,
		lpObj->AccountID, lpObj->Name);

	//GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
	GCInventoryItemOneSend(lpObj->m_Index, invenrotyTargetPos);
}

// 380 아이템 옵션 조합
BOOL CItemSystemFor380::ChaosMix380ItemOption(LPOBJECTSTRUCT lpObj)
{
	// 시스템 사용 가능 여부
	if ( m_bSystemFor380ItemOption != TRUE )
	{
		//380서버메시지
#ifdef ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818
		GCServerMsgStringSend(lMsg.Get(3383), lpObj->m_Index, 1); // "380 아이템 옵션 조합 시스템 이용 불가"

	#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
		// 조합 버튼을 누른 것이지만, 조합이 되지 않기 때문에 다시 누르지 않은 상태로 돌려야 한다.
		lpObj->bIsChaosMixCompleted	= false;		
	#endif

#endif
		
		return FALSE;
	}

	// ChaosBox Lock
	lpObj->ChaosLock = TRUE;

	int iValidItemCount = 0;				// 380 아이템
	int iJewelOfHarmony = 0;				// 조화의보석
	int iJewelOfSuho	= 0;				// 수호의보석

	int iInvalidItemCount = 0;			
	int iMixPrice = 0;

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	int iCharmOfLuckCount = 0;
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	
#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
	BYTE ExOption[8];
	#endif
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	// 1. 카오스조합창 아이템 검사
	CItem *pTargetItem = NULL;
	int iPosOfJewelOfHarmony = -1;
	int iPosOfJewelOfSuho	 = -1;
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			// 380아이템이면서, 옵션은 없는 아이템. 레벨은 4이상. 추가옵션이 4이상. 이어야 한다.
			if( Is380Item( &lpObj->pChaosBox[n] ) == TRUE 
				&& Is380OptionItem( &lpObj->pChaosBox[n] ) == FALSE
				&& lpObj->pChaosBox[n].m_Level > 3 
				&& lpObj->pChaosBox[n].m_Option3*4 > 3)
			{	
				
				// 유효 아이템 갯수
				++iValidItemCount;
				pTargetItem = &lpObj->pChaosBox[n];
			}
			else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity( lpObj->pChaosBox[n].m_Type ) == TRUE )
			{
				++iJewelOfHarmony;
				iPosOfJewelOfHarmony = n;
			}
			else if ( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 31) )
			{
				++iJewelOfSuho;
				iPosOfJewelOfSuho = n;
			}
#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14 , 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
			else
			{
				++iInvalidItemCount;
			}
#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
			// 380 아이템 조합시 조합창에 넣은 아이템 리스트 출력
			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[380Item][Item Mix] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d] Serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d] HO:[%d,%d]",
		#else
			LogAddTD("[380Item][Item Mix] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d] Serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d] HO:[%d,%d]",
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
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
		}
	}

	// 380아이템 1개. 수호보석 1개. 조화보석 1개.
#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	if(  iInvalidItemCount != 0 
		|| iValidItemCount != 1 
		|| iJewelOfHarmony != 1
		|| iJewelOfSuho    != 1 
		|| pTargetItem == NULL
		|| iPosOfJewelOfHarmony == -1
		|| iPosOfJewelOfSuho == -1
		|| iCharmOfLuckCount > 10 
		)
#else
	if(  iInvalidItemCount != 0 
		|| iValidItemCount != 1 
		|| iJewelOfHarmony != 1
		|| iJewelOfSuho    != 1 
		|| pTargetItem == NULL
		|| iPosOfJewelOfHarmony == -1
		|| iPosOfJewelOfSuho == -1 )
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
	
	// 2. 필요 젠 처리
	iMixPrice = m_iNeedZenFor380Option;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// 카오스 시스템 세율적용
	INT iChaosTaxMoney = (INT)(iMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100);
	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	iMixPrice = iMixPrice + iChaosTaxMoney;

	if (iMixPrice < 0)
	{
		iMixPrice = 0;
	}
#endif

	if( lpObj->Money < iMixPrice )
	{
		pMsg.Result = 0x02;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	lpObj->Money = lpObj->Money - iMixPrice;

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	g_MixSystem.LogChaosItem( lpObj, "380Item][Item Mix");
	LogAddTD("[380Item][Item Mix] - Mix Start");
#endif

	// 3. 확률 처리
	int iRate = rand()%100;
	int iRateSuccess = m_iRateSuccessRateForMix1;		// 단계1 : 레벨 4 ~ 6

	if ( pTargetItem->m_Level < 7)						// 단계1 : 레벨 4 ~ 6
		iRateSuccess = m_iRateSuccessRateForMix1;		
	else if ( pTargetItem->m_Level < 10 )				// 단계2 : 레벨 7 ~ 9 
		iRateSuccess = m_iRateSuccessRateForMix2;
	else if ( pTargetItem->m_Level < 14 )				// 단계3 : 레벨 10 ~ 3
		iRateSuccess = m_iRateSuccessRateForMix3;

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	iRateSuccess += iCharmOfLuckCount;
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111

	// 조화의보석 삭제
	lpObj->pChaosBox[iPosOfJewelOfHarmony].Clear();
	lpObj->pChaosBoxMap[iPosOfJewelOfHarmony] = 0xFF;
	// 수호보석 삭제
	lpObj->pChaosBox[iPosOfJewelOfSuho].Clear();
	lpObj->pChaosBoxMap[iPosOfJewelOfSuho] = 0xFF;

	if ( iRate < iRateSuccess )
	{
		// 옵션 생성
		_SetOption( pTargetItem, TRUE );

		// 카오스박스 내용 전송
		GCUserChaosBoxSend(lpObj);

#if TESTSERVER == 1
		DebugLog("강화 성공 확률 %d/%d\n", iRate, iRateSuccess);
#endif

#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
		g_MixSystem.LogChaosItem( lpObj, "[380Item][Item Mix]");
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
		
#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
		LogAddTD("[380Item][ItemMix] Mix Success [%s][%s], Money(%d-%d) Rate(%d/%d) Option(%d,%d) OptionValue(%d,%d)",
			lpObj->AccountID, lpObj->Name,
			lpObj->Money, iMixPrice,
			iRate, iRateSuccess,
			m_itemOption[pTargetItem->m_Type].m_Option1, m_itemOption[pTargetItem->m_Type].m_Option2,
			m_itemOption[pTargetItem->m_Type].m_Value1, m_itemOption[pTargetItem->m_Type].m_Value2);
#else
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
		// 380아이템 조합 성공시 붙은 옵션을 출력한다.
		LogAddTD("[380Item][ItemMix] Mix Success [%s][%s], Money(%d-%d) Rate(%d/%d) Option(%d,%d,%d,%d)",
			lpObj->AccountID, lpObj->Name,
			lpObj->Money, iMixPrice,
			iRate, iRateSuccess,
			m_itemOption[pTargetItem->m_Type].m_Option1, m_itemOption[pTargetItem->m_Type].m_Value1,
			m_itemOption[pTargetItem->m_Type].m_Option2, m_itemOption[pTargetItem->m_Type].m_Value2);
	#else
		LogAddTD("[380Item][ItemMix] Mix Success [%s][%s], Money(%d-%d) Rate(%d/%d)",
			lpObj->AccountID, lpObj->Name, 
			lpObj->Money, iMixPrice, 
			iRate, iRateSuccess);
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
	}
	else
	{	
		// 옵션 생성 실패
#if TESTSERVER == 1
		DebugLog("강화 실패 : 확률 %d/%d\n", iRate, iRateSuccess);
#endif

		// 카오스박스 내용 전송
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[380Item][ItemMix] Mix Fail [%s][%s], Money(%d-%d) Rate(%d/%d)",
			lpObj->AccountID, lpObj->Name, 
			lpObj->Money, iMixPrice, 
			iRate, iRateSuccess);
	}

	lpObj->ChaosLock = FALSE;

	return TRUE;	
}

#endif // ADD_380ITEM_NEWOPTION_20060711